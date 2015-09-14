#pragma once
#include <entry/entry.h>

class Camera
{
public:
	struct MouseCoords
	{
		int m_mx;
		int m_my;
		int m_mz;
	};

	Camera();
	~Camera();

	void Reset();
	
	void Update(const entry::MouseState& _mouseState);
	void UpplyTransform();
	void SetPosition(const float* _pos);
	void SetWidthHeight(int width, int height);

	void GetViewProj(float* dest) const;

private:
	MouseCoords m_mouseNow;
	MouseCoords m_mouseLast;

	int m_width;
	int m_height;

	float m_position[2];
	float m_fov;

	float viewProj[16];
	bool m_mouseDown;
};
