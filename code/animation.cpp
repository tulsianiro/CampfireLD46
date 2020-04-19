#define MAX_FRAMES 16
struct Animation
{
    i32 num_frames;
    f32 time_length;
    Texture texture_atlas;
    u32 anim_begin_index;
    u32 anim_end_index;
};

global Animation animation_cache[LAST_ANIMATION];

struct AnimationSM
{
    u32 animation_id;
    f32 animation_timer;
    b32 looping;
    b32 playing;
};

internal AnimationSM
init_animation_sm(u32 animation_cache_id = LAST_ANIMATION, b32 playing = true,
                  b32 looping = false)
{
    AnimationSM return_sm;
    return_sm.animation_id = animation_cache_id;
    return_sm.animation_timer = 0.0f;
    return_sm.looping = looping;
    return_sm.playing = playing;
    if(return_sm.animation_id == LAST_ANIMATION)
    {
        return_sm.playing = false;
    }
    
    return return_sm;
}

internal u32 get_animation_frame(Animation *animation,
                                     f32 object_animation_timer)
{    
    f32 howfarin = object_animation_timer / animation->time_length;
    i32 target_frame = (i32)HMM_Lerp(0, howfarin, animation->num_frames);
    return target_frame + animation->anim_begin_index;
}

inline void
animation_set(AnimationSM *animation_sm, u32 animation_cache_id = LAST_ANIMATION,
              b32 playing = true, b32 looping = false)
{
    if(animation_cache_id == animation_sm->animation_id && animation_sm->playing)
    {
        animation_sm->looping = looping;
        return;
    }
    
    animation_sm->animation_id = animation_cache_id;
    animation_sm->animation_timer = 0.0f;
    animation_sm->looping = looping;
    animation_sm->playing = playing;
    if(animation_sm->animation_id == LAST_ANIMATION)
    {
        animation_sm->playing = false;
    }

}

internal void
animation_sm_update(AnimationSM *animation_sm)
{
    Animation *animation = &animation_cache[animation_sm->animation_id];
    if(animation_sm->playing)
    {
        animation_sm->animation_timer += dt;
        while(animation_sm->animation_timer >= animation->time_length)
        {
            if(animation_sm->looping)
            {
                animation_sm->animation_timer -= animation->time_length;
            }
            else
            {
                animation_sm->playing = false;
                animation_sm->animation_timer = 0.0f;
                break;
            }
        }
    }
}

internal void init_animation(const char *animation_name, uint32 animation_cache_id,
                             i32 start_index, i32 end_index, hmm_v2 grid_dim, f32 time_length)
{
    Animation animation;
    animation.num_frames = (end_index - start_index) + 1;
    animation.time_length = time_length;
    animation.anim_begin_index = start_index;
    animation.anim_end_index = end_index;
    for (int i = 0; i < animation.num_frames; i++)
    {
        Texture texture;
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        char image_path[128];
        sprintf_s(image_path, "../res/textures/%s", animation_name, i);
        unsigned char *data = stbi_load(image_path, &texture.width, &texture.height,
                                        &texture.channels, 0);
        if (data)
        {
            if (texture.channels == 3)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else if (texture.channels == 4)
            {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }

            texture.is_atlas = true;
            texture.rows = texture.height / grid_dim.Y;
            texture.cols = texture.width / grid_dim.X;
            texture.grid_width = grid_dim.X;
            texture.grid_height = grid_dim.Y;
        }
        else
        {
            // TODO(rohan): Logging
        }
        
        stbi_image_free(data);
        animation.texture_atlas = texture;
    }

    animation_cache[animation_cache_id] = animation;
}
