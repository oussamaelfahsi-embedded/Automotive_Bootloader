#ifndef INTERFACES_ACCESSING_VAR_H
#define INTERFACES_ACCESSING_VAR_H



/*
- Data Element : DownloadStartAddr
Read Interface  : unsigned int RTE_Read_DataElement_DownloadStartAddr();
Write Interface : void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);

*/
extern volatile unsigned int DownloadStartAddr; // store the start address to performing the download 
unsigned int RTE_Read_DataElement_DownloadStartAddr();
void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);

/*

- Data Element : DownloadMemorySize
Read Interface  : unsigned int RTE_Read_DataElement_DownloadStartAddr();
Write Interface : void   RTE_Write_DataElement_DownloadStartAddr(unsigned int newValue);
*/
extern volatile unsigned int DownloadMemorySize; 
unsigned int RTE_Read_DataElement_DownloadMemorySize();
void   RTE_Write_DataElement_DownloadMemorySize(unsigned int newValue);

/*

- Data Element : SecurityAccessState
Read Interface  : unsigned char RTE_Read_DataElement_SecurityAccessState();
Write Interface : void RTE_Write_DataElement_SecurityAccessState(unsigned char newValue );
*/
extern  volatile uint8_t SecurityAccessState;
unsigned char RTE_Read_DataElement_SecurityAccessState();
void RTE_Write_DataElement_SecurityAccessState(unsigned char newValue );

#endif