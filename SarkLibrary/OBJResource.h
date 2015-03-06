#ifndef __OBJ_RESOURCE_H__
#define __OBJ_RESOURCE_H__

#include <vector>
#include "core.h"
#include "resources.h"
#include "primitives.hpp"

namespace sark {

	// OBJ model format resource.
	class OBJResource : public IModelResource, public IResourceLoader<OBJResource> {
	private:
		// vertex array
		std::vector<Vector3> mVertices;

		// face array
		std::vector<TriangleFace16> mFaces;

		// vertex normal array
		std::vector<Vector3> mNormals;

		// texture coordinate array
		std::vector<Vector2> mTexcoords;

	public:
		OBJResource();
		~OBJResource();

		// get vertices of the object
		std::vector<Vector3> GetVertices() const;

		// get triangle faces of the object
		std::vector<TriangleFace16> GetFaces() const;

		// get vertex normals of the object
		std::vector<Vector3> GetNormals() const;

		// get texture coordinates of the object
		std::vector<Vector2> GetTexcoords() const;
		
		// make this model resource to be center
		void MakeItCenter() override;

		// create model component from this resource
		AModel* CreateModel(const std::string& name = "",
			ASceneComponent* parent = NULL, bool activate = true) const override;

		// load obj resource.
		static OBJResource* LoadImp(const std::string& path);
	};

}
#endif