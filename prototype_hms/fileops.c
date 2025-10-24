/* 
 * Hospital Management System
 * File: fileops.c
 * Description: File operations for data persistence
 */

#include "hospital.h"

void savePatients() {
    FILE *fp = fopen("patients.dat", "wb");
    
    if(fp == NULL) {
        printf("*** Error: Unable to save patient data!\n");
        return;
    }
    
    fwrite(patients, sizeof(Patient), patientCount, fp);
    fclose(fp);
}

void loadPatients() {
    FILE *fp = fopen("patients.dat", "rb");
    
    if(fp == NULL) {
        printf("*** No previous patient data found. Starting fresh.\n");
        patientCount = 0;
        return;
    }
    
    patientCount = 0;
    while(fread(&patients[patientCount], sizeof(Patient), 1, fp) == 1) {
        patientCount++;
    }
    
    fclose(fp);
    printf(" Loaded %d patient record(s).\n", patientCount);
}

void saveDoctors() {
    FILE *fp = fopen("doctors.dat", "wb");
    
    if(fp == NULL) {
        printf("*** Error: Unable to save doctor data!\n");
        return;
    }
    
    fwrite(doctors, sizeof(Doctor), doctorCount, fp);
    fclose(fp);
}

void loadDoctors() {
    FILE *fp = fopen("doctors.dat", "rb");
    
    if(fp == NULL) {
        printf("*** No previous doctor data found. Starting fresh.\n");
        doctorCount = 0;
        return;
    }
    
    doctorCount = 0;
    while(fread(&doctors[doctorCount], sizeof(Doctor), 1, fp) == 1) {
        doctorCount++;
    }
    
    fclose(fp);
    printf(" Loaded %d doctor record(s).\n", doctorCount);
}

void saveAppointments() {
    FILE *fp = fopen("appointments.dat", "wb");
    
    if(fp == NULL) {
        printf("*** Error: Unable to save appointment data!\n");
        return;
    }
    
    fwrite(appointments, sizeof(Appointment), appointmentCount, fp);
    fclose(fp);
}

void loadAppointments() {
    FILE *fp = fopen("appointments.dat", "rb");
    
    if(fp == NULL) {
        printf("*** No previous appointment data found. Starting fresh.\n");
        appointmentCount = 0;
        return;
    }
    
    appointmentCount = 0;
    while(fread(&appointments[appointmentCount], sizeof(Appointment), 1, fp) == 1) {
        appointmentCount++;
    }
    
    fclose(fp);
    printf(" Loaded %d appointment record(s).\n", appointmentCount);
}

void saveRecords() {
    FILE *fp = fopen("medical_records.dat", "wb");
    
    if(fp == NULL) {
        printf("*** Error: Unable to save medical records!\n");
        return;
    }
    
    fwrite(records, sizeof(MedicalRecord), recordCount, fp);
    fclose(fp);
}

void loadRecords() {
    FILE *fp = fopen("medical_records.dat", "rb");
    
    if(fp == NULL) {
        printf("*** No previous medical records found. Starting fresh.\n");
        recordCount = 0;
        return;
    }
    
    recordCount = 0;
    while(fread(&records[recordCount], sizeof(MedicalRecord), 1, fp) == 1) {
        recordCount++;
    }
    
    fclose(fp);
    printf(" Loaded %d medical record(s).\n", recordCount);
}
