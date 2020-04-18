struct Texture
{
	i32 width;
	i32 height;
	i32 channels;
	u32 id;
};

global Texture texture_cache[LAST_TEXTURE];

#define MAX_FRAMES 16
struct Animation
{
    i32 num_frames;
    f32 time_length;
    Texture frames[MAX_FRAMES];
};

global Animation animation_cache[LAST_ANIMATION];

internal void init_animation(const char *animation_name, uint32 animation_cache_id, i32 num_frames, f32 time_length)
{
    Animation animation;
    animation.num_frames = num_frames;
    animation.time_length = time_length;
    for (int i = 0; i < num_frames; i++)
    {
        Texture texture;
        glGenTextures(1, &texture.id);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        char image_path[128];
        sprintf_s(image_path, "../res/textures/%s/%d.png", animation_name, i);
        unsigned char *data = stbi_load(image_path, &texture.width, &texture.height, &texture.channels, 0);
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
        }
        else
        {
            // TODO(rohan): Logging
        }
        
        stbi_image_free(data);
        animation.frames[i] = texture;
    }
    animation_cache[animation_cache_id] = animation;

}

internal uint32 init_texture(const char *image_name, uint32 texture_cache_id)
{
	Texture texture;
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	char image_path[128];
	sprintf_s(image_path, "../res/textures/%s", image_name);
	unsigned char *data = stbi_load(image_path, &texture.width, &texture.height, &texture.channels, 0);
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
	}
	else
	{
        // TODO(rohan): Logging
	}

	stbi_image_free(data);
	texture_cache[texture_cache_id] = texture;
	return texture.id;
}

internal void texture_use(GLenum tex_unit, Texture texture)
{
	glActiveTexture(tex_unit);
	glBindTexture(GL_TEXTURE_2D, texture.id);
}
