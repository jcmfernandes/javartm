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

import static javartm.TransactionStatus.*;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Map;
import java.util.WeakHashMap;
import java.util.concurrent.Callable;
import java.util.concurrent.Semaphore;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public final class Transaction {
	private static final Logger Log = LoggerFactory.getLogger(Transaction.class);

	private static final boolean RTM_SUPPORT;
	
	private static final Map<Object, Semaphore> SEMAPHORES = new WeakHashMap<Object, Semaphore>();

	private static final RTMException NO_RTM_SUPPORT = new RTMException("RTM not supported by current CPU");
	private static final RTMException NO_ACTIVE_TX = new RTMException("No active transaction");

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

		RTM_SUPPORT = n_rtmAvailable();
		if (! RTM_SUPPORT) {
			Log.warn("RTM not supported by current CPU. Attempting to use it may lead to JVM crashes");
		}
	}

	/**
	 * The private, native API.
	 */

	// Test CPU for RTM support
	private native static boolean n_rtmAvailable();
	// Test if there is an active transaction
	private native static boolean n_inTransaction();

	private native static int n_begin();
	private native static boolean n_commit();
	/**
	 * Abort and set returned status code.
	 * Note that reason MUST FIT as unsigned 8bits [0,255], otherwise it will be set to 0.
	 **/
	private native static boolean n_abort(long reason);

	//private native static <V> V n_doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock);

	/**
	 * The public API.
	 **/
	public static boolean inTransaction() {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;

		return n_inTransaction();
	}

	public static void begin() {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;
		
		int txStatus;
		if ((txStatus = n_begin()) == STARTED) {
			return;
		} else if (! isFlagged(txStatus, ABORT_EXPLICIT) ||
				(isFlagged(txStatus, ABORT_EXPLICIT) && getAbortReason(txStatus) == 0xFF)) {
			throw new CommitException(txStatus);
		}
	}

	public static void commit() {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;

		final boolean txStatus = n_commit();
		if (! txStatus) throw NO_ACTIVE_TX;
	}

	public static void abort(short reason) {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;

		if (reason < 0 || reason >= 255) {
			throw new RTMException("Abort reason has to be in the range [0;255[ but " + reason + " was passed");
		}
		final boolean txStatus = n_abort((long) reason);
		if (! txStatus) throw NO_ACTIVE_TX;
	}

	public static void abort() {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;

		final boolean txStatus = n_abort(0L);
		if (! txStatus) throw NO_ACTIVE_TX;
	}
	
	private static synchronized Semaphore getSemaphoreForAtomicBlock(Callable<?> atomicBlock) {
		Semaphore semaphore;
		if ((semaphore = SEMAPHORES.get(atomicBlock)) == null) {
			semaphore = new Semaphore(1, true);
			SEMAPHORES.put(atomicBlock, semaphore);
		}
		
		return semaphore;
	}
	
	public static <V> V doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock) throws Exception {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;
		
		if (atomicBlock == fallbackBlock) {
			throw new RTMException("atomicBlock and fallbackBlock can't reference the same object.");
		}
		
		while (true) {
			try {
				Transaction.begin();
				V result = null;
				try {
					result = atomicBlock.call();
				} catch (Throwable t) {
					// we just can't do anything with the Throwable object
					n_abort(0xFF);
				}
				Transaction.commit();
				return result;
			} catch (CommitException e) {
				if (isFlagged(e.getTxStatus(), ABORT_RETRY)) {
					// take advantage of the hardware hint
					continue;
				} // else
				return fallbackBlock.call();
			}
		}
	}

	public static <V> V doTransactionally(Callable<V> atomicBlock) throws Exception {
		if (! RTM_SUPPORT) throw NO_RTM_SUPPORT;
		
		final Semaphore semaphore = getSemaphoreForAtomicBlock(atomicBlock);
		while (true) {
			try {
				Transaction.begin();
				if (semaphore.availablePermits() == 0) {
					/* Started transaction but some other thread is executing
					 * Callable atomicBlock non-speculatively, and so we abort.
					 */
					n_abort(0xFF);
				} // else...

				V result = null;
				try {
					result = atomicBlock.call();
				} catch (Throwable t) {
					// we just can't do anything with the Throwable object
					n_abort(0xFF);
				}
				Transaction.commit();
				return result;
			} catch (CommitException e) {
				if (isFlagged(e.getTxStatus(), ABORT_RETRY)) {
					// take advantage of the hardware hint
					continue;
				} // else
				semaphore.acquireUninterruptibly();
				try {
					return atomicBlock.call();
				} finally {
					semaphore.release();
				}
			}
		}
	}
	
}
