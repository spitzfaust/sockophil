//
// Created by tobias on 01/10/16.
//
// Definitions needed by the server and the client for the twftp protocol
//

#pragma once

namespace sockophil {
    enum ClientAction {
        LIST, GET, PUT, QUIT
    };
    enum ErrorCode {
        FILE_NOT_FOUND, WRONG_PACKAGE, FILE_STORAGE
    };
}
