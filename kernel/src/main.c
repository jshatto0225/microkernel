#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include <limine.h>

#include "font8x8.h"

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_rsdp_request rsdp_request = {
    .id = LIMINE_RSDP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(4);
__attribute__((used, section(".limine_requests_start")))
static volatile u64 limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end")))
static volatile u64 limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void *memcpy(void *restrict dest, const void *restrict src, size_t n) {
    u8 *restrict pdest = (uint8_t *restrict)dest;
    const u8 *restrict psrc = (const uint8_t *restrict)src;

    for (size_t i = 0; i < n; i++)
        pdest[i] = psrc[i];

    return dest;
}

void *memset(void *s, int c, size_t n) {
    u8 *p = (u8 *)s;

    for (size_t i = 0; i < n; i++)
        p[i] = (u8)c;

    return s;
}

void *memmove(void *dest, const void *src, size_t n) {
    u8 *pdest = (u8 *)dest;
    const u8 *psrc = (const u8 *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++)
            pdest[i] = psrc[i];
    }
    else if (src < dest) {
        for (size_t i = n; i > 0; i--)
            pdest[i-1] = psrc[i-1];
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    const u8 *p1 = (const u8 *)s1;
    const u8 *p2 = (const u8 *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i])
            return p1[i] < p2[i] ? -1 : 1;
    }

    return 0;
}

static void __attribute((noreturn)) hcf(void) {
    for (;;)
        asm volatile ("hlt");
}

#define MAX_MEMMAP_REGIONS 64

#define MAX_ORDER 10

#define PAGE_SIZE 4096

#define PTL4_ENTRY_COUNT 512
#define PTL3_ENTRY_COUNT 512
#define PTL2_ENTRY_COUNT 512
#define PTL1_ENTRY_COUNT 512

#define PAGE_P (1ULL << 0)
#define PAGE_RW (1ULL << 1)
#define PAGE_PS (1ULL << 7)

#define MAX_IOAPICS 4
#define MAX_LAPICS 1

#define LAPIC_SPURIOUS_INTERRUPT_VECTOR (0x00F0 / 4)
#define LAPIC_ENABLE 0x00000100
#define LAPIC_TIMER_DIVIDE_CONFIGURATION (0x03E0 / 4)
#define LAPIC_TIMER_DIVIDE_CONFIGURATION_X1 0x0000000B
#define LAPIC_TIMER (0x0320 / 4)
#define LAPIC_PERIODIC 0x00020000
#define LAPIC_TIMER_INITIAL_COUNT (0x0380 / 4)
#define LAPIC_LINE_INTERRUPT_0 (0x0350 / 4)
#define LAPIC_LINE_INTERRUPT_1 (0x0360 / 4)
#define LAPIC_MASKED 0x00010000
#define LAPIC_VERSION (0x0030 / 4)
#define LAPIC_PERFORMANCE_COUNTER (0x0340 / 4)
#define LAPIC_ERROR (0x0370 / 4)
#define LAPIC_ERROR_STATUS (0x0280 / 4)
#define LAPIC_END_OF_INTERRUPT (0x00B0 / 4)
#define LAPIC_TASK_PRIORITY (0x0080 / 4)
#define LAPIC_ID (0x0020 / 4)

#define TRAP_DIVIDE 0
#define TRAP_DEBUG 1
#define TRAP_NON_MASKABLE_INTERRUPT 2
#define TRAP_BREAKPOINT 3
#define TRAP_OVERFLOW 4
#define TRAP_BOUNDS_CHECK 5
#define TRAP_ILLEGAL_OPCODE 6
#define TRAP_DEVICE_NOT_AVAILABLE 7
#define TRAP_DOUBLE_FAULT 8
#define TRAP_TSS 10
#define TRAP_SEGMENT_NOT_PRESENT 11
#define TRAP_STACK 12
#define TRAP_GENERAL_PROTECTION_FAULT 13
#define TRAP_PAGE_FAULT 14
#define TRAP_FLOATING_POINT_ERROR 16
#define TRAP_ALIGNMENT_CHECK 17
#define TRAP_MACHINE_CHECK 18
#define TRAP_SIMD_FLOATING_POINT_ERROR 19
#define TRAP_SYSCALL 64
#define TRAP_DEFAULT 500
#define TRAP_IRQ0 32

