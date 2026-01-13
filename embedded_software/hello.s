.section .text
.global _start

_start:
    # Load address of strings
    la a0, my_string_1
    call put_string
    
    la a0, my_string_2
    call put_string
    
    # Halt execution (using ebreak as per emulator implementation)
    ebreak

# Function to print a null-terminated string
# Input: a0 = address of string
put_string:
    # CharOut address is 0x10000000
    li a2, 0x10000000

loop_start:
    lbu a1, 0(a0)       # Load byte from string
    beq a1, zero, loop_end # If null terminator, exit loop
    
    sb a1, 0(a2)        # Write byte to CharOut
    addi a0, a0, 1      # Increment string pointer
    j loop_start

loop_end:
    ret

.section .rodata
my_string_1:
    .string "Hello, World!\n"
my_string_2:
    .string "What's up?\n"
