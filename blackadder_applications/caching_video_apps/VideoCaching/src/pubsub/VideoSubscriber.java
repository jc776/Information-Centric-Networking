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

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Hex;

import eu.pursuit.client.BlackAdderClient;
import eu.pursuit.core.ByteIdentifier;
import eu.pursuit.core.ItemName;
import eu.pursuit.core.ScopeID;
import eu.pursuit.core.Strategy;

/**
 * A minimal subscriber that subscribes to a single video.
 * Handles publications.
 * @author John Coady
 * @version - started 13/10/2011
 */
public class VideoSubscriber{
	
	private BlackAdderClient client;
	private ScopeID superScope;
	//private ItemName catName;
	private Strategy strategy;
	
	//private Vector<ItemName> subscribed;
	private ItemName videoItem;
	private ItemName cacheItem;
	
	public VideoSubscriber(BlackAdderClient client, ScopeID superScope, Strategy strategy){
		this.client = client;
		this.superScope = superScope;
		this.strategy = strategy;
		//subscribed = new Vector<ItemName>(0,1);
	}
	
	private ByteIdentifier stringToID(String itemString) throws DecoderException {
		return new ByteIdentifier(Hex.decodeHex(itemString.toCharArray()));
	}
	
	private class Result
	{
		public Result(boolean successful, ItemName item)
		{
			this.successful = successful;
			this.item = item;
		}
		
		public boolean successful;
		public ItemName item;

	}
	
	private Result subscribe(String itemString)
	{
		try {
			// Subscribe to the video
			ByteIdentifier catID = stringToID(itemString);
			ItemName item = new ItemName(superScope, catID);
			client.subscribeItem(item, strategy);
			
			return new Result(true,item);
		} catch (DecoderException e) {
			e.printStackTrace();
			return new Result(false,null);
		}
	}
	
	private boolean unsubscribe(String itemString)
	{
		try {
			// Subscribe from the video
			ByteIdentifier catID = stringToID(itemString);
			ItemName item = new ItemName(superScope, catID);
			client.unsubscribeItem(item, strategy);
			
			return true;
		} catch (DecoderException e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean subscribeTheVideo(){
		Result res = subscribe(NamedItems.getVideoString());
		
		if(res.successful)
			videoItem = res.item;
		
		return res.successful;
	}
	
	public boolean subscribeTheCache(){
		Result res = subscribe(NamedItems.getCacheString());
		
		if(res.successful)
			cacheItem = res.item;
		
		return res.successful;
	}
	
	
	public boolean unsubscribeTheVideo(){
		if(videoItem == null)
			return true;
		
		if(unsubscribe(NamedItems.getVideoString()))
		{
			videoItem = null;
			return true;
		}
		
		return false;
	}
	
	public boolean unsubscribeTheCache(){
		if(cacheItem == null)
			return true;
		
		if(unsubscribe(NamedItems.getCacheString()))
		{
			cacheItem = null;
			return true;
		}
		
		return false;
	}
	
	/**
	 * Removes all subscriptions
	 * @return a boolean indicating success
	 */
	public boolean cleanup()
	{	
		boolean video = this.unsubscribeTheVideo();
		boolean cache = this.unsubscribeTheCache();
		return video && cache;
	}
	
}
