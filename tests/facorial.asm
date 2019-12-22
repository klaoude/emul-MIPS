# factorial.asm
# this program ask a number to the user and print the factorial of this number

.data
prompt:    .asciiz     "Enter the number: "
resultStr: .asciiz     "! = "

.text
main:
    ori $a0, $zero, prompt
    ori $v0, $zero, 4
    syscall

    ori $v0, $zero, 5
    syscall
    or $a0, $zero, $v0

    jal fact

    ori $v0, $zero, 10
    syscall

fact:
    addi $sp, $sp, -8
    sw $s0, ($sp)
    sw $ra, 4($sp)

    ori $v0, $zero, 1
    beq $a0, 0, fact_done

    or $s0, $zero, $a0
    addi $a0, $a0, -1
    jal fact

    mult $s0, $v0
    mflo $v0

fact_done:
    lw $s0, ($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8

    jr $ra