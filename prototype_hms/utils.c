/*
 * Hospital Management System
 * File: utils.c
 * Description: Utility and validation functions
 */

#include "hospital.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pauseScreen() {
    printf("\nPress Enter to continue...");
    getchar();
}

void displayHeader(const char *title) {
    printf("\n");
    printf("====================================\n");
    printf("|  %-36s  |\n", title);
    printf("====================================\n");
    printf("\n");
}

int getValidAge() {
    int age;

    do {
        printf("Enter age (0-120): ");
        scanf("%d", &age);
        clearInputBuffer();

        if(age < 0 || age > 120) {
            printf("*** Invalid age! Must be between 0 and 120.\n");
        }
    } while(age < 0 || age > 120);

    return age;
}

void getValidBloodGroup(char bloodGroup[]) {
    int valid = 0;
    char validGroups[][5] = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};

    do {
        printf("Enter blood group (A+, A-, B+, B-, AB+, AB-, O+, O-): ");
        scanf("%s", bloodGroup);
        clearInputBuffer();

        valid = 0;
        for(int i = 0; i < 8; i++) {
            if(strcmp(bloodGroup, validGroups[i]) == 0) {
                valid = 1;
                break;
            }
        }

        if(!valid) {
            printf("*** Invalid blood group!\n");
        }
    } while(!valid);
}

void getValidPhone(char phone[]) {
    int valid;

    do {
        valid = 1;
        printf("Enter phone number (10 digits): ");
        scanf("%s", phone);
        clearInputBuffer();

        if(strlen(phone) != 10) {
            valid = 0;
            printf("*** Phone number must be exactly 10 digits!\n");
        } else {
            for(int i = 0; i < 10; i++) {
                if(phone[i] < '0' || phone[i] > '9') {
                    valid = 0;
                    printf("*** Phone number must contain only digits!\n");
                    break;
                }
            }
        }
    } while(!valid);
}

void getValidGender(char *gender) {
    do {
        printf("Enter gender (M/F): ");
        scanf(" %c", gender);
        clearInputBuffer();

        if(*gender != 'M' && *gender != 'F' && *gender != 'm' && *gender != 'f') {
            printf("*** Invalid gender! Please enter M or F.\n");
        } else {
            if(*gender == 'm') *gender = 'M';
            if(*gender == 'f') *gender = 'F';
            break;
        }
    } while(1);
}

int validateDate(char date[]) {
    if(strlen(date) != 10) return 0;
    if(date[2] != '/' || date[5] != '/') return 0;

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 +
               (date[8] - '0') * 10 + (date[9] - '0');

    if(month < 1 || month > 12) return 0;
    if(day < 1 || day > 31) return 0;
    if(year < 1900 || year > 2100) return 0;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(month == 2) {
        if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if(day > 29) return 0;
        } else {
            if(day > 28) return 0;
        }
    } else {
        if(day > daysInMonth[month - 1]) return 0;
    }

    return 1;
}

int searchPatientByID(int id) {
    for(int i = 0; i < patientCount; i++) {
        if(patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

int searchPatientByName(char name[]) {
    for(int i = 0; i < patientCount; i++) {
        if(strcmp(patients[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

int searchDoctorByID(int id) {
    for(int i = 0; i < doctorCount; i++) {
        if(doctors[i].id == id) {
            return i;
        }
    }
    return -1;
}

int searchAppointmentByID(int id) {
    for(int i = 0; i < appointmentCount; i++) {
        if(appointments[i].appointmentId == id) {
            return i;
        }
    }
    return -1;
}
