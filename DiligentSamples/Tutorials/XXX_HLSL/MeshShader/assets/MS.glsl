#extension GL_NV_mesh_shader:require

layout(local_size_x=1)in;

layout(max_vertices=1,max_primitives=1)out;

layout(triangles)out;

out PerVertexData
{
	vec4 color;

}v_out[];

const vec3 vertices[3]=
{
	vec3(-1,-1,0),vec3(0,1,0),vec3(1,-1,0)
};

const vec3 color[3]={vec3(1.0,0.0,0.0),vec3(0.0,1.0,0.0),vec3(0.0,0.0,1.0)}

void main()
{
	gl_MeshVerticesNV[0].gl_Position=vec4(vertices[0],1.0);
	gl_MeshVerticesNV[1].gl_Position=vec4(vertices[1],1.0);
	gl_MeshVerticesNV[2].gl_Position=vec4(vertices[2],1.0);

	v_out[0].color=vec4(colors[0],1.0);
	v_out[1].color=vec4(colors[1],1.0);
	v_out[2].color=vec4(colors[2],1.0);

	gl_PrimitiveCountNV=1;
}