/*
 * mrb_bitmap.c
 *
 *  Created on: 7/6/2015
 *      Author: Chronno
 */
#define GLEW_STATIC
#include <GL/glew.h>
#include <michelle.h>
#include <michelle/types.h>
#include <michelle/sdl.h>
#include <mruby.h>
#include <mruby/class.h>
#include <mruby/data.h>
#include <mruby/string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
static int rmask = 0xff000000;
static int gmask = 0x00ff0000;
static int bmask = 0x0000ff00;
static int amask = 0x000000ff;
#else
static int rmask = 0x000000ff;
static int gmask = 0x0000ff00;
static int bmask = 0x00ff0000;
static int amask = 0xff000000;
#endif

/**
 * mruby instance data free
 */
static void
bitmap_free (mrb_state *mrb, void *p)
{
  if (p)
    {
      MCL_Bitmap* bmp = (MCL_Bitmap*) p;
      SDL_FreeSurface (bmp->surface);
      free (bmp);
    }
}

/**
 * free function structure
 */
struct mrb_data_type const mrbal_bitmap_data_type = {"Bitmap", bitmap_free};

/**
 * Create a glTexture2d from a surface
 */
GLuint
surface_texture (SDL_Surface *surface)
{
  GLuint tid;
  GLenum texture_format;
  GLint ncolors;
  SDL_Surface* s = surface;
  /* Convert SDL_Surface to OpenGL Texture */
  ncolors = s->format->BytesPerPixel;
  if (ncolors == 4)
    {
      //alpha channel
      if (s->format->Rmask == 0x000000ff)
        {
          texture_format = GL_RGBA;
        }
      else
        {
          texture_format = GL_BGRA;
        }
    }
  else
    {
      if (ncolors == 3)
        {
          //no alpha channel
          if (s->format->Rmask == 0x000000ff)
            texture_format = GL_RGB;
          else
            texture_format = GL_BGR;
        }
      else
        {
          return 0;
        }
    }
  glGenTextures (1, &tid);
  glBindTexture (GL_TEXTURE_2D, tid);
  glTexImage2D (GL_TEXTURE_2D, 0, texture_format, s->w, s->h, 0, texture_format, GL_UNSIGNED_BYTE, s->pixels);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return tid;
}

/**
 *  Bitmap mruby Constructor
 */
static mrb_value
initialize (mrb_state *mrb, mrb_value self)
{
  MCL_Bitmap* surface;
  mrb_value param1;
  mrb_int height, count;
  DATA_TYPE (self) = &mrbal_bitmap_data_type;
  DATA_PTR (self) = NULL;
  count = mrb_get_args (mrb, "o|i", &param1, &height);
  surface = mrb_malloc (mrb, sizeof (MCL_Bitmap));
  switch (count)
    {
    case 1:
      if (mrb_string_p (param1))
        {
          const char *str = mrb_string_value_ptr (mrb, param1);
          surface->surface = IMG_Load (str);
          if (surface->surface == NULL)
            {
              printf ("Oh My Goodness, an error : %s", IMG_GetError ());
            }
        }
      else
        {
          mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong type of arguments");
          return mrb_nil_value ();
        }

      break;
    case 2:
      surface->surface = SDL_CreateRGBSurface (0, mrb_int (mrb, param1), height, 32, rmask, gmask, bmask, amask);
      break;
    default:
      mrb_raise (mrb, E_ARGUMENT_ERROR, "Wrong number of arguments");
      return mrb_nil_value ();
      break;
    }
  surface->glTexture = surface_texture (surface->surface);
  DATA_PTR (self) = surface;
  return self;
}

static mrb_value
width (mrb_state *mrb, mrb_value self)
{

  MCL_Bitmap *this = DATA_PTR (self);
  if (!this)
    {
      mrb_raise (mrb, E_RUNTIME_ERROR, "The bitmap has been disposed");
      return mrb_nil_value ();
    }
  return mrb_fixnum_value (this->surface->w);
}

static mrb_value
height (mrb_state *mrb, mrb_value self)
{
  MCL_Bitmap *this = DATA_PTR (self);
  if (!this)
    {
      mrb_raise (mrb, E_RUNTIME_ERROR, "The bitmap has been disposed");
      return mrb_nil_value ();
    }
  return mrb_fixnum_value (this->surface->h);
}

static mrb_value
dispose (mrb_state *mrb, mrb_value self)
{
  MCL_Bitmap *this = DATA_PTR (self);
  bitmap_free (mrb, this);
  DATA_PTR (self) = NULL;
  return mrb_nil_value ();
}

static mrb_value
disposed (mrb_state *mrb, mrb_value self)
{
  return DATA_PTR (self) ? mrb_false_value () : mrb_true_value ();
}

