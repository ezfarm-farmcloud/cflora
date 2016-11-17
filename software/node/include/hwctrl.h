
#ifndef __HW_CTRL__
#define __HW_CTRL__


#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include <stddef.h>
#include <linux/input.h>


#include "common.h"
 

int  KeyInput(void) ;
void Buzzer( char onoff ) ;
void BuzzerBeep( void ) ;

#endif
