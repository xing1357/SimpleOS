/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#ifndef PCI_H
#define PCI_H
#include "../../types.h"
#include "../../utils.h"
#include "../screen/screen.h"
#include "../ports/ports.h"

typedef union pci_dev {
    uint32 bits;
    struct {
        uint32 always_zero    : 2;
        uint32 field_num      : 6;
        uint32 function_num   : 3;
        uint32 device_num     : 5;
        uint32 bus_num        : 8;
        uint32 reserved       : 7;
        uint32 enable         : 1;
    };
} pci_dev_t;


#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC
#define PCI_VENDOR_ID            0x00
#define PCI_DEVICE_ID            0x02
#define PCI_COMMAND              0x04
#define PCI_STATUS               0x06
#define PCI_REVISION_ID          0x08
#define PCI_PROG_IF              0x09
#define PCI_SUBCLASS             0x0a
#define PCI_CLASS                0x0b
#define PCI_CACHE_LINE_SIZE      0x0c
#define PCI_LATENCY_TIMER        0x0d
#define PCI_HEADER_TYPE          0x0e
#define PCI_BIST                 0x0f
#define PCI_BAR0                 0x10
#define PCI_BAR1                 0x14
#define PCI_BAR2                 0x18
#define PCI_BAR3                 0x1C
#define PCI_BAR4                 0x20
#define PCI_BAR5                 0x24
#define PCI_INTERRUPT_LINE       0x3C
#define PCI_SECONDARY_BUS        0x09

#define PCI_HEADER_TYPE_DEVICE  0
#define PCI_HEADER_TYPE_BRIDGE  1
#define PCI_HEADER_TYPE_CARDBUS 2
#define PCI_TYPE_BRIDGE 0x0604
#define PCI_TYPE_SATA   0x0106
#define PCI_NONE 0xFFFF


#define DEVICE_PER_BUS           32
#define FUNCTION_PER_DEVICE      32

uint32 pci_read(pci_dev_t dev, uint32 field);
void pci_write(pci_dev_t dev, uint32 field, uint32 value);
uint32 get_device_type(pci_dev_t dev);
uint32 get_secondary_bus(pci_dev_t dev);
uint32 pci_reach_end(pci_dev_t dev);
pci_dev_t pci_scan_function(uint16 vendor_id, uint16 device_id, uint32 bus, uint32 device, uint32 function, int device_type);
pci_dev_t pci_scan_device(uint16 vendor_id, uint16 device_id, uint32 bus, uint32 device, int device_type);
pci_dev_t pci_scan_bus(uint16 vendor_id, uint16 device_id, uint32 bus, int device_type);
pci_dev_t pci_get_device(uint16 vendor_id, uint16 device_id, int device_type);
void pci_init();

#endif