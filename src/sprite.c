/*
 * sprite.c
 *
 *  Created on: Jun 25, 2015
 *      Author: Manuel
 */
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GL/glew.h>
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <michelle.h>
#include <kazmath/kazmath.h>
#include <michelle/types.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

static GLuint eab;
static int eabbind;
static GLuint indices[6] = {0, 1, 2, 2, 3, 0};

static GLuint lastTexture;
static GLuint lastVao;
static GLuint lastVbo;

void
bind_shader_attributes (GLuint shaderProgram, kmMat4* matrices)
{
  GLint texture_coord_attribute;
  GLint model;
  GLint view;
  GLint projection;
  GLint position_attribute = glGetAttribLocation (shaderProgram, "position");
  glVertexAttribPointer (position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray (position_attribute);

  texture_coord_attribute = glGetAttribLocation (shaderProgram, "texture_coord");
  glVertexAttribPointer (texture_coord_attribute, 2, GL_FLOAT, GL_TRUE, 0, (GLvoid *) (sizeof (GL_FLOAT) * 8));
  glEnableVertexAttribArray (texture_coord_attribute);

  model = glGetUniformLocation (shaderProgram, "Model");
  glUniformMatrix4fv (model, 1, GL_FALSE, matrices[0].mat);

  view = glGetUniformLocation (shaderProgram, "View");
  glUniformMatrix4fv (view, 1, GL_FALSE, matrices[1].mat);

  projection = glGetUniformLocation (shaderProgram, "Projection");
  glUniformMatrix4fv (projection, 1, GL_FALSE, matrices[2].mat);

}

void
draw_sprite (mrb_state *mrb, mrb_value sprite, kmMat4* matrices)
{
  MCL_Sprite *spr = DATA_PTR (sprite);
  MCL_Bitmap *bmp;
  mrb_value shader;
  MCL_Shader *shaderid;
  if (!spr->visible)
    {
      return;
    }
  bmp = DATA_PTR (mrb_iv_get (mrb, sprite, mrb_intern_lit (mrb, "@bitmap")));
  if (lastVao != spr->vao)
    {
      glBindVertexArray (spr->vao);
      lastVao = spr->vao;
    }
  if (lastVbo != spr->vbo)
    {
      glBindBuffer (GL_ARRAY_BUFFER, spr->vbo);
      lastVbo = spr->vbo;
    }
  if (!eabbind)
    {
      glGenBuffers (1, &eab);
      glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, eab);
      glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), indices, GL_STATIC_DRAW);
      eabbind = TRUE;
    }
  glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, eab);
  if (lastTexture != bmp->glTexture)
    {
      glBindTexture (GL_TEXTURE_2D, bmp->glTexture);
      lastTexture = bmp->glTexture;
    }

  kmMat4Identity(&matrices[0]);


  shader = mrb_iv_get (mrb, sprite, mrb_intern_lit (mrb, "@shader"));
  shaderid = DATA_PTR (shader);
  bind_shader_attributes (shaderid->shaderId, matrices);

  glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
