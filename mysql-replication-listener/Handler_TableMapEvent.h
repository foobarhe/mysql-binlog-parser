//
// Created by hemin on 8/19/15.
//

#ifndef BINLOG_HANDLER_TABLEMAPEVENT_H
#define BINLOG_HANDLER_TABLEMAPEVENT_H

#include "binlog_api.h"
#include <map>

using namespace std;

class Handler_TableMapEvent : public mysql::Content_handler {
public:
    Handler_TableMapEvent(map<uint64_t, Table_map_event *>& a_tableId2event);
    mysql::Binary_log_event *process_event(mysql::Table_map_event *a_event);
private:
    map<uint64_t, Table_map_event *>& m_tableId2event;
};

#endif //BINLOG_HANDLER_TABLEMAPEVENT_H
