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
	private ItemName theItem;
	
	public VideoSubscriber(BlackAdderClient client, ScopeID superScope, Strategy strategy){
		this.client = client;
		this.superScope = superScope;
		this.strategy = strategy;
		//subscribed = new Vector<ItemName>(0,1);
	}
	
	public boolean subscribeTheVideo(){	
		try {
			// Subscribe to the video
			String catString = "0000000000000000";
			ByteIdentifier catID = new ByteIdentifier(Hex.decodeHex(catString.toCharArray()));
			theItem = new ItemName(superScope, catID);
			client.subscribeItem(theItem, strategy);
			
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean unsubscribeTheVideo(){
		if(theItem == null)
			return true;
		
		try {
			// Subscribe to the catalog
			String catString = "0000000000000000";
			ByteIdentifier catID = new ByteIdentifier(Hex.decodeHex(catString.toCharArray()));
			theItem = new ItemName(superScope, catID);
			client.unsubscribeItem(theItem, strategy);
			
			theItem = null;
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	/**
	 * Removes all subscriptions
	 * @return a boolean indicating success
	 */
	public boolean cleanup(){	
		try {
			this.unsubscribeTheVideo();
			
			return true;
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			return false;
		}
	}
	
}
