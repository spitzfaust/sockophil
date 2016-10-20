//
// Created by tobias on 01/10/16.
//
// Definitions needed by the server and the client for the twftp protocol
//

#pragma once

namespace sockophil {
    enum ClientAction {
        list, get, put, quit
    };
    enum ErrorCode {
        file_not_found, wrong_request
    };
}
