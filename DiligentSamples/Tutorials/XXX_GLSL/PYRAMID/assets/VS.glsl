layout(location=0)in vec3 in_position;
layout(location=1)in vec2 in_texCoords;

uniform Constf
{
	vec3 offset;
};

out vec2 texCoords;



void main()
{

	vec3 position=offset+in_position;
	gl_Position=vec4(position,1.0);
	texCoords=in_texCoords;

}