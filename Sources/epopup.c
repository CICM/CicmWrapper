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
#include "elayer.h"
#include "eattr.h"

static void eobj_widget_notify(void* obj, t_symbol* widget, t_symbol* name, t_symbol* action)
{
    t_atom av[2];
    /*
    if(is_valid_symbol(obj->o_camo_id) && obj->o_camo_id->s_thing)
    {
        atom_setsym(av, name);
        atom_setsym(av+1, action);
        pd_typedmess(obj->o_camo_id->s_thing, widget, 2, av);
    }
     */
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
    /*
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
     */
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
                     eobj_getid(popup->c_owner)->s_name, popup->c_popup_id->s_name, (float)popup->c_items[i].c_id,
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

t_epopup* epopup_findbyname(t_symbol* name)
{
    t_class* c = epopup_setup();
    if(c)
    {
        return (t_epopup *)pd_findbyclass(name, c);
    }
    return NULL;
}








static void etexteditor_text(t_etextlayouteditor* x, t_symbol* s, int argc, t_atom* argv)
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
            strncat(x->c_text, atom_getsymbol(argv+argc-1)->s_name, MAXPDSTRING);
        }
    }
}

static t_class* etexteditor_setup()
{
    t_class* etexteditor_class = NULL;
    t_symbol* etexteditor1572_sym = gensym("etexteditor1572");
    if(!etexteditor1572_sym->s_thing)
    {
        etexteditor_class = class_new(gensym("etexteditor"), NULL, (t_method)NULL, sizeof(t_etextlayouteditor), CLASS_PD, A_GIMME, 0);
        etexteditor1572_sym->s_thing = (t_class **)etexteditor_class;
        class_addmethod(etexteditor_class, (t_method)etexteditor_text, gensym("text"), A_GIMME, 0);
        return etexteditor_class;
    }
    else
    {
        return (t_class *)etexteditor1572_sym->s_thing;
    }
}

t_etextlayouteditor* etexteditor_create(t_ebox* x)
{
    char buffer[MAXPDSTRING];
    t_canvas* canvas = NULL; t_etextlayouteditor* editor = NULL;
    t_class* c = etexteditor_setup();
    if(c)
    {
        editor = (t_etextlayouteditor *)pd_new(c);
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
            
            editor->c_owner = (t_object *)x;
            editor->c_text = NULL;
            editor->c_size = 0;
            efont_init(editor->c_font, gensym("Helvetica"), 0, 0, 12);
            
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
            sys_vgui("%s delete 0.0 end\n", editor->c_name->s_name);
            eobj_widget_notify((t_object *)x, s_cream_texteditor, editor->c_editor_id, s_cream_create);
        }
        return editor;
    }

    
    return NULL;
}

void etexteditor_destroy(t_etextlayouteditor* editor)
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

