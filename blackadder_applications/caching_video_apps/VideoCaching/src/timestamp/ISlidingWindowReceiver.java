package timestamp;

public interface ISlidingWindowReceiver<T> {
	public void onValue(T value, int timestamp);
}
