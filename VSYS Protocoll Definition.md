# VSYS Protocoll Definition

## Usage

Both sockserver and sockclient come with instructions and preset defaults which can be accessed in both server and client with the parameter `-h` for "help".

Preset are the following defaults:

- Client IP address: `127.0.0.1`
- Server Port: `1337`
- Client Port: `1337`
- Server Upload directory: `./upload`

## Header

Before a package or a file is sent a header is sent that holds the size of the data that should be received.

## Login

Before a Client can perform any actions on the server he has to login with his LDAP credentials. If the Client fails to provide correct credentials for `n` times the IP address of the client is blocked for `x` minutes.

To perform a login the Client sends a `LoginPackage` to the server. If the client is not blocked the server sends an `AccessPackage` with the login state, the current number of login tries and the maximum number of login tries to the client. After a successful login the client can send actions to the server.

If the client makes too many login attempts the IP address is blocked and the server sends only Packages of the type `ErrorPackage` with the `CLIENT_BLOCKED` error code to the client until the block time is over. 

## Actions

Actions are sent as `ActionPackage`. The following actions are possible.

### LIST

If an `ActionPackage` with the `LIST` action is sent to the server the server will respond with a `ListPackage`.

### GET

If an `ActionPackage` with the `GET` action and a valid filename of an existing file is sent to the server will reply with a `FileInfoPackage` that holds the name of the file. The file will be sent in chunks and the whole file should not be held in memory by the client but written to the filesystem immediately before reading the next chunk. 

If no valid filename was sent the server will send back an `ErrorPackage` with an appropriate error code.

### PUT

If an `ActionPackage` with the `PUT` action and a valid filename of an existing file is sent to the server the server will wait for an `FileInfoPackage` and then start receiving and storing the file to the upload directory in chunks.

### QUIT

If an `ActionPackage` with the `QUIT` action is sent to the server the server will close the socket and end the connection.

## Packages

The Protocol has a number of Packages that are used for communication between the server and the client.

### AccessPackage

Is sent by the server as response to a `LoginPackage`.

The package has the following fields:

- `tries` is the number of login attempts
- `max_tries` is the maximum number of login attempts
- `ok` is set to true if the client is logged in

### ActionPackage

Is sent by the client to request an action.

The package has the following fields:

- `action` is the action that is requested
- `filename` holds the name of a file if GET action is requested

### ErrorPackage

Is sent by the client or the server if an error occurred.

The package has the following field:

- `error_code` is the number of the error that occurred

### FileInfoPackage

Is sent by the server and the client to announce a file transfer.

The package has the following field:

- `filename` is the name of the file that is transferred

### ListPackage

Is sent by the server to the client as response to a `ActionPackage` with the `LIST` action.

The package has the following field:

- `list` is a list of the files in the upload directory of the server.

### LoginPackage

Is sent by the client to the server to provide the server with username and password of the user.

The package has the following fields:

- `username` is the users LDAP username
- `password` is the users LDAP password

### SuccessPackage

Is sent by the server to the client to inform the client that a `PUT` action has ended successfully.

The package has the following field:

- `filename` is the name of the file that was transferred

## File transfer

The file transfer actions PUT and GET do never read or write the whole file into memory but only read or write chunks of at most `BUF` size. This technique prevents high memory usage if big files are transferred.

## Constants

- `MAX_PORT = 65535` is the maximum port that can be used
- `MIN_PORT = 1024` is the minimum port that can be used
- `BUF = 1024` is the block size that is received or sent by the server or the client
- `MIN_NUMBER_THREADS = 4` is the minimum number of threads that is spawned at the start of the server
- `MAX_LOGIN_TRIES = 3` is the maximum number of login attempts
- `BLOCKING_MINUTES = 3` is the number of minutes a client is blocked