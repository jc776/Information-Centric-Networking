package io.jc776.blackadder.caching;

import java.util.LinkedList;

import javax.swing.JFrame;

public class GUI extends JFrame {
	private static final long serialVersionUID = 1L;

	public GUI()
	{
		initUI();
	}
	
    private void initUI() {
    	// Window
        setTitle("Simple example");
        setSize(300, 200);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
        
        // Contents
		LinkedList<Packet> list = new LinkedList<Packet>();
		list.add(new Packet(120,10));
		list.add(new Packet(40,10));
		list.add(new Packet(50,20));
		list.add(new Packet(60,40));
		list.add(new Packet(80,10));
		
		TimeBar timeBar = new TimeBar(list,0,200);
		timeBar.setSize(180, 20);
		
		this.add(timeBar);
    }
}
