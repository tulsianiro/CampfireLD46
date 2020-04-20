#include "collision.cpp"
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

    // follow follow test
    local_persist char buf[128];
    local_persist u32 accum = 0;
    if(!(accum++ % 15))
    {
        sprintf_s(buf, 128, "FPS:%d", (int)(1/dt));
    }
    hmm_v2 corrected_pos = player.pos;
    corrected_pos -= camera.player_offset_y;
    hmm_v2 text_render_pos = ortho_coordinates_to_window(corrected_pos);
    RenderText(&font_cache[DEFAULT_FONT], {1, 0, 0, 1}, text_render_pos, buf);
}
