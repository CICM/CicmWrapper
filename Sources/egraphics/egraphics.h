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

typedef enum _etext_justification_flags
{
    ETEXT_UP = 0,
    ETEXT_UP_LEFT = 1,
    ETEXT_UP_RIGHT = 2,
    ETEXT_DOWN = 3,
    ETEXT_DOWN_LEFT = 4,
    ETEXT_DOWN_RIGHT = 5,
	ETEXT_LEFT = 6,
    ETEXT_RIGHT = 7,
	ETEXT_CENTER = 8,
    
} t_etext_justification_flags;

typedef enum ecicm_text_wrap_flags
{
	ETEXT_NOWRAP = 0,
	ETEXT_WRAP = 1
} tecicm_text_wrap_flags;

void cicm_graphics_set_line_width(t_egraphics *g, double width);
void cicm_graphics_set_source_jrgba(t_egraphics *g, t_ergba *rgba);
void cicm_graphics_fill(t_egraphics *g);
void cicm_graphics_fill_preserve(t_egraphics *g);
void cicm_graphics_stroke(t_egraphics *g);
void cicm_graphics_stroke_preserve(t_egraphics *g);
void cicm_text_layout_draw(t_etextlayout* textlayout, t_egraphics *g);

void cicm_graphics_rotate(t_egraphics *g, double angle);
void cicm_graphics_set_matrix(t_egraphics *g, const t_cicm_matrix *matrix);
void cicm_graphics_apply_transforms(t_egraphics *g, long index); // PRIVATE //
void cicm_graphics_clip(t_egraphics *g, long index, int stroked);

int point_in_rect(double x, double y, double xmin, double ymin, double xmax, double ymax);
t_rect CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1, double xmin, double ymin, double xmax, double ymax);

void cicm_graphics_rectangle(t_egraphics *g, double x, double y, double width, double height);
void cicm_graphics_arc(t_egraphics *g, double xc, double yc, double radius, double angle1, double angle2);
void cicm_graphics_arc_negative(t_egraphics *g, double xc, double yc, double radius, double angle1, double angle2);
void cicm_graphics_line_to(t_egraphics *g, double x, double y);
void cicm_graphics_move_to(t_egraphics *g, double x, double y);

t_etextlayout* cicm_text_layout_create();
void cicm_text_layout_destroy(t_etextlayout* textlayout);
void cicm_text_layout_set(t_etextlayout* textlayout, char* text, t_efont *jfont,  double x, double y, double width,  double height, t_etext_justification_flags justification, tecicm_text_wrap_flags wrap);
void cicm_text_layout_settextcolor(t_etextlayout* textlayout, t_ergba* color);
t_efont* cicm_font_create(t_symbol* family, t_symbol* slant, t_symbol* weight, double size);
void cicm_font_destroy(t_efont* font);

void cicm_graphics_close_path(t_egraphics *g);
void cicm_graphics_smooth_line(t_egraphics *g);

void cicm_graphics_matrix_init(t_cicm_matrix *x, double xx, double yx, double xy, double yy, double x0, double y0);

t_ergba cicm_rgba_addContrast(t_ergba baseColor, double contrast, bool preserveAlpha = true);
char* cicm_rgba_to_hex(t_ergba color);

#endif 