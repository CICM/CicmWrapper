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


#include "d_ugen.h"

extern t_class *vinlet_class, *voutlet_class, *canvas_class;
static void ugen_doit(t_dspcontext *dc, t_ugenbox *u);
t_float *obj_findsignalscalar(t_object *x, int m);
int ugen_loud;

// Create a DSP Context
t_dspcontext* dsp_context_new()
{
    t_dspcontext *dc = (t_dspcontext *)getbytes(sizeof(*dc));
    dc->dc_ugenlist = 0;
    dc->dc_toplevel = 1;
    dc->dc_iosigs = NULL;
    dc->dc_ninlets = 0;
    dc->dc_noutlets = 0;
    dc->dc_parentcontext = NULL;
    
    return dc;
}

// Add a canvas to a DSP Context //
void dsp_context_addcanvas(t_dspcontext* dc, t_canvas *cnv)
{
    t_linetraverser t;
    t_gobj          *y;
    t_object        *ob;
    t_symbol        *dspsym = gensym("dsp");
    t_outconnect    *oc;
    char            block_alert = 0;
    char            inlet_alert = 0;
    char            outlet_alert = 0;
    if(dc && cnv)
    {
        for(y = cnv->gl_list; y; y = y->g_next)
        {
            if((ob = pd_checkobject(&y->g_pd)) && zgetfn(&y->g_pd, dspsym))
            {
                if(ob->te_g.g_pd->c_name == gensym("block~"))
                {
                    if(!block_alert)
                        pd_error(ob, "block~ are not allowed in hoa.process~.");
                    block_alert = 1;
                }
                else if(ob->te_g.g_pd->c_name == gensym("inlet"))
                {
                    if(!inlet_alert)
                        pd_error(ob, "inlet~ are not allowed in hoa.process~.");
                    inlet_alert = 1;
                }
                else if(ob->te_g.g_pd->c_name == gensym("outlet"))
                {
                    if(!outlet_alert)
                        pd_error(ob, "outlet~ are not allowed in hoa.process~.");
                    outlet_alert = 1;
                }
                else
                    dsp_context_addbox(dc, ob);
            }
        }
        
        linetraverser_start(&t, cnv);
        while((oc = linetraverser_next(&t)))
        {
            if (obj_issignaloutlet(t.tr_ob, t.tr_outno))
                dsp_context_addconnection(dc, t.tr_ob, t.tr_outno, t.tr_ob2, t.tr_inno);
        }
    }
}

// Get the index of a box, or -1 if it's not on the list //
int dsp_context_getboxindex(t_dspcontext *dc, t_ugenbox *x)
{
    int ret;
    t_ugenbox *u;
    for (u = dc->dc_ugenlist, ret = 0; u; u = u->u_next, ret++)
        if (u == x) return (ret);
    return (-1);
}

// Add a box to a DSP Context //
void dsp_context_addbox(t_dspcontext *dc, t_object *obj)
{
    t_ugenbox *x = (t_ugenbox *)getbytes(sizeof *x);
    int i;
    t_sigoutlet *uout;
    t_siginlet *uin;
    
    x->u_next = dc->dc_ugenlist;
    dc->dc_ugenlist = x;
    x->u_obj = obj;
    
    x->u_nin = obj_nsiginlets(obj);
    x->u_in = getbytes(x->u_nin * sizeof (*x->u_in));
    for (uin = x->u_in, i = x->u_nin; i--; uin++)
    {
        uin->i_nconnect = 0;
    }
    
    x->u_nout = obj_nsigoutlets(obj);
    x->u_out = getbytes(x->u_nout * sizeof (*x->u_out));
    for (uout = x->u_out, i = x->u_nout; i--; uout++)
    {
        uout->o_connections = 0;
        uout->o_nconnect = 0;
    }
}