#define IRQ_TIMER 0
#define IRQ_KBD 1
#define IRQ_COM1 4
#define IRQ_IDE 14
#define IRQ_ERROR 19
#define IRQ_SPURIOUS 31

#define IOAPIC_REG_ID 0x00
#define IOAPIC_REG_VERSION 0x01
#define IOAPIC_REG_TABLE 0x10
#define IOAPIC_INT_DISABLED 0x00010000
#define IOAPIC_IO_REG_SELECT 0
#define IOAPIC_IO_DATA 4


typedef struct {
    u64 entry;
} ptl1e_t;

typedef struct {
    u64 entry;
} ptl2e_t;

typedef struct {
    u64 entry;
} ptl3e_t;

typedef struct {
    u64 entry;
} ptl4e_t;

__attribute__((aligned(PAGE_SIZE)))
typedef struct {
    ptl1e_t table[PTL1_ENTRY_COUNT];
} ptl1_t;

__attribute__((aligned(PAGE_SIZE)))
typedef struct {
    ptl2e_t table[PTL2_ENTRY_COUNT];
} ptl2_t;

__attribute__((aligned(PAGE_SIZE)))
typedef struct {
    ptl3e_t table[PTL3_ENTRY_COUNT];
} ptl3_t;

__attribute__((aligned(PAGE_SIZE)))
typedef struct {
    ptl4e_t table[PTL4_ENTRY_COUNT];
} ptl4_t;

enum memmap_region_type {
    MEMMAP_REGION_USABLE = 0,
    MEMMAP_REGION_RESERVED = 1,
    MEMMAP_REGION_ACPI_RECLAIMABLE = 2,
    MEMMAP_REGION_ACPI_NVS = 3,
    MEMMAP_REGION_BAD_MEMORY = 4,
    MEMMAP_REGION_BOOTLOADER_RECLAIMABLE = 5,
    MEMMAP_REGION_EXECUTABLE_AND_MODULES = 6,
    MEMMAP_REGION_FRAMEBUFFER = 7,
    MEMMAP_REGION_ACPI_TABLES = 8
};

struct region {
    uintptr_t phys;
    size_t size;
    enum memmap_region_type type;
};

struct memmap {
    size_t region_count;
    struct region regions[MAX_MEMMAP_REGIONS];
};

struct free_list_node {
    struct free_list_node *next;
};

__attribute__((packed))
struct acpi_sdt_header {
    char signature[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char oemid[6];
    char oem_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
};

__attribute__((packed))
struct acpi_rsdp {
    char signature[8];
    u8 checksum;
    char oemid[6];
    u8 revision;
    u32 rsdt_addr;
    u32 length;
    u64 xsdt_addr;
    u8 extended_checksum;
    u8 reserved[3];
};

__attribute__((packed))
struct acpi_xsdt {
    struct acpi_sdt_header header;
    u64 entries[];
};

__attribute__((packed))
struct acpi_madt {
    struct acpi_sdt_header header;
    u32 lapic_addr;
    u32 flags;
    u8 entries[];
};

__attribute__((packed))
struct acpi_rsdt {
    struct acpi_sdt_header header;
    u32 entries[];
};

enum {
    MADT_LAPIC = 0,
    MADT_IOAPIC = 1,
    MADT_OVERRIDE = 2
};

__attribute__((packed))
struct madt_ioapic_entry {
    u8 type;
    u8 length;
    u8 ioapic_id;
    u8 reserved;
    u32 ioapic_addr;
    u32 gsi_base;
};

__attribute__((packed))
struct madt_lapic_entry {
    u8 type;
    u8 length;
    u8 cpu_id;
    u8 apic_id;
    u32 flags;
};

struct ioapic {
    u8 id;
    volatile u32 *addr;
    u32 gsi_base;
};

struct lapic {
    u8 cpu_id;
    u8 apic_id;
    u32 flags;
};

__attribute__((packed))
struct gdt_entry {
    u16 limit_low;
    u16 base_low;
    u8 base_middle;
    u8 access;
    u8 granularity;
    u8 base_high;
};

__attribute__((packed))
struct gdt_ptr {
    u16 limit;
    u64 base;
};

static struct gdt_entry gdt[3];

static volatile u32 *framebuffer;
static int framebuffer_width;
static int framebuffer_height;

static uintptr_t hhdm;

static struct memmap memmap;

static struct free_list_node *free_list[MAX_ORDER + 1];

static ptl4_t *kernel_ptl4;

extern char __kernel_offset[];
extern char __text_start[];
extern char __text_end[];
extern char __rodata_start[];
extern char __rodata_end[];
extern char __data_start[];
extern char __data_end[];
extern char __bss_start[];
extern char __bss_end[];

static volatile u32 *lapic;

static size_t ioapic_count;
static struct ioapic ioapics[MAX_IOAPICS];

static size_t lapic_count;
static struct lapic lapics[MAX_LAPICS];

static void outb(u16 port, u8 val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void draw_char(int x, int y, char c, u32 color) {
    for (int row = 0; row < 8; row++) {
        unsigned char bits = font8x8[(size_t)c][row];
        for (int col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col)))
                framebuffer[(y + row) * framebuffer_width + (x + col)] = color;
        }
    }
}

