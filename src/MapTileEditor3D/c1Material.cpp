#include "c1Material.h"

#include <GL/glew.h>

#include "Application.h"
#include "m1Render3D.h"
#include "m1Resources.h"
#include "r1Texture.h"

#include "ExternalTools/ImGui/imgui.h"

#include "ExternalTools/mmgr/mmgr.h"

c1Material::c1Material(Object* obj) : Component(obj, Type::Material)
{
	shader = App->render->bShader;

	tex = App->resources->Find("cat");
	auto r = App->resources->Get(tex);
	r->Attach();
}

c1Material::~c1Material()
{
}

void c1Material::BindTex()
{
	auto t = (r1Texture*)App->resources->Get(tex);
	if (t != nullptr)
		glBindTexture(GL_TEXTURE_2D, t->GetBufferID());
}

void c1Material::UnBindTex()
{
	glBindTexture(GL_TEXTURE_2D, NULL);
}

void c1Material::OnInspector()
{
	static bool choose_texture = false;
	if (ImGui::CollapsingHeader("Material")) {
		if (tex != 0u) {
			auto t = (r1Texture*)App->resources->Get(tex);
			if (t != nullptr) {
				ImGui::Text("Texture: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.f, 0.6f, 0.f, 1.f), "%s", t->name.c_str());

				ImGui::Text("UID: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.f, 0.6f, 0.f, 1.f), "%s", std::to_string(t->GetUID()).c_str());

				ImGui::Text("Referneces: "); ImGui::SameLine();
				ImGui::TextColored(ImVec4(1.f, 0.6f, 0.f, 1.f), "%u", t->references);

				ImGui::Image((void*)(intptr_t)(t->GetBufferID()), ImVec2(200, 200), ImVec2(0, 0), ImVec2(1, -1));
				if (ImGui::Button("Change texture")) {
					if (!choose_texture) {
						choose_texture = true;
						auto vec = App->resources->GetVectorOf(Resource::Type::Texture);
						for (auto i = vec.begin(); i != vec.end(); ++i) {
							if ((*i)->GetUID() != tex)
								((r1Texture*)(*i))->Attach();
						}
					}
				}

				if (choose_texture) {
					ChooseTextureWindow(choose_texture);
				}
			}
		}
	}
}

void c1Material::ChooseTextureWindow(bool& choose_texture)
{
	bool window = ImGui::Begin("Choose Texture", &choose_texture);

	if (!choose_texture) {
		auto vec = App->resources->GetVectorOf(Resource::Type::Texture);
		for (auto i = vec.begin(); i != vec.end(); ++i) {
			if ((*i)->GetUID() != tex)
				((r1Texture*)(*i))->Detach();
		}
	}

	if (window) {
		auto vec = App->resources->GetVectorOf(Resource::Type::Texture);

		for (auto i = vec.begin(); i < vec.end(); ++i) {
			if ((*i)->GetUID() != tex && ImGui::ImageButton((ImTextureID)((r1Texture*)(*i))->GetBufferID(), ImVec2(120, 120), ImVec2(0, 0), ImVec2(1, -1))) {
				(*i)->Attach();
				tex = (*i)->GetUID();
				choose_texture = false;
				for (auto j = vec.begin(); j != vec.end(); ++j) {
					((r1Texture*)(*j))->Detach();
				}
			}
		}
		ImGui::End();
	}
}