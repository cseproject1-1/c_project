/* 
 * Hospital Management System
 * File: patient.c
 * Description: Patient management module with all CRUD operations
 */

#include "hospital.h"

void patientMenu() {
    int choice;
    
    do {
        displayHeader("PATIENT MANAGEMENT");
        printf("1. Add New Patient\n");
        printf("2. View All Patients\n");
        printf("3. Search Patient\n");
        printf("4. Update Patient\n");
        printf("5. Delete Patient\n");
        printf("6. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                addPatient();
                break;
            case 2:
                viewAllPatients();
                break;
            case 3:
                searchPatient();
                break;
            case 4:
                updatePatient();
                break;
            case 5:
                deletePatient();
                break;
            case 6:
                printf("\nReturning to main menu...\n");
                break;
            default:
                printf("\n*** Invalid choice!\n");
        }
        
        if(choice != 6) {
            pauseScreen();
        }
    } while(choice != 6);
}

void addPatient() {
    if(patientCount >= MAX_PATIENTS) {
        printf("\n*** Patient database is full!\n");
        return;
    }
    
    Patient newPatient;
    
    displayHeader("ADD NEW PATIENT");
    
    // Auto-generate ID
    newPatient.id = patientCount + 1;
    printf("Patient ID: %d (Auto-generated)\n\n", newPatient.id);
    
    // Get patient details
    printf("Enter patient name: ");
    fgets(newPatient.name, sizeof(newPatient.name), stdin);
    newPatient.name[strcspn(newPatient.name, "\n")] = 0;
    
    newPatient.age = getValidAge();
    getValidGender(&newPatient.gender);
    getValidBloodGroup(newPatient.bloodGroup);
    getValidPhone(newPatient.contact);
    
    printf("Enter address: ");
    fgets(newPatient.address, sizeof(newPatient.address), stdin);
    newPatient.address[strcspn(newPatient.address, "\n")] = 0;
    
    printf("Enter disease/reason for visit: ");
    fgets(newPatient.disease, sizeof(newPatient.disease), stdin);
    newPatient.disease[strcspn(newPatient.disease, "\n")] = 0;
    
    printf("Enter admission date (DD/MM/YYYY): ");
    scanf("%s", newPatient.admissionDate);
    clearInputBuffer();
    
    // Add to array
    patients[patientCount] = newPatient;
    patientCount++;
    
    // Save to file
    savePatients();
    
    printf("\n✅ Patient added successfully!\n");
    printf("Patient ID: %d, Name: %s\n", newPatient.id, newPatient.name);
}

void viewAllPatients() {
    displayHeader("PATIENT LIST");
    
    if(patientCount == 0) {
        printf("\n❌ No patients found in the database!\n");
        return;
    }
    
    printf("\nTotal Patients: %d\n\n", patientCount);
    
    printf("%-5s %-20s %-5s %-8s %-10s %-15s %-20s\n", 
           "ID", "Name", "Age", "Gender", "Blood", "Contact", "Disease");
    printf("────────────────────────────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < patientCount; i++) {
        printf("%-5d %-20s %-5d %-8c %-10s %-15s %-20s\n",
               patients[i].id,
               patients[i].name,
               patients[i].age,
               patients[i].gender,
               patients[i].bloodGroup,
               patients[i].contact,
               patients[i].disease);
    }
}

