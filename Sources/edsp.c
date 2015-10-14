/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "edsp.h"
#include "eobj.h"

#include "m_imp.h"

struct _edsp
{
    t_object            d_object;           /*!< The object. */
    t_object*           d_owner;            /*!< The owner of the dsp manager. */
    size_t              d_size;             /*!< The number of signal inlets and outlets. */
    t_int*              d_vectors;          /*!< The vector that contains all the pointers for the perform method. */
    long                d_flags;            /*!< The flags to initialize the perform method. */
    void*               d_user_param;       /*!< The user parameters to pass through the perform method. */
    t_float**           d_sigs_out;         /*!< The array of signal vectors. */
    t_float*            d_sigs_real;        /*!< The real array of signal. */
    t_perform_method    d_method;           /*!< The user perform method. */
    long                d_misc;             /*!< The flag that could be inplace or not. */
};

static t_int* dsp_perform_inplace(t_int* w)
{
    t_object* x             = (t_object *)(w[1]);
    t_edsp* dsp             = (t_edsp *)(w[2]);
    long nsamples           = (long)(w[3]);
    long flag               = (long)(w[4]);
    void* user_p            = (void *)(w[5]);
    long nins               = (long)(w[6]);
    long nouts              = (long)(w[7]);
    t_sample** ins           = (t_sample **)(&w[8]);
    t_sample** outs          = (t_sample **)(&w[8 + nins]);
    
    dsp->d_method(x, (t_object *)dsp, ins, nins, outs, nouts, nsamples, flag, user_p);
    
    return w + (dsp->d_size + 1);
}

static t_int* dsp_perform_noinplace(t_int* w)
{
    int i;
    t_object* x             = (t_object *)(w[1]);
    t_edsp* dsp             = (t_edsp *)(w[2]);
    long nsamples           = (long)(w[3]);
    long flag               = (long)(w[4]);
    void* user_p            = (void *)(w[5]);
    long nins               = (long)(w[6]);
    long nouts              = (long)(w[7]);
    t_sample** ins           = (t_sample **)(&w[8]);
    t_sample** outs          = (t_sample **)(&w[8 + nins]);
    
    for(i = 0; i < nouts; i++)
    {
        memcpy(outs[i], dsp->d_sigs_out[i], (size_t)nsamples * sizeof(t_float));
    }
    
    dsp->d_method((t_object *)x, (t_object *)dsp, ins, nins, outs, nouts, nsamples, flag, user_p);
    
    return w + (dsp->d_size + 1);
}

static void edsp_flags(t_edsp *dsp, long flags)
{
    dsp->d_flags = flags;
}

static void edsp_prepare(t_edsp *dsp, t_signal **sp)
{
    short* count;
    t_int* temp;
    t_float **tempout, *tempreal;
    t_linetraverser t;
    t_outconnect    *oc;
    size_t i, samplesize;
    size_t nouts = (size_t)obj_nsigoutlets((t_object *)dsp->d_owner);
    size_t nins  = (size_t)obj_nsiginlets((t_object *)dsp->d_owner);
    t_dsp_method prepare = (t_dsp_method)zgetfn((t_pd *)dsp->d_owner, gensym("dodsp"));
    if(prepare && (nins || nouts) && sp && sp[0])
    {
        samplesize = (size_t)sp[0]->s_n;
        dsp->d_method = NULL;
        if(dsp->d_misc == EPD_NOINPLACE)
        {
            if(dsp->d_sigs_out)
            {
                tempout = (t_float **)realloc(dsp->d_sigs_out, (size_t)nouts * sizeof(t_float *));
            }
            else
            {
                tempout = (t_float **)malloc((size_t)nouts * sizeof(t_float *));
            }
            if(!tempout)
            {
                if(dsp->d_sigs_out)
                {
                    free(dsp->d_sigs_out);
                    dsp->d_sigs_out = NULL;
                }
                pd_error(dsp, "can't allocate memory for no inplace processing.");
                return;
            }
            dsp->d_sigs_out = tempout;
            
            if(dsp->d_sigs_real)
            {
                tempreal = (t_float *)realloc(dsp->d_sigs_real, (size_t)(nouts * samplesize) * sizeof(t_float));
            }
            else
            {
                tempreal = (t_float *)malloc((size_t)(nouts * samplesize) * sizeof(t_float));
            }
            if(!tempreal)
            {
                if(dsp->d_sigs_real)
                {
                    free(dsp->d_sigs_real);
                    dsp->d_sigs_real = NULL;
                }
                free(dsp->d_sigs_out);
                dsp->d_sigs_out = NULL;
                pd_error(dsp, "can't allocate memory for no inplace processing.");
                return;
            }
            dsp->d_sigs_real = tempreal;
            for(i = 0; i < nouts; i++)
            {
                dsp->d_sigs_out[i] = dsp->d_sigs_real+i*samplesize;
            }
        }
        if(dsp->d_vectors)
        {
            temp = (t_int *)realloc(dsp->d_vectors, (size_t)(nins + nouts + 7) * sizeof(t_int));
        }
        else
        {
            temp = (t_int *)malloc((size_t)(nins + nouts + 7) * sizeof(t_int));
        }
        if(!temp)
        {
            if(dsp->d_vectors)
            {
                free(dsp->d_vectors);
                dsp->d_vectors = NULL;
            }
            free(dsp->d_sigs_real);
            dsp->d_sigs_real = NULL;
            free(dsp->d_sigs_out);
            dsp->d_sigs_out = NULL;
            dsp->d_size = 0;
            pd_error(dsp, "can't allocate memory for dsp vector.");
            return;
        }
        dsp->d_vectors = temp;
        dsp->d_size = nins + nouts + 7;
        count = (short*)malloc((size_t)(nins + nouts) * sizeof(short));
        if(count)
        {
            for(i = 0; i < (nins + nouts); i++)
            {
                count[i] = 0;
            }
            linetraverser_start(&t, eobj_getcanvas(dsp->d_owner));
            while((oc = linetraverser_next(&t)))
            {
                if(t.tr_ob2 == dsp->d_owner && obj_issignaloutlet(t.tr_ob, t.tr_outno))
                {
                    count[t.tr_inno] = 1;
                }
            }
            
            dsp->d_vectors[0] = (t_int)dsp->d_owner;
            dsp->d_vectors[1] = (t_int)dsp;
            dsp->d_vectors[2] = (t_int)sp[0]->s_n;
            dsp->d_vectors[3] = (t_int)dsp->d_flags;
            dsp->d_vectors[4] = (t_int)dsp->d_user_param;
            dsp->d_vectors[5] = (t_int)nins;
            dsp->d_vectors[6] = (t_int)nouts;
            
            for(i = 7; i < dsp->d_size; i++)
            {
                if(sp[i - 7] && sp[i - 7]->s_vec)
                {
                    dsp->d_vectors[i] = (t_int)(sp[i - 7]->s_vec);
                }
                else
                {
                    free(count);
                    free(dsp->d_vectors);
                    dsp->d_vectors = NULL;
                    free(dsp->d_sigs_real);
                    dsp->d_sigs_real = NULL;
                    free(dsp->d_sigs_out);
                    dsp->d_sigs_out = NULL;
                    dsp->d_size = 0;
                    pd_error(dsp, "one of the signal isn't allocated.");
                    return;
                }
            }
            
            (prepare)(dsp->d_owner, (t_object *)dsp, count, sp[0]->s_sr, sp[0]->s_n, 0);
            if(dsp->d_method != NULL && dsp->d_misc == EPD_INPLACE)
            {
                dsp_addv(dsp_perform_inplace, (int)dsp->d_size, dsp->d_vectors);
            }
            else if(dsp->d_method != NULL && dsp->d_misc == EPD_NOINPLACE)
            {
                dsp_addv(dsp_perform_noinplace, (int)dsp->d_size, dsp->d_vectors);
            }
            
            free(count);
            return;
        }
        else
        {
            free(dsp->d_vectors);
            dsp->d_vectors = NULL;
            free(dsp->d_sigs_real);
            dsp->d_sigs_real = NULL;
            free(dsp->d_sigs_out);
            dsp->d_sigs_out = NULL;
            dsp->d_size = 0;
            pd_error(dsp, "can't allocate memory for dsp chain counter.");
        }
    }
}

