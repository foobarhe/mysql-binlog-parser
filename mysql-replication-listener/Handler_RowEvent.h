//
// Created by hemin on 8/19/15.
//

#ifndef BINLOG_HANDLER_ROWEVENT_H
#define BINLOG_HANDLER_ROWEVENT_H

#include "binlog_api.h"
#include <map>

using namespace std;

class Handler_RowEvent : public mysql::Content_handler {
public:
    Handler_RowEvent(map<uint64_t, Table_map_event *>& a_tableId2event);
    mysql::Binary_log_event *process_event(mysql::Row_event *a_event);
    bool is_text_field(Value & a_val);
    void table_insert(const std::string &a_table_name, mysql::Row_of_fields &a_fields);
    void table_update(const std::string &a_table_name, mysql::Row_of_fields &a_old_fields, mysql::Row_of_fields &a_new_fields);
    void table_delete(const std::string &a_table_name, mysql::Row_of_fields &a_fields);

private:
    map<uint64_t, Table_map_event *>& m_tableId2event;
};

#endif //BINLOG_HANDLER_ROWEVENT_H
