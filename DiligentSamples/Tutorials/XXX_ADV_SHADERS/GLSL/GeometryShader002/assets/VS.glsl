layout(location=0) in vec3 in_position;

out Vertex
{
	vec4 color;
}vertex;

void main()
{
	vertex.color=vec4(0.2,0.2,0.2,1.0);
	gl_Position=vec4(in_position,1,0);
}
