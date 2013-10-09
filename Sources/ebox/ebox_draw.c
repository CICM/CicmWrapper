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
    /*
    t_elayer* g = ebox_start_layer((t_object *)x, (t_object *)glist, gensym("eboxbg"), x->e_rect.width, x->e_rect.height);
    
    if(g)
    {
        egraphics_set_color_rgba(g, &x->e_boxparameters.d_boxfillcolor);
        egraphics_rectangle_rounded(g, 0, 0, x->e_rect.width + 100, x->e_rect.height, x->e_boxparameters.d_cornersize);
        egraphics_fill(g);
        
        ebox_end_layer((t_object *)x, (t_object *)glist, gensym("eboxbg"));
    }
    ebox_paint_layer((t_object *)x, (t_object *)glist, gensym("eboxbg"), 0, 0);
     */
}

void ebox_draw_border(t_ebox* x, t_glist* glist)
{
	int i;
	float bdsize;
	t_elayer* g;
    x->e_boxparameters.d_borderthickness = pd_clip_max(x->e_boxparameters.d_borderthickness, x->e_rect.width / 2.);
    bdsize = (x->e_boxparameters.d_borderthickness - 1.) * 0.5;
    g = ebox_start_layer((t_object *)x, (t_object *)glist, gensym("eboxbd"), x->e_rect.width + bdsize * 2 + 1, x->e_rect.height + bdsize * 2 + 1);
    
    if(glist_isvisible(glist))
    {
        if(x->e_selected)
        {
            egraphics_set_color_rgba(g, &rgba_blue);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->e_boxparameters.d_bordercolor);
        }
        egraphics_set_line_width(g, x->e_boxparameters.d_borderthickness);
        egraphics_rectangle_rounded(g, bdsize, bdsize, x->e_rect.width, x->e_rect.height, x->e_boxparameters.d_cornersize);
        egraphics_stroke(g);
        
        egraphics_set_line_width(g, 1.);
        
        for(i = 0; i < obj_ninlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            egraphics_rectangle(g, pos_x_inlet, -bdsize, 7, 3);
            if (obj_issignalinlet((t_object *)x, i))
            {
                egraphics_set_color_rgba(g, &rgba_inletsig);
                egraphics_fill(g);
            }
            else if(obj_isfloatinlet((t_object *)x, i))
            {
                egraphics_set_color_rgba(g, &rgba_black);
                egraphics_fill(g);
            }
            else
            {
                egraphics_set_color_rgba(g, &rgba_white);
                egraphics_fill(g);
                egraphics_set_color_rgba(g, &rgba_black);
                egraphics_stroke(g);
            }
        }
        
        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->e_rect.width - 8));
            
            if (obj_issignaloutlet((t_object *)x, i))
            {
                egraphics_rectangle(g, pos_x_outlet, x->e_rect.height - 2 + bdsize, 7, 2);
                egraphics_set_color_rgba(g, &rgba_inletsig);
                egraphics_fill(g);
            }
            else if(obj_isfloatoutlet((t_object *)x, i))
            {
                egraphics_rectangle(g, pos_x_outlet, x->e_rect.height - 2 + bdsize, 7, 2);
                egraphics_set_color_rgba(g, &rgba_black);
                egraphics_fill(g);
            }
            else
            {
                egraphics_rectangle(g, pos_x_outlet, x->e_rect.height - 2 + bdsize, 7, 2);
                egraphics_set_color_rgba(g, &rgba_white);
                egraphics_fill(g);
                egraphics_set_color_rgba(g, &rgba_black);
                egraphics_stroke(g);
            }
        }
        
        ebox_end_layer((t_object *)x, (t_object *)glist, gensym("eboxbd"));
    }
    ebox_paint_layer((t_object *)x, (t_object *)glist, gensym("eboxbd"), -bdsize, -bdsize);
}

void ebox_select(t_ebox* x, t_glist* glist)
{
    t_canvas* canvas = glist_getcanvas(glist);
    if(glist_isvisible(glist))
    {
        if(x->e_selected)
        {
            sys_vgui(".x%lx.c itemconfigure %lxeboxbd0 -fill %s\n", canvas, x,rgba_to_hex(rgba_blue));
        }
        else
        {
            sys_vgui(".x%lx.c itemconfigure %lxeboxbd0 -fill %s\n", canvas, x,rgba_to_hex(x->e_boxparameters.d_bordercolor));
        }
    }
}

