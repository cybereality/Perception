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
#ifndef AQUILINUS_IMGUI_HELPERS
#define AQUILINUS_IMGUI_HELPERS

#include <Windows.h>
#include "imgui.h"
#include <GL/glew.h>

namespace ImGui
{
	/**
	* Simple color structure.
	***/
	struct ColorSchemeHex
	{
		UINT uColor[5];
	};

	/**
	* Small color converter helper..
	**/
	inline ImVec4 HEX2Float_Color(UINT uHex)
	{
		ImVec4 rgbColor;
		rgbColor.x = ((uHex >> 16) & 0xFF) / 255.f;  // Extract the RR byte
		rgbColor.y = ((uHex >> 8) & 0xFF) / 255.f;   // Extract the GG byte
		rgbColor.z = ((uHex) & 0xFF) / 255.f;        // Extract the BB byte
		rgbColor.w = 1.f;                            // set alpha to 1.f

		return rgbColor;
	}

	/**
	* Small color compare helpre
	***/
	inline bool CmpColors(ImVec4 sC1, ImVec4 sC2)
	{
		return ((sC1.x + sC1.y + sC1.z) > (sC2.x + sC2.y + sC2.z));
	}

	/**
	* Set ImGui style by 5 HEX colors.
	**/
	inline void StyleColorsByScheme(ColorSchemeHex sScheme)
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		ImVec4* colors = style->Colors;

		// hex to float vectors
		ImVec4 asColor[5] = {
			HEX2Float_Color(sScheme.uColor[0]),
			HEX2Float_Color(sScheme.uColor[1]),
			HEX2Float_Color(sScheme.uColor[2]),
			HEX2Float_Color(sScheme.uColor[3]),
			HEX2Float_Color(sScheme.uColor[4])
		};

		// sort colors... first and last brightest and darkest ones
		bool bBright = false; if (CmpColors(asColor[0], asColor[1])) bBright = true;
		for (unsigned uI = 1; uI < 5; uI++)
		{
			if (CmpColors(asColor[uI], asColor[0]))
			{
				// exchange colors
				ImVec4 sTmp = asColor[0];
				asColor[0] = asColor[uI];
				asColor[uI] = sTmp;
			}
		}
		for (unsigned uI = 0; uI < 4; uI++)
		{
			if (CmpColors(asColor[4], asColor[uI]))
			{
				// exchange colors
				ImVec4 sTmp = asColor[4];
				asColor[4] = asColor[uI];
				asColor[uI] = sTmp;
			}
		}

