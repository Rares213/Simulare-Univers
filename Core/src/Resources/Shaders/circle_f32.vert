#version 460 core

layout (location = 0) in vec3 aPos;


layout(std430, binding = 1) buffer p_x
{
	float x[];
};

layout(std430, binding = 2) buffer p_y
{
	float y[];
};

layout(std430, binding = 3) buffer p_z
{
	float z[];
};

out vec2 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    position = vec2(aPos.x, aPos.y);

    gl_Position = projection * view * vec4(aPos.x + x[gl_InstanceID], aPos.y + y[gl_InstanceID], aPos.z + z[gl_InstanceID], 1.0);
 
}