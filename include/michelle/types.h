/*
 * types.h
 *
 *  Created on: Jun 19, 2015
 *      Author: Manuel
 */

#ifndef INCLUDE_MICHELLE_TYPES_H_
#define INCLUDE_MICHELLE_TYPES_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <mruby.h>

typedef struct MCL_window
{
  SDL_Window* window;
  SDL_GLContext* context;
  SDL_Renderer* renderer;
  SDL_Event event;
} MCL_Window;

#endif /* INCLUDE_MICHELLE_TYPES_H_ */
