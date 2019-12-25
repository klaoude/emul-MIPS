# factorial.asm
# this program ask a number to the user and print the factorial of this number

.data
prompt:    .asciiz     "Enter the number: "
resultStr: .asciiz     "! = "

.text
main:
    #print(prompt)
    la $a0, prompt
    li $v0, 4
    syscall                   

    #$t0 = gets()
    li $v0, 5
    syscall
    move $t0, $v0
    move $a0, $t0

    #t1 = fact(t0)
    jal fact
    move $t1, $v0

    #print(t0)
    move $a0, $t0
    li $v0, 1
    syscall

    #print(resultStr)
    la $a0, resultStr
    li $v0, 4
    syscall

    #print(t1)
    move $a0, $t1
    li $v0, 1
    syscall

    #exit()
    li $v0, 10
    syscall

fact:
    addi $sp, $sp, -8      # create space on stack
    sw $s0, ($sp)          # push s0
    sw $ra, 4($sp)         # push ra, store return addr

    li $v0, 1              # v0 = 1
    beq $a0, 0, fact_done  # if(a0 == 0) goto fact_done

    move $s0, $a0          # s0 = a0
    addi $a0, $a0, -1      # a0--
    jal fact               # fact()

    mul $v0, $s0, $v0      # v0 *= s0

fact_done:
    lw $s0, ($sp)          # pop s0
    lw $ra, 4($sp)         # pop ra
    addi $sp, $sp, 8       # sp += 8

    jr $ra                 # jmp ra