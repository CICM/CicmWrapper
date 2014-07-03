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

static t_class *erouter_class;
static t_symbol* erouter1572_sym;

void erouter_setup()
{
    t_erouter *x;
    t_class* c = NULL;
    int right = EMOD_CMD;
#ifdef __APPLE__
    
#elif _WINDOWS
	right += 8;
#else
    right += 16;
#endif
    
    erouter1572_sym = gensym("erouter1572");
    
    if(erouter1572_sym->s_thing != NULL)
        return;
    
    c = class_new(gensym("erouter"), NULL, (t_method)erouter_free, sizeof(t_erouter), CLASS_PD, A_NULL);
    if (c)
    {
        class_addmethod(c, (t_method)erouter_mousedown,         gensym("mousedown"), A_GIMME, 0);
        class_addmethod(c, (t_method)erouter_mouseup,           gensym("mouseup"), A_GIMME, 0);
        class_addmethod(c, (t_method)erouter_mousemove,         gensym("mousemove"), A_GIMME, 0);
        class_addmethod(c, (t_method)erouter_mouseglobal,       gensym("globalmouse"), A_FLOAT, A_FLOAT, 0);
        class_addmethod(c, (t_method)erouter_mousetopcanvas,    gensym("topcanvasmouse"), A_FLOAT, A_FLOAT, 0);
        
        class_addanything(c, erouter_anything);
        erouter_class = c;
        
        x = (t_erouter *)pd_new(erouter_class);
        pd_bind(&x->e_obj.ob_pd, gensym("#erouter"));
        pd_bind(&x->e_obj.te_g.g_pd, erouter1572_sym);
        
        x->e_nchilds    = 0;
        x->e_childs     = NULL;
        x->e_nlibraries = 0;
        x->e_libraries  = NULL;
        x->e_clock      = clock_new(x, (t_method)erouter_tick);
        clock_set(x->e_clock, 20);
        
        sys_vgui("bind all <Button-3> {pdsend {%s mousedown %%x %%y %i}}\n", erouter1572_sym->s_name, right);
        sys_vgui("bind all <Button-2> {pdsend {%s mousedown %%x %%y %i}}\n", erouter1572_sym->s_name, right);
        sys_vgui("bind all <Button-1> {pdsend {%s mousedown %%x %%y %%s}}\n", erouter1572_sym->s_name);
        sys_vgui("bind all <ButtonRelease> {pdsend {%s mouseup %%x %%y %%s}}\n", erouter1572_sym->s_name);
        sys_vgui("bind all <Motion> {pdsend {%s mousemove %%x %%y %%s}}\n", erouter1572_sym->s_name);
        
        sys_gui("namespace eval erouter1572 {} \n");
        
        // PATCHER MOUSE POSITION //
        sys_vgui("proc eobj_canvas_mouse {target patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_gui(" pdsend \"$target canvasmouse  [expr $x - $rx] [expr $y - $ry] \"\n");
        sys_gui("}\n");
        
        // GLOBAL MOUSE POSITION //
        sys_gui("proc erouter_global_mouse {} {\n");
        sys_gui(" set x [winfo pointerx .]\n");
        sys_gui(" set y [winfo pointery .]\n");
        sys_vgui(" pdsend \"%s globalmouse $x $y\"\n", erouter1572_sym->s_name);
        sys_gui("}\n");
        
        // TOP CANVAS MOUSE POSITION //
        sys_gui("proc erouter_topcanvas_mouse {} {\n");
        sys_gui(" set patcher [focus]\n");
        sys_gui(" set x [winfo rootx [winfo name .]]\n");
        sys_gui(" set y [winfo rooty [winfo name .]]\n");
        sys_vgui(" pdsend \"%s topcanvasmouse $x $y\"\n", erouter1572_sym->s_name);
        sys_gui("}\n");
        
        eproxy_setup();
    }
}

