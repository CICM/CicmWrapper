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

#include "ebox.h"

//! Indicate that an UI ebox should be redraw
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_redraw(t_ebox *x)
{
    t_eclass* c = eobj_getclass(x);
    if(ebox_isdrawable(x) && x->b_have_window)
    {
        ebox_invalidate_layer(x, gensym("eboxbd"));
        ebox_invalidate_layer(x, gensym("eboxio"));
        
        ebox_update(x);
        if(c->c_widget.w_paint)
            c->c_widget.w_paint(x, (t_object *)x->b_obj.o_canvas);
        ebox_draw_border(x);
        if(x->b_obj.o_canvas->gl_edit)
            ebox_draw_iolets(x);
    }
}

//! Initialize a rectangle with the UI ebox coordinates and size
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param rect  The rectangle to initialize
 \ @return      Nothing
*/
void ebox_get_rect_for_view(t_ebox *x, t_rect *rect)
{
    rect->x = x->b_rect.x;
    rect->y = x->b_rect.y;
    rect->width = x->b_rect.width;
    rect->height = x->b_rect.height;
}

//! Allocate and initialize an elayer
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param name      The layer name
 \ @param width     The width of the layer
 \ @param height    The height of the layer
 \ @return          Return an elayer or NULL if layer name is already used or if you can't write the layer
*/
t_elayer* ebox_start_layer(t_ebox *x, t_symbol *name, float width, float height)
{
	int i;
    char text[EPD_MAX_SIGS];
    
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        t_elayer* graphic = &x->b_layers[i];
        if(graphic->e_name == name)
        {
            if(graphic->e_state == EGRAPHICS_INVALID)
            {
                graphic->e_owner        = (t_object *)x;
                
                egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
                graphic->e_line_width   = 1.f;
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
                return &x->b_layers[i];
            }
            else
            {
                return NULL;
            }
        }
    }
    if(x->b_layers == NULL)
        x->b_layers = (t_elayer*)calloc(1, sizeof(t_elayer));
    else
        x->b_layers = (t_elayer*)realloc(x->b_layers, (x->b_number_of_layers + 1) * sizeof(t_elayer));
    if(x->b_layers)
    {
        t_elayer* graphic = x->b_layers+x->b_number_of_layers;
        x->b_number_of_layers++;
        
        graphic->e_owner        = (t_object *)x;
        
        egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
        graphic->e_line_width   = 1.f;
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

//! Indicate that a layer is ready to be painted
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param name      The layer name
 \ @return          Return 0 if the layer exist and -1 if the layer doesn't exist
*/
t_pd_err ebox_end_layer(t_ebox *b, t_symbol *name)
{
	int i;
    t_ebox* x = (t_ebox*)b;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            x->b_layers[i].e_state = EGRAPHICS_TODRAW;
            return 0;
        }
    }
    return -1;
}

//! Invalidate a layer and indicate that you can use it again
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param name      The layer name
 \ @return          Return 0 if the layer exist and -1 if the layer doesn't exist
*/
t_pd_err ebox_invalidate_layer(t_ebox *x, t_symbol *name)
{
	int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            x->b_layers[i].e_state = EGRAPHICS_INVALID;
            return 0;
        }
    }
    return -1;
}

//! Paint a layer
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @param name      The layer name
 \ @param x_p       The layer abscissa offset
 \ @param y_p       The layer ordinate offset
 \ @return          Return 0 if the layer has been painted and -1 if the layer doesn't exist or if the layer isn't ready to be painted