// Add a connection to a DSP Context //
void dsp_context_addconnection(t_dspcontext *dc, t_object *x1, int outno, t_object *x2,
                  int inno)
{
    t_ugenbox       *u1;
    t_ugenbox       *u2;
    t_sigoutlet     *uout;
    t_siginlet      *uin;
    t_sigoutconnect *oc;
    int sigoutno    = obj_sigoutletindex(x1, outno);
    int siginno     = obj_siginletindex(x2, inno);
    
    for (u1 = dc->dc_ugenlist; u1 && u1->u_obj != x1; u1 = u1->u_next);
    for (u2 = dc->dc_ugenlist; u2 && u2->u_obj != x2; u2 = u2->u_next);
    if (!u1 || !u2 || siginno < 0)
    {
        pd_error(u1->u_obj,  "signal outlet connect to nonsignal inlet (ignored)");
        return;
    }
    if (sigoutno < 0 || sigoutno >= u1->u_nout || siginno >= u2->u_nin)
    {
        bug("ugen_connect %s %s %d %d (%d %d)", class_getname(x1->ob_pd),  class_getname(x2->ob_pd), sigoutno, siginno, u1->u_nout, u2->u_nin);
    }
    uout = u1->u_out + sigoutno;
    uin = u2->u_in + siginno;
    
    // add a new connection to the outlet's list //
    oc = (t_sigoutconnect *)getbytes(sizeof *oc);
    oc->oc_next = uout->o_connections;
    uout->o_connections = oc;
    oc->oc_who = u2;
    oc->oc_inno = siginno;
    
    // update inlet and outlet counts  //
    uout->o_nconnect++;
    uin->i_nconnect++;
}

// Remove the canvas from a DSP Context //
void dsp_context_removecanvas(t_dspcontext *dc)
{
    int n;
    t_ugenbox *u;
    t_sigoutlet *uout;
    t_sigoutconnect *oc, *oc2;
    
    if(!dc)
        return;
    
    while (dc->dc_ugenlist)
    {
        // First free the connection //
        for (uout = dc->dc_ugenlist->u_out, n = dc->dc_ugenlist->u_nout; n--; uout++)
        {
            oc = uout->o_connections;
            while (oc)
            {
                oc2 = oc->oc_next;
                freebytes(oc, sizeof *oc);
                oc = oc2;
            }
        }
        
        // Then free the inlets and outlets //
        freebytes(dc->dc_ugenlist->u_out, dc->dc_ugenlist->u_nout * sizeof (*dc->dc_ugenlist->u_out));
        freebytes(dc->dc_ugenlist->u_in, dc->dc_ugenlist->u_nin * sizeof(*dc->dc_ugenlist->u_in));
        u = dc->dc_ugenlist;
        dc->dc_ugenlist = u->u_next;
        
        // Then free the boxes //
        freebytes(u, sizeof *u);
    }
}

// Free a DSP Context //
void dsp_context_free(t_dspcontext *dc)
{
    if(!dc)
        return;
    
    dsp_context_removecanvas(dc);
    freebytes(dc, sizeof(*dc));
}

// Compile a DSP Context //
void dsp_context_compile(t_dspcontext *dc)
{
    int i;
    t_ugenbox *u;
    t_sigoutlet *uout;
    t_siginlet *uin;
    t_signal **sigp;
    
    if(!dc)
        return;
    
    dc->dc_reblock = 1;
    dc->dc_switched = 0;
    dc->dc_srate = sys_getsr();
    dc->dc_vecsize = sys_getblksize();
    dc->dc_calcsize = sys_getblksize();
    
    if(dc->dc_iosigs)
    {
        for (i = 0, sigp = dc->dc_iosigs + dc->dc_ninlets; i < dc->dc_noutlets; i++, sigp++)
        {
            if ((*sigp)->s_isborrowed && !(*sigp)->s_borrowedfrom)
            {
                signal_setborrowed(*sigp, signal_new(dc->dc_vecsize, dc->dc_srate));
                (*sigp)->s_refcount++;
            }
        }
    }
    
    // Initialize for sorting //
    for(u = dc->dc_ugenlist; u; u = u->u_next)
    {
        u->u_done = 0;
        for(uout = u->u_out, i = u->u_nout; i--; uout++)
        {
            uout->o_nsent = 0;
        }
        for(uin = u->u_in, i = u->u_nin; i--; uin++)
        {
            uin->i_ngot = 0;
            uin->i_signal = 0;
        }
    }
    
    // Do the sort //
    for(u = dc->dc_ugenlist; u; u = u->u_next)
    {
        // Check that we have no connected signal inlets //
        if(u->u_done)
            continue;
        for(uin = u->u_in, i = u->u_nin; i--; uin++)
        {
            if (uin->i_nconnect)
                goto next;
        }
        ugen_doit(dc, u);
        //dsp_context_compilebox(dc, u);
    next: ;
    }
    
    // check for a DSP loop, which is evidenced here by the presence of ugens not yet scheduled. //
    for (u = dc->dc_ugenlist; u; u = u->u_next)
    {
        if (!u->u_done)
        {
            pd_error(u->u_obj,  "DSP loop detected (some tilde objects not scheduled)");
            // this might imply that we have unfilled "borrowed" outputs which we'd better fill in now. //
            for (i = 0, sigp = dc->dc_iosigs + dc->dc_ninlets; i < dc->dc_noutlets; i++, sigp++)
            {
                if((*sigp)->s_isborrowed && !(*sigp)->s_borrowedfrom)
                {
                    t_signal *s3 = signal_new(dc->dc_vecsize, dc->dc_srate);
                    signal_setborrowed(*sigp, s3);
                    (*sigp)->s_refcount++;
                    dsp_add_zero(s3->s_vec, s3->s_n);
                }
            }
            break;
        }
    }
}

