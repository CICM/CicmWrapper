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

#include "eattr.h"

void cicm_class_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
{
	int i, j;
    t_ebox* z = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->e_obj.te_g.g_pd;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].name == s)
        {
            t_symbol* type = c->c_attr[i].type;
            char *point = (char *)(x) + c->c_attr[i].offset;
            if(c->c_attr[i].clipped == 1 || c->c_attr[i].clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_min(atom_getfloat(argv+j), c->c_attr[i].minimum));
                    }
                }
            }
            if(c->c_attr[i].clipped == 2 || c->c_attr[i].clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_max(atom_getfloat(argv+j), c->c_attr[i].maximum));
                    }
                }
            }
            
            if(c->c_attr[i].setter)
            {
                c->c_attr[i].setter(x, c->c_attr[i].obj, argc, argv);
            }
            else if(type == gensym("int"))
            {
                int* pointor = (int *)point;
                for(j = 0; j < c->c_attr[i].size && j < argc; j++)
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
                for(j = 0; j < c->c_attr[i].size && j < argc; j++)
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
                for(j = 0; j < c->c_attr[i].size && j < argc; j++)
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
                for(j = 0; j < c->c_attr[i].size && j < argc; j++)
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
                for(j = 0; j < c->c_attr[i].size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_SYM)
                    {
                        pointor[j] = gensym(atom_getsymbol(argv+j)->s_name);
                    }
                }
            }
            c->c_widget.w_notify(x, s, gensym("attr_modified"), NULL, NULL);
            if(c->c_attr[i].paint)
            {
                c->c_widget.w_oksize(x, &z->e_rect);
                c->c_widget.w_getdrawparameters(x, NULL, &z->e_boxparameters);
                ewidget_vis((t_gobj *)x, z->e_canvas, 1);
            }
            
        }
    }
}

