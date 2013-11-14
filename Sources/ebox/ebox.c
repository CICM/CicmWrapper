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

#include "ebox.h"

void *ebox_alloc(t_eclass *c)
{
    t_pd *x;
    if(!c)
        bug("pd_new: apparently called before setup routine");
    x = (t_pd *)t_getbytes(c->c_class.c_size);
    *x = (t_pd)c;
    if (c->c_class.c_patchable)
    {
        ((t_object *)x)->ob_inlet = 0;
        ((t_object *)x)->ob_outlet = 0;
    }
    return (x);
}

void ebox_new(t_ebox *x, long flags, long argc, t_atom *argv)
{
    char buffer[MAXPDSTRING];
    
    x->e_flags = flags;
    
    sprintf(buffer,"%s%lx", class_getname(x->e_obj.te_g.g_pd), (long unsigned int)x);
    x->e_name_tcl = gensym(buffer);
    
    sprintf(buffer,"#%s", x->e_name_tcl->s_name);
    x->e_name_rcv = gensym(buffer);
    
    pd_bind(&x->e_obj.ob_pd, x->e_name_rcv);
    x->e_ready_to_draw      = 0;
    x->z_misc               = 1;
    x->e_number_of_layers   = 0;
    x->e_layers             = NULL;
    x->e_deserted_time      = 3000.;
    ebox_tk_ids(x, canvas_getcurrent());
}


void ebox_free(t_ebox* x)
{
    clock_free(x->e_deserted_clock);
    pd_unbind(&x->e_obj.ob_pd, x->e_name_rcv);
}

char ebox_getregister(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    return c->c_box;
}

void ebox_dspsetup(t_ebox *x, long nins, long nout)
{
    int i;
    nins = pd_clip_min(nins, 1);
    nout = pd_clip_min(nout, 0);
    x->e_perform_method = NULL;

    for( i = 0; i < 256; i++)
        x->z_sigs_out[i] = (t_float *)calloc(8192, sizeof(t_float));
    for(i = 1; i < nins; i++)
        x->e_inlets[i] = signalinlet_new(&x->e_obj, x->e_float);
    for(i = 0; i < nout; i++)
        x->e_outlets[i] = outlet_new(&x->e_obj, &s_signal);
    
    x->e_dsp_size = obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj) + 6;
}


void ebox_ready(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    
    ebox_get_mouse_global_position(x);
    
    x->e_selected_item  = EITEM_NONE;
    x->e_selected_inlet = -1;
    x->e_selected_outlet= -1;
    x->e_mouse_down     = 0;
    
    x->e_boxparameters.d_bordercolor = rgba_black;
    x->e_boxparameters.d_borderthickness = 1;
    x->e_boxparameters.d_boxfillcolor = rgba_white;
    x->e_boxparameters.d_cornersize = 0;
    c->c_widget.w_getdrawparameters(x, NULL, &x->e_boxparameters);
    x->e_deserted_clock = clock_new(x, (t_method)ebox_deserted);
    x->e_ready_to_draw = 1;
}

void ebox_dspfree(t_ebox *x)
{
    int i;
    for(i = 0; i < 256; i++)
        free(x->z_sigs_out[i]);
    
    if(!ebox_getregister(x))
        ebox_free(x);
}

void ebox_redraw(t_ebox *x)
{
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(x->e_ready_to_draw && glist_isvisible(x->e_canvas))
    {
        if(c->c_box == 0)
        {
            ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("eboxbd"));
            ebox_invalidate_layer((t_object *)x, (t_object *)x->e_canvas, gensym("eboxio"));
            ebox_update(x, x->e_canvas);
            if(c->c_widget.w_paint)
                c->c_widget.w_paint(x, (t_object *)x->e_canvas);
            ebox_draw_border(x, x->e_canvas);
            ebox_draw_iolets(x, x->e_canvas);
        }
    }
}


