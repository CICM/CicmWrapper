/**
 HoaLibrary : A High Order Ambisonics Library
 Copyright (c) 2012-2013 Julien Colafrancesco, Pierre Guillot, Eliott Paris, CICM, Universite Paris-8.
 All rights reserved.
 
 Website  : http://www.mshparisnord.fr/hoalibrary/
 Contacts : cicm.mshparisnord@gmail.com

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 - Redistributions may not be sold, nor may they be used in a commercial product or activity.
 - Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 - Neither the name of the CICM nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIALL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../../../PdEnhanced/Sources/pd_enhanced.h"

typedef struct  _loadmess
{
	t_ebox      l_box;
	t_outlet*   l_out;
    t_atom*     l_argv;
	long        l_argc;
    char        l_loaded;
    double      l_time;
} t_loadmess;

t_eclass *loadmess_class;

void loadmess_setup(void);
void *loadmess_new(t_symbol *s, int argc, t_atom *argv);
void loadmess_free(t_loadmess *x);
void loadmess_assist(t_loadmess *x, void *b, long m, long a, char *s);

void loadmess_click(t_loadmess *x);
void loadmess_output(t_loadmess *x);
void loadmess_loadbang(t_loadmess *x);

void loadmess_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("loadmess", (method)loadmess_new, (method)loadmess_free, (short)sizeof(t_loadmess), 0L, A_GIMME, 0);

    eclass_addmethod(c, (method) loadmess_loadbang,    "loadbang",         A_CANT, 0);
    eclass_addmethod(c, (method) loadmess_output,      "bang",             A_CANT, 0);
	eclass_addmethod(c, (method) loadmess_click,       "click",            A_CANT, 0);
    eclass_addmethod(c, (method) loadmess_assist,      "assist",           A_CANT, 0);
	
    eclass_register(CLASS_BOX, c);
	loadmess_class = c;
}

void *loadmess_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_loadmess *x =  NULL;
	t_binbuf* d;
    
    if (!(d = binbuf_via_atoms(argc,argv)))
        return NULL;
    
    x = (t_loadmess *)ebox_alloc(loadmess_class);
    x->l_loaded = 0;
    x->l_time = clock_getsystime();
    x->l_argc = argc;
    x->l_argv = calloc(x->l_argc, sizeof(t_atom));
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
            x->l_out = (t_outlet *)listout(x);
    }
    else
    {
        x->l_out = (t_outlet *)listout(x);
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
        outlet_list(x->l_out, &s_list, x->l_argc, x->l_argv);
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
	;
}

void loadmess_assist(t_loadmess *x, void *b, long m, long a, char *s)
{
	;
}


