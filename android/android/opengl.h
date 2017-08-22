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

#include <EGL/egl.h>

#define EGL_NO_CONFIG (EGLConfig)(0)

#if EGL_SOFT_LINKING
#define eglGetDisplay eglGetDisplayPtr()
#endif

#include <EGL/eglext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#ifndef EGL_EGLEXT_PROTOTYPES
SOFT_LINK(libEGL, eglCreateImageKHR, EGLImageKHR, EGLAPIENTRY, (EGLDisplay dpy, EGLContext ctx, EGLenum target, EGLClientBuffer buffer, const EGLint *attrib_list), (dpy, ctx, target, buffer, attrib_list));
SOFT_LINK(libEGL, eglDestroyImageKHR, EGLBoolean, EGLAPIENTRY, (EGLDisplay dpy, EGLImageKHR image), (dpy, image));
#endif

#ifndef GL_GLEXT_PROTOTYPES
SOFT_LINK(libGLESv2, glEGLImageTargetTexture2DOES, void, GL_APIENTRY, (GLenum target, GLeglImageOES image), (target, image));
#endif

#include <java/lang.h>

namespace javax {
namespace microedition {
namespace khronos {
namespace opengles {

typedef void* GL10;

} // namespace opengles
} // namespace khronos
} // namespace microedition
} // namespace javax

using GL10 = javax::microedition::khronos::opengles::GL10;
