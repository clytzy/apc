    .arch armv8-a
    .globl neon2_read
    .p2align 2
    .type neon2_read, @function
neon2_read:
	
	mov  	x9,x0  // 第一个参数 1000000 表示操作 1000000K 数据   1000000*1024
	mov 	x10,x1  // 第二个参数 100 表示循环100次
	mov 	x11,x2   // 第三个参数 1 , 目前没有使用
	mov 	x12,x3   //第四个参数 pi， 是一个地址 (char *)(((long)pi1)&0xffffffffffffff00);
	mov 	x13,x4   //第五个参数 po， (char *)(((long)po1)&0xffffffffffffff00);
							//次数
							//总量/4k
.L_loop:					// 重复取相同的8条4K bytes ，如果设置为1，则跟进成4k*N
	ldr		q0,[x3,#0]
	ldr		q1,[x3,#16]
	ldr		q2,[x3,#32]
	ldr		q3,[x3,#48]
	ldr		q4,[x3,#64]
	ldr		q5,[x3,#80]
	ldr		q6,[x3,#96]
	ldr		q7,[x3,#112]
	add		x3,x3,#128	
	//ldr         x5, [x3,#0]
	//str         x5, [x4,#0] 
	//ldr         x6, [x3,#8]
	//str         x6, [x4,#8] 
	//ldr         x7, [x3,#16]
	//str         x7, [x4,#16] 
	//ldr         x8, [x3,#24]
	//str         x8, [x4,#24] 
	//ldr         x14, [x3,#32]
	//str         x14, [x4,#32] 
	//ldr         x15, [x3,#40]
	//str         x15, [x4,#40] 
	//ldr         x16, [x3,#48]
	//str         x16, [x4,#48] 
	//ldr         x17, [x3,#56]
	//str         x17, [x4,#56] 
	//ldr         x18, [x3,#64]
	//str         x18, [x4,#64] 
	//ldr         x19, [x3,#72]
	//str         x19, [x4,#72] 
	//ldr         x20, [x3,#80]
	//str         x20, [x4,#80] 
	//ldr         x21, [x3,#88]
	//str         x21, [x4,#88] 
	//ldr         x22, [x3,#96]
	//str         x22, [x4,#96] 
	//ldr         x23, [x3,#104]
	//str         x23, [x4,#104] 
	//ldr         x24, [x3,#112]
	//str         x24, [x4,#112] 
	//ldr         x25, [x3,#120]
	//str         x25, [x4,#120] 
	//add		    x3,x3, #128	
	//add		    x4,x4, #128
    subs 	x0, x0, #1 //memblock*8   8*8*16=
    bgt .L_loop
	mov 	x0,x9
	mov 	x3,x12
	subs	x1,x1,#1 //loop num
    bgt .L_loop
	//mov 	x1,x10
	//subs	x2,x2,#1//1
	
	//str    	q0,[x4,#0]
	//str    	q1,[x4,#16]
	//str    	q2,[x4,#32]
	//str    	q3,[x4,#48]
	//str    	q4,[x4,#64]
	//str    	q5,[x4,#80]
	//str    	q6,[x4,#96]
	//str    	q7,[x4,#112]	
	//bgt  .L_loop
	ret 	
	
    .globl neon2_write
    .p2align 2
    .type neon2_write, @function
neon2_write:
	mov  	x9,x0
	mov 	x10,x1
	mov 	x11,x2
	mov 	x12,x3
	mov 	x13,x4
	ldr		q0,[x3,#0]
	ldr		q1,[x3,#16]
	ldr		q2,[x3,#32]
	ldr		q3,[x3,#48]
	ldr		q4,[x3,#64]
	ldr		q5,[x3,#80]
	ldr		q6,[x3,#96]
	ldr		q7,[x3,#112]							//次数
							//总量/4k
.L_loop1:					// 重复取相同的8条4K bytes ，如果设置为1，则跟进成4k*N
	str    	q0,[x4,#0]
	str    	q1,[x4,#16]
	str    	q2,[x4,#32]
	str    	q3,[x4,#48]
	str    	q4,[x4,#64]
	str    	q5,[x4,#80]
	str    	q6,[x4,#96]
	str    	q7,[x4,#112]
	add		x4,x4,#128
    subs 	x0, x0, #1
    bgt .L_loop1
	mov 	x0,x9
	mov 	x4,x13
	subs	x1,x1,#1
    bgt .L_loop1
	mov 	x1,x10
	subs	x2,x2,#1
	bgt  .L_loop1
	ret 	

    .globl neon2_read_write
    .p2align 2
    .type neon2_read_write, @function
neon2_read_write:
	mov  	x9,x0
	mov 	x10,x1
	mov 	x11,x2
	mov 	x12,x3
	mov 	x13,x4
	ldr		q0,[x3,#0]
	ldr		q1,[x3,#16]
	ldr		q2,[x3,#32]
	ldr		q3,[x3,#48]
	ldr		q4,[x3,#64]
	ldr		q5,[x3,#80]
	ldr		q6,[x3,#96]
	ldr		q7,[x3,#112]
	add		x3,x3,#128	
	
							//次数
							//总量/4k
.L_loop2:					// 重复取相同的8条4K bytes ，如果设置为1，则跟进成4k*N
	str    	q0,[x4,#0]
	ldr		q0,[x3,#0]
	str    	q1,[x4,#16]
	ldr		q1,[x3,#16]
	str    	q2,[x4,#32]
	ldr		q2,[x3,#32]
	str    	q3,[x4,#48]
	ldr		q3,[x3,#48]
	str    	q4,[x4,#64]
	ldr		q4,[x3,#64]
	str    	q5,[x4,#80]
	ldr		q5,[x3,#80]
	str    	q6,[x4,#96]
	ldr		q6,[x3,#96]
	str    	q7,[x4,#112]
	ldr		q7,[x3,#112]
	add		x3,x3,#128
	add		x4,x4,#128
    subs 	x0, x0, #1
    bgt .L_loop2
	mov 	x3,x12
	mov 	x4,x13
	mov 	x0,x9
	ldr		q0,[x3,#0]
	ldr		q1,[x3,#16]
	ldr		q2,[x3,#32]
	ldr		q3,[x3,#48]
	ldr		q4,[x3,#64]
	ldr		q5,[x3,#80]
	ldr		q6,[x3,#96]
	ldr		q7,[x3,#112]
	add		x3,x3,#128	
	subs	x1,x1,#1
    bgt .L_loop2
	mov 	x1,x10
	subs	x2,x2,#1
	bgt  .L_loop2
	ret 	
	


