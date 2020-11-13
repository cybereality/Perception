/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/
#ifndef AQU_PNG_DECODER
#define AQU_PNG_DECODER

#ifndef PNG_DECODER_APP_NAME
#define PNG_DECODER_APP_NAME L"PNG Decoder"
#endif
#define DEFLATE_DECOMPRESSOR_APP_NAME PNG_DECODER_APP_NAME

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "AQU_Deflate.h"

#pragma region PNG info field
/**
* Basic PNG image data as stored in the IHDR chunk.
***/
struct PNG_ImageData
{
	DWORD dwWidth;
	DWORD dwHeight;
	BYTE chBitDepth;
	BYTE chColorType;
	BYTE chCompressionMethod;
	BYTE chFilterMethod;
	BYTE chInterlaceMethod;
};
#pragma endregion

#pragma region PNG data

/*** PNG Header ***/
static const BYTE achHeader[8] = {137, 80, 78, 71, 13, 10, 26, 10 };

/*** PNG Critical chunks ***/
static const BYTE achChunk_IHDR[5] = "IHDR";   /**< Image Header. Contains all necessary basic image information. **/
static const BYTE achChunk_PLTE[5] = "PLTE";   /**< Palette. The PLTE chunk contains from 1 to 256 palette entries. **/
static const BYTE achChunk_IDAT[5] = "IDAT";   /**< Image data. The IDAT chunk contains the actual image data which is the output stream of the compression algorithm. **/
static const BYTE achChunk_IEND[5] = "IEND";   /**< Image trailer. The IEND chunk marks the end of the PNG datastream. **/

/*** PNG Ancillary chunks ***/
static const BYTE achChunk_tRNS[5] = "tRNS";   /**< Transparency. The tRNS chunk specifies either alpha values that are associated with palette entries (for indexed-colour images) or a single transparent colour (for greyscale and truecolour images). **/
static const BYTE achChunk_cHRM[5] = "cHRM";   /**< Primary chromaticities and white point. The cHRM chunk may be used to specify the 1931 CIE x,y chromaticities of the red, green, and blue display primaries used in the image, and the referenced white point. **/
static const BYTE achChunk_gAMA[5] = "gAMA";   /**< Image gamma. The gAMA chunk specifies the relationship between the image samples and the desired display output intensity. **/
static const BYTE achChunk_iCCP[5] = "iCCP";   /**< Embedded ICC profile. **/
static const BYTE achChunk_sBIT[5] = "sBIT";   /**< Significant bits. **/
static const BYTE achChunk_sRGB[5] = "sRGB";   /**< Standard RGB colour space. **/
static const BYTE achChunk_tEXt[5] = "tEXt";   /**< Textual data. **/
static const BYTE achChunk_iTXt[5] = "iTXt";   /**< International textual data. **/
static const BYTE achChunk_zTXt[5] = "zTXt";   /**< Compressed textual data. **/
static const BYTE achChunk_bKGD[5] = "bKGD";   /**< Background colour. **/
static const BYTE achChunk_hIST[5] = "hIST";   /**< Image histogram. **/
static const BYTE achChunk_pHYs[5] = "pHYs";   /**< Physical pixel dimensions. **/
static const BYTE achChunk_sPLT[5] = "sPLT";   /**< Suggested palette. **/
static const BYTE achChunk_tIME[5] = "tIME";   /**< Image last-modification time **/

/*** PNG Length defines ***/
#define PNG_HEADER_LENGTH 8
#define PNG_CHUNK_LENGTH 4

#pragma endregion

#pragma region CRC helpers
/**
* Table of CRCs of all 8-bit messages. 
***/
static unsigned long g_anCRCTable[256];

/**
* Flag: has the table been computed? Initially false. 
***/
static int g_bCRCTableComputed = 0;

