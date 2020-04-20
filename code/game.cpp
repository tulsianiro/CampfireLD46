#include "collision.cpp"

#define MAX_ROWS 64
#define MAX_COLS 128


struct Tile
{
    b32 blocking;
    int index;
    AABB aabb;
};

struct Spike
{
    hmm_v2 pos;
    AABB aabb;
};

global Spike spikes[512];
global int num_spikes;

#define MAX_AABBS 512
struct Level
{
    int death_y;
    char *level[MAX_ROWS];
    AABB aabb_list[MAX_AABBS];
    int num_aabbs;
    // 0, 0 is top left
    // max_col, max_row is bottom-right
    u32 max_col;
    u32 max_row;
    int tilescale;
    hmm_v2 world_offset;
    Tile tiles[MAX_COLS][MAX_ROWS];
};

global Level level;

#include "player.cpp"
#include "level.cpp"

void simulate_frame(Input *input)
{

    // follow cam
    camera.x = player.pos.X + camera.player_offset_x;
    camera.y = player.pos.Y + camera.player_offset_y;

    // update objects
    level_update_and_render();
    player_update_and_render();

    // follow player text test
    local_persist char buf[128];
    local_persist u32 accum = 0;
    if(!(accum++ % 15))
    {
        sprintf_s(buf, 128, "FPS:%d", (int)(1/dt));
    }

    RenderText(&font_cache[DEFAULT_FONT], {1, 0, 0, 1}, {0.01f, 0.05f}, buf);
}
