/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "float.h"
#include "epopup.h"
#include "eobj.h"
#include "ebox.h"
#include "egraphics.h"

static void eobj_widget_notify(t_eobj* obj, t_symbol* widget, t_symbol* name, t_symbol* action)
{
    t_atom av[2];
    if(is_valid_symbol(obj->o_camo_id) && obj->o_camo_id->s_thing)
    {
        atom_setsym(av, name);
        atom_setsym(av+1, action);
        pd_typedmess(obj->o_camo_id->s_thing, widget, 2, av);
    }
}

static t_class* epopup_setup()
{
    t_class* epopup_class = NULL;
    t_symbol* epopup1572_sym = gensym("epopup1572");
    if(!epopup1572_sym->s_thing)
    {
        epopup_class = class_new(gensym("epopup"), NULL, (t_method)NULL, sizeof(t_epopup), CLASS_PD, A_GIMME, 0);
        epopup1572_sym->s_thing = (t_class **)epopup_class;
        return epopup_class;
    }
    else
    {
        return (t_class *)epopup1572_sym->s_thing;
    }
}

t_epopup* epopupmenu_create(t_eobj* x)
{
    char buffer[MAXPDSTRING];
    t_epopup* popup = NULL; t_canvas* canvas = NULL;
    t_class* c = epopup_setup();
    if(c)
    {
        popup = (t_epopup *)pd_new(c);
        if(popup)
        {
            canvas = eobj_getcanvas(x);
            sprintf(buffer,".x%lx.c", (long unsigned int)canvas);
            popup->c_canvas_id = gensym(buffer);
            sprintf(buffer, "%s.popup%lx", popup->c_canvas_id->s_name, (long unsigned int)popup);
            popup->c_name = gensym(buffer);
            sprintf(buffer, "popup%lx", (long unsigned int)popup);
            popup->c_popup_id = gensym(buffer);
            popup->c_owner  = x;
            popup->c_size   = 0;
            popup->c_items  = NULL;
            pd_bind((t_pd *)popup, popup->c_popup_id);
            sys_vgui("destroy %s\n", popup->c_name->s_name);
            sys_vgui("menu %s -type normal\n", popup->c_name->s_name);
            
            eobj_widget_notify(x, s_cream_popup, popup->c_popup_id, s_cream_create);
        }
    }
    return popup;
}

void epopupmenu_destroy(t_epopup* popup)
{
    if(popup)
    {
        eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_destroy);
        pd_unbind((t_pd *)popup, popup->c_popup_id);
        sys_vgui("destroy %s\n", popup->c_name->s_name);
        if(popup->c_size && popup->c_items)
        {
            free(popup->c_items);
        }
        free(popup);
    }
}

void epopupmenu_setfont(t_epopup* popup, t_efont *font)
{
    sys_vgui("%s configure -font {{%s} %d %s italic}\n", popup->c_name->s_name, font[0].family->s_name, (int)font[0].size, font[0].weight->s_name, font[0].slant->s_name);
    memcpy(&popup->c_font, font, sizeof(t_efont));
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_attr_modified);
}

void epopupmenu_setbackgroundcolor(t_epopup *popup, t_rgba const* color)
{
    memcpy(&popup->c_bgcolor, color, sizeof(t_rgba));
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_attr_modified);
}

void epopupmenu_settextcolor(t_epopup *popup, t_rgba const* color)
{
    memcpy(&popup->c_txtcolor, color, sizeof(t_rgba));
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_attr_modified);
}

void epopupmenu_additem(t_epopup* popup, int itemid, const char *text, char checked, char disabled)
{
    t_epopup_item *temp;
    if(!popup->c_items)
    {
        popup->c_items = (t_epopup_item *)malloc(sizeof(t_epopup_item));
        if(popup->c_items)
        {
            popup->c_items[0].c_id = itemid;
            strncpy(popup->c_items[0].c_label, text, MAXPDSTRING);
            popup->c_items[0].c_checked = checked;
            popup->c_items[0].c_disable = disabled;
            popup->c_items[0].c_separator = 0;
            popup->c_size = 1;
        }
        else
        {
            popup->c_size = 0;
        }
    }
    else
    {
        temp = (t_epopup_item *)realloc(popup->c_items, sizeof(t_epopup_item) * (size_t)(popup->c_size + 1));
        if(temp)
        {
            popup->c_items = temp;
            popup->c_items[popup->c_size].c_id = itemid;
            strncpy(popup->c_items[popup->c_size].c_label, text, MAXPDSTRING);
            popup->c_items[popup->c_size].c_checked = checked;
            popup->c_items[popup->c_size].c_disable = disabled;
            popup->c_items[popup->c_size].c_separator = 0;
            popup->c_size++;
        }
    }
    
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_attr_modified);
}

void epopupmenu_addseperator(t_epopup* popup)
{
    t_epopup_item *temp;
    if(!popup->c_items)
    {
        popup->c_items = (t_epopup_item *)malloc(sizeof(t_epopup_item));
        if(popup->c_items)
        {
            popup->c_items[0].c_separator = 1;
            popup->c_size = 1;
        }
        else
        {
            popup->c_size = 0;
        }
    }
    else
    {
        temp = (t_epopup_item *)realloc(popup->c_items, sizeof(t_epopup_item) * (size_t)(popup->c_size + 1));
        if(temp)
        {
            popup->c_items = temp;
            popup->c_items[popup->c_size].c_separator = 1;
            popup->c_size++;
        }
    }
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_attr_modified);
}

