#pragma once

#include "Panels/Base/Panel.h"

#define COMPARE(selected, new_selected) if (((std::string*)selected)->compare(((std::string*)new_selected)) == 0) {

class p1Inspector :
	public Panel
{
public:
	enum class SelectedType {
		NONE = -1,

		OBJECT,
		LAYER,
		EDITOR_MAP,

		RESOURCE,

		MAX
	};
public:
	p1Inspector(bool start_enabled = true, bool appear_mainmenubar = true, bool can_close = true);
	~p1Inspector();

	void Update() override;

	void SetSelected(void* ptr, SelectedType type);

private:
	void* selected = nullptr;
	SelectedType type = SelectedType::NONE;

	bool loaded = false;

	char bufname[30];
};

