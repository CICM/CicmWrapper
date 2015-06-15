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
#include "egraphics.h"
#include "eobj.h"

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
static void ebox_attrprocess_default(void *x);

void ebox_new(t_ebox *x, long flags)
{
    x->b_flags = flags;
    x->b_ready_to_draw      = 0;
    x->b_have_window        = 0;
    x->b_number_of_layers   = 0;
    x->b_layers             = NULL;
    x->b_window_id          = NULL;
    x->b_receive_id         = s_null;
    x->b_send_id            = s_null;
    x->b_objpreset_id       = s_null;
    x->b_visible            = 1;
    eobj_getclass(x)->c_widget.w_dosave = (method)ebox_dosave;
    ebox_attrprocess_default(x);
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
    eobj_free(x);
    if(x->b_receive_id && x->b_receive_id != s_null)
    {
        pd_unbind((t_pd *)x, x->b_receive_id);
    }
    gfxstub_deleteforkey(x);
    if(eobj_isdsp(x))
    {
        eobj_dspfree(x);
    }
}

t_symbol* ebox_getfontname(t_ebox* x)
{
    return x->b_font.c_family;
}

t_symbol* ebox_getfontslant(t_ebox* x)
{
    return x->b_font.c_slant;
}

t_symbol* ebox_getfontweight(t_ebox* x)
{
    return x->b_font.c_weight;
}

float ebox_getfontsize(t_ebox* x)
{
    return x->b_font.c_size;
}

t_pd* ebox_getsender(t_ebox* x)
{
    t_symbol* sname;
    if(x->b_send_id && x->b_send_id != s_null)
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
    cursor = (int)pd_clip_minmax(cursor, 0, 12);
    sys_vgui("%s configure -cursor %s\n", x->b_drawing_id->s_name, my_cursorlist[cursor]);
}

void ebox_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    int     defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);
    
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
}

void ebox_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    int i;
    char attr_name[MAXPDSTRING];
    
    int defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

static void ebox_attrprocess_default(void *x)
{
    int i, j, k;
    long defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->defvals)
        {
            defc = c->c_attr[i]->size;
            defv = (t_atom *)calloc((unsigned long)defc, sizeof(t_atom));
            if(defc && defv)
            {
                char check = 0;
                char* str_start = c->c_attr[i]->defvals->s_name;
                for(j = 0; j < defc; j++)
                {
                    for(k = 0; k < (int)(strlen(str_start)); k++)
                    {
                        if(isalpha(str_start[k]))
                            check = 1;
                    }
                    if(check || strpbrk(str_start, "<>()'\""))
                    {
                        atom_setsym(defv+j, gensym(str_start));
                    }
                    else
                    {
                        float val = (float)strtod(str_start, &str_start);
                        atom_setfloat(defv+j, val);
                    }
                }
                eobj_attr_setvalueof(x, c->c_attr[i]->name, defc, defv);
            }
            if(defv)
            {
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

//! Widget
void ebox_wvis(t_gobj *z, t_glist *glist, int vis)
{
    t_ebox* x   = (t_ebox *)z;
    t_eclass* c = eobj_getclass(x);
    if(vis)
    {
        if(eobj_isbox(x))
        {
            if(x->b_ready_to_draw && x->b_visible)
            {
                ebox_create_window(x, glist);
                ebox_invalidate_all(x);
                // No redraw for the 1st paint
                
                if(x->b_obj.o_canvas && x->b_ready_to_draw && eobj_isbox(x)) // do not call ebox_isdrawable !!
                {
                    ebox_invalidate_layer(x, s_eboxbd);
                    ebox_invalidate_layer(x, s_eboxio);
                    
                    ebox_update(x);
                    if(c->c_widget.w_paint)
                        c->c_widget.w_paint(x, (t_object *)x->b_obj.o_canvas);
                    ebox_draw_border(x);
                    ebox_draw_iolets(x);
                }
            }
        }
    }
    else
    {
        ebox_erase(x);
    }
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

//! Widget
void ebox_wdisplace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    t_ebox *x;
#ifdef _WINDOWS
    t_gotfn m;
    
#endif
    x = (t_ebox *)z;
    
    x->b_rect.x += dx;
    x->b_rect.y += dy;
    x->b_obj.o_obj.te_xpix += dx;
    x->b_obj.o_obj.te_ypix += dy;
    
    ebox_move(x);
    
#ifdef _WINDOWS
    if(!x->b_selected_box)
    {
        m = getfn((t_pd *)x->b_obj.o_canvas, gensym("setbounds"));
        m(x->b_obj.o_canvas, glist->gl_screenx1, glist->gl_screeny1, glist->gl_screenx2, glist->gl_screeny2);
    }
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
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness));
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
             (int)(x->b_rect.x  - x->b_boxparameters.d_borderthickness),
             (int)(x->b_rect.y - x->b_boxparameters.d_borderthickness),
             x->b_drawing_id->s_name,
             x->b_window_id->s_name,
             (int)(x->b_rect.width + x->b_boxparameters.d_borderthickness * 2.),
             (int)(x->b_rect.height + x->b_boxparameters.d_borderthickness * 2.));
    
    sys_vgui("focus -force .x%lx.c\n", (long unsigned int) glist);
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
    
    if(mod >= 131080)
    {
        mod -= 131080;
        mod += EMOD_ALT;
    }
    else
        mod -= 8;
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
    else if(x->b_obj.o_canvas->gl_edit)
    {
        sys_vgui("focus %s\n", x->b_canvas_id->s_name);
    }
}

