/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "ebox.h"
#include "egraphics.h"
#include "eobj.h"
#include "eclass.h"
#include "epopup.h"
#include "float.h"

static char *my_cursorlist[] =
{
    "left_ptr",
    "center_ptr",
    "sb_v_double_arrow",
    "plus",
    "hand2",
    "circle",
    "X_cursor",
    "bottom_side",
    "bottom_right_corner",
    "right_side",
    "double_arrow",
    "exchange",
    "xterm"
};

static void ebox_create_window(t_ebox* x, t_glist* glist);
static void ebox_invalidate_all(t_ebox *x);
static void ebox_draw_border(t_ebox* x);
static void ebox_draw_iolets(t_ebox* x);
static void ebox_update(t_ebox *x);
static void ebox_erase(t_ebox* x);
static void ebox_select(t_ebox* x);
static void ebox_move(t_ebox* x);

void ebox_new(t_ebox *x, long flags)
{
    x->b_flags = flags;
    x->b_ready_to_draw      = 0;
    x->b_have_window        = 0;
    x->b_number_of_layers   = 0;
    x->b_layers             = NULL;
    x->b_window_id          = s_cream_empty;
    x->b_receive_id         = s_cream_empty;
    x->b_send_id            = s_cream_empty;
    x->b_preset_id          = s_cream_empty;
    x->b_visible            = 1;
    x->b_params             = NULL;
    x->b_nparams            = 0;
    eclass_attrs_setdefault((t_object *)x);
}

void ebox_ready(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->b_obj.o_obj.te_g.g_pd;
    x->b_selected_item  = EITEM_NONE;
    x->b_selected_box   = 0;
    x->b_selected_inlet = -1;
    x->b_selected_outlet= -1;
    x->b_mouse_down     = 0;

    x->b_boxparameters.d_bordercolor = rgba_black;
    x->b_boxparameters.d_borderthickness = 1;
    x->b_boxparameters.d_boxfillcolor = rgba_white;
    x->b_boxparameters.d_cornersize = 0;
    if(c->c_widget.w_getdrawparameters)
        c->c_widget.w_getdrawparameters(x, NULL, &x->b_boxparameters);
    x->b_ready_to_draw = 1;
}

void ebox_free(t_ebox* x)
{
    int i, j;
    t_elayer *layer;
    eobj_free(x);
    if(is_valid_symbol(x->b_receive_id))
    {
        pd_unbind((t_pd *)x, x->b_receive_id);
    }
    if(eobj_isdsp(x))
    {
        eobj_dspfree(x);
    }
    if(x->b_number_of_layers && x->b_layers)
    {
        for(i = 0; i < x->b_number_of_layers; i++)
        {
            layer = x->b_layers+i;
            if(layer->e_objects && layer->e_number_objects)
            {
                for(j = 0; j < layer->e_number_objects; j++)
                {
                    if(layer->e_objects[j].e_points)
                    {
                        free(layer->e_objects[j].e_points);
                    }
                    if(layer->e_objects[j].e_type == E_GOBJ_TEXT && layer->e_objects[j].e_text)
                    {
                        free(layer->e_objects[j].e_text);
                    }
                }
                free(layer->e_objects);
            }
            if(layer->e_new_objects.e_npoints && layer->e_new_objects.e_points)
            {
                free(layer->e_new_objects.e_points);
            }
            if(layer->e_new_objects.e_text && layer->e_new_objects.e_type == E_GOBJ_TEXT)
            {
                free(layer->e_new_objects.e_text);
            }
        }
        free(x->b_layers);
    }
    
    if(x->b_nparams && x->b_params)
    {
        for(i = 0; i < x->b_nparams; i++)
        {
            ebox_parameter_destroy(x, i);
        }
        free(x->b_params);
    }
}

t_pd* ebox_getsender(t_ebox* x)
{
    t_symbol* sname;
    if(is_valid_symbol(x->b_send_id))
    {
        sname = canvas_realizedollar(eobj_getcanvas(x), x->b_send_id);
        if(sname && sname->s_thing)
        {
            return x->b_send_id->s_thing;
        }
    }
    return NULL;
}

char ebox_isdrawable(t_ebox* x)
{
    if(eobj_isbox(x) && x->b_obj.o_canvas)
    {
        if(x->b_ready_to_draw && glist_isvisible(x->b_obj.o_canvas))
        {
            return 1;
        }
    }
    return 0;
}

void ebox_set_cursor(t_ebox* x, int cursor)
{
    if(x->b_drawing_id)
    {
        cursor = (int)pd_clip(cursor, 0, 12);
        sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[cursor]);
    }
    else
    {
        int todo;
    }
}

