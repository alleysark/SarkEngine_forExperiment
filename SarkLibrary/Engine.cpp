#include "Engine.h"
#include "Input.h"

namespace sark{

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
		mTimer(false), mbRunning(false), mnWndWidth(0), mnWndHeight(0), mClearColor(0),
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
					// update current scene
					mCurrentScene->Update();

					// render current scene
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glLoadIdentity();

					mCurrentScene->Render();

					SwapBuffers(mhDC);
				}
			}
		}
	}

	// get shader dictionary
	ShaderDictionary& Engine::GetShaderDict(){
		return mShDict;
	}

	// get resource manager
	ResourceManager& Engine::GetResourceManager(){
		return mResourceMgr;
	}

	// get engine timer
	const Timer& Engine::GetTimer() const{
		return mTimer;
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

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	// ---------- configuration methods ------------

	bool Engine::AddScene(const std::string& sceneName, AScene* scene, bool asCurrent){
		SceneContainer::const_iterator find = mScenes.find(sceneName);
		if (find != mScenes.cend())
			return false;
		mScenes[sceneName] = scene;
		if (asCurrent){
			if (mCurrentScene != NULL){
				mCurrentScene->OnLeave();
			}
			mCurrentScene = scene;
			mCurrentScene->OnEnter();
		}
		return true;
	}
	bool Engine::SetCurrentScene(const std::string& sceneName){
		SceneContainer::const_iterator find = mScenes.find(sceneName);
		if (find == mScenes.cend())
			return false;
		if (mCurrentScene != NULL){
			mCurrentScene->OnLeave();
		}
		mCurrentScene = find->second;
		mCurrentScene->OnEnter();
		return true;
	}

	void Engine::SetClearColor(const ColorRGBA& color){
		mClearColor = color;
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	}

	void Engine::ResizeWindow(uinteger width, uinteger height, bool callOnInside){
		mnWndWidth = width;
		mnWndHeight = height;
		mCurrentScene->OnScreenChanged(width, height);
		if (!callOnInside)
			SetWindowPos(mhWnd, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
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

			// mouse input handling
		case WM_MOUSEMOVE:
			Input::mouse.MovementUpdate(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
			Input::mouse.LButtonUpdate(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
			Input::mouse.MButtonUpdate(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			Input::mouse.RButtonUpdate(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_MOUSEWHEEL:
			Input::mouse.WheelUpdate(LOWORD(lParam), HIWORD(lParam),
				(real)GET_WHEEL_DELTA_WPARAM(wParam) / (real)WHEEL_DELTA);
			break;

		case WM_SIZE:
			Engine::instance->ResizeWindow(LOWORD(lParam), HIWORD(lParam), true);
			break;
		};
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

}