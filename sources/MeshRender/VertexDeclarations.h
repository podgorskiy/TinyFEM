#pragma once 

#include <bgfx.h>

struct NodeVertex
{
	float m_pos[3];
	float m_val;
	float m_def[3];

	static void init();

	static bgfx::VertexDecl ms_decl;
};


struct SignVertex
{
	float m_pos[3];
	
	static void init();

	static bgfx::VertexDecl ms_decl;
};