/**
* Make the table for a fast CRC. 
***/
static void MakeCRCTable(void)
{
	unsigned long c;
	int n, k;

	for (n = 0; n < 256; n++) {
		c = (unsigned long) n;
		for (k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		g_anCRCTable[n] = c;
	}
	g_bCRCTableComputed = 1;
}

/**
* Update a running CRC with the bytes buf[0..len-1]--the CRC
* should be initialized to all 1's, and the transmitted value
* is the 1's complement of the final running CRC (see the
* CRC() routine below)). 
***/
static unsigned long UpdateCRC(unsigned long crc, unsigned char *buf, int len)
{
	unsigned long c = crc;
	int n;

	if (!g_bCRCTableComputed)
		MakeCRCTable();
	for (n = 0; n < len; n++) {
		c = g_anCRCTable[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c;
}

/**
* Return the CRC of the bytes buf[0..len-1]. 
***/
static unsigned long CRC(unsigned char *buf, int len)
{
	return UpdateCRC(0xffffffffL, buf, len) ^ 0xffffffffL;
}

#pragma endregion

#pragma region Decoder helpers
/**
* Small helper to read an inverted DWORD out of a PNG data stream.
***/
inline DWORD PNG_ReadDWord(BYTE* pchByte) 
{ 
	return ((BYTE)pchByte[0]<<24)+((BYTE)pchByte[1]<<16)+((BYTE)pchByte[2]<<8)+(BYTE)pchByte[3]; 
}
/**
* Reads a single bit from a reversed Stream.
***/
static unsigned long PNG_ReadBitFromReversedStream(size_t& bitp, const unsigned char* bits) 
{ 
	unsigned long result = (bits[bitp >> 3] >> (7 - (bitp & 0x7))) & 1; 
	bitp++; 
	return result;
}
/**
* Reads bits (nbits) from a reversed Stream.
***/
static unsigned long PNG_ReadBitsFromReversedStream(size_t& bitp, const unsigned char* bits, unsigned long nbits)
{
	unsigned long result = 0;
	for(size_t i = nbits - 1; i < nbits; i--) result += ((PNG_ReadBitFromReversedStream(bitp, bits)) << i);
	return result;
}
/**
* Sets a bit in a reversed stream
**/
static void PNG_SetBitOfReversedStream(size_t& bitp, unsigned char* bits, unsigned long bit)
{ 
	bits[bitp >> 3] |=  (bit << (7 - (bitp & 0x7))); 
	bitp++; 
}
/**
* Paeth predicter, used by PNG filter type 4.
* This technique is due to Alan W. Paeth .
***/
static unsigned char PNG_PaethPredictor(short a, short b, short c)
{
	short p = a + b - c;
	short pa = p > a ? (p - a) : (a - p);
	short pb = p > b ? (p - b) : (b - p);
	short pc = p > c ? (p - c) : (c - p);
	return (unsigned char)((pa <= pb && pa <= pc) ? a : pb <= pc ? b : c);
}
/**
* Unfilter a decompressed PNG scanline.
***/
static HRESULT PNG_UnfilterScanline(unsigned char* recon, const unsigned char* scanline, const unsigned char* precon, size_t bytewidth, unsigned long filterType, size_t length)
{
	switch(filterType)
	{
	case 0: for(size_t i = 0; i < length; i++) recon[i] = scanline[i]; break;
	case 1:
		for(size_t i =         0; i < bytewidth; i++) recon[i] = scanline[i];
		for(size_t i = bytewidth; i <    length; i++) recon[i] = scanline[i] + recon[i - bytewidth];
		break;
	case 2:
		if(precon) for(size_t i = 0; i < length; i++) recon[i] = scanline[i] + precon[i];
		else       for(size_t i = 0; i < length; i++) recon[i] = scanline[i];
		break;
	case 3:
		if(precon)
		{
			for(size_t i =         0; i < bytewidth; i++) recon[i] = scanline[i] + precon[i] / 2;
			for(size_t i = bytewidth; i <    length; i++) recon[i] = scanline[i] + ((recon[i - bytewidth] + precon[i]) / 2);
		}
		else
		{
			for(size_t i =         0; i < bytewidth; i++) recon[i] = scanline[i];
			for(size_t i = bytewidth; i <    length; i++) recon[i] = scanline[i] + recon[i - bytewidth] / 2;
		}
		break;
	case 4:
		if(precon)
		{
			for(size_t i =         0; i < bytewidth; i++) recon[i] = scanline[i] + PNG_PaethPredictor(0, precon[i], 0);
			for(size_t i = bytewidth; i <    length; i++) recon[i] = scanline[i] + PNG_PaethPredictor(recon[i - bytewidth], precon[i], precon[i - bytewidth]);
		}
		else
		{
			for(size_t i =         0; i < bytewidth; i++) recon[i] = scanline[i];
			for(size_t i = bytewidth; i <    length; i++) recon[i] = scanline[i] + PNG_PaethPredictor(recon[i - bytewidth], 0, 0);
		}
		break;
	default: 
		// error: unexisting filter type given
		return E_FAIL; 
	}

	return S_OK;
}
#pragma endregion

#pragma region Macros
#define PNG_FREE_DATA free(achChunk);
#define PNG_OUTPUT_DEBUG(msg) { wsprintf(szLogBuf, L"%s - %s", PNG_DECODER_APP_NAME, msg ); OutputDebugString(szLogBuf); OutputDebugString(szPath); }
#define PNG_OUTPUT_DEBUG_NUMBER(msg, dw) { wsprintf(szLogBuf, msg, dw); OutputDebugString(szLogBuf); }
#define PNG_CLOSE_FILE(errmsg) { PNG_OUTPUT_DEBUG(errmsg); inFile.clear(); return E_FAIL; }
#define PNG_CLOSE_FILE_AND_FREE_DATA(errmsg) { PNG_FREE_DATA; PNG_CLOSE_FILE(errmsg); }
#define PNG_READ_CHUNK_LENGTH { inFile.read((char*)&achDwordBuf, sizeof(DWORD)); dwLength = PNG_ReadDWord(achDwordBuf); }
#define PNG_READ_CHUNK achChunk = (BYTE*)malloc((size_t)dwLength+PNG_CHUNK_LENGTH);	inFile.read((char*)achChunk, dwLength+PNG_CHUNK_LENGTH); if (inFile.gcount() < (dwLength+PNG_CHUNK_LENGTH)) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG chunk length corrupt !");
#define PNG_READ_CHUNK_CRC { inFile.read((char*)&achDwordBuf, sizeof(DWORD)); dwCRC = PNG_ReadDWord(achDwordBuf); }
#pragma endregion

#pragma region Decoder
/**
* Decode a PNG File, either from memory stream or from file.
***/
static HRESULT DecodePNGFile(LPWSTR szPath, BYTE* pPNGData, DWORD dwPNGSize, PNG_ImageData &sImageData, std::vector<unsigned char> &paImage)
{
	// PNG data bytes
	BYTE achDwordBuf[sizeof(DWORD)];
	DWORD dwLength;
	DWORD dwCRC;
	BYTE* achChunk;

	// deflate compression data vector
	std::vector<BYTE> paZData;
	// palette data vector
	std::vector<BYTE> paPalette;

	// debug output string
	wchar_t szLogBuf[256];

	// data stream
	std::stringstream inFile;
	inFile.clear();

	// if nullpointer path we take the memory stream
	if (szPath == nullptr)
	{
		// get the raw data from memory
		inFile.write((const char*)pPNGData, dwPNGSize);
	}
	else
	{
		// open file
		std::ifstream inFileSrc;
		inFileSrc.open(szPath, std::ios::in | std::ios::binary);
		if (inFileSrc.is_open())
		{
			inFile << inFileSrc.rdbuf();    
			inFileSrc.close();
		}
		else 
		{
			PNG_OUTPUT_DEBUG(L"File not found!");
			return E_FAIL;
		}
	}

	// set back binary stream
	inFile.seekg(0);
	inFile.seekp(0);

	// get through stream
	if (inFile.good())
	{
		// test the header
		BYTE achHeaderFile[PNG_HEADER_LENGTH];
		inFile.read((char*)achHeaderFile, PNG_HEADER_LENGTH);
		if (memcmp((const void*)achHeader, (const void*)achHeaderFile, PNG_HEADER_LENGTH) != NULL)
			PNG_CLOSE_FILE(L"PNG header corrupt");

		// test the IHDR chunk... first read length and chunk
		PNG_READ_CHUNK_LENGTH; 
		PNG_READ_CHUNK;

		// test IHDR chunk
		if (memcmp((const void*)achChunk_IHDR, (const void*)achChunk, PNG_CHUNK_LENGTH) != NULL)
			PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG IHDR chunk missing");

		// debug output
		OutputDebugStringA((char*)achChunk_IHDR);

		// read CRC
		PNG_READ_CHUNK_CRC;

		// create crc from data (+4 for chunk type)
		DWORD dwCRCData = CRC(achChunk, (int)dwLength+PNG_CHUNK_LENGTH);

		// CRC test only for IHDR and IEND chunks... others not valid !!
		if (dwCRC != dwCRCData) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG IHDR CRC test failed!");

		// debug output
		/*wsprintf(szLogBuf, L"CRC file %u", dwCRC); OutputDebugString(szLogBuf);
		wsprintf(szLogBuf, L"CRC data %u", dwCRCData); OutputDebugString(szLogBuf);*/

#pragma region IHDR chunk
		// IHDR chunk 
		// Chunk type:         4 bytes  // Width:              4 bytes      // Height:             4 bytes      // Bit depth:          1 byte
		// Color type:         1 byte	// Compression method: 1 byte		// Filter method:      1 byte		// Interlace method:   1 byte
		sImageData.dwWidth = PNG_ReadDWord(&achChunk[4]);
		sImageData.dwHeight = PNG_ReadDWord(&achChunk[8]);
		sImageData.chBitDepth = achChunk[12];
		sImageData.chColorType = achChunk[13];
		sImageData.chCompressionMethod = achChunk[14];
		sImageData.chFilterMethod = achChunk[15];
		sImageData.chInterlaceMethod = achChunk[16];

		// debug output
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG dwWidth : %u", sImageData.dwWidth);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG dwHeight : %u", sImageData.dwHeight);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG chBitDepth : %u", sImageData.chBitDepth);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG chColorType : %u", sImageData.chColorType);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG chCompressionMethod : %u", sImageData.chCompressionMethod);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG chFilterMethod : %u", sImageData.chFilterMethod);
		PNG_OUTPUT_DEBUG_NUMBER(L"PNG chInterlaceMethod : %u", sImageData.chInterlaceMethod);

		// test header data
		if ((sImageData.dwWidth == 0) || (sImageData.dwHeight == 0)) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG zero dimensions!");
		if (sImageData.chCompressionMethod != 0) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG only compression method zero (standard) is allowed!");
		if (sImageData.chFilterMethod != 0) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG only filter method zero (standard) is allowed!");
		if ((sImageData.chBitDepth != 1) && (sImageData.chBitDepth != 2) && (sImageData.chBitDepth != 4) && (sImageData.chBitDepth != 8) && (sImageData.chBitDepth != 16))
			PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG only bit depth 1, 2, 4, 8 or 16 allowed!");
		if ((sImageData.chColorType != 0) && (sImageData.chColorType != 2) && (sImageData.chColorType != 3) && (sImageData.chColorType != 4) && (sImageData.chColorType != 6))
			PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG only color types 0, 2, 3, 4 or 6 allowed!");
		if ((sImageData.chInterlaceMethod != 0) && (sImageData.chInterlaceMethod != 1))
			PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG only interlace method 0 or 1 allowed!");

#pragma endregion

		// free chunk data
		PNG_FREE_DATA;

		// loop through png chunks
		while(inFile.good())
		{
			// read chunk length
			PNG_READ_CHUNK_LENGTH;

			// debug output... 
			// wsprintf(szLogBuf, L"Chunk length %u", dwLength); OutputDebugString(szLogBuf);

			// read chunk
			PNG_READ_CHUNK;

			// read chunk crc
			PNG_READ_CHUNK_CRC;

			// create crc from data (+4 for chunk type)
			DWORD dwCRCData = CRC(achChunk, (int)dwLength+PNG_CHUNK_LENGTH);

			//// debug output... 
			//wsprintf(szLogBuf, L"CRC file %u", dwCRC); OutputDebugString(szLogBuf);
			//wsprintf(szLogBuf, L"CRC data %u", dwCRCData); OutputDebugString(szLogBuf);

			// chunk type parser....

#pragma region PNG Critical chunks
			if (memcmp((const void*)achChunk_PLTE, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) 
			{ 
				// clear the palette vector
				paPalette.clear();

				// add data to palette vector
				paPalette.insert(paPalette.end(), &achChunk[4], &achChunk[4 + dwLength]);

				// error : palette too big
				if(paPalette.size() > (3 * 256)) 
					PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG palette too big!");

			} // chunk not implemented 
			else if (memcmp((const void*)achChunk_IDAT, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) // compressed deflate data block
			{ 
				OutputDebugStringA((char*)achChunk_IDAT); 

				// add data to compressed data vector
				paZData.insert(paZData.end(), &achChunk[4], &achChunk[4 + dwLength]);
			} 
			else if (memcmp((const void*)achChunk_IEND, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) // Image End
			{ 
				OutputDebugStringA((char*)achChunk_IEND);

				// CRC test only for IHDR and IEND chunks... others not valid !!
				if (dwCRC != dwCRCData) PNG_CLOSE_FILE_AND_FREE_DATA(L"PNG IEND CRC test failed!");
			} 
#pragma endregion

#pragma region PNG Ancillary chunks
			else if (memcmp((const void*)achChunk_tRNS, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_tRNS); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_cHRM, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_cHRM); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_gAMA, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_gAMA); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_iCCP, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_iCCP); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_sBIT, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_sBIT); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_sRGB, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_sRGB); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_tEXt, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_tEXt); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_iTXt, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_iTXt); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_zTXt, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_zTXt); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_bKGD, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_bKGD); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_hIST, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_hIST); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_pHYs, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_pHYs); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_sPLT, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_sPLT); } // chunk not implemented 
			else if (memcmp((const void*)achChunk_tIME, (const void*)achChunk, PNG_CHUNK_LENGTH) == NULL) { OutputDebugStringA((char*)achChunk_tIME); } // chunk not implemented
			else PNG_OUTPUT_DEBUG(L"unknown PNG chunk!");
