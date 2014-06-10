/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../../../Sources/ejucebox/ejucebox.h"

typedef struct  _cjuce
{
	t_openglbox l_box;
	t_outlet*   f_out;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_bang;
    t_clock*    f_clock;
    char        f_active;
} t_cjuce;

t_eclass *cjuce_class;

void *cjuce_new(t_symbol *s, int argc, t_atom *argv);
void cjuce_free(t_cjuce *x);
void cjuce_assist(t_cjuce *x, void *b, long m, long a, char *s);
t_pd_err cjuce_notify(t_cjuce *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void cjuce_getdrawparams(t_cjuce *x, t_object *patcherview, t_edrawparams *params);
void cjuce_oksize(t_cjuce *x, t_rect *newrect);

void cjuce_output(t_cjuce *x, t_symbol* s, long argc, t_atom* argv);
void cjuce_paint(t_cjuce *x, Graphics &g);

void cjuce_mousedown(t_cjuce *x, t_object *patcherview, t_pt pt, long modifiers);
void cjuce_mouseup(t_cjuce *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void setup_c0x2ejuce(void)
{
	t_eclass *c;
    
	c = eclass_new("c.juce", (method)cjuce_new, (method)cjuce_free, (short)sizeof(t_cjuce), 0L, A_GIMME, 0);
    
    eopenglbox_initclass(c, 0);

    eclass_addmethod(c, (method) cjuce_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) cjuce_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) cjuce_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_output,          "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) cjuce_output,          "cjuce",             A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_output,          "list",             A_GIMME,0);
    eclass_addmethod(c, (method) cjuce_output,          "anything",         A_GIMME,0);
    
    eclass_addmethod(c, (method) cjuce_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) cjuce_mouseup,         "mouseup",          A_CANT, 0);
    
    CLASS_ATTR_INVISIBLE            (c, "fontname", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontweight", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontslant", 1);
    CLASS_ATTR_INVISIBLE            (c, "fontsize", 1);
	CLASS_ATTR_DEFAULT              (c, "size", 0, "16. 16.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_cjuce, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_cjuce, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "color");
    
	CLASS_ATTR_RGBA                 (c, "bacolor", 0, t_cjuce, f_color_bang);
	CLASS_ATTR_LABEL                (c, "bacolor", 0, "Bang Color");
	CLASS_ATTR_ORDER                (c, "bacolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bacolor", 0, "0. 0. 0. 1.");
	CLASS_ATTR_STYLE                (c, "bacolor", 0, "color");
    
    eclass_register(CLASS_BOX, c);
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
	t_cjuce *x =  NULL;
	t_binbuf* d;
    

    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_cjuce *)eobj_new(cjuce_class);
    flags = 0
    | EBOX_GROWLINK
    ;
    eopenglbox_new((t_openglbox *)x, flags);
    
    x->f_out = (t_outlet *)bangout((t_object *)x);
    x->f_active = 0;
    x->f_clock          = clock_new(x,(t_method)cjuce_mouseup);
	ebox_attrprocess_viabinbuf(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void cjuce_getdrawparams(t_cjuce *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_border;
}

void cjuce_oksize(t_cjuce *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 16.);
    newrect->height = pd_clip_min(newrect->height, 16.);
}

void cjuce_output(t_cjuce *x, t_symbol* s, long argc, t_atom* argv)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_bang(x->f_out);
    if(ebox_getsender((t_ebox *) x))
        pd_bang(ebox_getsender((t_ebox *) x));
    
    clock_delay(x->f_clock, 100);
}

void cjuce_free(t_cjuce *x)
{
	eopenglbox_free((t_openglbox *)x);
    clock_free(x->f_clock);
}

void cjuce_assist(t_cjuce *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err cjuce_notify(t_cjuce *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("bacolor"))
		{
			ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void cjuce_paint(t_cjuce *x, Graphics &g)
{
    t_rect rect;
    ebox_get_rect_for_view((t_ebox *)x, &rect);
    if(x->f_active)
    {
        g.setColour(Colour::fromFloatRGBA(x->f_color_bang.red, x->f_color_bang.green, x->f_color_bang.blue, 1));
    }
    else
    {
        g.setColour(Colour::fromFloatRGBA(x->f_color_background.red, x->f_color_background.green, x->f_color_background.blue, 1));
    }
    g.fillEllipse(0.05 * rect.width, 0.05 * rect.height, rect.width * 0.9, rect.height * 0.9);
}

void cjuce_mousedown(t_cjuce *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 1;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
    outlet_bang(x->f_out);
    if(ebox_getsender((t_ebox *) x))
        pd_bang(ebox_getsender((t_ebox *) x));
}

void cjuce_mouseup(t_cjuce *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_active = 0;
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}


