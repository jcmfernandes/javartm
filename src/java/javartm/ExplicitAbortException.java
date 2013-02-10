/*
 * javartm: a Java library for Restricted Transactional Memory
 * Copyright (C) 2013 João Fernandes <mail@joaofernandes.eu>
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
 
public class ExplicitAbortException extends RuntimeException {
	
	private final int reason;
	
	ExplicitAbortException(int reason) {
		this.reason = reason;
	}
	
	public int getReason() {
		return reason;
	}
}
