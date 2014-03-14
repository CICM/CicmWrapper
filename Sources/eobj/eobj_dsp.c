/*
 * CicmWrapper
 *
 * A wrapper for Pure Data
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

#include "eobj.h"

//! Initialize an edspobj
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @param nins  The number of signal inputs
 \ @param nouts The number of signal outputs
 \ @return      Nothing
*/
void eobj_dspsetup(void *x, long nins, long nouts)
{
    int i;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    nins = pd_clip_min(nins, 1);
    nouts = pd_clip_min(nouts, 0);
    if(eobj_isbox(x))
    {
        box->d_perform_method = NULL;
        box->d_misc           = 1;
        box->d_sigs_real      = (t_float *)calloc(256 * 8192, sizeof(t_float));
        for( i = 0; i < 256; i++)
            box->d_sigs_out[i] = box->d_sigs_real+i*8192;
        for(i = 1; i < nins; i++)
            box->d_inlets[i] = signalinlet_new(&box->b_obj.o_obj, box->d_float);
        for(i = 0; i < nouts; i++)
            box->d_outlets[i] = outlet_new(&box->b_obj.o_obj, &s_signal);
        
        box->d_dsp_size = obj_nsiginlets(&box->b_obj.o_obj) + obj_nsigoutlets(&box->b_obj.o_obj) + 6;
    }
    else
    {
        obj->d_perform_method = NULL;
        obj->d_misc           = 1;
        obj->d_sigs_real      = (t_float *)calloc(256 * 8192, sizeof(t_float));
        for( i = 0; i < 256; i++)
            obj->d_sigs_out[i] = obj->d_sigs_real+i*8192;
        for(i = 1; i < nins; i++)
            obj->d_inlets[i] = signalinlet_new(&obj->d_obj.o_obj, obj->d_float);
        for(i = 0; i < nouts; i++)
            obj->d_outlets[i] = outlet_new(&obj->d_obj.o_obj, &s_signal);
        
        obj->d_dsp_size = obj_nsiginlets(&obj->d_obj.o_obj) + obj_nsigoutlets(&obj->d_obj.o_obj) + 6;
    }
    
}

//! Free an edspobj (note that if the object is also an UI, you don't need to call ebox_free())
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @return      Nothing
*/
void eobj_dspfree(void *x)
{
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    if(eobj_isbox(x))
    {
        free(box->d_sigs_real);
    }
    else
    {
        free(obj->d_sigs_real);
    }
}

//! Resize the number of signal inputs of an edspobj
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @param nins  The number of signal inputs
 \ @return      Nothing
*/
void eobj_resize_inputs(void *x, long nins)
{
	int i;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    nins = pd_clip_min(nins, 1);
    
    if(eobj_isbox(x))
    {
        if(nins > obj_nsiginlets(&box->b_obj.o_obj))
        {
            for(i = obj_nsiginlets(&box->b_obj.o_obj); i < nins; i++)
            {
                box->d_inlets[i] = signalinlet_new(&box->b_obj.o_obj, box->d_float);
            }
        }
        else if(nins < obj_nsiginlets(&box->b_obj.o_obj))
        {
            for(i = obj_nsiginlets(&box->b_obj.o_obj) - 1; i >= nins; i--)
            {
                canvas_deletelines_for_io(box->b_obj.o_canvas, (t_text *)x, box->d_inlets[i], NULL);
                inlet_free(box->d_inlets[i]);
            }
        }
        box->d_dsp_size = obj_nsiginlets(&box->b_obj.o_obj) + obj_nsigoutlets(&box->b_obj.o_obj) + 6;
        canvas_fixlinesfor(box->b_obj.o_canvas, (t_text *)x);
    }
    else
    {
        if(nins > obj_nsiginlets(&obj->d_obj.o_obj))
        {
            for(i = obj_nsiginlets(&obj->d_obj.o_obj); i < nins; i++)
            {
                obj->d_inlets[i] = signalinlet_new(&obj->d_obj.o_obj, obj->d_float);
            }
        }
        else if(nins < obj_nsiginlets(&obj->d_obj.o_obj))
        {
            for(i = obj_nsiginlets(&obj->d_obj.o_obj) - 1; i >= nins; i--)
            {
                canvas_deletelines_for_io(obj->d_obj.o_canvas, (t_text *)x, obj->d_inlets[i], NULL);
                inlet_free(obj->d_inlets[i]);
            }
        }
        obj->d_dsp_size = obj_nsiginlets(&obj->d_obj.o_obj) + obj_nsigoutlets(&obj->d_obj.o_obj) + 6;
        canvas_fixlinesfor(obj->d_obj.o_canvas, (t_text *)x);
    }
}

