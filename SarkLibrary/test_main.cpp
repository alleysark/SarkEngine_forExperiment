#include "Engine.h"
#include "ShaderChef.h"
#include "ShaderProgram.h"
#include "ASceneComponent.h"
#include "Mesh.h"
#include "RigidCube.h"
#include "RigidSphere.h"
#include "BMPResource.h"
#include "PNGResource.h"
#include "OBJResource.h"
#include "Debug.h"
#include "Sampler.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "FrameBuffer.h"
#include "Collision.h"
#include "GJK_EPA.h"
#include "ConvexHull.h"
#include "OBoxCollider.h"
#include "SphereCollider.h"
#include "RigidBody.h"
#include "tools.h"
using namespace sark;

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "libpng16.lib")

Engine* gpEngine = Engine::GetInstance();

class PhysicsSimulationScene : public AScene{
public:
	enum LayerType{
		LAYER_PHYSICS = 0,
		LAYER_PICKABLE = 1
	};

	std::shared_ptr<ShaderProgram> renderer;

	DirectionalLight* mLight;

	Sampler* samp;
	Texture* tex;

	ConvexHull* makeBoxConvexHull(RigidCube* cube){
		real w = cube->GetWidth() / 2.f;
		real h = cube->GetHeight() / 2.f;
		real d = cube->GetDepth() / 2.f;

		std::vector<Vector3> points = {
			Vector3(-w, -h, -d), Vector3(w, -h, -d), Vector3(w, -h, d), Vector3(-w, -h, d),
			Vector3(-w, h, -d), Vector3(w, h, -d), Vector3(w, h, d), Vector3(-w, h, d)
		};
		std::vector<TriangleFace16> faces = {
			TriangleFace16(0, 1, 2), TriangleFace16(0, 2, 3),
			TriangleFace16(0, 7, 4), TriangleFace16(0, 3, 7),
			TriangleFace16(0, 4, 5), TriangleFace16(0, 5, 1),
			TriangleFace16(2, 1, 5), TriangleFace16(2, 5, 6),
			TriangleFace16(3, 2, 6), TriangleFace16(3, 6, 7),
			TriangleFace16(4, 6, 5), TriangleFace16(4, 7, 6)
		};
		return new ConvexHull(cube, points, faces);
	}

	std::vector<Vector3> makeConvexPoints(ASceneComponent* comp){
		ArrayBuffer::AttributeAccessor<Position3> poss 
			= comp->GetMesh()->GetArrayBuffer().GetAttributeAccessor<Position3>(AttributeSemantic::POSITION);
		ArrayBuffer::AttributeAccessor<TriangleFace16> faces 
			= comp->GetMesh()->GetArrayBuffer().GetAttributeAccessor<TriangleFace16>(AttributeSemantic::INDICES);
		std::vector<Vector3> points;
		uinteger sz = faces.Count();
		for (uinteger i = 0; i < sz; i++){
			for (uinteger j = 0; j < 3; j++){
				bool unique = true;
				uinteger sz = points.size();
				for (uinteger k = 0; k < sz; k++){
					if (poss[faces[i].idx[j]] == points[k]){
						unique = false;
						break;
					}
				}
				if (unique){
					points.push_back(poss[faces[i].idx[j]]);
				}
			}
		}
		return points;
	}

	PhysicsSimulationScene(){
		mLayers.push_back(Layer()); //layer for physics simulation
		mLayers.push_back(Layer()); //layer for pickable components.

		mCameras.push_back(Camera(Position3(0, 30, 30), Position3(0, 15, 0)));
		mMainCam = &mCameras[0];

		// --------------------------- sphere ---------------------------------
		/*
		RigidSphere* sphere = new RigidSphere(2.5, 20, 20, 1, 0, 0, true);
		sphere->SetCollider(new ConvexHull(sphere, makeConvexPoints(sphere)));
		mLayers[LAYER_PHYSICS].Push(sphere);
		sphere->GetTransform().Translate(0, 50, 0);
		AddSceneComponent(sphere);
		*/

		// ----------------------------- box ----------------------------------
		RigidCube* box = new RigidCube(5, 10, 5, 1, 0, 0, true);
		box->SetCollider(std::unique_ptr<ACollider>(makeBoxConvexHull(box)));
		mLayers[LAYER_PHYSICS].Push(box);
		mLayers[LAYER_PICKABLE].Push(box);
		box->GetTransform().Translate(0, 30, 0);
		AddSceneComponent(box);

		// ----------------------------- room ---------------------------------
		RigidCube* wall = new RigidCube(50, 1, 50, 0, 0, 0, false);
		wall->SetCollider(std::unique_ptr<ACollider>(makeBoxConvexHull(wall)));
		mLayers[LAYER_PHYSICS].Push(wall);
		wall->GetTransform().Translate(0, 0, 0);
		AddSceneComponent(wall);

		wall = new RigidCube(50, 1, 50, 0, 0, 0, false);
		wall->SetCollider(std::unique_ptr<ACollider>(makeBoxConvexHull(wall)));
		mLayers[LAYER_PHYSICS].Push(wall);
		wall->GetTransform().Rotate(0, math::deg2rad(90), math::deg2rad(90));
		wall->GetTransform().Translate(-25, 25.5f, 0);
		AddSceneComponent(wall);

		wall = new RigidCube(50, 1, 50, 0, 0, 0, false);
		wall->SetCollider(std::unique_ptr<ACollider>(makeBoxConvexHull(wall)));
		mLayers[LAYER_PHYSICS].Push(wall);
		wall->GetTransform().Rotate(0, math::deg2rad(90), math::deg2rad(-90));
		wall->GetTransform().Translate(25, 25.5f, 0);
		AddSceneComponent(wall);

		wall = new RigidCube(50, 1, 50, 0, 0, 0, false);
		wall->SetCollider(std::unique_ptr<ACollider>(makeBoxConvexHull(wall)));
		mLayers[LAYER_PHYSICS].Push(wall);
		wall->GetTransform().Rotate(0, math::deg2rad(90), 0);
		wall->GetTransform().Translate(0, 25.5f, -25);
		AddSceneComponent(wall);

		// ----------------------- directional light ---------------------------
		mLight = new DirectionalLight(
			ColorRGBA(0.3, 0.3, 0.3, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1));
		mLight->GetTransform().Rotate(0, math::deg2rad(-45), 0);
		AddSceneComponent(mLight);

		// ---------------------------------------------------------------------
		// make shader program
		ShaderProgram::Recipe recipe;
		recipe.attributes = {
			{ AttributeSemantic::POSITION, "position" },
			{ AttributeSemantic::NORMAL, "normal" },
			{ AttributeSemantic::TEXCOORD0, "texcoord0" }
		};
		recipe.vertexShader = "version 440;\n void main(){}";
		recipe.fragmentShader = "version 440:\n void main(){}";

		renderer = ShaderChef::CookShaderProgram(recipe);

		// load texture resource
		gpEngine->GetResourceManager().SetBasePath("D:\\DOWN\\");
		PNGResource* png = gpEngine->GetResourceManager().Load<PNGResource>("test.png");
		if (png == NULL)
			return;
		// create texture
		tex = new Texture(Texture::TEX_2D, png, Texture::InternalFormat::FOUR);

		// create sampler
		samp = new Sampler();
		samp->SetState(Sampler::WRAP_S, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::WRAP_T, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::MIN_FILTER, Sampler::Filter::LINEAR);
		samp->SetState(Sampler::MAG_FILTER, Sampler::Filter::LINEAR);
	}

