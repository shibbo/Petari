#pragma once

#include <revolution.h>

#define nullptr NULL
#define _MSL_USING_NAMESPACE

#define FORCE_INLINE __inline

#define NO_INLINE _Pragma("push") _Pragma("dont_inline on")

typedef struct PTMF {
    long delta;
    long vtbl_offs;
    union {
        void *addr;
        long vtbl_entry_offs;
    }    data;
} PTMF;