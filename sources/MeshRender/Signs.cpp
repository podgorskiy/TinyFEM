#include "VertexDeclarations.h"
#include "Signs.h"

bgfx::VertexBufferHandle LegendMesh::vbh;
bgfx::IndexBufferHandle LegendMesh::ibh;

bgfx::VertexBufferHandle LoadSign::vbh;
bgfx::IndexBufferHandle LoadSign::ibh;

bgfx::VertexBufferHandle ConstSign::vbh;
bgfx::IndexBufferHandle ConstSign::ibh;

void LegendMesh::Init()
{
	static const NodeVertex vertices[4] =
	{
		{ { -1.0f, -1.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f } },
		{ { 1.0f, -1.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f } },
		{ { 1.0f, 1.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f } },
		{ { -1.0f, 1.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f } },
	};
	static const uint16_t indices[6] =
	{
		0, 1, 2, 0, 2, 3
	};
	vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), NodeVertex::ms_decl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
};

void LoadSign::Init()
{
	static const SignVertex vertices[6] =
	{
		{ { 0.0f, 0.0f, 0.0f } },
		{ { 1.0f, 0.0f, 0.0f } },
		{ { 0.7f, 0.1f, 0.0f } },
		{ { 0.7f, 0.0f, 1.0f } },
		{ { 0.7f, -0.1f, 0.0f } },
		{ { 0.7f, 0.0f, -1.0f } },
	};
	static const uint16_t indices[16] =
	{
		0, 1,
		1, 2,
		1, 3,
		1, 4,
		1, 5,
		2, 3,
		3, 4,
		4, 5,
	};
	SignVertex::init();
	vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), SignVertex::ms_decl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
};

void ConstSign::Init()
{
	static const SignVertex vertices[6] =
	{
		{ { 0.0f, 0.0f, 0.0f } },
		{ { -0.2f, 0.0f, 0.0f } },
		{ { -0.2f, 0.15f, 0.0f } },
		{ { -0.2f, 0.0f, 0.15f } },
		{ { -0.2f, -0.15f, 0.0f } },
		{ { -0.2f, 0.0f, -0.15f } },
	};
	static const uint16_t indices[16] =
	{
		0, 1,
		1, 2,
		1, 3,
		1, 4,
		1, 5,
		2, 3,
		3, 4,
		4, 5,
	};
	SignVertex::init();
	vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), SignVertex::ms_decl);
	ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
};