static void clear_framebuffer() {
    memset((void *)framebuffer, 0, framebuffer_width * framebuffer_height * sizeof(uint32_t));
}

static void putc(char c) {
    static int cursor_x = 0;
    static int cursor_y = 0;

    switch (c) {
        case '\n': {
            cursor_y++;
            cursor_x = 0;
            break;
        }
        default: {
            draw_char(cursor_x * 8, cursor_y * 8, c, 0xFFFFFFFF);
            cursor_x = cursor_x + 1;
            break;
        }
    }

    if (cursor_x >= framebuffer_width / FONT_CHAR_WIDTH) {
        cursor_x = 0;
        cursor_y = cursor_y + 1;
    }

    if (cursor_y >= framebuffer_height / FONT_CHAR_HEIGHT) {
        clear_framebuffer();
        cursor_y = 0;
        cursor_x = 0;
    }
}

static void puts(const char *s) {
    if (!s) {
        putc('('); putc('n'); putc('u'); putc('l'); putc('l'); putc(')');
        return;
    }
    while (*s)
        putc(*s++);
}

static void putuint(u64 num, unsigned base) {
    char buf[32];
    int i = 0;

    if (num == 0) {
        putc('0');
        return;
    }

    while (num > 0) {
        u8 digit = num % base;
        buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        num /= base;
    }

    while (i--)
        putc(buf[i]);
}

static void putint(s64 num) {
    if (num < 0) {
        putc('-');
        putuint((u64)(-num), 10);
    } else {
        putuint((u64)num, 10);
    }
}

static void puthex(u64 num) {
    putc('0');
    putc('x');
    putuint(num, 16);
}

static void validate_bootloader(void) {
    if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
        hcf();
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
        hcf();
    if (memmap_request.response == NULL || memmap_request.response->entry_count < 1 || memmap_request.response->entry_count > MAX_MEMMAP_REGIONS)
        hcf();
    if (hhdm_request.response == NULL)
        hcf();
}

static void load_framebuffer(void) {
    struct limine_framebuffer *limine_framebuffer = framebuffer_request.response->framebuffers[0];
    framebuffer = limine_framebuffer->address;
    framebuffer_width = limine_framebuffer->width;
    framebuffer_height = limine_framebuffer->height;
}

