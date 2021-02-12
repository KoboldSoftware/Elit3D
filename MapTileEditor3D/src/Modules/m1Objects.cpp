#include "Modules/m1Objects.h"
#include "Objects/Object.h"
#include "Core/Application.h"
#include "Modules/m1Render3D.h"

#include "Resources/r1Shader.h"

#include "Tools/OpenGL/Viewport.h"

#include "Tools/System/Profiler.h"

#include "ExternalTools/mmgr/mmgr.h"

m1Objects::m1Objects(bool start_enabled) : Module("Objects", start_enabled)
{
}

m1Objects::~m1Objects()
{
	for (auto i = objects.begin(); i != objects.end(); ++i) {
		delete* i;
	}
}

Object* m1Objects::CreateEmptyObject()
{
	Object* obj = new Object();
	objects.push_back(obj);

	return obj;
}

bool m1Objects::Start()
{
	return true;
}

UpdateStatus m1Objects::Update()
{
	PROFILE_FUNCTION();

	App->render->GetViewport("scene")->Begin();

	auto shader = App->render->GetShader("default");
	shader->Use();

	for (auto i = objects.begin(); i != objects.end(); ++i) {
		(*i)->Update();
	}

	shader->SetMat4("model", float4x4::identity);

	App->render->GetViewport("scene")->End();

	return UpdateStatus::UPDATE_CONTINUE;
}