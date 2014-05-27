#ifndef __A_SCENE_H__
#define __A_SCENE_H__

#include <string>
#include <vector>
#include <list>
#include <map>
#include "core.h"
#include "ASceneComponent.h"
#include "Camera.h"

namespace sark{

	// pure abstract scene class.
	// 'scene' is one of a major element of the engine.
	// it is generally though of as the action in a single location(scene space)
	// and continuous time.
	// scene is composed of various types of scene components(objects, lights, sounds, etc..),
	// various physics-systems, cameras to take the view or graphic interfaces.
	// it has basic properties and functions of scene
	class AScene{
	public:
		// layer of scene component.
		// basically, components are contained in the component map of scene.
		// but to deal them efficiently(sortable, selectable, etc..),
		// they are replicated into the 'layer' as its memory address(pointer).
		// and it also be replicated other layers for the other purpose.
		class Layer{
		public:
			typedef std::vector<ASceneComponent*> ReplicaArray;
			typedef ReplicaArray::iterator ReplicaArrayIterator;
		private:
			ReplicaArray mReplicas;

		public:
			Layer();
			~Layer();

			// push component into this layer
			void Push(ASceneComponent* component);

			// pop component from this layer
			void Pop(ASceneComponent* component);
			// pop component from this layer
			void Pop(const ASceneComponent::ComponentID& componentId);
			// pop component from this layer
			void Pop(ReplicaArrayIterator itrator);

			// sort all scene components in this layer by relative distance of input position
			void Sort(const Position3& position);

			// get array iterator of begin
			ReplicaArrayIterator Begin();

			// get array iterator of end
			ReplicaArrayIterator End();

			// clear layer
			void Clear();
		};
		
		typedef std::map<ASceneComponent::ComponentID, ASceneComponent*> ComponentMap;
		typedef std::vector<Layer> LayerContainer;

		typedef std::vector<Camera> CameraContainer;

	protected:
		// map-like component container.
		// it contains whole scene components in this scene.
		ComponentMap mComponents;

		// scene has multi-method component containing system called 'layer'.
		// a scene component can be handled by one or more layers.
		// 
		// layers management its scene components. for the various purpose.
		// for example, physic-system layer, selectable component layer or
		// alpha-object sorting system layer, etc..
		LayerContainer mLayers;

		// scene can have one or more cameras.
		CameraContainer mCameras;
		Camera* mMainCam;

	public:
		// scene create all scene-dependent resources at constructor.
		// there is no any initializer interface. 
		// so, every derived class have to ensure initialization all of "your resources".
		AScene();

		// scene deletes all scene-dependent resources at destructor.
		// there is no any release interface.
		// so, every derived class have to ensure release all of "your resources".
		virtual ~AScene();

		// clear whole scene components
		void ClearSceneComponents();

		// add scene component
		bool AddSceneComponent(ASceneComponent* sceneComponent);

		// delete scene component by address
		bool DeleteSceneComponent(ASceneComponent* sceneComponent);
		// delete scene component by component id
		bool DeleteSceneComponent(const ASceneComponent::ComponentID& componentId);
		// delete all scene components by component name. it returns count of deleted components.
		uinteger DeleteSceneComponents(const std::string& componentName);

		// find the scene component from given component id
		ASceneComponent* FindSceneComponent(const ASceneComponent::ComponentID& componentId);
		// find whole scene components matched with given component name
		std::list<ASceneComponent*> FindSceneComponents(const std::string& componentName);


		// update interface
		virtual void Update() = 0;

		// render interface
		virtual void Render() = 0;

		// on enter to this scene.
		// it is called by scene manager of engine.
		virtual void OnEnter() = 0;

		// on leave from this scene.
		// it is called by scene manager of engine.
		virtual void OnLeave() = 0;

		// main scene camera view setting
		virtual void OnScreenChanged(uinteger width, uinteger height) = 0;
	};

}
#endif