struct AABB
{
    hmm_v2 pos;
    hmm_v2 half_dim;
};

inline b32
aabb_vs_aabb(AABB aabb1, AABB aabb2)
{
    hmm_v2 p1 = aabb1.pos;
    hmm_v2 half_size1 = aabb1.half_dim;
    hmm_v2 p2 = aabb2.pos;
    hmm_v2 half_size2 = aabb2.half_dim;
    
    return p1.Y + half_size1.Y >= p2.Y - half_size2.Y &&
        p1.Y - half_size1.Y <= p2.Y + half_size2.Y &&
        p1.X + half_size1.X >= p2.X - half_size2.X &&
        p1.X - half_size1.X < p2.X +  half_size2.X;
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


