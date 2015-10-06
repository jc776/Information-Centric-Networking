package cache;

import java.net.DatagramPacket;

public class DatagramSource
{
	boolean fromCache;
	public DatagramPacket packet;
	
	public DatagramSource(boolean fromCache, DatagramPacket packet)
	{
		this.fromCache = fromCache;
		this.packet = packet;
	}
}