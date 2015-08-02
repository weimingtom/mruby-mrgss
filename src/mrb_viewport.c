/*
 * mrb_viewport.c
 *
 *  Created on: Jun 21, 2015
 *      Author: Manuel
 */
#include <mruby.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <michelle.h>
#include <michelle/types.h>
#include <michelle/constructors.h>
#include <SDL2/SDL.h>

static void
viewport_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      free (p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_viewport_data_type =
  { "Viewport", viewport_free };

static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  mrb_value mrbrect;
  MCL_Viewport *viewport;
  DATA_TYPE (self) = &mrbal_viewport_data_type;
  DATA_PTR (self) = NULL;
  mrb_get_args (mrb, "o", &mrbrect);
  viewport = mrb_malloc (mrb, sizeof(MCL_Viewport));
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@rect"), mrbrect);
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "children"), mrb_ary_new (mrb));
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@visible"), mrb_true_value ());
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@z"), mrb_fixnum_value (0));
  viewport->sorted = TRUE;
  DATA_PTR (self) = viewport;

  return self;
}

static mrb_value
rect (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit(mrb, "@rect"));
}

static mrb_value
rect_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nrect;
  mrb_get_args (mrb, "o", &nrect);
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@rect"), nrect);
  return nrect;
}

static mrb_value
visible (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit(mrb, "@visible"));
}

static mrb_value
visible_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nvisible;
  mrb_get_args (mrb, "o", &nvisible);
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@rect"), nvisible);
  return nvisible;
}


static mrb_value
z (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit(mrb, "@z"));
}

static mrb_value
z_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value z;
  mrb_get_args (mrb, "o", &z);
  mrb_iv_set (mrb, self, mrb_intern_lit(mrb, "@z"), z);
  return z;
}
/**
 * Ruby Type Initializer
 */
void
mrb_viewport_init (mrb_state *mrb)
{
  struct RClass *viewport = mrb_define_class (mrb, "Viewport", mrb->object_class);
  mrb_define_method (mrb, viewport, "initialize", initialize, MRB_ARGS_REQ(2));
  mrb_define_method (mrb, viewport, "rect", rect, MRB_ARGS_NONE());
  mrb_define_method (mrb, viewport, "rect=", rect_eql, MRB_ARGS_REQ(1));
  mrb_define_method (mrb, viewport, "visible", visible, MRB_ARGS_NONE());
  mrb_define_method (mrb, viewport, "visible=", visible_eql, MRB_ARGS_REQ(1));
  mrb_define_method (mrb, viewport, "z", z, MRB_ARGS_NONE());
  mrb_define_method (mrb, viewport, "z=", z_eql, MRB_ARGS_REQ(1));
}
