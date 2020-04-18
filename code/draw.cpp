enum Shaders
{
    QUAD_SHADER,
    TEX_QUAD_SHADER,
    ANIM_QUAD_SHADER,
    TEXT_SHADER,
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
#include "text.cpp"

global hmm_mat4 projection_matrix;
global u32 quad_vao;

enum Blend
{
    FONT_BLEND,
    QUAD_BLEND
};

global u32 blend_mode;

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
    init_shader("anim_quad", ANIM_QUAD_SHADER);
	init_shader("text", TEXT_SHADER);
    
    // TEXTURES
    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    init_texture("awesomeface.png", TEST_TEXTURE);
    init_texture("0.png", TEST_TEXTURE2);

    // ANIMATIONS
    init_animation("player.png", TEST_ANIMATION, 16, 21, {32.0f, 32.0f}, .7f);

    // FONTS
    LoadFont("Inconsolata", DEFAULT_FONT);
    
    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
}

// QUAD
internal void draw_quad(hmm_v3 pos, hmm_v2 half_size, hmm_vec3 color)
{
    if(blend_mode == FONT_BLEND)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        blend_mode = QUAD_BLEND;
    }
    
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
    if(blend_mode == FONT_BLEND)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        blend_mode = QUAD_BLEND;
    }
    
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
    if(blend_mode == FONT_BLEND)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        blend_mode = QUAD_BLEND;
    }
    
    hmm_v2 half_size;
    half_size.X = (texture.width / 2) * scale; 
    half_size.Y = (texture.height / 2) * scale; 

    draw_textured_quad(pos, half_size, texture);
}

// ANIMATIONS
internal void draw_animated_quad(hmm_v3 pos, int scale, AnimationSM *animation_sm)
{
    if(blend_mode == FONT_BLEND)
    {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        blend_mode = QUAD_BLEND;
    }
        
    if(animation_sm->playing)
    {
        Animation *animation = &animation_cache[animation_sm->animation_id];
        Texture *texture = &animation->texture_atlas;
        
        f32 object_animation_timer = animation_sm->animation_timer;
        i32 anim_index = get_animation_frame(animation, object_animation_timer);
        i32 rows = texture->rows;
        i32 cols = texture->cols;
        f32 grid_x = (f32)(anim_index % cols);
        f32 grid_y = (f32)(HMM_Clamp(0, (anim_index - 1) / cols, ((anim_index - 1) / cols) + 1));
        
        shader_set(shader_cache[ANIM_QUAD_SHADER]);
        pos.X += (game_window.base_width / 2);
        pos.Y += (game_window.base_height / 2);

        hmm_v2 half_size = {(f32)(scale * texture->grid_width / 2),
                            (f32)(scale * texture->grid_height / 2)};
        hmm_vec2 uv_offset = {(f32)(grid_x * texture->grid_width / texture->width),
                              (f32)(grid_y * texture->grid_height / texture->height)};
        hmm_vec2 atlas_dim = {(f32)cols, (f32)rows};
        
        uniform_set_vec3(shader_cache[ANIM_QUAD_SHADER], "quad_pos", pos);
        uniform_set_vec2(shader_cache[ANIM_QUAD_SHADER], "quad_half_size", half_size);
        uniform_set_mat4(shader_cache[ANIM_QUAD_SHADER], "projection_matrix",
                         projection_matrix);
        uniform_set_vec2(shader_cache[ANIM_QUAD_SHADER], "uv_offset", uv_offset);
        uniform_set_vec2(shader_cache[ANIM_QUAD_SHADER], "atlas_dim", atlas_dim);
        
        texture_set(GL_TEXTURE0, *texture);    
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
       
    }
}

internal void RenderText(Font *font, hmm_vec4 color, hmm_vec2 position, const char *text)
{
    if(blend_mode == QUAD_BLEND)
    {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        blend_mode = FONT_BLEND;
    }

    
    GLuint shader = shader_cache[TEXT_SHADER];
    glUseProgram(shader);
    glBindVertexArray(quad_vao);
    
    glUniform1i(glGetUniformLocation(shader, "font"), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, font->texture.id);
    
    for(int i = 0; text[i]; ++i)
    {
        stbtt_aligned_quad quad;
        stbtt_GetBakedQuad(font->char_data, 512, 512, text[i]-32,  &position.X, &position.Y, &quad, 1);
        
        hmm_vec4 source = 
		{
            quad.s0, quad.t0,
            quad.s1 - quad.s0,
            quad.t1 - quad.t0,
        };
        
        hmm_vec4 destination = 
		{
            quad.x0, quad.y0,
            quad.x1 - quad.x0,
            quad.y1 - quad.y0,
        };
        
        glUniform2f(glGetUniformLocation(shader, "resolution"), (float)game_window.width, (float)game_window.height);
        glUniform4f(glGetUniformLocation(shader, "text_color"), color.X, color.Y, color.Z, color.W);
        glUniform4f(glGetUniformLocation(shader, "source"), source.X, source.Y, source.Z, source.W);
        glUniform4f(glGetUniformLocation(shader, "destination"), destination.X, destination.Y, destination.Z, destination.W);
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
}
