/* Copyright (c) 1997-1999 Miller Puckette.
* For information on usage and redistribution, and for a DISCLAIMER OF ALL
* WARRANTIES, see the file, "LICENSE.txt," in this distribution.  */

/*  These routines build a copy of the DSP portion of a graph, which is
    then sorted into a linear list of DSP operations which are added to
    the DSP duty cycle called by the scheduler.  Once that's been done,
    we delete the copy.  The DSP objects are represented by "ugenbox"
    structures which are parallel to the DSP objects in the graph and
    have vectors of siginlets and sigoutlets which record their
    interconnections.
*/

#ifndef DEF_EPD_UGEN
#define DEF_EPD_UGEN

#ifndef __m_pd_h_

#ifdef PD_EXTENDED
#include "pd-extended/m_pd.h"
#include "pd-extended/m_imp.h"
#include "pd-extended/g_canvas.h"
#include "pd-extended/s_stuff.h"
#else

#include "pd-vanilla/m_pd.h"
#include "pd-vanilla/m_imp.h"
#include "pd-vanilla/g_canvas.h"
#include "pd-vanilla/s_stuff.h"
#endif
#include <stdlib.h>
#include <stdarg.h>

#endif

typedef struct _ugenbox
{
    struct _siginlet *u_in;
    int u_nin;
    struct _sigoutlet *u_out;
    int u_nout;
    int u_phase;
    struct _ugenbox *u_next;
    t_object *u_obj;
    int u_done;
} t_ugenbox;

typedef struct _siginlet
{
    int i_nconnect;
    int i_ngot;
    t_signal *i_signal;
} t_siginlet;

typedef struct _sigoutconnect
{
    t_ugenbox *oc_who;
    int oc_inno;
    struct _sigoutconnect *oc_next;
} t_sigoutconnect;

typedef struct _sigoutlet
{
    int o_nconnect;
    int o_nsent;
    t_signal *o_signal;
    t_sigoutconnect *o_connections;
} t_sigoutlet;


typedef struct _dspcontext
{
    struct _ugenbox *dc_ugenlist;
    struct _dspcontext *dc_parentcontext;
    int dc_ninlets;
    int dc_noutlets;
    t_signal **dc_iosigs;
    t_float dc_srate;
    int dc_vecsize;         /* vector size, power of two */
    int dc_calcsize;        /* number of elements to calculate */
    char dc_toplevel;       /* true if "iosigs" is invalid. */
    char dc_reblock;        /* true if we have to reblock inlets/outlets */
    char dc_switched;       /* true if we're switched */
    
}t_dspcontext;

t_dspcontext* dsp_context_new();
void dsp_context_addcanvas(t_dspcontext* dc, t_canvas *cnv);
void dsp_context_addobject(t_dspcontext *dc, t_object *obj);
void dsp_context_addconnection(t_dspcontext *dc, t_object *x1, int outno, t_object *x2,  int inno);
void dsp_context_removecanvas(t_dspcontext *dc);
void dsp_context_free(t_dspcontext *dc);
void dsp_context_compile(t_dspcontext *dc);
void dsp_context_compilebox(t_dspcontext *dc, t_ugenbox *u);

t_signal *signal_new(int n, t_float sr);
void signal_setborrowed(t_signal *sig, t_signal *sig2);
void signal_makereusable(t_signal *sig);
t_signal *signal_newlike(const t_signal *sig);
int signal_compatible(t_signal *s1, t_signal *s2);
void signal_cleanup(void);

#endif

