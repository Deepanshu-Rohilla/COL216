.text
.globl main


main:

	li $v0, 5
	syscall
	move $t0, $v0   # Putting value of n in t0
    li $t6 ,1      # loading value of "1" in register t6

    blt $t0,$t6,CASE3  #if n=0 then go to case3 and print message and exit 

	l.s $f0,fnum1   # This will store the area and get update till we get final area
	li $t2, 1       # This will keep track of number of points considered at any given instant of time
	l.s $f6 ,fnum3

    li $v0, 5
    syscall
    move $t3, $v0    # Value of x1
    mtc1  $t3 , $f1  # Value of x1 in float register
    cvt.s.w $f1,$f1

    li $v0, 5
    syscall
    move $t4, $v0    # Value of y1
    mtc1  $t4 , $f2  # Value of y1
    cvt.s.w $f2,$f2
	
	while:

		beq $t0, $t2,exit  #Check the condition that only "n" point are entered

		li $v0, 5
		syscall
		move $a0 , $v0   # Value of x2
		mtc1  $a0 , $f3  
        cvt.s.w $f3,$f3  # Converting 32 bit-int Register into 32 bit- float Register

		li $v0, 5
		syscall
		move $a1, $v0    # Value of y2
		mtc1  $a1 , $f4  
        cvt.s.w $f4,$f4  # Converting 32 bit-int Register into 32 bit- float Register

		blt $t4, 0, CASE1  # if y1<0 then move to CASE1 label

		blt $a1, 0, CASE2  # if y2<0 then move to CASE2 label

        #Below program only run if y1>=0 and y2 >=0 
         
        # Both points line on above X-axis and below program us used to compute area

		add.s $f2, $f2, $f4  # Storing y1+y2 in t4
		sub.s $f1, $f3, $f1  # Storing x2-x1 in t3
		mul.s $f1,$f1, $f2   # Calculating (y2+y1)*(x2-x1)
		l.s $f5, fnum2       # storing 2.00 in f5
		div.s $f1,$f1, $f5   # calculating the area using formula 0.5*(y2+y1)*(x2-x1) 
		add.s $f0, $f0, $f1  # adding this value to total area
		addi $t2, $t2, 1     # increment the total points considered
		move $t3, $a0        # move x2 in x1 waala register (simply means  putting value of x2 in x1 )
		mtc1 $t3,$f1 
		cvt.s.w $f1,$f1      # Converting 32 bit-int Register into 32 bit- float Register
		move $t4, $a1        # move y2 in y1 waala register (simply means  putting value of y2 in y1 )
	    mtc1 $t4,$f2 
		cvt.s.w $f2,$f2      # Converting 32 bit-int Register into 32 bit- float Register  
		j while
		

	CASE1: 
    
		bgt $a1, 0, CASE2 # if y2 >0 then move to CASE 2 condition

        # Below program run if (y1<0 and y2<=0)  

		add.s $f2, $f2, $f4    # Storing y1+y2 in f2
		sub.s $f1, $f3, $f1    # Storing x2-x1 in f1
		mul.s $f2, $f2, $f6    # Making (y1+y2) positive by multiplying by (-1.00)
		mul.s $f1,$f1, $f2     # Calculating |(y2+y1)|*(x2-x1)
		l.s $f5, fnum2         # storing 2.00 in f5
		div.s $f1,$f1, $f5     # calculating the area using formula 0.5*|(y2+y1)|*(x2-x1) 
		add.s $f0, $f0, $f1    # adding this value to total area
		addi $t2, $t2, 1       # increment the total points considered
		move $t3, $a0          # move x2 in x1 waala register (simply means  putting value of x2 in x1 )
        mtc1 $t3,$f1
		cvt.s.w $f1,$f1        # Converting 32 bit-int Register into 32 bit- float Register
		move $t4, $a1          # move y2 in y1 waala register (simply means  putting value of y2 in y1 )
        mtc1 $t4,$f2
		cvt.s.w $f2,$f2        # Converting 32 bit-int Register into 32 bit- float Register
		j while


	CASE2:

        # Below program run if  (y1>=0 and y2<0) or (y1<0 and y2>0)

        mul.s $f9,$f1,$f4 
        mul.s $f10,$f2,$f3
        sub.s $f9,$f9,$f10
        sub.s $f10,$f4,$f2
        div.s $f9,$f9,$f10    # Calculate the "ZeroPoint" (i.e point on X-axis) i.e x-cordinate value where line joining (x1,y1) and (x2,y2) cut X-axis
                              # ZeroPoint = (x1*y2 - x2*y1)/(y2-y1)
        abs.s $f2,$f2         # making y1 value positive i.e |y1|
        abs.s $f4,$f4         # making y2 value positive i.e |y2|
        sub.s $f10,$f9,$f1     
        mul.s $f10,$f10,$f2
        sub.s $f9,$f3,$f9
        mul.s $f9,$f9,$f4
        add.s $f9,$f9,$f10
        l.s $f5, fnum2        # storing 2.00 in $f5
        div.s $f9,$f9,$f5     # Area is calculated by calulating area of 2 triangle using formula ((ZeroPoint-x1)*|(y1)| + (x2-ZeroPoint)*|(y2)|)/2.0
        add.s $f0, $f0, $f9   # adding this value to total area
        addi $t2, $t2, 1      # increment the total points considered
        move $t3, $a0         # move x2 in x1 waala register (simply means  putting value of x2 in x1 )
        mtc1 $t3,$f1
        cvt.s.w $f1,$f1       # Converting 32 bit-int Register into 32 bit- float Register
        move $t4, $a1         # move y2 in y1 waala register (simply means  putting value of y2 in y1 )
        mtc1 $t4,$f2 
        cvt.s.w $f2,$f2       # Converting 32 bit-int Register into 32 bit- float Register 

        j while

    CASE3:
       
       # when value of "n" is less than or equal to 0 i.e invalid input

       li $v0,4   #print_string syscall code=4
       la $a0,msg  #load the addess of msg
       syscall
    
       li $v0, 10
	   syscall
     
		
exit:
	
	li $v0, 2
    mov.s $f12, $f0   # Move contents of register $f0 to register $f12
    syscall

	li $v0, 10
	syscall

.data
    fnum1: .float 0.00
    fnum2: .float 2.00
    fnum3: .float -1.00
    msg: .asciiz "Value of n should be greater than 0" 















