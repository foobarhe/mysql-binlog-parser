//
// Created by hemin on 16-10-11.
//

#include "binlog.h"
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " url " << endl;
        cout << "Example: " << argv[0] << " mysql://root:123@127.0.0.1:3306 " << endl;
        return 1;
    }

    // driver
    binary_log::system::Binary_log_driver *drv= binary_log::system::create_transport(argv[1]);
    if (drv == NULL) {
        cout << "ERROR: create_transport failed" << endl;
        return 1;
    }

    // binlog
    binary_log::Binary_log binlog(drv);
    int res = binlog.connect();
    if (res != ERR_OK) {
        cout << "ERROR: binlog.connect() failed, reason=" << str_error(res) << endl;
        delete drv;
        return 1;
    }

    // get events
    Binary_log_event *event;
    Decoder decode;
    std::pair<unsigned char *, size_t> buf_and_len;
    stringstream ss;

    while (true) {
        // get event and decode
        int error_number;
        error_number= drv->get_next_event(&buf_and_len);
        if (error_number == ERR_OK) {
            const char *error= NULL;
            if (!(event= decode.decode_event((char*)buf_and_len.first, buf_and_len.second, &error, 0))) {
                std::cerr << error << std::endl;
                break;
            }
        }
        else {
            const char* msg=  str_error(error_number);
            std::cerr << msg << std::endl;
            break;
        }

        ss.str("");
        event->print_long_info(ss);

        cout << endl << "--- MAIN: got event" << endl
        << "type=" << event->get_event_type()<< endl
        << "info="<< ss.str() << endl;

        delete event;
    }

    delete drv;
    return 0;
}
