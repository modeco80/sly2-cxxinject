#include <utils/log.hpp>
#include <ml/cxx/string.hpp>

#include <utils/hook/classhook.hpp>
#include <ml/cxx/fixedvec.hpp>

#include <sly2/sw.h>

CLASS_HOOK0(void, CSw, Render) {
	CLo* egg = g_sw.FindObject(0x205, OID_jt, nil(CLo*));
	if(egg != nil(CLo*)) {
		utilLogf(LogInfo, "wow it there! Holy fuck! WATER IS REAL!");
	}

	return hook_CSw_Render.original(klass);
}


extern "C" int modMain() {
	utilLogf(LogInfo, "hey lois this is like that one time that i sneaked into the new sly copper game!");

	// test dynamic heap allocations to make sure that all is working good
	{
		ml::String stringTest = "hello world?";
		utilLogf(LogInfo, "this should say something: %s %08x", stringTest.cStr(), stringTest.cStr());
	}

	if(!hook_CSw_Render.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::Render()?");
		return 1;
	}

	return 0;

}
