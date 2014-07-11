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

typedef struct  _loadmess
{
	t_eobj      l_box;
	t_outlet*   l_out;
    t_atom*     l_argv;
	long        l_argc;
    char        l_loaded;
    double      l_time;
} t_loadmess;

t_eclass *loadmess_class;

void *loadmess_new(t_symbol *s, int argc, t_atom *argv);
void loadmess_free(t_loadmess *x);
void loadmess_assist(t_loadmess *x, void *b, long m, long a, char *s);

void loadmess_click(t_loadmess *x);
void loadmess_output(t_loadmess *x);
void loadmess_loadbang(t_loadmess *x);

extern "C" void setup_c0x2eloadmess(void)
{
	t_eclass *c;
    
	c = eclass_new("c.loadmess", (method)loadmess_new, (method)loadmess_free, (short)sizeof(t_loadmess), 0L, A_GIMME, 0);
    cream_initclass(c);
    
    eclass_addmethod(c, (method) loadmess_loadbang,    "loadbang",         A_CANT, 0);
    eclass_addmethod(c, (method) loadmess_output,      "bang",             A_CANT, 0);
	eclass_addmethod(c, (method) loadmess_click,       "click",            A_CANT, 0);
    eclass_addmethod(c, (method) loadmess_assist,      "assist",           A_CANT, 0);
	
    eclass_register(CLASS_OBJ, c);
	loadmess_class = c;
}

void *loadmess_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_loadmess *x =  NULL;
	t_binbuf* d;
    
    if (!(d = binbuf_via_atoms(argc,argv)))
        return NULL;
    
    x = (t_loadmess *)eobj_new(loadmess_class);
    x->l_loaded = 0;
    x->l_time = clock_getsystime();
    x->l_argc = argc;
    x->l_argv = (t_atom *)calloc(x->l_argc, sizeof(t_atom));
    for(i = 0; i < argc; i++)
        x->l_argv[i] = argv[i];
    
    if(!x->l_argc)
    {
        x->l_out = (t_outlet *)bangout(x);
    }
    else if(x->l_argc == 1)
    {
        if(atom_gettype(argv) == A_FLOAT)
            x->l_out = (t_outlet *)floatout(x);
        else if (atom_gettype(argv) == A_SYMBOL)
            x->l_out = (t_outlet *)symbolout(x);
    }
    else
    {
        if(atom_gettype(argv) == A_FLOAT)
            x->l_out = (t_outlet *)listout(x);
        else if (atom_gettype(argv) == A_SYMBOL)
            x->l_out = (t_outlet *)anythingout(x);
    }
    
    return (x);
}

void loadmess_loadbang(t_loadmess *x)
{
    if(!x->l_loaded)
        loadmess_output(x);
}

void loadmess_output(t_loadmess *x)
{
    if(!x->l_argc)
    {
        outlet_bang(x->l_out);
    }
    else if(x->l_argc == 1)
    {
        if(atom_gettype(x->l_argv) == A_FLOAT)
            outlet_float(x->l_out, atom_getfloat(x->l_argv));
        else if (atom_gettype(x->l_argv) == A_SYMBOL)
            outlet_symbol(x->l_out, atom_getsymbol(x->l_argv));
    }
    else
    {
        if(atom_gettype(x->l_argv) == A_FLOAT)
            outlet_list(x->l_out, &s_list, x->l_argc, x->l_argv);
        else if (atom_gettype(x->l_argv) == A_SYMBOL)
            outlet_anything(x->l_out, atom_getsym(x->l_argv), x->l_argc-1, x->l_argv+1);
    }
}

void loadmess_click(t_loadmess *x)
{
    if(clock_gettimesince(x->l_time) < 250.)
        loadmess_output(x);
    x->l_time = clock_getsystime();
}

void loadmess_free(t_loadmess *x)
{
	eobj_free(x);
}

void loadmess_assist(t_loadmess *x, void *b, long m, long a, char *s)
{
	;
}


