#ifndef SLY2_API_SW_H
#define SLY2_API_SW_H

#include <sly2/basic.h>
#include <sly2/objects/lo.h>

class CSw : public CBasic {
public:
	CLo* FindObject(int grffso, int oid, CLo* ploContext);
	CLo* FindObjects(int grffso, int oid, CLo* ploContext, void* proxytable, int cploMax, CLo** aplo);

	CLo* FindObjectByClass(int grffso, int cid, CLo* ploContext);
	CLo* FindObjectsByClass(int grffso, int cid, CLo* ploContext, int cploMax, CLo** aplo);

	const char* GetTextString(i32 id);

	void PostLoad();

	void Render();
};

extern CSw g_sw;

/// Creates a new level object.
CLo* PloNew(i32 cid, CLo* ploParent, i32 oid, i32 isplice);

#endif
