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

static void ewidget_init(t_eclass* c);

t_eclass* eclass_new(const char *name, t_typ_method newm, t_typ_method freem, size_t size, int flags, t_atomtype arg1, int arg2)
{
    char help[MAXPDSTRING];
    t_class *pd  = class_new(gensym(name), (t_newmethod)newm, (t_method)freem, size, flags, arg1, arg2);
    t_eclass* c  = (t_eclass *)resizebytes(pd, sizeof(*pd), sizeof(*c));
    if(c)
    {
        epd_init();
        c->c_nattr = 0;
        c->c_dsp   = 0;
        c->c_box   = 0;
        c->c_attr  = NULL;
        sprintf(help, "helps/%s", c->c_class.c_name->s_name);
        class_sethelpsymbol((t_class *)c, gensym(help));
    }
    else
    {
        bug("Memory allocation failed for the class %s.", name);
    }
    return c;
}

void eclass_init(t_eclass* c, long flags)
{
    eclass_guiinit(c, flags);
}

void eclass_guiinit(t_eclass* c, long flags)
{
    ewidget_init(c);
    c->c_box = 1;
    
    CLASS_ATTR_FLOAT_ARRAY  (c, "size", 0, t_ebox, b_rect.width, 2);
    CLASS_ATTR_DEFAULT      (c, "size", 0, "100. 100.");
    CLASS_ATTR_FILTER_MIN   (c, "size", 4);
    CLASS_ATTR_SAVE         (c, "size", 0);
    CLASS_ATTR_PAINT        (c, "size", 0);
    CLASS_ATTR_CATEGORY		(c, "size", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "size", 0, "Patching Size");
    CLASS_ATTR_ACCESSORS    (c, "size", NULL, (t_err_method)ebox_size_set);
    
    CLASS_ATTR_INT          (c, "pinned", 0, t_ebox, b_pinned);
    CLASS_ATTR_DEFAULT      (c, "pinned", 0, "0");
    CLASS_ATTR_FILTER_CLIP  (c, "pinned", 0, 1);
    CLASS_ATTR_SAVE         (c, "pinned", 0);
    CLASS_ATTR_CATEGORY		(c, "pinned", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "pinned", 0, "Pinned");
    CLASS_ATTR_STYLE        (c, "pinned", 0, "onoff");
    
    CLASS_ATTR_SYMBOL       (c, "receive", 0, t_ebox, b_receive_id);
    CLASS_ATTR_DEFAULT      (c, "receive", 0, "");
    CLASS_ATTR_ACCESSORS    (c, "receive", NULL, ebox_set_receiveid);
    CLASS_ATTR_SAVE         (c, "receive", 0);
    CLASS_ATTR_CATEGORY		(c, "receive", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "receive", 0, "Receive Symbol");
    
    CLASS_ATTR_SYMBOL       (c, "send", 0, t_ebox, b_send_id);
    CLASS_ATTR_DEFAULT      (c, "send", 0, "");
    CLASS_ATTR_ACCESSORS    (c, "send", NULL, ebox_set_sendid);
    CLASS_ATTR_SAVE         (c, "send", 0);
    CLASS_ATTR_CATEGORY		(c, "send", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "send", 0, "Send Symbol");
    
    if(flags & EBOX_TEXTFIELD)
    {
        class_addmethod((t_class *)c, (t_method)ebox_texteditor_keypress, gensym("texteditor_keypress"), A_SYMBOL,A_DEFFLOAT,0);
        class_addmethod((t_class *)c, (t_method)ebox_texteditor_keyfilter, gensym("texteditor_keyfilter"), A_SYMBOL,A_DEFFLOAT,0);
    }
    
    // GUI always needs this methods // 
    class_addmethod((t_class *)c, (t_method)ebox_mouse_enter, gensym("mouseenter"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_leave, gensym("mouseleave"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_move,  gensym("mousemove"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_down,  gensym("mousedown"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_up,    gensym("mouseup"),    A_GIMME, 0);
    
    class_addmethod((t_class *)c, (t_method)ebox_pos, gensym("pos"), A_DEFFLOAT,A_DEFFLOAT,0);
    class_addmethod((t_class *)c, (t_method)ebox_vis, gensym("vis"), A_DEFFLOAT,0);
    class_addmethod((t_class *)c, (t_method)ebox_set_parameter_attribute, gensym("param"), A_GIMME, 0);
    
    class_setwidget((t_class *)&c->c_class, (t_widgetbehavior *)&c->c_widget);
    class_setsavefn((t_class *)&c->c_class, (t_savefn)eobj_save);
}

void eclass_dspinit(t_eclass* c)
{
    c->c_dsp = 1;
    class_addmethod((t_class *)c, (t_method)eobj_dsp, gensym("dsp"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

static t_pd_err is_cicm(t_eobj* x)
{
    return 1;
}

t_pd_err eclass_register(t_symbol *name, t_eclass *c)
{
    if(c->c_box && c->c_dsp)
    {
        c->c_class.c_floatsignalin = calcoffset(t_edspbox, d_dsp.d_float);
    }
    else
    {
        c->c_class.c_floatsignalin = calcoffset(t_edspobj, d_dsp.d_float);
    }
    
    class_setpropertiesfn((t_class *)c, (t_propertiesfn)eobj_create_properties_window);
    class_addmethod((t_class *)c, (t_method)is_cicm, s_iscicm, A_NULL, 0);
    return 0;
}

void eclass_addmethod(t_eclass* c, t_typ_method m, const char* name, t_atomtype type, long dummy)
{
    if(gensym(name) == gensym("mouseenter"))
    {
        c->c_widget.w_mouseenter = m;
    }
    else if(gensym(name) == gensym("mouseleave"))
    {
        c->c_widget.w_mouseleave = m;
    }
    else if(gensym(name) == gensym("mousemove"))
    {
        c->c_widget.w_mousemove = m;
    }
    else if(gensym(name) == gensym("mousedown"))
    {
        c->c_widget.w_mousedown = m;
    }
    else if(gensym(name) == gensym("mousedrag"))
    {
        c->c_widget.w_mousedrag = m;
    }
    else if(gensym(name) == gensym("mouseup"))
    {
        c->c_widget.w_mouseup = m;
    }
    else if(gensym(name) == gensym("mousewheel"))
    {
        class_addmethod((t_class *)c, (t_method)ebox_mouse_wheel, gensym("mousewheel"), A_GIMME, 0);
        c->c_widget.w_mousewheel = m;
    }
    else if(gensym(name) == gensym("dblclick"))
    {
        class_addmethod((t_class *)c, (t_method)ebox_mouse_dblclick, gensym("dblclick"),   A_GIMME, 0);
        c->c_widget.w_dblclick = m;
    }
    else if(gensym(name) == gensym("key") || gensym(name) == gensym("keyfilter"))
    {
        if(c->c_widget.w_key == NULL && c->c_widget.w_keyfilter == NULL)
            class_addmethod((t_class *)c, (t_method)ebox_key,         gensym("key"),        A_GIMME, 0);
        if(gensym(name) == gensym("key"))
            c->c_widget.w_key = m;
        if(gensym(name) == gensym("keyfilter"))
            c->c_widget.w_keyfilter = m;
    }
    else if(gensym(name) == gensym("paint"))
    {
        c->c_widget.w_paint = m;
    }
    else if(gensym(name) == gensym("notify"))
    {
        c->c_widget.w_notify = (t_err_method)m;
    }
    else if(gensym(name) == gensym("getdrawparams"))
    {
        c->c_widget.w_getdrawparameters = m;
    }
    else if(gensym(name) == gensym("oksize"))
    {
        c->c_widget.w_oksize = m;
    }
    else if(gensym(name) == gensym("save") || gensym(name) == gensym("jsave"))
    {
        c->c_widget.w_save = m;
    }
    else if(gensym(name) == gensym("popup"))
    {
        class_addmethod((t_class *)c, (t_method)eobj_popup, gensym("popup"),  A_SYMBOL, A_DEFFLOAT, 0);
        c->c_widget.w_popup = m;
    }
    else if(gensym(name) == gensym("dsp") || gensym(name) == gensym("dsp64"))
    {
        c->c_widget.w_dsp = m;
    }
    else if(gensym(name) == &s_bang)
    {
        class_addbang((t_class *)c, m);
    }
    else if(gensym(name) == &s_float)
    {
        class_addfloat((t_class *)c, m);
    }
    else if(gensym(name) == &s_list)
    {
        class_addlist((t_class *)c, m);
    }
    else if(gensym(name) == &s_anything)
    {
        class_addanything((t_class *)c, m);
    }
    else if(gensym(name) == &s_symbol)
    {
        class_addsymbol((t_class *)c, m);
    }
    else if(gensym(name) == gensym("preset"))
    {
        int remove_preset;
        CLASS_ATTR_SYMBOL       (c, "presetname", 0, t_ebox, b_preset_id);
        CLASS_ATTR_DEFAULT      (c, "presetname", 0, "");
        CLASS_ATTR_SAVE         (c, "presetname", 0);
        CLASS_ATTR_CATEGORY		(c, "presetname", 0, "Basic");
        CLASS_ATTR_LABEL		(c, "presetname", 0, "Preset Name");
        CLASS_ATTR_ACCESSORS    (c, "presetname", NULL, ebox_set_presetid);
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, 0);
    }
    else if(gensym(name) == gensym("write"))
    {        
        class_addmethod((t_class *)c, (t_method)eobj_write, gensym(name), type, 0);
        class_addmethod((t_class *)c, (t_method)eobj_write, gensym("eobjwriteto"), type, 0);
        c->c_widget.w_write = m;
    }
    else if(gensym(name) == gensym("read"))
    {
        class_addmethod((t_class *)c, (t_method)eobj_read, gensym(name), type, 0);
        class_addmethod((t_class *)c, (t_method)eobj_read, gensym("eobjreadfrom"), type, 0);
        c->c_widget.w_read = m;
    }
    else if(gensym(name) == gensym("texteditor_keypress"))
    {
        c->c_widget.w_texteditor_keypress = m;
    }
    else if(gensym(name) == gensym("texteditor_keyfilter"))
    {
        c->c_widget.w_texteditor_keyfilter = m;
    }
    else
    {
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, 0);
    }
}

static t_eattr* eclass_getattr(t_eclass const* c, t_symbol const* name)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == name)
        {
            return c->c_attr[i];
        }
    }
    return NULL;
}

void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset)
{
    t_eattr* attr;
    t_eattr** attrs;
    char getattr[MAXPDSTRING];
    attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        error("%s already have %s attribute.", c->c_class.c_name->s_name, attrname);
        return ;
    }
    else if(size >= 1)
    {
        attr = (t_eattr*)malloc(sizeof(t_eattr));
        if(attr)
        {
            attr->name = gensym(attrname);
            attr->type = gensym(type);
            attr->category = c->c_class.c_name;
            attr->label = s_cream_empty;
            attr->style = s_cream_entry;
            attr->order = c->c_nattr+1;
            attr->save  = 0;
            attr->paint = 0;
            attr->invisible = 0;
            attr->flags     = flags;
            attr->offset    = offset;
            attr->size      = size;
            attr->sizemax   = maxsize;
            attr->getter    = NULL;
            attr->setter    = NULL;
            attr->clipped   = 0;
            attr->minimum   = 0;
            attr->maximum   = 1;
            attr->step      = 1;
            attr->defvals   = NULL;
            attr->itemslist = NULL;
            attr->itemssize = 0;
            
            attrs = (t_eattr **)realloc(c->c_attr, (size_t)(c->c_nattr + 1) * sizeof(t_eattr *));
            if(attrs)
            {
                c->c_attr = attrs;
                c->c_attr[c->c_nattr] = attr;
                class_addmethod((t_class *)c, (t_method)eclass_attr_setter, gensym(attrname), A_GIMME, 0);
                sprintf(getattr, "get%s", attrname);
                class_addmethod((t_class *)c, (t_method)eclass_attr_getter, gensym(getattr), A_GIMME, 0);
                c->c_nattr++;
            }
            else
            {
                error("%s can't increase memory for %s attribute.", c->c_class.c_name->s_name, attrname);
            }
            
        }
        else
        {
             error("%s can't allocate memory for %s attribute.", c->c_class.c_name->s_name, attrname);
        }
    }
    else
    {
        error("%s %s attribute size is null.", c->c_class.c_name->s_name, attrname);
    }
}