*/
t_pd_err ebox_paint_layer(t_ebox *x, t_symbol *name, float x_p, float y_p)
{
	int i, j;
    float bdsize, start, extent, radius;
    t_elayer* g = NULL;
    bdsize = x->b_boxparameters.d_borderthickness;
    sys_vgui("%s configure -bg %s\n", x->b_drawing_id->s_name, rgba_to_hex(x->b_boxparameters.d_boxfillcolor));
    
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            g = &x->b_layers[i];
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
            ////////////// PATH & LINE ///////////////////////////
            if(gobj->e_type == E_GOBJ_PATH && gobj->e_npoints > 3)
            {
                char header[256];
                char bottom[256];
                int mode = E_PATH_MOVE;
                if(gobj->e_filled)
                {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth true -fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->b_all_id->s_name);
                }
                else
                {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth true -fill %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                }
                
                t_pt * pt;
                for(j = 0; j < gobj->e_npoints; )
                {
                    pt = gobj->e_points+j++;
                    if(pt->x == E_PATH_MOVE)
                    {
                        if(mode == E_PATH_CURVE)
                        {
                            sys_vgui("%s", bottom);
                        }
                        sys_vgui("%s", header);
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p + bdsize), (int)(pt->y + y_p + bdsize));
                        mode = E_PATH_MOVE;
                    }
                    else if(pt->x == E_PATH_CURVE)
                    {
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p + bdsize), (int)(pt->y + y_p + bdsize));
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p + bdsize), (int)(pt->y + y_p + bdsize));
                        pt = gobj->e_points+j++;
                        sys_vgui("%d %d ", (int)(pt->x + x_p + bdsize), (int)(pt->y + y_p + bdsize));
                        mode = E_PATH_CURVE;
                    }
                    else
                    {
                        j++;
                    }
                }
                sys_vgui("%s", bottom);
                
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// RECT ///////////////////////////
            else if(gobj->e_type == E_GOBJ_RECT)
            {
                if(gobj->e_filled)
                    sys_vgui("%s create polygon ", x->b_drawing_id->s_name);
                else
                    sys_vgui("%s create line ", x->b_drawing_id->s_name);
                
                for(j = 0; j < gobj->e_npoints; j ++)
                {
                    sys_vgui("%d %d ", (int)(gobj->e_points[j].x + x_p + bdsize), (int)(gobj->e_points[j].y + y_p + bdsize));
                }
                
                if(gobj->e_filled)
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-fill %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// OVAL /////////////////
            else if (gobj->e_type == E_GOBJ_OVAL)
            {
                sys_vgui("%s create oval %d %d %d %d ",
                         x->b_drawing_id->s_name,
                         (int)(gobj->e_points[0].x + x_p + bdsize),
                         (int)(gobj->e_points[0].y + y_p + bdsize),
                         (int)(gobj->e_points[1].x + x_p + bdsize),
                         (int)(gobj->e_points[1].y + y_p + bdsize));
                if(gobj->e_filled)
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-outline %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// ARC /////////////////
            else if (gobj->e_type == E_GOBJ_ARC)
            {
                start = pd_angle(gobj->e_points[1].x - gobj->e_points[0].x,  gobj->e_points[1].y - gobj->e_points[0].y);
                
                extent = gobj->e_points[2].x;
                radius = gobj->e_points[2].y;

                sys_vgui("%s create arc %d %d %d %d -start %f -extent %f ",
                         x->b_drawing_id->s_name,
                         (int)(gobj->e_points[0].x - radius + x_p + bdsize),
                         (int)(gobj->e_points[0].y - radius + y_p + bdsize),
                         (int)(gobj->e_points[0].x + radius + x_p + bdsize),
                         (int)(gobj->e_points[0].y + radius + y_p + bdsize),
                         (float)start / EPD_2PI * 360.f,
                         (float)extent / EPD_2PI * 360.f);
                
                if(gobj->e_filled)
                    sys_vgui("-style pieslice -fill %s -width 0 -tags { %s %s }\n", gobj->e_color->s_name,  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-style arc -outline %s -width %f -tags { %s %s }\n", gobj->e_color->s_name, gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            ////////////// TEXT ////////////////
            else if(gobj->e_type == E_GOBJ_TEXT)
            {
                
                sys_vgui("%s create text %d %d -text {%s} -anchor %s -justify %s -font {%s %d %s %s} -fill %s -width %d -tags { %s %s }\n",
                         x->b_drawing_id->s_name,
                         (int)(gobj->e_points[0].x + x_p + bdsize),
                         (int)(gobj->e_points[0].y + y_p + bdsize),
                         gobj->e_text->s_name,
                         gobj->e_anchor->s_name,
                         gobj->e_justify->s_name,
                         gobj->e_font.c_family->s_name, (int)gobj->e_font.c_size, gobj->e_font.c_weight->s_name, gobj->e_font.c_slant->s_name,
                         gobj->e_color->s_name,
                         (int)(gobj->e_points[1].x),
                         g->e_id->s_name,
                         x->b_all_id->s_name);
                
                g->e_state = EGRAPHICS_CLOSE;
            }
            else
            {
                return -1;
            }
        }
    }
    else
    {
        return -1;
    }
    
    
    return 0;
}

//! @cond

//! Paint the box border (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_draw_border(t_ebox* x)
{
	float bdsize, bdcorner;
	t_elayer* g = NULL;
    bdcorner = pd_clip_max(x->b_boxparameters.d_cornersize, x->b_boxparameters.d_borderthickness - 1);
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, gensym("eboxbd"), x->b_rect.width, x->b_rect.height);
    
    if(g)
    {
        if(x->b_selected_box == EITEM_OBJ)
        {
            egraphics_set_color_rgba(g, &rgba_blue);
        }
        else
        {
            egraphics_set_color_rgba(g, &x->b_boxparameters.d_bordercolor);
        }
        egraphics_set_line_width(g, bdsize*2);
        egraphics_rectangle_rounded(g, 0, 0, x->b_rect.width+bdsize*2, x->b_rect.height+bdsize*2, bdcorner);
        egraphics_stroke(g);
        
        ebox_end_layer(x, gensym("eboxbd"));
    }
    ebox_paint_layer(x, gensym("eboxbd"), -bdsize, -bdsize);
}

