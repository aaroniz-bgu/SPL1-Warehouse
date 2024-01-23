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

BaseAction *ActionFactory::createAction(const string &input) {
    vector<string> commands = splitIntoWords(input);
    if (!commands.empty()) {
        string type = commands[0]; // First word should be the action type

        if (type == STEP) {
            // Create StepAction
            // Assuming StepAction takes an int as a constructor argument
            int numOfSteps = std::stoi(commands[1]);
            return new SimulateStep(numOfSteps);
        }
        else if (type == ORDER) {
            // Create OrderAction
            // Assuming OrderAction takes an int as a constructor argument
            int orderId = std::stoi(commands[1]);
            return new AddOrder(orderId);
        }
        else if (type == CUSTOMER) {
            // Create CustomerAction
            // Assuming CustomerAction constructor takes appropriate arguments
            string customerName = commands[1];
            string customerType = commands[2];
            int distance = std::stoi(commands[3]);
            int maxOrders = std::stoi(commands[4]);
            return new AddCustomer(customerName, customerType, distance, maxOrders);
        }
        else if (type == ORDERSTATUS) {
            // Create OrderStatusAction
            int orderId = std::stoi(commands[1]);
            return new PrintOrderStatus(orderId);
        }
        else if (type == CUSTOMERSTATUS) {
            // Create CustomerStatusAction
            int customerId = std::stoi(commands[1]);
            return new PrintCustomerStatus(customerId);
        }
        else if (type == VOLUNTEERSTATUS) {
            // Create VolunteerStatusAction
            int volunteerId = std::stoi(commands[1]);
            return new PrintVolunteerStatus(volunteerId);
        }
        else if (type == LOG) {
            // Create LogAction
            return new PrintActionsLog();
        }
        else if (type == CLOSE) {
            // Create CloseAction
            return new Close();
        }
        else if (type == BACKUP) {
            // Create BackupAction
            return new BackupWareHouse();
        }
        else if (type == RESTORE) {
            // Create RestoreAction
            return new RestoreWareHouse();
        }
        else {
            // Unknown action type
            // Handle unknown action type, maybe throw an exception or return a null pointer
        }
    } else {
        // Received empty command
        // Handle this scenario, maybe throw an exception or return a null pointer
    }
}

