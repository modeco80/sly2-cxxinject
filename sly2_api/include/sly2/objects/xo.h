#ifndef SLY2MOD_SDK_OBJECTS_XO_H
#define SLY2MOD_SDK_OBJECTS_XO_H

#include <sly2/objects/lo.h>

class CXo : public CLo {
public:
	XFORM* pParentXform;
	i32 dirtyStateFlags;
	u32 unk0x58;
	u32 unk0x5c;
	XFORM transform;
	XFORM transformPublic; // returned by GetTransform override

	void LoadXfm(CBinaryInputStream* pbis);

	virtual void InitObject();

	virtual void LoadFromBrx(CBinaryInputStream* pbis);

	virtual void TranslateToPos(const VECTOR& vector);
	virtual void RotateToMat(const MATRIX3& matRotate);
	virtual XFORM* GetTransform();

	virtual void InitState(int n); // Called by LoadXfm
	virtual void MatchUnk(CXo* pxoOther, XFORM* pxform);
};

ML_STATIC_ASSERT(sizeof(CXo) == 0xe0);

#endif
