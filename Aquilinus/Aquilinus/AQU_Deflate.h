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
#ifndef AQU_DEFLATE_DECOMPRESSOR
#define AQU_DEFLATE_DECOMPRESSOR

#ifndef DEFLATE_DECOMPRESSOR_APP_NAME
#define DEFLATE_DECOMPRESSOR_APP_NAME L"Deflate Decompressor"
#endif

#include <Windows.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#pragma region Deflate decompressor data
#define DEFLATE_MAX_BIT_LENGTH      288
#define DEFLATE_CODE_LENGHTS_NUMBER 19
#define DEFLATE_COPY_LENGTHS_NUMBER 29
#define DEFLATE_COPY_DISTANCE_NUMBER_32 30
#define DEFLATE_COPY_DISTANCE_NUMBER_64 32
static const BYTE achCodeLengthOrder[DEFLATE_CODE_LENGHTS_NUMBER] = 
{ 
	16, 17, 18,  0,  8,  7,  9,  6, 10, 5, 
	11,  4, 12,  3, 13,  2, 14,  1, 15 
};
static const WORD achCopyLength[DEFLATE_COPY_LENGTHS_NUMBER] =
{ 
	/* 257 */   3, /* 258 */   4, /* 259 */   5,
	/* 260 */   6, /* 261 */   7, /* 262 */   8, /* 263 */   9, /* 264 */  10,
	/* 265 */  11, /* 266 */  13, /* 267 */  15, /* 268 */  17, /* 269 */  19,
	/* 270 */  23, /* 271 */  27, /* 272 */  31, /* 273 */  35, /* 274 */  43,
	/* 275 */  51, /* 276 */  59, /* 277 */  67, /* 278 */  83, /* 279 */  99,
	/* 280 */ 115, /* 281 */ 131, /* 282 */ 163, /* 283 */ 195, /* 284 */ 227,
	/* 285 */ 258 
};
static const BYTE achExtraLengthBits[DEFLATE_COPY_LENGTHS_NUMBER] =
{ 
	/* 257 */ 0, /* 258 */ 0, /* 259 */ 0, /* 260 */ 0,
	/* 261 */ 0, /* 262 */ 0, /* 263 */ 0, /* 264 */ 0,
	/* 265 */ 1, /* 266 */ 1, /* 267 */ 1, /* 268 */ 1,
	/* 269 */ 2, /* 270 */ 2, /* 271 */ 2, /* 272 */ 2,
	/* 273 */ 3, /* 274 */ 3, /* 275 */ 3, /* 276 */ 3,
	/* 277 */ 4, /* 278 */ 4, /* 279 */ 4, /* 280 */ 4,
	/* 281 */ 5, /* 282 */ 5, /* 283 */ 5, /* 284 */ 5,
	/* 285 */ 0 
};
static const unsigned short achCopyDistance[DEFLATE_COPY_DISTANCE_NUMBER_64] =
{
	1,     2,    3,    4,    5,    7,    9,    13,    17,    25,
	33,    49,   65,   97,  129,  193,  257,   385,   513,   769,
	1025,  1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577,
	32769, 49153 /* the last two are Deflate64 only */ 
};
static const unsigned char achExtraDistanceBits[DEFLATE_COPY_DISTANCE_NUMBER_64] =
{
	0,  0,  0,  0,  1,  1,  2,  2,  3,  3,  
	4,  4,  5,  5,  6,  6,  7,  7,  8,  8, 
	9,  9, 10, 10, 11, 11, 12, 12, 13, 13, 
	14, 14 /* the last two are Deflate64 only */  
};
#pragma endregion