/* put a ugenbox on the chain, recursively putting any others on that
 this one might uncover. */
static void ugen_doit(t_dspcontext *dc, t_ugenbox *u)
{
    t_sigoutlet *uout;
    t_siginlet *uin;
    t_sigoutconnect *oc;
    t_class *class = pd_class(&u->u_obj->ob_pd);
    int i, n;
    /* suppress creating new signals for the outputs of signal
     inlets and subpatchs; except in the case we're an inlet and "blocking"
     is set.  We don't yet know if a subcanvas will be "blocking" so there
     we delay new signal creation, which will be handled by calling
     signal_setborrowed in the ugen_done_graph routine below. */
    int nonewsigs = (class == canvas_class && !(dc->dc_reblock));
    /* when we encounter a subcanvas or a signal outlet, suppress freeing
     the input signals as they may be "borrowed" for the super or sub
     patch; same exception as above, but also if we're "switched" we
     have to do a copy rather than a borrow.  */
    int nofreesigs = (class == canvas_class &&  !(dc->dc_reblock || dc->dc_switched));
    t_signal **insig, **outsig, **sig, *s1, *s2, *s3;
    t_ugenbox *u2;
    
    if (ugen_loud) post("doit %s %d %d", class_getname(class), nofreesigs,
                        nonewsigs);
    for (i = 0, uin = u->u_in; i < u->u_nin; i++, uin++)
    {
        if (!uin->i_nconnect)
        {
            t_float *scalar;
            s3 = signal_new(dc->dc_vecsize, dc->dc_srate);
            /* post("%s: unconnected signal inlet set to zero",
             class_getname(u->u_obj->ob_pd)); */
            if((scalar = obj_findsignalscalar(u->u_obj, i)))
                dsp_add_scalarcopy(scalar, s3->s_vec, s3->s_n);
            else
                dsp_add_zero(s3->s_vec, s3->s_n);
            uin->i_signal = s3;
            s3->s_refcount = 1;
        }
    }
    insig = (t_signal **)getbytes((u->u_nin + u->u_nout) * sizeof(t_signal *));
    outsig = insig + u->u_nin;
    for (sig = insig, uin = u->u_in, i = u->u_nin; i--; sig++, uin++)
    {
        int newrefcount;
        *sig = uin->i_signal;
        newrefcount = --(*sig)->s_refcount;
        /* if the reference count went to zero, we free the signal now,
         unless it's a subcanvas or outlet; these might keep the
         signal around to send to objects connected to them.  In this
         case we increment the reference count; the corresponding decrement
         is in sig_makereusable(). */
        if (nofreesigs)
            (*sig)->s_refcount++;
        else if (!newrefcount)
            signal_makereusable(*sig);
    }
    for (sig = outsig, uout = u->u_out, i = u->u_nout; i--; sig++, uout++)
    {
        /* similarly, for outlets of subcanvases we delay creating
         them; instead we create "borrowed" ones so that the refcount
         is known.  The subcanvas replaces the fake signal with one showing
         where the output data actually is, to avoid having to copy it.
         For any other object, we just allocate a new output vector;
         since we've already freed the inputs the objects might get called
         "in place." */
        if (nonewsigs)
        {
            *sig = uout->o_signal =
            signal_new(0, dc->dc_srate);
        }
        else
            *sig = uout->o_signal = signal_new(dc->dc_vecsize, dc->dc_srate);
        (*sig)->s_refcount = uout->o_nconnect;
    }
    /* now call the DSP scheduling routine for the ugen.  This
     routine must fill in "borrowed" signal outputs in case it's either
     a subcanvas or a signal inlet. */
    mess1(&u->u_obj->ob_pd, gensym("dsp"), insig);
    
    /* if any output signals aren't connected to anyone, free them
     now; otherwise they'll either get freed when the reference count
     goes back to zero, or even later as explained above. */
    
    for (sig = outsig, uout = u->u_out, i = u->u_nout; i--; sig++, uout++)
    {
        if (!(*sig)->s_refcount)
            signal_makereusable(*sig);
    }

    /* pass it on and trip anyone whose last inlet was filled */
    for (uout = u->u_out, i = u->u_nout; i--; uout++)
    {
        s1 = uout->o_signal;
        for (oc = uout->o_connections; oc; oc = oc->oc_next)
        {
            u2 = oc->oc_who;
            uin = &u2->u_in[oc->oc_inno];
            /* if there's already someone here, sum the two */
            if((s2 = uin->i_signal))
            {
                s1->s_refcount--;
                s2->s_refcount--;
                if (!signal_compatible(s1, s2))
                {
                    pd_error(u->u_obj, "%s: incompatible signal inputs",
                             class_getname(u->u_obj->ob_pd));
                    return;
                }
                s3 = signal_newlike(s1);
                dsp_add_plus(s1->s_vec, s2->s_vec, s3->s_vec, s1->s_n);
                uin->i_signal = s3;
                s3->s_refcount = 1;
                if (!s1->s_refcount) signal_makereusable(s1);
                if (!s2->s_refcount) signal_makereusable(s2);
            }
            else uin->i_signal = s1;
            uin->i_ngot++;
            /* if we didn't fill this inlet don't bother yet */
            if (uin->i_ngot < uin->i_nconnect)
                goto notyet;
            /* if there's more than one, check them all */
            if (u2->u_nin > 1)
            {
                for (uin = u2->u_in, n = u2->u_nin; n--; uin++)
                    if (uin->i_ngot < uin->i_nconnect) goto notyet;
            }
            /* so now we can schedule the ugen.  */
            ugen_doit(dc, u2);
        notyet: ;
        }
    }
    t_freebytes(insig,(u->u_nin + u->u_nout) * sizeof(t_signal *));
    u->u_done = 1;
}

