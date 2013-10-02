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

#include "ewidget.h"

void ewidget_init(t_eclass* c)
{
    c->c_widget.w_getrectfn         = ewidget_getrect;
    c->c_widget.w_visfn             = ewidget_vis;
    c->c_widget.w_displacefn        = ewidget_displace;
    c->c_widget.w_selectfn          = ewidget_select;
    c->c_widget.w_activatefn        = NULL;
    c->c_widget.w_deletefn          = ewidget_delete;
    c->c_widget.w_clickfn           = ewidget_mousemove;
    
    c->c_widget.w_paint             = (method)ewidget_paint_default;
    c->c_widget.w_mousemove         = (method)ewidget_mousemove_default;
    c->c_widget.w_mousedown         = (method)ewidget_mousedown_default;
    c->c_widget.w_mousedrag         = (method)ewidget_mousedrag_default;
    c->c_widget.w_mouseup           = (method)ewidget_mouseup_default;
    c->c_widget.w_dblclick          = NULL;
    c->c_widget.w_dblclicklong      = NULL;
    c->c_widget.w_key               = (method)ewidget_key_default;
    c->c_widget.w_keyfilter         = (method)ewidget_keyfilter_default;
    c->c_widget.w_deserted          = NULL;
    c->c_widget.w_getdrawparameters = (method)ewidget_getdrawparams_default;
    c->c_widget.w_notify            = (t_err_method)ewidget_notify_default;
    c->c_widget.w_save              = (method)ewidget_save_default;
    c->c_widget.w_popup             = (method)ewidget_popup_default;
    c->c_widget.w_dsp               = (method)ewidget_dsp_default;
    c->c_widget.w_oksize            = (method)ewidget_oksize_default;
}

void ewidget_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_ebox *x = (t_ebox *)z;
    *xp1 = text_xpix(&x->e_obj, glist);
    *yp1 = text_ypix(&x->e_obj, glist);
    *xp2 = *xp1 + x->e_rect.width;
    *yp2 = *yp1 + x->e_rect.height;
}

void ewidget_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_ebox* x   = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    if(vis && x->e_ready_to_draw)
    {
        ebox_invalidate_all(x, glist);
        ebox_update(x, glist);
        if (c->c_box == 0)
        {
            ebox_draw_background(x, glist);
            c->c_widget.w_paint(x, (t_object *)glist);
            ebox_draw_border(x, glist);
        }
    }
    else if(!vis)
    {
        ebox_erase(x, glist); 
    }
    
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ewidget_displace(t_gobj *z, t_glist *glist, int dx, int dy)
{
    t_ebox *x = (t_ebox *)z;
    
    x->e_obj.te_xpix += dx;
    x->e_obj.te_ypix += dy;
    ebox_move(x, glist);
    canvas_fixlinesfor(glist_getcanvas(glist), (t_text*)x);
}

void ewidget_select(t_gobj *z, t_glist *glist, int selected)
{
    t_ebox *x = (t_ebox *)z;
    x->e_selected = selected;
    ebox_select(x, glist);
}

void ewidget_delete(t_gobj *z, t_glist *glist)
{
    t_ebox *x = (t_ebox *)z;
    
    ebox_erase(x, glist);
    canvas_deletelinesfor(glist, (t_text *)z);
}

int ewidget_mousemove(t_gobj *z, struct _glist *glist, int posx, int posy, int shift, int alt, int ctrl, int mousedown)
{
    t_ebox* x   = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    x->e_mouse.x = posx - text_xpix(&x->e_obj, glist);
    x->e_mouse.y = posy - text_ypix(&x->e_obj, glist);
    
    clock_delay(x->e_deserted_clock, x->e_deserted_time);
    if(epopupmenu_mousemove(x->e_popup, x->e_mouse, mousedown))
        return 1;
    
    x->e_modifiers = EMOD_NONE;
    if(shift && !alt && !ctrl)
    {
        x->e_modifiers = EMOD_SHIFT;
    }
    else if (shift && alt && !ctrl)
    {
        x->e_modifiers = EMOD_SHIFTALT;
    }
    else if(!shift && alt && !ctrl)
    {
        x->e_modifiers = EMOD_ALT;
    }
    
    if(x->e_mouse_down)
    {
        x->e_mouse_down = 0;
        c->c_widget.w_mouseup(x, NULL, x->e_mouse, x->e_modifiers);
    }
    
    c->c_widget.w_mousemove(z, (t_object *)glist, x->e_mouse, x->e_modifiers);
    if(mousedown)
    {       
        x->e_modifiers = EMOD_NONE;
        if(shift && !alt && !ctrl)
        {
            x->e_modifiers = EMOD_SHIFT;
        }
        else if(shift && alt && !ctrl)
        {
            x->e_modifiers = EMOD_SHIFTALT;
        }
        else if(!shift && alt && !ctrl)
        {
            x->e_modifiers = EMOD_ALT;
        }
       
        ewidget_mousedown(x, glist, x->e_mouse, x->e_modifiers);
        glist_grab(glist, &x->e_obj.te_g, (t_glistmotionfn)ewidget_mousedrag, (t_glistkeyfn)ewidget_key, posx, posy);
    }
    
    return 1;
}

