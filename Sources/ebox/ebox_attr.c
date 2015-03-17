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

//! Change the attributes with an array of atoms
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param argc      The size of the atoms array
 \ @param argv      The atoms array
 \ @return          Nothing
 */
void ebox_attrprocess_viatoms(void *x, long argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    long    defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

//! Change the attributes with a binbuf
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @param d         The binbuf
 \ @return          Nothing
 */
void ebox_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    int i;
    char attr_name[EPD_MAX_SIGS];
    
    long defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

//! Change the attributes with their defaults values
/*
 \ @memberof        ebox
 \ @param x         The object
 \ @return          Nothing
 */
void ebox_attrprocess_default(void *x)
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
            defv = (t_atom *)calloc(defc, sizeof(t_atom));
            if(defc && defv)
            {
                char check = 0;
                char* str_start = c->c_attr[i]->defvals->s_name;
                for(j = 0; j < defc; j++)
                {
                    for(k = 0; k < strlen(str_start); k++)
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
                object_attr_setvalueof((t_object *)x, c->c_attr[i]->name, defc, defv);
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
    }
}






