/**
 * This header contains constants that are used by the server and the client.
 */

#pragma once

#include <string>
namespace sockophil {
/**
 * @var MAX_PORT is the maximum port number that is allowed
 */
const unsigned int MAX_PORT = 65535;
/**
 * @var MIN_PORT is the minimum port number that is allowed
 */
const unsigned int MIN_PORT = 1024;
/**
 * @var DEFAULT_PORT is the port that is used when none is given
 */
const unsigned int DEFAULT_PORT = 1337;
/**
 * @var BUF is the buffer that is read from the socket
 */
const unsigned int BUF = 1024;
/**
 * @var FILE_INFO_PACKAGE identifies the FileInfoPackage class
 */
const std::string FILE_INFO_PACKAGE = "FILE_INFO_PACKAGE";
/**
 * @var ACTION_PACKAGE identifies the ActionPackage class
 */
const std::string ACTION_PACKAGE = "ACTION_PACKAGE";
/**
 * @var LIST_PACKAGE identifies the ListPackage class
 */
const std::string LIST_PACKAGE = "LIST_PACKAGE";
/**
 * @var ERROR_PACKAGE identifies the ErrorPackage class
 */
const std::string ERROR_PACKAGE = "ERROR_PACKAGE";
/**
 * @var SUCCESS_PACKAGE identifies the SuccessPackage class
 */
const std::string SUCCESS_PACKAGE = "SUCCESS_PACKAGE";

/**
 * @var MIN_NUMBER_THREADS is the minimum number of threads in the server threadpool
 */
const unsigned int MIN_NUMBER_THREADS = 4;

}