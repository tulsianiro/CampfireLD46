const char *level_file = "../res/levels/level.txt";    
#define MAX_ROWS 64
#define MAX_COLS 128

struct Tile
{
    b32 blocking;
    int index;
};

struct Level
{
    char *level[MAX_ROWS];

    // 0, 0 is top left
    // max_col, max_row is bottom-right
    u32 max_col;
    u32 max_row;
    int tilescale;
    hmm_v2 world_offset;
    Tile tiles[MAX_COLS][MAX_ROWS];
};

global Level level;

void init_level()
{
    for (int i = 0; i < MAX_COLS; i++)
    {
        for (int j = 0; j < MAX_ROWS; j++)
        {
            level.tiles[i][j].index = -1;
        }
    }
    level.world_offset = {game_window.base_width / 2.0f, game_window.base_height / 2.0f};
    level.tilescale = 2;
    
    level.level[0]  = "........................................";
    level.level[1]  = "........................................";
    level.level[2]  = "........................................";
    level.level[3]  = "........................................";
    level.level[4]  = "........................................";
    level.level[5]  = "........................................";
    level.level[6]  = "........................................";
    level.level[7]  = "........................................";
    level.level[8]  = "........................................";
    level.level[9]  = "........................................";
    level.level[10] = "........................................";
    level.level[11] = "........................................";
    level.level[12] = "........................................";
    level.level[13] = "........................................";
    level.level[14] = "........................................";
    level.level[15] = "...............!........................";
    level.level[16] = "..............QQQQQ.....................";
    level.level[17] = ".......vwx...DEEEEEF....................";
    level.level[18] = ".......lmn..............................";
    level.level[19] = ".......bcd..............................";
    level.level[20] = "..NOP..XYZ..............................";
    level.level[21] = "DEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEF.";
    level.level[22] = "........................................";
    level.level[23] = "........................................";
    level.level[24] = "........................................";

    char *curr_char = level.level[0];
    int count = -1;
    while(*curr_char != '\0')
    {
        count++;
        curr_char++;
    }
    
    level.max_col = count;
    level.max_row = 24;

    for(int y = 0; y < level.max_row; ++y)
    {
        for(int x = 0; x < level.max_col; ++x)
        {
            char c = level.level[level.max_row - y][x];
            f32 world_x = x * 16.0f * level.tilescale - level.world_offset.X;
            f32 world_y = y * 16.0f * level.tilescale - level.world_offset.Y;
            
            switch(c)
            {
                case '.':
                {

                } break;
                case '!':
                {
                    player.pos = {world_x, world_y};
                } break;
                default:
                {
                    Tile tile;
                    tile.blocking = true;
                    tile.index = (int)c - '0';
                    level.tiles[y][x] = tile;
                } break;   
            }
        }
    }
}

internal void level_update_and_render()
{
    // render
    for (int y = 0; y < level.max_row; y++)
    {
        for (int x = 0; x < level.max_col; x++)
        {
            if (level.tiles[y][x].index != -1)
            {
                f32 world_x = x * 16.0f * level.tilescale - level.world_offset.X;
                f32 world_y = y * 16.0f * level.tilescale - level.world_offset.Y;
                hmm_v3 pos = {world_x, world_y, 0.0f};
                pos = world_to_screen(pos);
                draw_tilemapped_quad(pos, level.tilescale, level.tiles[y][x].index);
            }
        }
    }
}
