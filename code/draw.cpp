#include "shader.cpp"

hmm_mat4 projection_matrix;

global u32 quad_vao;

void draw_init()
{
    u32 half_width = game_window.base_width / 2;
    u32 half_height = game_window.base_height / 2;
    
    projection_matrix = HMM_Orthographic(0, game_window.base_width,
                                         0, game_window.base_height,
                                         -1.0f, 1.0f);
    init_shader("basic", BASIC_SHADER);
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
}

void draw_quad(hmm_v3 pos, hmm_v2 half_size, hmm_vec3 color)
{    
    shader_use(shader_cache[BASIC_SHADER]);

    pos.X += (game_window.base_width / 2);
    pos.Y += (game_window.base_height / 2);

    uniform_set_vec3(shader_cache[BASIC_SHADER], "quad_color", color);
    uniform_set_vec3(shader_cache[BASIC_SHADER], "quad_pos", pos);
    uniform_set_vec2(shader_cache[BASIC_SHADER], "quad_half_size", half_size);
    uniform_set_mat4(shader_cache[BASIC_SHADER], "projection_matrix",
                     projection_matrix);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// TODO(rohan): do this
// void draw_textured_quad(hmm_v3 pos, hmm_v2 half_size, Texture texture)
// {    
//     shader_use(shader_cache[BASIC_SHADER]);

//     pos.X += (game_window.base_width / 2);
//     pos.Y += (game_window.base_height / 2);

//     uniform_set_vec3(shader_cache[BASIC_SHADER], "quad_pos", pos);
//     uniform_set_vec2(shader_cache[BASIC_SHADER], "quad_half_size", half_size);
//     uniform_set_mat4(shader_cache[BASIC_SHADER], "projection_matrix",
//                      projection_matrix);
    
//     glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// }


