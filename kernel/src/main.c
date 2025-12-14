#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#include <limine.h>

#include "font8x8.h"

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
  .revision = 0
};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_memmap_request memmap_request = {
  .id = LIMINE_MEMMAP_REQUEST_ID,
  .revision = 0
};

__attribute__((used, section(".limine_requests"))) static volatile struct limine_hhdm_request hhdm_request = {
  .id = LIMINE_HHDM_REQUEST_ID,
  .revision = 0
};

__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);
__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void *
memcpy(void *restrict dest, const void *restrict src, size_t n)
{
  uint8_t *restrict pdest = (uint8_t *restrict)dest;
  const uint8_t *restrict psrc = (const uint8_t *restrict)src;

  for (size_t i = 0; i < n; i++)
  {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *
memset(void *s, int c, size_t n)
{
  uint8_t *p = (uint8_t *)s;

  for (size_t i = 0; i < n; i++)
  {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *
memmove(void *dest, const void *src, size_t n)
{
  uint8_t *pdest = (uint8_t *)dest;
  const uint8_t *psrc = (const uint8_t *)src;

  if (src > dest)
  {
    for (size_t i = 0; i < n; i++)
    {
      pdest[i] = psrc[i];
    }
  }
  else if (src < dest)
  {
    for (size_t i = n; i > 0; i--)
    {
      pdest[i-1] = psrc[i-1];
    }
  }

  return dest;
}

int
memcmp(const void *s1, const void *s2, size_t n)
{
  const uint8_t *p1 = (const uint8_t *)s1;
  const uint8_t *p2 = (const uint8_t *)s2;

  for (size_t i = 0; i < n; i++)
  {
    if (p1[i] != p2[i])
    {
      return p1[i] < p2[i] ? -1 : 1;
    }
  }

  return 0;
}

static void __attribute((noreturn))
hcf(void)
{
  for (;;)
  {
    asm ("hlt");
  }
}

#define MAX_MEMMAP_REGIONS 64

#define MAX_ORDER 10

#define PAGE_SIZE 4096

#define PTL4_ENTRY_COUNT 512
#define PTL3_ENTRY_COUNT 512
#define PTL2_ENTRY_COUNT 512
#define PTL1_ENTRY_COUNT 512

#define PAGE_PRESENT (1ULL << 0)
#define PAGE_RW (1ULL << 1)
#define PAGE_PS (1ULL << 7)

typedef struct
{
  uint64_t entry;
} ptl1e_t;

typedef struct
{
  uint64_t entry;
} ptl2e_t;

typedef struct
{
  uint64_t entry;
} ptl3e_t;

typedef struct
{
  uint64_t entry;
} ptl4e_t;

typedef struct __attribute__((aligned(PAGE_SIZE)))
{
  ptl1e_t table[PTL1_ENTRY_COUNT];
} ptl1_t;

typedef struct __attribute__((aligned(PAGE_SIZE)))
{
  ptl2e_t table[PTL2_ENTRY_COUNT];
} ptl2_t;

typedef struct __attribute__((aligned(PAGE_SIZE)))
{
  ptl3e_t table[PTL3_ENTRY_COUNT];
} ptl3_t;

typedef struct __attribute__((aligned(PAGE_SIZE)))
{
  ptl4e_t table[PTL4_ENTRY_COUNT];
} ptl4_t;

enum memmap_region_type
{
  MEMMAP_REGION_USABLE,
  MEMMAP_REGION_RESERVED,
  MEMMAP_REGION_ACPI_RECLAIMABLE,
  MEMMAP_REGION_ACPI_NVS,
  MEMMAP_REGION_BAD_MEMORY,
  MEMMAP_REGION_BOOTLOADER_RECLAIMABLE,
  MEMMAP_REGION_EXECUTABLE_AND_MODULES,
  MEMMAP_REGION_FRAMEBUFFER,
  MEMMAP_REGION_ACPI_TABLES
};

struct region
{
  uintptr_t phys;
  size_t size;
enum memmap_region_type type;
};

struct memmap
{
  size_t region_count;
  struct region regions[MAX_MEMMAP_REGIONS];
};

struct free_list_node
{
  struct free_list_node *next;
};

static volatile uint32_t *framebuffer;
static int framebuffer_width;
static int framebuffer_height;

static uintptr_t hhdm;

static struct memmap memmap;

static struct free_list_node *free_list[MAX_ORDER + 1];

static ptl4_t *kernel_ptl4;

extern uintptr_t __kernel_start;
extern uintptr_t __kernel_end;
extern uintptr_t __kernel_base;

static void
draw_char(int x, int y, char c, uint32_t color)
{
  for (int row = 0; row < 8; row++)
  {
    unsigned char bits = font8x8[(size_t)c][row];
    for (int col = 0; col < 8; col++)
    {
      if (bits & (1 << (7 - col)))
      {
        framebuffer[(y + row) * framebuffer_width + (x + col)] = color;
      }
    }
  }
}

static void
clear_framebuffer()
{
  memset((void *)framebuffer, 0, framebuffer_width * framebuffer_height * sizeof(uint32_t));
}

static void
putc(char c)
{
  static int cursor_x = 0;
  static int cursor_y = 0;

  switch (c) {
    case '\n':
    {
      cursor_y++;
      cursor_x = 0;
      break;
    }
    default:
    {
      draw_char(cursor_x * 8, cursor_y * 8, c, 0xFFFFFFFF);
      cursor_x = cursor_x + 1;
      break;
    }
  }

  if (cursor_x >= framebuffer_width / FONT_CHAR_WIDTH)
  {
    cursor_x = 0;
    cursor_y = cursor_y + 1;
  }

  if (cursor_y >= framebuffer_height / FONT_CHAR_HEIGHT)
  {
    clear_framebuffer();
    cursor_y = 0;
    cursor_x = 0;
  }
}

static void
puts(const char *s)
{
  while (*s)
  {
    putc(*s++);
  }
}

static void
early_print(const char *fmt, ...)
{
  va_list args;
  va_start(args, fmt);

  for (const char *p = fmt; *p; p++)
  {
    if (*p == '%')
    {
      p++;
      int is_long = 0;

      if (*p == 'l')
      {
        is_long = 1;
        p++;
      }

      switch (*p)
      {
        case 's':
        {
          char *str = va_arg(args, char *);
          for (; *str; str++)
          {
            putc(*str);
          }
          break;
        }
        case 'd':
        {
          if (is_long)
          {
            long num = va_arg(args, long);
            if (num < 0)
            {
              putc('-');
              num = -num;
            }
            char buf[30];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              buf[i++] = '0' + (num % 10);
              num /= 10;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          else
          {
            int num = va_arg(args, int);
            if (num < 0)
            {
              putc('-');
              num = -num;
            }
            char buf[20];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              buf[i++] = '0' + (num % 10);
              num /= 10;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          break;
        }
        case 'u':
        {
          if (is_long)
          {
            unsigned long num = va_arg(args, unsigned long);
            char buf[30];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              buf[i++] = '0' + (num % 10);
              num /= 10;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          else
          {
            unsigned int num = va_arg(args, unsigned int);
            char buf[20];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              buf[i++] = '0' + (num % 10);
              num /= 10;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          break;
        }
        case 'x':
        {
          putc('0');
          putc('x');
          if (is_long)
          {
            unsigned long num = va_arg(args, unsigned long);
            char buf[16];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              uint8_t digit = num & 0xF;
              buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
              num >>= 4;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          else
          {
            unsigned int num = va_arg(args, unsigned int);
            char buf[8];
            int i = 0;
            if (num == 0)
            {
              buf[i++] = '0';
            }
            while (num > 0)
            {
              uint8_t digit = num & 0xF;
              buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
              num >>= 4;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          break;
        }
        case 'c':
        {
          char c = (char)va_arg(args, int);
          putc(c);
          break;
        }
        case 'p':
        {
          uint64_t ptr = (uint64_t)va_arg(args, void *);
          putc('0');
          putc('x');
          char buf[16];
          int i = 0;
          if (ptr == 0)
          {
            putc('0');
          }
          else
          {
            while (ptr > 0)
            {
              uint8_t digit = ptr & 0xF;
              buf[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
              ptr >>= 4;
            }
            while (i--)
            {
              putc(buf[i]);
            }
          }
          break;
        }
        case '%':
        {
          putc('%');
          break;
        }
        default:
        {
          putc('?');
          break;
        }
      }
    }
  }
}

static void
validate_bootloader(void)
{
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
  {
    hcf();
  }

  if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
  {
    hcf();
  }

  if (memmap_request.response == NULL)
  {
    hcf();
  }

  if (hhdm_request.response == NULL)
  {
    hcf();
  }
}

static void
load_framebuffer(void)
{
  struct limine_framebuffer *limine_framebuffer = framebuffer_request.response->framebuffers[0];
  framebuffer = limine_framebuffer->address;
  framebuffer_width = limine_framebuffer->width;
  framebuffer_height = limine_framebuffer->height;
}

static void
load_memmap(void)
{
  struct limine_memmap_response *limine_memmap = memmap_request.response;
  memmap.region_count = limine_memmap->entry_count;

  if (limine_memmap->entry_count > MAX_MEMMAP_REGIONS)
  {
    puts("Too many entries in limine memmap\n");
    hcf();
  }

  for (size_t i = 0; i < limine_memmap->entry_count; i++)
  {
    memmap.regions[i].phys = limine_memmap->entries[i]->base;
    memmap.regions[i].size = limine_memmap->entries[i]->length;
    switch (limine_memmap->entries[i]->type)
    {
      case LIMINE_MEMMAP_USABLE:
      {
        memmap.regions[i].type = MEMMAP_REGION_USABLE;
        break;
      }
      case LIMINE_MEMMAP_RESERVED:
      {
        memmap.regions[i].type = MEMMAP_REGION_RESERVED;
        break;
      }
      case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
      {
        memmap.regions[i].type = MEMMAP_REGION_ACPI_RECLAIMABLE;
        break;
      }
      case LIMINE_MEMMAP_ACPI_NVS:
      {
        memmap.regions[i].type = MEMMAP_REGION_ACPI_NVS;
        break;
      }
      case LIMINE_MEMMAP_BAD_MEMORY:
      {
        memmap.regions[i].type = MEMMAP_REGION_BAD_MEMORY;
        break;
      }
      case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
      {
        memmap.regions[i].type = MEMMAP_REGION_BOOTLOADER_RECLAIMABLE;
        break;
      }
      case LIMINE_MEMMAP_EXECUTABLE_AND_MODULES:
      {
        memmap.regions[i].type = MEMMAP_REGION_EXECUTABLE_AND_MODULES;
        break;
      }
      case LIMINE_MEMMAP_FRAMEBUFFER:
      {
        memmap.regions[i].type = MEMMAP_REGION_FRAMEBUFFER;
        break;
      }
      case LIMINE_MEMMAP_ACPI_TABLES:
      {
        memmap.regions[i].type = MEMMAP_REGION_ACPI_TABLES;
        break;
      }
      default: // This should cover all of limines possible types but in the case of an error we should stop
      {
        puts("Invalid or unsupported region type in limine memmap\n");
        hcf();
      }
    }
  }
}

static void
load_hhdm(void)
{
  hhdm = hhdm_request.response->offset;
}

static void *
p2v(uintptr_t phys)
{
  return (void *)(phys + hhdm);
}

static uintptr_t
v2p(void *virt)
{
  return (uintptr_t)virt - hhdm;
}

static size_t
page_round_up(void *addr)
{
  return ((size_t)addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

static size_t
page_round_down(void *addr)
{
  return (size_t)addr & ~(PAGE_SIZE - 1);
}

static void
free_pages_early(void *addr, size_t order)
{
  if ((size_t)addr % PAGE_SIZE != 0)
  {
    puts("Attempted to free unaligned page\n");
    hcf();
  }

  struct free_list_node *n = (struct free_list_node *)addr;
  n->next = 0;

  struct free_list_node **pcurr = &free_list[order];
  struct free_list_node *curr = free_list[order];

  while (curr)
  {
    if (order < MAX_ORDER)
    {
      uintptr_t buddy = (uintptr_t)n ^ (PAGE_SIZE << order);
      if ((uintptr_t)curr == buddy)
      {
        *pcurr = curr->next;

        void *block;
        if (n < curr)
        {
          block = n;
        }
        else
        {
          block = curr;
        }

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

static void
free_range(void *base, void *end)
{
  size_t p = page_round_up(base);
  size_t e = page_round_down(end);

  while (p < e)
  {
    size_t remaining = e - p;
    size_t order = MAX_ORDER;

    while (order > 0 && ((size_t)PAGE_SIZE << order > remaining || (p & (((size_t)PAGE_SIZE << order) - 1)) != 0))
    {
      order--;
    }

    free_pages_early((void *)p, order);
    p += PAGE_SIZE << order;
  }
}

static void
free_usable_regions(void)
{
  for (size_t i = 0; i < memmap.region_count; i++)
  {
    if (memmap.regions[i].type != MEMMAP_REGION_USABLE)
    {
      continue;
    }

    char *base = p2v(memmap.regions[i].phys);
    char *end = base + memmap.regions[i].size;

    free_range(base, end);
  }
}

static void *
kalloc_early(size_t order)
{
  if (order > MAX_ORDER)
  {
    puts("Invalid order for kalloc_early\n");
    hcf();
  }

  struct free_list_node **pcurr = &free_list[order];
  struct free_list_node *curr = free_list[order];

  if (curr)
  {
    *pcurr = curr->next;
    memset(curr, 0, PAGE_SIZE << order);

    return (void *)curr;
  }

  for (size_t higher = order + 1; higher <= MAX_ORDER; higher++)
  {
    struct free_list_node **phigh = &free_list[higher];
    struct free_list_node *high_curr = free_list[higher];

    if (high_curr)
    {
      *phigh = high_curr->next;

      while (higher > order)
      {
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

static ptl3_t*
walk_ptl4(ptl4_t *ptl4, uint64_t index, int create)
{
  if (!(ptl4->table[index].entry & PAGE_PRESENT))
  {
    if (!create)
    {
      return 0;
    }

    void *new_page = kalloc_early(0);
    memset(new_page, 0, PAGE_SIZE);
    ptl4->table[index].entry = v2p(new_page) | PAGE_PRESENT | PAGE_RW;
  }

  return p2v(ptl4->table[index].entry & ~0xFFF);
}

static ptl2_t *
walk_ptl3(ptl3_t *ptl3, uint64_t index, int create)
{
  if (!(ptl3->table[index].entry & PAGE_PRESENT))
  {
    if (!create)
    {
      return 0;
    }

    void *new_page = kalloc_early(0);
    memset(new_page, 0, PAGE_SIZE);
    ptl3->table[index].entry = v2p(new_page) | PAGE_PRESENT | PAGE_RW;
  }

  return p2v(ptl3->table[index].entry & ~0xFFF);
}

static ptl1_t *
walk_ptl2(ptl2_t *ptl2, uintptr_t index, int create)
{
  if (!(ptl2->table[index].entry & PAGE_PRESENT))
  {
    if (!create)
    {
      return 0;
    }

    void *new_page = kalloc_early(0);
    memset(new_page, 0, PAGE_SIZE);
    ptl2->table[index].entry = v2p(new_page) | PAGE_PRESENT | PAGE_RW;
  }

  return p2v(ptl2->table[index].entry & ~0xFFF);
}

static void
map_page(ptl4_t *l4, uintptr_t pa, uintptr_t va, uintptr_t flags)
{
  uintptr_t l4_index = (va >> 39) & 0x1FF;
  uintptr_t l3_index = (va >> 30) & 0x1FF;
  uintptr_t l2_index = (va >> 21) & 0x1FF;
  uintptr_t l1_index = (va >> 12) & 0x1FF;

  ptl3_t *l3 = walk_ptl4(l4, l4_index, 1);
  ptl2_t *l2 = walk_ptl3(l3, l3_index, 1);
  ptl1_t *l1 = walk_ptl2(l2, l2_index, 1);

  l1->table[l1_index].entry = (pa & ~0xFFF) | (flags & 0xFFF);
}

static void
switch_ptl4(ptl4_t *l4)
{
  asm ("mov %0, %%cr3" : :  "r"(v2p(l4)) : "memory");
}

static void
create_kernel_page_table(void)
{
  kernel_ptl4 = kalloc_early(0);
  memset(kernel_ptl4, 0, PAGE_SIZE);

  for (uintptr_t i = 0; i < (2ULL << 30); i += PAGE_SIZE)
  {
    map_page(kernel_ptl4, i, (uintptr_t)p2v(i), PAGE_PRESENT | PAGE_RW);
  }

  for (uintptr_t i = (uintptr_t)&__kernel_start; i < (uintptr_t)&__kernel_end; i += PAGE_SIZE)
  {
    map_page(kernel_ptl4, i, (uintptr_t)p2v(i), PAGE_PRESENT | PAGE_RW);
  }
}

static void
mp_main(void)
{
}

static void __attribute__((noreturn))
ap_enter(void)
{
  mp_main();

  hcf();
}

void
kmain(void)
{
  validate_bootloader();

  load_framebuffer();
  load_memmap();
  load_hhdm();

  free_usable_regions();

  create_kernel_page_table();
  switch_ptl4(kernel_ptl4);

  mp_main();

  hcf();
}
