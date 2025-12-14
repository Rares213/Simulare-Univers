#version 460 core

out vec4 fragColor;

in vec2 position;

uniform float radius;
uniform vec4 color;

void main()
{	
	if(dot(position, position) > radius)
		discard;

	fragColor = vec4(color.x, color.y, color.z, 1.0);
};