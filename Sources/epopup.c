/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "epopup.h"
#include "eobj.h"
#include "ebox.h"
#include "egraphics.h"

static void epopup_notify(t_epopup* popup, ewidget_action action)
{
    t_atom av[2];
    if(popup->c_owner->o_camo_id->s_thing)
    {
        atom_setsym(av, popup->c_popup_id);
        atom_setfloat(av+1, (float)action);
        pd_typedmess(popup->c_owner->o_camo_id->s_thing, s_cream_popup, 2, av);
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
            
            epopup_notify(popup, EWIDGET_CREATE);
        }
    }
    return popup;
}

void epopupmenu_destroy(t_epopup* popup)
{
    if(popup)
    {
        epopup_notify(popup, EWIDGET_DESTROY);
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
    epopup_notify(popup, EWIDGET_CHANGED);
}

void epopupmenu_setbackgroundcolor(t_epopup *popup, t_rgba const* color)
{
    memcpy(&popup->c_bgcolor, color, sizeof(t_rgba));
    epopup_notify(popup, EWIDGET_CHANGED);
}

void epopupmenu_settextcolor(t_epopup *popup, t_rgba const* color)
{
    memcpy(&popup->c_txtcolor, color, sizeof(t_rgba));
    epopup_notify(popup, EWIDGET_CHANGED);
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
    
    epopup_notify(popup, EWIDGET_CHANGED);
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
    epopup_notify(popup, EWIDGET_CHANGED);
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
    epopup_notify(popup, EWIDGET_POPUP);
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

static void etexteditor_notify(t_etexteditor* editor, ewidget_action action)
{
    t_atom av[2];
    if(editor->c_owner->b_obj.o_camo_id->s_thing)
    {
        atom_setsym(av, editor->c_editor_id);
        atom_setfloat(av+1, (float)action);
        pd_typedmess(editor->c_owner->b_obj.o_camo_id->s_thing, s_cream_texteditor, 2, av);
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
            
            etexteditor_notify(editor, EWIDGET_CREATE);
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
        etexteditor_notify(editor, EWIDGET_DESTROY);
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
    etexteditor_notify(editor, EWIDGET_CHANGED);
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
    etexteditor_notify(editor, EWIDGET_CHANGED);
}

void etexteditor_clear(t_etexteditor* editor)
{
    sys_vgui("%s delete 0.0 end\n", editor->c_name->s_name);
    etexteditor_notify(editor, EWIDGET_CHANGED);
}

void etexteditor_setfont(t_etexteditor *editor, t_efont const* font)
{
    sys_vgui("%s configure -font {%s %d %s %s}\n", editor->c_name->s_name,
             font->c_family->s_name, (int)font->c_size, font->c_weight->s_name, font->c_slant->s_name);
    
    memcpy(&editor->c_font, font, sizeof(t_efont));
    etexteditor_notify(editor, EWIDGET_CHANGED);
}

void etexteditor_setbackgroundcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -background %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_bgcolor, color, sizeof(t_efont));
    etexteditor_notify(editor, EWIDGET_CHANGED);
}

void etexteditor_settextcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -foreground %s\n", editor->c_name->s_name, rgba_to_hex(color));
    
    memcpy(&editor->c_txtcolor, color, sizeof(t_efont));
    etexteditor_notify(editor, EWIDGET_CHANGED);
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
    etexteditor_notify(editor, EWIDGET_CHANGED);
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
    etexteditor_notify(editor, EWIDGET_POPUP);
}

