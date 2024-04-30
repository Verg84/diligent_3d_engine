uniform Constants
{
	mat4 MVP;
};

layout(location=0) in vec3 in_position;

void main()
{
	gl_Position=vec4(in_position,1.0)*MVP;
}