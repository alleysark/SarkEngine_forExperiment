#include "Engine.h"
#include "RigidCube.h"
using namespace sark;

#define SARKLIB_DBGMODE

Engine* gpEngine = Engine::GetInstance();

//test_vs.glsl
/*
	#version 400

	in vec3 position;
	in vec3 normal;

	uniform mat4 matWorld;
	uniform mat4 matView;
	uniform mat4 matProjection;

	out vec4 color;

	void main(){

	vec3 lightDir = -normalize(vec3(0.5, -0.3, -0.3));
	vec3 norm = normalize(normal);

	mat4 matWorldView = matView * matWorld;

	norm = (inverse(transpose(matWorldView))*vec4(norm, 1)).xyz;

	float angle = dot(lightDir, norm);
	color = vec4(angle, angle, angle,1);
	gl_Position = matProjection * matView * matWorld * vec4(position, 1);

	}
*/

//test_fs.glsl
/*
	#version 400

	in vec4 color;

	out vec4 fragColor;


	void main(){
	fragColor = color;
	}
*/


class TestScene : public AScene{
public:
	ShaderProgram* renderer;
	RigidCube* mObj;

	TestScene(){
		gpEngine->GetShaderDict().RegisterProgram("basic", { "test_vs.glsl" }, { "test_fs.glsl" });
		renderer = gpEngine->GetShaderDict().GetShaderProgram("basic");

		mLayers.push_back(new Layer());

		mCameras.push_back(new Camera(Position3(0, 20, 50), Position3(0, 0, 0)));
		mMainCam = mCameras[0];

		mObj = new RigidCube(10, 10, 10);
		mLayers[0]->AddSceneComponent(mObj);

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// scene key handlers
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveSideward(-10);
		}, Input::Keyboard::CODE_LEFT);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveSideward(10);
		}, Input::Keyboard::CODE_RIGHT);

		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveUpward(10);
		}, Input::Keyboard::CODE_UP);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveUpward(-10);
		}, Input::Keyboard::CODE_DOWN);

		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveForward(10);
		}, Input::Keyboard::CODE_HOME);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveForward(-10);
		}, Input::Keyboard::CODE_END);

		// rotate key
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Pitch(math::deg2rad(1));
		}, Input::Keyboard::CODE_W);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Pitch(math::deg2rad(-1));
		}, Input::Keyboard::CODE_S);

		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Yaw(math::deg2rad(1));
		}, Input::Keyboard::CODE_A);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Yaw(math::deg2rad(-1));
		}, Input::Keyboard::CODE_D);

		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Roll(math::deg2rad(1));
		}, Input::Keyboard::CODE_Q);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Roll(math::deg2rad(-1));
		}, Input::Keyboard::CODE_E);
	}
	~TestScene(){
	}

	void Update(){
		mObj->GetTransform().Rotate(Vector3(0, 1, 0), gpEngine->GetTimer().GetElapsedTime());
	}

	void Render(){
		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetUniform("matWorld", mObj->GetTransform().GetMatrix());
		mObj->Render();
		renderer->Unuse();
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height){
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.01f, 1000.f);
	}
};
//---------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
	

	// ===== initialize engine and user data =====
	gpEngine->InitializeApp(hInstance, nCmdShow, L"Test", L"Test");


	// =============== settings ==================
	// engine settings (after initialization)
	
	// user input handler setting
	Input::keyboard.RegisterKeyCombHandler([]()->void{
		gpEngine->Halt();
	}, Input::Keyboard::CODE_ESC);

	Input::keyboard.RegisterKeyCombHandler([]()->void{
		if (!gpEngine->Pause()){
			gpEngine->Resume();
		}
	}, Input::Keyboard::CODE_SPACE);
	
	int a = 0;
	Input::mouse.RegisterMouseHandler(Input::Mouse::EVENT_LBUTTON_CLICK, [&](const Position2& p, real ext)->void{
		
	});
	Input::mouse.RegisterMouseHandler(Input::Mouse::EVENT_LBUTTON_DRAG, [&](const Position2& p, real ext)->void{
	});

	// add custom scene
	gpEngine->AddScene("scMain", new TestScene(), true);

	gpEngine->SetClearColor(ColorRGBA(0.12f, 0.12f, 0.12f, 1.0f));
	gpEngine->ResizeWindow(600, 400);

	// ============= run engine loop =============
	gpEngine->Run();

	// ======= release engine and user data ======
	gpEngine->ReleaseApp();

	return 0;
}