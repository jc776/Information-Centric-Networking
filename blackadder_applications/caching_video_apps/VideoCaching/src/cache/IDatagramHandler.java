package cache;

import java.net.DatagramPacket;

/*
 * 'Is an object that can receive timestamped datagrams'
 */
public interface IDatagramHandler {

	void put(int timestamp, DatagramPacket packet);

}
