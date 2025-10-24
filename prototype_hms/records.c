/* 
 * Hospital Management System
 * File: records.c
 * Description: Medical records management module
 */

#include "hospital.h"

void medicalRecordsMenu() {
    int choice;
    
    do {
        displayHeader("MEDICAL RECORDS");
        printf("1. Add Diagnosis\n");
        printf("2. View Patient History\n");
        printf("3. Add Prescription\n");
        printf("4. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                addDiagnosis();
                break;
            case 2:
                viewPatientHistory();
                break;
            case 3:
                addPrescription();
                break;
            case 4:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\ Invalid choice!\n");
        }
        
        if(choice != 4) {
            pauseScreen();
        }
    } while(choice != 4);
}

void addDiagnosis() {
    if(recordCount >= MAX_RECORDS) {
        printf("\n*** Medical records database is full!\n");
        return;
    }
    
    if(patientCount == 0) {
        printf("\n*** No patients in database!\n");
        return;
    }
    
    MedicalRecord newRecord;
    
    displayHeader("ADD DIAGNOSIS");
    
    // Auto-generate record ID
    newRecord.recordId = recordCount + 1;
    printf("Record ID: %d (Auto-generated)\n\n", newRecord.recordId);
    
    // Get patient ID
    printf("Enter Patient ID: ");
    scanf("%d", &newRecord.patientId);
    clearInputBuffer();
    
    int pIndex = searchPatientByID(newRecord.patientId);
    if(pIndex == -1) {
        printf("\n*** Patient not found!\n");
        return;
    }
    printf("Patient: %s\n", patients[pIndex].name);
    
    // Get doctor ID
    printf("Enter Doctor ID: ");
    scanf("%d", &newRecord.doctorId);
    clearInputBuffer();
    
    int dIndex = searchDoctorByID(newRecord.doctorId);
    if(dIndex == -1) {
        printf("\n*** Doctor not found!\n");
        return;
    }
    printf("Doctor: Dr. %s\n\n", doctors[dIndex].name);
    
    // Get diagnosis details
    printf("Enter diagnosis: ");
    fgets(newRecord.diagnosis, sizeof(newRecord.diagnosis), stdin);
    newRecord.diagnosis[strcspn(newRecord.diagnosis, "\n")] = 0;
    
    printf("Enter prescription: ");
    fgets(newRecord.prescription, sizeof(newRecord.prescription), stdin);
    newRecord.prescription[strcspn(newRecord.prescription, "\n")] = 0;
    
    printf("Enter visit date (DD/MM/YYYY): ");
    scanf("%s", newRecord.visitDate);
    clearInputBuffer();
    
    printf("Enter treatment cost: $");
    scanf("%f", &newRecord.treatmentCost);
    clearInputBuffer();
    
    // Add to array
    records[recordCount] = newRecord;
    recordCount++;
    
    // Save to file
    saveRecords();
    
    printf("\n Medical record added successfully!\n");
    printf("Record ID: %d\n", newRecord.recordId);
}

void viewPatientHistory() {
    int patientId;
    
    displayHeader("PATIENT MEDICAL HISTORY");
    
    printf("Enter Patient ID: ");
    scanf("%d", &patientId);
    clearInputBuffer();
    
    int pIndex = searchPatientByID(patientId);
    if(pIndex == -1) {
        printf("\n*** Patient not found!\n");
        return;
    }
    
    printf("\nMedical History for: %s (ID: %d)\n", patients[pIndex].name, patientId);
    printf("====================================================================\n\n");
    
    int found = 0;
    for(int i = 0; i < recordCount; i++) {
        if(records[i].patientId == patientId) {
            int dIndex = searchDoctorByID(records[i].doctorId);
            
            printf("Record ID: %d\n", records[i].recordId);
            printf("Date: %s\n", records[i].visitDate);
            printf("Doctor: %s\n", (dIndex != -1) ? doctors[dIndex].name : "Unknown");
            printf("Diagnosis: %s\n", records[i].diagnosis);
            printf("Prescription: %s\n", records[i].prescription);
            printf("Cost: $%.2f\n", records[i].treatmentCost);
            printf("========================================================\n");
            found = 1;
        }
    }
    
    if(!found) {
        printf(" No medical records found for this patient.\n");
    }
}

void addPrescription() {
    int recordId;
    
    displayHeader("ADD PRESCRIPTION");
    
    printf("Enter Record ID: ");
    scanf("%d", &recordId);
    clearInputBuffer();
    
    int index = -1;
    for(int i = 0; i < recordCount; i++) {
        if(records[i].recordId == recordId) {
            index = i;
            break;
        }
    }
    
    if(index == -1) {
        printf("\n*** Record not found!\n");
        return;
    }
    
    printf("\nCurrent Prescription: %s\n", records[index].prescription);
    printf("\nEnter new/updated prescription: ");
    fgets(records[index].prescription, sizeof(records[index].prescription), stdin);
    records[index].prescription[strcspn(records[index].prescription, "\n")] = 0;
    
    saveRecords();
    printf("\n Prescription updated successfully!\n");
}
