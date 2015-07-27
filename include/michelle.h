/* 
 * File:   michelle.h
 * Author: Manuel
 *
 * Created on July 27, 2015, 5:18 PM
 */

#ifndef MICHELLE_H
#define	MICHELLE_H

#include <mruby.h>

#ifdef	__cplusplus
extern "C" {
#endif
void
mrb_color_init (mrb_state *mrb);
void
mrb_rect_init (mrb_state *mrb);
void
mrb_keyboard_init (mrb_state *mrb);
void
mrb_mouse_init (mrb_state *mrb);

#ifdef	__cplusplus
}
#endif

#endif	/* MICHELLE_H */

