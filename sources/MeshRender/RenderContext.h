#pragma once
#include <stdint.h>

class Camera;
class MaterialManager;

struct RenderContext
{
	Camera * m_camera;
	int m_width;
	int m_height;
	float m_alpha;
	uint64_t m_state;
	bool m_greyMode;
	bool m_renderWireframe;
	bool m_renderNodes;

	MaterialManager * m_materialManager;
};