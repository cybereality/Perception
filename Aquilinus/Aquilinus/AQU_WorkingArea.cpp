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

#pragma warning( disable : 26812 )

#pragma region AQU_WorkingArea include/define
#include"AQU_WorkingArea.h"
#include"AQU_2DData.h"
#include "..\dependecies\imgui\imgui_helpers.h"

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_INT(a) { wchar_t buf[128]; wsprintf(buf, L"%d", a); OutputDebugString(buf); }
#define DEBUG_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }

#pragma endregion

/// => Static fields
#pragma region AQU_WorkingArea static fields
WNDCLASSEX                                    AQU_WorkingArea::m_wc;
HWND                                          AQU_WorkingArea::m_hwnd;
HINSTANCE                                     AQU_WorkingArea::m_hInstance;
AQU_WorkingArea::WindowControl                AQU_WorkingArea::m_sWindowControl;
int                                           AQU_WorkingArea::m_nWindowWidth;
int                                           AQU_WorkingArea::m_nWindowHeight;
ImVec2                                        AQU_WorkingArea::m_sMouseCursor;
std::vector<NOD_Basic*>                       AQU_WorkingArea::m_paNodes;
AQU_WorkingAreaStatus                         AQU_WorkingArea::m_eWorkingAreaStatus;
AQU_TransferSite* AQU_WorkingArea::m_pcTransferSite;
std::vector<std::wstring>                     AQU_WorkingArea::m_vcPluginNames;
std::vector<std::wstring>                     AQU_WorkingArea::m_vcPluginCategories;
std::vector<std::wstring>                     AQU_WorkingArea::m_vcPluginFilePathes;
std::vector<UINT>                             AQU_WorkingArea::m_vcPluginIDs;
std::vector<AQU_Nodus*>                       AQU_WorkingArea::m_vcPlugins;
std::vector<HMODULE>                          AQU_WorkingArea::m_vcPluginHandles;
int                                           AQU_WorkingArea::m_nDataSheetCategorySelection;
int                                           AQU_WorkingArea::m_nDataSheetEntrySelection;
#pragma endregion

/// => Constructor / Destructor
#pragma region AQU_WorkingArea con-/destructor
/// <summary>
/// Constructor
/// </summary>
AQU_WorkingArea::AQU_WorkingArea(HINSTANCE hInstance, AQU_TransferSite* pcTransferSite)
{
	// set window size
	m_nWindowWidth = GetSystemMetrics(SM_CXFULLSCREEN) >> 1;
	m_nWindowHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	m_sWindowControl = {};
	m_sMouseCursor = {};

	// set module handle and config
	m_hInstance = hInstance;
	m_pcTransferSite = pcTransferSite;
	m_pcTransferSite->m_bIsWorkingArea = true;
	m_eWorkingAreaStatus = AQU_WorkingAreaStatus::Idle;

	// data sheet selection
	m_nDataSheetEntrySelection = -1;
	m_nDataSheetCategorySelection = -1;

	// enumerate plugins
	s_EnumeratePlugins(pcTransferSite->m_pFileManager->GetPluginPath());

	// clear nodes vector
	m_paNodes.clear();

	// set transfer site node vector address
	pcTransferSite->m_ppaNodes = &m_paNodes;

	// clear data sheet vector and supported interface indices vector
	m_pcTransferSite->m_paDataSheetCategories.clear();
	m_pcTransferSite->m_aInterfaceIndices.clear();

	// get node provider
	AQU_NodeProvider* pProvider = new AQU_NodeProvider();

	// loop through all possible interfaces
	for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
	{
		// interface is hooked up ?
		if (m_pcTransferSite->m_pConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
		{
			// add interface index
			m_pcTransferSite->m_aInterfaceIndices.push_back((UINT)i);

			// create category
			AQU_DataSheetCategory* psCategory = new AQU_DataSheetCategory();
			psCategory->m_szTitle = AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].szName;
			psCategory->m_bIsOpen = false;
			psCategory->m_paEntries.clear();

			int lenClass = (int)psCategory->m_szTitle.length() + 2;

			// loop through methods
			for (int j = 0; j < (int)AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].dwMethodsNumber; j++)
			{
				// get example node
				NOD_Basic* pNode = pProvider->Get_D3D_Node((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)i, j, 0, 0);

				if (pNode)
				{
					// create entry
					AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
					pEntry->m_bIsOpen = false;

					// set title
					int len = (int)wcslen(pNode->m_szTitle) + 1 - lenClass;
					pEntry->m_szTitle = std::wstring(pNode->m_szTitle, lenClass, len);

					// create sub entries
					for (int k = 0; k < (int)pNode->m_paCommandersTemporary.size(); k++)
						pEntry->m_aacSubEntries.push_back(std::wstring(pNode->m_paCommandersTemporary[k]->m_szTitle));

					// delete node
					delete pNode;

					// add entry
					psCategory->m_paEntries.push_back(pEntry);
				}
			}
			m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

		}
	}

	// create data sheet vertex shader tab, first create category and set tab index
	m_pcTransferSite->m_nVertexShaderTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	AQU_DataSheetCategory* psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Vertex Shaders";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();

	// add vertex shader category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// create data sheet pixel shader tab, first create category and set tab index
	m_pcTransferSite->m_nPixelShaderTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Pixel Shaders";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();

	// add vertex shader category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// create an elementary nodes tab, first create category and set tab index
	m_pcTransferSite->m_nElementaryNodesTabIndex = (UINT)m_pcTransferSite->m_paDataSheetCategories.size();
	psCategory = new AQU_DataSheetCategory();
	psCategory->m_szTitle = L"Elementary Nodes";
	psCategory->m_bIsOpen = false;
	psCategory->m_paEntries.clear();
	for (int i = 0; i < (int)AQU_ElementaryNodes::EN_NUMBER_OF_ELEMENTARY_NODES; i++)
	{
		// create entry
		AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
		pEntry->m_bIsOpen = false;

		// get name
		pEntry->m_szTitle = pProvider->Get_Elementary_Node_Name((AQU_ElementaryNodes)i);
		pEntry->m_aacSubEntries.push_back(std::wstring(pProvider->Get_Elementary_Node_Desc((AQU_ElementaryNodes)i)));

		// add entry
		psCategory->m_paEntries.push_back(pEntry);
	}

	// add elementary nodes category
	m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);

	// delete provider
	delete pProvider;

	// clear plugin indices vectors
	m_pcTransferSite->m_aPluginCategoryIndices.clear();
	m_pcTransferSite->m_aPluginEntryIndices.clear();

	// loop through enumerated plugins, create plugin entries and indices
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		UINT dwCategoryIndex = 0;

		// first, set category index
		for (int j = (int)m_pcTransferSite->m_nElementaryNodesTabIndex + 1; j < (int)m_pcTransferSite->m_paDataSheetCategories.size(); j++)
		{
			// compare name of the plugin enumeration category with the existing categories
			if (m_vcPluginCategories[i].compare(m_pcTransferSite->m_paDataSheetCategories[j]->m_szTitle) == 0)
			{
				// set category index
				dwCategoryIndex = (UINT)j;

				// add indices
				m_pcTransferSite->m_aPluginCategoryIndices.push_back(dwCategoryIndex);
				m_pcTransferSite->m_aPluginEntryIndices.push_back((UINT)m_pcTransferSite->m_paDataSheetCategories[j]->m_paEntries.size());

				// create and add entry
				AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
				pEntry->m_szTitle = m_vcPluginNames[i];
				m_pcTransferSite->m_paDataSheetCategories[j]->m_paEntries.push_back(pEntry);
			}
		}

		// no category ? create new
		if (dwCategoryIndex == 0)
		{
			// set indices
			m_pcTransferSite->m_aPluginCategoryIndices.push_back((UINT)m_pcTransferSite->m_paDataSheetCategories.size());
			m_pcTransferSite->m_aPluginEntryIndices.push_back(0);

			// create category
			psCategory = new AQU_DataSheetCategory();
			psCategory->m_szTitle = m_vcPluginCategories[i];
			psCategory->m_bIsOpen = false;
			psCategory->m_paEntries.clear();

			// create and add entry
			AQU_DataSheetEntry* pEntry = new AQU_DataSheetEntry();
			pEntry->m_szTitle = m_vcPluginNames[i];
			psCategory->m_paEntries.push_back(pEntry);

			// add category
			m_pcTransferSite->m_paDataSheetCategories.push_back(psCategory);
		}
	}

	// create the main window loop thread
	CreateThread(0, 0, s_WorkingAreaMsgThread, 0, 0, 0);
}

