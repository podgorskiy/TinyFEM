#include "MeshRenderer.h"
#include <bgfx.h>
#include <bx/timer.h>

#include "StrideDataFixedArray.h"
#include "Elements/ElementsContainer.h"
#include "Elements/IElement.h"

#include <ocornut-imgui/imgui.h>
#include <imgui/imgui.h>

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Signs.h"
#include "VertexDeclarations.h"

#include "TransformUtils.h"

#include "MaterialManager.h"

#include "Mesh.h"

#include "ScaleBar.h"

std::string meshViewOptionTitle = "Mesh view options";


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
	m_min = 0.0f;
	m_max = 1.0f;
	m_intervals = 10.0f;
	m_needResize = false;
	
	NodeVertex::init();
	LegendMesh::Init();
	LoadSign::Init();
	ConstSign::Init();

	m_materialManager.Init();
	m_materialManager.CreateDefaultMaterials();
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


void MeshRenderer::Render(int width, int height, Camera& camera)
{
	RenderContext cntxt;
	cntxt.m_camera = &camera;
	cntxt.m_height = height;
	cntxt.m_width = width;
	cntxt.m_materialManager = &m_materialManager;
	cntxt.m_renderNodes = m_renderNodes;
	cntxt.m_renderWireframe = m_renderWireframe;

	m_width = width;

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

	m_materialManager.SetIntervals(m_min, m_max, m_intervals);
	m_materialManager.SetRenderPlotEnabled(false);

	m_materialManager.SetDeformsScale(m_deformScale);

	cntxt.m_alpha = 1.0f;
	cntxt.m_greyMode = false;
	cntxt.m_state = 0;

	for (int i = 0, l = m_meshesList.size(); i < l; ++i)
	{
		m_meshesList[i].Render(cntxt);
	}

	m_materialManager.SetDeformsScale(0);

	cntxt.m_alpha = m_alphaUndeformedMesh;
	cntxt.m_greyMode = true;
	cntxt.m_state = BGFX_STATE_BLEND_EQUATION_ADD | BGFX_STATE_BLEND_ALPHA;

	if (m_renderUndeformedMesh)
	{
		for (int i = 0, l = m_meshesList.size(); i < l; ++i)
		{
			if (m_meshesList[i].HasDeforms())
			{
				m_meshesList[i].Render(cntxt);
			}
		}
	}

	cntxt.m_alpha = 1.0f;
	cntxt.m_greyMode = false;
	cntxt.m_state = 0;
	RenderScaleBar(cntxt, m_max, m_min, m_intervals);

	/*
	for (int i = 0, l = m_meshesList.m_objectList.size(); i < l; ++i)
	{
		if (m_renderElementsNumbers)
		{
			RenderElementsNumbers(m_meshesList.m_objectList[i], width, height, camera);
		}
		RenderBC(m_meshesList.m_objectList[i]);
	}

	if (m_renderNodesNumbers)
	{
		RenderNodesNumbers(width, height, camera);
	}
	*/
}

void MeshRenderer::Clear()
{
	/*
	if (m_meshesList.m_vbh.idx != uint16_t(- 1) )
	{
		bgfx::destroyVertexBuffer(m_meshesList.m_vbh);
		m_meshesList.m_vbh.idx = -1;
	}
	m_meshesList.m_nodesCopy.Init(0, 0);
	for (std::vector<Object>::iterator it = m_meshesList.m_objectList.begin(); it != m_meshesList.m_objectList.end(); ++it)
	{
		bgfx::destroyIndexBuffer(it->m_ibhTriang);
		bgfx::destroyIndexBuffer(it->m_ibhLines);
		delete it->m_elementsCenters;
	}
	m_meshesList.m_objectList.clear();
	*/
}

/*
void MeshRenderer::RenderElementsNumbers(const Object& mesh, int width, int height, const Camera& camera)
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

void MeshRenderer::RenderBC(const Object& mesh)
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
*/


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
