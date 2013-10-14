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
    c->c_widget.w_clickfn           = NULL;
    
    c->c_widget.w_paint             = NULL;
    c->c_widget.w_mouseenter        = NULL;
    c->c_widget.w_mouseleave        = NULL;
    c->c_widget.w_mousemove         = NULL;
    c->c_widget.w_mousedown         = NULL;
    c->c_widget.w_mousedrag         = NULL;
    c->c_widget.w_mouseup           = NULL;
    c->c_widget.w_dblclick          = NULL;
    c->c_widget.w_key               = (method)ewidget_key_default;
    c->c_widget.w_keyfilter         = (method)ewidget_keyfilter_default;
    c->c_widget.w_deserted          = NULL;
    c->c_widget.w_getdrawparameters = (method)ewidget_getdrawparams_default;
    c->c_widget.w_notify            = (t_err_method)ewidget_notify_default;
    c->c_widget.w_save              = (method)ewidget_save_default;
    c->c_widget.w_popup             = (method)ewidget_popup_default;
    c->c_widget.w_dsp               = NULL;
    c->c_widget.w_oksize            = (method)ewidget_oksize_default;
}

void ewidget_getrect(t_gobj *z, t_glist *glist, int *xp1, int *yp1, int *xp2, int *yp2)
{
    t_ebox *x = (t_ebox *)z;
    *xp1 = text_xpix(&x->e_obj, glist);
    *yp1 = text_ypix(&x->e_obj, glist);
    *xp2 = text_xpix(&x->e_obj, glist) + (int)x->e_rect.width + (int)(x->e_boxparameters.d_borderthickness);
    *yp2 = text_ypix(&x->e_obj, glist) + (int)x->e_rect.height + (int)(x->e_boxparameters.d_borderthickness);
}

void ewidget_vis(t_gobj *z, t_glist *glist, int vis)
{
    t_ebox* x   = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;

    if(vis && x->e_ready_to_draw)
    {
        if(c->c_box == 0)
        {
            ebox_create_window(x, glist);
            ebox_invalidate_all(x, x->e_canvas);
            ebox_update(x, x->e_canvas);
        
            if(c->c_widget.w_paint)
                c->c_widget.w_paint(x, (t_object *)glist);
            ebox_draw_border(x, glist);
            ebox_draw_iolets(x, glist);
        }
    }
    else
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
    x->e_rect.x = x->e_obj.te_xpix;
    x->e_rect.y = x->e_obj.te_ypix;
    ebox_move(x, glist);
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
    ebox_erase(x, x->e_canvas);
    canvas_deletelinesfor(glist, (t_text *)z);
}


void ewidget_key(t_ebox *x, t_floatarg fkey)
{
    if(fkey != 0)
    {
        t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
        if(fkey == EKEY_DEL || fkey == EKEY_ESC || fkey == EKEY_TAB || fkey == EKEY_ENTER)
            c->c_widget.w_keyfilter(x, x->e_canvas, (char)fkey, x->e_modifiers);
        else
            c->c_widget.w_key(x, x->e_canvas, (char)fkey, x->e_modifiers);
    }
    clock_delay(x->e_deserted_clock, x->e_deserted_time);
}

void ewidget_save(t_gobj *z, t_binbuf *b)
{
	int i;
	char mess[256];
	long argc = 0;
    t_atom* argv = NULL;
    t_ebox *x = (t_ebox *)z;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    char attr_name[256];
    binbuf_addv(b, "ssii", gensym("#X"), gensym("obj"), (t_int)x->e_obj.te_xpix, (t_int)x->e_obj.te_ypix);

    if(c->c_box)
    {
        argc = binbuf_getnatom(x->e_obj.te_binbuf);
        argv = binbuf_getvec(x->e_obj.te_binbuf);
        for (i = 0; i < argc; i++)
        {
            atom_string(argv+i, mess, 256);
            binbuf_addv(b, "s", gensym(mess));
        }
    }
    
    if (!c->c_box)
    {
        binbuf_addv(b, "s", gensym(x->e_classname));
		argc = 0;
        argv = NULL;
        for(i = 0; i < c->c_nattr; i++)
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






