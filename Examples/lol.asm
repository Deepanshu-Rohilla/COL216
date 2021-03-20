.text
.globl main
main:	

	li $v0, 5
	syscall
	move $t1, $v0

	mul $t1, $t1, -1


	li $v0, 1
	move $a0, $t1
	syscall

	li $v0, 10
	syscall
	.data