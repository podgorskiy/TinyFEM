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
#include "../shaders/fs_sign.h"
#include "../shaders/vs_sign.h"

#include <ocornut-imgui/imgui.h>
#include <imgui/imgui.h>

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

std::string meshViewOptionTitle = "Mesh view options";

struct NodeVertex
{
	float m_pos[3];
	float m_val;
	float m_def[3];

	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord0, 1, bgfx::AttribType::Float)
			.add(bgfx::Attrib::TexCoord1, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl NodeVertex::ms_decl;


struct LegendMesh
{
	static void Init()
	{
		static const NodeVertex vertices[4] =
		{
			{ {-1.0f, -1.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, -1.0f, 0.0f }, 0.0f, { 0.0f, 0.0f, 0.0f } },
			{ { 1.0f,  1.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f } },
			{ {-1.0f,  1.0f, 0.0f }, 1.0f, { 0.0f, 0.0f, 0.0f } },
		};
		static const uint16_t indices[6] =
		{
			0, 1, 2, 0, 2, 3
		};
		vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(vertices)), NodeVertex::ms_decl);
		ibh = bgfx::createIndexBuffer(bgfx::makeRef(indices, sizeof(indices)));
	};

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};


struct SignVertex
{
	float m_pos[3];
	static void init()
	{
		ms_decl
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();
	};

	static bgfx::VertexDecl ms_decl;
};

bgfx::VertexDecl SignVertex::ms_decl;

struct LoadSign
{
	static void Init()
	{
		static const SignVertex vertices[6] =
		{
			{ { 0.0f, 0.0f, 0.0f } },
			{ { 1.0f, 0.0f, 0.0f } },
			{ { 0.7f, 0.1f, 0.0f }},
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

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};

struct ConstSign
{
	static void Init()
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

	static bgfx::VertexBufferHandle vbh;
	static bgfx::IndexBufferHandle ibh;
};

bgfx::VertexBufferHandle LegendMesh::vbh;
bgfx::IndexBufferHandle LegendMesh::ibh;

bgfx::VertexBufferHandle LoadSign::vbh;
bgfx::IndexBufferHandle LoadSign::ibh;

bgfx::VertexBufferHandle ConstSign::vbh;
bgfx::IndexBufferHandle ConstSign::ibh;

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
	m_renderNodesNumbers = false;
	m_renderElementsNumbers = false;
	m_renderUndeformedMesh = true;
	m_alphaUndeformedMesh = 0.3f;
	m_deformScale = 50.0f;
	m_meshesList.m_vbh.idx = -1;
	m_min = 0.0f;
	m_max = 1.0f;
	m_intervals = 10.0f;
	m_needResize = false;
	
	NodeVertex::init();
	u_percantage = bgfx::createUniform("u_percantage", bgfx::UniformType::Vec4);
	u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
	u_deformScale = bgfx::createUniform("u_deformScale", bgfx::UniformType::Vec4);
	u_rangesIntervals = bgfx::createUniform("u_rangesIntervals", bgfx::UniformType::Vec4);
	
	CreateMaterial(vs_meshPlot_h, sizeof(vs_meshPlot_h), fs_meshPlot_h, sizeof(fs_meshPlot_h));

	LegendMesh::Init();
	LoadSign::Init();
	ConstSign::Init();
	CreateMaterial(vs_sign_h, sizeof(vs_sign_h), fs_sign_h, sizeof(fs_sign_h));
}

void MeshRenderer::SetNodes(
	const StrideDataFixedArray& nodes,
	const Eigen::VectorXf& deforms,
	const std::vector<float> values, 
	bool update)
{
	m_max_bound = Eigen::Vector3f(FLT_MIN, FLT_MIN, FLT_MIN);
	m_min_bound = Eigen::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
	int dof = nodes.GetDof();
	int nodesCount = nodes.GetCount();
	NodeVertex* vertices = new NodeVertex[nodesCount];

	m_meshesList.m_hasDeforms = deforms.size() == nodesCount * dof;
	m_meshesList.m_hasValues = values.size() == nodesCount;

	for (int i = 0; i < nodesCount; ++i)
	{
		vertices[i].m_val = 0;
		for (int j = 0; j < 3; ++j)
		{
			vertices[i].m_pos[j] = 0;
			vertices[i].m_def[j] = 0;
		}
		for (int j = 0; j < dof; ++j)
		{
			vertices[i].m_pos[j] = nodes(i, j);
			if (m_max_bound[j] < nodes(i, j)){ m_max_bound[j] = nodes(i, j); }
			if (m_min_bound[j] > nodes(i, j)){ m_min_bound[j] = nodes(i, j); }
		}
	}
	if (m_meshesList.m_hasDeforms)
	{
		for (int i = 0; i < nodesCount; ++i)
		{
			for (int j = 0; j < dof; ++j)
			{
				vertices[i].m_def[j] = deforms(dof * i + j);
			}
		}
	}
	if (m_meshesList.m_hasValues)
	{
		for (int i = 0; i < nodesCount; ++i)
		{
			vertices[i].m_val = values[i];
		}
	}
	m_meshesList.m_nodesDeformation = deforms;
	nodes.MakeCopy(m_meshesList.m_nodesCopy);
	m_meshesList.m_hasDeforms = (deforms.size() == nodesCount * dof);

	m_meshesList.m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(NodeVertex) * nodesCount), NodeVertex::ms_decl);
	//delete vertices;
	m_needResize = !update;
}