void epopupmenu_popup(t_epopup* popup, t_rect const* bounds)
{
    int i = 0;
    for(i = 0; i < popup->c_size; i++)
    {
        if(popup->c_items[i].c_separator)
        {
            sys_vgui("%s add separator\n", popup->c_name->s_name);
        }
        else
        {
            sys_vgui("set checked%s%i %i\n", popup->c_name->s_name, popup->c_items[i].c_id, (int)popup->c_items[i].c_checked);
            sys_vgui("%s add checkbutton \
                     -command {pdsend {%s popup %s %f}} \
                     -background %s -foreground %s\
                     -label {%s} \
                     -variable checked%s%i -state ",
                     popup->c_name->s_name,
                     popup->c_owner->o_id->s_name, popup->c_popup_id->s_name, (float)popup->c_items[i].c_id,
                     rgba_to_hex(&popup->c_bgcolor), rgba_to_hex(&popup->c_txtcolor),
                     popup->c_items[i].c_label,
                     popup->c_name->s_name, popup->c_items[i].c_id);
            
            if(popup->c_items[i].c_disable)
                sys_vgui("disable\n");
            else
                sys_vgui("active\n");
        }
    }
    
    sys_vgui("tk_popup %s [expr [winfo rootx %s] + %i] [expr [winfo rooty %s] + %i]\n", popup->c_name->s_name, popup->c_canvas_id->s_name, (int)bounds->x, popup->c_canvas_id->s_name, (int)bounds->y);
    
    memcpy(&popup->c_bounds, bounds, sizeof(t_rect));
    eobj_widget_notify(popup->c_owner, s_cream_popup, popup->c_popup_id, s_cream_popup);
}

t_epopup* epopupmenu_getfromsymbol(t_symbol* name)
{
    t_class* c = epopup_setup();
    if(c)
    {
        return (t_epopup *)pd_findbyclass(name, c);
    }
    return NULL;
}








static void etexteditor_text(t_etexteditor* x, t_symbol* s, int argc, t_atom* argv)
{
    int i;
    size_t lenght = 0;
    char* temp;
    char text[MAXPDSTRING];
    if(!argc)
    {
        if(x->c_text || x->c_size)
        {
            memset(x->c_text, 0, (size_t)x->c_size * sizeof(char));
        }
        return;
    }
    for(i = 0; i < argc; i++)
    {
        if(atom_gettype(argv+i) == A_FLOAT)
        {
            sprintf(text, "%g ", atom_getfloat(argv+i));
            lenght += strlen(text);
        }
        else if(atom_gettype(argv+i) == A_SYMBOL)
        {
            lenght += strlen(atom_getsymbol(argv+i)->s_name) + 1;
        }
        
    }
    if(!x->c_text || !x->c_size)
    {
        x->c_text = (char *)malloc((size_t)lenght * sizeof(char));
        if(x->c_text)
        {
            x->c_size = (int)lenght;
        }
        else
        {
            x->c_size = 0;
            return;
        }
    }
    else if(x->c_size < (int)lenght)
    {
        temp = realloc(x->c_text, (size_t)lenght * sizeof(char));
        if(temp)
        {
            x->c_text = temp;
            x->c_size = (int)lenght;
        }
        else
        {
            return;
        }
    }
    memset(x->c_text, 0, (size_t)x->c_size * sizeof(char));
    for(i = 0; i < argc - 1; i++)
    {
        if(atom_gettype(argv+i) == A_FLOAT)
        {
            sprintf(text, "%g ", atom_getfloat(argv+i));
            strncat(x->c_text, text, MAXPDSTRING);
        }
        else if(atom_gettype(argv+i) == A_SYMBOL)
        {
            strncat(x->c_text, atom_getsymbol(argv+i)->s_name, MAXPDSTRING);
            strncat(x->c_text, " ", 1);
        }
    }
    if(argc)
    {
        if(atom_gettype(argv+argc-1) == A_FLOAT)
        {
            sprintf(text, "%g", atom_getfloat(argv+argc-1));
            strncat(x->c_text, text, MAXPDSTRING);
        }
        else if(atom_gettype(argv+argc-1) == A_SYMBOL)
        {
            strncat(x->c_text, atom_getsymbol(argv+argc-2)->s_name, MAXPDSTRING);
        }
    }
}

static t_class* etexteditor_setup()
{
    t_class* etexteditor_class = NULL;
    t_symbol* etexteditor1572_sym = gensym("etexteditor1572");
    if(!etexteditor1572_sym->s_thing)
    {
        etexteditor_class = class_new(gensym("etexteditor"), NULL, (t_method)NULL, sizeof(t_etexteditor), CLASS_PD, A_GIMME, 0);
        etexteditor1572_sym->s_thing = (t_class **)etexteditor_class;
        class_addmethod(etexteditor_class, (t_method)etexteditor_text, gensym("text"), A_GIMME, 0);
        return etexteditor_class;
    }
    else
    {
        return (t_class *)etexteditor1572_sym->s_thing;
    }
}

t_etexteditor* etexteditor_create(t_ebox* x)
{
    char buffer[MAXPDSTRING];
    t_canvas* canvas = NULL; t_etexteditor* editor = NULL;
    t_class* c = etexteditor_setup();
    if(c)
    {
        editor = (t_etexteditor *)pd_new(c);
        if(editor)
        {
            canvas = eobj_getcanvas(x);
            sprintf(buffer, "texteditor%lx", (long unsigned int)editor);
            editor->c_editor_id = gensym(buffer);
            sprintf(buffer,".x%lx.c", (long unsigned int)canvas);
            editor->c_canvas_id = gensym(buffer);
            sprintf(buffer,"%s.frame%lx", editor->c_canvas_id->s_name, (long unsigned int)editor);
            editor->c_frame_id = gensym(buffer);
            sprintf(buffer, "%s.text%lx", editor->c_frame_id->s_name, (long unsigned int)editor);
            editor->c_name = gensym(buffer);
            sprintf(buffer, "%s.window%lx", editor->c_canvas_id->s_name, (long unsigned int)editor);
            editor->c_window_id = gensym(buffer);
            
            editor->c_owner = x;
            editor->c_text = NULL;
            editor->c_size = 0;
            
            editor->c_font.size   = 12;
            editor->c_font.family = gensym("Helvetica");
            editor->c_font.slant  = gensym("roman");
            editor->c_font.weight = gensym("normal");
            
            editor->c_bgcolor   = rgba_white;
            editor->c_txtcolor  = rgba_black;
            editor->c_wrap      = 0;
            
            pd_bind((t_pd *)editor, editor->c_editor_id);
            
            sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
            sys_vgui("destroy %s\n", editor->c_name->s_name);
            sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
            
            sys_vgui("frame %s -borderwidth 0.0 -highlightthickness 0 \n", editor->c_frame_id->s_name);
            sys_vgui("text %s -borderwidth 0.0 -highlightthickness 0 -insertborderwidth 0\n", editor->c_name->s_name);
            sys_vgui("pack %s -side left -fill both \n", editor->c_name->s_name);
            sys_vgui("pack %s -side bottom -fill both \n", editor->c_frame_id->s_name);
            
            eobj_widget_notify((t_eobj *)x, s_cream_texteditor, editor->c_editor_id, s_cream_create);
        }
        return editor;
    }

    
    return NULL;
}

