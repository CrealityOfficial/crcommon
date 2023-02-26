#ifndef CRCOMMON_HEADER_INTERFACE
#define CRCOMMON_HEADER_INTERFACE
#include "crcommon/interface.h"

#include "ccglobal/tracer.h"
#include "ccglobal/log.h"

#include "trimesh2/TriMesh.h"
#include "trimesh2/XForm.h"
#include <memory>
#include <fstream>

typedef std::shared_ptr<trimesh::TriMesh> TriMeshPtr;

#include "crcommon/Settings.h"
typedef std::shared_ptr<crcommon::Settings> SettingsPtr;

template<class T>
void templateSave(const T& t, std::ofstream& out)
{
	out.write((const char*)&t, sizeof(T));
}

template<class T>
void templateSave(const std::vector<T>& ts, std::ofstream& out)
{
	int size = (int)ts.size();
	templateSave<int>(size, out);
	out.write((const char*)ts.data(), size * sizeof(T));
}
#endif // CRCOMMON_HEADER_INTERFACE