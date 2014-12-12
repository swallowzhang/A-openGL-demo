package com.swallow.MyGlDemo1;

/**
 * Load lib and list the methods
 * @author swallow
 * @since 2014.11.26
 */
public class MyGLDemoLib {
	static{
		System.loadLibrary("mygldemo1");
	}
	
	public static native void nativeInit(int width, int height);
	public static native void nativeStep();
	public static native void nativeUpdate();
}
