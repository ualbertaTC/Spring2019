#version 120 
#extension GL_ARB_explicit_attrib_location : require

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture2D(texture_diffuse1, TexCoords);
    FragColor = vec4(1.0,1.0,1.0,1.0);
}
