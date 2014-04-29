#define SARKLIB_DBGMODE

#include "Engine.h"
#include "ShaderChef.h"
#include "ShaderProgram.h"
#include "RigidCube.h"
#include "RigidSphere.h"
#include "BMPResource.h"
#include "Debug.h"
using namespace sark;

#include "Sampler.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "FrameBuffer.h"

Engine* gpEngine = Engine::GetInstance();


class TestScene : public AScene{
public:
	ShaderProgram* renderer;
	ASceneComponent::ComponentID id;

	RigidCube* mObj;
	RigidSphere* mSphere;

	BMPResource* bmp;
	Sampler* samp;
	Texture* tex;

	// dynamic light list
	DirectionalLight* mLight;

	FrameBuffer fb;

	TestScene(){
		//ShaderChef::GetInstance()->RegisterShaderSourceFromFile("test_vs", "D:/DOWN/test_vs.glsl");
		//ShaderChef::GetInstance()->RegisterShaderSourceFromFile("test_fs", "D:/DOWN/test_fs.glsl");

		ShaderChef::GetInstance()->RegisterShaderSource("test_vs",
			"in vec3 position;\n\
			in vec3 normal;\n\
			in vec2 texCoord0;\n\
			\n\
			uniform mat4 matWorld;\n\
			uniform mat4 matView;\n\
			uniform mat4 matProjection;\n\
			\n\
			uniform vec3 eye;\n\
			\n\
			out vec3 norm;\n\
			out vec3 viewDir;\n\
			out vec2 texcoord;\n\
			\n\
			void main(){\n\
				mat4 matWorldView;\n\
				matWorldView = matView * matWorld;\n\
				\n\
				viewDir = vec3(0, 0, 0) - (matWorldView*vec4(position, 1)).xyz;\n\
				norm = (inverse(transpose(matWorldView))*vec4(normalize(normal), 0)).xyz;\n\
				\n\
				gl_Position = matProjection * matWorldView * vec4(position, 1);\n\
				texcoord = texCoord0;\n\
			}");
		ShaderChef::GetInstance()->RegisterShaderSource("test_fs",
			"struct DirLight{\n\
				vec4 amb;\n\
				vec4 dif;\n\
				vec4 spec;\n\
				vec3 dir;\n\
			};\n\
			\n\
			uniform mat4 matWorld;\n\
			uniform mat4 matView;\n\
			uniform mat4 matProjection;\n\
			\n\
			uniform sampler2D texSamp;\n\
			uniform DirLight light;\n\
			\n\
			in vec3 norm;\n\
			in vec3 viewDir;\n\
			in vec2 texcoord;\n\
			\n\
			out vec4 fragColor;\n\
			\n\
			void main(){\n\
				// static color\n\
				vec4 mtrlAmb = vec4(0.3, 0.3, 0.3, 1);\n\
				vec4 mtrlDif = vec4(0.7, 0.7, 0.7, 1);\n\
				vec4 mtrlSpec = vec4(0.9, 0.9, 0.9, 1);\n\
				float mtrlShi = 4;\n\
				vec4 globalAmb = vec4(0.1, 0.1, 0.1, 1);\n\
				\n\
				vec3 l_dir = normalize(-light.dir);\n\
				vec3 v_dir = normalize(viewDir);\n\
				vec3 n_dir = normalize(norm);\n\
				\n\
				float cosNL;\n\
				\n\
				vec3 specR;\n\
				vec4 spec = vec4(0, 0, 0, 0);\n\
				\n\
				vec4 l_color;\n\
				vec4 t_color;\n\
				\n\
				cosNL = max(dot(n_dir, l_dir), 0.01);\n\
				if (cosNL > 0){\n\
					specR = 2 * cosNL * n_dir - l_dir;\n\
					spec = pow(max(dot(specR, v_dir), 0), mtrlShi) * mtrlSpec * light.spec;\n\
				}\n\
				\n\
				l_color = (globalAmb + light.amb*mtrlAmb) + (mtrlDif*light.dif*cosNL) + spec;\n\
				t_color = texture2D(texSamp, texcoord.st);\n\
				\n\
				fragColor = l_color * t_color;\n\
			}");
		renderer = ShaderChef::GetInstance()->MakeProgram(ShaderChef::VERSION_330, { "test_vs" }, { "test_fs" });
		ShaderChef::GetInstance()->Clear();

		mLayers.push_back(Layer());
		mCameras.push_back(new Camera(Position3(0, 20, 20), Position3(0, 0, 0)));
		mMainCam = mCameras[0];

		mObj = new RigidCube("obj1", NULL, false, 10, 10, 10);
		AddSceneComponent(mObj);
		mLayers[0].Push(mObj);

		mSphere = new RigidSphere("obj2", mObj, false, 10, 20, 20);
		AddSceneComponent(mSphere);
		mLayers[0].Push(mSphere);

		mLight = new DirectionalLight(
			ColorRGBA(0.3, 0.3, 0.3, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1));
		AddSceneComponent(mLight);
		

		// load texture resource
		gpEngine->GetResourceManager().SetBasePath("D:\\DOWN\\");
		bmp = gpEngine->GetResourceManager().Load<BMPResource>("test.bmp");
		if (bmp == NULL)
			return;

		// create texture
		tex = new Texture(Texture::TEX_2D, bmp, Texture::InternalFormat::THREE);

		// create sampler
		samp = new Sampler();
		samp->SetState(Sampler::WRAP_S, Sampler::WrapMode::REPEAT);
		samp->SetState(Sampler::WRAP_T, Sampler::WrapMode::REPEAT);
		samp->SetState(Sampler::MIN_FILTER, Sampler::Filter::LINEAR);
		samp->SetState(Sampler::MAG_FILTER, Sampler::Filter::LINEAR);

		//----------- fb test
		fb.Bind();
		fb.AttachTexture(new Texture(Texture::TEX_2D, 0,
			Texture::InternalFormat::RGB, 1024, 768, 0, false,
			Texture::Format::RGB, Texture::PixelType::UNSIGNED_BYTE, NULL),
			FrameBuffer::AttachmentPoint::COLOR_ATTACHMENT0);
		fb.Unbind();

		if (!fb.CheckStatus())
			int a = 0;
	}