//! Resize the number of signal outputs of an edspobj
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @param nouts The number of signal outputs
 \ @return      Nothing
*/
void eobj_resize_outputs(void *x, long nouts)
{
    int i;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    nouts       = pd_clip_min(nouts, 0);

    if(eobj_isbox(x))
    {
        if(nouts > obj_nsigoutlets(&box->b_obj.o_obj))
        {
            for(i = obj_nsigoutlets(&box->b_obj.o_obj); i < nouts; i++)
            {
                box->d_outlets[i] = outlet_new(&box->b_obj.o_obj, &s_signal);
            }
        }
        else if (nouts < obj_nsigoutlets(&box->b_obj.o_obj))
        {
            for(i = obj_nsigoutlets(&box->b_obj.o_obj)-1; i >= nouts; i--)
            {
                canvas_deletelines_for_io(box->b_obj.o_canvas, (t_text *)x, NULL, box->d_outlets[i]);
                outlet_free(box->d_outlets[i]);
            }
        }
        box->d_dsp_size = obj_nsiginlets(&box->b_obj.o_obj) + obj_nsigoutlets(&box->b_obj.o_obj) + 6;
        canvas_fixlinesfor(box->b_obj.o_canvas, (t_text *)x);
    }
    else
    {
        if(nouts > obj_nsigoutlets(&obj->d_obj.o_obj))
        {
            for (i = obj_nsigoutlets(&obj->d_obj.o_obj); i < nouts; i++)
            {
                if(!obj->d_obj.o_canvas->gl_loading && glist_isvisible(obj->d_obj.o_canvas))
                    gobj_vis((t_gobj *)x, obj->d_obj.o_canvas, 0);
                obj->d_outlets[i] = outlet_new(&obj->d_obj.o_obj, &s_signal);
                if(!obj->d_obj.o_canvas->gl_loading && glist_isvisible(obj->d_obj.o_canvas))
                    gobj_vis((t_gobj *)x, obj->d_obj.o_canvas, 1);
            }
        }
        else if (nouts < obj_nsigoutlets(&obj->d_obj.o_obj))
        {
            for(i = obj_nsigoutlets(&obj->d_obj.o_obj) - 1; i >= nouts; i--)
            {
                if(!obj->d_obj.o_canvas->gl_loading && glist_isvisible(obj->d_obj.o_canvas))
                {
                    gobj_vis((t_gobj *)x, obj->d_obj.o_canvas, 0);
                    canvas_deletelines_for_io(obj->d_obj.o_canvas, (t_text *)x, NULL, obj->d_outlets[i]);
                }
                outlet_free(obj->d_outlets[i]);
                if(!obj->d_obj.o_canvas->gl_loading && glist_isvisible(obj->d_obj.o_canvas))
                    gobj_vis((t_gobj *)x, obj->d_obj.o_canvas, 1);
            }
        }
        obj->d_dsp_size = obj_nsiginlets(&obj->d_obj.o_obj) + obj_nsigoutlets(&obj->d_obj.o_obj) + 6;
        canvas_fixlinesfor(obj->d_obj.o_canvas, (t_text *)x);
    }
    
}

