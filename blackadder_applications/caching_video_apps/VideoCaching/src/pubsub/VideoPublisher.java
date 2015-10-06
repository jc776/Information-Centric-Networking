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

import model.VideoStream;

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
public class VideoPublisher{


	private BlackAdderClient client;
	//private Vector <VideoStream> videos;
	private ScopeID superScope;
	private ItemName catName;
	private Strategy strategy;
	//private Vector <ItemName> published;
	private long channelID;
	
	private VideoStream theVideo;
	private ItemName theItem;
	

	public VideoPublisher(BlackAdderClient client, ScopeID superScope, Strategy strategy){
		this.client = client;
		this.superScope = superScope;
		//videos = new Vector<VideoStream>(0,1);
		this.strategy = strategy;
		//published = new Vector<ItemName>(0,1);
		channelID = 1;
	}
	
	public void cleanup(){
		try {
			unpublishTheVideo();
		} catch (DecoderException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		
		/*
		// unpublish the catalog
		try {
			unpublishCatalog();
		} catch (DecoderException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		// unpublish everything else.
		for (ItemName in: published){
			client.unpublishItem(in, strategy);
		}
		*/
	}
	
	public boolean publishTheVideo(String path) throws DecoderException{
		try {
			String catString = NamedItems.getVideoString();
			ByteIdentifier catID = new ByteIdentifier(Hex.decodeHex(catString.toCharArray()));
			catName = new ItemName(superScope, catID);
			client.publishItem(catName, strategy);

			theItem = catName;
			theVideo = new VideoStream(path, getChannelID(), catString);
			
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean unpublishTheVideo() throws DecoderException{
		//String rid = "0000000000000000";
		
		if(theVideo == null)
			return true;
		
		try{
		    //ByteIdentifier newPubID = new ByteIdentifier(Hex.decodeHex(rid.toCharArray()));
			//ItemName newPubName = new ItemName(superScope, newPubID);
			client.unpublishItem(theItem, strategy);
			
			theVideo = null;
			//removeVideoByRid(rid);
			return true;
		}catch (Exception e){
			e.printStackTrace();
			return false;
		}
	}
	
/*
	public boolean publishVideo(String rid, String path) throws DecoderException{
		try{
		    ByteIdentifier newPubID = new ByteIdentifier(Hex.decodeHex(rid.toCharArray()));
			ItemName newPubName = new ItemName(superScope, newPubID);
			client.publishItem(newPubName, strategy);
			// Add to the list.
			published.add(newPubName);
			addVideo(new VideoStream(path, getChannelID(), rid));
			return true;
		}catch (Exception e){
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean unpublishVideo(String rid) throws DecoderException{
		try{
		    ByteIdentifier newPubID = new ByteIdentifier(Hex.decodeHex(rid.toCharArray()));
			ItemName newPubName = new ItemName(superScope, newPubID);
			client.unpublishItem(newPubName, strategy);
			// remove from list
			ItemName marked = null;
			for (ItemName in: published){
				if (Arrays.equals(in.getRendezvousId().getId(), newPubName.getRendezvousId().getId())){
					marked = in;
				}
			}
			if (marked!=null){
				published.remove(marked);
			}
			// remove videostream and from list.
			removeVideoByRid(rid);
			return true;
		}catch (Exception e){
			e.printStackTrace();
			return false;
		}
	}
	private boolean addVideo(VideoStream vs){
		if (!videos.contains(vs)){			
			return videos.add(vs);
		}else{
			return false;
		}
	}

	private boolean removeVideoByRid(String rid){
		VideoStream marker = null;
		for (VideoStream vs: videos){
			if (vs.getrID().equalsIgnoreCase(rid)){
				marker = vs;
			}
		}
		if (marker != null){
			return videos.remove(marker);
		}else{
			return false;
		}
	}
*/

/*
	public boolean publishCatalog() throws DecoderException{
		System.out.println("PUBLISHING THE INITIAL CATALOG");
		try {
			String catString = "0000000000000000";
			ByteIdentifier catID = new ByteIdentifier(Hex.decodeHex(catString.toCharArray()));
			catName = new ItemName(superScope, catID);
			client.publishItem(catName, strategy);

			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}
	
	private boolean unpublishCatalog() throws DecoderException{
		try {
			client.unpublishItem(catName, strategy);
			return true;
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
	}

	public byte[] getCatalog(){
		return catName.toByteArray();
	}
*/
	/**
	 * Retrieve the payload of the catalog
	 * @return the catalog.
	 */
/*
	public String getCatalogNames(){
		String catNames = "";
		// For each video (media) in the catalog
		for (VideoStream vs: videos){
			// embed the rid into the catalog
			catNames +="@" + vs.getrID() + "@";
			// start building the catalog
			catNames += vs.getChannelID() + "    ";	
			// if it's a media stream
			if (vs.getAbsolutePath().startsWith("http")){
				catNames += vs.getAbsolutePath() + "    ";
			}
			// if it's a media file
			else{
				String [] temp = vs.getAbsolutePath().split("/");
				String fileName = temp[temp.length-1];
				String [] parts = fileName.split("\\.");
				catNames += parts[0] + "    ";	
			}
			// insert delimiter for videos.
			catNames += vs.getTimestamp() + "--";
		}
		return catNames;
	}
	*/
	
	public VideoStream getTheVideo()
	{
		return theVideo;
	}
/*
	public VideoStream getVideoStreamByRID(String rid){
		for (VideoStream vs: videos){
			if (vs.getrID().equalsIgnoreCase(rid))
				return vs;
		}
		return null;
	}

	public ItemName getCatName() {
		return catName;
	}
*/
	private String getChannelID(){
		return "" + channelID++;
	}

}
