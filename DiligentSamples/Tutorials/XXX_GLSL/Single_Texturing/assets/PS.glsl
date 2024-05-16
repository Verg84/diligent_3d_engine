uniform sampler2D g_Texture;

in vec2 texCoords;
out vec4 FragColor;
void main()
{
	FragColor=texture(g_Texture,texCoords);
}