#pragma region Deflate decompressor fields
/**
* Huffman tree structure.
***/
struct HuffmanTree
{
	/**
	* Make the huffman tree given the lengths.
	***/
	int MakeFromLengths(const std::vector<unsigned long>& paBitLen, DWORD dwMaxBitLen)
	{ 
		unsigned long numcodes = (unsigned long)(paBitLen.size()), treepos = 0, nodefilled = 0;
		std::vector<unsigned long> tree1d(numcodes), blcount(dwMaxBitLen + 1, 0), nextcode(dwMaxBitLen + 1, 0);

		// count number of instances of each code length
		for(unsigned long bits = 0; bits < numcodes; bits++) blcount[paBitLen[bits]]++; 
		for(unsigned long bits = 1; bits <= dwMaxBitLen; bits++) nextcode[bits] = (nextcode[bits - 1] + blcount[bits - 1]) << 1;

		// generate all the codes
		for(unsigned long n = 0; n < numcodes; n++) if(paBitLen[n] != 0) tree1d[n] = nextcode[paBitLen[n]]++; 

		// create tree, 32767 here means the paTree2D isn't filled there yet
		paTree2D.clear(); paTree2D.resize(numcodes * 2, 32767); 

		// loop through the codes and the bits for this code
		for(unsigned long n = 0; n < numcodes; n++)
			for(unsigned long i = 0; i < paBitLen[n]; i++)
			{
				unsigned long bit = (tree1d[n] >> (paBitLen[n] - i - 1)) & 1;
				if(treepos > numcodes - 2) return 55;

				// not yet filled in
				if(paTree2D[2 * treepos + bit] == 32767) 
				{
					// last bit
					if(i + 1 == paBitLen[n]) { paTree2D[2 * treepos + bit] = n; treepos = 0; } 

					// addresses are encoded as values > numcodes
					else { paTree2D[2 * treepos + bit] = ++nodefilled + numcodes; treepos = nodefilled; } 
				}
				// subtract numcodes from address to get address value
				else treepos = paTree2D[2 * treepos + bit] - numcodes; 
			}
			return 0;
	}
	/**
	* Decodes a symbol from the tree.
	***/
	int decode(bool& decoded, unsigned long& result, size_t& treepos, unsigned long bit) const
	{
		unsigned long numcodes = (unsigned long)paTree2D.size() / 2;

		// error ? outside the codetree ?
		if(treepos >= numcodes) 
			return 11; 

		// get result
		result = paTree2D[2 * treepos + bit];
		decoded = (result < numcodes);
		treepos = decoded ? 0 : result - numcodes;
		return 0;
	}
	/**
	* 2D representation of a huffman tree.
	* The one dimension is "0" or "1", the other contains all nodes and leaves of the tree.
	***/
	std::vector<unsigned long> paTree2D; 
};
#pragma endregion

#pragma region Decompressor helpers
/**
* Small helper to read bits out of a byte stream.
***/
static DWORD ReadBits(BYTE* pchByte, DWORD &dwPos, DWORD &dwOffs, BYTE chBits)
{
	DWORD ret = 0;

	for (int i = 0; i < chBits; i++)
	{
		ret += ((pchByte[dwPos] & (0x1 << dwOffs)) > 0) << i;
		dwOffs++;
		if (dwOffs > 7)
		{
			dwOffs = 0;
			dwPos++;
		}
	}

	return ret;
}
/**
* Small helper to get the next token from a huffman tree.
* Decode a single symbol from given list of bits with given code tree. return value is the symbol.
***/
static unsigned long huffmanDecodeSymbol(BYTE* pchByte, DWORD &dwPos, DWORD &dwOffs, const HuffmanTree& codetree, size_t inlength, bool bError)
{ 
	bool decoded; 
	unsigned long ct;
	size_t treepos = 0;
	bError = false;

	while(true)
	{
		if((dwOffs == 0) && (dwPos >= inlength)) 
		{ 
			// error: end reached without endcode 
			bError = true; 
			return 0; 
		} 
		bError = (codetree.decode(decoded, ct, treepos, (ULONG)ReadBits(pchByte, dwPos, dwOffs, 1))>0); 
		if(bError) return 0; 
		if(decoded) return ct;
	}
}
#pragma endregion

#pragma region Macros
#define DEFLATE_OUTPUT_DEBUG(msg) { wsprintf(szLogBuf, L"%s - %s", DEFLATE_DECOMPRESSOR_APP_NAME, msg ); OutputDebugString(szLogBuf); }
#define DEFLATE_OUTPUT_DEBUG_NUMBER(msg, dw) { wsprintf(szLogBuf, msg, dw); OutputDebugString(szLogBuf); }
#pragma endregion

