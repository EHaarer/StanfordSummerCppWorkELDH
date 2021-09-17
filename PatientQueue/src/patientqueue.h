// This .h file was created by Ethan Haarer and Aidan Mundy for Stanford's cs106b.
// Alongside patientqueue.cpp, it forms a patient queue built on binary heaps.

#ifndef _patientqueue_h
#define _patientqueue_h

#include <iostream>
#include <string>
#include "patient.h"
using namespace std;

class PatientQueue {
public:
    /**
     * This class is a PatientQueue built on a binary heap system. It handles patient queueing for
     * a hospital given patient names and priority values.
     *
     * @param isMinHeap Pass true if the queue should be a minHeap, false if it should be a maxHeap
     */
    PatientQueue(bool isMinHeap = true);
    /// Deconstructor for PatientQueue.
    ~PatientQueue();
    /**
     * Given a name and a new priority, this function changes the priority of a patent and re sorts
     * the PatientQueue accordingly.
     * @param value Name of the patient to modify.
     * @param newPriority New priority value for the patient.
     */
    void changePriority(string value, int newPriority);
    /**
     * Sets all index positions greater than 0 in the myPatients array to an empty Patient.
     */
    void clear();
    /**
     * Runs before every operation of the PatientQueue. In its current state, it does nothing and
     * it is completely blank.
     */
    void debug();
    /**
     * Removes the first patient in the PatientQueue from the queue and returns their name.
     * Also sorts the PatientQueue to appropriately fill the newly empty space.
     * @return The name of the first patient in the PatientQueue.
     */
    string dequeue();
    /**
     * Adds a new patient to the PatientQueue and re sorts the queue accordingly.
     * @param value Name of the patient.
     * @param priority Priority of the patient.
     */
    void enqueue(string value, int priority);
    /**
     * Tells whether or not the PatientQueue is empty.
     * @return True if PatientQueue is empty, false if not.
     */
    bool isEmpty() const;
    /**
     * Returns the name of the first Patient in the queue.
     * @return the name of the first Patient in the queue.
     */
    string peek() const;
    /**
     * Returns the priority of the first Patient in the queue.
     * @return the priority of the first Patient in the queue.
     */
    int peekPriority() const;
    /**
     * Returns the size of the PatientQueue.
     * @return the size of the PatientQueue.
     */
    int size() const;
    /**
     * This is an operator overload for the << operator. It is used to print the PatientQueue
     * or to send the PatientQueue to an output stream.
     * @param out The output stream to output to.
     * @param queue The PatientQueue to print out/send to the "out" output stream.
     * @return The "out" output stream that was passed into the function.
     */
    friend ostream& operator <<(ostream& out, const PatientQueue& queue);

private:
    // Initial array of patients that starts at size 10.
    Patient* myPatients = new Patient[10];
    // The size of the myPatients array, including index 0.
    int myPatientsSize = 10;
    // The number of patients in the myPatients array.
    int numPatients = 0;
    // True if the PatientQueue is a minHeap, false if it is a maxHeap.
    bool isMin;
    /**
     * Reorder the Patient at index up/forwards in the queue according to their priority value
     * and name.
     * @param index The index of the patient to sort.
     * @return True if a patient was moved, false otherwise.
     */
    bool bubbleUp(const int index);
    /**
     * Reorder the Patient at index down/backwards in the queue according to their priority value
     * and name.
     * @param index The index of the patient to sort.
     */
    void bubbleDown(const int index);
    /**
     * Compares the patient at index1 to the patient at index2 by their priorities and names.
     * Will return true if the patient at index1 should be ahead of the patient at index2.
     * @param index1 The index of the fist patient to compare.
     * @param index2 The index of the second patient to compare.
     * @return True if the patient at index1 should be ahead of the patient at index2 in the queue.
     *         Also true if the patients at index1 and index2 have the same name and priority.
     */
    bool bubbleHelper(const int index1, const int index2);
    /**
     * Swaps the patient at index with its parent at index / 2 and then bubbles down the patient
     * that is now at index / 2.
     * @param index The index of the child to swap with its parent and then bubble.
     */
    void bubbleDownSwap(const int index);
};

#endif
