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

#include "eobj.h"

t_class *erouter_class;

void erouter_setup()
{
    t_erouter *x;
    t_class* c = NULL;
    if(gensym("erouter1572")->s_thing != NULL)
        return;
        
    c = class_new(gensym("erouter"), NULL, (t_method)erouter_free, sizeof(t_erouter), CLASS_PD, A_NULL);
    if (c)
    {
        class_addmethod(c, (t_method)eobj_attach_torouter,  gensym("ebox_attach"), A_GIMME, 0);
        class_addmethod(c, (t_method)eobj_detach_torouter,  gensym("ebox_detach"), A_GIMME, 0);
        class_addanything(c, erouter_anything);
        erouter_class = c;
        
        x = (t_erouter *)pd_new(erouter_class);
        pd_bind(&x->e_obj.ob_pd, gensym("#erouter"));
        x->e_nchilds    = 0;
        x->e_childs     = NULL;
        x->e_nlibraries = 0;
        x->e_libraries  = NULL;
        pd_bind(&x->e_obj.te_g.g_pd, gensym("erouter1572"));
    }
}

void erouter_add_libary(t_symbol* name, char* message, char* copyright, char* version)
{
    int i;
    if(gensym("erouter1572")->s_thing == NULL)
        erouter_setup();
    t_erouter *x = (t_erouter *)gensym("erouter1572")->s_thing;
    for(i = 0; i < x->e_nlibraries; i++)
    {
        if(x->e_libraries[i] == name)
            return;
    }
    if(x->e_nlibraries == 0)
    {
        x->e_libraries = (t_symbol **)malloc(sizeof(t_symbol *));
        x->e_libraries[0] = name;
        x->e_nlibraries = 1;
    }
    else
    {
        x->e_libraries = (t_symbol **)realloc(x->e_libraries,(x->e_nlibraries + 1) * sizeof(t_symbol *));
        x->e_libraries[x->e_nlibraries] = name;
        x->e_nlibraries++;
    }
    
    post(message);
    post(copyright);
    post(version);
}

void erouter_free(t_erouter *x)
{
	if(x->e_childs)
	{
		free(x->e_childs);
		x->e_childs = NULL;
	}
}

void eobj_detach_torouter(t_object* child)
{
    int i;
    if(gensym("erouter1572")->s_thing == NULL)
        erouter_setup();
    t_erouter *x = (t_erouter *)gensym("erouter1572")->s_thing;
    if(x)
    {
        for(i = 0; i < x->e_nchilds; i++)
        {
            if(x->e_childs[i] == child)
            {
                x->e_childs[i] = NULL;
            }
        }
    }
}

void eobj_attach_torouter(t_object* child)
{
    int i;
    if(gensym("erouter1572")->s_thing == NULL)
        erouter_setup();
    t_erouter *x = (t_erouter *)gensym("erouter1572")->s_thing;
    for(i = 0; i < x->e_nchilds; i++)
    {
        if(x->e_childs[i] == child)
        {
            //post("erouter already attach to %s", x->e_childs[i]->s_name);
            return;
        }
    }
    if(!x->e_nchilds || x->e_childs == NULL)
    {
        x->e_childs = (t_object **)calloc(1, sizeof(t_object *));
        if(x->e_childs)
        {
            x->e_childs[0]  = child;
            x->e_nchilds    = 1;
            return;
        }
        else
        {
            x->e_childs     = NULL;
            x->e_nchilds    = 0;
            return;
            //post("erouter can't attach to %s", x->e_childs[i]->s_name);
        }
    }
    else
    {
        for(i = 0; i < x->e_nchilds; i++)
        {
            if(x->e_childs[i] == NULL)
            {
                x->e_childs[i]  = child;
                return;
            }
        }
        
        x->e_childs = (t_object **)realloc(x->e_childs, (x->e_nchilds + 1) * sizeof(t_object *));
        if(x->e_childs)
        {
            x->e_childs[x->e_nchilds]  = child;
            x->e_nchilds++;
        }
        else
        {
            x->e_childs     = NULL;
            x->e_nchilds    = 0;
            //post("erouter can't attach to %s", x->e_childs[i]->s_name);
        }
    }
}

long erouter_getnobjects()
{
    if(gensym("erouter1572")->s_thing == NULL)
        erouter_setup();
    t_erouter *x = (t_erouter *)gensym("erouter1572")->s_thing;
    if(x)
        return x->e_nchilds;
    else
        return 0;
}

t_object* erouter_getobject(long index)
{
    if(gensym("erouter1572")->s_thing == NULL)
        erouter_setup();
    t_erouter *x = (t_erouter *)gensym("erouter1572")->s_thing;
    if(x)
    {
       if(index >= 0 && index < x->e_nchilds)
           return x->e_childs[index];
    }
    return NULL;
}

void erouter_anything(t_erouter *x, t_symbol *s, long argc, t_atom *argv)
{
    int i;
    t_eobj* z;
    rmethod nrmethod = NULL;
    if(argc >= 1 && argv && atom_gettype(argv) == A_SYM)
    {
        for(i = 0; i < x->e_nchilds; i++)
        {
            if(x->e_childs[i] != NULL)
            {
                z = (t_eobj *)x->e_childs[i];
                if(z->o_id == s)
                {
                    nrmethod = (rmethod)zgetfn(&z->o_obj.te_g.g_pd, atom_getsym(argv));
                    if(nrmethod)
                    {
                        pd_typedmess((t_pd *)z, atom_getsym(argv), argc, argv);
                    }
                }
            }
        }
    }
}


