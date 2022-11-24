#ifndef CRCOMMON_INTERFACE
#define CRCOMMON_INTERFACE
#include "ccglobal/export.h"

#if USE_CRCOMMON_DLL
	#define CRCOMMON_API CC_DECLARE_IMPORT
#elif USE_CRCOMMON_STATIC
	#define CRCOMMON_API CC_DECLARE_STATIC
#else
	#if CRCOMMON_DLL
		#define CRCOMMON_API CC_DECLARE_EXPORT
	#else
		#define CRCOMMON_API CC_DECLARE_STATIC
	#endif
#endif


#include "trimesh2/TriMesh.h"
#include <memory>

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;
#endif // CRCOMMON_INTERFACE