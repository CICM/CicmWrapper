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

t_etexteditor* etexteditor_create(t_eobj* x, t_symbol* name)
{
    char buffer[MAXPDSTRING];
    t_etexteditor* editor = (t_etexteditor *)malloc(sizeof(t_etexteditor));
    if(editor)
    {
        t_canvas* canvas = eobj_getcanvas(x);
        sprintf(buffer,".x%lx.c", (long unsigned int)canvas);
        editor->c_canvas_id = gensym(buffer);
        sprintf(buffer,"%s.frame%lx", editor->c_canvas_id->s_name, (long unsigned int)editor);
        editor->c_frame_id = gensym(buffer);
        sprintf(buffer, "%s.text%lx", editor->c_frame_id->s_name, (long unsigned int)x);
        editor->c_name = gensym(buffer);
        sprintf(buffer, "%s.window%lx", editor->c_canvas_id->s_name, (long unsigned int)x);
        editor->c_window_id = gensym(buffer);
        
        editor->c_send = x->o_id;
        editor->c_owner = x;
        
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

void etexteditor_destroy(t_etexteditor* editor)
{
    if(editor)
    {
        sys_vgui("destroy %s\n", editor->c_frame_id->s_name);
        sys_vgui("destroy %s\n", editor->c_name->s_name);
        sys_vgui("%s delete %s\n", editor->c_canvas_id->s_name, editor->c_window_id->s_name);
        free(editor);
    }
}

void etexteditor_settext(t_etexteditor* editor, const char* text)
{
    sys_vgui("%s insert 0.0 %s\n", editor->c_name->s_name, text);
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
    sys_vgui("bind %s <KeyPress> {+pdsend {%s texteditor_keypress %ld %%k}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    
    sys_vgui("bind %s <Escape> {+pdsend {%s texteditor_keyfilter 0}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Tab> {+pdsend {%s texteditor_keyfilter 1}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Return> {+pdsend {%s texteditor_keyfilter 2}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    sys_vgui("bind %s <Delete> {+pdsend {%s texteditor_keyfilter 3}}\n",
             editor->c_name->s_name, editor->c_owner->o_id->s_name, (int unsigned long)editor);
    
    sys_vgui("%s create window %d %d -anchor nw -window %s    \
             -tags %s -width %d -height %d \n",
             editor->c_canvas_id->s_name,
             (int)bounds->x, (int)bounds->y,
             editor->c_frame_id->s_name, editor->c_window_id->s_name,
             (int)bounds->width, (int)bounds->height);
    
    sys_vgui("focus -force %s\n", editor->c_name->s_name);
}








