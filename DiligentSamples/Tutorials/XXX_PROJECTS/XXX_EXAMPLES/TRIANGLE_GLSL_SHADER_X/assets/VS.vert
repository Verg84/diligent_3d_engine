layout (location=0) out vec4 color;
void main()
{
	vec4 positions[]=vec4[]
		(
			vec4(0.0,0.0,0.0,1.0),
			vec4(1.0,0.0,0.0,1.0),
			vec4(0.0,1.0,0.0,1.0)
		);
	gl_Position=positions[gl_VertexID];
	color=positions[gl_VertexID];
}