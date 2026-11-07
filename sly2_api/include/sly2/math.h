#ifndef SLY2MOD_SDK_XFORM_H
#define SLY2MOD_SDK_XFORM_H

#include <common/types.h>

#ifdef __cplusplus
extern "C" {
#endif

    /// A Vector4/quaternion.
    struct VECTOR {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };

    /// A 3x3 matrix.
    struct MATRIX3 {
        VECTOR v[3];
    };

    /// A transform
    struct XFORM {
        MATRIX3 mat;
        VECTOR pos;
    };

    // TODO: Add math routines.

#ifdef __cplusplus
}
#endif

#endif
