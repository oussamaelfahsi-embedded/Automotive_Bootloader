#ifndef MEMORY_H
#define MEMORY_H

typedef struct {
    uint32_t start_addr ;
    uint32_t  mem_size ; // in KBytes 
}Mem_str;

Mem_str Application_Section ;
Mem_str Calibration_Section ;




#endif 