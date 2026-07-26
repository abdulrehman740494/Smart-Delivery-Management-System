#include "RequestQueue.h"
#include "Package.h"
#include <iostream>


static PickupRequest queueArray[MAX_QUEUE_SIZE];
static int front = 0;
static int rear = 0;
static int count = 0;
static int nextRequestId = 1;

bool queueIsFull()  { return count == MAX_QUEUE_SIZE; }
bool queueIsEmpty() { return count == 0; }
int queueSize()     { return count; }

bool enqueueRequest(const std::string& customerName, const std::string& pickupDetails) {
    if (queueIsFull()) return false;

    queueArray[rear].requestId = nextRequestId++;
    queueArray[rear].customerName = customerName;
    queueArray[rear].pickupDetails = pickupDetails;

    rear = (rear + 1) % MAX_QUEUE_SIZE; // wrap around
    ++count;
    return true;
}

bool dequeueRequest(PickupRequest& out) {
    if (queueIsEmpty()) return false;

    out = queueArray[front];
    front = (front + 1) % MAX_QUEUE_SIZE; // wrap around
    --count;
    return true;
}

bool queuePeekAt(int offsetFromFront, PickupRequest& out) {
    if (offsetFromFront < 0 || offsetFromFront >= count) return false;
    out = queueArray[(front + offsetFromFront) % MAX_QUEUE_SIZE];
    return true;
}