	~PhysicsSimulationScene(){
		delete samp;
		delete tex;
	}

	void OnEnter() override{
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

			mLayers[LAYER_PICKABLE].Sort(mMainCam->GetEye());
			Layer::ReplicaArrayIterator itr = mLayers[LAYER_PICKABLE].Begin();
			Layer::ReplicaArrayIterator end = mLayers[LAYER_PICKABLE].End();
			for (; itr != end; itr++){
				const ACollider* bs = (*itr)->GetCollider();

				if (bs == NULL)
					continue;
				if (ray.IntersectWith(bs))
					graspComponent = (*itr);
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
			Input::Mouse::EVENT_RBUTTON_CLICK,
			[&](const Position2& pos, real ext)->void{
			Ray ray = mMainCam->ScreenToWorldRay(pos);

			mLayers[LAYER_PICKABLE].Sort(mMainCam->GetEye());
			Layer::ReplicaArrayIterator itr = mLayers[LAYER_PICKABLE].Begin();
			Layer::ReplicaArrayIterator end = mLayers[LAYER_PICKABLE].End();
			for (; itr != end; itr++){
				const ACollider* bs = (*itr)->GetCollider();

				if (bs == NULL)
					continue;
				Vector3 hitP;
				if (ray.IntersectWith(bs, &hitP)){
					(*itr)->GetRigidBody()->AddForceOn(hitP, ray.dir*100.f);
				}
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
		AScene::Layer::ReplicaArrayIterator itr = mLayers[LAYER_PHYSICS].Begin();
		AScene::Layer::ReplicaArrayIterator end = mLayers[LAYER_PHYSICS].End();
		for (; itr != end; itr++){
			(*itr)->Update();
		}

		//Collision::ProcessCollision(mLayers[LAYER_PHYSICS]);
		Collision::ProcessConvexCollision(mLayers[LAYER_PHYSICS]);
	}

	void Render(){
		//----------------
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());
		//----------------

		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetSampler("texSamp", samp, tex);

		renderer->SetUniform("eyePos", mMainCam->GetEye());
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
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height) override{
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);

		//----------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
		//----------------
	}
};

class TestScene : public AScene{
public:
	std::shared_ptr<ShaderProgram> renderer;

	// dynamic light list
	DirectionalLight* mLight;

	RigidCube* mObj;
	RigidSphere* mSphere;

	Sampler* samp;
	Texture* tex;

