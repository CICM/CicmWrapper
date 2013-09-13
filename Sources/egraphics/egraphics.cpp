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

#include "egraphics.h"

void cicm_graphics_apply_transforms(t_egraphics *g, long index)
{
    double x, x_p, y, y_p;
    double radius, angle;
    
    for(int  i = 0; i < g->c_obj_coords[index].size(); i += 2)
    {
        x_p = g->c_obj_coords[index][i];
        y_p = g->c_obj_coords[index][i+1];

        if(strcmp(g->c_obj_types[index].c_str(), "oval ") != 0 && strcmp(g->c_obj_types[index].c_str(), "arc ") != 0)
        {
            if(g->c_rotation)
            {
                radius = pd_radius(x_p, y_p);
                angle  = pd_radius(x_p, y_p) + g->c_rotation;
                x_p = pd_ordinate(radius, angle);
                y_p = pd_ordinate(radius, angle);
            }
        }
        if(g->c_matrix)
        {
            x = x_p * g->c_matrix->xx + y_p * g->c_matrix->xy + g->c_matrix->x0;
            y = x_p * g->c_matrix->yx + y_p * g->c_matrix->yy + g->c_matrix->y0;
            x_p = x;
            y_p = y;
        }
        
        g->c_obj_coords[index][i]   = x_p;
        g->c_obj_coords[index][i+1] = y_p;
    }
    
}

void cicm_graphics_clip(t_egraphics *g, long index, int stroked)
{
    if(strcmp(g->c_obj_types[index].c_str(), "oval ") == 0)
    {
        double x1, y1, x2, y2, center_x, center_y, radius;
        x1 = g->c_obj_coords[index][0];
        y1 = g->c_obj_coords[index][1];
        x2 = g->c_obj_coords[index][2];
        y2 = g->c_obj_coords[index][3];
        
        if(x1 > x2)
        {
            g->c_obj_coords[index][0] = x2;
            g->c_obj_coords[index][2] = x1;
            x1 = g->c_obj_coords[index][0];
            x2 = g->c_obj_coords[index][2];
        }
        if(y1 > y2)
        {
            g->c_obj_coords[index][1] = y2;
            g->c_obj_coords[index][3] = y1;
            y1 = g->c_obj_coords[index][1];
            y2 = g->c_obj_coords[index][3];
        }
        radius = (x2 - x1) / 2.;
        center_x = x1 + radius;
        center_y = y1 + radius;
        
        // OUT OF BOX //
        if((x1 < 0. && x2 < 0.) || (x1 > g->c_width && x2 > g->c_width) || (y1 < 0. && y2 < 0.) || (y1 > g->c_height && y2 > g->c_height))
        {
            g->c_obj_names.pop_back();
            g->c_obj_types.pop_back();
            g->c_obj_coords.pop_back();
            g->c_obj_options.pop_back();
            return;
        }
        // IN BOX //
        else if((x1 > 0. && x2 > 0.) && (x1 < g->c_width && x2 < g->c_width) && (y1 > 0. && y2 > 0.) && (y1 < g->c_height && y2 < g->c_height))
        {
            return;
        }
        else
        {
            g->c_obj_names.pop_back();
            g->c_obj_types.pop_back();
            g->c_obj_coords.pop_back();
            g->c_obj_options.pop_back();
        }
        // BIGGER THAN BOX //
        if(x1 < 0. && x2 > g->c_width && y1 < 0 && y2 > g->c_height)
        {
            cicm_graphics_rectangle(g, 0., 0., g->c_width, g->c_height);
            if(stroked)
                cicm_graphics_stroke_preserve(g);
            else
                cicm_graphics_fill_preserve(g);
            return;
        }
        // CUT ? -> LETS DRAW IT // OK... NOT CLEAN..
        /*
        else
        {
            double nx, ny;
            cicm_graphics_move_to(g, center_x, center_y + radius);
            for (int i = 0; i < 72; i++)
            {
                nx = radius * cos((double)i / 72. * CICM_2PI);
                ny = radius * sin((double)i / 72. * CICM_2PI);
                cicm_graphics_line_to(g, nx + center_x + 0.5, ny + center_y + 0.5);
            }
            cicm_graphics_close_path(g);
            if(stroked)
                cicm_graphics_stroke_preserve(g);
            else
                cicm_graphics_fill_preserve(g);
            return;
        }*/
    }
    else if(strcmp(g->c_obj_types[index].c_str(), "line ") == 0)
    {
        t_rect lines;
        double x0, y0, x1, y1;
        for(int  i = 2; i < g->c_obj_coords[index].size(); i += 2)
        {
            x0 = g->c_obj_coords[index][i-2];
            y0 = g->c_obj_coords[index][i-1];
            x1 = g->c_obj_coords[index][i];
            y1 = g->c_obj_coords[index][i+1];
            lines = CohenSutherlandLineClipAndDraw(x0, y0, x1, y1, 0., 0., g->c_width, g->c_height);
            g->c_obj_coords[index][i-2] = lines.x;
            g->c_obj_coords[index][i-1] = lines.y;
            g->c_obj_coords[index][i]  = lines.width;
            g->c_obj_coords[index][i+1]  = lines.height;
        }
        
    }
    else if(strcmp(g->c_obj_types[index].c_str(), "polygon ") == 0)
    {
        for(int  i = 0; i < g->c_obj_coords[index].size(); i += 2)
        {

            g->c_obj_coords[index][i] = pd_clip_minmax(g->c_obj_coords[index][i],  0., g->c_width);
            g->c_obj_coords[index][i+1]= pd_clip_minmax(g->c_obj_coords[index][i+1],  0., g->c_height);
        }
    }
    else if(strcmp(g->c_obj_types[index].c_str(), "arc ") != 0)
    {
        for(int  i = 0; i < g->c_obj_coords[index].size(); i += 2)
        {
            g->c_obj_coords[index][i] = pd_clip_minmax(g->c_obj_coords[index][i], 0., g->c_width);
            g->c_obj_coords[index][i+1] = pd_clip_minmax(g->c_obj_coords[index][i+1], 0., g->c_height);
        }
    }
}

