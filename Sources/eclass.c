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

static void eclass_properties_dialog(t_eclass* c);
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

    // DEFAULT ATTRIBUTES //
    CLASS_ATTR_FLOAT_ARRAY  (c, "size", 0, t_ebox, b_rect.width, 2);
    CLASS_ATTR_SYMBOL       (c, "fontname", 0, t_ebox, b_font.c_family);
    CLASS_ATTR_SYMBOL       (c, "fontweight", 0, t_ebox, b_font.c_weight);
    CLASS_ATTR_SYMBOL       (c, "fontslant", 0, t_ebox, b_font.c_slant);
    CLASS_ATTR_LONG         (c, "fontsize", 0, t_ebox, b_font.c_sizereal);
    CLASS_ATTR_SYMBOL       (c, "receive", 0, t_ebox, b_receive_id);
    CLASS_ATTR_SYMBOL       (c, "send", 0, t_ebox, b_send_id);

    CLASS_ATTR_DEFAULT      (c, "size", 0, "100. 100.");
    CLASS_ATTR_FILTER_MIN   (c, "size", 4);
    CLASS_ATTR_SAVE         (c, "size", 0);
    CLASS_ATTR_PAINT        (c, "size", 0);
    CLASS_ATTR_CATEGORY		(c, "size", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "size", 0, "Patching Size");
    CLASS_ATTR_ACCESSORS    (c, "size", NULL, (t_err_method)ebox_size_set);

    CLASS_ATTR_DEFAULT      (c, "fontname", 0, "Helvetica");
    CLASS_ATTR_SAVE         (c, "fontname", 0);
    CLASS_ATTR_PAINT        (c, "fontname", 0);
    CLASS_ATTR_CATEGORY		(c, "fontname", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontname", 0, "Font Name");
    CLASS_ATTR_ACCESSORS    (c, "fontname", NULL, (t_err_method)ebox_set_font);
    CLASS_ATTR_STYLE        (c, "fontname", 0, "menu");
    CLASS_ATTR_ITEMS        (c, "fontname", 0, "Helvetica Monaco Courier Times DejaVu");

    CLASS_ATTR_DEFAULT      (c, "fontweight", 0, "normal");
    CLASS_ATTR_SAVE         (c, "fontweight", 0);
    CLASS_ATTR_PAINT        (c, "fontweight", 0);
    CLASS_ATTR_CATEGORY		(c, "fontweight", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontweight", 0, "Font Weight");
    CLASS_ATTR_ACCESSORS    (c, "fontweight", NULL, (t_err_method)ebox_set_fontweight);
    CLASS_ATTR_STYLE        (c, "fontweight", 0, "menu");
    CLASS_ATTR_ITEMS        (c, "fontweight", 0, "normal bold");

    CLASS_ATTR_DEFAULT      (c, "fontslant", 0, "roman");
    CLASS_ATTR_SAVE         (c, "fontslant", 0);
    CLASS_ATTR_PAINT        (c, "fontslant", 0);
    CLASS_ATTR_CATEGORY		(c, "fontslant", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontslant", 0, "Font Slant");
    CLASS_ATTR_ACCESSORS    (c, "fontslant", NULL, (t_err_method)ebox_set_fontslant);
    CLASS_ATTR_STYLE        (c, "fontslant", 0, "menu");
    CLASS_ATTR_ITEMS        (c, "fontslant", 0, "roman italic");

    CLASS_ATTR_DEFAULT      (c, "fontsize", 0, "11");
    CLASS_ATTR_FILTER_MIN   (c, "fontsize", 4);
    CLASS_ATTR_SAVE         (c, "fontsize", 0);
    CLASS_ATTR_PAINT        (c, "fontsize", 0);
    CLASS_ATTR_CATEGORY		(c, "fontsize", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontsize", 0, "Font Size");
    CLASS_ATTR_ACCESSORS    (c, "fontsize", NULL, ebox_set_fontsize);
    CLASS_ATTR_STYLE        (c, "fontsize", 0, "number");

    CLASS_ATTR_DEFAULT      (c, "receive", 0, "(null)");
    CLASS_ATTR_ACCESSORS    (c, "receive", NULL, ebox_set_receiveid);
    CLASS_ATTR_SAVE         (c, "receive", 0);
    CLASS_ATTR_CATEGORY		(c, "receive", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "receive", 0, "Receive Symbol");

    CLASS_ATTR_DEFAULT      (c, "send", 0, "(null)");
    CLASS_ATTR_ACCESSORS    (c, "send", NULL, ebox_set_sendid);
    CLASS_ATTR_SAVE         (c, "send", 0);
    CLASS_ATTR_CATEGORY		(c, "send", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "send", 0, "Send Symbol");

    // GUI always need this methods //
    class_addmethod((t_class *)c, (t_method)ebox_attrprint,         gensym("attrprint"),    A_NULL,  0);
    class_addmethod((t_class *)c, (t_method)ebox_dialog,            gensym("dialog"),       A_GIMME, 0);

    class_addmethod((t_class *)c, (t_method)ebox_mouse_enter, gensym("mouseenter"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_leave, gensym("mouseleave"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_move,  gensym("mousemove"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_down,  gensym("mousedown"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_up,    gensym("mouseup"),    A_GIMME, 0);

    class_addmethod((t_class *)c, (t_method)ebox_pos, gensym("pos"), A_DEFFLOAT,A_DEFFLOAT,0);
    class_addmethod((t_class *)c, (t_method)ebox_vis, gensym("vis"), A_DEFFLOAT,0);

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

    if(c->c_nattr)
    {
        eclass_properties_dialog(c);
        class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
    }
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
        class_addmethod((t_class *)c, (t_method)eobj_popup,       gensym("popup"),  A_SYMBOL, A_DEFFLOAT, 0);
        c->c_widget.w_popup = m;
    }
    else if(gensym(name) == gensym("dsp") || gensym(name) == gensym("dsp64"))
    {
        c->c_widget.w_dsp = m;
    }
    else if(gensym(name) == gensym("bang"))
    {
        class_addbang((t_class *)c, m);
    }
    else if(gensym(name) == &s_float)
    {
        class_addfloat((t_class *)c, m);
    }
    else if(gensym(name) == gensym("list"))
    {
        class_addlist((t_class *)c, m);
    }
    else if(gensym(name) == gensym("anything"))
    {
        class_addanything((t_class *)c, m);
    }
    else if(gensym(name) == &s_symbol)
    {
        class_addsymbol((t_class *)c, m);
    }
    else if(gensym(name) == gensym("preset"))
    {
        CLASS_ATTR_SYMBOL       (c, "presetname", 0, t_ebox, b_objpreset_id);
        CLASS_ATTR_DEFAULT      (c, "presetname", 0, "(null)");
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
    else
    {
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, 0);
    }
}

void eclass_new_attr_typed(t_eclass* c, const char* attrname, const char* type, long size, long maxsize, long flags, long offset)
{
    int i;
    t_eattr* attr;
    t_eattr** attrs;
    char getattr[MAXPDSTRING];
    if(size >= 1)
    {
        for(i = 0; i < c->c_nattr; i++)
        {
            if(c->c_attr[i]->name == gensym(attrname))
            {
                error("%s already have %s attribute.", c->c_class.c_name->s_name, attrname);
                return ;
            }
        }
        attr = malloc(sizeof(t_eattr));
        if(attr)
        {
            attr->name = gensym(attrname);
            attr->type = gensym(type);
            attr->category = c->c_class.c_name;
            attr->label = gensym("");
            attr->style = gensym("entry");
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
                eclass_attr_label(c, attrname, 0, attrname);
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
        error("%s %s attribute size is too null.", c->c_class.c_name->s_name, attrname);
    }
}

void eclass_attr_default(t_eclass* c, const char* attrname, long flags, const char* value)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->defvals = gensym(value);
            return ;
        }
    }
}

void eclass_attr_category(t_eclass* c, const char* attrname, long flags, const char* category)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->category = gensym(category);
            return ;
        }
    }
}

void eclass_attr_order(t_eclass* c, const char* attrname, long flags, const char* order)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(isdigit(atoi(order)))
                c->c_attr[i]->order = atoi(order);
            return ;
        }
    }
}

