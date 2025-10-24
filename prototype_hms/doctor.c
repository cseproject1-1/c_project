/* 
 * Hospital Management System
 * File: doctor.c
 * Description: Doctor management module
 */

#include "hospital.h"

void doctorMenu() {
    int choice;
    
    do {
        displayHeader("DOCTOR MANAGEMENT");
        printf("1. Add New Doctor\n");
        printf("2. View All Doctors\n");
        printf("3. Search Doctor\n");
        printf("4. Update Doctor\n");
        printf("5. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                addDoctor();
                break;
            case 2:
                viewAllDoctors();
                break;
            case 3:
                searchDoctor();
                break;
            case 4:
                updateDoctor();
                break;
            case 5:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\n*** Invalid choice!\n");
        }
        
        if(choice != 5) {
            pauseScreen();
        }
    } while(choice != 5);
}

void addDoctor() {
    if(doctorCount >= MAX_DOCTORS) {
        printf("\n*** Doctor database is full!\n");
        return;
    }
    
    Doctor newDoctor;
    
    displayHeader("ADD NEW DOCTOR");
    
    // Auto-generate ID
    newDoctor.id = doctorCount + 1;
    printf("Doctor ID: %d (Auto-generated)\n\n", newDoctor.id);
    
    // Get doctor details
    printf("Enter doctor name: ");
    fgets(newDoctor.name, sizeof(newDoctor.name), stdin);
    newDoctor.name[strcspn(newDoctor.name, "\n")] = 0;
    
    printf("Enter specialization: ");
    fgets(newDoctor.specialization, sizeof(newDoctor.specialization), stdin);
    newDoctor.specialization[strcspn(newDoctor.specialization, "\n")] = 0;
    
    getValidPhone(newDoctor.contact);
    
    printf("Enter years of experience: ");
    scanf("%d", &newDoctor.experience);
    clearInputBuffer();
    
    printf("Enter consultation fee: ");
    scanf("%f", &newDoctor.consultationFee);
    clearInputBuffer();
    
    // Add to array
    doctors[doctorCount] = newDoctor;
    doctorCount++;
    
    // Save to file
    saveDoctors();
    
    printf("\n Doctor added successfully!\n");
    printf("Doctor ID: %d, Name: Dr. %s\n", newDoctor.id, newDoctor.name);
}

void viewAllDoctors() {
    displayHeader("DOCTOR LIST");
    
    if(doctorCount == 0) {
        printf("\n No doctors found in the database!\n");
        return;
    }
    
    printf("\nTotal Doctors: %d\n\n", doctorCount);
    
    printf("%-5s %-20s %-20s %-15s %-8s %-10s\n", 
           "ID", "Name", "Specialization", "Contact", "Exp(Yrs)", "Fee");
    printf("────────────────────────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < doctorCount; i++) {
        printf("%-5d %-20s %-20s %-15s %-8d $%-9.2f\n",
               doctors[i].id,
               doctors[i].name,
               doctors[i].specialization,
               doctors[i].contact,
               doctors[i].experience,
               doctors[i].consultationFee);
    }
}

void searchDoctor() {
    int choice;
    
    displayHeader("SEARCH DOCTOR");
    printf("1. Search by ID\n");
    printf("2. Search by Specialization\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    if(choice == 1) {
        int id;
        printf("\nEnter Doctor ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        
        int index = searchDoctorByID(id);
        
        if(index == -1) {
            printf("\n*** Doctor with ID %d not found!\n", id);
            return;
        }
        
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║       DOCTOR DETAILS                   ║\n");
        printf("╚════════════════════════════════════════╝\n\n");
        printf("Doctor ID:        %d\n", doctors[index].id);
        printf("Name:             Dr. %s\n", doctors[index].name);
        printf("Specialization:   %s\n", doctors[index].specialization);
        printf("Contact:          %s\n", doctors[index].contact);
        printf("Experience:       %d years\n", doctors[index].experience);
        printf("Consultation Fee: $%.2f\n", doctors[index].consultationFee);
        
    } else if(choice == 2) {
        char spec[50];
        printf("\nEnter specialization: ");
        fgets(spec, sizeof(spec), stdin);
        spec[strcspn(spec, "\n")] = 0;
        
        int found = 0;
        printf("\n╔════════════════════════════════════════╗\n");
        printf("║       SEARCH RESULTS                   ║\n");
        printf("╚════════════════════════════════════════╝\n\n");
        
        for(int i = 0; i < doctorCount; i++) {
            if(strstr(doctors[i].specialization, spec) != NULL) {
                printf("ID: %d, Dr. %s (%s), Fee: $%.2f\n",
                       doctors[i].id, doctors[i].name, 
                       doctors[i].specialization, doctors[i].consultationFee);
                found = 1;
            }
        }
        
        if(!found) {
            printf("*** No doctors found with specialization '%s'\n", spec);
        }
    } else {
        printf("\n Invalid choice!\n");
    }
}

void updateDoctor() {
    int id;
    
    displayHeader("UPDATE DOCTOR");
    
    printf("Enter Doctor ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = searchDoctorByID(id);
    
    if(index == -1) {
        printf("\n*** Doctor not found!\n");
        return;
    }
    
    printf("\nCurrent doctor: Dr. %s\n", doctors[index].name);
    printf("\nWhat do you want to update?\n");
    printf("1. Name\n");
    printf("2. Specialization\n");
    printf("3. Contact\n");
    printf("4. Experience\n");
    printf("5. Consultation Fee\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();
    
    switch(choice) {
        case 1:
            printf("Enter new name: ");
            fgets(doctors[index].name, sizeof(doctors[index].name), stdin);
            doctors[index].name[strcspn(doctors[index].name, "\n")] = 0;
            break;
        case 2:
            printf("Enter new specialization: ");
            fgets(doctors[index].specialization, sizeof(doctors[index].specialization), stdin);
            doctors[index].specialization[strcspn(doctors[index].specialization, "\n")] = 0;
            break;
        case 3:
            getValidPhone(doctors[index].contact);
            break;
        case 4:
            printf("Enter new experience (years): ");
            scanf("%d", &doctors[index].experience);
            clearInputBuffer();
            break;
        case 5:
            printf("Enter new consultation fee: ");
            scanf("%f", &doctors[index].consultationFee);
            clearInputBuffer();
            break;
        default:
            printf("\n*** Invalid choice!\n");
            return;
    }
    
    saveDoctors();
    printf("\n Doctor updated successfully!\n");
}
