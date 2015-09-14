#include "MeshRenderer.h"
#include <bgfx.h>
#include <bx/timer.h>
#include <bx/fpumath.h>
#include <bgfx_utils.h>

#include "StrideDataFixedArray.h"
#include "Elements/ElementsContainer.h"
#include "Elements/IElement.h"

#include "../shaders/fs_meshPlot.h"
#include "../shaders/vs_meshPlot.h"

#include <ocornut-imgui/imgui.h>
#include <imgui/imgui.h>


std::string meshViewOptionTitle = "Mesh view options";

struct Node2DVertex
{
	float m_x;
	float m_y;
	float m_v;

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Weight, 1, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl Node2DVertex::ms_decl;


void MeshRenderer::CreateMaterial(const uint8_t* v, uint32_t sizev, const uint8_t* f, uint32_t sizef)
{
	const bgfx::Memory* vs = bgfx::makeRef(v, sizev);
	const bgfx::Memory* fs = bgfx::makeRef(f, sizef);
	bgfx::ShaderHandle vsh = bgfx::createShader(vs);
	bgfx::ShaderHandle fsh = bgfx::createShader(fs);
	m_programs.push_back(bgfx::createProgram(vsh, fsh));
}

void MeshRenderer::Init()
{
	m_renderWireframe = true;
	m_renderNodes = true;
	m_showMeshViewOption = true;
	m_renderNodesNumbers = true;
	m_renderElementsNumbers = true;
	
	Node2DVertex::init();
	u_percantage = bgfx::createUniform("u_percantage", bgfx::UniformType::Vec4);
	u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
	
	CreateMaterial(vs_meshPlot_h, sizeof(vs_meshPlot_h), fs_meshPlot_h, sizeof(fs_meshPlot_h));
}

void MeshRenderer::AddMesh(const StrideDataFixedArray& nodes, const ElementsContainer& elements, const StrideDataFixedArray& values)
{
	Mesh m;

	int nodesCount = nodes.GetCount();
	Node2DVertex* vertices = new Node2DVertex[nodesCount];
	for (int i = 0; i < nodesCount; ++i)
	{
		vertices[i].m_x = nodes(i, 0);
		vertices[i].m_y = nodes(i, 1);
		vertices[i].m_v = 0;// float(i) / nodesCount;
	}
	m.m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(Node2DVertex) * nodesCount), Node2DVertex::ms_decl);

	m.nodesCopy = new StrideDataFixedArray;
	m.elementsCenters = new StrideDataFixedArray;

	nodes.MakeCopy(*m.nodesCopy);

	int elementsCount = elements.GetCount();
	m.elementsCenters->Init(2, elementsCount);
	{
		uint32_t* indices = new uint32_t[elementsCount * 3];
		for (int i = 0; i < elementsCount; ++i)
		{
			std::vector<int> ind = elements(i)->GetIndices();
			indices[3 * i + 0] = ind[0];
			indices[3 * i + 1] = ind[1];
			indices[3 * i + 2] = ind[2];
			(*m.elementsCenters)(i, 0) = ((*m.nodesCopy)(ind[0], 0) + (*m.nodesCopy)(ind[1], 0) + (*m.nodesCopy)(ind[2], 0)) / 3.0f;
			(*m.elementsCenters)(i, 1) = ((*m.nodesCopy)(ind[0], 1) + (*m.nodesCopy)(ind[1], 1) + (*m.nodesCopy)(ind[2], 1)) / 3.0f;
		}
		m.m_ibhTriang = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(uint32_t) * elementsCount * 3), BGFX_BUFFER_INDEX32);
	}
	{
		uint32_t* indices = new uint32_t[elementsCount * 6];
		for (int i = 0; i < elementsCount; ++i)
		{
			std::vector<int> ind = elements(i)->GetIndices();
			indices[6 * i + 0] = ind[0];
			indices[6 * i + 1] = ind[1];
			indices[6 * i + 2] = ind[1];
			indices[6 * i + 3] = ind[2];
			indices[6 * i + 4] = ind[2];
			indices[6 * i + 5] = ind[0];
		}
		m.m_ibhLines = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(uint32_t) * elementsCount * 6), BGFX_BUFFER_INDEX32);
	}
	m.m_mat = MT_Colored;
	m_meshes.push_back(m);
}

void MeshRenderer::ShowMeshViewOptions()
{
	if (!m_showMeshViewOption)
	{
		m_showMeshViewOption = true;
		ImGui::SetWindowFocus(meshViewOptionTitle.c_str());
		ResetMeshViewOptionsWindowPosition(true);
	}
}

