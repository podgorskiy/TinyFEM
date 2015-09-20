$input a_position

#include <bgfx_shader.sh>

uniform vec4 u_viewport;
uniform vec4 u_signSize;

void main()
{
	mat4 m = u_modelViewProj;
	//float sx = length(vec3(m[0][0], m[1][0], m[2][0]));
	float sy = length(vec3(m[0][1], m[1][1], m[2][1]));
	float sz = length(vec3(m[0][2], m[1][2], m[2][2]));
	mat4 scale = mat4(1./sy, 0,0,0,   0,1./sy,0,0,   0,0,1./sz,0, 0,0,0,1);
	mat4 mat_ = mul(scale,u_modelViewProj);
	gl_Position = mul(u_modelViewProj, vec4(vec3(0.0), 1.0)) +  mul(mat_, vec4(a_position.xyz * 0.1, 0.0));
}
