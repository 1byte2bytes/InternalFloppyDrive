//
//  FDC.c
//  InternalFloppyDrive
//
//  Created by Sydney Erickson on 7/4/17.
//  Copyright © 2017 Sydney Erickson. All rights reserved.
//

#include "FDC.h"
#include "IO.h"

void FDC_turn_motor_on() {
    outb(DIGITAL_OUTPUT_REGISTER, 0x10);
}

void FDC_turn_motor_off() {
    outb(DIGITAL_OUTPUT_REGISTER, 0x10);
}

int FDC_reset(int DriveType) {
    // Reset the controller
    outb(DIGITAL_OUTPUT_REGISTER, 0x00);
    outb(DIGITAL_OUTPUT_REGISTER, 0x0C);
    printf("Floppy drive reset command sent\n");
    
    // TODO: Wait for IRQ interrupt
    printf("Floppy drive reset IRQ recieved\n");
    
    // Send 4 sense interrupts
    // TODO: proper send_byte procedure
    for ( int i = 4; i > 0; i-- ) {
        outb(DATA_FIFO, SENSE_INTERRUPT);
        inb(DATA_FIFO);
        inb(DATA_FIFO);
        printf("Sent sense_interrupt signal\n");
    }
    
    if (DriveType != 4) {
        printf("Floppy drive is of a non-compatible size at the moment.\n");
        return -1;
    } else {
        // Set speed to 500kb for 1.44MB floppies
        outb(CONFIGURATION_CONTROL_REGISTER, 0x00);
        printf("Set speed to 500kb - 1.44MB 3.5\"\n");
    }
    
    // Configure the FDC
    // TODO: actually send configure data
    outb(DATA_FIFO, CONFIGURE);
    printf("Sent configure signal\n");
    
    // Lock our settings
    outb(DATA_FIFO, LOCK);
    printf("Sent lock signal\n");
    return 0;
}
