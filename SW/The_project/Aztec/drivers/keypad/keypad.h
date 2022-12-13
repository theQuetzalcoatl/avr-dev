#ifndef KEYPAD_H
#define KEYPAD_H

#include "../../kernel/kernel.h"

enum KEYPAD_NUMS
{
    KEYPAD_0,        
    KEYPAD_1,        
    KEYPAD_2,        
    KEYPAD_3,        
    KEYPAD_4,        
    KEYPAD_5,        
    KEYPAD_6,        
    KEYPAD_7,        
    KEYPAD_8,        
    KEYPAD_9,        
    KEYPAD_ASTERISK, 
    KEYPAD_POUND,    
    KEYPAD_NO_NUM   
};


extern void keypad_init_device(void);
extern char keypad_get_pressed_key(void);
extern char keypad_get_pressed_key_nonblocking(void);

#endif /* KEYPAD_H */

/*
 * keypad.h
 * This file is part of Aztec
 *
 * Copyright (C) 2021 - theQuetzalcoatl
 *
 * Aztec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Aztec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aztec. If not, see <http://www.gnu.org/licenses/>.
 */