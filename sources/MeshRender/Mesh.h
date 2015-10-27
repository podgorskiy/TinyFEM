#pragma once
#include <bgfx.h>
#include <vector>
#include <Eigen/Dense>

#include "StrideDataFixedArray.h"
#include "Load.h"
#include "RenderContext.h"

class ElementsContainer;
class Camera;

class FemMesh
{
private:
	FemMesh(const FemMesh& other){};
	void operator= (const FemMesh& other){};
public:
	FemMesh();

	/* Generates vertices for the mesh. */
	void SetNodes(const StrideDataArray& nodes,
		const Eigen::VectorXf& deforms,
		const std::vector<float> values);

	/* Allows update nodes data without rebuilding indices. */
	void UpdateNodes(const StrideDataArray& nodes,
		const Eigen::VectorXf& deforms,
		const std::vector<float> values);

	/* Pushes object that defined in ElementsContainer. Generates indices for rendering that object. */
	void PushObject(const ElementsContainer& elements,
		const IndexedStrideDataArray& nodalForceList,
		const NodalConstraintList& nodalConstraintList);

	/* Renders mesh. */
	void Render(const RenderContext& cntxt);

	bool HasDeforms();

private:
	struct Object
	{
		bgfx::IndexBufferHandle			m_ibhTriang;
		bgfx::IndexBufferHandle			m_ibhLines;
		StrideDataArray					m_elementsCenters;
		IndexedStrideDataArray			m_nodalForceList;
		NodalConstraintList				m_nodalConstraintList;
	};

	void RenderObject(const RenderContext& cntxt, const Object& mesh);

	std::vector<Object*>		m_objectList;
	
	bgfx::VertexBufferHandle	m_vbh;
	
	StrideDataArray				m_nodesCopy;
	
	Eigen::VectorXf				m_nodesDeformation;
	bool						m_hasDeforms;
	bool						m_hasValues;
	Eigen::Vector3f				m_max_bound;
	Eigen::Vector3f				m_min_bound;
};
