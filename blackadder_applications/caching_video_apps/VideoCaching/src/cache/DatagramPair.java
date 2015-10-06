package cache;

import java.net.DatagramPacket;

public class DatagramPair
{
	public int timestamp;
	public DatagramPacket packet;
	
	public DatagramPair(int timestamp, DatagramPacket packet)
	{
		this.timestamp = timestamp;
		this.packet = packet;
	}
}