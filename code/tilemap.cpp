#define MAX_TILES_IN_MAP 512

struct Tilemap
{
    char tiles[MAX_TILES_IN_MAP];
    Texture tileset;
    int map_width; // in tiles
    int map_height;
};

global Tilemap tilemap;

internal void init_tilemap(const char *map_filename, const char *tilemap_filename, int grid_width, int grid_height)
{
    // read texture
    
	glGenTextures(1, &tilemap.tileset.id);
	glBindTexture(GL_TEXTURE_2D, tilemap.tileset.id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	char image_path[128];
	sprintf_s(image_path, "../res/textures/%s", tilemap_filename);
	unsigned char *data = stbi_load(image_path, &tilemap.tileset.width, &tilemap.tileset.height, &tilemap.tileset.channels, 0);
	if (data)
	{
		if (tilemap.tileset.channels == 3)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tilemap.tileset.width, tilemap.tileset.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (tilemap.tileset.channels == 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tilemap.tileset.width, tilemap.tileset.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

        tilemap.tileset.is_atlas = true;
        tilemap.tileset.rows = tilemap.tileset.width / grid_width;
        tilemap.tileset.cols = tilemap.tileset.height / grid_height;
        tilemap.tileset.grid_width = grid_width;
        tilemap.tileset.grid_height = grid_height;
	}
	else
	{
        // TODO(rohan): Logging
	}
	stbi_image_free(data);

    // TODO(jun): read map from file
    // NOTE(jun): THIS MUST BE READ BOTTOM UP (bottom left 0,0), IN ROWS
    tilemap.map_width = 10; // TODO(jun): infer these from text file
    tilemap.map_height = 10;
}
