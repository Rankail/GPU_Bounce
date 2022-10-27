#version 460 core

out vec4 FragColor;

in float v_ColorIndex;


void main()
{
	if (v_ColorIndex == 0.f) { FragColor = vec4(1.0, 0.0, 0.0, 1.0); }
	else if (v_ColorIndex == 1.f) { FragColor = vec4(0.0, 1.0, 0.0, 1.0); }
	else if (v_ColorIndex == 2.f) { FragColor = vec4(0.0, 0.0, 1.0, 1.0); }
	else if (v_ColorIndex == 3.f) { FragColor = vec4(1.0, 1.0, 0.0, 1.0); }
	else if (v_ColorIndex == 4.f) { FragColor = vec4(0.0, 0.9, 0.9, 1.0); }
	else if (v_ColorIndex == 5.f) { FragColor = vec4(1.0, 0.0, 1.0, 1.0); }
	else { FragColor = vec4(0.5, 0.5, 0.5, 1.0); }
}