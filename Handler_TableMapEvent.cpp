//
// Created by hemin on 8/19/15.
//

#include "Handler_TableMapEvent.h"

Handler_TableMapEvent::Handler_TableMapEvent(map<uint64_t, mysql::Table_map_event *>& a_tableId2event)
        : m_tableId2event(a_tableId2event)
{
}

mysql::Binary_log_event *
Handler_TableMapEvent::process_event(mysql::Table_map_event *a_event)
{
    m_tableId2event[a_event->table_id] = a_event;

    return a_event;
}