void searchPatient() {
    int choice;
    
    displayHeader("SEARCH PATIENT");
    printf("1. Search by ID\n");
    printf("2. Search by Name\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    
    if(choice == 1) {
        int id;
        printf("\nEnter Patient ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        
        int index = searchPatientByID(id);
        
        if(index == -1) {
            printf("\n*** Patient with ID %d not found!\n", id);
            return;
        }
        
        // Display patient details
        printf("\n----------------------------------------\n");
        printf("|       PATIENT DETAILS                |\n");
        printf("----------------------------------------\n\n");
        printf("Patient ID:       %d\n", patients[index].id);
        printf("Name:             %s\n", patients[index].name);
        printf("Age:              %d years\n", patients[index].age);
        printf("Gender:           %c\n", patients[index].gender);
        printf("Blood Group:      %s\n", patients[index].bloodGroup);
        printf("Contact:          %s\n", patients[index].contact);
        printf("Address:          %s\n", patients[index].address);
        printf("Disease:          %s\n", patients[index].disease);
        printf("Admission Date:   %s\n", patients[index].admissionDate);
        
    } else if(choice == 2) {
        char name[50];
        printf("\nEnter patient name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        
        int index = searchPatientByName(name);
        
        if(index == -1) {
            printf("\n*** Patient '%s' not found!\n", name);
            return;
        }
        
        printf("\n*** Patient found!\n");
        printf("ID: %d, Name: %s, Age: %d, Disease: %s\n", 
               patients[index].id, patients[index].name, 
               patients[index].age, patients[index].disease);
    } else {
        printf("\n Invalid choice!\n");
    }
}

void updatePatient() {
    int id;
    
    displayHeader("UPDATE PATIENT");
    
    printf("Enter Patient ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = searchPatientByID(id);
    
    if(index == -1) {
        printf("\n*** Patient not found!\n");
        return;
    }
    
    printf("\nCurrent patient: %s\n", patients[index].name);
    printf("\nWhat do you want to update?\n");
    printf("1. Name\n");
    printf("2. Age\n");
    printf("3. Contact\n");
    printf("4. Address\n");
    printf("5. Disease\n");
    printf("6. All Details\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();
    
    switch(choice) {
        case 1:
            printf("Enter new name: ");
            fgets(patients[index].name, sizeof(patients[index].name), stdin);
            patients[index].name[strcspn(patients[index].name, "\n")] = 0;
            break;
        case 2:
            patients[index].age = getValidAge();
            break;
        case 3:
            getValidPhone(patients[index].contact);
            break;
        case 4:
            printf("Enter new address: ");
            fgets(patients[index].address, sizeof(patients[index].address), stdin);
            patients[index].address[strcspn(patients[index].address, "\n")] = 0;
            break;
        case 5:
            printf("Enter new disease: ");
            fgets(patients[index].disease, sizeof(patients[index].disease), stdin);
            patients[index].disease[strcspn(patients[index].disease, "\n")] = 0;
            break;
        case 6:
            printf("Enter new name: ");
            fgets(patients[index].name, sizeof(patients[index].name), stdin);
            patients[index].name[strcspn(patients[index].name, "\n")] = 0;
            
            patients[index].age = getValidAge();
            getValidGender(&patients[index].gender);
            getValidBloodGroup(patients[index].bloodGroup);
            getValidPhone(patients[index].contact);
            
            printf("Enter new address: ");
            fgets(patients[index].address, sizeof(patients[index].address), stdin);
            patients[index].address[strcspn(patients[index].address, "\n")] = 0;
            
            printf("Enter new disease: ");
            fgets(patients[index].disease, sizeof(patients[index].disease), stdin);
            patients[index].disease[strcspn(patients[index].disease, "\n")] = 0;
            break;
        default:
            printf("\n*** Invalid choice!\n");
            return;
    }
    
    savePatients();
    printf("\n*** Patient updated successfully!\n");
}

void deletePatient() {
    int id;
    
    displayHeader("DELETE PATIENT");
    
    printf("Enter Patient ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = searchPatientByID(id);
    
    if(index == -1) {
        printf("\n*** Patient not found!\n");
        return;
    }
    
    printf("\n*** WARNING: Are you sure you want to delete patient '%s'? (Y/N): ", 
           patients[index].name);
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if(confirm == 'Y' || confirm == 'y') {
        // Shift all elements after index to the left
        for(int i = index; i < patientCount - 1; i++) {
            patients[i] = patients[i + 1];
        }
        
        patientCount--;
        savePatients();
        
        printf("\n*** Patient deleted successfully!\n");
    } else {
        printf("\n*** Delete operation cancelled.\n");
    }
}
