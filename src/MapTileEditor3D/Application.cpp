#include "Application.h"

#include <Windows.h>
#include <shellapi.h>

#include "Logger.h"

#include "m1Input.h"
#include "m1Window.h"
#include "m1Render3D.h"
#include "m1Scene.h"
#include "m1GUI.h"
#include "m1Objects.h"
#include "m1Camera3D.h"
#include "m1Resources.h"
#include "m1Importer.h"
#include "m1Events.h"
#include "m1MapEditor.h"

#include "FileSystem.h"
#include "Random.h"
#include "OpenGLHelper.h"

#include "Profiler.h"

#include "ExternalTools/mmgr/mmgr.h"

Application::Application() 
{
}

Application::~Application()	
{
}

bool Application::Init()
{
	PROFILE_FUNCTION();
	//Create instances of modules
	input = new m1Input();
	window = new m1Window();
	render = new m1Render3D();
	scene = new m1Scene();
	gui = new m1GUI();
	objects = new m1Objects();
	camera = new m1Camera3D();
	resources = new m1Resources();
	importer = new m1Importer();
	events = new m1Events();
	map_editor = new m1MapEditor();

	//Assign order of execution to modules NOTE: Inverse order to CleanUp()
	modules.push_back(events);
	modules.push_back(input);
	modules.push_back(window);

	modules.push_back(resources);
	modules.push_back(importer);

	modules.push_back(camera);
	modules.push_back(scene);
	modules.push_back(map_editor);
	modules.push_back(objects);

	modules.push_back(gui);
	modules.push_back(render);

	//Create helpers
	file_system = new FileSystem();
	random = new Random();

	nlohmann::json conf = file_system->OpenJSONFile("Configuration/Configuration.json")["App"];

	if (conf.is_null())
		LOGNE("Configuration.json not found");

	name.assign(conf.value("name", "PROGRAM"));
	version.assign(conf.value("version", "0.1"));

	for (auto i = modules.begin(); i != modules.end(); ++i) {
		LOGN("Initializing module %s", (*i)->name.c_str());
		(*i)->Init(conf[(*i)->name]);
	}

	FillSysInfo();

	return true;
}

bool Application::Start()
{
	PROFILE_FUNCTION();

	for (auto i = modules.begin(); i != modules.end(); ++i) {
		LOGN("Starting module %s", (*i)->name.c_str());
		(*i)->Start();
	}


	return true;
}

void Application::PrepareUpdate()
{
	last_time = time;
	time = SDL_GetPerformanceCounter();
	++frame_count;

	dt = (float)((time - last_time) / (double)SDL_GetPerformanceFrequency());
	framerate = 1 / dt;
}

UpdateStatus Application::Update()
{
	PROFILE_FUNCTION();
	UpdateStatus ret = UpdateStatus::UPDATE_CONTINUE;

	PrepareUpdate();
	
	{
		PROFILE_SECTION("Application::PreUpdate");
		for (auto i = modules.begin(); i != modules.end(); ++i) {
			ret = (*i)->PreUpdate();
			if (ret != UpdateStatus::UPDATE_CONTINUE) {
				LOGN("Module %s returned %s on PreUpdate()", (*i)->name.c_str(), Module::UpdateStatusToString(ret).c_str());
				return ret;
			}
		}
	}
	
	{
		PROFILE_SECTION("Application::Update");
		for (auto i = modules.begin(); i != modules.end(); ++i) {
			ret = (*i)->Update();
			if (ret != UpdateStatus::UPDATE_CONTINUE) {
				LOGN("Module %s returned %s on PreUpdate()", (*i)->name.c_str(), Module::UpdateStatusToString(ret).c_str());
				return ret;
			}
		}
	}
	
	{
		PROFILE_SECTION("Application::PostUpdate");
		for (auto i = modules.begin(); i != modules.end(); ++i) {
			ret = (*i)->PostUpdate();
			if (ret != UpdateStatus::UPDATE_CONTINUE) {
				LOGN("Module %s returned %s on PreUpdate()", (*i)->name.c_str(), Module::UpdateStatusToString(ret).c_str());
				return ret;
			}
		}
	}

	FinishUpdate();

	return ret;
}

void Application::FinishUpdate()
{
}

bool Application::CleanUp()
{
	PROFILE_FUNCTION();
	bool ret = true;

	delete file_system;
	delete random;

	for (auto i = modules.rbegin(); i != modules.rend(); ++i) {
		LOGN("Cleaning Up module %s", (*i)->name.c_str());
		ret = (*i)->CleanUp();
		delete* i;
		*i = nullptr;
	}

	return ret;
}

const char* Application::GetName()
{
	return name.c_str();
}

inline const char* Application::GetName() const
{
	return name.c_str();
}

const char* Application::GetVersion()
{
	return version.c_str();
}

const char* Application::GetVersion() const
{
	return version.c_str();
}

float Application::GetDt() const
{
	return dt;
}

unsigned int Application::GetFrames() const
{
	return frame_count;
}

void Application::ExecuteURL(const char* url)
{
	// TODO: set it for linux and mac
	ShellExecute(0, 0, url, 0, 0, SW_SHOW);
}

void Application::FillSysInfo()
{
	sys_info.platform.assign(SDL_GetPlatform());
	sys_info.cpu_cores = SDL_GetCPUCount();
	sys_info.ram_mb = SDL_GetSystemRAM();

	sys_info.ogl_version = oglh::GetVersion();
	sys_info.vendor = oglh::GetVendor();
	sys_info.model = oglh::GetModel();

	std::string file = {
		"Platform: " + sys_info.platform + 
		"\nCPU:\n" +
		"\tModel: \n"
		"\tNumber Cores: " + std::to_string(sys_info.cpu_cores) + 
		"\nRAM: " + std::to_string(sys_info.ram_mb) + "mb\n" +
		"GPU:\n" + 
		"\tOpenGL version: " + sys_info.ogl_version + 
		"\n\tGraphics Card:\n\t\tVendor: " + sys_info.vendor + "\n\t\tModel: " + sys_info.model
	};

	FileSystem::SaveTextFile("SystemInfo.txt", file.c_str());
}
