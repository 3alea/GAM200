#pragma once
#include <./extern/glad/glad.h>
#include <./extern/glfw/glfw3.h>
#include "src\Engine\Core\AEXSystem.h"

using namespace AEX;

class Window2 : public IBase
{
	AEX_RTTI_DECL(Window2, IBase);

	Window2(int & width, int & height, const char * windowName);

public:
	u32 GetWidth() { return mWidth; }
	u32 GetHeight() { return mHeight; }

	operator GLFWwindow*() { return mWindow; }
	
private:
	GLFWwindow * mWindow;
	u32 mWidth;
	u32 mHeight;
};

class WindowManager2 : public ISystem
{
	AEX_RTTI_DECL(WindowManager2, ISystem);
	AEX_SINGLETON(WindowManager2);

public:
	Window2 * CreateNewWindow(int width, int height, const char *windowName);
	void ResizeWindow(Window2 *window, int width, int height);
	void ProcessInput(Window2 & window);

	Window2 * GetCurrentWindow() { return mCurrentWindow; }
	void SetCurrentWindow(Window2 *currentWindow) { mCurrentWindow = currentWindow; }

	bool Initialize();
	void Update();
	void Shutdown();

private:
	Window2 *mCurrentWindow;

	std::list<Window2*> mWindowList;
};

#define WindowMgr (WindowManager2::Instance())
