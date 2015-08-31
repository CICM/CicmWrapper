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
    sys_vgui("%s configure -font {%s %d %s italic}\n", popup->c_name->s_name, font[0].c_family->s_name, (int)font[0].c_size, font[0].c_weight->s_name, font[0].c_slant->s_name);
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
    for(i = 0; i < argc - 1; i++)
    {
        atom_string(argv+i, text, MAXPDSTRING);
        lenght += strlen(text) + 1;
        
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
    for(i = 0; i < argc - 2; i++)
    {
        atom_string(argv+i, text, MAXPDSTRING);
        strncat(x->c_text, text, MAXPDSTRING);
        strncat(x->c_text, " ", 1);
    }
    if(argc > 1)
    {
        atom_string(argv+argc-2, text, MAXPDSTRING);
        strncat(x->c_text, text, MAXPDSTRING);
    }
    if(argc && atom_gettype(argv+argc-1) == A_FLOAT && isalnum((char)atom_getfloat(argv+argc-1)))
    {
        sprintf(text, "%c", (char)atom_getfloat(argv+argc-1));
        strncat(x->c_text, text, MAXPDSTRING);
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
            
            editor->c_font.c_size   = 12;
            editor->c_font.c_family = gensym("Helvetica");
            editor->c_font.c_slant  = gensym("roman");
            editor->c_font.c_weight = gensym("normal");
            
            editor->c_bgcolor   = rgba_white;
            editor->c_txtcolor  = rgba_black;
            editor->c_wrap      = 0;
            
            pd_bind((t_pd *)editor, editor->c_editor_id);
            
            sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
            sys_vgui("destroy %s\n", editor->c_name->s_name);
            sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
            
            sys_vgui("frame %s -borderwidth 0.0 -highlightthickness 0 \n", editor->c_frame_id->s_name);
            sys_vgui("text %s -borderwidth 0.0 -highlightthickness 0 -insertborderwidth 0\n", editor->c_name->s_name);
            sys_vgui("pack %s -side left -fill both -expand 1 \n", editor->c_name->s_name);
            sys_vgui("pack %s -side bottom -fill both -expand 1 \n", editor->c_frame_id->s_name);
            
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
    sys_vgui("%s configure -font {%s %d %s %s}\n", editor->c_name->s_name,
             font->c_family->s_name, (int)font->c_size, font->c_weight->s_name, font->c_slant->s_name);
    
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
    sys_vgui("bind %s <KeyPress> {etext_sendtext %s %s %s %%k}\n",
             editor->c_name->s_name, editor->c_name->s_name,
             editor->c_editor_id->s_name, editor->c_owner->b_obj.o_id->s_name);
    
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


static t_class* eparameter_setup()
{
    t_class* eparameter_class = NULL;
    t_symbol* eparameter1572_sym = gensym("eparameter1572");
    if(!eparameter1572_sym->s_thing)
    {
        eparameter_class = class_new(gensym("eparameter"), NULL, (t_method)NULL, sizeof(t_etexteditor), CLASS_PD, A_GIMME, 0);
        eparameter1572_sym->s_thing = (t_class **)eparameter_class;
        return eparameter_class;
    }
    else
    {
        return (t_class *)eparameter1572_sym->s_thing;
    }
}

t_eparam* ebox_parameter_create(t_ebox *x, int index)
{
    int i;
    char text[MAXPDSTRING];
    t_eparam **temp  = NULL, *param = NULL;
    t_class* c = eparameter_setup();
    if(c)
    {
        if(index < x->b_nparams && x->b_params[index])
        {
            return x->b_params[index];
        }
        else
        {
            param = (t_eparam *)pd_new(c);
            if(param)
            {
                sprintf(text, "param%ld", (unsigned long)(param));
                param->p_bind       = gensym(text);
                param->p_name       = s_cream_empty;
                param->p_label      = s_cream_empty;
                param->p_owner      = x;
                param->p_index      = index;
                param->p_value      = 0.f;
                param->p_min        = 0.f;
                param->p_max        = 1.f;
                param->p_step       = FLT_EPSILON;
                param->p_getter     = (t_param_getter)NULL;
                param->p_setter     = (t_param_setter)NULL;
                param->p_getter_t   = (t_param_getter_t)NULL;
                param->p_setter_t   = (t_param_setter_t)NULL;
                param->p_auto       = 1;
                param->p_meta       = 0;
                pd_bind((t_pd *)param, param->p_bind);
                if(index < x->b_nparams)
                {
                    x->b_params[index] = param;
                    eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_create);
                    return param;
                }
                else if(x->b_nparams && x->b_params)
                {
                    temp = (t_eparam **)realloc(x->b_params, sizeof(t_eparam *) * (size_t)(index + 1));
                    if(temp)
                    {
                        x->b_params = temp;
                        for(i = x->b_nparams; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_create);
                        return param;
                    }
                    else
                    {
                        return NULL;
                    }
                }
                else
                {
                    x->b_params = (t_eparam **)malloc(sizeof(t_eparam *) * (size_t)(index + 1));
                    if(x->b_params)
                    {
                        for(i = 0; i < index; i++)
                        {
                            x->b_params[i] = NULL;
                        }
                        x->b_params[index] = param;
                        x->b_nparams = index+1;
                        eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_create);
                        return param;
                    }
                    else
                    {
                        x->b_nparams = 0;
                        return NULL;
                    }
                }
            }
            
        }
    }
    return NULL;
}

void ebox_parameter_destroy(t_ebox* x, int index)
{
    int i = 0;
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_destroy);
            pd_unbind((t_pd *)x->b_params[index], x->b_params[index]->p_bind);
            free(x->b_params[index]);
            x->b_params[index] = NULL;
            for(i = index; i < x->b_nparams - 1; i++)
            {
                x->b_params[i] = x->b_params[i+1];
            }
        }
    }
}