void ewidget_mousedown(t_ebox *x, t_glist *glist, t_pt pt, long modifiers)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    x->e_mouse_down = 1;
    
    if(c->c_widget.w_dblclick != NULL && sys_getrealtime() - x->e_lastclick < x->e_dblclick_time)
    {
            c->c_widget.w_dblclick(x, (t_object *)glist, pt, x->e_modifiers);
    }
    if(c->c_widget.w_dblclicklong != NULL && sys_getrealtime() - x->e_lastclick < x->e_dblclicklong_time)
    {
        c->c_widget.w_dblclicklong(x, (t_object *)glist, pt, x->e_modifiers);
    }
    else
        c->c_widget.w_mousedown(x, (t_object *)glist, pt, x->e_modifiers);
    
    x->e_lastclick = sys_getrealtime();
}

void ewidget_mousedrag(t_ebox *x, t_floatarg dx, t_floatarg dy)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    x->e_mouse.x += dx;
    x->e_mouse.y += dy;
    c->c_widget.w_mousedrag(x, x->e_glist, x->e_mouse, x->e_modifiers);
    clock_delay(x->e_deserted_clock, x->e_deserted_time);
}

void ewidget_key(t_ebox *x, t_floatarg fkey)
{
    if(fkey != 0)
    {
        t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
        if(fkey == EKEY_DEL || fkey == EKEY_ESC || fkey == EKEY_TAB || fkey == EKEY_ENTER)
            c->c_widget.w_keyfilter(x, x->e_glist, (char)fkey, x->e_modifiers);
        else
            c->c_widget.w_key(x, x->e_glist, (char)fkey, x->e_modifiers);
    }
    clock_delay(x->e_deserted_clock, x->e_deserted_time);
}

void ewidget_paint(t_ebox *x, t_glist *glist, int mode)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;

    if (c->c_box == 0)
    {
        if(x->e_no_redraw_box == 0)
            ebox_invalidate_layer((t_object *)x, (t_object *)glist, gensym("eboxbd"));
        ebox_draw_background(x, glist);
        ebox_update(x, glist);
        c->c_widget.w_paint(x, (t_object *)glist);
        ebox_draw_border(x, glist);
    }
}

void ewidget_save(t_gobj *z, t_binbuf *b)
{
    t_ebox *x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    char attr_name[256];
    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"), (t_int)x->e_obj.te_xpix, (t_int)x->e_obj.te_ypix);

    
    if(c->c_box)
    {
        long ac = binbuf_getnatom(x->e_obj.te_binbuf);
        t_atom* av = binbuf_getvec(x->e_obj.te_binbuf);
        char mess[256];
        for (int i = 0; i < ac; i++)
        {
            atom_string(av+i, mess, 256);
            binbuf_addv(b, "s", gensym(mess));
        }
    }
    
    if (!c->c_box)
    {
        binbuf_addv(b, "s", gensym(x->e_classname));
        long argc = 0;
        t_atom* argv = NULL;
        for(int i = 0; i < c->c_nattr; i++)
        {
            if(c->c_attr[i].save)
            {
                sprintf(attr_name, "@%s", c->c_attr[i].name->s_name);
                object_attr_getvalueof((t_object *)x, c->c_attr[i].name, &argc, &argv);
                if(argc && argv)
                {
                    binbuf_append_atoms(b, gensym(attr_name), argc, argv);
                    argc = 0;
                    free(argv);
                    argv = NULL;
                }
            }
        }
    }
    
    c->c_widget.w_save(x, b);
    binbuf_addv(b, ";");

}

void ewidget_paint_default(t_ebox *x, t_object *view){;}

void ewidget_mousemove_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers){;}

void ewidget_mousedown_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers){;}

void ewidget_mousedrag_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers){;}

void ewidget_mouseup_default(t_ebox *x, t_object *patcherview, t_pt pt, long modifiers){;}

void ewidget_key_default(t_ebox *x, t_object *patcherview, char textcharacter, long modifiers){;};

void ewidget_keyfilter_default(t_ebox *x, t_object *patcherview, char textcharacter, long modifiers)
{
    ewidget_key_default(x, patcherview, textcharacter, modifiers);
}

void ewidget_getdrawparams_default(t_ebox *x, t_object *patcherview, t_edrawparams *params)
{
    x->e_boxparameters.d_bordercolor = rgba_black;
    x->e_boxparameters.d_borderthickness = 1;
    x->e_boxparameters.d_boxfillcolor = rgba_white;
    x->e_boxparameters.d_cornersize = 0;
}

void ewidget_oksize_default(t_ebox *x, t_rect* rect){;}

t_pd_err ewidget_notify_default(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
    return 0;
}

void ewidget_save_default(t_gobj *z, t_binbuf *b){};

void ewidget_popup_default(t_ebox *x, t_symbol *s, long itemid, t_pt pt){};

void ewidget_dsp_default(t_ebox *x, t_object *dsp, short *count, double samplerate, long vectorsize, long flags){;}

void ewidget_perform_default(t_ebox *x, t_object *dsp, float **ins, long ni, float **outs, long no, long vectorsize, long flags, void *userparam){;}






