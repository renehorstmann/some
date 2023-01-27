#ifndef M_PUBLICTYPES_UCVEC3_H
#define M_PUBLICTYPES_UCVEC3_H

#include <assert.h>     // for static_assert







#include "ucvec2.h"
typedef union {
    unsigned char v[3];
    struct {
        unsigned char v0, v1, v2;
    };
    m_ucvec2 xy;
    struct {
        unsigned char x;
        union {
            struct {
                unsigned char y, z;
            };
            m_ucvec2 yz;
        };
    };
    m_ucvec2 rg;
    struct {
        unsigned char r;
        union {
            struct {
                unsigned char g, b;
            };
            m_ucvec2 gb;
        };
    };
} m_ucvec3;






static_assert(sizeof(m_ucvec3) == sizeof(unsigned char) * 3, "[Mathc] wrong expected size");

/** casts a unsigned char * to a dereferenced ucvec3 */
#define M_AS_UCVEC3(vec) (*(m_ucvec3 *) (vec))

#endif //M_PUBLICTYPES_UCVEC3_H
