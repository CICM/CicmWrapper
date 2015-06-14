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
#include "ecommon.h"

static t_eproxy* eproxy_new(void *owner, t_symbol* s);
static void eproxy_free(void *owner, t_eproxy* proxy);

void *eobj_new(t_eclass *c)
{
    t_eobj* x = NULL;
    char buffer[MAXPDSTRING];
    if(c)
    {
        x  = (t_eobj *)t_getbytes(c->c_class.c_size);
        if(x)
        {
            *((t_pd *)x)  = (t_pd)c;
            
            if(c->c_class.c_patchable)
            {
                ((t_object *)x)->ob_inlet = 0;
                ((t_object *)x)->ob_outlet = 0;
            }
            
            x->o_nproxy = 0;
            x->o_proxy = NULL;
            x->o_canvas = canvas_getcurrent();
            sprintf(buffer,"#%s%lx", c->c_class.c_name->s_name, (long unsigned int)x);
            x->o_id = gensym(buffer);
            pd_bind(&x->o_obj.ob_pd, x->o_id);
            sprintf(buffer,".x%lx.c", (long unsigned int)x->o_canvas);
            c->c_widget.w_dosave = (method)eobj_dosave;
        }
        else
        {
            bug("pd_new: object %s allocation failed", c->c_class.c_name->s_name);
        }
    }
    else
    {
        bug("pd_new: apparently called before setup routine");
    }
    
    return (x);
}

void eobj_free(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    if(z->o_id)
    {
        pd_unbind((t_pd *)x, z->o_id);
        z->o_id = NULL;
    }
    if(z->o_proxy && z->o_nproxy)
    {
        freebytes(z->o_proxy, (size_t)z->o_nproxy * sizeof(t_eproxy *));
        z->o_proxy = NULL;
        z->o_nproxy= 0;
    }
}

t_eproxy* eobj_proxynew(void* x)
{
    return eproxy_new(x, NULL);
}

int eobj_getproxy(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return z->o_current_proxy;
}

t_eclass* eobj_getclass(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return (t_eclass *)z->o_obj.te_g.g_pd;
}

t_symbol* eobj_getclassname(void* x)
{
    t_eobj *z = (t_eobj *)x;
    return z->o_obj.te_g.g_pd->c_name;
}

t_canvas* eobj_getcanvas(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    return z->o_canvas;
}

char eobj_isbox(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    t_eclass*   c = eobj_getclass(z);
    return c->c_box;
}

char eobj_isdsp(void *x)
{
    t_eobj*     z = (t_eobj *)x;
    t_eclass*   c = eobj_getclass(z);
    if(c->c_widget.w_dsp)
        return 1;
    else
        return 0;
}

void eobj_popup(t_eobj* x, t_symbol* s, float itemid)
{
    t_eclass* c = eobj_getclass(x);
    if(c->c_widget.w_popup)
    {
        c->c_widget.w_popup(x, s, (long)itemid);
    }
}

void eobj_dosave(t_eobj* x, t_binbuf *b)
{
    t_binbuf* d;
    t_eclass* c = eobj_getclass(x);
    if(c && b)
    {
        binbuf_addv(b, "ssii", &s__X, s_obj, (t_int)x->o_obj.te_xpix, (t_int)x->o_obj.te_ypix);
        d = x->o_obj.te_binbuf;
        if(d)
        {
            binbuf_addbinbuf(b, d);
        }
        if(c->c_widget.w_save != NULL)
        {
            c->c_widget.w_save(x, b);
        }
        binbuf_addv(b, ";");
    }
}

void eobj_save(t_gobj* x, t_binbuf *b)
{
    t_eclass* c;
    if(x && b)
    {
        c = eobj_getclass(x);
        if(c && c->c_widget.w_dosave != NULL)
        {
            c->c_widget.w_dosave((t_eobj* )x, b);
        }
        
    }
}

