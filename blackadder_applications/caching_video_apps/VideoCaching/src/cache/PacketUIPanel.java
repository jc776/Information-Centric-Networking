package cache;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;

import javax.swing.BorderFactory;
import javax.swing.JPanel;

public class PacketUIPanel extends JPanel{
	private static final long serialVersionUID = 1L;
	
	final int LENGTH = 430;
	int[] items = new int[LENGTH];
	int head = 0;
	
	public void put(int timestamp)
	{
		items[head] = timestamp;
		head = (++head) % LENGTH;
		// bluh, could optimise
		repaint();
	}
	
    public PacketUIPanel() {
        setBorder(BorderFactory.createLineBorder(Color.black));
    }

    public Dimension getPreferredSize() {
        return new Dimension(250,200);
    }

    public void paintComponent(Graphics g) {
        super.paintComponent(g);       

        int item;
        for(int i=0;i<LENGTH;++i)
        {
        	item = items[i];
        	int length = (item % 150);
        	g.setColor(item < 0 ? Color.RED : Color.BLACK);
        	g.drawLine(10+i,100,10+i,100+length);
        }
    }  
}
