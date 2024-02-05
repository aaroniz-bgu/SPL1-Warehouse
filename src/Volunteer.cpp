#include "../include/Volunteer.h"

// ###########################################################################
// ###                          AbstractVolunteer                          ###
// ###########################################################################

Volunteer::Volunteer(int id, const string &name) :
    completedOrderId(NO_ORDER),
    activeOrderId(NO_ORDER),
    id(id),
    name(name)
{ }

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
    return new CollectorVolunteer(*this);
}

void CollectorVolunteer::step()  {
    if(activeOrderId != NO_ORDER) {
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
 * @note Does not assign itself as the order's collector volunteer &
 * does not update the status - must be done by the caller.
 * @param order
 */
void CollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)) {
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
    if(activeOrderId != NO_ORDER) {
        output += "True\nOrderID: ";
        output += std::to_string(activeOrderId);
        output += "\nTimeLeft: ";
        output += std::to_string(timeLeft);
    } else {
        output += "False\n OrderID: None\nTimeLeft: None";
    }
    output += "\nOrdersLeft: No Limit";
    return output;
}

void CollectorVolunteer::visit(vector<Volunteer *> &freeCollectors, vector<Volunteer *> &freeDrivers) {
    if(!isBusy()) {
        freeCollectors.push_back(this);
    }
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
 * @note Does not assign itself as the order's collector volunteer &
 * does not update the status - must be done by the caller.
 * @param order
 */
void LimitedCollectorVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)) {
        CollectorVolunteer::acceptOrder(order);
        ordersLeft -= 1;
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
    if(activeOrderId != NO_ORDER) {
        output += "True\nOrderID: ";
        output += std::to_string(activeOrderId);
        output += "\nTimeLeft: ";
        output += std::to_string(getTimeLeft());
    } else {
        output += "False\n OrderID: None\nTimeLeft: None";
    }
    output += "\nOrdersLeft: ";
    output += std::to_string(ordersLeft);
    return output;
}

// ########################################################################### //
// ###                           DriverVolunteer                           ### //
// ########################################################################### //

/**
 * @param id the volunteer's identifier.
 * @param name the volunteer's name.
 * @param maxDistance the maximum distance the volunteer can travel.
 * @param distancePerStep the distance the volunteer travels in each step.
 */
DriverVolunteer::DriverVolunteer(int id, string name, int maxDistance, int distancePerStep) :
Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

/**
 * @returns a copy of the volunteer.
 */
DriverVolunteer *DriverVolunteer::clone() const{
    return new DriverVolunteer(*this);
}

/**
 * @returns how much distance is left for the order to be delivered.
 */
int DriverVolunteer::getDistanceLeft() const{
    return distanceLeft;
}

/**
 * @returns the maximum distance the volunteer can travel.
 */
int DriverVolunteer::getMaxDistance() const{
    return maxDistance;
}

/**
 * @returns the distance the volunteer travels in each step.
 */
int DriverVolunteer::getDistancePerStep() const{
    return distancePerStep;
}

/**
 * Decrease distanceLeft by distancePerStep.
 * @returns true if distanceLeft<=0,false otherwise.
 */
bool DriverVolunteer::decreaseDistanceLeft() {
    if(distanceLeft > 0) {
        distanceLeft = std::max(0, distanceLeft - distancePerStep);
        return distanceLeft == 0;
    }
    return true;
}

/**
 * @returns true if the volunteer is still able to take orders, otherwise false.
 */
bool DriverVolunteer::hasOrdersLeft() const {
    return true;
}

/**
 * @returns Signals whether the volunteer is not busy and the order is within the maxDistance.
 */
bool DriverVolunteer::canTakeOrder(const Order &order) const {
    return activeOrderId == NO_ORDER && order.getDistance() <= maxDistance;
}

/**
 * Assigns the order to this driver.
 * @note Does not assign itself as the order's driver volunteer &
 * does not update the status - must be done by the caller.
 * @param order The order to be assigned.
 */
void DriverVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)) {
        activeOrderId = order.getId();
        distanceLeft = order.getDistance();
    }
}

// Decrease distanceLeft by distancePerStep
void DriverVolunteer::step() {
    if(activeOrderId != NO_ORDER) {
        if(distanceLeft != 0 && decreaseDistanceLeft()) {
            completedOrderId = activeOrderId;
            activeOrderId = NO_ORDER;
        }
    }
}

/**
 * @returns a string description of the volunteer.
 */
string DriverVolunteer::toString() const {
    string output = "VolunteerID: ";
    output += std::to_string(getId());
    output += "\nisBusy: ";
    if(activeOrderId != NO_ORDER) {
        output += "True\nOrderID: ";
        output += std::to_string(activeOrderId);
        output += "\nDistanceLeft: ";
        output += std::to_string(distanceLeft);
    } else {
        output += "False\n OrderID: None\nTimeLeft: None";
    }
    output += "\nOrdersLeft: No Limit";
    return output;
}

void DriverVolunteer::visit(vector<Volunteer *> &freeCollectors, vector<Volunteer *> &freeDrivers) {
    if(!isBusy()) {
        freeDrivers.push_back(this);
    }
}

// ########################################################################### //
// ###                       LimitedDriverVolunteer                        ### //
// ########################################################################### //

/**
 * @param id the volunteer's identifier.
 * @param name the volunteer's name.
 * @param maxDistance the maximum distance the volunteer can travel.
 * @param distancePerStep the distance the volunteer travels in each step.
 * @param maxOrders the maximum number of orders the volunteer can take.
 */
LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders) :
DriverVolunteer(id, name, maxDistance, distancePerStep), maxOrders(maxOrders), ordersLeft(maxOrders) {}

/**
 * @returns a copy of this volunteer.
 */
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const {
    return new LimitedDriverVolunteer(*this);
}

/**
 * @returns the maximal number of orders this volunteer is able to make.
 */
int LimitedDriverVolunteer::getMaxOrders() const {
    return maxOrders;
}

/**
 * @returns the number of orders the volunteer can still take.
 */
int LimitedDriverVolunteer::getNumOrdersLeft() const {
    return ordersLeft;
}

/**
 * @returns true if the volunteer is still able to take orders, otherwise false.
 */
bool LimitedDriverVolunteer::hasOrdersLeft() const {
    return ordersLeft > 0;
}

/**
 * Signal if the volunteer is not busy, the order is within the maxDistance.
 * @returns true if the volunteer can take the order, otherwise false.
 */
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const {
    return activeOrderId == NO_ORDER && order.getDistance() <= getMaxDistance() && ordersLeft > 0;
}

/**
 * Assigns the order to this driver.
 * @note Does not assign itself as the order's driver volunteer &
 * does not update the status - must be done by the caller.
 * @param order The order to be assigned.
 */
void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    if(canTakeOrder(order)) {
        DriverVolunteer::acceptOrder(order);
        ordersLeft -= 1;
    }
}

/**
 * @returns a string description of the volunteer.
 */
string LimitedDriverVolunteer::toString() const {
    string output = "VolunteerID: ";
    output += std::to_string(getId());
    output += "\nisBusy: ";
    if(activeOrderId != NO_ORDER) {
        output += "True\nOrderID: ";
        output += std::to_string(activeOrderId);
        output += "\nDistanceLeft: ";
        output += std::to_string(getDistanceLeft());
    } else {
        output += "False\n OrderID: None\nTimeLeft: None";
    }
    output += "\nOrdersLeft: ";
    output += std::to_string(ordersLeft);
    return output;
}