void cicm_graphics_set_line_width(t_egraphics *g, double width)
{
    g->c_line_width = pd_clip_min(width, 0.);
}

void cicm_graphics_set_source_jrgba(t_egraphics *g, t_ergba *rgba)
{
    g->c_color = gensym(cicm_rgba_to_hex(*rgba));
}

void cicm_graphics_fill(t_egraphics *g)
{
    cicm_graphics_fill_preserve(g);
    g->c_new_obj_type.clear();
    g->c_new_obj_coords.clear();
    g->c_new_obj_options.clear();
}

void cicm_graphics_fill_preserve(t_egraphics *g)
{
    if(g->c_new_obj_type.size() && g->c_new_obj_coords.size())
    {
        char text[256];
        long index = g->c_obj_names.size();
        
        g->c_obj_types.push_back(g->c_new_obj_type);
        g->c_obj_coords.push_back(g->c_new_obj_coords);
        g->c_obj_options.push_back(g->c_new_obj_options);
        
        sprintf(text, "%lx%s%ld", (unsigned long)g->c_owner, g->c_name->s_name, index);
        g->c_obj_names.push_back(text);
        
        sprintf(text, "-fill %s -outline  %s -width  0 -tags %s\n", g->c_color->s_name, g->c_color->s_name, g->c_obj_names[index].c_str());
        g->c_obj_options[index].append(text);
        cicm_graphics_apply_transforms(g, index);
        cicm_graphics_clip(g, index, 0);
    }
}

void cicm_graphics_stroke(t_egraphics *g)
{
    cicm_graphics_stroke_preserve(g);
    g->c_new_obj_type.clear();
    g->c_new_obj_coords.clear();
    g->c_new_obj_options.clear();
}

