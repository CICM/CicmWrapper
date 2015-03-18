/*
 * CicmWrapper - Pure Data Enhanced
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

//! Method to get the attributes
/*
 \ @memberof        eattr
 \ @param x         The object pointer
 \ @param s         The attribute name
 \ @param argc      The pointer to an int that will contains the size of the attributes
 \ @param argv      The pointer to an array of atoms that will contains the attributes values
 \ @return          Nothing
*/
void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv)
{
    int i, j;
    char *point;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->b_obj.o_obj.te_g.g_pd;
    if(argv[0])
        free(argv);
    argc[0] = 0;

    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == s)
        {
            t_symbol* type = c->c_attr[i]->type;
            if(c->c_attr[i]->sizemax == 0)
                argc[0] = (int)c->c_attr[i]->size;
            else
            {
                point = (char *)x + c->c_attr[i]->size;
                argc[0] = (int)point[0];
                if(argc[0] > c->c_attr[i]->sizemax)
                    argc[0] = (int)c->c_attr[i]->sizemax;
            }

            point = (char *)x + c->c_attr[i]->offset;

            argv[0] = (t_atom *)calloc(argc[0], sizeof(t_atom));
            if(c->c_attr[i]->getter)
            {
                c->c_attr[i]->getter(x, c->c_attr[i], argc, argv);
            }
            else if(type == gensym("int"))
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setlong(argv[0]+j, ((int *)point)[j]);
                }
            }
            else if(type == gensym("long"))
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setlong(argv[0]+j, ((long *)point)[j]);
                }
            }
            else if(type == gensym("float"))
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setfloat(argv[0]+j, ((float *)point)[j]);
                }
            }
            else if(type == gensym("double"))
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setfloat(argv[0]+j, ((double *)point)[j]);
                }
            }
            else if(type == gensym("symbol"))
            {
                t_symbol** syms = (t_symbol **)point;
                for(j = 0; j < argc[0]; j++)
                {
                    if(syms[j])
                    {
                        atom_setsym(argv[0]+j, gensym(syms[j]->s_name));
                    }
                }
            }
            else if(type == gensym("atom"))
            {
                for(j = 0; j < argc[0]; j++)
                {
                    argv[0][j] = ((t_atom *)point)[j];
                }
            }
        }
    }
}
