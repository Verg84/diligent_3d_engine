uniform sampler2D sampl;
out vec4 FragColor;
in vec2 texCoords;

void main()
{
	FragColor=texture(sampl,texCoords);
}