void ebox_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    int     defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);
    t_ebox* z = (t_ebox *)x;
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
    if(eobj_isbox(x))
    {
        for(i = 0; i < z->b_nparams; i++)
        {
            sprintf(buffer, "@param%i", i);
            atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
            if(defc && defv)
            {
                if(atom_gettype(defv) == A_SYMBOL)
                    ebox_parameter_setname(x, i+1, atom_getsymbol(defv));
                if(defc > 1 && atom_gettype(defv+1) == A_SYMBOL)
                    ebox_parameter_setlabel(x, i+1, atom_getsymbol(defv+1));
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
    }
}

void ebox_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    int i;
    char buffer[MAXPDSTRING];

    int defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    t_ebox* z = (t_ebox *)x;
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
    if(eobj_isbox(x))
    {
        for(i = 0; i < z->b_nparams; i++)
        {
            sprintf(buffer, "@param%i", i);
            binbuf_get_attribute(d, gensym(buffer), &defc, &defv);
            if(defc && defv)
            {
                if(atom_gettype(defv) == A_SYMBOL)
                    ebox_parameter_setname(x, i+1, atom_getsymbol(defv));
                if(defc > 1 && atom_gettype(defv+1) == A_SYMBOL)
                    ebox_parameter_setlabel(x, i+1, atom_getsymbol(defv+1));
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
    }
}

//! Widget
void ebox_wgetrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_ebox *x = (t_ebox *)z;
    *xp1 = text_xpix(&x->b_obj.o_obj, glist);
    *yp1 = text_ypix(&x->b_obj.o_obj, glist) - (int)(x->b_boxparameters.d_borderthickness);
    *xp2 = text_xpix(&x->b_obj.o_obj, glist) + (int)x->b_rect.width + (int)(x->b_boxparameters.d_borderthickness);
    *yp2 = text_ypix(&x->b_obj.o_obj, glist) + (int)x->b_rect.height + (int)(x->b_boxparameters.d_borderthickness);
}

static void ebox_paint(t_ebox *x)
{
    t_eclass* c = eobj_getclass(x);
    ebox_update(x);
    sys_vgui("%s configure -bg %s\n", x->b_drawing_id->s_name, rgba_to_hex(&(x->b_boxparameters.d_boxfillcolor)));
    if(x->b_pinned)
    {
        sys_vgui((char *)"lower %s\n", x->b_drawing_id->s_name);
    }
    if(c->c_widget.w_paint)
    {
        c->c_widget.w_paint(x, (t_object *)eobj_getcanvas(x));
    }
    ebox_draw_border(x);
    ebox_draw_iolets(x);
}

//! Widget
void ebox_wvis(t_gobj *z, t_glist *glist, int vis)
{
    t_ebox* x   = (t_ebox *)z;
    if(vis)
    {
        if(eobj_isbox(x) && x->b_ready_to_draw && x->b_visible)
        {
            ebox_invalidate_all(x);
            ebox_create_window(x, glist);
            ebox_paint(x);
        }
    }
    else
    {
        ebox_erase(x);
        canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
    }
}

//! Widget
void ebox_wdisplace(t_gobj *z, t_glist *glist, int dx, int dy)
{
#ifdef _WINDOWS
    t_ebox* x = (t_ebox *)z;
    if(x->b_selected_box)
    {
        x->b_rect.x += dx;
        x->b_rect.y += dy;
        x->b_obj.o_obj.te_xpix += dx;
        x->b_obj.o_obj.te_ypix += dy;
        ebox_move(x);
    }
#else
    t_ebox* x = (t_ebox *)z;

    x->b_rect.x += dx;
    x->b_rect.y += dy;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix += dy;
    ebox_move(x);
#endif
}

//! Widget
void ebox_wselect(t_gobj *z, t_glist *glist, int selected)
{
    t_ebox *x = (t_ebox *)z;
    if(selected)
        x->b_selected_box = 1;
    else
        x->b_selected_box = 0;
    ebox_select(x);
}

//! Widget
void ebox_wdelete(t_gobj *z, t_glist *glist)
{
    t_ebox *x = (t_ebox *)z;
    ebox_erase(x);
    canvas_deletelinesfor(glist, (t_text *)z);
}

//! Initialize all the ebox ids (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param canvas    The ebox's canvas
 \ @return          Nothing
 */
static void ebox_tk_ids(t_ebox *x, t_canvas *canvas)
{
    char buffer[MAXPDSTRING];
    x->b_obj.o_canvas = canvas;
    sprintf(buffer,".x%lx.c", (long unsigned int) canvas);
    x->b_canvas_id = gensym(buffer);
    sprintf(buffer,"%s.ecanvas%lx", x->b_canvas_id->s_name, (long unsigned int)x);
    x->b_drawing_id = gensym(buffer);
    sprintf(buffer,"%s.ewindow%lx", x->b_canvas_id->s_name, (long unsigned int)x);
    x->b_window_id = gensym(buffer);
    sprintf(buffer,"all%lx", (long unsigned int)x);
    x->b_all_id = gensym(buffer);
}

static void ebox_bind_events(t_ebox* x)
{
    t_eclass* c = (t_eclass *)eobj_getclass(x);
    sys_vgui("bind %s <Button-3> {+pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
    sys_vgui("bind %s <Button-2> {+pdsend {%s mousedown %%x %%y %i}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name, EMOD_RIGHT);
    sys_vgui("bind %s <Button-1> {+pdsend {%s mousedown %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <ButtonRelease> {+pdsend {%s mouseup %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Motion> {+pdsend {%s mousemove %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    sys_vgui("bind %s <Enter> {+pdsend {%s mouseenter}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    sys_vgui("bind %s <Leave> {+pdsend {%s mouseleave}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);

    if(c->c_widget.w_dblclick)
    {
        sys_vgui("bind %s <Double-Button-1> {+pdsend {%s dblclick %%x %%y %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
    if(c->c_widget.w_mousewheel)
    {
        sys_vgui("bind %s <MouseWheel> {+pdsend {%s mousewheel  %%x %%y %%D %%s}}\n", x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
    if(c->c_widget.w_key || c->c_widget.w_keyfilter)
    {
        sys_vgui("bind %s <Key>  {+pdsend {%s key  %%k %%N}} \n",  x->b_drawing_id->s_name, x->b_obj.o_id->s_name);
    }
}

static void ebox_create_widget(t_ebox* x)
{
    sys_vgui("namespace eval ebox%lx {} \n", x);
    sys_vgui("destroy %s \n", x->b_drawing_id->s_name);

    sys_vgui("canvas %s -width %d -height %d -bd 0 -highlightthickness 0 -insertborderwidth 0 -state normal -takefocus 1 -insertwidth 0 -confine 0\n",
             x->b_drawing_id->s_name,
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));
}

static void ebox_create_window(t_ebox* x, t_glist* glist)
{
    x->b_have_window = 0;
    if(!glist->gl_havewindow)
    {
        x->b_isinsubcanvas = 1;
        x->b_rect.x = x->b_obj.o_obj.te_xpix;
        x->b_rect.y = x->b_obj.o_obj.te_ypix;

        while(!glist->gl_havewindow)
        {
            x->b_rect.x -= glist->gl_xmargin;
            x->b_rect.y -= glist->gl_ymargin;
            x->b_rect.x += glist->gl_obj.te_xpix;
            x->b_rect.y += glist->gl_obj.te_ypix;
            glist = glist->gl_owner;
        }
    }
    else
    {
        x->b_isinsubcanvas = 0;
        x->b_rect.x = x->b_obj.o_obj.te_xpix;
        x->b_rect.y = x->b_obj.o_obj.te_ypix;
    }

    ebox_tk_ids(x, glist_getcanvas(glist));
    ebox_create_widget(x);
    ebox_bind_events(x);

    sys_vgui("%s create window %d %d -anchor nw -window %s -tags %s -width %d -height %d\n",
             x->b_canvas_id->s_name,
             (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness),
             (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness),
             x->b_drawing_id->s_name,
             x->b_window_id->s_name,
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));

    x->b_have_window = 1;
}

static char is_for_box(t_ebox* x, long mod)
{
    return (!x->b_obj.o_canvas->gl_edit || (x->b_obj.o_canvas->gl_edit && mod == EMOD_CMD));
}

static long modifier_wrapper(long mod)
{
#ifdef __APPLE__
    if(mod >= 256)
    {
        mod -= 256;
    }
#elif _WINDOWS

    if(mod >= 131072)
    {
        mod -= 131072;
        mod += EMOD_ALT;
    }
    /*else
        mod -= 8;*/
#else
    if (mod == 24)//right click
        mod = EMOD_CMD;
    else if (mod & EMOD_CMD)
    {
        mod ^= EMOD_CMD;
        mod |= EMOD_ALT;
    }
#endif
    //post("MOD : %ld", mod);
    return mod;
}

void ebox_mouse_enter(t_ebox* x)
{
    t_eclass *c = eobj_getclass(x);

    if(!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        sys_vgui("focus %s\n", x->b_drawing_id->s_name);
        if(c->c_widget.w_mouseenter)
        {
            c->c_widget.w_mouseenter(x);
        }
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = eobj_getclass(x);

    if(!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        if(c->c_widget.w_mouseleave)
        {
            c->c_widget.w_mouseleave(x);
        }
        ebox_set_cursor(x, 0);
    }
    else if(x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        ebox_set_cursor(x, 4);
    }
}

void ebox_mouse_move(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    int i;
    int right, bottom;
    t_pt mouse;
    t_atom av[2];
    long modif = modifier_wrapper((long)atom_getfloat(argv+2));
    t_eclass *c = eobj_getclass(x);
    if(!x->b_mouse_down)
    {
        if(is_for_box(x, modif))
        {
            if(!(x->b_flags & EBOX_IGNORELOCKCLICK))
            {
                ebox_set_cursor(x, 1);
                if(c->c_widget.w_mousemove)
                {
                    mouse.x = atom_getfloat(argv);
                    mouse.y = atom_getfloat(argv+1);
                    c->c_widget.w_mousemove(x, x->b_obj.o_canvas, mouse, modif);
                }
            }
            else
            {
                ebox_set_cursor(x, 0);
            }
        }
        else if(!x->b_isinsubcanvas)
        {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv+1);
            x->b_selected_outlet    = -1;
            x->b_selected_inlet     = -1;
            x->b_selected_item      = EITEM_NONE;
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);

            right   = (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.);
            bottom  = (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.);

            // TOP //
            if(mouse.y >= 0 && mouse.y < 3)
            {
                for(i = 0; i < obj_noutlets((t_object *)x); i++)
                {
                    int pos_x_inlet = 0;
                    if(obj_ninlets((t_object *)x) != 1)
                        pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->b_rect.width - 8));

                    if(mouse.x >= pos_x_inlet && mouse.x <= pos_x_inlet +7)
                    {
                        x->b_selected_inlet = i;
                        ebox_set_cursor(x, 4);
                        break;
                    }
                }
                ebox_invalidate_layer(x, s_cream_eboxio);
                ebox_redraw(x);
                return;
            }
            // BOTTOM & RIGHT //
            else if(mouse.y > bottom - 3 && mouse.y <= bottom && mouse.x > right - 3 && mouse.x <= right)
            {
                x->b_selected_item = EITEM_CORNER;
                ebox_set_cursor(x, 8);
                return;
            }
            // BOTTOM //
            else if(mouse.y > bottom - 3 && mouse.y < bottom)
            {
                for(i = 0; i < obj_noutlets((t_object *)x); i++)
                {
                    int pos_x_outlet = 0;
                    if(obj_noutlets((t_object *)x) != 1)
                        pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->b_rect.width - 8));

                    if(mouse.x >= pos_x_outlet && mouse.x <= pos_x_outlet +7)
                    {
                        x->b_selected_outlet = i;
                        ebox_set_cursor(x, 5);
                        break;
                    }
                }
                if(x->b_selected_outlet == -1)
                {
                    x->b_selected_item = EITEM_BOTTOM;
                    ebox_set_cursor(x, 7);
                }
                ebox_invalidate_layer(x, s_cream_eboxio);
                ebox_redraw(x);
                return;
            }
            // RIGHT //
            else if(mouse.x > right - 3 && mouse.x <= right)
            {
                x->b_selected_item = EITEM_RIGHT;
                ebox_set_cursor(x, 9);
                return;
            }

            // BOX //
            ebox_set_cursor(x, 4);
            ebox_invalidate_layer(x, s_cream_eboxio);
            ebox_redraw(x);
        }
        else
        {
            sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
        }
    }
    else
    {
        if(is_for_box(x, modif))
        {
            if(c->c_widget.w_mousedrag && !(x->b_flags & EBOX_IGNORELOCKCLICK))
            {
                mouse.x = atom_getfloat(argv);
                mouse.y = atom_getfloat(argv+1);
                c->c_widget.w_mousedrag(x, x->b_obj.o_canvas, mouse, modif);
            }
        }
        else if(!x->b_isinsubcanvas)
        {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv+1);
            if(x->b_selected_item == EITEM_NONE)
            {
                sys_vgui("eobj_canvas_motion %s 0\n", x->b_canvas_id->s_name);
            }
            else if(!(x->b_flags & EBOX_GROWNO))
            {
                if(x->b_flags & EBOX_GROWLINK)
                {
                    if(x->b_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->b_rect_last.width + (mouse.y - x->b_rect_last.height));
                        atom_setfloat(av+1, mouse.y);
                    }
                    else if(x->b_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av+1, x->b_rect_last.height + (mouse.x - x->b_rect_last.width));
                    }
                    else if(x->b_selected_item == EITEM_CORNER)
                    {
                        if(mouse.y > mouse.x)
                        {
                            atom_setfloat(av, mouse.y);
                            atom_setfloat(av+1, mouse.y);
                        }
                        else
                        {
                            atom_setfloat(av, mouse.x);
                            atom_setfloat(av+1, mouse.x);
                        }
                    }
                }
                else if (x->b_flags & EBOX_GROWINDI)
                {
                    if(x->b_selected_item == EITEM_BOTTOM)
                    {
                        atom_setfloat(av, x->b_rect_last.width);
                        atom_setfloat(av+1, mouse.y);
                    }
                    else if(x->b_selected_item == EITEM_RIGHT)
                    {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av+1, x->b_rect_last.height);
                    }
                    else if(x->b_selected_item == EITEM_CORNER)
                    {
                        atom_setfloat(av, mouse.x);
                        atom_setfloat(av+1, mouse.y);
                    }
                }
                mess3((t_pd *)x, s_cream_size,  s_cream_size, (void *)2, (void *)av);
            }
        }
        else
        {
            sys_vgui("eobj_canvas_motion %s 1\n", x->b_canvas_id->s_name);
        }
    }
}