void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->defvals = gensym(value);
    }
}

void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->category = gensym(category);
    }
}

void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr && isdigit(order[0]))
    {
        attr->order = atoi(order);
    }
}

void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->label = gensym(label);
    }
}

void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style)
{
    t_symbol const* s_style = gensym(style);
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        if(s_style == s_cream_checkbutton || s_style == s_cream_onoff)
        {
            attr->style = s_cream_checkbutton;
        }
        else if(s_style == s_cream_color)
        {
            attr->style = s_cream_color;
        }
        else if(s_style == s_cream_number)
        {
            attr->style = s_cream_number;
        }
        else if(s_style == s_cream_menu)
        {
            attr->style = s_cream_menu;
        }
        else if(s_style == s_cream_font)
        {
            attr->style = s_cream_font;
        }
        else
        {
            attr->style = s_cream_entry;
        }
    }
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

void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        if(attr->clipped == 0)
            attr->clipped = 1;
        else if(attr->clipped == 2)
            attr->clipped = 3;
        
        attr->minimum = value;
    }
}

void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        if(attr->clipped == 0)
            attr->clipped = 2;
        else if(attr->clipped == 1)
            attr->clipped = 3;
        
        attr->maximum = value;
    }
}

void eclass_attr_step(t_eclass* c, const char* attrname, float value)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->step = value;
    }
}

