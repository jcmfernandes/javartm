package javartm;

class TransactionStatus {

	public static final int STARTED		= 0xFFFFFFFF;

	public static final int ABORT_EXPLICIT 	= 1 << 0;
	public static final int ABORT_RETRY 	= 1 << 1;
	public static final int ABORT_CONFLICT 	= 1 << 2;
	public static final int ABORT_CAPACITY 	= 1 << 3;
	public static final int ABORT_DEBUG 	= 1 << 4;
	public static final int ABORT_NESTED 	= 1 << 5;
	
	public static short getAbortReason(int status) {
		return (short) (status >>> 24);
	}
	
	public static boolean isFlagged(int txStatus, int flag) {
		return (txStatus & flag) == flag ? true : false;
	}
	
	private TransactionStatus() {}
	
}
