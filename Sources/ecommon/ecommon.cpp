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

void outlet_int(void* outlet, int val)
{
    outlet_float((t_outlet *)outlet, val);
}

void outlet_list(void* outlet, t_symbol *s, int argc, t_atom *argv)
{
    outlet_list((t_outlet *)outlet, s, argc, argv);
}

void* listout(void *x)
{
    return outlet_new((t_object *)x, &s_list);
}

void* floatout(void *x)
{
    return outlet_new((t_object *)x, &s_float);
}

void* object_method(void* x, t_symbol* s)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    return nrmethod(x, s);
}

void object_method(void* x, t_symbol* s, t_floatarg a, t_floatarg b,  t_floatarg c, t_floatarg d)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    nrmethod(x, s, a, b, c, d);
}

void object_method(void* x, t_symbol* s, void* z, method method, long number, void* other)
{
    rmethod nrmethod = (rmethod)getfn((t_pd *)x, s);
    nrmethod(x, s, z, method, number, other);
}

void object_attr_setvalueof(t_object *x, t_symbol* s, long argc, t_atom* argv)
{
    method setvalue = (method)getfn((t_pd *)x, s);
    setvalue(x, s, argc, argv);
}

void object_attr_getvalueof(t_object *x, t_symbol *s, long *argc, t_atom **argv)
{
    char realname[256];
    sprintf(realname, "get%s", s->s_name);
    method getvalue = (method)getfn((t_pd *)x, gensym(realname));
    getvalue(x, s, argc, argv);
}

t_pd_err binbuf_append_atoms(t_binbuf *d, t_symbol *key, long argc, t_atom *argv)
{
    for(int i = 0; i < argc; i++)
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
    t_atom* av = binbuf_getvec(d);
    long ac = binbuf_getnatom(d);
    int index = 0;
    argc[0] = 0;
    if(ac && av)
    {
        for(int i = 0; i < ac; i++)
        {
            if(atom_gettype(av+i) == A_SYM && atom_getsym(av+i) == key)
            {
                index = i + 2;
            }
        }
    }
    
    if(index)
    {
        int i = index;
        while (i < ac && atom_getsym(av+i) != gensym("}") && atom_getsym(av+i) != gensym("]"))
        {
            i++;
            argc[0]++;
        }
        argv[0] = new t_atom[argc[0]];
        for (int i = 0; i < argc[0]; i++)
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