void ebox_move(t_ebox* x, t_glist* glist)
{
    if(glist_isvisible(glist))
    {
        sys_vgui("%s coords %s %f %f\n", x->e_canvas_id->s_name, x->e_window_id->s_name, x->e_rect.x, x->e_rect.y);
    }
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ebox_invalidate_all(t_ebox *x, t_glist *glist)
{   
	int i;
    for(i = 0; i < x->e_number_of_layers; i++)
    {
        x->e_layers[i].e_state = EGRAPHICS_INVALID;
    }
}

void ebox_update(t_ebox *x, t_glist *glist)
{
	int i;
    if(glist_isvisible(x->e_canvas))
    {
        for(i = 0; i < x->e_number_of_layers; i++)
        {
            if(x->e_layers[i].e_state == EGRAPHICS_INVALID)
            {
                sys_vgui("%s delete %s\n", x->e_drawing_id->s_name, x->e_layers[i].e_id->s_name);
            }
        }
    }
}

void ebox_erase(t_ebox* x, t_glist* glist)
{
    if(glist_isvisible(x->e_canvas))
    {
        sys_vgui("destroy %s \n", x->e_drawing_id->s_name);
    }
    
    free(x->e_layers);
    x->e_number_of_layers = 0;
}

t_elayer* ebox_start_layer(t_object *b, t_object *view, t_symbol *name, float width, float height)
{
	int i;
    char text[256];
    t_ebox* x = (t_ebox*)b;   
    for(i = 0; i < x->e_number_of_layers; i++)
    {
        t_elayer* graphic = &x->e_layers[i];
        if(graphic->e_name == name)
        {
            if(graphic->e_state == EGRAPHICS_INVALID)
            {
                graphic->e_owner        = b;
                
                egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
                graphic->e_width        = 1.f;
                graphic->e_color        = gensym("#000000");
                graphic->e_rect.x       = 0.f;
                graphic->e_rect.y       = 0.f;
                graphic->e_rect.height  = pd_clip_min(height, 0.);
                graphic->e_rect.width   = pd_clip_min(width, 0.);
                
                graphic->e_number_objects  = 0;
                if(graphic->e_new_objects.e_points)
                    free(graphic->e_new_objects.e_points);
                graphic->e_new_objects.e_points = NULL;
                graphic->e_new_objects.e_npoints = 0;
                graphic->e_new_objects.e_roundness = 0.;
                graphic->e_objects      = NULL;
                sprintf(text, "%s%ld", name->s_name, (long)x);
                graphic->e_id          = gensym(text);
                
                graphic->e_state        = EGRAPHICS_OPEN;
                return &x->e_layers[i];
            }
            else
            {
                return NULL;
            }
        }
    }
    if(x->e_layers == NULL)
        x->e_layers = (t_elayer*)calloc(1, sizeof(t_elayer));
    else
        x->e_layers = (t_elayer*)realloc(x->e_layers, (x->e_number_of_layers + 1) * sizeof(t_elayer));
    if(x->e_layers)
    {
        t_elayer* graphic = x->e_layers+x->e_number_of_layers;
        x->e_number_of_layers++;
        
        graphic->e_owner        = b;

        egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
        graphic->e_width    = 1.f;
        graphic->e_color        = gensym("#000000");
        graphic->e_rect.x       = 0.f;
        graphic->e_rect.y       = 0.f;
        graphic->e_rect.height  = pd_clip_min(height, 0.);
        graphic->e_rect.width   = pd_clip_min(width, 0.);
        
        
        graphic->e_number_objects  = 0;
        graphic->e_new_objects.e_points = NULL;
        graphic->e_new_objects.e_npoints = 0;
        graphic->e_new_objects.e_roundness = 0.;
        graphic->e_objects      = NULL;
        
        graphic->e_state        = EGRAPHICS_OPEN;
        graphic->e_name         = name;
        sprintf(text, "%s%ld", name->s_name, (long)x);
        graphic->e_id          = gensym(text);
        return graphic;
    }
    else
    {
        return NULL;
    }
}

t_pd_err ebox_end_layer(t_object *b, t_object *view, t_symbol *name)
{
	int i;
    t_ebox* x = (t_ebox*)b;
    for(i = 0; i < x->e_number_of_layers; i++)
    {
        if(x->e_layers[i].e_name == name)
        {
            x->e_layers[i].e_state = EGRAPHICS_TODRAW;
            return 0;
        }
    }
    
    return 0;
}

t_pd_err ebox_invalidate_layer(t_object *b, t_object *view, t_symbol *name)
{
	int i;
    t_ebox* x = (t_ebox*)b;
    for(i = 0; i < x->e_number_of_layers; i++)
    {
        if(x->e_layers[i].e_name == name)
        {
            x->e_layers[i].e_state = EGRAPHICS_INVALID;
            return 0;
        }
    }
    return -1;
}

t_pd_err ebox_paint_layer(t_object *b, t_object *view, t_symbol *name, float x_p, float y_p)
{
	int i, j;
    t_ebox* x = (t_ebox *)b;
    t_elayer* g = NULL;
    
    sys_vgui("%s configure -bg %s -width %i -height %i\n", x->e_drawing_id->s_name, rgba_to_hex(x->e_boxparameters.d_boxfillcolor), (int)x->e_rect.width, (int)x->e_rect.height);
    sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->e_canvas_id->s_name, x->e_window_id->s_name, (int)x->e_rect.width, (int)x->e_rect.height);
    canvas_fixlinesfor(x->e_canvas, (t_text *)x);
    for(i = 0; i < x->e_number_of_layers; i++)
    {
        if(x->e_layers[i].e_name == name)
        {
            g = &x->e_layers[i];
            if(g->e_state != EGRAPHICS_TODRAW)
            {
                return -1;
            }
        }
    }
    if(g)
    {
        
        for(i = 0; i < g->e_number_objects; i++)
        {
            t_egobj* gobj = g->e_objects+i;
            ////////////// PATH, LINE AND RECT ///////////////////////////
            if(gobj->e_type == E_GOBJ_PATH || gobj->e_type == E_GOBJ_RECT)
            {
                if(gobj->e_filled)
                    sys_vgui("%s create polygon ", x->e_drawing_id->s_name);
                else
                    sys_vgui("%s create line ", x->e_drawing_id->s_name);
                
                for(j = 0; j < gobj->e_npoints; j ++)
                    sys_vgui("%d %d ", (int)(gobj->e_points[j].x + x_p), (int)(gobj->e_points[j].y + y_p));
                
                if(gobj->e_filled)
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->e_all_id->s_name);
                else
                    sys_vgui("-fill %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->e_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// OVAL /////////////////
            else if (gobj->e_type == E_GOBJ_OVAL)
            {
                sys_vgui("%s create oval %d %d %d %d ",
                        x->e_drawing_id->s_name,
                        (int)(gobj->e_points[0].x + x_p),
                        (int)(gobj->e_points[0].y + y_p),
                        (int)(gobj->e_points[1].x + x_p),
                        (int)(gobj->e_points[1].y + y_p));
                if(gobj->e_filled)
                     sys_vgui("-fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->e_all_id->s_name);
                else
                    sys_vgui("-outline %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->e_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// ARC /////////////////
            else if (gobj->e_type == E_GOBJ_ARC)
            {
                sys_vgui("%s create arc %d %d %d %d -start %f -extent %f",
                         x->e_drawing_id->s_name,
                         (int)(gobj->e_points[0].x + x_p),
                         (int)(gobj->e_points[0].y + y_p),
                         (int)(gobj->e_points[1].x + x_p),
                         (int)(gobj->e_points[1].y + y_p),
                         gobj->e_points[2].x,
                         gobj->e_points[2].y);
                if(gobj->e_filled)
                    sys_vgui("-style pieslice -fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->e_all_id->s_name);
                else
                    sys_vgui("-style arc -outline %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->e_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// TEXT ////////////////
            else if(gobj->e_type == E_GOBJ_TEXT)
            {
                
                sys_vgui("%s create text %d %d -text {%s} -anchor %s -font {%s %d %s} -fill %s -width %d -tags { %s %s }\n",
                         x->e_drawing_id->s_name,
                         (int)(gobj->e_points[0].x + x_p),
                         (int)(gobj->e_points[0].y + y_p),
                         gobj->e_text->s_name,
                         gobj->e_justify->s_name,
                         gobj->e_font.c_family->s_name, (int)gobj->e_font.c_size, gobj->e_font.c_weight->s_name,
                         gobj->e_color->s_name,
                         (int)(gobj->e_points[1].x),
                         g->e_id->s_name,
                         x->e_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            else
            {
                error("Invalid layer object %s : %i", x->e_layers[i].e_name->s_name, i);
                return -1;
            }
        }
    }
    else
    {
        error("Invalid layer name %s", name->s_name);
        return -1;
    }
    
    
    return 0;
}


