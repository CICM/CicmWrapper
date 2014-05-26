/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

extern "C" {
#include "../../../Sources/cicm_wrapper.h"
    
}
#include "../../../Sources/juceBox/jucebox_wrapper.h"

class CicmComponent : public Component
{
    
private:
    
public:
    CicmComponent(){};
    ~CicmComponent(){};
    
    void paint (Graphics& g)
    {
        post("hi paint");
        g.setColour(Colours::darkgrey);
        g.fillAll();
    }
};

typedef struct  _cjuce
{
	t_jucebox   l_box;
	t_outlet*   l_out;
    t_atom*     l_argv;
	long        l_argc;
    char        l_loaded;
    double      l_time;
} t_cjuce;

t_eclass *cjuce_class;

juce::MessageManager *MessManage;
void *cjuce_new(t_symbol *s, int argc, t_atom *argv);
void cjuce_free(t_cjuce *x);
void cjuce_assist(t_cjuce *x, void *b, long m, long a, char *s);

void cjuce_click(t_cjuce *x);
void cjuce_output(t_cjuce *x);
void cjuce_loadbang(t_cjuce *x);
void cjuce_float(t_cjuce *x, float f);
void cjuce_paint(t_cjuce *x, double w, double h);
void cjuce_anything(t_cjuce *x, t_symbol *s, int argc, t_atom *argv);

extern "C" void setup_c0x2ejuce(void)
{
	t_eclass *c;
    
	c = eclass_new("c.juce", (method)cjuce_new, (method)cjuce_free, (short)sizeof(t_cjuce), 0L, A_GIMME, 0);
    
    jucebox_initclass(c, NULL, 0);
    eclass_addmethod(c, (method) cjuce_loadbang,    "loadbang",         A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_output,      "bang",             A_CANT, 0);
	eclass_addmethod(c, (method) cjuce_click,       "click",            A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_assist,      "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) cjuce_float,       "float",             A_FLOAT, 0);
    eclass_addmethod(c, (method) cjuce_anything,    "anything",          A_GIMME, 0);
    
    eclass_register(CLASS_OBJ, c);
	cjuce_class = c;
}

void cjuce_anything(t_cjuce *x, t_symbol *s, int argc, t_atom *argv)
{
    post(s->s_name);
}

void cjuce_paint(t_cjuce *x, double w, double h)
{
    
}

void *cjuce_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_cjuce *x =  NULL;
	t_binbuf* d;
    
    if (!(d = binbuf_via_atoms(argc,argv)))
        return NULL;
    
    x = (t_cjuce *)eobj_new(cjuce_class);
    jucebox_new((t_jucebox *)x);
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

void cjuce_loadbang(t_cjuce *x)
{
    if(!x->l_loaded)
        cjuce_output(x);
}

void cjuce_float(t_cjuce *x, float f)
{
    jucebox_setsize((t_jucebox *)x);    
}

void cjuce_output(t_cjuce *x)
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

void cjuce_click(t_cjuce *x)
{
    if(clock_gettimesince(x->l_time) < 250.)
        cjuce_output(x);
    x->l_time = clock_getsystime();
}

void cjuce_free(t_cjuce *x)
{
	eobj_free(x);
    jucebox_free((t_jucebox *)x);
}

void cjuce_assist(t_cjuce *x, void *b, long m, long a, char *s)
{
	;
}

