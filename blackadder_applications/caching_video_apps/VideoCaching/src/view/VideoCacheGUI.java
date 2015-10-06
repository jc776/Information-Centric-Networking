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

package view;

import java.awt.EventQueue;

import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import java.awt.BorderLayout;
import java.awt.FlowLayout;

import javax.swing.JButton;

import org.apache.commons.codec.DecoderException;
import org.apache.commons.codec.binary.Hex;

import pubsub.CacheEventHandler;
import pubsub.VideoCachePublisher;
import pubsub.VideoSubscriber;

import util.ProjectPropertiesSingleton;

import eu.pursuit.client.BlackAdderClient;
import eu.pursuit.client.BlackadderWrapper;
import eu.pursuit.core.ByteIdentifier;
import eu.pursuit.core.ScopeID;
import eu.pursuit.core.Strategy;

import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.net.SocketException;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;

/**
 * Cache program: both a publisher and a subscriber
 * @author John Coady
 */
public class VideoCacheGUI implements CacheView, SubscriberView{

	private JFrame frmBlackvidPubsubber;
	
	private ByteIdentifier rootScopeId;
	private BlackAdderClient client;
	private ScopeID rootScope;
	
	private VideoCachePublisher videoCachePublisher;
	private VideoSubscriber videoSubscriber;
	//private long channelID;
	private Strategy strategy = Strategy.DOMAIN_LOCAL;

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					VideoCacheGUI window = new VideoCacheGUI();
					window.frmBlackvidPubsubber.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the application.
	 * @throws DecoderException 
	 * @throws SocketException 
	 */
	public VideoCacheGUI() throws DecoderException, SocketException {
		initialize();		
		
		// Setup the Blackadder environment.
		String sharedObjPath = ProjectPropertiesSingleton.getInstance().getProperty("BAWrapperPath");		
		BlackadderWrapper.configureObjectFile(sharedObjPath);		
		client = BlackAdderClient.getInstance();
		//channelID = 1;
		
		// Initialise the rootscope
		// use the root scope started by the publisher
		String rootScopeStr = "1111111111111111";
        ByteIdentifier rootId = new ByteIdentifier(Hex.decodeHex(rootScopeStr.toCharArray()));
        rootScope = new ScopeID(rootId);
        
		
        // The cache subscribes to the video AND publishes an item
        videoCachePublisher = new VideoCachePublisher(client, rootScope, strategy);
		videoSubscriber = new VideoSubscriber(client, rootScope, strategy);
		
		// Start the event handler
		CacheEventHandler handler = new CacheEventHandler(this,strategy);
		handler.start();
		
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frmBlackvidPubsubber = new JFrame();
		frmBlackvidPubsubber.addWindowListener(new WindowAdapter() {
			@Override
			public void windowClosing(WindowEvent e) {
				// unsubscribe from catalog - just in case
				videoSubscriber.cleanup();
				// and then disconnect.
				client.disconnect();
			}
		});
		frmBlackvidPubsubber.setTitle("Cache");
		frmBlackvidPubsubber.setBounds(100, 100, 450, 300);
		frmBlackvidPubsubber.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frmBlackvidPubsubber.getContentPane().setLayout(new BorderLayout(0, 0));
		
		final JPanel panel = new JPanel();
		frmBlackvidPubsubber.getContentPane().add(panel, BorderLayout.SOUTH);
		panel.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		JButton subscribeButton = new JButton("start cache");
		subscribeButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {	
				// Subscribe to the video and publish the cache
				boolean ok = false;
				try
				{
					ok = videoSubscriber.subscribeTheVideo() && videoCachePublisher.publishTheCache();
				} catch (DecoderException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
					ok = false;
				}
				if(!ok)
					JOptionPane.showMessageDialog(frmBlackvidPubsubber, "Something went wrong starting the cache.");

			}
		});
		panel.add(subscribeButton);
		
		JButton unsubscribeButton = new JButton("stop cache");
		unsubscribeButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				// Unsubscribe from the video and unpublish the cache
				if(!videoSubscriber.unsubscribeTheVideo())
					JOptionPane.showMessageDialog(frmBlackvidPubsubber, "Something went wrong stopping the cache - did you start it first?");
			}
		});
		panel.add(unsubscribeButton);
/*
		list = new List();
		frmBlackvidPubsubber.getContentPane().add(list, BorderLayout.CENTER);
*/
		JPanel panel_1 = new JPanel();
		frmBlackvidPubsubber.getContentPane().add(panel_1, BorderLayout.NORTH);
		panel_1.setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		// Instead, automatic refresh.
/*
		JButton refreshButton = new JButton("Refresh Catalogue");
		refreshButton.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent arg0) {
				// unsubscribe and then subscribe again.
//				videoSubscriber.unsubscribeCatalog();
				videoSubscriber.subscribeCatalog();
			}
		});
		panel_1.add(refreshButton);
*/
	}
	
	//private String getChannelID(){
	//	return "" + channelID++;
	//}

	public ByteIdentifier getRootScopeId() {
		return rootScopeId;
	}

	public void setRootScopeId(ByteIdentifier rootScopeId) {
		this.rootScopeId = rootScopeId;
	}

	public BlackAdderClient getClient() {
		return client;
	}

	public void setClient(BlackAdderClient client) {
		this.client = client;
	}

	public ScopeID getRootScope() {
		return rootScope;
	}

	public void setRootScope(ScopeID rootScope) {
		this.rootScope = rootScope;
	}

	//public void setChannelID(long channelID) {
	//	this.channelID = channelID;
	//}
	
	public void setVideoSubscriber(VideoSubscriber videoSubscriber) {
		this.videoSubscriber = videoSubscriber;
	}
	
	@Override
	public VideoSubscriber getVideoSubscriber() {
		return videoSubscriber;
	}

	@Override
	public VideoCachePublisher getCachePublisher() {
		return videoCachePublisher;
	}
}
