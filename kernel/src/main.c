#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>

#include "font8x8.h"

__attribute__((used, section(".limine_requests"))) static volatile struct limine_framebuffer_request framebuffer_request = {
  .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
  .revision = 0
};

__attribute__((used, section(".limine_requests"))) static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(4);
__attribute__((used, section(".limine_requests_start"))) static volatile uint64_t limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;
__attribute__((used, section(".limine_requests_end"))) static volatile uint64_t limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
  uint8_t *restrict pdest = (uint8_t *restrict)dest;
  const uint8_t *restrict psrc = (const uint8_t *restrict)src;

  for (size_t i = 0; i < n; i++)
  {
    pdest[i] = psrc[i];
  }

  return dest;
}

void *memset(void *s, int c, size_t n)
{
  uint8_t *p = (uint8_t *)s;

  for (size_t i = 0; i < n; i++)
  {
    p[i] = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dest, const void *src, size_t n)
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

int memcmp(const void *s1, const void *s2, size_t n)
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

static void hcf(void)
{
  for (;;)
  {
#if defined (__x86_64__)
    asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
    asm ("wfi");
#elif defined (__loongarch64)
    asm ("idle 0");
#endif
  }
}

volatile uint32_t *framebuffer;
int framebuffer_width;
int framebuffer_height;

void draw_char(size_t x, size_t y, char c, uint32_t color)
{
  for (size_t row = 0; row < 8; row++)
  {
    unsigned char bits = font8x8[(size_t)c][row];
    for (size_t col = 0; col < 8; col++)
    {
      if (bits & (1 << (7 - col)))
      {
        framebuffer[(y + row) * framebuffer_width + (x + col)] = color;
      }
    }
  }
}

void kclear_framebuffer()
{
  memset((void *)framebuffer, 0, framebuffer_width * framebuffer_height);
}

void kputc(char c)
{
  static size_t cursor_x = 0;
  static size_t cursor_y = 0;

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

  if (cursor_x > framebuffer_width / FONT_CHAR_WIDTH)
  {
    cursor_x = 0;
    cursor_y = cursor_y + 1;
  }

  if (cursor_y > framebuffer_height / FONT_CHAR_HEIGHT)
  {
    kclear_framebuffer();
    cursor_y = 0;
  }
}

void kmain(void)
{
  if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == false)
  {
    hcf();
  }

  if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
  {
    hcf();
  }

  struct limine_framebuffer *limine_framebuffer = framebuffer_request.response->framebuffers[0];

  framebuffer = limine_framebuffer->address;
  framebuffer_width = limine_framebuffer->width;
  framebuffer_height = limine_framebuffer->height;

  for (size_t i = 0; i < 128 * 5; i++)
  {
    if (i % 128 == 0)
    {
      kputc('\n');
    }
    kputc(i % 128);
  }

  hcf();
}
