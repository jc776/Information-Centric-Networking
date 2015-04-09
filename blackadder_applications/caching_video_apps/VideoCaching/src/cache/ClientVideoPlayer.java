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

package cache;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

import uk.co.caprica.vlcj.player.MediaPlayer;
import uk.co.caprica.vlcj.player.MediaPlayerFactory;

/**
 * Contains a VLCj MediaPlayer to pass bytes to.
 * @author John Coady
 * @version 07/04/2015
 */
public class ClientVideoPlayer{
	private DatagramSocket ds;
	private MediaPlayer mediaPlayer;
	
	public ClientVideoPlayer() throws SocketException{
		ds= new DatagramSocket();
		
		MediaPlayerFactory mediaPlayerFactory = new MediaPlayerFactory();
		mediaPlayer = mediaPlayerFactory.newEmbeddedMediaPlayer();
		
		// Controlling the delay via the cache instead.
		//MediaListPlayer mediaListPlayer = mediaPlayerFactory.newMediaListPlayer();
		//mediaListPlayer.setMediaPlayer(mediaPlayer);
		
		//MediaList mediaList = mediaPlayerFactory.newMediaList();
		//mediaList.addMedia("vlc://pause:3 "); // First, "play" a delay
		//mediaList.addMedia("udp://@:6666");   // Then, play video from this port
		
		//mediaListPlayer.setMediaList(mediaList);
		//mediaListPlayer.setMode(MediaListPlayerMode.LOOP);
		
		//mediaListPlayer.play();
		
		mediaPlayer.playMedia("udp://@:6666");
	}
	
	public void send(DatagramPacket packet) throws SocketException, IOException
	{
		packet.setAddress(InetAddress.getLocalHost());
		packet.setPort(6666);
		ds.send(packet);
	}
}