void eclass_attr_label(t_eclass* c, const char* attrname, long flags, const char* label)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->label = gensym(label);
            return ;
        }
    }
}

void eclass_attr_style(t_eclass* c, const char* attrname, long flags, const char* style)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(gensym(style) == gensym("checkbutton") || gensym(style) == gensym("onoff"))
            {
                c->c_attr[i]->style = gensym("checkbutton");
            }
            else if(gensym(style) == gensym("rgb") || gensym(style) == gensym("rgba") || gensym(style) == gensym("color"))
            {
                c->c_attr[i]->style = gensym("color");
            }
            else if(gensym(style) == gensym("spinbox") || gensym(style) == gensym("number"))
            {
                c->c_attr[i]->style = gensym("number");
            }
            else if(gensym(style) == gensym("menu"))
            {
                c->c_attr[i]->style = gensym("menu");
            }
            else
            {
                c->c_attr[i]->style = gensym("entry");
            }
            return ;
        }
    }
}

void eclass_attr_itemlist(t_eclass* c, const char* attrname, long flags, const char* list)
{
    int i, j = 0;
    char* pch;
    int size = 0;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            pch = strtok(gensym(list)->s_name," ,");

            while(pch != NULL)
            {
                pch = strtok(NULL, " ,");
                size++;
            }
            if(size > 0)
            {
                if(c->c_attr[i]->itemssize)
                {
                    c->c_attr[i]->itemslist = (t_symbol **)realloc(c->c_attr[i]->itemslist, (unsigned long)size * sizeof(t_symbol *));
                    if(c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                }
                else
                {
                    c->c_attr[i]->itemslist = (t_symbol **)calloc((unsigned long)size, sizeof(t_symbol *));
                    if(c->c_attr[i]->itemslist)
                        c->c_attr[i]->itemssize = size;
                }
                if(c->c_attr[i]->itemslist && c->c_attr[i]->itemssize)
                {
                    pch = strtok(gensym(list)->s_name," ,");
                    while(pch != NULL && (long)j < c->c_attr[i]->itemssize)
                    {
                        c->c_attr[i]->itemslist[j] = gensym(pch);
                        pch = strtok(NULL, " ,");
                        j++;
                    }
                }

            }
            else
            {
                if(c->c_attr[i]->itemssize)
                    free(c->c_attr[i]->itemslist);
                c->c_attr[i]->itemssize = 0;
            }
            return ;
        }
    }
}

