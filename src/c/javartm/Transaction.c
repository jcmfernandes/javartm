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

JNIEXPORT void JNICALL Java_javartm_Transaction_abort__(JNIEnv *env, jclass cls) {
	if (_xtest()) {
		_xabort(0);
	} else {
		// Tried to abort without active transaction
		// Throw an exception so that user code after the abort doesn't continue running
		jclass excClass = (*env)->FindClass(env, "java/lang/IllegalStateException");
		if (!excClass) return;
		(*env)->ThrowNew(env, excClass, "No active transaction to be aborted");
	}
}

JNIEXPORT void JNICALL Java_javartm_Transaction_abort__J(JNIEnv *env, jclass cls, jlong reason) {
	// I don't know why (register clobbering? bug?), but if reason is 32bits, then the returned
	// result after _xabort is always 0. Having reason as a long seems to work, don't know why.
	if (_xtest()) {
		// Supposedly the argument to _xabort must be encoded into the instruction.
		// To allow the user to specify his reason we must have an xabort for each value.
		// Welcome to low-level programming :)
		switch (reason) {
			case   0: _xabort(0);
			case   1: _xabort(1);
			case   2: _xabort(2);
			case   3: _xabort(3);
			case   4: _xabort(4);
			case   5: _xabort(5);
			case   6: _xabort(6);
			case   7: _xabort(7);
			case   8: _xabort(8);
			case   9: _xabort(9);
			case  10: _xabort(10);
			case  11: _xabort(11);
			case  12: _xabort(12);
			case  13: _xabort(13);
			case  14: _xabort(14);
			case  15: _xabort(15);
			case  16: _xabort(16);
			case  17: _xabort(17);
			case  18: _xabort(18);
			case  19: _xabort(19);
			case  20: _xabort(20);
			case  21: _xabort(21);
			case  22: _xabort(22);
			case  23: _xabort(23);
			case  24: _xabort(24);
			case  25: _xabort(25);
			case  26: _xabort(26);
			case  27: _xabort(27);
			case  28: _xabort(28);
			case  29: _xabort(29);
			case  30: _xabort(30);
			case  31: _xabort(31);
			case  32: _xabort(32);
			case  33: _xabort(33);
			case  34: _xabort(34);
			case  35: _xabort(35);
			case  36: _xabort(36);
			case  37: _xabort(37);
			case  38: _xabort(38);
			case  39: _xabort(39);
			case  40: _xabort(40);
			case  41: _xabort(41);
			case  42: _xabort(42);
			case  43: _xabort(43);
			case  44: _xabort(44);
			case  45: _xabort(45);
			case  46: _xabort(46);
			case  47: _xabort(47);
			case  48: _xabort(48);
			case  49: _xabort(49);
			case  50: _xabort(50);
			case  51: _xabort(51);
			case  52: _xabort(52);
			case  53: _xabort(53);
			case  54: _xabort(54);
			case  55: _xabort(55);
			case  56: _xabort(56);
			case  57: _xabort(57);
			case  58: _xabort(58);
			case  59: _xabort(59);
			case  60: _xabort(60);
			case  61: _xabort(61);
			case  62: _xabort(62);
			case  63: _xabort(63);
			case  64: _xabort(64);
			case  65: _xabort(65);
			case  66: _xabort(66);
			case  67: _xabort(67);
			case  68: _xabort(68);
			case  69: _xabort(69);
			case  70: _xabort(70);
			case  71: _xabort(71);
			case  72: _xabort(72);
			case  73: _xabort(73);
			case  74: _xabort(74);
			case  75: _xabort(75);
			case  76: _xabort(76);
			case  77: _xabort(77);
			case  78: _xabort(78);
			case  79: _xabort(79);
			case  80: _xabort(80);
			case  81: _xabort(81);
			case  82: _xabort(82);
			case  83: _xabort(83);
			case  84: _xabort(84);
			case  85: _xabort(85);
			case  86: _xabort(86);
			case  87: _xabort(87);
			case  88: _xabort(88);
			case  89: _xabort(89);
			case  90: _xabort(90);
			case  91: _xabort(91);
			case  92: _xabort(92);
			case  93: _xabort(93);
			case  94: _xabort(94);
			case  95: _xabort(95);
			case  96: _xabort(96);
			case  97: _xabort(97);
			case  98: _xabort(98);
			case  99: _xabort(99);
			case 100: _xabort(100);
			case 101: _xabort(101);
			case 102: _xabort(102);
			case 103: _xabort(103);
			case 104: _xabort(104);
			case 105: _xabort(105);
			case 106: _xabort(106);
			case 107: _xabort(107);
			case 108: _xabort(108);
			case 109: _xabort(109);
			case 110: _xabort(110);
			case 111: _xabort(111);
			case 112: _xabort(112);
			case 113: _xabort(113);
			case 114: _xabort(114);
			case 115: _xabort(115);
			case 116: _xabort(116);
			case 117: _xabort(117);
			case 118: _xabort(118);
			case 119: _xabort(119);
			case 120: _xabort(120);
			case 121: _xabort(121);
			case 122: _xabort(122);
			case 123: _xabort(123);
			case 124: _xabort(124);
			case 125: _xabort(125);
			case 126: _xabort(126);
			case 127: _xabort(127);
			case 128: _xabort(128);
			case 129: _xabort(129);
			case 130: _xabort(130);
			case 131: _xabort(131);
			case 132: _xabort(132);
			case 133: _xabort(133);
			case 134: _xabort(134);
			case 135: _xabort(135);
			case 136: _xabort(136);
			case 137: _xabort(137);
			case 138: _xabort(138);
			case 139: _xabort(139);
			case 140: _xabort(140);
			case 141: _xabort(141);
			case 142: _xabort(142);
			case 143: _xabort(143);
			case 144: _xabort(144);
			case 145: _xabort(145);
			case 146: _xabort(146);
			case 147: _xabort(147);
			case 148: _xabort(148);
			case 149: _xabort(149);
			case 150: _xabort(150);
			case 151: _xabort(151);
			case 152: _xabort(152);
			case 153: _xabort(153);
			case 154: _xabort(154);
			case 155: _xabort(155);
			case 156: _xabort(156);
			case 157: _xabort(157);
			case 158: _xabort(158);
			case 159: _xabort(159);
			case 160: _xabort(160);
			case 161: _xabort(161);
			case 162: _xabort(162);
			case 163: _xabort(163);
			case 164: _xabort(164);
			case 165: _xabort(165);
			case 166: _xabort(166);
			case 167: _xabort(167);
			case 168: _xabort(168);
			case 169: _xabort(169);
			case 170: _xabort(170);
			case 171: _xabort(171);
			case 172: _xabort(172);
			case 173: _xabort(173);
			case 174: _xabort(174);
			case 175: _xabort(175);
			case 176: _xabort(176);
			case 177: _xabort(177);
			case 178: _xabort(178);
			case 179: _xabort(179);
			case 180: _xabort(180);
			case 181: _xabort(181);
			case 182: _xabort(182);
			case 183: _xabort(183);
			case 184: _xabort(184);
			case 185: _xabort(185);
			case 186: _xabort(186);
			case 187: _xabort(187);
			case 188: _xabort(188);
			case 189: _xabort(189);
			case 190: _xabort(190);
			case 191: _xabort(191);
			case 192: _xabort(192);
			case 193: _xabort(193);
			case 194: _xabort(194);
			case 195: _xabort(195);
			case 196: _xabort(196);
			case 197: _xabort(197);
			case 198: _xabort(198);
			case 199: _xabort(199);
			case 200: _xabort(200);
			case 201: _xabort(201);
			case 202: _xabort(202);
			case 203: _xabort(203);
			case 204: _xabort(204);
			case 205: _xabort(205);
			case 206: _xabort(206);
			case 207: _xabort(207);
			case 208: _xabort(208);
			case 209: _xabort(209);
			case 210: _xabort(210);
			case 211: _xabort(211);
			case 212: _xabort(212);
			case 213: _xabort(213);
			case 214: _xabort(214);
			case 215: _xabort(215);
			case 216: _xabort(216);
			case 217: _xabort(217);
			case 218: _xabort(218);
			case 219: _xabort(219);
			case 220: _xabort(220);
			case 221: _xabort(221);
			case 222: _xabort(222);
			case 223: _xabort(223);
			case 224: _xabort(224);
			case 225: _xabort(225);
			case 226: _xabort(226);
			case 227: _xabort(227);
			case 228: _xabort(228);
			case 229: _xabort(229);
			case 230: _xabort(230);
			case 231: _xabort(231);
			case 232: _xabort(232);
			case 233: _xabort(233);
			case 234: _xabort(234);
			case 235: _xabort(235);
			case 236: _xabort(236);
			case 237: _xabort(237);
			case 238: _xabort(238);
			case 239: _xabort(239);
			case 240: _xabort(240);
			case 241: _xabort(241);
			case 242: _xabort(242);
			case 243: _xabort(243);
			case 244: _xabort(244);
			case 245: _xabort(245);
			case 246: _xabort(246);
			case 247: _xabort(247);
			case 248: _xabort(248);
			case 249: _xabort(249);
			case 250: _xabort(250);
			case 251: _xabort(251);
			case 252: _xabort(252);
			case 253: _xabort(253);
			case 254: _xabort(254);
			case 255: _xabort(255);
			 default: _xabort(0);
		}
	} else {
		// Tried to abort without active transaction
		// Throw an exception so that user code after the abort doesn't continue running
		jclass excClass = (*env)->FindClass(env, "java/lang/IllegalStateException");
		if (!excClass) return;
		(*env)->ThrowNew(env, excClass, "No active transaction to be aborted");
	}
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
