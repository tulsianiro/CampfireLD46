#version 330 core

out vec4 out_frag;
uniform vec3 quad_color;

void main()
{
    out_frag = vec4(quad_color, 1);
}