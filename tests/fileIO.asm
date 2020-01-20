# factorial.asm
# this program test read/write file syscall

.data
    file_path: .asciiz "test1.txt"
    toWrite:   .asciiz "This is the exemple for writing to the file. Second phrase"
    str1:      .asciiz "Opening "
    str2:      .asciiz " for writing."
    str3:      .asciiz " for reading."
    str4:      .asciiz "Content writed."
    str5:      .asciiz "Reading from file: "

.text
main:
    #print(str1)
    la $a0, str1
    li $v0, 4
    syscall

    #print(file_path)
    la $a0, file_path
    li $v0, 4
    syscall

    #print(str2)
    la $a0, str2
    li $v0, 4
    syscall

    #print('\n')
    li $a0, 10
    li $v0, 11
    syscall

    #t0 = open(file_path, "w")
    la $a0, file_path
    li $a1, 1
    li $a2, 0
    li $v0, 13
    syscall
    move $t0, $v0

    #write($to, toWrite, 59)
    move $a0, $t0
    la $a1, toWrite
    li $a2, 59
    li $v0, 15
    syscall

    #print(str4)
    la $a0, str4
    li $v0, 4
    syscall

    #print('\n')
    li $a0, 10
    li $v0, 11
    syscall

    #print(str5)
    la $a0, str5
    li $v0, 4
    syscall

    #fclose($t0)
    move $a0, $t0
    li $v0, 16
    syscall

    #$t0 = fopen(file_path, "r")
    la $a0, file_path
    li $a1, 0
    li $a2, 0
    li $v0, 13
    syscall
    move $t0, $v0

    #$t1 = malloc(45)
    li $a0, 44
    li $v0, 9
    syscall
    move $t1, $v0

    #$t1 = read($t0, 44)
    move $a0, $t0
    move $a1, $t1
    li $a2, 44
    li $v0, 14
    syscall

    move $a0, $t1
    li $v0, 4
    syscall

    #fclose($t0)
    move $a0, $t0
    li $v0, 16
    syscall

    #exit()
    li $v0, 10
    syscall