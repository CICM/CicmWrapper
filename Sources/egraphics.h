/*
 * CicmWrapper
 *
 * A wrapper for Pure Data
 *
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 *
 * Website  : http://www.mshparisnord.fr/HoaLibrary/
 * Contacts : cicm.mshparisnord@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#ifndef DEF_EGRAPHICS
#define DEF_EGRAPHICS

#include "ecommon.h"

/*! @addtogroup groupgraphics The Graphics Part
 * @brief The t_elayer part.
 * @details This part refers to the methods and structures used to paint.
 *  @{
 */

void egraphics_fill(t_elayer *g);
void egraphics_fill_preserve(t_elayer *g);
void egraphics_stroke(t_elayer *g);
void egraphics_stroke_preserve(t_elayer *g);
void etext_layout_draw(t_etext* textlayout, t_elayer *g);

void egraphics_set_line_splinestep(t_elayer *g, float smooth);
void egraphics_set_line_width(t_elayer *g, float width);
void egraphics_set_color_rgba(t_elayer *g, const t_rgba *rgba);
void egraphics_set_color_rgb(t_elayer *g, const t_rgb *rgb);
void egraphics_set_color_hex(t_elayer *g, t_symbol *hex);
void egraphics_set_color_hsla(t_elayer *g, const t_hsla *hsla);
void egraphics_set_color_hsl(t_elayer *g, const t_hsl *hsl);
void egraphics_rotate(t_elayer *g, float angle);
void egraphics_matrix_init(t_matrix *x, float xx, float yx, float xy, float yy, float x0, float y0);
void egraphics_set_matrix(t_elayer *g, const t_matrix *matrix);

// GRAPHICS MODIFICATIONS PRIVATE //
void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj);

void egraphics_line_to(t_elayer *g, float x, float y);
void egraphics_move_to(t_elayer *g, float x, float y);
void egraphics_curve_to(t_elayer *g, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy);
void egraphics_arc_to(t_elayer *g, float cx, float cy, float extend);
void egraphics_arc_oval_to(t_elayer *g, float cx, float cy, float radius, float extend);

void egraphics_line(t_elayer *g, float x0, float y0,  float x1, float y1);
void egraphics_curve(t_elayer *g, float startx, float starty, float ctrl1x, float ctrl1y, float ctrl2x, float ctrl2y, float endx, float endy);
void egraphics_line_fast(t_elayer *g, float x0, float y0, float x1, float y1);

void egraphics_close_path(t_elayer *g);
void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height);
void egraphics_rectangle_rounded(t_elayer *g, float x, float y, float width, float height, float roundness);
void egraphics_circle(t_elayer *g, float xc, float yc, float radius);
void egraphics_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy);
void egraphics_arc(t_elayer *g, float xc, float yc, float radius, float angle1, float angle2);
void egraphics_arc_oval(t_elayer *g, float xc, float yc, float radiusx, float radiusy, float angle1, float angle2);

t_etext* etext_layout_create(void);
void etext_layout_destroy(t_etext* textlayout);
void etext_layout_set(t_etext* textlayout, const char* text, t_efont *jfont,  float x, float y, float width,  float height, etextanchor_flags anchor, etextjustify_flags justify, etextwrap_flags wrap);
void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color);
t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, float size);
void efont_destroy(t_efont* font);

char* rgba_to_hex(t_rgba color);
char* rgb_to_hex(t_rgb color);
char* hsla_to_hex(t_hsla color);
char* hsl_to_hex(t_hsl color);
t_hsl rgb_to_hsl(t_rgb color);
t_hsla rgba_to_hsla(t_rgba color);
t_rgba hex_to_rgba(char* color);
t_rgb hex_to_rgb(char* color);
t_rgba hsla_to_rgba(t_hsla color);
t_rgb hsl_to_rgb(t_hsl color);

t_rgba rgba_addContrast(t_rgba color, float contrast);
t_rgb rgb_addContrast(t_rgb color, float contrast);
void rgba_set(t_rgba *color, float red, float green, float blue, float alpha);
void rgb_set(t_rgb *color, float red, float green, float blue);
void hsla_set(t_hsla *color, float hue, float saturation, float luminosity, float alpha);
void hsl_set(t_hsl *color, float hue, float saturation, float luminosity);

float pd_clip_min(float aValue, float aMinimum);
float pd_clip_max(float aValue, float aMaximum);
float pd_clip_minmax(float aValue, float aMinimum, float aMaximum);
float pd_ordinate(float radius, float angle);
float pd_abscissa(float radius, float angle);
float pd_radius(float x, float y);
float pd_angle(float x, float y);

/** @} */

#endif