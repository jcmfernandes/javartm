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
 
#include "rtm.h"

// Needed for JNI
#include <jni.h>
#include "javartm_Transaction.h"

// Other stuff
#include <stdio.h>
// JNI note: When doing a FindClass/GetMethodId/... fails, we just have to return, as the JVM will automatically throw an exception due to the failure

JNIEXPORT jboolean JNICALL Java_javartm_Transaction_rtmAvailable(JNIEnv *env, jclass cls) {
	return CPUID_RTM_CHECK() ?  1 : 0;
}

JNIEXPORT jboolean JNICALL Java_javartm_Transaction_n_1inTransaction(JNIEnv *env, jclass cls) {
	if (XTEST()) return 1;
	return 0;
}

JNIEXPORT jlong JNICALL Java_javartm_Transaction_n_1begin(JNIEnv *env, jclass cls) {
	__label__ failure;
	unsigned status = 0xFFFFFFFF;
	XBEGIN(failure);
	XFAIL_STATUS(failure, status);
	return (jlong) status;
}

JNIEXPORT void JNICALL Java_javartm_Transaction_n_1commit(JNIEnv *env, jclass cls) {
	if (XTEST()) {
		XEND();
	} else {
		// Tried to commit without active transaction
		// If we call XEND() we'll get a segfault
		jclass excClass = (*env)->FindClass(env, "java/lang/IllegalStateException");
		if (!excClass) return;
		(*env)->ThrowNew(env, excClass, "No active transaction to be committed");
	}
}

