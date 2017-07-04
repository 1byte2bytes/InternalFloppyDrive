//
//  Sleep_Bridge.hpp
//  InternalFloppyDrive
//
//  Created by Sydney Erickson on 7/4/17.
//  Copyright © 2017 Sydney Erickson. All rights reserved.
//

#ifndef Sleep_Bridge_hpp
#define Sleep_Bridge_hpp

#include <IOKit/IOLib.h>

void sleep(int time) {
    IOSleep(time);
}

#endif /* Sleep_Bridge_hpp */
