struct Texture
{
	i32 width;
	i32 height;
	i32 channels;
	u32 id;
    b32 is_atlas;
    i32 rows;
    i32 cols;
    i32 grid_width;
    i32 grid_height;
};

global Texture texture_cache[LAST_TEXTURE];

internal uint32 init_texture(const char *image_name, uint32 texture_cache_id,
                             u32 grid_width = -1, u32 grid_height = -1)
{
	Texture texture;
    
	glGenTextures(1, &texture.id);
	glBindTexture(GL_TEXTURE_2D, texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

        if(grid_width != -1 && grid_height != -1)
        {
            texture.is_atlas = true;
            texture.rows = texture.width / grid_width;
            texture.cols = texture.height / grid_height;
            texture.grid_width = grid_width;
            texture.grid_height = grid_height;
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

internal void texture_set(GLenum tex_unit, Texture texture)
{
	glActiveTexture(tex_unit);
	glBindTexture(GL_TEXTURE_2D, texture.id);
}
