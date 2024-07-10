#ifndef _DESCRIPTOR_TABLE_H
#define _DESCRIPTOR_TABLE

/** 
 * Creates a descriptor table named `TABLE`, with entries
 * named `TABLE_entry`. The table is constructed to have 
 * `NUM_ENTRIES` entries. 
 */
#define DESCRIPTOR_TABLE(TABLE, NUM_ENTRIES) \
    struct TABLE##_entry TABLE [NUM_ENTRIES];\
    struct TABLE {\
        unsigned short size;\
        unsigned int address;\
    } __attribute__((packed));\
    struct TABLE TABLE##_pointer;
    
#endif
