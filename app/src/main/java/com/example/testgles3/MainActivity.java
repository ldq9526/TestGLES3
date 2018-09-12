package com.example.testgles3;

import android.opengl.GLSurfaceView;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements View.OnTouchListener {

    private GLSurfaceView glSurfaceView;
    private float oldX, oldY;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main_activity);
        glSurfaceView = (GLSurfaceView)findViewById(R.id.gl_surface_view);
        GLRenderer glRenderer = new GLRenderer();
        glSurfaceView.setEGLContextClientVersion(3);
        glSurfaceView.setRenderer(glRenderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
        TextView errorText = (TextView)findViewById(R.id.error);
        if(!glRenderer.initialized)
            errorText.setText(GLUtils.getErrors());
        glSurfaceView.setOnTouchListener(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    public boolean onTouch(View v, MotionEvent event) {
        int action = event.getAction();
        if(MotionEvent.ACTION_DOWN == action) {
            oldX = event.getX();
            oldY = event.getY();
        } else if(MotionEvent.ACTION_MOVE == action) {
            float dx = event.getX() - oldX, dy = event.getY() - oldY;
            float adx = Math.abs(dx), ady = Math.abs(dy);
            if(adx > ady)
                GLUtils.update(dx/adx, 1);
            else
                GLUtils.update(dy/ady, 0);
            glSurfaceView.requestRender();
            oldX += dx;
            oldY += dy;
        } else if(MotionEvent.ACTION_UP != action) {
            GLUtils.reset();
            glSurfaceView.requestRender();
        }
        return true;
    }
}
