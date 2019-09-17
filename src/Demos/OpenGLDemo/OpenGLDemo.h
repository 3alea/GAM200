#pragma once

class OpenGLDemo : public AEX::IGameState
{
	virtual void Initialize();
	virtual void LoadResources();
	virtual void Update();
	virtual void Render();
	virtual void FreeResources();
};