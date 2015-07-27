/*
 * mrb_rect.c
 *
 *  Created on: 7/6/2015
 *      Author: Chronno
 */
#include <michelle.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <SDL2/SDL.h>

/**
 * mruby instance data free
 */
static void
rect_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      free (p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_rect_data_type =
  { "Rect", rect_free };

/**
 *  Rect mruby Constructor
 */
static mrb_value
initialize (mrb_state* mrb, mrb_value self)
{
  mrb_int x, y, w, h;
  SDL_Rect* rect;
  DATA_TYPE (self) = &mrbal_rect_data_type;
  DATA_PTR (self) = NULL;
  mrb_get_args (mrb, "iiii", &x, &y, &w, &h);
  rect = mrb_malloc (mrb, sizeof(SDL_Rect));
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  DATA_PTR (self) = rect;
  return self;
}

/**
 * Accessor rect x
 */
static mrb_value
accessor_x (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  rect = DATA_PTR(self);
  return mrb_fixnum_value (rect->x);
}

/**
 * Modifier rect x
 */
static mrb_value
modifier_x (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  mrb_int x;
  mrb_get_args (mrb, "i", &x);
  rect = DATA_PTR(self);
  rect->x = x;
  return mrb_fixnum_value (x);
}

/**
 * Accessor rect y
 */
static mrb_value
accessor_y (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  rect = DATA_PTR(self);
  return mrb_fixnum_value (rect->y);
}

/**
 * Modifier rect y
 */
static mrb_value
modifier_y (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  mrb_int y;
  mrb_get_args (mrb, "i", &y);
  rect = DATA_PTR(self);
  rect->y = y;
  return mrb_fixnum_value (y);
}

/**
 * Accessor rect w
 */
static mrb_value
accessor_w (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  rect = DATA_PTR(self);
  return mrb_fixnum_value (rect->w);
}

/**
 * Modifier rect w
 */
static mrb_value
modifier_w (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  mrb_int w;
  mrb_get_args (mrb, "i", &w);
  rect = DATA_PTR(self);
  rect->w = w;
  return mrb_fixnum_value (w);
}

/**
 * Accessor rect h
 */
static mrb_value
accessor_h (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  rect = DATA_PTR(self);
  return mrb_fixnum_value (rect->h);
}

/**
 * Modifier rect h
 */
static mrb_value
modifier_h (mrb_state* mrb, mrb_value self)
{
  SDL_Rect* rect;
  mrb_int h;
  mrb_get_args (mrb, "i", &h);
  rect = DATA_PTR(self);
  rect->w = h;
  return mrb_fixnum_value (h);
}

/**
 * Initialize mruby class
 */
void
mrb_rect_init (mrb_state *mrb)
{
  struct RClass *rect = mrb_define_class (mrb, "Rect", mrb->object_class);
  mrb_define_method (mrb, rect, "initialize", initialize, MRB_ARGS_REQ(4));
  mrb_define_method (mrb, rect, "x", accessor_x, MRB_ARGS_NONE());
  mrb_define_method (mrb, rect, "x=", modifier_x, MRB_ARGS_REQ(1));
  mrb_define_method (mrb, rect, "y", accessor_y, MRB_ARGS_NONE());
  mrb_define_method (mrb, rect, "y=", modifier_y, MRB_ARGS_REQ(1));
  mrb_define_method (mrb, rect, "w", accessor_w, MRB_ARGS_NONE());
  mrb_define_method (mrb, rect, "w=", modifier_w, MRB_ARGS_REQ(1));
  mrb_define_method (mrb, rect, "h", accessor_h, MRB_ARGS_NONE());
  mrb_define_method (mrb, rect, "h=", modifier_h, MRB_ARGS_REQ(1));
  MRB_SET_INSTANCE_TT(rect, MRB_TT_DATA);
}
