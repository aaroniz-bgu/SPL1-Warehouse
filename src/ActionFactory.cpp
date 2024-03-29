#include <stdexcept>
#include <iostream>
#include "../include/ActionFactory.h"
#include "../include/Action.h"


/**
 * Factory with default constructor.
 */
ActionFactory::ActionFactory() = default;

/**
 * @param str - the command to be parsed
 * @return - vector containing each of the words seperated by a whitespace,
 * if it see's a # it will ignore the rest of the line.
 */
vector<string> splitIntoWords(const string& str) {
    vector<string> words;
    string currentWord;
    for (char ch : str) {
        if (ch == '#') {
            // If '#' is encountered, stop processing the rest of the line
            break;
        } else if (ch == ' ') {
            // Check for whitespace
            if (!currentWord.empty()) {
                words.push_back(currentWord);
                currentWord.clear();
            }
        } else {
            currentWord += ch;
        }
    }
    if (!currentWord.empty()) {
        words.push_back(currentWord);
    }
    return words;
}

/**
 * Creates a new action according to the input, to use the action you get you have to use the act method.
 * @param input - string with each argument seperated by a space in the following format:
 * (action_name) (action_args) - (e.g. customer Ben soldier 4 2)
 * These are the different actions and args required:
 * step (number_of_steps),
 * order (customer_id),
 * customer (customer_name) (customer_type) (customer_distance) (max_orders),
 * orderStatus (order_id),
 * customerStatus (customer_id),
 * volunteerStatus (volunteer_id),
 * log,
 * close,
 * backup,
 * restore,
 * volunteer (volunteer_name) (volunteer_role)(options: collector/limited_collector/driver/limited_driver)
 * (volunteer_coolDown)/(volunteer_maxDistance)
 * (distance_per_step)(for drivers only) (volunteer_maxOrders)(optional)
 * @param isOpen - Where the warehouse is open or not. So that the function knows if it can use the volunteer command.
 * @return - A new action according to the action_name, to use the action you have to use the act method. Or a nullptr
 * if the actionType is unknown.
 */
BaseAction *ActionFactory::createAction(const string &input, bool isOpen) {
    vector<string> commands = splitIntoWords(input);
    if (!commands.empty()) {
        string type = commands[0]; // First word should be the action type
        try {
            int size = commands.size();
            if (type == STEP && size == 2) {
                int numOfSteps = std::stoi(commands[1]);
                return new SimulateStep(numOfSteps);
            }
            else if (type == ORDER && size == 2) {
                int orderId = std::stoi(commands[1]);
                return new AddOrder(orderId);
            }
            else if (type == CUSTOMER && size == 5) {
                string customerName = commands[1];
                string customerType = commands[2];
                int distance = std::stoi(commands[3]);
                int maxOrders = std::stoi(commands[4]);
                return new AddCustomer(customerName, customerType, distance, maxOrders);
            }
            else if (type == ORDERSTATUS && size == 2) {
                int orderId = std::stoi(commands[1]);
                return new PrintOrderStatus(orderId);
            }
            else if (type == CUSTOMERSTATUS && size == 2) {
                int customerId = std::stoi(commands[1]);
                return new PrintCustomerStatus(customerId);
            }
            else if (type == VOLUNTEERSTATUS && size == 2) {
                int volunteerId = std::stoi(commands[1]);
                return new PrintVolunteerStatus(volunteerId);
            }
            else if (type == LOG) {
                return new PrintActionsLog();
            }
            else if (type == CLOSE) {
                return new Close();
            }
            else if (type == BACKUP) {
                return new BackupWareHouse();
            }
            else if (type == RESTORE) {
                return new RestoreWareHouse();
            }
            else if (!isOpen && type == VOLUNTEER  && size > 3) {
                string volunteerName = commands[1];
                string volunteerRole = commands[2];
                if (volunteerRole == "collector") {
                    int cooldown = std::stoi(commands[3]);
                    return new AddVolunteer(volunteerName, cooldown, NOT_LIMITED);
                }
                else if (volunteerRole == "limited_collector" && size > 4) {
                    int cooldown = std::stoi(commands[3]);
                    int maxOrders = std::stoi(commands[4]);
                    return new AddVolunteer(volunteerName, cooldown, maxOrders);
                }
                else if (volunteerRole == "driver" && size > 4) {
                    int maxDistance = std::stoi(commands[3]);
                    int distancePerStep = std::stoi(commands[4]);
                    return new AddVolunteer(volunteerName, maxDistance, distancePerStep, NOT_LIMITED);
                }
                else if (volunteerRole == "limited_driver" && size > 5) {
                    int maxDistance = std::stoi(commands[3]);
                    int distancePerStep = std::stoi(commands[4]);
                    int maxOrders = std::stoi(commands[5]);
                    return new AddVolunteer(volunteerName, maxDistance, distancePerStep, maxOrders);
                }
                else {
                    std::cout << "Unknown volunteer role: " << volunteerRole << std::endl;
                }
            }
            else {
                std::cout << type << " is an unknown command or perhaps you had missing arguments." << std::endl;
                return nullptr;
            }
        }
        catch (exception &ex) {
            std::cout << "Unknown command (perhaps arguments were wrong)." << std::endl;
        }
    } else {
        std::cout << "Received empty line, make sure you type the command correctly." << std::endl;
    }
    return nullptr;
}