void MeshRenderer::ResetMeshViewOptionsWindowPosition(bool force)
{
	ImVec2 vpos;
	vpos.x = m_width - 200;
	vpos.y = 21;
	ImGuiSetCond_ cond = force ? ImGuiSetCond_Always : ImGuiSetCond_Appearing;
	ImGui::SetWindowPos(meshViewOptionTitle.c_str(), vpos, cond);
	ImVec2 vsize;
	vsize.x = 200;
	vsize.y = 160;
	ImGui::SetWindowSize(meshViewOptionTitle.c_str(), vsize, cond);
}

void TransformToWorldSpace(float xInp, float yInp, float zInp, int width, int height, const Camera& camera, float& xOut, float& yOut)
{
	float pos[4];
	float posTransformed[4];
	pos[0] = xInp;
	pos[1] = yInp;
	pos[2] = zInp;
	pos[3] = 1.0f;
	float viewProj[16];
	camera.GetViewProj(viewProj);
	bx::vec4MulMtx(posTransformed, pos, viewProj);
	
	xOut = posTransformed[0] / 2.0f * width + width / 2.0f;
	yOut = height / 2.0f - posTransformed[1] / 2.0f * height;
}

void MeshRenderer::Render(int width, int height, const Camera& camera)
{
	m_width = width;
	float percantage[4];
	percantage[0] = 1.0f;
	bgfx::setUniform(u_percantage, &percantage, 1);
	
	if (m_showMeshViewOption)
	{
		if (ImGui::Begin(meshViewOptionTitle.c_str(), &m_showMeshViewOption))
		{
			ImGui::Checkbox("Render wireframe", &m_renderWireframe);
			ImGui::Checkbox("Render nodes", &m_renderNodes);
			ImGui::Checkbox("Render elements lables", &m_renderElementsNumbers);
			ImGui::Checkbox("Render nodes lables", &m_renderNodesNumbers);
			ResetMeshViewOptionsWindowPosition(false);
		}
		ImGui::End();
	}
	
	for (int i = 0, l = m_meshes.size(); i < l; ++i)
	{
		float color[4];
		color[0] = 0.1;
		color[1] = 0.6;
		color[2] = 0.3;
		bgfx::setUniform(u_color, &color, 1);

		bgfx::setVertexBuffer(m_meshes[i].m_vbh);
		bgfx::setIndexBuffer(m_meshes[i].m_ibhTriang);
		bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA);
		bgfx::submit(0, m_programs[m_meshes[i].m_mat]);

		if (m_renderWireframe)
		{
			color[0] = 0.1;
			color[1] = 0.3;
			color[2] = 0.2;
			bgfx::setUniform(u_color, &color, 1);

			bgfx::setVertexBuffer(m_meshes[i].m_vbh);
			bgfx::setIndexBuffer(m_meshes[i].m_ibhLines);
			bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_LINES);
			bgfx::submit(0, m_programs[m_meshes[i].m_mat]);
		}
		if (m_renderNodes)
		{
			color[0] = 0.8;
			color[1] = 0.6;
			color[2] = 0.2;
			bgfx::setUniform(u_color, &color, 1);

			bgfx::setVertexBuffer(m_meshes[i].m_vbh);
			bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_POINTS | BGFX_STATE_POINT_SIZE(3));
			bgfx::submit(0, m_programs[m_meshes[i].m_mat]);
		}

		if (m_renderElementsNumbers)
		{
			for (int j = 0; j < m_meshes[i].elementsCenters->GetCount(); ++j)
			{
				float x, y;
				TransformToWorldSpace(
					(*m_meshes[i].elementsCenters)(j, 0),
					(*m_meshes[i].elementsCenters)(j, 1),
					0.0f, width, height, camera, x, y);
				char label[32];
				sprintf(label, "%d", j);
				imguiDrawText(x, y, ImguiTextAlign::Right, label, 0xFFFFFF00);
			}
		}
		if (m_renderNodesNumbers)
		{
			for (int j = 0; j < m_meshes[i].nodesCopy->GetCount(); ++j)
			{
				float x, y;
				TransformToWorldSpace(
					(*m_meshes[i].nodesCopy)(j, 0),
					(*m_meshes[i].nodesCopy)(j, 1),
					0.0f, width, height, camera, x, y);
				char label[32];
				sprintf(label, "%d", j);
				imguiDrawText(x, y, ImguiTextAlign::Right, label, 0xFF00FFFF);
			}
		}
	}
}

void MeshRenderer::Clear()
{
	m_meshes.clear();
}