// Put a ugenbox on the chain, recursively putting any others on that this one might uncover. //
void dsp_context_compilebox(t_dspcontext *dc, t_ugenbox *u)
{
    
    int i, n;
    t_sigoutlet *uout;
    t_siginlet *uin;
    t_sigoutconnect *oc;
    t_signal **insig, **outsig, **sig, *s1, *s2, *s3;
    t_ugenbox *u2;
    
    for(i = 0, uin = u->u_in; i < u->u_nin; i++, uin++)
    {
        if(!uin->i_nconnect)
        {
            t_float *scalar;
            s3 = signal_new(dc->dc_vecsize, dc->dc_srate);
            
            if((scalar = obj_findsignalscalar(u->u_obj, i)))
                dsp_add_scalarcopy(scalar, s3->s_vec, s3->s_n);
            else
                dsp_add_zero(s3->s_vec, s3->s_n);
            
            uin->i_signal = s3;
            s3->s_refcount = 1;
        }
    }
    
    insig = (t_signal **)getbytes((u->u_nin + u->u_nout) * sizeof(t_signal *));
    outsig = insig + u->u_nin;
    
    for (sig = insig, uin = u->u_in, i = u->u_nin; i--; sig++, uin++)
    {
        int newrefcount;
        *sig = uin->i_signal;
        newrefcount = --(*sig)->s_refcount;
        /* if the reference count went to zero, we free the signal now,
         unless it's a subcanvas or outlet; these might keep the
         signal around to send to objects connected to them.  In this
         case we increment the reference count; the corresponding decrement
         is in sig_makereusable(). */
        if (!newrefcount && (u->u_obj->te_g.g_pd->c_name != gensym("hoa.in~") || u->u_obj->te_g.g_pd->c_name != gensym("hoa.out~")))
            signal_makereusable(*sig);
    }
    for (sig = outsig, uout = u->u_out, i = u->u_nout; i--; sig++, uout++)
    {
        /* similarly, for outlets of subcanvases we delay creating
         them; instead we create "borrowed" ones so that the refcount
         is known.  The subcanvas replaces the fake signal with one showing
         where the output data actually is, to avoid having to copy it.
         For any other object, we just allocate a new output vector;
         since we've already freed the inputs the objects might get called
         "in place." */
        
        *sig = uout->o_signal = signal_new(dc->dc_vecsize, dc->dc_srate);
        (*sig)->s_refcount = uout->o_nconnect;
    }
    /* now call the DSP scheduling routine for the ugen.  This
     routine must fill in "borrowed" signal outputs in case it's either
     a subcanvas or a signal inlet. */
    mess1(&u->u_obj->ob_pd, gensym("dsp"), insig);
    
    /* if any output signals aren't connected to anyone, free them
     now; otherwise they'll either get freed when the reference count
     goes back to zero, or even later as explained above. */
    
    for (sig = outsig, uout = u->u_out, i = u->u_nout; i--; sig++, uout++)
    {
        if (!(*sig)->s_refcount)
            signal_makereusable(*sig);
    }
    
    /* pass it on and trip anyone whose last inlet was filled */
    for (uout = u->u_out, i = u->u_nout; i--; uout++)
    {
        s1 = uout->o_signal;
        for (oc = uout->o_connections; oc; oc = oc->oc_next)
        {
            u2 = oc->oc_who;
            uin = &u2->u_in[oc->oc_inno];
            /* if there's already someone here, sum the two */
            if((s2 = uin->i_signal))
            {
                s1->s_refcount--;
                s2->s_refcount--;
                if (!signal_compatible(s1, s2))
                {
                    pd_error(u->u_obj, "%s: incompatible signal inputs",
                             class_getname(u->u_obj->ob_pd));
                    return;
                }
                s3 = signal_newlike(s1);
                dsp_add_plus(s1->s_vec, s2->s_vec, s3->s_vec, s1->s_n);
                uin->i_signal = s3;
                s3->s_refcount = 1;
                if (!s1->s_refcount) signal_makereusable(s1);
                if (!s2->s_refcount) signal_makereusable(s2);
            }
            else uin->i_signal = s1;
            uin->i_ngot++;
            /* if we didn't fill this inlet don't bother yet */
            if (uin->i_ngot < uin->i_nconnect)
                goto notyet;
            /* if there's more than one, check them all */
            if (u2->u_nin > 1)
            {
                for (uin = u2->u_in, n = u2->u_nin; n--; uin++)
                    if (uin->i_ngot < uin->i_nconnect) goto notyet;
            }
            /* so now we can schedule the ugen.  */
            dsp_context_compilebox(dc, u2);
        notyet: ;
        }
    }
    t_freebytes(insig,(u->u_nin + u->u_nout) * sizeof(t_signal *));
    u->u_done = 1;
}

