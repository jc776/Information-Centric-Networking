package view;

import pubsub.VideoCachePublisher;
import eu.pursuit.client.BlackAdderClient;

public interface CacheView {
	public BlackAdderClient getClient();
	public VideoCachePublisher getCachePublisher();
}