#pragma region Deflate Decompressor
/**
* Simple deflate decompression method.
* Decompressed a raw deflate data block.
***/
static HRESULT DeflateDecompress(std::vector<BYTE> paZData, std::vector<BYTE> &paOut)
{
	// decompression done here...
	bool bBFinal = 0;
	BYTE chBType = 0;
	bool bError = 0;
	DWORD dwPos = 0;      /**< Source data position **/
	DWORD dwOffs = 0;     /**< Source data offset **/
	DWORD dwPosDest = 0;  /**< Destination data position **/

	// debug output string
	wchar_t szLogBuf[256];

	// decompressed data vector
	std::vector<BYTE> paDataDecompressed;

	// parse header bytes
	BYTE chCMethod = (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 4);
	BYTE chCMFlags = (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 4);
	BYTE chFCheck = (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 5);
	bool bFDict = (bool)(ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 1) > 0);
	BYTE chFLevel = (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 2);

	// test data header (skip FCHECK here meanwhile....)
	if (chCMethod != 8) { DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : only method 8 allowed!"); bError = true; }
	if (chCMFlags > 7) { DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : method flags > 7 not allowed!"); bError = true; }
	if (bFDict) { DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : dictionary not allowed!"); bError = true; }

	// loop through field
	while ((!bBFinal) && (!bError))
	{
		// read the block header
		bBFinal = (bool)(ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 1) > 0);
		chBType = (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 2);
				
		switch(chBType)
		{
		case 0: // 00 no compression
			{
				// move to next byte boundary
				if (dwOffs > 0) { dwOffs = 0; dwPos++; }

				// read and compare LEN and NLEN
				DWORD dwLen = paZData.data()[dwPos] + (paZData.data()[dwPos + 1] << 8);
				DWORD dwNLen = paZData.data()[dwPos + 2] + (paZData.data()[dwPos + 3] << 8);
				dwPos += 4;

				if ((dwLen + dwNLen) != 65535) { DEFLATE_OUTPUT_DEBUG(L"LEN != NLEN"); bError = true; break; }
				if ((dwLen + dwPos) > (DWORD)paZData.size()) { DEFLATE_OUTPUT_DEBUG(L"LEN + dwPos > size()"); bError = true; break; }

				// read data
				paDataDecompressed.insert(paDataDecompressed.end(), &paZData.data()[dwPos], &paZData.data()[dwPos + dwLen]);
				dwPos += dwLen;
			}
			break;	
		case 1: // 01 compressed with fixed Huffman codes
		case 2: // 10 compressed with dynamic Huffman codes
			{
				// the code trees
				HuffmanTree codeTree, codeTreeDistance;

				// fixed code table
				if (chBType == 1)
				{
					std::vector<unsigned long> bitlen(288, 8), bitlenD(32, 5);;
					for(size_t i = 144; i <= 255; i++) bitlen[i] = 9;
					for(size_t i = 256; i <= 279; i++) bitlen[i] = 7;
					codeTree.MakeFromLengths(bitlen, 15);
					codeTreeDistance.MakeFromLengths(bitlenD, 15);
				}
				// dynamic code table
				else 
				{
					// alphabet size
					size_t numLiterals   = 257 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 5); /**< number of literal/length codes + 257 **/
					size_t numDistance   =   1 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 5); /**< number of dist codes + 1 **/
					size_t numCodeLength =   4 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 4); /**< number of code length codes + 4 **/
					std::vector<unsigned long> paCodeLengthCode(DEFLATE_CODE_LENGHTS_NUMBER);       /**< lengths of tree to decode the lengths of the dynamic tree **/

					// read 19 codes defining bit lengths
					for (size_t i = 0; i < DEFLATE_CODE_LENGHTS_NUMBER; i++) 
						paCodeLengthCode[achCodeLengthOrder[i]] = (i < numCodeLength) ? (BYTE)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 3) : 0;

					// create code string huffman tree
					HuffmanTree codeStrings;
					int nError = codeStrings.MakeFromLengths(paCodeLengthCode, 7); 
					if(nError)
					{
						DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to create code length huffman tree!");
						bError = true;
						break;
					}

					// read bit lengths themselves
					WORD lastToken = 0;
					std::vector<ULONG> paBitLength;
					paBitLength.reserve(DEFLATE_MAX_BIT_LENGTH);
					while (paBitLength.size() < numLiterals + numDistance)
					{
						// get token
						WORD token = 
							(WORD)huffmanDecodeSymbol(paZData.data(), dwPos, dwOffs, codeStrings, paZData.size(), bError); 
						if(bError) 
						{
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to decode huffmann token!");
							break;
						}

						unsigned int howOften = 0;

						// single literal
						if (token < 16)
						{
							howOften  = 1;
							lastToken = token;
						}
						// repeat last 3x-6x
						else if (token == 16)
						{
							howOften = 3 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 2);
						}
						// 3x-10x zero
						else if (token == 17)
						{
							howOften  = 3 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 3);
							lastToken = 0;
						}
						else if (token == 18)// 11x-138x zero
						{
							howOften  = 11 + ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, 7);
							lastToken = 0;
						}
						else
						{
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : unknown huffman token !");
							bError = true;
							break;
						}

						// repeat lastTaken (howOften times)
						while (howOften--)
							paBitLength.push_back((unsigned char)lastToken);
					}

					// fill up with zeros
					paBitLength.resize(numLiterals+32, 0);

					// extract distance code lengths
					std::vector<ULONG> paDistanceLength;
					for (size_t i = 0; i < 32; i++)
						paDistanceLength.push_back(paBitLength[i + numLiterals]);

					// cut back, only literals
					paBitLength.resize(numLiterals);
					// removed too much ?
					paBitLength.resize(DEFLATE_MAX_BIT_LENGTH, 0);

					// now we've finally got HLIT and HDIST, so generate the code trees
					nError = codeTree.MakeFromLengths(paBitLength, 15);
					if(nError)
					{
						DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to create code huffman tree!");
						bError = true;
						break;
					}
					nError = codeTreeDistance.MakeFromLengths(paDistanceLength, 15);
					if(nError)
					{
						DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to create code distance huffman tree!");
						bError = true;
						break;
					}
				}

				// ...and inflate
				while(true)
				{
					// get next token
					WORD token = 
						(WORD)huffmanDecodeSymbol(paZData.data(), dwPos, dwOffs, codeTree, paZData.size(), bError); 
					if(bError) 
					{
						DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to decode huffmann token!");
						break;
					}

					// end code 256
					if (token == 256) 
						break;
					// literal symbol <= 255
					else if (token <= 255) 
					{
						if (dwPosDest >= paOut.size()) paOut.resize((dwPosDest + 1) * 2); //reserve more room
						paOut[dwPosDest++] = (BYTE)(token);
					}
					// length code >= 257 && <= 285
					else if (token >= 257 && token <= 285) 
					{
						// get length and extra length bits
						size_t length = achCopyLength[token - 257];
						size_t lengthExtraBits = achExtraLengthBits[token - 257];

						if (dwPos >= paZData.size())
						{ 
							// error, bit pointer will jump past memory
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : data length error!");
							bError = true;
							break; 
						} 
						length += ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, (BYTE)lengthExtraBits);

						// get the distance code
						DWORD dwCodeDist =
							(WORD)huffmanDecodeSymbol(paZData.data(), dwPos, dwOffs, codeTreeDistance, paZData.size(), bError); 
						if (bError) 
						{
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : failed to decode huffmann token!");
							break;
						}
						if (dwCodeDist > 29) 
						{ 
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : invalid dist code (30-31 are never used)!");
							bError = true;
							break; 
						}

						// get the distance and distance extra bits
						DWORD dwDistance = achCopyDistance[dwCodeDist]; 
						DWORD dwNumExtraBitsDist = achExtraDistanceBits[dwCodeDist];
						if (dwPos >= paZData.size())
						{ 
							// error, bit pointer will jump past memory
							DEFLATE_OUTPUT_DEBUG(L"DeflateDecoder : data length error!");
							bError = true;
							break; 
						} 
						dwDistance += (DWORD)ReadBits((BYTE*)paZData.data(), dwPos, dwOffs, (BYTE)dwNumExtraBitsDist);

						// backwards
						size_t start = dwPosDest, back = start - dwDistance; 

						// reserve more room if the destination vector is too small
						if(dwPosDest + length >= paOut.size()) paOut.resize((dwPosDest + length) * 2); 

						// provide the data
						for(size_t i = 0; i < length; i++) 
						{ 
							paOut[dwPosDest++] = paOut[back++]; 
							if(back >= start) back = start - dwDistance; 
						}
					}
				}
			}
			break;
		case 3: // 11 reserved(error)
			bError = true;
			break;
		}

		DEFLATE_OUTPUT_DEBUG_NUMBER(L"DeflateDecoder : decoded data size : %u", dwPosDest);
	}

	if (bError)
		return E_FAIL;
	else return S_OK;
}
#pragma endregion

#endif