void eobj_write(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_eclass* c = eobj_getclass(x);
    
    // The file name is defined
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
        // The folder seems defined
        if(pch != NULL)
        {
            atom_setsym(av, atom_getsymbol(argv));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
        // The folder isn't defined so write it in the canvas folder
        else
        {
            sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
            atom_setsym(av, gensym(buf));
            if(c->c_widget.w_write)
                c->c_widget.w_write(x, s, 1, av);
            return;
        }
    }
    // The file name is not defined so we popup a window
    else
    {
        sys_vgui("eobj_saveas %s nothing nothing\n", x->o_id->s_name);
    }
}

void eobj_attrprocess_viatoms(void *x, int argc, t_atom *argv)
{
    int     i;
    char    buffer[MAXPDSTRING];
    int     defc        = 0;
    t_atom* defv        = NULL;
    t_eclass* c         = eobj_getclass(x);
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(buffer, "@%s", c->c_attr[i]->name->s_name);
        atoms_get_attribute(argc, argv, gensym(buffer), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void eobj_attrprocess_viabinbuf(void *x, t_binbuf *d)
{
    int i;
    char attr_name[MAXPDSTRING];
    
    int defc       = 0;
    t_atom* defv    = NULL;
    t_eclass* c     = eobj_getclass(x);
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(attr_name, "@%s", c->c_attr[i]->name->s_name);
        binbuf_get_attribute(d, gensym(attr_name), &defc, &defv);
        if(defc && defv)
        {
            object_attr_setvalueof((t_object *)x, c->c_attr[i]->name, defc, defv);
            defc = 0;
            free(defv);
            defv = NULL;
        }
    }
}

void eobj_read(t_eobj* x, t_symbol* s, int argc, t_atom *argv)
{
    char buf[MAXPDSTRING];
    char* pch;
    t_atom av[1];
    t_namelist* var;
    t_eclass* c = eobj_getclass(x);
    
    // Name
    if(argc && argv && atom_gettype(argv) == A_SYMBOL)
    {
        // Valid path
        if((access(atom_getsymbol(argv)->s_name, O_RDONLY) != -1))
        {
            if(c->c_widget.w_read)
                c->c_widget.w_read(x, s, 1, argv);
        }
        // Invalid path or no path
        else
        {
            // Wrong path but we don't care
            pch = strpbrk(atom_getsymbol(argv)->s_name, "/\"");
            if(pch != NULL)
            {
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, argv);
            }
            else
            {
                // Look in the canvas folder
                sprintf(buf, "%s/%s", canvas_getdir(x->o_canvas)->s_name, atom_getsymbol(argv)->s_name);
                if((access(buf, O_RDONLY) != -1))
                {
                    atom_setsym(av, gensym(buf));
                    if(c->c_widget.w_read)
                        c->c_widget.w_read(x, s, 1, av);
                    return;
                }
                // Look in the search path
                var = sys_searchpath;
                while (var)
                {
                    sprintf(buf, "%s/%s", var->nl_string, atom_getsymbol(argv)->s_name);
                    if((access(buf, O_RDONLY) != -1))
                    {
                        atom_setsym(av, gensym(buf));
                        if(c->c_widget.w_read)
                            c->c_widget.w_read(x, s, 1, av);
                        return;
                    }
                    var = var->nl_next;
                }
                
                // Nothing work but we don't care
                if(c->c_widget.w_read)
                    c->c_widget.w_read(x, s, 1, av);
                return;
            }
        }
    }
    // No name so we popup a window
    else
    {
        sys_vgui("eobj_openfrom %s\n", x->o_id->s_name);
    }
}

