# How to add Interrupt Process

### Define IRQ number
IRQ number are defined at `irq.h`  
Define IRQ number  
ex) `os/arch/arm/include/XXX/irq.h`
```
/* Processor Exceptions (vectors 0-15) */

#define XXX_IRQ_RESERVED    (0)     /* Reserved vector (only used with CONFIG_DEBUG_FEATURES) */
/* Vector  0: Reset stack pointer value */
/* Vector  1: Reset (not handler as an IRQ) */
#define XXX_IRQ_NMI         (2)     /* Vector  2: Non-Maskable Interrupt (NMI) */
...
#define XXX_IRQ_SYSTICK     (15)    /* Vector 15: System tick */

/* External interrupts (vectors >= 16) */

#define XXX_IRQ_EXTINT      (16)    /* Vector number of the first external interrupt */
#define XXX_IRQ_EXTINTX     (X)     /* external interrupt X */
...
```

### Add Exception Vector table
Exception Vector table are defined at `vectors.S`  
Add and route interrupt  
ex) `os/arch/arm/XXX/XXX_vectors_S`

##### Add Exception Vector
```
_vectors:

/* Processor Exceptions */

	.word	IDLE_STACK          /* Vector  0: Reset stack pointer */
	.word	__start             /* Vector  1: Reset vector */
	...
	.word	XXX_systick         /* Vector 15: System tick */

/* External Interrupts */

    .word	XXX_extint_xxx      /* Vector x: define chip ext-interrupt route here */
	...
```

##### Route IRQ Handler
```
handlers:
	HANDLER	XXX_reserved, XXX_IRQ_RESERVED  /* Unexpected/reserved vector */
	...
	HANDLER	XXX_systick, XXX_IRQ_SYSTICK    /* Vector 15: System tick */	
	HANDLER	XXX_extint_xxx, XXX_IRQ_EXTINX  /* Vector: Ext-IRQx handler */
	...
```

### Implement ISR and Attach IRQ
Implement and attach real interrupt process function as you need  
ex)  
`os/arch/arm/src/XXX/XXX_irq.c`  
`os/arch/arm/src/XXX/XXX_timerisr.c`  
`...`

##### Implement ISR
```
int up_extintisr(int irq, uint32_t *regs)
{
	/* do isr */
}
```

##### Attach IRQ
```
(void)irq_attach(XXX_IRQ_EXTINX, (xcpt_t)up_extintisr, NULL);
```