static void load_memmap(void) {
    struct limine_memmap_response *limine_memmap = memmap_request.response;
    memmap.region_count = limine_memmap->entry_count;

    for (size_t i = 0; i < limine_memmap->entry_count; i++) {
        memmap.regions[i].phys = limine_memmap->entries[i]->base;
        memmap.regions[i].size = limine_memmap->entries[i]->length;
        switch (limine_memmap->entries[i]->type) {
            case LIMINE_MEMMAP_USABLE:
                memmap.regions[i].type = MEMMAP_REGION_USABLE;
                break;
            case LIMINE_MEMMAP_RESERVED:
                memmap.regions[i].type = MEMMAP_REGION_RESERVED;
                break;
            case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                memmap.regions[i].type = MEMMAP_REGION_ACPI_RECLAIMABLE;
                break;
            case LIMINE_MEMMAP_ACPI_NVS:
                memmap.regions[i].type = MEMMAP_REGION_ACPI_NVS;
                break;
            case LIMINE_MEMMAP_BAD_MEMORY:
                memmap.regions[i].type = MEMMAP_REGION_BAD_MEMORY;
                break;
            case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                memmap.regions[i].type = MEMMAP_REGION_BOOTLOADER_RECLAIMABLE;
                break;
            case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
                memmap.regions[i].type = MEMMAP_REGION_EXECUTABLE_AND_MODULES;
                break;
            case LIMINE_MEMMAP_FRAMEBUFFER:
                memmap.regions[i].type = MEMMAP_REGION_FRAMEBUFFER;
                break;
            case LIMINE_MEMMAP_ACPI_TABLES:
                memmap.regions[i].type = MEMMAP_REGION_ACPI_TABLES;
                break;
            default: // This should cover all of limines possible types but in the case of an error we should stop
                puts("Invalid or unsupported region type in limine memmap\n");
                hcf();
        }
    }
}

static void load_hhdm(void) {
    hhdm = hhdm_request.response->offset;
}

static uintptr_t p2v(uintptr_t phys) {
    return phys + hhdm;
}

static uintptr_t v2p(uintptr_t virt) {
    return virt - hhdm;
}