	TestScene(){
		ShaderProgram::Recipe recipe;
		recipe.attributes = {
			{ AttributeSemantic::POSITION, "position" },
			{ AttributeSemantic::NORMAL, "normal" },
			{ AttributeSemantic::TEXCOORD0, "texcoord0" }
		};
		recipe.vertexShader =
			"#version 440						\n"
			"in vec3 position;					\n"
			"in vec3 normal;					\n"
			"in vec2 texcoord0;					\n"
			"uniform mat4 matWorld;				\n"
			"uniform mat4 matView;				\n"
			"uniform mat4 matProjection;		\n"
			"out vec4 wpos;						\n"
			"out vec3 norm;						\n"
			"out vec2 texcoord;					\n"
			"void main(){													\n"
			"	wpos = matWorld * vec4(position, 1);						\n"
			"	norm = (transpose(inverse(matWorld))*vec4(normal, 0)).xyz;	\n"
			"	texcoord = texcoord0;										\n"
			"	gl_Position = matProjection * matView * wpos;				\n"
			"}";
		recipe.fragmentShader =
			"#version 440										\n"
			"struct DirLight{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	vec3 dir;										\n"
			"};													\n"
			"struct Material{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	float shi;										\n"
			"};													\n"
			"													\n"
			"uniform mat4 matWorld;								\n"
			"uniform mat4 matView;								\n"
			"uniform mat4 matProjection;						\n"
			"uniform vec3 weye;									\n"
			"uniform DirLight light;							\n"
			"uniform sampler2D texSamp;							\n"
			"													\n"
			"in vec4 wpos;										\n"
			"in vec3 norm;										\n"
			"in vec2 texcoord;									\n"
			"													\n"
			"out vec4 fragColor;								\n"
			"													\n"
			"vec4 globalAmb = vec4(0.1, 0.1, 0.1, 1);			\n"
			"Material mtrl = Material(vec4(0.9, 0.9, 0.9, 1),	\n"
			"					vec4(1, 1, 1, 1),				\n"
			"					vec4(0.9, 0.9, 0.9, 1), 16.0);	\n"
			"													\n"
			"void main(){										\n"
			"	vec3 n = normalize(norm);						\n"
			"	vec3 v = normalize(weye - wpos.xyz);			\n"
			"	vec3 l = -light.dir;							\n"
			"	float sc=0;										\n"
			"	float cosNL = max(dot(n,l), 0.05);				\n"
			"	vec4 texDif;									\n"
			"													\n"
			"	if(cosNL > 0.05 ){								\n"
			"		vec3 r = 2*cosNL*n - l;						\n"
			"		sc = pow(max(dot(r,v),0), mtrl.shi);		\n"
			"	}												\n"
			"													\n"
			"	texDif = texture(texSamp, texcoord);			\n"
			"	fragColor = cosNL*light.dif*texDif				\n"
			"				+ sc*light.spec*mtrl.spec			\n"
			"				+ light.amb*mtrl.amb;				\n"
			"}";

		renderer = ShaderChef::CookShaderProgram(recipe);

		mLayers.push_back(Layer());
		mCameras.push_back(Camera(Position3(0, 40, 30), Position3(0, 0, 0)));
		mMainCam = &mCameras[0];

		mObj = new RigidCube(10, 10, 10, 0.1, 0, 0, false);
		mObj->SetCollider(std::unique_ptr<ACollider>(new OBoxCollider(mObj, Vector3(0), 5)));
		mLayers[0].Push(mObj);
		mObj->GetTransform().Translate(0, 0, 0);
		AddSceneComponent(mObj);

		mSphere = new RigidSphere(10, 40, 40, 0, 0, 0, false);
		mSphere->SetCollider(std::unique_ptr<ACollider>(new SphereCollider(mSphere, Vector3(0), 10)));
		mLayers[0].Push(mSphere);
		mSphere->GetTransform().Translate(20, 0, 0);
		AddSceneComponent(mSphere);

		// obj test
		OBJResource* obj = gpEngine->GetResourceManager().Load<OBJResource>("D:\\Down\\bunny_tex.obj");
		if (obj == NULL) {
			LogError("Failed to load object");
		}
		obj->MakeItCenter();
		StaticModel* model = static_cast<StaticModel*>(obj->CreateModel());
		const Vector3 center = tool::ComputeCenterOfMass(obj->GetVertices());
		model->SetCollider(std::unique_ptr<ACollider>(new OBoxCollider(model, center, Vector3(10,10,10))));
		model->GetTransform().Scale(100, 100, 100);
		mLayers[0].Push(model);
		AddSceneComponent(model);


		mLight = new DirectionalLight(
			ColorRGBA(0.3, 0.3, 0.3, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1));
		mLight->GetTransform().Rotate(0, math::deg2rad(-45), 0);
		AddSceneComponent(mLight);

		// load texture resource
		PNGResource* png = gpEngine->GetResourceManager().Load<PNGResource>("D:\\Down\\test.png");
		if (png == NULL) {
			LogError("Failed to load texture");
		}
		// create texture
		tex = new Texture(Texture::TEX_2D, png, Texture::InternalFormat::FOUR);

		// create sampler
		samp = new Sampler();
		samp->SetState(Sampler::WRAP_S, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::WRAP_T, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::MIN_FILTER, Sampler::Filter::LINEAR);
		samp->SetState(Sampler::MAG_FILTER, Sampler::Filter::LINEAR);
	}

	~TestScene(){
		delete samp;
		delete tex;
	}

	void OnEnter() override{
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
				const ACollider* bs = (*itr)->GetCollider();

				if (bs == NULL)
					continue;
				if (ray.IntersectWith(bs))
					graspComponent = (*itr);
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
		//mObj->GetTransform().Translate(gpEngine->GetTimer().GetElapsedTime(), 0, 0);
		//mSphere->GetTransform().Rotate(0, gpEngine->GetTimer().GetElapsedTime(), 0);
		//mLight->GetTransform().Rotate(Vector3::Up, gpEngine->GetTimer().GetElapsedTime() / 2.f);

		mObj->Update();
		mSphere->Update();
	}

	void Render(){
		//----------------
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());
		//----------------

		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetSampler("texSamp", samp, tex);

		renderer->SetUniform("eyePos", mMainCam->GetEye());
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
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height) override{
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);

		//----------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
		//----------------
	}
};

class ShadowmapTestScene : public AScene{
public:
	std::shared_ptr<ShaderProgram> renderer;

	RigidCube* mObj;
	RigidSphere* mSphere;

	Sampler* samp;
	Texture* tex;

	// dynamic light list
	DirectionalLight* mLight;

	// shadow map
	FrameBuffer* mShadowMap;
	Sampler* mShSamp;
	std::shared_ptr<ShaderProgram> mShadowRenderer;

