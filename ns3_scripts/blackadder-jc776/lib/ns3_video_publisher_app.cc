/*
 * License - see original publisher.cc.
 * 
 * Changes - See header file.
 */

#include "ns3_video_publisher_app.h"

std::string stringify(double x)
 {
   std::ostringstream o;
   if (!(o << x))
     throw "stringify(double)";
   return o.str();
 }

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
        m_Event = EventId();
        
        //jc776: Constant info ID.
        UniformVariable part1;
        m_video_infoID = part1.GetValue();
        //jc776: "video-like" publishing
        m_video_publishing = false;
        m_video_channelID = "";
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
        //jc776: Publish scope 0 ('contains all the videos')
        NS_LOG_FUNCTION(this);
        PubSubApplication::StartApplication();


        std::string id = "0000000000000000";
        std::string prefix_id; // "blank" - This is the top level scope
        std::string bin_id = hex_to_chararray(id);
        std::string bin_prefix_id = hex_to_chararray(prefix_id);
        publish_scope(bin_id, bin_prefix_id, DOMAIN_LOCAL, NULL, 0);
        
        Simulator::Schedule(Seconds(2.), &VideoPublisher::advertise_info, this);
        m_Event = Simulator::Schedule(Seconds(0.), &VideoPublisher::change_data, this);
    }
    
    void VideoPublisher::advertise_info(void) {
        NS_LOG_FUNCTION(this);
        std::string prefix_id = "0000000000000000"; // This is immediately under the top level scope
        std::string bin_prefix_id = hex_to_chararray(prefix_id);
        
        //jc776: ... and one information item of (one-time) random ID ('the video')
        char *testID = (char *) malloc(8);
        memcpy(testID, &m_video_infoID, sizeof (double)); //jc776: "d1" from publisher.cc - now uses member variable
        std::string FINALID(testID, sizeof (double));
        
        NS_LOG_INFO("Info ID: " << m_video_infoID << " -> " << chararray_to_hex(FINALID));
        //std::string bin_prefix_id = hex_to_chararray(prefix_id); // already calculated above
        publish_info(FINALID, bin_prefix_id, DOMAIN_LOCAL, NULL, 0);
        free(testID);
    }

    void VideoPublisher::StopApplication() {
        NS_LOG_FUNCTION(this);
        Simulator::Cancel(m_Event);
        PubSubApplication::StopApplication();
    }

    void VideoPublisher::change_data(void) {
        //jc776: These events should really only happen 'while publishing' i.e. while people are subscribed.
        // This is to simplify logic/minimize changes.
        NS_LOG_FUNCTION(this);
        NS_ASSERT(m_Event.IsExpired());

        if(m_video_publishing)
        {
                //jc776: The packet is 1316 of the same letter, changing when the event is called (loops 'A' to 'Z')
                // This is slightly less than 'Youtube 720p' data rate.

	        m_video_data = (m_video_data + 1) % 26;
                char *payload = (char *) malloc(1316);
                memset(payload, 'A'+m_video_data, 1316);
                publish_data(m_video_channelID, DOMAIN_LOCAL, NULL, 0, payload, 1000);
                free(payload);
	
	        NS_LOG_INFO("Payload letter " << string(1,'A'+m_video_data));
        }
	
	//jc776: 500ms changed to 3/5000s = 3/5ms = 600us

        m_Event = Simulator::Schedule(MicroSeconds(600), &VideoPublisher::change_data, this);
    }

    void VideoPublisher::EventHandler(Ptr<Event> ev) {
        switch (ev->type) {
            case SCOPE_PUBLISHED:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": SCOPE_PUBLISHED: " << chararray_to_hex(ev->id));
                break;
            case SCOPE_UNPUBLISHED:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": SCOPE_UNPUBLISHED: " << chararray_to_hex(ev->id));
                break;
            case START_PUBLISH:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": START_PUBLISH: " << chararray_to_hex(ev->id));
		
                //jc776
                m_video_publishing = true;   // Start publishing changing data
                m_video_channelID = ev->id;  // In response to 'this' event
                m_video_data = 0;            // Start with 'A'

                break;
            case STOP_PUBLISH:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": STOP_PUBLISH: " << chararray_to_hex(ev->id));
                m_video_publishing = false;
                
                break;
            case PUBLISHED_DATA:
                NS_LOG_INFO("Node " << GetNode()->GetId() << ": PUBLISHED_DATA: " << chararray_to_hex(ev->id));
                break;
        }
    }

} // Namespace ns3
