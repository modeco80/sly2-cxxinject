#include <utils/log.hpp>
#include <ml/cxx/string.hpp>

#include <utils/hook/classhook.hpp>

#include <sly2/sw.h>

// helpers
#define mlPeek(object, offset, T) \
*reinterpret_cast<const T*>(reinterpret_cast<const u8*>(&object) + offset)
#define mlPoke(object, offset, T, value) \
	*reinterpret_cast<T*>(reinterpret_cast<u8*>(&object) + offset) = value

void hexdump(const void *mem, size_t len, unsigned int prefix, bool printChar);

CLo* pxoTestObject;

// Declare a hook for CSw::Render(). Note that declaring a hook
// does not immediately hook the function. You need to do that inside of
// modMain() or a function called by modMain() or a hook.
CLASS_HOOK0(void, CSw, Render) {
	return hook_CSw_Render.original(klass);
}


CLASS_HOOK0(void, CSw, PostLoad) {

	utilLogf(LogInfo, "PostLoad of WID %d",
		mlPeek(g_sw, 0x3c80, u32)
	);

	if(mlPeek(g_sw, 0x3c80, u32) == -1) {
		return hook_CSw_PostLoad.original(klass);
	}

	pxoTestObject = PloNew(3 /*CID_XO*/, nil(CLo*), -1, -1);

	if(pxoTestObject) {
		BasicTypeInfo* pTypeInfo = pxoTestObject->GetTypeInfo();
		utilLogf(LogInfo, "made xo successfully, cid %d, ptr 0x%08x", pTypeInfo->cidThis, pxoTestObject);:

		hexdump(pxoTestObject, 0x50 /* TODO sizeof(CLo) */, 0, true);
	}

	// move test xo
	CLo* pjt = g_sw.FindObject(0x205, OID_jt, nil(CLo*));
	if(pjt != nil(CLo*)) {
		pxoTestObject->MatchObject(pjt);
		XFORM* xfm = pxoTestObject->GetTransform();
		utilLogf(LogInfo, "test xo is now at (%0.3f, %0.3f, %0.3f)", xfm->pos.x, xfm->pos.y, xfm->pos.z);
	}



	return hook_CSw_PostLoad.original(klass);
}

extern "C" int modMain() {
	utilLogf(LogInfo, "C++ hook initalized");

	// Hook functions.

	if(!hook_CSw_Render.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::Render()");
	}

	if(!hook_CSw_PostLoad.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::PostLoad()");
	}

	return 0;

}
