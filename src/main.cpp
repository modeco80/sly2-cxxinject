#include <utils/log.hpp>
#include <ml/cxx/string.hpp>

#include <utils/hook/fnhook.hpp>
#include <ml/cxx/fixedvec.hpp>

ml::FixedVec<void(*)(), 32> hookList;

extern "C" void CSw__Render(void* psw);

class ExecHookPredicate {
public:
	bool operator()(void(*pfn)()) {
		pfn();
		return true;
	}
};

FUNC_HOOK(CSw__Render, void, void* psw) {
	ExecHookPredicate execHook;
	hookList.forEach(execHook);
	return hook_CSw__Render.original(psw);
}

void testHook() {
	//utilLogf(LogInfo, "testHook() called in CSw::Render() hook");
	// TODO ld symbol
	*reinterpret_cast<float*>(0x002DE188) = 1.f;
}

extern "C" int modMain() {
	utilLogf(LogInfo, "hey lois this is like that one time that i sneaked into the new sly copper game!");

	// test dynamic heap allocations to make sure that all is working good
	{
		ml::String stringTest = "hello world?";
		utilLogf(LogInfo, "this should say something: %s %08x", stringTest.cStr(), stringTest.cStr());
	}

	if(!hook_CSw__Render.hook()) {
		utilLogf(LogErr, "Couldn't hook CSw::Render()?");
		return 1;
	}

	// test
	hookList.push(testHook);

	return 0;

}
