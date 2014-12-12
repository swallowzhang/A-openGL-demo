#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

#include "regular_polygon.cpp"

#define  LOG_TAG		"mygldemo"
#define  LOGD(...)		__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  	__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  	__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

static void printGLString(const char* tag, GLenum s){
	const char* strResult = (const char*)glGetString(s);
	LOGI("GL %s = %s\n", tag, strResult);
}

static void checkGLError(const char* op){
	for(GLint error = glGetError(); error; error = glGetError()){
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

static const char gVertexShader[] =
	"attribute vec4 vPosition;\n"
	"void main(){\n"
	"	gl_Position = vPosition;\n"
	"}\n";

static const char gFragmentShader[]=
	"precision mediump float;\n"
	"void main(){\n"
	"	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
	"}\n";

GLuint loadShader(GLenum shaderType, const char* src){
	GLuint shader = glCreateShader(shaderType);
	assert(shader);

	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if(!compiled){
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if(infoLen){
			char* buf = (char*)malloc(infoLen);
			if(buf){
				glGetShaderInfoLog(shader, infoLen, NULL, buf);
				LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
	}
	return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource){
	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if(!vertexShader)
		return 0;

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if(!pixelShader)
		return 0;

	GLuint program = glCreateProgram();
	assert(program);

	glAttachShader(program, vertexShader);
	checkGLError("glAttachShader");
	glAttachShader(program, pixelShader);
	checkGLError("glAttachShader");
	glLinkProgram(program);
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if(linkStatus != GL_TRUE){
		GLint bufLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
		if(bufLength){
			char* buf = (char*) malloc(bufLength);
			if(buf){
				glGetProgramInfoLog(program, bufLength, NULL, buf);
				LOGE("Could not link program:\n%s\n", buf);
				free(buf);
			}
		}
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

GLuint gProgram;
GLuint gvPositionHandle;
GLfloat center[] = {100, 100};
RegularPolygon* circle = NULL;
static int width = -1;
static int height = -1;
static bool direct = true;

bool setupGraphics(int w, int h){
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	LOGI("setGraphics(%d, %d)", w, h);
	gProgram = createProgram(gVertexShader, gFragmentShader);
	if(!gProgram){
		LOGE("Could not create program.");
		return false;
	}
	gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");
	checkGLError("glGetAttribLocation");
	LOGI("glGetAttribLocation(\"vPosition\") = %d\n", gvPositionHandle);
	glViewport(0, 0, w, h);
	checkGLError("glViewport");

	if(circle != NULL)
		delete(circle);
	center[0] = 100;
	center[1] = 100;
	circle = new RegularPolygon(20, &center[0]);
	circle->translate(w, h);
	width = w;
	height = h;

	return true;
}

//const GLfloat gTriangleVertices[][2] = {
//		{0.0f, 0.5f},
//		{-0.5f, -0.5f},
//		{0.5f, -0.5f}
//};

long getCurrentTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return tv.tv_sec * 1000 + tv.tv_usec/1000;
}

void moveCircle(long current){
//    center[0] += 1;
	if(center[0] >= width-20)
		direct = false;
	else if(center[0] <= 20)
		direct = true;

	if(direct)
		center[0] += 4;
	else
		center[0] -= 4;

    if(circle != NULL)
    	delete(circle);
    circle = new RegularPolygon(20, &center[0]);
    circle->translate(width, height);
}

void renderFrame(){
	if(circle == NULL)
		return;
//	LOGD("eeeeeeeeeeeeeeeeeeeee   %ld", getCurrentTime());
	static long start = getCurrentTime();
	static float grey = 0.1f;
//	grey += 0.01f;
	if(grey > 1.0f)
		grey = 0.0f;

//	moveCircle(0);

	glClearColor(grey, grey, grey, 1.0f);
	checkGLError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGLError("glClear");

	glUseProgram(gProgram);
	checkGLError("glUseProgram");

	glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, circle->mVertices);
	checkGLError("glVertexAttribPointer");
	glEnableVertexAttribArray(gvPositionHandle);
	checkGLError("glEnableVertexAttribArray");
	glDrawArrays(GL_TRIANGLE_FAN, 0, dotCounts);
	checkGLError("glDrawArrays");
	static long end = getCurrentTime();
	delete(circle);
	circle = NULL;
//	LOGD("Timmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm   %ld", end-start);
}

void updateCircleBuf(){
	if(circle == NULL){
		moveCircle(0);
	}
}

extern "C" {
    JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeInit(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeStep(JNIEnv * env, jobject obj);
    JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeUpdate(JNIEnv * env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeInit(JNIEnv * env, jobject obj,  jint width, jint height){
    setupGraphics(width, height);
}

JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeStep(JNIEnv * env, jobject obj){
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_swallow_MyGlDemo1_MyGLDemoLib_nativeUpdate(JNIEnv * env, jobject obj){
	updateCircleBuf();
}
