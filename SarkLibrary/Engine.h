#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <Windows.h>
#include <string>

#include <GL/gl.h>
#include <GL/glu.h>

#include "core.h"

namespace sarklib{

	class Engine{
	private:
		static Engine* instance;

		HINSTANCE mhInst;
		HWND mhWnd;
		HDC mhDC;
		HGLRC mhRC;

		bool mbRunning;

		unsigned int mnWndWidth, mnWndHeight;
		Color mClearColor;
		bool mbIs2D;

		Engine();

	public:
		static Engine* GetInstance();

		bool InitializeApp(HINSTANCE hInstance, int nCmdShow, std::wstring strClassName, std::wstring strAppName);

		void Run();
		void Pause();

		void Update(float fDeltTime);

		void Render();

		void ReleaseApp();

		void Resize(unsigned int width, unsigned int height);
		
		// configuration methods
		void SetClearColor(const Color& color);
		void SetViewMode(bool as2D);

	private:
		bool EnableGLContext();

		void DisableGLContext();

		void SetupGL();

	public:
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	};

}
#endif