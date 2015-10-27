#include "VertexDeclarations.h"

bgfx::VertexDecl NodeVertex::ms_decl;
bgfx::VertexDecl SignVertex::ms_decl;

void NodeVertex::init()
{
	ms_decl
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
		.end();
};

void SignVertex::init()
{
	ms_decl
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.end();
};