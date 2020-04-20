const char *level_file = "../res/levels/level.txt";    

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
    level.level[15] = "........................................";
    level.level[16] = "..............QQQQQ.....................";
    level.level[17] = ".......vwx...DEEEEEF....................";
    level.level[18] = ".......lmn..............................";
    level.level[19] = ".......bcd....!.........................";
    level.level[20] = "..NOP..XYZ..............................";
    level.level[21] = "DEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEF.";
    level.level[22] = ":;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;<.";
    level.level[23] = "011111111111111111111111111111111111112.";
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
                    tile.aabb = init_aabb_from_tile(y, x, level.tilescale, level.world_offset);
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
                hmm_v2 quad_pos = world_to_screen(level.tiles[y][x].aabb.pos);
                hmm_v3 final_pos = {quad_pos.X, quad_pos.Y, 0.0f};
                draw_quad(final_pos, level.tiles[y][x].aabb.half_dim, {0.0, 1.0, 1.0});
            }
        }
    }
}
