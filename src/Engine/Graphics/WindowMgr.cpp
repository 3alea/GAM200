#include <iostream>
#include <cassert>

#include "WindowMgr.h"

Window2::Window2(int & width, int & height, const char * windowName = "New window") : mWidth(width), mHeight(height)
{
	mWindow = glfwCreateWindow(width, height, windowName, NULL, NULL);
	if (mWindow == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(true);
	};

	glfwMakeContextCurrent(mWindow);
}

WindowManager2::WindowManager2() {}

Window2 * WindowManager2::CreateNewWindow(int width = 800, int height = 600, const char * windowName = "New window")
{
	mCurrentWindow = new Window2(width, height, windowName);
	mWindowList.push_front(mCurrentWindow);
	return mCurrentWindow;
}

void WindowManager2::ResizeWindow(Window2 * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void WindowManager2::ProcessInput(Window2 & window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
		
}

bool WindowManager2::Initialize()
{
	std::cout << "WindowMgr : Initialize" << std::endl;
	Window2* window = WindowMgr->CreateNewWindow(800, 600, "test");

	WindowMgr->SetCurrentWindow(window);

	std::cout << "Initializing GLAD" << std::endl;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	
	return true;
}

void WindowManager2::Update()
{

}

void WindowManager2::Shutdown()
{
	for (auto it = mWindowList.begin(); it != mWindowList.end(); it++)
		delete *it;
}
