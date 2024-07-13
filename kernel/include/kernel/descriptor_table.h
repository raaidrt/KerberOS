#ifndef _DESCRIPTOR_TABLE_H
#define _DESCRIPTOR_TABLE

/**
 */
#define DESCRIPTOR_TABLE(TABLE, NUM_ENTRIES) \
    struct TABLE##_entry TABLE [NUM_ENTRIES];\
    __attribute__((aligned(0x10))) \
    struct TABLE {\
        unsigned short limit;\
        unsigned int base;\
    } __attribute__((packed));\
    struct TABLE TABLE##_pointer;
    
#endif
