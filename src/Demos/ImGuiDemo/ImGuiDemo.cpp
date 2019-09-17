#include "src\Engine\AEX.h"
#include "ImGuiDemo.h"
#include <iostream>
#include "./src/Engine/Imgui/imgui.h"
#include "./src/Engine/Composition/AEXObjectManager.h"
#include "./src/Engine/Composition/AEXFactory.h"
#include <fstream>
using namespace AEX;

//gl includes
#include "Extern\GL\glew.h"
#include "Extern\GL\wglew.h"
#include "extern\GL\GL.h"

// HELPER FUNCTIONS DECLARATIONS

static GLuint       g_FontTexture = 0;
static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
static unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;

static bool			ImGui_ShowcaseHelpWindow = false;

void CreateFontTexture()
{
	ImGuiIO& io = ImGui::GetIO();

	// Build texture atlas
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

															  // Create OpenGL texture
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Cleanup (don't clear the input data if you want to append new fonts later)
	io.Fonts->ClearInputData();
	io.Fonts->ClearTexData();
}
void CreateGUIDeviceObject()
{
	// Backup GL state
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

	const GLchar *vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
	glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
	glCompileShader(g_VertHandle);
	glCompileShader(g_FragHandle);
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

	glGenBuffers(1, &g_VboHandle);
	glGenBuffers(1, &g_ElementsHandle);

	glGenVertexArrays(1, &g_VaoHandle);
	glBindVertexArray(g_VaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
	glEnableVertexAttribArray(g_AttribLocationPosition);
	glEnableVertexAttribArray(g_AttribLocationUV);
	glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	CreateFontTexture();

	// Restore modified GL state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindVertexArray(last_vertex_array);
}
void InitializeImGui()
{
	// Create context
	ImGui::CreateContext();

	// initialize IO
	ImGuiIO & io = ImGui::GetIO();

	// keyboard mapping from Win32 input to Imgui input 
	{
		io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
		io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
		io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
		io.KeyMap[ImGuiKey_Home] = VK_HOME;
		io.KeyMap[ImGuiKey_End] = VK_END;
		io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
		io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = 'A';
		io.KeyMap[ImGuiKey_C] = 'C';
		io.KeyMap[ImGuiKey_V] = 'V';
		io.KeyMap[ImGuiKey_X] = 'X';
		io.KeyMap[ImGuiKey_Y] = 'Y';
		io.KeyMap[ImGuiKey_Z] = 'Z';
	}

	// pass the win32 handle.
	io.ImeWindowHandle = AEX::WindowManager::Instance()->GetMainWindow()->GetHandle();

	// Create GUI objects
	CreateGUIDeviceObject();
}
void UpdateGui()
{
	ImGuiIO& io = ImGui::GetIO();
	AEX::Window * win = AEX::WindowManager::Instance()->GetMainWindow();

	// Setup display size (every frame to accommodate for window resizing)
	int w = win->GetWidth(), h = win->GetHeight();
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(1, 1);

	// Setup time step
	io.DeltaTime = (f32)AEX::FRC::Instance()->GetFrameTime();

	// Setup inputs
	if (aexInput->MouseInWindow())
	{
		::POINT mp;
		::GetCursorPos(&mp);
		::ScreenToClient(win->GetHandle(), &mp);
		io.MousePos = ImVec2((float)mp.x, (float)mp.y);   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
	}
	else
	{
		io.MousePos = ImVec2(-1, -1);
	}

	for (int i = 0; i < 2; i++)
	{
		io.MouseDown[i] = aexInput->MousePressed(i);
	}

	io.MouseWheel = aexInput->GetMouseWheel();

	// Reset all keyboard inputs to false
	for (int i = 0; i < AEX_INPUT_KEY_NUM; ++i)
		io.KeysDown[i] = false;

	// set the keys that were pressed this frame
	const std::vector<u8> & keyspressed = Input::Instance()->AllKeyPressed();
	FOR_EACH(it, keyspressed)
	{
		io.KeysDown[*it] = true;
	}

	//pass in text data
	// @TODO : This is far from being complete. Add whatever else you need
	bool caps = LOWORD(::GetKeyState(VK_CAPITAL)) == 1;
	bool shift = Input::Instance()->KeyPressed(Input::eShift);

	// get the triggered keys (keys down ONLY THIS frame)
	const std::vector<u8> & keystriggered = Input::Instance()->AllKeyTriggered();
	FOR_EACH(it, keystriggered)
	{
		if (*it == VK_DELETE ||
			*it == VK_BACK ||
			*it == VK_LEFT ||
			*it == VK_RIGHT ||
			*it == VK_UP ||
			*it == VK_DOWN
			)
			continue;

		if (!caps && !shift && *it >= 'A' && *it <= 'Z')
			io.AddInputCharacter(*it + 32);
		else
			io.AddInputCharacter(*it);
	}

	io.KeyCtrl = Input::Instance()->KeyPressed(Input::eControl);
	io.KeyShift = Input::Instance()->KeyPressed(Input::eShift);


	// Hide OS mouse cursor if ImGui is drawing it

	// Start the frame
	ImGui::NewFrame();
}
void DrawGui()
{
	// call IMGui Render
	ImGui::Render();

	// get the draw list and draw everything
	ImDrawData * draw_data = ImGui::GetDrawData();

	// Backup GL state
	GLint last_program, last_texture, last_array_buffer, last_element_array_buffer, last_vertex_array;
	GLboolean depth_test_enabled, cull_face_enabled, blend_enabled;
	glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	glGetBooleanv(GL_DEPTH_TEST, &depth_test_enabled);
	glGetBooleanv(GL_CULL_FACE, &cull_face_enabled);
	glGetBooleanv(GL_BLEND, &blend_enabled);


	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	if (!blend_enabled)	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	if (cull_face_enabled)	glDisable(GL_CULL_FACE);
	if (depth_test_enabled) glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Handle cases of screen coordinates != from framebuffer coordinates (e.g. retina displays)
	ImGuiIO& io = ImGui::GetIO();
	float fb_height = io.DisplaySize.y * io.DisplayFramebufferScale.y;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Setup orthographic projection matrix
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.0f, 0.0f },
		{ -1.0f, 1.0f, 0.0f, 1.0f },
	};
	glUseProgram(g_ShaderHandle);
	glUniform1i(g_AttribLocationTex, 0);
	glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindVertexArray(g_VaoHandle);

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

		for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
		{
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, GL_UNSIGNED_SHORT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram(last_program);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBindVertexArray(last_vertex_array);
	glDisable(GL_SCISSOR_TEST);

	if (!blend_enabled)glDisable(GL_BLEND);
	if (cull_face_enabled)glEnable(GL_CULL_FACE);
	if (depth_test_enabled)glEnable(GL_DEPTH_TEST);
}

void  Transform_OnGui(AEX::Transform & tr)
{
	ImGui::DragFloat3("Position", tr.mTranslationZ.v, 0.5f, -10000.0f, 10000.0f);
	ImGui::DragFloat2("Scale", tr.mScale.v, 0.5f, -10000.0f, 10000.0f);
	ImGui::DragFloat("Rotation", &tr.mOrientation, 0.5f, -10000.0f, 10000.0f);
}

AEX::Transform trans;

void MyEditor()
{
	if (ImGui_ShowcaseHelpWindow)
		ImGui::ShowDemoWindow();

	if (ImGui::BeginMainMenuBar()) // Main Menu (top-horizontal bar)
	{
		if (ImGui::BeginMenu("File")) // Main Menu block
		{
			if (ImGui::BeginMenu("New", "Ctrl + N")) // Main menu block Item
			{
				if (ImGui::MenuItem("GameObject"))
				{
					aexFactory->Register<GameObject>();
					aexFactory->Create(GameObject::TYPE().GetName());
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Open", "Ctrl + O"))
			{
				json j;
				// @TODO : GameState Management: Serializing
				std::string levelName = "Test Level";
				std::ifstream inFile(levelName + ".json");
				if (inFile.good() && inFile.is_open())
				{
					inFile >> j;
					inFile.close();
					ObjectManager::Instance()->operator>>(j[levelName]);
				}
			}

			if (ImGui::MenuItem("Save", "Ctrl + S"))
			{
				json j;
				// @TODO : GameState Management: Serializing
				std::string levelName = "Test Level";
				ObjectManager::Instance()->operator<<(j[levelName]);

				// save to a file. 
				std::ofstream outFile((levelName + ".json"));
				if (outFile.good() && outFile.is_open())
				{
					outFile << std::setw(4) << j;
					outFile.close();
				}
			}

			if (ImGui::MenuItem("Save as..."))
			{

			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) ImGui::EndMenu();
		if (ImGui::BeginMenu("View")) ImGui::EndMenu();

		if (ImGui::BeginMenu("Help"))
		{
			ImGui_ShowcaseHelpWindow = !ImGui_ShowcaseHelpWindow;
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (ImGui::Begin("Object List"))
		ObjectManager::Instance()->OnGui();
	ImGui::End();
}

// ----------------------------------------------------------------------------
// GAMESTATE FUNCTIONS
void ImGuiDemo::Initialize()
{
	// initialize ImGui
	InitializeImGui();
}
void ImGuiDemo::LoadResources()
{
	// Images
	//aexGraphics->LoadTexture(".\\data\\Images\\Default.png");

	// Models
	//aexGraphics->LoadModel(".\\data\\Models\\Quad.model");
}
void ImGuiDemo::Update()
{
	// get main window dimensions
	auto mainWin = aexWindowMgr->GetMainWindow();
	auto winWH = AEVec2(f32(mainWin->GetWidth()), f32(mainWin->GetHeight()));

	// control the engine
	if (aexInput->KeyTriggered('B'))
		aexTime->LockFrameRate(!aexTime->FrameRateLocked());
	/*if (aexInput->KeyTriggered('V'))
		aexGraphics->SetVSyncEnabled(!aexGraphics->GetVSyncEnabled());*/
	if (aexInput->KeyPressed(VK_OEM_PLUS))
		aexTime->SetMaxFrameRate(aexTime->GetMaxFrameRate() + 1.0);
	if (aexInput->KeyPressed(VK_OEM_MINUS))
		aexTime->SetMaxFrameRate(aexTime->GetMaxFrameRate() - 1.0);
	if (aexInput->KeyTriggered('F'))
		mainWin->SetFullScreen(!mainWin->GetFullScreen());

	f32 fps = (f32)aexTime->GetFrameRate();
	std::string wintitle = "Simple Demo - FPS: "; wintitle += std::to_string(fps);
	if (aexTime->FrameRateLocked())	wintitle += "(LOCKED)";
	//wintitle += " - VSYNC: ";	wintitle +=	aexGraphics->GetVSyncEnabled() ? "ON" : "OFF";
	wintitle += " - Controls: FPS: 'B', '+/-'. VSYNC: 'V'";
	aexWindowMgr->GetMainWindow()->SetTitle(wintitle.c_str());


	Logic::Instance()->Update();

}
void ImGuiDemo::Render()
{
	UpdateGui();

	auto mainWin = aexWindowMgr->GetMainWindow();
	auto winWH = AEVec2(f32(mainWin->GetWidth()), f32(mainWin->GetHeight()));

	//aexGraphics->SetViewport(0, 0, s32(winWH.x), s32(winWH.y));
	//aexGraphics->SetClearColor(Color(0.7f,0.7f,0.7f,1.0f));
	//aexGraphics->ClearFrameBuffer();

	MyEditor();

	DrawGui();

	//aexGraphics->Present();
}
