#ifndef UTILS_HOOK_FNHOOK_HPP
#define UTILS_HOOK_FNHOOK_HPP

#include <utils/hook/trampoline.hpp>

/// Declares a hook which can trampoline back to its original function,
/// and does not take any arguments.
#define FUNC_HOOK0(ret, funcName)                                                             \
	static class Hook_##funcName {                                                            \
		HookHandle handle;                                                                    \
		ret (*original)();                                                                    \
		static ret hookImpl();                                                                \
                                                                                              \
	   public:                                                                                \
		bool hook() {                                                                         \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                                 \
		}                                                                                     \
	} hook_##funcName;                                                                        \
	ret Hook_##funcName::hookImpl()

/// Declares a hook which can trampoline back to its original function,
/// and does takes any amount of arguments.
#define FUNC_HOOK(ret, funcName, ...)                                                         \
	static class Hook_##funcName {                                                            \
		HookHandle handle;                                                                    \
		ret (*original)(__VA_ARGS__);                                                         \
		static ret hookImpl(__VA_ARGS__);                                                     \
                                                                                              \
	   public:                                                                                \
		bool hook() {                                                                         \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, (void**)&original); \
			return handle != nil(HookHandle);                                                 \
		}                                                                                     \
	} hook_##funcName;                                                                        \
	ret Hook_##funcName::hookImpl(__VA_ARGS__)

/// Declares a hook which does not execute its original function, does not
/// save a trampoline, and takes no arguments.
#define FUNC_REPLACE0(ret, funcName)                                                    \
	static class Hook_##funcName {                                                      \
		HookHandle handle;                                                              \
		static ret hookImpl();                                                          \
                                                                                        \
	   public:                                                                          \
		bool hook() {                                                                   \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, nil(void**)); \
			return handle != nil(HookHandle);                                           \
		}                                                                               \
	} hook_##funcName;                                                                  \
	ret Hook_##funcName::hookImpl()

/// Declares a hook which does not execute its original function, does not
/// save a trampoline, and takes any amount of arguments.
#define FUNC_REPLACE(ret, funcName, ...)                                                \
	static class Hook_##funcName {                                                      \
		HookHandle handle;                                                              \
		static ret hookImpl(__VA_ARGS__);                                               \
                                                                                        \
	   public:                                                                          \
		bool hook() {                                                                   \
			handle = trampolineHook((void*)(&funcName), (void*)&hookImpl, nil(void**)); \
			return handle != nil(HookHandle);                                           \
		}                                                                               \
	} hook_##funcName;                                                                  \
	ret Hook_##funcName::hookImpl(__VA_ARGS__)

#endif
