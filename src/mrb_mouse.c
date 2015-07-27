#include <mruby.h>
#include <mruby/value.h>
#include <mruby/array.h>
#include <michelle.h>
#include <michelle/constructors.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>

static mrb_int mouse_x, mouse_y;

static mrb_value
mouse_update (mrb_state *mrb, mrb_int capa)
{
  SDL_PumpEvents ();
  SDL_GetMouseState (&mouse_x, &mouse_y);
  return mrb_nil_value ();
}

static mrb_value
mouse_position (mrb_state *mrb, mrb_int capa)
{
  mrb_value res = mrb_ary_new(mrb);
  mrb_ary_push(mrb, res, mrb_fixnum_value(mouse_x));
  mrb_ary_push(mrb, res, mrb_fixnum_value(mouse_y));
  return res;
}

static mrb_value
mrgsl_mouse_x (mrb_state *mrb, mrb_int capa)
{
  return mrb_fixnum_value (mouse_x);
}
static mrb_value
mrgsl_mouse_y (mrb_state *mrb, mrb_int capa)
{
  return mrb_fixnum_value (mouse_y);
}

static mrb_value
left_click (mrb_state* mrb, mrb_value self)
{
  mrb_int button;
  mrb_get_args (mrb, "i", &button);
  switch (button)
    {
    case 1:
      return mrb_bool_value(SDL_GetMouseState (NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT));
      break;
    case 2:
      return mrb_bool_value(SDL_GetMouseState (NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE));
      break;
    case 3:
      return mrb_bool_value(SDL_GetMouseState (NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT));
      break;
    default:
      return mrb_false_value ();
    }
}

/*
 *  Module Graphics
 */
void
mrb_mouse_init (mrb_state *mrb)
{
  struct RClass *hw;
  hw = mrb_define_module(mrb, "Mouse");
  mrb_define_class_method (mrb, hw, "update", (mrb_func_t) mouse_update, MRB_ARGS_NONE());
  mrb_define_class_method (mrb, hw, "position", (mrb_func_t) mouse_position, MRB_ARGS_NONE());
  mrb_define_class_method (mrb, hw, "x", (mrb_func_t) mrgsl_mouse_x, MRB_ARGS_NONE());
  mrb_define_class_method (mrb, hw, "y", (mrb_func_t) mrgsl_mouse_y, MRB_ARGS_NONE());
  mrb_define_class_method (mrb, hw, "click?", (mrb_func_t) left_click, MRB_ARGS_REQ(1));
}
