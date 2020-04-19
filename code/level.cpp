const char *level_file = "../res/levels/level.txt";    
#define MAX_ROWS 64

struct Level
{
    char *level[MAX_ROWS];

    // 0, 0 is top left
    // max_col, max_row is bottom-right
    u32 max_col;
    u32 max_row;
};

global Level level;

void init_level()
{
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
    level.level[15] = "........................................";
    level.level[16] = "........................................";
    level.level[17] = ".............########...................";
    level.level[18] = "........................................";
    level.level[19] = "....P...................................";
    level.level[20] = "########################################";
    level.level[21] = "........................................";
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
            hmm_v2 world_offset = {game_window.base_width / 2.0f, game_window.base_height / 2.0f};
            f32 world_x = x * 16.0f - world_offset.X;
            f32 world_y = y * 16.0f - world_offset.Y;
            
            switch(c)
            {
                case 'P':
                {
                    player.pos = {world_x, world_y};
                } break;
            }

        }
    }
}
