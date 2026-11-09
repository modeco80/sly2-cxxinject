#ifndef SLY2MOD_SDK_LO_H
#define SLY2MOD_SDK_LO_H

#include <sly2/basic.h>
#include <sly2/math.h>
#include <sly2/bis.h>
#include <sly2/objects/oid.h>

class CLo : public CBasic {
public:
    // TODO: More members so we can model inheritance graph
    // correctly.
    CLo* ploParent;
    u32 unk;
    OID oid;

    // Vtable methods are first

	virtual void InitObject();

	virtual void OnAddObject();
	virtual void OnRemoveObject();

    virtual void vf3();
    virtual void vf4();
    virtual void vf5();
    virtual void vf6();

	// TODO: Adding these
	virtual void vfPAD1(); // ?????
	virtual void vfPAD2();

    virtual void LoadFromBrx(CBinaryInputStream* pbis);
    virtual void PostLoad();

    virtual void Update();

    virtual void vf7();
    virtual void vf8();
    virtual void vf9();
    virtual void vf10();
    virtual void vf11();

    virtual void vf12(); // 0xb4
    virtual void vf13(); // 0xbc

    virtual void vf14();

    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();


    // Transform-related methods. On CLo, these are stubs (for instance, GetTransform() always
    // returns a static identity XFORM instance on CLo).
    // On CXo/CSo (base classes for objects with Transform or Physics),
    // these will actually do what they say

    virtual void TranslateToPos(const VECTOR& vector);
    virtual void RotateToMat(const MATRIX3& matRotate);
    virtual XFORM* GetTransform();


    virtual void vf19();
    virtual void vf20();
    virtual void vf21();
    virtual void vf22();
    virtual void vf23();
    virtual void vf24();

    /// Matches another object's xform exactly. CLo base implementation
    /// simply copies the transform of the other object using CLo virtual functions.
    virtual void MatchObject(CLo* ploMatch);

    virtual void vf25();

    /// Not sure what to call these. CloneHelper seems to actually do the,
    /// well, cloning, process, though. The base CLo implementation simply
    /// memcpy's the class directly. Kind of a hack, but, to be fair,
    /// so is their whole heap!
    virtual void Clone(CLo* ploClone);
    virtual void CloneHelper(CLo* ploClone);

    virtual void vf26();
    virtual void vf27();
    virtual void vf28();
    virtual void vf29();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();

};

#endif