void MeshRenderer::PushMesh(	const ElementsContainer& elements, int elementStart, int elementEnd,
								const IndexedStrideDataFixedArray& nodalForceList,
								const NodalConstraintList& nodalConstraintList)
{
	FemMesh m;
	int dof = m_meshesList.m_nodesCopy.GetDof();
	int nodesCount = m_meshesList.m_nodesCopy.GetCount();
	if (elements.GetCount() < elementEnd)
	{
		return;
	}
	int elementsCount = elementEnd - elementStart;
		
	m.m_nodalConstraintList = nodalConstraintList;
	
	m.m_elementsCenters = new StrideDataFixedArray;
	m.m_nodalForceList = new IndexedStrideDataFixedArray;

	nodalForceList.MakeCopy(*m.m_nodalForceList);

	m.m_elementsCenters->Init(dof, elementEnd - elementStart);
	{
		std::vector<uint32_t>* indices = new std::vector<uint32_t>;
		for (int i = 0; i < elementsCount; ++i)
		{
			std::vector<int> ind = elements(i + elementStart)->GetIndices();
			if (ind.size() == 3)
			{
				indices->push_back(ind[0]);
				indices->push_back(ind[1]);
				indices->push_back(ind[2]);
			}			
			if (ind.size() == 4)
			{
				indices->push_back(ind[0]);
				indices->push_back(ind[1]);
				indices->push_back(ind[3]);
				indices->push_back(ind[1]);
				indices->push_back(ind[2]);
				indices->push_back(ind[3]);
			}
			//(*m.m_elementsCenters)(i, 0) = (m_meshesList.m_nodesCopy(ind[0], 0) + (m_meshesList.m_nodesCopy)(ind[1], 0) + (m_meshesList.m_nodesCopy)(ind[2], 0)) / 3.0f;
			//(*m.m_elementsCenters)(i, 1) = (m_meshesList.m_nodesCopy(ind[0], 1) + (m_meshesList.m_nodesCopy)(ind[1], 1) + (m_meshesList.m_nodesCopy)(ind[2], 1)) / 3.0f;
		}
		m.m_ibhTriang = bgfx::createIndexBuffer(bgfx::makeRef(&(*indices)[0], sizeof(uint32_t) * indices->size()), BGFX_BUFFER_INDEX32);
		//delete indices;
	}
	{
		std::vector<uint32_t>* indices = new std::vector<uint32_t>;
		for (int i = 0; i < elementsCount; ++i)
		{
			std::vector<int> ind = elements(i + elementStart)->GetIndices();			
			if (ind.size() == 3)
			{
				indices->push_back(ind[0]);
				indices->push_back(ind[1]);
				indices->push_back(ind[1]);
				indices->push_back(ind[2]);
				indices->push_back(ind[2]);
				indices->push_back(ind[0]);
			}
			if (ind.size() == 4)
			{
				indices->push_back(ind[0]);
				indices->push_back(ind[1]);
				indices->push_back(ind[1]);
				indices->push_back(ind[2]);
				indices->push_back(ind[2]);
				indices->push_back(ind[3]);
				indices->push_back(ind[3]);
				indices->push_back(ind[0]);
			}
		}
		m.m_ibhLines = bgfx::createIndexBuffer(bgfx::makeRef(&(*indices)[0], sizeof(uint32_t) * indices->size()), BGFX_BUFFER_INDEX32);
		//delete indices;
	}
	m_meshesList.m_meshes.push_back(m);
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
	int width = 300;
	vpos.x = static_cast<float>(m_width - width);
	vpos.y = 21;
	ImGuiSetCond_ cond = force ? ImGuiSetCond_Always : ImGuiSetCond_Appearing;
	ImGui::SetWindowPos(meshViewOptionTitle.c_str(), vpos, cond);
	ImVec2 vsize;
	vsize.x = static_cast<float>(width);
	vsize.y = 300;
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

void MeshRenderer::Render(int width, int height, Camera& camera)
{
#ifdef WIN32
	_set_output_format(_TWO_DIGIT_EXPONENT);
#endif

	if (m_needResize)
	{
		float m_position[2];
		m_position[0] = (m_max_bound.x() + m_min_bound.x()) / 2.0f;
		m_position[1] = (m_max_bound.y() + m_min_bound.y()) / 2.0f;
		float fovx = m_max_bound.x() - m_min_bound.x();
		float fovy = m_max_bound.y() - m_min_bound.y();
		float fov = fovx > fovy ? fovx : fovy;
		camera.SetPosition(m_position);
		camera.SetFOV(fov * 1.5f);
		m_needResize = false;
	}

	m_width = width;
	Eigen::Vector4f percantage(1.0,0,0,0);
	bgfx::setUniform(u_percantage, &percantage, 1);
	
	if (m_showMeshViewOption)
	{
		if (ImGui::Begin(meshViewOptionTitle.c_str(), &m_showMeshViewOption))
		{
			ImGui::Text("Mesh and lables");
			ImGui::Checkbox("Render wireframe", &m_renderWireframe);
			ImGui::Checkbox("Render nodes", &m_renderNodes);
			ImGui::Checkbox("Render elements lables", &m_renderElementsNumbers);
			ImGui::Checkbox("Render nodes lables", &m_renderNodesNumbers);
			ImGui::Separator();
			ImGui::Text("Undeformed mesh");
			ImGui::Checkbox("Render undeformed mesh", &m_renderUndeformedMesh);
			ImGui::PushItemWidth(100);
			ImGui::SliderFloat("Undeformed mesh\n transparency", &m_alphaUndeformedMesh, 0.0f, 1.0f);
			ImGui::PopItemWidth();
			ImGui::Separator();
			ImGui::Text("Deformations");
			ImGui::PushItemWidth(280);
			ImGui::SliderFloat("", &m_deformScale, 0.0f, 1000.0f, "Deformations x%.0f", 3.0f);
			ImGui::PopItemWidth();
			ResetMeshViewOptionsWindowPosition(false);
		}
		ImGui::End();
	}

	Eigen::Vector4f deformScale(m_deformScale, m_deformScale, m_deformScale, m_deformScale);
	bgfx::setUniform(u_deformScale, deformScale.data(), 1);

	Eigen::Vector4f rangesIntervals(m_min, m_max, m_intervals - 1.0f, 0.0f);
	bgfx::setUniform(u_rangesIntervals, rangesIntervals.data(), 1);

	for (int i = 0, l = m_meshesList.m_meshes.size(); i < l; ++i)
	{
		RenderMesh(m_meshesList.m_meshes[i], 1.0f, 0);
	}

	deformScale.setZero();
	bgfx::setUniform(u_deformScale, deformScale.data(), 1);

	if (m_renderUndeformedMesh && m_meshesList.m_hasDeforms)
	{
		for (int i = 0, l = m_meshesList.m_meshes.size(); i < l; ++i)
		{
			RenderMesh(m_meshesList.m_meshes[i], m_alphaUndeformedMesh, BGFX_STATE_BLEND_EQUATION_ADD | BGFX_STATE_BLEND_ALPHA, true);
		}
	}

	{
		float view[16] = {1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f};
		float ortho[16];

		float BarTop = 1.05f;
		float BarHeight = 0.9f;
		float BarWidth = 0.1f;
		float BarLeft = 0.02f;

		float b = (4.0f / BarHeight* BarTop - 8.0f / BarHeight + 2.0f)*0.5;
		float t = 4.0f / BarHeight + b;
		float l = (4.0f / BarWidth* (2.0 - BarLeft - BarWidth) - 8.0f / BarWidth + 2.0f)*0.5;
		float r = 4.0f / BarWidth + l;
		bx::mtxOrtho(ortho, 
			l, r, 
			b,
			t,
			-2.0f, 2.0f);
		bgfx::setViewTransform(1, view, ortho);
		bgfx::setViewRect(1, 0, 0, width, height);
		
		for (int i = 0; i <= m_intervals; i++)
		{
			char label[1024];
			sprintf(label, "%.3e", m_max - i * (m_max - m_min) / m_intervals);
			float x = (BarLeft + BarWidth + 0.01f) / 2 * width;
			float y = (BarTop + BarHeight / m_intervals * i + 0.01f) / 2 * height;
			imguiDrawText(static_cast<int>(x), static_cast<int>(y), ImguiTextAlign::Left, label, 0xFFFFFFFF);
		}

		Eigen::Vector4f percantage(0.0, 0, 0, 0);
		bgfx::setUniform(u_percantage, &percantage, 1);
		bgfx::setVertexBuffer(LegendMesh::vbh);
		bgfx::setIndexBuffer(LegendMesh::ibh);
		bgfx::setState(BGFX_STATE_RGB_WRITE);
		bgfx::submit(1, m_programs[0]);
		percantage.x() = 1.0f;
		bgfx::setUniform(u_percantage, &percantage, 1);
		camera.UpplyTransform();
	}

	for (int i = 0, l = m_meshesList.m_meshes.size(); i < l; ++i)
	{
		if (m_renderElementsNumbers)
		{
			RenderElementsNumbers(m_meshesList.m_meshes[i], width, height, camera);
		}
		RenderBC(m_meshesList.m_meshes[i]);
	}

	if (m_renderNodesNumbers)
	{
		RenderNodesNumbers(width, height, camera);
	}
}

void MeshRenderer::Clear()
{
	if (m_meshesList.m_vbh.idx != uint16_t(- 1) )
	{
		bgfx::destroyVertexBuffer(m_meshesList.m_vbh);
		m_meshesList.m_vbh.idx = -1;
	}
	m_meshesList.m_nodesCopy.Init(0, 0);
	for (std::vector<FemMesh>::iterator it = m_meshesList.m_meshes.begin(); it != m_meshesList.m_meshes.end(); ++it)
	{
		bgfx::destroyIndexBuffer(it->m_ibhTriang);
		bgfx::destroyIndexBuffer(it->m_ibhLines);
		delete it->m_elementsCenters;
	}
	m_meshesList.m_meshes.clear();
}

void MeshRenderer::RenderMesh(const FemMesh& mesh, float alpha, uint64_t _state, bool grey)
{
	Eigen::Vector4f color(0.1, 0.6, 0.3, alpha);
	if (grey) { color.x() *= 0.5; color.y() *= 0.5; color.z() *= 0.5; }
	bgfx::setUniform(u_color, color.data(), 1);
	
	if (!grey)
	{
		Eigen::Vector4f percantage(0.0, 0, 0, 0);
		bgfx::setUniform(u_percantage, &percantage, 1);
	}

	bgfx::setVertexBuffer(m_meshesList.m_vbh);
	bgfx::setIndexBuffer(mesh.m_ibhTriang);
	bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | _state);
	bgfx::submit(0, m_programs[0]);

	if (!grey)
	{
		Eigen::Vector4f percantage(1.0, 0, 0, 0);
		bgfx::setUniform(u_percantage, &percantage, 1);
	}

	if (m_renderWireframe)
	{
		Eigen::Vector4f color(0.0, 0.0, 0.0, alpha);
		//if (grey) { color.x() *= 0.5; color.y() *= 0.5; color.z() *= 0.5; }
		bgfx::setUniform(u_color, color.data(), 1);

		bgfx::setVertexBuffer(m_meshesList.m_vbh);
		bgfx::setIndexBuffer(mesh.m_ibhLines);
		bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_LINES | _state);
		bgfx::submit(0, m_programs[0]);
	}
	if (m_renderNodes)
	{
		Eigen::Vector4f color(0.8, 0.6, 0.2, alpha);
		bgfx::setUniform(u_color, color.data(), 1);

		bgfx::setVertexBuffer(m_meshesList.m_vbh);
		bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_POINTS | BGFX_STATE_POINT_SIZE(3) | _state);
		bgfx::submit(0, m_programs[0]);
	}
}

