#include <mruby.h>
#include <stdio.h>
#include <michelle.h>

void
mrb_mruby_mrgss_gem_init (mrb_state* mrb)
{
  mrb_color_init (mrb);
  mrb_rect_init (mrb);
  mrb_keyboard_init (mrb);
  mrb_mouse_init (mrb);
}

void
mrb_mruby_mrgss_gem_final (mrb_state* mrb)
{
  printf ("hello");
  /* finalizer */
}