void ebox_mouse_down(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    t_pt mouse;
    long modif  = modifier_wrapper((long)atom_getfloat(argv+2));
    t_eclass *c = eobj_getclass(x);
    if(is_for_box(x, modif))
    {
        if(c->c_widget.w_mousedown && !(x->b_flags & EBOX_IGNORELOCKCLICK))
        {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv+1);
            c->c_widget.w_mousedown(x, x->b_obj.o_canvas, mouse, modif);
        }
    }
    else
    {
        if(x->b_selected_item == EITEM_NONE)
        {
            if(modif == EMOD_SHIFT)
            {
                sys_vgui("eobj_canvas_down %s 1\n", x->b_canvas_id->s_name);
            }
            else if(modif == EMOD_RIGHT)
            {
                sys_vgui("eobj_canvas_right %s\n", x->b_canvas_id->s_name);
            }
            else
            {
                sys_vgui("eobj_canvas_down %s 0\n", x->b_canvas_id->s_name);
            }
        }
        else
        {
            x->b_rect_last = x->b_rect;
        }
    }
    x->b_mouse_down = 1;
}

void ebox_mouse_up(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    t_pt mouse;
    long modif  = modifier_wrapper((long)atom_getfloat(argv+2));
    t_eclass *c = eobj_getclass(x);
    if(is_for_box(x, modif))
    {
        if(c->c_widget.w_mouseup && !(x->b_flags & EBOX_IGNORELOCKCLICK))
        {
            mouse.x = atom_getfloat(argv);
            mouse.y = atom_getfloat(argv+1);
            c->c_widget.w_mouseup(x, x->b_obj.o_canvas, mouse, modif);
        }
    }
    else
    {
        sys_vgui("eobj_canvas_up %s\n", x->b_canvas_id->s_name);
    }
    x->b_mouse_down = 0;
}

