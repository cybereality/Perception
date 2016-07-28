/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OpenVR DirectMode - Open Virtual Reality Direct Mode Rendering Node
Copyright (C) 2016 Denis Reischl

File <OpenVR-DirectMode.h> and
Class <OpenVR-DirectMode> :
Copyright (C) 2016 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin
and Neil Schneider

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

#include"AQU_Nodus.h"
#include"Resources.h"
#include<stdio.h>
#include<stdlib.h>
#include<sstream>
#include<vector>

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include <openvr.h>
#pragma comment(lib, "openvr_api.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_DECOMMANDERS                         11

#define OPENVR_OVERLAY_NAME                            "key.MTBS3D"
#define OPENVR_OVERLAY_FRIENDLY_NAME                   "MTBS3D"
#define OPENVR_HUD_OVERLAY_NAME                        "key.HUD"
#define OPENVR_HUD_OVERLAY_FRIENDLY_NAME               "HUD"

/**
* Matrix helper DX.
* @returns Left handed D3D matrix from Steam HmdMatrix34_t type.
***/
D3DMATRIX GetLH(vr::HmdMatrix34_t sMat)
{
	D3DXMATRIX sRet = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], -sMat.m[2][0], 0.0,
		sMat.m[0][1], sMat.m[1][1], -sMat.m[2][1], 0.0,
		-sMat.m[0][2], -sMat.m[1][2], sMat.m[2][2], 0.0,
		sMat.m[0][3], sMat.m[1][3], -sMat.m[2][3], 1.0f
		);

	return sRet;
}
/**
* Matrix helper DX.
* @returns Left handed D3D matrix from Steam HmdMatrix44_t type.
***/
D3DMATRIX GetLH(vr::HmdMatrix44_t sMat)
{
	D3DXMATRIX sRet = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], -sMat.m[2][0], sMat.m[3][0],
		sMat.m[0][1], sMat.m[1][1], -sMat.m[2][1], sMat.m[3][1],
		-sMat.m[0][2], -sMat.m[1][2], sMat.m[2][2], sMat.m[3][2],
		sMat.m[0][3], sMat.m[1][3], -sMat.m[2][3], sMat.m[3][3]
		);

	return sRet;
}
/**
* Matrix helper DX.
* @returns Left handed OpenVR projection matrix for specified eye.
***/
D3DXMATRIX GetHMDMatrixProjectionEyeLH(vr::IVRSystem* pHmd, vr::Hmd_Eye nEye, float fNearClip, float fFarClip)
{
	if (!pHmd)
		return D3DXMATRIX();

	// get raw projection data
	float fLeft, fRight, fTop, fBottom;
	pHmd->GetProjectionRaw(nEye, &fLeft, &fRight, &fTop, &fBottom);

	// create matrix
	D3DXMATRIX sRet;
	D3DXVECTOR2 asVec[2];
	asVec[0].x = fLeft;
	asVec[0].y = fRight;
	asVec[1].x = -fBottom;
	asVec[1].y = -fTop;
	asVec[0] *= fNearClip;
	asVec[1] *= fNearClip;
	D3DXMatrixPerspectiveOffCenterLH(&sRet, asVec[0].x, asVec[0].y, asVec[1].x, asVec[1].y, fNearClip, fFarClip);

	return sRet;
}
/**
* Matrix helper DX.
* @returns Left handed OpenVR pose matrix for specified eye.
***/
D3DXMATRIX GetHMDMatrixPoseEyeLH(vr::IVRSystem* pHmd, vr::Hmd_Eye nEye)
{
	if (!pHmd)
		return D3DXMATRIX();

	vr::HmdMatrix34_t sMat = pHmd->GetEyeToHeadTransform(nEye);
	D3DXMATRIX matrixObj = D3DXMATRIX(
		sMat.m[0][0], sMat.m[1][0], sMat.m[2][0], 0.0,
		sMat.m[0][1], sMat.m[1][1], sMat.m[2][1], 0.0,
		sMat.m[0][2], sMat.m[1][2], sMat.m[2][2], 0.0,
		sMat.m[0][3], sMat.m[1][3], sMat.m[2][3], 1.0f
		);

	D3DXMATRIX matrixObjInv;
	D3DXMatrixInverse(&matrixObjInv, 0, &matrixObj);
	return matrixObjInv;
}

/**
* Node Decommander Enumeration.
***/
enum OpenVR_Decommanders
{
	LeftTexture11,
	RightTexture11,
	LeftTexture10,
	RightTexture10,
	LeftTexture9,
	RightTexture9,
	IVRSystem,
	ZoomOut,
	HUDTexture11,
	HUDTexture10,
	HUDTexture9,
};

/**
* Vireio Open Source VR DirectMode Node Plugin.
***/
class OpenVR_DirectMode : public AQU_Nodus
{
public:
	OpenVR_DirectMode();
	virtual ~OpenVR_DirectMode();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);
