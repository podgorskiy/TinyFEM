$input a_position, a_texcoord0, a_texcoord1
$output v_weight

uniform vec4 u_deformScale;

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj, vec4(a_position + a_texcoord1 * u_deformScale.xyz, 1.0));
	v_weight = a_texcoord0;
}
