/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eclass.h"
#include "ecommon.h"
#include "eobj.h"
#include "ebox.h"
#include "epopup.h"
#include "egraphics.h"

extern void eobj_initclass(t_eclass* c);
extern void ebox_initclass(t_eclass* c);
extern void edsp_initclass(t_eclass* c);

t_eclass *eclass_new(const char *name, t_method newmethod, t_method freemethod, size_t size, int flags, t_atomtype type1, ...)
{
    int count = 0;
    va_list ap;
    t_atomtype vec[MAXPDARG+1];
    t_atomtype *vp = vec;
    t_eclass *c = NULL;
    t_pd *f = NULL, *a = NULL;
    char text[MAXPDSTRING];
    *vp = type1;
    va_start(ap, type1);
    while(*vp && count < MAXPDARG)
    {
        vp++;
        count++;
        *vp = va_arg(ap, t_atomtype);
    }
    va_end(ap);
    if(count == 0)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1);
    }
    else if(count == 1)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0]);
    }
    else if(count == 2)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0], vec[1]);
    }
    else if(count == 3)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0], vec[1], vec[2]);
    }
    else if(count == 3)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0], vec[1], vec[2], vec[3]);
    }
    else if(count == 4)
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0], vec[1], vec[2], vec[3], vec[4]);
    }
    else
    {
        c = class_new(gensym(name), (t_newmethod)newmethod, freemethod, size, flags, type1, vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
    }
    if(c)
    {
        f = (t_pd *)eflags_new();
        sprintf(text, "%lxflags", (unsigned long)c);
        pd_bind(f, gensym(text));
        a = (t_pd *)eattrset_new();
        sprintf(text, "%lxattrs", (unsigned long)c);
        pd_bind(f, gensym(text));
        eobj_initclass(c);
    }
    return (c);
}

void eclass_guiinit(t_eclass* c, long flags)
{
    class_addmethod(c, (t_method)truemethod, s_cream_isgui, A_CANT, 0);
    ebox_initclass(c);
}

void eclass_dspinit(t_eclass* c)
{
    edsp_initclass(c);
}

static t_eflagman* eclass_getflagman(t_eclass const* c)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxflags", (unsigned long)c);
    return eflags_findbyname(gensym(text));
}

static t_eattrset* eclass_getattrset(t_eclass const* c)
{
    char text[MAXPDSTRING];
    sprintf(text, "%lxattrs", (unsigned long)c);
    return eattrset_findbyname(gensym(text));
}

long eclass_getflags(t_eclass const* c)
{
    t_eflagman* fl = eclass_getflagman(c);
    if(fl)
    {
        return eflags_get(fl);
    }
    return 0;
}

t_pd_err eclass_register(t_symbol *name, t_eclass *c)
{
    if(eclass_getflags(c) & EBOX_TEXTFIELD)
    {
        class_addmethod((t_class *)c, (t_method)ebox_texteditor_keypress, gensym("texteditor_keypress"), A_SYMBOL,A_DEFFLOAT,0);
        class_addmethod((t_class *)c, (t_method)ebox_texteditor_keyfilter, gensym("texteditor_keyfilter"), A_SYMBOL,A_DEFFLOAT,0);
        class_addmethod((t_class *)c, (t_method)ebox_texteditor_focus, gensym("texteditor_focus"), A_SYMBOL,A_DEFFLOAT,0);
    }
    
    class_addmethod((t_class *)c, (t_method)ebox_set_parameter_attribute, gensym("param"), A_GIMME, 0);
    return 0;
}

void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type1, ...)
{
    int count = 0;
    va_list ap;
    t_atomtype vec[MAXPDARG+1];
    t_atomtype *vp = vec;
    *vp = type1;
    va_start(ap, type1);
    while(*vp && count < MAXPDARG)
    {
        vp++;
        count++;
        *vp = va_arg(ap, t_atomtype);
    }
    va_end(ap);
    
    if(count == 0)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1);
    }
    else if(count == 1)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0]);
    }
    else if(count == 2)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0], vec[1]);
    }
    else if(count == 3)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0], vec[1], vec[2]);
    }
    else if(count == 3)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0], vec[1], vec[2], vec[3]);
    }
    else if(count == 4)
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0], vec[1], vec[2], vec[3], vec[4]);
    }
    else
    {
        class_addmethod(c, (t_method)m, gensym(name), type1, vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
    }
}

long eclass_getnattrs(t_eclass const* c)
{
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        return (long)as->s_size;
    }
    return 0;
}

static t_eattr* eclass_getattr(t_eclass const* c, t_symbol* name)
{
    t_eattrset* as = eclass_getattrset(c);
    if(c)
    {
        return eattrset_getattr(as, name);
    }
}

void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type,
                           size_t size, size_t maxsize, long flags, size_t offset)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_new(as, name, gensym(type), size, maxsize, offset);
        eattrset_attr_category(as, name, gensym(class_getname(c)));
        eattrset_attr_flags(as, name, flags);
        class_addmethod(c, (t_method)NULL, name, A_GIMME, 0);
    }
}

void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value)
{
    size_t ndefaults;
    t_atom* defaults;
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        int todo_parser;
        eattrset_attr_default(as, name, ndefaults, defaults);
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_category(as, name, gensym(category));
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_order(as, name, (long)atoi(order));
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_label(as, name, gensym(label));
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_style(as, name, gensym(style));
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_items(t_eclass* c, const char* attrname, long flags, const char* list)
{
    size_t nitems;
    t_symbol** items;
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        int todo_parser;
        eattrset_attr_items(as, name, nitems, items);
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_filter_min(as, name, value);
    }
}

void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_filter_max(as, name, value);
    }
}

void eclass_attr_step(t_eclass* c, const char* attrname, float value)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_step(as, name, value);
    }
}

void eclass_attr_save(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_save(as, name, 1);
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_paint(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_paint(as, name, 1);
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_visible(as, name, 0);
        eattrset_attr_flags(as, name, flags);
    }
}

void eclass_attr_accessor(t_eclass* c, const char* attrname, t_getter_method getter, t_setter_method setter)
{
    t_symbol* name = gensym(attrname);
    t_eattrset* as = eclass_getattrset(c);
    if(as)
    {
        eattrset_attr_accessor(as, name, getter, setter);
    }
}






