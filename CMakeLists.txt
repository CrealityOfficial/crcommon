__enable_cxx17()
__cc_find(trimesh2)
__cc_find(rapidjson)
__cc_find(boost)

set(LIBS rapidjson)
set(ILIBS trimesh2 boost_filesystem boost_nowide)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRCS crcommon/interface.h
		 crcommon/header.h
		 crcommon/Settings.h
		 crcommon/Settings.cpp
		 crcommon/jsonloader.h
		 crcommon/jsonloader.cpp
		 crcommon/jsonhelper.h
		 crcommon/jsonhelper.cpp
		 crcommon/base/parametermeta.h
		 crcommon/base/parametermeta.cpp
		 )
		 
set(INCS)
set(DEFS BOOST_ALL_NO_LIB BOOST_ALL_DYN_LINK)
set(INTERFACES)

__add_common_library(crcommon)
__set_folder_targets(slice TARGET crcommon)
								