#pragma once
#include "../Composition/AEXComposition.h"
#include "Shader.h"
#include "../Core/AEXSystem.h"

using namespace AEX;

class GraphicsManager : public ISystem
{
	AEX_RTTI_DECL(GraphicsManager, ISystem);
	AEX_SINGLETON(GraphicsManager);

public:
	bool Initialize();
	void Update();
	void Render();
	void Shutdown();

	Shader* CreateShader(const char*, const char*, const char*);
	Shader* FindShaderByName(const char*);
	void	SetToCurrentShader(const char*);
	void	RemoveShader(const char*);

	Shader* mCurrentShader;
	std::list<std::pair<Shader*, std::string>> mShaderList;
};

#define GfxMgr (GraphicsManager::Instance())