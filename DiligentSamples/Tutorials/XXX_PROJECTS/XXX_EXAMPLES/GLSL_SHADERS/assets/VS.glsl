void main()
{
	vec4 triangle_positions[]=
		vec4[]
		(
			vec4(0.0f,0.0f,0.0f,1.0f),
			vec4(1.0f,0.0f,0.0f,1.0f),
			vec4(0.0f,1.0f,0.0f,1.0f)
		);
	gl_Position=triangle_positions[gl_VertexID];
}
