#version 330

@vertex
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_color;

uniform mat4 mvpMat;

out vec4 pass_vColor;

void main()
{
    gl_Position = vec4(in_position, 1.0) * mvpMat;
    pass_vColor = vec4(in_color, 0.5);
}


@fragment
in vec4 pass_vColor;

out vec4 out_color;

void main(void)
{
	out_color = pass_vColor;
}