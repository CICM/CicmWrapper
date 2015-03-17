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

#include "eclass.h"

//! Method to set the attributes
/*
 \ @memberof        eattr
 \ @param x         The object pointer
 \ @param s         The attribute name
 \ @param argc      The size of the array of atoms
 \ @param argv      The array of atoms that contains the attributes values
 \ @return          Nothing
*/
void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
{
	long i, j, size;
    char *point;
    long *point_size;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->b_obj.o_obj.te_g.g_pd;
    
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == s)
        {
            t_symbol* type = c->c_attr[i]->type;
            if(c->c_attr[i]->sizemax == 0)
                size = c->c_attr[i]->size;
            else
            {
                if(argc > c->c_attr[i]->sizemax)
                    argc = (int)c->c_attr[i]->sizemax;
                size = argc;
                point = (char *)x + c->c_attr[i]->size;
                point_size = (long *)point;
                point_size[0] = (long)size;
            }
            
            point = (char *)(x) + c->c_attr[i]->offset;
            
            if(c->c_attr[i]->clipped == 1 || c->c_attr[i]->clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_min(atom_getfloat(argv+j), c->c_attr[i]->minimum));
                    }
                }
            }
            if(c->c_attr[i]->clipped == 2 || c->c_attr[i]->clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_max(atom_getfloat(argv+j), c->c_attr[i]->maximum));
                    }
                }
            }
 
            if(c->c_attr[i]->setter)
            {
                c->c_attr[i]->setter(x, c->c_attr[i], argc, argv);
            }
            else if(type == gensym("int"))
            {
                int* pointor = (int *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_LONG)
                    {
                        pointor[j] = (int)atom_getlong(argv+j);
                    }
                }
            }
            else if(type == gensym("long"))
            {
                long* pointor = (long *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_LONG)
                    {
                        pointor[j] = (long)atom_getlong(argv+j);
                    }
                }
            }
            else if(type == gensym("float"))
            {
                float* pointor = (float *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (float)atom_getfloat(argv+j);
                    }
                }
            }
            else if(type == gensym("double"))
            {
                double* pointor = (double *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (double)atom_getfloat(argv+j);
                    }
                }
            }
            else if(type == gensym("symbol"))
            {
                t_symbol** pointor = (t_symbol **)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_SYM)
                    {
                        pointor[j] = gensym(atom_getsymbol(argv+j)->s_name);
                    }
                }
            }
            else if(type == gensym("atom"))
            {
                t_atom* pointor = (t_atom *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    pointor[j] = argv[j];
                }
            }
            
            ebox_notify(z, s, gensym("attr_modified"), NULL, NULL);
            if(c->c_widget.w_notify != NULL)
                c->c_widget.w_notify(x, s, gensym("attr_modified"), NULL, NULL);
            
            if(c->c_attr[i]->paint)
            {
                if(c->c_widget.w_oksize != NULL)
                    c->c_widget.w_oksize(x, &z->b_rect);
                if(c->c_widget.w_getdrawparameters != NULL)
                    c->c_widget.w_getdrawparameters(x, NULL, &z->b_boxparameters);
                
                ebox_redraw(z);
            }
            if(eobj_isbox(x) && ebox_isdrawable((t_ebox*) x))
            {
                canvas_dirty(eobj_getcanvas(x), 1);
            }
        }
    }
}

