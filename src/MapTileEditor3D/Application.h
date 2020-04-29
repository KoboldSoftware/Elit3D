#pragma once

#include <vector>
#include <string>

enum class UpdateStatus;

class Module;
class m1Window;
class m1Render3D;
class m1Input;
class m1Scene;
class m1GUI;
class m1Objects;
class m1Camera3D;

class FileSystem;

class Application
{
public:
	Application();
	~Application();

	bool Init();
	bool Start();
	UpdateStatus Update();
	bool CleanUp();

public:
	m1Window*	window = nullptr;
	m1Render3D* render = nullptr;
	m1Input*	input  = nullptr;
	m1Scene*	scene = nullptr;
	m1GUI*		gui = nullptr;
	m1Objects* objects = nullptr;
	m1Camera3D* camera = nullptr;

	FileSystem* file_system = nullptr;

public:
	const char* GetName();
	const char* GetName() const;

private:
	std::vector<Module*> modules;

	std::string name;
};

extern Application* App;