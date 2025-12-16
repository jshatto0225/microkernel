#!/usr/bin/env python3

NUM_VECTORS = 256
OUT_FILE = "src/vector.s"

def main():
    with open(OUT_FILE, "w") as f:
        f.write("/* Auto-generated interrupt vector table */\n")
        f.write(".intel_syntax noprefix\n\n")

        # Exported table
        f.write(".section .rodata\n")
        f.write(".global trap_vectors\n")
        f.write("trap_vectors:\n")

        for i in range(NUM_VECTORS):
            f.write(f"    .quad vector_{i}\n")

        f.write("\n.section .text\n")

        # Vector entry points
        for i in range(NUM_VECTORS):
            f.write(f".global vector_{i}\n")
            f.write(f"vector_{i}:\n")
            if not (i == 8 or (i >= 10 and i <= 14) or i == 17):
                f.write(f"    push 0\n")
            f.write(f"    push {i}\n")
            f.write(f"    jmp isr_common\n\n")

        # Common handler entry (defined elsewhere or here)
        f.write("/*\n")
        f.write(" * Expected ABI for isr_common:\n")
        f.write(" *   stack top -> vector number\n")
        f.write(" */\n")
        f.write(".extern isr_common\n")

if __name__ == "__main__":
    main()
