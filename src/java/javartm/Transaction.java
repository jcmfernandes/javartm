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

package javartm;

import java.io.*;
import java.util.concurrent.Callable;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Transaction {
	private static final Logger Log = LoggerFactory.getLogger(Transaction.class);

	public static final long STARTED		= 0xFFFFFFFF;

	public static final int ABORT_EXPLICIT 	= 1 << 0;
	public static final int ABORT_RETRY 	= 1 << 1;
	public static final int ABORT_CONFLICT 	= 1 << 2;
	public static final int ABORT_CAPACITY 	= 1 << 3;
	public static final int ABORT_DEBUG 	= 1 << 4;
	public static final int ABORT_NESTED 	= 1 << 5;

	public static final boolean RTM_AVAILABLE;

	static {
		// Attempt to load native library from jar
		InputStream libFile = Transaction.class.getResourceAsStream("libjavartm.so");
		if (libFile != null) {
			try {
				// Native libraries *have* to be loaded from a file, so we
				// create a temporary file, dump the native library from the
				// jar, and then load it from there
				File f = File.createTempFile("libjavartm", "so");
				f.deleteOnExit();
				FileOutputStream fos = new FileOutputStream(f);
				int read;
				byte[] buffer = new byte[4096];
				while ((read = libFile.read(buffer, 0, buffer.length)) != -1) {
					fos.write(buffer, 0, read);
				}
				fos.close();
				Runtime.getRuntime().load(f.getCanonicalPath());
			} catch (IOException e) {
				Log.warn("Exception trying to load native library", e);
			}
		} else {
			// Embedded libjavartm.so not found, trying to load directly
			System.loadLibrary("javartm");
		}

		RTM_AVAILABLE = rtmAvailable();

		if (!RTM_AVAILABLE) {
			Log.warn("RTM not supported by current CPU. Attempting to use it may lead to JVM crashes");
		}
	}

	/**
	 * Test CPU for RTM support.
	 */
	private native static boolean rtmAvailable();

	private native static boolean n_inTransaction();
	private native static long n_begin();
	private native static void n_commit();

	/**
	 * Abort and set returned status code.
	 * Note that reason MUST FIT as unsigned 8bits [0,255], otherwise it will be set to 0.
	 **/
	private native static void n_abort(long reason);

	private native static <V> V n_doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock);
	
	/**
	 * The public API.
	 **/
	public static short getAbortReason(long txStatus) {
		return (short) (((int) txStatus) >>> 24);
	}
	
	public static boolean inTransaction() {
		if (RTM_AVAILABLE) return n_inTransaction();
		return false; // throw exception?
	}
	
	public static long begin(int tries) {
		if (! RTM_AVAILABLE) return -1; // throw exception? most likely yes!
		
		long status;
		if (tries > 0) {
			do {
				tries--;
				status = n_begin();
			} while(status != STARTED && tries > 0);
			return status;
		} else {
			do {
				status = n_begin();
			} while(status != STARTED);
			return status;
		}
	}
	
	public static long begin() {
		return begin(1);
	}
	
	public static void commit() {
		if (RTM_AVAILABLE) n_commit();
	}
	
	public static void abort() {
		if (RTM_AVAILABLE) n_abort(0L);
	}
	
	public static void abort(short reason) {
		if (reason < 0 || reason > 255) throw new RuntimeException("ui");
		if (RTM_AVAILABLE) n_abort((long) reason);
	}
	
	public static <V> V doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock) {
		if (RTM_AVAILABLE) return n_doTransactionally(atomicBlock, fallbackBlock);
		throw new RuntimeException("lala");
	}
}
