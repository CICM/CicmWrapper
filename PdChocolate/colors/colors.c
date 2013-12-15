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

typedef struct  _colors
{
	t_ebox      j_box;
    
    t_outlet*   f_out_rgb;
    t_outlet*   f_out_hsl;
    t_outlet*   f_out_hex;
    
    t_hsla**    f_matrix_colors;
    t_pt        f_matrix_sizes;
    long        f_reverse;
    float       f_saturation;
    float       f_hue;
    float       f_lightness;
    
    t_pt        f_color_picked;
    t_pt        f_color_hover;
    
	t_rgba		f_color_background;
	t_rgba		f_color_border;
    
} t_colors;

t_eclass *colors_class;

void colors_setup(void);
void *colors_new(t_symbol *s, int argc, t_atom *argv);
void colors_free(t_colors *x);
void colors_assist(t_colors *x, void *b, long m, long a, char *s);

void colors_set(t_colors *x, t_symbol *s, long ac, t_atom *av);
void colors_list(t_colors *x, t_symbol *s, long ac, t_atom *av);
void colors_output(t_colors *x);

t_pd_err colors_notify(t_colors *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
t_pd_err colors_matrix_set(t_colors *x, t_object *attr, long ac, t_atom *av);
t_pd_err colors_reverse_set(t_colors *x, t_object *attr, long ac, t_atom *av);
t_pd_err colors_saturation_set(t_colors *x, t_object *attr, long ac, t_atom *av);
t_pd_err colors_hue_set(t_colors *x, t_object *attr, long ac, t_atom *av);
t_pd_err colors_lightness_set(t_colors *x, t_object *attr, long ac, t_atom *av);

void colors_getdrawparams(t_colors *x, t_object *patcherview, t_edrawparams *params);
void colors_oksize(t_colors *x, t_rect *newrect);

void colors_paint(t_colors *x, t_object *view);
void draw_background(t_colors *x,  t_object *view, t_rect *rect);
void draw_hover(t_colors *x,  t_object *view, t_rect *rect);
void draw_picked(t_colors *x, t_object *view, t_rect *rect);

void colors_mousemove(t_colors *x, t_object *patcherview, t_pt pt, long modifiers);
void colors_mousedown(t_colors *x, t_object *patcherview, t_pt pt, long modifiers);
void colors_mouseleave(t_colors *x, t_object *patcherview, t_pt pt, long modifiers);
void colors_preset(t_colors *x, t_binbuf *b);

void colors_setup(void)
{
	t_eclass *c;
    
	c = eclass_new("colors", (method)colors_new, (method)colors_free, (short)sizeof(t_colors), 0L, A_GIMME, 0);
    class_addcreator((t_newmethod)colors_new, gensym("swatch"), A_GIMME, 0);
	eclass_init(c, 0);
	
	eclass_addmethod(c, (method) colors_assist,          "assist",           A_CANT, 0);
	eclass_addmethod(c, (method) colors_paint,           "paint",            A_CANT, 0);
	eclass_addmethod(c, (method) colors_notify,          "notify",           A_CANT, 0);
    eclass_addmethod(c, (method) colors_getdrawparams,   "getdrawparams",    A_CANT, 0);
    eclass_addmethod(c, (method) colors_oksize,          "oksize",           A_CANT, 0);
    eclass_addmethod(c, (method) colors_set,             "set",              A_GIMME,0);
    eclass_addmethod(c, (method) colors_list,            "list",             A_GIMME,0);
    eclass_addmethod(c, (method) colors_output,          "bang",             A_CANT, 0);
    
    eclass_addmethod(c, (method) colors_mousemove,       "mousemove",        A_CANT, 0);
    eclass_addmethod(c, (method) colors_mousedown,       "mousedown",        A_CANT, 0);
    eclass_addmethod(c, (method) colors_mousedown,       "mousedrag",        A_CANT, 0);
    eclass_addmethod(c, (method) colors_mouseleave,      "mouseleave",       A_CANT, 0);
    
    eclass_addmethod(c, (method) colors_preset,          "preset",           A_CANT, 0);
    
	CLASS_ATTR_DEFAULT              (c, "size", 0, "181 105");
	
    CLASS_ATTR_FLOAT_ARRAY          (c, "matrix", 0, t_colors, f_matrix_sizes, 2);
	CLASS_ATTR_LABEL                (c, "matrix", 0, "Matrix Size");
    CLASS_ATTR_ACCESSORS			(c, "matrix", NULL, colors_matrix_set);
	CLASS_ATTR_ORDER                (c, "matrix", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "matrix", 0, "24. 13.");
    CLASS_ATTR_SAVE                 (c, "matrix", 0);
    
    CLASS_ATTR_LONG                 (c, "reverse", 0, t_colors, f_reverse);
	CLASS_ATTR_LABEL                (c, "reverse", 0, "Matrix Reversed");
    CLASS_ATTR_ACCESSORS			(c, "reverse", NULL, colors_reverse_set);
	CLASS_ATTR_ORDER                (c, "reverse", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "reverse", 0, "0");
    CLASS_ATTR_SAVE                 (c, "reverse", 0);
    
    CLASS_ATTR_FLOAT                (c, "saturation", 0, t_colors, f_saturation);
	CLASS_ATTR_LABEL                (c, "saturation", 0, "Saturation");
    CLASS_ATTR_ACCESSORS			(c, "saturation", NULL, colors_saturation_set);
	CLASS_ATTR_ORDER                (c, "saturation", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "saturation", 0, "1.");
    CLASS_ATTR_SAVE                 (c, "saturation", 0);
    
    CLASS_ATTR_FLOAT                (c, "hue", 0, t_colors, f_hue);
	CLASS_ATTR_LABEL                (c, "hue", 0, "Fist Hue");
    CLASS_ATTR_ACCESSORS			(c, "hue", NULL, colors_hue_set);
	CLASS_ATTR_ORDER                (c, "hue", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "hue", 0, "0.");
    CLASS_ATTR_SAVE                 (c, "hue", 0);
    
    CLASS_ATTR_FLOAT                (c, "lightness", 0, t_colors, f_lightness);
	CLASS_ATTR_LABEL                (c, "lightness", 0, "First Lightness");
    CLASS_ATTR_ACCESSORS			(c, "lightness", NULL, colors_lightness_set);
	CLASS_ATTR_ORDER                (c, "lightness", 0, "1");
	CLASS_ATTR_DEFAULT              (c, "lightness", 0, "1.");
    CLASS_ATTR_SAVE                 (c, "lightness", 0);
    
    CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_colors, f_color_background);
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_ORDER                (c, "bgcolor", 0, "1");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.75 0.75 0.75 1.");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_colors, f_color_border);
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Box Border Color");
	CLASS_ATTR_ORDER                (c, "bdcolor", 0, "2");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.5 0.5 0.5 1.");
	
    eclass_register(CLASS_NOBOX, c);
	colors_class = c;
}

