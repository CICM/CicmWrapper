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

#include "ebox.h"

void ebox_draw_background(t_ebox* x, t_glist* glist)
{
    t_egraphics* g = ebox_start_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuibackground"), x->e_rect.width, x->e_rect.height);
    
    if(g)
    {
        cicm_graphics_set_source_jrgba(g, &x->e_boxparameters.d_boxfillcolor);
        cicm_graphics_rectangle(g, 0, 0, x->e_rect.width, x->e_rect.height);
        cicm_graphics_fill(g);
        
        ebox_end_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuibackground"));
    }
    ebox_paint_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuibackground"), 0., 0.);
}

void ebox_draw_text(t_ebox* x, t_glist* glist)
{
    t_egraphics* g = ebox_start_layer((t_object *)x, (t_object *)glist, gensym("cicmboxext"), x->e_rect.width, x->e_rect.height);
    
    if(g)
    {
        t_ergba black = {0., 0., 0., 1.};
        cicm_graphics_set_line_width(g, 1.);
        t_etextlayout *jtl = cicm_text_layout_create();
        cicm_text_layout_settextcolor(jtl, &black);
        cicm_text_layout_set(jtl, x->e_box_text->s_name, &x->e_font, 2., 9, 0., 0., ETEXT_LEFT, ETEXT_NOWRAP);
        cicm_text_layout_draw(jtl, g);
        cicm_text_layout_destroy(jtl);
 
        ebox_end_layer((t_object *)x, (t_object *)glist, gensym("cicmboxext"));
    }
    ebox_paint_layer((t_object *)x, (t_object *)glist, gensym("cicmboxext"), 0., 0.);
}

void ebox_draw_border(t_ebox* x, t_glist* glist)
{
    t_egraphics* g = ebox_start_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuiborder"), x->e_rect.width, x->e_rect.height);
    
    if(g)
    {
        t_ergba black = {0., 0., 0, 1.};
        
        cicm_graphics_set_line_width(g, x->e_boxparameters.d_borderthickness);
        cicm_graphics_set_source_jrgba(g, &x->e_boxparameters.d_bordercolor);
        cicm_graphics_rectangle(g, 0, 0, x->e_rect.width, x->e_rect.height);
        cicm_graphics_stroke(g);
        
        cicm_graphics_set_source_jrgba(g, &black);
        for(int i = 0; i < obj_ninlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            cicm_graphics_rectangle(g, pos_x_inlet, 0, 9, 3);
            cicm_graphics_fill(g);
        }
        
        for(int i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            cicm_graphics_rectangle(g, pos_x_outlet, x->e_rect.height - 2, 9, 3);
            cicm_graphics_fill(g);
        }
        
        ebox_end_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuiborder"));
    }
    ebox_paint_layer((t_object *)x, (t_object *)glist, gensym("cicmboxuiborder"), 0., 0.);
}

void ebox_select(t_ebox* x, t_glist* glist)
{
    t_canvas* canvas = glist_getcanvas(glist);
    if(x->e_selected)
    {
        t_ergba blue = {0., 0., 1., 1.};
        sys_vgui(".x%lx.c itemconfigure %lxcicmboxuiborder0 -outline %s\n", canvas, x, cicm_rgba_to_hex(blue));
    }
    else
        sys_vgui(".x%lx.c itemconfigure %lxcicmboxuiborder0 -outline %s\n", canvas, x, cicm_rgba_to_hex(x->e_boxparameters.d_bordercolor));
}

