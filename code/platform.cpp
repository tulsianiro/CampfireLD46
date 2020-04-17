#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#define HANDMADE_MATH_IMPLEMENTATION
#include "handmade_math.h"

#include <stdio.h>
#include <malloc.h>

#include "utils.cpp"
#include "platform.h"
#include "draw.cpp"
#include "game.cpp"

internal void
framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height)
{
    game_window.width = width;
    game_window.height = height;

    // TODO(rohan & jun): Do stretching black bar BS
    int horizontal_offset = 0;
    int vertical_offset = 0;
    f32 ideal_ratio = 1.7777f;
    f32 leeway = 0.03f;
    f32 true_ratio = (f32) width / (f32) height;
    if (true_ratio > ideal_ratio + leeway)
    {
        int ideal_width = ideal_ratio * height;
        horizontal_offset = (width - ideal_width) / 2;
    }
    
    else if (true_ratio < ideal_ratio - leeway)
    {
        int ideal_height = width / ideal_ratio;
        vertical_offset = (height - ideal_height) / 2;
    }
    printf("hori: %d\n",horizontal_offset);
    printf("verti: %d\n",vertical_offset);
    game_window.horizontal_offset = horizontal_offset;
    game_window.vertical_offset = vertical_offset;
	glViewport(horizontal_offset, vertical_offset, width - 2 * horizontal_offset, height - 2 * vertical_offset);
}

internal void update_input_state()
{
    glfwPollEvents();
    
    Button *buttons = input.buttons;
	for (i32 i = 0; i < KEY_LAST; ++i) 
	{
        buttons[i].changed = false;
        b32 key_state = glfwGetKey(game_window.window, i);
        
		if (key_state == GLFW_PRESS) 
		{
            if(!buttons[i].is_down)
            {
                buttons[i].changed = true;
            }

            buttons[i].is_down = true;
		}
		else 
		{
            if(buttons[i].is_down)
            {
                buttons[i].changed = true;
            }

            buttons[i].is_down = false;
        }
	}

    glfwGetCursorPos(game_window.window, &input.mouse.mouse_x, &input.mouse.mouse_y);
	i32 left_mouse_button_state = glfwGetMouseButton(game_window.window, GLFW_MOUSE_BUTTON_LEFT);
	i32 right_mouse_button_state = glfwGetMouseButton(game_window.window, GLFW_MOUSE_BUTTON_RIGHT);

    Mouse *mouse = &input.mouse;
    mouse->left_changed = false;
    mouse->right_changed = false;
    if(left_mouse_button_state == GLFW_PRESS)
    {
        if(!mouse->left_is_down)
        {
            mouse->left_changed = true;
        }

        mouse->left_is_down = true;
    }
    else
    {
        if(mouse->left_is_down)
        {
            mouse->left_changed = true;
        }

        mouse->left_is_down = false;
    }

    
    if(right_mouse_button_state == GLFW_PRESS)
    {
        if(!mouse->right_is_down)
        {
            mouse->right_changed = true;
        }

        mouse->right_is_down = true;
    }
    else
    {
        if(mouse->right_is_down)
        {
            mouse->right_changed = true;
        }

        mouse->right_is_down = false;
    }
}

int main()
{
    game_window.width = 1280;
    game_window.height = 720;
    game_window.base_width = 640;
    game_window.base_height = 360;
    
    glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	game_window.window = glfwCreateWindow(game_window.width, game_window.height,
                                          "Ludum", NULL, NULL);
	if (game_window.window == NULL)
	{
        // TODO(rohan): Logging
        return -1;
	}

	glfwMakeContextCurrent(game_window.window);
	glfwSetFramebufferSizeCallback(game_window.window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
        // TODO(rohan): Logging
		return -1;
	}

    f64 last_time = glfwGetTime();
    f32 last_dt = 0.01666f;

    glfwSwapInterval(1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    draw_init();
    
    while(!glfwWindowShouldClose(game_window.window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        update_input_state();
        simulate_frame(&input, last_dt);
        
        glfwSwapBuffers(game_window.window);
        
        f64 now_time = glfwGetTime();
        last_dt = (f32)(now_time - last_time);
        last_time = now_time;
    }
    
    return 0;
}