void eclass_attr_filter_min(t_eclass* c, const char* attrname, float value)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 1;
            else if(c->c_attr[i]->clipped == 2)
                c->c_attr[i]->clipped = 3;

            c->c_attr[i]->minimum = value;
            return ;
        }
    }
}

void eclass_attr_filter_max(t_eclass* c, const char* attrname, float value)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            if(c->c_attr[i]->clipped == 0)
                c->c_attr[i]->clipped = 2;
            else if(c->c_attr[i]->clipped == 1)
                c->c_attr[i]->clipped = 3;

            c->c_attr[i]->maximum = value;
            return ;
        }
    }
}

void eclass_attr_step(t_eclass* c, const char* attrname, float value)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->step = value;
            return ;
        }
    }
}

void eclass_attr_save(t_eclass* c, const char* attrname, long flags)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->save = 1;
            return ;
        }
    }
}

void eclass_attr_paint(t_eclass* c, const char* attrname, long flags)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->paint = 1;
            return ;
        }
    }
}

void eclass_attr_invisible(t_eclass* c, const char* attrname, long flags)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->invisible = 1;
            return ;
        }
    }
}

void eclass_attr_accessor(t_eclass* c, const char* attrname, t_err_method getter, t_err_method setter)
{
    int i;
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == gensym(attrname))
        {
            c->c_attr[i]->getter = getter;
            c->c_attr[i]->setter = setter;
            return ;
        }
    }
}

void eclass_attr_getter(t_object* x, t_symbol *s, int* argc, t_atom** argv)
{
    int i, j;
    char *point;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->b_obj.o_obj.te_g.g_pd;

    // if argc is null, this might have been wrongly called from patcher
    if(!argc)
      return;

    if(argv[0] && argc[0])
        free(argv);
    argc[0] = 0;

    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == s)
        {
            t_symbol* type = c->c_attr[i]->type;
            if(c->c_attr[i]->sizemax == 0)
                argc[0] = (int)c->c_attr[i]->size;
            else
            {
                point = (char *)x + c->c_attr[i]->size;
                argc[0] = (int)point[0];
                if(argc[0] > c->c_attr[i]->sizemax)
                    argc[0] = (int)c->c_attr[i]->sizemax;
            }

            point = (char *)x + c->c_attr[i]->offset;

            argv[0] = (t_atom *)calloc((size_t)argc[0], sizeof(t_atom));
            if(c->c_attr[i]->getter)
            {
                c->c_attr[i]->getter(x, c->c_attr[i], argc, argv);
            }
            else if(type == s_int)
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setlong(argv[0]+j, ((int *)point)[j]);
                }
            }
            else if(type == s_long)
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setlong(argv[0]+j, ((long *)point)[j]);
                }
            }
            else if(type == &s_float)
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setfloat(argv[0]+j, ((float *)point)[j]);
                }
            }
            else if(type == s_double)
            {
                for(j = 0; j < argc[0]; j++)
                {
                    atom_setfloat(argv[0]+j, (float)(((double *)point)[j]));
                }
            }
            else if(type == &s_symbol)
            {
                t_symbol** syms = (t_symbol **)point;
                for(j = 0; j < argc[0]; j++)
                {
                    if(syms[j])
                    {
                        atom_setsym(argv[0]+j, gensym(syms[j]->s_name));
                    }
                }
            }
            else if(type == s_atom)
            {
                for(j = 0; j < argc[0]; j++)
                {
                    argv[0][j] = ((t_atom *)point)[j];
                }
            }
        }
    }
}

