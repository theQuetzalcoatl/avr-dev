#include "stdint.h"
#include "kernel_logging.h"

typedef struct
{
    struct
    {
        char entry[CONFIG_K_LOG_ENTRY_LENGTH_BYTES];
    }entries[CONFIG_K_LOG_ENTRY_NUM];   
    uint8_t r_index;
    uint8_t w_index;
}klog_t;

static klog_t kernel_log = {0};
static k_error_t (*printing_func)(const char *); // Holds the printing function, specified by the user during logging init.


 /* pre increment - circular fifo
  * also pushes read index in front of itself
  */
void klog_log(const char *log_msg)
{
    ++kernel_log.w_index;
    if(kernel_log.w_index >= CONFIG_K_LOG_ENTRY_NUM) kernel_log.w_index = 0;
    
    /* pushing the read index in front of the write one if needed */
    if(kernel_log.w_index == kernel_log.r_index){
        ++kernel_log.r_index;
        if(kernel_log.r_index == CONFIG_K_LOG_ENTRY_NUM) kernel_log.r_index = 0; 
    }
    
    /* actually logging */
    uint8_t i = 0;
    while(log_msg[i] && i < CONFIG_K_LOG_ENTRY_LENGTH_BYTES){
        kernel_log.entries[kernel_log.w_index].entry[i] = log_msg[i];
        ++i;
    }
}


/* post increment - circular fifo
 * alawys stops when reaches write index
 */
k_error_t klog_print(void)
{
    k_error_t ret = NO_ERROR;
    
    while(kernel_log.r_index != kernel_log.w_index){
        (void)printing_func(kernel_log.entries[kernel_log.r_index].entry);
    	++kernel_log.r_index;
        if(kernel_log.r_index >= CONFIG_K_LOG_ENTRY_NUM) kernel_log.r_index = 0;
        if(ret != NO_ERROR) break;
    }
    ret = printing_func(kernel_log.entries[kernel_log.r_index].entry);
    
    return ret;
}


void klog_init(k_error_t (*rec_func)(const char *))
{
    printing_func = rec_func;
}

/*
 * kernel_logging.h
 * This file is part of Aztec
 *
 * Copyright (C) 2022 - theQuetzalcoatl
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


