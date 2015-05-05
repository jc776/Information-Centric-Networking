package cache;

/**
 * Buffers datagrams & cache's multiple-datagram packets, outputting to video player
 * @author John Coady
 * @version 07/04/2015
 */
 
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.Timer;
import java.util.TimerTask;
import java.net.DatagramPacket;

import java.io.IOException;
import java.net.SocketException;
 
public class CacheDatagramHandler extends Thread implements IDatagramHandler
{
	private ClientVideoPlayer player;
	private BlockingQueue<DatagramPacket> queue;
	
	private boolean started;
	private Timer timer;
	
	public CacheDatagramHandler(ClientVideoPlayer player)
	{
		this.player = player;
		
		queue = new LinkedBlockingQueue<DatagramPacket>();
		timer = new Timer();
	}
	
	// When I first recieve a packet, wait three seconds, then start the thread.
	public synchronized void put(int timestamp, DatagramPacket packet)
	{
		// synchronized for 'first packet starts the timer'
		// not needed for 'queue'.
		if(!started)
		{
			timer.schedule(new TimerTask()
			{
				@Override
		  		public void run()
		  		{
		  			CacheDatagramHandler.this.start();
				}
			}, 3*1000);
			
			started = true;
		}
		
		try
		{
			queue.put(packet);
		}
		catch(InterruptedException ex)
		{
			// Not sure if expecting this whenever, or on end, or on error...
			System.out.println("ClientDatagramCache: Queue interrupted.");
			Thread.currentThread().interrupt();
		}
	}
	
	// Thread: consume items from the queue as fast as possible
	public void run()
	{
		try
		{
			while (true)
			{
				consume(queue.take());
			}
		}
		catch(InterruptedException ex)
		{
			// Not sure if expecting this whenever, or on end, or on error...
			System.out.println("ClientDatagramCache: Queue interrupted.");
			Thread.currentThread().interrupt();
		}
	}
	
	// Send a packet to the video player
	void consume(DatagramPacket packet)
	{
		// I'd prefer the program to end on exceptions here?
		try
		{
			player.send(packet);
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
