package view;

import pubsub.VideoSubscriber;
import eu.pursuit.client.BlackAdderClient;

public interface SubscriberView {
	public BlackAdderClient getClient();
	public VideoSubscriber getVideoSubscriber();
	public void populateCatalogList(String content);

}