static mrb_value
get_pixel (mrb_state *mrb, mrb_value self)
{
  mrb_value params[4];
  mrb_int x, y;
  Uint8 r, g, b, a;
  MCL_Bitmap* this = DATA_PTR (self);
  if (!this)
    {
      mrb_raise (mrb, E_RUNTIME_ERROR, "The bitmap has been disposed");
      return mrb_nil_value ();
    }
  mrb_get_args (mrb, "ii", &x, &y);
  if (x < 0 || y < 0 || x >= this->surface->w || y >= this->surface->h)
    {
      r = 0;
      g = 0;
      b = 0;
      a = 255;
    }
  else
    {
      Uint32 pix = sdl_bitmap_get_pixel (this->surface, x, y);
      SDL_GetRGBA (pix, this->surface->format, &r, &g, &b, &a);
    }

  params[0] = mrb_fixnum_value ((mrb_int) r);
  params[1] = mrb_fixnum_value ((mrb_int) g);
  params[2] = mrb_fixnum_value ((mrb_int) b);
  params[3] = mrb_fixnum_value ((mrb_int) a);
  return mrb_obj_new (mrb, mrb_class_get (mrb, "Color"), 4, params);
}

static mrb_value
set_pixel (mrb_state *mrb, mrb_value self)
{
  mrb_int x, y;
  mrb_value color;
  SDL_Color *colour;
  Uint32 pixel;
  MCL_Bitmap *this = DATA_PTR (self);
  mrb_get_args (mrb, "oo", &x, &y, &color);
  colour = DATA_PTR (color);
  pixel = SDL_MapRGBA (this->surface->format, colour->r, colour->g, colour->b, colour->a);
  sdl_bitmap_set_pixel (this->surface, x, y, pixel);
  this->glTexture = surface_texture (this->surface);
  return color;
}

static mrb_value
fill_rect (mrb_state *mrb, mrb_value self)
{
  mrb_value color, rect;
  Uint32 pixel;
  SDL_Color *colour;
  SDL_Rect *rectc;
  MCL_Bitmap *this = DATA_PTR (self);
  mrb_get_args (mrb, "oo", &rect, &color);
  colour = DATA_PTR (color);
  rectc = DATA_PTR (rect);
  pixel = SDL_MapRGBA (this->surface->format, colour->r, colour->g, colour->b, colour->a);
  SDL_FillRect (this->surface, rectc, pixel);
  this->glTexture = surface_texture (this->surface);
  return self;
}

static mrb_value
blit (mrb_state *mrb, mrb_value self)
{
  SDL_Rect *src_rect;
  SDL_Point *origin;
  MCL_Bitmap *src_bmp;
  SDL_Rect dst_rect;
  MCL_Bitmap *this = DATA_PTR (self);
  mrb_value rect, bitmap, point;
  mrb_get_args (mrb, "ooo", &point, &bitmap, &rect);
  src_rect = DATA_PTR (rect);
  origin = DATA_PTR (point);
  dst_rect.x = origin->x;
  dst_rect.y = origin->y;
  dst_rect.w = src_rect->w;
  dst_rect.h = src_rect->h;
  src_bmp = DATA_PTR (bitmap);
  SDL_BlitSurface (src_bmp->surface, src_rect, this->surface, &dst_rect);
  this->glTexture = surface_texture (this->surface);
  return self;
}

static mrb_value
scaled_blit (mrb_state *mrb, mrb_value self)
{
  SDL_Rect *src_rect;
  SDL_Rect *dst_rect;
  MCL_Bitmap *src_bmp;
  MCL_Bitmap *this = DATA_PTR (self);
  mrb_value srect, drect, bitmap;
  mrb_get_args (mrb, "ooo", &drect, &bitmap, &srect);
  src_rect = DATA_PTR (srect);
  dst_rect = DATA_PTR (drect);
  src_bmp = DATA_PTR (bitmap);
  SDL_BlitScaled (src_bmp->surface, src_rect, this->surface, dst_rect);
  this->glTexture = surface_texture (this->surface);
  return self;
}

/**
 * Initialize mruby class
 */
void
mrb_bitmap_init (mrb_state *mrb)
{
  struct RClass *bitmap = mrb_define_class (mrb, "Bitmap", mrb->object_class);
  mrb_define_method (mrb, bitmap, "initialize", initialize, MRB_ARGS_REQ (1) | MRB_ARGS_OPT (1));
  mrb_define_method (mrb, bitmap, "width", width, MRB_ARGS_NONE ());
  mrb_define_method (mrb, bitmap, "height", height, MRB_ARGS_NONE ());
  mrb_define_method (mrb, bitmap, "dispose", dispose, MRB_ARGS_NONE ());
  mrb_define_method (mrb, bitmap, "disposed?", disposed, MRB_ARGS_NONE ());
  mrb_define_method (mrb, bitmap, "get_pixel", get_pixel, MRB_ARGS_REQ (2));
  mrb_define_method (mrb, bitmap, "set_pixel", set_pixel, MRB_ARGS_REQ (3));
  mrb_define_method (mrb, bitmap, "fill_rect", fill_rect, MRB_ARGS_REQ (2));
  mrb_define_method (mrb, bitmap, "blit", blit, MRB_ARGS_REQ (3));
  mrb_define_method (mrb, bitmap, "scaled_blit", scaled_blit, MRB_ARGS_REQ (3));
  MRB_SET_INSTANCE_TT (bitmap, MRB_TT_DATA);
}
