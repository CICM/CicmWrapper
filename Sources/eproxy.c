/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eproxy.h"

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

struct _eproxy
{
    t_pd        p_pd;       /*!< The class object. */
    t_object*   p_owner;    /*!< The pointer to the eobj owner. */
    t_inlet*    p_inlet;    /*!< The pointer to the inlet. */
    int         p_index;    /*!< The index of the proxy. */
};

static void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_typedmess((t_pd *)x->p_owner, s, argc, argv);
}

static void eproxy_bang(t_eproxy *x)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_bang((t_pd *)x->p_owner);
}

static void eproxy_float(t_eproxy *x, float f)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_float((t_pd *)x->p_owner, f);
}

static void eproxy_symbol(t_eproxy *x, t_symbol* s)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_symbol((t_pd *)x->p_owner, s);
}

static void eproxy_pointer(t_eproxy *x, t_gpointer *gp)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_pointer((t_pd *)x->p_owner, gp);
}

static void eproxy_list(t_eproxy *x, t_symbol* s, int argc, t_atom* argv)
{
    t_eobj *z = (t_eobj *)x->p_owner;
    z->o_cproxy = x->p_index;
    pd_list((t_pd *)x->p_owner, s, argc, argv);
}

static t_class* eproxy_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eproxy1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eproxy"), (t_newmethod)NULL, (t_method)NULL, sizeof(t_eproxy), CLASS_PD, A_NULL, 0);
        if(c)
        {
            class_addanything(c, (t_method)eproxy_anything);
            class_addbang(c,  (t_method)eproxy_bang);
            class_addfloat(c,  (t_method)eproxy_float);
            class_addsymbol(c,  (t_method)eproxy_symbol);
            class_addlist(c, (t_method)eproxy_list);
            class_addpointer(c, (t_method)eproxy_pointer);
            obj = pd_new(c);
            pd_bind(obj, gensym("eproxy1572"));
        }
        else
        {
            error("can't initialize proxy inlet class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

static t_eproxy* eproxy_new(void *owner, t_symbol* s)
{
    t_eproxy* proxy;
    t_eproxy ** temp;
    t_eobj *z = (t_eobj *)owner;
    t_edsp *dsp = eobj_getdsp(owner);
    eproxy_class = eproxy_setup();
    if(z->o_proxy)
    {
        temp = (t_eproxy **)realloc(z->o_proxy, (size_t)(z->o_nproxy + 1) * sizeof(t_eproxy *));
    }
    else
    {
        temp = (t_eproxy **)malloc(1 * sizeof(t_eproxy *));
    }
    if(temp)
    {
        z->o_proxy = temp;
        proxy = (t_eproxy *)pd_new(eproxy_class);
        proxy->p_owner = (t_object *)owner;
        proxy->p_pd    = eproxy_class;
        proxy->p_index = z->o_nproxy;
        proxy->p_inlet = einlet_new((t_object *)owner, (t_pd *)proxy, s);
        if(dsp)
        {
            proxy->p_inlet->i_un.iu_floatsignalvalue = dsp->d_float;
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
    t_eproxy ** temp;
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
                temp = (t_eproxy **)realloc(z->o_proxy, (size_t)(z->o_nproxy - 1) * sizeof(t_eproxy *));
                if(temp)
                {
                    z->o_proxy = temp;
                }
                else
                {
                    pd_error(owner, "a proxy hasn't been freed.");
                }
                z->o_nproxy--;
                
            }
        }
        else
        {
            pd_error(owner, "can't free a proxy if it's not the last one.");
        }
    }
}






