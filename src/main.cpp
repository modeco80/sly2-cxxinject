#include <utils/log.hpp>
#include <ml/cxx/string.hpp>

#include <utils/hook/classhook.hpp>

#include <sly2/sw.h>
#include <sly2/objects/xo.h>

// helpers to peek and poke structures
#define mlPeek(object, offset, T) \
	*reinterpret_cast<const T*>(reinterpret_cast<const u8*>(object) + offset)
#define mlPoke(object, offset, T, value) \
	*reinterpret_cast<T*>(reinterpret_cast<u8*>(object) + offset) = value

void hexdump(const void *mem, size_t len, unsigned int prefix, bool printChar);

CLo* pTestSpire;
bool bFound = false;
#define TEST_OID 69420

// Declare a hook for CSw::Render(). Note that declaring a hook
// does not immediately hook the function. You need to do that inside of
// modMain() or a function called by modMain() or a hook.
CLASS_HOOK0(void, CSw, Render) {
#if 1
	if(!bFound) {
		CLo* pFoundSpire = g_sw.FindObject(0x405, TEST_OID, nil(CLo*));
		if(pFoundSpire != nil(CLo*)
			&& pFoundSpire == pTestSpire) {
			bFound = true;
			utilLogf(LogInfo, "Found test spire with oid %d", TEST_OID);
			hexdump(pFoundSpire, 0x50 /* TODO sizeof(CLo) */, 0, true);

		}
	}
#endif

	return hook_CSw_Render.original(klass);
}


CLASS_HOOK0(void, CSw, PostLoad) {
	if(mlPeek(&g_sw, 0x3c80, u32) == -1) {
		return hook_CSw_PostLoad.original(klass);
	}

	utilLogf(LogInfo, "PostLoad of WID %d",
			  mlPeek(&g_sw, 0x3c80, u32)
	);


	bFound = false;
	pTestSpire = PloNew(CID_SPIRE, nil(CLo*), TEST_OID, -1);

	if(pTestSpire) {
		BasicTypeInfo* pTypeInfo = pTestSpire->GetTypeInfo();
		utilLogf(LogInfo, "made test object successfully, cid %d, ptr 0x%08x", pTypeInfo->cidThis, pTestSpire);
		hexdump(pTestSpire, 0x50 /* TODO sizeof(CLo) */, 0, true);
	}

	// move test xo roughly to a decent spot
	CXo* pjt = (CXo*)g_sw.FindObject(0x205, OID_jt, nil(CLo*));
	if(pjt != nil(CXo*)) {
		XFORM* xfm = pjt->GetTransform();
		XFORM xfmNudgeDown = *xfm;
		xfmNudgeDown.pos.z -= 95.f;


		pTestSpire->SetTransform(&xfmNudgeDown);

		pjt->MatchObject(pTestSpire);

		//pxoTestObject->MatchObject(pjt);
		//XFORM* xfm = pxoTestObject->GetTransform();
		//XFORM xfmNudgeDown = *xfm;
		//xfmNudgeDown.pos.y -= 1000;

		//testcode
		xfm = pTestSpire->GetTransform();
		utilLogf(LogInfo, "test object is now at (%0.3f, %0.3f, %0.3f)", xfm->pos.x, xfm->pos.y, xfm->pos.z);
	}



	return hook_CSw_PostLoad.original(klass);
}

extern "C" int modMain() {
	utilLogf(LogInfo, "C++ hook initalized %d", sizeof(CLo));

	// Hook functions.

	if(!hook_CSw_Render.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::Render()");
	}

	if(!hook_CSw_PostLoad.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::PostLoad()");
	}

	return 0;

}
