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