void ebox_mouse_dblclick(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    t_pt mouse;
    t_eclass *c = eobj_getclass(x);
    long modif  = modifier_wrapper((long)atom_getfloat(argv+2));
    if(is_for_box(x, modif) && c->c_widget.w_dblclick && !(x->b_flags & EBOX_IGNORELOCKCLICK))
    {
        mouse.x = atom_getfloat(argv);
        mouse.y = atom_getfloat(argv+1);
        c->c_widget.w_dblclick(x, x->b_obj.o_canvas, mouse);
    }
}

void ebox_mouse_wheel(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    t_pt mouse;
    float delta;
    long modif  = modifier_wrapper((long)atom_getfloat(argv+2));
    t_eclass *c = eobj_getclass(x);
    if(is_for_box(x, modif) && c->c_widget.w_mousewheel && !(x->b_flags & EBOX_IGNORELOCKCLICK))
    {
        mouse.x = atom_getfloat(argv);
        mouse.y = atom_getfloat(argv+1);
        delta   = atom_getfloat(argv+2);
        modif   = modifier_wrapper((long)atom_getfloat(argv+3));
        c->c_widget.w_mousewheel(x, x->b_obj.o_canvas, mouse, modif, delta, delta);
    }
}

void ebox_key(t_ebox* x, t_symbol* s, int argc, t_atom *argv)
{
    t_eclass *c = eobj_getclass(x);

    if(argc >= 2 && argv && atom_gettype(argv+1) == A_FLOAT)
    {
        if(!x->b_obj.o_canvas->gl_edit)
        {
            if(atom_getfloat(argv+1) == 65288)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_DEL, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_DEL, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65289)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_TAB, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_TAB, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65293)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_RETURN, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_RETURN, 0);
                }
            }
            else if(atom_getfloat(argv+1) == 65307)
            {
                if(c->c_widget.w_keyfilter)
                {
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ESC, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_ESC, 0);
                }
            }
            else
            {
                if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, (char)atom_getfloat(argv+1), 0);
                }
            }
        }
    }
}

void ebox_pos(t_ebox* x, float newx, float newy)
{
    x->b_rect.x = newx;
    x->b_rect.y = newy;
    x->b_obj.o_obj.te_xpix = (short)newx;
    x->b_obj.o_obj.te_ypix = (short)newy;

    ebox_move(x);
}

void ebox_vis(t_ebox* x, int vis)
{
    vis = (int)pd_clip(vis, 0, 1);
    if(x->b_visible != vis)
    {
        x->b_visible = (char)vis;
        if(x->b_visible && x->b_ready_to_draw && x->b_obj.o_canvas)
        {
            ebox_redraw(x);
        }
        else
        {
            ebox_erase(x);
        }
    }
}

t_pd_err ebox_set_receiveid(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    t_symbol* sname;
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        if(is_valid_symbol(x->b_receive_id))
        {
            sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }
        x->b_receive_id = atom_getsymbol(argv);
        if(is_valid_symbol(x->b_receive_id))
        {
            sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
            pd_bind(&x->b_obj.o_obj.ob_pd, sname);
        }
    }
    else
    {
        if(is_valid_symbol(x->b_receive_id))
        {
            sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }
        x->b_receive_id = s_cream_empty;
    }
    return 0;
}

t_pd_err ebox_set_sendid(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        x->b_send_id = atom_getsymbol(argv);
    }
    else
    {
        x->b_send_id = s_cream_empty;
    }

    return 0;
}

t_symbol* ebox_get_presetid(t_ebox* x)
{
    if(x->b_preset_id)
    {
        return x->b_preset_id;
    }
    else
    {
        return s_cream_empty;
    }
}

t_pd_err ebox_set_presetid(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        x->b_preset_id = atom_getsymbol(argv);
    }
    else
    {
        x->b_preset_id = s_cream_empty;
    }
    return 0;
}

