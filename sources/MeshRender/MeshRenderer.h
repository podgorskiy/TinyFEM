#pragma once
#include <bgfx.h>
#include <vector>

#include "StrideDataFixedArray.h"
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
	void AddMesh(const StrideDataFixedArray& nodes, const ElementsContainer& elements, const StrideDataFixedArray& values);
	void Clear();
	void Render(int width, int height, const Camera& camera);
	void ShowMeshViewOptions();
	void ResetMeshViewOptionsWindowPosition(bool force);

private:

	struct Mesh
	{
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibhTriang;
		bgfx::IndexBufferHandle m_ibhLines;
		StrideDataFixedArray* nodesCopy;
		StrideDataFixedArray* elementsCenters;
		Material m_mat;
	};

	void CreateMaterial(const uint8_t* v, uint32_t sizev, const uint8_t* f, uint32_t sizef);

	std::vector<bgfx::ProgramHandle> m_programs;
	std::vector<Mesh> m_meshes;
	bgfx::UniformHandle u_percantage;
	bgfx::UniformHandle u_color;

	bool m_renderWireframe;
	bool m_renderNodes;
	bool m_showMeshViewOption;
	bool m_renderNodesNumbers;
	bool m_renderElementsNumbers;
	int m_width;
};