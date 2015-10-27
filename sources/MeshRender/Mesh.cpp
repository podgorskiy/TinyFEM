#include "Mesh.h"
#include "VertexDeclarations.h"
#include "Elements/ElementsContainer.h"
#include "MaterialManager.h"

void FemMesh::UpdateNodes(const StrideDataArray& nodes,
	const Eigen::VectorXf& deforms,
	const std::vector<float> values)
{
	SetNodes(nodes, deforms, values);
}

void FemMesh::SetNodes(
	const StrideDataArray& nodes,
	const Eigen::VectorXf& deforms,
	const std::vector<float> values)
{
	m_max_bound = Eigen::Vector3f(FLT_MIN, FLT_MIN, FLT_MIN);
	m_min_bound = Eigen::Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
	int dof = nodes.GetDof();
	int nodesCount = nodes.GetCount();
	NodeVertex* vertices = new NodeVertex[nodesCount];

	m_hasDeforms = deforms.size() == nodesCount * dof;
	m_hasValues = values.size() == nodesCount;

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
	if (m_hasDeforms)
	{
		for (int i = 0; i < nodesCount; ++i)
		{
			for (int j = 0; j < dof; ++j)
			{
				vertices[i].m_def[j] = deforms(dof * i + j);
			}
		}
	}
	if (m_hasValues)
	{
		for (int i = 0; i < nodesCount; ++i)
		{
			vertices[i].m_val = values[i];
		}
	}
	m_nodesDeformation = deforms;
	nodes.MakeCopy(m_nodesCopy);
	m_hasDeforms = (deforms.size() == nodesCount * dof);

	m_vbh = bgfx::createVertexBuffer(bgfx::makeRef(vertices, sizeof(NodeVertex) * nodesCount), NodeVertex::ms_decl);

	//m_needResize = !update;
}

void FemMesh::PushObject(
	const ElementsContainer& elements,
	const IndexedStrideDataArray& nodalForceList,
	const NodalConstraintList& nodalConstraintList)
{
	Object* m_ = new Object();
	m_objectList.push_back(m_);
	Object& m = *m_;

	int dof = m_nodesCopy.GetDof();
	int nodesCount = m_nodesCopy.GetCount();
	int elementsCount = elements.GetCount();

	m.m_nodalConstraintList = nodalConstraintList;
	
	nodalForceList.MakeCopy(m.m_nodalForceList);

	m.m_elementsCenters.Init(dof, elementsCount);
	{
		std::vector<uint32_t>* indices = new std::vector<uint32_t>;
		for (int i = 0; i < elementsCount; ++i)
		{
			std::vector<int> ind = elements(i)->GetIndices();
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
			std::vector<int> ind = elements(i)->GetIndices();
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
}

void FemMesh::Render(const RenderContext& cntxt)
{
	for (int i = 0, l = m_objectList.size(); i < l; ++i)
	{
		RenderObject(cntxt, *m_objectList[i]);
	}
}


void FemMesh::RenderObject(const RenderContext& cntxt, const Object& mesh)
{
	Eigen::Vector4f color(0.1, 0.6, 0.3, cntxt.m_alpha);
	if (cntxt.m_greyMode) { color.x() *= 0.5; color.y() *= 0.5; color.z() *= 0.5; }

	cntxt.m_materialManager->SetCurrentRenderColor(color);

	if (!cntxt.m_greyMode)
	{
		cntxt.m_materialManager->SetRenderPlotEnabled(true);
	}

	bgfx::setVertexBuffer(m_vbh);
	bgfx::setIndexBuffer(mesh.m_ibhTriang);
	bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | cntxt.m_state);
	cntxt.m_materialManager->SubmitMeshReder(0);

	if (!cntxt.m_greyMode)
	{
		cntxt.m_materialManager->SetRenderPlotEnabled(false);
	}

	if (cntxt.m_renderWireframe)
	{
		Eigen::Vector4f color(0.0, 0.0, 0.0, cntxt.m_alpha);
		cntxt.m_materialManager->SetCurrentRenderColor(color);

		bgfx::setVertexBuffer(m_vbh);
		bgfx::setIndexBuffer(mesh.m_ibhLines);
		bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_LINES | cntxt.m_state);
		cntxt.m_materialManager->SubmitMeshReder(0);
	}
	if (cntxt.m_renderNodes)
	{
		Eigen::Vector4f color(0.8, 0.6, 0.2, cntxt.m_alpha);
		cntxt.m_materialManager->SetCurrentRenderColor(color);

		bgfx::setVertexBuffer(m_vbh);
		bgfx::setState(BGFX_STATE_RGB_WRITE | BGFX_STATE_MSAA | BGFX_STATE_PT_POINTS | BGFX_STATE_POINT_SIZE(3) | cntxt.m_state);
		cntxt.m_materialManager->SubmitMeshReder(0);
	}
}

bool FemMesh::HasDeforms()
{
	return m_hasDeforms;
}

FemMesh::FemMesh()
{
	m_vbh.idx = -1;
}

/*
void MeshRenderer::RenderSign(const Object& mesh, int node, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, float direction[3])
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
*/