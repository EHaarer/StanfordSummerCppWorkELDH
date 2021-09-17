// This code was written by Ethan Haarer and Aidan Mundy for Stanford's cs106b. It runs the logic for doctor / patient scheduling called Doctors Without Orders.
#include "DoctorsWithoutOrders.h"
#include "set.h"
#include "map.h"
#include <climits>
#include <iostream>
using namespace std;

bool canAllPatientsBeSeen(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule);
bool checkRemainingHours(Doctor& inDoc, Patient& inPat, Vector<Patient>& patients, Map<string, Set<string>>& schedule);
bool scheduleHelper(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule, int patientIndex);

/**
 * Given a list of doctors and a list of patients, determines whether all the patients can
 * be seen. If so, this function fills in the schedule outparameter with a map from doctors
 * to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @return Whether or not a schedule was found.
 */
bool canAllPatientsBeSeen(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule) {
    return scheduleHelper(doctors, patients, schedule, 0);
}

/**
 * Given a list of doctors and a list of patients, recursively determines whether all the
 * patients can be seen. If so, this function fills in the schedule outparameter with a map
 * from doctors to the set of patients that doctor would see.
 *
 * @param doctors  The list of the doctors available to work.
 * @param patients The list of the patients that need to be seen.
 * @param schedule An outparameter that will be filled in with the schedule, should one exist.
 * @param patientIndex The number of patients that have been scheduled in this branch.
 * @return Whether or not a schedule was found.
 */
bool scheduleHelper(Vector<Doctor> &doctors, Vector<Patient> &patients, Map<string, Set<string>>& schedule, int patientIndex) {
    // Base Case
    if (patientIndex == patients.size()) {  // If we completed looping through all patients, return true.
        return true;
    }

    // Recursive Case
    for (int i = 0; i < doctors.size(); i++) {
        if (checkRemainingHours(doctors[i], patients[patientIndex], patients, schedule)){
            schedule[doctors[i].name].add(patients[patientIndex].name);     // Choose: Assign current patient to current doctor's schedule
            if(scheduleHelper(doctors, patients, schedule, patientIndex + 1)) {   // Explore: Recursively calls the next step in the scheduling branch.
                return true;
            }
            schedule[doctors[i].name].remove(patients[patientIndex].name);  // UnChoose: Remove current patient from current doctor.
        }
    }
    return false; // If current patient at Patient index cannot be assigned to any doctor, return false.
}

/**
 * Given a list of doctors and a list of patients, determines the remaining
 * Given a doctor, a patient, and the complete list of patients, determines if
 * the patient will fit in the doctors schedule.
 *
 * @param inDoc    The doctor to check.
 * @param inPat    The patient to check
 * @param patients A vector of all patients.
 * @param schedule The current schedule.
 * @return Whether or not the inPat will fit in the inDoc's schedule.
 */
bool checkRemainingHours(Doctor& inDoc, Patient& inPat, Vector<Patient>& patients, Map<string, Set<string>>& schedule) {
    int numHoursLeft = inDoc.hoursFree;

    // This sets numHoursLeft to the doctors current available hours.
    for (Patient person : patients){
        if (schedule[inDoc.name].contains(person.name)){
            numHoursLeft -= person.hoursNeeded;
        }
    }

    return !(numHoursLeft - inPat.hoursNeeded < 0); // Returns true if patient fits in the schedule.
}