void ebox_resize_inputs(t_ebox *x, long nins)
{
	int i = 0;
    nins = pd_clip_min(nins, 1);
    
    if(nins > obj_nsiginlets(&x->e_obj))
    {
        for(i = obj_nsiginlets(&x->e_obj); i < nins; i++)
        {
            x->e_inlets[i] = signalinlet_new(&x->e_obj, x->e_float);
        }
    }
    else if(nins < obj_nsiginlets(&x->e_obj))
    {
        for(i = obj_nsiginlets(&x->e_obj) - 1; i >= nins - 1; i--)
        {
            canvas_deletelines_for_io(x->e_canvas, (t_text *)x, x->e_inlets[i], NULL);
            inlet_free(x->e_inlets[i]);
        }
    }
    x->e_dsp_size = obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj) + 6;
    ebox_redraw(x);
}

void ebox_resize_outputs(t_ebox *x, long nouts)
{
    int i;
    t_eclass *c = (t_eclass *)x->e_obj.te_g.g_pd;
    nouts       = pd_clip_min(nouts, 0);
    
    if(!c->c_box)
    {
        if(nouts > obj_nsigoutlets(&x->e_obj))
        {
            for(i = obj_nsigoutlets(&x->e_obj); i < nouts; i++)
            {
                x->e_outlets[i] = outlet_new(&x->e_obj, &s_signal);
            }
        }
        else if (nouts < obj_nsigoutlets(&x->e_obj))
        {
            for(i = obj_nsigoutlets(&x->e_obj)-1; i >= nouts; i--)
            {
                canvas_deletelines_for_io(x->e_canvas, (t_text *)x, NULL, x->e_outlets[i]);
                outlet_free(x->e_outlets[i]);
            }
        }    
    }
    else
    {
        if(nouts > obj_nsigoutlets(&x->e_obj))
        {
            for (i = obj_nsigoutlets(&x->e_obj); i < nouts; i++)
            {
                if(!x->e_canvas->gl_loading && glist_isvisible(x->e_canvas))
                    gobj_vis((t_gobj *)x, x->e_canvas, 0);
                x->e_outlets[i] = outlet_new(&x->e_obj, &s_signal);
                if(!x->e_canvas->gl_loading && glist_isvisible(x->e_canvas))
                    gobj_vis((t_gobj *)x, x->e_canvas, 1);
            }
        }
        else if (nouts < obj_nsigoutlets(&x->e_obj))
        {
            for(i = obj_nsigoutlets(&x->e_obj) - 1; i >= nouts; i--)
            {
                if(!x->e_canvas->gl_loading && glist_isvisible(x->e_canvas))
                {
                    gobj_vis((t_gobj *)x, x->e_canvas, 0);
                    canvas_deletelines_for_io(x->e_canvas, (t_text *)x, NULL, x->e_outlets[i]);
                }
                outlet_free(x->e_outlets[i]);
                if(!x->e_canvas->gl_loading && glist_isvisible(x->e_canvas))
                    gobj_vis((t_gobj *)x, x->e_canvas, 1);
            }
        }
    }

    x->e_dsp_size = obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj) + 6;
    ebox_redraw(x);
}

void ebox_dsp(t_ebox *x, t_signal **sp)
{
    int i;
	//t_linetraverser t;
    //t_outconnect *oc = NULL;
    t_eclass *c  = (t_eclass *)x->e_obj.te_g.g_pd;
    short* count = (short*)calloc((obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj)), sizeof(short));
    /*
    for(i = 0; i < obj_nsiginlets(&x->e_obj); i++)
    {
        count[i] = 0;
        linetraverser_start(&t, x->e_canvas);
        while((oc = linetraverser_next(&t)))
        {
            if (t.tr_ob2 == (t_object*)x && t.tr_inno == i)
            {
                count[i] = 1;
            }
        }
    }
    
    for(i = obj_nsiginlets(&x->e_obj); i < (obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj)); i++)
    {
        count[i] = 0;
        linetraverser_start(&t, x->e_canvas);
        while((oc = linetraverser_next(&t)))
        {
            if(t.tr_ob == (t_object*)x && t.tr_outno == i)
            {
                count[i] = 1;
            }
        }
    }*/
    if(c->c_widget.w_dsp != NULL)
        c->c_widget.w_dsp(x, x, &count, sp[0]->s_sr, sp[0]->s_n, 0);
    
    x->e_dsp_vectors[0] = (t_int)x;
    x->e_dsp_vectors[1] = (t_int)sp[0]->s_n;
    x->e_dsp_vectors[2] = (t_int)x->e_dsp_flag;
    x->e_dsp_vectors[3] = (t_int)x->e_dsp_user_param;
    x->e_dsp_vectors[4] = (t_int)obj_nsiginlets(&x->e_obj);
    x->e_dsp_vectors[5] = (t_int)obj_nsigoutlets(&x->e_obj);
    
    for(i = 6; i < x->e_dsp_size; i++)
    {
        x->e_dsp_vectors[i] = (t_int)(sp[i - 6]->s_vec);
    }
    
    dsp_addv(ebox_perform, (int)x->e_dsp_size, x->e_dsp_vectors);
	free(count);
}

