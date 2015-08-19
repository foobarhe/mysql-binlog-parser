//
// Created by hemin on 8/19/15.
//

#ifndef BINLOG_HANDLER_INCIDENTEVENT_H
#define BINLOG_HANDLER_INCIDENTEVENT_H

#include "binlog_api.h"

class Handler_IncidentEvent : public mysql::Content_handler {
public:
    Handler_IncidentEvent();
    mysql::Binary_log_event *process_event(mysql::Incident_event *a_event);
};


#endif //BINLOG_HANDLER_INCIDENTEVENT_H
