#ifndef __SCENES_H__
#define __SCENES_H__

#include <string>
#include <vector>
#include "core.h"
#include "Transform.h"

namespace sarklib{

	// pure abstract scene component class
	// *the transformation of scene component is always done as T.R.(S) order
	// *transform x vertices = world coord vertices
	class ASceneComponent{
	private:
		Transform mTransform;
		bool mbVisible;

	public:
		ASceneComponent();

		virtual ~ASceneComponent();

		// render interface
		virtual void Render() = 0;

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

			void Sort(Vector3 pivotPosition);
		};

	protected:
		std::string mstrName;
		
		Layer mGUILayer;
		Layer mObjectLayer;
		Layer mBackLayer;

	public:
		AScene(std::string name);

		virtual ~AScene();

		const std::string& GetName() const;

		virtual void Render() = 0;
	};

}
#endif