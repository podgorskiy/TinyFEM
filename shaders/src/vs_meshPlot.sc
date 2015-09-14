$input a_position, a_weight
$output v_weight

#include <bgfx_shader.sh>

void main()
{
	gl_Position = mul(u_modelViewProj,vec4(vec3(a_position.x, a_position.y, 0.0), 1.0));
	v_weight = a_weight;
}
