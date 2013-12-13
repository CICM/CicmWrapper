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

//! Initialize an ebox for DSP
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param nins  The number of signal inputs
 \ @param nouts The number of signal outputs
 \ @return      Nothing
*/
void ebox_dspsetup(t_ebox *x, long nins, long nouts)
{
    int i;
    nins = pd_clip_min(nins, 1);
    nouts = pd_clip_min(nouts, 0);
    x->e_perform_method = NULL;
    
    for( i = 0; i < 256; i++)
        x->z_sigs_out[i] = (t_float *)calloc(8192, sizeof(t_float));
    for(i = 1; i < nins; i++)
        x->e_inlets[i] = signalinlet_new(&x->e_obj, x->e_float);
    for(i = 0; i < nouts; i++)
        x->e_outlets[i] = outlet_new(&x->e_obj, &s_signal);
    
    x->e_dsp_size = obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj) + 6;
}

//! Free a dsp ebox (note that if the object is also an UI, you don't need to call ebox_free())
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @return      Nothing
*/
void ebox_dspfree(t_ebox *x)
{
    int i;
    for(i = 0; i < 256; i++)
        free(x->z_sigs_out[i]);
    
    if(!ebox_getregister(x))
        ebox_free(x);
}

//! Resize the number of signal inputs of an ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param nins  The number of signal inputs
 \ @return      Nothing
*/
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

//! Resize the number of signal outputs of an ebox
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param nouts The number of signal outputs
 \ @return      Nothing
*/
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

//! The dsp method (PRIVATE)
/*
 \ @memberof    ebox
 \ @param x     The ebox pointer
 \ @param sp    The pointers to signal structures
 \ @return      Nothing
*/
void ebox_dsp(t_ebox *x, t_signal **sp)
{
    int i;
    t_eclass *c  = (t_eclass *)x->e_obj.te_g.g_pd;
    short* count = (short*)calloc((obj_nsiginlets(&x->e_obj) + obj_nsigoutlets(&x->e_obj)), sizeof(short));

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

//! The perform method (PRIVATE)
/*
 \ @memberof    ebox
 \ @param w     The pointer sent by the dsp method
 \ @return      Nothing
*/
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

//! The dsp add method (PRIVATE)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param s         Nothing (for Max 6 compatibility)
 \ @param obj       Nothing (for Max 6 compatibility)
 \ @param m         The user perform method
 \ @param flags     The user perform flags
 \ @param userparam The user perform parameters
 \ @return          Nothing
*/
void ebox_dsp_add(t_ebox *x, t_symbol* s, t_object* obj, method m, long flags, void *userparam)
{
    x->e_dsp_flag = flags;
    x->e_dsp_user_param = userparam;
    x->e_perform_method = m;
}

//! The function should return the inlets and outlets dsp connections (PRIVATE AND NOT READY)
/*
 \ @memberof        ebox
 \ @param x         The ebox
 \ @param count     The vector that will contain the connection informations
 \ @return          Nothing
*/
void ebox_getconnections(t_ebox* x, short* count)
{
    t_gobj*y;
    t_object* obj;
    t_object* dest;
    int obj_nout;
    t_outlet* out;
    t_inlet * in;
    t_outconnect* conn;
    
    for(y = x->e_canvas->gl_owner->gl_list; y; y = y->g_next) /* traverse all objects in canvas */
    {
        obj         = (t_object *)y;
        obj_nout    =  obj_noutlets(obj);
        int nout=0;
        int sourcewhich=0;
        
        for(nout=0; nout<obj_nout; nout++) /* traverse all outlets of each object */
        {
            out     = NULL;
            in      = NULL;
            dest    = NULL;
            conn    = obj_starttraverseoutlet(obj, &out, nout);
            
            while(conn)
            { /* traverse all connections from each outlet */
                int which;
                conn = obj_nexttraverseoutlet(conn, &dest, &in, &which);
                if(dest == &x->e_obj)
                {
                    //int connid = glist_getindex(x->e_canvas->gl_owner, (t_gobj*)obj);
                    //("inlet from %d:%d to my:%d", 0, sourcewhich, which);
                }
            }
            sourcewhich++;
        }
    }
}












