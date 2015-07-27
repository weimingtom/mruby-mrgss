/*
 * constructors.c
 *
 *  Created on: Jun 23, 2015
 *      Author: Manuel
 */

#include <michelle/constructors.h>
#include <mruby.h>
#include <SDL2/SDL.h>

mrb_value
MCL_rect_new (mrb_state *mrb, mrb_int x, mrb_int y, mrb_int w, mrb_int h)
{
  mrb_value constructorparams[4];
  constructorparams[0] = mrb_fixnum_value (x);
  constructorparams[1] = mrb_fixnum_value (y);
  constructorparams[2] = mrb_fixnum_value (w);
  constructorparams[3] = mrb_fixnum_value (h);
  return mrb_obj_new (mrb, mrb_class_get (mrb, "Rect"), 4, constructorparams);
}

mrb_value
MCL_rect_transform (mrb_state *mrb, SDL_Rect *rect)
{
  return MCL_rect_new (mrb, rect->x, rect->y, rect->w, rect->h);
}


mrb_value
MCL_color_new (mrb_state *mrb, mrb_int r, mrb_int g, mrb_int b, mrb_int a)
{
  mrb_value constructorparams[4];
  constructorparams[0] = mrb_fixnum_value (r);
  constructorparams[1] = mrb_fixnum_value (g);
  constructorparams[2] = mrb_fixnum_value (b);
  constructorparams[3] = mrb_fixnum_value (a);
  return mrb_obj_new (mrb, mrb_class_get (mrb, "Color"), 4, constructorparams);
}

mrb_value
MCL_color_transform (mrb_state *mrb, SDL_Color *color)
{
  return MCL_color_new (mrb, color->r, color->g, color->b, color->a);
}

mrb_value
MCL_viewport_new (mrb_state *mrb, int x, int y, int w, int h)
{
  mrb_value rect;
  mrb_value constructorparams[4];
  constructorparams[0] = mrb_fixnum_value (x);
  constructorparams[1] = mrb_fixnum_value (y);
  constructorparams[2] = mrb_fixnum_value (w);
  constructorparams[3] = mrb_fixnum_value (h);
  rect = mrb_obj_new (mrb, mrb_class_get (mrb, "Rect"), 4, constructorparams);
  return mrb_obj_new (mrb, mrb_class_get (mrb, "Viewport"), 1, &rect);
}
