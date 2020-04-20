struct AABB
{
    hmm_v2 pos;
    hmm_v2 half_dim;
};

inline b32
aabb_vs_aabb(AABB aabb1, AABB aabb2, AABB *intersection = 0)
{
    hmm_v2 p1 = aabb1.pos;
    hmm_v2 half_size1 = aabb1.half_dim;
    hmm_v2 p2 = aabb2.pos;
    hmm_v2 half_size2 = aabb2.half_dim;

    f32 aabb1_min_x = aabb1.pos.X - aabb1.half_dim.X;
    f32 aabb2_min_x = aabb2.pos.X - aabb2.half_dim.X;
    f32 aabb1_max_x = aabb1.pos.X + aabb1.half_dim.X;
    f32 aabb2_max_x = aabb2.pos.X + aabb2.half_dim.X;
    f32 aabb1_min_y = aabb1.pos.Y - aabb1.half_dim.Y;
    f32 aabb2_min_y = aabb2.pos.Y - aabb2.half_dim.Y;
    f32 aabb1_max_y = aabb1.pos.Y + aabb1.half_dim.Y;
    f32 aabb2_max_y = aabb2.pos.Y + aabb2.half_dim.Y;

    b32 intersecting = true;
    
    if(aabb1_max_x <= aabb2_min_x)
    {
        // aabb1 is to the left of 2
        intersecting = false;
    }
    if(aabb1_min_x >= aabb2_max_x)
    {
        // aabb1 is to the right of 2
        intersecting = false;
    }
    if(aabb1_max_y <= aabb2_min_y)
    {
        // aabb1 is to the bottom of 2
        intersecting = false;
    }
    if(aabb1_min_y >= aabb2_max_y)
    {
        // aabb1 is to the top of 2
        intersecting = false;
    }    
                        
    if(intersection && intersecting)
    {
        f32 minx, miny, maxx, maxy;

        if (aabb2_min_x <= aabb1_min_x)
        {
            minx = aabb1_min_x;
        }
        else
        {
            minx = aabb2_min_x;
        }

        if (aabb2_max_x >= aabb1_max_x)
        {
            maxx = aabb1_max_x;
        }
        else
        {
            maxx = aabb2_max_x;
        }

        if (aabb2_min_y <= aabb1_min_y)
        {
            miny = aabb1_min_y;
        }
        else
        {
            miny = aabb2_min_y;
        }

        if (aabb2_max_y >= aabb1_max_y)
        {
            maxy = aabb1_max_y;
        }
        else
        {
            maxy = aabb2_max_y;
        }

        hmm_v2 half_dim = {(maxx - minx) / 2.0f, (maxy - miny) / 2.0f};
        hmm_v2 intersection_center = {minx + half_dim.X, miny + half_dim.Y};
        
        (*intersection) = {intersection_center, half_dim};
    }
        
    return intersecting;
}

internal AABB init_aabb_from_tile(int row, int col, int scale, hmm_v2 world_offset)
{
    AABB return_aabb;
    return_aabb.pos.X = (col * tilemap.grid_width * scale) - world_offset.X;
    return_aabb.pos.Y = (row * tilemap.grid_height * scale) - world_offset.Y;
    return_aabb.half_dim = {(f32)(tilemap.grid_width) * scale / 2.0f,
                            (f32)(tilemap.grid_height) * scale / 2.0f};
    return return_aabb;
}

// internal AABB init_aabb_from_texture(hmm_v2 pos, u32 texture_cache_id)
// {
//     AABB return_aabb;
//     return_aabb.pos = pos;
//     Texture *texture = &texture_cache[texture_cache_id];
//     return_aabb.half_dim = {(f32)(texture->width / 2), (f32)(texture->height / 2)};
//     return return_aabb;
// }

// internal AABB init_aabb_from_anim(hmm_v2 pos, AnimationSM *anim_sm)
// {
//     AABB return_aabb;
//     return_aabb.pos = pos;
//     Texture texture = animation_cache[anim_sm->animation_id].frames[0];
//     return_aabb.half_dim = {(f32)(texture.width / 2), (f32)(texture.height / 2)};
//     return return_aabb;
// }


