//
// Created by hemin on 8/19/15.
//

#include "Handler_IncidentEvent.h"
#include <iostream>

using namespace std;

Handler_IncidentEvent::Handler_IncidentEvent() : mysql::Content_handler()
{

}

mysql::Binary_log_event *
Handler_IncidentEvent::process_event(mysql::Incident_event *a_event)
{
    cout << "Event type: " << mysql::system::get_event_type_str(a_event->get_event_type())
    << " length: " << a_event->header()->event_length
    << " next pos: " << a_event->header()->next_position << endl;

    cout << "type= " << (unsigned) a_event->type << " message= " << a_event->message << endl;

    // Consume the event
    delete a_event;
    return 0;
}
