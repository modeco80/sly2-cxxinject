#include <utils/log.hpp>
#include <ml/cxx/string.hpp>

#include <utils/hook/classhook.hpp>
#include <ml/cxx/fixedvec.hpp>

#include <sly2/sw.h>

// Declare a hook for CSw::Render(). Note that declaring a hook
// does not immediately hook the function. You need to do that inside of
// modMain() or a function called by modMain() or a hook.
CLASS_HOOK0(void, CSw, Render) {
	CLo* egg = g_sw.FindObject(0x205, OID_jt, nil(CLo*));
	if(egg != nil(CLo*)) {
		utilLogf(LogInfo, "wow it there! Holy fuck! WATER IS REAL!");
	}

	return hook_CSw_Render.original(klass);
}


extern "C" int modMain() {
	utilLogf(LogInfo, "C++ hook initalized");

	// Hook CSw::Render function.
	if(!hook_CSw_Render.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::Render()?");
		return 1;
	}

	return 0;

}
