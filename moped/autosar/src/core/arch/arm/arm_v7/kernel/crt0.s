.extern	__ram_end__
.extern	__und_stack_size__
.extern __abt_stack_size__
.extern __fiq_stack_size__
.extern __irq_stack_size__
.extern __svc_stack_size__
.extern _bss_start
.extern _bss_end
.extern IrqHandler
.extern Irq_Entry
.extern main

     .set    MODE_USR, 0x10
     .set    MODE_FIQ, 0x11
     .set    MODE_IRQ, 0x12
     .set    MODE_SVC, 0x13
     .set    MODE_ABT, 0x17
     .set    MODE_UND, 0x1B
     .set    MODE_SYS, 0x1F
     .set    MODE_HYP, 0x1A
     .set    MODE_MASK, 0x1F
     .set    I_BIT, 0x80
     .set    F_BIT, 0x40

     .section .init
     .code 32
     .balign 4
	 .globl _start
_start:
_exception_vector:	
	                                    /* All the following instruction should be read as: Load the address at symbol into the program counter.*/
		 ldr  pc,reset_handler		    /*	Processor Reset handler 		-- we will have to force this on the raspi! Because this is the first instruction executed, of cause it causes an immediate branch into reset!*/
		 ldr  pc,undefined_handler	    /* 	Undefined instruction handler 	-- processors that don't have thumb can emulate thumb!*/
   		 ldr  pc,swi_handler			/* 	Software interrupt / TRAP (SVC) -- system SVC handler for switching to kernel mode.*/
   		 ldr  pc,prefetch_handler		/* 	Prefetch/abort handler.*/
    	 ldr  pc,data_handler			/* 	Data abort handler/*/
    	 nop
         ldr  pc,irq_handler			/* 	IRQ handler*/
         ldr  pc,fiq_handler			/*	Fast interrupt handler.*/

	.code 32
	.balign 4
/* Here we create an exception address table! This means that reset/hang/irq can be absolute addresses*/
reset_handler:
 					.word ResetHandler
undefined_handler:
  					.word UndHandler
swi_handler:
					.word SwiHandler
prefetch_handler:
 					.word PrefetchHandler
data_handler:
					.word AbortHandler
					.word   0
irq_handler:
					.word IrqHandler
fiq_handler:
					.word FiqHandler


.code   32
.balign 4
.global ResetHandler
ResetHandler:

    /* enable jtag */
    ldr r10, =0x3F200000
    ldr r11, =0x00002000
    ldr r12, =0x0061B0C0
    str r11, [r10, #0]
    str r12, [r10, #8]
           
    /* are we in hyp mode? */
    mrs r10, CPSR_all
    and r10, r10, #MODE_MASK
    cmp r10, #MODE_HYP
    bne svc_reset

    /* hyp -> svc mode switch */
    ldr r10, =svc_reset
    ldr r11, =MODE_SVC | I_BIT | F_BIT
    msr elr_hyp, r10
    msr spsr_hyp, r11
    eret

svc_reset:
	/* set exception vector */
	ldr r0, = _exception_vector
	MCR p15, 0, r0, c12, c0, 0
 


		ldr     r0, =__ram_end__
        /* Undefined */
        msr     CPSR_c, #MODE_UND | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__und_stack_size__
        sub     r0, r0, r1
        /* Abort */
        msr     CPSR_c, #MODE_ABT | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__abt_stack_size__
        sub     r0, r0, r1
        /* FIQ */
        msr     CPSR_c, #MODE_FIQ | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__fiq_stack_size__
        sub     r0, r0, r1
        /* IRQ */
        msr     CPSR_c, #MODE_IRQ | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__irq_stack_size__
        sub     r0, r0, r1
        /* Supervisor */
        msr     CPSR_c, #MODE_SVC | I_BIT | F_BIT
        mov     sp, r0
        ldr     r1, =__svc_stack_size__
        sub     r0, r0, r1
        /* System */
        msr     CPSR_c, #MODE_SYS | I_BIT | F_BIT
        mov     sp, r0


	    mrc p15, 0, r0, c1, c0, 0
	    orr r0, #1 << 22
	    mcr p15, 0, r0, c1, c0, 0

        mov     r0, #0
        ldr     r1, =_bss_start
        ldr     r2, =_bss_end
bssloop:
        cmp     r1, r2
        strlo   r0, [r1], #4
        blo     bssloop

        /* Call the application's entry point.*/
	    mov     r2,		#0xDF
        msr     cpsr_c, r2
        b       main

.code 32
.balign 4
.weak UndHandler
UndHandler:

.weak SwiHandler
SwiHandler:

.weak PrefetchHandler
PrefetchHandler:

.weak AbortHandler
AbortHandler:

.weak FiqHandler
FiqHandler:

.global _unhandled_exception
_unhandled_exception:
        b       _unhandled_exception
