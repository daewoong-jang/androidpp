/*
 * Copyright (C) 2017 Daewoong Jang.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "Matrix.h"

#include <android++/LogHelper.h>

static const float pi = 3.14159265358979323846;
static const float deg2rad = pi / 180.0f;

namespace android {
namespace opengl {

struct Float3 {
    float x, y, z;

    static Float3& emplace(float* arr, int32_t offset = 0)
    {
        return *new (arr + offset) Float3();
    }

    Float3() { }
    Float3(float x, float y, float z)
        : x(x), y(y), z(z)
    {
    }

    static float length(float x, float y, float z)
    {
        return sqrt(x * x + y * y + z * z);
    }

    float length()
    {
        return length(x, y, z);
    }

    static void normalize(float& x, float& y, float& z)
    {
        const float r = 1.0f / length(x, y, z);
        x *= r;
        y *= r;
        z *= r;
    }

    void normalize()
    {
        normalize(x, y, z);
    }

    float& operator[](int32_t i)
    {
        return (&x)[i];
    }

    void operator()(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Float3& operator=(const Float3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    Float3& operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
};

inline Float3 operator*(const Float3& f, float s)
{
    Float3 r;
    r.x = f.x * s;
    r.y = f.y * s;
    r.z = f.z * s;
    return r;
}

inline float operator*(const Float3& u, const Float3& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

inline Float3 operator%(const Float3& u, const Float3& v)
{
    Float3 r;
    r.x = u.y * v.z - u.z * v.y;
    r.y = u.z * v.x - u.x * v.z;
    r.z = u.x * v.y - u.y * v.x;
    return r;
}

struct Float4 : public Float3 {
    float w;

    static Float4& emplace(float* arr, int32_t offset = 0)
    {
        return *new (arr + offset) Float4();
    }

    Float4() { }
    Float4(float x, float y, float z, float w)
        : Float3(x, y, z), w(w)
    {
    }

    Float3& xyz() { return *this; }
    const Float3& xyz() const { return *this; }

    void operator()(float x, float y, float z, float w)
    {
        Float3::operator()(x, y, z);
        this->w = w;
    }

    Float4& operator=(const Float4& other)
    {
        Float3::operator=(other);
        w = other.w;
        return *this;
    }

    Float4& operator*=(float s)
    {
        Float3::operator*=(s);
        w *= s;
        return *this;
    }
};

inline Float4 operator*(const Float4& f, float s)
{
    Float4 r;
    r.x = f.x * s;
    r.y = f.y * s;
    r.z = f.z * s;
    r.w = f.w * s;
    return r;
}

inline float operator*(const Float4& u, const Float4& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
}

template<int32_t Stride>
struct Component3 {
    float& a;
    float& b;
    float& c;

    Component3(float &a)
        : a(a), b(*(&a + Stride)), c(*(&a + Stride * 2))
    {
    }

    float& operator[](int32_t i)
    {
        return (&a)[i * Stride];
    }

    void operator()(float a, float b, float c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
    }

    Component3& operator=(const Component3& other)
    {
        a = other.a;
        b = other.b;
        c = other.c;
        return *this;
    }

    Component3& operator*=(float s)
    {
        a *= s;
        b *= s;
        c *= s;
        return *this;
    }
};

template<int32_t Stride>
inline float operator*(const Component3<Stride>& u, const Float3& v)
{
    return u.a * v.x + u.b * v.y + u.c * v.z;
}

template<int32_t Stride>
struct Component4 : public Component3<Stride> {
    using BaseComponents = Component3<Stride>;
    float& d;

    Component4(float &a)
        : BaseComponents(a)
        , d(*(&a + Stride * 3))
    {
    }

    BaseComponents& abc() { return *this; }
    const BaseComponents& abc() const { return *this; }

    void operator()(float a, float b, float c, float d)
    {
        BaseComponents::operator()(a, b, c);
        this->d = d;
    }

    Component4& operator=(const Component4& other)
    {
        BaseComponents::operator=(other);
        d = other.d;
        return *this;
    }

    Component4& operator*=(float s)
    {
        BaseComponents::operator*=(s);
        d *= s;
        return *this;
    }
};

template<int32_t Stride>
inline float operator*(const Component4<Stride>& u, const Float4& v)
{
    return u.a * v.x + u.b * v.y + u.c * v.z + u.d * v.w;
}

template<int32_t Stride>
inline float operator*(const Component4<Stride>& u, const Float3& v)
{
    return u.abc() * v;
}

struct Matrix4 {
public:
    using Components = Component4<4>;

    Float4 f[4];

    static Matrix4& emplace(float* arr, int32_t offset = 0)
    {
        return *new (arr + offset) Matrix4();
    }

    Matrix4() { }

    Float4& operator[](int32_t i)
    {
        return f[i];
    }

    Components column(int32_t i)
    {
        return Components(f[0][i]);
    }
};

void Matrix::frustumM(float* m, int32_t offset, float left, float right, float bottom, float top, float near, float far)
{
    assert_wtf(left != right);
    assert_wtf(top != bottom);
    assert_wtf(near != far);
    assert_wtf(near > 0.0f);
    assert_wtf(far > 0.0f);

    Matrix4& mat = Matrix4::emplace(m, offset);
    const float rw = 1.0f / (right - left);
    const float rh = 1.0f / (top - bottom);
    const float rd = 1.0f / (near - far);
    mat[0](2.0f * (near * rw), 0.0f, 0.0f, 0.0f);
    mat[1](0.0f, 2.0f * (near * rh), 0.0f, 0.0f);
    mat[2]((right + left) * rw, (top + bottom) * rh, (far + near) * rd, -1.0f);
    mat[3](0.0f, 0.0f, 2.0f * (far * near * rd), 0.0f);
}

bool Matrix::invertM(float* mInv, int32_t mInvOffset, float* m, int32_t mOffset)
{
    Matrix4& inv = Matrix4::emplace(mInv, mInvOffset);
    Matrix4& mat = Matrix4::emplace(m, mOffset);

    inv[0](mat[1][1] * mat[2][2] * mat[3][3] - mat[1][1] * mat[2][3] * mat[3][2] -
        mat[2][1] * mat[1][2] * mat[3][3] + mat[2][1] * mat[1][3] * mat[3][2] +
        mat[3][1] * mat[1][2] * mat[2][3] - mat[3][1] * mat[1][3] * mat[2][2],

        -mat[0][1] * mat[2][2] * mat[3][3] + mat[0][1] * mat[2][3] * mat[3][2] +
        mat[2][1] * mat[0][2] * mat[3][3] - mat[2][1] * mat[0][3] * mat[3][2] -
        mat[3][1] * mat[0][2] * mat[2][3] + mat[3][1] * mat[0][3] * mat[2][2],

        mat[0][1] * mat[1][2] * mat[3][3] - mat[0][1] * mat[1][3] * mat[3][2] -
        mat[1][1] * mat[0][2] * mat[3][3] + mat[1][1] * mat[0][3] * mat[3][2] +
        mat[3][1] * mat[0][2] * mat[1][3] - mat[3][1] * mat[0][3] * mat[1][2],

        -mat[0][1] * mat[1][2] * mat[2][3] + mat[0][1] * mat[1][3] * mat[2][2] +
        mat[1][1] * mat[0][2] * mat[2][3] - mat[1][1] * mat[0][3] * mat[2][2] -
        mat[2][1] * mat[0][2] * mat[1][3] + mat[2][1] * mat[0][3] * mat[1][2]);

    inv[1](-mat[1][0] * mat[2][2] * mat[3][3] + mat[1][0] * mat[2][3] * mat[3][2] +
        mat[2][0] * mat[1][2] * mat[3][3] - mat[2][0] * mat[1][3] * mat[3][2] -
        mat[3][0] * mat[1][2] * mat[2][3] + mat[3][0] * mat[1][3] * mat[2][2],

        mat[0][0] * mat[2][2] * mat[3][3] - mat[0][0] * mat[2][3] * mat[3][2] -
        mat[2][0] * mat[0][2] * mat[3][3] + mat[2][0] * mat[0][3] * mat[3][2] +
        mat[3][0] * mat[0][2] * mat[2][3] - mat[3][0] * mat[0][3] * mat[2][2],

        -mat[0][0] * mat[1][2] * mat[3][3] + mat[0][0] * mat[1][3] * mat[3][2] +
        mat[1][0] * mat[0][2] * mat[3][3] - mat[1][0] * mat[0][3] * mat[3][2] -
        mat[3][0] * mat[0][2] * mat[1][3] + mat[3][0] * mat[0][3] * mat[1][2],

        mat[0][0] * mat[1][2] * mat[2][3] - mat[0][0] * mat[1][3] * mat[2][2] -
        mat[1][0] * mat[0][2] * mat[2][3] + mat[1][0] * mat[0][3] * mat[2][2] +
        mat[2][0] * mat[0][2] * mat[1][3] - mat[2][0] * mat[0][3] * mat[1][2]);

    inv[2](mat[1][0] * mat[2][1] * mat[3][3] - mat[1][0] * mat[2][3] * mat[3][1] -
        mat[2][0] * mat[1][1] * mat[3][3] + mat[2][0] * mat[1][3] * mat[3][1] +
        mat[3][0] * mat[1][1] * mat[2][3] - mat[3][0] * mat[1][3] * mat[2][1],

        -mat[0][0] * mat[2][1] * mat[3][3] + mat[0][0] * mat[2][3] * mat[3][1] +
        mat[2][0] * mat[0][1] * mat[3][3] - mat[2][0] * mat[0][3] * mat[3][1] -
        mat[3][0] * mat[0][1] * mat[2][3] + mat[3][0] * mat[0][3] * mat[2][1],

        mat[0][0] * mat[1][1] * mat[3][3] - mat[0][0] * mat[1][3] * mat[3][1] -
        mat[1][0] * mat[0][1] * mat[3][3] + mat[1][0] * mat[0][3] * mat[3][1] +
        mat[3][0] * mat[0][1] * mat[1][3] - mat[3][0] * mat[0][3] * mat[1][1],

        -mat[0][0] * mat[1][1] * mat[2][3] + mat[0][0] * mat[1][3] * mat[2][1] +
        mat[1][0] * mat[0][1] * mat[2][3] - mat[1][0] * mat[0][3] * mat[2][1] -
        mat[2][0] * mat[0][1] * mat[1][3] + mat[2][0] * mat[0][3] * mat[1][1]);

    inv[3](-mat[1][0] * mat[2][1] * mat[3][2] + mat[1][0] * mat[2][2] * mat[3][1] +
        mat[2][0] * mat[1][1] * mat[3][2] - mat[2][0] * mat[1][2] * mat[3][1] -
        mat[3][0] * mat[1][1] * mat[2][2] + mat[3][0] * mat[1][2] * mat[2][1],

        mat[0][0] * mat[2][1] * mat[3][2] - mat[0][0] * mat[2][2] * mat[3][1] -
        mat[2][0] * mat[0][1] * mat[3][2] + mat[2][0] * mat[0][2] * mat[3][1] +
        mat[3][0] * mat[0][1] * mat[2][2] - mat[3][0] * mat[0][2] * mat[2][1],

        -mat[0][0] * mat[1][1] * mat[3][2] + mat[0][0] * mat[1][2] * mat[3][1] +
        mat[1][0] * mat[0][1] * mat[3][2] - mat[1][0] * mat[0][2] * mat[3][1] -
        mat[3][0] * mat[0][1] * mat[1][2] + mat[3][0] * mat[0][2] * mat[1][1],

        mat[0][0] * mat[1][1] * mat[2][2] - mat[0][0] * mat[1][2] * mat[2][1] -
        mat[1][0] * mat[0][1] * mat[2][2] + mat[1][0] * mat[0][2] * mat[2][1] +
        mat[2][0] * mat[0][1] * mat[1][2] - mat[2][0] * mat[0][2] * mat[1][1]);

    float det = mat[0][0] * inv[0][0] + mat[0][1] * inv[1][0] + mat[0][2] * inv[2][0] + mat[0][3] * inv[3][0];

    if (det == 0.0f)
        return false;

    det = 1.0 / det;

    inv[0] *= det;
    inv[1] *= det;
    inv[2] *= det;
    inv[3] *= det;

    return true;
}

float Matrix::length(float x, float y, float z)
{
    return sqrt(x * x + y * y + z * z);
}

void Matrix::multiplyMM(float* result, int32_t resultOffset, float* lhs, int32_t lhsOffset, float* rhs, int32_t rhsOffset)
{
    Matrix4& mat = Matrix4::emplace(result, resultOffset);
    Matrix4& ltm = Matrix4::emplace(lhs, lhsOffset);
    Matrix4& rtm = Matrix4::emplace(rhs, rhsOffset);
    Matrix4::Components col[4] = { ltm.column(0), ltm.column(1), ltm.column(2), ltm.column(3) };
    mat[0](col[0] * rtm[0], col[1] * rtm[0], col[2] * rtm[0], col[3] * rtm[0]);
    mat[1](col[0] * rtm[1], col[1] * rtm[1], col[2] * rtm[1], col[3] * rtm[1]);
    mat[2](col[0] * rtm[2], col[1] * rtm[2], col[2] * rtm[2], col[3] * rtm[2]);
    mat[3](col[0] * rtm[3], col[1] * rtm[3], col[2] * rtm[3], col[3] * rtm[3]);
}

void Matrix::multiplyMV(float* resultVec, int32_t resultVecOffset, float* lhsMat, int32_t lhsMatOffset, float* rhsVec, int32_t rhsVecOffset)
{
    Float4& vec = Float4::emplace(resultVec, resultVecOffset);
    Matrix4& ltm = Matrix4::emplace(lhsMat, lhsMatOffset);
    Float4& rtv = Float4::emplace(rhsVec, rhsVecOffset);
    vec(ltm.column(0) * rtv, ltm.column(1) * rtv, ltm.column(2) * rtv, ltm.column(3) * rtv);
}

void Matrix::orthoM(float* m, int32_t mOffset, float left, float right, float bottom, float top, float near, float far)
{
    assert_wtf(left != right);
    assert_wtf(top != bottom);
    assert_wtf(near != far);

    Matrix4& mat = Matrix4::emplace(m, mOffset);
    const float rw = 1.0f / (right - left);
    const float rh = 1.0f / (top - bottom);
    const float rd = 1.0f / (far - near);
    mat[0](2.0f * (rw), 0.0f, 0.0f, 0.0f);
    mat[1](0.0f, 2.0f * (rh), 0.0f, 0.0f);
    mat[2](0.0f, 0.0f, -2.0f * (rd), 0.0f);
    mat[3](-(right + left) * rw, -(top + bottom) * rh, -(far + near) * rd, 1.0f);
}

void Matrix::perspectiveM(float* m, int32_t offset, float fovy, float aspect, float zNear, float zFar)
{
    const float f = 1.0f / tanf(fovy * (pi / 360.0f));
    const float rd = 1.0f / (zNear - zFar);

    Matrix4& mat = Matrix4::emplace(m, offset);
    mat[0](f / aspect, 0.0f, 0.0f, 0.0f);
    mat[1](0.0f, f, 0.0f, 0.0f);
    mat[2](0.0f, 0.0f, (zFar + zNear) * rd, -1.0f);
    mat[3](0.0f, 0.0f, 2.0f * zFar * zNear * rd, 0.0f);
}

void Matrix::rotateM(float* rm, int32_t rmOffset, float* m, int32_t mOffset, float a, float x, float y, float z)
{
    float temp[32];
    setRotateM(temp, 0, a, x, y, z);
    multiplyMM(rm, rmOffset, m, mOffset, temp, 0);
}

void Matrix::rotateM(float* m, int32_t mOffset, float a, float x, float y, float z)
{
    float temp[32];
    setRotateM(temp, 0, a, x, y, z);
    multiplyMM(temp, 16, m, mOffset, temp, 0);
    memcpy(m + mOffset, temp + 16, sizeof(float) * 16);
}

void Matrix::scaleM(float* m, int32_t mOffset, float x, float y, float z)
{
    Matrix4& mat = Matrix4::emplace(m, mOffset);
    mat[0] *= x;
    mat[1] *= y;
    mat[2] *= z;
}

void Matrix::scaleM(float* sm, int32_t smOffset, float* m, int32_t mOffset, float x, float y, float z)
{
    Matrix4& src = Matrix4::emplace(m, mOffset);
    Matrix4& mat = Matrix4::emplace(sm, smOffset);
    mat[0] = src[0] * x;
    mat[1] = src[1] * y;
    mat[2] = src[2] * z;
    mat[3] = src[3];
}

void Matrix::setIdentityM(float* sm, int32_t smOffset)
{
    Matrix4& mat = Matrix4::emplace(sm, smOffset);
    mat[0](1.0f, 0.0f, 0.0f, 0.0f);
    mat[1](0.0f, 1.0f, 0.0f, 0.0f);
    mat[2](0.0f, 0.0f, 1.0f, 0.0f);
    mat[3](0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix::setLookAtM(float* rm, int32_t rmOffset, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ)
{
    Float3 f(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
    f.normalize();

    Float3 up(upX, upY, upZ);
    Float3 s = f % up;
    s.normalize();

    Float3 u = s % f;

    Matrix4& mat = Matrix4::emplace(rm, rmOffset);
    mat[0](s.x, u.x, -f.x, 0.0f);
    mat[1](s.y, u.y, -f.y, 0.0f);
    mat[2](s.z, u.z, -f.z, 0.0f);
    mat[3](0.0f, 0.0f, 0.0f, 1.0f);

    translateM(rm, rmOffset, -eyeX, -eyeY, -eyeZ);
}

void Matrix::setRotateEulerM(float* rm, int32_t rmOffset, float x, float y, float z)
{
    Float3 rot(x * deg2rad, y * deg2rad, z * deg2rad);
    const float cx = cosf(rot.x);
    const float sx = sinf(rot.x);
    const float cy = cosf(rot.y);
    const float sy = sinf(rot.y);
    const float cz = cosf(rot.z);
    const float sz = sinf(rot.z);
    const float cxsy = cx * sy;
    const float sxsy = sx * sy;

    Matrix4& mat = Matrix4::emplace(rm, rmOffset);
    mat[0](cy * cz, -cy * sz, sy, 0.0f);
    mat[1](cxsy * cz + cx * sz, -cxsy * sz + cx * cz, -sx * cy, 0.0f);
    mat[2](-sxsy * cz + sx * sz, sxsy * sz + sx * cz, cx * cy, 0.0f);
    mat[3](0.0f, 0.0f, 0.0f, 1.0f);
}

void Matrix::setRotateM(float* rm, int32_t rmOffset, float a, float x, float y, float z)
{
    Matrix4& mat = Matrix4::emplace(rm, rmOffset);
    float angle = a * deg2rad;
    const float c = cosf(angle);
    const float s = sinf(angle);
    if (x == 1.0f && y == 0.0f && z == 0.0f) {
        mat[0](1.0f, 0.0f, 0.0f, 0.0f);
        mat[1](0.0f, c, s, 0.0f);
        mat[2](0.0f, -s, c, 0.0f);
        mat[3](0.0f, 0.0f, 0.0f, 1.0f);
    } else if (x == 0.0f && y == 1.0f && z == 0.0f) {
        mat[0](c, 0.0f, -s, 0.0f);
        mat[1](0.0f, 1.0f, 0.0f, 0.0f);
        mat[2](s, 0.0f, c, 0.0f);
        mat[3](0.0f, 0.0f, 0.0f, 1.0f);
    } else if (x == 0.0f && y == 0.0f && z == 1.0f) {
        mat[0](c, s, 0.0f, 0.0f);
        mat[1](-s, c, 0.0f, 0.0f);
        mat[2](0.0f, 0.0f, 1.0f, 0.0f);
        mat[3](0.0f, 0.0f, 0.0f, 1.0f);
    } else {
        Float3::normalize(x, y, z);
        const float nc = 1.0f - c;
        const float xy = x * y;
        const float yz = y * z;
        const float zx = z * x;
        const float xs = x * s;
        const float ys = y * s;
        const float zs = z * s;
        mat[0](x * x * nc + c, xy * nc + zs, zx * nc - ys, 0.0f);
        mat[1](xy * nc - zs, y * y * nc + c, yz * nc + xs, 0.0f);
        mat[2](zx * nc + ys, yz * nc - xs, z * z * nc + c, 0.0f);
        mat[3](0.0f, 0.0f, 0.0f, 1.0f);
    }
}

void Matrix::translateM(float* m, int32_t mOffset, float x, float y, float z)
{
    Matrix4& mat = Matrix4::emplace(m, mOffset);
    const Float4 vec(x, y, z, 1.0f);
    mat[3](mat.column(0) * vec, mat.column(1) * vec, mat.column(2) * vec, mat.column(3) * vec);
}

void Matrix::translateM(float* tm, int32_t tmOffset, float* m, int32_t mOffset, float x, float y, float z)
{
    Matrix4& src = Matrix4::emplace(m, mOffset);
    Matrix4& mat = Matrix4::emplace(tm, tmOffset);
    const Float4 vec(x, y, z, 1.0f);
    mat[0] = src[0];
    mat[1] = src[1];
    mat[2] = src[2];
    mat[3](src.column(0) * vec, src.column(1) * vec, src.column(2) * vec, src.column(3) * vec);
}

void Matrix::transposeM(float* mTrans, int32_t mTransOffset, float* m, int32_t mOffset)
{
    Matrix4& src = Matrix4::emplace(m, mOffset);
    Matrix4& mat = Matrix4::emplace(mTrans, mTransOffset);
    mat[0](src[0][0], src[1][0], src[2][0], src[3][0]);
    mat[1](src[0][1], src[1][1], src[2][1], src[3][1]);
    mat[2](src[0][2], src[1][2], src[2][2], src[3][2]);
    mat[3](src[0][3], src[1][3], src[2][3], src[3][3]);
}

} // namespace opengl
} // namespace android
