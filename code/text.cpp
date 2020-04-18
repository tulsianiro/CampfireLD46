struct Font
{
    Texture texture;
    stbtt_bakedchar char_data[96];
};

enum Fonts
{
    DEFAULT_FONT,
    LAST_FONT
};

Font font_cache[LAST_FONT];

internal Font InitFontFromTTFData(void *data, int data_size)
{
    Font font = {0};
    unsigned char *temp_bitmap = (unsigned char *)malloc(512 * 512);
    stbtt_BakeFontBitmap((const unsigned char *)data, 0, 32.0, temp_bitmap, 512, 512, 32, 96, font.char_data);
    
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, temp_bitmap);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    font.texture.id = id;
    font.texture.width = 512;
    font.texture.height = 512;
	font.texture.channels = 1;
    
    free(temp_bitmap);
    return font;
}


internal void LoadFont(const char *filename, u32 font_cache_id)
{
    char font_path[128];
	sprintf_s(font_path, "../res/fonts/%s.ttf", filename);
    
    Font result = {0};
    u32 size = 0;
	char *file = read_file(font_path, &size);
    if(file)
    {
        result = InitFontFromTTFData(file, size);
		free(file);
    }

    font_cache[font_cache_id] = result;
}
