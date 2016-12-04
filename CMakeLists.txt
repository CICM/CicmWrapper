cmake_minimum_required(VERSION 3.0)
project(CicmWrapper)

message("Configuring CicmWrapper")

add_definitions(-O2 -fPIC)

include_directories(${PD_MAIN_PATH}/src)

set(PROJECT_SRCS
  Sources/ebox.c
  Sources/eclass.c
  Sources/ecommon.c
  Sources/egraphics.c
  Sources/eobj.c
  Sources/epopup.c
)

set(PROJECT_HDRS
  Sources/cicm_wrapper.h
  Sources/ebox.h
  Sources/eclass.h
  Sources/ecommon.h
  Sources/edefine.h
  Sources/egraphics.h
  Sources/enative.h
  Sources/eobj.h
  Sources/epdmax.h
  Sources/epopup.h
)

add_library(${PROJECT_NAME} ${PROJECT_SRCS})
