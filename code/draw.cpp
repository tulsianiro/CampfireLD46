enum Shaders
{
    QUAD_SHADER,
    TEX_QUAD_SHADER,
    LAST_SHADER
};

enum Textures
{
    TEST_TEXTURE,
    TEST_TEXTURE2,
    LAST_TEXTURE
};

enum Animations
{
    TEST_ANIMATION,
    LAST_ANIMATION
};

#include "shader.cpp"
#include "texture.cpp"
#include "animation.cpp"

global hmm_mat4 projection_matrix;
global u32 quad_vao;

void draw_init()
{
    u32 half_width = game_window.base_width / 2;
    u32 half_height = game_window.base_height / 2;
    
    projection_matrix = HMM_Orthographic(0, game_window.base_width,
                                         0, game_window.base_height,
                                         -1.0f, 1.0f);

    // SHADER
    init_shader("quad", QUAD_SHADER);
    init_shader("tex_quad", TEX_QUAD_SHADER);
    
    // TEXTURES
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_texture("awesomeface.png", TEST_TEXTURE);
    init_texture("0.png", TEST_TEXTURE2);

    // ANIMATIONS
    init_animation("fontanitest", TEST_ANIMATION, 3, 2.0f);
    
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
}

// QUAD
internal void draw_quad(hmm_v3 pos, hmm_v2 half_size, hmm_vec3 color)
{    
    shader_set(shader_cache[QUAD_SHADER]);

    pos.X += (game_window.base_width / 2);
    pos.Y += (game_window.base_height / 2);

    uniform_set_vec3(shader_cache[QUAD_SHADER], "quad_color", color);
    uniform_set_vec3(shader_cache[QUAD_SHADER], "quad_pos", pos);
    uniform_set_vec2(shader_cache[QUAD_SHADER], "quad_half_size", half_size);
    uniform_set_mat4(shader_cache[QUAD_SHADER], "projection_matrix",
                     projection_matrix);
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// TEXTURES
internal void draw_textured_quad(hmm_v3 pos, hmm_v2 half_size, Texture texture)
{    
    shader_set(shader_cache[TEX_QUAD_SHADER]);
    pos.X += (game_window.base_width / 2);
    pos.Y += (game_window.base_height / 2);

    uniform_set_vec3(shader_cache[TEX_QUAD_SHADER], "quad_pos", pos);
    uniform_set_vec2(shader_cache[TEX_QUAD_SHADER], "quad_half_size", half_size);
    uniform_set_mat4(shader_cache[TEX_QUAD_SHADER], "projection_matrix",
                     projection_matrix);

    texture_set(GL_TEXTURE0, texture);    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

internal void draw_textured_quad(hmm_v3 pos, int scale, Texture texture)
{
    hmm_v2 half_size;
    half_size.X = (texture.width / 2) * scale; 
    half_size.Y = (texture.height / 2) * scale; 

    draw_textured_quad(pos, half_size, texture);

    shader_set(shader_cache[TEX_QUAD_SHADER]);
    pos.X += (game_window.base_width / 2);
    pos.Y += (game_window.base_height / 2);

    uniform_set_vec3(shader_cache[TEX_QUAD_SHADER], "quad_pos", pos);
    uniform_set_vec2(shader_cache[TEX_QUAD_SHADER], "quad_half_size", half_size);
    uniform_set_mat4(shader_cache[TEX_QUAD_SHADER], "projection_matrix",
                     projection_matrix);

    texture_set(GL_TEXTURE0, texture);    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// ANIMATIONS
internal void draw_animated_quad(hmm_v3 pos, int scale, AnimationSM *animation_sm)
{
    if(animation_sm->playing)
    {
        Animation *animation = &animation_cache[animation_sm->animation_id];
        f32 object_animation_timer = animation_sm->animation_timer;
        Texture texture = get_animation_frame(animation, object_animation_timer);
        draw_textured_quad(pos, scale, texture);
    }
}

