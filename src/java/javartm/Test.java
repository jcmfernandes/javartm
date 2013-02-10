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

public class Test {
	public static int x = 0;
	public static int y = 0;

	public static void main(String[] args) {;
		boolean res = false;

		// --

		System.out.println("Trying to perform transaction (using doTransactionally)");

		try {
		res = Transaction.doTransactionally(new Callable<Boolean>() {
			public Boolean call() {
				x = 1;
				y = 1;
				return true;
			}
		}, new Callable<Boolean>() {
			public Boolean call() {
				return false;
			}
		});
		} catch (Exception e) { }

		System.out.println("Transaction result: " + res + " (x: " + x + ", y: " + y + ")");

		// --

		System.out.println("Testing abort (using doTransactionally)");

		try {
		res = Transaction.doTransactionally(new Callable<Boolean>() {
			public Boolean call() {
				Transaction.abort();
				throw new AssertionError("Should never happen");
			}
		}, new Callable<Boolean>() {
			public Boolean call() {
				System.out.println("fallbackBlock!");
				return true;
			}
		});
		} catch (Exception e) { }

		System.out.println("Transaction result: " + res + " (x: " + x + ", y: " + y + ")");

		// --

		System.out.println("Trying to perform transaction (using manual API)");

		res = false;

		int txStatus = Transaction.begin();
		if (txStatus == Transaction.STARTED) {
			x = 2;
			y = 2;
			res = true;
			Transaction.commit();
		}

		System.out.println("Transaction result: " + res + " (x: " + x + ", y: " + y + ", txStatus: " + txStatus + ")");

		// --

		System.out.println("Trying to commit without an active transaction...");
		try {
			Transaction.commit();
			throw new AssertionError("Should never happen");
		} catch (TransactionException e) {
			System.out.println("Successfully got an exception...");
			e.printStackTrace();
		}

		// --

		System.out.println("Trying to abort without an active transaction...");
		try {
			Transaction.abort();
			System.out.println("Error: After abort!");
		} catch (TransactionException e) {
			System.out.println("Successfully got an exception...");
			e.printStackTrace();
		}

		// --

		System.out.println("Trying abort(0 -> 255)");
		int statusZero = 0;
		int retry = 0;
		for (int i = 0; i <= 255; i++) {
			txStatus = Transaction.begin();
			if (txStatus == Transaction.STARTED) {
				Transaction.abort((short) i);
			}
			if (((txStatus & Transaction.ABORT_EXPLICIT) != 0) && (Transaction.getAbortReason(txStatus) != i)) {
				System.out.println("Unexpected txStatus for i: " + i + " txStatus: " + txStatus);
			} else if (txStatus == 0) {
				statusZero++;
			} else if ((txStatus & Transaction.ABORT_RETRY) != 0) {
				retry++;
			} else if ((txStatus & Transaction.ABORT_EXPLICIT) == 0) {
				System.out.println("Unexpected txStatus for i: " + i + " txStatus: " + txStatus + " (bit 0 unset)");
			}
		}
		System.out.println("abort(0 -> 255) results: " + (255 - statusZero - retry) + " correct status codes; " +
					statusZero + " times status == 0; " + retry + " may retry");

		// --

		System.out.println("Finished testing!");
	}
}
