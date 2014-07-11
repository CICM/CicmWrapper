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

#include "../c.library.h"


typedef struct  _patcherinfos
{
	t_eobj      j_box;
    t_canvas*   f_canvas;
	t_outlet*   f_out_name;
    t_outlet*   f_out_path;
    t_outlet*   f_out_coords;

    double      f_time;
    long        f_notify;
} t_patcherinfos;

t_eclass *patcherinfos_class;

void *patcherinfos_new(t_symbol *s, int argc, t_atom *argv);
void patcherinfos_output(t_patcherinfos *x);
void patcherinfos_free(t_patcherinfos *x);
void patcherinfos_assist(t_patcherinfos *x, void *b, long m, long a, char *s);
void patcherinfos_click(t_patcherinfos *x);
void patcherinfos_bang(t_patcherinfos *x);
void canvas_dialog_alias(t_glist *x, t_symbol *s, int argc, t_atom *argv);

t_gotfn cnv_method;

extern "C" void setup_c0x2epatcherinfos(void)
{
	t_eclass *c;
    
	c = eclass_new("c.patcherinfos", (method)patcherinfos_new, (method)patcherinfos_free, (short)sizeof(t_patcherinfos), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)patcherinfos_new, gensym("c.canvasinfos"), A_GIMME, 0);
    cream_initclass(c);
    
    eclass_addmethod(c, (method)patcherinfos_output,      "bang",       A_CANT, 0);
    eclass_addmethod(c, (method)patcherinfos_click,       "click",      A_CANT, 0);
    
    CLASS_ATTR_LONG             (c, "notify", 0, t_patcherinfos, f_notify);
    CLASS_ATTR_LABEL            (c, "notify", 0, "Notify mode");
    CLASS_ATTR_DEFAULT          (c, "notify", 0, "0");
    CLASS_ATTR_ORDER            (c, "notify", 0, "1");
    CLASS_ATTR_FILTER_CLIP      (c, "notify", 0, 1);
    CLASS_ATTR_SAVE             (c, "notify", 0);
    
    eclass_register(CLASS_OBJ, c);
	patcherinfos_class = c;
}

void canvas_setgraph(t_glist *x, int flag, int nogoprect)
{
    if (!flag && glist_isgraph(x))
    {
        int hadeditor = (x->gl_editor != 0);
        if (hadeditor)
            canvas_destroy_editor(x);
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
            gobj_vis(&x->gl_gobj, x->gl_owner, 0);
        x->gl_isgraph = 0;
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
        {
            gobj_vis(&x->gl_gobj, x->gl_owner, 1);
            canvas_fixlinesfor(x->gl_owner, &x->gl_obj);
        }
    }
    else if (flag)
    {
        if (x->gl_pixwidth <= 0)
            x->gl_pixwidth = GLIST_DEFGRAPHWIDTH;
        
        if (x->gl_pixheight <= 0)
            x->gl_pixheight = GLIST_DEFGRAPHHEIGHT;
        
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
            gobj_vis(&x->gl_gobj, x->gl_owner, 0);
        x->gl_isgraph = 1;
        x->gl_hidetext = !(!(flag&2));
        x->gl_goprect = !nogoprect;
        if (glist_isvisible(x) && x->gl_goprect)
            glist_redraw(x);
        if (x->gl_owner && !x->gl_loading && glist_isvisible(x->gl_owner))
        {
            gobj_vis(&x->gl_gobj, x->gl_owner, 1);
            canvas_fixlinesfor(x->gl_owner, &x->gl_obj);
        }
    }
}