void MeshRenderer::RenderElementsNumbers(const FemMesh& mesh, int width, int height, const Camera& camera)
{
	for (int j = 0; j < mesh.m_elementsCenters->GetCount(); ++j)
	{
		float x, y;
		TransformToWorldSpace(
			(*mesh.m_elementsCenters)(j, 0),
			(*mesh.m_elementsCenters)(j, 1),
			0.0f, width, height, camera, x, y);
	char label[32];
	sprintf(label, "%d", j);
	imguiDrawText(static_cast<int>(x), static_cast<int>(y), ImguiTextAlign::Right, label, 0xFFFFFF00);
	}
}

void MeshRenderer::RenderNodesNumbers(int width, int height, const Camera& camera)
{
	int dof = m_meshesList.m_nodesCopy.GetDof();
	for (int j = 0; j < m_meshesList.m_nodesCopy.GetCount(); ++j)
	{
		float x, y;
		if (m_meshesList.m_hasDeforms)
		{
			TransformToWorldSpace(
				m_meshesList.m_nodesCopy(j, 0) + m_meshesList.m_nodesDeformation[dof * j + 0] * m_deformScale,
				m_meshesList.m_nodesCopy(j, 1) + m_meshesList.m_nodesDeformation[dof * j + 1] * m_deformScale,
				dof == 3 ? (m_meshesList.m_nodesCopy(j, 2) + m_meshesList.m_nodesDeformation[dof * j + 2] * m_deformScale) : 0.0f,
				width, height, camera, x, y);
		}
		else
		{
			TransformToWorldSpace(
				m_meshesList.m_nodesCopy(j, 0),
				m_meshesList.m_nodesCopy(j, 1),
				dof == 3 ? m_meshesList.m_nodesCopy(j, 2) : 0.0f,
				width, height, camera, x, y);
		}
		char label[32];
		sprintf(label, "%d", j);
		imguiDrawText(static_cast<int>(x), static_cast<int>(y), ImguiTextAlign::Right, label, 0xFF00FFFF);
	}
}

