#pragma once

#include "Tools/Math/int2.h"

class Viewport
{
	friend class m1Render3D;
	friend class p1Scene;
private:
	Viewport();
	~Viewport();

	void Clear();

	void Update();
	void UpdateSize(int x, int y);
	void Blit() const;

	unsigned int GetTexture() const;

public:
	void Begin() const;
	void End() const;

public:
	float color[3] = { .2f, .2f, .2f };

private:
	int smaa = 32;
	int2 size;

	enum FBOIDS
	{
		FBO,
		FBO_MS,
		TEXTURE,
		TEXTURE_MS,
		RBO,
		RBO_MS
	};
	unsigned int ID[6];

	//Camera* camera = nullptr;
};
