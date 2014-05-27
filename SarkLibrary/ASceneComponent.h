#ifndef __A_SCENE_COMPONENT_H__
#define __A_SCENE_COMPONENT_H__

#include <string>
#include <vector>
#include <list>
#include <map>
#include "core.h"
#include "Transform.h"

namespace sark{

	class ACollider;
	class Mesh;
	class RigidBody;

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

		// component activation indicator
		bool mActivated;

	public:
		// relative distance of this and camera.
		// it doesn't guarantee the ensure value. it just used by outter class
		real rel_distance;

	public:
		// component id is automatically generated when constructor is called.
		// 'name' can be empty to make default name.
		// 'parent' can be NULL for the root scene component.
		// 'activate' avtivates this scene component or not.
		ASceneComponent(const std::string& name, ASceneComponent* parent, bool activate);

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
		ChildComponentContainer& GetChildren();


		// it considers the Transform class as its friend 
		// to allow the accessing component hierarchy.
		friend Transform;

		// get transform object of this component. it is local transform object.
		Transform& GetTransform();

		// get collider. it is Nullable pointer.
		virtual const ACollider* GetCollider() const = 0;
		
		// set new collider. or set NULL.
		// you may pass new collider through memory allocation.
		// *note: do not pass the address of plane variable.
		virtual void SetCollider(ACollider* newColl) = 0;

		// get mesh object of scene component.
		// it can be NULL for the shapeless component like light.
		virtual Mesh* GetMesh() = 0;

		// get rigid body. it can be NULL for the non-rigid body.
		virtual RigidBody* GetRigidBody() = 0;

		// is this component activated?
		bool IsActive() const;
		// set component activation property
		void Activate(bool act);
	};

}
#endif