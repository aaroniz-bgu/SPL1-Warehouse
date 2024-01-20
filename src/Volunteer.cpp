#include "Volunteer.h"

// ###########################################################################
// ###                          AbstractVolunteer                          ###
// ###########################################################################

Volunteer::Volunteer(int id, const string &name) : id(id), name(name), completedOrderId(NO_ORDER), activeOrderId(NO_ORDER) { }

/**
 * @returns the volunteer's identifier.
 */
int Volunteer::getId() const{
    return id;
}

/**
 * @returns the volunteer's name.
 */
const string &Volunteer::getName() const{
    return name;
}

/**
 * @returns the volunteer's active order identifier if there is one active, otherwise -1.
 */
int Volunteer::getActiveOrderId() const{
    return activeOrderId;
}

/**
 * @returns the volunteer's completed order identifier if there is one completed, otherwise -1.
 */
int Volunteer::getCompletedOrderId() const{
    return completedOrderId;
}

/**
 * @returns true if the volunteer is busy, otherwise false.
 */
bool Volunteer::isBusy() const{
    return activeOrderId != NO_ORDER;
}

// ###########################################################################
// ###                         CollectorVolunteer                          ###
// ###########################################################################

/**
 * @param id the volunteer's identifier.
 * @param name the volunteer's name.
 * @param coolDown the volunteer's cool down.
 */
CollectorVolunteer::CollectorVolunteer(int id, string name, int coolDown) :
Volunteer(id, name), coolDown(coolDown), timeLeft(0) { }

/**
 * @returns a copy of the volunteer.
 */
CollectorVolunteer *CollectorVolunteer::clone() const  {
    // FIXME - most likely it's ok and no copy constructor is needed, but make sure.
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()  {
    if(activeOrderId != -1) {
        if(timeLeft != 0 && decreaseCoolDown()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

/**
 * @returns the volunteer's cool down.
 */
int CollectorVolunteer::getCoolDown() const {
    return coolDown;
}

/**
 * @returns the time left until the volunteer is available again.
 */
int CollectorVolunteer::getTimeLeft() const {
    return timeLeft;
}

/**
 * Decrease timeLeft by 1 if it is not 0.
 * @returns true if timeLeft=0 and false otherwise.
 */
bool CollectorVolunteer::decreaseCoolDown() {
    if(timeLeft != 0) {
        timeLeft -= 1;
        return timeLeft == 0;
    }
    return true;
}

/**
 * @returns true if the volunteer is still able to take orders, otherwise false.
 */
bool CollectorVolunteer::hasOrdersLeft() const {
    return true;
}

/**
 * @returns true if the volunteer can take the order, otherwise false.
 */
bool CollectorVolunteer::canTakeOrder(const Order &order) const {
    return activeOrderId == -1;
}

/**
 * Accepts an order as the volunteer's active order (if it's able to accept order).
 * @note Does not assign itself as the order's collector volunteer.
 * @param order
 */
void CollectorVolunteer::acceptOrder(const Order &order) {
    if(activeOrderId == -1) {
        timeLeft = coolDown;
        activeOrderId = order.getId();
    }
}

/**
 * @returns a string description of the volunteer.
 */
string CollectorVolunteer::toString() const {
    string output = "VolunteerID: " + std::to_string(getId()) + "\n";
    output += "isBusy: ";
    output += ((timeLeft == 0) ? "False\n" : "True\n");
    output += "OrderID: ";
    output += ((activeOrderId != -1) ? std::to_string(activeOrderId) : "None");
    output += "\nTimeLeft: ";
    output += ((timeLeft != 0) ? std::to_string(timeLeft) : "None");
    output += "\nOrdersLeft: No Limit";
    return output;
}

// ###########################################################################
// ###                      LimitedCollectorVolunteer                      ###
// ###########################################################################

/**
 * @param id the volunteer's identifier.
 * @param name the volunteer's name.
 * @param coolDown the volunteer's cool down.
 * @param maxOrders the maximum number of orders the volunteer can take.
 */
LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, string name, int coolDown ,int maxOrders) :
CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) { }

/**
 * @returns a copy of the volunteer.
 */
LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const {
    // FIXME - most likely it's ok and no copy constructor is needed, but make sure.
    return new LimitedCollectorVolunteer(*this);
}

/**
 * @returns true if the volunteer is still able to take orders, otherwise false.
 */
bool LimitedCollectorVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

/**
 * @returns true if the volunteer can take the order, otherwise false.
 */
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const {
    return activeOrderId == -1 && ordersLeft > 0;
}

/**
 * Accepts an order as the volunteer's active order (if it's able to accept order).
 * @note Does not assign itself as the order's collector volunteer.
 * @param order
 */
void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    if(activeOrderId == -1 && ordersLeft > 0) {
        ordersLeft -= 1;
        CollectorVolunteer::acceptOrder(order);
    }
}

/**
 * @returns the maximum number of orders the volunteer can take.
 */
int LimitedCollectorVolunteer::getMaxOrders() const {
    return maxOrders;
}

/**
 * @returns the number of orders the volunteer can still take.
 */
int LimitedCollectorVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

/**
 * @returns a string description of the volunteer.
 */
string LimitedCollectorVolunteer::toString() const {
    string output = "VolunteerID: " + std::to_string(getId()) + "\n";
    output += "isBusy: ";
    output += ((getTimeLeft() == 0) ? "False\n" : "True\n");
    output += "OrderID: ";
    output += ((activeOrderId != -1) ? std::to_string(activeOrderId) : "None");
    output += "\nTimeLeft: ";
    output += ((getTimeLeft() != 0) ? std::to_string(getTimeLeft()) : "None");
    output += "\nOrdersLeft: ";
    output += std::to_string(ordersLeft);
    return output;
}