	~TestScene(){
		ClearCameras();
		ClearSceneComponents();
		mLayers.clear();

		delete renderer;
		delete samp;
		delete tex;
	}

	void OnEnter() override{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// mouse handlers
		static Position2 prvPos;
		static ASceneComponent* graspComponent = NULL;

		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_LBUTTON_DRAG,
			[&](const Position2& pos, real ext)->void{
			Vector2 mv = pos - prvPos;
			if (graspComponent == NULL){
				mMainCam->Yaw(mv.x / 180.f);
				mMainCam->Pitch(mv.y / 180.f);
			}
			else{
				graspComponent->GetTransform().RotateMore(mMainCam->GetBasisV(), mv.x / 180.f);
				graspComponent->GetTransform().RotateMore(mMainCam->GetBasisU(), mv.y / 180.f);
			}
			prvPos = pos;
		});
		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_LBUTTON_DOWN,
			[&](const Position2& pos, real ext)->void{
			prvPos = pos;

			Ray ray = mMainCam->ScreenToWorldRay(pos);

			mLayers[0].Sort(mMainCam->GetEye());
			Layer::ReplicaArrayIterator itr = mLayers[0].Begin();
			Layer::ReplicaArrayIterator end = mLayers[0].End();
			for (; itr != end; itr++){
				if ((*itr)->GetBoundingShape() == NULL)
					continue;

				if ((*itr)->GetBoundingShape()->IsIntersectedWith(&ray)){
					graspComponent = (*itr);
					break;
				}
			}
		});
		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_LBUTTON_UP,
			[&](const Position2& pos, real ext)->void{
			if (graspComponent != NULL){
				graspComponent = NULL;
			}
		});
		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_WHEEL,
			[&](const Position2& pos, real ext)->void{
			mMainCam->MoveForward(ext);
		});


		// scene key handlers
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveSideward(-1);
		}, Input::Keyboard::CODE_A);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveSideward(1);
		}, Input::Keyboard::CODE_D);

		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveUpward(1);
		}, Input::Keyboard::CODE_W);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->MoveUpward(-1);
		}, Input::Keyboard::CODE_S);

		// rotate key
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Roll(math::deg2rad(1));
		}, Input::Keyboard::CODE_Q);
		Input::keyboard.RegisterKeyCombHandler([&]()->void{
			mMainCam->Roll(math::deg2rad(-1));
		}, Input::Keyboard::CODE_E);
	}

	void OnLeave() override{}

	void Update(){
		mSphere->GetTransform().Translate({ 20, 0, 0 });

		//mSphere->GetTransform().RotateMore(Vector3::Up, -0.01f);
		//mObj->GetTransform().RotateMore(Vector3::Up, 0.02f);

		mLight->GetTransform().Rotate(Vector3::Up, gpEngine->GetTimer().GetElapsedTime() / 2.f);

		mObj->Update();
		mSphere->Update();
	}

	void Render(){
		/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());
		glBegin(GL_LINES);
		const OrientedBox* ob = ((const OrientedBox*)mObj->GetBoundingShape());
		for (integer i = 0; i < 3; i++){
			Vector3 a = ob->pos + ob->axis[i].xyz*ob->axis[i].w;
			Vector3 b = ob->pos - ob->axis[i].xyz*ob->axis[i].w;
			glVertex3f(a.x, a.y, a.z);
			glVertex3f(b.x, b.y, b.z);
		}
		glEnd();
		*/

		fb.Bind();
		glPushAttrib(GL_VIEWPORT_BIT | GL_COLOR_BUFFER_BIT);
		glViewport(0, 0, 1024, 768);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetSampler("texSamp", samp, tex);

		renderer->SetUniform("light.amb", mLight->GetAmbient());
		renderer->SetUniform("light.dif", mLight->GetDiffuse());
		renderer->SetUniform("light.spec", mLight->GetSpecular());
		renderer->SetUniform("light.dir", mLight->GetTransform().GetDirection());

		ComponentMap::iterator itr = mComponents.begin();
		ComponentMap::iterator end = mComponents.end();
		for (; itr != end; itr++){
			renderer->SetUniform("matWorld",
				itr->second->GetTransform().GetMatrix());

			itr->second->Render();
		}
		renderer->Unuse();
		fb.Unbind();
		glPopAttrib();

		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetSampler("texSamp", samp, fb.GetTextureAt(0));

		renderer->SetUniform("light.amb", mLight->GetAmbient());
		renderer->SetUniform("light.dif", mLight->GetDiffuse());
		renderer->SetUniform("light.spec", mLight->GetSpecular());
		renderer->SetUniform("light.dir", mLight->GetTransform().GetDirection());

		itr = mComponents.begin();
		end = mComponents.end();
		for (; itr != end; itr++){
			renderer->SetUniform("matWorld",
				itr->second->GetTransform().GetMatrix());

			itr->second->Render();
		}
		renderer->Unuse();
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height) override{
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);
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