void eobj_dspsetup(void *x, long nins, long nouts)
{
    int i;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    nins = (long)pd_clip_min(nins, 0);
    nouts = (long)pd_clip_min(nouts, 0);
    
    if(eobj_isbox(x))
    {
        box->d_sigs_out       = NULL;
        box->d_sigs_real      = NULL;
        box->d_perform_method = NULL;
        box->d_dsp_size       = 0;
        box->d_dsp_vectors    = NULL;
        box->d_misc           = E_INPLACE;
    }
    else
    {
        obj->d_sigs_out       = NULL;
        obj->d_sigs_real      = NULL;
        obj->d_perform_method = NULL;
        obj->d_dsp_size       = 0;
        obj->d_dsp_vectors    = NULL;
        obj->d_misc           = E_INPLACE;
    }
    for(i = obj_nsigoutlets((t_object *)x); i < nouts; i++)
    {
        outlet_new((t_object *)x, &s_signal);
    }
    for(i = obj_nsiginlets((t_object *)x); i < nins; i++)
    {
        eproxy_new(x, &s_signal);
    }
}

void eobj_dspfree(void *x)
{
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    size_t nouts = (size_t)obj_nsigoutlets((t_object *)x);
    if(eobj_isbox(x))
    {
        if(box->d_sigs_out)
            freebytes(box->d_sigs_out, nouts * sizeof(t_float *));
        if(box->d_sigs_real)
            free(box->d_sigs_real);
        if(box->d_dsp_vectors)
            freebytes(box->d_dsp_vectors, (size_t)box->d_dsp_size * sizeof(t_int));
    }
    else
    {
        if(obj->d_sigs_out)
            freebytes(obj->d_sigs_out, nouts * sizeof(t_float *));
        if(obj->d_sigs_real)
            free(obj->d_sigs_real);
        if(obj->d_dsp_vectors)
            freebytes(obj->d_dsp_vectors, (size_t)obj->d_dsp_size * sizeof(t_int));
    }
    eobj_free(x);
}

void eobj_resize_inputs(void *x, long nins)
{
    int i, cinlts;
    t_eobj* obj = (t_eobj *)x;
    nins = (long)pd_clip_min(nins, 1);
    cinlts = obj_nsiginlets((t_object *)x);
    if(nins > cinlts)
    {
        for(i = cinlts; i < nins; i++)
        {
            eproxy_new(obj, &s_signal);
        }
    }
    else if(nins < cinlts)
    {
        for(i = obj->o_nproxy - 1; i >= nins; --i)
        {
            eproxy_free(obj, obj->o_proxy[i]);
        }
    }
    canvas_fixlinesfor(eobj_getcanvas(obj), (t_text *)x);
}

