//
// Created by tobias on 01/10/16.
//

#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <memory>
#include <cstring>
#include <arpa/inet.h>
#include <sstream>
#include <fstream>
#include <sockophil/Networking.h>
#include "sockophil/Helper.h"
#include "cereal/archives/portable_binary.hpp"
#include "nlohmann/json.hpp"
#include "sockophil/constants.h"
#include "sockophil/DataPackage.h"
#include "sockclient/SocketConnectionException.h"
#include "sockophil/SocketCreationException.h"
#include "sockclient/CurrentDirectoryException.h"
#include "sockclient/Client.h"
#include "sockophil/protocol.h"

namespace sockclient {

    /**
     * Constructor that sets the member vars, create the socket and connects to the socket
     * @param port to connect to
     * @param ip_address to connect to
     */
    Client::Client(unsigned short port, std::string ip_address) : port(port), ip_address(ip_address) {
        char current_directory[1024];
        if(getcwd(current_directory, sizeof(current_directory)) == nullptr) {
            throw CurrentDirectoryException(errno);
        }
        this->current_directory = current_directory;
        this->menu = std::make_shared<Menu>(port, ip_address, current_directory);
        this->connected = false;
        this->create_socket();
        this->connect_to_socket();

    }
    /**
     * Destructor closes the socket
     */
    Client::~Client() {
        this->close_socket();
    }

    /**
     * Try to create a socket
     */
    void Client::create_socket() {
        this->socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
        if(this->socket_descriptor == -1) {
            throw sockophil::SocketCreationException(errno);
        }
        std::memset(&this->address,0,sizeof(this->address));
        this->address.sin_family = AF_INET;
        this->address.sin_port = htons(this->port);
        inet_aton(this->ip_address.c_str(), &this->address.sin_addr);
    }

    /**
     * The main method of the Class that is called from the main function
     *
     * Calls the menu to prompt the user for input and reacts on it.
     */
    void Client::run() {
        bool check = true;
        while(check) {
            auto selection = this->menu->selection_prompt();
            switch(selection.get_action()) {
                case sockophil::list:
                    this->request_a_list();
                    break;
                case sockophil::quit:
                    this->bid_server_farewell();
                    check = false;
                    break;
                case sockophil::get:
                    this->download_a_file(selection.get_filename());
                    break;
                case sockophil::put:
                    this->upload_a_file(selection.get_filename());
                    break;
            }
        }
    }

    void Client::request_a_list() {
        this->send_request(std::make_shared<sockophil::RequestPackage>(sockophil::list));
        auto pkg = this->receive_list_response();
        this->menu->render_list_response(pkg->get_list());
    }

    void Client::bid_server_farewell() {
        this->send_request(std::make_shared<sockophil::RequestPackage>(sockophil::quit));
    }

    void Client::upload_a_file(std::string filename) {
        std::vector<uint8_t> content;
        std::ifstream ifs(filename, std::ios::binary);
        std::for_each(std::istreambuf_iterator<char>(ifs),
                      std::istreambuf_iterator<char>(),
                      [&content](const char c){
                          content.push_back(c);
                      });
        this->send_request(std::make_shared<sockophil::RequestPackage>(sockophil::put, filename, content.size()));
        this->send_data(std::make_shared<sockophil::DataPackage>(content));

    }

    void Client::download_a_file(std::string filename) {
        this->send_request(std::make_shared<sockophil::RequestPackage>(sockophil::get, filename));
    }

    void Client::send_request(const std::shared_ptr<sockophil::RequestPackage> package) const {
        this->send_to_server(package);
    }

    void Client::send_data(const std::shared_ptr<sockophil::DataPackage> package) const {
        this->send_to_server(package);
    }

    void Client::send_to_server(const std::shared_ptr<sockophil::Package> package) const {
        this->send_package(this->socket_descriptor, package);
    }


    /**
     * Try to connect to the socket
     */
    void Client::connect_to_socket() {
        if (connect(this->socket_descriptor, (struct sockaddr *) &this->address, sizeof (this->address)) != 0) {
            throw SocketConnectionException(errno);
        } else {
            this->connected = true;
        }
    }

    /**
     * Close the socket
     */
    void Client::close_socket() {
        //close(this->socket_descriptor);
    }

    std::shared_ptr<sockophil::ListPackage> Client::receive_list_response() const {
        auto pkg = this->receive_response();
        if(pkg->get_type() == sockophil::LIST_PACKAGE) {
            return std::static_pointer_cast<sockophil::ListPackage>(pkg);
        } else {
            // @todo throw something
        }
    }

    std::shared_ptr<sockophil::Package> Client::receive_response() const {
        return this->receive_package(this->socket_descriptor);
    }
}
