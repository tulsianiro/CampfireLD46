#include "collision.cpp"

global AnimationSM animation_sm;
global b32 game_initialized = false;

void init_game()
{
     animation_sm = init_animation_sm(TEST_ANIMATION, true, true);
     // play_sound(TEST_AUDIO);
}

void simulate_frame(Input *input)
{
    if(!game_initialized)
    {
        init_game();
        game_initialized = true;
    }

    // TODO(rohan): Game Object Updates
    {
        animation_sm_update(&animation_sm);
    }

    hmm_v3 pos1 = {0.0f, 0.0f, 0.0f};
    hmm_v3 pos2 = {(f32)input->mouse.mouse_x, (f32)input->mouse.mouse_y, 0.0f};
    
    AABB aabb1 = init_aabb_from_anim({pos1.X, pos1.Y}, &animation_sm);
    AABB aabb2 = init_aabb_from_texture({pos2.X, pos2.Y}, TEST_TEXTURE2);

    draw_quad({aabb1.pos.X, aabb1.pos.Y, 0}, aabb1.half_dim, {0.8, 0.5, 0.6});
    draw_quad({aabb2.pos.X, aabb2.pos.Y, 0}, aabb2.half_dim, {0.8, 0.5, 0.6});
    draw_animated_quad(pos1, 1, &animation_sm);
    draw_textured_quad(pos2, 1, texture_cache[TEST_TEXTURE2]);

    if(aabb_vs_aabb(aabb1, aabb2))
    {
        draw_quad({-100, -100, 0}, {50, 50}, {0.0, 0.4, 0.8});
    }
}
