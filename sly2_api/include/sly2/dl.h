#ifndef SLY2MOD_SDK_DL_H
#define SLY2MOD_SDK_DL_H

#include <sly2/types.h>

struct DLE {
    void* pvPrev;
    void* pvNext;
};

struct DL {
    void* pvFirst;
    void* pvLast;

    // Offset in structure to where DLE field is.
    i32 ibDle;
};

// Initializes a [DL] structure. [ibDle] is expected to be the offset
// in the struct where a [DLE] struct is placed.
void InitDl(DL* pdl, i32 ibDle);

void ClearDl(DL* pdl);
void ClearDle(DLE* pdle);

// Insertion/removal from lists
void AppendDl(DL* pdl, void* pvItem);
void PrependDlEntry(DL* pdl, void* pvItem);
void InsertDlEntryBefore(DL* pdl, void* pvNext, void* pvItem);
void RemoveDlEntry(DL* pdl, void* pvItem);

BOOL FIsDlEmpty(DL* pdl);

// TODO: CDl if needed
#endif
