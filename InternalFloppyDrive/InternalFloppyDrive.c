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
#include "IO.h"

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
bool FloppyMotorOn = false;
bool GotIRQInterrupt = false;

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
        
        FDC_reset(a);
    }
    
    return KERN_SUCCESS;
}

kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