#pragma endregion

			// free chunk data
			PNG_FREE_DATA;

			// end of file ?
			if (inFile.peek() == EOF) break;
		}

		// clear source data vector
		inFile.clear();

		// resize the return scanline vector
		std::vector<unsigned char> paOut(((sImageData.dwWidth * (sImageData.dwHeight * sImageData.chBitDepth + 7)) / 8) + sImageData.dwHeight); 

		// call decompressor, then filter scanlines
		if (FAILED(DeflateDecompress(paZData, paOut)))
		{
			PNG_OUTPUT_DEBUG(L"DeflateDecoder : an Error occured!");
		}
		else
		{
			// get bits per pixel
			DWORD dwBPP = 0;
			if(sImageData.chColorType == 2) 
				dwBPP = (3 * sImageData.chBitDepth);
			else if(sImageData.chColorType >= 4) 
				dwBPP = (sImageData.chColorType - 2) * sImageData.chBitDepth;
			else 
				dwBPP = sImageData.chBitDepth;

			// filter the image
			size_t bytewidth = (dwBPP + 7) / 8, outlength = (sImageData.dwHeight * sImageData.dwWidth * dwBPP + 7) / 8;

			// resize the image buffer
			paImage.resize(outlength); 

			// use a regular pointer to the std::vector for faster code if compiled without optimization
			unsigned char* out_ = outlength ? &paImage[0] : 0; 

			// no interlace, just filter
			if(sImageData.chInterlaceMethod == 0) 
			{
				// length in bytes of a scanline, excluding the filtertype byte
				size_t linestart = 0, linelength = (sImageData.dwWidth * dwBPP + 7) / 8; 

				// byte per byte
				if(dwBPP >= 8) 
					for(unsigned long y = 0; y < sImageData.dwHeight; y++)
					{
						// first byte = filter type
						unsigned long filterType = paOut[linestart];

						// set the previous line
						const unsigned char* prevline = (y == 0) ? 0 : &out_[(y - 1) * sImageData.dwWidth * bytewidth];

						// debug output
						/*PNG_OUTPUT_DEBUG_NUMBER(L"linestart %u", linestart);
						PNG_OUTPUT_DEBUG_NUMBER(L"%u", filterType);*/

						// unfilter this scanline
						if(FAILED(PNG_UnfilterScanline(&out_[linestart - y], &paOut[linestart + 1], prevline, bytewidth, filterType,  linelength))) 
						{
							PNG_OUTPUT_DEBUG(L"DeflateDecoder : failed to unfilter Scanline");
							return E_FAIL;
						}
						// go to start of next scanline
						linestart += (1 + linelength); 
					}
				else 
				{
					// less than 8 bits per pixel, so fill it up bit per bit
					std::vector<unsigned char> templine((sImageData.dwWidth * dwBPP + 7) >> 3);
					for(size_t y = 0, obp = 0; y < sImageData.dwHeight; y++)
					{
						// first byte = filter type 
						unsigned long filterType = paOut[linestart];

						// set the previous line
						const unsigned char* prevline = (y == 0) ? 0 : &out_[(y - 1) * sImageData.dwWidth * bytewidth];

						// debug output
						/*PNG_OUTPUT_DEBUG_NUMBER(L"linestart %u", linestart);
						PNG_OUTPUT_DEBUG_NUMBER(L"%u", filterType);*/

						// unfilter this scanline
						if(FAILED(PNG_UnfilterScanline(&templine[0], &paOut[linestart + 1], prevline, bytewidth, filterType, linelength))) 
						{
							PNG_OUTPUT_DEBUG(L"DeflateDecoder : failed to unfilter Scanline");
							return E_FAIL;
						}

						// fill up bits
						for(size_t bp = 0; bp < sImageData.dwWidth * dwBPP;) PNG_SetBitOfReversedStream(obp, out_, PNG_ReadBitFromReversedStream(bp, &templine[0]));

						//go to start of next scanline
						linestart += (1 + linelength); 
					}
				}
			}

			// convert to ARGB - true by default right now
			if(true) 
			{
				// RGB color
				if(sImageData.chBitDepth == 8 && sImageData.chColorType == 2) 
				{
					BYTE* pchData = new BYTE[paImage.size()];
					memcpy((void*)pchData, (void*)paImage.data(), paImage.size());

					// resize image
					paImage.resize((sImageData.dwHeight * sImageData.dwWidth * 32 + 7) / 8);

					for(size_t i = 0; i < (sImageData.dwHeight * sImageData.dwWidth); i++)
					{
						// BGR -> RGB 
						for(size_t c = 0; c < 3; c++) paImage[4 * i + c] = pchData[3 * i + /*2 -*/ c];

						// only full alpha currently supported
						paImage[4 * i + 3] = 255;
					}

					delete pchData;
				}
				// 8Bit Color Palette
				else if(sImageData.chBitDepth == 8 && sImageData.chColorType == 3) 
				{
					BYTE* pchData = new BYTE[paImage.size()];
					memcpy((void*)pchData, (void*)paImage.data(), paImage.size());

					// resize image
					paImage.resize((sImageData.dwHeight * sImageData.dwWidth * 32 + 7) / 8);

					for(size_t i = 0; i < (sImageData.dwHeight * sImageData.dwWidth); i++)
					{		
						if(4U * pchData[i] >= paPalette.size()) 
						{
							PNG_OUTPUT_DEBUG(L"PNG invalid palette.");
							return E_FAIL;
						}

						// get rgb colors from the palette -> ARGB
						for(size_t c = 0; c < 3; c++) paImage[4 * i + c] = paPalette[3 * pchData[i] + 2 - c]; 

						// set alpha
						paImage[4 * i + 3] = 255; 
					}

					delete pchData;
				}
				// RGBA color (in case ABGR)
				else if(sImageData.chBitDepth == 8 && sImageData.chColorType == 6)
				{
					BYTE* pchData = new BYTE[paImage.size()];
					memcpy((void*)pchData, (void*)paImage.data(), paImage.size());

					for(size_t i = 0; i < (sImageData.dwHeight * sImageData.dwWidth); i++)
					{
						// BGR -> RGB 
						for(size_t c = 0; c < 3; c++) paImage[4 * i + c] = pchData[4 * i + 2 - c];

						// set alpha
						paImage[4 * i + 3] = pchData[4 * i + 3];
					}

					delete pchData;
				}
				else
				{
					PNG_OUTPUT_DEBUG(L"PNG Format not supported!");
					return E_FAIL;
				}
			}

			PNG_OUTPUT_DEBUG(L"PNG Image successfully decoded.");
		}
	}
	else 
	{
		PNG_OUTPUT_DEBUG(L"No data present!");
		return E_FAIL;
	}

	return S_OK;
}

#pragma endregion

#endif