void eobj_dsp(void *x, t_signal **sp)
{
    int i;
    short* count;
    t_linetraverser t;
    t_outconnect    *oc;
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    t_eclass *c    = eobj_getclass(x);
    int nouts = obj_nsigoutlets((t_object *)x);
    int nins  = obj_nsiginlets((t_object *)x);
    int samplesize;
    
    if(sp && sp[0])
    {
        samplesize = sp[0]->s_n;
        if(eobj_isbox(x))
        {
            if(box->d_misc == E_NO_INPLACE)
            {
                if(box->d_sigs_out)
                {
                    box->d_sigs_out = (t_float **)realloc(box->d_sigs_out, (size_t)nouts * sizeof(t_float *));
                }
                else
                {
                    box->d_sigs_out = (t_float **)getbytes((size_t)nouts * sizeof(t_float *));
                }
                if(!box->d_sigs_out)
                {
                    pd_error(box, "can't allocate memory for ni inpace processing.");
                    return;
                }
                if(box->d_sigs_out)
                {
                    box->d_sigs_real = (t_float *)realloc(box->d_sigs_real, (size_t)(nouts * samplesize) * sizeof(t_float));
                }
                else
                {
                    box->d_sigs_real = (t_float *)getbytes((size_t)(nouts * samplesize) * sizeof(t_float));
                }
                if(!box->d_sigs_real)
                {
                    free(box->d_sigs_out);
                    pd_error(box, "can't allocate memory for ni inpace processing.");
                    return;
                }
                for(i = 0; i < nouts; i++)
                {
                    box->d_sigs_out[i] = box->d_sigs_real+i*samplesize;
                }
            }
            if(box->d_dsp_vectors)
            {
                box->d_dsp_vectors = (t_int *)resizebytes(box->d_dsp_vectors, (size_t)box->d_dsp_size * sizeof(t_int), (size_t)(nins + nouts + 6) * sizeof(t_int));
            }
            else
            {
                box->d_dsp_vectors = (t_int *)getbytes((size_t)box->d_dsp_size * sizeof(t_int));
            }
            if(!box->d_dsp_vectors)
            {
                box->d_dsp_size = 0;
                pd_error(x, "can't allocate memory for dsp vector.");
                return;
            }
            box->d_dsp_size = nins + nouts + 6;
            count = (short*)getbytes((size_t)(nins + nouts) * sizeof(short));
            if(count)
            {
                for(i = 0; i < (obj_nsiginlets((t_object *)box) + obj_nsigoutlets((t_object *)box)); i++)
                {
                    count[i] = 0;
                }
                linetraverser_start(&t, eobj_getcanvas(x));
                while((oc = linetraverser_next(&t)))
                {
                    if(t.tr_ob2 == x && obj_issignaloutlet(t.tr_ob, t.tr_outno))
                    {
                        count[t.tr_inno] = 1;
                    }
                }
                
                box->d_dsp_vectors[0] = (t_int)x;
                box->d_dsp_vectors[1] = (t_int)sp[0]->s_n;
                box->d_dsp_vectors[2] = (t_int)box->d_dsp_flag;
                box->d_dsp_vectors[3] = (t_int)box->d_dsp_user_param;
                box->d_dsp_vectors[4] = (t_int)obj_nsiginlets((t_object *)box);
                box->d_dsp_vectors[5] = (t_int)obj_nsigoutlets((t_object *)box);
                
                for(i = 6; i < box->d_dsp_size; i++)
                {
                    box->d_dsp_vectors[i] = (t_int)(sp[i - 6]->s_vec);
                }
                
                if(c->c_widget.w_dsp != NULL)
                    c->c_widget.w_dsp(x, x, count, sp[0]->s_sr, sp[0]->s_n, 0);
                
                if(box->d_perform_method != NULL && box->d_misc == E_INPLACE)
                    dsp_addv(eobj_perform_box, (int)box->d_dsp_size, box->d_dsp_vectors);
                else if(box->d_perform_method != NULL && box->d_misc == E_NO_INPLACE)
                    dsp_addv(eobj_perform_box_no_inplace, (int)box->d_dsp_size, box->d_dsp_vectors);
                
                freebytes(count, (size_t)(nins + nouts) * sizeof(short));
                return;
            }
            else
            {
                pd_error(x, "can't allocate memory for dsp chain counter.");
            }
        }
        else
        {
            if(obj->d_misc == E_NO_INPLACE)
            {
                if(obj->d_sigs_out)
                {
                    obj->d_sigs_out = (t_float **)realloc(obj->d_sigs_out, (size_t)nouts * sizeof(t_float *));
                }
                else
                {
                    obj->d_sigs_out = (t_float **)getbytes((size_t)nouts * sizeof(t_float *));
                }
                if(!obj->d_sigs_out)
                {
                    pd_error(obj, "can't allocate memory for ni inpace processing.");
                    return;
                }
                if(obj->d_sigs_out)
                {
                    obj->d_sigs_real = (t_float *)realloc(obj->d_sigs_real, (size_t)(nouts * samplesize) * sizeof(t_float));
                }
                else
                {
                    obj->d_sigs_real = (t_float *)getbytes((size_t)(nouts * samplesize) * sizeof(t_float));
                }
                if(!obj->d_sigs_real)
                {
                    free(obj->d_sigs_out);
                    pd_error(obj, "can't allocate memory for ni inpace processing.");
                    return;
                }
                for(i = 0; i < nouts; i++)
                {
                    obj->d_sigs_out[i] = obj->d_sigs_real+i*samplesize;
                }
            }
            
            if(obj->d_dsp_vectors)
            {
                obj->d_dsp_vectors = (t_int *)resizebytes(obj->d_dsp_vectors, (size_t)obj->d_dsp_size * sizeof(t_int), (size_t)(nins + nouts + 6) * sizeof(t_int));
            }
            else
            {
                obj->d_dsp_vectors = (t_int *)getbytes((size_t)obj->d_dsp_size * sizeof(t_int));
            }
            if(!obj->d_dsp_vectors)
            {
                obj->d_dsp_size = 0;
                pd_error(x, "can't allocate memory for dsp vector.");
                return;
            }
            obj->d_dsp_size = nins + nouts + 6;
            count = (short*)getbytes((size_t)(nins + nouts) * sizeof(short));
            if(count)
            {
                for(i = 0; i < (obj_nsiginlets((t_object *)obj) + obj_nsigoutlets((t_object *)obj)); i++)
                {
                    count[i] = 0;
                }
                linetraverser_start(&t, eobj_getcanvas(x));
                while((oc = linetraverser_next(&t)))
                {
                    if(t.tr_ob2 == x && obj_issignaloutlet(t.tr_ob, t.tr_outno))
                    {
                        count[t.tr_inno] = 1;
                    }
                }
                
                obj->d_dsp_vectors[0] = (t_int)x;
                obj->d_dsp_vectors[1] = (t_int)sp[0]->s_n;
                obj->d_dsp_vectors[2] = (t_int)obj->d_dsp_flag;
                obj->d_dsp_vectors[3] = (t_int)obj->d_dsp_user_param;
                obj->d_dsp_vectors[4] = (t_int)obj_nsiginlets((t_object *)obj);
                obj->d_dsp_vectors[5] = (t_int)obj_nsigoutlets((t_object *)obj);
                
                for(i = 6; i < obj->d_dsp_size; i++)
                {
                    obj->d_dsp_vectors[i] = (t_int)(sp[i - 6]->s_vec);
                }
                
                if(c->c_widget.w_dsp != NULL)
                    c->c_widget.w_dsp(x, x, count, sp[0]->s_sr, sp[0]->s_n, 0);
                
                if(obj->d_perform_method != NULL && obj->d_misc == E_INPLACE)
                    dsp_addv(eobj_perform, (int)obj->d_dsp_size, obj->d_dsp_vectors);
                else if(obj->d_perform_method != NULL && obj->d_misc == E_NO_INPLACE)
                    dsp_addv(eobj_perform_no_inplace, (int)obj->d_dsp_size, obj->d_dsp_vectors);
                
                freebytes(count, (size_t)(nins + nouts) * sizeof(short));
                return;
            }
            else
            {
                pd_error(x, "can't allocate memory for dsp chain counter.");
            }
        }
    }
}