	ShadowmapTestScene(){
		ShaderProgram::Recipe recipe;
		recipe.attributes = {
			{ AttributeSemantic::POSITION, "position" },
			{ AttributeSemantic::NORMAL, "normal" },
			{ AttributeSemantic::TEXCOORD0, "texcoord0" }
		};
		recipe.vertexShader =
			"#version 440						\n"
			"in vec3 position;					\n"
			"in vec3 normal;					\n"
			"in vec2 texcoord0;					\n"
			"uniform mat4 matWorld;				\n"
			"uniform mat4 matView;				\n"
			"uniform mat4 matProjection;		\n"
			"out vec4 wpos;						\n"
			"out vec3 norm;						\n"
			"out vec2 texcoord;					\n"
			"void main(){													\n"
			"	wpos = matWorld * vec4(position, 1);						\n"
			"	norm = (transpose(inverse(matWorld))*vec4(normal, 0)).xyz;	\n"
			"	texcoord = texcoord0;										\n"
			"	gl_Position = matProjection * matView * wpos;				\n"
			"}";
		recipe.fragmentShader =
			"#version 440										\n"
			"struct DirLight{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	vec3 dir;										\n"
			"};													\n"
			"struct Material{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	float shi;										\n"
			"};													\n"
			"													\n"
			"uniform mat4 matWorld;								\n"
			"uniform mat4 matView;								\n"
			"uniform mat4 matProjection;						\n"
			"uniform mat4 lightVP;								\n"
			"uniform vec3 weye;									\n"
			"uniform DirLight light;							\n"
			"uniform sampler2D texSamp;							\n"
			"uniform sampler2D shadowTexSamp;					\n"
			"													\n"
			"in vec4 wpos;										\n"
			"in vec3 norm;										\n"
			"in vec2 texcoord;									\n"
			"													\n"
			"out vec4 fragColor;								\n"
			"													\n"
			"vec4 globalAmb = vec4(0.1, 0.1, 0.1, 1);			\n"
			"Material mtrl = Material(vec4(0.3, 0.3, 0.3, 1),	\n"
			"					vec4(0.7,0.7,0.7,1),			\n"
			"					vec4(0.9, 0.9, 0.9, 1), 16.0);	\n"
			"													\n"
			"void main(){										\n"
			"	vec3 n = normalize(norm);						\n"
			"	vec3 v = normalize(weye - wpos.xyz);			\n"
			"	vec3 l = -light.dir;							\n"
			"	float sc=0;										\n"
			"	float cosNL = max(dot(n,l), 0.05);				\n"
			"	vec4 shCoord = lightVP * wpos;					\n"
			"	float shDepth;									\n"
			"	vec4 texDif;									\n"
			"	float shd=1.0;									\n"
			"													\n"
			"	shCoord.xyz /= shCoord.w;						\n"
			"	shCoord.x = (shCoord.x + 1.0) / 2.0;			\n"
			"	shCoord.y = (shCoord.y + 1.0) / 2.0;			\n"
			"	shCoord.z = (shCoord.z + 1.0) / 2.0;			\n"
			"	shDepth = texture(shadowTexSamp, shCoord.xy).x;	\n"
			"													\n"
			"	if(shDepth+0.005 < shCoord.z){					\n"
			"		shd = 0.3;									\n"
			"	}												\n"
			"													\n"
			"	if(cosNL > 0.05 ){								\n"
			"		vec3 r = 2*cosNL*n - l;						\n"
			"		sc = pow(max(dot(r,v),0), mtrl.shi);		\n"
			"	}												\n"
			"													\n"
			"	texDif = texture(texSamp, texcoord);			\n"
			"	fragColor = cosNL*light.dif*texDif * shd		\n"
			"				+ sc*light.spec*mtrl.spec * shd		\n"
			"				+ light.amb*mtrl.amb;				\n"
			"}";

		renderer = ShaderChef::CookShaderProgram(recipe);

		mLayers.push_back(Layer());
		mCameras.push_back(Camera(Position3(0, 20, 20), Position3(0, 0, 0)));

		RigidCube* ground = new RigidCube("ground", NULL, true, 1000, 1, 200);
		ground->GetTransform().Translate(0, -30, 0);
		AddSceneComponent(ground);

		ground = new RigidCube("ground2", NULL, true, 100, 1, 100);
		ground->GetTransform().Rotate(0, math::deg2rad(90), 0);
		ground->GetTransform().Translate(0, 0, -100);
		AddSceneComponent(ground);

		mObj = new RigidCube("obj1", NULL, false, 10, 10, 10);
		mObj->SetCollider(std::unique_ptr<ACollider>(new OBoxCollider(mObj, 0, { 5, 5, 5 })));
		AddSceneComponent(mObj);
		mLayers[0].Push(mObj);

		mSphere = new RigidSphere("obj2", mObj, false, 10, 40, 40);
		mSphere->SetCollider(std::unique_ptr<ACollider>(new SphereCollider(mSphere, 0, 10)));
		mSphere->GetTransform().Translate(20, 0, 0);
		AddSceneComponent(mSphere);
		mLayers[0].Push(mSphere);

		mLight = new DirectionalLight(
			ColorRGBA(0.3, 0.3, 0.3, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1));
		mLight->GetTransform().Rotate(0, math::deg2rad(225), 0);
		AddSceneComponent(mLight);

		// load texture resource
		PNGResource* png = gpEngine->GetResourceManager().Load<PNGResource>("D:\\Down\\test.png");
		if (png == NULL)
			return;

		// create texture
		tex = new Texture(Texture::TEX_2D, png, Texture::InternalFormat::THREE);

		// create sampler
		samp = new Sampler();
		samp->SetState(Sampler::WRAP_S, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::WRAP_T, Sampler::WrapMode::CLAMP_TO_EDGE);
		samp->SetState(Sampler::MIN_FILTER, Sampler::Filter::LINEAR);
		samp->SetState(Sampler::MAG_FILTER, Sampler::Filter::LINEAR);


		//-------------------------------------
		recipe.attributes = { { AttributeSemantic::POSITION, "position" } };
		recipe.vertexShader =
			"#version 440												\n"
			"in vec3 position;											\n"
			"uniform mat4 matWorld;										\n"
			"uniform mat4 matLightVP;									\n"
			"void main(){												\n"
			"	gl_Position = matLightVP * matWorld * vec4(position, 1);\n"
			"}";
		recipe.fragmentShader =
			"#version 440					\n"
			"out float fragDepth;			\n"
			"void main(){					\n"
			"	fragDepth = gl_FragCoord.z;	\n"
			"}";

		mShadowRenderer = ShaderChef::CookShaderProgram(recipe);

		// shadow map frame buffer
		mShadowMap = new FrameBuffer();
		mShadowMap->Bind();
		mShadowMap->AttachTexture(
			new Texture(Texture::TEX_2D, 0,
			Texture::InternalFormat::DEPTH_COMPONENT, 1024, 1024, 0, false,
			Texture::Format::DEPTH_COMPONENT, Texture::PixelType::UNSIGNED_BYTE, NULL),
			FrameBuffer::AttachmentPoint::DEPTH_ATTACHMENT);
		glDrawBuffer(GL_NONE);//we won't bind a color texture with the currently bound FBO
		glReadBuffer(GL_NONE);

		if (!mShadowMap->CheckStatus()){
			LogError("shadow map state error");
			gpEngine->Pause();
		}
		mShadowMap->Unbind();

		mShSamp = new Sampler();
		mShSamp->SetState(Sampler::WRAP_S, Sampler::WrapMode::CLAMP);
		mShSamp->SetState(Sampler::WRAP_T, Sampler::WrapMode::CLAMP);
		mShSamp->SetState(Sampler::MIN_FILTER, Sampler::Filter::NEAREST);
		mShSamp->SetState(Sampler::MAG_FILTER, Sampler::Filter::NEAREST);
		
		mCameras.push_back(
			Camera(Position3(0,0,0), mLight->GetTransform().GetDirection())
			);
		mCameras[1].Orthographic(-200, 200, -200, 200, -500, 500);
		mCameras[1].SetViewport(0, 0, 1024, 1024);

		mMainCam = &mCameras[0];
	}

