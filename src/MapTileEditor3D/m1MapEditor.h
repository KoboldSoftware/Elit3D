#pragma once

#include "Module.h"

#include "ExternalTools/MathGeoLib/include/Math/float3.h"

class Object;
class p1Scene;
class p1Tileset;
class Layer;

class m1MapEditor :
    public Module
{
public:
    m1MapEditor();
    ~m1MapEditor();

    bool Start() override;
    UpdateStatus Update() override;
    bool CleanUp() override;

    void SaveMap();
    void LoadMap(const uint64_t& id);

    void MousePicking(const float3& position);
    void ResizeMap(int width, int height);

    bool ValidMap() const;

private:
    uint64_t map = 0ULL;
    
    p1Tileset* panel_tileset = nullptr;
};