t_pd_err ebox_size_set(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    float width, height;
    if(argc && argv)
    {
        if(x->b_flags & EBOX_GROWNO)
            return 0;
        else if(x->b_flags & EBOX_GROWLINK)
        {
            if(atom_gettype(argv) == A_FLOAT)
            {
                width  = (float)pd_clip_min(atom_getfloat(argv), 4);
                height = x->b_rect.height;
                x->b_rect.height += width - x->b_rect.width;
                if(x->b_rect.height < 4)
                {
                    x->b_rect.width += 4 - height;
                    x->b_rect.height = 4;
                }
                else
                {
                    x->b_rect.width  =  width;
                }
            }

        }
        else if (x->b_flags & EBOX_GROWINDI)
        {
            if(atom_gettype(argv) == A_FLOAT)
                x->b_rect.width = (float)pd_clip_min(atom_getfloat(argv), 4);
            if(atom_gettype(argv+1) == A_FLOAT)
                x->b_rect.height = (float)pd_clip_min(atom_getfloat(argv+1), 4);
        }
    }

    return 0;
}

void ebox_texteditor_keypress(t_ebox *x, t_symbol *s, float f)
{
    t_etexteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keypress)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keypress(x, editor, (int)f);
        }
    }
}

void ebox_texteditor_keyfilter(t_ebox *x, t_symbol *s, float f)
{
    t_etexteditor* editor;
    const t_eclass* c = eobj_getclass(x);
    if(c && c->c_widget.w_texteditor_keyfilter)
    {
        editor = etexteditor_getfromsymbol(s);
        if(editor)
        {
            c->c_widget.w_texteditor_keyfilter(x, editor, (int)f);
        }
    }
}

t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_eclass* c = eobj_getclass(x);
    if(s == s_cream_size)
    {
        if(c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->b_rect);
        ebox_invalidate_all(x);
        if(ebox_isdrawable(x))
        {
            sys_vgui("%s itemconfigure %s -width %d -height %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.), (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));
            canvas_fixlinesfor(x->b_obj.o_canvas, (t_text *)x);
        }
        ebox_redraw(x);
    }
    else if(s == s_pinned && ebox_isdrawable(x))
    {
        if(x->b_pinned)
        {
            sys_vgui((char *)"lower %s\n", x->b_drawing_id->s_name);
        }
        else
        {
            sys_vgui((char *)"raise %s\n", x->b_drawing_id->s_name);
        }
    }

    return 0;
}

void ebox_redraw(t_ebox *x)
{
    if(ebox_isdrawable(x) && x->b_have_window)
    {
        ebox_paint(x);
    }
    if(x->b_obj.o_camo_id->s_thing)
    {
        pd_symbol(x->b_obj.o_camo_id->s_thing, s_cream_repaint);
    }
}

void ebox_get_rect_for_view(t_ebox *x, t_rect *rect)
{
    rect->x = x->b_rect.x;
    rect->y = x->b_rect.y;
    rect->width = x->b_rect.width;
    rect->height = x->b_rect.height;
}

