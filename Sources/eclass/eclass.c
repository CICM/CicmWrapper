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

t_eclass* eclass_new(char *name, method newmethod, method freemethod, size_t size, int flags, t_atomtype arg1, int arg2)
{
    t_class *pd  = class_new(gensym(name), (t_newmethod)newmethod, (t_method)freemethod, size, flags, arg1, arg2);
    t_eclass* c;
    c = (t_eclass *)resizebytes(pd, sizeof(*pd), sizeof(*c));
    c->c_nattr = 0;
    c->c_attr  = (t_eattr *)malloc(sizeof(t_eattr));
    
#ifdef E_PUB
post(".________________________________________________.");
post("|                                                |");
post("|                  Pd Enhanced                   |");
post("| Pure Data Enhanced : Version 0.1 at sept. 2013 |");
post("|  Organization : Université Paris 8 | CICM      |");
post("|          Author : Pierre Guillot               |");
post("|________________________________________________|");
#endif
    return c;
}

void eclass_init(t_eclass* c, long flags)
{
    ewidget_init(c);
    eclass_default_attributes(c);
    
    class_addmethod((t_class *)c, (t_method)ebox_mouse_enter, gensym("mouseenter"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_leave, gensym("mouseleave"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_move,  gensym("mousemove"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_down,  gensym("mousedown"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_up,    gensym("mouseup"),    A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_drag,  gensym("mousedrag"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_wheel, gensym("mousewheel"), A_GIMME, 0);
    
    class_addmethod((t_class *)c, (t_method)ebox_keyup,     gensym("keyup"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_keydown,   gensym("keydown"),A_GIMME, 0);
    
    
    class_addmethod((t_class *)c, (t_method)ebox_patcher_editmode,  gensym("editmode"),     A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_patcher_motion,    gensym("motion"),       A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_patcher_key,       gensym("key"),          A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_patcher_mouse,     gensym("mouse"),        A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_patcher_focus,     gensym("focus"),        A_GIMME, 0);
    
    class_addmethod((t_class *)c, (t_method)ebox_popup,                     gensym("popup"),  A_SYMBOL, A_DEFFLOAT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_set_mouse_global_position, gensym("globalmouse"), A_DEFFLOAT,A_DEFFLOAT,0);
    class_addmethod((t_class *)c, (t_method)ebox_set_mouse_patcher_position,gensym("patchermouse"), A_DEFFLOAT,A_DEFFLOAT,0);

    class_setwidget((t_class *)&c->c_class, (t_widgetbehavior *)&c->c_widget);
    class_setsavefn((t_class *)&c->c_class, ebox_save);
    class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
    class_addmethod((t_class *)c, (t_method)ebox_dialog,     gensym("dialog"),        A_GIMME, 0);
    
    // For global and patcher mouse position //
    sys_gui("proc global_mousepos {target} {\n");
    sys_gui(" set x [winfo pointerx .]\n");
    sys_gui(" set y [winfo pointery .]\n");
    sys_gui(" pdsend \"$target globalmouse $x $y\"\n");
    sys_gui("}\n");
    
    sys_gui("proc patcher_mousepos {target patcher} {\n");
    sys_gui(" set x [winfo rootx $patcher]\n");
    sys_gui(" set y [winfo rooty $patcher]\n");
    sys_gui(" pdsend \"$target patchermouse $x $y\"\n");
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
    class_setsavefn((t_class *)c, ebox_save);
    //class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
    
    CLASS_MAINSIGNALIN((t_class *)c, t_ebox, e_float);
    class_addmethod((t_class *)c, (t_method)ebox_dsp, gensym("dsp"), A_CANT, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

void eclass_default_attributes(t_eclass* c)
{
    CLASS_ATTR_FLOAT_ARRAY  (c, "size", 0, t_ebox, e_rect.width, 2);
    CLASS_ATTR_DEFAULT      (c, "size", 0, "100. 100.");
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
    CLASS_ATTR_LABEL		(c, "fontname", 0, "Font Name");
    
    CLASS_ATTR_SYMBOL       (c, "fontweight", 0, t_ebox, e_font.c_weight);
    CLASS_ATTR_DEFAULT      (c, "fontweight", 0, "normal");
    CLASS_ATTR_SAVE         (c, "fontweight", 0);
    CLASS_ATTR_PAINT        (c, "fontweight", 0);
    CLASS_ATTR_CATEGORY		(c, "fontweight", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontweight", 0, "Font Weight");
    
    CLASS_ATTR_SYMBOL       (c, "fontslant", 0, t_ebox, e_font.c_slant);
    CLASS_ATTR_DEFAULT      (c, "fontslant", 0, "regular");
    CLASS_ATTR_SAVE         (c, "fontslant", 0);
    CLASS_ATTR_PAINT        (c, "fontslant", 0);
    CLASS_ATTR_CATEGORY		(c, "fontslant", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontslant", 0, "Font Slant");
    
    CLASS_ATTR_FLOAT        (c, "fontsize", 0, t_ebox, e_font.c_size);
    CLASS_ATTR_DEFAULT      (c, "fontsize", 0, "11");
    CLASS_ATTR_FILTER_MIN   (c, "fontsize", 4);
    CLASS_ATTR_SAVE         (c, "fontsize", 0);
    CLASS_ATTR_PAINT        (c, "fontsize", 0);
    CLASS_ATTR_CATEGORY		(c, "fontsize", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontsize", 0, "Font Size");
}

void eclass_properties_dialog(t_eclass* c)
{
    int i, j;
    char buffer[1000];
    char temp[1000];
    // DIALOG WINDOW APPLY //
    sys_vgui("proc pdtk_%s_dialog_apply {id} { \n", c->c_class.c_name->s_name);
    sys_gui("set vid [string trimleft $id .]\n");
    for(i = 0; i < c->c_nattr; i++)
    {
        sys_vgui("set var_%s [concat %s_$vid] \n", c->c_attr[i].name->s_name, c->c_attr[i].name->s_name);
        sys_vgui("global $var_%s \n", c->c_attr[i].name->s_name);
    }
    
    sprintf(buffer, "set cmd [concat $id dialog ");
   
    for(i = 0; i < c->c_nattr; i++)
    {
        sprintf(temp, "@%s ", c->c_attr[i].name->s_name);
        strcat(buffer, temp);
        sprintf(temp, "[eval concat $$var_%s] ", c->c_attr[i].name->s_name);
        strcat(buffer, temp);
    }
    strcat(buffer, "]\n");
    sys_gui(buffer);
    //sys_gui("]\n");
    sys_gui("pdsend $cmd\n");
    sys_gui("}\n");
    
    /*
    // DIALOG WINDOW CANCEL //
    sys_vgui("proc pdtk_%s_dialog_cancel {id}{ \n", c->c_class.c_name->s_name);
    sys_gui("set cmd [concat $id cancel ]\n");
    sys_gui("pdsend cmd\n");
    sys_gui("}\n");
    
    // DIALOG WINDOW OK //
    sys_vgui("proc pdtk_%s_dialog_ok {id}{ \n", c->c_class.c_name->s_name);
    sys_vgui("pdtk_%s_dialog_apply  $id\n", c->c_class.c_name->s_name);
    sys_vgui("pdtk_%s_dialog_cancel $id\n", c->c_class.c_name->s_name);
    sys_gui("}\n");
    */
    
    // DIALOG WINDOW CREATION //
    sys_vgui("proc pdtk_%s_dialog {id \n", c->c_class.c_name->s_name);
    for(i = 0; i < c->c_nattr; i++)
    {
        for(j = 0; j < c->c_attr[i].sizemax; j++)
        {
            sys_vgui("%s%i \n", c->c_attr[i].name->s_name, j);
        }
    }
    sys_gui("} {\n");
    sys_gui("set vid [string trimleft $id .]\n");
    
    for(i = 0; i < c->c_nattr; i++)
    {
        sys_vgui("set var_%s [concat %s_$vid]\n", c->c_attr[i].name->s_name, c->c_attr[i].name->s_name);
        sys_vgui("global $var_%s \n", c->c_attr[i].name->s_name);
        sys_vgui("set $var_%s \"\n", c->c_attr[i].name->s_name);
        for(j = 0; j < c->c_attr[i].sizemax; j++)
        {
            sys_vgui("$%s%i\n", c->c_attr[i].name->s_name, j);
        }
        sys_vgui("\" \n");
    }
    sys_vgui("toplevel $id\n");
    sys_vgui("wm title $id {%s properties}\n", c->c_class.c_name->s_name);

    for(i = 0; i < c->c_nattr; i++)
    {
        sys_vgui("frame $id.0%s \n", c->c_attr[i].name->s_name);
        sys_vgui("pack  $id.0%s -side top -anchor w\n", c->c_attr[i].name->s_name);
        sys_vgui("frame $id.1%s \n", c->c_attr[i].name->s_name);
        sys_vgui("pack  $id.1%s -side top -anchor e\n", c->c_attr[i].name->s_name);
        
        sys_vgui("label $id.0%s.label -text \"%s :\"\n", c->c_attr[i].name->s_name, c->c_attr[i].label->s_name);
        sys_vgui("entry $id.1%s.entry -textvariable $var_%s\n", c->c_attr[i].name->s_name, c->c_attr[i].name->s_name);
        sys_vgui("pack  $id.0%s.label -side left -anchor w\n", c->c_attr[i].name->s_name);
        sys_vgui("pack  $id.1%s.entry -side right -anchor e\n", c->c_attr[i].name->s_name);
        sys_vgui("bind  $id.1%s.entry <KeyPress-Return> [concat pdtk_%s_dialog_apply $id]\n", c->c_attr[i].name->s_name, c->c_class.c_name->s_name);
    }
    sys_gui("}\n");
    
}

t_pd_err eclass_register(t_symbol *name_space, t_eclass *c)
{
    if(name_space == gensym("box"))
        c->c_box = 1;
    else
        c->c_box = 0;
    
    eclass_properties_dialog(c);
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
    else if(gensym(name) == gensym("mousewheel"))
    {
        c->c_widget.w_mousewheel = m;
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
    else if(gensym(name) == gensym("float"))
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
    else
    {
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, anything);
    }
}




