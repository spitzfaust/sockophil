//
// Created by tobias on 01/10/16.
//
#include <cctype>
#include <string>
#include <functional>
#include <sys/ioctl.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include "rlutil/rlutil.h"
#include "sockclient/Menu.h"

namespace sockclient {

/**
 * Constructor sets the member vars.
 * @param port the client listens to
 * @param ip_address the client listens to
 * @param current_directory the client is running in and downloads files to
 */
Menu::Menu(int port, std::string ip_address, std::string current_directory)
    : current_directory(current_directory), first_run(true) {
  this->connected_on = ip_address + ":" + std::to_string(port);
  this->update_terminal_dimensions();
}

/**
 * @brief Print a message that is centered horizontaly in the terminal
 * @param message is printed to the terminal
 * @param filling left and right from the message
 * @param bg_color is the background color (rlutil)
 * @param color is the font color (rlutil)
 */
void Menu::render_centered_msg(const std::string &message, const char filling,
                               const int color) const noexcept {
  rlutil::setColor(color);
  int first_width = (message.length() % 2 ? this->terminal_width - 1 : this->terminal_width) / 2;
  for (int j = 0; j < (first_width - (message.length() / 2)); ++j) {
    std::cout << filling;
  }
  std::cout << message;
  for (int j = 0; j < ((this->terminal_width / 2) - (message.length() / 2)); ++j) {
    std::cout << filling;
  }
  rlutil::resetColor();
  std::cout << std::endl;
}

/**
 * @brief Print a horizontal line to the terminal
 * @param filling character that is printed
 * @param bg_color is the background color (rlutil)
 * @param color is the font color (rlutil)
 */
void Menu::render_hr(const char filling, const int color) const noexcept {
  rlutil::setColor(color);
  for (int i = 0; i < this->terminal_width; ++i) {
    std::cout << filling;
  }
  std::cout << std::endl;
  rlutil::resetColor();
}

/**
 * @brief Print a welcome message to the screen. Is only called once on start of the client.
 */
void Menu::render_welcome_msg() const noexcept {
  std::string welcome_msg = "sockclient v0.1";
  rlutil::saveDefaultColor();
  this->render_hr('x');
  this->render_centered_msg(welcome_msg);
  this->render_hr('x');
  rlutil::setColor(rlutil::LIGHTGREEN);
  std::cout << this->clear_eol() << std::endl;
  std::cout << "\u2714 Connected on:   " << this->get_connected_on() << std::endl;
  std::cout << "\u2714 Downloading to: " << this->get_current_directory() << std::endl;
  rlutil::resetColor();
}

/**
 * @brief Get the width and height of the terminal window.
 */
void Menu::update_terminal_dimensions() noexcept {
  this->terminal_width = rlutil::tcols();
  this->terminal_height = rlutil::trows();
}

/**
 * @brief Get the current_directory
 * @return current_directory
 */
std::string Menu::get_current_directory() const noexcept {
  return this->current_directory;
}

/**
 * @brief Get the connected_on string
 * @return connected_on
 */
std::string Menu::get_connected_on() const noexcept {
  return this->connected_on;
}

/**
 * @brief Display a Menu for the User to choose from the different Client actions
 * @return the selection the user made
 */
ClientSelection Menu::selection_prompt() {
  sockophil::ClientAction action;
  std::string filename = "";
  this->update_terminal_dimensions();
  if (this->first_run) {
    this->render_welcome_msg();
    this->first_run = false;
  }
  action = this->action_prompt();
  if (action == sockophil::PUT || action == sockophil::GET) {
    filename = this->filename_prompt();
    return ClientSelection(action, filename);
  }
  return ClientSelection(action);
}

/**
 * @brief Get input from the user until a valid action is entered
 * @return the coresponding ClientAction to the entered string
 */
sockophil::ClientAction Menu::action_prompt() const noexcept {
  std::string input_action = "";
  sockophil::ClientAction action;
  while (true) {
    this->render_action_prompt();
    std::cin >> input_action;
    std::transform(input_action.begin(), input_action.end(), input_action.begin(), ::tolower);
    if (input_action == "list" || input_action == "l") {
      action = sockophil::LIST;
      break;
    } else if (input_action == "put" || input_action == "p") {
      action = sockophil::PUT;
      break;
    } else if (input_action == "get" || input_action == "g") {
      action = sockophil::GET;
      break;
    } else if (input_action == "quit" || input_action == "q") {
      action = sockophil::QUIT;
      break;
    }

    std::cout << "Error: Invalid input!" << std::endl;
  }
  return action;
}

/**
 * @brief Render a prompt for an action
 */
void Menu::render_action_prompt() const noexcept {
  rlutil::setColor(rlutil::LIGHTGREEN);
  std::cout << std::endl;
  std::cout << "\u250F Commands:" << std::endl;
  std::cout << "\u2523\u2501\u2501 L List" << std::endl;
  std::cout << "\u2523\u2501\u2501 P Put" << std::endl;
  std::cout << "\u2523\u2501\u2501 G Get" << std::endl;
  std::cout << "\u2517\u2501\u2501 Q Quit" << std::endl;
  std::cout << "> ";
}

/**
 * @brief Prompt for a filename until a valid one is entered
 * @return filename for put or get command
 */
std::string Menu::filename_prompt() const noexcept {
  std::string filename = "";
  while (true) {
    this->render_filename_prompt();
    /* get the line from stdin and ignore leading whitespace */
    std::getline(std::cin >> std::ws, filename);
    /* check if filename has whitespace or is empty */
    if (std::any_of(filename.begin(), filename.end(), [](char c) { return std::isspace(c); }) &&
        filename.empty()) {
      std::cout << "Error: Invalid input!" << std::endl;
    } else {
      return filename;
    }
  }
}
/**
 * @brief Render a prompt for a filename
 */
void Menu::render_filename_prompt() const noexcept {
  rlutil::setColor(rlutil::LIGHTGREEN);
  std::cout << "Filename:" << std::endl;
  std::cout << "> ";
}

/**
 * Return a string that clears until the end of line.
 *
 * Used for lines that should have background color.
 * @return string to clear to the end of line
 */
std::string Menu::clear_eol() const noexcept {
  return "\x1B[K";
}

/**
 * @brief Render the list of elements in the upload dir of the server
 * @param list is a vector of elements
 */
void Menu::render_list_response(std::vector<std::string> list) const noexcept {
  this->render_hr('-');
  rlutil::setColor(rlutil::LIGHTGREEN);
  /* check if there are no elements */
  if (list.empty()) {
    std::cout << "No files on the server." << std::endl;
  } else {
    std::cout << "Files on server:" << std::endl;
    /* print eache element one by one */
    for (auto &&item : list) {
      std::cout << "|  " << item << std::endl;
    }
  }
  rlutil::resetColor();
  this->render_hr('-');
}

/**
 * @brief Render an error message
 * @param error_msg is the error message
 */
void Menu::render_error(const std::string &error_msg) const noexcept {
  this->render_hr('-', rlutil::RED);
  rlutil::setColor(rlutil::RED);
  std::cout << error_msg << std::endl;
  rlutil::resetColor();
  this->render_hr('-', rlutil::RED);

}

/**
 * @brief Generate an error string out of an error code and the performed action and render it
 * @param action is the action that was performed while the error happend
 * @param error_code is the code that identifies the error
 */
void Menu::render_error(sockophil::ClientAction action, sockophil::ErrorCode error_code) const noexcept {
  std::string error_msg = "";

  switch (action) {
    case sockophil::GET:
      error_msg = "Get";
      break;
    case sockophil::LIST:
      error_msg = "List";
      break;
    case sockophil::PUT:
      error_msg = "Put";
      break;
    case sockophil::QUIT:
      error_msg = "Quit";
      break;
    default:
      error_msg = "Client";
      break;
  }
  error_msg += " Error: ";
  switch (error_code) {
    case sockophil::FILE_NOT_FOUND:
      error_msg += "Could not find the file.";
      break;
    case sockophil::WRONG_PACKAGE:
      error_msg += "Received an unexpected package.";
      break;
    case sockophil::FILE_STORAGE:
      error_msg += "Could not save the file.";
      break;
    default:
      error_msg += "Unknown error.";
      break;
  }
  this->render_error(error_msg);
}

/**
 * @brief Render a success message
 * @param success_msg is the message that should be rendered
 */
void Menu::render_success(const std::string &success_msg) const noexcept {
  this->render_hr('~', rlutil::LIGHTBLUE);
  rlutil::setColor(rlutil::LIGHTBLUE);
  std::cout << success_msg << std::endl;
  rlutil::resetColor();
  this->render_hr('~', rlutil::LIGHTBLUE);
}
}