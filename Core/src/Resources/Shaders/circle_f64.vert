#version 460 core

layout (location = 0) in vec3 aPos;


layout(std430, binding = 1) buffer p_x
{
	double x[];
};

layout(std430, binding = 2) buffer p_y
{
	double y[];
};

layout(std430, binding = 3) buffer p_z
{
	double z[];
};

out vec2 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    position = vec2(aPos.x, aPos.y);

	float positionX = aPos.x + float(x[gl_InstanceID]);
	float positionY = aPos.y + float(y[gl_InstanceID]);
	float positionZ = aPos.z + float(z[gl_InstanceID]);

    gl_Position = projection * view * vec4( positionX, positionY, positionZ, 1.0);
 
}