#version 460 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in float a_ColorIndex;

out float v_ColorIndex;

void main()
{
	gl_Position = vec4(a_Pos, 1.0);
	v_ColorIndex = a_ColorIndex;
}

