/*
 * CicmWrapper
 * Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8
 * All rights reserved.
 * Website  : https://github.com/CICM/CicmWrapper
 * Contacts : cicm.mshparisnord@gmail.com
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "epath.h"

/**
 * @enum epath_maker
 * @brief The types of path.
 * @details It define all the path type.
 */
typedef enum
{
    E_PATH_MOVE     = 0,   /*!< This type is move. */
    E_PATH_LINE     = 1,   /*!< This type is line maker. */
    E_PATH_CURVE    = 2,   /*!< This type is curve. */
    E_PATH_CLOSE    = 3,   /*!< This type is curve. */
    E_PATH_RECT     = 4,   /*!< This type is rectangle. */
    E_PATH_OVAL     = 5,   /*!< This type is oval. */
    E_PATH_ARC      = 6    /*!< This type is arc. */
} epath_maker;


/**
 * @struct t_epath
 * @brief The path object.
 * @details It contains the all the informations to be drawn.
 */
struct _epath
{
    t_pt*   p_points;   /*!< The points. */
    size_t  p_size;     /*!< The number of valid points. */
    size_t  p_rsize;    /*!< The real number of points. */
};

static inline char epath_prealloc(t_epath *p, const size_t size)
{
    t_pt* temp;
    if(p->p_size + size > p->p_rsize)
    {
        if(p->p_rsize && p->p_points)
        {
            temp = (t_pt *)realloc(p->p_points, (p->p_size + size) * sizeof(t_pt));
            if(temp)
            {
                p->p_points = temp;
                p->p_rsize  = p->p_size + size;
                return 1;
            }
            pd_error(NULL, "can't allocate enought space for path.");
            return 0;
        }
        else
        {
            p->p_points = (t_pt *)malloc(size * sizeof(t_pt));
            if(p->p_points)
            {
                p->p_rsize = size;
                return 1;
            }
            pd_error(NULL, "can't allocate enought space for path.");
            return 0;
        }
    }
    return 1;
}

t_epath* epath_new(void)
{
    t_epath *p = malloc(sizeof(t_epath));
    if(p)
    {
        p->p_points = NULL;
        p->p_rsize  = 0;
        p->p_size   = 0;
    }
    return p;
}

void epath_free(t_epath *p)
{
    if(p->p_points || p->p_rsize)
    {
        free(p->p_points);
    }
    p->p_points = NULL;
    p->p_rsize  = 0;
    p->p_size   = 0;
    free(p);
}

void epath_clear(t_epath *p)
{
    p->p_size   = 0;
}

void epath_move_to(t_epath *p, const float x, const float y)
{
    if(epath_prealloc(p, 2))
    {
        p->p_points[p->p_size].x    = E_PATH_MOVE;
        p->p_points[p->p_size+1].x  = x;
        p->p_points[p->p_size+1].y  = y;
        p->p_size += 2;
    }
}

void epath_line_to(t_epath *p, const float x, const float y)
{
    if(epath_prealloc(p, 2))
    {
        p->p_points[p->p_size].x    = E_PATH_LINE;
        p->p_points[p->p_size+1].x  = x;
        p->p_points[p->p_size+1].y  = y;
        p->p_size += 2;
    }
}

void epath_line(t_epath *p, const float x0, const float y0, const float x1, const float y1)
{
    if(epath_prealloc(p, 4))
    {
        p->p_points[p->p_size].x    = E_PATH_MOVE;
        p->p_points[p->p_size+1].x  = x0;
        p->p_points[p->p_size+1].y  = y0;
        p->p_points[p->p_size+2].x  = E_PATH_LINE;
        p->p_points[p->p_size+3].x  = x1;
        p->p_points[p->p_size+3].y  = y1;
        p->p_size += 4;
    }
}

void epath_curve_to(t_epath *p, const float c1x, const float c1y,
                                const float c2x, const float c2y,
                                const float ex, const float ey)
{
    if(epath_prealloc(p, 4))
    {
        p->p_points[p->p_size].x    = E_PATH_CURVE;
        p->p_points[p->p_size+1].x  = c1x;
        p->p_points[p->p_size+1].y  = c1y;
        p->p_points[p->p_size+2].x  = c2x;
        p->p_points[p->p_size+2].y  = c1y;
        p->p_points[p->p_size+3].x  = ex;
        p->p_points[p->p_size+3].y  = ey;
        p->p_size += 4;
    }
}

void epath_curve(t_epath *p, const float sx, const float sy,
                  const float c1x, const float c1y,
                  const float c2x, const float c2y,
                  const float ex, const float ey)
{
    if(epath_prealloc(p, 6))
    {
        p->p_points[p->p_size].x    = E_PATH_MOVE;
        p->p_points[p->p_size+1].x  = sx;
        p->p_points[p->p_size+1].y  = sy;
        p->p_points[p->p_size+2].x  = E_PATH_CURVE;
        p->p_points[p->p_size+3].x  = c1x;
        p->p_points[p->p_size+3].y  = c1y;
        p->p_points[p->p_size+4].x  = c2x;
        p->p_points[p->p_size+4].y  = c1y;
        p->p_points[p->p_size+5].x  = ex;
        p->p_points[p->p_size+5].y  = ey;
        p->p_size += 6;
    }
}

