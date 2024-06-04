layout(triangles)in;
layout(triangle_strip)out;
layout(max_vertices=3)out;

in Vertex
{
	vec4 color;
}vertex[];

out vec4 color;

void main()
{
	
	for(int n=0;n<gl_in.length();i++)
	{
		if(gl_InvocationID==1)
			color=vec4(1.0,0.0,0.0,1.0);
		else
			color=vertex.color[i];
		EmitVertex();
		
	}EndPrimitive();
}