void *colors_new(t_symbol *s, int argc, t_atom *argv)
{
    int i;
	t_colors *x =  NULL;
	t_binbuf* d;
    long flags;
	if (!(d = binbuf_via_atoms(argc,argv)))
		return NULL;
    
	x = (t_colors *)ebox_alloc(colors_class);
    
    x->f_matrix_colors   = (t_hsla **)malloc(1 * sizeof(t_hsla *));
    for(i = 0; i < 1; i++)
    {
        x->f_matrix_colors[i]   = (t_hsla *)malloc(1 * sizeof(t_hsla));
    }
    x->f_matrix_sizes.x = 1;
    x->f_matrix_sizes.y = 1;
    
    flags = 0
    | EBOX_GROWINDI
    ;
	ebox_new((t_ebox *)x, flags);
	x->j_box.b_firstin = (t_object *)x;
    
    x->f_color_hover.x = -10;
    x->f_color_hover.y = -10;
    x->f_color_picked.x = -10;
    x->f_color_picked.y = -10;
    x->f_out_rgb = (t_outlet *)listout(x);
    x->f_out_hsl = (t_outlet *)listout(x);
    x->f_out_hex = (t_outlet *)outlet_new((t_object *)x, &s_symbol);
    
	binbuf_attr_process(x, d);
	ebox_ready((t_ebox *)x);
	return (x);
}

void colors_getdrawparams(t_colors *x, t_object *patcherview, t_edrawparams *params)
{
	params->d_borderthickness   = 2;
	params->d_cornersize        = 2;
    params->d_bordercolor       = x->f_color_border;
    params->d_boxfillcolor      = x->f_color_background;
}

