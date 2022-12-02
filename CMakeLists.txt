__cc_find(polyclipping)
__cc_find(rapidjson)

set(LIBS rapidjson)
set(ILIBS trimesh2 polyclipping)

set(SRCS crcommon/interface.h
		 crcommon/header.h
		 crcommon/Settings.h
		 crcommon/Settings.cpp
		 crcommon/jsonloader.h
		 crcommon/jsonloader.cpp
		 )
		 
set(INCS)
set(DEFS)
set(INTERFACES)

__add_common_library(crcommon)
								