void eclass_attr_setter(t_object* x, t_symbol *s, int argc, t_atom *argv)
{
    long i, j, size;
    char *point;
    long *point_size;
    t_ebox* z   = (t_ebox *)x;
    t_eclass* c = (t_eclass *)z->b_obj.o_obj.te_g.g_pd;

    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->name == s)
        {
            t_symbol* type = c->c_attr[i]->type;
            if(c->c_attr[i]->sizemax == 0)
            {
                size = c->c_attr[i]->size;
            }
            else
            {
                if(argc > c->c_attr[i]->sizemax)
                {
                    argc = (int)c->c_attr[i]->sizemax;
                }
                size = argc;
                point = (char *)x + c->c_attr[i]->size;
                point_size = (long *)point;
                point_size[0] = (long)size;
            }

            point = (char *)(x) + c->c_attr[i]->offset;

            if(c->c_attr[i]->clipped == 1 || c->c_attr[i]->clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_min(atom_getfloat(argv+j), c->c_attr[i]->minimum));
                    }
                }
            }
            if(c->c_attr[i]->clipped == 2 || c->c_attr[i]->clipped == 3)
            {
                for(j = 0; j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        atom_setfloat(argv+j, (float)pd_clip_max(atom_getfloat(argv+j), c->c_attr[i]->maximum));
                    }
                }
            }

            if(c->c_attr[i]->setter)
            {
                c->c_attr[i]->setter(x, c->c_attr[i], argc, argv);
            }
            else if(type == s_int)
            {
                int* pointor = (int *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (int)atom_getlong(argv+j);
                    }
                }
            }
            else if(type == s_long)
            {
                long* pointor = (long *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (long)atom_getlong(argv+j);
                    }
                }
            }
            else if(type == &s_float)
            {
                float* pointor = (float *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (float)atom_getfloat(argv+j);
                    }
                }
            }
            else if(type == s_double)
            {
                double* pointor = (double *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_FLOAT)
                    {
                        pointor[j] = (double)atom_getfloat(argv+j);
                    }
                }
            }
            else if(type == &s_symbol)
            {
                t_symbol** pointor = (t_symbol **)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    if(atom_gettype(argv+j) == A_SYMBOL)
                    {
                        pointor[j] = gensym(atom_getsymbol(argv+j)->s_name);
                    }
                }
            }
            else if(type == s_atom)
            {
                t_atom* pointor = (t_atom *)point;
                for(j = 0; j < size && j < argc; j++)
                {
                    pointor[j] = argv[j];
                }
            }

            ebox_notify(z, s, s_attr_modified, NULL, NULL);
            if(c->c_widget.w_notify != NULL)
            {
                c->c_widget.w_notify(x, s, s_attr_modified, NULL, NULL);
            }

            if(c->c_attr[i]->paint)
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
            if(c->c_attr[i]->save && eobj_isbox(x) && ebox_isdrawable((t_ebox*) x))
            {
                canvas_dirty(eobj_getcanvas(x), 1);
            }
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
    c->c_widget.w_dosave            = NULL;
    c->c_widget.w_popup             = NULL;
    c->c_widget.w_dsp               = NULL;
    c->c_widget.w_oksize            = NULL;
    c->c_widget.w_write             = NULL;
    c->c_widget.w_read              = NULL;
}

//! Initialize the tcl/tk properties dialog window functions // PRIVATE
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @return              Nothing
 */
