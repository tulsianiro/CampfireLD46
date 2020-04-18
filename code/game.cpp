void simulate_frame(Input *input, float dt)
{
    // reset draw jobs
    num_draw_jobs = 0;

    // NOTE: draw queue should be filled by object updates
    // TEST OBJECT 1
    Animation obj1_animation = animation_cache[TEST_ANIMATION];
    static f32 obj1_time = 0; // this will be a member of the game object
    obj1_time += dt;
    hmm_v3 obj1_pos = {0.0f, 0.0f, 0.0f};
    int obj1_scale = 1;
    DrawJob obj1_drawjob = {};
    obj1_drawjob.pos = obj1_pos;
    obj1_drawjob.scale = obj1_scale;
    obj1_drawjob.texture = get_animation_frame(obj1_animation, &obj1_time);
    draw_queue[num_draw_jobs++] = obj1_drawjob;

    // TEST OBJECT 2
    Animation obj2_animation = animation_cache[TEST_ANIMATION];
    static f32 obj2_time = 0; // this will be a member of the game object
    obj2_time += dt;
    hmm_v3 obj2_pos = {50.0f, 50.0f, 0.0f};
    int obj2_scale = 1;
    DrawJob obj2_drawjob = {};
    obj2_drawjob.pos = obj2_pos;
    obj2_drawjob.scale = obj2_scale;
    obj2_drawjob.texture = get_animation_frame(obj2_animation, &obj2_time);
    draw_queue[num_draw_jobs++] = obj2_drawjob;

    // draw_animated_quad({0.0f, 0.0f, 0.0f}, 1, animation, &object_time);
    // draw_textured_quad({0.0f, 0.0f, 0.0f}, 1, animation.frames[target_frame]);
    // draw_quad({0.0f, 0.0f, 0.0f}, {320.0f, 180.0f}, {1.0f, 1.0f, 1.0f});

    for (int i = 0; i < num_draw_jobs; i++)
    {
        DrawJob draw_job = draw_queue[i];
        // TODO(jun): add different types of draws?
        draw_textured_quad(draw_job.pos, draw_job.scale, draw_job.texture);
    }
}
