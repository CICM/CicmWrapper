## CICM WRAPPER
<p>Copyright (C) 2013 Pierre Guillot, CICM - Université Paris 8</p>

##### Presentation

The CICM wrapper is a C/TK library that aims to facilitate the creation of objects for Pure Data. One of the main axis is the creation of graphical user interfaces and signal processing. Pd Chocolate and Pd Coffee are externals libraries not essential but very useful.

- The inlets/outlets are automatically generated.
- The background color, border size, border color are easy to set.
- The GUI objects can be resized with click and drag.
- The graphics API use layers and "clip" the drawings within the box boundaries.
- The attributes (size, colors, values and whatever you want) are saved with the patch.
- The inlets/outlets are dynamics.
- The management of mouse motion and keys is simplified.
- The convertion from Max to Pd and from Pd to Max 6 is really easy.
and a lot of other stuff, I can't enumerate everything !

#### Author :

###### Pierre Guillot

#### To compile :

Add the sources folder to your project. Include "cicm_wrapper.h" file instead of "m_pd.h". Compile like any Pd Project.
You can use the projects to compile the externals : XCode for Mac Os, Visual Studio for Windows and Code::Block for Linux.
Projects are compiled against Pd Extended 0.43 then the externals are not compatible with older versions of Pd but are compatible
with Pd 0.45. Note that if you compile against Pd 0.45, the externals won't work with Pd 0.43. Open the c.library project to compile the Cream library.

#### ToDo :

- Update the properties window : attributes order, categories, etc. (low)
- Secure the resize i/o method. (low)
- Assit method.

#### License : GPL.


