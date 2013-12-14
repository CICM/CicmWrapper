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

#include "erouter.h"

void ebox_router(t_ebox* x, t_symbol* s, long argc, t_atom* argv)
{
    if(s == gensym("attach"))
    {

        sys_gui("proc erouter_attach {target} {\n");
        sys_gui("global erouter\n");
        sys_gui("if { [info exists erouter] == 0} {\n");
        sys_gui("pdsend \"$target router notalloc attach\"\n");
        sys_gui("} else {\n");
        sys_gui("pdsend \"#erouter ebox_attach $target\"\n");
        sys_gui("}\n");
        sys_gui("}\n");
        
        sys_vgui("erouter_attach %s\n", x->e_object_id->s_name);
    }
    else if(s == gensym("detach"))
    {
        sys_gui("proc erouter_detach {target} {\n");
        sys_gui("global erouter\n");
        sys_gui("if { [info exists erouter] == 0} {\n");
        sys_gui("pdsend \"$target router notalloc detach\"\n");
        sys_gui("} else {\n");
        sys_gui("pdsend \"#erouter ebox_detach $target\"\n");
        sys_gui("}\n");
        sys_gui("}\n");
        
        sys_vgui("erouter_detach %s\n", x->e_object_id->s_name);
    }
    else if(atom_getsym(argv) == gensym("notalloc"))
    {
        erouter_setup();
        if(atom_getsym(argv+1) == gensym("attach"))
            sys_vgui("erouter_attach %s\n", x->e_object_id->s_name);
        if(atom_getsym(argv+1) == gensym("detach"))
            sys_vgui("erouter_detach %s\n", x->e_object_id->s_name);
    }
}

void erouter_setup()
{
    t_class* c;
    t_erouter *x;
    c = class_new(gensym("erouter"), NULL, (t_method)erouter_free, sizeof(t_erouter), CLASS_PD, 0);

    class_addmethod(c, (t_method)erouter_attach,  gensym("ebox_attach"), A_SYMBOL);
    class_addmethod(c, (t_method)erouter_detach,  gensym("ebox_detach"), A_SYMBOL);
    class_addanything(c, erouter_anything);
    erouter_class = c;

    x = (t_erouter *)pd_new(erouter_class);
    pd_bind(&x->e_obj.ob_pd, gensym("#erouter"));
    x->e_nchilds    = 0;
    x->e_childs     = NULL;
    sys_vgui("set erouter %ld\n", (long)x);
}

void erouter_free(t_erouter *x)
{
    free(x->e_childs);
}

void erouter_detach(t_erouter *x, t_symbol* child)
{
    int i, j;
    for(i = 0; i < x->e_nchilds; i++)
    {
        if(x->e_childs[i] == child)
        {
            for(j = i; j < x->e_nchilds-1; j++)
            {
                x->e_childs[j] = x->e_childs[j+1];
            }
            x->e_nchilds--;
        }
    }
    x->e_childs = (t_symbol **)realloc(x->e_childs, x->e_nchilds * sizeof(t_symbol *));
    if(!x->e_childs)
    {
        x->e_childs     = NULL;
        x->e_nchilds    = 0;
    }
}

void erouter_attach(t_erouter *x, t_symbol* child)
{
    int i;
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
        x->e_childs = (t_symbol **)calloc(1, sizeof(t_symbol *));
        if(x->e_childs)
        {
            x->e_childs[0]  = child;
            x->e_nchilds    = 1;
        }
        else
        {
            x->e_childs     = NULL;
            x->e_nchilds    = 0;
            //post("erouter can't attach to %s", x->e_childs[i]->s_name);
        }
    }
    else
    {
        x->e_childs = (t_symbol **)realloc(x->e_childs, (x->e_nchilds + 1) * sizeof(t_symbol *));
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

void erouter_anything(t_erouter *x, t_symbol *s, long argc, t_atom *argv)
{
    int i;
    if(argc >= 1 && argv && atom_gettype(argv) == A_SYM)
    {
        for(i = 0; i < x->e_nchilds; i++)
        {
            if(x->e_childs[i] == s)
            {
                sys_vgui("pdsend {%s %s}\n", s->s_name, atom_getsym(argv)->s_name);
            }
        }
    }
}