	~ShadowmapTestScene(){
		delete samp;
		delete tex;

		delete mShadowMap;
		delete mShSamp;
	}

	void OnEnter() override{
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
				const ACollider* bs = (*itr)->GetCollider();

				if (bs == NULL)
					continue;
				if (ray.IntersectWith(bs)){
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
		mObj->Update();
		mSphere->Update();
	}

	void Render(){
		Matrix4 lightVP = mCameras[1].GetProjMatrix() * mCameras[1].GetViewMatrix();
		ComponentMap::iterator itr;
		ComponentMap::iterator end;

		// ------1st--------
		mShadowMap->Bind();
		mCameras[1].SetViewport();
		glColorMask(0, 0, 0, 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);

		mShadowRenderer->Use();
		mShadowRenderer->SetUniform("matLightVP", lightVP);

		itr = mComponents.begin();
		end = mComponents.end();
		for (; itr != end; itr++){
			mShadowRenderer->SetUniform("matWorld",
				itr->second->GetTransform().GetMatrix());
			itr->second->Render();
		}
		
		glEnable(GL_CULL_FACE);
		mShadowRenderer->Unuse();
		mShadowMap->Unbind();

		// -----2nd---------
		glColorMask(1, 1, 1, 1);
		mMainCam->SetViewport();
		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		renderer->SetUniform("lightVP", lightVP);

		renderer->SetUniform("weye", mMainCam->GetEye());
		renderer->SetUniform("light.amb", mLight->GetAmbient());
		renderer->SetUniform("light.dif", mLight->GetDiffuse());
		renderer->SetUniform("light.spec", mLight->GetSpecular());
		renderer->SetUniform("light.dir", mLight->GetTransform().GetDirection());

		renderer->SetSampler("texSamp", samp, tex);
		renderer->SetSampler("shadowTexSamp", mShSamp, mShadowMap->GetTextureAt(0), 1);

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
		//mMainCam = &mCameras[0];
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);
	}
};







#include "AModel.h"

// animated model.
// hierarchical joint and mesh attribute of
// relative joint and blending weight.
class AnimatedModel : public AModel{
public:
	class Joint{
	private:
		friend AnimatedModel;

		uinteger id;

		integer pid;

		// Mp_i; to-parent space change matrix.
		Matrix4 Mp;

		// inv(Md_i); inverse of to-default pose matrix.
		// inv(Md_i) = inv(Mp_i) * inv(Md_{i-1})
		Matrix4 invMd;

	public:
		Joint(uinteger ID, const Matrix4& M_p, 
			const Joint* parentJoint=NULL)
			: id(ID), Mp(M_p)
		{
			if (parentJoint == NULL){
				pid = -1;
				invMd = Mp.Inverse();
			}
			else{
				pid = parentJoint->id;
				invMd = Mp.Inverse() * parentJoint->invMd;
			}
		}
	};

	class Animator{
	public:
		struct Animation{
			struct TransformData{
				real time;
				Vector3 translation;
				Quaternion rotation;
				// interpolation method
			};

			std::vector<TransformData> datas;

			const Matrix4 Interpolate(real time){
				uinteger sz = datas.size();
				uinteger i = 1;
				for (; i < sz; i++){
					if (time <= datas[i].time)
						break;
				}

				if (time == datas[i].time){
					Matrix4 mat = datas[i].rotation.ToMatrix4(true);
					mat.m[0][3] = datas[i].translation.x;
					mat.m[1][3] = datas[i].translation.y;
					mat.m[2][3] = datas[i].translation.z;
					return mat;
				}

				const TransformData& A = datas[i - 1];
				const TransformData& B = datas[i];

				real t = (time - A.time) / (B.time - A.time);

				Vector3 erpT = (1.f - t) * A.translation - t * B.translation;
				Quaternion erpQ = Quaternion::Slerp(A.rotation, B.rotation, t);

				Matrix4 mat = erpQ.ToMatrix4(true);
				mat.m[0][3] = erpT.x;
				mat.m[1][3] = erpT.y;
				mat.m[2][3] = erpT.z;
				return mat;
			}
		};

