#include "MaterialManager.h"

#include "../shaders/fs_meshPlot.h"
#include "../shaders/vs_meshPlot.h"
#include "../shaders/fs_sign.h"
#include "../shaders/vs_sign.h"

#include <Eigen/Dense>

void MaterialManager::Init()
{
	u_percantage = bgfx::createUniform("u_percantage", bgfx::UniformType::Vec4);
	u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
	u_deformScale = bgfx::createUniform("u_deformScale", bgfx::UniformType::Vec4);
	u_rangesIntervals = bgfx::createUniform("u_rangesIntervals", bgfx::UniformType::Vec4);
}

void MaterialManager::CreateMaterial(const uint8_t* v, uint32_t sizev, const uint8_t* f, uint32_t sizef)
{
	const bgfx::Memory* vs = bgfx::makeRef(v, sizev);
	const bgfx::Memory* fs = bgfx::makeRef(f, sizef);
	bgfx::ShaderHandle vsh = bgfx::createShader(vs);
	bgfx::ShaderHandle fsh = bgfx::createShader(fs);
	m_programs.push_back(bgfx::createProgram(vsh, fsh));
}

void MaterialManager::CreateDefaultMaterials()
{
	CreateMaterial(vs_meshPlot_h, sizeof(vs_meshPlot_h), fs_meshPlot_h, sizeof(fs_meshPlot_h));
	CreateMaterial(vs_sign_h, sizeof(vs_sign_h), fs_sign_h, sizeof(fs_sign_h));
}

void MaterialManager::SetRenderPlotEnabled(bool enabled)
{
	Eigen::Vector4f percantage(
		enabled ? 0.0f : 1.0f,
		0, 0, 0);
	bgfx::setUniform(u_percantage, &percantage, 1);
}

void MaterialManager::SetCurrentRenderColor(const Eigen::Vector4f& color)
{
	bgfx::setUniform(u_color, color.data(), 1);
}

void MaterialManager::SubmitMeshReder(int view)
{
	bgfx::submit(view, m_programs[0]);
}

void MaterialManager::SubmitSignRender(int view)
{
	bgfx::submit(view, m_programs[1]);
}

void MaterialManager::SetDeformsScale(float scale)
{
	Eigen::Vector4f deformScale(scale, scale, scale, scale);
	bgfx::setUniform(u_deformScale, deformScale.data(), 1);
}

void MaterialManager::SetIntervals(float min, float max, float intervals)
{
	Eigen::Vector4f rangesIntervals(min, max, intervals - 1.0f, 0.0f);
	bgfx::setUniform(u_rangesIntervals, rangesIntervals.data(), 1);
}