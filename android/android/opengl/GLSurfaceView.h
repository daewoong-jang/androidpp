/*
 * Copyright (C) 2016 NAVER Corp. All rights reserved.
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
#include <android/view/View.h>

namespace android {
namespace opengl {

class GLSurfaceView : public view::View {
    NONCOPYABLE(GLSurfaceView);
public:
    static const bool DEBUG_GL = false;
    static const bool PRINT_ALL_CONFIGS = false;

    // The renderer only renders when the surface is created, or when requestRender() is called. 
    static const int32_t RENDERMODE_WHEN_DIRTY = 0;
    // The renderer is called continuously to re-render the scene.
    static const int32_t RENDERMODE_CONTINUOUSLY = 1;

    class Renderer {
    public:
        virtual ~Renderer() { }

        virtual void onSurfaceCreated(GL10 gl) = 0;
        virtual void onSurfaceChanged(GL10 gl, int32_t width, int32_t height) = 0;
        virtual void onDrawFrame(GL10 gl) = 0;
    };

    ANDROID_EXPORT GLSurfaceView(Context&);
    ANDROID_EXPORT virtual ~GLSurfaceView();

    // Inform the default EGLContextFactory and default EGLConfigChooser which EGLContext client version to pick. 
    ANDROID_EXPORT virtual void setEGLContextClientVersion(int32_t version);
    // Set the rendering mode. 
    ANDROID_EXPORT virtual void setRenderMode(int32_t renderMode);
    // Control whether the EGL context is preserved when the GLSurfaceView is paused and resumed. 
    ANDROID_EXPORT virtual void setPreserveEGLContextOnPause(bool preserveOnPause);
    ANDROID_EXPORT virtual bool getPreserveEGLContextOnPause();
    // Set the renderer associated with this view. 
    ANDROID_EXPORT virtual void setRenderer(Renderer* renderer);

    // Request that the renderer render a frame. 
    ANDROID_EXPORT virtual void requestRender();

    // This is called immediately after any structural changes (format or size) have been made to the surface.
    ANDROID_EXPORT virtual void surfaceChanged(int32_t format, int32_t width, int32_t height);
    // This is called immediately after the surface is first created.
    ANDROID_EXPORT virtual void surfaceCreated();
    // This is called immediately before a surface is being destroyed.
    ANDROID_EXPORT virtual void surfaceDestroyed();

    // Called after onRestoreInstanceState(Bundle), onRestart(), or onPause(), for your activity to start interacting with the user. 
    ANDROID_EXPORT virtual void onResume();
    // Called as part of the activity lifecycle when an activity is going into the background, but has not (yet) been killed. 
    ANDROID_EXPORT virtual void onPause();

    // Queue a runnable to be run on the GL rendering thread. 
    ANDROID_EXPORT virtual void queueEvent(std::function<void ()>&& r);

    // view.View
    ANDROID_EXPORT virtual void onAttachedToWindow() override;
    ANDROID_EXPORT virtual void onDetachedFromWindow() override;
    ANDROID_EXPORT virtual void onLayout(bool changed, int32_t left, int32_t top, int32_t right, int32_t bottom) override;

private:
    class GLThread;
    friend class GLThread;

    int32_t m_eglContextClientVersion;
    Renderer* m_renderer;
    std::unique_ptr<GLThread> m_glThread;
    bool m_preserveEGLContextOnPause { false };
};

} // namespace opengl
} // namespace android

using GLSurfaceView = android::opengl::GLSurfaceView;