float ebox_parameter_getvalue(t_ebox* x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_getter)
            {
                return x->b_params[index]->p_getter(x->b_params[index]->p_owner, x->b_params[index]);
            }
            return x->b_params[index]->p_value;
        }
    }
    return 0;
}

void ebox_parameter_setvalue(t_ebox* x, int index, float value, char notify)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_setter)
            {
                x->b_params[index]->p_setter(x->b_params[index]->p_owner, x->b_params[index], value);
            }
            else
            {
                if(x->b_params[index]->p_min < x->b_params[index]->p_max)
                {
                    x->b_params[index]->p_value = pd_clip_minmax(value, x->b_params[index]->p_min, x->b_params[index]->p_max);
                }
                else
                {
                    x->b_params[index]->p_value  = pd_clip_minmax(value, x->b_params[index]->p_max, x->b_params[index]->p_min);
                }
            }
            if(notify)
            {
                ebox_parameter_notify_changes(x, index);
            }
        }
    }
}

float ebox_parameter_getvalue_normalized(t_ebox* x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_max == x->b_params[index]->p_min)
            {
                return 1.;
            }
            else if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                return (ebox_parameter_getvalue(x, index) - x->b_params[index]->p_min) / (x->b_params[index]->p_max - x->b_params[index]->p_min);
            }
            else
            {
                return (ebox_parameter_getvalue(x, index) - x->b_params[index]->p_max) / (x->b_params[index]->p_min - x->b_params[index]->p_max);
            }
        }
    }
    return 0.f;
}

void ebox_parameter_setvalue_normalized(t_ebox* x, int index, float value, char notify)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            if(x->b_params[index]->p_min < x->b_params[index]->p_max)
            {
                ebox_parameter_setvalue(x, index, (value * (x->b_params[index]->p_max - x->b_params[index]->p_min) + x->b_params[index]->p_min), notify);
            }
            else
            {
                ebox_parameter_setvalue(x, index, ((1.f - value) * (x->b_params[index]->p_min - x->b_params[index]->p_max) + x->b_params[index]->p_max), notify);
            }
        }
    }
}

float ebox_parameter_getmin(t_ebox* x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_min;
        }
    }
    return 1.f;
}

float ebox_parameter_getmax(t_ebox* x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return x->b_params[index]->p_max;
        }
    }
    return 1.f;
}

char ebox_parameter_isinverted(t_ebox* x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            return (char)(x->b_params[index]->p_max < x->b_params[index]->p_min);
        }
    }
    return 0;
}

void ebox_parameter_notify_changes(t_ebox *x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            eobj_widget_notify((t_eobj *)x->b_params[index]->p_owner, s_cream_parameter, x->b_params[index]->p_bind, s_cream_changes);
        }
    }
}

void ebox_parameter_begin_changes(t_ebox *x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_beginchanges);
        }
    }
}

void ebox_parameter_end_changes(t_ebox *x, int index)
{
    if(index >= 0 && index < x->b_nparams)
    {
        if(x->b_params[index])
        {
            eobj_widget_notify((t_eobj *)x, s_cream_parameter, x->b_params[index]->p_bind, s_cream_endchanges);
        }
    }
}

float eparameter_getvalue(t_eparam* param)
{
    if(param->p_getter)
    {
        return param->p_getter(param->p_owner, param);
    }
    return param->p_value;
}

float eparameter_getvalue_normalized(t_eparam* param)
{
    if(param->p_max == param->p_min)
    {
        return 1.;
    }
    else if(param->p_min < param->p_max)
    {
        return (eparameter_getvalue(param) - param->p_min) / (param->p_max - param->p_min);
    }
    else
    {
        return (eparameter_getvalue(param) - param->p_max) / (param->p_min - param->p_max);
    }
}

void eparameter_getvalue_text(t_eparam* param, char* text)
{
    if(param->p_getter_t)
    {
        param->p_getter_t(param->p_owner, param, text);
    }
    else
    {
        sprintf(text, "%.4f", eparameter_getvalue(param));
    }
}

void eparameter_setvalue(t_eparam* param, float value)
{
    t_eclass* c = eobj_getclass(param->p_owner);
    if(param->p_setter)
    {
        param->p_setter(param->p_owner, param, value);
    }
    else
    {
        if(param->p_min < param->p_max)
            param->p_value = pd_clip_minmax(value, param->p_min, param->p_max);
        else
            param->p_value  = pd_clip_minmax(value, param->p_max, param->p_min);
        
        if(c->c_widget.w_notify)
        {
            c->c_widget.w_notify(param->p_owner, param->p_bind, s_cream_param_changed, param, param->p_value);
        }
    }
}

void eparameter_setvalue_normalized(t_eparam* param, float value)
{
    if(param->p_min < param->p_max)
    {
        eparameter_setvalue(param, (value * (param->p_max - param->p_min) + param->p_min));
    }
    else
    {
        eparameter_setvalue(param, ((1.f - value) * (param->p_min - param->p_max) + param->p_max));
    }
}

void eparameter_setvalue_text(t_eparam* param, char const* text)
{
    if(param->p_setter_t)
    {
        param->p_setter_t(param->p_owner, param, text);
    }
    else if(isdigit(text))
    {
        eparameter_setvalue(param, atof(text));
    }
}

t_eparam* eparameter_getfromsymbol(t_symbol* name)
{
    t_class* c = eparameter_setup();
    if(c)
    {
        return (t_eparam *)pd_findbyclass(name, c);
    }
    return NULL;
}





