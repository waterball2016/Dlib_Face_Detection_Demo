// Copyright (C) 2013  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_sIMD4I_Hh_
#define DLIB_sIMD4I_Hh_

#include "simd_check.h"
#include "../uintn.h"

namespace dlib
{

    class simd4i
    {
    public:
        typedef int32 type;

        inline simd4i() {}
        inline simd4i(int32 f) { x = vdupq_n_s32(f); }
        inline simd4i(int32 r0, int32 r1, int32 r2, int32 r3)
        {
            int32 __attribute__((aligned(16))) data[4] = { r0, r1, r2, r3 };
            x = vld1q_s32(data);
        }
        inline simd4i(const int32x4_t& val):x(val) {}

        inline simd4i& operator=(const int32x4_t& val)
        {
            x = val;
            return *this;
        }

        inline operator int32x4_t() const { return x; }
        inline operator uint32x4_t() const { return (uint32x4_t)x; }

        inline void load_aligned(const type* ptr)  { x = vld1q_s32(ptr); }
        inline void store_aligned(type* ptr) const { vst1q_s32(ptr, x); }
        inline void load(const type* ptr)          { x = vld1q_s32(ptr); }
        inline void store(type* ptr)         const { vst1q_s32(ptr, x); }

        inline unsigned int size() const { return 4; }
        inline int32 operator[](unsigned int idx) const
        {
            int32 temp[4];
            store(temp);
            return temp[idx];
        }

    private:
        int32x4_t x;
    };

// ----------------------------------------------------------------------------------------

    inline std::ostream& operator<<(std::ostream& out, const simd4i& item)
    {
        int32 temp[4];
        item.store(temp);
        out << "(" << temp[0] << ", " << temp[1] << ", " << temp[2] << ", " << temp[3] << ")";
        return out;
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator+ (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vaddq_s32(lhs, rhs);
    }
    inline simd4i& operator+= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs + rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator- (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vsubq_s32(lhs, rhs);
    }
    inline simd4i& operator-= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs - rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator* (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vmulq_s32(lhs, rhs);
    }
    inline simd4i& operator*= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs * rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator& (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vandq_s32(lhs, rhs);
    }
    inline simd4i& operator&= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs & rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator| (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vorrq_s32(lhs, rhs);
    }
    inline simd4i& operator|= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs | rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator^ (const simd4i& lhs, const simd4i& rhs) 
    { 
        return veorq_s32(lhs, rhs);
    }
    inline simd4i& operator^= (simd4i& lhs, const simd4i& rhs) 
    { return lhs = lhs ^ rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator~ (const simd4i& lhs) 
    { 
        return vmvnq_s32(lhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator<< (const simd4i& lhs, const int& rhs) 
    { 
        return vshlq_s32(lhs, simd4i(rhs));
    }
    inline simd4i& operator<<= (simd4i& lhs, const int& rhs) 
    { return lhs = lhs << rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator>> (const simd4i& lhs, const int& rhs) 
    { 
        int32 _lhs[4]; lhs.store(_lhs);
        return simd4i(_lhs[0]>>rhs,
                      _lhs[1]>>rhs,
                      _lhs[2]>>rhs,
                      _lhs[3]>>rhs);
    }
    inline simd4i& operator>>= (simd4i& lhs, const int& rhs) 
    { return lhs = lhs >> rhs; return lhs;}

// ----------------------------------------------------------------------------------------

    inline simd4i operator== (const simd4i& lhs, const simd4i& rhs) 
    { 
        return (int32x4_t)vceqq_s32(lhs,rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator!= (const simd4i& lhs, const simd4i& rhs) 
    { 
        return ~(lhs==rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator< (const simd4i& lhs, const simd4i& rhs) 
    { 
        return (int32x4_t)vcltq_s32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator> (const simd4i& lhs, const simd4i& rhs) 
    { 
        return rhs < lhs;
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator<= (const simd4i& lhs, const simd4i& rhs) 
    { 
        return (int32x4_t)vcleq_s32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i operator>= (const simd4i& lhs, const simd4i& rhs) 
    { 
        return rhs <= lhs;
    }

// ----------------------------------------------------------------------------------------

    inline simd4i min (const simd4i& lhs, const simd4i& rhs) 
    { 
        return (int32x4_t)vminq_s32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4i max (const simd4i& lhs, const simd4i& rhs) 
    { 
        return vmaxq_s32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline int32 sum(const simd4i& item)
    {
        int32x2_t r = vadd_s32(vget_high_s32(item), vget_low_s32(item));
        return vget_lane_s32(vpadd_s32(r, r), 0);
    }

// ----------------------------------------------------------------------------------------

    // perform cmp ? a : b
    inline simd4i select(const simd4i& cmp, const simd4i& a, const simd4i& b)
    {
        return vbslq_s32(cmp, a, b);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_sIMD4I_Hh_

