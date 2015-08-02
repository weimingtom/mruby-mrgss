/*
 * viewport.c
 *
 *  Created on: Jun 24, 2015
 *      Author: Manuel
 */
#include <stdlib.h>
#include <mruby.h>
#include <mruby/data.h>
#include <mruby/variable.h>
#include <mruby/array.h>
#include <michelle.h>
#include <michelle/types.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>


kmMat4 matrices[3];

void
viewport_add_children (mrb_state *mrb, mrb_value viewport, mrb_value child)
{
  MCL_Viewport *viewp = DATA_PTR(viewport);
  int ai = mrb_gc_arena_save(mrb);
  mrb_value children = mrb_iv_get (mrb, viewport, mrb_intern_lit(mrb, "children"));
  mrb_ary_push (mrb, children, child );
  mrb_gc_arena_restore(mrb, ai);
  viewp->sorted = FALSE;
}

void
viewport_remove_children (mrb_state *mrb, mrb_value viewport, mrb_value child)
{
  mrb_value children = mrb_iv_get (mrb, viewport, mrb_intern_lit(mrb, "children"));
  mrb_int size = mrb_ary_len (mrb, children);
  for (int i = size - 1; i >= 0; --i)
    {
      mrb_value drawable = mrb_ary_entry (children, i);
      if (mrb_obj_equal (mrb, drawable, child))
	{
	  drawable = mrb_nil_value ();
	}
    }
}

static
int
zcompare (const void * first, const void * second)
{
  MCL_Sprite* a = DATA_PTR(*(mrb_value*) first);
  MCL_Sprite* b = DATA_PTR(*(mrb_value*) second);
  if (a->z > b->z)
    {
      return 1;
    }
  else if (a->z < b->z)
    {
      return -1;
    }
  else
    {
      return 0;
    }
}

void
create_matrices (kmMat4 matrices[], SDL_Rect* rect)
{
  kmMat4IsIdentity(&matrices[1]);
  kmMat4OrthographicProjection(&matrices[2], 0.0f,rect->w, rect->h, 0.0f, 0.0f, 1.0f);
}

void
draw_viewport (mrb_state *mrb, mrb_value viewport)
{
  SDL_Rect* crect;
  MCL_Viewport *viewp = DATA_PTR(viewport);
  mrb_value children = mrb_iv_get (mrb, viewport, mrb_intern_lit(mrb, "children"));
  mrb_value visible = mrb_iv_get (mrb, viewport, mrb_intern_lit(mrb, "@visible"));
  mrb_value rect = mrb_iv_get (mrb, viewport, mrb_intern_lit(mrb, "@rect"));
  mrb_int size = mrb_ary_len (mrb, children);
  if (!mrb_bool(visible))
    {
      return;
    }
  if (!viewp->sorted)
    {
      struct RArray* anArray = mrb_ary_ptr(children);
      qsort(anArray->ptr, anArray->len, sizeof(mrb_value), zcompare);
      viewp->sorted = TRUE;
    }

  crect = DATA_PTR(rect);
  create_matrices (matrices, crect);
  for (int i = 0; i < size; i++)
    {
      draw_sprite (mrb, mrb_ary_entry (children, i), matrices);
    }
}
