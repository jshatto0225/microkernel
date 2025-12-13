#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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

#define PAGE_TABLE_ENTRY_COUNT 512

#define HUGE_PAGE (1ULL << 7)

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

static uintptr_t *pml4;

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
      cursor_y++;
      cursor_x = 0;
      break;
    default:
      draw_char(cursor_x * 8, cursor_y * 8, c, 0xFFFFFFFF);
      cursor_x = cursor_x + 1;
      break;
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

    while (order > 0 && ((size_t)PAGE_SIZE << order > remaining || (p & ((size_t)PAGE_SIZE << order) - 1) != 0))
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

static uintptr_t
clear_pte_flags(uintptr_t pte)
{
  return pte & ~0xFFF;
}

static uintptr_t
pte_flags(uintptr_t pte)
{
  return pte & 0xFFF;
}

static void
vm_init(void)
{
  pml4 = kalloc_early(0);

  uintptr_t old_pml4_entry;
  asm ("mov %%cr3, %0" : "=r"(old_pml4_entry));
  uintptr_t *old_pml4 = p2v(clear_pte_flags(old_pml4_entry));

  for (size_t i = 0; i < PAGE_TABLE_ENTRY_COUNT; i++)
  {
    if (old_pml4[i] == 0)
    {
      continue;
    }

    uintptr_t *old_pdpt = p2v(clear_pte_flags(old_pml4[i]));

    uintptr_t *pdpt = kalloc_early(0);
    memset(pdpt, 0, PAGE_SIZE);
    pml4[i] = v2p(pdpt) | pte_flags(old_pml4[i]);

    for (size_t j = 0; j < PAGE_TABLE_ENTRY_COUNT; j++)
    {
      if (old_pdpt[j] == 0)
      {
        continue;
      }

      if (old_pdpt[j] & HUGE_PAGE)
      {
        pdpt[j] = old_pdpt[j];
        continue;
      }

      uintptr_t *old_pd = p2v(clear_pte_flags(old_pdpt[j]));

      uintptr_t *pd = kalloc_early(0);
      memset(pd, 0, PAGE_SIZE);
      pdpt[j] = v2p(pd) | pte_flags(old_pdpt[j]);

      for (size_t k = 0; k < PAGE_TABLE_ENTRY_COUNT; k++)
      {
        if (old_pd[k] == 0)
        {
          continue;
        }

        if (old_pd[k] & HUGE_PAGE)
        {
          pd[k] = old_pd[k];
          continue;
        }

        uintptr_t *old_pt = p2v(clear_pte_flags(old_pd[k]));

        uintptr_t *pt = kalloc_early(0);
        memset(pt, 0, PAGE_SIZE);
        pd[k] = v2p(pt) | pte_flags(old_pd[k]);

        memcpy(pt, old_pt, sizeof(uintptr_t) * PAGE_TABLE_ENTRY_COUNT);
      }
    }
  }

  asm("mov %0, %%cr3" : : "r"(v2p(pml4) | pte_flags(old_pml4_entry)) : "memory");
}

void
kmain(void)
{
  validate_bootloader();

  load_framebuffer();
  load_memmap();
  load_hhdm();

  free_usable_regions();

  vm_init();

  hcf();
}
