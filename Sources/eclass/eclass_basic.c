/*
 * CicmWrapper - Pure Data Enhanced
 *
 * A wrapper for Pure Data
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

//! Allocate the memory and initialize a new eclass
/*
 \ @memberof            eclass
 \ @param name          The eclass name
 \ @param newmethod     The new method
 \ @param freemethod    The free method
 \ @param size          The size of the eclass
 \ @param flags         The class flags
 \ @param arg1          The type of parameters the new function
 \ @param arg2          Another argument
 \ @return              This function return the new eclass
*/
t_eclass* eclass_new(char *name, method newmethod, method freemethod, size_t size, int flags, t_atomtype arg1, int arg2)
{
    char help[MAXPDSTRING];
    t_class *pd  = class_new(gensym(name), (t_newmethod)newmethod, (t_method)freemethod, size, flags, arg1, arg2);
    t_eclass* c;
    c = (t_eclass *)resizebytes(pd, sizeof(*pd), sizeof(*c));
    c->c_nattr = 0;
    c->c_dsp   = 0;
    c->c_box   = 0;
    c->c_juce  = 0;
    c->c_attr  = NULL;

    erouter_setup();

    sprintf(help, "helps/%s", c->c_class.c_name->s_name);
    class_sethelpsymbol((t_class *)c, gensym(help));

    class_addmethod((t_class *)c, (t_method)eobj_mousecanvas,  gensym("canvasmouse"), A_GIMME, 0);
    return c;
}

//! Initialize an eclass for UI behavior
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @param flags         Nothing for the moment
 \ @return              Nothing