/// <summary>
/// Destructor
/// </summary>
AQU_WorkingArea::~AQU_WorkingArea()
{
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginNames.clear();

	auto it = m_paNodes.begin();
	while (it != m_paNodes.end())
	{
		if ((*it) != NULL)
			delete (*it);
		it = m_paNodes.erase(it);
	}
	auto it1 = m_vcPlugins.begin();
	while (it1 != m_vcPlugins.end())
	{
		if ((*it1) != NULL)
			delete (*it1);
		it1 = m_vcPlugins.erase(it1);
	}
	auto it2 = m_vcPluginHandles.begin();
	while (it2 != m_vcPluginHandles.end())
	{
		if ((*it2) != NULL)
			FreeLibrary(*it2);
		it2 = m_vcPluginHandles.erase(it2);
	}
}
#pragma endregion

/// => Methods
#pragma region AQU_WorkingArea methods
/// <summary>
/// => Working Area main thread
/// <param name="param">not used</param>
/// </summary>
DWORD WINAPI AQU_WorkingArea::s_WorkingAreaMsgThread(void* param)
{
	UNREFERENCED_PARAMETER(param);

	// glfw: initialize and configure
	if (!glfwInit()) { OutputDebugString(L"Inicio: GLFW Init fail !"); return 1; }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	glfwWindowHint(GLFW_DECORATED, 0);
	GLFWwindow* window = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, "Aquilinus", NULL, NULL);
	if (window == NULL)
	{
		OutputDebugString(L"Inicio: GLFW window creation failed !");
		glfwTerminate();
		return -1;
	}

	// set size, position, callback methods, enable vsync
	glfwSetWindowSizeLimits(window, m_nWindowWidth, m_nWindowHeight, m_nWindowWidth, m_nWindowHeight);
	glfwSetWindowPos(window, m_nWindowWidth, 0);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, s_Viewport_callback);
	glfwSetCursorPosCallback(window, s_Cursor_position_callback);
	glfwSetMouseButtonCallback(window, s_Mouse_button_callback);

	// and init glew
	bool err = glewInit() != GLEW_OK;
	if (err)
	{
		OutputDebugString(L"Inicio: Failed to initialize OpenGL loader!\n");
		return 1;
	}

	// Setup Dear ImGui context, font
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImFontConfig sConfig = {};
	sConfig.SizePixels = 16;
	ImFont* psFontSmall = io.Fonts->AddFontDefault(&sConfig);
	sConfig.SizePixels = 22;
	ImFont* psFontMedium = io.Fonts->AddFontDefault(&sConfig);

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(nullptr);

	OutputDebugString(L"Aquilinus : Injectd window creation succeeded !");

	// set color scheme
	ImGui::ColorSchemeHex sScheme = {};
	CopyMemory(&sScheme.uColor[0], &g_colorSchemes[m_pcTransferSite->m_pConfig->dwColorSchemeIndex].uColor[0], sizeof(UINT[5]));
	ImGui::StyleColorsByScheme(sScheme);
	ImVec4 clear_color = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];

	// => <main> render loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events, update window fields
		glfwPollEvents();
		glfwGetWindowPos(window, &m_sWindowControl.nW_posx, &m_sWindowControl.nW_posy);
		glfwSetWindowPos(window, m_sWindowControl.nW_posx + m_sWindowControl.nOffset_cpx, m_sWindowControl.nW_posy + m_sWindowControl.nOffset_cpy);
		m_sWindowControl.nControlEvent = 0;
		m_sWindowControl.nOffset_cpx = 0;
		m_sWindowControl.nOffset_cpy = 0;
		m_sWindowControl.nCp_x += m_sWindowControl.nOffset_cpx;
		m_sWindowControl.nCp_y += m_sWindowControl.nOffset_cpy;

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// main window set flags and zero pos
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoCollapse
			| ImGuiWindowFlags_NoBackground
			| ImGuiWindowFlags_NoBringToFrontOnFocus;
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2((float)m_nWindowWidth, (float)m_nWindowHeight), ImGuiCond_FirstUseEver);

		// => <main loop> window
		if (ImGui::Begin("Aquilinus", nullptr, window_flags))
		{
			// => <main loop window> menu bar
#pragma region Main Menu Bar

			// get small font, create main menu bar
			ImGui::PushFont(psFontSmall);
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					static bool s_bExtern = false;
					if (ImGui::MenuItem("Save...", "CTRL+S"))
					{
						// set config bool
						m_pcTransferSite->m_pConfig->bExternalSave = (BOOL)s_bExtern;

						// and save...
						m_pcTransferSite->m_pFileManager->SaveWorkingArea(m_pcTransferSite->m_pConfig, &m_paNodes, MAX_INTERFACES_NUMBER);
					}
					if (ImGui::MenuItem("Compile...", "CTRL+C")) 
					{
						// set config bool
						m_pcTransferSite->m_pConfig->bExternalSave = (BOOL)s_bExtern;

						// compile profile
						m_pcTransferSite->m_pFileManager->CompileProfile(m_pcTransferSite->m_pConfig, &m_paNodes, MAX_INTERFACES_NUMBER);
					}
					ImGui::Separator();
					ImGui::MenuItem("Extern Save", NULL, &s_bExtern);
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit"))
				{
					if (ImGui::MenuItem("Reinstate", "CTRL+Z")) 
					{
						CreateThread(NULL, 0, m_pcTransferSite->m_pD3D9ReinstateInterfaces, NULL, 0, NULL);
						//CreateThread(NULL, 0, m_pcTransferSite->m_pD3D929ReinstateInterfaces, NULL, 0, NULL);
						CreateThread(NULL, 0, m_pcTransferSite->m_pD3D10ReinstateInterfaces, NULL, 0, NULL);
						CreateThread(NULL, 0, m_pcTransferSite->m_pD3D11ReinstateInterfaces, NULL, 0, NULL);
					}
					if (ImGui::MenuItem("Delete loose", "CTRL+Y")) 
					{
						// loop through nodes, delete all unconnected
						for (UINT unI = 0; unI < (UINT)m_paNodes.size(); unI++)
						{
							bool bConnected = false;
							// test all decommanders for connection
							for (UINT unJ = 0; unJ < (UINT)m_paNodes[unI]->m_paDecommanders.size(); unJ++)
								if (m_paNodes[unI]->m_paDecommanders[unJ]->m_paCommanders.size())
									bConnected = true;
							// test all commanders for connection
							for (UINT unJ = 0; unJ < (UINT)m_paNodes[unI]->m_paCommanders.size(); unJ++)
								if (m_paNodes[unI]->m_paCommanders[unJ]->m_paDecommanders.size())
									bConnected = true;
							// test provoker, invoker
							if (m_paNodes[unI]->GetProvokerConnectionsNumber() || m_paNodes[unI]->GetInvokerConnectionsNumber())
								bConnected = true;

							// exchange index with last node
							if ((!bConnected) && (unI < (m_paNodes.size() - 1)))
							{
								NOD_Basic* pcNodeToDelete = m_paNodes[unI];
								NOD_Basic* pcNodeToKeep = m_paNodes[m_paNodes.size() - 1];

								// set new index
								pcNodeToKeep->SetNewIndex((DWORD)unI);

								// set nodes new
								m_paNodes[unI] = pcNodeToKeep;
								m_paNodes[m_paNodes.size() - 1] = pcNodeToDelete;
							}

							if (!bConnected)
							{
								// delete last
								m_paNodes.erase(m_paNodes.end() - 1);

								// back one index to also verify the node to be kept
								unI--;
							}
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

#pragma endregion
			// => <main loop window> info text
#pragma region Info Text Background

			// get start y position, output description and a separator line... menu bar height ~ 1.2xFontSize
			float fStartY = ImGui::GetFontSize();
			ImVec2 sStart_rect_max = { 0.0f, fStartY * 1.2f };
			ImVec2 sStart_rect_text = { fStartY * .5f, fStartY * 2.5f };
			ImGui::SetCursorPos(sStart_rect_text);
			ImGui::Text("Aquilinus : Vireio Perception 3D Modification Studio");
			ImGui::Separator();

			// output injected interfaces info
			for (int i = 0; i < SUPPORTED_INTERFACES_NUMBER; i++)
			{
				if (m_pcTransferSite->m_pConfig->eInjectionTechnique[i] != AQU_InjectionTechniques::NoInjection)
				{
					// create text buffers
					char buffer[64];
					wsprintfA(buffer, "%d", m_pcTransferSite->m_anInterfaceRefCount[i]);
					std::wstring acBuffW(AQU_SUPPORTEDINTERFACES::g_sSupportedInterfaces[i].szName, 0, 64);
					std::string acBuffA;
					for (wchar_t c : acBuffW) acBuffA += (char)c;

					// output interface ref count
					ImGui::Text(buffer);

					// adjust spacing and output interface name
					ImGui::SameLine(50.0f); ImGui::Text(acBuffA.c_str());
				}
			}

#pragma endregion
			// => <main loop window> categories
#pragma region Categories

			// get last y position
			ImVec2 sChild_rect_max = ImGui::GetItemRectMax();
			sChild_rect_max.x = 0.0f;
			sChild_rect_max.y += ImGui::GetFontSize();

			// loop through categories
			for (std::vector<AQU_DataSheetEntry*>::size_type h = 0; h != m_pcTransferSite->m_paDataSheetCategories.size(); h++)
			{
				// set next window start position
				ImGui::SetNextWindowPos(sChild_rect_max, ImGuiCond_FirstUseEver);
				ImVec2 sSize = { 240.0f, 100.0f };
				ImGui::SetNextWindowSize(sSize, ImGuiCond_FirstUseEver);
				sChild_rect_max.y += sSize.y;

				// new category window
				std::string acTitleA;
				std::wstring acTitleW(m_pcTransferSite->m_paDataSheetCategories[h]->m_szTitle);
				for (wchar_t c : acTitleW) acTitleA += (char)c;
				if (ImGui::Begin(acTitleA.c_str()))
				{
					// loop through entries
					for (std::vector<AQU_DataSheetEntry*>::size_type i = 0; i != m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries.size(); i++)
					{
						// entry as selectable, first convert to std::string()
						std::wstring acSelectableW(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_szTitle);
						std::string acSelectableA;
						for (wchar_t c : acSelectableW) acSelectableA += (char)c;
						ImGui::Selectable(acSelectableA.c_str(), (m_nDataSheetEntrySelection == (int)i) && (m_nDataSheetCategorySelection == (int)h));

						// sub entries ?
						if (m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_aacSubEntries.size() > 0)
						{
							// output sub entries if hovered
							if (ImGui::IsItemHovered())
							{
								ImGui::BeginTooltip();
								ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
								// output sub-entries
								for (int j = 0; j < (int)m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_aacSubEntries.size(); j++)
								{
									std::wstring acSubW(m_pcTransferSite->m_paDataSheetCategories[h]->m_paEntries[i]->m_aacSubEntries[j]);
									std::string acSubA;
									for (wchar_t c : acSubW) acSubA += (char)c;
									ImGui::Text(acSubA.c_str());
								}
								ImGui::PopTextWrapPos();
								ImGui::EndTooltip();
							}
						}

						// create drag and drop source
						if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoHoldToOpenOthers))
						{
							// set selection as DnD source
							m_nDataSheetEntrySelection = (int)i;
							m_nDataSheetCategorySelection = (int)h;

							// set interface + method as info text
							ImGui::Text("%s::%s", acTitleA.c_str(), acSelectableA.c_str());

							// create ImNodes compatible payload data (empty... just for compatiblity)
							struct _DragConnectionPayload
							{
								void* node_id = nullptr;
								const char* slot_title = nullptr;
								int slot_kind = 0;
							} drag_data{};

							drag_data.node_id = nullptr;
							drag_data.slot_kind = 0;
							drag_data.slot_title = nullptr;

							ImGui::SetDragDropPayload("Aqu_DnD", &drag_data, sizeof(drag_data));
							ImGui::EndDragDropSource();
						}


					}
				}

				// adjust window minimum y position
				ImVec2 sPos = ImGui::GetWindowPos();
				if (sPos.y < sStart_rect_max.y) { sPos.y = sStart_rect_max.y; ImGui::SetWindowPos(sPos); }

				ImGui::End();

			}

#pragma endregion
			// => <main loop window> node creation
#pragma region Node creation (Drag n Drop Target)

			// create a node editor canvas
			static ImNodes::CanvasState canvas = {};
			ImVec2 sPosCanvas = sStart_rect_max;
			sPosCanvas.x += 300.f;
			ImGui::SetNextWindowPos(sPosCanvas, ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2((float)m_nWindowWidth - sPosCanvas.x, (float)m_nWindowHeight - sPosCanvas.y), ImGuiCond_FirstUseEver);
			if (ImGui::Begin("ImNodes", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
			{
				ImNodes::BeginCanvas(&canvas);

				// draw a cross in the center of the canvas...
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				ImVec2 pos = ImGui::GetWindowPos();

				// ...(canvas grid color is ImGuiCol_Separator, so set a much lighter color here)...
				ImU32 grid_color = ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]);
				draw_list->AddLine(ImVec2(canvas.offset.x, canvas.offset.y - 50.f) + pos, ImVec2(canvas.offset.x, canvas.offset.y + 50.f) + pos, grid_color);
				draw_list->AddLine(ImVec2(canvas.offset.x - 50.f, canvas.offset.y) + pos, ImVec2(canvas.offset.x + 50.f, canvas.offset.y) + pos, grid_color);

				// ...and a small circle (center by (+.5f,+.5f)?)
				draw_list->AddCircle(canvas.offset + pos + ImVec2(.5f, .5f), 10.f, grid_color, 0, 2.f);

				// get old position, set position by mouse cursor
				ImVec2 sPosOld = ImGui::GetCursorPos();
				ImVec2 sTargetPos = ImGui::GetMousePos();
				sTargetPos.x -= ImGui::GetWindowPos().x;
				sTargetPos.y -= ImGui::GetWindowPos().y;
				ImGui::SetCursorPos(sTargetPos);

				// create fake selectable as DnD target
				const char* name1 = " ";
				ImGui::Selectable(name1, false, ImGuiSelectableFlags_Disabled);

				// subtract canvas offset for Drag n Drop
				sTargetPos.x -= canvas.offset.x;
				sTargetPos.y -= canvas.offset.y;

				// create drag and drop target
				if (ImGui::BeginDragDropTarget())
				{
					// create node
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Aqu_DnD", ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
					{
						// create node if d3d interface method selected in the data sheet
						if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1) && (m_nDataSheetCategorySelection < (int)m_pcTransferSite->m_aInterfaceIndices.size()))
						{
							// node already present ?
							if (!m_pcTransferSite->IsD3DNodePresent((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection], m_nDataSheetEntrySelection))
							{
								// get node provider
								AQU_NodeProvider* pProvider = new AQU_NodeProvider();

								// create node
								NOD_Basic* pNode = pProvider->Get_D3D_Node((AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection],
									(UINT)m_nDataSheetEntrySelection,
									(LONG)sTargetPos.x,
									(LONG)sTargetPos.y);

								// delete provider
								delete pProvider;

								// convert strings
								pNode->ConvertStrings();

								// register the node
								m_pcTransferSite->RegisterD3DNode(pNode,
									(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces)m_pcTransferSite->m_aInterfaceIndices[m_nDataSheetCategorySelection],
									m_nDataSheetEntrySelection);

								// add node
								m_paNodes.push_back(pNode);

							}
						}
						// create elementary node ?
						else if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1) && (m_nDataSheetCategorySelection == m_pcTransferSite->m_nElementaryNodesTabIndex))
						{
							// get node provider
							AQU_NodeProvider* pProvider = new AQU_NodeProvider();

							// create node
							NOD_Basic* pNode = pProvider->Get_Elementary_Node((AQU_ElementaryNodes)m_nDataSheetEntrySelection,
								(LONG)sTargetPos.x,
								(LONG)sTargetPos.y);

							// delete provider
							delete pProvider;

							// convert strings
							pNode->ConvertStrings();

							// add node
							m_paNodes.push_back(pNode);
						}
						// create plugin node ?
						else if ((m_nDataSheetCategorySelection > -1) && (m_nDataSheetEntrySelection > -1))
						{
							// get plugin node index
							DWORD dwIndex = 0;
							for (int i = 0; i < (int)m_pcTransferSite->m_aPluginCategoryIndices.size(); i++)
							{
								if (m_pcTransferSite->m_aPluginCategoryIndices[i] == m_nDataSheetCategorySelection)
								{
									if (m_pcTransferSite->m_aPluginEntryIndices[i] == m_nDataSheetEntrySelection)
										dwIndex = (DWORD)i;
								}
							}

							// create plugin node
							NOD_Plugin* pPlugin = new NOD_Plugin(
								(LONG)sTargetPos.x,
								(LONG)sTargetPos.y,
								m_vcPluginFilePathes[dwIndex].c_str());

							// convert strings
							pPlugin->ConvertStrings();

							// add node
							m_paNodes.push_back(pPlugin);
						}
					}
					ImGui::EndDragDropTarget();
				}

				// set old position, continue
				ImGui::SetCursorPos(sPosOld);

