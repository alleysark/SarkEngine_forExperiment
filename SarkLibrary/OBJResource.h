#ifndef __OBJ_RESOURCE_H__
#define __OBJ_RESOURCE_H__

#include <vector>
#include "core.h"
#include "resources.h"
#include "primitives.hpp"

namespace sark {

	class StaticModel;
	class ASceneComponent;

	// OBJ model format resource.
	class OBJResource : public IResource, public IResourceLoader<OBJResource> {
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
		
		// make model from this
		StaticModel* MakeModel(const std::string& name = "", 
			ASceneComponent* parent = NULL, bool activate = true);

		// load obj resource.
		static OBJResource* LoadImp(const std::string& path);
	};

}
#endif