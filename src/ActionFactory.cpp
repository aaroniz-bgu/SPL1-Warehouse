#include <stdexcept>
#include "ActionFactory.h"
#include "Action.h"


/**
 * Factory with default constructor.
 */
ActionFactory::ActionFactory() = default;

//TODO make sure this function works.
/**
 * @param str - the command to be parsed
 * @return - vector containing each of the words seperated by a whitespace
 */
vector<string> splitIntoWords(const string& str) {
    vector<string> words;
    string currentWord;
    for (char ch : str) {
        if (ch == ' ') { // Check for whitespace
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
 * restore
 * @return - A new action according to the action_name, to use the action you have to use the act method.
 */
BaseAction *ActionFactory::createAction(const string &input) {
    vector<string> commands = splitIntoWords(input);
    if (!commands.empty()) {
        string type = commands[0]; // First word should be the action type
        if (type == STEP) {
            int numOfSteps = std::stoi(commands[1]);
            return new SimulateStep(numOfSteps);
        }
        else if (type == ORDER) {
            int orderId = std::stoi(commands[1]);
            return new AddOrder(orderId);
        }
        else if (type == CUSTOMER) {
            string customerName = commands[1];
            string customerType = commands[2];
            int distance = std::stoi(commands[3]);
            int maxOrders = std::stoi(commands[4]);
            return new AddCustomer(customerName, customerType, distance, maxOrders);
        }
        else if (type == ORDERSTATUS) {
            int orderId = std::stoi(commands[1]);
            return new PrintOrderStatus(orderId);
        }
        else if (type == CUSTOMERSTATUS) {
            int customerId = std::stoi(commands[1]);
            return new PrintCustomerStatus(customerId);
        }
        else if (type == VOLUNTEERSTATUS) {
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
        else if (type == VOLUNTEER) {
            string volunteerName = commands[1];
            string volunteerRole = commands[2];
            if (volunteerRole == "collector") {
                int cooldown = std::stoi(commands[3]);
                return new AddVolunteer(volunteerName, cooldown, NOT_LIMITED);
            }
            else if (volunteerRole == "limited_collector") {
                int cooldown = std::stoi(commands[3]);
                int maxOrders = std::stoi(commands[4]);
                return new AddVolunteer(volunteerName, cooldown, maxOrders);
            }
            else if (volunteerRole == "driver") {
                int maxDistance = std::stoi(commands[3]);
                int distancePerStep = std::stoi(commands[4]);
                return new AddVolunteer(volunteerName, maxDistance, distancePerStep, NOT_LIMITED);
            }
            else if (volunteerRole == "limited_driver") {
                int maxDistance = std::stoi(commands[3]);
                int distancePerStep = std::stoi(commands[4]);
                int maxOrders = std::stoi(commands[5]);
                return new AddVolunteer(volunteerName, maxDistance, distancePerStep, maxOrders);
            }
            else {
                // Unknown volunteer role
            }
        }
        else {
            // Unknown action type
        }
    } else {
        // Received empty command
    }
}