		struct _PlayMode_wrapper{
			enum PlayMode{
				ONCE = 0, REPEAT
			};
		};
		typedef _PlayMode_wrapper::PlayMode PlayMode;

		struct _PlayState_wrapper{
			enum PlayState{
				STOP = 0, PAUSE, RUN
			};
		};
		typedef _PlayState_wrapper::PlayState PlayState;

	private:
		friend AnimatedModel;

		// animation table.
		// count is same as joint's.
		// each animations must have to have at least
		// two transformation datas of beginnings and endings.
		std::vector<Animation> anims;

		// total constant play time
		real playTime;

		// currently passing time
		real passingTime;

		// play mode. (not supported yet)
		PlayMode mode;
		PlayState state;

	public:
		Animator()
			: playTime(0.f), passingTime(0.f),
			mode(PlayMode::ONCE), state(PlayState::STOP)
		{}

		Animator(real play_time, 
			PlayMode play_mode = PlayMode::ONCE, 
			PlayState play_state = PlayState::STOP)
			: playTime(play_time), passingTime(0.f),
			mode(play_mode), state(play_state)
		{}
		~Animator(){}

		void SetAnimations(const std::vector<Animation>& animations){
			anims = animations;
		}

		void Run(){
			state = PlayState::RUN;
		}

		void Pause(){
			state = PlayState::PAUSE;
		}

		void Stop(){
			state = PlayState::STOP;
			passingTime = 0.f;
		}

		void SetPlayMode(PlayMode play_mode){
			mode = play_mode;
		}

	private:
		void Update(real deltaTime){
			if (state != PlayState::RUN)
				return;

			passingTime += deltaTime;
			if (passingTime > playTime){
				if (mode == PlayMode::ONCE){
					state = PlayState::STOP;
					passingTime = 0.f;
				}
				else{
					passingTime = playTime - passingTime;
				}
			}
		}
	};

	typedef std::vector<Matrix4> MatrixPalette;

private:
	std::vector<Joint> mJoints; //joint table
	Animator mAnimator;
	MatrixPalette mPalette;

public:
	AnimatedModel(const std::string& name, ASceneComponent* parent, bool activate)
		: AModel(name, parent, activate){}
	
	virtual ~AnimatedModel(){}

	void SetJointTable(const std::vector<Joint>& joints){
		mJoints = joints;
		mPalette.resize(joints.size());
	}

	void SetAnimator(const Animator& animator){
		mAnimator = animator;
	}

	Animator& GetAnimator(){
		return mAnimator;
	}

	const MatrixPalette& GetMatrixPalette() const{
		return mPalette;
	}

	void Update() override{
		if (mCollider != NULL)
			mCollider->Update();
		if (mRigidBody != NULL)
			mRigidBody->Update();

		// update animator time
		mAnimator.Update(Engine::GetInstance()->GetTimer().GetDeltaTime());

		// update matrix palette
		// TODO: index 순서대로 부모뼈가 아닐수도 있음. 트리순환 구조로 변경

		Matrix4* matWs = new Matrix4[mPalette.size()];
		uinteger count = mJoints.size();
		for (uinteger i = 0; i < count; i++){
			Matrix4 Mc = mAnimator.anims[i].Interpolate(mAnimator.passingTime);
			if (mJoints[i].pid == -1){
				// Mi_0 = Mc_0
				matWs[i] = Mc;
				mPalette[i] = Mc;
			}
			else{
				// Mw_i = Mw_{i-1} * Mp_i * Mc_i
				matWs[i] = matWs[mJoints[i].pid] * mJoints[i].Mp * Mc;
				// Mi = Mw_i * invMd_i
				mPalette[i] = matWs[i] * mJoints[i].invMd;
			}
		}
		delete[] matWs;
	}

	// uniform matrix palette should to be passed before.
	void Render() override{
		mMesh->Draw();
	}
};

class TmpScene : public AScene{
public:
	std::shared_ptr<ShaderProgram> renderer;

	AnimatedModel* animModel;

