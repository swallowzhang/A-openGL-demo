LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libmygldemo1
LOCAL_SRC_FILES := mygldemo.cpp regular_polygon.cpp
LOCAL_CFLAGS := -Werror
LOCAL_LDLIBS := -llog -lGLESv2

include $(BUILD_SHARED_LIBRARY)