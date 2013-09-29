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

void egraphics_set_line_width(t_egraphics *g, float width)
{
    g->e_width= pd_clip_min(width, 0.);
}

void egraphics_set_source_jrgba(t_egraphics *g, t_ergba *rgba)
{
    g->e_color = gensym(cicm_rgba_to_hex(*rgba));
}

void egraphics_paint(t_egraphics *g, int filled, int preserved)
{
    if(g->e_new_objects.e_type != E_GOBJ_INVALID)
    {
        g->e_objects = (t_egraphics_obj *)realloc(g->e_objects, (g->e_number_objects + 1) * sizeof(t_egraphics_obj));
        if(g->e_objects)
        {
            long index = g->e_number_objects;
            g->e_number_objects++;
            if (filled)
            {
                g->e_objects[index].e_filled = 1;
            }
            else
            {
                g->e_objects[index].e_filled = 0;
            }
            g->e_objects[index].e_type      = g->e_new_objects.e_type;
            g->e_objects[index].e_angles[0] = g->e_new_objects.e_angles[0];
            g->e_objects[index].e_angles[1] = g->e_new_objects.e_angles[1];
            g->e_objects[index].e_npoints   = g->e_new_objects.e_npoints;
            g->e_objects[index].e_points = (t_pt*)calloc(g->e_objects[index].e_npoints, sizeof(t_pt));
            for (int i = 0; i < g->e_objects[index].e_npoints; i++)
            {
                g->e_objects[index].e_points[i]   = g->e_new_objects.e_points[i];
            }
            g->e_objects[index].e_roundness = g->e_new_objects.e_roundness;
            
            g->e_objects[index].e_color = g->e_color;
            g->e_objects[index].e_width = g->e_width;
            
            char text[256];
            sprintf(text, "%lx%s%ld", (unsigned long)g->e_owner, g->e_name->s_name, index);
    
            g->e_objects[index].e_name = gensym(text);
         
            egraphics_apply_rotation(g, g->e_objects+index);
            egraphics_apply_matrix(g, g->e_objects+index);
            egraphics_clip(g, g->e_objects+index);
            if(!preserved)
            {
                g->e_new_objects.e_roundness = 0;
            }
        }
    }
}

void egraphics_fill_preserve(t_egraphics *g)
{
    egraphics_paint(g, 1, 1);
}

void egraphics_fill(t_egraphics *g)
{
    egraphics_paint(g, 1, 0);
}

void egraphics_stroke_preserve(t_egraphics *g)
{
    egraphics_paint(g, 0, 1);
}

void egraphics_stroke(t_egraphics *g)
{
    egraphics_paint(g, 0, 0);
}
/*
void cicm_graphics_stroke_preserve(t_egraphics *g)
{
    if(g->c_new_obj_type.size() && g->c_new_obj_coords.size())
    {
        char text[256];
        long index = g->c_obj_names.size();
        
        if(g->c_new_obj_type == "polygon ")
        {
            g->c_obj_types.push_back("line ");
            g->c_obj_coords.push_back(g->c_new_obj_coords);
            sprintf(text, "%lx%s%ld", (unsigned long)g->e_owner, g->e_name->s_name, index);
            g->c_obj_names.push_back(text);
            g->c_obj_options.push_back(g->c_new_obj_options);
            sprintf(text, "-fill %s -width %d -tags %s\n", g->e_color->s_name, (int)g->e_width, g->c_obj_names[index].c_str());
            g->c_obj_options[index].append(text);
        }
        else
        {
            g->c_obj_types.push_back(g->c_new_obj_type);
            g->c_obj_coords.push_back(g->c_new_obj_coords);
            sprintf(text, "%lx%s%ld", (unsigned long)g->e_owner, g->e_name->s_name, index);
            g->c_obj_names.push_back(text);
            g->c_obj_options.push_back(g->c_new_obj_options);
            sprintf(text, "-outline %s -width %d -tags %s\n", g->e_color->s_name, (int)g->e_width, g->c_obj_names[index].c_str());
            g->c_obj_options[index].append(text);
        }
        
    }
}

void cicm_text_layout_draw(t_etextlayout* textlayout, t_egraphics *g)
{
    char text[1024];
    long index = g->c_obj_names.size();
    
    sprintf(text, "%lx%s%ld", (unsigned long)g->e_owner, g->e_name->s_name, index);
    g->c_obj_names.push_back(text);
    
    g->c_obj_types.push_back("text ");
    g->c_new_obj_coords.push_back(textlayout->c_rect.x);
    g->c_new_obj_coords.push_back(textlayout->c_rect.y);
    g->c_obj_coords.push_back(g->c_new_obj_coords);
    g->c_new_obj_coords.clear();
    
    g->c_obj_options.push_back("-text ");
    sprintf(text, "{%s} ", textlayout->c_text->s_name);
    g->c_obj_options[index].append(text);
    g->c_obj_options[index].append("-anchor ");
    g->c_obj_options[index].append(textlayout->c_justification->s_name);
    g->c_obj_options[index].append(" -font ");
    sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
    g->c_obj_options[index].append(text);
    sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
    g->c_obj_options[index].append(text);

    //cicm_graphics_apply_transforms(g, index);
    
    // CLIP FOR TEXT //s
    double x1, y1;
    x1 = g->c_obj_coords[index][0];
    y1 = g->c_obj_coords[index][1];
    if(x1 < -1 * (strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75) || x1 > g->e_rect.width || y1 < textlayout->c_font.c_size / 2.  || y1 > g->e_rect.height - textlayout->c_font.c_size / 2.)
    {
        g->c_obj_names.pop_back();
        g->c_obj_types.pop_back();
        g->c_obj_coords.pop_back();
        g->c_obj_options.pop_back();
    }
    else if(x1 > g->e_rect.width - strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75)
    {
        int strsize = (g->e_rect.width - x1) / (textlayout->c_font.c_size / 1.75);
        std::string newtext = textlayout->c_text->s_name;
        newtext.resize(strsize);
        g->c_obj_options[index].assign("-text ");
        sprintf(text, "{%s} ", newtext.c_str());
        g->c_obj_options[index].append(text);
        g->c_obj_options[index].append("-anchor ");
        g->c_obj_options[index].append(textlayout->c_justification->s_name);
        g->c_obj_options[index].append(" -font ");
        sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
        g->c_obj_options[index].append(text);
        sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
        g->c_obj_options[index].append(text);
    }
    else if(x1 < 0)
    {
        int strsize = x1 / (textlayout->c_font.c_size / 1.75) * -1;
        g->c_obj_coords[index][0] = 0.;
        g->c_obj_options[index].assign("-text ");
        sprintf(text, "{%s} ", textlayout->c_text->s_name+strsize);
        g->c_obj_options[index].append(text);
        g->c_obj_options[index].append("-anchor ");
        g->c_obj_options[index].append(textlayout->c_justification->s_name);
        g->c_obj_options[index].append(" -font ");
        sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
        g->c_obj_options[index].append(text);
        sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
        g->c_obj_options[index].append(text);
    }
}
 */

