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

import java.util.concurrent.Callable;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Transaction {
	private static final Logger Log = LoggerFactory.getLogger(Transaction.class);

	static {
		System.loadLibrary("javartm");
		if (!rtmAvailable()) {
			Log.warn("RTM not supported by current CPU. Attempting to use it may lead to JVM crashes");
		}
	}

	public native static boolean rtmAvailable();
	public native static boolean inTransaction();
	public native static int begin();
	public native static void commit();
	public native static void abort();
	//public native static void abort(byte reason);

	public native static <V> V doTransactionally(Callable<V> atomicBlock, Callable<V> fallbackBlock);
}