void canvas_dialog_alias(t_glist *x, t_symbol *s, int argc, t_atom *argv)
{
    t_gobj *y = NULL;
    t_patcherinfos *z = NULL;
    
    t_float xperpix, yperpix, x1, y1, x2, y2, xpix, ypix, xmargin, ymargin;
    int graphme, redraw = 0;
    
    xperpix = atom_getfloatarg(0, argc, argv);
    yperpix = atom_getfloatarg(1, argc, argv);
    graphme = (int)(atom_getfloatarg(2, argc, argv));
    x1 = atom_getfloatarg(3, argc, argv);
    y1 = atom_getfloatarg(4, argc, argv);
    x2 = atom_getfloatarg(5, argc, argv);
    y2 = atom_getfloatarg(6, argc, argv);
    xpix = atom_getfloatarg(7, argc, argv);
    ypix = atom_getfloatarg(8, argc, argv);
    xmargin = atom_getfloatarg(9, argc, argv);
    ymargin = atom_getfloatarg(10, argc, argv);
    
    x->gl_pixwidth = xpix;
    x->gl_pixheight = ypix;
    x->gl_xmargin = xmargin;
    x->gl_ymargin = ymargin;
    
    yperpix = -yperpix;
    if (xperpix == 0)
        xperpix = 1;
    if (yperpix == 0)
        yperpix = 1;
    
    if (graphme)
    {
        if (x1 != x2)
            x->gl_x1 = x1, x->gl_x2 = x2;
        else x->gl_x1 = 0, x->gl_x2 = 1;
        if (y1 != y2)
            x->gl_y1 = y1, x->gl_y2 = y2;
        else x->gl_y1 = 0, x->gl_y2 = 1;
    }
    else
    {
        if (xperpix != glist_dpixtodx(x, 1) || yperpix != glist_dpixtody(x, 1))
            redraw = 1;
        if (xperpix > 0)
        {
            x->gl_x1 = 0;
            x->gl_x2 = xperpix;
        }
        else
        {
            x->gl_x1 = -xperpix * (x->gl_screenx2 - x->gl_screenx1);
            x->gl_x2 = x->gl_x1 + xperpix;
        }
        if (yperpix > 0)
        {
            x->gl_y1 = 0;
            x->gl_y2 = yperpix;
        }
        else
        {
            x->gl_y1 = -yperpix * (x->gl_screeny2 - x->gl_screeny1);
            x->gl_y2 = x->gl_y1 + yperpix;
        }
    }
    /* LATER avoid doing 2 redraws here (possibly one inside setgraph) */
    canvas_setgraph(x, graphme, 0);
    canvas_dirty(x, 1);
    if (x->gl_havewindow)
        canvas_redraw(x);
    else if (glist_isvisible(x->gl_owner))
    {
        gobj_vis(&x->gl_gobj, x->gl_owner, 0);
        gobj_vis(&x->gl_gobj, x->gl_owner, 1);
    }
    
    for(y = x->gl_list; y; y = y->g_next)
    {
        if(eobj_getclassname(y) == gensym("c.patcherinfos"))
        {
            z = (t_patcherinfos *)y;
            if(z->f_notify)
                pd_bang((t_pd *)z);
        }
    }

}

void *patcherinfos_new(t_symbol *s, int argc, t_atom *argv)
{
	t_patcherinfos *x =  NULL;
    t_binbuf *d;
    x = (t_patcherinfos *)eobj_new(patcherinfos_class);
    
    if (!(d = binbuf_via_atoms(argc,argv)))
        return NULL;
    
    if(x)
    {
        if(canvas_getcurrent())
        {
            x->f_canvas = glist_getcanvas(canvas_getcurrent());
            for(int i = 0; i < x->f_canvas->gl_pd->c_size - 90; i++)
            {
                if(x->f_canvas->gl_obj.te_g.g_pd->c_methods[i].me_name == gensym("donecanvasdialog"))
                {
                    cnv_method = x->f_canvas->gl_obj.te_g.g_pd->c_methods[i].me_fun;
                    x->f_canvas->gl_obj.te_g.g_pd->c_methods[i].me_fun = (t_gotfn)canvas_dialog_alias;
                    break;
                }
            }
            
        }
        else
            x->f_canvas = NULL;
        
        x->f_out_name = (t_outlet *)symbolout(x);
        x->f_out_path = (t_outlet *)symbolout(x);
        x->f_out_coords = (t_outlet *)listout(x);
        x->f_time = clock_getsystime();
        
        
        ebox_attrprocess_viabinbuf(x, d);
    }
    
    return (x);
}

void patcherinfos_click(t_patcherinfos *x)
{
    if(clock_gettimesince(x->f_time) < 250.)
        patcherinfos_output(x);
    x->f_time = clock_getsystime();
}

void patcherinfos_output(t_patcherinfos *x)
{
    t_atom av[4];
    if(x->f_canvas)
    {
        outlet_symbol(x->f_out_name, x->f_canvas->gl_name);
        outlet_symbol(x->f_out_path, canvas_getdir(x->f_canvas));
  
        atom_setfloat(av, x->f_canvas->gl_screenx1);
        atom_setfloat(av+1, x->f_canvas->gl_screeny1);
        atom_setfloat(av+2, x->f_canvas->gl_screenx2);
        atom_setfloat(av+3, x->f_canvas->gl_screeny2);
        outlet_anything(x->f_out_coords, gensym("windowsize"), 4, av);
        
        atom_setfloat(av, x->f_canvas->gl_xmargin);
        atom_setfloat(av+1, x->f_canvas->gl_ymargin);
        atom_setfloat(av+2, x->f_canvas->gl_pixwidth);
        atom_setfloat(av+3, x->f_canvas->gl_pixheight);
        outlet_anything(x->f_out_coords, gensym("canvassize"), 4, av);
    }
}

void patcherinfos_free(t_patcherinfos *x)
{
    eobj_free(x);
}

void patcherinfos_assist(t_patcherinfos *x, void *b, long m, long a, char *s)
{
	;
}


