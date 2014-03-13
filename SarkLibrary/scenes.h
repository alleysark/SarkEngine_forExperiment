#ifndef __SCENES_H__
#define __SCENES_H__

#include <string>
#include <vector>
#include <map>
#include "core.h"
#include "Transform.h"
#include "Camera.h"

namespace sarklib{

	// pure abstract scene components class.
	// all scene components have unique component id
	class ASceneComponent{
	public:
		typedef uint32 ComponentID;

	private:
		static ComponentID _nextComponentID;
		static ComponentID _getUniqueComponentID();

	protected:
		// unique component id
		ComponentID mComponentId;

		// transformation helper property
		Transform mTransform;
		// component availability indicator
		bool mbEnable;

	public:
		// relative distance of this and camera.
		// it doesn't guarantee the ensure value. it just used by outter class
		real rel_distance;

	public:
		ASceneComponent();

		// every derived class have to ensure release of your resources.
		virtual ~ASceneComponent();

		// interface
		virtual void Update() = 0;
		virtual void Render() = 0;


		const ComponentID& GetComponentID() const;

		Transform& GetTransform();

		bool IsEnable() const;
		void Enable(bool enable);
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