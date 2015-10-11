/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eflags.h"

struct _eflagman
{
    t_object        f_object;
    long            f_flags;
};

static t_class* eflags_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eflags1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eflags"), (t_newmethod)NULL, (t_method)NULL, sizeof(t_eflagman), CLASS_PD, A_NULL, 0);
        if(c)
        {
            obj = pd_new(c);
            pd_bind(obj, gensym("eflags1572"));
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

t_eflagman* eflags_new(void)
{
    t_eflagman* x = NULL;
    t_class*    c = eflags_setup();
    if(c)
    {
        x = (t_eflagman *)pd_new(c);
        if(x)
        {
            x->f_flags = 0;
        }
    }
    return x;
}

t_eflagman* eflags_findbyname(t_symbol* name)
{
    t_class* c = eflags_setup();
    if(c)
    {
        return (t_eflagman *)pd_findbyclass(name, c);
    }
    return NULL;
}

void eflags_free(t_eflagman *flags)
{
    pd_free((t_pd *)flags);
}

long eflags_get(t_eflagman const* flagman)
{
    return flagman->f_flags;
}

char eflags_has(t_eflagman const* flagman, long flag)
{
    return flagman->f_flags & flag;
}

void eflags_set(t_eflagman* flagman, long flags)
{
    flagman->f_flags = flags;
}

void eflags_add(t_eflagman* flagman, long flag)
{
    flagman->f_flags = flagman->f_flags | flag;
}

void eflags_remove(t_eflagman* flagman, long flag)
{
    flagman->f_flags = flagman->f_flags & ~flag;
}






