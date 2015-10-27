#pragma once

#include <bgfx.h>

struct LegendMesh
{
	static void Init();

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};

struct LoadSign
{
	static void Init();

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};

struct ConstSign
{
	static void Init();

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};
