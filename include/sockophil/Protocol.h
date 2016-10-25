//
// Created by tobias on 01/10/16.
//
// Definitions needed by the server and the client for the twftp protocol
//

#pragma once

namespace sockophil {
    /**
     * Actions that can be performed by the Client
     */
    enum ClientAction {
        /**
         * List all the files in the upload directory
         */
        LIST,
        /**
         * Get a file from the server
         */
        GET,
        /**
         * Put a file to the server
         */
        PUT,
        /**
         * Tell the server that you quit
         */
        QUIT
    };
    /**
     * Error codes that can be sent via an ErrorPackage
     */
    enum ErrorCode {
        /**
         * The requested file was not found
         */
        FILE_NOT_FOUND,
        /**
         * A different package type was expected
         */
        WRONG_PACKAGE,
        /**
         * The sent file could not be stored
         */
        FILE_STORAGE
    };
}
