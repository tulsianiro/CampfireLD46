void simulate_frame(Input *input, float dt)
{
    Animation animation = animation_cache[TEST_ANIMATION];
    static f32 object_time = 0; // this will be a member of the game object
    object_time += dt;

    draw_animated_quad({0.0f, 0.0f, 0.0f}, 1, animation, &object_time);
    // draw_textured_quad({0.0f, 0.0f, 0.0f}, 1, animation.frames[target_frame]);
    // draw_quad({0.0f, 0.0f, 0.0f}, {320.0f, 180.0f}, {1.0f, 1.0f, 1.0f});
}