#pragma endregion
			// => <main loop window> connectors + nodes
#pragma region connectors + nodes

				// update / draw the nodes
				for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
				{
					if (ImNodes::BeginNode((void*)m_paNodes[i], &m_paNodes[i]->m_sPos, &m_paNodes[i]->m_bActive))
					{
						// set node size X ... add space for slots
						ImVec2 sSize = m_paNodes[i]->GetNodeSize();
						sSize.x += m_paNodes[i]->GetSlotSpace();
						sSize.y = 0.f;
						ImGui::ItemSize(sSize);

						// set size state, center node title
						auto* storage = ImGui::GetStateStorage();
						float node_width = sSize.x;
						storage->SetFloat(ImGui::GetID("node-width"), node_width);
						if (node_width > 0)
						{
							// center node title
							ImVec2 title_size = m_paNodes[i]->GetNodeHeaderTextSize();
							if (node_width > title_size.x)
								ImGui::SetCursorPosX(ImGui::GetCursorPosX() + node_width / 2.f - title_size.x / 2.f);
						}

						// Update node and render node title.. 
						m_paNodes[i]->Update();
						ImGui::BeginGroup();

						// backup cursor position
						ImVec2 sPos = ImGui::GetCursorPos();

						ImGui::BeginGroup();
						{
							// decommander slots
							int nSum = (int)m_paNodes[i]->m_paDecommanders.size();
							for (int j = 0; j < nSum; j++)
							{
								// create slot
								ImNodes::BeginSlot(m_paNodes[i]->m_paDecommanders[j]->m_szTitleA, ImNodes::InputSlotKind((int)m_paNodes[i]->m_paDecommanders[j]->m_ePlugtype));
								m_paNodes[i]->InputSlot(m_paNodes[i]->m_paDecommanders[j]->m_szTitleA, ImNodes::InputSlotKind((int)m_paNodes[i]->m_paDecommanders[j]->m_ePlugtype));
								ImNodes::EndSlot();
							}

							// invoker
							if (m_paNodes[i]->HasInvoker())
							{
								ImNodes::BeginSlot("Invoke", ImNodes::InputSlotKind((int)NOD_Plugtype::AQU_PROVOKE));
								m_paNodes[i]->InputSlot("Invoke", ImNodes::InputSlotKind((int)NOD_Plugtype::AQU_PROVOKE));
								ImNodes::EndSlot();
							}
						}
						ImGui::EndGroup();

						// reset cursor position
						ImGui::SetCursorPos(sPos);

						ImGui::BeginGroup();
						{
							int nSum = (int)m_paNodes[i]->m_paCommanders.size();
							for (int j = 0; j < nSum; j++)
							{
								// create slot
								ImNodes::BeginSlot(m_paNodes[i]->m_paCommanders[j]->m_szTitleA, ImNodes::OutputSlotKind((int)m_paNodes[i]->m_paCommanders[j]->m_ePlugtype));
								m_paNodes[i]->OutputSlot(m_paNodes[i]->m_paCommanders[j]->m_szTitleA, ImNodes::OutputSlotKind((int)m_paNodes[i]->m_paCommanders[j]->m_ePlugtype));
								ImNodes::EndSlot();
							}

							// provoker
							if (m_paNodes[i]->HasProvoker())
							{
								ImNodes::BeginSlot("Provoke", ImNodes::OutputSlotKind((int)NOD_Plugtype::AQU_PROVOKE));
								m_paNodes[i]->OutputSlot("Provoke", ImNodes::OutputSlotKind((int)NOD_Plugtype::AQU_PROVOKE));
								ImNodes::EndSlot();
							}
						}
						ImGui::EndGroup();

						ImGui::EndGroup();
						ImNodes::EndNode();
					}

					// draw commander-decommander connections
					for (std::vector<NOD_Commander*>::size_type j = 0; j != m_paNodes[i]->m_paCommanders.size(); j++)
					{
						for (std::vector<NOD_Decommander*>::size_type k = 0; k != m_paNodes[i]->m_paCommanders[j]->m_paDecommanders.size(); k++)
						{
							// get destination node index and update connection
							LONG lDestNodeIndex = m_paNodes[i]->m_paCommanders[j]->m_paDecommanders[k]->m_lNodeIndex;
							ImNodes::Connection((void*)m_paNodes[lDestNodeIndex], m_paNodes[i]->m_paCommanders[j]->m_paDecommanders[k]->m_szTitleA, 
								(void*)m_paNodes[i], m_paNodes[i]->m_paCommanders[j]->m_szTitleA);
						}
					}

					// draw provoker-invoker connections
					for (std::vector<NOD_Invoker*>::size_type j = 0; j != m_paNodes[i]->m_cProvoker.m_paInvokers.size(); j++)
					{
						// get destination node index and update connection
						LONG lDestNodeIndex = m_paNodes[i]->m_cProvoker.m_paInvokers[j]->m_lNodeIndex;
						ImNodes::Connection((void*)m_paNodes[lDestNodeIndex], "Invoke", (void*)m_paNodes[i], "Provoke");
					}
				}

				// new connection ?
				NOD_Basic* pNodeIn = nullptr;
				NOD_Basic* pNodeOut = nullptr;
				const char* pcInput_slot = nullptr;
				const char* pcOutput_slot = nullptr;
				if (ImNodes::GetNewConnection((void**)&pNodeIn, &pcInput_slot, (void**)&pNodeOut, &pcOutput_slot))
				{
					if ((!pNodeIn) || (!pcInput_slot) || (!pNodeOut) || (!pcOutput_slot)) break;
					std::string acIn = std::string(pcInput_slot);
					std::string acOut = std::string(pcOutput_slot);
					
					// get the input node index 
					int nIx = -1;
					for (int nI = 0; nI < (int)m_paNodes.size(); nI++)
						if (pNodeIn == m_paNodes[nI]) nIx = nI;

					if ((acIn.compare("Invoke") == 0) && (acOut.compare("Provoke") == 0))
					{
						// connect invoker
						if (nIx > -1)
							pNodeOut->ConnectInvoker(pNodeIn, (LONG)nIx);
						else
							OutputDebugString(L"Aquilinus: (Error) Node not listed !!");
					}
					else
					{
						// connect decommander
						if (nIx > -1)
							pNodeOut->ConnectDecommander(pNodeIn, (LONG)nIx, acOut, acIn);
						else
							OutputDebugString(L"Aquilinus: (Error) Node not listed !!");
					}

				}

				ImNodes::EndCanvas();
			}
			ImGui::End();

