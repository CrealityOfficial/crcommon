__cc_find(polyclipping)

set(LIBS)
set(ILIBS trimesh2 polyclipping)

set(SRCS crcommon/interface.h
		 crcommon/Settings.h
		 crcommon/Settings.cpp
		 )
		 
set(INCS)
set(DEFS)
set(INTERFACES)

__add_common_library(crcommon)
								