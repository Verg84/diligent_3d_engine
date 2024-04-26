layout(location=0)in vec3 position_in;

uniform Constants
{
	mat4 MVP;	
};



void main()
{
	gl_Position=vec4(position_in,1.0)*MVP;
}