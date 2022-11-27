#ifndef CRCOMMON_HEADER_INTERFACE
#define CRCOMMON_HEADER_INTERFACE
#include "crcommon/interface.h"

#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"
#include <memory>

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;

#include "crcommon/Settings.h"
typedef std::shared_ptr<crcommon::Settings> SettingsPtr;

#endif // CRCOMMON_HEADER_INTERFACE