#pragma endregion

			ImGui::PopFont();
		}
		else
		{
			OutputDebugString(L"Aquilinus: ImGui failed to create window !");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		ImGui::End();

		// => <main loop> render
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	// => <main> cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();

	// cleanup nodes vector
	for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
	{
		delete m_paNodes[i];
	}
	m_paNodes.clear();

	return (DWORD)0;
}

/// <summary>
/// => Load work space
/// Loads all workspace data from the file specified in the aquilinus configuration.
/// </summary>
HRESULT AQU_WorkingArea::s_LoadWorkSpace()
{
	// set d3d override to true
	m_pcTransferSite->m_bForceD3D = true;

	// get a data stream
	std::stringstream sstrDataStream;

	// load the stream
	HRESULT hr = m_pcTransferSite->m_pFileManager->LoadWorkingArea(m_pcTransferSite->m_pConfig->szWorkspaceFilePath, sstrDataStream);

	// and decode the stream
	if (SUCCEEDED(hr))
	{
		// first, read the game name to see wether this is an empty profile
		wchar_t szEntryName[MAX_JOLIET_FILENAME];
		sstrDataStream.read((char*)szEntryName, ENTRY_SIZE);

		// empty profile ?
		if (szEntryName[0] == 0)
			m_pcTransferSite->m_pConfig->bEmptyProcess = TRUE;
		else
			m_pcTransferSite->m_pConfig->bEmptyProcess = FALSE;

		// now, ignore the size of the additional option data block
		sstrDataStream.ignore(sizeof(unsigned __int32) * OPTIONS_RESERVED);

		// ignore picture boolean and the path if true
		BOOL bPicture;
		sstrDataStream.read((char*)&bPicture, sizeof(BOOL));
		if (bPicture)
			sstrDataStream.ignore(MAX_PATH * sizeof(wchar_t));

		// ignore the detour time delay and injection repetition
		sstrDataStream.ignore(sizeof(unsigned __int32));
		sstrDataStream.ignore(sizeof(unsigned __int32));

		// now, ignore the injection techniques
		unsigned __int32 dwSupportedInterfacesNumber;
		sstrDataStream.read((char*)&dwSupportedInterfacesNumber, sizeof(unsigned __int32));
		sstrDataStream.ignore((std::streamsize)dwSupportedInterfacesNumber * (std::streamsize)sizeof(__int32));

		// read the number of nodes, resize node vector
		unsigned __int32 dwNodeNumber;
		sstrDataStream.read((char*)&dwNodeNumber, sizeof(unsigned __int32));
		m_paNodes.resize(dwNodeNumber);

		// get a node provider
		AQU_NodeProvider* pProvider = new AQU_NodeProvider();

		// loop through the nodes to add node data 
		for (UINT i = 0; i != dwNodeNumber; i++)
		{
			OutputDebugString(L"New Node!");

			// get the node hash
			unsigned __int32 id;
			sstrDataStream.read((char*)&id, sizeof(unsigned __int32));

			// get the node position
			ImVec2 sPos = {};
			sstrDataStream.read((char*)&sPos.x, sizeof(float));
			sstrDataStream.read((char*)&sPos.y, sizeof(float));

			// load plugin info (if plugin node)
			unsigned __int32 idPlugin = 0;
			wchar_t szFileName[64];
			wchar_t szFilePath[MAX_PATH];
			if (id == ELEMENTARY_NODE_PLUGIN)
			{
				// and write to stream
				sstrDataStream.read((char*)&idPlugin, sizeof(unsigned __int32));
				sstrDataStream.read((char*)&szFileName[0], sizeof(wchar_t) * 64);
				
				// create full path
				wsprintf(szFilePath, L"%s%s", m_pcTransferSite->m_pFileManager->GetPluginPath(), szFileName);

				OutputDebugString(szFilePath);
			}

			// read node data
			unsigned __int32 dwDataSize = 0;
			sstrDataStream.read((char*)&dwDataSize, sizeof(unsigned __int32));

			// read the data
			char* pcData = new char[dwDataSize];
			if (dwDataSize)
				sstrDataStream.read((char*)pcData, dwDataSize);

			// get a node pointer... TODO !! NODE POSITION FLOAT
			NOD_Basic* pNode;
			if (FAILED(pProvider->Get_Node(pNode, id, (LONG)sPos.x, (LONG)sPos.y, idPlugin, szFilePath)))
			{
				// unregister all nodes
				m_pcTransferSite->UnregisterAllNodes();

				// debug output
				wchar_t buf[64];
				wsprintf(buf, L"Aquilinus : Unknown node type %u", id);
				OutputDebugString(buf);

				for (UINT j = 0; j < i; j++)
					delete m_paNodes[j];

				m_paNodes.clear();
				delete pProvider;
				sstrDataStream.clear();
				return E_FAIL;
			}

			// init node data
			pNode->InitNodeData(pcData, dwDataSize);

			// register this node
			m_pcTransferSite->RegisterD3DNode(pNode, id);

			// and add the node
			m_paNodes[i] = pNode;
			delete [] pcData;
		}

		delete pProvider;

		// loop through the nodes to add node connections
		for (std::vector<NOD_Basic*>::size_type i = 0; i != m_paNodes.size(); i++)
		{
			// get the number of commanders
			unsigned __int32 dwCommandersNumber;
			sstrDataStream.read((char*)&dwCommandersNumber, sizeof(unsigned __int32));

			// loop through commanders, get the number of connections and the connection indices
			for (DWORD j = 0; j < dwCommandersNumber; j++)
			{
				// get the commander connections number
				unsigned __int32 dwConnectionsNumber;
				sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(unsigned __int32));

				// loop through decommanders, set indices
				for (DWORD k = 0; k < dwConnectionsNumber; k++)
				{
					// get decommander node index
					LONG lNodeIndex;
					sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));
					// get decommander index
					LONG lDecommanderIndex;
					sstrDataStream.read((char*)&lDecommanderIndex, sizeof(LONG));

					// output debug data
					OutputDebugString(L"Connect Decommander");
					wchar_t buf[64]; wsprintf(buf, L"nodes : %u %u cix : %u dix : %u", i, lNodeIndex, j, lDecommanderIndex); OutputDebugString(buf);

					// and connect
					m_paNodes[i]->ConnectDecommander(m_paNodes[lNodeIndex], lNodeIndex, j, lDecommanderIndex);
				}
			}

			// get the provoker connections number
			unsigned __int32 dwConnectionsNumber;
			sstrDataStream.read((char*)&dwConnectionsNumber, sizeof(unsigned __int32));

			// loop through decommanders, return indices
			for (int j = 0; j < (int)dwConnectionsNumber; j++)
			{
				LONG lNodeIndex;
				sstrDataStream.read((char*)&lNodeIndex, sizeof(LONG));

				// output debug data
				OutputDebugString(L"Connect Invoker");
				wchar_t buf[64]; wsprintf(buf, L"nodes : %u %u", i, lNodeIndex); OutputDebugString(buf);

				// and connect
				m_paNodes[i]->ConnectInvoker(m_paNodes[lNodeIndex], lNodeIndex);
			}
		}
	}

	// set d3d override to false
	m_pcTransferSite->m_bForceD3D = false;

	return hr;
}

