.text
.globl main

main:

    li $t9 ,0   # This is used to store final answer or for valid postfix expression it will be only number present in stack finally
    li $t8, 0   # This is used to store "current  size of stack" at any time 
    li $t7,2    # Used for comparing : Whether Size of stack is greater than 2 or not for valid operation to be perform 
    li $t6,-1   # Used to store -1 which will be used to swap the sign after subtraction.
    li $s6,-1   # Used as counter for accesssing  string character from memory address 

    la $a0, st1    # Load and print string asking for Postfix expression
    li $v0, 4
    syscall
 
    li $v0, 8       # Take input in form of string for Postfix expression 

    la $a0, buffer  # load byte space into address 
    syscall

    while:
        
        la $s1, buffer  # buffer = $s1
        li $s2, '+'
        li $s3, '-'
        li $s4, '*'
        li $s5, '\n'
	    addi $s6, $s6, 1  # counter increment  
	
	   # get buffer[counter]

	    add $s1, $s1, $s6


	    lb $t1, 0($s1)	# t1 = value of buffer[counter]
	
		# Check whether character belong to an operation(+,*,-) or numeric value form 0-9
	
	    beq $t1, $s2, addition        # '+' : Check whether if $t1 = '+' 

	    beq $t1, $s3, subtraction     # '-' : Check whether if $t1 = '-' 

	    beq $t1, $s4, multiplication  # '*'  : Check whether if $t1 = '*' 

        beq $t1 , $s5 ,check          # Check whether we have reached at end of line ("\n")

        blt $t1, 48, error1  # Check if char is not a digit (ascii<'0') => This is to check whether string contain (0-9) numeric or not
        bgt $t1, 57, error1  # Check if char is not a digit (ascii>'9') => This is to check whether string contain (0-9) numeric or not
        addi $t1, $t1, -48  # Converts t1's ascii value to decimal value 

        subu $sp, $sp ,4    # Memory access for "Push operation" in Stack
        sw $t1 ,($sp)       # Pushing of data (0-9) into stack
        addi $t8,$t8,1      # Update the size of Stack

        j while

        addition:

            blt $t8 , $t7 ,error  # Check whether stack size is less than 2 or not if is less than 2 then print error messsage otherwise perform operation

            lw $t2 , ($sp)     # Pop the element from the Stack and store in the $t2
            addu $sp,$sp, 4    # Memory access for pop operation in Stack

            lw $t3 , ($sp)     # Pop the element from the Stack and store in the $t3
            addu $sp,$sp, 4    # Memory access for pop operation in Stack

            add $t9,$t2,$t3    # Addition operation performed 

            sub $t8,$t8,$t7    # Reduce the size of stack by 2

            subu $sp, $sp ,4   # Memory access for push operation in Stack
            sw $t9 ,($sp)      # Pushing of numeric data  into stack
            addi $t8,$t8,1     # Update the size of Stack

            j while

        subtraction:

            blt $t8 , $t7 ,error   # Check whether stack size is less than 2 or not if is less than 2 then print error messsage otherwise perform operation

            lw $t2 , ($sp)     # Pop the element from the Stack and store in the $t2
            addu $sp,$sp, 4    # Memory access for pop operation in Stack

            lw $t3 , ($sp)     # Pop the element from the Stack and store in the $t3
            addu $sp,$sp, 4    # Memory access for pop operation in Stack

            sub $t9 ,$t2,$t3   # Subtraction operation performed 
            mul $t9, $t9, $t6


            sub $t8,$t8,$t7    # Reduce the size of stack by 2

            subu $sp, $sp ,4   # Memory access for push operation in Stack
            sw $t9 ,($sp)      # Pushing of numeric data into stack
            addi $t8,$t8,1     # Update the size of Stack

            j while

        multiplication:

            blt $t8 , $t7 ,error   # Check whether stack size is less than 2 or not if is less than 2 then print error messsage otherwise perform operation


            lw $t2 , ($sp)    # Pop the element from the Stack and store in the $t2
            addu $sp,$sp, 4   # Memory access for pop operation in Stack

            lw $t3 , ($sp)    # Pop the element from the Stack and store in the $t3
            addu $sp,$sp, 4   # Memory access for pop operation in Stack

            mul $t9 ,$t2,$t3  # Multiplication operation performed 

            sub $t8,$t8,$t7   # Reduce the size of stack by 2

            subu $sp, $sp ,4  # Memory access for push operation in Stack
            sw $t9 ,($sp)     # Pushing of numeric data  into stack
            addi $t8,$t8,1    # Update the size of Stack
        
            j while

        error:

            li $v0,4          # Print_string syscall code=4
            la $a0,msg        # Load the address of msg1
            syscall
    
            li $v0, 10        # Terminate the program
	        syscall

        error1:

            li $v0,4          # Print_string syscall code=4
            la $a0,msg1       # Load the address of msg1
            syscall
    
            li $v0, 10        # Terminate the program
	        syscall

    check:

       li  $t2 ,1 
       beq $t8,$t2, exit      # Check whether stack size is equal to 1 or not

       li $v0,4               # Print_string syscall code=4
       la $a0,msg             # Load the address of msg
       syscall

	   li $v0, 10             # Terminate the program
	   syscall

exit:
	
    lw $t9 , ($sp)           # Load the final answer from stack into $t9
    addu $sp,$sp, 4          # Memory address adjustment 

	li,$v0,1                 
    move $a0,$t9             # Print the final answer 
    syscall

	li $v0, 10               # Terminate the program
	syscall

.data
  
    buffer: .space 1000
    st1:  .asciiz "Enter the postfix expression: "
    msg: .asciiz "Expression for Postfix is wrong"
    msg1: .asciiz "Expression for Postfix is wrong as it involve :- Character for operation which is not allowed "
    num1 :  .word 0