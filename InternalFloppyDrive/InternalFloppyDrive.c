//
//  InternalFloppyDrive.c
//  InternalFloppyDrive
//
//  Created by Sydney Erickson on 7/3/17.
//  Copyright © 2017 Sydney Erickson. All rights reserved.
//

#include <sys/systm.h>
#include <mach/mach_types.h>

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
    char *drive_type[5] = { "no floppy drive", "360kb 5.25in floppy drive", "1.2mb 5.25in floppy drive", "720kb 3.5in", "1.44mb 3.5in", "2.88mb 3.5in"};
    printf("Floppy drive A is an:\n");
    printf(drive_type[a]);
    printf("\nFloppy drive B is an:\n");
    printf(drive_type[b]);
    printf("\n");
    
    return KERN_SUCCESS;
}

kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