//! Paint the box inlets and outlets (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_draw_iolets(t_ebox* x)
{
    int i;
	float bdsize;
	t_elayer* g = NULL;
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, gensym("eboxio"), x->b_rect.width, x->b_rect.height);
    
    if(g)
    {
        egraphics_set_line_width(g, 1);
        for(i = 0; i < obj_ninlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_inlet, 0, 7, 2);
            if(x->b_selected_inlet == i)
            {
                egraphics_set_color_rgba(g, &rgba_blue);
                egraphics_fill(g);
            }
            else if (obj_issignalinlet((t_object *)x, i))
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
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_outlet, x->b_rect.height - 3 + bdsize*2, 7, 2);
            if(x->b_selected_outlet == i)
            {
                egraphics_set_color_rgba(g, &rgba_blue);
                egraphics_fill(g);
            }
            else if (obj_issignaloutlet((t_object *)x, i))
            {
                egraphics_set_color_rgba(g, &rgba_inletsig);
                egraphics_fill(g);
            }
            else if(obj_isfloatoutlet((t_object *)x, i))
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
        ebox_end_layer(x, gensym("eboxio"));
    }
    ebox_paint_layer(x, gensym("eboxio"), 0, -bdsize);
}

//! Invalidate all the layers (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_invalidate_all(t_ebox *x)
{   
	int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        x->b_layers[i].e_state = EGRAPHICS_INVALID;
    }
}

//! Delete all the invalidated layer in a canvas (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_update(t_ebox *x)
{
	int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_state == EGRAPHICS_INVALID)
        {
            sys_vgui("%s delete %s\n", x->b_drawing_id->s_name, x->b_layers[i].e_id->s_name);
        }
    }
}

//! Delete the canvas and the layers (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_erase(t_ebox* x)
{
    if(x->b_obj.o_canvas && glist_isvisible(x->b_obj.o_canvas) && x->b_have_window)
    {
        sys_vgui("destroy %s \n", x->b_drawing_id->s_name);
        x->b_have_window = 0;
    }
    if(x->b_layers)
	{
		free(x->b_layers);
		x->b_layers = NULL;
	}
    x->b_number_of_layers = 0;
}

//! Notify the canvas that the box has been selected and change the border color (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_select(t_ebox* x)
{
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        if(x->b_selected_box == EITEM_OBJ)
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(rgba_blue));
        }
        else
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(x->b_boxparameters.d_bordercolor));
        }
    }
}

//! Notify the canvas that the box has been moved and change canvas coordinates (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox pointer
 \ @return          Nothing
*/
void ebox_move(t_ebox* x)
{
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        sys_vgui("%s coords %s %d %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness), (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness));
    }
    canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
}



//! @encond







