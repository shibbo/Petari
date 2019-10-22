#ifndef TMATRIX34_H
#define TMATRIX34_H

#include "types.h"
#include "JGeometry/TVec3.h"

namespace JGeometry
{
    template<typename T>
    class SMatrix34C
    {
    public:
        /* empty */
    };

    template<typename T>
    class TMatrix34
    {
    public:
        void mult(const JGeometry::TVec3<f32> &, JGeometry::TVec3<f32> &);
        void concat(const JGeometry::SMatrix34C<f32> &, const JGeometry::SMatrix34C<f32> &);
    };
};

#endif // TMATRIX34_H