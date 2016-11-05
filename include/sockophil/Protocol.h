//
// Created by tobias on 01/10/16.
//
// Definitions needed by the server and the client for the twftp protocol
//

#pragma once

namespace sockophil {
/**
 * @brief Actions that can be performed by the Client
 */
enum ClientAction {
  /**
   * @var List all the files in the upload directory
   */
      LIST,
  /**
   * @var Get a file from the server
   */
      GET,
  /**
   * @var Put a file to the server
   */
      PUT,
  /**
   * @var Tell the server that you quit
   */
      QUIT,
  /**
   * @var Tell the server your login data
   */
      LOGIN
};
/**
 * @brief Error codes that can be sent via an ErrorPackage
 */
enum ErrorCode {
  /**
   * @var The requested file was not found
   */
      FILE_NOT_FOUND,
  /**
   * @var A different package type was expected
   */
      WRONG_PACKAGE,
  /**
   * @var The sent file could not be stored
   */
      FILE_STORAGE
};
}
