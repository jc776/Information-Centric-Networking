package cache;

/**
 * Stores ~1 sec of packets at a time
 * @author John Coady
 * @version 07/04/2015
 */

import java.util.ArrayList;
import java.util.List;
import java.net.DatagramPacket;
 
public class CacheDatagramHandler implements IDatagramHandler
{
	private ArrayList<DatagramPair> cache = new ArrayList<DatagramPair>(2000);
	
	public CacheDatagramHandler()
	{
	}
	
	// When I receive a packet, put it in the list
	// Remove the oldest elements when it gets full
	public synchronized void put(int timestamp, DatagramPacket packet)
	{
		cache.add(new DatagramPair(timestamp,packet));
		
		if(cache.size() == 2000)
		{
			// remove the oldest elements
			cache.subList(0, 750).clear();
		}
	}
	
	@SuppressWarnings("unchecked") // cache is always a List<DP> so clone result should be.
	public synchronized List<DatagramPair> getContents()
	{
		return (List<DatagramPair>) cache.clone();
	}
}
