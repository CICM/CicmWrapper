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

t_erouter* my_erouter;
t_class *erouter_class;

t_erouter* glist_return_erouter(t_object* glist)
{
    return my_erouter;
}

void erouter_setup(t_glist* obj)
{
    int i;
    t_erouter *x;
    rmethod nrmethod = NULL;
    t_class* glist = obj->gl_obj.te_g.g_pd;
    t_class* c = NULL;
    if(!glist)
    {
        return;
    }
    
    for(i = 0; i < glist->c_nmethod; i++)
    {
        if(glist->c_methods[i].me_name == gensym("erouter"))
        {
            nrmethod = (rmethod)glist->c_methods[i].me_fun;
            my_erouter = (t_erouter*)nrmethod(obj);
            if(my_erouter != NULL)
                return;
        }
    }

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
        
        my_erouter = x;
        class_addmethod(glist, (t_method)glist_return_erouter, gensym("erouter"), A_GIMME, 0);
        
        post("Chocolate and Coffee Libraries by Pierre Guillot");
        post("© 2013 - 2014  CICM | Paris 8 University");
        post("Version Beta 0.1");
    }
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
    t_erouter *x = my_erouter;
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
    t_erouter *x = my_erouter;
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
    t_erouter *x = my_erouter;
    if(x)
        return x->e_nchilds;
    else
        return 0;
}

t_object* erouter_getobject(long index)
{
    t_erouter *x = my_erouter;
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


