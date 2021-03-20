.text
.globl main
main:
	li $v0, 5
	syscall
	move $t0, $v0  # Putting value of n in t0

	li $t1, 0 # This will store the area 
	li $t2, 1 # This will keep track of number of points considered at any given instant of time

	li $v0, 5
	syscall
	move $t3, $v0 # Value of x1

	li $v0, 5
	syscall
	move $t4, $v0 # Value of y1
	
	while:
		beq $t0, $t2,exit
		li $v0, 5
		syscall
		move $a0, $v0 # Value of x2

		li $v0, 5
		syscall
		move $a1, $v0 # Value of y2

		# formula is 0.5*(y2+y1)*(x2-x1) for area 

		blt $t4, 0, ELSE1
		blt $a1, 0, ELSE2

		add $t4, $t4, $a1  # Storing y1+y2 in t4
		sub $t3, $a0, $t3 # Storing x2-x1 in t3
		mult $t3, $t4 # Calculating (y2+y1)*(x2-x1)
		mflo $t3 # Storing the product in t3
		li $t4, 2 # storing 2 in t4
		div $t3, $t4 # calculating the area
		mflo $t3 # extracting area from low register
		add $t1, $t1, $t3 # adding this value to total area
		addi $t2, $t2, 1 # increment the total points considered
		move $t3, $a0 # move y2 in y1 waala register
		move $t4, $a1 # move x2 in x1 waala register
		j while
		

	ELSE1: 
		bgt $a1, 0, ELSE2
		add $t4, $t4, $a1  # Storing y1+y2 in t4
		mul $t1, $t1, -1
		sub $t3, $a0, $t3 # Storing x2-x1 in t3
		mult $t3, $t4 # Calculating (y2+y1)*(x2-x1)
		mflo $t3 # Storing the product in t3
		li $t4, 2 # storing 2 in t4
		div $t3, $t4 # calculating the area
		mflo $t3 # extracting area from low register
		add $t1, $t1, $t3 # adding this value to total area
		addi $t2, $t2, 1 # increment the total points considered
		move $t3, $a0 # move y2 in y1 waala register
		move $t4, $a1 # move x2 in x1 waala register
		j while


	ELSE2:
			# float zeroPt = (x1*y2 - x2*y1)/(y2-y1);
			# return ((zeroPt-x1)*abs(y1) + (x2-zeroPt)*abs(y2))/2.0;


			
			# make sure to execute the following line in the end:
	#		add $t1, $t1, $t3 # adding this value to total area 
	#	addi $t2, $t2, 1 # increment the total points considered
	#	move $t3, $a0 # move y2 in y1 waala register
	#	move $t4, $a1 # move x2 in x1 waala register
		



		j while
exit:
	li $v0, 1
	move $a0, $t1
	syscall

	li $v0, 10
	syscall
	.data














