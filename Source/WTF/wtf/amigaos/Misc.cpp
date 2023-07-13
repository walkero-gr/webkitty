#include "config.h"
#include <exec/types.h>
#include <stdlib.h>
#include <sys/syslimits.h>
#include <proto/dos.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include "Platform.h"

extern "C" void Fail(unsigned char *)
{
	// Libjpeg fail message...
	// TODO
}

extern "C" char *realpath(const char *file_name, char *resolved_name)
{
	BPTR l = IDOS->Lock(file_name, SHARED_LOCK);
	if (l)
	{
		if (IDOS->NameFromLock(l, resolved_name, PATH_MAX))
		{
			IDOS->UnLock(l);
			return resolved_name;
		}
		IDOS->UnLock(l);
	}

	return nullptr;
}

namespace WTF {

bool HasAltivec::m_hasAltivec;

HasAltivec::HasAltivec()
{
	LONG altivec = 0;
	IExec->GetCPUInfoTags(GCIT_VectorUnit, &altivec, TAG_DONE);
	// TODO: Disabled altivec here
	m_hasAltivec = altivec == 0;
}

bool HasAltivec::hasAltivec()
{
	static HasAltivec __hs;
	return m_hasAltivec;
}

}
