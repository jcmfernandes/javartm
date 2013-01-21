/*
 * javartm: a Java library for Restricted Transactional Memory
 * Copyright (C) 2013 Ivo Anjo <ivo.anjo@ist.utl.pt>
 *
 * This file is part of javartm.
 *
 * javartm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * javartm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with javartm.  If not, see <http://www.gnu.org/licenses/>.
 */

// COMPILE WITH gcc 4.8 and -mrtm !

// RTM intrinsics
#include <immintrin.h>

#ifndef __RTM__
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#if GCC_VERSION < 40800
#error RTM intrinsics support not found. GCC version >= 4.8 is needed for RTM support.
#else
#error RTM intrinsics support not found. Please compile with -mrtm.
#endif
#endif

#ifdef __RTM__

// Used to test cpuid support
#include <x86intrin.h>
#include <cpuid.h>

// Needed for JNI
#include <jni.h>
#include "javartm_Transaction.h"

// Other stuff
#include <stdio.h>
// JNI note: When doing a FindClass/GetMethodId/... fails, we just have to return, as the JVM will automatically throw an exception due to the failure

JNIEXPORT jboolean JNICALL Java_javartm_Transaction_rtmAvailable(JNIEnv *env, jclass cls) {
	unsigned int eax, ebx, ecx, edx;
	if (__get_cpuid_max(0, NULL) >= 7) {
		__cpuid_count(7, 0, eax, ebx, ecx, edx);
		if (ebx & bit_RTM) return 1;
	}
	return 0;
}

JNIEXPORT jboolean JNICALL Java_javartm_Transaction_inTransaction(JNIEnv *env, jclass cls) {
	return _xtest();
}

JNIEXPORT jint JNICALL Java_javartm_Transaction_begin(JNIEnv *env, jclass cls) {
	return _xbegin();
}

JNIEXPORT void JNICALL Java_javartm_Transaction_commit(JNIEnv *env, jclass cls) {
	if (_xtest()) {
		_xend();
	} else {
		// Tried to commit without active transaction
		// If we call _xend() we'll get a segfault
		jclass excClass = (*env)->FindClass(env, "java/lang/IllegalStateException");
		if (!excClass) return;
		(*env)->ThrowNew(env, excClass, "No active transaction to be committed");
	}
}

JNIEXPORT void JNICALL Java_javartm_Transaction_abort(JNIEnv *env, jclass cls) {
	_xabort(0);
}

JNIEXPORT jobject JNICALL Java_javartm_Transaction_doTransactionally(JNIEnv *env, jclass cls, jobject atomicBlock, jobject fallbackBlock) {
	// TODO: Add some caching
	jclass atomicBlockClass = (*env)->GetObjectClass(env, atomicBlock);
	jmethodID callMethodId = (*env)->GetMethodID(env, atomicBlockClass, "call", "()Ljava/lang/Object;");
	if (!callMethodId) return NULL;

	printf("Preparing execution...\n");
	int res = _xbegin();
	if (_xtest()) {
		jobject retValue = (*env)->CallObjectMethod(env, atomicBlock, callMethodId);
		_xend();
		printf("Successful commit\n");
		return retValue;
	}

	printf("Abort or failed to start tx res = %d\n", res);
	jclass fallbackBlockClass = (*env)->GetObjectClass(env, fallbackBlock);
	callMethodId = (*env)->GetMethodID(env, fallbackBlockClass, "call", "()Ljava/lang/Object;");
	if (!callMethodId) return NULL;
	return (*env)->CallObjectMethod(env, fallbackBlock, callMethodId);
}

#endif // __RTM__
