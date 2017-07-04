//
//  InternalFloppyDrive.c
//  InternalFloppyDrive
//
//  Created by Sydney Erickson on 7/3/17.
//  Copyright © 2017 Sydney Erickson. All rights reserved.
//

#include <sys/systm.h>
#include <mach/mach_types.h>

#include "FDC.h"

typedef int bool;
enum { false, true };

/*
 __          __     _____  _   _ _____ _   _  _____
 \ \        / /\   |  __ \| \ | |_   _| \ | |/ ____|
  \ \  /\  / /  \  | |__) |  \| | | | |  \| | |  __
   \ \/  \/ / /\ \ |  _  /| . ` | | | | . ` | | |_ |
    \  /\  / ____ \| | \ \| |\  |_| |_| |\  | |__| |
     \/  \/_/    \_\_|  \_\_| \_|_____|_| \_|\_____|
 
 SET THIS TO FALSE AT YOUR OWN RISK. DISABLING THIS CHECK MIGHT
 ALLOW THE DRIVER TO CONTROL UNSTABLE DRIVES, WHICH MAY RESULT
 IN AN UNSTABLE SYSTEM OR DESTROYING YOUR DRIVE.
*/

bool DoFDCCheck = false;
 
 

/* OSDev stuff */
static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
    /* There's an outb %al, $imm8  encoding, for compile-time constant port numbers that fit in 8b.  (N constraint).
     * Wider immediate constants would be truncated at assemble-time (e.g. "i" constraint).
     * The  outb  %al, %dx  encoding is the only option for all other cases.
     * %1 expands to %dx because  port  is a uint16_t.  %w1 could be used if we had the port number a wider C type */
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                  : "=a"(ret)
                  : "Nd"(port) );
    return ret;
}

kern_return_t InternalFloppyDrive_start(kmod_info_t * ki, void *d);
kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d);

kern_return_t InternalFloppyDrive_start(kmod_info_t * ki, void *d)
{
    // Detect floppy disks
    unsigned char c;
    outb(0x70, 0x10);
    c = inb(0x71);
    
    // Split byte for each floppy drive
    char a = c >> 4; // get the high nibble
    char b = c & 0xF; // get the low nibble by ANDing out the high nibble
    
    // Print floppy disk info
    char *drive_type[6] = { "no floppy drive", "360kb 5.25in floppy drive", "1.2mb 5.25in floppy drive", "720kb 3.5in", "1.44mb 3.5in", "2.88mb 3.5in"};
    printf("Floppy drive A is an:\n");
    printf("%s", drive_type[a]);
    printf("\nFloppy drive B is an:\n");
    printf("%s", drive_type[b]);
    printf("\n");
    
    if (a != 0) {
        // We have a main floppy drive!
        // Check the version of the FDC
        outb(DATA_FIFO, VERSION);
        int c;
        c = inb(MAIN_STATUS_REGISTER);
        if (c != 0x90) {
            printf("Incompatible floppy drive controller found. Exiting.\n");
            printf("Got controller: %d - wanted %d\n", c, 0x90);
            
            if (DoFDCCheck == false) {
                printf("Floppy Disk Controller check has been disabled. Procced at your own risk.\n");
            } else {
                return KERN_ABORTED;
            }
        } else {
            printf("Found an 82077AA FDC.\n");
        }
        // Reset the controller
        outb(DIGITAL_OUTPUT_REGISTER, 0x00);
        outb(DIGITAL_OUTPUT_REGISTER, 0x0C);
        printf("Floppy drive reset command sent");
        // TODO: Wait for IRQ interrupt
        printf("Floppy drive reset IRQ recieved");
        // Send 4 sense interrupts
        // TODO: proper send_byte procedure
        for ( int i = 4; i > 0; i-- ) {
            outb(DATA_FIFO, SENSE_INTERRUPT);
            inb(DATA_FIFO);
            inb(DATA_FIFO);
            printf("Sent sense_interrupt signal");
        }
        
        if (a != 5) {
            printf("Floppy drive is of a non-compatible size at the moment.\n");
            return KERN_ABORTED;
        } else {
            // Set speed to 500kb for 1.44MB floppies
            outb(CONFIGURATION_CONTROL_REGISTER, 0x00);
            printf("Set speed to 500kb - 1.44MB 3.5\"");
        }
        
        // Configure the FDC
        outb(DATA_FIFO, CONFIGURE);
        printf("Sent configure signal");
        // TODO: actually send configure data
        // Lock our settings
        outb(DATA_FIFO, LOCK);
        printf("Sent lock signal");
    }
    
    return KERN_SUCCESS;
}

kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
