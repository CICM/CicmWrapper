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

t_epopup* epopupmenu_create(t_eobj* x, t_symbol* name)
{
    t_epopup* popup = (t_epopup *)malloc(sizeof(t_epopup));
    if(popup)
    {
        popup->c_send = x->o_id;
        popup->c_name = name;
        sys_vgui("destroy .eboxpopup%s\n", popup->c_name->s_name);
        sys_vgui("menu .eboxpopup%s -tearoff 0\n", popup->c_name->s_name);
    }    
    return popup;
}

void epopupmenu_destroy(t_epopup* popup)
{
    if(popup)
    {
        sys_vgui("destroy .eboxpopup%s\n", popup->c_name->s_name);
        free(popup);
    }
}

void epopupmenu_setfont(t_epopup* popup, t_efont *font)
{
    sys_vgui(".eboxpopup%s configure -font {%s %d %s italic}\n", popup->c_name->s_name, font[0].c_family->s_name, (int)font[0].c_size, font[0].c_weight->s_name, font[0].c_slant->s_name);
}

void epopupmenu_additem(t_epopup* popup, int itemid, const char *text, char checked, char disabled)
{
    sys_vgui(".eboxpopup%s add command ", popup->c_name->s_name);
    sys_vgui("-command {pdsend {%s popup %s %f}} ", popup->c_send->s_name, popup->c_name->s_name, (float)itemid);
    sys_vgui("-label {%s} ", text);
    if(disabled)
        sys_vgui("-state disable\n");
    else
        sys_vgui("-state active\n");
}

void epopupmenu_addseperator(t_epopup* popup)
{
    sys_vgui(".eboxpopup%s add separator\n", popup->c_name->s_name);
}

void epopupmenu_popup(t_epopup* popup, t_pt pos)
{
    sys_vgui(".eboxpopup%s post %i %i\n", popup->c_name->s_name, (int)pos.x, (int)pos.y);
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
    if(argc && atom_gettype(argv+argc-1) == A_FLOAT)
    {
        sprintf(text, "%c", (char)atom_getfloat(argv+argc-1));
        strncat(x->c_text, text, MAXPDSTRING);
    }
    post(x->c_text);
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

t_etexteditor* etexteditor_create(t_eobj* x, t_symbol* name)
{
    char buffer[MAXPDSTRING];
    t_class* c = etexteditor_setup();
    if(c)
    {
        t_etexteditor* editor = (t_etexteditor *)pd_new(c);
        if(editor)
        {
            t_canvas* canvas = eobj_getcanvas(x);
            sprintf(buffer,".x%lx.c", (long unsigned int)canvas);
            editor->c_canvas_id = gensym(buffer);
            sprintf(buffer,"%s.frame%lx", editor->c_canvas_id->s_name, (long unsigned int)editor);
            editor->c_frame_id = gensym(buffer);
            sprintf(buffer, "%s.text%lx", editor->c_frame_id->s_name, (long unsigned int)editor);
            editor->c_name = gensym(buffer);
            sprintf(buffer, "%s.window%lx", editor->c_canvas_id->s_name, (long unsigned int)editor);
            editor->c_window_id = gensym(buffer);
            
            editor->c_send = x->o_id;
            editor->c_owner = x;
            editor->c_text = NULL;
            editor->c_size = 0;
            sprintf(buffer, "texteditor%lx", (long unsigned int)editor);
            pd_bind((t_pd *)editor, gensym(buffer));
            
            sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
            sys_vgui("destroy %s\n", editor->c_name->s_name);
            sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
            
            sys_vgui("frame %s -borderwidth 0.0 -highlightthickness 0 \n", editor->c_frame_id->s_name);
            sys_vgui("text %s -borderwidth 0.0 -highlightthickness 0 -insertborderwidth 0\n", editor->c_name->s_name);
            sys_vgui("pack %s -side left -fill both -expand 1 \n", editor->c_name->s_name);
            sys_vgui("pack %s -side bottom -fill both -expand 1 \n", editor->c_frame_id->s_name);
        }
        return editor;
    }
    
    return NULL;
}

void etexteditor_destroy(t_etexteditor* editor)
{
    char buffer[MAXPDSTRING];
    if(editor)
    {
        sprintf(buffer, "texteditor%lx", (long unsigned int)editor);
        pd_unbind((t_pd *)editor, gensym(buffer));
        sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
        sys_vgui("destroy %s\n", editor->c_name->s_name);
        sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
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
}

void etexteditor_clear(t_etexteditor* editor)
{
    sys_vgui("%s delete 0 end\n", editor->c_name->s_name);
}

void etexteditor_setfont(t_etexteditor *editor, t_efont const* font)
{
    sys_vgui("%s configure -font {%s %d %s %s}\n", editor->c_name->s_name,
             font->c_family->s_name, (int)font->c_size, font->c_weight->s_name, font->c_slant->s_name);
}

void etexteditor_setbackgroundcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -background %s\n", editor->c_name->s_name, rgba_to_hex(color));
}

void etexteditor_settextcolor(t_etexteditor *editor, t_rgba const* color)
{
    sys_vgui("%s configure -foreground %s\n", editor->c_name->s_name, rgba_to_hex(color));
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
}

void etexteditor_popup(t_etexteditor *editor, t_rect const* bounds)
{
    char buffer[MAXPDSTRING];
    sprintf(buffer, "texteditor%lx", (long unsigned int)editor);
    
    sys_vgui("bind %s <KeyPress> {etext_sendtext %s %s %s %ld %%k}\n",
             editor->c_name->s_name, editor->c_name->s_name, buffer, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    
    sys_vgui("bind %s <Escape> {+pdsend {%s texteditor_keyfilter %ld 0}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Tab> {+pdsend {%s texteditor_keyfilter %ld 1}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Return> {+pdsend {%s texteditor_keyfilter %ld 2}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Delete> {+pdsend {%s texteditor_keyfilter %ld 3}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    
    sys_vgui("%s create window %d %d -anchor nw -window %s    \
             -tags %s -width %d -height %d \n",
             editor->c_canvas_id->s_name,
             (int)bounds->x, (int)bounds->y,
             editor->c_frame_id->s_name, editor->c_window_id->s_name,
             (int)bounds->width, (int)bounds->height);
}

void etexteditor_grabfocus(t_etexteditor *editor)
{
    sys_vgui("focus -force %s\n", editor->c_name->s_name);
}







