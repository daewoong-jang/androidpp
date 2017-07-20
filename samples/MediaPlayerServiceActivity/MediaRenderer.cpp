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

#include "MediaRenderer.h"

#include "MediaSurface.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/opengl/Matrix.h>
#include <android++/LogHelper.h>

namespace com {
namespace example {

static const int32_t FLOAT_SIZE_BYTES = 4;
static const int32_t TRIANGLE_VERTICES_DATA_STRIDE_BYTES = 5 * FLOAT_SIZE_BYTES;
static const int32_t TRIANGLE_VERTICES_DATA_POS_OFFSET = 0;
static const int32_t TRIANGLE_VERTICES_DATA_UV_OFFSET = 3;
static const float mTriangleVerticesData[] = {
        // X, Y, Z, U, V
        -1.0f, -1.0f, 0, 0.f, 0.f,
        1.0f, -1.0f, 0, 1.f, 0.f,
        -1.0f, 1.0f, 0, 0.f, 1.f,
        1.0f, 1.0f, 0, 1.f, 1.f,
};

static const std::string mVertexShader =
        "uniform mat4 uMVPMatrix;\n" \
        "uniform mat4 uSTMatrix;\n" \
        "attribute vec4 aPosition;\n" \
        "attribute vec4 aTextureCoord;\n" \
        "varying vec2 vTextureCoord;\n" \
        "void main() {\n" \
        "  gl_Position = uMVPMatrix * aPosition;\n" \
        "  vTextureCoord = (uSTMatrix * aTextureCoord).xy;\n" \
        "}\n";

static const std::string mFragmentShader =
        "#extension GL_OES_EGL_image_external : require\n" \
        "precision mediump float;\n" \
        "varying vec2 vTextureCoord;\n" \
        "uniform samplerExternalOES sTexture;\n" \
        "void main() {\n" \
        "  gl_FragColor = texture2D(sTexture, vTextureCoord);\n" \
        "}\n";

MediaRenderer::MediaRenderer(Context& context)
{
    mTriangleVertices.insert(mTriangleVertices.end(), mTriangleVerticesData, mTriangleVerticesData + sizeof(mTriangleVerticesData) / sizeof(float));

    Matrix::setIdentityM(mSTMatrix, 0);
}

MediaRenderer::~MediaRenderer()
{
}

void MediaRenderer::setSurface(std::passed_ptr<MediaSurface> surface)
{
    synchronized (this) {
        mSurface = surface;
    }
}

void MediaRenderer::onSurfaceCreated(GL10 gl)
{
    mProgram = createProgram(mVertexShader, mFragmentShader);
    if (mProgram == 0) {
        return;
    }
    maPositionHandle = glGetAttribLocation(mProgram, "aPosition");
    checkGlError("glGetAttribLocation aPosition");
    if (maPositionHandle == -1) {
        LOGA("Could not get attrib location for aPosition");
    }
    maTextureHandle = glGetAttribLocation(mProgram, "aTextureCoord");
    checkGlError("glGetAttribLocation aTextureCoord");
    if (maTextureHandle == -1) {
        LOGA("Could not get attrib location for aTextureCoord");
    }

    muMVPMatrixHandle = glGetUniformLocation(mProgram, "uMVPMatrix");
    checkGlError("glGetUniformLocation uMVPMatrix");
    if (muMVPMatrixHandle == -1) {
        LOGA("Could not get attrib location for uMVPMatrix");
    }

    muSTMatrixHandle = glGetUniformLocation(mProgram, "uSTMatrix");
    checkGlError("glGetUniformLocation uSTMatrix");
    if (muSTMatrixHandle == -1) {
        LOGA("Could not get attrib location for uSTMatrix");
    }
}

void MediaRenderer::onSurfaceChanged(GL10 gl, int32_t width, int32_t height)
{
}

void MediaRenderer::onDrawFrame(GL10 gl)
{
    synchronized (this) {
        if (!mSurface)
            return;
        
        mSurface->updateTexture(mSTMatrix);
    }

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glUseProgram(mProgram);
    checkGlError("glUseProgram");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, mSurface->getTextureName());

    glVertexAttribPointer(maPositionHandle, 3, GL_FLOAT,
            false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices.data() + TRIANGLE_VERTICES_DATA_POS_OFFSET);
    checkGlError("glVertexAttribPointer maPosition");
    glEnableVertexAttribArray(maPositionHandle);
    checkGlError("glEnableVertexAttribArray maPositionHandle");

    glVertexAttribPointer(maTextureHandle, 3, GL_FLOAT,
            false, TRIANGLE_VERTICES_DATA_STRIDE_BYTES, mTriangleVertices.data() + TRIANGLE_VERTICES_DATA_UV_OFFSET);
    checkGlError("glVertexAttribPointer maTextureHandle");
    glEnableVertexAttribArray(maTextureHandle);
    checkGlError("glEnableVertexAttribArray maTextureHandle");

    Matrix::setIdentityM(mMVPMatrix, 0);
    glUniformMatrix4fv(muMVPMatrixHandle, 1, false, mMVPMatrix);
    glUniformMatrix4fv(muSTMatrixHandle, 1, false, mSTMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    checkGlError("glDrawArrays");
    glFinish();
}

int32_t MediaRenderer::loadShader(int32_t shaderType, std::string source)
{
    int32_t shader = glCreateShader(shaderType);
    if (shader != 0) {
        const GLchar* string[1] = { source.c_str() };
        GLint length[1] = { source.length() };
        glShaderSource(shader, 1, string, length);
        glCompileShader(shader);
        int32_t compiled[1];
        glGetShaderiv(shader, GL_COMPILE_STATUS, compiled);
        if (compiled[0] == 0) {
            LOGE("Could not compile shader %d :", shaderType);
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

int32_t MediaRenderer::createProgram(std::string vertexSource, std::string fragmentSource)
{
    int32_t vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (vertexShader == 0) {
        return 0;
    }
    int32_t pixelShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (pixelShader == 0) {
        return 0;
    }

    int32_t program = glCreateProgram();
    if (program != 0) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        int32_t linkStatus[1];
        glGetProgramiv(program, GL_LINK_STATUS, linkStatus);
        if (linkStatus[0] != GL_TRUE) {
            LOGE("Could not link program: ");
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

void MediaRenderer::checkGlError(std::string op)
{
    int32_t error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        LOGA("%s: glError %d", op, error);
    }
}

} // namespace example
} // namespace com
