#pragma once
#include <vector>
#include <bgfx.h>
#include <Eigen/Dense>

class MaterialManager
{
public:
	void Init();

	void CreateDefaultMaterials();

	void CreateMaterial(const uint8_t* v, uint32_t sizev, const uint8_t* f, uint32_t sizef);

	void SetRenderPlotEnabled(bool enabled);

	void SetCurrentRenderColor(const Eigen::Vector4f& color);

	void SubmitMeshReder(int view);

	void SubmitSignRender(int view);

	void SetDeformsScale(float scale);

	void SetIntervals(float min, float max, float intervals);

private:
	std::vector<bgfx::ProgramHandle> m_programs;
	bgfx::UniformHandle u_percantage;
	bgfx::UniformHandle u_color;
	bgfx::UniformHandle u_deformScale;
	bgfx::UniformHandle u_rangesIntervals;
};