void eclass_attr_save(t_eclass* c, const char* attrname, long flags)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->save = 1;
    }
}

void eclass_attr_paint(t_eclass* c, const char* attrname, long flags)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->paint = 1;
    }
}

void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->invisible = 1;
    }
}

void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter)
{
    t_eattr* attr = eclass_getattr(c, gensym(attrname));
    if(attr)
    {
        attr->getter = getter;
        attr->setter = setter;
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
    *argv = NULL;
    *argc = 0;
    t_symbol* type; t_efont* font; t_symbol** syms;
    t_eattr* attr = eclass_getattr(eobj_getclass(x), s);
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
        else if(type == s_cream_font)
        {
            font = (t_efont *)point;
            atom_setsym(argv[0], font->family);
            atom_setfloat(argv[0]+1, font->size);
            atom_setsym(argv[0]+2, font->slant);
            atom_setsym(argv[0]+3, font->weight);
        }
    }
}

void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
{
    long j, size;
    char *point;
    long *point_size;
    t_symbol* type;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->b_obj.o_obj.te_g.g_pd;
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
        else if(type == s_cream_font)
        {
            t_efont* pointor = (t_efont *)point;
            if(ac && atom_gettype(av) == A_SYMBOL)
            {
                pointor->family = atom_getsymbol(av);
            }
            if(ac > 1 && atom_gettype(av+1) == A_FLOAT)
                pointor->size   = atom_getfloat(av+1);
            if(ac > 2 && atom_gettype(av+2) == A_SYMBOL)
            {
                if(atom_getsymbol(av+2) == gensym("bold"))
                {
                    pointor->weight = atom_getsymbol(av+2);
                    if(ac > 3 && atom_gettype(av+3) == A_SYMBOL &&  atom_getsymbol(av+3) == gensym("italic"))
                    {
                        pointor->slant = atom_getsymbol(av+3);
                    }
                    else
                    {
                        pointor->slant = gensym("roman");
                    }
                }
                else if(atom_getsymbol(av+2) == gensym("italic"))
                {
                    pointor->slant = atom_getsymbol(av+2);
                    if(ac > 3 && atom_gettype(av+3) == A_SYMBOL &&  atom_getsymbol(av+3) == gensym("bold"))
                    {
                        pointor->weight = atom_getsymbol(av+3);
                    }
                    else
                    {
                        pointor->weight = gensym("roman");
                    }
                }
                else
                {
                    pointor->weight = gensym("normal");
                    pointor->slant = gensym("roman");
                }
            }
            else
            {
                pointor->weight = gensym("normal");
                pointor->slant = gensym("roman");
            }
        }
        
        ebox_notify(z, s, s_cream_attr_modified, NULL, NULL);
        if(c->c_widget.w_notify != NULL)
        {
            c->c_widget.w_notify(x, s, s_cream_attr_modified, NULL, NULL);
        }
        if(type == s_cream_font && eobj_isbox(x) && z->b_flags & EBOX_FONTSIZE)
        {
            eobj_attr_setvalueof(x, s_cream_size, 0, NULL);
        }
        if(attr->paint)
        {
            if(c->c_widget.w_oksize != NULL)
            {
                c->c_widget.w_oksize(x, &z->b_rect);
            }
            if(c->c_widget.w_getdrawparameters != NULL)
            {
                c->c_widget.w_getdrawparameters(x, NULL, &z->b_boxparameters);
            }
            ebox_redraw(z);
        }
        if(attr->save && eobj_isbox(x) && ebox_isdrawable((t_ebox*) x))
        {
            canvas_dirty(eobj_getcanvas(x), 1);
        }
        
        if(ac && av)
        {
            free(av);
        }
    }
}