void ebox_mouse_leave(t_ebox* x)
{
    t_eclass *c = eobj_getclass(x);
    
    if(!x->b_obj.o_canvas->gl_edit && !x->b_mouse_down)
    {
        sys_vgui("focus %s\n", x->b_canvas_id->s_name);
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
                ebox_invalidate_layer(x, s_eboxio);
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
                ebox_invalidate_layer(x, s_eboxio);
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
            ebox_invalidate_layer(x, s_eboxio);
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
                mess3((t_pd *)x, s_size,  s_size, (void *)2, (void *)av);
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
                    c->c_widget.w_keyfilter(x, NULL, EKEY_ENTER, 0);
                }
                else if(c->c_widget.w_key)
                {
                    c->c_widget.w_key(x, NULL, EKEY_ENTER, 0);
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

void ebox_dosave(t_ebox* x, t_binbuf *b)
{
    int         i = 0, state = 0, argc = 0;
    char        attr_name[MAXPDSTRING];
    t_atom*     argv    = NULL;
    t_eclass *c = eobj_getclass(x);
    if(c && b)
    {
        state = canvas_suspend_dsp();
        binbuf_addv(b, "ssiis", &s__X, s_obj, (int)x->b_obj.o_obj.te_xpix, (int)x->b_obj.o_obj.te_ypix, eobj_getclassname(x));
        for(i = 0; i < c->c_nattr; i++)
        {
            if(c->c_attr[i] && c->c_attr[i]->save && c->c_attr[i]->name)
            {
                eobj_attr_getvalueof(x, c->c_attr[i]->name, &argc, &argv);
                if(argc && argv)
                {
                    snprintf(attr_name, MAXPDSTRING, "@%s", c->c_attr[i]->name->s_name);
                    binbuf_append_attribute(b, gensym(attr_name), argc, argv);
                    argc = 0;
                    free(argv);
                    argv = NULL;
                }
            }
        }
        
        if(c->c_widget.w_save != NULL)
            c->c_widget.w_save(x, b);
        
        binbuf_addv(b, ";");
        canvas_resume_dsp(state);
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
    vis = (int)pd_clip_minmax(vis, 0, 1);
    if(x->b_visible != vis)
    {
        x->b_visible = (char)vis;
        if(x->b_visible && x->b_ready_to_draw && x->b_obj.o_canvas)
        {
            canvas_redraw(x->b_obj.o_canvas);
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
    if(argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null)
    {
        if(x->b_receive_id != s_null)
        {
            sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }
        x->b_receive_id = atom_getsymbol(argv);
        sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
        pd_bind(&x->b_obj.o_obj.ob_pd, sname);
    }
    else
    {
        if(x->b_receive_id != s_null)
        {
            sname = canvas_realizedollar(eobj_getcanvas(x), x->b_receive_id);
            pd_unbind(&x->b_obj.o_obj.ob_pd, sname);
        }
        x->b_receive_id = s_null;
    }
    return 0;
}

t_pd_err ebox_set_sendid(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null)
    {
        x->b_send_id = atom_getsymbol(argv);
    }
    else
    {
        x->b_send_id = s_null;
    }
    
    return 0;
}

t_symbol* ebox_get_presetid(t_ebox* x)
{
    if(x->b_objpreset_id != s_null)
    {
        return x->b_objpreset_id;
    }
    else
    {
        return s_null;
    }
}

t_pd_err ebox_set_presetid(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL && atom_getsymbol(argv) != s_null)
    {
        x->b_objpreset_id = atom_getsymbol(argv);
    }
    else
    {
        x->b_objpreset_id = s_null;
    }
    return 0;
}

t_pd_err ebox_set_font(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        if(atom_getsymbol(argv) == s_null)
            x->b_font.c_family = gensym("Helvetica");
        else
            x->b_font.c_family = atom_getsymbol(argv);
    }
    else
        x->b_font.c_family = gensym("Helvetica");
    
    x->b_font.c_family = gensym(strtok(x->b_font.c_family->s_name," ',.-"));
    x->b_font.c_family->s_name[0] = (char)toupper(x->b_font.c_family->s_name[0]);
    return 0;
}

t_pd_err ebox_set_fontweight(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        if(atom_getsymbol(argv) == gensym("bold") || atom_getsymbol(argv) == gensym("Bold"))
            x->b_font.c_weight = gensym("bold");
        else
            x->b_font.c_weight = gensym("normal");
    }
    else
        x->b_font.c_weight = gensym("normal");
    
    return 0;
}

t_pd_err ebox_set_fontslant(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        if(atom_getsymbol(argv) == gensym("italic") || atom_getsymbol(argv) == gensym("Italic"))
            x->b_font.c_slant = gensym("italic");
        else
            x->b_font.c_slant = gensym("roman");
    }
    else
        x->b_font.c_slant = gensym("roman");
    
    return 0;
}

