#pragma once
#include <bgfx.h>
#include <vector>
#include <Eigen/Dense>

#include "StrideDataFixedArray.h"
#include "Load.h"
#include "Camera.h"

class ElementsContainer;

class MeshRenderer
{
public:
	enum Material
	{
		MT_Colored = 0,
		MT_Plot,
	};

	void Init();

	
	void Clear();
	void Render(int width, int height, Camera& camera);
	void ShowMeshViewOptions();
	void ResetMeshViewOptionsWindowPosition(bool force);
	void SetRanges(float min, float max);
	void SetIntervals(int intervals);
	void ResetNeedForResize(){m_needResize = false;};
	
	class Mesh
	{
	public:
		struct Object
		{
			bgfx::IndexBufferHandle			m_ibhTriang;
			bgfx::IndexBufferHandle			m_ibhLines;
			StrideDataFixedArray*			m_elementsCenters;
			IndexedStrideDataFixedArray*	m_nodalForceList;
			NodalConstraintList				m_nodalConstraintList;
		};
		Mesh();
		void SetNodes(const StrideDataFixedArray& nodes,
			const Eigen::VectorXf& deforms,
			const std::vector<float> values,
			bool update = false);
		void PushMesh(const ElementsContainer& elements, int elementStart, int elementEnd,
			const IndexedStrideDataFixedArray& nodalForceList,
			const NodalConstraintList& nodalConstraintList);
		void Render(int width, int height, Camera& camera);
	private:
		void RenderObject(const Object& mesh, float alpha, uint64_t _state, bool grey = false);
		std::vector<Object>			m_objectList;
		bgfx::VertexBufferHandle	m_vbh;
		StrideDataFixedArray		m_nodesCopy;
		Eigen::VectorXf				m_nodesDeformation;
		bool						m_hasDeforms;
		bool						m_hasValues;
	};

private:

	void CreateMaterial(const uint8_t* v, uint32_t sizev, const uint8_t* f, uint32_t sizef);
	void RenderElementsNumbers(const Mesh::Object& mesh, int width, int height, const Camera& camera);
	void RenderNodesNumbers(int width, int height, const Camera& camera);

	void RenderBC(const Mesh::Object& mesh);
	void RenderSign(const Mesh::Object& mesh, int node, bgfx::VertexBufferHandle, bgfx::IndexBufferHandle, float direction[3]);

	Mesh m_meshesList;
	std::vector<bgfx::ProgramHandle> m_programs;
	bgfx::UniformHandle u_percantage;
	bgfx::UniformHandle u_color;
	bgfx::UniformHandle u_deformScale;
	bgfx::UniformHandle u_rangesIntervals;

	bool m_renderWireframe;
	bool m_renderNodes;
	bool m_showMeshViewOption;
	bool m_renderNodesNumbers;
	bool m_renderElementsNumbers;
	bool m_renderUndeformedMesh;
	float m_alphaUndeformedMesh;
	int m_width;

	float m_deformScale;
	float m_min;
	float m_max;
	float m_intervals;


	Eigen::Vector3f m_max_bound;
	Eigen::Vector3f m_min_bound;
	bool m_needResize;
};