#define GL_SILENCE_DEPRECATION
#include "./MLX42/include/MLX42/MLX42.h"
#include <OpenGL/OpenGL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_types/_int32_t.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#define WIDTH 800
#define HEIGHT 600


typedef struct s_gl_init
{
    mlx_t     *mlx;
    
    int32_t     win_width;
    int32_t     win_height;

    float   time_value;

    const char    *vert_shader;
    const char    *frag_shader;
    
    GLuint  vert;
    GLuint  frag;
    GLuint  vao;
    GLuint  vbo;
    GLuint  program;
} t_gl_init;

// Utility to load file into string
char* load_file_to_string(const char* filename) {
    FILE* f = fopen(filename, "rb");
    if (!f) 
        return NULL;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);
    char* buf = malloc(len + 1);
    fread(buf, 1, len, f);
    buf[len] = '\0';
    fclose(f);
    return buf;
}

// Function to handle key presses
void key_hook(mlx_key_data_t keydata, void* param)
{
    if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
    {   
        mlx_t* mlx = param;
        mlx_close_window(mlx);
    }
}

/* void resize_hook(int32_t width, int32_t height, void* param)
{
    t_gl_init *gl_init = (t_gl_init*)param;

    glViewport(0, 0, width, height);
    glFlush();
    glFinish();
    gl_init->win_width = width;
    gl_init->win_height = height;
    
} */

void    gl_cleanup(t_gl_init *gl_init)
{
    glDeleteProgram(gl_init->program);
    glDeleteShader(gl_init->vert);
    glDeleteShader(gl_init->frag);
    glDeleteBuffers(1, &gl_init->vbo);
    glDeleteVertexArrays(1, &gl_init->vao);
    free((void*)gl_init->vert_shader);
    free((void*)gl_init->frag_shader);
}

void    main_loop(void *param)
{
    t_gl_init   *gl_init = (t_gl_init *)param;

    int32_t     mouse_x;
    int32_t     mouse_y;

    float       mouse_x_norm;
    float       mouse_y_norm;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glViewport(0, 0, viewport[2], viewport[3]);
    float fb_width = (float)viewport[2];
    float fb_height = (float)viewport[3];
    

    gl_init->time_value = mlx_get_time();

    mlx_get_mouse_pos(gl_init->mlx, &mouse_x, &mouse_y);
    mouse_x_norm = mouse_x / fb_width;
    mouse_y_norm = mouse_y / fb_height;

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(gl_init->program);

    GLint loc_time = glGetUniformLocation(gl_init->program, "time");
    GLint loc_resolution = glGetUniformLocation(gl_init->program, "resolution");
    GLint loc_mouse = glGetUniformLocation(gl_init->program, "mouse");

    glUniform1f(loc_time, gl_init->time_value);
    glUniform2f(loc_mouse, mouse_x_norm, mouse_y_norm);
    glUniform2f(loc_resolution, fb_width, fb_height);

    glBindVertexArray(gl_init->vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    printf("Window: %d x %d, Framebuffer: %d x %d\n",
       gl_init->win_width, gl_init->win_height, viewport[2], viewport[3]);
}

void    quad_setup(t_gl_init *gl_init)
{
float quad[] = {
    -1.0f, -1.0f, // lower left
     1.0f, -1.0f, // lower right
    -1.0f,  1.0f, // upper left
     1.0f,  1.0f  // upper right
};

    glGenVertexArrays(1, &gl_init->vao);
    glBindVertexArray(gl_init->vao);
    glGenBuffers(1, &gl_init->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_init->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

int shader_setup(t_gl_init *gl_init)
{
    gl_init->vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(gl_init->vert, 1, &gl_init->vert_shader, NULL);
    glCompileShader(gl_init->vert);

    // Check for errors
    GLint success;
    glGetShaderiv(gl_init->vert, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(gl_init->vert, 512, NULL, log);
        printf("Vertex shader error: %s\n", log);
        return 1;
    }

    gl_init->frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(gl_init->frag, 1, &gl_init->frag_shader, NULL);
    glCompileShader(gl_init->frag);
    glGetShaderiv(gl_init->frag, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetShaderInfoLog(gl_init->frag, 512, NULL, log);
        printf("Fragment shader error: %s\n", log);
        return 1;
    }

    gl_init->program = glCreateProgram();
    glAttachShader(gl_init->program, gl_init->vert);
    glAttachShader(gl_init->program, gl_init->frag);
    
    glLinkProgram(gl_init->program);
    glGetProgramiv(gl_init->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[512];
        glGetProgramInfoLog(gl_init->program, 512, NULL, log);
        printf("Shader program link error: %s\n", log);
        return 1;
    }
    return (0);
}

int    load_shaders(t_gl_init *gl_init)
{  
    gl_init->vert_shader = load_file_to_string("./shaders/animated/raytrace.vert");
    gl_init->frag_shader = load_file_to_string("./shaders/animated/raytrace.frag");
    if (!gl_init->vert_shader || !gl_init->frag_shader)
    {
        puts("Shader file not found!"); 
        return 1;
    }
    return (0);
}

int32_t main(void)
{
    t_gl_init gl_init = {0};


    // 1. Initialize MLX42
    if ((gl_init.mlx = mlx_init(WIDTH, HEIGHT, "Ray Tracing Shader", true)) == NULL)
    {
        puts(mlx_strerror(mlx_errno));
        return (EXIT_FAILURE);
    }

    // init values
    gl_init.win_width = gl_init.mlx->width;
    gl_init.win_height = gl_init.mlx->height;

    // Set up key hooks
    mlx_key_hook(gl_init.mlx, key_hook, gl_init.mlx);
    // mlx_resize_hook(gl_init.mlx, resize_hook, &gl_init);
    
    
    // Load shaders from files
    if(load_shaders(&gl_init))
        return(1);

    // shader setup
    if(shader_setup(&gl_init))
        return (1);

    // quad setup
    quad_setup(&gl_init);

    // main loop
    mlx_loop_hook(gl_init.mlx, main_loop, &gl_init);
    mlx_loop(gl_init.mlx);


    //cleanup
    gl_cleanup(&gl_init);

    return (0);
}