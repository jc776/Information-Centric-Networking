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

import timestamp.SlidingWindowQueue1;
import timestamp.ISlidingWindowReceiver;
import timestamp.SetStatus;

public class ClientDatagramHandler extends Thread implements
		ISlidingWindowReceiver<DatagramPacket>, IDatagramHandler {
	private ClientVideoPlayer player;
	
	private BlockingQueue<DatagramPacket> playerQueue;
	private SlidingWindowQueue1<DatagramPacket> frameWindow;

	private boolean started;
	private Timer timer;

	public ClientDatagramHandler(ClientVideoPlayer player) {
		this.player = player;

		playerQueue = new LinkedBlockingQueue<DatagramPacket>();
		frameWindow = new SlidingWindowQueue1<DatagramPacket>(this, 3000, 0);
		timer = new Timer();
	}

	// Thread: consume items from the queue as fast as possible
	public void run() {
		try {
			while (true) {
				consume(playerQueue.take());
			}
		} catch (InterruptedException ex) {
			// Not sure if expecting this whenever, or on end, or on error...
			System.out.println("ClientDatagramCache: Queue interrupted.");
			Thread.currentThread().interrupt();
		}
	}

	// Send a packet to the video player
	void consume(DatagramPacket packet) {
		// I'd prefer the program to end on exceptions here?
		try {
			player.send(packet);
		} catch (SocketException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	// IDatagramCache
	// When I first recieve a packet, wait three seconds, then start the thread.
	// Put the packet into the sliding-window queue.
	public synchronized void put(int timestamp, DatagramPacket packet) {
		// synchronized for 'first packet starts the timer'
		// not needed for 'queue'.
		if (!started) {
			timer.schedule(new TimerTask() {
				@Override
				public void run() {
					ClientDatagramHandler.this.start();
				}
			}, 3 * 1000);

			started = true;
		}

		SetStatus setStatus = frameWindow.set(timestamp,packet);
		String status = "#" + timestamp + ": ";
		
		switch(setStatus)
		{
			case DROPPED_PAST:
			case OVERWRITE_SAME:
				status += "Already seen.";
				break;
			case OK_NEXT:
			case OK_STORED:
				status += "OK.";
				break;
			case OVERWRITE_DIFFERENT:
				status += "ERROR: Differs to already-received copy: ";
				break;
			case SKIPPED_FUTURE:
				status += "Far ahead or frames dropped, skipping to catch up.";
				break;
			default:
				status += "ERROR: Interface changed.";
				break;
		}
		
		System.out.println(status);
	}

	// ISlidingWindowReceiver
	@Override
	public void onValue(DatagramPacket value, int timestamp) {
		System.out.println("Processed: " + timestamp);
		try {
			playerQueue.put(value);
		} catch (InterruptedException ex) {
			// Not sure if expecting this whenever, or on end, or on error...
			System.out.println("ClientDatagramCache: Queue interrupted.");
			Thread.currentThread().interrupt();
		}

	}
}
