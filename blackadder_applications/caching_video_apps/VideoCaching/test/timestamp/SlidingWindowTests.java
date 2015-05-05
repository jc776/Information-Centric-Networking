package timestamp;

import static org.junit.Assert.*;

import java.util.Arrays;
import java.util.LinkedList;
import java.util.List;

import org.junit.Before;
import org.junit.Test;

import static org.hamcrest.CoreMatchers.*;

public class SlidingWindowTests {
	private class Receiver implements ISlidingWindowReceiver<Integer>
	{
		public List<Integer> received = new LinkedList<Integer>();
		
		@Override
		public void onValue(Integer value, int timestamp) {
			received.add(value);
		}
		
	}
	
	Receiver receiver;
	SlidingWindowQueue1<Integer> queue;

	@Before
	public void setUp() throws Exception {
		// capacity 10, start at 0
		receiver = new Receiver();
		queue = new SlidingWindowQueue1<Integer>(receiver,10,0);
	}

	@Test
	public void testSimple() {
		// Insert a few values
		queue.set(0, 1);
		queue.set(1, 2);
		queue.set(2, 3);
		
		// Check we received all of them
		assertThat(receiver.received, is(equalTo(Arrays.asList(1, 2, 3))));
	}
	
	@Test
	public void testWaiting() {
		// Insert values with a gap
		queue.set(0, 1);
		queue.set(1, 2);
		queue.set(3, 4);
		
		// Check we received only the ones before the gap
		assertThat(receiver.received, is(equalTo(Arrays.asList(1, 2))));
		
		// Fill the gap
		queue.set(2, 3);
		
		// Check we received all of them
		assertThat(receiver.received, is(equalTo(Arrays.asList(1, 2, 3, 4))));
	}

	@Test
	public void testSkipping() {
		queue.set(1, 2);
		queue.set(3, 4);
		
		// Contents
		// [ ][2][ ][4][ ][ ][ ][ ][ ][ ]
		//  ^
		//  next
		
		// We're waiting for the first gap
		assertThat(queue.getNextTimestamp(),is(0));
		assertTrue(receiver.received.isEmpty());
		
		// Insert a value that's past the capacity
		queue.set(12, 13);
		
		//System.out.println(queue.__getBuffer());
		
		// We skip past the first gap to fit this new value in.
		// (12 >= 10, so skip to (12 - 10) = 2)
		// It moves nextTimestamp forwards so that the new value is at the end (start + capacity) of the buffer
		// (13)[4][ ][ ][ ][ ][ ][ ][ ][ ][13]
		//  ^
		
		// The capacity-slot buffer is actually a circular buffer. The 13s are the same.
		// The slot being pointed to is the next expected timestamp, but won't be overwritten when that arrives.
		// [] "next 10"
		// () "previous 1"
		
		assertThat(queue.getNextTimestamp(),is(2));
		assertThat(receiver.received, is(equalTo(Arrays.asList(2))));
		//System.out.println(receiver.received.toString());
		
		// The next expected timestamp (TS 2) is input and received without being stored on top of the "13"
		queue.set(2, 3);
		
		// TS 3 (value 4) is present, so is also received.
		
		//  [ ][ ][ ][ ][ ][ ][ ][ ][13][ ]
		//   ^
		assertThat(queue.getNextTimestamp(),is(4));
		assertThat(receiver.received, is(equalTo(Arrays.asList(2,3,4))));
		
		queue.set(4,5);
		queue.set(5,6);
		queue.set(6,7);
		queue.set(7,8);
		queue.set(8,9);
		queue.set(9,10);
		queue.set(10,11);
		queue.set(11,12);
		
		//  13
		//  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][ ]
		//   ^
		assertThat(queue.getNextTimestamp(),is(13));
		// The value '13' gets received, so wasn't overwritten.
		assertThat(receiver.received, is(equalTo(Arrays.asList(2,3,4,5,6,7,8,9,10,11,12,13))));
	}
	
	@Test
	public void testSkippingFar() {
		queue.set(1, 2);
		queue.set(3, 4);
		
		queue.set(100, 101);
		
		// We've skipped up until (new timestamp - capacity) = (100 - 10) = 90
		// Contents:
		//  90
		//  [ ][ ][ ][ ][ ][ ][ ][ ][ ][ ][101]
		//   ^
		assertThat(queue.getNextTimestamp(),is(90));
		// We've output the two values we rushed past
		assertThat(receiver.received, is(equalTo(Arrays.asList(2,4))));
		
		// We can't fill in a TS that's been skipped
		assertThat(queue.set(89, 90),is(SetStatus.DROPPED_PAST));
		
		// We can fill in the middle of the current contents
		queue.set(91,92);
		queue.set(92,93);
		// We can fill in the start of the current contents, which triggers receiving TS 90 and 91
		queue.set(90,91);
		
		assertThat(receiver.received, is(equalTo(Arrays.asList(2,4,91,92,93))));
	}
	
	@Test
	public void testStatus() {
		assertThat(queue.set(0, 1),is(SetStatus.OK_NEXT));
		assertThat(queue.set(0, 10),is(SetStatus.DROPPED_PAST));
		
		assertThat(queue.set(2, 3),is(SetStatus.OK_STORED));
		assertThat(queue.set(2, 3),is(SetStatus.OVERWRITE_SAME));
		assertThat(queue.set(2, 4),is(SetStatus.OVERWRITE_DIFFERENT));
		
		assertThat(queue.set(12,13),is(SetStatus.SKIPPED_FUTURE));
	}
}
 