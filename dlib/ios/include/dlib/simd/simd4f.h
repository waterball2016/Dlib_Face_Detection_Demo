// Copyright (C) 2013  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_sIMD4F_Hh_
#define DLIB_sIMD4F_Hh_

#include "simd_check.h"
#include "simd4i.h"
#include <cmath>
#include <iostream>

namespace dlib
{

    class simd4f
    {
    public:
        typedef float type;

        inline simd4f() {}
        inline simd4f(float f) { x = vdupq_n_f32(f); }
        inline simd4f(float r0, float r1, float r2, float r3)
        {
            float __attribute__ ((aligned (16))) data[4] = { r0, r1, r2, r3 };
            x = vld1q_f32(data);
        }
        inline simd4f(const float32x4_t& val):x(val) {}
        inline simd4f(const simd4i& val):x(vcvtq_f32_s32(val)) {}

        inline simd4f& operator=(const simd4i& val)
        {
            x = simd4f(val);
            return *this;
        }

        inline simd4f& operator=(const float& val)
        {
            x = simd4f(val);
            return *this;
        }

        inline simd4f& operator=(const float32x4_t& val)
        {
            x = val;
            return *this;
        }

        inline operator float32x4_t() const { return x; }

        // truncate to 32bit integers
        inline operator int32x4_t() const { return vcvtq_s32_f32(x); }

        inline void load_aligned(const type* ptr)  { x = vld1q_f32(ptr); }
        inline void store_aligned(type* ptr) const { vst1q_f32(ptr, x); }
        inline void load(const type* ptr)          { x = vld1q_f32(ptr); }
        inline void store(type* ptr)         const { vst1q_f32(ptr, x); }

        inline unsigned int size() const { return 4; }
        inline float operator[](unsigned int idx) const
        {
            float temp[4];
            store(temp);
            return temp[idx];
        }

    private:
        float32x4_t x;
    };


    typedef simd4i simd4f_bool;
    
// ----------------------------------------------------------------------------------------