t_sample* eobj_getsignalinput(void *x, long index)
{
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    if(eobj_isbox(x) && index < obj_nsiginlets((t_object *)x) && index >= 0 && box->d_dsp_vectors )
    {
        return (t_sample *)box->d_dsp_vectors[6 + index];
    }
    else if(index < obj_nsiginlets((t_object *)x) && index >= 0 && obj->d_dsp_vectors)
    {
        
        return (t_sample *)obj->d_dsp_vectors[6 + index];
    }
    return NULL;
}

t_sample* eobj_getsignaloutput(void *x, long index)
{
    t_edspobj* obj = (t_edspobj *)x;
    t_edspbox* box = (t_edspbox *)x;
    
    if(eobj_isbox(x) && index < obj_nsigoutlets((t_object *)x) && index >= 0 && box->d_dsp_vectors)
    {
        if(box->d_misc == E_NO_INPLACE)
            return box->d_sigs_out[index];
        else
            return (t_sample *)box->d_dsp_vectors[index + 6 + obj_nsiginlets((t_object *)x)];
    }
    else if(index < obj_nsigoutlets((t_object *)x) && index >= 0 && obj->d_dsp_vectors)
    {
        if(obj->d_misc == E_NO_INPLACE)
            return obj->d_sigs_out[index];
        else
            return (t_sample *)obj->d_dsp_vectors[index + 6 + obj_nsiginlets((t_object *)x)];
    }
    return NULL;
}

