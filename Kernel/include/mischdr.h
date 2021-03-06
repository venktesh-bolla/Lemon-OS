#pragma once

#include <stdint.h>
#include <acpispec/tables.h>

#define MULTIBOOT2_TAG_ALIGN 8

typedef struct {
	uint32_t flags;
	uint32_t memoryLo;
	uint32_t memoryHi;
	uint32_t bootDevice;
	uint32_t cmdline;
	uint32_t modsCount;
	uint32_t modsAddr;
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
	uint32_t mmapLength;
	uint32_t mmapAddr;
	uint32_t drivesLength;
	uint32_t drivesAddr;
	uint32_t configTable;
	uint32_t bootloaderName;
	uint32_t apmTable;

	uint32_t vbeControlInfo;
	uint32_t vbeModeInfo;
	uint16_t vbeMode;
	uint16_t vbeInterfaceSeg;
	uint16_t vbeInterfaceOff;
	uint16_t vbeInterfaceLen;

	uint64_t framebufferAddr;
	uint32_t framebufferPitch;
	uint32_t framebufferWidth;
	uint32_t framebufferHeight;
	uint8_t framebufferBpp;
	uint8_t framebufferType;
}__attribute__ ((packed)) multiboot1_info_t;

typedef struct
{
	uint32_t mod_start;
	uint32_t mod_end;
	uint32_t string;
	uint32_t reserved;
}__attribute__((packed)) multiboot1_module_t;

typedef struct
{
	uint32_t size;
	uint64_t base;
	uint64_t length;
	uint32_t type;
} __attribute__((packed)) multiboot1_memory_map_t;

enum {
	Mboot2CmdLine = 1,
	Mboot2BootloaderName = 2,
	Mboot2Modules = 3,
	Mboot2MemoryInfo = 4,
	Mboot2BIOSBootDevice = 5,
	Mboot2MemoryMap = 6,
	Mboot2VBEInfo = 7,
	Mboot2FramebufferInfo = 8,
	Mboot2ELFSymbols = 9,
	Mboot2APMTable = 10,
	Mboot2EFISystemTable32 = 11,
	Mboot2EFISystemTable64 = 12,
	Mboot2SMBIOSTables = 13,
	Mboot2ACPI1RSDP = 14,
	Mboot2ACPI2RSDP = 15,
	Mboot2NetInfo = 16,
	Mboot2EFIMemoryMap = 17,
	Mboot2EFIBootServices = 18,
	Mboot2EFIImageHandle32 = 19,
	Mboot2EFIImageHandle64 = 20,
	Mboot2ImageLoadBase = 21,
};

typedef struct{
	uint32_t totalSize;
	uint32_t reserved;
	uint8_t tags[];
} __attribute__((packed)) multiboot2_info_header_t;

typedef struct mb2tag{
	uint32_t type;
	uint32_t size;
	uint8_t tag[];

	mb2tag* next(){
		return reinterpret_cast<mb2tag*>(((uintptr_t)this) + ((size + MULTIBOOT2_TAG_ALIGN - 1) & ~(MULTIBOOT2_TAG_ALIGN - 1))); // Get next tag making sure to round up to alignment
	}
} __attribute__((packed)) multiboot2_tag_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2Modules;
		uint32_t size;
		uint32_t moduleStart;
		uint32_t moduleEnd;
		char string[];
	};
} __attribute__((packed)) multiboot2_module_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2MemoryInfo;
		uint32_t size;

		uint32_t memoryLower;
		uint32_t memoryUpper;
	};
 } __attribute__((packed)) multiboot2_memory_info_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2CmdLine;
		uint32_t size;
		char string[];
	};
} __attribute__((packed)) multiboot2_cmdline_t;

typedef struct {
	uint64_t base;
	uint64_t length;
	uint32_t type;
	uint32_t reserved;
} __attribute__((packed)) multiboot2_mmap_entry_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2MemoryMap;
		uint32_t size;

		uint32_t entrySize; // Size of each entry
		uint32_t entryVersion;
		
		multiboot2_mmap_entry_t entries[];
	};
} __attribute__((packed)) multiboot2_memory_map_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2ACPI1RSDP;
		uint32_t size;
		struct acpi_xsdp_t rsdp;
	};
} __attribute__((packed)) multiboot2_acpi1_rsdp_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2ACPI2RSDP;
		uint32_t size;
		struct acpi_xsdp_t rsdp;
	};
} __attribute__((packed)) multiboot2_acpi2_rsdp_t;

typedef union {
	multiboot2_tag_t tag;
	struct{
		uint32_t type = Mboot2FramebufferInfo;
		uint32_t size;

		uint64_t framebufferAddr;
		uint32_t framebufferPitch;
		uint32_t framebufferWidth;
		uint32_t framebufferHeight;
		uint8_t framebufferBpp;
		uint8_t framebufferType;
		uint8_t reserved;
	};

	// There is extra colour info here that we ignore for now
} __attribute__((packed)) multiboot2_framebuffer_info_t;

enum {
	VideoModeIndexed = 0,
	VideoModeRGB = 1,
	VideoModeText = 2,
};

typedef struct{
    uint32_t width; // Resolution width
    uint32_t height; // Resolution height
    uint16_t bpp; // Resolution depth/bits per pixel

    uint32_t pitch; // Video mode pitch

    void* address; // Video memory address
	uintptr_t physicalAddress;

	int type;
} video_mode_t;

typedef struct {
	uintptr_t memory_low;
	uintptr_t memory_high;

	multiboot1_memory_map_t* mem_map;
	uintptr_t memory_map_len;
} memory_info_t;

typedef struct {
	uintptr_t base;
	uintptr_t size;
} boot_module_t;