t_int *zero_perform(t_int *w)   /* zero out a vector */
{
    t_sample *out = (t_sample *)(w[1]);
    int n = (int)(w[2]);
    while (n--) *out++ = 0; 
    return (w+3);
}

t_int *zero_perf8(t_int *w)
{
    t_sample *out = (t_sample *)(w[1]);
    int n = (int)(w[2]);
    
    for (; n; n -= 8, out += 8)
    {
        out[0] = 0;
        out[1] = 0;
        out[2] = 0;
        out[3] = 0;
        out[4] = 0;
        out[5] = 0;
        out[6] = 0;
        out[7] = 0;
    }
    return (w+3);
}

void dsp_add_zero(t_sample *out, int n)
{
    if (n&7)
        dsp_add(zero_perform, 2, out, n);
    else        
        dsp_add(zero_perf8, 2, out, n);
}

/* ---------------- signals ---------------------------- */

int ilog2(int n)
{
    int r = -1;
    if (n <= 0) return(0);
    while (n)
    {
        r++;
        n >>= 1;
    }
    return (r);
}

    /* list of signals which can be reused, sorted by buffer size */
t_signal *signal_freelist[MAXLOGSIG+1];
    /* list of reusable "borrowed" signals (which don't own sample buffers) */
t_signal *signal_freeborrowed;
    /* list of all signals allocated (not including "borrowed" ones) */