void epath_copy_path(t_epath *p, t_epath const* other)
{
    if(other->p_size && epath_prealloc(p, other->p_size))
    {
        memcpy(p->p_points, other->p_points, other->p_size * sizeof(t_pt));
        p->p_size = other->p_size;
    }
    else
    {
        p->p_size = 0;
    }
}

void epath_swap_path(t_epath *p, t_epath *other)
{
    t_epath temp;
    memcpy(&temp, other, sizeof(t_epath));
    memcpy(other, p, sizeof(t_epath));
    memcpy(p, &temp, sizeof(t_epath));
}

void epath_add_path(t_epath *p, t_epath const* other)
{
    if(other->p_size && epath_prealloc(p, other->p_size))
    {
        memcpy(p->p_points+p->p_size, other->p_points, other->p_size * sizeof(t_pt));
        p->p_size += other->p_size;
    }
}

void epath_close(t_epath *p)
{
    if(epath_prealloc(p, 1))
    {
        p->p_points[p->p_size].x    = E_PATH_CLOSE;
    }
}

void epath_rectangle_rounded(t_epath *p, const float x, const float y,
                                         const float width, const float height,
                                         const float roundness)
{
    if(epath_prealloc(p, 4))
    {
        p->p_points[p->p_size].x    = E_PATH_RECT;
        p->p_points[p->p_size+1].x  = x;
        p->p_points[p->p_size+1].y  = y;
        p->p_points[p->p_size+2].x  = x + width;
        p->p_points[p->p_size+2].y  = y + height;
        p->p_points[p->p_size+3].x  = roundness;
        p->p_size += 4;
    }
}


void epath_rectangle(t_epath *p, const float x, const float y,
                                 const float width, const float height)
{
    epath_rectangle_rounded(p, x, y, width, height, 0.f);
}

void epath_oval(t_epath *p, const float xc, const float yc, const float radiusx, const float radiusy)
{
    if(epath_prealloc(p, 3))
    {
        p->p_points[p->p_size].x    = E_PATH_OVAL;
        p->p_points[p->p_size+1].x  = xc - radiusx;
        p->p_points[p->p_size+1].y  = yc - radiusy;
        p->p_points[p->p_size+2].x  = xc + radiusx;
        p->p_points[p->p_size+2].y  = yc + radiusy;
        p->p_size += 3;
    }
}

void epath_circle(t_epath *p, const float xc, const float yc, const float radius)
{
    epath_oval(p, xc, yc, radius, radius);
}

void epath_arc_oval(t_epath *p, const float xc, const float yc,
                                const float radiusx, const float radiusy,
                                const float angle1, const float angle2)
{
    if(epath_prealloc(p, 4))
    {
        p->p_points[p->p_size].x    = E_PATH_ARC;
        p->p_points[p->p_size+1].x  = xc - radiusx;
        p->p_points[p->p_size+1].y  = yc - radiusy;
        p->p_points[p->p_size+2].x  = xc + radiusx;
        p->p_points[p->p_size+2].y  = yc + radiusy;
        p->p_points[p->p_size+3].x  = angle1;
        p->p_points[p->p_size+3].y  = angle2;
        p->p_size += 4;
    }
}

void epath_arc(t_epath *p, const float xc, const float yc, const float radius,
                           const float angle1, const float angle2)
{
    epath_arc_oval(p, xc, yc, radius, radius, angle1, angle2);
}

static inline void epoint_apply_matrix(t_pt *pt, t_matrix const* matrix)
{
    t_pt temp;
    temp.x = pt->x * matrix->xx + pt->y * matrix->xy + matrix->x0;
    temp.y = pt->x * matrix->yx + pt->y * matrix->yy + matrix->y0;
}

void epath_apply_matrix(t_epath *p, t_matrix const* matrix)
{
    size_t i = 0;
    while(i < p->p_size)
    {
        if(p->p_points[i].x == E_PATH_MOVE || p->p_points[i].x == E_PATH_LINE)
        {
            epoint_apply_matrix(p->p_points+i+1, matrix);
            i += 2;
        }
        else if(p->p_points[i].x == E_PATH_CURVE)
        {
            epoint_apply_matrix(p->p_points+i+1, matrix);
            epoint_apply_matrix(p->p_points+i+2, matrix);
            epoint_apply_matrix(p->p_points+i+3, matrix);
            i += 4;
        }
        else if(p->p_points[i].x == E_PATH_CLOSE)
        {
            i += 1;
        }
        else if(p->p_points[i].x == E_PATH_RECT)
        {
            epoint_apply_matrix(p->p_points+i+1, matrix);
            epoint_apply_matrix(p->p_points+i+2, matrix);
            i += 4;
        }
        else if(p->p_points[i].x == E_PATH_OVAL)
        {
            epoint_apply_matrix(p->p_points+i+1, matrix);
            epoint_apply_matrix(p->p_points+i+2, matrix);
            i += 3;
        }
        else if(p->p_points[i].x == E_PATH_ARC)
        {
            epoint_apply_matrix(p->p_points+i+1, matrix);
            epoint_apply_matrix(p->p_points+i+2, matrix);
            i += 4;
        }
    }
}









