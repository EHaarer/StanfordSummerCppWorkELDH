// This .cpp file was created by Ethan Haarer and Aidan Mundy for Stanford's cs106b.
// Alongside patientqueue.h, it forms a patient queue built on binary heaps.

// Referenced https://www.geeksforgeeks.org/delete-in-c/ for use of the delete() method,
// specifically "6. Deleting variables of User Defined data types".
#include "patientqueue.h"

PatientQueue::PatientQueue(bool isMinHeap) {
    isMin = isMinHeap;
    clear(); // Clear to set all values in the array to a blank Patient.
}

PatientQueue::~PatientQueue() {
    delete(myPatients); // Clearing memory.
}

void PatientQueue::changePriority(string value, int newPriority) {
    // Loop through all patents in the array.
    for (int i = 1; i <= numPatients; i++) {
        // If a patient with the input name is found, change their priority and stop searching.
        if (myPatients[i].name == value) {
            myPatients[i].priority = newPriority;
            if(!bubbleUp(i)){ // Only run bubbleDown if bubbleUp did not change [i]'s position.
                bubbleDown(i);
            }
            return; // Stop after first matching patient is found and modified.
        }
    }
}

void PatientQueue::clear() {
    // Clears the array by setting blank patients to all index positions except 0.
    for (int i = 1; i < myPatientsSize; i++) {
        myPatients[i] = Patient();
    }
    numPatients = 0;
}

void PatientQueue::debug() {
    // Intentionally left empty.
}

string PatientQueue::dequeue() {
    if (isEmpty()){
        throw "Can't do that, there isn't anybody in the Queue!";
    }
    string outPatient = myPatients[1].name; // Temporarily store the patient before it is deleted.
    myPatients[1] = myPatients[numPatients]; // Bump up last patient to the front.
    myPatients[numPatients] = Patient(); // Clear last patients slot.
    numPatients--;
    bubbleDown(1); // Bubble down patient that was bumped up.
    return outPatient;
}

void PatientQueue::enqueue(string value, int priority) {
    // If the array is full, double the size of the array.
    if (numPatients >= myPatientsSize - 1){
        Patient* myPatientsResize = new Patient[myPatientsSize * 2];
        // Copy over all patients into the temporary array.
        for (int i = 1; i < myPatientsSize;i++) {
            myPatientsResize[i] = myPatients[i];
        }
        delete(myPatients); // Clearing memory.
        myPatientsSize *= 2;
        myPatients = myPatientsResize;
        delete(myPatientsResize); // Clearing memory.
    }

    // Add the new patient to the queue and then bubbleUp from its index.
    numPatients++;
    myPatients[numPatients] = Patient(value, priority);
    bubbleUp(numPatients);
}

bool PatientQueue::isEmpty() const {
    return numPatients == 0;
}

string PatientQueue::peek() const {
    if (isEmpty()){
        throw "Can't do that, there isn't anybody in the Queue!";
    }
    return myPatients[1].name;
}

int PatientQueue::peekPriority() const {
    if (isEmpty()){
        throw "Can't do that, there isn't anybody in the Queue!";
    }
    return myPatients[1].priority;
}

int PatientQueue::size() const {
    return numPatients;
}

ostream& operator <<(ostream& out, const PatientQueue& queue) {
    out << "{";
    // If the queue isnt empty, print the first Patient. Necessary because of fencepost pattern.
    if (!queue.isEmpty()){
        out << queue.myPatients[1];
    }
    // Print the remainder of the patients in the queue.
    for (int i = 2; i <= queue.numPatients; i++) {
        out << ", " << queue.myPatients[i];
    }
    out << "}";
    return out;
}

bool PatientQueue::bubbleUp(const int index){
    // Dont run the function if the index is out of bounds or if the parent index is out of bounds.
    if (index >= myPatientsSize || index < 1 || index / 2 < 1) {
        return false;
    }

    // If the child at [index] should be further ahead in the queue than its parent, swap the child
    // and its parent and then bubbleUp from the child's new index.
    if (bubbleHelper(index, index / 2)){
        swap(myPatients[index], myPatients[index / 2]);
        bubbleUp(index / 2);
        return true;
    }
    return false;
}

void PatientQueue::bubbleDown(const int index){
    // Dont run the function if the index is out of bounds.
    if (index < 1 || index >= myPatientsSize) {
        return;
    }

    // True if grandchild 1 should be further ahead in the queue than the child at [index].
    bool gc1 = bubbleHelper(index * 2, index);
    // True if grandchild 2 should be further ahead in the queue than the child at [index].
    bool gc2 = bubbleHelper((index * 2) + 1, index);

    // If both grandchildren should be further ahead, check which will actually be swapped.
    if (gc1 && gc2) {
        // If grandchild 1 should be further ahead in the queue than grandchild 2, swap
        // grandchild 1 and the child at [index] and then bubbleDown from the child's new index.
        if(bubbleHelper(index * 2, (index * 2) + 1)){
            bubbleDownSwap(index * 2);
        // Otherwise, swap grandchild 2 with the child at [index] and then bubbleDown from the
        // child's new index.
        } else {
            bubbleDownSwap((index * 2) + 1);
        }
    //  If only grandchild 1 should be further ahead in the queue than the child at [index],
    //  swap grandchild 1 and the child at [index] and then bubbleDown from the child's new index.
    } else if (gc1) {
        bubbleDownSwap(index * 2);
    //  If only grandchild 2 should be further ahead in the queue than the child at [index],
    //  swap grandchild 2 and the child at [index] and then bubbleDown from the child's new index.
    } else if (gc2) {
        bubbleDownSwap((index * 2) + 1);
    }
}

void PatientQueue::bubbleDownSwap(const int index){
    // Swap [index] with [index / 2] and then bubble from [index]'s new position at index / 2.
    swap(myPatients[index/2], myPatients[index]);
    bubbleDown(index);
}

bool PatientQueue::bubbleHelper(const int index1, const int index2){
    // Dont run the function if index1 or index2 are out of bounds.
    // Index1 and index2 are checked before using this function in multiple ways depending on what
    // this function is being used for, but a few implementations rely on the below if statement for
    // accurate results. None of them need index2 to be checked, however it is for safety's sake.
    if (index1 < 1 || index1 > numPatients || index2 < 1 || index2 > numPatients) {
        return false;
    }

    // True if [index1] should be ahead of [index2] in the queue according to priority values.
    bool priorityValue = (isMin ? myPatients[index1].priority < myPatients[index2].priority :
                                  myPatients[index1].priority > myPatients[index2].priority);
    // True if [index1] and [index2] have the same priority values.
    bool prioritySame = myPatients[index1].priority == myPatients[index2].priority;
    // True if [index1] should be ahead of [index2] in the queue according to names.
    // Also true if [index1] and [index2] have the same name.
    bool nameValue = (isMin ? myPatients[index1].name <= myPatients[index2].name :
                              myPatients[index1].name >= myPatients[index2].name);

    // Returns true if [index1] should be ahead of [index2] in the queue.
    // Also returns true if [index1] == [index2] (ie they have the same names and priority).
    return priorityValue || (prioritySame && nameValue);
}
