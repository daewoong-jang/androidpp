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

#pragma once

#include <android/opengl.h>
#include <android++/IdentifierErasure.h>

namespace android {
namespace opengl {

class Matrix {
public:
    // Defines a projection matrix in terms of six clip planes.
    ANDROID_EXPORT static void frustumM(float* m, int32_t offset, float left, float right, float bottom, float top, float near, float far);
    // Inverts a 4 x 4 matrix.
    ANDROID_EXPORT static bool invertM(float* mInv, int32_t mInvOffset, float* m, int32_t mOffset);
    // Computes the length of a vector.
    ANDROID_EXPORT static float length(float x, float y, float z);
    // Multiplies two 4x4 matrices together and stores the result in a third 4x4 matrix.
    ANDROID_EXPORT static void multiplyMM(float* result, int32_t resultOffset, float* lhs, int32_t lhsOffset, float* rhs, int32_t rhsOffset);
    // Multiplies a 4 element vector by a 4x4 matrix and stores the result in a 4-element column vector.
    ANDROID_EXPORT static void multiplyMV(float* resultVec, int32_t resultVecOffset, float* lhsMat, int32_t lhsMatOffset, float* rhsVec, int32_t rhsVecOffset);
    // Computes an orthographic projection matrix.
    ANDROID_EXPORT static void orthoM(float* m, int32_t mOffset, float left, float right, float bottom, float top, float near, float far);
    // Defines a projection matrix in terms of a field of view angle, an aspect ratio, and z clip planes.
    ANDROID_EXPORT static void perspectiveM(float* m, int32_t offset, float fovy, float aspect, float zNear, float zFar);
    // Rotates matrix m by angle a (in degrees) around the axis (x, y, z).
    ANDROID_EXPORT static void rotateM(float* rm, int32_t rmOffset, float* m, int32_t mOffset, float a, float x, float y, float z);
    // Rotates matrix m in place by angle a (in degrees) around the axis (x, y, z).
    ANDROID_EXPORT static void rotateM(float* m, int32_t mOffset, float a, float x, float y, float z);
    // Scales matrix m in place by sx, sy, and sz.
    ANDROID_EXPORT static void scaleM(float* m, int32_t mOffset, float x, float y, float z);
    // Scales matrix m by x, y, and z, putting the result in sm.
    ANDROID_EXPORT static void scaleM(float* sm, int32_t smOffset, float* m, int32_t mOffset, float x, float y, float z);
    // Sets matrix m to the identity matrix.
    ANDROID_EXPORT static void setIdentityM(float* sm, int32_t smOffset);
    // Defines a viewing transformation in terms of an eye point, a center of view, and an up vector.
    ANDROID_EXPORT static void setLookAtM(float* rm, int32_t rmOffset, float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ, float upX, float upY, float upZ);
    // Converts Euler angles to a rotation matrix.
    ANDROID_EXPORT static void setRotateEulerM(float* rm, int32_t rmOffset, float x, float y, float z);
    // Creates a matrix for rotation by angle a (in degrees) around the axis (x, y, z).
    ANDROID_EXPORT static void setRotateM(float* rm, int32_t rmOffset, float a, float x, float y, float z);
    // Translates matrix m by x, y, and z in place.
    ANDROID_EXPORT static void translateM(float* m, int32_t mOffset, float x, float y, float z);
    // Translates matrix m by x, y, and z, putting the result in tm.
    ANDROID_EXPORT static void translateM(float* tm, int32_t tmOffset, float* m, int32_t mOffset, float x, float y, float z);
    // Transposes a 4 x 4 matrix.
    ANDROID_EXPORT static void transposeM(float* mTrans, int32_t mTransOffset, float* m, int32_t mOffset);

private:
    Matrix() = default;
};

} // namespace opengl
} // namespace android

using Matrix = android::opengl::Matrix;
