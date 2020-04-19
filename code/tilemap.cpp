#define MAX_TILES_IN_MAP 512

struct Tilemap
{
    char tiles[MAX_TILES_IN_MAP];
    int num_tiles;
    Texture texture;
};

global Tilemap tilemap;

internal void init_tilemap(const char *tilemap_filename, int grid_width, int grid_height)
{
    // read texture
	glGenTextures(1, &tilemap.texture.id);
	glBindTexture(GL_TEXTURE_2D, tilemap.texture.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	char image_path[128];
	sprintf_s(image_path, "../res/textures/%s", tilemap_filename);
	unsigned char *data = stbi_load(image_path, &tilemap.texture.width, &tilemap.texture.height, &tilemap.texture.channels, 0);
	if (data)
	{
		if (tilemap.texture.channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tilemap.texture.width, tilemap.texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (tilemap.texture.channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tilemap.texture.width, tilemap.texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

        tilemap.texture.is_atlas = true;
        tilemap.texture.rows = tilemap.texture.width / grid_width;
        tilemap.texture.cols = tilemap.texture.height / grid_height;
        tilemap.texture.grid_width = grid_width;
        tilemap.texture.grid_height = grid_height;
        tilemap.num_tiles = tilemap.texture.rows * tilemap.texture.cols;
	}
	else
	{
        // TODO(rohan): Logging
	}
	stbi_image_free(data);
}
