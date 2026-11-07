from mkpatch.patch import PatchStep, PatchContext
from mkpatch.utils import alignUp

## Hook StartupMemMgr() to jump to the start of the stub.
class HookStartup(PatchStep):
	def runStep(self, context: PatchContext):
		context.patchMakeStaticThunk(context.ldScript.symbol('StartupMemMgr__Fv'), context.patchBinary.symbol('_start'))
		# Only called at the end of Startup().
		#context.patchMakeStaticThunk(context.ldScript.symbol('CProg__End'), context.patchBinary.symbol('_start'))
