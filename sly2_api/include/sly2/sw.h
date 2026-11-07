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

	void Render();
};

extern CSw g_sw;

#endif
