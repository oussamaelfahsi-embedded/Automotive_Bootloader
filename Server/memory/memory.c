#include "memory.h"


void MEM_init(){
    // Definition of Application Section 
    Application_Section.start_addr = 0x12131415U;
    Application_Section.mem_size = 123U; // in KBytes 
    
    // Definition of Calibration Section 

}


// Check if an addr inside a specific section
// each section has a start address and a size 
uint8_t MEM_is_addr_Valid( uint32_t addr , Mem_str section  ){
    
    MEM_init();
    
    if( addr  >= section.start_addr && addr < ( addr + section.start_addr + (section.mem_size*1024)  ) ){
        return 0x00u;
    }
    return 0x01u
}