	TmpScene(){
		ShaderProgram::Recipe recipe;
		recipe.attributes = {
			{ AttributeSemantic::POSITION, "position" },
			{ AttributeSemantic::TEXCOORD0, "texcoord0" },
			{ AttributeSemantic::NORMAL, "normal" },
			{ AttributeSemantic::TEXCOORD1, "matIndex" },
			{ AttributeSemantic::TEXCOORD2, "weights" }
		};
		recipe.vertexShader =
			"#version 440				\n"
			"in vec3 position;			\n"
			"in vec2 texcoord0;			\n"
			"in vec3 normal;			\n"
			"in vec4 matIndex;			\n" //palette index
			"in vec4 weights;			\n" //blending weight
			"uniform mat4 matWorld;		\n"
			"uniform mat4 matView;		\n"
			"uniform mat4 matProjection;\n"
			"uniform mat4 palette[20];	\n"
			"out vec3 norm;				\n"
			"void main(){				\n"
			"	vec3 blendedPos = vec3(0,0,0);\n"
			"	vec3 p;					\n"
			"	vec3 n;					\n"
			"	norm = vec3(0,0,0);		\n"
			"	for(int i=0; i<4; i++){											\n"
			"		if(matIndex[i] == -1)										\n"
			"			break;													\n"
			"		p = (palette[int(matIndex[i])] * vec4(position,1.0)).xyz;	\n"
			"		blendedPos += weights[i] * p;								\n"
			"		n = (palette[int(matIndex[i])] * vec4(normal, 0.0)).xyz;	\n"
			"		norm +=	weights[i] * n;									\n"
			"	}																\n"
			"	gl_Position = matProjection * matView * matWorld * vec4(blendedPos, 1);\n"
			"}";
		recipe.fragmentShader =
			"#version 440				\n"
			"out vec4 fragColor;		\n"
			"in vec3 norm;				\n"
			"void main(){				\n"
			"	fragColor = vec4(1,1,1, 1);	\n"
			"}";

		renderer = ShaderChef::CookShaderProgram(recipe);

		mLayers.push_back(Layer());
		mCameras.push_back(Camera(Position3(0, 0, 20), Position3(0, 0, 0)));
		mMainCam = &mCameras[0];

		// create test animated model
		animModel = new AnimatedModel("", NULL, true);

		ArrayBuffer& arr = animModel->GetMesh()->GetArrayBuffer();
		arr.GenAttributeBuffer<Position3>(AttributeSemantic::POSITION, {
			Position3(0, 0, 0), Position3(5, 0, 0), Position3(8,0,0), 
			Position3(10, 0, 0), Position3(10, 0, 0), Position3(10, 0, 0)
		});
		arr.GenAttributeBuffer<Normal>(AttributeSemantic::NORMAL, {
			Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1), 
			Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1)
		});
		arr.GenAttributeBuffer<Texcoord>(AttributeSemantic::TEXCOORD0, {
			Texcoord(0, 1), Texcoord(1, 1), Texcoord(1, 0),
			Texcoord(1, 0), Texcoord(1, 0), Texcoord(1, 0)
		});
		arr.GenAttributeBuffer<Vector4>(AttributeSemantic::TEXCOORD1, {
			Vector4(0, -1, -1, -1), Vector4(1, -1, -1, -1), Vector4(2, -1, -1, -1),
			Vector4(2, -1, -1, -1), Vector4(1, -1, -1, -1), Vector4(1,2, -1, -1)
		});
		arr.GenAttributeBuffer<Vector4>(AttributeSemantic::TEXCOORD2, {
			Vector4(1, 0, 0, 0), Vector4(1, 0, 0, 0), Vector4(1, 0, 0, 0),
			Vector4(1, 0, 0, 0), Vector4(1, 0, 0, 0), Vector4(0.5, 0.5, 0, 0)
		});
		arr.SetDrawMode(ArrayBuffer::DrawMode::POINTS);

		std::vector<AnimatedModel::Joint> jtb;
		jtb.push_back(AnimatedModel::Joint(0, Matrix4(1.f)));
		jtb.push_back(AnimatedModel::Joint(1,
			Matrix4(
			1, 0, 0, 5,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1), &jtb[0]));
		jtb.push_back(AnimatedModel::Joint(1,
			Matrix4(
			1, 0, 0, 3,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1), &jtb[1]));
		
		AnimatedModel::Animator animator(10.0f, AnimatedModel::Animator::PlayMode::REPEAT);
		
		std::vector<AnimatedModel::Animator::Animation> anims;
		AnimatedModel::Animator::Animation anim;
		anim.datas.push_back({ 0, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anim.datas.push_back({ 5, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anim.datas.push_back({ 10, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anims.push_back(anim);

		anim.datas.clear();
		anim.datas.push_back({ 0, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anim.datas.push_back({ 5, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anim.datas.push_back({ 10, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anims.push_back(anim);

		anim.datas.clear();
		anim.datas.push_back({ 0, Vector3(0, 0, 0), Quaternion(Vector3::Forward, 0, true) });
		anim.datas.push_back({ 5, Vector3(0, 0, 0), Quaternion(Vector3::Forward, math::deg2rad(-45), true) });
		anim.datas.push_back({ 10, Vector3(0, 0, 0), Quaternion(Vector3::Forward, math::deg2rad(-90), true) });
		anims.push_back(anim);
		animator.SetAnimations(anims);
		animator.Run();

		animModel->SetJointTable(jtb);
		animModel->SetAnimator(animator);
		AddSceneComponent(animModel);
	}

	~TmpScene(){
	}

	void OnEnter() override{
	}

	void OnLeave() override{}

	void Update(){
		animModel->Update();
	}

	void Render(){
		//----------------
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());
		//----------------

		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());
		
		renderer->SetUniform("palette", &animModel->GetMatrixPalette()[0], animModel->GetMatrixPalette().size());
		renderer->SetUniform("matWorld", animModel->GetTransform().GetMatrix());
		animModel->Render();

		renderer->Unuse();
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height) override{
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);

		//----------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
		//----------------
	}
};






class TestScene2 : public AScene{
public:
	std::shared_ptr<ShaderProgram> renderer;

	// dynamic light list
	DirectionalLight* mLight;

	TestScene2(){
		ShaderProgram::Recipe recipe;
		recipe.attributes = {
				{ AttributeSemantic::POSITION, "position" },
				{ AttributeSemantic::NORMAL, "normal" }
		};
		recipe.vertexShader =
			"#version 440						\n"
			"in vec3 position;					\n"
			"in vec3 normal;					\n"
			"uniform mat4 matWorld;				\n"
			"uniform mat4 matView;				\n"
			"uniform mat4 matProjection;		\n"
			"out vec4 wpos;						\n"
			"out vec3 norm;						\n"
			"void main(){													\n"
			"	wpos = matWorld * vec4(position, 1);						\n"
			"	norm = (transpose(inverse(matWorld))*vec4(normal, 0)).xyz;	\n"
			"	gl_Position = matProjection * matView * wpos;				\n"
			"}";
		recipe.fragmentShader =
			"#version 440										\n"
			"struct DirLight{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	vec3 dir;										\n"
			"};													\n"
			"struct Material{									\n"
			"	vec4 amb;										\n"
			"	vec4 dif;										\n"
			"	vec4 spec;										\n"
			"	float shi;										\n"
			"};													\n"
			"													\n"
			"uniform mat4 matWorld;								\n"
			"uniform mat4 matView;								\n"
			"uniform mat4 matProjection;						\n"
			"uniform vec3 weye;									\n"
			"uniform DirLight light;							\n"
			"													\n"
			"in vec4 wpos;										\n"
			"in vec3 norm;										\n"
			"													\n"
			"out vec4 fragColor;								\n"
			"													\n"
			"Material mtrl = Material(vec4(0.3, 0.3, 0.3, 1),	\n"
			"					vec4(0.7,0.7,0.7,1),			\n"
			"					vec4(0.9, 0.9, 0.9, 1), 16.0);	\n"
			"													\n"
			"void main(){										\n"
			"	vec3 n = normalize(norm);						\n"
			"	vec3 v = normalize(weye - wpos.xyz);			\n"
			"	vec3 l = -light.dir;							\n"
			"	float sc=0;										\n"
			"	float cosNL = max(dot(n,l), 0.05);				\n"
			"													\n"
			"	if(cosNL > 0.05 ){								\n"
			"		vec3 r = 2*cosNL*n - l;						\n"
			"		sc = pow(max(dot(r,v),0), mtrl.shi);		\n"
			"	}												\n"
			"													\n"
			"	fragColor = cosNL*light.dif*mtrl.dif			\n"
			"				+ sc*light.spec*mtrl.spec			\n"
			"				+ light.amb*mtrl.amb;				\n"
			"}";
		renderer = ShaderChef::CookShaderProgram(recipe);

		mLayers.push_back(Layer());
		mCameras.push_back(Camera(Position3(0, 40, 30), Position3(0, 0, 0)));
		mMainCam = &mCameras[0];

		RigidCube* model = new RigidCube(10, 10, 10, 0, 0, 0, false);
		
		/*
		StaticModel* model = new StaticModel("", NULL, true);
		ArrayBuffer& arrbuf = model->GetMesh()->GetArrayBuffer();
		
		arrbuf.GenAttributeBuffer<Position3>(AttributeSemantic::POSITION, {
			Vector3(0, 5, 0), Vector3(-7, -3, 0), Vector3(7, -3, 0)
		});
		arrbuf.GenAttributeBuffer<Normal>(AttributeSemantic::NORMAL, {
			Normal(0, 0, 1), Normal(0, 0, 1), Normal(0, 0, 1)
		});
		arrbuf.GenAttributeBuffer<TriangleFace16>(AttributeSemantic::INDICES, {
			TriangleFace16(0, 1, 2)
		});
		arrbuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);
		*/

		model->SetCollider(std::unique_ptr<ACollider>(new OBoxCollider(model, 0, Vector3(5,5,5))));
		AddSceneComponent(model);
		mLayers[0].Push(model);

		mLight = new DirectionalLight(
			ColorRGBA(0.3, 0.3, 0.3, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1),
			ColorRGBA(0.9, 0.9, 0.9, 1));
		mLight->GetTransform().Rotate(0, math::deg2rad(-45), 0);
		AddSceneComponent(mLight);
	}

	~TestScene2(){}

	void OnEnter() override{
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
				const ACollider* bs = (*itr)->GetCollider();

				if (bs == NULL)
					continue;
				if (ray.IntersectWith(bs))
					graspComponent = (*itr);
			}
		});
		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_LBUTTON_UP,
			[&](const Position2& pos, real ext)->void{

		});
		Input::mouse.RegisterMouseHandler(
			Input::Mouse::EVENT_WHEEL,
			[&](const Position2& pos, real ext)->void{
			mMainCam->MoveForward(ext);
		});
	}

	void OnLeave() override{}

	void Update(){
	}

	void Render(){
		//----------------
		glMultTransposeMatrixf(mMainCam->GetViewMatrix().GetRawMatrix());
		//----------------

		renderer->Use();
		renderer->SetUniform("matView", mMainCam->GetViewMatrix());
		renderer->SetUniform("matProjection", mMainCam->GetProjMatrix());

		renderer->SetUniform("eyePos", mMainCam->GetEye());
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
	}

	// main scene camera view setting
	void OnScreenChanged(uinteger width, uinteger height) override{
		mMainCam->SetViewport(0, 0, width, height);
		mMainCam->Perspective(60, (real)width / (real)height, 0.1f, 1000.f);

		//----------------
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMultTransposeMatrixf(mMainCam->GetProjMatrix().GetRawMatrix());
		glMatrixMode(GL_MODELVIEW);
		//----------------
	}
};


//---------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow){
	// ============ initialization ===============
	gpEngine->InitializeApp(hInstance, nCmdShow, L"test", L"test");

	// =============== settings ==================
	// engine settings (after initialization)
	
	// user input handler setting
	Input::keyboard.RegisterKeyCombHandler([]()->void{
		gpEngine->Halt();
	}, Input::Keyboard::CODE_ESC);

	
	// add custom scene
	gpEngine->AddScene("scMain", new TestScene(), true);

	// initialize global settings
	gpEngine->SetClearColor(ColorRGBA(0.12f, 0.12f, 0.12f, 1.0f));
	gpEngine->ResizeWindow(1200, 800);

	// ============= run engine loop =============
//	gpEngine->GetTimer().FixateFlow(true);
//	gpEngine->GetTimer().SetFixedDeltaTime(1.f / 30.f);
	gpEngine->Run();

	// ======= release engine and user data ======
	gpEngine->ReleaseApp();

	return 0;
}