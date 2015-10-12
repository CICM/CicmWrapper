/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "eattr.h"
#include "egraphics.h"

struct _eattr
{
    t_object        a_obj;        /*!< The object. */
    t_symbol*       a_name;       /*!< The name of the attribute. */
    t_symbol*       a_type;       /*!< The type of the attribute (int, long, float, double, rgba, etc.). */
    t_symbol*       a_category;   /*!< The category of the attribute. */
    t_symbol*       a_label;      /*!< The label of the attribute. */
    t_symbol*       a_style;      /*!< The style of the attribute (checkbutton, color, number, entry, menu). */
    char            a_saved;      /*!< If the attribute should be saved. */
    char            a_paint;      /*!< If the attribute should repaint the t_ebox when it has changed. */
    char            a_invisible;  /*!< If the attribute is invisible. */
    long            a_order;      /*!< The order of the attribute. */
    long			a_flags;      /*!< The flags of the attribute. */
    
    size_t          a_offset;     /*!< The offset of the attribute in the object structure. */
    size_t          a_sizemax;    /*!< The maximum size of the attribute if the attribute is an array. */
    size_t          a_size;       /*!< The size of the attribute if the attribute is an array. */
    
    t_getter_method a_getter;     /*!< The getter method of the attribute. */
    t_setter_method a_setter;     /*!< The setter method of the attribute. */
    
    char            a_clipped;    /*!< If the attribute is clipped if it's value or an array of numerical values. */
    float           a_minimum;    /*!< The minimum value of the attribute. */
    float           a_maximum;    /*!< The maximum value of the attribute. */
    float           a_step;       /*!< The increment or decrement step calue of the attribute. */
    
    t_atom*         a_defaults;   /*!< The default values of the attribute. */
    size_t          a_ndefaults;  /*!< The number of default values of the attribute. */
    t_symbol**      a_items;      /*!< The available items of an attribute if it is a menu. */
    size_t          a_nitems;     /*!< The number of available items of an attribute if it is a menu. */
};

static inline void* eattr_getvalue_adress(t_object const* x, t_eattr const* attr)
{
    return (char *)x + attr->a_offset;
}

static inline long* eattr_getsize_adress(t_object const* x, t_eattr const* attr)
{
    return (long *)((char *)x + attr->a_size);
}

static inline size_t eattr_getsize(t_object const* x, t_eattr const* attr)
{
    const size_t size = (size_t)(long *)((char *)x + attr->a_size);
    if(attr->a_sizemax == 0)
    {
        return attr->a_size;
    }
    return attr->a_sizemax < size ? attr->a_sizemax : size;
}

static float atom_getfloatcliparg(int which, int argc, t_atom* argv, char clipping, float min, float max)
{
    if(clipping == 1)
    {
        return pd_clip_min(atom_getfloatarg(which, argc, argv), min);
    }
    else if(clipping == 2)
    {
        return pd_clip_max(atom_getfloatarg(which, argc, argv), max);
    }
    else if(clipping == 3)
    {
        return pd_clip(atom_getfloatarg(which, argc, argv), min, max);
    }
    return atom_getfloatarg(which, argc, argv);
}

static t_pd_err eattr_setvalue_char(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    char* pointer = (char *)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = (char)atom_getfloatcliparg(i, argc, argv, attr->a_clipped, attr->a_minimum, attr->a_maximum);
    }
    return 0;
}

static t_pd_err eattr_setvalue_int(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    int* pointer = (int *)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = (int)atom_getfloatcliparg(i, argc, argv, attr->a_clipped, attr->a_minimum, attr->a_maximum);
    }
    return 0;
}

static t_pd_err eattr_setvalue_long(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    long* pointer = (long *)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = (long)atom_getfloatcliparg(i, argc, argv, attr->a_clipped, attr->a_minimum, attr->a_maximum);
    }
    return 0;
}

static t_pd_err eattr_setvalue_float(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    float* pointer = (float *)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = atom_getfloatcliparg(i, argc, argv, attr->a_clipped, attr->a_minimum, attr->a_maximum);
    }
    return 0;
}

