/*
 * License - see original publisher.h.
 * 
 * Changes:
 * - Packet size to 1500b (Maybe customizable later)
 * - Delay from 1/2sec to 3/5000 sec. (w/ packet size, "Youtube 720p". Videos are fast.)
 * - "Randomly generated info ID" once instead of every event to match Blackadder API comments on "video"
 * - Changed "application startup", "event every 600us" and "first subscriber" for this
 */

#ifndef VIDEO_PUBLISHER_NS3_H
#define VIDEO_PUBLISHER_NS3_H

#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/blackadder-module.h>

//jc776
#include <string>
//"stringify":
#include <sstream>




namespace ns3 {

    /**@brief Extended example publisher to work 'like the video streaming example' and record statistics
     */
    class VideoPublisher : public PubSubApplication {
    public:
        /**@brief As mandated by NS3..no attributes.
         */
        static TypeId GetTypeId(void);
        /**@brief Constructor: it initializes the single NS3 event in the code. The next time at which the publisher will advertise a new information item. 
         */
        VideoPublisher();
        virtual ~VideoPublisher();
        /**@brief The EventListener that is registered with the ServiceModel Class. It asynchronously called when a Blackadder Event is received. 
         * When a START_PUBLISH Event is received, this publisher will immediately (no simulation time is passed) publish the respective dummy data.
         * 
         * @param ev a smart pointer to the received Event.
         */
        void EventHandler(Ptr<Event> ev);
    protected:
        void DoStart(void);
        virtual void DoDispose(void);
    private:
        /**@brief When the publisher is started, it advertises root scope 00000000000 and schedules (at 0 seconds) an NS3 event. 
         * When the event runs, the publisher overwrites the scope's contents and schedules another event.
         * This aims to work like 'Youtube 720p quality/bitrate'
         * 
         */
        virtual void StartApplication(void);
        /**@brief It cancels any scheduled NS3 event.
         */
        virtual void StopApplication(void);
        /**@
         * 
         */
        //jc776: Separate events.
        void change_data(void);
        void advertise_info(void);

        Time m_interval;
        EventId m_Event;
        
	//jc776: Send *changing* data on *constant* info ID.
        // smurf naming in case I'm messing up 'PubSubApplication' or something
        string m_video_channelID; // name taken from (video_streaming's) 'video_publisher.cc'
        double m_video_infoID;    // originally the constant "d1" from (BA+ns3 example's) publisher.cc
        bool m_video_publishing;
	char m_video_data;

    };

} // namespace ns3

#endif