static void ewidget_init(t_eclass* c)
{
    c->c_widget.w_getrectfn         = ebox_wgetrect;
    c->c_widget.w_visfn             = ebox_wvis;
    c->c_widget.w_displacefn        = ebox_wdisplace;
    c->c_widget.w_selectfn          = ebox_wselect;
    c->c_widget.w_activatefn        = NULL;
    c->c_widget.w_deletefn          = ebox_wdelete;
    c->c_widget.w_clickfn           = NULL;
    
    c->c_widget.w_paint             = NULL;
    c->c_widget.w_mouseenter        = NULL;
    c->c_widget.w_mouseleave        = NULL;
    c->c_widget.w_mousemove         = NULL;
    c->c_widget.w_mousedown         = NULL;
    c->c_widget.w_mousedrag         = NULL;
    c->c_widget.w_mouseup           = NULL;
    c->c_widget.w_mousewheel        = NULL;
    c->c_widget.w_dblclick          = NULL;
    c->c_widget.w_key               = NULL;
    c->c_widget.w_keyfilter         = NULL;
    c->c_widget.w_getdrawparameters = NULL;
    c->c_widget.w_notify            = NULL;
    c->c_widget.w_save              = NULL;
    c->c_widget.w_popup             = NULL;
    c->c_widget.w_dsp               = NULL;
    c->c_widget.w_oksize            = NULL;
    c->c_widget.w_write             = NULL;
    c->c_widget.w_read              = NULL;
    c->c_widget.w_texteditor_keypress = NULL;
    c->c_widget.w_texteditor_keyfilter= NULL;
}




