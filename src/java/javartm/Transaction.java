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

public class Transaction {
	private static final Logger Log = LoggerFactory.getLogger(Transaction.class);

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

		if (!rtmAvailable()) {
			Log.warn("RTM not supported by current CPU. Attempting to use it may lead to JVM crashes");
		}
	}

	public native static boolean rtmAvailable();
	public native static boolean inTransaction();
	public native static int begin();
	public native static void commit();
	public native static void abort();

	/**
	 * Abort and set returned status code.
	 * Note that reason MUST FIT as unsigned 8bits [0,255], otherwise it will be set to 0.
	 **/
	public native static void abort(long reason);

	public native static <V> V doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock);
}