void cicm_graphics_stroke_preserve(t_egraphics *g)
{
    if(g->c_new_obj_type.size() && g->c_new_obj_coords.size())
    {
        char text[256];
        long index = g->c_obj_names.size();
        
        if(g->c_new_obj_type == "polygon ")
        {
            g->c_obj_types.push_back("line ");
            g->c_obj_coords.push_back(g->c_new_obj_coords);
            sprintf(text, "%lx%s%ld", (unsigned long)g->c_owner, g->c_name->s_name, index);
            g->c_obj_names.push_back(text);
            g->c_obj_options.push_back(g->c_new_obj_options);
            sprintf(text, "-fill %s -width %d -tags %s\n", g->c_color->s_name, (int)g->c_line_width, g->c_obj_names[index].c_str());
            g->c_obj_options[index].append(text);
        }
        else
        {
            g->c_obj_types.push_back(g->c_new_obj_type);
            g->c_obj_coords.push_back(g->c_new_obj_coords);
            sprintf(text, "%lx%s%ld", (unsigned long)g->c_owner, g->c_name->s_name, index);
            g->c_obj_names.push_back(text);
            g->c_obj_options.push_back(g->c_new_obj_options);
            sprintf(text, "-outline %s -width %d -tags %s\n", g->c_color->s_name, (int)g->c_line_width, g->c_obj_names[index].c_str());
            g->c_obj_options[index].append(text);
        }
        
        cicm_graphics_apply_transforms(g, index);
        cicm_graphics_clip(g, index, 1);
    }
}

void cicm_text_layout_draw(t_etextlayout* textlayout, t_egraphics *g)
{
    char text[1024];
    long index = g->c_obj_names.size();
    
    sprintf(text, "%lx%s%ld", (unsigned long)g->c_owner, g->c_name->s_name, index);
    g->c_obj_names.push_back(text);
    
    g->c_obj_types.push_back("text ");
    g->c_new_obj_coords.push_back(textlayout->c_rect.x);
    g->c_new_obj_coords.push_back(textlayout->c_rect.y);
    g->c_obj_coords.push_back(g->c_new_obj_coords);
    g->c_new_obj_coords.clear();
    
    g->c_obj_options.push_back("-text ");
    sprintf(text, "{%s} ", textlayout->c_text->s_name);
    g->c_obj_options[index].append(text);
    g->c_obj_options[index].append("-anchor ");
    g->c_obj_options[index].append(textlayout->c_justification->s_name);
    g->c_obj_options[index].append(" -font ");
    sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
    g->c_obj_options[index].append(text);
    sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
    g->c_obj_options[index].append(text);

    cicm_graphics_apply_transforms(g, index);
   
    // CLIP FOR TEXT //
    double x1, y1;
    x1 = g->c_obj_coords[index][0];
    y1 = g->c_obj_coords[index][1];
    if(x1 < -1 * (strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75) || x1 > g->c_width || y1 < textlayout->c_font.c_size / 2.  || y1 > g->c_height - textlayout->c_font.c_size / 2.)
    {
        g->c_obj_names.pop_back();
        g->c_obj_types.pop_back();
        g->c_obj_coords.pop_back();
        g->c_obj_options.pop_back();
    }
    else if(x1 > g->c_width - strlen(textlayout->c_text->s_name) * textlayout->c_font.c_size / 1.75)
    {
        int strsize = (g->c_width - x1) / (textlayout->c_font.c_size / 1.75);
        std::string newtext = textlayout->c_text->s_name;
        newtext.resize(strsize);
        g->c_obj_options[index].assign("-text ");
        sprintf(text, "{%s} ", newtext.c_str());
        g->c_obj_options[index].append(text);
        g->c_obj_options[index].append("-anchor ");
        g->c_obj_options[index].append(textlayout->c_justification->s_name);
        g->c_obj_options[index].append(" -font ");
        sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
        g->c_obj_options[index].append(text);
        sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
        g->c_obj_options[index].append(text);
    }
    else if(x1 < 0)
    {
        int strsize = x1 / (textlayout->c_font.c_size / 1.75) * -1;
        g->c_obj_coords[index][0] = 0.;
        g->c_obj_options[index].assign("-text ");
        sprintf(text, "{%s} ", textlayout->c_text->s_name+strsize);
        g->c_obj_options[index].append(text);
        g->c_obj_options[index].append("-anchor ");
        g->c_obj_options[index].append(textlayout->c_justification->s_name);
        g->c_obj_options[index].append(" -font ");
        sprintf(text, "{%s %d %s} ", textlayout->c_font.c_family->s_name, (int)textlayout->c_font.c_size, textlayout->c_font.c_weight->s_name);
        g->c_obj_options[index].append(text);
        sprintf(text, "-fill %s -width %d -tags %s\n", gensym(cicm_rgba_to_hex(textlayout->c_color))->s_name, (int)textlayout->c_rect.width, g->c_obj_names[index].c_str());
        g->c_obj_options[index].append(text);
    }
}


