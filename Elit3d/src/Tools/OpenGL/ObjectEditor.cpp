#include "Tools/OpenGL/ObjectEditor.h"
#include "Tools/OpenGL/OpenGLHelper.h"

#include "Core/Application.h"
#include "Modules/m1Render3D.h"
#include "Resources/r1Shader.h"

#include "ExternalTools/ImGui/imgui.h"
#include "ExternalTools/MathGeoLib/include/Math/float4x4.h"

#include "ExternalTools/mmgr/mmgr.h"

ObjectEditor::ObjectEditor() {
	vertices.size = 4; //Quad
	vertices.data = new float[vertices.size * 3]{
		0.f, 0.f, 0.f,
		1.f, 0.f, 0.f,
		1.f, 1.f, 0.f,
		0.f, 1.f, 0.f
	};
	indices.size = 6;
	indices.data = new unsigned int[indices.size]{
		0, 2, 1,
		0, 3, 2
	};
	uv.size = vertices.size;
	uv.data = new float[uv.size * 2]{
		0.f, 0.f,
		1.f, 0.f,
		1.f, 1.f,
		0.f, 1.f
	};

	oglh::GenVAO(VAO);
	oglh::GenArrayBuffer(vertices.id, vertices.size, sizeof(float), 3, vertices.data, 0, 3);
	oglh::GenElementBuffer(indices.id, indices.size, indices.data);
	oglh::GenArrayBuffer(uv.id, uv.size, sizeof(float), 2, uv.data, 1, 2);

	euler = { 30.f, 0.f, 0.f };
	rot = Quat::FromEulerXYZ(DegToRad(euler.x), DegToRad(euler.y), DegToRad(euler.z));
}

ObjectEditor::~ObjectEditor() {
	oglh::DeleteVAO(VAO, vertices.id, indices.id);
	oglh::DeleteBuffer(uv.id);

	delete[] vertices.data;
	delete[] indices.data;
	delete[] uv.data;
}

void ObjectEditor::Draw(r1Shader* shader)
{
	oglh::BindBuffers(VAO, vertices.id, indices.id);
	oglh::BindBuffer(uv.id);

	shader->SetMat4("model", float4x4::FromTRS(position, rot, float3(size * scale, 1.f)));

	oglh::DrawElements(indices.size);
}

nlohmann::json ObjectEditor::ToJson()
{
	nlohmann::json mesh;

	float4x4 mat = float4x4::FromTRS(position, rot, float3(size * scale, 1.f));
	for (int i = 0; i < 4; ++i) {
		float3 v = mat.MulPos(float3(vertices.data[i * 3], vertices.data[i * 3 + 1], vertices.data[i * 3 + 2]));
		mesh["object"]["vertices"].push_back(v.x);
		mesh["object"]["vertices"].push_back(v.y);
		mesh["object"]["vertices"].push_back(v.z);
		mesh["object"]["texCoords"].push_back(uv.data[i * 2]);
		mesh["object"]["texCoords"].push_back(uv.data[i * 2 + 1]);
	}

	mesh["mesh"]["name"] = name;
	mesh["mesh"]["position"] = position.SerializeToString();
	mesh["mesh"]["size"] = size.SerializeToString();
	mesh["mesh"]["scale"] = scale;
	mesh["mesh"]["rotation"] = rot.SerializeToString();

	return mesh;
}