static size_t page_round_up(uintptr_t addr) {
    return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

static size_t page_round_down(uintptr_t addr) {
    return addr & ~(PAGE_SIZE - 1);
}

static void free_pages_early(void *addr, size_t order) {
    if ((size_t)addr % PAGE_SIZE != 0) {
        puts("Attempted to free unaligned page\n");
        hcf();
    }

    struct free_list_node *n = (struct free_list_node *)addr;
    n->next = 0;

    struct free_list_node **pcurr = &free_list[order];
    struct free_list_node *curr = free_list[order];

    while (curr) {
        if (order < MAX_ORDER) {
            uintptr_t buddy = (uintptr_t)n ^ (PAGE_SIZE << order);
            if ((uintptr_t)curr == buddy) {
                *pcurr = curr->next;

                void *block;
                if (n < curr)
                    block = n;
                else
                    block = curr;

                free_pages_early(block, order + 1);
                return;
            }
        }

        pcurr = &curr->next;
        curr = curr->next;
    }

    n->next = *pcurr;
    *pcurr = n;
}

static void free_range(void *base, void *end) {
    size_t p = page_round_up((uintptr_t)base);
    size_t e = page_round_down((uintptr_t)end);

    while (p < e) {
        size_t remaining = e - p;
        size_t order = MAX_ORDER;

        while (order > 0 && ((size_t)PAGE_SIZE << order > remaining || (p & (((size_t)PAGE_SIZE << order) - 1)) != 0))
            order--;

        free_pages_early((void *)p, order);
        p += PAGE_SIZE << order;
    }
}

static void free_usable_regions(void) {
    for (size_t i = 0; i < memmap.region_count; i++) {
        if (memmap.regions[i].type != MEMMAP_REGION_USABLE)
            continue;

        char *base = (char *)p2v(memmap.regions[i].phys);
        char *end = base + memmap.regions[i].size;

        free_range(base, end);
    }
}

static void *kalloc_early(size_t order) {
    if (order > MAX_ORDER) {
        puts("Invalid order for kalloc_early\n");
        hcf();
    }

    struct free_list_node **pcurr = &free_list[order];
    struct free_list_node *curr = free_list[order];

    if (curr) {
        *pcurr = curr->next;
        memset(curr, 0, PAGE_SIZE << order);

        return (void *)curr;
    }

    for (size_t higher = order + 1; higher <= MAX_ORDER; higher++) {
        struct free_list_node **phigh = &free_list[higher];
        struct free_list_node *high_curr = free_list[higher];

        if (high_curr) {
            *phigh = high_curr->next;

            while (higher > order) {
                higher--;

                struct free_list_node *buddy = (struct free_list_node *)((uintptr_t)high_curr + (PAGE_SIZE << higher));

                buddy->next = free_list[higher];
                free_list[higher] = buddy;
            }

            memset(high_curr, 0, PAGE_SIZE << order);

            return (void *)high_curr;
        }
    }

    puts("Could not find suitable block for kalloc_early\n");
    hcf();
}

static ptl3_t *walk_ptl4(ptl4_t *ptl4, u64 index, int create) {
    if (!(ptl4->table[index].entry & PAGE_P)) {
        if (!create)
            return 0;

        void *new_page = kalloc_early(0);
        memset(new_page, 0, PAGE_SIZE);
        ptl4->table[index].entry = v2p((uintptr_t)new_page) | PAGE_P | PAGE_RW;
    }

    return (ptl3_t *)p2v(ptl4->table[index].entry & ~0xFFF);
}

static ptl2_t *walk_ptl3(ptl3_t *ptl3, u64 index, int create) {
    if (!(ptl3->table[index].entry & PAGE_P)) {
        if (!create)
            return 0;

        void *new_page = kalloc_early(0);
        memset(new_page, 0, PAGE_SIZE);
        ptl3->table[index].entry = v2p((uintptr_t)new_page) | PAGE_P | PAGE_RW;
    }

    return (ptl2_t *)p2v(ptl3->table[index].entry & ~0xFFF);
}

static ptl1_t *walk_ptl2(ptl2_t *ptl2, uintptr_t index, int create) {
    if (!(ptl2->table[index].entry & PAGE_P)) {
        if (!create)
            return 0;

        void *new_page = kalloc_early(0);
        memset(new_page, 0, PAGE_SIZE);
        ptl2->table[index].entry = v2p((uintptr_t)new_page) | PAGE_P | PAGE_RW;
    }

    return (ptl1_t *)p2v(ptl2->table[index].entry & ~0xFFF);
}

static void map_page(ptl4_t *l4, uintptr_t pa, uintptr_t va, uintptr_t flags) {
    uintptr_t l4_index = (va >> 39) & 0x1FF;
    uintptr_t l3_index = (va >> 30) & 0x1FF;
    uintptr_t l2_index = (va >> 21) & 0x1FF;
    uintptr_t l1_index = (va >> 12) & 0x1FF;

    ptl3_t *l3 = walk_ptl4(l4, l4_index, 1);
    ptl2_t *l2 = walk_ptl3(l3, l3_index, 1);
    ptl1_t *l1 = walk_ptl2(l2, l2_index, 1);

    l1->table[l1_index].entry = (pa & ~0xFFF) | (flags & 0xFFF);
}

static void switch_ptl4(ptl4_t *l4) {
    asm volatile ("mov %0, %%cr3" : :  "r"(v2p((uintptr_t)l4)) : "memory");
}

static void init_paging(void) {
    kernel_ptl4 = kalloc_early(0);
    memset(kernel_ptl4, 0, PAGE_SIZE);

    for (size_t i = 0; i < memmap.region_count; i++) {
        switch (memmap.regions[i].type) {
            case MEMMAP_REGION_USABLE:
            case MEMMAP_REGION_RESERVED:
            case MEMMAP_REGION_ACPI_RECLAIMABLE:
            case MEMMAP_REGION_ACPI_NVS:
            case MEMMAP_REGION_BAD_MEMORY:
            case MEMMAP_REGION_BOOTLOADER_RECLAIMABLE:
            case MEMMAP_REGION_FRAMEBUFFER:
            case MEMMAP_REGION_ACPI_TABLES:
                puts("mapping region: ");
                puthex(memmap.regions[i].phys);
                puts(" -> ");
                puthex(p2v(memmap.regions[i].phys));
                puts(", size: ");
                puthex(memmap.regions[i].size);
                puts("\n");
                for (uintptr_t j = 0; j < memmap.regions[i].size; j += PAGE_SIZE) {
                    uintptr_t pa = j + memmap.regions[i].phys;
                    uintptr_t va = p2v(pa);
                    map_page(kernel_ptl4, pa, va, PAGE_P | PAGE_RW);
                }
                break;
            case MEMMAP_REGION_EXECUTABLE_AND_MODULES: {
                for (uintptr_t j = 0; j < memmap.regions[i].size; j += PAGE_SIZE) {
                    uintptr_t pa = j + memmap.regions[i].phys;
                    uintptr_t va = j + (uintptr_t)__kernel_offset;
                    uintptr_t flags = PAGE_P;
                    if (va >= (uintptr_t)__data_start)
                        flags |= PAGE_RW;
                    map_page(kernel_ptl4, pa, va, flags);
                }
                break;
            }
        }
    }

    switch_ptl4(kernel_ptl4);
}

static void madt_parse(struct acpi_madt *madt) {
    lapic = (volatile u32 *)p2v(madt->lapic_addr);
    
    u8 *ptr = madt->entries;
    u8 *end = (u8 *)madt + madt->header.length;

    while (ptr + 2 <= end) {
        u8 type = ptr[0];
        u8 len = ptr[1];

        if (len < 2 || ptr + len > end)
            break;

        switch (type) {
            case MADT_IOAPIC:
                struct madt_ioapic_entry *ioe = (struct madt_ioapic_entry *)ptr;
                if (ioapic_count >= MAX_IOAPICS) {
                    puts("Too many ioapics\n");
                    hcf();
                }
                ioapics[ioapic_count].id = ioe->ioapic_id;
                ioapics[ioapic_count].addr = (volatile u32 *)p2v(ioe->ioapic_addr);
                ioapics[ioapic_count].gsi_base = ioe->gsi_base;
                ioapic_count++;
                break;
            case MADT_LAPIC:
                struct madt_lapic_entry *le = (struct madt_lapic_entry *)ptr;
                if (lapic_count >= MAX_LAPICS) {
                    puts("Too many lapics\n");
                    hcf();
                }
                lapics[lapic_count].cpu_id = le->cpu_id;
                lapics[lapic_count].apic_id = le->apic_id;
                lapics[lapic_count].flags = le->flags;
                lapic_count++;
            default:
                break;
        }
        ptr += len;
    }
}

static void xsdt_parse(struct acpi_xsdt *xsdt) {
    u64 entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;

    for (size_t i = 0; i < entries; i++) {
        struct acpi_sdt_header *hdr = (struct acpi_sdt_header *)p2v(xsdt->entries[i]);
        if (!hdr)
            continue;

        if (hdr->signature[0] == 'A' && hdr->signature[1] == 'P' && hdr->signature[2] == 'I' && hdr->signature[3] == 'C') {
            madt_parse((struct acpi_madt *)hdr);
            return;
        }
    }
    
    puts("APIC entry not found in xsdt\n");
    hcf();
}

static void rsdt_parse(struct acpi_rsdt *rsdt) {
    u64 entries = (rsdt->header.length - sizeof(rsdt->header)) / 8;

    for (size_t i = 0; i < entries; i++) {
        struct acpi_sdt_header *hdr = (struct acpi_sdt_header *)p2v(rsdt->entries[i]);
        if (!hdr)
            continue;

        if (hdr->signature[0] == 'A' && hdr->signature[1] == 'P' && hdr->signature[2] == 'I' && hdr->signature[3] == 'C') {
            madt_parse((struct acpi_madt *)hdr);
            return;
        }
    }

    puts("APIC entry not found in rsdt\n");
    hcf();
}

static void load_apic(void) {
    if (rsdp_request.response == NULL) {
        puts("No acpi present\n");
        return;
    }
    struct acpi_rsdp *rsdp = (struct acpi_rsdp *)rsdp_request.response->address;
    if (rsdp->revision >= 2)
        xsdt_parse((struct acpi_xsdt *)p2v(rsdp->xsdt_addr));
    else
        rsdt_parse((struct acpi_rsdt *)p2v(rsdp->rsdt_addr));
}

void gdt_set_entry(int num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt[num].base_low = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = limit & 0xFFFF;
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

static void init_gdt(void) {
    
}

static void lapicw(size_t index, int value) {
    lapic[index] = value;
    (void)lapic[LAPIC_ID];
}

static void init_lapic(void) {
    if (!lapic) {
        puts("No lapic found\n");
        hcf();
    }

    map_page(kernel_ptl4, v2p((uintptr_t)lapic), (uintptr_t)lapic, PAGE_P | PAGE_RW);
    
    lapicw(LAPIC_SPURIOUS_INTERRUPT_VECTOR, LAPIC_ENABLE | (TRAP_IRQ0 + IRQ_SPURIOUS));

    lapicw(LAPIC_TIMER_DIVIDE_CONFIGURATION, LAPIC_TIMER_DIVIDE_CONFIGURATION_X1);
    lapicw(LAPIC_TIMER, LAPIC_PERIODIC | (TRAP_IRQ0 + IRQ_TIMER));
    lapicw(LAPIC_TIMER_INITIAL_COUNT, 10000000);

    lapicw(LAPIC_LINE_INTERRUPT_0, LAPIC_MASKED);
    lapicw(LAPIC_LINE_INTERRUPT_1, LAPIC_MASKED);

    if (((lapic[LAPIC_VERSION] >> 16) & 0xFF) >= 4)
        lapicw(LAPIC_PERFORMANCE_COUNTER, LAPIC_MASKED);

    lapicw(LAPIC_ERROR, TRAP_IRQ0 + IRQ_ERROR);

    lapicw(LAPIC_ERROR_STATUS, 0);
    lapicw(LAPIC_ERROR_STATUS, 0);

    lapicw(LAPIC_END_OF_INTERRUPT, 0);

    lapicw(LAPIC_TASK_PRIORITY, 0);
}

static void init_pic(void) {
    // Disable
    outb(0x12, 0xFF);
    outb(0xA1, 0xFF);
}

static int ioapicread(u64 index, u32 reg) {
    volatile u32 *mmio = ioapics[index].addr;
    mmio[IOAPIC_IO_REG_SELECT] = reg;
    return mmio[IOAPIC_IO_DATA];
}

static void ioapicwrite(u64 index, u32 reg, u32 data) {
    volatile u32 *mmio = ioapics[index].addr;
    mmio[IOAPIC_IO_REG_SELECT] = reg;
    mmio[IOAPIC_IO_DATA] = data;
}

static void init_ioapic(void) {
    for (u64 i = 0; i < ioapic_count; i++) {
        map_page(kernel_ptl4, v2p((uintptr_t)ioapics[i].addr), (uintptr_t)ioapics[i].addr, PAGE_P | PAGE_RW);
        
        u32 id;
        u32 maxintr;

        maxintr = (ioapicread(i, IOAPIC_REG_VERSION) >> 16) & 0xFF;
        id = ioapicread(i, IOAPIC_REG_ID) >> 24;
        if (id != ioapics[i].id) {
            continue;
        }

        for (u64 j = 0; j <= maxintr; j++) {
            ioapicwrite(i, IOAPIC_REG_TABLE + 2 * j, IOAPIC_INT_DISABLED | (TRAP_IRQ0 + j));
            ioapicwrite(i, IOAPIC_REG_TABLE + 2 * j + 1, 0);
        }
    }
}

static void mp_main(void) {
//    init_idt();
//    init_sched();
}

__attribute__((noreturn))
static void ap_enter(void) {
    mp_main();

    hcf();
}

void kmain(void) {
    validate_bootloader();

    load_framebuffer();
    load_memmap();
    load_hhdm();
    load_apic();

    free_usable_regions();

//    init_mp();
    init_paging();
    init_lapic();
//    init_gdt();
    init_pic();
    init_ioapic();
//    init_tv();

    mp_main();

    hcf();
}
