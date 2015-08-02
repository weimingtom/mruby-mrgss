/*
 * mrb_shader.c
 *
 *  Created on: Jul 7, 2015
 *      Author: Manuel
 */
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <michelle.h>
#include <michelle/types.h>
#include <michelle/sdl.h>

///////////////////////////////////////////////////////
////
///////////////////////////////////////////////////////
static void
shader_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      free (p);
    }
}

struct mrb_data_type const mrbal_shader_data_type =
  { "Shader", shader_free };
///////////////////////////////////////////////////////
////
///////////////////////////////////////////////////////
mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  char *vertex;
  char *fragment;
  GLuint vs, fs;
  MCL_Shader* shader;
  mrb_int count;
  DATA_TYPE (self) = &mrbal_shader_data_type;
  DATA_PTR (self) = NULL;
  count = mrb_get_args (mrb, "|zz", &vertex, &fragment);
  shader = mrb_malloc (mrb, sizeof(MCL_Shader));
  if (count < 2)
    {
      shader->shaderId = get_default_shader();
    }
  else
    {
      vs = compile_shader (vertex, GL_VERTEX_SHADER);
      fs = compile_shader (vertex, GL_FRAGMENT_SHADER);
      shader->shaderId = compile_shader_program (vs, fs);
    }
  DATA_PTR(self) = shader;
  return self;
}
///////////////////////////////////////////////////////
////
///////////////////////////////////////////////////////
void
mrb_shader_init (mrb_state *mrb)
{
  struct RClass *shader = mrb_define_class (mrb, "Shader", mrb->object_class);
  mrb_define_method (mrb, shader, "initialize", initialize, MRB_ARGS_OPT(2));
  MRB_SET_INSTANCE_TT (shader, MRB_TT_DATA);
}
