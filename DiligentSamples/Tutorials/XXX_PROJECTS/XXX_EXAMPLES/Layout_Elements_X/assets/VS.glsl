layout(location=0) in vec4 position_in;
layout(location=0)out vec4 color;

void main()
{
	gl_Position=position_in;
	color=position_in;
}