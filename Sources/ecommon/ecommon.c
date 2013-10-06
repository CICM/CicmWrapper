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

#include "ecommon.h"

void outlet_int(t_outlet* outlet, int val)
{
    outlet_float(outlet, val);
}

void* listout(void *x)
{
    return outlet_new((t_object *)x, &s_list);
}

void* floatout(void *x)
{
    return outlet_new((t_object *)x, &s_float);
}

int obj_isfloatoutlet(t_object *x, int m)
{
    int n;
    t_outlet *o2;
    for (o2 = x->ob_outlet, n = 0; o2 && m--; o2 = o2->o_next);
    return (o2 && (o2->o_sym == &s_float));
}

int obj_isfloatinlet(t_object *x, int m)
{
    t_inlet *i;
    if (x->ob_pd->c_firstin)
    {
        if (!m)
            return (x->ob_pd->c_firstin && x->ob_pd->c_floatsignalin);
        else m--;
    }
    for (i = x->ob_inlet; i && m; i = i->i_next, m--)
        ;
    return (i && (i->i_symfrom == &s_float));
}

void canvas_deletelines_for_io(t_canvas *x, t_text *text, t_inlet *inp, t_outlet *outp)
{
    t_linetraverser t;
    t_outconnect *oc;
    linetraverser_start(&t, x);
    while ((oc = linetraverser_next(&t)))
    {
        if ((t.tr_ob == text && t.tr_outlet == outp) ||
            (t.tr_ob2 == text && t.tr_inlet == inp))
        {
            if (glist_isvisible(x))
            {
                sys_vgui(".x%lx.c delete l%lx\n",
                         glist_getcanvas(x), oc);
            }
            obj_disconnect(t.tr_ob, t.tr_outno, t.tr_ob2, t.tr_inno);
        }
    }
}

void* object_method(void* x, t_symbol* s, void* z, method method, long number, void* other)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s, z, method, number, other);
}
/*
void* object_method(void* x, t_symbol* s, ...)
{
    va_list arguments;
    
    va_start(arguments, s);
    while (*vp)
    {
        vp++;
        count++;
        *vp = va_arg(arguments, t_atomtype);
    }
    va_end(ap);
    
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s, ...);
}*/

void object_attr_setvalueof(t_object *x, t_symbol* s, long argc, t_atom* argv)
{
    method setvalue = (method)getfn((t_pd *)x, s);
    setvalue(x, s, argc, argv);
}

void object_attr_getvalueof(t_object *x, t_symbol *s, long *argc, t_atom **argv)
{
    char realname[256];
	method getvalue;
    sprintf(realname, "get%s", s->s_name);
    getvalue = (method)getfn((t_pd *)x, gensym(realname));
    getvalue(x, s, argc, argv);
}

t_pd_err binbuf_append_atoms(t_binbuf *d, t_symbol *key, long argc, t_atom *argv)
{
    int i;
    for(i = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_SYM && (atom_getsymbol(argv+i) == gensym("") || atom_getsymbol(argv+i) == gensym(" ")))
        {
            atom_setsym(argv+i, gensym("s_nosymbol"));
        }
        
    }
    binbuf_addv(d, "s", key);
    binbuf_addv(d, "s", gensym("["));
    binbuf_add(d, (int)argc, argv);
    binbuf_addv(d, "s", gensym("]"));
    
    return 0;
}

t_pd_err binbuf_copy_atoms(t_binbuf *d, t_symbol *key, long *argc, t_atom **argv)
{
    int i;
    t_atom* av = binbuf_getvec(d);
    long ac = binbuf_getnatom(d);
    int index = 0;
    argc[0] = 0;
    if(ac && av)
    {
        for(i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == key)
            {
                index = i + 2;
            }
        }
    }
    
    if(index)
    {
        i = index;
        while (i < ac && atom_getsym(av+i) != gensym("}") && atom_getsym(av+i) != gensym("]"))
        {
            i++;
            argc[0]++;
        }
        argv[0] = (t_atom *)calloc(argc[0], sizeof(t_atom));
        for (i = 0; i < argc[0]; i++)
        {
            if(atom_gettype(av+i+index) == A_SYM && atom_getsym(av+i+index) == gensym("s_nosymbol"))
            {
                atom_setsym(argv[0]+i, gensym(" "));
            }
            else
            {
                argv[0][i] = av[i+index];
            }
        }
    }
    else
    {
        argc[0] = 0;
        argv = NULL;
    }
    return 0;
}

double pd_clip_min(double aValue, double aMinimum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else
        return aValue;
}

double pd_clip_max(double aValue, double aMaximum)
{
    if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}

double pd_ordinate(const double radius, const double angle)
{
    return radius * sin(angle);
}

double pd_abscissa(const double radius, const double angle)
{
    return radius * cos(angle);
}

double pd_radius(double x, double y)
{
    return sqrt(x*x + y*y);
}

double pd_angle(double x, double y)
{
    return atan2(y, x);
}

double pd_clip_minmax(double aValue, double aMinimum, double aMaximum)
{
    if(aValue < aMinimum)
        return aMinimum;
    else if(aValue > aMaximum)
        return aMaximum;
    else
        return aValue;
}