void colors_oksize(t_colors *x, t_rect *newrect)
{
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 10.);
    
    float ratio = (newrect->width - 1.) / (float)x->f_matrix_sizes.x;
    if(ratio - (int)ratio != 0)
    {
        ratio = round(ratio);
        newrect->width = ratio * (float)x->f_matrix_sizes.x + 1.;
    }
    ratio = (newrect->height - 1.) / (float)x->f_matrix_sizes.y;
    if(ratio - (int)ratio != 0)
    {
        ratio = round(ratio);
        newrect->height = ratio * (float)x->f_matrix_sizes.y + 1.;
    }
    
    newrect->width = pd_clip_min(newrect->width, 30.);
    newrect->height = pd_clip_min(newrect->height, 10.);
}

void colors_set(t_colors *x, t_symbol *s, long ac, t_atom *av)
{
    if(ac > 1 && av)
    {
        if(atom_gettype(av) == A_FLOAT)
            x->f_color_picked.x = pd_clip_minmax((int)atom_getfloat(av), 0, x->f_matrix_sizes.x-1);
        if(atom_gettype(av+1) == A_FLOAT)
            x->f_color_picked.y = pd_clip_minmax((int)atom_getfloat(av+1), 0, x->f_matrix_sizes.y-1);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("picked_layer"));
        ebox_redraw((t_ebox *)x);
    }
}

void colors_list(t_colors *x, t_symbol *s, long ac, t_atom *av)
{
    if(ac > 1 && av)
    {
        if(atom_gettype(av) == A_FLOAT)
            x->f_color_picked.x = pd_clip_minmax((int)atom_getfloat(av), 0, x->f_matrix_sizes.x-1);
        if(atom_gettype(av+1) == A_FLOAT)
            x->f_color_picked.y = pd_clip_minmax((int)atom_getfloat(av+1), 0, x->f_matrix_sizes.y-1);
        
        ebox_invalidate_layer((t_ebox *)x, gensym("picked_layer"));
        ebox_redraw((t_ebox *)x);
        colors_output(x);
    }
}

void colors_output(t_colors *x)
{
    t_rgba color_rgb;
    t_hsla color_hls;
    t_symbol* color_hex;
    t_atom av[3];
    if(x->j_box.e_ready_to_draw && x->f_color_picked.x >= 0 && x->f_color_picked.y >= 0)
    {
        color_hls = x->f_matrix_colors[(int)x->f_color_picked.x][(int)x->f_color_picked.y];
        color_rgb = hsla_to_rgba(color_hls);
        color_hex = gensym(rgba_to_hex(color_rgb));
        atom_setfloat(av, color_rgb.red);
        atom_setfloat(av+1, color_rgb.green);
        atom_setfloat(av+2, color_rgb.blue);
        outlet_list(x->f_out_rgb, &s_list, 3, av);
        atom_setfloat(av, color_hls.hue);
        atom_setfloat(av+1, color_hls.lightness);
        atom_setfloat(av+2, color_hls.saturation);
        outlet_list(x->f_out_hsl, &s_list, 3, av);
        outlet_symbol(x->f_out_hex, color_hex);
    }
}

void colors_free(t_colors *x)
{
    int i;
	ebox_free((t_ebox *)x);
    for(i = 0; i < x->f_matrix_sizes.x; i++)
    {
        free(x->f_matrix_colors[i]);
    }
    free(x->f_matrix_colors);
}

void colors_assist(t_colors *x, void *b, long m, long a, char *s)
{
	;
}

t_pd_err colors_notify(t_colors *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	if (msg == gensym("attr_modified"))
	{
        ebox_redraw((t_ebox *)x);
	}
	return 0;
}

void colors_paint(t_colors *x, t_object *view)
{
	t_rect rect;
	ebox_get_rect_for_view((t_ebox *)x, &rect);
    draw_background(x, view, &rect);
    draw_picked(x, view, &rect);
    draw_hover(x, view, &rect);
}