private:
	/**
	* Submit left and right shared texture constantly.
	***/
	static DWORD WINAPI SubmitFramesConstantly(void* Param)
	{
		// optimize openvr for 1080p, draw space at top and bottom black (1200p)
		/*const static float fVMin = -(((1200.0f - 1080.0f) / 2.0f) / 1080.0f);
		const static float fVMax = 1.0f + (((1200.0f - 1080.0f) / 2.0f) / 1080.0f);*/

		while (true)
		{
			if (m_ppHMD)
			{
				if (*m_ppHMD)
				{
					if ((m_bInit) && (vr::VRCompositor()->CanRenderScene()) && (!vr::VROverlay()->IsDashboardVisible()))
					{
						// left + right
						for (int nEye = 0; nEye < 2; nEye++)
						{
							// fill openvr texture struct
							vr::Texture_t sTexture = { (void*)m_pcTex11Shared[nEye], vr::API_DirectX, vr::ColorSpace_Gamma };

							//// adjust aspect ratio ... TODO !! CORRECTIONS ON CINEMA NODE
							//vr::VRTextureBounds_t sBounds;
							//if (nEye == (int)vr::Eye_Left)
							//{
							//	sBounds.uMin = m_fHorizontalRatioCorrectionLeft + m_fHorizontalOffsetCorrectionLeft;
							//	sBounds.uMax = 1.0f - m_fHorizontalRatioCorrectionLeft + m_fHorizontalOffsetCorrectionLeft;
							//}
							//else
							//{
							//	sBounds.uMin = m_fHorizontalRatioCorrectionRight + m_fHorizontalOffsetCorrectionRight;
							//	sBounds.uMax = 1.0f - m_fHorizontalRatioCorrectionRight + m_fHorizontalOffsetCorrectionRight;
							//}
							//sBounds.vMin = fVMin;
							//sBounds.vMax = fVMax;

							// submit left texture
							vr::VRCompositor()->Submit((vr::EVREye)nEye, &sTexture);// , &sBounds);
						}

						// sleep for 10 milliseconds (default) to ensure frame is submitted ~90 times per second (for reprojection 20ms -> 45fps)
						Sleep(m_unSleepTime);
						vr::VRCompositor()->ForceInterleavedReprojectionOn(m_bForceInterleavedReprojection);
					}
				}
			}
		}
	}

	/**
	* Temporary directx 11 device for OpenVR.
	***/
	ID3D11Device* m_pcDeviceTemporary;
	/**
	* Temporary directx 11 device context for OpenVR.
	***/
	ID3D11DeviceContext* m_pcContextTemporary;
	/**
	* Stereo Textures input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexView11[2];
	/**
	* HUD Texture input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexViewHud11;
	/**
	* Zoom out switch.
	***/
	BOOL* m_pbZoomOut;
	/**
	* OpenVR system.
	***/
	static vr::IVRSystem **m_ppHMD;
	/**
	* The (tracked) device poses (for all devices).
	***/
	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	/**
	* The device pose matrices.
	***/
	D3DXMATRIX m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];
	/**
	* The overlay handle.
	***/
	vr::VROverlayHandle_t m_ulOverlayHandle;
	/**
	* The overlay thumbnail handle.
	***/
	vr::VROverlayHandle_t m_ulOverlayThumbnailHandle;
	/**
	* The HUD overlay handle.
	***/
	vr::VROverlayHandle_t m_ulHUDOverlayHandle;
	/**
	* Left eye aspect ratio correction.
	***/
	static float m_fHorizontalRatioCorrectionLeft;
	/**
	* Right eye aspect ratio correction.
	***/
	static float m_fHorizontalRatioCorrectionRight;
	/**
	* Left eye lens offset correction.
	***/
	static float m_fHorizontalOffsetCorrectionLeft;
	/**
	* Right eye lens offset correction.
	***/
	static float m_fHorizontalOffsetCorrectionRight;
	/**
	* True if OpenVR is initialized.
	***/
	static bool m_bInit;
	/**
	* Static thread handle.
	***/
	static HANDLE m_pThread;
	/**
	* The 2D vertex shader.
	***/
	ID3D11VertexShader* m_pcVertexShader11;
	/**
	* The 2D pixel shader.
	***/
	ID3D11PixelShader* m_pcPixelShader11;
	/**
	* The 2D vertex layout.
	***/
	ID3D11InputLayout* m_pcVertexLayout11;
	/**
	* The 2D vertex buffer.
	***/
	ID3D11Buffer* m_pcVertexBuffer11;
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	ID3D11Buffer* m_pcConstantBufferDirect11;
	/**
	* Basic sampler state.
	***/
	ID3D11SamplerState* m_pcSamplerState;
	/**
	* Drawing texture (created by game device).
	***/
	ID3D11Texture2D* m_pcTex11Draw[2];
	/**
	* Drawing texture view (created by game device).
	***/
	ID3D11RenderTargetView* m_pcTex11DrawRTV[2];
	/**
	* Copy texture shared (created by game device).
	***/
	ID3D11Texture2D* m_pcTex11Copy[2];
	/**
	* Shared texture (created by temporary device 1.1)
	***/
	static ID3D11Texture2D* m_pcTex11Shared[2];
	/**
	* Copy texture shared for HUD (created by game device).
	***/
	ID3D11Texture2D* m_pcTex11CopyHUD;
	/**
	* Shared texture for HUD (created by temporary device 1.1)
	***/
	ID3D11Texture2D* m_pcTex11SharedHUD;
	/**
	* Overlay Transform type.
	***/
	enum OverlayTransformType
	{
		Absolute,                /**< Sets the transform to absolute tracking origin. */
		TrackedDeviceRelative,   /**< Sets the transform to relative to the transform of the specified tracked device. */
		TrackedDeviceComponent,  /**< Sets the transform to draw the overlay on a rendermodel component mesh instead of a quad. */
	};
	/**
	* HUD Overlay Properties
	* To be read from the ini file.
	***/
	struct Overlay_Properties_HUD
	{
		OverlayTransformType eTransform;     /**< The type of transform for the HUD overlay **/
		vr::HmdVector3_t sVectorTranslation; /**< The translation vector for the HUD (in meters) **/
		vr::HmdColor_t sColor;               /**< The color for the HUD (including alpha) **/
		float fWidth;                        /**< The width of the HUD (in meters) **/
		union
		{
			vr::ETrackingUniverseOrigin eOrigin;   /**< The universe origin (for absolute transform) ***/
			vr::TrackedDeviceIndex_t nDeviceIndex; /**< The device index (for device relative transform) ***/
		};
	} m_sOverlayPropertiesHud;
	/**
	* Dashboard Overlay Properties
	* To be read from the ini file.
	***/
	struct Overlay_Properties_Dashboard
	{
		vr::HmdColor_t sColor;               /**< The color for the Dashboard overlay (including alpha) **/
		float fWidth;                        /**< The width of the Dashboard overlay (in meters) **/
	} m_sOverlayPropertiesDashboard;
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
	/**
	* True if interleaved reprojection is forced on.
	***/
	static bool m_bForceInterleavedReprojection;
	/**
	* Time to sleep for each submission frame. (in ms)
	***/
	static DWORD m_unSleepTime;
	/**
	* Default aspect ratio.
	***/
	float m_fAspectRatio;
	/**
	* The 3D vertex shader for the openVR models.
	***/
	ID3D11VertexShader* m_pcVSGeometry11;
	/**
	* The 3D pixel shader for the openVR models.
	***/
	ID3D11PixelShader* m_pcPSGeometry11;
	/**
	* The 3D vertex layout for the openVR models.
	***/
	ID3D11InputLayout* m_pcVLGeometry11;
	/**
	* The depth stencil DX11 left/right.
	***/
	ID3D11Texture2D* m_pcDSGeometry11[2];
	/**
	* The depth stencil view DX11.
	***/
	ID3D11DepthStencilView* m_pcDSVGeometry11[2];
	/**
	* The d3d11 sampler.
	***/
	ID3D11SamplerState* m_pcSampler11;
	/**
	* Constant buffer data structure.
	***/
	GeometryConstantBuffer m_sGeometryConstants;
	/**
	* The constant buffer for geometry shaders.
	***/
	ID3D11Buffer* m_pcConstantBufferGeometry;
	/**
	* Current view matrix.
	***/
	D3DXMATRIX m_sView;
	/**
	* Current eye pose matrix left/right.
	***/
	D3DXMATRIX m_sToEye[2];
	/**
	* Current projection matrix left/right.
	***/
	D3DXMATRIX m_sProj[2];
	/**
	* True if all render models are created.
	***/
	bool m_bRenderModelsCreated;
	/**
	* DX version of the OpenVR RenderModel_t structure.
	***/
	struct RenderModel_D3D
	{
		ID3D11Buffer* pcVertexBuffer;               /**< Vertex buffer for the mesh **/
		uint32_t unVertexCount;						/**< Number of vertices in the vertex data **/
		ID3D11Buffer* pcIndexBuffer;                /**< Indices into the vertex data for each triangle **/
		uint32_t unTriangleCount;					/**< Number of triangles in the mesh. Index count is 3 * TriangleCount **/
		ID3D11Texture2D* pcTexture;                 /**< Texture **/
		ID3D11ShaderResourceView* pcTextureSRV;     /**< Texture SRV **/
		uint32_t unTrackedDeviceIndex;              /**< Index of the device for this model **/
	};
	/**
	* Vector of all models to render.
	***/
	std::vector<RenderModel_D3D> m_asRenderModels;
	/**
	* Cinema room setup structure.
	* Fields from Vireio Cinema necessary for OpenVR.
	***/
	struct CinemaRoomSetup
	{
		float fScreenWidth;        /**< The width of the cinema screen, in physical meters. */
		float fScreenLevel;        /**< The vertical level of the cinema center, in physical meters. */
		float fScreenDepth;        /**< The depth of the cinema screen, in physical meters. */
	} m_sCinemaRoomSetup;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OpenVR_DirectMode* pOpenVR_DirectMode = new OpenVR_DirectMode();
	return static_cast<AQU_Nodus*>(pOpenVR_DirectMode);
}
