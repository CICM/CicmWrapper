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


void egraphics_fill(t_egraphics *g);
void egraphics_fill_preserve(t_egraphics *g);
void egraphics_stroke(t_egraphics *g);
void egraphics_stroke_preserve(t_egraphics *g);
void cicm_text_layout_draw(t_etextlayout* textlayout, t_egraphics *g);

void egraphics_set_line_width(t_egraphics *g, float width);
void egraphics_set_source_jrgba(t_egraphics *g, t_ergba *rgba);
void egraphics_rotate_set(t_egraphics *g, float angle);
void egraphics_rotate_add(t_egraphics *g, float angle);
void egraphics_matrix_init(t_ematrix *x, float xx, float yx, float xy, float yy, float x0, float y0);
void egraphics_set_matrix(t_egraphics *g, const t_ematrix *matrix);

void egraphics_apply_rotation(t_egraphics *g, t_egraphics_obj* gobj); // PRIVATE //
void egraphics_apply_matrix(t_egraphics *g, t_egraphics_obj* gobj);   // PRIVATE //
void egraphics_clip(t_egraphics *g, t_egraphics_obj* gobj);           // PRIVATE //

void egraphics_line_to(t_egraphics *g, float x, float y);
void egraphics_move_to(t_egraphics *g, float x, float y);
void egraphics_line(t_egraphics *g, float x0, float y0,  float x1, float y1);
void egraphics_rectangle(t_egraphics *g, float x, float y, float width, float height);
void egraphics_rectangle_rounded(t_egraphics *g, float x, float y, float width, float height, float roundness);

/*
t_etextlayout* cicm_text_layout_create();
void cicm_text_layout_destroy(t_etextlayout* textlayout);
void cicm_text_layout_set(t_etextlayout* textlayout, char* text, t_efont *jfont,  double x, double y, double width,  double height, t_etext_justification_flags justification, tecicm_text_wrap_flags wrap);
void cicm_text_layout_settextcolor(t_etextlayout* textlayout, t_ergba* color);
t_efont* cicm_font_create(t_symbol* family, t_symbol* slant, t_symbol* weight, double size);
void cicm_font_destroy(t_efont* font);
*/
//void cicm_graphics_close_path(t_egraphics *g);
//void cicm_graphics_smooth_line(t_egraphics *g);

void cicm_graphics_matrix_init(t_ematrix *x, double xx, double yx, double xy, double yy, double x0, double y0);

t_ergba cicm_rgba_addContrast(t_ergba baseColor, double contrast, bool preserveAlpha = true);
char* cicm_rgba_to_hex(t_ergba color);

#endif 