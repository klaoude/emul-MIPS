# factorial.asm
# this program test read/write file syscall

.data
    file_path: .asciiz "test1.txt"
    #toWrite:   .asciiz ""
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

    #exit()
    li $v0, 10
    syscall