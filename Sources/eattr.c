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

struct _eattr
{
    t_object        a_obj;        /*!< The object. */
    t_symbol*       a_name;       /*!< The name of the attribute. */
    t_symbol*       a_type;       /*!< The type of the attribute (int, long, float, double, rgba, etc.). */
    t_symbol*       a_category;   /*!< The dummy category of the attribute. */
    t_symbol*       a_label;      /*!< The label of the attribute. */
    t_symbol*       a_style;      /*!< The style of the attribute (checkbutton, color, number, entry, menu). */
    char            a_save;       /*!< If the attribute should be saved. */
    char            a_paint;      /*!< If the attribute should repaint the t_ebox when it has changed. */
    char            a_invisible;  /*!< If the attribute is invisible. */
    long            a_order;      /*!< The order of the attribute. */
    long			a_flags;      /*!< The flags of the attribute. */
    long            a_offset;     /*!< The offset of the attribute in the object structure. */
    long            a_sizemax;    /*!< The maximum size of the attribute if the attribute is an array. */
    long            a_size;       /*!< The size of the attribute if the attribute is an array. */
    
    t_getter_method a_getter;     /*!< The getter method of the attribute. */
    t_setter_method a_setter;     /*!< The setter method of the attribute. */
    long            a_clipped;    /*!< If the attribute is clipped if it's value or an array of numerical values. */
    float           a_minimum;    /*!< The minimum value of the attribute. */
    float           a_maximum;    /*!< The maximum value of the attribute. */
    float           a_step;       /*!< The increment or decrement step calue of the attribute. */
    t_symbol*       a_defvals;    /*!< The default value of the attribute. */
    t_symbol**      a_items;      /*!< The available items of an attribute if it is a menu. */
    long            a_nitems;     /*!< The number of available items of an attribute if it is a menu. */
};

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

char eattr_shouldrepaint(t_eattr const* attr)
{
    return attr->a_save;
}

char eattr_isvisible(t_eattr const* attr)
{
    return !attr->a_invisible;
}

char eattr_issaved(t_eattr const* attr)
{
    return attr->a_save;
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

void eattr_getitems(t_eattr const*attr, long* nitems, t_symbol*** items)
{
    if(attr->a_nitems && attr->a_items)
    {
        *items = (t_symbol **)malloc((size_t)attr->a_nitems * sizeof(t_symbol *));
        if(*items)
        {
            memcpy(*items, attr->a_items, (size_t)attr->a_nitems * sizeof(t_symbol *));
            *nitems = attr->a_nitems;
            return;
        }
    }
    *nitems = 0;
    items   = NULL;
}

static void eattr_setdefault(t_eattr* x, t_symbol* value)
{
    x->a_defvals = value;
}

static void eattr_setcategory(t_eattr* x, t_symbol* category)
{
    x->a_category = category;
}

static void eattr_setlabel(t_eattr* x, t_symbol* label)
{
    x->a_label = label;
}

static void eattr_setstyle(t_eattr* x, t_symbol* style)
{
    if(style == s_cream_checkbutton || style == s_cream_onoff)
    {
        x->a_style = s_cream_checkbutton;
    }
    else if(style == s_cream_color)
    {
        x->a_style = s_cream_color;
    }
    else if(style == s_cream_number)
    {
        x->a_style = s_cream_number;
    }
    else if(style == s_cream_menu)
    {
        x->a_style = s_cream_menu;
    }
    else if(style == s_cream_font)
    {
        x->a_style = s_cream_font;
    }
    else
    {
        x->a_style = s_cream_entry;
    }
}

static void eattr_setorder(t_eattr* x, float order)
{
    x->a_order = (long)order;
}

static void eattr_setflags(t_eattr* x, float flags)
{
    x->a_flags = (long)flags;
}

static void eattr_setmin(t_eattr* x, float value)
{
    if(x->a_clipped == 0)
        x->a_clipped = 1;
    else if(x->a_clipped == 2)
        x->a_clipped = 3;
    x->a_minimum = value;
}

static void eattr_setmax(t_eattr* x, float value)
{
    if(x->a_clipped == 0)
        x->a_clipped = 2;
    else if(x->a_clipped == 1)
        x->a_clipped = 3;
    
    x->a_maximum = value;
}

static void eattr_free(t_eattr *x)
{
    if(x->a_itemssize && x->a_itemslist)
    {
        free(x->a_itemslist);
        x->a_itemslist = NULL;
        x->a_itemssize = 0;
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

t_eattr *eattr_new(t_symbol *name, t_symbol *type, long size, long maxsize, long flags, long offset)
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
            x->a_save       = 0;
            x->a_paint      = 0;
            x->a_invisible  = 0;
            x->a_order      = 0;
            x->a_flags      = flags;
            x->a_offset     = offset;
            x->a_size       = size;
            x->a_sizemax    = maxsize;
            x->a_getter     = NULL;
            x->a_setter     = NULL;
            x->a_clipped    = 0;
            x->a_minimum    = 0;
            x->a_maximum    = 1;
            x->a_step       = 1;
            x->a_defvals    = NULL;
            x->a_items      = NULL;
            x->a_nitems     = 0;
        }
        else
        {
            error("can't allocate attribute %s.", name->s_name);
        }
    }
    return x;
}

