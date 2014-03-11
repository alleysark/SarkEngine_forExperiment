#include "Engine.h"

namespace sarklib{

	Engine::Engine(){
		mhInst = NULL;
		mhWnd = NULL;
		mhDC = NULL;
		mhRC = NULL;

		mbRunning = false;
		mnWndWidth = mnWndHeight = 100;
		mClearColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		mbIs2D = false;
	}

	Engine* Engine::instance = NULL;
	Engine* Engine::GetInstance(){
		if (instance == NULL)
			instance = new Engine();
		return instance;
	}

	bool Engine::InitializeApp(HINSTANCE hInstance, int nCmdShow, std::wstring strClassName, std::wstring strAppName){
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

		mnWndWidth = 640;
		mnWndHeight = 480;
		mhWnd = CreateWindow(strClassName.c_str(), strAppName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, mnWndWidth, mnWndHeight,
			NULL, (HMENU)NULL, hInstance, NULL);
		if (mhWnd == NULL)
			return false;

		ShowWindow(mhWnd, nCmdShow);

		if (!EnableGLContext())
			return false;

		SetupGL();
		return true;
	}

	void Engine::Run(){
		mbRunning = true;
		MSG msg;

		unsigned long passTime = timeGetTime();
		unsigned long curTime;

		while (mbRunning){
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				// handle or dispatch messages
				if (msg.message == WM_QUIT){
					mbRunning = false;
				}
				else{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else{
				curTime = timeGetTime();
				Update((real)(curTime - passTime) / 1000.0f);
				passTime = curTime;

				Render();
			}
		}
	}

	void Engine::Update(real fDeltTime){

	}

	void Engine::Render(){
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static real theta = 0;

		glPushMatrix();
		glRotatef(theta, 0.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.0f, 0.0f, 0.0f); glVertex2f(0.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f); glVertex2f(0.87f, -0.5f);
		glColor3f(0.0f, 0.0f, 1.0f); glVertex2f(-0.87f, -0.5f);
		glEnd();
		glPopMatrix();

		SwapBuffers(mhDC);

		theta += 1.0f;
	}

	void Engine::ReleaseApp(){
		DisableGLContext();
	}

	void Engine::Resize(unsigned int width, unsigned int height){
		mnWndWidth = width;
		mnWndHeight = height;

		glViewport(0, 0, mnWndWidth, mnWndHeight);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		if (mbIs2D){
			// orthogonal view
			if (mnWndHeight >= mnWndWidth){
				real ratio = (real)mnWndWidth / (real)mnWndHeight;
				glOrtho(-500.0f*ratio, 500.0f*ratio, -500.0f, 500.0f, 0.0f, 1000.0f);
			}
			else{
				real ratio = (real)mnWndHeight / (real)mnWndWidth;
				glOrtho(-500.0f, 500.0f, -500.0f, 500.0f, 0.0f, 1000.0f);
			}
		}
		else{
			gluPerspective(60, (real)mnWndWidth / (real)mnWndHeight, 0.1f, 1000.0f);
		}

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void Engine::SetClearColor(const Color& color){
		mClearColor = color;
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
	}

	void Engine::SetViewMode(bool as2D){
		mbIs2D = as2D;
	}


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

	void Engine::DisableGLContext(){
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(mhRC);
		ReleaseDC(mhWnd, mhDC);
	}

	void Engine::SetupGL(){
		glClearColor(mClearColor.r, mClearColor.g, mClearColor.b, mClearColor.a);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);

		glPolygonMode(GL_FRONT_FACE, GL_FILL);
	}


	LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam){
		switch (iMessage){
		case WM_DESTROY:
			Engine::instance->mbRunning = false;
			return 0;

		case WM_SIZE:
			Engine::instance->Resize(LOWORD(lParam), HIWORD(lParam));
			return 0;
		};
		return DefWindowProc(hWnd, iMessage, wParam, lParam);
	}

}