t_int* eobj_perform(t_int* w)
{
    t_edspobj* x            = (t_edspobj *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    x->d_perform_method(x, NULL, ins, nins, outs, nouts, nsamples, flag, user_p);
    
    return w + (x->d_dsp_size + 1);
}

t_int* eobj_perform_no_inplace(t_int* w)
{
    int i;
    t_edspobj* x            = (t_edspobj *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    x->d_perform_method(x, NULL, ins, nins, x->d_sigs_out, nouts, nsamples, flag, user_p);
    
    for(i = 0; i < nouts; i++)
    {
        memcpy(outs[i], x->d_sigs_out[i], (size_t)nsamples * sizeof(t_float));
    }
    return w + (x->d_dsp_size + 1);
}

t_int* eobj_perform_box(t_int* w)
{
    t_edspbox* x            = (t_edspbox *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    x->d_perform_method(x, NULL, ins, nins, outs, nouts, nsamples, flag, user_p);
    
    return w + (x->d_dsp_size + 1);
}

t_int* eobj_perform_box_no_inplace(t_int* w)
{
    int i;
    t_edspbox* x            = (t_edspbox *)(w[1]);
    long nsamples           = (long)(w[2]);
    long flag               = (long)(w[3]);
    void* user_p            = (void *)(w[4]);
    long nins               = (long)(w[5]);
    long nouts              = (long)(w[6]);
    t_float** ins           = (t_float **)(&w[7]);
    t_float** outs          = (t_float **)(&w[7 + nins]);
    
    x->d_perform_method(x, NULL, ins, nins, x->d_sigs_out, nouts, nsamples, flag, user_p);
    
    for(i = 0; i < nouts; i++)
    {
        memcpy(outs[i], x->d_sigs_out[i], (size_t)nsamples * sizeof(t_float));
    }
    
    return w + (x->d_dsp_size + 1);
}

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
//! @cond
union inletunion
{
    t_symbol *iu_symto;
    t_gpointer *iu_pointerslot;
    t_float *iu_floatslot;
    t_symbol **iu_symslot;
#ifdef PD_BLOBS
    t_blob **iu_blobslot;
#endif
    t_float iu_floatsignalvalue;
};

struct _inlet
{
    t_pd i_pd;
    struct _inlet *i_next;
    t_object *i_owner;
    t_pd *i_dest;
    t_symbol *i_symfrom;
    union inletunion i_un;
};

struct _outlet
{
    t_object *o_owner;
    struct _outlet *o_next;
    t_outconnect *o_connections;
    t_symbol *o_sym;
};
//! @endcond

static t_class* eproxy_class;

static void inlet_wrong(t_inlet *x, t_symbol *s)
{
    pd_error(x->i_owner, "inlet: expected '%s' but got '%s'",
             x->i_symfrom->s_name, s->s_name);
}

static void new_inlet_list(t_inlet *x, t_symbol *s, int argc, t_atom *argv);

static void new_inlet_bang(t_inlet *x)
{
    if (x->i_symfrom == &s_bang)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "");
    else if (!x->i_symfrom)
        pd_bang(x->i_dest);
    else if (x->i_symfrom == &s_list)
        new_inlet_list(x, &s_bang, 0, 0);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)proxy->p_owner, &s_bang, 0, NULL);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_bang);
}

static void new_inlet_pointer(t_inlet *x, t_gpointer *gp)
{
    if (x->i_symfrom == &s_pointer)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "p", gp);
    else if (!x->i_symfrom)
        pd_pointer(x->i_dest, gp);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETPOINTER(&a, gp);
        new_inlet_list(x, &s_pointer, 1, &a);
    }
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETPOINTER(&a, gp);
        pd_typedmess((t_pd *)x->i_dest, &s_pointer, 1, &a);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_pointer);
}

