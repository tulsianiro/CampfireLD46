enum Direction
{
    LEFT,
    RIGHT
};

struct Player
{
    hmm_v2 spawn_pos;
    hmm_v2 pos;
    hmm_v2 vel;
    f32 speed;
    i32 scale;
    AnimationSM anim_sm;
    u32 direction;
    AABB aabb;
    b32 in_air;

    // movement vars
    // natural
    f32 horizontal_drag_ground = 0.8f;
    f32 horizontal_drag_air = 0.8f;
    f32 gravity = 1050.0f;
    f32 epsilon = 0.5f;

    // active
    int max_fat_jump_frames = 6;
    int fat_jump_frames = max_fat_jump_frames;
    f32 run_accel = 4000.0f;
    f32 jump_accel = 6000.0f;
    f32 max_horizontal_speed = 300.0f;
    f32 max_vertical_speed = 800.0f;
    b32 jump_ready = false;
};

global Player player;

internal Player
init_player(hmm_v2 pos)
{
    Player return_player;
    return_player.pos = pos;
    return_player.spawn_pos = pos;
    return_player.vel = {0.0f, 0.0f};
    return_player.speed = 10.0f;
    return_player.scale = 2;

    return_player.aabb.pos = return_player.pos;
    hmm_v2 player_half_dim = {8.0f, 16.0f};
    return_player.aabb.half_dim = return_player.scale * player_half_dim;

    return_player.anim_sm = init_animation_sm(IDLE_RIGHT_ANIMATION, true, true);
    return_player.direction = RIGHT;
    return_player.in_air = true;
    
    return return_player;
}

internal void set_respawn_pos(hmm_v2 pos)
{
    player.spawn_pos = pos;
}
internal void respawn_player()
{
    player.pos.X = player.spawn_pos.X;
    player.pos.Y = player.spawn_pos.Y + 40.0f;
    player.vel = {0.0f, 0.0f};
    player.direction = RIGHT;
    player.in_air = true;
}

internal void
player_update_and_render()
{
    b32 is_moving = false;
    b32 changed_direction = false;

    if (key_pressed(KEY_Q))
    {
        respawn_player();
    }
    
    if(key_is_down(KEY_LEFT) && key_is_down(KEY_RIGHT))
    {
        // DO NOTHING
    }
    else if(key_is_down(KEY_LEFT))
    {
        if(player.vel.X > 0)
        {
            changed_direction = true;
        }
        is_moving = true;
        player.direction = LEFT;
        player.vel.X -= player.run_accel * dt;
        animation_set(&player.anim_sm, RUNNING_LEFT_ANIMATION, true, true);
     
    }
    else if(key_is_down(KEY_RIGHT))
    {
        if (player.vel.X < 0)
        {
            changed_direction = true;
        }
        is_moving = true;
        player.vel.X += player.run_accel * dt;
        player.direction = RIGHT;
        animation_set(&player.anim_sm, RUNNING_RIGHT_ANIMATION, true, true);
    }

    if(key_is_down(KEY_C))
    {
        if (player.fat_jump_frames > 0 && player.jump_ready)
        {
            player.fat_jump_frames -= 1;
            player.vel.Y += player.jump_accel * dt;
        }
    }
    else if(player.in_air)
    {
        player.jump_ready = false;
    }

    // natural forces
    static int safety_frames = 10;
    if(safety_frames > 0)
    {
        safety_frames--;
    }
    else
    {
        player.vel.Y -= player.gravity * dt;
    }
    if (!is_moving || changed_direction)
    {
        if (player.in_air)
        {
            player.vel.X *= player.horizontal_drag_air;
        }
        else
        {
            player.vel.X *= player.horizontal_drag_ground;
        }
        f32 absX = player.vel.X >= 0 ? player.vel.X : -player.vel.X;
        if (absX < player.epsilon)
        {
            player.vel.X = 0;
        }
    }

// apply limitations
    player.vel.X = HMM_Clamp(-player.max_horizontal_speed, player.vel.X, player.max_horizontal_speed);
    player.vel.Y = HMM_Clamp(-player.max_vertical_speed, player.vel.Y, player.max_vertical_speed);
    
    // set position
    player.pos += player.vel * dt;
    hmm_v3 pos = {player.pos.X, player.pos.Y, 0.0f};
    player.aabb.pos = {player.pos.X, player.pos.Y};
    b32 did_collide = false;
    player.in_air = true; // this gets unset in while loop if player bottom collides
    do
    {
        did_collide = false;
        
        for(int i = 0; i < level.num_aabbs; ++i)
        {
            AABB aabb = level.aabb_list[i];
            // hmm_v2 quad_pos = world_to_screen(aabb.pos);
            // hmm_v3 final_pos = {quad_pos.X, quad_pos.Y, 0.0f};
            // draw_quad(final_pos, aabb.half_dim, {0.0, 0.2, 0.6});
        
            AABB intersection;
            b32 did_intersect = aabb_vs_aabb(player.aabb, aabb, &intersection);
            if(did_intersect)
            {
                did_collide = true;
                int x_dir = (player.aabb.pos.X - intersection.pos.X) > 0 ? 1 : -1;
                int y_dir = (player.aabb.pos.Y - intersection.pos.Y) > 0 ? 1 : -1;

                if(intersection.half_dim.Y <= intersection.half_dim.X)
                {
                    x_dir = 0;
                    player.vel.Y = 0.0f;
                }
                else
                {
                    y_dir = 0;
                    player.vel.X = 0.0f;
                }

                if(y_dir == 1)
                {
                    player.in_air = false;
                    player.fat_jump_frames = player.max_fat_jump_frames;
                    player.jump_ready = true;
                }
            
                player.aabb.pos.X += (intersection.half_dim.X) * x_dir;
                player.aabb.pos.Y += (intersection.half_dim.Y) * y_dir;
            }
        }
    } while(did_collide);

    if(player.in_air)
    {
        if(player.direction == LEFT)
        {
            animation_set(&player.anim_sm, FALLING_LEFT_ANIMATION, true, true);
        }
        else
        {
            animation_set(&player.anim_sm, FALLING_RIGHT_ANIMATION, true, true);
        }
    }

    if(!is_moving && !player.in_air)
    {
        if(player.direction == LEFT)
        {
            animation_set(&player.anim_sm, IDLE_LEFT_ANIMATION, true, true);
        }
        else if(player.direction == RIGHT)
        {
            animation_set(&player.anim_sm, IDLE_RIGHT_ANIMATION, true, true);
        }
    }
    
    player.pos.X = player.aabb.pos.X;
    player.pos.Y = player.aabb.pos.Y;
    if (player.pos.Y < level.death_y)
    {
        respawn_player();
    }
    for (int i = 0; i < num_spikes; i++)
    {
        Spike spike = spikes[i];

        b32 collided = aabb_vs_aabb(player.aabb, spike.aabb);
        if(collided)
        {
            respawn_player();
        }
        int dummy = 1;
    }
    for (int i = 0; i < num_fireplaces; i++)
    {
        Fireplace fireplace = fireplaces[i];
        b32 collided = aabb_vs_aabb(player.aabb, fireplace.aabb);
        if(collided)
        {
            player.spawn_pos = fireplace.pos;
        }
    }
    pos = world_to_screen({player.pos.X, player.pos.Y, 0.0f});
    // draw_quad(pos, player.aabb.half_dim, {1.0, 0.0, 0.0});
    draw_animated_quad(pos, player.scale, &player.anim_sm);
    animation_sm_update(&player.anim_sm);
}
