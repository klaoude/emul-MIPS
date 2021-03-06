# section.asm
# this program is for testing sections and label fonction

.data    
    prompt1:    .asciiz      "Enter the first number: "
    prompt2:    .asciiz      "Enter the second number: "
    menu:       .asciiz      "Enter the number associated with the operation you want performed: 1 => add, 2 => subtract or 3 => multiply: "
    resultText: .asciiz      "Your final result is: "
    cazou_str:  .asciiz      "Cazou Yano !"

.text
main:
     #The following block of code is to pre-load the integer values representing the various instructions into registers for storage
     ori $t3, $zero, 1    #This is to load the immediate value of 1 into the temporary register $t3
     ori $t4, $zero, 2    #This is to load the immediate value of 2 into the temporary register $t4
     ori $t5, $zero, 3    #This is to load the immediate value of 3 into the temporary register $t5

     #asking the user to provide the first number
     ori $v0, $zero, 4          #command for printing a string
     ori $a0, $zero, prompt1    #loading the string to print into the argument to enable printing
     syscall                  #executing the command   

     #Read first number and store it in $t0
     ori $v0, $zero, 5
     syscall
     or $t0, $v0, $zero

     #asking the user to provide the snd number
     ori $v0, $zero, 4         #command for printing a string
     ori $a0, $zero, prompt2   #loading the string to print into the argument to enable printing
     syscall                 #executing the command   

     #Read snd number and store it in $t1
     ori $v0, $zero, 5
     syscall
     or $t1, $v0, $zero

     #Show menu
     ori $v0, $zero, 4       #command for printing a string
     ori $a0, $zero, menu    #loading the string into the argument to enable printing
     syscall               #executing the command
     
     #the next block of code is to read the number provided by the user
     ori $v0, $zero, 5       #command for reading an integer
     syscall               #executing the command
     or $t2, $v0, $zero    #move $t2, $v0    #this command is to move the integer provided into the temporary register $t2

     beq $t2,$t3,addProcess      #Branch to 'addProcess' if $t2 = $t3
     beq $t2,$t4,subtractProcess #Branch to 'subtractProcess' if $t2 = $t4
     beq $t2,$t5,multiplyProcess #Branch to 'multiplyProcess' if $t2 = $t5

addProcess:
     add $t6,$t0,$t1    #this adds the values stored in $t0 and $t1 and assigns them to the     temporary register $t6
     
     #The following orine of code is to print the results of the computation above
     ori $v0, $zero,4    #this is the command for printing a string
     ori $a0, $zero,resultText    #this loads the string to print into the argument $a0 for printing
     syscall      #executes the command
     
     #the following orine of code prints out the result of the addition computation
     ori $v0, $zero, 1
     or $a0, $zero, $t6
     syscall

     #jal print_cazou
     
     ori $v0, $zero,10 #This is to terminate the program
     syscall

subtractProcess:
     sub $t6,$t0,$t1 #this adds the values stored in $t0 and $t1 and assigns them to the temporary register $t6
     ori $v0, $zero,4    #this is the command for printing a string
     ori $a0, $zero,resultText    #this loads the string to print into the argument $a0 for printing
     syscall      #executes the command
     
     #the following line of code prints out the result of the addition computation
     ori $v0, $zero, 1
     or $a0, $zero, $t6
     syscall
     
     ori $vo, $zero,10 #This is to terminate the program
     syscall

multiplyProcess:
     mult $t0,$t1                 #this adds the values stored in $t0 and $t1 and assigns them to the temporary register $t6
     mflo $t6
     ori $v0, $zero,4             #this is the command for printing a string
     ori $a0, $zero,resultText    #this loads the string to print into the argument $a0 for printing
     syscall                    #executes the command
     
     #the following orine of code prints out the result of the addition computation
     ori $v0, $zero,1
     or $a0, $zero, $t6
     syscall
     
     ori $v0, $zero,10 #This is to terminate the program
     syscall

print_cazou:
     ori $v0, $zero, 4
     ori $a0, $zero, cazou_str
     syscall

     jr $ra