
D:/sdk-RTL8730E_v8.2b_beta/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


Disassembly of section .ram_image1.text:

30003050 <FLASH_ClockDiv>:
30003050:	2200      	movs	r2, #0
30003052:	2180      	movs	r1, #128	; 0x80
30003054:	b538      	push	{r3, r4, r5, lr}
30003056:	4605      	mov	r5, r0
30003058:	4c1e      	ldr	r4, [pc, #120]	; (300030d4 <FLASH_ClockDiv+0x84>)
3000305a:	4610      	mov	r0, r2
3000305c:	47a0      	blx	r4
3000305e:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30003062:	022d      	lsls	r5, r5, #8
30003064:	230a      	movs	r3, #10
30003066:	6c11      	ldr	r1, [r2, #64]	; 0x40
30003068:	f405 6570 	and.w	r5, r5, #3840	; 0xf00
3000306c:	f421 5140 	bic.w	r1, r1, #12288	; 0x3000
30003070:	6411      	str	r1, [r2, #64]	; 0x40
30003072:	6c11      	ldr	r1, [r2, #64]	; 0x40
30003074:	f421 6170 	bic.w	r1, r1, #3840	; 0xf00
30003078:	430d      	orrs	r5, r1
3000307a:	4917      	ldr	r1, [pc, #92]	; (300030d8 <FLASH_ClockDiv+0x88>)
3000307c:	6415      	str	r5, [r2, #64]	; 0x40
3000307e:	e001      	b.n	30003084 <FLASH_ClockDiv+0x34>
30003080:	2a00      	cmp	r2, #0
30003082:	db02      	blt.n	3000308a <FLASH_ClockDiv+0x3a>
30003084:	3b01      	subs	r3, #1
30003086:	6c0a      	ldr	r2, [r1, #64]	; 0x40
30003088:	d1fa      	bne.n	30003080 <FLASH_ClockDiv+0x30>
3000308a:	230a      	movs	r3, #10
3000308c:	4912      	ldr	r1, [pc, #72]	; (300030d8 <FLASH_ClockDiv+0x88>)
3000308e:	e000      	b.n	30003092 <FLASH_ClockDiv+0x42>
30003090:	b11b      	cbz	r3, 3000309a <FLASH_ClockDiv+0x4a>
30003092:	6c0a      	ldr	r2, [r1, #64]	; 0x40
30003094:	3b01      	subs	r3, #1
30003096:	0092      	lsls	r2, r2, #2
30003098:	d5fa      	bpl.n	30003090 <FLASH_ClockDiv+0x40>
3000309a:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
3000309e:	220a      	movs	r2, #10
300030a0:	6c0b      	ldr	r3, [r1, #64]	; 0x40
300030a2:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
300030a6:	640b      	str	r3, [r1, #64]	; 0x40
300030a8:	6c0b      	ldr	r3, [r1, #64]	; 0x40
300030aa:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300030ae:	640b      	str	r3, [r1, #64]	; 0x40
300030b0:	e000      	b.n	300030b4 <FLASH_ClockDiv+0x64>
300030b2:	b12a      	cbz	r2, 300030c0 <FLASH_ClockDiv+0x70>
300030b4:	6c0b      	ldr	r3, [r1, #64]	; 0x40
300030b6:	3a01      	subs	r2, #1
300030b8:	f003 0303 	and.w	r3, r3, #3
300030bc:	2b03      	cmp	r3, #3
300030be:	d1f8      	bne.n	300030b2 <FLASH_ClockDiv+0x62>
300030c0:	2201      	movs	r2, #1
300030c2:	2180      	movs	r1, #128	; 0x80
300030c4:	f44f 7080 	mov.w	r0, #256	; 0x100
300030c8:	47a0      	blx	r4
300030ca:	2003      	movs	r0, #3
300030cc:	4b03      	ldr	r3, [pc, #12]	; (300030dc <FLASH_ClockDiv+0x8c>)
300030ce:	4798      	blx	r3
300030d0:	2001      	movs	r0, #1
300030d2:	bd38      	pop	{r3, r4, r5, pc}
300030d4:	0000b479 	.word	0x0000b479
300030d8:	42008800 	.word	0x42008800
300030dc:	0000b569 	.word	0x0000b569

300030e0 <FLASH_CalibrationPhaseIdx>:
300030e0:	2200      	movs	r2, #0
300030e2:	2180      	movs	r1, #128	; 0x80
300030e4:	b538      	push	{r3, r4, r5, lr}
300030e6:	4604      	mov	r4, r0
300030e8:	4d1a      	ldr	r5, [pc, #104]	; (30003154 <FLASH_CalibrationPhaseIdx+0x74>)
300030ea:	4610      	mov	r0, r2
300030ec:	47a8      	blx	r5
300030ee:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
300030f2:	230a      	movs	r3, #10
300030f4:	6c0a      	ldr	r2, [r1, #64]	; 0x40
300030f6:	f422 5280 	bic.w	r2, r2, #4096	; 0x1000
300030fa:	640a      	str	r2, [r1, #64]	; 0x40
300030fc:	6c0a      	ldr	r2, [r1, #64]	; 0x40
300030fe:	f422 5200 	bic.w	r2, r2, #8192	; 0x2000
30003102:	640a      	str	r2, [r1, #64]	; 0x40
30003104:	e000      	b.n	30003108 <FLASH_CalibrationPhaseIdx+0x28>
30003106:	b11b      	cbz	r3, 30003110 <FLASH_CalibrationPhaseIdx+0x30>
30003108:	6c0a      	ldr	r2, [r1, #64]	; 0x40
3000310a:	3b01      	subs	r3, #1
3000310c:	0792      	lsls	r2, r2, #30
3000310e:	d1fa      	bne.n	30003106 <FLASH_CalibrationPhaseIdx+0x26>
30003110:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30003114:	00a4      	lsls	r4, r4, #2
30003116:	220a      	movs	r2, #10
30003118:	6c0b      	ldr	r3, [r1, #64]	; 0x40
3000311a:	f004 047c 	and.w	r4, r4, #124	; 0x7c
3000311e:	f023 037c 	bic.w	r3, r3, #124	; 0x7c
30003122:	431c      	orrs	r4, r3
30003124:	640c      	str	r4, [r1, #64]	; 0x40
30003126:	6c0b      	ldr	r3, [r1, #64]	; 0x40
30003128:	f443 5300 	orr.w	r3, r3, #8192	; 0x2000
3000312c:	640b      	str	r3, [r1, #64]	; 0x40
3000312e:	6c0b      	ldr	r3, [r1, #64]	; 0x40
30003130:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30003134:	640b      	str	r3, [r1, #64]	; 0x40
30003136:	e000      	b.n	3000313a <FLASH_CalibrationPhaseIdx+0x5a>
30003138:	b12a      	cbz	r2, 30003146 <FLASH_CalibrationPhaseIdx+0x66>
3000313a:	6c0b      	ldr	r3, [r1, #64]	; 0x40
3000313c:	3a01      	subs	r2, #1
3000313e:	f003 0303 	and.w	r3, r3, #3
30003142:	2b03      	cmp	r3, #3
30003144:	d1f8      	bne.n	30003138 <FLASH_CalibrationPhaseIdx+0x58>
30003146:	2000      	movs	r0, #0
30003148:	2201      	movs	r2, #1
3000314a:	2180      	movs	r1, #128	; 0x80
3000314c:	47a8      	blx	r5
3000314e:	2001      	movs	r0, #1
30003150:	bd38      	pop	{r3, r4, r5, pc}
30003152:	bf00      	nop
30003154:	0000b479 	.word	0x0000b479

30003158 <FLASH_CalibrationNewCmd>:
30003158:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000315c:	2801      	cmp	r0, #1
3000315e:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30003162:	f04f 0001 	mov.w	r0, #1
30003166:	6c1b      	ldr	r3, [r3, #64]	; 0x40
30003168:	bf0c      	ite	eq
3000316a:	f443 4300 	orreq.w	r3, r3, #32768	; 0x8000
3000316e:	f423 4300 	bicne.w	r3, r3, #32768	; 0x8000
30003172:	6413      	str	r3, [r2, #64]	; 0x40
30003174:	4770      	bx	lr
30003176:	bf00      	nop

30003178 <FLASH_CalibrationNew>:
30003178:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000317c:	b087      	sub	sp, #28
3000317e:	0052      	lsls	r2, r2, #1
30003180:	4699      	mov	r9, r3
30003182:	4682      	mov	sl, r0
30003184:	f89d 3040 	ldrb.w	r3, [sp, #64]	; 0x40
30003188:	3201      	adds	r2, #1
3000318a:	9101      	str	r1, [sp, #4]
3000318c:	9303      	str	r3, [sp, #12]
3000318e:	fa5f f882 	uxtb.w	r8, r2
30003192:	4b76      	ldr	r3, [pc, #472]	; (3000336c <FLASH_CalibrationNew+0x1f4>)
30003194:	f89d 4044 	ldrb.w	r4, [sp, #68]	; 0x44
30003198:	4798      	blx	r3
3000319a:	9901      	ldr	r1, [sp, #4]
3000319c:	2800      	cmp	r0, #0
3000319e:	f000 80a7 	beq.w	300032f0 <FLASH_CalibrationNew+0x178>
300031a2:	4873      	ldr	r0, [pc, #460]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300031a4:	4b73      	ldr	r3, [pc, #460]	; (30003374 <FLASH_CalibrationNew+0x1fc>)
300031a6:	4798      	blx	r3
300031a8:	4971      	ldr	r1, [pc, #452]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300031aa:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
300031ae:	9b03      	ldr	r3, [sp, #12]
300031b0:	45a0      	cmp	r8, r4
300031b2:	7949      	ldrb	r1, [r1, #5]
300031b4:	ea4f 3303 	mov.w	r3, r3, lsl #12
300031b8:	6151      	str	r1, [r2, #20]
300031ba:	f8d2 111c 	ldr.w	r1, [r2, #284]	; 0x11c
300031be:	b29b      	uxth	r3, r3
300031c0:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
300031c4:	ea43 0301 	orr.w	r3, r3, r1
300031c8:	f8c2 311c 	str.w	r3, [r2, #284]	; 0x11c
300031cc:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300031d0:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300031d2:	f422 4200 	bic.w	r2, r2, #32768	; 0x8000
300031d6:	641a      	str	r2, [r3, #64]	; 0x40
300031d8:	6c1a      	ldr	r2, [r3, #64]	; 0x40
300031da:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300031de:	641a      	str	r2, [r3, #64]	; 0x40
300031e0:	f0c0 80a4 	bcc.w	3000332c <FLASH_CalibrationNew+0x1b4>
300031e4:	f04f 33ff 	mov.w	r3, #4294967295
300031e8:	2700      	movs	r7, #0
300031ea:	f8df b1b4 	ldr.w	fp, [pc, #436]	; 300033a0 <FLASH_CalibrationNew+0x228>
300031ee:	f04f 6500 	mov.w	r5, #134217728	; 0x8000000
300031f2:	4e61      	ldr	r6, [pc, #388]	; (30003378 <FLASH_CalibrationNew+0x200>)
300031f4:	9302      	str	r3, [sp, #8]
300031f6:	9301      	str	r3, [sp, #4]
300031f8:	e008      	b.n	3000320c <FLASH_CalibrationNew+0x94>
300031fa:	9b01      	ldr	r3, [sp, #4]
300031fc:	9402      	str	r4, [sp, #8]
300031fe:	3301      	adds	r3, #1
30003200:	d05f      	beq.n	300032c2 <FLASH_CalibrationNew+0x14a>
30003202:	444f      	add	r7, r9
30003204:	444c      	add	r4, r9
30003206:	b2e4      	uxtb	r4, r4
30003208:	45a0      	cmp	r8, r4
3000320a:	d360      	bcc.n	300032ce <FLASH_CalibrationNew+0x156>
3000320c:	4620      	mov	r0, r4
3000320e:	f7ff ff67 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
30003212:	6973      	ldr	r3, [r6, #20]
30003214:	03db      	lsls	r3, r3, #15
30003216:	d507      	bpl.n	30003228 <FLASH_CalibrationNew+0xb0>
30003218:	f3bf 8f4f 	dsb	sy
3000321c:	f8c6 525c 	str.w	r5, [r6, #604]	; 0x25c
30003220:	f3bf 8f4f 	dsb	sy
30003224:	f3bf 8f6f 	isb	sy
30003228:	47d8      	blx	fp
3000322a:	6829      	ldr	r1, [r5, #0]
3000322c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003230:	9104      	str	r1, [sp, #16]
30003232:	686a      	ldr	r2, [r5, #4]
30003234:	9205      	str	r2, [sp, #20]
30003236:	b11b      	cbz	r3, 30003240 <FLASH_CalibrationNew+0xc8>
30003238:	4b50      	ldr	r3, [pc, #320]	; (3000337c <FLASH_CalibrationNew+0x204>)
3000323a:	681b      	ldr	r3, [r3, #0]
3000323c:	0398      	lsls	r0, r3, #14
3000323e:	d452      	bmi.n	300032e6 <FLASH_CalibrationNew+0x16e>
30003240:	2208      	movs	r2, #8
30003242:	494f      	ldr	r1, [pc, #316]	; (30003380 <FLASH_CalibrationNew+0x208>)
30003244:	a804      	add	r0, sp, #16
30003246:	4b4f      	ldr	r3, [pc, #316]	; (30003384 <FLASH_CalibrationNew+0x20c>)
30003248:	4798      	blx	r3
3000324a:	2800      	cmp	r0, #0
3000324c:	d0d5      	beq.n	300031fa <FLASH_CalibrationNew+0x82>
3000324e:	2f00      	cmp	r7, #0
30003250:	ddd8      	ble.n	30003204 <FLASH_CalibrationNew+0x8c>
30003252:	9b02      	ldr	r3, [sp, #8]
30003254:	9a01      	ldr	r2, [sp, #4]
30003256:	061c      	lsls	r4, r3, #24
30003258:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
3000325c:	ea44 4402 	orr.w	r4, r4, r2, lsl #16
30003260:	433c      	orrs	r4, r7
30003262:	2b00      	cmp	r3, #0
30003264:	d05d      	beq.n	30003322 <FLASH_CalibrationNew+0x1aa>
30003266:	4d45      	ldr	r5, [pc, #276]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003268:	682a      	ldr	r2, [r5, #0]
3000326a:	0390      	lsls	r0, r2, #14
3000326c:	d508      	bpl.n	30003280 <FLASH_CalibrationNew+0x108>
3000326e:	9903      	ldr	r1, [sp, #12]
30003270:	4845      	ldr	r0, [pc, #276]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003272:	f009 fa4d 	bl	3000c710 <__DiagPrintf_veneer>
30003276:	682b      	ldr	r3, [r5, #0]
30003278:	039b      	lsls	r3, r3, #14
3000327a:	d46c      	bmi.n	30003356 <FLASH_CalibrationNew+0x1de>
3000327c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003280:	9a01      	ldr	r2, [sp, #4]
30003282:	eb02 0167 	add.w	r1, r2, r7, asr #1
30003286:	b2c9      	uxtb	r1, r1
30003288:	f88a 1061 	strb.w	r1, [sl, #97]	; 0x61
3000328c:	b11b      	cbz	r3, 30003296 <FLASH_CalibrationNew+0x11e>
3000328e:	4b3b      	ldr	r3, [pc, #236]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003290:	681b      	ldr	r3, [r3, #0]
30003292:	039d      	lsls	r5, r3, #14
30003294:	d465      	bmi.n	30003362 <FLASH_CalibrationNew+0x1ea>
30003296:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000329a:	4d35      	ldr	r5, [pc, #212]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
3000329c:	6c13      	ldr	r3, [r2, #64]	; 0x40
3000329e:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300032a2:	6413      	str	r3, [r2, #64]	; 0x40
300032a4:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300032a8:	79aa      	ldrb	r2, [r5, #6]
300032aa:	615a      	str	r2, [r3, #20]
300032ac:	4b2f      	ldr	r3, [pc, #188]	; (3000336c <FLASH_CalibrationNew+0x1f4>)
300032ae:	4798      	blx	r3
300032b0:	b310      	cbz	r0, 300032f8 <FLASH_CalibrationNew+0x180>
300032b2:	4628      	mov	r0, r5
300032b4:	7929      	ldrb	r1, [r5, #4]
300032b6:	4b2f      	ldr	r3, [pc, #188]	; (30003374 <FLASH_CalibrationNew+0x1fc>)
300032b8:	4798      	blx	r3
300032ba:	4620      	mov	r0, r4
300032bc:	b007      	add	sp, #28
300032be:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300032c2:	9401      	str	r4, [sp, #4]
300032c4:	444c      	add	r4, r9
300032c6:	2701      	movs	r7, #1
300032c8:	b2e4      	uxtb	r4, r4
300032ca:	45a0      	cmp	r8, r4
300032cc:	d29e      	bcs.n	3000320c <FLASH_CalibrationNew+0x94>
300032ce:	9b02      	ldr	r3, [sp, #8]
300032d0:	061c      	lsls	r4, r3, #24
300032d2:	9b01      	ldr	r3, [sp, #4]
300032d4:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
300032d8:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
300032dc:	433c      	orrs	r4, r7
300032de:	b9a3      	cbnz	r3, 3000330a <FLASH_CalibrationNew+0x192>
300032e0:	2f00      	cmp	r7, #0
300032e2:	ddd8      	ble.n	30003296 <FLASH_CalibrationNew+0x11e>
300032e4:	e7ca      	b.n	3000327c <FLASH_CalibrationNew+0x104>
300032e6:	4623      	mov	r3, r4
300032e8:	4828      	ldr	r0, [pc, #160]	; (3000338c <FLASH_CalibrationNew+0x214>)
300032ea:	f009 fa11 	bl	3000c710 <__DiagPrintf_veneer>
300032ee:	e7a7      	b.n	30003240 <FLASH_CalibrationNew+0xc8>
300032f0:	481f      	ldr	r0, [pc, #124]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300032f2:	4b27      	ldr	r3, [pc, #156]	; (30003390 <FLASH_CalibrationNew+0x218>)
300032f4:	4798      	blx	r3
300032f6:	e757      	b.n	300031a8 <FLASH_CalibrationNew+0x30>
300032f8:	4b1d      	ldr	r3, [pc, #116]	; (30003370 <FLASH_CalibrationNew+0x1f8>)
300032fa:	7919      	ldrb	r1, [r3, #4]
300032fc:	4618      	mov	r0, r3
300032fe:	4b24      	ldr	r3, [pc, #144]	; (30003390 <FLASH_CalibrationNew+0x218>)
30003300:	4798      	blx	r3
30003302:	4620      	mov	r0, r4
30003304:	b007      	add	sp, #28
30003306:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000330a:	4d1c      	ldr	r5, [pc, #112]	; (3000337c <FLASH_CalibrationNew+0x204>)
3000330c:	682b      	ldr	r3, [r5, #0]
3000330e:	039b      	lsls	r3, r3, #14
30003310:	d5e6      	bpl.n	300032e0 <FLASH_CalibrationNew+0x168>
30003312:	9903      	ldr	r1, [sp, #12]
30003314:	481c      	ldr	r0, [pc, #112]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003316:	f009 f9fb 	bl	3000c710 <__DiagPrintf_veneer>
3000331a:	682b      	ldr	r3, [r5, #0]
3000331c:	039e      	lsls	r6, r3, #14
3000331e:	d41a      	bmi.n	30003356 <FLASH_CalibrationNew+0x1de>
30003320:	e7de      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003322:	eb02 0767 	add.w	r7, r2, r7, asr #1
30003326:	f88a 7061 	strb.w	r7, [sl, #97]	; 0x61
3000332a:	e7b4      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000332c:	f89a 3060 	ldrb.w	r3, [sl, #96]	; 0x60
30003330:	b90b      	cbnz	r3, 30003336 <FLASH_CalibrationNew+0x1be>
30003332:	4c18      	ldr	r4, [pc, #96]	; (30003394 <FLASH_CalibrationNew+0x21c>)
30003334:	e7af      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
30003336:	4c11      	ldr	r4, [pc, #68]	; (3000337c <FLASH_CalibrationNew+0x204>)
30003338:	6823      	ldr	r3, [r4, #0]
3000333a:	0399      	lsls	r1, r3, #14
3000333c:	d5f9      	bpl.n	30003332 <FLASH_CalibrationNew+0x1ba>
3000333e:	9903      	ldr	r1, [sp, #12]
30003340:	4811      	ldr	r0, [pc, #68]	; (30003388 <FLASH_CalibrationNew+0x210>)
30003342:	f009 f9e5 	bl	3000c710 <__DiagPrintf_veneer>
30003346:	6823      	ldr	r3, [r4, #0]
30003348:	039a      	lsls	r2, r3, #14
3000334a:	d5f2      	bpl.n	30003332 <FLASH_CalibrationNew+0x1ba>
3000334c:	f04f 33ff 	mov.w	r3, #4294967295
30003350:	2700      	movs	r7, #0
30003352:	4c10      	ldr	r4, [pc, #64]	; (30003394 <FLASH_CalibrationNew+0x21c>)
30003354:	9301      	str	r3, [sp, #4]
30003356:	463a      	mov	r2, r7
30003358:	9901      	ldr	r1, [sp, #4]
3000335a:	480f      	ldr	r0, [pc, #60]	; (30003398 <FLASH_CalibrationNew+0x220>)
3000335c:	f009 f9d8 	bl	3000c710 <__DiagPrintf_veneer>
30003360:	e7be      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003362:	480e      	ldr	r0, [pc, #56]	; (3000339c <FLASH_CalibrationNew+0x224>)
30003364:	f009 f9d4 	bl	3000c710 <__DiagPrintf_veneer>
30003368:	e795      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000336a:	bf00      	nop
3000336c:	3000a219 	.word	0x3000a219
30003370:	2001c01c 	.word	0x2001c01c
30003374:	00009c99 	.word	0x00009c99
30003378:	e000ed00 	.word	0xe000ed00
3000337c:	2001c00c 	.word	0x2001c00c
30003380:	00054060 	.word	0x00054060
30003384:	000129bd 	.word	0x000129bd
30003388:	3000c790 	.word	0x3000c790
3000338c:	3000c764 	.word	0x3000c764
30003390:	0000b7e9 	.word	0x0000b7e9
30003394:	ffff0000 	.word	0xffff0000
30003398:	3000c7b4 	.word	0x3000c7b4
3000339c:	3000c7f0 	.word	0x3000c7f0
300033a0:	3000a021 	.word	0x3000a021

300033a4 <FLASH_Calibration>:
300033a4:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300033a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300033ac:	695c      	ldr	r4, [r3, #20]
300033ae:	b087      	sub	sp, #28
300033b0:	4616      	mov	r6, r2
300033b2:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300033b6:	9403      	str	r4, [sp, #12]
300033b8:	4607      	mov	r7, r0
300033ba:	f8d3 311c 	ldr.w	r3, [r3, #284]	; 0x11c
300033be:	9100      	str	r1, [sp, #0]
300033c0:	9304      	str	r3, [sp, #16]
300033c2:	7943      	ldrb	r3, [r0, #5]
300033c4:	9302      	str	r3, [sp, #8]
300033c6:	6c13      	ldr	r3, [r2, #64]	; 0x40
300033c8:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300033cc:	6413      	str	r3, [r2, #64]	; 0x40
300033ce:	4b44      	ldr	r3, [pc, #272]	; (300034e0 <FLASH_Calibration+0x13c>)
300033d0:	4798      	blx	r3
300033d2:	9900      	ldr	r1, [sp, #0]
300033d4:	2800      	cmp	r0, #0
300033d6:	d067      	beq.n	300034a8 <FLASH_Calibration+0x104>
300033d8:	4842      	ldr	r0, [pc, #264]	; (300034e4 <FLASH_Calibration+0x140>)
300033da:	4b43      	ldr	r3, [pc, #268]	; (300034e8 <FLASH_Calibration+0x144>)
300033dc:	4798      	blx	r3
300033de:	9a02      	ldr	r2, [sp, #8]
300033e0:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300033e4:	eb16 0642 	adds.w	r6, r6, r2, lsl #1
300033e8:	615a      	str	r2, [r3, #20]
300033ea:	d077      	beq.n	300034dc <FLASH_Calibration+0x138>
300033ec:	f04f 0800 	mov.w	r8, #0
300033f0:	f04f 33ff 	mov.w	r3, #4294967295
300033f4:	f8df 910c 	ldr.w	r9, [pc, #268]	; 30003504 <FLASH_Calibration+0x160>
300033f8:	f04f 6400 	mov.w	r4, #134217728	; 0x8000000
300033fc:	46c2      	mov	sl, r8
300033fe:	4d3b      	ldr	r5, [pc, #236]	; (300034ec <FLASH_Calibration+0x148>)
30003400:	f8df b0f4 	ldr.w	fp, [pc, #244]	; 300034f8 <FLASH_Calibration+0x154>
30003404:	9301      	str	r3, [sp, #4]
30003406:	f8cd 8000 	str.w	r8, [sp]
3000340a:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000340e:	ea4f 330a 	mov.w	r3, sl, lsl #12
30003412:	f8d0 111c 	ldr.w	r1, [r0, #284]	; 0x11c
30003416:	b29b      	uxth	r3, r3
30003418:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
3000341c:	430b      	orrs	r3, r1
3000341e:	f8c0 311c 	str.w	r3, [r0, #284]	; 0x11c
30003422:	696b      	ldr	r3, [r5, #20]
30003424:	03d9      	lsls	r1, r3, #15
30003426:	d507      	bpl.n	30003438 <FLASH_Calibration+0x94>
30003428:	f3bf 8f4f 	dsb	sy
3000342c:	f8c5 425c 	str.w	r4, [r5, #604]	; 0x25c
30003430:	f3bf 8f4f 	dsb	sy
30003434:	f3bf 8f6f 	isb	sy
30003438:	4b2d      	ldr	r3, [pc, #180]	; (300034f0 <FLASH_Calibration+0x14c>)
3000343a:	4798      	blx	r3
3000343c:	f897 3060 	ldrb.w	r3, [r7, #96]	; 0x60
30003440:	6821      	ldr	r1, [r4, #0]
30003442:	b11b      	cbz	r3, 3000344c <FLASH_Calibration+0xa8>
30003444:	f8db 3000 	ldr.w	r3, [fp]
30003448:	039a      	lsls	r2, r3, #14
3000344a:	d43f      	bmi.n	300034cc <FLASH_Calibration+0x128>
3000344c:	f8d9 3000 	ldr.w	r3, [r9]
30003450:	428b      	cmp	r3, r1
30003452:	d109      	bne.n	30003468 <FLASH_Calibration+0xc4>
30003454:	9b01      	ldr	r3, [sp, #4]
30003456:	f108 0801 	add.w	r8, r8, #1
3000345a:	3301      	adds	r3, #1
3000345c:	f04f 0301 	mov.w	r3, #1
30003460:	bf08      	it	eq
30003462:	f8cd a004 	streq.w	sl, [sp, #4]
30003466:	9300      	str	r3, [sp, #0]
30003468:	f10a 0a01 	add.w	sl, sl, #1
3000346c:	4556      	cmp	r6, sl
3000346e:	d1cc      	bne.n	3000340a <FLASH_Calibration+0x66>
30003470:	f1b8 0f00 	cmp.w	r8, #0
30003474:	dd06      	ble.n	30003484 <FLASH_Calibration+0xe0>
30003476:	9b01      	ldr	r3, [sp, #4]
30003478:	eb03 0168 	add.w	r1, r3, r8, asr #1
3000347c:	f897 3060 	ldrb.w	r3, [r7, #96]	; 0x60
30003480:	7779      	strb	r1, [r7, #29]
30003482:	b9d3      	cbnz	r3, 300034ba <FLASH_Calibration+0x116>
30003484:	4b16      	ldr	r3, [pc, #88]	; (300034e0 <FLASH_Calibration+0x13c>)
30003486:	4798      	blx	r3
30003488:	b190      	cbz	r0, 300034b0 <FLASH_Calibration+0x10c>
3000348a:	4816      	ldr	r0, [pc, #88]	; (300034e4 <FLASH_Calibration+0x140>)
3000348c:	4b16      	ldr	r3, [pc, #88]	; (300034e8 <FLASH_Calibration+0x144>)
3000348e:	7901      	ldrb	r1, [r0, #4]
30003490:	4798      	blx	r3
30003492:	9a03      	ldr	r2, [sp, #12]
30003494:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003498:	9800      	ldr	r0, [sp, #0]
3000349a:	615a      	str	r2, [r3, #20]
3000349c:	9a04      	ldr	r2, [sp, #16]
3000349e:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300034a2:	b007      	add	sp, #28
300034a4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034a8:	480e      	ldr	r0, [pc, #56]	; (300034e4 <FLASH_Calibration+0x140>)
300034aa:	4b12      	ldr	r3, [pc, #72]	; (300034f4 <FLASH_Calibration+0x150>)
300034ac:	4798      	blx	r3
300034ae:	e796      	b.n	300033de <FLASH_Calibration+0x3a>
300034b0:	480c      	ldr	r0, [pc, #48]	; (300034e4 <FLASH_Calibration+0x140>)
300034b2:	4b10      	ldr	r3, [pc, #64]	; (300034f4 <FLASH_Calibration+0x150>)
300034b4:	7901      	ldrb	r1, [r0, #4]
300034b6:	4798      	blx	r3
300034b8:	e7eb      	b.n	30003492 <FLASH_Calibration+0xee>
300034ba:	4b0f      	ldr	r3, [pc, #60]	; (300034f8 <FLASH_Calibration+0x154>)
300034bc:	681b      	ldr	r3, [r3, #0]
300034be:	039b      	lsls	r3, r3, #14
300034c0:	d5e0      	bpl.n	30003484 <FLASH_Calibration+0xe0>
300034c2:	9a02      	ldr	r2, [sp, #8]
300034c4:	480d      	ldr	r0, [pc, #52]	; (300034fc <FLASH_Calibration+0x158>)
300034c6:	f009 f923 	bl	3000c710 <__DiagPrintf_veneer>
300034ca:	e7db      	b.n	30003484 <FLASH_Calibration+0xe0>
300034cc:	4652      	mov	r2, sl
300034ce:	9b02      	ldr	r3, [sp, #8]
300034d0:	480b      	ldr	r0, [pc, #44]	; (30003500 <FLASH_Calibration+0x15c>)
300034d2:	9105      	str	r1, [sp, #20]
300034d4:	f009 f91c 	bl	3000c710 <__DiagPrintf_veneer>
300034d8:	9905      	ldr	r1, [sp, #20]
300034da:	e7b7      	b.n	3000344c <FLASH_Calibration+0xa8>
300034dc:	9600      	str	r6, [sp, #0]
300034de:	e7d1      	b.n	30003484 <FLASH_Calibration+0xe0>
300034e0:	3000a219 	.word	0x3000a219
300034e4:	2001c01c 	.word	0x2001c01c
300034e8:	00009c99 	.word	0x00009c99
300034ec:	e000ed00 	.word	0xe000ed00
300034f0:	3000a021 	.word	0x3000a021
300034f4:	0000b7e9 	.word	0x0000b7e9
300034f8:	2001c00c 	.word	0x2001c00c
300034fc:	3000c84c 	.word	0x3000c84c
30003500:	3000c81c 	.word	0x3000c81c
30003504:	00054060 	.word	0x00054060

30003508 <_flash_calibration_highspeed>:
30003508:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000350c:	2400      	movs	r4, #0
3000350e:	b085      	sub	sp, #20
30003510:	4d24      	ldr	r5, [pc, #144]	; (300035a4 <_flash_calibration_highspeed+0x9c>)
30003512:	f04f 0801 	mov.w	r8, #1
30003516:	46a3      	mov	fp, r4
30003518:	4626      	mov	r6, r4
3000351a:	46a1      	mov	r9, r4
3000351c:	460a      	mov	r2, r1
3000351e:	4627      	mov	r7, r4
30003520:	4682      	mov	sl, r0
30003522:	b2e0      	uxtb	r0, r4
30003524:	2301      	movs	r3, #1
30003526:	4651      	mov	r1, sl
30003528:	9203      	str	r2, [sp, #12]
3000352a:	e9cd 0800 	strd	r0, r8, [sp]
3000352e:	4628      	mov	r0, r5
30003530:	f7ff fe22 	bl	30003178 <FLASH_CalibrationNew>
30003534:	f895 3061 	ldrb.w	r3, [r5, #97]	; 0x61
30003538:	b2c0      	uxtb	r0, r0
3000353a:	9a03      	ldr	r2, [sp, #12]
3000353c:	b10b      	cbz	r3, 30003542 <_flash_calibration_highspeed+0x3a>
3000353e:	2802      	cmp	r0, #2
30003540:	d80f      	bhi.n	30003562 <_flash_calibration_highspeed+0x5a>
30003542:	42b0      	cmp	r0, r6
30003544:	d811      	bhi.n	3000356a <_flash_calibration_highspeed+0x62>
30003546:	b106      	cbz	r6, 3000354a <_flash_calibration_highspeed+0x42>
30003548:	b198      	cbz	r0, 30003572 <_flash_calibration_highspeed+0x6a>
3000354a:	3401      	adds	r4, #1
3000354c:	f885 9061 	strb.w	r9, [r5, #97]	; 0x61
30003550:	2c05      	cmp	r4, #5
30003552:	d1e6      	bne.n	30003522 <_flash_calibration_highspeed+0x1a>
30003554:	46ba      	mov	sl, r7
30003556:	b96e      	cbnz	r6, 30003574 <_flash_calibration_highspeed+0x6c>
30003558:	4630      	mov	r0, r6
3000355a:	776e      	strb	r6, [r5, #29]
3000355c:	b005      	add	sp, #20
3000355e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003562:	42b0      	cmp	r0, r6
30003564:	f043 0301 	orr.w	r3, r3, #1
30003568:	d9ef      	bls.n	3000354a <_flash_calibration_highspeed+0x42>
3000356a:	469b      	mov	fp, r3
3000356c:	4627      	mov	r7, r4
3000356e:	4606      	mov	r6, r0
30003570:	e7eb      	b.n	3000354a <_flash_calibration_highspeed+0x42>
30003572:	46ba      	mov	sl, r7
30003574:	4b0c      	ldr	r3, [pc, #48]	; (300035a8 <_flash_calibration_highspeed+0xa0>)
30003576:	689b      	ldr	r3, [r3, #8]
30003578:	079b      	lsls	r3, r3, #30
3000357a:	d40b      	bmi.n	30003594 <_flash_calibration_highspeed+0x8c>
3000357c:	fa5f fa8a 	uxtb.w	sl, sl
30003580:	2001      	movs	r0, #1
30003582:	f885 b061 	strb.w	fp, [r5, #97]	; 0x61
30003586:	f885 a01c 	strb.w	sl, [r5, #28]
3000358a:	f885 a01d 	strb.w	sl, [r5, #29]
3000358e:	b005      	add	sp, #20
30003590:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003594:	465b      	mov	r3, fp
30003596:	4632      	mov	r2, r6
30003598:	4651      	mov	r1, sl
3000359a:	4804      	ldr	r0, [pc, #16]	; (300035ac <_flash_calibration_highspeed+0xa4>)
3000359c:	f009 f8b8 	bl	3000c710 <__DiagPrintf_veneer>
300035a0:	e7ec      	b.n	3000357c <_flash_calibration_highspeed+0x74>
300035a2:	bf00      	nop
300035a4:	2001c01c 	.word	0x2001c01c
300035a8:	2001c00c 	.word	0x2001c00c
300035ac:	3000c878 	.word	0x3000c878

300035b0 <flash_calibration_highspeed>:
300035b0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300035b2:	4d20      	ldr	r5, [pc, #128]	; (30003634 <flash_calibration_highspeed+0x84>)
300035b4:	4604      	mov	r4, r0
300035b6:	2700      	movs	r7, #0
300035b8:	792e      	ldrb	r6, [r5, #4]
300035ba:	f885 7060 	strb.w	r7, [r5, #96]	; 0x60
300035be:	f7ff fd47 	bl	30003050 <FLASH_ClockDiv>
300035c2:	4621      	mov	r1, r4
300035c4:	4630      	mov	r0, r6
300035c6:	f7ff ff9f 	bl	30003508 <_flash_calibration_highspeed>
300035ca:	2801      	cmp	r0, #1
300035cc:	d00e      	beq.n	300035ec <flash_calibration_highspeed+0x3c>
300035ce:	2001      	movs	r0, #1
300035d0:	4b19      	ldr	r3, [pc, #100]	; (30003638 <flash_calibration_highspeed+0x88>)
300035d2:	4798      	blx	r3
300035d4:	4b19      	ldr	r3, [pc, #100]	; (3000363c <flash_calibration_highspeed+0x8c>)
300035d6:	6898      	ldr	r0, [r3, #8]
300035d8:	f010 0002 	ands.w	r0, r0, #2
300035dc:	d100      	bne.n	300035e0 <flash_calibration_highspeed+0x30>
300035de:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300035e0:	4621      	mov	r1, r4
300035e2:	4817      	ldr	r0, [pc, #92]	; (30003640 <flash_calibration_highspeed+0x90>)
300035e4:	f009 f894 	bl	3000c710 <__DiagPrintf_veneer>
300035e8:	4638      	mov	r0, r7
300035ea:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300035ec:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300035f0:	f895 0061 	ldrb.w	r0, [r5, #97]	; 0x61
300035f4:	6c13      	ldr	r3, [r2, #64]	; 0x40
300035f6:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300035fa:	6413      	str	r3, [r2, #64]	; 0x40
300035fc:	f7ff fd70 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
30003600:	4b10      	ldr	r3, [pc, #64]	; (30003644 <flash_calibration_highspeed+0x94>)
30003602:	4798      	blx	r3
30003604:	b940      	cbnz	r0, 30003618 <flash_calibration_highspeed+0x68>
30003606:	4b10      	ldr	r3, [pc, #64]	; (30003648 <flash_calibration_highspeed+0x98>)
30003608:	4630      	mov	r0, r6
3000360a:	4798      	blx	r3
3000360c:	4b0b      	ldr	r3, [pc, #44]	; (3000363c <flash_calibration_highspeed+0x8c>)
3000360e:	689b      	ldr	r3, [r3, #8]
30003610:	079b      	lsls	r3, r3, #30
30003612:	d408      	bmi.n	30003626 <flash_calibration_highspeed+0x76>
30003614:	2001      	movs	r0, #1
30003616:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003618:	4b0c      	ldr	r3, [pc, #48]	; (3000364c <flash_calibration_highspeed+0x9c>)
3000361a:	4630      	mov	r0, r6
3000361c:	4798      	blx	r3
3000361e:	4b07      	ldr	r3, [pc, #28]	; (3000363c <flash_calibration_highspeed+0x8c>)
30003620:	689b      	ldr	r3, [r3, #8]
30003622:	079b      	lsls	r3, r3, #30
30003624:	d5f6      	bpl.n	30003614 <flash_calibration_highspeed+0x64>
30003626:	4621      	mov	r1, r4
30003628:	4809      	ldr	r0, [pc, #36]	; (30003650 <flash_calibration_highspeed+0xa0>)
3000362a:	f009 f871 	bl	3000c710 <__DiagPrintf_veneer>
3000362e:	2001      	movs	r0, #1
30003630:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003632:	bf00      	nop
30003634:	2001c01c 	.word	0x2001c01c
30003638:	0000b569 	.word	0x0000b569
3000363c:	2001c00c 	.word	0x2001c00c
30003640:	3000c8e0 	.word	0x3000c8e0
30003644:	3000a219 	.word	0x3000a219
30003648:	0000c045 	.word	0x0000c045
3000364c:	0000a53d 	.word	0x0000a53d
30003650:	3000c8b0 	.word	0x3000c8b0

30003654 <flash_rx_mode_switch>:
30003654:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003658:	2300      	movs	r3, #0
3000365a:	b089      	sub	sp, #36	; 0x24
3000365c:	4d51      	ldr	r5, [pc, #324]	; (300037a4 <flash_rx_mode_switch+0x150>)
3000365e:	4684      	mov	ip, r0
30003660:	ac03      	add	r4, sp, #12
30003662:	f88d 3003 	strb.w	r3, [sp, #3]
30003666:	f1bc 0f04 	cmp.w	ip, #4
3000366a:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
3000366c:	c40f      	stmia	r4!, {r0, r1, r2, r3}
3000366e:	682b      	ldr	r3, [r5, #0]
30003670:	6023      	str	r3, [r4, #0]
30003672:	d876      	bhi.n	30003762 <flash_rx_mode_switch+0x10e>
30003674:	fa4f f08c 	sxtb.w	r0, ip
30003678:	ab03      	add	r3, sp, #12
3000367a:	4e4b      	ldr	r6, [pc, #300]	; (300037a8 <flash_rx_mode_switch+0x154>)
3000367c:	f1c0 0404 	rsb	r4, r0, #4
30003680:	f1c0 0022 	rsb	r0, r0, #34	; 0x22
30003684:	f8df a148 	ldr.w	sl, [pc, #328]	; 300037d0 <flash_rx_mode_switch+0x17c>
30003688:	b267      	sxtb	r7, r4
3000368a:	fa46 f580 	sxtab	r5, r6, r0
3000368e:	b2e4      	uxtb	r4, r4
30003690:	f8df 9140 	ldr.w	r9, [pc, #320]	; 300037d4 <flash_rx_mode_switch+0x180>
30003694:	eb03 0787 	add.w	r7, r3, r7, lsl #2
30003698:	f8df 813c 	ldr.w	r8, [pc, #316]	; 300037d8 <flash_rx_mode_switch+0x184>
3000369c:	f8df b13c 	ldr.w	fp, [pc, #316]	; 300037dc <flash_rx_mode_switch+0x188>
300036a0:	e035      	b.n	3000370e <flash_rx_mode_switch+0xba>
300036a2:	2c04      	cmp	r4, #4
300036a4:	d03b      	beq.n	3000371e <flash_rx_mode_switch+0xca>
300036a6:	2c02      	cmp	r4, #2
300036a8:	d039      	beq.n	3000371e <flash_rx_mode_switch+0xca>
300036aa:	6833      	ldr	r3, [r6, #0]
300036ac:	2b03      	cmp	r3, #3
300036ae:	d03c      	beq.n	3000372a <flash_rx_mode_switch+0xd6>
300036b0:	2300      	movs	r3, #0
300036b2:	4620      	mov	r0, r4
300036b4:	7773      	strb	r3, [r6, #29]
300036b6:	47d8      	blx	fp
300036b8:	4b3c      	ldr	r3, [pc, #240]	; (300037ac <flash_rx_mode_switch+0x158>)
300036ba:	695a      	ldr	r2, [r3, #20]
300036bc:	03d1      	lsls	r1, r2, #15
300036be:	d509      	bpl.n	300036d4 <flash_rx_mode_switch+0x80>
300036c0:	f3bf 8f4f 	dsb	sy
300036c4:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
300036c8:	f8c3 225c 	str.w	r2, [r3, #604]	; 0x25c
300036cc:	f3bf 8f4f 	dsb	sy
300036d0:	f3bf 8f6f 	isb	sy
300036d4:	47c8      	blx	r9
300036d6:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
300036da:	4935      	ldr	r1, [pc, #212]	; (300037b0 <flash_rx_mode_switch+0x15c>)
300036dc:	a801      	add	r0, sp, #4
300036de:	681a      	ldr	r2, [r3, #0]
300036e0:	9201      	str	r2, [sp, #4]
300036e2:	2208      	movs	r2, #8
300036e4:	685b      	ldr	r3, [r3, #4]
300036e6:	9302      	str	r3, [sp, #8]
300036e8:	47c0      	blx	r8
300036ea:	2800      	cmp	r0, #0
300036ec:	d048      	beq.n	30003780 <flash_rx_mode_switch+0x12c>
300036ee:	f896 3060 	ldrb.w	r3, [r6, #96]	; 0x60
300036f2:	b11b      	cbz	r3, 300036fc <flash_rx_mode_switch+0xa8>
300036f4:	4b2f      	ldr	r3, [pc, #188]	; (300037b4 <flash_rx_mode_switch+0x160>)
300036f6:	689b      	ldr	r3, [r3, #8]
300036f8:	079b      	lsls	r3, r3, #30
300036fa:	d42c      	bmi.n	30003756 <flash_rx_mode_switch+0x102>
300036fc:	1e62      	subs	r2, r4, #1
300036fe:	f1c4 0305 	rsb	r3, r4, #5
30003702:	3f04      	subs	r7, #4
30003704:	3d01      	subs	r5, #1
30003706:	b2d4      	uxtb	r4, r2
30003708:	b2db      	uxtb	r3, r3
3000370a:	2cff      	cmp	r4, #255	; 0xff
3000370c:	d028      	beq.n	30003760 <flash_rx_mode_switch+0x10c>
3000370e:	47d0      	blx	sl
30003710:	2800      	cmp	r0, #0
30003712:	d1c6      	bne.n	300036a2 <flash_rx_mode_switch+0x4e>
30003714:	7770      	strb	r0, [r6, #29]
30003716:	4620      	mov	r0, r4
30003718:	4b27      	ldr	r3, [pc, #156]	; (300037b8 <flash_rx_mode_switch+0x164>)
3000371a:	4798      	blx	r3
3000371c:	e7cc      	b.n	300036b8 <flash_rx_mode_switch+0x64>
3000371e:	2301      	movs	r3, #1
30003720:	f886 3063 	strb.w	r3, [r6, #99]	; 0x63
30003724:	6833      	ldr	r3, [r6, #0]
30003726:	2b03      	cmp	r3, #3
30003728:	d1c2      	bne.n	300036b0 <flash_rx_mode_switch+0x5c>
3000372a:	f10d 0203 	add.w	r2, sp, #3
3000372e:	2101      	movs	r1, #1
30003730:	2085      	movs	r0, #133	; 0x85
30003732:	4b22      	ldr	r3, [pc, #136]	; (300037bc <flash_rx_mode_switch+0x168>)
30003734:	4798      	blx	r3
30003736:	f89d 3003 	ldrb.w	r3, [sp, #3]
3000373a:	7829      	ldrb	r1, [r5, #0]
3000373c:	f10d 0203 	add.w	r2, sp, #3
30003740:	f003 030f 	and.w	r3, r3, #15
30003744:	2081      	movs	r0, #129	; 0x81
30003746:	ea43 1301 	orr.w	r3, r3, r1, lsl #4
3000374a:	2101      	movs	r1, #1
3000374c:	f88d 3003 	strb.w	r3, [sp, #3]
30003750:	4b1b      	ldr	r3, [pc, #108]	; (300037c0 <flash_rx_mode_switch+0x16c>)
30003752:	4798      	blx	r3
30003754:	e7ac      	b.n	300036b0 <flash_rx_mode_switch+0x5c>
30003756:	6839      	ldr	r1, [r7, #0]
30003758:	481a      	ldr	r0, [pc, #104]	; (300037c4 <flash_rx_mode_switch+0x170>)
3000375a:	f008 ffd9 	bl	3000c710 <__DiagPrintf_veneer>
3000375e:	e7cd      	b.n	300036fc <flash_rx_mode_switch+0xa8>
30003760:	469c      	mov	ip, r3
30003762:	f1bc 0f05 	cmp.w	ip, #5
30003766:	d10f      	bne.n	30003788 <flash_rx_mode_switch+0x134>
30003768:	4b12      	ldr	r3, [pc, #72]	; (300037b4 <flash_rx_mode_switch+0x160>)
3000376a:	6818      	ldr	r0, [r3, #0]
3000376c:	f010 0002 	ands.w	r0, r0, #2
30003770:	d00b      	beq.n	3000378a <flash_rx_mode_switch+0x136>
30003772:	4815      	ldr	r0, [pc, #84]	; (300037c8 <flash_rx_mode_switch+0x174>)
30003774:	f008 ffcc 	bl	3000c710 <__DiagPrintf_veneer>
30003778:	2000      	movs	r0, #0
3000377a:	b009      	add	sp, #36	; 0x24
3000377c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003780:	4b0c      	ldr	r3, [pc, #48]	; (300037b4 <flash_rx_mode_switch+0x160>)
30003782:	689b      	ldr	r3, [r3, #8]
30003784:	079a      	lsls	r2, r3, #30
30003786:	d403      	bmi.n	30003790 <flash_rx_mode_switch+0x13c>
30003788:	2001      	movs	r0, #1
3000378a:	b009      	add	sp, #36	; 0x24
3000378c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003790:	ab08      	add	r3, sp, #32
30003792:	480e      	ldr	r0, [pc, #56]	; (300037cc <flash_rx_mode_switch+0x178>)
30003794:	eb03 0484 	add.w	r4, r3, r4, lsl #2
30003798:	f854 1c14 	ldr.w	r1, [r4, #-20]
3000379c:	f008 ffb8 	bl	3000c710 <__DiagPrintf_veneer>
300037a0:	e7f2      	b.n	30003788 <flash_rx_mode_switch+0x134>
300037a2:	bf00      	nop
300037a4:	3000db44 	.word	0x3000db44
300037a8:	2001c01c 	.word	0x2001c01c
300037ac:	e000ed00 	.word	0xe000ed00
300037b0:	00054060 	.word	0x00054060
300037b4:	2001c00c 	.word	0x2001c00c
300037b8:	0000c045 	.word	0x0000c045
300037bc:	00009db5 	.word	0x00009db5
300037c0:	0000a149 	.word	0x0000a149
300037c4:	3000c93c 	.word	0x3000c93c
300037c8:	3000c96c 	.word	0x3000c96c
300037cc:	3000c914 	.word	0x3000c914
300037d0:	3000a219 	.word	0x3000a219
300037d4:	3000a021 	.word	0x3000a021
300037d8:	000129bd 	.word	0x000129bd
300037dc:	0000a53d 	.word	0x0000a53d

300037e0 <flash_highspeed_setup>:
300037e0:	4aaa      	ldr	r2, [pc, #680]	; (30003a8c <flash_highspeed_setup+0x2ac>)
300037e2:	f44f 4300 	mov.w	r3, #32768	; 0x8000
300037e6:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300037ea:	8811      	ldrh	r1, [r2, #0]
300037ec:	b084      	sub	sp, #16
300037ee:	2500      	movs	r5, #0
300037f0:	4219      	tst	r1, r3
300037f2:	f105 0201 	add.w	r2, r5, #1
300037f6:	ea4f 0353 	mov.w	r3, r3, lsr #1
300037fa:	d102      	bne.n	30003802 <flash_highspeed_setup+0x22>
300037fc:	b2d5      	uxtb	r5, r2
300037fe:	2d10      	cmp	r5, #16
30003800:	d1f6      	bne.n	300037f0 <flash_highspeed_setup+0x10>
30003802:	4aa3      	ldr	r2, [pc, #652]	; (30003a90 <flash_highspeed_setup+0x2b0>)
30003804:	2400      	movs	r4, #0
30003806:	f44f 4300 	mov.w	r3, #32768	; 0x8000
3000380a:	8811      	ldrh	r1, [r2, #0]
3000380c:	4219      	tst	r1, r3
3000380e:	f104 0201 	add.w	r2, r4, #1
30003812:	ea4f 0353 	mov.w	r3, r3, lsr #1
30003816:	d102      	bne.n	3000381e <flash_highspeed_setup+0x3e>
30003818:	b2d4      	uxtb	r4, r2
3000381a:	2c10      	cmp	r4, #16
3000381c:	d1f6      	bne.n	3000380c <flash_highspeed_setup+0x2c>
3000381e:	b672      	cpsid	i
30003820:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30003824:	f8df a2b4 	ldr.w	sl, [pc, #692]	; 30003adc <flash_highspeed_setup+0x2fc>
30003828:	4e9a      	ldr	r6, [pc, #616]	; (30003a94 <flash_highspeed_setup+0x2b4>)
3000382a:	6813      	ldr	r3, [r2, #0]
3000382c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30003830:	6013      	str	r3, [r2, #0]
30003832:	47d0      	blx	sl
30003834:	2800      	cmp	r0, #0
30003836:	d03a      	beq.n	300038ae <flash_highspeed_setup+0xce>
30003838:	4f97      	ldr	r7, [pc, #604]	; (30003a98 <flash_highspeed_setup+0x2b8>)
3000383a:	2103      	movs	r1, #3
3000383c:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
30003840:	aa03      	add	r2, sp, #12
30003842:	f8df 829c 	ldr.w	r8, [pc, #668]	; 30003ae0 <flash_highspeed_setup+0x300>
30003846:	47c0      	blx	r8
30003848:	68bb      	ldr	r3, [r7, #8]
3000384a:	0798      	lsls	r0, r3, #30
3000384c:	f100 80f6 	bmi.w	30003a3c <flash_highspeed_setup+0x25c>
30003850:	f89d 900d 	ldrb.w	r9, [sp, #13]
30003854:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003858:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000385c:	ea49 4903 	orr.w	r9, r9, r3, lsl #16
30003860:	f89d 300c 	ldrb.w	r3, [sp, #12]
30003864:	ea49 0903 	orr.w	r9, r9, r3
30003868:	47d0      	blx	sl
3000386a:	4b8c      	ldr	r3, [pc, #560]	; (30003a9c <flash_highspeed_setup+0x2bc>)
3000386c:	4a8c      	ldr	r2, [pc, #560]	; (30003aa0 <flash_highspeed_setup+0x2c0>)
3000386e:	2800      	cmp	r0, #0
30003870:	bf08      	it	eq
30003872:	4613      	moveq	r3, r2
30003874:	e007      	b.n	30003886 <flash_highspeed_setup+0xa6>
30003876:	e9d3 1200 	ldrd	r1, r2, [r3]
3000387a:	4618      	mov	r0, r3
3000387c:	3314      	adds	r3, #20
3000387e:	ea09 0202 	and.w	r2, r9, r2
30003882:	428a      	cmp	r2, r1
30003884:	d075      	beq.n	30003972 <flash_highspeed_setup+0x192>
30003886:	7a1a      	ldrb	r2, [r3, #8]
30003888:	2aff      	cmp	r2, #255	; 0xff
3000388a:	d1f4      	bne.n	30003876 <flash_highspeed_setup+0x96>
3000388c:	f8df 9254 	ldr.w	r9, [pc, #596]	; 30003ae4 <flash_highspeed_setup+0x304>
30003890:	2300      	movs	r3, #0
30003892:	f8c9 3000 	str.w	r3, [r9]
30003896:	683b      	ldr	r3, [r7, #0]
30003898:	0399      	lsls	r1, r3, #14
3000389a:	f100 8175 	bmi.w	30003b88 <flash_highspeed_setup+0x3a8>
3000389e:	4881      	ldr	r0, [pc, #516]	; (30003aa4 <flash_highspeed_setup+0x2c4>)
300038a0:	f240 2127 	movw	r1, #551	; 0x227
300038a4:	f008 ff2c 	bl	3000c700 <__io_assert_failed_veneer>
300038a8:	f8d9 0000 	ldr.w	r0, [r9]
300038ac:	e067      	b.n	3000397e <flash_highspeed_setup+0x19e>
300038ae:	ab03      	add	r3, sp, #12
300038b0:	4602      	mov	r2, r0
300038b2:	4601      	mov	r1, r0
300038b4:	4f7c      	ldr	r7, [pc, #496]	; (30003aa8 <flash_highspeed_setup+0x2c8>)
300038b6:	9300      	str	r3, [sp, #0]
300038b8:	2302      	movs	r3, #2
300038ba:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
300038be:	47b8      	blx	r7
300038c0:	4f75      	ldr	r7, [pc, #468]	; (30003a98 <flash_highspeed_setup+0x2b8>)
300038c2:	68bb      	ldr	r3, [r7, #8]
300038c4:	079a      	lsls	r2, r3, #30
300038c6:	f100 80c3 	bmi.w	30003a50 <flash_highspeed_setup+0x270>
300038ca:	f89d 800d 	ldrb.w	r8, [sp, #13]
300038ce:	f89d 300e 	ldrb.w	r3, [sp, #14]
300038d2:	ea4f 2808 	mov.w	r8, r8, lsl #8
300038d6:	ea48 4803 	orr.w	r8, r8, r3, lsl #16
300038da:	f89d 300c 	ldrb.w	r3, [sp, #12]
300038de:	ea48 0803 	orr.w	r8, r8, r3
300038e2:	47d0      	blx	sl
300038e4:	4b6d      	ldr	r3, [pc, #436]	; (30003a9c <flash_highspeed_setup+0x2bc>)
300038e6:	4a6e      	ldr	r2, [pc, #440]	; (30003aa0 <flash_highspeed_setup+0x2c0>)
300038e8:	2800      	cmp	r0, #0
300038ea:	bf08      	it	eq
300038ec:	4613      	moveq	r3, r2
300038ee:	e008      	b.n	30003902 <flash_highspeed_setup+0x122>
300038f0:	e9d3 1200 	ldrd	r1, r2, [r3]
300038f4:	4618      	mov	r0, r3
300038f6:	3314      	adds	r3, #20
300038f8:	ea08 0202 	and.w	r2, r8, r2
300038fc:	428a      	cmp	r2, r1
300038fe:	f000 8084 	beq.w	30003a0a <flash_highspeed_setup+0x22a>
30003902:	7a1a      	ldrb	r2, [r3, #8]
30003904:	2aff      	cmp	r2, #255	; 0xff
30003906:	d1f3      	bne.n	300038f0 <flash_highspeed_setup+0x110>
30003908:	f8df 91d8 	ldr.w	r9, [pc, #472]	; 30003ae4 <flash_highspeed_setup+0x304>
3000390c:	2300      	movs	r3, #0
3000390e:	f8c9 3000 	str.w	r3, [r9]
30003912:	683b      	ldr	r3, [r7, #0]
30003914:	039b      	lsls	r3, r3, #14
30003916:	f100 813b 	bmi.w	30003b90 <flash_highspeed_setup+0x3b0>
3000391a:	4864      	ldr	r0, [pc, #400]	; (30003aac <flash_highspeed_setup+0x2cc>)
3000391c:	f240 215e 	movw	r1, #606	; 0x25e
30003920:	f008 feee 	bl	3000c700 <__io_assert_failed_veneer>
30003924:	f8d9 0000 	ldr.w	r0, [r9]
30003928:	7a03      	ldrb	r3, [r0, #8]
3000392a:	2b04      	cmp	r3, #4
3000392c:	d977      	bls.n	30003a1e <flash_highspeed_setup+0x23e>
3000392e:	2bfe      	cmp	r3, #254	; 0xfe
30003930:	d104      	bne.n	3000393c <flash_highspeed_setup+0x15c>
30003932:	6903      	ldr	r3, [r0, #16]
30003934:	2b00      	cmp	r3, #0
30003936:	f000 8115 	beq.w	30003b64 <flash_highspeed_setup+0x384>
3000393a:	4798      	blx	r3
3000393c:	68f3      	ldr	r3, [r6, #12]
3000393e:	2b00      	cmp	r3, #0
30003940:	f040 80fa 	bne.w	30003b38 <flash_highspeed_setup+0x358>
30003944:	4e5a      	ldr	r6, [pc, #360]	; (30003ab0 <flash_highspeed_setup+0x2d0>)
30003946:	2100      	movs	r1, #0
30003948:	20a0      	movs	r0, #160	; 0xa0
3000394a:	47b0      	blx	r6
3000394c:	4628      	mov	r0, r5
3000394e:	f7ff fe81 	bl	30003654 <flash_rx_mode_switch>
30003952:	4b58      	ldr	r3, [pc, #352]	; (30003ab4 <flash_highspeed_setup+0x2d4>)
30003954:	4798      	blx	r3
30003956:	2801      	cmp	r0, #1
30003958:	d007      	beq.n	3000396a <flash_highspeed_setup+0x18a>
3000395a:	2c06      	cmp	r4, #6
3000395c:	bf28      	it	cs
3000395e:	2406      	movcs	r4, #6
30003960:	f1c4 0009 	rsb	r0, r4, #9
30003964:	b2c0      	uxtb	r0, r0
30003966:	f7ff fe23 	bl	300035b0 <flash_calibration_highspeed>
3000396a:	b662      	cpsie	i
3000396c:	b004      	add	sp, #16
3000396e:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003972:	f8df 9170 	ldr.w	r9, [pc, #368]	; 30003ae4 <flash_highspeed_setup+0x304>
30003976:	f8c9 0000 	str.w	r0, [r9]
3000397a:	2800      	cmp	r0, #0
3000397c:	d08b      	beq.n	30003896 <flash_highspeed_setup+0xb6>
3000397e:	7a03      	ldrb	r3, [r0, #8]
30003980:	2b05      	cmp	r3, #5
30003982:	d80b      	bhi.n	3000399c <flash_highspeed_setup+0x1bc>
30003984:	3b01      	subs	r3, #1
30003986:	4843      	ldr	r0, [pc, #268]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003988:	2b04      	cmp	r3, #4
3000398a:	f200 8105 	bhi.w	30003b98 <flash_highspeed_setup+0x3b8>
3000398e:	e8df f013 	tbh	[pc, r3, lsl #1]
30003992:	00b2      	.short	0x00b2
30003994:	00c600af 	.word	0x00c600af
30003998:	00bd00c3 	.word	0x00bd00c3
3000399c:	2bfe      	cmp	r3, #254	; 0xfe
3000399e:	d104      	bne.n	300039aa <flash_highspeed_setup+0x1ca>
300039a0:	6903      	ldr	r3, [r0, #16]
300039a2:	2b00      	cmp	r3, #0
300039a4:	f000 80e7 	beq.w	30003b76 <flash_highspeed_setup+0x396>
300039a8:	4798      	blx	r3
300039aa:	4b43      	ldr	r3, [pc, #268]	; (30003ab8 <flash_highspeed_setup+0x2d8>)
300039ac:	4798      	blx	r3
300039ae:	b110      	cbz	r0, 300039b6 <flash_highspeed_setup+0x1d6>
300039b0:	2304      	movs	r3, #4
300039b2:	f886 3062 	strb.w	r3, [r6, #98]	; 0x62
300039b6:	f8d9 3000 	ldr.w	r3, [r9]
300039ba:	aa03      	add	r2, sp, #12
300039bc:	2101      	movs	r1, #1
300039be:	f896 0056 	ldrb.w	r0, [r6, #86]	; 0x56
300039c2:	f8d3 900c 	ldr.w	r9, [r3, #12]
300039c6:	68f3      	ldr	r3, [r6, #12]
300039c8:	9302      	str	r3, [sp, #8]
300039ca:	47c0      	blx	r8
300039cc:	69b3      	ldr	r3, [r6, #24]
300039ce:	2b00      	cmp	r3, #0
300039d0:	f040 80aa 	bne.w	30003b28 <flash_highspeed_setup+0x348>
300039d4:	4b39      	ldr	r3, [pc, #228]	; (30003abc <flash_highspeed_setup+0x2dc>)
300039d6:	781b      	ldrb	r3, [r3, #0]
300039d8:	2b00      	cmp	r3, #0
300039da:	f040 80b8 	bne.w	30003b4e <flash_highspeed_setup+0x36e>
300039de:	2701      	movs	r7, #1
300039e0:	9b03      	ldr	r3, [sp, #12]
300039e2:	a803      	add	r0, sp, #12
300039e4:	463a      	mov	r2, r7
300039e6:	a902      	add	r1, sp, #8
300039e8:	ea03 0309 	and.w	r3, r3, r9
300039ec:	9303      	str	r3, [sp, #12]
300039ee:	4b34      	ldr	r3, [pc, #208]	; (30003ac0 <flash_highspeed_setup+0x2e0>)
300039f0:	4798      	blx	r3
300039f2:	2800      	cmp	r0, #0
300039f4:	d0aa      	beq.n	3000394c <flash_highspeed_setup+0x16c>
300039f6:	f896 3059 	ldrb.w	r3, [r6, #89]	; 0x59
300039fa:	aa02      	add	r2, sp, #8
300039fc:	f896 0058 	ldrb.w	r0, [r6, #88]	; 0x58
30003a00:	bb73      	cbnz	r3, 30003a60 <flash_highspeed_setup+0x280>
30003a02:	4639      	mov	r1, r7
30003a04:	4b2f      	ldr	r3, [pc, #188]	; (30003ac4 <flash_highspeed_setup+0x2e4>)
30003a06:	4798      	blx	r3
30003a08:	e7a0      	b.n	3000394c <flash_highspeed_setup+0x16c>
30003a0a:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 30003ae4 <flash_highspeed_setup+0x304>
30003a0e:	f8c9 0000 	str.w	r0, [r9]
30003a12:	2800      	cmp	r0, #0
30003a14:	f43f af7d 	beq.w	30003912 <flash_highspeed_setup+0x132>
30003a18:	7a03      	ldrb	r3, [r0, #8]
30003a1a:	2b04      	cmp	r3, #4
30003a1c:	d887      	bhi.n	3000392e <flash_highspeed_setup+0x14e>
30003a1e:	2b04      	cmp	r3, #4
30003a20:	d88c      	bhi.n	3000393c <flash_highspeed_setup+0x15c>
30003a22:	a201      	add	r2, pc, #4	; (adr r2, 30003a28 <flash_highspeed_setup+0x248>)
30003a24:	f852 f023 	ldr.w	pc, [r2, r3, lsl #2]
30003a28:	30003a85 	.word	0x30003a85
30003a2c:	30003ae9 	.word	0x30003ae9
30003a30:	30003a7d 	.word	0x30003a7d
30003a34:	3000393d 	.word	0x3000393d
30003a38:	30003a75 	.word	0x30003a75
30003a3c:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003a40:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a44:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a48:	481f      	ldr	r0, [pc, #124]	; (30003ac8 <flash_highspeed_setup+0x2e8>)
30003a4a:	f008 fe61 	bl	3000c710 <__DiagPrintf_veneer>
30003a4e:	e6ff      	b.n	30003850 <flash_highspeed_setup+0x70>
30003a50:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a54:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a58:	481c      	ldr	r0, [pc, #112]	; (30003acc <flash_highspeed_setup+0x2ec>)
30003a5a:	f008 fe59 	bl	3000c710 <__DiagPrintf_veneer>
30003a5e:	e734      	b.n	300038ca <flash_highspeed_setup+0xea>
30003a60:	2101      	movs	r1, #1
30003a62:	4f18      	ldr	r7, [pc, #96]	; (30003ac4 <flash_highspeed_setup+0x2e4>)
30003a64:	47b8      	blx	r7
30003a66:	f10d 0209 	add.w	r2, sp, #9
30003a6a:	f896 0059 	ldrb.w	r0, [r6, #89]	; 0x59
30003a6e:	2101      	movs	r1, #1
30003a70:	47b8      	blx	r7
30003a72:	e76b      	b.n	3000394c <flash_highspeed_setup+0x16c>
30003a74:	4807      	ldr	r0, [pc, #28]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a76:	4b16      	ldr	r3, [pc, #88]	; (30003ad0 <flash_highspeed_setup+0x2f0>)
30003a78:	4798      	blx	r3
30003a7a:	e75f      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a7c:	4805      	ldr	r0, [pc, #20]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a7e:	4b15      	ldr	r3, [pc, #84]	; (30003ad4 <flash_highspeed_setup+0x2f4>)
30003a80:	4798      	blx	r3
30003a82:	e75b      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a84:	4803      	ldr	r0, [pc, #12]	; (30003a94 <flash_highspeed_setup+0x2b4>)
30003a86:	4b14      	ldr	r3, [pc, #80]	; (30003ad8 <flash_highspeed_setup+0x2f8>)
30003a88:	4798      	blx	r3
30003a8a:	e757      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003a8c:	3000dab4 	.word	0x3000dab4
30003a90:	3000dab6 	.word	0x3000dab6
30003a94:	2001c01c 	.word	0x2001c01c
30003a98:	2001c00c 	.word	0x2001c00c
30003a9c:	3000d9b0 	.word	0x3000d9b0
30003aa0:	3000dab8 	.word	0x3000dab8
30003aa4:	3000db68 	.word	0x3000db68
30003aa8:	0000b8c1 	.word	0x0000b8c1
30003aac:	3000db58 	.word	0x3000db58
30003ab0:	0000bd0d 	.word	0x0000bd0d
30003ab4:	0000c0f9 	.word	0x0000c0f9
30003ab8:	0000c1d9 	.word	0x0000c1d9
30003abc:	30010190 	.word	0x30010190
30003ac0:	000129bd 	.word	0x000129bd
30003ac4:	0000a149 	.word	0x0000a149
30003ac8:	3000c9a4 	.word	0x3000c9a4
30003acc:	3000c9f8 	.word	0x3000c9f8
30003ad0:	0000be71 	.word	0x0000be71
30003ad4:	0000bee9 	.word	0x0000bee9
30003ad8:	0000bfcd 	.word	0x0000bfcd
30003adc:	3000a219 	.word	0x3000a219
30003ae0:	00009db5 	.word	0x00009db5
30003ae4:	30010194 	.word	0x30010194
30003ae8:	482d      	ldr	r0, [pc, #180]	; (30003ba0 <flash_highspeed_setup+0x3c0>)
30003aea:	4b2e      	ldr	r3, [pc, #184]	; (30003ba4 <flash_highspeed_setup+0x3c4>)
30003aec:	4798      	blx	r3
30003aee:	e725      	b.n	3000393c <flash_highspeed_setup+0x15c>
30003af0:	4b2d      	ldr	r3, [pc, #180]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003af2:	4798      	blx	r3
30003af4:	e759      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003af6:	4b2d      	ldr	r3, [pc, #180]	; (30003bac <flash_highspeed_setup+0x3cc>)
30003af8:	4798      	blx	r3
30003afa:	f89d 300e 	ldrb.w	r3, [sp, #14]
30003afe:	2b15      	cmp	r3, #21
30003b00:	f67f af53 	bls.w	300039aa <flash_highspeed_setup+0x1ca>
30003b04:	2331      	movs	r3, #49	; 0x31
30003b06:	f886 3059 	strb.w	r3, [r6, #89]	; 0x59
30003b0a:	e74e      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b0c:	4b26      	ldr	r3, [pc, #152]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003b0e:	4798      	blx	r3
30003b10:	4b27      	ldr	r3, [pc, #156]	; (30003bb0 <flash_highspeed_setup+0x3d0>)
30003b12:	2201      	movs	r2, #1
30003b14:	701a      	strb	r2, [r3, #0]
30003b16:	e748      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b18:	4b26      	ldr	r3, [pc, #152]	; (30003bb4 <flash_highspeed_setup+0x3d4>)
30003b1a:	4798      	blx	r3
30003b1c:	e745      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b1e:	4b22      	ldr	r3, [pc, #136]	; (30003ba8 <flash_highspeed_setup+0x3c8>)
30003b20:	4798      	blx	r3
30003b22:	2300      	movs	r3, #0
30003b24:	60f3      	str	r3, [r6, #12]
30003b26:	e740      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b28:	f10d 020d 	add.w	r2, sp, #13
30003b2c:	2101      	movs	r1, #1
30003b2e:	f896 0057 	ldrb.w	r0, [r6, #87]	; 0x57
30003b32:	2702      	movs	r7, #2
30003b34:	47c0      	blx	r8
30003b36:	e753      	b.n	300039e0 <flash_highspeed_setup+0x200>
30003b38:	4b1f      	ldr	r3, [pc, #124]	; (30003bb8 <flash_highspeed_setup+0x3d8>)
30003b3a:	20b0      	movs	r0, #176	; 0xb0
30003b3c:	4798      	blx	r3
30003b3e:	4601      	mov	r1, r0
30003b40:	4e1e      	ldr	r6, [pc, #120]	; (30003bbc <flash_highspeed_setup+0x3dc>)
30003b42:	20b0      	movs	r0, #176	; 0xb0
30003b44:	f041 0101 	orr.w	r1, r1, #1
30003b48:	b2c9      	uxtb	r1, r1
30003b4a:	47b0      	blx	r6
30003b4c:	e6fb      	b.n	30003946 <flash_highspeed_setup+0x166>
30003b4e:	f10d 020d 	add.w	r2, sp, #13
30003b52:	2102      	movs	r1, #2
30003b54:	2015      	movs	r0, #21
30003b56:	2703      	movs	r7, #3
30003b58:	47c0      	blx	r8
30003b5a:	9b02      	ldr	r3, [sp, #8]
30003b5c:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
30003b60:	9302      	str	r3, [sp, #8]
30003b62:	e73d      	b.n	300039e0 <flash_highspeed_setup+0x200>
30003b64:	f44f 711c 	mov.w	r1, #624	; 0x270
30003b68:	4815      	ldr	r0, [pc, #84]	; (30003bc0 <flash_highspeed_setup+0x3e0>)
30003b6a:	f008 fdc9 	bl	3000c700 <__io_assert_failed_veneer>
30003b6e:	f8d9 3000 	ldr.w	r3, [r9]
30003b72:	691b      	ldr	r3, [r3, #16]
30003b74:	e6e1      	b.n	3000393a <flash_highspeed_setup+0x15a>
30003b76:	f44f 7111 	mov.w	r1, #580	; 0x244
30003b7a:	4812      	ldr	r0, [pc, #72]	; (30003bc4 <flash_highspeed_setup+0x3e4>)
30003b7c:	f008 fdc0 	bl	3000c700 <__io_assert_failed_veneer>
30003b80:	f8d9 3000 	ldr.w	r3, [r9]
30003b84:	691b      	ldr	r3, [r3, #16]
30003b86:	e70f      	b.n	300039a8 <flash_highspeed_setup+0x1c8>
30003b88:	480f      	ldr	r0, [pc, #60]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b8a:	f008 fdc1 	bl	3000c710 <__DiagPrintf_veneer>
30003b8e:	e686      	b.n	3000389e <flash_highspeed_setup+0xbe>
30003b90:	480d      	ldr	r0, [pc, #52]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b92:	f008 fdbd 	bl	3000c710 <__DiagPrintf_veneer>
30003b96:	e6c0      	b.n	3000391a <flash_highspeed_setup+0x13a>
30003b98:	4b0c      	ldr	r3, [pc, #48]	; (30003bcc <flash_highspeed_setup+0x3ec>)
30003b9a:	4798      	blx	r3
30003b9c:	e705      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b9e:	bf00      	nop
30003ba0:	2001c01c 	.word	0x2001c01c
30003ba4:	0000bf59 	.word	0x0000bf59
30003ba8:	0000a3c5 	.word	0x0000a3c5
30003bac:	0000a441 	.word	0x0000a441
30003bb0:	30010190 	.word	0x30010190
30003bb4:	0000a345 	.word	0x0000a345
30003bb8:	0000b991 	.word	0x0000b991
30003bbc:	0000bd0d 	.word	0x0000bd0d
30003bc0:	3000db58 	.word	0x3000db58
30003bc4:	3000db68 	.word	0x3000db68
30003bc8:	3000c9d4 	.word	0x3000c9d4
30003bcc:	0000a4c1 	.word	0x0000a4c1

30003bd0 <BOOT_LoadSubImage>:
30003bd0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003bd4:	b08b      	sub	sp, #44	; 0x2c
30003bd6:	460f      	mov	r7, r1
30003bd8:	9300      	str	r3, [sp, #0]
30003bda:	f89d 3050 	ldrb.w	r3, [sp, #80]	; 0x50
30003bde:	9301      	str	r3, [sp, #4]
30003be0:	b34a      	cbz	r2, 30003c36 <BOOT_LoadSubImage+0x66>
30003be2:	4691      	mov	r9, r2
30003be4:	2600      	movs	r6, #0
30003be6:	f8df a14c 	ldr.w	sl, [pc, #332]	; 30003d34 <BOOT_LoadSubImage+0x164>
30003bea:	4605      	mov	r5, r0
30003bec:	2220      	movs	r2, #32
30003bee:	4639      	mov	r1, r7
30003bf0:	a802      	add	r0, sp, #8
30003bf2:	f002 f893 	bl	30005d1c <BOOT_ImgCopy>
30003bf6:	9b02      	ldr	r3, [sp, #8]
30003bf8:	4553      	cmp	r3, sl
30003bfa:	d155      	bne.n	30003ca8 <BOOT_LoadSubImage+0xd8>
30003bfc:	9b03      	ldr	r3, [sp, #12]
30003bfe:	4a46      	ldr	r2, [pc, #280]	; (30003d18 <BOOT_LoadSubImage+0x148>)
30003c00:	4293      	cmp	r3, r2
30003c02:	d151      	bne.n	30003ca8 <BOOT_LoadSubImage+0xd8>
30003c04:	4b45      	ldr	r3, [pc, #276]	; (30003d1c <BOOT_LoadSubImage+0x14c>)
30003c06:	e9dd 2004 	ldrd	r2, r0, [sp, #16]
30003c0a:	18c3      	adds	r3, r0, r3
30003c0c:	f1a0 0b20 	sub.w	fp, r0, #32
30003c10:	f102 0820 	add.w	r8, r2, #32
30003c14:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003c18:	d213      	bcs.n	30003c42 <BOOT_LoadSubImage+0x72>
30003c1a:	b145      	cbz	r5, 30003c2e <BOOT_LoadSubImage+0x5e>
30003c1c:	1d2b      	adds	r3, r5, #4
30003c1e:	f845 b036 	str.w	fp, [r5, r6, lsl #3]
30003c22:	f843 8036 	str.w	r8, [r3, r6, lsl #3]
30003c26:	4b3e      	ldr	r3, [pc, #248]	; (30003d20 <BOOT_LoadSubImage+0x150>)
30003c28:	689b      	ldr	r3, [r3, #8]
30003c2a:	079b      	lsls	r3, r3, #30
30003c2c:	d433      	bmi.n	30003c96 <BOOT_LoadSubImage+0xc6>
30003c2e:	3601      	adds	r6, #1
30003c30:	4447      	add	r7, r8
30003c32:	454e      	cmp	r6, r9
30003c34:	d1da      	bne.n	30003bec <BOOT_LoadSubImage+0x1c>
30003c36:	4b3b      	ldr	r3, [pc, #236]	; (30003d24 <BOOT_LoadSubImage+0x154>)
30003c38:	2001      	movs	r0, #1
30003c3a:	601f      	str	r7, [r3, #0]
30003c3c:	b00b      	add	sp, #44	; 0x2c
30003c3e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003c42:	4c39      	ldr	r4, [pc, #228]	; (30003d28 <BOOT_LoadSubImage+0x158>)
30003c44:	4642      	mov	r2, r8
30003c46:	4639      	mov	r1, r7
30003c48:	4658      	mov	r0, fp
30003c4a:	f002 f867 	bl	30005d1c <BOOT_ImgCopy>
30003c4e:	6963      	ldr	r3, [r4, #20]
30003c50:	03da      	lsls	r2, r3, #15
30003c52:	d5e2      	bpl.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003c54:	ea0b 0308 	and.w	r3, fp, r8
30003c58:	3301      	adds	r3, #1
30003c5a:	d02c      	beq.n	30003cb6 <BOOT_LoadSubImage+0xe6>
30003c5c:	f01b 031f 	ands.w	r3, fp, #31
30003c60:	d049      	beq.n	30003cf6 <BOOT_LoadSubImage+0x126>
30003c62:	4443      	add	r3, r8
30003c64:	f02b 021f 	bic.w	r2, fp, #31
30003c68:	f3bf 8f4f 	dsb	sy
30003c6c:	2b00      	cmp	r3, #0
30003c6e:	dd0d      	ble.n	30003c8c <BOOT_LoadSubImage+0xbc>
30003c70:	1e59      	subs	r1, r3, #1
30003c72:	f102 0320 	add.w	r3, r2, #32
30003c76:	482c      	ldr	r0, [pc, #176]	; (30003d28 <BOOT_LoadSubImage+0x158>)
30003c78:	f021 011f 	bic.w	r1, r1, #31
30003c7c:	4419      	add	r1, r3
30003c7e:	4299      	cmp	r1, r3
30003c80:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003c84:	461a      	mov	r2, r3
30003c86:	f103 0320 	add.w	r3, r3, #32
30003c8a:	d1f8      	bne.n	30003c7e <BOOT_LoadSubImage+0xae>
30003c8c:	f3bf 8f4f 	dsb	sy
30003c90:	f3bf 8f6f 	isb	sy
30003c94:	e7c1      	b.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003c96:	9900      	ldr	r1, [sp, #0]
30003c98:	465a      	mov	r2, fp
30003c9a:	4643      	mov	r3, r8
30003c9c:	4823      	ldr	r0, [pc, #140]	; (30003d2c <BOOT_LoadSubImage+0x15c>)
30003c9e:	f851 1026 	ldr.w	r1, [r1, r6, lsl #2]
30003ca2:	f008 fd35 	bl	3000c710 <__DiagPrintf_veneer>
30003ca6:	e7c2      	b.n	30003c2e <BOOT_LoadSubImage+0x5e>
30003ca8:	9b01      	ldr	r3, [sp, #4]
30003caa:	2b01      	cmp	r3, #1
30003cac:	d026      	beq.n	30003cfc <BOOT_LoadSubImage+0x12c>
30003cae:	2000      	movs	r0, #0
30003cb0:	b00b      	add	sp, #44	; 0x2c
30003cb2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003cb6:	2300      	movs	r3, #0
30003cb8:	f8c4 3084 	str.w	r3, [r4, #132]	; 0x84
30003cbc:	f3bf 8f4f 	dsb	sy
30003cc0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30003cc4:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003cc8:	f3c3 304e 	ubfx	r0, r3, #13, #15
30003ccc:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30003cd0:	0140      	lsls	r0, r0, #5
30003cd2:	ea00 010e 	and.w	r1, r0, lr
30003cd6:	4663      	mov	r3, ip
30003cd8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30003cdc:	3b01      	subs	r3, #1
30003cde:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30003ce2:	d2f9      	bcs.n	30003cd8 <BOOT_LoadSubImage+0x108>
30003ce4:	3820      	subs	r0, #32
30003ce6:	f110 0f20 	cmn.w	r0, #32
30003cea:	d1f2      	bne.n	30003cd2 <BOOT_LoadSubImage+0x102>
30003cec:	f3bf 8f4f 	dsb	sy
30003cf0:	f3bf 8f6f 	isb	sy
30003cf4:	e791      	b.n	30003c1a <BOOT_LoadSubImage+0x4a>
30003cf6:	4643      	mov	r3, r8
30003cf8:	465a      	mov	r2, fp
30003cfa:	e7b5      	b.n	30003c68 <BOOT_LoadSubImage+0x98>
30003cfc:	4b08      	ldr	r3, [pc, #32]	; (30003d20 <BOOT_LoadSubImage+0x150>)
30003cfe:	6898      	ldr	r0, [r3, #8]
30003d00:	f010 0002 	ands.w	r0, r0, #2
30003d04:	d0d4      	beq.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d06:	9b00      	ldr	r3, [sp, #0]
30003d08:	4809      	ldr	r0, [pc, #36]	; (30003d30 <BOOT_LoadSubImage+0x160>)
30003d0a:	f853 1026 	ldr.w	r1, [r3, r6, lsl #2]
30003d0e:	f008 fcff 	bl	3000c710 <__DiagPrintf_veneer>
30003d12:	2000      	movs	r0, #0
30003d14:	e7cc      	b.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d16:	bf00      	nop
30003d18:	31313738 	.word	0x31313738
30003d1c:	f7ffffe0 	.word	0xf7ffffe0
30003d20:	2001c00c 	.word	0x2001c00c
30003d24:	3000f3f8 	.word	0x3000f3f8
30003d28:	e000ed00 	.word	0xe000ed00
30003d2c:	3000cadc 	.word	0x3000cadc
30003d30:	3000cab4 	.word	0x3000cab4
30003d34:	35393138 	.word	0x35393138

30003d38 <BOOT_LoadAPSubImage>:
30003d38:	4bb1      	ldr	r3, [pc, #708]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003d3a:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30003d3e:	460c      	mov	r4, r1
30003d40:	4606      	mov	r6, r0
30003d42:	b08a      	sub	sp, #40	; 0x28
30003d44:	4615      	mov	r5, r2
30003d46:	2220      	movs	r2, #32
30003d48:	e893 0003 	ldmia.w	r3, {r0, r1}
30003d4c:	0c0b      	lsrs	r3, r1, #16
30003d4e:	9000      	str	r0, [sp, #0]
30003d50:	f8ad 1004 	strh.w	r1, [sp, #4]
30003d54:	a802      	add	r0, sp, #8
30003d56:	4621      	mov	r1, r4
30003d58:	f88d 3006 	strb.w	r3, [sp, #6]
30003d5c:	f001 ffde 	bl	30005d1c <BOOT_ImgCopy>
30003d60:	4ba8      	ldr	r3, [pc, #672]	; (30004004 <BOOT_LoadAPSubImage+0x2cc>)
30003d62:	9a02      	ldr	r2, [sp, #8]
30003d64:	429a      	cmp	r2, r3
30003d66:	d108      	bne.n	30003d7a <BOOT_LoadAPSubImage+0x42>
30003d68:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30003d6c:	9a03      	ldr	r2, [sp, #12]
30003d6e:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30003d72:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30003d76:	429a      	cmp	r2, r3
30003d78:	d010      	beq.n	30003d9c <BOOT_LoadAPSubImage+0x64>
30003d7a:	2d01      	cmp	r5, #1
30003d7c:	d003      	beq.n	30003d86 <BOOT_LoadAPSubImage+0x4e>
30003d7e:	2000      	movs	r0, #0
30003d80:	b00a      	add	sp, #40	; 0x28
30003d82:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003d86:	4ba0      	ldr	r3, [pc, #640]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003d88:	6898      	ldr	r0, [r3, #8]
30003d8a:	f010 0002 	ands.w	r0, r0, #2
30003d8e:	d0f7      	beq.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d90:	4669      	mov	r1, sp
30003d92:	489e      	ldr	r0, [pc, #632]	; (3000400c <BOOT_LoadAPSubImage+0x2d4>)
30003d94:	f008 fcbc 	bl	3000c710 <__DiagPrintf_veneer>
30003d98:	2000      	movs	r0, #0
30003d9a:	e7f1      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d9c:	9a05      	ldr	r2, [sp, #20]
30003d9e:	4b9c      	ldr	r3, [pc, #624]	; (30004010 <BOOT_LoadAPSubImage+0x2d8>)
30003da0:	6ca1      	ldr	r1, [r4, #72]	; 0x48
30003da2:	f1a2 0520 	sub.w	r5, r2, #32
30003da6:	4413      	add	r3, r2
30003da8:	f101 08d0 	add.w	r8, r1, #208	; 0xd0
30003dac:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003db0:	d239      	bcs.n	30003e26 <BOOT_LoadAPSubImage+0xee>
30003db2:	4b98      	ldr	r3, [pc, #608]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003db4:	4445      	add	r5, r8
30003db6:	f8d4 9070 	ldr.w	r9, [r4, #112]	; 0x70
30003dba:	4798      	blx	r3
30003dbc:	4f92      	ldr	r7, [pc, #584]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003dbe:	2801      	cmp	r0, #1
30003dc0:	44a0      	add	r8, r4
30003dc2:	68bb      	ldr	r3, [r7, #8]
30003dc4:	d01f      	beq.n	30003e06 <BOOT_LoadAPSubImage+0xce>
30003dc6:	079a      	lsls	r2, r3, #30
30003dc8:	f100 8083 	bmi.w	30003ed2 <BOOT_LoadAPSubImage+0x19a>
30003dcc:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003dd0:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dd4:	f080 8081 	bcs.w	30003eda <BOOT_LoadAPSubImage+0x1a2>
30003dd8:	444d      	add	r5, r9
30003dda:	eb08 0109 	add.w	r1, r8, r9
30003dde:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003de2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003de6:	d249      	bcs.n	30003e7c <BOOT_LoadAPSubImage+0x144>
30003de8:	b14e      	cbz	r6, 30003dfe <BOOT_LoadAPSubImage+0xc6>
30003dea:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
30003dee:	3a20      	subs	r2, #32
30003df0:	3320      	adds	r3, #32
30003df2:	e9c6 2300 	strd	r2, r3, [r6]
30003df6:	68b9      	ldr	r1, [r7, #8]
30003df8:	0789      	lsls	r1, r1, #30
30003dfa:	f100 80b5 	bmi.w	30003f68 <BOOT_LoadAPSubImage+0x230>
30003dfe:	2001      	movs	r0, #1
30003e00:	b00a      	add	sp, #40	; 0x28
30003e02:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30003e06:	0798      	lsls	r0, r3, #30
30003e08:	f100 80b3 	bmi.w	30003f72 <BOOT_LoadAPSubImage+0x23a>
30003e0c:	464a      	mov	r2, r9
30003e0e:	4629      	mov	r1, r5
30003e10:	4640      	mov	r0, r8
30003e12:	f000 fc07 	bl	30004624 <BOOT_LoadBL32RDPImg>
30003e16:	2800      	cmp	r0, #0
30003e18:	d1de      	bne.n	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003e1a:	683b      	ldr	r3, [r7, #0]
30003e1c:	0799      	lsls	r1, r3, #30
30003e1e:	f100 80cb 	bmi.w	30003fb8 <BOOT_LoadAPSubImage+0x280>
30003e22:	20ff      	movs	r0, #255	; 0xff
30003e24:	e7ac      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003e26:	4f7c      	ldr	r7, [pc, #496]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e28:	4642      	mov	r2, r8
30003e2a:	4621      	mov	r1, r4
30003e2c:	4628      	mov	r0, r5
30003e2e:	f001 ff75 	bl	30005d1c <BOOT_ImgCopy>
30003e32:	697b      	ldr	r3, [r7, #20]
30003e34:	03db      	lsls	r3, r3, #15
30003e36:	d5bc      	bpl.n	30003db2 <BOOT_LoadAPSubImage+0x7a>
30003e38:	ea05 0308 	and.w	r3, r5, r8
30003e3c:	3301      	adds	r3, #1
30003e3e:	d077      	beq.n	30003f30 <BOOT_LoadAPSubImage+0x1f8>
30003e40:	f015 031f 	ands.w	r3, r5, #31
30003e44:	f000 8099 	beq.w	30003f7a <BOOT_LoadAPSubImage+0x242>
30003e48:	4443      	add	r3, r8
30003e4a:	f025 021f 	bic.w	r2, r5, #31
30003e4e:	f3bf 8f4f 	dsb	sy
30003e52:	2b00      	cmp	r3, #0
30003e54:	dd0d      	ble.n	30003e72 <BOOT_LoadAPSubImage+0x13a>
30003e56:	1e59      	subs	r1, r3, #1
30003e58:	f102 0320 	add.w	r3, r2, #32
30003e5c:	486e      	ldr	r0, [pc, #440]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e5e:	f021 011f 	bic.w	r1, r1, #31
30003e62:	4419      	add	r1, r3
30003e64:	428b      	cmp	r3, r1
30003e66:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003e6a:	461a      	mov	r2, r3
30003e6c:	f103 0320 	add.w	r3, r3, #32
30003e70:	d1f8      	bne.n	30003e64 <BOOT_LoadAPSubImage+0x12c>
30003e72:	f3bf 8f4f 	dsb	sy
30003e76:	f3bf 8f6f 	isb	sy
30003e7a:	e79a      	b.n	30003db2 <BOOT_LoadAPSubImage+0x7a>
30003e7c:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30003e80:	4628      	mov	r0, r5
30003e82:	4622      	mov	r2, r4
30003e84:	f001 ff4a 	bl	30005d1c <BOOT_ImgCopy>
30003e88:	4963      	ldr	r1, [pc, #396]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003e8a:	694b      	ldr	r3, [r1, #20]
30003e8c:	03d8      	lsls	r0, r3, #15
30003e8e:	d5ab      	bpl.n	30003de8 <BOOT_LoadAPSubImage+0xb0>
30003e90:	ea05 0304 	and.w	r3, r5, r4
30003e94:	3301      	adds	r3, #1
30003e96:	d073      	beq.n	30003f80 <BOOT_LoadAPSubImage+0x248>
30003e98:	f015 031f 	ands.w	r3, r5, #31
30003e9c:	d002      	beq.n	30003ea4 <BOOT_LoadAPSubImage+0x16c>
30003e9e:	f025 051f 	bic.w	r5, r5, #31
30003ea2:	441c      	add	r4, r3
30003ea4:	f3bf 8f4f 	dsb	sy
30003ea8:	2c00      	cmp	r4, #0
30003eaa:	dd0d      	ble.n	30003ec8 <BOOT_LoadAPSubImage+0x190>
30003eac:	1e62      	subs	r2, r4, #1
30003eae:	f105 0320 	add.w	r3, r5, #32
30003eb2:	4959      	ldr	r1, [pc, #356]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003eb4:	f022 021f 	bic.w	r2, r2, #31
30003eb8:	441a      	add	r2, r3
30003eba:	4293      	cmp	r3, r2
30003ebc:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30003ec0:	461d      	mov	r5, r3
30003ec2:	f103 0320 	add.w	r3, r3, #32
30003ec6:	d1f8      	bne.n	30003eba <BOOT_LoadAPSubImage+0x182>
30003ec8:	f3bf 8f4f 	dsb	sy
30003ecc:	f3bf 8f6f 	isb	sy
30003ed0:	e78a      	b.n	30003de8 <BOOT_LoadAPSubImage+0xb0>
30003ed2:	4852      	ldr	r0, [pc, #328]	; (3000401c <BOOT_LoadAPSubImage+0x2e4>)
30003ed4:	f008 fc1c 	bl	3000c710 <__DiagPrintf_veneer>
30003ed8:	e778      	b.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003eda:	4641      	mov	r1, r8
30003edc:	464a      	mov	r2, r9
30003ede:	4628      	mov	r0, r5
30003ee0:	f001 ff1c 	bl	30005d1c <BOOT_ImgCopy>
30003ee4:	494c      	ldr	r1, [pc, #304]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003ee6:	694b      	ldr	r3, [r1, #20]
30003ee8:	03db      	lsls	r3, r3, #15
30003eea:	f57f af75 	bpl.w	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003eee:	ea05 0309 	and.w	r3, r5, r9
30003ef2:	3301      	adds	r3, #1
30003ef4:	d064      	beq.n	30003fc0 <BOOT_LoadAPSubImage+0x288>
30003ef6:	f015 031f 	ands.w	r3, r5, #31
30003efa:	d07d      	beq.n	30003ff8 <BOOT_LoadAPSubImage+0x2c0>
30003efc:	444b      	add	r3, r9
30003efe:	f025 021f 	bic.w	r2, r5, #31
30003f02:	f3bf 8f4f 	dsb	sy
30003f06:	2b00      	cmp	r3, #0
30003f08:	dd0d      	ble.n	30003f26 <BOOT_LoadAPSubImage+0x1ee>
30003f0a:	1e59      	subs	r1, r3, #1
30003f0c:	f102 0320 	add.w	r3, r2, #32
30003f10:	4841      	ldr	r0, [pc, #260]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003f12:	f021 011f 	bic.w	r1, r1, #31
30003f16:	4419      	add	r1, r3
30003f18:	428b      	cmp	r3, r1
30003f1a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003f1e:	461a      	mov	r2, r3
30003f20:	f103 0320 	add.w	r3, r3, #32
30003f24:	d1f8      	bne.n	30003f18 <BOOT_LoadAPSubImage+0x1e0>
30003f26:	f3bf 8f4f 	dsb	sy
30003f2a:	f3bf 8f6f 	isb	sy
30003f2e:	e753      	b.n	30003dd8 <BOOT_LoadAPSubImage+0xa0>
30003f30:	2300      	movs	r3, #0
30003f32:	f8c7 3084 	str.w	r3, [r7, #132]	; 0x84
30003f36:	f3bf 8f4f 	dsb	sy
30003f3a:	f8d7 0080 	ldr.w	r0, [r7, #128]	; 0x80
30003f3e:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003f42:	f3c0 324e 	ubfx	r2, r0, #13, #15
30003f46:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30003f4a:	0152      	lsls	r2, r2, #5
30003f4c:	ea02 0c0e 	and.w	ip, r2, lr
30003f50:	4603      	mov	r3, r0
30003f52:	ea4c 7183 	orr.w	r1, ip, r3, lsl #30
30003f56:	3b01      	subs	r3, #1
30003f58:	f8c7 1274 	str.w	r1, [r7, #628]	; 0x274
30003f5c:	d2f9      	bcs.n	30003f52 <BOOT_LoadAPSubImage+0x21a>
30003f5e:	3a20      	subs	r2, #32
30003f60:	f112 0f20 	cmn.w	r2, #32
30003f64:	d1f2      	bne.n	30003f4c <BOOT_LoadAPSubImage+0x214>
30003f66:	e784      	b.n	30003e72 <BOOT_LoadAPSubImage+0x13a>
30003f68:	4669      	mov	r1, sp
30003f6a:	482d      	ldr	r0, [pc, #180]	; (30004020 <BOOT_LoadAPSubImage+0x2e8>)
30003f6c:	f008 fbd0 	bl	3000c710 <__DiagPrintf_veneer>
30003f70:	e745      	b.n	30003dfe <BOOT_LoadAPSubImage+0xc6>
30003f72:	482c      	ldr	r0, [pc, #176]	; (30004024 <BOOT_LoadAPSubImage+0x2ec>)
30003f74:	f008 fbcc 	bl	3000c710 <__DiagPrintf_veneer>
30003f78:	e748      	b.n	30003e0c <BOOT_LoadAPSubImage+0xd4>
30003f7a:	4643      	mov	r3, r8
30003f7c:	462a      	mov	r2, r5
30003f7e:	e766      	b.n	30003e4e <BOOT_LoadAPSubImage+0x116>
30003f80:	2300      	movs	r3, #0
30003f82:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003f86:	f3bf 8f4f 	dsb	sy
30003f8a:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003f8e:	f643 75e0 	movw	r5, #16352	; 0x3fe0
30003f92:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30003f96:	f3c3 334e 	ubfx	r3, r3, #13, #15
30003f9a:	015c      	lsls	r4, r3, #5
30003f9c:	ea04 0005 	and.w	r0, r4, r5
30003fa0:	4663      	mov	r3, ip
30003fa2:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003fa6:	3b01      	subs	r3, #1
30003fa8:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003fac:	d2f9      	bcs.n	30003fa2 <BOOT_LoadAPSubImage+0x26a>
30003fae:	3c20      	subs	r4, #32
30003fb0:	f114 0f20 	cmn.w	r4, #32
30003fb4:	d1f2      	bne.n	30003f9c <BOOT_LoadAPSubImage+0x264>
30003fb6:	e787      	b.n	30003ec8 <BOOT_LoadAPSubImage+0x190>
30003fb8:	481b      	ldr	r0, [pc, #108]	; (30004028 <BOOT_LoadAPSubImage+0x2f0>)
30003fba:	f008 fba9 	bl	3000c710 <__DiagPrintf_veneer>
30003fbe:	e730      	b.n	30003e22 <BOOT_LoadAPSubImage+0xea>
30003fc0:	2300      	movs	r3, #0
30003fc2:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003fc6:	f3bf 8f4f 	dsb	sy
30003fca:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003fce:	f643 7ae0 	movw	sl, #16352	; 0x3fe0
30003fd2:	f3c3 324e 	ubfx	r2, r3, #13, #15
30003fd6:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30003fda:	0152      	lsls	r2, r2, #5
30003fdc:	ea02 0c0a 	and.w	ip, r2, sl
30003fe0:	4673      	mov	r3, lr
30003fe2:	ea4c 7083 	orr.w	r0, ip, r3, lsl #30
30003fe6:	3b01      	subs	r3, #1
30003fe8:	f8c1 0274 	str.w	r0, [r1, #628]	; 0x274
30003fec:	d2f9      	bcs.n	30003fe2 <BOOT_LoadAPSubImage+0x2aa>
30003fee:	3a20      	subs	r2, #32
30003ff0:	f112 0f20 	cmn.w	r2, #32
30003ff4:	d1f2      	bne.n	30003fdc <BOOT_LoadAPSubImage+0x2a4>
30003ff6:	e796      	b.n	30003f26 <BOOT_LoadAPSubImage+0x1ee>
30003ff8:	464b      	mov	r3, r9
30003ffa:	462a      	mov	r2, r5
30003ffc:	e781      	b.n	30003f02 <BOOT_LoadAPSubImage+0x1ca>
30003ffe:	bf00      	nop
30004000:	3000caac 	.word	0x3000caac
30004004:	35393138 	.word	0x35393138
30004008:	2001c00c 	.word	0x2001c00c
3000400c:	3000cab4 	.word	0x3000cab4
30004010:	f7ffffe0 	.word	0xf7ffffe0
30004014:	0000c109 	.word	0x0000c109
30004018:	e000ed00 	.word	0xe000ed00
3000401c:	3000cb28 	.word	0x3000cb28
30004020:	3000cadc 	.word	0x3000cadc
30004024:	3000cb04 	.word	0x3000cb04
30004028:	3000cb4c 	.word	0x3000cb4c

3000402c <BOOT_RSIPIvSet>:
3000402c:	b530      	push	{r4, r5, lr}
3000402e:	2300      	movs	r3, #0
30004030:	b083      	sub	sp, #12
30004032:	4a09      	ldr	r2, [pc, #36]	; (30004058 <BOOT_RSIPIvSet+0x2c>)
30004034:	4605      	mov	r5, r0
30004036:	460c      	mov	r4, r1
30004038:	9300      	str	r3, [sp, #0]
3000403a:	9301      	str	r3, [sp, #4]
3000403c:	4790      	blx	r2
3000403e:	b148      	cbz	r0, 30004054 <BOOT_RSIPIvSet+0x28>
30004040:	f105 0120 	add.w	r1, r5, #32
30004044:	4b05      	ldr	r3, [pc, #20]	; (3000405c <BOOT_RSIPIvSet+0x30>)
30004046:	2208      	movs	r2, #8
30004048:	4668      	mov	r0, sp
3000404a:	4798      	blx	r3
3000404c:	4669      	mov	r1, sp
3000404e:	b2e0      	uxtb	r0, r4
30004050:	4b03      	ldr	r3, [pc, #12]	; (30004060 <BOOT_RSIPIvSet+0x34>)
30004052:	4798      	blx	r3
30004054:	b003      	add	sp, #12
30004056:	bd30      	pop	{r4, r5, pc}
30004058:	0000c129 	.word	0x0000c129
3000405c:	00012a1d 	.word	0x00012a1d
30004060:	30009f09 	.word	0x30009f09

30004064 <BOOT_OTFCheck>:
30004064:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004068:	4d1d      	ldr	r5, [pc, #116]	; (300040e0 <BOOT_OTFCheck+0x7c>)
3000406a:	b082      	sub	sp, #8
3000406c:	4604      	mov	r4, r0
3000406e:	4688      	mov	r8, r1
30004070:	4617      	mov	r7, r2
30004072:	461e      	mov	r6, r3
30004074:	47a8      	blx	r5
30004076:	b1b0      	cbz	r0, 300040a6 <BOOT_OTFCheck+0x42>
30004078:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
3000407c:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004080:	d211      	bcs.n	300040a6 <BOOT_OTFCheck+0x42>
30004082:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30004086:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000408a:	d20c      	bcs.n	300040a6 <BOOT_OTFCheck+0x42>
3000408c:	4d15      	ldr	r5, [pc, #84]	; (300040e4 <BOOT_OTFCheck+0x80>)
3000408e:	682b      	ldr	r3, [r5, #0]
30004090:	039a      	lsls	r2, r3, #14
30004092:	d420      	bmi.n	300040d6 <BOOT_OTFCheck+0x72>
30004094:	4b14      	ldr	r3, [pc, #80]	; (300040e8 <BOOT_OTFCheck+0x84>)
30004096:	4798      	blx	r3
30004098:	4603      	mov	r3, r0
3000409a:	b138      	cbz	r0, 300040ac <BOOT_OTFCheck+0x48>
3000409c:	2801      	cmp	r0, #1
3000409e:	d006      	beq.n	300040ae <BOOT_OTFCheck+0x4a>
300040a0:	682b      	ldr	r3, [r5, #0]
300040a2:	039b      	lsls	r3, r3, #14
300040a4:	d411      	bmi.n	300040ca <BOOT_OTFCheck+0x66>
300040a6:	b002      	add	sp, #8
300040a8:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300040ac:	2302      	movs	r3, #2
300040ae:	4630      	mov	r0, r6
300040b0:	4621      	mov	r1, r4
300040b2:	9301      	str	r3, [sp, #4]
300040b4:	4642      	mov	r2, r8
300040b6:	2301      	movs	r3, #1
300040b8:	4c0c      	ldr	r4, [pc, #48]	; (300040ec <BOOT_OTFCheck+0x88>)
300040ba:	9700      	str	r7, [sp, #0]
300040bc:	47a0      	blx	r4
300040be:	2001      	movs	r0, #1
300040c0:	4b0b      	ldr	r3, [pc, #44]	; (300040f0 <BOOT_OTFCheck+0x8c>)
300040c2:	b002      	add	sp, #8
300040c4:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040c8:	4718      	bx	r3
300040ca:	480a      	ldr	r0, [pc, #40]	; (300040f4 <BOOT_OTFCheck+0x90>)
300040cc:	b002      	add	sp, #8
300040ce:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040d2:	f008 bb1d 	b.w	3000c710 <__DiagPrintf_veneer>
300040d6:	4808      	ldr	r0, [pc, #32]	; (300040f8 <BOOT_OTFCheck+0x94>)
300040d8:	f008 fb1a 	bl	3000c710 <__DiagPrintf_veneer>
300040dc:	e7da      	b.n	30004094 <BOOT_OTFCheck+0x30>
300040de:	bf00      	nop
300040e0:	0000c129 	.word	0x0000c129
300040e4:	2001c00c 	.word	0x2001c00c
300040e8:	0000c245 	.word	0x0000c245
300040ec:	30009f3d 	.word	0x30009f3d
300040f0:	30009f25 	.word	0x30009f25
300040f4:	3000cb90 	.word	0x3000cb90
300040f8:	3000cb80 	.word	0x3000cb80

300040fc <BOOT_OTA_LoadIMG2>:
300040fc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004100:	4b6e      	ldr	r3, [pc, #440]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c0>)
30004102:	b08d      	sub	sp, #52	; 0x34
30004104:	4a6e      	ldr	r2, [pc, #440]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1c4>)
30004106:	f103 040c 	add.w	r4, r3, #12
3000410a:	9003      	str	r0, [sp, #12]
3000410c:	f8d2 8000 	ldr.w	r8, [r2]
30004110:	ae06      	add	r6, sp, #24
30004112:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30004116:	ab09      	add	r3, sp, #36	; 0x24
30004118:	e886 0007 	stmia.w	r6, {r0, r1, r2}
3000411c:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30004120:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30004124:	4641      	mov	r1, r8
30004126:	4867      	ldr	r0, [pc, #412]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
30004128:	f44f 72b2 	mov.w	r2, #356	; 0x164
3000412c:	f001 fdf6 	bl	30005d1c <BOOT_ImgCopy>
30004130:	4641      	mov	r1, r8
30004132:	4864      	ldr	r0, [pc, #400]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
30004134:	f000 fd5c 	bl	30004bf0 <BOOT_CertificateCheck>
30004138:	2800      	cmp	r0, #0
3000413a:	f040 80b7 	bne.w	300042ac <BOOT_OTA_LoadIMG2+0x1b0>
3000413e:	4604      	mov	r4, r0
30004140:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004144:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004148:	485f      	ldr	r0, [pc, #380]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1cc>)
3000414a:	f001 fde7 	bl	30005d1c <BOOT_ImgCopy>
3000414e:	4b5f      	ldr	r3, [pc, #380]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d0>)
30004150:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004154:	4798      	blx	r3
30004156:	2800      	cmp	r0, #0
30004158:	f040 809e 	bne.w	30004298 <BOOT_OTA_LoadIMG2+0x19c>
3000415c:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30004160:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30004310 <BOOT_OTA_LoadIMG2+0x214>
30004164:	4a5a      	ldr	r2, [pc, #360]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1d4>)
30004166:	2000      	movs	r0, #0
30004168:	4445      	add	r5, r8
3000416a:	4659      	mov	r1, fp
3000416c:	4f59      	ldr	r7, [pc, #356]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1d8>)
3000416e:	f04f 0801 	mov.w	r8, #1
30004172:	462b      	mov	r3, r5
30004174:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30004314 <BOOT_OTA_LoadIMG2+0x218>
30004178:	47b8      	blx	r7
3000417a:	2101      	movs	r1, #1
3000417c:	2000      	movs	r0, #0
3000417e:	4b56      	ldr	r3, [pc, #344]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1dc>)
30004180:	4798      	blx	r3
30004182:	2301      	movs	r3, #1
30004184:	4952      	ldr	r1, [pc, #328]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1d4>)
30004186:	4658      	mov	r0, fp
30004188:	461a      	mov	r2, r3
3000418a:	f7ff ff6b 	bl	30004064 <BOOT_OTFCheck>
3000418e:	47c8      	blx	r9
30004190:	2800      	cmp	r0, #0
30004192:	bf0c      	ite	eq
30004194:	46aa      	moveq	sl, r5
30004196:	46da      	movne	sl, fp
30004198:	4633      	mov	r3, r6
3000419a:	2203      	movs	r2, #3
3000419c:	4651      	mov	r1, sl
3000419e:	484f      	ldr	r0, [pc, #316]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e0>)
300041a0:	f8cd 8000 	str.w	r8, [sp]
300041a4:	f7ff fd14 	bl	30003bd0 <BOOT_LoadSubImage>
300041a8:	2800      	cmp	r0, #0
300041aa:	d070      	beq.n	3000428e <BOOT_OTA_LoadIMG2+0x192>
300041ac:	4e4b      	ldr	r6, [pc, #300]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e0>)
300041ae:	47c8      	blx	r9
300041b0:	2800      	cmp	r0, #0
300041b2:	bf08      	it	eq
300041b4:	46ab      	moveq	fp, r5
300041b6:	4f47      	ldr	r7, [pc, #284]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1d8>)
300041b8:	68f2      	ldr	r2, [r6, #12]
300041ba:	4640      	mov	r0, r8
300041bc:	6873      	ldr	r3, [r6, #4]
300041be:	f8c6 b000 	str.w	fp, [r6]
300041c2:	4413      	add	r3, r2
300041c4:	6972      	ldr	r2, [r6, #20]
300041c6:	f8df b150 	ldr.w	fp, [pc, #336]	; 30004318 <BOOT_OTA_LoadIMG2+0x21c>
300041ca:	441a      	add	r2, r3
300041cc:	4b44      	ldr	r3, [pc, #272]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1e4>)
300041ce:	eb0a 0102 	add.w	r1, sl, r2
300041d2:	61b1      	str	r1, [r6, #24]
300041d4:	1e51      	subs	r1, r2, #1
300041d6:	400b      	ands	r3, r1
300041d8:	4659      	mov	r1, fp
300041da:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300041de:	441d      	add	r5, r3
300041e0:	1a9b      	subs	r3, r3, r2
300041e2:	4a40      	ldr	r2, [pc, #256]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1e8>)
300041e4:	61f3      	str	r3, [r6, #28]
300041e6:	462b      	mov	r3, r5
300041e8:	47b8      	blx	r7
300041ea:	4641      	mov	r1, r8
300041ec:	4640      	mov	r0, r8
300041ee:	4b3a      	ldr	r3, [pc, #232]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1dc>)
300041f0:	4798      	blx	r3
300041f2:	4642      	mov	r2, r8
300041f4:	2302      	movs	r3, #2
300041f6:	493b      	ldr	r1, [pc, #236]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1e8>)
300041f8:	4658      	mov	r0, fp
300041fa:	f7ff ff33 	bl	30004064 <BOOT_OTFCheck>
300041fe:	47c8      	blx	r9
30004200:	2800      	cmp	r0, #0
30004202:	bf0c      	ite	eq
30004204:	462f      	moveq	r7, r5
30004206:	465f      	movne	r7, fp
30004208:	ab09      	add	r3, sp, #36	; 0x24
3000420a:	2203      	movs	r2, #3
3000420c:	f106 0020 	add.w	r0, r6, #32
30004210:	4639      	mov	r1, r7
30004212:	f8cd 8000 	str.w	r8, [sp]
30004216:	f7ff fcdb 	bl	30003bd0 <BOOT_LoadSubImage>
3000421a:	2800      	cmp	r0, #0
3000421c:	d037      	beq.n	3000428e <BOOT_OTA_LoadIMG2+0x192>
3000421e:	47c8      	blx	r9
30004220:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30004222:	6a73      	ldr	r3, [r6, #36]	; 0x24
30004224:	2800      	cmp	r0, #0
30004226:	bf08      	it	eq
30004228:	46ab      	moveq	fp, r5
3000422a:	4827      	ldr	r0, [pc, #156]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1cc>)
3000422c:	4413      	add	r3, r2
3000422e:	6b72      	ldr	r2, [r6, #52]	; 0x34
30004230:	f8c6 b020 	str.w	fp, [r6, #32]
30004234:	4413      	add	r3, r2
30004236:	4a2a      	ldr	r2, [pc, #168]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1e4>)
30004238:	1e59      	subs	r1, r3, #1
3000423a:	441f      	add	r7, r3
3000423c:	400a      	ands	r2, r1
3000423e:	2100      	movs	r1, #0
30004240:	63b7      	str	r7, [r6, #56]	; 0x38
30004242:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004246:	9100      	str	r1, [sp, #0]
30004248:	1ad1      	subs	r1, r2, r3
3000424a:	2900      	cmp	r1, #0
3000424c:	63f1      	str	r1, [r6, #60]	; 0x3c
3000424e:	4631      	mov	r1, r6
30004250:	bf18      	it	ne
30004252:	4613      	movne	r3, r2
30004254:	4a24      	ldr	r2, [pc, #144]	; (300042e8 <BOOT_OTA_LoadIMG2+0x1ec>)
30004256:	441d      	add	r5, r3
30004258:	4b1a      	ldr	r3, [pc, #104]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1c8>)
3000425a:	6015      	str	r5, [r2, #0]
3000425c:	bf0c      	ite	eq
3000425e:	2207      	moveq	r2, #7
30004260:	2208      	movne	r2, #8
30004262:	f000 fbb1 	bl	300049c8 <BOOT_SignatureCheck>
30004266:	4b21      	ldr	r3, [pc, #132]	; (300042ec <BOOT_OTA_LoadIMG2+0x1f0>)
30004268:	4605      	mov	r5, r0
3000426a:	689b      	ldr	r3, [r3, #8]
3000426c:	079b      	lsls	r3, r3, #30
3000426e:	d50a      	bpl.n	30004286 <BOOT_OTA_LoadIMG2+0x18a>
30004270:	4b1f      	ldr	r3, [pc, #124]	; (300042f0 <BOOT_OTA_LoadIMG2+0x1f4>)
30004272:	4a20      	ldr	r2, [pc, #128]	; (300042f4 <BOOT_OTA_LoadIMG2+0x1f8>)
30004274:	9903      	ldr	r1, [sp, #12]
30004276:	2800      	cmp	r0, #0
30004278:	bf18      	it	ne
3000427a:	461a      	movne	r2, r3
3000427c:	481e      	ldr	r0, [pc, #120]	; (300042f8 <BOOT_OTA_LoadIMG2+0x1fc>)
3000427e:	3101      	adds	r1, #1
30004280:	462b      	mov	r3, r5
30004282:	f008 fa45 	bl	3000c710 <__DiagPrintf_veneer>
30004286:	b11d      	cbz	r5, 30004290 <BOOT_OTA_LoadIMG2+0x194>
30004288:	481c      	ldr	r0, [pc, #112]	; (300042fc <BOOT_OTA_LoadIMG2+0x200>)
3000428a:	f008 fa41 	bl	3000c710 <__DiagPrintf_veneer>
3000428e:	24ff      	movs	r4, #255	; 0xff
30004290:	4620      	mov	r0, r4
30004292:	b00d      	add	sp, #52	; 0x34
30004294:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004298:	2208      	movs	r2, #8
3000429a:	4919      	ldr	r1, [pc, #100]	; (30004300 <BOOT_OTA_LoadIMG2+0x204>)
3000429c:	4b19      	ldr	r3, [pc, #100]	; (30004304 <BOOT_OTA_LoadIMG2+0x208>)
3000429e:	a804      	add	r0, sp, #16
300042a0:	4798      	blx	r3
300042a2:	a904      	add	r1, sp, #16
300042a4:	2001      	movs	r0, #1
300042a6:	4b18      	ldr	r3, [pc, #96]	; (30004308 <BOOT_OTA_LoadIMG2+0x20c>)
300042a8:	4798      	blx	r3
300042aa:	e757      	b.n	3000415c <BOOT_OTA_LoadIMG2+0x60>
300042ac:	24ff      	movs	r4, #255	; 0xff
300042ae:	4817      	ldr	r0, [pc, #92]	; (3000430c <BOOT_OTA_LoadIMG2+0x210>)
300042b0:	f008 fa2e 	bl	3000c710 <__DiagPrintf_veneer>
300042b4:	4620      	mov	r0, r4
300042b6:	b00d      	add	sp, #52	; 0x34
300042b8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300042bc:	3000e880 	.word	0x3000e880
300042c0:	30010590 	.word	0x30010590
300042c4:	30010234 	.word	0x30010234
300042c8:	30010398 	.word	0x30010398
300042cc:	0000c129 	.word	0x0000c129
300042d0:	0cffffe0 	.word	0x0cffffe0
300042d4:	30009fb9 	.word	0x30009fb9
300042d8:	30009ff9 	.word	0x30009ff9
300042dc:	30010540 	.word	0x30010540
300042e0:	fffff000 	.word	0xfffff000
300042e4:	0dffffe0 	.word	0x0dffffe0
300042e8:	3000f3f8 	.word	0x3000f3f8
300042ec:	2001c00c 	.word	0x2001c00c
300042f0:	3000cbac 	.word	0x3000cbac
300042f4:	3000cba4 	.word	0x3000cba4
300042f8:	3000cbdc 	.word	0x3000cbdc
300042fc:	3000cc10 	.word	0x3000cc10
30004300:	300103b8 	.word	0x300103b8
30004304:	00012a1d 	.word	0x00012a1d
30004308:	30009f09 	.word	0x30009f09
3000430c:	3000cbb4 	.word	0x3000cbb4
30004310:	0c000000 	.word	0x0c000000
30004314:	3000a219 	.word	0x3000a219
30004318:	0d000000 	.word	0x0d000000

3000431c <BOOT_OTA_Region_Init>:
3000431c:	4b24      	ldr	r3, [pc, #144]	; (300043b0 <BOOT_OTA_Region_Init+0x94>)
3000431e:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004322:	4798      	blx	r3
30004324:	4b23      	ldr	r3, [pc, #140]	; (300043b4 <BOOT_OTA_Region_Init+0x98>)
30004326:	4a24      	ldr	r2, [pc, #144]	; (300043b8 <BOOT_OTA_Region_Init+0x9c>)
30004328:	4924      	ldr	r1, [pc, #144]	; (300043bc <BOOT_OTA_Region_Init+0xa0>)
3000432a:	2800      	cmp	r0, #0
3000432c:	bf08      	it	eq
3000432e:	461a      	moveq	r2, r3
30004330:	6813      	ldr	r3, [r2, #0]
30004332:	2bff      	cmp	r3, #255	; 0xff
30004334:	e9d1 0400 	ldrd	r0, r4, [r1]
30004338:	d029      	beq.n	3000438e <BOOT_OTA_Region_Init+0x72>
3000433a:	2500      	movs	r5, #0
3000433c:	e9d1 e804 	ldrd	lr, r8, [r1, #16]
30004340:	46ac      	mov	ip, r5
30004342:	462f      	mov	r7, r5
30004344:	462e      	mov	r6, r5
30004346:	e00c      	b.n	30004362 <BOOT_OTA_Region_Init+0x46>
30004348:	2b03      	cmp	r3, #3
3000434a:	d028      	beq.n	3000439e <BOOT_OTA_Region_Init+0x82>
3000434c:	2b04      	cmp	r3, #4
3000434e:	d029      	beq.n	300043a4 <BOOT_OTA_Region_Init+0x88>
30004350:	2b05      	cmp	r3, #5
30004352:	d102      	bne.n	3000435a <BOOT_OTA_Region_Init+0x3e>
30004354:	f8d2 8004 	ldr.w	r8, [r2, #4]
30004358:	2501      	movs	r5, #1
3000435a:	f852 3f0c 	ldr.w	r3, [r2, #12]!
3000435e:	2bff      	cmp	r3, #255	; 0xff
30004360:	d007      	beq.n	30004372 <BOOT_OTA_Region_Init+0x56>
30004362:	2b02      	cmp	r3, #2
30004364:	d1f0      	bne.n	30004348 <BOOT_OTA_Region_Init+0x2c>
30004366:	6850      	ldr	r0, [r2, #4]
30004368:	2601      	movs	r6, #1
3000436a:	f852 3f0c 	ldr.w	r3, [r2, #12]!
3000436e:	2bff      	cmp	r3, #255	; 0xff
30004370:	d1f7      	bne.n	30004362 <BOOT_OTA_Region_Init+0x46>
30004372:	b106      	cbz	r6, 30004376 <BOOT_OTA_Region_Init+0x5a>
30004374:	6008      	str	r0, [r1, #0]
30004376:	b107      	cbz	r7, 3000437a <BOOT_OTA_Region_Init+0x5e>
30004378:	604c      	str	r4, [r1, #4]
3000437a:	f1bc 0f00 	cmp.w	ip, #0
3000437e:	d001      	beq.n	30004384 <BOOT_OTA_Region_Init+0x68>
30004380:	f8c1 e010 	str.w	lr, [r1, #16]
30004384:	e9d1 0400 	ldrd	r0, r4, [r1]
30004388:	b10d      	cbz	r5, 3000438e <BOOT_OTA_Region_Init+0x72>
3000438a:	f8c1 8014 	str.w	r8, [r1, #20]
3000438e:	f500 5080 	add.w	r0, r0, #4096	; 0x1000
30004392:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
30004396:	e9c1 0402 	strd	r0, r4, [r1, #8]
3000439a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000439e:	6854      	ldr	r4, [r2, #4]
300043a0:	2701      	movs	r7, #1
300043a2:	e7da      	b.n	3000435a <BOOT_OTA_Region_Init+0x3e>
300043a4:	f8d2 e004 	ldr.w	lr, [r2, #4]
300043a8:	f04f 0c01 	mov.w	ip, #1
300043ac:	e7d5      	b.n	3000435a <BOOT_OTA_Region_Init+0x3e>
300043ae:	bf00      	nop
300043b0:	3000a219 	.word	0x3000a219
300043b4:	3000f520 	.word	0x3000f520
300043b8:	3000f598 	.word	0x3000f598
300043bc:	30010528 	.word	0x30010528

300043c0 <BOOT_OTA_IMG2>:
300043c0:	f7ff be9c 	b.w	300040fc <BOOT_OTA_LoadIMG2>

300043c4 <BOOT_OTA_AP>:
300043c4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300043c8:	4c31      	ldr	r4, [pc, #196]	; (30004490 <BOOT_OTA_AP+0xcc>)
300043ca:	b0ed      	sub	sp, #436	; 0x1b4
300043cc:	4b31      	ldr	r3, [pc, #196]	; (30004494 <BOOT_OTA_AP+0xd0>)
300043ce:	ad05      	add	r5, sp, #20
300043d0:	af03      	add	r7, sp, #12
300043d2:	681b      	ldr	r3, [r3, #0]
300043d4:	4e30      	ldr	r6, [pc, #192]	; (30004498 <BOOT_OTA_AP+0xd4>)
300043d6:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300043da:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
300043de:	340c      	adds	r4, #12
300043e0:	e885 0007 	stmia.w	r5, {r0, r1, r2}
300043e4:	e894 0003 	ldmia.w	r4, {r0, r1}
300043e8:	f44f 72c8 	mov.w	r2, #400	; 0x190
300043ec:	e887 0003 	stmia.w	r7, {r0, r1}
300043f0:	4619      	mov	r1, r3
300043f2:	a808      	add	r0, sp, #32
300043f4:	f001 fc92 	bl	30005d1c <BOOT_ImgCopy>
300043f8:	2201      	movs	r2, #1
300043fa:	462b      	mov	r3, r5
300043fc:	4641      	mov	r1, r8
300043fe:	9200      	str	r2, [sp, #0]
30004400:	4630      	mov	r0, r6
30004402:	2203      	movs	r2, #3
30004404:	f7ff fbe4 	bl	30003bd0 <BOOT_LoadSubImage>
30004408:	2801      	cmp	r0, #1
3000440a:	d122      	bne.n	30004452 <BOOT_OTA_AP+0x8e>
3000440c:	f04f 0902 	mov.w	r9, #2
30004410:	4604      	mov	r4, r0
30004412:	4b22      	ldr	r3, [pc, #136]	; (3000449c <BOOT_OTA_AP+0xd8>)
30004414:	a808      	add	r0, sp, #32
30004416:	2203      	movs	r2, #3
30004418:	4631      	mov	r1, r6
3000441a:	f8cd 9000 	str.w	r9, [sp]
3000441e:	f000 fad3 	bl	300049c8 <BOOT_SignatureCheck>
30004422:	4605      	mov	r5, r0
30004424:	bb30      	cbnz	r0, 30004474 <BOOT_OTA_AP+0xb0>
30004426:	464a      	mov	r2, r9
30004428:	463b      	mov	r3, r7
3000442a:	4641      	mov	r1, r8
3000442c:	4630      	mov	r0, r6
3000442e:	9400      	str	r4, [sp, #0]
30004430:	f7ff fbce 	bl	30003bd0 <BOOT_LoadSubImage>
30004434:	6871      	ldr	r1, [r6, #4]
30004436:	68f3      	ldr	r3, [r6, #12]
30004438:	4622      	mov	r2, r4
3000443a:	f106 0010 	add.w	r0, r6, #16
3000443e:	4419      	add	r1, r3
30004440:	4441      	add	r1, r8
30004442:	f7ff fc79 	bl	30003d38 <BOOT_LoadAPSubImage>
30004446:	2801      	cmp	r0, #1
30004448:	d10b      	bne.n	30004462 <BOOT_OTA_AP+0x9e>
3000444a:	4628      	mov	r0, r5
3000444c:	b06d      	add	sp, #436	; 0x1b4
3000444e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004452:	25ff      	movs	r5, #255	; 0xff
30004454:	4812      	ldr	r0, [pc, #72]	; (300044a0 <BOOT_OTA_AP+0xdc>)
30004456:	f008 f95b 	bl	3000c710 <__DiagPrintf_veneer>
3000445a:	4628      	mov	r0, r5
3000445c:	b06d      	add	sp, #436	; 0x1b4
3000445e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004462:	4b10      	ldr	r3, [pc, #64]	; (300044a4 <BOOT_OTA_AP+0xe0>)
30004464:	681b      	ldr	r3, [r3, #0]
30004466:	079b      	lsls	r3, r3, #30
30004468:	d40c      	bmi.n	30004484 <BOOT_OTA_AP+0xc0>
3000446a:	25ff      	movs	r5, #255	; 0xff
3000446c:	4628      	mov	r0, r5
3000446e:	b06d      	add	sp, #436	; 0x1b4
30004470:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004474:	25ff      	movs	r5, #255	; 0xff
30004476:	480c      	ldr	r0, [pc, #48]	; (300044a8 <BOOT_OTA_AP+0xe4>)
30004478:	f008 f94a 	bl	3000c710 <__DiagPrintf_veneer>
3000447c:	4628      	mov	r0, r5
3000447e:	b06d      	add	sp, #436	; 0x1b4
30004480:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004484:	4809      	ldr	r0, [pc, #36]	; (300044ac <BOOT_OTA_AP+0xe8>)
30004486:	25ff      	movs	r5, #255	; 0xff
30004488:	f008 f942 	bl	3000c710 <__DiagPrintf_veneer>
3000448c:	e7dd      	b.n	3000444a <BOOT_OTA_AP+0x86>
3000448e:	bf00      	nop
30004490:	3000e898 	.word	0x3000e898
30004494:	3000f3f8 	.word	0x3000f3f8
30004498:	30010540 	.word	0x30010540
3000449c:	30010234 	.word	0x30010234
300044a0:	3000cc58 	.word	0x3000cc58
300044a4:	2001c00c 	.word	0x2001c00c
300044a8:	3000cc34 	.word	0x3000cc34
300044ac:	3000cc7c 	.word	0x3000cc7c

300044b0 <BOOT_LoadRDPImg>:
300044b0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300044b4:	4d50      	ldr	r5, [pc, #320]	; (300045f8 <BOOT_LoadRDPImg+0x148>)
300044b6:	b0d7      	sub	sp, #348	; 0x15c
300044b8:	4613      	mov	r3, r2
300044ba:	468b      	mov	fp, r1
300044bc:	4604      	mov	r4, r0
300044be:	aa04      	add	r2, sp, #16
300044c0:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300044c4:	e895 0003 	ldmia.w	r5, {r0, r1}
300044c8:	2500      	movs	r5, #0
300044ca:	e882 0003 	stmia.w	r2, {r0, r1}
300044ce:	4619      	mov	r1, r3
300044d0:	f44f 72c8 	mov.w	r2, #400	; 0x190
300044d4:	4620      	mov	r0, r4
300044d6:	f001 fc21 	bl	30005d1c <BOOT_ImgCopy>
300044da:	ab04      	add	r3, sp, #16
300044dc:	2202      	movs	r2, #2
300044de:	4641      	mov	r1, r8
300044e0:	4658      	mov	r0, fp
300044e2:	9500      	str	r5, [sp, #0]
300044e4:	f7ff fb74 	bl	30003bd0 <BOOT_LoadSubImage>
300044e8:	2801      	cmp	r0, #1
300044ea:	d070      	beq.n	300045ce <BOOT_LoadRDPImg+0x11e>
300044ec:	4b43      	ldr	r3, [pc, #268]	; (300045fc <BOOT_LoadRDPImg+0x14c>)
300044ee:	4628      	mov	r0, r5
300044f0:	4798      	blx	r3
300044f2:	2101      	movs	r1, #1
300044f4:	4b42      	ldr	r3, [pc, #264]	; (30004600 <BOOT_LoadRDPImg+0x150>)
300044f6:	4628      	mov	r0, r5
300044f8:	4798      	blx	r3
300044fa:	af06      	add	r7, sp, #24
300044fc:	2120      	movs	r1, #32
300044fe:	4b41      	ldr	r3, [pc, #260]	; (30004604 <BOOT_LoadRDPImg+0x154>)
30004500:	a80e      	add	r0, sp, #56	; 0x38
30004502:	f8df 9118 	ldr.w	r9, [pc, #280]	; 3000461c <BOOT_LoadRDPImg+0x16c>
30004506:	4798      	blx	r3
30004508:	f8df a114 	ldr.w	sl, [pc, #276]	; 30004620 <BOOT_LoadRDPImg+0x170>
3000450c:	f104 0120 	add.w	r1, r4, #32
30004510:	2210      	movs	r2, #16
30004512:	a80a      	add	r0, sp, #40	; 0x28
30004514:	47d0      	blx	sl
30004516:	9503      	str	r5, [sp, #12]
30004518:	4641      	mov	r1, r8
3000451a:	a816      	add	r0, sp, #88	; 0x58
3000451c:	2210      	movs	r2, #16
3000451e:	f001 fbfd 	bl	30005d1c <BOOT_ImgCopy>
30004522:	2310      	movs	r3, #16
30004524:	aa0a      	add	r2, sp, #40	; 0x28
30004526:	a816      	add	r0, sp, #88	; 0x58
30004528:	4619      	mov	r1, r3
3000452a:	9700      	str	r7, [sp, #0]
3000452c:	47c8      	blx	r9
3000452e:	683b      	ldr	r3, [r7, #0]
30004530:	4a35      	ldr	r2, [pc, #212]	; (30004608 <BOOT_LoadRDPImg+0x158>)
30004532:	4293      	cmp	r3, r2
30004534:	4b35      	ldr	r3, [pc, #212]	; (3000460c <BOOT_LoadRDPImg+0x15c>)
30004536:	d14e      	bne.n	300045d6 <BOOT_LoadRDPImg+0x126>
30004538:	687a      	ldr	r2, [r7, #4]
3000453a:	4935      	ldr	r1, [pc, #212]	; (30004610 <BOOT_LoadRDPImg+0x160>)
3000453c:	689b      	ldr	r3, [r3, #8]
3000453e:	428a      	cmp	r2, r1
30004540:	f003 0302 	and.w	r3, r3, #2
30004544:	d155      	bne.n	300045f2 <BOOT_LoadRDPImg+0x142>
30004546:	68fe      	ldr	r6, [r7, #12]
30004548:	68bd      	ldr	r5, [r7, #8]
3000454a:	3e20      	subs	r6, #32
3000454c:	3520      	adds	r5, #32
3000454e:	2b00      	cmp	r3, #0
30004550:	d133      	bne.n	300045ba <BOOT_LoadRDPImg+0x10a>
30004552:	f1bb 0f00 	cmp.w	fp, #0
30004556:	d006      	beq.n	30004566 <BOOT_LoadRDPImg+0xb6>
30004558:	9a03      	ldr	r2, [sp, #12]
3000455a:	f10b 0304 	add.w	r3, fp, #4
3000455e:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30004562:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30004566:	b1dd      	cbz	r5, 300045a0 <BOOT_LoadRDPImg+0xf0>
30004568:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
3000456c:	462c      	mov	r4, r5
3000456e:	4641      	mov	r1, r8
30004570:	a816      	add	r0, sp, #88	; 0x58
30004572:	bf28      	it	cs
30004574:	f44f 7480 	movcs.w	r4, #256	; 0x100
30004578:	4622      	mov	r2, r4
3000457a:	44a0      	add	r8, r4
3000457c:	f001 fbce 	bl	30005d1c <BOOT_ImgCopy>
30004580:	aa0a      	add	r2, sp, #40	; 0x28
30004582:	4621      	mov	r1, r4
30004584:	2310      	movs	r3, #16
30004586:	9600      	str	r6, [sp, #0]
30004588:	a816      	add	r0, sp, #88	; 0x58
3000458a:	47c8      	blx	r9
3000458c:	ab16      	add	r3, sp, #88	; 0x58
3000458e:	f1a4 0110 	sub.w	r1, r4, #16
30004592:	2210      	movs	r2, #16
30004594:	a80a      	add	r0, sp, #40	; 0x28
30004596:	4426      	add	r6, r4
30004598:	4419      	add	r1, r3
3000459a:	47d0      	blx	sl
3000459c:	1b2d      	subs	r5, r5, r4
3000459e:	d1e3      	bne.n	30004568 <BOOT_LoadRDPImg+0xb8>
300045a0:	9b03      	ldr	r3, [sp, #12]
300045a2:	b13b      	cbz	r3, 300045b4 <BOOT_LoadRDPImg+0x104>
300045a4:	2100      	movs	r1, #0
300045a6:	4b16      	ldr	r3, [pc, #88]	; (30004600 <BOOT_LoadRDPImg+0x150>)
300045a8:	4608      	mov	r0, r1
300045aa:	4798      	blx	r3
300045ac:	2002      	movs	r0, #2
300045ae:	b057      	add	sp, #348	; 0x15c
300045b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300045b4:	2301      	movs	r3, #1
300045b6:	9303      	str	r3, [sp, #12]
300045b8:	e7ae      	b.n	30004518 <BOOT_LoadRDPImg+0x68>
300045ba:	a804      	add	r0, sp, #16
300045bc:	9903      	ldr	r1, [sp, #12]
300045be:	462b      	mov	r3, r5
300045c0:	4632      	mov	r2, r6
300045c2:	f850 1021 	ldr.w	r1, [r0, r1, lsl #2]
300045c6:	4813      	ldr	r0, [pc, #76]	; (30004614 <BOOT_LoadRDPImg+0x164>)
300045c8:	f008 f8a2 	bl	3000c710 <__DiagPrintf_veneer>
300045cc:	e7c1      	b.n	30004552 <BOOT_LoadRDPImg+0xa2>
300045ce:	2002      	movs	r0, #2
300045d0:	b057      	add	sp, #348	; 0x15c
300045d2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300045d6:	6898      	ldr	r0, [r3, #8]
300045d8:	f000 0002 	and.w	r0, r0, #2
300045dc:	2800      	cmp	r0, #0
300045de:	d0e6      	beq.n	300045ae <BOOT_LoadRDPImg+0xfe>
300045e0:	9b03      	ldr	r3, [sp, #12]
300045e2:	aa04      	add	r2, sp, #16
300045e4:	480c      	ldr	r0, [pc, #48]	; (30004618 <BOOT_LoadRDPImg+0x168>)
300045e6:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
300045ea:	f008 f891 	bl	3000c710 <__DiagPrintf_veneer>
300045ee:	2000      	movs	r0, #0
300045f0:	e7dd      	b.n	300045ae <BOOT_LoadRDPImg+0xfe>
300045f2:	4618      	mov	r0, r3
300045f4:	e7f2      	b.n	300045dc <BOOT_LoadRDPImg+0x12c>
300045f6:	bf00      	nop
300045f8:	3000e8ac 	.word	0x3000e8ac
300045fc:	00002429 	.word	0x00002429
30004600:	00002479 	.word	0x00002479
30004604:	00004641 	.word	0x00004641
30004608:	35393138 	.word	0x35393138
3000460c:	2001c00c 	.word	0x2001c00c
30004610:	31313738 	.word	0x31313738
30004614:	3000cadc 	.word	0x3000cadc
30004618:	3000cab4 	.word	0x3000cab4
3000461c:	000046f5 	.word	0x000046f5
30004620:	00012a1d 	.word	0x00012a1d

30004624 <BOOT_LoadBL32RDPImg>:
30004624:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004628:	4b35      	ldr	r3, [pc, #212]	; (30004700 <BOOT_LoadBL32RDPImg+0xdc>)
3000462a:	b0d1      	sub	sp, #324	; 0x144
3000462c:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 3000471c <BOOT_LoadBL32RDPImg+0xf8>
30004630:	4607      	mov	r7, r0
30004632:	f10d 0810 	add.w	r8, sp, #16
30004636:	460e      	mov	r6, r1
30004638:	4615      	mov	r5, r2
3000463a:	ac02      	add	r4, sp, #8
3000463c:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30004720 <BOOT_LoadBL32RDPImg+0xfc>
30004640:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30004724 <BOOT_LoadBL32RDPImg+0x100>
30004644:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004646:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
3000464a:	e89c 0003 	ldmia.w	ip, {r0, r1}
3000464e:	e884 0003 	stmia.w	r4, {r0, r1}
30004652:	2000      	movs	r0, #0
30004654:	47c8      	blx	r9
30004656:	2101      	movs	r1, #1
30004658:	2000      	movs	r0, #0
3000465a:	47d0      	blx	sl
3000465c:	2120      	movs	r1, #32
3000465e:	4b29      	ldr	r3, [pc, #164]	; (30004704 <BOOT_LoadBL32RDPImg+0xe0>)
30004660:	eb0d 0001 	add.w	r0, sp, r1
30004664:	4798      	blx	r3
30004666:	4b28      	ldr	r3, [pc, #160]	; (30004708 <BOOT_LoadBL32RDPImg+0xe4>)
30004668:	689b      	ldr	r3, [r3, #8]
3000466a:	079b      	lsls	r3, r3, #30
3000466c:	d436      	bmi.n	300046dc <BOOT_LoadBL32RDPImg+0xb8>
3000466e:	b1fd      	cbz	r5, 300046b0 <BOOT_LoadBL32RDPImg+0x8c>
30004670:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30004728 <BOOT_LoadBL32RDPImg+0x104>
30004674:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 3000472c <BOOT_LoadBL32RDPImg+0x108>
30004678:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
3000467c:	462c      	mov	r4, r5
3000467e:	4639      	mov	r1, r7
30004680:	a810      	add	r0, sp, #64	; 0x40
30004682:	bf28      	it	cs
30004684:	f44f 7480 	movcs.w	r4, #256	; 0x100
30004688:	4622      	mov	r2, r4
3000468a:	4427      	add	r7, r4
3000468c:	f001 fb46 	bl	30005d1c <BOOT_ImgCopy>
30004690:	4642      	mov	r2, r8
30004692:	4621      	mov	r1, r4
30004694:	2310      	movs	r3, #16
30004696:	9600      	str	r6, [sp, #0]
30004698:	a810      	add	r0, sp, #64	; 0x40
3000469a:	47d8      	blx	fp
3000469c:	ab10      	add	r3, sp, #64	; 0x40
3000469e:	f1a4 0110 	sub.w	r1, r4, #16
300046a2:	2210      	movs	r2, #16
300046a4:	4640      	mov	r0, r8
300046a6:	4426      	add	r6, r4
300046a8:	4419      	add	r1, r3
300046aa:	47c8      	blx	r9
300046ac:	1b2d      	subs	r5, r5, r4
300046ae:	d1e3      	bne.n	30004678 <BOOT_LoadBL32RDPImg+0x54>
300046b0:	f856 2c20 	ldr.w	r2, [r6, #-32]
300046b4:	3e20      	subs	r6, #32
300046b6:	4b15      	ldr	r3, [pc, #84]	; (3000470c <BOOT_LoadBL32RDPImg+0xe8>)
300046b8:	429a      	cmp	r2, r3
300046ba:	d108      	bne.n	300046ce <BOOT_LoadBL32RDPImg+0xaa>
300046bc:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
300046c0:	6872      	ldr	r2, [r6, #4]
300046c2:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
300046c6:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
300046ca:	429a      	cmp	r2, r3
300046cc:	d00d      	beq.n	300046ea <BOOT_LoadBL32RDPImg+0xc6>
300046ce:	4810      	ldr	r0, [pc, #64]	; (30004710 <BOOT_LoadBL32RDPImg+0xec>)
300046d0:	f008 f81e 	bl	3000c710 <__DiagPrintf_veneer>
300046d4:	2000      	movs	r0, #0
300046d6:	b051      	add	sp, #324	; 0x144
300046d8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300046dc:	4621      	mov	r1, r4
300046de:	462b      	mov	r3, r5
300046e0:	4632      	mov	r2, r6
300046e2:	480c      	ldr	r0, [pc, #48]	; (30004714 <BOOT_LoadBL32RDPImg+0xf0>)
300046e4:	f008 f814 	bl	3000c710 <__DiagPrintf_veneer>
300046e8:	e7c1      	b.n	3000466e <BOOT_LoadBL32RDPImg+0x4a>
300046ea:	480b      	ldr	r0, [pc, #44]	; (30004718 <BOOT_LoadBL32RDPImg+0xf4>)
300046ec:	f008 f810 	bl	3000c710 <__DiagPrintf_veneer>
300046f0:	2100      	movs	r1, #0
300046f2:	4608      	mov	r0, r1
300046f4:	47d0      	blx	sl
300046f6:	2001      	movs	r0, #1
300046f8:	b051      	add	sp, #324	; 0x144
300046fa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300046fe:	bf00      	nop
30004700:	3000ccfc 	.word	0x3000ccfc
30004704:	00004641 	.word	0x00004641
30004708:	2001c00c 	.word	0x2001c00c
3000470c:	35393138 	.word	0x35393138
30004710:	3000cce0 	.word	0x3000cce0
30004714:	3000cadc 	.word	0x3000cadc
30004718:	3000ccec 	.word	0x3000ccec
3000471c:	3000cd10 	.word	0x3000cd10
30004720:	00002429 	.word	0x00002429
30004724:	00002479 	.word	0x00002479
30004728:	000046f5 	.word	0x000046f5
3000472c:	00012a1d 	.word	0x00012a1d

30004730 <_BOOT_Validate_ImgHash>:
30004730:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004734:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30004738:	4605      	mov	r5, r0
3000473a:	4690      	mov	r8, r2
3000473c:	22e0      	movs	r2, #224	; 0xe0
3000473e:	9103      	str	r1, [sp, #12]
30004740:	2100      	movs	r1, #0
30004742:	a818      	add	r0, sp, #96	; 0x60
30004744:	461c      	mov	r4, r3
30004746:	f88d 101f 	strb.w	r1, [sp, #31]
3000474a:	f007 ffb1 	bl	3000c6b0 <____wrap_memset_veneer>
3000474e:	4628      	mov	r0, r5
30004750:	f10d 021f 	add.w	r2, sp, #31
30004754:	f10d 011e 	add.w	r1, sp, #30
30004758:	f007 ffea 	bl	3000c730 <__SBOOT_GetMdType_veneer>
3000475c:	9001      	str	r0, [sp, #4]
3000475e:	2800      	cmp	r0, #0
30004760:	d166      	bne.n	30004830 <_BOOT_Validate_ImgHash+0x100>
30004762:	f89d 301f 	ldrb.w	r3, [sp, #31]
30004766:	2b00      	cmp	r3, #0
30004768:	d067      	beq.n	3000483a <_BOOT_Validate_ImgHash+0x10a>
3000476a:	2101      	movs	r1, #1
3000476c:	4605      	mov	r5, r0
3000476e:	4b80      	ldr	r3, [pc, #512]	; (30004970 <_BOOT_Validate_ImgHash+0x240>)
30004770:	4608      	mov	r0, r1
30004772:	4798      	blx	r3
30004774:	4629      	mov	r1, r5
30004776:	ab18      	add	r3, sp, #96	; 0x60
30004778:	2220      	movs	r2, #32
3000477a:	f89d 001e 	ldrb.w	r0, [sp, #30]
3000477e:	4d7d      	ldr	r5, [pc, #500]	; (30004974 <_BOOT_Validate_ImgHash+0x244>)
30004780:	47a8      	blx	r5
30004782:	2800      	cmp	r0, #0
30004784:	f040 808b 	bne.w	3000489e <_BOOT_Validate_ImgHash+0x16e>
30004788:	b32c      	cbz	r4, 300047d6 <_BOOT_Validate_ImgHash+0xa6>
3000478a:	aad0      	add	r2, sp, #832	; 0x340
3000478c:	1e63      	subs	r3, r4, #1
3000478e:	ae50      	add	r6, sp, #320	; 0x140
30004790:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 3000498c <_BOOT_Validate_ImgHash+0x25c>
30004794:	f012 0218 	ands.w	r2, r2, #24
30004798:	b2db      	uxtb	r3, r3
3000479a:	f026 091f 	bic.w	r9, r6, #31
3000479e:	4d76      	ldr	r5, [pc, #472]	; (30004978 <_BOOT_Validate_ImgHash+0x248>)
300047a0:	9204      	str	r2, [sp, #16]
300047a2:	f108 0208 	add.w	r2, r8, #8
300047a6:	bf08      	it	eq
300047a8:	46b1      	moveq	r9, r6
300047aa:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300047ae:	9302      	str	r3, [sp, #8]
300047b0:	e9d8 b700 	ldrd	fp, r7, [r8]
300047b4:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300047b8:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300047bc:	f0c0 80a7 	bcc.w	3000490e <_BOOT_Validate_ImgHash+0x1de>
300047c0:	4639      	mov	r1, r7
300047c2:	4658      	mov	r0, fp
300047c4:	aa18      	add	r2, sp, #96	; 0x60
300047c6:	47d0      	blx	sl
300047c8:	2800      	cmp	r0, #0
300047ca:	d168      	bne.n	3000489e <_BOOT_Validate_ImgHash+0x16e>
300047cc:	f108 0808 	add.w	r8, r8, #8
300047d0:	9b02      	ldr	r3, [sp, #8]
300047d2:	4543      	cmp	r3, r8
300047d4:	d1ec      	bne.n	300047b0 <_BOOT_Validate_ImgHash+0x80>
300047d6:	ac08      	add	r4, sp, #32
300047d8:	a918      	add	r1, sp, #96	; 0x60
300047da:	4b68      	ldr	r3, [pc, #416]	; (3000497c <_BOOT_Validate_ImgHash+0x24c>)
300047dc:	4620      	mov	r0, r4
300047de:	4798      	blx	r3
300047e0:	4601      	mov	r1, r0
300047e2:	2800      	cmp	r0, #0
300047e4:	d15b      	bne.n	3000489e <_BOOT_Validate_ImgHash+0x16e>
300047e6:	2001      	movs	r0, #1
300047e8:	4b61      	ldr	r3, [pc, #388]	; (30004970 <_BOOT_Validate_ImgHash+0x240>)
300047ea:	4798      	blx	r3
300047ec:	f89d 501e 	ldrb.w	r5, [sp, #30]
300047f0:	4b63      	ldr	r3, [pc, #396]	; (30004980 <_BOOT_Validate_ImgHash+0x250>)
300047f2:	4798      	blx	r3
300047f4:	fbb0 f3f5 	udiv	r3, r0, r5
300047f8:	fb03 0315 	mls	r3, r3, r5, r0
300047fc:	b2db      	uxtb	r3, r3
300047fe:	b1bd      	cbz	r5, 30004830 <_BOOT_Validate_ImgHash+0x100>
30004800:	2000      	movs	r0, #0
30004802:	9f03      	ldr	r7, [sp, #12]
30004804:	4606      	mov	r6, r0
30004806:	1c5a      	adds	r2, r3, #1
30004808:	5cf9      	ldrb	r1, [r7, r3]
3000480a:	5ce3      	ldrb	r3, [r4, r3]
3000480c:	3001      	adds	r0, #1
3000480e:	b2d2      	uxtb	r2, r2
30004810:	404b      	eors	r3, r1
30004812:	431e      	orrs	r6, r3
30004814:	fbb2 f3f5 	udiv	r3, r2, r5
30004818:	fb05 2213 	mls	r2, r5, r3, r2
3000481c:	b2d3      	uxtb	r3, r2
3000481e:	b2c2      	uxtb	r2, r0
30004820:	4295      	cmp	r5, r2
30004822:	d8f0      	bhi.n	30004806 <_BOOT_Validate_ImgHash+0xd6>
30004824:	2e00      	cmp	r6, #0
30004826:	9b01      	ldr	r3, [sp, #4]
30004828:	bf18      	it	ne
3000482a:	f06f 0306 	mvnne.w	r3, #6
3000482e:	9301      	str	r3, [sp, #4]
30004830:	9801      	ldr	r0, [sp, #4]
30004832:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30004836:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000483a:	a918      	add	r1, sp, #96	; 0x60
3000483c:	f89d 001e 	ldrb.w	r0, [sp, #30]
30004840:	4b50      	ldr	r3, [pc, #320]	; (30004984 <_BOOT_Validate_ImgHash+0x254>)
30004842:	4798      	blx	r3
30004844:	bb58      	cbnz	r0, 3000489e <_BOOT_Validate_ImgHash+0x16e>
30004846:	b31c      	cbz	r4, 30004890 <_BOOT_Validate_ImgHash+0x160>
30004848:	aad0      	add	r2, sp, #832	; 0x340
3000484a:	1e63      	subs	r3, r4, #1
3000484c:	ae50      	add	r6, sp, #320	; 0x140
3000484e:	f8df a140 	ldr.w	sl, [pc, #320]	; 30004990 <_BOOT_Validate_ImgHash+0x260>
30004852:	f012 0218 	ands.w	r2, r2, #24
30004856:	b2db      	uxtb	r3, r3
30004858:	f026 091f 	bic.w	r9, r6, #31
3000485c:	4d46      	ldr	r5, [pc, #280]	; (30004978 <_BOOT_Validate_ImgHash+0x248>)
3000485e:	9204      	str	r2, [sp, #16]
30004860:	f108 0208 	add.w	r2, r8, #8
30004864:	bf08      	it	eq
30004866:	46b1      	moveq	r9, r6
30004868:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
3000486c:	9302      	str	r3, [sp, #8]
3000486e:	e9d8 b700 	ldrd	fp, r7, [r8]
30004872:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30004876:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
3000487a:	d318      	bcc.n	300048ae <_BOOT_Validate_ImgHash+0x17e>
3000487c:	4639      	mov	r1, r7
3000487e:	4658      	mov	r0, fp
30004880:	aa18      	add	r2, sp, #96	; 0x60
30004882:	47d0      	blx	sl
30004884:	b958      	cbnz	r0, 3000489e <_BOOT_Validate_ImgHash+0x16e>
30004886:	f108 0808 	add.w	r8, r8, #8
3000488a:	9b02      	ldr	r3, [sp, #8]
3000488c:	4543      	cmp	r3, r8
3000488e:	d1ee      	bne.n	3000486e <_BOOT_Validate_ImgHash+0x13e>
30004890:	ac08      	add	r4, sp, #32
30004892:	a918      	add	r1, sp, #96	; 0x60
30004894:	4b3c      	ldr	r3, [pc, #240]	; (30004988 <_BOOT_Validate_ImgHash+0x258>)
30004896:	4620      	mov	r0, r4
30004898:	4798      	blx	r3
3000489a:	2800      	cmp	r0, #0
3000489c:	d0a6      	beq.n	300047ec <_BOOT_Validate_ImgHash+0xbc>
3000489e:	f06f 030a 	mvn.w	r3, #10
300048a2:	9301      	str	r3, [sp, #4]
300048a4:	9801      	ldr	r0, [sp, #4]
300048a6:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300048aa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300048ae:	2f00      	cmp	r7, #0
300048b0:	d0e9      	beq.n	30004886 <_BOOT_Validate_ImgHash+0x156>
300048b2:	9c04      	ldr	r4, [sp, #16]
300048b4:	f8cd 8014 	str.w	r8, [sp, #20]
300048b8:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
300048bc:	46b8      	mov	r8, r7
300048be:	4659      	mov	r1, fp
300048c0:	4630      	mov	r0, r6
300048c2:	bf28      	it	cs
300048c4:	f44f 7800 	movcs.w	r8, #512	; 0x200
300048c8:	4642      	mov	r2, r8
300048ca:	f001 fa27 	bl	30005d1c <BOOT_ImgCopy>
300048ce:	696a      	ldr	r2, [r5, #20]
300048d0:	03d3      	lsls	r3, r2, #15
300048d2:	d50f      	bpl.n	300048f4 <_BOOT_Validate_ImgHash+0x1c4>
300048d4:	eb08 0004 	add.w	r0, r8, r4
300048d8:	f3bf 8f4f 	dsb	sy
300048dc:	4448      	add	r0, r9
300048de:	464a      	mov	r2, r9
300048e0:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
300048e4:	3220      	adds	r2, #32
300048e6:	1a83      	subs	r3, r0, r2
300048e8:	2b00      	cmp	r3, #0
300048ea:	dcf9      	bgt.n	300048e0 <_BOOT_Validate_ImgHash+0x1b0>
300048ec:	f3bf 8f4f 	dsb	sy
300048f0:	f3bf 8f6f 	isb	sy
300048f4:	aa18      	add	r2, sp, #96	; 0x60
300048f6:	4641      	mov	r1, r8
300048f8:	4630      	mov	r0, r6
300048fa:	47d0      	blx	sl
300048fc:	2800      	cmp	r0, #0
300048fe:	d1ce      	bne.n	3000489e <_BOOT_Validate_ImgHash+0x16e>
30004900:	ebb7 0708 	subs.w	r7, r7, r8
30004904:	44c3      	add	fp, r8
30004906:	d1d7      	bne.n	300048b8 <_BOOT_Validate_ImgHash+0x188>
30004908:	f8dd 8014 	ldr.w	r8, [sp, #20]
3000490c:	e7bb      	b.n	30004886 <_BOOT_Validate_ImgHash+0x156>
3000490e:	2f00      	cmp	r7, #0
30004910:	f43f af5c 	beq.w	300047cc <_BOOT_Validate_ImgHash+0x9c>
30004914:	9c04      	ldr	r4, [sp, #16]
30004916:	f8cd 8014 	str.w	r8, [sp, #20]
3000491a:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
3000491e:	46b8      	mov	r8, r7
30004920:	4659      	mov	r1, fp
30004922:	4630      	mov	r0, r6
30004924:	bf28      	it	cs
30004926:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000492a:	4642      	mov	r2, r8
3000492c:	f001 f9f6 	bl	30005d1c <BOOT_ImgCopy>
30004930:	696a      	ldr	r2, [r5, #20]
30004932:	03d2      	lsls	r2, r2, #15
30004934:	d50f      	bpl.n	30004956 <_BOOT_Validate_ImgHash+0x226>
30004936:	eb08 0004 	add.w	r0, r8, r4
3000493a:	f3bf 8f4f 	dsb	sy
3000493e:	4448      	add	r0, r9
30004940:	464a      	mov	r2, r9
30004942:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30004946:	3220      	adds	r2, #32
30004948:	1a83      	subs	r3, r0, r2
3000494a:	2b00      	cmp	r3, #0
3000494c:	dcf9      	bgt.n	30004942 <_BOOT_Validate_ImgHash+0x212>
3000494e:	f3bf 8f4f 	dsb	sy
30004952:	f3bf 8f6f 	isb	sy
30004956:	aa18      	add	r2, sp, #96	; 0x60
30004958:	4641      	mov	r1, r8
3000495a:	4630      	mov	r0, r6
3000495c:	47d0      	blx	sl
3000495e:	2800      	cmp	r0, #0
30004960:	d19d      	bne.n	3000489e <_BOOT_Validate_ImgHash+0x16e>
30004962:	ebb7 0708 	subs.w	r7, r7, r8
30004966:	44c3      	add	fp, r8
30004968:	d1d7      	bne.n	3000491a <_BOOT_Validate_ImgHash+0x1ea>
3000496a:	f8dd 8014 	ldr.w	r8, [sp, #20]
3000496e:	e72d      	b.n	300047cc <_BOOT_Validate_ImgHash+0x9c>
30004970:	000024dd 	.word	0x000024dd
30004974:	30009b81 	.word	0x30009b81
30004978:	e000ed00 	.word	0xe000ed00
3000497c:	00005bb9 	.word	0x00005bb9
30004980:	0000ea4d 	.word	0x0000ea4d
30004984:	00004ffd 	.word	0x00004ffd
30004988:	000053f1 	.word	0x000053f1
3000498c:	00005b4d 	.word	0x00005b4d
30004990:	00005385 	.word	0x00005385

30004994 <BOOT_Validate_ImgHash>:
30004994:	b530      	push	{r4, r5, lr}
30004996:	b085      	sub	sp, #20
30004998:	4d0a      	ldr	r5, [pc, #40]	; (300049c4 <BOOT_Validate_ImgHash+0x30>)
3000499a:	4604      	mov	r4, r0
3000499c:	9301      	str	r3, [sp, #4]
3000499e:	e9cd 2102 	strd	r2, r1, [sp, #8]
300049a2:	47a8      	blx	r5
300049a4:	9903      	ldr	r1, [sp, #12]
300049a6:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300049aa:	b128      	cbz	r0, 300049b8 <BOOT_Validate_ImgHash+0x24>
300049ac:	4620      	mov	r0, r4
300049ae:	b005      	add	sp, #20
300049b0:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300049b4:	f007 be74 	b.w	3000c6a0 <__SBOOT_Validate_ImgHash_veneer>
300049b8:	4620      	mov	r0, r4
300049ba:	b005      	add	sp, #20
300049bc:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300049c0:	f7ff beb6 	b.w	30004730 <_BOOT_Validate_ImgHash>
300049c4:	3000a219 	.word	0x3000a219

300049c8 <BOOT_SignatureCheck>:
300049c8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300049cc:	461e      	mov	r6, r3
300049ce:	b08b      	sub	sp, #44	; 0x2c
300049d0:	4b7e      	ldr	r3, [pc, #504]	; (30004bcc <BOOT_SignatureCheck+0x204>)
300049d2:	4680      	mov	r8, r0
300049d4:	4689      	mov	r9, r1
300049d6:	4617      	mov	r7, r2
300049d8:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
300049dc:	4c7c      	ldr	r4, [pc, #496]	; (30004bd0 <BOOT_SignatureCheck+0x208>)
300049de:	9d12      	ldr	r5, [sp, #72]	; 0x48
300049e0:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300049e2:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
300049e6:	47a0      	blx	r4
300049e8:	b918      	cbnz	r0, 300049f2 <BOOT_SignatureCheck+0x2a>
300049ea:	2000      	movs	r0, #0
300049ec:	b00b      	add	sp, #44	; 0x2c
300049ee:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300049f2:	4b78      	ldr	r3, [pc, #480]	; (30004bd4 <BOOT_SignatureCheck+0x20c>)
300049f4:	2000      	movs	r0, #0
300049f6:	4798      	blx	r3
300049f8:	f898 2012 	ldrb.w	r2, [r8, #18]
300049fc:	f898 3013 	ldrb.w	r3, [r8, #19]
30004a00:	f10d 0117 	add.w	r1, sp, #23
30004a04:	f10d 0016 	add.w	r0, sp, #22
30004a08:	f007 fe72 	bl	3000c6f0 <__SBOOT_Validate_Algorithm_veneer>
30004a0c:	4602      	mov	r2, r0
30004a0e:	b9e8      	cbnz	r0, 30004a4c <BOOT_SignatureCheck+0x84>
30004a10:	69f0      	ldr	r0, [r6, #28]
30004a12:	b918      	cbnz	r0, 30004a1c <BOOT_SignatureCheck+0x54>
30004a14:	e0d7      	b.n	30004bc6 <BOOT_SignatureCheck+0x1fe>
30004a16:	b2ca      	uxtb	r2, r1
30004a18:	4282      	cmp	r2, r0
30004a1a:	d208      	bcs.n	30004a2e <BOOT_SignatureCheck+0x66>
30004a1c:	eb02 04c2 	add.w	r4, r2, r2, lsl #3
30004a20:	1c51      	adds	r1, r2, #1
30004a22:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30004a26:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30004a2a:	42ab      	cmp	r3, r5
30004a2c:	d1f3      	bne.n	30004a16 <BOOT_SignatureCheck+0x4e>
30004a2e:	2124      	movs	r1, #36	; 0x24
30004a30:	23b4      	movs	r3, #180	; 0xb4
30004a32:	fb11 3202 	smlabb	r2, r1, r2, r3
30004a36:	f108 0470 	add.w	r4, r8, #112	; 0x70
30004a3a:	4432      	add	r2, r6
30004a3c:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004a40:	4621      	mov	r1, r4
30004a42:	f007 fe3d 	bl	3000c6c0 <__SBOOT_Validate_PubKey_veneer>
30004a46:	4602      	mov	r2, r0
30004a48:	2800      	cmp	r0, #0
30004a4a:	d072      	beq.n	30004b32 <BOOT_SignatureCheck+0x16a>
30004a4c:	4b62      	ldr	r3, [pc, #392]	; (30004bd8 <BOOT_SignatureCheck+0x210>)
30004a4e:	681b      	ldr	r3, [r3, #0]
30004a50:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004a54:	2b00      	cmp	r3, #0
30004a56:	f040 80a4 	bne.w	30004ba2 <BOOT_SignatureCheck+0x1da>
30004a5a:	2f00      	cmp	r7, #0
30004a5c:	d045      	beq.n	30004aea <BOOT_SignatureCheck+0x122>
30004a5e:	464d      	mov	r5, r9
30004a60:	f107 39ff 	add.w	r9, r7, #4294967295
30004a64:	f04f 0800 	mov.w	r8, #0
30004a68:	4f5c      	ldr	r7, [pc, #368]	; (30004bdc <BOOT_SignatureCheck+0x214>)
30004a6a:	fa5f f389 	uxtb.w	r3, r9
30004a6e:	f105 0908 	add.w	r9, r5, #8
30004a72:	1d2e      	adds	r6, r5, #4
30004a74:	4c5a      	ldr	r4, [pc, #360]	; (30004be0 <BOOT_SignatureCheck+0x218>)
30004a76:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30004a7a:	e003      	b.n	30004a84 <BOOT_SignatureCheck+0xbc>
30004a7c:	3508      	adds	r5, #8
30004a7e:	3608      	adds	r6, #8
30004a80:	45a9      	cmp	r9, r5
30004a82:	d032      	beq.n	30004aea <BOOT_SignatureCheck+0x122>
30004a84:	f856 0c04 	ldr.w	r0, [r6, #-4]
30004a88:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30004a8c:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004a90:	d3f4      	bcc.n	30004a7c <BOOT_SignatureCheck+0xb4>
30004a92:	2100      	movs	r1, #0
30004a94:	686a      	ldr	r2, [r5, #4]
30004a96:	47b8      	blx	r7
30004a98:	6963      	ldr	r3, [r4, #20]
30004a9a:	03d9      	lsls	r1, r3, #15
30004a9c:	d5ee      	bpl.n	30004a7c <BOOT_SignatureCheck+0xb4>
30004a9e:	f856 2c04 	ldr.w	r2, [r6, #-4]
30004aa2:	686b      	ldr	r3, [r5, #4]
30004aa4:	ea02 0103 	and.w	r1, r2, r3
30004aa8:	3101      	adds	r1, #1
30004aaa:	d022      	beq.n	30004af2 <BOOT_SignatureCheck+0x12a>
30004aac:	f012 011f 	ands.w	r1, r2, #31
30004ab0:	d002      	beq.n	30004ab8 <BOOT_SignatureCheck+0xf0>
30004ab2:	440b      	add	r3, r1
30004ab4:	f022 021f 	bic.w	r2, r2, #31
30004ab8:	f3bf 8f4f 	dsb	sy
30004abc:	2b00      	cmp	r3, #0
30004abe:	dd0c      	ble.n	30004ada <BOOT_SignatureCheck+0x112>
30004ac0:	1e59      	subs	r1, r3, #1
30004ac2:	f102 0320 	add.w	r3, r2, #32
30004ac6:	f021 011f 	bic.w	r1, r1, #31
30004aca:	4419      	add	r1, r3
30004acc:	428b      	cmp	r3, r1
30004ace:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30004ad2:	461a      	mov	r2, r3
30004ad4:	f103 0320 	add.w	r3, r3, #32
30004ad8:	d1f8      	bne.n	30004acc <BOOT_SignatureCheck+0x104>
30004ada:	f3bf 8f4f 	dsb	sy
30004ade:	f3bf 8f6f 	isb	sy
30004ae2:	3508      	adds	r5, #8
30004ae4:	3608      	adds	r6, #8
30004ae6:	45a9      	cmp	r9, r5
30004ae8:	d1cc      	bne.n	30004a84 <BOOT_SignatureCheck+0xbc>
30004aea:	20ff      	movs	r0, #255	; 0xff
30004aec:	b00b      	add	sp, #44	; 0x2c
30004aee:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004af2:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30004af6:	f3bf 8f4f 	dsb	sy
30004afa:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30004afe:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004b02:	f3c3 304e 	ubfx	r0, r3, #13, #15
30004b06:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30004b0a:	0140      	lsls	r0, r0, #5
30004b0c:	ea00 010e 	and.w	r1, r0, lr
30004b10:	4663      	mov	r3, ip
30004b12:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004b16:	3b01      	subs	r3, #1
30004b18:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30004b1c:	1c5a      	adds	r2, r3, #1
30004b1e:	d1f8      	bne.n	30004b12 <BOOT_SignatureCheck+0x14a>
30004b20:	3820      	subs	r0, #32
30004b22:	f110 0f20 	cmn.w	r0, #32
30004b26:	d1f1      	bne.n	30004b0c <BOOT_SignatureCheck+0x144>
30004b28:	f3bf 8f4f 	dsb	sy
30004b2c:	f3bf 8f6f 	isb	sy
30004b30:	e7d7      	b.n	30004ae2 <BOOT_SignatureCheck+0x11a>
30004b32:	f508 7380 	add.w	r3, r8, #256	; 0x100
30004b36:	4622      	mov	r2, r4
30004b38:	f89d 1017 	ldrb.w	r1, [sp, #23]
30004b3c:	9301      	str	r3, [sp, #4]
30004b3e:	f44f 7380 	mov.w	r3, #256	; 0x100
30004b42:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004b46:	9300      	str	r3, [sp, #0]
30004b48:	4643      	mov	r3, r8
30004b4a:	f007 fdc1 	bl	3000c6d0 <__SBOOT_Validate_Signature_veneer>
30004b4e:	4602      	mov	r2, r0
30004b50:	2800      	cmp	r0, #0
30004b52:	f47f af7b 	bne.w	30004a4c <BOOT_SignatureCheck+0x84>
30004b56:	f108 0530 	add.w	r5, r8, #48	; 0x30
30004b5a:	4b22      	ldr	r3, [pc, #136]	; (30004be4 <BOOT_SignatureCheck+0x21c>)
30004b5c:	f89d 4017 	ldrb.w	r4, [sp, #23]
30004b60:	4798      	blx	r3
30004b62:	4629      	mov	r1, r5
30004b64:	bb40      	cbnz	r0, 30004bb8 <BOOT_SignatureCheck+0x1f0>
30004b66:	464a      	mov	r2, r9
30004b68:	4620      	mov	r0, r4
30004b6a:	463b      	mov	r3, r7
30004b6c:	f7ff fde0 	bl	30004730 <_BOOT_Validate_ImgHash>
30004b70:	4602      	mov	r2, r0
30004b72:	4b19      	ldr	r3, [pc, #100]	; (30004bd8 <BOOT_SignatureCheck+0x210>)
30004b74:	681b      	ldr	r3, [r3, #0]
30004b76:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004b7a:	2a00      	cmp	r2, #0
30004b7c:	f47f af6a 	bne.w	30004a54 <BOOT_SignatureCheck+0x8c>
30004b80:	2b00      	cmp	r3, #0
30004b82:	f43f af32 	beq.w	300049ea <BOOT_SignatureCheck+0x22>
30004b86:	f898 3011 	ldrb.w	r3, [r8, #17]
30004b8a:	9203      	str	r2, [sp, #12]
30004b8c:	aa0a      	add	r2, sp, #40	; 0x28
30004b8e:	4816      	ldr	r0, [pc, #88]	; (30004be8 <BOOT_SignatureCheck+0x220>)
30004b90:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30004b94:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004b98:	f007 fdba 	bl	3000c710 <__DiagPrintf_veneer>
30004b9c:	9a03      	ldr	r2, [sp, #12]
30004b9e:	4610      	mov	r0, r2
30004ba0:	e724      	b.n	300049ec <BOOT_SignatureCheck+0x24>
30004ba2:	a90a      	add	r1, sp, #40	; 0x28
30004ba4:	f898 3011 	ldrb.w	r3, [r8, #17]
30004ba8:	4810      	ldr	r0, [pc, #64]	; (30004bec <BOOT_SignatureCheck+0x224>)
30004baa:	eb01 0383 	add.w	r3, r1, r3, lsl #2
30004bae:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004bb2:	f007 fdad 	bl	3000c710 <__DiagPrintf_veneer>
30004bb6:	e750      	b.n	30004a5a <BOOT_SignatureCheck+0x92>
30004bb8:	464a      	mov	r2, r9
30004bba:	4620      	mov	r0, r4
30004bbc:	463b      	mov	r3, r7
30004bbe:	f007 fd6f 	bl	3000c6a0 <__SBOOT_Validate_ImgHash_veneer>
30004bc2:	4602      	mov	r2, r0
30004bc4:	e7d5      	b.n	30004b72 <BOOT_SignatureCheck+0x1aa>
30004bc6:	22b4      	movs	r2, #180	; 0xb4
30004bc8:	e735      	b.n	30004a36 <BOOT_SignatureCheck+0x6e>
30004bca:	bf00      	nop
30004bcc:	3000e8b4 	.word	0x3000e8b4
30004bd0:	0000c149 	.word	0x0000c149
30004bd4:	00002451 	.word	0x00002451
30004bd8:	2001c00c 	.word	0x2001c00c
30004bdc:	00012be5 	.word	0x00012be5
30004be0:	e000ed00 	.word	0xe000ed00
30004be4:	3000a219 	.word	0x3000a219
30004be8:	3000cd18 	.word	0x3000cd18
30004bec:	3000cd2c 	.word	0x3000cd2c

30004bf0 <BOOT_CertificateCheck>:
30004bf0:	b530      	push	{r4, r5, lr}
30004bf2:	4b2b      	ldr	r3, [pc, #172]	; (30004ca0 <BOOT_CertificateCheck+0xb0>)
30004bf4:	b0b1      	sub	sp, #196	; 0xc4
30004bf6:	4604      	mov	r4, r0
30004bf8:	460d      	mov	r5, r1
30004bfa:	4798      	blx	r3
30004bfc:	b910      	cbnz	r0, 30004c04 <BOOT_CertificateCheck+0x14>
30004bfe:	2000      	movs	r0, #0
30004c00:	b031      	add	sp, #196	; 0xc4
30004c02:	bd30      	pop	{r4, r5, pc}
30004c04:	69a1      	ldr	r1, [r4, #24]
30004c06:	2290      	movs	r2, #144	; 0x90
30004c08:	a80c      	add	r0, sp, #48	; 0x30
30004c0a:	4429      	add	r1, r5
30004c0c:	f001 f886 	bl	30005d1c <BOOT_ImgCopy>
30004c10:	4b24      	ldr	r3, [pc, #144]	; (30004ca4 <BOOT_CertificateCheck+0xb4>)
30004c12:	4798      	blx	r3
30004c14:	4924      	ldr	r1, [pc, #144]	; (30004ca8 <BOOT_CertificateCheck+0xb8>)
30004c16:	f000 031f 	and.w	r3, r0, #31
30004c1a:	2220      	movs	r2, #32
30004c1c:	6808      	ldr	r0, [r1, #0]
30004c1e:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30004c22:	a930      	add	r1, sp, #192	; 0xc0
30004c24:	3a01      	subs	r2, #1
30004c26:	f813 c000 	ldrb.w	ip, [r3, r0]
30004c2a:	4419      	add	r1, r3
30004c2c:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30004c30:	f103 0301 	add.w	r3, r3, #1
30004c34:	f801 ccb0 	strb.w	ip, [r1, #-176]
30004c38:	f003 031f 	and.w	r3, r3, #31
30004c3c:	d1f1      	bne.n	30004c22 <BOOT_CertificateCheck+0x32>
30004c3e:	4610      	mov	r0, r2
30004c40:	4b1a      	ldr	r3, [pc, #104]	; (30004cac <BOOT_CertificateCheck+0xbc>)
30004c42:	4798      	blx	r3
30004c44:	7ce3      	ldrb	r3, [r4, #19]
30004c46:	7ca2      	ldrb	r2, [r4, #18]
30004c48:	f10d 010f 	add.w	r1, sp, #15
30004c4c:	f10d 000e 	add.w	r0, sp, #14
30004c50:	f007 fd4e 	bl	3000c6f0 <__SBOOT_Validate_Algorithm_veneer>
30004c54:	b940      	cbnz	r0, 30004c68 <BOOT_CertificateCheck+0x78>
30004c56:	f104 0520 	add.w	r5, r4, #32
30004c5a:	aa04      	add	r2, sp, #16
30004c5c:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004c60:	4629      	mov	r1, r5
30004c62:	f007 fd2d 	bl	3000c6c0 <__SBOOT_Validate_PubKey_veneer>
30004c66:	b130      	cbz	r0, 30004c76 <BOOT_CertificateCheck+0x86>
30004c68:	4b11      	ldr	r3, [pc, #68]	; (30004cb0 <BOOT_CertificateCheck+0xc0>)
30004c6a:	681b      	ldr	r3, [r3, #0]
30004c6c:	039b      	lsls	r3, r3, #14
30004c6e:	d411      	bmi.n	30004c94 <BOOT_CertificateCheck+0xa4>
30004c70:	20ff      	movs	r0, #255	; 0xff
30004c72:	b031      	add	sp, #196	; 0xc4
30004c74:	bd30      	pop	{r4, r5, pc}
30004c76:	69a0      	ldr	r0, [r4, #24]
30004c78:	4623      	mov	r3, r4
30004c7a:	ac0c      	add	r4, sp, #48	; 0x30
30004c7c:	462a      	mov	r2, r5
30004c7e:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004c82:	e9cd 0400 	strd	r0, r4, [sp]
30004c86:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004c8a:	f007 fd21 	bl	3000c6d0 <__SBOOT_Validate_Signature_veneer>
30004c8e:	2800      	cmp	r0, #0
30004c90:	d0b5      	beq.n	30004bfe <BOOT_CertificateCheck+0xe>
30004c92:	e7e9      	b.n	30004c68 <BOOT_CertificateCheck+0x78>
30004c94:	4601      	mov	r1, r0
30004c96:	4807      	ldr	r0, [pc, #28]	; (30004cb4 <BOOT_CertificateCheck+0xc4>)
30004c98:	f007 fd3a 	bl	3000c710 <__DiagPrintf_veneer>
30004c9c:	20ff      	movs	r0, #255	; 0xff
30004c9e:	e7af      	b.n	30004c00 <BOOT_CertificateCheck+0x10>
30004ca0:	0000c149 	.word	0x0000c149
30004ca4:	0000ea4d 	.word	0x0000ea4d
30004ca8:	3000ebe0 	.word	0x3000ebe0
30004cac:	00002451 	.word	0x00002451
30004cb0:	2001c00c 	.word	0x2001c00c
30004cb4:	3000cd48 	.word	0x3000cd48

30004cb8 <BOOT_RAM_TZCfg>:
30004cb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004cbc:	b085      	sub	sp, #20
30004cbe:	4e58      	ldr	r6, [pc, #352]	; (30004e20 <BOOT_RAM_TZCfg+0x168>)
30004cc0:	4958      	ldr	r1, [pc, #352]	; (30004e24 <BOOT_RAM_TZCfg+0x16c>)
30004cc2:	2501      	movs	r5, #1
30004cc4:	f10d 0e08 	add.w	lr, sp, #8
30004cc8:	4a57      	ldr	r2, [pc, #348]	; (30004e28 <BOOT_RAM_TZCfg+0x170>)
30004cca:	4b58      	ldr	r3, [pc, #352]	; (30004e2c <BOOT_RAM_TZCfg+0x174>)
30004ccc:	46ec      	mov	ip, sp
30004cce:	4677      	mov	r7, lr
30004cd0:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30004e4c <BOOT_RAM_TZCfg+0x194>
30004cd4:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30004cd8:	f8df 9174 	ldr.w	r9, [pc, #372]	; 30004e50 <BOOT_RAM_TZCfg+0x198>
30004cdc:	9202      	str	r2, [sp, #8]
30004cde:	9303      	str	r3, [sp, #12]
30004ce0:	e9cd 6100 	strd	r6, r1, [sp]
30004ce4:	f85c 2b04 	ldr.w	r2, [ip], #4
30004ce8:	2300      	movs	r3, #0
30004cea:	f85e 1b04 	ldr.w	r1, [lr], #4
30004cee:	680c      	ldr	r4, [r1, #0]
30004cf0:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30004cf4:	fa05 f003 	lsl.w	r0, r5, r3
30004cf8:	f1b4 3fff 	cmp.w	r4, #4294967295
30004cfc:	d00c      	beq.n	30004d18 <BOOT_RAM_TZCfg+0x60>
30004cfe:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30004d02:	3301      	adds	r3, #1
30004d04:	684c      	ldr	r4, [r1, #4]
30004d06:	310c      	adds	r1, #12
30004d08:	2b08      	cmp	r3, #8
30004d0a:	f8cb 4004 	str.w	r4, [fp, #4]
30004d0e:	6c14      	ldr	r4, [r2, #64]	; 0x40
30004d10:	ea40 0004 	orr.w	r0, r0, r4
30004d14:	6410      	str	r0, [r2, #64]	; 0x40
30004d16:	d1ea      	bne.n	30004cee <BOOT_RAM_TZCfg+0x36>
30004d18:	42b2      	cmp	r2, r6
30004d1a:	d072      	beq.n	30004e02 <BOOT_RAM_TZCfg+0x14a>
30004d1c:	6455      	str	r5, [r2, #68]	; 0x44
30004d1e:	45bc      	cmp	ip, r7
30004d20:	d1e0      	bne.n	30004ce4 <BOOT_RAM_TZCfg+0x2c>
30004d22:	4b43      	ldr	r3, [pc, #268]	; (30004e30 <BOOT_RAM_TZCfg+0x178>)
30004d24:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
30004d28:	f04f 32ff 	mov.w	r2, #4294967295
30004d2c:	4841      	ldr	r0, [pc, #260]	; (30004e34 <BOOT_RAM_TZCfg+0x17c>)
30004d2e:	6019      	str	r1, [r3, #0]
30004d30:	2501      	movs	r5, #1
30004d32:	605a      	str	r2, [r3, #4]
30004d34:	2100      	movs	r1, #0
30004d36:	4a40      	ldr	r2, [pc, #256]	; (30004e38 <BOOT_RAM_TZCfg+0x180>)
30004d38:	4c40      	ldr	r4, [pc, #256]	; (30004e3c <BOOT_RAM_TZCfg+0x184>)
30004d3a:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
30004d3e:	6813      	ldr	r3, [r2, #0]
30004d40:	3301      	adds	r3, #1
30004d42:	d019      	beq.n	30004d78 <BOOT_RAM_TZCfg+0xc0>
30004d44:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30004d48:	320c      	adds	r2, #12
30004d4a:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30004d4e:	3101      	adds	r1, #1
30004d50:	f023 031f 	bic.w	r3, r3, #31
30004d54:	2908      	cmp	r1, #8
30004d56:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30004d5a:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30004d5e:	ea4f 0343 	mov.w	r3, r3, lsl #1
30004d62:	f020 001f 	bic.w	r0, r0, #31
30004d66:	f003 0302 	and.w	r3, r3, #2
30004d6a:	ea43 0300 	orr.w	r3, r3, r0
30004d6e:	f043 0301 	orr.w	r3, r3, #1
30004d72:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30004d76:	d1e2      	bne.n	30004d3e <BOOT_RAM_TZCfg+0x86>
30004d78:	4830      	ldr	r0, [pc, #192]	; (30004e3c <BOOT_RAM_TZCfg+0x184>)
30004d7a:	2201      	movs	r2, #1
30004d7c:	4b30      	ldr	r3, [pc, #192]	; (30004e40 <BOOT_RAM_TZCfg+0x188>)
30004d7e:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30004d82:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004d86:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30004d8a:	d11e      	bne.n	30004dca <BOOT_RAM_TZCfg+0x112>
30004d8c:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004d90:	f3bf 8f4f 	dsb	sy
30004d94:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004d98:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004d9c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004da0:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004da4:	0164      	lsls	r4, r4, #5
30004da6:	ea04 0106 	and.w	r1, r4, r6
30004daa:	462b      	mov	r3, r5
30004dac:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004db0:	3b01      	subs	r3, #1
30004db2:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004db6:	1c5a      	adds	r2, r3, #1
30004db8:	d1f8      	bne.n	30004dac <BOOT_RAM_TZCfg+0xf4>
30004dba:	3c20      	subs	r4, #32
30004dbc:	f114 0f20 	cmn.w	r4, #32
30004dc0:	d1f1      	bne.n	30004da6 <BOOT_RAM_TZCfg+0xee>
30004dc2:	f3bf 8f4f 	dsb	sy
30004dc6:	f3bf 8f6f 	isb	sy
30004dca:	4b1c      	ldr	r3, [pc, #112]	; (30004e3c <BOOT_RAM_TZCfg+0x184>)
30004dcc:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30004dd0:	491c      	ldr	r1, [pc, #112]	; (30004e44 <BOOT_RAM_TZCfg+0x18c>)
30004dd2:	691c      	ldr	r4, [r3, #16]
30004dd4:	4a1c      	ldr	r2, [pc, #112]	; (30004e48 <BOOT_RAM_TZCfg+0x190>)
30004dd6:	f044 0408 	orr.w	r4, r4, #8
30004dda:	611c      	str	r4, [r3, #16]
30004ddc:	f04f 34ff 	mov.w	r4, #4294967295
30004de0:	68d8      	ldr	r0, [r3, #12]
30004de2:	4028      	ands	r0, r5
30004de4:	4301      	orrs	r1, r0
30004de6:	60d9      	str	r1, [r3, #12]
30004de8:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30004dec:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30004df0:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30004df4:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30004df8:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30004dfc:	b005      	add	sp, #20
30004dfe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004e02:	f898 3000 	ldrb.w	r3, [r8]
30004e06:	2b00      	cmp	r3, #0
30004e08:	d189      	bne.n	30004d1e <BOOT_RAM_TZCfg+0x66>
30004e0a:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30004e0e:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30004e12:	6c33      	ldr	r3, [r6, #64]	; 0x40
30004e14:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30004e18:	6433      	str	r3, [r6, #64]	; 0x40
30004e1a:	6475      	str	r5, [r6, #68]	; 0x44
30004e1c:	e77f      	b.n	30004d1e <BOOT_RAM_TZCfg+0x66>
30004e1e:	bf00      	nop
30004e20:	51001a00 	.word	0x51001a00
30004e24:	51001200 	.word	0x51001200
30004e28:	3000edc0 	.word	0x3000edc0
30004e2c:	3000ed60 	.word	0x3000ed60
30004e30:	51001b00 	.word	0x51001b00
30004e34:	500e0000 	.word	0x500e0000
30004e38:	3000ee20 	.word	0x3000ee20
30004e3c:	e000ed00 	.word	0xe000ed00
30004e40:	42008000 	.word	0x42008000
30004e44:	05fa6000 	.word	0x05fa6000
30004e48:	e000e100 	.word	0xe000e100
30004e4c:	3000ebda 	.word	0x3000ebda
30004e50:	005fffff 	.word	0x005fffff

30004e54 <BOOT_INT_SecureFault>:
30004e54:	f3ef 8008 	mrs	r0, MSP
30004e58:	f3ef 8109 	mrs	r1, PSP
30004e5c:	4672      	mov	r2, lr
30004e5e:	f04f 0304 	mov.w	r3, #4
30004e62:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004e66:	f000 b895 	b.w	30004f94 <BOOT_FaultHandler>
30004e6a:	4770      	bx	lr

30004e6c <AsciiToHex>:
30004e6c:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30004e70:	2b05      	cmp	r3, #5
30004e72:	d802      	bhi.n	30004e7a <AsciiToHex+0xe>
30004e74:	3837      	subs	r0, #55	; 0x37
30004e76:	b2c0      	uxtb	r0, r0
30004e78:	4770      	bx	lr
30004e7a:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30004e7e:	2b05      	cmp	r3, #5
30004e80:	d802      	bhi.n	30004e88 <AsciiToHex+0x1c>
30004e82:	3857      	subs	r0, #87	; 0x57
30004e84:	b2c0      	uxtb	r0, r0
30004e86:	4770      	bx	lr
30004e88:	3830      	subs	r0, #48	; 0x30
30004e8a:	b2c0      	uxtb	r0, r0
30004e8c:	280a      	cmp	r0, #10
30004e8e:	bf28      	it	cs
30004e90:	20ff      	movcs	r0, #255	; 0xff
30004e92:	4770      	bx	lr

30004e94 <hotfix_get_addr_size>:
30004e94:	2800      	cmp	r0, #0
30004e96:	d054      	beq.n	30004f42 <hotfix_get_addr_size+0xae>
30004e98:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004e9c:	f04f 0a00 	mov.w	sl, #0
30004ea0:	b083      	sub	sp, #12
30004ea2:	f1a3 0b04 	sub.w	fp, r3, #4
30004ea6:	1c4b      	adds	r3, r1, #1
30004ea8:	4688      	mov	r8, r1
30004eaa:	4681      	mov	r9, r0
30004eac:	9300      	str	r3, [sp, #0]
30004eae:	4617      	mov	r7, r2
30004eb0:	1e53      	subs	r3, r2, #1
30004eb2:	f101 0409 	add.w	r4, r1, #9
30004eb6:	1dd5      	adds	r5, r2, #7
30004eb8:	4656      	mov	r6, sl
30004eba:	9301      	str	r3, [sp, #4]
30004ebc:	f8c8 6004 	str.w	r6, [r8, #4]
30004ec0:	210b      	movs	r1, #11
30004ec2:	f8c8 6000 	str.w	r6, [r8]
30004ec6:	4640      	mov	r0, r8
30004ec8:	f8c8 6007 	str.w	r6, [r8, #7]
30004ecc:	603e      	str	r6, [r7, #0]
30004ece:	607e      	str	r6, [r7, #4]
30004ed0:	f002 ff8c 	bl	30007dec <ymodem_uart_getdata>
30004ed4:	e9dd 1000 	ldrd	r1, r0, [sp]
30004ed8:	e004      	b.n	30004ee4 <hotfix_get_addr_size+0x50>
30004eda:	b2d3      	uxtb	r3, r2
30004edc:	428c      	cmp	r4, r1
30004ede:	f800 3f01 	strb.w	r3, [r0, #1]!
30004ee2:	d01a      	beq.n	30004f1a <hotfix_get_addr_size+0x86>
30004ee4:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30004ee8:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30004eec:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30004ef0:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30004ef4:	f1be 0f05 	cmp.w	lr, #5
30004ef8:	b2db      	uxtb	r3, r3
30004efa:	d9ef      	bls.n	30004edc <hotfix_get_addr_size+0x48>
30004efc:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30004f00:	f1bc 0f05 	cmp.w	ip, #5
30004f04:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30004f08:	b2db      	uxtb	r3, r3
30004f0a:	d9e6      	bls.n	30004eda <hotfix_get_addr_size+0x46>
30004f0c:	2b0a      	cmp	r3, #10
30004f0e:	bf28      	it	cs
30004f10:	23ff      	movcs	r3, #255	; 0xff
30004f12:	428c      	cmp	r4, r1
30004f14:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f18:	d1e4      	bne.n	30004ee4 <hotfix_get_addr_size+0x50>
30004f1a:	4639      	mov	r1, r7
30004f1c:	2300      	movs	r3, #0
30004f1e:	f811 2b01 	ldrb.w	r2, [r1], #1
30004f22:	4313      	orrs	r3, r2
30004f24:	42a9      	cmp	r1, r5
30004f26:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004f2a:	d1f8      	bne.n	30004f1e <hotfix_get_addr_size+0x8a>
30004f2c:	79fa      	ldrb	r2, [r7, #7]
30004f2e:	f10a 0a01 	add.w	sl, sl, #1
30004f32:	4313      	orrs	r3, r2
30004f34:	45ca      	cmp	sl, r9
30004f36:	f84b 3f04 	str.w	r3, [fp, #4]!
30004f3a:	d1bf      	bne.n	30004ebc <hotfix_get_addr_size+0x28>
30004f3c:	b003      	add	sp, #12
30004f3e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f42:	4770      	bx	lr

30004f44 <ascii_to_integer>:
30004f44:	2900      	cmp	r1, #0
30004f46:	4684      	mov	ip, r0
30004f48:	dd12      	ble.n	30004f70 <ascii_to_integer+0x2c>
30004f4a:	3901      	subs	r1, #1
30004f4c:	1e42      	subs	r2, r0, #1
30004f4e:	2000      	movs	r0, #0
30004f50:	448c      	add	ip, r1
30004f52:	e001      	b.n	30004f58 <ascii_to_integer+0x14>
30004f54:	4562      	cmp	r2, ip
30004f56:	d00d      	beq.n	30004f74 <ascii_to_integer+0x30>
30004f58:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30004f5c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30004f60:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30004f64:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30004f68:	2909      	cmp	r1, #9
30004f6a:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30004f6e:	d9f1      	bls.n	30004f54 <ascii_to_integer+0x10>
30004f70:	2000      	movs	r0, #0
30004f72:	4770      	bx	lr
30004f74:	4770      	bx	lr
30004f76:	bf00      	nop

30004f78 <eight_4bit_to_int>:
30004f78:	2300      	movs	r3, #0
30004f7a:	f100 0c07 	add.w	ip, r0, #7
30004f7e:	4602      	mov	r2, r0
30004f80:	f812 1b01 	ldrb.w	r1, [r2], #1
30004f84:	430b      	orrs	r3, r1
30004f86:	4562      	cmp	r2, ip
30004f88:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004f8c:	d1f8      	bne.n	30004f80 <eight_4bit_to_int+0x8>
30004f8e:	79c0      	ldrb	r0, [r0, #7]
30004f90:	4318      	orrs	r0, r3
30004f92:	4770      	bx	lr

30004f94 <BOOT_FaultHandler>:
30004f94:	b5f0      	push	{r4, r5, r6, r7, lr}
30004f96:	461f      	mov	r7, r3
30004f98:	4b39      	ldr	r3, [pc, #228]	; (30005080 <BOOT_FaultHandler+0xec>)
30004f9a:	b095      	sub	sp, #84	; 0x54
30004f9c:	4616      	mov	r6, r2
30004f9e:	4604      	mov	r4, r0
30004fa0:	4a38      	ldr	r2, [pc, #224]	; (30005084 <BOOT_FaultHandler+0xf0>)
30004fa2:	2001      	movs	r0, #1
30004fa4:	460d      	mov	r5, r1
30004fa6:	4619      	mov	r1, r3
30004fa8:	f007 f92e 	bl	3000c208 <rtk_log_write>
30004fac:	2244      	movs	r2, #68	; 0x44
30004fae:	2100      	movs	r1, #0
30004fb0:	a803      	add	r0, sp, #12
30004fb2:	f007 fb7d 	bl	3000c6b0 <____wrap_memset_veneer>
30004fb6:	0672      	lsls	r2, r6, #25
30004fb8:	4b31      	ldr	r3, [pc, #196]	; (30005080 <BOOT_FaultHandler+0xec>)
30004fba:	d54f      	bpl.n	3000505c <BOOT_FaultHandler+0xc8>
30004fbc:	4619      	mov	r1, r3
30004fbe:	4a32      	ldr	r2, [pc, #200]	; (30005088 <BOOT_FaultHandler+0xf4>)
30004fc0:	2001      	movs	r0, #1
30004fc2:	f007 f921 	bl	3000c208 <rtk_log_write>
30004fc6:	f006 010c 	and.w	r1, r6, #12
30004fca:	46a6      	mov	lr, r4
30004fcc:	f1a1 010c 	sub.w	r1, r1, #12
30004fd0:	fab1 f181 	clz	r1, r1
30004fd4:	0949      	lsrs	r1, r1, #5
30004fd6:	f1a4 0c24 	sub.w	ip, r4, #36	; 0x24
30004fda:	1f22      	subs	r2, r4, #4
30004fdc:	a804      	add	r0, sp, #16
30004fde:	9603      	str	r6, [sp, #12]
30004fe0:	f85c 3f04 	ldr.w	r3, [ip, #4]!
30004fe4:	4594      	cmp	ip, r2
30004fe6:	f840 3b04 	str.w	r3, [r0], #4
30004fea:	d1f9      	bne.n	30004fe0 <BOOT_FaultHandler+0x4c>
30004fec:	2900      	cmp	r1, #0
30004fee:	bf08      	it	eq
30004ff0:	4675      	moveq	r5, lr
30004ff2:	f10d 0c30 	add.w	ip, sp, #48	; 0x30
30004ff6:	1f28      	subs	r0, r5, #4
30004ff8:	f105 031c 	add.w	r3, r5, #28
30004ffc:	f850 4f04 	ldr.w	r4, [r0, #4]!
30005000:	4283      	cmp	r3, r0
30005002:	f84c 4b04 	str.w	r4, [ip], #4
30005006:	d1f9      	bne.n	30004ffc <BOOT_FaultHandler+0x68>
30005008:	4c20      	ldr	r4, [pc, #128]	; (3000508c <BOOT_FaultHandler+0xf8>)
3000500a:	aa03      	add	r2, sp, #12
3000500c:	4629      	mov	r1, r5
3000500e:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30005010:	f006 fe24 	bl	3000bc5c <crash_dump>
30005014:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30005018:	4b19      	ldr	r3, [pc, #100]	; (30005080 <BOOT_FaultHandler+0xec>)
3000501a:	2001      	movs	r0, #1
3000501c:	9100      	str	r1, [sp, #0]
3000501e:	4a1c      	ldr	r2, [pc, #112]	; (30005090 <BOOT_FaultHandler+0xfc>)
30005020:	4619      	mov	r1, r3
30005022:	f007 f8f1 	bl	3000c208 <rtk_log_write>
30005026:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
3000502a:	4b15      	ldr	r3, [pc, #84]	; (30005080 <BOOT_FaultHandler+0xec>)
3000502c:	2001      	movs	r0, #1
3000502e:	9200      	str	r2, [sp, #0]
30005030:	4619      	mov	r1, r3
30005032:	4a18      	ldr	r2, [pc, #96]	; (30005094 <BOOT_FaultHandler+0x100>)
30005034:	f007 f8e8 	bl	3000c208 <rtk_log_write>
30005038:	6a61      	ldr	r1, [r4, #36]	; 0x24
3000503a:	4b11      	ldr	r3, [pc, #68]	; (30005080 <BOOT_FaultHandler+0xec>)
3000503c:	2001      	movs	r0, #1
3000503e:	9100      	str	r1, [sp, #0]
30005040:	4a15      	ldr	r2, [pc, #84]	; (30005098 <BOOT_FaultHandler+0x104>)
30005042:	4619      	mov	r1, r3
30005044:	f007 f8e0 	bl	3000c208 <rtk_log_write>
30005048:	f3ef 8088 	mrs	r0, MSP_NS
3000504c:	f3ef 8189 	mrs	r1, PSP_NS
30005050:	463b      	mov	r3, r7
30005052:	4632      	mov	r2, r6
30005054:	f007 fb1c 	bl	3000c690 <__INT_HardFault_C_veneer>
30005058:	b015      	add	sp, #84	; 0x54
3000505a:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000505c:	4a0f      	ldr	r2, [pc, #60]	; (3000509c <BOOT_FaultHandler+0x108>)
3000505e:	2001      	movs	r0, #1
30005060:	4619      	mov	r1, r3
30005062:	f007 f8d1 	bl	3000c208 <rtk_log_write>
30005066:	f3ef 8394 	mrs	r3, CONTROL_NS
3000506a:	f3ef 8e88 	mrs	lr, MSP_NS
3000506e:	f3ef 8589 	mrs	r5, PSP_NS
30005072:	f016 0108 	ands.w	r1, r6, #8
30005076:	bf18      	it	ne
30005078:	f3c3 0140 	ubfxne	r1, r3, #1, #1
3000507c:	e7ab      	b.n	30004fd6 <BOOT_FaultHandler+0x42>
3000507e:	bf00      	nop
30005080:	3000cd6c 	.word	0x3000cd6c
30005084:	3000cd74 	.word	0x3000cd74
30005088:	3000cd98 	.word	0x3000cd98
3000508c:	e000ed00 	.word	0xe000ed00
30005090:	3000cdec 	.word	0x3000cdec
30005094:	3000ce04 	.word	0x3000ce04
30005098:	3000ce1c 	.word	0x3000ce1c
3000509c:	3000cdc0 	.word	0x3000cdc0

300050a0 <FLASH_Erase_With_Lock>:
300050a0:	4b11      	ldr	r3, [pc, #68]	; (300050e8 <FLASH_Erase_With_Lock+0x48>)
300050a2:	b510      	push	{r4, lr}
300050a4:	460c      	mov	r4, r1
300050a6:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
300050aa:	4798      	blx	r3
300050ac:	4b0f      	ldr	r3, [pc, #60]	; (300050ec <FLASH_Erase_With_Lock+0x4c>)
300050ae:	695b      	ldr	r3, [r3, #20]
300050b0:	03db      	lsls	r3, r3, #15
300050b2:	d512      	bpl.n	300050da <FLASH_Erase_With_Lock+0x3a>
300050b4:	f014 021f 	ands.w	r2, r4, #31
300050b8:	d110      	bne.n	300050dc <FLASH_Erase_With_Lock+0x3c>
300050ba:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300050be:	f3bf 8f4f 	dsb	sy
300050c2:	480a      	ldr	r0, [pc, #40]	; (300050ec <FLASH_Erase_With_Lock+0x4c>)
300050c4:	4422      	add	r2, r4
300050c6:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
300050ca:	3420      	adds	r4, #32
300050cc:	1b13      	subs	r3, r2, r4
300050ce:	2b00      	cmp	r3, #0
300050d0:	dcf9      	bgt.n	300050c6 <FLASH_Erase_With_Lock+0x26>
300050d2:	f3bf 8f4f 	dsb	sy
300050d6:	f3bf 8f6f 	isb	sy
300050da:	bd10      	pop	{r4, pc}
300050dc:	f024 041f 	bic.w	r4, r4, #31
300050e0:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
300050e4:	e7eb      	b.n	300050be <FLASH_Erase_With_Lock+0x1e>
300050e6:	bf00      	nop
300050e8:	0000a16d 	.word	0x0000a16d
300050ec:	e000ed00 	.word	0xe000ed00

300050f0 <FLASH_TxData_With_Lock>:
300050f0:	b538      	push	{r3, r4, r5, lr}
300050f2:	4b13      	ldr	r3, [pc, #76]	; (30005140 <FLASH_TxData_With_Lock+0x50>)
300050f4:	4604      	mov	r4, r0
300050f6:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
300050fa:	460d      	mov	r5, r1
300050fc:	4798      	blx	r3
300050fe:	4b11      	ldr	r3, [pc, #68]	; (30005144 <FLASH_TxData_With_Lock+0x54>)
30005100:	695b      	ldr	r3, [r3, #20]
30005102:	03db      	lsls	r3, r3, #15
30005104:	d517      	bpl.n	30005136 <FLASH_TxData_With_Lock+0x46>
30005106:	f014 031f 	ands.w	r3, r4, #31
3000510a:	d115      	bne.n	30005138 <FLASH_TxData_With_Lock+0x48>
3000510c:	f3bf 8f4f 	dsb	sy
30005110:	b16d      	cbz	r5, 3000512e <FLASH_TxData_With_Lock+0x3e>
30005112:	1e6a      	subs	r2, r5, #1
30005114:	f104 0320 	add.w	r3, r4, #32
30005118:	490a      	ldr	r1, [pc, #40]	; (30005144 <FLASH_TxData_With_Lock+0x54>)
3000511a:	f022 021f 	bic.w	r2, r2, #31
3000511e:	441a      	add	r2, r3
30005120:	4293      	cmp	r3, r2
30005122:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30005126:	461c      	mov	r4, r3
30005128:	f103 0320 	add.w	r3, r3, #32
3000512c:	d1f8      	bne.n	30005120 <FLASH_TxData_With_Lock+0x30>
3000512e:	f3bf 8f4f 	dsb	sy
30005132:	f3bf 8f6f 	isb	sy
30005136:	bd38      	pop	{r3, r4, r5, pc}
30005138:	f024 041f 	bic.w	r4, r4, #31
3000513c:	441d      	add	r5, r3
3000513e:	e7e5      	b.n	3000510c <FLASH_TxData_With_Lock+0x1c>
30005140:	0000a04d 	.word	0x0000a04d
30005144:	e000ed00 	.word	0xe000ed00

30005148 <flash_read_id>:
30005148:	b538      	push	{r3, r4, r5, lr}
3000514a:	4d04      	ldr	r5, [pc, #16]	; (3000515c <flash_read_id+0x14>)
3000514c:	460c      	mov	r4, r1
3000514e:	4602      	mov	r2, r0
30005150:	4b03      	ldr	r3, [pc, #12]	; (30005160 <flash_read_id+0x18>)
30005152:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30005156:	4798      	blx	r3
30005158:	4620      	mov	r0, r4
3000515a:	bd38      	pop	{r3, r4, r5, pc}
3000515c:	2001c01c 	.word	0x2001c01c
30005160:	00009db5 	.word	0x00009db5

30005164 <mcc_ReverseBit>:
30005164:	0843      	lsrs	r3, r0, #1
30005166:	08c1      	lsrs	r1, r0, #3
30005168:	f003 0308 	and.w	r3, r3, #8
3000516c:	0942      	lsrs	r2, r0, #5
3000516e:	f001 0104 	and.w	r1, r1, #4
30005172:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30005176:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
3000517a:	f002 0202 	and.w	r2, r2, #2
3000517e:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30005182:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30005186:	430b      	orrs	r3, r1
30005188:	00c1      	lsls	r1, r0, #3
3000518a:	4313      	orrs	r3, r2
3000518c:	f001 0120 	and.w	r1, r1, #32
30005190:	0042      	lsls	r2, r0, #1
30005192:	ea43 000c 	orr.w	r0, r3, ip
30005196:	f002 0310 	and.w	r3, r2, #16
3000519a:	4308      	orrs	r0, r1
3000519c:	4318      	orrs	r0, r3
3000519e:	b2c0      	uxtb	r0, r0
300051a0:	4770      	bx	lr
300051a2:	bf00      	nop

300051a4 <mcc_init_crc32>:
300051a4:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300051a6:	2600      	movs	r6, #0
300051a8:	4f16      	ldr	r7, [pc, #88]	; (30005204 <mcc_init_crc32+0x60>)
300051aa:	4605      	mov	r5, r0
300051ac:	b2f0      	uxtb	r0, r6
300051ae:	f7ff ffd9 	bl	30005164 <mcc_ReverseBit>
300051b2:	2208      	movs	r2, #8
300051b4:	0604      	lsls	r4, r0, #24
300051b6:	2c00      	cmp	r4, #0
300051b8:	ea4f 0444 	mov.w	r4, r4, lsl #1
300051bc:	bfb8      	it	lt
300051be:	407c      	eorlt	r4, r7
300051c0:	3a01      	subs	r2, #1
300051c2:	d1f8      	bne.n	300051b6 <mcc_init_crc32+0x12>
300051c4:	0e20      	lsrs	r0, r4, #24
300051c6:	3504      	adds	r5, #4
300051c8:	f7ff ffcc 	bl	30005164 <mcc_ReverseBit>
300051cc:	4603      	mov	r3, r0
300051ce:	f3c4 4007 	ubfx	r0, r4, #16, #8
300051d2:	3601      	adds	r6, #1
300051d4:	f805 3c04 	strb.w	r3, [r5, #-4]
300051d8:	f7ff ffc4 	bl	30005164 <mcc_ReverseBit>
300051dc:	4603      	mov	r3, r0
300051de:	f3c4 2007 	ubfx	r0, r4, #8, #8
300051e2:	f805 3c03 	strb.w	r3, [r5, #-3]
300051e6:	f7ff ffbd 	bl	30005164 <mcc_ReverseBit>
300051ea:	4603      	mov	r3, r0
300051ec:	b2e0      	uxtb	r0, r4
300051ee:	f805 3c02 	strb.w	r3, [r5, #-2]
300051f2:	f7ff ffb7 	bl	30005164 <mcc_ReverseBit>
300051f6:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
300051fa:	f805 0c01 	strb.w	r0, [r5, #-1]
300051fe:	d1d5      	bne.n	300051ac <mcc_init_crc32+0x8>
30005200:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30005202:	bf00      	nop
30005204:	04c11db7 	.word	0x04c11db7

30005208 <mcc_crc32>:
30005208:	b179      	cbz	r1, 3000522a <mcc_crc32+0x22>
3000520a:	eb00 0c01 	add.w	ip, r0, r1
3000520e:	f04f 31ff 	mov.w	r1, #4294967295
30005212:	f810 3b01 	ldrb.w	r3, [r0], #1
30005216:	404b      	eors	r3, r1
30005218:	4560      	cmp	r0, ip
3000521a:	b2db      	uxtb	r3, r3
3000521c:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30005220:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005224:	d1f5      	bne.n	30005212 <mcc_crc32+0xa>
30005226:	43c8      	mvns	r0, r1
30005228:	4770      	bx	lr
3000522a:	4608      	mov	r0, r1
3000522c:	4770      	bx	lr
3000522e:	bf00      	nop

30005230 <recovery_check>:
30005230:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30005234:	4283      	cmp	r3, r0
30005236:	d322      	bcc.n	3000527e <recovery_check+0x4e>
30005238:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
3000523c:	d91f      	bls.n	3000527e <recovery_check+0x4e>
3000523e:	b510      	push	{r4, lr}
30005240:	f8d0 400a 	ldr.w	r4, [r0, #10]
30005244:	f104 030c 	add.w	r3, r4, #12
30005248:	4299      	cmp	r1, r3
3000524a:	d316      	bcc.n	3000527a <recovery_check+0x4a>
3000524c:	f100 0c04 	add.w	ip, r0, #4
30005250:	b183      	cbz	r3, 30005274 <recovery_check+0x44>
30005252:	f100 0e10 	add.w	lr, r0, #16
30005256:	f04f 31ff 	mov.w	r1, #4294967295
3000525a:	44a6      	add	lr, r4
3000525c:	4c0b      	ldr	r4, [pc, #44]	; (3000528c <recovery_check+0x5c>)
3000525e:	f81c 3b01 	ldrb.w	r3, [ip], #1
30005262:	404b      	eors	r3, r1
30005264:	45f4      	cmp	ip, lr
30005266:	b2db      	uxtb	r3, r3
30005268:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000526c:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005270:	d1f5      	bne.n	3000525e <recovery_check+0x2e>
30005272:	43cb      	mvns	r3, r1
30005274:	6801      	ldr	r1, [r0, #0]
30005276:	4299      	cmp	r1, r3
30005278:	d003      	beq.n	30005282 <recovery_check+0x52>
3000527a:	2000      	movs	r0, #0
3000527c:	bd10      	pop	{r4, pc}
3000527e:	2000      	movs	r0, #0
30005280:	4770      	bx	lr
30005282:	f8d0 3006 	ldr.w	r3, [r0, #6]
30005286:	2001      	movs	r0, #1
30005288:	6013      	str	r3, [r2, #0]
3000528a:	bd10      	pop	{r4, pc}
3000528c:	3000ee80 	.word	0x3000ee80

30005290 <rewrite_bp>:
30005290:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30005294:	b0a3      	sub	sp, #140	; 0x8c
30005296:	4616      	mov	r6, r2
30005298:	4607      	mov	r7, r0
3000529a:	460d      	mov	r5, r1
3000529c:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
300052a0:	4699      	mov	r9, r3
300052a2:	f10d 0808 	add.w	r8, sp, #8
300052a6:	2280      	movs	r2, #128	; 0x80
300052a8:	2c00      	cmp	r4, #0
300052aa:	f000 80b2 	beq.w	30005412 <rewrite_bp+0x182>
300052ae:	4619      	mov	r1, r3
300052b0:	4640      	mov	r0, r8
300052b2:	f007 fa15 	bl	3000c6e0 <____wrap_memcpy_veneer>
300052b6:	2301      	movs	r3, #1
300052b8:	f109 0404 	add.w	r4, r9, #4
300052bc:	4649      	mov	r1, r9
300052be:	2002      	movs	r0, #2
300052c0:	f88d 7014 	strb.w	r7, [sp, #20]
300052c4:	e9cd 3303 	strd	r3, r3, [sp, #12]
300052c8:	f7ff feea 	bl	300050a0 <FLASH_Erase_With_Lock>
300052cc:	4620      	mov	r0, r4
300052ce:	217c      	movs	r1, #124	; 0x7c
300052d0:	aa03      	add	r2, sp, #12
300052d2:	f7ff ff0d 	bl	300050f0 <FLASH_TxData_With_Lock>
300052d6:	4620      	mov	r0, r4
300052d8:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
300052dc:	f04f 33ff 	mov.w	r3, #4294967295
300052e0:	4c54      	ldr	r4, [pc, #336]	; (30005434 <rewrite_bp+0x1a4>)
300052e2:	f810 cb01 	ldrb.w	ip, [r0], #1
300052e6:	ea8c 0c03 	eor.w	ip, ip, r3
300052ea:	4288      	cmp	r0, r1
300052ec:	fa5f fc8c 	uxtb.w	ip, ip
300052f0:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
300052f4:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
300052f8:	d1f3      	bne.n	300052e2 <rewrite_bp+0x52>
300052fa:	43db      	mvns	r3, r3
300052fc:	4649      	mov	r1, r9
300052fe:	2002      	movs	r0, #2
30005300:	9302      	str	r3, [sp, #8]
30005302:	f7ff fecd 	bl	300050a0 <FLASH_Erase_With_Lock>
30005306:	4642      	mov	r2, r8
30005308:	4648      	mov	r0, r9
3000530a:	2180      	movs	r1, #128	; 0x80
3000530c:	f7ff fef0 	bl	300050f0 <FLASH_TxData_With_Lock>
30005310:	4b49      	ldr	r3, [pc, #292]	; (30005438 <rewrite_bp+0x1a8>)
30005312:	aa01      	add	r2, sp, #4
30005314:	2104      	movs	r1, #4
30005316:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
3000531a:	4b48      	ldr	r3, [pc, #288]	; (3000543c <rewrite_bp+0x1ac>)
3000531c:	4798      	blx	r3
3000531e:	f89d 3006 	ldrb.w	r3, [sp, #6]
30005322:	f04f 0c01 	mov.w	ip, #1
30005326:	f8d5 e00a 	ldr.w	lr, [r5, #10]
3000532a:	682a      	ldr	r2, [r5, #0]
3000532c:	fa0c fc03 	lsl.w	ip, ip, r3
30005330:	f10e 0e0c 	add.w	lr, lr, #12
30005334:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005338:	45e6      	cmp	lr, ip
3000533a:	6831      	ldr	r1, [r6, #0]
3000533c:	f103 030c 	add.w	r3, r3, #12
30005340:	bf28      	it	cs
30005342:	46e6      	movcs	lr, ip
30005344:	459c      	cmp	ip, r3
30005346:	bf28      	it	cs
30005348:	469c      	movcs	ip, r3
3000534a:	bb6f      	cbnz	r7, 300053a8 <rewrite_bp+0x118>
3000534c:	1d28      	adds	r0, r5, #4
3000534e:	f1be 0f00 	cmp.w	lr, #0
30005352:	d00e      	beq.n	30005372 <rewrite_bp+0xe2>
30005354:	4486      	add	lr, r0
30005356:	f04f 37ff 	mov.w	r7, #4294967295
3000535a:	f810 3b01 	ldrb.w	r3, [r0], #1
3000535e:	407b      	eors	r3, r7
30005360:	4570      	cmp	r0, lr
30005362:	b2db      	uxtb	r3, r3
30005364:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005368:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000536c:	d1f5      	bne.n	3000535a <rewrite_bp+0xca>
3000536e:	ea6f 0e07 	mvn.w	lr, r7
30005372:	4572      	cmp	r2, lr
30005374:	d042      	beq.n	300053fc <rewrite_bp+0x16c>
30005376:	1d32      	adds	r2, r6, #4
30005378:	f1bc 0f00 	cmp.w	ip, #0
3000537c:	d00e      	beq.n	3000539c <rewrite_bp+0x10c>
3000537e:	4494      	add	ip, r2
30005380:	f04f 37ff 	mov.w	r7, #4294967295
30005384:	f812 3b01 	ldrb.w	r3, [r2], #1
30005388:	407b      	eors	r3, r7
3000538a:	4562      	cmp	r2, ip
3000538c:	b2db      	uxtb	r3, r3
3000538e:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005392:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005396:	d1f5      	bne.n	30005384 <rewrite_bp+0xf4>
30005398:	ea6f 0c07 	mvn.w	ip, r7
3000539c:	4561      	cmp	r1, ip
3000539e:	d03d      	beq.n	3000541c <rewrite_bp+0x18c>
300053a0:	4827      	ldr	r0, [pc, #156]	; (30005440 <rewrite_bp+0x1b0>)
300053a2:	f007 f9b5 	bl	3000c710 <__DiagPrintf_veneer>
300053a6:	e7fe      	b.n	300053a6 <rewrite_bp+0x116>
300053a8:	1d30      	adds	r0, r6, #4
300053aa:	f1bc 0f00 	cmp.w	ip, #0
300053ae:	d00e      	beq.n	300053ce <rewrite_bp+0x13e>
300053b0:	4484      	add	ip, r0
300053b2:	f04f 37ff 	mov.w	r7, #4294967295
300053b6:	f810 3b01 	ldrb.w	r3, [r0], #1
300053ba:	407b      	eors	r3, r7
300053bc:	4560      	cmp	r0, ip
300053be:	b2db      	uxtb	r3, r3
300053c0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300053c4:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300053c8:	d1f5      	bne.n	300053b6 <rewrite_bp+0x126>
300053ca:	ea6f 0c07 	mvn.w	ip, r7
300053ce:	4561      	cmp	r1, ip
300053d0:	d024      	beq.n	3000541c <rewrite_bp+0x18c>
300053d2:	1d29      	adds	r1, r5, #4
300053d4:	f1be 0f00 	cmp.w	lr, #0
300053d8:	d00e      	beq.n	300053f8 <rewrite_bp+0x168>
300053da:	448e      	add	lr, r1
300053dc:	f04f 37ff 	mov.w	r7, #4294967295
300053e0:	f811 3b01 	ldrb.w	r3, [r1], #1
300053e4:	407b      	eors	r3, r7
300053e6:	4571      	cmp	r1, lr
300053e8:	b2db      	uxtb	r3, r3
300053ea:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300053ee:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300053f2:	d1f5      	bne.n	300053e0 <rewrite_bp+0x150>
300053f4:	ea6f 0e07 	mvn.w	lr, r7
300053f8:	4572      	cmp	r2, lr
300053fa:	d1d1      	bne.n	300053a0 <rewrite_bp+0x110>
300053fc:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30005400:	4810      	ldr	r0, [pc, #64]	; (30005444 <rewrite_bp+0x1b4>)
30005402:	f8d5 1006 	ldr.w	r1, [r5, #6]
30005406:	f007 f983 	bl	3000c710 <__DiagPrintf_veneer>
3000540a:	2001      	movs	r0, #1
3000540c:	b023      	add	sp, #140	; 0x8c
3000540e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30005412:	992a      	ldr	r1, [sp, #168]	; 0xa8
30005414:	4640      	mov	r0, r8
30005416:	f007 f963 	bl	3000c6e0 <____wrap_memcpy_veneer>
3000541a:	e74c      	b.n	300052b6 <rewrite_bp+0x26>
3000541c:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
30005420:	4809      	ldr	r0, [pc, #36]	; (30005448 <rewrite_bp+0x1b8>)
30005422:	f8d6 1006 	ldr.w	r1, [r6, #6]
30005426:	f007 f973 	bl	3000c710 <__DiagPrintf_veneer>
3000542a:	2002      	movs	r0, #2
3000542c:	b023      	add	sp, #140	; 0x8c
3000542e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30005432:	bf00      	nop
30005434:	3000ee80 	.word	0x3000ee80
30005438:	2001c01c 	.word	0x2001c01c
3000543c:	00009db5 	.word	0x00009db5
30005440:	3000ce64 	.word	0x3000ce64
30005444:	3000ce34 	.word	0x3000ce34
30005448:	3000ce4c 	.word	0x3000ce4c

3000544c <BOOT_SectionInit>:
3000544c:	4800      	ldr	r0, [pc, #0]	; (30005450 <BOOT_SectionInit+0x4>)
3000544e:	4770      	bx	lr
30005450:	60000020 	.word	0x60000020

30005454 <BOOT_NsStart>:
30005454:	b570      	push	{r4, r5, r6, lr}
30005456:	4c23      	ldr	r4, [pc, #140]	; (300054e4 <BOOT_NsStart+0x90>)
30005458:	4605      	mov	r5, r0
3000545a:	f384 8808 	msr	MSP, r4
3000545e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30005462:	2100      	movs	r1, #0
30005464:	4620      	mov	r0, r4
30005466:	4e20      	ldr	r6, [pc, #128]	; (300054e8 <BOOT_NsStart+0x94>)
30005468:	47b0      	blx	r6
3000546a:	4a20      	ldr	r2, [pc, #128]	; (300054ec <BOOT_NsStart+0x98>)
3000546c:	6953      	ldr	r3, [r2, #20]
3000546e:	03d9      	lsls	r1, r3, #15
30005470:	d50b      	bpl.n	3000548a <BOOT_NsStart+0x36>
30005472:	f3bf 8f4f 	dsb	sy
30005476:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
3000547a:	3420      	adds	r4, #32
3000547c:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
30005480:	d1f9      	bne.n	30005476 <BOOT_NsStart+0x22>
30005482:	f3bf 8f4f 	dsb	sy
30005486:	f3bf 8f6f 	isb	sy
3000548a:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
3000548e:	f383 8808 	msr	MSP, r3
30005492:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30005496:	2100      	movs	r1, #0
30005498:	4815      	ldr	r0, [pc, #84]	; (300054f0 <BOOT_NsStart+0x9c>)
3000549a:	47b0      	blx	r6
3000549c:	4a13      	ldr	r2, [pc, #76]	; (300054ec <BOOT_NsStart+0x98>)
3000549e:	6953      	ldr	r3, [r2, #20]
300054a0:	03db      	lsls	r3, r3, #15
300054a2:	d50c      	bpl.n	300054be <BOOT_NsStart+0x6a>
300054a4:	f3bf 8f4f 	dsb	sy
300054a8:	4b11      	ldr	r3, [pc, #68]	; (300054f0 <BOOT_NsStart+0x9c>)
300054aa:	490e      	ldr	r1, [pc, #56]	; (300054e4 <BOOT_NsStart+0x90>)
300054ac:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
300054b0:	3320      	adds	r3, #32
300054b2:	428b      	cmp	r3, r1
300054b4:	d1fa      	bne.n	300054ac <BOOT_NsStart+0x58>
300054b6:	f3bf 8f4f 	dsb	sy
300054ba:	f3bf 8f6f 	isb	sy
300054be:	f025 0501 	bic.w	r5, r5, #1
300054c2:	086d      	lsrs	r5, r5, #1
300054c4:	006d      	lsls	r5, r5, #1
300054c6:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
300054ca:	ed2d 8b10 	vpush	{d8-d15}
300054ce:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
300054d2:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
300054d6:	47ac      	blxns	r5
300054d8:	ecbd 8b10 	vpop	{d8-d15}
300054dc:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
300054e0:	bd70      	pop	{r4, r5, r6, pc}
300054e2:	bf00      	nop
300054e4:	30002000 	.word	0x30002000
300054e8:	00012be5 	.word	0x00012be5
300054ec:	e000ed00 	.word	0xe000ed00
300054f0:	30001000 	.word	0x30001000

300054f4 <BOOT_RccConfig>:
300054f4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300054f8:	4c2f      	ldr	r4, [pc, #188]	; (300055b8 <BOOT_RccConfig+0xc4>)
300054fa:	b090      	sub	sp, #64	; 0x40
300054fc:	4f2f      	ldr	r7, [pc, #188]	; (300055bc <BOOT_RccConfig+0xc8>)
300054fe:	466d      	mov	r5, sp
30005500:	f10d 0c20 	add.w	ip, sp, #32
30005504:	683e      	ldr	r6, [r7, #0]
30005506:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000550a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000550e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30005512:	f104 0310 	add.w	r3, r4, #16
30005516:	2400      	movs	r4, #0
30005518:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000551a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000551e:	1c72      	adds	r2, r6, #1
30005520:	e9cd 4404 	strd	r4, r4, [sp, #16]
30005524:	e9cd 4406 	strd	r4, r4, [sp, #24]
30005528:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000552c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30005530:	d03f      	beq.n	300055b2 <BOOT_RccConfig+0xbe>
30005532:	463a      	mov	r2, r7
30005534:	6891      	ldr	r1, [r2, #8]
30005536:	0fb3      	lsrs	r3, r6, #30
30005538:	a810      	add	r0, sp, #64	; 0x40
3000553a:	2901      	cmp	r1, #1
3000553c:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30005540:	d10e      	bne.n	30005560 <BOOT_RccConfig+0x6c>
30005542:	f853 0c30 	ldr.w	r0, [r3, #-48]
30005546:	6854      	ldr	r4, [r2, #4]
30005548:	4330      	orrs	r0, r6
3000554a:	ae10      	add	r6, sp, #64	; 0x40
3000554c:	0fa1      	lsrs	r1, r4, #30
3000554e:	f843 0c30 	str.w	r0, [r3, #-48]
30005552:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30005556:	f851 3c10 	ldr.w	r3, [r1, #-16]
3000555a:	4323      	orrs	r3, r4
3000555c:	f841 3c10 	str.w	r3, [r1, #-16]
30005560:	f852 6f0c 	ldr.w	r6, [r2, #12]!
30005564:	1c73      	adds	r3, r6, #1
30005566:	d1e5      	bne.n	30005534 <BOOT_RccConfig+0x40>
30005568:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000556a:	9c04      	ldr	r4, [sp, #16]
3000556c:	af0d      	add	r7, sp, #52	; 0x34
3000556e:	ae05      	add	r6, sp, #20
30005570:	4a13      	ldr	r2, [pc, #76]	; (300055c0 <BOOT_RccConfig+0xcc>)
30005572:	f85c 0b04 	ldr.w	r0, [ip], #4
30005576:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
3000557a:	f855 1b04 	ldr.w	r1, [r5], #4
3000557e:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
30005582:	f850 8002 	ldr.w	r8, [r0, r2]
30005586:	45e6      	cmp	lr, ip
30005588:	ea43 0308 	orr.w	r3, r3, r8
3000558c:	5083      	str	r3, [r0, r2]
3000558e:	588b      	ldr	r3, [r1, r2]
30005590:	ea44 0403 	orr.w	r4, r4, r3
30005594:	508c      	str	r4, [r1, r2]
30005596:	d004      	beq.n	300055a2 <BOOT_RccConfig+0xae>
30005598:	f857 3b04 	ldr.w	r3, [r7], #4
3000559c:	f856 4b04 	ldr.w	r4, [r6], #4
300055a0:	e7e7      	b.n	30005572 <BOOT_RccConfig+0x7e>
300055a2:	200a      	movs	r0, #10
300055a4:	4b07      	ldr	r3, [pc, #28]	; (300055c4 <BOOT_RccConfig+0xd0>)
300055a6:	4798      	blx	r3
300055a8:	4b07      	ldr	r3, [pc, #28]	; (300055c8 <BOOT_RccConfig+0xd4>)
300055aa:	b010      	add	sp, #64	; 0x40
300055ac:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300055b0:	4718      	bx	r3
300055b2:	4623      	mov	r3, r4
300055b4:	e7da      	b.n	3000556c <BOOT_RccConfig+0x78>
300055b6:	bf00      	nop
300055b8:	3000e8c4 	.word	0x3000e8c4
300055bc:	3000ecc4 	.word	0x3000ecc4
300055c0:	42008000 	.word	0x42008000
300055c4:	00009b2d 	.word	0x00009b2d
300055c8:	300080f1 	.word	0x300080f1

300055cc <BOOT_CACHEWRR_Set>:
300055cc:	4b14      	ldr	r3, [pc, #80]	; (30005620 <BOOT_CACHEWRR_Set+0x54>)
300055ce:	b430      	push	{r4, r5}
300055d0:	681a      	ldr	r2, [r3, #0]
300055d2:	2a01      	cmp	r2, #1
300055d4:	d10f      	bne.n	300055f6 <BOOT_CACHEWRR_Set+0x2a>
300055d6:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
300055da:	685d      	ldr	r5, [r3, #4]
300055dc:	689c      	ldr	r4, [r3, #8]
300055de:	6882      	ldr	r2, [r0, #8]
300055e0:	4910      	ldr	r1, [pc, #64]	; (30005624 <BOOT_CACHEWRR_Set+0x58>)
300055e2:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
300055e6:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
300055ea:	6082      	str	r2, [r0, #8]
300055ec:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
300055f0:	68da      	ldr	r2, [r3, #12]
300055f2:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
300055f6:	691a      	ldr	r2, [r3, #16]
300055f8:	2a01      	cmp	r2, #1
300055fa:	d10f      	bne.n	3000561c <BOOT_CACHEWRR_Set+0x50>
300055fc:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005600:	695d      	ldr	r5, [r3, #20]
30005602:	4a08      	ldr	r2, [pc, #32]	; (30005624 <BOOT_CACHEWRR_Set+0x58>)
30005604:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005608:	688b      	ldr	r3, [r1, #8]
3000560a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000560e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30005612:	608b      	str	r3, [r1, #8]
30005614:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30005618:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000561c:	bc30      	pop	{r4, r5}
3000561e:	4770      	bx	lr
30005620:	3000f4e8 	.word	0x3000f4e8
30005624:	e0042000 	.word	0xe0042000

30005628 <BOOT_TCMSet>:
30005628:	2300      	movs	r3, #0
3000562a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000562c:	4db3      	ldr	r5, [pc, #716]	; (300058fc <BOOT_TCMSet+0x2d4>)
3000562e:	461c      	mov	r4, r3
30005630:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30005634:	3307      	adds	r3, #7
30005636:	4282      	cmp	r2, r0
30005638:	d03c      	beq.n	300056b4 <BOOT_TCMSet+0x8c>
3000563a:	3401      	adds	r4, #1
3000563c:	2c07      	cmp	r4, #7
3000563e:	d1f7      	bne.n	30005630 <BOOT_TCMSet+0x8>
30005640:	f3bf 8f4f 	dsb	sy
30005644:	f3bf 8f6f 	isb	sy
30005648:	48ad      	ldr	r0, [pc, #692]	; (30005900 <BOOT_TCMSet+0x2d8>)
3000564a:	2200      	movs	r2, #0
3000564c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005650:	f3bf 8f4f 	dsb	sy
30005654:	f3bf 8f6f 	isb	sy
30005658:	6943      	ldr	r3, [r0, #20]
3000565a:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000565e:	6143      	str	r3, [r0, #20]
30005660:	f3bf 8f4f 	dsb	sy
30005664:	f3bf 8f6f 	isb	sy
30005668:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000566c:	f3bf 8f4f 	dsb	sy
30005670:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30005674:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005678:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000567c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30005680:	0164      	lsls	r4, r4, #5
30005682:	ea04 0106 	and.w	r1, r4, r6
30005686:	462b      	mov	r3, r5
30005688:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000568c:	3b01      	subs	r3, #1
3000568e:	1c5f      	adds	r7, r3, #1
30005690:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30005694:	d1f8      	bne.n	30005688 <BOOT_TCMSet+0x60>
30005696:	3c20      	subs	r4, #32
30005698:	f114 0f20 	cmn.w	r4, #32
3000569c:	d1f1      	bne.n	30005682 <BOOT_TCMSet+0x5a>
3000569e:	f3bf 8f4f 	dsb	sy
300056a2:	6943      	ldr	r3, [r0, #20]
300056a4:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300056a8:	6143      	str	r3, [r0, #20]
300056aa:	f3bf 8f4f 	dsb	sy
300056ae:	f3bf 8f6f 	isb	sy
300056b2:	bdf0      	pop	{r4, r5, r6, r7, pc}
300056b4:	2c00      	cmp	r4, #0
300056b6:	d0c3      	beq.n	30005640 <BOOT_TCMSet+0x18>
300056b8:	f3bf 8f4f 	dsb	sy
300056bc:	f3bf 8f6f 	isb	sy
300056c0:	4890      	ldr	r0, [pc, #576]	; (30005904 <BOOT_TCMSet+0x2dc>)
300056c2:	2300      	movs	r3, #0
300056c4:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
300056c8:	f3bf 8f4f 	dsb	sy
300056cc:	f3bf 8f6f 	isb	sy
300056d0:	6942      	ldr	r2, [r0, #20]
300056d2:	03d2      	lsls	r2, r2, #15
300056d4:	d520      	bpl.n	30005718 <BOOT_TCMSet+0xf0>
300056d6:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300056da:	f3bf 8f4f 	dsb	sy
300056de:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300056e2:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300056e6:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300056ea:	f3c3 334e 	ubfx	r3, r3, #13, #15
300056ee:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300056f2:	ea0c 0106 	and.w	r1, ip, r6
300056f6:	4673      	mov	r3, lr
300056f8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056fc:	3b01      	subs	r3, #1
300056fe:	1c5f      	adds	r7, r3, #1
30005700:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30005704:	d1f8      	bne.n	300056f8 <BOOT_TCMSet+0xd0>
30005706:	f1ac 0c20 	sub.w	ip, ip, #32
3000570a:	f11c 0f20 	cmn.w	ip, #32
3000570e:	d1f0      	bne.n	300056f2 <BOOT_TCMSet+0xca>
30005710:	f3bf 8f4f 	dsb	sy
30005714:	f3bf 8f6f 	isb	sy
30005718:	f3bf 8f4f 	dsb	sy
3000571c:	f3bf 8f6f 	isb	sy
30005720:	4878      	ldr	r0, [pc, #480]	; (30005904 <BOOT_TCMSet+0x2dc>)
30005722:	2200      	movs	r2, #0
30005724:	6943      	ldr	r3, [r0, #20]
30005726:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000572a:	6143      	str	r3, [r0, #20]
3000572c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005730:	f3bf 8f4f 	dsb	sy
30005734:	f3bf 8f6f 	isb	sy
30005738:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000573c:	f3bf 8f4f 	dsb	sy
30005740:	6943      	ldr	r3, [r0, #20]
30005742:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30005746:	6143      	str	r3, [r0, #20]
30005748:	f3bf 8f4f 	dsb	sy
3000574c:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30005750:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30005754:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30005758:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000575c:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30005760:	ea0c 010e 	and.w	r1, ip, lr
30005764:	4633      	mov	r3, r6
30005766:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000576a:	3b01      	subs	r3, #1
3000576c:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30005770:	1c5a      	adds	r2, r3, #1
30005772:	d1f8      	bne.n	30005766 <BOOT_TCMSet+0x13e>
30005774:	f1ac 0c20 	sub.w	ip, ip, #32
30005778:	f11c 0f20 	cmn.w	ip, #32
3000577c:	d1f0      	bne.n	30005760 <BOOT_TCMSet+0x138>
3000577e:	f3bf 8f4f 	dsb	sy
30005782:	f3bf 8f6f 	isb	sy
30005786:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
3000578a:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
3000578e:	00e3      	lsls	r3, r4, #3
30005790:	6b41      	ldr	r1, [r0, #52]	; 0x34
30005792:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005796:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
3000579a:	6341      	str	r1, [r0, #52]	; 0x34
3000579c:	6911      	ldr	r1, [r2, #16]
3000579e:	2903      	cmp	r1, #3
300057a0:	d00f      	beq.n	300057c2 <BOOT_TCMSet+0x19a>
300057a2:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300057a6:	4858      	ldr	r0, [pc, #352]	; (30005908 <BOOT_TCMSet+0x2e0>)
300057a8:	f04f 0c00 	mov.w	ip, #0
300057ac:	4f57      	ldr	r7, [pc, #348]	; (3000590c <BOOT_TCMSet+0x2e4>)
300057ae:	68b2      	ldr	r2, [r6, #8]
300057b0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300057b4:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300057b8:	60b2      	str	r2, [r6, #8]
300057ba:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300057be:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
300057c2:	1b1a      	subs	r2, r3, r4
300057c4:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300057c8:	68d1      	ldr	r1, [r2, #12]
300057ca:	2903      	cmp	r1, #3
300057cc:	d00f      	beq.n	300057ee <BOOT_TCMSet+0x1c6>
300057ce:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300057d2:	484d      	ldr	r0, [pc, #308]	; (30005908 <BOOT_TCMSet+0x2e0>)
300057d4:	f04f 0c00 	mov.w	ip, #0
300057d8:	4f4c      	ldr	r7, [pc, #304]	; (3000590c <BOOT_TCMSet+0x2e4>)
300057da:	68b2      	ldr	r2, [r6, #8]
300057dc:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
300057e0:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
300057e4:	60b2      	str	r2, [r6, #8]
300057e6:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
300057ea:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
300057ee:	1b1a      	subs	r2, r3, r4
300057f0:	4945      	ldr	r1, [pc, #276]	; (30005908 <BOOT_TCMSet+0x2e0>)
300057f2:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300057f6:	6950      	ldr	r0, [r2, #20]
300057f8:	6608      	str	r0, [r1, #96]	; 0x60
300057fa:	6990      	ldr	r0, [r2, #24]
300057fc:	6648      	str	r0, [r1, #100]	; 0x64
300057fe:	6852      	ldr	r2, [r2, #4]
30005800:	2a01      	cmp	r2, #1
30005802:	d04b      	beq.n	3000589c <BOOT_TCMSet+0x274>
30005804:	1b1b      	subs	r3, r3, r4
30005806:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000580a:	68ab      	ldr	r3, [r5, #8]
3000580c:	2b01      	cmp	r3, #1
3000580e:	f47f af50 	bne.w	300056b2 <BOOT_TCMSet+0x8a>
30005812:	483b      	ldr	r0, [pc, #236]	; (30005900 <BOOT_TCMSet+0x2d8>)
30005814:	2300      	movs	r3, #0
30005816:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000581a:	f3bf 8f4f 	dsb	sy
3000581e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30005822:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005826:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000582a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000582e:	0164      	lsls	r4, r4, #5
30005830:	ea04 0106 	and.w	r1, r4, r6
30005834:	462b      	mov	r3, r5
30005836:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000583a:	3b01      	subs	r3, #1
3000583c:	1c5f      	adds	r7, r3, #1
3000583e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30005842:	d1f8      	bne.n	30005836 <BOOT_TCMSet+0x20e>
30005844:	3c20      	subs	r4, #32
30005846:	f114 0f20 	cmn.w	r4, #32
3000584a:	d1f1      	bne.n	30005830 <BOOT_TCMSet+0x208>
3000584c:	f3bf 8f4f 	dsb	sy
30005850:	6943      	ldr	r3, [r0, #20]
30005852:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005856:	6143      	str	r3, [r0, #20]
30005858:	f3bf 8f4f 	dsb	sy
3000585c:	f3bf 8f6f 	isb	sy
30005860:	4828      	ldr	r0, [pc, #160]	; (30005904 <BOOT_TCMSet+0x2dc>)
30005862:	2300      	movs	r3, #0
30005864:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005868:	f3bf 8f4f 	dsb	sy
3000586c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30005870:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005874:	f3c5 344e 	ubfx	r4, r5, #13, #15
30005878:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000587c:	0164      	lsls	r4, r4, #5
3000587e:	ea04 0106 	and.w	r1, r4, r6
30005882:	462b      	mov	r3, r5
30005884:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005888:	3b01      	subs	r3, #1
3000588a:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000588e:	1c5a      	adds	r2, r3, #1
30005890:	d1f8      	bne.n	30005884 <BOOT_TCMSet+0x25c>
30005892:	3c20      	subs	r4, #32
30005894:	f114 0f20 	cmn.w	r4, #32
30005898:	d1f1      	bne.n	3000587e <BOOT_TCMSet+0x256>
3000589a:	e700      	b.n	3000569e <BOOT_TCMSet+0x76>
3000589c:	f3bf 8f4f 	dsb	sy
300058a0:	f3bf 8f6f 	isb	sy
300058a4:	4a16      	ldr	r2, [pc, #88]	; (30005900 <BOOT_TCMSet+0x2d8>)
300058a6:	2000      	movs	r0, #0
300058a8:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300058ac:	f3bf 8f4f 	dsb	sy
300058b0:	f3bf 8f6f 	isb	sy
300058b4:	6951      	ldr	r1, [r2, #20]
300058b6:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300058ba:	6151      	str	r1, [r2, #20]
300058bc:	f3bf 8f4f 	dsb	sy
300058c0:	f3bf 8f6f 	isb	sy
300058c4:	f3bf 8f4f 	dsb	sy
300058c8:	f3bf 8f6f 	isb	sy
300058cc:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
300058d0:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300058d4:	f3bf 8f4f 	dsb	sy
300058d8:	f3bf 8f6f 	isb	sy
300058dc:	6951      	ldr	r1, [r2, #20]
300058de:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300058e2:	6151      	str	r1, [r2, #20]
300058e4:	f3bf 8f4f 	dsb	sy
300058e8:	f3bf 8f6f 	isb	sy
300058ec:	1b1b      	subs	r3, r3, r4
300058ee:	eb05 0583 	add.w	r5, r5, r3, lsl #2
300058f2:	68ab      	ldr	r3, [r5, #8]
300058f4:	2b01      	cmp	r3, #1
300058f6:	f47f aedc 	bne.w	300056b2 <BOOT_TCMSet+0x8a>
300058fa:	e78a      	b.n	30005812 <BOOT_TCMSet+0x1ea>
300058fc:	3000f424 	.word	0x3000f424
30005900:	e002ed00 	.word	0xe002ed00
30005904:	e000ed00 	.word	0xe000ed00
30005908:	e0042000 	.word	0xe0042000
3000590c:	ffff0000 	.word	0xffff0000

30005910 <BOOT_GRstConfig>:
30005910:	b538      	push	{r3, r4, r5, lr}
30005912:	4c09      	ldr	r4, [pc, #36]	; (30005938 <BOOT_GRstConfig+0x28>)
30005914:	f241 050a 	movw	r5, #4106	; 0x100a
30005918:	2028      	movs	r0, #40	; 0x28
3000591a:	4a08      	ldr	r2, [pc, #32]	; (3000593c <BOOT_GRstConfig+0x2c>)
3000591c:	8823      	ldrh	r3, [r4, #0]
3000591e:	b29b      	uxth	r3, r3
30005920:	431d      	orrs	r5, r3
30005922:	8025      	strh	r5, [r4, #0]
30005924:	4790      	blx	r2
30005926:	88a3      	ldrh	r3, [r4, #4]
30005928:	b29b      	uxth	r3, r3
3000592a:	ea23 0305 	bic.w	r3, r3, r5
3000592e:	80a3      	strh	r3, [r4, #4]
30005930:	8863      	ldrh	r3, [r4, #2]
30005932:	8065      	strh	r5, [r4, #2]
30005934:	bd38      	pop	{r3, r4, r5, pc}
30005936:	bf00      	nop
30005938:	42008200 	.word	0x42008200
3000593c:	00009b2d 	.word	0x00009b2d

30005940 <BOOT_DDR_LCDC_HPR>:
30005940:	4a04      	ldr	r2, [pc, #16]	; (30005954 <BOOT_DDR_LCDC_HPR+0x14>)
30005942:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005946:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
3000594a:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
3000594e:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005952:	4770      	bx	lr
30005954:	42008000 	.word	0x42008000

30005958 <BOOT_DDR_Init>:
30005958:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000595c:	b530      	push	{r4, r5, lr}
3000595e:	4d15      	ldr	r5, [pc, #84]	; (300059b4 <BOOT_DDR_Init+0x5c>)
30005960:	b099      	sub	sp, #100	; 0x64
30005962:	6b1c      	ldr	r4, [r3, #48]	; 0x30
30005964:	47a8      	blx	r5
30005966:	2803      	cmp	r0, #3
30005968:	d11e      	bne.n	300059a8 <BOOT_DDR_Init+0x50>
3000596a:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
3000596e:	4a12      	ldr	r2, [pc, #72]	; (300059b8 <BOOT_DDR_Init+0x60>)
30005970:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005974:	4911      	ldr	r1, [pc, #68]	; (300059bc <BOOT_DDR_Init+0x64>)
30005976:	a801      	add	r0, sp, #4
30005978:	631c      	str	r4, [r3, #48]	; 0x30
3000597a:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
3000597e:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
30005982:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
30005986:	4788      	blx	r1
30005988:	a801      	add	r0, sp, #4
3000598a:	4b0d      	ldr	r3, [pc, #52]	; (300059c0 <BOOT_DDR_Init+0x68>)
3000598c:	4798      	blx	r3
3000598e:	4b0d      	ldr	r3, [pc, #52]	; (300059c4 <BOOT_DDR_Init+0x6c>)
30005990:	4798      	blx	r3
30005992:	4b0d      	ldr	r3, [pc, #52]	; (300059c8 <BOOT_DDR_Init+0x70>)
30005994:	4798      	blx	r3
30005996:	4b0d      	ldr	r3, [pc, #52]	; (300059cc <BOOT_DDR_Init+0x74>)
30005998:	4798      	blx	r3
3000599a:	2101      	movs	r1, #1
3000599c:	f240 10ff 	movw	r0, #511	; 0x1ff
300059a0:	4b0b      	ldr	r3, [pc, #44]	; (300059d0 <BOOT_DDR_Init+0x78>)
300059a2:	4798      	blx	r3
300059a4:	b019      	add	sp, #100	; 0x64
300059a6:	bd30      	pop	{r4, r5, pc}
300059a8:	47a8      	blx	r5
300059aa:	2802      	cmp	r0, #2
300059ac:	d0dd      	beq.n	3000596a <BOOT_DDR_Init+0x12>
300059ae:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
300059b2:	e7dc      	b.n	3000596e <BOOT_DDR_Init+0x16>
300059b4:	0000c0f9 	.word	0x0000c0f9
300059b8:	42008000 	.word	0x42008000
300059bc:	3000820d 	.word	0x3000820d
300059c0:	30008711 	.word	0x30008711
300059c4:	3000b639 	.word	0x3000b639
300059c8:	300087e1 	.word	0x300087e1
300059cc:	300087e5 	.word	0x300087e5
300059d0:	3000b5e1 	.word	0x3000b5e1

300059d4 <BOOT_PSRAM_Init>:
300059d4:	b500      	push	{lr}
300059d6:	b089      	sub	sp, #36	; 0x24
300059d8:	4b1a      	ldr	r3, [pc, #104]	; (30005a44 <BOOT_PSRAM_Init+0x70>)
300059da:	a801      	add	r0, sp, #4
300059dc:	4798      	blx	r3
300059de:	a801      	add	r0, sp, #4
300059e0:	4b19      	ldr	r3, [pc, #100]	; (30005a48 <BOOT_PSRAM_Init+0x74>)
300059e2:	4798      	blx	r3
300059e4:	4b19      	ldr	r3, [pc, #100]	; (30005a4c <BOOT_PSRAM_Init+0x78>)
300059e6:	4798      	blx	r3
300059e8:	4b19      	ldr	r3, [pc, #100]	; (30005a50 <BOOT_PSRAM_Init+0x7c>)
300059ea:	4798      	blx	r3
300059ec:	2801      	cmp	r0, #1
300059ee:	d026      	beq.n	30005a3e <BOOT_PSRAM_Init+0x6a>
300059f0:	4b18      	ldr	r3, [pc, #96]	; (30005a54 <BOOT_PSRAM_Init+0x80>)
300059f2:	689b      	ldr	r3, [r3, #8]
300059f4:	079a      	lsls	r2, r3, #30
300059f6:	d41e      	bmi.n	30005a36 <BOOT_PSRAM_Init+0x62>
300059f8:	4b17      	ldr	r3, [pc, #92]	; (30005a58 <BOOT_PSRAM_Init+0x84>)
300059fa:	4798      	blx	r3
300059fc:	4b17      	ldr	r3, [pc, #92]	; (30005a5c <BOOT_PSRAM_Init+0x88>)
300059fe:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30005a02:	009b      	lsls	r3, r3, #2
30005a04:	d510      	bpl.n	30005a28 <BOOT_PSRAM_Init+0x54>
30005a06:	4a16      	ldr	r2, [pc, #88]	; (30005a60 <BOOT_PSRAM_Init+0x8c>)
30005a08:	4b16      	ldr	r3, [pc, #88]	; (30005a64 <BOOT_PSRAM_Init+0x90>)
30005a0a:	6812      	ldr	r2, [r2, #0]
30005a0c:	605a      	str	r2, [r3, #4]
30005a0e:	4a16      	ldr	r2, [pc, #88]	; (30005a68 <BOOT_PSRAM_Init+0x94>)
30005a10:	f04f 0c20 	mov.w	ip, #32
30005a14:	2101      	movs	r1, #1
30005a16:	4b15      	ldr	r3, [pc, #84]	; (30005a6c <BOOT_PSRAM_Init+0x98>)
30005a18:	6952      	ldr	r2, [r2, #20]
30005a1a:	4608      	mov	r0, r1
30005a1c:	fbbc f2f2 	udiv	r2, ip, r2
30005a20:	4798      	blx	r3
30005a22:	b009      	add	sp, #36	; 0x24
30005a24:	f85d fb04 	ldr.w	pc, [sp], #4
30005a28:	4b11      	ldr	r3, [pc, #68]	; (30005a70 <BOOT_PSRAM_Init+0x9c>)
30005a2a:	4798      	blx	r3
30005a2c:	4a0d      	ldr	r2, [pc, #52]	; (30005a64 <BOOT_PSRAM_Init+0x90>)
30005a2e:	4b0c      	ldr	r3, [pc, #48]	; (30005a60 <BOOT_PSRAM_Init+0x8c>)
30005a30:	6852      	ldr	r2, [r2, #4]
30005a32:	601a      	str	r2, [r3, #0]
30005a34:	e7eb      	b.n	30005a0e <BOOT_PSRAM_Init+0x3a>
30005a36:	480f      	ldr	r0, [pc, #60]	; (30005a74 <BOOT_PSRAM_Init+0xa0>)
30005a38:	f006 fe6a 	bl	3000c710 <__DiagPrintf_veneer>
30005a3c:	e7dc      	b.n	300059f8 <BOOT_PSRAM_Init+0x24>
30005a3e:	4b0e      	ldr	r3, [pc, #56]	; (30005a78 <BOOT_PSRAM_Init+0xa4>)
30005a40:	4798      	blx	r3
30005a42:	e7db      	b.n	300059fc <BOOT_PSRAM_Init+0x28>
30005a44:	30009389 	.word	0x30009389
30005a48:	300093a9 	.word	0x300093a9
30005a4c:	30009411 	.word	0x30009411
30005a50:	300080a5 	.word	0x300080a5
30005a54:	2001c00c 	.word	0x2001c00c
30005a58:	300096b5 	.word	0x300096b5
30005a5c:	42008000 	.word	0x42008000
30005a60:	30010600 	.word	0x30010600
30005a64:	41002000 	.word	0x41002000
30005a68:	300101a0 	.word	0x300101a0
30005a6c:	30009979 	.word	0x30009979
30005a70:	300096f1 	.word	0x300096f1
30005a74:	3000ce84 	.word	0x3000ce84
30005a78:	300095dd 	.word	0x300095dd

30005a7c <BOOT_WakeFromPG>:
30005a7c:	b538      	push	{r3, r4, r5, lr}
30005a7e:	4c49      	ldr	r4, [pc, #292]	; (30005ba4 <BOOT_WakeFromPG+0x128>)
30005a80:	f7ff f91a 	bl	30004cb8 <BOOT_RAM_TZCfg>
30005a84:	69a3      	ldr	r3, [r4, #24]
30005a86:	b10b      	cbz	r3, 30005a8c <BOOT_WakeFromPG+0x10>
30005a88:	4a47      	ldr	r2, [pc, #284]	; (30005ba8 <BOOT_WakeFromPG+0x12c>)
30005a8a:	6093      	str	r3, [r2, #8]
30005a8c:	4b47      	ldr	r3, [pc, #284]	; (30005bac <BOOT_WakeFromPG+0x130>)
30005a8e:	f06f 0c1a 	mvn.w	ip, #26
30005a92:	4847      	ldr	r0, [pc, #284]	; (30005bb0 <BOOT_WakeFromPG+0x134>)
30005a94:	f103 0e50 	add.w	lr, r3, #80	; 0x50
30005a98:	ebac 0c04 	sub.w	ip, ip, r4
30005a9c:	eb0c 0203 	add.w	r2, ip, r3
30005aa0:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005aa4:	4402      	add	r2, r0
30005aa6:	4573      	cmp	r3, lr
30005aa8:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005aac:	d1f6      	bne.n	30005a9c <BOOT_WakeFromPG+0x20>
30005aae:	6822      	ldr	r2, [r4, #0]
30005ab0:	4b3d      	ldr	r3, [pc, #244]	; (30005ba8 <BOOT_WakeFromPG+0x12c>)
30005ab2:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005ab6:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005aba:	6002      	str	r2, [r0, #0]
30005abc:	6045      	str	r5, [r0, #4]
30005abe:	6081      	str	r1, [r0, #8]
30005ac0:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005ac2:	4a3c      	ldr	r2, [pc, #240]	; (30005bb4 <BOOT_WakeFromPG+0x138>)
30005ac4:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005ac8:	4c3b      	ldr	r4, [pc, #236]	; (30005bb8 <BOOT_WakeFromPG+0x13c>)
30005aca:	6259      	str	r1, [r3, #36]	; 0x24
30005acc:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005ace:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005ad2:	6251      	str	r1, [r2, #36]	; 0x24
30005ad4:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005ad8:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005adc:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005ae0:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005ae4:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005ae8:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005aec:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005af0:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005af4:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005af8:	f7ff fd68 	bl	300055cc <BOOT_CACHEWRR_Set>
30005afc:	4b2f      	ldr	r3, [pc, #188]	; (30005bbc <BOOT_WakeFromPG+0x140>)
30005afe:	6818      	ldr	r0, [r3, #0]
30005b00:	f7ff fd92 	bl	30005628 <BOOT_TCMSet>
30005b04:	2201      	movs	r2, #1
30005b06:	492e      	ldr	r1, [pc, #184]	; (30005bc0 <BOOT_WakeFromPG+0x144>)
30005b08:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005b0c:	47a0      	blx	r4
30005b0e:	4b2d      	ldr	r3, [pc, #180]	; (30005bc4 <BOOT_WakeFromPG+0x148>)
30005b10:	4798      	blx	r3
30005b12:	4b2d      	ldr	r3, [pc, #180]	; (30005bc8 <BOOT_WakeFromPG+0x14c>)
30005b14:	4798      	blx	r3
30005b16:	2801      	cmp	r0, #1
30005b18:	d12d      	bne.n	30005b76 <BOOT_WakeFromPG+0xfa>
30005b1a:	4b2c      	ldr	r3, [pc, #176]	; (30005bcc <BOOT_WakeFromPG+0x150>)
30005b1c:	689b      	ldr	r3, [r3, #8]
30005b1e:	079b      	lsls	r3, r3, #30
30005b20:	d43c      	bmi.n	30005b9c <BOOT_WakeFromPG+0x120>
30005b22:	2201      	movs	r2, #1
30005b24:	492a      	ldr	r1, [pc, #168]	; (30005bd0 <BOOT_WakeFromPG+0x154>)
30005b26:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005b2a:	47a0      	blx	r4
30005b2c:	2201      	movs	r2, #1
30005b2e:	4929      	ldr	r1, [pc, #164]	; (30005bd4 <BOOT_WakeFromPG+0x158>)
30005b30:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005b34:	47a0      	blx	r4
30005b36:	f7ff ff0f 	bl	30005958 <BOOT_DDR_Init>
30005b3a:	4a27      	ldr	r2, [pc, #156]	; (30005bd8 <BOOT_WakeFromPG+0x15c>)
30005b3c:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005b40:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005b44:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005b48:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005b4c:	4a23      	ldr	r2, [pc, #140]	; (30005bdc <BOOT_WakeFromPG+0x160>)
30005b4e:	4c19      	ldr	r4, [pc, #100]	; (30005bb4 <BOOT_WakeFromPG+0x138>)
30005b50:	6893      	ldr	r3, [r2, #8]
30005b52:	4923      	ldr	r1, [pc, #140]	; (30005be0 <BOOT_WakeFromPG+0x164>)
30005b54:	6852      	ldr	r2, [r2, #4]
30005b56:	4823      	ldr	r0, [pc, #140]	; (30005be4 <BOOT_WakeFromPG+0x168>)
30005b58:	605a      	str	r2, [r3, #4]
30005b5a:	60a3      	str	r3, [r4, #8]
30005b5c:	4a22      	ldr	r2, [pc, #136]	; (30005be8 <BOOT_WakeFromPG+0x16c>)
30005b5e:	61c8      	str	r0, [r1, #28]
30005b60:	f382 8888 	msr	MSP_NS, r2
30005b64:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
30005b68:	f382 8809 	msr	PSP, r2
30005b6c:	6858      	ldr	r0, [r3, #4]
30005b6e:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30005b72:	f7ff bc6f 	b.w	30005454 <BOOT_NsStart>
30005b76:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005b7a:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005b7e:	2201      	movs	r2, #1
30005b80:	47a0      	blx	r4
30005b82:	4a15      	ldr	r2, [pc, #84]	; (30005bd8 <BOOT_WakeFromPG+0x15c>)
30005b84:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005b88:	f043 0302 	orr.w	r3, r3, #2
30005b8c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005b90:	f7ff ff20 	bl	300059d4 <BOOT_PSRAM_Init>
30005b94:	2096      	movs	r0, #150	; 0x96
30005b96:	4b15      	ldr	r3, [pc, #84]	; (30005bec <BOOT_WakeFromPG+0x170>)
30005b98:	4798      	blx	r3
30005b9a:	e7d7      	b.n	30005b4c <BOOT_WakeFromPG+0xd0>
30005b9c:	4814      	ldr	r0, [pc, #80]	; (30005bf0 <BOOT_WakeFromPG+0x174>)
30005b9e:	f006 fdb7 	bl	3000c710 <__DiagPrintf_veneer>
30005ba2:	e7be      	b.n	30005b22 <BOOT_WakeFromPG+0xa6>
30005ba4:	30010594 	.word	0x30010594
30005ba8:	e000ed00 	.word	0xe000ed00
30005bac:	300105af 	.word	0x300105af
30005bb0:	e000e100 	.word	0xe000e100
30005bb4:	e002ed00 	.word	0xe002ed00
30005bb8:	0000b479 	.word	0x0000b479
30005bbc:	3000ebdc 	.word	0x3000ebdc
30005bc0:	40001000 	.word	0x40001000
30005bc4:	0000d835 	.word	0x0000d835
30005bc8:	30007fcd 	.word	0x30007fcd
30005bcc:	2001c00c 	.word	0x2001c00c
30005bd0:	40080000 	.word	0x40080000
30005bd4:	40040000 	.word	0x40040000
30005bd8:	42008000 	.word	0x42008000
30005bdc:	60000020 	.word	0x60000020
30005be0:	30000000 	.word	0x30000000
30005be4:	30004e55 	.word	0x30004e55
30005be8:	2001bffc 	.word	0x2001bffc
30005bec:	00009b2d 	.word	0x00009b2d
30005bf0:	3000cea8 	.word	0x3000cea8

30005bf4 <BOOT_SCBConfig_HP>:
30005bf4:	4b0e      	ldr	r3, [pc, #56]	; (30005c30 <BOOT_SCBConfig_HP+0x3c>)
30005bf6:	4a0f      	ldr	r2, [pc, #60]	; (30005c34 <BOOT_SCBConfig_HP+0x40>)
30005bf8:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005bfa:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005bfe:	6259      	str	r1, [r3, #36]	; 0x24
30005c00:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005c02:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005c06:	6251      	str	r1, [r2, #36]	; 0x24
30005c08:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005c0c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005c10:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005c14:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005c18:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005c1c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005c20:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005c24:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005c28:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005c2c:	4770      	bx	lr
30005c2e:	bf00      	nop
30005c30:	e000ed00 	.word	0xe000ed00
30005c34:	e002ed00 	.word	0xe002ed00

30005c38 <BOOT_SCBVTORBackup_HP>:
30005c38:	4a02      	ldr	r2, [pc, #8]	; (30005c44 <BOOT_SCBVTORBackup_HP+0xc>)
30005c3a:	4b03      	ldr	r3, [pc, #12]	; (30005c48 <BOOT_SCBVTORBackup_HP+0x10>)
30005c3c:	6892      	ldr	r2, [r2, #8]
30005c3e:	619a      	str	r2, [r3, #24]
30005c40:	4770      	bx	lr
30005c42:	bf00      	nop
30005c44:	e000ed00 	.word	0xe000ed00
30005c48:	30010594 	.word	0x30010594

30005c4c <BOOT_SCBVTORReFill_HP>:
30005c4c:	4a02      	ldr	r2, [pc, #8]	; (30005c58 <BOOT_SCBVTORReFill_HP+0xc>)
30005c4e:	4b03      	ldr	r3, [pc, #12]	; (30005c5c <BOOT_SCBVTORReFill_HP+0x10>)
30005c50:	6992      	ldr	r2, [r2, #24]
30005c52:	609a      	str	r2, [r3, #8]
30005c54:	4770      	bx	lr
30005c56:	bf00      	nop
30005c58:	30010594 	.word	0x30010594
30005c5c:	e000ed00 	.word	0xe000ed00

30005c60 <BOOT_NVICBackup_HP>:
30005c60:	491a      	ldr	r1, [pc, #104]	; (30005ccc <BOOT_NVICBackup_HP+0x6c>)
30005c62:	f06f 001a 	mvn.w	r0, #26
30005c66:	b410      	push	{r4}
30005c68:	4c19      	ldr	r4, [pc, #100]	; (30005cd0 <BOOT_NVICBackup_HP+0x70>)
30005c6a:	680a      	ldr	r2, [r1, #0]
30005c6c:	f104 031b 	add.w	r3, r4, #27
30005c70:	1b00      	subs	r0, r0, r4
30005c72:	6022      	str	r2, [r4, #0]
30005c74:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005c78:	684a      	ldr	r2, [r1, #4]
30005c7a:	6062      	str	r2, [r4, #4]
30005c7c:	688a      	ldr	r2, [r1, #8]
30005c7e:	60a2      	str	r2, [r4, #8]
30005c80:	18c2      	adds	r2, r0, r3
30005c82:	440a      	add	r2, r1
30005c84:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005c88:	f803 2f01 	strb.w	r2, [r3, #1]!
30005c8c:	4563      	cmp	r3, ip
30005c8e:	d1f7      	bne.n	30005c80 <BOOT_NVICBackup_HP+0x20>
30005c90:	4b10      	ldr	r3, [pc, #64]	; (30005cd4 <BOOT_NVICBackup_HP+0x74>)
30005c92:	695b      	ldr	r3, [r3, #20]
30005c94:	03db      	lsls	r3, r3, #15
30005c96:	d512      	bpl.n	30005cbe <BOOT_NVICBackup_HP+0x5e>
30005c98:	f014 011f 	ands.w	r1, r4, #31
30005c9c:	4b0c      	ldr	r3, [pc, #48]	; (30005cd0 <BOOT_NVICBackup_HP+0x70>)
30005c9e:	d111      	bne.n	30005cc4 <BOOT_NVICBackup_HP+0x64>
30005ca0:	216c      	movs	r1, #108	; 0x6c
30005ca2:	f3bf 8f4f 	dsb	sy
30005ca6:	480b      	ldr	r0, [pc, #44]	; (30005cd4 <BOOT_NVICBackup_HP+0x74>)
30005ca8:	4419      	add	r1, r3
30005caa:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005cae:	3320      	adds	r3, #32
30005cb0:	1aca      	subs	r2, r1, r3
30005cb2:	2a00      	cmp	r2, #0
30005cb4:	dcf9      	bgt.n	30005caa <BOOT_NVICBackup_HP+0x4a>
30005cb6:	f3bf 8f4f 	dsb	sy
30005cba:	f3bf 8f6f 	isb	sy
30005cbe:	f85d 4b04 	ldr.w	r4, [sp], #4
30005cc2:	4770      	bx	lr
30005cc4:	f024 031f 	bic.w	r3, r4, #31
30005cc8:	316c      	adds	r1, #108	; 0x6c
30005cca:	e7ea      	b.n	30005ca2 <BOOT_NVICBackup_HP+0x42>
30005ccc:	e000e100 	.word	0xe000e100
30005cd0:	30010594 	.word	0x30010594
30005cd4:	e000ed00 	.word	0xe000ed00

30005cd8 <BOOT_NVICReFill_HP>:
30005cd8:	b510      	push	{r4, lr}
30005cda:	f06f 0c1a 	mvn.w	ip, #26
30005cde:	4c0d      	ldr	r4, [pc, #52]	; (30005d14 <BOOT_NVICReFill_HP+0x3c>)
30005ce0:	480d      	ldr	r0, [pc, #52]	; (30005d18 <BOOT_NVICReFill_HP+0x40>)
30005ce2:	f104 031b 	add.w	r3, r4, #27
30005ce6:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005cea:	ebac 0c04 	sub.w	ip, ip, r4
30005cee:	eb0c 0203 	add.w	r2, ip, r3
30005cf2:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005cf6:	4402      	add	r2, r0
30005cf8:	4573      	cmp	r3, lr
30005cfa:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005cfe:	d1f6      	bne.n	30005cee <BOOT_NVICReFill_HP+0x16>
30005d00:	e9d4 3100 	ldrd	r3, r1, [r4]
30005d04:	68a2      	ldr	r2, [r4, #8]
30005d06:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005d0a:	6003      	str	r3, [r0, #0]
30005d0c:	6041      	str	r1, [r0, #4]
30005d0e:	6082      	str	r2, [r0, #8]
30005d10:	bd10      	pop	{r4, pc}
30005d12:	bf00      	nop
30005d14:	30010594 	.word	0x30010594
30005d18:	e000e100 	.word	0xe000e100

30005d1c <BOOT_ImgCopy>:
30005d1c:	b510      	push	{r4, lr}
30005d1e:	b082      	sub	sp, #8
30005d20:	4b09      	ldr	r3, [pc, #36]	; (30005d48 <BOOT_ImgCopy+0x2c>)
30005d22:	4604      	mov	r4, r0
30005d24:	e9cd 2100 	strd	r2, r1, [sp]
30005d28:	4798      	blx	r3
30005d2a:	e9dd 2100 	ldrd	r2, r1, [sp]
30005d2e:	b128      	cbz	r0, 30005d3c <BOOT_ImgCopy+0x20>
30005d30:	4620      	mov	r0, r4
30005d32:	4b06      	ldr	r3, [pc, #24]	; (30005d4c <BOOT_ImgCopy+0x30>)
30005d34:	b002      	add	sp, #8
30005d36:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005d3a:	4718      	bx	r3
30005d3c:	4620      	mov	r0, r4
30005d3e:	b002      	add	sp, #8
30005d40:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005d44:	f002 bf96 	b.w	30008c74 <NandImgCopy>
30005d48:	3000a219 	.word	0x3000a219
30005d4c:	00012a1d 	.word	0x00012a1d

30005d50 <BOOT_LoadImages>:
30005d50:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005d54:	4b57      	ldr	r3, [pc, #348]	; (30005eb4 <BOOT_LoadImages+0x164>)
30005d56:	2700      	movs	r7, #0
30005d58:	2104      	movs	r1, #4
30005d5a:	2601      	movs	r6, #1
30005d5c:	ed2d 8b02 	vpush	{d8}
30005d60:	b0b7      	sub	sp, #220	; 0xdc
30005d62:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005d66:	4b54      	ldr	r3, [pc, #336]	; (30005eb8 <BOOT_LoadImages+0x168>)
30005d68:	aa11      	add	r2, sp, #68	; 0x44
30005d6a:	9703      	str	r7, [sp, #12]
30005d6c:	9705      	str	r7, [sp, #20]
30005d6e:	4798      	blx	r3
30005d70:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30005d74:	409e      	lsls	r6, r3
30005d76:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
30005d7a:	f000 823a 	beq.w	300061f2 <BOOT_LoadImages+0x4a2>
30005d7e:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
30005d82:	f000 8375 	beq.w	30006470 <BOOT_LoadImages+0x720>
30005d86:	4c4d      	ldr	r4, [pc, #308]	; (30005ebc <BOOT_LoadImages+0x16c>)
30005d88:	f04f 0800 	mov.w	r8, #0
30005d8c:	4d4c      	ldr	r5, [pc, #304]	; (30005ec0 <BOOT_LoadImages+0x170>)
30005d8e:	eb06 0904 	add.w	r9, r6, r4
30005d92:	484c      	ldr	r0, [pc, #304]	; (30005ec4 <BOOT_LoadImages+0x174>)
30005d94:	4435      	add	r5, r6
30005d96:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
30005d9a:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
30005d9e:	f7ff fa01 	bl	300051a4 <mcc_init_crc32>
30005da2:	f8d9 3004 	ldr.w	r3, [r9, #4]
30005da6:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005daa:	f899 200c 	ldrb.w	r2, [r9, #12]
30005dae:	9308      	str	r3, [sp, #32]
30005db0:	3b01      	subs	r3, #1
30005db2:	9206      	str	r2, [sp, #24]
30005db4:	3303      	adds	r3, #3
30005db6:	686a      	ldr	r2, [r5, #4]
30005db8:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005dbc:	9204      	str	r2, [sp, #16]
30005dbe:	f200 81bc 	bhi.w	3000613a <BOOT_LoadImages+0x3ea>
30005dc2:	f8d9 2008 	ldr.w	r2, [r9, #8]
30005dc6:	9b04      	ldr	r3, [sp, #16]
30005dc8:	3a01      	subs	r2, #1
30005dca:	3b01      	subs	r3, #1
30005dcc:	3203      	adds	r2, #3
30005dce:	d868      	bhi.n	30005ea2 <BOOT_LoadImages+0x152>
30005dd0:	9a06      	ldr	r2, [sp, #24]
30005dd2:	9307      	str	r3, [sp, #28]
30005dd4:	2a01      	cmp	r2, #1
30005dd6:	f240 8305 	bls.w	300063e4 <BOOT_LoadImages+0x694>
30005dda:	483b      	ldr	r0, [pc, #236]	; (30005ec8 <BOOT_LoadImages+0x178>)
30005ddc:	f006 fc98 	bl	3000c710 <__DiagPrintf_veneer>
30005de0:	9b07      	ldr	r3, [sp, #28]
30005de2:	3303      	adds	r3, #3
30005de4:	d864      	bhi.n	30005eb0 <BOOT_LoadImages+0x160>
30005de6:	f10a 3aff 	add.w	sl, sl, #4294967295
30005dea:	f11a 0f03 	cmn.w	sl, #3
30005dee:	f200 81aa 	bhi.w	30006146 <BOOT_LoadImages+0x3f6>
30005df2:	f1bb 0f01 	cmp.w	fp, #1
30005df6:	f200 8347 	bhi.w	30006488 <BOOT_LoadImages+0x738>
30005dfa:	9904      	ldr	r1, [sp, #16]
30005dfc:	4833      	ldr	r0, [pc, #204]	; (30005ecc <BOOT_LoadImages+0x17c>)
30005dfe:	f006 fc87 	bl	3000c710 <__DiagPrintf_veneer>
30005e02:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005e06:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005e0a:	4c2e      	ldr	r4, [pc, #184]	; (30005ec4 <BOOT_LoadImages+0x174>)
30005e0c:	9307      	str	r3, [sp, #28]
30005e0e:	2300      	movs	r3, #0
30005e10:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005e14:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005e18:	9309      	str	r3, [sp, #36]	; 0x24
30005e1a:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005ee4 <BOOT_LoadImages+0x194>
30005e1e:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30005e22:	6829      	ldr	r1, [r5, #0]
30005e24:	f04f 30ff 	mov.w	r0, #4294967295
30005e28:	44b4      	add	ip, r6
30005e2a:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005e2e:	4042      	eors	r2, r0
30005e30:	459c      	cmp	ip, r3
30005e32:	b2d2      	uxtb	r2, r2
30005e34:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005e38:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005e3c:	d1f5      	bne.n	30005e2a <BOOT_LoadImages+0xda>
30005e3e:	43c2      	mvns	r2, r0
30005e40:	930f      	str	r3, [sp, #60]	; 0x3c
30005e42:	4291      	cmp	r1, r2
30005e44:	f000 8301 	beq.w	3000644a <BOOT_LoadImages+0x6fa>
30005e48:	4821      	ldr	r0, [pc, #132]	; (30005ed0 <BOOT_LoadImages+0x180>)
30005e4a:	f006 fc61 	bl	3000c710 <__DiagPrintf_veneer>
30005e4e:	2200      	movs	r2, #0
30005e50:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005e52:	9909      	ldr	r1, [sp, #36]	; 0x24
30005e54:	2900      	cmp	r1, #0
30005e56:	d04e      	beq.n	30005ef6 <BOOT_LoadImages+0x1a6>
30005e58:	f899 100c 	ldrb.w	r1, [r9, #12]
30005e5c:	4553      	cmp	r3, sl
30005e5e:	9106      	str	r1, [sp, #24]
30005e60:	d342      	bcc.n	30005ee8 <BOOT_LoadImages+0x198>
30005e62:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005e66:	45e2      	cmp	sl, ip
30005e68:	d93e      	bls.n	30005ee8 <BOOT_LoadImages+0x198>
30005e6a:	9807      	ldr	r0, [sp, #28]
30005e6c:	4560      	cmp	r0, ip
30005e6e:	f240 8203 	bls.w	30006278 <BOOT_LoadImages+0x528>
30005e72:	4283      	cmp	r3, r0
30005e74:	f0c0 8200 	bcc.w	30006278 <BOOT_LoadImages+0x528>
30005e78:	2a00      	cmp	r2, #0
30005e7a:	f000 820d 	beq.w	30006298 <BOOT_LoadImages+0x548>
30005e7e:	455b      	cmp	r3, fp
30005e80:	d301      	bcc.n	30005e86 <BOOT_LoadImages+0x136>
30005e82:	45e3      	cmp	fp, ip
30005e84:	d845      	bhi.n	30005f12 <BOOT_LoadImages+0x1c2>
30005e86:	4813      	ldr	r0, [pc, #76]	; (30005ed4 <BOOT_LoadImages+0x184>)
30005e88:	f006 fc42 	bl	3000c710 <__DiagPrintf_veneer>
30005e8c:	9f06      	ldr	r7, [sp, #24]
30005e8e:	4812      	ldr	r0, [pc, #72]	; (30005ed8 <BOOT_LoadImages+0x188>)
30005e90:	464d      	mov	r5, r9
30005e92:	f006 fc3d 	bl	3000c710 <__DiagPrintf_veneer>
30005e96:	9b07      	ldr	r3, [sp, #28]
30005e98:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005e9c:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005ea0:	e049      	b.n	30005f36 <BOOT_LoadImages+0x1e6>
30005ea2:	480e      	ldr	r0, [pc, #56]	; (30005edc <BOOT_LoadImages+0x18c>)
30005ea4:	9307      	str	r3, [sp, #28]
30005ea6:	f006 fc33 	bl	3000c710 <__DiagPrintf_veneer>
30005eaa:	9b07      	ldr	r3, [sp, #28]
30005eac:	3303      	adds	r3, #3
30005eae:	d99a      	bls.n	30005de6 <BOOT_LoadImages+0x96>
30005eb0:	480b      	ldr	r0, [pc, #44]	; (30005ee0 <BOOT_LoadImages+0x190>)
30005eb2:	e149      	b.n	30006148 <BOOT_LoadImages+0x3f8>
30005eb4:	2001c01c 	.word	0x2001c01c
30005eb8:	00009db5 	.word	0x00009db5
30005ebc:	07ffe000 	.word	0x07ffe000
30005ec0:	07fff000 	.word	0x07fff000
30005ec4:	3000ee80 	.word	0x3000ee80
30005ec8:	3000cf20 	.word	0x3000cf20
30005ecc:	3000d308 	.word	0x3000d308
30005ed0:	3000d024 	.word	0x3000d024
30005ed4:	3000d2ec 	.word	0x3000d2ec
30005ed8:	3000d0b0 	.word	0x3000d0b0
30005edc:	3000cf0c 	.word	0x3000cf0c
30005ee0:	3000d2b8 	.word	0x3000d2b8
30005ee4:	07fff004 	.word	0x07fff004
30005ee8:	48c4      	ldr	r0, [pc, #784]	; (300061fc <BOOT_LoadImages+0x4ac>)
30005eea:	930f      	str	r3, [sp, #60]	; 0x3c
30005eec:	9209      	str	r2, [sp, #36]	; 0x24
30005eee:	f006 fc0f 	bl	3000c710 <__DiagPrintf_veneer>
30005ef2:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005ef4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005ef6:	2a00      	cmp	r2, #0
30005ef8:	f000 8131 	beq.w	3000615e <BOOT_LoadImages+0x40e>
30005efc:	455b      	cmp	r3, fp
30005efe:	f0c0 82c5 	bcc.w	3000648c <BOOT_LoadImages+0x73c>
30005f02:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005f06:	bf2c      	ite	cs
30005f08:	2200      	movcs	r2, #0
30005f0a:	2201      	movcc	r2, #1
30005f0c:	2a00      	cmp	r2, #0
30005f0e:	f040 82bd 	bne.w	3000648c <BOOT_LoadImages+0x73c>
30005f12:	4598      	cmp	r8, r3
30005f14:	f200 81b8 	bhi.w	30006288 <BOOT_LoadImages+0x538>
30005f18:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30005f1c:	f0c0 81b4 	bcc.w	30006288 <BOOT_LoadImages+0x538>
30005f20:	7b2f      	ldrb	r7, [r5, #12]
30005f22:	2a00      	cmp	r2, #0
30005f24:	f040 81de 	bne.w	300062e4 <BOOT_LoadImages+0x594>
30005f28:	48b5      	ldr	r0, [pc, #724]	; (30006200 <BOOT_LoadImages+0x4b0>)
30005f2a:	f006 fbf1 	bl	3000c710 <__DiagPrintf_veneer>
30005f2e:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005f32:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30005f36:	4639      	mov	r1, r7
30005f38:	48b2      	ldr	r0, [pc, #712]	; (30006204 <BOOT_LoadImages+0x4b4>)
30005f3a:	f006 fbe9 	bl	3000c710 <__DiagPrintf_veneer>
30005f3e:	4bb2      	ldr	r3, [pc, #712]	; (30006208 <BOOT_LoadImages+0x4b8>)
30005f40:	4798      	blx	r3
30005f42:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005f46:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005f4a:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005f4e:	6809      	ldr	r1, [r1, #0]
30005f50:	330c      	adds	r3, #12
30005f52:	910b      	str	r1, [sp, #44]	; 0x2c
30005f54:	42b3      	cmp	r3, r6
30005f56:	bf28      	it	cs
30005f58:	4633      	movcs	r3, r6
30005f5a:	930d      	str	r3, [sp, #52]	; 0x34
30005f5c:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005f60:	6812      	ldr	r2, [r2, #0]
30005f62:	330c      	adds	r3, #12
30005f64:	920a      	str	r2, [sp, #40]	; 0x28
30005f66:	429e      	cmp	r6, r3
30005f68:	bf28      	it	cs
30005f6a:	461e      	movcs	r6, r3
30005f6c:	960c      	str	r6, [sp, #48]	; 0x30
30005f6e:	2800      	cmp	r0, #0
30005f70:	f040 8194 	bne.w	3000629c <BOOT_LoadImages+0x54c>
30005f74:	4ba5      	ldr	r3, [pc, #660]	; (3000620c <BOOT_LoadImages+0x4bc>)
30005f76:	4606      	mov	r6, r0
30005f78:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30006238 <BOOT_LoadImages+0x4e8>
30005f7c:	4680      	mov	r8, r0
30005f7e:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 3000623c <BOOT_LoadImages+0x4ec>
30005f82:	4682      	mov	sl, r0
30005f84:	e9cd 0303 	strd	r0, r3, [sp, #12]
30005f88:	2f00      	cmp	r7, #0
30005f8a:	f040 81c9 	bne.w	30006320 <BOOT_LoadImages+0x5d0>
30005f8e:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005f92:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005f94:	f10c 0104 	add.w	r1, ip, #4
30005f98:	2b00      	cmp	r3, #0
30005f9a:	f000 83d4 	beq.w	30006746 <BOOT_LoadImages+0x9f6>
30005f9e:	1d18      	adds	r0, r3, #4
30005fa0:	f04f 32ff 	mov.w	r2, #4294967295
30005fa4:	4460      	add	r0, ip
30005fa6:	f811 3b01 	ldrb.w	r3, [r1], #1
30005faa:	4053      	eors	r3, r2
30005fac:	4281      	cmp	r1, r0
30005fae:	b2db      	uxtb	r3, r3
30005fb0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005fb4:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005fb8:	d1f5      	bne.n	30005fa6 <BOOT_LoadImages+0x256>
30005fba:	43d2      	mvns	r2, r2
30005fbc:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005fbe:	4293      	cmp	r3, r2
30005fc0:	f000 838d 	beq.w	300066de <BOOT_LoadImages+0x98e>
30005fc4:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005fc8:	f088 0301 	eor.w	r3, r8, #1
30005fcc:	3101      	adds	r1, #1
30005fce:	f003 0301 	and.w	r3, r3, #1
30005fd2:	ea46 0603 	orr.w	r6, r6, r3
30005fd6:	f000 8345 	beq.w	30006664 <BOOT_LoadImages+0x914>
30005fda:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005fdc:	488c      	ldr	r0, [pc, #560]	; (30006210 <BOOT_LoadImages+0x4c0>)
30005fde:	f006 fb97 	bl	3000c710 <__DiagPrintf_veneer>
30005fe2:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005fe6:	2002      	movs	r0, #2
30005fe8:	f7ff f85a 	bl	300050a0 <FLASH_Erase_With_Lock>
30005fec:	2e00      	cmp	r6, #0
30005fee:	f040 831f 	bne.w	30006630 <BOOT_LoadImages+0x8e0>
30005ff2:	9605      	str	r6, [sp, #20]
30005ff4:	f08a 0601 	eor.w	r6, sl, #1
30005ff8:	f04f 0801 	mov.w	r8, #1
30005ffc:	f006 0601 	and.w	r6, r6, #1
30006000:	9b03      	ldr	r3, [sp, #12]
30006002:	2b01      	cmp	r3, #1
30006004:	f000 8238 	beq.w	30006478 <BOOT_LoadImages+0x728>
30006008:	9b03      	ldr	r3, [sp, #12]
3000600a:	2b02      	cmp	r3, #2
3000600c:	f000 83a9 	beq.w	30006762 <BOOT_LoadImages+0xa12>
30006010:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30006012:	4650      	mov	r0, sl
30006014:	f8c9 3000 	str.w	r3, [r9]
30006018:	47d8      	blx	fp
3000601a:	1e03      	subs	r3, r0, #0
3000601c:	bf18      	it	ne
3000601e:	2301      	movne	r3, #1
30006020:	2800      	cmp	r0, #0
30006022:	f000 8156 	beq.w	300062d2 <BOOT_LoadImages+0x582>
30006026:	2e00      	cmp	r6, #0
30006028:	f000 8153 	beq.w	300062d2 <BOOT_LoadImages+0x582>
3000602c:	fab7 f787 	clz	r7, r7
30006030:	9b05      	ldr	r3, [sp, #20]
30006032:	097f      	lsrs	r7, r7, #5
30006034:	2b00      	cmp	r3, #0
30006036:	f000 83e1 	beq.w	300067fc <BOOT_LoadImages+0xaac>
3000603a:	469a      	mov	sl, r3
3000603c:	4c75      	ldr	r4, [pc, #468]	; (30006214 <BOOT_LoadImages+0x4c4>)
3000603e:	2f00      	cmp	r7, #0
30006040:	f040 8138 	bne.w	300062b4 <BOOT_LoadImages+0x564>
30006044:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006048:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000604c:	3201      	adds	r2, #1
3000604e:	f000 82c5 	beq.w	300065dc <BOOT_LoadImages+0x88c>
30006052:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006056:	4870      	ldr	r0, [pc, #448]	; (30006218 <BOOT_LoadImages+0x4c8>)
30006058:	f006 fb5a 	bl	3000c710 <__DiagPrintf_veneer>
3000605c:	2301      	movs	r3, #1
3000605e:	9303      	str	r3, [sp, #12]
30006060:	f04f 0801 	mov.w	r8, #1
30006064:	4646      	mov	r6, r8
30006066:	f8cd 8014 	str.w	r8, [sp, #20]
3000606a:	f1ba 0f00 	cmp.w	sl, #0
3000606e:	d0c7      	beq.n	30006000 <BOOT_LoadImages+0x2b0>
30006070:	ab16      	add	r3, sp, #88	; 0x58
30006072:	2280      	movs	r2, #128	; 0x80
30006074:	21ff      	movs	r1, #255	; 0xff
30006076:	46d0      	mov	r8, sl
30006078:	461e      	mov	r6, r3
3000607a:	4618      	mov	r0, r3
3000607c:	ee08 3a10 	vmov	s16, r3
30006080:	f006 fb16 	bl	3000c6b0 <____wrap_memset_veneer>
30006084:	2280      	movs	r2, #128	; 0x80
30006086:	4629      	mov	r1, r5
30006088:	4630      	mov	r0, r6
3000608a:	f006 fb29 	bl	3000c6e0 <____wrap_memcpy_veneer>
3000608e:	2f01      	cmp	r7, #1
30006090:	f000 831e 	beq.w	300066d0 <BOOT_LoadImages+0x980>
30006094:	2100      	movs	r1, #0
30006096:	f8cd a014 	str.w	sl, [sp, #20]
3000609a:	f04f 0a01 	mov.w	sl, #1
3000609e:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300060a2:	1d2e      	adds	r6, r5, #4
300060a4:	485d      	ldr	r0, [pc, #372]	; (3000621c <BOOT_LoadImages+0x4cc>)
300060a6:	f006 fb33 	bl	3000c710 <__DiagPrintf_veneer>
300060aa:	4629      	mov	r1, r5
300060ac:	2002      	movs	r0, #2
300060ae:	f7fe fff7 	bl	300050a0 <FLASH_Erase_With_Lock>
300060b2:	aa17      	add	r2, sp, #92	; 0x5c
300060b4:	217c      	movs	r1, #124	; 0x7c
300060b6:	4630      	mov	r0, r6
300060b8:	f7ff f81a 	bl	300050f0 <FLASH_TxData_With_Lock>
300060bc:	4630      	mov	r0, r6
300060be:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
300060c2:	f04f 32ff 	mov.w	r2, #4294967295
300060c6:	f810 3b01 	ldrb.w	r3, [r0], #1
300060ca:	4053      	eors	r3, r2
300060cc:	4288      	cmp	r0, r1
300060ce:	b2db      	uxtb	r3, r3
300060d0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300060d4:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300060d8:	d1f5      	bne.n	300060c6 <BOOT_LoadImages+0x376>
300060da:	43d2      	mvns	r2, r2
300060dc:	4629      	mov	r1, r5
300060de:	2002      	movs	r0, #2
300060e0:	9216      	str	r2, [sp, #88]	; 0x58
300060e2:	f7fe ffdd 	bl	300050a0 <FLASH_Erase_With_Lock>
300060e6:	ee18 2a10 	vmov	r2, s16
300060ea:	2180      	movs	r1, #128	; 0x80
300060ec:	4628      	mov	r0, r5
300060ee:	f7fe ffff 	bl	300050f0 <FLASH_TxData_With_Lock>
300060f2:	9b05      	ldr	r3, [sp, #20]
300060f4:	2b00      	cmp	r3, #0
300060f6:	f000 8143 	beq.w	30006380 <BOOT_LoadImages+0x630>
300060fa:	f1b8 0f00 	cmp.w	r8, #0
300060fe:	f040 8376 	bne.w	300067ee <BOOT_LoadImages+0xa9e>
30006102:	2f01      	cmp	r7, #1
30006104:	f000 8280 	beq.w	30006608 <BOOT_LoadImages+0x8b8>
30006108:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000610c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006110:	3201      	adds	r2, #1
30006112:	f000 81d0 	beq.w	300064b6 <BOOT_LoadImages+0x766>
30006116:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000611a:	4841      	ldr	r0, [pc, #260]	; (30006220 <BOOT_LoadImages+0x4d0>)
3000611c:	f006 faf8 	bl	3000c710 <__DiagPrintf_veneer>
30006120:	f08a 0601 	eor.w	r6, sl, #1
30006124:	f04f 0801 	mov.w	r8, #1
30006128:	f006 0601 	and.w	r6, r6, #1
3000612c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006130:	3310      	adds	r3, #16
30006132:	930e      	str	r3, [sp, #56]	; 0x38
30006134:	2302      	movs	r3, #2
30006136:	9303      	str	r3, [sp, #12]
30006138:	e76a      	b.n	30006010 <BOOT_LoadImages+0x2c0>
3000613a:	483a      	ldr	r0, [pc, #232]	; (30006224 <BOOT_LoadImages+0x4d4>)
3000613c:	f006 fae8 	bl	3000c710 <__DiagPrintf_veneer>
30006140:	9b04      	ldr	r3, [sp, #16]
30006142:	3b01      	subs	r3, #1
30006144:	e64d      	b.n	30005de2 <BOOT_LoadImages+0x92>
30006146:	4838      	ldr	r0, [pc, #224]	; (30006228 <BOOT_LoadImages+0x4d8>)
30006148:	f006 fae2 	bl	3000c710 <__DiagPrintf_veneer>
3000614c:	f8d9 3011 	ldr.w	r3, [r9, #17]
30006150:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006154:	9307      	str	r3, [sp, #28]
30006156:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000615a:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000615e:	4833      	ldr	r0, [pc, #204]	; (3000622c <BOOT_LoadImages+0x4dc>)
30006160:	f006 fad6 	bl	3000c710 <__DiagPrintf_veneer>
30006164:	4832      	ldr	r0, [pc, #200]	; (30006230 <BOOT_LoadImages+0x4e0>)
30006166:	f006 fad3 	bl	3000c710 <__DiagPrintf_veneer>
3000616a:	4631      	mov	r1, r6
3000616c:	aa12      	add	r2, sp, #72	; 0x48
3000616e:	4650      	mov	r0, sl
30006170:	f7ff f85e 	bl	30005230 <recovery_check>
30006174:	4631      	mov	r1, r6
30006176:	4604      	mov	r4, r0
30006178:	aa13      	add	r2, sp, #76	; 0x4c
3000617a:	9807      	ldr	r0, [sp, #28]
3000617c:	f7ff f858 	bl	30005230 <recovery_check>
30006180:	4631      	mov	r1, r6
30006182:	aa14      	add	r2, sp, #80	; 0x50
30006184:	9004      	str	r0, [sp, #16]
30006186:	4658      	mov	r0, fp
30006188:	f7ff f852 	bl	30005230 <recovery_check>
3000618c:	4631      	mov	r1, r6
3000618e:	aa15      	add	r2, sp, #84	; 0x54
30006190:	4606      	mov	r6, r0
30006192:	4640      	mov	r0, r8
30006194:	f7ff f84c 	bl	30005230 <recovery_check>
30006198:	9b04      	ldr	r3, [sp, #16]
3000619a:	4306      	orrs	r6, r0
3000619c:	431c      	orrs	r4, r3
3000619e:	d04f      	beq.n	30006240 <BOOT_LoadImages+0x4f0>
300061a0:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
300061a4:	2e00      	cmp	r6, #0
300061a6:	f000 818a 	beq.w	300064be <BOOT_LoadImages+0x76e>
300061aa:	4293      	cmp	r3, r2
300061ac:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
300061b0:	f0c0 8273 	bcc.w	3000669a <BOOT_LoadImages+0x94a>
300061b4:	428b      	cmp	r3, r1
300061b6:	d302      	bcc.n	300061be <BOOT_LoadImages+0x46e>
300061b8:	4283      	cmp	r3, r0
300061ba:	f080 82fb 	bcs.w	300067b4 <BOOT_LoadImages+0xa64>
300061be:	4293      	cmp	r3, r2
300061c0:	f000 826b 	beq.w	3000669a <BOOT_LoadImages+0x94a>
300061c4:	4293      	cmp	r3, r2
300061c6:	bf38      	it	cc
300061c8:	4613      	movcc	r3, r2
300061ca:	428b      	cmp	r3, r1
300061cc:	f240 82dc 	bls.w	30006788 <BOOT_LoadImages+0xa38>
300061d0:	428b      	cmp	r3, r1
300061d2:	bf38      	it	cc
300061d4:	460b      	movcc	r3, r1
300061d6:	4283      	cmp	r3, r0
300061d8:	f240 82e5 	bls.w	300067a6 <BOOT_LoadImages+0xa56>
300061dc:	f04f 0800 	mov.w	r8, #0
300061e0:	4b0a      	ldr	r3, [pc, #40]	; (3000620c <BOOT_LoadImages+0x4bc>)
300061e2:	2601      	movs	r6, #1
300061e4:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30006238 <BOOT_LoadImages+0x4e8>
300061e8:	f8df b050 	ldr.w	fp, [pc, #80]	; 3000623c <BOOT_LoadImages+0x4ec>
300061ec:	46c2      	mov	sl, r8
300061ee:	9304      	str	r3, [sp, #16]
300061f0:	e70e      	b.n	30006010 <BOOT_LoadImages+0x2c0>
300061f2:	4810      	ldr	r0, [pc, #64]	; (30006234 <BOOT_LoadImages+0x4e4>)
300061f4:	f006 fa8c 	bl	3000c710 <__DiagPrintf_veneer>
300061f8:	e5c5      	b.n	30005d86 <BOOT_LoadImages+0x36>
300061fa:	bf00      	nop
300061fc:	3000d064 	.word	0x3000d064
30006200:	3000d0c0 	.word	0x3000d0c0
30006204:	3000d0f4 	.word	0x3000d0f4
30006208:	0000c149 	.word	0x0000c149
3000620c:	3000ebda 	.word	0x3000ebda
30006210:	3000d164 	.word	0x3000d164
30006214:	3000ee80 	.word	0x3000ee80
30006218:	3000ce34 	.word	0x3000ce34
3000621c:	3000d238 	.word	0x3000d238
30006220:	3000ce4c 	.word	0x3000ce4c
30006224:	3000cef4 	.word	0x3000cef4
30006228:	3000cf58 	.word	0x3000cf58
3000622c:	3000d0d0 	.word	0x3000d0d0
30006230:	3000d0e4 	.word	0x3000d0e4
30006234:	3000ced0 	.word	0x3000ced0
30006238:	30010590 	.word	0x30010590
3000623c:	300043c1 	.word	0x300043c1
30006240:	2e00      	cmp	r6, #0
30006242:	f000 827c 	beq.w	3000673e <BOOT_LoadImages+0x9ee>
30006246:	4659      	mov	r1, fp
30006248:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
3000624c:	e9cd 5400 	strd	r5, r4, [sp]
30006250:	429a      	cmp	r2, r3
30006252:	464b      	mov	r3, r9
30006254:	4642      	mov	r2, r8
30006256:	f080 821e 	bcs.w	30006696 <BOOT_LoadImages+0x946>
3000625a:	2001      	movs	r0, #1
3000625c:	f7ff f818 	bl	30005290 <rewrite_bp>
30006260:	46a0      	mov	r8, r4
30006262:	b2c3      	uxtb	r3, r0
30006264:	2601      	movs	r6, #1
30006266:	46a2      	mov	sl, r4
30006268:	f8df 9318 	ldr.w	r9, [pc, #792]	; 30006584 <BOOT_LoadImages+0x834>
3000626c:	9303      	str	r3, [sp, #12]
3000626e:	4bb0      	ldr	r3, [pc, #704]	; (30006530 <BOOT_LoadImages+0x7e0>)
30006270:	f8df b314 	ldr.w	fp, [pc, #788]	; 30006588 <BOOT_LoadImages+0x838>
30006274:	9304      	str	r3, [sp, #16]
30006276:	e6c3      	b.n	30006000 <BOOT_LoadImages+0x2b0>
30006278:	48ae      	ldr	r0, [pc, #696]	; (30006534 <BOOT_LoadImages+0x7e4>)
3000627a:	930f      	str	r3, [sp, #60]	; 0x3c
3000627c:	9209      	str	r2, [sp, #36]	; 0x24
3000627e:	f006 fa47 	bl	3000c710 <__DiagPrintf_veneer>
30006282:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006284:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30006286:	e636      	b.n	30005ef6 <BOOT_LoadImages+0x1a6>
30006288:	48ab      	ldr	r0, [pc, #684]	; (30006538 <BOOT_LoadImages+0x7e8>)
3000628a:	9204      	str	r2, [sp, #16]
3000628c:	f006 fa40 	bl	3000c710 <__DiagPrintf_veneer>
30006290:	9a04      	ldr	r2, [sp, #16]
30006292:	2a00      	cmp	r2, #0
30006294:	f43f af63 	beq.w	3000615e <BOOT_LoadImages+0x40e>
30006298:	9f06      	ldr	r7, [sp, #24]
3000629a:	e5f8      	b.n	30005e8e <BOOT_LoadImages+0x13e>
3000629c:	2300      	movs	r3, #0
3000629e:	4aa4      	ldr	r2, [pc, #656]	; (30006530 <BOOT_LoadImages+0x7e0>)
300062a0:	f8df 92e0 	ldr.w	r9, [pc, #736]	; 30006584 <BOOT_LoadImages+0x834>
300062a4:	f8df b2e0 	ldr.w	fp, [pc, #736]	; 30006588 <BOOT_LoadImages+0x838>
300062a8:	469a      	mov	sl, r3
300062aa:	9303      	str	r3, [sp, #12]
300062ac:	9204      	str	r2, [sp, #16]
300062ae:	2f00      	cmp	r7, #0
300062b0:	f43f aec8 	beq.w	30006044 <BOOT_LoadImages+0x2f4>
300062b4:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300062b8:	f8d3 200a 	ldr.w	r2, [r3, #10]
300062bc:	3201      	adds	r2, #1
300062be:	f000 8118 	beq.w	300064f2 <BOOT_LoadImages+0x7a2>
300062c2:	f8d3 1006 	ldr.w	r1, [r3, #6]
300062c6:	489d      	ldr	r0, [pc, #628]	; (3000653c <BOOT_LoadImages+0x7ec>)
300062c8:	f006 fa22 	bl	3000c710 <__DiagPrintf_veneer>
300062cc:	2302      	movs	r3, #2
300062ce:	9303      	str	r3, [sp, #12]
300062d0:	e6c6      	b.n	30006060 <BOOT_LoadImages+0x310>
300062d2:	f00a 0401 	and.w	r4, sl, #1
300062d6:	b1c3      	cbz	r3, 3000630a <BOOT_LoadImages+0x5ba>
300062d8:	b1bc      	cbz	r4, 3000630a <BOOT_LoadImages+0x5ba>
300062da:	4c99      	ldr	r4, [pc, #612]	; (30006540 <BOOT_LoadImages+0x7f0>)
300062dc:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300062e0:	47a0      	blx	r4
300062e2:	e7fb      	b.n	300062dc <BOOT_LoadImages+0x58c>
300062e4:	4897      	ldr	r0, [pc, #604]	; (30006544 <BOOT_LoadImages+0x7f4>)
300062e6:	f006 fa13 	bl	3000c710 <__DiagPrintf_veneer>
300062ea:	9b08      	ldr	r3, [sp, #32]
300062ec:	9a04      	ldr	r2, [sp, #16]
300062ee:	4293      	cmp	r3, r2
300062f0:	f4ff ae1a 	bcc.w	30005f28 <BOOT_LoadImages+0x1d8>
300062f4:	4894      	ldr	r0, [pc, #592]	; (30006548 <BOOT_LoadImages+0x7f8>)
300062f6:	464d      	mov	r5, r9
300062f8:	f006 fa0a 	bl	3000c710 <__DiagPrintf_veneer>
300062fc:	9b07      	ldr	r3, [sp, #28]
300062fe:	9f06      	ldr	r7, [sp, #24]
30006300:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30006304:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30006308:	e615      	b.n	30005f36 <BOOT_LoadImages+0x1e6>
3000630a:	9b04      	ldr	r3, [sp, #16]
3000630c:	781b      	ldrb	r3, [r3, #0]
3000630e:	2b00      	cmp	r3, #0
30006310:	f040 80c0 	bne.w	30006494 <BOOT_LoadImages+0x744>
30006314:	2001      	movs	r0, #1
30006316:	b037      	add	sp, #220	; 0xdc
30006318:	ecbd 8b02 	vpop	{d8}
3000631c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006320:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30006324:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006326:	f10c 0104 	add.w	r1, ip, #4
3000632a:	2b00      	cmp	r3, #0
3000632c:	f000 820d 	beq.w	3000674a <BOOT_LoadImages+0x9fa>
30006330:	1d18      	adds	r0, r3, #4
30006332:	f04f 32ff 	mov.w	r2, #4294967295
30006336:	4460      	add	r0, ip
30006338:	f811 3b01 	ldrb.w	r3, [r1], #1
3000633c:	4053      	eors	r3, r2
3000633e:	4281      	cmp	r1, r0
30006340:	b2db      	uxtb	r3, r3
30006342:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006346:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000634a:	d1f5      	bne.n	30006338 <BOOT_LoadImages+0x5e8>
3000634c:	43d2      	mvns	r2, r2
3000634e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006350:	4293      	cmp	r3, r2
30006352:	f000 81d4 	beq.w	300066fe <BOOT_LoadImages+0x9ae>
30006356:	f8dc 100a 	ldr.w	r1, [ip, #10]
3000635a:	f088 0301 	eor.w	r3, r8, #1
3000635e:	3101      	adds	r1, #1
30006360:	f003 0301 	and.w	r3, r3, #1
30006364:	ea46 0603 	orr.w	r6, r6, r3
30006368:	f000 8178 	beq.w	3000665c <BOOT_LoadImages+0x90c>
3000636c:	990a      	ldr	r1, [sp, #40]	; 0x28
3000636e:	4877      	ldr	r0, [pc, #476]	; (3000654c <BOOT_LoadImages+0x7fc>)
30006370:	f006 f9ce 	bl	3000c710 <__DiagPrintf_veneer>
30006374:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006378:	2002      	movs	r0, #2
3000637a:	f7fe fe91 	bl	300050a0 <FLASH_Erase_With_Lock>
3000637e:	e635      	b.n	30005fec <BOOT_LoadImages+0x29c>
30006380:	f1b8 0f00 	cmp.w	r8, #0
30006384:	f040 822e 	bne.w	300067e4 <BOOT_LoadImages+0xa94>
30006388:	2f01      	cmp	r7, #1
3000638a:	f000 80ff 	beq.w	3000658c <BOOT_LoadImages+0x83c>
3000638e:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30006392:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006394:	1d31      	adds	r1, r6, #4
30006396:	2b00      	cmp	r3, #0
30006398:	f000 81e5 	beq.w	30006766 <BOOT_LoadImages+0xa16>
3000639c:	1d18      	adds	r0, r3, #4
3000639e:	f04f 32ff 	mov.w	r2, #4294967295
300063a2:	4430      	add	r0, r6
300063a4:	f811 3b01 	ldrb.w	r3, [r1], #1
300063a8:	4053      	eors	r3, r2
300063aa:	4281      	cmp	r1, r0
300063ac:	b2db      	uxtb	r3, r3
300063ae:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300063b2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300063b6:	d1f5      	bne.n	300063a4 <BOOT_LoadImages+0x654>
300063b8:	43d2      	mvns	r2, r2
300063ba:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300063bc:	4293      	cmp	r3, r2
300063be:	f000 81af 	beq.w	30006720 <BOOT_LoadImages+0x9d0>
300063c2:	f8d6 300a 	ldr.w	r3, [r6, #10]
300063c6:	3301      	adds	r3, #1
300063c8:	d075      	beq.n	300064b6 <BOOT_LoadImages+0x766>
300063ca:	990a      	ldr	r1, [sp, #40]	; 0x28
300063cc:	485f      	ldr	r0, [pc, #380]	; (3000654c <BOOT_LoadImages+0x7fc>)
300063ce:	f006 f99f 	bl	3000c710 <__DiagPrintf_veneer>
300063d2:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300063d6:	2002      	movs	r0, #2
300063d8:	f7fe fe62 	bl	300050a0 <FLASH_Erase_With_Lock>
300063dc:	485c      	ldr	r0, [pc, #368]	; (30006550 <BOOT_LoadImages+0x800>)
300063de:	f006 f997 	bl	3000c710 <__DiagPrintf_veneer>
300063e2:	e7fe      	b.n	300063e2 <BOOT_LoadImages+0x692>
300063e4:	9908      	ldr	r1, [sp, #32]
300063e6:	485b      	ldr	r0, [pc, #364]	; (30006554 <BOOT_LoadImages+0x804>)
300063e8:	f006 f992 	bl	3000c710 <__DiagPrintf_veneer>
300063ec:	9b07      	ldr	r3, [sp, #28]
300063ee:	3303      	adds	r3, #3
300063f0:	f240 81ad 	bls.w	3000674e <BOOT_LoadImages+0x9fe>
300063f4:	4858      	ldr	r0, [pc, #352]	; (30006558 <BOOT_LoadImages+0x808>)
300063f6:	f006 f98b 	bl	3000c710 <__DiagPrintf_veneer>
300063fa:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
300063fe:	f8d9 3011 	ldr.w	r3, [r9, #17]
30006402:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006406:	9307      	str	r3, [sp, #28]
30006408:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000640c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30006410:	4852      	ldr	r0, [pc, #328]	; (3000655c <BOOT_LoadImages+0x80c>)
30006412:	f04f 32ff 	mov.w	r2, #4294967295
30006416:	5931      	ldr	r1, [r6, r4]
30006418:	4430      	add	r0, r6
3000641a:	4c51      	ldr	r4, [pc, #324]	; (30006560 <BOOT_LoadImages+0x810>)
3000641c:	f810 3b01 	ldrb.w	r3, [r0], #1
30006420:	4053      	eors	r3, r2
30006422:	4285      	cmp	r5, r0
30006424:	b2db      	uxtb	r3, r3
30006426:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000642a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000642e:	d1f5      	bne.n	3000641c <BOOT_LoadImages+0x6cc>
30006430:	43d2      	mvns	r2, r2
30006432:	4291      	cmp	r1, r2
30006434:	d010      	beq.n	30006458 <BOOT_LoadImages+0x708>
30006436:	484b      	ldr	r0, [pc, #300]	; (30006564 <BOOT_LoadImages+0x814>)
30006438:	f006 f96a 	bl	3000c710 <__DiagPrintf_veneer>
3000643c:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000643e:	2b00      	cmp	r3, #0
30006440:	f43f ae8d 	beq.w	3000615e <BOOT_LoadImages+0x40e>
30006444:	2300      	movs	r3, #0
30006446:	9309      	str	r3, [sp, #36]	; 0x24
30006448:	e4e7      	b.n	30005e1a <BOOT_LoadImages+0xca>
3000644a:	460a      	mov	r2, r1
3000644c:	4846      	ldr	r0, [pc, #280]	; (30006568 <BOOT_LoadImages+0x818>)
3000644e:	f006 f95f 	bl	3000c710 <__DiagPrintf_veneer>
30006452:	2201      	movs	r2, #1
30006454:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30006456:	e4fc      	b.n	30005e52 <BOOT_LoadImages+0x102>
30006458:	460a      	mov	r2, r1
3000645a:	4844      	ldr	r0, [pc, #272]	; (3000656c <BOOT_LoadImages+0x81c>)
3000645c:	f006 f958 	bl	3000c710 <__DiagPrintf_veneer>
30006460:	9b09      	ldr	r3, [sp, #36]	; 0x24
30006462:	2b00      	cmp	r3, #0
30006464:	f47f acd9 	bne.w	30005e1a <BOOT_LoadImages+0xca>
30006468:	461a      	mov	r2, r3
3000646a:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
3000646e:	e4f3      	b.n	30005e58 <BOOT_LoadImages+0x108>
30006470:	483f      	ldr	r0, [pc, #252]	; (30006570 <BOOT_LoadImages+0x820>)
30006472:	f006 f94d 	bl	3000c710 <__DiagPrintf_veneer>
30006476:	e486      	b.n	30005d86 <BOOT_LoadImages+0x36>
30006478:	4c39      	ldr	r4, [pc, #228]	; (30006560 <BOOT_LoadImages+0x810>)
3000647a:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000647e:	3310      	adds	r3, #16
30006480:	930e      	str	r3, [sp, #56]	; 0x38
30006482:	2301      	movs	r3, #1
30006484:	9303      	str	r3, [sp, #12]
30006486:	e5c3      	b.n	30006010 <BOOT_LoadImages+0x2c0>
30006488:	483a      	ldr	r0, [pc, #232]	; (30006574 <BOOT_LoadImages+0x824>)
3000648a:	e65d      	b.n	30006148 <BOOT_LoadImages+0x3f8>
3000648c:	483a      	ldr	r0, [pc, #232]	; (30006578 <BOOT_LoadImages+0x828>)
3000648e:	f006 f93f 	bl	3000c710 <__DiagPrintf_veneer>
30006492:	e664      	b.n	3000615e <BOOT_LoadImages+0x40e>
30006494:	4b39      	ldr	r3, [pc, #228]	; (3000657c <BOOT_LoadImages+0x82c>)
30006496:	4650      	mov	r0, sl
30006498:	4798      	blx	r3
3000649a:	1e03      	subs	r3, r0, #0
3000649c:	bf18      	it	ne
3000649e:	2301      	movne	r3, #1
300064a0:	b110      	cbz	r0, 300064a8 <BOOT_LoadImages+0x758>
300064a2:	2e00      	cmp	r6, #0
300064a4:	f47f adc2 	bne.w	3000602c <BOOT_LoadImages+0x2dc>
300064a8:	2c00      	cmp	r4, #0
300064aa:	f43f af33 	beq.w	30006314 <BOOT_LoadImages+0x5c4>
300064ae:	2b00      	cmp	r3, #0
300064b0:	f47f af13 	bne.w	300062da <BOOT_LoadImages+0x58a>
300064b4:	e72e      	b.n	30006314 <BOOT_LoadImages+0x5c4>
300064b6:	4832      	ldr	r0, [pc, #200]	; (30006580 <BOOT_LoadImages+0x830>)
300064b8:	f006 f92a 	bl	3000c710 <__DiagPrintf_veneer>
300064bc:	e78e      	b.n	300063dc <BOOT_LoadImages+0x68c>
300064be:	4293      	cmp	r3, r2
300064c0:	f04f 0401 	mov.w	r4, #1
300064c4:	464b      	mov	r3, r9
300064c6:	4651      	mov	r1, sl
300064c8:	bf28      	it	cs
300064ca:	4630      	movcs	r0, r6
300064cc:	9a07      	ldr	r2, [sp, #28]
300064ce:	bf38      	it	cc
300064d0:	4620      	movcc	r0, r4
300064d2:	9500      	str	r5, [sp, #0]
300064d4:	9401      	str	r4, [sp, #4]
300064d6:	46b0      	mov	r8, r6
300064d8:	f7fe feda 	bl	30005290 <rewrite_bp>
300064dc:	4626      	mov	r6, r4
300064de:	b2c3      	uxtb	r3, r0
300064e0:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30006584 <BOOT_LoadImages+0x834>
300064e4:	46c2      	mov	sl, r8
300064e6:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 30006588 <BOOT_LoadImages+0x838>
300064ea:	9303      	str	r3, [sp, #12]
300064ec:	4b10      	ldr	r3, [pc, #64]	; (30006530 <BOOT_LoadImages+0x7e0>)
300064ee:	9304      	str	r3, [sp, #16]
300064f0:	e586      	b.n	30006000 <BOOT_LoadImages+0x2b0>
300064f2:	ab16      	add	r3, sp, #88	; 0x58
300064f4:	4822      	ldr	r0, [pc, #136]	; (30006580 <BOOT_LoadImages+0x830>)
300064f6:	461e      	mov	r6, r3
300064f8:	ee08 3a10 	vmov	s16, r3
300064fc:	f006 f908 	bl	3000c710 <__DiagPrintf_veneer>
30006500:	2280      	movs	r2, #128	; 0x80
30006502:	21ff      	movs	r1, #255	; 0xff
30006504:	4630      	mov	r0, r6
30006506:	f006 f8d3 	bl	3000c6b0 <____wrap_memset_veneer>
3000650a:	2280      	movs	r2, #128	; 0x80
3000650c:	4629      	mov	r1, r5
3000650e:	4630      	mov	r0, r6
30006510:	f006 f8e6 	bl	3000c6e0 <____wrap_memcpy_veneer>
30006514:	f1ba 0f00 	cmp.w	sl, #0
30006518:	f040 80a8 	bne.w	3000666c <BOOT_LoadImages+0x91c>
3000651c:	2301      	movs	r3, #1
3000651e:	9305      	str	r3, [sp, #20]
30006520:	46d0      	mov	r8, sl
30006522:	f04f 0a00 	mov.w	sl, #0
30006526:	4651      	mov	r1, sl
30006528:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
3000652c:	e5b9      	b.n	300060a2 <BOOT_LoadImages+0x352>
3000652e:	bf00      	nop
30006530:	3000ebda 	.word	0x3000ebda
30006534:	3000d080 	.word	0x3000d080
30006538:	3000d29c 	.word	0x3000d29c
3000653c:	3000ce4c 	.word	0x3000ce4c
30006540:	00009be5 	.word	0x00009be5
30006544:	3000d09c 	.word	0x3000d09c
30006548:	3000d0b0 	.word	0x3000d0b0
3000654c:	3000d1ec 	.word	0x3000d1ec
30006550:	3000ce64 	.word	0x3000ce64
30006554:	3000cf3c 	.word	0x3000cf3c
30006558:	3000d2b8 	.word	0x3000d2b8
3000655c:	07ffe004 	.word	0x07ffe004
30006560:	3000ee80 	.word	0x3000ee80
30006564:	3000cfa8 	.word	0x3000cfa8
30006568:	3000cfe8 	.word	0x3000cfe8
3000656c:	3000cf6c 	.word	0x3000cf6c
30006570:	3000cee0 	.word	0x3000cee0
30006574:	3000d2d0 	.word	0x3000d2d0
30006578:	3000d2ec 	.word	0x3000d2ec
3000657c:	300043c5 	.word	0x300043c5
30006580:	3000d118 	.word	0x3000d118
30006584:	30010590 	.word	0x30010590
30006588:	300043c1 	.word	0x300043c1
3000658c:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
30006590:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30006592:	1d31      	adds	r1, r6, #4
30006594:	2b00      	cmp	r3, #0
30006596:	f000 8102 	beq.w	3000679e <BOOT_LoadImages+0xa4e>
3000659a:	1d18      	adds	r0, r3, #4
3000659c:	f04f 32ff 	mov.w	r2, #4294967295
300065a0:	4430      	add	r0, r6
300065a2:	f811 3b01 	ldrb.w	r3, [r1], #1
300065a6:	4053      	eors	r3, r2
300065a8:	4281      	cmp	r1, r0
300065aa:	b2db      	uxtb	r3, r3
300065ac:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300065b0:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300065b4:	d1f5      	bne.n	300065a2 <BOOT_LoadImages+0x852>
300065b6:	43d2      	mvns	r2, r2
300065b8:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300065ba:	4293      	cmp	r3, r2
300065bc:	f000 80d5 	beq.w	3000676a <BOOT_LoadImages+0xa1a>
300065c0:	f8d6 300a 	ldr.w	r3, [r6, #10]
300065c4:	3301      	adds	r3, #1
300065c6:	d062      	beq.n	3000668e <BOOT_LoadImages+0x93e>
300065c8:	990b      	ldr	r1, [sp, #44]	; 0x2c
300065ca:	4890      	ldr	r0, [pc, #576]	; (3000680c <BOOT_LoadImages+0xabc>)
300065cc:	f006 f8a0 	bl	3000c710 <__DiagPrintf_veneer>
300065d0:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
300065d4:	2002      	movs	r0, #2
300065d6:	f7fe fd63 	bl	300050a0 <FLASH_Erase_With_Lock>
300065da:	e6ff      	b.n	300063dc <BOOT_LoadImages+0x68c>
300065dc:	ab16      	add	r3, sp, #88	; 0x58
300065de:	488c      	ldr	r0, [pc, #560]	; (30006810 <BOOT_LoadImages+0xac0>)
300065e0:	461e      	mov	r6, r3
300065e2:	ee08 3a10 	vmov	s16, r3
300065e6:	f006 f893 	bl	3000c710 <__DiagPrintf_veneer>
300065ea:	2280      	movs	r2, #128	; 0x80
300065ec:	21ff      	movs	r1, #255	; 0xff
300065ee:	4630      	mov	r0, r6
300065f0:	f006 f85e 	bl	3000c6b0 <____wrap_memset_veneer>
300065f4:	2280      	movs	r2, #128	; 0x80
300065f6:	4629      	mov	r1, r5
300065f8:	4630      	mov	r0, r6
300065fa:	f006 f871 	bl	3000c6e0 <____wrap_memcpy_veneer>
300065fe:	f1ba 0f00 	cmp.w	sl, #0
30006602:	d036      	beq.n	30006672 <BOOT_LoadImages+0x922>
30006604:	46b8      	mov	r8, r7
30006606:	e545      	b.n	30006094 <BOOT_LoadImages+0x344>
30006608:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000660c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006610:	3201      	adds	r2, #1
30006612:	d03c      	beq.n	3000668e <BOOT_LoadImages+0x93e>
30006614:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006618:	487e      	ldr	r0, [pc, #504]	; (30006814 <BOOT_LoadImages+0xac4>)
3000661a:	f006 f879 	bl	3000c710 <__DiagPrintf_veneer>
3000661e:	9705      	str	r7, [sp, #20]
30006620:	f08a 0601 	eor.w	r6, sl, #1
30006624:	f04f 0801 	mov.w	r8, #1
30006628:	f006 0601 	and.w	r6, r6, #1
3000662c:	4647      	mov	r7, r8
3000662e:	e724      	b.n	3000647a <BOOT_LoadImages+0x72a>
30006630:	ab16      	add	r3, sp, #88	; 0x58
30006632:	2280      	movs	r2, #128	; 0x80
30006634:	21ff      	movs	r1, #255	; 0xff
30006636:	461e      	mov	r6, r3
30006638:	4618      	mov	r0, r3
3000663a:	ee08 3a10 	vmov	s16, r3
3000663e:	f006 f837 	bl	3000c6b0 <____wrap_memset_veneer>
30006642:	2280      	movs	r2, #128	; 0x80
30006644:	4629      	mov	r1, r5
30006646:	4630      	mov	r0, r6
30006648:	f006 f84a 	bl	3000c6e0 <____wrap_memcpy_veneer>
3000664c:	f1ba 0f00 	cmp.w	sl, #0
30006650:	f040 80d9 	bne.w	30006806 <BOOT_LoadImages+0xab6>
30006654:	b9bf      	cbnz	r7, 30006686 <BOOT_LoadImages+0x936>
30006656:	46c2      	mov	sl, r8
30006658:	9705      	str	r7, [sp, #20]
3000665a:	e00c      	b.n	30006676 <BOOT_LoadImages+0x926>
3000665c:	486e      	ldr	r0, [pc, #440]	; (30006818 <BOOT_LoadImages+0xac8>)
3000665e:	f006 f857 	bl	3000c710 <__DiagPrintf_veneer>
30006662:	e4c3      	b.n	30005fec <BOOT_LoadImages+0x29c>
30006664:	486a      	ldr	r0, [pc, #424]	; (30006810 <BOOT_LoadImages+0xac0>)
30006666:	f006 f853 	bl	3000c710 <__DiagPrintf_veneer>
3000666a:	e4bf      	b.n	30005fec <BOOT_LoadImages+0x29c>
3000666c:	f04f 0800 	mov.w	r8, #0
30006670:	e50d      	b.n	3000608e <BOOT_LoadImages+0x33e>
30006672:	2301      	movs	r3, #1
30006674:	9305      	str	r3, [sp, #20]
30006676:	46d0      	mov	r8, sl
30006678:	2101      	movs	r1, #1
3000667a:	f04f 0a00 	mov.w	sl, #0
3000667e:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30006682:	4657      	mov	r7, sl
30006684:	e50d      	b.n	300060a2 <BOOT_LoadImages+0x352>
30006686:	f8cd a014 	str.w	sl, [sp, #20]
3000668a:	46c2      	mov	sl, r8
3000668c:	e748      	b.n	30006520 <BOOT_LoadImages+0x7d0>
3000668e:	4860      	ldr	r0, [pc, #384]	; (30006810 <BOOT_LoadImages+0xac0>)
30006690:	f006 f83e 	bl	3000c710 <__DiagPrintf_veneer>
30006694:	e6a2      	b.n	300063dc <BOOT_LoadImages+0x68c>
30006696:	4620      	mov	r0, r4
30006698:	e5e0      	b.n	3000625c <BOOT_LoadImages+0x50c>
3000669a:	428a      	cmp	r2, r1
3000669c:	f4ff ad92 	bcc.w	300061c4 <BOOT_LoadImages+0x474>
300066a0:	4282      	cmp	r2, r0
300066a2:	f4ff ad8f 	bcc.w	300061c4 <BOOT_LoadImages+0x474>
300066a6:	2601      	movs	r6, #1
300066a8:	464b      	mov	r3, r9
300066aa:	9a07      	ldr	r2, [sp, #28]
300066ac:	4651      	mov	r1, sl
300066ae:	4630      	mov	r0, r6
300066b0:	9500      	str	r5, [sp, #0]
300066b2:	9601      	str	r6, [sp, #4]
300066b4:	f7fe fdec 	bl	30005290 <rewrite_bp>
300066b8:	f04f 0800 	mov.w	r8, #0
300066bc:	b2c3      	uxtb	r3, r0
300066be:	f8df 9180 	ldr.w	r9, [pc, #384]	; 30006840 <BOOT_LoadImages+0xaf0>
300066c2:	f8df b180 	ldr.w	fp, [pc, #384]	; 30006844 <BOOT_LoadImages+0xaf4>
300066c6:	46c2      	mov	sl, r8
300066c8:	9303      	str	r3, [sp, #12]
300066ca:	4b54      	ldr	r3, [pc, #336]	; (3000681c <BOOT_LoadImages+0xacc>)
300066cc:	9304      	str	r3, [sp, #16]
300066ce:	e497      	b.n	30006000 <BOOT_LoadImages+0x2b0>
300066d0:	f8cd a014 	str.w	sl, [sp, #20]
300066d4:	4639      	mov	r1, r7
300066d6:	46ba      	mov	sl, r7
300066d8:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
300066dc:	e4e1      	b.n	300060a2 <BOOT_LoadImages+0x352>
300066de:	461a      	mov	r2, r3
300066e0:	4619      	mov	r1, r3
300066e2:	484f      	ldr	r0, [pc, #316]	; (30006820 <BOOT_LoadImages+0xad0>)
300066e4:	f006 f814 	bl	3000c710 <__DiagPrintf_veneer>
300066e8:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066ec:	4849      	ldr	r0, [pc, #292]	; (30006814 <BOOT_LoadImages+0xac4>)
300066ee:	f8d3 1006 	ldr.w	r1, [r3, #6]
300066f2:	f006 f80d 	bl	3000c710 <__DiagPrintf_veneer>
300066f6:	2301      	movs	r3, #1
300066f8:	4698      	mov	r8, r3
300066fa:	9303      	str	r3, [sp, #12]
300066fc:	e476      	b.n	30005fec <BOOT_LoadImages+0x29c>
300066fe:	461a      	mov	r2, r3
30006700:	4619      	mov	r1, r3
30006702:	4848      	ldr	r0, [pc, #288]	; (30006824 <BOOT_LoadImages+0xad4>)
30006704:	f04f 0801 	mov.w	r8, #1
30006708:	f006 f802 	bl	3000c710 <__DiagPrintf_veneer>
3000670c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006710:	4845      	ldr	r0, [pc, #276]	; (30006828 <BOOT_LoadImages+0xad8>)
30006712:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006716:	f005 fffb 	bl	3000c710 <__DiagPrintf_veneer>
3000671a:	2302      	movs	r3, #2
3000671c:	9303      	str	r3, [sp, #12]
3000671e:	e465      	b.n	30005fec <BOOT_LoadImages+0x29c>
30006720:	461a      	mov	r2, r3
30006722:	4619      	mov	r1, r3
30006724:	483f      	ldr	r0, [pc, #252]	; (30006824 <BOOT_LoadImages+0xad4>)
30006726:	f005 fff3 	bl	3000c710 <__DiagPrintf_veneer>
3000672a:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000672e:	483e      	ldr	r0, [pc, #248]	; (30006828 <BOOT_LoadImages+0xad8>)
30006730:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006734:	f005 ffec 	bl	3000c710 <__DiagPrintf_veneer>
30006738:	2300      	movs	r3, #0
3000673a:	9305      	str	r3, [sp, #20]
3000673c:	e4f0      	b.n	30006120 <BOOT_LoadImages+0x3d0>
3000673e:	483b      	ldr	r0, [pc, #236]	; (3000682c <BOOT_LoadImages+0xadc>)
30006740:	f005 ffe6 	bl	3000c710 <__DiagPrintf_veneer>
30006744:	e7fe      	b.n	30006744 <BOOT_LoadImages+0x9f4>
30006746:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30006748:	e438      	b.n	30005fbc <BOOT_LoadImages+0x26c>
3000674a:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000674c:	e5ff      	b.n	3000634e <BOOT_LoadImages+0x5fe>
3000674e:	f10a 3aff 	add.w	sl, sl, #4294967295
30006752:	f11a 0f03 	cmn.w	sl, #3
30006756:	d824      	bhi.n	300067a2 <BOOT_LoadImages+0xa52>
30006758:	f1bb 0f01 	cmp.w	fp, #1
3000675c:	d932      	bls.n	300067c4 <BOOT_LoadImages+0xa74>
3000675e:	4834      	ldr	r0, [pc, #208]	; (30006830 <BOOT_LoadImages+0xae0>)
30006760:	e649      	b.n	300063f6 <BOOT_LoadImages+0x6a6>
30006762:	4c34      	ldr	r4, [pc, #208]	; (30006834 <BOOT_LoadImages+0xae4>)
30006764:	e4e2      	b.n	3000612c <BOOT_LoadImages+0x3dc>
30006766:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30006768:	e627      	b.n	300063ba <BOOT_LoadImages+0x66a>
3000676a:	461a      	mov	r2, r3
3000676c:	4619      	mov	r1, r3
3000676e:	482c      	ldr	r0, [pc, #176]	; (30006820 <BOOT_LoadImages+0xad0>)
30006770:	f005 ffce 	bl	3000c710 <__DiagPrintf_veneer>
30006774:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006778:	4826      	ldr	r0, [pc, #152]	; (30006814 <BOOT_LoadImages+0xac4>)
3000677a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000677e:	f005 ffc7 	bl	3000c710 <__DiagPrintf_veneer>
30006782:	2300      	movs	r3, #0
30006784:	9305      	str	r3, [sp, #20]
30006786:	e74b      	b.n	30006620 <BOOT_LoadImages+0x8d0>
30006788:	4281      	cmp	r1, r0
3000678a:	f4ff ad21 	bcc.w	300061d0 <BOOT_LoadImages+0x480>
3000678e:	2400      	movs	r4, #0
30006790:	464b      	mov	r3, r9
30006792:	4642      	mov	r2, r8
30006794:	4659      	mov	r1, fp
30006796:	4620      	mov	r0, r4
30006798:	9500      	str	r5, [sp, #0]
3000679a:	9401      	str	r4, [sp, #4]
3000679c:	e55e      	b.n	3000625c <BOOT_LoadImages+0x50c>
3000679e:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300067a0:	e70a      	b.n	300065b8 <BOOT_LoadImages+0x868>
300067a2:	4825      	ldr	r0, [pc, #148]	; (30006838 <BOOT_LoadImages+0xae8>)
300067a4:	e627      	b.n	300063f6 <BOOT_LoadImages+0x6a6>
300067a6:	2400      	movs	r4, #0
300067a8:	464b      	mov	r3, r9
300067aa:	4642      	mov	r2, r8
300067ac:	4659      	mov	r1, fp
300067ae:	9500      	str	r5, [sp, #0]
300067b0:	9401      	str	r4, [sp, #4]
300067b2:	e552      	b.n	3000625a <BOOT_LoadImages+0x50a>
300067b4:	2601      	movs	r6, #1
300067b6:	464b      	mov	r3, r9
300067b8:	9a07      	ldr	r2, [sp, #28]
300067ba:	4651      	mov	r1, sl
300067bc:	2000      	movs	r0, #0
300067be:	9500      	str	r5, [sp, #0]
300067c0:	9601      	str	r6, [sp, #4]
300067c2:	e777      	b.n	300066b4 <BOOT_LoadImages+0x964>
300067c4:	9904      	ldr	r1, [sp, #16]
300067c6:	481d      	ldr	r0, [pc, #116]	; (3000683c <BOOT_LoadImages+0xaec>)
300067c8:	f005 ffa2 	bl	3000c710 <__DiagPrintf_veneer>
300067cc:	f8d9 3011 	ldr.w	r3, [r9, #17]
300067d0:	f8d9 a00d 	ldr.w	sl, [r9, #13]
300067d4:	9307      	str	r3, [sp, #28]
300067d6:	2301      	movs	r3, #1
300067d8:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300067dc:	f8d5 8011 	ldr.w	r8, [r5, #17]
300067e0:	9309      	str	r3, [sp, #36]	; 0x24
300067e2:	e615      	b.n	30006410 <BOOT_LoadImages+0x6c0>
300067e4:	f08a 0601 	eor.w	r6, sl, #1
300067e8:	f006 0601 	and.w	r6, r6, #1
300067ec:	e408      	b.n	30006000 <BOOT_LoadImages+0x2b0>
300067ee:	f08a 0601 	eor.w	r6, sl, #1
300067f2:	f8cd 8014 	str.w	r8, [sp, #20]
300067f6:	f006 0601 	and.w	r6, r6, #1
300067fa:	e401      	b.n	30006000 <BOOT_LoadImages+0x2b0>
300067fc:	2601      	movs	r6, #1
300067fe:	4c0d      	ldr	r4, [pc, #52]	; (30006834 <BOOT_LoadImages+0xae4>)
30006800:	46b2      	mov	sl, r6
30006802:	f7ff bbc1 	b.w	30005f88 <BOOT_LoadImages+0x238>
30006806:	f04f 0a00 	mov.w	sl, #0
3000680a:	e440      	b.n	3000608e <BOOT_LoadImages+0x33e>
3000680c:	3000d164 	.word	0x3000d164
30006810:	3000d108 	.word	0x3000d108
30006814:	3000ce34 	.word	0x3000ce34
30006818:	3000d118 	.word	0x3000d118
3000681c:	3000ebda 	.word	0x3000ebda
30006820:	3000d128 	.word	0x3000d128
30006824:	3000d1b0 	.word	0x3000d1b0
30006828:	3000ce4c 	.word	0x3000ce4c
3000682c:	3000d270 	.word	0x3000d270
30006830:	3000d2d0 	.word	0x3000d2d0
30006834:	3000ee80 	.word	0x3000ee80
30006838:	3000cf58 	.word	0x3000cf58
3000683c:	3000d308 	.word	0x3000d308
30006840:	30010590 	.word	0x30010590
30006844:	300043c1 	.word	0x300043c1

30006848 <BOOT_ReasonSet>:
30006848:	4a03      	ldr	r2, [pc, #12]	; (30006858 <BOOT_ReasonSet+0x10>)
3000684a:	4904      	ldr	r1, [pc, #16]	; (3000685c <BOOT_ReasonSet+0x14>)
3000684c:	8c13      	ldrh	r3, [r2, #32]
3000684e:	b29b      	uxth	r3, r3
30006850:	8413      	strh	r3, [r2, #32]
30006852:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006856:	4770      	bx	lr
30006858:	42008000 	.word	0x42008000
3000685c:	42008200 	.word	0x42008200

30006860 <BOOT_Enable_KM0>:
30006860:	b510      	push	{r4, lr}
30006862:	4c13      	ldr	r4, [pc, #76]	; (300068b0 <BOOT_Enable_KM0+0x50>)
30006864:	47a0      	blx	r4
30006866:	b110      	cbz	r0, 3000686e <BOOT_Enable_KM0+0xe>
30006868:	4b12      	ldr	r3, [pc, #72]	; (300068b4 <BOOT_Enable_KM0+0x54>)
3000686a:	4798      	blx	r3
3000686c:	b1b0      	cbz	r0, 3000689c <BOOT_Enable_KM0+0x3c>
3000686e:	47a0      	blx	r4
30006870:	b940      	cbnz	r0, 30006884 <BOOT_Enable_KM0+0x24>
30006872:	4a11      	ldr	r2, [pc, #68]	; (300068b8 <BOOT_Enable_KM0+0x58>)
30006874:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
30006878:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
3000687c:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
30006880:	bf40      	sev
30006882:	bd10      	pop	{r4, pc}
30006884:	4b0b      	ldr	r3, [pc, #44]	; (300068b4 <BOOT_Enable_KM0+0x54>)
30006886:	4798      	blx	r3
30006888:	2800      	cmp	r0, #0
3000688a:	d0f2      	beq.n	30006872 <BOOT_Enable_KM0+0x12>
3000688c:	4b0b      	ldr	r3, [pc, #44]	; (300068bc <BOOT_Enable_KM0+0x5c>)
3000688e:	689b      	ldr	r3, [r3, #8]
30006890:	079b      	lsls	r3, r3, #30
30006892:	d5ee      	bpl.n	30006872 <BOOT_Enable_KM0+0x12>
30006894:	480a      	ldr	r0, [pc, #40]	; (300068c0 <BOOT_Enable_KM0+0x60>)
30006896:	f005 ff3b 	bl	3000c710 <__DiagPrintf_veneer>
3000689a:	e7ea      	b.n	30006872 <BOOT_Enable_KM0+0x12>
3000689c:	4b07      	ldr	r3, [pc, #28]	; (300068bc <BOOT_Enable_KM0+0x5c>)
3000689e:	689b      	ldr	r3, [r3, #8]
300068a0:	079a      	lsls	r2, r3, #30
300068a2:	d5ee      	bpl.n	30006882 <BOOT_Enable_KM0+0x22>
300068a4:	4807      	ldr	r0, [pc, #28]	; (300068c4 <BOOT_Enable_KM0+0x64>)
300068a6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300068aa:	f005 bf31 	b.w	3000c710 <__DiagPrintf_veneer>
300068ae:	bf00      	nop
300068b0:	0000c09d 	.word	0x0000c09d
300068b4:	0000c0ad 	.word	0x0000c0ad
300068b8:	42008000 	.word	0x42008000
300068bc:	2001c00c 	.word	0x2001c00c
300068c0:	3000d360 	.word	0x3000d360
300068c4:	3000d324 	.word	0x3000d324

300068c8 <BOOT_AP_Clk_Get>:
300068c8:	4a06      	ldr	r2, [pc, #24]	; (300068e4 <BOOT_AP_Clk_Get+0x1c>)
300068ca:	6913      	ldr	r3, [r2, #16]
300068cc:	2b00      	cmp	r3, #0
300068ce:	db03      	blt.n	300068d8 <BOOT_AP_Clk_Get+0x10>
300068d0:	6810      	ldr	r0, [r2, #0]
300068d2:	fbb0 f0f3 	udiv	r0, r0, r3
300068d6:	4770      	bx	lr
300068d8:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300068dc:	6850      	ldr	r0, [r2, #4]
300068de:	fbb0 f0f3 	udiv	r0, r0, r3
300068e2:	4770      	bx	lr
300068e4:	3000f508 	.word	0x3000f508

300068e8 <BOOT_SOC_ClkSet>:
300068e8:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300068ec:	4f60      	ldr	r7, [pc, #384]	; (30006a70 <BOOT_SOC_ClkSet+0x188>)
300068ee:	693d      	ldr	r5, [r7, #16]
300068f0:	683e      	ldr	r6, [r7, #0]
300068f2:	2d00      	cmp	r5, #0
300068f4:	db79      	blt.n	300069ea <BOOT_SOC_ClkSet+0x102>
300068f6:	fbb6 f5f5 	udiv	r5, r6, r5
300068fa:	4b5e      	ldr	r3, [pc, #376]	; (30006a74 <BOOT_SOC_ClkSet+0x18c>)
300068fc:	f8d7 8008 	ldr.w	r8, [r7, #8]
30006900:	fba3 1306 	umull	r1, r3, r3, r6
30006904:	68fa      	ldr	r2, [r7, #12]
30006906:	f1b8 0f01 	cmp.w	r8, #1
3000690a:	ea4f 19d3 	mov.w	r9, r3, lsr #7
3000690e:	ea4f 1a53 	mov.w	sl, r3, lsr #5
30006912:	ea4f 1393 	mov.w	r3, r3, lsr #6
30006916:	f109 39ff 	add.w	r9, r9, #4294967295
3000691a:	f10a 3aff 	add.w	sl, sl, #4294967295
3000691e:	f103 34ff 	add.w	r4, r3, #4294967295
30006922:	fbb6 f6f2 	udiv	r6, r6, r2
30006926:	d071      	beq.n	30006a0c <BOOT_SOC_ClkSet+0x124>
30006928:	4b53      	ldr	r3, [pc, #332]	; (30006a78 <BOOT_SOC_ClkSet+0x190>)
3000692a:	2200      	movs	r2, #0
3000692c:	2efa      	cmp	r6, #250	; 0xfa
3000692e:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30006932:	d865      	bhi.n	30006a00 <BOOT_SOC_ClkSet+0x118>
30006934:	f8df 8170 	ldr.w	r8, [pc, #368]	; 30006aa8 <BOOT_SOC_ClkSet+0x1c0>
30006938:	f898 3000 	ldrb.w	r3, [r8]
3000693c:	2b01      	cmp	r3, #1
3000693e:	d103      	bne.n	30006948 <BOOT_SOC_ClkSet+0x60>
30006940:	f5b5 7f66 	cmp.w	r5, #920	; 0x398
30006944:	f200 8088 	bhi.w	30006a58 <BOOT_SOC_ClkSet+0x170>
30006948:	2001      	movs	r0, #1
3000694a:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000694e:	f001 fbd9 	bl	30008104 <CLK_SWITCH_XTAL>
30006952:	683b      	ldr	r3, [r7, #0]
30006954:	4849      	ldr	r0, [pc, #292]	; (30006a7c <BOOT_SOC_ClkSet+0x194>)
30006956:	f409 69e0 	and.w	r9, r9, #1792	; 0x700
3000695a:	0524      	lsls	r4, r4, #20
3000695c:	fb03 f000 	mul.w	r0, r3, r0
30006960:	4b47      	ldr	r3, [pc, #284]	; (30006a80 <BOOT_SOC_ClkSet+0x198>)
30006962:	4798      	blx	r3
30006964:	68fa      	ldr	r2, [r7, #12]
30006966:	4847      	ldr	r0, [pc, #284]	; (30006a84 <BOOT_SOC_ClkSet+0x19c>)
30006968:	ea4f 310a 	mov.w	r1, sl, lsl #12
3000696c:	1e53      	subs	r3, r2, #1
3000696e:	4a46      	ldr	r2, [pc, #280]	; (30006a88 <BOOT_SOC_ClkSet+0x1a0>)
30006970:	f8d0 c228 	ldr.w	ip, [r0, #552]	; 0x228
30006974:	b289      	uxth	r1, r1
30006976:	f003 0307 	and.w	r3, r3, #7
3000697a:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
3000697e:	ea0c 0202 	and.w	r2, ip, r2
30006982:	4313      	orrs	r3, r2
30006984:	ea49 0303 	orr.w	r3, r9, r3
30006988:	430b      	orrs	r3, r1
3000698a:	4323      	orrs	r3, r4
3000698c:	4c3f      	ldr	r4, [pc, #252]	; (30006a8c <BOOT_SOC_ClkSet+0x1a4>)
3000698e:	f8c0 3228 	str.w	r3, [r0, #552]	; 0x228
30006992:	2000      	movs	r0, #0
30006994:	f001 fbb6 	bl	30008104 <CLK_SWITCH_XTAL>
30006998:	68a3      	ldr	r3, [r4, #8]
3000699a:	079a      	lsls	r2, r3, #30
3000699c:	d42b      	bmi.n	300069f6 <BOOT_SOC_ClkSet+0x10e>
3000699e:	f898 3000 	ldrb.w	r3, [r8]
300069a2:	b303      	cbz	r3, 300069e6 <BOOT_SOC_ClkSet+0xfe>
300069a4:	693b      	ldr	r3, [r7, #16]
300069a6:	2b00      	cmp	r3, #0
300069a8:	db48      	blt.n	30006a3c <BOOT_SOC_ClkSet+0x154>
300069aa:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300069ae:	6913      	ldr	r3, [r2, #16]
300069b0:	f043 030c 	orr.w	r3, r3, #12
300069b4:	6113      	str	r3, [r2, #16]
300069b6:	2200      	movs	r2, #0
300069b8:	4b2f      	ldr	r3, [pc, #188]	; (30006a78 <BOOT_SOC_ClkSet+0x190>)
300069ba:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300069be:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
300069c2:	693b      	ldr	r3, [r7, #16]
300069c4:	68a0      	ldr	r0, [r4, #8]
300069c6:	690a      	ldr	r2, [r1, #16]
300069c8:	3b01      	subs	r3, #1
300069ca:	f022 0203 	bic.w	r2, r2, #3
300069ce:	f003 0303 	and.w	r3, r3, #3
300069d2:	4313      	orrs	r3, r2
300069d4:	610b      	str	r3, [r1, #16]
300069d6:	0783      	lsls	r3, r0, #30
300069d8:	d505      	bpl.n	300069e6 <BOOT_SOC_ClkSet+0xfe>
300069da:	4629      	mov	r1, r5
300069dc:	482c      	ldr	r0, [pc, #176]	; (30006a90 <BOOT_SOC_ClkSet+0x1a8>)
300069de:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300069e2:	f005 be95 	b.w	3000c710 <__DiagPrintf_veneer>
300069e6:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300069ea:	f025 4500 	bic.w	r5, r5, #2147483648	; 0x80000000
300069ee:	687b      	ldr	r3, [r7, #4]
300069f0:	fbb3 f5f5 	udiv	r5, r3, r5
300069f4:	e781      	b.n	300068fa <BOOT_SOC_ClkSet+0x12>
300069f6:	4631      	mov	r1, r6
300069f8:	4826      	ldr	r0, [pc, #152]	; (30006a94 <BOOT_SOC_ClkSet+0x1ac>)
300069fa:	f005 fe89 	bl	3000c710 <__DiagPrintf_veneer>
300069fe:	e7ce      	b.n	3000699e <BOOT_SOC_ClkSet+0xb6>
30006a00:	f240 61bb 	movw	r1, #1723	; 0x6bb
30006a04:	4824      	ldr	r0, [pc, #144]	; (30006a98 <BOOT_SOC_ClkSet+0x1b0>)
30006a06:	f005 fe7b 	bl	3000c700 <__io_assert_failed_veneer>
30006a0a:	e793      	b.n	30006934 <BOOT_SOC_ClkSet+0x4c>
30006a0c:	4b23      	ldr	r3, [pc, #140]	; (30006a9c <BOOT_SOC_ClkSet+0x1b4>)
30006a0e:	4640      	mov	r0, r8
30006a10:	4798      	blx	r3
30006a12:	4b19      	ldr	r3, [pc, #100]	; (30006a78 <BOOT_SOC_ClkSet+0x190>)
30006a14:	f5b6 7fa7 	cmp.w	r6, #334	; 0x14e
30006a18:	f883 810c 	strb.w	r8, [r3, #268]	; 0x10c
30006a1c:	d222      	bcs.n	30006a64 <BOOT_SOC_ClkSet+0x17c>
30006a1e:	f8df 8088 	ldr.w	r8, [pc, #136]	; 30006aa8 <BOOT_SOC_ClkSet+0x1c0>
30006a22:	f898 3000 	ldrb.w	r3, [r8]
30006a26:	2b01      	cmp	r3, #1
30006a28:	d18e      	bne.n	30006948 <BOOT_SOC_ClkSet+0x60>
30006a2a:	f5b5 6f96 	cmp.w	r5, #1200	; 0x4b0
30006a2e:	d98b      	bls.n	30006948 <BOOT_SOC_ClkSet+0x60>
30006a30:	f240 61b7 	movw	r1, #1719	; 0x6b7
30006a34:	4818      	ldr	r0, [pc, #96]	; (30006a98 <BOOT_SOC_ClkSet+0x1b0>)
30006a36:	f005 fe63 	bl	3000c700 <__io_assert_failed_veneer>
30006a3a:	e785      	b.n	30006948 <BOOT_SOC_ClkSet+0x60>
30006a3c:	687b      	ldr	r3, [r7, #4]
30006a3e:	480f      	ldr	r0, [pc, #60]	; (30006a7c <BOOT_SOC_ClkSet+0x194>)
30006a40:	fb03 f000 	mul.w	r0, r3, r0
30006a44:	4b16      	ldr	r3, [pc, #88]	; (30006aa0 <BOOT_SOC_ClkSet+0x1b8>)
30006a46:	4798      	blx	r3
30006a48:	4b16      	ldr	r3, [pc, #88]	; (30006aa4 <BOOT_SOC_ClkSet+0x1bc>)
30006a4a:	2001      	movs	r0, #1
30006a4c:	4798      	blx	r3
30006a4e:	4b0a      	ldr	r3, [pc, #40]	; (30006a78 <BOOT_SOC_ClkSet+0x190>)
30006a50:	2201      	movs	r2, #1
30006a52:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006a56:	e7b2      	b.n	300069be <BOOT_SOC_ClkSet+0xd6>
30006a58:	f240 61bd 	movw	r1, #1725	; 0x6bd
30006a5c:	480e      	ldr	r0, [pc, #56]	; (30006a98 <BOOT_SOC_ClkSet+0x1b0>)
30006a5e:	f005 fe4f 	bl	3000c700 <__io_assert_failed_veneer>
30006a62:	e771      	b.n	30006948 <BOOT_SOC_ClkSet+0x60>
30006a64:	f240 61b5 	movw	r1, #1717	; 0x6b5
30006a68:	480b      	ldr	r0, [pc, #44]	; (30006a98 <BOOT_SOC_ClkSet+0x1b0>)
30006a6a:	f005 fe49 	bl	3000c700 <__io_assert_failed_veneer>
30006a6e:	e7d6      	b.n	30006a1e <BOOT_SOC_ClkSet+0x136>
30006a70:	3000f508 	.word	0x3000f508
30006a74:	51eb851f 	.word	0x51eb851f
30006a78:	23020000 	.word	0x23020000
30006a7c:	000f4240 	.word	0x000f4240
30006a80:	3000910d 	.word	0x3000910d
30006a84:	42008000 	.word	0x42008000
30006a88:	ff8f08f8 	.word	0xff8f08f8
30006a8c:	2001c00c 	.word	0x2001c00c
30006a90:	3000d3bc 	.word	0x3000d3bc
30006a94:	3000d390 	.word	0x3000d390
30006a98:	3000e8e4 	.word	0x3000e8e4
30006a9c:	3000b8b1 	.word	0x3000b8b1
30006aa0:	3000918d 	.word	0x3000918d
30006aa4:	300091cd 	.word	0x300091cd
30006aa8:	3000ebda 	.word	0x3000ebda

30006aac <BOOT_Disable_AP>:
30006aac:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006ab0:	4906      	ldr	r1, [pc, #24]	; (30006acc <BOOT_Disable_AP+0x20>)
30006ab2:	685a      	ldr	r2, [r3, #4]
30006ab4:	f042 0202 	orr.w	r2, r2, #2
30006ab8:	605a      	str	r2, [r3, #4]
30006aba:	685a      	ldr	r2, [r3, #4]
30006abc:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006ac0:	605a      	str	r2, [r3, #4]
30006ac2:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006ac4:	f043 0303 	orr.w	r3, r3, #3
30006ac8:	678b      	str	r3, [r1, #120]	; 0x78
30006aca:	4770      	bx	lr
30006acc:	41000200 	.word	0x41000200

30006ad0 <BOOT_Enable_AP>:
30006ad0:	b538      	push	{r3, r4, r5, lr}
30006ad2:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006ad6:	2201      	movs	r2, #1
30006ad8:	2104      	movs	r1, #4
30006ada:	4b2b      	ldr	r3, [pc, #172]	; (30006b88 <BOOT_Enable_AP+0xb8>)
30006adc:	482b      	ldr	r0, [pc, #172]	; (30006b8c <BOOT_Enable_AP+0xbc>)
30006ade:	4798      	blx	r3
30006ae0:	6863      	ldr	r3, [r4, #4]
30006ae2:	4d2b      	ldr	r5, [pc, #172]	; (30006b90 <BOOT_Enable_AP+0xc0>)
30006ae4:	2032      	movs	r0, #50	; 0x32
30006ae6:	f043 0302 	orr.w	r3, r3, #2
30006aea:	6063      	str	r3, [r4, #4]
30006aec:	6863      	ldr	r3, [r4, #4]
30006aee:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30006af2:	6063      	str	r3, [r4, #4]
30006af4:	6823      	ldr	r3, [r4, #0]
30006af6:	f043 0302 	orr.w	r3, r3, #2
30006afa:	6023      	str	r3, [r4, #0]
30006afc:	47a8      	blx	r5
30006afe:	6823      	ldr	r3, [r4, #0]
30006b00:	2032      	movs	r0, #50	; 0x32
30006b02:	f043 0304 	orr.w	r3, r3, #4
30006b06:	6023      	str	r3, [r4, #0]
30006b08:	6823      	ldr	r3, [r4, #0]
30006b0a:	f043 0301 	orr.w	r3, r3, #1
30006b0e:	6023      	str	r3, [r4, #0]
30006b10:	6823      	ldr	r3, [r4, #0]
30006b12:	f043 0310 	orr.w	r3, r3, #16
30006b16:	6023      	str	r3, [r4, #0]
30006b18:	47a8      	blx	r5
30006b1a:	6823      	ldr	r3, [r4, #0]
30006b1c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30006b20:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006b24:	6023      	str	r3, [r4, #0]
30006b26:	47a8      	blx	r5
30006b28:	6863      	ldr	r3, [r4, #4]
30006b2a:	4a1a      	ldr	r2, [pc, #104]	; (30006b94 <BOOT_Enable_AP+0xc4>)
30006b2c:	f023 0310 	bic.w	r3, r3, #16
30006b30:	6063      	str	r3, [r4, #4]
30006b32:	6863      	ldr	r3, [r4, #4]
30006b34:	f023 0302 	bic.w	r3, r3, #2
30006b38:	6063      	str	r3, [r4, #4]
30006b3a:	4790      	blx	r2
30006b3c:	b178      	cbz	r0, 30006b5e <BOOT_Enable_AP+0x8e>
30006b3e:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006b42:	4815      	ldr	r0, [pc, #84]	; (30006b98 <BOOT_Enable_AP+0xc8>)
30006b44:	4a15      	ldr	r2, [pc, #84]	; (30006b9c <BOOT_Enable_AP+0xcc>)
30006b46:	68d9      	ldr	r1, [r3, #12]
30006b48:	f041 0101 	orr.w	r1, r1, #1
30006b4c:	60d9      	str	r1, [r3, #12]
30006b4e:	6899      	ldr	r1, [r3, #8]
30006b50:	f041 0101 	orr.w	r1, r1, #1
30006b54:	6099      	str	r1, [r3, #8]
30006b56:	6843      	ldr	r3, [r0, #4]
30006b58:	431a      	orrs	r2, r3
30006b5a:	6042      	str	r2, [r0, #4]
30006b5c:	bd38      	pop	{r3, r4, r5, pc}
30006b5e:	4604      	mov	r4, r0
30006b60:	4d0f      	ldr	r5, [pc, #60]	; (30006ba0 <BOOT_Enable_AP+0xd0>)
30006b62:	4601      	mov	r1, r0
30006b64:	2041      	movs	r0, #65	; 0x41
30006b66:	47a8      	blx	r5
30006b68:	4621      	mov	r1, r4
30006b6a:	2042      	movs	r0, #66	; 0x42
30006b6c:	47a8      	blx	r5
30006b6e:	4621      	mov	r1, r4
30006b70:	2043      	movs	r0, #67	; 0x43
30006b72:	47a8      	blx	r5
30006b74:	4621      	mov	r1, r4
30006b76:	2044      	movs	r0, #68	; 0x44
30006b78:	47a8      	blx	r5
30006b7a:	4621      	mov	r1, r4
30006b7c:	2045      	movs	r0, #69	; 0x45
30006b7e:	47a8      	blx	r5
30006b80:	4621      	mov	r1, r4
30006b82:	2046      	movs	r0, #70	; 0x46
30006b84:	47a8      	blx	r5
30006b86:	e7da      	b.n	30006b3e <BOOT_Enable_AP+0x6e>
30006b88:	0000aab9 	.word	0x0000aab9
30006b8c:	4200c000 	.word	0x4200c000
30006b90:	00009b2d 	.word	0x00009b2d
30006b94:	3000a261 	.word	0x3000a261
30006b98:	41000200 	.word	0x41000200
30006b9c:	01001111 	.word	0x01001111
30006ba0:	0000b20d 	.word	0x0000b20d

30006ba4 <BOOT_RAM_KeyDeriveFunc>:
30006ba4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ba8:	ed2d 8b04 	vpush	{d8-d9}
30006bac:	b0cb      	sub	sp, #300	; 0x12c
30006bae:	4c6a      	ldr	r4, [pc, #424]	; (30006d58 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006bb0:	4680      	mov	r8, r0
30006bb2:	4689      	mov	r9, r1
30006bb4:	4869      	ldr	r0, [pc, #420]	; (30006d5c <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006bb6:	ee08 2a90 	vmov	s17, r2
30006bba:	4969      	ldr	r1, [pc, #420]	; (30006d60 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006bbc:	2201      	movs	r2, #1
30006bbe:	ee09 3a10 	vmov	s18, r3
30006bc2:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006bc6:	47a0      	blx	r4
30006bc8:	2201      	movs	r2, #1
30006bca:	4966      	ldr	r1, [pc, #408]	; (30006d64 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006bcc:	ae02      	add	r6, sp, #8
30006bce:	4864      	ldr	r0, [pc, #400]	; (30006d60 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006bd0:	47a0      	blx	r4
30006bd2:	4b65      	ldr	r3, [pc, #404]	; (30006d68 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006bd4:	2000      	movs	r0, #0
30006bd6:	4798      	blx	r3
30006bd8:	22e0      	movs	r2, #224	; 0xe0
30006bda:	2100      	movs	r1, #0
30006bdc:	a812      	add	r0, sp, #72	; 0x48
30006bde:	f005 fd67 	bl	3000c6b0 <____wrap_memset_veneer>
30006be2:	2300      	movs	r3, #0
30006be4:	f8ad 3004 	strh.w	r3, [sp, #4]
30006be8:	f88d 3006 	strb.w	r3, [sp, #6]
30006bec:	2301      	movs	r3, #1
30006bee:	f88d 3007 	strb.w	r3, [sp, #7]
30006bf2:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006bf4:	2b00      	cmp	r3, #0
30006bf6:	f000 80ab 	beq.w	30006d50 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006bfa:	ab0a      	add	r3, sp, #40	; 0x28
30006bfc:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006d7c <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006c00:	4f5a      	ldr	r7, [pc, #360]	; (30006d6c <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006c02:	ee08 3a10 	vmov	s16, r3
30006c06:	ab12      	add	r3, sp, #72	; 0x48
30006c08:	464a      	mov	r2, r9
30006c0a:	4641      	mov	r1, r8
30006c0c:	2020      	movs	r0, #32
30006c0e:	47d8      	blx	fp
30006c10:	aa12      	add	r2, sp, #72	; 0x48
30006c12:	ee19 1a10 	vmov	r1, s18
30006c16:	ee18 0a90 	vmov	r0, s17
30006c1a:	47b8      	blx	r7
30006c1c:	ee18 5a10 	vmov	r5, s16
30006c20:	aa12      	add	r2, sp, #72	; 0x48
30006c22:	2104      	movs	r1, #4
30006c24:	a801      	add	r0, sp, #4
30006c26:	4634      	mov	r4, r6
30006c28:	47b8      	blx	r7
30006c2a:	a912      	add	r1, sp, #72	; 0x48
30006c2c:	4b50      	ldr	r3, [pc, #320]	; (30006d70 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c2e:	ee18 0a10 	vmov	r0, s16
30006c32:	4798      	blx	r3
30006c34:	f1ba 0f01 	cmp.w	sl, #1
30006c38:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006c3a:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006c3c:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006c40:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006c44:	d91f      	bls.n	30006c86 <BOOT_RAM_KeyDeriveFunc+0xe2>
30006c46:	2501      	movs	r5, #1
30006c48:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006c4c:	ab12      	add	r3, sp, #72	; 0x48
30006c4e:	4641      	mov	r1, r8
30006c50:	464a      	mov	r2, r9
30006c52:	2020      	movs	r0, #32
30006c54:	47d8      	blx	fp
30006c56:	aa12      	add	r2, sp, #72	; 0x48
30006c58:	2120      	movs	r1, #32
30006c5a:	4630      	mov	r0, r6
30006c5c:	47b8      	blx	r7
30006c5e:	a912      	add	r1, sp, #72	; 0x48
30006c60:	4b43      	ldr	r3, [pc, #268]	; (30006d70 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c62:	4630      	mov	r0, r6
30006c64:	4798      	blx	r3
30006c66:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006c6a:	f10d 0107 	add.w	r1, sp, #7
30006c6e:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006c72:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006c76:	42a3      	cmp	r3, r4
30006c78:	ea82 0200 	eor.w	r2, r2, r0
30006c7c:	701a      	strb	r2, [r3, #0]
30006c7e:	d1f6      	bne.n	30006c6e <BOOT_RAM_KeyDeriveFunc+0xca>
30006c80:	3501      	adds	r5, #1
30006c82:	45aa      	cmp	sl, r5
30006c84:	d1e2      	bne.n	30006c4c <BOOT_RAM_KeyDeriveFunc+0xa8>
30006c86:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006c88:	ee18 1a10 	vmov	r1, s16
30006c8c:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c8e:	2a20      	cmp	r2, #32
30006c90:	985a      	ldr	r0, [sp, #360]	; 0x168
30006c92:	bf28      	it	cs
30006c94:	2220      	movcs	r2, #32
30006c96:	1a9b      	subs	r3, r3, r2
30006c98:	9359      	str	r3, [sp, #356]	; 0x164
30006c9a:	4603      	mov	r3, r0
30006c9c:	4413      	add	r3, r2
30006c9e:	935a      	str	r3, [sp, #360]	; 0x168
30006ca0:	f005 fd1e 	bl	3000c6e0 <____wrap_memcpy_veneer>
30006ca4:	f10d 0207 	add.w	r2, sp, #7
30006ca8:	2104      	movs	r1, #4
30006caa:	7813      	ldrb	r3, [r2, #0]
30006cac:	3901      	subs	r1, #1
30006cae:	3301      	adds	r3, #1
30006cb0:	b2db      	uxtb	r3, r3
30006cb2:	f802 3901 	strb.w	r3, [r2], #-1
30006cb6:	b90b      	cbnz	r3, 30006cbc <BOOT_RAM_KeyDeriveFunc+0x118>
30006cb8:	2900      	cmp	r1, #0
30006cba:	d1f6      	bne.n	30006caa <BOOT_RAM_KeyDeriveFunc+0x106>
30006cbc:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006cbe:	2b00      	cmp	r3, #0
30006cc0:	d1a1      	bne.n	30006c06 <BOOT_RAM_KeyDeriveFunc+0x62>
30006cc2:	2220      	movs	r2, #32
30006cc4:	2100      	movs	r1, #0
30006cc6:	4c2b      	ldr	r4, [pc, #172]	; (30006d74 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006cc8:	4630      	mov	r0, r6
30006cca:	47a0      	blx	r4
30006ccc:	2220      	movs	r2, #32
30006cce:	2100      	movs	r1, #0
30006cd0:	ee18 0a10 	vmov	r0, s16
30006cd4:	47a0      	blx	r4
30006cd6:	4b28      	ldr	r3, [pc, #160]	; (30006d78 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cd8:	695b      	ldr	r3, [r3, #20]
30006cda:	03da      	lsls	r2, r3, #15
30006cdc:	d512      	bpl.n	30006d04 <BOOT_RAM_KeyDeriveFunc+0x160>
30006cde:	f016 021f 	ands.w	r2, r6, #31
30006ce2:	4633      	mov	r3, r6
30006ce4:	d130      	bne.n	30006d48 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006ce6:	2220      	movs	r2, #32
30006ce8:	f3bf 8f4f 	dsb	sy
30006cec:	4822      	ldr	r0, [pc, #136]	; (30006d78 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cee:	441a      	add	r2, r3
30006cf0:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cf4:	3320      	adds	r3, #32
30006cf6:	1ad1      	subs	r1, r2, r3
30006cf8:	2900      	cmp	r1, #0
30006cfa:	dcf9      	bgt.n	30006cf0 <BOOT_RAM_KeyDeriveFunc+0x14c>
30006cfc:	f3bf 8f4f 	dsb	sy
30006d00:	f3bf 8f6f 	isb	sy
30006d04:	4b1c      	ldr	r3, [pc, #112]	; (30006d78 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006d06:	695b      	ldr	r3, [r3, #20]
30006d08:	03db      	lsls	r3, r3, #15
30006d0a:	d513      	bpl.n	30006d34 <BOOT_RAM_KeyDeriveFunc+0x190>
30006d0c:	ee18 3a10 	vmov	r3, s16
30006d10:	f013 021f 	ands.w	r2, r3, #31
30006d14:	d114      	bne.n	30006d40 <BOOT_RAM_KeyDeriveFunc+0x19c>
30006d16:	2220      	movs	r2, #32
30006d18:	f3bf 8f4f 	dsb	sy
30006d1c:	4816      	ldr	r0, [pc, #88]	; (30006d78 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006d1e:	441a      	add	r2, r3
30006d20:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006d24:	3320      	adds	r3, #32
30006d26:	1ad1      	subs	r1, r2, r3
30006d28:	2900      	cmp	r1, #0
30006d2a:	dcf9      	bgt.n	30006d20 <BOOT_RAM_KeyDeriveFunc+0x17c>
30006d2c:	f3bf 8f4f 	dsb	sy
30006d30:	f3bf 8f6f 	isb	sy
30006d34:	2000      	movs	r0, #0
30006d36:	b04b      	add	sp, #300	; 0x12c
30006d38:	ecbd 8b04 	vpop	{d8-d9}
30006d3c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d40:	f023 031f 	bic.w	r3, r3, #31
30006d44:	3220      	adds	r2, #32
30006d46:	e7e7      	b.n	30006d18 <BOOT_RAM_KeyDeriveFunc+0x174>
30006d48:	f026 031f 	bic.w	r3, r6, #31
30006d4c:	3220      	adds	r2, #32
30006d4e:	e7cb      	b.n	30006ce8 <BOOT_RAM_KeyDeriveFunc+0x144>
30006d50:	ab0a      	add	r3, sp, #40	; 0x28
30006d52:	ee08 3a10 	vmov	s16, r3
30006d56:	e7b4      	b.n	30006cc2 <BOOT_RAM_KeyDeriveFunc+0x11e>
30006d58:	0000b479 	.word	0x0000b479
30006d5c:	40000008 	.word	0x40000008
30006d60:	40000004 	.word	0x40000004
30006d64:	40000002 	.word	0x40000002
30006d68:	00002451 	.word	0x00002451
30006d6c:	00005b4d 	.word	0x00005b4d
30006d70:	00005bb9 	.word	0x00005bb9
30006d74:	00012be5 	.word	0x00012be5
30006d78:	e000ed00 	.word	0xe000ed00
30006d7c:	30009b81 	.word	0x30009b81

30006d80 <password_hash_check>:
30006d80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d84:	b08f      	sub	sp, #60	; 0x3c
30006d86:	4690      	mov	r8, r2
30006d88:	461d      	mov	r5, r3
30006d8a:	4606      	mov	r6, r0
30006d8c:	af04      	add	r7, sp, #16
30006d8e:	3201      	adds	r2, #1
30006d90:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d92:	6079      	str	r1, [r7, #4]
30006d94:	2100      	movs	r1, #0
30006d96:	f103 0008 	add.w	r0, r3, #8
30006d9a:	f108 0308 	add.w	r3, r8, #8
30006d9e:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006da2:	f023 0307 	bic.w	r3, r3, #7
30006da6:	f020 0007 	bic.w	r0, r0, #7
30006daa:	ebad 0d03 	sub.w	sp, sp, r3
30006dae:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006db0:	f10d 0910 	add.w	r9, sp, #16
30006db4:	3301      	adds	r3, #1
30006db6:	ebad 0d00 	sub.w	sp, sp, r0
30006dba:	603b      	str	r3, [r7, #0]
30006dbc:	4648      	mov	r0, r9
30006dbe:	4b3f      	ldr	r3, [pc, #252]	; (30006ebc <password_hash_check+0x13c>)
30006dc0:	f10d 0a10 	add.w	sl, sp, #16
30006dc4:	4798      	blx	r3
30006dc6:	683b      	ldr	r3, [r7, #0]
30006dc8:	2100      	movs	r1, #0
30006dca:	4650      	mov	r0, sl
30006dcc:	461a      	mov	r2, r3
30006dce:	4b3b      	ldr	r3, [pc, #236]	; (30006ebc <password_hash_check+0x13c>)
30006dd0:	4798      	blx	r3
30006dd2:	f1b8 0001 	subs.w	r0, r8, #1
30006dd6:	d01d      	beq.n	30006e14 <password_hash_check+0x94>
30006dd8:	2200      	movs	r2, #0
30006dda:	f109 33ff 	add.w	r3, r9, #4294967295
30006dde:	4611      	mov	r1, r2
30006de0:	e00b      	b.n	30006dfa <password_hash_check+0x7a>
30006de2:	f108 38ff 	add.w	r8, r8, #4294967295
30006de6:	2100      	movs	r1, #0
30006de8:	1c9c      	adds	r4, r3, #2
30006dea:	4282      	cmp	r2, r0
30006dec:	7059      	strb	r1, [r3, #1]
30006dee:	f103 0301 	add.w	r3, r3, #1
30006df2:	4611      	mov	r1, r2
30006df4:	eba4 0409 	sub.w	r4, r4, r9
30006df8:	d20b      	bcs.n	30006e12 <password_hash_check+0x92>
30006dfa:	1874      	adds	r4, r6, r1
30006dfc:	f102 0c01 	add.w	ip, r2, #1
30006e00:	5c71      	ldrb	r1, [r6, r1]
30006e02:	3202      	adds	r2, #2
30006e04:	7864      	ldrb	r4, [r4, #1]
30006e06:	2c30      	cmp	r4, #48	; 0x30
30006e08:	d101      	bne.n	30006e0e <password_hash_check+0x8e>
30006e0a:	295c      	cmp	r1, #92	; 0x5c
30006e0c:	d0e9      	beq.n	30006de2 <password_hash_check+0x62>
30006e0e:	4662      	mov	r2, ip
30006e10:	e7ea      	b.n	30006de8 <password_hash_check+0x68>
30006e12:	4406      	add	r6, r0
30006e14:	7833      	ldrb	r3, [r6, #0]
30006e16:	f809 3004 	strb.w	r3, [r9, r4]
30006e1a:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006e1c:	1e5c      	subs	r4, r3, #1
30006e1e:	d04a      	beq.n	30006eb6 <password_hash_check+0x136>
30006e20:	2200      	movs	r2, #0
30006e22:	f10a 33ff 	add.w	r3, sl, #4294967295
30006e26:	4611      	mov	r1, r2
30006e28:	e00b      	b.n	30006e42 <password_hash_check+0xc2>
30006e2a:	f10b 3bff 	add.w	fp, fp, #4294967295
30006e2e:	2100      	movs	r1, #0
30006e30:	1c98      	adds	r0, r3, #2
30006e32:	42a2      	cmp	r2, r4
30006e34:	7059      	strb	r1, [r3, #1]
30006e36:	f103 0301 	add.w	r3, r3, #1
30006e3a:	4611      	mov	r1, r2
30006e3c:	eba0 000a 	sub.w	r0, r0, sl
30006e40:	d20a      	bcs.n	30006e58 <password_hash_check+0xd8>
30006e42:	1868      	adds	r0, r5, r1
30006e44:	1c56      	adds	r6, r2, #1
30006e46:	5c69      	ldrb	r1, [r5, r1]
30006e48:	3202      	adds	r2, #2
30006e4a:	7840      	ldrb	r0, [r0, #1]
30006e4c:	2830      	cmp	r0, #48	; 0x30
30006e4e:	d101      	bne.n	30006e54 <password_hash_check+0xd4>
30006e50:	295c      	cmp	r1, #92	; 0x5c
30006e52:	d0ea      	beq.n	30006e2a <password_hash_check+0xaa>
30006e54:	4632      	mov	r2, r6
30006e56:	e7eb      	b.n	30006e30 <password_hash_check+0xb0>
30006e58:	4425      	add	r5, r4
30006e5a:	782b      	ldrb	r3, [r5, #0]
30006e5c:	f107 0408 	add.w	r4, r7, #8
30006e60:	2520      	movs	r5, #32
30006e62:	4641      	mov	r1, r8
30006e64:	f80a 3000 	strb.w	r3, [sl, r0]
30006e68:	4652      	mov	r2, sl
30006e6a:	687b      	ldr	r3, [r7, #4]
30006e6c:	4648      	mov	r0, r9
30006e6e:	9402      	str	r4, [sp, #8]
30006e70:	9300      	str	r3, [sp, #0]
30006e72:	465b      	mov	r3, fp
30006e74:	9501      	str	r5, [sp, #4]
30006e76:	f7ff fe95 	bl	30006ba4 <BOOT_RAM_KeyDeriveFunc>
30006e7a:	4620      	mov	r0, r4
30006e7c:	462a      	mov	r2, r5
30006e7e:	4910      	ldr	r1, [pc, #64]	; (30006ec0 <password_hash_check+0x140>)
30006e80:	f005 fc4e 	bl	3000c720 <____wrap_memcmp_veneer>
30006e84:	4604      	mov	r4, r0
30006e86:	b968      	cbnz	r0, 30006ea4 <password_hash_check+0x124>
30006e88:	480e      	ldr	r0, [pc, #56]	; (30006ec4 <password_hash_check+0x144>)
30006e8a:	f005 fc41 	bl	3000c710 <__DiagPrintf_veneer>
30006e8e:	462a      	mov	r2, r5
30006e90:	4621      	mov	r1, r4
30006e92:	f107 0008 	add.w	r0, r7, #8
30006e96:	4b09      	ldr	r3, [pc, #36]	; (30006ebc <password_hash_check+0x13c>)
30006e98:	4798      	blx	r3
30006e9a:	4620      	mov	r0, r4
30006e9c:	372c      	adds	r7, #44	; 0x2c
30006e9e:	46bd      	mov	sp, r7
30006ea0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ea4:	2401      	movs	r4, #1
30006ea6:	4808      	ldr	r0, [pc, #32]	; (30006ec8 <password_hash_check+0x148>)
30006ea8:	f005 fc32 	bl	3000c710 <__DiagPrintf_veneer>
30006eac:	372c      	adds	r7, #44	; 0x2c
30006eae:	4620      	mov	r0, r4
30006eb0:	46bd      	mov	sp, r7
30006eb2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006eb6:	4620      	mov	r0, r4
30006eb8:	e7cf      	b.n	30006e5a <password_hash_check+0xda>
30006eba:	bf00      	nop
30006ebc:	00012be5 	.word	0x00012be5
30006ec0:	3000f288 	.word	0x3000f288
30006ec4:	3000d400 	.word	0x3000d400
30006ec8:	3000d3e8 	.word	0x3000d3e8

30006ecc <BOOT_Share_Memory_Patch>:
30006ecc:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30006ed0:	4817      	ldr	r0, [pc, #92]	; (30006f30 <BOOT_Share_Memory_Patch+0x64>)
30006ed2:	4918      	ldr	r1, [pc, #96]	; (30006f34 <BOOT_Share_Memory_Patch+0x68>)
30006ed4:	b430      	push	{r4, r5}
30006ed6:	6b13      	ldr	r3, [r2, #48]	; 0x30
30006ed8:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30006edc:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30006ee0:	6313      	str	r3, [r2, #48]	; 0x30
30006ee2:	6823      	ldr	r3, [r4, #0]
30006ee4:	6804      	ldr	r4, [r0, #0]
30006ee6:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006eea:	6809      	ldr	r1, [r1, #0]
30006eec:	4423      	add	r3, r4
30006eee:	6805      	ldr	r5, [r0, #0]
30006ef0:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30006ef4:	440b      	add	r3, r1
30006ef6:	4910      	ldr	r1, [pc, #64]	; (30006f38 <BOOT_Share_Memory_Patch+0x6c>)
30006ef8:	6808      	ldr	r0, [r1, #0]
30006efa:	442b      	add	r3, r5
30006efc:	6b11      	ldr	r1, [r2, #48]	; 0x30
30006efe:	4403      	add	r3, r0
30006f00:	480e      	ldr	r0, [pc, #56]	; (30006f3c <BOOT_Share_Memory_Patch+0x70>)
30006f02:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30006f06:	6311      	str	r1, [r2, #48]	; 0x30
30006f08:	6821      	ldr	r1, [r4, #0]
30006f0a:	4a0d      	ldr	r2, [pc, #52]	; (30006f40 <BOOT_Share_Memory_Patch+0x74>)
30006f0c:	6804      	ldr	r4, [r0, #0]
30006f0e:	440b      	add	r3, r1
30006f10:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006f14:	6811      	ldr	r1, [r2, #0]
30006f16:	4423      	add	r3, r4
30006f18:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30006f1c:	6800      	ldr	r0, [r0, #0]
30006f1e:	440b      	add	r3, r1
30006f20:	6812      	ldr	r2, [r2, #0]
30006f22:	4908      	ldr	r1, [pc, #32]	; (30006f44 <BOOT_Share_Memory_Patch+0x78>)
30006f24:	4403      	add	r3, r0
30006f26:	6808      	ldr	r0, [r1, #0]
30006f28:	4413      	add	r3, r2
30006f2a:	bc30      	pop	{r4, r5}
30006f2c:	4418      	add	r0, r3
30006f2e:	4770      	bx	lr
30006f30:	2200c000 	.word	0x2200c000
30006f34:	22018000 	.word	0x22018000
30006f38:	22100000 	.word	0x22100000
30006f3c:	20010000 	.word	0x20010000
30006f40:	20020000 	.word	0x20020000
30006f44:	23010000 	.word	0x23010000

30006f48 <BOOT_Image1>:
30006f48:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006f4c:	4b96      	ldr	r3, [pc, #600]	; (300071a8 <BOOT_Image1+0x260>)
30006f4e:	b08b      	sub	sp, #44	; 0x2c
30006f50:	4798      	blx	r3
30006f52:	2800      	cmp	r0, #0
30006f54:	f040 8349 	bne.w	300075ea <BOOT_Image1+0x6a2>
30006f58:	4894      	ldr	r0, [pc, #592]	; (300071ac <BOOT_Image1+0x264>)
30006f5a:	f005 fbd9 	bl	3000c710 <__DiagPrintf_veneer>
30006f5e:	4994      	ldr	r1, [pc, #592]	; (300071b0 <BOOT_Image1+0x268>)
30006f60:	4894      	ldr	r0, [pc, #592]	; (300071b4 <BOOT_Image1+0x26c>)
30006f62:	f005 fbd5 	bl	3000c710 <__DiagPrintf_veneer>
30006f66:	4894      	ldr	r0, [pc, #592]	; (300071b8 <BOOT_Image1+0x270>)
30006f68:	4a94      	ldr	r2, [pc, #592]	; (300071bc <BOOT_Image1+0x274>)
30006f6a:	2100      	movs	r1, #0
30006f6c:	4b94      	ldr	r3, [pc, #592]	; (300071c0 <BOOT_Image1+0x278>)
30006f6e:	1a12      	subs	r2, r2, r0
30006f70:	4798      	blx	r3
30006f72:	4a94      	ldr	r2, [pc, #592]	; (300071c4 <BOOT_Image1+0x27c>)
30006f74:	4994      	ldr	r1, [pc, #592]	; (300071c8 <BOOT_Image1+0x280>)
30006f76:	8c13      	ldrh	r3, [r2, #32]
30006f78:	b29b      	uxth	r3, r3
30006f7a:	8413      	strh	r3, [r2, #32]
30006f7c:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006f80:	f002 fd20 	bl	300099c4 <BOOT_Reason>
30006f84:	2800      	cmp	r0, #0
30006f86:	f000 8371 	beq.w	3000766c <BOOT_Image1+0x724>
30006f8a:	4e90      	ldr	r6, [pc, #576]	; (300071cc <BOOT_Image1+0x284>)
30006f8c:	4b90      	ldr	r3, [pc, #576]	; (300071d0 <BOOT_Image1+0x288>)
30006f8e:	4798      	blx	r3
30006f90:	7834      	ldrb	r4, [r6, #0]
30006f92:	2c00      	cmp	r4, #0
30006f94:	f000 832d 	beq.w	300075f2 <BOOT_Image1+0x6aa>
30006f98:	2000      	movs	r0, #0
30006f9a:	4b8e      	ldr	r3, [pc, #568]	; (300071d4 <BOOT_Image1+0x28c>)
30006f9c:	4798      	blx	r3
30006f9e:	4b8e      	ldr	r3, [pc, #568]	; (300071d8 <BOOT_Image1+0x290>)
30006fa0:	2001      	movs	r0, #1
30006fa2:	4798      	blx	r3
30006fa4:	4d88      	ldr	r5, [pc, #544]	; (300071c8 <BOOT_Image1+0x280>)
30006fa6:	f7ff fc9f 	bl	300068e8 <BOOT_SOC_ClkSet>
30006faa:	f241 070a 	movw	r7, #4106	; 0x100a
30006fae:	2028      	movs	r0, #40	; 0x28
30006fb0:	882b      	ldrh	r3, [r5, #0]
30006fb2:	4c8a      	ldr	r4, [pc, #552]	; (300071dc <BOOT_Image1+0x294>)
30006fb4:	b29b      	uxth	r3, r3
30006fb6:	431f      	orrs	r7, r3
30006fb8:	4b89      	ldr	r3, [pc, #548]	; (300071e0 <BOOT_Image1+0x298>)
30006fba:	802f      	strh	r7, [r5, #0]
30006fbc:	4798      	blx	r3
30006fbe:	88ab      	ldrh	r3, [r5, #4]
30006fc0:	b29b      	uxth	r3, r3
30006fc2:	ea23 0307 	bic.w	r3, r3, r7
30006fc6:	80ab      	strh	r3, [r5, #4]
30006fc8:	886b      	ldrh	r3, [r5, #2]
30006fca:	806f      	strh	r7, [r5, #2]
30006fcc:	47a0      	blx	r4
30006fce:	2802      	cmp	r0, #2
30006fd0:	f000 8319 	beq.w	30007606 <BOOT_Image1+0x6be>
30006fd4:	4b83      	ldr	r3, [pc, #524]	; (300071e4 <BOOT_Image1+0x29c>)
30006fd6:	2501      	movs	r5, #1
30006fd8:	4983      	ldr	r1, [pc, #524]	; (300071e8 <BOOT_Image1+0x2a0>)
30006fda:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006fde:	f883 510e 	strb.w	r5, [r3, #270]	; 0x10e
30006fe2:	462a      	mov	r2, r5
30006fe4:	4f81      	ldr	r7, [pc, #516]	; (300071ec <BOOT_Image1+0x2a4>)
30006fe6:	47b8      	blx	r7
30006fe8:	462a      	mov	r2, r5
30006fea:	4981      	ldr	r1, [pc, #516]	; (300071f0 <BOOT_Image1+0x2a8>)
30006fec:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006ff0:	47b8      	blx	r7
30006ff2:	4a74      	ldr	r2, [pc, #464]	; (300071c4 <BOOT_Image1+0x27c>)
30006ff4:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006ff8:	f043 0303 	orr.w	r3, r3, #3
30006ffc:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30007000:	4b7c      	ldr	r3, [pc, #496]	; (300071f4 <BOOT_Image1+0x2ac>)
30007002:	4798      	blx	r3
30007004:	4a6f      	ldr	r2, [pc, #444]	; (300071c4 <BOOT_Image1+0x27c>)
30007006:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
3000700a:	f023 0303 	bic.w	r3, r3, #3
3000700e:	b108      	cbz	r0, 30007014 <BOOT_Image1+0xcc>
30007010:	f043 0302 	orr.w	r3, r3, #2
30007014:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30007018:	4b77      	ldr	r3, [pc, #476]	; (300071f8 <BOOT_Image1+0x2b0>)
3000701a:	781b      	ldrb	r3, [r3, #0]
3000701c:	2b00      	cmp	r3, #0
3000701e:	f040 8336 	bne.w	3000768e <BOOT_Image1+0x746>
30007022:	4d76      	ldr	r5, [pc, #472]	; (300071fc <BOOT_Image1+0x2b4>)
30007024:	2102      	movs	r1, #2
30007026:	2201      	movs	r2, #1
30007028:	4875      	ldr	r0, [pc, #468]	; (30007200 <BOOT_Image1+0x2b8>)
3000702a:	47a8      	blx	r5
3000702c:	4d75      	ldr	r5, [pc, #468]	; (30007204 <BOOT_Image1+0x2bc>)
3000702e:	68ab      	ldr	r3, [r5, #8]
30007030:	0799      	lsls	r1, r3, #30
30007032:	f100 8326 	bmi.w	30007682 <BOOT_Image1+0x73a>
30007036:	4974      	ldr	r1, [pc, #464]	; (30007208 <BOOT_Image1+0x2c0>)
30007038:	e841 f100 	tt	r1, r1
3000703c:	68ab      	ldr	r3, [r5, #8]
3000703e:	f3c1 5180 	ubfx	r1, r1, #22, #1
30007042:	079a      	lsls	r2, r3, #30
30007044:	f100 8319 	bmi.w	3000767a <BOOT_Image1+0x732>
30007048:	4b70      	ldr	r3, [pc, #448]	; (3000720c <BOOT_Image1+0x2c4>)
3000704a:	4f71      	ldr	r7, [pc, #452]	; (30007210 <BOOT_Image1+0x2c8>)
3000704c:	681b      	ldr	r3, [r3, #0]
3000704e:	4798      	blx	r3
30007050:	f7fe fa50 	bl	300054f4 <BOOT_RccConfig>
30007054:	f7fe faba 	bl	300055cc <BOOT_CACHEWRR_Set>
30007058:	4b6e      	ldr	r3, [pc, #440]	; (30007214 <BOOT_Image1+0x2cc>)
3000705a:	6818      	ldr	r0, [r3, #0]
3000705c:	f7fe fae4 	bl	30005628 <BOOT_TCMSet>
30007060:	4b6d      	ldr	r3, [pc, #436]	; (30007218 <BOOT_Image1+0x2d0>)
30007062:	695b      	ldr	r3, [r3, #20]
30007064:	03db      	lsls	r3, r3, #15
30007066:	d513      	bpl.n	30007090 <BOOT_Image1+0x148>
30007068:	f017 011f 	ands.w	r1, r7, #31
3000706c:	f040 8265 	bne.w	3000753a <BOOT_Image1+0x5f2>
30007070:	463b      	mov	r3, r7
30007072:	2174      	movs	r1, #116	; 0x74
30007074:	f3bf 8f4f 	dsb	sy
30007078:	4867      	ldr	r0, [pc, #412]	; (30007218 <BOOT_Image1+0x2d0>)
3000707a:	4419      	add	r1, r3
3000707c:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30007080:	3320      	adds	r3, #32
30007082:	1aca      	subs	r2, r1, r3
30007084:	2a00      	cmp	r2, #0
30007086:	dcf9      	bgt.n	3000707c <BOOT_Image1+0x134>
30007088:	f3bf 8f4f 	dsb	sy
3000708c:	f3bf 8f6f 	isb	sy
30007090:	4b4c      	ldr	r3, [pc, #304]	; (300071c4 <BOOT_Image1+0x27c>)
30007092:	2200      	movs	r2, #0
30007094:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30007098:	4b52      	ldr	r3, [pc, #328]	; (300071e4 <BOOT_Image1+0x29c>)
3000709a:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000709e:	f7fc fb9f 	bl	300037e0 <flash_highspeed_setup>
300070a2:	f001 fc27 	bl	300088f4 <flash_layout_init>
300070a6:	47a0      	blx	r4
300070a8:	2802      	cmp	r0, #2
300070aa:	f000 82c1 	beq.w	30007630 <BOOT_Image1+0x6e8>
300070ae:	7832      	ldrb	r2, [r6, #0]
300070b0:	68ab      	ldr	r3, [r5, #8]
300070b2:	b162      	cbz	r2, 300070ce <BOOT_Image1+0x186>
300070b4:	4c59      	ldr	r4, [pc, #356]	; (3000721c <BOOT_Image1+0x2d4>)
300070b6:	4e5a      	ldr	r6, [pc, #360]	; (30007220 <BOOT_Image1+0x2d8>)
300070b8:	f013 0f02 	tst.w	r3, #2
300070bc:	4630      	mov	r0, r6
300070be:	d001      	beq.n	300070c4 <BOOT_Image1+0x17c>
300070c0:	f005 fb26 	bl	3000c710 <__DiagPrintf_veneer>
300070c4:	f241 3088 	movw	r0, #5000	; 0x1388
300070c8:	47a0      	blx	r4
300070ca:	68ab      	ldr	r3, [r5, #8]
300070cc:	e7f4      	b.n	300070b8 <BOOT_Image1+0x170>
300070ce:	079a      	lsls	r2, r3, #30
300070d0:	f100 8364 	bmi.w	3000779c <BOOT_Image1+0x854>
300070d4:	f7fe fc40 	bl	30005958 <BOOT_DDR_Init>
300070d8:	4a3a      	ldr	r2, [pc, #232]	; (300071c4 <BOOT_Image1+0x27c>)
300070da:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300070de:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300070e2:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300070e6:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300070ea:	2400      	movs	r4, #0
300070ec:	4b35      	ldr	r3, [pc, #212]	; (300071c4 <BOOT_Image1+0x27c>)
300070ee:	f8c3 42d0 	str.w	r4, [r3, #720]	; 0x2d0
300070f2:	f8c3 42d4 	str.w	r4, [r3, #724]	; 0x2d4
300070f6:	f8c3 42d8 	str.w	r4, [r3, #728]	; 0x2d8
300070fa:	f8c3 40d0 	str.w	r4, [r3, #208]	; 0xd0
300070fe:	f7ff fee5 	bl	30006ecc <BOOT_Share_Memory_Patch>
30007102:	2307      	movs	r3, #7
30007104:	f10d 021d 	add.w	r2, sp, #29
30007108:	f44f 7159 	mov.w	r1, #868	; 0x364
3000710c:	4620      	mov	r0, r4
3000710e:	f001 fff1 	bl	300090f4 <EFUSERead8>
30007112:	f10d 011f 	add.w	r1, sp, #31
30007116:	f44f 705a 	mov.w	r0, #872	; 0x368
3000711a:	f88d 401e 	strb.w	r4, [sp, #30]
3000711e:	f88d 401f 	strb.w	r4, [sp, #31]
30007122:	f001 ff15 	bl	30008f50 <OTP_Read8>
30007126:	f10d 011e 	add.w	r1, sp, #30
3000712a:	f240 3069 	movw	r0, #873	; 0x369
3000712e:	f001 ff0f 	bl	30008f50 <OTP_Read8>
30007132:	f89d 301d 	ldrb.w	r3, [sp, #29]
30007136:	f013 0f1e 	tst.w	r3, #30
3000713a:	d10a      	bne.n	30007152 <BOOT_Image1+0x20a>
3000713c:	f89d 201e 	ldrb.w	r2, [sp, #30]
30007140:	f89d 301f 	ldrb.w	r3, [sp, #31]
30007144:	f002 0208 	and.w	r2, r2, #8
30007148:	f003 0301 	and.w	r3, r3, #1
3000714c:	4313      	orrs	r3, r2
3000714e:	f040 8221 	bne.w	30007594 <BOOT_Image1+0x64c>
30007152:	4b34      	ldr	r3, [pc, #208]	; (30007224 <BOOT_Image1+0x2dc>)
30007154:	781b      	ldrb	r3, [r3, #0]
30007156:	2b00      	cmp	r3, #0
30007158:	f040 823d 	bne.w	300075d6 <BOOT_Image1+0x68e>
3000715c:	682b      	ldr	r3, [r5, #0]
3000715e:	f043 0302 	orr.w	r3, r3, #2
30007162:	602b      	str	r3, [r5, #0]
30007164:	4b30      	ldr	r3, [pc, #192]	; (30007228 <BOOT_Image1+0x2e0>)
30007166:	4798      	blx	r3
30007168:	2801      	cmp	r0, #1
3000716a:	f04f 0100 	mov.w	r1, #0
3000716e:	f04f 0002 	mov.w	r0, #2
30007172:	bf0c      	ite	eq
30007174:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30007178:	4a2c      	ldrne	r2, [pc, #176]	; (3000722c <BOOT_Image1+0x2e4>)
3000717a:	f000 fdcf 	bl	30007d1c <ymodem_uart_port_init>
3000717e:	4b2a      	ldr	r3, [pc, #168]	; (30007228 <BOOT_Image1+0x2e0>)
30007180:	4798      	blx	r3
30007182:	2800      	cmp	r0, #0
30007184:	f000 826d 	beq.w	30007662 <BOOT_Image1+0x71a>
30007188:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
3000718c:	4828      	ldr	r0, [pc, #160]	; (30007230 <BOOT_Image1+0x2e8>)
3000718e:	f005 fabf 	bl	3000c710 <__DiagPrintf_veneer>
30007192:	4828      	ldr	r0, [pc, #160]	; (30007234 <BOOT_Image1+0x2ec>)
30007194:	2664      	movs	r6, #100	; 0x64
30007196:	f005 fa73 	bl	3000c680 <____wrap_strlen_veneer>
3000719a:	4601      	mov	r1, r0
3000719c:	4825      	ldr	r0, [pc, #148]	; (30007234 <BOOT_Image1+0x2ec>)
3000719e:	4c1f      	ldr	r4, [pc, #124]	; (3000721c <BOOT_Image1+0x2d4>)
300071a0:	f000 fe16 	bl	30007dd0 <ymodem_uart_putdata>
300071a4:	e04b      	b.n	3000723e <BOOT_Image1+0x2f6>
300071a6:	bf00      	nop
300071a8:	0000c149 	.word	0x0000c149
300071ac:	3000d41c 	.word	0x3000d41c
300071b0:	3000e8f4 	.word	0x3000e8f4
300071b4:	3000d440 	.word	0x3000d440
300071b8:	30010160 	.word	0x30010160
300071bc:	30010630 	.word	0x30010630
300071c0:	00012be5 	.word	0x00012be5
300071c4:	42008000 	.word	0x42008000
300071c8:	42008200 	.word	0x42008200
300071cc:	3000ebd9 	.word	0x3000ebd9
300071d0:	3000b8d1 	.word	0x3000b8d1
300071d4:	3000b741 	.word	0x3000b741
300071d8:	3000b661 	.word	0x3000b661
300071dc:	30007fcd 	.word	0x30007fcd
300071e0:	00009b2d 	.word	0x00009b2d
300071e4:	23020000 	.word	0x23020000
300071e8:	40080000 	.word	0x40080000
300071ec:	0000b479 	.word	0x0000b479
300071f0:	40040000 	.word	0x40040000
300071f4:	3000a261 	.word	0x3000a261
300071f8:	3000ebd8 	.word	0x3000ebd8
300071fc:	0000aab9 	.word	0x0000aab9
30007200:	4200c000 	.word	0x4200c000
30007204:	2001c00c 	.word	0x2001c00c
30007208:	30006f49 	.word	0x30006f49
3000720c:	00033000 	.word	0x00033000
30007210:	2001c01c 	.word	0x2001c01c
30007214:	3000ebdc 	.word	0x3000ebdc
30007218:	e000ed00 	.word	0xe000ed00
3000721c:	00009be5 	.word	0x00009be5
30007220:	3000d4f0 	.word	0x3000d4f0
30007224:	3000ebed 	.word	0x3000ebed
30007228:	0000c169 	.word	0x0000c169
3000722c:	0016e360 	.word	0x0016e360
30007230:	3000d5c0 	.word	0x3000d5c0
30007234:	3000f2a8 	.word	0x3000f2a8
30007238:	3e01      	subs	r6, #1
3000723a:	f000 82b3 	beq.w	300077a4 <BOOT_Image1+0x85c>
3000723e:	2001      	movs	r0, #1
30007240:	47a0      	blx	r4
30007242:	f000 fdab 	bl	30007d9c <ymodem_uart_readable>
30007246:	2800      	cmp	r0, #0
30007248:	d0f6      	beq.n	30007238 <BOOT_Image1+0x2f0>
3000724a:	f000 fda7 	bl	30007d9c <ymodem_uart_readable>
3000724e:	2800      	cmp	r0, #0
30007250:	f000 82f8 	beq.w	30007844 <BOOT_Image1+0x8fc>
30007254:	2301      	movs	r3, #1
30007256:	9303      	str	r3, [sp, #12]
30007258:	2001      	movs	r0, #1
3000725a:	47a0      	blx	r4
3000725c:	2108      	movs	r1, #8
3000725e:	48b9      	ldr	r0, [pc, #740]	; (30007544 <BOOT_Image1+0x5fc>)
30007260:	f000 fdc4 	bl	30007dec <ymodem_uart_getdata>
30007264:	49b7      	ldr	r1, [pc, #732]	; (30007544 <BOOT_Image1+0x5fc>)
30007266:	2208      	movs	r2, #8
30007268:	4bb7      	ldr	r3, [pc, #732]	; (30007548 <BOOT_Image1+0x600>)
3000726a:	f101 0080 	add.w	r0, r1, #128	; 0x80
3000726e:	4798      	blx	r3
30007270:	9004      	str	r0, [sp, #16]
30007272:	2800      	cmp	r0, #0
30007274:	f040 835c 	bne.w	30007930 <BOOT_Image1+0x9e8>
30007278:	f000 fd90 	bl	30007d9c <ymodem_uart_readable>
3000727c:	2800      	cmp	r0, #0
3000727e:	d0fb      	beq.n	30007278 <BOOT_Image1+0x330>
30007280:	2101      	movs	r1, #1
30007282:	48b2      	ldr	r0, [pc, #712]	; (3000754c <BOOT_Image1+0x604>)
30007284:	f000 fdb2 	bl	30007dec <ymodem_uart_getdata>
30007288:	f000 fd88 	bl	30007d9c <ymodem_uart_readable>
3000728c:	2800      	cmp	r0, #0
3000728e:	d0fb      	beq.n	30007288 <BOOT_Image1+0x340>
30007290:	4eaf      	ldr	r6, [pc, #700]	; (30007550 <BOOT_Image1+0x608>)
30007292:	2101      	movs	r1, #1
30007294:	f206 4051 	addw	r0, r6, #1105	; 0x451
30007298:	f000 fda8 	bl	30007dec <ymodem_uart_getdata>
3000729c:	f896 2451 	ldrb.w	r2, [r6, #1105]	; 0x451
300072a0:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300072a4:	429a      	cmp	r2, r3
300072a6:	f000 834e 	beq.w	30007946 <BOOT_Image1+0x9fe>
300072aa:	f206 4952 	addw	r9, r6, #1106	; 0x452
300072ae:	2700      	movs	r7, #0
300072b0:	46b8      	mov	r8, r7
300072b2:	3701      	adds	r7, #1
300072b4:	f000 fd72 	bl	30007d9c <ymodem_uart_readable>
300072b8:	2800      	cmp	r0, #0
300072ba:	d0fb      	beq.n	300072b4 <BOOT_Image1+0x36c>
300072bc:	4648      	mov	r0, r9
300072be:	2101      	movs	r1, #1
300072c0:	f000 fd94 	bl	30007dec <ymodem_uart_getdata>
300072c4:	f899 2000 	ldrb.w	r2, [r9]
300072c8:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300072cc:	f108 080b 	add.w	r8, r8, #11
300072d0:	f109 0901 	add.w	r9, r9, #1
300072d4:	46bb      	mov	fp, r7
300072d6:	429a      	cmp	r2, r3
300072d8:	d1ea      	bne.n	300072b0 <BOOT_Image1+0x368>
300072da:	2f00      	cmp	r7, #0
300072dc:	dd13      	ble.n	30007306 <BOOT_Image1+0x3be>
300072de:	f506 618a 	add.w	r1, r6, #1104	; 0x450
300072e2:	2300      	movs	r3, #0
300072e4:	19c8      	adds	r0, r1, r7
300072e6:	e001      	b.n	300072ec <BOOT_Image1+0x3a4>
300072e8:	4288      	cmp	r0, r1
300072ea:	d00d      	beq.n	30007308 <BOOT_Image1+0x3c0>
300072ec:	f811 2f01 	ldrb.w	r2, [r1, #1]!
300072f0:	eb03 0383 	add.w	r3, r3, r3, lsl #2
300072f4:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
300072f8:	eb02 0343 	add.w	r3, r2, r3, lsl #1
300072fc:	f1bc 0f09 	cmp.w	ip, #9
30007300:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30007304:	d9f0      	bls.n	300072e8 <BOOT_Image1+0x3a0>
30007306:	2300      	movs	r3, #0
30007308:	f8c6 34d4 	str.w	r3, [r6, #1236]	; 0x4d4
3000730c:	f000 fd46 	bl	30007d9c <ymodem_uart_readable>
30007310:	2800      	cmp	r0, #0
30007312:	d0fb      	beq.n	3000730c <BOOT_Image1+0x3c4>
30007314:	498b      	ldr	r1, [pc, #556]	; (30007544 <BOOT_Image1+0x5fc>)
30007316:	460b      	mov	r3, r1
30007318:	2101      	movs	r1, #1
3000731a:	eb03 0008 	add.w	r0, r3, r8
3000731e:	9302      	str	r3, [sp, #8]
30007320:	f000 fd64 	bl	30007dec <ymodem_uart_getdata>
30007324:	9902      	ldr	r1, [sp, #8]
30007326:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
3000732a:	f811 2008 	ldrb.w	r2, [r1, r8]
3000732e:	429a      	cmp	r2, r3
30007330:	f000 8311 	beq.w	30007956 <BOOT_Image1+0xa0e>
30007334:	f10b 080b 	add.w	r8, fp, #11
30007338:	f04f 0900 	mov.w	r9, #0
3000733c:	4488      	add	r8, r1
3000733e:	f109 0901 	add.w	r9, r9, #1
30007342:	f000 fd2b 	bl	30007d9c <ymodem_uart_readable>
30007346:	2800      	cmp	r0, #0
30007348:	d0fb      	beq.n	30007342 <BOOT_Image1+0x3fa>
3000734a:	4640      	mov	r0, r8
3000734c:	2101      	movs	r1, #1
3000734e:	f000 fd4d 	bl	30007dec <ymodem_uart_getdata>
30007352:	f898 2000 	ldrb.w	r2, [r8]
30007356:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
3000735a:	f108 0801 	add.w	r8, r8, #1
3000735e:	eb09 0a0b 	add.w	sl, r9, fp
30007362:	429a      	cmp	r2, r3
30007364:	d1eb      	bne.n	3000733e <BOOT_Image1+0x3f6>
30007366:	f000 fd19 	bl	30007d9c <ymodem_uart_readable>
3000736a:	2800      	cmp	r0, #0
3000736c:	d0fb      	beq.n	30007366 <BOOT_Image1+0x41e>
3000736e:	9b02      	ldr	r3, [sp, #8]
30007370:	f10a 080b 	add.w	r8, sl, #11
30007374:	2101      	movs	r1, #1
30007376:	eb03 0008 	add.w	r0, r3, r8
3000737a:	f000 fd37 	bl	30007dec <ymodem_uart_getdata>
3000737e:	9902      	ldr	r1, [sp, #8]
30007380:	f896 24d1 	ldrb.w	r2, [r6, #1233]	; 0x4d1
30007384:	f811 3008 	ldrb.w	r3, [r1, r8]
30007388:	429a      	cmp	r2, r3
3000738a:	f000 82e2 	beq.w	30007952 <BOOT_Image1+0xa0a>
3000738e:	f10a 080c 	add.w	r8, sl, #12
30007392:	46d3      	mov	fp, sl
30007394:	f8dd a010 	ldr.w	sl, [sp, #16]
30007398:	4488      	add	r8, r1
3000739a:	9405      	str	r4, [sp, #20]
3000739c:	f10a 0a01 	add.w	sl, sl, #1
300073a0:	f000 fcfc 	bl	30007d9c <ymodem_uart_readable>
300073a4:	2800      	cmp	r0, #0
300073a6:	d0fb      	beq.n	300073a0 <BOOT_Image1+0x458>
300073a8:	4640      	mov	r0, r8
300073aa:	2101      	movs	r1, #1
300073ac:	f000 fd1e 	bl	30007dec <ymodem_uart_getdata>
300073b0:	f898 2000 	ldrb.w	r2, [r8]
300073b4:	f896 34d1 	ldrb.w	r3, [r6, #1233]	; 0x4d1
300073b8:	f108 0801 	add.w	r8, r8, #1
300073bc:	eb0a 010b 	add.w	r1, sl, fp
300073c0:	429a      	cmp	r2, r3
300073c2:	d1eb      	bne.n	3000739c <BOOT_Image1+0x454>
300073c4:	468b      	mov	fp, r1
300073c6:	9c05      	ldr	r4, [sp, #20]
300073c8:	f8cd a010 	str.w	sl, [sp, #16]
300073cc:	9b02      	ldr	r3, [sp, #8]
300073ce:	f10b 080c 	add.w	r8, fp, #12
300073d2:	2101      	movs	r1, #1
300073d4:	eb03 0008 	add.w	r0, r3, r8
300073d8:	f000 fd08 	bl	30007dec <ymodem_uart_getdata>
300073dc:	9b02      	ldr	r3, [sp, #8]
300073de:	f813 1008 	ldrb.w	r1, [r3, r8]
300073e2:	290a      	cmp	r1, #10
300073e4:	f8c6 14d8 	str.w	r1, [r6, #1240]	; 0x4d8
300073e8:	f200 829e 	bhi.w	30007928 <BOOT_Image1+0x9e0>
300073ec:	9b02      	ldr	r3, [sp, #8]
300073ee:	f10b 000d 	add.w	r0, fp, #13
300073f2:	2101      	movs	r1, #1
300073f4:	f10b 080e 	add.w	r8, fp, #14
300073f8:	4418      	add	r0, r3
300073fa:	f000 fcf7 	bl	30007dec <ymodem_uart_getdata>
300073fe:	9b02      	ldr	r3, [sp, #8]
30007400:	2102      	movs	r1, #2
30007402:	4498      	add	r8, r3
30007404:	4640      	mov	r0, r8
30007406:	f000 fcf1 	bl	30007dec <ymodem_uart_getdata>
3000740a:	4641      	mov	r1, r8
3000740c:	2202      	movs	r2, #2
3000740e:	4851      	ldr	r0, [pc, #324]	; (30007554 <BOOT_Image1+0x60c>)
30007410:	4b4d      	ldr	r3, [pc, #308]	; (30007548 <BOOT_Image1+0x600>)
30007412:	4798      	blx	r3
30007414:	2800      	cmp	r0, #0
30007416:	f040 8292 	bne.w	3000793e <BOOT_Image1+0x9f6>
3000741a:	9b02      	ldr	r3, [sp, #8]
3000741c:	f10b 0010 	add.w	r0, fp, #16
30007420:	2101      	movs	r1, #1
30007422:	f10b 0811 	add.w	r8, fp, #17
30007426:	4418      	add	r0, r3
30007428:	f000 fce0 	bl	30007dec <ymodem_uart_getdata>
3000742c:	4b4a      	ldr	r3, [pc, #296]	; (30007558 <BOOT_Image1+0x610>)
3000742e:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
30007432:	f103 0228 	add.w	r2, r3, #40	; 0x28
30007436:	f103 0130 	add.w	r1, r3, #48	; 0x30
3000743a:	f7fd fd2b 	bl	30004e94 <hotfix_get_addr_size>
3000743e:	9b02      	ldr	r3, [sp, #8]
30007440:	2102      	movs	r1, #2
30007442:	4498      	add	r8, r3
30007444:	4640      	mov	r0, r8
30007446:	f000 fcd1 	bl	30007dec <ymodem_uart_getdata>
3000744a:	4641      	mov	r1, r8
3000744c:	2202      	movs	r2, #2
3000744e:	4843      	ldr	r0, [pc, #268]	; (3000755c <BOOT_Image1+0x614>)
30007450:	4b3d      	ldr	r3, [pc, #244]	; (30007548 <BOOT_Image1+0x600>)
30007452:	4798      	blx	r3
30007454:	2800      	cmp	r0, #0
30007456:	f000 813b 	beq.w	300076d0 <BOOT_Image1+0x788>
3000745a:	9b03      	ldr	r3, [sp, #12]
3000745c:	2b00      	cmp	r3, #0
3000745e:	f000 8182 	beq.w	30007766 <BOOT_Image1+0x81e>
30007462:	f004 ff0d 	bl	3000c280 <BOOT_VerCheck>
30007466:	f7fe fc73 	bl	30005d50 <BOOT_LoadImages>
3000746a:	2800      	cmp	r0, #0
3000746c:	f000 8129 	beq.w	300076c2 <BOOT_Image1+0x77a>
30007470:	f7ff f9f6 	bl	30006860 <BOOT_Enable_KM0>
30007474:	2200      	movs	r2, #0
30007476:	2101      	movs	r1, #1
30007478:	4b39      	ldr	r3, [pc, #228]	; (30007560 <BOOT_Image1+0x618>)
3000747a:	483a      	ldr	r0, [pc, #232]	; (30007564 <BOOT_Image1+0x61c>)
3000747c:	4798      	blx	r3
3000747e:	4b3a      	ldr	r3, [pc, #232]	; (30007568 <BOOT_Image1+0x620>)
30007480:	2008      	movs	r0, #8
30007482:	4798      	blx	r3
30007484:	f7fd fc18 	bl	30004cb8 <BOOT_RAM_TZCfg>
30007488:	4b38      	ldr	r3, [pc, #224]	; (3000756c <BOOT_Image1+0x624>)
3000748a:	781b      	ldrb	r3, [r3, #0]
3000748c:	2b01      	cmp	r3, #1
3000748e:	f000 8243 	beq.w	30007918 <BOOT_Image1+0x9d0>
30007492:	4b37      	ldr	r3, [pc, #220]	; (30007570 <BOOT_Image1+0x628>)
30007494:	781b      	ldrb	r3, [r3, #0]
30007496:	2b00      	cmp	r3, #0
30007498:	f000 816c 	beq.w	30007774 <BOOT_Image1+0x82c>
3000749c:	f7ff fb18 	bl	30006ad0 <BOOT_Enable_AP>
300074a0:	4a34      	ldr	r2, [pc, #208]	; (30007574 <BOOT_Image1+0x62c>)
300074a2:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
300074a6:	f043 0306 	orr.w	r3, r3, #6
300074aa:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300074ae:	4b2f      	ldr	r3, [pc, #188]	; (3000756c <BOOT_Image1+0x624>)
300074b0:	781b      	ldrb	r3, [r3, #0]
300074b2:	2b01      	cmp	r3, #1
300074b4:	f000 8228 	beq.w	30007908 <BOOT_Image1+0x9c0>
300074b8:	4a2e      	ldr	r2, [pc, #184]	; (30007574 <BOOT_Image1+0x62c>)
300074ba:	492f      	ldr	r1, [pc, #188]	; (30007578 <BOOT_Image1+0x630>)
300074bc:	f892 3067 	ldrb.w	r3, [r2, #103]	; 0x67
300074c0:	688c      	ldr	r4, [r1, #8]
300074c2:	f043 0301 	orr.w	r3, r3, #1
300074c6:	482d      	ldr	r0, [pc, #180]	; (3000757c <BOOT_Image1+0x634>)
300074c8:	f882 3067 	strb.w	r3, [r2, #103]	; 0x67
300074cc:	680b      	ldr	r3, [r1, #0]
300074ce:	4a2c      	ldr	r2, [pc, #176]	; (30007580 <BOOT_Image1+0x638>)
300074d0:	6063      	str	r3, [r4, #4]
300074d2:	492c      	ldr	r1, [pc, #176]	; (30007584 <BOOT_Image1+0x63c>)
300074d4:	4b2c      	ldr	r3, [pc, #176]	; (30007588 <BOOT_Image1+0x640>)
300074d6:	6094      	str	r4, [r2, #8]
300074d8:	61c8      	str	r0, [r1, #28]
300074da:	6a59      	ldr	r1, [r3, #36]	; 0x24
300074dc:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300074e0:	6259      	str	r1, [r3, #36]	; 0x24
300074e2:	6a51      	ldr	r1, [r2, #36]	; 0x24
300074e4:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300074e8:	6251      	str	r1, [r2, #36]	; 0x24
300074ea:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300074ee:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300074f2:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300074f6:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300074fa:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300074fe:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30007502:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30007506:	4921      	ldr	r1, [pc, #132]	; (3000758c <BOOT_Image1+0x644>)
30007508:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
3000750c:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30007510:	689b      	ldr	r3, [r3, #8]
30007512:	618b      	str	r3, [r1, #24]
30007514:	f7fe fba4 	bl	30005c60 <BOOT_NVICBackup_HP>
30007518:	4b1d      	ldr	r3, [pc, #116]	; (30007590 <BOOT_Image1+0x648>)
3000751a:	f383 8888 	msr	MSP_NS, r3
3000751e:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30007522:	f383 8809 	msr	PSP, r3
30007526:	68ab      	ldr	r3, [r5, #8]
30007528:	079b      	lsls	r3, r3, #30
3000752a:	f100 8141 	bmi.w	300077b0 <BOOT_Image1+0x868>
3000752e:	6860      	ldr	r0, [r4, #4]
30007530:	f7fd ff90 	bl	30005454 <BOOT_NsStart>
30007534:	b00b      	add	sp, #44	; 0x2c
30007536:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000753a:	3174      	adds	r1, #116	; 0x74
3000753c:	f027 031f 	bic.w	r3, r7, #31
30007540:	e598      	b.n	30007074 <BOOT_Image1+0x12c>
30007542:	bf00      	nop
30007544:	3000f2c8 	.word	0x3000f2c8
30007548:	000129bd 	.word	0x000129bd
3000754c:	3000f2d0 	.word	0x3000f2d0
30007550:	3000ee80 	.word	0x3000ee80
30007554:	3000f35c 	.word	0x3000f35c
30007558:	3000f360 	.word	0x3000f360
3000755c:	3000f39c 	.word	0x3000f39c
30007560:	0000ac6d 	.word	0x0000ac6d
30007564:	4200c000 	.word	0x4200c000
30007568:	0000e1d9 	.word	0x0000e1d9
3000756c:	3000ebed 	.word	0x3000ebed
30007570:	3000ebda 	.word	0x3000ebda
30007574:	42008200 	.word	0x42008200
30007578:	60000020 	.word	0x60000020
3000757c:	30004e55 	.word	0x30004e55
30007580:	e002ed00 	.word	0xe002ed00
30007584:	30000000 	.word	0x30000000
30007588:	e000ed00 	.word	0xe000ed00
3000758c:	30010594 	.word	0x30010594
30007590:	2001bffc 	.word	0x2001bffc
30007594:	488b      	ldr	r0, [pc, #556]	; (300077c4 <BOOT_Image1+0x87c>)
30007596:	f005 f8bb 	bl	3000c710 <__DiagPrintf_veneer>
3000759a:	f89d 101f 	ldrb.w	r1, [sp, #31]
3000759e:	f89d 301e 	ldrb.w	r3, [sp, #30]
300075a2:	f44f 705a 	mov.w	r0, #872	; 0x368
300075a6:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
300075aa:	f023 0308 	bic.w	r3, r3, #8
300075ae:	f88d 101f 	strb.w	r1, [sp, #31]
300075b2:	f88d 301e 	strb.w	r3, [sp, #30]
300075b6:	f001 fcd5 	bl	30008f64 <OTP_Write8>
300075ba:	f89d 101e 	ldrb.w	r1, [sp, #30]
300075be:	f240 3069 	movw	r0, #873	; 0x369
300075c2:	f001 fccf 	bl	30008f64 <OTP_Write8>
300075c6:	4880      	ldr	r0, [pc, #512]	; (300077c8 <BOOT_Image1+0x880>)
300075c8:	f005 f8a2 	bl	3000c710 <__DiagPrintf_veneer>
300075cc:	4b7f      	ldr	r3, [pc, #508]	; (300077cc <BOOT_Image1+0x884>)
300075ce:	781b      	ldrb	r3, [r3, #0]
300075d0:	2b00      	cmp	r3, #0
300075d2:	f43f adc3 	beq.w	3000715c <BOOT_Image1+0x214>
300075d6:	4b7e      	ldr	r3, [pc, #504]	; (300077d0 <BOOT_Image1+0x888>)
300075d8:	4798      	blx	r3
300075da:	2800      	cmp	r0, #0
300075dc:	f47f adbe 	bne.w	3000715c <BOOT_Image1+0x214>
300075e0:	68ab      	ldr	r3, [r5, #8]
300075e2:	f043 0302 	orr.w	r3, r3, #2
300075e6:	60ab      	str	r3, [r5, #8]
300075e8:	e5b8      	b.n	3000715c <BOOT_Image1+0x214>
300075ea:	487a      	ldr	r0, [pc, #488]	; (300077d4 <BOOT_Image1+0x88c>)
300075ec:	f005 f890 	bl	3000c710 <__DiagPrintf_veneer>
300075f0:	e4b5      	b.n	30006f5e <BOOT_Image1+0x16>
300075f2:	4b79      	ldr	r3, [pc, #484]	; (300077d8 <BOOT_Image1+0x890>)
300075f4:	2001      	movs	r0, #1
300075f6:	4798      	blx	r3
300075f8:	4620      	mov	r0, r4
300075fa:	4b78      	ldr	r3, [pc, #480]	; (300077dc <BOOT_Image1+0x894>)
300075fc:	4798      	blx	r3
300075fe:	4b78      	ldr	r3, [pc, #480]	; (300077e0 <BOOT_Image1+0x898>)
30007600:	2001      	movs	r0, #1
30007602:	4798      	blx	r3
30007604:	e4ce      	b.n	30006fa4 <BOOT_Image1+0x5c>
30007606:	6deb      	ldr	r3, [r5, #92]	; 0x5c
30007608:	2201      	movs	r2, #1
3000760a:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000760e:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30007612:	65eb      	str	r3, [r5, #92]	; 0x5c
30007614:	4b73      	ldr	r3, [pc, #460]	; (300077e4 <BOOT_Image1+0x89c>)
30007616:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
3000761a:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000761e:	4b72      	ldr	r3, [pc, #456]	; (300077e8 <BOOT_Image1+0x8a0>)
30007620:	4798      	blx	r3
30007622:	f8d5 30fc 	ldr.w	r3, [r5, #252]	; 0xfc
30007626:	f043 0302 	orr.w	r3, r3, #2
3000762a:	f8c5 30fc 	str.w	r3, [r5, #252]	; 0xfc
3000762e:	e4e7      	b.n	30007000 <BOOT_Image1+0xb8>
30007630:	68ab      	ldr	r3, [r5, #8]
30007632:	0798      	lsls	r0, r3, #30
30007634:	f100 80ae 	bmi.w	30007794 <BOOT_Image1+0x84c>
30007638:	4b6c      	ldr	r3, [pc, #432]	; (300077ec <BOOT_Image1+0x8a4>)
3000763a:	4798      	blx	r3
3000763c:	f7fe f9ca 	bl	300059d4 <BOOT_PSRAM_Init>
30007640:	7830      	ldrb	r0, [r6, #0]
30007642:	2800      	cmp	r0, #0
30007644:	f47f ad51 	bne.w	300070ea <BOOT_Image1+0x1a2>
30007648:	4b69      	ldr	r3, [pc, #420]	; (300077f0 <BOOT_Image1+0x8a8>)
3000764a:	4798      	blx	r3
3000764c:	2800      	cmp	r0, #0
3000764e:	f43f ad4c 	beq.w	300070ea <BOOT_Image1+0x1a2>
30007652:	682b      	ldr	r3, [r5, #0]
30007654:	0399      	lsls	r1, r3, #14
30007656:	f57f ad48 	bpl.w	300070ea <BOOT_Image1+0x1a2>
3000765a:	4866      	ldr	r0, [pc, #408]	; (300077f4 <BOOT_Image1+0x8ac>)
3000765c:	f005 f858 	bl	3000c710 <__DiagPrintf_veneer>
30007660:	e543      	b.n	300070ea <BOOT_Image1+0x1a2>
30007662:	4965      	ldr	r1, [pc, #404]	; (300077f8 <BOOT_Image1+0x8b0>)
30007664:	4865      	ldr	r0, [pc, #404]	; (300077fc <BOOT_Image1+0x8b4>)
30007666:	f005 f853 	bl	3000c710 <__DiagPrintf_veneer>
3000766a:	e592      	b.n	30007192 <BOOT_Image1+0x24a>
3000766c:	4601      	mov	r1, r0
3000766e:	f44f 7201 	mov.w	r2, #516	; 0x204
30007672:	485c      	ldr	r0, [pc, #368]	; (300077e4 <BOOT_Image1+0x89c>)
30007674:	f005 f81c 	bl	3000c6b0 <____wrap_memset_veneer>
30007678:	e487      	b.n	30006f8a <BOOT_Image1+0x42>
3000767a:	4861      	ldr	r0, [pc, #388]	; (30007800 <BOOT_Image1+0x8b8>)
3000767c:	f005 f848 	bl	3000c710 <__DiagPrintf_veneer>
30007680:	e4e2      	b.n	30007048 <BOOT_Image1+0x100>
30007682:	f3ef 8108 	mrs	r1, MSP
30007686:	485f      	ldr	r0, [pc, #380]	; (30007804 <BOOT_Image1+0x8bc>)
30007688:	f005 f842 	bl	3000c710 <__DiagPrintf_veneer>
3000768c:	e4d3      	b.n	30007036 <BOOT_Image1+0xee>
3000768e:	4f5e      	ldr	r7, [pc, #376]	; (30007808 <BOOT_Image1+0x8c0>)
30007690:	47b8      	blx	r7
30007692:	4605      	mov	r5, r0
30007694:	47b8      	blx	r7
30007696:	2d02      	cmp	r5, #2
30007698:	f43f acc3 	beq.w	30007022 <BOOT_Image1+0xda>
3000769c:	2803      	cmp	r0, #3
3000769e:	f43f acc0 	beq.w	30007022 <BOOT_Image1+0xda>
300076a2:	4b5a      	ldr	r3, [pc, #360]	; (3000780c <BOOT_Image1+0x8c4>)
300076a4:	4798      	blx	r3
300076a6:	4d5a      	ldr	r5, [pc, #360]	; (30007810 <BOOT_Image1+0x8c8>)
300076a8:	2200      	movs	r2, #0
300076aa:	2101      	movs	r1, #1
300076ac:	4859      	ldr	r0, [pc, #356]	; (30007814 <BOOT_Image1+0x8cc>)
300076ae:	47a8      	blx	r5
300076b0:	2101      	movs	r1, #1
300076b2:	4858      	ldr	r0, [pc, #352]	; (30007814 <BOOT_Image1+0x8cc>)
300076b4:	4b58      	ldr	r3, [pc, #352]	; (30007818 <BOOT_Image1+0x8d0>)
300076b6:	4798      	blx	r3
300076b8:	2201      	movs	r2, #1
300076ba:	4856      	ldr	r0, [pc, #344]	; (30007814 <BOOT_Image1+0x8cc>)
300076bc:	4611      	mov	r1, r2
300076be:	47a8      	blx	r5
300076c0:	e4b0      	b.n	30007024 <BOOT_Image1+0xdc>
300076c2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300076c6:	47a0      	blx	r4
300076c8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300076cc:	47a0      	blx	r4
300076ce:	e7f8      	b.n	300076c2 <BOOT_Image1+0x77a>
300076d0:	9b02      	ldr	r3, [sp, #8]
300076d2:	f10b 0013 	add.w	r0, fp, #19
300076d6:	2101      	movs	r1, #1
300076d8:	f10b 0814 	add.w	r8, fp, #20
300076dc:	4418      	add	r0, r3
300076de:	f000 fb85 	bl	30007dec <ymodem_uart_getdata>
300076e2:	4b4e      	ldr	r3, [pc, #312]	; (3000781c <BOOT_Image1+0x8d4>)
300076e4:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
300076e8:	f1a3 0218 	sub.w	r2, r3, #24
300076ec:	f1a3 0110 	sub.w	r1, r3, #16
300076f0:	f7fd fbd0 	bl	30004e94 <hotfix_get_addr_size>
300076f4:	9b02      	ldr	r3, [sp, #8]
300076f6:	2102      	movs	r1, #2
300076f8:	4498      	add	r8, r3
300076fa:	4640      	mov	r0, r8
300076fc:	f000 fb76 	bl	30007dec <ymodem_uart_getdata>
30007700:	4641      	mov	r1, r8
30007702:	2202      	movs	r2, #2
30007704:	4846      	ldr	r0, [pc, #280]	; (30007820 <BOOT_Image1+0x8d8>)
30007706:	4b47      	ldr	r3, [pc, #284]	; (30007824 <BOOT_Image1+0x8dc>)
30007708:	4798      	blx	r3
3000770a:	2800      	cmp	r0, #0
3000770c:	f47f aea5 	bne.w	3000745a <BOOT_Image1+0x512>
30007710:	9b02      	ldr	r3, [sp, #8]
30007712:	f10b 0016 	add.w	r0, fp, #22
30007716:	2101      	movs	r1, #1
30007718:	f10b 0817 	add.w	r8, fp, #23
3000771c:	4418      	add	r0, r3
3000771e:	f000 fb65 	bl	30007dec <ymodem_uart_getdata>
30007722:	4b41      	ldr	r3, [pc, #260]	; (30007828 <BOOT_Image1+0x8e0>)
30007724:	f8d6 04d8 	ldr.w	r0, [r6, #1240]	; 0x4d8
30007728:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
3000772c:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30007730:	f7fd fbb0 	bl	30004e94 <hotfix_get_addr_size>
30007734:	9a02      	ldr	r2, [sp, #8]
30007736:	2102      	movs	r1, #2
30007738:	4490      	add	r8, r2
3000773a:	4640      	mov	r0, r8
3000773c:	f000 fb56 	bl	30007dec <ymodem_uart_getdata>
30007740:	4641      	mov	r1, r8
30007742:	2202      	movs	r2, #2
30007744:	4839      	ldr	r0, [pc, #228]	; (3000782c <BOOT_Image1+0x8e4>)
30007746:	4b37      	ldr	r3, [pc, #220]	; (30007824 <BOOT_Image1+0x8dc>)
30007748:	4798      	blx	r3
3000774a:	2800      	cmp	r0, #0
3000774c:	f47f ae85 	bne.w	3000745a <BOOT_Image1+0x512>
30007750:	9b02      	ldr	r3, [sp, #8]
30007752:	f10b 0019 	add.w	r0, fp, #25
30007756:	2101      	movs	r1, #1
30007758:	4418      	add	r0, r3
3000775a:	f000 fb47 	bl	30007dec <ymodem_uart_getdata>
3000775e:	9b03      	ldr	r3, [sp, #12]
30007760:	2b00      	cmp	r3, #0
30007762:	f040 80fc 	bne.w	3000795e <BOOT_Image1+0xa16>
30007766:	2002      	movs	r0, #2
30007768:	f000 faf0 	bl	30007d4c <ymodem_uart_port_deinit>
3000776c:	4830      	ldr	r0, [pc, #192]	; (30007830 <BOOT_Image1+0x8e8>)
3000776e:	f004 ffcf 	bl	3000c710 <__DiagPrintf_veneer>
30007772:	e676      	b.n	30007462 <BOOT_Image1+0x51a>
30007774:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30007778:	492e      	ldr	r1, [pc, #184]	; (30007834 <BOOT_Image1+0x8ec>)
3000777a:	685a      	ldr	r2, [r3, #4]
3000777c:	f042 0202 	orr.w	r2, r2, #2
30007780:	605a      	str	r2, [r3, #4]
30007782:	685a      	ldr	r2, [r3, #4]
30007784:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30007788:	605a      	str	r2, [r3, #4]
3000778a:	6f8b      	ldr	r3, [r1, #120]	; 0x78
3000778c:	f043 0303 	orr.w	r3, r3, #3
30007790:	678b      	str	r3, [r1, #120]	; 0x78
30007792:	e68c      	b.n	300074ae <BOOT_Image1+0x566>
30007794:	4828      	ldr	r0, [pc, #160]	; (30007838 <BOOT_Image1+0x8f0>)
30007796:	f004 ffbb 	bl	3000c710 <__DiagPrintf_veneer>
3000779a:	e74d      	b.n	30007638 <BOOT_Image1+0x6f0>
3000779c:	4827      	ldr	r0, [pc, #156]	; (3000783c <BOOT_Image1+0x8f4>)
3000779e:	f004 ffb7 	bl	3000c710 <__DiagPrintf_veneer>
300077a2:	e497      	b.n	300070d4 <BOOT_Image1+0x18c>
300077a4:	f000 fafa 	bl	30007d9c <ymodem_uart_readable>
300077a8:	2800      	cmp	r0, #0
300077aa:	d0dc      	beq.n	30007766 <BOOT_Image1+0x81e>
300077ac:	9603      	str	r6, [sp, #12]
300077ae:	e553      	b.n	30007258 <BOOT_Image1+0x310>
300077b0:	6861      	ldr	r1, [r4, #4]
300077b2:	4823      	ldr	r0, [pc, #140]	; (30007840 <BOOT_Image1+0x8f8>)
300077b4:	f004 ffac 	bl	3000c710 <__DiagPrintf_veneer>
300077b8:	6860      	ldr	r0, [r4, #4]
300077ba:	f7fd fe4b 	bl	30005454 <BOOT_NsStart>
300077be:	b00b      	add	sp, #44	; 0x2c
300077c0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300077c4:	3000d564 	.word	0x3000d564
300077c8:	3000d59c 	.word	0x3000d59c
300077cc:	3000ebed 	.word	0x3000ebed
300077d0:	0000c179 	.word	0x0000c179
300077d4:	3000d42c 	.word	0x3000d42c
300077d8:	3000b661 	.word	0x3000b661
300077dc:	3000b891 	.word	0x3000b891
300077e0:	3000b801 	.word	0x3000b801
300077e4:	23020000 	.word	0x23020000
300077e8:	0000b479 	.word	0x0000b479
300077ec:	3000922d 	.word	0x3000922d
300077f0:	3000b761 	.word	0x3000b761
300077f4:	3000d4e0 	.word	0x3000d4e0
300077f8:	0016e360 	.word	0x0016e360
300077fc:	3000d5c0 	.word	0x3000d5c0
30007800:	3000d488 	.word	0x3000d488
30007804:	3000d458 	.word	0x3000d458
30007808:	0000c0f9 	.word	0x0000c0f9
3000780c:	0000aea1 	.word	0x0000aea1
30007810:	0000aab9 	.word	0x0000aab9
30007814:	4200c000 	.word	0x4200c000
30007818:	0000aaa5 	.word	0x0000aaa5
3000781c:	3000f3a0 	.word	0x3000f3a0
30007820:	3000f3c8 	.word	0x3000f3c8
30007824:	000129bd 	.word	0x000129bd
30007828:	3000f3cc 	.word	0x3000f3cc
3000782c:	3000f3f4 	.word	0x3000f3f4
30007830:	3000d5f0 	.word	0x3000d5f0
30007834:	41000200 	.word	0x41000200
30007838:	3000d4b8 	.word	0x3000d4b8
3000783c:	3000d540 	.word	0x3000d540
30007840:	3000d648 	.word	0x3000d648
30007844:	4e49      	ldr	r6, [pc, #292]	; (3000796c <BOOT_Image1+0xa24>)
30007846:	4680      	mov	r8, r0
30007848:	f04f 0a0b 	mov.w	sl, #11
3000784c:	270a      	movs	r7, #10
3000784e:	f506 6389 	add.w	r3, r6, #1096	; 0x448
30007852:	4681      	mov	r9, r0
30007854:	9302      	str	r3, [sp, #8]
30007856:	2004      	movs	r0, #4
30007858:	f000 fa78 	bl	30007d4c <ymodem_uart_port_deinit>
3000785c:	9a02      	ldr	r2, [sp, #8]
3000785e:	f8cd 8000 	str.w	r8, [sp]
30007862:	4613      	mov	r3, r2
30007864:	19d0      	adds	r0, r2, r7
30007866:	f8d6 14d4 	ldr.w	r1, [r6, #1236]	; 0x4d4
3000786a:	464a      	mov	r2, r9
3000786c:	4453      	add	r3, sl
3000786e:	f7ff fa87 	bl	30006d80 <password_hash_check>
30007872:	4680      	mov	r8, r0
30007874:	2800      	cmp	r0, #0
30007876:	f47f af76 	bne.w	30007766 <BOOT_Image1+0x81e>
3000787a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000787e:	47a0      	blx	r4
30007880:	483b      	ldr	r0, [pc, #236]	; (30007970 <BOOT_Image1+0xa28>)
30007882:	f004 fefd 	bl	3000c680 <____wrap_strlen_veneer>
30007886:	4601      	mov	r1, r0
30007888:	4839      	ldr	r0, [pc, #228]	; (30007970 <BOOT_Image1+0xa28>)
3000788a:	f000 faa1 	bl	30007dd0 <ymodem_uart_putdata>
3000788e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007892:	47a0      	blx	r4
30007894:	f8d6 24d8 	ldr.w	r2, [r6, #1240]	; 0x4d8
30007898:	4936      	ldr	r1, [pc, #216]	; (30007974 <BOOT_Image1+0xa2c>)
3000789a:	2002      	movs	r0, #2
3000789c:	f000 fb38 	bl	30007f10 <ymodem_img_download>
300078a0:	2032      	movs	r0, #50	; 0x32
300078a2:	47a0      	blx	r4
300078a4:	f8d6 34d8 	ldr.w	r3, [r6, #1240]	; 0x4d8
300078a8:	b1e3      	cbz	r3, 300078e4 <BOOT_Image1+0x99c>
300078aa:	f8df 90f4 	ldr.w	r9, [pc, #244]	; 300079a0 <BOOT_Image1+0xa58>
300078ae:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
300078b2:	f859 7b04 	ldr.w	r7, [r9], #4
300078b6:	0b3f      	lsrs	r7, r7, #12
300078b8:	d00c      	beq.n	300078d4 <BOOT_Image1+0x98c>
300078ba:	2500      	movs	r5, #0
300078bc:	f8da 1000 	ldr.w	r1, [sl]
300078c0:	2002      	movs	r0, #2
300078c2:	eb01 3105 	add.w	r1, r1, r5, lsl #12
300078c6:	3501      	adds	r5, #1
300078c8:	f7fd fbea 	bl	300050a0 <FLASH_Erase_With_Lock>
300078cc:	2032      	movs	r0, #50	; 0x32
300078ce:	47a0      	blx	r4
300078d0:	42af      	cmp	r7, r5
300078d2:	d1f3      	bne.n	300078bc <BOOT_Image1+0x974>
300078d4:	f108 0801 	add.w	r8, r8, #1
300078d8:	f8d6 34d8 	ldr.w	r3, [r6, #1240]	; 0x4d8
300078dc:	f10a 0a04 	add.w	sl, sl, #4
300078e0:	4543      	cmp	r3, r8
300078e2:	d8e6      	bhi.n	300078b2 <BOOT_Image1+0x96a>
300078e4:	4824      	ldr	r0, [pc, #144]	; (30007978 <BOOT_Image1+0xa30>)
300078e6:	f004 ff13 	bl	3000c710 <__DiagPrintf_veneer>
300078ea:	4b24      	ldr	r3, [pc, #144]	; (3000797c <BOOT_Image1+0xa34>)
300078ec:	a808      	add	r0, sp, #32
300078ee:	4798      	blx	r3
300078f0:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
300078f4:	4822      	ldr	r0, [pc, #136]	; (30007980 <BOOT_Image1+0xa38>)
300078f6:	a908      	add	r1, sp, #32
300078f8:	4b22      	ldr	r3, [pc, #136]	; (30007984 <BOOT_Image1+0xa3c>)
300078fa:	f8ad 2022 	strh.w	r2, [sp, #34]	; 0x22
300078fe:	4798      	blx	r3
30007900:	481f      	ldr	r0, [pc, #124]	; (30007980 <BOOT_Image1+0xa38>)
30007902:	4b21      	ldr	r3, [pc, #132]	; (30007988 <BOOT_Image1+0xa40>)
30007904:	4798      	blx	r3
30007906:	e7fe      	b.n	30007906 <BOOT_Image1+0x9be>
30007908:	4b20      	ldr	r3, [pc, #128]	; (3000798c <BOOT_Image1+0xa44>)
3000790a:	4798      	blx	r3
3000790c:	2801      	cmp	r0, #1
3000790e:	bf0c      	ite	eq
30007910:	20e6      	moveq	r0, #230	; 0xe6
30007912:	2019      	movne	r0, #25
30007914:	47a0      	blx	r4
30007916:	e5cf      	b.n	300074b8 <BOOT_Image1+0x570>
30007918:	4b1c      	ldr	r3, [pc, #112]	; (3000798c <BOOT_Image1+0xa44>)
3000791a:	4798      	blx	r3
3000791c:	2801      	cmp	r0, #1
3000791e:	bf0c      	ite	eq
30007920:	2019      	moveq	r0, #25
30007922:	2004      	movne	r0, #4
30007924:	47a0      	blx	r4
30007926:	e5b4      	b.n	30007492 <BOOT_Image1+0x54a>
30007928:	4819      	ldr	r0, [pc, #100]	; (30007990 <BOOT_Image1+0xa48>)
3000792a:	f004 fef1 	bl	3000c710 <__DiagPrintf_veneer>
3000792e:	e594      	b.n	3000745a <BOOT_Image1+0x512>
30007930:	4818      	ldr	r0, [pc, #96]	; (30007994 <BOOT_Image1+0xa4c>)
30007932:	f004 feed 	bl	3000c710 <__DiagPrintf_veneer>
30007936:	4818      	ldr	r0, [pc, #96]	; (30007998 <BOOT_Image1+0xa50>)
30007938:	f004 feea 	bl	3000c710 <__DiagPrintf_veneer>
3000793c:	e58d      	b.n	3000745a <BOOT_Image1+0x512>
3000793e:	4817      	ldr	r0, [pc, #92]	; (3000799c <BOOT_Image1+0xa54>)
30007940:	f004 fee6 	bl	3000c710 <__DiagPrintf_veneer>
30007944:	e589      	b.n	3000745a <BOOT_Image1+0x512>
30007946:	2700      	movs	r7, #0
30007948:	f04f 080a 	mov.w	r8, #10
3000794c:	463b      	mov	r3, r7
3000794e:	46bb      	mov	fp, r7
30007950:	e4da      	b.n	30007308 <BOOT_Image1+0x3c0>
30007952:	46d3      	mov	fp, sl
30007954:	e53a      	b.n	300073cc <BOOT_Image1+0x484>
30007956:	46da      	mov	sl, fp
30007958:	f04f 0900 	mov.w	r9, #0
3000795c:	e503      	b.n	30007366 <BOOT_Image1+0x41e>
3000795e:	f107 0a0b 	add.w	sl, r7, #11
30007962:	f8dd 8010 	ldr.w	r8, [sp, #16]
30007966:	370a      	adds	r7, #10
30007968:	44ca      	add	sl, r9
3000796a:	e774      	b.n	30007856 <BOOT_Image1+0x90e>
3000796c:	3000ee80 	.word	0x3000ee80
30007970:	3000fbac 	.word	0x3000fbac
30007974:	3000f360 	.word	0x3000f360
30007978:	3000d628 	.word	0x3000d628
3000797c:	0000d3f9 	.word	0x0000d3f9
30007980:	41000440 	.word	0x41000440
30007984:	0000d409 	.word	0x0000d409
30007988:	0000d489 	.word	0x0000d489
3000798c:	0000c169 	.word	0x0000c169
30007990:	3000d600 	.word	0x3000d600
30007994:	3000d5d8 	.word	0x3000d5d8
30007998:	3000d5f0 	.word	0x3000d5f0
3000799c:	3000d614 	.word	0x3000d614
300079a0:	3000f3cc 	.word	0x3000f3cc

300079a4 <ymodem_inquiry>:
300079a4:	2806      	cmp	r0, #6
300079a6:	b510      	push	{r4, lr}
300079a8:	4604      	mov	r4, r0
300079aa:	d003      	beq.n	300079b4 <ymodem_inquiry+0x10>
300079ac:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079b0:	4b03      	ldr	r3, [pc, #12]	; (300079c0 <ymodem_inquiry+0x1c>)
300079b2:	4798      	blx	r3
300079b4:	4620      	mov	r0, r4
300079b6:	f000 fa07 	bl	30007dc8 <ymodem_uart_putc>
300079ba:	2001      	movs	r0, #1
300079bc:	bd10      	pop	{r4, pc}
300079be:	bf00      	nop
300079c0:	00009b2d 	.word	0x00009b2d

300079c4 <ymodem_get_first>:
300079c4:	b530      	push	{r4, r5, lr}
300079c6:	b083      	sub	sp, #12
300079c8:	2300      	movs	r3, #0
300079ca:	4604      	mov	r4, r0
300079cc:	4936      	ldr	r1, [pc, #216]	; (30007aa8 <ymodem_get_first+0xe4>)
300079ce:	f10d 0006 	add.w	r0, sp, #6
300079d2:	f88d 3006 	strb.w	r3, [sp, #6]
300079d6:	f000 fa19 	bl	30007e0c <ymodem_uart_getc_to>
300079da:	b9d0      	cbnz	r0, 30007a12 <ymodem_get_first+0x4e>
300079dc:	f89d 3006 	ldrb.w	r3, [sp, #6]
300079e0:	3b01      	subs	r3, #1
300079e2:	2b17      	cmp	r3, #23
300079e4:	d80d      	bhi.n	30007a02 <ymodem_get_first+0x3e>
300079e6:	e8df f003 	tbb	[pc, r3]
300079ea:	231e      	.short	0x231e
300079ec:	0c0c290c 	.word	0x0c0c290c
300079f0:	0c0c0c0c 	.word	0x0c0c0c0c
300079f4:	0c0c0c0c 	.word	0x0c0c0c0c
300079f8:	0c0c0c0c 	.word	0x0c0c0c0c
300079fc:	0c0c0c0c 	.word	0x0c0c0c0c
30007a00:	190c      	.short	0x190c
30007a02:	4d29      	ldr	r5, [pc, #164]	; (30007aa8 <ymodem_get_first+0xe4>)
30007a04:	4629      	mov	r1, r5
30007a06:	f10d 0007 	add.w	r0, sp, #7
30007a0a:	f000 f9ff 	bl	30007e0c <ymodem_uart_getc_to>
30007a0e:	2800      	cmp	r0, #0
30007a10:	d0f8      	beq.n	30007a04 <ymodem_get_first+0x40>
30007a12:	2301      	movs	r3, #1
30007a14:	2008      	movs	r0, #8
30007a16:	60a3      	str	r3, [r4, #8]
30007a18:	b003      	add	sp, #12
30007a1a:	bd30      	pop	{r4, r5, pc}
30007a1c:	2301      	movs	r3, #1
30007a1e:	2002      	movs	r0, #2
30007a20:	60a3      	str	r3, [r4, #8]
30007a22:	b003      	add	sp, #12
30007a24:	bd30      	pop	{r4, r5, pc}
30007a26:	2380      	movs	r3, #128	; 0x80
30007a28:	2001      	movs	r0, #1
30007a2a:	80a3      	strh	r3, [r4, #4]
30007a2c:	b003      	add	sp, #12
30007a2e:	bd30      	pop	{r4, r5, pc}
30007a30:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007a34:	2001      	movs	r0, #1
30007a36:	80a3      	strh	r3, [r4, #4]
30007a38:	b003      	add	sp, #12
30007a3a:	bd30      	pop	{r4, r5, pc}
30007a3c:	2006      	movs	r0, #6
30007a3e:	4d1b      	ldr	r5, [pc, #108]	; (30007aac <ymodem_get_first+0xe8>)
30007a40:	f000 f9c2 	bl	30007dc8 <ymodem_uart_putc>
30007a44:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a48:	47a8      	blx	r5
30007a4a:	2043      	movs	r0, #67	; 0x43
30007a4c:	f000 f9bc 	bl	30007dc8 <ymodem_uart_putc>
30007a50:	2006      	movs	r0, #6
30007a52:	f000 f9b9 	bl	30007dc8 <ymodem_uart_putc>
30007a56:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a5a:	47a8      	blx	r5
30007a5c:	2043      	movs	r0, #67	; 0x43
30007a5e:	f000 f9b3 	bl	30007dc8 <ymodem_uart_putc>
30007a62:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a66:	47a8      	blx	r5
30007a68:	2018      	movs	r0, #24
30007a6a:	f000 f9ad 	bl	30007dc8 <ymodem_uart_putc>
30007a6e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a72:	47a8      	blx	r5
30007a74:	2018      	movs	r0, #24
30007a76:	f000 f9a7 	bl	30007dc8 <ymodem_uart_putc>
30007a7a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a7e:	47a8      	blx	r5
30007a80:	2018      	movs	r0, #24
30007a82:	f000 f9a1 	bl	30007dc8 <ymodem_uart_putc>
30007a86:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a8a:	47a8      	blx	r5
30007a8c:	2018      	movs	r0, #24
30007a8e:	f000 f99b 	bl	30007dc8 <ymodem_uart_putc>
30007a92:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a96:	47a8      	blx	r5
30007a98:	2018      	movs	r0, #24
30007a9a:	f000 f995 	bl	30007dc8 <ymodem_uart_putc>
30007a9e:	2301      	movs	r3, #1
30007aa0:	2005      	movs	r0, #5
30007aa2:	60a3      	str	r3, [r4, #8]
30007aa4:	b003      	add	sp, #12
30007aa6:	bd30      	pop	{r4, r5, pc}
30007aa8:	000caa30 	.word	0x000caa30
30007aac:	00009b2d 	.word	0x00009b2d

30007ab0 <ymodem_get_others>:
30007ab0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007ab4:	b083      	sub	sp, #12
30007ab6:	2300      	movs	r3, #0
30007ab8:	4605      	mov	r5, r0
30007aba:	4952      	ldr	r1, [pc, #328]	; (30007c04 <ymodem_get_others+0x154>)
30007abc:	a801      	add	r0, sp, #4
30007abe:	f88d 3006 	strb.w	r3, [sp, #6]
30007ac2:	f88d 3007 	strb.w	r3, [sp, #7]
30007ac6:	f000 f9a1 	bl	30007e0c <ymodem_uart_getc_to>
30007aca:	bb38      	cbnz	r0, 30007b1c <ymodem_get_others+0x6c>
30007acc:	494d      	ldr	r1, [pc, #308]	; (30007c04 <ymodem_get_others+0x154>)
30007ace:	f10d 0005 	add.w	r0, sp, #5
30007ad2:	f000 f99b 	bl	30007e0c <ymodem_uart_getc_to>
30007ad6:	4607      	mov	r7, r0
30007ad8:	bb00      	cbnz	r0, 30007b1c <ymodem_get_others+0x6c>
30007ada:	f89d 2004 	ldrb.w	r2, [sp, #4]
30007ade:	f89d 6005 	ldrb.w	r6, [sp, #5]
30007ae2:	43d3      	mvns	r3, r2
30007ae4:	b2db      	uxtb	r3, r3
30007ae6:	429e      	cmp	r6, r3
30007ae8:	bf0c      	ite	eq
30007aea:	2601      	moveq	r6, #1
30007aec:	2604      	movne	r6, #4
30007aee:	682b      	ldr	r3, [r5, #0]
30007af0:	429a      	cmp	r2, r3
30007af2:	d21a      	bcs.n	30007b2a <ymodem_get_others+0x7a>
30007af4:	2701      	movs	r7, #1
30007af6:	88ab      	ldrh	r3, [r5, #4]
30007af8:	b1d3      	cbz	r3, 30007b30 <ymodem_get_others+0x80>
30007afa:	2400      	movs	r4, #0
30007afc:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007c08 <ymodem_get_others+0x158>
30007b00:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007c04 <ymodem_get_others+0x154>
30007b04:	e002      	b.n	30007b0c <ymodem_get_others+0x5c>
30007b06:	88ab      	ldrh	r3, [r5, #4]
30007b08:	42a3      	cmp	r3, r4
30007b0a:	d911      	bls.n	30007b30 <ymodem_get_others+0x80>
30007b0c:	eb04 0009 	add.w	r0, r4, r9
30007b10:	4641      	mov	r1, r8
30007b12:	3401      	adds	r4, #1
30007b14:	f000 f97a 	bl	30007e0c <ymodem_uart_getc_to>
30007b18:	2800      	cmp	r0, #0
30007b1a:	d0f4      	beq.n	30007b06 <ymodem_get_others+0x56>
30007b1c:	2301      	movs	r3, #1
30007b1e:	2608      	movs	r6, #8
30007b20:	60ab      	str	r3, [r5, #8]
30007b22:	4630      	mov	r0, r6
30007b24:	b003      	add	sp, #12
30007b26:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007b2a:	d0e4      	beq.n	30007af6 <ymodem_get_others+0x46>
30007b2c:	2604      	movs	r6, #4
30007b2e:	e7e2      	b.n	30007af6 <ymodem_get_others+0x46>
30007b30:	4934      	ldr	r1, [pc, #208]	; (30007c04 <ymodem_get_others+0x154>)
30007b32:	f10d 0006 	add.w	r0, sp, #6
30007b36:	f000 f969 	bl	30007e0c <ymodem_uart_getc_to>
30007b3a:	2800      	cmp	r0, #0
30007b3c:	d1ee      	bne.n	30007b1c <ymodem_get_others+0x6c>
30007b3e:	4931      	ldr	r1, [pc, #196]	; (30007c04 <ymodem_get_others+0x154>)
30007b40:	f10d 0007 	add.w	r0, sp, #7
30007b44:	f000 f962 	bl	30007e0c <ymodem_uart_getc_to>
30007b48:	2800      	cmp	r0, #0
30007b4a:	d1e7      	bne.n	30007b1c <ymodem_get_others+0x6c>
30007b4c:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007b50:	f89d e007 	ldrb.w	lr, [sp, #7]
30007b54:	88aa      	ldrh	r2, [r5, #4]
30007b56:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
30007b5a:	b36a      	cbz	r2, 30007bb8 <ymodem_get_others+0x108>
30007b5c:	4603      	mov	r3, r0
30007b5e:	482a      	ldr	r0, [pc, #168]	; (30007c08 <ymodem_get_others+0x158>)
30007b60:	f241 0c21 	movw	ip, #4129	; 0x1021
30007b64:	eb02 0800 	add.w	r8, r2, r0
30007b68:	7801      	ldrb	r1, [r0, #0]
30007b6a:	3001      	adds	r0, #1
30007b6c:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007b70:	2108      	movs	r1, #8
30007b72:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
30007b76:	005b      	lsls	r3, r3, #1
30007b78:	b10c      	cbz	r4, 30007b7e <ymodem_get_others+0xce>
30007b7a:	ea83 030c 	eor.w	r3, r3, ip
30007b7e:	3901      	subs	r1, #1
30007b80:	d1f7      	bne.n	30007b72 <ymodem_get_others+0xc2>
30007b82:	4580      	cmp	r8, r0
30007b84:	d1f0      	bne.n	30007b68 <ymodem_get_others+0xb8>
30007b86:	b29b      	uxth	r3, r3
30007b88:	459e      	cmp	lr, r3
30007b8a:	d00a      	beq.n	30007ba2 <ymodem_get_others+0xf2>
30007b8c:	4b1f      	ldr	r3, [pc, #124]	; (30007c0c <ymodem_get_others+0x15c>)
30007b8e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007b92:	4798      	blx	r3
30007b94:	2015      	movs	r0, #21
30007b96:	f000 f917 	bl	30007dc8 <ymodem_uart_putc>
30007b9a:	2301      	movs	r3, #1
30007b9c:	2604      	movs	r6, #4
30007b9e:	60ab      	str	r3, [r5, #8]
30007ba0:	e7bf      	b.n	30007b22 <ymodem_get_others+0x72>
30007ba2:	b917      	cbnz	r7, 30007baa <ymodem_get_others+0xfa>
30007ba4:	4c1a      	ldr	r4, [pc, #104]	; (30007c10 <ymodem_get_others+0x160>)
30007ba6:	6823      	ldr	r3, [r4, #0]
30007ba8:	b9b3      	cbnz	r3, 30007bd8 <ymodem_get_others+0x128>
30007baa:	f000 f947 	bl	30007e3c <ymodem_uart_clean_rx>
30007bae:	2e01      	cmp	r6, #1
30007bb0:	d004      	beq.n	30007bbc <ymodem_get_others+0x10c>
30007bb2:	2e04      	cmp	r6, #4
30007bb4:	d1b5      	bne.n	30007b22 <ymodem_get_others+0x72>
30007bb6:	e7e9      	b.n	30007b8c <ymodem_get_others+0xdc>
30007bb8:	4613      	mov	r3, r2
30007bba:	e7e5      	b.n	30007b88 <ymodem_get_others+0xd8>
30007bbc:	4c14      	ldr	r4, [pc, #80]	; (30007c10 <ymodem_get_others+0x160>)
30007bbe:	2006      	movs	r0, #6
30007bc0:	60ae      	str	r6, [r5, #8]
30007bc2:	f000 f901 	bl	30007dc8 <ymodem_uart_putc>
30007bc6:	6823      	ldr	r3, [r4, #0]
30007bc8:	b193      	cbz	r3, 30007bf0 <ymodem_get_others+0x140>
30007bca:	2f00      	cmp	r7, #0
30007bcc:	f103 0301 	add.w	r3, r3, #1
30007bd0:	bf18      	it	ne
30007bd2:	2604      	movne	r6, #4
30007bd4:	6023      	str	r3, [r4, #0]
30007bd6:	e7a4      	b.n	30007b22 <ymodem_get_others+0x72>
30007bd8:	68eb      	ldr	r3, [r5, #12]
30007bda:	6861      	ldr	r1, [r4, #4]
30007bdc:	3301      	adds	r3, #1
30007bde:	480a      	ldr	r0, [pc, #40]	; (30007c08 <ymodem_get_others+0x158>)
30007be0:	60eb      	str	r3, [r5, #12]
30007be2:	f000 f94f 	bl	30007e84 <UARTIMG_Write>
30007be6:	6862      	ldr	r2, [r4, #4]
30007be8:	88ab      	ldrh	r3, [r5, #4]
30007bea:	4413      	add	r3, r2
30007bec:	6063      	str	r3, [r4, #4]
30007bee:	e7dc      	b.n	30007baa <ymodem_get_others+0xfa>
30007bf0:	4b06      	ldr	r3, [pc, #24]	; (30007c0c <ymodem_get_others+0x15c>)
30007bf2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007bf6:	4798      	blx	r3
30007bf8:	2043      	movs	r0, #67	; 0x43
30007bfa:	f000 f8e5 	bl	30007dc8 <ymodem_uart_putc>
30007bfe:	6823      	ldr	r3, [r4, #0]
30007c00:	e7e3      	b.n	30007bca <ymodem_get_others+0x11a>
30007c02:	bf00      	nop
30007c04:	000caa30 	.word	0x000caa30
30007c08:	20016ffc 	.word	0x20016ffc
30007c0c:	00009b2d 	.word	0x00009b2d
30007c10:	3000f404 	.word	0x3000f404

30007c14 <ymodem_rx_frame>:
30007c14:	2300      	movs	r3, #0
30007c16:	2201      	movs	r2, #1
30007c18:	b510      	push	{r4, lr}
30007c1a:	4604      	mov	r4, r0
30007c1c:	6083      	str	r3, [r0, #8]
30007c1e:	8082      	strh	r2, [r0, #4]
30007c20:	f7ff fed0 	bl	300079c4 <ymodem_get_first>
30007c24:	68a3      	ldr	r3, [r4, #8]
30007c26:	b92b      	cbnz	r3, 30007c34 <ymodem_rx_frame+0x20>
30007c28:	4620      	mov	r0, r4
30007c2a:	f7ff ff41 	bl	30007ab0 <ymodem_get_others>
30007c2e:	68a3      	ldr	r3, [r4, #8]
30007c30:	2b00      	cmp	r3, #0
30007c32:	d0f9      	beq.n	30007c28 <ymodem_rx_frame+0x14>
30007c34:	2801      	cmp	r0, #1
30007c36:	d000      	beq.n	30007c3a <ymodem_rx_frame+0x26>
30007c38:	bd10      	pop	{r4, pc}
30007c3a:	6823      	ldr	r3, [r4, #0]
30007c3c:	3301      	adds	r3, #1
30007c3e:	b2db      	uxtb	r3, r3
30007c40:	6023      	str	r3, [r4, #0]
30007c42:	bd10      	pop	{r4, pc}

30007c44 <ymodem_img_rxbuffer>:
30007c44:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007c48:	2300      	movs	r3, #0
30007c4a:	4c28      	ldr	r4, [pc, #160]	; (30007cec <ymodem_img_rxbuffer+0xa8>)
30007c4c:	4e28      	ldr	r6, [pc, #160]	; (30007cf0 <ymodem_img_rxbuffer+0xac>)
30007c4e:	460d      	mov	r5, r1
30007c50:	60a3      	str	r3, [r4, #8]
30007c52:	6163      	str	r3, [r4, #20]
30007c54:	f000 f8f2 	bl	30007e3c <ymodem_uart_clean_rx>
30007c58:	2043      	movs	r0, #67	; 0x43
30007c5a:	f000 f8b5 	bl	30007dc8 <ymodem_uart_putc>
30007c5e:	f24c 3050 	movw	r0, #50000	; 0xc350
30007c62:	47b0      	blx	r6
30007c64:	f000 f89a 	bl	30007d9c <ymodem_uart_readable>
30007c68:	2800      	cmp	r0, #0
30007c6a:	d0f5      	beq.n	30007c58 <ymodem_img_rxbuffer+0x14>
30007c6c:	2700      	movs	r7, #0
30007c6e:	f04f 0801 	mov.w	r8, #1
30007c72:	e9c4 7500 	strd	r7, r5, [r4]
30007c76:	4d1f      	ldr	r5, [pc, #124]	; (30007cf4 <ymodem_img_rxbuffer+0xb0>)
30007c78:	4628      	mov	r0, r5
30007c7a:	f8a4 800c 	strh.w	r8, [r4, #12]
30007c7e:	6127      	str	r7, [r4, #16]
30007c80:	f7ff fea0 	bl	300079c4 <ymodem_get_first>
30007c84:	6923      	ldr	r3, [r4, #16]
30007c86:	b92b      	cbnz	r3, 30007c94 <ymodem_img_rxbuffer+0x50>
30007c88:	4628      	mov	r0, r5
30007c8a:	f7ff ff11 	bl	30007ab0 <ymodem_get_others>
30007c8e:	6923      	ldr	r3, [r4, #16]
30007c90:	2b00      	cmp	r3, #0
30007c92:	d0f9      	beq.n	30007c88 <ymodem_img_rxbuffer+0x44>
30007c94:	2801      	cmp	r0, #1
30007c96:	d024      	beq.n	30007ce2 <ymodem_img_rxbuffer+0x9e>
30007c98:	3804      	subs	r0, #4
30007c9a:	2804      	cmp	r0, #4
30007c9c:	d80c      	bhi.n	30007cb8 <ymodem_img_rxbuffer+0x74>
30007c9e:	a301      	add	r3, pc, #4	; (adr r3, 30007ca4 <ymodem_img_rxbuffer+0x60>)
30007ca0:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007ca4:	30007c79 	.word	0x30007c79
30007ca8:	30007cc5 	.word	0x30007cc5
30007cac:	30007cb9 	.word	0x30007cb9
30007cb0:	30007cb9 	.word	0x30007cb9
30007cb4:	30007cd1 	.word	0x30007cd1
30007cb8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007cbc:	47b0      	blx	r6
30007cbe:	2018      	movs	r0, #24
30007cc0:	f000 f882 	bl	30007dc8 <ymodem_uart_putc>
30007cc4:	200a      	movs	r0, #10
30007cc6:	f000 f8c9 	bl	30007e5c <ymodem_uart_waitbusy>
30007cca:	2001      	movs	r0, #1
30007ccc:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007cd0:	f000 f8b4 	bl	30007e3c <ymodem_uart_clean_rx>
30007cd4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007cd8:	47b0      	blx	r6
30007cda:	2015      	movs	r0, #21
30007cdc:	f000 f874 	bl	30007dc8 <ymodem_uart_putc>
30007ce0:	e7ca      	b.n	30007c78 <ymodem_img_rxbuffer+0x34>
30007ce2:	68a3      	ldr	r3, [r4, #8]
30007ce4:	3301      	adds	r3, #1
30007ce6:	b2db      	uxtb	r3, r3
30007ce8:	60a3      	str	r3, [r4, #8]
30007cea:	e7c5      	b.n	30007c78 <ymodem_img_rxbuffer+0x34>
30007cec:	3000f404 	.word	0x3000f404
30007cf0:	00009b2d 	.word	0x00009b2d
30007cf4:	3000f40c 	.word	0x3000f40c

30007cf8 <ymodem_log_open>:
30007cf8:	b510      	push	{r4, lr}
30007cfa:	2303      	movs	r3, #3
30007cfc:	4c04      	ldr	r4, [pc, #16]	; (30007d10 <ymodem_log_open+0x18>)
30007cfe:	2200      	movs	r2, #0
30007d00:	2108      	movs	r1, #8
30007d02:	4804      	ldr	r0, [pc, #16]	; (30007d14 <ymodem_log_open+0x1c>)
30007d04:	47a0      	blx	r4
30007d06:	2008      	movs	r0, #8
30007d08:	4b03      	ldr	r3, [pc, #12]	; (30007d18 <ymodem_log_open+0x20>)
30007d0a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007d0e:	4718      	bx	r3
30007d10:	3000a199 	.word	0x3000a199
30007d14:	0000ed39 	.word	0x0000ed39
30007d18:	0000e1bd 	.word	0x0000e1bd

30007d1c <ymodem_uart_port_init>:
30007d1c:	b500      	push	{lr}
30007d1e:	b083      	sub	sp, #12
30007d20:	2008      	movs	r0, #8
30007d22:	4b06      	ldr	r3, [pc, #24]	; (30007d3c <ymodem_uart_port_init+0x20>)
30007d24:	9201      	str	r2, [sp, #4]
30007d26:	4798      	blx	r3
30007d28:	4b05      	ldr	r3, [pc, #20]	; (30007d40 <ymodem_uart_port_init+0x24>)
30007d2a:	2008      	movs	r0, #8
30007d2c:	4798      	blx	r3
30007d2e:	9901      	ldr	r1, [sp, #4]
30007d30:	4804      	ldr	r0, [pc, #16]	; (30007d44 <ymodem_uart_port_init+0x28>)
30007d32:	4b05      	ldr	r3, [pc, #20]	; (30007d48 <ymodem_uart_port_init+0x2c>)
30007d34:	b003      	add	sp, #12
30007d36:	f85d eb04 	ldr.w	lr, [sp], #4
30007d3a:	4718      	bx	r3
30007d3c:	0000e1d9 	.word	0x0000e1d9
30007d40:	0000e2fd 	.word	0x0000e2fd
30007d44:	4200c000 	.word	0x4200c000
30007d48:	0000a9e5 	.word	0x0000a9e5

30007d4c <ymodem_uart_port_deinit>:
30007d4c:	2804      	cmp	r0, #4
30007d4e:	d004      	beq.n	30007d5a <ymodem_uart_port_deinit+0xe>
30007d50:	4b0d      	ldr	r3, [pc, #52]	; (30007d88 <ymodem_uart_port_deinit+0x3c>)
30007d52:	681b      	ldr	r3, [r3, #0]
30007d54:	039b      	lsls	r3, r3, #14
30007d56:	d40f      	bmi.n	30007d78 <ymodem_uart_port_deinit+0x2c>
30007d58:	4770      	bx	lr
30007d5a:	2200      	movs	r2, #0
30007d5c:	2303      	movs	r3, #3
30007d5e:	2108      	movs	r1, #8
30007d60:	480a      	ldr	r0, [pc, #40]	; (30007d8c <ymodem_uart_port_deinit+0x40>)
30007d62:	b510      	push	{r4, lr}
30007d64:	4c0a      	ldr	r4, [pc, #40]	; (30007d90 <ymodem_uart_port_deinit+0x44>)
30007d66:	47a0      	blx	r4
30007d68:	4b0a      	ldr	r3, [pc, #40]	; (30007d94 <ymodem_uart_port_deinit+0x48>)
30007d6a:	2008      	movs	r0, #8
30007d6c:	4798      	blx	r3
30007d6e:	4b06      	ldr	r3, [pc, #24]	; (30007d88 <ymodem_uart_port_deinit+0x3c>)
30007d70:	681b      	ldr	r3, [r3, #0]
30007d72:	039a      	lsls	r2, r3, #14
30007d74:	d403      	bmi.n	30007d7e <ymodem_uart_port_deinit+0x32>
30007d76:	bd10      	pop	{r4, pc}
30007d78:	4807      	ldr	r0, [pc, #28]	; (30007d98 <ymodem_uart_port_deinit+0x4c>)
30007d7a:	f004 bcc9 	b.w	3000c710 <__DiagPrintf_veneer>
30007d7e:	4806      	ldr	r0, [pc, #24]	; (30007d98 <ymodem_uart_port_deinit+0x4c>)
30007d80:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007d84:	f004 bcc4 	b.w	3000c710 <__DiagPrintf_veneer>
30007d88:	2001c00c 	.word	0x2001c00c
30007d8c:	0000ed39 	.word	0x0000ed39
30007d90:	3000a199 	.word	0x3000a199
30007d94:	0000e1bd 	.word	0x0000e1bd
30007d98:	3000d680 	.word	0x3000d680

30007d9c <ymodem_uart_readable>:
30007d9c:	4b00      	ldr	r3, [pc, #0]	; (30007da0 <ymodem_uart_readable+0x4>)
30007d9e:	4718      	bx	r3
30007da0:	0000ac39 	.word	0x0000ac39

30007da4 <ymodem_uart_writable>:
30007da4:	4a03      	ldr	r2, [pc, #12]	; (30007db4 <ymodem_uart_writable+0x10>)
30007da6:	b508      	push	{r3, lr}
30007da8:	4b03      	ldr	r3, [pc, #12]	; (30007db8 <ymodem_uart_writable+0x14>)
30007daa:	6810      	ldr	r0, [r2, #0]
30007dac:	4798      	blx	r3
30007dae:	b2c0      	uxtb	r0, r0
30007db0:	bd08      	pop	{r3, pc}
30007db2:	bf00      	nop
30007db4:	3000f41c 	.word	0x3000f41c
30007db8:	0000d1a9 	.word	0x0000d1a9

30007dbc <ymodem_uart_getc>:
30007dbc:	2000      	movs	r0, #0
30007dbe:	4b01      	ldr	r3, [pc, #4]	; (30007dc4 <ymodem_uart_getc+0x8>)
30007dc0:	4718      	bx	r3
30007dc2:	bf00      	nop
30007dc4:	0000ac1d 	.word	0x0000ac1d

30007dc8 <ymodem_uart_putc>:
30007dc8:	4b00      	ldr	r3, [pc, #0]	; (30007dcc <ymodem_uart_putc+0x4>)
30007dca:	4718      	bx	r3
30007dcc:	0000abb5 	.word	0x0000abb5

30007dd0 <ymodem_uart_putdata>:
30007dd0:	b149      	cbz	r1, 30007de6 <ymodem_uart_putdata+0x16>
30007dd2:	b570      	push	{r4, r5, r6, lr}
30007dd4:	4604      	mov	r4, r0
30007dd6:	1845      	adds	r5, r0, r1
30007dd8:	4e03      	ldr	r6, [pc, #12]	; (30007de8 <ymodem_uart_putdata+0x18>)
30007dda:	f814 0b01 	ldrb.w	r0, [r4], #1
30007dde:	47b0      	blx	r6
30007de0:	42ac      	cmp	r4, r5
30007de2:	d1fa      	bne.n	30007dda <ymodem_uart_putdata+0xa>
30007de4:	bd70      	pop	{r4, r5, r6, pc}
30007de6:	4770      	bx	lr
30007de8:	0000abb5 	.word	0x0000abb5

30007dec <ymodem_uart_getdata>:
30007dec:	b151      	cbz	r1, 30007e04 <ymodem_uart_getdata+0x18>
30007dee:	b570      	push	{r4, r5, r6, lr}
30007df0:	4604      	mov	r4, r0
30007df2:	1845      	adds	r5, r0, r1
30007df4:	4e04      	ldr	r6, [pc, #16]	; (30007e08 <ymodem_uart_getdata+0x1c>)
30007df6:	2001      	movs	r0, #1
30007df8:	47b0      	blx	r6
30007dfa:	f804 0b01 	strb.w	r0, [r4], #1
30007dfe:	42ac      	cmp	r4, r5
30007e00:	d1f9      	bne.n	30007df6 <ymodem_uart_getdata+0xa>
30007e02:	bd70      	pop	{r4, r5, r6, pc}
30007e04:	4770      	bx	lr
30007e06:	bf00      	nop
30007e08:	0000ac1d 	.word	0x0000ac1d

30007e0c <ymodem_uart_getc_to>:
30007e0c:	b570      	push	{r4, r5, r6, lr}
30007e0e:	4d09      	ldr	r5, [pc, #36]	; (30007e34 <ymodem_uart_getc_to+0x28>)
30007e10:	4606      	mov	r6, r0
30007e12:	460c      	mov	r4, r1
30007e14:	e001      	b.n	30007e1a <ymodem_uart_getc_to+0xe>
30007e16:	3c01      	subs	r4, #1
30007e18:	d009      	beq.n	30007e2e <ymodem_uart_getc_to+0x22>
30007e1a:	47a8      	blx	r5
30007e1c:	2800      	cmp	r0, #0
30007e1e:	d0fa      	beq.n	30007e16 <ymodem_uart_getc_to+0xa>
30007e20:	4b05      	ldr	r3, [pc, #20]	; (30007e38 <ymodem_uart_getc_to+0x2c>)
30007e22:	2000      	movs	r0, #0
30007e24:	4798      	blx	r3
30007e26:	4603      	mov	r3, r0
30007e28:	2000      	movs	r0, #0
30007e2a:	7033      	strb	r3, [r6, #0]
30007e2c:	bd70      	pop	{r4, r5, r6, pc}
30007e2e:	2001      	movs	r0, #1
30007e30:	bd70      	pop	{r4, r5, r6, pc}
30007e32:	bf00      	nop
30007e34:	0000ac39 	.word	0x0000ac39
30007e38:	0000ac1d 	.word	0x0000ac1d

30007e3c <ymodem_uart_clean_rx>:
30007e3c:	b538      	push	{r3, r4, r5, lr}
30007e3e:	4c05      	ldr	r4, [pc, #20]	; (30007e54 <ymodem_uart_clean_rx+0x18>)
30007e40:	4d05      	ldr	r5, [pc, #20]	; (30007e58 <ymodem_uart_clean_rx+0x1c>)
30007e42:	e000      	b.n	30007e46 <ymodem_uart_clean_rx+0xa>
30007e44:	47a8      	blx	r5
30007e46:	47a0      	blx	r4
30007e48:	4603      	mov	r3, r0
30007e4a:	2000      	movs	r0, #0
30007e4c:	2b00      	cmp	r3, #0
30007e4e:	d1f9      	bne.n	30007e44 <ymodem_uart_clean_rx+0x8>
30007e50:	bd38      	pop	{r3, r4, r5, pc}
30007e52:	bf00      	nop
30007e54:	0000ac39 	.word	0x0000ac39
30007e58:	0000ac1d 	.word	0x0000ac1d

30007e5c <ymodem_uart_waitbusy>:
30007e5c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007e5e:	2400      	movs	r4, #0
30007e60:	4e06      	ldr	r6, [pc, #24]	; (30007e7c <ymodem_uart_waitbusy+0x20>)
30007e62:	4f07      	ldr	r7, [pc, #28]	; (30007e80 <ymodem_uart_waitbusy+0x24>)
30007e64:	4605      	mov	r5, r0
30007e66:	e002      	b.n	30007e6e <ymodem_uart_waitbusy+0x12>
30007e68:	47b8      	blx	r7
30007e6a:	42ac      	cmp	r4, r5
30007e6c:	d804      	bhi.n	30007e78 <ymodem_uart_waitbusy+0x1c>
30007e6e:	6973      	ldr	r3, [r6, #20]
30007e70:	3401      	adds	r4, #1
30007e72:	2064      	movs	r0, #100	; 0x64
30007e74:	03db      	lsls	r3, r3, #15
30007e76:	d5f7      	bpl.n	30007e68 <ymodem_uart_waitbusy+0xc>
30007e78:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007e7a:	bf00      	nop
30007e7c:	4200c000 	.word	0x4200c000
30007e80:	00009b2d 	.word	0x00009b2d

30007e84 <UARTIMG_Write>:
30007e84:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007e88:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007e8c:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007e90:	d21d      	bcs.n	30007ece <UARTIMG_Write+0x4a>
30007e92:	3a01      	subs	r2, #1
30007e94:	4f19      	ldr	r7, [pc, #100]	; (30007efc <UARTIMG_Write+0x78>)
30007e96:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007f0c <UARTIMG_Write+0x88>
30007e9a:	4688      	mov	r8, r1
30007e9c:	1916      	adds	r6, r2, r4
30007e9e:	4605      	mov	r5, r0
30007ea0:	f8d9 3000 	ldr.w	r3, [r9]
30007ea4:	403e      	ands	r6, r7
30007ea6:	4027      	ands	r7, r4
30007ea8:	42bb      	cmp	r3, r7
30007eaa:	d91b      	bls.n	30007ee4 <UARTIMG_Write+0x60>
30007eac:	42b3      	cmp	r3, r6
30007eae:	d910      	bls.n	30007ed2 <UARTIMG_Write+0x4e>
30007eb0:	4e13      	ldr	r6, [pc, #76]	; (30007f00 <UARTIMG_Write+0x7c>)
30007eb2:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007eb6:	4f13      	ldr	r7, [pc, #76]	; (30007f04 <UARTIMG_Write+0x80>)
30007eb8:	4446      	add	r6, r8
30007eba:	eba3 0808 	sub.w	r8, r3, r8
30007ebe:	eb08 0204 	add.w	r2, r8, r4
30007ec2:	4620      	mov	r0, r4
30007ec4:	3408      	adds	r4, #8
30007ec6:	2108      	movs	r1, #8
30007ec8:	47b8      	blx	r7
30007eca:	42b4      	cmp	r4, r6
30007ecc:	d1f7      	bne.n	30007ebe <UARTIMG_Write+0x3a>
30007ece:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007ed2:	4631      	mov	r1, r6
30007ed4:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007ed8:	2002      	movs	r0, #2
30007eda:	4b0b      	ldr	r3, [pc, #44]	; (30007f08 <UARTIMG_Write+0x84>)
30007edc:	4798      	blx	r3
30007ede:	f8c9 6000 	str.w	r6, [r9]
30007ee2:	e7e5      	b.n	30007eb0 <UARTIMG_Write+0x2c>
30007ee4:	4b08      	ldr	r3, [pc, #32]	; (30007f08 <UARTIMG_Write+0x84>)
30007ee6:	4639      	mov	r1, r7
30007ee8:	2002      	movs	r0, #2
30007eea:	4798      	blx	r3
30007eec:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007ef0:	42b3      	cmp	r3, r6
30007ef2:	f8c9 3000 	str.w	r3, [r9]
30007ef6:	d8db      	bhi.n	30007eb0 <UARTIMG_Write+0x2c>
30007ef8:	e7eb      	b.n	30007ed2 <UARTIMG_Write+0x4e>
30007efa:	bf00      	nop
30007efc:	fffff000 	.word	0xfffff000
30007f00:	f8000400 	.word	0xf8000400
30007f04:	0000a04d 	.word	0x0000a04d
30007f08:	0000a16d 	.word	0x0000a16d
30007f0c:	3000f420 	.word	0x3000f420

30007f10 <ymodem_img_download>:
30007f10:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007f14:	4616      	mov	r6, r2
30007f16:	460d      	mov	r5, r1
30007f18:	4a0a      	ldr	r2, [pc, #40]	; (30007f44 <ymodem_img_download+0x34>)
30007f1a:	2100      	movs	r1, #0
30007f1c:	4680      	mov	r8, r0
30007f1e:	f7ff fefd 	bl	30007d1c <ymodem_uart_port_init>
30007f22:	b156      	cbz	r6, 30007f3a <ymodem_img_download+0x2a>
30007f24:	3d04      	subs	r5, #4
30007f26:	2400      	movs	r4, #0
30007f28:	4f06      	ldr	r7, [pc, #24]	; (30007f44 <ymodem_img_download+0x34>)
30007f2a:	3401      	adds	r4, #1
30007f2c:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007f30:	4638      	mov	r0, r7
30007f32:	f7ff fe87 	bl	30007c44 <ymodem_img_rxbuffer>
30007f36:	42b4      	cmp	r4, r6
30007f38:	d1f7      	bne.n	30007f2a <ymodem_img_download+0x1a>
30007f3a:	4640      	mov	r0, r8
30007f3c:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007f40:	f7ff bf04 	b.w	30007d4c <ymodem_uart_port_deinit>
30007f44:	0016e360 	.word	0x0016e360

30007f48 <ChipInfo_Invalid>:
30007f48:	4e06      	ldr	r6, [pc, #24]	; (30007f64 <ChipInfo_Invalid+0x1c>)
30007f4a:	4d07      	ldr	r5, [pc, #28]	; (30007f68 <ChipInfo_Invalid+0x20>)
30007f4c:	4c07      	ldr	r4, [pc, #28]	; (30007f6c <ChipInfo_Invalid+0x24>)
30007f4e:	b508      	push	{r3, lr}
30007f50:	462b      	mov	r3, r5
30007f52:	4622      	mov	r2, r4
30007f54:	4904      	ldr	r1, [pc, #16]	; (30007f68 <ChipInfo_Invalid+0x20>)
30007f56:	2002      	movs	r0, #2
30007f58:	f004 f956 	bl	3000c208 <rtk_log_write>
30007f5c:	f242 7010 	movw	r0, #10000	; 0x2710
30007f60:	47b0      	blx	r6
30007f62:	e7f5      	b.n	30007f50 <ChipInfo_Invalid+0x8>
30007f64:	00009be5 	.word	0x00009be5
30007f68:	3000d6a0 	.word	0x3000d6a0
30007f6c:	3000d6ac 	.word	0x3000d6ac

30007f70 <ChipInfo_Get>:
30007f70:	b570      	push	{r4, r5, r6, lr}
30007f72:	4c11      	ldr	r4, [pc, #68]	; (30007fb8 <ChipInfo_Get+0x48>)
30007f74:	7820      	ldrb	r0, [r4, #0]
30007f76:	28ff      	cmp	r0, #255	; 0xff
30007f78:	d104      	bne.n	30007f84 <ChipInfo_Get+0x14>
30007f7a:	4d10      	ldr	r5, [pc, #64]	; (30007fbc <ChipInfo_Get+0x4c>)
30007f7c:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007f80:	b108      	cbz	r0, 30007f86 <ChipInfo_Get+0x16>
30007f82:	7020      	strb	r0, [r4, #0]
30007f84:	bd70      	pop	{r4, r5, r6, pc}
30007f86:	4621      	mov	r1, r4
30007f88:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007f8c:	f000 ffe0 	bl	30008f50 <OTP_Read8>
30007f90:	7820      	ldrb	r0, [r4, #0]
30007f92:	28ff      	cmp	r0, #255	; 0xff
30007f94:	d002      	beq.n	30007f9c <ChipInfo_Get+0x2c>
30007f96:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007f9a:	bd70      	pop	{r4, r5, r6, pc}
30007f9c:	4e08      	ldr	r6, [pc, #32]	; (30007fc0 <ChipInfo_Get+0x50>)
30007f9e:	4d09      	ldr	r5, [pc, #36]	; (30007fc4 <ChipInfo_Get+0x54>)
30007fa0:	4c09      	ldr	r4, [pc, #36]	; (30007fc8 <ChipInfo_Get+0x58>)
30007fa2:	462b      	mov	r3, r5
30007fa4:	4622      	mov	r2, r4
30007fa6:	4907      	ldr	r1, [pc, #28]	; (30007fc4 <ChipInfo_Get+0x54>)
30007fa8:	2003      	movs	r0, #3
30007faa:	f004 f92d 	bl	3000c208 <rtk_log_write>
30007fae:	f242 7010 	movw	r0, #10000	; 0x2710
30007fb2:	47b0      	blx	r6
30007fb4:	e7f5      	b.n	30007fa2 <ChipInfo_Get+0x32>
30007fb6:	bf00      	nop
30007fb8:	3000f51c 	.word	0x3000f51c
30007fbc:	23020000 	.word	0x23020000
30007fc0:	00009be5 	.word	0x00009be5
30007fc4:	3000d6a0 	.word	0x3000d6a0
30007fc8:	3000d6d0 	.word	0x3000d6d0

30007fcc <ChipInfo_MemoryType>:
30007fcc:	b510      	push	{r4, lr}
30007fce:	4c11      	ldr	r4, [pc, #68]	; (30008014 <ChipInfo_MemoryType+0x48>)
30007fd0:	47a0      	blx	r4
30007fd2:	2802      	cmp	r0, #2
30007fd4:	d019      	beq.n	3000800a <ChipInfo_MemoryType+0x3e>
30007fd6:	47a0      	blx	r4
30007fd8:	2803      	cmp	r0, #3
30007fda:	d016      	beq.n	3000800a <ChipInfo_MemoryType+0x3e>
30007fdc:	4c0e      	ldr	r4, [pc, #56]	; (30008018 <ChipInfo_MemoryType+0x4c>)
30007fde:	4b0f      	ldr	r3, [pc, #60]	; (3000801c <ChipInfo_MemoryType+0x50>)
30007fe0:	4798      	blx	r3
30007fe2:	2100      	movs	r1, #0
30007fe4:	23e0      	movs	r3, #224	; 0xe0
30007fe6:	4622      	mov	r2, r4
30007fe8:	e004      	b.n	30007ff4 <ChipInfo_MemoryType+0x28>
30007fea:	7a13      	ldrb	r3, [r2, #8]
30007fec:	3101      	adds	r1, #1
30007fee:	3206      	adds	r2, #6
30007ff0:	2bff      	cmp	r3, #255	; 0xff
30007ff2:	d00c      	beq.n	3000800e <ChipInfo_MemoryType+0x42>
30007ff4:	4298      	cmp	r0, r3
30007ff6:	d1f8      	bne.n	30007fea <ChipInfo_MemoryType+0x1e>
30007ff8:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ffc:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30008000:	78cb      	ldrb	r3, [r1, #3]
30008002:	2b02      	cmp	r3, #2
30008004:	d001      	beq.n	3000800a <ChipInfo_MemoryType+0x3e>
30008006:	2001      	movs	r0, #1
30008008:	bd10      	pop	{r4, pc}
3000800a:	2002      	movs	r0, #2
3000800c:	bd10      	pop	{r4, pc}
3000800e:	f7ff ff9b 	bl	30007f48 <ChipInfo_Invalid>
30008012:	bf00      	nop
30008014:	0000c0f9 	.word	0x0000c0f9
30008018:	3000d6fc 	.word	0x3000d6fc
3000801c:	30007f71 	.word	0x30007f71

30008020 <ChipInfo_DDRType>:
30008020:	b510      	push	{r4, lr}
30008022:	4b0f      	ldr	r3, [pc, #60]	; (30008060 <ChipInfo_DDRType+0x40>)
30008024:	4798      	blx	r3
30008026:	4b0f      	ldr	r3, [pc, #60]	; (30008064 <ChipInfo_DDRType+0x44>)
30008028:	4604      	mov	r4, r0
3000802a:	4798      	blx	r3
3000802c:	2801      	cmp	r0, #1
3000802e:	d112      	bne.n	30008056 <ChipInfo_DDRType+0x36>
30008030:	480d      	ldr	r0, [pc, #52]	; (30008068 <ChipInfo_DDRType+0x48>)
30008032:	2100      	movs	r1, #0
30008034:	22e0      	movs	r2, #224	; 0xe0
30008036:	4603      	mov	r3, r0
30008038:	e003      	b.n	30008042 <ChipInfo_DDRType+0x22>
3000803a:	789a      	ldrb	r2, [r3, #2]
3000803c:	3101      	adds	r1, #1
3000803e:	2aff      	cmp	r2, #255	; 0xff
30008040:	d00b      	beq.n	3000805a <ChipInfo_DDRType+0x3a>
30008042:	4294      	cmp	r4, r2
30008044:	f103 0306 	add.w	r3, r3, #6
30008048:	d1f7      	bne.n	3000803a <ChipInfo_DDRType+0x1a>
3000804a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000804e:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30008052:	78c8      	ldrb	r0, [r1, #3]
30008054:	bd10      	pop	{r4, pc}
30008056:	20ff      	movs	r0, #255	; 0xff
30008058:	bd10      	pop	{r4, pc}
3000805a:	2000      	movs	r0, #0
3000805c:	bd10      	pop	{r4, pc}
3000805e:	bf00      	nop
30008060:	30007f71 	.word	0x30007f71
30008064:	30007fcd 	.word	0x30007fcd
30008068:	3000d6fc 	.word	0x3000d6fc

3000806c <ChipInfo_MemorySize>:
3000806c:	b510      	push	{r4, lr}
3000806e:	4c0b      	ldr	r4, [pc, #44]	; (3000809c <ChipInfo_MemorySize+0x30>)
30008070:	4b0b      	ldr	r3, [pc, #44]	; (300080a0 <ChipInfo_MemorySize+0x34>)
30008072:	4798      	blx	r3
30008074:	2100      	movs	r1, #0
30008076:	23e0      	movs	r3, #224	; 0xe0
30008078:	4622      	mov	r2, r4
3000807a:	e004      	b.n	30008086 <ChipInfo_MemorySize+0x1a>
3000807c:	7a13      	ldrb	r3, [r2, #8]
3000807e:	3101      	adds	r1, #1
30008080:	3206      	adds	r2, #6
30008082:	2bff      	cmp	r3, #255	; 0xff
30008084:	d007      	beq.n	30008096 <ChipInfo_MemorySize+0x2a>
30008086:	4298      	cmp	r0, r3
30008088:	d1f8      	bne.n	3000807c <ChipInfo_MemorySize+0x10>
3000808a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
3000808e:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30008092:	7908      	ldrb	r0, [r1, #4]
30008094:	bd10      	pop	{r4, pc}
30008096:	f7ff ff57 	bl	30007f48 <ChipInfo_Invalid>
3000809a:	bf00      	nop
3000809c:	3000d6fc 	.word	0x3000d6fc
300080a0:	30007f71 	.word	0x30007f71

300080a4 <ChipInfo_MemoryVendor>:
300080a4:	b510      	push	{r4, lr}
300080a6:	4c0f      	ldr	r4, [pc, #60]	; (300080e4 <ChipInfo_MemoryVendor+0x40>)
300080a8:	47a0      	blx	r4
300080aa:	2803      	cmp	r0, #3
300080ac:	d016      	beq.n	300080dc <ChipInfo_MemoryVendor+0x38>
300080ae:	47a0      	blx	r4
300080b0:	2802      	cmp	r0, #2
300080b2:	d012      	beq.n	300080da <ChipInfo_MemoryVendor+0x36>
300080b4:	4c0c      	ldr	r4, [pc, #48]	; (300080e8 <ChipInfo_MemoryVendor+0x44>)
300080b6:	4b0d      	ldr	r3, [pc, #52]	; (300080ec <ChipInfo_MemoryVendor+0x48>)
300080b8:	4798      	blx	r3
300080ba:	2100      	movs	r1, #0
300080bc:	23e0      	movs	r3, #224	; 0xe0
300080be:	4622      	mov	r2, r4
300080c0:	e004      	b.n	300080cc <ChipInfo_MemoryVendor+0x28>
300080c2:	7a13      	ldrb	r3, [r2, #8]
300080c4:	3101      	adds	r1, #1
300080c6:	3206      	adds	r2, #6
300080c8:	2bff      	cmp	r3, #255	; 0xff
300080ca:	d009      	beq.n	300080e0 <ChipInfo_MemoryVendor+0x3c>
300080cc:	4298      	cmp	r0, r3
300080ce:	d1f8      	bne.n	300080c2 <ChipInfo_MemoryVendor+0x1e>
300080d0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
300080d4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
300080d8:	7948      	ldrb	r0, [r1, #5]
300080da:	bd10      	pop	{r4, pc}
300080dc:	2001      	movs	r0, #1
300080de:	bd10      	pop	{r4, pc}
300080e0:	f7ff ff32 	bl	30007f48 <ChipInfo_Invalid>
300080e4:	0000c0f9 	.word	0x0000c0f9
300080e8:	3000d6fc 	.word	0x3000d6fc
300080ec:	30007f71 	.word	0x30007f71

300080f0 <SDM32K_Enable>:
300080f0:	4a02      	ldr	r2, [pc, #8]	; (300080fc <SDM32K_Enable+0xc>)
300080f2:	4b03      	ldr	r3, [pc, #12]	; (30008100 <SDM32K_Enable+0x10>)
300080f4:	6811      	ldr	r1, [r2, #0]
300080f6:	430b      	orrs	r3, r1
300080f8:	6013      	str	r3, [r2, #0]
300080fa:	4770      	bx	lr
300080fc:	42008e00 	.word	0x42008e00
30008100:	c0060000 	.word	0xc0060000

30008104 <CLK_SWITCH_XTAL>:
30008104:	b508      	push	{r3, lr}
30008106:	2801      	cmp	r0, #1
30008108:	4b0a      	ldr	r3, [pc, #40]	; (30008134 <CLK_SWITCH_XTAL+0x30>)
3000810a:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
3000810e:	d00d      	beq.n	3000812c <CLK_SWITCH_XTAL+0x28>
30008110:	4b09      	ldr	r3, [pc, #36]	; (30008138 <CLK_SWITCH_XTAL+0x34>)
30008112:	4313      	orrs	r3, r2
30008114:	4907      	ldr	r1, [pc, #28]	; (30008134 <CLK_SWITCH_XTAL+0x30>)
30008116:	4a09      	ldr	r2, [pc, #36]	; (3000813c <CLK_SWITCH_XTAL+0x38>)
30008118:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
3000811c:	4790      	blx	r2
3000811e:	4a08      	ldr	r2, [pc, #32]	; (30008140 <CLK_SWITCH_XTAL+0x3c>)
30008120:	4b08      	ldr	r3, [pc, #32]	; (30008144 <CLK_SWITCH_XTAL+0x40>)
30008122:	fba2 2000 	umull	r2, r0, r2, r0
30008126:	0c80      	lsrs	r0, r0, #18
30008128:	6018      	str	r0, [r3, #0]
3000812a:	bd08      	pop	{r3, pc}
3000812c:	4b06      	ldr	r3, [pc, #24]	; (30008148 <CLK_SWITCH_XTAL+0x44>)
3000812e:	4013      	ands	r3, r2
30008130:	e7f0      	b.n	30008114 <CLK_SWITCH_XTAL+0x10>
30008132:	bf00      	nop
30008134:	42008000 	.word	0x42008000
30008138:	80000500 	.word	0x80000500
3000813c:	30009e69 	.word	0x30009e69
30008140:	431bde83 	.word	0x431bde83
30008144:	2001c700 	.word	0x2001c700
30008148:	7ffffaff 	.word	0x7ffffaff

3000814c <DDR_PHY_ChipInfo.part.0>:
3000814c:	b508      	push	{r3, lr}
3000814e:	4b24      	ldr	r3, [pc, #144]	; (300081e0 <DDR_PHY_ChipInfo.part.0+0x94>)
30008150:	4798      	blx	r3
30008152:	280e      	cmp	r0, #14
30008154:	d80d      	bhi.n	30008172 <DDR_PHY_ChipInfo.part.0+0x26>
30008156:	2801      	cmp	r0, #1
30008158:	d912      	bls.n	30008180 <DDR_PHY_ChipInfo.part.0+0x34>
3000815a:	1e83      	subs	r3, r0, #2
3000815c:	2b0c      	cmp	r3, #12
3000815e:	d80f      	bhi.n	30008180 <DDR_PHY_ChipInfo.part.0+0x34>
30008160:	e8df f003 	tbb	[pc, r3]
30008164:	210e1d19 	.word	0x210e1d19
30008168:	2a0e260e 	.word	0x2a0e260e
3000816c:	340e0e2f 	.word	0x340e0e2f
30008170:	39          	.byte	0x39
30008171:	00          	.byte	0x00
30008172:	28e0      	cmp	r0, #224	; 0xe0
30008174:	d104      	bne.n	30008180 <DDR_PHY_ChipInfo.part.0+0x34>
30008176:	2301      	movs	r3, #1
30008178:	4a1a      	ldr	r2, [pc, #104]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
3000817a:	4618      	mov	r0, r3
3000817c:	7013      	strb	r3, [r2, #0]
3000817e:	bd08      	pop	{r3, pc}
30008180:	4b19      	ldr	r3, [pc, #100]	; (300081e8 <DDR_PHY_ChipInfo.part.0+0x9c>)
30008182:	2002      	movs	r0, #2
30008184:	4a19      	ldr	r2, [pc, #100]	; (300081ec <DDR_PHY_ChipInfo.part.0+0xa0>)
30008186:	4619      	mov	r1, r3
30008188:	f004 f83e 	bl	3000c208 <rtk_log_write>
3000818c:	2300      	movs	r3, #0
3000818e:	4a15      	ldr	r2, [pc, #84]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
30008190:	4618      	mov	r0, r3
30008192:	7013      	strb	r3, [r2, #0]
30008194:	bd08      	pop	{r3, pc}
30008196:	4b13      	ldr	r3, [pc, #76]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
30008198:	2202      	movs	r2, #2
3000819a:	701a      	strb	r2, [r3, #0]
3000819c:	bd08      	pop	{r3, pc}
3000819e:	4b11      	ldr	r3, [pc, #68]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081a0:	2203      	movs	r2, #3
300081a2:	701a      	strb	r2, [r3, #0]
300081a4:	bd08      	pop	{r3, pc}
300081a6:	2304      	movs	r3, #4
300081a8:	4a0e      	ldr	r2, [pc, #56]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081aa:	4618      	mov	r0, r3
300081ac:	7013      	strb	r3, [r2, #0]
300081ae:	bd08      	pop	{r3, pc}
300081b0:	4b0c      	ldr	r3, [pc, #48]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081b2:	2207      	movs	r2, #7
300081b4:	701a      	strb	r2, [r3, #0]
300081b6:	bd08      	pop	{r3, pc}
300081b8:	2305      	movs	r3, #5
300081ba:	4a0a      	ldr	r2, [pc, #40]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081bc:	4618      	mov	r0, r3
300081be:	7013      	strb	r3, [r2, #0]
300081c0:	bd08      	pop	{r3, pc}
300081c2:	2306      	movs	r3, #6
300081c4:	4a07      	ldr	r2, [pc, #28]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081c6:	4618      	mov	r0, r3
300081c8:	7013      	strb	r3, [r2, #0]
300081ca:	bd08      	pop	{r3, pc}
300081cc:	2308      	movs	r3, #8
300081ce:	4a05      	ldr	r2, [pc, #20]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081d0:	4618      	mov	r0, r3
300081d2:	7013      	strb	r3, [r2, #0]
300081d4:	bd08      	pop	{r3, pc}
300081d6:	2309      	movs	r3, #9
300081d8:	4a02      	ldr	r2, [pc, #8]	; (300081e4 <DDR_PHY_ChipInfo.part.0+0x98>)
300081da:	4618      	mov	r0, r3
300081dc:	7013      	strb	r3, [r2, #0]
300081de:	bd08      	pop	{r3, pc}
300081e0:	30007f71 	.word	0x30007f71
300081e4:	3000f51d 	.word	0x3000f51d
300081e8:	3000d780 	.word	0x3000d780
300081ec:	3000d788 	.word	0x3000d788

300081f0 <DDR_PHY_ChipInfo_ddrtype>:
300081f0:	b510      	push	{r4, lr}
300081f2:	4c04      	ldr	r4, [pc, #16]	; (30008204 <DDR_PHY_ChipInfo_ddrtype+0x14>)
300081f4:	7820      	ldrb	r0, [r4, #0]
300081f6:	b100      	cbz	r0, 300081fa <DDR_PHY_ChipInfo_ddrtype+0xa>
300081f8:	bd10      	pop	{r4, pc}
300081fa:	4b03      	ldr	r3, [pc, #12]	; (30008208 <DDR_PHY_ChipInfo_ddrtype+0x18>)
300081fc:	4798      	blx	r3
300081fe:	7020      	strb	r0, [r4, #0]
30008200:	bd10      	pop	{r4, pc}
30008202:	bf00      	nop
30008204:	30010160 	.word	0x30010160
30008208:	30008021 	.word	0x30008021

3000820c <DDR_PHY_StructInit>:
3000820c:	f240 2315 	movw	r3, #533	; 0x215
30008210:	6583      	str	r3, [r0, #88]	; 0x58
30008212:	4770      	bx	lr

30008214 <DDR_PHY_DLL_CLK_DIV>:
30008214:	b570      	push	{r4, r5, r6, lr}
30008216:	4d5a      	ldr	r5, [pc, #360]	; (30008380 <DDR_PHY_DLL_CLK_DIV+0x16c>)
30008218:	495a      	ldr	r1, [pc, #360]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000821a:	fba5 4200 	umull	r4, r2, r5, r0
3000821e:	4b5a      	ldr	r3, [pc, #360]	; (30008388 <DDR_PHY_DLL_CLK_DIV+0x174>)
30008220:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30008222:	0952      	lsrs	r2, r2, #5
30008224:	4c59      	ldr	r4, [pc, #356]	; (3000838c <DDR_PHY_DLL_CLK_DIV+0x178>)
30008226:	4403      	add	r3, r0
30008228:	4859      	ldr	r0, [pc, #356]	; (30008390 <DDR_PHY_DLL_CLK_DIV+0x17c>)
3000822a:	3a03      	subs	r2, #3
3000822c:	4034      	ands	r4, r6
3000822e:	eb02 0682 	add.w	r6, r2, r2, lsl #2
30008232:	eba3 03c6 	sub.w	r3, r3, r6, lsl #3
30008236:	02db      	lsls	r3, r3, #11
30008238:	fba5 5303 	umull	r5, r3, r5, r3
3000823c:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30008240:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008244:	4320      	orrs	r0, r4
30008246:	0214      	lsls	r4, r2, #8
30008248:	b2d2      	uxtb	r2, r2
3000824a:	6248      	str	r0, [r1, #36]	; 0x24
3000824c:	b2a4      	uxth	r4, r4
3000824e:	6a88      	ldr	r0, [r1, #40]	; 0x28
30008250:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30008254:	4304      	orrs	r4, r0
30008256:	484f      	ldr	r0, [pc, #316]	; (30008394 <DDR_PHY_DLL_CLK_DIV+0x180>)
30008258:	628c      	str	r4, [r1, #40]	; 0x28
3000825a:	6a4c      	ldr	r4, [r1, #36]	; 0x24
3000825c:	4020      	ands	r0, r4
3000825e:	2401      	movs	r4, #1
30008260:	4303      	orrs	r3, r0
30008262:	624b      	str	r3, [r1, #36]	; 0x24
30008264:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30008266:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
3000826a:	431a      	orrs	r2, r3
3000826c:	4b4a      	ldr	r3, [pc, #296]	; (30008398 <DDR_PHY_DLL_CLK_DIV+0x184>)
3000826e:	628a      	str	r2, [r1, #40]	; 0x28
30008270:	678c      	str	r4, [r1, #120]	; 0x78
30008272:	7818      	ldrb	r0, [r3, #0]
30008274:	28ff      	cmp	r0, #255	; 0xff
30008276:	d076      	beq.n	30008366 <DDR_PHY_DLL_CLK_DIV+0x152>
30008278:	4c48      	ldr	r4, [pc, #288]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x188>)
3000827a:	0145      	lsls	r5, r0, #5
3000827c:	4b41      	ldr	r3, [pc, #260]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000827e:	eb04 1040 	add.w	r0, r4, r0, lsl #5
30008282:	5962      	ldr	r2, [r4, r5]
30008284:	4e46      	ldr	r6, [pc, #280]	; (300083a0 <DDR_PHY_DLL_CLK_DIV+0x18c>)
30008286:	611a      	str	r2, [r3, #16]
30008288:	6842      	ldr	r2, [r0, #4]
3000828a:	615a      	str	r2, [r3, #20]
3000828c:	6882      	ldr	r2, [r0, #8]
3000828e:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
30008292:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
30008296:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
3000829a:	609a      	str	r2, [r3, #8]
3000829c:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300082a0:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300082a4:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300082a8:	7830      	ldrb	r0, [r6, #0]
300082aa:	2800      	cmp	r0, #0
300082ac:	d057      	beq.n	3000835e <DDR_PHY_DLL_CLK_DIV+0x14a>
300082ae:	2801      	cmp	r0, #1
300082b0:	eb04 0305 	add.w	r3, r4, r5
300082b4:	4a33      	ldr	r2, [pc, #204]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
300082b6:	d037      	beq.n	30008328 <DDR_PHY_DLL_CLK_DIV+0x114>
300082b8:	2122      	movs	r1, #34	; 0x22
300082ba:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300082be:	6959      	ldr	r1, [r3, #20]
300082c0:	f8c2 1080 	str.w	r1, [r2, #128]	; 0x80
300082c4:	e9d3 1306 	ldrd	r1, r3, [r3, #24]
300082c8:	f8c2 1084 	str.w	r1, [r2, #132]	; 0x84
300082cc:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
300082d0:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300082d4:	4933      	ldr	r1, [pc, #204]	; (300083a4 <DDR_PHY_DLL_CLK_DIV+0x190>)
300082d6:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300082da:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300082de:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300082e2:	009b      	lsls	r3, r3, #2
300082e4:	d516      	bpl.n	30008314 <DDR_PHY_DLL_CLK_DIV+0x100>
300082e6:	2800      	cmp	r0, #0
300082e8:	d044      	beq.n	30008374 <DDR_PHY_DLL_CLK_DIV+0x160>
300082ea:	4b26      	ldr	r3, [pc, #152]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
300082ec:	4a2e      	ldr	r2, [pc, #184]	; (300083a8 <DDR_PHY_DLL_CLK_DIV+0x194>)
300082ee:	601a      	str	r2, [r3, #0]
300082f0:	4b24      	ldr	r3, [pc, #144]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
300082f2:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300082f6:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
300082fa:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
300082fe:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008302:	699a      	ldr	r2, [r3, #24]
30008304:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30008308:	619a      	str	r2, [r3, #24]
3000830a:	699a      	ldr	r2, [r3, #24]
3000830c:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008310:	619a      	str	r2, [r3, #24]
30008312:	bd70      	pop	{r4, r5, r6, pc}
30008314:	b920      	cbnz	r0, 30008320 <DDR_PHY_DLL_CLK_DIV+0x10c>
30008316:	4b25      	ldr	r3, [pc, #148]	; (300083ac <DDR_PHY_DLL_CLK_DIV+0x198>)
30008318:	4798      	blx	r3
3000831a:	2801      	cmp	r0, #1
3000831c:	7030      	strb	r0, [r6, #0]
3000831e:	d025      	beq.n	3000836c <DDR_PHY_DLL_CLK_DIV+0x158>
30008320:	4b18      	ldr	r3, [pc, #96]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008322:	4a23      	ldr	r2, [pc, #140]	; (300083b0 <DDR_PHY_DLL_CLK_DIV+0x19c>)
30008324:	601a      	str	r2, [r3, #0]
30008326:	e7e3      	b.n	300082f0 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008328:	2033      	movs	r0, #51	; 0x33
3000832a:	695c      	ldr	r4, [r3, #20]
3000832c:	491d      	ldr	r1, [pc, #116]	; (300083a4 <DDR_PHY_DLL_CLK_DIV+0x190>)
3000832e:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
30008332:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008336:	e9d3 0306 	ldrd	r0, r3, [r3, #24]
3000833a:	f8c2 0084 	str.w	r0, [r2, #132]	; 0x84
3000833e:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
30008342:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008346:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
3000834a:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000834e:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008352:	009a      	lsls	r2, r3, #2
30008354:	d50a      	bpl.n	3000836c <DDR_PHY_DLL_CLK_DIV+0x158>
30008356:	4b0b      	ldr	r3, [pc, #44]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008358:	4a16      	ldr	r2, [pc, #88]	; (300083b4 <DDR_PHY_DLL_CLK_DIV+0x1a0>)
3000835a:	601a      	str	r2, [r3, #0]
3000835c:	e7c8      	b.n	300082f0 <DDR_PHY_DLL_CLK_DIV+0xdc>
3000835e:	4b13      	ldr	r3, [pc, #76]	; (300083ac <DDR_PHY_DLL_CLK_DIV+0x198>)
30008360:	4798      	blx	r3
30008362:	7030      	strb	r0, [r6, #0]
30008364:	e7a3      	b.n	300082ae <DDR_PHY_DLL_CLK_DIV+0x9a>
30008366:	f7ff fef1 	bl	3000814c <DDR_PHY_ChipInfo.part.0>
3000836a:	e785      	b.n	30008278 <DDR_PHY_DLL_CLK_DIV+0x64>
3000836c:	4b05      	ldr	r3, [pc, #20]	; (30008384 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000836e:	4a12      	ldr	r2, [pc, #72]	; (300083b8 <DDR_PHY_DLL_CLK_DIV+0x1a4>)
30008370:	601a      	str	r2, [r3, #0]
30008372:	e7bd      	b.n	300082f0 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008374:	4b0d      	ldr	r3, [pc, #52]	; (300083ac <DDR_PHY_DLL_CLK_DIV+0x198>)
30008376:	4798      	blx	r3
30008378:	2801      	cmp	r0, #1
3000837a:	7030      	strb	r0, [r6, #0]
3000837c:	d1b5      	bne.n	300082ea <DDR_PHY_DLL_CLK_DIV+0xd6>
3000837e:	e7ea      	b.n	30008356 <DDR_PHY_DLL_CLK_DIV+0x142>
30008380:	cccccccd 	.word	0xcccccccd
30008384:	41011000 	.word	0x41011000
30008388:	001fff88 	.word	0x001fff88
3000838c:	f800ffff 	.word	0xf800ffff
30008390:	07ff0000 	.word	0x07ff0000
30008394:	fffff800 	.word	0xfffff800
30008398:	3000f51d 	.word	0x3000f51d
3000839c:	3000d7a8 	.word	0x3000d7a8
300083a0:	30010160 	.word	0x30010160
300083a4:	42008000 	.word	0x42008000
300083a8:	4c000821 	.word	0x4c000821
300083ac:	30008021 	.word	0x30008021
300083b0:	40000021 	.word	0x40000021
300083b4:	4c000903 	.word	0x4c000903
300083b8:	40000103 	.word	0x40000103

300083bc <DDR_PHY_CRT_Init>:
300083bc:	492f      	ldr	r1, [pc, #188]	; (3000847c <DDR_PHY_CRT_Init+0xc0>)
300083be:	2005      	movs	r0, #5
300083c0:	4a2f      	ldr	r2, [pc, #188]	; (30008480 <DDR_PHY_CRT_Init+0xc4>)
300083c2:	b510      	push	{r4, lr}
300083c4:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
300083c8:	4c2e      	ldr	r4, [pc, #184]	; (30008484 <DDR_PHY_CRT_Init+0xc8>)
300083ca:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300083ce:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
300083d2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300083d6:	f043 0301 	orr.w	r3, r3, #1
300083da:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300083de:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300083e2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300083e6:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300083ea:	68e3      	ldr	r3, [r4, #12]
300083ec:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300083f0:	60e3      	str	r3, [r4, #12]
300083f2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300083f6:	f043 0302 	orr.w	r3, r3, #2
300083fa:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300083fe:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008402:	f023 0308 	bic.w	r3, r3, #8
30008406:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000840a:	4790      	blx	r2
3000840c:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008410:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008414:	491c      	ldr	r1, [pc, #112]	; (30008488 <DDR_PHY_CRT_Init+0xcc>)
30008416:	f042 0204 	orr.w	r2, r2, #4
3000841a:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000841e:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008422:	f042 0208 	orr.w	r2, r2, #8
30008426:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000842a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000842c:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
30008430:	62a2      	str	r2, [r4, #40]	; 0x28
30008432:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008434:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30008438:	62a2      	str	r2, [r4, #40]	; 0x28
3000843a:	69e2      	ldr	r2, [r4, #28]
3000843c:	f042 0208 	orr.w	r2, r2, #8
30008440:	61e2      	str	r2, [r4, #28]
30008442:	68a2      	ldr	r2, [r4, #8]
30008444:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008448:	60a2      	str	r2, [r4, #8]
3000844a:	6862      	ldr	r2, [r4, #4]
3000844c:	4313      	orrs	r3, r2
3000844e:	6063      	str	r3, [r4, #4]
30008450:	68a3      	ldr	r3, [r4, #8]
30008452:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008456:	60a3      	str	r3, [r4, #8]
30008458:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
3000845c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008460:	f023 0303 	bic.w	r3, r3, #3
30008464:	f043 0302 	orr.w	r3, r3, #2
30008468:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000846c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008470:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30008474:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008478:	bd10      	pop	{r4, pc}
3000847a:	bf00      	nop
3000847c:	42008000 	.word	0x42008000
30008480:	00009b2d 	.word	0x00009b2d
30008484:	41011000 	.word	0x41011000
30008488:	90161f00 	.word	0x90161f00

3000848c <DDR_PHY_R240_ZQ_CAL>:
3000848c:	b538      	push	{r3, r4, r5, lr}
3000848e:	4b1e      	ldr	r3, [pc, #120]	; (30008508 <DDR_PHY_R240_ZQ_CAL+0x7c>)
30008490:	7818      	ldrb	r0, [r3, #0]
30008492:	28ff      	cmp	r0, #255	; 0xff
30008494:	d034      	beq.n	30008500 <DDR_PHY_R240_ZQ_CAL+0x74>
30008496:	4a1d      	ldr	r2, [pc, #116]	; (3000850c <DDR_PHY_R240_ZQ_CAL+0x80>)
30008498:	eb00 0080 	add.w	r0, r0, r0, lsl #2
3000849c:	4b1c      	ldr	r3, [pc, #112]	; (30008510 <DDR_PHY_R240_ZQ_CAL+0x84>)
3000849e:	2400      	movs	r4, #0
300084a0:	eb02 0180 	add.w	r1, r2, r0, lsl #2
300084a4:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
300084a8:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
300084ac:	7c0a      	ldrb	r2, [r1, #16]
300084ae:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
300084b2:	0052      	lsls	r2, r2, #1
300084b4:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300084b8:	4302      	orrs	r2, r0
300084ba:	6848      	ldr	r0, [r1, #4]
300084bc:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300084c0:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
300084c4:	4a13      	ldr	r2, [pc, #76]	; (30008514 <DDR_PHY_R240_ZQ_CAL+0x88>)
300084c6:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300084ca:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300084ce:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300084d2:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
300084d6:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
300084da:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300084de:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
300084e2:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300084e6:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300084ea:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300084ee:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300084f2:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300084f6:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
300084fa:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300084fe:	bd38      	pop	{r3, r4, r5, pc}
30008500:	f7ff fe24 	bl	3000814c <DDR_PHY_ChipInfo.part.0>
30008504:	e7c7      	b.n	30008496 <DDR_PHY_R240_ZQ_CAL+0xa>
30008506:	bf00      	nop
30008508:	3000f51d 	.word	0x3000f51d
3000850c:	3000d8e8 	.word	0x3000d8e8
30008510:	41011000 	.word	0x41011000
30008514:	00262626 	.word	0x00262626

30008518 <DDR_PHY_ZQ_SET_SEL>:
30008518:	b410      	push	{r4}
3000851a:	4b25      	ldr	r3, [pc, #148]	; (300085b0 <DDR_PHY_ZQ_SET_SEL+0x98>)
3000851c:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008520:	4a24      	ldr	r2, [pc, #144]	; (300085b4 <DDR_PHY_ZQ_SET_SEL+0x9c>)
30008522:	4c25      	ldr	r4, [pc, #148]	; (300085b8 <DDR_PHY_ZQ_SET_SEL+0xa0>)
30008524:	4925      	ldr	r1, [pc, #148]	; (300085bc <DDR_PHY_ZQ_SET_SEL+0xa4>)
30008526:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
3000852a:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000852e:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30008532:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30008536:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
3000853a:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
3000853e:	f64f 74ff 	movw	r4, #65535	; 0xffff
30008542:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30008546:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
3000854a:	2200      	movs	r2, #0
3000854c:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008550:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30008554:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008558:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
3000855c:	f04f 31ff 	mov.w	r1, #4294967295
30008560:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30008564:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008568:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
3000856c:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008570:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30008574:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008578:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
3000857c:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008580:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30008584:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30008588:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
3000858c:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30008590:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30008594:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30008598:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
3000859c:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
300085a0:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
300085a4:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
300085a8:	f85d 4b04 	ldr.w	r4, [sp], #4
300085ac:	4770      	bx	lr
300085ae:	bf00      	nop
300085b0:	41011000 	.word	0x41011000
300085b4:	ffff1111 	.word	0xffff1111
300085b8:	ffff3333 	.word	0xffff3333
300085bc:	ffff0000 	.word	0xffff0000

300085c0 <DDR_PHY_DELAY_TAP_SET>:
300085c0:	b510      	push	{r4, lr}
300085c2:	4b15      	ldr	r3, [pc, #84]	; (30008618 <DDR_PHY_DELAY_TAP_SET+0x58>)
300085c4:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300085c8:	4914      	ldr	r1, [pc, #80]	; (3000861c <DDR_PHY_DELAY_TAP_SET+0x5c>)
300085ca:	4c15      	ldr	r4, [pc, #84]	; (30008620 <DDR_PHY_DELAY_TAP_SET+0x60>)
300085cc:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300085d0:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
300085d4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300085d8:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300085dc:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300085e0:	7820      	ldrb	r0, [r4, #0]
300085e2:	b1a0      	cbz	r0, 3000860e <DDR_PHY_DELAY_TAP_SET+0x4e>
300085e4:	2801      	cmp	r0, #1
300085e6:	4b0c      	ldr	r3, [pc, #48]	; (30008618 <DDR_PHY_DELAY_TAP_SET+0x58>)
300085e8:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
300085ec:	bf0c      	ite	eq
300085ee:	4a0d      	ldreq	r2, [pc, #52]	; (30008624 <DDR_PHY_DELAY_TAP_SET+0x64>)
300085f0:	4a0d      	ldrne	r2, [pc, #52]	; (30008628 <DDR_PHY_DELAY_TAP_SET+0x68>)
300085f2:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
300085f6:	f640 0288 	movw	r2, #2184	; 0x888
300085fa:	4b07      	ldr	r3, [pc, #28]	; (30008618 <DDR_PHY_DELAY_TAP_SET+0x58>)
300085fc:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008600:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30008604:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008608:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000860c:	bd10      	pop	{r4, pc}
3000860e:	4b07      	ldr	r3, [pc, #28]	; (3000862c <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008610:	4798      	blx	r3
30008612:	7020      	strb	r0, [r4, #0]
30008614:	e7e6      	b.n	300085e4 <DDR_PHY_DELAY_TAP_SET+0x24>
30008616:	bf00      	nop
30008618:	41011000 	.word	0x41011000
3000861c:	20888888 	.word	0x20888888
30008620:	30010160 	.word	0x30010160
30008624:	7c033333 	.word	0x7c033333
30008628:	7c011111 	.word	0x7c011111
3000862c:	30008021 	.word	0x30008021

30008630 <DDR_PHY_READ_CTRL>:
30008630:	4b10      	ldr	r3, [pc, #64]	; (30008674 <DDR_PHY_READ_CTRL+0x44>)
30008632:	b510      	push	{r4, lr}
30008634:	7818      	ldrb	r0, [r3, #0]
30008636:	28ff      	cmp	r0, #255	; 0xff
30008638:	d019      	beq.n	3000866e <DDR_PHY_READ_CTRL+0x3e>
3000863a:	4a0f      	ldr	r2, [pc, #60]	; (30008678 <DDR_PHY_READ_CTRL+0x48>)
3000863c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008640:	4b0e      	ldr	r3, [pc, #56]	; (3000867c <DDR_PHY_READ_CTRL+0x4c>)
30008642:	2108      	movs	r1, #8
30008644:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30008648:	22fe      	movs	r2, #254	; 0xfe
3000864a:	7c44      	ldrb	r4, [r0, #17]
3000864c:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30008650:	7c84      	ldrb	r4, [r0, #18]
30008652:	7cc0      	ldrb	r0, [r0, #19]
30008654:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30008658:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
3000865c:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008660:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30008664:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008668:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000866c:	bd10      	pop	{r4, pc}
3000866e:	f7ff fd6d 	bl	3000814c <DDR_PHY_ChipInfo.part.0>
30008672:	e7e2      	b.n	3000863a <DDR_PHY_READ_CTRL+0xa>
30008674:	3000f51d 	.word	0x3000f51d
30008678:	3000d8e8 	.word	0x3000d8e8
3000867c:	41011000 	.word	0x41011000

30008680 <DDR_PHY_READ_LEVELING>:
30008680:	b508      	push	{r3, lr}
30008682:	4b21      	ldr	r3, [pc, #132]	; (30008708 <DDR_PHY_READ_LEVELING+0x88>)
30008684:	7818      	ldrb	r0, [r3, #0]
30008686:	28ff      	cmp	r0, #255	; 0xff
30008688:	d03b      	beq.n	30008702 <DDR_PHY_READ_LEVELING+0x82>
3000868a:	4b20      	ldr	r3, [pc, #128]	; (3000870c <DDR_PHY_READ_LEVELING+0x8c>)
3000868c:	b1e8      	cbz	r0, 300086ca <DDR_PHY_READ_LEVELING+0x4a>
3000868e:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30008692:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008696:	220f      	movs	r2, #15
30008698:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
3000869c:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300086a0:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300086a4:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300086a8:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300086ac:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300086b0:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300086b4:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
300086b8:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300086bc:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300086c0:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300086c4:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300086c8:	bd08      	pop	{r3, pc}
300086ca:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300086ce:	2110      	movs	r1, #16
300086d0:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300086d4:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300086d8:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300086dc:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300086e0:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
300086e4:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
300086e8:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
300086ec:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
300086f0:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
300086f4:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
300086f8:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
300086fc:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008700:	bd08      	pop	{r3, pc}
30008702:	f7ff fd23 	bl	3000814c <DDR_PHY_ChipInfo.part.0>
30008706:	e7c0      	b.n	3000868a <DDR_PHY_READ_LEVELING+0xa>
30008708:	3000f51d 	.word	0x3000f51d
3000870c:	41011000 	.word	0x41011000

30008710 <DDR_PHY_Init>:
30008710:	b510      	push	{r4, lr}
30008712:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008714:	4c2f      	ldr	r4, [pc, #188]	; (300087d4 <DDR_PHY_Init+0xc4>)
30008716:	f7ff fd7d 	bl	30008214 <DDR_PHY_DLL_CLK_DIV>
3000871a:	f7ff fe4f 	bl	300083bc <DDR_PHY_CRT_Init>
3000871e:	f7ff feb5 	bl	3000848c <DDR_PHY_R240_ZQ_CAL>
30008722:	f7ff fef9 	bl	30008518 <DDR_PHY_ZQ_SET_SEL>
30008726:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
3000872a:	2300      	movs	r3, #0
3000872c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008730:	4829      	ldr	r0, [pc, #164]	; (300087d8 <DDR_PHY_Init+0xc8>)
30008732:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008736:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
3000873a:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
3000873e:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30008742:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008746:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
3000874a:	f7ff ff39 	bl	300085c0 <DDR_PHY_DELAY_TAP_SET>
3000874e:	f7ff ff6f 	bl	30008630 <DDR_PHY_READ_CTRL>
30008752:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008756:	4a21      	ldr	r2, [pc, #132]	; (300087dc <DDR_PHY_Init+0xcc>)
30008758:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
3000875c:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008760:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008764:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008768:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
3000876c:	f7ff ff88 	bl	30008680 <DDR_PHY_READ_LEVELING>
30008770:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008774:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008778:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
3000877c:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008780:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008784:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008788:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
3000878c:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008790:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30008794:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30008798:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
3000879c:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300087a0:	f043 0303 	orr.w	r3, r3, #3
300087a4:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300087a8:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300087ac:	f043 030c 	orr.w	r3, r3, #12
300087b0:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300087b4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300087b8:	f023 0303 	bic.w	r3, r3, #3
300087bc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300087c0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300087c4:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300087c8:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300087cc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300087d0:	bd10      	pop	{r4, pc}
300087d2:	bf00      	nop
300087d4:	41011000 	.word	0x41011000
300087d8:	00045500 	.word	0x00045500
300087dc:	11117777 	.word	0x11117777

300087e0 <DDR_PHY_CAL>:
300087e0:	4770      	bx	lr
300087e2:	bf00      	nop

300087e4 <DDR_PHY_AutoGating>:
300087e4:	4a0c      	ldr	r2, [pc, #48]	; (30008818 <DDR_PHY_AutoGating+0x34>)
300087e6:	2032      	movs	r0, #50	; 0x32
300087e8:	490c      	ldr	r1, [pc, #48]	; (3000881c <DDR_PHY_AutoGating+0x38>)
300087ea:	b508      	push	{r3, lr}
300087ec:	6893      	ldr	r3, [r2, #8]
300087ee:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
300087f2:	6093      	str	r3, [r2, #8]
300087f4:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
300087f8:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
300087fc:	f043 0314 	orr.w	r3, r3, #20
30008800:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008804:	4788      	blx	r1
30008806:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000880a:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000880e:	f043 0303 	orr.w	r3, r3, #3
30008812:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008816:	bd08      	pop	{r3, pc}
30008818:	41011000 	.word	0x41011000
3000881c:	00009b2d 	.word	0x00009b2d

30008820 <flash_init_userdef>:
30008820:	4b14      	ldr	r3, [pc, #80]	; (30008874 <flash_init_userdef+0x54>)
30008822:	2201      	movs	r2, #1
30008824:	2140      	movs	r1, #64	; 0x40
30008826:	f44f 6000 	mov.w	r0, #2048	; 0x800
3000882a:	b430      	push	{r4, r5}
3000882c:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008830:	2502      	movs	r5, #2
30008832:	2400      	movs	r4, #0
30008834:	f640 0104 	movw	r1, #2052	; 0x804
30008838:	601a      	str	r2, [r3, #0]
3000883a:	22bb      	movs	r2, #187	; 0xbb
3000883c:	83d8      	strh	r0, [r3, #30]
3000883e:	8419      	strh	r1, [r3, #32]
30008840:	203b      	movs	r0, #59	; 0x3b
30008842:	21eb      	movs	r1, #235	; 0xeb
30008844:	629a      	str	r2, [r3, #40]	; 0x28
30008846:	4a0c      	ldr	r2, [pc, #48]	; (30008878 <flash_init_userdef+0x58>)
30008848:	6258      	str	r0, [r3, #36]	; 0x24
3000884a:	6319      	str	r1, [r3, #48]	; 0x30
3000884c:	f64a 3120 	movw	r1, #43808	; 0xab20
30008850:	655a      	str	r2, [r3, #84]	; 0x54
30008852:	22b9      	movs	r2, #185	; 0xb9
30008854:	4809      	ldr	r0, [pc, #36]	; (3000887c <flash_init_userdef+0x5c>)
30008856:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
3000885a:	6598      	str	r0, [r3, #88]	; 0x58
3000885c:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008860:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008864:	2406      	movs	r4, #6
30008866:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
3000886a:	246b      	movs	r4, #107	; 0x6b
3000886c:	62dc      	str	r4, [r3, #44]	; 0x2c
3000886e:	bc30      	pop	{r4, r5}
30008870:	4770      	bx	lr
30008872:	bf00      	nop
30008874:	2001c01c 	.word	0x2001c01c
30008878:	00059f06 	.word	0x00059f06
3000887c:	d8600001 	.word	0xd8600001

30008880 <nand_init_userdef>:
30008880:	4b18      	ldr	r3, [pc, #96]	; (300088e4 <nand_init_userdef+0x64>)
30008882:	2001      	movs	r0, #1
30008884:	2200      	movs	r2, #0
30008886:	2102      	movs	r1, #2
30008888:	b410      	push	{r4}
3000888a:	6018      	str	r0, [r3, #0]
3000888c:	f640 0408 	movw	r4, #2056	; 0x808
30008890:	6159      	str	r1, [r3, #20]
30008892:	83dc      	strh	r4, [r3, #30]
30008894:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008898:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
3000889c:	841c      	strh	r4, [r3, #32]
3000889e:	246b      	movs	r4, #107	; 0x6b
300088a0:	619a      	str	r2, [r3, #24]
300088a2:	62dc      	str	r4, [r3, #44]	; 0x2c
300088a4:	4c10      	ldr	r4, [pc, #64]	; (300088e8 <nand_init_userdef+0x68>)
300088a6:	775a      	strb	r2, [r3, #29]
300088a8:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
300088ac:	629a      	str	r2, [r3, #40]	; 0x28
300088ae:	631a      	str	r2, [r3, #48]	; 0x30
300088b0:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
300088b4:	65da      	str	r2, [r3, #92]	; 0x5c
300088b6:	e9c3 0003 	strd	r0, r0, [r3, #12]
300088ba:	203b      	movs	r0, #59	; 0x3b
300088bc:	6258      	str	r0, [r3, #36]	; 0x24
300088be:	200b      	movs	r0, #11
300088c0:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
300088c4:	6698      	str	r0, [r3, #104]	; 0x68
300088c6:	2008      	movs	r0, #8
300088c8:	4908      	ldr	r1, [pc, #32]	; (300088ec <nand_init_userdef+0x6c>)
300088ca:	64d8      	str	r0, [r3, #76]	; 0x4c
300088cc:	f241 0013 	movw	r0, #4115	; 0x1013
300088d0:	6599      	str	r1, [r3, #88]	; 0x58
300088d2:	4907      	ldr	r1, [pc, #28]	; (300088f0 <nand_init_userdef+0x70>)
300088d4:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
300088d8:	6719      	str	r1, [r3, #112]	; 0x70
300088da:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
300088de:	f85d 4b04 	ldr.w	r4, [sp], #4
300088e2:	4770      	bx	lr
300088e4:	2001c01c 	.word	0x2001c01c
300088e8:	000f9f06 	.word	0x000f9f06
300088ec:	d800001f 	.word	0xd800001f
300088f0:	10300804 	.word	0x10300804

300088f4 <flash_layout_init>:
300088f4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300088f8:	4b65      	ldr	r3, [pc, #404]	; (30008a90 <flash_layout_init+0x19c>)
300088fa:	b08f      	sub	sp, #60	; 0x3c
300088fc:	4798      	blx	r3
300088fe:	4b65      	ldr	r3, [pc, #404]	; (30008a94 <flash_layout_init+0x1a0>)
30008900:	4a65      	ldr	r2, [pc, #404]	; (30008a98 <flash_layout_init+0x1a4>)
30008902:	2800      	cmp	r0, #0
30008904:	bf08      	it	eq
30008906:	4613      	moveq	r3, r2
30008908:	681f      	ldr	r7, [r3, #0]
3000890a:	2fff      	cmp	r7, #255	; 0xff
3000890c:	d05a      	beq.n	300089c4 <flash_layout_init+0xd0>
3000890e:	f8df b19c 	ldr.w	fp, [pc, #412]	; 30008aac <flash_layout_init+0x1b8>
30008912:	f04f 0900 	mov.w	r9, #0
30008916:	4a61      	ldr	r2, [pc, #388]	; (30008a9c <flash_layout_init+0x1a8>)
30008918:	f8db 1000 	ldr.w	r1, [fp]
3000891c:	46c8      	mov	r8, r9
3000891e:	f8df e190 	ldr.w	lr, [pc, #400]	; 30008ab0 <flash_layout_init+0x1bc>
30008922:	464e      	mov	r6, r9
30008924:	910d      	str	r1, [sp, #52]	; 0x34
30008926:	464c      	mov	r4, r9
30008928:	495d      	ldr	r1, [pc, #372]	; (30008aa0 <flash_layout_init+0x1ac>)
3000892a:	6812      	ldr	r2, [r2, #0]
3000892c:	6809      	ldr	r1, [r1, #0]
3000892e:	f8df c184 	ldr.w	ip, [pc, #388]	; 30008ab4 <flash_layout_init+0x1c0>
30008932:	9109      	str	r1, [sp, #36]	; 0x24
30008934:	495b      	ldr	r1, [pc, #364]	; (30008aa4 <flash_layout_init+0x1b0>)
30008936:	920c      	str	r2, [sp, #48]	; 0x30
30008938:	6809      	ldr	r1, [r1, #0]
3000893a:	4a5b      	ldr	r2, [pc, #364]	; (30008aa8 <flash_layout_init+0x1b4>)
3000893c:	910b      	str	r1, [sp, #44]	; 0x2c
3000893e:	f8de 1000 	ldr.w	r1, [lr]
30008942:	f8cd 9028 	str.w	r9, [sp, #40]	; 0x28
30008946:	9107      	str	r1, [sp, #28]
30008948:	f89c 1000 	ldrb.w	r1, [ip]
3000894c:	f8cd 9018 	str.w	r9, [sp, #24]
30008950:	9108      	str	r1, [sp, #32]
30008952:	6951      	ldr	r1, [r2, #20]
30008954:	f8cd 9010 	str.w	r9, [sp, #16]
30008958:	9105      	str	r1, [sp, #20]
3000895a:	6911      	ldr	r1, [r2, #16]
3000895c:	f8cd 9008 	str.w	r9, [sp, #8]
30008960:	9103      	str	r1, [sp, #12]
30008962:	68d1      	ldr	r1, [r2, #12]
30008964:	9101      	str	r1, [sp, #4]
30008966:	6891      	ldr	r1, [r2, #8]
30008968:	e9d2 0500 	ldrd	r0, r5, [r2]
3000896c:	9100      	str	r1, [sp, #0]
3000896e:	4649      	mov	r1, r9
30008970:	2f00      	cmp	r7, #0
30008972:	d12a      	bne.n	300089ca <flash_layout_init+0xd6>
30008974:	6858      	ldr	r0, [r3, #4]
30008976:	2101      	movs	r1, #1
30008978:	f853 7f0c 	ldr.w	r7, [r3, #12]!
3000897c:	2fff      	cmp	r7, #255	; 0xff
3000897e:	d1f7      	bne.n	30008970 <flash_layout_init+0x7c>
30008980:	b101      	cbz	r1, 30008984 <flash_layout_init+0x90>
30008982:	6010      	str	r0, [r2, #0]
30008984:	b104      	cbz	r4, 30008988 <flash_layout_init+0x94>
30008986:	6055      	str	r5, [r2, #4]
30008988:	b10e      	cbz	r6, 3000898e <flash_layout_init+0x9a>
3000898a:	9b00      	ldr	r3, [sp, #0]
3000898c:	6093      	str	r3, [r2, #8]
3000898e:	f1b8 0f00 	cmp.w	r8, #0
30008992:	d001      	beq.n	30008998 <flash_layout_init+0xa4>
30008994:	9b01      	ldr	r3, [sp, #4]
30008996:	60d3      	str	r3, [r2, #12]
30008998:	9b02      	ldr	r3, [sp, #8]
3000899a:	b10b      	cbz	r3, 300089a0 <flash_layout_init+0xac>
3000899c:	9b03      	ldr	r3, [sp, #12]
3000899e:	6113      	str	r3, [r2, #16]
300089a0:	9b04      	ldr	r3, [sp, #16]
300089a2:	2b00      	cmp	r3, #0
300089a4:	d169      	bne.n	30008a7a <flash_layout_init+0x186>
300089a6:	9b06      	ldr	r3, [sp, #24]
300089a8:	2b00      	cmp	r3, #0
300089aa:	d15f      	bne.n	30008a6c <flash_layout_init+0x178>
300089ac:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300089ae:	2b00      	cmp	r3, #0
300089b0:	d166      	bne.n	30008a80 <flash_layout_init+0x18c>
300089b2:	f1b9 0f00 	cmp.w	r9, #0
300089b6:	d005      	beq.n	300089c4 <flash_layout_init+0xd0>
300089b8:	4b38      	ldr	r3, [pc, #224]	; (30008a9c <flash_layout_init+0x1a8>)
300089ba:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300089bc:	601a      	str	r2, [r3, #0]
300089be:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300089c0:	f8cb 3000 	str.w	r3, [fp]
300089c4:	b00f      	add	sp, #60	; 0x3c
300089c6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089ca:	2f01      	cmp	r7, #1
300089cc:	d01a      	beq.n	30008a04 <flash_layout_init+0x110>
300089ce:	2f02      	cmp	r7, #2
300089d0:	d01b      	beq.n	30008a0a <flash_layout_init+0x116>
300089d2:	2f03      	cmp	r7, #3
300089d4:	d01d      	beq.n	30008a12 <flash_layout_init+0x11e>
300089d6:	2f04      	cmp	r7, #4
300089d8:	d020      	beq.n	30008a1c <flash_layout_init+0x128>
300089da:	2f05      	cmp	r7, #5
300089dc:	d023      	beq.n	30008a26 <flash_layout_init+0x132>
300089de:	2f06      	cmp	r7, #6
300089e0:	d026      	beq.n	30008a30 <flash_layout_init+0x13c>
300089e2:	2f07      	cmp	r7, #7
300089e4:	d035      	beq.n	30008a52 <flash_layout_init+0x15e>
300089e6:	2f08      	cmp	r7, #8
300089e8:	d1c6      	bne.n	30008978 <flash_layout_init+0x84>
300089ea:	689f      	ldr	r7, [r3, #8]
300089ec:	f8d3 9004 	ldr.w	r9, [r3, #4]
300089f0:	3701      	adds	r7, #1
300089f2:	eba7 0709 	sub.w	r7, r7, r9
300089f6:	970d      	str	r7, [sp, #52]	; 0x34
300089f8:	f109 4778 	add.w	r7, r9, #4160749568	; 0xf8000000
300089fc:	f04f 0901 	mov.w	r9, #1
30008a00:	970c      	str	r7, [sp, #48]	; 0x30
30008a02:	e7b9      	b.n	30008978 <flash_layout_init+0x84>
30008a04:	463c      	mov	r4, r7
30008a06:	685d      	ldr	r5, [r3, #4]
30008a08:	e7b6      	b.n	30008978 <flash_layout_init+0x84>
30008a0a:	685e      	ldr	r6, [r3, #4]
30008a0c:	9600      	str	r6, [sp, #0]
30008a0e:	2601      	movs	r6, #1
30008a10:	e7b2      	b.n	30008978 <flash_layout_init+0x84>
30008a12:	685f      	ldr	r7, [r3, #4]
30008a14:	f04f 0801 	mov.w	r8, #1
30008a18:	9701      	str	r7, [sp, #4]
30008a1a:	e7ad      	b.n	30008978 <flash_layout_init+0x84>
30008a1c:	685f      	ldr	r7, [r3, #4]
30008a1e:	9703      	str	r7, [sp, #12]
30008a20:	2701      	movs	r7, #1
30008a22:	9702      	str	r7, [sp, #8]
30008a24:	e7a8      	b.n	30008978 <flash_layout_init+0x84>
30008a26:	685f      	ldr	r7, [r3, #4]
30008a28:	9705      	str	r7, [sp, #20]
30008a2a:	2701      	movs	r7, #1
30008a2c:	9704      	str	r7, [sp, #16]
30008a2e:	e7a3      	b.n	30008978 <flash_layout_init+0x84>
30008a30:	689f      	ldr	r7, [r3, #8]
30008a32:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008a36:	3701      	adds	r7, #1
30008a38:	eba7 070a 	sub.w	r7, r7, sl
30008a3c:	9706      	str	r7, [sp, #24]
30008a3e:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008a42:	9707      	str	r7, [sp, #28]
30008a44:	9f06      	ldr	r7, [sp, #24]
30008a46:	f3c7 3707 	ubfx	r7, r7, #12, #8
30008a4a:	9708      	str	r7, [sp, #32]
30008a4c:	2701      	movs	r7, #1
30008a4e:	9706      	str	r7, [sp, #24]
30008a50:	e792      	b.n	30008978 <flash_layout_init+0x84>
30008a52:	689f      	ldr	r7, [r3, #8]
30008a54:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008a58:	3701      	adds	r7, #1
30008a5a:	eba7 070a 	sub.w	r7, r7, sl
30008a5e:	970b      	str	r7, [sp, #44]	; 0x2c
30008a60:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008a64:	9709      	str	r7, [sp, #36]	; 0x24
30008a66:	2701      	movs	r7, #1
30008a68:	970a      	str	r7, [sp, #40]	; 0x28
30008a6a:	e785      	b.n	30008978 <flash_layout_init+0x84>
30008a6c:	9b07      	ldr	r3, [sp, #28]
30008a6e:	f8ce 3000 	str.w	r3, [lr]
30008a72:	9b08      	ldr	r3, [sp, #32]
30008a74:	f88c 3000 	strb.w	r3, [ip]
30008a78:	e798      	b.n	300089ac <flash_layout_init+0xb8>
30008a7a:	9b05      	ldr	r3, [sp, #20]
30008a7c:	6153      	str	r3, [r2, #20]
30008a7e:	e792      	b.n	300089a6 <flash_layout_init+0xb2>
30008a80:	4b07      	ldr	r3, [pc, #28]	; (30008aa0 <flash_layout_init+0x1ac>)
30008a82:	9a09      	ldr	r2, [sp, #36]	; 0x24
30008a84:	601a      	str	r2, [r3, #0]
30008a86:	4b07      	ldr	r3, [pc, #28]	; (30008aa4 <flash_layout_init+0x1b0>)
30008a88:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30008a8a:	601a      	str	r2, [r3, #0]
30008a8c:	e791      	b.n	300089b2 <flash_layout_init+0xbe>
30008a8e:	bf00      	nop
30008a90:	3000a219 	.word	0x3000a219
30008a94:	3000f598 	.word	0x3000f598
30008a98:	3000f520 	.word	0x3000f520
30008a9c:	30010184 	.word	0x30010184
30008aa0:	3001017c 	.word	0x3001017c
30008aa4:	30010180 	.word	0x30010180
30008aa8:	30010164 	.word	0x30010164
30008aac:	30010188 	.word	0x30010188
30008ab0:	3001018c 	.word	0x3001018c
30008ab4:	3000f5f8 	.word	0x3000f5f8

30008ab8 <flash_get_layout_info>:
30008ab8:	b570      	push	{r4, r5, r6, lr}
30008aba:	4b0f      	ldr	r3, [pc, #60]	; (30008af8 <flash_get_layout_info+0x40>)
30008abc:	4615      	mov	r5, r2
30008abe:	4604      	mov	r4, r0
30008ac0:	460e      	mov	r6, r1
30008ac2:	4798      	blx	r3
30008ac4:	4b0d      	ldr	r3, [pc, #52]	; (30008afc <flash_get_layout_info+0x44>)
30008ac6:	4a0e      	ldr	r2, [pc, #56]	; (30008b00 <flash_get_layout_info+0x48>)
30008ac8:	2800      	cmp	r0, #0
30008aca:	bf14      	ite	ne
30008acc:	469c      	movne	ip, r3
30008ace:	4694      	moveq	ip, r2
30008ad0:	f8dc 3000 	ldr.w	r3, [ip]
30008ad4:	2bff      	cmp	r3, #255	; 0xff
30008ad6:	d104      	bne.n	30008ae2 <flash_get_layout_info+0x2a>
30008ad8:	e00d      	b.n	30008af6 <flash_get_layout_info+0x3e>
30008ada:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
30008ade:	2bff      	cmp	r3, #255	; 0xff
30008ae0:	d009      	beq.n	30008af6 <flash_get_layout_info+0x3e>
30008ae2:	429c      	cmp	r4, r3
30008ae4:	d1f9      	bne.n	30008ada <flash_get_layout_info+0x22>
30008ae6:	b116      	cbz	r6, 30008aee <flash_get_layout_info+0x36>
30008ae8:	f8dc 3004 	ldr.w	r3, [ip, #4]
30008aec:	6033      	str	r3, [r6, #0]
30008aee:	b115      	cbz	r5, 30008af6 <flash_get_layout_info+0x3e>
30008af0:	f8dc 3008 	ldr.w	r3, [ip, #8]
30008af4:	602b      	str	r3, [r5, #0]
30008af6:	bd70      	pop	{r4, r5, r6, pc}
30008af8:	3000a219 	.word	0x3000a219
30008afc:	3000f598 	.word	0x3000f598
30008b00:	3000f520 	.word	0x3000f520

30008b04 <NAND_CHECK_IS_BAD_BLOCK>:
30008b04:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008b08:	4e24      	ldr	r6, [pc, #144]	; (30008b9c <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30008b0a:	0907      	lsrs	r7, r0, #4
30008b0c:	0045      	lsls	r5, r0, #1
30008b0e:	b085      	sub	sp, #20
30008b10:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008b14:	f005 051e 	and.w	r5, r5, #30
30008b18:	40eb      	lsrs	r3, r5
30008b1a:	f003 0303 	and.w	r3, r3, #3
30008b1e:	2b02      	cmp	r3, #2
30008b20:	d02c      	beq.n	30008b7c <NAND_CHECK_IS_BAD_BLOCK+0x78>
30008b22:	2b01      	cmp	r3, #1
30008b24:	d026      	beq.n	30008b74 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30008b26:	4b1e      	ldr	r3, [pc, #120]	; (30008ba0 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008b28:	f04f 0801 	mov.w	r8, #1
30008b2c:	2204      	movs	r2, #4
30008b2e:	4604      	mov	r4, r0
30008b30:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008b34:	0180      	lsls	r0, r0, #6
30008b36:	ab03      	add	r3, sp, #12
30008b38:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30008bac <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30008b3c:	fa08 f101 	lsl.w	r1, r8, r1
30008b40:	47c8      	blx	r9
30008b42:	f89d 300c 	ldrb.w	r3, [sp, #12]
30008b46:	f89d 200d 	ldrb.w	r2, [sp, #13]
30008b4a:	4013      	ands	r3, r2
30008b4c:	2bff      	cmp	r3, #255	; 0xff
30008b4e:	d019      	beq.n	30008b84 <NAND_CHECK_IS_BAD_BLOCK+0x80>
30008b50:	2002      	movs	r0, #2
30008b52:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
30008b56:	4b13      	ldr	r3, [pc, #76]	; (30008ba4 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008b58:	fa00 f505 	lsl.w	r5, r0, r5
30008b5c:	9400      	str	r4, [sp, #0]
30008b5e:	4619      	mov	r1, r3
30008b60:	432a      	orrs	r2, r5
30008b62:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
30008b66:	4a10      	ldr	r2, [pc, #64]	; (30008ba8 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008b68:	f003 fb4e 	bl	3000c208 <rtk_log_write>
30008b6c:	4640      	mov	r0, r8
30008b6e:	b005      	add	sp, #20
30008b70:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008b74:	2000      	movs	r0, #0
30008b76:	b005      	add	sp, #20
30008b78:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008b7c:	2001      	movs	r0, #1
30008b7e:	b005      	add	sp, #20
30008b80:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008b84:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008b88:	fa08 f005 	lsl.w	r0, r8, r5
30008b8c:	4303      	orrs	r3, r0
30008b8e:	2000      	movs	r0, #0
30008b90:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008b94:	b005      	add	sp, #20
30008b96:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008b9a:	bf00      	nop
30008b9c:	30010198 	.word	0x30010198
30008ba0:	2001c01c 	.word	0x2001c01c
30008ba4:	3000db7c 	.word	0x3000db7c
30008ba8:	3000db88 	.word	0x3000db88
30008bac:	3000a169 	.word	0x3000a169

30008bb0 <Nand_Get_NandAddr>:
30008bb0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008bb4:	4f10      	ldr	r7, [pc, #64]	; (30008bf8 <Nand_Get_NandAddr+0x48>)
30008bb6:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008bba:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008bbe:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008bc2:	40e8      	lsrs	r0, r5
30008bc4:	fa26 f505 	lsr.w	r5, r6, r5
30008bc8:	09ad      	lsrs	r5, r5, #6
30008bca:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008bce:	d90f      	bls.n	30008bf0 <Nand_Get_NandAddr+0x40>
30008bd0:	0984      	lsrs	r4, r0, #6
30008bd2:	f04f 0840 	mov.w	r8, #64	; 0x40
30008bd6:	4620      	mov	r0, r4
30008bd8:	3401      	adds	r4, #1
30008bda:	f7ff ff93 	bl	30008b04 <NAND_CHECK_IS_BAD_BLOCK>
30008bde:	b128      	cbz	r0, 30008bec <Nand_Get_NandAddr+0x3c>
30008be0:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008be4:	3501      	adds	r5, #1
30008be6:	fa08 f303 	lsl.w	r3, r8, r3
30008bea:	441e      	add	r6, r3
30008bec:	42a5      	cmp	r5, r4
30008bee:	d8f2      	bhi.n	30008bd6 <Nand_Get_NandAddr+0x26>
30008bf0:	4630      	mov	r0, r6
30008bf2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008bf6:	bf00      	nop
30008bf8:	2001c01c 	.word	0x2001c01c

30008bfc <Nand_L2P_Table>:
30008bfc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008c00:	b082      	sub	sp, #8
30008c02:	4604      	mov	r4, r0
30008c04:	2002      	movs	r0, #2
30008c06:	aa01      	add	r2, sp, #4
30008c08:	4669      	mov	r1, sp
30008c0a:	f7ff ff55 	bl	30008ab8 <flash_get_layout_info>
30008c0e:	9800      	ldr	r0, [sp, #0]
30008c10:	42a0      	cmp	r0, r4
30008c12:	d810      	bhi.n	30008c36 <Nand_L2P_Table+0x3a>
30008c14:	9b01      	ldr	r3, [sp, #4]
30008c16:	42a3      	cmp	r3, r4
30008c18:	d805      	bhi.n	30008c26 <Nand_L2P_Table+0x2a>
30008c1a:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008c1e:	4620      	mov	r0, r4
30008c20:	b002      	add	sp, #8
30008c22:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008c26:	4621      	mov	r1, r4
30008c28:	f7ff ffc2 	bl	30008bb0 <Nand_Get_NandAddr>
30008c2c:	4604      	mov	r4, r0
30008c2e:	4620      	mov	r0, r4
30008c30:	b002      	add	sp, #8
30008c32:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008c36:	4f0e      	ldr	r7, [pc, #56]	; (30008c70 <Nand_L2P_Table+0x74>)
30008c38:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008c3c:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30008c40:	fa24 f606 	lsr.w	r6, r4, r6
30008c44:	09b6      	lsrs	r6, r6, #6
30008c46:	d0ea      	beq.n	30008c1e <Nand_L2P_Table+0x22>
30008c48:	2500      	movs	r5, #0
30008c4a:	f04f 0840 	mov.w	r8, #64	; 0x40
30008c4e:	4628      	mov	r0, r5
30008c50:	3501      	adds	r5, #1
30008c52:	f7ff ff57 	bl	30008b04 <NAND_CHECK_IS_BAD_BLOCK>
30008c56:	b128      	cbz	r0, 30008c64 <Nand_L2P_Table+0x68>
30008c58:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008c5c:	3601      	adds	r6, #1
30008c5e:	fa08 f303 	lsl.w	r3, r8, r3
30008c62:	441c      	add	r4, r3
30008c64:	42b5      	cmp	r5, r6
30008c66:	d3f2      	bcc.n	30008c4e <Nand_L2P_Table+0x52>
30008c68:	4620      	mov	r0, r4
30008c6a:	b002      	add	sp, #8
30008c6c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008c70:	2001c01c 	.word	0x2001c01c

30008c74 <NandImgCopy>:
30008c74:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008c78:	4617      	mov	r7, r2
30008c7a:	4603      	mov	r3, r0
30008c7c:	b089      	sub	sp, #36	; 0x24
30008c7e:	f8df 9100 	ldr.w	r9, [pc, #256]	; 30008d80 <NandImgCopy+0x10c>
30008c82:	f107 38ff 	add.w	r8, r7, #4294967295
30008c86:	4608      	mov	r0, r1
30008c88:	9307      	str	r3, [sp, #28]
30008c8a:	f7ff ffb7 	bl	30008bfc <Nand_L2P_Table>
30008c8e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008c92:	4480      	add	r8, r0
30008c94:	fa20 f406 	lsr.w	r4, r0, r6
30008c98:	fa28 f806 	lsr.w	r8, r8, r6
30008c9c:	eba8 0804 	sub.w	r8, r8, r4
30008ca0:	f1b8 3fff 	cmp.w	r8, #4294967295
30008ca4:	d054      	beq.n	30008d50 <NandImgCopy+0xdc>
30008ca6:	2300      	movs	r3, #0
30008ca8:	4605      	mov	r5, r0
30008caa:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008cae:	469a      	mov	sl, r3
30008cb0:	9306      	str	r3, [sp, #24]
30008cb2:	463b      	mov	r3, r7
30008cb4:	4647      	mov	r7, r8
30008cb6:	4698      	mov	r8, r3
30008cb8:	e020      	b.n	30008cfc <NandImgCopy+0x88>
30008cba:	f1c2 0320 	rsb	r3, r2, #32
30008cbe:	f04f 31ff 	mov.w	r1, #4294967295
30008cc2:	40d9      	lsrs	r1, r3
30008cc4:	9b07      	ldr	r3, [sp, #28]
30008cc6:	4029      	ands	r1, r5
30008cc8:	4453      	add	r3, sl
30008cca:	b377      	cbz	r7, 30008d2a <NandImgCopy+0xb6>
30008ccc:	2601      	movs	r6, #1
30008cce:	4620      	mov	r0, r4
30008cd0:	9105      	str	r1, [sp, #20]
30008cd2:	fa06 f202 	lsl.w	r2, r6, r2
30008cd6:	4e27      	ldr	r6, [pc, #156]	; (30008d74 <NandImgCopy+0x100>)
30008cd8:	9204      	str	r2, [sp, #16]
30008cda:	1a52      	subs	r2, r2, r1
30008cdc:	4492      	add	sl, r2
30008cde:	4415      	add	r5, r2
30008ce0:	47b0      	blx	r6
30008ce2:	f010 0f70 	tst.w	r0, #112	; 0x70
30008ce6:	9b04      	ldr	r3, [sp, #16]
30008ce8:	9905      	ldr	r1, [sp, #20]
30008cea:	d134      	bne.n	30008d56 <NandImgCopy+0xe2>
30008cec:	eba8 0803 	sub.w	r8, r8, r3
30008cf0:	4488      	add	r8, r1
30008cf2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008cf6:	3f01      	subs	r7, #1
30008cf8:	fa25 f406 	lsr.w	r4, r5, r6
30008cfc:	9b06      	ldr	r3, [sp, #24]
30008cfe:	4632      	mov	r2, r6
30008d00:	09a6      	lsrs	r6, r4, #6
30008d02:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008d06:	d8d8      	bhi.n	30008cba <NandImgCopy+0x46>
30008d08:	e005      	b.n	30008d16 <NandImgCopy+0xa2>
30008d0a:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008d0e:	3440      	adds	r4, #64	; 0x40
30008d10:	fa0b f303 	lsl.w	r3, fp, r3
30008d14:	441d      	add	r5, r3
30008d16:	4630      	mov	r0, r6
30008d18:	3601      	adds	r6, #1
30008d1a:	f7ff fef3 	bl	30008b04 <NAND_CHECK_IS_BAD_BLOCK>
30008d1e:	2800      	cmp	r0, #0
30008d20:	d1f3      	bne.n	30008d0a <NandImgCopy+0x96>
30008d22:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008d26:	9606      	str	r6, [sp, #24]
30008d28:	e7c7      	b.n	30008cba <NandImgCopy+0x46>
30008d2a:	4642      	mov	r2, r8
30008d2c:	4620      	mov	r0, r4
30008d2e:	4d11      	ldr	r5, [pc, #68]	; (30008d74 <NandImgCopy+0x100>)
30008d30:	47a8      	blx	r5
30008d32:	f010 0f70 	tst.w	r0, #112	; 0x70
30008d36:	d00b      	beq.n	30008d50 <NandImgCopy+0xdc>
30008d38:	09a1      	lsrs	r1, r4, #6
30008d3a:	4b0f      	ldr	r3, [pc, #60]	; (30008d78 <NandImgCopy+0x104>)
30008d3c:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008d40:	4a0e      	ldr	r2, [pc, #56]	; (30008d7c <NandImgCopy+0x108>)
30008d42:	e9cd 1001 	strd	r1, r0, [sp, #4]
30008d46:	9400      	str	r4, [sp, #0]
30008d48:	4619      	mov	r1, r3
30008d4a:	2004      	movs	r0, #4
30008d4c:	f003 fa5c 	bl	3000c208 <rtk_log_write>
30008d50:	b009      	add	sp, #36	; 0x24
30008d52:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008d56:	1ac9      	subs	r1, r1, r3
30008d58:	09a3      	lsrs	r3, r4, #6
30008d5a:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008d5e:	9002      	str	r0, [sp, #8]
30008d60:	9301      	str	r3, [sp, #4]
30008d62:	4488      	add	r8, r1
30008d64:	4b04      	ldr	r3, [pc, #16]	; (30008d78 <NandImgCopy+0x104>)
30008d66:	2004      	movs	r0, #4
30008d68:	4a04      	ldr	r2, [pc, #16]	; (30008d7c <NandImgCopy+0x108>)
30008d6a:	4619      	mov	r1, r3
30008d6c:	9400      	str	r4, [sp, #0]
30008d6e:	f003 fa4b 	bl	3000c208 <rtk_log_write>
30008d72:	e7be      	b.n	30008cf2 <NandImgCopy+0x7e>
30008d74:	3000a169 	.word	0x3000a169
30008d78:	3000db7c 	.word	0x3000db7c
30008d7c:	3000dba0 	.word	0x3000dba0
30008d80:	2001c01c 	.word	0x2001c01c

30008d84 <OTP_Read8.part.0>:
30008d84:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008d88:	4b30      	ldr	r3, [pc, #192]	; (30008e4c <OTP_Read8.part.0+0xc8>)
30008d8a:	4680      	mov	r8, r0
30008d8c:	e843 f300 	tt	r3, r3
30008d90:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008d94:	4a2e      	ldr	r2, [pc, #184]	; (30008e50 <OTP_Read8.part.0+0xcc>)
30008d96:	4e2f      	ldr	r6, [pc, #188]	; (30008e54 <OTP_Read8.part.0+0xd0>)
30008d98:	b082      	sub	sp, #8
30008d9a:	2b00      	cmp	r3, #0
30008d9c:	4689      	mov	r9, r1
30008d9e:	bf0e      	itee	eq
30008da0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008da4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008da8:	4616      	movne	r6, r2
30008daa:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008dac:	05d8      	lsls	r0, r3, #23
30008dae:	d505      	bpl.n	30008dbc <OTP_Read8.part.0+0x38>
30008db0:	4d29      	ldr	r5, [pc, #164]	; (30008e58 <OTP_Read8.part.0+0xd4>)
30008db2:	2064      	movs	r0, #100	; 0x64
30008db4:	47a8      	blx	r5
30008db6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008db8:	05d9      	lsls	r1, r3, #23
30008dba:	d4fa      	bmi.n	30008db2 <OTP_Read8.part.0+0x2e>
30008dbc:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008dbe:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008dc2:	643b      	str	r3, [r7, #64]	; 0x40
30008dc4:	4b25      	ldr	r3, [pc, #148]	; (30008e5c <OTP_Read8.part.0+0xd8>)
30008dc6:	681a      	ldr	r2, [r3, #0]
30008dc8:	07d2      	lsls	r2, r2, #31
30008dca:	d403      	bmi.n	30008dd4 <OTP_Read8.part.0+0x50>
30008dcc:	681a      	ldr	r2, [r3, #0]
30008dce:	f042 0201 	orr.w	r2, r2, #1
30008dd2:	601a      	str	r2, [r3, #0]
30008dd4:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008dd8:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008ddc:	694a      	ldr	r2, [r1, #20]
30008dde:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008de2:	614a      	str	r2, [r1, #20]
30008de4:	6033      	str	r3, [r6, #0]
30008de6:	6833      	ldr	r3, [r6, #0]
30008de8:	2b00      	cmp	r3, #0
30008dea:	db10      	blt.n	30008e0e <OTP_Read8.part.0+0x8a>
30008dec:	2400      	movs	r4, #0
30008dee:	4d1a      	ldr	r5, [pc, #104]	; (30008e58 <OTP_Read8.part.0+0xd4>)
30008df0:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008df4:	e001      	b.n	30008dfa <OTP_Read8.part.0+0x76>
30008df6:	4554      	cmp	r4, sl
30008df8:	d01a      	beq.n	30008e30 <OTP_Read8.part.0+0xac>
30008dfa:	2005      	movs	r0, #5
30008dfc:	3401      	adds	r4, #1
30008dfe:	47a8      	blx	r5
30008e00:	6833      	ldr	r3, [r6, #0]
30008e02:	2b00      	cmp	r3, #0
30008e04:	daf7      	bge.n	30008df6 <OTP_Read8.part.0+0x72>
30008e06:	f644 6320 	movw	r3, #20000	; 0x4e20
30008e0a:	429c      	cmp	r4, r3
30008e0c:	d010      	beq.n	30008e30 <OTP_Read8.part.0+0xac>
30008e0e:	6833      	ldr	r3, [r6, #0]
30008e10:	2001      	movs	r0, #1
30008e12:	f889 3000 	strb.w	r3, [r9]
30008e16:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008e1a:	6953      	ldr	r3, [r2, #20]
30008e1c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008e20:	6153      	str	r3, [r2, #20]
30008e22:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008e24:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008e28:	643b      	str	r3, [r7, #64]	; 0x40
30008e2a:	b002      	add	sp, #8
30008e2c:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008e30:	22ff      	movs	r2, #255	; 0xff
30008e32:	4b0b      	ldr	r3, [pc, #44]	; (30008e60 <OTP_Read8.part.0+0xdc>)
30008e34:	2002      	movs	r0, #2
30008e36:	f8cd 8000 	str.w	r8, [sp]
30008e3a:	4619      	mov	r1, r3
30008e3c:	f889 2000 	strb.w	r2, [r9]
30008e40:	4a08      	ldr	r2, [pc, #32]	; (30008e64 <OTP_Read8.part.0+0xe0>)
30008e42:	f003 f9e1 	bl	3000c208 <rtk_log_write>
30008e46:	2000      	movs	r0, #0
30008e48:	e7e5      	b.n	30008e16 <OTP_Read8.part.0+0x92>
30008e4a:	bf00      	nop
30008e4c:	0000e5f9 	.word	0x0000e5f9
30008e50:	5200000c 	.word	0x5200000c
30008e54:	42000008 	.word	0x42000008
30008e58:	00009b2d 	.word	0x00009b2d
30008e5c:	42008000 	.word	0x42008000
30008e60:	3000dbdc 	.word	0x3000dbdc
30008e64:	3000dbe0 	.word	0x3000dbe0

30008e68 <OTP_ProgramMarginRead8.part.0>:
30008e68:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008e6c:	4b31      	ldr	r3, [pc, #196]	; (30008f34 <OTP_ProgramMarginRead8.part.0+0xcc>)
30008e6e:	4680      	mov	r8, r0
30008e70:	e843 f300 	tt	r3, r3
30008e74:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008e78:	4a2f      	ldr	r2, [pc, #188]	; (30008f38 <OTP_ProgramMarginRead8.part.0+0xd0>)
30008e7a:	4e30      	ldr	r6, [pc, #192]	; (30008f3c <OTP_ProgramMarginRead8.part.0+0xd4>)
30008e7c:	b082      	sub	sp, #8
30008e7e:	2b00      	cmp	r3, #0
30008e80:	4689      	mov	r9, r1
30008e82:	bf0e      	itee	eq
30008e84:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008e88:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008e8c:	4616      	movne	r6, r2
30008e8e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008e90:	05d8      	lsls	r0, r3, #23
30008e92:	d505      	bpl.n	30008ea0 <OTP_ProgramMarginRead8.part.0+0x38>
30008e94:	4d2a      	ldr	r5, [pc, #168]	; (30008f40 <OTP_ProgramMarginRead8.part.0+0xd8>)
30008e96:	2064      	movs	r0, #100	; 0x64
30008e98:	47a8      	blx	r5
30008e9a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008e9c:	05d9      	lsls	r1, r3, #23
30008e9e:	d4fa      	bmi.n	30008e96 <OTP_ProgramMarginRead8.part.0+0x2e>
30008ea0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008ea2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008ea6:	643b      	str	r3, [r7, #64]	; 0x40
30008ea8:	4b26      	ldr	r3, [pc, #152]	; (30008f44 <OTP_ProgramMarginRead8.part.0+0xdc>)
30008eaa:	681a      	ldr	r2, [r3, #0]
30008eac:	07d2      	lsls	r2, r2, #31
30008eae:	d403      	bmi.n	30008eb8 <OTP_ProgramMarginRead8.part.0+0x50>
30008eb0:	681a      	ldr	r2, [r3, #0]
30008eb2:	f042 0201 	orr.w	r2, r2, #1
30008eb6:	601a      	str	r2, [r3, #0]
30008eb8:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008ebc:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008ec0:	694a      	ldr	r2, [r1, #20]
30008ec2:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30008ec6:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008eca:	614a      	str	r2, [r1, #20]
30008ecc:	6033      	str	r3, [r6, #0]
30008ece:	6833      	ldr	r3, [r6, #0]
30008ed0:	2b00      	cmp	r3, #0
30008ed2:	db10      	blt.n	30008ef6 <OTP_ProgramMarginRead8.part.0+0x8e>
30008ed4:	2400      	movs	r4, #0
30008ed6:	4d1a      	ldr	r5, [pc, #104]	; (30008f40 <OTP_ProgramMarginRead8.part.0+0xd8>)
30008ed8:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008edc:	e001      	b.n	30008ee2 <OTP_ProgramMarginRead8.part.0+0x7a>
30008ede:	4554      	cmp	r4, sl
30008ee0:	d01a      	beq.n	30008f18 <OTP_ProgramMarginRead8.part.0+0xb0>
30008ee2:	2005      	movs	r0, #5
30008ee4:	3401      	adds	r4, #1
30008ee6:	47a8      	blx	r5
30008ee8:	6833      	ldr	r3, [r6, #0]
30008eea:	2b00      	cmp	r3, #0
30008eec:	daf7      	bge.n	30008ede <OTP_ProgramMarginRead8.part.0+0x76>
30008eee:	f644 6320 	movw	r3, #20000	; 0x4e20
30008ef2:	429c      	cmp	r4, r3
30008ef4:	d010      	beq.n	30008f18 <OTP_ProgramMarginRead8.part.0+0xb0>
30008ef6:	6833      	ldr	r3, [r6, #0]
30008ef8:	2001      	movs	r0, #1
30008efa:	f889 3000 	strb.w	r3, [r9]
30008efe:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008f02:	6953      	ldr	r3, [r2, #20]
30008f04:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008f08:	6153      	str	r3, [r2, #20]
30008f0a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008f0c:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008f10:	643b      	str	r3, [r7, #64]	; 0x40
30008f12:	b002      	add	sp, #8
30008f14:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008f18:	22ff      	movs	r2, #255	; 0xff
30008f1a:	4b0b      	ldr	r3, [pc, #44]	; (30008f48 <OTP_ProgramMarginRead8.part.0+0xe0>)
30008f1c:	2002      	movs	r0, #2
30008f1e:	f8cd 8000 	str.w	r8, [sp]
30008f22:	4619      	mov	r1, r3
30008f24:	f889 2000 	strb.w	r2, [r9]
30008f28:	4a08      	ldr	r2, [pc, #32]	; (30008f4c <OTP_ProgramMarginRead8.part.0+0xe4>)
30008f2a:	f003 f96d 	bl	3000c208 <rtk_log_write>
30008f2e:	2000      	movs	r0, #0
30008f30:	e7e5      	b.n	30008efe <OTP_ProgramMarginRead8.part.0+0x96>
30008f32:	bf00      	nop
30008f34:	0000e5f9 	.word	0x0000e5f9
30008f38:	5200000c 	.word	0x5200000c
30008f3c:	42000008 	.word	0x42000008
30008f40:	00009b2d 	.word	0x00009b2d
30008f44:	42008000 	.word	0x42008000
30008f48:	3000dbdc 	.word	0x3000dbdc
30008f4c:	3000dbe0 	.word	0x3000dbe0

30008f50 <OTP_Read8>:
30008f50:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008f54:	d201      	bcs.n	30008f5a <OTP_Read8+0xa>
30008f56:	f7ff bf15 	b.w	30008d84 <OTP_Read8.part.0>
30008f5a:	22ff      	movs	r2, #255	; 0xff
30008f5c:	2000      	movs	r0, #0
30008f5e:	700a      	strb	r2, [r1, #0]
30008f60:	4770      	bx	lr
30008f62:	bf00      	nop

30008f64 <OTP_Write8>:
30008f64:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008f68:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008f6c:	b087      	sub	sp, #28
30008f6e:	d278      	bcs.n	30009062 <OTP_Write8+0xfe>
30008f70:	4689      	mov	r9, r1
30008f72:	f10d 0117 	add.w	r1, sp, #23
30008f76:	4680      	mov	r8, r0
30008f78:	f7ff ff76 	bl	30008e68 <OTP_ProgramMarginRead8.part.0>
30008f7c:	2800      	cmp	r0, #0
30008f7e:	d073      	beq.n	30009068 <OTP_Write8+0x104>
30008f80:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008f84:	2202      	movs	r2, #2
30008f86:	46ca      	mov	sl, r9
30008f88:	9303      	str	r3, [sp, #12]
30008f8a:	f89d 3017 	ldrb.w	r3, [sp, #23]
30008f8e:	9202      	str	r2, [sp, #8]
30008f90:	ea6a 0a03 	orn	sl, sl, r3
30008f94:	fa5f fa8a 	uxtb.w	sl, sl
30008f98:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
30008f9c:	d04f      	beq.n	3000903e <OTP_Write8+0xda>
30008f9e:	4b4b      	ldr	r3, [pc, #300]	; (300090cc <OTP_Write8+0x168>)
30008fa0:	e843 f300 	tt	r3, r3
30008fa4:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008fa8:	4a49      	ldr	r2, [pc, #292]	; (300090d0 <OTP_Write8+0x16c>)
30008faa:	2b00      	cmp	r3, #0
30008fac:	4b49      	ldr	r3, [pc, #292]	; (300090d4 <OTP_Write8+0x170>)
30008fae:	bf0b      	itete	eq
30008fb0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008fb4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008fb8:	461d      	moveq	r5, r3
30008fba:	4615      	movne	r5, r2
30008fbc:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fbe:	05d8      	lsls	r0, r3, #23
30008fc0:	d505      	bpl.n	30008fce <OTP_Write8+0x6a>
30008fc2:	4c45      	ldr	r4, [pc, #276]	; (300090d8 <OTP_Write8+0x174>)
30008fc4:	2064      	movs	r0, #100	; 0x64
30008fc6:	47a0      	blx	r4
30008fc8:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fca:	05d9      	lsls	r1, r3, #23
30008fcc:	d4fa      	bmi.n	30008fc4 <OTP_Write8+0x60>
30008fce:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fd0:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008fd4:	643b      	str	r3, [r7, #64]	; 0x40
30008fd6:	4b41      	ldr	r3, [pc, #260]	; (300090dc <OTP_Write8+0x178>)
30008fd8:	681a      	ldr	r2, [r3, #0]
30008fda:	07d2      	lsls	r2, r2, #31
30008fdc:	d403      	bmi.n	30008fe6 <OTP_Write8+0x82>
30008fde:	681a      	ldr	r2, [r3, #0]
30008fe0:	f042 0201 	orr.w	r2, r2, #1
30008fe4:	601a      	str	r2, [r3, #0]
30008fe6:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008fea:	9b03      	ldr	r3, [sp, #12]
30008fec:	694a      	ldr	r2, [r1, #20]
30008fee:	ea4a 0303 	orr.w	r3, sl, r3
30008ff2:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30008ff6:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30008ffa:	614a      	str	r2, [r1, #20]
30008ffc:	602b      	str	r3, [r5, #0]
30008ffe:	682b      	ldr	r3, [r5, #0]
30009000:	2b00      	cmp	r3, #0
30009002:	da12      	bge.n	3000902a <OTP_Write8+0xc6>
30009004:	f04f 0b00 	mov.w	fp, #0
30009008:	4c33      	ldr	r4, [pc, #204]	; (300090d8 <OTP_Write8+0x174>)
3000900a:	f644 6620 	movw	r6, #20000	; 0x4e20
3000900e:	e001      	b.n	30009014 <OTP_Write8+0xb0>
30009010:	45b3      	cmp	fp, r6
30009012:	d036      	beq.n	30009082 <OTP_Write8+0x11e>
30009014:	2005      	movs	r0, #5
30009016:	f10b 0b01 	add.w	fp, fp, #1
3000901a:	47a0      	blx	r4
3000901c:	682b      	ldr	r3, [r5, #0]
3000901e:	2b00      	cmp	r3, #0
30009020:	dbf6      	blt.n	30009010 <OTP_Write8+0xac>
30009022:	f644 6220 	movw	r2, #20000	; 0x4e20
30009026:	4593      	cmp	fp, r2
30009028:	d02b      	beq.n	30009082 <OTP_Write8+0x11e>
3000902a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000902e:	6953      	ldr	r3, [r2, #20]
30009030:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30009034:	6153      	str	r3, [r2, #20]
30009036:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009038:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000903c:	643b      	str	r3, [r7, #64]	; 0x40
3000903e:	f10d 0117 	add.w	r1, sp, #23
30009042:	4640      	mov	r0, r8
30009044:	f7ff ff10 	bl	30008e68 <OTP_ProgramMarginRead8.part.0>
30009048:	2800      	cmp	r0, #0
3000904a:	d034      	beq.n	300090b6 <OTP_Write8+0x152>
3000904c:	f89d 3017 	ldrb.w	r3, [sp, #23]
30009050:	4599      	cmp	r9, r3
30009052:	d039      	beq.n	300090c8 <OTP_Write8+0x164>
30009054:	9a02      	ldr	r2, [sp, #8]
30009056:	2a01      	cmp	r2, #1
30009058:	d110      	bne.n	3000907c <OTP_Write8+0x118>
3000905a:	2000      	movs	r0, #0
3000905c:	b007      	add	sp, #28
3000905e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009062:	23ff      	movs	r3, #255	; 0xff
30009064:	f88d 3017 	strb.w	r3, [sp, #23]
30009068:	4b1d      	ldr	r3, [pc, #116]	; (300090e0 <OTP_Write8+0x17c>)
3000906a:	2002      	movs	r0, #2
3000906c:	4a1d      	ldr	r2, [pc, #116]	; (300090e4 <OTP_Write8+0x180>)
3000906e:	4619      	mov	r1, r3
30009070:	f003 f8ca 	bl	3000c208 <rtk_log_write>
30009074:	2000      	movs	r0, #0
30009076:	b007      	add	sp, #28
30009078:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000907c:	2201      	movs	r2, #1
3000907e:	9202      	str	r2, [sp, #8]
30009080:	e786      	b.n	30008f90 <OTP_Write8+0x2c>
30009082:	4b17      	ldr	r3, [pc, #92]	; (300090e0 <OTP_Write8+0x17c>)
30009084:	2003      	movs	r0, #3
30009086:	4a18      	ldr	r2, [pc, #96]	; (300090e8 <OTP_Write8+0x184>)
30009088:	4619      	mov	r1, r3
3000908a:	f8cd 8000 	str.w	r8, [sp]
3000908e:	f003 f8bb 	bl	3000c208 <rtk_log_write>
30009092:	f04f 4084 	mov.w	r0, #1107296256	; 0x42000000
30009096:	4b12      	ldr	r3, [pc, #72]	; (300090e0 <OTP_Write8+0x17c>)
30009098:	6941      	ldr	r1, [r0, #20]
3000909a:	4a14      	ldr	r2, [pc, #80]	; (300090ec <OTP_Write8+0x188>)
3000909c:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
300090a0:	6141      	str	r1, [r0, #20]
300090a2:	4619      	mov	r1, r3
300090a4:	6c38      	ldr	r0, [r7, #64]	; 0x40
300090a6:	f420 7080 	bic.w	r0, r0, #256	; 0x100
300090aa:	6438      	str	r0, [r7, #64]	; 0x40
300090ac:	2002      	movs	r0, #2
300090ae:	f003 f8ab 	bl	3000c208 <rtk_log_write>
300090b2:	2000      	movs	r0, #0
300090b4:	e7df      	b.n	30009076 <OTP_Write8+0x112>
300090b6:	4b0a      	ldr	r3, [pc, #40]	; (300090e0 <OTP_Write8+0x17c>)
300090b8:	4604      	mov	r4, r0
300090ba:	4a0d      	ldr	r2, [pc, #52]	; (300090f0 <OTP_Write8+0x18c>)
300090bc:	2002      	movs	r0, #2
300090be:	4619      	mov	r1, r3
300090c0:	f003 f8a2 	bl	3000c208 <rtk_log_write>
300090c4:	4620      	mov	r0, r4
300090c6:	e7d6      	b.n	30009076 <OTP_Write8+0x112>
300090c8:	2001      	movs	r0, #1
300090ca:	e7d4      	b.n	30009076 <OTP_Write8+0x112>
300090cc:	0000e5f9 	.word	0x0000e5f9
300090d0:	5200000c 	.word	0x5200000c
300090d4:	42000008 	.word	0x42000008
300090d8:	00009b2d 	.word	0x00009b2d
300090dc:	42008000 	.word	0x42008000
300090e0:	3000dbdc 	.word	0x3000dbdc
300090e4:	3000dbfc 	.word	0x3000dbfc
300090e8:	3000dc14 	.word	0x3000dc14
300090ec:	3000dc30 	.word	0x3000dc30
300090f0:	3000dc4c 	.word	0x3000dc4c

300090f4 <EFUSERead8>:
300090f4:	4608      	mov	r0, r1
300090f6:	4611      	mov	r1, r2
300090f8:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
300090fc:	d201      	bcs.n	30009102 <EFUSERead8+0xe>
300090fe:	f7ff be41 	b.w	30008d84 <OTP_Read8.part.0>
30009102:	23ff      	movs	r3, #255	; 0xff
30009104:	2000      	movs	r0, #0
30009106:	7013      	strb	r3, [r2, #0]
30009108:	4770      	bx	lr
3000910a:	bf00      	nop

3000910c <PLL_NP_ClkSet>:
3000910c:	b570      	push	{r4, r5, r6, lr}
3000910e:	4604      	mov	r4, r0
30009110:	4b1b      	ldr	r3, [pc, #108]	; (30009180 <PLL_NP_ClkSet+0x74>)
30009112:	4798      	blx	r3
30009114:	4d1b      	ldr	r5, [pc, #108]	; (30009184 <PLL_NP_ClkSet+0x78>)
30009116:	08c2      	lsrs	r2, r0, #3
30009118:	fbb4 f3f0 	udiv	r3, r4, r0
3000911c:	fb00 4c13 	mls	ip, r0, r3, r4
30009120:	1e9c      	subs	r4, r3, #2
30009122:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009124:	0164      	lsls	r4, r4, #5
30009126:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
3000912a:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
3000912e:	431c      	orrs	r4, r3
30009130:	fbbc f3f2 	udiv	r3, ip, r2
30009134:	646c      	str	r4, [r5, #68]	; 0x44
30009136:	041c      	lsls	r4, r3, #16
30009138:	fb02 c013 	mls	r0, r2, r3, ip
3000913c:	2300      	movs	r3, #0
3000913e:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30009140:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30009144:	4619      	mov	r1, r3
30009146:	fa1f fc86 	uxth.w	ip, r6
3000914a:	4e0f      	ldr	r6, [pc, #60]	; (30009188 <PLL_NP_ClkSet+0x7c>)
3000914c:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30009150:	ea44 040c 	orr.w	r4, r4, ip
30009154:	f003 f8e2 	bl	3000c31c <__aeabi_uldivmod>
30009158:	4603      	mov	r3, r0
3000915a:	2001      	movs	r0, #1
3000915c:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30009160:	64ec      	str	r4, [r5, #76]	; 0x4c
30009162:	47b0      	blx	r6
30009164:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009166:	203c      	movs	r0, #60	; 0x3c
30009168:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
3000916c:	646b      	str	r3, [r5, #68]	; 0x44
3000916e:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009170:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30009174:	646b      	str	r3, [r5, #68]	; 0x44
30009176:	4633      	mov	r3, r6
30009178:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000917c:	4718      	bx	r3
3000917e:	bf00      	nop
30009180:	000099f5 	.word	0x000099f5
30009184:	42008800 	.word	0x42008800
30009188:	00009b2d 	.word	0x00009b2d

3000918c <PLL_AP_ClkSet>:
3000918c:	b510      	push	{r4, lr}
3000918e:	4604      	mov	r4, r0
30009190:	4b0b      	ldr	r3, [pc, #44]	; (300091c0 <PLL_AP_ClkSet+0x34>)
30009192:	4798      	blx	r3
30009194:	fbb4 f0f0 	udiv	r0, r4, r0
30009198:	f1a0 0319 	sub.w	r3, r0, #25
3000919c:	1e84      	subs	r4, r0, #2
3000919e:	2b0f      	cmp	r3, #15
300091a0:	d807      	bhi.n	300091b2 <PLL_AP_ClkSet+0x26>
300091a2:	4b08      	ldr	r3, [pc, #32]	; (300091c4 <PLL_AP_ClkSet+0x38>)
300091a4:	b2e0      	uxtb	r0, r4
300091a6:	695c      	ldr	r4, [r3, #20]
300091a8:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
300091ac:	4320      	orrs	r0, r4
300091ae:	6158      	str	r0, [r3, #20]
300091b0:	bd10      	pop	{r4, pc}
300091b2:	f240 1181 	movw	r1, #385	; 0x181
300091b6:	4804      	ldr	r0, [pc, #16]	; (300091c8 <PLL_AP_ClkSet+0x3c>)
300091b8:	f003 faa2 	bl	3000c700 <__io_assert_failed_veneer>
300091bc:	e7f1      	b.n	300091a2 <PLL_AP_ClkSet+0x16>
300091be:	bf00      	nop
300091c0:	000099f5 	.word	0x000099f5
300091c4:	41000300 	.word	0x41000300
300091c8:	3000dc68 	.word	0x3000dc68

300091cc <PLL_AP>:
300091cc:	2801      	cmp	r0, #1
300091ce:	b538      	push	{r3, r4, r5, lr}
300091d0:	d009      	beq.n	300091e6 <PLL_AP+0x1a>
300091d2:	4b13      	ldr	r3, [pc, #76]	; (30009220 <PLL_AP+0x54>)
300091d4:	681a      	ldr	r2, [r3, #0]
300091d6:	f022 0209 	bic.w	r2, r2, #9
300091da:	601a      	str	r2, [r3, #0]
300091dc:	681a      	ldr	r2, [r3, #0]
300091de:	f022 0206 	bic.w	r2, r2, #6
300091e2:	601a      	str	r2, [r3, #0]
300091e4:	bd38      	pop	{r3, r4, r5, pc}
300091e6:	4b0f      	ldr	r3, [pc, #60]	; (30009224 <PLL_AP+0x58>)
300091e8:	6f9a      	ldr	r2, [r3, #120]	; 0x78
300091ea:	f012 0f05 	tst.w	r2, #5
300091ee:	d00f      	beq.n	30009210 <PLL_AP+0x44>
300091f0:	4d0d      	ldr	r5, [pc, #52]	; (30009228 <PLL_AP+0x5c>)
300091f2:	4c0b      	ldr	r4, [pc, #44]	; (30009220 <PLL_AP+0x54>)
300091f4:	2001      	movs	r0, #1
300091f6:	6823      	ldr	r3, [r4, #0]
300091f8:	f043 0306 	orr.w	r3, r3, #6
300091fc:	6023      	str	r3, [r4, #0]
300091fe:	47a8      	blx	r5
30009200:	6823      	ldr	r3, [r4, #0]
30009202:	f043 0309 	orr.w	r3, r3, #9
30009206:	6023      	str	r3, [r4, #0]
30009208:	6863      	ldr	r3, [r4, #4]
3000920a:	2b00      	cmp	r3, #0
3000920c:	dafc      	bge.n	30009208 <PLL_AP+0x3c>
3000920e:	bd38      	pop	{r3, r4, r5, pc}
30009210:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30009212:	20a0      	movs	r0, #160	; 0xa0
30009214:	4d04      	ldr	r5, [pc, #16]	; (30009228 <PLL_AP+0x5c>)
30009216:	f042 0207 	orr.w	r2, r2, #7
3000921a:	679a      	str	r2, [r3, #120]	; 0x78
3000921c:	47a8      	blx	r5
3000921e:	e7e8      	b.n	300091f2 <PLL_AP+0x26>
30009220:	41000300 	.word	0x41000300
30009224:	42008800 	.word	0x42008800
30009228:	00009b2d 	.word	0x00009b2d

3000922c <PSRAM_INFO_Update>:
3000922c:	4947      	ldr	r1, [pc, #284]	; (3000934c <PSRAM_INFO_Update+0x120>)
3000922e:	4a48      	ldr	r2, [pc, #288]	; (30009350 <PSRAM_INFO_Update+0x124>)
30009230:	b538      	push	{r3, r4, r5, lr}
30009232:	f8d1 1228 	ldr.w	r1, [r1, #552]	; 0x228
30009236:	6815      	ldr	r5, [r2, #0]
30009238:	4b46      	ldr	r3, [pc, #280]	; (30009354 <PSRAM_INFO_Update+0x128>)
3000923a:	f3c1 2102 	ubfx	r1, r1, #8, #3
3000923e:	4c46      	ldr	r4, [pc, #280]	; (30009358 <PSRAM_INFO_Update+0x12c>)
30009240:	3101      	adds	r1, #1
30009242:	fb03 f505 	mul.w	r5, r3, r5
30009246:	4b45      	ldr	r3, [pc, #276]	; (3000935c <PSRAM_INFO_Update+0x130>)
30009248:	fbb5 f5f1 	udiv	r5, r5, r1
3000924c:	086a      	lsrs	r2, r5, #1
3000924e:	60e2      	str	r2, [r4, #12]
30009250:	4798      	blx	r3
30009252:	4b43      	ldr	r3, [pc, #268]	; (30009360 <PSRAM_INFO_Update+0x134>)
30009254:	6020      	str	r0, [r4, #0]
30009256:	4798      	blx	r3
30009258:	4b42      	ldr	r3, [pc, #264]	; (30009364 <PSRAM_INFO_Update+0x138>)
3000925a:	220a      	movs	r2, #10
3000925c:	60a0      	str	r0, [r4, #8]
3000925e:	6122      	str	r2, [r4, #16]
30009260:	2220      	movs	r2, #32
30009262:	fbb3 f3f5 	udiv	r3, r3, r5
30009266:	6163      	str	r3, [r4, #20]
30009268:	fbb2 f3f3 	udiv	r3, r2, r3
3000926c:	6822      	ldr	r2, [r4, #0]
3000926e:	6223      	str	r3, [r4, #32]
30009270:	2a01      	cmp	r2, #1
30009272:	68e3      	ldr	r3, [r4, #12]
30009274:	4a3c      	ldr	r2, [pc, #240]	; (30009368 <PSRAM_INFO_Update+0x13c>)
30009276:	d019      	beq.n	300092ac <PSRAM_INFO_Update+0x80>
30009278:	4293      	cmp	r3, r2
3000927a:	d80c      	bhi.n	30009296 <PSRAM_INFO_Update+0x6a>
3000927c:	2304      	movs	r3, #4
3000927e:	2108      	movs	r1, #8
30009280:	461a      	mov	r2, r3
30009282:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009286:	4b39      	ldr	r3, [pc, #228]	; (3000936c <PSRAM_INFO_Update+0x140>)
30009288:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
3000928c:	4b38      	ldr	r3, [pc, #224]	; (30009370 <PSRAM_INFO_Update+0x144>)
3000928e:	689b      	ldr	r3, [r3, #8]
30009290:	079b      	lsls	r3, r3, #30
30009292:	d415      	bmi.n	300092c0 <PSRAM_INFO_Update+0x94>
30009294:	bd38      	pop	{r3, r4, r5, pc}
30009296:	4a37      	ldr	r2, [pc, #220]	; (30009374 <PSRAM_INFO_Update+0x148>)
30009298:	4293      	cmp	r3, r2
3000929a:	d91c      	bls.n	300092d6 <PSRAM_INFO_Update+0xaa>
3000929c:	4a36      	ldr	r2, [pc, #216]	; (30009378 <PSRAM_INFO_Update+0x14c>)
3000929e:	4293      	cmp	r3, r2
300092a0:	d82a      	bhi.n	300092f8 <PSRAM_INFO_Update+0xcc>
300092a2:	2306      	movs	r3, #6
300092a4:	461a      	mov	r2, r3
300092a6:	e9c4 3306 	strd	r3, r3, [r4, #24]
300092aa:	e7ec      	b.n	30009286 <PSRAM_INFO_Update+0x5a>
300092ac:	4293      	cmp	r3, r2
300092ae:	d90d      	bls.n	300092cc <PSRAM_INFO_Update+0xa0>
300092b0:	4a30      	ldr	r2, [pc, #192]	; (30009374 <PSRAM_INFO_Update+0x148>)
300092b2:	4293      	cmp	r3, r2
300092b4:	d815      	bhi.n	300092e2 <PSRAM_INFO_Update+0xb6>
300092b6:	2205      	movs	r2, #5
300092b8:	230f      	movs	r3, #15
300092ba:	e9c4 2306 	strd	r2, r3, [r4, #24]
300092be:	e7e5      	b.n	3000928c <PSRAM_INFO_Update+0x60>
300092c0:	4629      	mov	r1, r5
300092c2:	482e      	ldr	r0, [pc, #184]	; (3000937c <PSRAM_INFO_Update+0x150>)
300092c4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300092c8:	f003 ba22 	b.w	3000c710 <__DiagPrintf_veneer>
300092cc:	2204      	movs	r2, #4
300092ce:	230f      	movs	r3, #15
300092d0:	e9c4 2306 	strd	r2, r3, [r4, #24]
300092d4:	e7da      	b.n	3000928c <PSRAM_INFO_Update+0x60>
300092d6:	2305      	movs	r3, #5
300092d8:	2108      	movs	r1, #8
300092da:	461a      	mov	r2, r3
300092dc:	e9c4 3106 	strd	r3, r1, [r4, #24]
300092e0:	e7d1      	b.n	30009286 <PSRAM_INFO_Update+0x5a>
300092e2:	4a25      	ldr	r2, [pc, #148]	; (30009378 <PSRAM_INFO_Update+0x14c>)
300092e4:	4293      	cmp	r3, r2
300092e6:	d917      	bls.n	30009318 <PSRAM_INFO_Update+0xec>
300092e8:	4a25      	ldr	r2, [pc, #148]	; (30009380 <PSRAM_INFO_Update+0x154>)
300092ea:	4293      	cmp	r3, r2
300092ec:	d81f      	bhi.n	3000932e <PSRAM_INFO_Update+0x102>
300092ee:	2207      	movs	r2, #7
300092f0:	2314      	movs	r3, #20
300092f2:	e9c4 2306 	strd	r2, r3, [r4, #24]
300092f6:	e7c9      	b.n	3000928c <PSRAM_INFO_Update+0x60>
300092f8:	4a21      	ldr	r2, [pc, #132]	; (30009380 <PSRAM_INFO_Update+0x154>)
300092fa:	4293      	cmp	r3, r2
300092fc:	d911      	bls.n	30009322 <PSRAM_INFO_Update+0xf6>
300092fe:	4a21      	ldr	r2, [pc, #132]	; (30009384 <PSRAM_INFO_Update+0x158>)
30009300:	4293      	cmp	r3, r2
30009302:	bf8c      	ite	hi
30009304:	2300      	movhi	r3, #0
30009306:	2301      	movls	r3, #1
30009308:	2804      	cmp	r0, #4
3000930a:	d100      	bne.n	3000930e <PSRAM_INFO_Update+0xe2>
3000930c:	b94b      	cbnz	r3, 30009322 <PSRAM_INFO_Update+0xf6>
3000930e:	2803      	cmp	r0, #3
30009310:	d100      	bne.n	30009314 <PSRAM_INFO_Update+0xe8>
30009312:	b9a3      	cbnz	r3, 3000933e <PSRAM_INFO_Update+0x112>
30009314:	7e22      	ldrb	r2, [r4, #24]
30009316:	e7b6      	b.n	30009286 <PSRAM_INFO_Update+0x5a>
30009318:	2206      	movs	r2, #6
3000931a:	2312      	movs	r3, #18
3000931c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009320:	e7b4      	b.n	3000928c <PSRAM_INFO_Update+0x60>
30009322:	2307      	movs	r3, #7
30009324:	2106      	movs	r1, #6
30009326:	461a      	mov	r2, r3
30009328:	e9c4 3106 	strd	r3, r1, [r4, #24]
3000932c:	e7ab      	b.n	30009286 <PSRAM_INFO_Update+0x5a>
3000932e:	4a15      	ldr	r2, [pc, #84]	; (30009384 <PSRAM_INFO_Update+0x158>)
30009330:	4293      	cmp	r3, r2
30009332:	d8ab      	bhi.n	3000928c <PSRAM_INFO_Update+0x60>
30009334:	2208      	movs	r2, #8
30009336:	231c      	movs	r3, #28
30009338:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000933c:	e7a6      	b.n	3000928c <PSRAM_INFO_Update+0x60>
3000933e:	230a      	movs	r3, #10
30009340:	2106      	movs	r1, #6
30009342:	461a      	mov	r2, r3
30009344:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009348:	e79d      	b.n	30009286 <PSRAM_INFO_Update+0x5a>
3000934a:	bf00      	nop
3000934c:	42008000 	.word	0x42008000
30009350:	3000f508 	.word	0x3000f508
30009354:	000f4240 	.word	0x000f4240
30009358:	300101a0 	.word	0x300101a0
3000935c:	300080a5 	.word	0x300080a5
30009360:	3000806d 	.word	0x3000806d
30009364:	3b9aca00 	.word	0x3b9aca00
30009368:	0632ea00 	.word	0x0632ea00
3000936c:	23020000 	.word	0x23020000
30009370:	2001c00c 	.word	0x2001c00c
30009374:	07ed6b40 	.word	0x07ed6b40
30009378:	09e4f580 	.word	0x09e4f580
3000937c:	3000dc78 	.word	0x3000dc78
30009380:	0bebc200 	.word	0x0bebc200
30009384:	0ee6b280 	.word	0x0ee6b280

30009388 <PSRAM_PHY_StructInit>:
30009388:	2203      	movs	r2, #3
3000938a:	2301      	movs	r3, #1
3000938c:	210a      	movs	r1, #10
3000938e:	b410      	push	{r4}
30009390:	e9c0 2202 	strd	r2, r2, [r0, #8]
30009394:	2402      	movs	r4, #2
30009396:	2207      	movs	r2, #7
30009398:	e9c0 3400 	strd	r3, r4, [r0]
3000939c:	e9c0 1304 	strd	r1, r3, [r0, #16]
300093a0:	f85d 4b04 	ldr.w	r4, [sp], #4
300093a4:	6182      	str	r2, [r0, #24]
300093a6:	4770      	bx	lr

300093a8 <PSRAM_PHY_Init>:
300093a8:	b538      	push	{r3, r4, r5, lr}
300093aa:	4604      	mov	r4, r0
300093ac:	6903      	ldr	r3, [r0, #16]
300093ae:	4d14      	ldr	r5, [pc, #80]	; (30009400 <PSRAM_PHY_Init+0x58>)
300093b0:	f003 031f 	and.w	r3, r3, #31
300093b4:	4913      	ldr	r1, [pc, #76]	; (30009404 <PSRAM_PHY_Init+0x5c>)
300093b6:	e9d0 0200 	ldrd	r0, r2, [r0]
300093ba:	0612      	lsls	r2, r2, #24
300093bc:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
300093c0:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
300093c4:	4313      	orrs	r3, r2
300093c6:	68a2      	ldr	r2, [r4, #8]
300093c8:	0412      	lsls	r2, r2, #16
300093ca:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
300093ce:	4313      	orrs	r3, r2
300093d0:	68e2      	ldr	r2, [r4, #12]
300093d2:	0212      	lsls	r2, r2, #8
300093d4:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300093d8:	4313      	orrs	r3, r2
300093da:	606b      	str	r3, [r5, #4]
300093dc:	4788      	blx	r1
300093de:	2801      	cmp	r0, #1
300093e0:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
300093e4:	bf08      	it	eq
300093e6:	4a08      	ldreq	r2, [pc, #32]	; (30009408 <PSRAM_PHY_Init+0x60>)
300093e8:	ea4f 1303 	mov.w	r3, r3, lsl #4
300093ec:	f001 010f 	and.w	r1, r1, #15
300093f0:	bf18      	it	ne
300093f2:	4a06      	ldrne	r2, [pc, #24]	; (3000940c <PSRAM_PHY_Init+0x64>)
300093f4:	b2db      	uxtb	r3, r3
300093f6:	430b      	orrs	r3, r1
300093f8:	431a      	orrs	r2, r3
300093fa:	61aa      	str	r2, [r5, #24]
300093fc:	bd38      	pop	{r3, r4, r5, pc}
300093fe:	bf00      	nop
30009400:	41002000 	.word	0x41002000
30009404:	300080a5 	.word	0x300080a5
30009408:	80000e00 	.word	0x80000e00
3000940c:	80000f00 	.word	0x80000f00

30009410 <PSRAM_CTRL_Init>:
30009410:	b470      	push	{r4, r5, r6}
30009412:	4b4c      	ldr	r3, [pc, #304]	; (30009544 <PSRAM_CTRL_Init+0x134>)
30009414:	2400      	movs	r4, #0
30009416:	4a4c      	ldr	r2, [pc, #304]	; (30009548 <PSRAM_CTRL_Init+0x138>)
30009418:	f24d 0101 	movw	r1, #53249	; 0xd001
3000941c:	609c      	str	r4, [r3, #8]
3000941e:	2001      	movs	r0, #1
30009420:	601a      	str	r2, [r3, #0]
30009422:	4a4a      	ldr	r2, [pc, #296]	; (3000954c <PSRAM_CTRL_Init+0x13c>)
30009424:	6158      	str	r0, [r3, #20]
30009426:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
3000942a:	6811      	ldr	r1, [r2, #0]
3000942c:	2902      	cmp	r1, #2
3000942e:	d04a      	beq.n	300094c6 <PSRAM_CTRL_Init+0xb6>
30009430:	6954      	ldr	r4, [r2, #20]
30009432:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30009436:	69d0      	ldr	r0, [r2, #28]
30009438:	6915      	ldr	r5, [r2, #16]
3000943a:	fbb0 f0f4 	udiv	r0, r0, r4
3000943e:	fbb1 f1f4 	udiv	r1, r1, r4
30009442:	6994      	ldr	r4, [r2, #24]
30009444:	f005 020f 	and.w	r2, r5, #15
30009448:	f241 5551 	movw	r5, #5457	; 0x1551
3000944c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30009450:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009454:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30009458:	094a      	lsrs	r2, r1, #5
3000945a:	0181      	lsls	r1, r0, #6
3000945c:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30009460:	4d3b      	ldr	r5, [pc, #236]	; (30009550 <PSRAM_CTRL_Init+0x140>)
30009462:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30009466:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
3000946a:	00a2      	lsls	r2, r4, #2
3000946c:	4301      	orrs	r1, r0
3000946e:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
30009472:	3a01      	subs	r2, #1
30009474:	430d      	orrs	r5, r1
30009476:	f242 0120 	movw	r1, #8224	; 0x2020
3000947a:	f3c2 020b 	ubfx	r2, r2, #0, #12
3000947e:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
30009482:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
30009486:	f44f 2180 	mov.w	r1, #262144	; 0x40000
3000948a:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
3000948e:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
30009492:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30009496:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
3000949a:	4420      	add	r0, r4
3000949c:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300094a0:	f640 71fe 	movw	r1, #4094	; 0xffe
300094a4:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300094a8:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300094ac:	432a      	orrs	r2, r5
300094ae:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300094b2:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300094b6:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300094ba:	4a22      	ldr	r2, [pc, #136]	; (30009544 <PSRAM_CTRL_Init+0x134>)
300094bc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300094be:	061b      	lsls	r3, r3, #24
300094c0:	d5fc      	bpl.n	300094bc <PSRAM_CTRL_Init+0xac>
300094c2:	bc70      	pop	{r4, r5, r6}
300094c4:	4770      	bx	lr
300094c6:	6955      	ldr	r5, [r2, #20]
300094c8:	207d      	movs	r0, #125	; 0x7d
300094ca:	69d1      	ldr	r1, [r2, #28]
300094cc:	26a0      	movs	r6, #160	; 0xa0
300094ce:	6914      	ldr	r4, [r2, #16]
300094d0:	6992      	ldr	r2, [r2, #24]
300094d2:	f004 040f 	and.w	r4, r4, #15
300094d6:	0092      	lsls	r2, r2, #2
300094d8:	fbb1 f1f5 	udiv	r1, r1, r5
300094dc:	fbb0 f5f5 	udiv	r5, r0, r5
300094e0:	f245 5051 	movw	r0, #21841	; 0x5551
300094e4:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
300094e8:	f001 003f 	and.w	r0, r1, #63	; 0x3f
300094ec:	0189      	lsls	r1, r1, #6
300094ee:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
300094f2:	f641 4570 	movw	r5, #7280	; 0x1c70
300094f6:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300094fa:	4325      	orrs	r5, r4
300094fc:	2420      	movs	r4, #32
300094fe:	4308      	orrs	r0, r1
30009500:	1e51      	subs	r1, r2, #1
30009502:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30009506:	3a02      	subs	r2, #2
30009508:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
3000950c:	f3c1 010b 	ubfx	r1, r1, #0, #12
30009510:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009514:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30009518:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
3000951c:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30009520:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009524:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30009528:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
3000952c:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30009530:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009534:	4301      	orrs	r1, r0
30009536:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000953a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000953e:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30009542:	e7ba      	b.n	300094ba <PSRAM_CTRL_Init+0xaa>
30009544:	41012000 	.word	0x41012000
30009548:	00bfe000 	.word	0x00bfe000
3000954c:	300101a0 	.word	0x300101a0
30009550:	00107000 	.word	0x00107000

30009554 <PSRAM_REG_Write>:
30009554:	b430      	push	{r4, r5}
30009556:	4c20      	ldr	r4, [pc, #128]	; (300095d8 <PSRAM_REG_Write+0x84>)
30009558:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000955a:	f013 0301 	ands.w	r3, r3, #1
3000955e:	d1fb      	bne.n	30009558 <PSRAM_REG_Write+0x4>
30009560:	6825      	ldr	r5, [r4, #0]
30009562:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009566:	6025      	str	r5, [r4, #0]
30009568:	60a3      	str	r3, [r4, #8]
3000956a:	6825      	ldr	r5, [r4, #0]
3000956c:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009570:	6025      	str	r5, [r4, #0]
30009572:	6063      	str	r3, [r4, #4]
30009574:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30009578:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
3000957c:	23c0      	movs	r3, #192	; 0xc0
3000957e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009582:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009586:	0e03      	lsrs	r3, r0, #24
30009588:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000958c:	f3c0 4307 	ubfx	r3, r0, #16, #8
30009590:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009594:	f3c0 2307 	ubfx	r3, r0, #8, #8
30009598:	b2c0      	uxtb	r0, r0
3000959a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000959e:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300095a2:	b141      	cbz	r1, 300095b6 <PSRAM_REG_Write+0x62>
300095a4:	3a01      	subs	r2, #1
300095a6:	480c      	ldr	r0, [pc, #48]	; (300095d8 <PSRAM_REG_Write+0x84>)
300095a8:	4411      	add	r1, r2
300095aa:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300095ae:	428a      	cmp	r2, r1
300095b0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300095b4:	d1f9      	bne.n	300095aa <PSRAM_REG_Write+0x56>
300095b6:	4a08      	ldr	r2, [pc, #32]	; (300095d8 <PSRAM_REG_Write+0x84>)
300095b8:	2301      	movs	r3, #1
300095ba:	6093      	str	r3, [r2, #8]
300095bc:	6893      	ldr	r3, [r2, #8]
300095be:	07d9      	lsls	r1, r3, #31
300095c0:	d4fc      	bmi.n	300095bc <PSRAM_REG_Write+0x68>
300095c2:	4a05      	ldr	r2, [pc, #20]	; (300095d8 <PSRAM_REG_Write+0x84>)
300095c4:	6a93      	ldr	r3, [r2, #40]	; 0x28
300095c6:	07db      	lsls	r3, r3, #31
300095c8:	d4fc      	bmi.n	300095c4 <PSRAM_REG_Write+0x70>
300095ca:	6813      	ldr	r3, [r2, #0]
300095cc:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300095d0:	6013      	str	r3, [r2, #0]
300095d2:	bc30      	pop	{r4, r5}
300095d4:	4770      	bx	lr
300095d6:	bf00      	nop
300095d8:	41012000 	.word	0x41012000

300095dc <PSRAM_APM_DEVIC_Init>:
300095dc:	4b12      	ldr	r3, [pc, #72]	; (30009628 <PSRAM_APM_DEVIC_Init+0x4c>)
300095de:	2102      	movs	r1, #2
300095e0:	2000      	movs	r0, #0
300095e2:	b530      	push	{r4, r5, lr}
300095e4:	4c11      	ldr	r4, [pc, #68]	; (3000962c <PSRAM_APM_DEVIC_Init+0x50>)
300095e6:	b083      	sub	sp, #12
300095e8:	699d      	ldr	r5, [r3, #24]
300095ea:	466a      	mov	r2, sp
300095ec:	eb04 0c05 	add.w	ip, r4, r5
300095f0:	1eeb      	subs	r3, r5, #3
300095f2:	4c0f      	ldr	r4, [pc, #60]	; (30009630 <PSRAM_APM_DEVIC_Init+0x54>)
300095f4:	408b      	lsls	r3, r1
300095f6:	f81c cc03 	ldrb.w	ip, [ip, #-3]
300095fa:	f003 031c 	and.w	r3, r3, #28
300095fe:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30009602:	f043 0320 	orr.w	r3, r3, #32
30009606:	fa5f fc8c 	uxtb.w	ip, ip
3000960a:	f88d 3000 	strb.w	r3, [sp]
3000960e:	f88d c004 	strb.w	ip, [sp, #4]
30009612:	f88d 3001 	strb.w	r3, [sp, #1]
30009616:	f88d c005 	strb.w	ip, [sp, #5]
3000961a:	47a0      	blx	r4
3000961c:	aa01      	add	r2, sp, #4
3000961e:	2102      	movs	r1, #2
30009620:	2004      	movs	r0, #4
30009622:	47a0      	blx	r4
30009624:	b003      	add	sp, #12
30009626:	bd30      	pop	{r4, r5, pc}
30009628:	300101a0 	.word	0x300101a0
3000962c:	3000f5fc 	.word	0x3000f5fc
30009630:	30009555 	.word	0x30009555

30009634 <PSRAM_WB_REG_Write>:
30009634:	b430      	push	{r4, r5}
30009636:	4c1e      	ldr	r4, [pc, #120]	; (300096b0 <PSRAM_WB_REG_Write+0x7c>)
30009638:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000963a:	f013 0301 	ands.w	r3, r3, #1
3000963e:	d1fb      	bne.n	30009638 <PSRAM_WB_REG_Write+0x4>
30009640:	6825      	ldr	r5, [r4, #0]
30009642:	b2c0      	uxtb	r0, r0
30009644:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009648:	6025      	str	r5, [r4, #0]
3000964a:	60a3      	str	r3, [r4, #8]
3000964c:	6825      	ldr	r5, [r4, #0]
3000964e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009652:	6025      	str	r5, [r4, #0]
30009654:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30009658:	6063      	str	r3, [r4, #4]
3000965a:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000965e:	2560      	movs	r5, #96	; 0x60
30009660:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009664:	2501      	movs	r5, #1
30009666:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000966a:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
3000966e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009672:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009676:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
3000967a:	b141      	cbz	r1, 3000968e <PSRAM_WB_REG_Write+0x5a>
3000967c:	3a01      	subs	r2, #1
3000967e:	480c      	ldr	r0, [pc, #48]	; (300096b0 <PSRAM_WB_REG_Write+0x7c>)
30009680:	4411      	add	r1, r2
30009682:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30009686:	428a      	cmp	r2, r1
30009688:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000968c:	d1f9      	bne.n	30009682 <PSRAM_WB_REG_Write+0x4e>
3000968e:	4a08      	ldr	r2, [pc, #32]	; (300096b0 <PSRAM_WB_REG_Write+0x7c>)
30009690:	2301      	movs	r3, #1
30009692:	6093      	str	r3, [r2, #8]
30009694:	6893      	ldr	r3, [r2, #8]
30009696:	07d9      	lsls	r1, r3, #31
30009698:	d4fc      	bmi.n	30009694 <PSRAM_WB_REG_Write+0x60>
3000969a:	4a05      	ldr	r2, [pc, #20]	; (300096b0 <PSRAM_WB_REG_Write+0x7c>)
3000969c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000969e:	07db      	lsls	r3, r3, #31
300096a0:	d4fc      	bmi.n	3000969c <PSRAM_WB_REG_Write+0x68>
300096a2:	6813      	ldr	r3, [r2, #0]
300096a4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300096a8:	6013      	str	r3, [r2, #0]
300096aa:	bc30      	pop	{r4, r5}
300096ac:	4770      	bx	lr
300096ae:	bf00      	nop
300096b0:	41012000 	.word	0x41012000

300096b4 <PSRAM_WB_DEVIC_Init>:
300096b4:	490b      	ldr	r1, [pc, #44]	; (300096e4 <PSRAM_WB_DEVIC_Init+0x30>)
300096b6:	4b0c      	ldr	r3, [pc, #48]	; (300096e8 <PSRAM_WB_DEVIC_Init+0x34>)
300096b8:	b510      	push	{r4, lr}
300096ba:	6988      	ldr	r0, [r1, #24]
300096bc:	b082      	sub	sp, #8
300096be:	248f      	movs	r4, #143	; 0x8f
300096c0:	2102      	movs	r1, #2
300096c2:	4403      	add	r3, r0
300096c4:	aa01      	add	r2, sp, #4
300096c6:	f88d 4004 	strb.w	r4, [sp, #4]
300096ca:	2000      	movs	r0, #0
300096cc:	f813 3c03 	ldrb.w	r3, [r3, #-3]
300096d0:	4c06      	ldr	r4, [pc, #24]	; (300096ec <PSRAM_WB_DEVIC_Init+0x38>)
300096d2:	011b      	lsls	r3, r3, #4
300096d4:	f043 030c 	orr.w	r3, r3, #12
300096d8:	f88d 3005 	strb.w	r3, [sp, #5]
300096dc:	47a0      	blx	r4
300096de:	b002      	add	sp, #8
300096e0:	bd10      	pop	{r4, pc}
300096e2:	bf00      	nop
300096e4:	300101a0 	.word	0x300101a0
300096e8:	3000f61c 	.word	0x3000f61c
300096ec:	30009635 	.word	0x30009635

300096f0 <PSRAM_calibration>:
300096f0:	4a97      	ldr	r2, [pc, #604]	; (30009950 <PSRAM_calibration+0x260>)
300096f2:	2300      	movs	r3, #0
300096f4:	4997      	ldr	r1, [pc, #604]	; (30009954 <PSRAM_calibration+0x264>)
300096f6:	4898      	ldr	r0, [pc, #608]	; (30009958 <PSRAM_calibration+0x268>)
300096f8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300096fc:	6852      	ldr	r2, [r2, #4]
300096fe:	b08f      	sub	sp, #60	; 0x3c
30009700:	e9cd 2307 	strd	r2, r3, [sp, #28]
30009704:	400a      	ands	r2, r1
30009706:	930d      	str	r3, [sp, #52]	; 0x34
30009708:	e9cd 3309 	strd	r3, r3, [sp, #36]	; 0x24
3000970c:	e9cd 330b 	strd	r3, r3, [sp, #44]	; 0x2c
30009710:	9206      	str	r2, [sp, #24]
30009712:	6942      	ldr	r2, [r0, #20]
30009714:	03d1      	lsls	r1, r2, #15
30009716:	d51e      	bpl.n	30009756 <PSRAM_calibration+0x66>
30009718:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000971c:	f3bf 8f4f 	dsb	sy
30009720:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009724:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009728:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000972c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009730:	0164      	lsls	r4, r4, #5
30009732:	ea04 0106 	and.w	r1, r4, r6
30009736:	462b      	mov	r3, r5
30009738:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000973c:	3b01      	subs	r3, #1
3000973e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009742:	1c5a      	adds	r2, r3, #1
30009744:	d1f8      	bne.n	30009738 <PSRAM_calibration+0x48>
30009746:	3c20      	subs	r4, #32
30009748:	f114 0f20 	cmn.w	r4, #32
3000974c:	d1f1      	bne.n	30009732 <PSRAM_calibration+0x42>
3000974e:	f3bf 8f4f 	dsb	sy
30009752:	f3bf 8f6f 	isb	sy
30009756:	4880      	ldr	r0, [pc, #512]	; (30009958 <PSRAM_calibration+0x268>)
30009758:	2300      	movs	r3, #0
3000975a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000975e:	f3bf 8f4f 	dsb	sy
30009762:	6943      	ldr	r3, [r0, #20]
30009764:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009768:	6143      	str	r3, [r0, #20]
3000976a:	f3bf 8f4f 	dsb	sy
3000976e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009772:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009776:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000977a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000977e:	0164      	lsls	r4, r4, #5
30009780:	ea04 0106 	and.w	r1, r4, r6
30009784:	462b      	mov	r3, r5
30009786:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000978a:	3b01      	subs	r3, #1
3000978c:	1c5f      	adds	r7, r3, #1
3000978e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009792:	d1f8      	bne.n	30009786 <PSRAM_calibration+0x96>
30009794:	3c20      	subs	r4, #32
30009796:	f114 0f20 	cmn.w	r4, #32
3000979a:	d1f1      	bne.n	30009780 <PSRAM_calibration+0x90>
3000979c:	f3bf 8f4f 	dsb	sy
300097a0:	f3bf 8f6f 	isb	sy
300097a4:	2204      	movs	r2, #4
300097a6:	461e      	mov	r6, r3
300097a8:	9302      	str	r3, [sp, #8]
300097aa:	2700      	movs	r7, #0
300097ac:	9205      	str	r2, [sp, #20]
300097ae:	46b3      	mov	fp, r6
300097b0:	4a67      	ldr	r2, [pc, #412]	; (30009950 <PSRAM_calibration+0x260>)
300097b2:	4c6a      	ldr	r4, [pc, #424]	; (3000995c <PSRAM_calibration+0x26c>)
300097b4:	6813      	ldr	r3, [r2, #0]
300097b6:	f8df 91b8 	ldr.w	r9, [pc, #440]	; 30009970 <PSRAM_calibration+0x280>
300097ba:	f023 0301 	bic.w	r3, r3, #1
300097be:	f8df a1b4 	ldr.w	sl, [pc, #436]	; 30009974 <PSRAM_calibration+0x284>
300097c2:	6013      	str	r3, [r2, #0]
300097c4:	2301      	movs	r3, #1
300097c6:	9301      	str	r3, [sp, #4]
300097c8:	9b06      	ldr	r3, [sp, #24]
300097ca:	9a01      	ldr	r2, [sp, #4]
300097cc:	9700      	str	r7, [sp, #0]
300097ce:	2700      	movs	r7, #0
300097d0:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
300097d4:	463d      	mov	r5, r7
300097d6:	e9cd 6b03 	strd	r6, fp, [sp, #12]
300097da:	f04f 3bff 	mov.w	fp, #4294967295
300097de:	465e      	mov	r6, fp
300097e0:	e010      	b.n	30009804 <PSRAM_calibration+0x114>
300097e2:	f1b6 3fff 	cmp.w	r6, #4294967295
300097e6:	bf08      	it	eq
300097e8:	462e      	moveq	r6, r5
300097ea:	2d1f      	cmp	r5, #31
300097ec:	eb06 0b07 	add.w	fp, r6, r7
300097f0:	f107 0701 	add.w	r7, r7, #1
300097f4:	d102      	bne.n	300097fc <PSRAM_calibration+0x10c>
300097f6:	9b00      	ldr	r3, [sp, #0]
300097f8:	429f      	cmp	r7, r3
300097fa:	dc46      	bgt.n	3000988a <PSRAM_calibration+0x19a>
300097fc:	3501      	adds	r5, #1
300097fe:	2d20      	cmp	r5, #32
30009800:	f000 80a1 	beq.w	30009946 <PSRAM_calibration+0x256>
30009804:	ea48 0205 	orr.w	r2, r8, r5
30009808:	4b51      	ldr	r3, [pc, #324]	; (30009950 <PSRAM_calibration+0x260>)
3000980a:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000980e:	4954      	ldr	r1, [pc, #336]	; (30009960 <PSRAM_calibration+0x270>)
30009810:	605a      	str	r2, [r3, #4]
30009812:	6822      	ldr	r2, [r4, #0]
30009814:	4b53      	ldr	r3, [pc, #332]	; (30009964 <PSRAM_calibration+0x274>)
30009816:	f8ce 2000 	str.w	r2, [lr]
3000981a:	6860      	ldr	r0, [r4, #4]
3000981c:	4a52      	ldr	r2, [pc, #328]	; (30009968 <PSRAM_calibration+0x278>)
3000981e:	f8c9 0000 	str.w	r0, [r9]
30009822:	68a0      	ldr	r0, [r4, #8]
30009824:	f8ca 0000 	str.w	r0, [sl]
30009828:	68e0      	ldr	r0, [r4, #12]
3000982a:	6018      	str	r0, [r3, #0]
3000982c:	6920      	ldr	r0, [r4, #16]
3000982e:	6008      	str	r0, [r1, #0]
30009830:	6960      	ldr	r0, [r4, #20]
30009832:	6010      	str	r0, [r2, #0]
30009834:	f8de 0000 	ldr.w	r0, [lr]
30009838:	9008      	str	r0, [sp, #32]
3000983a:	f8d9 0000 	ldr.w	r0, [r9]
3000983e:	9009      	str	r0, [sp, #36]	; 0x24
30009840:	f8da 0000 	ldr.w	r0, [sl]
30009844:	900a      	str	r0, [sp, #40]	; 0x28
30009846:	6818      	ldr	r0, [r3, #0]
30009848:	4b48      	ldr	r3, [pc, #288]	; (3000996c <PSRAM_calibration+0x27c>)
3000984a:	900b      	str	r0, [sp, #44]	; 0x2c
3000984c:	a808      	add	r0, sp, #32
3000984e:	6809      	ldr	r1, [r1, #0]
30009850:	910c      	str	r1, [sp, #48]	; 0x30
30009852:	6812      	ldr	r2, [r2, #0]
30009854:	4941      	ldr	r1, [pc, #260]	; (3000995c <PSRAM_calibration+0x26c>)
30009856:	920d      	str	r2, [sp, #52]	; 0x34
30009858:	2218      	movs	r2, #24
3000985a:	4798      	blx	r3
3000985c:	2800      	cmp	r0, #0
3000985e:	d0c0      	beq.n	300097e2 <PSRAM_calibration+0xf2>
30009860:	1c70      	adds	r0, r6, #1
30009862:	d0cb      	beq.n	300097fc <PSRAM_calibration+0x10c>
30009864:	9b00      	ldr	r3, [sp, #0]
30009866:	42bb      	cmp	r3, r7
30009868:	da0a      	bge.n	30009880 <PSRAM_calibration+0x190>
3000986a:	9b01      	ldr	r3, [sp, #4]
3000986c:	f8cd b010 	str.w	fp, [sp, #16]
30009870:	f04f 3bff 	mov.w	fp, #4294967295
30009874:	9700      	str	r7, [sp, #0]
30009876:	2700      	movs	r7, #0
30009878:	9603      	str	r6, [sp, #12]
3000987a:	465e      	mov	r6, fp
3000987c:	9302      	str	r3, [sp, #8]
3000987e:	e7bd      	b.n	300097fc <PSRAM_calibration+0x10c>
30009880:	f04f 3bff 	mov.w	fp, #4294967295
30009884:	2700      	movs	r7, #0
30009886:	465e      	mov	r6, fp
30009888:	e7b8      	b.n	300097fc <PSRAM_calibration+0x10c>
3000988a:	9b01      	ldr	r3, [sp, #4]
3000988c:	9302      	str	r3, [sp, #8]
3000988e:	9b01      	ldr	r3, [sp, #4]
30009890:	005b      	lsls	r3, r3, #1
30009892:	9301      	str	r3, [sp, #4]
30009894:	9b05      	ldr	r3, [sp, #20]
30009896:	3b01      	subs	r3, #1
30009898:	9305      	str	r3, [sp, #20]
3000989a:	d195      	bne.n	300097c8 <PSRAM_calibration+0xd8>
3000989c:	482e      	ldr	r0, [pc, #184]	; (30009958 <PSRAM_calibration+0x268>)
3000989e:	6943      	ldr	r3, [r0, #20]
300098a0:	03d9      	lsls	r1, r3, #15
300098a2:	d425      	bmi.n	300098f0 <PSRAM_calibration+0x200>
300098a4:	9b05      	ldr	r3, [sp, #20]
300098a6:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300098aa:	f3bf 8f4f 	dsb	sy
300098ae:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300098b2:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
300098b6:	f3c5 344e 	ubfx	r4, r5, #13, #15
300098ba:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300098be:	0164      	lsls	r4, r4, #5
300098c0:	ea04 010c 	and.w	r1, r4, ip
300098c4:	462b      	mov	r3, r5
300098c6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300098ca:	3b01      	subs	r3, #1
300098cc:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300098d0:	1c5a      	adds	r2, r3, #1
300098d2:	d1f8      	bne.n	300098c6 <PSRAM_calibration+0x1d6>
300098d4:	3c20      	subs	r4, #32
300098d6:	f114 0f20 	cmn.w	r4, #32
300098da:	d1f1      	bne.n	300098c0 <PSRAM_calibration+0x1d0>
300098dc:	f3bf 8f4f 	dsb	sy
300098e0:	6943      	ldr	r3, [r0, #20]
300098e2:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300098e6:	6143      	str	r3, [r0, #20]
300098e8:	f3bf 8f4f 	dsb	sy
300098ec:	f3bf 8f6f 	isb	sy
300098f0:	2f08      	cmp	r7, #8
300098f2:	dd24      	ble.n	3000993e <PSRAM_calibration+0x24e>
300098f4:	ebbb 0306 	subs.w	r3, fp, r6
300098f8:	9802      	ldr	r0, [sp, #8]
300098fa:	bf48      	it	mi
300098fc:	3301      	addmi	r3, #1
300098fe:	eb1b 0606 	adds.w	r6, fp, r6
30009902:	ea4f 0363 	mov.w	r3, r3, asr #1
30009906:	bf48      	it	mi
30009908:	3601      	addmi	r6, #1
3000990a:	1e9a      	subs	r2, r3, #2
3000990c:	9b07      	ldr	r3, [sp, #28]
3000990e:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009912:	0414      	lsls	r4, r2, #16
30009914:	f3c6 0344 	ubfx	r3, r6, #1, #5
30009918:	0212      	lsls	r2, r2, #8
3000991a:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
3000991e:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009922:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009926:	2001      	movs	r0, #1
30009928:	430b      	orrs	r3, r1
3000992a:	4909      	ldr	r1, [pc, #36]	; (30009950 <PSRAM_calibration+0x260>)
3000992c:	4323      	orrs	r3, r4
3000992e:	4313      	orrs	r3, r2
30009930:	604b      	str	r3, [r1, #4]
30009932:	680b      	ldr	r3, [r1, #0]
30009934:	4303      	orrs	r3, r0
30009936:	600b      	str	r3, [r1, #0]
30009938:	b00f      	add	sp, #60	; 0x3c
3000993a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000993e:	2000      	movs	r0, #0
30009940:	b00f      	add	sp, #60	; 0x3c
30009942:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009946:	9f00      	ldr	r7, [sp, #0]
30009948:	e9dd 6b03 	ldrd	r6, fp, [sp, #12]
3000994c:	e79f      	b.n	3000988e <PSRAM_calibration+0x19e>
3000994e:	bf00      	nop
30009950:	41002000 	.word	0x41002000
30009954:	0fffffe0 	.word	0x0fffffe0
30009958:	e000ed00 	.word	0xe000ed00
3000995c:	3000f604 	.word	0x3000f604
30009960:	60200000 	.word	0x60200000
30009964:	60150000 	.word	0x60150000
30009968:	60250000 	.word	0x60250000
3000996c:	000129bd 	.word	0x000129bd
30009970:	60050000 	.word	0x60050000
30009974:	60100000 	.word	0x60100000

30009978 <PSRAM_AutoGating>:
30009978:	b1c0      	cbz	r0, 300099ac <PSRAM_AutoGating+0x34>
3000997a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000997e:	4810      	ldr	r0, [pc, #64]	; (300099c0 <PSRAM_AutoGating+0x48>)
30009980:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009984:	0252      	lsls	r2, r2, #9
30009986:	b410      	push	{r4}
30009988:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
3000998c:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30009990:	4020      	ands	r0, r4
30009992:	4301      	orrs	r1, r0
30009994:	430a      	orrs	r2, r1
30009996:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
3000999a:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
3000999e:	f042 0201 	orr.w	r2, r2, #1
300099a2:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300099a6:	f85d 4b04 	ldr.w	r4, [sp], #4
300099aa:	4770      	bx	lr
300099ac:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300099b0:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300099b4:	f023 0301 	bic.w	r3, r3, #1
300099b8:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300099bc:	4770      	bx	lr
300099be:	bf00      	nop
300099c0:	ffffc000 	.word	0xffffc000

300099c4 <BOOT_Reason>:
300099c4:	4b02      	ldr	r3, [pc, #8]	; (300099d0 <BOOT_Reason+0xc>)
300099c6:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
300099ca:	b280      	uxth	r0, r0
300099cc:	4770      	bx	lr
300099ce:	bf00      	nop
300099d0:	42008200 	.word	0x42008200

300099d4 <CRYPTO_SetSecurityModeAD>:
300099d4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300099d8:	b083      	sub	sp, #12
300099da:	460d      	mov	r5, r1
300099dc:	4616      	mov	r6, r2
300099de:	4699      	mov	r9, r3
300099e0:	4604      	mov	r4, r0
300099e2:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
300099e6:	2800      	cmp	r0, #0
300099e8:	f000 80a9 	beq.w	30009b3e <CRYPTO_SetSecurityModeAD+0x16a>
300099ec:	7823      	ldrb	r3, [r4, #0]
300099ee:	2b01      	cmp	r3, #1
300099f0:	d003      	beq.n	300099fa <CRYPTO_SetSecurityModeAD+0x26>
300099f2:	2125      	movs	r1, #37	; 0x25
300099f4:	485d      	ldr	r0, [pc, #372]	; (30009b6c <CRYPTO_SetSecurityModeAD+0x198>)
300099f6:	f002 fe83 	bl	3000c700 <__io_assert_failed_veneer>
300099fa:	2300      	movs	r3, #0
300099fc:	1c6a      	adds	r2, r5, #1
300099fe:	6065      	str	r5, [r4, #4]
30009a00:	8123      	strh	r3, [r4, #8]
30009a02:	d008      	beq.n	30009a16 <CRYPTO_SetSecurityModeAD+0x42>
30009a04:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009a08:	2b20      	cmp	r3, #32
30009a0a:	d07b      	beq.n	30009b04 <CRYPTO_SetSecurityModeAD+0x130>
30009a0c:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30009a10:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009a14:	7265      	strb	r5, [r4, #9]
30009a16:	1c73      	adds	r3, r6, #1
30009a18:	60e6      	str	r6, [r4, #12]
30009a1a:	f000 8082 	beq.w	30009b22 <CRYPTO_SetSecurityModeAD+0x14e>
30009a1e:	f006 0303 	and.w	r3, r6, #3
30009a22:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009a26:	7422      	strb	r2, [r4, #16]
30009a28:	f1a3 0202 	sub.w	r2, r3, #2
30009a2c:	2b01      	cmp	r3, #1
30009a2e:	fab2 f282 	clz	r2, r2
30009a32:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009a36:	7462      	strb	r2, [r4, #17]
30009a38:	fab3 f283 	clz	r2, r3
30009a3c:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009a40:	74a2      	strb	r2, [r4, #18]
30009a42:	bf0c      	ite	eq
30009a44:	2201      	moveq	r2, #1
30009a46:	2200      	movne	r2, #0
30009a48:	74e2      	strb	r2, [r4, #19]
30009a4a:	d13f      	bne.n	30009acc <CRYPTO_SetSecurityModeAD+0xf8>
30009a4c:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009a50:	2b40      	cmp	r3, #64	; 0x40
30009a52:	f000 8083 	beq.w	30009b5c <CRYPTO_SetSecurityModeAD+0x188>
30009a56:	d86a      	bhi.n	30009b2e <CRYPTO_SetSecurityModeAD+0x15a>
30009a58:	2b10      	cmp	r3, #16
30009a5a:	d06d      	beq.n	30009b38 <CRYPTO_SetSecurityModeAD+0x164>
30009a5c:	2b20      	cmp	r3, #32
30009a5e:	d173      	bne.n	30009b48 <CRYPTO_SetSecurityModeAD+0x174>
30009a60:	7523      	strb	r3, [r4, #20]
30009a62:	62a3      	str	r3, [r4, #40]	; 0x28
30009a64:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009a66:	6227      	str	r7, [r4, #32]
30009a68:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009a6c:	2f00      	cmp	r7, #0
30009a6e:	d038      	beq.n	30009ae2 <CRYPTO_SetSecurityModeAD+0x10e>
30009a70:	7d23      	ldrb	r3, [r4, #20]
30009a72:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009a76:	2b30      	cmp	r3, #48	; 0x30
30009a78:	65e0      	str	r0, [r4, #92]	; 0x5c
30009a7a:	d046      	beq.n	30009b0a <CRYPTO_SetSecurityModeAD+0x136>
30009a7c:	2b40      	cmp	r3, #64	; 0x40
30009a7e:	d044      	beq.n	30009b0a <CRYPTO_SetSecurityModeAD+0x136>
30009a80:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009a84:	2240      	movs	r2, #64	; 0x40
30009a86:	2136      	movs	r1, #54	; 0x36
30009a88:	4d39      	ldr	r5, [pc, #228]	; (30009b70 <CRYPTO_SetSecurityModeAD+0x19c>)
30009a8a:	6623      	str	r3, [r4, #96]	; 0x60
30009a8c:	47a8      	blx	r5
30009a8e:	2240      	movs	r2, #64	; 0x40
30009a90:	215c      	movs	r1, #92	; 0x5c
30009a92:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009a94:	47a8      	blx	r5
30009a96:	4b37      	ldr	r3, [pc, #220]	; (30009b74 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009a98:	681d      	ldr	r5, [r3, #0]
30009a9a:	bb25      	cbnz	r5, 30009ae6 <CRYPTO_SetSecurityModeAD+0x112>
30009a9c:	f1b8 0f00 	cmp.w	r8, #0
30009aa0:	d05f      	beq.n	30009b62 <CRYPTO_SetSecurityModeAD+0x18e>
30009aa2:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009aa6:	e001      	b.n	30009aac <CRYPTO_SetSecurityModeAD+0xd8>
30009aa8:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009aac:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30009aae:	f818 3005 	ldrb.w	r3, [r8, r5]
30009ab2:	5d51      	ldrb	r1, [r2, r5]
30009ab4:	404b      	eors	r3, r1
30009ab6:	5553      	strb	r3, [r2, r5]
30009ab8:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009aba:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009abc:	5d51      	ldrb	r1, [r2, r5]
30009abe:	5d5b      	ldrb	r3, [r3, r5]
30009ac0:	404b      	eors	r3, r1
30009ac2:	5553      	strb	r3, [r2, r5]
30009ac4:	3501      	adds	r5, #1
30009ac6:	42af      	cmp	r7, r5
30009ac8:	d1ee      	bne.n	30009aa8 <CRYPTO_SetSecurityModeAD+0xd4>
30009aca:	e00c      	b.n	30009ae6 <CRYPTO_SetSecurityModeAD+0x112>
30009acc:	2200      	movs	r2, #0
30009ace:	7522      	strb	r2, [r4, #20]
30009ad0:	b99b      	cbnz	r3, 30009afa <CRYPTO_SetSecurityModeAD+0x126>
30009ad2:	2314      	movs	r3, #20
30009ad4:	6227      	str	r7, [r4, #32]
30009ad6:	62a3      	str	r3, [r4, #40]	; 0x28
30009ad8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009ada:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009ade:	2f00      	cmp	r7, #0
30009ae0:	d1c6      	bne.n	30009a70 <CRYPTO_SetSecurityModeAD+0x9c>
30009ae2:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009ae6:	3601      	adds	r6, #1
30009ae8:	d003      	beq.n	30009af2 <CRYPTO_SetSecurityModeAD+0x11e>
30009aea:	2201      	movs	r2, #1
30009aec:	2300      	movs	r3, #0
30009aee:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009af0:	6323      	str	r3, [r4, #48]	; 0x30
30009af2:	2000      	movs	r0, #0
30009af4:	b003      	add	sp, #12
30009af6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009afa:	2b02      	cmp	r3, #2
30009afc:	d114      	bne.n	30009b28 <CRYPTO_SetSecurityModeAD+0x154>
30009afe:	2310      	movs	r3, #16
30009b00:	62a3      	str	r3, [r4, #40]	; 0x28
30009b02:	e7af      	b.n	30009a64 <CRYPTO_SetSecurityModeAD+0x90>
30009b04:	2301      	movs	r3, #1
30009b06:	7223      	strb	r3, [r4, #8]
30009b08:	e780      	b.n	30009a0c <CRYPTO_SetSecurityModeAD+0x38>
30009b0a:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009b0e:	2280      	movs	r2, #128	; 0x80
30009b10:	2136      	movs	r1, #54	; 0x36
30009b12:	4d17      	ldr	r5, [pc, #92]	; (30009b70 <CRYPTO_SetSecurityModeAD+0x19c>)
30009b14:	6623      	str	r3, [r4, #96]	; 0x60
30009b16:	47a8      	blx	r5
30009b18:	2280      	movs	r2, #128	; 0x80
30009b1a:	215c      	movs	r1, #92	; 0x5c
30009b1c:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009b1e:	47a8      	blx	r5
30009b20:	e7b9      	b.n	30009a96 <CRYPTO_SetSecurityModeAD+0xc2>
30009b22:	2300      	movs	r3, #0
30009b24:	6123      	str	r3, [r4, #16]
30009b26:	7523      	strb	r3, [r4, #20]
30009b28:	2300      	movs	r3, #0
30009b2a:	62a3      	str	r3, [r4, #40]	; 0x28
30009b2c:	e79a      	b.n	30009a64 <CRYPTO_SetSecurityModeAD+0x90>
30009b2e:	2b80      	cmp	r3, #128	; 0x80
30009b30:	d10a      	bne.n	30009b48 <CRYPTO_SetSecurityModeAD+0x174>
30009b32:	2340      	movs	r3, #64	; 0x40
30009b34:	7523      	strb	r3, [r4, #20]
30009b36:	e794      	b.n	30009a62 <CRYPTO_SetSecurityModeAD+0x8e>
30009b38:	231c      	movs	r3, #28
30009b3a:	7523      	strb	r3, [r4, #20]
30009b3c:	e7f5      	b.n	30009b2a <CRYPTO_SetSecurityModeAD+0x156>
30009b3e:	2124      	movs	r1, #36	; 0x24
30009b40:	480a      	ldr	r0, [pc, #40]	; (30009b6c <CRYPTO_SetSecurityModeAD+0x198>)
30009b42:	f002 fddd 	bl	3000c700 <__io_assert_failed_veneer>
30009b46:	e751      	b.n	300099ec <CRYPTO_SetSecurityModeAD+0x18>
30009b48:	4b0b      	ldr	r3, [pc, #44]	; (30009b78 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009b4a:	2002      	movs	r0, #2
30009b4c:	4a0b      	ldr	r2, [pc, #44]	; (30009b7c <CRYPTO_SetSecurityModeAD+0x1a8>)
30009b4e:	4619      	mov	r1, r3
30009b50:	9600      	str	r6, [sp, #0]
30009b52:	f002 fb59 	bl	3000c208 <rtk_log_write>
30009b56:	2300      	movs	r3, #0
30009b58:	7523      	strb	r3, [r4, #20]
30009b5a:	e7e6      	b.n	30009b2a <CRYPTO_SetSecurityModeAD+0x156>
30009b5c:	2330      	movs	r3, #48	; 0x30
30009b5e:	7523      	strb	r3, [r4, #20]
30009b60:	e7e3      	b.n	30009b2a <CRYPTO_SetSecurityModeAD+0x156>
30009b62:	2180      	movs	r1, #128	; 0x80
30009b64:	4801      	ldr	r0, [pc, #4]	; (30009b6c <CRYPTO_SetSecurityModeAD+0x198>)
30009b66:	f002 fdcb 	bl	3000c700 <__io_assert_failed_veneer>
30009b6a:	e79a      	b.n	30009aa2 <CRYPTO_SetSecurityModeAD+0xce>
30009b6c:	3000dd48 	.word	0x3000dd48
30009b70:	00012be5 	.word	0x00012be5
30009b74:	2001c200 	.word	0x2001c200
30009b78:	3000dcac 	.word	0x3000dcac
30009b7c:	3000dcb8 	.word	0x3000dcb8

30009b80 <rtl_crypto_hmac_sha2_init>:
30009b80:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009b84:	4c97      	ldr	r4, [pc, #604]	; (30009de4 <rtl_crypto_hmac_sha2_init+0x264>)
30009b86:	4617      	mov	r7, r2
30009b88:	e844 f400 	tt	r4, r4
30009b8c:	4a96      	ldr	r2, [pc, #600]	; (30009de8 <rtl_crypto_hmac_sha2_init+0x268>)
30009b8e:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30009b92:	461c      	mov	r4, r3
30009b94:	4b95      	ldr	r3, [pc, #596]	; (30009dec <rtl_crypto_hmac_sha2_init+0x26c>)
30009b96:	bf18      	it	ne
30009b98:	4613      	movne	r3, r2
30009b9a:	b085      	sub	sp, #20
30009b9c:	4605      	mov	r5, r0
30009b9e:	460e      	mov	r6, r1
30009ba0:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30009ba2:	2b00      	cmp	r3, #0
30009ba4:	db05      	blt.n	30009bb2 <rtl_crypto_hmac_sha2_init+0x32>
30009ba6:	2900      	cmp	r1, #0
30009ba8:	f000 8118 	beq.w	30009ddc <rtl_crypto_hmac_sha2_init+0x25c>
30009bac:	078b      	lsls	r3, r1, #30
30009bae:	f040 8112 	bne.w	30009dd6 <rtl_crypto_hmac_sha2_init+0x256>
30009bb2:	2d20      	cmp	r5, #32
30009bb4:	bf18      	it	ne
30009bb6:	2d1c      	cmpne	r5, #28
30009bb8:	bf0c      	ite	eq
30009bba:	f04f 0901 	moveq.w	r9, #1
30009bbe:	f04f 0900 	movne.w	r9, #0
30009bc2:	d04d      	beq.n	30009c60 <rtl_crypto_hmac_sha2_init+0xe0>
30009bc4:	2d40      	cmp	r5, #64	; 0x40
30009bc6:	bf18      	it	ne
30009bc8:	2d30      	cmpne	r5, #48	; 0x30
30009bca:	d102      	bne.n	30009bd2 <rtl_crypto_hmac_sha2_init+0x52>
30009bcc:	2f80      	cmp	r7, #128	; 0x80
30009bce:	f200 80f9 	bhi.w	30009dc4 <rtl_crypto_hmac_sha2_init+0x244>
30009bd2:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009e0c <rtl_crypto_hmac_sha2_init+0x28c>
30009bd6:	f898 3000 	ldrb.w	r3, [r8]
30009bda:	2b01      	cmp	r3, #1
30009bdc:	d003      	beq.n	30009be6 <rtl_crypto_hmac_sha2_init+0x66>
30009bde:	21b4      	movs	r1, #180	; 0xb4
30009be0:	4883      	ldr	r0, [pc, #524]	; (30009df0 <rtl_crypto_hmac_sha2_init+0x270>)
30009be2:	f002 fd8d 	bl	3000c700 <__io_assert_failed_veneer>
30009be6:	2d30      	cmp	r5, #48	; 0x30
30009be8:	f000 809b 	beq.w	30009d22 <rtl_crypto_hmac_sha2_init+0x1a2>
30009bec:	d83f      	bhi.n	30009c6e <rtl_crypto_hmac_sha2_init+0xee>
30009bee:	2d1c      	cmp	r5, #28
30009bf0:	f000 80cb 	beq.w	30009d8a <rtl_crypto_hmac_sha2_init+0x20a>
30009bf4:	f1b9 0f00 	cmp.w	r9, #0
30009bf8:	d078      	beq.n	30009cec <rtl_crypto_hmac_sha2_init+0x16c>
30009bfa:	2c00      	cmp	r4, #0
30009bfc:	f000 808f 	beq.w	30009d1e <rtl_crypto_hmac_sha2_init+0x19e>
30009c00:	4b7c      	ldr	r3, [pc, #496]	; (30009df4 <rtl_crypto_hmac_sha2_init+0x274>)
30009c02:	f8df e210 	ldr.w	lr, [pc, #528]	; 30009e14 <rtl_crypto_hmac_sha2_init+0x294>
30009c06:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30009e24 <rtl_crypto_hmac_sha2_init+0x2a4>
30009c0a:	4a7b      	ldr	r2, [pc, #492]	; (30009df8 <rtl_crypto_hmac_sha2_init+0x278>)
30009c0c:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009c10:	4b7a      	ldr	r3, [pc, #488]	; (30009dfc <rtl_crypto_hmac_sha2_init+0x27c>)
30009c12:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009c16:	4b7a      	ldr	r3, [pc, #488]	; (30009e00 <rtl_crypto_hmac_sha2_init+0x280>)
30009c18:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009c1c:	4b79      	ldr	r3, [pc, #484]	; (30009e04 <rtl_crypto_hmac_sha2_init+0x284>)
30009c1e:	2225      	movs	r2, #37	; 0x25
30009c20:	6223      	str	r3, [r4, #32]
30009c22:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009c26:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009c2a:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009c2e:	6263      	str	r3, [r4, #36]	; 0x24
30009c30:	2300      	movs	r3, #0
30009c32:	f04f 31ff 	mov.w	r1, #4294967295
30009c36:	4d74      	ldr	r5, [pc, #464]	; (30009e08 <rtl_crypto_hmac_sha2_init+0x288>)
30009c38:	9300      	str	r3, [sp, #0]
30009c3a:	9702      	str	r7, [sp, #8]
30009c3c:	9601      	str	r6, [sp, #4]
30009c3e:	4873      	ldr	r0, [pc, #460]	; (30009e0c <rtl_crypto_hmac_sha2_init+0x28c>)
30009c40:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009c44:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009c48:	47a8      	blx	r5
30009c4a:	4605      	mov	r5, r0
30009c4c:	4b70      	ldr	r3, [pc, #448]	; (30009e10 <rtl_crypto_hmac_sha2_init+0x290>)
30009c4e:	4620      	mov	r0, r4
30009c50:	4798      	blx	r3
30009c52:	2301      	movs	r3, #1
30009c54:	4628      	mov	r0, r5
30009c56:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009c5a:	b005      	add	sp, #20
30009c5c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009c60:	2f40      	cmp	r7, #64	; 0x40
30009c62:	d9b6      	bls.n	30009bd2 <rtl_crypto_hmac_sha2_init+0x52>
30009c64:	21ae      	movs	r1, #174	; 0xae
30009c66:	4862      	ldr	r0, [pc, #392]	; (30009df0 <rtl_crypto_hmac_sha2_init+0x270>)
30009c68:	f002 fd4a 	bl	3000c700 <__io_assert_failed_veneer>
30009c6c:	e7b1      	b.n	30009bd2 <rtl_crypto_hmac_sha2_init+0x52>
30009c6e:	2d40      	cmp	r5, #64	; 0x40
30009c70:	d13c      	bne.n	30009cec <rtl_crypto_hmac_sha2_init+0x16c>
30009c72:	2c00      	cmp	r4, #0
30009c74:	d040      	beq.n	30009cf8 <rtl_crypto_hmac_sha2_init+0x178>
30009c76:	4b67      	ldr	r3, [pc, #412]	; (30009e14 <rtl_crypto_hmac_sha2_init+0x294>)
30009c78:	4a67      	ldr	r2, [pc, #412]	; (30009e18 <rtl_crypto_hmac_sha2_init+0x298>)
30009c7a:	4968      	ldr	r1, [pc, #416]	; (30009e1c <rtl_crypto_hmac_sha2_init+0x29c>)
30009c7c:	4d68      	ldr	r5, [pc, #416]	; (30009e20 <rtl_crypto_hmac_sha2_init+0x2a0>)
30009c7e:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009e64 <rtl_crypto_hmac_sha2_init+0x2e4>
30009c82:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009c86:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009c8a:	2285      	movs	r2, #133	; 0x85
30009c8c:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30009c90:	33d4      	adds	r3, #212	; 0xd4
30009c92:	6163      	str	r3, [r4, #20]
30009c94:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009c98:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009c9c:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30009ca0:	6123      	str	r3, [r4, #16]
30009ca2:	4b60      	ldr	r3, [pc, #384]	; (30009e24 <rtl_crypto_hmac_sha2_init+0x2a4>)
30009ca4:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009ca8:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009cac:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30009cb0:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009cb4:	6263      	str	r3, [r4, #36]	; 0x24
30009cb6:	4b51      	ldr	r3, [pc, #324]	; (30009dfc <rtl_crypto_hmac_sha2_init+0x27c>)
30009cb8:	6223      	str	r3, [r4, #32]
30009cba:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009cbe:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30009cc2:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009cc6:	62e3      	str	r3, [r4, #44]	; 0x2c
30009cc8:	4b4b      	ldr	r3, [pc, #300]	; (30009df8 <rtl_crypto_hmac_sha2_init+0x278>)
30009cca:	62a3      	str	r3, [r4, #40]	; 0x28
30009ccc:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30009cd0:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009cd4:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009cd8:	6363      	str	r3, [r4, #52]	; 0x34
30009cda:	4b49      	ldr	r3, [pc, #292]	; (30009e00 <rtl_crypto_hmac_sha2_init+0x280>)
30009cdc:	6323      	str	r3, [r4, #48]	; 0x30
30009cde:	4b49      	ldr	r3, [pc, #292]	; (30009e04 <rtl_crypto_hmac_sha2_init+0x284>)
30009ce0:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009ce4:	4b50      	ldr	r3, [pc, #320]	; (30009e28 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009ce6:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009cea:	e7a1      	b.n	30009c30 <rtl_crypto_hmac_sha2_init+0xb0>
30009cec:	f04f 35ff 	mov.w	r5, #4294967295
30009cf0:	4628      	mov	r0, r5
30009cf2:	b005      	add	sp, #20
30009cf4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009cf8:	2285      	movs	r2, #133	; 0x85
30009cfa:	2300      	movs	r3, #0
30009cfc:	f04f 31ff 	mov.w	r1, #4294967295
30009d00:	4c41      	ldr	r4, [pc, #260]	; (30009e08 <rtl_crypto_hmac_sha2_init+0x288>)
30009d02:	9702      	str	r7, [sp, #8]
30009d04:	4841      	ldr	r0, [pc, #260]	; (30009e0c <rtl_crypto_hmac_sha2_init+0x28c>)
30009d06:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009d0a:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009d0e:	e9cd 3600 	strd	r3, r6, [sp]
30009d12:	47a0      	blx	r4
30009d14:	4605      	mov	r5, r0
30009d16:	4628      	mov	r0, r5
30009d18:	b005      	add	sp, #20
30009d1a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009d1e:	2225      	movs	r2, #37	; 0x25
30009d20:	e7eb      	b.n	30009cfa <rtl_crypto_hmac_sha2_init+0x17a>
30009d22:	2c00      	cmp	r4, #0
30009d24:	d053      	beq.n	30009dce <rtl_crypto_hmac_sha2_init+0x24e>
30009d26:	4b41      	ldr	r3, [pc, #260]	; (30009e2c <rtl_crypto_hmac_sha2_init+0x2ac>)
30009d28:	f8df e130 	ldr.w	lr, [pc, #304]	; 30009e5c <rtl_crypto_hmac_sha2_init+0x2dc>
30009d2c:	4a40      	ldr	r2, [pc, #256]	; (30009e30 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009d2e:	4941      	ldr	r1, [pc, #260]	; (30009e34 <rtl_crypto_hmac_sha2_init+0x2b4>)
30009d30:	4d41      	ldr	r5, [pc, #260]	; (30009e38 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009d32:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30009e60 <rtl_crypto_hmac_sha2_init+0x2e0>
30009d36:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009d3a:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30009d3e:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30009d42:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30009d46:	6163      	str	r3, [r4, #20]
30009d48:	4b3c      	ldr	r3, [pc, #240]	; (30009e3c <rtl_crypto_hmac_sha2_init+0x2bc>)
30009d4a:	6123      	str	r3, [r4, #16]
30009d4c:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30009d50:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30009d54:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30009d58:	61e3      	str	r3, [r4, #28]
30009d5a:	4b39      	ldr	r3, [pc, #228]	; (30009e40 <rtl_crypto_hmac_sha2_init+0x2c0>)
30009d5c:	61a3      	str	r3, [r4, #24]
30009d5e:	4b39      	ldr	r3, [pc, #228]	; (30009e44 <rtl_crypto_hmac_sha2_init+0x2c4>)
30009d60:	e9c4 3208 	strd	r3, r2, [r4, #32]
30009d64:	4b38      	ldr	r3, [pc, #224]	; (30009e48 <rtl_crypto_hmac_sha2_init+0x2c8>)
30009d66:	2245      	movs	r2, #69	; 0x45
30009d68:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30009d6c:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30009d70:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30009d74:	33aa      	adds	r3, #170	; 0xaa
30009d76:	6363      	str	r3, [r4, #52]	; 0x34
30009d78:	4b34      	ldr	r3, [pc, #208]	; (30009e4c <rtl_crypto_hmac_sha2_init+0x2cc>)
30009d7a:	6323      	str	r3, [r4, #48]	; 0x30
30009d7c:	4b34      	ldr	r3, [pc, #208]	; (30009e50 <rtl_crypto_hmac_sha2_init+0x2d0>)
30009d7e:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009d82:	4b34      	ldr	r3, [pc, #208]	; (30009e54 <rtl_crypto_hmac_sha2_init+0x2d4>)
30009d84:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009d88:	e752      	b.n	30009c30 <rtl_crypto_hmac_sha2_init+0xb0>
30009d8a:	b314      	cbz	r4, 30009dd2 <rtl_crypto_hmac_sha2_init+0x252>
30009d8c:	4b32      	ldr	r3, [pc, #200]	; (30009e58 <rtl_crypto_hmac_sha2_init+0x2d8>)
30009d8e:	2215      	movs	r2, #21
30009d90:	4932      	ldr	r1, [pc, #200]	; (30009e5c <rtl_crypto_hmac_sha2_init+0x2dc>)
30009d92:	4827      	ldr	r0, [pc, #156]	; (30009e30 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009d94:	4d28      	ldr	r5, [pc, #160]	; (30009e38 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009d96:	e9c4 1302 	strd	r1, r3, [r4, #8]
30009d9a:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30009d9e:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30009da2:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30009da6:	6123      	str	r3, [r4, #16]
30009da8:	4b22      	ldr	r3, [pc, #136]	; (30009e34 <rtl_crypto_hmac_sha2_init+0x2b4>)
30009daa:	e9c4 0305 	strd	r0, r3, [r4, #20]
30009dae:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30009db2:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30009db6:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009dba:	61e3      	str	r3, [r4, #28]
30009dbc:	4b28      	ldr	r3, [pc, #160]	; (30009e60 <rtl_crypto_hmac_sha2_init+0x2e0>)
30009dbe:	e9c4 5308 	strd	r5, r3, [r4, #32]
30009dc2:	e735      	b.n	30009c30 <rtl_crypto_hmac_sha2_init+0xb0>
30009dc4:	21b1      	movs	r1, #177	; 0xb1
30009dc6:	480a      	ldr	r0, [pc, #40]	; (30009df0 <rtl_crypto_hmac_sha2_init+0x270>)
30009dc8:	f002 fc9a 	bl	3000c700 <__io_assert_failed_veneer>
30009dcc:	e701      	b.n	30009bd2 <rtl_crypto_hmac_sha2_init+0x52>
30009dce:	2245      	movs	r2, #69	; 0x45
30009dd0:	e793      	b.n	30009cfa <rtl_crypto_hmac_sha2_init+0x17a>
30009dd2:	2215      	movs	r2, #21
30009dd4:	e791      	b.n	30009cfa <rtl_crypto_hmac_sha2_init+0x17a>
30009dd6:	f06f 0505 	mvn.w	r5, #5
30009dda:	e789      	b.n	30009cf0 <rtl_crypto_hmac_sha2_init+0x170>
30009ddc:	f06f 0503 	mvn.w	r5, #3
30009de0:	e786      	b.n	30009cf0 <rtl_crypto_hmac_sha2_init+0x170>
30009de2:	bf00      	nop
30009de4:	0000e5f9 	.word	0x0000e5f9
30009de8:	500c8000 	.word	0x500c8000
30009dec:	400c8000 	.word	0x400c8000
30009df0:	3000dd2c 	.word	0x3000dd2c
30009df4:	bb67ae85 	.word	0xbb67ae85
30009df8:	510e527f 	.word	0x510e527f
30009dfc:	a54ff53a 	.word	0xa54ff53a
30009e00:	9b05688c 	.word	0x9b05688c
30009e04:	1f83d9ab 	.word	0x1f83d9ab
30009e08:	300099d5 	.word	0x300099d5
30009e0c:	2001c460 	.word	0x2001c460
30009e10:	00004c99 	.word	0x00004c99
30009e14:	6a09e667 	.word	0x6a09e667
30009e18:	f3bcc908 	.word	0xf3bcc908
30009e1c:	fe94f82b 	.word	0xfe94f82b
30009e20:	fb41bd6b 	.word	0xfb41bd6b
30009e24:	3c6ef372 	.word	0x3c6ef372
30009e28:	5be0cd19 	.word	0x5be0cd19
30009e2c:	cbbb9d5d 	.word	0xcbbb9d5d
30009e30:	f70e5939 	.word	0xf70e5939
30009e34:	ffc00b31 	.word	0xffc00b31
30009e38:	64f98fa7 	.word	0x64f98fa7
30009e3c:	629a292a 	.word	0x629a292a
30009e40:	9159015a 	.word	0x9159015a
30009e44:	152fecd8 	.word	0x152fecd8
30009e48:	67332667 	.word	0x67332667
30009e4c:	8eb44a87 	.word	0x8eb44a87
30009e50:	db0c2e0d 	.word	0xdb0c2e0d
30009e54:	47b5481d 	.word	0x47b5481d
30009e58:	367cd507 	.word	0x367cd507
30009e5c:	c1059ed8 	.word	0xc1059ed8
30009e60:	befa4fa4 	.word	0xbefa4fa4
30009e64:	137e2179 	.word	0x137e2179

30009e68 <CPU_ClkGet>:
30009e68:	b570      	push	{r4, r5, r6, lr}
30009e6a:	4d1e      	ldr	r5, [pc, #120]	; (30009ee4 <CPU_ClkGet+0x7c>)
30009e6c:	4b1e      	ldr	r3, [pc, #120]	; (30009ee8 <CPU_ClkGet+0x80>)
30009e6e:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30009e72:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009e76:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009e7a:	4798      	blx	r3
30009e7c:	b92c      	cbnz	r4, 30009e8a <CPU_ClkGet+0x22>
30009e7e:	2801      	cmp	r0, #1
30009e80:	d007      	beq.n	30009e92 <CPU_ClkGet+0x2a>
30009e82:	07b2      	lsls	r2, r6, #30
30009e84:	d407      	bmi.n	30009e96 <CPU_ClkGet+0x2e>
30009e86:	4819      	ldr	r0, [pc, #100]	; (30009eec <CPU_ClkGet+0x84>)
30009e88:	bd70      	pop	{r4, r5, r6, pc}
30009e8a:	2c01      	cmp	r4, #1
30009e8c:	d007      	beq.n	30009e9e <CPU_ClkGet+0x36>
30009e8e:	2000      	movs	r0, #0
30009e90:	bd70      	pop	{r4, r5, r6, pc}
30009e92:	4817      	ldr	r0, [pc, #92]	; (30009ef0 <CPU_ClkGet+0x88>)
30009e94:	bd70      	pop	{r4, r5, r6, pc}
30009e96:	4b17      	ldr	r3, [pc, #92]	; (30009ef4 <CPU_ClkGet+0x8c>)
30009e98:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009e9c:	4718      	bx	r3
30009e9e:	2801      	cmp	r0, #1
30009ea0:	d011      	beq.n	30009ec6 <CPU_ClkGet+0x5e>
30009ea2:	05f3      	lsls	r3, r6, #23
30009ea4:	d51c      	bpl.n	30009ee0 <CPU_ClkGet+0x78>
30009ea6:	4914      	ldr	r1, [pc, #80]	; (30009ef8 <CPU_ClkGet+0x90>)
30009ea8:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30009eac:	6c48      	ldr	r0, [r1, #68]	; 0x44
30009eae:	4a13      	ldr	r2, [pc, #76]	; (30009efc <CPU_ClkGet+0x94>)
30009eb0:	f003 0307 	and.w	r3, r3, #7
30009eb4:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009eb8:	3301      	adds	r3, #1
30009eba:	3002      	adds	r0, #2
30009ebc:	fb02 f000 	mul.w	r0, r2, r0
30009ec0:	fbb0 f0f3 	udiv	r0, r0, r3
30009ec4:	bd70      	pop	{r4, r5, r6, pc}
30009ec6:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009eca:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30009ece:	f3c3 4007 	ubfx	r0, r3, #16, #8
30009ed2:	d003      	beq.n	30009edc <CPU_ClkGet+0x74>
30009ed4:	4b0a      	ldr	r3, [pc, #40]	; (30009f00 <CPU_ClkGet+0x98>)
30009ed6:	fb03 f000 	mul.w	r0, r3, r0
30009eda:	bd70      	pop	{r4, r5, r6, pc}
30009edc:	4809      	ldr	r0, [pc, #36]	; (30009f04 <CPU_ClkGet+0x9c>)
30009ede:	bd70      	pop	{r4, r5, r6, pc}
30009ee0:	4806      	ldr	r0, [pc, #24]	; (30009efc <CPU_ClkGet+0x94>)
30009ee2:	bd70      	pop	{r4, r5, r6, pc}
30009ee4:	42008000 	.word	0x42008000
30009ee8:	0000c0f9 	.word	0x0000c0f9
30009eec:	003d0900 	.word	0x003d0900
30009ef0:	01312d00 	.word	0x01312d00
30009ef4:	000099f5 	.word	0x000099f5
30009ef8:	42008800 	.word	0x42008800
30009efc:	02625a00 	.word	0x02625a00
30009f00:	000f4240 	.word	0x000f4240
30009f04:	02faf080 	.word	0x02faf080

30009f08 <RSIP_IV_Set>:
30009f08:	4b05      	ldr	r3, [pc, #20]	; (30009f20 <RSIP_IV_Set+0x18>)
30009f0a:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30009f0e:	680b      	ldr	r3, [r1, #0]
30009f10:	ba1b      	rev	r3, r3
30009f12:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009f16:	684b      	ldr	r3, [r1, #4]
30009f18:	ba1b      	rev	r3, r3
30009f1a:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009f1e:	4770      	bx	lr
30009f20:	42008c00 	.word	0x42008c00

30009f24 <RSIP_OTF_Cmd>:
30009f24:	4a04      	ldr	r2, [pc, #16]	; (30009f38 <RSIP_OTF_Cmd+0x14>)
30009f26:	2801      	cmp	r0, #1
30009f28:	6953      	ldr	r3, [r2, #20]
30009f2a:	bf0c      	ite	eq
30009f2c:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009f30:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009f34:	6153      	str	r3, [r2, #20]
30009f36:	4770      	bx	lr
30009f38:	42008c00 	.word	0x42008c00

30009f3c <RSIP_OTF_Enable>:
30009f3c:	b570      	push	{r4, r5, r6, lr}
30009f3e:	4604      	mov	r4, r0
30009f40:	b1f3      	cbz	r3, 30009f80 <RSIP_OTF_Enable+0x44>
30009f42:	9b04      	ldr	r3, [sp, #16]
30009f44:	460d      	mov	r5, r1
30009f46:	4616      	mov	r6, r2
30009f48:	2b07      	cmp	r3, #7
30009f4a:	d827      	bhi.n	30009f9c <RSIP_OTF_Enable+0x60>
30009f4c:	9b05      	ldr	r3, [sp, #20]
30009f4e:	3b01      	subs	r3, #1
30009f50:	2b01      	cmp	r3, #1
30009f52:	d81d      	bhi.n	30009f90 <RSIP_OTF_Enable+0x54>
30009f54:	9a04      	ldr	r2, [sp, #16]
30009f56:	9b05      	ldr	r3, [sp, #20]
30009f58:	f002 0007 	and.w	r0, r2, #7
30009f5c:	005b      	lsls	r3, r3, #1
30009f5e:	4305      	orrs	r5, r0
30009f60:	4813      	ldr	r0, [pc, #76]	; (30009fb0 <RSIP_OTF_Enable+0x74>)
30009f62:	f003 0306 	and.w	r3, r3, #6
30009f66:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009f6a:	ea43 0206 	orr.w	r2, r3, r6
30009f6e:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009f70:	f042 0201 	orr.w	r2, r2, #1
30009f74:	f023 0301 	bic.w	r3, r3, #1
30009f78:	6463      	str	r3, [r4, #68]	; 0x44
30009f7a:	6425      	str	r5, [r4, #64]	; 0x40
30009f7c:	6462      	str	r2, [r4, #68]	; 0x44
30009f7e:	bd70      	pop	{r4, r5, r6, pc}
30009f80:	480b      	ldr	r0, [pc, #44]	; (30009fb0 <RSIP_OTF_Enable+0x74>)
30009f82:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009f86:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009f88:	f023 0301 	bic.w	r3, r3, #1
30009f8c:	6463      	str	r3, [r4, #68]	; 0x44
30009f8e:	bd70      	pop	{r4, r5, r6, pc}
30009f90:	f240 2131 	movw	r1, #561	; 0x231
30009f94:	4807      	ldr	r0, [pc, #28]	; (30009fb4 <RSIP_OTF_Enable+0x78>)
30009f96:	f002 fbb3 	bl	3000c700 <__io_assert_failed_veneer>
30009f9a:	e7db      	b.n	30009f54 <RSIP_OTF_Enable+0x18>
30009f9c:	f44f 710c 	mov.w	r1, #560	; 0x230
30009fa0:	4804      	ldr	r0, [pc, #16]	; (30009fb4 <RSIP_OTF_Enable+0x78>)
30009fa2:	f002 fbad 	bl	3000c700 <__io_assert_failed_veneer>
30009fa6:	9b05      	ldr	r3, [sp, #20]
30009fa8:	3b01      	subs	r3, #1
30009faa:	2b01      	cmp	r3, #1
30009fac:	d9d2      	bls.n	30009f54 <RSIP_OTF_Enable+0x18>
30009fae:	e7ef      	b.n	30009f90 <RSIP_OTF_Enable+0x54>
30009fb0:	42008c00 	.word	0x42008c00
30009fb4:	3000dd1c 	.word	0x3000dd1c

30009fb8 <RSIP_MMU_Config>:
30009fb8:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009fbc:	311f      	adds	r1, #31
30009fbe:	321f      	adds	r2, #31
30009fc0:	331f      	adds	r3, #31
30009fc2:	0080      	lsls	r0, r0, #2
30009fc4:	0949      	lsrs	r1, r1, #5
30009fc6:	0952      	lsrs	r2, r2, #5
30009fc8:	095b      	lsrs	r3, r3, #5
30009fca:	0249      	lsls	r1, r1, #9
30009fcc:	0252      	lsls	r2, r2, #9
30009fce:	025b      	lsls	r3, r3, #9
30009fd0:	b410      	push	{r4}
30009fd2:	4c08      	ldr	r4, [pc, #32]	; (30009ff4 <RSIP_MMU_Config+0x3c>)
30009fd4:	4404      	add	r4, r0
30009fd6:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009fda:	f020 0001 	bic.w	r0, r0, #1
30009fde:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30009fe2:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30009fe6:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009fea:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009fee:	f85d 4b04 	ldr.w	r4, [sp], #4
30009ff2:	4770      	bx	lr
30009ff4:	42008c00 	.word	0x42008c00

30009ff8 <RSIP_MMU_Cmd>:
30009ff8:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009ffc:	4b07      	ldr	r3, [pc, #28]	; (3000a01c <RSIP_MMU_Cmd+0x24>)
30009ffe:	0080      	lsls	r0, r0, #2
3000a000:	4403      	add	r3, r0
3000a002:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000a006:	b121      	cbz	r1, 3000a012 <RSIP_MMU_Cmd+0x1a>
3000a008:	f042 0201 	orr.w	r2, r2, #1
3000a00c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a010:	4770      	bx	lr
3000a012:	f022 0201 	bic.w	r2, r2, #1
3000a016:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a01a:	4770      	bx	lr
3000a01c:	42008c00 	.word	0x42008c00

3000a020 <RSIP_MMU_Cache_Clean>:
3000a020:	4a02      	ldr	r2, [pc, #8]	; (3000a02c <RSIP_MMU_Cache_Clean+0xc>)
3000a022:	6993      	ldr	r3, [r2, #24]
3000a024:	f043 0304 	orr.w	r3, r3, #4
3000a028:	6193      	str	r3, [r2, #24]
3000a02a:	4770      	bx	lr
3000a02c:	42008c00 	.word	0x42008c00

3000a030 <NAND_RxData>:
3000a030:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
3000a034:	460c      	mov	r4, r1
3000a036:	4616      	mov	r6, r2
3000a038:	461f      	mov	r7, r3
3000a03a:	2800      	cmp	r0, #0
3000a03c:	f000 808a 	beq.w	3000a154 <NAND_RxData+0x124>
3000a040:	4605      	mov	r5, r0
3000a042:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000a160 <NAND_RxData+0x130>
3000a046:	2000      	movs	r0, #0
3000a048:	f8df 8118 	ldr.w	r8, [pc, #280]	; 3000a164 <NAND_RxData+0x134>
3000a04c:	47c0      	blx	r8
3000a04e:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
3000a052:	2000      	movs	r0, #0
3000a054:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
3000a058:	6819      	ldr	r1, [r3, #0]
3000a05a:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
3000a05e:	6019      	str	r1, [r3, #0]
3000a060:	6819      	ldr	r1, [r3, #0]
3000a062:	f441 7140 	orr.w	r1, r1, #768	; 0x300
3000a066:	6019      	str	r1, [r3, #0]
3000a068:	605a      	str	r2, [r3, #4]
3000a06a:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
3000a06e:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
3000a072:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
3000a076:	ea4f 420c 	mov.w	r2, ip, lsl #16
3000a07a:	f1bc 0f03 	cmp.w	ip, #3
3000a07e:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
3000a082:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a086:	ea42 0201 	orr.w	r2, r2, r1
3000a08a:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
3000a08e:	f04f 0201 	mov.w	r2, #1
3000a092:	611a      	str	r2, [r3, #16]
3000a094:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000a098:	bf08      	it	eq
3000a09a:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
3000a09e:	f3c4 2307 	ubfx	r3, r4, #8, #8
3000a0a2:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000a0a6:	b2e4      	uxtb	r4, r4
3000a0a8:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000a0ac:	2301      	movs	r3, #1
3000a0ae:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
3000a0b2:	f017 0403 	ands.w	r4, r7, #3
3000a0b6:	6083      	str	r3, [r0, #8]
3000a0b8:	d137      	bne.n	3000a12a <NAND_RxData+0xfa>
3000a0ba:	463b      	mov	r3, r7
3000a0bc:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
3000a0c0:	42b4      	cmp	r4, r6
3000a0c2:	d216      	bcs.n	3000a0f2 <NAND_RxData+0xc2>
3000a0c4:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000a0c8:	f012 0f3c 	tst.w	r2, #60	; 0x3c
3000a0cc:	f3c2 0583 	ubfx	r5, r2, #2, #4
3000a0d0:	d00c      	beq.n	3000a0ec <NAND_RxData+0xbc>
3000a0d2:	00ad      	lsls	r5, r5, #2
3000a0d4:	1f19      	subs	r1, r3, #4
3000a0d6:	f1a5 0c04 	sub.w	ip, r5, #4
3000a0da:	449c      	add	ip, r3
3000a0dc:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
3000a0e0:	f841 0f04 	str.w	r0, [r1, #4]!
3000a0e4:	458c      	cmp	ip, r1
3000a0e6:	d1f9      	bne.n	3000a0dc <NAND_RxData+0xac>
3000a0e8:	442b      	add	r3, r5
3000a0ea:	442c      	add	r4, r5
3000a0ec:	1b32      	subs	r2, r6, r4
3000a0ee:	2a03      	cmp	r2, #3
3000a0f0:	d8e6      	bhi.n	3000a0c0 <NAND_RxData+0x90>
3000a0f2:	1e63      	subs	r3, r4, #1
3000a0f4:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a0f8:	441f      	add	r7, r3
3000a0fa:	e002      	b.n	3000a102 <NAND_RxData+0xd2>
3000a0fc:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a0fe:	071b      	lsls	r3, r3, #28
3000a100:	d40d      	bmi.n	3000a11e <NAND_RxData+0xee>
3000a102:	42b4      	cmp	r4, r6
3000a104:	d3fa      	bcc.n	3000a0fc <NAND_RxData+0xcc>
3000a106:	2004      	movs	r0, #4
3000a108:	47c0      	blx	r8
3000a10a:	2000      	movs	r0, #0
3000a10c:	47c0      	blx	r8
3000a10e:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a112:	6813      	ldr	r3, [r2, #0]
3000a114:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000a118:	6013      	str	r3, [r2, #0]
3000a11a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
3000a11e:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
3000a122:	3401      	adds	r4, #1
3000a124:	f807 3f01 	strb.w	r3, [r7, #1]!
3000a128:	e7eb      	b.n	3000a102 <NAND_RxData+0xd2>
3000a12a:	f1c4 0404 	rsb	r4, r4, #4
3000a12e:	2200      	movs	r2, #0
3000a130:	4639      	mov	r1, r7
3000a132:	e00a      	b.n	3000a14a <NAND_RxData+0x11a>
3000a134:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000a136:	072d      	lsls	r5, r5, #28
3000a138:	d505      	bpl.n	3000a146 <NAND_RxData+0x116>
3000a13a:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
3000a13e:	3201      	adds	r2, #1
3000a140:	700b      	strb	r3, [r1, #0]
3000a142:	18b9      	adds	r1, r7, r2
3000a144:	460b      	mov	r3, r1
3000a146:	4294      	cmp	r4, r2
3000a148:	d0b8      	beq.n	3000a0bc <NAND_RxData+0x8c>
3000a14a:	42b2      	cmp	r2, r6
3000a14c:	460b      	mov	r3, r1
3000a14e:	d3f1      	bcc.n	3000a134 <NAND_RxData+0x104>
3000a150:	4614      	mov	r4, r2
3000a152:	e7ce      	b.n	3000a0f2 <NAND_RxData+0xc2>
3000a154:	f8df 9008 	ldr.w	r9, [pc, #8]	; 3000a160 <NAND_RxData+0x130>
3000a158:	f899 5008 	ldrb.w	r5, [r9, #8]
3000a15c:	e773      	b.n	3000a046 <NAND_RxData+0x16>
3000a15e:	bf00      	nop
3000a160:	2001c01c 	.word	0x2001c01c
3000a164:	0000b9c1 	.word	0x0000b9c1

3000a168 <NAND_Page_Read>:
3000a168:	b530      	push	{r4, r5, lr}
3000a16a:	b085      	sub	sp, #20
3000a16c:	4c07      	ldr	r4, [pc, #28]	; (3000a18c <NAND_Page_Read+0x24>)
3000a16e:	4d08      	ldr	r5, [pc, #32]	; (3000a190 <NAND_Page_Read+0x28>)
3000a170:	9301      	str	r3, [sp, #4]
3000a172:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000a176:	47a0      	blx	r4
3000a178:	4604      	mov	r4, r0
3000a17a:	9903      	ldr	r1, [sp, #12]
3000a17c:	7a28      	ldrb	r0, [r5, #8]
3000a17e:	4d05      	ldr	r5, [pc, #20]	; (3000a194 <NAND_Page_Read+0x2c>)
3000a180:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000a184:	47a8      	blx	r5
3000a186:	4620      	mov	r0, r4
3000a188:	b005      	add	sp, #20
3000a18a:	bd30      	pop	{r4, r5, pc}
3000a18c:	0000bd85 	.word	0x0000bd85
3000a190:	2001c01c 	.word	0x2001c01c
3000a194:	3000a031 	.word	0x3000a031

3000a198 <irq_register>:
3000a198:	b530      	push	{r4, r5, lr}
3000a19a:	461c      	mov	r4, r3
3000a19c:	460d      	mov	r5, r1
3000a19e:	f040 0001 	orr.w	r0, r0, #1
3000a1a2:	4916      	ldr	r1, [pc, #88]	; (3000a1fc <irq_register+0x64>)
3000a1a4:	4b16      	ldr	r3, [pc, #88]	; (3000a200 <irq_register+0x68>)
3000a1a6:	2c07      	cmp	r4, #7
3000a1a8:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
3000a1ac:	b085      	sub	sp, #20
3000a1ae:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
3000a1b2:	d813      	bhi.n	3000a1dc <irq_register+0x44>
3000a1b4:	2c07      	cmp	r4, #7
3000a1b6:	bf28      	it	cs
3000a1b8:	2407      	movcs	r4, #7
3000a1ba:	2d00      	cmp	r5, #0
3000a1bc:	ea4f 1444 	mov.w	r4, r4, lsl #5
3000a1c0:	b2e4      	uxtb	r4, r4
3000a1c2:	db04      	blt.n	3000a1ce <irq_register+0x36>
3000a1c4:	4b0f      	ldr	r3, [pc, #60]	; (3000a204 <irq_register+0x6c>)
3000a1c6:	2001      	movs	r0, #1
3000a1c8:	555c      	strb	r4, [r3, r5]
3000a1ca:	b005      	add	sp, #20
3000a1cc:	bd30      	pop	{r4, r5, pc}
3000a1ce:	f005 050f 	and.w	r5, r5, #15
3000a1d2:	4b0d      	ldr	r3, [pc, #52]	; (3000a208 <irq_register+0x70>)
3000a1d4:	2001      	movs	r0, #1
3000a1d6:	555c      	strb	r4, [r3, r5]
3000a1d8:	b005      	add	sp, #20
3000a1da:	bd30      	pop	{r4, r5, pc}
3000a1dc:	4b0b      	ldr	r3, [pc, #44]	; (3000a20c <irq_register+0x74>)
3000a1de:	2107      	movs	r1, #7
3000a1e0:	4a0b      	ldr	r2, [pc, #44]	; (3000a210 <irq_register+0x78>)
3000a1e2:	2003      	movs	r0, #3
3000a1e4:	9500      	str	r5, [sp, #0]
3000a1e6:	e9cd 4101 	strd	r4, r1, [sp, #4]
3000a1ea:	4619      	mov	r1, r3
3000a1ec:	f002 f80c 	bl	3000c208 <rtk_log_write>
3000a1f0:	f240 315b 	movw	r1, #859	; 0x35b
3000a1f4:	4807      	ldr	r0, [pc, #28]	; (3000a214 <irq_register+0x7c>)
3000a1f6:	f002 fa83 	bl	3000c700 <__io_assert_failed_veneer>
3000a1fa:	e7db      	b.n	3000a1b4 <irq_register+0x1c>
3000a1fc:	30000184 	.word	0x30000184
3000a200:	300002c4 	.word	0x300002c4
3000a204:	e000e400 	.word	0xe000e400
3000a208:	e000ed14 	.word	0xe000ed14
3000a20c:	3000dcac 	.word	0x3000dcac
3000a210:	3000dce0 	.word	0x3000dce0
3000a214:	3000dd08 	.word	0x3000dd08

3000a218 <SYSCFG_BootFromNor>:
3000a218:	4b0e      	ldr	r3, [pc, #56]	; (3000a254 <SYSCFG_BootFromNor+0x3c>)
3000a21a:	b510      	push	{r4, lr}
3000a21c:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
3000a220:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
3000a224:	0392      	lsls	r2, r2, #14
3000a226:	d50a      	bpl.n	3000a23e <SYSCFG_BootFromNor+0x26>
3000a228:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
3000a22c:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000a230:	d105      	bne.n	3000a23e <SYSCFG_BootFromNor+0x26>
3000a232:	4b09      	ldr	r3, [pc, #36]	; (3000a258 <SYSCFG_BootFromNor+0x40>)
3000a234:	4798      	blx	r3
3000a236:	3800      	subs	r0, #0
3000a238:	bf18      	it	ne
3000a23a:	2001      	movne	r0, #1
3000a23c:	bd10      	pop	{r4, pc}
3000a23e:	4c07      	ldr	r4, [pc, #28]	; (3000a25c <SYSCFG_BootFromNor+0x44>)
3000a240:	47a0      	blx	r4
3000a242:	2802      	cmp	r0, #2
3000a244:	d004      	beq.n	3000a250 <SYSCFG_BootFromNor+0x38>
3000a246:	47a0      	blx	r4
3000a248:	2801      	cmp	r0, #1
3000a24a:	d1f2      	bne.n	3000a232 <SYSCFG_BootFromNor+0x1a>
3000a24c:	2000      	movs	r0, #0
3000a24e:	bd10      	pop	{r4, pc}
3000a250:	2001      	movs	r0, #1
3000a252:	bd10      	pop	{r4, pc}
3000a254:	42008000 	.word	0x42008000
3000a258:	0000c0bd 	.word	0x0000c0bd
3000a25c:	0000c1a9 	.word	0x0000c1a9

3000a260 <SYSCFG_RLVersion>:
3000a260:	4b07      	ldr	r3, [pc, #28]	; (3000a280 <SYSCFG_RLVersion+0x20>)
3000a262:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a266:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a26a:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000a26e:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000a272:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a276:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a27a:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a27e:	4770      	bx	lr
3000a280:	42008000 	.word	0x42008000

3000a284 <ddr_init_index>:
3000a284:	b570      	push	{r4, r5, r6, lr}
3000a286:	4b0f      	ldr	r3, [pc, #60]	; (3000a2c4 <ddr_init_index+0x40>)
3000a288:	4798      	blx	r3
3000a28a:	4604      	mov	r4, r0
3000a28c:	4b0e      	ldr	r3, [pc, #56]	; (3000a2c8 <ddr_init_index+0x44>)
3000a28e:	4798      	blx	r3
3000a290:	2c03      	cmp	r4, #3
3000a292:	d014      	beq.n	3000a2be <ddr_init_index+0x3a>
3000a294:	2c01      	cmp	r4, #1
3000a296:	d00c      	beq.n	3000a2b2 <ddr_init_index+0x2e>
3000a298:	4e0c      	ldr	r6, [pc, #48]	; (3000a2cc <ddr_init_index+0x48>)
3000a29a:	4d0d      	ldr	r5, [pc, #52]	; (3000a2d0 <ddr_init_index+0x4c>)
3000a29c:	4c0d      	ldr	r4, [pc, #52]	; (3000a2d4 <ddr_init_index+0x50>)
3000a29e:	462b      	mov	r3, r5
3000a2a0:	4622      	mov	r2, r4
3000a2a2:	490b      	ldr	r1, [pc, #44]	; (3000a2d0 <ddr_init_index+0x4c>)
3000a2a4:	2002      	movs	r0, #2
3000a2a6:	f001 ffaf 	bl	3000c208 <rtk_log_write>
3000a2aa:	f242 7010 	movw	r0, #10000	; 0x2710
3000a2ae:	47b0      	blx	r6
3000a2b0:	e7f5      	b.n	3000a29e <ddr_init_index+0x1a>
3000a2b2:	2802      	cmp	r0, #2
3000a2b4:	d003      	beq.n	3000a2be <ddr_init_index+0x3a>
3000a2b6:	2803      	cmp	r0, #3
3000a2b8:	d1ee      	bne.n	3000a298 <ddr_init_index+0x14>
3000a2ba:	2000      	movs	r0, #0
3000a2bc:	bd70      	pop	{r4, r5, r6, pc}
3000a2be:	4620      	mov	r0, r4
3000a2c0:	bd70      	pop	{r4, r5, r6, pc}
3000a2c2:	bf00      	nop
3000a2c4:	300081f1 	.word	0x300081f1
3000a2c8:	3000806d 	.word	0x3000806d
3000a2cc:	00009be5 	.word	0x00009be5
3000a2d0:	3000dd64 	.word	0x3000dd64
3000a2d4:	3000dd68 	.word	0x3000dd68

3000a2d8 <rxi316_perf_tune>:
3000a2d8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a2dc:	4606      	mov	r6, r0
3000a2de:	6800      	ldr	r0, [r0, #0]
3000a2e0:	b0c1      	sub	sp, #260	; 0x104
3000a2e2:	7985      	ldrb	r5, [r0, #6]
3000a2e4:	2d00      	cmp	r5, #0
3000a2e6:	f000 832c 	beq.w	3000a942 <rxi316_perf_tune+0x66a>
3000a2ea:	2d01      	cmp	r5, #1
3000a2ec:	d003      	beq.n	3000a2f6 <rxi316_perf_tune+0x1e>
3000a2ee:	2d03      	cmp	r5, #3
3000a2f0:	bf14      	ite	ne
3000a2f2:	2502      	movne	r5, #2
3000a2f4:	2500      	moveq	r5, #0
3000a2f6:	68f3      	ldr	r3, [r6, #12]
3000a2f8:	2420      	movs	r4, #32
3000a2fa:	4684      	mov	ip, r0
3000a2fc:	6859      	ldr	r1, [r3, #4]
3000a2fe:	46a3      	mov	fp, r4
3000a300:	46a2      	mov	sl, r4
3000a302:	46a0      	mov	r8, r4
3000a304:	784b      	ldrb	r3, [r1, #1]
3000a306:	46a1      	mov	r9, r4
3000a308:	7bcf      	ldrb	r7, [r1, #15]
3000a30a:	9321      	str	r3, [sp, #132]	; 0x84
3000a30c:	788b      	ldrb	r3, [r1, #2]
3000a30e:	780a      	ldrb	r2, [r1, #0]
3000a310:	9322      	str	r3, [sp, #136]	; 0x88
3000a312:	78cb      	ldrb	r3, [r1, #3]
3000a314:	9323      	str	r3, [sp, #140]	; 0x8c
3000a316:	790b      	ldrb	r3, [r1, #4]
3000a318:	9324      	str	r3, [sp, #144]	; 0x90
3000a31a:	794b      	ldrb	r3, [r1, #5]
3000a31c:	9325      	str	r3, [sp, #148]	; 0x94
3000a31e:	798b      	ldrb	r3, [r1, #6]
3000a320:	9326      	str	r3, [sp, #152]	; 0x98
3000a322:	79cb      	ldrb	r3, [r1, #7]
3000a324:	9327      	str	r3, [sp, #156]	; 0x9c
3000a326:	7a0b      	ldrb	r3, [r1, #8]
3000a328:	9328      	str	r3, [sp, #160]	; 0xa0
3000a32a:	7a4b      	ldrb	r3, [r1, #9]
3000a32c:	9329      	str	r3, [sp, #164]	; 0xa4
3000a32e:	7a8b      	ldrb	r3, [r1, #10]
3000a330:	932a      	str	r3, [sp, #168]	; 0xa8
3000a332:	7acb      	ldrb	r3, [r1, #11]
3000a334:	932b      	str	r3, [sp, #172]	; 0xac
3000a336:	7b0b      	ldrb	r3, [r1, #12]
3000a338:	932c      	str	r3, [sp, #176]	; 0xb0
3000a33a:	7b4b      	ldrb	r3, [r1, #13]
3000a33c:	932d      	str	r3, [sp, #180]	; 0xb4
3000a33e:	7b8b      	ldrb	r3, [r1, #14]
3000a340:	932e      	str	r3, [sp, #184]	; 0xb8
3000a342:	2300      	movs	r3, #0
3000a344:	972f      	str	r7, [sp, #188]	; 0xbc
3000a346:	7c0f      	ldrb	r7, [r1, #16]
3000a348:	941c      	str	r4, [sp, #112]	; 0x70
3000a34a:	9730      	str	r7, [sp, #192]	; 0xc0
3000a34c:	7c4f      	ldrb	r7, [r1, #17]
3000a34e:	9731      	str	r7, [sp, #196]	; 0xc4
3000a350:	7c8f      	ldrb	r7, [r1, #18]
3000a352:	9732      	str	r7, [sp, #200]	; 0xc8
3000a354:	7ccf      	ldrb	r7, [r1, #19]
3000a356:	9733      	str	r7, [sp, #204]	; 0xcc
3000a358:	7d0f      	ldrb	r7, [r1, #20]
3000a35a:	9734      	str	r7, [sp, #208]	; 0xd0
3000a35c:	7d4f      	ldrb	r7, [r1, #21]
3000a35e:	9735      	str	r7, [sp, #212]	; 0xd4
3000a360:	7d8f      	ldrb	r7, [r1, #22]
3000a362:	9736      	str	r7, [sp, #216]	; 0xd8
3000a364:	7dcf      	ldrb	r7, [r1, #23]
3000a366:	9737      	str	r7, [sp, #220]	; 0xdc
3000a368:	7e0f      	ldrb	r7, [r1, #24]
3000a36a:	9738      	str	r7, [sp, #224]	; 0xe0
3000a36c:	7e4f      	ldrb	r7, [r1, #25]
3000a36e:	9739      	str	r7, [sp, #228]	; 0xe4
3000a370:	7e8f      	ldrb	r7, [r1, #26]
3000a372:	973a      	str	r7, [sp, #232]	; 0xe8
3000a374:	7ecf      	ldrb	r7, [r1, #27]
3000a376:	973b      	str	r7, [sp, #236]	; 0xec
3000a378:	7f0f      	ldrb	r7, [r1, #28]
3000a37a:	973c      	str	r7, [sp, #240]	; 0xf0
3000a37c:	7f4f      	ldrb	r7, [r1, #29]
3000a37e:	973d      	str	r7, [sp, #244]	; 0xf4
3000a380:	7f8f      	ldrb	r7, [r1, #30]
3000a382:	7fc9      	ldrb	r1, [r1, #31]
3000a384:	973e      	str	r7, [sp, #248]	; 0xf8
3000a386:	4627      	mov	r7, r4
3000a388:	913f      	str	r1, [sp, #252]	; 0xfc
3000a38a:	4619      	mov	r1, r3
3000a38c:	9408      	str	r4, [sp, #32]
3000a38e:	941b      	str	r4, [sp, #108]	; 0x6c
3000a390:	9403      	str	r4, [sp, #12]
3000a392:	941a      	str	r4, [sp, #104]	; 0x68
3000a394:	9409      	str	r4, [sp, #36]	; 0x24
3000a396:	9302      	str	r3, [sp, #8]
3000a398:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a39c:	e9cd 4400 	strd	r4, r4, [sp]
3000a3a0:	e9cd 4406 	strd	r4, r4, [sp, #24]
3000a3a4:	e9cd 4418 	strd	r4, r4, [sp, #96]	; 0x60
3000a3a8:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a3ac:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a3b0:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a3b4:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a3b8:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a3bc:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a3c0:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a3c4:	b94a      	cbnz	r2, 3000a3da <rxi316_perf_tune+0x102>
3000a3c6:	3101      	adds	r1, #1
3000a3c8:	4699      	mov	r9, r3
3000a3ca:	3301      	adds	r3, #1
3000a3cc:	2b20      	cmp	r3, #32
3000a3ce:	d069      	beq.n	3000a4a4 <rxi316_perf_tune+0x1cc>
3000a3d0:	aa20      	add	r2, sp, #128	; 0x80
3000a3d2:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a3d6:	2a00      	cmp	r2, #0
3000a3d8:	d0f5      	beq.n	3000a3c6 <rxi316_perf_tune+0xee>
3000a3da:	2a01      	cmp	r2, #1
3000a3dc:	d05d      	beq.n	3000a49a <rxi316_perf_tune+0x1c2>
3000a3de:	2a02      	cmp	r2, #2
3000a3e0:	d06e      	beq.n	3000a4c0 <rxi316_perf_tune+0x1e8>
3000a3e2:	2a03      	cmp	r2, #3
3000a3e4:	d06f      	beq.n	3000a4c6 <rxi316_perf_tune+0x1ee>
3000a3e6:	2a04      	cmp	r2, #4
3000a3e8:	d070      	beq.n	3000a4cc <rxi316_perf_tune+0x1f4>
3000a3ea:	2a05      	cmp	r2, #5
3000a3ec:	d071      	beq.n	3000a4d2 <rxi316_perf_tune+0x1fa>
3000a3ee:	2a06      	cmp	r2, #6
3000a3f0:	d072      	beq.n	3000a4d8 <rxi316_perf_tune+0x200>
3000a3f2:	2a07      	cmp	r2, #7
3000a3f4:	f000 82a7 	beq.w	3000a946 <rxi316_perf_tune+0x66e>
3000a3f8:	2a08      	cmp	r2, #8
3000a3fa:	f000 8364 	beq.w	3000aac6 <rxi316_perf_tune+0x7ee>
3000a3fe:	2a09      	cmp	r2, #9
3000a400:	f000 8364 	beq.w	3000aacc <rxi316_perf_tune+0x7f4>
3000a404:	2a0a      	cmp	r2, #10
3000a406:	f000 836c 	beq.w	3000aae2 <rxi316_perf_tune+0x80a>
3000a40a:	2a0b      	cmp	r2, #11
3000a40c:	f000 8364 	beq.w	3000aad8 <rxi316_perf_tune+0x800>
3000a410:	2a0c      	cmp	r2, #12
3000a412:	f000 836b 	beq.w	3000aaec <rxi316_perf_tune+0x814>
3000a416:	2a0d      	cmp	r2, #13
3000a418:	f000 8377 	beq.w	3000ab0a <rxi316_perf_tune+0x832>
3000a41c:	2a0e      	cmp	r2, #14
3000a41e:	f000 836f 	beq.w	3000ab00 <rxi316_perf_tune+0x828>
3000a422:	2a0f      	cmp	r2, #15
3000a424:	f000 8376 	beq.w	3000ab14 <rxi316_perf_tune+0x83c>
3000a428:	2a10      	cmp	r2, #16
3000a42a:	f000 8378 	beq.w	3000ab1e <rxi316_perf_tune+0x846>
3000a42e:	2a11      	cmp	r2, #17
3000a430:	f000 8377 	beq.w	3000ab22 <rxi316_perf_tune+0x84a>
3000a434:	2a12      	cmp	r2, #18
3000a436:	f000 8376 	beq.w	3000ab26 <rxi316_perf_tune+0x84e>
3000a43a:	2a13      	cmp	r2, #19
3000a43c:	f000 8377 	beq.w	3000ab2e <rxi316_perf_tune+0x856>
3000a440:	2a14      	cmp	r2, #20
3000a442:	f000 8372 	beq.w	3000ab2a <rxi316_perf_tune+0x852>
3000a446:	2a15      	cmp	r2, #21
3000a448:	f000 8379 	beq.w	3000ab3e <rxi316_perf_tune+0x866>
3000a44c:	2a16      	cmp	r2, #22
3000a44e:	f000 8374 	beq.w	3000ab3a <rxi316_perf_tune+0x862>
3000a452:	2a17      	cmp	r2, #23
3000a454:	f000 8383 	beq.w	3000ab5e <rxi316_perf_tune+0x886>
3000a458:	2a18      	cmp	r2, #24
3000a45a:	f000 837e 	beq.w	3000ab5a <rxi316_perf_tune+0x882>
3000a45e:	2a19      	cmp	r2, #25
3000a460:	f000 8379 	beq.w	3000ab56 <rxi316_perf_tune+0x87e>
3000a464:	2a1a      	cmp	r2, #26
3000a466:	f000 8374 	beq.w	3000ab52 <rxi316_perf_tune+0x87a>
3000a46a:	2a1b      	cmp	r2, #27
3000a46c:	f000 836f 	beq.w	3000ab4e <rxi316_perf_tune+0x876>
3000a470:	2a1c      	cmp	r2, #28
3000a472:	f000 836a 	beq.w	3000ab4a <rxi316_perf_tune+0x872>
3000a476:	2a1d      	cmp	r2, #29
3000a478:	f000 8365 	beq.w	3000ab46 <rxi316_perf_tune+0x86e>
3000a47c:	2a1e      	cmp	r2, #30
3000a47e:	f000 8360 	beq.w	3000ab42 <rxi316_perf_tune+0x86a>
3000a482:	2a1f      	cmp	r2, #31
3000a484:	f000 8357 	beq.w	3000ab36 <rxi316_perf_tune+0x85e>
3000a488:	2a20      	cmp	r2, #32
3000a48a:	f000 8352 	beq.w	3000ab32 <rxi316_perf_tune+0x85a>
3000a48e:	9808      	ldr	r0, [sp, #32]
3000a490:	2a21      	cmp	r2, #33	; 0x21
3000a492:	bf08      	it	eq
3000a494:	4618      	moveq	r0, r3
3000a496:	9008      	str	r0, [sp, #32]
3000a498:	e797      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a49a:	4698      	mov	r8, r3
3000a49c:	3301      	adds	r3, #1
3000a49e:	3101      	adds	r1, #1
3000a4a0:	2b20      	cmp	r3, #32
3000a4a2:	d195      	bne.n	3000a3d0 <rxi316_perf_tune+0xf8>
3000a4a4:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a4a8:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a4ac:	b1ba      	cbz	r2, 3000a4de <rxi316_perf_tune+0x206>
3000a4ae:	2a01      	cmp	r2, #1
3000a4b0:	f000 830f 	beq.w	3000aad2 <rxi316_perf_tune+0x7fa>
3000a4b4:	2a03      	cmp	r2, #3
3000a4b6:	bf0c      	ite	eq
3000a4b8:	2204      	moveq	r2, #4
3000a4ba:	2203      	movne	r2, #3
3000a4bc:	921d      	str	r2, [sp, #116]	; 0x74
3000a4be:	e010      	b.n	3000a4e2 <rxi316_perf_tune+0x20a>
3000a4c0:	3101      	adds	r1, #1
3000a4c2:	461f      	mov	r7, r3
3000a4c4:	e781      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a4c6:	3101      	adds	r1, #1
3000a4c8:	469a      	mov	sl, r3
3000a4ca:	e77e      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a4cc:	3101      	adds	r1, #1
3000a4ce:	469b      	mov	fp, r3
3000a4d0:	e77b      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a4d2:	3101      	adds	r1, #1
3000a4d4:	9300      	str	r3, [sp, #0]
3000a4d6:	e778      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a4d8:	3101      	adds	r1, #1
3000a4da:	9301      	str	r3, [sp, #4]
3000a4dc:	e775      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a4de:	2201      	movs	r2, #1
3000a4e0:	921d      	str	r2, [sp, #116]	; 0x74
3000a4e2:	3b02      	subs	r3, #2
3000a4e4:	428b      	cmp	r3, r1
3000a4e6:	d005      	beq.n	3000a4f4 <rxi316_perf_tune+0x21c>
3000a4e8:	4b90      	ldr	r3, [pc, #576]	; (3000a72c <rxi316_perf_tune+0x454>)
3000a4ea:	2002      	movs	r0, #2
3000a4ec:	4a90      	ldr	r2, [pc, #576]	; (3000a730 <rxi316_perf_tune+0x458>)
3000a4ee:	4619      	mov	r1, r3
3000a4f0:	f001 fe8a 	bl	3000c208 <rtk_log_write>
3000a4f4:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a4f6:	9a02      	ldr	r2, [sp, #8]
3000a4f8:	4293      	cmp	r3, r2
3000a4fa:	d005      	beq.n	3000a508 <rxi316_perf_tune+0x230>
3000a4fc:	4b8b      	ldr	r3, [pc, #556]	; (3000a72c <rxi316_perf_tune+0x454>)
3000a4fe:	2002      	movs	r0, #2
3000a500:	4a8c      	ldr	r2, [pc, #560]	; (3000a734 <rxi316_perf_tune+0x45c>)
3000a502:	4619      	mov	r1, r3
3000a504:	f001 fe80 	bl	3000c208 <rtk_log_write>
3000a508:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a50a:	2b20      	cmp	r3, #32
3000a50c:	f000 82d8 	beq.w	3000aac0 <rxi316_perf_tune+0x7e8>
3000a510:	1b5a      	subs	r2, r3, r5
3000a512:	3a05      	subs	r2, #5
3000a514:	f002 030f 	and.w	r3, r2, #15
3000a518:	9302      	str	r3, [sp, #8]
3000a51a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a51c:	2b20      	cmp	r3, #32
3000a51e:	f000 82cd 	beq.w	3000aabc <rxi316_perf_tune+0x7e4>
3000a522:	1b5a      	subs	r2, r3, r5
3000a524:	3a06      	subs	r2, #6
3000a526:	0112      	lsls	r2, r2, #4
3000a528:	b2d2      	uxtb	r2, r2
3000a52a:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a52c:	2b20      	cmp	r3, #32
3000a52e:	f000 82c1 	beq.w	3000aab4 <rxi316_perf_tune+0x7dc>
3000a532:	1b5b      	subs	r3, r3, r5
3000a534:	3b07      	subs	r3, #7
3000a536:	021b      	lsls	r3, r3, #8
3000a538:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a53c:	930b      	str	r3, [sp, #44]	; 0x2c
3000a53e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a540:	2b20      	cmp	r3, #32
3000a542:	f000 8214 	beq.w	3000a96e <rxi316_perf_tune+0x696>
3000a546:	1b5b      	subs	r3, r3, r5
3000a548:	3b08      	subs	r3, #8
3000a54a:	031b      	lsls	r3, r3, #12
3000a54c:	b29b      	uxth	r3, r3
3000a54e:	930a      	str	r3, [sp, #40]	; 0x28
3000a550:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a552:	2b20      	cmp	r3, #32
3000a554:	f000 8212 	beq.w	3000a97c <rxi316_perf_tune+0x6a4>
3000a558:	1b5b      	subs	r3, r3, r5
3000a55a:	3b09      	subs	r3, #9
3000a55c:	041b      	lsls	r3, r3, #16
3000a55e:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a562:	9309      	str	r3, [sp, #36]	; 0x24
3000a564:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a566:	2b20      	cmp	r3, #32
3000a568:	f000 820f 	beq.w	3000a98a <rxi316_perf_tune+0x6b2>
3000a56c:	1b5b      	subs	r3, r3, r5
3000a56e:	3b0a      	subs	r3, #10
3000a570:	051b      	lsls	r3, r3, #20
3000a572:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a576:	930c      	str	r3, [sp, #48]	; 0x30
3000a578:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a57a:	2b20      	cmp	r3, #32
3000a57c:	f000 820c 	beq.w	3000a998 <rxi316_perf_tune+0x6c0>
3000a580:	1b5b      	subs	r3, r3, r5
3000a582:	3b0b      	subs	r3, #11
3000a584:	061b      	lsls	r3, r3, #24
3000a586:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a58a:	930d      	str	r3, [sp, #52]	; 0x34
3000a58c:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a58e:	2b20      	cmp	r3, #32
3000a590:	f000 8209 	beq.w	3000a9a6 <rxi316_perf_tune+0x6ce>
3000a594:	1b5b      	subs	r3, r3, r5
3000a596:	3b0c      	subs	r3, #12
3000a598:	071b      	lsls	r3, r3, #28
3000a59a:	930e      	str	r3, [sp, #56]	; 0x38
3000a59c:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a59e:	2b20      	cmp	r3, #32
3000a5a0:	f000 8208 	beq.w	3000a9b4 <rxi316_perf_tune+0x6dc>
3000a5a4:	1b5b      	subs	r3, r3, r5
3000a5a6:	3b0d      	subs	r3, #13
3000a5a8:	f003 030f 	and.w	r3, r3, #15
3000a5ac:	930f      	str	r3, [sp, #60]	; 0x3c
3000a5ae:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a5b0:	2b20      	cmp	r3, #32
3000a5b2:	f000 8205 	beq.w	3000a9c0 <rxi316_perf_tune+0x6e8>
3000a5b6:	1b5b      	subs	r3, r3, r5
3000a5b8:	3b0e      	subs	r3, #14
3000a5ba:	011b      	lsls	r3, r3, #4
3000a5bc:	b2db      	uxtb	r3, r3
3000a5be:	9310      	str	r3, [sp, #64]	; 0x40
3000a5c0:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a5c2:	2b20      	cmp	r3, #32
3000a5c4:	f000 8202 	beq.w	3000a9cc <rxi316_perf_tune+0x6f4>
3000a5c8:	1b5b      	subs	r3, r3, r5
3000a5ca:	3b0f      	subs	r3, #15
3000a5cc:	021b      	lsls	r3, r3, #8
3000a5ce:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a5d2:	9311      	str	r3, [sp, #68]	; 0x44
3000a5d4:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a5d6:	2b20      	cmp	r3, #32
3000a5d8:	f000 81ff 	beq.w	3000a9da <rxi316_perf_tune+0x702>
3000a5dc:	1b5b      	subs	r3, r3, r5
3000a5de:	031b      	lsls	r3, r3, #12
3000a5e0:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a5e4:	9312      	str	r3, [sp, #72]	; 0x48
3000a5e6:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a5e8:	2b20      	cmp	r3, #32
3000a5ea:	f000 81fd 	beq.w	3000a9e8 <rxi316_perf_tune+0x710>
3000a5ee:	1b5b      	subs	r3, r3, r5
3000a5f0:	3b11      	subs	r3, #17
3000a5f2:	041b      	lsls	r3, r3, #16
3000a5f4:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a5f8:	9313      	str	r3, [sp, #76]	; 0x4c
3000a5fa:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a5fc:	2b20      	cmp	r3, #32
3000a5fe:	f000 81fa 	beq.w	3000a9f6 <rxi316_perf_tune+0x71e>
3000a602:	1b5b      	subs	r3, r3, r5
3000a604:	3b12      	subs	r3, #18
3000a606:	051b      	lsls	r3, r3, #20
3000a608:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a60c:	931f      	str	r3, [sp, #124]	; 0x7c
3000a60e:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a610:	2b20      	cmp	r3, #32
3000a612:	f000 81f7 	beq.w	3000aa04 <rxi316_perf_tune+0x72c>
3000a616:	1b5b      	subs	r3, r3, r5
3000a618:	3b13      	subs	r3, #19
3000a61a:	061b      	lsls	r3, r3, #24
3000a61c:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a620:	931e      	str	r3, [sp, #120]	; 0x78
3000a622:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a624:	2b20      	cmp	r3, #32
3000a626:	f000 81f4 	beq.w	3000aa12 <rxi316_perf_tune+0x73a>
3000a62a:	1b5b      	subs	r3, r3, r5
3000a62c:	3b14      	subs	r3, #20
3000a62e:	071b      	lsls	r3, r3, #28
3000a630:	931d      	str	r3, [sp, #116]	; 0x74
3000a632:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a634:	2b20      	cmp	r3, #32
3000a636:	f000 81f3 	beq.w	3000aa20 <rxi316_perf_tune+0x748>
3000a63a:	1b5b      	subs	r3, r3, r5
3000a63c:	3b15      	subs	r3, #21
3000a63e:	f003 030f 	and.w	r3, r3, #15
3000a642:	9318      	str	r3, [sp, #96]	; 0x60
3000a644:	9b06      	ldr	r3, [sp, #24]
3000a646:	2b20      	cmp	r3, #32
3000a648:	f000 81f0 	beq.w	3000aa2c <rxi316_perf_tune+0x754>
3000a64c:	1b5b      	subs	r3, r3, r5
3000a64e:	3b02      	subs	r3, #2
3000a650:	f003 031f 	and.w	r3, r3, #31
3000a654:	9317      	str	r3, [sp, #92]	; 0x5c
3000a656:	9b07      	ldr	r3, [sp, #28]
3000a658:	2b20      	cmp	r3, #32
3000a65a:	f000 81ed 	beq.w	3000aa38 <rxi316_perf_tune+0x760>
3000a65e:	1b5b      	subs	r3, r3, r5
3000a660:	3b02      	subs	r3, #2
3000a662:	015b      	lsls	r3, r3, #5
3000a664:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a668:	9316      	str	r3, [sp, #88]	; 0x58
3000a66a:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a66c:	2b20      	cmp	r3, #32
3000a66e:	f000 81ea 	beq.w	3000aa46 <rxi316_perf_tune+0x76e>
3000a672:	1b5b      	subs	r3, r3, r5
3000a674:	f1b9 0f20 	cmp.w	r9, #32
3000a678:	f1a3 0302 	sub.w	r3, r3, #2
3000a67c:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a680:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a684:	9315      	str	r3, [sp, #84]	; 0x54
3000a686:	f000 81e5 	beq.w	3000aa54 <rxi316_perf_tune+0x77c>
3000a68a:	eba9 0105 	sub.w	r1, r9, r5
3000a68e:	f1b8 0f20 	cmp.w	r8, #32
3000a692:	f1a1 0102 	sub.w	r1, r1, #2
3000a696:	f001 030f 	and.w	r3, r1, #15
3000a69a:	9314      	str	r3, [sp, #80]	; 0x50
3000a69c:	f000 81e0 	beq.w	3000aa60 <rxi316_perf_tune+0x788>
3000a6a0:	eba8 0105 	sub.w	r1, r8, r5
3000a6a4:	2f20      	cmp	r7, #32
3000a6a6:	f1a1 0103 	sub.w	r1, r1, #3
3000a6aa:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a6ae:	b2c9      	uxtb	r1, r1
3000a6b0:	f000 81db 	beq.w	3000aa6a <rxi316_perf_tune+0x792>
3000a6b4:	1b7b      	subs	r3, r7, r5
3000a6b6:	f1ba 0f20 	cmp.w	sl, #32
3000a6ba:	f1a3 0304 	sub.w	r3, r3, #4
3000a6be:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a6c2:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a6c6:	9307      	str	r3, [sp, #28]
3000a6c8:	f000 81d6 	beq.w	3000aa78 <rxi316_perf_tune+0x7a0>
3000a6cc:	ebaa 0305 	sub.w	r3, sl, r5
3000a6d0:	f1bb 0f20 	cmp.w	fp, #32
3000a6d4:	f1a3 0305 	sub.w	r3, r3, #5
3000a6d8:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a6dc:	b29b      	uxth	r3, r3
3000a6de:	9306      	str	r3, [sp, #24]
3000a6e0:	f000 81d1 	beq.w	3000aa86 <rxi316_perf_tune+0x7ae>
3000a6e4:	ebab 0c05 	sub.w	ip, fp, r5
3000a6e8:	9b00      	ldr	r3, [sp, #0]
3000a6ea:	f1ac 0c06 	sub.w	ip, ip, #6
3000a6ee:	2b20      	cmp	r3, #32
3000a6f0:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a6f4:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a6f8:	f000 81cb 	beq.w	3000aa92 <rxi316_perf_tune+0x7ba>
3000a6fc:	eba3 0e05 	sub.w	lr, r3, r5
3000a700:	9b01      	ldr	r3, [sp, #4]
3000a702:	f1ae 0e07 	sub.w	lr, lr, #7
3000a706:	2b20      	cmp	r3, #32
3000a708:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a70c:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a710:	f000 81c5 	beq.w	3000aa9e <rxi316_perf_tune+0x7c6>
3000a714:	1b5f      	subs	r7, r3, r5
3000a716:	3f08      	subs	r7, #8
3000a718:	063f      	lsls	r7, r7, #24
3000a71a:	f007 6370 	and.w	r3, r7, #251658240	; 0xf000000
3000a71e:	9301      	str	r3, [sp, #4]
3000a720:	9b03      	ldr	r3, [sp, #12]
3000a722:	2b20      	cmp	r3, #32
3000a724:	f000 81c2 	beq.w	3000aaac <rxi316_perf_tune+0x7d4>
3000a728:	e006      	b.n	3000a738 <rxi316_perf_tune+0x460>
3000a72a:	bf00      	nop
3000a72c:	3000dd64 	.word	0x3000dd64
3000a730:	3000dd8c 	.word	0x3000dd8c
3000a734:	3000ddd0 	.word	0x3000ddd0
3000a738:	1b5b      	subs	r3, r3, r5
3000a73a:	3b09      	subs	r3, #9
3000a73c:	071b      	lsls	r3, r3, #28
3000a73e:	9300      	str	r3, [sp, #0]
3000a740:	9b04      	ldr	r3, [sp, #16]
3000a742:	2b20      	cmp	r3, #32
3000a744:	f000 8110 	beq.w	3000a968 <rxi316_perf_tune+0x690>
3000a748:	eba3 0805 	sub.w	r8, r3, r5
3000a74c:	f1a8 080a 	sub.w	r8, r8, #10
3000a750:	f008 080f 	and.w	r8, r8, #15
3000a754:	9b05      	ldr	r3, [sp, #20]
3000a756:	2b20      	cmp	r3, #32
3000a758:	f000 8104 	beq.w	3000a964 <rxi316_perf_tune+0x68c>
3000a75c:	1b58      	subs	r0, r3, r5
3000a75e:	380b      	subs	r0, #11
3000a760:	0100      	lsls	r0, r0, #4
3000a762:	b2c0      	uxtb	r0, r0
3000a764:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a766:	2b20      	cmp	r3, #32
3000a768:	f000 80f9 	beq.w	3000a95e <rxi316_perf_tune+0x686>
3000a76c:	eba3 0a05 	sub.w	sl, r3, r5
3000a770:	f1aa 0a02 	sub.w	sl, sl, #2
3000a774:	f00a 0a1f 	and.w	sl, sl, #31
3000a778:	9b08      	ldr	r3, [sp, #32]
3000a77a:	2b20      	cmp	r3, #32
3000a77c:	f000 80ec 	beq.w	3000a958 <rxi316_perf_tune+0x680>
3000a780:	eba3 0905 	sub.w	r9, r3, r5
3000a784:	f1a9 0902 	sub.w	r9, r9, #2
3000a788:	f009 091f 	and.w	r9, r9, #31
3000a78c:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a78e:	2b20      	cmp	r3, #32
3000a790:	f000 80df 	beq.w	3000a952 <rxi316_perf_tune+0x67a>
3000a794:	eba3 0b05 	sub.w	fp, r3, r5
3000a798:	f1ab 0b02 	sub.w	fp, fp, #2
3000a79c:	f00b 0b1f 	and.w	fp, fp, #31
3000a7a0:	2c20      	cmp	r4, #32
3000a7a2:	f000 80d3 	beq.w	3000a94c <rxi316_perf_tune+0x674>
3000a7a6:	1b63      	subs	r3, r4, r5
3000a7a8:	3b02      	subs	r3, #2
3000a7aa:	015b      	lsls	r3, r3, #5
3000a7ac:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a7b0:	9c02      	ldr	r4, [sp, #8]
3000a7b2:	ea40 0008 	orr.w	r0, r0, r8
3000a7b6:	ea43 030b 	orr.w	r3, r3, fp
3000a7ba:	4314      	orrs	r4, r2
3000a7bc:	4622      	mov	r2, r4
3000a7be:	e9dd 450f 	ldrd	r4, r5, [sp, #60]	; 0x3c
3000a7c2:	432c      	orrs	r4, r5
3000a7c4:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000a7c6:	4329      	orrs	r1, r5
3000a7c8:	e9dd 5716 	ldrd	r5, r7, [sp, #88]	; 0x58
3000a7cc:	433d      	orrs	r5, r7
3000a7ce:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
3000a7d0:	433a      	orrs	r2, r7
3000a7d2:	9f11      	ldr	r7, [sp, #68]	; 0x44
3000a7d4:	433c      	orrs	r4, r7
3000a7d6:	9f07      	ldr	r7, [sp, #28]
3000a7d8:	4339      	orrs	r1, r7
3000a7da:	9f15      	ldr	r7, [sp, #84]	; 0x54
3000a7dc:	433d      	orrs	r5, r7
3000a7de:	9f0a      	ldr	r7, [sp, #40]	; 0x28
3000a7e0:	433a      	orrs	r2, r7
3000a7e2:	9f12      	ldr	r7, [sp, #72]	; 0x48
3000a7e4:	433c      	orrs	r4, r7
3000a7e6:	9f06      	ldr	r7, [sp, #24]
3000a7e8:	4339      	orrs	r1, r7
3000a7ea:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a7ec:	433a      	orrs	r2, r7
3000a7ee:	9f13      	ldr	r7, [sp, #76]	; 0x4c
3000a7f0:	ea41 010c 	orr.w	r1, r1, ip
3000a7f4:	433c      	orrs	r4, r7
3000a7f6:	9f0c      	ldr	r7, [sp, #48]	; 0x30
3000a7f8:	ea41 010e 	orr.w	r1, r1, lr
3000a7fc:	433a      	orrs	r2, r7
3000a7fe:	9f1f      	ldr	r7, [sp, #124]	; 0x7c
3000a800:	433c      	orrs	r4, r7
3000a802:	9f0d      	ldr	r7, [sp, #52]	; 0x34
3000a804:	433a      	orrs	r2, r7
3000a806:	9f1e      	ldr	r7, [sp, #120]	; 0x78
3000a808:	433c      	orrs	r4, r7
3000a80a:	9f01      	ldr	r7, [sp, #4]
3000a80c:	4339      	orrs	r1, r7
3000a80e:	9f0e      	ldr	r7, [sp, #56]	; 0x38
3000a810:	433a      	orrs	r2, r7
3000a812:	9f1d      	ldr	r7, [sp, #116]	; 0x74
3000a814:	433c      	orrs	r4, r7
3000a816:	9f00      	ldr	r7, [sp, #0]
3000a818:	4339      	orrs	r1, r7
3000a81a:	4fb7      	ldr	r7, [pc, #732]	; (3000aaf8 <rxi316_perf_tune+0x820>)
3000a81c:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000a820:	f8c7 4404 	str.w	r4, [r7, #1028]	; 0x404
3000a824:	9a18      	ldr	r2, [sp, #96]	; 0x60
3000a826:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000a82a:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000a82e:	f8c7 1410 	str.w	r1, [r7, #1040]	; 0x410
3000a832:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000a836:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000a83a:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000a83e:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000a842:	68f3      	ldr	r3, [r6, #12]
3000a844:	689a      	ldr	r2, [r3, #8]
3000a846:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a84a:	f003 0301 	and.w	r3, r3, #1
3000a84e:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a852:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000a856:	68f3      	ldr	r3, [r6, #12]
3000a858:	689a      	ldr	r2, [r3, #8]
3000a85a:	7a13      	ldrb	r3, [r2, #8]
3000a85c:	6951      	ldr	r1, [r2, #20]
3000a85e:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000a862:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000a866:	0209      	lsls	r1, r1, #8
3000a868:	0412      	lsls	r2, r2, #16
3000a86a:	b289      	uxth	r1, r1
3000a86c:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a870:	430b      	orrs	r3, r1
3000a872:	4313      	orrs	r3, r2
3000a874:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000a878:	68f3      	ldr	r3, [r6, #12]
3000a87a:	6899      	ldr	r1, [r3, #8]
3000a87c:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000a880:	7e09      	ldrb	r1, [r1, #24]
3000a882:	021b      	lsls	r3, r3, #8
3000a884:	0412      	lsls	r2, r2, #16
3000a886:	b29b      	uxth	r3, r3
3000a888:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a88c:	4313      	orrs	r3, r2
3000a88e:	430b      	orrs	r3, r1
3000a890:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000a894:	68f3      	ldr	r3, [r6, #12]
3000a896:	689a      	ldr	r2, [r3, #8]
3000a898:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a89c:	041b      	lsls	r3, r3, #16
3000a89e:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a8a2:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a8a6:	4313      	orrs	r3, r2
3000a8a8:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000a8ac:	68f3      	ldr	r3, [r6, #12]
3000a8ae:	6899      	ldr	r1, [r3, #8]
3000a8b0:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a8b2:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a8b4:	005b      	lsls	r3, r3, #1
3000a8b6:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a8b8:	0412      	lsls	r2, r2, #16
3000a8ba:	f003 0302 	and.w	r3, r3, #2
3000a8be:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a8c2:	4313      	orrs	r3, r2
3000a8c4:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000a8c6:	f000 0101 	and.w	r1, r0, #1
3000a8ca:	0212      	lsls	r2, r2, #8
3000a8cc:	430b      	orrs	r3, r1
3000a8ce:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a8d2:	4313      	orrs	r3, r2
3000a8d4:	4a89      	ldr	r2, [pc, #548]	; (3000aafc <rxi316_perf_tune+0x824>)
3000a8d6:	431a      	orrs	r2, r3
3000a8d8:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000a8dc:	68f3      	ldr	r3, [r6, #12]
3000a8de:	689b      	ldr	r3, [r3, #8]
3000a8e0:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000a8e2:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000a8e6:	68f3      	ldr	r3, [r6, #12]
3000a8e8:	689b      	ldr	r3, [r3, #8]
3000a8ea:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000a8ec:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000a8f0:	68f3      	ldr	r3, [r6, #12]
3000a8f2:	689b      	ldr	r3, [r3, #8]
3000a8f4:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000a8f6:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000a8fa:	68f3      	ldr	r3, [r6, #12]
3000a8fc:	689b      	ldr	r3, [r3, #8]
3000a8fe:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000a900:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000a904:	68f3      	ldr	r3, [r6, #12]
3000a906:	689b      	ldr	r3, [r3, #8]
3000a908:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000a90a:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000a90e:	68f3      	ldr	r3, [r6, #12]
3000a910:	689b      	ldr	r3, [r3, #8]
3000a912:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000a914:	f003 030f 	and.w	r3, r3, #15
3000a918:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000a91c:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000a920:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000a924:	f043 0304 	orr.w	r3, r3, #4
3000a928:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000a92c:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000a930:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000a934:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000a938:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000a93c:	b041      	add	sp, #260	; 0x104
3000a93e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a942:	2502      	movs	r5, #2
3000a944:	e4d7      	b.n	3000a2f6 <rxi316_perf_tune+0x1e>
3000a946:	3101      	adds	r1, #1
3000a948:	9303      	str	r3, [sp, #12]
3000a94a:	e53e      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000a94c:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a950:	e72e      	b.n	3000a7b0 <rxi316_perf_tune+0x4d8>
3000a952:	f04f 0b1f 	mov.w	fp, #31
3000a956:	e723      	b.n	3000a7a0 <rxi316_perf_tune+0x4c8>
3000a958:	f04f 091f 	mov.w	r9, #31
3000a95c:	e716      	b.n	3000a78c <rxi316_perf_tune+0x4b4>
3000a95e:	f04f 0a1f 	mov.w	sl, #31
3000a962:	e709      	b.n	3000a778 <rxi316_perf_tune+0x4a0>
3000a964:	20f0      	movs	r0, #240	; 0xf0
3000a966:	e6fd      	b.n	3000a764 <rxi316_perf_tune+0x48c>
3000a968:	f04f 080f 	mov.w	r8, #15
3000a96c:	e6f2      	b.n	3000a754 <rxi316_perf_tune+0x47c>
3000a96e:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a972:	930a      	str	r3, [sp, #40]	; 0x28
3000a974:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a976:	2b20      	cmp	r3, #32
3000a978:	f47f adee 	bne.w	3000a558 <rxi316_perf_tune+0x280>
3000a97c:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a980:	9309      	str	r3, [sp, #36]	; 0x24
3000a982:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a984:	2b20      	cmp	r3, #32
3000a986:	f47f adf1 	bne.w	3000a56c <rxi316_perf_tune+0x294>
3000a98a:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a98e:	930c      	str	r3, [sp, #48]	; 0x30
3000a990:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a992:	2b20      	cmp	r3, #32
3000a994:	f47f adf4 	bne.w	3000a580 <rxi316_perf_tune+0x2a8>
3000a998:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a99c:	930d      	str	r3, [sp, #52]	; 0x34
3000a99e:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a9a0:	2b20      	cmp	r3, #32
3000a9a2:	f47f adf7 	bne.w	3000a594 <rxi316_perf_tune+0x2bc>
3000a9a6:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a9aa:	930e      	str	r3, [sp, #56]	; 0x38
3000a9ac:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a9ae:	2b20      	cmp	r3, #32
3000a9b0:	f47f adf8 	bne.w	3000a5a4 <rxi316_perf_tune+0x2cc>
3000a9b4:	230f      	movs	r3, #15
3000a9b6:	930f      	str	r3, [sp, #60]	; 0x3c
3000a9b8:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a9ba:	2b20      	cmp	r3, #32
3000a9bc:	f47f adfb 	bne.w	3000a5b6 <rxi316_perf_tune+0x2de>
3000a9c0:	23f0      	movs	r3, #240	; 0xf0
3000a9c2:	9310      	str	r3, [sp, #64]	; 0x40
3000a9c4:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a9c6:	2b20      	cmp	r3, #32
3000a9c8:	f47f adfe 	bne.w	3000a5c8 <rxi316_perf_tune+0x2f0>
3000a9cc:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a9d0:	9311      	str	r3, [sp, #68]	; 0x44
3000a9d2:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a9d4:	2b20      	cmp	r3, #32
3000a9d6:	f47f ae01 	bne.w	3000a5dc <rxi316_perf_tune+0x304>
3000a9da:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a9de:	9312      	str	r3, [sp, #72]	; 0x48
3000a9e0:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a9e2:	2b20      	cmp	r3, #32
3000a9e4:	f47f ae03 	bne.w	3000a5ee <rxi316_perf_tune+0x316>
3000a9e8:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a9ec:	9313      	str	r3, [sp, #76]	; 0x4c
3000a9ee:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a9f0:	2b20      	cmp	r3, #32
3000a9f2:	f47f ae06 	bne.w	3000a602 <rxi316_perf_tune+0x32a>
3000a9f6:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a9fa:	931f      	str	r3, [sp, #124]	; 0x7c
3000a9fc:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a9fe:	2b20      	cmp	r3, #32
3000aa00:	f47f ae09 	bne.w	3000a616 <rxi316_perf_tune+0x33e>
3000aa04:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000aa08:	931e      	str	r3, [sp, #120]	; 0x78
3000aa0a:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000aa0c:	2b20      	cmp	r3, #32
3000aa0e:	f47f ae0c 	bne.w	3000a62a <rxi316_perf_tune+0x352>
3000aa12:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000aa16:	931d      	str	r3, [sp, #116]	; 0x74
3000aa18:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000aa1a:	2b20      	cmp	r3, #32
3000aa1c:	f47f ae0d 	bne.w	3000a63a <rxi316_perf_tune+0x362>
3000aa20:	230f      	movs	r3, #15
3000aa22:	9318      	str	r3, [sp, #96]	; 0x60
3000aa24:	9b06      	ldr	r3, [sp, #24]
3000aa26:	2b20      	cmp	r3, #32
3000aa28:	f47f ae10 	bne.w	3000a64c <rxi316_perf_tune+0x374>
3000aa2c:	231f      	movs	r3, #31
3000aa2e:	9317      	str	r3, [sp, #92]	; 0x5c
3000aa30:	9b07      	ldr	r3, [sp, #28]
3000aa32:	2b20      	cmp	r3, #32
3000aa34:	f47f ae13 	bne.w	3000a65e <rxi316_perf_tune+0x386>
3000aa38:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000aa3c:	9316      	str	r3, [sp, #88]	; 0x58
3000aa3e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000aa40:	2b20      	cmp	r3, #32
3000aa42:	f47f ae16 	bne.w	3000a672 <rxi316_perf_tune+0x39a>
3000aa46:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000aa4a:	f1b9 0f20 	cmp.w	r9, #32
3000aa4e:	9315      	str	r3, [sp, #84]	; 0x54
3000aa50:	f47f ae1b 	bne.w	3000a68a <rxi316_perf_tune+0x3b2>
3000aa54:	230f      	movs	r3, #15
3000aa56:	f1b8 0f20 	cmp.w	r8, #32
3000aa5a:	9314      	str	r3, [sp, #80]	; 0x50
3000aa5c:	f47f ae20 	bne.w	3000a6a0 <rxi316_perf_tune+0x3c8>
3000aa60:	2f20      	cmp	r7, #32
3000aa62:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000aa66:	f47f ae25 	bne.w	3000a6b4 <rxi316_perf_tune+0x3dc>
3000aa6a:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000aa6e:	f1ba 0f20 	cmp.w	sl, #32
3000aa72:	9307      	str	r3, [sp, #28]
3000aa74:	f47f ae2a 	bne.w	3000a6cc <rxi316_perf_tune+0x3f4>
3000aa78:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000aa7c:	f1bb 0f20 	cmp.w	fp, #32
3000aa80:	9306      	str	r3, [sp, #24]
3000aa82:	f47f ae2f 	bne.w	3000a6e4 <rxi316_perf_tune+0x40c>
3000aa86:	9b00      	ldr	r3, [sp, #0]
3000aa88:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000aa8c:	2b20      	cmp	r3, #32
3000aa8e:	f47f ae35 	bne.w	3000a6fc <rxi316_perf_tune+0x424>
3000aa92:	9b01      	ldr	r3, [sp, #4]
3000aa94:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000aa98:	2b20      	cmp	r3, #32
3000aa9a:	f47f ae3b 	bne.w	3000a714 <rxi316_perf_tune+0x43c>
3000aa9e:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000aaa2:	9301      	str	r3, [sp, #4]
3000aaa4:	9b03      	ldr	r3, [sp, #12]
3000aaa6:	2b20      	cmp	r3, #32
3000aaa8:	f47f ae46 	bne.w	3000a738 <rxi316_perf_tune+0x460>
3000aaac:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000aab0:	9300      	str	r3, [sp, #0]
3000aab2:	e645      	b.n	3000a740 <rxi316_perf_tune+0x468>
3000aab4:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000aab8:	930b      	str	r3, [sp, #44]	; 0x2c
3000aaba:	e540      	b.n	3000a53e <rxi316_perf_tune+0x266>
3000aabc:	22f0      	movs	r2, #240	; 0xf0
3000aabe:	e534      	b.n	3000a52a <rxi316_perf_tune+0x252>
3000aac0:	230f      	movs	r3, #15
3000aac2:	9302      	str	r3, [sp, #8]
3000aac4:	e529      	b.n	3000a51a <rxi316_perf_tune+0x242>
3000aac6:	3101      	adds	r1, #1
3000aac8:	9304      	str	r3, [sp, #16]
3000aaca:	e47e      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000aacc:	3101      	adds	r1, #1
3000aace:	9305      	str	r3, [sp, #20]
3000aad0:	e47b      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000aad2:	2202      	movs	r2, #2
3000aad4:	921d      	str	r2, [sp, #116]	; 0x74
3000aad6:	e504      	b.n	3000a4e2 <rxi316_perf_tune+0x20a>
3000aad8:	9a02      	ldr	r2, [sp, #8]
3000aada:	9307      	str	r3, [sp, #28]
3000aadc:	3201      	adds	r2, #1
3000aade:	9202      	str	r2, [sp, #8]
3000aae0:	e473      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000aae2:	9a02      	ldr	r2, [sp, #8]
3000aae4:	9306      	str	r3, [sp, #24]
3000aae6:	3201      	adds	r2, #1
3000aae8:	9202      	str	r2, [sp, #8]
3000aaea:	e46e      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000aaec:	9a02      	ldr	r2, [sp, #8]
3000aaee:	931a      	str	r3, [sp, #104]	; 0x68
3000aaf0:	3201      	adds	r2, #1
3000aaf2:	9202      	str	r2, [sp, #8]
3000aaf4:	e469      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000aaf6:	bf00      	nop
3000aaf8:	41100000 	.word	0x41100000
3000aafc:	40000008 	.word	0x40000008
3000ab00:	9a02      	ldr	r2, [sp, #8]
3000ab02:	461c      	mov	r4, r3
3000ab04:	3201      	adds	r2, #1
3000ab06:	9202      	str	r2, [sp, #8]
3000ab08:	e45f      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab0a:	9a02      	ldr	r2, [sp, #8]
3000ab0c:	931c      	str	r3, [sp, #112]	; 0x70
3000ab0e:	3201      	adds	r2, #1
3000ab10:	9202      	str	r2, [sp, #8]
3000ab12:	e45a      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab14:	9a02      	ldr	r2, [sp, #8]
3000ab16:	931b      	str	r3, [sp, #108]	; 0x6c
3000ab18:	3201      	adds	r2, #1
3000ab1a:	9202      	str	r2, [sp, #8]
3000ab1c:	e455      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab1e:	930d      	str	r3, [sp, #52]	; 0x34
3000ab20:	e453      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab22:	930c      	str	r3, [sp, #48]	; 0x30
3000ab24:	e451      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab26:	930b      	str	r3, [sp, #44]	; 0x2c
3000ab28:	e44f      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab2a:	9309      	str	r3, [sp, #36]	; 0x24
3000ab2c:	e44d      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab2e:	930a      	str	r3, [sp, #40]	; 0x28
3000ab30:	e44b      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab32:	9319      	str	r3, [sp, #100]	; 0x64
3000ab34:	e449      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab36:	9318      	str	r3, [sp, #96]	; 0x60
3000ab38:	e447      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab3a:	930f      	str	r3, [sp, #60]	; 0x3c
3000ab3c:	e445      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab3e:	930e      	str	r3, [sp, #56]	; 0x38
3000ab40:	e443      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab42:	9317      	str	r3, [sp, #92]	; 0x5c
3000ab44:	e441      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab46:	9316      	str	r3, [sp, #88]	; 0x58
3000ab48:	e43f      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab4a:	9315      	str	r3, [sp, #84]	; 0x54
3000ab4c:	e43d      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab4e:	9314      	str	r3, [sp, #80]	; 0x50
3000ab50:	e43b      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab52:	9313      	str	r3, [sp, #76]	; 0x4c
3000ab54:	e439      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab56:	9312      	str	r3, [sp, #72]	; 0x48
3000ab58:	e437      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab5a:	9311      	str	r3, [sp, #68]	; 0x44
3000ab5c:	e435      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab5e:	9310      	str	r3, [sp, #64]	; 0x40
3000ab60:	e433      	b.n	3000a3ca <rxi316_perf_tune+0xf2>
3000ab62:	bf00      	nop

3000ab64 <rxi316_dram_init>:
3000ab64:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000ab68:	4604      	mov	r4, r0
3000ab6a:	b08f      	sub	sp, #60	; 0x3c
3000ab6c:	4b10      	ldr	r3, [pc, #64]	; (3000abb0 <rxi316_dram_init+0x4c>)
3000ab6e:	4798      	blx	r3
3000ab70:	f894 b014 	ldrb.w	fp, [r4, #20]
3000ab74:	2101      	movs	r1, #1
3000ab76:	900c      	str	r0, [sp, #48]	; 0x30
3000ab78:	fa01 f30b 	lsl.w	r3, r1, fp
3000ab7c:	9302      	str	r3, [sp, #8]
3000ab7e:	6823      	ldr	r3, [r4, #0]
3000ab80:	78da      	ldrb	r2, [r3, #3]
3000ab82:	6923      	ldr	r3, [r4, #16]
3000ab84:	fa03 f30b 	lsl.w	r3, r3, fp
3000ab88:	b1a2      	cbz	r2, 3000abb4 <rxi316_dram_init+0x50>
3000ab8a:	428a      	cmp	r2, r1
3000ab8c:	f000 847c 	beq.w	3000b488 <rxi316_dram_init+0x924>
3000ab90:	2a02      	cmp	r2, #2
3000ab92:	bf0b      	itete	eq
3000ab94:	2206      	moveq	r2, #6
3000ab96:	220a      	movne	r2, #10
3000ab98:	2540      	moveq	r5, #64	; 0x40
3000ab9a:	2580      	movne	r5, #128	; 0x80
3000ab9c:	9207      	str	r2, [sp, #28]
3000ab9e:	bf0c      	ite	eq
3000aba0:	2204      	moveq	r2, #4
3000aba2:	2208      	movne	r2, #8
3000aba4:	9208      	str	r2, [sp, #32]
3000aba6:	bf0c      	ite	eq
3000aba8:	2208      	moveq	r2, #8
3000abaa:	2210      	movne	r2, #16
3000abac:	9201      	str	r2, [sp, #4]
3000abae:	e007      	b.n	3000abc0 <rxi316_dram_init+0x5c>
3000abb0:	300081f1 	.word	0x300081f1
3000abb4:	2203      	movs	r2, #3
3000abb6:	25c0      	movs	r5, #192	; 0xc0
3000abb8:	9108      	str	r1, [sp, #32]
3000abba:	9207      	str	r2, [sp, #28]
3000abbc:	2202      	movs	r2, #2
3000abbe:	9201      	str	r2, [sp, #4]
3000abc0:	6860      	ldr	r0, [r4, #4]
3000abc2:	f04f 31ff 	mov.w	r1, #4294967295
3000abc6:	4ac2      	ldr	r2, [pc, #776]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000abc8:	6806      	ldr	r6, [r0, #0]
3000abca:	960d      	str	r6, [sp, #52]	; 0x34
3000abcc:	6846      	ldr	r6, [r0, #4]
3000abce:	6880      	ldr	r0, [r0, #8]
3000abd0:	9609      	str	r6, [sp, #36]	; 0x24
3000abd2:	9003      	str	r0, [sp, #12]
3000abd4:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000abd8:	6820      	ldr	r0, [r4, #0]
3000abda:	7981      	ldrb	r1, [r0, #6]
3000abdc:	6882      	ldr	r2, [r0, #8]
3000abde:	b129      	cbz	r1, 3000abec <rxi316_dram_init+0x88>
3000abe0:	2901      	cmp	r1, #1
3000abe2:	f000 8449 	beq.w	3000b478 <rxi316_dram_init+0x914>
3000abe6:	2903      	cmp	r1, #3
3000abe8:	f000 8456 	beq.w	3000b498 <rxi316_dram_init+0x934>
3000abec:	3a06      	subs	r2, #6
3000abee:	7881      	ldrb	r1, [r0, #2]
3000abf0:	f002 020f 	and.w	r2, r2, #15
3000abf4:	7940      	ldrb	r0, [r0, #5]
3000abf6:	0109      	lsls	r1, r1, #4
3000abf8:	0280      	lsls	r0, r0, #10
3000abfa:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000abfe:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000ac02:	4301      	orrs	r1, r0
3000ac04:	430d      	orrs	r5, r1
3000ac06:	432a      	orrs	r2, r5
3000ac08:	4db1      	ldr	r5, [pc, #708]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000ac0a:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000ac0e:	68e2      	ldr	r2, [r4, #12]
3000ac10:	6812      	ldr	r2, [r2, #0]
3000ac12:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000ac16:	f002 0207 	and.w	r2, r2, #7
3000ac1a:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000ac1e:	68e2      	ldr	r2, [r4, #12]
3000ac20:	6811      	ldr	r1, [r2, #0]
3000ac22:	6822      	ldr	r2, [r4, #0]
3000ac24:	6848      	ldr	r0, [r1, #4]
3000ac26:	f892 9000 	ldrb.w	r9, [r2]
3000ac2a:	688a      	ldr	r2, [r1, #8]
3000ac2c:	9004      	str	r0, [sp, #16]
3000ac2e:	0300      	lsls	r0, r0, #12
3000ac30:	0412      	lsls	r2, r2, #16
3000ac32:	f009 070f 	and.w	r7, r9, #15
3000ac36:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000ac3a:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000ac3e:	4302      	orrs	r2, r0
3000ac40:	68c8      	ldr	r0, [r1, #12]
3000ac42:	0440      	lsls	r0, r0, #17
3000ac44:	433a      	orrs	r2, r7
3000ac46:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000ac4a:	6908      	ldr	r0, [r1, #16]
3000ac4c:	0480      	lsls	r0, r0, #18
3000ac4e:	4332      	orrs	r2, r6
3000ac50:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000ac54:	4310      	orrs	r0, r2
3000ac56:	694a      	ldr	r2, [r1, #20]
3000ac58:	04d2      	lsls	r2, r2, #19
3000ac5a:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000ac5e:	4310      	orrs	r0, r2
3000ac60:	698a      	ldr	r2, [r1, #24]
3000ac62:	0552      	lsls	r2, r2, #21
3000ac64:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000ac68:	4310      	orrs	r0, r2
3000ac6a:	69ca      	ldr	r2, [r1, #28]
3000ac6c:	0592      	lsls	r2, r2, #22
3000ac6e:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000ac72:	4310      	orrs	r0, r2
3000ac74:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000ac76:	0612      	lsls	r2, r2, #24
3000ac78:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000ac7c:	4302      	orrs	r2, r0
3000ac7e:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000ac82:	06c0      	lsls	r0, r0, #27
3000ac84:	0789      	lsls	r1, r1, #30
3000ac86:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000ac8a:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000ac8e:	4310      	orrs	r0, r2
3000ac90:	9a02      	ldr	r2, [sp, #8]
3000ac92:	0212      	lsls	r2, r2, #8
3000ac94:	4308      	orrs	r0, r1
3000ac96:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000ac9a:	4310      	orrs	r0, r2
3000ac9c:	6068      	str	r0, [r5, #4]
3000ac9e:	6822      	ldr	r2, [r4, #0]
3000aca0:	7992      	ldrb	r2, [r2, #6]
3000aca2:	2a00      	cmp	r2, #0
3000aca4:	f040 8317 	bne.w	3000b2d6 <rxi316_dram_init+0x772>
3000aca8:	686a      	ldr	r2, [r5, #4]
3000acaa:	f042 0210 	orr.w	r2, r2, #16
3000acae:	606a      	str	r2, [r5, #4]
3000acb0:	68e2      	ldr	r2, [r4, #12]
3000acb2:	9802      	ldr	r0, [sp, #8]
3000acb4:	6812      	ldr	r2, [r2, #0]
3000acb6:	f100 3aff 	add.w	sl, r0, #4294967295
3000acba:	6861      	ldr	r1, [r4, #4]
3000acbc:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000acbe:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000acc2:	9005      	str	r0, [sp, #20]
3000acc4:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000acc6:	fa2e f80b 	lsr.w	r8, lr, fp
3000acca:	6949      	ldr	r1, [r1, #20]
3000accc:	ea1e 0f0a 	tst.w	lr, sl
3000acd0:	9006      	str	r0, [sp, #24]
3000acd2:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000acd4:	fa21 fe0b 	lsr.w	lr, r1, fp
3000acd8:	bf18      	it	ne
3000acda:	f108 0801 	addne.w	r8, r8, #1
3000acde:	ea11 0f0a 	tst.w	r1, sl
3000ace2:	9906      	ldr	r1, [sp, #24]
3000ace4:	950a      	str	r5, [sp, #40]	; 0x28
3000ace6:	bf18      	it	ne
3000ace8:	f10e 0e01 	addne.w	lr, lr, #1
3000acec:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000acee:	0049      	lsls	r1, r1, #1
3000acf0:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000acf4:	950b      	str	r5, [sp, #44]	; 0x2c
3000acf6:	f001 0102 	and.w	r1, r1, #2
3000acfa:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000acfe:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000ad02:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000ad06:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000ad0a:	0080      	lsls	r0, r0, #2
3000ad0c:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000ad10:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000ad14:	9006      	str	r0, [sp, #24]
3000ad16:	013f      	lsls	r7, r7, #4
3000ad18:	9805      	ldr	r0, [sp, #20]
3000ad1a:	f00c 0c08 	and.w	ip, ip, #8
3000ad1e:	0176      	lsls	r6, r6, #5
3000ad20:	f007 0710 	and.w	r7, r7, #16
3000ad24:	f000 0001 	and.w	r0, r0, #1
3000ad28:	f006 0620 	and.w	r6, r6, #32
3000ad2c:	4605      	mov	r5, r0
3000ad2e:	9806      	ldr	r0, [sp, #24]
3000ad30:	f000 0004 	and.w	r0, r0, #4
3000ad34:	4301      	orrs	r1, r0
3000ad36:	4329      	orrs	r1, r5
3000ad38:	ea41 0c0c 	orr.w	ip, r1, ip
3000ad3c:	990a      	ldr	r1, [sp, #40]	; 0x28
3000ad3e:	ea4c 0707 	orr.w	r7, ip, r7
3000ad42:	018d      	lsls	r5, r1, #6
3000ad44:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ad46:	4337      	orrs	r7, r6
3000ad48:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000ad4c:	0208      	lsls	r0, r1, #8
3000ad4e:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000ad50:	432f      	orrs	r7, r5
3000ad52:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000ad56:	06c9      	lsls	r1, r1, #27
3000ad58:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000ad5c:	4307      	orrs	r7, r0
3000ad5e:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000ad62:	0736      	lsls	r6, r6, #28
3000ad64:	430f      	orrs	r7, r1
3000ad66:	0752      	lsls	r2, r2, #29
3000ad68:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000ad6c:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000ad70:	433e      	orrs	r6, r7
3000ad72:	4316      	orrs	r6, r2
3000ad74:	4a56      	ldr	r2, [pc, #344]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000ad76:	ea46 0608 	orr.w	r6, r6, r8
3000ad7a:	ea46 060e 	orr.w	r6, r6, lr
3000ad7e:	6096      	str	r6, [r2, #8]
3000ad80:	6862      	ldr	r2, [r4, #4]
3000ad82:	6990      	ldr	r0, [r2, #24]
3000ad84:	9a01      	ldr	r2, [sp, #4]
3000ad86:	2a10      	cmp	r2, #16
3000ad88:	6822      	ldr	r2, [r4, #0]
3000ad8a:	7811      	ldrb	r1, [r2, #0]
3000ad8c:	7916      	ldrb	r6, [r2, #4]
3000ad8e:	f000 833d 	beq.w	3000b40c <rxi316_dram_init+0x8a8>
3000ad92:	f1a1 0502 	sub.w	r5, r1, #2
3000ad96:	f1a1 0e09 	sub.w	lr, r1, #9
3000ad9a:	9f01      	ldr	r7, [sp, #4]
3000ad9c:	fab5 f585 	clz	r5, r5
3000ada0:	fabe fe8e 	clz	lr, lr
3000ada4:	2f08      	cmp	r7, #8
3000ada6:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000adaa:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000adae:	ea45 020e 	orr.w	r2, r5, lr
3000adb2:	f000 829e 	beq.w	3000b2f2 <rxi316_dram_init+0x78e>
3000adb6:	9f01      	ldr	r7, [sp, #4]
3000adb8:	2f04      	cmp	r7, #4
3000adba:	f000 83a5 	beq.w	3000b508 <rxi316_dram_init+0x9a4>
3000adbe:	9f01      	ldr	r7, [sp, #4]
3000adc0:	2f02      	cmp	r7, #2
3000adc2:	bf08      	it	eq
3000adc4:	4310      	orreq	r0, r2
3000adc6:	2a00      	cmp	r2, #0
3000adc8:	f040 8298 	bne.w	3000b2fc <rxi316_dram_init+0x798>
3000adcc:	2903      	cmp	r1, #3
3000adce:	f000 8368 	beq.w	3000b4a2 <rxi316_dram_init+0x93e>
3000add2:	2500      	movs	r5, #0
3000add4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000add8:	462f      	mov	r7, r5
3000adda:	433d      	orrs	r5, r7
3000addc:	4f3c      	ldr	r7, [pc, #240]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000adde:	430d      	orrs	r5, r1
3000ade0:	4305      	orrs	r5, r0
3000ade2:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000ade6:	637e      	str	r6, [r7, #52]	; 0x34
3000ade8:	6862      	ldr	r2, [r4, #4]
3000adea:	9903      	ldr	r1, [sp, #12]
3000adec:	69d2      	ldr	r2, [r2, #28]
3000adee:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000adf2:	63ba      	str	r2, [r7, #56]	; 0x38
3000adf4:	6822      	ldr	r2, [r4, #0]
3000adf6:	7810      	ldrb	r0, [r2, #0]
3000adf8:	6862      	ldr	r2, [r4, #4]
3000adfa:	2803      	cmp	r0, #3
3000adfc:	6a11      	ldr	r1, [r2, #32]
3000adfe:	f000 82f8 	beq.w	3000b3f2 <rxi316_dram_init+0x88e>
3000ae02:	2809      	cmp	r0, #9
3000ae04:	f000 830d 	beq.w	3000b422 <rxi316_dram_init+0x8be>
3000ae08:	4831      	ldr	r0, [pc, #196]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000ae0a:	63c1      	str	r1, [r0, #60]	; 0x3c
3000ae0c:	6862      	ldr	r2, [r4, #4]
3000ae0e:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000ae10:	6402      	str	r2, [r0, #64]	; 0x40
3000ae12:	6865      	ldr	r5, [r4, #4]
3000ae14:	68e8      	ldr	r0, [r5, #12]
3000ae16:	682a      	ldr	r2, [r5, #0]
3000ae18:	6869      	ldr	r1, [r5, #4]
3000ae1a:	4402      	add	r2, r0
3000ae1c:	68ae      	ldr	r6, [r5, #8]
3000ae1e:	4401      	add	r1, r0
3000ae20:	ea1a 0f02 	tst.w	sl, r2
3000ae24:	f000 82b8 	beq.w	3000b398 <rxi316_dram_init+0x834>
3000ae28:	fa22 f20b 	lsr.w	r2, r2, fp
3000ae2c:	1c50      	adds	r0, r2, #1
3000ae2e:	ea1a 0f01 	tst.w	sl, r1
3000ae32:	fa21 f50b 	lsr.w	r5, r1, fp
3000ae36:	6822      	ldr	r2, [r4, #0]
3000ae38:	fa26 f10b 	lsr.w	r1, r6, fp
3000ae3c:	bf18      	it	ne
3000ae3e:	3501      	addne	r5, #1
3000ae40:	ea1a 0f06 	tst.w	sl, r6
3000ae44:	7816      	ldrb	r6, [r2, #0]
3000ae46:	bf18      	it	ne
3000ae48:	3101      	addne	r1, #1
3000ae4a:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000ae4e:	1eb2      	subs	r2, r6, #2
3000ae50:	2a01      	cmp	r2, #1
3000ae52:	f240 82a4 	bls.w	3000b39e <rxi316_dram_init+0x83a>
3000ae56:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000ae5a:	fbb7 f2f3 	udiv	r2, r7, r3
3000ae5e:	fb03 7712 	mls	r7, r3, r2, r7
3000ae62:	b107      	cbz	r7, 3000ae66 <rxi316_dram_init+0x302>
3000ae64:	3201      	adds	r2, #1
3000ae66:	2e09      	cmp	r6, #9
3000ae68:	f040 82b3 	bne.w	3000b3d2 <rxi316_dram_init+0x86e>
3000ae6c:	9e08      	ldr	r6, [sp, #32]
3000ae6e:	fa26 f60b 	lsr.w	r6, r6, fp
3000ae72:	016d      	lsls	r5, r5, #5
3000ae74:	f000 001f 	and.w	r0, r0, #31
3000ae78:	0289      	lsls	r1, r1, #10
3000ae7a:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000ae7e:	0512      	lsls	r2, r2, #20
3000ae80:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000ae84:	4305      	orrs	r5, r0
3000ae86:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000ae8a:	430d      	orrs	r5, r1
3000ae8c:	4910      	ldr	r1, [pc, #64]	; (3000aed0 <rxi316_dram_init+0x36c>)
3000ae8e:	4315      	orrs	r5, r2
3000ae90:	0672      	lsls	r2, r6, #25
3000ae92:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000ae96:	432a      	orrs	r2, r5
3000ae98:	630a      	str	r2, [r1, #48]	; 0x30
3000ae9a:	68a0      	ldr	r0, [r4, #8]
3000ae9c:	6802      	ldr	r2, [r0, #0]
3000ae9e:	fbb2 f1f3 	udiv	r1, r2, r3
3000aea2:	fb03 2211 	mls	r2, r3, r1, r2
3000aea6:	2a00      	cmp	r2, #0
3000aea8:	f000 8274 	beq.w	3000b394 <rxi316_dram_init+0x830>
3000aeac:	1c4a      	adds	r2, r1, #1
3000aeae:	9205      	str	r2, [sp, #20]
3000aeb0:	6842      	ldr	r2, [r0, #4]
3000aeb2:	fbb2 f0f3 	udiv	r0, r2, r3
3000aeb6:	fb03 2210 	mls	r2, r3, r0, r2
3000aeba:	2a00      	cmp	r2, #0
3000aebc:	f000 8268 	beq.w	3000b390 <rxi316_dram_init+0x82c>
3000aec0:	38c7      	subs	r0, #199	; 0xc7
3000aec2:	68e2      	ldr	r2, [r4, #12]
3000aec4:	6811      	ldr	r1, [r2, #0]
3000aec6:	4a03      	ldr	r2, [pc, #12]	; (3000aed4 <rxi316_dram_init+0x370>)
3000aec8:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000aecc:	e004      	b.n	3000aed8 <rxi316_dram_init+0x374>
3000aece:	bf00      	nop
3000aed0:	41100000 	.word	0x41100000
3000aed4:	00ffff00 	.word	0x00ffff00
3000aed8:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000aedc:	900a      	str	r0, [sp, #40]	; 0x28
3000aede:	0712      	lsls	r2, r2, #28
3000aee0:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000aee4:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000aee8:	430a      	orrs	r2, r1
3000aeea:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000aeee:	430a      	orrs	r2, r1
3000aef0:	49c3      	ldr	r1, [pc, #780]	; (3000b200 <rxi316_dram_init+0x69c>)
3000aef2:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000aef6:	610a      	str	r2, [r1, #16]
3000aef8:	68a2      	ldr	r2, [r4, #8]
3000aefa:	6911      	ldr	r1, [r2, #16]
3000aefc:	fbb1 fcf3 	udiv	ip, r1, r3
3000af00:	fb03 111c 	mls	r1, r3, ip, r1
3000af04:	2900      	cmp	r1, #0
3000af06:	f000 8240 	beq.w	3000b38a <rxi316_dram_init+0x826>
3000af0a:	f10c 0101 	add.w	r1, ip, #1
3000af0e:	9106      	str	r1, [sp, #24]
3000af10:	6991      	ldr	r1, [r2, #24]
3000af12:	fbb1 f0f3 	udiv	r0, r1, r3
3000af16:	fb03 1110 	mls	r1, r3, r0, r1
3000af1a:	b101      	cbz	r1, 3000af1e <rxi316_dram_init+0x3ba>
3000af1c:	3001      	adds	r0, #1
3000af1e:	68d1      	ldr	r1, [r2, #12]
3000af20:	fbb1 f5f3 	udiv	r5, r1, r3
3000af24:	fb03 1115 	mls	r1, r3, r5, r1
3000af28:	9503      	str	r5, [sp, #12]
3000af2a:	b109      	cbz	r1, 3000af30 <rxi316_dram_init+0x3cc>
3000af2c:	3501      	adds	r5, #1
3000af2e:	9503      	str	r5, [sp, #12]
3000af30:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000af32:	fbb5 f1f3 	udiv	r1, r5, r3
3000af36:	fb03 5511 	mls	r5, r3, r1, r5
3000af3a:	b105      	cbz	r5, 3000af3e <rxi316_dram_init+0x3da>
3000af3c:	3101      	adds	r1, #1
3000af3e:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000af40:	0349      	lsls	r1, r1, #13
3000af42:	68e2      	ldr	r2, [r4, #12]
3000af44:	fa25 f60b 	lsr.w	r6, r5, fp
3000af48:	ea15 0f0a 	tst.w	r5, sl
3000af4c:	6812      	ldr	r2, [r2, #0]
3000af4e:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000af52:	bf18      	it	ne
3000af54:	3601      	addne	r6, #1
3000af56:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000af58:	05d2      	lsls	r2, r2, #23
3000af5a:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000af5e:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000af62:	9a06      	ldr	r2, [sp, #24]
3000af64:	0112      	lsls	r2, r2, #4
3000af66:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000af6a:	4332      	orrs	r2, r6
3000af6c:	0246      	lsls	r6, r0, #9
3000af6e:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000af72:	4316      	orrs	r6, r2
3000af74:	9a03      	ldr	r2, [sp, #12]
3000af76:	f002 020f 	and.w	r2, r2, #15
3000af7a:	4316      	orrs	r6, r2
3000af7c:	4aa0      	ldr	r2, [pc, #640]	; (3000b200 <rxi316_dram_init+0x69c>)
3000af7e:	4331      	orrs	r1, r6
3000af80:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000af84:	6151      	str	r1, [r2, #20]
3000af86:	68a7      	ldr	r7, [r4, #8]
3000af88:	697a      	ldr	r2, [r7, #20]
3000af8a:	fbb2 fef3 	udiv	lr, r2, r3
3000af8e:	fb03 221e 	mls	r2, r3, lr, r2
3000af92:	b10a      	cbz	r2, 3000af98 <rxi316_dram_init+0x434>
3000af94:	f10e 0e01 	add.w	lr, lr, #1
3000af98:	6939      	ldr	r1, [r7, #16]
3000af9a:	fbb1 f2f3 	udiv	r2, r1, r3
3000af9e:	fb03 1112 	mls	r1, r3, r2, r1
3000afa2:	b101      	cbz	r1, 3000afa6 <rxi316_dram_init+0x442>
3000afa4:	3201      	adds	r2, #1
3000afa6:	68fd      	ldr	r5, [r7, #12]
3000afa8:	fbb5 f1f3 	udiv	r1, r5, r3
3000afac:	fb03 5511 	mls	r5, r3, r1, r5
3000afb0:	b105      	cbz	r5, 3000afb4 <rxi316_dram_init+0x450>
3000afb2:	3101      	adds	r1, #1
3000afb4:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000afb6:	fbb5 f6f3 	udiv	r6, r5, r3
3000afba:	fb03 5516 	mls	r5, r3, r6, r5
3000afbe:	b105      	cbz	r5, 3000afc2 <rxi316_dram_init+0x45e>
3000afc0:	3601      	adds	r6, #1
3000afc2:	68bd      	ldr	r5, [r7, #8]
3000afc4:	4411      	add	r1, r2
3000afc6:	fbb5 f2f3 	udiv	r2, r5, r3
3000afca:	42b1      	cmp	r1, r6
3000afcc:	fb03 5512 	mls	r5, r3, r2, r5
3000afd0:	bf38      	it	cc
3000afd2:	4631      	movcc	r1, r6
3000afd4:	b905      	cbnz	r5, 3000afd8 <rxi316_dram_init+0x474>
3000afd6:	3a01      	subs	r2, #1
3000afd8:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000afda:	ea15 0f0a 	tst.w	r5, sl
3000afde:	fa25 fc0b 	lsr.w	ip, r5, fp
3000afe2:	69fd      	ldr	r5, [r7, #28]
3000afe4:	bf18      	it	ne
3000afe6:	f10c 0c01 	addne.w	ip, ip, #1
3000afea:	fa25 f60b 	lsr.w	r6, r5, fp
3000afee:	ea15 0f0a 	tst.w	r5, sl
3000aff2:	bf18      	it	ne
3000aff4:	3601      	addne	r6, #1
3000aff6:	f1b9 0f09 	cmp.w	r9, #9
3000affa:	f000 81f5 	beq.w	3000b3e8 <rxi316_dram_init+0x884>
3000affe:	f1b9 0f02 	cmp.w	r9, #2
3000b002:	f000 8233 	beq.w	3000b46c <rxi316_dram_init+0x908>
3000b006:	f1b9 0f03 	cmp.w	r9, #3
3000b00a:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b00c:	f000 8298 	beq.w	3000b540 <rxi316_dram_init+0x9dc>
3000b010:	46a8      	mov	r8, r5
3000b012:	9d01      	ldr	r5, [sp, #4]
3000b014:	44a8      	add	r8, r5
3000b016:	fa28 f50b 	lsr.w	r5, r8, fp
3000b01a:	ea18 0f0a 	tst.w	r8, sl
3000b01e:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000b022:	bf18      	it	ne
3000b024:	3501      	addne	r5, #1
3000b026:	fbb8 f7f3 	udiv	r7, r8, r3
3000b02a:	fb03 8817 	mls	r8, r3, r7, r8
3000b02e:	f1b8 0f00 	cmp.w	r8, #0
3000b032:	d000      	beq.n	3000b036 <rxi316_dram_init+0x4d2>
3000b034:	3701      	adds	r7, #1
3000b036:	0109      	lsls	r1, r1, #4
3000b038:	f00e 0e0f 	and.w	lr, lr, #15
3000b03c:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000b040:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000b044:	063f      	lsls	r7, r7, #24
3000b046:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000b04a:	ea41 0e0e 	orr.w	lr, r1, lr
3000b04e:	0291      	lsls	r1, r2, #10
3000b050:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000b054:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000b058:	ea4e 0e01 	orr.w	lr, lr, r1
3000b05c:	0471      	lsls	r1, r6, #17
3000b05e:	ea4e 0c0c 	orr.w	ip, lr, ip
3000b062:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000b066:	ea4c 0c01 	orr.w	ip, ip, r1
3000b06a:	0529      	lsls	r1, r5, #20
3000b06c:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000b070:	ea4c 0c01 	orr.w	ip, ip, r1
3000b074:	4962      	ldr	r1, [pc, #392]	; (3000b200 <rxi316_dram_init+0x69c>)
3000b076:	ea4c 0707 	orr.w	r7, ip, r7
3000b07a:	618f      	str	r7, [r1, #24]
3000b07c:	6821      	ldr	r1, [r4, #0]
3000b07e:	780f      	ldrb	r7, [r1, #0]
3000b080:	2f02      	cmp	r7, #2
3000b082:	f000 8152 	beq.w	3000b32a <rxi316_dram_init+0x7c6>
3000b086:	2f09      	cmp	r7, #9
3000b088:	f000 814f 	beq.w	3000b32a <rxi316_dram_init+0x7c6>
3000b08c:	68e1      	ldr	r1, [r4, #12]
3000b08e:	2f03      	cmp	r7, #3
3000b090:	6809      	ldr	r1, [r1, #0]
3000b092:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000b094:	f001 080f 	and.w	r8, r1, #15
3000b098:	68a1      	ldr	r1, [r4, #8]
3000b09a:	6a09      	ldr	r1, [r1, #32]
3000b09c:	d005      	beq.n	3000b0aa <rxi316_dram_init+0x546>
3000b09e:	ea0a 0701 	and.w	r7, sl, r1
3000b0a2:	fa21 f10b 	lsr.w	r1, r1, fp
3000b0a6:	b107      	cbz	r7, 3000b0aa <rxi316_dram_init+0x546>
3000b0a8:	3101      	adds	r1, #1
3000b0aa:	f1b9 0f02 	cmp.w	r9, #2
3000b0ae:	f000 8154 	beq.w	3000b35a <rxi316_dram_init+0x7f6>
3000b0b2:	f1b9 0f03 	cmp.w	r9, #3
3000b0b6:	f000 81b6 	beq.w	3000b426 <rxi316_dram_init+0x8c2>
3000b0ba:	f1b9 0f09 	cmp.w	r9, #9
3000b0be:	f000 822f 	beq.w	3000b520 <rxi316_dram_init+0x9bc>
3000b0c2:	f04f 0e00 	mov.w	lr, #0
3000b0c6:	46f1      	mov	r9, lr
3000b0c8:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b0cc:	0109      	lsls	r1, r1, #4
3000b0ce:	9f06      	ldr	r7, [sp, #24]
3000b0d0:	b2c9      	uxtb	r1, r1
3000b0d2:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b0d6:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b0d8:	ea41 0108 	orr.w	r1, r1, r8
3000b0dc:	4339      	orrs	r1, r7
3000b0de:	4f48      	ldr	r7, [pc, #288]	; (3000b200 <rxi316_dram_init+0x69c>)
3000b0e0:	ea41 0109 	orr.w	r1, r1, r9
3000b0e4:	ea41 010e 	orr.w	r1, r1, lr
3000b0e8:	61f9      	str	r1, [r7, #28]
3000b0ea:	68a7      	ldr	r7, [r4, #8]
3000b0ec:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b0ee:	fbb4 f1f3 	udiv	r1, r4, r3
3000b0f2:	fb03 4311 	mls	r3, r3, r1, r4
3000b0f6:	b103      	cbz	r3, 3000b0fa <rxi316_dram_init+0x596>
3000b0f8:	3101      	adds	r1, #1
3000b0fa:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b0fc:	094b      	lsrs	r3, r1, #5
3000b0fe:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b102:	0912      	lsrs	r2, r2, #4
3000b104:	ea14 0f0a 	tst.w	r4, sl
3000b108:	fa24 f80b 	lsr.w	r8, r4, fp
3000b10c:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b10e:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b112:	bf18      	it	ne
3000b114:	f108 0801 	addne.w	r8, r8, #1
3000b118:	9306      	str	r3, [sp, #24]
3000b11a:	ea14 0f0a 	tst.w	r4, sl
3000b11e:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b122:	9b01      	ldr	r3, [sp, #4]
3000b124:	fa29 f40b 	lsr.w	r4, r9, fp
3000b128:	bf18      	it	ne
3000b12a:	f10e 0e01 	addne.w	lr, lr, #1
3000b12e:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b130:	ea19 0f0a 	tst.w	r9, sl
3000b134:	fa23 f90b 	lsr.w	r9, r3, fp
3000b138:	9905      	ldr	r1, [sp, #20]
3000b13a:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b13e:	bf18      	it	ne
3000b140:	3401      	addne	r4, #1
3000b142:	45b9      	cmp	r9, r7
3000b144:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b148:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b14c:	f240 80eb 	bls.w	3000b326 <rxi316_dram_init+0x7c2>
3000b150:	eba9 0707 	sub.w	r7, r9, r7
3000b154:	fa27 f40b 	lsr.w	r4, r7, fp
3000b158:	ea17 070a 	ands.w	r7, r7, sl
3000b15c:	d001      	beq.n	3000b162 <rxi316_dram_init+0x5fe>
3000b15e:	3401      	adds	r4, #1
3000b160:	2700      	movs	r7, #0
3000b162:	9b04      	ldr	r3, [sp, #16]
3000b164:	b10b      	cbz	r3, 3000b16a <rxi316_dram_init+0x606>
3000b166:	073b      	lsls	r3, r7, #28
3000b168:	9304      	str	r3, [sp, #16]
3000b16a:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b16e:	9b06      	ldr	r3, [sp, #24]
3000b170:	0289      	lsls	r1, r1, #10
3000b172:	02c0      	lsls	r0, r0, #11
3000b174:	f00c 0c04 	and.w	ip, ip, #4
3000b178:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b17c:	01d2      	lsls	r2, r2, #7
3000b17e:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b182:	0236      	lsls	r6, r6, #8
3000b184:	ea4c 0c01 	orr.w	ip, ip, r1
3000b188:	b2d2      	uxtb	r2, r2
3000b18a:	026d      	lsls	r5, r5, #9
3000b18c:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b190:	ea4c 0c00 	orr.w	ip, ip, r0
3000b194:	00df      	lsls	r7, r3, #3
3000b196:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b19a:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b19e:	ea4c 0202 	orr.w	r2, ip, r2
3000b1a2:	f007 0708 	and.w	r7, r7, #8
3000b1a6:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b1aa:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b1ae:	4332      	orrs	r2, r6
3000b1b0:	0524      	lsls	r4, r4, #20
3000b1b2:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b1b6:	4912      	ldr	r1, [pc, #72]	; (3000b200 <rxi316_dram_init+0x69c>)
3000b1b8:	ea42 0305 	orr.w	r3, r2, r5
3000b1bc:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b1c0:	9a04      	ldr	r2, [sp, #16]
3000b1c2:	433b      	orrs	r3, r7
3000b1c4:	ea43 0308 	orr.w	r3, r3, r8
3000b1c8:	ea43 030e 	orr.w	r3, r3, lr
3000b1cc:	4323      	orrs	r3, r4
3000b1ce:	4313      	orrs	r3, r2
3000b1d0:	9a03      	ldr	r2, [sp, #12]
3000b1d2:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b1d6:	620b      	str	r3, [r1, #32]
3000b1d8:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b1dc:	9b02      	ldr	r3, [sp, #8]
3000b1de:	2b01      	cmp	r3, #1
3000b1e0:	d002      	beq.n	3000b1e8 <rxi316_dram_init+0x684>
3000b1e2:	9b08      	ldr	r3, [sp, #32]
3000b1e4:	3303      	adds	r3, #3
3000b1e6:	9307      	str	r3, [sp, #28]
3000b1e8:	4b05      	ldr	r3, [pc, #20]	; (3000b200 <rxi316_dram_init+0x69c>)
3000b1ea:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b1ee:	9a07      	ldr	r2, [sp, #28]
3000b1f0:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b1f4:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b1f8:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b1fc:	e002      	b.n	3000b204 <rxi316_dram_init+0x6a0>
3000b1fe:	bf00      	nop
3000b200:	41100000 	.word	0x41100000
3000b204:	60da      	str	r2, [r3, #12]
3000b206:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b20a:	430a      	orrs	r2, r1
3000b20c:	49b3      	ldr	r1, [pc, #716]	; (3000b4dc <rxi316_dram_init+0x978>)
3000b20e:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b212:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b216:	0096      	lsls	r6, r2, #2
3000b218:	d57f      	bpl.n	3000b31a <rxi316_dram_init+0x7b6>
3000b21a:	691a      	ldr	r2, [r3, #16]
3000b21c:	4cb0      	ldr	r4, [pc, #704]	; (3000b4e0 <rxi316_dram_init+0x97c>)
3000b21e:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b222:	611a      	str	r2, [r3, #16]
3000b224:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b228:	601a      	str	r2, [r3, #0]
3000b22a:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b22e:	f043 0303 	orr.w	r3, r3, #3
3000b232:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b236:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b23a:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b23c:	2b01      	cmp	r3, #1
3000b23e:	4ba9      	ldr	r3, [pc, #676]	; (3000b4e4 <rxi316_dram_init+0x980>)
3000b240:	bf0c      	ite	eq
3000b242:	4aa9      	ldreq	r2, [pc, #676]	; (3000b4e8 <rxi316_dram_init+0x984>)
3000b244:	4aa9      	ldrne	r2, [pc, #676]	; (3000b4ec <rxi316_dram_init+0x988>)
3000b246:	601a      	str	r2, [r3, #0]
3000b248:	2203      	movs	r2, #3
3000b24a:	68e0      	ldr	r0, [r4, #12]
3000b24c:	4ba8      	ldr	r3, [pc, #672]	; (3000b4f0 <rxi316_dram_init+0x98c>)
3000b24e:	4003      	ands	r3, r0
3000b250:	60e3      	str	r3, [r4, #12]
3000b252:	2308      	movs	r3, #8
3000b254:	62a1      	str	r1, [r4, #40]	; 0x28
3000b256:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b258:	6023      	str	r3, [r4, #0]
3000b25a:	6823      	ldr	r3, [r4, #0]
3000b25c:	071d      	lsls	r5, r3, #28
3000b25e:	d5fc      	bpl.n	3000b25a <rxi316_dram_init+0x6f6>
3000b260:	4ba4      	ldr	r3, [pc, #656]	; (3000b4f4 <rxi316_dram_init+0x990>)
3000b262:	2001      	movs	r0, #1
3000b264:	4798      	blx	r3
3000b266:	68e2      	ldr	r2, [r4, #12]
3000b268:	4ba1      	ldr	r3, [pc, #644]	; (3000b4f0 <rxi316_dram_init+0x98c>)
3000b26a:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b26e:	4013      	ands	r3, r2
3000b270:	2203      	movs	r2, #3
3000b272:	60e3      	str	r3, [r4, #12]
3000b274:	2308      	movs	r3, #8
3000b276:	62a1      	str	r1, [r4, #40]	; 0x28
3000b278:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b27a:	4a99      	ldr	r2, [pc, #612]	; (3000b4e0 <rxi316_dram_init+0x97c>)
3000b27c:	6023      	str	r3, [r4, #0]
3000b27e:	6813      	ldr	r3, [r2, #0]
3000b280:	071c      	lsls	r4, r3, #28
3000b282:	d5fc      	bpl.n	3000b27e <rxi316_dram_init+0x71a>
3000b284:	68d0      	ldr	r0, [r2, #12]
3000b286:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b28a:	4b99      	ldr	r3, [pc, #612]	; (3000b4f0 <rxi316_dram_init+0x98c>)
3000b28c:	4003      	ands	r3, r0
3000b28e:	2008      	movs	r0, #8
3000b290:	60d3      	str	r3, [r2, #12]
3000b292:	6291      	str	r1, [r2, #40]	; 0x28
3000b294:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b296:	4992      	ldr	r1, [pc, #584]	; (3000b4e0 <rxi316_dram_init+0x97c>)
3000b298:	f023 0302 	bic.w	r3, r3, #2
3000b29c:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b29e:	6010      	str	r0, [r2, #0]
3000b2a0:	680b      	ldr	r3, [r1, #0]
3000b2a2:	0718      	lsls	r0, r3, #28
3000b2a4:	d5fc      	bpl.n	3000b2a0 <rxi316_dram_init+0x73c>
3000b2a6:	4a8e      	ldr	r2, [pc, #568]	; (3000b4e0 <rxi316_dram_init+0x97c>)
3000b2a8:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b2ac:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b2b0:	60d3      	str	r3, [r2, #12]
3000b2b2:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b2b4:	6913      	ldr	r3, [r2, #16]
3000b2b6:	4318      	orrs	r0, r3
3000b2b8:	6110      	str	r0, [r2, #16]
3000b2ba:	6913      	ldr	r3, [r2, #16]
3000b2bc:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b2c0:	6113      	str	r3, [r2, #16]
3000b2c2:	6011      	str	r1, [r2, #0]
3000b2c4:	68d3      	ldr	r3, [r2, #12]
3000b2c6:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b2ca:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b2ce:	d1f9      	bne.n	3000b2c4 <rxi316_dram_init+0x760>
3000b2d0:	b00f      	add	sp, #60	; 0x3c
3000b2d2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b2d6:	2a01      	cmp	r2, #1
3000b2d8:	f000 80d1 	beq.w	3000b47e <rxi316_dram_init+0x91a>
3000b2dc:	2a02      	cmp	r2, #2
3000b2de:	f000 8139 	beq.w	3000b554 <rxi316_dram_init+0x9f0>
3000b2e2:	2a03      	cmp	r2, #3
3000b2e4:	f47f ace4 	bne.w	3000acb0 <rxi316_dram_init+0x14c>
3000b2e8:	686a      	ldr	r2, [r5, #4]
3000b2ea:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b2ee:	606a      	str	r2, [r5, #4]
3000b2f0:	e4de      	b.n	3000acb0 <rxi316_dram_init+0x14c>
3000b2f2:	2a00      	cmp	r2, #0
3000b2f4:	f43f ad6a 	beq.w	3000adcc <rxi316_dram_init+0x268>
3000b2f8:	2203      	movs	r2, #3
3000b2fa:	4310      	orrs	r0, r2
3000b2fc:	68a2      	ldr	r2, [r4, #8]
3000b2fe:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b300:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b302:	013f      	lsls	r7, r7, #4
3000b304:	2d00      	cmp	r5, #0
3000b306:	f000 80d7 	beq.w	3000b4b8 <rxi316_dram_init+0x954>
3000b30a:	1e91      	subs	r1, r2, #2
3000b30c:	2906      	cmp	r1, #6
3000b30e:	f200 80b0 	bhi.w	3000b472 <rxi316_dram_init+0x90e>
3000b312:	1e55      	subs	r5, r2, #1
3000b314:	2100      	movs	r1, #0
3000b316:	026d      	lsls	r5, r5, #9
3000b318:	e55f      	b.n	3000adda <rxi316_dram_init+0x276>
3000b31a:	2201      	movs	r2, #1
3000b31c:	601a      	str	r2, [r3, #0]
3000b31e:	681a      	ldr	r2, [r3, #0]
3000b320:	07d2      	lsls	r2, r2, #31
3000b322:	d5fc      	bpl.n	3000b31e <rxi316_dram_init+0x7ba>
3000b324:	e7bf      	b.n	3000b2a6 <rxi316_dram_init+0x742>
3000b326:	2701      	movs	r7, #1
3000b328:	e71b      	b.n	3000b162 <rxi316_dram_init+0x5fe>
3000b32a:	68a1      	ldr	r1, [r4, #8]
3000b32c:	2f03      	cmp	r7, #3
3000b32e:	6a09      	ldr	r1, [r1, #32]
3000b330:	f000 80bf 	beq.w	3000b4b2 <rxi316_dram_init+0x94e>
3000b334:	2f02      	cmp	r7, #2
3000b336:	f040 8118 	bne.w	3000b56a <rxi316_dram_init+0xa06>
3000b33a:	fbb1 fcf3 	udiv	ip, r1, r3
3000b33e:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b342:	4661      	mov	r1, ip
3000b344:	f1be 0f00 	cmp.w	lr, #0
3000b348:	f000 80a9 	beq.w	3000b49e <rxi316_dram_init+0x93a>
3000b34c:	f1b9 0f02 	cmp.w	r9, #2
3000b350:	f101 0101 	add.w	r1, r1, #1
3000b354:	46b8      	mov	r8, r7
3000b356:	f47f aeac 	bne.w	3000b0b2 <rxi316_dram_init+0x54e>
3000b35a:	4f67      	ldr	r7, [pc, #412]	; (3000b4f8 <rxi316_dram_init+0x994>)
3000b35c:	f04f 0e00 	mov.w	lr, #0
3000b360:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b4fc <rxi316_dram_init+0x998>
3000b364:	fbb7 f7f3 	udiv	r7, r7, r3
3000b368:	fbb9 f9f3 	udiv	r9, r9, r3
3000b36c:	09ff      	lsrs	r7, r7, #7
3000b36e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b372:	3701      	adds	r7, #1
3000b374:	f109 0c01 	add.w	ip, r9, #1
3000b378:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b500 <rxi316_dram_init+0x99c>
3000b37c:	023f      	lsls	r7, r7, #8
3000b37e:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b382:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b386:	9709      	str	r7, [sp, #36]	; 0x24
3000b388:	e6a0      	b.n	3000b0cc <rxi316_dram_init+0x568>
3000b38a:	f8cd c018 	str.w	ip, [sp, #24]
3000b38e:	e5bf      	b.n	3000af10 <rxi316_dram_init+0x3ac>
3000b390:	38c8      	subs	r0, #200	; 0xc8
3000b392:	e596      	b.n	3000aec2 <rxi316_dram_init+0x35e>
3000b394:	9105      	str	r1, [sp, #20]
3000b396:	e58b      	b.n	3000aeb0 <rxi316_dram_init+0x34c>
3000b398:	fa22 f00b 	lsr.w	r0, r2, fp
3000b39c:	e547      	b.n	3000ae2e <rxi316_dram_init+0x2ca>
3000b39e:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b3a2:	ea1a 0f07 	tst.w	sl, r7
3000b3a6:	fa27 f20b 	lsr.w	r2, r7, fp
3000b3aa:	d000      	beq.n	3000b3ae <rxi316_dram_init+0x84a>
3000b3ac:	3201      	adds	r2, #1
3000b3ae:	2e02      	cmp	r6, #2
3000b3b0:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b3b4:	d10f      	bne.n	3000b3d6 <rxi316_dram_init+0x872>
3000b3b6:	9e08      	ldr	r6, [sp, #32]
3000b3b8:	3e02      	subs	r6, #2
3000b3ba:	fa26 f60b 	lsr.w	r6, r6, fp
3000b3be:	4437      	add	r7, r6
3000b3c0:	fbb7 f6f3 	udiv	r6, r7, r3
3000b3c4:	fb03 7716 	mls	r7, r3, r6, r7
3000b3c8:	2f00      	cmp	r7, #0
3000b3ca:	f43f ad52 	beq.w	3000ae72 <rxi316_dram_init+0x30e>
3000b3ce:	3601      	adds	r6, #1
3000b3d0:	e54f      	b.n	3000ae72 <rxi316_dram_init+0x30e>
3000b3d2:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b3d6:	fbb7 f6f3 	udiv	r6, r7, r3
3000b3da:	fb03 7716 	mls	r7, r3, r6, r7
3000b3de:	2f00      	cmp	r7, #0
3000b3e0:	f43f ad47 	beq.w	3000ae72 <rxi316_dram_init+0x30e>
3000b3e4:	3601      	adds	r6, #1
3000b3e6:	e544      	b.n	3000ae72 <rxi316_dram_init+0x30e>
3000b3e8:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b3ea:	46a8      	mov	r8, r5
3000b3ec:	9d08      	ldr	r5, [sp, #32]
3000b3ee:	44a8      	add	r8, r5
3000b3f0:	e611      	b.n	3000b016 <rxi316_dram_init+0x4b2>
3000b3f2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b3f4:	1f50      	subs	r0, r2, #5
3000b3f6:	00c0      	lsls	r0, r0, #3
3000b3f8:	68e2      	ldr	r2, [r4, #12]
3000b3fa:	6812      	ldr	r2, [r2, #0]
3000b3fc:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b3fe:	fab2 f282 	clz	r2, r2
3000b402:	0952      	lsrs	r2, r2, #5
3000b404:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b408:	4301      	orrs	r1, r0
3000b40a:	e4fd      	b.n	3000ae08 <rxi316_dram_init+0x2a4>
3000b40c:	f1a1 0502 	sub.w	r5, r1, #2
3000b410:	2909      	cmp	r1, #9
3000b412:	fab5 f585 	clz	r5, r5
3000b416:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b41a:	f000 8099 	beq.w	3000b550 <rxi316_dram_init+0x9ec>
3000b41e:	462a      	mov	r2, r5
3000b420:	e4d1      	b.n	3000adc6 <rxi316_dram_init+0x262>
3000b422:	2000      	movs	r0, #0
3000b424:	e7e8      	b.n	3000b3f8 <rxi316_dram_init+0x894>
3000b426:	f242 7710 	movw	r7, #10000	; 0x2710
3000b42a:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b504 <rxi316_dram_init+0x9a0>
3000b42e:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b4fc <rxi316_dram_init+0x998>
3000b432:	fbb7 fcf3 	udiv	ip, r7, r3
3000b436:	9f05      	ldr	r7, [sp, #20]
3000b438:	4467      	add	r7, ip
3000b43a:	fbb9 f9f3 	udiv	r9, r9, r3
3000b43e:	fbbe fef3 	udiv	lr, lr, r3
3000b442:	09ff      	lsrs	r7, r7, #7
3000b444:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b448:	3701      	adds	r7, #1
3000b44a:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b44e:	f109 0c01 	add.w	ip, r9, #1
3000b452:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b500 <rxi316_dram_init+0x99c>
3000b456:	023f      	lsls	r7, r7, #8
3000b458:	f10e 0e01 	add.w	lr, lr, #1
3000b45c:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b460:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b464:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b468:	9709      	str	r7, [sp, #36]	; 0x24
3000b46a:	e62f      	b.n	3000b0cc <rxi316_dram_init+0x568>
3000b46c:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b470:	e5d1      	b.n	3000b016 <rxi316_dram_init+0x4b2>
3000b472:	2100      	movs	r1, #0
3000b474:	460d      	mov	r5, r1
3000b476:	e4b0      	b.n	3000adda <rxi316_dram_init+0x276>
3000b478:	3a07      	subs	r2, #7
3000b47a:	f7ff bbb8 	b.w	3000abee <rxi316_dram_init+0x8a>
3000b47e:	686a      	ldr	r2, [r5, #4]
3000b480:	f042 0220 	orr.w	r2, r2, #32
3000b484:	606a      	str	r2, [r5, #4]
3000b486:	e413      	b.n	3000acb0 <rxi316_dram_init+0x14c>
3000b488:	2204      	movs	r2, #4
3000b48a:	2102      	movs	r1, #2
3000b48c:	2500      	movs	r5, #0
3000b48e:	9201      	str	r2, [sp, #4]
3000b490:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b494:	f7ff bb94 	b.w	3000abc0 <rxi316_dram_init+0x5c>
3000b498:	3a08      	subs	r2, #8
3000b49a:	f7ff bba8 	b.w	3000abee <rxi316_dram_init+0x8a>
3000b49e:	46b8      	mov	r8, r7
3000b4a0:	e603      	b.n	3000b0aa <rxi316_dram_init+0x546>
3000b4a2:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b4a4:	2a04      	cmp	r2, #4
3000b4a6:	d90c      	bls.n	3000b4c2 <rxi316_dram_init+0x95e>
3000b4a8:	2a0b      	cmp	r2, #11
3000b4aa:	d86a      	bhi.n	3000b582 <rxi316_dram_init+0xa1e>
3000b4ac:	1f17      	subs	r7, r2, #4
3000b4ae:	013f      	lsls	r7, r7, #4
3000b4b0:	e008      	b.n	3000b4c4 <rxi316_dram_init+0x960>
3000b4b2:	f04f 0802 	mov.w	r8, #2
3000b4b6:	e5f8      	b.n	3000b0aa <rxi316_dram_init+0x546>
3000b4b8:	2903      	cmp	r1, #3
3000b4ba:	d005      	beq.n	3000b4c8 <rxi316_dram_init+0x964>
3000b4bc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4c0:	e48b      	b.n	3000adda <rxi316_dram_init+0x276>
3000b4c2:	2710      	movs	r7, #16
3000b4c4:	68a2      	ldr	r2, [r4, #8]
3000b4c6:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b4c8:	2a04      	cmp	r2, #4
3000b4ca:	d949      	bls.n	3000b560 <rxi316_dram_init+0x9fc>
3000b4cc:	1f51      	subs	r1, r2, #5
3000b4ce:	2903      	cmp	r1, #3
3000b4d0:	d84e      	bhi.n	3000b570 <rxi316_dram_init+0xa0c>
3000b4d2:	1f15      	subs	r5, r2, #4
3000b4d4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4d8:	026d      	lsls	r5, r5, #9
3000b4da:	e47e      	b.n	3000adda <rxi316_dram_init+0x276>
3000b4dc:	42008000 	.word	0x42008000
3000b4e0:	41100000 	.word	0x41100000
3000b4e4:	41011000 	.word	0x41011000
3000b4e8:	44000103 	.word	0x44000103
3000b4ec:	44000021 	.word	0x44000021
3000b4f0:	fff8fffc 	.word	0xfff8fffc
3000b4f4:	00009b2d 	.word	0x00009b2d
3000b4f8:	00061a80 	.word	0x00061a80
3000b4fc:	0bebc200 	.word	0x0bebc200
3000b500:	003ff000 	.word	0x003ff000
3000b504:	1dcd6500 	.word	0x1dcd6500
3000b508:	f1a1 0c02 	sub.w	ip, r1, #2
3000b50c:	f1bc 0f01 	cmp.w	ip, #1
3000b510:	d903      	bls.n	3000b51a <rxi316_dram_init+0x9b6>
3000b512:	f1be 0f00 	cmp.w	lr, #0
3000b516:	f43f ac56 	beq.w	3000adc6 <rxi316_dram_init+0x262>
3000b51a:	f040 0002 	orr.w	r0, r0, #2
3000b51e:	e452      	b.n	3000adc6 <rxi316_dram_init+0x262>
3000b520:	4f1d      	ldr	r7, [pc, #116]	; (3000b598 <rxi316_dram_init+0xa34>)
3000b522:	f04f 0e00 	mov.w	lr, #0
3000b526:	fbb7 f9f3 	udiv	r9, r7, r3
3000b52a:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b52e:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b532:	f109 0701 	add.w	r7, r9, #1
3000b536:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b59c <rxi316_dram_init+0xa38>
3000b53a:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b53e:	e5c5      	b.n	3000b0cc <rxi316_dram_init+0x568>
3000b540:	f105 0802 	add.w	r8, r5, #2
3000b544:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b546:	eba8 0805 	sub.w	r8, r8, r5
3000b54a:	9d08      	ldr	r5, [sp, #32]
3000b54c:	44a8      	add	r8, r5
3000b54e:	e562      	b.n	3000b016 <rxi316_dram_init+0x4b2>
3000b550:	2204      	movs	r2, #4
3000b552:	e6d2      	b.n	3000b2fa <rxi316_dram_init+0x796>
3000b554:	686a      	ldr	r2, [r5, #4]
3000b556:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b55a:	606a      	str	r2, [r5, #4]
3000b55c:	f7ff bba8 	b.w	3000acb0 <rxi316_dram_init+0x14c>
3000b560:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b564:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b568:	e437      	b.n	3000adda <rxi316_dram_init+0x276>
3000b56a:	f04f 0802 	mov.w	r8, #2
3000b56e:	e596      	b.n	3000b09e <rxi316_dram_init+0x53a>
3000b570:	f1a2 0109 	sub.w	r1, r2, #9
3000b574:	2905      	cmp	r1, #5
3000b576:	d80b      	bhi.n	3000b590 <rxi316_dram_init+0xa2c>
3000b578:	0855      	lsrs	r5, r2, #1
3000b57a:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b57e:	026d      	lsls	r5, r5, #9
3000b580:	e42b      	b.n	3000adda <rxi316_dram_init+0x276>
3000b582:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b584:	f040 0004 	orr.w	r0, r0, #4
3000b588:	f1a2 070c 	sub.w	r7, r2, #12
3000b58c:	013f      	lsls	r7, r7, #4
3000b58e:	e799      	b.n	3000b4c4 <rxi316_dram_init+0x960>
3000b590:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b594:	2500      	movs	r5, #0
3000b596:	e420      	b.n	3000adda <rxi316_dram_init+0x276>
3000b598:	0bebc200 	.word	0x0bebc200
3000b59c:	003ff000 	.word	0x003ff000

3000b5a0 <rxi316_ftend_init>:
3000b5a0:	2300      	movs	r3, #0
3000b5a2:	b510      	push	{r4, lr}
3000b5a4:	4c0c      	ldr	r4, [pc, #48]	; (3000b5d8 <rxi316_ftend_init+0x38>)
3000b5a6:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b5aa:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b5ae:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b5b2:	f7fe fe67 	bl	3000a284 <ddr_init_index>
3000b5b6:	4a09      	ldr	r2, [pc, #36]	; (3000b5dc <rxi316_ftend_init+0x3c>)
3000b5b8:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b5bc:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b5c0:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b5c4:	68cb      	ldr	r3, [r1, #12]
3000b5c6:	689b      	ldr	r3, [r3, #8]
3000b5c8:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b5ca:	3301      	adds	r3, #1
3000b5cc:	b2db      	uxtb	r3, r3
3000b5ce:	4313      	orrs	r3, r2
3000b5d0:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b5d4:	bd10      	pop	{r4, pc}
3000b5d6:	bf00      	nop
3000b5d8:	41101000 	.word	0x41101000
3000b5dc:	3000fa10 	.word	0x3000fa10

3000b5e0 <rxi316_DynSre_init>:
3000b5e0:	2901      	cmp	r1, #1
3000b5e2:	d013      	beq.n	3000b60c <rxi316_DynSre_init+0x2c>
3000b5e4:	4a12      	ldr	r2, [pc, #72]	; (3000b630 <rxi316_DynSre_init+0x50>)
3000b5e6:	6893      	ldr	r3, [r2, #8]
3000b5e8:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b5ec:	6093      	str	r3, [r2, #8]
3000b5ee:	4b10      	ldr	r3, [pc, #64]	; (3000b630 <rxi316_DynSre_init+0x50>)
3000b5f0:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b5f4:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b5f8:	625a      	str	r2, [r3, #36]	; 0x24
3000b5fa:	629a      	str	r2, [r3, #40]	; 0x28
3000b5fc:	69da      	ldr	r2, [r3, #28]
3000b5fe:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b602:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b606:	61da      	str	r2, [r3, #28]
3000b608:	6019      	str	r1, [r3, #0]
3000b60a:	4770      	bx	lr
3000b60c:	4b08      	ldr	r3, [pc, #32]	; (3000b630 <rxi316_DynSre_init+0x50>)
3000b60e:	b280      	uxth	r0, r0
3000b610:	4a08      	ldr	r2, [pc, #32]	; (3000b634 <rxi316_DynSre_init+0x54>)
3000b612:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b616:	400a      	ands	r2, r1
3000b618:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b61c:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b620:	4310      	orrs	r0, r2
3000b622:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b626:	689a      	ldr	r2, [r3, #8]
3000b628:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b62c:	609a      	str	r2, [r3, #8]
3000b62e:	e7de      	b.n	3000b5ee <rxi316_DynSre_init+0xe>
3000b630:	41100000 	.word	0x41100000
3000b634:	ffff0000 	.word	0xffff0000

3000b638 <ddr_init>:
3000b638:	b500      	push	{lr}
3000b63a:	b083      	sub	sp, #12
3000b63c:	f7fe fe22 	bl	3000a284 <ddr_init_index>
3000b640:	4b06      	ldr	r3, [pc, #24]	; (3000b65c <ddr_init+0x24>)
3000b642:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b646:	9001      	str	r0, [sp, #4]
3000b648:	f7fe fe46 	bl	3000a2d8 <rxi316_perf_tune>
3000b64c:	9801      	ldr	r0, [sp, #4]
3000b64e:	f7ff fa89 	bl	3000ab64 <rxi316_dram_init>
3000b652:	b003      	add	sp, #12
3000b654:	f85d eb04 	ldr.w	lr, [sp], #4
3000b658:	f7ff bfa2 	b.w	3000b5a0 <rxi316_ftend_init>
3000b65c:	3000fa10 	.word	0x3000fa10

3000b660 <SWR_MEM>:
3000b660:	2801      	cmp	r0, #1
3000b662:	d00e      	beq.n	3000b682 <SWR_MEM+0x22>
3000b664:	4b31      	ldr	r3, [pc, #196]	; (3000b72c <SWR_MEM+0xcc>)
3000b666:	2000      	movs	r0, #0
3000b668:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b66c:	f022 0201 	bic.w	r2, r2, #1
3000b670:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b674:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b678:	f022 0202 	bic.w	r2, r2, #2
3000b67c:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b680:	4770      	bx	lr
3000b682:	b510      	push	{r4, lr}
3000b684:	4c29      	ldr	r4, [pc, #164]	; (3000b72c <SWR_MEM+0xcc>)
3000b686:	4a2a      	ldr	r2, [pc, #168]	; (3000b730 <SWR_MEM+0xd0>)
3000b688:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b68c:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b690:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b694:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b698:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b69c:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b6a0:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6a4:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b6a8:	f023 0303 	bic.w	r3, r3, #3
3000b6ac:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b6b0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6b4:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b6b8:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6bc:	4790      	blx	r2
3000b6be:	2803      	cmp	r0, #3
3000b6c0:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b6c4:	d026      	beq.n	3000b714 <SWR_MEM+0xb4>
3000b6c6:	f043 0303 	orr.w	r3, r3, #3
3000b6ca:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b6ce:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6d2:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b6d6:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6da:	4c14      	ldr	r4, [pc, #80]	; (3000b72c <SWR_MEM+0xcc>)
3000b6dc:	4b15      	ldr	r3, [pc, #84]	; (3000b734 <SWR_MEM+0xd4>)
3000b6de:	4798      	blx	r3
3000b6e0:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b6e4:	4a14      	ldr	r2, [pc, #80]	; (3000b738 <SWR_MEM+0xd8>)
3000b6e6:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b6ea:	f043 0302 	orr.w	r3, r3, #2
3000b6ee:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b6f2:	4790      	blx	r2
3000b6f4:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b6f8:	4b10      	ldr	r3, [pc, #64]	; (3000b73c <SWR_MEM+0xdc>)
3000b6fa:	f042 0201 	orr.w	r2, r2, #1
3000b6fe:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b702:	e001      	b.n	3000b708 <SWR_MEM+0xa8>
3000b704:	3b01      	subs	r3, #1
3000b706:	d00e      	beq.n	3000b726 <SWR_MEM+0xc6>
3000b708:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b70c:	03d2      	lsls	r2, r2, #15
3000b70e:	d5f9      	bpl.n	3000b704 <SWR_MEM+0xa4>
3000b710:	2000      	movs	r0, #0
3000b712:	bd10      	pop	{r4, pc}
3000b714:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b718:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b71c:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b720:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b724:	e7d9      	b.n	3000b6da <SWR_MEM+0x7a>
3000b726:	2001      	movs	r0, #1
3000b728:	bd10      	pop	{r4, pc}
3000b72a:	bf00      	nop
3000b72c:	42008800 	.word	0x42008800
3000b730:	30008021 	.word	0x30008021
3000b734:	3000b919 	.word	0x3000b919
3000b738:	00009b2d 	.word	0x00009b2d
3000b73c:	000186a0 	.word	0x000186a0

3000b740 <SWR_MEM_Manual>:
3000b740:	4a06      	ldr	r2, [pc, #24]	; (3000b75c <SWR_MEM_Manual+0x1c>)
3000b742:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b746:	b120      	cbz	r0, 3000b752 <SWR_MEM_Manual+0x12>
3000b748:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b74c:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b750:	4770      	bx	lr
3000b752:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b756:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b75a:	4770      	bx	lr
3000b75c:	42008800 	.word	0x42008800

3000b760 <SWR_MEM_Mode_Set>:
3000b760:	2801      	cmp	r0, #1
3000b762:	b538      	push	{r3, r4, r5, lr}
3000b764:	d807      	bhi.n	3000b776 <SWR_MEM_Mode_Set+0x16>
3000b766:	d10a      	bne.n	3000b77e <SWR_MEM_Mode_Set+0x1e>
3000b768:	4c22      	ldr	r4, [pc, #136]	; (3000b7f4 <SWR_MEM_Mode_Set+0x94>)
3000b76a:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b76e:	035b      	lsls	r3, r3, #13
3000b770:	d51b      	bpl.n	3000b7aa <SWR_MEM_Mode_Set+0x4a>
3000b772:	2000      	movs	r0, #0
3000b774:	bd38      	pop	{r3, r4, r5, pc}
3000b776:	217e      	movs	r1, #126	; 0x7e
3000b778:	481f      	ldr	r0, [pc, #124]	; (3000b7f8 <SWR_MEM_Mode_Set+0x98>)
3000b77a:	f000 ffc1 	bl	3000c700 <__io_assert_failed_veneer>
3000b77e:	491d      	ldr	r1, [pc, #116]	; (3000b7f4 <SWR_MEM_Mode_Set+0x94>)
3000b780:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b784:	0358      	lsls	r0, r3, #13
3000b786:	d5f4      	bpl.n	3000b772 <SWR_MEM_Mode_Set+0x12>
3000b788:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b78c:	f242 7310 	movw	r3, #10000	; 0x2710
3000b790:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b794:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b798:	e001      	b.n	3000b79e <SWR_MEM_Mode_Set+0x3e>
3000b79a:	3b01      	subs	r3, #1
3000b79c:	d027      	beq.n	3000b7ee <SWR_MEM_Mode_Set+0x8e>
3000b79e:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b7a2:	0352      	lsls	r2, r2, #13
3000b7a4:	d4f9      	bmi.n	3000b79a <SWR_MEM_Mode_Set+0x3a>
3000b7a6:	2000      	movs	r0, #0
3000b7a8:	e7e4      	b.n	3000b774 <SWR_MEM_Mode_Set+0x14>
3000b7aa:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b7ae:	2064      	movs	r0, #100	; 0x64
3000b7b0:	4d12      	ldr	r5, [pc, #72]	; (3000b7fc <SWR_MEM_Mode_Set+0x9c>)
3000b7b2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b7b6:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b7ba:	47a8      	blx	r5
3000b7bc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b7c0:	2064      	movs	r0, #100	; 0x64
3000b7c2:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b7c6:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b7ca:	47a8      	blx	r5
3000b7cc:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b7d0:	f242 7310 	movw	r3, #10000	; 0x2710
3000b7d4:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b7d8:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b7dc:	e001      	b.n	3000b7e2 <SWR_MEM_Mode_Set+0x82>
3000b7de:	3b01      	subs	r3, #1
3000b7e0:	d005      	beq.n	3000b7ee <SWR_MEM_Mode_Set+0x8e>
3000b7e2:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b7e6:	0355      	lsls	r5, r2, #13
3000b7e8:	d5f9      	bpl.n	3000b7de <SWR_MEM_Mode_Set+0x7e>
3000b7ea:	2000      	movs	r0, #0
3000b7ec:	e7c2      	b.n	3000b774 <SWR_MEM_Mode_Set+0x14>
3000b7ee:	2001      	movs	r0, #1
3000b7f0:	bd38      	pop	{r3, r4, r5, pc}
3000b7f2:	bf00      	nop
3000b7f4:	42008800 	.word	0x42008800
3000b7f8:	3000de14 	.word	0x3000de14
3000b7fc:	00009b2d 	.word	0x00009b2d

3000b800 <SWR_AUDIO>:
3000b800:	2801      	cmp	r0, #1
3000b802:	d00e      	beq.n	3000b822 <SWR_AUDIO+0x22>
3000b804:	4b1e      	ldr	r3, [pc, #120]	; (3000b880 <SWR_AUDIO+0x80>)
3000b806:	2000      	movs	r0, #0
3000b808:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b80c:	f022 0201 	bic.w	r2, r2, #1
3000b810:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b814:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b818:	f022 0202 	bic.w	r2, r2, #2
3000b81c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b820:	4770      	bx	lr
3000b822:	b510      	push	{r4, lr}
3000b824:	4c16      	ldr	r4, [pc, #88]	; (3000b880 <SWR_AUDIO+0x80>)
3000b826:	4b17      	ldr	r3, [pc, #92]	; (3000b884 <SWR_AUDIO+0x84>)
3000b828:	4798      	blx	r3
3000b82a:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b82e:	4a16      	ldr	r2, [pc, #88]	; (3000b888 <SWR_AUDIO+0x88>)
3000b830:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b834:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b838:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b83c:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b840:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b844:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b848:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b84c:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b850:	f043 0302 	orr.w	r3, r3, #2
3000b854:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b858:	4790      	blx	r2
3000b85a:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b85e:	4b0b      	ldr	r3, [pc, #44]	; (3000b88c <SWR_AUDIO+0x8c>)
3000b860:	f042 0201 	orr.w	r2, r2, #1
3000b864:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b868:	e001      	b.n	3000b86e <SWR_AUDIO+0x6e>
3000b86a:	3b01      	subs	r3, #1
3000b86c:	d005      	beq.n	3000b87a <SWR_AUDIO+0x7a>
3000b86e:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b872:	03d2      	lsls	r2, r2, #15
3000b874:	d5f9      	bpl.n	3000b86a <SWR_AUDIO+0x6a>
3000b876:	2000      	movs	r0, #0
3000b878:	bd10      	pop	{r4, pc}
3000b87a:	2001      	movs	r0, #1
3000b87c:	bd10      	pop	{r4, pc}
3000b87e:	bf00      	nop
3000b880:	42008800 	.word	0x42008800
3000b884:	3000ba75 	.word	0x3000ba75
3000b888:	00009b2d 	.word	0x00009b2d
3000b88c:	000186a0 	.word	0x000186a0

3000b890 <SWR_AUDIO_Manual>:
3000b890:	4a06      	ldr	r2, [pc, #24]	; (3000b8ac <SWR_AUDIO_Manual+0x1c>)
3000b892:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b896:	b120      	cbz	r0, 3000b8a2 <SWR_AUDIO_Manual+0x12>
3000b898:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b89c:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b8a0:	4770      	bx	lr
3000b8a2:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b8a6:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b8aa:	4770      	bx	lr
3000b8ac:	42008800 	.word	0x42008800

3000b8b0 <SWR_BST_MODE_Set>:
3000b8b0:	4a06      	ldr	r2, [pc, #24]	; (3000b8cc <SWR_BST_MODE_Set+0x1c>)
3000b8b2:	6813      	ldr	r3, [r2, #0]
3000b8b4:	b118      	cbz	r0, 3000b8be <SWR_BST_MODE_Set+0xe>
3000b8b6:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b8ba:	6013      	str	r3, [r2, #0]
3000b8bc:	4770      	bx	lr
3000b8be:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b8c2:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b8c6:	6013      	str	r3, [r2, #0]
3000b8c8:	4770      	bx	lr
3000b8ca:	bf00      	nop
3000b8cc:	42008100 	.word	0x42008100

3000b8d0 <SWR_Calib_DCore>:
3000b8d0:	4b0f      	ldr	r3, [pc, #60]	; (3000b910 <SWR_Calib_DCore+0x40>)
3000b8d2:	f240 3231 	movw	r2, #817	; 0x331
3000b8d6:	490f      	ldr	r1, [pc, #60]	; (3000b914 <SWR_Calib_DCore+0x44>)
3000b8d8:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b8da:	4001      	ands	r1, r0
3000b8dc:	430a      	orrs	r2, r1
3000b8de:	651a      	str	r2, [r3, #80]	; 0x50
3000b8e0:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b8e2:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b8e6:	641a      	str	r2, [r3, #64]	; 0x40
3000b8e8:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b8ec:	f042 0201 	orr.w	r2, r2, #1
3000b8f0:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b8f4:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b8f8:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b8fc:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b900:	685a      	ldr	r2, [r3, #4]
3000b902:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b906:	f042 0204 	orr.w	r2, r2, #4
3000b90a:	605a      	str	r2, [r3, #4]
3000b90c:	4770      	bx	lr
3000b90e:	bf00      	nop
3000b910:	42008100 	.word	0x42008100
3000b914:	fffffccc 	.word	0xfffffccc

3000b918 <SWR_Calib_MEM>:
3000b918:	4b51      	ldr	r3, [pc, #324]	; (3000ba60 <SWR_Calib_MEM+0x148>)
3000b91a:	f240 3231 	movw	r2, #817	; 0x331
3000b91e:	4951      	ldr	r1, [pc, #324]	; (3000ba64 <SWR_Calib_MEM+0x14c>)
3000b920:	f04f 0c00 	mov.w	ip, #0
3000b924:	b510      	push	{r4, lr}
3000b926:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b92a:	b082      	sub	sp, #8
3000b92c:	4001      	ands	r1, r0
3000b92e:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b932:	430a      	orrs	r2, r1
3000b934:	f10d 0107 	add.w	r1, sp, #7
3000b938:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b93c:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b940:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b944:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b948:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b94c:	f042 0201 	orr.w	r2, r2, #1
3000b950:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b954:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b958:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b95c:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b960:	f88d c007 	strb.w	ip, [sp, #7]
3000b964:	f7fd faf4 	bl	30008f50 <OTP_Read8>
3000b968:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b96c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b970:	2af0      	cmp	r2, #240	; 0xf0
3000b972:	d156      	bne.n	3000ba22 <SWR_Calib_MEM+0x10a>
3000b974:	f003 030f 	and.w	r3, r3, #15
3000b978:	2b0f      	cmp	r3, #15
3000b97a:	d04e      	beq.n	3000ba1a <SWR_Calib_MEM+0x102>
3000b97c:	2302      	movs	r3, #2
3000b97e:	4c3a      	ldr	r4, [pc, #232]	; (3000ba68 <SWR_Calib_MEM+0x150>)
3000b980:	f10d 0106 	add.w	r1, sp, #6
3000b984:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b988:	7023      	strb	r3, [r4, #0]
3000b98a:	f7fd fae1 	bl	30008f50 <OTP_Read8>
3000b98e:	7822      	ldrb	r2, [r4, #0]
3000b990:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b994:	2a03      	cmp	r2, #3
3000b996:	d05a      	beq.n	3000ba4e <SWR_Calib_MEM+0x136>
3000b998:	2a02      	cmp	r2, #2
3000b99a:	bf0c      	ite	eq
3000b99c:	f003 030f 	andeq.w	r3, r3, #15
3000b9a0:	2300      	movne	r3, #0
3000b9a2:	492f      	ldr	r1, [pc, #188]	; (3000ba60 <SWR_Calib_MEM+0x148>)
3000b9a4:	4831      	ldr	r0, [pc, #196]	; (3000ba6c <SWR_Calib_MEM+0x154>)
3000b9a6:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b9aa:	f022 020f 	bic.w	r2, r2, #15
3000b9ae:	4313      	orrs	r3, r2
3000b9b0:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b9b4:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b9b6:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b9ba:	2b07      	cmp	r3, #7
3000b9bc:	d03a      	beq.n	3000ba34 <SWR_Calib_MEM+0x11c>
3000b9be:	2300      	movs	r3, #0
3000b9c0:	f10d 0107 	add.w	r1, sp, #7
3000b9c4:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b9c8:	f88d 3007 	strb.w	r3, [sp, #7]
3000b9cc:	f7fd fac0 	bl	30008f50 <OTP_Read8>
3000b9d0:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b9d4:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b9d8:	2af0      	cmp	r2, #240	; 0xf0
3000b9da:	d124      	bne.n	3000ba26 <SWR_Calib_MEM+0x10e>
3000b9dc:	f003 030f 	and.w	r3, r3, #15
3000b9e0:	2b0f      	cmp	r3, #15
3000b9e2:	d022      	beq.n	3000ba2a <SWR_Calib_MEM+0x112>
3000b9e4:	2302      	movs	r3, #2
3000b9e6:	4c22      	ldr	r4, [pc, #136]	; (3000ba70 <SWR_Calib_MEM+0x158>)
3000b9e8:	f10d 0107 	add.w	r1, sp, #7
3000b9ec:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b9f0:	7023      	strb	r3, [r4, #0]
3000b9f2:	f7fd faad 	bl	30008f50 <OTP_Read8>
3000b9f6:	7822      	ldrb	r2, [r4, #0]
3000b9f8:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b9fc:	2a03      	cmp	r2, #3
3000b9fe:	d028      	beq.n	3000ba52 <SWR_Calib_MEM+0x13a>
3000ba00:	2a02      	cmp	r2, #2
3000ba02:	d029      	beq.n	3000ba58 <SWR_Calib_MEM+0x140>
3000ba04:	2200      	movs	r2, #0
3000ba06:	4916      	ldr	r1, [pc, #88]	; (3000ba60 <SWR_Calib_MEM+0x148>)
3000ba08:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000ba0c:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ba10:	4313      	orrs	r3, r2
3000ba12:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000ba16:	b002      	add	sp, #8
3000ba18:	bd10      	pop	{r4, pc}
3000ba1a:	4b13      	ldr	r3, [pc, #76]	; (3000ba68 <SWR_Calib_MEM+0x150>)
3000ba1c:	2201      	movs	r2, #1
3000ba1e:	701a      	strb	r2, [r3, #0]
3000ba20:	e7cd      	b.n	3000b9be <SWR_Calib_MEM+0xa6>
3000ba22:	2303      	movs	r3, #3
3000ba24:	e7ab      	b.n	3000b97e <SWR_Calib_MEM+0x66>
3000ba26:	2303      	movs	r3, #3
3000ba28:	e7dd      	b.n	3000b9e6 <SWR_Calib_MEM+0xce>
3000ba2a:	4b11      	ldr	r3, [pc, #68]	; (3000ba70 <SWR_Calib_MEM+0x158>)
3000ba2c:	2201      	movs	r2, #1
3000ba2e:	701a      	strb	r2, [r3, #0]
3000ba30:	b002      	add	sp, #8
3000ba32:	bd10      	pop	{r4, pc}
3000ba34:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000ba36:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000ba3a:	d1c0      	bne.n	3000b9be <SWR_Calib_MEM+0xa6>
3000ba3c:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000ba40:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000ba44:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000ba48:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000ba4c:	e7b7      	b.n	3000b9be <SWR_Calib_MEM+0xa6>
3000ba4e:	091b      	lsrs	r3, r3, #4
3000ba50:	e7a7      	b.n	3000b9a2 <SWR_Calib_MEM+0x8a>
3000ba52:	091b      	lsrs	r3, r3, #4
3000ba54:	059a      	lsls	r2, r3, #22
3000ba56:	e7d6      	b.n	3000ba06 <SWR_Calib_MEM+0xee>
3000ba58:	f003 030f 	and.w	r3, r3, #15
3000ba5c:	059a      	lsls	r2, r3, #22
3000ba5e:	e7d2      	b.n	3000ba06 <SWR_Calib_MEM+0xee>
3000ba60:	42008800 	.word	0x42008800
3000ba64:	fffffccc 	.word	0xfffffccc
3000ba68:	3000fbab 	.word	0x3000fbab
3000ba6c:	42008100 	.word	0x42008100
3000ba70:	3000fbaa 	.word	0x3000fbaa

3000ba74 <SWR_Calib_AUD>:
3000ba74:	4b48      	ldr	r3, [pc, #288]	; (3000bb98 <SWR_Calib_AUD+0x124>)
3000ba76:	f240 3231 	movw	r2, #817	; 0x331
3000ba7a:	4948      	ldr	r1, [pc, #288]	; (3000bb9c <SWR_Calib_AUD+0x128>)
3000ba7c:	f04f 0c00 	mov.w	ip, #0
3000ba80:	b510      	push	{r4, lr}
3000ba82:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000ba86:	b082      	sub	sp, #8
3000ba88:	4001      	ands	r1, r0
3000ba8a:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000ba8e:	430a      	orrs	r2, r1
3000ba90:	f10d 0107 	add.w	r1, sp, #7
3000ba94:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000ba98:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000ba9c:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000baa0:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000baa4:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000baa8:	f042 0201 	orr.w	r2, r2, #1
3000baac:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000bab0:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000bab4:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000bab8:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000babc:	f88d c007 	strb.w	ip, [sp, #7]
3000bac0:	f7fd fa46 	bl	30008f50 <OTP_Read8>
3000bac4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bac8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bacc:	2af0      	cmp	r2, #240	; 0xf0
3000bace:	d150      	bne.n	3000bb72 <SWR_Calib_AUD+0xfe>
3000bad0:	f003 030f 	and.w	r3, r3, #15
3000bad4:	2b0f      	cmp	r3, #15
3000bad6:	d048      	beq.n	3000bb6a <SWR_Calib_AUD+0xf6>
3000bad8:	2302      	movs	r3, #2
3000bada:	4c31      	ldr	r4, [pc, #196]	; (3000bba0 <SWR_Calib_AUD+0x12c>)
3000badc:	f10d 0106 	add.w	r1, sp, #6
3000bae0:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000bae4:	7023      	strb	r3, [r4, #0]
3000bae6:	f7fd fa33 	bl	30008f50 <OTP_Read8>
3000baea:	7822      	ldrb	r2, [r4, #0]
3000baec:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000baf0:	2a03      	cmp	r2, #3
3000baf2:	d047      	beq.n	3000bb84 <SWR_Calib_AUD+0x110>
3000baf4:	2a02      	cmp	r2, #2
3000baf6:	bf0c      	ite	eq
3000baf8:	f003 030f 	andeq.w	r3, r3, #15
3000bafc:	2300      	movne	r3, #0
3000bafe:	4926      	ldr	r1, [pc, #152]	; (3000bb98 <SWR_Calib_AUD+0x124>)
3000bb00:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000bb04:	f022 020f 	bic.w	r2, r2, #15
3000bb08:	4313      	orrs	r3, r2
3000bb0a:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000bb0e:	2300      	movs	r3, #0
3000bb10:	f10d 0107 	add.w	r1, sp, #7
3000bb14:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000bb18:	f88d 3007 	strb.w	r3, [sp, #7]
3000bb1c:	f7fd fa18 	bl	30008f50 <OTP_Read8>
3000bb20:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bb24:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bb28:	2af0      	cmp	r2, #240	; 0xf0
3000bb2a:	d124      	bne.n	3000bb76 <SWR_Calib_AUD+0x102>
3000bb2c:	f003 030f 	and.w	r3, r3, #15
3000bb30:	2b0f      	cmp	r3, #15
3000bb32:	d022      	beq.n	3000bb7a <SWR_Calib_AUD+0x106>
3000bb34:	2302      	movs	r3, #2
3000bb36:	4c1b      	ldr	r4, [pc, #108]	; (3000bba4 <SWR_Calib_AUD+0x130>)
3000bb38:	f10d 0107 	add.w	r1, sp, #7
3000bb3c:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000bb40:	7023      	strb	r3, [r4, #0]
3000bb42:	f7fd fa05 	bl	30008f50 <OTP_Read8>
3000bb46:	7822      	ldrb	r2, [r4, #0]
3000bb48:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bb4c:	2a03      	cmp	r2, #3
3000bb4e:	d01b      	beq.n	3000bb88 <SWR_Calib_AUD+0x114>
3000bb50:	2a02      	cmp	r2, #2
3000bb52:	d01c      	beq.n	3000bb8e <SWR_Calib_AUD+0x11a>
3000bb54:	2200      	movs	r2, #0
3000bb56:	4910      	ldr	r1, [pc, #64]	; (3000bb98 <SWR_Calib_AUD+0x124>)
3000bb58:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000bb5c:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000bb60:	4313      	orrs	r3, r2
3000bb62:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000bb66:	b002      	add	sp, #8
3000bb68:	bd10      	pop	{r4, pc}
3000bb6a:	4b0d      	ldr	r3, [pc, #52]	; (3000bba0 <SWR_Calib_AUD+0x12c>)
3000bb6c:	2201      	movs	r2, #1
3000bb6e:	701a      	strb	r2, [r3, #0]
3000bb70:	e7cd      	b.n	3000bb0e <SWR_Calib_AUD+0x9a>
3000bb72:	2303      	movs	r3, #3
3000bb74:	e7b1      	b.n	3000bada <SWR_Calib_AUD+0x66>
3000bb76:	2303      	movs	r3, #3
3000bb78:	e7dd      	b.n	3000bb36 <SWR_Calib_AUD+0xc2>
3000bb7a:	4b0a      	ldr	r3, [pc, #40]	; (3000bba4 <SWR_Calib_AUD+0x130>)
3000bb7c:	2201      	movs	r2, #1
3000bb7e:	701a      	strb	r2, [r3, #0]
3000bb80:	b002      	add	sp, #8
3000bb82:	bd10      	pop	{r4, pc}
3000bb84:	091b      	lsrs	r3, r3, #4
3000bb86:	e7ba      	b.n	3000bafe <SWR_Calib_AUD+0x8a>
3000bb88:	091b      	lsrs	r3, r3, #4
3000bb8a:	059a      	lsls	r2, r3, #22
3000bb8c:	e7e3      	b.n	3000bb56 <SWR_Calib_AUD+0xe2>
3000bb8e:	f003 030f 	and.w	r3, r3, #15
3000bb92:	059a      	lsls	r2, r3, #22
3000bb94:	e7df      	b.n	3000bb56 <SWR_Calib_AUD+0xe2>
3000bb96:	bf00      	nop
3000bb98:	42008800 	.word	0x42008800
3000bb9c:	fffffccc 	.word	0xfffffccc
3000bba0:	3000fba9 	.word	0x3000fba9
3000bba4:	3000fba8 	.word	0x3000fba8

3000bba8 <crash_dump_memory>:
3000bba8:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000bbac:	460e      	mov	r6, r1
3000bbae:	4680      	mov	r8, r0
3000bbb0:	4b21      	ldr	r3, [pc, #132]	; (3000bc38 <crash_dump_memory+0x90>)
3000bbb2:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000bbb6:	4a21      	ldr	r2, [pc, #132]	; (3000bc3c <crash_dump_memory+0x94>)
3000bbb8:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000bbbc:	4644      	mov	r4, r8
3000bbbe:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000bbc2:	4b1f      	ldr	r3, [pc, #124]	; (3000bc40 <crash_dump_memory+0x98>)
3000bbc4:	481f      	ldr	r0, [pc, #124]	; (3000bc44 <crash_dump_memory+0x9c>)
3000bbc6:	bf88      	it	hi
3000bbc8:	2680      	movhi	r6, #128	; 0x80
3000bbca:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000bbce:	4f1e      	ldr	r7, [pc, #120]	; (3000bc48 <crash_dump_memory+0xa0>)
3000bbd0:	bf14      	ite	ne
3000bbd2:	4611      	movne	r1, r2
3000bbd4:	4619      	moveq	r1, r3
3000bbd6:	f000 fd9b 	bl	3000c710 <__DiagPrintf_veneer>
3000bbda:	4632      	mov	r2, r6
3000bbdc:	4641      	mov	r1, r8
3000bbde:	481b      	ldr	r0, [pc, #108]	; (3000bc4c <crash_dump_memory+0xa4>)
3000bbe0:	f000 fd96 	bl	3000c710 <__DiagPrintf_veneer>
3000bbe4:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bc54 <crash_dump_memory+0xac>
3000bbe8:	2300      	movs	r3, #0
3000bbea:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000bc58 <crash_dump_memory+0xb0>
3000bbee:	e007      	b.n	3000bc00 <crash_dump_memory+0x58>
3000bbf0:	6821      	ldr	r1, [r4, #0]
3000bbf2:	4638      	mov	r0, r7
3000bbf4:	f000 fd8c 	bl	3000c710 <__DiagPrintf_veneer>
3000bbf8:	42ae      	cmp	r6, r5
3000bbfa:	d00f      	beq.n	3000bc1c <crash_dump_memory+0x74>
3000bbfc:	3404      	adds	r4, #4
3000bbfe:	462b      	mov	r3, r5
3000bc00:	1c5d      	adds	r5, r3, #1
3000bc02:	b183      	cbz	r3, 3000bc26 <crash_dump_memory+0x7e>
3000bc04:	079b      	lsls	r3, r3, #30
3000bc06:	d1f3      	bne.n	3000bbf0 <crash_dump_memory+0x48>
3000bc08:	4621      	mov	r1, r4
3000bc0a:	4648      	mov	r0, r9
3000bc0c:	f000 fd80 	bl	3000c710 <__DiagPrintf_veneer>
3000bc10:	6821      	ldr	r1, [r4, #0]
3000bc12:	4638      	mov	r0, r7
3000bc14:	f000 fd7c 	bl	3000c710 <__DiagPrintf_veneer>
3000bc18:	42ae      	cmp	r6, r5
3000bc1a:	d1ef      	bne.n	3000bbfc <crash_dump_memory+0x54>
3000bc1c:	480c      	ldr	r0, [pc, #48]	; (3000bc50 <crash_dump_memory+0xa8>)
3000bc1e:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000bc22:	f000 bd75 	b.w	3000c710 <__DiagPrintf_veneer>
3000bc26:	4641      	mov	r1, r8
3000bc28:	4650      	mov	r0, sl
3000bc2a:	f000 fd71 	bl	3000c710 <__DiagPrintf_veneer>
3000bc2e:	6821      	ldr	r1, [r4, #0]
3000bc30:	4638      	mov	r0, r7
3000bc32:	f000 fd6d 	bl	3000c710 <__DiagPrintf_veneer>
3000bc36:	e7e1      	b.n	3000bbfc <crash_dump_memory+0x54>
3000bc38:	42008000 	.word	0x42008000
3000bc3c:	3000de28 	.word	0x3000de28
3000bc40:	3000de2c 	.word	0x3000de2c
3000bc44:	3000de30 	.word	0x3000de30
3000bc48:	3000de98 	.word	0x3000de98
3000bc4c:	3000de54 	.word	0x3000de54
3000bc50:	3000cdbc 	.word	0x3000cdbc
3000bc54:	3000dea0 	.word	0x3000dea0
3000bc58:	3000de90 	.word	0x3000de90

3000bc5c <crash_dump>:
3000bc5c:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bc60:	4c4f      	ldr	r4, [pc, #316]	; (3000bda0 <crash_dump+0x144>)
3000bc62:	4615      	mov	r5, r2
3000bc64:	4688      	mov	r8, r1
3000bc66:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000bc6a:	484e      	ldr	r0, [pc, #312]	; (3000bda4 <crash_dump+0x148>)
3000bc6c:	462f      	mov	r7, r5
3000bc6e:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000bdd0 <crash_dump+0x174>
3000bc72:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000bc76:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000bddc <crash_dump+0x180>
3000bc7a:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000bc7e:	e9c4 2100 	strd	r2, r1, [r4]
3000bc82:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000bc84:	60a2      	str	r2, [r4, #8]
3000bc86:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000bc88:	60e2      	str	r2, [r4, #12]
3000bc8a:	686a      	ldr	r2, [r5, #4]
3000bc8c:	6122      	str	r2, [r4, #16]
3000bc8e:	68aa      	ldr	r2, [r5, #8]
3000bc90:	6162      	str	r2, [r4, #20]
3000bc92:	68ea      	ldr	r2, [r5, #12]
3000bc94:	61a2      	str	r2, [r4, #24]
3000bc96:	692a      	ldr	r2, [r5, #16]
3000bc98:	61e2      	str	r2, [r4, #28]
3000bc9a:	696a      	ldr	r2, [r5, #20]
3000bc9c:	6222      	str	r2, [r4, #32]
3000bc9e:	69aa      	ldr	r2, [r5, #24]
3000bca0:	6262      	str	r2, [r4, #36]	; 0x24
3000bca2:	69ea      	ldr	r2, [r5, #28]
3000bca4:	62a2      	str	r2, [r4, #40]	; 0x28
3000bca6:	6a2a      	ldr	r2, [r5, #32]
3000bca8:	62e2      	str	r2, [r4, #44]	; 0x2c
3000bcaa:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bcac:	6322      	str	r2, [r4, #48]	; 0x30
3000bcae:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000bcb0:	6362      	str	r2, [r4, #52]	; 0x34
3000bcb2:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000bcb4:	63a2      	str	r2, [r4, #56]	; 0x38
3000bcb6:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000bcb8:	63e2      	str	r2, [r4, #60]	; 0x3c
3000bcba:	682a      	ldr	r2, [r5, #0]
3000bcbc:	6422      	str	r2, [r4, #64]	; 0x40
3000bcbe:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000bcc2:	6462      	str	r2, [r4, #68]	; 0x44
3000bcc4:	4a38      	ldr	r2, [pc, #224]	; (3000bda8 <crash_dump+0x14c>)
3000bcc6:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000bcca:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000bcce:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000bcd2:	64e1      	str	r1, [r4, #76]	; 0x4c
3000bcd4:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000bcd8:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000bcdc:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000bce0:	6561      	str	r1, [r4, #84]	; 0x54
3000bce2:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000bce4:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000bce8:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000bcec:	65e1      	str	r1, [r4, #92]	; 0x5c
3000bcee:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000bcf0:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000bcf2:	b289      	uxth	r1, r1
3000bcf4:	b292      	uxth	r2, r2
3000bcf6:	6621      	str	r1, [r4, #96]	; 0x60
3000bcf8:	6662      	str	r2, [r4, #100]	; 0x64
3000bcfa:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000bcfe:	66a2      	str	r2, [r4, #104]	; 0x68
3000bd00:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000bd04:	66e3      	str	r3, [r4, #108]	; 0x6c
3000bd06:	f000 fd03 	bl	3000c710 <__DiagPrintf_veneer>
3000bd0a:	4620      	mov	r0, r4
3000bd0c:	462c      	mov	r4, r5
3000bd0e:	f000 f867 	bl	3000bde0 <fault_diagnosis>
3000bd12:	4826      	ldr	r0, [pc, #152]	; (3000bdac <crash_dump+0x150>)
3000bd14:	f000 fcfc 	bl	3000c710 <__DiagPrintf_veneer>
3000bd18:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000bd1a:	4825      	ldr	r0, [pc, #148]	; (3000bdb0 <crash_dump+0x154>)
3000bd1c:	f000 fcf8 	bl	3000c710 <__DiagPrintf_veneer>
3000bd20:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000bd22:	4824      	ldr	r0, [pc, #144]	; (3000bdb4 <crash_dump+0x158>)
3000bd24:	f000 fcf4 	bl	3000c710 <__DiagPrintf_veneer>
3000bd28:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000bd2a:	4823      	ldr	r0, [pc, #140]	; (3000bdb8 <crash_dump+0x15c>)
3000bd2c:	f000 fcf0 	bl	3000c710 <__DiagPrintf_veneer>
3000bd30:	f857 1b20 	ldr.w	r1, [r7], #32
3000bd34:	4821      	ldr	r0, [pc, #132]	; (3000bdbc <crash_dump+0x160>)
3000bd36:	463e      	mov	r6, r7
3000bd38:	f000 fcea 	bl	3000c710 <__DiagPrintf_veneer>
3000bd3c:	4920      	ldr	r1, [pc, #128]	; (3000bdc0 <crash_dump+0x164>)
3000bd3e:	4658      	mov	r0, fp
3000bd40:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bd44:	f000 fce4 	bl	3000c710 <__DiagPrintf_veneer>
3000bd48:	454e      	cmp	r6, r9
3000bd4a:	d008      	beq.n	3000bd5e <crash_dump+0x102>
3000bd4c:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bd50:	4658      	mov	r0, fp
3000bd52:	f85a 1b04 	ldr.w	r1, [sl], #4
3000bd56:	f000 fcdb 	bl	3000c710 <__DiagPrintf_veneer>
3000bd5a:	454e      	cmp	r6, r9
3000bd5c:	d1f6      	bne.n	3000bd4c <crash_dump+0xf0>
3000bd5e:	4e19      	ldr	r6, [pc, #100]	; (3000bdc4 <crash_dump+0x168>)
3000bd60:	4919      	ldr	r1, [pc, #100]	; (3000bdc8 <crash_dump+0x16c>)
3000bd62:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bdd0 <crash_dump+0x174>
3000bd66:	e001      	b.n	3000bd6c <crash_dump+0x110>
3000bd68:	f856 1b04 	ldr.w	r1, [r6], #4
3000bd6c:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000bd70:	4648      	mov	r0, r9
3000bd72:	f000 fccd 	bl	3000c710 <__DiagPrintf_veneer>
3000bd76:	42bc      	cmp	r4, r7
3000bd78:	d1f6      	bne.n	3000bd68 <crash_dump+0x10c>
3000bd7a:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bd7c:	4913      	ldr	r1, [pc, #76]	; (3000bdcc <crash_dump+0x170>)
3000bd7e:	4814      	ldr	r0, [pc, #80]	; (3000bdd0 <crash_dump+0x174>)
3000bd80:	f000 fcc6 	bl	3000c710 <__DiagPrintf_veneer>
3000bd84:	2180      	movs	r1, #128	; 0x80
3000bd86:	4640      	mov	r0, r8
3000bd88:	f7ff ff0e 	bl	3000bba8 <crash_dump_memory>
3000bd8c:	4811      	ldr	r0, [pc, #68]	; (3000bdd4 <crash_dump+0x178>)
3000bd8e:	f000 fcbf 	bl	3000c710 <__DiagPrintf_veneer>
3000bd92:	4811      	ldr	r0, [pc, #68]	; (3000bdd8 <crash_dump+0x17c>)
3000bd94:	f000 fcbc 	bl	3000c710 <__DiagPrintf_veneer>
3000bd98:	2000      	movs	r0, #0
3000bd9a:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bd9e:	bf00      	nop
3000bda0:	300101c4 	.word	0x300101c4
3000bda4:	3000deb4 	.word	0x3000deb4
3000bda8:	e000ed00 	.word	0xe000ed00
3000bdac:	3000ded8 	.word	0x3000ded8
3000bdb0:	3000df00 	.word	0x3000df00
3000bdb4:	3000df10 	.word	0x3000df10
3000bdb8:	3000df20 	.word	0x3000df20
3000bdbc:	3000df30 	.word	0x3000df30
3000bdc0:	3000deac 	.word	0x3000deac
3000bdc4:	3000df90 	.word	0x3000df90
3000bdc8:	3000deb0 	.word	0x3000deb0
3000bdcc:	3000df50 	.word	0x3000df50
3000bdd0:	3000df40 	.word	0x3000df40
3000bdd4:	3000df54 	.word	0x3000df54
3000bdd8:	3000df80 	.word	0x3000df80
3000bddc:	3000dfb0 	.word	0x3000dfb0

3000bde0 <fault_diagnosis>:
3000bde0:	b510      	push	{r4, lr}
3000bde2:	4604      	mov	r4, r0
3000bde4:	4888      	ldr	r0, [pc, #544]	; (3000c008 <fault_diagnosis+0x228>)
3000bde6:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000bde8:	f000 fc92 	bl	3000c710 <__DiagPrintf_veneer>
3000bdec:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000bdf0:	079b      	lsls	r3, r3, #30
3000bdf2:	f100 808a 	bmi.w	3000bf0a <fault_diagnosis+0x12a>
3000bdf6:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bdf8:	b1e1      	cbz	r1, 3000be34 <fault_diagnosis+0x54>
3000bdfa:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bdfe:	07d8      	lsls	r0, r3, #31
3000be00:	f100 8094 	bmi.w	3000bf2c <fault_diagnosis+0x14c>
3000be04:	079a      	lsls	r2, r3, #30
3000be06:	f100 809a 	bmi.w	3000bf3e <fault_diagnosis+0x15e>
3000be0a:	0758      	lsls	r0, r3, #29
3000be0c:	f100 80a1 	bmi.w	3000bf52 <fault_diagnosis+0x172>
3000be10:	0719      	lsls	r1, r3, #28
3000be12:	f100 80a8 	bmi.w	3000bf66 <fault_diagnosis+0x186>
3000be16:	06da      	lsls	r2, r3, #27
3000be18:	f100 80af 	bmi.w	3000bf7a <fault_diagnosis+0x19a>
3000be1c:	0698      	lsls	r0, r3, #26
3000be1e:	f100 80b6 	bmi.w	3000bf8e <fault_diagnosis+0x1ae>
3000be22:	0619      	lsls	r1, r3, #24
3000be24:	f100 80bd 	bmi.w	3000bfa2 <fault_diagnosis+0x1c2>
3000be28:	065a      	lsls	r2, r3, #25
3000be2a:	d503      	bpl.n	3000be34 <fault_diagnosis+0x54>
3000be2c:	f013 0f28 	tst.w	r3, #40	; 0x28
3000be30:	f040 8186 	bne.w	3000c140 <fault_diagnosis+0x360>
3000be34:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000be38:	b199      	cbz	r1, 3000be62 <fault_diagnosis+0x82>
3000be3a:	07cb      	lsls	r3, r1, #31
3000be3c:	f100 80b9 	bmi.w	3000bfb2 <fault_diagnosis+0x1d2>
3000be40:	0788      	lsls	r0, r1, #30
3000be42:	f100 80bf 	bmi.w	3000bfc4 <fault_diagnosis+0x1e4>
3000be46:	070a      	lsls	r2, r1, #28
3000be48:	f100 80c5 	bmi.w	3000bfd6 <fault_diagnosis+0x1f6>
3000be4c:	06cb      	lsls	r3, r1, #27
3000be4e:	f100 80cb 	bmi.w	3000bfe8 <fault_diagnosis+0x208>
3000be52:	0688      	lsls	r0, r1, #26
3000be54:	f100 80d1 	bmi.w	3000bffa <fault_diagnosis+0x21a>
3000be58:	060a      	lsls	r2, r1, #24
3000be5a:	d502      	bpl.n	3000be62 <fault_diagnosis+0x82>
3000be5c:	078b      	lsls	r3, r1, #30
3000be5e:	f040 8194 	bne.w	3000c18a <fault_diagnosis+0x3aa>
3000be62:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000be66:	b1b1      	cbz	r1, 3000be96 <fault_diagnosis+0xb6>
3000be68:	07c8      	lsls	r0, r1, #31
3000be6a:	f100 80f7 	bmi.w	3000c05c <fault_diagnosis+0x27c>
3000be6e:	078a      	lsls	r2, r1, #30
3000be70:	f100 80fd 	bmi.w	3000c06e <fault_diagnosis+0x28e>
3000be74:	074b      	lsls	r3, r1, #29
3000be76:	f100 8103 	bmi.w	3000c080 <fault_diagnosis+0x2a0>
3000be7a:	0708      	lsls	r0, r1, #28
3000be7c:	f100 8109 	bmi.w	3000c092 <fault_diagnosis+0x2b2>
3000be80:	06ca      	lsls	r2, r1, #27
3000be82:	f100 810f 	bmi.w	3000c0a4 <fault_diagnosis+0x2c4>
3000be86:	068b      	lsls	r3, r1, #26
3000be88:	f100 8115 	bmi.w	3000c0b6 <fault_diagnosis+0x2d6>
3000be8c:	0608      	lsls	r0, r1, #24
3000be8e:	d502      	bpl.n	3000be96 <fault_diagnosis+0xb6>
3000be90:	0789      	lsls	r1, r1, #30
3000be92:	f100 8180 	bmi.w	3000c196 <fault_diagnosis+0x3b6>
3000be96:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be9a:	b1b9      	cbz	r1, 3000becc <fault_diagnosis+0xec>
3000be9c:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bea0:	07da      	lsls	r2, r3, #31
3000bea2:	f100 810f 	bmi.w	3000c0c4 <fault_diagnosis+0x2e4>
3000bea6:	0798      	lsls	r0, r3, #30
3000bea8:	f100 8115 	bmi.w	3000c0d6 <fault_diagnosis+0x2f6>
3000beac:	0759      	lsls	r1, r3, #29
3000beae:	f100 811d 	bmi.w	3000c0ec <fault_diagnosis+0x30c>
3000beb2:	071a      	lsls	r2, r3, #28
3000beb4:	f100 8125 	bmi.w	3000c102 <fault_diagnosis+0x322>
3000beb8:	06db      	lsls	r3, r3, #27
3000beba:	f100 812d 	bmi.w	3000c118 <fault_diagnosis+0x338>
3000bebe:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bec2:	07d8      	lsls	r0, r3, #31
3000bec4:	f100 8133 	bmi.w	3000c12e <fault_diagnosis+0x34e>
3000bec8:	0799      	lsls	r1, r3, #30
3000beca:	d424      	bmi.n	3000bf16 <fault_diagnosis+0x136>
3000becc:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bed0:	2b00      	cmp	r3, #0
3000bed2:	db00      	blt.n	3000bed6 <fault_diagnosis+0xf6>
3000bed4:	bd10      	pop	{r4, pc}
3000bed6:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bed8:	2900      	cmp	r1, #0
3000beda:	d0fb      	beq.n	3000bed4 <fault_diagnosis+0xf4>
3000bedc:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bee0:	07da      	lsls	r2, r3, #31
3000bee2:	f100 8143 	bmi.w	3000c16c <fault_diagnosis+0x38c>
3000bee6:	0798      	lsls	r0, r3, #30
3000bee8:	f100 8138 	bmi.w	3000c15c <fault_diagnosis+0x37c>
3000beec:	0759      	lsls	r1, r3, #29
3000beee:	f100 812d 	bmi.w	3000c14c <fault_diagnosis+0x36c>
3000bef2:	071a      	lsls	r2, r3, #28
3000bef4:	f100 8141 	bmi.w	3000c17a <fault_diagnosis+0x39a>
3000bef8:	06db      	lsls	r3, r3, #27
3000befa:	d5eb      	bpl.n	3000bed4 <fault_diagnosis+0xf4>
3000befc:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000befe:	4a43      	ldr	r2, [pc, #268]	; (3000c00c <fault_diagnosis+0x22c>)
3000bf00:	4843      	ldr	r0, [pc, #268]	; (3000c010 <fault_diagnosis+0x230>)
3000bf02:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000bf06:	f000 bc03 	b.w	3000c710 <__DiagPrintf_veneer>
3000bf0a:	4a42      	ldr	r2, [pc, #264]	; (3000c014 <fault_diagnosis+0x234>)
3000bf0c:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000bf0e:	4842      	ldr	r0, [pc, #264]	; (3000c018 <fault_diagnosis+0x238>)
3000bf10:	f000 fbfe 	bl	3000c710 <__DiagPrintf_veneer>
3000bf14:	e76f      	b.n	3000bdf6 <fault_diagnosis+0x16>
3000bf16:	4a41      	ldr	r2, [pc, #260]	; (3000c01c <fault_diagnosis+0x23c>)
3000bf18:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bf1c:	4840      	ldr	r0, [pc, #256]	; (3000c020 <fault_diagnosis+0x240>)
3000bf1e:	f000 fbf7 	bl	3000c710 <__DiagPrintf_veneer>
3000bf22:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bf26:	2b00      	cmp	r3, #0
3000bf28:	dad4      	bge.n	3000bed4 <fault_diagnosis+0xf4>
3000bf2a:	e7d4      	b.n	3000bed6 <fault_diagnosis+0xf6>
3000bf2c:	4a3d      	ldr	r2, [pc, #244]	; (3000c024 <fault_diagnosis+0x244>)
3000bf2e:	483e      	ldr	r0, [pc, #248]	; (3000c028 <fault_diagnosis+0x248>)
3000bf30:	f000 fbee 	bl	3000c710 <__DiagPrintf_veneer>
3000bf34:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf38:	079a      	lsls	r2, r3, #30
3000bf3a:	f57f af66 	bpl.w	3000be0a <fault_diagnosis+0x2a>
3000bf3e:	4a3b      	ldr	r2, [pc, #236]	; (3000c02c <fault_diagnosis+0x24c>)
3000bf40:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bf42:	4839      	ldr	r0, [pc, #228]	; (3000c028 <fault_diagnosis+0x248>)
3000bf44:	f000 fbe4 	bl	3000c710 <__DiagPrintf_veneer>
3000bf48:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf4c:	0758      	lsls	r0, r3, #29
3000bf4e:	f57f af5f 	bpl.w	3000be10 <fault_diagnosis+0x30>
3000bf52:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bf54:	4a36      	ldr	r2, [pc, #216]	; (3000c030 <fault_diagnosis+0x250>)
3000bf56:	4834      	ldr	r0, [pc, #208]	; (3000c028 <fault_diagnosis+0x248>)
3000bf58:	f000 fbda 	bl	3000c710 <__DiagPrintf_veneer>
3000bf5c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf60:	0719      	lsls	r1, r3, #28
3000bf62:	f57f af58 	bpl.w	3000be16 <fault_diagnosis+0x36>
3000bf66:	4a33      	ldr	r2, [pc, #204]	; (3000c034 <fault_diagnosis+0x254>)
3000bf68:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bf6a:	482f      	ldr	r0, [pc, #188]	; (3000c028 <fault_diagnosis+0x248>)
3000bf6c:	f000 fbd0 	bl	3000c710 <__DiagPrintf_veneer>
3000bf70:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf74:	06da      	lsls	r2, r3, #27
3000bf76:	f57f af51 	bpl.w	3000be1c <fault_diagnosis+0x3c>
3000bf7a:	4a2f      	ldr	r2, [pc, #188]	; (3000c038 <fault_diagnosis+0x258>)
3000bf7c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bf7e:	482a      	ldr	r0, [pc, #168]	; (3000c028 <fault_diagnosis+0x248>)
3000bf80:	f000 fbc6 	bl	3000c710 <__DiagPrintf_veneer>
3000bf84:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf88:	0698      	lsls	r0, r3, #26
3000bf8a:	f57f af4a 	bpl.w	3000be22 <fault_diagnosis+0x42>
3000bf8e:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bf90:	4a2a      	ldr	r2, [pc, #168]	; (3000c03c <fault_diagnosis+0x25c>)
3000bf92:	4825      	ldr	r0, [pc, #148]	; (3000c028 <fault_diagnosis+0x248>)
3000bf94:	f000 fbbc 	bl	3000c710 <__DiagPrintf_veneer>
3000bf98:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bf9c:	0619      	lsls	r1, r3, #24
3000bf9e:	f57f af43 	bpl.w	3000be28 <fault_diagnosis+0x48>
3000bfa2:	4a27      	ldr	r2, [pc, #156]	; (3000c040 <fault_diagnosis+0x260>)
3000bfa4:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bfa6:	4820      	ldr	r0, [pc, #128]	; (3000c028 <fault_diagnosis+0x248>)
3000bfa8:	f000 fbb2 	bl	3000c710 <__DiagPrintf_veneer>
3000bfac:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bfb0:	e73a      	b.n	3000be28 <fault_diagnosis+0x48>
3000bfb2:	4a24      	ldr	r2, [pc, #144]	; (3000c044 <fault_diagnosis+0x264>)
3000bfb4:	4824      	ldr	r0, [pc, #144]	; (3000c048 <fault_diagnosis+0x268>)
3000bfb6:	f000 fbab 	bl	3000c710 <__DiagPrintf_veneer>
3000bfba:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bfbe:	0788      	lsls	r0, r1, #30
3000bfc0:	f57f af41 	bpl.w	3000be46 <fault_diagnosis+0x66>
3000bfc4:	4a21      	ldr	r2, [pc, #132]	; (3000c04c <fault_diagnosis+0x26c>)
3000bfc6:	4820      	ldr	r0, [pc, #128]	; (3000c048 <fault_diagnosis+0x268>)
3000bfc8:	f000 fba2 	bl	3000c710 <__DiagPrintf_veneer>
3000bfcc:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bfd0:	070a      	lsls	r2, r1, #28
3000bfd2:	f57f af3b 	bpl.w	3000be4c <fault_diagnosis+0x6c>
3000bfd6:	4a1e      	ldr	r2, [pc, #120]	; (3000c050 <fault_diagnosis+0x270>)
3000bfd8:	481b      	ldr	r0, [pc, #108]	; (3000c048 <fault_diagnosis+0x268>)
3000bfda:	f000 fb99 	bl	3000c710 <__DiagPrintf_veneer>
3000bfde:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bfe2:	06cb      	lsls	r3, r1, #27
3000bfe4:	f57f af35 	bpl.w	3000be52 <fault_diagnosis+0x72>
3000bfe8:	4a1a      	ldr	r2, [pc, #104]	; (3000c054 <fault_diagnosis+0x274>)
3000bfea:	4817      	ldr	r0, [pc, #92]	; (3000c048 <fault_diagnosis+0x268>)
3000bfec:	f000 fb90 	bl	3000c710 <__DiagPrintf_veneer>
3000bff0:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bff4:	0688      	lsls	r0, r1, #26
3000bff6:	f57f af2f 	bpl.w	3000be58 <fault_diagnosis+0x78>
3000bffa:	4a17      	ldr	r2, [pc, #92]	; (3000c058 <fault_diagnosis+0x278>)
3000bffc:	4812      	ldr	r0, [pc, #72]	; (3000c048 <fault_diagnosis+0x268>)
3000bffe:	f000 fb87 	bl	3000c710 <__DiagPrintf_veneer>
3000c002:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000c006:	e727      	b.n	3000be58 <fault_diagnosis+0x78>
3000c008:	3000dfc0 	.word	0x3000dfc0
3000c00c:	3000e850 	.word	0x3000e850
3000c010:	3000e7a8 	.word	0x3000e7a8
3000c014:	3000dfd8 	.word	0x3000dfd8
3000c018:	3000e004 	.word	0x3000e004
3000c01c:	3000e70c 	.word	0x3000e70c
3000c020:	3000e550 	.word	0x3000e550
3000c024:	3000e01c 	.word	0x3000e01c
3000c028:	3000e04c 	.word	0x3000e04c
3000c02c:	3000e064 	.word	0x3000e064
3000c030:	3000e09c 	.word	0x3000e09c
3000c034:	3000e0d4 	.word	0x3000e0d4
3000c038:	3000e10c 	.word	0x3000e10c
3000c03c:	3000e13c 	.word	0x3000e13c
3000c040:	3000e178 	.word	0x3000e178
3000c044:	3000e1e8 	.word	0x3000e1e8
3000c048:	3000e22c 	.word	0x3000e22c
3000c04c:	3000e244 	.word	0x3000e244
3000c050:	3000e280 	.word	0x3000e280
3000c054:	3000e2b8 	.word	0x3000e2b8
3000c058:	3000e2ec 	.word	0x3000e2ec
3000c05c:	4a51      	ldr	r2, [pc, #324]	; (3000c1a4 <fault_diagnosis+0x3c4>)
3000c05e:	4852      	ldr	r0, [pc, #328]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c060:	f000 fb56 	bl	3000c710 <__DiagPrintf_veneer>
3000c064:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c068:	078a      	lsls	r2, r1, #30
3000c06a:	f57f af03 	bpl.w	3000be74 <fault_diagnosis+0x94>
3000c06e:	4a4f      	ldr	r2, [pc, #316]	; (3000c1ac <fault_diagnosis+0x3cc>)
3000c070:	484d      	ldr	r0, [pc, #308]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c072:	f000 fb4d 	bl	3000c710 <__DiagPrintf_veneer>
3000c076:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c07a:	074b      	lsls	r3, r1, #29
3000c07c:	f57f aefd 	bpl.w	3000be7a <fault_diagnosis+0x9a>
3000c080:	4a4b      	ldr	r2, [pc, #300]	; (3000c1b0 <fault_diagnosis+0x3d0>)
3000c082:	4849      	ldr	r0, [pc, #292]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c084:	f000 fb44 	bl	3000c710 <__DiagPrintf_veneer>
3000c088:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c08c:	0708      	lsls	r0, r1, #28
3000c08e:	f57f aef7 	bpl.w	3000be80 <fault_diagnosis+0xa0>
3000c092:	4a48      	ldr	r2, [pc, #288]	; (3000c1b4 <fault_diagnosis+0x3d4>)
3000c094:	4844      	ldr	r0, [pc, #272]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c096:	f000 fb3b 	bl	3000c710 <__DiagPrintf_veneer>
3000c09a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c09e:	06ca      	lsls	r2, r1, #27
3000c0a0:	f57f aef1 	bpl.w	3000be86 <fault_diagnosis+0xa6>
3000c0a4:	4a44      	ldr	r2, [pc, #272]	; (3000c1b8 <fault_diagnosis+0x3d8>)
3000c0a6:	4840      	ldr	r0, [pc, #256]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c0a8:	f000 fb32 	bl	3000c710 <__DiagPrintf_veneer>
3000c0ac:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c0b0:	068b      	lsls	r3, r1, #26
3000c0b2:	f57f aeeb 	bpl.w	3000be8c <fault_diagnosis+0xac>
3000c0b6:	4a41      	ldr	r2, [pc, #260]	; (3000c1bc <fault_diagnosis+0x3dc>)
3000c0b8:	483b      	ldr	r0, [pc, #236]	; (3000c1a8 <fault_diagnosis+0x3c8>)
3000c0ba:	f000 fb29 	bl	3000c710 <__DiagPrintf_veneer>
3000c0be:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000c0c2:	e6e3      	b.n	3000be8c <fault_diagnosis+0xac>
3000c0c4:	4a3e      	ldr	r2, [pc, #248]	; (3000c1c0 <fault_diagnosis+0x3e0>)
3000c0c6:	483f      	ldr	r0, [pc, #252]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c0c8:	f000 fb22 	bl	3000c710 <__DiagPrintf_veneer>
3000c0cc:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c0d0:	0798      	lsls	r0, r3, #30
3000c0d2:	f57f aeeb 	bpl.w	3000beac <fault_diagnosis+0xcc>
3000c0d6:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c0da:	4a3b      	ldr	r2, [pc, #236]	; (3000c1c8 <fault_diagnosis+0x3e8>)
3000c0dc:	4839      	ldr	r0, [pc, #228]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c0de:	f000 fb17 	bl	3000c710 <__DiagPrintf_veneer>
3000c0e2:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c0e6:	0759      	lsls	r1, r3, #29
3000c0e8:	f57f aee3 	bpl.w	3000beb2 <fault_diagnosis+0xd2>
3000c0ec:	4a37      	ldr	r2, [pc, #220]	; (3000c1cc <fault_diagnosis+0x3ec>)
3000c0ee:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c0f2:	4834      	ldr	r0, [pc, #208]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c0f4:	f000 fb0c 	bl	3000c710 <__DiagPrintf_veneer>
3000c0f8:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c0fc:	071a      	lsls	r2, r3, #28
3000c0fe:	f57f aedb 	bpl.w	3000beb8 <fault_diagnosis+0xd8>
3000c102:	4a33      	ldr	r2, [pc, #204]	; (3000c1d0 <fault_diagnosis+0x3f0>)
3000c104:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c108:	482e      	ldr	r0, [pc, #184]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c10a:	f000 fb01 	bl	3000c710 <__DiagPrintf_veneer>
3000c10e:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c112:	06db      	lsls	r3, r3, #27
3000c114:	f57f aed3 	bpl.w	3000bebe <fault_diagnosis+0xde>
3000c118:	4a2e      	ldr	r2, [pc, #184]	; (3000c1d4 <fault_diagnosis+0x3f4>)
3000c11a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c11e:	4829      	ldr	r0, [pc, #164]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c120:	f000 faf6 	bl	3000c710 <__DiagPrintf_veneer>
3000c124:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c128:	07d8      	lsls	r0, r3, #31
3000c12a:	f57f aecd 	bpl.w	3000bec8 <fault_diagnosis+0xe8>
3000c12e:	4a2a      	ldr	r2, [pc, #168]	; (3000c1d8 <fault_diagnosis+0x3f8>)
3000c130:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c134:	4823      	ldr	r0, [pc, #140]	; (3000c1c4 <fault_diagnosis+0x3e4>)
3000c136:	f000 faeb 	bl	3000c710 <__DiagPrintf_veneer>
3000c13a:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c13e:	e6c3      	b.n	3000bec8 <fault_diagnosis+0xe8>
3000c140:	4a26      	ldr	r2, [pc, #152]	; (3000c1dc <fault_diagnosis+0x3fc>)
3000c142:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000c144:	4826      	ldr	r0, [pc, #152]	; (3000c1e0 <fault_diagnosis+0x400>)
3000c146:	f000 fae3 	bl	3000c710 <__DiagPrintf_veneer>
3000c14a:	e673      	b.n	3000be34 <fault_diagnosis+0x54>
3000c14c:	4a25      	ldr	r2, [pc, #148]	; (3000c1e4 <fault_diagnosis+0x404>)
3000c14e:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c150:	4825      	ldr	r0, [pc, #148]	; (3000c1e8 <fault_diagnosis+0x408>)
3000c152:	f000 fadd 	bl	3000c710 <__DiagPrintf_veneer>
3000c156:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c15a:	e6ca      	b.n	3000bef2 <fault_diagnosis+0x112>
3000c15c:	4a23      	ldr	r2, [pc, #140]	; (3000c1ec <fault_diagnosis+0x40c>)
3000c15e:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c160:	4821      	ldr	r0, [pc, #132]	; (3000c1e8 <fault_diagnosis+0x408>)
3000c162:	f000 fad5 	bl	3000c710 <__DiagPrintf_veneer>
3000c166:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c16a:	e6bf      	b.n	3000beec <fault_diagnosis+0x10c>
3000c16c:	4a20      	ldr	r2, [pc, #128]	; (3000c1f0 <fault_diagnosis+0x410>)
3000c16e:	481e      	ldr	r0, [pc, #120]	; (3000c1e8 <fault_diagnosis+0x408>)
3000c170:	f000 face 	bl	3000c710 <__DiagPrintf_veneer>
3000c174:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c178:	e6b5      	b.n	3000bee6 <fault_diagnosis+0x106>
3000c17a:	4a1e      	ldr	r2, [pc, #120]	; (3000c1f4 <fault_diagnosis+0x414>)
3000c17c:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c17e:	481a      	ldr	r0, [pc, #104]	; (3000c1e8 <fault_diagnosis+0x408>)
3000c180:	f000 fac6 	bl	3000c710 <__DiagPrintf_veneer>
3000c184:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c188:	e6b6      	b.n	3000bef8 <fault_diagnosis+0x118>
3000c18a:	4a1b      	ldr	r2, [pc, #108]	; (3000c1f8 <fault_diagnosis+0x418>)
3000c18c:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000c18e:	481b      	ldr	r0, [pc, #108]	; (3000c1fc <fault_diagnosis+0x41c>)
3000c190:	f000 fabe 	bl	3000c710 <__DiagPrintf_veneer>
3000c194:	e665      	b.n	3000be62 <fault_diagnosis+0x82>
3000c196:	4a1a      	ldr	r2, [pc, #104]	; (3000c200 <fault_diagnosis+0x420>)
3000c198:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000c19a:	481a      	ldr	r0, [pc, #104]	; (3000c204 <fault_diagnosis+0x424>)
3000c19c:	f000 fab8 	bl	3000c710 <__DiagPrintf_veneer>
3000c1a0:	e679      	b.n	3000be96 <fault_diagnosis+0xb6>
3000c1a2:	bf00      	nop
3000c1a4:	3000e380 	.word	0x3000e380
3000c1a8:	3000e3b4 	.word	0x3000e3b4
3000c1ac:	3000e3cc 	.word	0x3000e3cc
3000c1b0:	3000e404 	.word	0x3000e404
3000c1b4:	3000e43c 	.word	0x3000e43c
3000c1b8:	3000e464 	.word	0x3000e464
3000c1bc:	3000e48c 	.word	0x3000e48c
3000c1c0:	3000e508 	.word	0x3000e508
3000c1c4:	3000e550 	.word	0x3000e550
3000c1c8:	3000e568 	.word	0x3000e568
3000c1cc:	3000e5b4 	.word	0x3000e5b4
3000c1d0:	3000e614 	.word	0x3000e614
3000c1d4:	3000e65c 	.word	0x3000e65c
3000c1d8:	3000e6b8 	.word	0x3000e6b8
3000c1dc:	3000e1a8 	.word	0x3000e1a8
3000c1e0:	3000e1d0 	.word	0x3000e1d0
3000c1e4:	3000e7f4 	.word	0x3000e7f4
3000c1e8:	3000e7a8 	.word	0x3000e7a8
3000c1ec:	3000e7c0 	.word	0x3000e7c0
3000c1f0:	3000e778 	.word	0x3000e778
3000c1f4:	3000e820 	.word	0x3000e820
3000c1f8:	3000e338 	.word	0x3000e338
3000c1fc:	3000e368 	.word	0x3000e368
3000c200:	3000e4cc 	.word	0x3000e4cc
3000c204:	3000e4f0 	.word	0x3000e4f0

3000c208 <rtk_log_write>:
3000c208:	b40c      	push	{r2, r3}
3000c20a:	4b18      	ldr	r3, [pc, #96]	; (3000c26c <rtk_log_write+0x64>)
3000c20c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c210:	681a      	ldr	r2, [r3, #0]
3000c212:	b083      	sub	sp, #12
3000c214:	4680      	mov	r8, r0
3000c216:	460e      	mov	r6, r1
3000c218:	2a02      	cmp	r2, #2
3000c21a:	d825      	bhi.n	3000c268 <rtk_log_write+0x60>
3000c21c:	681f      	ldr	r7, [r3, #0]
3000c21e:	b1a7      	cbz	r7, 3000c24a <rtk_log_write+0x42>
3000c220:	4d13      	ldr	r5, [pc, #76]	; (3000c270 <rtk_log_write+0x68>)
3000c222:	2400      	movs	r4, #0
3000c224:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000c27c <rtk_log_write+0x74>
3000c228:	e002      	b.n	3000c230 <rtk_log_write+0x28>
3000c22a:	3401      	adds	r4, #1
3000c22c:	42a7      	cmp	r7, r4
3000c22e:	d90c      	bls.n	3000c24a <rtk_log_write+0x42>
3000c230:	4628      	mov	r0, r5
3000c232:	4631      	mov	r1, r6
3000c234:	350b      	adds	r5, #11
3000c236:	47c8      	blx	r9
3000c238:	2800      	cmp	r0, #0
3000c23a:	d1f6      	bne.n	3000c22a <rtk_log_write+0x22>
3000c23c:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000c240:	4b0c      	ldr	r3, [pc, #48]	; (3000c274 <rtk_log_write+0x6c>)
3000c242:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000c246:	5d1b      	ldrb	r3, [r3, r4]
3000c248:	e001      	b.n	3000c24e <rtk_log_write+0x46>
3000c24a:	4b0b      	ldr	r3, [pc, #44]	; (3000c278 <rtk_log_write+0x70>)
3000c24c:	781b      	ldrb	r3, [r3, #0]
3000c24e:	4598      	cmp	r8, r3
3000c250:	d805      	bhi.n	3000c25e <rtk_log_write+0x56>
3000c252:	aa0b      	add	r2, sp, #44	; 0x2c
3000c254:	990a      	ldr	r1, [sp, #40]	; 0x28
3000c256:	2000      	movs	r0, #0
3000c258:	9201      	str	r2, [sp, #4]
3000c25a:	f000 fa71 	bl	3000c740 <__DiagVSprintf_veneer>
3000c25e:	b003      	add	sp, #12
3000c260:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c264:	b002      	add	sp, #8
3000c266:	4770      	bx	lr
3000c268:	2703      	movs	r7, #3
3000c26a:	e7d9      	b.n	3000c220 <rtk_log_write+0x18>
3000c26c:	30010604 	.word	0x30010604
3000c270:	30010609 	.word	0x30010609
3000c274:	30010608 	.word	0x30010608
3000c278:	3000fbbc 	.word	0x3000fbbc
3000c27c:	00012c89 	.word	0x00012c89

3000c280 <BOOT_VerCheck>:
3000c280:	b570      	push	{r4, r5, r6, lr}
3000c282:	4b1d      	ldr	r3, [pc, #116]	; (3000c2f8 <BOOT_VerCheck+0x78>)
3000c284:	4798      	blx	r3
3000c286:	b100      	cbz	r0, 3000c28a <BOOT_VerCheck+0xa>
3000c288:	bd70      	pop	{r4, r5, r6, pc}
3000c28a:	4b1c      	ldr	r3, [pc, #112]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c28c:	2503      	movs	r5, #3
3000c28e:	4c1b      	ldr	r4, [pc, #108]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c290:	2003      	movs	r0, #3
3000c292:	4e1b      	ldr	r6, [pc, #108]	; (3000c300 <BOOT_VerCheck+0x80>)
3000c294:	4619      	mov	r1, r3
3000c296:	4a1b      	ldr	r2, [pc, #108]	; (3000c304 <BOOT_VerCheck+0x84>)
3000c298:	f7ff ffb6 	bl	3000c208 <rtk_log_write>
3000c29c:	4632      	mov	r2, r6
3000c29e:	4623      	mov	r3, r4
3000c2a0:	4916      	ldr	r1, [pc, #88]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2a2:	2003      	movs	r0, #3
3000c2a4:	f7ff ffb0 	bl	3000c208 <rtk_log_write>
3000c2a8:	4623      	mov	r3, r4
3000c2aa:	4a17      	ldr	r2, [pc, #92]	; (3000c308 <BOOT_VerCheck+0x88>)
3000c2ac:	2003      	movs	r0, #3
3000c2ae:	4913      	ldr	r1, [pc, #76]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2b0:	f7ff ffaa 	bl	3000c208 <rtk_log_write>
3000c2b4:	4623      	mov	r3, r4
3000c2b6:	4a15      	ldr	r2, [pc, #84]	; (3000c30c <BOOT_VerCheck+0x8c>)
3000c2b8:	2003      	movs	r0, #3
3000c2ba:	4910      	ldr	r1, [pc, #64]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2bc:	f7ff ffa4 	bl	3000c208 <rtk_log_write>
3000c2c0:	4623      	mov	r3, r4
3000c2c2:	4a13      	ldr	r2, [pc, #76]	; (3000c310 <BOOT_VerCheck+0x90>)
3000c2c4:	2003      	movs	r0, #3
3000c2c6:	490d      	ldr	r1, [pc, #52]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2c8:	f7ff ff9e 	bl	3000c208 <rtk_log_write>
3000c2cc:	4623      	mov	r3, r4
3000c2ce:	4632      	mov	r2, r6
3000c2d0:	490a      	ldr	r1, [pc, #40]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2d2:	2003      	movs	r0, #3
3000c2d4:	f7ff ff98 	bl	3000c208 <rtk_log_write>
3000c2d8:	4623      	mov	r3, r4
3000c2da:	4a0e      	ldr	r2, [pc, #56]	; (3000c314 <BOOT_VerCheck+0x94>)
3000c2dc:	2003      	movs	r0, #3
3000c2de:	4907      	ldr	r1, [pc, #28]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2e0:	f7ff ff92 	bl	3000c208 <rtk_log_write>
3000c2e4:	3d01      	subs	r5, #1
3000c2e6:	d1d9      	bne.n	3000c29c <BOOT_VerCheck+0x1c>
3000c2e8:	4b04      	ldr	r3, [pc, #16]	; (3000c2fc <BOOT_VerCheck+0x7c>)
3000c2ea:	2003      	movs	r0, #3
3000c2ec:	4a0a      	ldr	r2, [pc, #40]	; (3000c318 <BOOT_VerCheck+0x98>)
3000c2ee:	4619      	mov	r1, r3
3000c2f0:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000c2f4:	f7ff bf88 	b.w	3000c208 <rtk_log_write>
3000c2f8:	3000a261 	.word	0x3000a261
3000c2fc:	3000cd6c 	.word	0x3000cd6c
3000c300:	3000e968 	.word	0x3000e968
3000c304:	3000e924 	.word	0x3000e924
3000c308:	3000e9ac 	.word	0x3000e9ac
3000c30c:	3000e9f0 	.word	0x3000e9f0
3000c310:	3000ea34 	.word	0x3000ea34
3000c314:	3000ea78 	.word	0x3000ea78
3000c318:	3000eabc 	.word	0x3000eabc

3000c31c <__aeabi_uldivmod>:
3000c31c:	b953      	cbnz	r3, 3000c334 <__aeabi_uldivmod+0x18>
3000c31e:	b94a      	cbnz	r2, 3000c334 <__aeabi_uldivmod+0x18>
3000c320:	2900      	cmp	r1, #0
3000c322:	bf08      	it	eq
3000c324:	2800      	cmpeq	r0, #0
3000c326:	bf1c      	itt	ne
3000c328:	f04f 31ff 	movne.w	r1, #4294967295
3000c32c:	f04f 30ff 	movne.w	r0, #4294967295
3000c330:	f000 b960 	b.w	3000c5f4 <__aeabi_idiv0>
3000c334:	f1ad 0c08 	sub.w	ip, sp, #8
3000c338:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000c33c:	f000 f806 	bl	3000c34c <__udivmoddi4>
3000c340:	f8dd e004 	ldr.w	lr, [sp, #4]
3000c344:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000c348:	b004      	add	sp, #16
3000c34a:	4770      	bx	lr

3000c34c <__udivmoddi4>:
3000c34c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000c350:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000c352:	4688      	mov	r8, r1
3000c354:	468e      	mov	lr, r1
3000c356:	4615      	mov	r5, r2
3000c358:	4604      	mov	r4, r0
3000c35a:	4619      	mov	r1, r3
3000c35c:	2b00      	cmp	r3, #0
3000c35e:	f040 80c6 	bne.w	3000c4ee <__udivmoddi4+0x1a2>
3000c362:	4542      	cmp	r2, r8
3000c364:	fab2 f782 	clz	r7, r2
3000c368:	d946      	bls.n	3000c3f8 <__udivmoddi4+0xac>
3000c36a:	b14f      	cbz	r7, 3000c380 <__udivmoddi4+0x34>
3000c36c:	f1c7 0c20 	rsb	ip, r7, #32
3000c370:	fa08 f307 	lsl.w	r3, r8, r7
3000c374:	40bd      	lsls	r5, r7
3000c376:	40bc      	lsls	r4, r7
3000c378:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c37c:	ea4c 0e03 	orr.w	lr, ip, r3
3000c380:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000c384:	fa1f fc85 	uxth.w	ip, r5
3000c388:	0c22      	lsrs	r2, r4, #16
3000c38a:	fbbe f9f8 	udiv	r9, lr, r8
3000c38e:	fb08 e319 	mls	r3, r8, r9, lr
3000c392:	fb09 fa0c 	mul.w	sl, r9, ip
3000c396:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000c39a:	459a      	cmp	sl, r3
3000c39c:	d928      	bls.n	3000c3f0 <__udivmoddi4+0xa4>
3000c39e:	18eb      	adds	r3, r5, r3
3000c3a0:	f109 30ff 	add.w	r0, r9, #4294967295
3000c3a4:	d204      	bcs.n	3000c3b0 <__udivmoddi4+0x64>
3000c3a6:	459a      	cmp	sl, r3
3000c3a8:	d902      	bls.n	3000c3b0 <__udivmoddi4+0x64>
3000c3aa:	f1a9 0002 	sub.w	r0, r9, #2
3000c3ae:	442b      	add	r3, r5
3000c3b0:	eba3 030a 	sub.w	r3, r3, sl
3000c3b4:	b2a4      	uxth	r4, r4
3000c3b6:	fbb3 f2f8 	udiv	r2, r3, r8
3000c3ba:	fb08 3312 	mls	r3, r8, r2, r3
3000c3be:	fb02 fc0c 	mul.w	ip, r2, ip
3000c3c2:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c3c6:	45a4      	cmp	ip, r4
3000c3c8:	d914      	bls.n	3000c3f4 <__udivmoddi4+0xa8>
3000c3ca:	192c      	adds	r4, r5, r4
3000c3cc:	f102 33ff 	add.w	r3, r2, #4294967295
3000c3d0:	d203      	bcs.n	3000c3da <__udivmoddi4+0x8e>
3000c3d2:	45a4      	cmp	ip, r4
3000c3d4:	d901      	bls.n	3000c3da <__udivmoddi4+0x8e>
3000c3d6:	1e93      	subs	r3, r2, #2
3000c3d8:	442c      	add	r4, r5
3000c3da:	eba4 040c 	sub.w	r4, r4, ip
3000c3de:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c3e2:	b11e      	cbz	r6, 3000c3ec <__udivmoddi4+0xa0>
3000c3e4:	40fc      	lsrs	r4, r7
3000c3e6:	2300      	movs	r3, #0
3000c3e8:	6034      	str	r4, [r6, #0]
3000c3ea:	6073      	str	r3, [r6, #4]
3000c3ec:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c3f0:	4648      	mov	r0, r9
3000c3f2:	e7dd      	b.n	3000c3b0 <__udivmoddi4+0x64>
3000c3f4:	4613      	mov	r3, r2
3000c3f6:	e7f0      	b.n	3000c3da <__udivmoddi4+0x8e>
3000c3f8:	b902      	cbnz	r2, 3000c3fc <__udivmoddi4+0xb0>
3000c3fa:	deff      	udf	#255	; 0xff
3000c3fc:	bb8f      	cbnz	r7, 3000c462 <__udivmoddi4+0x116>
3000c3fe:	eba8 0302 	sub.w	r3, r8, r2
3000c402:	2101      	movs	r1, #1
3000c404:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c408:	b2aa      	uxth	r2, r5
3000c40a:	0c20      	lsrs	r0, r4, #16
3000c40c:	fbb3 fcfe 	udiv	ip, r3, lr
3000c410:	fb0e 331c 	mls	r3, lr, ip, r3
3000c414:	fb0c f802 	mul.w	r8, ip, r2
3000c418:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c41c:	4598      	cmp	r8, r3
3000c41e:	d962      	bls.n	3000c4e6 <__udivmoddi4+0x19a>
3000c420:	18eb      	adds	r3, r5, r3
3000c422:	f10c 30ff 	add.w	r0, ip, #4294967295
3000c426:	d204      	bcs.n	3000c432 <__udivmoddi4+0xe6>
3000c428:	4598      	cmp	r8, r3
3000c42a:	d902      	bls.n	3000c432 <__udivmoddi4+0xe6>
3000c42c:	f1ac 0002 	sub.w	r0, ip, #2
3000c430:	442b      	add	r3, r5
3000c432:	eba3 0308 	sub.w	r3, r3, r8
3000c436:	b2a4      	uxth	r4, r4
3000c438:	fbb3 fcfe 	udiv	ip, r3, lr
3000c43c:	fb0e 331c 	mls	r3, lr, ip, r3
3000c440:	fb0c f202 	mul.w	r2, ip, r2
3000c444:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c448:	42a2      	cmp	r2, r4
3000c44a:	d94e      	bls.n	3000c4ea <__udivmoddi4+0x19e>
3000c44c:	192c      	adds	r4, r5, r4
3000c44e:	f10c 33ff 	add.w	r3, ip, #4294967295
3000c452:	d204      	bcs.n	3000c45e <__udivmoddi4+0x112>
3000c454:	42a2      	cmp	r2, r4
3000c456:	d902      	bls.n	3000c45e <__udivmoddi4+0x112>
3000c458:	f1ac 0302 	sub.w	r3, ip, #2
3000c45c:	442c      	add	r4, r5
3000c45e:	1aa4      	subs	r4, r4, r2
3000c460:	e7bd      	b.n	3000c3de <__udivmoddi4+0x92>
3000c462:	f1c7 0c20 	rsb	ip, r7, #32
3000c466:	40bd      	lsls	r5, r7
3000c468:	fa08 f307 	lsl.w	r3, r8, r7
3000c46c:	40bc      	lsls	r4, r7
3000c46e:	fa28 f90c 	lsr.w	r9, r8, ip
3000c472:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c476:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c47a:	ea4c 0203 	orr.w	r2, ip, r3
3000c47e:	b2ab      	uxth	r3, r5
3000c480:	fbb9 fcfe 	udiv	ip, r9, lr
3000c484:	0c11      	lsrs	r1, r2, #16
3000c486:	fb0e 901c 	mls	r0, lr, ip, r9
3000c48a:	fb0c f803 	mul.w	r8, ip, r3
3000c48e:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c492:	4580      	cmp	r8, r0
3000c494:	d923      	bls.n	3000c4de <__udivmoddi4+0x192>
3000c496:	1828      	adds	r0, r5, r0
3000c498:	f10c 31ff 	add.w	r1, ip, #4294967295
3000c49c:	d204      	bcs.n	3000c4a8 <__udivmoddi4+0x15c>
3000c49e:	4580      	cmp	r8, r0
3000c4a0:	d902      	bls.n	3000c4a8 <__udivmoddi4+0x15c>
3000c4a2:	f1ac 0102 	sub.w	r1, ip, #2
3000c4a6:	4428      	add	r0, r5
3000c4a8:	eba0 0008 	sub.w	r0, r0, r8
3000c4ac:	b292      	uxth	r2, r2
3000c4ae:	fbb0 fcfe 	udiv	ip, r0, lr
3000c4b2:	fb0e 001c 	mls	r0, lr, ip, r0
3000c4b6:	fb0c f803 	mul.w	r8, ip, r3
3000c4ba:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c4be:	4598      	cmp	r8, r3
3000c4c0:	d90f      	bls.n	3000c4e2 <__udivmoddi4+0x196>
3000c4c2:	18eb      	adds	r3, r5, r3
3000c4c4:	f10c 32ff 	add.w	r2, ip, #4294967295
3000c4c8:	d204      	bcs.n	3000c4d4 <__udivmoddi4+0x188>
3000c4ca:	4598      	cmp	r8, r3
3000c4cc:	d902      	bls.n	3000c4d4 <__udivmoddi4+0x188>
3000c4ce:	f1ac 0202 	sub.w	r2, ip, #2
3000c4d2:	442b      	add	r3, r5
3000c4d4:	eba3 0308 	sub.w	r3, r3, r8
3000c4d8:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c4dc:	e792      	b.n	3000c404 <__udivmoddi4+0xb8>
3000c4de:	4661      	mov	r1, ip
3000c4e0:	e7e2      	b.n	3000c4a8 <__udivmoddi4+0x15c>
3000c4e2:	4662      	mov	r2, ip
3000c4e4:	e7f6      	b.n	3000c4d4 <__udivmoddi4+0x188>
3000c4e6:	4660      	mov	r0, ip
3000c4e8:	e7a3      	b.n	3000c432 <__udivmoddi4+0xe6>
3000c4ea:	4663      	mov	r3, ip
3000c4ec:	e7b7      	b.n	3000c45e <__udivmoddi4+0x112>
3000c4ee:	4543      	cmp	r3, r8
3000c4f0:	d905      	bls.n	3000c4fe <__udivmoddi4+0x1b2>
3000c4f2:	b10e      	cbz	r6, 3000c4f8 <__udivmoddi4+0x1ac>
3000c4f4:	e9c6 0800 	strd	r0, r8, [r6]
3000c4f8:	2100      	movs	r1, #0
3000c4fa:	4608      	mov	r0, r1
3000c4fc:	e776      	b.n	3000c3ec <__udivmoddi4+0xa0>
3000c4fe:	fab3 fc83 	clz	ip, r3
3000c502:	f1bc 0f00 	cmp.w	ip, #0
3000c506:	d110      	bne.n	3000c52a <__udivmoddi4+0x1de>
3000c508:	4543      	cmp	r3, r8
3000c50a:	d301      	bcc.n	3000c510 <__udivmoddi4+0x1c4>
3000c50c:	4282      	cmp	r2, r0
3000c50e:	d80a      	bhi.n	3000c526 <__udivmoddi4+0x1da>
3000c510:	1a84      	subs	r4, r0, r2
3000c512:	eb68 0303 	sbc.w	r3, r8, r3
3000c516:	2001      	movs	r0, #1
3000c518:	469e      	mov	lr, r3
3000c51a:	2e00      	cmp	r6, #0
3000c51c:	d068      	beq.n	3000c5f0 <__udivmoddi4+0x2a4>
3000c51e:	e9c6 4e00 	strd	r4, lr, [r6]
3000c522:	2100      	movs	r1, #0
3000c524:	e762      	b.n	3000c3ec <__udivmoddi4+0xa0>
3000c526:	4660      	mov	r0, ip
3000c528:	e7f7      	b.n	3000c51a <__udivmoddi4+0x1ce>
3000c52a:	f1cc 0e20 	rsb	lr, ip, #32
3000c52e:	fa03 f30c 	lsl.w	r3, r3, ip
3000c532:	fa02 f50c 	lsl.w	r5, r2, ip
3000c536:	fa00 f70c 	lsl.w	r7, r0, ip
3000c53a:	fa22 f40e 	lsr.w	r4, r2, lr
3000c53e:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c542:	fa20 f10e 	lsr.w	r1, r0, lr
3000c546:	431c      	orrs	r4, r3
3000c548:	fa08 f30c 	lsl.w	r3, r8, ip
3000c54c:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c550:	4319      	orrs	r1, r3
3000c552:	b2a3      	uxth	r3, r4
3000c554:	0c0a      	lsrs	r2, r1, #16
3000c556:	fbbb f8f9 	udiv	r8, fp, r9
3000c55a:	fb09 bb18 	mls	fp, r9, r8, fp
3000c55e:	fb08 fa03 	mul.w	sl, r8, r3
3000c562:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c566:	4592      	cmp	sl, r2
3000c568:	d93e      	bls.n	3000c5e8 <__udivmoddi4+0x29c>
3000c56a:	18a2      	adds	r2, r4, r2
3000c56c:	f108 30ff 	add.w	r0, r8, #4294967295
3000c570:	d204      	bcs.n	3000c57c <__udivmoddi4+0x230>
3000c572:	4592      	cmp	sl, r2
3000c574:	d902      	bls.n	3000c57c <__udivmoddi4+0x230>
3000c576:	f1a8 0002 	sub.w	r0, r8, #2
3000c57a:	4422      	add	r2, r4
3000c57c:	eba2 020a 	sub.w	r2, r2, sl
3000c580:	b289      	uxth	r1, r1
3000c582:	fbb2 f8f9 	udiv	r8, r2, r9
3000c586:	fb09 2218 	mls	r2, r9, r8, r2
3000c58a:	fb08 f303 	mul.w	r3, r8, r3
3000c58e:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c592:	4293      	cmp	r3, r2
3000c594:	d92a      	bls.n	3000c5ec <__udivmoddi4+0x2a0>
3000c596:	18a2      	adds	r2, r4, r2
3000c598:	f108 31ff 	add.w	r1, r8, #4294967295
3000c59c:	d204      	bcs.n	3000c5a8 <__udivmoddi4+0x25c>
3000c59e:	4293      	cmp	r3, r2
3000c5a0:	d902      	bls.n	3000c5a8 <__udivmoddi4+0x25c>
3000c5a2:	f1a8 0102 	sub.w	r1, r8, #2
3000c5a6:	4422      	add	r2, r4
3000c5a8:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c5ac:	1ad2      	subs	r2, r2, r3
3000c5ae:	fba0 9805 	umull	r9, r8, r0, r5
3000c5b2:	4542      	cmp	r2, r8
3000c5b4:	464b      	mov	r3, r9
3000c5b6:	4641      	mov	r1, r8
3000c5b8:	d302      	bcc.n	3000c5c0 <__udivmoddi4+0x274>
3000c5ba:	d106      	bne.n	3000c5ca <__udivmoddi4+0x27e>
3000c5bc:	454f      	cmp	r7, r9
3000c5be:	d204      	bcs.n	3000c5ca <__udivmoddi4+0x27e>
3000c5c0:	3801      	subs	r0, #1
3000c5c2:	ebb9 0305 	subs.w	r3, r9, r5
3000c5c6:	eb68 0104 	sbc.w	r1, r8, r4
3000c5ca:	b18e      	cbz	r6, 3000c5f0 <__udivmoddi4+0x2a4>
3000c5cc:	1afc      	subs	r4, r7, r3
3000c5ce:	eb62 0701 	sbc.w	r7, r2, r1
3000c5d2:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c5d6:	fa24 f30c 	lsr.w	r3, r4, ip
3000c5da:	fa27 f70c 	lsr.w	r7, r7, ip
3000c5de:	ea4e 0303 	orr.w	r3, lr, r3
3000c5e2:	e9c6 3700 	strd	r3, r7, [r6]
3000c5e6:	e79c      	b.n	3000c522 <__udivmoddi4+0x1d6>
3000c5e8:	4640      	mov	r0, r8
3000c5ea:	e7c7      	b.n	3000c57c <__udivmoddi4+0x230>
3000c5ec:	4641      	mov	r1, r8
3000c5ee:	e7db      	b.n	3000c5a8 <__udivmoddi4+0x25c>
3000c5f0:	4631      	mov	r1, r6
3000c5f2:	e6fb      	b.n	3000c3ec <__udivmoddi4+0xa0>

3000c5f4 <__aeabi_idiv0>:
3000c5f4:	4770      	bx	lr
3000c5f6:	bf00      	nop

3000c5f8 <_mbtowc_r>:
3000c5f8:	b430      	push	{r4, r5}
3000c5fa:	4c06      	ldr	r4, [pc, #24]	; (3000c614 <_mbtowc_r+0x1c>)
3000c5fc:	4d06      	ldr	r5, [pc, #24]	; (3000c618 <_mbtowc_r+0x20>)
3000c5fe:	6824      	ldr	r4, [r4, #0]
3000c600:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c602:	2c00      	cmp	r4, #0
3000c604:	bf08      	it	eq
3000c606:	462c      	moveq	r4, r5
3000c608:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c60c:	46a4      	mov	ip, r4
3000c60e:	bc30      	pop	{r4, r5}
3000c610:	4760      	bx	ip
3000c612:	bf00      	nop
3000c614:	3000fd30 	.word	0x3000fd30
3000c618:	3000fbc0 	.word	0x3000fbc0

3000c61c <__ascii_mbtowc>:
3000c61c:	b082      	sub	sp, #8
3000c61e:	b901      	cbnz	r1, 3000c622 <__ascii_mbtowc+0x6>
3000c620:	a901      	add	r1, sp, #4
3000c622:	b142      	cbz	r2, 3000c636 <__ascii_mbtowc+0x1a>
3000c624:	b14b      	cbz	r3, 3000c63a <__ascii_mbtowc+0x1e>
3000c626:	7813      	ldrb	r3, [r2, #0]
3000c628:	600b      	str	r3, [r1, #0]
3000c62a:	7812      	ldrb	r2, [r2, #0]
3000c62c:	1e10      	subs	r0, r2, #0
3000c62e:	bf18      	it	ne
3000c630:	2001      	movne	r0, #1
3000c632:	b002      	add	sp, #8
3000c634:	4770      	bx	lr
3000c636:	4610      	mov	r0, r2
3000c638:	e7fb      	b.n	3000c632 <__ascii_mbtowc+0x16>
3000c63a:	f06f 0001 	mvn.w	r0, #1
3000c63e:	e7f8      	b.n	3000c632 <__ascii_mbtowc+0x16>

3000c640 <_wctomb_r>:
3000c640:	b430      	push	{r4, r5}
3000c642:	4c06      	ldr	r4, [pc, #24]	; (3000c65c <_wctomb_r+0x1c>)
3000c644:	4d06      	ldr	r5, [pc, #24]	; (3000c660 <_wctomb_r+0x20>)
3000c646:	6824      	ldr	r4, [r4, #0]
3000c648:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c64a:	2c00      	cmp	r4, #0
3000c64c:	bf08      	it	eq
3000c64e:	462c      	moveq	r4, r5
3000c650:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c654:	46a4      	mov	ip, r4
3000c656:	bc30      	pop	{r4, r5}
3000c658:	4760      	bx	ip
3000c65a:	bf00      	nop
3000c65c:	3000fd30 	.word	0x3000fd30
3000c660:	3000fbc0 	.word	0x3000fbc0

3000c664 <__ascii_wctomb>:
3000c664:	b149      	cbz	r1, 3000c67a <__ascii_wctomb+0x16>
3000c666:	2aff      	cmp	r2, #255	; 0xff
3000c668:	bf8d      	iteet	hi
3000c66a:	238a      	movhi	r3, #138	; 0x8a
3000c66c:	2001      	movls	r0, #1
3000c66e:	700a      	strbls	r2, [r1, #0]
3000c670:	6003      	strhi	r3, [r0, #0]
3000c672:	bf88      	it	hi
3000c674:	f04f 30ff 	movhi.w	r0, #4294967295
3000c678:	4770      	bx	lr
3000c67a:	4608      	mov	r0, r1
3000c67c:	4770      	bx	lr
3000c67e:	bf00      	nop

3000c680 <____wrap_strlen_veneer>:
3000c680:	b401      	push	{r0}
3000c682:	4802      	ldr	r0, [pc, #8]	; (3000c68c <____wrap_strlen_veneer+0xc>)
3000c684:	4684      	mov	ip, r0
3000c686:	bc01      	pop	{r0}
3000c688:	4760      	bx	ip
3000c68a:	bf00      	nop
3000c68c:	00012f25 	.word	0x00012f25

3000c690 <__INT_HardFault_C_veneer>:
3000c690:	b401      	push	{r0}
3000c692:	4802      	ldr	r0, [pc, #8]	; (3000c69c <__INT_HardFault_C_veneer+0xc>)
3000c694:	4684      	mov	ip, r0
3000c696:	bc01      	pop	{r0}
3000c698:	4760      	bx	ip
3000c69a:	bf00      	nop
3000c69c:	0000d8ed 	.word	0x0000d8ed

3000c6a0 <__SBOOT_Validate_ImgHash_veneer>:
3000c6a0:	b401      	push	{r0}
3000c6a2:	4802      	ldr	r0, [pc, #8]	; (3000c6ac <__SBOOT_Validate_ImgHash_veneer+0xc>)
3000c6a4:	4684      	mov	ip, r0
3000c6a6:	bc01      	pop	{r0}
3000c6a8:	4760      	bx	ip
3000c6aa:	bf00      	nop
3000c6ac:	00003541 	.word	0x00003541

3000c6b0 <____wrap_memset_veneer>:
3000c6b0:	b401      	push	{r0}
3000c6b2:	4802      	ldr	r0, [pc, #8]	; (3000c6bc <____wrap_memset_veneer+0xc>)
3000c6b4:	4684      	mov	ip, r0
3000c6b6:	bc01      	pop	{r0}
3000c6b8:	4760      	bx	ip
3000c6ba:	bf00      	nop
3000c6bc:	00012efd 	.word	0x00012efd

3000c6c0 <__SBOOT_Validate_PubKey_veneer>:
3000c6c0:	b401      	push	{r0}
3000c6c2:	4802      	ldr	r0, [pc, #8]	; (3000c6cc <__SBOOT_Validate_PubKey_veneer+0xc>)
3000c6c4:	4684      	mov	ip, r0
3000c6c6:	bc01      	pop	{r0}
3000c6c8:	4760      	bx	ip
3000c6ca:	bf00      	nop
3000c6cc:	00003495 	.word	0x00003495

3000c6d0 <__SBOOT_Validate_Signature_veneer>:
3000c6d0:	b401      	push	{r0}
3000c6d2:	4802      	ldr	r0, [pc, #8]	; (3000c6dc <__SBOOT_Validate_Signature_veneer+0xc>)
3000c6d4:	4684      	mov	ip, r0
3000c6d6:	bc01      	pop	{r0}
3000c6d8:	4760      	bx	ip
3000c6da:	bf00      	nop
3000c6dc:	00003431 	.word	0x00003431

3000c6e0 <____wrap_memcpy_veneer>:
3000c6e0:	b401      	push	{r0}
3000c6e2:	4802      	ldr	r0, [pc, #8]	; (3000c6ec <____wrap_memcpy_veneer+0xc>)
3000c6e4:	4684      	mov	ip, r0
3000c6e6:	bc01      	pop	{r0}
3000c6e8:	4760      	bx	ip
3000c6ea:	bf00      	nop
3000c6ec:	00012f05 	.word	0x00012f05

3000c6f0 <__SBOOT_Validate_Algorithm_veneer>:
3000c6f0:	b401      	push	{r0}
3000c6f2:	4802      	ldr	r0, [pc, #8]	; (3000c6fc <__SBOOT_Validate_Algorithm_veneer+0xc>)
3000c6f4:	4684      	mov	ip, r0
3000c6f6:	bc01      	pop	{r0}
3000c6f8:	4760      	bx	ip
3000c6fa:	bf00      	nop
3000c6fc:	000033ed 	.word	0x000033ed

3000c700 <__io_assert_failed_veneer>:
3000c700:	b401      	push	{r0}
3000c702:	4802      	ldr	r0, [pc, #8]	; (3000c70c <__io_assert_failed_veneer+0xc>)
3000c704:	4684      	mov	ip, r0
3000c706:	bc01      	pop	{r0}
3000c708:	4760      	bx	ip
3000c70a:	bf00      	nop
3000c70c:	0000992d 	.word	0x0000992d

3000c710 <__DiagPrintf_veneer>:
3000c710:	b401      	push	{r0}
3000c712:	4802      	ldr	r0, [pc, #8]	; (3000c71c <__DiagPrintf_veneer+0xc>)
3000c714:	4684      	mov	ip, r0
3000c716:	bc01      	pop	{r0}
3000c718:	4760      	bx	ip
3000c71a:	bf00      	nop
3000c71c:	0000e5f9 	.word	0x0000e5f9

3000c720 <____wrap_memcmp_veneer>:
3000c720:	b401      	push	{r0}
3000c722:	4802      	ldr	r0, [pc, #8]	; (3000c72c <____wrap_memcmp_veneer+0xc>)
3000c724:	4684      	mov	ip, r0
3000c726:	bc01      	pop	{r0}
3000c728:	4760      	bx	ip
3000c72a:	bf00      	nop
3000c72c:	00012f0d 	.word	0x00012f0d

3000c730 <__SBOOT_GetMdType_veneer>:
3000c730:	b401      	push	{r0}
3000c732:	4802      	ldr	r0, [pc, #8]	; (3000c73c <__SBOOT_GetMdType_veneer+0xc>)
3000c734:	4684      	mov	ip, r0
3000c736:	bc01      	pop	{r0}
3000c738:	4760      	bx	ip
3000c73a:	bf00      	nop
3000c73c:	0000319d 	.word	0x0000319d

3000c740 <__DiagVSprintf_veneer>:
3000c740:	b401      	push	{r0}
3000c742:	4802      	ldr	r0, [pc, #8]	; (3000c74c <__DiagVSprintf_veneer+0xc>)
3000c744:	4684      	mov	ip, r0
3000c746:	bc01      	pop	{r0}
3000c748:	4760      	bx	ip
3000c74a:	bf00      	nop
3000c74c:	0000e331 	.word	0x0000e331
3000c750:	004f4931 	.word	0x004f4931
3000c754:	00004f32 	.word	0x00004f32
3000c758:	004f4932 	.word	0x004f4932
3000c75c:	00004f34 	.word	0x00004f34
3000c760:	004f4934 	.word	0x004f4934
3000c764:	414c460d 	.word	0x414c460d
3000c768:	435f4853 	.word	0x435f4853
3000c76c:	62696c61 	.word	0x62696c61
3000c770:	69746172 	.word	0x69746172
3000c774:	654e6e6f 	.word	0x654e6e6f
3000c778:	78252077 	.word	0x78252077
3000c77c:	2078253a 	.word	0x2078253a
3000c780:	73616870 	.word	0x73616870
3000c784:	68735f65 	.word	0x68735f65
3000c788:	3a746669 	.word	0x3a746669
3000c78c:	000a7825 	.word	0x000a7825
3000c790:	414c460d 	.word	0x414c460d
3000c794:	435f4853 	.word	0x435f4853
3000c798:	62696c61 	.word	0x62696c61
3000c79c:	69746172 	.word	0x69746172
3000c7a0:	654e6e6f 	.word	0x654e6e6f
3000c7a4:	694c2077 	.word	0x694c2077
3000c7a8:	6544656e 	.word	0x6544656e
3000c7ac:	3a79616c 	.word	0x3a79616c
3000c7b0:	000a7825 	.word	0x000a7825
3000c7b4:	414c460d 	.word	0x414c460d
3000c7b8:	435f4853 	.word	0x435f4853
3000c7bc:	62696c61 	.word	0x62696c61
3000c7c0:	69746172 	.word	0x69746172
3000c7c4:	654e6e6f 	.word	0x654e6e6f
3000c7c8:	69772077 	.word	0x69772077
3000c7cc:	776f646e 	.word	0x776f646e
3000c7d0:	74735f31 	.word	0x74735f31
3000c7d4:	3a747261 	.word	0x3a747261
3000c7d8:	77206425 	.word	0x77206425
3000c7dc:	6f646e69 	.word	0x6f646e69
3000c7e0:	735f3177 	.word	0x735f3177
3000c7e4:	3a657a69 	.word	0x3a657a69
3000c7e8:	0a206425 	.word	0x0a206425
3000c7ec:	00000000 	.word	0x00000000
3000c7f0:	414c460d 	.word	0x414c460d
3000c7f4:	435f4853 	.word	0x435f4853
3000c7f8:	62696c61 	.word	0x62696c61
3000c7fc:	69746172 	.word	0x69746172
3000c800:	654e6e6f 	.word	0x654e6e6f
3000c804:	68702077 	.word	0x68702077
3000c808:	5f657361 	.word	0x5f657361
3000c80c:	66696873 	.word	0x66696873
3000c810:	64695f74 	.word	0x64695f74
3000c814:	64253a78 	.word	0x64253a78
3000c818:	00000a20 	.word	0x00000a20
3000c81c:	414c460d 	.word	0x414c460d
3000c820:	435f4853 	.word	0x435f4853
3000c824:	62696c61 	.word	0x62696c61
3000c828:	69746172 	.word	0x69746172
3000c82c:	25206e6f 	.word	0x25206e6f
3000c830:	61732078 	.word	0x61732078
3000c834:	656c706d 	.word	0x656c706d
3000c838:	6168705f 	.word	0x6168705f
3000c83c:	253a6573 	.word	0x253a6573
3000c840:	61722078 	.word	0x61722078
3000c844:	253a6574 	.word	0x253a6574
3000c848:	00000a78 	.word	0x00000a78
3000c84c:	414c460d 	.word	0x414c460d
3000c850:	435f4853 	.word	0x435f4853
3000c854:	62696c61 	.word	0x62696c61
3000c858:	69746172 	.word	0x69746172
3000c85c:	73206e6f 	.word	0x73206e6f
3000c860:	6c706d61 	.word	0x6c706d61
3000c864:	68705f65 	.word	0x68705f65
3000c868:	3a657361 	.word	0x3a657361
3000c86c:	72207825 	.word	0x72207825
3000c870:	3a657461 	.word	0x3a657461
3000c874:	000a7825 	.word	0x000a7825
3000c878:	444f4d5b 	.word	0x444f4d5b
3000c87c:	5f454c55 	.word	0x5f454c55
3000c880:	544f4f42 	.word	0x544f4f42
3000c884:	56454c2d 	.word	0x56454c2d
3000c888:	495f4c45 	.word	0x495f4c45
3000c88c:	5d4f464e 	.word	0x5d4f464e
3000c890:	6c61633a 	.word	0x6c61633a
3000c894:	61726269 	.word	0x61726269
3000c898:	6e6f6974 	.word	0x6e6f6974
3000c89c:	3a6b6f5f 	.word	0x3a6b6f5f
3000c8a0:	3a64255b 	.word	0x3a64255b
3000c8a4:	253a6425 	.word	0x253a6425
3000c8a8:	0a205d64 	.word	0x0a205d64
3000c8ac:	00000000 	.word	0x00000000
3000c8b0:	444f4d5b 	.word	0x444f4d5b
3000c8b4:	5f454c55 	.word	0x5f454c55
3000c8b8:	544f4f42 	.word	0x544f4f42
3000c8bc:	56454c2d 	.word	0x56454c2d
3000c8c0:	495f4c45 	.word	0x495f4c45
3000c8c4:	5d4f464e 	.word	0x5d4f464e
3000c8c8:	414c463a 	.word	0x414c463a
3000c8cc:	43204853 	.word	0x43204853
3000c8d0:	42494c41 	.word	0x42494c41
3000c8d4:	2578305b 	.word	0x2578305b
3000c8d8:	4b4f2078 	.word	0x4b4f2078
3000c8dc:	00000a5d 	.word	0x00000a5d
3000c8e0:	444f4d5b 	.word	0x444f4d5b
3000c8e4:	5f454c55 	.word	0x5f454c55
3000c8e8:	544f4f42 	.word	0x544f4f42
3000c8ec:	56454c2d 	.word	0x56454c2d
3000c8f0:	495f4c45 	.word	0x495f4c45
3000c8f4:	5d4f464e 	.word	0x5d4f464e
3000c8f8:	414c463a 	.word	0x414c463a
3000c8fc:	43204853 	.word	0x43204853
3000c900:	42494c41 	.word	0x42494c41
3000c904:	2578305b 	.word	0x2578305b
3000c908:	41462078 	.word	0x41462078
3000c90c:	0a5d4c49 	.word	0x0a5d4c49
3000c910:	00000000 	.word	0x00000000
3000c914:	444f4d5b 	.word	0x444f4d5b
3000c918:	5f454c55 	.word	0x5f454c55
3000c91c:	544f4f42 	.word	0x544f4f42
3000c920:	56454c2d 	.word	0x56454c2d
3000c924:	495f4c45 	.word	0x495f4c45
3000c928:	5d4f464e 	.word	0x5d4f464e
3000c92c:	616c463a 	.word	0x616c463a
3000c930:	52206873 	.word	0x52206873
3000c934:	20646165 	.word	0x20646165
3000c938:	000a7325 	.word	0x000a7325
3000c93c:	444f4d5b 	.word	0x444f4d5b
3000c940:	5f454c55 	.word	0x5f454c55
3000c944:	544f4f42 	.word	0x544f4f42
3000c948:	56454c2d 	.word	0x56454c2d
3000c94c:	495f4c45 	.word	0x495f4c45
3000c950:	5d4f464e 	.word	0x5d4f464e
3000c954:	616c463a 	.word	0x616c463a
3000c958:	52206873 	.word	0x52206873
3000c95c:	20646165 	.word	0x20646165
3000c960:	202c7325 	.word	0x202c7325
3000c964:	4c494146 	.word	0x4c494146
3000c968:	0000000a 	.word	0x0000000a
3000c96c:	444f4d5b 	.word	0x444f4d5b
3000c970:	5f454c55 	.word	0x5f454c55
3000c974:	544f4f42 	.word	0x544f4f42
3000c978:	56454c2d 	.word	0x56454c2d
3000c97c:	455f4c45 	.word	0x455f4c45
3000c980:	524f5252 	.word	0x524f5252
3000c984:	6c463a5d 	.word	0x6c463a5d
3000c988:	20687361 	.word	0x20687361
3000c98c:	74697753 	.word	0x74697753
3000c990:	52206863 	.word	0x52206863
3000c994:	20646165 	.word	0x20646165
3000c998:	65646f4d 	.word	0x65646f4d
3000c99c:	49414620 	.word	0x49414620
3000c9a0:	00000a4c 	.word	0x00000a4c
3000c9a4:	444f4d5b 	.word	0x444f4d5b
3000c9a8:	5f454c55 	.word	0x5f454c55
3000c9ac:	544f4f42 	.word	0x544f4f42
3000c9b0:	56454c2d 	.word	0x56454c2d
3000c9b4:	495f4c45 	.word	0x495f4c45
3000c9b8:	5d4f464e 	.word	0x5d4f464e
3000c9bc:	616c463a 	.word	0x616c463a
3000c9c0:	49206873 	.word	0x49206873
3000c9c4:	25203a44 	.word	0x25203a44
3000c9c8:	78252d78 	.word	0x78252d78
3000c9cc:	0a78252d 	.word	0x0a78252d
3000c9d0:	00000000 	.word	0x00000000
3000c9d4:	6968540d 	.word	0x6968540d
3000c9d8:	6c662073 	.word	0x6c662073
3000c9dc:	20687361 	.word	0x20687361
3000c9e0:	65707974 	.word	0x65707974
3000c9e4:	20736920 	.word	0x20736920
3000c9e8:	20746f6e 	.word	0x20746f6e
3000c9ec:	70707573 	.word	0x70707573
3000c9f0:	6574726f 	.word	0x6574726f
3000c9f4:	000a2164 	.word	0x000a2164
3000c9f8:	444f4d5b 	.word	0x444f4d5b
3000c9fc:	5f454c55 	.word	0x5f454c55
3000ca00:	544f4f42 	.word	0x544f4f42
3000ca04:	56454c2d 	.word	0x56454c2d
3000ca08:	495f4c45 	.word	0x495f4c45
3000ca0c:	5d4f464e 	.word	0x5d4f464e
3000ca10:	4e414e3a 	.word	0x4e414e3a
3000ca14:	44492044 	.word	0x44492044
3000ca18:	7825203a 	.word	0x7825203a
3000ca1c:	0a78252d 	.word	0x0a78252d
3000ca20:	00000000 	.word	0x00000000
3000ca24:	00003552 	.word	0x00003552
3000ca28:	00003652 	.word	0x00003652
3000ca2c:	00003752 	.word	0x00003752
3000ca30:	00003852 	.word	0x00003852
3000ca34:	00003952 	.word	0x00003952
3000ca38:	00303152 	.word	0x00303152
3000ca3c:	00313152 	.word	0x00313152
3000ca40:	00003152 	.word	0x00003152
3000ca44:	00003252 	.word	0x00003252
3000ca48:	00003352 	.word	0x00003352
3000ca4c:	20304d4b 	.word	0x20304d4b
3000ca50:	20504958 	.word	0x20504958
3000ca54:	00474d49 	.word	0x00474d49
3000ca58:	20304d4b 	.word	0x20304d4b
3000ca5c:	4d415253 	.word	0x4d415253
3000ca60:	00000000 	.word	0x00000000
3000ca64:	20304d4b 	.word	0x20304d4b
3000ca68:	4d415244 	.word	0x4d415244
3000ca6c:	00000000 	.word	0x00000000
3000ca70:	20344d4b 	.word	0x20344d4b
3000ca74:	20504958 	.word	0x20504958
3000ca78:	00474d49 	.word	0x00474d49
3000ca7c:	20344d4b 	.word	0x20344d4b
3000ca80:	4d415253 	.word	0x4d415253
3000ca84:	00000000 	.word	0x00000000
3000ca88:	20344d4b 	.word	0x20344d4b
3000ca8c:	4d415244 	.word	0x4d415244
3000ca90:	00000000 	.word	0x00000000
3000ca94:	42205041 	.word	0x42205041
3000ca98:	5320314c 	.word	0x5320314c
3000ca9c:	004d4152 	.word	0x004d4152
3000caa0:	42205041 	.word	0x42205041
3000caa4:	4420314c 	.word	0x4420314c
3000caa8:	004d4152 	.word	0x004d4152
3000caac:	46205041 	.word	0x46205041
3000cab0:	00005049 	.word	0x00005049
3000cab4:	444f4d5b 	.word	0x444f4d5b
3000cab8:	5f454c55 	.word	0x5f454c55
3000cabc:	544f4f42 	.word	0x544f4f42
3000cac0:	56454c2d 	.word	0x56454c2d
3000cac4:	495f4c45 	.word	0x495f4c45
3000cac8:	5d4f464e 	.word	0x5d4f464e
3000cacc:	2073253a 	.word	0x2073253a
3000cad0:	61766e49 	.word	0x61766e49
3000cad4:	0a64696c 	.word	0x0a64696c
3000cad8:	00000000 	.word	0x00000000
3000cadc:	444f4d5b 	.word	0x444f4d5b
3000cae0:	5f454c55 	.word	0x5f454c55
3000cae4:	544f4f42 	.word	0x544f4f42
3000cae8:	56454c2d 	.word	0x56454c2d
3000caec:	495f4c45 	.word	0x495f4c45
3000caf0:	5d4f464e 	.word	0x5d4f464e
3000caf4:	5b73253a 	.word	0x5b73253a
3000caf8:	78383025 	.word	0x78383025
3000cafc:	5d78253a 	.word	0x5d78253a
3000cb00:	0000000a 	.word	0x0000000a
3000cb04:	444f4d5b 	.word	0x444f4d5b
3000cb08:	5f454c55 	.word	0x5f454c55
3000cb0c:	544f4f42 	.word	0x544f4f42
3000cb10:	56454c2d 	.word	0x56454c2d
3000cb14:	495f4c45 	.word	0x495f4c45
3000cb18:	5d4f464e 	.word	0x5d4f464e
3000cb1c:	5044523a 	.word	0x5044523a
3000cb20:	0a4e4520 	.word	0x0a4e4520
3000cb24:	00000000 	.word	0x00000000
3000cb28:	444f4d5b 	.word	0x444f4d5b
3000cb2c:	5f454c55 	.word	0x5f454c55
3000cb30:	544f4f42 	.word	0x544f4f42
3000cb34:	56454c2d 	.word	0x56454c2d
3000cb38:	495f4c45 	.word	0x495f4c45
3000cb3c:	5d4f464e 	.word	0x5d4f464e
3000cb40:	5044523a 	.word	0x5044523a
3000cb44:	46464f20 	.word	0x46464f20
3000cb48:	0000000a 	.word	0x0000000a
3000cb4c:	444f4d5b 	.word	0x444f4d5b
3000cb50:	5f454c55 	.word	0x5f454c55
3000cb54:	544f4f42 	.word	0x544f4f42
3000cb58:	56454c2d 	.word	0x56454c2d
3000cb5c:	455f4c45 	.word	0x455f4c45
3000cb60:	524f5252 	.word	0x524f5252
3000cb64:	61463a5d 	.word	0x61463a5d
3000cb68:	74206c69 	.word	0x74206c69
3000cb6c:	6f6c206f 	.word	0x6f6c206f
3000cb70:	52206461 	.word	0x52206461
3000cb74:	69205044 	.word	0x69205044
3000cb78:	6567616d 	.word	0x6567616d
3000cb7c:	00000a21 	.word	0x00000a21
3000cb80:	474d490d 	.word	0x474d490d
3000cb84:	544f2032 	.word	0x544f2032
3000cb88:	4e452046 	.word	0x4e452046
3000cb8c:	0000000a 	.word	0x0000000a
3000cb90:	46544f0d 	.word	0x46544f0d
3000cb94:	646f4d20 	.word	0x646f4d20
3000cb98:	72652065 	.word	0x72652065
3000cb9c:	0d726f72 	.word	0x0d726f72
3000cba0:	0000000a 	.word	0x0000000a
3000cba4:	494c4156 	.word	0x494c4156
3000cba8:	00000044 	.word	0x00000044
3000cbac:	41564e49 	.word	0x41564e49
3000cbb0:	0044494c 	.word	0x0044494c
3000cbb4:	74726563 	.word	0x74726563
3000cbb8:	63696669 	.word	0x63696669
3000cbbc:	20657461 	.word	0x20657461
3000cbc0:	20434345 	.word	0x20434345
3000cbc4:	69726576 	.word	0x69726576
3000cbc8:	66207966 	.word	0x66207966
3000cbcc:	2c6c6961 	.word	0x2c6c6961
3000cbd0:	6c6f7220 	.word	0x6c6f7220
3000cbd4:	6361626c 	.word	0x6361626c
3000cbd8:	000a0d6b 	.word	0x000a0d6b
3000cbdc:	444f4d5b 	.word	0x444f4d5b
3000cbe0:	5f454c55 	.word	0x5f454c55
3000cbe4:	544f4f42 	.word	0x544f4f42
3000cbe8:	56454c2d 	.word	0x56454c2d
3000cbec:	495f4c45 	.word	0x495f4c45
3000cbf0:	5d4f464e 	.word	0x5d4f464e
3000cbf4:	474d493a 	.word	0x474d493a
3000cbf8:	544f2832 	.word	0x544f2832
3000cbfc:	29642541 	.word	0x29642541
3000cc00:	2c732520 	.word	0x2c732520
3000cc04:	74657220 	.word	0x74657220
3000cc08:	6425203a 	.word	0x6425203a
3000cc0c:	0000000a 	.word	0x0000000a
3000cc10:	32474d49 	.word	0x32474d49
3000cc14:	43434520 	.word	0x43434520
3000cc18:	72657620 	.word	0x72657620
3000cc1c:	20796669 	.word	0x20796669
3000cc20:	6c696166 	.word	0x6c696166
3000cc24:	6f72202c 	.word	0x6f72202c
3000cc28:	61626c6c 	.word	0x61626c6c
3000cc2c:	0a0d6b63 	.word	0x0a0d6b63
3000cc30:	00000000 	.word	0x00000000
3000cc34:	5f41544f 	.word	0x5f41544f
3000cc38:	45205041 	.word	0x45205041
3000cc3c:	76204343 	.word	0x76204343
3000cc40:	66697265 	.word	0x66697265
3000cc44:	61662079 	.word	0x61662079
3000cc48:	202c6c69 	.word	0x202c6c69
3000cc4c:	6c6c6f72 	.word	0x6c6c6f72
3000cc50:	6b636162 	.word	0x6b636162
3000cc54:	00000a0d 	.word	0x00000a0d
3000cc58:	64616f4c 	.word	0x64616f4c
3000cc5c:	41544f20 	.word	0x41544f20
3000cc60:	2050415f 	.word	0x2050415f
3000cc64:	67616d69 	.word	0x67616d69
3000cc68:	61662065 	.word	0x61662065
3000cc6c:	202c6c69 	.word	0x202c6c69
3000cc70:	6c6c6f72 	.word	0x6c6c6f72
3000cc74:	6b636162 	.word	0x6b636162
3000cc78:	00000a0d 	.word	0x00000a0d
3000cc7c:	444f4d5b 	.word	0x444f4d5b
3000cc80:	5f454c55 	.word	0x5f454c55
3000cc84:	544f4f42 	.word	0x544f4f42
3000cc88:	56454c2d 	.word	0x56454c2d
3000cc8c:	455f4c45 	.word	0x455f4c45
3000cc90:	524f5252 	.word	0x524f5252
3000cc94:	61463a5d 	.word	0x61463a5d
3000cc98:	74206c69 	.word	0x74206c69
3000cc9c:	6f6c206f 	.word	0x6f6c206f
3000cca0:	41206461 	.word	0x41206461
3000cca4:	6d692050 	.word	0x6d692050
3000cca8:	21656761 	.word	0x21656761
3000ccac:	0000000a 	.word	0x0000000a
3000ccb0:	20344d4b 	.word	0x20344d4b
3000ccb4:	33474d49 	.word	0x33474d49
3000ccb8:	00000000 	.word	0x00000000
3000ccbc:	20344d4b 	.word	0x20344d4b
3000ccc0:	0043534e 	.word	0x0043534e
3000ccc4:	31474d49 	.word	0x31474d49
3000ccc8:	00000000 	.word	0x00000000
3000cccc:	32474d49 	.word	0x32474d49
3000ccd0:	00000000 	.word	0x00000000
3000ccd4:	42205041 	.word	0x42205041
3000ccd8:	4626314c 	.word	0x4626314c
3000ccdc:	00005049 	.word	0x00005049
3000cce0:	20504452 	.word	0x20504452
3000cce4:	4c494146 	.word	0x4c494146
3000cce8:	0000000a 	.word	0x0000000a
3000ccec:	20504452 	.word	0x20504452
3000ccf0:	43435553 	.word	0x43435553
3000ccf4:	0a535345 	.word	0x0a535345
3000ccf8:	00000000 	.word	0x00000000
3000ccfc:	059e4eb3 	.word	0x059e4eb3
3000cd00:	4b11ea39 	.word	0x4b11ea39
3000cd04:	2a1b920a 	.word	0x2a1b920a
3000cd08:	29939cee 	.word	0x29939cee
3000cd0c:	00000000 	.word	0x00000000
3000cd10:	42205041 	.word	0x42205041
3000cd14:	0032334c 	.word	0x0032334c
3000cd18:	2073250d 	.word	0x2073250d
3000cd1c:	49524556 	.word	0x49524556
3000cd20:	50205946 	.word	0x50205946
3000cd24:	0a535341 	.word	0x0a535341
3000cd28:	00000000 	.word	0x00000000
3000cd2c:	2073250d 	.word	0x2073250d
3000cd30:	49524556 	.word	0x49524556
3000cd34:	46205946 	.word	0x46205946
3000cd38:	2c4c4941 	.word	0x2c4c4941
3000cd3c:	74657220 	.word	0x74657220
3000cd40:	25203d20 	.word	0x25203d20
3000cd44:	00000a64 	.word	0x00000a64
3000cd48:	7265430d 	.word	0x7265430d
3000cd4c:	69666974 	.word	0x69666974
3000cd50:	65746163 	.word	0x65746163
3000cd54:	52455620 	.word	0x52455620
3000cd58:	20594649 	.word	0x20594649
3000cd5c:	4c494146 	.word	0x4c494146
3000cd60:	6572202c 	.word	0x6572202c
3000cd64:	203d2074 	.word	0x203d2074
3000cd68:	000a6425 	.word	0x000a6425
3000cd6c:	544f4f42 	.word	0x544f4f42
3000cd70:	00000000 	.word	0x00000000
3000cd74:	2d73255b 	.word	0x2d73255b
3000cd78:	48205d41 	.word	0x48205d41
3000cd7c:	20647261 	.word	0x20647261
3000cd80:	6c756146 	.word	0x6c756146
3000cd84:	61502074 	.word	0x61502074
3000cd88:	20686374 	.word	0x20686374
3000cd8c:	63655328 	.word	0x63655328
3000cd90:	29657275 	.word	0x29657275
3000cd94:	00000a0d 	.word	0x00000a0d
3000cd98:	2d73255b 	.word	0x2d73255b
3000cd9c:	45205d41 	.word	0x45205d41
3000cda0:	70656378 	.word	0x70656378
3000cda4:	6e6f6974 	.word	0x6e6f6974
3000cda8:	6f726620 	.word	0x6f726620
3000cdac:	6573206d 	.word	0x6573206d
3000cdb0:	65727563 	.word	0x65727563
3000cdb4:	726f7720 	.word	0x726f7720
3000cdb8:	0d21646c 	.word	0x0d21646c
3000cdbc:	0000000a 	.word	0x0000000a
3000cdc0:	2d73255b 	.word	0x2d73255b
3000cdc4:	45205d41 	.word	0x45205d41
3000cdc8:	70656378 	.word	0x70656378
3000cdcc:	6e6f6974 	.word	0x6e6f6974
3000cdd0:	6f726620 	.word	0x6f726620
3000cdd4:	6f6e206d 	.word	0x6f6e206d
3000cdd8:	65732d6e 	.word	0x65732d6e
3000cddc:	65727563 	.word	0x65727563
3000cde0:	726f7720 	.word	0x726f7720
3000cde4:	0d21646c 	.word	0x0d21646c
3000cde8:	0000000a 	.word	0x0000000a
3000cdec:	2d73255b 	.word	0x2d73255b
3000cdf0:	53205d41 	.word	0x53205d41
3000cdf4:	20525346 	.word	0x20525346
3000cdf8:	30203d20 	.word	0x30203d20
3000cdfc:	38302578 	.word	0x38302578
3000ce00:	000a0d78 	.word	0x000a0d78
3000ce04:	2d73255b 	.word	0x2d73255b
3000ce08:	53205d41 	.word	0x53205d41
3000ce0c:	20524146 	.word	0x20524146
3000ce10:	30203d20 	.word	0x30203d20
3000ce14:	38302578 	.word	0x38302578
3000ce18:	000a0d78 	.word	0x000a0d78
3000ce1c:	2d73255b 	.word	0x2d73255b
3000ce20:	53205d41 	.word	0x53205d41
3000ce24:	52534348 	.word	0x52534348
3000ce28:	30203d20 	.word	0x30203d20
3000ce2c:	38302578 	.word	0x38302578
3000ce30:	000a0d78 	.word	0x000a0d78
3000ce34:	3141544f 	.word	0x3141544f
3000ce38:	45535520 	.word	0x45535520
3000ce3c:	6576202c 	.word	0x6576202c
3000ce40:	6f697372 	.word	0x6f697372
3000ce44:	25203a6e 	.word	0x25203a6e
3000ce48:	00000a64 	.word	0x00000a64
3000ce4c:	3241544f 	.word	0x3241544f
3000ce50:	45535520 	.word	0x45535520
3000ce54:	6576202c 	.word	0x6576202c
3000ce58:	6f697372 	.word	0x6f697372
3000ce5c:	25203a6e 	.word	0x25203a6e
3000ce60:	00000a64 	.word	0x00000a64
3000ce64:	68746f42 	.word	0x68746f42
3000ce68:	41544f20 	.word	0x41544f20
3000ce6c:	616d6920 	.word	0x616d6920
3000ce70:	76206567 	.word	0x76206567
3000ce74:	66697265 	.word	0x66697265
3000ce78:	61662079 	.word	0x61662079
3000ce7c:	64656c69 	.word	0x64656c69
3000ce80:	0000000a 	.word	0x0000000a
3000ce84:	444f4d5b 	.word	0x444f4d5b
3000ce88:	5f454c55 	.word	0x5f454c55
3000ce8c:	544f4f42 	.word	0x544f4f42
3000ce90:	56454c2d 	.word	0x56454c2d
3000ce94:	495f4c45 	.word	0x495f4c45
3000ce98:	5d4f464e 	.word	0x5d4f464e
3000ce9c:	696e493a 	.word	0x696e493a
3000cea0:	42572074 	.word	0x42572074
3000cea4:	00000a0d 	.word	0x00000a0d
3000cea8:	444f4d5b 	.word	0x444f4d5b
3000ceac:	5f454c55 	.word	0x5f454c55
3000ceb0:	544f4f42 	.word	0x544f4f42
3000ceb4:	56454c2d 	.word	0x56454c2d
3000ceb8:	495f4c45 	.word	0x495f4c45
3000cebc:	5d4f464e 	.word	0x5d4f464e
3000cec0:	4965523a 	.word	0x4965523a
3000cec4:	2074696e 	.word	0x2074696e
3000cec8:	0d524444 	.word	0x0d524444
3000cecc:	0000000a 	.word	0x0000000a
3000ced0:	73616c66 	.word	0x73616c66
3000ced4:	69735f68 	.word	0x69735f68
3000ced8:	203a657a 	.word	0x203a657a
3000cedc:	000a4d38 	.word	0x000a4d38
3000cee0:	73616c66 	.word	0x73616c66
3000cee4:	69735f68 	.word	0x69735f68
3000cee8:	203a657a 	.word	0x203a657a
3000ceec:	0a4d3631 	.word	0x0a4d3631
3000cef0:	00000000 	.word	0x00000000
3000cef4:	20315042 	.word	0x20315042
3000cef8:	73726576 	.word	0x73726576
3000cefc:	206e6f69 	.word	0x206e6f69
3000cf00:	61766e69 	.word	0x61766e69
3000cf04:	0a64696c 	.word	0x0a64696c
3000cf08:	00000000 	.word	0x00000000
3000cf0c:	20315042 	.word	0x20315042
3000cf10:	6d726f66 	.word	0x6d726f66
3000cf14:	69207461 	.word	0x69207461
3000cf18:	6c61766e 	.word	0x6c61766e
3000cf1c:	000a6469 	.word	0x000a6469
3000cf20:	20315042 	.word	0x20315042
3000cf24:	69746361 	.word	0x69746361
3000cf28:	69206576 	.word	0x69206576
3000cf2c:	7865646e 	.word	0x7865646e
3000cf30:	766e6920 	.word	0x766e6920
3000cf34:	64696c61 	.word	0x64696c61
3000cf38:	0000000a 	.word	0x0000000a
3000cf3c:	20315042 	.word	0x20315042
3000cf40:	61746164 	.word	0x61746164
3000cf44:	6c617620 	.word	0x6c617620
3000cf48:	202c6469 	.word	0x202c6469
3000cf4c:	73726576 	.word	0x73726576
3000cf50:	3a6e6f69 	.word	0x3a6e6f69
3000cf54:	000a6425 	.word	0x000a6425
3000cf58:	20325042 	.word	0x20325042
3000cf5c:	6d726f66 	.word	0x6d726f66
3000cf60:	69207461 	.word	0x69207461
3000cf64:	6c61766e 	.word	0x6c61766e
3000cf68:	000a6469 	.word	0x000a6469
3000cf6c:	20315042 	.word	0x20315042
3000cf70:	33435243 	.word	0x33435243
3000cf74:	616d2032 	.word	0x616d2032
3000cf78:	2c686374 	.word	0x2c686374
3000cf7c:	74746120 	.word	0x74746120
3000cf80:	65686361 	.word	0x65686361
3000cf84:	52432064 	.word	0x52432064
3000cf88:	30203a43 	.word	0x30203a43
3000cf8c:	2c782578 	.word	0x2c782578
3000cf90:	6c616320 	.word	0x6c616320
3000cf94:	616c7563 	.word	0x616c7563
3000cf98:	20646574 	.word	0x20646574
3000cf9c:	3a435243 	.word	0x3a435243
3000cfa0:	25783020 	.word	0x25783020
3000cfa4:	00000a78 	.word	0x00000a78
3000cfa8:	20315042 	.word	0x20315042
3000cfac:	33435243 	.word	0x33435243
3000cfb0:	6f6e2032 	.word	0x6f6e2032
3000cfb4:	616d2074 	.word	0x616d2074
3000cfb8:	2c686374 	.word	0x2c686374
3000cfbc:	74746120 	.word	0x74746120
3000cfc0:	65686361 	.word	0x65686361
3000cfc4:	52432064 	.word	0x52432064
3000cfc8:	30203a43 	.word	0x30203a43
3000cfcc:	2c782578 	.word	0x2c782578
3000cfd0:	6c616320 	.word	0x6c616320
3000cfd4:	616c7563 	.word	0x616c7563
3000cfd8:	20646574 	.word	0x20646574
3000cfdc:	3a435243 	.word	0x3a435243
3000cfe0:	25783020 	.word	0x25783020
3000cfe4:	00000a78 	.word	0x00000a78
3000cfe8:	20325042 	.word	0x20325042
3000cfec:	33435243 	.word	0x33435243
3000cff0:	616d2032 	.word	0x616d2032
3000cff4:	2c686374 	.word	0x2c686374
3000cff8:	74746120 	.word	0x74746120
3000cffc:	65686361 	.word	0x65686361
3000d000:	52432064 	.word	0x52432064
3000d004:	30203a43 	.word	0x30203a43
3000d008:	2c782578 	.word	0x2c782578
3000d00c:	6c616320 	.word	0x6c616320
3000d010:	616c7563 	.word	0x616c7563
3000d014:	20646574 	.word	0x20646574
3000d018:	3a435243 	.word	0x3a435243
3000d01c:	25783020 	.word	0x25783020
3000d020:	00000a78 	.word	0x00000a78
3000d024:	20325042 	.word	0x20325042
3000d028:	33435243 	.word	0x33435243
3000d02c:	6f6e2032 	.word	0x6f6e2032
3000d030:	616d2074 	.word	0x616d2074
3000d034:	2c686374 	.word	0x2c686374
3000d038:	74746120 	.word	0x74746120
3000d03c:	65686361 	.word	0x65686361
3000d040:	52432064 	.word	0x52432064
3000d044:	30203a43 	.word	0x30203a43
3000d048:	2c782578 	.word	0x2c782578
3000d04c:	6c616320 	.word	0x6c616320
3000d050:	616c7563 	.word	0x616c7563
3000d054:	20646574 	.word	0x20646574
3000d058:	3a435243 	.word	0x3a435243
3000d05c:	25783020 	.word	0x25783020
3000d060:	00000a78 	.word	0x00000a78
3000d064:	20315042 	.word	0x20315042
3000d068:	3161746f 	.word	0x3161746f
3000d06c:	64646120 	.word	0x64646120
3000d070:	73736572 	.word	0x73736572
3000d074:	766e6920 	.word	0x766e6920
3000d078:	64696c61 	.word	0x64696c61
3000d07c:	0000000a 	.word	0x0000000a
3000d080:	20315042 	.word	0x20315042
3000d084:	3261746f 	.word	0x3261746f
3000d088:	64646120 	.word	0x64646120
3000d08c:	73736572 	.word	0x73736572
3000d090:	766e6920 	.word	0x766e6920
3000d094:	64696c61 	.word	0x64696c61
3000d098:	0000000a 	.word	0x0000000a
3000d09c:	68746f42 	.word	0x68746f42
3000d0a0:	20504220 	.word	0x20504220
3000d0a4:	20435243 	.word	0x20435243
3000d0a8:	696c6176 	.word	0x696c6176
3000d0ac:	00000a64 	.word	0x00000a64
3000d0b0:	20315042 	.word	0x20315042
3000d0b4:	656c6573 	.word	0x656c6573
3000d0b8:	64657463 	.word	0x64657463
3000d0bc:	0000000a 	.word	0x0000000a
3000d0c0:	20325042 	.word	0x20325042
3000d0c4:	656c6573 	.word	0x656c6573
3000d0c8:	64657463 	.word	0x64657463
3000d0cc:	0000000a 	.word	0x0000000a
3000d0d0:	68746f42 	.word	0x68746f42
3000d0d4:	20504220 	.word	0x20504220
3000d0d8:	61766e69 	.word	0x61766e69
3000d0dc:	0a64696c 	.word	0x0a64696c
3000d0e0:	00000000 	.word	0x00000000
3000d0e4:	6f636552 	.word	0x6f636552
3000d0e8:	79726576 	.word	0x79726576
3000d0ec:	61747320 	.word	0x61747320
3000d0f0:	000a7472 	.word	0x000a7472
3000d0f4:	69746341 	.word	0x69746341
3000d0f8:	49206576 	.word	0x49206576
3000d0fc:	7865646e 	.word	0x7865646e
3000d100:	6425203a 	.word	0x6425203a
3000d104:	0000000a 	.word	0x0000000a
3000d108:	3141544f 	.word	0x3141544f
3000d10c:	766e6920 	.word	0x766e6920
3000d110:	64696c61 	.word	0x64696c61
3000d114:	0000000a 	.word	0x0000000a
3000d118:	3241544f 	.word	0x3241544f
3000d11c:	766e6920 	.word	0x766e6920
3000d120:	64696c61 	.word	0x64696c61
3000d124:	0000000a 	.word	0x0000000a
3000d128:	3141544f 	.word	0x3141544f
3000d12c:	43524320 	.word	0x43524320
3000d130:	6d203233 	.word	0x6d203233
3000d134:	68637461 	.word	0x68637461
3000d138:	7461202c 	.word	0x7461202c
3000d13c:	68636174 	.word	0x68636174
3000d140:	43206465 	.word	0x43206465
3000d144:	203a4352 	.word	0x203a4352
3000d148:	78257830 	.word	0x78257830
3000d14c:	6163202c 	.word	0x6163202c
3000d150:	6c75636c 	.word	0x6c75636c
3000d154:	64657461 	.word	0x64657461
3000d158:	43524320 	.word	0x43524320
3000d15c:	7830203a 	.word	0x7830203a
3000d160:	000a7825 	.word	0x000a7825
3000d164:	3141544f 	.word	0x3141544f
3000d168:	43524320 	.word	0x43524320
3000d16c:	6e203233 	.word	0x6e203233
3000d170:	6d20746f 	.word	0x6d20746f
3000d174:	68637461 	.word	0x68637461
3000d178:	7461202c 	.word	0x7461202c
3000d17c:	68636174 	.word	0x68636174
3000d180:	43206465 	.word	0x43206465
3000d184:	203a4352 	.word	0x203a4352
3000d188:	78257830 	.word	0x78257830
3000d18c:	6163202c 	.word	0x6163202c
3000d190:	6c75636c 	.word	0x6c75636c
3000d194:	64657461 	.word	0x64657461
3000d198:	43524320 	.word	0x43524320
3000d19c:	7830203a 	.word	0x7830203a
3000d1a0:	202c7825 	.word	0x202c7825
3000d1a4:	73617265 	.word	0x73617265
3000d1a8:	544f2065 	.word	0x544f2065
3000d1ac:	000a3141 	.word	0x000a3141
3000d1b0:	3241544f 	.word	0x3241544f
3000d1b4:	43524320 	.word	0x43524320
3000d1b8:	6d203233 	.word	0x6d203233
3000d1bc:	68637461 	.word	0x68637461
3000d1c0:	7461202c 	.word	0x7461202c
3000d1c4:	68636174 	.word	0x68636174
3000d1c8:	43206465 	.word	0x43206465
3000d1cc:	203a4352 	.word	0x203a4352
3000d1d0:	78257830 	.word	0x78257830
3000d1d4:	6163202c 	.word	0x6163202c
3000d1d8:	6c75636c 	.word	0x6c75636c
3000d1dc:	64657461 	.word	0x64657461
3000d1e0:	43524320 	.word	0x43524320
3000d1e4:	7830203a 	.word	0x7830203a
3000d1e8:	000a7825 	.word	0x000a7825
3000d1ec:	3241544f 	.word	0x3241544f
3000d1f0:	43524320 	.word	0x43524320
3000d1f4:	6e203233 	.word	0x6e203233
3000d1f8:	6d20746f 	.word	0x6d20746f
3000d1fc:	68637461 	.word	0x68637461
3000d200:	7461202c 	.word	0x7461202c
3000d204:	68636174 	.word	0x68636174
3000d208:	43206465 	.word	0x43206465
3000d20c:	203a4352 	.word	0x203a4352
3000d210:	78257830 	.word	0x78257830
3000d214:	6163202c 	.word	0x6163202c
3000d218:	6c75636c 	.word	0x6c75636c
3000d21c:	64657461 	.word	0x64657461
3000d220:	43524320 	.word	0x43524320
3000d224:	7830203a 	.word	0x7830203a
3000d228:	202c7825 	.word	0x202c7825
3000d22c:	73617265 	.word	0x73617265
3000d230:	544f2065 	.word	0x544f2065
3000d234:	000a3241 	.word	0x000a3241
3000d238:	69746341 	.word	0x69746341
3000d23c:	69206576 	.word	0x69206576
3000d240:	7865646e 	.word	0x7865646e
3000d244:	41544f20 	.word	0x41544f20
3000d248:	766e6920 	.word	0x766e6920
3000d24c:	64696c61 	.word	0x64696c61
3000d250:	6572202c 	.word	0x6572202c
3000d254:	74697277 	.word	0x74697277
3000d258:	63612065 	.word	0x63612065
3000d25c:	65766974 	.word	0x65766974
3000d260:	646e6920 	.word	0x646e6920
3000d264:	74207865 	.word	0x74207865
3000d268:	6425206f 	.word	0x6425206f
3000d26c:	0000000a 	.word	0x0000000a
3000d270:	206c6c41 	.word	0x206c6c41
3000d274:	2041544f 	.word	0x2041544f
3000d278:	72646461 	.word	0x72646461
3000d27c:	65737365 	.word	0x65737365
3000d280:	6f6e2073 	.word	0x6f6e2073
3000d284:	61762074 	.word	0x61762074
3000d288:	2c64696c 	.word	0x2c64696c
3000d28c:	63657220 	.word	0x63657220
3000d290:	7265766f 	.word	0x7265766f
3000d294:	61662079 	.word	0x61662079
3000d298:	000a6c69 	.word	0x000a6c69
3000d29c:	20325042 	.word	0x20325042
3000d2a0:	3261746f 	.word	0x3261746f
3000d2a4:	64646120 	.word	0x64646120
3000d2a8:	73736572 	.word	0x73736572
3000d2ac:	766e6920 	.word	0x766e6920
3000d2b0:	64696c61 	.word	0x64696c61
3000d2b4:	0000000a 	.word	0x0000000a
3000d2b8:	20325042 	.word	0x20325042
3000d2bc:	73726576 	.word	0x73726576
3000d2c0:	206e6f69 	.word	0x206e6f69
3000d2c4:	61766e69 	.word	0x61766e69
3000d2c8:	0a64696c 	.word	0x0a64696c
3000d2cc:	00000000 	.word	0x00000000
3000d2d0:	20325042 	.word	0x20325042
3000d2d4:	69746361 	.word	0x69746361
3000d2d8:	69206576 	.word	0x69206576
3000d2dc:	7865646e 	.word	0x7865646e
3000d2e0:	766e6920 	.word	0x766e6920
3000d2e4:	64696c61 	.word	0x64696c61
3000d2e8:	0000000a 	.word	0x0000000a
3000d2ec:	20325042 	.word	0x20325042
3000d2f0:	3161746f 	.word	0x3161746f
3000d2f4:	64646120 	.word	0x64646120
3000d2f8:	73736572 	.word	0x73736572
3000d2fc:	766e6920 	.word	0x766e6920
3000d300:	64696c61 	.word	0x64696c61
3000d304:	0000000a 	.word	0x0000000a
3000d308:	20325042 	.word	0x20325042
3000d30c:	61746164 	.word	0x61746164
3000d310:	6c617620 	.word	0x6c617620
3000d314:	202c6469 	.word	0x202c6469
3000d318:	73726576 	.word	0x73726576
3000d31c:	3a6e6f69 	.word	0x3a6e6f69
3000d320:	000a6425 	.word	0x000a6425
3000d324:	444f4d5b 	.word	0x444f4d5b
3000d328:	5f454c55 	.word	0x5f454c55
3000d32c:	544f4f42 	.word	0x544f4f42
3000d330:	56454c2d 	.word	0x56454c2d
3000d334:	495f4c45 	.word	0x495f4c45
3000d338:	5d4f464e 	.word	0x5d4f464e
3000d33c:	7369443a 	.word	0x7369443a
3000d340:	656c6261 	.word	0x656c6261
3000d344:	304d4b20 	.word	0x304d4b20
3000d348:	6f6f4220 	.word	0x6f6f4220
3000d34c:	69202074 	.word	0x69202074
3000d350:	6554206e 	.word	0x6554206e
3000d354:	6d207473 	.word	0x6d207473
3000d358:	0d65646f 	.word	0x0d65646f
3000d35c:	0000000a 	.word	0x0000000a
3000d360:	444f4d5b 	.word	0x444f4d5b
3000d364:	5f454c55 	.word	0x5f454c55
3000d368:	544f4f42 	.word	0x544f4f42
3000d36c:	56454c2d 	.word	0x56454c2d
3000d370:	495f4c45 	.word	0x495f4c45
3000d374:	5d4f464e 	.word	0x5d4f464e
3000d378:	7079423a 	.word	0x7079423a
3000d37c:	20737361 	.word	0x20737361
3000d380:	2050544f 	.word	0x2050544f
3000d384:	6f747561 	.word	0x6f747561
3000d388:	64616f6c 	.word	0x64616f6c
3000d38c:	00000a0d 	.word	0x00000a0d
3000d390:	444f4d5b 	.word	0x444f4d5b
3000d394:	5f454c55 	.word	0x5f454c55
3000d398:	544f4f42 	.word	0x544f4f42
3000d39c:	56454c2d 	.word	0x56454c2d
3000d3a0:	495f4c45 	.word	0x495f4c45
3000d3a4:	5d4f464e 	.word	0x5d4f464e
3000d3a8:	20504e3a 	.word	0x20504e3a
3000d3ac:	71657246 	.word	0x71657246
3000d3b0:	20642520 	.word	0x20642520
3000d3b4:	0a7a484d 	.word	0x0a7a484d
3000d3b8:	00000000 	.word	0x00000000
3000d3bc:	444f4d5b 	.word	0x444f4d5b
3000d3c0:	5f454c55 	.word	0x5f454c55
3000d3c4:	544f4f42 	.word	0x544f4f42
3000d3c8:	56454c2d 	.word	0x56454c2d
3000d3cc:	495f4c45 	.word	0x495f4c45
3000d3d0:	5d4f464e 	.word	0x5d4f464e
3000d3d4:	2050413a 	.word	0x2050413a
3000d3d8:	71657246 	.word	0x71657246
3000d3dc:	20642520 	.word	0x20642520
3000d3e0:	0a7a484d 	.word	0x0a7a484d
3000d3e4:	00000000 	.word	0x00000000
3000d3e8:	7361700a 	.word	0x7361700a
3000d3ec:	726f7773 	.word	0x726f7773
3000d3f0:	65762064 	.word	0x65762064
3000d3f4:	79666972 	.word	0x79666972
3000d3f8:	69616620 	.word	0x69616620
3000d3fc:	00000a6c 	.word	0x00000a6c
3000d400:	7361700a 	.word	0x7361700a
3000d404:	726f7773 	.word	0x726f7773
3000d408:	65762064 	.word	0x65762064
3000d40c:	79666972 	.word	0x79666972
3000d410:	63757320 	.word	0x63757320
3000d414:	73736563 	.word	0x73736563
3000d418:	0000000a 	.word	0x0000000a
3000d41c:	31474d49 	.word	0x31474d49
3000d420:	4f425320 	.word	0x4f425320
3000d424:	4f20544f 	.word	0x4f20544f
3000d428:	000a4646 	.word	0x000a4646
3000d42c:	31474d49 	.word	0x31474d49
3000d430:	4f425320 	.word	0x4f425320
3000d434:	5320544f 	.word	0x5320544f
3000d438:	45434355 	.word	0x45434355
3000d43c:	000a5353 	.word	0x000a5353
3000d440:	746f6f62 	.word	0x746f6f62
3000d444:	64616f6c 	.word	0x64616f6c
3000d448:	765f7265 	.word	0x765f7265
3000d44c:	69737265 	.word	0x69737265
3000d450:	25206e6f 	.word	0x25206e6f
3000d454:	00000a73 	.word	0x00000a73
3000d458:	444f4d5b 	.word	0x444f4d5b
3000d45c:	5f454c55 	.word	0x5f454c55
3000d460:	544f4f42 	.word	0x544f4f42
3000d464:	56454c2d 	.word	0x56454c2d
3000d468:	495f4c45 	.word	0x495f4c45
3000d46c:	5d4f464e 	.word	0x5d4f464e
3000d470:	474d493a 	.word	0x474d493a
3000d474:	4e452031 	.word	0x4e452031
3000d478:	20524554 	.word	0x20524554
3000d47c:	3a50534d 	.word	0x3a50534d
3000d480:	3830255b 	.word	0x3830255b
3000d484:	000a5d78 	.word	0x000a5d78
3000d488:	444f4d5b 	.word	0x444f4d5b
3000d48c:	5f454c55 	.word	0x5f454c55
3000d490:	544f4f42 	.word	0x544f4f42
3000d494:	56454c2d 	.word	0x56454c2d
3000d498:	495f4c45 	.word	0x495f4c45
3000d49c:	5d4f464e 	.word	0x5d4f464e
3000d4a0:	474d493a 	.word	0x474d493a
3000d4a4:	45532031 	.word	0x45532031
3000d4a8:	45525543 	.word	0x45525543
3000d4ac:	41545320 	.word	0x41545320
3000d4b0:	203a4554 	.word	0x203a4554
3000d4b4:	000a6425 	.word	0x000a6425
3000d4b8:	444f4d5b 	.word	0x444f4d5b
3000d4bc:	5f454c55 	.word	0x5f454c55
3000d4c0:	544f4f42 	.word	0x544f4f42
3000d4c4:	56454c2d 	.word	0x56454c2d
3000d4c8:	495f4c45 	.word	0x495f4c45
3000d4cc:	5d4f464e 	.word	0x5d4f464e
3000d4d0:	696e493a 	.word	0x696e493a
3000d4d4:	53502074 	.word	0x53502074
3000d4d8:	0d4d4152 	.word	0x0d4d4152
3000d4dc:	0000000a 	.word	0x0000000a
3000d4e0:	7465730d 	.word	0x7465730d
3000d4e4:	6d667020 	.word	0x6d667020
3000d4e8:	69616620 	.word	0x69616620
3000d4ec:	000a0d6c 	.word	0x000a0d6c
3000d4f0:	444f4d5b 	.word	0x444f4d5b
3000d4f4:	5f454c55 	.word	0x5f454c55
3000d4f8:	544f4f42 	.word	0x544f4f42
3000d4fc:	56454c2d 	.word	0x56454c2d
3000d500:	495f4c45 	.word	0x495f4c45
3000d504:	5d4f464e 	.word	0x5d4f464e
3000d508:	5252453a 	.word	0x5252453a
3000d50c:	2121524f 	.word	0x2121524f
3000d510:	6f685320 	.word	0x6f685320
3000d514:	20646c75 	.word	0x20646c75
3000d518:	20746f4e 	.word	0x20746f4e
3000d51c:	62616e65 	.word	0x62616e65
3000d520:	4d20656c 	.word	0x4d20656c
3000d524:	77536d65 	.word	0x77536d65
3000d528:	6e4f5f72 	.word	0x6e4f5f72
3000d52c:	6920796c 	.word	0x6920796c
3000d530:	4444206e 	.word	0x4444206e
3000d534:	68432052 	.word	0x68432052
3000d538:	21217069 	.word	0x21217069
3000d53c:	000a0d21 	.word	0x000a0d21
3000d540:	444f4d5b 	.word	0x444f4d5b
3000d544:	5f454c55 	.word	0x5f454c55
3000d548:	544f4f42 	.word	0x544f4f42
3000d54c:	56454c2d 	.word	0x56454c2d
3000d550:	495f4c45 	.word	0x495f4c45
3000d554:	5d4f464e 	.word	0x5d4f464e
3000d558:	696e493a 	.word	0x696e493a
3000d55c:	44442074 	.word	0x44442074
3000d560:	000a0d52 	.word	0x000a0d52
3000d564:	20445753 	.word	0x20445753
3000d568:	64207369 	.word	0x64207369
3000d56c:	62617369 	.word	0x62617369
3000d570:	2c64656c 	.word	0x2c64656c
3000d574:	6f727020 	.word	0x6f727020
3000d578:	64656563 	.word	0x64656563
3000d57c:	206f7420 	.word	0x206f7420
3000d580:	61736964 	.word	0x61736964
3000d584:	20656c62 	.word	0x20656c62
3000d588:	74726155 	.word	0x74726155
3000d58c:	776f4420 	.word	0x776f4420
3000d590:	616f6c6e 	.word	0x616f6c6e
3000d594:	2e2e2e64 	.word	0x2e2e2e64
3000d598:	0000000a 	.word	0x0000000a
3000d59c:	74726155 	.word	0x74726155
3000d5a0:	776f4420 	.word	0x776f4420
3000d5a4:	616f6c6e 	.word	0x616f6c6e
3000d5a8:	61682064 	.word	0x61682064
3000d5ac:	65622073 	.word	0x65622073
3000d5b0:	64206e65 	.word	0x64206e65
3000d5b4:	62617369 	.word	0x62617369
3000d5b8:	0a64656c 	.word	0x0a64656c
3000d5bc:	00000000 	.word	0x00000000
3000d5c0:	55676f4c 	.word	0x55676f4c
3000d5c4:	20747261 	.word	0x20747261
3000d5c8:	64756142 	.word	0x64756142
3000d5cc:	65746172 	.word	0x65746172
3000d5d0:	6425203a 	.word	0x6425203a
3000d5d4:	0000000a 	.word	0x0000000a
3000d5d8:	4848530a 	.word	0x4848530a
3000d5dc:	4946544f 	.word	0x4946544f
3000d5e0:	6f6e2058 	.word	0x6f6e2058
3000d5e4:	65722074 	.word	0x65722074
3000d5e8:	76696563 	.word	0x76696563
3000d5ec:	000a6465 	.word	0x000a6465
3000d5f0:	726f4e0a 	.word	0x726f4e0a
3000d5f4:	206c616d 	.word	0x206c616d
3000d5f8:	746f6f62 	.word	0x746f6f62
3000d5fc:	0000000a 	.word	0x0000000a
3000d600:	5f6e6962 	.word	0x5f6e6962
3000d604:	313e6f6e 	.word	0x313e6f6e
3000d608:	3d202c30 	.word	0x3d202c30
3000d60c:	0a642520 	.word	0x0a642520
3000d610:	00000000 	.word	0x00000000
3000d614:	6e206164 	.word	0x6e206164
3000d618:	7220746f 	.word	0x7220746f
3000d61c:	69656365 	.word	0x69656365
3000d620:	0a646576 	.word	0x0a646576
3000d624:	00000000 	.word	0x00000000
3000d628:	746f680a 	.word	0x746f680a
3000d62c:	20786966 	.word	0x20786966
3000d630:	656e6f64 	.word	0x656e6f64
3000d634:	202e2e2e 	.word	0x202e2e2e
3000d638:	6f626552 	.word	0x6f626552
3000d63c:	6e69746f 	.word	0x6e69746f
3000d640:	0a2e2e67 	.word	0x0a2e2e67
3000d644:	00000000 	.word	0x00000000
3000d648:	444f4d5b 	.word	0x444f4d5b
3000d64c:	5f454c55 	.word	0x5f454c55
3000d650:	544f4f42 	.word	0x544f4f42
3000d654:	56454c2d 	.word	0x56454c2d
3000d658:	495f4c45 	.word	0x495f4c45
3000d65c:	5d4f464e 	.word	0x5d4f464e
3000d660:	6174533a 	.word	0x6174533a
3000d664:	4e207472 	.word	0x4e207472
3000d668:	65536e6f 	.word	0x65536e6f
3000d66c:	65727563 	.word	0x65727563
3000d670:	30204020 	.word	0x30204020
3000d674:	20782578 	.word	0x20782578
3000d678:	0d2e2e2e 	.word	0x0d2e2e2e
3000d67c:	0000000a 	.word	0x0000000a
3000d680:	6c630a0d 	.word	0x6c630a0d
3000d684:	2065736f 	.word	0x2065736f
3000d688:	646f4d79 	.word	0x646f4d79
3000d68c:	54206d65 	.word	0x54206d65
3000d690:	736e6172 	.word	0x736e6172
3000d694:	20726566 	.word	0x20726566
3000d698:	0d2e2e2e 	.word	0x0d2e2e2e
3000d69c:	0000000a 	.word	0x0000000a
3000d6a0:	50494843 	.word	0x50494843
3000d6a4:	4f464e49 	.word	0x4f464e49
3000d6a8:	00000000 	.word	0x00000000
3000d6ac:	2d73255b 	.word	0x2d73255b
3000d6b0:	49205d45 	.word	0x49205d45
3000d6b4:	6c61766e 	.word	0x6c61766e
3000d6b8:	43206469 	.word	0x43206469
3000d6bc:	696e6968 	.word	0x696e6968
3000d6c0:	216f666e 	.word	0x216f666e
3000d6c4:	65684320 	.word	0x65684320
3000d6c8:	4f206b63 	.word	0x4f206b63
3000d6cc:	000a5054 	.word	0x000a5054
3000d6d0:	2d73255b 	.word	0x2d73255b
3000d6d4:	50205d57 	.word	0x50205d57
3000d6d8:	7361656c 	.word	0x7361656c
3000d6dc:	72702065 	.word	0x72702065
3000d6e0:	6172676f 	.word	0x6172676f
3000d6e4:	6863206d 	.word	0x6863206d
3000d6e8:	6e697069 	.word	0x6e697069
3000d6ec:	69206f66 	.word	0x69206f66
3000d6f0:	544f206e 	.word	0x544f206e
3000d6f4:	0a212050 	.word	0x0a212050
3000d6f8:	00000000 	.word	0x00000000

3000d6fc <Memory_Info>:
3000d6fc:	03e00007 00060004 010202c0 02010100     ................
3000d70c:	02000102 00040302 02040400 01010102     ................
3000d71c:	01020221 02240401 03000102 00030103     !.....$.........
3000d72c:	01050500 06000002 01020206 01070700     ................
3000d73c:	08000002 01020208 02280801 09000102     ..........(.....
3000d74c:	00040309 03290901 0a000004 0003010a     ......).........
3000d75c:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d76c:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d77c:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d78c:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d79c:	62696c61 6f696172 000a0d6e              alibraion...

3000d7a8 <ddrphy_tx_scan>:
3000d7a8:	00000000 00080000 00080808 00000000     ................
3000d7b8:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d7c8:	07060012 000f0707 080f0f0f 00000008     ................
3000d7d8:	20000001 22222222 000c0012 34000000     ... """".......4
3000d7e8:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d7f8:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d808:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d818:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d828:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d838:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d848:	0e0e001a 00170e0e 12171717 00000012     ................
3000d858:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d868:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d878:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d888:	0f0f001b 00180f0f 13181818 00000013     ................
3000d898:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d8a8:	1110001b 00191111 13191919 00000013     ................
3000d8b8:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d8c8:	1313001c 001c1313 121c1c1c 00000012     ................
3000d8d8:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d8e8 <ddrphy_zq_rx_scan>:
	...
3000d8f0:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d900:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d910:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d920:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d930:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d940:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d950:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d960:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d970:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d980:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d990:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d9a0:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d9b0 <Flash_AVL>:
3000d9b0:	000000ef 000000ff 00000000 000043fc     .............C..
3000d9c0:	00000000 000000a1 000000ff 00000000     ................
3000d9d0:	0000fffc 00000000 0000000b 000000ff     ................
3000d9e0:	00000000 000043fc 00000000 0000000e     .....C..........
3000d9f0:	000000ff 00000000 000043fc 00000000     .........C......
3000da00:	000000c8 000000ff 00000001 000043fc     .............C..
3000da10:	00000000 000028c2 0000ffff 00000005     .....(..........
3000da20:	000200fc 00000000 000000c2 000000ff     ................
3000da30:	00000002 000000fc 00000000 00000068     ............h...
3000da40:	000000ff 00000002 000000fc 00000000     ................
3000da50:	00000051 000000ff 00000002 000000fc     Q...............
3000da60:	00000000 0000001c 000000ff 00000003     ................
3000da70:	000000fc 00000000 00000020 000000ff     ........ .......
3000da80:	00000004 000000fc 00000000 00000000     ................
3000da90:	000000ff 000000fe ffffffff 30008821     ............!..0
3000daa0:	000000ff ffffffff 000000ff ffffffff     ................
3000dab0:	00000000                                ....

3000dab4 <Flash_ReadMode>:
3000dab4:	                                         ..

3000dab6 <Flash_Speed>:
3000dab6:	                                         ..

3000dab8 <NAND_AVL>:
3000dab8:	000000ef 000000ff 00000000 ffffffff     ................
3000dac8:	00000000 000000c8 000000ff 00000001     ................
3000dad8:	ffffffff 00000000 000000e5 000000ff     ................
3000dae8:	00000001 ffffffff 00000000 000000c2     ................
3000daf8:	000000ff 00000002 ffffffff 00000000     ................
3000db08:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000db20:	000000ff 000000fe ffffffff 30008881     ...............0
3000db30:	000000ff ffffffff 000000ff ffffffff     ................
3000db40:	00000000 3000c750 3000c754 3000c758     ....P..0T..0X..0
3000db50:	3000c75c 3000c760                       \..0`..0

3000db58 <__FUNCTION__.0>:
3000db58:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000db68 <__FUNCTION__.1>:
3000db68:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000db78:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000db88:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000db98:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000dba8:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000dbb8:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000dbc8:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000dbd8:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000dbe8:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000dbf8:	00000a20 2d73255b 50205d45 5220524d      ...[%s-E] PMR R
3000dc08:	20646165 6f727265 000a2172 2d73255b     ead error!..[%s-
3000dc18:	4f205d57 575f5054 65746972 61462038     W] OTP_Write8 Fa
3000dc28:	25206c69 000a2078 2d73255b 4f205d45     il %x ..[%s-E] O
3000dc38:	70205054 72676f72 65206d61 726f7272     TP program error
3000dc48:	00000a21 2d73255b 50205d45 2032524d     !...[%s-E] PMR2 
3000dc58:	64616552 72726520 0a21726f 00000000     Read error!.....

3000dc68 <__FUNCTION__.1>:
3000dc68:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000dc78:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000dc88:	495f4c45 5d4f464e 5253503a 43204d41     EL_INFO]:PSRAM C
3000dc98:	206c7274 3a4b4c43 20642520 0a207a48     trl CLK: %d Hz .
3000dca8:	00000000 504d4f52 48435441 00000000     ....ROMPATCH....
3000dcb8:	2d73255b 4e205d45 6874206f 61207369     [%s-E] No this a
3000dcc8:	5f687475 65707974 29642528 726f6620     uth_type(%d) for
3000dcd8:	41485320 00000a32 2d73255b 69205d57      SHA2...[%s-W] i
3000dce8:	255b7172 70205d64 726f6972 20797469     rq[%d] priority 
3000dcf8:	73206425 6c6c6168 203d3c20 000a6425     %d shall <= %d..

3000dd08 <__FUNCTION__.0>:
3000dd08:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000dd18:	00000000                                ....

3000dd1c <__FUNCTION__.1>:
3000dd1c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000dd2c <__FUNCTION__.3>:
3000dd2c:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000dd3c:	32616873 696e695f 00000074              sha2_init...

3000dd48 <__FUNCTION__.4>:
3000dd48:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000dd58:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000dd68:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000dd78:	6e207369 6320746f 69666e6f 65727567     is not configure
3000dd88:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000dd98:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000dda8:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000ddb8:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000ddc8:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000ddd8:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000dde8:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000ddf8:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000de08:	69625f6b 21217374 00000a21              k_bits!!!...

3000de14 <__FUNCTION__.1>:
3000de14:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000de24:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000de34:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000de44:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000de54:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000de64:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000de74:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000de84:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000de94:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000dea4:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000deb4:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000dec4:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000ded4:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000dee4:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000def4:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000df04:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000df14:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000df24:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000df34:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000df44:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000df54:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000df64:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000df74:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000df84:	00000000                                ....

3000df88 <register_string>:
3000df88:	00000000 3000deb0 3000ca24 3000ca28     .......0$..0(..0
3000df98:	3000ca2c 3000ca30 3000ca34 3000ca38     ,..00..04..08..0
3000dfa8:	3000ca3c 3000deac 3000ca40 3000ca44     <..0...0@..0D..0
3000dfb8:	3000ca48 3000df50 65637845 6f697470     H..0P..0Exceptio
3000dfc8:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000dfd8:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000dfe8:	64657375 20796220 6c696166 76206465     used by failed v
3000dff8:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000e008:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000e018:	00000000 75636553 79746972 75616620     ....Security fau
3000e028:	6920746c 61632073 64657375 20796220     lt is caused by 
3000e038:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000e048:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000e058:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000e068:	79746972 75616620 6920746c 61632073     rity fault is ca
3000e078:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000e088:	65746e69 74697267 69732079 74616e67     integrity signat
3000e098:	00657275 75636553 79746972 75616620     ure.Security fau
3000e0a8:	6920746c 61632073 64657375 20796220     lt is caused by 
3000e0b8:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000e0c8:	6572206e 6e727574 00000000 75636553     n return....Secu
3000e0d8:	79746972 75616620 6920746c 61632073     rity fault is ca
3000e0e8:	64657375 20796220 72747441 74756269     used by Attribut
3000e0f8:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000e108:	00006e6f 75636553 79746972 75616620     on..Security fau
3000e118:	6920746c 61632073 64657375 20796220     lt is caused by 
3000e128:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000e138:	00006e6f 75636553 79746972 75616620     on..Security fau
3000e148:	6920746c 61632073 64657375 20796220     lt is caused by 
3000e158:	797a614c 61747320 70206574 65736572     Lazy state prese
3000e168:	74617672 206e6f69 6f727265 00000072     rvation error...
3000e178:	75636553 79746972 75616620 6920746c     Security fault i
3000e188:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000e198:	61747320 65206574 726f7272 00000000      state error....
3000e1a8:	20656854 75636573 79746972 75616620     The security fau
3000e1b8:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000e1c8:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000e1d8:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e1e8:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000e1f8:	61662074 20746c75 63207369 65737561     t fault is cause
3000e208:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000e218:	63636120 20737365 6c6f6976 6f697461      access violatio
3000e228:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000e238:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000e248:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000e258:	20746c75 63207369 65737561 79622064     ult is caused by
3000e268:	74616420 63612061 73736563 6f697620      data access vio
3000e278:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000e288:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000e298:	63207369 65737561 79622064 736e7520     is caused by uns
3000e2a8:	6b636174 20676e69 6f727265 00000072     tacking error...
3000e2b8:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000e2c8:	61662074 20746c75 63207369 65737561     t fault is cause
3000e2d8:	79622064 61747320 6e696b63 72652067     d by stacking er
3000e2e8:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000e2f8:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000e308:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000e318:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000e328:	70206574 65736572 74617672 006e6f69     te preservation.
3000e338:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000e348:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000e358:	64657272 64646120 73736572 00736920     rred address is.
3000e368:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000e378:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000e388:	73692074 75616320 20646573 69207962     t is caused by i
3000e398:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000e3a8:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000e3b8:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000e3c8:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000e3d8:	75616320 20646573 70207962 69636572      caused by preci
3000e3e8:	64206573 20617461 65636361 76207373     se data access v
3000e3f8:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000e408:	6c756166 73692074 75616320 20646573     fault is caused 
3000e418:	69207962 6572706d 65736963 74616420     by imprecise dat
3000e428:	63612061 73736563 6f697620 6974616c     a access violati
3000e438:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000e448:	75616320 20646573 75207962 6174736e      caused by unsta
3000e458:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000e468:	6c756166 73692074 75616320 20646573     fault is caused 
3000e478:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000e488:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000e498:	75616320 20646573 66207962 74616f6c      caused by float
3000e4a8:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000e4b8:	65746174 65727020 76726573 6f697461     tate preservatio
3000e4c8:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000e4d8:	636f2074 72727563 61206465 65726464     t occurred addre
3000e4e8:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000e4f8:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e508:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e518:	65737561 79622064 74746120 74706d65     aused by attempt
3000e528:	6f742073 65786520 65747563 206e6120     s to execute an 
3000e538:	65646e75 656e6966 6e692064 75727473     undefined instru
3000e548:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000e558:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e568:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e578:	65737561 79622064 74746120 74706d65     aused by attempt
3000e588:	6f742073 69777320 20686374 61206f74     s to switch to a
3000e598:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000e5a8:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e5b8:	61662065 20746c75 63207369 65737561     e fault is cause
3000e5c8:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e5d8:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e5e8:	74697720 20612068 20646162 756c6176      with a bad valu
3000e5f8:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e608:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e618:	61662065 20746c75 63207369 65737561     e fault is cause
3000e628:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e638:	65786520 65747563 63206120 6f72706f      execute a copro
3000e648:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e658:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e668:	63207369 65737561 79622064 646e6920     is caused by ind
3000e678:	74616369 74207365 20746168 74732061     icates that a st
3000e688:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e698:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e6a8:	61742073 206e656b 63616c70 00000065     s taken place...
3000e6b8:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e6c8:	65737561 79622064 646e6920 74616369     aused by indicat
3000e6d8:	74207365 20746168 75206e61 696c616e     es that an unali
3000e6e8:	64656e67 63636120 20737365 6c756166     gned access faul
3000e6f8:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e708:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e718:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e728:	74616369 61207365 76696420 20656469     icates a divide 
3000e738:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e748:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e758:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e768:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e778:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e788:	65737561 79622064 6c616820 65722074     aused by halt re
3000e798:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e7a8:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e7b8:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e7c8:	20746c75 63207369 65737561 79622064     ult is caused by
3000e7d8:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e7e8:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e7f8:	61662067 20746c75 63207369 65737561     g fault is cause
3000e808:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e818:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e828:	20746c75 63207369 65737561 79622064     ult is caused by
3000e838:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e848:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e858:	20746c75 63207369 65737561 79622064     ult is caused by
3000e868:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e878:	72657373 00646574 3000ca4c 3000ca58     sserted.L..0X..0
3000e888:	3000ca64 3000ca70 3000ca7c 3000ca88     d..0p..0|..0...0
3000e898:	3000ca94 3000caa0 3000caac 3000ca94     ...0...0...0...0
3000e8a8:	3000caa0 3000ccb0 3000ccbc 3000ccc4     ...0...0...0...0
3000e8b8:	3000cccc 3000ccb4 3000ccd4 00000208     ...0...0...0....
3000e8c8:	0000020c 00000210 00000008 00000214     ................
3000e8d8:	00000218 0000021c 0000000c              ............

3000e8e4 <__FUNCTION__.0>:
3000e8e4:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000e8f4 <km4_bootloader_rev>:
3000e8f4:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e904:	665f7265 64373532 325f3339 2f333230     er_f257d93_2023/
3000e914:	302f3231 32312d31 3a38313a 00003034     12/01-12:18:40..
3000e924:	2d73255b 2a205d57 2a2a2a2a 2a2a2a2a     [%s-W] *********
3000e934:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e944:	4157202a 4e494e52 2a2a2047 2a2a2a2a     * WARNING ******
3000e954:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e964:	00000a2a 2d73255b 2a205d57 20202020     *...[%s-W] *    
3000e974:	20202020 20202020 20202020 20202020                     
3000e984:	20202020 20202020 20202020 20202020                     
3000e994:	20202020 20202020 20202020 20202020                     
3000e9a4:	20202020 00000a2a 2d73255b 2a205d57         *...[%s-W] *
3000e9b4:	20202020 20202020 20202020 72754320                  Cur
3000e9c4:	746e6572 69686320 65762070 6f697372     rent chip versio
3000e9d4:	7369206e 75434120 21212174 20202020     n is ACut!!!    
3000e9e4:	20202020 20202020 00000a2a 2d73255b             *...[%s-
3000e9f4:	2a205d57 52202020 53204b54 64204b44     W] *   RTK SDK d
3000ea04:	2073656f 20746f6e 70707573 2074726f     oes not support 
3000ea14:	72727563 20746e65 73726576 206e6f69     current version 
3000ea24:	20796e61 65726f6d 2020202e 00000a2a     any more.   *...
3000ea34:	2d73255b 2a205d57 20202020 6c502020     [%s-W] *      Pl
3000ea44:	65736165 6e6f6320 74636174 4b545220     ease contact RTK
3000ea54:	204d5020 20726f66 65726f6d 6f727020      PM for more pro
3000ea64:	74637564 666e6920 2020216f 20202020     duct info!      
3000ea74:	00000a2a 2d73255b 2a205d57 2a2a2a2a     *...[%s-W] *****
3000ea84:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000ea94:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000eaa4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000eab4:	2a2a2a2a 00000a2a 2d73255b 0a205d57     *****...[%s-W] .
3000eac4:	50004300 5849534f                        .C.POSIX...

3000eacf <_ctype_>:
3000eacf:	20202000 20202020 28282020 20282828     .         ((((( 
3000eadf:	20202020 20202020 20202020 20202020                     
3000eaef:	10108820 10101010 10101010 10101010      ...............
3000eaff:	04040410 04040404 10040404 10101010     ................
3000eb0f:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000eb1f:	01010101 01010101 01010101 10101010     ................
3000eb2f:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000eb3f:	02020202 02020202 02020202 10101010     ................
3000eb4f:	00000020 00000000 00000000 00000000      ...............
	...