t_elayer* ebox_start_layer(t_ebox *x, t_symbol *name, float width, float height)
{
    int i, j;
    char text[MAXPDSTRING];
	t_elayer *temp, *graphic;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        graphic = &x->b_layers[i];
        if(graphic->e_name == name)
        {
            if(graphic->e_state == EGRAPHICS_INVALID)
            {
                graphic->e_owner        = (t_object *)x;

                egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
                graphic->e_line_width   = 1.f;
                graphic->e_color        = rgba_black;
                graphic->e_rect.x       = 0.f;
                graphic->e_rect.y       = 0.f;
                graphic->e_rect.height  = (float)pd_clip_min(height, 0.);
                graphic->e_rect.width   = (float)pd_clip_min(width, 0.);

                for(j = 0; j < graphic->e_number_objects; j++)
                {
                    if(graphic->e_objects[j].e_points)
                    {
                        free(graphic->e_objects[j].e_points);
                    }
                    graphic->e_objects[j].e_points = NULL;
                    graphic->e_objects[j].e_npoints = 0;
                    if(graphic->e_objects[j].e_type == E_GOBJ_TEXT && graphic->e_objects[j].e_text)
                    {
                        free(graphic->e_objects[j].e_text);
                    }
                    graphic->e_objects[j].e_text = NULL;
                }
                if(graphic->e_objects && graphic->e_number_objects)
                {
                    free(graphic->e_objects);
                    graphic->e_objects = NULL;
                }
                graphic->e_number_objects  = 0;
                graphic->e_new_objects.e_npoints = 0;
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
    {
        temp = (t_elayer*)calloc(1, sizeof(t_elayer));
    }
    else
    {
        temp = (t_elayer*)realloc(x->b_layers, (size_t)(x->b_number_of_layers + 1) * sizeof(t_elayer));
    }
    if(temp)
    {
        x->b_layers = temp;
        graphic = x->b_layers+x->b_number_of_layers;
        x->b_number_of_layers++;

        graphic->e_owner        = (t_object *)x;

        egraphics_matrix_init(&graphic->e_matrix, 1., 0., 0., 1., 0., 0.);
        graphic->e_line_width   = 1.f;
        graphic->e_color        = rgba_black;
        graphic->e_rect.x       = 0.f;
        graphic->e_rect.y       = 0.f;
        graphic->e_rect.height  = (float)pd_clip_min(height, 0.);
        graphic->e_rect.width   = (float)pd_clip_min(width, 0.);

        graphic->e_number_objects  = 0;
        graphic->e_objects      = NULL;
        graphic->e_new_objects.e_points = NULL;
        graphic->e_new_objects.e_npoints = 0;
        graphic->e_new_objects.e_rspace = 0;
    
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

t_pd_err ebox_end_layer(t_ebox *x, t_symbol *name)
{
    int i;
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

static void stringifyJustification(int justification, char* text)
{
    if(justification & ETEXT_TOP)
        sprintf(text, "-anchor n");
    else if(justification & ETEXT_BOTTOM)
        sprintf(text, "-anchor s");
    else
        sprintf(text, "-anchor ");
    
    if(justification & ETEXT_LEFT)
        strncat(text, "w", 1);
    else if(justification & ETEXT_RIGHT)
        strncat(text, "e", 1);
    else if(justification & ETEXT_CENTRED)
        strncat(text, "center", 6);
}

static t_pt recomputeRectangle(int justification, t_pt const* p1, t_pt const* p2)
{
    t_pt pt;
    if(justification & ETEXT_TOP)
        pt.y = p1->y;
    else if(justification & ETEXT_BOTTOM)
        pt.y = p1->y + p2->y;
    else
        pt.y = p1->y + p2->y * 0.5f;
    
    if(justification & ETEXT_LEFT)
        pt.x = p1->x;
    else if(justification & ETEXT_RIGHT)
        pt.x = p1->x + p2->x;
    else
        pt.x = p1->x + p2->x * 0.5f;
    
    return pt;
}

t_pd_err ebox_paint_layer(t_ebox *x, t_symbol *name, float x_p, float y_p)
{
    if(!x->b_drawing_id)
    {
        return 0;
    }
    int i, j;
    float bdsize, start, extent, radius;
    t_elayer* g = NULL;
    bdsize = x->b_boxparameters.d_borderthickness;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        if(x->b_layers[i].e_name == name)
        {
            g = &x->b_layers[i];
            if(g->e_state == EGRAPHICS_CLOSE)
            {
                sys_vgui("%s raise %s\n", x->b_drawing_id->s_name, g->e_id->s_name);
                return -1;
            }
            else if(g->e_state != EGRAPHICS_TODRAW)
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
            t_pt * pt;
            ////////////// PATH & LINE ///////////////////////////
            if(gobj->e_type == E_GOBJ_PATH && gobj->e_npoints > 3)
            {
                char header[256];
                char bottom[256];
                int mode = E_PATH_MOVE;
                if(gobj->e_filled)
                {
                    sprintf(header, "%s create polygon ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                }
                else
                {
                    sprintf(header, "%s create line ", x->b_drawing_id->s_name);
                    sprintf(bottom, "-smooth bezier -fill %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                }
                
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
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt+1)->x + x_p + bdsize), (int)((pt+1)->y + y_p + bdsize),
                                 (int)((pt+2)->x + x_p + bdsize), (int)((pt+2)->y + y_p + bdsize),
                                 (int)((pt+3)->x + x_p + bdsize), (int)((pt+3)->y + y_p + bdsize));
                        j += 3;
                        mode = E_PATH_CURVE;
                    }
                    else if(pt->x == E_PATH_LINE)
                    {
                        pt = gobj->e_points+j-1;
                        sys_vgui("%d %d %d %d %d %d ",
                                 (int)((pt-1)->x + x_p + bdsize), (int)((pt-1)->y + y_p + bdsize),
                                 (int)((pt+1)->x + x_p + bdsize), (int)((pt+1)->y + y_p + bdsize),
                                 (int)((pt+1)->x + x_p + bdsize), (int)((pt+1)->y + y_p + bdsize));
                        ++j;
                        mode = E_PATH_CURVE;
                    }
                    else
                    {
                        j++;
                    }
                }
                sys_vgui("%s", bottom);
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
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-fill %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
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
                    sys_vgui("-fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-outline %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
            }
            ////////////// ARC /////////////////
            else if (gobj->e_type == E_GOBJ_ARC)
            {
                start = (float)pd_angle(gobj->e_points[1].x - gobj->e_points[0].x,  gobj->e_points[1].y - gobj->e_points[0].y);
                
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
                    sys_vgui("-style pieslice -fill %s -width 0 -tags { %s %s }\n", rgba_to_hex(&gobj->e_color),  g->e_id->s_name, x->b_all_id->s_name);
                else
                    sys_vgui("-style arc -outline %s -width %f -tags { %s %s }\n", rgba_to_hex(&gobj->e_color), gobj->e_width, g->e_id->s_name, x->b_all_id->s_name);
                
            }
            ////////////// TEXT ////////////////
            else if(gobj->e_type == E_GOBJ_TEXT)
            {
                char text[256];
                stringifyJustification(gobj->e_justify, text);
                const t_pt np = recomputeRectangle(gobj->e_justify, gobj->e_points, gobj->e_points+1);
                sys_vgui("%s create text %d %d -text {%s} %s -font {{%s} %d %s %s} -fill %s -width %d -tags { %s %s }\n",
                         x->b_drawing_id->s_name,
                         (int)(np.x + x_p + bdsize),
                         (int)(np.y + y_p + bdsize),
                         gobj->e_text,
                         text,
                         gobj->e_font.family->s_name, (int)gobj->e_font.size, gobj->e_font.weight->s_name, gobj->e_font.slant->s_name,
                         rgba_to_hex(&gobj->e_color),
                         (int)(gobj->e_points[1].x) * (int)gobj->e_wrap,
                         g->e_id->s_name,
                         x->b_all_id->s_name);
            }
            else
            {
                return -1;
            }
        }
        g->e_state = EGRAPHICS_CLOSE;
    }
    else
    {
        return -1;
    }
    
    return 0;
}

static void ebox_draw_border(t_ebox* x)
{
    float bdsize, bdcorner;
    t_elayer* g = NULL;
    bdcorner = (float)pd_clip_max(x->b_boxparameters.d_cornersize, x->b_boxparameters.d_borderthickness - 1);
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, s_cream_eboxbd, x->b_rect.width, x->b_rect.height);

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

        ebox_end_layer(x, s_cream_eboxbd);
    }
    ebox_paint_layer(x, s_cream_eboxbd, -bdsize, -bdsize);
}

static void ebox_draw_iolets(t_ebox* x)
{
    int i;
    float bdsize;
    t_elayer* g = NULL;
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, s_cream_eboxio, x->b_rect.width, x->b_rect.height);

    if(g)
    {
        egraphics_set_line_width(g, 1);
        for(i = 0; i < obj_ninlets((t_object *)x); i++)
        {
            int pos_x_inlet = 0;
            if(obj_ninlets((t_object *)x) != 1)
                pos_x_inlet = (int)(i / (float)(obj_ninlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_inlet, 0, 7, 1);
            egraphics_set_color_rgba(g, &rgba_black);
            if(!x->b_isinsubcanvas)
                egraphics_stroke(g);
        }

        for(i = 0; i < obj_noutlets((t_object *)x); i++)
        {
            int pos_x_outlet = 0;
            if(obj_noutlets((t_object *)x) != 1)
                pos_x_outlet = (int)(i / (float)(obj_noutlets((t_object *)x) - 1) * (x->b_rect.width - 8));
            egraphics_rectangle(g, pos_x_outlet, x->b_rect.height - 2 + bdsize * 2, 7, 1);
            egraphics_set_color_rgba(g, &rgba_black);
            if(!x->b_isinsubcanvas)
                egraphics_stroke(g);
        }
        ebox_end_layer(x, s_cream_eboxio);
    }
    ebox_paint_layer(x, s_cream_eboxio, 0, -bdsize);
}

static void ebox_invalidate_all(t_ebox *x)
{
    int i;
    for(i = 0; i < x->b_number_of_layers; i++)
    {
        x->b_layers[i].e_state = EGRAPHICS_INVALID;
    }
}

static void ebox_update(t_ebox *x)
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

static void ebox_erase(t_ebox* x)
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

static void ebox_select(t_ebox* x)
{
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        if(x->b_selected_box == EITEM_OBJ)
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(&rgba_blue));
        }
        else
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(&x->b_boxparameters.d_bordercolor));
        }
    }
}

