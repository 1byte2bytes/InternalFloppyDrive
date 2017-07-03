//
//  InternalFloppyDrive.c
//  InternalFloppyDrive
//
//  Created by Sydney Erickson on 7/3/17.
//  Copyright © 2017 Sydney Erickson. All rights reserved.
//

#include <mach/mach_types.h>

kern_return_t InternalFloppyDrive_start(kmod_info_t * ki, void *d);
kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d);

kern_return_t InternalFloppyDrive_start(kmod_info_t * ki, void *d)
{
    return KERN_SUCCESS;
}

kern_return_t InternalFloppyDrive_stop(kmod_info_t *ki, void *d)
{
    return KERN_SUCCESS;
}
