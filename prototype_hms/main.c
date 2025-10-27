/* 
 * Hospital Management System
 * File: main.c
 * Description: Main program entry point with menu system
 */

#include "hospital.h"

// Global variable definitions
Patient patients[MAX_PATIENTS];
Doctor doctors[MAX_DOCTORS];
Appointment appointments[MAX_APPOINTMENTS];
MedicalRecord records[MAX_RECORDS];

int patientCount = 0;
int doctorCount = 0;
int appointmentCount = 0;
int recordCount = 0;

int main() {
    int choice;
    
    // Load all data from files
    loadPatients();
    loadDoctors();
    loadAppointments();
    loadRecords();
    
    // Display welcome banner
      char s[]="\n| Hospital management System |\n";
    for (int i=1;s[i]!='\0';i++)
    {
        printf("-");
         for (long long j=0;j<=40000000;j++)
        {

        }
    }

    for(int i=0;s[i]!='\0';i++)
    {
        printf("%c",s[i]);


        for (long long j=0;j<=100000000;j++)
        {

        }

    }
    for (int i=1;s[i]!='\0';i++)
    {
        printf("-");
         for (long long j=0;j<=40000000;j++)
        {

        }
    }
    // Main menu loop
    do {
       char s[]="\n|            MAIN MENU             |\n";
    for (int i=1;s[i]!='\0';i++)
    {
        printf("-");
         for (long long j=1;j<=40000000;j++)
        {
        }
    }

    for(int i=0;s[i]!='\0';i++)
    {
        printf("%c",s[i]);


        for (long long j=0;j<=100000000;j++)
        {
        }

    }
    for (int i=1;s[i]!='\0';i++)
    {
        printf("-");
         for (long long j=1;j<=40000000;j++)
        {
        }
    }  printf("\n");
        printf("1. Patient Management\n");
        printf("2. Doctor Management\n");
        printf("3. Appointment Management\n");
        printf("4. Medical Records\n");
        printf("5. Reports\n");
        printf("6. Exit\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                patientMenu();
                break;
            case 2:
                doctorMenu();
                break;
            case 3:
                appointmentMenu();
                break;
            case 4:
                medicalRecordsMenu();
                break;
            case 5:
                reportsMenu();
                break;
            case 6:
                printf("\n====================================\n");
                printf("|   Thank you for using HMS!       |\n");
                printf("|   Healthcare made digital & simple |\n");
                printf("====================================\n");
                break;
            default:
                printf("\n*** Invalid choice! Please try again.\n");
        }
    } while(choice != 6);
    
    return 0;
}