void MeshRenderer::RenderBC(const FemMesh& mesh)
{
	int dof = m_meshesList.m_nodesCopy.GetDof();
	{
		Eigen::Vector4f color(1.0, 0.0, 0.0, 1.0);
		bgfx::setUniform(u_color, color.data(), 1);
	}
	for (int j = 0; j < mesh.m_nodalForceList->GetCount(); ++j)
	{
		int n = (*mesh.m_nodalForceList)[j];
		float direction[3];
		direction[0] = (*mesh.m_nodalForceList)(j, 0);
		direction[1] = (*mesh.m_nodalForceList)(j, 1);
		direction[2] = dof == 3 ? ((*mesh.m_nodalForceList)(j, 2)): 0.0f;
		RenderSign(mesh, n, LoadSign::vbh, LoadSign::ibh, direction);
	}
	{
		Eigen::Vector4f color(1.0, 1.0, 1.0, 1.0);
		bgfx::setUniform(u_color, color.data(), 1);
	}
	for (NodalConstraintList::const_iterator it = mesh.m_nodalConstraintList.begin(); it != mesh.m_nodalConstraintList.end(); ++it)
	{
		int n = it->node;
		for (int i = 0; i < 3; ++i)
		{
			if (it->type & i)
			{
				float direction[3] = { 0, 0, 0 };
				switch (it->type & i)
				{
				case NodalConstraint::UX:
					direction[0] = 1;
					break;
				case NodalConstraint::UY:
					direction[1] = 1;
					break;
				case NodalConstraint::UZ:
					direction[2] = 1;
					break;
				}
				RenderSign(mesh, n, ConstSign::vbh, ConstSign::ibh, direction);
			}
		}
	}
}

