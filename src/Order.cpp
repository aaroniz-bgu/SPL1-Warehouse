#include <stdexcept>
#include "Order.h"

/**
 * Constructor
 * @param id - order identifier
 * @param customerId - customer identifier
 * @param distance - distance of the customer
 */
Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance) {
    status = OrderStatus::PENDING;
    collectorId = NO_VOLUNTEER;
    driverId = NO_VOLUNTEER;
}

/**
 * Returns the order's id
 */
int Order::getId() const {
    return id;
}

/**
 * Returns the order's customer id
 */
int Order::getCustomerId() const {
    return customerId;
}

/**
 * Sets the order's status
 */
void Order::setStatus(OrderStatus status) {
    this->status = status;
}

/**
 * Sets the order's collector id
 */
void Order::setCollectorId(int collectorId) {
    this->collectorId = collectorId;
}

/**
 * Sets the order's driver id
 */
void Order::setDriverId(int driverId) {
    this->driverId = driverId;
}

/**
 * Returns the order's collector id
 */
int Order::getCollectorId() const {
    return collectorId;
}

/**
 * Returns the order's driver id
 */
int Order::getDriverId() const {
    return driverId;
}

/**
 * Returns the order's status
 */
OrderStatus Order::getStatus() const {
    return status;
}

/**
 * Returns a descriptor of the order
 */
const string Order::toString() const {
    //TODO
    throw std::logic_error("Unimplemented method");
}
