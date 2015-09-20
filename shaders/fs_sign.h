static const uint8_t fs_sign_h[101] =
{
	0x46, 0x53, 0x48, 0x04, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07, 0x75, 0x5f, 0x63, 0x6f, 0x6c, // FSH........u_col
	0x6f, 0x72, 0x02, 0x01, 0x00, 0x00, 0x01, 0x00, 0x48, 0x00, 0x00, 0x00, 0x75, 0x6e, 0x69, 0x66, // or......H...unif
	0x6f, 0x72, 0x6d, 0x20, 0x68, 0x69, 0x67, 0x68, 0x70, 0x20, 0x76, 0x65, 0x63, 0x34, 0x20, 0x75, // orm highp vec4 u
	0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, 0x0a, 0x76, 0x6f, 0x69, 0x64, 0x20, 0x6d, 0x61, 0x69, // _color;.void mai
	0x6e, 0x20, 0x28, 0x29, 0x0a, 0x7b, 0x0a, 0x20, 0x20, 0x67, 0x6c, 0x5f, 0x46, 0x72, 0x61, 0x67, // n ().{.  gl_Frag
	0x43, 0x6f, 0x6c, 0x6f, 0x72, 0x20, 0x3d, 0x20, 0x75, 0x5f, 0x63, 0x6f, 0x6c, 0x6f, 0x72, 0x3b, // Color = u_color;
	0x0a, 0x7d, 0x0a, 0x0a, 0x00,                                                                   // .}...
};