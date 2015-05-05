package io.jc776.blackadder.caching;

import java.awt.EventQueue;

//import eu.pursuit.core.*;

public class Main {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
        EventQueue.invokeLater(new Runnable() {
            
            @Override
            public void run() {
                ClientWindow gui = new ClientWindow();
                gui.setVisible(true);
            }
        });
	}

}