		colors[ImGuiCol_Text] = asColor[0];
		colors[ImGuiCol_TextDisabled] = ImLerp(asColor[0], asColor[4], 0.6f);
		colors[ImGuiCol_WindowBg] = asColor[4];
		colors[ImGuiCol_ChildBg] = ImLerp(asColor[0], asColor[4], 0.95f); colors[ImGuiCol_ChildBg].w = .9f;
		colors[ImGuiCol_PopupBg] = ImLerp(asColor[0], asColor[4], 0.92f); colors[ImGuiCol_PopupBg].w = .94f;
		colors[ImGuiCol_Border] = ImLerp(asColor[0], asColor[3], 0.43f); colors[ImGuiCol_Border].w = .5f;
		colors[ImGuiCol_BorderShadow] = ImLerp(asColor[0], asColor[4], 0.9f);;
		colors[ImGuiCol_FrameBg] = ImLerp(asColor[1], asColor[2], 0.7f); colors[ImGuiCol_FrameBg].w = .54f;
		colors[ImGuiCol_FrameBgHovered] = ImLerp(asColor[1], asColor[2], 0.9f); colors[ImGuiCol_FrameBgHovered].w = .4f;
		colors[ImGuiCol_FrameBgActive] = ImLerp(asColor[1], asColor[2], 0.95f); colors[ImGuiCol_FrameBgActive].w = .6f;
		colors[ImGuiCol_TitleBg] = ImLerp(asColor[0], asColor[4], 0.60f);
		colors[ImGuiCol_TitleBgActive] = ImLerp(asColor[0], asColor[4], 0.45f);
		colors[ImGuiCol_TitleBgCollapsed] = asColor[4]; colors[ImGuiCol_TitleBgCollapsed].w = .5f;
		colors[ImGuiCol_MenuBarBg] = ImLerp(asColor[0], asColor[4], 0.64f);
		colors[ImGuiCol_ScrollbarBg] = ImLerp(asColor[0], asColor[4], 0.55f); colors[ImGuiCol_ScrollbarBg].w = .5f;
		colors[ImGuiCol_ScrollbarGrab] = ImLerp(asColor[0], asColor[4], 0.31f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImLerp(asColor[0], asColor[4], 0.41f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImLerp(asColor[0], asColor[4], 0.51f);
		colors[ImGuiCol_CheckMark] = ImLerp(asColor[2], asColor[3], 0.3f);
		colors[ImGuiCol_SliderGrab] = ImLerp(asColor[2], asColor[3], 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImLerp(asColor[2], asColor[3], 0.9f);
		colors[ImGuiCol_Button] = ImLerp(asColor[2], asColor[3], 0.9f); colors[ImGuiCol_Button].w = .4f;
		colors[ImGuiCol_ButtonHovered] = ImLerp(asColor[2], asColor[3], 0.8f);
		colors[ImGuiCol_ButtonActive] = ImLerp(asColor[1], asColor[3], 0.9f);
		colors[ImGuiCol_Header] = ImLerp(asColor[2], asColor[3], 0.7f); colors[ImGuiCol_Header].w = .4f;
		colors[ImGuiCol_HeaderHovered] = ImLerp(asColor[2], asColor[3], 0.7f); colors[ImGuiCol_HeaderHovered].w = .8f;
		colors[ImGuiCol_HeaderActive] = ImLerp(asColor[2], asColor[3], 0.8f);
		colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
		colors[ImGuiCol_SeparatorHovered] = ImLerp(asColor[0], asColor[3], 0.7f); colors[ImGuiCol_SeparatorHovered].w = 0.78f;
		colors[ImGuiCol_SeparatorActive] = ImLerp(asColor[0], asColor[3], 0.7f); colors[ImGuiCol_SeparatorActive].w = 1.00f;
		colors[ImGuiCol_ResizeGrip] = ImLerp(asColor[0], asColor[3], 0.9f); colors[ImGuiCol_ResizeGrip].w = 0.25f;
		colors[ImGuiCol_ResizeGripHovered] = ImLerp(asColor[0], asColor[3], 0.9f); colors[ImGuiCol_ResizeGripHovered].w = 0.67f;
		colors[ImGuiCol_ResizeGripActive] = ImLerp(asColor[0], asColor[3], 0.9f); colors[ImGuiCol_ResizeGripActive].w = 0.95f;
		colors[ImGuiCol_Tab] = ImLerp(colors[ImGuiCol_Header], colors[ImGuiCol_TitleBgActive], 0.80f);
		colors[ImGuiCol_TabHovered] = colors[ImGuiCol_HeaderHovered];
		colors[ImGuiCol_TabActive] = ImLerp(colors[ImGuiCol_HeaderActive], colors[ImGuiCol_TitleBgActive], 0.60f);
		colors[ImGuiCol_TabUnfocused] = ImLerp(colors[ImGuiCol_Tab], colors[ImGuiCol_TitleBg], 0.80f);
		colors[ImGuiCol_TabUnfocusedActive] = ImLerp(colors[ImGuiCol_TabActive], colors[ImGuiCol_TitleBg], 0.40f);
		colors[ImGuiCol_PlotLines] = ImLerp(asColor[0], asColor[4], 0.6f);
		colors[ImGuiCol_PlotLinesHovered] = ImLerp(asColor[0], asColor[3], 0.1f);
		colors[ImGuiCol_PlotHistogram] = ImLerp(asColor[0], asColor[3], 0.4f);
		colors[ImGuiCol_PlotHistogramHovered] = ImLerp(asColor[3], asColor[2], 0.7f);
		colors[ImGuiCol_TextSelectedBg] = ImLerp(asColor[3], asColor[2], 0.7f); colors[ImGuiCol_TextSelectedBg].w = .3f;
		colors[ImGuiCol_DragDropTarget] = asColor[3]; colors[ImGuiCol_DragDropTarget].w = .9f;
		colors[ImGuiCol_NavHighlight] = ImLerp(asColor[1], asColor[2], 0.8f);
		colors[ImGuiCol_NavWindowingHighlight] = asColor[0]; colors[ImGuiCol_NavWindowingHighlight].w = .9f;
		colors[ImGuiCol_NavWindowingDimBg] = ImLerp(asColor[0], asColor[4], 0.8f); colors[ImGuiCol_NavWindowingDimBg].w = .2f;
		colors[ImGuiCol_ModalWindowDimBg] = ImLerp(asColor[0], asColor[4], 0.8f); colors[ImGuiCol_ModalWindowDimBg].w = .35f;
	}

	/// <summary>
	/// Converts HBITMAP to raw pixel data
	/// </summary>
	/// <param name="hBitmap">Handle to the Bitmap</param>
	/// <param name="nWidth">Returns image width</param>
	/// <param name="nHeight">Returns image height</param>
	/// <returns>Vector to raw pixel data.</returns>
	inline std::vector<unsigned char> BitmapToPixel(HBITMAP pBitmap, int& nWidth, int& nHeight)
	{
		BITMAP sBmp = {};
		BITMAPINFO sInfo = {};
		std::vector<unsigned char> acPixels = std::vector<unsigned char>();

		// get bitmap description
		HDC pDC = CreateCompatibleDC(NULL);
		HBITMAP pOldBitmap = (HBITMAP)SelectObject(pDC, pBitmap);
		GetObject(pBitmap, sizeof(sBmp), &sBmp);

		// fill info structure
		sInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		sInfo.bmiHeader.biWidth = nWidth = sBmp.bmWidth;
		sInfo.bmiHeader.biHeight = nHeight = sBmp.bmHeight;
		sInfo.bmiHeader.biPlanes = 1;
		sInfo.bmiHeader.biBitCount = sBmp.bmBitsPixel;
		sInfo.bmiHeader.biCompression = BI_RGB;
		sInfo.bmiHeader.biSizeImage = ((nWidth * sBmp.bmBitsPixel + 31) / 32) * 4 * nHeight;

		// resize and fill vector
		acPixels.resize(sInfo.bmiHeader.biSizeImage);
		GetDIBits(pDC, pBitmap, 0, nHeight, &acPixels[0], &sInfo, DIB_RGB_COLORS);
		SelectObject(pDC, pOldBitmap);

		// convert RGBA->RGB ?
		if (sBmp.bmBitsPixel == 32)
		{
			int iCountIn = 4;
			int iCountOut = 0;
			for (; iCountIn < (int)(sInfo.bmiHeader.biSizeImage - 3); iCountIn += 4)
			{
				acPixels[iCountOut] = acPixels[iCountIn];
				acPixels[iCountOut + 1] = acPixels[iCountIn + 1];
				acPixels[iCountOut + 2] = acPixels[iCountIn + 2];
				iCountOut += 3;
			}
		}

		nHeight = std::abs(nHeight);
		DeleteDC(pDC);
		return acPixels;
	}

	/// <summary>
	/// Simple helper function to load a bitmap into a OpenGL texture with common settings
	/// </summary>
	/// <param name="hBitmap">Input bitmap handle</param>
	/// <param name="pnTexture">Returns the GL texture identifier</param>
	/// <param name="pnWidth">Returns image width</param>
	/// <param name="pnHeight">Returns image height</param>
	/// <returns>True if succeed</returns>
	inline bool CreateTextureFromBitmap(HBITMAP hBitmap, GLuint* pnTexture, int* pnWidth, int* pnHeight)
	{
		// Load from file
		int nImageWidth = 0;
		int nImageHeight = 0;

		// get raw pixel data
		std::vector<unsigned char> acImage_data = BitmapToPixel(hBitmap, nImageWidth, nImageHeight);
		if ((!acImage_data.size()) || (!nImageWidth) || (!nImageHeight)) return false;

		// Create a OpenGL texture identifier
		GLuint nImageTexture;
		glGenTextures(1, &nImageTexture);
		glBindTexture(GL_TEXTURE_2D, nImageTexture);

		// Setup filtering parameters for display
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

		// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif

		// check input format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nImageWidth, nImageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, acImage_data.data());

		*pnTexture = nImageTexture;
		*pnWidth = nImageWidth;
		*pnHeight = nImageHeight;

		glDisable(GL_TEXTURE_2D);

		return true;
	}
}

#endif
