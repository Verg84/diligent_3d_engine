void main()
{
	vec4 positions[]=vec4[]
	(
		vec4(-0.9,-0.9,0.0,1.0),
		vec4(0.2,-0.2,0.0,1.0),
		vec4(0.0,1.0,0.0,1.0)
	);

	gl_Position=positions[gl_VertexID];
}