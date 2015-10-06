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
import java.nio.ByteBuffer;
import java.util.List;

import cache.CacheDatagramHandler;
import cache.DatagramPair;

import eu.pursuit.core.Event;
import eu.pursuit.core.Publication;
import eu.pursuit.core.Strategy;
import view.CacheView;

/**
 * The cache is both a publisher and a subscriber.
 */
public class CacheEventHandler extends Thread {

	private CacheView cache;
	private CacheDatagramHandler cacheMemory = new CacheDatagramHandler();
	
	// Necessary, since we publish things now
	private Strategy strategy = Strategy.DOMAIN_LOCAL;
	
	public CacheEventHandler(CacheView cache, Strategy strategy) {
		this.cache = cache;
		this.strategy = strategy;
	}

	public void run() {
		System.err.println("Processing the Events from the Thread");
		// Process the events...
		while (true) {
			//System.err.println("Cache: Getting event...");
			Event event = cache.getClient().getNextEvent();
			//System.err.println("Cache: Got event...");
			//System.err.println(event.getType());
			//byte[] id = event.getId();
			//String idStr = Util.byteArrayToHex(id);// this.getStringFromBytes(id);
			
			//byte[] cat = publisher.getVideoPublisher().getCatalog();
			switch (event.getType()) {
			case START_PUBLISH:
				// Only possible thing is "the cache"
				System.out.println("Publish the cache once.");
				publishCacheDataOnce();
				System.out.println("Finished event");
				break;
			case STOP_PUBLISH:
				// Ignore, we only publish once.
				break;
			case PUBLISHED_DATA:
				// It's a packet, so store it
				
				byte[] buffer = event.getDataCopy();
				
				
				int datagram_size = 1316;
				int timestamp_size = 4;
				int timestamp = ByteBuffer.wrap(buffer).getInt();
				//System.out.println("TIME: " + timestamp);

				DatagramPacket packet = new DatagramPacket(buffer,
						0, timestamp_size + datagram_size); // the whole thing, including timestamp
				cacheMemory.put(timestamp, packet);
				event.freeNativeBuffer();
				break;
			case SCOPE_PUBLISHED:
			case SCOPE_UNPUBLISHED:
				System.err.println("Cache: Recieved unexpected non-demo event " + event.getType().toString());
				break;
			default:
				System.err.println("Cache: Recieved unexpected event " + event.getType().toString());
			}
		}
	}
	
	private void publishCacheDataOnce()
	{
		// Get the catalog data
		List<DatagramPair> contents = cacheMemory.getContents();
		
		int timestamp_size = 4;
		int datagram_size = 1316;
		//DatagramPacket p = null;
		ByteBuffer pub_buffer = ByteBuffer.allocate(timestamp_size + datagram_size);
		pub_buffer.clear();
		
		// Lots of 1-packet responses
		// Ordering vs Server's ones?
		
		for(DatagramPair pair : contents)
		{
			byte[] data = pair.packet.getData();
			System.out.println("Cache sending: " + pair.timestamp + " len. " + data.length + "," + pair.packet.getLength());
			// Ready for writing from the start
			pub_buffer.clear();
			// write the same timestamp but negative
			// marking "is from cache" (red lines on Client UI)
			pub_buffer.putInt(-pair.timestamp);
			// Write the data, not including the non-neg. timestamp (overflow)
			pub_buffer.put(pair.packet.getData(),timestamp_size,datagram_size);
			// Ready for reading what we've just written
			pub_buffer.flip();
			
			Publication pub = new Publication(cache
					.getCachePublisher().getCacheName(),
					pub_buffer.array());
			
			cache.getClient().publishData(pub, strategy);
			System.out.println("Sent: " + pair.timestamp);
		}
		System.out.println("Done sending.");
	}
}
