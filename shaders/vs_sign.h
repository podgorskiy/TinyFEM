static const uint8_t vs_sign_h[1100] =
{
	0x56, 0x53, 0x48, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x0f, 0x75, 0x5f, 0x6d, 0x6f, 0x64, // VSH........u_mod
	0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x04, 0x01, 0x00, 0x00, 0x01, 0x00, // elViewProj......
	0x27, 0x04, 0x00, 0x00, 0x61, 0x74, 0x74, 0x72, 0x69, 0x62, 0x75, 0x74, 0x65, 0x20, 0x68, 0x69, // '...attribute hi
	0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, // ghp vec3 a_posit
	0x69, 0x6f, 0x6e, 0x3b, 0x0a, 0x75, 0x6e, 0x69, 0x66, 0x6f, 0x72, 0x6d, 0x20, 0x68, 0x69, 0x67, // ion;.uniform hig
	0x68, 0x70, 0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, // hp mat4 u_modelV
	0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, // iewProj;.void ma
	0x69, 0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, // in ().{.  highp 
	0x76, 0x65, 0x63, 0x33, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x3b, 0x0a, 0x20, // vec3 tmpvar_1;. 
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x78, 0x20, 0x3d, 0x20, 0x75, 0x5f, //  tmpvar_1.x = u_
	0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x5b, 0x30, 0x5d, // modelViewProj[0]
	0x2e, 0x79, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x2e, 0x79, // .y;.  tmpvar_1.y
	0x20, 0x3d, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, //  = u_modelViewPr
	0x6f, 0x6a, 0x5b, 0x31, 0x5d, 0x2e, 0x79, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // oj[1].y;.  tmpva
	0x72, 0x5f, 0x31, 0x2e, 0x7a, 0x20, 0x3d, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, // r_1.z = u_modelV
	0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x5b, 0x32, 0x5d, 0x2e, 0x79, 0x3b, 0x0a, 0x20, 0x20, // iewProj[2].y;.  
	0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x66, 0x6c, 0x6f, 0x61, 0x74, 0x20, 0x74, 0x6d, 0x70, 0x76, // highp float tmpv
	0x61, 0x72, 0x5f, 0x32, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, // ar_2;.  tmpvar_2
	0x20, 0x3d, 0x20, 0x73, 0x71, 0x72, 0x74, 0x28, 0x64, 0x6f, 0x74, 0x20, 0x28, 0x74, 0x6d, 0x70, //  = sqrt(dot (tmp
	0x76, 0x61, 0x72, 0x5f, 0x31, 0x2c, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x31, 0x29, // var_1, tmpvar_1)
	0x29, 0x3b, 0x0a, 0x20, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x33, 0x20, // );.  highp vec3 
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, // tmpvar_3;.  tmpv
	0x61, 0x72, 0x5f, 0x33, 0x2e, 0x78, 0x20, 0x3d, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, // ar_3.x = u_model
	0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x5b, 0x30, 0x5d, 0x2e, 0x7a, 0x3b, 0x0a, 0x20, // ViewProj[0].z;. 
	0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x75, 0x5f, //  tmpvar_3.y = u_
	0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x5b, 0x31, 0x5d, // modelViewProj[1]
	0x2e, 0x7a, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x2e, 0x7a, // .z;.  tmpvar_3.z
	0x20, 0x3d, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, //  = u_modelViewPr
	0x6f, 0x6a, 0x5b, 0x32, 0x5d, 0x2e, 0x7a, 0x3b, 0x0a, 0x20, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, // oj[2].z;.  highp
	0x20, 0x6d, 0x61, 0x74, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x3b, 0x0a, //  mat4 tmpvar_4;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x30, 0x5d, 0x2e, 0x78, 0x20, //   tmpvar_4[0].x 
	0x3d, 0x20, 0x28, 0x31, 0x2e, 0x30, 0x2f, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, // = (1.0/(tmpvar_2
	0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x30, // ));.  tmpvar_4[0
	0x5d, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, // ].y = 0.0;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x30, 0x5d, 0x2e, 0x7a, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, // var_4[0].z = 0.0
	0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x30, 0x5d, 0x2e, // ;.  tmpvar_4[0].
	0x77, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // w = 0.0;.  tmpva
	0x72, 0x5f, 0x34, 0x5b, 0x31, 0x5d, 0x2e, 0x78, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, // r_4[1].x = 0.0;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x31, 0x5d, 0x2e, 0x79, 0x20, //   tmpvar_4[1].y 
	0x3d, 0x20, 0x28, 0x31, 0x2e, 0x30, 0x2f, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x32, // = (1.0/(tmpvar_2
	0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x31, // ));.  tmpvar_4[1
	0x5d, 0x2e, 0x7a, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, // ].z = 0.0;.  tmp
	0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x31, 0x5d, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, // var_4[1].w = 0.0
	0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x32, 0x5d, 0x2e, // ;.  tmpvar_4[2].
	0x78, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // x = 0.0;.  tmpva
	0x72, 0x5f, 0x34, 0x5b, 0x32, 0x5d, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, // r_4[2].y = 0.0;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x32, 0x5d, 0x2e, 0x7a, 0x20, //   tmpvar_4[2].z 
	0x3d, 0x20, 0x69, 0x6e, 0x76, 0x65, 0x72, 0x73, 0x65, 0x73, 0x71, 0x72, 0x74, 0x28, 0x64, 0x6f, // = inversesqrt(do
	0x74, 0x20, 0x28, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x33, 0x2c, 0x20, 0x74, 0x6d, 0x70, // t (tmpvar_3, tmp
	0x76, 0x61, 0x72, 0x5f, 0x33, 0x29, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, // var_3));.  tmpva
	0x72, 0x5f, 0x34, 0x5b, 0x32, 0x5d, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, // r_4[2].w = 0.0;.
	0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x33, 0x5d, 0x2e, 0x78, 0x20, //   tmpvar_4[3].x 
	0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // = 0.0;.  tmpvar_
	0x34, 0x5b, 0x33, 0x5d, 0x2e, 0x79, 0x20, 0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, // 4[3].y = 0.0;.  
	0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, 0x33, 0x5d, 0x2e, 0x7a, 0x20, 0x3d, 0x20, // tmpvar_4[3].z = 
	0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x5b, // 0.0;.  tmpvar_4[
	0x33, 0x5d, 0x2e, 0x77, 0x20, 0x3d, 0x20, 0x31, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x68, 0x69, // 3].w = 1.0;.  hi
	0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // ghp vec4 tmpvar_
	0x35, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, 0x35, 0x2e, 0x77, 0x20, // 5;.  tmpvar_5.w 
	0x3d, 0x20, 0x30, 0x2e, 0x30, 0x3b, 0x0a, 0x20, 0x20, 0x74, 0x6d, 0x70, 0x76, 0x61, 0x72, 0x5f, // = 0.0;.  tmpvar_
	0x35, 0x2e, 0x78, 0x79, 0x7a, 0x20, 0x3d, 0x20, 0x28, 0x61, 0x5f, 0x70, 0x6f, 0x73, 0x69, 0x74, // 5.xyz = (a_posit
	0x69, 0x6f, 0x6e, 0x20, 0x2a, 0x20, 0x30, 0x2e, 0x31, 0x29, 0x3b, 0x0a, 0x20, 0x20, 0x67, 0x6c, // ion * 0.1);.  gl
	0x5f, 0x50, 0x6f, 0x73, 0x69, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x3d, 0x20, 0x28, 0x28, 0x75, 0x5f, // _Position = ((u_
	0x6d, 0x6f, 0x64, 0x65, 0x6c, 0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x20, 0x2a, 0x20, // modelViewProj * 
	0x76, 0x65, 0x63, 0x34, 0x28, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x30, 0x2e, 0x30, 0x2c, 0x20, 0x30, // vec4(0.0, 0.0, 0
	0x2e, 0x30, 0x2c, 0x20, 0x31, 0x2e, 0x30, 0x29, 0x29, 0x20, 0x2b, 0x20, 0x28, 0x28, 0x74, 0x6d, // .0, 1.0)) + ((tm
	0x70, 0x76, 0x61, 0x72, 0x5f, 0x34, 0x20, 0x2a, 0x20, 0x75, 0x5f, 0x6d, 0x6f, 0x64, 0x65, 0x6c, // pvar_4 * u_model
	0x56, 0x69, 0x65, 0x77, 0x50, 0x72, 0x6f, 0x6a, 0x29, 0x20, 0x2a, 0x20, 0x74, 0x6d, 0x70, 0x76, // ViewProj) * tmpv
	0x61, 0x72, 0x5f, 0x35, 0x29, 0x29, 0x3b, 0x0a, 0x7d, 0x0a, 0x0a, 0x00,                         // ar_5));.}...
};
