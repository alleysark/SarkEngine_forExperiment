#ifndef __SCENES_H__
#define __SCENES_H__

#include <string>
#include <vector>
#include <map>
#include "core.h"
#include "Transform.h"

namespace sarklib{

	// pure abstract scene component class.
	// all scene components have unique component id
	class ASceneComponent{
	public:
		typedef uint32 ComponentID;

	private:
		static ComponentID _nextComponentID;
		static ComponentID _getUniqueComponentID();

	protected:
		ComponentID mComponentId;

		Transform mTransform;
		bool mbVisible;

	public:
		ASceneComponent();

		virtual ~ASceneComponent();

		// render interface
		virtual void Render() = 0;


		const ComponentID& GetComponentID() const;

		Transform& GetTransform();

		bool IsVisible() const;
		void SetVisibility(bool visibility);
	};



	// pure abstract scene class
	// it has basic properties of scene
	class AScene{
	protected:
		class Layer{
		public:
			typedef std::vector<ASceneComponent*> ComponentContainer;
			ComponentContainer sceneComponents;
		private:
			bool mbNeedSorted;

		public:
			Layer(bool needSorted = false);
			~Layer();

			void Sort(Vector3 pivotPosition);

			void AddSceneComponent(ASceneComponent* sceneComponent);

			ASceneComponent* FindSceneComponent(const ASceneComponent::ComponentID& componentId);
		};

	protected:
		std::string mstrName;
		
		typedef std::map<std::string, Layer*> LayerContainer;
		LayerContainer mLayers;

	public:
		AScene(std::string name);

		virtual ~AScene();

		// render interface
		virtual void Render() = 0;


		const std::string& GetName() const;

		bool AddLayer(std::string layerName, bool needSorted = false);

		bool AddSceneComponent(ASceneComponent* sceneComponent, std::string layerName);

		ASceneComponent* GetSceneComponent(const ASceneComponent::ComponentID& componentId, std::string layerName);
	};

}
#endif