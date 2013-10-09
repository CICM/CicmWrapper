/*
 * PdEnhanced - Pure Data Enhanced 
 *
 * An add-on for Pure Data
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

#include "eclass.h"

static int puclicite = 0;

t_eclass* eclass_new(char *name, method newmethod, method freemethod, size_t size, int flags, t_atomtype arg1, int arg2)
{
    t_class *pd  = class_new(gensym(name), (t_newmethod)newmethod, (t_method)freemethod, size, flags, arg1, arg2);
    t_eclass* c;
    c = (t_eclass *)resizebytes(pd, sizeof(*pd), sizeof(*c));
    c->c_nattr = 0;
    c->c_attr  = (t_eattr *)malloc(sizeof(t_eattr));
    
#ifdef NO_PUB
    puclicite = 1;
#endif
    if(!puclicite)
    {
        post(".________________________________________________.");
        post("|                                                |");
        post("|                  Pd Enhanced                   |");
        post("| Pure Data Enhanced : Version 0.1 at sept. 2013 |");
        post("|  Organization : Université Paris 8 | CICM      |");
        post("|          Author : Pierre Guillot               |");
        post("|________________________________________________|");
        puclicite = 1;
    }
    return c;
}

void eclass_init(t_eclass* c, long flags)
{
    ewidget_init(c);
    class_setsavefn((t_class *)c, ewidget_save);
    class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
    class_setwidget((t_class *)c, (t_widgetbehavior *)&c->c_widget);
    
    // This is for Max compatibilty //
    CLASS_ATTR_FLOAT_ARRAY  (c, "patching_rect", 0, t_ebox, e_rect, 4);
    CLASS_ATTR_DEFAULT      (c, "patching_rect", 0, "0 0 200 200");
    CLASS_ATTR_FILTER_MIN   (c, "patching_rect", 4);
    CLASS_ATTR_SAVE         (c, "patching_rect", 0);
    CLASS_ATTR_PAINT        (c, "patching_rect", 0);
    CLASS_ATTR_CATEGORY		(c, "patching_rect", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "patching_rect", 0, "Patching rectangle");
    
    CLASS_ATTR_FLOAT_ARRAY  (c, "size", 0, t_ebox, e_rect.width, 2);
    CLASS_ATTR_FILTER_MIN   (c, "size", 4);
    CLASS_ATTR_SAVE         (c, "size", 0);
    CLASS_ATTR_PAINT        (c, "size", 0);
    CLASS_ATTR_CATEGORY		(c, "size", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "size", 0, "Patching Size");
    
    CLASS_ATTR_SYMBOL       (c, "fontname", 0, t_ebox, e_font.c_family);
    CLASS_ATTR_DEFAULT      (c, "fontname", 0, "helvetica");
    CLASS_ATTR_SAVE         (c, "fontname", 0);
    CLASS_ATTR_PAINT        (c, "fontname", 0);
    CLASS_ATTR_CATEGORY		(c, "fontname", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontname", 0, "Font name");
    
    CLASS_ATTR_SYMBOL       (c, "fontweight", 0, t_ebox, e_font.c_weight);
    CLASS_ATTR_DEFAULT      (c, "fontweight", 0, "normal");
    CLASS_ATTR_SAVE         (c, "fontweight", 0);
    CLASS_ATTR_PAINT        (c, "fontweight", 0);
    CLASS_ATTR_CATEGORY		(c, "fontweight", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontweight", 0, "Font weight");
    
    CLASS_ATTR_SYMBOL       (c, "fontslant", 0, t_ebox, e_font.c_slant);
    CLASS_ATTR_DEFAULT      (c, "fontslant", 0, "regular");
    CLASS_ATTR_SAVE         (c, "fontslant", 0);
    CLASS_ATTR_PAINT        (c, "fontslant", 0);
    CLASS_ATTR_CATEGORY		(c, "fontslant", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontslant", 0, "Font slant");
    
    CLASS_ATTR_FLOAT        (c, "fontsize", 0, t_ebox, e_font.c_size);
    CLASS_ATTR_DEFAULT      (c, "fontsize", 0, "11");
    CLASS_ATTR_FILTER_MIN   (c, "fontsize", 4);
    CLASS_ATTR_SAVE         (c, "fontsize", 0);
    CLASS_ATTR_PAINT        (c, "fontsize", 0);
    CLASS_ATTR_CATEGORY		(c, "fontsize", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontsize", 0, "Font size");
    
    class_addmethod((t_class *)c, (t_method)ebox_mouse_enter, gensym("mouseenter"), A_CANT);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_leave, gensym("mouseleave"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_move,  gensym("mousemove"),  A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_down,  gensym("mousedown"),  A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_up,    gensym("mouseup"),    A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_drag,  gensym("mousedrag"),  A_DEFFLOAT, A_DEFFLOAT, A_DEFFLOAT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_set_mouse_global_position,  gensym("globalmouse"), A_DEFFLOAT,A_DEFFLOAT,0);
    
    // For global mouse position //
    sys_gui("proc global_mousepos {target} {\n");
    sys_gui(" set x [winfo pointerx .]\n");
    sys_gui(" set y [winfo pointery .]\n");
    sys_gui(" pdsend \"$target globalmouse $x $y\"\n");
    sys_gui("}\n");
}

void eclass_dspinit(t_eclass* c)
{
    CLASS_MAINSIGNALIN((t_class *)c, t_ebox, e_float);
    class_addmethod((t_class *)c, (t_method)ebox_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

void eclassbox_dspinit(t_eclass* c)
{
    ewidget_init(c);
    class_setsavefn((t_class *)c, ewidget_save);
    class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
    
    CLASS_MAINSIGNALIN((t_class *)c, t_ebox, e_float);
    class_addmethod((t_class *)c, (t_method)ebox_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

t_pd_err eclass_register(t_symbol *name_space, t_eclass *c)
{
    if(name_space == gensym("box"))
    {
        c->c_box = 1;
    }
    else
        c->c_box = 0;
    
    return 0;
}

void eclass_addmethod(t_eclass* c, method m, char* name, t_atomtype type, long anything)
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
    else if(gensym(name) == gensym("dblclick"))
    {
        c->c_widget.w_dblclick = m;
    }
    else if(gensym(name) == gensym("key"))
    {
        c->c_widget.w_key = m;
    }
    else if(gensym(name) == gensym("keyfilter"))
    {
        c->c_widget.w_keyfilter = m;
    }
    else if(gensym(name) == gensym("deserted"))
    {
        c->c_widget.w_deserted = m;
    }
    else if(gensym(name) == gensym("paint"))
    {
        c->c_widget.w_paint = m;
    }
    else if(gensym(name) == gensym("assist"))
    {
        ;
    }
    else if(gensym(name) == gensym("notify"))
    {
        c->c_widget.w_notify = (t_err_method)m;
    }
    else if(gensym(name) == gensym("anything"))
    {
        class_addanything((t_class *)c, m);
    }
    else if(gensym(name) == gensym("getdrawparams"))
    {
        c->c_widget.w_getdrawparameters = m;
    }
    else if(gensym(name) == gensym("oksize"))
    {
        c->c_widget.w_oksize = m;
    }
    else if(gensym(name) == gensym("bang"))
    {
        class_addbang((t_class *)c, m);
    }
    else if(gensym(name) == gensym("float"))
    {
        class_addfloat((t_class *)c, m);
    }
    else if(gensym(name) == gensym("list"))
    {
        class_addlist((t_class *)c, m);
    }
    else if(gensym(name) == gensym("save") || gensym(name) == gensym("jsave"))
    {
        c->c_widget.w_save = m;
    }
    else if(gensym(name) == gensym("popup"))
    {
        c->c_widget.w_popup = m;
    }
    else if(gensym(name) == gensym("dsp") || gensym(name) == gensym("dsp64"))
    {
        c->c_widget.w_dsp = m;
    }
    else
    {
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, anything);
    }
}




