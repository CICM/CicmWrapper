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

#include "../c.library.h"

typedef struct  _pak
{
	t_eobj      j_box;
	t_outlet*   f_out;
    t_atom*     f_argv;
	long        f_argc;
    char*       f_selectors;
} t_pak;

t_eclass *pak_class;

void *pak_new(t_symbol *s, int argc, t_atom *argv);
void pak_anything(t_pak *x, t_symbol *s, int argc, t_atom *argv);
void pak_list(t_pak *x, t_symbol *s, int argc, t_atom *argv);
void pak_float(t_pak *x, float f);
void pak_symbol(t_pak *x, t_symbol *s);
void pak_free(t_pak *x);
void pak_assist(t_pak *x, void *b, long m, long a, char *s);

void pak_output(t_pak *x);

extern "C" void setup_c0x2epak(void)
{
	t_eclass *c;
    
	c = eclass_new("c.pak", (method)pak_new, (method)pak_free, (short)sizeof(t_pak), CLASS_NOINLET, A_GIMME, 0);
    cream_initclass(c);
    
    eclass_addmethod(c, (method)pak_anything,    "anything",       A_GIMME, 0);
    eclass_addmethod(c, (method)pak_list,        "list",           A_GIMME, 0);
    eclass_addmethod(c, (method)pak_float,       "float",          A_FLOAT, 0);
    eclass_addmethod(c, (method)pak_symbol,      "symbol",         A_SYMBOL,0);
    eclass_addmethod(c, (method)pak_output,      "bang",           A_CANT,  0);
    
    eclass_register(CLASS_OBJ, c);
	pak_class = c;
}

void *pak_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_pak *x =  NULL;
    
    x = (t_pak *)eobj_new(pak_class);
    if(x)
    {
        if(argc < 2)
        {
            argc = 2;
        }
        x->f_argc = argc;
        x->f_argv = (t_atom *)calloc(x->f_argc, sizeof(t_atom));
        x->f_selectors = (char *)calloc(x->f_argc, sizeof(char));
        eobj_proxynew(x);
        eobj_proxynew(x);
        if(argc > 0 &&atom_gettype(argv) == A_SYM && (atom_getsym(argv) == gensym("f") || atom_getsym(argv) == gensym("float")))
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, 0.);
        }
        else if(argc > 0 && atom_gettype(argv) == A_FLOAT)
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, atom_getfloat(argv));
        }
        else if(argc > 0 && atom_gettype(argv) == A_SYM && (atom_getsym(argv) == gensym("s") || atom_getsym(argv) == gensym("symbol")))
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, gensym("symbol"));
        }
        else if(argc > 0 && atom_gettype(argv) == A_SYM)
        {
            x->f_selectors[0] = 1;
            atom_setsym(x->f_argv, atom_getsym(argv));
        }
        else
        {
            x->f_selectors[0] = 0;
            atom_setfloat(x->f_argv, 0.);
        }
        if(argc > 1 && atom_gettype(argv+1) == A_SYM && (atom_getsym(argv+1) == gensym("f") || atom_getsym(argv+1) == gensym("float")))
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, 0.);
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_FLOAT)
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, atom_getfloat(argv+1));
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYM && (atom_getsym(argv+1) == gensym("s") || atom_getsym(argv+1) == gensym("symbol")))
        {
            x->f_selectors[1] = 1;
            atom_setsym(x->f_argv+1, gensym("symbol"));
        }
        else if(argc > 1 && atom_gettype(argv+1) == A_SYM)
        {
            x->f_selectors[1] = 1;
            atom_setsym(x->f_argv+1, atom_getsym(argv+1));
        }
        else
        {
            x->f_selectors[1] = 0;
            atom_setfloat(x->f_argv+1, 0.);
        }
        for(i = 2; i < x->f_argc; i++)
        {
            eobj_proxynew(x);
            if(atom_gettype(argv+i) == A_SYM && (atom_getsym(argv+i) == gensym("f") || atom_getsym(argv+i) == gensym("float")))
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, 0.);
            }
            else if(atom_gettype(argv+i) == A_FLOAT)
            {
                x->f_selectors[i] = 0;
                atom_setfloat(x->f_argv+i, atom_getfloat(argv));
            }
            else if(atom_gettype(argv+i) == A_SYM && (atom_getsym(argv+i) == gensym("s") || atom_getsym(argv+i) == gensym("symbol")))
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
            else if(atom_gettype(argv+i) == A_SYM)
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, atom_getsym(argv+i));
            }
            else
            {
                x->f_selectors[i] = 1;
                atom_setsym(x->f_argv+i, gensym("symbol"));
            }
        }
        x->f_out = (t_outlet *)listout(x);
    }
    
    return (x);
}

void pak_list(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(argc && x->f_selectors[index] == 0 && atom_gettype(argv) == A_FLOAT)
    {
        atom_setfloat(x->f_argv+index, atom_getfloat(argv));
        pak_output(x);
    }
    else if(argc && x->f_selectors[index] == 1 && atom_gettype(argv) == A_SYM)
    {
        atom_setsym(x->f_argv+index, atom_getsym(argv));
        pak_output(x);
    }
}

void pak_float(t_pak *x, float f)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 0)
    {
        atom_setfloat(x->f_argv+index, f);
        pak_output(x);
    }
}

void pak_anything(t_pak *x, t_symbol *s, int argc, t_atom *argv)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

void pak_symbol(t_pak *x, t_symbol *s)
{
    int index = eobj_getproxy((t_ebox *)x);
    if(x->f_selectors[index] == 1)
    {
        atom_setsym(x->f_argv+index, s);
        pak_output(x);
    }
}

void pak_output(t_pak *x)
{
    outlet_list(x->f_out, &s_list, x->f_argc, x->f_argv);
}

void pak_free(t_pak *x)
{
    eobj_free(x);
	free(x->f_selectors);
    free(x->f_argv);
}

void pak_assist(t_pak *x, void *b, long m, long a, char *s)
{
	;
}