//! @cond
//! The dsp method (PRIVATE)
/*
 \ @memberof    edspobj
 \ @param x     The edspobj pointer
 \ @param sp    The pointers to signal structures
 \ @return      Nothing
*/
void eobj_dsp(void *x, t_signal **sp)
{
    int i;
    short* count;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    t_eclass *c    = eobj_getclass(x);
    if(eobj_isbox(x))
    {
        count = (short*)calloc((obj_nsiginlets(&box->b_obj.o_obj) + obj_nsigoutlets(&box->b_obj.o_obj)), sizeof(short));
        
        if(c->c_widget.w_dsp != NULL)
            c->c_widget.w_dsp(x, x, &count, sp[0]->s_sr, sp[0]->s_n, 0);
        
        box->d_dsp_vectors[0] = (t_int)x;
        box->d_dsp_vectors[1] = (t_int)sp[0]->s_n;
        box->d_dsp_vectors[2] = (t_int)box->d_dsp_flag;
        box->d_dsp_vectors[3] = (t_int)box->d_dsp_user_param;
        box->d_dsp_vectors[4] = (t_int)obj_nsiginlets(&box->b_obj.o_obj);
        box->d_dsp_vectors[5] = (t_int)obj_nsigoutlets(&box->b_obj.o_obj);
        
        for(i = 6; i < box->d_dsp_size; i++)
        {
            box->d_dsp_vectors[i] = (t_int)(sp[i - 6]->s_vec);
        }
        
        dsp_addv(eobj_perform_box, (int)box->d_dsp_size, box->d_dsp_vectors);
    }
    else
    {
        count = (short*)calloc((obj_nsiginlets(&obj->d_obj.o_obj) + obj_nsigoutlets(&obj->d_obj.o_obj)), sizeof(short));
        
        if(c->c_widget.w_dsp != NULL)
            c->c_widget.w_dsp(x, x, &count, sp[0]->s_sr, sp[0]->s_n, 0);
        
        obj->d_dsp_vectors[0] = (t_int)x;
        obj->d_dsp_vectors[1] = (t_int)sp[0]->s_n;
        obj->d_dsp_vectors[2] = (t_int)obj->d_dsp_flag;
        obj->d_dsp_vectors[3] = (t_int)obj->d_dsp_user_param;
        obj->d_dsp_vectors[4] = (t_int)obj_nsiginlets(&obj->d_obj.o_obj);
        obj->d_dsp_vectors[5] = (t_int)obj_nsigoutlets(&obj->d_obj.o_obj);
        
        for(i = 6; i < obj->d_dsp_size; i++)
        {
            obj->d_dsp_vectors[i] = (t_int)(sp[i - 6]->s_vec);
        }
        
        dsp_addv(eobj_perform, (int)obj->d_dsp_size, obj->d_dsp_vectors);
    }
    
	free(count);
}

//! The perform method (PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
*/
t_int* eobj_perform(t_int* w)
{
	int i, j;
    t_edspobj* x            = (t_edspobj *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    if(x->d_misc == E_NO_INPLACE)
    {
        t_float *outs_real, *outs_perf;
        if(x->d_perform_method != NULL)
            x->d_perform_method(x, NULL, ins, nins, x->d_sigs_out, nouts, nsamples, flag, user_p);
        for(i = 0; i < nouts; i++)
        {
            outs_perf = x->d_sigs_out[i];
            outs_real = outs[i];
            for(j = 0; j < nsamples; j++)
            {
                outs_real[j] = outs_perf[j];
            }
        }
    }
    else
    {
        if(x->d_perform_method != NULL)
            x->d_perform_method(x, NULL, ins, nins, outs, nouts, nsamples, flag, user_p);
    }
    return w + (x->d_dsp_size + 1);
}

//! The perform method for box (PRIVATE)
/*
 \ @memberof    edspobj
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
 */
t_int* eobj_perform_box(t_int* w)
{
	int i, j;
    t_edspbox* x            = (t_edspbox *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    if(x->d_misc == E_NO_INPLACE)
    {
        t_float *outs_real, *outs_perf;
        if(x->d_perform_method != NULL)
            x->d_perform_method(x, NULL, ins, nins, x->d_sigs_out, nouts, nsamples, flag, user_p);
        for(i = 0; i < nouts; i++)
        {
            outs_perf = x->d_sigs_out[i];
            outs_real = outs[i];
            for(j = 0; j < nsamples; j++)
            {
                outs_real[j] = outs_perf[j];
            }
        }
    }
    else
    {
        if(x->d_perform_method != NULL)
            x->d_perform_method(x, NULL, ins, nins, outs, nouts, nsamples, flag, user_p);
    }
    return w + (x->d_dsp_size + 1);
}

//! The dsp add method (PRIVATE)
/*
 \ @memberof        edspobj
 \ @param x         The edspobj
 \ @param s         Nothing (for Max 6 compatibility)
 \ @param obj       Nothing (for Max 6 compatibility)
 \ @param m         The user perform method
 \ @param flags     The user perform flags
 \ @param userparam The user perform parameters
 \ @return          Nothing
*/
void eobj_dsp_add(void *x, t_symbol* s, t_object* za, method m, long flags, void *userparam)
{
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    if(eobj_isbox(x))
    {
        box->d_dsp_flag = flags;
        box->d_dsp_user_param = userparam;
        box->d_perform_method = m;
    }
    else
    {
        obj->d_dsp_flag = flags;
        obj->d_dsp_user_param = userparam;
        obj->d_perform_method = m;
    }
}

void eobj_getconnections(void* x, short* count)
{
    ;
}

//! @endcond











