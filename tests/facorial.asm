# factorial.asm
# this program ask a number to the user and print the factorial of this number

.data
prompt:    .asciiz     "Enter the number: "
resultStr: .asciiz     "! = "

.text
main:
    #print(prompt)
    ori $a0, $zero, prompt
    ori $v0, $zero, 4
    syscall                   

    #$a0 = gets()
    ori $v0, $zero, 5
    syscall
    or $a0, $zero, $v0

    #t0 = a0
    or $t0, $zero, $a0

    #t1 = fact()
    jal fact
    or $t1, $zero, $v0

    #print(t0)
    or $a0, $zero, $t0
    ori $v0, $zero, 1
    syscall

    #print(resultStr)
    ori $a0, $zero, resultStr
    ori $v0, $zero, 4
    syscall

    #print(t1)
    or $a0, $zero, $t1
    ori $v0, $zero, 1
    syscall

    #exit()
    ori $v0, $zero, 10
    syscall

fact:
    addi $sp, $sp, -8      # create space on stack
    sw $s0, ($sp)          # push s0
    sw $ra, 4($sp)         # push ra, store return addr

    ori $v0, $zero, 1      # v0 = 1
    beq $a0, 0, fact_done  # if(a0 == 0) goto fact_done

    or $s0, $zero, $a0     # s0 = a0
    addi $a0, $a0, -1      # a0--
    jal fact               # fact()

    mult $s0, $v0          # v0 *= s0
    mflo $v0

fact_done:
    lw $s0, ($sp)          # pop s0
    lw $ra, 4($sp)         # pop ra
    addi $sp, $sp, 8       # sp += 8

    jr $ra                 # jmp ra