void erouter_free(t_erouter *x)
{
	if(x->e_childs)
	{
		free(x->e_childs);
		x->e_childs = NULL;
	}
    clock_free(x->e_clock);
}

void eobj_detach_torouter(t_object* child)
{
    int i;
	t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
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
	t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
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
        }
    }
}

long erouter_getnobjects()
{
	t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
    if(x)
        return x->e_nchilds;
    else
        return 0;
}

t_object* erouter_getobject(long index)
{
	t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
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
                        pd_typedmess((t_pd *)z, atom_getsym(argv), (int)argc, argv);
                    }
                }
            }
        }
    }
}

void erouter_mousedown(t_erouter *x, t_symbol *s, int argc, t_atom *argv)
{
    x->e_mouse_down = 1;
    x->e_mouse_modifier = (long)atom_getfloat(argv+2);
#ifdef __APPLE__
    
#elif _WINDOWS
    
	if(x->e_mouse_modifier >= 131080)
	{
		x->e_mouse_modifier -= 131080;
		x->e_mouse_modifier += EMOD_ALT;
	}
	else
		x->e_mouse_modifier -= 8;
#else
    x->e_mouse_modifier -= 16;
#endif
}

void erouter_mouseup(t_erouter *x, t_symbol *s, int argc, t_atom *argv)
{
    x->e_mouse_down = 0;
    x->e_mouse_modifier = (long)atom_getfloat(argv+2);
#ifdef __APPLE__
    
#elif _WINDOWS
    
	if(x->e_mouse_modifier >= 131080)
	{
		x->e_mouse_modifier -= 131080;
		x->e_mouse_modifier += EMOD_ALT;
	}
	else
		x->e_mouse_modifier -= 8;
#else
    x->e_mouse_modifier -= 16;
#endif
}

void erouter_mousemove(t_erouter *x, t_symbol *s, int argc, t_atom *argv)
{
    x->e_mouse_modifier = (long)atom_getfloat(argv+2);
#ifdef __APPLE__
    
#elif _WINDOWS
    
	if(x->e_mouse_modifier >= 131080)
	{
		x->e_mouse_modifier -= 131080;
		x->e_mouse_modifier += EMOD_ALT;
	}
	else
		x->e_mouse_modifier -= 8;
#else
    x->e_mouse_modifier -= 16;
#endif
}

void erouter_tick(t_erouter * x)
{
    sys_gui("erouter_global_mouse\n");
    clock_delay(x->e_clock, 20.);
}

void erouter_mouseglobal(t_erouter *x, float px, float py)
{
    x->e_mouse_global_position.x = px;
    x->e_mouse_global_position.y = py;
}

void erouter_mousetopcanvas(t_erouter *x, float px, float py)
{
    x->e_mouse_topcanvas_position.x = px;
    x->e_mouse_topcanvas_position.y = py;
}

t_pt erouter_getmouse_global_position()
{
    t_pt pt;
    t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
    if(x)
    {
        return x->e_mouse_global_position;
    }
    else
    {
        pt.x = 0;
        pt.y = 0;
        return pt;
    }
}

t_pt erouter_getmouse_topcanvas_position()
{
    t_pt pt;
    t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
    if(x)
    {
        return x->e_mouse_topcanvas_position;
    }
    else
    {
        pt.x = 0;
        pt.y = 0;
        return pt;
    }
}

long erouter_getmouse_modifier()
{
    t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
    if(x)
    {
        if(x->e_mouse_modifier >= 256)
            return x->e_mouse_modifier - 256;
        else
            return x->e_mouse_modifier;
    }
    else
    {
        return 0;
    }
}

char erouter_getmouse_status()
{
    t_erouter *x = NULL;
    if(erouter1572_sym->s_thing == NULL)
        erouter_setup();
    x = (t_erouter *)erouter1572_sym->s_thing;
    if(x)
    {
        return x->e_mouse_down;
    }
    else
    {
        return 0;
    }
}






