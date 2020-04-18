global AnimationSM animation_sm;
global AnimationSM animation_sm2;

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
    
    draw_animated_quad({0.0f, 0.0f, 0.0f}, 1, &animation_sm);
    draw_textured_quad({100.0f, 100.0f, 0.0f}, 1, texture_cache[TEST_TEXTURE2]);
}
