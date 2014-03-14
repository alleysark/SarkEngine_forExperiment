#include "Engine.h"
#include "Input.h"

namespace sarklib{

	// singleton instance
	Engine* Engine::instance = NULL;
	// get instance of this engine
	Engine* Engine::GetInstance(){
		if (instance == NULL)
			instance = new Engine();
		return instance;
	}


	Engine::Engine() :
		mhInst(NULL), mhWnd(NULL), mhDC(NULL), mhRC(NULL),
		mTimer(false), mbRunning(false), mnWndWidth(0), mnWndHeight(0), mClearColor(0), mbIs2D(false),
		mCurrentScene(NULL)
	{ }

	Engine::Engine(const Engine&){}
	const Engine& Engine::operator=(const Engine&){ return *this; }


	// initialize application
	bool Engine::InitializeApp(HINSTANCE hInstance, integer nCmdShow, std::wstring strClassName, std::wstring strAppName){
		mhInst = hInstance;

		WNDCLASSEX wc;
		ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = strClassName.c_str();
		if (!RegisterClassEx(&wc))
			return false;

		mhWnd = CreateWindow(strClassName.c_str(), strAppName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, mnWndWidth, mnWndHeight,
			NULL, (HMENU)NULL, hInstance, NULL);
		if (mhWnd == NULL)
			return false;

		ShowWindow(mhWnd, nCmdShow);

		if (!EnableGLContext())
			return false;

		glewInit();

		SetupGL();
		return true;
	}

	// run engine loop
	void Engine::Run(){
		mbRunning = true;
		MSG msg;

		mTimer.Resume();
		while (mbRunning){
			if (PeekMessage(&msg, mhWnd, 0, 0, PM_REMOVE)){
				// translate and dispatch messages
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{
				if (mTimer.Update()){
					Update();
					Render();
				}
			}
		}
	}

	// pause engine loop. message loop is not paused
	bool Engine::Pause(){
		return mTimer.Pause();
	}

	// resume engine loop
	bool Engine::Resume(){
		return mTimer.Resume();
	}

	// call a halt to running engine
	void Engine::Halt(){
		mTimer.Pause();
		mbRunning = false;
	}

	// release application
	void Engine::ReleaseApp(){
		DisableGLContext();
	}

	// enable opengl device and rendering context
	bool Engine::EnableGLContext(){
		PIXELFORMATDESCRIPTOR pfd;
		int nPixelFormat;

		mhDC = GetDC(mhWnd);
		memset(&pfd, 0, sizeof(pfd));

		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 16;
		pfd.iLayerType = PFD_MAIN_PLANE;
		nPixelFormat = ChoosePixelFormat(mhDC, &pfd);
		SetPixelFormat(mhDC, nPixelFormat, &pfd);

		mhRC = wglCreateContext(mhDC);
		return (bool)wglMakeCurrent(mhDC, mhRC);
	}

	// disable opengl device and rendering context
	void Engine::DisableGLContext(){
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mhRC);
		ReleaseDC(mhWnd, mhDC);
	}

	// setup opengl state
	void Engine::SetupGL(){
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glPolygonMode(GL_FRONT_FACE, GL_FILL);

		glMatrixMode(GL_MODELVIEW);
	}



	// update current scene and other frame depentent components
	void Engine::Update(){
		mCurrentScene->Update();
	}

	// render current scene
	void Engine::Render(){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		mCurrentScene->Render();

		SwapBuffers(mhDC);
	}

	

	// ---------- configuration methods ------------

	bool Engine::AddScene(const std::string& sceneName, AScene* scene){
		SceneContainer::const_iterator find = mScenes.find(sceneName);
		if (find != mScenes.cend())
			return false;
		mScenes[sceneName] = scene;
		return true;
	}
	bool Engine::SetCurrentScene(const std::string& sceneName){
		SceneContainer::const_iterator find = mScenes.find(sceneName);
		if (find == mScenes.cend())
			return false;
		mCurrentScene = find->second;
		return true;
	}

	void Engine::SetClearColor(const Color& color){
		mClearColor = color;
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	}

	void Engine::SetViewMode(bool as2D){
		mbIs2D = as2D;
	}

	void Engine::ResizeWindow(uinteger width, uinteger height){
		mnWndWidth = width;
		mnWndHeight = height;

		SetWindowPos(mhWnd, NULL, 0, 0, mnWndWidth, mnWndHeight, SWP_NOMOVE | SWP_NOZORDER);

		SetViewport(mnWndWidth, mnWndHeight);
		SetProjectionMatrix(mnWndWidth, mnWndHeight);
	}

	void Engine::SetViewport(uinteger width, uinteger height){
		glViewport(0, 0, width, height);
	}

	void Engine::SetProjectionMatrix(uinteger width, uinteger height){
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (mbIs2D){
			// orthogonal view
			if (height >= width){
				real ratio = (real)width / (real)height;
				glOrtho(-500.0f*ratio, 500.0f*ratio, -500.0f, 500.0f, 0.0f, 1000.0f);
			}
			else{
				real ratio = (real)height / (real)width;
				glOrtho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, 1000.0f);
			}
		}
		else{
			gluPerspective(60, (real)width / (real)height, 0.1f, 1000.0f);
		}

		glMatrixMode(GL_MODELVIEW);
	}

	LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
		switch (iMessage){
		case WM_DESTROY:
			Engine::instance->mbRunning = false;
			return 0;

		case WM_CLOSE:
			// TODO: if there are file processing or network connection,
			// app can't be ternimated. just return 0;
			// or else, just break to make handle message into DefWindowProc
			break;

			// input handling
		case WM_KEYDOWN:
			Input::keyboard.Down(wParam);
			break;
		case WM_KEYUP:
			Input::keyboard.Up(wParam);
			break;

		case WM_SIZE:
			Engine::instance->mnWndWidth = LOWORD(lParam);
			Engine::instance->mnWndHeight = HIWORD(lParam);
			Engine::instance->SetViewport(Engine::instance->mnWndWidth, Engine::instance->mnWndHeight);
			Engine::instance->SetProjectionMatrix(Engine::instance->mnWndWidth, Engine::instance->mnWndHeight);
			break;
		};
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

}