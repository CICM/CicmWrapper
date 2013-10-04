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

t_symbol* egraphics_clip_text_perform(t_rect rect, t_pt* pt, float size, t_symbol* text, t_symbol* justify)
{
    //We assume that people use the system font...
    float width = sys_fontwidth(size);
    float height = sys_fontheight(size);
    
    if(justify == gensym("center") || justify == gensym("w") || justify == gensym("e")) // y center
    {
        if(pt->y - height * 0.5 < rect.y || pt->y + height * 0.5 > rect.y + rect.height)
            return NULL;
    }
    else if (justify == gensym("n") || justify == gensym("nw") || justify == gensym("ne")) // y down
    {
        if(pt->y < rect.y || pt->y + height > rect.y + rect.height)
            return NULL;
    }
    else if (justify == gensym("s") || justify == gensym("sw") || justify == gensym("se")) // y up
    {
        if(pt->y - height < rect.y || pt->y > rect.y + rect.height)
            return NULL;
    }
    
    if(justify == gensym("center") || justify == gensym("s") || justify == gensym("n")) // x center
    {
        if(pt->x - width * strlen(text->s_name) * 0.5 < rect.x)
        {
            int diff = (rect.x - (pt->x - width * strlen(text->s_name) * 0.5)) / width + 0.5;
            text = gensym(text->s_name+diff);
            pt->x += diff * width;
        }
        if(pt->x + width * strlen(text->s_name) * 0.5 > rect.x + rect.height)
        {
            char temp[1024];
            int diff = ((pt->x + width * strlen(text->s_name) * 0.5) - (rect.x + rect.width)) / width + 0.5;
            snprintf(temp, strlen(text->s_name) - diff, "%s", text->s_name);
            text = gensym(temp);
        }
    }
    else if (justify == gensym("e") || justify == gensym("ne") || justify == gensym("se")) // x right
    {
        if(pt->x - width * strlen(text->s_name) < rect.x)
        {
            int diff = (rect.x - (pt->x - width * strlen(text->s_name))) / width + 0.5;
            text = gensym(text->s_name+diff);
            pt->x += diff * width;
        }
        if(pt->x > rect.x + rect.width)
        {
            char temp[1024];
            int diff = ((pt->x) - (rect.x + rect.width)) / width + 0.5;
            snprintf(temp, strlen(text->s_name) - diff, "%s", text->s_name);
            text = gensym(temp);
        }
    }
    else if (justify == gensym("w") || justify == gensym("nw") || justify == gensym("sw")) // x left
    {
        if(pt->x < rect.x)
        {
            int diff = (rect.x - pt->x) / width + 0.5;
            text = gensym(text->s_name+diff);
            pt->x += diff * width;
        }
        if(pt->x + width * strlen(text->s_name) > rect.x + rect.width)
        {
            char temp[1024];
            int diff = ((pt->x + width * strlen(text->s_name)) - (rect.x + rect.width)) / width + 0.5;
            snprintf(temp, strlen(text->s_name) - diff, "%s", text->s_name);
            text = gensym(temp);
        }
    }
    
    return text;
}

void egraphics_clip_text(t_elayer *g, t_egobj* gobj)
{
    gobj->e_text = egraphics_clip_text_perform(g->e_rect, gobj->e_points, gobj->e_font.c_size, gobj->e_text, gobj->e_justify);
    if(gobj->e_text == NULL)
    {
        free(gobj->e_points);
        gobj->e_type = E_GOBJ_INVALID;
    }
}