t_signal *signal_usedlist;

/* call this when DSP is stopped to free all the signals */
void signal_cleanup(void)
{
    t_signal *sig;
    int i;
    while ((sig = signal_usedlist))
    {
        signal_usedlist = sig->s_nextused;
        if (!sig->s_isborrowed)
            t_freebytes(sig->s_vec, sig->s_vecsize * sizeof (*sig->s_vec));
        t_freebytes(sig, sizeof *sig);
    }
    for (i = 0; i <= MAXLOGSIG; i++)
        signal_freelist[i] = 0;
    signal_freeborrowed = 0;
}

    /* mark the signal "reusable." */
void signal_makereusable(t_signal *sig)
{
    int logn = ilog2(sig->s_vecsize);
#if 1
    t_signal *s5;
    for (s5 = signal_freeborrowed; s5; s5 = s5->s_nextfree)
    {
        if (s5 == sig)
        {
            bug("signal_free 3");
            return;
        }
    }
    for (s5 = signal_freelist[logn]; s5; s5 = s5->s_nextfree)
    {
        if (s5 == sig)
        {
            bug("signal_free 4");
            return;
        }
    }
#endif
    if (ugen_loud) post("free %lx: %d", sig, sig->s_isborrowed);
    if (sig->s_isborrowed)
    {
            /* if the signal is borrowed, decrement the borrowed-from signal's
                reference count, possibly marking it reusable too */
        t_signal *s2 = sig->s_borrowedfrom;
        if ((s2 == sig) || !s2)
            bug("signal_free");
        s2->s_refcount--;
        if (!s2->s_refcount)
            signal_makereusable(s2);
        sig->s_nextfree = signal_freeborrowed;
        signal_freeborrowed = sig;
    }
    else
    {
            /* if it's a real signal (not borrowed), put it on the free list
                so we can reuse it. */
        if (signal_freelist[logn] == sig) bug("signal_free 2");
        sig->s_nextfree = signal_freelist[logn];
        signal_freelist[logn] = sig;
    }
}

    /* reclaim or make an audio signal.  If n is zero, return a "borrowed"
    signal whose buffer and size will be obtained later via
    signal_setborrowed(). */

t_signal *signal_new(int n, t_float sr)
{
    int logn, vecsize = 0;
    t_signal *ret, **whichlist;
    logn = ilog2(n);
    if (n)
    {
        if ((vecsize = (1<<logn)) != n)
            vecsize *= 2;
        if (logn > MAXLOGSIG)
            bug("signal buffer too large");
        whichlist = signal_freelist + logn;
    }
    else
        whichlist = &signal_freeborrowed;

        /* first try to reclaim one from the free list */
    if ((ret = *whichlist))
        *whichlist = ret->s_nextfree;
    else
    {
            /* LATER figure out what to do for out-of-space here! */
        ret = (t_signal *)t_getbytes(sizeof *ret);
        if (n)
        {
            ret->s_vec = (t_sample *)getbytes(vecsize * sizeof (*ret->s_vec));
            ret->s_isborrowed = 0;
        }
        else
        {
            ret->s_vec = 0;
            ret->s_isborrowed = 1;
        }
        ret->s_nextused = signal_usedlist;
        signal_usedlist = ret;
    }
    ret->s_n = n;
    ret->s_vecsize = vecsize;
    ret->s_sr = sr;
    ret->s_refcount = 0;
    ret->s_borrowedfrom = 0;
    if (ugen_loud) post("new %lx: %d", ret, ret->s_isborrowed);
    return (ret);
}

t_signal *signal_newlike(const t_signal *sig)
{
    return (signal_new(sig->s_n, sig->s_sr));
}

void signal_setborrowed(t_signal *sig, t_signal *sig2)
{
    if (!sig->s_isborrowed || sig->s_borrowedfrom)
        bug("signal_setborrowed");
    if (sig == sig2)
        bug("signal_setborrowed 2");
    sig->s_borrowedfrom = sig2;
    sig->s_vec = sig2->s_vec;
    sig->s_n = sig2->s_n;
    sig->s_vecsize = sig2->s_vecsize;
}

int signal_compatible(t_signal *s1, t_signal *s2)
{
    return (s1->s_n == s2->s_n && s1->s_sr == s2->s_sr);
}




