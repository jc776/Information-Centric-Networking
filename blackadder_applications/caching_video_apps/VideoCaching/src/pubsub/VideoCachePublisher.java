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

//import java.util.Arrays;
//import java.util.Vector;

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Hex;

import eu.pursuit.client.BlackAdderClient;
import eu.pursuit.core.ByteIdentifier;
import eu.pursuit.core.ItemName;
import eu.pursuit.core.ScopeID;
import eu.pursuit.core.Strategy;


/**
 * Minimal publisher that only publishes a single video.
 * @author John Coady
 */
public class VideoCachePublisher{


	private BlackAdderClient client;
	private ScopeID superScope;
	private ItemName catName;
	private Strategy strategy;
	
	private ItemName theItem;
	

	public VideoCachePublisher(BlackAdderClient client, ScopeID superScope, Strategy strategy){
		this.client = client;
		this.superScope = superScope;
		this.strategy = strategy;
	}
	
	public void cleanup(){
		try {
			unpublishTheCache();
		} catch (DecoderException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
	}
	
	public boolean publishTheCache() throws DecoderException{
		try {
			String catString = NamedItems.getCacheString();
			ByteIdentifier catID = new ByteIdentifier(Hex.decodeHex(catString.toCharArray()));
			catName = new ItemName(superScope, catID);
			client.publishItem(catName, strategy);

			theItem = catName;
			
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean unpublishTheCache() throws DecoderException{
		//String rid = "0000000000000000";
		
		if(theItem == null)
			return true;
		
		try{
			client.unpublishItem(theItem, strategy);
			
			theItem = null;
			return true;
		}catch (Exception e){
			e.printStackTrace();
			return false;
		}
	}
	
	public ItemName getCacheName()
	{
		return theItem;
	}
}
