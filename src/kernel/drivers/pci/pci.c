/*
Copyright 2021 Harvey Xing 
Licensed under MIT ( https://github.com/xing1357/SimpleOS/blob/main/LICENSE )
*/

#include "pci.h"

uint32 pci_size_map[60];
pci_dev_t dev_zero= {0};

uint32 pci_read(pci_dev_t dev, uint32 field) {
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outportl(PCI_CONFIG_ADDRESS, dev.bits);

	uint32 size = pci_size_map[field];
	if(size == 1) {
		uint8 t =inb(PCI_CONFIG_DATA + (field & 3));
		return t;
	}
	else if(size == 2) {
		uint16 t = inports(PCI_CONFIG_DATA + (field & 2));
		return t;
	}
	else if(size == 4){
		uint32 t = inportl(PCI_CONFIG_DATA);
		return t;
	}
	return 0xffff;
}

void pci_write(pci_dev_t dev, uint32 field, uint32 value) {
	dev.field_num = (field & 0xFC) >> 2;
	dev.enable = 1;
	outportl(PCI_CONFIG_ADDRESS, dev.bits);
	outportl(PCI_CONFIG_DATA, value);
}

uint32 get_device_type(pci_dev_t dev) {
	uint32 t = pci_read(dev, PCI_CLASS) << 8;
	return t | pci_read(dev, PCI_SUBCLASS);
}

uint32 get_secondary_bus(pci_dev_t dev) {
	return pci_read(dev, PCI_SECONDARY_BUS);
}

uint32 pci_reach_end(pci_dev_t dev) {
	uint32 t = pci_read(dev, PCI_HEADER_TYPE);
	return !t;
}


pci_dev_t pci_scan_function(uint16 vendor_id, uint16 device_id, uint32 bus, uint32 device, uint32 function, int device_type) {
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;
	dev.function_num = function;
	if(get_device_type(dev) == PCI_TYPE_BRIDGE) {
		pci_scan_bus(vendor_id, device_id, get_secondary_bus(dev), device_type);
	}

	if(device_type == -1 || device_type == get_device_type(dev)) {
		uint32 devid  = pci_read(dev, PCI_DEVICE_ID);
		uint32 vendid = pci_read(dev, PCI_VENDOR_ID);
		if(devid == device_id && vendor_id == vendid)
			return dev;
	}
	return dev_zero;
}

pci_dev_t pci_scan_device(uint16 vendor_id, uint16 device_id, uint32 bus, uint32 device, int device_type) {
	pci_dev_t dev = {0};
	dev.bus_num = bus;
	dev.device_num = device;

	if(pci_read(dev,PCI_VENDOR_ID) == PCI_NONE)
		return dev_zero;

	pci_dev_t t = pci_scan_function(vendor_id, device_id, bus, device, 0, device_type);
	if(t.bits)
		return t;

	if(pci_reach_end(dev))
		return dev_zero;

	for(int function = 1; function < FUNCTION_PER_DEVICE; function++) {
		if(pci_read(dev,PCI_VENDOR_ID) != PCI_NONE) {
			t = pci_scan_function(vendor_id, device_id, bus, device, function, device_type);
			if(t.bits)
				return t;
		}
	}
	return dev_zero;
}

pci_dev_t pci_scan_bus(uint16 vendor_id, uint16 device_id, uint32 bus, int device_type) {
	for(int device = 0; device < DEVICE_PER_BUS; device++) {
		pci_dev_t t = pci_scan_device(vendor_id, device_id, bus, device, device_type);
		if(t.bits)
			return t;
	}
	return dev_zero;
}

pci_dev_t pci_get_device(uint16 vendor_id, uint16 device_id, int device_type) {

	pci_dev_t t = pci_scan_bus(vendor_id, device_id, 0, device_type);
	if(t.bits)
		return t;

	if(pci_reach_end(dev_zero)) {
		print_string("PCI Get device failed...\n");
	}
	for(int function = 1; function < FUNCTION_PER_DEVICE; function++) {
		pci_dev_t dev = {0};
		dev.function_num = function;

		if(pci_read(dev, PCI_VENDOR_ID) == PCI_NONE)
			break;
		t = pci_scan_bus(vendor_id, device_id, function, device_type);
		if(t.bits)
			return t;
	}
	return dev_zero;
}

void pci_init() {
	pci_size_map[PCI_VENDOR_ID] =	2;
	pci_size_map[PCI_DEVICE_ID] =	2;
	pci_size_map[PCI_COMMAND]	=	2;
	pci_size_map[PCI_STATUS]	=	2;
	pci_size_map[PCI_SUBCLASS]	=	1;
	pci_size_map[PCI_CLASS]		=	1;
	pci_size_map[PCI_CACHE_LINE_SIZE]	= 1;
	pci_size_map[PCI_LATENCY_TIMER]		= 1;
	pci_size_map[PCI_HEADER_TYPE] = 1;
	pci_size_map[PCI_BIST] = 1;
	pci_size_map[PCI_BAR0] = 4;
	pci_size_map[PCI_BAR1] = 4;
	pci_size_map[PCI_BAR2] = 4;
	pci_size_map[PCI_BAR3] = 4;
	pci_size_map[PCI_BAR4] = 4;
	pci_size_map[PCI_BAR5] = 4;
	pci_size_map[PCI_INTERRUPT_LINE]	= 1;
	pci_size_map[PCI_SECONDARY_BUS]		= 1;
}