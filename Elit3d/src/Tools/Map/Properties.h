#pragma once
#include <unordered_map>
#include <string>
#include "ExternalTools/JSON/json.hpp"
#include "ExternalTools/pugixml/pugixml.hpp"
class TypeVar;

class Properties
{
public:
	Properties();
	~Properties();

	void Display();

	void SaveProperties(nlohmann::json& propParent) const;
	void SaveProperties(pugi::xml_node& propParent) const;

	void LoadProperties(const nlohmann::json& propParent);
	void LoadProperties(const pugi::xml_node& propParent);

	void Clear();

private:
	void CreateProperty();
	void DisplayProperties();

private:
	std::unordered_map<std::string, TypeVar*> properties;
};

