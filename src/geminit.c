#include <mruby.h>
#include <stdio.h>
#include <michelle.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

void
mrb_mruby_mrgss_gem_init (mrb_state* mrb)
{
  if (SDL_Init (SDL_INIT_EVERYTHING) < 0)
    {
      printf ("fallo la inicializacion");
    }
  if (IMG_Init (IMG_INIT_JPG | IMG_INIT_PNG) < 0)
    {
      printf ("fallo la lib de imagenes image");
    }
  if (TTF_Init () < 0)
    {
      printf ("fallo la lib de ttf");
    }
  mrb_color_init (mrb);
  mrb_rect_init (mrb);
  mrb_bitmap_init (mrb);
  mrb_viewport_init (mrb);
  mrb_shader_init (mrb);
  mrb_sprite_init (mrb);
  mrb_screen_init (mrb);
  mrb_keyboard_init (mrb);
  mrb_mouse_init (mrb);
}

void
mrb_mruby_mrgss_gem_final (mrb_state* mrb)
{
  printf ("hello");
  /* finalizer */
}
