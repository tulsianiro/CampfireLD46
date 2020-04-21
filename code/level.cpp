const char *level_file = "../res/levels/level.txt";    

struct AABB_Specifier
{
    u32 x0;
    u32 x1;
    u32 y0;
    u32 y1;
};

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
    
    level.max_row = 0;
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = "........................................................vwx......................";
    level.level[level.max_row++] = "........................................................lmn......................";
    level.level[level.max_row++] = ".............#...........................I..............bcd......................";
    level.level[level.max_row++] = "............>?@...............I........>??@......QQQQQOPXYZQQQ...................";
    level.level[level.max_row++] = "...................>@........>?@................>?????????????@..................";
    level.level[level.max_row++] = "..A..........................456.................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = "..............II.................................................................";
    level.level[level.max_row++] = "......A......>??@................................................................";
    level.level[level.max_row++] = "......7....................QQ....................................................";
    level.level[level.max_row++] = "...................A.......>@....................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = ".................................................................................";
    level.level[level.max_row++] = ".......vwx...........vwx..........QQ.............................................";
    level.level[level.max_row++] = ".......lmn...........lmn..........>@.............................................";
    level.level[level.max_row++] = ".....!.bcd...........bcd.........................................................";
    level.level[level.max_row++] = "..NOP#.XYZ.....II.NNPXYZO..#.QQ..................................................";
    level.level[level.max_row++] = "DEEEEEEEEEEEEEEEEEEEEEEEEEEEEEF..................................................";
    level.level[level.max_row++] = ":;;;;;;;;;;;;;;;;;;;;;;;;;;;;;<..................................................";
    level.level[level.max_row++] = "0111111111111111111111111111112..................................................";
    level.level[level.max_row++] = ".................................................................................";

    level.death_y = -level.world_offset.Y - 300;
    AABB_Specifier aabb_specifiers[] =
    {
        {0, 31, 1, 4},
        {33, 37, 6, 7},
        {26, 30, 10, 11},
        {19, 20, 10, 11},
        {13, 17, 12, 13},
        {6, 7, 12, 13},
        {19, 21, 17, 18},
        {2, 3, 16, 17},
        {12, 15, 18, 19},
        {29, 32, 17, 18},
        {39, 43, 18, 19},
        {48, 63, 17, 18},
        
        
        
        
    };
    level.num_aabbs = sizeof(aabb_specifiers) / sizeof(AABB_Specifier);

    level.max_row--;
    char *curr_char = level.level[0];
    int count = -1;
    while(*curr_char != '\0') {count++; curr_char++;}
    level.max_col = count;

    for(int i = 0; i < level.num_aabbs; ++i)
    {
        AABB_Specifier aabb_spec = aabb_specifiers[i];
        f32 x0_world = aabb_spec.x0 * tilemap.grid_width * level.tilescale - level.world_offset.X -  (tilemap.grid_width * level.tilescale / 2.0f);
        f32 x1_world =  aabb_spec.x1 * tilemap.grid_width * level.tilescale - level.world_offset.X - (tilemap.grid_width * level.tilescale / 2.0f);
        f32 y0_world = aabb_spec.y0 * tilemap.grid_height * level.tilescale - level.world_offset.Y - (tilemap.grid_height * level.tilescale / 2.0f);
        f32 y1_world = aabb_spec.y1 * tilemap.grid_height * level.tilescale - level.world_offset.Y - (tilemap.grid_height * level.tilescale / 2.0f);
        hmm_v2 half_dim = {(x1_world - x0_world) / 2.0f, (y1_world - y0_world) / 2.0f};
        hmm_v2 pos = {x0_world + half_dim.X, y0_world + half_dim.Y};
        AABB tempAABB = {pos, half_dim};
        level.aabb_list[i] = tempAABB;
    }
    
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
                case '#':
                {
                    Fireplace fireplace;
                    fireplace.pos.X = x * 32.0f - level.world_offset.X;
                    fireplace.pos.Y = y * 32.0f - level.world_offset.Y;
                    AABB aabb;
                    aabb.pos = fireplace.pos;
                    aabb.half_dim = {32.0f, 32.0f};
                    fireplace.aabb = aabb;
                    fireplace.anim_sm = init_animation_sm(FIREPLACE_ANIMATION, true, true);
                    // fireplace.anim_sm.animation_timer = (f32)num_fireplaces / 3.0f;
                    
                    fireplaces[num_fireplaces++] = fireplace;
                } break;
                case '4':
                case '5':
                case '6':
                case '7':
                case 'H':
                case 'I':
                case 'J':
                case 'K':
                case 'R':
                case 'U':
                case '\\':
                case '_':
                case 'f':
                case 'i':
                case 'p':
                case 's':
                {
                    // spike
                    Tile tile;
                    tile.blocking = false;
                    tile.index = (int)c - '0';
                    level.tiles[y][x] = tile;
                    Spike spike;
                    spike.pos.X = x * 32.0f - level.world_offset.X;
                    spike.pos.Y = y * 32.0f - level.world_offset.Y;
                    AABB aabb;
                    aabb.pos = spike.pos;
                    aabb.half_dim = {11.0f, 11.0f};
                    spike.aabb = aabb;
                    spikes[num_spikes++] = spike;
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
                if(pos.X >= -320 && pos.X <= 1690 && pos.Y >= -180 && pos.Y <= 620)
                {
                    pos = world_to_screen(pos);
                    draw_tilemapped_quad(pos, level.tilescale, level.tiles[y][x].index, player.pos, player.light_quantity);
                }
            }
        }
    }
}
