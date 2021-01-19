#pragma once
#include "Resources/Base/Resource.h"
#include <vector>
#include "ExternalTools/JSON/json.hpp"

#include "ExternalTools/MathGeoLib/include/Math/float4x4.h"

#include "Tools/OpenGL/Buffer.h"

struct aiNode;
struct aiScene;
class Object;
class r1Mesh;

class r1Model :
	public Resource
{
	struct Node {
		~Node() {
			for (auto i = children.begin(); i != children.end(); ++i)
				delete* i;
		}
		std::string name;

		uint64_t id_mesh = 0ULL;

		float4x4 transform = float4x4::identity;

		std::vector<Node*> children;
		Node* parent = nullptr;
	};

public:
	r1Model(const uint64_t& uid);
	~r1Model();

	void Load() override;
	void LoadNode(aiNode* root, const aiScene* scene, Node* parent);

	void CreateObject(Object* root);

	void CreateChildren(r1Model::Node* parent, Object* r);

	std::vector<r1Mesh*> meshes;
	Node* root = nullptr;
};

