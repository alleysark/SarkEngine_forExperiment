#ifndef __SCENES_H__
#define __SCENES_H__

#include <string>
#include <vector>
#include <list>
#include <map>
#include "core.h"
#include "Transform.h"
#include "Camera.h"

namespace sark{

	// pure abstract scene components class.
	// all scene components have unique component id
	class ASceneComponent{
	public:
		typedef uint32 ComponentID;
		typedef std::list<ASceneComponent*> ChildComponentContainer;

	private:
		static ComponentID _nextComponentID;
		static ComponentID _getUniqueComponentID();

	protected:
		// unique component id
		ComponentID mComponentId;

		// name of scene component. it is duplicatable name
		std::string mComponentName;

		// parent scene component of this component.
		// scene component can be composed as hierarchical structure.
		// please do not make the circle in the family-tree.
		ASceneComponent* mParent;

		// child scene components of this component.
		// scene component can be composed as hierarchical structure.
		// please do not make the circle in the family-tree.
		ChildComponentContainer mChildren;

		// local scene component transform object
		Transform mTransform;

		// combined transform matrix of all ancestors
		Matrix4 mAbsoluteTransformMat;

		// component activation indicator
		bool mActivated;

	public:
		// relative distance of this and camera.
		// it doesn't guarantee the ensure value. it just used by outter class
		real rel_distance;

	public:
		// component id is automatically generated when constructor is called.
		ASceneComponent(ASceneComponent* parent = NULL);

		// component id is automatically generated when constructor is called.
		ASceneComponent(const std::string& name, ASceneComponent* parent = NULL);

		// every derived class have to ensure release of your resources.
		virtual ~ASceneComponent();

		// update interface
		virtual void Update() = 0;

		// render interface
		virtual void Render() = 0;


		// get scene component ID
		const ComponentID& GetComponentID() const;

		// get scene component name
		const std::string& GetComponentName() const;
		// set scene component name
		void SetComponentName(const std::string& name);


		// get parent component pointer
		ASceneComponent* GetParent() const;
		// set new parent. parent can be NULL to make this global component.
		void SetParent(ASceneComponent* newParent);


		// get a child of id
		ASceneComponent* GetChild(const ComponentID& id);
		// get a child who is firstly matched with queried name
		ASceneComponent* GetChild(const std::string& name);

		// push a child into its children container as uniquely
		bool PushChild(ASceneComponent* child);

		// pull a child from its children container. it doesn't delete pulled child from memory
		ASceneComponent* PullChild(const ComponentID& id);
		

		// get all the children who are matched with queried name
		const ChildComponentContainer GetChildren(const std::string& name);
		// save all the children who are matched with queried name 
		// into given list reference. it returns the size of result.
		uint32 GetChildren(const std::string& name, ChildComponentContainer& refContainer);

		// get the children container
		const ChildComponentContainer& GetChildren() const;


		// get transform object of this component. it is local transform object.
		Transform& GetTransform();

		// get combined transformation matrix of all ancestors
		const Matrix4& GetAbsoluteMatrix();

	private:
		// it considers the Transform class as its friend 
		// to allow the access of 'TransformChanged()' method
		friend Transform;

		// it makes its absolute transform matrix stained and also children's
		void TransformStained();
		
	public:
		// is this component activated?
		bool IsActive() const;
		// set component activation property
		void Activate(bool act);
	};



	// pure abstract scene class
	// it has basic properties and functions of scene
	class AScene{
	protected:
		// layer of scene component.
		// it has multi-method component containing system.
		// basically, components are contained in the map-like structure by its component id.
		// but for the support of sorted components, they are replicated into the list-like structure
		// as its memory address(pointer).
		class Layer{
		private:
			typedef std::map<ASceneComponent::ComponentID, ASceneComponent*> ComponentMap;
			typedef std::vector<ASceneComponent*> ComponentReplicaArray;

			ComponentMap components;
			ComponentReplicaArray componentReplicas;

			bool mNeedSorted;

		public:
			// if layer needs to be sorted, scene will sort it when camera is updated
			Layer(bool needSorted = false);
			// layer is deleted with all the contained scene components
			~Layer();

			// does it need to be sorted?
			bool NeedSorted() const;

			// sort all scene components in this layer by relative distance of input position
			void Sort(const Position3& position);

			// add scene component into this layer
			void AddSceneComponent(ASceneComponent* sceneComponent);

			// delete scene component from this layer
			void DeleteSceneComponent(ASceneComponent* sceneComponent);
			// delete scene component from this layer
			void DeleteSceneComponent(const ASceneComponent::ComponentID& componentId);

			// find the scene component from given component id
			ASceneComponent* FindSceneComponent(const ASceneComponent::ComponentID& componentId);

			// update all scene components
			void UpdateAll();

			// render all scene components
			void RenderAll();
		};

	protected:
		typedef std::vector<Layer*> LayerContainer;
		// scene can have multiple layers for management its scene components
		// to using layer container.
		// for example, physic-system layer for physical processing between 
		// the component and others in this layer.
		// and background layer for nonparticipant objects
		LayerContainer mLayers;

		typedef std::vector<Camera*> CameraContainer;
		CameraContainer mCameras;
		Camera* mMainCam;

	public:
		// scene create all scene-dependent resources at constructor.
		// there is no any initializer interface. 
		// so, every derived class have to ensure initialization of "your resources".
		AScene();

		// scene deletes all scene-dependent resources at destructor.
		// there is no any release interface.
		// so, every derived class have to ensure release of "your resources".
		virtual ~AScene();

		// update interface
		virtual void Update() = 0;

		// render interface
		virtual void Render() = 0;
	};

}
#endif