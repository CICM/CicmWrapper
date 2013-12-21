/*
 * this is intended to be an example of writing a proxy class that will handle
 * arbitrary list input on a 'cold' inlet.
 *
 * I believe that if you replace 'proxy' with the name of your proxy class, and 'yourclass'
 * with the name of your base class, you can reuse this for the basis of a new external.
 */
#include "m_pd.h"

/*
 * declare the proxy object class
 */
t_class *proxy_class = NULL;

/*
 * declare your class
 */
t_class *yourclass_class = NULL;

typedef struct _proxy {
	t_pd l_pd;
	// if you want to maintain a pointer to your main class,
	// if not, besure to change the 'init' function
	void *yourclass;
} t_proxy;

typedef struct _yourclass {
	t_object x_obj;
	t_proxy pxy;
} t_yourclass;

static void proxy_init(t_proxy *p, t_yourclass *b) {
	p -> l_pd = proxy_class;
	
	p -> yourclass = (void *) b;
}

#define SYMBOL_LENGTH 100
static void proxy_anything(t_proxy *x, t_symbol *s, int argc, t_atom *argv)
{
	int i;
	char buf[SYMBOL_LENGTH];
	post("proxy_anything: %s", s -> s_name);
	for(i = 0; i < argc; i++) {
		atom_string(&argv[i], buf, SYMBOL_LENGTH);
		post("argv[%d]: %s", i, buf);
	}
}

/*
 * I don't think this ever gets called, but may be required for completeness
 *
 * this is pretty much replaced by the 'init' function, that you call from your 'new'.
 */
static void *proxy_new(t_symbol *s, int argc, t_atom *argv) {
	t_proxy *x = (t_proxy *)pd_new(proxy_class);
	if (x) {
		post("proxy_new");
	} else {
		post("proxy_new: could not create memory for new");
	}
	return x;
}

/*
 * This too is probably superfluous, unless you create something in the init function,
 * inwhich case, you should probably call it from your free method in your class.
 */
static void proxy_free(t_proxy *p) {
	post("proxy_free");
}

static void proxy_setup(void) {
	post("proxy_setup");
	proxy_class =
		(t_class *)class_new(gensym("proxy"),
							 (t_newmethod)proxy_new,
							 (t_method)proxy_free,
							 sizeof(t_proxy),
							 0,
							 A_GIMME,
							 0);
	class_addanything(proxy_class, (t_method)proxy_anything);
}

static void *yourclass_new(t_symbol *s, int argc, t_atom *argv) {
	t_yourclass *x = (t_yourclass *)pd_new(yourclass_class);
	if (x) {
		// first make the sql_buffer
		proxy_init(&x -> pxy, x);
		
		// this connects up the proxy inlet
		inlet_new(&x -> x_obj, &x -> pxy.l_pd, 0, 0);
		post("yourclass_new");
	}
	return x;
}

static void yourclass_free(t_yourclass *x) {
	post("yourclass_free");
}

void yourclass_setup(void) {
	post("yourclass_setup");
	yourclass_class =
		(t_class *)class_new(gensym("yourclass"),
							 (t_newmethod)yourclass_new,
							 (t_method)yourclass_free,
							 sizeof(t_yourclass),
							 0,
							 A_GIMME,
							 0);
	
	// be sure to call the proxy class setup before we finish
	proxy_setup();
}
