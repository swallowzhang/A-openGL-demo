package com.swallow.MyGlDemo1;

import android.app.Activity;
import android.os.Bundle;

/**
 * OpenGL demo main activity, this demo for test OpenGL ES 2.0
 * @author swallow
 * @since 2014.11.26
 */
public class GlDemoActivity extends Activity {
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		this.setContentView(new MyGLSurfaceView(this));
	}
}
