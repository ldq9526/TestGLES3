package com.example.testgles3;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLRenderer implements GLSurfaceView.Renderer {
    public boolean initialized;

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        initialized = GLUtils.initialize();
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int w, int h) {
        GLUtils.resize(w, h);
    }

    @Override
    public void onDrawFrame(GL10 gl10) {
        GLUtils.display();
    }
}