void etexteditor_grabfocus(t_etexteditor *editor)
{
    sys_vgui("focus -force %s\n", editor->c_name->s_name);
    etexteditor_notify(editor, EWIDGET_GRABFOCUS);
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


static t_eparam* ebox_get_parameter(t_ebox *x, t_symbol* name)
{
    int i;
    for(i = 0; i < x->b_nparams; i++)
    {
        if(x->b_params[i].p_name == name)
        {
            return x->b_params+i;
        }
    }
    return NULL;
}

void ebox_parameter_new(t_ebox *x, t_symbol* name)
{
    t_eparam* temp = NULL;
    if(x->b_nparams && x->b_params)
    {
        temp = (t_eparam *)realloc(x->b_params, sizeof(t_eparam) * (size_t)(x->b_nparams + 1));
        if(temp)
        {
            x->b_params = temp;
            x->b_params[x->b_nparams].p_name       = name;
            x->b_params[x->b_nparams].p_label      = s_cream_empty;
            x->b_params[x->b_nparams].p_owner      = x;
            x->b_params[x->b_nparams].p_min        = 0.f;
            x->b_params[x->b_nparams].p_max        = 1.f;
            x->b_params[x->b_nparams].p_default    = 0.f;
            x->b_params[x->b_nparams].p_getter     = (t_err_method)NULL;
            x->b_params[x->b_nparams].p_setter     = (t_err_method)NULL;
            x->b_params[x->b_nparams].p_auto       = 1;
            x->b_params[x->b_nparams].p_meta       = 0;
            x->b_nparams++;
        }
    }
    else
    {
        x->b_params = (t_eparam *)malloc(sizeof(t_eparam));
        if(x->b_params)
        {
            x->b_params[0].p_name       = name;
            x->b_params[0].p_label      = s_cream_empty;
            x->b_params[0].p_owner      = x;
            x->b_params[0].p_min        = 0.f;
            x->b_params[0].p_max        = 1.f;
            x->b_params[0].p_default    = 0.f;
            x->b_params[0].p_getter     = (t_err_method)NULL;
            x->b_params[0].p_setter     = (t_err_method)NULL;
            x->b_params[0].p_auto       = 1;
            x->b_params[0].p_meta       = 0;
            
            x->b_nparams = 1;
        }
        else
        {
            x->b_nparams = 0;
        }
    }
}

void ebox_parameter_label(t_ebox *x, t_symbol* name, t_symbol* label)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param)
    {
        param->p_label = label;
    }
}

void ebox_parameter_default(t_ebox *x, t_symbol* name, float value)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param)
    {
        param->p_default = value;
    }
}

void ebox_parameter_minmax(t_ebox *x, t_symbol* name, float min, float max)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param)
    {
        param->p_min = min;
        param->p_max = max;
    }
}

void ebox_parameter_methods(t_ebox *x, t_symbol* name, t_err_method getter, t_err_method setter)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param)
    {
        param->p_getter = getter;
        param->p_setter = setter;
    }
}

void ebox_parameter_options(t_ebox *x, t_symbol* name, char meta, char autom)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param)
    {
        param->p_meta = meta;
        param->p_auto = autom;
    }
}

void ebox_parameter_set(t_ebox *x, t_symbol* name, float f)
{
    t_eparam* param = ebox_get_parameter(x, name);
    if(param && param->p_setter)
    {
        if(param->p_max == param->p_min)
        {
            param->p_setter(param->p_owner, param->p_name, param->p_max);
        }
        else if(param->p_min < param->p_max)
        {
            param->p_setter(param->p_owner, param->p_name, f * (param->p_max - param->p_min) + param->p_min);
        }
        else if(param->p_min > param->p_max)
        {
            param->p_setter(param->p_owner, param->p_name, (1.f - f) * (param->p_min - param->p_max) + param->p_max);
        }
    }
}

float ebox_parameter_get(t_ebox *x, t_symbol* name)
{
    float f = 0.f;
    t_eparam* param = ebox_get_parameter(x, name);
    if(param && param->p_getter)
    {
        param->p_getter(param->p_owner, param->p_name, &f);
        if(param->p_max == param->p_min)
        {
            f = f / param->p_max;
        }
        else if(param->p_min < param->p_max)
        {
            f = (f + param->p_min) / (param->p_max  - param->p_min);
        }
        else if(param->p_min > param->p_max)
        {
            f = (f + param->p_max) / (param->p_min  - param->p_max);
        }
    }
    return f;
}

float ebox_parameter_getdefault(t_ebox *x, t_symbol* name)
{
    float f = 0.f;
    t_eparam* param = ebox_get_parameter(x, name);
    if(param && param->p_getter)
    {
        if(param->p_max == param->p_min)
        {
            f = f / param->p_max;
        }
        else if(param->p_min < param->p_max)
        {
            f = (f + param->p_min) / (param->p_max  - param->p_min);
        }
        else if(param->p_min > param->p_max)
        {
            f = (f + param->p_max) / (param->p_min  - param->p_max);
        }
    }
    return f;
}

void ebox_parameter_gettextforvalue(t_ebox *x, t_symbol* name, char** value, float f)
{
    int todo;
}

float ebox_parameter_getvaluefortext(t_ebox *x, t_symbol* name, char const* value)
{
    int todo;
    return 0.f;
}





