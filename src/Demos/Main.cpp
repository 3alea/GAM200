#include "src\Engine\AEX.h"
#include "src\Engine\Graphics\GfxMgr.h"
#include "src\Engine\Graphics\WindowMgr.h"
#include <iostream>
#include "Simple Demo\SimpleDemo.h"
#include "JsonDemo\JsonDemo.h"
#include "ImGuiDemo\ImGuiDemo.h"
#include "OpenGLDemo\OpenGLDemo.h"

void main(void)
{
	assert(GfxMgr->Initialize());
	aexEngine->Initialize();

	aexEngine->Run(new OpenGLDemo);
	AEX::AEXEngine::ReleaseInstance();

	GfxMgr->Shutdown();
	WindowMgr->Shutdown();
}