void ebox_move(t_ebox* x, t_glist* glist)
{
    t_egraphics* g;
    t_canvas *canvas = glist_getcanvas(glist);
    int pos_x = text_xpix(&x->e_obj, glist);
    int pos_y = text_ypix(&x->e_obj, glist);    
    
    char temp[246];
    char coordinates[2048] = "";
    for(long i = 0; i < x->e_graphics.size(); i++)
    {
        g = &x->e_graphics[i];
        for(long j = 0; j < g->c_obj_coords.size(); j++)
        {
            sprintf(coordinates, "");
            for(int k = 0; k < g->c_obj_coords[j].size(); k += 2)
            {
                sprintf(temp, "%d %d ", (int)((int)g->c_obj_coords[j][k] + (int)g->c_offset_x + pos_x), (int)((int)g->c_obj_coords[j][k+1] + (int)g->c_offset_y + pos_y));
                strcat(coordinates, temp);
            }
            sys_vgui(".x%lx.c coords %s %s\n",canvas, g->c_obj_names[j].c_str(), coordinates);
        }
    }
}

void ebox_invalidate_all(t_ebox *x, t_glist *glist)
{   
    for(long i = 0; i < x->e_graphics.size(); i++)
    {
        x->e_graphics[i].c_state = CICM_GRAPHICS_INVALID;
    }
}

void ebox_update(t_ebox *x, t_glist *glist)
{
    t_egraphics* g;
    t_canvas *canvas = glist_getcanvas(glist);
    
    for(long i = 0; i < x->e_graphics.size(); i++)
    {
        g = &x->e_graphics[i];
        if(g->c_state == CICM_GRAPHICS_INVALID)
        {
            for(long j = 0; j < g->c_obj_names.size(); j++)
            {
                sys_vgui(".x%lx.c delete %s\n", canvas, g->c_obj_names[j].c_str());
            }
        }
    }
}

void ebox_erase(t_ebox* x, t_glist* glist)
{
    t_egraphics* g;
    t_canvas *canvas = glist_getcanvas(glist);
    
    for(long i = 0; i < x->e_graphics.size(); i++)
    {
        g = &x->e_graphics[i];
        for(long j = 0; j < g->c_obj_names.size(); j++)
        {
            sys_vgui(".x%lx.c delete %s\n", canvas, g->c_obj_names[j].c_str());
        }
    }
    
    x->e_graphics.clear();
}

t_egraphics* ebox_start_layer(t_object *b, t_object *view, t_symbol *name, double width, double height)
{
    t_ebox* x = (t_ebox*)b;
    t_egraphics* j = NULL;
    t_canvas* c = glist_getcanvas((t_glist *)view);
    char canvas[256];
    sprintf(canvas, ".x%lx.c", (unsigned long)c);
    
    int check = x->e_graphics.size();
    for(int i = 0; i < x->e_graphics.size(); i++)
    {
        if(x->e_graphics[i].c_name == name)
        {
            if(x->e_graphics[i].c_state == CICM_GRAPHICS_INVALID)
            {
                x->e_graphics[i].c_owner = b;
                x->e_graphics[i].c_canvas = c;
                x->e_graphics[i].c_canvas_text.assign(canvas);
                
                x->e_graphics[i].c_obj_names.clear();
                x->e_graphics[i].c_obj_coords.clear();
                x->e_graphics[i].c_obj_types.clear();
                x->e_graphics[i].c_obj_options.clear();
                
                x->e_graphics[i].c_matrix = NULL;
                x->e_graphics[i].c_rotation = 0.;

                
                x->e_graphics[i].c_height = pd_clip_min(height, 0.);
                x->e_graphics[i].c_width  = pd_clip_min(width, 0.);
                x->e_graphics[i].c_state = CICM_GRAPHICS_OPEN;
                j = &x->e_graphics[i];
                return j;
            }
            else
            {
                return NULL;
            }
        }
    }
    t_egraphics temp;
    temp.c_owner = b;
    temp.c_canvas = c;
    temp.c_canvas_text.assign(canvas);
    
    temp.c_height = pd_clip_min(height, 0.);
    temp.c_width  = pd_clip_min(width, 0.);
    temp.c_state = CICM_GRAPHICS_OPEN;
    temp.c_name = name;
    x->e_graphics.push_back(temp);
    if(x->e_graphics.size() > check)
    {
        j = &x->e_graphics[x->e_graphics.size() - 1];
    }
    
    return j;
}

