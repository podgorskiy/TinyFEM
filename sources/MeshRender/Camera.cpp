#include "Camera.h"

#include <entry/entry.h>
#include <bx/timer.h>
#include <bx/fpumath.h>

#include <entry/entry.h>
#include <bgfx_utils.h>

#include <bgfx.h>
#include <bx/timer.h>
#include <bx/fpumath.h>

Camera::Camera()
{
	Reset();
	entry::MouseState mouseState;
	Update(mouseState);
};

Camera::~Camera()
{};

void Camera::Reset()
{
	m_mouseNow.m_mx = 0;
	m_mouseNow.m_my = 0;
	m_mouseNow.m_mz = 0;
	m_mouseLast.m_mx = 0;
	m_mouseLast.m_my = 0;
	m_mouseLast.m_mz = 0;
	m_position[0] = 0;
	m_position[1] = 0;
	m_fov = 10.0f;
	m_mouseDown = false;
};

void Camera::Update(const entry::MouseState& _mouseState)
{
	if (!m_mouseDown)
	{
		m_mouseLast.m_mx = _mouseState.m_mx;
		m_mouseLast.m_my = _mouseState.m_my;
	}

	m_mouseDown = !!_mouseState.m_buttons[entry::MouseButton::Right];

	if (m_mouseDown)
	{
		m_mouseNow.m_mx = _mouseState.m_mx;
		m_mouseNow.m_my = _mouseState.m_my;
	}

	m_mouseNow.m_mz = _mouseState.m_mz;
	int32_t deltaZ = m_mouseNow.m_mz - m_mouseLast.m_mz;
	m_fov *= (1.0f + deltaZ / 20.0f);
	m_mouseLast.m_mz = m_mouseNow.m_mz;

	if (m_mouseDown)
	{
		int32_t deltaX = m_mouseNow.m_mx - m_mouseLast.m_mx;
		int32_t deltaY = m_mouseNow.m_my - m_mouseLast.m_my;
		
		m_position[0] -= float(deltaX) / m_height * m_fov;
		m_position[1] += float(deltaY) / m_height * m_fov;

		m_mouseLast.m_mx = m_mouseNow.m_mx;
		m_mouseLast.m_my = m_mouseNow.m_my;
	}
};

void Camera::UpplyTransform()
{
	float at[3] = { m_position[0], m_position[1], 0.0f };
	float eye[3] = { m_position[0], m_position[1], -1.0f };

	float view[16];
	bx::mtxLookAt(view, eye, at);

	float w, h;
	w = m_width * m_fov / m_height;
	h = m_fov;

	float ortho[16];
	bx::mtxOrtho(ortho, -w / 2, w / 2, -h / 2, h / 2, -2.0f, 2.0f);
	bgfx::setViewTransform(0, view, ortho);


	bx::mtxMul(viewProj, view, ortho);

	bgfx::setViewRect(0, 0, 0, m_width, m_height);
};

void Camera::GetViewProj(float* dest) const
{
	memcpy(dest, viewProj, sizeof(viewProj));
}

void Camera::SetPosition(const float* _pos)
{
	memcpy(m_position, _pos, sizeof(float) * 2);
};

void Camera::SetWidthHeight(int width, int height)
{
	m_width = width;
	m_height = height;
};
