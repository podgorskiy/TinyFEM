$input v_weight
uniform vec4 u_percantage;
uniform vec4 u_color;

//#include <bgfx_shader.sh>

void main()
{
	vec3 color = vec3(v_weight);
	gl_FragColor = vec4( mix(color, u_color.xyz, u_percantage.x), 1.0);
}
