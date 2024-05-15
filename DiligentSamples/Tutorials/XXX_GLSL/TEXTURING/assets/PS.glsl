uniform sampler2D g_Texture;

out vec4 FragColor;
in vec2 texCoords;

void main()
{
	FragColor=texture(g_Texture,texCoords);
}