uniform Constants
{
	mat4 MVP;
};

layout(location=0) in vec3 in_position;
layout(location=1) in vec2 in_texCoords;

out vec2 texCoords;

void main()
{
	gl_Position=vec4(in_position,1.0)*MVP;
	texCoords=in_texCoords;

}