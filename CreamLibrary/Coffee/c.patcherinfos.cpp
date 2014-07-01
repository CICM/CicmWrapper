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

void canvas_dialog_alias(t_glist *x, t_symbol *s, int argc, t_atom *argv)
{
    t_gobj *y = NULL;
    t_patcherinfos *z = NULL;
    cnv_method(x, s, argc, argv);
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