t_pd_err ebox_set_fontsize(t_ebox *x, t_object *attr, int argc, t_atom *argv)
{
    if(argc && argv && atom_gettype(argv) == A_FLOAT)
    {
        x->b_font.c_sizereal = (long)pd_clip_min(atom_getfloat(argv), 4);
    }
    else
        x->b_font.c_sizereal = 11;
#ifdef __APPLE__
    x->b_font.c_size = x->b_font.c_sizereal;
#elif  _WINDOWS
    x->b_font.c_size = x->b_font.c_sizereal - 1;
#else
    x->b_font.c_size = x->b_font.c_sizereal - 3;
#endif
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

t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    t_eclass* c = eobj_getclass(x);
    if(s == s_size)
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
    
    return 0;
}

void ebox_attrprint(t_ebox* x)
{
    int i;
    t_eclass* c = eobj_getclass(x);
    post("%s attributes :", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {
        
        post("Label : \"%s\" Name : \"%s\" Type : \"%s\" Size : \"%i\"", c->c_attr[i]->label->s_name, c->c_attr[i]->name->s_name, c->c_attr[i]->type->s_name, c->c_attr[i]->size);
    }
}

void ebox_properties(t_ebox *x, t_glist *glist)
{
    int i, j;
    t_atom *argv = NULL;
    int    argc = 0;
    t_eclass* c = eobj_getclass(x);
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];
    
    sprintf(buffer, "pdtk_%s_dialog %%s", c->c_class.c_name->s_name);
    
    for(i = 0; i < c->c_nattr; i++)
    {
        if(!c->c_attr[i]->invisible)
        {
            eobj_attr_getvalueof((t_object *)x, c->c_attr[i]->name, &argc, &argv);
            strcat(buffer, " ");
            strcat(buffer, "\"");
            if(argc && argv)
            {
#ifndef _WINDOWS
                for(j = 0; j < argc-1; j++)
#else
                    for(j = 0; j < argc; j++)
#endif
                    {
                        atom_string(argv+j, temp, MAXPDSTRING);
                        if(c->c_attr[i]->type == &s_symbol && strchr(temp, ' '))
                        {
                            
                            strcat(buffer, "'");
                            strcat(buffer, temp);
                            strcat(buffer, "'");
                        }
                        else
                        {
                            strcat(buffer, temp);
                        }
                        strcat(buffer, " ");
                    }
#ifndef _WINDOWS
                atom_string(argv+j, temp, MAXPDSTRING);
                if(c->c_attr[i]->type == &s_symbol && strchr(temp, ' '))
                {
                    
                    strcat(buffer, "'");
                    strcat(buffer, temp);
                    strcat(buffer, "'");
                }
                else
                {
                    strcat(buffer, temp);
                }
                free(argv);
                argc = 0;
#endif
            }
            strcat(buffer, "\"");
        }
    }
    strcat(buffer, "\n");
    
    gfxstub_new((t_pd *)x, x, buffer);
}

