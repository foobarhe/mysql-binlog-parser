// based on mysql-replication-listener
// https://code.launchpad.net/~mysql/mysql-replication-listener/trunk

#include <iostream>
#include <map>
#include <sstream>
#include "binlog_api.h"
#include "Handler_IncidentEvent.h"
#include "Handler_TableMapEvent.h"
#include "Handler_RowEvent.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " url " << endl;
        cout << "Example: " << argv[0] << " mysql://root:123@127.0.0.1:3306 " << endl;
        return 1;
    }

    // connect
    mysql::Binary_log binlog(mysql::system::create_transport(argv[1]));
    if (binlog.connect()) {
        cout << "can't connect to the master." << endl;
        return 1;
    }
    cout << "connected to the server " << argv[1] << endl;
//    if (binlog.set_position(4) != ERR_OK) {
//        cout << "Can't reposition the binary log reader." << endl;
//        return 1;
//    }


    // set callbacks for events

    // mysql::Basic_transaction_parser transaction_parser;
    // binlog.content_handler_pipeline()->push_back(&transaction_parser);

    Handler_IncidentEvent incidentHandler;
    map<uint64_t, mysql::Table_map_event *> tableId2event;
    Handler_TableMapEvent tableMapEventHandler(tableId2event);
    Handler_RowEvent rowEventHandler(tableId2event);

    binlog.content_handler_pipeline()->push_back(&incidentHandler);
    binlog.content_handler_pipeline()->push_back(&tableMapEventHandler);
    binlog.content_handler_pipeline()->push_back(&rowEventHandler);
    cout << "set handler pipeline for events" << endl;

    int res;
    mysql::Binary_log_event *event;
    stringstream ss;

    unsigned long nextPos;

    // loop for events
    while (true) {
        res = binlog.wait_for_next_event(&event);
        if (res != mysql::ERR_OK) {
            cout << "wait_for_next_event returns " << res << " (" << mysql::str_error(res) <<")"<< endl;
            break;
        }

        nextPos = event->header()->next_position;

        ss.str("");
        event->print_long_info(ss);
        cout << endl << "--- MAIN: got event" << endl
        << "type=" << event->get_event_type()<<"(" << mysql::system::get_event_type_str(event->get_event_type())<<")" << endl
        << "nextPos=" << nextPos << endl
        << "info="<< ss.str() << endl;

//        // Perform a special action based on event type
//        switch (event->get_event_type()) {
//            case mysql::QUERY_EVENT: {
//                const mysql::Query_event *qev = static_cast<const mysql::Query_event *>(event);
//                std::cout << "query= " << qev->query << " db= " << qev->db_name  << std::endl;
//                break;
//            }
//            case mysql::ROTATE_EVENT: {
//                mysql::Rotate_event *rot = static_cast<mysql::Rotate_event *>(event);
//                std::cout << "filename= " << rot->binlog_file.c_str() << " pos= " << rot->binlog_pos << std::endl;
//                break;
//            }
//        }

    }

    return 0;
}
