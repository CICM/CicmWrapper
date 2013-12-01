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

extern "C"
{
#include "../../../PdEnhanced/Sources/pd_enhanced.h"
}

typedef struct _incdec
{
	t_ebox      j_box;
    
    t_outlet*   f_out;
    float       f_increment;
    float       f_value;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_arrow;
    int         f_mouse_down;
    char        f_setted;
    
} t_incdec;

t_eclass *incdec_class;

void incdec_tilde_setup(void);
void *incdec_new(t_symbol *s, int argc, t_atom *argv);
void incdec_free(t_incdec *x);
void incdec_assist(t_incdec *x, void *b, long m, long a, char *s);

void incdec_set(t_incdec *x, float f);
void incdec_float(t_incdec *x, float f);
void incdec_output(t_incdec *x);
void incdec_inc(t_incdec *x);
void incdec_dec(t_incdec *x);

t_pd_err incdec_notify(t_incdec *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void incdec_getdrawparams(t_incdec *x, t_object *patcherview, t_edrawparams *params);
void incdec_oksize(t_incdec *x, t_rect *newrect);

void incdec_paint(t_incdec *x, t_object *view);
void draw_background(t_incdec *x,  t_object *view, t_rect *rect);

void incdec_mousedown(t_incdec *x, t_object *patcherview, t_pt pt, long modifiers);
void incdec_mouseup(t_incdec *x, t_object *patcherview, t_pt pt, long modifiers);

extern "C" void incdec_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("incdec", (method)incdec_new, (method)incdec_free, (short)sizeof(t_incdec), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) incdec_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) incdec_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) incdec_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) incdec_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) incdec_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) incdec_set,             "set",              A_FLOAT,0);
    eclass_addmethod(c, (method) incdec_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) incdec_output,          "bang",             A_CANT, 0);
    eclass_addmethod(c, (method) incdec_inc,             "inc",              A_CANT, 0);
    eclass_addmethod(c, (method) incdec_dec,             "dec",              A_CANT, 0);
    eclass_addmethod(c, (method) incdec_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) incdec_mouseup,         "mouseup",          A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "13 20");
	
    CLASS_ATTR_FLOAT                (c, "step", 0, t_incdec, f_increment);
	CLASS_ATTR_LABEL                (c, "step", 0, "Step increment");
    CLASS_ATTR_FILTER_MIN           (c, "step", 0.);
	CLASS_ATTR_ORDER                (c, "step", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "step", 0, "1.");
    
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_incdec, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.35 0.23 0.13 1.");
	
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_incdec, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.27 0.21 0. 1");
	
	CLASS_ATTR_RGBA                 (c, "arcolor", 0, t_incdec, f_color_arrow);
	CLASS_ATTR_LABEL                (c, "arcolor", 0, "Point Color");
	CLASS_ATTR_ORDER                (c, "arcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "arcolor", 0, "0.94 0.85 0.57 1");
	
	
    eclass_register(CLASS_NOBOX, c);
	incdec_class = c;
}

void *incdec_new(t_symbol *s, int argc, t_atom *argv)
{
	t_incdec *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_incdec *)ebox_alloc(incdec_class);
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    x->f_value = 0.;
    x->f_mouse_down = 0;
    x->f_out = (t_outlet *)floatout(x);
	binbuf_attr_process(x, d);
    x->f_setted = 0;
	ebox_ready((t_ebox *)x);
	return (x);
}

void incdec_getdrawparams(t_incdec *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void incdec_oksize(t_incdec *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
}

void incdec_set(t_incdec *x, float f)
{
    x->f_setted = 1;
    x->f_value = f;
}

void incdec_float(t_incdec *x, float f)
{
    if(x->f_value != f || x->f_setted == 1)
    {
        x->f_value = f;
        incdec_output(x);
    }
}

void incdec_output(t_incdec *x)
{
    x->f_setted = 0;
    outlet_float((t_outlet*)x->f_out, (float)x->f_value);
}

void incdec_inc(t_incdec *x)
{
    x->f_value += x->f_increment;
    incdec_output(x);
}

void incdec_dec(t_incdec *x)
{
    x->f_value -= x->f_increment;
    incdec_output(x);
}

void incdec_free(t_incdec *x)
{
	ebox_free((t_ebox *)x);
}

void incdec_assist(t_incdec *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err incdec_notify(t_incdec *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("arcolor"))
		{
			ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void incdec_paint(t_incdec *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_object *)x, view, &rect);
    draw_background(x, view, &rect);
}

void draw_background(t_incdec *x, t_object *view, t_rect *rect)
{
    float height;
	t_elayer *g = ebox_start_layer((t_object *)x, view, gensym("background_layer"), rect->width, rect->height);
    
	if (g)
	{
        // Background //
        egraphics_set_color_rgba(g, &x->f_color_arrow);
        if(x->f_mouse_down == 1)
            egraphics_rectangle(g, 0, 0, rect->width, rect->height / 2.);
        else if(x->f_mouse_down == -1)
            egraphics_rectangle(g, 0, rect->height / 2., rect->width, rect->height);
        egraphics_fill(g);
        
        
        egraphics_set_color_rgba(g, &x->f_color_arrow);
        
        // Arrow Up //
        if(x->f_mouse_down == 1)
            egraphics_set_color_rgba(g, &x->f_color_background);
        else
            egraphics_set_color_rgba(g, &x->f_color_arrow);
        
        if((rect->height / 2.) * 0.1 < 2)
            height = (rect->height / 2.) - 2.;
        else
            height = (rect->height / 2.) * 0.9;
        
        egraphics_move_to(g, rect->width * 0.1, height);
        egraphics_line_to(g, rect->width * 0.9, height);
        egraphics_line_to(g, rect->width * 0.5, (rect->height / 2.) * 0.1);
        egraphics_fill(g);
        
        // Arrow Down //
        if(x->f_mouse_down == -1)
            egraphics_set_color_rgba(g, &x->f_color_background);
        else
            egraphics_set_color_rgba(g, &x->f_color_arrow);
        
        if((rect->height / 2.) * 0.1 < 2)
            height = (rect->height / 2.) + 2.;
        else
            height = (rect->height / 2.) * 1.1;
        
        egraphics_move_to(g, rect->width * 0.1, height);
        egraphics_line_to(g, rect->width * 0.9, height);
        egraphics_line_to(g, rect->width * 0.5, (rect->height / 2.) * 1.9);
        egraphics_fill(g);
        
        // Middle Line //
        egraphics_set_color_rgba(g, &x->f_color_border);
        egraphics_set_line_width(g, 2);
        egraphics_line_fast(g, 0., rect->height / 2., rect->width, rect->height / 2.);
        
        ebox_end_layer((t_object*)x, view, gensym("background_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("background_layer"), 0., 0.);
}

void incdec_mousedown(t_incdec *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(pt.y < x->j_box.e_rect.height / 2.)
    {
        incdec_inc(x);
        x->f_mouse_down = 1;
    }
    else
    {
        incdec_dec(x);
        x->f_mouse_down = -1;
    }
    ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}


void incdec_mouseup(t_incdec *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_mouse_down = 0;
    ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
    ebox_redraw((t_ebox *)x);
}







