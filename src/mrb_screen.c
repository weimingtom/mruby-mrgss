/*
 * mrb_screen.c
 *
 *  Created on: 13/6/2015
 *      Author: Chronno
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <stdlib.h>
#include <time.h>
#include <michelle.h>
#include <michelle/types.h>
#include <michelle/constructors.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/variable.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

/**
 * mruby instance data free
 */
static void
window_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      MCL_Window* win = (MCL_Window*) p;
      SDL_GL_DeleteContext (win->context);
      SDL_DestroyWindow (win->window);
      free (p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_window_data_type ={"Window", window_free};

static mrb_value
initialize (mrb_state* mrb, mrb_value self)
{
  mrb_value mrbrect, mrbviewport, mrbtitle;
  mrb_int count;
  MCL_Window *window;
  SDL_Rect *rect;
  const char *title;
  DATA_TYPE (self) = &mrbal_window_data_type;
  DATA_PTR (self) = NULL;
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER, 1);
  count = mrb_get_args (mrb, "oo|o", &mrbtitle, &mrbrect, &mrbviewport);
  window = mrb_malloc (mrb, sizeof (MCL_Window));
  title = mrb_string_value_ptr (mrb, mrbtitle);
  rect = DATA_PTR (mrbrect);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@rect"), mrbrect);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@clear_color"), MCL_color_new (mrb, 255, 0, 0, 255));
  window->window = SDL_CreateWindow (title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, rect->w, rect->h, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
  window->renderer = SDL_CreateRenderer (window->window, -1, SDL_RENDERER_ACCELERATED);
  window->context = SDL_GL_CreateContext (window->window);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  SDL_GL_SetSwapInterval (1);
  glewExperimental = GL_TRUE;
  glewInit ();
  if (count > 2)
    {
      mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@viewport"), mrbviewport);
    }
  else
    {
      mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@viewport"), MCL_viewport_new (mrb, rect->x, rect->y, rect->w, rect->h));
    }
  DATA_PTR (self) = window;
  return self;
}

static mrb_value
update (mrb_state* mrb, mrb_value self)
{
  MCL_Window *win;
  mrb_value mrbviewport, mrbclearcolor;
  SDL_Color *clearcolor;
  win = DATA_PTR (self);
  mrbclearcolor = mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@clear_color"));
  clearcolor = DATA_PTR (mrbclearcolor);
  glClearColor (clearcolor->r, clearcolor->g, clearcolor->b, clearcolor->a);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  mrbviewport = mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport"));
  draw_viewport (mrb, mrbviewport);
  SDL_GL_SwapWindow (win->window);
  SDL_PollEvent (&win->event);
  return self;
}

static mrb_value
viewport (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport"));
}

static mrb_value
viewport_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nviewport;
  mrb_get_args (mrb, "o", &nviewport);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@viewport"), nviewport);
  return nviewport;
}

/**
 * Initialize mruby class
 */
void
mrb_screen_init (mrb_state *mrb)
{
  struct RClass *screen = mrb_define_class (mrb, "Screen", mrb->object_class);
  MRB_SET_INSTANCE_TT (screen, MRB_TT_DATA);
  mrb_define_method (mrb, screen, "initialize", initialize, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, screen, "update", update, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, screen, "viewport", viewport, MRB_ARGS_NONE ());
  mrb_define_method (mrb, screen, "viewport=", viewport_eql, MRB_ARGS_REQ (1));

}