void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list)
{
    int j = 0;
    char* pch;
    int size = 0;
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        pch = strtok(gensym(list)->s_name," ,");
        
        while(pch != NULL)
        {
            pch = strtok(NULL, " ,");
            size++;
        }
        if(size > 0)
        {
            if(attr->itemssize)
            {
                attr->itemslist = (t_symbol **)realloc(attr->itemslist, (unsigned long)size * sizeof(t_symbol *));
                if(attr->itemslist)
                    attr->itemssize = size;
            }
            else
            {
                attr->itemslist = (t_symbol **)calloc((unsigned long)size, sizeof(t_symbol *));
                if(attr->itemslist)
                    attr->itemssize = size;
            }
            if(attr->itemslist && attr->itemssize)
            {
                pch = strtok(gensym(list)->s_name," ,");
                while(pch != NULL && (long)j < attr->itemssize)
                {
                    attr->itemslist[j] = gensym(pch);
                    pch = strtok(NULL, " ,");
                    j++;
                }
            }
            
        }
        else
        {
            if(attr->itemssize)
                free(attr->itemslist);
            attr->itemssize = 0;
        }
    }
}

static void eclass_attr_dosetdefault(t_object* x, t_eattr* attr)
{
    char *pch = NULL;
    const char* temp;
    int argc = 0;
    char* point;
    t_atom* argv = NULL;
    const size_t size = attr->sizemax ? ((size_t)attr->sizemax) : ((size_t)attr->size);
    if(attr->type == s_cream_font)
    {
        point = (char *)x + attr->offset;
        efont_init((t_efont *)point, gensym("DejaVu"), 0, 0, 11.f);
    }
    if(attr->defvals)
    {
        argv = (t_atom *)malloc(size* sizeof(t_atom));
        if(argv)
        {
            temp = attr->defvals->s_name;
            pch = estrtok(&temp, " ',\"", pch);
            while(pch != NULL)
            {
                if(isdigit(pch[0]))
                {
                    atom_setfloat(argv+argc, (float)atof(pch));
                    ++argc;
                }
                else if(isalpha(pch[0]))
                {
                    atom_setsym(argv+argc, gensym(pch));
                    ++argc;
                }
                if(argc > (int)size)
                {
                    break;
                }
                pch = estrtok(&temp, " ',\"", pch);
            }
            eclass_attr_setter(x, attr->name, argc, argv);
            free(argv);
        }
    }
}

void eclass_attr_setdefault(t_object* x, t_symbol *s)
{
    t_eattr* attr = eclass_getattr(eobj_getclass(x), s);
    if(attr)
    {
        eclass_attr_dosetdefault(x, attr);
    }
}