void cicm_graphics_rotate(t_egraphics *g, double angle)
{
    g->c_rotation += angle;
}


void cicm_graphics_set_matrix(t_egraphics *g, const t_cicm_matrix *matrix)
{
    g->c_matrix = matrix;
}

t_rect CohenSutherlandLineClipAndDraw(double x0, double y0, double x1, double y1, double xmin, double ymin, double xmax, double ymax)
{
    // compute outcodes for P0, P1, and whatever point lies outside the clip rectangle
    int outcode0 = point_in_rect(x0, y0, xmin, ymin, xmax, ymax);
    int outcode1 = point_in_rect(x1, y1, xmin, ymin, xmax, ymax);
    bool accept = false;
    
    while (true) {
        if (!(outcode0 | outcode1)) { // Bitwise OR is 0. Trivially accept and get out of loop
            accept = true;
            break;
        } else if (outcode0 & outcode1) { // Bitwise AND is not 0. Trivially reject and get out of loop
            break;
        } else {
            // failed both tests, so calculate the line segment to clip
            // from an outside point to an intersection with clip edge
            double x, y;
            
            // At least one endpoint is outside the clip rectangle; pick it.
            int outcodeOut = outcode0 ? outcode0 : outcode1;
            
            // Now find the intersection point;
            // use formulas y = y0 + slope * (x - x0), x = x0 + (1 / slope) * (y - y0)
            if (outcodeOut & 8) {           // point is above the clip rectangle
                x = x0 + (x1 - x0) * (ymax - y0) / (y1 - y0);
                y = ymax;
            } else if (outcodeOut & 4) { // point is below the clip rectangle
                x = x0 + (x1 - x0) * (ymin - y0) / (y1 - y0);
                y = ymin;
            } else if (outcodeOut & 2) {  // point is to the right of clip rectangle
                y = y0 + (y1 - y0) * (xmax - x0) / (x1 - x0);
                x = xmax;
            } else if (outcodeOut & 1) {   // point is to the left of clip rectangle
                y = y0 + (y1 - y0) * (xmin - x0) / (x1 - x0);
                x = xmin;
            }
            
            // Now we move outside point to intersection point to clip
            // and get ready for next pass.
            if (outcodeOut == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = point_in_rect(x0, y0, xmin, ymin, xmax, ymax);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = point_in_rect(x1, y1, xmin, ymin, xmax, ymax);
            }
        }
    }
    if (accept)
    {
        t_rect lines;
        lines.x = x0;
        lines.y = y0;
        lines.width = x1;
        lines.height = y1;
        return lines;
    }
    else
    {
        t_rect lines;
        lines.x = -1;
        lines.y = -1;
        lines.width = -1;
        lines.height = -1;
        return lines;
    }
        
}

int point_in_rect(double x, double y, double xmin, double ymin, double xmax, double ymax)
{
    int code = 0;
    
    if (x < xmin)           // to the left of clip window
        code |= 1;
    else if (x > xmax)      // to the right of clip window
        code |= 2;
    if (y < ymin)           // below the clip window
        code |= 4;
    else if (y > ymax)      // above the clip window
        code |= 8;
    
    return code;
}


