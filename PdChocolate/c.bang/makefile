###################################################
#
# Makefile for c.bang.pd_darwin
# Creator [Xcode -> Makefile Ver: Feb 14 2007 09:18:41]
# Created: [Sat Jan 18 11:45:58 2014]
#
###################################################

#
# Macros
#

CC = /usr/bin/gcc 
CC_OPTIONS =
LNK_OPTIONS = \
		-framework Accelerate\
		-LSystem/Library/Frameworks\
		-arch i386\
		-dynamiclib


#
# INCLUDE directories for c.bang.pd_darwin
#

INCLUDE = -I.\
		-I../../Sources/ebox\
		-I../../Sources/eclass\
		-I../../Sources/ecommon\
		-I../../Sources/egraphics\
		-I../../Sources/eobj\
		-I../../Sources\
		-I../../Sources/epopup


#
# Build c.bang.pd_darwin
#

c.bang.pd_darwin : \
		./c.bang.o\
		./ebox_attr.o\
		./ebox_basic.o\
		./ebox_draw.o\
		./ebox_handle.o\
		./ebox_patcher.o\
		./ebox_window.o\
		./eclass_attrgetter.o\
		./eclass_attrnew.o\
		./eclass_attroptions.o\
		./eclass_attrsetter.o\
		./eclass_basic.o\
		./eclass_widget.o\
		./ecommon.o\
		./egraphics.o\
		./egraphics_color.o\
		./egraphics_forms.o\
		./egraphics_matrix.o\
		./egraphics_text.o\
		./eobj_basic.o\
		./eobj_dsp.o\
		./eobj_handle.o\
		./eobj_proxy.o\
		./eobj_router.o\
		./epopup.o
	$(CC) $(LNK_OPTIONS) \
		./c.bang.o\
		./ebox_attr.o\
		./ebox_basic.o\
		./ebox_draw.o\
		./ebox_handle.o\
		./ebox_patcher.o\
		./ebox_window.o\
		./eclass_attrgetter.o\
		./eclass_attrnew.o\
		./eclass_attroptions.o\
		./eclass_attrsetter.o\
		./eclass_basic.o\
		./eclass_widget.o\
		./ecommon.o\
		./egraphics.o\
		./egraphics_color.o\
		./egraphics_forms.o\
		./egraphics_matrix.o\
		./egraphics_text.o\
		./eobj_basic.o\
		./eobj_dsp.o\
		./eobj_handle.o\
		./eobj_proxy.o\
		./eobj_router.o\
		./epopup.o\
		-o c.bang.pd_darwin

clean : 
		rm \
		./c.bang.o\
		./ebox_attr.o\
		./ebox_basic.o\
		./ebox_draw.o\
		./ebox_handle.o\
		./ebox_patcher.o\
		./ebox_window.o\
		./eclass_attrgetter.o\
		./eclass_attrnew.o\
		./eclass_attroptions.o\
		./eclass_attrsetter.o\
		./eclass_basic.o\
		./eclass_widget.o\
		./ecommon.o\
		./egraphics.o\
		./egraphics_color.o\
		./egraphics_forms.o\
		./egraphics_matrix.o\
		./egraphics_text.o\
		./eobj_basic.o\
		./eobj_dsp.o\
		./eobj_handle.o\
		./eobj_proxy.o\
		./eobj_router.o\
		./epopup.o\
		c.bang.pd_darwin

install : c.bang.pd_darwin
		cp c.bang.pd_darwin c.bang.pd_darwin

#
# Build the parts of c.bang.pd_darwin
#


# Item # 1 -- c.bang --
./c.bang.o : c.bang.cpp
	$(CC) $(CC_OPTIONS) c.bang.cpp -c $(INCLUDE) -o ./c.bang.o


# Item # 2 -- ebox_attr --
./ebox_attr.o : ../../Sources/ebox/ebox_attr.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_attr.c -c $(INCLUDE) -o ./ebox_attr.o


# Item # 3 -- ebox_basic --
./ebox_basic.o : ../../Sources/ebox/ebox_basic.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_basic.c -c $(INCLUDE) -o ./ebox_basic.o


# Item # 4 -- ebox_draw --
./ebox_draw.o : ../../Sources/ebox/ebox_draw.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_draw.c -c $(INCLUDE) -o ./ebox_draw.o


# Item # 5 -- ebox_handle --
./ebox_handle.o : ../../Sources/ebox/ebox_handle.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_handle.c -c $(INCLUDE) -o ./ebox_handle.o


# Item # 6 -- ebox_patcher --
./ebox_patcher.o : ../../Sources/ebox/ebox_patcher.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_patcher.c -c $(INCLUDE) -o ./ebox_patcher.o