*/
void eclass_init(t_eclass* c, long flags)
{
    ewidget_init(c);

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
    CLASS_ATTR_ACCESSORS    (c, "size", NULL, ebox_size_set);

    CLASS_ATTR_DEFAULT      (c, "fontname", 0, "Helvetica");
    CLASS_ATTR_SAVE         (c, "fontname", 0);
    CLASS_ATTR_PAINT        (c, "fontname", 0);
    CLASS_ATTR_CATEGORY		(c, "fontname", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontname", 0, "Font Name");
    CLASS_ATTR_ACCESSORS    (c, "fontname", NULL, ebox_set_font);
    CLASS_ATTR_STYLE        (c, "fontname", 0, "menu");
    CLASS_ATTR_ITEMS        (c, "fontname", 0, "Helvetica Monaco Courier Times DejaVu");

    CLASS_ATTR_DEFAULT      (c, "fontweight", 0, "normal");
    CLASS_ATTR_SAVE         (c, "fontweight", 0);
    CLASS_ATTR_PAINT        (c, "fontweight", 0);
    CLASS_ATTR_CATEGORY		(c, "fontweight", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontweight", 0, "Font Weight");
    CLASS_ATTR_ACCESSORS    (c, "fontweight", NULL, ebox_set_fontweight);
    CLASS_ATTR_STYLE        (c, "fontweight", 0, "menu");
    CLASS_ATTR_ITEMS        (c, "fontweight", 0, "normal bold");

    CLASS_ATTR_DEFAULT      (c, "fontslant", 0, "roman");
    CLASS_ATTR_SAVE         (c, "fontslant", 0);
    CLASS_ATTR_PAINT        (c, "fontslant", 0);
    CLASS_ATTR_CATEGORY		(c, "fontslant", 0, "Basic");
    CLASS_ATTR_LABEL		(c, "fontslant", 0, "Font Slant");
    CLASS_ATTR_ACCESSORS    (c, "fontslant", NULL, ebox_set_fontslant);
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
    class_addmethod((t_class *)c, (t_method)ebox_patcher_editmode,  gensym("editmode"),     A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_attrprint,         gensym("attrprint"),    A_NULL,  0);
    class_addmethod((t_class *)c, (t_method)ebox_dialog,            gensym("dialog"),       A_GIMME, 0);

    class_addmethod((t_class *)c, (t_method)ebox_mouse_enter, gensym("mouseenter"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_leave, gensym("mouseleave"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_move,  gensym("mousemove"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_down,  gensym("mousedown"),  A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_up,    gensym("mouseup"),    A_GIMME, 0);
    class_addmethod((t_class *)c, (t_method)ebox_mouse_drag,  gensym("mousedrag"),  A_GIMME, 0);

    class_addmethod((t_class *)c, (t_method)ebox_pos, gensym("pos"), A_DEFFLOAT,A_DEFFLOAT,0);
    class_addmethod((t_class *)c, (t_method)ebox_vis, gensym("vis"), A_DEFFLOAT,0);

    class_setwidget((t_class *)&c->c_class, (t_widgetbehavior *)&c->c_widget);
    class_setsavefn((t_class *)&c->c_class, (t_savefn)eobj_save);
    class_setpropertiesfn((t_class *)c, (t_propertiesfn)ebox_properties);
}

//! Initialize an UI eclass for DSP behavior
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @return              Nothing
*/
void eclass_dspinit(t_eclass* c)
{
    c->c_dsp = 1;
    CLASS_MAINSIGNALIN((t_class *)c, t_edspbox, d_float);
    class_addmethod((t_class *)c, (t_method)eobj_dsp, gensym("dsp"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

//! Initialize an box eclass for DSP behavior
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @return              Nothing
*/
void eclassbox_dspinit(t_eclass* c)
{
    c->c_dsp = 1;
    CLASS_MAINSIGNALIN((t_class *)c, t_edspobj, d_float);
    class_addmethod((t_class *)c, (t_method)eobj_dsp, gensym("dsp"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add"), A_NULL, 0);
    class_addmethod((t_class *)c, (t_method)eobj_dsp_add, gensym("dsp_add64"), A_NULL, 0);
}

//! Initialize the tcl/tk properties dialog window functions // PRIVATE
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @return              Nothing
*/
void eclass_properties_dialog(t_eclass* c)
{
    int i, j;
    char buffer[1000];
    char temp[1000];

    sys_vgui("proc pdtk_rgba_to_hex_%s {red green blue alpha} { \n", c->c_class.c_name->s_name);
    sys_gui("set nR [expr int( $red * 65025 )]\n");
    sys_gui("set nG [expr int( $green * 65025 )]\n");
    sys_gui("set nB [expr int( $blue * 65025 )]\n");
    sys_gui("set col [format {%4.4x} $nR]\n");
    sys_gui("append col [format {%4.4x} $nG]\n");
    sys_gui("append col [format {%4.4x} $nB]\n");
    sys_gui("return #$col\n");
    sys_gui("}\n");

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
            strcat(buffer, temp);
            sprintf(temp, "[concat $nR2 $nG2 $nB2] ");
            strcat(buffer, temp);
            strcat(buffer, "]\n");
            sys_gui(buffer);
            sys_gui("pdsend $cmd\n");
            sys_gui("}\n");
        }/*
        if(c->c_attr[i]->style == gensym("menu"))
        {
            sys_vgui("proc pdtk_%s_menu_apply_%s {id index} { \n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
            for(j = 0; j < c->c_attr[i]->itemssize; j++)
            {
                sys_vgui("if {$index == %i} {pdsend [concat $id dialog $id %i @%s %s]}\n", j, i+1, c->c_attr[i]->name->s_name, c->c_attr[i]->itemslist[j]->s_name);
            }
            sys_gui("}\n");

            sys_vgui("proc pdtk_%s_menu_%s {id} { \n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
            sys_vgui("destroy $id.sele%i.menu\n", i+1);
            sys_vgui("menu $id.sele%i.menu -tearoff 0 -font {Helvetica 12}\n", i+1);
            for(j = 0; j < c->c_attr[i]->itemssize; j++)
            {
                sys_vgui("$id.sele%i.menu add radiobutton -label {%s} -command [pdtk_%s_menu_apply_%s $id %i]\n", i+1, c->c_attr[i]->itemslist[j]->s_name, c->c_class.c_name->s_name, c->c_attr[i]->name->s_name, j);
            }
            sys_vgui("$id.sele%i.menu post [winfo pointerx .] [winfo pointery .]\n", i+1);
            sys_gui("}\n");
        }*/
        else
        {
            sys_vgui("proc pdtk_%s_dialog_apply_%s {id} { \n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
            sys_gui("set vid [string trimleft $id .]\n");
            sys_vgui("set var_%s [concat %s_$vid] \n", c->c_attr[i]->name->s_name, c->c_attr[i]->name->s_name);
            sys_vgui("global $var_%s \n", c->c_attr[i]->name->s_name);
            sprintf(buffer, "set cmd [concat $id dialog $id %i ", i+1);
            sprintf(temp, "@%s ", c->c_attr[i]->name->s_name);
            strcat(buffer, temp);
            sprintf(temp, "[eval concat $$var_%s] ", c->c_attr[i]->name->s_name);
            strcat(buffer, temp);
            strcat(buffer, "]\n");
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
                sys_vgui("set color [eval pdtk_rgba_to_hex_%s $%s]\n", c->c_class.c_name->s_name, c->c_attr[i]->name->s_name);
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

//! Initialize the eclass name space BOX or NOBOX
/*
 \ @memberof            eclass
 \ @param name          The name space
 \ @param c             The eclass pointer
 \ @return              Always 0 (for the moment)
*/
t_pd_err eclass_register(t_symbol *name, t_eclass *c)
{
    if(name == gensym("box"))
        c->c_box = 1;
    else
        c->c_box = 0;

    eclass_properties_dialog(c);
    return 0;
}

//! Add methods to an eclass
/*
 \ @memberof            eclass
 \ @param c             The eclass pointer
 \ @param m             The method function
 \ @param name          The method selector
 \ @param type          The method type
 \ @param anything      Another argument
 \ @return              Nothing
*/
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
    else if(gensym(name) == gensym("deserted"))
    {
        class_addmethod((t_class *)c, (t_method)ebox_patcher_focus,   gensym("focus"),  A_NULL,  0);
        c->c_widget.w_deserted = m;
    }
    else if(gensym(name) == gensym("paint"))
    {
        if(c->c_juce)
            c->c_widget.w_paint_juce = m;
        else
            c->c_widget.w_paint = m;
    }
    else if(gensym(name) == gensym("assist"))
    {
        ;
    }
    else if(gensym(name) == gensym("notify"))
    {
        if(c->c_juce)
            c->c_widget.w_notify_juce = (t_err_method)m;
        else
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
    else if(gensym(name) == gensym("symbol"))
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
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, anything);
    }
    else if(gensym(name) == gensym("write"))
    {
		 // SAVE DIALOG //
		sys_gui("proc eobj_saveas {name initialfile initialdir} {\n");
		sys_gui("if { ! [file isdirectory $initialdir]} {set initialdir $::env(HOME)}\n");
		sys_gui("set filename [tk_getSaveFile -initialfile $initialfile -initialdir $initialdir -defaultextension .pd -filetypes $::filetypes]\n");
		sys_gui("if {$filename eq \"\"} return;\n");

		sys_gui("set extension [file extension $filename]\n");
		sys_gui("set oldfilename $filename\n");

		sys_gui("if {$filename ne $oldfilename && [file exists $filename]} {\n");
		sys_gui("set answer [tk_messageBox -type okcancel -icon question -default cancel-message [_ \"$filename\" already exists. Do you want to replace it?]]\n");
		sys_gui("if {$answer eq \"cancel\"} return;\n");
		sys_gui("}\n");
		sys_gui("set dirname [file dirname $filename]\n");
		sys_gui("set basename [file tail $filename]\n");
		sys_gui("pdsend \"$name eobjwriteto [enquote_path $dirname/$basename]\"\n");
		sys_gui("set ::filenewdir $dirname\n");
		sys_gui("::pd_guiprefs::update_recentfiles $filename\n");
		sys_gui("}\n");

        class_addmethod((t_class *)c, (t_method)eobj_write, gensym(name), type, anything);
        class_addmethod((t_class *)c, (t_method)eobj_write, gensym("eobjwriteto"), type, anything);
        c->c_widget.w_write = m;
    }
    else if(gensym(name) == gensym("read"))
    {
		 // OPEN DIALOG //
		sys_gui("proc eobj_openfrom {name} {\n");
		sys_gui("if { ! [file isdirectory $::filenewdir]} {\n");
		sys_gui("set ::filenewdir [file normalize $::env(HOME)]\n");
		sys_gui("}\n");
		sys_gui("set files [tk_getOpenFile -multiple true -initialdir $::fileopendir]\n");
		sys_gui("pdsend \"$name eobjreadfrom [enquote_path $files]\"\n");
		sys_gui("}\n");
        class_addmethod((t_class *)c, (t_method)eobj_read, gensym(name), type, anything);
        class_addmethod((t_class *)c, (t_method)eobj_read, gensym("eobjreadfrom"), type, anything);
        c->c_widget.w_read = m;
    }
    else
    {
        class_addmethod((t_class *)c, (t_method)m, gensym(name), type, anything);
    }
}




