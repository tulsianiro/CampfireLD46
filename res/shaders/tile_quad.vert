#version 330 core

uniform vec3 quad_pos;
uniform vec2 quad_half_size;
uniform mat4 projection_matrix;
uniform vec2 atlas_dim;

out vec2 uv;
out vec2 frag_pos;

void main()
{
    vec2 vertices[] = vec2[](vec2(0, 0), vec2(0, 1), vec2(1, 0), vec2(1, 1));

    vec2 vertex = vertices[gl_VertexID];
    uv = vertex / atlas_dim; 

    vertex -= 0.5;
    vertex *= 2;
    float position_x = quad_pos.x + vertex.x * quad_half_size.x;
    float position_y = quad_pos.y + vertex.y * quad_half_size.y;

    frag_pos = vec2(position_x, position_y);
    
    vec4 position = vec4(position_x, position_y, 0, 1);
    gl_Position = projection_matrix * position;
}