void etexteditor_settext(t_etextlayouteditor* editor, const char* text)
{
    char* temp;
    const size_t lenght = strlen(text);
    sys_vgui("%s insert 0.0 {%s}\n", editor->c_name->s_name, text);
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

void etexteditor_gettext(t_etextlayouteditor *editor, char** text)
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

void etexteditor_clear(t_etextlayouteditor* editor)
{
    sys_vgui("%s delete 0.0 end\n", editor->c_name->s_name);
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setfont(t_etextlayouteditor *editor, t_efont const* font)
{
    sys_vgui("%s configure -font {{%s} %d %s %s}\n", editor->c_name->s_name,
             font->family->s_name, (int)font->size, font->weight->s_name, font->slant->s_name);
    
    memcpy(&editor->c_font, font, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setbackgroundcolor(t_etextlayouteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -background %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_bgcolor, color, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_settextcolor(t_etextlayouteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -foreground %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_txtcolor, color, sizeof(t_efont));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_attr_modified);
}

void etexteditor_setwrap(t_etextlayouteditor *editor, char wrap)
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

void etexteditor_popup(t_etextlayouteditor *editor, t_rect const* bounds)
{
    t_rect rect;
    ebox_getbounds((t_ebox *)editor->c_owner, NULL, &rect);
    sys_vgui("bind %s <KeyRelease> {+pdsend \"%s text [%s get 0.0 end]\"}\n",
             editor->c_name->s_name, editor->c_editor_id->s_name, editor->c_name->s_name);
    sys_vgui("bind %s <<Modified>> {+pdsend \"%s text [%s get 0.0 end]\"}\n",
             editor->c_name->s_name, editor->c_editor_id->s_name, editor->c_name->s_name);
    sys_vgui("bind %s <<Paste>> {+pdsend \"%s text [%s get 0.0 end]\"}\n",
             editor->c_name->s_name, editor->c_editor_id->s_name, editor->c_name->s_name);
    sys_vgui("bind %s <<Cut>> {+pdsend \"%s text [%s get 0.0 end]\"}\n",
             editor->c_name->s_name, editor->c_editor_id->s_name, editor->c_name->s_name);

    sys_vgui("bind %s <KeyPress> {+pdsend {%s texteditor_keypress %s %%k}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Delete> {+pdsend {%s texteditor_keyfilter %s 0}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Tab> {+pdsend {%s texteditor_keyfilter %s 1}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Return> {+pdsend {%s texteditor_keyfilter %s 2}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <Escape> {+pdsend {%s texteditor_keyfilter %s 3}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <FocusIn> {+pdsend {%s texteditor_focus %s 1}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    sys_vgui("bind %s <FocusOut> {+pdsend {%s texteditor_focus %s 0}}\n",
             editor->c_name->s_name, eobj_getid(editor->c_owner)->s_name, editor->c_editor_id->s_name);
    
    sys_vgui("%s create window %d %d -anchor nw -window %s    \
             -tags %s -width %d -height %d \n",
             editor->c_canvas_id->s_name,
             (int)(bounds->x + rect.x), (int)(bounds->y + rect.y),
             editor->c_frame_id->s_name, editor->c_window_id->s_name,
             (int)bounds->width, (int)bounds->height);
    
    memcpy(&editor->c_bounds, bounds, sizeof(t_rect));
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_popup);
}

void etexteditor_grabfocus(t_etextlayouteditor *editor)
{
    sys_vgui("focus -force %s\n", editor->c_name->s_name);
    eobj_widget_notify((t_eobj *)(editor->c_owner), s_cream_texteditor, editor->c_editor_id, s_cream_grabfocus);
}

t_etextlayouteditor* etexteditor_getfromsymbol(t_symbol* name)
{
    t_class* c = etexteditor_setup();
    if(c)
    {
        return (t_etextlayouteditor *)pd_findbyclass(name, c);
    }
    return NULL;
}






static t_class* ewindowprop_setup()
{
    t_class* eclass = NULL;
    t_pd* obj = gensym("ewindowprop1572")->s_thing;
    if(!obj)
    {
        eclass = class_new(gensym("ewindowprop"), NULL, (t_method)NULL, sizeof(t_ewindowprop), CLASS_PD, A_GIMME, 0);
        obj = pd_new(eclass);
        pd_bind(obj, gensym("ewindowprop1572"));
        return eclass;
    }
    else
    {
        return *obj;
    }
}

t_ewindowprop* ewindowprop_getfromobject(t_eobj* x)
{
    char text[MAXPDSTRING];
    t_class* c = ewindowprop_setup();
    if(c)
    {
        sprintf(text, "windowprop%lx", (unsigned long)x);
        return (t_ewindowprop *)pd_findbyclass(gensym(text), c);
    }
    return NULL;
}

void ewindowprop_destroy(t_eobj* x)
{
    char text[MAXPDSTRING];
    t_ewindowprop* wm = ewindowprop_getfromobject(x);
    if(wm)
    {
        sys_vgui("destroy .epw%lx\n", (unsigned long)x);
        sprintf(text, "windowprop%lx", (unsigned long)x);
        pd_unbind((t_pd *)wm, gensym(text));
        free(wm);
    }
}

void ewindowprop_update(t_eobj* x)
{
    int i, j, argc;
    t_atom* argv;
    t_rgba color;
    char text[MAXPDSTRING];
    t_ewindowprop* wm = ewindowprop_getfromobject(x);
    const unsigned long ref = (unsigned long)x;
    t_eclass const* c = eobj_getclass(x);
    if(wm)
    {
        /*
        for(i = 0; i < c->c_nattr; i++)
        {
            if(!c->c_attr[i]->invisible)
            {
                eobj_attr_getvalue(x,  c->c_attr[i]->name, &argc, &argv);
                if(c->c_attr[i]->style == s_cream_checkbutton)
                {
                    sys_vgui("set var%lxattr_value%i %lx\n", ref, i+1, atom_getintarg(0, argc, argv));
                }
                else if(c->c_attr[i]->style == s_cream_color)
                {
                    atom_getcolorarg(0, argc, argv, &color);
                    sys_vgui("set var%lxattr_value%i %s\n", ref, i+1, rgba_to_hex(&color));
                    sys_vgui("if {[winfo exists .epw%lx]} {\n \
                             .epw%lx.attr_values%i.label configure -readonlybackground $var%lxattr_value%i\n}\n",
                             ref, ref, i+1, ref, i+1);
                }
                else if(c->c_attr[i]->style == s_cream_number)
                {
                    sys_vgui("set var%lxattr_value%i %g\n", ref, i+1, atom_getfloatarg(0, argc, argv));
                }
                else if(c->c_attr[i]->style == s_cream_menu)
                {
                    sys_vgui("set var%lxattr_value%i \"%s\"\n", ref, i+1, atom_getsymbolarg(0, argc, argv));
                }
                else
                {
                    if(argc && argv)
                    {
                        sys_vgui("set var%lxattr_value%i [concat ", ref, i+1);
                        for(j = 0; j < argc; j++)
                        {
                            if(atom_gettype(argv+j) == A_FLOAT)
                            {
                                sys_vgui("%g ", atom_getfloat(argv+j));
                            }
                            else if(atom_gettype(argv+j) == A_SYMBOL)
                            {
                                atom_string(argv+j, text, MAXPDSTRING);
                                sys_vgui("'%s' ", text);
                            }
                        }
                        sys_gui("]\n");
                    }
                    else
                    {
                        sys_vgui("set var%lxattr_value%i \"\"\n", ref, i+1);
                    }
                }
            }
        }
         */
    }
}

static void ewindowprop_addattr(t_ewindowprop* x, t_eattr* attr, int i)
{
    int j, argc;
    t_atom* argv;
    t_rgba color;
    char text[MAXPDSTRING];
    const unsigned long ref = (unsigned long)x->c_owner;
    
    /*
    if(attr->style == s_cream_font)
        return;
    
    sys_vgui("frame .epw%lx.attr_label%i\n", ref, i+1);
    sys_vgui("frame .epw%lx.attr_name%i\n",  ref, i+1);
    sys_vgui("frame .epw%lx.attr_values%i\n",ref, i+1);
    
    sys_vgui("label .epw%lx.attr_label%i.label -justify left -font {Helvetica 12} -text \"%s\"\n", ref, i+1, attr->label->s_name);
    sys_vgui("label .epw%lx.attr_name%i.label -justify left -font {Helvetica 12 italic} -text \"%s\"\n", ref, i+1, attr->name->s_name);
    
    eobj_attr_getvalue(x->c_owner,  attr->name, &argc, &argv);
    if(attr->style == s_cream_checkbutton)
    {
        sys_vgui("set var%lxattr_value%i %f\n", ref, i+1, (int)atom_getfloatarg(0, argc, argv));
        
        sys_vgui("checkbutton .epw%lx.attr_values%i.label ", ref, i+1);
        sys_vgui("-variable var%lxattr_value%i -command {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
    }
    else if(attr->style == s_cream_color)
    {
        atom_getcolorarg(0, argc, argv, &color);
        sys_vgui("set var%lxattr_value%i %s\n", ref, i+1, rgba_to_hex(&color));
        
        sys_vgui("entry .epw%lx.attr_values%i.label -font {Helvetica 12} -width 20 ", ref, i+1);
        sys_vgui("-readonlybackground $var%lxattr_value%i -state readonly\n", ref, i+1);

        sys_vgui("bind .epw%lx.attr_values%i.label <Button> ", ref, i+1);
        sys_vgui("[concat epicker_apply %s %s $var%lxattr_value%i .epw%lx.attr_values%i.label]\n",
                 eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1, ref, i+1);
    }
    else if(attr->style == s_cream_number)
    {
        sys_vgui("set var%lxattr_value%i %g\n", ref, i+1, atom_getfloatarg(0, argc, argv));
        
        sys_vgui("spinbox .epw%lx.attr_values%i.label -font {Helvetica 12} -width 18 ", ref, i+1);
        sys_vgui("-textvariable var%lxattr_value%i -command {pdsend \"%s %s $var%lxattr_value%i\"} ",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("-increment %f -from %f -to %f\n",
                 attr->step, (attr->clipped % 2) ? attr->minimum : FLT_MIN, (attr->clipped > 1) ? attr->maximum : FLT_MAX);
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getfloatarg(0, argc, argv));
    }
    else if(attr->style == s_cream_menu)
    {
        sys_vgui("set var%lxattr_value%i \"%s\"\n", ref, i+1, atom_getsymbolarg(0, argc, argv));
        
        sys_vgui("spinbox .epw%lx.attr_values%i.label -font {Helvetica 12} -width 18 -state readonly ", ref, i+1);
        sys_vgui("-textvariable [string trim var%lxattr_value%i] -command {pdsend \"%s %s $var%lxattr_value%i\"}",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("-values {");
        for(j = 0; j < attr->itemssize; j++)
        {
            sys_vgui("%s ", attr->itemslist[attr->itemssize - 1 - j]->s_name);
        }
        sys_vgui("}\n");
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getsymbolarg(0, argc, argv));
    }
    else
    {
        if(argc && argv)
        {
            sys_vgui("set var%lxattr_value%i [concat ", ref, i+1);
            for(j = 0; j < argc; j++)
            {
                if(atom_gettype(argv+j) == A_FLOAT)
                {
                    sys_vgui("%g ", atom_getfloat(argv+j));
                }
                else if(atom_gettype(argv+j) == A_SYMBOL)
                {
                    atom_string(argv+j, text, MAXPDSTRING);
                    sys_vgui("'%s' ", text);
                }
            }
            sys_gui("]\n");
        }
        else
        {
            sys_vgui("set var%lxattr_value%i \"\"\n", ref, i+1);
        }
        
        sys_vgui("entry .epw%lx.attr_values%i.label -font {Helvetica 12} -width 20 ", ref, i+1);
        sys_vgui("-textvariable var%lxattr_value%i\n", ref, i+1);
        
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Return> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Tab> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <FocusOut> {pdsend \"%s %s $var%lxattr_value%i\"}\n",
                 ref, i+1, eobj_getid(x->c_owner)->s_name, attr->name->s_name, ref, i+1);
        sys_vgui("bind .epw%lx.attr_values%i.label <KeyPress-Escape> {set var%lxattr_value%i %f}\n",
                 ref, i+1, ref, i+1, atom_getsymbolarg(0, argc, argv));
    }
    if(argc && argv)
    {
        free(argv);
    }
     */
    argv = NULL;
    argc = 0;
    
    
    sys_vgui("pack .epw%lx.attr_label%i.label    -side left -fill both -expand 1\n",    ref, i+1);
    sys_vgui("pack .epw%lx.attr_name%i.label     -side left -fill both -expand 1\n",    ref, i+1);
    sys_vgui("pack .epw%lx.attr_values%i.label   -side left -fill both -expand 1\n",    ref, i+1);
    
    sys_vgui("grid config .epw%lx.attr_label%i   -column 0 -columnspan 2 -row %i -sticky w\n", ref, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.attr_name%i    -column 2 -row %i -sticky w\n",               ref, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.attr_values%i  -column 3 -columnspan 2 -row %i -sticky w\n", ref, i+1, x->c_nitems++);
}

static void ewindowprop_addparam(t_ewindowprop* x, t_eparam const* param, int i)
{
    t_ebox const* z        = (t_ebox const*)x->c_owner;
    const unsigned long tx = (unsigned long)x->c_owner;
    
    sys_vgui("frame .epw%lx.params_menu_index%i\n",   tx, i+1);
    sys_vgui("frame .epw%lx.params_menu_name%i\n",    tx, i+1);
    sys_vgui("frame .epw%lx.params_menu_label%i\n",   tx, i+1);
    /*
    sys_vgui("set var%lxparam_menu_index%i %i\n", tx, i+1, z->b_params[i]->p_index);
    sys_vgui("entry .epw%lx.params_menu_index%i.entry -font {Helvetica 12} -width 5 \
             -textvariable var%lxparam_menu_index%i\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_INDEX) ? "disable" : "normal");
    sys_vgui("set var%lxparam_menu_name%i \"%s\"\n", tx, i+1, param->p_name->s_name);
    sys_vgui("entry .epw%lx.params_menu_name%i.entry -font {Helvetica 12} -width 13 \
             -textvariable var%lxparam_menu_name%i -state %s\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_NAME) ? "disable" : "normal");
    sys_vgui("set var%lxparam_menu_label%i \"%s\"\n", tx, i+1, z->b_params[i]->p_label->s_name);
    sys_vgui("entry .epw%lx.params_menu_label%i.entry -font {Helvetica 12} -width 15 \
             -textvariable var%lxparam_menu_label%i -state %s\n", tx, i+1, tx, i+1,
             (z->b_params[i]->p_flags & EPARAM_STATIC_LABEL) ? "disable" : "normal");
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Return> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Return> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Return> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Tab> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Tab> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Tab> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <FocusOut> {pdsend \"%s param %i index $var%lxparam_menu_index%i\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <FocusOut> {pdsend \"%s param %i name '$var%lxparam_menu_name%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <FocusOut> {pdsend \"%s param %i label '$var%lxparam_menu_label%i'\"}\n",
             tx, i+1, z->b_obj.o_id->s_name, i+1, tx, i+1);
    
    
    sys_vgui("bind .epw%lx.params_menu_index%i.entry <KeyPress-Escape> {set var%lxparam_menu_index%i %i}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_index);
    sys_vgui("bind .epw%lx.params_menu_name%i.entry <KeyPress-Escape> {set var%lxparam_menu_name%i %s}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_name->s_name);
    sys_vgui("bind .epw%lx.params_menu_label%i.entry <KeyPress-Escape> {set var%lxparam_menu_label%i %s}\n",
             tx, i+1, tx, i+1, z->b_params[i]->p_label->s_name);
    
    sys_vgui("pack  .epw%lx.params_menu_index%i.entry -side left -fill both -expand 1\n",  tx);
    sys_vgui("pack  .epw%lx.params_menu_name%i.entry -side left -fill both -expand 1\n",  tx);
    sys_vgui("pack  .epw%lx.params_menu_label%i.entry -side left -fill both -expand 1\n",  tx);
    
    sys_vgui("grid config .epw%lx.params_menu_index%i -column 0 -row %i -sticky w\n",   tx, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.params_menu_name%i -column 1 -row %i -sticky w\n",    tx, i+1, x->c_nitems);
    sys_vgui("grid config .epw%lx.params_menu_label%i -column 2 -row %i -sticky w\n",   tx, i+1, x->c_nitems++);
     */
}

t_ewindowprop* ewindowprop_create(t_eobj* x)
{
    int i;
    char text[MAXPDSTRING];
    t_ewindowprop* wm = ewindowprop_getfromobject(x);
    const unsigned long ref = (unsigned long)x;
    t_eclass const* c = eobj_getclass(x);
    t_ebox const* z   = (t_ebox *)x;
    /*
    if(!wm)
    {
        t_class* c2 = ewindowprop_setup();
        if(c2)
        {
            wm = (t_ewindowprop *)pd_new(c2);
            if(wm)
            {
                sprintf(text, "windowprop%lx", ref);
                wm->c_owner = x;
                pd_bind((t_pd *)wm, gensym(text));
            }
        }
    }
    else
    {
        sys_vgui("destroy .epw%lx\n", ref);
    }
    if(wm)
    {
        wm->c_nitems = 0;
        // Window
        sys_vgui("toplevel .epw%lx\n", ref);
        sys_vgui("wm title .epw%lx {%s properties}\n", ref, c->c_class.c_name->s_name);
        sys_vgui("wm resizable .epw%lx 0 0\n", ref);
        
        // Attrs
        sys_vgui("frame .epw%lx.attrs\n", ref);
        sys_vgui("label .epw%lx.attrs.label -justify center -font {Helvetica 13 bold} -text Attributes -height 2\n", ref);
        sys_vgui("pack  .epw%lx.attrs.label -side left -fill both -expand 1\n", ref);
        sys_vgui("grid config .epw%lx.attrs -columnspan 5 -row %i -sticky w\n", ref, wm->c_nitems++);
        
        sys_vgui("frame .epw%lx.attrs_menu_label\n", ref);
        sys_vgui("frame .epw%lx.attrs_menu_name\n",  ref);
        sys_vgui("frame .epw%lx.attrs_menu_values\n",ref);
        
        sys_vgui("label .epw%lx.attrs_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",    ref);
        sys_vgui("label .epw%lx.attrs_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",      ref);
        sys_vgui("label .epw%lx.attrs_menu_values.label -justify left -font {Helvetica 12 bold} -text Values\n",  ref);
        
        sys_vgui("pack  .epw%lx.attrs_menu_label.label -side left -fill both -expand 1\n",   ref);
        sys_vgui("pack  .epw%lx.attrs_menu_name.label -side left -fill both -expand 1\n",    ref);
        sys_vgui("pack  .epw%lx.attrs_menu_values.label -side left -fill both -expand 1\n",  ref);
        
        sys_vgui("grid config .epw%lx.attrs_menu_label -column 0 -columnspan 2 -row %i -sticky w\n",   ref, wm->c_nitems);
        sys_vgui("grid config .epw%lx.attrs_menu_name -column 2 -row %i -sticky w\n",                  ref, wm->c_nitems);
        sys_vgui("grid config .epw%lx.attrs_menu_values -column 3 -columnspan 2 -row %i -sticky w\n",  ref, wm->c_nitems++);
        
        for(i = 0; i < c->c_nattr; i++)
        {
            if(!c->c_attr[i]->invisible)
            {
                ewindowprop_addattr(wm, c->c_attr[i], i);
            }
        }
        
        // Params
        if(eobj_isbox(x) && z->b_nparams)
        {
            sys_vgui("frame .epw%lx.params\n", ref);
            sys_vgui("label .epw%lx.params.label -justify center -font {Helvetica 13  bold} -text Parameters -height 2\n", ref);
            sys_vgui("pack  .epw%lx.params.label -side left -fill both -expand 1\n",  ref);
            sys_vgui("grid config .epw%lx.params -columnspan 3 -row %i -sticky w\n", ref, wm->c_nitems++);
            
            sys_vgui("frame .epw%lx.params_menu_index\n",ref);
            sys_vgui("frame .epw%lx.params_menu_name\n", ref);
            sys_vgui("frame .epw%lx.params_menu_label\n",ref);
            
            sys_vgui("label .epw%lx.params_menu_index.label -justify left -font {Helvetica 12 bold} -text Index\n",   ref);
            sys_vgui("label .epw%lx.params_menu_name.label -justify left -font {Helvetica 12 bold} -text Name\n",     ref);
            sys_vgui("label .epw%lx.params_menu_label.label -justify left -font {Helvetica 12 bold} -text Label\n",   ref);
            
            sys_vgui("pack  .epw%lx.params_menu_index.label -side left -fill both -expand 1\n",  ref);
            sys_vgui("pack  .epw%lx.params_menu_name.label -side left -fill both -expand 1\n",   ref);
            sys_vgui("pack  .epw%lx.params_menu_label.label -side left -fill both -expand 1\n",  ref);
            
            sys_vgui("grid config .epw%lx.params_menu_index -column 0 -row %i -sticky w\n",    ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.params_menu_name -column 1 -row %i -sticky w\n",     ref, wm->c_nitems);
            sys_vgui("grid config .epw%lx.params_menu_label -column 2 -row %i -sticky w\n",    ref, wm->c_nitems++);
            
            for(i = 0; i < z->b_nparams; i++)
            {
                if(z->b_params[i])
                {
                    ewindowprop_addparam(wm, z->b_params[i], i);
                }
            }
        }
    }
     */
    return wm;
}



static float ecicmwrapper_getversion(t_object* x)
{
    return CICM_VERSION;
}

static int cicmwrapper_setup()
{
    float version;
    t_flt_method getversion = NULL;
    t_class* ecicmwrapper_class = NULL;
    t_pd* cicm = gensym("cicmwrapper_1572")->s_thing;
    if(!cicm)
    {
        ecicmwrapper_class = class_new(gensym("cicmwrapper"), NULL, (t_method)NULL, sizeof(t_object), CLASS_PD, A_GIMME, 0);
        class_addmethod(ecicmwrapper_class, (t_method)ecicmwrapper_getversion, gensym("version"), A_NULL, 0);
        
        cicm = pd_new(ecicmwrapper_class);
        if(cicm)
        {
            pd_bind(cicm, gensym("cicmwrapper_1572"));
        }
    }
    else
    {
        getversion = (t_flt_method)getfn((t_pd *)cicm, gensym("version"));
        if(getversion)
        {
            version = getversion(cicm);
            if(version != CICM_VERSION)
            {
                pd_error(cicm, "Two version of the CICM wrapper are used : %g and %g.\nIt could generates problems.", version, CICM_VERSION);
                ecicmwrapper_class = class_new(gensym("cicmwrapper"), NULL, (t_method)NULL, sizeof(t_object), CLASS_PD, A_GIMME, 0);
                class_addmethod(ecicmwrapper_class, (t_method)ecicmwrapper_getversion, gensym("version"), A_NULL, 0);
                
                cicm = pd_new(ecicmwrapper_class);
                if(cicm)
                {
                    pd_bind(cicm, gensym("cicmwrapper_1572"));
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            pd_error(cicm, "Two version of the CICM wrapper are used : 0.4 and %g.\nIt could generates problems.", CICM_VERSION);
            ecicmwrapper_class = class_new(gensym("cicmwrapper"), NULL, (t_method)NULL, sizeof(t_object), CLASS_PD, A_GIMME, 0);
            class_addmethod(ecicmwrapper_class, (t_method)ecicmwrapper_getversion, gensym("version"), A_NULL, 0);
            
            cicm = pd_new(ecicmwrapper_class);
            if(cicm)
            {
                pd_bind(cicm, gensym("cicmwrapper_1572"));
            }
        }
    }
    return 1;
}

void cicmwrapper_init(void)
{
    if(cicmwrapper_setup())
    {
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
        sys_gui("pdsend \"$name dowrite [enquote_path $dirname/$basename]\"\n");
        sys_gui("set ::filenewdir $dirname\n");
        sys_gui("::pd_guiprefs::update_recentfiles $filename\n");
        sys_gui("}\n");
        
        // OBJECT OPEN FILE //
        sys_gui("proc eobj_openfrom {name} {\n");
        sys_gui("if { ! [file isdirectory $::filenewdir]} {\n");
        sys_gui("set ::filenewdir [file normalize $::env(HOME)]\n");
        sys_gui("}\n");
        sys_gui("set files [tk_getOpenFile -multiple true -initialdir $::fileopendir]\n");
        sys_gui("pdsend \"$name doread [enquote_path $files]\"\n");
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
    }
}