void MeshRenderer::RenderSign(const FemMesh& mesh, int node, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, float direction[3])
{
	int dof = m_meshesList.m_nodesCopy.GetDof();
	float xp;
	float yp;
	float zp;
	if (m_meshesList.m_hasDeforms)
	{
		xp = m_meshesList.m_nodesCopy(node, 0) + m_meshesList.m_nodesDeformation[dof * node + 0] * m_deformScale;
		yp = m_meshesList.m_nodesCopy(node, 1) + m_meshesList.m_nodesDeformation[dof * node + 1] * m_deformScale;
		zp = dof == 3 ? (m_meshesList.m_nodesCopy(node, 2) + m_meshesList.m_nodesDeformation[dof * node + 2] * m_deformScale) : 0.0f;

	}
	else
	{
		xp = m_meshesList.m_nodesCopy(node, 0);
		yp = m_meshesList.m_nodesCopy(node, 1);
		zp = dof == 3 ? (m_meshesList.m_nodesCopy(node, 2)) : 0.0f;
	}

	glm::mat4 translation = glm::translate(glm::vec3(xp, yp, zp));

	glm::vec3 loadDirection = glm::normalize(glm::vec3(direction[0], direction[1], direction[2]));

	glm::quat q;
	glm::vec3 a = glm::cross(glm::vec3(1.0, 0.0, 0.0), loadDirection);
	q.x = a.x;
	q.y = a.y;
	q.z = a.z;
	q.w = sqrt(1.0f + loadDirection.x);
	q = glm::normalize(q);
	glm::mat4 rotation = glm::mat4(q);

	bgfx::setVertexBuffer(vbh);
	bgfx::setIndexBuffer(ibh);
	glm::mat4 transform = translation* rotation;
	bgfx::setTransform(&transform);

	bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_LINES);
	bgfx::submit(0, m_programs[1]);
}

void MeshRenderer::SetRanges(float min, float max)
{
	m_min = min;
	m_max = max;
	if (m_max - m_min < (m_min + m_max) / 2.0f * 1e-3f)
	{
		m_max = m_min + (m_min + m_max) / 2.0f * 1e-3f;
	}
}

void MeshRenderer::SetIntervals(int intervals)
{
	m_intervals = intervals;
}