static void ebox_move(t_ebox* x)
{
    if(glist_isvisible(x->b_obj.o_canvas))
    {
        sys_vgui("%s coords %s %d %d\n", x->b_canvas_id->s_name, x->b_window_id->s_name, (int)(x->b_rect.x - x->b_boxparameters.d_borderthickness), (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness));
    }
    canvas_fixlinesfor(glist_getcanvas(x->b_obj.o_canvas), (t_text*)x);
}








void ebox_set_parameter_attribute(t_ebox *x, t_symbol *s, int argc, t_atom* argv)
{
    t_eparam* param;
    if(argc > 2 && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_SYMBOL)
    {
        param = eparameter_getbyindex(x, (int)atom_getfloat(argv));
        if(param)
        {
            if(atom_getsymbol(argv+1) == gensym("name") && atom_gettype(argv+2) == A_SYMBOL)
            {
                eparameter_setname(param, atom_getsymbol(argv+2));
            }
            else if(atom_getsymbol(argv+1) == gensym("label") && atom_gettype(argv+2) == A_SYMBOL)
            {
                eparameter_setlabel(param, atom_getsymbol(argv+2));
            }
        }
    }
}

static void ebox_parameter_notify(t_eparam* param, t_symbol* action)
{
    t_atom av[3];
    if(is_valid_symbol(param->p_owner->b_obj.o_camo_id) && param->p_owner->b_obj.o_camo_id->s_thing)
    {
        atom_setsym(av, param->p_bind);
        atom_setsym(av+1, action);
        atom_setfloat(av+2, (float)param->p_index);
        pd_typedmess(param->p_owner->b_obj.o_camo_id->s_thing, s_cream_parameter, 3, av);
    }
}

static t_class* eparameter_setup()
{
    t_class* eparameter_class = NULL;
    t_symbol* eparameter1572_sym = gensym("eparameter1572");
    if(!eparameter1572_sym->s_thing)
    {
        eparameter_class = class_new(gensym("eparameter"), NULL, (t_method)NULL, sizeof(t_etexteditor), CLASS_PD, A_GIMME, 0);
        eparameter1572_sym->s_thing = (t_class **)eparameter_class;
        return eparameter_class;
    }
    else
    {
        return (t_class *)eparameter1572_sym->s_thing;
    }
}

t_eparam* ebox_parameter_create(t_ebox *x, int index)
{
    int i;
    char text[MAXPDSTRING];
    t_eparam **temp  = NULL, *param = NULL;
    t_class* c = eparameter_setup();
    if(c)
    {
        index--;
        if(index < x->b_nparams && x->b_params[index])
        {
            return x->b_params[index];
        }
        else
        {
            param = (t_eparam *)pd_new(c);
            if(param)
            {
                sprintf(text, "param%ld", (unsigned long)(param));
                param->p_bind       = gensym(text);
                param->p_name       = s_cream_empty;
                param->p_label      = s_cream_empty;
                param->p_owner      = x;
                param->p_index      = index+1;
                param->p_value      = 0.f;
                param->p_min        = 0.f;
                param->p_max        = 1.f;
                param->p_nstep      = (int)(1.f / FLT_EPSILON) + 1;
                param->p_getter     = (t_param_getter)NULL;
                param->p_setter     = (t_param_setter)NULL;
                param->p_getter_t   = (t_param_getter_t)NULL;
                param->p_setter_t   = (t_param_setter_t)NULL;
                param->p_auto       = 1;
                param->p_meta       = 0;
                param->p_enable     = 1;
                param->p_flags      = 0;
                pd_bind((t_pd *)param, param->p_bind);
                if(x->b_params && index < x->b_nparams)
                {
                    x->b_params[index] = param;
                    ebox_parameter_notify(param, s_cream_create);
                    return param;
                }
                else if(x->b_params && index >= x->b_nparams)
                {
                    temp = (t_eparam **)realloc(x->b_params, sizeof(t_eparam *) * (size_t)(index + 1));
                    if(temp)
                    {
                        x->b_params = temp;
                        for(i = (int)x->b_nparams; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else if(!x->b_params || !x->b_nparams)
                {
                    x->b_params = (t_eparam **)malloc(sizeof(t_eparam *) * (size_t)(index + 1));
                    if(x->b_params)
                    {
                        for(i = 0; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        ebox_parameter_notify(param, s_cream_create);
                        return param;
                    }
                    else
                    {
                        x->b_nparams = 0;
                        return NULL;
                    }
                }
            }
            
        }
    }
    return NULL;
}

void ebox_parameter_destroy(t_ebox* x, int index)
{
    int i = 0;
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_destroy);
            pd_unbind((t_pd *)x->b_params[index], x->b_params[index]->p_bind);
            free(x->b_params[index]);
            x->b_params[index] = NULL;
            for(i = index; i < x->b_nparams - 1; i++)
            {
                x->b_params[i] = x->b_params[i+1];
            }
        }
    }
}

float ebox_parameter_getvalue(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_getter)
            {
                return x->b_params[index]->p_getter(x->b_params[index]->p_owner, x->b_params[index]);
            }
            return x->b_params[index]->p_value;
        }
    }
    return 0;
}

void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_setter)
            {
                x->b_params[index]->p_setter(x->b_params[index]->p_owner, x->b_params[index], value);
            }
            else
            {
                if(x->b_params[index]->p_min < x->b_params[index]->p_max)
                {
                    x->b_params[index]->p_value = pd_clip(value, x->b_params[index]->p_min, x->b_params[index]->p_max);
                }
                else
                {
                    x->b_params[index]->p_value  = pd_clip(value, x->b_params[index]->p_max, x->b_params[index]->p_min);
                }
            }
            if(notify)
            {
                ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
            }
        }
    }
}

