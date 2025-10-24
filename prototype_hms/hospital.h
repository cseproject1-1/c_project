/* 
 * Hospital Management System
 * File: hospital.h
 * Description: Header file with all structure definitions and function prototypes
 */

#ifndef HOSPITAL_H
#define HOSPITAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constants
#define MAX_PATIENTS 100
#define MAX_DOCTORS 50
#define MAX_APPOINTMENTS 200
#define MAX_RECORDS 200

// Patient Structure
typedef struct {
    int id;
    char name[50];
    int age;
    char gender;
    char bloodGroup[5];
    char contact[15];
    char address[100];
    char disease[50];
    char admissionDate[15];
} Patient;

// Doctor Structure
typedef struct {
    int id;
    char name[50];
    char specialization[50];
    char contact[15];
    int experience;
    float consultationFee;
} Doctor;

// Appointment Structure
typedef struct {
    int appointmentId;
    int patientId;
    int doctorId;
    char date[15];
    char time[10];
    char status[20]; // Scheduled, Completed, Cancelled
} Appointment;

// Medical Record Structure
typedef struct {
    int recordId;
    int patientId;
    int doctorId;
    char diagnosis[200];
    char prescription[500];
    char visitDate[15];
    float treatmentCost;
} MedicalRecord;

// Global Variables Declaration
extern Patient patients[MAX_PATIENTS];
extern Doctor doctors[MAX_DOCTORS];
extern Appointment appointments[MAX_APPOINTMENTS];
extern MedicalRecord records[MAX_RECORDS];

extern int patientCount;
extern int doctorCount;
extern int appointmentCount;
extern int recordCount;

// Function Prototypes - Patient Management
void patientMenu();
void addPatient();
void viewAllPatients();
void searchPatient();
void updatePatient();
void deletePatient();

// Function Prototypes - Doctor Management
void doctorMenu();
void addDoctor();
void viewAllDoctors();
void searchDoctor();
void updateDoctor();

// Function Prototypes - Appointment Management
void appointmentMenu();
void bookAppointment();
void viewAllAppointments();
void cancelAppointment();
void viewTodayAppointments();

// Function Prototypes - Medical Records
void medicalRecordsMenu();
void addDiagnosis();
void viewPatientHistory();
void addPrescription();

// Function Prototypes - Reports
void reportsMenu();
void generatePatientStats();
void generateDoctorSchedule();
void generateRevenueReport();

// Function Prototypes - File Operations
void savePatients();
void loadPatients();
void saveDoctors();
void loadDoctors();
void saveAppointments();
void loadAppointments();
void saveRecords();
void loadRecords();

// Function Prototypes - Search Operations
int searchPatientByID(int id);
int searchDoctorByID(int id);
int searchPatientByName(char name[]);
int searchAppointmentByID(int id);

// Function Prototypes - Utility Functions
int getValidAge();
void getValidBloodGroup(char bloodGroup[]);
void getValidPhone(char phone[]);
void getValidGender(char *gender);
int validateDate(char date[]);
void clearInputBuffer();
void pauseScreen();
void displayHeader(const char *title);

#endif
