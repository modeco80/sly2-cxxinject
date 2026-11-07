#ifndef SLY2MOD_SDK_BASIC_H
#define SLY2MOD_SDK_BASIC_H

#include <sly2/types.h>

// TODO:
// - Define CID
//  - Add CID_USER for mod LOs
// - Splice: define SYMEVID for CBasic::HandleSpliceEvent

// Splice forward-decls.
class CFrame;
class CRef;

class CBasic;
struct EOPID; // TODO
class CBinaryInputStream;

struct BasicTypeInfo {
	// Superclass pointer
	BasicTypeInfo* ptiSuper;
	i32 cidThis;
	i32 cidNextBase; // Next baseclass
	u32 size;
	// Creates a new instance of the object.
	CBasic* (*pfnCreate)();
};


class CBasic {
	// The splice module frame for this object.
	CFrame* pframe;
	u32 unk1;
	u32 unk2;
public:
	// TODO introduce more non-virtuals.

	// Anything called vf* is not known yet. Parameters/const-ness may also be incorrect.

	virtual void vf1();
	virtual void vf2();
	virtual void HandleSpliceEvent(i32 symidEvent, BOOL fImmediate, int cargs, void** ppvargs, CRef* refRet);
	virtual void vf3();
	virtual void vf4();
	virtual BasicTypeInfo* GetTypeInfo();


	// Non-virtuals
	void FireTriggerEvent();

	// Brx options.
	void LoadOptionFromBrx(EOPID* peopid, u32 grfeopid, CBinaryInputStream* pbis);
	void LoadOptionsFromBrx(CBinaryInputStream* pbis);

	// Gets the CID of this Basic.
	u32 GetCid();

	// Sets new Splice module frame.
	void SetFrame(CFrame* pframe);

	bool IsDerivedFrom(u32 cidOther);
};

ML_STATIC_ASSERT(sizeof(CBasic) == 0x10);

#endif