void etexteditor_destroy(t_etexteditor* editor)
{
    if(editor)
    {
        sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
        sys_vgui("destroy %s\n", editor->c_name->s_name);
        sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
        eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_destroy);
        pd_unbind((t_pd *)editor, editor->c_editor_id);
        free(editor);
    }
}

void etexteditor_settext(t_etexteditor* editor, const char* text)
{
    char* temp;
    const size_t lenght = strlen(text);
    sys_vgui("%s insert 0.0 %s\n", editor->c_name->s_name, text);
    if(!editor->c_text || !editor->c_size)
    {
        editor->c_text = (char *)malloc((size_t)lenght * sizeof(char));
        if(editor->c_text)
        {
            editor->c_size = (int)lenght;
        }
        else
        {
            editor->c_size = 0;
            return;
        }
    }
    else if(editor->c_size < (int)lenght)
    {
        temp = realloc(editor->c_text, (size_t)lenght * sizeof(char));
        if(temp)
        {
            editor->c_text = temp;
            editor->c_size = (int)lenght;
        }
        else
        {
            return;
        }
    }
    memcpy(editor->c_text, text, lenght * sizeof(char));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_gettext(t_etexteditor *editor, char** text)
{
    if(editor->c_text && editor->c_size)
    {
        *text = (char *)malloc((size_t)editor->c_size * sizeof(char));
        if(*text)
        {
            memcpy(*text, editor->c_text, (size_t)editor->c_size * sizeof(char));
        }
        else
        {
            *text = (char *)malloc(sizeof(char));
            *text[0] = '0';
        }
    }
    else
    {
        *text = (char *)malloc(sizeof(char));
        *text[0] = '0';
    }
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_clear(t_etexteditor* editor)
{
    sys_vgui("%s delete 0.0 end\n", editor->c_name->s_name);
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setfont(t_etexteditor *editor, t_efont const* font)
{
    sys_vgui("%s configure -font {{%s} %d %s %s}\n", editor->c_name->s_name,
             font->family->s_name, (int)font->size, font->weight->s_name, font->slant->s_name);
    
    memcpy(&editor->c_font, font, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setbackgroundcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -background %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_bgcolor, color, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_settextcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -foreground %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_txtcolor, color, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setwrap(t_etexteditor *editor, char wrap)
{
    if(wrap)
    {
        sys_vgui("%s configure -wrap word\n", editor->c_name->s_name, (int)wrap);
    }
    else
    {
        sys_vgui("%s configure -wrap none\n", editor->c_name->s_name, (int)wrap);
    }
    
    editor->c_wrap = wrap;
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_popup(t_etexteditor *editor, t_rect const* bounds)
{
    t_rect rect;
    ebox_get_rect_for_view(editor->c_owner, &rect);
    sys_vgui("bind %s <<Modified>> {etext_sendtext %s %s}\n",
             editor->c_name->s_name, editor->c_name->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <<Paste>> {etext_sendtext %s %s}\n",
             editor->c_name->s_name, editor->c_name->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <<Cut>> {etext_sendtext %s %s}\n",
             editor->c_name->s_name, editor->c_name->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <KeyRelease> {etext_sendtext %s %s}\n",
             editor->c_name->s_name, editor->c_name->s_name, editor->c_editor_id->s_name);
    
    sys_vgui("bind %s <KeyPress> {+pdsend {%s texteditor_keypress %s %%k}}\n",
             editor->c_name->s_name, editor->c_owner->b_obj.o_id->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Escape> {+pdsend {%s texteditor_keyfilter %s 0}}\n",
             editor->c_name->s_name, editor->c_owner->b_obj.o_id->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Tab> {+pdsend {%s texteditor_keyfilter %s 1}}\n",
             editor->c_name->s_name, editor->c_owner->b_obj.o_id->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Return> {+pdsend {%s texteditor_keyfilter %s 2}}\n",
             editor->c_name->s_name, editor->c_owner->b_obj.o_id->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Delete> {+pdsend {%s texteditor_keyfilter %s 3}}\n",
             editor->c_name->s_name, editor->c_owner->b_obj.o_id->s_name, editor->c_editor_id->s_name);
    
    sys_vgui("%s create window %d %d -anchor nw -window %s    \
             -tags %s -width %d -height %d \n",
             editor->c_canvas_id->s_name,
             (int)(bounds->x + rect.x), (int)(bounds->y + rect.y),
             editor->c_frame_id->s_name, editor->c_window_id->s_name,
             (int)bounds->width, (int)bounds->height);
    
    memcpy(&editor->c_bounds, bounds, sizeof(t_rect));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_popup);
}

void etexteditor_grabfocus(t_etexteditor *editor)
{
    sys_vgui("focus -force %s\n", editor->c_name->s_name);
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_grabfocus);
}

t_etexteditor* etexteditor_getfromsymbol(t_symbol* name)
{
    t_class* c = etexteditor_setup();
    if(c)
    {
        return (t_etexteditor *)pd_findbyclass(name, c);
    }
    return NULL;
}


void eobj_destroy_properties_window(t_eobj* x)
{
    sys_vgui("destroy .epw%ld\n", (unsigned long)x);
}

void eobj_create_properties_window(t_eobj* x, t_glist *glist)
{
    int i, j, nitems = 0, argc = 0;
    t_atom* argv = NULL;
    t_rgba color;
    char tx[MAXPDSTRING];
    char va[MAXPDSTRING];
    const t_eattr* attr;
    const t_eclass* c = eobj_getclass(x);
    const t_ebox* z = (t_ebox *)x;
    
    sprintf(tx, ".epw%ld", (unsigned long)x);
    sprintf(va, "var%ld", (unsigned long)x);
    
    sys_vgui("destroy %s\n", tx);
    sys_vgui("toplevel %s\n", tx);
    sys_vgui("wm title %s {%s properties}\n", tx, eobj_getclassname(x)->s_name);
    sys_vgui("wm resizable %s 0 0\n", tx);
    
    sys_vgui("frame %s.attrs\n", tx);
    sys_vgui("label %s.attrs.label -justify center -font {Helvetica 13 bold} -text Attributes -height 2\n", tx);
    sys_vgui("pack  %s.attrs.label -side left -fill both -expand 1\n",  tx);
    sys_vgui("grid config %s.attrs -columnspan 5 -row %i -sticky w\n", tx, nitems++);
    
    sys_vgui("frame %s.attrs_menu_label\n", tx);
    sys_vgui("frame %s.attrs_menu_name\n",  tx);
    sys_vgui("frame %s.attrs_menu_values\n",tx);
    
    sys_vgui("label %s.attrs_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",    tx);
    sys_vgui("label %s.attrs_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",      tx);
    sys_vgui("label %s.attrs_menu_values.label -justify left -font {Helvetica 12 bold} -text Values\n",  tx);
    
    sys_vgui("pack  %s.attrs_menu_label.label -side left -fill both -expand 1\n",   tx);
    sys_vgui("pack  %s.attrs_menu_name.label -side left -fill both -expand 1\n",    tx);
    sys_vgui("pack  %s.attrs_menu_values.label -side left -fill both -expand 1\n",  tx);
    
    sys_vgui("grid config %s.attrs_menu_label -column 0 -columnspan 2 -row %i -sticky w\n",   tx, nitems);
    sys_vgui("grid config %s.attrs_menu_name -column 2 -row %i -sticky w\n",                  tx, nitems);
    sys_vgui("grid config %s.attrs_menu_values -column 3 -columnspan 2 -row %i -sticky w\n",  tx, nitems++);
    
    
    for(i = 0; i < c->c_nattr; i++)
    {
        attr = c->c_attr[i];
        if(!attr->invisible)
        {
            sys_vgui("frame %s.attr_label%i\n", tx, i+1);
            sys_vgui("frame %s.attr_name%i\n",  tx, i+1);
            sys_vgui("frame %s.attr_values%i\n",tx, i+1);
            
            sys_vgui("label %s.attr_label%i.label -justify left -font {Helvetica 12} -text \"%s\"\n",
                     tx, i+1, attr->label->s_name);
            sys_vgui("label %s.attr_name%i.label -justify left -font {Helvetica 12 italic} -text \"%s\"\n",
                     tx, i+1, attr->name->s_name);
            
            eobj_attr_getvalueof(x,  attr->name, &argc, &argv);
            if(attr->style == s_cream_checkbutton)
            {
                if(argc && argv && atom_gettype(argv) == A_FLOAT)
                    sys_vgui("set %sattr_value%i %i\n", va, i+1, (int)atom_getfloat(argv));
                else
                    sys_vgui("set %sattr_value%i 0\n", va, i+1);
                sys_vgui("checkbutton %s.attr_values%i.label -variable %sattr_value%i -command {pdsend \"%s %s $%sattr_value%i\"}\n",
                         tx, i+1, va, i+1, x->o_id->s_name, attr->name->s_name, va, i+1);
            }
            else if(attr->style == s_cream_color)
            {
                if(argc && argv && atom_gettype(argv) == A_FLOAT && atom_gettype(argv+1) == A_FLOAT
                   && atom_gettype(argv+2) == A_FLOAT && atom_gettype(argv+3) == A_FLOAT)
                {
                    color.red = atom_getfloat(argv); color.green = atom_getfloat(argv+1);
                    color.blue = atom_getfloat(argv+2); color.alpha = atom_getfloat(argv+3);
                }
                else
                {
                    color = rgba_white;
                }
                sys_vgui("set %sattr_value%i %s\n", va, i+1, rgba_to_hex(&color));
                sys_vgui("entry %s.attr_values%i.label -font {Helvetica 12} -width 20 -readonlybackground $%sattr_value%i -state readonly\n",
                         tx, i+1, va, i+1);
                sys_vgui("bind %s.attr_values%i.label <Button> [concat epicker_apply %s %s $%sattr_value%i %s.attr_values%i.label]\n",
                         tx, i+1, x->o_id->s_name, attr->name->s_name, va, i+1, tx, i+1);
            }
            else if(attr->style == s_cream_number)
            {
                if(argc && argv && atom_gettype(argv) == A_FLOAT)
                    sys_vgui("set %sattr_value%i %g\n", va, i+1, atom_getfloat(argv));
                else
                    sys_vgui("set %sattr_value%i 0\n", va, i+1);
                sys_vgui("spinbox %s.attr_values%i.label -font {Helvetica 12} -width 18 \
                         -textvariable %sattr_value%i -command {pdsend \"%s %s $%sattr_value%i\"} \
                         -increment %f -from %f -to %f\n", tx, i+1, va, i+1,
                         x->o_id->s_name, attr->name->s_name, va, i+1,
                         attr->step, (attr->clipped % 2) ? attr->minimum : FLT_MIN,
                         (attr->clipped > 1) ? attr->maximum : FLT_MAX);
                sys_vgui("bind %s.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $%sattr_value%i\"}\n",
                         tx, i+1, x->o_id->s_name, attr->name->s_name, va, i+1);
            }
            else if(attr->style == s_cream_menu && atom_gettype(argv) == A_SYMBOL)
            {
                if(argc && argv && atom_gettype(argv) == A_SYMBOL)
                    sys_vgui("set %sattr_value%i %s\n", va, i+1,  atom_getsymbol(argv)->s_name);
                else
                    sys_vgui("set %sattr_value%i \"\"\n", va, i+1);
                sys_vgui("spinbox %s.attr_values%i.label -font {Helvetica 12} -width 18 -state readonly\
                         -textvariable [string trim %sattr_value%i] -command {pdsend \"%s %s $%sattr_value%i\"} \
                         -values {", tx, i+1, va, i+1,
                         x->o_id->s_name, attr->name->s_name, va, i+1);
                for(j = 0; j < attr->itemssize; j++)
                {
                    sys_vgui("%s ", attr->itemslist[attr->itemssize - 1 - j]->s_name);
                }
                sys_vgui("}\n");
            }
            else if(attr->style == s_cream_font)
            {
                if(argc && argv && atom_gettype(argv) == A_SYMBOL && atom_gettype(argv+1) == A_FLOAT
                   && atom_gettype(argv+2) == A_SYMBOL && atom_gettype(argv+3) == A_SYMBOL)
                {
                    sys_vgui("set %sattr_value%i \"{%s} %i %s %s\"\n", va, i+1, atom_getsymbol(argv)->s_name, (int)atom_getfloat(argv+1), atom_getsymbol(argv+2)->s_name, atom_getsymbol(argv+3)->s_name);
                    sys_vgui("entry %s.attr_values%i.label -font {\"%s\" 12 %s %s} -width 20 -textvariable %sattr_value%i -state readonly\n"
                             ,tx, i+1,
                             atom_getsymbol(argv)->s_name, atom_getsymbol(argv+2)->s_name, atom_getsymbol(argv+3)->s_name,
                             va, i+1);
                }
                else
                {
                    sys_vgui("set %sattr_value%i \"\"\n", va, i+1);
                    sys_vgui("entry %s.attr_values%i.label -font {Helvetica 12} -width 20 -textvariable %sattr_value%i -state readonly\n"
                             ,tx, i+1,
                             atom_getsymbol(argv)->s_name, atom_getsymbol(argv+2)->s_name, atom_getsymbol(argv+3)->s_name,
                             va, i+1);
                }
                
                sys_vgui("bind %s.attr_values%i.label <Button> [concat efont_apply %s %s {$%sattr_value%i} %s.attr_values%i.label]\n",
                         tx, i+1, x->o_id->s_name, attr->name->s_name, va, i+1, tx, i+1);
            }
            else
            {
                
                if(argc && argv)
                {
                    sys_vgui("set %sattr_value%i [concat ", va, i+1);
                    for(j = 0; j < argc - 1; j++)
                    {
                        if(atom_gettype(argv+j) == A_FLOAT)
                        {
                            sys_vgui("%g ", atom_getfloat(argv+j));
                        }
                        else if(atom_gettype(argv+j) == A_SYMBOL)
                        {
                            sys_vgui("'%s' ", atom_getsymbol(argv+j)->s_name);
                        }
                    }
                    if(atom_gettype(argv+argc-1) == A_FLOAT)
                    {
                        sys_vgui("%g", atom_getfloat(argv+argc-1));
                    }
                    else if(atom_gettype(argv+argc-1) == A_SYMBOL && is_valid_symbol(atom_getsymbol(argv+argc-1)))
                    {
                        sys_vgui("'%s'", atom_getsymbol(argv+argc-1)->s_name);
                    }
                    sys_gui("]\n");
                }
                else
                {
                    sys_vgui("set %sattr_value%i \"\"", va, i+1);
                }
                sys_vgui("entry %s.attr_values%i.label -font {Helvetica 12} -width 20 \
                         -textvariable %sattr_value%i\n", tx, i+1, va, i+1);
                sys_vgui("bind %s.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $%sattr_value%i\"}\n",
                         tx, i+1, x->o_id->s_name, attr->name->s_name, va, i+1);
                
            }
            if(argc && argv)
            {
                free(argv);
            }
            argv = NULL;
            argc = 0;
            
            sys_vgui("pack %s.attr_label%i.label    -side left -fill both -expand 1\n",     tx, i+1);
            sys_vgui("pack %s.attr_name%i.label     -side left -fill both -expand 1\n",     tx, i+1);
            sys_vgui("pack %s.attr_values%i.label   -side left -fill both -expand 1\n",    tx, i+1);
            
            sys_vgui("grid config %s.attr_label%i   -column 0 -columnspan 2 -row %i -sticky w\n", tx, i+1, nitems);
            sys_vgui("grid config %s.attr_name%i    -column 2 -row %i -sticky w\n",               tx, i+1, nitems);
            sys_vgui("grid config %s.attr_values%i  -column 3 -columnspan 2 -row %i -sticky w\n", tx, i+1, nitems++);
        }
    }
    if(eobj_isbox(x) && z->b_nparams)
    {
        sys_vgui("frame %s.params\n", tx);
        sys_vgui("label %s.params.label -justify center -font {Helvetica 13  bold} -text Parameters -height 2\n", tx);
        sys_vgui("pack  %s.params.label -side left -fill both -expand 1\n",  tx);
        sys_vgui("grid config %s.params -columnspan 5 -row %i -sticky w\n", tx, nitems++);
        
        sys_vgui("frame %s.params_menu_index\n",tx);
        sys_vgui("frame %s.params_menu_name\n", tx);
        sys_vgui("frame %s.params_menu_label\n",tx);
        
        sys_vgui("label %s.params_menu_index.label -justify left -font {Helvetica 12 bold} -text Index\n",   tx);
        sys_vgui("label %s.params_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",     tx);
        sys_vgui("label %s.params_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",   tx);
        
        sys_vgui("pack  %s.params_menu_index.label -side left -fill both -expand 1\n",  tx);
        sys_vgui("pack  %s.params_menu_name.label -side left -fill both -expand 1\n",   tx);
        sys_vgui("pack  %s.params_menu_label.label -side left -fill both -expand 1\n",  tx);
        
        sys_vgui("grid config %s.params_menu_index -column 0 -row %i -sticky w\n",    tx, nitems);
        sys_vgui("grid config %s.params_menu_name -column 1 -row %i -sticky w\n",     tx, nitems);
        sys_vgui("grid config %s.params_menu_label -column 2 -row %i -sticky w\n",    tx, nitems++);
        
        
        for(i = 0; i < z->b_nparams; i++)
        {
            if(z->b_params[i])
            {
                sys_vgui("frame %s.param_menu_index%i\n",   tx, i+1);
                sys_vgui("frame %s.param_menu_name%i\n",    tx, i+1);
                sys_vgui("frame %s.param_menu_label%i\n",   tx, i+1);
                
                sys_vgui("label %s.param_menu_index%i.entry -font {Helvetica 12} -width 1\
                         -text \"%i\"\n",tx, i+1, i+1);
                sys_vgui("set %sparam_menu_name%i \"%s\"\n", va, i+1, z->b_params[i]->p_name->s_name);
                sys_vgui("entry %s.param_menu_name%i.entry -font {Helvetica 12} -width 13 \
                         -textvariable %sparam_menu_name%i -state %s\n", tx, i+1, va, i+1,
                         (z->b_params[i]->p_flags & EPARAM_STATIC_NAME) ? "disable" : "normal");
                sys_vgui("set %sparam_menu_label%i \"%s\"\n", va, i+1, z->b_params[i]->p_label->s_name);
                sys_vgui("entry %s.param_menu_label%i.entry -font {Helvetica 12} -width 15 \
                         -textvariable %sparam_menu_label%i -state %s\n", tx, i+1, va, i+1,
                         (z->b_params[i]->p_flags & EPARAM_STATIC_LABEL) ? "disable" : "normal");
                
                sys_vgui("bind %s.param_menu_name%i.entry <KeyPress-Return> {pdsend \"%s param %i name $%sparam_menu_name%i\"}\n",
                         tx, i+1, x->o_id->s_name, i+1, va, i+1);
                sys_vgui("bind %s.param_menu_label%i.entry <KeyPress-Return> {pdsend \"%s param %i label $%sparam_menu_label%i\"}\n",
                         tx, i+1, x->o_id->s_name, i+1, va, i+1);
                
                sys_vgui("pack  %s.param_menu_index%i.entry -side left -fill both -expand 1\n",  tx);
                sys_vgui("pack  %s.param_menu_name%i.entry -side left -fill both -expand 1\n",  tx);
                sys_vgui("pack  %s.param_menu_label%i.entry -side left -fill both -expand 1\n",  tx);
                
                sys_vgui("grid config %s.param_menu_index%i -column 0 -row %i -sticky w\n",   tx, i+1, nitems);
                sys_vgui("grid config %s.param_menu_name%i -column 1 -row %i -sticky w\n",    tx, i+1, nitems);
                sys_vgui("grid config %s.param_menu_label%i -column 2 -row %i -sticky w\n",   tx, i+1, nitems++);
            }
        }
    }
}


void tcltk_create_methods(void)
{
    t_symbol* epd_symbol = gensym("epd1572");
    if(!epd_symbol->s_thing)
    {
        // PATCHER MOUSE MOTION //
        sys_vgui("proc eobj_canvas_motion {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_motion $patcher [expr $x - $rx] [expr $y - $ry] $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE DOWN //
        sys_vgui("proc eobj_canvas_down {patcher val} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouse $patcher [expr $x - $rx] [expr $y - $ry] 0 $val\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE UP //
        sys_vgui("proc eobj_canvas_up {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_mouseup $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
        
        // PATCHER MOUSE RIGHT //
        sys_vgui("proc eobj_canvas_right {patcher} {\n");
        sys_gui(" set rx [winfo rootx $patcher]\n");
        sys_gui(" set ry [winfo rooty $patcher]\n");
        sys_gui(" set x  [winfo pointerx .]\n");
        sys_gui(" set y  [winfo pointery .]\n");
        sys_vgui(" pdtk_canvas_rightclick $patcher [expr $x - $rx] [expr $y - $ry] 0\n");
        sys_gui("}\n");
        
        // OBJECT SAVE FILE //
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
        
        // OBJECT OPEN FILE //
        sys_gui("proc eobj_openfrom {name} {\n");
        sys_gui("if { ! [file isdirectory $::filenewdir]} {\n");
        sys_gui("set ::filenewdir [file normalize $::env(HOME)]\n");
        sys_gui("}\n");
        sys_gui("set files [tk_getOpenFile -multiple true -initialdir $::fileopendir]\n");
        sys_gui("pdsend \"$name eobjreadfrom [enquote_path $files]\"\n");
        sys_gui("}\n");
        
        // RGBA TO HEX //
        sys_gui("proc eobj_rgba_to_hex {red green blue alpha} { \n");
        sys_gui("set nR [expr int( $red * 65025 )]\n");
        sys_gui("set nG [expr int( $green * 65025 )]\n");
        sys_gui("set nB [expr int( $blue * 65025 )]\n");
        sys_gui("set col [format {%4.4x} $nR]\n");
        sys_gui("append col [format {%4.4x} $nG]\n");
        sys_gui("append col [format {%4.4x} $nB]\n");
        sys_gui("return #$col\n");
        sys_gui("}\n");
        
        // SEND TEXTFIELD TEXT //
        sys_gui("proc etext_sendtext {widget name} { \n");
        sys_gui("set text [$widget get 0.0 end]\n");
        sys_gui("pdsend \"$name text $text\"\n");
        sys_gui("}\n");
        
        // COLOR PICKER WINOW //
        sys_gui("proc epicker_apply {objid attrname initcolor sentry} { \n");
        sys_gui("set color [tk_chooseColor -title \"Choose Color\" -initialcolor $initcolor]\n");
        sys_gui("if {$color == \"\"} return \n");
        sys_gui("foreach {red2 green2 blue2} [winfo rgb . $color] {}\n");
        sys_gui("set nR2 [expr ( $red2 / 65025. )]\n");
        sys_gui("set nG2 [expr ( $green2 / 65025. )]\n");
        sys_gui("set nB2 [expr ( $blue2 / 65025. )]\n");
        sys_gui("if {$nR2 > 1.} {set nR2 1.} \n");
        sys_gui("if {$nG2 > 1.} {set nG2 1.} \n");
        sys_gui("if {$nB2 > 1.} {set nB2 1.} \n");
        sys_gui("pdsend \"$objid $attrname $nR2 $nG2 $nB2\"\n");
        sys_gui("$sentry configure -readonlybackground $color\n");
        sys_gui("}\n");
        
        // FONT PICKER WINOW //
        sys_gui("proc efont_apply {objid attrname initfont sentry} { \n");
        sys_gui("set nfont [::ChooseFont::ChooseFont $initfont]\n");
        sys_gui("pdsend \"$objid $attrname $nfont\"\n");
        sys_gui("$sentry configure -state normal\n");
        sys_gui("$sentry delete 0 end\n");
        sys_gui("$sentry insert 0 [string trim $nfont]\n");
        sys_gui("$sentry configure -state readonly\n");
        sys_gui("}\n");
        

        sys_gui("namespace eval ::ChooseFont {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   set S(W) .cfont\n");
        sys_gui("   set S(fonts) [lsort -dictionary [font families]]\n");
        sys_gui("   set S(styles) {Regular Italic Bold \"Bold Italic\"}\n");
        sys_gui("\n");
        sys_gui("   set S(sizes) {8 9 10 11 12 14 16 18 20 22 24 26 28 36 48 72}\n");
        sys_gui("   set S(strike) 0\n");
        sys_gui("   set S(under) 0\n");
        sys_gui("   set S(first) 1\n");
        sys_gui("\n");
        sys_gui("   set S(fonts,lcase) {}\n");
        sys_gui("   foreach font $S(fonts) { lappend S(fonts,lcase) [string tolower $font]}\n");
        sys_gui("   set S(styles,lcase) {regular italic bold \"bold italic\"}\n");
        sys_gui("   set S(sizes,lcase) $S(sizes)\n");
        sys_gui("\n");
        sys_gui("}\n");
        sys_gui("proc ::ChooseFont::ChooseFont {{defaultFont \"\"}} {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   destroy $S(W)\n");
        sys_gui("   toplevel $S(W) -padx 10 -pady 10\n");
        sys_gui("   wm title $S(W) \"Font\"\n");
        sys_gui("\n");
        sys_gui("   set tile [expr {[catch {package present tile}] ? \"\" : \"::ttk\"}]\n");
        sys_gui("\n");
        sys_gui("   ${tile}::label $S(W).font -text \"Font:\"\n");
        sys_gui("   ${tile}::label $S(W).style -text \"Font style:\"\n");
        sys_gui("   ${tile}::label $S(W).size -text \"Size:\"\n");
        sys_gui("   entry $S(W).efont -textvariable ::ChooseFont::S(font) ;# -state disabled\n");
        sys_gui("   entry $S(W).estyle -textvariable ::ChooseFont::S(style) ;# -state disabled\n");
        sys_gui("   entry $S(W).esize -textvariable ::ChooseFont::S(size) -width 0 \
                -validate key -vcmd {string is double %P}\n");
        sys_gui("\n");
        sys_gui("   ${tile}::scrollbar $S(W).sbfonts -command [list $S(W).lfonts yview]\n");
        sys_gui("   listbox $S(W).lfonts -listvariable ::ChooseFont::S(fonts) -height 7  \
                -yscroll [list $S(W).sbfonts set] -height 7 -exportselection 0\n");
        sys_gui("   listbox $S(W).lstyles -listvariable ::ChooseFont::S(styles) -height 7  \
                -exportselection 0\n");
        sys_gui("   ${tile}::scrollbar $S(W).sbsizes -command [list $S(W).lsizes yview]\n");
        sys_gui("   listbox $S(W).lsizes -listvariable ::ChooseFont::S(sizes)  \
                -yscroll [list $S(W).sbsizes set] -width 6 -height 7 -exportselection 0\n");
        sys_gui("\n");
        sys_gui("   bind $S(W).lfonts <<ListboxSelect>> [list ::ChooseFont::Click font]\n");
        sys_gui("   bind $S(W).lstyles <<ListboxSelect>> [list ::ChooseFont::Click style]\n");
        sys_gui("   bind $S(W).lsizes <<ListboxSelect>> [list ::ChooseFont::Click size]\n");
        sys_gui("\n");
        sys_gui("   set WE $S(W).effects\n");
        sys_gui("   ${tile}::labelframe $WE -text \"Effects\"\n");
        sys_gui("   ${tile}::checkbutton $WE.strike -variable ::ChooseFont::S(strike)  \
                -text Strikeout -command [list ::ChooseFont::Click strike]\n");
        sys_gui("   ${tile}::checkbutton $WE.under -variable ::ChooseFont::S(under)  \
                -text Underline -command [list ::ChooseFont::Click under]\n");
        sys_gui("\n");
        sys_gui("   ${tile}::button $S(W).ok -text OK -command [list ::ChooseFont::Done 1]\n");
        sys_gui("   ${tile}::button $S(W).cancel -text Cancel -command [list ::ChooseFont::Done 0]\n");
        sys_gui("   wm protocol $S(W) WM_DELETE_WINDOW [list ::ChooseFont::Done 0]\n");
        sys_gui("\n");
        sys_gui("   grid $S(W).font - x $S(W).style - x $S(W).size - x -sticky w\n");
        sys_gui("   grid $S(W).efont - x $S(W).estyle - x $S(W).esize - x $S(W).ok -sticky ew\n");
        sys_gui("   grid $S(W).lfonts $S(W).sbfonts x  \
                $S(W).lstyles - x  \
                $S(W).lsizes $S(W).sbsizes x  \
                $S(W).cancel -sticky news\n");
        sys_gui("   grid config $S(W).cancel -sticky n -pady 5\n");
        sys_gui("   grid columnconfigure $S(W) {2 5 8} -minsize 10\n");
        sys_gui("   grid columnconfigure $S(W) {0 3 6} -weight 1\n");
        sys_gui("\n");
        sys_gui("   grid $WE.strike -sticky w -padx 10\n");
        sys_gui("   grid $WE.under -sticky w -padx 10\n");
        sys_gui("   grid columnconfigure $WE 1 -weight 1\n");
        sys_gui("   grid $WE - x -sticky news -row 100 -column 0\n");
        sys_gui("\n");
        sys_gui("   set WS $S(W).sample\n");
        sys_gui("   ${tile}::labelframe $WS -text \"Sample\"\n");
        sys_gui("   label $WS.fsample -bd 2 -relief sunken\n");
        sys_gui("   label $WS.fsample.sample -text \"AaBbYyZz\"\n");
        sys_gui("   set S(sample) $WS.fsample.sample\n");
        sys_gui("   pack $WS.fsample -fill both -expand 1 -padx 10 -pady 10 -ipady 15\n");
        sys_gui("   pack $WS.fsample.sample -fill both -expand 1\n");
        sys_gui("   pack propagate $WS.fsample 0\n");
        sys_gui("\n");
        sys_gui("   grid rowconfigure $S(W) 2 -weight 1\n");
        sys_gui("   grid rowconfigure $S(W) 99 -minsize 30\n");
        sys_gui("   grid $WS - - - - -sticky news -row 100 -column 3\n");
        sys_gui("   grid rowconfigure $S(W) 101 -minsize 30\n");
        sys_gui("\n");
        sys_gui("   trace variable ::ChooseFont::S(size) w ::ChooseFont::Tracer\n");
        sys_gui("   trace variable ::ChooseFont::S(style) w ::ChooseFont::Tracer\n");
        sys_gui("   trace variable ::ChooseFont::S(font) w ::ChooseFont::Tracer\n");
        sys_gui("   ::ChooseFont::Init $defaultFont\n");
        sys_gui("   tkwait window $S(W)\n");
        sys_gui("   return $S(result)\n");
        sys_gui("}\n");
        sys_gui("\n");
        sys_gui("proc ::ChooseFont::Done {ok} {\n");
        sys_gui("   if {! $ok} {set ::ChooseFont::S(result) \"\"}\n");
        sys_gui("   destroy $::ChooseFont::S(W)\n");
        sys_gui("}\n");
        sys_gui("proc ::ChooseFont::Init {{defaultFont \"\"}} {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   if {$S(first) || $defaultFont ne \"\"} {\n");
        sys_gui("       if {$defaultFont eq \"\"} {\n");
        sys_gui("           set defaultFont [[entry .___e] cget -font]\n");
        sys_gui("           destroy .___e\n");
        sys_gui("       }\n");
        sys_gui("       array set F [font actual $defaultFont]\n");
        sys_gui("       set S(font) $F(-family)\n");
        sys_gui("       set S(size) $F(-size)\n");
        sys_gui("       set S(strike) $F(-overstrike)\n");
        sys_gui("       set S(under) $F(-underline)\n");
        sys_gui("       set S(style) \"Regular\"\n");
        sys_gui("       if {$F(-weight) eq \"bold\" && $F(-slant) eq \"italic\"} {\n");
        sys_gui("           set S(style) \"Bold Italic\"\n");
        sys_gui("       } elseif {$F(-weight) eq \"bold\"} {\n");
        sys_gui("           set S(style) \"Bold\"\n");
        sys_gui("       } elseif {$F(-slant) eq \"italic\"} {\n");
        sys_gui("           set S(style) \"Italic\"\n");
        sys_gui("       }\n");
        sys_gui("\n");
        sys_gui("       set S(first) 0\n");
        sys_gui("   }\n");
        sys_gui("\n");
        sys_gui("   ::ChooseFont::Tracer a b c\n");
        sys_gui("   ::ChooseFont::Show\n");
        sys_gui("}\n");
        sys_gui("\n");
        sys_gui("proc ::ChooseFont::Click {who} {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   if {$who eq \"font\"} {\n");
        sys_gui("       set S(font) [$S(W).lfonts get [$S(W).lfonts curselection]]\n");
        sys_gui("   } elseif {$who eq \"style\"} {\n");
        sys_gui("       set S(style) [$S(W).lstyles get [$S(W).lstyles curselection]]\n");
        sys_gui("   } elseif {$who eq \"size\"} {\n");
        sys_gui("       set S(size) [$S(W).lsizes get [$S(W).lsizes curselection]]\n");
        sys_gui("   }\n");
        sys_gui("   ::ChooseFont::Show\n");
        sys_gui("}\n");
        sys_gui("proc ::ChooseFont::Tracer {var1 var2 op} {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   set bad 0\n");
        sys_gui("   set nstate normal\n");
        sys_gui("   foreach var {font style size} {\n");
        sys_gui("       set value [string tolower $S($var)]\n");
        sys_gui("       $S(W).l${var}s selection clear 0 end\n");
        sys_gui("       set n [lsearch -exact $S(${var}s,lcase) $value]\n");
        sys_gui("       $S(W).l${var}s selection set $n\n");
        sys_gui("       if {$n != -1} {\n");
        sys_gui("           set S($var) [lindex $S(${var}s) $n]\n");
        sys_gui("           $S(W).e$var icursor end\n");
        sys_gui("           $S(W).e$var selection clear\n");
        sys_gui("       } else {                                ;\n");
        sys_gui("           set n [lsearch -glob $S(${var}s,lcase) \"$value*\"]\n");
        sys_gui("           set bad 1\n");
        sys_gui("           if {$var ne \"size\" || ! [string is double -strict $value]} {\n");
        sys_gui("               set nstate disabled\n");
        sys_gui("           }\n");
        sys_gui("       }\n");
        sys_gui("       $S(W).l${var}s see $n\n");
        sys_gui("   }\n");
        sys_gui("   if {! $bad} ::ChooseFont::Show\n");
        sys_gui("   $S(W).ok config -state $nstate\n");
        sys_gui("}\n");
        sys_gui("\n");
        sys_gui("proc ::ChooseFont::Show {} {\n");
        sys_gui("   variable S\n");
        sys_gui("\n");
        sys_gui("   set S(result) [list $S(font) $S(size)]\n");
        sys_gui("   if {$S(style) eq \"Bold\"} { lappend S(result) bold }\n");
        sys_gui("   if {$S(style) eq \"Italic\"} { lappend S(result) italic }\n");
        sys_gui("   if {$S(style) eq \"Bold Italic\"} { lappend S(result) bold italic}\n");
        sys_gui("   if {$S(strike)} { lappend S(result) overstrike}\n");
        sys_gui("   if {$S(under)} { lappend S(result) underline}\n");
        sys_gui("\n");
        sys_gui("   $S(sample) config -font $S(result)\n");
        sys_gui("}\n");
        
        
        
        epd_symbol->s_thing = (t_class **)1;
    }
}




