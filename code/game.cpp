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
        init_camera(0, 100);
        game_initialized = true;
    }

    if (key_is_down(KEY_LEFT))
    {
        camera.x -= 5;
    }
    if (key_is_down(KEY_RIGHT))
    {
        camera.x += 5;
    }
        if (key_is_down(KEY_DOWN))
    {
        camera.y -= 5;
    }
    if (key_is_down(KEY_UP))
    {
        camera.y += 5;
    }

    for(int i = 0; i < 1; i++)
    {
        hmm_vec3 tilemapped_pos = {-330.0f, 0.0f, 0.0f};
        tilemapped_pos = world_to_screen(tilemapped_pos);
        draw_tilemapped_quad(tilemapped_pos, 3, 2);
    }
        // TODO(rohan): Game Object Updates
    {
        animation_sm_update(&animation_sm);
    }

    hmm_vec3 animated_pos = {0.0f, 0.0f, 0.0f};
    animated_pos = world_to_screen(animated_pos);
    // draw_animated_quad(animated_pos, 3, &animation_sm);

    // hmm_v3 pos1 = {0.0f, 0.0f, 0.0f};
    // hmm_v3 pos2 = {(f32)input->mouse.mouse_x, (f32)input->mouse.mouse_y, 0.0f};
    
    // AABB aabb1 = init_aabb_from_anim({pos1.X, pos1.Y}, &animation_sm);
    // AABB aabb2 = init_aabb_from_texture({pos2.X, pos2.Y}, TEST_TEXTURE2);

    // draw_quad({aabb1.pos.X, aabb1.pos.Y, 0}, aabb1.half_dim, {0.8, 0.5, 0.6});
    // draw_quad({pos2.X, pos2.Y, 0},
    //           {(f32)(texture_cache[TEST_TEXTURE2].width / 2.0f),
    //            (f32)(texture_cache[TEST_TEXTURE2].height / 2.0f)},
    //           {0.8, 0.5, 0.6});
    
    // draw_animated_quad(pos1, 1, &animation_sm);
    // draw_textured_quad(pos2, 1, texture_cache[TEST_TEXTURE2]);
  
    // if(aabb_vs_aabb(aabb1, aabb2))
    // {
    //     draw_quad({-100, -100, 0}, {50, 50}, {0.0, 0.4, 0.8});
    // }

    char buf[128];
    sprintf_s(buf, 128, "FPS:%d", (int)(1/dt));
    RenderText(&font_cache[DEFAULT_FONT], {1, 0, 0, 1}, {25.0f, 25.0f}, buf);
}