void eclass_attrs_setdefault(t_object* x)
{
    int i;
    t_eclass* c = eobj_getclass(x);
    for(i = 0; i < c->c_nattr; i++)
    {
        eclass_attr_dosetdefault(x, c->c_attr[i]);
    }
}

void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv)
{
    int j;
    char *point;
    t_symbol* type; t_symbol** syms;
    t_eattr* attr = eclass_getattr(eobj_getclass(x), s);
    *argv = NULL;
    *argc = 0;
    if(attr)
    {
        type = attr->type;
        if(attr->sizemax == 0)
        {
            argc[0] = (int)attr->size;
        }
        else
        {
            point = (char *)x + attr->size;
            argc[0] = (int)point[0];
            if(argc[0] > attr->sizemax)
            {
                argc[0] = (int)attr->sizemax;
            }
        }
        
        point = (char *)x + attr->offset;
        
        argv[0] = (t_atom *)calloc((size_t)argc[0], sizeof(t_atom));
        if(attr->getter)
        {
            attr->getter(x, attr, argc, argv);
        }
        else if(type == s_cream_char)
        {
            for(j = 0; j < argc[0]; j++)
            {
                atom_setlong(argv[0]+j, ((char *)point)[j]);
            }
        }
        else if(type == s_cream_int)
        {
            for(j = 0; j < argc[0]; j++)
            {
                atom_setlong(argv[0]+j, ((int *)point)[j]);
            }
        }
        else if(type == s_cream_long)
        {
            for(j = 0; j < argc[0]; j++)
            {
                atom_setlong(argv[0]+j, ((long *)point)[j]);
            }
        }
        else if(type == s_cream_float)
        {
            for(j = 0; j < argc[0]; j++)
            {
                atom_setfloat(argv[0]+j, ((float *)point)[j]);
            }
        }
        else if(type == s_cream_double)
        {
            for(j = 0; j < argc[0]; j++)
            {
                atom_setfloat(argv[0]+j, (float)(((double *)point)[j]));
            }
        }
        else if(type == s_cream_symbol)
        {
            syms = (t_symbol **)point;
            for(j = 0; j < argc[0]; j++)
            {
                atom_setsym(argv[0]+j, get_valid_symbol(syms[j]));
            }
        }
        else if(type == s_cream_atom)
        {
            for(j = 0; j < argc[0]; j++)
            {
                argv[0][j] = ((t_atom *)point)[j];
            }
        }
    }
}

