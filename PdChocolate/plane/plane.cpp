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

typedef struct  _plane
{
	t_ebox      j_box;
    
    t_outlet*   f_out_x;
    t_outlet*   f_out_y;
    
    t_pt        f_point;
	t_rgba		f_color_background;
	t_rgba		f_color_border;
	t_rgba		f_color_point;
    
} t_plane;

t_eclass *plane_class;

extern "C" void plane_tilde_setup(void);
void *plane_new(t_symbol *s, int argc, t_atom *argv);
void plane_free(t_plane *x);
void plane_assist(t_plane *x, void *b, long m, long a, char *s);

void plane_float(t_plane *x, t_floatarg f);
void plane_set(t_plane *x, t_floatarg f);
void plane_output(t_plane *x);
t_pd_err plane_notify(t_plane *x, t_symbol *s, t_symbol *msg, void *sender, void *data);

void plane_getdrawparams(t_plane *x, t_object *patcherview, t_edrawparams *params);
void plane_oksize(t_plane *x, t_rect *newrect);

void plane_paint(t_plane *x, t_object *view);
void draw_point(t_plane *x,  t_object *view, t_rect *rect);

void plane_mousedown(t_plane *x, t_object *patcherview, t_pt pt, long modifiers);
void plane_mousedrag(t_plane *x, t_object *patcherview, t_pt pt, long modifiers);
void plane_mousemove(t_plane *x, t_object *patcherview, t_pt pt, long modifiers);

void plane_key(t_plane *x, t_object *patcherview, char textcharacter, long modifiers);
void plane_keyfilter(t_plane *x, t_object *patcherview, char textcharacter, long modifiers);
void plane_deserted(t_plane *x, t_object *patcherview);

extern "C" void plane_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("plane", (method)plane_new, (method)plane_free, (short)sizeof(t_plane), 0L, A_GIMME, 0);
    
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) plane_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) plane_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) plane_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) plane_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) plane_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) plane_float,           "float",            A_FLOAT,0);
    eclass_addmethod(c, (method) plane_set,             "set",              A_FLOAT,0);
    eclass_addmethod(c, (method) plane_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) plane_mousedown,        "mousedown",       A_CANT, 0);
    eclass_addmethod(c, (method) plane_mousedrag,        "mousedrag",       A_CANT, 0);
    eclass_addmethod(c, (method) plane_mousemove,        "mousemove",       A_CANT, 0);
    
	CLASS_ATTR_DEFAULT			(c, "size", 0, "55 55");
	
    CLASS_ATTR_FLOAT_ARRAY      (c, "point", 0, t_plane, f_point, 2);
	CLASS_ATTR_LABEL			(c, "point", 0, "Point position");
	CLASS_ATTR_ORDER			(c, "point", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "point", 0, "0. 0.");
    
	CLASS_ATTR_RGBA				(c, "bgcolor", 0, t_plane, f_color_background);
	CLASS_ATTR_LABEL			(c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER			(c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bgcolor", 0, "0.35 0.23 0.13 1.");
	
	CLASS_ATTR_RGBA				(c, "bdcolor", 0, t_plane, f_color_border);
	CLASS_ATTR_LABEL			(c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER			(c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "bdcolor", 0, "0.27 0.21 0. 1");
	
	CLASS_ATTR_RGBA				(c, "ptcolor", 0, t_plane, f_color_point);
	CLASS_ATTR_LABEL			(c, "ptcolor", 0, "Text Color");
	CLASS_ATTR_ORDER			(c, "ptcolor", 0, "3");
	CLASS_ATTR_DEFAULT_SAVE_PAINT(c, "ptcolor", 0, "1. 1. 1. 1.");
	
	
    eclass_register(CLASS_NOBOX, c);
	plane_class = c;
    
    post("PdChocolate by Pierre Guillot - CICM | Université Paris 8");
    post("PdChocolate has been elaborated with the PdEnhanced Library");
}

void *plane_new(t_symbol *s, int argc, t_atom *argv)
{
	t_plane *x =  NULL;
	t_binbuf* d;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_plane *)ebox_alloc(plane_class);
    
	ebox_new((t_ebox *)x, 0, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
    
    floatinlet_new((t_object *)x, &x->f_point.y);
    x->f_out_x = (t_outlet *)floatout(x);
    x->f_out_y = (t_outlet *)floatout(x);
	attr_binbuf_process(x, d);
    
	ebox_ready((t_ebox *)x);
	return (x);
}

void plane_getdrawparams(t_plane *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void plane_oksize(t_plane *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 15.);
    newrect->height = pd_clip_min(newrect->height, 15.);
    
    if(newrect->height > newrect->width)
        newrect->width = newrect->height;
    else
        newrect->height = newrect->width;
}

void plane_float(t_plane *x, t_floatarg f)
{
    x->f_point.x = f;
    plane_output(x);
    ebox_invalidate_layer((t_object *)x, NULL, gensym("point_layer"));
    ebox_redraw((t_ebox *)x);
}

void plane_set(t_plane *x, t_floatarg f)
{
    ebox_invalidate_layer((t_object *)x, NULL, gensym("point_layer"));
    ebox_redraw((t_ebox *)x);
}

void plane_output(t_plane *x)
{
    outlet_float((t_outlet*)x->f_out_x, (float)x->f_point.x);
    outlet_float((t_outlet*)x->f_out_x, (float)x->f_point.y);
}

void plane_free(t_plane *x)
{
	ebox_free((t_ebox *)x);
}

void plane_assist(t_plane *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err plane_notify(t_plane *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
		if(s == gensym("bgcolor") || s == gensym("bdcolor") || s == gensym("ptcolor") || s == gensym("point"))
		{
			ebox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			ebox_invalidate_layer((t_object *)x, NULL, gensym("point_layer"));
		}
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void plane_paint(t_plane *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_object *)x, view, &rect);
    draw_point(x, view, &rect);
}

void draw_point(t_plane *x, t_object *view, t_rect *rect)
{
    /*
	t_elayer *g = ebox_start_layer((t_object *)x, view, gensym("point_layer"), rect->width, rect->height);
    
	if (g)
	{
        ebox_end_layer((t_object*)x, view, gensym("point_layer"));
	}
	ebox_paint_layer((t_object *)x, view, gensym("point_layer"), 0., 0.);
     */
}

void plane_mousedown(t_plane *x, t_object *patcherview, t_pt pt, long modifiers)
{
    if(pt.x)
    {
        ;
    }
}

void plane_mousedrag(t_plane *x, t_object *patcherview, t_pt pt, long modifiers)
{
    ;
}

void plane_mousemove(t_plane *x, t_object *patcherview, t_pt pt, long modifiers)
{
    canvas_setcursor(glist_getcanvas((t_glist *)patcherview), 1);
}








