#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <Windows.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>

#include <map>
#include "core.h"
#include "scenes.h"

namespace sarklib{

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

		// the running state of engine
		bool mbRunning;

		// window size
		unsigned int mnWndWidth, mnWndHeight;

		// color buffer clear color
		Color mClearColor;
		
		// it decides which projection method is used
		// that true to orthogonal projection, false to perspective projection
		bool mbIs2D;


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
		bool InitializeApp(HINSTANCE hInstance, int nCmdShow, std::wstring strClassName, std::wstring strAppName);

		// run engine loop
		void Run();

		// pause engine loop
		void Pause();

		// release application
		void ReleaseApp();

	private:
		// enable opengl device and rendering context
		bool EnableGLContext();

		// disable opengl device and rendering context
		void DisableGLContext();

		// setup opengl state
		void SetupGL();

	private:
		// update current scene and other frame depentent components
		void Update();

		// render current scene
		void Render();


		// ---------- configuration methods ------------
	public:
		bool AddScene(const std::string& sceneName, AScene* scene);
		bool SetCurrentScene(const std::string& sceneName);

		void SetClearColor(const Color& color);
		void SetViewMode(bool as2D);

		void Resize(unsigned int width, unsigned int height);

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	};

}
#endif