void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
{
    long j, size;
    char *point;
    long *point_size;
    t_symbol* type;
    t_edrawparams params;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)eobj_getclass(x);
    int ac = 0; t_atom* av = NULL;
    t_eattr* attr = eclass_getattr(eobj_getclass(x), s);
    if(attr)
    {
        unparse_atoms(argc, argv, &ac, &av);
        
        type = attr->type;
        if(attr->sizemax == 0)
        {
            size = attr->size;
        }
        else
        {
            if(ac > attr->sizemax)
            {
                ac = (int)attr->sizemax;
            }
            size = ac;
            point = (char *)x + attr->size;
            point_size = (long *)point;
            point_size[0] = (long)size;
        }
        
        point = (char *)(x) + attr->offset;
        
        if(attr->clipped == 1 || attr->clipped == 3)
        {
            for(j = 0; j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    atom_setfloat(av+j, (float)pd_clip_min(atom_getfloat(av+j), attr->minimum));
                }
            }
        }
        if(attr->clipped == 2 || attr->clipped == 3)
        {
            for(j = 0; j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    atom_setfloat(av+j, (float)pd_clip_max(atom_getfloat(av+j), attr->maximum));
                }
            }
        }
        
        if(attr->setter)
        {
            attr->setter(x, attr, ac, av);
        }
        else if(type == s_cream_char)
        {
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    point[j] = (char)atom_getlong(av+j);
                }
            }
        }
        else if(type == s_cream_int)
        {
            int* pointor = (int *)point;
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    pointor[j] = (int)atom_getlong(av+j);
                }
            }
        }
        else if(type == s_cream_long)
        {
            long* pointor = (long *)point;
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    pointor[j] = (long)atom_getlong(av+j);
                }
            }
        }
        else if(type == s_cream_float)
        {
            float* pointor = (float *)point;
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    pointor[j] = (float)atom_getfloat(av+j);
                }
            }
        }
        else if(type == s_cream_double)
        {
            double* pointor = (double *)point;
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_FLOAT)
                {
                    pointor[j] = (double)atom_getfloat(av+j);
                }
            }
        }
        else if(type == s_cream_symbol)
        {
            t_symbol** pointor = (t_symbol **)point;
            for(j = 0; j < size && j < ac; j++)
            {
                if(atom_gettype(av+j) == A_SYMBOL)
                {
                    pointor[j] = atom_getsymbol(av+j);
                }
            }
        }
        else if(type == s_cream_atom)
        {
            t_atom* pointor = (t_atom *)point;
            for(j = 0; j < size && j < ac; j++)
            {
                pointor[j] = av[j];
            }
        }
        
        ebox_notify(z, s, s_cream_attr_modified, NULL, NULL);
        if(c->c_widget.w_notify != NULL)
        {
            c->c_widget.w_notify(x, s, s_cream_attr_modified, NULL, NULL);
        }
        if(attr->paint)
        {
            if(c->c_widget.w_oksize != NULL)
            {
                c->c_widget.w_oksize(x, &z->b_rect);
            }
            if(c->c_widget.w_getdrawparameters != NULL)
            {
                c->c_widget.w_getdrawparameters(x, NULL, &params);
                if(!rgba_is_equal(&(params.d_bordercolor), &(z->b_boxparameters.d_bordercolor)))
                {
                    memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
                    ebox_invalidate_layer((t_ebox *)x, s_cream_eboxbd);
                }
                else if(params.d_borderthickness != z->b_boxparameters.d_borderthickness)
                {
                    memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
                    ebox_notify(z, s_cream_size, s_cream_size, NULL, NULL);
                }
                else
                {
                    memcpy(&z->b_boxparameters, &params, sizeof(t_edrawparams));
                }
            }
            ebox_redraw(z);
        }
        if(attr->save && eobj_isbox(x) && !eobj_getcanvas(x)->gl_loading)
        {
            canvas_dirty(eobj_getcanvas(x), 1);
        }
        ewindowprop_update((t_eobj *)x);
        if(ac && av)
        {
            free(av);
        }
    }
}







typedef struct _eattrset
{
    t_object        s_object;   /*!< The object. */
    size_t          s_nattrs;   /*!< The number of attributes. */
    t_eattr**       s_attrs;    /*!< The attributes. */
} t_eattrset;

static t_class* eattrset_setup()
{
    t_class* c = NULL;
    t_pd* obj = gensym("eattrset1572")->s_thing;
    if(!obj)
    {
        c = class_new(gensym("eattrset"), (t_newmethod)NULL, (t_method)NULL, sizeof(t_eattrset), CLASS_PD, A_NULL, 0);
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

void eattrset_free(t_eattrset* attrset)
{
    pd_unbind((t_pd *)attrset, attrset->s_name);
    if(attrset->s_nattrs && attrset->s_attrs)
    {
        free(attrset->s_attrs);
        attrset->s_attrs = NULL;
        attrset->s_nattrs  = 0;
    }
    pd_free((t_pd *)attrset);
}

t_symbol* eattrset_getname(t_eattrset const* attrset)
{
    return attrset->s_name;
}

size_t eattrset_getnattrs(t_eattrset const* attrset)
{
    return attrset->s_nattrs;
}

void eclass_getattrs(t_eattrset const* attrset, size_t* nattrs, t_object*** attrs)
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

size_t eattrset_getncategories(t_eattrset const* attrset)
{
    return attrset->s_ncates;
}

void eclass_getcategories(t_eattrset const* attrset, size_t* ncates, t_symbol*** cates)
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

void eclass_getcategory_attrs(t_eattrset const* attrset, t_symbol const* name, size_t* nattrs, t_object*** attrs)
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







