#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <Windows.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#include <string>
#include <map>
#include "core.h"
#include "AScene.h"
#include "Timer.h"
#include "Input.h"

namespace sark{

	// engine is the core single-tone class to make application.
	// it creates window and initializes graphics library(especially OpenGL).
	// almost of all application components like scene, physics system, 
	// devices are handled by this class
	class Engine{
	private:
		// singleton instance
		static Engine* instance;

		// ----- Windows OS dependent properties -----
		
		// instance handler
		HINSTANCE mhInst;
		// window handler
		HWND mhWnd;
		// device context
		HDC mhDC;
		// opengl rendering context
		HGLRC mhRC;

		// ----------- engine properties -------------

		// engine timer
		Timer mTimer;

		// the running state of engine
		bool mbRunning;

		// window size
		uinteger mnWndWidth, mnWndHeight;

		// color buffer clear color
		ColorRGBA mClearColor;


		typedef std::map<std::string, AScene*> SceneContainer;
		// scene container
		SceneContainer mScenes;
		// current scene pointer for performance
		AScene* mCurrentScene;

		
	private:
		// singleton feature
		Engine();
		Engine(const Engine&);
		const Engine& operator=(const Engine&);

	public:
		// get instance of this engine
		static Engine* GetInstance();

		// initialize application
		bool InitializeApp(HINSTANCE hInstance, integer nCmdShow, std::wstring strClassName, std::wstring strAppName);

		// run engine loop
		void Run();

		// get engine timer
		const Timer& GetTimer() const;

		// pause engine loop. message loop is not paused
		bool Pause();

		// resume engine loop
		bool Resume();

		// call a halt to running engine
		void Halt();

		// release application
		void ReleaseApp();

	private:
		// enable opengl device and rendering context
		bool EnableGLContext();

		// disable opengl device and rendering context
		void DisableGLContext();

		// setup opengl state
		void SetupGL();

		// ---------- configuration methods ------------
	public:
		bool AddScene(const std::string& sceneName, AScene* scene, bool asCurrent = false);
		bool SetCurrentScene(const std::string& sceneName);

		void SetClearColor(const ColorRGBA& color);

		void ResizeWindow(uinteger width, uinteger height, bool callOnInside = false);

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	};

}
#endif