void draw_background(t_colors *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("background_layer"), rect->width, rect->height);
    int i, j, incx, incY;
    int block_width = rect->width / (float)x->f_matrix_sizes.x;
    int block_height = rect->height / (float)x->f_matrix_sizes.y;
	if (g)
	{
        for(incx = 0, i = 0; i < x->f_matrix_sizes.x; i++, incx += block_width)
        {
            for(incY = 0, j = 0; j < x->f_matrix_sizes.y; j++, incY += block_height)
            {
                egraphics_set_color_hsla(g, &x->f_matrix_colors[i][j]);
                egraphics_rectangle_rounded(g, incx+1, incY+1, block_width-2, block_height-2, 1);
                egraphics_fill(g);
            }
        }
        
        ebox_end_layer((t_ebox*)x, gensym("background_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("background_layer"), 0, 0);
}

void draw_picked(t_colors *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("picked_layer"), rect->width, rect->height);
    int block_width = rect->width / (float)x->f_matrix_sizes.x;
    int block_height = rect->height / (float)x->f_matrix_sizes.y;
    
	if(g)
	{
        if(x->f_color_picked.x >= 0 && x->f_color_picked.y >= 0)
        {
            egraphics_set_color_rgba(g, &x->f_color_border);
            egraphics_set_line_width(g, 2);
            egraphics_rectangle_rounded(g, (int)x->f_color_picked.x * block_width + 1, (int)x->f_color_picked.y * block_height +1, block_width-2, block_height-2, 1);
            egraphics_stroke(g);
        }
        ebox_end_layer((t_ebox*)x, gensym("picked_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("picked_layer"), 0, 0);
}


void draw_hover(t_colors *x, t_object *view, t_rect *rect)
{
	t_elayer *g = ebox_start_layer((t_ebox *)x, gensym("hover_layer"), rect->width, rect->height);
    int block_width = rect->width / (float)x->f_matrix_sizes.x;
    int block_height = rect->height / (float)x->f_matrix_sizes.y;
    
	if(g)
	{
        if(x->f_color_hover.x >= 0 && x->f_color_hover.y >= 0)
        {
            egraphics_set_color_hsla(g, &x->f_matrix_colors[(int)x->f_color_hover.x][(int)x->f_color_hover.y]);
            egraphics_rectangle_rounded(g, (int)x->f_color_hover.x * block_width, (int)x->f_color_hover.y * block_height, block_width, block_height, 1);
            egraphics_fill(g);
        }
        ebox_end_layer((t_ebox*)x, gensym("hover_layer"));
	}
	ebox_paint_layer((t_ebox *)x, gensym("hover_layer"), 0, 0);
}

void colors_mousemove(t_colors *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_color_hover.x = pd_clip_minmax((int)(pt.x / (x->j_box.e_rect.width / (float)x->f_matrix_sizes.x)), 0, x->f_matrix_sizes.x-1);
    x->f_color_hover.y = pd_clip_minmax((int)(pt.y / (x->j_box.e_rect.height / (float)x->f_matrix_sizes.y)), 0, x->f_matrix_sizes.y-1);
    ebox_invalidate_layer((t_ebox *)x, gensym("hover_layer"));
    ebox_redraw((t_ebox *)x);
}
void colors_mousedown(t_colors *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_color_hover.x = -10;
    x->f_color_hover.y = -10;
    x->f_color_picked.x = pd_clip_minmax((int)(pt.x / (x->j_box.e_rect.width / (float)x->f_matrix_sizes.x)), 0, x->f_matrix_sizes.x-1);
    x->f_color_picked.y = pd_clip_minmax((int)(pt.y / (x->j_box.e_rect.height / (float)x->f_matrix_sizes.y)), 0, x->f_matrix_sizes.y-1);
    ebox_invalidate_layer((t_ebox *)x, gensym("hover_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("picked_layer"));
    ebox_redraw((t_ebox *)x);
    colors_output(x);
}

void colors_mouseleave(t_colors *x, t_object *patcherview, t_pt pt, long modifiers)
{
    x->f_color_hover.x = -10;
    x->f_color_hover.y = -10;
    ebox_invalidate_layer((t_ebox *)x, gensym("hover_layer"));
    ebox_redraw((t_ebox *)x);
}

void colors_preset(t_colors *x, t_binbuf *b)
{
   binbuf_addv(b, "sff", gensym("list"), x->f_color_picked.x, x->f_color_picked.y);
}

void colors_computecolors(t_colors *x)
{
    int i, j;
    
    x->f_color_hover.x = -10;
    x->f_color_hover.y = -10;
    
    x->f_color_picked.x = -10;
    x->f_color_picked.y = -10;
    
    t_hsla color_ref;
    hsla_set(&color_ref, x->f_hue, x->f_saturation, x->f_lightness, 1.);
    if(x->f_reverse)
    {
        for(j = 0; j < x->f_matrix_sizes.y; j++)
        {
            color_ref.lightness = x->f_lightness;
            for (i = 0; i < x->f_matrix_sizes.x; i++)
            {
                x->f_matrix_colors[i][j] = color_ref;
                color_ref.lightness -= (1. / (float)(x->f_matrix_sizes.x - 1));
                if(x->f_matrix_colors[i][j].lightness > 1)
                    x->f_matrix_colors[i][j].lightness = 1. - (x->f_matrix_colors[i][j].lightness - 1.);
                else if(x->f_matrix_colors[i][j].lightness < 0.)
                    x->f_matrix_colors[i][j].lightness = -x->f_matrix_colors[i][j].lightness;
            }
            color_ref.hue += (1. / (float)(x->f_matrix_sizes.y));
        }
    }
    else
    {
        for(i = 0; i < x->f_matrix_sizes.x; i++)
        {
            color_ref.lightness = x->f_lightness;
            for (j = 0; j < x->f_matrix_sizes.y; j++)
            {
                x->f_matrix_colors[i][j] = color_ref;
                color_ref.lightness -= (1. / (float)(x->f_matrix_sizes.y - 1));
                if(x->f_matrix_colors[i][j].lightness > 1)
                    x->f_matrix_colors[i][j].lightness = 1. - (x->f_matrix_colors[i][j].lightness - 1.);
                else if(x->f_matrix_colors[i][j].lightness < 0.)
                    x->f_matrix_colors[i][j].lightness = -x->f_matrix_colors[i][j].lightness;
            }
            color_ref.hue += (1. / (float)(x->f_matrix_sizes.x));
        }
    }
    
    ebox_invalidate_layer((t_ebox *)x, gensym("background_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("hover_layer"));
    ebox_invalidate_layer((t_ebox *)x, gensym("picked_layer"));
    ebox_redraw((t_ebox *)x);
}

t_pd_err colors_matrix_set(t_colors *x, t_object *attr, long ac, t_atom *av)
{
    int i;
    if(ac > 1 && av && atom_gettype(av) == A_FLOAT && atom_gettype(av+1) == A_FLOAT)
    {
        for(i = 0; i < x->f_matrix_sizes.x; i++)
        {
            free(x->f_matrix_colors[i]);
        }
        free(x->f_matrix_colors);
        
        x->f_matrix_sizes.x = (int)pd_clip_min(atom_getfloat(av), 1);
        x->f_matrix_sizes.y = (int)pd_clip_min(atom_getfloat(av+1), 1);
        x->f_matrix_colors   = (t_hsla **)malloc(x->f_matrix_sizes.x * sizeof(t_hsla *));
        for(i = 0; i < x->f_matrix_sizes.x; i++)
        {
            x->f_matrix_colors[i]   = (t_hsla *)malloc(x->f_matrix_sizes.y * sizeof(t_hsla));
        }
        colors_computecolors(x);
    }
    return 0;
}

t_pd_err colors_saturation_set(t_colors *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_saturation = pd_clip_minmax(atom_getfloat(av), 0., 1.);
        colors_computecolors(x);
    }
    return 0;
}

t_pd_err colors_hue_set(t_colors *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_hue = pd_clip_minmax(atom_getfloat(av), 0., 1.);
        colors_computecolors(x);
    }
    return 0;
}

t_pd_err colors_lightness_set(t_colors *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_lightness = pd_clip_minmax(atom_getfloat(av), 0., 1.);
        colors_computecolors(x);
    }
    return 0;
}

t_pd_err colors_reverse_set(t_colors *x, t_object *attr, long ac, t_atom *av)
{
    if(ac && av && atom_gettype(av) == A_FLOAT)
    {
        x->f_reverse = pd_clip_minmax((int)atom_getfloat(av), 0., 1.);
        colors_computecolors(x);
    }
    return 0;
}




