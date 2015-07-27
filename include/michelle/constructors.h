/*
 * constructors.h
 *
 *  Created on: Jun 23, 2015
 *      Author: Manuel
 */

#ifndef INCLUDE_MICHELLE_CONSTRUCTORS_H_
#define INCLUDE_MICHELLE_CONSTRUCTORS_H_
#include <mruby.h>
#include <SDL2/SDL.h>

mrb_value
MCL_color_new (mrb_state *mrb, mrb_int r, mrb_int g, mrb_int b, mrb_int a);

mrb_value
MCL_color_transform (mrb_state *mrb, SDL_Color *color);

mrb_value
MCL_rect_new (mrb_state *mrb, mrb_int x, mrb_int y, mrb_int w, mrb_int h);

mrb_value
MCL_rect_transform (mrb_state *mrb, SDL_Rect *rect);

mrb_value
MCL_viewport_new (mrb_state *mrb, int x, int y, int w, int h);
#endif /* INCLUDE_MICHELLE_CONSTRUCTORS_H_ */