static void new_inlet_float(t_inlet *x, t_float f)
{
    if (x->i_symfrom == &s_float)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "f", (t_floatarg)f);
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETFLOAT(&a, f);
        pd_typedmess((t_pd *)x->i_dest, &s_float, 1, &a);
        z->o_current_proxy = 0;
    }
    else if (x->i_symfrom == &s_signal)
        x->i_un.iu_floatsignalvalue = f;
    else if (!x->i_symfrom)
        pd_float(x->i_dest, f);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETFLOAT(&a, f);
        new_inlet_list(x, &s_float, 1, &a);
    }
    else inlet_wrong(x, &s_float);
}

static void new_inlet_symbol(t_inlet *x, t_symbol *s)
{
    if (x->i_symfrom == &s_symbol)
        pd_vmess(x->i_dest, x->i_un.iu_symto, "s", s);
    else if (!x->i_symfrom)
        pd_symbol(x->i_dest, s);
    else if (x->i_symfrom == &s_list)
    {
        t_atom a;
        SETSYMBOL(&a, s);
        new_inlet_list(x, &s_symbol, 1, &a);
    }
    else if (*x->i_dest == eproxy_class)
    {
        t_atom a;
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        SETSYMBOL(&a, s);
        pd_typedmess((t_pd *)x->i_dest, &s_symbol, 1, &a);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, &s_symbol);
}

static void new_inlet_list(t_inlet *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->i_symfrom == &s_list || x->i_symfrom == &s_float || x->i_symfrom == &s_symbol || x->i_symfrom == &s_pointer)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    }
    else if (!x->i_symfrom)
    {
        typedmess(x->i_dest, s, argc, argv);
        //pd_list(x->i_dest, s, argc, argv);
    }
    else if (!argc)
        new_inlet_bang(x);
    else if (argc==1 && argv->a_type == A_FLOAT)
        new_inlet_float(x, atom_getfloat(argv));
    else if (argc==1 && argv->a_type == A_SYMBOL)
        new_inlet_symbol(x, atom_getsymbol(argv));
    else inlet_wrong(x, &s_list);
    
}

static void new_inlet_anything(t_inlet *x, t_symbol *s, int argc, t_atom *argv)
{
    if (x->i_symfrom == s)
        typedmess(x->i_dest, x->i_un.iu_symto, argc, argv);
    else if (!x->i_symfrom)
        typedmess(x->i_dest, s, argc, argv);
    else if (*x->i_dest == eproxy_class)
    {
        t_eproxy* proxy = (t_eproxy *)x->i_dest;
        t_eobj *z = (t_eobj *)proxy->p_owner;
        z->o_current_proxy = proxy->p_index;
        pd_typedmess((t_pd *)x->i_dest, s, argc, argv);
        z->o_current_proxy = 0;
    }
    else inlet_wrong(x, s);
}

static void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_typedmess((t_pd *)x->p_owner, s, argc, argv);
}

static void eproxy_bang(t_eproxy *x)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_bang((t_pd *)x->p_owner);
}

static void eproxy_float(t_eproxy *x, float f)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_float((t_pd *)x->p_owner, f);
}

static void eproxy_symbol(t_eproxy *x, t_symbol* s)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_symbol((t_pd *)x->p_owner, s);
}

static void eproxy_list(t_eproxy *x, t_symbol* s, int argc, t_atom* argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_current_proxy = x->p_index;
    pd_list((t_pd *)x->p_owner, s, argc, argv);
}

static t_class* eproxy_setup()
{
    t_symbol* eproxy1572_sym = gensym("eproxy1572");
    if(!eproxy1572_sym->s_thing)
    {
        eproxy_class = class_new(gensym("eproxy"), NULL, (t_method)NULL, sizeof(t_eproxy), CLASS_PD, A_GIMME, 0);
        eproxy1572_sym->s_thing = (t_class **)eproxy_class;
        class_addanything(eproxy_class, (t_method)eproxy_anything);
        class_addbang(eproxy_class,  (t_method)eproxy_bang);
        class_addfloat(eproxy_class,  (t_method)eproxy_float);
        class_addsymbol(eproxy_class,  (t_method)eproxy_symbol);
        class_addlist(eproxy_class, (t_method)eproxy_list);
        return eproxy_class;
    }
    else
    {
        return (t_class *)eproxy1572_sym->s_thing;
    }
}

