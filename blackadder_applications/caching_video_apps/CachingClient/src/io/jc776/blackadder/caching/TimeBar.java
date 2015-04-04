package io.jc776.blackadder.caching;

import java.awt.Graphics;
import java.util.List;

import javax.swing.JPanel;

public class TimeBar extends JPanel
{
	private static final long serialVersionUID = 1L;
	
	private List<Packet> packets;
	private int now;
	private int end;
	
	public TimeBar(List<Packet> _packets, int _now, int _end)
	{
		packets = _packets;
		now = _now;
		end = _end;
	}
	
	public void paint(Graphics g)
	{
        super.paint(g);
        //int frameWidth = this.getWidth() / listSize;
        
        // width / (end - now)
        int interval = (end - now);
        int unit = this.getWidth() / interval;
        
        for(Packet packet : packets)
        {
            g.fillRect(packet.startTime * unit,
            		0,
            		packet.length * unit,
            		this.getHeight());
        }
    }
}
