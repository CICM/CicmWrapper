/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
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

#include "../estruct.h"

// PAINT METHOD //
void egraphics_fill(t_elayer *g);
void egraphics_fill_preserve(t_elayer *g);
void egraphics_stroke(t_elayer *g);
void egraphics_stroke_preserve(t_elayer *g);
void etext_layout_draw(t_etext* textlayout, t_elayer *g);

// GRAPHICS MADIFICATIONS //
void egraphics_set_line_splinestep(t_elayer *g, float smooth);
void egraphics_set_line_width(t_elayer *g, float width);
void egraphics_set_source_jrgba(t_elayer *g, t_rgba *rgba);
void egraphics_rotate_set(t_elayer *g, float angle);
void egraphics_rotate_add(t_elayer *g, float angle);
void egraphics_matrix_init(t_matrix *x, float xx, float yx, float xy, float yy, float x0, float y0);
void egraphics_set_matrix(t_elayer *g, const t_matrix *matrix);

// GRAPHICS MADIFICATIONS PRIVATE //
void egraphics_apply_rotation(t_elayer *g, t_egobj* gobj); // PRIVATE //
void egraphics_apply_matrix(t_elayer *g, t_egobj* gobj);   // PRIVATE //
void egraphics_clip_object(t_elayer *g, t_egobj* gobj);           // PRIVATE //

// FORM //
void egraphics_line_to(t_elayer *g, float x, float y);
void egraphics_move_to(t_elayer *g, float x, float y);
void egraphics_line(t_elayer *g, float x0, float y0,  float x1, float y1);
void egraphics_close_path(t_elayer *g);
void egraphics_rectangle(t_elayer *g, float x, float y, float width, float height);
void egraphics_rectangle_rounded(t_elayer *g, float x, float y, float width, float height, float roundness);


t_etext* etext_layout_create();
void etext_layout_destroy(t_etext* textlayout);
void etext_layout_set(t_etext* textlayout, char* text, t_efont *jfont,  double x, double y, double width,  double height, t_etextjustify_flags justification, t_etextwrap_flags wrap);
void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color);
t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, double size);
void efont_destroy(t_efont* font);

// COLOR //
char* rgba_to_hex(t_rgba color);
char* rgb_to_hex(t_rgb color);
t_rgba rgba_addContrast(t_rgba color, float contrast, bool preserveAlpha);
t_rgb rgb_addContrast(t_rgb color, float contrast);
void rgba_set(t_rgba *color, float red, float green, float blue, float alpha);
void rgb_set(t_rgb *color, float red, float green, float blue);

// Ohlala...
__attribute__((used)) static t_rgba rgba_black         = {0., 0., 0., 1.};
__attribute__((used)) static t_rgba rgba_greydark      = {0.3, 0.3, 0.3, 1.};
__attribute__((used)) static t_rgba rgba_grey          = {0.5, 0.5, 0.5, 1.};
__attribute__((used)) static t_rgba rgba_greylight     = {0.8, 0.8, 0.8, 1.};
__attribute__((used)) static t_rgba rgba_white         = {1., 1., 1., 1.};
__attribute__((used)) static t_rgba rgba_blue          = {0., 0., 1., 1.};
__attribute__((used)) static t_rgba rgba_green         = {0., 1., 0., 1.};
__attribute__((used)) static t_rgba rgba_red           = {1., 0., 0., 1.};
__attribute__((used)) static t_rgba rgba_bluelight     = {0.65, 0.65, 0.65, 1.};


#endif