void ebox_dialog(t_ebox *x, t_symbol *s, int argc, t_atom *argv)
{
    int i;
    int attrindex;
    t_eclass* c = eobj_getclass(x);
    t_atom *av;
    int ac;
    char buffer[MAXPDSTRING];
    char temp[MAXPDSTRING];
    t_rgb color;
    
    if(argc > 2 && argv)
    {
        ebox_attrprocess_viatoms(x, argc, argv);
        if(atom_gettype(argv) == A_SYMBOL && atom_gettype(argv+1) ==A_FLOAT)
        {
            attrindex = (int)atom_getfloat(argv+1) - 1;
            if(attrindex >= 0 && attrindex < c->c_nattr)
            {
                eobj_attr_getvalueof((t_object *)x, c->c_attr[attrindex]->name, &ac, &av);
                if(ac && av)
                {
                    if(c->c_attr[attrindex]->style == gensym("checkbutton"))
                    {
                        if(atom_getfloat(av) == 0)
                            sys_vgui("%s.sele%i.selec deselect \n", atom_getsymbol(argv)->s_name, attrindex+1);
                        else
                            sys_vgui("%s.sele%i.selec select \n", atom_getsymbol(argv)->s_name, attrindex+1);
                    }
                    else if(c->c_attr[attrindex]->style == gensym("color"))
                    {
                        color.red = atom_getfloat(av);
                        color.green = atom_getfloat(av+1);
                        color.blue = atom_getfloat(av+2);
                        sys_vgui("%s.sele%i.selec configure -readonlybackground %s \n", atom_getsymbol(argv)->s_name, attrindex+1, rgb_to_hex(color));
                    }
                    else if(c->c_attr[attrindex]->style == gensym("menu"))
                    {
                        atom_string(av, buffer, MAXPDSTRING);
                        for(i = 1; i < ac; i++)
                        {
                            atom_string(av+i, temp, MAXPDSTRING);
                            strcat(buffer, " ");
                            strcat(buffer, temp);
                        }
                        sys_vgui("%s.sele%i.selec delete 0 end \n", atom_getsymbol(argv)->s_name, attrindex+1);
                        sys_vgui("%s.sele%i.selec insert 0 \"%s\" \n", atom_getsymbol(argv)->s_name, attrindex+1, buffer);
                    }
                    else
                    {
                        atom_string(av, buffer, MAXPDSTRING);
                        for(i = 1; i < ac; i++)
                        {
                            atom_string(av+i, temp, MAXPDSTRING);
                            strcat(buffer, " ");
                            strcat(buffer, temp);
                        }
                        sys_vgui("%s.sele%i.selec delete 0 end \n", atom_getsymbol(argv)->s_name, attrindex+1);
                        sys_vgui("%s.sele%i.selec insert 0 \"%s\" \n", atom_getsymbol(argv)->s_name, attrindex+1, buffer);
                        
                    }
                }
                
            }
        }
    }
}

void ebox_redraw(t_ebox *x)
{
    t_eclass* c = eobj_getclass(x);
    if(ebox_isdrawable(x) && x->b_have_window)
    {
        ebox_invalidate_layer(x, s_eboxbd);
        ebox_invalidate_layer(x, s_eboxio);
        
        ebox_update(x);
        if(c->c_widget.w_paint)
            c->c_widget.w_paint(x, (t_object *)x->b_obj.o_canvas);
        ebox_draw_border(x);
        ebox_draw_iolets(x);
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
                }
                if(graphic->e_objects)
                {
                    free(graphic->e_objects);
                }
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
        x->b_layers = (t_elayer*)realloc(x->b_layers, (size_t)(x->b_number_of_layers + 1) * sizeof(t_elayer));
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
        graphic->e_rect.height  = (float)pd_clip_min(height, 0.);
        graphic->e_rect.width   = (float)pd_clip_min(width, 0.);
        
        
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

static void ebox_draw_border(t_ebox* x)
{
    float bdsize, bdcorner;
    t_elayer* g = NULL;
    bdcorner = (float)pd_clip_max(x->b_boxparameters.d_cornersize, x->b_boxparameters.d_borderthickness - 1);
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, s_eboxbd, x->b_rect.width, x->b_rect.height);
    
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
        
        ebox_end_layer(x, s_eboxbd);
    }
    ebox_paint_layer(x, s_eboxbd, -bdsize, -bdsize);
}

static void ebox_draw_iolets(t_ebox* x)
{
    int i;
    float bdsize;
    t_elayer* g = NULL;
    bdsize = x->b_boxparameters.d_borderthickness;
    g = ebox_start_layer(x, s_eboxio, x->b_rect.width, x->b_rect.height);
    
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
        ebox_end_layer(x, s_eboxio);
    }
    ebox_paint_layer(x, s_eboxio, 0, -bdsize);
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
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(rgba_blue));
        }
        else
        {
            sys_vgui("%s itemconfigure eboxbd%ld -fill %s\n", x->b_drawing_id->s_name, x,rgba_to_hex(x->b_boxparameters.d_bordercolor));
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








