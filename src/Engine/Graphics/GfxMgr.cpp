#include "../Composition/AEXObjectManager.h"
#include "WindowMgr.h"
#include "GfxMgr.h"

#include <cassert>
#include <iostream>

/*
#pragma region Vertex Shaders
const char *vs_ColorFlicker = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
"out vec4 vertexColor;\n" // specify a color output to the fragment shader
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n" // see how we directly give a vec3 to vec4's constructor
"vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n" // set the output variable to a dark-red color
"}\n\0";


const char *vs_ColorInterp = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"   // the position variable has attribute position 0
"layout(location = 1) in vec3 aColor;\n" // the color variable has attribute position 1
"out vec3 ourColor;\n" // output a color to the fragment shader
"void main()\n"
"{\n"
	"gl_Position = vec4(aPos, 1.0);\n"
	"ourColor = aColor;\n" // set ourColor to the input color we got from the vertex data
"}\n\0";
#pragma endregion

#pragma region Fragment Shaders
const char *fs_ColorFlicker = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n" // we set this variable in the OpenGL code.
"void main()\n"
"{\n"
"FragColor = ourColor;\n"
"}\n\0";

const char *fs_ColorInterp = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
	"FragColor = vec4(ourColor, 1.0);\n"
"}\n\0";
#pragma endregion
*/

GraphicsManager::GraphicsManager() {}

bool GraphicsManager::Initialize()
{
	std::cout << "GfxMgr : Initialize" << std::endl;
	std::cout << "Initializing GLFW" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	WindowMgr->Initialize();

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	return true; // 1
}

void GraphicsManager::Update()
{
	
}

void GraphicsManager::Render()
{
	if (!glfwWindowShouldClose(*WindowMgr->GetCurrentWindow()))
	{
		// Window Input
		WindowMgr->ProcessInput(*WindowMgr->GetCurrentWindow());

		// Rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); // Clear the buffer

		ObjMgr->Render();

		// check and call events (poll IO events) and swap the buffers
		glfwPollEvents();
		glfwSwapBuffers(*WindowMgr->GetCurrentWindow());
	}
	else
		exit(1);
}

void GraphicsManager::Shutdown()
{
	mShaderList.clear();
	glfwTerminate();
}

Shader * GraphicsManager::CreateShader(const char * shaderName, const char * vtxPath, const char * fragPath)
{
	Shader * tempShader = new Shader(vtxPath, fragPath);
	std::pair<Shader*, std::string> tempPair(tempShader, shaderName);
	mShaderList.push_back(tempPair);
	mCurrentShader = tempShader;
	return tempShader;
}

Shader * GraphicsManager::FindShaderByName(const char * shaderName)
{
	for (auto it = mShaderList.begin(); it != mShaderList.end(); it++)
		if (it->second == shaderName)
			return it->first;

	std::cout << "GfxMgr ERROR : Could not find Shader" << std::endl;
	return nullptr;
}

void GraphicsManager::SetToCurrentShader(const char * shaderName)
{
	if (Shader * tempShader = FindShaderByName(shaderName))
		mCurrentShader = tempShader;
}

void GraphicsManager::RemoveShader(const char * shaderName)
{
	for (auto it = mShaderList.begin(); it != mShaderList.end(); it++)
		if (it->second == shaderName)
		{
			std::cout << "GfxMgr : " << it->second << " Shader removed" << std::endl;
			mShaderList.erase(it);
			return;
		}

	std::cout << "GfxMgr ERROR : Could not find/erase Shader" << std::endl;
}
