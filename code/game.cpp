#include "collision.cpp"
#include "player.cpp"
#include "level.cpp"

global b32 game_initialized = false;

void simulate_frame(Input *input)
{
    if(!game_initialized)
    {
        play_sound(TEST_AUDIO);
        change_volume(TEST_AUDIO, 0.1f);
        player = init_player({0.0f, 0.0f});
        init_camera(0, 100);
        init_level();
        game_initialized = true;
    }

    camera.x = player.pos.X;
    camera.y = player.pos.Y + 100;
    
    level_update_and_render();
    player_update_and_render();

    local_persist char buf[128];
    local_persist u32 accum = 0;
    if(!(accum++ % 15))
    {
        sprintf_s(buf, 128, "FPS:%d", (int)(1/dt));
    }

    hmm_v2 text_render_pos = ortho_coordinates_to_window(world_to_screen(player.pos));
    RenderText(&font_cache[DEFAULT_FONT], {1, 0, 0, 1}, text_render_pos, buf);
}