static void eclass_properties_dialog(t_eclass* c)
{
    int i, j, lenght;
    char buffer[1000];
    char temp[1000];

    // DIALOG WINDOW APPLY //
    for(i = 0; i < c->c_nattr; i++)
    {
        if(c->c_attr[i]->style == gensym("color"))
        {
            sys_vgui("proc pdtk_%s_picker_apply_%s {id red green blue alpha} { \n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
            sys_gui("set nR [expr int( $red * 65025 )]\n");
            sys_gui("set nG [expr int( $green * 65025 )]\n");
            sys_gui("set nB [expr int( $blue * 65025 )]\n");
            sys_gui("set col [format {%4.4x} $nR]\n");
            sys_gui("append col [format {%4.4x} $nG]\n");
            sys_gui("append col [format {%4.4x} $nB]\n");
            sys_vgui("set color [tk_chooseColor -title {%s} -initialcolor #${col}]\n", c->c_attr[i]->label->s_name, c->c_attr[i]->name->s_name);
            sys_gui("if {$color == \"\"} return \n");
            sys_gui("foreach {red2 green2 blue2} [winfo rgb . $color] {}\n");
            sys_gui("set nR2 [expr ( $red2 / 65025. )]\n");
            sys_gui("set nG2 [expr ( $green2 / 65025. )]\n");
            sys_gui("set nB2 [expr ( $blue2 / 65025. )]\n");
            sys_gui("if {$nR2 > 1.} {set nR2 1.} \n");
            sys_gui("if {$nG2 > 1.} {set nG2 1.} \n");
            sys_gui("if {$nB2 > 1.} {set nB2 1.} \n");
            sprintf(buffer, "set cmd [concat $id dialog $id %i ", i+1);
            sprintf(temp, "@%s ", c->c_attr[i]->name->s_name);
            lenght = (int)strlen(temp);
            strncat(buffer, temp, lenght);
            sprintf(temp, "[concat $nR2 $nG2 $nB2] ");
            lenght = (int)strlen(temp);
            strncat(buffer, temp, lenght);
            strncat(buffer, "]\n", 2);
            sys_gui(buffer);
            sys_gui("pdsend $cmd\n");
            sys_gui("}\n");
        }
        else
        {
            sys_vgui("proc pdtk_%s_dialog_apply_%s {id} { \n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
            sys_gui("set vid [string trimleft $id .]\n");
            sys_vgui("set var_%s [concat %s_$vid] \n", c->c_attr[i]->name->s_name, c->c_attr[i]->name->s_name);
            sys_vgui("global $var_%s \n", c->c_attr[i]->name->s_name);
            sprintf(buffer, "set cmd [concat $id dialog $id %i ", i+1);
            sprintf(temp, "@%s ", c->c_attr[i]->name->s_name);
            lenght = (int)strlen(temp);
            strncat(buffer, temp, lenght);
            sprintf(temp, "[eval concat $$var_%s] ", c->c_attr[i]->name->s_name);
            lenght = (int)strlen(temp);
            strncat(buffer, temp, lenght);
            strncat(buffer, "]\n", 2);
            sys_gui(buffer);
            sys_gui("pdsend $cmd\n");
            sys_gui("}\n");
        }
    }

    // DIALOG WINDOW CREATION //
    sys_vgui("proc pdtk_%s_dialog {id \n", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {
        if(!c->c_attr[i]->invisible)
        {
            sys_vgui("%s \n", c->c_attr[i]->name->s_name);
        }
    }
    sys_gui("} {\n");
    sys_gui("set vid [string trimleft $id .]\n");

    for(i = 0; i < c->c_nattr; i++)
    {
        if(!c->c_attr[i]->invisible)
        {
            sys_vgui("set var_%s [concat %s_$vid]\n", c->c_attr[i]->name->s_name, c->c_attr[i]->name->s_name);
            sys_vgui("global $var_%s \n", c->c_attr[i]->name->s_name);
            sys_vgui("set $var_%s $%s\n", c->c_attr[i]->name->s_name,  c->c_attr[i]->name->s_name);
        }
    }
    sys_vgui("toplevel $id\n");
    sys_vgui("wm title $id {%s properties}\n", c->c_class.c_name->s_name);
    sys_vgui("wm resizable $id 0 0\n", c->c_class.c_name->s_name);

    for(i = 0; i < c->c_nattr; i++)
    {
        if(!c->c_attr[i]->invisible)
        {
            sys_vgui("frame $id.name%i \n", i+1);
            sys_vgui("frame $id.sele%i \n", i+1);

            // ATTRIBUTES NAMES //
            sys_vgui("label $id.name%i.name -justify left -font {Helvetica 12} -text \"%s :\"\n", i+1, c->c_attr[i]->label->s_name);
            sys_vgui("pack  $id.name%i.name -side left\n", i+1);
            // SELECTOR //

            if(c->c_attr[i]->style == gensym("checkbutton"))
            {
                sys_vgui("checkbutton $id.sele%i.selec -variable $var_%s -command  [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_attr[i]->name->s_name, c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
                sys_vgui("pack  $id.sele%i.selec -side left\n", i+1);
            }
            else if(c->c_attr[i]->style == gensym("color"))
            {
                sys_vgui("set color [eval eobj_rgba_to_hex $%s]\n", c->c_attr[i]->name->s_name);
                sys_vgui("entry $id.sele%i.selec -font {Helvetica 12} -width 20 -readonlybackground $color -state readonly\n", i+1);
                sys_vgui("bind  $id.sele%i.selec <Button> [concat pdtk_%s_picker_apply_%s $id $%s]\n", i+1, c->c_class.c_name->s_name, c->c_attr[i]->name->s_name, c->c_attr[i]->name->s_name);
                sys_vgui("pack  $id.sele%i.selec -side left\n", i+1);
            }
            else if(c->c_attr[i]->style == gensym("number"))
            {
                sys_vgui("spinbox $id.sele%i.selec -font {Helvetica 12} -width 18 -textvariable [string trim $var_%s] -increment %f \n", i+1, c->c_attr[i]->name->s_name, (float)c->c_attr[i]->step);
                sys_vgui("$id.sele%i.selec configure -command [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_class.c_name->s_name,  c->c_attr[i]->name->s_name);
                sys_vgui("$id.sele%i.selec configure -from -9999999999999 -to 9999999999999\n", i+1, (float)c->c_attr[i]->maximum); // Should be enough
                sys_vgui("$id.sele%i.selec delete 0 end \n", i+1);
                sys_vgui("$id.sele%i.selec insert 0 $%s \n", i+1, c->c_attr[i]->name->s_name);

                sys_vgui("bind $id.sele%i.selec <KeyPress-Return> [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_class.c_name->s_name,  c->c_attr[i]->name->s_name);
                sys_vgui("pack $id.sele%i.selec -side left\n", i+1);
            }
            else if(c->c_attr[i]->style == gensym("menu"))
            {
                sys_vgui("spinbox $id.sele%i.selec -font {Helvetica 12} -width 18 -textvariable [string trim $var_%s] -state readonly\n", i+1, c->c_attr[i]->name->s_name);
                sys_vgui("$id.sele%i.selec configure -command [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_class.c_name->s_name,  c->c_attr[i]->name->s_name);
                sys_vgui("$id.sele%i.selec configure -value {", i+1);
                for(j = 0; j < c->c_attr[i]->itemssize; j++)
                {
                    sys_vgui("%s ", c->c_attr[i]->itemslist[c->c_attr[i]->itemssize - 1 - j]->s_name);
                }
                sys_vgui("}\n");

                sys_vgui("bind $id.sele%i.selec <KeyPress-Return> [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_class.c_name->s_name,  c->c_attr[i]->name->s_name);
                sys_vgui("pack $id.sele%i.selec -side left\n", i+1);
                sys_vgui("$id.sele%i.selec set $%s \n", i+1, c->c_attr[i]->name->s_name);
            }
            else
            {
                sys_vgui("entry $id.sele%i.selec -font {Helvetica 12} -width 20 -textvariable [string trim $var_%s]\n", i+1, c->c_attr[i]->name->s_name);
                sys_vgui("bind $id.sele%i.selec <KeyPress-Return> [concat pdtk_%s_dialog_apply_%s $id]\n", i+1, c->c_class.c_name->s_name,  c->c_attr[i]->name->s_name);
                sys_vgui("pack $id.sele%i.selec -side left\n", i+1);

            }

            sys_vgui("grid config $id.name%i -column 0 -row %i -sticky w\n", i+1, i+1);
            sys_vgui("grid config $id.sele%i -column 1 -row %i -sticky w\n", i+1, i+1);
        }
    }
    sys_gui("}\n");

}




