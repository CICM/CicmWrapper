/*
 // Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
 // For information on usage and redistribution, and for a DISCLAIMER OF ALL
 // WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 */

#include "jucebox_wrapper.h"

void jucebox_initclass(t_eclass* c, method paint, long flags)
{
    //eclass_init((t_eclass *)c, flags);
    
	eclass_addmethod((t_eclass *)c, (method)jucebox_paint, "paint", A_CANT, 0);
    eclass_addmethod((t_eclass *)c, (method)paint, "jucebox_paint", A_CANT, 0);
    
    initialiseMac();
}

void jucebox_new(t_jucebox* x)
{
    JUCE_AUTORELEASEPOOL
    {
        initialiseJuce_GUI();
        x->j_component = new PdJuceComponent((t_ebox *)x);
    }
}

void jucebox_free(t_jucebox* x)
{
    ebox_free((t_ebox *)x);
	x->j_component->removeFromDesktop();
    delete x->j_component;
}

void jucebox_paint(t_jucebox* x, t_object *patcherview)
{
    x->j_component->repaint();
}


void jucebox_setsize(t_jucebox* x)
{
    ;
}
