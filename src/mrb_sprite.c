/*
 * mrb_sprite.c
 *
 *  Created on: Jun 25, 2015
 *      Author: Manuel
 */
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/array.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <michelle.h>
#include <michelle/types.h>
#include <michelle/constructors.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

void
calculate_vertex_position (GLfloat *buffer, int x, int y, int w, int h)
{
  //top left
  buffer[0] = (float) x;
  buffer[1] = (float) y;
  //top right
  buffer[2] = (float) (x + w);
  buffer[3] = (float) (y);
  // bottom left
  buffer[4] = (float) (x + w);
  buffer[5] = (float) (y + h);
  // bottom right
  buffer[6] = (float) (x);
  buffer[7] = (float) (y + h);
}

void
calculate_texture_coords (GLfloat *buffer, float x, float y, float w, float h)
{
  //top left
  buffer[0] = (float) x * 1.0f;
  buffer[1] = (float) y * 1.0f;

  //top right
  buffer[2] = (float) w + x * 1.0f;
  buffer[3] = (float) y * 1.0f;
  // bottom left
  buffer[4] = (float) w + x * 1.0f;
  buffer[5] = (float) h + y * 1.0f;
  // bottom right
  buffer[6] = (float) x * 1.0f;
  buffer[7] = (float) h + y * 1.0f;

}

static void
sprite_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      free (p);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_sprite_data_type = {"Sprite", sprite_free};

static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *sprite;
  MCL_Bitmap *bmp;
  mrb_value parent, bitmap;
  DATA_TYPE (self) = &mrbal_sprite_data_type;
  DATA_PTR (self) = NULL;
  mrb_get_args (mrb, "oo", &parent, &bitmap);
  sprite = mrb_malloc (mrb, sizeof (MCL_Sprite));
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@viewport"), parent);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@bitmap"), bitmap);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@shader"), mrb_obj_new (mrb, mrb_class_get (mrb, "Shader"), 0, NULL));
  glGenVertexArrays (1, &sprite->vao);
  glGenBuffers (1, &sprite->vbo);
  glGenBuffers (1, &sprite->eab);
  bmp = DATA_PTR (bitmap);
  sprite->angle = 0;
  sprite->ox = 0;
  sprite->oy = 0;
  sprite->x = 0;
  sprite->y = 0;
  sprite->visible = TRUE;
  sprite->z = 0;
  sprite->src_rect = (SDL_Rect
                      ){0, 0, bmp->surface->w, bmp->surface->h};
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@src_rect"), MCL_rect_new (mrb, 0, 0, bmp->surface->w, bmp->surface->h));

  calculate_vertex_position (sprite->vertices_position, 0, 0, bmp->surface->w, bmp->surface->h);
  calculate_texture_coords (sprite->texture_coord, 0, 0, bmp->surface->w, bmp->surface->h);
  glBindBuffer (GL_ARRAY_BUFFER, sprite->vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (sprite->vertices_position) + sizeof (sprite->texture_coord), NULL, GL_STATIC_DRAW);
  glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, sprite->vertices_position);
  glBufferSubData (GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * 8, sizeof (GL_FLOAT) * 8, sprite->texture_coord);
  viewport_add_children (mrb, parent, self);

  DATA_PTR (self) = sprite;

  return self;
}

static mrb_value
rect (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr = DATA_PTR (self);
  return MCL_rect_transform (mrb, &spr->src_rect);
}

static mrb_value
rect_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nrect;
  SDL_Rect* rect;
  MCL_Bitmap *bmp;
  mrb_value mbitmap;
  MCL_Sprite* spr = DATA_PTR (self);
  mrb_get_args (mrb, "d", &rect, &mrbal_rect_data_type);
  spr->src_rect = *rect;
  mbitmap = mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@bitmap"));
  bmp = DATA_PTR (mbitmap);
  calculate_vertex_position (spr->vertices_position, spr->x, spr->y, rect->w, rect->h);
  calculate_texture_coords (spr->texture_coord, (float) rect->x / (float) bmp->surface->w, (float) rect->y / (float) bmp->surface->h, (float) rect->w / (float) bmp->surface->w, (float) rect->h / (float) bmp->surface->h);
  glBindBuffer (GL_ARRAY_BUFFER, spr->vbo);
  glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertices_position);
  glBufferSubData (GL_ARRAY_BUFFER, sizeof (GL_FLOAT) * 8, sizeof (GL_FLOAT) * 8, spr->texture_coord);
  return nrect;
}

static mrb_value
visible (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr = DATA_PTR (self);
  return mrb_bool_value (spr->visible);
}

