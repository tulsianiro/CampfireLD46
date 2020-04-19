#define MAX_TILES_IN_MAP 512

struct Tilemap
{
    char tiles[MAX_TILES_IN_MAP];
    int num_tiles;
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

    // read map from file
    // NOTE(jun): THIS MUST BE READ BOTTOM UP (bottom left 0,0), IN ROWS
    // NOTE(jun): using stdio for now and you can't stop me
    #include <stdio.h>
 
    FILE *fp;
    char map_path[128];
	sprintf_s(map_path, "../res/%s", map_filename);
    fp = fopen(map_path, "r");
    char str[MAX_TILES_IN_MAP];

    // read width and height
    fgets(str, 5, fp);
    tilemap.map_width = strtoi(str);
    fgets(str, 5, fp);
    tilemap.map_height = strtoi(str);
    tilemap.num_tiles = tilemap.map_width * tilemap.map_height;
    int map_size_1d = tilemap.num_tiles;
    // read line by line, placing in tilemap backwards
    for (int i = 0 ; i < tilemap.map_height; i++)
    {
        fgets(str, MAX_TILES_IN_MAP, fp);
        int index = 0;
        map_size_1d -= tilemap.map_width;
        for(int j = map_size_1d; j < map_size_1d + tilemap.map_width; j++)
        {
            tilemap.tiles[j] = str[index++];
        }
    }
    fclose(fp);
}