t_int* ebox_perform(t_int* w)
{
	int i, j;
    t_ebox* x               = (t_ebox *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    if(x->z_misc == E_NO_INPLACE)
    {
        t_float *outs_real, *outs_perf;
        if(x->e_perform_method != NULL)
            x->e_perform_method(x, NULL, ins, nins, x->z_sigs_out, nouts, nsamples, flag, user_p);
        for(i = 0; i < nouts; i++)
        {
            outs_perf = x->z_sigs_out[i];
            outs_real = outs[i];
            for(j = 0; j < nsamples; j++)
            {
                outs_real[j] = outs_perf[j];
            }
        }
    }
    else
    {
        if(x->e_perform_method != NULL)
            x->e_perform_method(x, NULL, ins, nins, outs, nouts, nsamples, flag, user_p);
    }
    return w + (x->e_dsp_size + 1);
}


void ebox_dsp_add(t_ebox *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam)
{
    x->e_dsp_flag = flags;
    x->e_dsp_user_param = userparam;
    x->e_perform_method = m;
}

void ebox_get_rect_for_view(t_object *z, t_object *patcherview, t_rect *rect)
{
    t_ebox* x = (t_ebox *)z;
    rect->x = x->e_rect.x = x->e_obj.te_xpix;
    rect->y = x->e_rect.y = x->e_obj.te_ypix;
    rect->width = x->e_rect.width;
    rect->height = x->e_rect.height;
}

void ebox_properties(t_gobj *z, t_glist *glist)
{
    ;
}

t_pd_err ebox_notify(t_ebox *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	int i;
    t_eclass* c = (t_eclass *)x->e_obj.te_g.g_pd;
    if(s == gensym("patching_rect") || s == gensym("size"))
    {
        if(c->c_widget.w_oksize != NULL)
            c->c_widget.w_oksize(x, &x->e_rect);
        for(i = 0; i < x->e_number_of_layers; i++)
        {
            x->e_layers[i].e_state = EGRAPHICS_INVALID;
        }
        ebox_redraw(x);
    }
    return 0;
}

t_binbuf* binbuf_via_atoms(long ac, t_atom *av)
{
    t_binbuf* dico = binbuf_new();
    binbuf_add(dico, (int)ac, av);
    return dico;
}

void binbuf_attr_process(void *x, t_binbuf *d)
{
    int i, j;
    char attr_name[256];
    long defc       = 0;
    t_atom* defv    = NULL;
    t_ebox* z       = (t_ebox *)x;
    t_eclass* c     = (t_eclass *)z->e_obj.te_g.g_pd;

    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i].defvals)
        {
            defc = c->c_attr[i].size;
            defv = (t_atom *)calloc(defc, sizeof(t_atom));
            if(defc && defv)
            {
                char* str_start = c->c_attr[i].defvals->s_name;
                for(j = 0; j < defc; j++)
                {
                    if(isalpha(str_start[0]))
                    {
                        atom_setsym(defv+j, gensym(str_start));
                    }
                    else
                    {
                        float val = (float)strtod(str_start, &str_start);
                        atom_setfloat(defv+j, val);
                    }
                }
                object_attr_setvalueof((t_object *)x, c->c_attr[i].name, defc, defv);
                defc = 0;
                free(defv);
                defv = NULL;
            }
        }
    }
    
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i].name->s_name);
        binbuf_copy_atoms(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i].name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

t_symbol* ebox_get_fontname(t_ebox* x)
{
    return x->e_font.c_family;
}

t_symbol* ebox_font_slant(t_ebox* x)
{
    return x->e_font.c_slant;
}

t_symbol* ebox_font_weight(t_ebox* x)
{
    return x->e_font.c_weight;
}

float ebox_font_size(t_ebox* x)
{
    return x->e_font.c_size;
}
