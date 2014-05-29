/*
// Copyright (c) 2012-2014 Eliott Paris & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_JUCEBOX_WRAPPER
#define DEF_JUCEBOX_WRAPPER

#ifdef JUCE_WRAPPER

extern "C" {
#include "../epd.h"
#include "../ecommon/ecommon.h"
#include "../epd_max.h"
}

#include <JuceHeader.h>

using namespace juce;
extern void initialiseMac(void);

class PdJuceComponent : public Component
{
private:
    t_ebox*  m_box;
    Colour ecolor_to_jcolor(t_rgba color);
    
public:
    
	PdJuceComponent(t_ebox* owner);
    void paint (Graphics& g);
    ~PdJuceComponent();
};


typedef struct _jucebox
{
	t_ebox              j_box;
	PdJuceComponent*    j_component;
} t_jucebox;

typedef struct _dspjucebox
{
	t_edspbox           j_box;
	PdJuceComponent*    j_component;
} t_dspjucebox;

void ejucebox_new(t_jucebox* x);
void ejucebox_initclass(t_eclass* c, method paint, long flags);
void ejucebox_free(t_jucebox* x);
void ejucebox_paint(t_jucebox* x, t_object *patcherview);
void ejucebox_change(t_jucebox* x, void* wm);

void ejucewidget_vis(t_gobj *z, t_glist *glist, int vis);
void ejucewidget_displace(t_gobj *z, t_glist *glist, int dx, int dy);
void ejucewidget_select(t_gobj *z, t_glist *glist, int selected);
void ejucewidget_delete(t_gobj *z, t_glist *glist);

#endif


#endif