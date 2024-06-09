#extension GL_NV_mesh_shader : require

layout(local_size_x = 1) in;
layout(triangles, max_vertices = 3, max_primitives = 1) out;

// Custom vertex output block
layout (location = 0) out PerVertexData
{
  vec4 color;
} v_out[];   // [max_vertices]


float scale = 0.95;
const vec3 vertices[3] = {vec3(-1,-1,0), vec3(0,1,0), vec3(1,-1,0)};
const vec3 colors[3] = {vec3(1.0,0.0,0.0), vec3(0.0,1.0,0.0), vec3(0.0,0.0,1.0)};


void main()
{
  vec4 pos = vec4(vertices[0] * scale, 1.0);
  // GL->VK conventions...
  pos.y = -pos.y; pos.z = (pos.z + pos.w) / 2.0;
  gl_MeshVerticesNV[0].gl_Position = pos; 

  pos = vec4(vertices[1] * scale, 1.0);
  pos.y = -pos.y; pos.z = (pos.z + pos.w) / 2.0;
  gl_MeshVerticesNV[1].gl_Position = pos; 

  pos = vec4(vertices[2] * scale, 1.0);
  pos.y = -pos.y; pos.z = (pos.z + pos.w) / 2.0;
  gl_MeshVerticesNV[2].gl_Position = pos; 


  v_out[0].color = vec4(colors[0], 1.0);
  v_out[1].color = vec4(colors[1], 1.0);
  v_out[2].color = vec4(colors[2], 1.0);


  gl_PrimitiveIndicesNV[0] = 0;
  gl_PrimitiveIndicesNV[1] = 1;
  gl_PrimitiveIndicesNV[2] = 2;
  
  gl_PrimitiveCountNV = 1;
}