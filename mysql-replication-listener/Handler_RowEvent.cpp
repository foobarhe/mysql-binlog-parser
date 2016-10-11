//
// Created by hemin on 8/19/15.
//

#include "Handler_RowEvent.h"

Handler_RowEvent::Handler_RowEvent(map<uint64_t, Table_map_event *>& a_tableId2event) : m_tableId2event(a_tableId2event)
{
}

mysql::Binary_log_event *
Handler_RowEvent::process_event(mysql::Row_event *a_event)
{
    switch (a_event->get_event_type()) {
        case mysql::WRITE_ROWS_EVENT:
        case mysql::WRITE_ROWS_EVENT_V1:
        case mysql::DELETE_ROWS_EVENT:
        case mysql::DELETE_ROWS_EVENT_V1:
        case mysql::UPDATE_ROWS_EVENT:
        case mysql::UPDATE_ROWS_EVENT_V1: {
            mysql::Row_event *rowEvent = static_cast<mysql::Row_event *>(a_event);
            uint64_t table_id = rowEvent->table_id;
            mysql::Table_map_event * tmEvent = m_tableId2event[table_id];
            if (tmEvent == NULL) {
                cout << "Table id " << table_id << " was not registered by any preceding table map event." << endl;
                return a_event;
            }
            cout << "ROW_EVENTS: table_id=" << table_id << ", db_name=" << tmEvent->db_name << ", table_name=" << tmEvent->table_name << endl;

            // Each row event contains multiple rows and fields. The Row_iterator allows us to iterate one row at a time.
            mysql::Row_event_set rows(rowEvent, tmEvent);
            std::ostringstream os;
            os << tmEvent->db_name << '.' << tmEvent->table_name;

            try {
                mysql::Row_event_set::iterator it = rows.begin();
                do {
                    mysql::Row_of_fields fields = *it;
                    if (a_event->get_event_type() == mysql::WRITE_ROWS_EVENT ||
                            a_event->get_event_type() == mysql::WRITE_ROWS_EVENT_V1)
                        table_insert(os.str(), fields);
                    if (a_event->get_event_type() == mysql::UPDATE_ROWS_EVENT ||
                            a_event->get_event_type() == mysql::UPDATE_ROWS_EVENT_V1)
                    {
                        ++it;
                        mysql::Row_of_fields fields2 = *it;
                        table_update(os.str(), fields, fields2);
                    }
                    if (a_event->get_event_type() == mysql::DELETE_ROWS_EVENT ||
                            a_event->get_event_type() == mysql::DELETE_ROWS_EVENT_V1)
                        table_delete(os.str(), fields);
                } while (++it != rows.end());
            }
            catch (const std::logic_error& le) {
                std::cerr << "MySQL Data Type error: " << le.what() << '\n';
            }
        }
    }

    delete a_event;
    return 0;
}


bool
Handler_RowEvent::is_text_field(Value& a_val) {
    if (a_val.type() == MYSQL_TYPE_VARCHAR ||
            a_val.type() == MYSQL_TYPE_BLOB ||
            a_val.type() == MYSQL_TYPE_MEDIUM_BLOB ||
            a_val.type() == MYSQL_TYPE_LONG_BLOB) {
        return true;
    }
    return false;
}

void
Handler_RowEvent::table_insert(const std::string &a_table_name, mysql::Row_of_fields &a_fields) {
    std::cout << "INSERT INTO "
    << a_table_name
    << " VALUES (";
    mysql::Row_of_fields::iterator field_it = a_fields.begin();
    mysql::Converter converter;
    do {
        // Each row contains a vector of Value objects. The converter allows us to transform the value into another representation.
        std::string str;
        converter.to(str, *field_it);
        if (is_text_field(*field_it))
            std::cout << '\'';
        std::cout << str;
        if (is_text_field(*field_it))
            std::cout << '\'';
        ++field_it;
        if (field_it != a_fields.end())
            std::cout << ", ";
    } while (field_it != a_fields.end());
    std::cout << ")" << std::endl;
}

void
Handler_RowEvent::table_update(const std::string &a_table_name,
                  mysql::Row_of_fields &a_old_fields,
                  mysql::Row_of_fields &a_new_fields) {
    std::cout << "UPDATE "
    << a_table_name
    << " SET ";

    int field_id = 0;
    mysql::Row_of_fields::iterator field_it = a_new_fields.begin();
    mysql::Converter converter;
    do {
        std::string str;
        converter.to(str, *field_it);
        std::cout << field_id << "= ";
        if (is_text_field(*field_it))
            std::cout << '\'';
        std::cout << str;
        if (is_text_field(*field_it))
            std::cout << '\'';
        ++field_it;
        ++field_id;
        if (field_it != a_new_fields.end())
            std::cout << ", ";
    } while (field_it != a_new_fields.end());
    std::cout << " WHERE ";

    field_it = a_old_fields.begin();
    field_id = 0;
    do {
        std::string str;
        converter.to(str, *field_it);
        std::cout << field_id << "= ";
        if (is_text_field(*field_it))
            std::cout << '\'';
        std::cout << str;
        if (is_text_field(*field_it))
            std::cout << '\'';
        ++field_it;
        ++field_id;
        if (field_it != a_old_fields.end())
            std::cout << " AND ";
    } while (field_it != a_old_fields.end());
    std::cout << " LIMIT 1" << std::endl;

}


void
Handler_RowEvent::table_delete(const std::string &a_table_name, mysql::Row_of_fields &a_fields) {
    std::cout << "DELETE FROM "
    << a_table_name
    << " WHERE ";
    mysql::Row_of_fields::iterator field_it = a_fields.begin();
    int field_id = 0;
    mysql::Converter converter;
    do {

        std::string str;
        converter.to(str, *field_it);
        std::cout << field_id << "= ";
        if (is_text_field(*field_it))
            std::cout << '\'';
        std::cout << str;
        if (is_text_field(*field_it))
            std::cout << '\'';
        ++field_it;
        ++field_id;
        if (field_it != a_fields.end())
            std::cout << " AND ";
    } while (field_it != a_fields.end());
    std::cout << " LIMIT 1" << std::endl;
}
