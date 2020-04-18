#version 330 core

out vec4 out_frag;
in vec2 uv;

uniform sampler2D quad_texture;
uniform vec2 uv_offset;

void main()
{
    out_frag = texture(quad_texture, uv + uv_offset);
}