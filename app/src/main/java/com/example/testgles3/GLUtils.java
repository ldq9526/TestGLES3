package com.example.testgles3;

public class GLUtils {
    static {
        System.loadLibrary("native-lib");
    }

    public static native boolean initialize();
    public static native String getErrors();
    public static native void resize(int width, int height);
    public static native void display();
    public static native void update(float angle, int rotationAxis);
    public static native void reset();
}
