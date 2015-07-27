/*
 * sdl.h
 *
 *  Created on: Jun 22, 2015
 *      Author: Manuel
 */
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#ifndef INCLUDE_MICHELLE_SDL_H_
#define INCLUDE_MICHELLE_SDL_H_
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
Uint32
sdl_bitmap_get_pixel (SDL_Surface *surface, int x, int y);
void
sdl_bitmap_set_pixel (SDL_Surface *surface, int x, int y, Uint32 pixel);
GLuint
compile_shader_program (GLuint vertexShader, GLuint fragmentShader);
GLuint
compile_shader (char *shader, int type);
GLuint
get_default_shader ();
#endif /* INCLUDE_MICHELLE_SDL_H_ */