JNIEXPORT void JNICALL Java_javartm_Transaction_n_1abort(JNIEnv *env, jclass cls, jlong reason) {
	// I don't know why (register clobbering? bug?), but if reason is 32bits, then the returned
	// result after XABORT is always 0. Having reason as a long seems to work, don't know why.
	if (XTEST()) {
		// Supposedly the argument to XABORT must be encoded into the instruction.
		// To allow the user to specify his reason we must have an xabort for each value.
		// Welcome to low-level programming :)
		switch (reason) {
			case   0: XABORT(0);
			case   1: XABORT(1);
			case   2: XABORT(2);
			case   3: XABORT(3);
			case   4: XABORT(4);
			case   5: XABORT(5);
			case   6: XABORT(6);
			case   7: XABORT(7);
			case   8: XABORT(8);
			case   9: XABORT(9);
			case  10: XABORT(10);
			case  11: XABORT(11);
			case  12: XABORT(12);
			case  13: XABORT(13);
			case  14: XABORT(14);
			case  15: XABORT(15);
			case  16: XABORT(16);
			case  17: XABORT(17);
			case  18: XABORT(18);
			case  19: XABORT(19);
			case  20: XABORT(20);
			case  21: XABORT(21);
			case  22: XABORT(22);
			case  23: XABORT(23);
			case  24: XABORT(24);
			case  25: XABORT(25);
			case  26: XABORT(26);
			case  27: XABORT(27);
			case  28: XABORT(28);
			case  29: XABORT(29);
			case  30: XABORT(30);
			case  31: XABORT(31);
			case  32: XABORT(32);
			case  33: XABORT(33);
			case  34: XABORT(34);
			case  35: XABORT(35);
			case  36: XABORT(36);
			case  37: XABORT(37);
			case  38: XABORT(38);
			case  39: XABORT(39);
			case  40: XABORT(40);
			case  41: XABORT(41);
			case  42: XABORT(42);
			case  43: XABORT(43);
			case  44: XABORT(44);
			case  45: XABORT(45);
			case  46: XABORT(46);
			case  47: XABORT(47);
			case  48: XABORT(48);
			case  49: XABORT(49);
			case  50: XABORT(50);
			case  51: XABORT(51);
			case  52: XABORT(52);
			case  53: XABORT(53);
			case  54: XABORT(54);
			case  55: XABORT(55);
			case  56: XABORT(56);
			case  57: XABORT(57);
			case  58: XABORT(58);
			case  59: XABORT(59);
			case  60: XABORT(60);
			case  61: XABORT(61);
			case  62: XABORT(62);
			case  63: XABORT(63);
			case  64: XABORT(64);
			case  65: XABORT(65);
			case  66: XABORT(66);
			case  67: XABORT(67);
			case  68: XABORT(68);
			case  69: XABORT(69);
			case  70: XABORT(70);
			case  71: XABORT(71);
			case  72: XABORT(72);
			case  73: XABORT(73);
			case  74: XABORT(74);
			case  75: XABORT(75);
			case  76: XABORT(76);
			case  77: XABORT(77);
			case  78: XABORT(78);
			case  79: XABORT(79);
			case  80: XABORT(80);
			case  81: XABORT(81);
			case  82: XABORT(82);
			case  83: XABORT(83);
			case  84: XABORT(84);
			case  85: XABORT(85);
			case  86: XABORT(86);
			case  87: XABORT(87);
			case  88: XABORT(88);
			case  89: XABORT(89);
			case  90: XABORT(90);
			case  91: XABORT(91);
			case  92: XABORT(92);
			case  93: XABORT(93);
			case  94: XABORT(94);
			case  95: XABORT(95);
			case  96: XABORT(96);
			case  97: XABORT(97);
			case  98: XABORT(98);
			case  99: XABORT(99);
			case 100: XABORT(100);
			case 101: XABORT(101);
			case 102: XABORT(102);
			case 103: XABORT(103);
			case 104: XABORT(104);
			case 105: XABORT(105);
			case 106: XABORT(106);
			case 107: XABORT(107);
			case 108: XABORT(108);
			case 109: XABORT(109);
			case 110: XABORT(110);
			case 111: XABORT(111);
			case 112: XABORT(112);
			case 113: XABORT(113);
			case 114: XABORT(114);
			case 115: XABORT(115);
			case 116: XABORT(116);
			case 117: XABORT(117);
			case 118: XABORT(118);
			case 119: XABORT(119);
			case 120: XABORT(120);
			case 121: XABORT(121);
			case 122: XABORT(122);
			case 123: XABORT(123);
			case 124: XABORT(124);
			case 125: XABORT(125);
			case 126: XABORT(126);
			case 127: XABORT(127);
			case 128: XABORT(128);
			case 129: XABORT(129);
			case 130: XABORT(130);
			case 131: XABORT(131);
			case 132: XABORT(132);
			case 133: XABORT(133);
			case 134: XABORT(134);
			case 135: XABORT(135);
			case 136: XABORT(136);
			case 137: XABORT(137);
			case 138: XABORT(138);
			case 139: XABORT(139);
			case 140: XABORT(140);
			case 141: XABORT(141);
			case 142: XABORT(142);
			case 143: XABORT(143);
			case 144: XABORT(144);
			case 145: XABORT(145);
			case 146: XABORT(146);
			case 147: XABORT(147);
			case 148: XABORT(148);
			case 149: XABORT(149);
			case 150: XABORT(150);
			case 151: XABORT(151);
			case 152: XABORT(152);
			case 153: XABORT(153);
			case 154: XABORT(154);
			case 155: XABORT(155);
			case 156: XABORT(156);
			case 157: XABORT(157);
			case 158: XABORT(158);
			case 159: XABORT(159);
			case 160: XABORT(160);
			case 161: XABORT(161);
			case 162: XABORT(162);
			case 163: XABORT(163);
			case 164: XABORT(164);
			case 165: XABORT(165);
			case 166: XABORT(166);
			case 167: XABORT(167);
			case 168: XABORT(168);
			case 169: XABORT(169);
			case 170: XABORT(170);
			case 171: XABORT(171);
			case 172: XABORT(172);
			case 173: XABORT(173);
			case 174: XABORT(174);
			case 175: XABORT(175);
			case 176: XABORT(176);
			case 177: XABORT(177);
			case 178: XABORT(178);
			case 179: XABORT(179);
			case 180: XABORT(180);
			case 181: XABORT(181);
			case 182: XABORT(182);
			case 183: XABORT(183);
			case 184: XABORT(184);
			case 185: XABORT(185);
			case 186: XABORT(186);
			case 187: XABORT(187);
			case 188: XABORT(188);
			case 189: XABORT(189);
			case 190: XABORT(190);
			case 191: XABORT(191);
			case 192: XABORT(192);
			case 193: XABORT(193);
			case 194: XABORT(194);
			case 195: XABORT(195);
			case 196: XABORT(196);
			case 197: XABORT(197);
			case 198: XABORT(198);
			case 199: XABORT(199);
			case 200: XABORT(200);
			case 201: XABORT(201);
			case 202: XABORT(202);
			case 203: XABORT(203);
			case 204: XABORT(204);
			case 205: XABORT(205);
			case 206: XABORT(206);
			case 207: XABORT(207);
			case 208: XABORT(208);
			case 209: XABORT(209);
			case 210: XABORT(210);
			case 211: XABORT(211);
			case 212: XABORT(212);
			case 213: XABORT(213);
			case 214: XABORT(214);
			case 215: XABORT(215);
			case 216: XABORT(216);
			case 217: XABORT(217);
			case 218: XABORT(218);
			case 219: XABORT(219);
			case 220: XABORT(220);
			case 221: XABORT(221);
			case 222: XABORT(222);
			case 223: XABORT(223);
			case 224: XABORT(224);
			case 225: XABORT(225);
			case 226: XABORT(226);
			case 227: XABORT(227);
			case 228: XABORT(228);
			case 229: XABORT(229);
			case 230: XABORT(230);
			case 231: XABORT(231);
			case 232: XABORT(232);
			case 233: XABORT(233);
			case 234: XABORT(234);
			case 235: XABORT(235);
			case 236: XABORT(236);
			case 237: XABORT(237);
			case 238: XABORT(238);
			case 239: XABORT(239);
			case 240: XABORT(240);
			case 241: XABORT(241);
			case 242: XABORT(242);
			case 243: XABORT(243);
			case 244: XABORT(244);
			case 245: XABORT(245);
			case 246: XABORT(246);
			case 247: XABORT(247);
			case 248: XABORT(248);
			case 249: XABORT(249);
			case 250: XABORT(250);
			case 251: XABORT(251);
			case 252: XABORT(252);
			case 253: XABORT(253);
			case 254: XABORT(254);
			case 255: XABORT(255);
			 default: XABORT(0);
		}
	} else {
		// Tried to abort without active transaction
		// Throw an exception so that user code after the abort doesn't continue running
		jclass excClass = (*env)->FindClass(env, "java/lang/IllegalStateException");
		if (!excClass) return;
		(*env)->ThrowNew(env, excClass, "No active transaction to be aborted");
	}
}

JNIEXPORT jobject JNICALL Java_javartm_Transaction_n_1doTransactionally(JNIEnv *env, jclass cls, jobject atomicBlock, jobject fallbackBlock) {
	__label__ failure;
	
	// TODO: Add some caching
	jclass atomicBlockClass = (*env)->GetObjectClass(env, atomicBlock);
	jmethodID callMethodId = (*env)->GetMethodID(env, atomicBlockClass, "call", "()Ljava/lang/Object;");
	if (!callMethodId) return NULL;

	printf("Preparing execution...\n");
	unsigned status = 0xFFFFFFFF;
	XBEGIN(failure);
	jobject retValue = (*env)->CallObjectMethod(env, atomicBlock, callMethodId);
	XEND();
	printf("Successful commit\n");
	return retValue;

	XFAIL_STATUS(failure, status);
	printf("Abort or failed to start tx status = %d\n", status);
	jclass fallbackBlockClass = (*env)->GetObjectClass(env, fallbackBlock);
	callMethodId = (*env)->GetMethodID(env, fallbackBlockClass, "call", "()Ljava/lang/Object;");
	if (!callMethodId) return NULL;
	return (*env)->CallObjectMethod(env, fallbackBlock, callMethodId);
}