static t_pd_err eattr_setvalue_double(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    double* pointer = (double *)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = (double)atom_getfloatcliparg(i, argc, argv, attr->a_clipped, attr->a_minimum, attr->a_maximum);
    }
    return 0;
}

static t_pd_err eattr_setvalue_symbol(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    int i;
    t_symbol** pointer = (t_symbol **)eattr_getvalue_adress(x, attr);
    for(i = 0; i < argc; i++)
    {
        pointer[i] = atom_getsymbolarg(i, argc, argv);
    }
    return 0;
}

static t_pd_err eattr_setvalue_atom(t_object* x, t_eattr* attr, int argc, t_atom* argv)
{
    t_atom* pointer = (t_atom *)eattr_getvalue_adress(x, attr);
    memcpy(pointer, argv, (size_t)argc * sizeof(t_atom));
    return 0;
}

static void eattr_setvalue(t_eattr* attr, t_object *x, int argc, t_atom *argv)
{
    int ac = 0; t_atom* av = NULL;
    int this_stuff_should_be_in_the_submethod;
    unparse_atoms(argc, argv, &ac, &av);
    if(attr->a_sizemax == 0)
    {
        ac = ((int)attr->a_size < ac) ? (int)attr->a_size : ac;
    }
    else
    {
        ac = ((int)attr->a_sizemax < ac) ? (int)attr->a_sizemax : ac;
        *(eattr_getsize_adress(x, attr)) = ac;
    }
    (attr->a_setter)(x, (t_object *)attr, ac, av);
    if(ac && av)
    {
        free(av);
    }
}

static void eattr_setvalue_default(t_eattr* attr, t_object *x)
{
    eattr_setvalue(attr, x, (int)attr->a_ndefaults, attr->a_defaults);
}






