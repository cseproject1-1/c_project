/* 
 * Hospital Management System
 * File: appointment.c
 * Description: Appointment management module
 */

#include "hospital.h"

void appointmentMenu() {
    int choice;
    
    do {
        displayHeader("APPOINTMENT MANAGEMENT");
        printf("1. Book Appointment\n");
        printf("2. View All Appointments\n");
        printf("3. Cancel Appointment\n");
        printf("4. View Today's Appointments\n");
        printf("5. Back to Main Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();
        
        switch(choice) {
            case 1:
                bookAppointment();
                break;
            case 2:
                viewAllAppointments();
                break;
            case 3:
                cancelAppointment();
                break;
            case 4:
                viewTodayAppointments();
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

void bookAppointment() {
    if(appointmentCount >= MAX_APPOINTMENTS) {
        printf("\n*** Appointment database is full!\n");
        return;
    }
    
    if(patientCount == 0) {
        printf("\n*** No patients in database! Please add patients first.\n");
        return;
    }
    
    if(doctorCount == 0) {
        printf("\n*** No doctors in database! Please add doctors first.\n");
        return;
    }
    
    Appointment newAppt;
    
    displayHeader("BOOK APPOINTMENT");
    
    // Auto-generate appointment ID
    newAppt.appointmentId = appointmentCount + 1;
    printf("Appointment ID: %d (Auto-generated)\n\n", newAppt.appointmentId);
    
    // Get patient ID
    printf("Enter Patient ID: ");
    scanf("%d", &newAppt.patientId);
    clearInputBuffer();
    
    // Verify patient exists
    int patientIndex = searchPatientByID(newAppt.patientId);
    if(patientIndex == -1) {
        printf("\n*** Patient not found!\n");
        return;
    }
    printf("Patient: %s\n\n", patients[patientIndex].name);
    
    // Display available doctors
    printf("Available Doctors:\n");
    printf("%-5s %-20s %-20s\n", "ID", "Name", "Specialization");
    printf("──────────────────────────────────────────────\n");
    for(int i = 0; i < doctorCount; i++) {
        printf("%-5d %-20s %-20s\n", doctors[i].id, doctors[i].name, 
               doctors[i].specialization);
    }
    
    printf("\nEnter Doctor ID: ");
    scanf("%d", &newAppt.doctorId);
    clearInputBuffer();
    
    // Verify doctor exists
    int doctorIndex = searchDoctorByID(newAppt.doctorId);
    if(doctorIndex == -1) {
        printf("\n*** Doctor not found!\n");
        return;
    }
    printf("Doctor: Dr. %s (%s)\n", doctors[doctorIndex].name, 
           doctors[doctorIndex].specialization);
    
    // Get appointment date and time
    printf("\nEnter appointment date (DD/MM/YYYY): ");
    scanf("%s", newAppt.date);
    clearInputBuffer();
    
    printf("Enter appointment time (HH:MM): ");
    scanf("%s", newAppt.time);
    clearInputBuffer();
    
    strcpy(newAppt.status, "Scheduled");
    
    // Add to array
    appointments[appointmentCount] = newAppt;
    appointmentCount++;
    
    // Save to file
    saveAppointments();
    
    printf("\n Appointment booked successfully!\n");
    printf("Appointment ID: %d\n", newAppt.appointmentId);
    printf("Patient: %s\n", patients[patientIndex].name);
    printf("Doctor: Dr. %s\n", doctors[doctorIndex].name);
    printf("Date & Time: %s at %s\n", newAppt.date, newAppt.time);
}

void viewAllAppointments() {
    displayHeader("APPOINTMENT LIST");
    
    if(appointmentCount == 0) {
        printf("\n No appointments found!\n");
        return;
    }
    
    printf("\nTotal Appointments: %d\n\n", appointmentCount);
    
    printf("%-6s %-10s %-20s %-20s %-12s %-8s %-15s\n", 
           "Appt ID", "Pat ID", "Patient", "Doctor", "Date", "Time", "Status");
    printf("───────────────────────────────────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < appointmentCount; i++) {
        int pIndex = searchPatientByID(appointments[i].patientId);
        int dIndex = searchDoctorByID(appointments[i].doctorId);
        
        printf("%-6d %-10d %-20s %-20s %-12s %-8s %-15s\n",
               appointments[i].appointmentId,
               appointments[i].patientId,
               (pIndex != -1) ? patients[pIndex].name : "Unknown",
               (dIndex != -1) ? doctors[dIndex].name : "Unknown",
               appointments[i].date,
               appointments[i].time,
               appointments[i].status);
    }
}

void cancelAppointment() {
    int id;
    
    displayHeader("CANCEL APPOINTMENT");
    
    printf("Enter Appointment ID to cancel: ");
    scanf("%d", &id);
    clearInputBuffer();
    
    int index = searchAppointmentByID(id);
    
    if(index == -1) {
        printf("\n*** Appointment not found!\n");
        return;
    }
    
    if(strcmp(appointments[index].status, "Cancelled") == 0) {
        printf("\n*** Appointment is already cancelled!\n");
        return;
    }
    
    printf("\n*** WARNING: Are you sure you want to cancel this appointment? (Y/N): ");
    char confirm;
    scanf(" %c", &confirm);
    clearInputBuffer();
    
    if(confirm == 'Y' || confirm == 'y') {
        strcpy(appointments[index].status, "Cancelled");
        saveAppointments();
        printf("\n*** Appointment cancelled successfully!\n");
    } else {
        printf("\n*** Cancellation aborted.\n");
    }
}

void viewTodayAppointments() {
    char today[15];
    
    displayHeader("TODAY'S APPOINTMENTS");
    
    printf("Enter today's date (DD/MM/YYYY): ");
    scanf("%s", today);
    clearInputBuffer();
    
    int found = 0;
    
    printf("\nAppointments for %s:\n\n", today);
    printf("%-6s %-20s %-20s %-8s %-15s\n", 
           "Appt ID", "Patient", "Doctor", "Time", "Status");
    printf("─────────────────────────────────────────────────────────────────────────\n");
    
    for(int i = 0; i < appointmentCount; i++) {
        if(strcmp(appointments[i].date, today) == 0) {
            int pIndex = searchPatientByID(appointments[i].patientId);
            int dIndex = searchDoctorByID(appointments[i].doctorId);
            
            printf("%-6d %-20s %-20s %-8s %-15s\n",
                   appointments[i].appointmentId,
                   (pIndex != -1) ? patients[pIndex].name : "Unknown",
                   (dIndex != -1) ? doctors[dIndex].name : "Unknown",
                   appointments[i].time,
                   appointments[i].status);
            found = 1;
        }
    }
    
    if(!found) {
        printf("\n No appointments scheduled for %s\n", today);
    }
}
