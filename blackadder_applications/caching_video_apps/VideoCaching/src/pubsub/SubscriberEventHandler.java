/************************************************************************
 * Copyright (C) 2010-2011  Ben Tagger                                  *
 * All rights reserved.                                                 *
 *                                                                      *
 * This program is free software; you can redistribute it and/or        *
 * modify it under the terms of the GNU General Public License version  *
 * 2 as published by the Free Software Foundation.                      *
 *                                                                      *
 * Alternatively, this software may be distributed under the terms of   *
 * the BSD license.                                                     *
 *                                                                      *
 * See LICENSE and COPYING for more details.                            *
 ************************************************************************/

package pubsub;

import java.net.DatagramPacket;
import java.net.SocketException;

import eu.pursuit.core.Event;
import view.SubscriberView;
import cache.IDatagramHandler;
import cache.PacketUIPanel;

// jc776
import java.nio.ByteBuffer;

/**
 * Handles Blackadder subscription events and sends packets to the local cache
 * 
 * @author John Coady
 * @version 07/04/2015
 */
public class SubscriberEventHandler extends Thread {

	private PacketUIPanel panel;
	private SubscriberView gui;
	private IDatagramHandler cache;

	public SubscriberEventHandler(PacketUIPanel panel, SubscriberView gui, IDatagramHandler cache)
			throws SocketException {
		this.panel = panel;
		this.gui = gui;
		this.cache = cache;
	}

	public void run() {
		System.err.println("Processing the Events from the Thread");
		// Process the events...
		while (true) {
			Event event = gui.getClient().getNextEvent();
			// System.err.println("Got " + event.getType());
			switch (event.getType()) {
			case PUBLISHED_DATA:
				// Subscriber receives event.
				// Minimal version: it is a single timestamped packet, whether it's from the cache or the server
				// get the packet and UDP it.
				// try {
				// jc776: Packet = [TIMESTAMP][VIDEO DATAGRAM]

				// Later: Determine source, split Cache publications into
				// packets
				// Send both kinds of packets to a better handler...
				byte[] buffer = event.getDataCopy();
				
				int datagram_size = 1316;
				int timestamp_size = 4;
				int timestamp = ByteBuffer.wrap(buffer).getInt();
				//System.out.println("TIME: " + timestamp);

				DatagramPacket packet = new DatagramPacket(buffer,
						timestamp_size, datagram_size);
				
				// uses "< 0" timestamps to display source: red vs black
				panel.put(timestamp);
				
				boolean fromCache = timestamp < 0;
				if(fromCache)
					timestamp = -timestamp;
				
				cache.put(timestamp, packet);
				// } catch (SocketException e) {
				// // TODO Auto-generated catch block
				// e.printStackTrace();
				// } catch (IOException e) {
				// // TODO Auto-generated catch block
				// e.printStackTrace();
				// }
				event.freeNativeBuffer();
				break;

			case START_PUBLISH:
			case STOP_PUBLISH:
				System.err
						.println("Subscriber: Recieved unexpected publisher-only event "
								+ event.getType().toString());
				break;
			case SCOPE_PUBLISHED:
			case SCOPE_UNPUBLISHED:
				System.err
						.println("Subscriber: Recieved unexpected non-demo event "
								+ event.getType().toString());
				break;
			default:
				System.err.println("Publisher: Recieved unexpected event "
						+ event.getType().toString());
			}
		}
	}

	// public static void main(String args[]) throws SocketException {
	// (new SubscriberEventHandler(null)).start();
	// }
}