static t_inlet* einlet_new(t_object* owner, t_pd* proxy, t_symbol* s)
{
    t_class* inlet_class;
    t_inlet* inlet = inlet_new((t_object *)owner, (t_pd *)proxy, s, s);
    if(inlet)
    {
        inlet_class = inlet->i_pd;
        inlet_class->c_bangmethod = (t_bangmethod)new_inlet_bang;
        inlet_class->c_pointermethod = (t_pointermethod)new_inlet_pointer;
        inlet_class->c_floatmethod = (t_floatmethod)new_inlet_float;
        inlet_class->c_symbolmethod = (t_symbolmethod)new_inlet_symbol;
        inlet_class->c_listmethod = (t_listmethod)new_inlet_list;
        inlet_class->c_anymethod = (t_anymethod)new_inlet_anything;
    }
    return inlet;
}

static t_eproxy* eproxy_new(void *owner, t_symbol* s)
{
    t_eproxy* proxy;
    t_eobj *z = (t_eobj *)owner;
    eproxy_class = eproxy_setup();
    if(z->o_proxy)
    {
        z->o_proxy = (t_eproxy **)resizebytes(z->o_proxy, (size_t)(z->o_nproxy) * sizeof(t_eproxy *), (size_t)(z->o_nproxy + 1) * sizeof(t_eproxy *));
    }
    else
    {
        z->o_proxy = (t_eproxy **)getbytes(1 * sizeof(t_eproxy *));
    }
    if(z->o_proxy)
    {
        proxy = (t_eproxy *)pd_new(eproxy_class);
        proxy->p_owner = (t_object *)owner;
        proxy->p_pd    = eproxy_class;
        proxy->p_index = z->o_nproxy;
        proxy->p_inlet = einlet_new((t_object *)owner, (t_pd *)proxy, s);
        if(eobj_isdsp(z))
        {
            proxy->p_inlet->i_un.iu_floatsignalvalue = ((t_edspobj *)z)->d_float;
        }
        z->o_proxy[z->o_nproxy] = proxy;
        z->o_nproxy++;
        return proxy;
    }
    else
    {
        pd_error(z, "cons't allocate memory for a new proxy inlet.");
        return NULL;
    }
}

static void canvas_deletelines_for_io(t_canvas *x, t_text *text, t_inlet *inp, t_outlet *outp)
{
    t_linetraverser t;
    t_outconnect *oc;
    linetraverser_start(&t, x);
    while((oc = linetraverser_next(&t)))
    {
        if ((t.tr_ob == text && t.tr_outlet == outp) || (t.tr_ob2 == text && t.tr_inlet == inp))
        {
            if(glist_isvisible(x))
            {
                sys_vgui(".x%lx.c delete l%lx\n", glist_getcanvas(x), oc);
            }
            obj_disconnect(t.tr_ob, t.tr_outno, t.tr_ob2, t.tr_inno);
        }
    }
}

static void eproxy_free(void *owner, t_eproxy* proxy)
{
    t_eobj *z   = (t_eobj *)owner;
    if(z && proxy && proxy->p_owner == (t_object *)z)
    {
        if(z->o_nproxy == proxy->p_index + 1)
        {
            canvas_deletelines_for_io(eobj_getcanvas(owner), (t_text *)owner, proxy->p_inlet, NULL);
            inlet_free(proxy->p_inlet);
            if(z->o_nproxy == 1)
            {
                pd_free((t_pd *)z->o_proxy);
                z->o_nproxy = 0;
            }
            else
            {
                z->o_proxy = (t_eproxy **)resizebytes(z->o_proxy, (size_t)(z->o_nproxy) * sizeof(t_eproxy *), (size_t)(z->o_nproxy - 1) * sizeof(t_eproxy *));
                z->o_nproxy--;
            }
        }
        else
        {
            pd_error(owner, "can't free a proxy if it's not the last one.");
        }
    }
}

//! @endcond





