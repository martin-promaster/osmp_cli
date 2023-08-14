#include <stdlib.h>
#include <jni.h>
#include "../include/pmscore.h"
#include "../include/cn_ybits_pms_common_nfi_NativeProxy.h"

JNIEXPORT jint JNICALL Java_cn_ybits_pms_common_nfi_NativeProxy_welcomeMessage
  (JNIEnv *pEnv, jclass clazz) {
  return  (*pEnv)->GetVersion(pEnv);
  //return welcomeMessage();
}

JNIEXPORT jint JNICALL Java_cn_ybits_pms_common_nfi_NativeProxy_add
  (JNIEnv *pEnv, jclass clazz, jint ja, jint jb) {
	return add(ja, jb);
}

JNIEXPORT jdouble JNICALL Java_cn_ybits_pms_common_nfi_NativeProxy_calcCost
  (JNIEnv *pEnv, jclass clazz, jdouble d) {
	return calcCost(d);
}

JNIEXPORT jstring JNICALL Java_cn_ybits_pms_common_nfi_NativeProxy_mailSecurityInfo
(JNIEnv* pEnv, jclass clazz) {
    char* newchar = (char*)malloc(sizeof(char)*MAX_PASSWORD);
    GenerateMailSecurityInfo(newchar);
    return (*pEnv)->NewStringUTF(pEnv, newchar);
}