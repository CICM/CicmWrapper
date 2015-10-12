/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eparam.h"
#include "eobj.h"

#include "m_imp.h"

typedef void (*t_proxy_method)(void* x, size_t index);

struct _eparam
{
    t_object        p_object;
    t_symbol*       p_bind;
    t_symbol*       p_name;
    t_symbol*       p_label;
    t_object*       p_owner;
    int             p_index;
    float           p_value;
    float           p_min;
    float           p_max;
    int             p_nstep;
    t_param_getter  p_getter;
    t_param_setter  p_setter;
    t_param_getter_t p_getter_t;
    t_param_setter_t p_setter_t;
    char            p_auto;
    char            p_meta;
    char            p_enable;
    long            p_flags;
    
};

float eparameter_getvalue(t_eparam* param)
{
    if(param->p_getter)
    {
        return param->p_getter(param->p_owner, param);
    }
    return param->p_value;
}

float eparameter_getvalue_normalized(t_eparam* param)
{
    if(param->p_max == param->p_min)
    {
        return 1.;
    }
    else if(param->p_min < param->p_max)
    {
        return (eparameter_getvalue(param) - param->p_min) / (param->p_max - param->p_min);
    }
    else
    {
        return (eparameter_getvalue(param) - param->p_max) / (param->p_min - param->p_max);
    }
}

void eparameter_getvalue_text(t_eparam* param, char* text)
{
    if(param->p_getter_t)
    {
        param->p_getter_t(param->p_owner, param->p_index, text);
    }
    else
    {
        sprintf(text, "%g", eparameter_getvalue(param));
    }
}

static void eparameter_notify_owner(t_eparam* param, t_symbol* message)
{
    t_eclass* c = eobj_getclass(param->p_owner);
    if(c->c_widget.w_notify)
    {
        c->c_widget.w_notify(param->p_owner, param->p_bind, message, NULL, NULL);
    }
}

static float eparameter_compute_value(float value, float min, float max, float nstep)
{
    const float step = (max - min) / nstep;
    const float rval = floorf((value - min) / step + 0.5);
    return pd_clip_max(step * rval + min, max);
}


void eparameter_setvalue(t_eparam* param, float value)
{
    if(param->p_setter)
    {
        param->p_setter(param->p_owner, param, value);
    }
    else
    {
        if(param->p_min < param->p_max)
        {
            param->p_value = eparameter_compute_value(value, param->p_min, param->p_max, (float)param->p_nstep);
        }
        else
        {
            param->p_value = eparameter_compute_value(value, param->p_max, param->p_min, (float)param->p_nstep);
        }
        eparameter_notify_owner(param, s_cream_value_changed);
    }
}

void eparameter_setvalue_normalized(t_eparam* param, float value)
{
    if(param->p_min < param->p_max)
    {
        eparameter_setvalue(param, (value * (param->p_max - param->p_min) + param->p_min));
    }
    else
    {
        eparameter_setvalue(param, (value * (param->p_min - param->p_max) + param->p_max));
    }
}

void eparameter_setvalue_text(t_eparam* param, char const* text)
{
    if(param->p_setter_t)
    {
        param->p_setter_t(param->p_owner, param->p_index, text);
    }
    else if(isdigit(text[0]))
    {
        eparameter_setvalue(param, atof(text));
    }
}

void eparameter_setname(t_eparam* param, t_symbol* name)
{
    if(!(param->p_flags & EPARAM_STATIC_NAME))
    {
        param->p_name = get_valid_symbol(name);
        eparameter_notify_owner(param, s_cream_attr_modified);
        canvas_dirty(eobj_getcanvas(param->p_owner), 1);
    }
}

void eparameter_setlabel(t_eparam* param, t_symbol* label)
{
    if(!(param->p_flags & EPARAM_STATIC_LABEL))
    {
        param->p_label = get_valid_symbol(label);
        eparameter_notify_owner(param, s_cream_attr_modified);
        canvas_dirty(eobj_getcanvas(param->p_owner), 1);
    }
}

void eparameter_setindex(t_eparam* param, int index)
{
    if(!(param->p_flags & EPARAM_STATIC_INDEX))
    {
        param->p_index = index;
        eparameter_notify_owner(param, s_cream_attr_modified);
        canvas_dirty(eobj_getcanvas(param->p_owner), 1);
    }
}

t_eparam* eparameter_getfromsymbol(t_symbol* name)
{
    t_class* c = eparameter_setup();
    if(c)
    {
        return (t_eparam *)pd_findbyclass(name, c);
    }
    return NULL;
    
}

static t_class* eproxy_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eproxy1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eproxy"), (t_newmethod)NULL, (t_method)eproxy_free, sizeof(t_eproxy), CLASS_PD, A_NULL, 0);
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

t_eproxy* eproxy_new(t_object *owner, t_symbol* s, size_t index)
{
    t_class* c = eproxy_setup();
    t_eproxy* x = NULL;
    if(c)
    {
        x = (t_eproxy *)pd_new(c);
        if(x)
        {
            x->p_method = (t_proxy_method)zgetfn((t_pd *)owner, gensym("setproxyindex"));
            x->p_owner  = owner;
            x->p_index  =index;
            x->p_inlet  = inlet_new(owner, (t_pd *)x, s, s);
        }
        else
        {
            pd_error(owner, "can't allocate proxy inlet.");
        }
    }
    return x;
}