static t_pd_err eattr_getvalue_char(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    char const* pointer = (char const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setfloat((*argv)+i, (float)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_int(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    int const* pointer = (int const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setfloat((*argv)+i, (float)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_long(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    long const* pointer = (long const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setfloat((*argv)+i, (float)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_float(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    float const* pointer = (float const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setfloat((*argv)+i, (float)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_double(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    double const* pointer = (double const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setfloat((*argv)+i, (float)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_symbol(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    int i;
    t_symbol* const* pointer = (t_symbol* const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
        for(i = 0; i < *argc; i++)
        {
            atom_setsym((*argv)+i, (t_symbol *)(pointer[i]));
        }
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue_atom(t_object* x, t_eattr* attr, int* argc, t_atom** argv)
{
    t_atom const* pointer = (t_atom const*)eattr_getvalue_adress(x, attr);
    *argc = (int)eattr_getsize(x, attr);
    *argv = (t_atom *)malloc((size_t)(*argc) * sizeof(t_atom));
    if(*argv)
    {
         memcpy(argv, pointer, (size_t)argc * sizeof(t_atom));
        return 0;
    }
    return -1;
}

static t_pd_err eattr_getvalue(t_eattr* attr, t_object *x, int* argc, t_atom **argv)
{
    if(attr->a_getter)
    {
        return (attr->a_getter)(x, (t_object *)attr, argc, argv);
    }
    pd_error(attr, "attribute getter isn't initialized.");
    return -1;
}





static void eattr_free(t_eattr *attr)
{
    if(attr->a_nitems && attr->a_items)
    {
        free(attr->a_items);
        attr->a_items = NULL;
        attr->a_nitems = 0;
    }
    if(attr->a_defaults && attr->a_ndefaults)
    {
        free(attr->a_defaults);
        attr->a_defaults = NULL;
        attr->a_ndefaults = 0;
    }
}

static t_class* eattr_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eattr1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eattr"), (t_newmethod)NULL, (t_method)eattr_free, sizeof(t_eattr), CLASS_PD, A_NULL, 0);
        if(c)
        {
            class_addmethod(c, (t_method)eattr_getvalue, gensym("getvalue"), A_CANT, 0);
            class_addmethod(c, (t_method)eattr_setvalue, gensym("setvalue"), A_CANT, 0);
            class_addmethod(c, (t_method)eattr_setvalue_default, gensym("setvaluedefault"), A_CANT, 0);
            obj = pd_new(c);
            pd_bind(obj, gensym("eattr1572"));
        }
        else
        {
            error("can't initialize attribute class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

t_eattr *eattr_new(t_symbol *name, t_symbol *type, size_t size, size_t maxsize, size_t offset)
{
    t_eattr *x = NULL;
    t_class* c = eattr_setup();
    if(c)
    {
        x  = (t_eattr *)pd_new(c);
        if(x)
        {
            x->a_name       = name;
            x->a_type       = type;
            x->a_category   = s_cream_empty;
            x->a_label      = s_cream_empty;
            x->a_style      = s_cream_empty;
            x->a_saved       = 0;
            x->a_paint      = 0;
            x->a_invisible  = 0;
            x->a_order      = 0;
            x->a_flags      = 0;
            x->a_offset     = offset;
            x->a_size       = size;
            x->a_sizemax    = maxsize;
            x->a_clipped    = 0;
            x->a_minimum    = 0;
            x->a_maximum    = 1;
            x->a_step       = 1;
            x->a_ndefaults  = 0;
            x->a_defaults   = NULL;
            x->a_items      = NULL;
            x->a_nitems     = 0;
            
            
            if(x->a_type == s_cream_char)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_char;
                x->a_getter = (t_getter_method)eattr_getvalue_char;
            }
            else if(x->a_type == s_cream_int)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_int;
                x->a_getter = (t_getter_method)eattr_getvalue_int;
            }
            else if(x->a_type == s_cream_long)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_long;
                x->a_getter = (t_getter_method)eattr_getvalue_long;
            }
            else if(x->a_type == s_cream_float)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_float;
                x->a_getter = (t_getter_method)eattr_getvalue_float;
            }
            else if(x->a_type == s_cream_double)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_double;
                x->a_getter = (t_getter_method)eattr_getvalue_double;
            }
            else if(x->a_type == s_cream_symbol)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_symbol;
                x->a_getter = (t_getter_method)eattr_getvalue_symbol;
            }
            else if(x->a_type == s_cream_atom)
            {
                x->a_setter = (t_setter_method)eattr_setvalue_atom;
                x->a_getter = (t_getter_method)eattr_getvalue_atom;
            }
            else
            {
                x->a_getter     = NULL;
                x->a_setter     = NULL;
            }
        }
        else
        {
            error("can't allocate attribute %s.", name->s_name);
        }
    }
    return x;
}

t_symbol* eattr_getname(t_eattr const* attr)
{
    return attr->a_name;
}

t_symbol* eattr_gettype(t_eattr const* attr)
{
    return attr->a_type;
}

t_symbol* eattr_getcategory(t_eattr const* attr)
{
    return attr->a_category;
}

t_symbol* eattr_getlabel(t_eattr const* attr)
{
    return attr->a_label;
}

t_symbol* eattr_getstyle(t_eattr const* attr)
{
    return attr->a_style;
}

long eattr_getorder(t_eattr const* attr)
{
    return attr->a_order;
}

char eattr_repaint(t_eattr const* attr)
{
    return attr->a_paint;
}

char eattr_isvisible(t_eattr const* attr)
{
    return !attr->a_invisible;
}

char eattr_issaved(t_eattr const* attr)
{
    return attr->a_saved;
}

char eattr_hasminimum(t_eattr const* attr)
{
    return attr->a_clipped == 1 || attr->a_clipped == 3;
}

char eattr_hasmaximum(t_eattr const* attr)
{
    return attr->a_clipped == 2 || attr->a_clipped == 3;
}

float eattr_getminimum(t_eattr const* attr)
{
    return attr->a_minimum;
}

float eattr_getmaximum(t_eattr const* attr)
{
    return attr->a_maximum;
}

float eattr_getstep(t_eattr const* attr)
{
    return attr->a_step;
}

void eattr_getitems(t_eattr const*attr, size_t* nitems, t_symbol*** items)
{
    if(attr->a_nitems && attr->a_items)
    {
        *items = (t_symbol **)malloc(attr->a_nitems * sizeof(t_symbol *));
        if(*items)
        {
            memcpy(*items, attr->a_items, attr->a_nitems * sizeof(t_symbol *));
            *nitems = attr->a_nitems;
            return;
        }
    }
    *nitems = 0;
    items   = NULL;
}





static void eattr_setdefault(t_eattr* attr, size_t ndefault, t_atom* defaults)
{
    t_atom* temp;
    if(ndefault && defaults)
    {
        if(attr->a_ndefaults && attr->a_defaults)
        {
            temp = (t_atom *)realloc(attr->a_defaults, ndefault * sizeof(t_atom));
            if(temp)
            {
                attr->a_defaults    = temp;
                attr->a_ndefaults   = ndefault;
                memcpy(attr->a_defaults, defaults, ndefault * sizeof(t_atom));
            }
            else
            {
                free(attr->a_defaults);
                attr->a_defaults   = NULL;
                attr->a_ndefaults  = 0;
                pd_error(attr, "can't allocate memory for attribute's default values.");
            }
        }
        else
        {
            attr->a_defaults = (t_atom *)malloc(ndefault * sizeof(t_atom));
            if(attr->a_defaults)
            {
                attr->a_ndefaults  = ndefault;
                memcpy(attr->a_defaults, defaults, ndefault * sizeof(t_atom));
            }
            else
            {
                pd_error(attr, "can't allocate memory for attribute's items.");
            }
        }
    }
    else
    {
        if(attr->a_ndefaults && attr->a_defaults)
        {
            free(attr->a_defaults);
        }
        attr->a_defaults    = NULL;
        attr->a_ndefaults   = 0;
    }
}

static void eattr_setcategory(t_eattr* attr, t_symbol* category)
{
    attr->a_category = category;
}

static void eattr_setlabel(t_eattr* attr, t_symbol* label)
{
    attr->a_label = label;
}

static void eattr_setstyle(t_eattr* attr, t_symbol* style)
{
    if(style == s_cream_checkbutton || style == s_cream_onoff)
    {
        attr->a_style = s_cream_checkbutton;
    }
    else if(style == s_cream_color)
    {
        attr->a_style = s_cream_color;
    }
    else if(style == s_cream_number)
    {
        attr->a_style = s_cream_number;
    }
    else if(style == s_cream_menu)
    {
        attr->a_style = s_cream_menu;
    }
    else if(style == s_cream_font)
    {
        attr->a_style = s_cream_font;
    }
    else
    {
        attr->a_style = s_cream_entry;
    }
}

static void eattr_setorder(t_eattr* attr, long order)
{
    attr->a_order = order;
}

static void eattr_setflags(t_eattr* attr, long flags)
{
    attr->a_flags = flags;
}

static void eattr_setmin(t_eattr* attr, float value)
{
    (attr->a_clipped == 0) ? (attr->a_clipped = 1) : (attr->a_clipped = 3);
    attr->a_minimum = value;
}

static void eattr_setmax(t_eattr* attr, float value)
{
    (attr->a_clipped == 0) ? (attr->a_clipped = 2) : (attr->a_clipped = 3);
    attr->a_maximum = value;
}

static void eattr_setstep(t_eattr* attr, float value)
{
    attr->a_step = value;
}

static void eattr_setitems(t_eattr* attr, size_t nitems, t_symbol** items)
{
    t_symbol** temp;
    if(nitems && items)
    {
        if(attr->a_nitems && attr->a_items)
        {
            temp = (t_symbol **)realloc(attr->a_items, nitems * sizeof(t_symbol *));
            if(temp)
            {
                attr->a_items   = temp;
                attr->a_nitems  = nitems;
                memcpy(attr->a_items, items, nitems * sizeof(t_symbol *));
            }
            else
            {
                free(attr->a_items);
                attr->a_items   = NULL;
                attr->a_nitems  = 0;
                pd_error(attr, "can't allocate memory for attribute's items.");
            }
        }
        else
        {
            attr->a_items = (t_symbol **)malloc(nitems * sizeof(t_symbol *));
            if(attr->a_items)
            {
                attr->a_nitems  = nitems;
                memcpy(attr->a_items, items, nitems * sizeof(t_symbol *));
            }
            else
            {
                pd_error(attr, "can't allocate memory for attribute's items.");
            }
        }
    }
    else
    {
        if(attr->a_nitems && attr->a_items)
        {
            free(attr->a_items);
        }
        attr->a_items   = NULL;
        attr->a_nitems  = 0;
    }
}

static void eattr_setaccessors(t_eattr* attr, t_getter_method getter, t_setter_method setter)
{
    attr->a_getter = getter;
    attr->a_setter = setter;
}

static void eattr_shouldsave(t_eattr* attr, char saved)
{
    attr->a_saved = saved;
}

static void eattr_shouldrepaint(t_eattr* attr, char repaint)
{
    attr->a_paint = repaint;
}

static void eattr_shoulddisplay(t_eattr* attr, char visible)
{
    attr->a_invisible = !visible;
}








struct _eattrset
{
    t_object        s_object;   /*!< The object. */
    size_t          s_nattrs;   /*!< The number of attributes. */
    t_eattr**       s_attrs;    /*!< The attributes. */
};

static void eattrset_free(t_eattrset* attrset)
{
    size_t i = 0;
    if(attrset->s_nattrs && attrset->s_attrs)
    {
        for(i = 0; i < attrset->s_nattrs; i++)
        {
            eattr_free(attrset->s_attrs[i]);
        }
        free(attrset->s_attrs);
        attrset->s_attrs = NULL;
        attrset->s_nattrs  = 0;
    }
}

static t_class* eattrset_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eattrset1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eattrset"), (t_newmethod)NULL, (t_method)eattrset_free, sizeof(t_eattrset), CLASS_PD, A_NULL, 0);
        if(c)
        {
            obj = pd_new(c);
            pd_bind(obj, gensym("eattrset1572"));
        }
        else
        {
            error("can't initialize attribute set class.");
        }
        return c;
    }
    else
    {
        return *obj;
    }
}

t_eattrset* eattrset_create()
{
    t_eattrset* x = NULL;
    t_class*    c = eattrset_setup();
    if(c)
    {
        x = (t_eattrset *)pd_new(c2);
        if(x)
        {
            x->s_nattrs = 0;
            x->s_attrs  = NULL;
        }
    }
    return x;
}

t_eattrset* eattrset_findbyname(t_symbol* name)
{
    t_eattrset* x = NULL;
    t_class*    c = eattrset_setup();
    if(c)
    {
        x = (t_eattrset *)pd_findbyclass(name, c);
    }
    return x;
}

t_symbol* eattrset_getname(t_eattrset const* attrset)
{
    return attrset->s_name;
}

size_t eattrset_getnattrs(t_eattrset const* attrset)
{
    return attrset->s_nattrs;
}

void eattrset_getattrs(t_eattrset const* attrset, size_t* nattrs, t_eattr*** attrs)
{
    if(attrset->s_nattrs && attrset->s_attrs)
    {
        *attrs  = (t_eattr **)malloc(attrset->s_nattrs * sizeof(t_eattr *));
        if(*attrs)
        {
            memcpy(*attrs, attrset->s_attrs, attrset->s_nattrs * sizeof(t_eattr *));
            *nattrs = attrset->s_nattrs;
            return;
        }
    }
    *nattrs = 0;
    *attrs  = NULL;
}

t_eattr* eattrset_getattr(t_eattrset const* attrset, t_symbol const* name)
{
    size_t i = 0;
    for(i = 0; i < attrset->s_nattrs; i++)
    {
        if(eattr_getname(attrset->s_attrs[i] == name))
        {
            return attrset->s_attrs[i];
        }
    }
    return NULL;
}


size_t eattrset_getncategories(t_eattrset const* attrset)
{
    return attrset->s_ncates;
}

void eattrset_getcategories(t_eattrset const* attrset, size_t* ncates, t_symbol*** cates)
{
    if(attrset->s_ncates && attrset->s_cates)
    {
        *cates  = (t_symbol **)malloc(attrset->s_ncates * sizeof(t_symbol *));
        if(*cates)
        {
            memcpy(*cates, attrset->s_cates, attrset->s_ncates * sizeof(t_eattr *));
            *ncates = attrset->s_ncates;
            return;
        }
    }
    *s_ncates = 0;
    *cates    = NULL;
}

void eattrset_getcategory_attrs(t_eattrset const* attrset, t_symbol const* name, size_t* nattrs, t_object*** attrs)
{
    if(attrset->s_nattrs && attrset->s_attrs)
    {
        *attrs  = (t_object **)malloc(attrset->s_nattrs * sizeof(t_eattr *));
        if(*attrs)
        {
            memcpy(*attrs, attrset->s_attrs, attrset->s_nattrs * sizeof(t_eattr *));
            *nattrs = attrset->s_nattrs;
            return;
        }
    }
    *nattrs = 0;
    *attrs  = NULL;
}

t_eattr* eattrset_attr_new(t_eattrset* attrset, t_symbol* name, t_symbol* type, size_t size, size_t maxsize, size_t offset)
{
    size_t i = 0;
    t_eattr **temp = NULL, *newattr = NULL;
    if(eattrset_getattr(attrset, name))
    {
        pd_error(attrset, "attribute set already have the %s attribute.", name->s_name);
        return;
    }
    newattr = eattr_new(name, type, size, maxsize, offset);
    if(newattr)
    {
        if(attrset->s_nattrs && attrset->s_attrs)
        {
            temp = (t_eattr**)realloc(attrset->s_attrs, (attrset->s_nattrs + 1) * sizeof(t_eattr *));
            if(temp)
            {
                attrset->s_attrs = temp;
                attrset->s_attrs[attrset->s_nattrs] = newattr;
                attrset->s_nattrs++;
            }
            else
            {
                pd_error(attrset, "attribute set can't allocate memory for the new attribute %s.", name->s_name);
                return;
            }
        }
        else
        {
            attrset->s_attrs = (t_eattr**)malloc(sizeof(t_eattr *));
            if(attrset->s_attrs)
            {
                attrset->s_attrs[0] = newattr;
                attrset->s_nattrs = 1;
            }
            else
            {
                pd_error(attrset, "attribute set can't allocate memory for the new attribute %s.", name->s_name);
                return;
            }
        }
    }
    return newattr;
}

void eattrset_attr_default(t_eattrset* attrset, t_symbol* name, size_t ndefaults, t_atom* defaults)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setdefault(attr, ndefaults, defaults);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_category(t_eattrset* attrset, t_symbol* name, t_symbol* category)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setcategory(attr, category);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_order(t_eattrset* attrset, t_symbol* name, long order)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setorder(attr, order);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_flags(t_eattrset* attrset, t_symbol* name, long flags)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setflags(attr, flags);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_label(t_eattrset* attrset, t_symbol* name, t_symbol* label)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setlabel(attr, label);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_style(t_eattrset* attrset, t_symbol* name, t_symbol* style)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setstyle(attr, style);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_filter_min(t_eattrset* attrset, t_symbol* name, float value)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setmin(attr, value);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_filter_max(t_eattrset* attrset, t_symbol* name, float value)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setmax(attr, value);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_step(t_eattrset* attrset, t_symbol* name, float value)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setstep(attr, value);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_save(t_eattrset* attrset, t_symbol* name, char saved)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_shouldsave(attr, saved);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_paint(t_eattrset* attrset, t_symbol* name, char repaint)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_shouldrepaint(attr, repaint);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_visible(t_eattrset* attrset, t_symbol* name, char visible)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_shoulddisplay(attr, visible);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_accessor(t_eattrset* attrset, t_symbol* name, t_getter_method getter, t_setter_method setter)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setaccessors(attr, getter, setter);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}

void eattrset_attr_items(t_eattrset* attrset, t_symbol* name, size_t nitems, t_symbol** items)
{
    t_eattr *attr = eattrset_getattr(attrset, name);
    if(attr)
    {
        eattr_setitems(attr, nitems, items);
    }
    else
    {
        pd_error(attrset, "has no attribute %s.", name->s_name);
        return;
    }
}