# Item # 7 -- ebox_window --
./ebox_window.o : ../../Sources/ebox/ebox_window.c
	$(CC) $(CC_OPTIONS) ../../Sources/ebox/ebox_window.c -c $(INCLUDE) -o ./ebox_window.o


# Item # 8 -- eclass_attrgetter --
./eclass_attrgetter.o : ../../Sources/eclass/eclass_attrgetter.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_attrgetter.c -c $(INCLUDE) -o ./eclass_attrgetter.o


# Item # 9 -- eclass_attrnew --
./eclass_attrnew.o : ../../Sources/eclass/eclass_attrnew.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_attrnew.c -c $(INCLUDE) -o ./eclass_attrnew.o


# Item # 10 -- eclass_attroptions --
./eclass_attroptions.o : ../../Sources/eclass/eclass_attroptions.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_attroptions.c -c $(INCLUDE) -o ./eclass_attroptions.o


# Item # 11 -- eclass_attrsetter --
./eclass_attrsetter.o : ../../Sources/eclass/eclass_attrsetter.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_attrsetter.c -c $(INCLUDE) -o ./eclass_attrsetter.o


# Item # 12 -- eclass_basic --
./eclass_basic.o : ../../Sources/eclass/eclass_basic.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_basic.c -c $(INCLUDE) -o ./eclass_basic.o


# Item # 13 -- eclass_widget --
./eclass_widget.o : ../../Sources/eclass/eclass_widget.c
	$(CC) $(CC_OPTIONS) ../../Sources/eclass/eclass_widget.c -c $(INCLUDE) -o ./eclass_widget.o


# Item # 14 -- ecommon --
./ecommon.o : ../../Sources/ecommon/ecommon.c
	$(CC) $(CC_OPTIONS) ../../Sources/ecommon/ecommon.c -c $(INCLUDE) -o ./ecommon.o


# Item # 15 -- egraphics --
./egraphics.o : ../../Sources/egraphics/egraphics.c
	$(CC) $(CC_OPTIONS) ../../Sources/egraphics/egraphics.c -c $(INCLUDE) -o ./egraphics.o


# Item # 16 -- egraphics_color --
./egraphics_color.o : ../../Sources/egraphics/egraphics_color.c
	$(CC) $(CC_OPTIONS) ../../Sources/egraphics/egraphics_color.c -c $(INCLUDE) -o ./egraphics_color.o


# Item # 17 -- egraphics_forms --
./egraphics_forms.o : ../../Sources/egraphics/egraphics_forms.c
	$(CC) $(CC_OPTIONS) ../../Sources/egraphics/egraphics_forms.c -c $(INCLUDE) -o ./egraphics_forms.o


# Item # 18 -- egraphics_matrix --
./egraphics_matrix.o : ../../Sources/egraphics/egraphics_matrix.c
	$(CC) $(CC_OPTIONS) ../../Sources/egraphics/egraphics_matrix.c -c $(INCLUDE) -o ./egraphics_matrix.o


# Item # 19 -- egraphics_text --
./egraphics_text.o : ../../Sources/egraphics/egraphics_text.c
	$(CC) $(CC_OPTIONS) ../../Sources/egraphics/egraphics_text.c -c $(INCLUDE) -o ./egraphics_text.o


# Item # 20 -- eobj_basic --
./eobj_basic.o : ../../Sources/eobj/eobj_basic.c
	$(CC) $(CC_OPTIONS) ../../Sources/eobj/eobj_basic.c -c $(INCLUDE) -o ./eobj_basic.o


# Item # 21 -- eobj_dsp --
./eobj_dsp.o : ../../Sources/eobj/eobj_dsp.c
	$(CC) $(CC_OPTIONS) ../../Sources/eobj/eobj_dsp.c -c $(INCLUDE) -o ./eobj_dsp.o


# Item # 22 -- eobj_handle --
./eobj_handle.o : ../../Sources/eobj/eobj_handle.c
	$(CC) $(CC_OPTIONS) ../../Sources/eobj/eobj_handle.c -c $(INCLUDE) -o ./eobj_handle.o


# Item # 23 -- eobj_proxy --
./eobj_proxy.o : ../../Sources/eobj/eobj_proxy.c
	$(CC) $(CC_OPTIONS) ../../Sources/eobj/eobj_proxy.c -c $(INCLUDE) -o ./eobj_proxy.o


# Item # 24 -- eobj_router --
./eobj_router.o : ../../Sources/eobj/eobj_router.c
	$(CC) $(CC_OPTIONS) ../../Sources/eobj/eobj_router.c -c $(INCLUDE) -o ./eobj_router.o


# Item # 25 -- epopup --
./epopup.o : ../../Sources/epopup/epopup.c
	$(CC) $(CC_OPTIONS) ../../Sources/epopup/epopup.c -c $(INCLUDE) -o ./epopup.o


##### END RUN ####
