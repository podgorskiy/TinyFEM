$input v_weight
uniform vec4 u_percantage;
uniform vec4 u_color;
uniform vec4 u_rangesIntervals;

//#include <bgfx_shader.sh>

void main()
{
	//float w = (v_weight - u_rangesIntervals.x) / (u_rangesIntervals.y - u_rangesIntervals.x);
	float w = v_weight;
	float v = floor(w * (u_rangesIntervals.z + 1.0))/u_rangesIntervals.z;
	vec3 color = clamp(vec3((v - 0.5)*4.0, 2.0 - abs(v- 0.5)*4.0,	2.0 - v*4.0), 0.0, 1.0);
	gl_FragColor =  mix(vec4(color * 0.85,  1.0), u_color, u_percantage.x);
}
