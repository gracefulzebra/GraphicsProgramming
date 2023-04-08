#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

void main()
{
	//Scales the position by the MVP
	gl_Position = transform * vec4(aPos, 1.0);
}