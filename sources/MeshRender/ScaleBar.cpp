#include "ScaleBar.h"
#include "RenderContext.h"
#include "MaterialManager.h"
#include "Signs.h"
#include "Camera.h"

#include <bgfx.h>
#include <bx/fpumath.h>
#include <bgfx_utils.h>

#include <ocornut-imgui/imgui.h>
#include <imgui/imgui.h>

void RenderScaleBar(const RenderContext& cntxt, float max, float min, float intervals)
{
	float view[16] = { 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f, 0, 0, 0, 0, 1.0f };
	float ortho[16];

	float BarTop = 1.05f;
	float BarHeight = 0.9f;
	float BarWidth = 0.1f;
	float BarLeft = 0.02f;

	float b = (4.0f / BarHeight* BarTop - 8.0f / BarHeight + 2.0f)*0.5f;
	float t = 4.0f / BarHeight + b;
	float l = (4.0f / BarWidth* (2.0f - BarLeft - BarWidth) - 8.0f / BarWidth + 2.0f)*0.5f;
	float r = 4.0f / BarWidth + l;
	bx::mtxOrtho(ortho,
		l, r,
		b,
		t,
		-2.0f, 2.0f);
	bgfx::setViewTransform(1, view, ortho);
	bgfx::setViewRect(1, 0, 0, cntxt.m_width, cntxt.m_height);

	for (int i = 0; i <= intervals; i++)
	{
		char label[1024];
		sprintf(label, "%.3e", max - i * (max - min) / intervals);
		float x = (BarLeft + BarWidth + 0.01f) / 2 * cntxt.m_width;
		float y = (BarTop + BarHeight / intervals * i + 0.01f) / 2 * cntxt.m_height;
		imguiDrawText(static_cast<int>(x), static_cast<int>(y), ImguiTextAlign::Left, label, 0xFFFFFFFF);
	}

	cntxt.m_materialManager->SetRenderPlotEnabled(true);

	bgfx::setVertexBuffer(LegendMesh::vbh);
	bgfx::setIndexBuffer(LegendMesh::ibh);
	bgfx::setState(BGFX_STATE_RGB_WRITE);
	cntxt.m_materialManager->SubmitMeshReder(1);

	cntxt.m_materialManager->SetRenderPlotEnabled(false);
	cntxt.m_camera->UpplyTransform();
}
	