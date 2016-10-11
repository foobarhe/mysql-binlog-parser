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
    cout << "got Incident_event message=" << a_event->message << endl;

    // Consume the event
    delete a_event;
    return 0;
}
