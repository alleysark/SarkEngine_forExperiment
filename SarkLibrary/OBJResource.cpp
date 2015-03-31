#include "OBJResource.h"
#include "Debug.h"
#include "StaticModel.h"
#include "ArrayBuffer.h"
#include "Mesh.h"
#include "tools.h"
#include <fstream>
#include <string>
#include <iterator>
#include <sstream>

namespace sark {

	OBJResource::OBJResource() {
	}

	OBJResource::~OBJResource() {
	}

	// get vertices of the object
	std::vector<Vector3> OBJResource::GetVertices() const {
		return mVertices;
	}

	// get triangle faces of the object
	std::vector<TriangleFace16> OBJResource::GetFaces() const {
		return mFaces;
	}

	// get vertex normals of the object
	std::vector<Vector3> OBJResource::GetNormals() const {
		return mNormals;
	}

	// get texture coordinates of the object
	std::vector<Vector2> OBJResource::GetTexcoords() const {
		return mTexcoords;
	}

	// make this model resource to be center
	void OBJResource::MakeItCenter() {
		Vector3 center = tool::ComputeCenterOfMass(mVertices);
		
		auto itr = mVertices.begin(), end = mVertices.end();
		for (; itr != end; itr++) {
			(*itr) -= center;
		}
	}

	// create model component from this resource
	AModel* OBJResource::CreateModel(const std::string& name, 
		ASceneComponent* parent, bool activate) const
	{
		StaticModel* model = new StaticModel(name, parent, activate);

		ArrayBuffer& arrbuf = model->GetMesh()->GetArrayBuffer();
		
		arrbuf.GenAttributeBuffer<Vector3>(AttributeSemantic::POSITION, mVertices);
		if (mNormals.size() != 0)
			arrbuf.GenAttributeBuffer<Vector3>(AttributeSemantic::NORMAL, mNormals);
		if (mTexcoords.size() != 0)
			arrbuf.GenAttributeBuffer<Vector2>(AttributeSemantic::TEXCOORD0, mTexcoords);
		if (mFaces.size() != 0) {
			arrbuf.GenAttributeBuffer<TriangleFace16>(AttributeSemantic::INDICES, mFaces);
		}
		arrbuf.SetDrawMode(ArrayBuffer::DrawMode::TRIANGLES);

		return model;
	}

	// load obj resource.
	s_ptr<OBJResource> OBJResource::LoadImp(const std::string& path) {
		std::ifstream fin(path.c_str());
		if (!fin.is_open()) {
			LogError("cannot open file " + path);
			return NULL;
		}

		std::vector<Vector3> vertices;
		std::vector<TriangleFace16> faces;
		std::vector<Vector3> normals;
		std::vector<Vector2> texcoords;

		integer faceDef = 0;	//001: v
								//011: v/vt
								//111: v/vt/vn
								//101: v/vn

		std::string line;

		while (std::getline(fin, line)) {
			if (line == "" || line[0] == '#')
				continue;

			std::istringstream buf(line);
			std::istream_iterator<std::string> beg(buf), end;

			std::vector<std::string> toks(beg, end);

			if (toks[0] == "v") {
				vertices.push_back(
					Vector3(
					std::stof(toks[1]),
					std::stof(toks[2]),
					std::stof(toks[3]))
					);

				// set vertex flag
				faceDef |= 1;
			}
			else if (toks[0] == "vt") {
				texcoords.push_back(
					Vector2(
					std::stof(toks[1]),
					std::stof(toks[2]))
					);

				// set texcoord flag
				faceDef |= 2;
			}
			else if (toks[0] == "vn") {
				normals.push_back(
					Vector3(
					std::stof(toks[1]),
					std::stof(toks[2]),
					std::stof(toks[3]))
					);

				// set normal flag
				faceDef |= 4;
			}
			else if (toks[0] == "f") {
				TriangleFace16 face;
				switch (faceDef) {
				case 1: // v
					face.a = uint16(std::stoi(toks[1])) - 1;
					face.b = uint16(std::stoi(toks[2])) - 1;
					face.c = uint16(std::stoi(toks[3])) - 1;
					break;
				case 3: // v/vt
				case 5: // v/vn
				case 7: // v/vt/vn
					// just read the front value v of each token
					for (integer i = 0; i < 3; i++) {
						std::string& tok = toks[i + 1];
						face.idx[i] = uint16(std::stoi(tok.substr(0, tok.find('/')))) - 1;
					}
					break;
				}
				faces.push_back(face);
			}
		}

		s_ptr<OBJResource> obj = s_ptr<OBJResource>(new OBJResource());
		obj->mVertices = vertices;
		obj->mFaces = faces;
		obj->mNormals = normals;
		obj->mTexcoords = texcoords;
		
		return obj;
	}

}