/*
 * License - see original publisher.cc.
 * 
 * Changes - See header file.
 */

#include "ns3_video_publisher_app.h"

namespace ns3 {

    NS_LOG_COMPONENT_DEFINE("VideoPublisher");
    NS_OBJECT_ENSURE_REGISTERED(VideoPublisher);

    TypeId VideoPublisher::GetTypeId(void) {
        static TypeId tid = TypeId("ns3::VideoPublisher")
                .SetParent<PubSubApplication > ()
                .AddConstructor<VideoPublisher > ();
        return tid;
    }

    VideoPublisher::VideoPublisher() {
        //jc776: Constant info ID.
        UniformVariable part1;
        m_infoID = part1.GetValue();
        
        m_Event = EventId();
    }

    VideoPublisher::~VideoPublisher() {
    }

    void VideoPublisher::DoStart(void) {
        NS_LOG_FUNCTION(this);
        m_cb = MakeCallback(&VideoPublisher::EventHandler, this);
        PubSubApplication::DoStart();
    }

    void VideoPublisher::DoDispose(void) {
        NS_LOG_FUNCTION(this);
        PubSubApplication::DoDispose();
    }

    void VideoPublisher::StartApplication(void) {
        NS_LOG_FUNCTION(this);
        PubSubApplication::StartApplication();
        std::string id = "0000000000000000";
        std::string prefix_id;
        std::string bin_id = hex_to_chararray(id);
        std::string bin_prefix_id = hex_to_chararray(prefix_id);
        publish_scope(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0);
        m_Event = Simulator::Schedule(Seconds(0.), &VideoPublisher::publish, this);
    }

    void VideoPublisher::StopApplication() {
        NS_LOG_FUNCTION(this);
        Simulator::Cancel(m_Event);
        PubSubApplication::StopApplication();
    }

    void VideoPublisher::publish(void) {
        NS_LOG_FUNCTION(this);
        NS_ASSERT(m_Event.IsExpired());
        std::string prefix_id = "0000000000000000";

	//jc776: Constant info ID - these are now member variables.
        //UniformVariable part1;
        //double d1;
        //d1 = part1.GetValue();

        char *testID = (char *) malloc(8);
        memcpy(testID, &m_infoID, sizeof (double)); //jc776: "d1"
        std::string FINALID(testID, sizeof (double));
        std::string bin_prefix_id = hex_to_chararray(prefix_id);
        publish_info(FINALID, bin_prefix_id, DOMAIN_LOCAL, NULL, 0);
        free(testID);
	
	//NS_LOG_INFO("Millisecond.");
	
	//jc776: 500ms -> 3/5000s = 3/5ms = 600us
        m_Event = Simulator::Schedule(MicroSeconds(600), &VideoPublisher::publish, this);
    }

    void VideoPublisher::EventHandler(Ptr<Event> ev) {
        char *payload;
        switch (ev->type) {
            case SCOPE_PUBLISHED:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": SCOPE_PUBLISHED: " << chararray_to_hex(ev->id));
                break;
            case SCOPE_UNPUBLISHED:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": SCOPE_UNPUBLISHED: " << chararray_to_hex(ev->id));
                break;
            case START_PUBLISH:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": START_PUBLISH: " << chararray_to_hex(ev->id));
		//jc776: Packet from "1000 A's" to "1316 bytes of ID"
		m_data = (m_data + 1) % 26;
		
                payload = (char *) malloc(1316);
                memset(payload, 'A'+m_data, 1316);
                publish_data(ev->id, DOMAIN_LOCAL, NULL, 0, payload, 1500);
                free(payload);
                break;
            case STOP_PUBLISH:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": STOP_PUBLISH: " << chararray_to_hex(ev->id));
                break;
            case PUBLISHED_DATA:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": PUBLISHED_DATA: " << chararray_to_hex(ev->id));
                break;
        }
    }

} // Namespace ns3