    inline std::ostream& operator<<(std::ostream& out, const simd4f& item)
    {
        float temp[4];
        item.store(temp);
        out << "(" << temp[0] << ", " << temp[1] << ", " << temp[2] << ", " << temp[3] << ")";
        return out;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f operator+ (const simd4f& lhs, const simd4f& rhs) 
    {   
		return vaddq_f32(lhs, rhs);
    }
    inline simd4f& operator+= (simd4f& lhs, const simd4f& rhs) 
    { lhs = lhs + rhs; return lhs; }

// ----------------------------------------------------------------------------------------

    inline simd4f operator- (const simd4f& lhs, const simd4f& rhs) 
    { 
        return vsubq_f32(lhs, rhs);
    }
    inline simd4f& operator-= (simd4f& lhs, const simd4f& rhs) 
    { lhs = lhs - rhs; return lhs; }

// ----------------------------------------------------------------------------------------

    inline simd4f operator* (const simd4f& lhs, const simd4f& rhs) 
    { 
        return vmulq_f32(lhs, rhs);
    }
    inline simd4f& operator*= (simd4f& lhs, const simd4f& rhs) 
    { lhs = lhs * rhs; return lhs; }

// ----------------------------------------------------------------------------------------

    inline simd4f operator/ (const simd4f& lhs, const simd4f& rhs) 
    { 
        float32x4_t reciprocal = vrecpeq_f32(rhs);
        reciprocal = vmulq_f32(vrecpsq_f32(rhs, reciprocal), reciprocal);
        reciprocal = vmulq_f32(vrecpsq_f32(rhs, reciprocal), reciprocal);
        float32x4_t result = vmulq_f32(lhs,reciprocal);
        return result;
    }
    inline simd4f& operator/= (simd4f& lhs, const simd4f& rhs)
    { lhs = lhs / rhs; return lhs; }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator== (const simd4f& lhs, const simd4f& rhs)
    {
        return (int32x4_t)vceqq_f32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator!= (const simd4f& lhs, const simd4f& rhs)
    {
        return ~(lhs==rhs);     // simd4f_bool is simd4i typedef, can use ~
    }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator< (const simd4f& lhs, const simd4f& rhs)
    {
        return (int32x4_t)vcltq_f32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator> (const simd4f& lhs, const simd4f& rhs) 
    { 
        return rhs < lhs;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator<= (const simd4f& lhs, const simd4f& rhs)
    {
        return (int32x4_t)vcleq_f32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4f_bool operator>= (const simd4f& lhs, const simd4f& rhs)
    {
        return rhs <= lhs;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f min (const simd4f& lhs, const simd4f& rhs)
    {
        return vminq_f32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4f max (const simd4f& lhs, const simd4f& rhs)
    {
        return vmaxq_f32(lhs, rhs);
    }

// ----------------------------------------------------------------------------------------

    inline simd4f reciprocal (const simd4f& item)
    {
        float32x4_t estimate  = vrecpeq_f32(item);
        estimate  = vmulq_f32(vrecpsq_f32(estimate , item), estimate );
        estimate  = vmulq_f32(vrecpsq_f32(estimate , item), estimate );
        return estimate ;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f reciprocal_sqrt (const simd4f& item)
    {
        float32x4_t estimate = vrsqrteq_f32(item);
        simd4f estimate2 = vmulq_f32(estimate, item);
        estimate = vmulq_f32(estimate, vrsqrtsq_f32(estimate2, estimate));
        return estimate;
    }

// ----------------------------------------------------------------------------------------

    inline float dot(const simd4f& lhs, const simd4f& rhs);
    inline float sum(const simd4f& item)
    {
        float32x2_t r = vadd_f32(vget_high_f32(item), vget_low_f32(item));
        return vget_lane_f32(vpadd_f32(r, r), 0);
    }

// ----------------------------------------------------------------------------------------

    inline float dot(const simd4f& lhs, const simd4f& rhs)
    {
#ifdef DLIB_HAVE_SSE41
        return _mm_cvtss_f32(_mm_dp_ps(lhs, rhs, 0xff));
#else
        return sum(lhs*rhs);
#endif
    }

// ----------------------------------------------------------------------------------------

    inline simd4f sqrt(const simd4f& item)
    {
        float32x4_t q_step_0 = vrsqrteq_f32(item);
        float32x4_t q_step_parm0 = vmulq_f32(item, q_step_0);
        float32x4_t q_step_result0 = vrsqrtsq_f32(q_step_parm0, q_step_0);
        float32x4_t q_step_1 = vmulq_f32(q_step_0, q_step_result0);
        float32x4_t q_step_parm1 = vmulq_f32(item, q_step_1);
        float32x4_t q_step_result1 = vrsqrtsq_f32(q_step_parm1, q_step_1);
        float32x4_t q_step_2 = vmulq_f32(q_step_1, q_step_result1);
        float32x4_t res3 = vmulq_f32(item, q_step_2);

        // normalize sqrt(0)=0
        uint32x4_t zcomp = vceqq_f32(vdupq_n_f32(0), item);
        float32x4_t rcorr = vbslq_f32(zcomp, item, res3);
        return rcorr;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f ceil(const simd4f& item)
    {
        float temp[4];
        item.store(temp);
        temp[0] = std::ceil(temp[0]);
        temp[1] = std::ceil(temp[1]);
        temp[2] = std::ceil(temp[2]);
        temp[3] = std::ceil(temp[3]);
        simd4f temp2;
        temp2.load(temp);
        return temp2;
    }

// ----------------------------------------------------------------------------------------

    inline simd4f floor(const simd4f& item)
    {
        float temp[4];
        item.store(temp);
        temp[0] = std::floor(temp[0]);
        temp[1] = std::floor(temp[1]);
        temp[2] = std::floor(temp[2]);
        temp[3] = std::floor(temp[3]);
        simd4f temp2;
        temp2.load(temp);
        return temp2;
    }

// ----------------------------------------------------------------------------------------

    // perform cmp ? a : b
    inline simd4f select(const simd4f_bool& cmp, const simd4f& a, const simd4f& b)
    {
        return vbslq_f32(cmp, a, b);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_sIMD4F_Hh_

