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

typedef struct  _patchermess
{
	t_eobj      j_box;
    t_canvas*   f_canvas;
    
} t_patchermess;

t_eclass *patchermess_class;

void *patchermess_new(t_symbol *s, int argc, t_atom *argv);
void patchermess_free(t_patchermess *x);
void patchermess_assist(t_patchermess *x, void *b, long m, long a, char *s);
void patchermess_anything(t_patchermess *x, t_symbol *s, long argc, t_atom* argv);

extern "C" void setup_c0x2epatchermess(void)
{
	t_eclass *c;
    
	c = eclass_new("c.patchermess", (method)patchermess_new, (method)patchermess_free, (short)sizeof(t_patchermess), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)patchermess_new, gensym("c.canvasmess"), A_GIMME, 0);
    cream_initclass(c);
    
    eclass_addmethod(c, (method)patchermess_anything, "anything", A_GIMME, 0);
    
    eclass_register(CLASS_OBJ, c);
	patchermess_class = c;
}

void *patchermess_new(t_symbol *s, int argc, t_atom *argv)
{
	t_patchermess *x =  NULL;
    
    x = (t_patchermess *)eobj_new(patchermess_class);
    if(x)
    {
        if(canvas_getcurrent())
        {
            x->f_canvas = glist_getcanvas(canvas_getcurrent());
        }
        else
            x->f_canvas = NULL;
        
    }
    
    return (x);
}

void patchermess_anything(t_patchermess *x, t_symbol *s, long argc, t_atom* argv)
{
    if(eobj_getcanvas(x))
    {
        pd_typedmess((t_pd *)eobj_getcanvas(x), s, argc, argv);
    }
}

void patchermess_free(t_patchermess *x)
{
    eobj_free(x);
}

void patchermess_assist(t_patchermess *x, void *b, long m, long a, char *s)
{
	;
}


