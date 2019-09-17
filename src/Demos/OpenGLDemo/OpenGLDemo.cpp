#include "src\Engine\Composition\AEXObjectManager.h"
#include "src\Engine\Logic\AEXGameState.h"
#include "src\Engine\Graphics\GfxMgr.h"
#include "src\Engine\Graphics\WindowMgr.h"
#include "OpenGLDemo.h"

void OpenGLDemo::Initialize()
{
	ObjMgr->Initialize();
	ObjMgr->CreateGO("lol");
}

void OpenGLDemo::LoadResources()
{
	GfxMgr->CreateShader("test", "data/Shaders/TextureColor.vs", "data/Shaders/TextureColor.fs");
}

void OpenGLDemo::Update()
{
	GfxMgr->Update();
	WindowMgr->Update();
}

void OpenGLDemo::Render()
{
	GfxMgr->Render();
}

void OpenGLDemo::FreeResources()
{
}
