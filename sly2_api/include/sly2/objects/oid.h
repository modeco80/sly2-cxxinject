#ifndef SLY2MOD_SDK_OBJECTS_OID_H
#define SLY2MOD_SDK_OBJECTS_OID_H

#include <sly2/types.h>

enum OID {
    OID_Nil = -1,
    OID_Unknown = 0,

    // TODO: Name OIDs.
    OID_jt = 7
};

enum CID {
	CID_BASIC = 0, // CBasic
	CID_SW = 1, // CSw
	CID_LO = 2, // CLo
	CID_XO = 3, // CXo


	CID_SPIRE = 39,
};

#endif
