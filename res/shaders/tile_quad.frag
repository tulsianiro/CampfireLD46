#version 330 core

out vec4 out_frag;
in vec2 uv;
in vec2 frag_pos;

uniform sampler2D quad_texture;
uniform vec2 uv_offset;

uniform vec2 light_pos;
uniform float light_quantity;

void main()
{
    vec2 euclidean_line = frag_pos - light_pos;
    float distance = abs(length(euclidean_line));
    float light_amount = 1.0f - (distance / 400.0f / (light_quantity + 0.01));      
    out_frag = light_amount * (texture(quad_texture, uv + uv_offset));
}