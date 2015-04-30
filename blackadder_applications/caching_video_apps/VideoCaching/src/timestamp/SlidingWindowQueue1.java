package timestamp;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class SlidingWindowQueue1<T> {
	// does not account for timestamp going past INT_MAX
	
	// set(): Puts a new value in, and MIGHT cause one or more outputs (if it's the next expected one, or receiving far-future items)
	
	private ISlidingWindowReceiver<T> receiver;
	
	//private boolean dropFarFuture;
	private int nextTimestamp;
	private final int capacity;
	private final List<T> buffer;
	private int head = 0;
	//private int tail = 0;
	
	public SlidingWindowQueue1(ISlidingWindowReceiver<T> receiver, int capacity, int initialTimestamp)
	{
		this.receiver = receiver;
		// boolean dropFarFuture, 
		//this.dropFarFuture = dropFarFuture;
		this.capacity = capacity;
		this.buffer = new ArrayList<T>(Collections.nCopies(capacity, (T) null));
		
		this.nextTimestamp = initialTimestamp;
		
	}
	
	// receive nextTimestamp: Output it immediately, increment timestamp, check and output next slots if stored.
	// receive nextTimestamp+1 up to nextTimestamp+capacity: Store it, wait.
	// receive x = nextTimestamp+capacity+1 onwards: Skip up until (x - 300), outputting any intermediate values. 
	
	// rushTo x: Skip any ones we don't have up to x, using callback for any stored values.
	
	public SetStatus set(int timestamp, T value)
	{
		int arraySlot = (timestamp - nextTimestamp);
		
		// Drop 'past' values
		if(arraySlot < 0)
			return SetStatus.DROPPED_PAST;
		
		// Received the next expected value
		if(arraySlot == 0)
		{
			// process it immediately, no need to store.
			onValue(value,nextTimestamp);
			incHead();
			
			// now output slot 2+ if they're immediately available
			outputFront();
			return SetStatus.OK_NEXT;
		}
		
		SetStatus status = SetStatus.OK_STORED;
		
		// Received a far-future packet - drop, or skip ahead to receive it
		if(arraySlot >= capacity)
		{
			//if(dropFarFuture)
			//	return SetStatus.DROPPED_FUTURE;
			
			rushTo(timestamp - capacity);
			// Since arraySlot is outside capacity, it should be blank. outputFront can't go past it.
			outputFront();
			arraySlot = (timestamp - nextTimestamp);
			// assert arraySlot < capacity
			
			status = SetStatus.SKIPPED_FUTURE;
		}
		
		int circularSlot = (arraySlot + head) % capacity;
		
		T oldValue = buffer.set(circularSlot,value);
		
		// Notify if this slot was already filled
		// This won't occur at the same time as a 'skip', so only one return code needed.
		if(oldValue != null)
			status = (oldValue.equals(value)) ? SetStatus.OVERWRITE_SAME : SetStatus.OVERWRITE_DIFFERENT;
		
		return status;
	}
	
	public int getNextTimestamp()
	{
		return nextTimestamp;
	}
	
	public List<T> __getBuffer()
	{
		return buffer;
	}
	
	private void incHead()
	{
		head = (++head) % capacity;
		++nextTimestamp;
	}
	
	// Output values until the first blank one
	private void outputFront()
	{
		// set returns the replaced value.
		T value = buffer.set(head,null);
		while(value != null)
		{
			onValue(value,nextTimestamp);
			
			incHead();
			value = buffer.set(head,null);
		}
	}
	
	// Output anything we have to get to at least this slot, skipping empty slots
	private void rushTo(int timestamp)
	{
		while(nextTimestamp < timestamp)
		{
			T value = buffer.set(head,null);
			
			if(value != null)
				onValue(value,nextTimestamp);
			
			incHead();
		}
	}
	
	private void onValue(T value,int timestamp)
	{
		receiver.onValue(value, timestamp);
		//System.out.println("Output #" + timestamp + ":" + value.toString());
	}
	
}
