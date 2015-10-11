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

#include "m_imp.h"

typedef void (*t_proxy_method)(void* x, size_t index);

struct _eproxy
{
    t_pd            p_pd;       /*!< The class object. */
    t_object*       p_owner;    /*!< The pointer to the eobj owner. */
    t_inlet*        p_inlet;    /*!< The pointer to the inlet. */
    size_t          p_index;    /*!< The index of the proxy. */
    t_proxy_method  p_method;   /*!< The proxy setter. */
};

static void eproxy_anything(t_eproxy *x, t_symbol *s, int argc, t_atom *argv)
{
    x->p_method(x->p_owner, x->p_index);
    pd_typedmess((t_pd *)x->p_owner, s, argc, argv);
}

static void eproxy_bang(t_eproxy *x)
{
    x->p_method(x->p_owner, x->p_index);
    pd_bang((t_pd *)x->p_owner);
}

static void eproxy_float(t_eproxy *x, float f)
{
    x->p_method(x->p_owner, x->p_index);
    pd_float((t_pd *)x->p_owner, f);
}

static void eproxy_symbol(t_eproxy *x, t_symbol* s)
{
    x->p_method(x->p_owner, x->p_index);
    pd_symbol((t_pd *)x->p_owner, s);
}

static void eproxy_pointer(t_eproxy *x, t_gpointer *gp)
{
    x->p_method(x->p_owner, x->p_index);
    pd_pointer((t_pd *)x->p_owner, gp);
}

static void eproxy_list(t_eproxy *x, t_symbol* s, int argc, t_atom* argv)
{
    x->p_method(x->p_owner, x->p_index);
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

t_eproxy* eproxy_new(t_object *owner, t_symbol* s)
{
    t_eproxy* x;
    t_eproxy** temp;
    t_eobj *z = (t_eobj *)owner;
    t_class* c = eproxy_setup();
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
        x = (t_eproxy *)pd_new(c);
        x->p_method = (t_proxy_method)zgetfn((t_pd *)owner, gensym("setproxyindex"));
        x->p_owner  = owner;
        x->p_index  = z->o_nproxy;
        x->p_inlet  = inlet_new(owner, (t_pd *)x, s, s);
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






