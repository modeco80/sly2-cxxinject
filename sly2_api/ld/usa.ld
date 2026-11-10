/* C library */
strlen = 0x0021ed28;
strcat = 0x0021eda0;
memcpy = 0x0023320c;
memset = 0x002333b4;
sprintf = 0x0021ecd8;
vsprintf = 0x0021ec60;

/* libgcc runtime stuff. it's techinically not safe to share this */
/* since it's a contract with the compiler but it works well enough */
fptodp = 0x0023e290;

/* This is where the stub is loaded **/
__HOOK_BlobCopy = 0x00c00000;



/* Important fun */

CProg__End = 0x001c8db8;
SetPhase = 0x001c2a78;

/* heap manager */
/* Heap start. Note that we are linked here, but we load (with a stub) at a different spot. */
_OriginalHeapStart = 0x00472200;
s_pvGlobalMin = 0x002f6380;
s_pvHeapMin = 0x002f6384;
g_pvHeapMax = 0x002f638c;

s_pvStackMin = 0x002f6388;
s_ipvStackCur = 0x002f639c;
s_critsectStack = 0x002f6390;

PvAllocClear__FRC8AllocReq = 0x001ae2a0;

StartupMemMgr__Fv = 0x001ae168;
CritSectInit = 0x0021f028;

/* NOTE this is a contract between the stub and the blob */
BLOB_LOAD_ADDRESS = _OriginalHeapStart;


/* CBasic */
IsDerivedFrom__6CBasicUi = 0x00132b10;

/* CLo */
SetTransform__3CLoP5XFORM = 0x001a6be8;

/* CSw */
/* global SW instance. This is a static.*/
g_sw = 0x003dd490;
FindObject__3CSwiiP3CLo = 0x0015e938;
Render__3CSw = 0x001cda80;
GetTextString__3CSwi = 0x0013e368;

PostLoad__3CSw = 0x0020e740;

PloNew__FiP3CLoii = 0x0013e3c8;

