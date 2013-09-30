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

#include "egraphics.h"

t_etext* etext_layout_create()
{
    t_etext* new_text_layout = (t_etext *)malloc(sizeof(t_etext));
    new_text_layout->c_color.red = 0.;
    new_text_layout->c_color.green = 0.;
    new_text_layout->c_color.blue = 0.;
    new_text_layout->c_color.alpha = 1.;

    return new_text_layout;
}

void etext_layout_destroy(t_etext* textlayout)
{
    free(textlayout);
}

void etext_layout_set(t_etext* textlayout, char* text, t_efont *jfont,  double x, double y, double width,  double height, t_etextjustify_flags justification, t_etextwrap_flags wrap)
{
    textlayout->c_text = gensym(text);
    textlayout->c_font = jfont[0];
    textlayout->c_rect.x = x;
    textlayout->c_rect.y = y;
    textlayout->c_rect.width = width;
    textlayout->c_rect.height = height;

    if(wrap == ETEXT_NOWRAP)
    {
        textlayout->c_rect.width = 0.;
    }

    if(justification == ETEXT_UP)
        textlayout->c_justify = gensym("n");
    else if(justification == ETEXT_UP_RIGHT)
        textlayout->c_justify = gensym("ne");
    else if(justification == ETEXT_RIGHT)
        textlayout->c_justify = gensym("e");
    else if(justification == ETEXT_DOWN_RIGHT)
        textlayout->c_justify = gensym("se");
    else if(justification == ETEXT_DOWN)
        textlayout->c_justify = gensym("s");
    else if(justification == ETEXT_DOWN_LEFT)
        textlayout->c_justify = gensym("sw");
    else if(justification == ETEXT_LEFT)
        textlayout->c_justify = gensym("w");
    else if(justification == ETEXT_UP_LEFT)
        textlayout->c_justify = gensym("nw");
    else
        textlayout->c_justify = gensym("center");

}

void etext_layout_settextcolor(t_etext* textlayout, t_rgba* color)
{
    textlayout->c_color = color[0];
}

t_efont* efont_create(t_symbol* family, t_symbol* slant, t_symbol* weight, double size)
{
    t_efont* new_font = (t_efont *)malloc(sizeof(t_efont));
    new_font[0].c_family = family;
    
    new_font[0].c_slant = slant;
    if(new_font[0].c_slant  != gensym("italic"))
        new_font[0].c_slant = gensym("regular");
    
    new_font[0].c_weight = weight;
    if(new_font[0].c_weight  != gensym("bold"))
        new_font[0].c_weight = gensym("normal");
    
    new_font[0].c_size = pd_clip_min(size, 1.);
    return new_font;
}

void efont_destroy(t_efont* font)
{
    free(font);
}