/// <summary>
/// => Enumerate plugins
/// Enumerates all Node-plugins.
/// <param name="szDllPath">Aquilinus plugin path</param>
/// </summary>
HRESULT AQU_WorkingArea::s_EnumeratePlugins(LPCWSTR szDllPath)
{
	WIN32_FIND_DATA ffd;
	wchar_t szDir[MAX_PATH];
	HANDLE hFind = INVALID_HANDLE_VALUE;

	// clear enumeration vectors
	m_vcPluginNames.clear();
	m_vcPluginFilePathes.clear();
	m_vcPluginIDs.clear();
	m_vcPluginCategories.clear();
	m_vcPlugins.clear();
	m_vcPluginHandles.clear();

	// create plugins path
	wsprintf(szDir, L"%s%s", szDllPath, L"*");
//#ifdef _DEBUG
	OutputDebugString(szDir);
//#endif

	// find first file
	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
		return E_FAIL;

	// loop through files
	do
	{
		// get the extension
		const wchar_t* dot = wcsrchr(ffd.cFileName, '.');
		if (!dot || dot == ffd.cFileName) dot = L"";

		// ".dll" ?? 
		if (wcscmp(dot, L".dll") == NULL)
		{
			wchar_t szDllDir[MAX_PATH];

			OutputDebugString(L"Dll found :");
			OutputDebugString(ffd.cFileName);

			// create plugin path and load module
			wsprintf(szDllDir, L"%s%s", szDllPath, ffd.cFileName);
			HMODULE hm = LoadLibraryW(szDllDir);

			if (hm != NULL)
			{
				// define plugin creation type
				typedef AQU_Nodus* (*AQU_Nodus_Create)();
				AQU_Nodus_Create m_pAQU_Nodus_Create;

				// get plugin creation method
				m_pAQU_Nodus_Create = (AQU_Nodus_Create)GetProcAddress(hm, "AQU_Nodus_Create");

				if (m_pAQU_Nodus_Create)
				{
					// create a test nodus class to test for directx version support
					AQU_Nodus* pNodus = m_pAQU_Nodus_Create();

					// get the plugin name
					LPSTR szName = new char[64];
					const char* szSrcName = pNodus->GetNodeType();
					UINT dwLen = (UINT)strlen(szSrcName);
					if (dwLen > 63) dwLen = 63;
					CopyMemory((void*)szName, (void*)szSrcName, dwLen + 1);

					// get the path name
					LPWSTR szPathName = new wchar_t[MAX_PATH];
					dwLen = (UINT)wcslen(szDllDir);
					if (dwLen > MAX_PATH - 1) dwLen = MAX_PATH - 1;
					CopyMemory((void*)szPathName, (void*)szDllDir, (dwLen + 1) * sizeof(wchar_t));

					// enumerate all necessary data
					std::string acTmp = std::string(szName);
					m_vcPluginNames.push_back(std::wstring(acTmp.begin(), acTmp.end()));
					m_vcPluginFilePathes.push_back(szPathName);
					m_vcPluginIDs.push_back(pNodus->GetNodeTypeId());
					m_vcPluginCategories.push_back(std::wstring(pNodus->GetCategory()));

					// keep the prototypes to delete them at class destruction
					m_vcPlugins.push_back(pNodus);
				}

				m_vcPluginHandles.push_back(hm);
			}
			else MessageBox(NULL, ffd.cFileName, L"Failed to load !", MB_OK);

		}

	} while (FindNextFile(hFind, &ffd) != 0);

	// output debug data
	for (int i = 0; i < (int)m_vcPluginNames.size(); i++)
	{
		OutputDebugStringW(m_vcPluginNames[i].c_str());
		OutputDebugStringW(m_vcPluginFilePathes[i].c_str());
		OutputDebugStringW(m_vcPluginCategories[i].c_str());
	}

	return S_OK;
}