static void edsp_add(t_edsp *dsp, t_object* x, t_perform_method m, long flags, void *params)
{
    dsp->d_flags = flags;
    dsp->d_user_param = params;
    dsp->d_method = m;
}

static t_sample* edsp_getinsamples(t_edsp *dsp, size_t index)
{
    if(index < (size_t)obj_nsiginlets(dsp->d_owner) && dsp->d_vectors )
    {
        return (t_sample *)dsp->d_vectors[7 + index];
    }
    return NULL;
}

static t_sample* edsp_getoutsamples(t_edsp *dsp, size_t index)
{
    if(index < (size_t)obj_nsigoutlets(dsp->d_owner) && dsp->d_vectors)
    {
        if(dsp->d_misc == EPD_NOINPLACE)
            return dsp->d_sigs_out[index];
        else
            return (t_sample *)dsp->d_vectors[index + 7 + (size_t)obj_nsiginlets(dsp->d_owner)];
    }
    return NULL;
}

static void edsp_free(t_edsp *dsp)
{
    if(dsp->d_sigs_out)
        free(dsp->d_sigs_out);
    if(dsp->d_sigs_real)
        free(dsp->d_sigs_real);
    if(dsp->d_vectors)
        free(dsp->d_vectors);
}


static t_class* edsp_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("edsp1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("edsp"), (t_newmethod)NULL, (t_method)edsp_free, sizeof(t_edsp), CLASS_PD, A_NULL, 0);
        if(c)
        {
            class_addmethod(c, (t_method)edsp_flags,        gensym("flags"),        A_CANT);
            class_addmethod(c, (t_method)edsp_prepare,      gensym("prepare"),      A_CANT);
            class_addmethod(c, (t_method)edsp_add,          gensym("dsp_add"),      A_CANT);
            class_addmethod(c, (t_method)edsp_add,          gensym("dsp_add64"),    A_CANT);
            class_addmethod(c, (t_method)edsp_getinsamples, gensym("insamples"),    A_CANT);
            class_addmethod(c, (t_method)edsp_getoutsamples,gensym("outsamples"),   A_CANT);
            obj = pd_new(c);
            pd_bind(obj, gensym("edsp1572"));
        }
        else
        {
            error("can't initialize flags class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

t_edsp* edsp_new(t_object* owner)
{
    t_edsp*  x = NULL;
    t_class* c = edsp_setup();
    if(c)
    {
        x = (t_edsp *)pd_new(c);
        if(x)
        {
            x->d_owner          = owner;
            x->d_sigs_out       = NULL;
            x->d_sigs_real      = NULL;
            x->d_method         = NULL;
            x->d_size           = 0;
            x->d_vectors        = NULL;
            x->d_misc           = EPD_INPLACE;
        }
        else
        {
            pd_error(owner, "can't allocate dsp manager.");
        }
    }
    return x;
}

t_edsp* edsp_findbyname(t_symbol* name)
{
    t_class* c = edsp_setup();
    if(c)
    {
        return (t_edsp *)pd_findbyclass(name, c);
    }
    return NULL;
}






