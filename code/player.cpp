struct Player
{
    hmm_v2 pos;
    hmm_v2 vel;
    f32 speed;
    i32 scale;
    AnimationSM anim_sm;
};

global Player player;

internal Player
init_player(hmm_v2 pos)
{
    Player return_player;
    return_player.pos = pos;
    return_player.vel = {0.0f, 0.0f};
    return_player.speed = 10.0f;
    return_player.scale = 2;
    return_player.anim_sm = init_animation_sm(TEST_ANIMATION, true, true);
    return return_player;
}

internal void
player_update_and_render()
{
    // TODO(rohan): set anim sm state to different animations
    if (key_is_down(KEY_LEFT))
    {
        player.pos.X -= 5;
    }
    if (key_is_down(KEY_RIGHT))
    {
        player.pos.X += 5;
    }
    if (key_is_down(KEY_DOWN))
    {
        player.pos.Y -= 5;
    }
    if (key_is_down(KEY_UP))
    {
        player.pos.Y += 5;
    }

    hmm_v3 pos = {player.pos.X, player.pos.Y, 0.0f};
    draw_animated_quad(pos, 2, &player.anim_sm);

    animation_sm_update(&player.anim_sm);
}