float ebox_parameter_getvalue_normalized(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_max == x->b_params[index]->p_min)
            {
                return 1.;
            }
            else if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_min) / (x->b_params[index]->p_max - x->b_params[index]->p_min);
            }
            else
            {
                return (ebox_parameter_getvalue(x, index+1) - x->b_params[index]->p_max) / (x->b_params[index]->p_min - x->b_params[index]->p_max);
            }
        }
    }
    return 0.f;
}

void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_max - x->b_params[index]->p_min) + x->b_params[index]->p_min), notify);
            }
            else
            {
                ebox_parameter_setvalue(x, index+1, (value * (x->b_params[index]->p_min - x->b_params[index]->p_max) + x->b_params[index]->p_max), notify);
            }
        }
    }
}

float ebox_parameter_getmin(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_min;
        }
    }
    return 1.f;
}

float ebox_parameter_getmax(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_max;
        }
    }
    return 1.f;
}

int ebox_parameter_getnstep(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_nstep;
        }
    }
    return 1;
}

t_symbol* ebox_parameter_getbind(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_bind;
        }
    }
    return NULL;
}

char ebox_parameter_isinverted(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return (char)(x->b_params[index]->p_max < x->b_params[index]->p_min);
        }
    }
    return 0;
}

void ebox_parameter_notify_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_value_changed);
        }
    }
}

void ebox_parameter_begin_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_beginchanges);
        }
    }
}

void ebox_parameter_end_changes(t_ebox *x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            ebox_parameter_notify(x->b_params[index], s_cream_endchanges);
        }
    }
}

void ebox_parameter_setname(t_ebox* x, int index, t_symbol* name)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_name = get_valid_symbol(name);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setlabel(t_ebox* x, int index, t_symbol* label)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_label = get_valid_symbol(label);
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setminmax(t_ebox* x, int index, float min, float max)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_min = min;
            x->b_params[index]->p_max = max;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
            ebox_parameter_setvalue(x, index+1, x->b_params[index]->p_value, 1);
        }
    }
}

void ebox_parameter_setnstep(t_ebox* x, int index, int nstep)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_nstep = nstep > 1 ? (int)nstep : 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setflags(t_ebox* x, int index, long flags)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_flags = flags;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_enable(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 1;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_disable(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_enable = 0;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setsettergetter(t_ebox* x, int index, t_param_setter setter, t_param_getter getter)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter = setter;
            x->b_params[index]->p_getter = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}

void ebox_parameter_setsettergetter_text(t_ebox* x, int index, t_param_setter_t setter, t_param_getter_t getter)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            x->b_params[index]->p_setter_t = setter;
            x->b_params[index]->p_getter_t = getter;
            ebox_parameter_notify(x->b_params[index], s_cream_attr_modified);
        }
    }
}



float eparameter_getvalue(t_eparam* param)
{
    if(param->p_getter)
    {
        return param->p_getter(param->p_owner, param);
    }
    return param->p_value;
}

float eparameter_getvalue_normalized(t_eparam* param)
{
    if(param->p_max == param->p_min)
    {
        return 1.;
    }
    else if(param->p_min < param->p_max)
    {
        return (eparameter_getvalue(param) - param->p_min) / (param->p_max - param->p_min);
    }
    else
    {
        return (eparameter_getvalue(param) - param->p_max) / (param->p_min - param->p_max);
    }
}

void eparameter_getvalue_text(t_eparam* param, char* text)
{
    if(param->p_getter_t)
    {
        param->p_getter_t(param->p_owner, param->p_index, text);
    }
    else
    {
        sprintf(text, "%g", eparameter_getvalue(param));
    }
}

static void eparameter_notify_owner(t_eparam* param, t_symbol* message)
{
    t_eclass* c = eobj_getclass(param->p_owner);
    if(c->c_widget.w_notify)
    {
        c->c_widget.w_notify(param->p_owner, param->p_bind, message, NULL, NULL);
    }
}

static float eparameter_compute_value(float value, float min, float max, float nstep)
{
    const float step = (max - min) / nstep;
    const float rval = floorf((value - min) / step + 0.5);
    return pd_clip_max(step * rval + min, max);
}


void eparameter_setvalue(t_eparam* param, float value)
{
    if(param->p_setter)
    {
        param->p_setter(param->p_owner, param, value);
    }
    else
    {
        if(param->p_min < param->p_max)
        {
            param->p_value = eparameter_compute_value(value, param->p_min, param->p_max, (float)param->p_nstep);
        }
        else
        {
            param->p_value = eparameter_compute_value(value, param->p_max, param->p_min, (float)param->p_nstep);
        }
        eparameter_notify_owner(param, s_cream_value_changed);
    }
}

void eparameter_setvalue_normalized(t_eparam* param, float value)
{
    if(param->p_min < param->p_max)
    {
        eparameter_setvalue(param, (value * (param->p_max - param->p_min) + param->p_min));
    }
    else
    {
        eparameter_setvalue(param, (value * (param->p_min - param->p_max) + param->p_max));
    }
}

void eparameter_setvalue_text(t_eparam* param, char const* text)
{
    if(param->p_setter_t)
    {
        param->p_setter_t(param->p_owner, param->p_index, text);
    }
    else if(isdigit(text[0]))
    {
        eparameter_setvalue(param, atof(text));
    }
}

void eparameter_setname(t_eparam* param, t_symbol* name)
{
    if(!(param->p_flags & EPARAM_STATIC_NAME))
    {
        param->p_name = get_valid_symbol(name);
        eparameter_notify_owner(param, s_cream_attr_modified);
        canvas_dirty(eobj_getcanvas(eobj_getcanvas(param->p_owner)), 1);
    }
}

void eparameter_setlabel(t_eparam* param, t_symbol* label)
{
    if(!(param->p_flags & EPARAM_STATIC_LABEL))
    {
        param->p_label = get_valid_symbol(label);
        eparameter_notify_owner(param, s_cream_attr_modified);
        canvas_dirty(eobj_getcanvas(eobj_getcanvas(param->p_owner)), 1);
    }
}

void eparameter_setindex(t_eparam* param, int index)
{
    param->p_index = index;
    eparameter_notify_owner(param, s_cream_attr_modified);
}

t_eparam* eparameter_getfromsymbol(t_symbol* name)
{
    t_class* c = eparameter_setup();
    if(c)
    {
        return (t_eparam *)pd_findbyclass(name, c);
    }
    return NULL;
}

t_eparam* eparameter_getbyindex(t_ebox* x, int index)
{
    index--;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index];
        }
    }
    return NULL;
}








