/*
 * Hospital Management System
 * File: reports.c
 * Description: Reports and statistics module
 */

#include "hospital.h"

void reportsMenu() {
    int choice;

    do {
        displayHeader("REPORTS & STATISTICS");
        printf("1. Patient Statistics\n");
        printf("2. Doctor Schedule\n");
        printf("3. Revenue Report\n");
        printf("4. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch(choice) {
            case 1:
                generatePatientStats();
                break;
            case 2:
                generateDoctorSchedule();
                break;
            case 3:
                generateRevenueReport();
                break;
            case 4:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\n*** Invalid choice!\n");
        }

        if(choice != 4) {
            pauseScreen();
        }
    } while(choice != 4);
}

void generatePatientStats() {
    displayHeader("PATIENT STATISTICS");

    if(patientCount == 0) {
        printf("\n*** No patients to analyze!\n");
        return;
    }

    printf("\n OVERVIEW\n");
    printf("═══════════════════════════════════════════\n");
    printf("Total Patients: %d\n\n", patientCount);

    // Gender statistics
    int maleCount = 0, femaleCount = 0;
    for(int i = 0; i < patientCount; i++) {
        if(patients[i].gender == 'M' || patients[i].gender == 'm') {
            maleCount++;
        } else {
            femaleCount++;
        }
    }

    printf(" GENDER DISTRIBUTION\n");
    printf("═══════════════════════════════════════════\n");
    printf("Male Patients:   %d (%.1f%%)\n", maleCount,
           (maleCount * 100.0) / patientCount);
    printf("Female Patients: %d (%.1f%%)\n\n", femaleCount,
           (femaleCount * 100.0) / patientCount);

    // Blood group statistics
    char bloodGroups[][5] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
    int bloodGroupCounts[8] = {0};

    for(int i = 0; i < patientCount; i++) {
        for(int j = 0; j < 8; j++) {
            if(strcmp(patients[i].bloodGroup, bloodGroups[j]) == 0) {
                bloodGroupCounts[j]++;
                break;
            }
        }
    }

    printf("BLOOD GROUP DISTRIBUTION\n");
    printf("====================================\n");
    for(int i = 0; i < 8; i++) {
        if(bloodGroupCounts[i] > 0) {
            printf("%-4s: %d patient(s)\n", bloodGroups[i], bloodGroupCounts[i]);
        }
    }

    // Age statistics
    int ageSum = 0;
    int minAge = 120, maxAge = 0;
    for(int i = 0; i < patientCount; i++) {
        ageSum += patients[i].age;
        if(patients[i].age < minAge) minAge = patients[i].age;
        if(patients[i].age > maxAge) maxAge = patients[i].age;
    }

    printf("\n AGE STATISTICS\n");
    printf("====================================\n");
    printf("Average Age: %.1f years\n", (float)ageSum / patientCount);
    printf("Youngest:    %d years\n", minAge);
    printf("Oldest:      %d years\n", maxAge);
}

void generateDoctorSchedule() {
    displayHeader("DOCTOR SCHEDULE");

    if(doctorCount == 0) {
        printf("\n*** No doctors in database!\n");
        return;
    }

    printf("\n DOCTOR AVAILABILITY & APPOINTMENTS\n");
    printf("=================================================================\n\n");

    for(int i = 0; i < doctorCount; i++) {
        printf("Dr. %s (%s)\n", doctors[i].name, doctors[i].specialization);
        printf("Contact: %s | Fee: $%.2f\n", doctors[i].contact,
               doctors[i].consultationFee);

        // Count appointments for this doctor
        int apptCount = 0;
        printf("Appointments:\n");

        for(int j = 0; j < appointmentCount; j++) {
            if(appointments[j].doctorId == doctors[i].id) {
                int pIndex = searchPatientByID(appointments[j].patientId);
                printf("  - %s at %s on %s [%s]\n",
                       (pIndex != -1) ? patients[pIndex].name : "Unknown",
                       appointments[j].time,
                       appointments[j].date,
                       appointments[j].status);
                apptCount++;
            }
        }

        if(apptCount == 0) {
            printf("  No appointments scheduled\n");
        }

        printf("Total Appointments: %d\n", apptCount);
        printf("───────────────────────────────────────────────────────────\n");
    }
}

void generateRevenueReport() {
    displayHeader("REVENUE REPORT");

    if(recordCount == 0) {
        printf("\n*** No medical records available for revenue calculation!\n");
        return;
    }

    float totalRevenue = 0;
    float doctorRevenue[MAX_DOCTORS] = {0};

    printf("\n FINANCIAL SUMMARY\n");
    printf("═══════════════════════════════════════════════════════════\n\n");

    // Calculate total revenue and per-doctor revenue
    for(int i = 0; i < recordCount; i++) {
        totalRevenue += records[i].treatmentCost;

        // Add to doctor's revenue
        int dIndex = searchDoctorByID(records[i].doctorId);
        if(dIndex != -1) {
            doctorRevenue[dIndex] += records[i].treatmentCost;
        }
    }

    printf("Total Revenue: $%.2f\n", totalRevenue);
    printf("Total Treatments: %d\n", recordCount);
    printf("Average Treatment Cost: $%.2f\n\n", totalRevenue / recordCount);

    printf(" REVENUE BY DOCTOR\n");
    printf("═══════════════════════════════════════════════════════════\n");

    for(int i = 0; i < doctorCount; i++) {
        if(doctorRevenue[i] > 0) {
            printf("Dr. %-20s: $%10.2f (%.1f%%)\n",
                   doctors[i].name,
                   doctorRevenue[i],
                   (doctorRevenue[i] / totalRevenue) * 100);
        }
    }

    // Appointment statistics
    int scheduledCount = 0, completedCount = 0, cancelledCount = 0;
    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].status, "Scheduled") == 0) {
            scheduledCount++;
        } else if(strcmp(appointments[i].status, "Completed") == 0) {
            completedCount++;
        } else if(strcmp(appointments[i].status, "Cancelled") == 0) {
            cancelledCount++;
        }
    }

    printf("\nAPPOINTMENT STATISTICS\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("Total Appointments: %d\n", appointmentCount);
    printf("Scheduled:          %d\n", scheduledCount);
    printf("Completed:          %d\n", completedCount);
    printf("Cancelled:          %d\n", cancelledCount);
}
