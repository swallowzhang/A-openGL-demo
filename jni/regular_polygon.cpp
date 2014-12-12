#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/log.h>

static const GLuint dotCounts = 10;

class RegularPolygon{
public:
	RegularPolygon(GLfloat radium, GLfloat* center){
		this->init(radium, center);
	}
	~RegularPolygon(){
		free(this->mVertices);
	}
	GLfloat* mVertices;

	void translate(int width, int height){
		int i = 0;
		for(i; i<dotCounts; i++){
			mVertices[i*2] = mVertices[i*2]*2/width;
			mVertices[i*2+1] = mVertices[i*2+1]*2/height;

			mVertices[i*2] -= 1;
			mVertices[i*2+1] = 1-mVertices[i*2+1];
		}
	}


private:
	void init(GLfloat radium, GLfloat* center){
		mVertices = (GLfloat*)malloc(dotCounts*2*4);
		float angle = 0;
		int i = 0;
		for(i; i<dotCounts; i++){
			mVertices[i*2] = center[0] + radium*cos(angle);
			mVertices[i*2 + 1] = center[1] + radium*sin(angle);
			angle += 3.1415926*2/(dotCounts-1);
		}
	}
};

