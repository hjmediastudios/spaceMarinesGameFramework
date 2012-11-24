#version 330

@vertex
layout (location = 0) in vec3 in_position;
uniform vec3 color;

uniform mat4 mvpMat;

out vec4 pass_vColor;

void main()
{
    gl_Position = vec4(in_position, 1.0) * mvpMat;
    pass_vColor = vec4(color, 0.5f);
}


@fragment
in vec4 pass_vColor;

out vec4 out_color;

void main(void)
{
	out_color = pass_vColor;
}