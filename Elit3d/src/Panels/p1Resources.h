#pragma once
#include "Panels/Base/Panel.h"
#include "Tools/FileSystem.h"

#include "Modules/m1Resources.h"

class p1Project :
	public Panel
{
public:
	p1Project(bool start_enabled = true, bool appear_mainmenubar = true, bool can_close = true);
	~p1Project();

	void Start() override;
	void Update() override;

	void DragDropTargetFolder(const std::string& i);

private:
	void SideTreeFolder(const Folder* folder);

private:
	m1Resources::EResourceType GetEType(const std::string& file) const; //TODO inline

private:
	Folder* root = nullptr;
	Folder* selected = nullptr;

	std::map<m1Resources::EResourceType, unsigned int> ids;

	std::string payload;

	float size = 0.5f;
};

