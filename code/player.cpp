enum Direction
{
    LEFT,
    RIGHT
};

struct Player
{
    hmm_v2 pos;
    hmm_v2 vel;
    f32 speed;
    i32 scale;
    AnimationSM anim_sm;
    u32 direction;
    AABB aabb;
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
    return_player.anim_sm = init_animation_sm(IDLE_RIGHT_ANIMATION, true, true);
    return_player.aabb.pos = return_player.pos;
    hmm_v2 player_half_dim = {8.0f, 16.0f};
    return_player.aabb.half_dim = return_player.scale * player_half_dim;
    
    return return_player;
}

internal void
player_update_and_render()
{
    b32 is_moving = false;
    
    // TODO(rohan): set anim sm state to different animations
    if(key_is_down(KEY_LEFT) && key_is_down(KEY_RIGHT))
    {
    }
    else if(key_is_down(KEY_LEFT))
    {
        is_moving = true;
        player.direction = LEFT;
        player.pos.X -= 5;
        animation_set(&player.anim_sm, RUNNING_LEFT_ANIMATION, true, true);
    }
    else if(key_is_down(KEY_RIGHT))
    {
        is_moving = true;
        player.pos.X += 5;
        player.direction = RIGHT;
        animation_set(&player.anim_sm, RUNNING_RIGHT_ANIMATION, true, true);
    }
    // test motion for follow cam
    if(key_is_down(KEY_UP))
    {
        player.pos.Y += 5;
    }
    if(key_is_down(KEY_DOWN))
    {
        player.pos.Y -= 5;
    }

    if(!is_moving)
    {
        if(player.direction == LEFT)
        {
            animation_set(&player.anim_sm, IDLE_LEFT_ANIMATION, true, true);
        }
        else if(player.direction == RIGHT)
        {
            animation_set(&player.anim_sm, FALLING_RIGHT_ANIMATION, true, true);
        }
    }

    hmm_v3 pos = {player.pos.X, player.pos.Y, 0.0f};
    player.aabb.pos = {player.pos.X, player.pos.Y};
    int player_grid_x = (player.pos.X + level.world_offset.X) / (tilemap.grid_width * level.tilescale);
    int player_grid_y = (player.pos.Y + level.world_offset.Y) / (tilemap.grid_height * level.tilescale);
    // hmm_v2 tile_pos = level.tiles[player_grid_y][player_grid_x].aabb.pos;
    // hmm_v2 tile_half_boi = level.tiles[player_grid_y][player_grid_x].aabb.half_dim;
    // check your own tile
    if(aabb_vs_aabb(player.aabb, level.tiles[player_grid_y][player_grid_x].aabb))
    {
        
    }

    if(aabb_vs_aabb(player.aabb, level.tiles[player_grid_y][player_grid_x].aabb))
    {
     
    }
    
    pos = world_to_screen(pos);
    // tile_pos = world_to_screen(tile_pos);
    draw_quad(pos, player.aabb.half_dim, {1.0, 0.0, 0.0});
    // draw_quad({tile_pos.X, tile_pos.Y, 0.0f}, tile_half_boi, {0.0, 1.0, 0.0});
    draw_animated_quad(pos, player.scale, &player.anim_sm);
    animation_sm_update(&player.anim_sm);
}