static mrb_value
visible_eql (mrb_state *mrb, mrb_value self)
{
  mrb_bool nvisible;
  MCL_Sprite* spr = DATA_PTR (self);
  mrb_get_args (mrb, "b", &nvisible);
  spr->visible = nvisible;
  return mrb_bool_value (nvisible);
}

static mrb_value
ox (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->ox);
}

static mrb_value
ox_eql (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr;
  mrb_int norigin;
  mrb_get_args (mrb, "i", &norigin);
  spr = DATA_PTR (self);
  spr->ox = norigin;
  return mrb_fixnum_value (norigin);
}

static mrb_value
oy (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->oy);
}

static mrb_value
oy_eql (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite* spr;
  mrb_int norigin;
  mrb_get_args (mrb, "i", &norigin);
  spr = DATA_PTR (self);
  spr->oy = norigin;
  return mrb_fixnum_value (norigin);
}

static mrb_value
z (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->z);
}

static mrb_value
z_eql (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *spr;
  MCL_Viewport* viewp;
  mrb_int nz;
  mrb_get_args (mrb, "i", &nz);
  spr = DATA_PTR (self);
  viewp = DATA_PTR (mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport")));
  viewp->sorted = FALSE;
  spr->z = nz;
  return mrb_fixnum_value (nz);
}

static mrb_value
angle (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->angle);
}

static mrb_value
angle_eql (mrb_state *mrb, mrb_value self)
{
  mrb_int angle;
  MCL_Sprite* spr;
  mrb_get_args (mrb, "i", &angle);
  spr = DATA_PTR (self);
  spr->angle = angle;
  return mrb_fixnum_value (spr->angle);
}

static mrb_value
x (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->x);
}

static mrb_value
y (mrb_state *mrb, mrb_value self)
{
  MCL_Sprite *spr = DATA_PTR (self);
  return mrb_fixnum_value (spr->y);
}

static mrb_value
x_eql (mrb_state *mrb, mrb_value self)
{
  mrb_int x;
  MCL_Sprite* spr;
  mrb_get_args (mrb, "i", &x);
  spr = DATA_PTR (self);
  spr->x = x;
  calculate_vertex_position (spr->vertices_position, spr->x, spr->y, spr->src_rect.w, spr->src_rect.h);
  glBindBuffer (GL_ARRAY_BUFFER, spr->vbo);
  glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertices_position);
  return mrb_fixnum_value (spr->x);
}

static mrb_value
y_eql (mrb_state *mrb, mrb_value self)
{
  mrb_int y;
  MCL_Sprite* spr;
  mrb_get_args (mrb, "i", &y);
  spr = DATA_PTR (self);
  spr->y = y;
  calculate_vertex_position (spr->vertices_position, spr->x, spr->y, spr->src_rect.w, spr->src_rect.h);
  glBindBuffer (GL_ARRAY_BUFFER, spr->vbo);
  glBufferSubData (GL_ARRAY_BUFFER, 0, sizeof (GL_FLOAT) * 8, spr->vertices_position);
  return mrb_fixnum_value (spr->y);
}

static mrb_value
parent (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport"));
}

static mrb_value
parent_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nparent;
  mrb_get_args (mrb, "o", &nparent);
  viewport_remove_children (mrb, mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@viewport")), self);
  viewport_add_children (mrb, nparent, self);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@viewport"), nparent);
  return nparent;
}

static mrb_value
shader (mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get (mrb, self, mrb_intern_lit (mrb, "@shader"));
}

static mrb_value
shader_eql (mrb_state *mrb, mrb_value self)
{
  mrb_value nshader;
  mrb_get_args (mrb, "o", &nshader);
  mrb_iv_set (mrb, self, mrb_intern_lit (mrb, "@shader"), nshader);
  return nshader;
}

/**
 * Ruby Type Initializer
 */
void
mrb_sprite_init (mrb_state *mrb)
{
  struct RClass *sprite = mrb_define_class (mrb, "Sprite", mrb->object_class);
  mrb_define_method (mrb, sprite, "initialize", initialize, MRB_ARGS_REQ (2));
  mrb_define_method (mrb, sprite, "src_rect", rect, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "src_rect=", rect_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "visible", visible, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "visible=", visible_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "z", z, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "z=", z_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "ox", ox, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "ox=", ox_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "oy", oy, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "oy=", oy_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "angle", angle, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "angle=", angle_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "viewport", parent, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "viewport=", parent_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "x", x, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "x=", x_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "y", y, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "y=", y_eql, MRB_ARGS_REQ (1));
  mrb_define_method (mrb, sprite, "shader", shader, MRB_ARGS_NONE ());
  mrb_define_method (mrb, sprite, "shader=", shader_eql, MRB_ARGS_REQ (1));
  MRB_SET_INSTANCE_TT (sprite, MRB_TT_DATA);
}

