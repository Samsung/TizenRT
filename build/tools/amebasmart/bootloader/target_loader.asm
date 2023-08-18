
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
30003272:	f008 fddd 	bl	3000be30 <__DiagPrintf_veneer>
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
300032ea:	f008 fda1 	bl	3000be30 <__DiagPrintf_veneer>
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
30003316:	f008 fd8b 	bl	3000be30 <__DiagPrintf_veneer>
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
30003342:	f008 fd75 	bl	3000be30 <__DiagPrintf_veneer>
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
3000335c:	f008 fd68 	bl	3000be30 <__DiagPrintf_veneer>
30003360:	e7be      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003362:	480e      	ldr	r0, [pc, #56]	; (3000339c <FLASH_CalibrationNew+0x224>)
30003364:	f008 fd64 	bl	3000be30 <__DiagPrintf_veneer>
30003368:	e795      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000336a:	bf00      	nop
3000336c:	3000a149 	.word	0x3000a149
30003370:	2001c01c 	.word	0x2001c01c
30003374:	00009c99 	.word	0x00009c99
30003378:	e000ed00 	.word	0xe000ed00
3000337c:	2001c00c 	.word	0x2001c00c
30003380:	00054060 	.word	0x00054060
30003384:	000129bd 	.word	0x000129bd
30003388:	3000bea0 	.word	0x3000bea0
3000338c:	3000be74 	.word	0x3000be74
30003390:	0000b7e9 	.word	0x0000b7e9
30003394:	ffff0000 	.word	0xffff0000
30003398:	3000bec4 	.word	0x3000bec4
3000339c:	3000bf00 	.word	0x3000bf00
300033a0:	30009f59 	.word	0x30009f59

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
300034c6:	f008 fcb3 	bl	3000be30 <__DiagPrintf_veneer>
300034ca:	e7db      	b.n	30003484 <FLASH_Calibration+0xe0>
300034cc:	4652      	mov	r2, sl
300034ce:	9b02      	ldr	r3, [sp, #8]
300034d0:	480b      	ldr	r0, [pc, #44]	; (30003500 <FLASH_Calibration+0x15c>)
300034d2:	9105      	str	r1, [sp, #20]
300034d4:	f008 fcac 	bl	3000be30 <__DiagPrintf_veneer>
300034d8:	9905      	ldr	r1, [sp, #20]
300034da:	e7b7      	b.n	3000344c <FLASH_Calibration+0xa8>
300034dc:	9600      	str	r6, [sp, #0]
300034de:	e7d1      	b.n	30003484 <FLASH_Calibration+0xe0>
300034e0:	3000a149 	.word	0x3000a149
300034e4:	2001c01c 	.word	0x2001c01c
300034e8:	00009c99 	.word	0x00009c99
300034ec:	e000ed00 	.word	0xe000ed00
300034f0:	30009f59 	.word	0x30009f59
300034f4:	0000b7e9 	.word	0x0000b7e9
300034f8:	2001c00c 	.word	0x2001c00c
300034fc:	3000bf5c 	.word	0x3000bf5c
30003500:	3000bf2c 	.word	0x3000bf2c
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
3000359c:	f008 fc48 	bl	3000be30 <__DiagPrintf_veneer>
300035a0:	e7ec      	b.n	3000357c <_flash_calibration_highspeed+0x74>
300035a2:	bf00      	nop
300035a4:	2001c01c 	.word	0x2001c01c
300035a8:	2001c00c 	.word	0x2001c00c
300035ac:	3000bf88 	.word	0x3000bf88

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
300035e4:	f008 fc24 	bl	3000be30 <__DiagPrintf_veneer>
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
3000362a:	f008 fc01 	bl	3000be30 <__DiagPrintf_veneer>
3000362e:	2001      	movs	r0, #1
30003630:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003632:	bf00      	nop
30003634:	2001c01c 	.word	0x2001c01c
30003638:	0000b569 	.word	0x0000b569
3000363c:	2001c00c 	.word	0x2001c00c
30003640:	3000bff0 	.word	0x3000bff0
30003644:	3000a149 	.word	0x3000a149
30003648:	0000c045 	.word	0x0000c045
3000364c:	0000a53d 	.word	0x0000a53d
30003650:	3000bfc0 	.word	0x3000bfc0

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
3000375a:	f008 fb69 	bl	3000be30 <__DiagPrintf_veneer>
3000375e:	e7cd      	b.n	300036fc <flash_rx_mode_switch+0xa8>
30003760:	469c      	mov	ip, r3
30003762:	f1bc 0f05 	cmp.w	ip, #5
30003766:	d10f      	bne.n	30003788 <flash_rx_mode_switch+0x134>
30003768:	4b12      	ldr	r3, [pc, #72]	; (300037b4 <flash_rx_mode_switch+0x160>)
3000376a:	6818      	ldr	r0, [r3, #0]
3000376c:	f010 0002 	ands.w	r0, r0, #2
30003770:	d00b      	beq.n	3000378a <flash_rx_mode_switch+0x136>
30003772:	4815      	ldr	r0, [pc, #84]	; (300037c8 <flash_rx_mode_switch+0x174>)
30003774:	f008 fb5c 	bl	3000be30 <__DiagPrintf_veneer>
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
3000379c:	f008 fb48 	bl	3000be30 <__DiagPrintf_veneer>
300037a0:	e7f2      	b.n	30003788 <flash_rx_mode_switch+0x134>
300037a2:	bf00      	nop
300037a4:	3000cf18 	.word	0x3000cf18
300037a8:	2001c01c 	.word	0x2001c01c
300037ac:	e000ed00 	.word	0xe000ed00
300037b0:	00054060 	.word	0x00054060
300037b4:	2001c00c 	.word	0x2001c00c
300037b8:	0000c045 	.word	0x0000c045
300037bc:	00009db5 	.word	0x00009db5
300037c0:	0000a149 	.word	0x0000a149
300037c4:	3000c04c 	.word	0x3000c04c
300037c8:	3000c07c 	.word	0x3000c07c
300037cc:	3000c024 	.word	0x3000c024
300037d0:	3000a149 	.word	0x3000a149
300037d4:	30009f59 	.word	0x30009f59
300037d8:	000129bd 	.word	0x000129bd
300037dc:	0000a53d 	.word	0x0000a53d

300037e0 <flash_highspeed_setup>:
300037e0:	4aaa      	ldr	r2, [pc, #680]	; (30003a8c <flash_highspeed_setup+0x2ac>)
300037e2:	f44f 4300 	mov.w	r3, #32768	; 0x8000
300037e6:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300037ea:	2500      	movs	r5, #0
300037ec:	b084      	sub	sp, #16
300037ee:	8811      	ldrh	r1, [r2, #0]
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
300038a0:	f44f 710b 	mov.w	r1, #556	; 0x22c
300038a4:	f008 fab4 	bl	3000be10 <__io_assert_failed_veneer>
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
3000391c:	f240 2163 	movw	r1, #611	; 0x263
30003920:	f008 fa76 	bl	3000be10 <__io_assert_failed_veneer>
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
30003a4a:	f008 f9f1 	bl	3000be30 <__DiagPrintf_veneer>
30003a4e:	e6ff      	b.n	30003850 <flash_highspeed_setup+0x70>
30003a50:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a54:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a58:	481c      	ldr	r0, [pc, #112]	; (30003acc <flash_highspeed_setup+0x2ec>)
30003a5a:	f008 f9e9 	bl	3000be30 <__DiagPrintf_veneer>
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
30003a8c:	3000ce88 	.word	0x3000ce88
30003a90:	3000ce8a 	.word	0x3000ce8a
30003a94:	2001c01c 	.word	0x2001c01c
30003a98:	2001c00c 	.word	0x2001c00c
30003a9c:	3000cd84 	.word	0x3000cd84
30003aa0:	3000ce8c 	.word	0x3000ce8c
30003aa4:	3000cf3c 	.word	0x3000cf3c
30003aa8:	0000b8c1 	.word	0x0000b8c1
30003aac:	3000cf2c 	.word	0x3000cf2c
30003ab0:	0000bd0d 	.word	0x0000bd0d
30003ab4:	0000c0f9 	.word	0x0000c0f9
30003ab8:	0000c1d9 	.word	0x0000c1d9
30003abc:	3000e590 	.word	0x3000e590
30003ac0:	000129bd 	.word	0x000129bd
30003ac4:	0000a149 	.word	0x0000a149
30003ac8:	3000c0b4 	.word	0x3000c0b4
30003acc:	3000c108 	.word	0x3000c108
30003ad0:	0000be71 	.word	0x0000be71
30003ad4:	0000bee9 	.word	0x0000bee9
30003ad8:	0000bfcd 	.word	0x0000bfcd
30003adc:	3000a149 	.word	0x3000a149
30003ae0:	00009db5 	.word	0x00009db5
30003ae4:	3000e594 	.word	0x3000e594
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
30003b64:	f240 2175 	movw	r1, #629	; 0x275
30003b68:	4815      	ldr	r0, [pc, #84]	; (30003bc0 <flash_highspeed_setup+0x3e0>)
30003b6a:	f008 f951 	bl	3000be10 <__io_assert_failed_veneer>
30003b6e:	f8d9 3000 	ldr.w	r3, [r9]
30003b72:	691b      	ldr	r3, [r3, #16]
30003b74:	e6e1      	b.n	3000393a <flash_highspeed_setup+0x15a>
30003b76:	f240 2149 	movw	r1, #585	; 0x249
30003b7a:	4812      	ldr	r0, [pc, #72]	; (30003bc4 <flash_highspeed_setup+0x3e4>)
30003b7c:	f008 f948 	bl	3000be10 <__io_assert_failed_veneer>
30003b80:	f8d9 3000 	ldr.w	r3, [r9]
30003b84:	691b      	ldr	r3, [r3, #16]
30003b86:	e70f      	b.n	300039a8 <flash_highspeed_setup+0x1c8>
30003b88:	480f      	ldr	r0, [pc, #60]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b8a:	f008 f951 	bl	3000be30 <__DiagPrintf_veneer>
30003b8e:	e686      	b.n	3000389e <flash_highspeed_setup+0xbe>
30003b90:	480d      	ldr	r0, [pc, #52]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b92:	f008 f94d 	bl	3000be30 <__DiagPrintf_veneer>
30003b96:	e6c0      	b.n	3000391a <flash_highspeed_setup+0x13a>
30003b98:	4b0c      	ldr	r3, [pc, #48]	; (30003bcc <flash_highspeed_setup+0x3ec>)
30003b9a:	4798      	blx	r3
30003b9c:	e705      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b9e:	bf00      	nop
30003ba0:	2001c01c 	.word	0x2001c01c
30003ba4:	0000bf59 	.word	0x0000bf59
30003ba8:	0000a3c5 	.word	0x0000a3c5
30003bac:	0000a441 	.word	0x0000a441
30003bb0:	3000e590 	.word	0x3000e590
30003bb4:	0000a345 	.word	0x0000a345
30003bb8:	0000b991 	.word	0x0000b991
30003bbc:	0000bd0d 	.word	0x0000bd0d
30003bc0:	3000cf2c 	.word	0x3000cf2c
30003bc4:	3000cf3c 	.word	0x3000cf3c
30003bc8:	3000c0e4 	.word	0x3000c0e4
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
30003bf2:	f002 f857 	bl	30005ca4 <BOOT_ImgCopy>
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
30003c4a:	f002 f82b 	bl	30005ca4 <BOOT_ImgCopy>
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
30003ca2:	f008 f8c5 	bl	3000be30 <__DiagPrintf_veneer>
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
30003d0e:	f008 f88f 	bl	3000be30 <__DiagPrintf_veneer>
30003d12:	2000      	movs	r0, #0
30003d14:	e7cc      	b.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d16:	bf00      	nop
30003d18:	31313738 	.word	0x31313738
30003d1c:	f7ffffe0 	.word	0xf7ffffe0
30003d20:	2001c00c 	.word	0x2001c00c
30003d24:	3000dd9c 	.word	0x3000dd9c
30003d28:	e000ed00 	.word	0xe000ed00
30003d2c:	3000c1c4 	.word	0x3000c1c4
30003d30:	3000c19c 	.word	0x3000c19c
30003d34:	35393138 	.word	0x35393138

30003d38 <BOOT_LoadAPSubImage>:
30003d38:	4baf      	ldr	r3, [pc, #700]	; (30003ff8 <BOOT_LoadAPSubImage+0x2c0>)
30003d3a:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003d3e:	460c      	mov	r4, r1
30003d40:	4606      	mov	r6, r0
30003d42:	b08b      	sub	sp, #44	; 0x2c
30003d44:	4615      	mov	r5, r2
30003d46:	2220      	movs	r2, #32
30003d48:	e893 0003 	ldmia.w	r3, {r0, r1}
30003d4c:	0c0b      	lsrs	r3, r1, #16
30003d4e:	9000      	str	r0, [sp, #0]
30003d50:	f8ad 1004 	strh.w	r1, [sp, #4]
30003d54:	a802      	add	r0, sp, #8
30003d56:	4621      	mov	r1, r4
30003d58:	f88d 3006 	strb.w	r3, [sp, #6]
30003d5c:	f001 ffa2 	bl	30005ca4 <BOOT_ImgCopy>
30003d60:	4ba6      	ldr	r3, [pc, #664]	; (30003ffc <BOOT_LoadAPSubImage+0x2c4>)
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
30003d80:	b00b      	add	sp, #44	; 0x2c
30003d82:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003d86:	4b9e      	ldr	r3, [pc, #632]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003d88:	6898      	ldr	r0, [r3, #8]
30003d8a:	f010 0002 	ands.w	r0, r0, #2
30003d8e:	d0f7      	beq.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d90:	4669      	mov	r1, sp
30003d92:	489c      	ldr	r0, [pc, #624]	; (30004004 <BOOT_LoadAPSubImage+0x2cc>)
30003d94:	f008 f84c 	bl	3000be30 <__DiagPrintf_veneer>
30003d98:	2000      	movs	r0, #0
30003d9a:	e7f1      	b.n	30003d80 <BOOT_LoadAPSubImage+0x48>
30003d9c:	9a05      	ldr	r2, [sp, #20]
30003d9e:	4b9a      	ldr	r3, [pc, #616]	; (30004008 <BOOT_LoadAPSubImage+0x2d0>)
30003da0:	6ca7      	ldr	r7, [r4, #72]	; 0x48
30003da2:	f1a2 0820 	sub.w	r8, r2, #32
30003da6:	4413      	add	r3, r2
30003da8:	37d0      	adds	r7, #208	; 0xd0
30003daa:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dae:	d23b      	bcs.n	30003e28 <BOOT_LoadAPSubImage+0xf0>
30003db0:	4b96      	ldr	r3, [pc, #600]	; (3000400c <BOOT_LoadAPSubImage+0x2d4>)
30003db2:	44b8      	add	r8, r7
30003db4:	f8d4 9070 	ldr.w	r9, [r4, #112]	; 0x70
30003db8:	4798      	blx	r3
30003dba:	2801      	cmp	r0, #1
30003dbc:	4427      	add	r7, r4
30003dbe:	d01e      	beq.n	30003dfe <BOOT_LoadAPSubImage+0xc6>
30003dc0:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30003dc4:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003dc8:	f080 8084 	bcs.w	30003ed4 <BOOT_LoadAPSubImage+0x19c>
30003dcc:	eb08 0509 	add.w	r5, r8, r9
30003dd0:	eb07 0109 	add.w	r1, r7, r9
30003dd4:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003dd8:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003ddc:	d24f      	bcs.n	30003e7e <BOOT_LoadAPSubImage+0x146>
30003dde:	b156      	cbz	r6, 30003df6 <BOOT_LoadAPSubImage+0xbe>
30003de0:	4987      	ldr	r1, [pc, #540]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003de2:	e9dd 3204 	ldrd	r3, r2, [sp, #16]
30003de6:	3a20      	subs	r2, #32
30003de8:	3320      	adds	r3, #32
30003dea:	e9c6 2300 	strd	r2, r3, [r6]
30003dee:	6889      	ldr	r1, [r1, #8]
30003df0:	0789      	lsls	r1, r1, #30
30003df2:	f100 80b6 	bmi.w	30003f62 <BOOT_LoadAPSubImage+0x22a>
30003df6:	2001      	movs	r0, #1
30003df8:	b00b      	add	sp, #44	; 0x2c
30003dfa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003dfe:	4d80      	ldr	r5, [pc, #512]	; (30004000 <BOOT_LoadAPSubImage+0x2c8>)
30003e00:	68ab      	ldr	r3, [r5, #8]
30003e02:	0799      	lsls	r1, r3, #30
30003e04:	f100 80b2 	bmi.w	30003f6c <BOOT_LoadAPSubImage+0x234>
30003e08:	464a      	mov	r2, r9
30003e0a:	4641      	mov	r1, r8
30003e0c:	4638      	mov	r0, r7
30003e0e:	f000 fc4b 	bl	300046a8 <BOOT_LoadBL32RDPImg>
30003e12:	2800      	cmp	r0, #0
30003e14:	d1da      	bne.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003e16:	682b      	ldr	r3, [r5, #0]
30003e18:	079a      	lsls	r2, r3, #30
30003e1a:	f100 80ca 	bmi.w	30003fb2 <BOOT_LoadAPSubImage+0x27a>
30003e1e:	4c7c      	ldr	r4, [pc, #496]	; (30004010 <BOOT_LoadAPSubImage+0x2d8>)
30003e20:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30003e24:	47a0      	blx	r4
30003e26:	e7fb      	b.n	30003e20 <BOOT_LoadAPSubImage+0xe8>
30003e28:	4d7a      	ldr	r5, [pc, #488]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e2a:	4640      	mov	r0, r8
30003e2c:	463a      	mov	r2, r7
30003e2e:	4621      	mov	r1, r4
30003e30:	f001 ff38 	bl	30005ca4 <BOOT_ImgCopy>
30003e34:	696b      	ldr	r3, [r5, #20]
30003e36:	03d8      	lsls	r0, r3, #15
30003e38:	d5ba      	bpl.n	30003db0 <BOOT_LoadAPSubImage+0x78>
30003e3a:	ea08 0307 	and.w	r3, r8, r7
30003e3e:	3301      	adds	r3, #1
30003e40:	d073      	beq.n	30003f2a <BOOT_LoadAPSubImage+0x1f2>
30003e42:	f018 031f 	ands.w	r3, r8, #31
30003e46:	f000 8095 	beq.w	30003f74 <BOOT_LoadAPSubImage+0x23c>
30003e4a:	443b      	add	r3, r7
30003e4c:	f028 021f 	bic.w	r2, r8, #31
30003e50:	f3bf 8f4f 	dsb	sy
30003e54:	2b00      	cmp	r3, #0
30003e56:	dd0d      	ble.n	30003e74 <BOOT_LoadAPSubImage+0x13c>
30003e58:	1e59      	subs	r1, r3, #1
30003e5a:	f102 0320 	add.w	r3, r2, #32
30003e5e:	486d      	ldr	r0, [pc, #436]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e60:	f021 011f 	bic.w	r1, r1, #31
30003e64:	4419      	add	r1, r3
30003e66:	428b      	cmp	r3, r1
30003e68:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003e6c:	461a      	mov	r2, r3
30003e6e:	f103 0320 	add.w	r3, r3, #32
30003e72:	d1f8      	bne.n	30003e66 <BOOT_LoadAPSubImage+0x12e>
30003e74:	f3bf 8f4f 	dsb	sy
30003e78:	f3bf 8f6f 	isb	sy
30003e7c:	e798      	b.n	30003db0 <BOOT_LoadAPSubImage+0x78>
30003e7e:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30003e82:	4628      	mov	r0, r5
30003e84:	4622      	mov	r2, r4
30003e86:	f001 ff0d 	bl	30005ca4 <BOOT_ImgCopy>
30003e8a:	4962      	ldr	r1, [pc, #392]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003e8c:	694b      	ldr	r3, [r1, #20]
30003e8e:	03d8      	lsls	r0, r3, #15
30003e90:	d5a5      	bpl.n	30003dde <BOOT_LoadAPSubImage+0xa6>
30003e92:	ea05 0304 	and.w	r3, r5, r4
30003e96:	3301      	adds	r3, #1
30003e98:	d06f      	beq.n	30003f7a <BOOT_LoadAPSubImage+0x242>
30003e9a:	f015 031f 	ands.w	r3, r5, #31
30003e9e:	d002      	beq.n	30003ea6 <BOOT_LoadAPSubImage+0x16e>
30003ea0:	f025 051f 	bic.w	r5, r5, #31
30003ea4:	441c      	add	r4, r3
30003ea6:	f3bf 8f4f 	dsb	sy
30003eaa:	2c00      	cmp	r4, #0
30003eac:	dd0d      	ble.n	30003eca <BOOT_LoadAPSubImage+0x192>
30003eae:	1e62      	subs	r2, r4, #1
30003eb0:	f105 0320 	add.w	r3, r5, #32
30003eb4:	4957      	ldr	r1, [pc, #348]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003eb6:	f022 021f 	bic.w	r2, r2, #31
30003eba:	441a      	add	r2, r3
30003ebc:	4293      	cmp	r3, r2
30003ebe:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30003ec2:	461d      	mov	r5, r3
30003ec4:	f103 0320 	add.w	r3, r3, #32
30003ec8:	d1f8      	bne.n	30003ebc <BOOT_LoadAPSubImage+0x184>
30003eca:	f3bf 8f4f 	dsb	sy
30003ece:	f3bf 8f6f 	isb	sy
30003ed2:	e784      	b.n	30003dde <BOOT_LoadAPSubImage+0xa6>
30003ed4:	4d4f      	ldr	r5, [pc, #316]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003ed6:	464a      	mov	r2, r9
30003ed8:	4639      	mov	r1, r7
30003eda:	4640      	mov	r0, r8
30003edc:	f001 fee2 	bl	30005ca4 <BOOT_ImgCopy>
30003ee0:	696b      	ldr	r3, [r5, #20]
30003ee2:	03db      	lsls	r3, r3, #15
30003ee4:	f57f af72 	bpl.w	30003dcc <BOOT_LoadAPSubImage+0x94>
30003ee8:	ea08 0309 	and.w	r3, r8, r9
30003eec:	3301      	adds	r3, #1
30003eee:	d067      	beq.n	30003fc0 <BOOT_LoadAPSubImage+0x288>
30003ef0:	f018 031f 	ands.w	r3, r8, #31
30003ef4:	d061      	beq.n	30003fba <BOOT_LoadAPSubImage+0x282>
30003ef6:	444b      	add	r3, r9
30003ef8:	f028 021f 	bic.w	r2, r8, #31
30003efc:	f3bf 8f4f 	dsb	sy
30003f00:	2b00      	cmp	r3, #0
30003f02:	dd0d      	ble.n	30003f20 <BOOT_LoadAPSubImage+0x1e8>
30003f04:	1e59      	subs	r1, r3, #1
30003f06:	f102 0320 	add.w	r3, r2, #32
30003f0a:	4842      	ldr	r0, [pc, #264]	; (30004014 <BOOT_LoadAPSubImage+0x2dc>)
30003f0c:	f021 011f 	bic.w	r1, r1, #31
30003f10:	4419      	add	r1, r3
30003f12:	428b      	cmp	r3, r1
30003f14:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003f18:	461a      	mov	r2, r3
30003f1a:	f103 0320 	add.w	r3, r3, #32
30003f1e:	d1f8      	bne.n	30003f12 <BOOT_LoadAPSubImage+0x1da>
30003f20:	f3bf 8f4f 	dsb	sy
30003f24:	f3bf 8f6f 	isb	sy
30003f28:	e750      	b.n	30003dcc <BOOT_LoadAPSubImage+0x94>
30003f2a:	2300      	movs	r3, #0
30003f2c:	f8c5 3084 	str.w	r3, [r5, #132]	; 0x84
30003f30:	f3bf 8f4f 	dsb	sy
30003f34:	f8d5 0080 	ldr.w	r0, [r5, #128]	; 0x80
30003f38:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003f3c:	f3c0 324e 	ubfx	r2, r0, #13, #15
30003f40:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30003f44:	0152      	lsls	r2, r2, #5
30003f46:	ea02 0c0e 	and.w	ip, r2, lr
30003f4a:	4603      	mov	r3, r0
30003f4c:	ea4c 7183 	orr.w	r1, ip, r3, lsl #30
30003f50:	3b01      	subs	r3, #1
30003f52:	f8c5 1274 	str.w	r1, [r5, #628]	; 0x274
30003f56:	d2f9      	bcs.n	30003f4c <BOOT_LoadAPSubImage+0x214>
30003f58:	3a20      	subs	r2, #32
30003f5a:	f112 0f20 	cmn.w	r2, #32
30003f5e:	d1f2      	bne.n	30003f46 <BOOT_LoadAPSubImage+0x20e>
30003f60:	e788      	b.n	30003e74 <BOOT_LoadAPSubImage+0x13c>
30003f62:	4669      	mov	r1, sp
30003f64:	482c      	ldr	r0, [pc, #176]	; (30004018 <BOOT_LoadAPSubImage+0x2e0>)
30003f66:	f007 ff63 	bl	3000be30 <__DiagPrintf_veneer>
30003f6a:	e744      	b.n	30003df6 <BOOT_LoadAPSubImage+0xbe>
30003f6c:	482b      	ldr	r0, [pc, #172]	; (3000401c <BOOT_LoadAPSubImage+0x2e4>)
30003f6e:	f007 ff5f 	bl	3000be30 <__DiagPrintf_veneer>
30003f72:	e749      	b.n	30003e08 <BOOT_LoadAPSubImage+0xd0>
30003f74:	463b      	mov	r3, r7
30003f76:	4642      	mov	r2, r8
30003f78:	e76a      	b.n	30003e50 <BOOT_LoadAPSubImage+0x118>
30003f7a:	2300      	movs	r3, #0
30003f7c:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003f80:	f3bf 8f4f 	dsb	sy
30003f84:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30003f88:	f643 77e0 	movw	r7, #16352	; 0x3fe0
30003f8c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30003f90:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30003f94:	0164      	lsls	r4, r4, #5
30003f96:	ea04 0007 	and.w	r0, r4, r7
30003f9a:	462b      	mov	r3, r5
30003f9c:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003fa0:	3b01      	subs	r3, #1
30003fa2:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003fa6:	d2f9      	bcs.n	30003f9c <BOOT_LoadAPSubImage+0x264>
30003fa8:	3c20      	subs	r4, #32
30003faa:	f114 0f20 	cmn.w	r4, #32
30003fae:	d1f2      	bne.n	30003f96 <BOOT_LoadAPSubImage+0x25e>
30003fb0:	e78b      	b.n	30003eca <BOOT_LoadAPSubImage+0x192>
30003fb2:	481b      	ldr	r0, [pc, #108]	; (30004020 <BOOT_LoadAPSubImage+0x2e8>)
30003fb4:	f007 ff3c 	bl	3000be30 <__DiagPrintf_veneer>
30003fb8:	e731      	b.n	30003e1e <BOOT_LoadAPSubImage+0xe6>
30003fba:	464b      	mov	r3, r9
30003fbc:	4642      	mov	r2, r8
30003fbe:	e79d      	b.n	30003efc <BOOT_LoadAPSubImage+0x1c4>
30003fc0:	2300      	movs	r3, #0
30003fc2:	f8c5 3084 	str.w	r3, [r5, #132]	; 0x84
30003fc6:	f3bf 8f4f 	dsb	sy
30003fca:	f8d5 1080 	ldr.w	r1, [r5, #128]	; 0x80
30003fce:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003fd2:	f3c1 324e 	ubfx	r2, r1, #13, #15
30003fd6:	f3c1 01c9 	ubfx	r1, r1, #3, #10
30003fda:	0152      	lsls	r2, r2, #5
30003fdc:	ea02 0c0e 	and.w	ip, r2, lr
30003fe0:	460b      	mov	r3, r1
30003fe2:	ea4c 7083 	orr.w	r0, ip, r3, lsl #30
30003fe6:	3b01      	subs	r3, #1
30003fe8:	f8c5 0274 	str.w	r0, [r5, #628]	; 0x274
30003fec:	d2f9      	bcs.n	30003fe2 <BOOT_LoadAPSubImage+0x2aa>
30003fee:	3a20      	subs	r2, #32
30003ff0:	f112 0f20 	cmn.w	r2, #32
30003ff4:	d1f2      	bne.n	30003fdc <BOOT_LoadAPSubImage+0x2a4>
30003ff6:	e793      	b.n	30003f20 <BOOT_LoadAPSubImage+0x1e8>
30003ff8:	3000c194 	.word	0x3000c194
30003ffc:	35393138 	.word	0x35393138
30004000:	2001c00c 	.word	0x2001c00c
30004004:	3000c19c 	.word	0x3000c19c
30004008:	f7ffffe0 	.word	0xf7ffffe0
3000400c:	0000c109 	.word	0x0000c109
30004010:	00009be5 	.word	0x00009be5
30004014:	e000ed00 	.word	0xe000ed00
30004018:	3000c1c4 	.word	0x3000c1c4
3000401c:	3000c1ec 	.word	0x3000c1ec
30004020:	3000c210 	.word	0x3000c210

30004024 <BOOT_RSIPIvSet>:
30004024:	b530      	push	{r4, r5, lr}
30004026:	2300      	movs	r3, #0
30004028:	b083      	sub	sp, #12
3000402a:	4a09      	ldr	r2, [pc, #36]	; (30004050 <BOOT_RSIPIvSet+0x2c>)
3000402c:	4605      	mov	r5, r0
3000402e:	460c      	mov	r4, r1
30004030:	9300      	str	r3, [sp, #0]
30004032:	9301      	str	r3, [sp, #4]
30004034:	4790      	blx	r2
30004036:	b148      	cbz	r0, 3000404c <BOOT_RSIPIvSet+0x28>
30004038:	f105 0120 	add.w	r1, r5, #32
3000403c:	4b05      	ldr	r3, [pc, #20]	; (30004054 <BOOT_RSIPIvSet+0x30>)
3000403e:	2208      	movs	r2, #8
30004040:	4668      	mov	r0, sp
30004042:	4798      	blx	r3
30004044:	4669      	mov	r1, sp
30004046:	b2e0      	uxtb	r0, r4
30004048:	4b03      	ldr	r3, [pc, #12]	; (30004058 <BOOT_RSIPIvSet+0x34>)
3000404a:	4798      	blx	r3
3000404c:	b003      	add	sp, #12
3000404e:	bd30      	pop	{r4, r5, pc}
30004050:	0000c129 	.word	0x0000c129
30004054:	00012a1d 	.word	0x00012a1d
30004058:	30009e41 	.word	0x30009e41

3000405c <BOOT_OTFCheck>:
3000405c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004060:	4d1d      	ldr	r5, [pc, #116]	; (300040d8 <BOOT_OTFCheck+0x7c>)
30004062:	b082      	sub	sp, #8
30004064:	4604      	mov	r4, r0
30004066:	4688      	mov	r8, r1
30004068:	4617      	mov	r7, r2
3000406a:	461e      	mov	r6, r3
3000406c:	47a8      	blx	r5
3000406e:	b1b0      	cbz	r0, 3000409e <BOOT_OTFCheck+0x42>
30004070:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30004074:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004078:	d211      	bcs.n	3000409e <BOOT_OTFCheck+0x42>
3000407a:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
3000407e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004082:	d20c      	bcs.n	3000409e <BOOT_OTFCheck+0x42>
30004084:	4d15      	ldr	r5, [pc, #84]	; (300040dc <BOOT_OTFCheck+0x80>)
30004086:	682b      	ldr	r3, [r5, #0]
30004088:	039a      	lsls	r2, r3, #14
3000408a:	d420      	bmi.n	300040ce <BOOT_OTFCheck+0x72>
3000408c:	4b14      	ldr	r3, [pc, #80]	; (300040e0 <BOOT_OTFCheck+0x84>)
3000408e:	4798      	blx	r3
30004090:	4603      	mov	r3, r0
30004092:	b138      	cbz	r0, 300040a4 <BOOT_OTFCheck+0x48>
30004094:	2801      	cmp	r0, #1
30004096:	d006      	beq.n	300040a6 <BOOT_OTFCheck+0x4a>
30004098:	682b      	ldr	r3, [r5, #0]
3000409a:	039b      	lsls	r3, r3, #14
3000409c:	d411      	bmi.n	300040c2 <BOOT_OTFCheck+0x66>
3000409e:	b002      	add	sp, #8
300040a0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300040a4:	2302      	movs	r3, #2
300040a6:	4630      	mov	r0, r6
300040a8:	4621      	mov	r1, r4
300040aa:	9301      	str	r3, [sp, #4]
300040ac:	4642      	mov	r2, r8
300040ae:	2301      	movs	r3, #1
300040b0:	4c0c      	ldr	r4, [pc, #48]	; (300040e4 <BOOT_OTFCheck+0x88>)
300040b2:	9700      	str	r7, [sp, #0]
300040b4:	47a0      	blx	r4
300040b6:	2001      	movs	r0, #1
300040b8:	4b0b      	ldr	r3, [pc, #44]	; (300040e8 <BOOT_OTFCheck+0x8c>)
300040ba:	b002      	add	sp, #8
300040bc:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040c0:	4718      	bx	r3
300040c2:	480a      	ldr	r0, [pc, #40]	; (300040ec <BOOT_OTFCheck+0x90>)
300040c4:	b002      	add	sp, #8
300040c6:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300040ca:	f007 beb1 	b.w	3000be30 <__DiagPrintf_veneer>
300040ce:	4808      	ldr	r0, [pc, #32]	; (300040f0 <BOOT_OTFCheck+0x94>)
300040d0:	f007 feae 	bl	3000be30 <__DiagPrintf_veneer>
300040d4:	e7da      	b.n	3000408c <BOOT_OTFCheck+0x30>
300040d6:	bf00      	nop
300040d8:	0000c129 	.word	0x0000c129
300040dc:	2001c00c 	.word	0x2001c00c
300040e0:	0000c245 	.word	0x0000c245
300040e4:	30009e75 	.word	0x30009e75
300040e8:	30009e5d 	.word	0x30009e5d
300040ec:	3000c254 	.word	0x3000c254
300040f0:	3000c244 	.word	0x3000c244

300040f4 <BOOT_OTA_LoadIMG2>:
300040f4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300040f8:	4b6e      	ldr	r3, [pc, #440]	; (300042b4 <BOOT_OTA_LoadIMG2+0x1c0>)
300040fa:	b08d      	sub	sp, #52	; 0x34
300040fc:	4a6e      	ldr	r2, [pc, #440]	; (300042b8 <BOOT_OTA_LoadIMG2+0x1c4>)
300040fe:	f103 040c 	add.w	r4, r3, #12
30004102:	9003      	str	r0, [sp, #12]
30004104:	f8d2 8000 	ldr.w	r8, [r2]
30004108:	ae06      	add	r6, sp, #24
3000410a:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
3000410e:	ab09      	add	r3, sp, #36	; 0x24
30004110:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30004114:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30004118:	e883 0007 	stmia.w	r3, {r0, r1, r2}
3000411c:	4641      	mov	r1, r8
3000411e:	4867      	ldr	r0, [pc, #412]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
30004120:	f44f 72b2 	mov.w	r2, #356	; 0x164
30004124:	f001 fdbe 	bl	30005ca4 <BOOT_ImgCopy>
30004128:	4641      	mov	r1, r8
3000412a:	4864      	ldr	r0, [pc, #400]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
3000412c:	f000 fd86 	bl	30004c3c <BOOT_CertificateCheck>
30004130:	2800      	cmp	r0, #0
30004132:	f040 80b7 	bne.w	300042a4 <BOOT_OTA_LoadIMG2+0x1b0>
30004136:	4604      	mov	r4, r0
30004138:	f44f 72c8 	mov.w	r2, #400	; 0x190
3000413c:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004140:	485f      	ldr	r0, [pc, #380]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30004142:	f001 fdaf 	bl	30005ca4 <BOOT_ImgCopy>
30004146:	4b5f      	ldr	r3, [pc, #380]	; (300042c4 <BOOT_OTA_LoadIMG2+0x1d0>)
30004148:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000414c:	4798      	blx	r3
3000414e:	2800      	cmp	r0, #0
30004150:	f040 809e 	bne.w	30004290 <BOOT_OTA_LoadIMG2+0x19c>
30004154:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30004158:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30004308 <BOOT_OTA_LoadIMG2+0x214>
3000415c:	4a5a      	ldr	r2, [pc, #360]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000415e:	2000      	movs	r0, #0
30004160:	4445      	add	r5, r8
30004162:	4659      	mov	r1, fp
30004164:	4f59      	ldr	r7, [pc, #356]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d8>)
30004166:	f04f 0801 	mov.w	r8, #1
3000416a:	462b      	mov	r3, r5
3000416c:	f8df 919c 	ldr.w	r9, [pc, #412]	; 3000430c <BOOT_OTA_LoadIMG2+0x218>
30004170:	47b8      	blx	r7
30004172:	2101      	movs	r1, #1
30004174:	2000      	movs	r0, #0
30004176:	4b56      	ldr	r3, [pc, #344]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1dc>)
30004178:	4798      	blx	r3
3000417a:	2301      	movs	r3, #1
3000417c:	4952      	ldr	r1, [pc, #328]	; (300042c8 <BOOT_OTA_LoadIMG2+0x1d4>)
3000417e:	4658      	mov	r0, fp
30004180:	461a      	mov	r2, r3
30004182:	f7ff ff6b 	bl	3000405c <BOOT_OTFCheck>
30004186:	47c8      	blx	r9
30004188:	2800      	cmp	r0, #0
3000418a:	bf0c      	ite	eq
3000418c:	46aa      	moveq	sl, r5
3000418e:	46da      	movne	sl, fp
30004190:	4633      	mov	r3, r6
30004192:	2203      	movs	r2, #3
30004194:	4651      	mov	r1, sl
30004196:	484f      	ldr	r0, [pc, #316]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1e0>)
30004198:	f8cd 8000 	str.w	r8, [sp]
3000419c:	f7ff fd18 	bl	30003bd0 <BOOT_LoadSubImage>
300041a0:	2800      	cmp	r0, #0
300041a2:	d070      	beq.n	30004286 <BOOT_OTA_LoadIMG2+0x192>
300041a4:	4e4b      	ldr	r6, [pc, #300]	; (300042d4 <BOOT_OTA_LoadIMG2+0x1e0>)
300041a6:	47c8      	blx	r9
300041a8:	2800      	cmp	r0, #0
300041aa:	bf08      	it	eq
300041ac:	46ab      	moveq	fp, r5
300041ae:	4f47      	ldr	r7, [pc, #284]	; (300042cc <BOOT_OTA_LoadIMG2+0x1d8>)
300041b0:	68f2      	ldr	r2, [r6, #12]
300041b2:	4640      	mov	r0, r8
300041b4:	6873      	ldr	r3, [r6, #4]
300041b6:	f8c6 b000 	str.w	fp, [r6]
300041ba:	4413      	add	r3, r2
300041bc:	6972      	ldr	r2, [r6, #20]
300041be:	f8df b150 	ldr.w	fp, [pc, #336]	; 30004310 <BOOT_OTA_LoadIMG2+0x21c>
300041c2:	441a      	add	r2, r3
300041c4:	4b44      	ldr	r3, [pc, #272]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1e4>)
300041c6:	eb0a 0102 	add.w	r1, sl, r2
300041ca:	61b1      	str	r1, [r6, #24]
300041cc:	1e51      	subs	r1, r2, #1
300041ce:	400b      	ands	r3, r1
300041d0:	4659      	mov	r1, fp
300041d2:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
300041d6:	441d      	add	r5, r3
300041d8:	1a9b      	subs	r3, r3, r2
300041da:	4a40      	ldr	r2, [pc, #256]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e8>)
300041dc:	61f3      	str	r3, [r6, #28]
300041de:	462b      	mov	r3, r5
300041e0:	47b8      	blx	r7
300041e2:	4641      	mov	r1, r8
300041e4:	4640      	mov	r0, r8
300041e6:	4b3a      	ldr	r3, [pc, #232]	; (300042d0 <BOOT_OTA_LoadIMG2+0x1dc>)
300041e8:	4798      	blx	r3
300041ea:	4642      	mov	r2, r8
300041ec:	2302      	movs	r3, #2
300041ee:	493b      	ldr	r1, [pc, #236]	; (300042dc <BOOT_OTA_LoadIMG2+0x1e8>)
300041f0:	4658      	mov	r0, fp
300041f2:	f7ff ff33 	bl	3000405c <BOOT_OTFCheck>
300041f6:	47c8      	blx	r9
300041f8:	2800      	cmp	r0, #0
300041fa:	bf0c      	ite	eq
300041fc:	462f      	moveq	r7, r5
300041fe:	465f      	movne	r7, fp
30004200:	ab09      	add	r3, sp, #36	; 0x24
30004202:	2203      	movs	r2, #3
30004204:	f106 0020 	add.w	r0, r6, #32
30004208:	4639      	mov	r1, r7
3000420a:	f8cd 8000 	str.w	r8, [sp]
3000420e:	f7ff fcdf 	bl	30003bd0 <BOOT_LoadSubImage>
30004212:	2800      	cmp	r0, #0
30004214:	d037      	beq.n	30004286 <BOOT_OTA_LoadIMG2+0x192>
30004216:	47c8      	blx	r9
30004218:	6af2      	ldr	r2, [r6, #44]	; 0x2c
3000421a:	6a73      	ldr	r3, [r6, #36]	; 0x24
3000421c:	2800      	cmp	r0, #0
3000421e:	bf08      	it	eq
30004220:	46ab      	moveq	fp, r5
30004222:	4827      	ldr	r0, [pc, #156]	; (300042c0 <BOOT_OTA_LoadIMG2+0x1cc>)
30004224:	4413      	add	r3, r2
30004226:	6b72      	ldr	r2, [r6, #52]	; 0x34
30004228:	f8c6 b020 	str.w	fp, [r6, #32]
3000422c:	4413      	add	r3, r2
3000422e:	4a2a      	ldr	r2, [pc, #168]	; (300042d8 <BOOT_OTA_LoadIMG2+0x1e4>)
30004230:	1e59      	subs	r1, r3, #1
30004232:	441f      	add	r7, r3
30004234:	400a      	ands	r2, r1
30004236:	2100      	movs	r1, #0
30004238:	63b7      	str	r7, [r6, #56]	; 0x38
3000423a:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
3000423e:	9100      	str	r1, [sp, #0]
30004240:	1ad1      	subs	r1, r2, r3
30004242:	2900      	cmp	r1, #0
30004244:	63f1      	str	r1, [r6, #60]	; 0x3c
30004246:	4631      	mov	r1, r6
30004248:	bf18      	it	ne
3000424a:	4613      	movne	r3, r2
3000424c:	4a24      	ldr	r2, [pc, #144]	; (300042e0 <BOOT_OTA_LoadIMG2+0x1ec>)
3000424e:	441d      	add	r5, r3
30004250:	4b1a      	ldr	r3, [pc, #104]	; (300042bc <BOOT_OTA_LoadIMG2+0x1c8>)
30004252:	6015      	str	r5, [r2, #0]
30004254:	bf0c      	ite	eq
30004256:	2207      	moveq	r2, #7
30004258:	2208      	movne	r2, #8
3000425a:	f000 fbdb 	bl	30004a14 <BOOT_SignatureCheck>
3000425e:	4b21      	ldr	r3, [pc, #132]	; (300042e4 <BOOT_OTA_LoadIMG2+0x1f0>)
30004260:	4605      	mov	r5, r0
30004262:	689b      	ldr	r3, [r3, #8]
30004264:	079b      	lsls	r3, r3, #30
30004266:	d50a      	bpl.n	3000427e <BOOT_OTA_LoadIMG2+0x18a>
30004268:	4b1f      	ldr	r3, [pc, #124]	; (300042e8 <BOOT_OTA_LoadIMG2+0x1f4>)
3000426a:	4a20      	ldr	r2, [pc, #128]	; (300042ec <BOOT_OTA_LoadIMG2+0x1f8>)
3000426c:	9903      	ldr	r1, [sp, #12]
3000426e:	2800      	cmp	r0, #0
30004270:	bf18      	it	ne
30004272:	461a      	movne	r2, r3
30004274:	481e      	ldr	r0, [pc, #120]	; (300042f0 <BOOT_OTA_LoadIMG2+0x1fc>)
30004276:	3101      	adds	r1, #1
30004278:	462b      	mov	r3, r5
3000427a:	f007 fdd9 	bl	3000be30 <__DiagPrintf_veneer>
3000427e:	b11d      	cbz	r5, 30004288 <BOOT_OTA_LoadIMG2+0x194>
30004280:	481c      	ldr	r0, [pc, #112]	; (300042f4 <BOOT_OTA_LoadIMG2+0x200>)
30004282:	f007 fdd5 	bl	3000be30 <__DiagPrintf_veneer>
30004286:	24ff      	movs	r4, #255	; 0xff
30004288:	4620      	mov	r0, r4
3000428a:	b00d      	add	sp, #52	; 0x34
3000428c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004290:	2208      	movs	r2, #8
30004292:	4919      	ldr	r1, [pc, #100]	; (300042f8 <BOOT_OTA_LoadIMG2+0x204>)
30004294:	4b19      	ldr	r3, [pc, #100]	; (300042fc <BOOT_OTA_LoadIMG2+0x208>)
30004296:	a804      	add	r0, sp, #16
30004298:	4798      	blx	r3
3000429a:	a904      	add	r1, sp, #16
3000429c:	2001      	movs	r0, #1
3000429e:	4b18      	ldr	r3, [pc, #96]	; (30004300 <BOOT_OTA_LoadIMG2+0x20c>)
300042a0:	4798      	blx	r3
300042a2:	e757      	b.n	30004154 <BOOT_OTA_LoadIMG2+0x60>
300042a4:	24ff      	movs	r4, #255	; 0xff
300042a6:	4817      	ldr	r0, [pc, #92]	; (30004304 <BOOT_OTA_LoadIMG2+0x210>)
300042a8:	f007 fdc2 	bl	3000be30 <__DiagPrintf_veneer>
300042ac:	4620      	mov	r0, r4
300042ae:	b00d      	add	sp, #52	; 0x34
300042b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300042b4:	3000d1e8 	.word	0x3000d1e8
300042b8:	3000e920 	.word	0x3000e920
300042bc:	3000e5c4 	.word	0x3000e5c4
300042c0:	3000e728 	.word	0x3000e728
300042c4:	0000c129 	.word	0x0000c129
300042c8:	0cffffe0 	.word	0x0cffffe0
300042cc:	30009ef1 	.word	0x30009ef1
300042d0:	30009f31 	.word	0x30009f31
300042d4:	3000e8d0 	.word	0x3000e8d0
300042d8:	fffff000 	.word	0xfffff000
300042dc:	0effffe0 	.word	0x0effffe0
300042e0:	3000dd9c 	.word	0x3000dd9c
300042e4:	2001c00c 	.word	0x2001c00c
300042e8:	3000c270 	.word	0x3000c270
300042ec:	3000c268 	.word	0x3000c268
300042f0:	3000c2a0 	.word	0x3000c2a0
300042f4:	3000c2d4 	.word	0x3000c2d4
300042f8:	3000e748 	.word	0x3000e748
300042fc:	00012a1d 	.word	0x00012a1d
30004300:	30009e41 	.word	0x30009e41
30004304:	3000c278 	.word	0x3000c278
30004308:	0c000000 	.word	0x0c000000
3000430c:	3000a149 	.word	0x3000a149
30004310:	0e000000 	.word	0x0e000000

30004314 <BOOT_OTA_Region_Init>:
30004314:	4b24      	ldr	r3, [pc, #144]	; (300043a8 <BOOT_OTA_Region_Init+0x94>)
30004316:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000431a:	4798      	blx	r3
3000431c:	4b23      	ldr	r3, [pc, #140]	; (300043ac <BOOT_OTA_Region_Init+0x98>)
3000431e:	4a24      	ldr	r2, [pc, #144]	; (300043b0 <BOOT_OTA_Region_Init+0x9c>)
30004320:	4924      	ldr	r1, [pc, #144]	; (300043b4 <BOOT_OTA_Region_Init+0xa0>)
30004322:	2800      	cmp	r0, #0
30004324:	bf08      	it	eq
30004326:	461a      	moveq	r2, r3
30004328:	6813      	ldr	r3, [r2, #0]
3000432a:	2bff      	cmp	r3, #255	; 0xff
3000432c:	e9d1 0400 	ldrd	r0, r4, [r1]
30004330:	d029      	beq.n	30004386 <BOOT_OTA_Region_Init+0x72>
30004332:	2500      	movs	r5, #0
30004334:	e9d1 e804 	ldrd	lr, r8, [r1, #16]
30004338:	46ac      	mov	ip, r5
3000433a:	462f      	mov	r7, r5
3000433c:	462e      	mov	r6, r5
3000433e:	e00c      	b.n	3000435a <BOOT_OTA_Region_Init+0x46>
30004340:	2b03      	cmp	r3, #3
30004342:	d028      	beq.n	30004396 <BOOT_OTA_Region_Init+0x82>
30004344:	2b04      	cmp	r3, #4
30004346:	d029      	beq.n	3000439c <BOOT_OTA_Region_Init+0x88>
30004348:	2b05      	cmp	r3, #5
3000434a:	d102      	bne.n	30004352 <BOOT_OTA_Region_Init+0x3e>
3000434c:	f8d2 8004 	ldr.w	r8, [r2, #4]
30004350:	2501      	movs	r5, #1
30004352:	f852 3f0c 	ldr.w	r3, [r2, #12]!
30004356:	2bff      	cmp	r3, #255	; 0xff
30004358:	d007      	beq.n	3000436a <BOOT_OTA_Region_Init+0x56>
3000435a:	2b02      	cmp	r3, #2
3000435c:	d1f0      	bne.n	30004340 <BOOT_OTA_Region_Init+0x2c>
3000435e:	6850      	ldr	r0, [r2, #4]
30004360:	2601      	movs	r6, #1
30004362:	f852 3f0c 	ldr.w	r3, [r2, #12]!
30004366:	2bff      	cmp	r3, #255	; 0xff
30004368:	d1f7      	bne.n	3000435a <BOOT_OTA_Region_Init+0x46>
3000436a:	b106      	cbz	r6, 3000436e <BOOT_OTA_Region_Init+0x5a>
3000436c:	6008      	str	r0, [r1, #0]
3000436e:	b107      	cbz	r7, 30004372 <BOOT_OTA_Region_Init+0x5e>
30004370:	604c      	str	r4, [r1, #4]
30004372:	f1bc 0f00 	cmp.w	ip, #0
30004376:	d001      	beq.n	3000437c <BOOT_OTA_Region_Init+0x68>
30004378:	f8c1 e010 	str.w	lr, [r1, #16]
3000437c:	e9d1 0400 	ldrd	r0, r4, [r1]
30004380:	b10d      	cbz	r5, 30004386 <BOOT_OTA_Region_Init+0x72>
30004382:	f8c1 8014 	str.w	r8, [r1, #20]
30004386:	f500 5080 	add.w	r0, r0, #4096	; 0x1000
3000438a:	f504 5480 	add.w	r4, r4, #4096	; 0x1000
3000438e:	e9c1 0402 	strd	r0, r4, [r1, #8]
30004392:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30004396:	6854      	ldr	r4, [r2, #4]
30004398:	2701      	movs	r7, #1
3000439a:	e7da      	b.n	30004352 <BOOT_OTA_Region_Init+0x3e>
3000439c:	f8d2 e004 	ldr.w	lr, [r2, #4]
300043a0:	f04f 0c01 	mov.w	ip, #1
300043a4:	e7d5      	b.n	30004352 <BOOT_OTA_Region_Init+0x3e>
300043a6:	bf00      	nop
300043a8:	3000a149 	.word	0x3000a149
300043ac:	3000dec4 	.word	0x3000dec4
300043b0:	3000df3c 	.word	0x3000df3c
300043b4:	3000e8b8 	.word	0x3000e8b8

300043b8 <BOOT_OTA_IMG2>:
300043b8:	f7ff be9c 	b.w	300040f4 <BOOT_OTA_LoadIMG2>

300043bc <BOOT_OTA_AP>:
300043bc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300043c0:	4c31      	ldr	r4, [pc, #196]	; (30004488 <BOOT_OTA_AP+0xcc>)
300043c2:	b0ed      	sub	sp, #436	; 0x1b4
300043c4:	4b31      	ldr	r3, [pc, #196]	; (3000448c <BOOT_OTA_AP+0xd0>)
300043c6:	ad05      	add	r5, sp, #20
300043c8:	af03      	add	r7, sp, #12
300043ca:	681b      	ldr	r3, [r3, #0]
300043cc:	4e30      	ldr	r6, [pc, #192]	; (30004490 <BOOT_OTA_AP+0xd4>)
300043ce:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
300043d2:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
300043d6:	340c      	adds	r4, #12
300043d8:	e885 0007 	stmia.w	r5, {r0, r1, r2}
300043dc:	e894 0003 	ldmia.w	r4, {r0, r1}
300043e0:	f44f 72c8 	mov.w	r2, #400	; 0x190
300043e4:	e887 0003 	stmia.w	r7, {r0, r1}
300043e8:	4619      	mov	r1, r3
300043ea:	a808      	add	r0, sp, #32
300043ec:	f001 fc5a 	bl	30005ca4 <BOOT_ImgCopy>
300043f0:	2201      	movs	r2, #1
300043f2:	462b      	mov	r3, r5
300043f4:	4641      	mov	r1, r8
300043f6:	9200      	str	r2, [sp, #0]
300043f8:	4630      	mov	r0, r6
300043fa:	2203      	movs	r2, #3
300043fc:	f7ff fbe8 	bl	30003bd0 <BOOT_LoadSubImage>
30004400:	2801      	cmp	r0, #1
30004402:	d122      	bne.n	3000444a <BOOT_OTA_AP+0x8e>
30004404:	f04f 0902 	mov.w	r9, #2
30004408:	4604      	mov	r4, r0
3000440a:	4b22      	ldr	r3, [pc, #136]	; (30004494 <BOOT_OTA_AP+0xd8>)
3000440c:	a808      	add	r0, sp, #32
3000440e:	2203      	movs	r2, #3
30004410:	4631      	mov	r1, r6
30004412:	f8cd 9000 	str.w	r9, [sp]
30004416:	f000 fafd 	bl	30004a14 <BOOT_SignatureCheck>
3000441a:	4605      	mov	r5, r0
3000441c:	bb30      	cbnz	r0, 3000446c <BOOT_OTA_AP+0xb0>
3000441e:	464a      	mov	r2, r9
30004420:	463b      	mov	r3, r7
30004422:	4641      	mov	r1, r8
30004424:	4630      	mov	r0, r6
30004426:	9400      	str	r4, [sp, #0]
30004428:	f7ff fbd2 	bl	30003bd0 <BOOT_LoadSubImage>
3000442c:	6871      	ldr	r1, [r6, #4]
3000442e:	68f3      	ldr	r3, [r6, #12]
30004430:	4622      	mov	r2, r4
30004432:	f106 0010 	add.w	r0, r6, #16
30004436:	4419      	add	r1, r3
30004438:	4441      	add	r1, r8
3000443a:	f7ff fc7d 	bl	30003d38 <BOOT_LoadAPSubImage>
3000443e:	2801      	cmp	r0, #1
30004440:	d10b      	bne.n	3000445a <BOOT_OTA_AP+0x9e>
30004442:	4628      	mov	r0, r5
30004444:	b06d      	add	sp, #436	; 0x1b4
30004446:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000444a:	25ff      	movs	r5, #255	; 0xff
3000444c:	4812      	ldr	r0, [pc, #72]	; (30004498 <BOOT_OTA_AP+0xdc>)
3000444e:	f007 fcef 	bl	3000be30 <__DiagPrintf_veneer>
30004452:	4628      	mov	r0, r5
30004454:	b06d      	add	sp, #436	; 0x1b4
30004456:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000445a:	4b10      	ldr	r3, [pc, #64]	; (3000449c <BOOT_OTA_AP+0xe0>)
3000445c:	681b      	ldr	r3, [r3, #0]
3000445e:	079b      	lsls	r3, r3, #30
30004460:	d40c      	bmi.n	3000447c <BOOT_OTA_AP+0xc0>
30004462:	25ff      	movs	r5, #255	; 0xff
30004464:	4628      	mov	r0, r5
30004466:	b06d      	add	sp, #436	; 0x1b4
30004468:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000446c:	25ff      	movs	r5, #255	; 0xff
3000446e:	480c      	ldr	r0, [pc, #48]	; (300044a0 <BOOT_OTA_AP+0xe4>)
30004470:	f007 fcde 	bl	3000be30 <__DiagPrintf_veneer>
30004474:	4628      	mov	r0, r5
30004476:	b06d      	add	sp, #436	; 0x1b4
30004478:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000447c:	4809      	ldr	r0, [pc, #36]	; (300044a4 <BOOT_OTA_AP+0xe8>)
3000447e:	25ff      	movs	r5, #255	; 0xff
30004480:	f007 fcd6 	bl	3000be30 <__DiagPrintf_veneer>
30004484:	e7dd      	b.n	30004442 <BOOT_OTA_AP+0x86>
30004486:	bf00      	nop
30004488:	3000d200 	.word	0x3000d200
3000448c:	3000dd9c 	.word	0x3000dd9c
30004490:	3000e8d0 	.word	0x3000e8d0
30004494:	3000e5c4 	.word	0x3000e5c4
30004498:	3000c31c 	.word	0x3000c31c
3000449c:	2001c00c 	.word	0x2001c00c
300044a0:	3000c2f8 	.word	0x3000c2f8
300044a4:	3000c340 	.word	0x3000c340

300044a8 <BOOT_OTFMaskConfig>:
300044a8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300044ac:	4c1c      	ldr	r4, [pc, #112]	; (30004520 <BOOT_OTFMaskConfig+0x78>)
300044ae:	b082      	sub	sp, #8
300044b0:	e9d4 1200 	ldrd	r1, r2, [r4]
300044b4:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044b8:	1c4d      	adds	r5, r1, #1
300044ba:	68a0      	ldr	r0, [r4, #8]
300044bc:	d027      	beq.n	3000450e <BOOT_OTFMaskConfig+0x66>
300044be:	2500      	movs	r5, #0
300044c0:	4e18      	ldr	r6, [pc, #96]	; (30004524 <BOOT_OTFMaskConfig+0x7c>)
300044c2:	f8df 806c 	ldr.w	r8, [pc, #108]	; 30004530 <BOOT_OTFMaskConfig+0x88>
300044c6:	4f18      	ldr	r7, [pc, #96]	; (30004528 <BOOT_OTFMaskConfig+0x80>)
300044c8:	e00d      	b.n	300044e6 <BOOT_OTFMaskConfig+0x3e>
300044ca:	6833      	ldr	r3, [r6, #0]
300044cc:	079a      	lsls	r2, r3, #30
300044ce:	d416      	bmi.n	300044fe <BOOT_OTFMaskConfig+0x56>
300044d0:	6961      	ldr	r1, [r4, #20]
300044d2:	3501      	adds	r5, #1
300044d4:	69a2      	ldr	r2, [r4, #24]
300044d6:	3414      	adds	r4, #20
300044d8:	1c4b      	adds	r3, r1, #1
300044da:	f8d4 c010 	ldr.w	ip, [r4, #16]
300044de:	68a0      	ldr	r0, [r4, #8]
300044e0:	d015      	beq.n	3000450e <BOOT_OTFMaskConfig+0x66>
300044e2:	2d08      	cmp	r5, #8
300044e4:	d00f      	beq.n	30004506 <BOOT_OTFMaskConfig+0x5e>
300044e6:	f101 4378 	add.w	r3, r1, #4160749568	; 0xf8000000
300044ea:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300044ee:	d2ec      	bcs.n	300044ca <BOOT_OTFMaskConfig+0x22>
300044f0:	9001      	str	r0, [sp, #4]
300044f2:	2301      	movs	r3, #1
300044f4:	4628      	mov	r0, r5
300044f6:	f8cd c000 	str.w	ip, [sp]
300044fa:	47b8      	blx	r7
300044fc:	e7e8      	b.n	300044d0 <BOOT_OTFMaskConfig+0x28>
300044fe:	4640      	mov	r0, r8
30004500:	f007 fc96 	bl	3000be30 <__DiagPrintf_veneer>
30004504:	e7e4      	b.n	300044d0 <BOOT_OTFMaskConfig+0x28>
30004506:	4b07      	ldr	r3, [pc, #28]	; (30004524 <BOOT_OTFMaskConfig+0x7c>)
30004508:	681b      	ldr	r3, [r3, #0]
3000450a:	0799      	lsls	r1, r3, #30
3000450c:	d402      	bmi.n	30004514 <BOOT_OTFMaskConfig+0x6c>
3000450e:	b002      	add	sp, #8
30004510:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30004514:	4805      	ldr	r0, [pc, #20]	; (3000452c <BOOT_OTFMaskConfig+0x84>)
30004516:	b002      	add	sp, #8
30004518:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
3000451c:	f007 bc88 	b.w	3000be30 <__DiagPrintf_veneer>
30004520:	3000d2ac 	.word	0x3000d2ac
30004524:	2001c00c 	.word	0x2001c00c
30004528:	30009e75 	.word	0x30009e75
3000452c:	3000c3a4 	.word	0x3000c3a4
30004530:	3000c3ec 	.word	0x3000c3ec

30004534 <BOOT_LoadRDPImg>:
30004534:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004538:	4d50      	ldr	r5, [pc, #320]	; (3000467c <BOOT_LoadRDPImg+0x148>)
3000453a:	b0d7      	sub	sp, #348	; 0x15c
3000453c:	4613      	mov	r3, r2
3000453e:	468b      	mov	fp, r1
30004540:	4604      	mov	r4, r0
30004542:	aa04      	add	r2, sp, #16
30004544:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30004548:	e895 0003 	ldmia.w	r5, {r0, r1}
3000454c:	2500      	movs	r5, #0
3000454e:	e882 0003 	stmia.w	r2, {r0, r1}
30004552:	4619      	mov	r1, r3
30004554:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004558:	4620      	mov	r0, r4
3000455a:	f001 fba3 	bl	30005ca4 <BOOT_ImgCopy>
3000455e:	ab04      	add	r3, sp, #16
30004560:	2202      	movs	r2, #2
30004562:	4641      	mov	r1, r8
30004564:	4658      	mov	r0, fp
30004566:	9500      	str	r5, [sp, #0]
30004568:	f7ff fb32 	bl	30003bd0 <BOOT_LoadSubImage>
3000456c:	2801      	cmp	r0, #1
3000456e:	d070      	beq.n	30004652 <BOOT_LoadRDPImg+0x11e>
30004570:	4b43      	ldr	r3, [pc, #268]	; (30004680 <BOOT_LoadRDPImg+0x14c>)
30004572:	4628      	mov	r0, r5
30004574:	4798      	blx	r3
30004576:	2101      	movs	r1, #1
30004578:	4b42      	ldr	r3, [pc, #264]	; (30004684 <BOOT_LoadRDPImg+0x150>)
3000457a:	4628      	mov	r0, r5
3000457c:	4798      	blx	r3
3000457e:	af06      	add	r7, sp, #24
30004580:	2120      	movs	r1, #32
30004582:	4b41      	ldr	r3, [pc, #260]	; (30004688 <BOOT_LoadRDPImg+0x154>)
30004584:	a80e      	add	r0, sp, #56	; 0x38
30004586:	f8df 9118 	ldr.w	r9, [pc, #280]	; 300046a0 <BOOT_LoadRDPImg+0x16c>
3000458a:	4798      	blx	r3
3000458c:	f8df a114 	ldr.w	sl, [pc, #276]	; 300046a4 <BOOT_LoadRDPImg+0x170>
30004590:	f104 0120 	add.w	r1, r4, #32
30004594:	2210      	movs	r2, #16
30004596:	a80a      	add	r0, sp, #40	; 0x28
30004598:	47d0      	blx	sl
3000459a:	9503      	str	r5, [sp, #12]
3000459c:	4641      	mov	r1, r8
3000459e:	a816      	add	r0, sp, #88	; 0x58
300045a0:	2210      	movs	r2, #16
300045a2:	f001 fb7f 	bl	30005ca4 <BOOT_ImgCopy>
300045a6:	2310      	movs	r3, #16
300045a8:	aa0a      	add	r2, sp, #40	; 0x28
300045aa:	a816      	add	r0, sp, #88	; 0x58
300045ac:	4619      	mov	r1, r3
300045ae:	9700      	str	r7, [sp, #0]
300045b0:	47c8      	blx	r9
300045b2:	683b      	ldr	r3, [r7, #0]
300045b4:	4a35      	ldr	r2, [pc, #212]	; (3000468c <BOOT_LoadRDPImg+0x158>)
300045b6:	4293      	cmp	r3, r2
300045b8:	4b35      	ldr	r3, [pc, #212]	; (30004690 <BOOT_LoadRDPImg+0x15c>)
300045ba:	d14e      	bne.n	3000465a <BOOT_LoadRDPImg+0x126>
300045bc:	687a      	ldr	r2, [r7, #4]
300045be:	4935      	ldr	r1, [pc, #212]	; (30004694 <BOOT_LoadRDPImg+0x160>)
300045c0:	689b      	ldr	r3, [r3, #8]
300045c2:	428a      	cmp	r2, r1
300045c4:	f003 0302 	and.w	r3, r3, #2
300045c8:	d155      	bne.n	30004676 <BOOT_LoadRDPImg+0x142>
300045ca:	68fe      	ldr	r6, [r7, #12]
300045cc:	68bd      	ldr	r5, [r7, #8]
300045ce:	3e20      	subs	r6, #32
300045d0:	3520      	adds	r5, #32
300045d2:	2b00      	cmp	r3, #0
300045d4:	d133      	bne.n	3000463e <BOOT_LoadRDPImg+0x10a>
300045d6:	f1bb 0f00 	cmp.w	fp, #0
300045da:	d006      	beq.n	300045ea <BOOT_LoadRDPImg+0xb6>
300045dc:	9a03      	ldr	r2, [sp, #12]
300045de:	f10b 0304 	add.w	r3, fp, #4
300045e2:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
300045e6:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
300045ea:	b1dd      	cbz	r5, 30004624 <BOOT_LoadRDPImg+0xf0>
300045ec:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300045f0:	462c      	mov	r4, r5
300045f2:	4641      	mov	r1, r8
300045f4:	a816      	add	r0, sp, #88	; 0x58
300045f6:	bf28      	it	cs
300045f8:	f44f 7480 	movcs.w	r4, #256	; 0x100
300045fc:	4622      	mov	r2, r4
300045fe:	44a0      	add	r8, r4
30004600:	f001 fb50 	bl	30005ca4 <BOOT_ImgCopy>
30004604:	aa0a      	add	r2, sp, #40	; 0x28
30004606:	4621      	mov	r1, r4
30004608:	2310      	movs	r3, #16
3000460a:	9600      	str	r6, [sp, #0]
3000460c:	a816      	add	r0, sp, #88	; 0x58
3000460e:	47c8      	blx	r9
30004610:	ab16      	add	r3, sp, #88	; 0x58
30004612:	f1a4 0110 	sub.w	r1, r4, #16
30004616:	2210      	movs	r2, #16
30004618:	a80a      	add	r0, sp, #40	; 0x28
3000461a:	4426      	add	r6, r4
3000461c:	4419      	add	r1, r3
3000461e:	47d0      	blx	sl
30004620:	1b2d      	subs	r5, r5, r4
30004622:	d1e3      	bne.n	300045ec <BOOT_LoadRDPImg+0xb8>
30004624:	9b03      	ldr	r3, [sp, #12]
30004626:	b13b      	cbz	r3, 30004638 <BOOT_LoadRDPImg+0x104>
30004628:	2100      	movs	r1, #0
3000462a:	4b16      	ldr	r3, [pc, #88]	; (30004684 <BOOT_LoadRDPImg+0x150>)
3000462c:	4608      	mov	r0, r1
3000462e:	4798      	blx	r3
30004630:	2002      	movs	r0, #2
30004632:	b057      	add	sp, #348	; 0x15c
30004634:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004638:	2301      	movs	r3, #1
3000463a:	9303      	str	r3, [sp, #12]
3000463c:	e7ae      	b.n	3000459c <BOOT_LoadRDPImg+0x68>
3000463e:	a804      	add	r0, sp, #16
30004640:	9903      	ldr	r1, [sp, #12]
30004642:	462b      	mov	r3, r5
30004644:	4632      	mov	r2, r6
30004646:	f850 1021 	ldr.w	r1, [r0, r1, lsl #2]
3000464a:	4813      	ldr	r0, [pc, #76]	; (30004698 <BOOT_LoadRDPImg+0x164>)
3000464c:	f007 fbf0 	bl	3000be30 <__DiagPrintf_veneer>
30004650:	e7c1      	b.n	300045d6 <BOOT_LoadRDPImg+0xa2>
30004652:	2002      	movs	r0, #2
30004654:	b057      	add	sp, #348	; 0x15c
30004656:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000465a:	6898      	ldr	r0, [r3, #8]
3000465c:	f000 0002 	and.w	r0, r0, #2
30004660:	2800      	cmp	r0, #0
30004662:	d0e6      	beq.n	30004632 <BOOT_LoadRDPImg+0xfe>
30004664:	9b03      	ldr	r3, [sp, #12]
30004666:	aa04      	add	r2, sp, #16
30004668:	480c      	ldr	r0, [pc, #48]	; (3000469c <BOOT_LoadRDPImg+0x168>)
3000466a:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
3000466e:	f007 fbdf 	bl	3000be30 <__DiagPrintf_veneer>
30004672:	2000      	movs	r0, #0
30004674:	e7dd      	b.n	30004632 <BOOT_LoadRDPImg+0xfe>
30004676:	4618      	mov	r0, r3
30004678:	e7f2      	b.n	30004660 <BOOT_LoadRDPImg+0x12c>
3000467a:	bf00      	nop
3000467c:	3000d214 	.word	0x3000d214
30004680:	00002429 	.word	0x00002429
30004684:	00002479 	.word	0x00002479
30004688:	00004641 	.word	0x00004641
3000468c:	35393138 	.word	0x35393138
30004690:	2001c00c 	.word	0x2001c00c
30004694:	31313738 	.word	0x31313738
30004698:	3000c1c4 	.word	0x3000c1c4
3000469c:	3000c19c 	.word	0x3000c19c
300046a0:	000046f5 	.word	0x000046f5
300046a4:	00012a1d 	.word	0x00012a1d

300046a8 <BOOT_LoadBL32RDPImg>:
300046a8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300046ac:	4b28      	ldr	r3, [pc, #160]	; (30004750 <BOOT_LoadBL32RDPImg+0xa8>)
300046ae:	b0d1      	sub	sp, #324	; 0x144
300046b0:	f8df c0ac 	ldr.w	ip, [pc, #172]	; 30004760 <BOOT_LoadBL32RDPImg+0xb8>
300046b4:	4607      	mov	r7, r0
300046b6:	f10d 0810 	add.w	r8, sp, #16
300046ba:	460e      	mov	r6, r1
300046bc:	4615      	mov	r5, r2
300046be:	ac02      	add	r4, sp, #8
300046c0:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 30004764 <BOOT_LoadBL32RDPImg+0xbc>
300046c4:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 30004768 <BOOT_LoadBL32RDPImg+0xc0>
300046c8:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300046ca:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
300046ce:	e89c 0003 	ldmia.w	ip, {r0, r1}
300046d2:	e884 0003 	stmia.w	r4, {r0, r1}
300046d6:	2000      	movs	r0, #0
300046d8:	47c8      	blx	r9
300046da:	2101      	movs	r1, #1
300046dc:	2000      	movs	r0, #0
300046de:	47d8      	blx	fp
300046e0:	2120      	movs	r1, #32
300046e2:	4b1c      	ldr	r3, [pc, #112]	; (30004754 <BOOT_LoadBL32RDPImg+0xac>)
300046e4:	eb0d 0001 	add.w	r0, sp, r1
300046e8:	4798      	blx	r3
300046ea:	4b1b      	ldr	r3, [pc, #108]	; (30004758 <BOOT_LoadBL32RDPImg+0xb0>)
300046ec:	689b      	ldr	r3, [r3, #8]
300046ee:	079b      	lsls	r3, r3, #30
300046f0:	d427      	bmi.n	30004742 <BOOT_LoadBL32RDPImg+0x9a>
300046f2:	b1fd      	cbz	r5, 30004734 <BOOT_LoadBL32RDPImg+0x8c>
300046f4:	f8df a074 	ldr.w	sl, [pc, #116]	; 3000476c <BOOT_LoadBL32RDPImg+0xc4>
300046f8:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30004770 <BOOT_LoadBL32RDPImg+0xc8>
300046fc:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30004700:	462c      	mov	r4, r5
30004702:	4639      	mov	r1, r7
30004704:	a810      	add	r0, sp, #64	; 0x40
30004706:	bf28      	it	cs
30004708:	f44f 7480 	movcs.w	r4, #256	; 0x100
3000470c:	4622      	mov	r2, r4
3000470e:	4427      	add	r7, r4
30004710:	f001 fac8 	bl	30005ca4 <BOOT_ImgCopy>
30004714:	4642      	mov	r2, r8
30004716:	4621      	mov	r1, r4
30004718:	2310      	movs	r3, #16
3000471a:	9600      	str	r6, [sp, #0]
3000471c:	a810      	add	r0, sp, #64	; 0x40
3000471e:	47d0      	blx	sl
30004720:	ab10      	add	r3, sp, #64	; 0x40
30004722:	f1a4 0110 	sub.w	r1, r4, #16
30004726:	2210      	movs	r2, #16
30004728:	4640      	mov	r0, r8
3000472a:	4426      	add	r6, r4
3000472c:	4419      	add	r1, r3
3000472e:	47c8      	blx	r9
30004730:	1b2d      	subs	r5, r5, r4
30004732:	d1e3      	bne.n	300046fc <BOOT_LoadBL32RDPImg+0x54>
30004734:	2100      	movs	r1, #0
30004736:	4608      	mov	r0, r1
30004738:	47d8      	blx	fp
3000473a:	2001      	movs	r0, #1
3000473c:	b051      	add	sp, #324	; 0x144
3000473e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004742:	4621      	mov	r1, r4
30004744:	462b      	mov	r3, r5
30004746:	4632      	mov	r2, r6
30004748:	4804      	ldr	r0, [pc, #16]	; (3000475c <BOOT_LoadBL32RDPImg+0xb4>)
3000474a:	f007 fb71 	bl	3000be30 <__DiagPrintf_veneer>
3000474e:	e7d0      	b.n	300046f2 <BOOT_LoadBL32RDPImg+0x4a>
30004750:	3000c424 	.word	0x3000c424
30004754:	00004641 	.word	0x00004641
30004758:	2001c00c 	.word	0x2001c00c
3000475c:	3000c1c4 	.word	0x3000c1c4
30004760:	3000c438 	.word	0x3000c438
30004764:	00002429 	.word	0x00002429
30004768:	00002479 	.word	0x00002479
3000476c:	000046f5 	.word	0x000046f5
30004770:	00012a1d 	.word	0x00012a1d

30004774 <_BOOT_Validate_ImgHash>:
30004774:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004778:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
3000477c:	4605      	mov	r5, r0
3000477e:	4690      	mov	r8, r2
30004780:	22e0      	movs	r2, #224	; 0xe0
30004782:	9103      	str	r1, [sp, #12]
30004784:	2100      	movs	r1, #0
30004786:	a818      	add	r0, sp, #96	; 0x60
30004788:	461c      	mov	r4, r3
3000478a:	f88d 101f 	strb.w	r1, [sp, #31]
3000478e:	f007 fb2f 	bl	3000bdf0 <____wrap_memset_veneer>
30004792:	4628      	mov	r0, r5
30004794:	f10d 021f 	add.w	r2, sp, #31
30004798:	f10d 011e 	add.w	r1, sp, #30
3000479c:	f007 fb10 	bl	3000bdc0 <__SBOOT_GetMdType_veneer>
300047a0:	9001      	str	r0, [sp, #4]
300047a2:	2800      	cmp	r0, #0
300047a4:	d166      	bne.n	30004874 <_BOOT_Validate_ImgHash+0x100>
300047a6:	f89d 301f 	ldrb.w	r3, [sp, #31]
300047aa:	2b00      	cmp	r3, #0
300047ac:	d067      	beq.n	3000487e <_BOOT_Validate_ImgHash+0x10a>
300047ae:	2101      	movs	r1, #1
300047b0:	4605      	mov	r5, r0
300047b2:	4b80      	ldr	r3, [pc, #512]	; (300049b4 <_BOOT_Validate_ImgHash+0x240>)
300047b4:	4608      	mov	r0, r1
300047b6:	4798      	blx	r3
300047b8:	4629      	mov	r1, r5
300047ba:	ab18      	add	r3, sp, #96	; 0x60
300047bc:	2220      	movs	r2, #32
300047be:	f89d 001e 	ldrb.w	r0, [sp, #30]
300047c2:	4d7d      	ldr	r5, [pc, #500]	; (300049b8 <_BOOT_Validate_ImgHash+0x244>)
300047c4:	47a8      	blx	r5
300047c6:	2800      	cmp	r0, #0
300047c8:	f040 808b 	bne.w	300048e2 <_BOOT_Validate_ImgHash+0x16e>
300047cc:	b32c      	cbz	r4, 3000481a <_BOOT_Validate_ImgHash+0xa6>
300047ce:	aad0      	add	r2, sp, #832	; 0x340
300047d0:	1e63      	subs	r3, r4, #1
300047d2:	ae50      	add	r6, sp, #320	; 0x140
300047d4:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 300049d0 <_BOOT_Validate_ImgHash+0x25c>
300047d8:	f012 0218 	ands.w	r2, r2, #24
300047dc:	b2db      	uxtb	r3, r3
300047de:	f026 091f 	bic.w	r9, r6, #31
300047e2:	4d76      	ldr	r5, [pc, #472]	; (300049bc <_BOOT_Validate_ImgHash+0x248>)
300047e4:	9204      	str	r2, [sp, #16]
300047e6:	f108 0208 	add.w	r2, r8, #8
300047ea:	bf08      	it	eq
300047ec:	46b1      	moveq	r9, r6
300047ee:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300047f2:	9302      	str	r3, [sp, #8]
300047f4:	e9d8 b700 	ldrd	fp, r7, [r8]
300047f8:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300047fc:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004800:	f0c0 80a7 	bcc.w	30004952 <_BOOT_Validate_ImgHash+0x1de>
30004804:	4639      	mov	r1, r7
30004806:	4658      	mov	r0, fp
30004808:	aa18      	add	r2, sp, #96	; 0x60
3000480a:	47d0      	blx	sl
3000480c:	2800      	cmp	r0, #0
3000480e:	d168      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
30004810:	f108 0808 	add.w	r8, r8, #8
30004814:	9b02      	ldr	r3, [sp, #8]
30004816:	4543      	cmp	r3, r8
30004818:	d1ec      	bne.n	300047f4 <_BOOT_Validate_ImgHash+0x80>
3000481a:	ac08      	add	r4, sp, #32
3000481c:	a918      	add	r1, sp, #96	; 0x60
3000481e:	4b68      	ldr	r3, [pc, #416]	; (300049c0 <_BOOT_Validate_ImgHash+0x24c>)
30004820:	4620      	mov	r0, r4
30004822:	4798      	blx	r3
30004824:	4601      	mov	r1, r0
30004826:	2800      	cmp	r0, #0
30004828:	d15b      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
3000482a:	2001      	movs	r0, #1
3000482c:	4b61      	ldr	r3, [pc, #388]	; (300049b4 <_BOOT_Validate_ImgHash+0x240>)
3000482e:	4798      	blx	r3
30004830:	f89d 501e 	ldrb.w	r5, [sp, #30]
30004834:	4b63      	ldr	r3, [pc, #396]	; (300049c4 <_BOOT_Validate_ImgHash+0x250>)
30004836:	4798      	blx	r3
30004838:	fbb0 f3f5 	udiv	r3, r0, r5
3000483c:	fb03 0315 	mls	r3, r3, r5, r0
30004840:	b2db      	uxtb	r3, r3
30004842:	b1bd      	cbz	r5, 30004874 <_BOOT_Validate_ImgHash+0x100>
30004844:	2000      	movs	r0, #0
30004846:	9f03      	ldr	r7, [sp, #12]
30004848:	4606      	mov	r6, r0
3000484a:	1c5a      	adds	r2, r3, #1
3000484c:	5cf9      	ldrb	r1, [r7, r3]
3000484e:	5ce3      	ldrb	r3, [r4, r3]
30004850:	3001      	adds	r0, #1
30004852:	b2d2      	uxtb	r2, r2
30004854:	404b      	eors	r3, r1
30004856:	431e      	orrs	r6, r3
30004858:	fbb2 f3f5 	udiv	r3, r2, r5
3000485c:	fb05 2213 	mls	r2, r5, r3, r2
30004860:	b2d3      	uxtb	r3, r2
30004862:	b2c2      	uxtb	r2, r0
30004864:	4295      	cmp	r5, r2
30004866:	d8f0      	bhi.n	3000484a <_BOOT_Validate_ImgHash+0xd6>
30004868:	2e00      	cmp	r6, #0
3000486a:	9b01      	ldr	r3, [sp, #4]
3000486c:	bf18      	it	ne
3000486e:	f06f 0306 	mvnne.w	r3, #6
30004872:	9301      	str	r3, [sp, #4]
30004874:	9801      	ldr	r0, [sp, #4]
30004876:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
3000487a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000487e:	a918      	add	r1, sp, #96	; 0x60
30004880:	f89d 001e 	ldrb.w	r0, [sp, #30]
30004884:	4b50      	ldr	r3, [pc, #320]	; (300049c8 <_BOOT_Validate_ImgHash+0x254>)
30004886:	4798      	blx	r3
30004888:	bb58      	cbnz	r0, 300048e2 <_BOOT_Validate_ImgHash+0x16e>
3000488a:	b31c      	cbz	r4, 300048d4 <_BOOT_Validate_ImgHash+0x160>
3000488c:	aad0      	add	r2, sp, #832	; 0x340
3000488e:	1e63      	subs	r3, r4, #1
30004890:	ae50      	add	r6, sp, #320	; 0x140
30004892:	f8df a140 	ldr.w	sl, [pc, #320]	; 300049d4 <_BOOT_Validate_ImgHash+0x260>
30004896:	f012 0218 	ands.w	r2, r2, #24
3000489a:	b2db      	uxtb	r3, r3
3000489c:	f026 091f 	bic.w	r9, r6, #31
300048a0:	4d46      	ldr	r5, [pc, #280]	; (300049bc <_BOOT_Validate_ImgHash+0x248>)
300048a2:	9204      	str	r2, [sp, #16]
300048a4:	f108 0208 	add.w	r2, r8, #8
300048a8:	bf08      	it	eq
300048aa:	46b1      	moveq	r9, r6
300048ac:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300048b0:	9302      	str	r3, [sp, #8]
300048b2:	e9d8 b700 	ldrd	fp, r7, [r8]
300048b6:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300048ba:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300048be:	d318      	bcc.n	300048f2 <_BOOT_Validate_ImgHash+0x17e>
300048c0:	4639      	mov	r1, r7
300048c2:	4658      	mov	r0, fp
300048c4:	aa18      	add	r2, sp, #96	; 0x60
300048c6:	47d0      	blx	sl
300048c8:	b958      	cbnz	r0, 300048e2 <_BOOT_Validate_ImgHash+0x16e>
300048ca:	f108 0808 	add.w	r8, r8, #8
300048ce:	9b02      	ldr	r3, [sp, #8]
300048d0:	4543      	cmp	r3, r8
300048d2:	d1ee      	bne.n	300048b2 <_BOOT_Validate_ImgHash+0x13e>
300048d4:	ac08      	add	r4, sp, #32
300048d6:	a918      	add	r1, sp, #96	; 0x60
300048d8:	4b3c      	ldr	r3, [pc, #240]	; (300049cc <_BOOT_Validate_ImgHash+0x258>)
300048da:	4620      	mov	r0, r4
300048dc:	4798      	blx	r3
300048de:	2800      	cmp	r0, #0
300048e0:	d0a6      	beq.n	30004830 <_BOOT_Validate_ImgHash+0xbc>
300048e2:	f06f 030a 	mvn.w	r3, #10
300048e6:	9301      	str	r3, [sp, #4]
300048e8:	9801      	ldr	r0, [sp, #4]
300048ea:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300048ee:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300048f2:	2f00      	cmp	r7, #0
300048f4:	d0e9      	beq.n	300048ca <_BOOT_Validate_ImgHash+0x156>
300048f6:	9c04      	ldr	r4, [sp, #16]
300048f8:	f8cd 8014 	str.w	r8, [sp, #20]
300048fc:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004900:	46b8      	mov	r8, r7
30004902:	4659      	mov	r1, fp
30004904:	4630      	mov	r0, r6
30004906:	bf28      	it	cs
30004908:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000490c:	4642      	mov	r2, r8
3000490e:	f001 f9c9 	bl	30005ca4 <BOOT_ImgCopy>
30004912:	696a      	ldr	r2, [r5, #20]
30004914:	03d3      	lsls	r3, r2, #15
30004916:	d50f      	bpl.n	30004938 <_BOOT_Validate_ImgHash+0x1c4>
30004918:	eb08 0004 	add.w	r0, r8, r4
3000491c:	f3bf 8f4f 	dsb	sy
30004920:	4448      	add	r0, r9
30004922:	464a      	mov	r2, r9
30004924:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30004928:	3220      	adds	r2, #32
3000492a:	1a83      	subs	r3, r0, r2
3000492c:	2b00      	cmp	r3, #0
3000492e:	dcf9      	bgt.n	30004924 <_BOOT_Validate_ImgHash+0x1b0>
30004930:	f3bf 8f4f 	dsb	sy
30004934:	f3bf 8f6f 	isb	sy
30004938:	aa18      	add	r2, sp, #96	; 0x60
3000493a:	4641      	mov	r1, r8
3000493c:	4630      	mov	r0, r6
3000493e:	47d0      	blx	sl
30004940:	2800      	cmp	r0, #0
30004942:	d1ce      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
30004944:	ebb7 0708 	subs.w	r7, r7, r8
30004948:	44c3      	add	fp, r8
3000494a:	d1d7      	bne.n	300048fc <_BOOT_Validate_ImgHash+0x188>
3000494c:	f8dd 8014 	ldr.w	r8, [sp, #20]
30004950:	e7bb      	b.n	300048ca <_BOOT_Validate_ImgHash+0x156>
30004952:	2f00      	cmp	r7, #0
30004954:	f43f af5c 	beq.w	30004810 <_BOOT_Validate_ImgHash+0x9c>
30004958:	9c04      	ldr	r4, [sp, #16]
3000495a:	f8cd 8014 	str.w	r8, [sp, #20]
3000495e:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004962:	46b8      	mov	r8, r7
30004964:	4659      	mov	r1, fp
30004966:	4630      	mov	r0, r6
30004968:	bf28      	it	cs
3000496a:	f44f 7800 	movcs.w	r8, #512	; 0x200
3000496e:	4642      	mov	r2, r8
30004970:	f001 f998 	bl	30005ca4 <BOOT_ImgCopy>
30004974:	696a      	ldr	r2, [r5, #20]
30004976:	03d2      	lsls	r2, r2, #15
30004978:	d50f      	bpl.n	3000499a <_BOOT_Validate_ImgHash+0x226>
3000497a:	eb08 0004 	add.w	r0, r8, r4
3000497e:	f3bf 8f4f 	dsb	sy
30004982:	4448      	add	r0, r9
30004984:	464a      	mov	r2, r9
30004986:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
3000498a:	3220      	adds	r2, #32
3000498c:	1a83      	subs	r3, r0, r2
3000498e:	2b00      	cmp	r3, #0
30004990:	dcf9      	bgt.n	30004986 <_BOOT_Validate_ImgHash+0x212>
30004992:	f3bf 8f4f 	dsb	sy
30004996:	f3bf 8f6f 	isb	sy
3000499a:	aa18      	add	r2, sp, #96	; 0x60
3000499c:	4641      	mov	r1, r8
3000499e:	4630      	mov	r0, r6
300049a0:	47d0      	blx	sl
300049a2:	2800      	cmp	r0, #0
300049a4:	d19d      	bne.n	300048e2 <_BOOT_Validate_ImgHash+0x16e>
300049a6:	ebb7 0708 	subs.w	r7, r7, r8
300049aa:	44c3      	add	fp, r8
300049ac:	d1d7      	bne.n	3000495e <_BOOT_Validate_ImgHash+0x1ea>
300049ae:	f8dd 8014 	ldr.w	r8, [sp, #20]
300049b2:	e72d      	b.n	30004810 <_BOOT_Validate_ImgHash+0x9c>
300049b4:	000024dd 	.word	0x000024dd
300049b8:	30009ab9 	.word	0x30009ab9
300049bc:	e000ed00 	.word	0xe000ed00
300049c0:	00005bb9 	.word	0x00005bb9
300049c4:	0000ea4d 	.word	0x0000ea4d
300049c8:	00004ffd 	.word	0x00004ffd
300049cc:	000053f1 	.word	0x000053f1
300049d0:	00005b4d 	.word	0x00005b4d
300049d4:	00005385 	.word	0x00005385

300049d8 <BOOT_Validate_PubKey>:
300049d8:	f007 ba32 	b.w	3000be40 <__SBOOT_Validate_PubKey_veneer>

300049dc <BOOT_Validate_Signature>:
300049dc:	f007 ba00 	b.w	3000bde0 <__SBOOT_Validate_Signature_veneer>

300049e0 <BOOT_Validate_ImgHash>:
300049e0:	b530      	push	{r4, r5, lr}
300049e2:	b085      	sub	sp, #20
300049e4:	4d0a      	ldr	r5, [pc, #40]	; (30004a10 <BOOT_Validate_ImgHash+0x30>)
300049e6:	4604      	mov	r4, r0
300049e8:	9301      	str	r3, [sp, #4]
300049ea:	e9cd 2102 	strd	r2, r1, [sp, #8]
300049ee:	47a8      	blx	r5
300049f0:	9903      	ldr	r1, [sp, #12]
300049f2:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300049f6:	b128      	cbz	r0, 30004a04 <BOOT_Validate_ImgHash+0x24>
300049f8:	4620      	mov	r0, r4
300049fa:	b005      	add	sp, #20
300049fc:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a00:	f007 ba0e 	b.w	3000be20 <__SBOOT_Validate_ImgHash_veneer>
30004a04:	4620      	mov	r0, r4
30004a06:	b005      	add	sp, #20
30004a08:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a0c:	f7ff beb2 	b.w	30004774 <_BOOT_Validate_ImgHash>
30004a10:	3000a149 	.word	0x3000a149

30004a14 <BOOT_SignatureCheck>:
30004a14:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30004a18:	461e      	mov	r6, r3
30004a1a:	b08b      	sub	sp, #44	; 0x2c
30004a1c:	4b7e      	ldr	r3, [pc, #504]	; (30004c18 <BOOT_SignatureCheck+0x204>)
30004a1e:	4680      	mov	r8, r0
30004a20:	4689      	mov	r9, r1
30004a22:	4617      	mov	r7, r2
30004a24:	f10d 0c28 	add.w	ip, sp, #40	; 0x28
30004a28:	4c7c      	ldr	r4, [pc, #496]	; (30004c1c <BOOT_SignatureCheck+0x208>)
30004a2a:	9d12      	ldr	r5, [sp, #72]	; 0x48
30004a2c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004a2e:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30004a32:	47a0      	blx	r4
30004a34:	b918      	cbnz	r0, 30004a3e <BOOT_SignatureCheck+0x2a>
30004a36:	2000      	movs	r0, #0
30004a38:	b00b      	add	sp, #44	; 0x2c
30004a3a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004a3e:	4b78      	ldr	r3, [pc, #480]	; (30004c20 <BOOT_SignatureCheck+0x20c>)
30004a40:	2000      	movs	r0, #0
30004a42:	4798      	blx	r3
30004a44:	f898 2012 	ldrb.w	r2, [r8, #18]
30004a48:	f898 3013 	ldrb.w	r3, [r8, #19]
30004a4c:	f10d 0117 	add.w	r1, sp, #23
30004a50:	f10d 0016 	add.w	r0, sp, #22
30004a54:	f007 f9bc 	bl	3000bdd0 <__SBOOT_Validate_Algorithm_veneer>
30004a58:	4602      	mov	r2, r0
30004a5a:	b9e8      	cbnz	r0, 30004a98 <BOOT_SignatureCheck+0x84>
30004a5c:	69f0      	ldr	r0, [r6, #28]
30004a5e:	b918      	cbnz	r0, 30004a68 <BOOT_SignatureCheck+0x54>
30004a60:	e0d7      	b.n	30004c12 <BOOT_SignatureCheck+0x1fe>
30004a62:	b2ca      	uxtb	r2, r1
30004a64:	4282      	cmp	r2, r0
30004a66:	d208      	bcs.n	30004a7a <BOOT_SignatureCheck+0x66>
30004a68:	eb02 04c2 	add.w	r4, r2, r2, lsl #3
30004a6c:	1c51      	adds	r1, r2, #1
30004a6e:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30004a72:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30004a76:	42ab      	cmp	r3, r5
30004a78:	d1f3      	bne.n	30004a62 <BOOT_SignatureCheck+0x4e>
30004a7a:	2124      	movs	r1, #36	; 0x24
30004a7c:	23b4      	movs	r3, #180	; 0xb4
30004a7e:	fb11 3202 	smlabb	r2, r1, r2, r3
30004a82:	f108 0470 	add.w	r4, r8, #112	; 0x70
30004a86:	4432      	add	r2, r6
30004a88:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004a8c:	4621      	mov	r1, r4
30004a8e:	f007 f9d7 	bl	3000be40 <__SBOOT_Validate_PubKey_veneer>
30004a92:	4602      	mov	r2, r0
30004a94:	2800      	cmp	r0, #0
30004a96:	d072      	beq.n	30004b7e <BOOT_SignatureCheck+0x16a>
30004a98:	4b62      	ldr	r3, [pc, #392]	; (30004c24 <BOOT_SignatureCheck+0x210>)
30004a9a:	681b      	ldr	r3, [r3, #0]
30004a9c:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004aa0:	2b00      	cmp	r3, #0
30004aa2:	f040 80a4 	bne.w	30004bee <BOOT_SignatureCheck+0x1da>
30004aa6:	2f00      	cmp	r7, #0
30004aa8:	d045      	beq.n	30004b36 <BOOT_SignatureCheck+0x122>
30004aaa:	464d      	mov	r5, r9
30004aac:	f107 39ff 	add.w	r9, r7, #4294967295
30004ab0:	f04f 0800 	mov.w	r8, #0
30004ab4:	4f5c      	ldr	r7, [pc, #368]	; (30004c28 <BOOT_SignatureCheck+0x214>)
30004ab6:	fa5f f389 	uxtb.w	r3, r9
30004aba:	f105 0908 	add.w	r9, r5, #8
30004abe:	1d2e      	adds	r6, r5, #4
30004ac0:	4c5a      	ldr	r4, [pc, #360]	; (30004c2c <BOOT_SignatureCheck+0x218>)
30004ac2:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30004ac6:	e003      	b.n	30004ad0 <BOOT_SignatureCheck+0xbc>
30004ac8:	3508      	adds	r5, #8
30004aca:	3608      	adds	r6, #8
30004acc:	45a9      	cmp	r9, r5
30004ace:	d032      	beq.n	30004b36 <BOOT_SignatureCheck+0x122>
30004ad0:	f856 0c04 	ldr.w	r0, [r6, #-4]
30004ad4:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30004ad8:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30004adc:	d3f4      	bcc.n	30004ac8 <BOOT_SignatureCheck+0xb4>
30004ade:	2100      	movs	r1, #0
30004ae0:	686a      	ldr	r2, [r5, #4]
30004ae2:	47b8      	blx	r7
30004ae4:	6963      	ldr	r3, [r4, #20]
30004ae6:	03d9      	lsls	r1, r3, #15
30004ae8:	d5ee      	bpl.n	30004ac8 <BOOT_SignatureCheck+0xb4>
30004aea:	f856 2c04 	ldr.w	r2, [r6, #-4]
30004aee:	686b      	ldr	r3, [r5, #4]
30004af0:	ea02 0103 	and.w	r1, r2, r3
30004af4:	3101      	adds	r1, #1
30004af6:	d022      	beq.n	30004b3e <BOOT_SignatureCheck+0x12a>
30004af8:	f012 011f 	ands.w	r1, r2, #31
30004afc:	d002      	beq.n	30004b04 <BOOT_SignatureCheck+0xf0>
30004afe:	440b      	add	r3, r1
30004b00:	f022 021f 	bic.w	r2, r2, #31
30004b04:	f3bf 8f4f 	dsb	sy
30004b08:	2b00      	cmp	r3, #0
30004b0a:	dd0c      	ble.n	30004b26 <BOOT_SignatureCheck+0x112>
30004b0c:	1e59      	subs	r1, r3, #1
30004b0e:	f102 0320 	add.w	r3, r2, #32
30004b12:	f021 011f 	bic.w	r1, r1, #31
30004b16:	4419      	add	r1, r3
30004b18:	428b      	cmp	r3, r1
30004b1a:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
30004b1e:	461a      	mov	r2, r3
30004b20:	f103 0320 	add.w	r3, r3, #32
30004b24:	d1f8      	bne.n	30004b18 <BOOT_SignatureCheck+0x104>
30004b26:	f3bf 8f4f 	dsb	sy
30004b2a:	f3bf 8f6f 	isb	sy
30004b2e:	3508      	adds	r5, #8
30004b30:	3608      	adds	r6, #8
30004b32:	45a9      	cmp	r9, r5
30004b34:	d1cc      	bne.n	30004ad0 <BOOT_SignatureCheck+0xbc>
30004b36:	20ff      	movs	r0, #255	; 0xff
30004b38:	b00b      	add	sp, #44	; 0x2c
30004b3a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30004b3e:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30004b42:	f3bf 8f4f 	dsb	sy
30004b46:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30004b4a:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004b4e:	f3c3 304e 	ubfx	r0, r3, #13, #15
30004b52:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30004b56:	0140      	lsls	r0, r0, #5
30004b58:	ea00 010e 	and.w	r1, r0, lr
30004b5c:	4663      	mov	r3, ip
30004b5e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004b62:	3b01      	subs	r3, #1
30004b64:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30004b68:	1c5a      	adds	r2, r3, #1
30004b6a:	d1f8      	bne.n	30004b5e <BOOT_SignatureCheck+0x14a>
30004b6c:	3820      	subs	r0, #32
30004b6e:	f110 0f20 	cmn.w	r0, #32
30004b72:	d1f1      	bne.n	30004b58 <BOOT_SignatureCheck+0x144>
30004b74:	f3bf 8f4f 	dsb	sy
30004b78:	f3bf 8f6f 	isb	sy
30004b7c:	e7d7      	b.n	30004b2e <BOOT_SignatureCheck+0x11a>
30004b7e:	f508 7380 	add.w	r3, r8, #256	; 0x100
30004b82:	4622      	mov	r2, r4
30004b84:	f89d 1017 	ldrb.w	r1, [sp, #23]
30004b88:	9301      	str	r3, [sp, #4]
30004b8a:	f44f 7380 	mov.w	r3, #256	; 0x100
30004b8e:	f89d 0016 	ldrb.w	r0, [sp, #22]
30004b92:	9300      	str	r3, [sp, #0]
30004b94:	4643      	mov	r3, r8
30004b96:	f007 f923 	bl	3000bde0 <__SBOOT_Validate_Signature_veneer>
30004b9a:	4602      	mov	r2, r0
30004b9c:	2800      	cmp	r0, #0
30004b9e:	f47f af7b 	bne.w	30004a98 <BOOT_SignatureCheck+0x84>
30004ba2:	f108 0530 	add.w	r5, r8, #48	; 0x30
30004ba6:	4b22      	ldr	r3, [pc, #136]	; (30004c30 <BOOT_SignatureCheck+0x21c>)
30004ba8:	f89d 4017 	ldrb.w	r4, [sp, #23]
30004bac:	4798      	blx	r3
30004bae:	4629      	mov	r1, r5
30004bb0:	bb40      	cbnz	r0, 30004c04 <BOOT_SignatureCheck+0x1f0>
30004bb2:	464a      	mov	r2, r9
30004bb4:	4620      	mov	r0, r4
30004bb6:	463b      	mov	r3, r7
30004bb8:	f7ff fddc 	bl	30004774 <_BOOT_Validate_ImgHash>
30004bbc:	4602      	mov	r2, r0
30004bbe:	4b19      	ldr	r3, [pc, #100]	; (30004c24 <BOOT_SignatureCheck+0x210>)
30004bc0:	681b      	ldr	r3, [r3, #0]
30004bc2:	f403 3300 	and.w	r3, r3, #131072	; 0x20000
30004bc6:	2a00      	cmp	r2, #0
30004bc8:	f47f af6a 	bne.w	30004aa0 <BOOT_SignatureCheck+0x8c>
30004bcc:	2b00      	cmp	r3, #0
30004bce:	f43f af32 	beq.w	30004a36 <BOOT_SignatureCheck+0x22>
30004bd2:	f898 3011 	ldrb.w	r3, [r8, #17]
30004bd6:	9203      	str	r2, [sp, #12]
30004bd8:	aa0a      	add	r2, sp, #40	; 0x28
30004bda:	4816      	ldr	r0, [pc, #88]	; (30004c34 <BOOT_SignatureCheck+0x220>)
30004bdc:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30004be0:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004be4:	f007 f924 	bl	3000be30 <__DiagPrintf_veneer>
30004be8:	9a03      	ldr	r2, [sp, #12]
30004bea:	4610      	mov	r0, r2
30004bec:	e724      	b.n	30004a38 <BOOT_SignatureCheck+0x24>
30004bee:	a90a      	add	r1, sp, #40	; 0x28
30004bf0:	f898 3011 	ldrb.w	r3, [r8, #17]
30004bf4:	4810      	ldr	r0, [pc, #64]	; (30004c38 <BOOT_SignatureCheck+0x224>)
30004bf6:	eb01 0383 	add.w	r3, r1, r3, lsl #2
30004bfa:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004bfe:	f007 f917 	bl	3000be30 <__DiagPrintf_veneer>
30004c02:	e750      	b.n	30004aa6 <BOOT_SignatureCheck+0x92>
30004c04:	464a      	mov	r2, r9
30004c06:	4620      	mov	r0, r4
30004c08:	463b      	mov	r3, r7
30004c0a:	f007 f909 	bl	3000be20 <__SBOOT_Validate_ImgHash_veneer>
30004c0e:	4602      	mov	r2, r0
30004c10:	e7d5      	b.n	30004bbe <BOOT_SignatureCheck+0x1aa>
30004c12:	22b4      	movs	r2, #180	; 0xb4
30004c14:	e735      	b.n	30004a82 <BOOT_SignatureCheck+0x6e>
30004c16:	bf00      	nop
30004c18:	3000d21c 	.word	0x3000d21c
30004c1c:	0000c149 	.word	0x0000c149
30004c20:	00002451 	.word	0x00002451
30004c24:	2001c00c 	.word	0x2001c00c
30004c28:	00012be5 	.word	0x00012be5
30004c2c:	e000ed00 	.word	0xe000ed00
30004c30:	3000a149 	.word	0x3000a149
30004c34:	3000c440 	.word	0x3000c440
30004c38:	3000c454 	.word	0x3000c454

30004c3c <BOOT_CertificateCheck>:
30004c3c:	b530      	push	{r4, r5, lr}
30004c3e:	4b2b      	ldr	r3, [pc, #172]	; (30004cec <BOOT_CertificateCheck+0xb0>)
30004c40:	b0b1      	sub	sp, #196	; 0xc4
30004c42:	4604      	mov	r4, r0
30004c44:	460d      	mov	r5, r1
30004c46:	4798      	blx	r3
30004c48:	b910      	cbnz	r0, 30004c50 <BOOT_CertificateCheck+0x14>
30004c4a:	2000      	movs	r0, #0
30004c4c:	b031      	add	sp, #196	; 0xc4
30004c4e:	bd30      	pop	{r4, r5, pc}
30004c50:	69a1      	ldr	r1, [r4, #24]
30004c52:	2290      	movs	r2, #144	; 0x90
30004c54:	a80c      	add	r0, sp, #48	; 0x30
30004c56:	4429      	add	r1, r5
30004c58:	f001 f824 	bl	30005ca4 <BOOT_ImgCopy>
30004c5c:	4b24      	ldr	r3, [pc, #144]	; (30004cf0 <BOOT_CertificateCheck+0xb4>)
30004c5e:	4798      	blx	r3
30004c60:	4924      	ldr	r1, [pc, #144]	; (30004cf4 <BOOT_CertificateCheck+0xb8>)
30004c62:	f000 031f 	and.w	r3, r0, #31
30004c66:	2220      	movs	r2, #32
30004c68:	6808      	ldr	r0, [r1, #0]
30004c6a:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30004c6e:	a930      	add	r1, sp, #192	; 0xc0
30004c70:	3a01      	subs	r2, #1
30004c72:	f813 c000 	ldrb.w	ip, [r3, r0]
30004c76:	4419      	add	r1, r3
30004c78:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30004c7c:	f103 0301 	add.w	r3, r3, #1
30004c80:	f801 ccb0 	strb.w	ip, [r1, #-176]
30004c84:	f003 031f 	and.w	r3, r3, #31
30004c88:	d1f1      	bne.n	30004c6e <BOOT_CertificateCheck+0x32>
30004c8a:	4610      	mov	r0, r2
30004c8c:	4b1a      	ldr	r3, [pc, #104]	; (30004cf8 <BOOT_CertificateCheck+0xbc>)
30004c8e:	4798      	blx	r3
30004c90:	7ce3      	ldrb	r3, [r4, #19]
30004c92:	7ca2      	ldrb	r2, [r4, #18]
30004c94:	f10d 010f 	add.w	r1, sp, #15
30004c98:	f10d 000e 	add.w	r0, sp, #14
30004c9c:	f007 f898 	bl	3000bdd0 <__SBOOT_Validate_Algorithm_veneer>
30004ca0:	b940      	cbnz	r0, 30004cb4 <BOOT_CertificateCheck+0x78>
30004ca2:	f104 0520 	add.w	r5, r4, #32
30004ca6:	aa04      	add	r2, sp, #16
30004ca8:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cac:	4629      	mov	r1, r5
30004cae:	f007 f8c7 	bl	3000be40 <__SBOOT_Validate_PubKey_veneer>
30004cb2:	b130      	cbz	r0, 30004cc2 <BOOT_CertificateCheck+0x86>
30004cb4:	4b11      	ldr	r3, [pc, #68]	; (30004cfc <BOOT_CertificateCheck+0xc0>)
30004cb6:	681b      	ldr	r3, [r3, #0]
30004cb8:	039b      	lsls	r3, r3, #14
30004cba:	d411      	bmi.n	30004ce0 <BOOT_CertificateCheck+0xa4>
30004cbc:	20ff      	movs	r0, #255	; 0xff
30004cbe:	b031      	add	sp, #196	; 0xc4
30004cc0:	bd30      	pop	{r4, r5, pc}
30004cc2:	69a0      	ldr	r0, [r4, #24]
30004cc4:	4623      	mov	r3, r4
30004cc6:	ac0c      	add	r4, sp, #48	; 0x30
30004cc8:	462a      	mov	r2, r5
30004cca:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004cce:	e9cd 0400 	strd	r0, r4, [sp]
30004cd2:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cd6:	f007 f883 	bl	3000bde0 <__SBOOT_Validate_Signature_veneer>
30004cda:	2800      	cmp	r0, #0
30004cdc:	d0b5      	beq.n	30004c4a <BOOT_CertificateCheck+0xe>
30004cde:	e7e9      	b.n	30004cb4 <BOOT_CertificateCheck+0x78>
30004ce0:	4601      	mov	r1, r0
30004ce2:	4807      	ldr	r0, [pc, #28]	; (30004d00 <BOOT_CertificateCheck+0xc4>)
30004ce4:	f007 f8a4 	bl	3000be30 <__DiagPrintf_veneer>
30004ce8:	20ff      	movs	r0, #255	; 0xff
30004cea:	e7af      	b.n	30004c4c <BOOT_CertificateCheck+0x10>
30004cec:	0000c149 	.word	0x0000c149
30004cf0:	0000ea4d 	.word	0x0000ea4d
30004cf4:	3000d29c 	.word	0x3000d29c
30004cf8:	00002451 	.word	0x00002451
30004cfc:	2001c00c 	.word	0x2001c00c
30004d00:	3000c470 	.word	0x3000c470

30004d04 <BOOT_RAM_TZCfg>:
30004d04:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004d08:	b085      	sub	sp, #20
30004d0a:	4e56      	ldr	r6, [pc, #344]	; (30004e64 <BOOT_RAM_TZCfg+0x160>)
30004d0c:	4956      	ldr	r1, [pc, #344]	; (30004e68 <BOOT_RAM_TZCfg+0x164>)
30004d0e:	2501      	movs	r5, #1
30004d10:	f10d 0e08 	add.w	lr, sp, #8
30004d14:	4a55      	ldr	r2, [pc, #340]	; (30004e6c <BOOT_RAM_TZCfg+0x168>)
30004d16:	4b56      	ldr	r3, [pc, #344]	; (30004e70 <BOOT_RAM_TZCfg+0x16c>)
30004d18:	46ec      	mov	ip, sp
30004d1a:	4677      	mov	r7, lr
30004d1c:	f8df 816c 	ldr.w	r8, [pc, #364]	; 30004e8c <BOOT_RAM_TZCfg+0x188>
30004d20:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30004d24:	f8df 9168 	ldr.w	r9, [pc, #360]	; 30004e90 <BOOT_RAM_TZCfg+0x18c>
30004d28:	9202      	str	r2, [sp, #8]
30004d2a:	9303      	str	r3, [sp, #12]
30004d2c:	e9cd 6100 	strd	r6, r1, [sp]
30004d30:	f85c 2b04 	ldr.w	r2, [ip], #4
30004d34:	2300      	movs	r3, #0
30004d36:	f85e 1b04 	ldr.w	r1, [lr], #4
30004d3a:	680c      	ldr	r4, [r1, #0]
30004d3c:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30004d40:	fa05 f003 	lsl.w	r0, r5, r3
30004d44:	f1b4 3fff 	cmp.w	r4, #4294967295
30004d48:	d00c      	beq.n	30004d64 <BOOT_RAM_TZCfg+0x60>
30004d4a:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30004d4e:	3301      	adds	r3, #1
30004d50:	684c      	ldr	r4, [r1, #4]
30004d52:	310c      	adds	r1, #12
30004d54:	2b08      	cmp	r3, #8
30004d56:	f8cb 4004 	str.w	r4, [fp, #4]
30004d5a:	6c14      	ldr	r4, [r2, #64]	; 0x40
30004d5c:	ea40 0004 	orr.w	r0, r0, r4
30004d60:	6410      	str	r0, [r2, #64]	; 0x40
30004d62:	d1ea      	bne.n	30004d3a <BOOT_RAM_TZCfg+0x36>
30004d64:	42b2      	cmp	r2, r6
30004d66:	d06e      	beq.n	30004e46 <BOOT_RAM_TZCfg+0x142>
30004d68:	6455      	str	r5, [r2, #68]	; 0x44
30004d6a:	45bc      	cmp	ip, r7
30004d6c:	d1e0      	bne.n	30004d30 <BOOT_RAM_TZCfg+0x2c>
30004d6e:	4b41      	ldr	r3, [pc, #260]	; (30004e74 <BOOT_RAM_TZCfg+0x170>)
30004d70:	f46f 3400 	mvn.w	r4, #131072	; 0x20000
30004d74:	f04f 30ff 	mov.w	r0, #4294967295
30004d78:	4a3f      	ldr	r2, [pc, #252]	; (30004e78 <BOOT_RAM_TZCfg+0x174>)
30004d7a:	601c      	str	r4, [r3, #0]
30004d7c:	2100      	movs	r1, #0
30004d7e:	4c3f      	ldr	r4, [pc, #252]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004d80:	6058      	str	r0, [r3, #4]
30004d82:	6813      	ldr	r3, [r2, #0]
30004d84:	3301      	adds	r3, #1
30004d86:	d019      	beq.n	30004dbc <BOOT_RAM_TZCfg+0xb8>
30004d88:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30004d8c:	320c      	adds	r2, #12
30004d8e:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30004d92:	3101      	adds	r1, #1
30004d94:	f023 031f 	bic.w	r3, r3, #31
30004d98:	2908      	cmp	r1, #8
30004d9a:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30004d9e:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30004da2:	ea4f 0343 	mov.w	r3, r3, lsl #1
30004da6:	f020 001f 	bic.w	r0, r0, #31
30004daa:	f003 0302 	and.w	r3, r3, #2
30004dae:	ea43 0300 	orr.w	r3, r3, r0
30004db2:	f043 0301 	orr.w	r3, r3, #1
30004db6:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30004dba:	d1e2      	bne.n	30004d82 <BOOT_RAM_TZCfg+0x7e>
30004dbc:	482f      	ldr	r0, [pc, #188]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004dbe:	2201      	movs	r2, #1
30004dc0:	4b2f      	ldr	r3, [pc, #188]	; (30004e80 <BOOT_RAM_TZCfg+0x17c>)
30004dc2:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30004dc6:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004dca:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30004dce:	d11e      	bne.n	30004e0e <BOOT_RAM_TZCfg+0x10a>
30004dd0:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004dd4:	f3bf 8f4f 	dsb	sy
30004dd8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004ddc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004de0:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004de4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004de8:	0164      	lsls	r4, r4, #5
30004dea:	ea04 0106 	and.w	r1, r4, r6
30004dee:	462b      	mov	r3, r5
30004df0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004df4:	3b01      	subs	r3, #1
30004df6:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004dfa:	1c5a      	adds	r2, r3, #1
30004dfc:	d1f8      	bne.n	30004df0 <BOOT_RAM_TZCfg+0xec>
30004dfe:	3c20      	subs	r4, #32
30004e00:	f114 0f20 	cmn.w	r4, #32
30004e04:	d1f1      	bne.n	30004dea <BOOT_RAM_TZCfg+0xe6>
30004e06:	f3bf 8f4f 	dsb	sy
30004e0a:	f3bf 8f6f 	isb	sy
30004e0e:	4b1b      	ldr	r3, [pc, #108]	; (30004e7c <BOOT_RAM_TZCfg+0x178>)
30004e10:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30004e14:	491b      	ldr	r1, [pc, #108]	; (30004e84 <BOOT_RAM_TZCfg+0x180>)
30004e16:	691c      	ldr	r4, [r3, #16]
30004e18:	4a1b      	ldr	r2, [pc, #108]	; (30004e88 <BOOT_RAM_TZCfg+0x184>)
30004e1a:	f044 0408 	orr.w	r4, r4, #8
30004e1e:	611c      	str	r4, [r3, #16]
30004e20:	f04f 34ff 	mov.w	r4, #4294967295
30004e24:	68d8      	ldr	r0, [r3, #12]
30004e26:	4028      	ands	r0, r5
30004e28:	4301      	orrs	r1, r0
30004e2a:	60d9      	str	r1, [r3, #12]
30004e2c:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30004e30:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30004e34:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30004e38:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30004e3c:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30004e40:	b005      	add	sp, #20
30004e42:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004e46:	f898 3000 	ldrb.w	r3, [r8]
30004e4a:	2b00      	cmp	r3, #0
30004e4c:	d18d      	bne.n	30004d6a <BOOT_RAM_TZCfg+0x66>
30004e4e:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30004e52:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30004e56:	6c33      	ldr	r3, [r6, #64]	; 0x40
30004e58:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30004e5c:	6433      	str	r3, [r6, #64]	; 0x40
30004e5e:	6475      	str	r5, [r6, #68]	; 0x44
30004e60:	e783      	b.n	30004d6a <BOOT_RAM_TZCfg+0x66>
30004e62:	bf00      	nop
30004e64:	51001a00 	.word	0x51001a00
30004e68:	51001200 	.word	0x51001200
30004e6c:	3000d768 	.word	0x3000d768
30004e70:	3000d708 	.word	0x3000d708
30004e74:	51001b00 	.word	0x51001b00
30004e78:	3000d7c8 	.word	0x3000d7c8
30004e7c:	e000ed00 	.word	0xe000ed00
30004e80:	42008000 	.word	0x42008000
30004e84:	05fa6000 	.word	0x05fa6000
30004e88:	e000e100 	.word	0xe000e100
30004e8c:	3000d296 	.word	0x3000d296
30004e90:	005fffff 	.word	0x005fffff

30004e94 <AsciiToHex>:
30004e94:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30004e98:	2b05      	cmp	r3, #5
30004e9a:	d802      	bhi.n	30004ea2 <AsciiToHex+0xe>
30004e9c:	3837      	subs	r0, #55	; 0x37
30004e9e:	b2c0      	uxtb	r0, r0
30004ea0:	4770      	bx	lr
30004ea2:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30004ea6:	2b05      	cmp	r3, #5
30004ea8:	d802      	bhi.n	30004eb0 <AsciiToHex+0x1c>
30004eaa:	3857      	subs	r0, #87	; 0x57
30004eac:	b2c0      	uxtb	r0, r0
30004eae:	4770      	bx	lr
30004eb0:	3830      	subs	r0, #48	; 0x30
30004eb2:	b2c0      	uxtb	r0, r0
30004eb4:	280a      	cmp	r0, #10
30004eb6:	bf28      	it	cs
30004eb8:	20ff      	movcs	r0, #255	; 0xff
30004eba:	4770      	bx	lr

30004ebc <hotfix_get_addr_size>:
30004ebc:	2800      	cmp	r0, #0
30004ebe:	d054      	beq.n	30004f6a <hotfix_get_addr_size+0xae>
30004ec0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004ec4:	f04f 0a00 	mov.w	sl, #0
30004ec8:	b083      	sub	sp, #12
30004eca:	f1a3 0b04 	sub.w	fp, r3, #4
30004ece:	1c4b      	adds	r3, r1, #1
30004ed0:	4688      	mov	r8, r1
30004ed2:	4681      	mov	r9, r0
30004ed4:	9300      	str	r3, [sp, #0]
30004ed6:	4617      	mov	r7, r2
30004ed8:	1e53      	subs	r3, r2, #1
30004eda:	f101 0409 	add.w	r4, r1, #9
30004ede:	1dd5      	adds	r5, r2, #7
30004ee0:	4656      	mov	r6, sl
30004ee2:	9301      	str	r3, [sp, #4]
30004ee4:	f8c8 6004 	str.w	r6, [r8, #4]
30004ee8:	210b      	movs	r1, #11
30004eea:	f8c8 6000 	str.w	r6, [r8]
30004eee:	4640      	mov	r0, r8
30004ef0:	f8c8 6007 	str.w	r6, [r8, #7]
30004ef4:	603e      	str	r6, [r7, #0]
30004ef6:	607e      	str	r6, [r7, #4]
30004ef8:	f002 fe6a 	bl	30007bd0 <ymodem_uart_getdata>
30004efc:	e9dd 1000 	ldrd	r1, r0, [sp]
30004f00:	e004      	b.n	30004f0c <hotfix_get_addr_size+0x50>
30004f02:	b2d3      	uxtb	r3, r2
30004f04:	428c      	cmp	r4, r1
30004f06:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f0a:	d01a      	beq.n	30004f42 <hotfix_get_addr_size+0x86>
30004f0c:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30004f10:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30004f14:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30004f18:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30004f1c:	f1be 0f05 	cmp.w	lr, #5
30004f20:	b2db      	uxtb	r3, r3
30004f22:	d9ef      	bls.n	30004f04 <hotfix_get_addr_size+0x48>
30004f24:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30004f28:	f1bc 0f05 	cmp.w	ip, #5
30004f2c:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30004f30:	b2db      	uxtb	r3, r3
30004f32:	d9e6      	bls.n	30004f02 <hotfix_get_addr_size+0x46>
30004f34:	2b0a      	cmp	r3, #10
30004f36:	bf28      	it	cs
30004f38:	23ff      	movcs	r3, #255	; 0xff
30004f3a:	428c      	cmp	r4, r1
30004f3c:	f800 3f01 	strb.w	r3, [r0, #1]!
30004f40:	d1e4      	bne.n	30004f0c <hotfix_get_addr_size+0x50>
30004f42:	4639      	mov	r1, r7
30004f44:	2300      	movs	r3, #0
30004f46:	f811 2b01 	ldrb.w	r2, [r1], #1
30004f4a:	4313      	orrs	r3, r2
30004f4c:	42a9      	cmp	r1, r5
30004f4e:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004f52:	d1f8      	bne.n	30004f46 <hotfix_get_addr_size+0x8a>
30004f54:	79fa      	ldrb	r2, [r7, #7]
30004f56:	f10a 0a01 	add.w	sl, sl, #1
30004f5a:	4313      	orrs	r3, r2
30004f5c:	45ca      	cmp	sl, r9
30004f5e:	f84b 3f04 	str.w	r3, [fp, #4]!
30004f62:	d1bf      	bne.n	30004ee4 <hotfix_get_addr_size+0x28>
30004f64:	b003      	add	sp, #12
30004f66:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f6a:	4770      	bx	lr

30004f6c <ascii_to_integer>:
30004f6c:	2900      	cmp	r1, #0
30004f6e:	4684      	mov	ip, r0
30004f70:	dd12      	ble.n	30004f98 <ascii_to_integer+0x2c>
30004f72:	3901      	subs	r1, #1
30004f74:	1e42      	subs	r2, r0, #1
30004f76:	2000      	movs	r0, #0
30004f78:	448c      	add	ip, r1
30004f7a:	e001      	b.n	30004f80 <ascii_to_integer+0x14>
30004f7c:	4562      	cmp	r2, ip
30004f7e:	d00d      	beq.n	30004f9c <ascii_to_integer+0x30>
30004f80:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30004f84:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30004f88:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30004f8c:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30004f90:	2909      	cmp	r1, #9
30004f92:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30004f96:	d9f1      	bls.n	30004f7c <ascii_to_integer+0x10>
30004f98:	2000      	movs	r0, #0
30004f9a:	4770      	bx	lr
30004f9c:	4770      	bx	lr
30004f9e:	bf00      	nop

30004fa0 <eight_4bit_to_int>:
30004fa0:	2300      	movs	r3, #0
30004fa2:	f100 0c07 	add.w	ip, r0, #7
30004fa6:	4602      	mov	r2, r0
30004fa8:	f812 1b01 	ldrb.w	r1, [r2], #1
30004fac:	430b      	orrs	r3, r1
30004fae:	4562      	cmp	r2, ip
30004fb0:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004fb4:	d1f8      	bne.n	30004fa8 <eight_4bit_to_int+0x8>
30004fb6:	79c0      	ldrb	r0, [r0, #7]
30004fb8:	4318      	orrs	r0, r3
30004fba:	4770      	bx	lr

30004fbc <FLASH_Erase_With_Lock>:
30004fbc:	4b11      	ldr	r3, [pc, #68]	; (30005004 <FLASH_Erase_With_Lock+0x48>)
30004fbe:	b510      	push	{r4, lr}
30004fc0:	460c      	mov	r4, r1
30004fc2:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30004fc6:	4798      	blx	r3
30004fc8:	4b0f      	ldr	r3, [pc, #60]	; (30005008 <FLASH_Erase_With_Lock+0x4c>)
30004fca:	695b      	ldr	r3, [r3, #20]
30004fcc:	03db      	lsls	r3, r3, #15
30004fce:	d512      	bpl.n	30004ff6 <FLASH_Erase_With_Lock+0x3a>
30004fd0:	f014 021f 	ands.w	r2, r4, #31
30004fd4:	d110      	bne.n	30004ff8 <FLASH_Erase_With_Lock+0x3c>
30004fd6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30004fda:	f3bf 8f4f 	dsb	sy
30004fde:	480a      	ldr	r0, [pc, #40]	; (30005008 <FLASH_Erase_With_Lock+0x4c>)
30004fe0:	4422      	add	r2, r4
30004fe2:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30004fe6:	3420      	adds	r4, #32
30004fe8:	1b13      	subs	r3, r2, r4
30004fea:	2b00      	cmp	r3, #0
30004fec:	dcf9      	bgt.n	30004fe2 <FLASH_Erase_With_Lock+0x26>
30004fee:	f3bf 8f4f 	dsb	sy
30004ff2:	f3bf 8f6f 	isb	sy
30004ff6:	bd10      	pop	{r4, pc}
30004ff8:	f024 041f 	bic.w	r4, r4, #31
30004ffc:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30005000:	e7eb      	b.n	30004fda <FLASH_Erase_With_Lock+0x1e>
30005002:	bf00      	nop
30005004:	0000a16d 	.word	0x0000a16d
30005008:	e000ed00 	.word	0xe000ed00

3000500c <FLASH_TxData_With_Lock>:
3000500c:	b538      	push	{r3, r4, r5, lr}
3000500e:	4b13      	ldr	r3, [pc, #76]	; (3000505c <FLASH_TxData_With_Lock+0x50>)
30005010:	4604      	mov	r4, r0
30005012:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30005016:	460d      	mov	r5, r1
30005018:	4798      	blx	r3
3000501a:	4b11      	ldr	r3, [pc, #68]	; (30005060 <FLASH_TxData_With_Lock+0x54>)
3000501c:	695b      	ldr	r3, [r3, #20]
3000501e:	03db      	lsls	r3, r3, #15
30005020:	d517      	bpl.n	30005052 <FLASH_TxData_With_Lock+0x46>
30005022:	f014 031f 	ands.w	r3, r4, #31
30005026:	d115      	bne.n	30005054 <FLASH_TxData_With_Lock+0x48>
30005028:	f3bf 8f4f 	dsb	sy
3000502c:	b16d      	cbz	r5, 3000504a <FLASH_TxData_With_Lock+0x3e>
3000502e:	1e6a      	subs	r2, r5, #1
30005030:	f104 0320 	add.w	r3, r4, #32
30005034:	490a      	ldr	r1, [pc, #40]	; (30005060 <FLASH_TxData_With_Lock+0x54>)
30005036:	f022 021f 	bic.w	r2, r2, #31
3000503a:	441a      	add	r2, r3
3000503c:	4293      	cmp	r3, r2
3000503e:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30005042:	461c      	mov	r4, r3
30005044:	f103 0320 	add.w	r3, r3, #32
30005048:	d1f8      	bne.n	3000503c <FLASH_TxData_With_Lock+0x30>
3000504a:	f3bf 8f4f 	dsb	sy
3000504e:	f3bf 8f6f 	isb	sy
30005052:	bd38      	pop	{r3, r4, r5, pc}
30005054:	f024 041f 	bic.w	r4, r4, #31
30005058:	441d      	add	r5, r3
3000505a:	e7e5      	b.n	30005028 <FLASH_TxData_With_Lock+0x1c>
3000505c:	0000a04d 	.word	0x0000a04d
30005060:	e000ed00 	.word	0xe000ed00

30005064 <flash_read_id>:
30005064:	b538      	push	{r3, r4, r5, lr}
30005066:	4d04      	ldr	r5, [pc, #16]	; (30005078 <flash_read_id+0x14>)
30005068:	460c      	mov	r4, r1
3000506a:	4602      	mov	r2, r0
3000506c:	4b03      	ldr	r3, [pc, #12]	; (3000507c <flash_read_id+0x18>)
3000506e:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30005072:	4798      	blx	r3
30005074:	4620      	mov	r0, r4
30005076:	bd38      	pop	{r3, r4, r5, pc}
30005078:	2001c01c 	.word	0x2001c01c
3000507c:	00009db5 	.word	0x00009db5

30005080 <mcc_ReverseBit>:
30005080:	0843      	lsrs	r3, r0, #1
30005082:	08c1      	lsrs	r1, r0, #3
30005084:	f003 0308 	and.w	r3, r3, #8
30005088:	0942      	lsrs	r2, r0, #5
3000508a:	f001 0104 	and.w	r1, r1, #4
3000508e:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30005092:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30005096:	f002 0202 	and.w	r2, r2, #2
3000509a:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
3000509e:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
300050a2:	430b      	orrs	r3, r1
300050a4:	00c1      	lsls	r1, r0, #3
300050a6:	4313      	orrs	r3, r2
300050a8:	f001 0120 	and.w	r1, r1, #32
300050ac:	0042      	lsls	r2, r0, #1
300050ae:	ea43 000c 	orr.w	r0, r3, ip
300050b2:	f002 0310 	and.w	r3, r2, #16
300050b6:	4308      	orrs	r0, r1
300050b8:	4318      	orrs	r0, r3
300050ba:	b2c0      	uxtb	r0, r0
300050bc:	4770      	bx	lr
300050be:	bf00      	nop

300050c0 <mcc_init_crc32>:
300050c0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
300050c2:	2600      	movs	r6, #0
300050c4:	4f16      	ldr	r7, [pc, #88]	; (30005120 <mcc_init_crc32+0x60>)
300050c6:	4605      	mov	r5, r0
300050c8:	b2f0      	uxtb	r0, r6
300050ca:	f7ff ffd9 	bl	30005080 <mcc_ReverseBit>
300050ce:	2208      	movs	r2, #8
300050d0:	0604      	lsls	r4, r0, #24
300050d2:	2c00      	cmp	r4, #0
300050d4:	ea4f 0444 	mov.w	r4, r4, lsl #1
300050d8:	bfb8      	it	lt
300050da:	407c      	eorlt	r4, r7
300050dc:	3a01      	subs	r2, #1
300050de:	d1f8      	bne.n	300050d2 <mcc_init_crc32+0x12>
300050e0:	0e20      	lsrs	r0, r4, #24
300050e2:	3504      	adds	r5, #4
300050e4:	f7ff ffcc 	bl	30005080 <mcc_ReverseBit>
300050e8:	4603      	mov	r3, r0
300050ea:	f3c4 4007 	ubfx	r0, r4, #16, #8
300050ee:	3601      	adds	r6, #1
300050f0:	f805 3c04 	strb.w	r3, [r5, #-4]
300050f4:	f7ff ffc4 	bl	30005080 <mcc_ReverseBit>
300050f8:	4603      	mov	r3, r0
300050fa:	f3c4 2007 	ubfx	r0, r4, #8, #8
300050fe:	f805 3c03 	strb.w	r3, [r5, #-3]
30005102:	f7ff ffbd 	bl	30005080 <mcc_ReverseBit>
30005106:	4603      	mov	r3, r0
30005108:	b2e0      	uxtb	r0, r4
3000510a:	f805 3c02 	strb.w	r3, [r5, #-2]
3000510e:	f7ff ffb7 	bl	30005080 <mcc_ReverseBit>
30005112:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30005116:	f805 0c01 	strb.w	r0, [r5, #-1]
3000511a:	d1d5      	bne.n	300050c8 <mcc_init_crc32+0x8>
3000511c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
3000511e:	bf00      	nop
30005120:	04c11db7 	.word	0x04c11db7

30005124 <mcc_crc32>:
30005124:	b179      	cbz	r1, 30005146 <mcc_crc32+0x22>
30005126:	eb00 0c01 	add.w	ip, r0, r1
3000512a:	f04f 31ff 	mov.w	r1, #4294967295
3000512e:	f810 3b01 	ldrb.w	r3, [r0], #1
30005132:	404b      	eors	r3, r1
30005134:	4560      	cmp	r0, ip
30005136:	b2db      	uxtb	r3, r3
30005138:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
3000513c:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30005140:	d1f5      	bne.n	3000512e <mcc_crc32+0xa>
30005142:	43c8      	mvns	r0, r1
30005144:	4770      	bx	lr
30005146:	4608      	mov	r0, r1
30005148:	4770      	bx	lr
3000514a:	bf00      	nop

3000514c <recovery_check>:
3000514c:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30005150:	4283      	cmp	r3, r0
30005152:	d322      	bcc.n	3000519a <recovery_check+0x4e>
30005154:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30005158:	d91f      	bls.n	3000519a <recovery_check+0x4e>
3000515a:	b510      	push	{r4, lr}
3000515c:	f8d0 400a 	ldr.w	r4, [r0, #10]
30005160:	f104 030c 	add.w	r3, r4, #12
30005164:	4299      	cmp	r1, r3
30005166:	d316      	bcc.n	30005196 <recovery_check+0x4a>
30005168:	f100 0c04 	add.w	ip, r0, #4
3000516c:	b183      	cbz	r3, 30005190 <recovery_check+0x44>
3000516e:	f100 0e10 	add.w	lr, r0, #16
30005172:	f04f 31ff 	mov.w	r1, #4294967295
30005176:	44a6      	add	lr, r4
30005178:	4c0b      	ldr	r4, [pc, #44]	; (300051a8 <recovery_check+0x5c>)
3000517a:	f81c 3b01 	ldrb.w	r3, [ip], #1
3000517e:	404b      	eors	r3, r1
30005180:	45f4      	cmp	ip, lr
30005182:	b2db      	uxtb	r3, r3
30005184:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005188:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
3000518c:	d1f5      	bne.n	3000517a <recovery_check+0x2e>
3000518e:	43cb      	mvns	r3, r1
30005190:	6801      	ldr	r1, [r0, #0]
30005192:	4299      	cmp	r1, r3
30005194:	d003      	beq.n	3000519e <recovery_check+0x52>
30005196:	2000      	movs	r0, #0
30005198:	bd10      	pop	{r4, pc}
3000519a:	2000      	movs	r0, #0
3000519c:	4770      	bx	lr
3000519e:	f8d0 3006 	ldr.w	r3, [r0, #6]
300051a2:	2001      	movs	r0, #1
300051a4:	6013      	str	r3, [r2, #0]
300051a6:	bd10      	pop	{r4, pc}
300051a8:	3000d828 	.word	0x3000d828

300051ac <rewrite_bp>:
300051ac:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300051b0:	b0a3      	sub	sp, #140	; 0x8c
300051b2:	4616      	mov	r6, r2
300051b4:	4607      	mov	r7, r0
300051b6:	460d      	mov	r5, r1
300051b8:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
300051bc:	4699      	mov	r9, r3
300051be:	f10d 0808 	add.w	r8, sp, #8
300051c2:	2280      	movs	r2, #128	; 0x80
300051c4:	2c00      	cmp	r4, #0
300051c6:	f000 80b2 	beq.w	3000532e <rewrite_bp+0x182>
300051ca:	4619      	mov	r1, r3
300051cc:	4640      	mov	r0, r8
300051ce:	f006 fe17 	bl	3000be00 <____wrap_memcpy_veneer>
300051d2:	2301      	movs	r3, #1
300051d4:	f109 0404 	add.w	r4, r9, #4
300051d8:	4649      	mov	r1, r9
300051da:	2002      	movs	r0, #2
300051dc:	f88d 7014 	strb.w	r7, [sp, #20]
300051e0:	e9cd 3303 	strd	r3, r3, [sp, #12]
300051e4:	f7ff feea 	bl	30004fbc <FLASH_Erase_With_Lock>
300051e8:	4620      	mov	r0, r4
300051ea:	217c      	movs	r1, #124	; 0x7c
300051ec:	aa03      	add	r2, sp, #12
300051ee:	f7ff ff0d 	bl	3000500c <FLASH_TxData_With_Lock>
300051f2:	4620      	mov	r0, r4
300051f4:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
300051f8:	f04f 33ff 	mov.w	r3, #4294967295
300051fc:	4c5c      	ldr	r4, [pc, #368]	; (30005370 <rewrite_bp+0x1c4>)
300051fe:	f810 cb01 	ldrb.w	ip, [r0], #1
30005202:	ea8c 0c03 	eor.w	ip, ip, r3
30005206:	4288      	cmp	r0, r1
30005208:	fa5f fc8c 	uxtb.w	ip, ip
3000520c:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30005210:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005214:	d1f3      	bne.n	300051fe <rewrite_bp+0x52>
30005216:	43db      	mvns	r3, r3
30005218:	4649      	mov	r1, r9
3000521a:	2002      	movs	r0, #2
3000521c:	9302      	str	r3, [sp, #8]
3000521e:	f7ff fecd 	bl	30004fbc <FLASH_Erase_With_Lock>
30005222:	4642      	mov	r2, r8
30005224:	4648      	mov	r0, r9
30005226:	2180      	movs	r1, #128	; 0x80
30005228:	f7ff fef0 	bl	3000500c <FLASH_TxData_With_Lock>
3000522c:	4b51      	ldr	r3, [pc, #324]	; (30005374 <rewrite_bp+0x1c8>)
3000522e:	aa01      	add	r2, sp, #4
30005230:	2104      	movs	r1, #4
30005232:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005236:	4b50      	ldr	r3, [pc, #320]	; (30005378 <rewrite_bp+0x1cc>)
30005238:	4798      	blx	r3
3000523a:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000523e:	f04f 0c01 	mov.w	ip, #1
30005242:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30005246:	682a      	ldr	r2, [r5, #0]
30005248:	fa0c fc03 	lsl.w	ip, ip, r3
3000524c:	f10e 0e0c 	add.w	lr, lr, #12
30005250:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005254:	45e6      	cmp	lr, ip
30005256:	6831      	ldr	r1, [r6, #0]
30005258:	f103 030c 	add.w	r3, r3, #12
3000525c:	bf28      	it	cs
3000525e:	46e6      	movcs	lr, ip
30005260:	459c      	cmp	ip, r3
30005262:	bf28      	it	cs
30005264:	469c      	movcs	ip, r3
30005266:	bb9f      	cbnz	r7, 300052d0 <rewrite_bp+0x124>
30005268:	1d28      	adds	r0, r5, #4
3000526a:	f1be 0f00 	cmp.w	lr, #0
3000526e:	d00e      	beq.n	3000528e <rewrite_bp+0xe2>
30005270:	4486      	add	lr, r0
30005272:	f04f 37ff 	mov.w	r7, #4294967295
30005276:	f810 3b01 	ldrb.w	r3, [r0], #1
3000527a:	407b      	eors	r3, r7
3000527c:	4570      	cmp	r0, lr
3000527e:	b2db      	uxtb	r3, r3
30005280:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005284:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005288:	d1f5      	bne.n	30005276 <rewrite_bp+0xca>
3000528a:	ea6f 0e07 	mvn.w	lr, r7
3000528e:	4572      	cmp	r2, lr
30005290:	d052      	beq.n	30005338 <rewrite_bp+0x18c>
30005292:	1d32      	adds	r2, r6, #4
30005294:	f1bc 0f00 	cmp.w	ip, #0
30005298:	d00e      	beq.n	300052b8 <rewrite_bp+0x10c>
3000529a:	4494      	add	ip, r2
3000529c:	f04f 37ff 	mov.w	r7, #4294967295
300052a0:	f812 3b01 	ldrb.w	r3, [r2], #1
300052a4:	407b      	eors	r3, r7
300052a6:	4562      	cmp	r2, ip
300052a8:	b2db      	uxtb	r3, r3
300052aa:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052ae:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052b2:	d1f5      	bne.n	300052a0 <rewrite_bp+0xf4>
300052b4:	ea6f 0c07 	mvn.w	ip, r7
300052b8:	4561      	cmp	r1, ip
300052ba:	d133      	bne.n	30005324 <rewrite_bp+0x178>
300052bc:	4b2f      	ldr	r3, [pc, #188]	; (3000537c <rewrite_bp+0x1d0>)
300052be:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
300052c2:	689b      	ldr	r3, [r3, #8]
300052c4:	0799      	lsls	r1, r3, #30
300052c6:	d44c      	bmi.n	30005362 <rewrite_bp+0x1b6>
300052c8:	2002      	movs	r0, #2
300052ca:	b023      	add	sp, #140	; 0x8c
300052cc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300052d0:	1d30      	adds	r0, r6, #4
300052d2:	f1bc 0f00 	cmp.w	ip, #0
300052d6:	d00e      	beq.n	300052f6 <rewrite_bp+0x14a>
300052d8:	4484      	add	ip, r0
300052da:	f04f 37ff 	mov.w	r7, #4294967295
300052de:	f810 3b01 	ldrb.w	r3, [r0], #1
300052e2:	407b      	eors	r3, r7
300052e4:	4560      	cmp	r0, ip
300052e6:	b2db      	uxtb	r3, r3
300052e8:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300052ec:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300052f0:	d1f5      	bne.n	300052de <rewrite_bp+0x132>
300052f2:	ea6f 0c07 	mvn.w	ip, r7
300052f6:	4561      	cmp	r1, ip
300052f8:	d0e0      	beq.n	300052bc <rewrite_bp+0x110>
300052fa:	1d29      	adds	r1, r5, #4
300052fc:	f1be 0f00 	cmp.w	lr, #0
30005300:	d00e      	beq.n	30005320 <rewrite_bp+0x174>
30005302:	448e      	add	lr, r1
30005304:	f04f 37ff 	mov.w	r7, #4294967295
30005308:	f811 3b01 	ldrb.w	r3, [r1], #1
3000530c:	407b      	eors	r3, r7
3000530e:	4571      	cmp	r1, lr
30005310:	b2db      	uxtb	r3, r3
30005312:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005316:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000531a:	d1f5      	bne.n	30005308 <rewrite_bp+0x15c>
3000531c:	ea6f 0e07 	mvn.w	lr, r7
30005320:	4572      	cmp	r2, lr
30005322:	d009      	beq.n	30005338 <rewrite_bp+0x18c>
30005324:	4b15      	ldr	r3, [pc, #84]	; (3000537c <rewrite_bp+0x1d0>)
30005326:	689b      	ldr	r3, [r3, #8]
30005328:	079b      	lsls	r3, r3, #30
3000532a:	d40f      	bmi.n	3000534c <rewrite_bp+0x1a0>
3000532c:	e7fe      	b.n	3000532c <rewrite_bp+0x180>
3000532e:	992a      	ldr	r1, [sp, #168]	; 0xa8
30005330:	4640      	mov	r0, r8
30005332:	f006 fd65 	bl	3000be00 <____wrap_memcpy_veneer>
30005336:	e74c      	b.n	300051d2 <rewrite_bp+0x26>
30005338:	4b10      	ldr	r3, [pc, #64]	; (3000537c <rewrite_bp+0x1d0>)
3000533a:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
3000533e:	689b      	ldr	r3, [r3, #8]
30005340:	079a      	lsls	r2, r3, #30
30005342:	d407      	bmi.n	30005354 <rewrite_bp+0x1a8>
30005344:	2001      	movs	r0, #1
30005346:	b023      	add	sp, #140	; 0x8c
30005348:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000534c:	480c      	ldr	r0, [pc, #48]	; (30005380 <rewrite_bp+0x1d4>)
3000534e:	f006 fd6f 	bl	3000be30 <__DiagPrintf_veneer>
30005352:	e7eb      	b.n	3000532c <rewrite_bp+0x180>
30005354:	f8d5 1006 	ldr.w	r1, [r5, #6]
30005358:	480a      	ldr	r0, [pc, #40]	; (30005384 <rewrite_bp+0x1d8>)
3000535a:	f006 fd69 	bl	3000be30 <__DiagPrintf_veneer>
3000535e:	2001      	movs	r0, #1
30005360:	e7b3      	b.n	300052ca <rewrite_bp+0x11e>
30005362:	f8d6 1006 	ldr.w	r1, [r6, #6]
30005366:	4808      	ldr	r0, [pc, #32]	; (30005388 <rewrite_bp+0x1dc>)
30005368:	f006 fd62 	bl	3000be30 <__DiagPrintf_veneer>
3000536c:	e7ac      	b.n	300052c8 <rewrite_bp+0x11c>
3000536e:	bf00      	nop
30005370:	3000d828 	.word	0x3000d828
30005374:	2001c01c 	.word	0x2001c01c
30005378:	00009db5 	.word	0x00009db5
3000537c:	2001c00c 	.word	0x2001c00c
30005380:	3000c4f4 	.word	0x3000c4f4
30005384:	3000c494 	.word	0x3000c494
30005388:	3000c4c4 	.word	0x3000c4c4

3000538c <BOOT_SectionInit>:
3000538c:	4800      	ldr	r0, [pc, #0]	; (30005390 <BOOT_SectionInit+0x4>)
3000538e:	4770      	bx	lr
30005390:	60000020 	.word	0x60000020

30005394 <BOOT_NsStart>:
30005394:	b570      	push	{r4, r5, r6, lr}
30005396:	4c23      	ldr	r4, [pc, #140]	; (30005424 <BOOT_NsStart+0x90>)
30005398:	4605      	mov	r5, r0
3000539a:	f384 8808 	msr	MSP, r4
3000539e:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300053a2:	2100      	movs	r1, #0
300053a4:	4620      	mov	r0, r4
300053a6:	4e20      	ldr	r6, [pc, #128]	; (30005428 <BOOT_NsStart+0x94>)
300053a8:	47b0      	blx	r6
300053aa:	4a20      	ldr	r2, [pc, #128]	; (3000542c <BOOT_NsStart+0x98>)
300053ac:	6953      	ldr	r3, [r2, #20]
300053ae:	03d9      	lsls	r1, r3, #15
300053b0:	d50b      	bpl.n	300053ca <BOOT_NsStart+0x36>
300053b2:	f3bf 8f4f 	dsb	sy
300053b6:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300053ba:	3420      	adds	r4, #32
300053bc:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300053c0:	d1f9      	bne.n	300053b6 <BOOT_NsStart+0x22>
300053c2:	f3bf 8f4f 	dsb	sy
300053c6:	f3bf 8f6f 	isb	sy
300053ca:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300053ce:	f383 8808 	msr	MSP, r3
300053d2:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300053d6:	2100      	movs	r1, #0
300053d8:	4815      	ldr	r0, [pc, #84]	; (30005430 <BOOT_NsStart+0x9c>)
300053da:	47b0      	blx	r6
300053dc:	4a13      	ldr	r2, [pc, #76]	; (3000542c <BOOT_NsStart+0x98>)
300053de:	6953      	ldr	r3, [r2, #20]
300053e0:	03db      	lsls	r3, r3, #15
300053e2:	d50c      	bpl.n	300053fe <BOOT_NsStart+0x6a>
300053e4:	f3bf 8f4f 	dsb	sy
300053e8:	4b11      	ldr	r3, [pc, #68]	; (30005430 <BOOT_NsStart+0x9c>)
300053ea:	490e      	ldr	r1, [pc, #56]	; (30005424 <BOOT_NsStart+0x90>)
300053ec:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
300053f0:	3320      	adds	r3, #32
300053f2:	428b      	cmp	r3, r1
300053f4:	d1fa      	bne.n	300053ec <BOOT_NsStart+0x58>
300053f6:	f3bf 8f4f 	dsb	sy
300053fa:	f3bf 8f6f 	isb	sy
300053fe:	f025 0501 	bic.w	r5, r5, #1
30005402:	086d      	lsrs	r5, r5, #1
30005404:	006d      	lsls	r5, r5, #1
30005406:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
3000540a:	ed2d 8b10 	vpush	{d8-d15}
3000540e:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
30005412:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30005416:	47ac      	blxns	r5
30005418:	ecbd 8b10 	vpop	{d8-d15}
3000541c:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005420:	bd70      	pop	{r4, r5, r6, pc}
30005422:	bf00      	nop
30005424:	30002000 	.word	0x30002000
30005428:	00012be5 	.word	0x00012be5
3000542c:	e000ed00 	.word	0xe000ed00
30005430:	30001000 	.word	0x30001000

30005434 <BOOT_RccConfig>:
30005434:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005438:	4c2f      	ldr	r4, [pc, #188]	; (300054f8 <BOOT_RccConfig+0xc4>)
3000543a:	b090      	sub	sp, #64	; 0x40
3000543c:	4f2f      	ldr	r7, [pc, #188]	; (300054fc <BOOT_RccConfig+0xc8>)
3000543e:	466d      	mov	r5, sp
30005440:	f10d 0c20 	add.w	ip, sp, #32
30005444:	683e      	ldr	r6, [r7, #0]
30005446:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
3000544a:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000544e:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
30005452:	f104 0310 	add.w	r3, r4, #16
30005456:	2400      	movs	r4, #0
30005458:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000545a:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000545e:	1c72      	adds	r2, r6, #1
30005460:	e9cd 4404 	strd	r4, r4, [sp, #16]
30005464:	e9cd 4406 	strd	r4, r4, [sp, #24]
30005468:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000546c:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30005470:	d03f      	beq.n	300054f2 <BOOT_RccConfig+0xbe>
30005472:	463a      	mov	r2, r7
30005474:	6891      	ldr	r1, [r2, #8]
30005476:	0fb3      	lsrs	r3, r6, #30
30005478:	a810      	add	r0, sp, #64	; 0x40
3000547a:	2901      	cmp	r1, #1
3000547c:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30005480:	d10e      	bne.n	300054a0 <BOOT_RccConfig+0x6c>
30005482:	f853 0c30 	ldr.w	r0, [r3, #-48]
30005486:	6854      	ldr	r4, [r2, #4]
30005488:	4330      	orrs	r0, r6
3000548a:	ae10      	add	r6, sp, #64	; 0x40
3000548c:	0fa1      	lsrs	r1, r4, #30
3000548e:	f843 0c30 	str.w	r0, [r3, #-48]
30005492:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30005496:	f851 3c10 	ldr.w	r3, [r1, #-16]
3000549a:	4323      	orrs	r3, r4
3000549c:	f841 3c10 	str.w	r3, [r1, #-16]
300054a0:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300054a4:	1c73      	adds	r3, r6, #1
300054a6:	d1e5      	bne.n	30005474 <BOOT_RccConfig+0x40>
300054a8:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300054aa:	9c04      	ldr	r4, [sp, #16]
300054ac:	af0d      	add	r7, sp, #52	; 0x34
300054ae:	ae05      	add	r6, sp, #20
300054b0:	4a13      	ldr	r2, [pc, #76]	; (30005500 <BOOT_RccConfig+0xcc>)
300054b2:	f85c 0b04 	ldr.w	r0, [ip], #4
300054b6:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300054ba:	f855 1b04 	ldr.w	r1, [r5], #4
300054be:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300054c2:	f850 8002 	ldr.w	r8, [r0, r2]
300054c6:	45e6      	cmp	lr, ip
300054c8:	ea43 0308 	orr.w	r3, r3, r8
300054cc:	5083      	str	r3, [r0, r2]
300054ce:	588b      	ldr	r3, [r1, r2]
300054d0:	ea44 0403 	orr.w	r4, r4, r3
300054d4:	508c      	str	r4, [r1, r2]
300054d6:	d004      	beq.n	300054e2 <BOOT_RccConfig+0xae>
300054d8:	f857 3b04 	ldr.w	r3, [r7], #4
300054dc:	f856 4b04 	ldr.w	r4, [r6], #4
300054e0:	e7e7      	b.n	300054b2 <BOOT_RccConfig+0x7e>
300054e2:	200a      	movs	r0, #10
300054e4:	4b07      	ldr	r3, [pc, #28]	; (30005504 <BOOT_RccConfig+0xd0>)
300054e6:	4798      	blx	r3
300054e8:	4b07      	ldr	r3, [pc, #28]	; (30005508 <BOOT_RccConfig+0xd4>)
300054ea:	b010      	add	sp, #64	; 0x40
300054ec:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300054f0:	4718      	bx	r3
300054f2:	4623      	mov	r3, r4
300054f4:	e7da      	b.n	300054ac <BOOT_RccConfig+0x78>
300054f6:	bf00      	nop
300054f8:	3000d22c 	.word	0x3000d22c
300054fc:	3000d380 	.word	0x3000d380
30005500:	42008000 	.word	0x42008000
30005504:	00009b2d 	.word	0x00009b2d
30005508:	30007ed9 	.word	0x30007ed9

3000550c <BOOT_CACHEWRR_Set>:
3000550c:	4b14      	ldr	r3, [pc, #80]	; (30005560 <BOOT_CACHEWRR_Set+0x54>)
3000550e:	b430      	push	{r4, r5}
30005510:	681a      	ldr	r2, [r3, #0]
30005512:	2a01      	cmp	r2, #1
30005514:	d10f      	bne.n	30005536 <BOOT_CACHEWRR_Set+0x2a>
30005516:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
3000551a:	685d      	ldr	r5, [r3, #4]
3000551c:	689c      	ldr	r4, [r3, #8]
3000551e:	6882      	ldr	r2, [r0, #8]
30005520:	4910      	ldr	r1, [pc, #64]	; (30005564 <BOOT_CACHEWRR_Set+0x58>)
30005522:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005526:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
3000552a:	6082      	str	r2, [r0, #8]
3000552c:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30005530:	68da      	ldr	r2, [r3, #12]
30005532:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30005536:	691a      	ldr	r2, [r3, #16]
30005538:	2a01      	cmp	r2, #1
3000553a:	d10f      	bne.n	3000555c <BOOT_CACHEWRR_Set+0x50>
3000553c:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005540:	695d      	ldr	r5, [r3, #20]
30005542:	4a08      	ldr	r2, [pc, #32]	; (30005564 <BOOT_CACHEWRR_Set+0x58>)
30005544:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005548:	688b      	ldr	r3, [r1, #8]
3000554a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000554e:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
30005552:	608b      	str	r3, [r1, #8]
30005554:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30005558:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
3000555c:	bc30      	pop	{r4, r5}
3000555e:	4770      	bx	lr
30005560:	3000de8c 	.word	0x3000de8c
30005564:	e0042000 	.word	0xe0042000

30005568 <BOOT_TCMSet>:
30005568:	2300      	movs	r3, #0
3000556a:	b5f0      	push	{r4, r5, r6, r7, lr}
3000556c:	4db3      	ldr	r5, [pc, #716]	; (3000583c <BOOT_TCMSet+0x2d4>)
3000556e:	461c      	mov	r4, r3
30005570:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30005574:	3307      	adds	r3, #7
30005576:	4282      	cmp	r2, r0
30005578:	d03c      	beq.n	300055f4 <BOOT_TCMSet+0x8c>
3000557a:	3401      	adds	r4, #1
3000557c:	2c07      	cmp	r4, #7
3000557e:	d1f7      	bne.n	30005570 <BOOT_TCMSet+0x8>
30005580:	f3bf 8f4f 	dsb	sy
30005584:	f3bf 8f6f 	isb	sy
30005588:	48ad      	ldr	r0, [pc, #692]	; (30005840 <BOOT_TCMSet+0x2d8>)
3000558a:	2200      	movs	r2, #0
3000558c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005590:	f3bf 8f4f 	dsb	sy
30005594:	f3bf 8f6f 	isb	sy
30005598:	6943      	ldr	r3, [r0, #20]
3000559a:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000559e:	6143      	str	r3, [r0, #20]
300055a0:	f3bf 8f4f 	dsb	sy
300055a4:	f3bf 8f6f 	isb	sy
300055a8:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300055ac:	f3bf 8f4f 	dsb	sy
300055b0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300055b4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300055b8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300055bc:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300055c0:	0164      	lsls	r4, r4, #5
300055c2:	ea04 0106 	and.w	r1, r4, r6
300055c6:	462b      	mov	r3, r5
300055c8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300055cc:	3b01      	subs	r3, #1
300055ce:	1c5f      	adds	r7, r3, #1
300055d0:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300055d4:	d1f8      	bne.n	300055c8 <BOOT_TCMSet+0x60>
300055d6:	3c20      	subs	r4, #32
300055d8:	f114 0f20 	cmn.w	r4, #32
300055dc:	d1f1      	bne.n	300055c2 <BOOT_TCMSet+0x5a>
300055de:	f3bf 8f4f 	dsb	sy
300055e2:	6943      	ldr	r3, [r0, #20]
300055e4:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300055e8:	6143      	str	r3, [r0, #20]
300055ea:	f3bf 8f4f 	dsb	sy
300055ee:	f3bf 8f6f 	isb	sy
300055f2:	bdf0      	pop	{r4, r5, r6, r7, pc}
300055f4:	2c00      	cmp	r4, #0
300055f6:	d0c3      	beq.n	30005580 <BOOT_TCMSet+0x18>
300055f8:	f3bf 8f4f 	dsb	sy
300055fc:	f3bf 8f6f 	isb	sy
30005600:	4890      	ldr	r0, [pc, #576]	; (30005844 <BOOT_TCMSet+0x2dc>)
30005602:	2300      	movs	r3, #0
30005604:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30005608:	f3bf 8f4f 	dsb	sy
3000560c:	f3bf 8f6f 	isb	sy
30005610:	6942      	ldr	r2, [r0, #20]
30005612:	03d2      	lsls	r2, r2, #15
30005614:	d520      	bpl.n	30005658 <BOOT_TCMSet+0xf0>
30005616:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000561a:	f3bf 8f4f 	dsb	sy
3000561e:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30005622:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005626:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
3000562a:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000562e:	ea4f 1c43 	mov.w	ip, r3, lsl #5
30005632:	ea0c 0106 	and.w	r1, ip, r6
30005636:	4673      	mov	r3, lr
30005638:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000563c:	3b01      	subs	r3, #1
3000563e:	1c5f      	adds	r7, r3, #1
30005640:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30005644:	d1f8      	bne.n	30005638 <BOOT_TCMSet+0xd0>
30005646:	f1ac 0c20 	sub.w	ip, ip, #32
3000564a:	f11c 0f20 	cmn.w	ip, #32
3000564e:	d1f0      	bne.n	30005632 <BOOT_TCMSet+0xca>
30005650:	f3bf 8f4f 	dsb	sy
30005654:	f3bf 8f6f 	isb	sy
30005658:	f3bf 8f4f 	dsb	sy
3000565c:	f3bf 8f6f 	isb	sy
30005660:	4878      	ldr	r0, [pc, #480]	; (30005844 <BOOT_TCMSet+0x2dc>)
30005662:	2200      	movs	r2, #0
30005664:	6943      	ldr	r3, [r0, #20]
30005666:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
3000566a:	6143      	str	r3, [r0, #20]
3000566c:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005670:	f3bf 8f4f 	dsb	sy
30005674:	f3bf 8f6f 	isb	sy
30005678:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
3000567c:	f3bf 8f4f 	dsb	sy
30005680:	6943      	ldr	r3, [r0, #20]
30005682:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30005686:	6143      	str	r3, [r0, #20]
30005688:	f3bf 8f4f 	dsb	sy
3000568c:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
30005690:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30005694:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30005698:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000569c:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300056a0:	ea0c 010e 	and.w	r1, ip, lr
300056a4:	4633      	mov	r3, r6
300056a6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056aa:	3b01      	subs	r3, #1
300056ac:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300056b0:	1c5a      	adds	r2, r3, #1
300056b2:	d1f8      	bne.n	300056a6 <BOOT_TCMSet+0x13e>
300056b4:	f1ac 0c20 	sub.w	ip, ip, #32
300056b8:	f11c 0f20 	cmn.w	ip, #32
300056bc:	d1f0      	bne.n	300056a0 <BOOT_TCMSet+0x138>
300056be:	f3bf 8f4f 	dsb	sy
300056c2:	f3bf 8f6f 	isb	sy
300056c6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300056ca:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300056ce:	00e3      	lsls	r3, r4, #3
300056d0:	6b41      	ldr	r1, [r0, #52]	; 0x34
300056d2:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300056d6:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300056da:	6341      	str	r1, [r0, #52]	; 0x34
300056dc:	6911      	ldr	r1, [r2, #16]
300056de:	2903      	cmp	r1, #3
300056e0:	d00f      	beq.n	30005702 <BOOT_TCMSet+0x19a>
300056e2:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300056e6:	4858      	ldr	r0, [pc, #352]	; (30005848 <BOOT_TCMSet+0x2e0>)
300056e8:	f04f 0c00 	mov.w	ip, #0
300056ec:	4f57      	ldr	r7, [pc, #348]	; (3000584c <BOOT_TCMSet+0x2e4>)
300056ee:	68b2      	ldr	r2, [r6, #8]
300056f0:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300056f4:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300056f8:	60b2      	str	r2, [r6, #8]
300056fa:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300056fe:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
30005702:	1b1a      	subs	r2, r3, r4
30005704:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005708:	68d1      	ldr	r1, [r2, #12]
3000570a:	2903      	cmp	r1, #3
3000570c:	d00f      	beq.n	3000572e <BOOT_TCMSet+0x1c6>
3000570e:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
30005712:	484d      	ldr	r0, [pc, #308]	; (30005848 <BOOT_TCMSet+0x2e0>)
30005714:	f04f 0c00 	mov.w	ip, #0
30005718:	4f4c      	ldr	r7, [pc, #304]	; (3000584c <BOOT_TCMSet+0x2e4>)
3000571a:	68b2      	ldr	r2, [r6, #8]
3000571c:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005720:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30005724:	60b2      	str	r2, [r6, #8]
30005726:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
3000572a:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000572e:	1b1a      	subs	r2, r3, r4
30005730:	4945      	ldr	r1, [pc, #276]	; (30005848 <BOOT_TCMSet+0x2e0>)
30005732:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005736:	6950      	ldr	r0, [r2, #20]
30005738:	6608      	str	r0, [r1, #96]	; 0x60
3000573a:	6990      	ldr	r0, [r2, #24]
3000573c:	6648      	str	r0, [r1, #100]	; 0x64
3000573e:	6852      	ldr	r2, [r2, #4]
30005740:	2a01      	cmp	r2, #1
30005742:	d04b      	beq.n	300057dc <BOOT_TCMSet+0x274>
30005744:	1b1b      	subs	r3, r3, r4
30005746:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000574a:	68ab      	ldr	r3, [r5, #8]
3000574c:	2b01      	cmp	r3, #1
3000574e:	f47f af50 	bne.w	300055f2 <BOOT_TCMSet+0x8a>
30005752:	483b      	ldr	r0, [pc, #236]	; (30005840 <BOOT_TCMSet+0x2d8>)
30005754:	2300      	movs	r3, #0
30005756:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000575a:	f3bf 8f4f 	dsb	sy
3000575e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30005762:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30005766:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000576a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000576e:	0164      	lsls	r4, r4, #5
30005770:	ea04 0106 	and.w	r1, r4, r6
30005774:	462b      	mov	r3, r5
30005776:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000577a:	3b01      	subs	r3, #1
3000577c:	1c5f      	adds	r7, r3, #1
3000577e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30005782:	d1f8      	bne.n	30005776 <BOOT_TCMSet+0x20e>
30005784:	3c20      	subs	r4, #32
30005786:	f114 0f20 	cmn.w	r4, #32
3000578a:	d1f1      	bne.n	30005770 <BOOT_TCMSet+0x208>
3000578c:	f3bf 8f4f 	dsb	sy
30005790:	6943      	ldr	r3, [r0, #20]
30005792:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005796:	6143      	str	r3, [r0, #20]
30005798:	f3bf 8f4f 	dsb	sy
3000579c:	f3bf 8f6f 	isb	sy
300057a0:	4828      	ldr	r0, [pc, #160]	; (30005844 <BOOT_TCMSet+0x2dc>)
300057a2:	2300      	movs	r3, #0
300057a4:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300057a8:	f3bf 8f4f 	dsb	sy
300057ac:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300057b0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300057b4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300057b8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300057bc:	0164      	lsls	r4, r4, #5
300057be:	ea04 0106 	and.w	r1, r4, r6
300057c2:	462b      	mov	r3, r5
300057c4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300057c8:	3b01      	subs	r3, #1
300057ca:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300057ce:	1c5a      	adds	r2, r3, #1
300057d0:	d1f8      	bne.n	300057c4 <BOOT_TCMSet+0x25c>
300057d2:	3c20      	subs	r4, #32
300057d4:	f114 0f20 	cmn.w	r4, #32
300057d8:	d1f1      	bne.n	300057be <BOOT_TCMSet+0x256>
300057da:	e700      	b.n	300055de <BOOT_TCMSet+0x76>
300057dc:	f3bf 8f4f 	dsb	sy
300057e0:	f3bf 8f6f 	isb	sy
300057e4:	4a16      	ldr	r2, [pc, #88]	; (30005840 <BOOT_TCMSet+0x2d8>)
300057e6:	2000      	movs	r0, #0
300057e8:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300057ec:	f3bf 8f4f 	dsb	sy
300057f0:	f3bf 8f6f 	isb	sy
300057f4:	6951      	ldr	r1, [r2, #20]
300057f6:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300057fa:	6151      	str	r1, [r2, #20]
300057fc:	f3bf 8f4f 	dsb	sy
30005800:	f3bf 8f6f 	isb	sy
30005804:	f3bf 8f4f 	dsb	sy
30005808:	f3bf 8f6f 	isb	sy
3000580c:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30005810:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30005814:	f3bf 8f4f 	dsb	sy
30005818:	f3bf 8f6f 	isb	sy
3000581c:	6951      	ldr	r1, [r2, #20]
3000581e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30005822:	6151      	str	r1, [r2, #20]
30005824:	f3bf 8f4f 	dsb	sy
30005828:	f3bf 8f6f 	isb	sy
3000582c:	1b1b      	subs	r3, r3, r4
3000582e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30005832:	68ab      	ldr	r3, [r5, #8]
30005834:	2b01      	cmp	r3, #1
30005836:	f47f aedc 	bne.w	300055f2 <BOOT_TCMSet+0x8a>
3000583a:	e78a      	b.n	30005752 <BOOT_TCMSet+0x1ea>
3000583c:	3000ddc8 	.word	0x3000ddc8
30005840:	e002ed00 	.word	0xe002ed00
30005844:	e000ed00 	.word	0xe000ed00
30005848:	e0042000 	.word	0xe0042000
3000584c:	ffff0000 	.word	0xffff0000

30005850 <BOOT_GRstConfig>:
30005850:	4b13      	ldr	r3, [pc, #76]	; (300058a0 <BOOT_GRstConfig+0x50>)
30005852:	b570      	push	{r4, r5, r6, lr}
30005854:	681a      	ldr	r2, [r3, #0]
30005856:	1c50      	adds	r0, r2, #1
30005858:	d01e      	beq.n	30005898 <BOOT_GRstConfig+0x48>
3000585a:	2000      	movs	r0, #0
3000585c:	6859      	ldr	r1, [r3, #4]
3000585e:	2901      	cmp	r1, #1
30005860:	bf08      	it	eq
30005862:	4310      	orreq	r0, r2
30005864:	f853 2f08 	ldr.w	r2, [r3, #8]!
30005868:	1c51      	adds	r1, r2, #1
3000586a:	d1f7      	bne.n	3000585c <BOOT_GRstConfig+0xc>
3000586c:	b284      	uxth	r4, r0
3000586e:	43e3      	mvns	r3, r4
30005870:	b29e      	uxth	r6, r3
30005872:	4d0c      	ldr	r5, [pc, #48]	; (300058a4 <BOOT_GRstConfig+0x54>)
30005874:	2028      	movs	r0, #40	; 0x28
30005876:	4a0c      	ldr	r2, [pc, #48]	; (300058a8 <BOOT_GRstConfig+0x58>)
30005878:	882b      	ldrh	r3, [r5, #0]
3000587a:	b29b      	uxth	r3, r3
3000587c:	4323      	orrs	r3, r4
3000587e:	802b      	strh	r3, [r5, #0]
30005880:	4790      	blx	r2
30005882:	4a0a      	ldr	r2, [pc, #40]	; (300058ac <BOOT_GRstConfig+0x5c>)
30005884:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
30005888:	4033      	ands	r3, r6
3000588a:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
3000588e:	8868      	ldrh	r0, [r5, #2]
30005890:	b280      	uxth	r0, r0
30005892:	4320      	orrs	r0, r4
30005894:	8068      	strh	r0, [r5, #2]
30005896:	bd70      	pop	{r4, r5, r6, pc}
30005898:	f64f 76ff 	movw	r6, #65535	; 0xffff
3000589c:	2400      	movs	r4, #0
3000589e:	e7e8      	b.n	30005872 <BOOT_GRstConfig+0x22>
300058a0:	3000d360 	.word	0x3000d360
300058a4:	42008200 	.word	0x42008200
300058a8:	00009b2d 	.word	0x00009b2d
300058ac:	42008000 	.word	0x42008000

300058b0 <BOOT_DDR_LCDC_HPR>:
300058b0:	4a04      	ldr	r2, [pc, #16]	; (300058c4 <BOOT_DDR_LCDC_HPR+0x14>)
300058b2:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300058b6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300058ba:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300058be:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300058c2:	4770      	bx	lr
300058c4:	42008000 	.word	0x42008000

300058c8 <BOOT_DDR_Init>:
300058c8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300058cc:	b530      	push	{r4, r5, lr}
300058ce:	4d15      	ldr	r5, [pc, #84]	; (30005924 <BOOT_DDR_Init+0x5c>)
300058d0:	b099      	sub	sp, #100	; 0x64
300058d2:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300058d4:	47a8      	blx	r5
300058d6:	2803      	cmp	r0, #3
300058d8:	d11e      	bne.n	30005918 <BOOT_DDR_Init+0x50>
300058da:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300058de:	4a12      	ldr	r2, [pc, #72]	; (30005928 <BOOT_DDR_Init+0x60>)
300058e0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300058e4:	4911      	ldr	r1, [pc, #68]	; (3000592c <BOOT_DDR_Init+0x64>)
300058e6:	a801      	add	r0, sp, #4
300058e8:	631c      	str	r4, [r3, #48]	; 0x30
300058ea:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300058ee:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300058f2:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300058f6:	4788      	blx	r1
300058f8:	a801      	add	r0, sp, #4
300058fa:	4b0d      	ldr	r3, [pc, #52]	; (30005930 <BOOT_DDR_Init+0x68>)
300058fc:	4798      	blx	r3
300058fe:	4b0d      	ldr	r3, [pc, #52]	; (30005934 <BOOT_DDR_Init+0x6c>)
30005900:	4798      	blx	r3
30005902:	4b0d      	ldr	r3, [pc, #52]	; (30005938 <BOOT_DDR_Init+0x70>)
30005904:	4798      	blx	r3
30005906:	4b0d      	ldr	r3, [pc, #52]	; (3000593c <BOOT_DDR_Init+0x74>)
30005908:	4798      	blx	r3
3000590a:	2101      	movs	r1, #1
3000590c:	f240 10ff 	movw	r0, #511	; 0x1ff
30005910:	4b0b      	ldr	r3, [pc, #44]	; (30005940 <BOOT_DDR_Init+0x78>)
30005912:	4798      	blx	r3
30005914:	b019      	add	sp, #100	; 0x64
30005916:	bd30      	pop	{r4, r5, pc}
30005918:	47a8      	blx	r5
3000591a:	2802      	cmp	r0, #2
3000591c:	d0dd      	beq.n	300058da <BOOT_DDR_Init+0x12>
3000591e:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
30005922:	e7dc      	b.n	300058de <BOOT_DDR_Init+0x16>
30005924:	0000c0f9 	.word	0x0000c0f9
30005928:	42008000 	.word	0x42008000
3000592c:	30007f51 	.word	0x30007f51
30005930:	30008929 	.word	0x30008929
30005934:	3000b595 	.word	0x3000b595
30005938:	300089f9 	.word	0x300089f9
3000593c:	300089fd 	.word	0x300089fd
30005940:	3000b53d 	.word	0x3000b53d

30005944 <BOOT_PSRAM_Init>:
30005944:	4a1f      	ldr	r2, [pc, #124]	; (300059c4 <BOOT_PSRAM_Init+0x80>)
30005946:	4920      	ldr	r1, [pc, #128]	; (300059c8 <BOOT_PSRAM_Init+0x84>)
30005948:	b500      	push	{lr}
3000594a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
3000594e:	b089      	sub	sp, #36	; 0x24
30005950:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005954:	a801      	add	r0, sp, #4
30005956:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
3000595a:	4788      	blx	r1
3000595c:	a801      	add	r0, sp, #4
3000595e:	4b1b      	ldr	r3, [pc, #108]	; (300059cc <BOOT_PSRAM_Init+0x88>)
30005960:	4798      	blx	r3
30005962:	4b1b      	ldr	r3, [pc, #108]	; (300059d0 <BOOT_PSRAM_Init+0x8c>)
30005964:	4798      	blx	r3
30005966:	4b1b      	ldr	r3, [pc, #108]	; (300059d4 <BOOT_PSRAM_Init+0x90>)
30005968:	4798      	blx	r3
3000596a:	2801      	cmp	r0, #1
3000596c:	d026      	beq.n	300059bc <BOOT_PSRAM_Init+0x78>
3000596e:	4b1a      	ldr	r3, [pc, #104]	; (300059d8 <BOOT_PSRAM_Init+0x94>)
30005970:	689b      	ldr	r3, [r3, #8]
30005972:	079a      	lsls	r2, r3, #30
30005974:	d41e      	bmi.n	300059b4 <BOOT_PSRAM_Init+0x70>
30005976:	4b19      	ldr	r3, [pc, #100]	; (300059dc <BOOT_PSRAM_Init+0x98>)
30005978:	4798      	blx	r3
3000597a:	4b12      	ldr	r3, [pc, #72]	; (300059c4 <BOOT_PSRAM_Init+0x80>)
3000597c:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30005980:	009b      	lsls	r3, r3, #2
30005982:	d510      	bpl.n	300059a6 <BOOT_PSRAM_Init+0x62>
30005984:	4a16      	ldr	r2, [pc, #88]	; (300059e0 <BOOT_PSRAM_Init+0x9c>)
30005986:	4b17      	ldr	r3, [pc, #92]	; (300059e4 <BOOT_PSRAM_Init+0xa0>)
30005988:	6812      	ldr	r2, [r2, #0]
3000598a:	605a      	str	r2, [r3, #4]
3000598c:	4a16      	ldr	r2, [pc, #88]	; (300059e8 <BOOT_PSRAM_Init+0xa4>)
3000598e:	f04f 0c20 	mov.w	ip, #32
30005992:	2101      	movs	r1, #1
30005994:	4b15      	ldr	r3, [pc, #84]	; (300059ec <BOOT_PSRAM_Init+0xa8>)
30005996:	6952      	ldr	r2, [r2, #20]
30005998:	4608      	mov	r0, r1
3000599a:	fbbc f2f2 	udiv	r2, ip, r2
3000599e:	4798      	blx	r3
300059a0:	b009      	add	sp, #36	; 0x24
300059a2:	f85d fb04 	ldr.w	pc, [sp], #4
300059a6:	4b12      	ldr	r3, [pc, #72]	; (300059f0 <BOOT_PSRAM_Init+0xac>)
300059a8:	4798      	blx	r3
300059aa:	4a0e      	ldr	r2, [pc, #56]	; (300059e4 <BOOT_PSRAM_Init+0xa0>)
300059ac:	4b0c      	ldr	r3, [pc, #48]	; (300059e0 <BOOT_PSRAM_Init+0x9c>)
300059ae:	6852      	ldr	r2, [r2, #4]
300059b0:	601a      	str	r2, [r3, #0]
300059b2:	e7eb      	b.n	3000598c <BOOT_PSRAM_Init+0x48>
300059b4:	480f      	ldr	r0, [pc, #60]	; (300059f4 <BOOT_PSRAM_Init+0xb0>)
300059b6:	f006 fa3b 	bl	3000be30 <__DiagPrintf_veneer>
300059ba:	e7dc      	b.n	30005976 <BOOT_PSRAM_Init+0x32>
300059bc:	4b0e      	ldr	r3, [pc, #56]	; (300059f8 <BOOT_PSRAM_Init+0xb4>)
300059be:	4798      	blx	r3
300059c0:	e7db      	b.n	3000597a <BOOT_PSRAM_Init+0x36>
300059c2:	bf00      	nop
300059c4:	42008000 	.word	0x42008000
300059c8:	300092a5 	.word	0x300092a5
300059cc:	300092c5 	.word	0x300092c5
300059d0:	3000932d 	.word	0x3000932d
300059d4:	30007e8d 	.word	0x30007e8d
300059d8:	2001c00c 	.word	0x2001c00c
300059dc:	300095d1 	.word	0x300095d1
300059e0:	3000e990 	.word	0x3000e990
300059e4:	41002000 	.word	0x41002000
300059e8:	3000e5a0 	.word	0x3000e5a0
300059ec:	300098b5 	.word	0x300098b5
300059f0:	3000960d 	.word	0x3000960d
300059f4:	3000c52c 	.word	0x3000c52c
300059f8:	300094f9 	.word	0x300094f9

300059fc <BOOT_WakeFromPG>:
300059fc:	b538      	push	{r3, r4, r5, lr}
300059fe:	4c4b      	ldr	r4, [pc, #300]	; (30005b2c <BOOT_WakeFromPG+0x130>)
30005a00:	f7ff f980 	bl	30004d04 <BOOT_RAM_TZCfg>
30005a04:	69a3      	ldr	r3, [r4, #24]
30005a06:	b10b      	cbz	r3, 30005a0c <BOOT_WakeFromPG+0x10>
30005a08:	4a49      	ldr	r2, [pc, #292]	; (30005b30 <BOOT_WakeFromPG+0x134>)
30005a0a:	6093      	str	r3, [r2, #8]
30005a0c:	4b49      	ldr	r3, [pc, #292]	; (30005b34 <BOOT_WakeFromPG+0x138>)
30005a0e:	f06f 0c1a 	mvn.w	ip, #26
30005a12:	4849      	ldr	r0, [pc, #292]	; (30005b38 <BOOT_WakeFromPG+0x13c>)
30005a14:	f103 0e50 	add.w	lr, r3, #80	; 0x50
30005a18:	ebac 0c04 	sub.w	ip, ip, r4
30005a1c:	eb0c 0203 	add.w	r2, ip, r3
30005a20:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005a24:	4402      	add	r2, r0
30005a26:	4573      	cmp	r3, lr
30005a28:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005a2c:	d1f6      	bne.n	30005a1c <BOOT_WakeFromPG+0x20>
30005a2e:	6822      	ldr	r2, [r4, #0]
30005a30:	4b3f      	ldr	r3, [pc, #252]	; (30005b30 <BOOT_WakeFromPG+0x134>)
30005a32:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005a36:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005a3a:	6002      	str	r2, [r0, #0]
30005a3c:	6045      	str	r5, [r0, #4]
30005a3e:	6081      	str	r1, [r0, #8]
30005a40:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005a42:	4a3e      	ldr	r2, [pc, #248]	; (30005b3c <BOOT_WakeFromPG+0x140>)
30005a44:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005a48:	4c3d      	ldr	r4, [pc, #244]	; (30005b40 <BOOT_WakeFromPG+0x144>)
30005a4a:	6259      	str	r1, [r3, #36]	; 0x24
30005a4c:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005a4e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005a52:	6251      	str	r1, [r2, #36]	; 0x24
30005a54:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005a58:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a5c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005a60:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005a64:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a68:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005a6c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005a70:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005a74:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005a78:	f7ff fd48 	bl	3000550c <BOOT_CACHEWRR_Set>
30005a7c:	4b31      	ldr	r3, [pc, #196]	; (30005b44 <BOOT_WakeFromPG+0x148>)
30005a7e:	6818      	ldr	r0, [r3, #0]
30005a80:	f7ff fd72 	bl	30005568 <BOOT_TCMSet>
30005a84:	2201      	movs	r2, #1
30005a86:	4930      	ldr	r1, [pc, #192]	; (30005b48 <BOOT_WakeFromPG+0x14c>)
30005a88:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005a8c:	47a0      	blx	r4
30005a8e:	4b2f      	ldr	r3, [pc, #188]	; (30005b4c <BOOT_WakeFromPG+0x150>)
30005a90:	4798      	blx	r3
30005a92:	4b2f      	ldr	r3, [pc, #188]	; (30005b50 <BOOT_WakeFromPG+0x154>)
30005a94:	4798      	blx	r3
30005a96:	2801      	cmp	r0, #1
30005a98:	d130      	bne.n	30005afc <BOOT_WakeFromPG+0x100>
30005a9a:	4b2e      	ldr	r3, [pc, #184]	; (30005b54 <BOOT_WakeFromPG+0x158>)
30005a9c:	689b      	ldr	r3, [r3, #8]
30005a9e:	079b      	lsls	r3, r3, #30
30005aa0:	d43f      	bmi.n	30005b22 <BOOT_WakeFromPG+0x126>
30005aa2:	2201      	movs	r2, #1
30005aa4:	492c      	ldr	r1, [pc, #176]	; (30005b58 <BOOT_WakeFromPG+0x15c>)
30005aa6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005aaa:	47a0      	blx	r4
30005aac:	2201      	movs	r2, #1
30005aae:	492b      	ldr	r1, [pc, #172]	; (30005b5c <BOOT_WakeFromPG+0x160>)
30005ab0:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005ab4:	47a0      	blx	r4
30005ab6:	f7ff ff07 	bl	300058c8 <BOOT_DDR_Init>
30005aba:	4a29      	ldr	r2, [pc, #164]	; (30005b60 <BOOT_WakeFromPG+0x164>)
30005abc:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005ac0:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005ac4:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005ac8:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005acc:	4925      	ldr	r1, [pc, #148]	; (30005b64 <BOOT_WakeFromPG+0x168>)
30005ace:	4a26      	ldr	r2, [pc, #152]	; (30005b68 <BOOT_WakeFromPG+0x16c>)
30005ad0:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
30005ad4:	4c19      	ldr	r4, [pc, #100]	; (30005b3c <BOOT_WakeFromPG+0x140>)
30005ad6:	f043 0301 	orr.w	r3, r3, #1
30005ada:	4824      	ldr	r0, [pc, #144]	; (30005b6c <BOOT_WakeFromPG+0x170>)
30005adc:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
30005ae0:	6893      	ldr	r3, [r2, #8]
30005ae2:	6852      	ldr	r2, [r2, #4]
30005ae4:	605a      	str	r2, [r3, #4]
30005ae6:	60a3      	str	r3, [r4, #8]
30005ae8:	f380 8888 	msr	MSP_NS, r0
30005aec:	4a20      	ldr	r2, [pc, #128]	; (30005b70 <BOOT_WakeFromPG+0x174>)
30005aee:	f382 8809 	msr	PSP, r2
30005af2:	6858      	ldr	r0, [r3, #4]
30005af4:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30005af8:	f7ff bc4c 	b.w	30005394 <BOOT_NsStart>
30005afc:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005b00:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005b04:	2201      	movs	r2, #1
30005b06:	47a0      	blx	r4
30005b08:	4a15      	ldr	r2, [pc, #84]	; (30005b60 <BOOT_WakeFromPG+0x164>)
30005b0a:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005b0e:	f043 0302 	orr.w	r3, r3, #2
30005b12:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005b16:	f7ff ff15 	bl	30005944 <BOOT_PSRAM_Init>
30005b1a:	2096      	movs	r0, #150	; 0x96
30005b1c:	4b15      	ldr	r3, [pc, #84]	; (30005b74 <BOOT_WakeFromPG+0x178>)
30005b1e:	4798      	blx	r3
30005b20:	e7d4      	b.n	30005acc <BOOT_WakeFromPG+0xd0>
30005b22:	4815      	ldr	r0, [pc, #84]	; (30005b78 <BOOT_WakeFromPG+0x17c>)
30005b24:	f006 f984 	bl	3000be30 <__DiagPrintf_veneer>
30005b28:	e7bb      	b.n	30005aa2 <BOOT_WakeFromPG+0xa6>
30005b2a:	bf00      	nop
30005b2c:	3000e924 	.word	0x3000e924
30005b30:	e000ed00 	.word	0xe000ed00
30005b34:	3000e93f 	.word	0x3000e93f
30005b38:	e000e100 	.word	0xe000e100
30005b3c:	e002ed00 	.word	0xe002ed00
30005b40:	0000b479 	.word	0x0000b479
30005b44:	3000d298 	.word	0x3000d298
30005b48:	40001000 	.word	0x40001000
30005b4c:	0000d835 	.word	0x0000d835
30005b50:	30007db5 	.word	0x30007db5
30005b54:	2001c00c 	.word	0x2001c00c
30005b58:	40080000 	.word	0x40080000
30005b5c:	40040000 	.word	0x40040000
30005b60:	42008000 	.word	0x42008000
30005b64:	42008200 	.word	0x42008200
30005b68:	60000020 	.word	0x60000020
30005b6c:	2001bffc 	.word	0x2001bffc
30005b70:	2001b7fc 	.word	0x2001b7fc
30005b74:	00009b2d 	.word	0x00009b2d
30005b78:	3000c550 	.word	0x3000c550

30005b7c <BOOT_SCBConfig_HP>:
30005b7c:	4b0e      	ldr	r3, [pc, #56]	; (30005bb8 <BOOT_SCBConfig_HP+0x3c>)
30005b7e:	4a0f      	ldr	r2, [pc, #60]	; (30005bbc <BOOT_SCBConfig_HP+0x40>)
30005b80:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005b82:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005b86:	6259      	str	r1, [r3, #36]	; 0x24
30005b88:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005b8a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005b8e:	6251      	str	r1, [r2, #36]	; 0x24
30005b90:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005b94:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005b98:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005b9c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005ba0:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005ba4:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005ba8:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005bac:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005bb0:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005bb4:	4770      	bx	lr
30005bb6:	bf00      	nop
30005bb8:	e000ed00 	.word	0xe000ed00
30005bbc:	e002ed00 	.word	0xe002ed00

30005bc0 <BOOT_SCBVTORBackup_HP>:
30005bc0:	4a02      	ldr	r2, [pc, #8]	; (30005bcc <BOOT_SCBVTORBackup_HP+0xc>)
30005bc2:	4b03      	ldr	r3, [pc, #12]	; (30005bd0 <BOOT_SCBVTORBackup_HP+0x10>)
30005bc4:	6892      	ldr	r2, [r2, #8]
30005bc6:	619a      	str	r2, [r3, #24]
30005bc8:	4770      	bx	lr
30005bca:	bf00      	nop
30005bcc:	e000ed00 	.word	0xe000ed00
30005bd0:	3000e924 	.word	0x3000e924

30005bd4 <BOOT_SCBVTORReFill_HP>:
30005bd4:	4a02      	ldr	r2, [pc, #8]	; (30005be0 <BOOT_SCBVTORReFill_HP+0xc>)
30005bd6:	4b03      	ldr	r3, [pc, #12]	; (30005be4 <BOOT_SCBVTORReFill_HP+0x10>)
30005bd8:	6992      	ldr	r2, [r2, #24]
30005bda:	609a      	str	r2, [r3, #8]
30005bdc:	4770      	bx	lr
30005bde:	bf00      	nop
30005be0:	3000e924 	.word	0x3000e924
30005be4:	e000ed00 	.word	0xe000ed00

30005be8 <BOOT_NVICBackup_HP>:
30005be8:	491a      	ldr	r1, [pc, #104]	; (30005c54 <BOOT_NVICBackup_HP+0x6c>)
30005bea:	f06f 001a 	mvn.w	r0, #26
30005bee:	b410      	push	{r4}
30005bf0:	4c19      	ldr	r4, [pc, #100]	; (30005c58 <BOOT_NVICBackup_HP+0x70>)
30005bf2:	680a      	ldr	r2, [r1, #0]
30005bf4:	f104 031b 	add.w	r3, r4, #27
30005bf8:	1b00      	subs	r0, r0, r4
30005bfa:	6022      	str	r2, [r4, #0]
30005bfc:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005c00:	684a      	ldr	r2, [r1, #4]
30005c02:	6062      	str	r2, [r4, #4]
30005c04:	688a      	ldr	r2, [r1, #8]
30005c06:	60a2      	str	r2, [r4, #8]
30005c08:	18c2      	adds	r2, r0, r3
30005c0a:	440a      	add	r2, r1
30005c0c:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005c10:	f803 2f01 	strb.w	r2, [r3, #1]!
30005c14:	4563      	cmp	r3, ip
30005c16:	d1f7      	bne.n	30005c08 <BOOT_NVICBackup_HP+0x20>
30005c18:	4b10      	ldr	r3, [pc, #64]	; (30005c5c <BOOT_NVICBackup_HP+0x74>)
30005c1a:	695b      	ldr	r3, [r3, #20]
30005c1c:	03db      	lsls	r3, r3, #15
30005c1e:	d512      	bpl.n	30005c46 <BOOT_NVICBackup_HP+0x5e>
30005c20:	f014 011f 	ands.w	r1, r4, #31
30005c24:	4b0c      	ldr	r3, [pc, #48]	; (30005c58 <BOOT_NVICBackup_HP+0x70>)
30005c26:	d111      	bne.n	30005c4c <BOOT_NVICBackup_HP+0x64>
30005c28:	216c      	movs	r1, #108	; 0x6c
30005c2a:	f3bf 8f4f 	dsb	sy
30005c2e:	480b      	ldr	r0, [pc, #44]	; (30005c5c <BOOT_NVICBackup_HP+0x74>)
30005c30:	4419      	add	r1, r3
30005c32:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005c36:	3320      	adds	r3, #32
30005c38:	1aca      	subs	r2, r1, r3
30005c3a:	2a00      	cmp	r2, #0
30005c3c:	dcf9      	bgt.n	30005c32 <BOOT_NVICBackup_HP+0x4a>
30005c3e:	f3bf 8f4f 	dsb	sy
30005c42:	f3bf 8f6f 	isb	sy
30005c46:	f85d 4b04 	ldr.w	r4, [sp], #4
30005c4a:	4770      	bx	lr
30005c4c:	f024 031f 	bic.w	r3, r4, #31
30005c50:	316c      	adds	r1, #108	; 0x6c
30005c52:	e7ea      	b.n	30005c2a <BOOT_NVICBackup_HP+0x42>
30005c54:	e000e100 	.word	0xe000e100
30005c58:	3000e924 	.word	0x3000e924
30005c5c:	e000ed00 	.word	0xe000ed00

30005c60 <BOOT_NVICReFill_HP>:
30005c60:	b510      	push	{r4, lr}
30005c62:	f06f 0c1a 	mvn.w	ip, #26
30005c66:	4c0d      	ldr	r4, [pc, #52]	; (30005c9c <BOOT_NVICReFill_HP+0x3c>)
30005c68:	480d      	ldr	r0, [pc, #52]	; (30005ca0 <BOOT_NVICReFill_HP+0x40>)
30005c6a:	f104 031b 	add.w	r3, r4, #27
30005c6e:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005c72:	ebac 0c04 	sub.w	ip, ip, r4
30005c76:	eb0c 0203 	add.w	r2, ip, r3
30005c7a:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005c7e:	4402      	add	r2, r0
30005c80:	4573      	cmp	r3, lr
30005c82:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005c86:	d1f6      	bne.n	30005c76 <BOOT_NVICReFill_HP+0x16>
30005c88:	e9d4 3100 	ldrd	r3, r1, [r4]
30005c8c:	68a2      	ldr	r2, [r4, #8]
30005c8e:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005c92:	6003      	str	r3, [r0, #0]
30005c94:	6041      	str	r1, [r0, #4]
30005c96:	6082      	str	r2, [r0, #8]
30005c98:	bd10      	pop	{r4, pc}
30005c9a:	bf00      	nop
30005c9c:	3000e924 	.word	0x3000e924
30005ca0:	e000e100 	.word	0xe000e100

30005ca4 <BOOT_ImgCopy>:
30005ca4:	b510      	push	{r4, lr}
30005ca6:	b082      	sub	sp, #8
30005ca8:	4b09      	ldr	r3, [pc, #36]	; (30005cd0 <BOOT_ImgCopy+0x2c>)
30005caa:	4604      	mov	r4, r0
30005cac:	e9cd 2100 	strd	r2, r1, [sp]
30005cb0:	4798      	blx	r3
30005cb2:	e9dd 2100 	ldrd	r2, r1, [sp]
30005cb6:	b128      	cbz	r0, 30005cc4 <BOOT_ImgCopy+0x20>
30005cb8:	4620      	mov	r0, r4
30005cba:	4b06      	ldr	r3, [pc, #24]	; (30005cd4 <BOOT_ImgCopy+0x30>)
30005cbc:	b002      	add	sp, #8
30005cbe:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005cc2:	4718      	bx	r3
30005cc4:	4620      	mov	r0, r4
30005cc6:	b002      	add	sp, #8
30005cc8:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005ccc:	f003 b878 	b.w	30008dc0 <NandImgCopy>
30005cd0:	3000a149 	.word	0x3000a149
30005cd4:	00012a1d 	.word	0x00012a1d

30005cd8 <BOOT_LoadImages>:
30005cd8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005cdc:	4b52      	ldr	r3, [pc, #328]	; (30005e28 <BOOT_LoadImages+0x150>)
30005cde:	b0b7      	sub	sp, #220	; 0xdc
30005ce0:	2600      	movs	r6, #0
30005ce2:	2104      	movs	r1, #4
30005ce4:	aa11      	add	r2, sp, #68	; 0x44
30005ce6:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005cea:	4b50      	ldr	r3, [pc, #320]	; (30005e2c <BOOT_LoadImages+0x154>)
30005cec:	f04f 0901 	mov.w	r9, #1
30005cf0:	9603      	str	r6, [sp, #12]
30005cf2:	9605      	str	r6, [sp, #20]
30005cf4:	4798      	blx	r3
30005cf6:	f89d 7046 	ldrb.w	r7, [sp, #70]	; 0x46
30005cfa:	4c4d      	ldr	r4, [pc, #308]	; (30005e30 <BOOT_LoadImages+0x158>)
30005cfc:	fa09 f707 	lsl.w	r7, r9, r7
30005d00:	484c      	ldr	r0, [pc, #304]	; (30005e34 <BOOT_LoadImages+0x15c>)
30005d02:	4d4d      	ldr	r5, [pc, #308]	; (30005e38 <BOOT_LoadImages+0x160>)
30005d04:	eb07 0804 	add.w	r8, r7, r4
30005d08:	9614      	str	r6, [sp, #80]	; 0x50
30005d0a:	9615      	str	r6, [sp, #84]	; 0x54
30005d0c:	443d      	add	r5, r7
30005d0e:	e9cd 6612 	strd	r6, r6, [sp, #72]	; 0x48
30005d12:	f7ff f9d5 	bl	300050c0 <mcc_init_crc32>
30005d16:	f8d8 3004 	ldr.w	r3, [r8, #4]
30005d1a:	f898 200c 	ldrb.w	r2, [r8, #12]
30005d1e:	9308      	str	r3, [sp, #32]
30005d20:	3b01      	subs	r3, #1
30005d22:	9206      	str	r2, [sp, #24]
30005d24:	3303      	adds	r3, #3
30005d26:	686a      	ldr	r2, [r5, #4]
30005d28:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005d2c:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005d30:	9204      	str	r2, [sp, #16]
30005d32:	f200 81b8 	bhi.w	300060a6 <BOOT_LoadImages+0x3ce>
30005d36:	f8d8 3008 	ldr.w	r3, [r8, #8]
30005d3a:	9a04      	ldr	r2, [sp, #16]
30005d3c:	3b01      	subs	r3, #1
30005d3e:	3a01      	subs	r2, #1
30005d40:	3303      	adds	r3, #3
30005d42:	d867      	bhi.n	30005e14 <BOOT_LoadImages+0x13c>
30005d44:	9b06      	ldr	r3, [sp, #24]
30005d46:	9207      	str	r2, [sp, #28]
30005d48:	2b01      	cmp	r3, #1
30005d4a:	f240 82fd 	bls.w	30006348 <BOOT_LoadImages+0x670>
30005d4e:	483b      	ldr	r0, [pc, #236]	; (30005e3c <BOOT_LoadImages+0x164>)
30005d50:	f006 f86e 	bl	3000be30 <__DiagPrintf_veneer>
30005d54:	9a07      	ldr	r2, [sp, #28]
30005d56:	3203      	adds	r2, #3
30005d58:	d863      	bhi.n	30005e22 <BOOT_LoadImages+0x14a>
30005d5a:	f10a 3aff 	add.w	sl, sl, #4294967295
30005d5e:	f11a 0f03 	cmn.w	sl, #3
30005d62:	f200 81a6 	bhi.w	300060b2 <BOOT_LoadImages+0x3da>
30005d66:	f1bb 0f01 	cmp.w	fp, #1
30005d6a:	f200 833a 	bhi.w	300063e2 <BOOT_LoadImages+0x70a>
30005d6e:	9904      	ldr	r1, [sp, #16]
30005d70:	4833      	ldr	r0, [pc, #204]	; (30005e40 <BOOT_LoadImages+0x168>)
30005d72:	f006 f85d 	bl	3000be30 <__DiagPrintf_veneer>
30005d76:	f8d8 3011 	ldr.w	r3, [r8, #17]
30005d7a:	f8d8 a00d 	ldr.w	sl, [r8, #13]
30005d7e:	4c2d      	ldr	r4, [pc, #180]	; (30005e34 <BOOT_LoadImages+0x15c>)
30005d80:	9307      	str	r3, [sp, #28]
30005d82:	2300      	movs	r3, #0
30005d84:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005d88:	f8d5 9011 	ldr.w	r9, [r5, #17]
30005d8c:	9309      	str	r3, [sp, #36]	; 0x24
30005d8e:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005e58 <BOOT_LoadImages+0x180>
30005d92:	f107 6300 	add.w	r3, r7, #134217728	; 0x8000000
30005d96:	6829      	ldr	r1, [r5, #0]
30005d98:	f04f 30ff 	mov.w	r0, #4294967295
30005d9c:	44bc      	add	ip, r7
30005d9e:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005da2:	4042      	eors	r2, r0
30005da4:	459c      	cmp	ip, r3
30005da6:	b2d2      	uxtb	r2, r2
30005da8:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005dac:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005db0:	d1f5      	bne.n	30005d9e <BOOT_LoadImages+0xc6>
30005db2:	43c2      	mvns	r2, r0
30005db4:	930f      	str	r3, [sp, #60]	; 0x3c
30005db6:	4291      	cmp	r1, r2
30005db8:	f000 82f8 	beq.w	300063ac <BOOT_LoadImages+0x6d4>
30005dbc:	4821      	ldr	r0, [pc, #132]	; (30005e44 <BOOT_LoadImages+0x16c>)
30005dbe:	f006 f837 	bl	3000be30 <__DiagPrintf_veneer>
30005dc2:	2200      	movs	r2, #0
30005dc4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005dc6:	9909      	ldr	r1, [sp, #36]	; 0x24
30005dc8:	2900      	cmp	r1, #0
30005dca:	d04e      	beq.n	30005e6a <BOOT_LoadImages+0x192>
30005dcc:	f898 100c 	ldrb.w	r1, [r8, #12]
30005dd0:	4553      	cmp	r3, sl
30005dd2:	9106      	str	r1, [sp, #24]
30005dd4:	d342      	bcc.n	30005e5c <BOOT_LoadImages+0x184>
30005dd6:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005dda:	45e2      	cmp	sl, ip
30005ddc:	d93e      	bls.n	30005e5c <BOOT_LoadImages+0x184>
30005dde:	9807      	ldr	r0, [sp, #28]
30005de0:	4560      	cmp	r0, ip
30005de2:	f240 81f7 	bls.w	300061d4 <BOOT_LoadImages+0x4fc>
30005de6:	4283      	cmp	r3, r0
30005de8:	f0c0 81f4 	bcc.w	300061d4 <BOOT_LoadImages+0x4fc>
30005dec:	2a00      	cmp	r2, #0
30005dee:	f000 8201 	beq.w	300061f4 <BOOT_LoadImages+0x51c>
30005df2:	455b      	cmp	r3, fp
30005df4:	f080 83e0 	bcs.w	300065b8 <BOOT_LoadImages+0x8e0>
30005df8:	4813      	ldr	r0, [pc, #76]	; (30005e48 <BOOT_LoadImages+0x170>)
30005dfa:	f006 f819 	bl	3000be30 <__DiagPrintf_veneer>
30005dfe:	9e06      	ldr	r6, [sp, #24]
30005e00:	4812      	ldr	r0, [pc, #72]	; (30005e4c <BOOT_LoadImages+0x174>)
30005e02:	4645      	mov	r5, r8
30005e04:	f006 f814 	bl	3000be30 <__DiagPrintf_veneer>
30005e08:	9b07      	ldr	r3, [sp, #28]
30005e0a:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005e0e:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005e12:	e04a      	b.n	30005eaa <BOOT_LoadImages+0x1d2>
30005e14:	480e      	ldr	r0, [pc, #56]	; (30005e50 <BOOT_LoadImages+0x178>)
30005e16:	9207      	str	r2, [sp, #28]
30005e18:	f006 f80a 	bl	3000be30 <__DiagPrintf_veneer>
30005e1c:	9a07      	ldr	r2, [sp, #28]
30005e1e:	3203      	adds	r2, #3
30005e20:	d99b      	bls.n	30005d5a <BOOT_LoadImages+0x82>
30005e22:	480c      	ldr	r0, [pc, #48]	; (30005e54 <BOOT_LoadImages+0x17c>)
30005e24:	e146      	b.n	300060b4 <BOOT_LoadImages+0x3dc>
30005e26:	bf00      	nop
30005e28:	2001c01c 	.word	0x2001c01c
30005e2c:	00009db5 	.word	0x00009db5
30005e30:	07ffe000 	.word	0x07ffe000
30005e34:	3000d828 	.word	0x3000d828
30005e38:	07fff000 	.word	0x07fff000
30005e3c:	3000c5a4 	.word	0x3000c5a4
30005e40:	3000c98c 	.word	0x3000c98c
30005e44:	3000c6a8 	.word	0x3000c6a8
30005e48:	3000c970 	.word	0x3000c970
30005e4c:	3000c734 	.word	0x3000c734
30005e50:	3000c590 	.word	0x3000c590
30005e54:	3000c93c 	.word	0x3000c93c
30005e58:	07fff004 	.word	0x07fff004
30005e5c:	48c0      	ldr	r0, [pc, #768]	; (30006160 <BOOT_LoadImages+0x488>)
30005e5e:	930f      	str	r3, [sp, #60]	; 0x3c
30005e60:	9209      	str	r2, [sp, #36]	; 0x24
30005e62:	f005 ffe5 	bl	3000be30 <__DiagPrintf_veneer>
30005e66:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005e68:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005e6a:	2a00      	cmp	r2, #0
30005e6c:	f000 812d 	beq.w	300060ca <BOOT_LoadImages+0x3f2>
30005e70:	459b      	cmp	fp, r3
30005e72:	f200 82b8 	bhi.w	300063e6 <BOOT_LoadImages+0x70e>
30005e76:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005e7a:	bf2c      	ite	cs
30005e7c:	2200      	movcs	r2, #0
30005e7e:	2201      	movcc	r2, #1
30005e80:	2a00      	cmp	r2, #0
30005e82:	f040 82b0 	bne.w	300063e6 <BOOT_LoadImages+0x70e>
30005e86:	4599      	cmp	r9, r3
30005e88:	f200 81ac 	bhi.w	300061e4 <BOOT_LoadImages+0x50c>
30005e8c:	f1b9 6f00 	cmp.w	r9, #134217728	; 0x8000000
30005e90:	f0c0 81a8 	bcc.w	300061e4 <BOOT_LoadImages+0x50c>
30005e94:	7b2e      	ldrb	r6, [r5, #12]
30005e96:	2a00      	cmp	r2, #0
30005e98:	f040 81d2 	bne.w	30006240 <BOOT_LoadImages+0x568>
30005e9c:	48b1      	ldr	r0, [pc, #708]	; (30006164 <BOOT_LoadImages+0x48c>)
30005e9e:	f005 ffc7 	bl	3000be30 <__DiagPrintf_veneer>
30005ea2:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005ea6:	f8c4 9404 	str.w	r9, [r4, #1028]	; 0x404
30005eaa:	4631      	mov	r1, r6
30005eac:	48ae      	ldr	r0, [pc, #696]	; (30006168 <BOOT_LoadImages+0x490>)
30005eae:	f005 ffbf 	bl	3000be30 <__DiagPrintf_veneer>
30005eb2:	4bae      	ldr	r3, [pc, #696]	; (3000616c <BOOT_LoadImages+0x494>)
30005eb4:	4798      	blx	r3
30005eb6:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005eba:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005ebe:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005ec2:	6809      	ldr	r1, [r1, #0]
30005ec4:	330c      	adds	r3, #12
30005ec6:	910b      	str	r1, [sp, #44]	; 0x2c
30005ec8:	42bb      	cmp	r3, r7
30005eca:	bf28      	it	cs
30005ecc:	463b      	movcs	r3, r7
30005ece:	930d      	str	r3, [sp, #52]	; 0x34
30005ed0:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005ed4:	6812      	ldr	r2, [r2, #0]
30005ed6:	330c      	adds	r3, #12
30005ed8:	920a      	str	r2, [sp, #40]	; 0x28
30005eda:	429f      	cmp	r7, r3
30005edc:	bf28      	it	cs
30005ede:	461f      	movcs	r7, r3
30005ee0:	970c      	str	r7, [sp, #48]	; 0x30
30005ee2:	2800      	cmp	r0, #0
30005ee4:	f040 8188 	bne.w	300061f8 <BOOT_LoadImages+0x520>
30005ee8:	4ba1      	ldr	r3, [pc, #644]	; (30006170 <BOOT_LoadImages+0x498>)
30005eea:	4607      	mov	r7, r0
30005eec:	f8df b2a8 	ldr.w	fp, [pc, #680]	; 30006198 <BOOT_LoadImages+0x4c0>
30005ef0:	4681      	mov	r9, r0
30005ef2:	9304      	str	r3, [sp, #16]
30005ef4:	4680      	mov	r8, r0
30005ef6:	4b9f      	ldr	r3, [pc, #636]	; (30006174 <BOOT_LoadImages+0x49c>)
30005ef8:	9003      	str	r0, [sp, #12]
30005efa:	9306      	str	r3, [sp, #24]
30005efc:	2e00      	cmp	r6, #0
30005efe:	f040 81bb 	bne.w	30006278 <BOOT_LoadImages+0x5a0>
30005f02:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005f06:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005f08:	f10c 0104 	add.w	r1, ip, #4
30005f0c:	2b00      	cmp	r3, #0
30005f0e:	f000 83d2 	beq.w	300066b6 <BOOT_LoadImages+0x9de>
30005f12:	1d18      	adds	r0, r3, #4
30005f14:	f04f 32ff 	mov.w	r2, #4294967295
30005f18:	4460      	add	r0, ip
30005f1a:	f811 3b01 	ldrb.w	r3, [r1], #1
30005f1e:	4053      	eors	r3, r2
30005f20:	4281      	cmp	r1, r0
30005f22:	b2db      	uxtb	r3, r3
30005f24:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005f28:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005f2c:	d1f5      	bne.n	30005f1a <BOOT_LoadImages+0x242>
30005f2e:	43d2      	mvns	r2, r2
30005f30:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005f32:	4293      	cmp	r3, r2
30005f34:	f000 8396 	beq.w	30006664 <BOOT_LoadImages+0x98c>
30005f38:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005f3c:	f089 0301 	eor.w	r3, r9, #1
30005f40:	3101      	adds	r1, #1
30005f42:	f003 0301 	and.w	r3, r3, #1
30005f46:	ea47 0703 	orr.w	r7, r7, r3
30005f4a:	f000 831b 	beq.w	30006584 <BOOT_LoadImages+0x8ac>
30005f4e:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005f50:	4889      	ldr	r0, [pc, #548]	; (30006178 <BOOT_LoadImages+0x4a0>)
30005f52:	f005 ff6d 	bl	3000be30 <__DiagPrintf_veneer>
30005f56:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005f5a:	2002      	movs	r0, #2
30005f5c:	f7ff f82e 	bl	30004fbc <FLASH_Erase_With_Lock>
30005f60:	2f00      	cmp	r7, #0
30005f62:	f040 8244 	bne.w	300063ee <BOOT_LoadImages+0x716>
30005f66:	9705      	str	r7, [sp, #20]
30005f68:	f088 0701 	eor.w	r7, r8, #1
30005f6c:	f04f 0901 	mov.w	r9, #1
30005f70:	f007 0701 	and.w	r7, r7, #1
30005f74:	9b03      	ldr	r3, [sp, #12]
30005f76:	2b01      	cmp	r3, #1
30005f78:	f000 822b 	beq.w	300063d2 <BOOT_LoadImages+0x6fa>
30005f7c:	9b03      	ldr	r3, [sp, #12]
30005f7e:	2b02      	cmp	r3, #2
30005f80:	f000 83f6 	beq.w	30006770 <BOOT_LoadImages+0xa98>
30005f84:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30005f86:	4640      	mov	r0, r8
30005f88:	f8cb 3000 	str.w	r3, [fp]
30005f8c:	9b04      	ldr	r3, [sp, #16]
30005f8e:	4798      	blx	r3
30005f90:	1e03      	subs	r3, r0, #0
30005f92:	bf18      	it	ne
30005f94:	2301      	movne	r3, #1
30005f96:	2800      	cmp	r0, #0
30005f98:	f000 8149 	beq.w	3000622e <BOOT_LoadImages+0x556>
30005f9c:	2f00      	cmp	r7, #0
30005f9e:	f000 8146 	beq.w	3000622e <BOOT_LoadImages+0x556>
30005fa2:	fab6 f686 	clz	r6, r6
30005fa6:	9b05      	ldr	r3, [sp, #20]
30005fa8:	0976      	lsrs	r6, r6, #5
30005faa:	2b00      	cmp	r3, #0
30005fac:	f000 83ec 	beq.w	30006788 <BOOT_LoadImages+0xab0>
30005fb0:	4698      	mov	r8, r3
30005fb2:	4c72      	ldr	r4, [pc, #456]	; (3000617c <BOOT_LoadImages+0x4a4>)
30005fb4:	2e00      	cmp	r6, #0
30005fb6:	f040 812b 	bne.w	30006210 <BOOT_LoadImages+0x538>
30005fba:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005fbe:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005fc2:	3201      	adds	r2, #1
30005fc4:	f000 822c 	beq.w	30006420 <BOOT_LoadImages+0x748>
30005fc8:	4a6d      	ldr	r2, [pc, #436]	; (30006180 <BOOT_LoadImages+0x4a8>)
30005fca:	6892      	ldr	r2, [r2, #8]
30005fcc:	0791      	lsls	r1, r2, #30
30005fce:	f100 82e9 	bmi.w	300065a4 <BOOT_LoadImages+0x8cc>
30005fd2:	2301      	movs	r3, #1
30005fd4:	9303      	str	r3, [sp, #12]
30005fd6:	f04f 0901 	mov.w	r9, #1
30005fda:	464f      	mov	r7, r9
30005fdc:	f8cd 9014 	str.w	r9, [sp, #20]
30005fe0:	f1b8 0f00 	cmp.w	r8, #0
30005fe4:	d0c6      	beq.n	30005f74 <BOOT_LoadImages+0x29c>
30005fe6:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30005fea:	2280      	movs	r2, #128	; 0x80
30005fec:	21ff      	movs	r1, #255	; 0xff
30005fee:	46c1      	mov	r9, r8
30005ff0:	4650      	mov	r0, sl
30005ff2:	f005 fefd 	bl	3000bdf0 <____wrap_memset_veneer>
30005ff6:	2280      	movs	r2, #128	; 0x80
30005ff8:	4629      	mov	r1, r5
30005ffa:	4650      	mov	r0, sl
30005ffc:	f005 ff00 	bl	3000be00 <____wrap_memcpy_veneer>
30006000:	2e01      	cmp	r6, #1
30006002:	f000 8304 	beq.w	3000660e <BOOT_LoadImages+0x936>
30006006:	2100      	movs	r1, #0
30006008:	f8cd 8014 	str.w	r8, [sp, #20]
3000600c:	f04f 0801 	mov.w	r8, #1
30006010:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30006014:	1d2f      	adds	r7, r5, #4
30006016:	485b      	ldr	r0, [pc, #364]	; (30006184 <BOOT_LoadImages+0x4ac>)
30006018:	f005 ff0a 	bl	3000be30 <__DiagPrintf_veneer>
3000601c:	4629      	mov	r1, r5
3000601e:	2002      	movs	r0, #2
30006020:	f7fe ffcc 	bl	30004fbc <FLASH_Erase_With_Lock>
30006024:	aa17      	add	r2, sp, #92	; 0x5c
30006026:	217c      	movs	r1, #124	; 0x7c
30006028:	4638      	mov	r0, r7
3000602a:	f7fe ffef 	bl	3000500c <FLASH_TxData_With_Lock>
3000602e:	4638      	mov	r0, r7
30006030:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30006034:	f04f 32ff 	mov.w	r2, #4294967295
30006038:	f810 3b01 	ldrb.w	r3, [r0], #1
3000603c:	4053      	eors	r3, r2
3000603e:	4288      	cmp	r0, r1
30006040:	b2db      	uxtb	r3, r3
30006042:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006046:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000604a:	d1f5      	bne.n	30006038 <BOOT_LoadImages+0x360>
3000604c:	43d2      	mvns	r2, r2
3000604e:	4629      	mov	r1, r5
30006050:	2002      	movs	r0, #2
30006052:	9216      	str	r2, [sp, #88]	; 0x58
30006054:	f7fe ffb2 	bl	30004fbc <FLASH_Erase_With_Lock>
30006058:	4652      	mov	r2, sl
3000605a:	2180      	movs	r1, #128	; 0x80
3000605c:	4628      	mov	r0, r5
3000605e:	f7fe ffd5 	bl	3000500c <FLASH_TxData_With_Lock>
30006062:	9b05      	ldr	r3, [sp, #20]
30006064:	2b00      	cmp	r3, #0
30006066:	f000 8137 	beq.w	300062d8 <BOOT_LoadImages+0x600>
3000606a:	f1b9 0f00 	cmp.w	r9, #0
3000606e:	f040 83a9 	bne.w	300067c4 <BOOT_LoadImages+0xaec>
30006072:	2e01      	cmp	r6, #1
30006074:	f000 8270 	beq.w	30006558 <BOOT_LoadImages+0x880>
30006078:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000607c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006080:	3201      	adds	r2, #1
30006082:	f000 827b 	beq.w	3000657c <BOOT_LoadImages+0x8a4>
30006086:	4a3e      	ldr	r2, [pc, #248]	; (30006180 <BOOT_LoadImages+0x4a8>)
30006088:	6892      	ldr	r2, [r2, #8]
3000608a:	0792      	lsls	r2, r2, #30
3000608c:	f100 82f7 	bmi.w	3000667e <BOOT_LoadImages+0x9a6>
30006090:	f088 0701 	eor.w	r7, r8, #1
30006094:	f04f 0901 	mov.w	r9, #1
30006098:	f007 0701 	and.w	r7, r7, #1
3000609c:	3310      	adds	r3, #16
3000609e:	930e      	str	r3, [sp, #56]	; 0x38
300060a0:	2302      	movs	r3, #2
300060a2:	9303      	str	r3, [sp, #12]
300060a4:	e76e      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
300060a6:	4838      	ldr	r0, [pc, #224]	; (30006188 <BOOT_LoadImages+0x4b0>)
300060a8:	f005 fec2 	bl	3000be30 <__DiagPrintf_veneer>
300060ac:	9b04      	ldr	r3, [sp, #16]
300060ae:	1e5a      	subs	r2, r3, #1
300060b0:	e651      	b.n	30005d56 <BOOT_LoadImages+0x7e>
300060b2:	4836      	ldr	r0, [pc, #216]	; (3000618c <BOOT_LoadImages+0x4b4>)
300060b4:	f005 febc 	bl	3000be30 <__DiagPrintf_veneer>
300060b8:	f8d8 3011 	ldr.w	r3, [r8, #17]
300060bc:	f8d8 a00d 	ldr.w	sl, [r8, #13]
300060c0:	9307      	str	r3, [sp, #28]
300060c2:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300060c6:	f8d5 9011 	ldr.w	r9, [r5, #17]
300060ca:	4831      	ldr	r0, [pc, #196]	; (30006190 <BOOT_LoadImages+0x4b8>)
300060cc:	f005 feb0 	bl	3000be30 <__DiagPrintf_veneer>
300060d0:	4830      	ldr	r0, [pc, #192]	; (30006194 <BOOT_LoadImages+0x4bc>)
300060d2:	f005 fead 	bl	3000be30 <__DiagPrintf_veneer>
300060d6:	4639      	mov	r1, r7
300060d8:	aa12      	add	r2, sp, #72	; 0x48
300060da:	4650      	mov	r0, sl
300060dc:	f7ff f836 	bl	3000514c <recovery_check>
300060e0:	4639      	mov	r1, r7
300060e2:	4604      	mov	r4, r0
300060e4:	aa13      	add	r2, sp, #76	; 0x4c
300060e6:	9807      	ldr	r0, [sp, #28]
300060e8:	f7ff f830 	bl	3000514c <recovery_check>
300060ec:	4639      	mov	r1, r7
300060ee:	aa14      	add	r2, sp, #80	; 0x50
300060f0:	9004      	str	r0, [sp, #16]
300060f2:	4658      	mov	r0, fp
300060f4:	f7ff f82a 	bl	3000514c <recovery_check>
300060f8:	4639      	mov	r1, r7
300060fa:	aa15      	add	r2, sp, #84	; 0x54
300060fc:	4607      	mov	r7, r0
300060fe:	4648      	mov	r0, r9
30006100:	f7ff f824 	bl	3000514c <recovery_check>
30006104:	9b04      	ldr	r3, [sp, #16]
30006106:	4307      	orrs	r7, r0
30006108:	431c      	orrs	r4, r3
3000610a:	d047      	beq.n	3000619c <BOOT_LoadImages+0x4c4>
3000610c:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30006110:	2f00      	cmp	r7, #0
30006112:	f000 81df 	beq.w	300064d4 <BOOT_LoadImages+0x7fc>
30006116:	4293      	cmp	r3, r2
30006118:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
3000611c:	f0c0 8250 	bcc.w	300065c0 <BOOT_LoadImages+0x8e8>
30006120:	428b      	cmp	r3, r1
30006122:	d302      	bcc.n	3000612a <BOOT_LoadImages+0x452>
30006124:	4283      	cmp	r3, r0
30006126:	f080 8327 	bcs.w	30006778 <BOOT_LoadImages+0xaa0>
3000612a:	4293      	cmp	r3, r2
3000612c:	f000 8248 	beq.w	300065c0 <BOOT_LoadImages+0x8e8>
30006130:	4293      	cmp	r3, r2
30006132:	bf38      	it	cc
30006134:	4613      	movcc	r3, r2
30006136:	428b      	cmp	r3, r1
30006138:	f240 82fb 	bls.w	30006732 <BOOT_LoadImages+0xa5a>
3000613c:	428b      	cmp	r3, r1
3000613e:	bf38      	it	cc
30006140:	460b      	movcc	r3, r1
30006142:	4283      	cmp	r3, r0
30006144:	f240 830d 	bls.w	30006762 <BOOT_LoadImages+0xa8a>
30006148:	4b09      	ldr	r3, [pc, #36]	; (30006170 <BOOT_LoadImages+0x498>)
3000614a:	f04f 0900 	mov.w	r9, #0
3000614e:	2701      	movs	r7, #1
30006150:	f8df b044 	ldr.w	fp, [pc, #68]	; 30006198 <BOOT_LoadImages+0x4c0>
30006154:	9304      	str	r3, [sp, #16]
30006156:	46c8      	mov	r8, r9
30006158:	4b06      	ldr	r3, [pc, #24]	; (30006174 <BOOT_LoadImages+0x49c>)
3000615a:	9306      	str	r3, [sp, #24]
3000615c:	e712      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
3000615e:	bf00      	nop
30006160:	3000c6e8 	.word	0x3000c6e8
30006164:	3000c744 	.word	0x3000c744
30006168:	3000c778 	.word	0x3000c778
3000616c:	0000c149 	.word	0x0000c149
30006170:	300043b9 	.word	0x300043b9
30006174:	3000d296 	.word	0x3000d296
30006178:	3000c7e8 	.word	0x3000c7e8
3000617c:	3000d828 	.word	0x3000d828
30006180:	2001c00c 	.word	0x2001c00c
30006184:	3000c8bc 	.word	0x3000c8bc
30006188:	3000c578 	.word	0x3000c578
3000618c:	3000c5dc 	.word	0x3000c5dc
30006190:	3000c754 	.word	0x3000c754
30006194:	3000c768 	.word	0x3000c768
30006198:	3000e920 	.word	0x3000e920
3000619c:	2f00      	cmp	r7, #0
3000619e:	f000 8284 	beq.w	300066aa <BOOT_LoadImages+0x9d2>
300061a2:	4659      	mov	r1, fp
300061a4:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
300061a8:	e9cd 5400 	strd	r5, r4, [sp]
300061ac:	429a      	cmp	r2, r3
300061ae:	4643      	mov	r3, r8
300061b0:	464a      	mov	r2, r9
300061b2:	f080 81ff 	bcs.w	300065b4 <BOOT_LoadImages+0x8dc>
300061b6:	2001      	movs	r0, #1
300061b8:	f7fe fff8 	bl	300051ac <rewrite_bp>
300061bc:	46a1      	mov	r9, r4
300061be:	b2c3      	uxtb	r3, r0
300061c0:	2701      	movs	r7, #1
300061c2:	46a0      	mov	r8, r4
300061c4:	f8df b308 	ldr.w	fp, [pc, #776]	; 300064d0 <BOOT_LoadImages+0x7f8>
300061c8:	9303      	str	r3, [sp, #12]
300061ca:	4bac      	ldr	r3, [pc, #688]	; (3000647c <BOOT_LoadImages+0x7a4>)
300061cc:	9304      	str	r3, [sp, #16]
300061ce:	4bac      	ldr	r3, [pc, #688]	; (30006480 <BOOT_LoadImages+0x7a8>)
300061d0:	9306      	str	r3, [sp, #24]
300061d2:	e6cf      	b.n	30005f74 <BOOT_LoadImages+0x29c>
300061d4:	48ab      	ldr	r0, [pc, #684]	; (30006484 <BOOT_LoadImages+0x7ac>)
300061d6:	930f      	str	r3, [sp, #60]	; 0x3c
300061d8:	9209      	str	r2, [sp, #36]	; 0x24
300061da:	f005 fe29 	bl	3000be30 <__DiagPrintf_veneer>
300061de:	9a09      	ldr	r2, [sp, #36]	; 0x24
300061e0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300061e2:	e642      	b.n	30005e6a <BOOT_LoadImages+0x192>
300061e4:	48a8      	ldr	r0, [pc, #672]	; (30006488 <BOOT_LoadImages+0x7b0>)
300061e6:	9204      	str	r2, [sp, #16]
300061e8:	f005 fe22 	bl	3000be30 <__DiagPrintf_veneer>
300061ec:	9a04      	ldr	r2, [sp, #16]
300061ee:	2a00      	cmp	r2, #0
300061f0:	f43f af6b 	beq.w	300060ca <BOOT_LoadImages+0x3f2>
300061f4:	9e06      	ldr	r6, [sp, #24]
300061f6:	e603      	b.n	30005e00 <BOOT_LoadImages+0x128>
300061f8:	4aa0      	ldr	r2, [pc, #640]	; (3000647c <BOOT_LoadImages+0x7a4>)
300061fa:	2300      	movs	r3, #0
300061fc:	f8df b2d0 	ldr.w	fp, [pc, #720]	; 300064d0 <BOOT_LoadImages+0x7f8>
30006200:	9204      	str	r2, [sp, #16]
30006202:	4698      	mov	r8, r3
30006204:	4a9e      	ldr	r2, [pc, #632]	; (30006480 <BOOT_LoadImages+0x7a8>)
30006206:	9303      	str	r3, [sp, #12]
30006208:	9206      	str	r2, [sp, #24]
3000620a:	2e00      	cmp	r6, #0
3000620c:	f43f aed5 	beq.w	30005fba <BOOT_LoadImages+0x2e2>
30006210:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006214:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006218:	3201      	adds	r2, #1
3000621a:	f000 81ff 	beq.w	3000661c <BOOT_LoadImages+0x944>
3000621e:	4a9b      	ldr	r2, [pc, #620]	; (3000648c <BOOT_LoadImages+0x7b4>)
30006220:	6892      	ldr	r2, [r2, #8]
30006222:	0792      	lsls	r2, r2, #30
30006224:	f100 81b6 	bmi.w	30006594 <BOOT_LoadImages+0x8bc>
30006228:	2302      	movs	r3, #2
3000622a:	9303      	str	r3, [sp, #12]
3000622c:	e6d3      	b.n	30005fd6 <BOOT_LoadImages+0x2fe>
3000622e:	f008 0401 	and.w	r4, r8, #1
30006232:	b1c3      	cbz	r3, 30006266 <BOOT_LoadImages+0x58e>
30006234:	b1bc      	cbz	r4, 30006266 <BOOT_LoadImages+0x58e>
30006236:	4c96      	ldr	r4, [pc, #600]	; (30006490 <BOOT_LoadImages+0x7b8>)
30006238:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000623c:	47a0      	blx	r4
3000623e:	e7fb      	b.n	30006238 <BOOT_LoadImages+0x560>
30006240:	4894      	ldr	r0, [pc, #592]	; (30006494 <BOOT_LoadImages+0x7bc>)
30006242:	f005 fdf5 	bl	3000be30 <__DiagPrintf_veneer>
30006246:	9b08      	ldr	r3, [sp, #32]
30006248:	9a04      	ldr	r2, [sp, #16]
3000624a:	4293      	cmp	r3, r2
3000624c:	f4ff ae26 	bcc.w	30005e9c <BOOT_LoadImages+0x1c4>
30006250:	4891      	ldr	r0, [pc, #580]	; (30006498 <BOOT_LoadImages+0x7c0>)
30006252:	4645      	mov	r5, r8
30006254:	f005 fdec 	bl	3000be30 <__DiagPrintf_veneer>
30006258:	9b07      	ldr	r3, [sp, #28]
3000625a:	9e06      	ldr	r6, [sp, #24]
3000625c:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30006260:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30006264:	e621      	b.n	30005eaa <BOOT_LoadImages+0x1d2>
30006266:	9b06      	ldr	r3, [sp, #24]
30006268:	781b      	ldrb	r3, [r3, #0]
3000626a:	2b00      	cmp	r3, #0
3000626c:	f040 80f4 	bne.w	30006458 <BOOT_LoadImages+0x780>
30006270:	2001      	movs	r0, #1
30006272:	b037      	add	sp, #220	; 0xdc
30006274:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006278:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
3000627c:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000627e:	f10c 0104 	add.w	r1, ip, #4
30006282:	2b00      	cmp	r3, #0
30006284:	f000 8215 	beq.w	300066b2 <BOOT_LoadImages+0x9da>
30006288:	1d18      	adds	r0, r3, #4
3000628a:	f04f 32ff 	mov.w	r2, #4294967295
3000628e:	4460      	add	r0, ip
30006290:	f811 3b01 	ldrb.w	r3, [r1], #1
30006294:	4053      	eors	r3, r2
30006296:	4281      	cmp	r1, r0
30006298:	b2db      	uxtb	r3, r3
3000629a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000629e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300062a2:	d1f5      	bne.n	30006290 <BOOT_LoadImages+0x5b8>
300062a4:	43d2      	mvns	r2, r2
300062a6:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300062a8:	4293      	cmp	r3, r2
300062aa:	f000 81cd 	beq.w	30006648 <BOOT_LoadImages+0x970>
300062ae:	f8dc 100a 	ldr.w	r1, [ip, #10]
300062b2:	f089 0301 	eor.w	r3, r9, #1
300062b6:	3101      	adds	r1, #1
300062b8:	f003 0301 	and.w	r3, r3, #1
300062bc:	ea47 0703 	orr.w	r7, r7, r3
300062c0:	f000 8164 	beq.w	3000658c <BOOT_LoadImages+0x8b4>
300062c4:	990a      	ldr	r1, [sp, #40]	; 0x28
300062c6:	4875      	ldr	r0, [pc, #468]	; (3000649c <BOOT_LoadImages+0x7c4>)
300062c8:	f005 fdb2 	bl	3000be30 <__DiagPrintf_veneer>
300062cc:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300062d0:	2002      	movs	r0, #2
300062d2:	f7fe fe73 	bl	30004fbc <FLASH_Erase_With_Lock>
300062d6:	e643      	b.n	30005f60 <BOOT_LoadImages+0x288>
300062d8:	f1b9 0f00 	cmp.w	r9, #0
300062dc:	f040 826c 	bne.w	300067b8 <BOOT_LoadImages+0xae0>
300062e0:	2e01      	cmp	r6, #1
300062e2:	f000 8111 	beq.w	30006508 <BOOT_LoadImages+0x830>
300062e6:	f8d4 7404 	ldr.w	r7, [r4, #1028]	; 0x404
300062ea:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300062ec:	1d39      	adds	r1, r7, #4
300062ee:	2b00      	cmp	r3, #0
300062f0:	f000 8212 	beq.w	30006718 <BOOT_LoadImages+0xa40>
300062f4:	1d18      	adds	r0, r3, #4
300062f6:	f04f 32ff 	mov.w	r2, #4294967295
300062fa:	4438      	add	r0, r7
300062fc:	f811 3b01 	ldrb.w	r3, [r1], #1
30006300:	4053      	eors	r3, r2
30006302:	4281      	cmp	r1, r0
30006304:	b2db      	uxtb	r3, r3
30006306:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000630a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000630e:	d1f5      	bne.n	300062fc <BOOT_LoadImages+0x624>
30006310:	43d2      	mvns	r2, r2
30006312:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006314:	4293      	cmp	r3, r2
30006316:	f000 81ba 	beq.w	3000668e <BOOT_LoadImages+0x9b6>
3000631a:	f8d7 300a 	ldr.w	r3, [r7, #10]
3000631e:	3301      	adds	r3, #1
30006320:	f000 812c 	beq.w	3000657c <BOOT_LoadImages+0x8a4>
30006324:	990a      	ldr	r1, [sp, #40]	; 0x28
30006326:	485d      	ldr	r0, [pc, #372]	; (3000649c <BOOT_LoadImages+0x7c4>)
30006328:	f005 fd82 	bl	3000be30 <__DiagPrintf_veneer>
3000632c:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006330:	2002      	movs	r0, #2
30006332:	f7fe fe43 	bl	30004fbc <FLASH_Erase_With_Lock>
30006336:	4b55      	ldr	r3, [pc, #340]	; (3000648c <BOOT_LoadImages+0x7b4>)
30006338:	689b      	ldr	r3, [r3, #8]
3000633a:	079b      	lsls	r3, r3, #30
3000633c:	d400      	bmi.n	30006340 <BOOT_LoadImages+0x668>
3000633e:	e7fe      	b.n	3000633e <BOOT_LoadImages+0x666>
30006340:	4857      	ldr	r0, [pc, #348]	; (300064a0 <BOOT_LoadImages+0x7c8>)
30006342:	f005 fd75 	bl	3000be30 <__DiagPrintf_veneer>
30006346:	e7fa      	b.n	3000633e <BOOT_LoadImages+0x666>
30006348:	9908      	ldr	r1, [sp, #32]
3000634a:	4856      	ldr	r0, [pc, #344]	; (300064a4 <BOOT_LoadImages+0x7cc>)
3000634c:	f005 fd70 	bl	3000be30 <__DiagPrintf_veneer>
30006350:	9a07      	ldr	r2, [sp, #28]
30006352:	3203      	adds	r2, #3
30006354:	f240 81b4 	bls.w	300066c0 <BOOT_LoadImages+0x9e8>
30006358:	4853      	ldr	r0, [pc, #332]	; (300064a8 <BOOT_LoadImages+0x7d0>)
3000635a:	f005 fd69 	bl	3000be30 <__DiagPrintf_veneer>
3000635e:	f8d8 3011 	ldr.w	r3, [r8, #17]
30006362:	f8d8 a00d 	ldr.w	sl, [r8, #13]
30006366:	9307      	str	r3, [sp, #28]
30006368:	f8d5 b00d 	ldr.w	fp, [r5, #13]
3000636c:	f8d5 9011 	ldr.w	r9, [r5, #17]
30006370:	9609      	str	r6, [sp, #36]	; 0x24
30006372:	484e      	ldr	r0, [pc, #312]	; (300064ac <BOOT_LoadImages+0x7d4>)
30006374:	f04f 32ff 	mov.w	r2, #4294967295
30006378:	5939      	ldr	r1, [r7, r4]
3000637a:	4438      	add	r0, r7
3000637c:	4c4c      	ldr	r4, [pc, #304]	; (300064b0 <BOOT_LoadImages+0x7d8>)
3000637e:	f810 3b01 	ldrb.w	r3, [r0], #1
30006382:	4053      	eors	r3, r2
30006384:	4285      	cmp	r5, r0
30006386:	b2db      	uxtb	r3, r3
30006388:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000638c:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006390:	d1f5      	bne.n	3000637e <BOOT_LoadImages+0x6a6>
30006392:	43d2      	mvns	r2, r2
30006394:	4291      	cmp	r1, r2
30006396:	d010      	beq.n	300063ba <BOOT_LoadImages+0x6e2>
30006398:	4846      	ldr	r0, [pc, #280]	; (300064b4 <BOOT_LoadImages+0x7dc>)
3000639a:	f005 fd49 	bl	3000be30 <__DiagPrintf_veneer>
3000639e:	9b09      	ldr	r3, [sp, #36]	; 0x24
300063a0:	2b00      	cmp	r3, #0
300063a2:	f43f ae92 	beq.w	300060ca <BOOT_LoadImages+0x3f2>
300063a6:	2300      	movs	r3, #0
300063a8:	9309      	str	r3, [sp, #36]	; 0x24
300063aa:	e4f0      	b.n	30005d8e <BOOT_LoadImages+0xb6>
300063ac:	460a      	mov	r2, r1
300063ae:	4842      	ldr	r0, [pc, #264]	; (300064b8 <BOOT_LoadImages+0x7e0>)
300063b0:	f005 fd3e 	bl	3000be30 <__DiagPrintf_veneer>
300063b4:	2201      	movs	r2, #1
300063b6:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300063b8:	e505      	b.n	30005dc6 <BOOT_LoadImages+0xee>
300063ba:	460a      	mov	r2, r1
300063bc:	483f      	ldr	r0, [pc, #252]	; (300064bc <BOOT_LoadImages+0x7e4>)
300063be:	f005 fd37 	bl	3000be30 <__DiagPrintf_veneer>
300063c2:	9b09      	ldr	r3, [sp, #36]	; 0x24
300063c4:	2b00      	cmp	r3, #0
300063c6:	f47f ace2 	bne.w	30005d8e <BOOT_LoadImages+0xb6>
300063ca:	461a      	mov	r2, r3
300063cc:	f107 6300 	add.w	r3, r7, #134217728	; 0x8000000
300063d0:	e4fc      	b.n	30005dcc <BOOT_LoadImages+0xf4>
300063d2:	4c37      	ldr	r4, [pc, #220]	; (300064b0 <BOOT_LoadImages+0x7d8>)
300063d4:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300063d8:	3310      	adds	r3, #16
300063da:	930e      	str	r3, [sp, #56]	; 0x38
300063dc:	2301      	movs	r3, #1
300063de:	9303      	str	r3, [sp, #12]
300063e0:	e5d0      	b.n	30005f84 <BOOT_LoadImages+0x2ac>
300063e2:	4837      	ldr	r0, [pc, #220]	; (300064c0 <BOOT_LoadImages+0x7e8>)
300063e4:	e666      	b.n	300060b4 <BOOT_LoadImages+0x3dc>
300063e6:	4837      	ldr	r0, [pc, #220]	; (300064c4 <BOOT_LoadImages+0x7ec>)
300063e8:	f005 fd22 	bl	3000be30 <__DiagPrintf_veneer>
300063ec:	e66d      	b.n	300060ca <BOOT_LoadImages+0x3f2>
300063ee:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
300063f2:	2280      	movs	r2, #128	; 0x80
300063f4:	21ff      	movs	r1, #255	; 0xff
300063f6:	4650      	mov	r0, sl
300063f8:	f005 fcfa 	bl	3000bdf0 <____wrap_memset_veneer>
300063fc:	2280      	movs	r2, #128	; 0x80
300063fe:	4629      	mov	r1, r5
30006400:	4650      	mov	r0, sl
30006402:	f005 fcfd 	bl	3000be00 <____wrap_memcpy_veneer>
30006406:	f1b8 0f00 	cmp.w	r8, #0
3000640a:	f040 81d2 	bne.w	300067b2 <BOOT_LoadImages+0xada>
3000640e:	b1e6      	cbz	r6, 3000644a <BOOT_LoadImages+0x772>
30006410:	f8cd 8014 	str.w	r8, [sp, #20]
30006414:	f04f 0800 	mov.w	r8, #0
30006418:	4641      	mov	r1, r8
3000641a:	f88d 8064 	strb.w	r8, [sp, #100]	; 0x64
3000641e:	e5f9      	b.n	30006014 <BOOT_LoadImages+0x33c>
30006420:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006424:	4828      	ldr	r0, [pc, #160]	; (300064c8 <BOOT_LoadImages+0x7f0>)
30006426:	f005 fd03 	bl	3000be30 <__DiagPrintf_veneer>
3000642a:	2280      	movs	r2, #128	; 0x80
3000642c:	21ff      	movs	r1, #255	; 0xff
3000642e:	4650      	mov	r0, sl
30006430:	f005 fcde 	bl	3000bdf0 <____wrap_memset_veneer>
30006434:	2280      	movs	r2, #128	; 0x80
30006436:	4629      	mov	r1, r5
30006438:	4650      	mov	r0, sl
3000643a:	46b1      	mov	r9, r6
3000643c:	f005 fce0 	bl	3000be00 <____wrap_memcpy_veneer>
30006440:	f1b8 0f00 	cmp.w	r8, #0
30006444:	f47f addf 	bne.w	30006006 <BOOT_LoadImages+0x32e>
30006448:	2601      	movs	r6, #1
3000644a:	2101      	movs	r1, #1
3000644c:	9605      	str	r6, [sp, #20]
3000644e:	2600      	movs	r6, #0
30006450:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30006454:	46b0      	mov	r8, r6
30006456:	e5dd      	b.n	30006014 <BOOT_LoadImages+0x33c>
30006458:	4b1c      	ldr	r3, [pc, #112]	; (300064cc <BOOT_LoadImages+0x7f4>)
3000645a:	4640      	mov	r0, r8
3000645c:	4798      	blx	r3
3000645e:	1e03      	subs	r3, r0, #0
30006460:	bf18      	it	ne
30006462:	2301      	movne	r3, #1
30006464:	b110      	cbz	r0, 3000646c <BOOT_LoadImages+0x794>
30006466:	2f00      	cmp	r7, #0
30006468:	f47f ad9b 	bne.w	30005fa2 <BOOT_LoadImages+0x2ca>
3000646c:	2c00      	cmp	r4, #0
3000646e:	f43f aeff 	beq.w	30006270 <BOOT_LoadImages+0x598>
30006472:	2b00      	cmp	r3, #0
30006474:	f47f aedf 	bne.w	30006236 <BOOT_LoadImages+0x55e>
30006478:	e6fa      	b.n	30006270 <BOOT_LoadImages+0x598>
3000647a:	bf00      	nop
3000647c:	300043b9 	.word	0x300043b9
30006480:	3000d296 	.word	0x3000d296
30006484:	3000c704 	.word	0x3000c704
30006488:	3000c920 	.word	0x3000c920
3000648c:	2001c00c 	.word	0x2001c00c
30006490:	00009be5 	.word	0x00009be5
30006494:	3000c720 	.word	0x3000c720
30006498:	3000c734 	.word	0x3000c734
3000649c:	3000c870 	.word	0x3000c870
300064a0:	3000c4f4 	.word	0x3000c4f4
300064a4:	3000c5c0 	.word	0x3000c5c0
300064a8:	3000c93c 	.word	0x3000c93c
300064ac:	07ffe004 	.word	0x07ffe004
300064b0:	3000d828 	.word	0x3000d828
300064b4:	3000c62c 	.word	0x3000c62c
300064b8:	3000c66c 	.word	0x3000c66c
300064bc:	3000c5f0 	.word	0x3000c5f0
300064c0:	3000c954 	.word	0x3000c954
300064c4:	3000c970 	.word	0x3000c970
300064c8:	3000c78c 	.word	0x3000c78c
300064cc:	300043bd 	.word	0x300043bd
300064d0:	3000e920 	.word	0x3000e920
300064d4:	4293      	cmp	r3, r2
300064d6:	f04f 0401 	mov.w	r4, #1
300064da:	4643      	mov	r3, r8
300064dc:	9a07      	ldr	r2, [sp, #28]
300064de:	bf28      	it	cs
300064e0:	4638      	movcs	r0, r7
300064e2:	4651      	mov	r1, sl
300064e4:	bf38      	it	cc
300064e6:	4620      	movcc	r0, r4
300064e8:	9500      	str	r5, [sp, #0]
300064ea:	9401      	str	r4, [sp, #4]
300064ec:	46b9      	mov	r9, r7
300064ee:	f7fe fe5d 	bl	300051ac <rewrite_bp>
300064f2:	4627      	mov	r7, r4
300064f4:	b2c3      	uxtb	r3, r0
300064f6:	f8df b318 	ldr.w	fp, [pc, #792]	; 30006810 <BOOT_LoadImages+0xb38>
300064fa:	46c8      	mov	r8, r9
300064fc:	9303      	str	r3, [sp, #12]
300064fe:	4bb5      	ldr	r3, [pc, #724]	; (300067d4 <BOOT_LoadImages+0xafc>)
30006500:	9304      	str	r3, [sp, #16]
30006502:	4bb5      	ldr	r3, [pc, #724]	; (300067d8 <BOOT_LoadImages+0xb00>)
30006504:	9306      	str	r3, [sp, #24]
30006506:	e535      	b.n	30005f74 <BOOT_LoadImages+0x29c>
30006508:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
3000650c:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000650e:	1d31      	adds	r1, r6, #4
30006510:	2b00      	cmp	r3, #0
30006512:	f000 8124 	beq.w	3000675e <BOOT_LoadImages+0xa86>
30006516:	1d18      	adds	r0, r3, #4
30006518:	f04f 32ff 	mov.w	r2, #4294967295
3000651c:	4430      	add	r0, r6
3000651e:	f811 3b01 	ldrb.w	r3, [r1], #1
30006522:	4053      	eors	r3, r2
30006524:	4281      	cmp	r1, r0
30006526:	b2db      	uxtb	r3, r3
30006528:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000652c:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006530:	d1f5      	bne.n	3000651e <BOOT_LoadImages+0x846>
30006532:	43d2      	mvns	r2, r2
30006534:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30006536:	4293      	cmp	r3, r2
30006538:	f000 80cc 	beq.w	300066d4 <BOOT_LoadImages+0x9fc>
3000653c:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006540:	3301      	adds	r3, #1
30006542:	d058      	beq.n	300065f6 <BOOT_LoadImages+0x91e>
30006544:	990b      	ldr	r1, [sp, #44]	; 0x2c
30006546:	48a5      	ldr	r0, [pc, #660]	; (300067dc <BOOT_LoadImages+0xb04>)
30006548:	f005 fc72 	bl	3000be30 <__DiagPrintf_veneer>
3000654c:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30006550:	2002      	movs	r0, #2
30006552:	f7fe fd33 	bl	30004fbc <FLASH_Erase_With_Lock>
30006556:	e6ee      	b.n	30006336 <BOOT_LoadImages+0x65e>
30006558:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000655c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006560:	3201      	adds	r2, #1
30006562:	d048      	beq.n	300065f6 <BOOT_LoadImages+0x91e>
30006564:	4a9e      	ldr	r2, [pc, #632]	; (300067e0 <BOOT_LoadImages+0xb08>)
30006566:	6892      	ldr	r2, [r2, #8]
30006568:	0791      	lsls	r1, r2, #30
3000656a:	d448      	bmi.n	300065fe <BOOT_LoadImages+0x926>
3000656c:	f088 0701 	eor.w	r7, r8, #1
30006570:	9605      	str	r6, [sp, #20]
30006572:	2601      	movs	r6, #1
30006574:	f007 0701 	and.w	r7, r7, #1
30006578:	46b1      	mov	r9, r6
3000657a:	e72d      	b.n	300063d8 <BOOT_LoadImages+0x700>
3000657c:	4899      	ldr	r0, [pc, #612]	; (300067e4 <BOOT_LoadImages+0xb0c>)
3000657e:	f005 fc57 	bl	3000be30 <__DiagPrintf_veneer>
30006582:	e6d8      	b.n	30006336 <BOOT_LoadImages+0x65e>
30006584:	4898      	ldr	r0, [pc, #608]	; (300067e8 <BOOT_LoadImages+0xb10>)
30006586:	f005 fc53 	bl	3000be30 <__DiagPrintf_veneer>
3000658a:	e4e9      	b.n	30005f60 <BOOT_LoadImages+0x288>
3000658c:	4895      	ldr	r0, [pc, #596]	; (300067e4 <BOOT_LoadImages+0xb0c>)
3000658e:	f005 fc4f 	bl	3000be30 <__DiagPrintf_veneer>
30006592:	e4e5      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006594:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006598:	4894      	ldr	r0, [pc, #592]	; (300067ec <BOOT_LoadImages+0xb14>)
3000659a:	f005 fc49 	bl	3000be30 <__DiagPrintf_veneer>
3000659e:	2302      	movs	r3, #2
300065a0:	9303      	str	r3, [sp, #12]
300065a2:	e518      	b.n	30005fd6 <BOOT_LoadImages+0x2fe>
300065a4:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065a8:	4891      	ldr	r0, [pc, #580]	; (300067f0 <BOOT_LoadImages+0xb18>)
300065aa:	f005 fc41 	bl	3000be30 <__DiagPrintf_veneer>
300065ae:	2301      	movs	r3, #1
300065b0:	9303      	str	r3, [sp, #12]
300065b2:	e510      	b.n	30005fd6 <BOOT_LoadImages+0x2fe>
300065b4:	4620      	mov	r0, r4
300065b6:	e5ff      	b.n	300061b8 <BOOT_LoadImages+0x4e0>
300065b8:	45e3      	cmp	fp, ip
300065ba:	f67f ac1d 	bls.w	30005df8 <BOOT_LoadImages+0x120>
300065be:	e462      	b.n	30005e86 <BOOT_LoadImages+0x1ae>
300065c0:	428a      	cmp	r2, r1
300065c2:	f4ff adb5 	bcc.w	30006130 <BOOT_LoadImages+0x458>
300065c6:	4282      	cmp	r2, r0
300065c8:	f4ff adb2 	bcc.w	30006130 <BOOT_LoadImages+0x458>
300065cc:	2701      	movs	r7, #1
300065ce:	4643      	mov	r3, r8
300065d0:	9a07      	ldr	r2, [sp, #28]
300065d2:	4651      	mov	r1, sl
300065d4:	4638      	mov	r0, r7
300065d6:	9500      	str	r5, [sp, #0]
300065d8:	9701      	str	r7, [sp, #4]
300065da:	f7fe fde7 	bl	300051ac <rewrite_bp>
300065de:	f04f 0900 	mov.w	r9, #0
300065e2:	b2c3      	uxtb	r3, r0
300065e4:	f8df b228 	ldr.w	fp, [pc, #552]	; 30006810 <BOOT_LoadImages+0xb38>
300065e8:	46c8      	mov	r8, r9
300065ea:	9303      	str	r3, [sp, #12]
300065ec:	4b79      	ldr	r3, [pc, #484]	; (300067d4 <BOOT_LoadImages+0xafc>)
300065ee:	9304      	str	r3, [sp, #16]
300065f0:	4b79      	ldr	r3, [pc, #484]	; (300067d8 <BOOT_LoadImages+0xb00>)
300065f2:	9306      	str	r3, [sp, #24]
300065f4:	e4be      	b.n	30005f74 <BOOT_LoadImages+0x29c>
300065f6:	487c      	ldr	r0, [pc, #496]	; (300067e8 <BOOT_LoadImages+0xb10>)
300065f8:	f005 fc1a 	bl	3000be30 <__DiagPrintf_veneer>
300065fc:	e69b      	b.n	30006336 <BOOT_LoadImages+0x65e>
300065fe:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006602:	487b      	ldr	r0, [pc, #492]	; (300067f0 <BOOT_LoadImages+0xb18>)
30006604:	f005 fc14 	bl	3000be30 <__DiagPrintf_veneer>
30006608:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000660c:	e7ae      	b.n	3000656c <BOOT_LoadImages+0x894>
3000660e:	f8cd 8014 	str.w	r8, [sp, #20]
30006612:	4631      	mov	r1, r6
30006614:	46b0      	mov	r8, r6
30006616:	f88d 6064 	strb.w	r6, [sp, #100]	; 0x64
3000661a:	e4fb      	b.n	30006014 <BOOT_LoadImages+0x33c>
3000661c:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006620:	4870      	ldr	r0, [pc, #448]	; (300067e4 <BOOT_LoadImages+0xb0c>)
30006622:	f005 fc05 	bl	3000be30 <__DiagPrintf_veneer>
30006626:	2280      	movs	r2, #128	; 0x80
30006628:	21ff      	movs	r1, #255	; 0xff
3000662a:	4650      	mov	r0, sl
3000662c:	f005 fbe0 	bl	3000bdf0 <____wrap_memset_veneer>
30006630:	2280      	movs	r2, #128	; 0x80
30006632:	4629      	mov	r1, r5
30006634:	4650      	mov	r0, sl
30006636:	f005 fbe3 	bl	3000be00 <____wrap_memcpy_veneer>
3000663a:	f1b8 0f00 	cmp.w	r8, #0
3000663e:	d13c      	bne.n	300066ba <BOOT_LoadImages+0x9e2>
30006640:	46c1      	mov	r9, r8
30006642:	f04f 0801 	mov.w	r8, #1
30006646:	e6e3      	b.n	30006410 <BOOT_LoadImages+0x738>
30006648:	461a      	mov	r2, r3
3000664a:	4619      	mov	r1, r3
3000664c:	4869      	ldr	r0, [pc, #420]	; (300067f4 <BOOT_LoadImages+0xb1c>)
3000664e:	f005 fbef 	bl	3000be30 <__DiagPrintf_veneer>
30006652:	4b63      	ldr	r3, [pc, #396]	; (300067e0 <BOOT_LoadImages+0xb08>)
30006654:	689b      	ldr	r3, [r3, #8]
30006656:	0798      	lsls	r0, r3, #30
30006658:	d452      	bmi.n	30006700 <BOOT_LoadImages+0xa28>
3000665a:	2302      	movs	r3, #2
3000665c:	f04f 0901 	mov.w	r9, #1
30006660:	9303      	str	r3, [sp, #12]
30006662:	e47d      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006664:	461a      	mov	r2, r3
30006666:	4619      	mov	r1, r3
30006668:	4863      	ldr	r0, [pc, #396]	; (300067f8 <BOOT_LoadImages+0xb20>)
3000666a:	f005 fbe1 	bl	3000be30 <__DiagPrintf_veneer>
3000666e:	4b5c      	ldr	r3, [pc, #368]	; (300067e0 <BOOT_LoadImages+0xb08>)
30006670:	689b      	ldr	r3, [r3, #8]
30006672:	079b      	lsls	r3, r3, #30
30006674:	d452      	bmi.n	3000671c <BOOT_LoadImages+0xa44>
30006676:	2301      	movs	r3, #1
30006678:	4699      	mov	r9, r3
3000667a:	9303      	str	r3, [sp, #12]
3000667c:	e470      	b.n	30005f60 <BOOT_LoadImages+0x288>
3000667e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006682:	485a      	ldr	r0, [pc, #360]	; (300067ec <BOOT_LoadImages+0xb14>)
30006684:	f005 fbd4 	bl	3000be30 <__DiagPrintf_veneer>
30006688:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000668c:	e500      	b.n	30006090 <BOOT_LoadImages+0x3b8>
3000668e:	461a      	mov	r2, r3
30006690:	4619      	mov	r1, r3
30006692:	4858      	ldr	r0, [pc, #352]	; (300067f4 <BOOT_LoadImages+0xb1c>)
30006694:	f005 fbcc 	bl	3000be30 <__DiagPrintf_veneer>
30006698:	4b51      	ldr	r3, [pc, #324]	; (300067e0 <BOOT_LoadImages+0xb08>)
3000669a:	689a      	ldr	r2, [r3, #8]
3000669c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300066a0:	f012 0202 	ands.w	r2, r2, #2
300066a4:	d150      	bne.n	30006748 <BOOT_LoadImages+0xa70>
300066a6:	9205      	str	r2, [sp, #20]
300066a8:	e4f2      	b.n	30006090 <BOOT_LoadImages+0x3b8>
300066aa:	4854      	ldr	r0, [pc, #336]	; (300067fc <BOOT_LoadImages+0xb24>)
300066ac:	f005 fbc0 	bl	3000be30 <__DiagPrintf_veneer>
300066b0:	e7fe      	b.n	300066b0 <BOOT_LoadImages+0x9d8>
300066b2:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300066b4:	e5f7      	b.n	300062a6 <BOOT_LoadImages+0x5ce>
300066b6:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300066b8:	e43a      	b.n	30005f30 <BOOT_LoadImages+0x258>
300066ba:	f04f 0900 	mov.w	r9, #0
300066be:	e49f      	b.n	30006000 <BOOT_LoadImages+0x328>
300066c0:	f10a 3aff 	add.w	sl, sl, #4294967295
300066c4:	f11a 0f03 	cmn.w	sl, #3
300066c8:	d847      	bhi.n	3000675a <BOOT_LoadImages+0xa82>
300066ca:	f1bb 0f01 	cmp.w	fp, #1
300066ce:	d960      	bls.n	30006792 <BOOT_LoadImages+0xaba>
300066d0:	484b      	ldr	r0, [pc, #300]	; (30006800 <BOOT_LoadImages+0xb28>)
300066d2:	e642      	b.n	3000635a <BOOT_LoadImages+0x682>
300066d4:	461a      	mov	r2, r3
300066d6:	4619      	mov	r1, r3
300066d8:	4847      	ldr	r0, [pc, #284]	; (300067f8 <BOOT_LoadImages+0xb20>)
300066da:	f005 fba9 	bl	3000be30 <__DiagPrintf_veneer>
300066de:	4b40      	ldr	r3, [pc, #256]	; (300067e0 <BOOT_LoadImages+0xb08>)
300066e0:	689e      	ldr	r6, [r3, #8]
300066e2:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066e6:	f016 0602 	ands.w	r6, r6, #2
300066ea:	f43f af3f 	beq.w	3000656c <BOOT_LoadImages+0x894>
300066ee:	f8d3 1006 	ldr.w	r1, [r3, #6]
300066f2:	2600      	movs	r6, #0
300066f4:	483e      	ldr	r0, [pc, #248]	; (300067f0 <BOOT_LoadImages+0xb18>)
300066f6:	f005 fb9b 	bl	3000be30 <__DiagPrintf_veneer>
300066fa:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066fe:	e735      	b.n	3000656c <BOOT_LoadImages+0x894>
30006700:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006704:	f04f 0901 	mov.w	r9, #1
30006708:	4838      	ldr	r0, [pc, #224]	; (300067ec <BOOT_LoadImages+0xb14>)
3000670a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000670e:	f005 fb8f 	bl	3000be30 <__DiagPrintf_veneer>
30006712:	2302      	movs	r3, #2
30006714:	9303      	str	r3, [sp, #12]
30006716:	e423      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006718:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000671a:	e5fa      	b.n	30006312 <BOOT_LoadImages+0x63a>
3000671c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006720:	4833      	ldr	r0, [pc, #204]	; (300067f0 <BOOT_LoadImages+0xb18>)
30006722:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006726:	f005 fb83 	bl	3000be30 <__DiagPrintf_veneer>
3000672a:	2301      	movs	r3, #1
3000672c:	4699      	mov	r9, r3
3000672e:	9303      	str	r3, [sp, #12]
30006730:	e416      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006732:	4281      	cmp	r1, r0
30006734:	f4ff ad02 	bcc.w	3000613c <BOOT_LoadImages+0x464>
30006738:	2400      	movs	r4, #0
3000673a:	4643      	mov	r3, r8
3000673c:	464a      	mov	r2, r9
3000673e:	4659      	mov	r1, fp
30006740:	4620      	mov	r0, r4
30006742:	9500      	str	r5, [sp, #0]
30006744:	9401      	str	r4, [sp, #4]
30006746:	e537      	b.n	300061b8 <BOOT_LoadImages+0x4e0>
30006748:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000674c:	4827      	ldr	r0, [pc, #156]	; (300067ec <BOOT_LoadImages+0xb14>)
3000674e:	f005 fb6f 	bl	3000be30 <__DiagPrintf_veneer>
30006752:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006756:	2200      	movs	r2, #0
30006758:	e7a5      	b.n	300066a6 <BOOT_LoadImages+0x9ce>
3000675a:	482a      	ldr	r0, [pc, #168]	; (30006804 <BOOT_LoadImages+0xb2c>)
3000675c:	e5fd      	b.n	3000635a <BOOT_LoadImages+0x682>
3000675e:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30006760:	e6e8      	b.n	30006534 <BOOT_LoadImages+0x85c>
30006762:	2400      	movs	r4, #0
30006764:	4643      	mov	r3, r8
30006766:	464a      	mov	r2, r9
30006768:	4659      	mov	r1, fp
3000676a:	9500      	str	r5, [sp, #0]
3000676c:	9401      	str	r4, [sp, #4]
3000676e:	e522      	b.n	300061b6 <BOOT_LoadImages+0x4de>
30006770:	4c25      	ldr	r4, [pc, #148]	; (30006808 <BOOT_LoadImages+0xb30>)
30006772:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006776:	e491      	b.n	3000609c <BOOT_LoadImages+0x3c4>
30006778:	2701      	movs	r7, #1
3000677a:	4643      	mov	r3, r8
3000677c:	9a07      	ldr	r2, [sp, #28]
3000677e:	4651      	mov	r1, sl
30006780:	2000      	movs	r0, #0
30006782:	9500      	str	r5, [sp, #0]
30006784:	9701      	str	r7, [sp, #4]
30006786:	e728      	b.n	300065da <BOOT_LoadImages+0x902>
30006788:	2701      	movs	r7, #1
3000678a:	4c1f      	ldr	r4, [pc, #124]	; (30006808 <BOOT_LoadImages+0xb30>)
3000678c:	46b8      	mov	r8, r7
3000678e:	f7ff bbb5 	b.w	30005efc <BOOT_LoadImages+0x224>
30006792:	9904      	ldr	r1, [sp, #16]
30006794:	481d      	ldr	r0, [pc, #116]	; (3000680c <BOOT_LoadImages+0xb34>)
30006796:	f005 fb4b 	bl	3000be30 <__DiagPrintf_veneer>
3000679a:	f8cd 9024 	str.w	r9, [sp, #36]	; 0x24
3000679e:	f8d8 3011 	ldr.w	r3, [r8, #17]
300067a2:	f8d8 a00d 	ldr.w	sl, [r8, #13]
300067a6:	9307      	str	r3, [sp, #28]
300067a8:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300067ac:	f8d5 9011 	ldr.w	r9, [r5, #17]
300067b0:	e5df      	b.n	30006372 <BOOT_LoadImages+0x69a>
300067b2:	f04f 0800 	mov.w	r8, #0
300067b6:	e423      	b.n	30006000 <BOOT_LoadImages+0x328>
300067b8:	f088 0701 	eor.w	r7, r8, #1
300067bc:	f007 0701 	and.w	r7, r7, #1
300067c0:	f7ff bbd8 	b.w	30005f74 <BOOT_LoadImages+0x29c>
300067c4:	f088 0701 	eor.w	r7, r8, #1
300067c8:	f8cd 9014 	str.w	r9, [sp, #20]
300067cc:	f007 0701 	and.w	r7, r7, #1
300067d0:	f7ff bbd0 	b.w	30005f74 <BOOT_LoadImages+0x29c>
300067d4:	300043b9 	.word	0x300043b9
300067d8:	3000d296 	.word	0x3000d296
300067dc:	3000c7e8 	.word	0x3000c7e8
300067e0:	2001c00c 	.word	0x2001c00c
300067e4:	3000c79c 	.word	0x3000c79c
300067e8:	3000c78c 	.word	0x3000c78c
300067ec:	3000c4c4 	.word	0x3000c4c4
300067f0:	3000c494 	.word	0x3000c494
300067f4:	3000c834 	.word	0x3000c834
300067f8:	3000c7ac 	.word	0x3000c7ac
300067fc:	3000c8f4 	.word	0x3000c8f4
30006800:	3000c954 	.word	0x3000c954
30006804:	3000c5dc 	.word	0x3000c5dc
30006808:	3000d828 	.word	0x3000d828
3000680c:	3000c98c 	.word	0x3000c98c
30006810:	3000e920 	.word	0x3000e920

30006814 <BOOT_ReasonSet>:
30006814:	4a03      	ldr	r2, [pc, #12]	; (30006824 <BOOT_ReasonSet+0x10>)
30006816:	4904      	ldr	r1, [pc, #16]	; (30006828 <BOOT_ReasonSet+0x14>)
30006818:	8c13      	ldrh	r3, [r2, #32]
3000681a:	b29b      	uxth	r3, r3
3000681c:	8413      	strh	r3, [r2, #32]
3000681e:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006822:	4770      	bx	lr
30006824:	42008000 	.word	0x42008000
30006828:	42008200 	.word	0x42008200

3000682c <BOOT_Enable_KM0>:
3000682c:	b510      	push	{r4, lr}
3000682e:	4c13      	ldr	r4, [pc, #76]	; (3000687c <BOOT_Enable_KM0+0x50>)
30006830:	47a0      	blx	r4
30006832:	b110      	cbz	r0, 3000683a <BOOT_Enable_KM0+0xe>
30006834:	4b12      	ldr	r3, [pc, #72]	; (30006880 <BOOT_Enable_KM0+0x54>)
30006836:	4798      	blx	r3
30006838:	b1b0      	cbz	r0, 30006868 <BOOT_Enable_KM0+0x3c>
3000683a:	47a0      	blx	r4
3000683c:	b940      	cbnz	r0, 30006850 <BOOT_Enable_KM0+0x24>
3000683e:	4a11      	ldr	r2, [pc, #68]	; (30006884 <BOOT_Enable_KM0+0x58>)
30006840:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
30006844:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
30006848:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
3000684c:	bf40      	sev
3000684e:	bd10      	pop	{r4, pc}
30006850:	4b0b      	ldr	r3, [pc, #44]	; (30006880 <BOOT_Enable_KM0+0x54>)
30006852:	4798      	blx	r3
30006854:	2800      	cmp	r0, #0
30006856:	d0f2      	beq.n	3000683e <BOOT_Enable_KM0+0x12>
30006858:	4b0b      	ldr	r3, [pc, #44]	; (30006888 <BOOT_Enable_KM0+0x5c>)
3000685a:	689b      	ldr	r3, [r3, #8]
3000685c:	079b      	lsls	r3, r3, #30
3000685e:	d5ee      	bpl.n	3000683e <BOOT_Enable_KM0+0x12>
30006860:	480a      	ldr	r0, [pc, #40]	; (3000688c <BOOT_Enable_KM0+0x60>)
30006862:	f005 fae5 	bl	3000be30 <__DiagPrintf_veneer>
30006866:	e7ea      	b.n	3000683e <BOOT_Enable_KM0+0x12>
30006868:	4b07      	ldr	r3, [pc, #28]	; (30006888 <BOOT_Enable_KM0+0x5c>)
3000686a:	689b      	ldr	r3, [r3, #8]
3000686c:	079a      	lsls	r2, r3, #30
3000686e:	d5ee      	bpl.n	3000684e <BOOT_Enable_KM0+0x22>
30006870:	4807      	ldr	r0, [pc, #28]	; (30006890 <BOOT_Enable_KM0+0x64>)
30006872:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006876:	f005 badb 	b.w	3000be30 <__DiagPrintf_veneer>
3000687a:	bf00      	nop
3000687c:	0000c09d 	.word	0x0000c09d
30006880:	0000c0ad 	.word	0x0000c0ad
30006884:	42008000 	.word	0x42008000
30006888:	2001c00c 	.word	0x2001c00c
3000688c:	3000c9e4 	.word	0x3000c9e4
30006890:	3000c9a8 	.word	0x3000c9a8

30006894 <BOOT_AP_Clk_Get>:
30006894:	4a06      	ldr	r2, [pc, #24]	; (300068b0 <BOOT_AP_Clk_Get+0x1c>)
30006896:	6913      	ldr	r3, [r2, #16]
30006898:	2b00      	cmp	r3, #0
3000689a:	db03      	blt.n	300068a4 <BOOT_AP_Clk_Get+0x10>
3000689c:	6810      	ldr	r0, [r2, #0]
3000689e:	fbb0 f0f3 	udiv	r0, r0, r3
300068a2:	4770      	bx	lr
300068a4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300068a8:	6850      	ldr	r0, [r2, #4]
300068aa:	fbb0 f0f3 	udiv	r0, r0, r3
300068ae:	4770      	bx	lr
300068b0:	3000deac 	.word	0x3000deac

300068b4 <BOOT_SOC_ClkSet>:
300068b4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300068b8:	4f5b      	ldr	r7, [pc, #364]	; (30006a28 <BOOT_SOC_ClkSet+0x174>)
300068ba:	693d      	ldr	r5, [r7, #16]
300068bc:	683e      	ldr	r6, [r7, #0]
300068be:	2d00      	cmp	r5, #0
300068c0:	db74      	blt.n	300069ac <BOOT_SOC_ClkSet+0xf8>
300068c2:	fbb6 f5f5 	udiv	r5, r6, r5
300068c6:	4b59      	ldr	r3, [pc, #356]	; (30006a2c <BOOT_SOC_ClkSet+0x178>)
300068c8:	f8d7 a008 	ldr.w	sl, [r7, #8]
300068cc:	fba3 1306 	umull	r1, r3, r3, r6
300068d0:	68fa      	ldr	r2, [r7, #12]
300068d2:	f1ba 0f01 	cmp.w	sl, #1
300068d6:	ea4f 19d3 	mov.w	r9, r3, lsr #7
300068da:	ea4f 1853 	mov.w	r8, r3, lsr #5
300068de:	ea4f 1393 	mov.w	r3, r3, lsr #6
300068e2:	f109 39ff 	add.w	r9, r9, #4294967295
300068e6:	f108 38ff 	add.w	r8, r8, #4294967295
300068ea:	f103 34ff 	add.w	r4, r3, #4294967295
300068ee:	fbb6 f6f2 	udiv	r6, r6, r2
300068f2:	d072      	beq.n	300069da <BOOT_SOC_ClkSet+0x126>
300068f4:	4b4e      	ldr	r3, [pc, #312]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
300068f6:	2200      	movs	r2, #0
300068f8:	f5b6 7f8c 	cmp.w	r6, #280	; 0x118
300068fc:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30006900:	d865      	bhi.n	300069ce <BOOT_SOC_ClkSet+0x11a>
30006902:	f5b5 6f87 	cmp.w	r5, #1080	; 0x438
30006906:	d85c      	bhi.n	300069c2 <BOOT_SOC_ClkSet+0x10e>
30006908:	2001      	movs	r0, #1
3000690a:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000690e:	f001 faed 	bl	30007eec <CLK_SWITCH_XTAL>
30006912:	683b      	ldr	r3, [r7, #0]
30006914:	4847      	ldr	r0, [pc, #284]	; (30006a34 <BOOT_SOC_ClkSet+0x180>)
30006916:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000691a:	f409 69e0 	and.w	r9, r9, #1792	; 0x700
3000691e:	0524      	lsls	r4, r4, #20
30006920:	fb03 f000 	mul.w	r0, r3, r0
30006924:	4b44      	ldr	r3, [pc, #272]	; (30006a38 <BOOT_SOC_ClkSet+0x184>)
30006926:	4798      	blx	r3
30006928:	4944      	ldr	r1, [pc, #272]	; (30006a3c <BOOT_SOC_ClkSet+0x188>)
3000692a:	68fb      	ldr	r3, [r7, #12]
3000692c:	fa1f f888 	uxth.w	r8, r8
30006930:	f8d1 0228 	ldr.w	r0, [r1, #552]	; 0x228
30006934:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
30006938:	3b01      	subs	r3, #1
3000693a:	4a41      	ldr	r2, [pc, #260]	; (30006a40 <BOOT_SOC_ClkSet+0x18c>)
3000693c:	f003 0307 	and.w	r3, r3, #7
30006940:	4002      	ands	r2, r0
30006942:	2000      	movs	r0, #0
30006944:	4313      	orrs	r3, r2
30006946:	ea49 0303 	orr.w	r3, r9, r3
3000694a:	ea48 0303 	orr.w	r3, r8, r3
3000694e:	4323      	orrs	r3, r4
30006950:	4c3c      	ldr	r4, [pc, #240]	; (30006a44 <BOOT_SOC_ClkSet+0x190>)
30006952:	f8c1 3228 	str.w	r3, [r1, #552]	; 0x228
30006956:	f001 fac9 	bl	30007eec <CLK_SWITCH_XTAL>
3000695a:	68a3      	ldr	r3, [r4, #8]
3000695c:	079a      	lsls	r2, r3, #30
3000695e:	d42b      	bmi.n	300069b8 <BOOT_SOC_ClkSet+0x104>
30006960:	4b39      	ldr	r3, [pc, #228]	; (30006a48 <BOOT_SOC_ClkSet+0x194>)
30006962:	781b      	ldrb	r3, [r3, #0]
30006964:	b303      	cbz	r3, 300069a8 <BOOT_SOC_ClkSet+0xf4>
30006966:	693b      	ldr	r3, [r7, #16]
30006968:	2b00      	cmp	r3, #0
3000696a:	db48      	blt.n	300069fe <BOOT_SOC_ClkSet+0x14a>
3000696c:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30006970:	6913      	ldr	r3, [r2, #16]
30006972:	f043 030c 	orr.w	r3, r3, #12
30006976:	6113      	str	r3, [r2, #16]
30006978:	2200      	movs	r2, #0
3000697a:	4b2d      	ldr	r3, [pc, #180]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
3000697c:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006980:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30006984:	693b      	ldr	r3, [r7, #16]
30006986:	68a0      	ldr	r0, [r4, #8]
30006988:	690a      	ldr	r2, [r1, #16]
3000698a:	3b01      	subs	r3, #1
3000698c:	f022 0203 	bic.w	r2, r2, #3
30006990:	f003 0303 	and.w	r3, r3, #3
30006994:	4313      	orrs	r3, r2
30006996:	610b      	str	r3, [r1, #16]
30006998:	0783      	lsls	r3, r0, #30
3000699a:	d505      	bpl.n	300069a8 <BOOT_SOC_ClkSet+0xf4>
3000699c:	4629      	mov	r1, r5
3000699e:	482b      	ldr	r0, [pc, #172]	; (30006a4c <BOOT_SOC_ClkSet+0x198>)
300069a0:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300069a4:	f005 ba44 	b.w	3000be30 <__DiagPrintf_veneer>
300069a8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300069ac:	f025 4500 	bic.w	r5, r5, #2147483648	; 0x80000000
300069b0:	687b      	ldr	r3, [r7, #4]
300069b2:	fbb3 f5f5 	udiv	r5, r3, r5
300069b6:	e786      	b.n	300068c6 <BOOT_SOC_ClkSet+0x12>
300069b8:	4631      	mov	r1, r6
300069ba:	4825      	ldr	r0, [pc, #148]	; (30006a50 <BOOT_SOC_ClkSet+0x19c>)
300069bc:	f005 fa38 	bl	3000be30 <__DiagPrintf_veneer>
300069c0:	e7ce      	b.n	30006960 <BOOT_SOC_ClkSet+0xac>
300069c2:	f44f 61cc 	mov.w	r1, #1632	; 0x660
300069c6:	4823      	ldr	r0, [pc, #140]	; (30006a54 <BOOT_SOC_ClkSet+0x1a0>)
300069c8:	f005 fa22 	bl	3000be10 <__io_assert_failed_veneer>
300069cc:	e79c      	b.n	30006908 <BOOT_SOC_ClkSet+0x54>
300069ce:	f240 615f 	movw	r1, #1631	; 0x65f
300069d2:	4820      	ldr	r0, [pc, #128]	; (30006a54 <BOOT_SOC_ClkSet+0x1a0>)
300069d4:	f005 fa1c 	bl	3000be10 <__io_assert_failed_veneer>
300069d8:	e793      	b.n	30006902 <BOOT_SOC_ClkSet+0x4e>
300069da:	4b1f      	ldr	r3, [pc, #124]	; (30006a58 <BOOT_SOC_ClkSet+0x1a4>)
300069dc:	4650      	mov	r0, sl
300069de:	4798      	blx	r3
300069e0:	4b13      	ldr	r3, [pc, #76]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
300069e2:	f5b6 7fb4 	cmp.w	r6, #360	; 0x168
300069e6:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
300069ea:	d816      	bhi.n	30006a1a <BOOT_SOC_ClkSet+0x166>
300069ec:	f5b5 6fb9 	cmp.w	r5, #1480	; 0x5c8
300069f0:	d98a      	bls.n	30006908 <BOOT_SOC_ClkSet+0x54>
300069f2:	f240 615c 	movw	r1, #1628	; 0x65c
300069f6:	4817      	ldr	r0, [pc, #92]	; (30006a54 <BOOT_SOC_ClkSet+0x1a0>)
300069f8:	f005 fa0a 	bl	3000be10 <__io_assert_failed_veneer>
300069fc:	e784      	b.n	30006908 <BOOT_SOC_ClkSet+0x54>
300069fe:	687b      	ldr	r3, [r7, #4]
30006a00:	480c      	ldr	r0, [pc, #48]	; (30006a34 <BOOT_SOC_ClkSet+0x180>)
30006a02:	fb03 f000 	mul.w	r0, r3, r0
30006a06:	4b15      	ldr	r3, [pc, #84]	; (30006a5c <BOOT_SOC_ClkSet+0x1a8>)
30006a08:	4798      	blx	r3
30006a0a:	4b15      	ldr	r3, [pc, #84]	; (30006a60 <BOOT_SOC_ClkSet+0x1ac>)
30006a0c:	2001      	movs	r0, #1
30006a0e:	4798      	blx	r3
30006a10:	4b07      	ldr	r3, [pc, #28]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
30006a12:	2201      	movs	r2, #1
30006a14:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006a18:	e7b2      	b.n	30006980 <BOOT_SOC_ClkSet+0xcc>
30006a1a:	f240 615b 	movw	r1, #1627	; 0x65b
30006a1e:	480d      	ldr	r0, [pc, #52]	; (30006a54 <BOOT_SOC_ClkSet+0x1a0>)
30006a20:	f005 f9f6 	bl	3000be10 <__io_assert_failed_veneer>
30006a24:	e7e2      	b.n	300069ec <BOOT_SOC_ClkSet+0x138>
30006a26:	bf00      	nop
30006a28:	3000deac 	.word	0x3000deac
30006a2c:	51eb851f 	.word	0x51eb851f
30006a30:	23020000 	.word	0x23020000
30006a34:	000f4240 	.word	0x000f4240
30006a38:	30009041 	.word	0x30009041
30006a3c:	42008000 	.word	0x42008000
30006a40:	ff8f08f8 	.word	0xff8f08f8
30006a44:	2001c00c 	.word	0x2001c00c
30006a48:	3000d296 	.word	0x3000d296
30006a4c:	3000ca40 	.word	0x3000ca40
30006a50:	3000ca14 	.word	0x3000ca14
30006a54:	3000d24c 	.word	0x3000d24c
30006a58:	3000b80d 	.word	0x3000b80d
30006a5c:	300090c1 	.word	0x300090c1
30006a60:	30009101 	.word	0x30009101

30006a64 <BOOT_Disable_AP>:
30006a64:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006a68:	4906      	ldr	r1, [pc, #24]	; (30006a84 <BOOT_Disable_AP+0x20>)
30006a6a:	685a      	ldr	r2, [r3, #4]
30006a6c:	f042 0202 	orr.w	r2, r2, #2
30006a70:	605a      	str	r2, [r3, #4]
30006a72:	685a      	ldr	r2, [r3, #4]
30006a74:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006a78:	605a      	str	r2, [r3, #4]
30006a7a:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006a7c:	f043 0303 	orr.w	r3, r3, #3
30006a80:	678b      	str	r3, [r1, #120]	; 0x78
30006a82:	4770      	bx	lr
30006a84:	41000200 	.word	0x41000200

30006a88 <BOOT_Enable_AP>:
30006a88:	b538      	push	{r3, r4, r5, lr}
30006a8a:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006a8e:	2201      	movs	r2, #1
30006a90:	2104      	movs	r1, #4
30006a92:	4b2b      	ldr	r3, [pc, #172]	; (30006b40 <BOOT_Enable_AP+0xb8>)
30006a94:	482b      	ldr	r0, [pc, #172]	; (30006b44 <BOOT_Enable_AP+0xbc>)
30006a96:	4798      	blx	r3
30006a98:	6863      	ldr	r3, [r4, #4]
30006a9a:	4d2b      	ldr	r5, [pc, #172]	; (30006b48 <BOOT_Enable_AP+0xc0>)
30006a9c:	2032      	movs	r0, #50	; 0x32
30006a9e:	f043 0302 	orr.w	r3, r3, #2
30006aa2:	6063      	str	r3, [r4, #4]
30006aa4:	6863      	ldr	r3, [r4, #4]
30006aa6:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30006aaa:	6063      	str	r3, [r4, #4]
30006aac:	6823      	ldr	r3, [r4, #0]
30006aae:	f043 0302 	orr.w	r3, r3, #2
30006ab2:	6023      	str	r3, [r4, #0]
30006ab4:	47a8      	blx	r5
30006ab6:	6823      	ldr	r3, [r4, #0]
30006ab8:	2032      	movs	r0, #50	; 0x32
30006aba:	f043 0304 	orr.w	r3, r3, #4
30006abe:	6023      	str	r3, [r4, #0]
30006ac0:	6823      	ldr	r3, [r4, #0]
30006ac2:	f043 0301 	orr.w	r3, r3, #1
30006ac6:	6023      	str	r3, [r4, #0]
30006ac8:	6823      	ldr	r3, [r4, #0]
30006aca:	f043 0310 	orr.w	r3, r3, #16
30006ace:	6023      	str	r3, [r4, #0]
30006ad0:	47a8      	blx	r5
30006ad2:	6823      	ldr	r3, [r4, #0]
30006ad4:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30006ad8:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006adc:	6023      	str	r3, [r4, #0]
30006ade:	47a8      	blx	r5
30006ae0:	6863      	ldr	r3, [r4, #4]
30006ae2:	4a1a      	ldr	r2, [pc, #104]	; (30006b4c <BOOT_Enable_AP+0xc4>)
30006ae4:	f023 0310 	bic.w	r3, r3, #16
30006ae8:	6063      	str	r3, [r4, #4]
30006aea:	6863      	ldr	r3, [r4, #4]
30006aec:	f023 0302 	bic.w	r3, r3, #2
30006af0:	6063      	str	r3, [r4, #4]
30006af2:	4790      	blx	r2
30006af4:	b178      	cbz	r0, 30006b16 <BOOT_Enable_AP+0x8e>
30006af6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006afa:	4815      	ldr	r0, [pc, #84]	; (30006b50 <BOOT_Enable_AP+0xc8>)
30006afc:	4a15      	ldr	r2, [pc, #84]	; (30006b54 <BOOT_Enable_AP+0xcc>)
30006afe:	68d9      	ldr	r1, [r3, #12]
30006b00:	f041 0101 	orr.w	r1, r1, #1
30006b04:	60d9      	str	r1, [r3, #12]
30006b06:	6899      	ldr	r1, [r3, #8]
30006b08:	f041 0101 	orr.w	r1, r1, #1
30006b0c:	6099      	str	r1, [r3, #8]
30006b0e:	6843      	ldr	r3, [r0, #4]
30006b10:	431a      	orrs	r2, r3
30006b12:	6042      	str	r2, [r0, #4]
30006b14:	bd38      	pop	{r3, r4, r5, pc}
30006b16:	4604      	mov	r4, r0
30006b18:	4d0f      	ldr	r5, [pc, #60]	; (30006b58 <BOOT_Enable_AP+0xd0>)
30006b1a:	4601      	mov	r1, r0
30006b1c:	2041      	movs	r0, #65	; 0x41
30006b1e:	47a8      	blx	r5
30006b20:	4621      	mov	r1, r4
30006b22:	2042      	movs	r0, #66	; 0x42
30006b24:	47a8      	blx	r5
30006b26:	4621      	mov	r1, r4
30006b28:	2043      	movs	r0, #67	; 0x43
30006b2a:	47a8      	blx	r5
30006b2c:	4621      	mov	r1, r4
30006b2e:	2044      	movs	r0, #68	; 0x44
30006b30:	47a8      	blx	r5
30006b32:	4621      	mov	r1, r4
30006b34:	2045      	movs	r0, #69	; 0x45
30006b36:	47a8      	blx	r5
30006b38:	4621      	mov	r1, r4
30006b3a:	2046      	movs	r0, #70	; 0x46
30006b3c:	47a8      	blx	r5
30006b3e:	e7da      	b.n	30006af6 <BOOT_Enable_AP+0x6e>
30006b40:	0000aab9 	.word	0x0000aab9
30006b44:	4200c000 	.word	0x4200c000
30006b48:	00009b2d 	.word	0x00009b2d
30006b4c:	3000a191 	.word	0x3000a191
30006b50:	41000200 	.word	0x41000200
30006b54:	01001111 	.word	0x01001111
30006b58:	0000b20d 	.word	0x0000b20d

30006b5c <BOOT_RAM_KeyDeriveFunc>:
30006b5c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006b60:	ed2d 8b04 	vpush	{d8-d9}
30006b64:	b0cb      	sub	sp, #300	; 0x12c
30006b66:	4c6a      	ldr	r4, [pc, #424]	; (30006d10 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006b68:	4680      	mov	r8, r0
30006b6a:	4689      	mov	r9, r1
30006b6c:	4869      	ldr	r0, [pc, #420]	; (30006d14 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006b6e:	ee08 2a90 	vmov	s17, r2
30006b72:	4969      	ldr	r1, [pc, #420]	; (30006d18 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006b74:	2201      	movs	r2, #1
30006b76:	ee09 3a10 	vmov	s18, r3
30006b7a:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006b7e:	47a0      	blx	r4
30006b80:	2201      	movs	r2, #1
30006b82:	4966      	ldr	r1, [pc, #408]	; (30006d1c <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006b84:	ae02      	add	r6, sp, #8
30006b86:	4864      	ldr	r0, [pc, #400]	; (30006d18 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006b88:	47a0      	blx	r4
30006b8a:	4b65      	ldr	r3, [pc, #404]	; (30006d20 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006b8c:	2000      	movs	r0, #0
30006b8e:	4798      	blx	r3
30006b90:	22e0      	movs	r2, #224	; 0xe0
30006b92:	2100      	movs	r1, #0
30006b94:	a812      	add	r0, sp, #72	; 0x48
30006b96:	f005 f92b 	bl	3000bdf0 <____wrap_memset_veneer>
30006b9a:	2300      	movs	r3, #0
30006b9c:	f8ad 3004 	strh.w	r3, [sp, #4]
30006ba0:	f88d 3006 	strb.w	r3, [sp, #6]
30006ba4:	2301      	movs	r3, #1
30006ba6:	f88d 3007 	strb.w	r3, [sp, #7]
30006baa:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006bac:	2b00      	cmp	r3, #0
30006bae:	f000 80ab 	beq.w	30006d08 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006bb2:	ab0a      	add	r3, sp, #40	; 0x28
30006bb4:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006d34 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006bb8:	4f5a      	ldr	r7, [pc, #360]	; (30006d24 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006bba:	ee08 3a10 	vmov	s16, r3
30006bbe:	ab12      	add	r3, sp, #72	; 0x48
30006bc0:	464a      	mov	r2, r9
30006bc2:	4641      	mov	r1, r8
30006bc4:	2020      	movs	r0, #32
30006bc6:	47d8      	blx	fp
30006bc8:	aa12      	add	r2, sp, #72	; 0x48
30006bca:	ee19 1a10 	vmov	r1, s18
30006bce:	ee18 0a90 	vmov	r0, s17
30006bd2:	47b8      	blx	r7
30006bd4:	ee18 5a10 	vmov	r5, s16
30006bd8:	aa12      	add	r2, sp, #72	; 0x48
30006bda:	2104      	movs	r1, #4
30006bdc:	a801      	add	r0, sp, #4
30006bde:	4634      	mov	r4, r6
30006be0:	47b8      	blx	r7
30006be2:	a912      	add	r1, sp, #72	; 0x48
30006be4:	4b50      	ldr	r3, [pc, #320]	; (30006d28 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006be6:	ee18 0a10 	vmov	r0, s16
30006bea:	4798      	blx	r3
30006bec:	f1ba 0f01 	cmp.w	sl, #1
30006bf0:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006bf2:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006bf4:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006bf8:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006bfc:	d91f      	bls.n	30006c3e <BOOT_RAM_KeyDeriveFunc+0xe2>
30006bfe:	2501      	movs	r5, #1
30006c00:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006c04:	ab12      	add	r3, sp, #72	; 0x48
30006c06:	4641      	mov	r1, r8
30006c08:	464a      	mov	r2, r9
30006c0a:	2020      	movs	r0, #32
30006c0c:	47d8      	blx	fp
30006c0e:	aa12      	add	r2, sp, #72	; 0x48
30006c10:	2120      	movs	r1, #32
30006c12:	4630      	mov	r0, r6
30006c14:	47b8      	blx	r7
30006c16:	a912      	add	r1, sp, #72	; 0x48
30006c18:	4b43      	ldr	r3, [pc, #268]	; (30006d28 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c1a:	4630      	mov	r0, r6
30006c1c:	4798      	blx	r3
30006c1e:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006c22:	f10d 0107 	add.w	r1, sp, #7
30006c26:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006c2a:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006c2e:	42a3      	cmp	r3, r4
30006c30:	ea82 0200 	eor.w	r2, r2, r0
30006c34:	701a      	strb	r2, [r3, #0]
30006c36:	d1f6      	bne.n	30006c26 <BOOT_RAM_KeyDeriveFunc+0xca>
30006c38:	3501      	adds	r5, #1
30006c3a:	45aa      	cmp	sl, r5
30006c3c:	d1e2      	bne.n	30006c04 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006c3e:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006c40:	ee18 1a10 	vmov	r1, s16
30006c44:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c46:	2a20      	cmp	r2, #32
30006c48:	985a      	ldr	r0, [sp, #360]	; 0x168
30006c4a:	bf28      	it	cs
30006c4c:	2220      	movcs	r2, #32
30006c4e:	1a9b      	subs	r3, r3, r2
30006c50:	9359      	str	r3, [sp, #356]	; 0x164
30006c52:	4603      	mov	r3, r0
30006c54:	4413      	add	r3, r2
30006c56:	935a      	str	r3, [sp, #360]	; 0x168
30006c58:	f005 f8d2 	bl	3000be00 <____wrap_memcpy_veneer>
30006c5c:	f10d 0207 	add.w	r2, sp, #7
30006c60:	2104      	movs	r1, #4
30006c62:	7813      	ldrb	r3, [r2, #0]
30006c64:	3901      	subs	r1, #1
30006c66:	3301      	adds	r3, #1
30006c68:	b2db      	uxtb	r3, r3
30006c6a:	f802 3901 	strb.w	r3, [r2], #-1
30006c6e:	b90b      	cbnz	r3, 30006c74 <BOOT_RAM_KeyDeriveFunc+0x118>
30006c70:	2900      	cmp	r1, #0
30006c72:	d1f6      	bne.n	30006c62 <BOOT_RAM_KeyDeriveFunc+0x106>
30006c74:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c76:	2b00      	cmp	r3, #0
30006c78:	d1a1      	bne.n	30006bbe <BOOT_RAM_KeyDeriveFunc+0x62>
30006c7a:	2220      	movs	r2, #32
30006c7c:	2100      	movs	r1, #0
30006c7e:	4c2b      	ldr	r4, [pc, #172]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006c80:	4630      	mov	r0, r6
30006c82:	47a0      	blx	r4
30006c84:	2220      	movs	r2, #32
30006c86:	2100      	movs	r1, #0
30006c88:	ee18 0a10 	vmov	r0, s16
30006c8c:	47a0      	blx	r4
30006c8e:	4b28      	ldr	r3, [pc, #160]	; (30006d30 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006c90:	695b      	ldr	r3, [r3, #20]
30006c92:	03da      	lsls	r2, r3, #15
30006c94:	d512      	bpl.n	30006cbc <BOOT_RAM_KeyDeriveFunc+0x160>
30006c96:	f016 021f 	ands.w	r2, r6, #31
30006c9a:	4633      	mov	r3, r6
30006c9c:	d130      	bne.n	30006d00 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006c9e:	2220      	movs	r2, #32
30006ca0:	f3bf 8f4f 	dsb	sy
30006ca4:	4822      	ldr	r0, [pc, #136]	; (30006d30 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006ca6:	441a      	add	r2, r3
30006ca8:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cac:	3320      	adds	r3, #32
30006cae:	1ad1      	subs	r1, r2, r3
30006cb0:	2900      	cmp	r1, #0
30006cb2:	dcf9      	bgt.n	30006ca8 <BOOT_RAM_KeyDeriveFunc+0x14c>
30006cb4:	f3bf 8f4f 	dsb	sy
30006cb8:	f3bf 8f6f 	isb	sy
30006cbc:	4b1c      	ldr	r3, [pc, #112]	; (30006d30 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cbe:	695b      	ldr	r3, [r3, #20]
30006cc0:	03db      	lsls	r3, r3, #15
30006cc2:	d513      	bpl.n	30006cec <BOOT_RAM_KeyDeriveFunc+0x190>
30006cc4:	ee18 3a10 	vmov	r3, s16
30006cc8:	f013 021f 	ands.w	r2, r3, #31
30006ccc:	d114      	bne.n	30006cf8 <BOOT_RAM_KeyDeriveFunc+0x19c>
30006cce:	2220      	movs	r2, #32
30006cd0:	f3bf 8f4f 	dsb	sy
30006cd4:	4816      	ldr	r0, [pc, #88]	; (30006d30 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cd6:	441a      	add	r2, r3
30006cd8:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cdc:	3320      	adds	r3, #32
30006cde:	1ad1      	subs	r1, r2, r3
30006ce0:	2900      	cmp	r1, #0
30006ce2:	dcf9      	bgt.n	30006cd8 <BOOT_RAM_KeyDeriveFunc+0x17c>
30006ce4:	f3bf 8f4f 	dsb	sy
30006ce8:	f3bf 8f6f 	isb	sy
30006cec:	2000      	movs	r0, #0
30006cee:	b04b      	add	sp, #300	; 0x12c
30006cf0:	ecbd 8b04 	vpop	{d8-d9}
30006cf4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006cf8:	f023 031f 	bic.w	r3, r3, #31
30006cfc:	3220      	adds	r2, #32
30006cfe:	e7e7      	b.n	30006cd0 <BOOT_RAM_KeyDeriveFunc+0x174>
30006d00:	f026 031f 	bic.w	r3, r6, #31
30006d04:	3220      	adds	r2, #32
30006d06:	e7cb      	b.n	30006ca0 <BOOT_RAM_KeyDeriveFunc+0x144>
30006d08:	ab0a      	add	r3, sp, #40	; 0x28
30006d0a:	ee08 3a10 	vmov	s16, r3
30006d0e:	e7b4      	b.n	30006c7a <BOOT_RAM_KeyDeriveFunc+0x11e>
30006d10:	0000b479 	.word	0x0000b479
30006d14:	40000008 	.word	0x40000008
30006d18:	40000004 	.word	0x40000004
30006d1c:	40000002 	.word	0x40000002
30006d20:	00002451 	.word	0x00002451
30006d24:	00005b4d 	.word	0x00005b4d
30006d28:	00005bb9 	.word	0x00005bb9
30006d2c:	00012be5 	.word	0x00012be5
30006d30:	e000ed00 	.word	0xe000ed00
30006d34:	30009ab9 	.word	0x30009ab9

30006d38 <password_hash_check>:
30006d38:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d3c:	b08f      	sub	sp, #60	; 0x3c
30006d3e:	4690      	mov	r8, r2
30006d40:	461d      	mov	r5, r3
30006d42:	4606      	mov	r6, r0
30006d44:	af04      	add	r7, sp, #16
30006d46:	3201      	adds	r2, #1
30006d48:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d4a:	6079      	str	r1, [r7, #4]
30006d4c:	2100      	movs	r1, #0
30006d4e:	f103 0008 	add.w	r0, r3, #8
30006d52:	f108 0308 	add.w	r3, r8, #8
30006d56:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006d5a:	f023 0307 	bic.w	r3, r3, #7
30006d5e:	f020 0007 	bic.w	r0, r0, #7
30006d62:	ebad 0d03 	sub.w	sp, sp, r3
30006d66:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d68:	f10d 0910 	add.w	r9, sp, #16
30006d6c:	3301      	adds	r3, #1
30006d6e:	ebad 0d00 	sub.w	sp, sp, r0
30006d72:	603b      	str	r3, [r7, #0]
30006d74:	4648      	mov	r0, r9
30006d76:	4b3f      	ldr	r3, [pc, #252]	; (30006e74 <password_hash_check+0x13c>)
30006d78:	f10d 0a10 	add.w	sl, sp, #16
30006d7c:	4798      	blx	r3
30006d7e:	683b      	ldr	r3, [r7, #0]
30006d80:	2100      	movs	r1, #0
30006d82:	4650      	mov	r0, sl
30006d84:	461a      	mov	r2, r3
30006d86:	4b3b      	ldr	r3, [pc, #236]	; (30006e74 <password_hash_check+0x13c>)
30006d88:	4798      	blx	r3
30006d8a:	f1b8 0001 	subs.w	r0, r8, #1
30006d8e:	d01d      	beq.n	30006dcc <password_hash_check+0x94>
30006d90:	2200      	movs	r2, #0
30006d92:	f109 33ff 	add.w	r3, r9, #4294967295
30006d96:	4611      	mov	r1, r2
30006d98:	e00b      	b.n	30006db2 <password_hash_check+0x7a>
30006d9a:	f108 38ff 	add.w	r8, r8, #4294967295
30006d9e:	2100      	movs	r1, #0
30006da0:	1c9c      	adds	r4, r3, #2
30006da2:	4282      	cmp	r2, r0
30006da4:	7059      	strb	r1, [r3, #1]
30006da6:	f103 0301 	add.w	r3, r3, #1
30006daa:	4611      	mov	r1, r2
30006dac:	eba4 0409 	sub.w	r4, r4, r9
30006db0:	d20b      	bcs.n	30006dca <password_hash_check+0x92>
30006db2:	1874      	adds	r4, r6, r1
30006db4:	f102 0c01 	add.w	ip, r2, #1
30006db8:	5c71      	ldrb	r1, [r6, r1]
30006dba:	3202      	adds	r2, #2
30006dbc:	7864      	ldrb	r4, [r4, #1]
30006dbe:	2c30      	cmp	r4, #48	; 0x30
30006dc0:	d101      	bne.n	30006dc6 <password_hash_check+0x8e>
30006dc2:	295c      	cmp	r1, #92	; 0x5c
30006dc4:	d0e9      	beq.n	30006d9a <password_hash_check+0x62>
30006dc6:	4662      	mov	r2, ip
30006dc8:	e7ea      	b.n	30006da0 <password_hash_check+0x68>
30006dca:	4406      	add	r6, r0
30006dcc:	7833      	ldrb	r3, [r6, #0]
30006dce:	f809 3004 	strb.w	r3, [r9, r4]
30006dd2:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006dd4:	1e5c      	subs	r4, r3, #1
30006dd6:	d04a      	beq.n	30006e6e <password_hash_check+0x136>
30006dd8:	2200      	movs	r2, #0
30006dda:	f10a 33ff 	add.w	r3, sl, #4294967295
30006dde:	4611      	mov	r1, r2
30006de0:	e00b      	b.n	30006dfa <password_hash_check+0xc2>
30006de2:	f10b 3bff 	add.w	fp, fp, #4294967295
30006de6:	2100      	movs	r1, #0
30006de8:	1c98      	adds	r0, r3, #2
30006dea:	42a2      	cmp	r2, r4
30006dec:	7059      	strb	r1, [r3, #1]
30006dee:	f103 0301 	add.w	r3, r3, #1
30006df2:	4611      	mov	r1, r2
30006df4:	eba0 000a 	sub.w	r0, r0, sl
30006df8:	d20a      	bcs.n	30006e10 <password_hash_check+0xd8>
30006dfa:	1868      	adds	r0, r5, r1
30006dfc:	1c56      	adds	r6, r2, #1
30006dfe:	5c69      	ldrb	r1, [r5, r1]
30006e00:	3202      	adds	r2, #2
30006e02:	7840      	ldrb	r0, [r0, #1]
30006e04:	2830      	cmp	r0, #48	; 0x30
30006e06:	d101      	bne.n	30006e0c <password_hash_check+0xd4>
30006e08:	295c      	cmp	r1, #92	; 0x5c
30006e0a:	d0ea      	beq.n	30006de2 <password_hash_check+0xaa>
30006e0c:	4632      	mov	r2, r6
30006e0e:	e7eb      	b.n	30006de8 <password_hash_check+0xb0>
30006e10:	4425      	add	r5, r4
30006e12:	782b      	ldrb	r3, [r5, #0]
30006e14:	f107 0408 	add.w	r4, r7, #8
30006e18:	2520      	movs	r5, #32
30006e1a:	4641      	mov	r1, r8
30006e1c:	f80a 3000 	strb.w	r3, [sl, r0]
30006e20:	4652      	mov	r2, sl
30006e22:	687b      	ldr	r3, [r7, #4]
30006e24:	4648      	mov	r0, r9
30006e26:	9402      	str	r4, [sp, #8]
30006e28:	9300      	str	r3, [sp, #0]
30006e2a:	465b      	mov	r3, fp
30006e2c:	9501      	str	r5, [sp, #4]
30006e2e:	f7ff fe95 	bl	30006b5c <BOOT_RAM_KeyDeriveFunc>
30006e32:	4620      	mov	r0, r4
30006e34:	462a      	mov	r2, r5
30006e36:	4910      	ldr	r1, [pc, #64]	; (30006e78 <password_hash_check+0x140>)
30006e38:	f005 f80a 	bl	3000be50 <____wrap_memcmp_veneer>
30006e3c:	4604      	mov	r4, r0
30006e3e:	b968      	cbnz	r0, 30006e5c <password_hash_check+0x124>
30006e40:	480e      	ldr	r0, [pc, #56]	; (30006e7c <password_hash_check+0x144>)
30006e42:	f004 fff5 	bl	3000be30 <__DiagPrintf_veneer>
30006e46:	462a      	mov	r2, r5
30006e48:	4621      	mov	r1, r4
30006e4a:	f107 0008 	add.w	r0, r7, #8
30006e4e:	4b09      	ldr	r3, [pc, #36]	; (30006e74 <password_hash_check+0x13c>)
30006e50:	4798      	blx	r3
30006e52:	4620      	mov	r0, r4
30006e54:	372c      	adds	r7, #44	; 0x2c
30006e56:	46bd      	mov	sp, r7
30006e58:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e5c:	2401      	movs	r4, #1
30006e5e:	4808      	ldr	r0, [pc, #32]	; (30006e80 <password_hash_check+0x148>)
30006e60:	f004 ffe6 	bl	3000be30 <__DiagPrintf_veneer>
30006e64:	372c      	adds	r7, #44	; 0x2c
30006e66:	4620      	mov	r0, r4
30006e68:	46bd      	mov	sp, r7
30006e6a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e6e:	4620      	mov	r0, r4
30006e70:	e7cf      	b.n	30006e12 <password_hash_check+0xda>
30006e72:	bf00      	nop
30006e74:	00012be5 	.word	0x00012be5
30006e78:	3000dc30 	.word	0x3000dc30
30006e7c:	3000ca84 	.word	0x3000ca84
30006e80:	3000ca6c 	.word	0x3000ca6c

30006e84 <BOOT_Image1>:
30006e84:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006e88:	4994      	ldr	r1, [pc, #592]	; (300070dc <BOOT_Image1+0x258>)
30006e8a:	b089      	sub	sp, #36	; 0x24
30006e8c:	4894      	ldr	r0, [pc, #592]	; (300070e0 <BOOT_Image1+0x25c>)
30006e8e:	f004 ffcf 	bl	3000be30 <__DiagPrintf_veneer>
30006e92:	4894      	ldr	r0, [pc, #592]	; (300070e4 <BOOT_Image1+0x260>)
30006e94:	4a94      	ldr	r2, [pc, #592]	; (300070e8 <BOOT_Image1+0x264>)
30006e96:	2100      	movs	r1, #0
30006e98:	4b94      	ldr	r3, [pc, #592]	; (300070ec <BOOT_Image1+0x268>)
30006e9a:	1a12      	subs	r2, r2, r0
30006e9c:	4798      	blx	r3
30006e9e:	4a94      	ldr	r2, [pc, #592]	; (300070f0 <BOOT_Image1+0x26c>)
30006ea0:	4994      	ldr	r1, [pc, #592]	; (300070f4 <BOOT_Image1+0x270>)
30006ea2:	8c13      	ldrh	r3, [r2, #32]
30006ea4:	b29b      	uxth	r3, r3
30006ea6:	8413      	strh	r3, [r2, #32]
30006ea8:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006eac:	f002 fd28 	bl	30009900 <BOOT_Reason>
30006eb0:	2800      	cmp	r0, #0
30006eb2:	f000 82ba 	beq.w	3000742a <BOOT_Image1+0x5a6>
30006eb6:	4b90      	ldr	r3, [pc, #576]	; (300070f8 <BOOT_Image1+0x274>)
30006eb8:	781b      	ldrb	r3, [r3, #0]
30006eba:	2b00      	cmp	r3, #0
30006ebc:	f040 82c0 	bne.w	30007440 <BOOT_Image1+0x5bc>
30006ec0:	4d8e      	ldr	r5, [pc, #568]	; (300070fc <BOOT_Image1+0x278>)
30006ec2:	682b      	ldr	r3, [r5, #0]
30006ec4:	4e8e      	ldr	r6, [pc, #568]	; (30007100 <BOOT_Image1+0x27c>)
30006ec6:	f043 0302 	orr.w	r3, r3, #2
30006eca:	4a8e      	ldr	r2, [pc, #568]	; (30007104 <BOOT_Image1+0x280>)
30006ecc:	602b      	str	r3, [r5, #0]
30006ece:	4790      	blx	r2
30006ed0:	7834      	ldrb	r4, [r6, #0]
30006ed2:	2c00      	cmp	r4, #0
30006ed4:	f000 8270 	beq.w	300073b8 <BOOT_Image1+0x534>
30006ed8:	2000      	movs	r0, #0
30006eda:	4b8b      	ldr	r3, [pc, #556]	; (30007108 <BOOT_Image1+0x284>)
30006edc:	4798      	blx	r3
30006ede:	4b8b      	ldr	r3, [pc, #556]	; (3000710c <BOOT_Image1+0x288>)
30006ee0:	2001      	movs	r0, #1
30006ee2:	4798      	blx	r3
30006ee4:	4c8a      	ldr	r4, [pc, #552]	; (30007110 <BOOT_Image1+0x28c>)
30006ee6:	f7ff fce5 	bl	300068b4 <BOOT_SOC_ClkSet>
30006eea:	f7fe fcb1 	bl	30005850 <BOOT_GRstConfig>
30006eee:	47a0      	blx	r4
30006ef0:	2802      	cmp	r0, #2
30006ef2:	f000 826b 	beq.w	300073cc <BOOT_Image1+0x548>
30006ef6:	4b87      	ldr	r3, [pc, #540]	; (30007114 <BOOT_Image1+0x290>)
30006ef8:	2701      	movs	r7, #1
30006efa:	4987      	ldr	r1, [pc, #540]	; (30007118 <BOOT_Image1+0x294>)
30006efc:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006f00:	f883 710e 	strb.w	r7, [r3, #270]	; 0x10e
30006f04:	463a      	mov	r2, r7
30006f06:	f8df 8258 	ldr.w	r8, [pc, #600]	; 30007160 <BOOT_Image1+0x2dc>
30006f0a:	47c0      	blx	r8
30006f0c:	463a      	mov	r2, r7
30006f0e:	4983      	ldr	r1, [pc, #524]	; (3000711c <BOOT_Image1+0x298>)
30006f10:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006f14:	47c0      	blx	r8
30006f16:	4a76      	ldr	r2, [pc, #472]	; (300070f0 <BOOT_Image1+0x26c>)
30006f18:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006f1c:	f043 0303 	orr.w	r3, r3, #3
30006f20:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006f24:	4b7e      	ldr	r3, [pc, #504]	; (30007120 <BOOT_Image1+0x29c>)
30006f26:	4798      	blx	r3
30006f28:	b930      	cbnz	r0, 30006f38 <BOOT_Image1+0xb4>
30006f2a:	4a71      	ldr	r2, [pc, #452]	; (300070f0 <BOOT_Image1+0x26c>)
30006f2c:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006f30:	f023 0303 	bic.w	r3, r3, #3
30006f34:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006f38:	4b7a      	ldr	r3, [pc, #488]	; (30007124 <BOOT_Image1+0x2a0>)
30006f3a:	781b      	ldrb	r3, [r3, #0]
30006f3c:	2b00      	cmp	r3, #0
30006f3e:	f040 82cd 	bne.w	300074dc <BOOT_Image1+0x658>
30006f42:	4f79      	ldr	r7, [pc, #484]	; (30007128 <BOOT_Image1+0x2a4>)
30006f44:	2102      	movs	r1, #2
30006f46:	2201      	movs	r2, #1
30006f48:	4878      	ldr	r0, [pc, #480]	; (3000712c <BOOT_Image1+0x2a8>)
30006f4a:	47b8      	blx	r7
30006f4c:	68ab      	ldr	r3, [r5, #8]
30006f4e:	0799      	lsls	r1, r3, #30
30006f50:	f100 8281 	bmi.w	30007456 <BOOT_Image1+0x5d2>
30006f54:	4976      	ldr	r1, [pc, #472]	; (30007130 <BOOT_Image1+0x2ac>)
30006f56:	e841 f100 	tt	r1, r1
30006f5a:	68ab      	ldr	r3, [r5, #8]
30006f5c:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006f60:	079a      	lsls	r2, r3, #30
30006f62:	f100 825e 	bmi.w	30007422 <BOOT_Image1+0x59e>
30006f66:	4b73      	ldr	r3, [pc, #460]	; (30007134 <BOOT_Image1+0x2b0>)
30006f68:	4f73      	ldr	r7, [pc, #460]	; (30007138 <BOOT_Image1+0x2b4>)
30006f6a:	681b      	ldr	r3, [r3, #0]
30006f6c:	4798      	blx	r3
30006f6e:	f7fe fa61 	bl	30005434 <BOOT_RccConfig>
30006f72:	f7fe facb 	bl	3000550c <BOOT_CACHEWRR_Set>
30006f76:	4b71      	ldr	r3, [pc, #452]	; (3000713c <BOOT_Image1+0x2b8>)
30006f78:	6818      	ldr	r0, [r3, #0]
30006f7a:	f7fe faf5 	bl	30005568 <BOOT_TCMSet>
30006f7e:	4b70      	ldr	r3, [pc, #448]	; (30007140 <BOOT_Image1+0x2bc>)
30006f80:	695b      	ldr	r3, [r3, #20]
30006f82:	03db      	lsls	r3, r3, #15
30006f84:	d513      	bpl.n	30006fae <BOOT_Image1+0x12a>
30006f86:	f017 011f 	ands.w	r1, r7, #31
30006f8a:	f040 8211 	bne.w	300073b0 <BOOT_Image1+0x52c>
30006f8e:	463b      	mov	r3, r7
30006f90:	2174      	movs	r1, #116	; 0x74
30006f92:	f3bf 8f4f 	dsb	sy
30006f96:	486a      	ldr	r0, [pc, #424]	; (30007140 <BOOT_Image1+0x2bc>)
30006f98:	4419      	add	r1, r3
30006f9a:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006f9e:	3320      	adds	r3, #32
30006fa0:	1aca      	subs	r2, r1, r3
30006fa2:	2a00      	cmp	r2, #0
30006fa4:	dcf9      	bgt.n	30006f9a <BOOT_Image1+0x116>
30006fa6:	f3bf 8f4f 	dsb	sy
30006faa:	f3bf 8f6f 	isb	sy
30006fae:	4b50      	ldr	r3, [pc, #320]	; (300070f0 <BOOT_Image1+0x26c>)
30006fb0:	2200      	movs	r2, #0
30006fb2:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30006fb6:	4b57      	ldr	r3, [pc, #348]	; (30007114 <BOOT_Image1+0x290>)
30006fb8:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30006fbc:	f7fc fc10 	bl	300037e0 <flash_highspeed_setup>
30006fc0:	f001 fd9e 	bl	30008b00 <flash_layout_init>
30006fc4:	47a0      	blx	r4
30006fc6:	2802      	cmp	r0, #2
30006fc8:	f000 8212 	beq.w	300073f0 <BOOT_Image1+0x56c>
30006fcc:	7832      	ldrb	r2, [r6, #0]
30006fce:	68ab      	ldr	r3, [r5, #8]
30006fd0:	2a00      	cmp	r2, #0
30006fd2:	f040 81e0 	bne.w	30007396 <BOOT_Image1+0x512>
30006fd6:	079a      	lsls	r2, r3, #30
30006fd8:	f100 830e 	bmi.w	300075f8 <BOOT_Image1+0x774>
30006fdc:	f7fe fc74 	bl	300058c8 <BOOT_DDR_Init>
30006fe0:	4a43      	ldr	r2, [pc, #268]	; (300070f0 <BOOT_Image1+0x26c>)
30006fe2:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30006fe6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30006fea:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30006fee:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30006ff2:	2100      	movs	r1, #0
30006ff4:	4b3e      	ldr	r3, [pc, #248]	; (300070f0 <BOOT_Image1+0x26c>)
30006ff6:	4a53      	ldr	r2, [pc, #332]	; (30007144 <BOOT_Image1+0x2c0>)
30006ff8:	2002      	movs	r0, #2
30006ffa:	f8c3 12d0 	str.w	r1, [r3, #720]	; 0x2d0
30006ffe:	2632      	movs	r6, #50	; 0x32
30007000:	f8c3 12d4 	str.w	r1, [r3, #724]	; 0x2d4
30007004:	f8c3 12d8 	str.w	r1, [r3, #728]	; 0x2d8
30007008:	f8c3 10d0 	str.w	r1, [r3, #208]	; 0xd0
3000700c:	f000 fd78 	bl	30007b00 <ymodem_uart_port_init>
30007010:	484d      	ldr	r0, [pc, #308]	; (30007148 <BOOT_Image1+0x2c4>)
30007012:	f004 fecd 	bl	3000bdb0 <____wrap_strlen_veneer>
30007016:	4601      	mov	r1, r0
30007018:	484b      	ldr	r0, [pc, #300]	; (30007148 <BOOT_Image1+0x2c4>)
3000701a:	4c4c      	ldr	r4, [pc, #304]	; (3000714c <BOOT_Image1+0x2c8>)
3000701c:	f000 fdca 	bl	30007bb4 <ymodem_uart_putdata>
30007020:	e002      	b.n	30007028 <BOOT_Image1+0x1a4>
30007022:	3e01      	subs	r6, #1
30007024:	f000 82ce 	beq.w	300075c4 <BOOT_Image1+0x740>
30007028:	2001      	movs	r0, #1
3000702a:	47a0      	blx	r4
3000702c:	f000 fda8 	bl	30007b80 <ymodem_uart_readable>
30007030:	2800      	cmp	r0, #0
30007032:	d0f6      	beq.n	30007022 <BOOT_Image1+0x19e>
30007034:	f000 fda4 	bl	30007b80 <ymodem_uart_readable>
30007038:	2800      	cmp	r0, #0
3000703a:	f000 82eb 	beq.w	30007614 <BOOT_Image1+0x790>
3000703e:	2301      	movs	r3, #1
30007040:	9303      	str	r3, [sp, #12]
30007042:	2001      	movs	r0, #1
30007044:	47a0      	blx	r4
30007046:	2108      	movs	r1, #8
30007048:	4841      	ldr	r0, [pc, #260]	; (30007150 <BOOT_Image1+0x2cc>)
3000704a:	f000 fdc1 	bl	30007bd0 <ymodem_uart_getdata>
3000704e:	4940      	ldr	r1, [pc, #256]	; (30007150 <BOOT_Image1+0x2cc>)
30007050:	2208      	movs	r2, #8
30007052:	4b40      	ldr	r3, [pc, #256]	; (30007154 <BOOT_Image1+0x2d0>)
30007054:	f101 0080 	add.w	r0, r1, #128	; 0x80
30007058:	4798      	blx	r3
3000705a:	4606      	mov	r6, r0
3000705c:	2800      	cmp	r0, #0
3000705e:	f040 833f 	bne.w	300076e0 <BOOT_Image1+0x85c>
30007062:	f000 fd8d 	bl	30007b80 <ymodem_uart_readable>
30007066:	2800      	cmp	r0, #0
30007068:	d0fb      	beq.n	30007062 <BOOT_Image1+0x1de>
3000706a:	2101      	movs	r1, #1
3000706c:	483a      	ldr	r0, [pc, #232]	; (30007158 <BOOT_Image1+0x2d4>)
3000706e:	f000 fdaf 	bl	30007bd0 <ymodem_uart_getdata>
30007072:	f000 fd85 	bl	30007b80 <ymodem_uart_readable>
30007076:	2800      	cmp	r0, #0
30007078:	d0fb      	beq.n	30007072 <BOOT_Image1+0x1ee>
3000707a:	4f38      	ldr	r7, [pc, #224]	; (3000715c <BOOT_Image1+0x2d8>)
3000707c:	2101      	movs	r1, #1
3000707e:	f207 404d 	addw	r0, r7, #1101	; 0x44d
30007082:	f000 fda5 	bl	30007bd0 <ymodem_uart_getdata>
30007086:	f897 244d 	ldrb.w	r2, [r7, #1101]	; 0x44d
3000708a:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
3000708e:	429a      	cmp	r2, r3
30007090:	f000 8331 	beq.w	300076f6 <BOOT_Image1+0x872>
30007094:	f207 4a4e 	addw	sl, r7, #1102	; 0x44e
30007098:	f04f 0800 	mov.w	r8, #0
3000709c:	46c1      	mov	r9, r8
3000709e:	f108 0801 	add.w	r8, r8, #1
300070a2:	f000 fd6d 	bl	30007b80 <ymodem_uart_readable>
300070a6:	2800      	cmp	r0, #0
300070a8:	d0fb      	beq.n	300070a2 <BOOT_Image1+0x21e>
300070aa:	4650      	mov	r0, sl
300070ac:	2101      	movs	r1, #1
300070ae:	f000 fd8f 	bl	30007bd0 <ymodem_uart_getdata>
300070b2:	f89a 2000 	ldrb.w	r2, [sl]
300070b6:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
300070ba:	f109 090b 	add.w	r9, r9, #11
300070be:	f10a 0a01 	add.w	sl, sl, #1
300070c2:	46c3      	mov	fp, r8
300070c4:	429a      	cmp	r2, r3
300070c6:	d1e9      	bne.n	3000709c <BOOT_Image1+0x218>
300070c8:	f1b8 0f00 	cmp.w	r8, #0
300070cc:	dd59      	ble.n	30007182 <BOOT_Image1+0x2fe>
300070ce:	f207 414c 	addw	r1, r7, #1100	; 0x44c
300070d2:	2300      	movs	r3, #0
300070d4:	eb01 0008 	add.w	r0, r1, r8
300070d8:	e046      	b.n	30007168 <BOOT_Image1+0x2e4>
300070da:	bf00      	nop
300070dc:	3000d25c 	.word	0x3000d25c
300070e0:	3000caa0 	.word	0x3000caa0
300070e4:	3000e560 	.word	0x3000e560
300070e8:	3000e994 	.word	0x3000e994
300070ec:	00012be5 	.word	0x00012be5
300070f0:	42008000 	.word	0x42008000
300070f4:	42008200 	.word	0x42008200
300070f8:	3000d2a9 	.word	0x3000d2a9
300070fc:	2001c00c 	.word	0x2001c00c
30007100:	3000d295 	.word	0x3000d295
30007104:	3000b82d 	.word	0x3000b82d
30007108:	3000b69d 	.word	0x3000b69d
3000710c:	3000b5bd 	.word	0x3000b5bd
30007110:	30007db5 	.word	0x30007db5
30007114:	23020000 	.word	0x23020000
30007118:	40080000 	.word	0x40080000
3000711c:	40040000 	.word	0x40040000
30007120:	3000a191 	.word	0x3000a191
30007124:	3000d294 	.word	0x3000d294
30007128:	0000aab9 	.word	0x0000aab9
3000712c:	4200c000 	.word	0x4200c000
30007130:	30006e85 	.word	0x30006e85
30007134:	00033000 	.word	0x00033000
30007138:	2001c01c 	.word	0x2001c01c
3000713c:	3000d298 	.word	0x3000d298
30007140:	e000ed00 	.word	0xe000ed00
30007144:	0016e360 	.word	0x0016e360
30007148:	3000dc50 	.word	0x3000dc50
3000714c:	00009be5 	.word	0x00009be5
30007150:	3000dc6c 	.word	0x3000dc6c
30007154:	000129bd 	.word	0x000129bd
30007158:	3000dc74 	.word	0x3000dc74
3000715c:	3000d828 	.word	0x3000d828
30007160:	0000b479 	.word	0x0000b479
30007164:	4288      	cmp	r0, r1
30007166:	d00d      	beq.n	30007184 <BOOT_Image1+0x300>
30007168:	f811 2f01 	ldrb.w	r2, [r1, #1]!
3000716c:	eb03 0383 	add.w	r3, r3, r3, lsl #2
30007170:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30007174:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30007178:	f1bc 0f09 	cmp.w	ip, #9
3000717c:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30007180:	d9f0      	bls.n	30007164 <BOOT_Image1+0x2e0>
30007182:	2300      	movs	r3, #0
30007184:	f8c7 34d0 	str.w	r3, [r7, #1232]	; 0x4d0
30007188:	f000 fcfa 	bl	30007b80 <ymodem_uart_readable>
3000718c:	2800      	cmp	r0, #0
3000718e:	d0fb      	beq.n	30007188 <BOOT_Image1+0x304>
30007190:	49b4      	ldr	r1, [pc, #720]	; (30007464 <BOOT_Image1+0x5e0>)
30007192:	460b      	mov	r3, r1
30007194:	2101      	movs	r1, #1
30007196:	eb03 0009 	add.w	r0, r3, r9
3000719a:	9302      	str	r3, [sp, #8]
3000719c:	f000 fd18 	bl	30007bd0 <ymodem_uart_getdata>
300071a0:	9902      	ldr	r1, [sp, #8]
300071a2:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
300071a6:	f811 2009 	ldrb.w	r2, [r1, r9]
300071aa:	429a      	cmp	r2, r3
300071ac:	f000 82ac 	beq.w	30007708 <BOOT_Image1+0x884>
300071b0:	f10b 090b 	add.w	r9, fp, #11
300071b4:	2300      	movs	r3, #0
300071b6:	9404      	str	r4, [sp, #16]
300071b8:	4489      	add	r9, r1
300071ba:	469a      	mov	sl, r3
300071bc:	f10a 0a01 	add.w	sl, sl, #1
300071c0:	f000 fcde 	bl	30007b80 <ymodem_uart_readable>
300071c4:	2800      	cmp	r0, #0
300071c6:	d0fb      	beq.n	300071c0 <BOOT_Image1+0x33c>
300071c8:	4648      	mov	r0, r9
300071ca:	2101      	movs	r1, #1
300071cc:	f000 fd00 	bl	30007bd0 <ymodem_uart_getdata>
300071d0:	f899 2000 	ldrb.w	r2, [r9]
300071d4:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
300071d8:	f109 0901 	add.w	r9, r9, #1
300071dc:	eb0a 010b 	add.w	r1, sl, fp
300071e0:	429a      	cmp	r2, r3
300071e2:	d1eb      	bne.n	300071bc <BOOT_Image1+0x338>
300071e4:	f8cd a014 	str.w	sl, [sp, #20]
300071e8:	468a      	mov	sl, r1
300071ea:	9c04      	ldr	r4, [sp, #16]
300071ec:	f000 fcc8 	bl	30007b80 <ymodem_uart_readable>
300071f0:	2800      	cmp	r0, #0
300071f2:	d0fb      	beq.n	300071ec <BOOT_Image1+0x368>
300071f4:	9b02      	ldr	r3, [sp, #8]
300071f6:	f10a 090b 	add.w	r9, sl, #11
300071fa:	2101      	movs	r1, #1
300071fc:	eb03 0009 	add.w	r0, r3, r9
30007200:	f000 fce6 	bl	30007bd0 <ymodem_uart_getdata>
30007204:	9902      	ldr	r1, [sp, #8]
30007206:	f897 24cd 	ldrb.w	r2, [r7, #1229]	; 0x4cd
3000720a:	f811 3009 	ldrb.w	r3, [r1, r9]
3000720e:	429a      	cmp	r2, r3
30007210:	f000 8278 	beq.w	30007704 <BOOT_Image1+0x880>
30007214:	f10a 090c 	add.w	r9, sl, #12
30007218:	4489      	add	r9, r1
3000721a:	3601      	adds	r6, #1
3000721c:	f000 fcb0 	bl	30007b80 <ymodem_uart_readable>
30007220:	2800      	cmp	r0, #0
30007222:	d0fb      	beq.n	3000721c <BOOT_Image1+0x398>
30007224:	4648      	mov	r0, r9
30007226:	2101      	movs	r1, #1
30007228:	f000 fcd2 	bl	30007bd0 <ymodem_uart_getdata>
3000722c:	f899 2000 	ldrb.w	r2, [r9]
30007230:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
30007234:	f109 0901 	add.w	r9, r9, #1
30007238:	eb06 0b0a 	add.w	fp, r6, sl
3000723c:	429a      	cmp	r2, r3
3000723e:	d1ec      	bne.n	3000721a <BOOT_Image1+0x396>
30007240:	9b02      	ldr	r3, [sp, #8]
30007242:	f10b 090c 	add.w	r9, fp, #12
30007246:	2101      	movs	r1, #1
30007248:	eb03 0009 	add.w	r0, r3, r9
3000724c:	f000 fcc0 	bl	30007bd0 <ymodem_uart_getdata>
30007250:	9b02      	ldr	r3, [sp, #8]
30007252:	f813 1009 	ldrb.w	r1, [r3, r9]
30007256:	290a      	cmp	r1, #10
30007258:	f8c7 14d4 	str.w	r1, [r7, #1236]	; 0x4d4
3000725c:	f200 823c 	bhi.w	300076d8 <BOOT_Image1+0x854>
30007260:	9b02      	ldr	r3, [sp, #8]
30007262:	f10b 000d 	add.w	r0, fp, #13
30007266:	2101      	movs	r1, #1
30007268:	f10b 090e 	add.w	r9, fp, #14
3000726c:	4418      	add	r0, r3
3000726e:	f000 fcaf 	bl	30007bd0 <ymodem_uart_getdata>
30007272:	9b02      	ldr	r3, [sp, #8]
30007274:	2102      	movs	r1, #2
30007276:	4499      	add	r9, r3
30007278:	4648      	mov	r0, r9
3000727a:	f000 fca9 	bl	30007bd0 <ymodem_uart_getdata>
3000727e:	4649      	mov	r1, r9
30007280:	2202      	movs	r2, #2
30007282:	4879      	ldr	r0, [pc, #484]	; (30007468 <BOOT_Image1+0x5e4>)
30007284:	4b79      	ldr	r3, [pc, #484]	; (3000746c <BOOT_Image1+0x5e8>)
30007286:	4798      	blx	r3
30007288:	2800      	cmp	r0, #0
3000728a:	f040 8230 	bne.w	300076ee <BOOT_Image1+0x86a>
3000728e:	9b02      	ldr	r3, [sp, #8]
30007290:	f10b 0010 	add.w	r0, fp, #16
30007294:	2101      	movs	r1, #1
30007296:	f10b 0911 	add.w	r9, fp, #17
3000729a:	4418      	add	r0, r3
3000729c:	f000 fc98 	bl	30007bd0 <ymodem_uart_getdata>
300072a0:	4b73      	ldr	r3, [pc, #460]	; (30007470 <BOOT_Image1+0x5ec>)
300072a2:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
300072a6:	f103 0228 	add.w	r2, r3, #40	; 0x28
300072aa:	f103 0130 	add.w	r1, r3, #48	; 0x30
300072ae:	f7fd fe05 	bl	30004ebc <hotfix_get_addr_size>
300072b2:	9b02      	ldr	r3, [sp, #8]
300072b4:	2102      	movs	r1, #2
300072b6:	4499      	add	r9, r3
300072b8:	4648      	mov	r0, r9
300072ba:	f000 fc89 	bl	30007bd0 <ymodem_uart_getdata>
300072be:	4649      	mov	r1, r9
300072c0:	2202      	movs	r2, #2
300072c2:	486c      	ldr	r0, [pc, #432]	; (30007474 <BOOT_Image1+0x5f0>)
300072c4:	4b69      	ldr	r3, [pc, #420]	; (3000746c <BOOT_Image1+0x5e8>)
300072c6:	4798      	blx	r3
300072c8:	2800      	cmp	r0, #0
300072ca:	f000 8129 	beq.w	30007520 <BOOT_Image1+0x69c>
300072ce:	9b03      	ldr	r3, [sp, #12]
300072d0:	2b00      	cmp	r3, #0
300072d2:	f000 8170 	beq.w	300075b6 <BOOT_Image1+0x732>
300072d6:	f7fe fcff 	bl	30005cd8 <BOOT_LoadImages>
300072da:	2800      	cmp	r0, #0
300072dc:	f000 8119 	beq.w	30007512 <BOOT_Image1+0x68e>
300072e0:	f7ff faa4 	bl	3000682c <BOOT_Enable_KM0>
300072e4:	2008      	movs	r0, #8
300072e6:	4b64      	ldr	r3, [pc, #400]	; (30007478 <BOOT_Image1+0x5f4>)
300072e8:	4798      	blx	r3
300072ea:	4b64      	ldr	r3, [pc, #400]	; (3000747c <BOOT_Image1+0x5f8>)
300072ec:	2201      	movs	r2, #1
300072ee:	f8c3 2398 	str.w	r2, [r3, #920]	; 0x398
300072f2:	f7fd fd07 	bl	30004d04 <BOOT_RAM_TZCfg>
300072f6:	2004      	movs	r0, #4
300072f8:	47a0      	blx	r4
300072fa:	4b61      	ldr	r3, [pc, #388]	; (30007480 <BOOT_Image1+0x5fc>)
300072fc:	781b      	ldrb	r3, [r3, #0]
300072fe:	2b00      	cmp	r3, #0
30007300:	f000 8166 	beq.w	300075d0 <BOOT_Image1+0x74c>
30007304:	f7ff fbc0 	bl	30006a88 <BOOT_Enable_AP>
30007308:	4a5e      	ldr	r2, [pc, #376]	; (30007484 <BOOT_Image1+0x600>)
3000730a:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
3000730e:	f043 0302 	orr.w	r3, r3, #2
30007312:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30007316:	2019      	movs	r0, #25
30007318:	47a0      	blx	r4
3000731a:	495a      	ldr	r1, [pc, #360]	; (30007484 <BOOT_Image1+0x600>)
3000731c:	4a5a      	ldr	r2, [pc, #360]	; (30007488 <BOOT_Image1+0x604>)
3000731e:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
30007322:	6894      	ldr	r4, [r2, #8]
30007324:	f043 0301 	orr.w	r3, r3, #1
30007328:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
3000732c:	4b57      	ldr	r3, [pc, #348]	; (3000748c <BOOT_Image1+0x608>)
3000732e:	6811      	ldr	r1, [r2, #0]
30007330:	4a57      	ldr	r2, [pc, #348]	; (30007490 <BOOT_Image1+0x60c>)
30007332:	6061      	str	r1, [r4, #4]
30007334:	6094      	str	r4, [r2, #8]
30007336:	6a59      	ldr	r1, [r3, #36]	; 0x24
30007338:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000733c:	6259      	str	r1, [r3, #36]	; 0x24
3000733e:	6a51      	ldr	r1, [r2, #36]	; 0x24
30007340:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30007344:	6251      	str	r1, [r2, #36]	; 0x24
30007346:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000734a:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000734e:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30007352:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30007356:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000735a:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000735e:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30007362:	494c      	ldr	r1, [pc, #304]	; (30007494 <BOOT_Image1+0x610>)
30007364:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30007368:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
3000736c:	689b      	ldr	r3, [r3, #8]
3000736e:	618b      	str	r3, [r1, #24]
30007370:	f7fe fc3a 	bl	30005be8 <BOOT_NVICBackup_HP>
30007374:	4b48      	ldr	r3, [pc, #288]	; (30007498 <BOOT_Image1+0x614>)
30007376:	f383 8888 	msr	MSP_NS, r3
3000737a:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000737e:	f383 8809 	msr	PSP, r3
30007382:	68ab      	ldr	r3, [r5, #8]
30007384:	079b      	lsls	r3, r3, #30
30007386:	f100 813b 	bmi.w	30007600 <BOOT_Image1+0x77c>
3000738a:	6860      	ldr	r0, [r4, #4]
3000738c:	b009      	add	sp, #36	; 0x24
3000738e:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007392:	f7fd bfff 	b.w	30005394 <BOOT_NsStart>
30007396:	4c41      	ldr	r4, [pc, #260]	; (3000749c <BOOT_Image1+0x618>)
30007398:	4e41      	ldr	r6, [pc, #260]	; (300074a0 <BOOT_Image1+0x61c>)
3000739a:	f013 0f02 	tst.w	r3, #2
3000739e:	4630      	mov	r0, r6
300073a0:	d001      	beq.n	300073a6 <BOOT_Image1+0x522>
300073a2:	f004 fd45 	bl	3000be30 <__DiagPrintf_veneer>
300073a6:	f241 3088 	movw	r0, #5000	; 0x1388
300073aa:	47a0      	blx	r4
300073ac:	68ab      	ldr	r3, [r5, #8]
300073ae:	e7f4      	b.n	3000739a <BOOT_Image1+0x516>
300073b0:	3174      	adds	r1, #116	; 0x74
300073b2:	f027 031f 	bic.w	r3, r7, #31
300073b6:	e5ec      	b.n	30006f92 <BOOT_Image1+0x10e>
300073b8:	4b3a      	ldr	r3, [pc, #232]	; (300074a4 <BOOT_Image1+0x620>)
300073ba:	2001      	movs	r0, #1
300073bc:	4798      	blx	r3
300073be:	4620      	mov	r0, r4
300073c0:	4b39      	ldr	r3, [pc, #228]	; (300074a8 <BOOT_Image1+0x624>)
300073c2:	4798      	blx	r3
300073c4:	4b39      	ldr	r3, [pc, #228]	; (300074ac <BOOT_Image1+0x628>)
300073c6:	2001      	movs	r0, #1
300073c8:	4798      	blx	r3
300073ca:	e58b      	b.n	30006ee4 <BOOT_Image1+0x60>
300073cc:	4b38      	ldr	r3, [pc, #224]	; (300074b0 <BOOT_Image1+0x62c>)
300073ce:	2201      	movs	r2, #1
300073d0:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
300073d4:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
300073d8:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
300073dc:	4b35      	ldr	r3, [pc, #212]	; (300074b4 <BOOT_Image1+0x630>)
300073de:	4798      	blx	r3
300073e0:	4a35      	ldr	r2, [pc, #212]	; (300074b8 <BOOT_Image1+0x634>)
300073e2:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
300073e6:	f043 0302 	orr.w	r3, r3, #2
300073ea:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300073ee:	e599      	b.n	30006f24 <BOOT_Image1+0xa0>
300073f0:	68ab      	ldr	r3, [r5, #8]
300073f2:	0798      	lsls	r0, r3, #30
300073f4:	f100 80fc 	bmi.w	300075f0 <BOOT_Image1+0x76c>
300073f8:	4b30      	ldr	r3, [pc, #192]	; (300074bc <BOOT_Image1+0x638>)
300073fa:	4798      	blx	r3
300073fc:	f7fe faa2 	bl	30005944 <BOOT_PSRAM_Init>
30007400:	7830      	ldrb	r0, [r6, #0]
30007402:	2800      	cmp	r0, #0
30007404:	f47f adf5 	bne.w	30006ff2 <BOOT_Image1+0x16e>
30007408:	4b2d      	ldr	r3, [pc, #180]	; (300074c0 <BOOT_Image1+0x63c>)
3000740a:	4798      	blx	r3
3000740c:	2800      	cmp	r0, #0
3000740e:	f43f adf0 	beq.w	30006ff2 <BOOT_Image1+0x16e>
30007412:	682b      	ldr	r3, [r5, #0]
30007414:	0399      	lsls	r1, r3, #14
30007416:	f57f adec 	bpl.w	30006ff2 <BOOT_Image1+0x16e>
3000741a:	482a      	ldr	r0, [pc, #168]	; (300074c4 <BOOT_Image1+0x640>)
3000741c:	f004 fd08 	bl	3000be30 <__DiagPrintf_veneer>
30007420:	e5e7      	b.n	30006ff2 <BOOT_Image1+0x16e>
30007422:	4829      	ldr	r0, [pc, #164]	; (300074c8 <BOOT_Image1+0x644>)
30007424:	f004 fd04 	bl	3000be30 <__DiagPrintf_veneer>
30007428:	e59d      	b.n	30006f66 <BOOT_Image1+0xe2>
3000742a:	4601      	mov	r1, r0
3000742c:	f44f 7201 	mov.w	r2, #516	; 0x204
30007430:	481f      	ldr	r0, [pc, #124]	; (300074b0 <BOOT_Image1+0x62c>)
30007432:	f004 fcdd 	bl	3000bdf0 <____wrap_memset_veneer>
30007436:	4b25      	ldr	r3, [pc, #148]	; (300074cc <BOOT_Image1+0x648>)
30007438:	781b      	ldrb	r3, [r3, #0]
3000743a:	2b00      	cmp	r3, #0
3000743c:	f43f ad40 	beq.w	30006ec0 <BOOT_Image1+0x3c>
30007440:	4b23      	ldr	r3, [pc, #140]	; (300074d0 <BOOT_Image1+0x64c>)
30007442:	4d24      	ldr	r5, [pc, #144]	; (300074d4 <BOOT_Image1+0x650>)
30007444:	4798      	blx	r3
30007446:	2800      	cmp	r0, #0
30007448:	f47f ad3b 	bne.w	30006ec2 <BOOT_Image1+0x3e>
3000744c:	68ab      	ldr	r3, [r5, #8]
3000744e:	f043 0302 	orr.w	r3, r3, #2
30007452:	60ab      	str	r3, [r5, #8]
30007454:	e535      	b.n	30006ec2 <BOOT_Image1+0x3e>
30007456:	f3ef 8108 	mrs	r1, MSP
3000745a:	481f      	ldr	r0, [pc, #124]	; (300074d8 <BOOT_Image1+0x654>)
3000745c:	f004 fce8 	bl	3000be30 <__DiagPrintf_veneer>
30007460:	e578      	b.n	30006f54 <BOOT_Image1+0xd0>
30007462:	bf00      	nop
30007464:	3000dc6c 	.word	0x3000dc6c
30007468:	3000dd00 	.word	0x3000dd00
3000746c:	000129bd 	.word	0x000129bd
30007470:	3000dd04 	.word	0x3000dd04
30007474:	3000dd40 	.word	0x3000dd40
30007478:	0000e1d9 	.word	0x0000e1d9
3000747c:	400e0000 	.word	0x400e0000
30007480:	3000d296 	.word	0x3000d296
30007484:	42008200 	.word	0x42008200
30007488:	60000020 	.word	0x60000020
3000748c:	e000ed00 	.word	0xe000ed00
30007490:	e002ed00 	.word	0xe002ed00
30007494:	3000e924 	.word	0x3000e924
30007498:	2001bffc 	.word	0x2001bffc
3000749c:	00009be5 	.word	0x00009be5
300074a0:	3000cb50 	.word	0x3000cb50
300074a4:	3000b5bd 	.word	0x3000b5bd
300074a8:	3000b7ed 	.word	0x3000b7ed
300074ac:	3000b75d 	.word	0x3000b75d
300074b0:	23020000 	.word	0x23020000
300074b4:	0000b479 	.word	0x0000b479
300074b8:	42008000 	.word	0x42008000
300074bc:	30009161 	.word	0x30009161
300074c0:	3000b6bd 	.word	0x3000b6bd
300074c4:	3000cb40 	.word	0x3000cb40
300074c8:	3000cae8 	.word	0x3000cae8
300074cc:	3000d2a9 	.word	0x3000d2a9
300074d0:	0000c179 	.word	0x0000c179
300074d4:	2001c00c 	.word	0x2001c00c
300074d8:	3000cab8 	.word	0x3000cab8
300074dc:	f8df 82a4 	ldr.w	r8, [pc, #676]	; 30007784 <BOOT_Image1+0x900>
300074e0:	47c0      	blx	r8
300074e2:	4607      	mov	r7, r0
300074e4:	47c0      	blx	r8
300074e6:	2f02      	cmp	r7, #2
300074e8:	f43f ad2b 	beq.w	30006f42 <BOOT_Image1+0xbe>
300074ec:	2803      	cmp	r0, #3
300074ee:	f43f ad28 	beq.w	30006f42 <BOOT_Image1+0xbe>
300074f2:	4b8b      	ldr	r3, [pc, #556]	; (30007720 <BOOT_Image1+0x89c>)
300074f4:	4798      	blx	r3
300074f6:	4f8b      	ldr	r7, [pc, #556]	; (30007724 <BOOT_Image1+0x8a0>)
300074f8:	2200      	movs	r2, #0
300074fa:	2101      	movs	r1, #1
300074fc:	488a      	ldr	r0, [pc, #552]	; (30007728 <BOOT_Image1+0x8a4>)
300074fe:	47b8      	blx	r7
30007500:	2101      	movs	r1, #1
30007502:	4889      	ldr	r0, [pc, #548]	; (30007728 <BOOT_Image1+0x8a4>)
30007504:	4b89      	ldr	r3, [pc, #548]	; (3000772c <BOOT_Image1+0x8a8>)
30007506:	4798      	blx	r3
30007508:	2201      	movs	r2, #1
3000750a:	4887      	ldr	r0, [pc, #540]	; (30007728 <BOOT_Image1+0x8a4>)
3000750c:	4611      	mov	r1, r2
3000750e:	47b8      	blx	r7
30007510:	e518      	b.n	30006f44 <BOOT_Image1+0xc0>
30007512:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007516:	47a0      	blx	r4
30007518:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000751c:	47a0      	blx	r4
3000751e:	e7f8      	b.n	30007512 <BOOT_Image1+0x68e>
30007520:	9b02      	ldr	r3, [sp, #8]
30007522:	f10b 0013 	add.w	r0, fp, #19
30007526:	2101      	movs	r1, #1
30007528:	f10b 0914 	add.w	r9, fp, #20
3000752c:	4418      	add	r0, r3
3000752e:	f000 fb4f 	bl	30007bd0 <ymodem_uart_getdata>
30007532:	4b7f      	ldr	r3, [pc, #508]	; (30007730 <BOOT_Image1+0x8ac>)
30007534:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
30007538:	f1a3 0218 	sub.w	r2, r3, #24
3000753c:	f1a3 0110 	sub.w	r1, r3, #16
30007540:	f7fd fcbc 	bl	30004ebc <hotfix_get_addr_size>
30007544:	9b02      	ldr	r3, [sp, #8]
30007546:	2102      	movs	r1, #2
30007548:	4499      	add	r9, r3
3000754a:	4648      	mov	r0, r9
3000754c:	f000 fb40 	bl	30007bd0 <ymodem_uart_getdata>
30007550:	4649      	mov	r1, r9
30007552:	2202      	movs	r2, #2
30007554:	4877      	ldr	r0, [pc, #476]	; (30007734 <BOOT_Image1+0x8b0>)
30007556:	4b78      	ldr	r3, [pc, #480]	; (30007738 <BOOT_Image1+0x8b4>)
30007558:	4798      	blx	r3
3000755a:	2800      	cmp	r0, #0
3000755c:	f47f aeb7 	bne.w	300072ce <BOOT_Image1+0x44a>
30007560:	9b02      	ldr	r3, [sp, #8]
30007562:	f10b 0016 	add.w	r0, fp, #22
30007566:	2101      	movs	r1, #1
30007568:	f10b 0917 	add.w	r9, fp, #23
3000756c:	4418      	add	r0, r3
3000756e:	f000 fb2f 	bl	30007bd0 <ymodem_uart_getdata>
30007572:	4b72      	ldr	r3, [pc, #456]	; (3000773c <BOOT_Image1+0x8b8>)
30007574:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
30007578:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
3000757c:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30007580:	f7fd fc9c 	bl	30004ebc <hotfix_get_addr_size>
30007584:	9a02      	ldr	r2, [sp, #8]
30007586:	2102      	movs	r1, #2
30007588:	4491      	add	r9, r2
3000758a:	4648      	mov	r0, r9
3000758c:	f000 fb20 	bl	30007bd0 <ymodem_uart_getdata>
30007590:	4649      	mov	r1, r9
30007592:	2202      	movs	r2, #2
30007594:	486a      	ldr	r0, [pc, #424]	; (30007740 <BOOT_Image1+0x8bc>)
30007596:	4b68      	ldr	r3, [pc, #416]	; (30007738 <BOOT_Image1+0x8b4>)
30007598:	4798      	blx	r3
3000759a:	2800      	cmp	r0, #0
3000759c:	f47f ae97 	bne.w	300072ce <BOOT_Image1+0x44a>
300075a0:	9b02      	ldr	r3, [sp, #8]
300075a2:	f10b 0019 	add.w	r0, fp, #25
300075a6:	2101      	movs	r1, #1
300075a8:	4418      	add	r0, r3
300075aa:	f000 fb11 	bl	30007bd0 <ymodem_uart_getdata>
300075ae:	9b03      	ldr	r3, [sp, #12]
300075b0:	2b00      	cmp	r3, #0
300075b2:	f040 80ad 	bne.w	30007710 <BOOT_Image1+0x88c>
300075b6:	2002      	movs	r0, #2
300075b8:	f000 faba 	bl	30007b30 <ymodem_uart_port_deinit>
300075bc:	4861      	ldr	r0, [pc, #388]	; (30007744 <BOOT_Image1+0x8c0>)
300075be:	f004 fc37 	bl	3000be30 <__DiagPrintf_veneer>
300075c2:	e688      	b.n	300072d6 <BOOT_Image1+0x452>
300075c4:	f000 fadc 	bl	30007b80 <ymodem_uart_readable>
300075c8:	2800      	cmp	r0, #0
300075ca:	d0f4      	beq.n	300075b6 <BOOT_Image1+0x732>
300075cc:	9603      	str	r6, [sp, #12]
300075ce:	e538      	b.n	30007042 <BOOT_Image1+0x1be>
300075d0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300075d4:	495c      	ldr	r1, [pc, #368]	; (30007748 <BOOT_Image1+0x8c4>)
300075d6:	685a      	ldr	r2, [r3, #4]
300075d8:	f042 0202 	orr.w	r2, r2, #2
300075dc:	605a      	str	r2, [r3, #4]
300075de:	685a      	ldr	r2, [r3, #4]
300075e0:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300075e4:	605a      	str	r2, [r3, #4]
300075e6:	6f8b      	ldr	r3, [r1, #120]	; 0x78
300075e8:	f043 0303 	orr.w	r3, r3, #3
300075ec:	678b      	str	r3, [r1, #120]	; 0x78
300075ee:	e692      	b.n	30007316 <BOOT_Image1+0x492>
300075f0:	4856      	ldr	r0, [pc, #344]	; (3000774c <BOOT_Image1+0x8c8>)
300075f2:	f004 fc1d 	bl	3000be30 <__DiagPrintf_veneer>
300075f6:	e6ff      	b.n	300073f8 <BOOT_Image1+0x574>
300075f8:	4855      	ldr	r0, [pc, #340]	; (30007750 <BOOT_Image1+0x8cc>)
300075fa:	f004 fc19 	bl	3000be30 <__DiagPrintf_veneer>
300075fe:	e4ed      	b.n	30006fdc <BOOT_Image1+0x158>
30007600:	6861      	ldr	r1, [r4, #4]
30007602:	4854      	ldr	r0, [pc, #336]	; (30007754 <BOOT_Image1+0x8d0>)
30007604:	f004 fc14 	bl	3000be30 <__DiagPrintf_veneer>
30007608:	6860      	ldr	r0, [r4, #4]
3000760a:	b009      	add	sp, #36	; 0x24
3000760c:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007610:	f7fd bec0 	b.w	30005394 <BOOT_NsStart>
30007614:	4f50      	ldr	r7, [pc, #320]	; (30007758 <BOOT_Image1+0x8d4>)
30007616:	4606      	mov	r6, r0
30007618:	f04f 090b 	mov.w	r9, #11
3000761c:	4682      	mov	sl, r0
3000761e:	f207 4344 	addw	r3, r7, #1092	; 0x444
30007622:	f04f 080a 	mov.w	r8, #10
30007626:	9302      	str	r3, [sp, #8]
30007628:	2004      	movs	r0, #4
3000762a:	f000 fa81 	bl	30007b30 <ymodem_uart_port_deinit>
3000762e:	9a02      	ldr	r2, [sp, #8]
30007630:	9600      	str	r6, [sp, #0]
30007632:	4613      	mov	r3, r2
30007634:	eb02 0008 	add.w	r0, r2, r8
30007638:	f8d7 14d0 	ldr.w	r1, [r7, #1232]	; 0x4d0
3000763c:	4652      	mov	r2, sl
3000763e:	444b      	add	r3, r9
30007640:	f7ff fb7a 	bl	30006d38 <password_hash_check>
30007644:	4680      	mov	r8, r0
30007646:	2800      	cmp	r0, #0
30007648:	d1b5      	bne.n	300075b6 <BOOT_Image1+0x732>
3000764a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000764e:	47a0      	blx	r4
30007650:	4842      	ldr	r0, [pc, #264]	; (3000775c <BOOT_Image1+0x8d8>)
30007652:	f004 fbad 	bl	3000bdb0 <____wrap_strlen_veneer>
30007656:	4601      	mov	r1, r0
30007658:	4840      	ldr	r0, [pc, #256]	; (3000775c <BOOT_Image1+0x8d8>)
3000765a:	f000 faab 	bl	30007bb4 <ymodem_uart_putdata>
3000765e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007662:	47a0      	blx	r4
30007664:	f8d7 24d4 	ldr.w	r2, [r7, #1236]	; 0x4d4
30007668:	493d      	ldr	r1, [pc, #244]	; (30007760 <BOOT_Image1+0x8dc>)
3000766a:	2002      	movs	r0, #2
3000766c:	f000 fb42 	bl	30007cf4 <ymodem_img_download>
30007670:	2032      	movs	r0, #50	; 0x32
30007672:	47a0      	blx	r4
30007674:	f8d7 34d4 	ldr.w	r3, [r7, #1236]	; 0x4d4
30007678:	b1e3      	cbz	r3, 300076b4 <BOOT_Image1+0x830>
3000767a:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000773c <BOOT_Image1+0x8b8>
3000767e:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
30007682:	f859 6b04 	ldr.w	r6, [r9], #4
30007686:	0b36      	lsrs	r6, r6, #12
30007688:	d00c      	beq.n	300076a4 <BOOT_Image1+0x820>
3000768a:	2500      	movs	r5, #0
3000768c:	f8da 1000 	ldr.w	r1, [sl]
30007690:	2002      	movs	r0, #2
30007692:	eb01 3105 	add.w	r1, r1, r5, lsl #12
30007696:	3501      	adds	r5, #1
30007698:	f7fd fc90 	bl	30004fbc <FLASH_Erase_With_Lock>
3000769c:	2032      	movs	r0, #50	; 0x32
3000769e:	47a0      	blx	r4
300076a0:	42ae      	cmp	r6, r5
300076a2:	d1f3      	bne.n	3000768c <BOOT_Image1+0x808>
300076a4:	f108 0801 	add.w	r8, r8, #1
300076a8:	f8d7 34d4 	ldr.w	r3, [r7, #1236]	; 0x4d4
300076ac:	f10a 0a04 	add.w	sl, sl, #4
300076b0:	4543      	cmp	r3, r8
300076b2:	d8e6      	bhi.n	30007682 <BOOT_Image1+0x7fe>
300076b4:	482b      	ldr	r0, [pc, #172]	; (30007764 <BOOT_Image1+0x8e0>)
300076b6:	f004 fbbb 	bl	3000be30 <__DiagPrintf_veneer>
300076ba:	4b2b      	ldr	r3, [pc, #172]	; (30007768 <BOOT_Image1+0x8e4>)
300076bc:	a806      	add	r0, sp, #24
300076be:	4798      	blx	r3
300076c0:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
300076c4:	4829      	ldr	r0, [pc, #164]	; (3000776c <BOOT_Image1+0x8e8>)
300076c6:	a906      	add	r1, sp, #24
300076c8:	4b29      	ldr	r3, [pc, #164]	; (30007770 <BOOT_Image1+0x8ec>)
300076ca:	f8ad 201a 	strh.w	r2, [sp, #26]
300076ce:	4798      	blx	r3
300076d0:	4826      	ldr	r0, [pc, #152]	; (3000776c <BOOT_Image1+0x8e8>)
300076d2:	4b28      	ldr	r3, [pc, #160]	; (30007774 <BOOT_Image1+0x8f0>)
300076d4:	4798      	blx	r3
300076d6:	e7fe      	b.n	300076d6 <BOOT_Image1+0x852>
300076d8:	4827      	ldr	r0, [pc, #156]	; (30007778 <BOOT_Image1+0x8f4>)
300076da:	f004 fba9 	bl	3000be30 <__DiagPrintf_veneer>
300076de:	e5f6      	b.n	300072ce <BOOT_Image1+0x44a>
300076e0:	4826      	ldr	r0, [pc, #152]	; (3000777c <BOOT_Image1+0x8f8>)
300076e2:	f004 fba5 	bl	3000be30 <__DiagPrintf_veneer>
300076e6:	4817      	ldr	r0, [pc, #92]	; (30007744 <BOOT_Image1+0x8c0>)
300076e8:	f004 fba2 	bl	3000be30 <__DiagPrintf_veneer>
300076ec:	e5ef      	b.n	300072ce <BOOT_Image1+0x44a>
300076ee:	4824      	ldr	r0, [pc, #144]	; (30007780 <BOOT_Image1+0x8fc>)
300076f0:	f004 fb9e 	bl	3000be30 <__DiagPrintf_veneer>
300076f4:	e5eb      	b.n	300072ce <BOOT_Image1+0x44a>
300076f6:	f04f 0800 	mov.w	r8, #0
300076fa:	f04f 090a 	mov.w	r9, #10
300076fe:	4643      	mov	r3, r8
30007700:	46c3      	mov	fp, r8
30007702:	e53f      	b.n	30007184 <BOOT_Image1+0x300>
30007704:	46d3      	mov	fp, sl
30007706:	e59b      	b.n	30007240 <BOOT_Image1+0x3bc>
30007708:	2300      	movs	r3, #0
3000770a:	46da      	mov	sl, fp
3000770c:	9305      	str	r3, [sp, #20]
3000770e:	e56d      	b.n	300071ec <BOOT_Image1+0x368>
30007710:	f108 090b 	add.w	r9, r8, #11
30007714:	f8dd a014 	ldr.w	sl, [sp, #20]
30007718:	f108 080a 	add.w	r8, r8, #10
3000771c:	44d1      	add	r9, sl
3000771e:	e783      	b.n	30007628 <BOOT_Image1+0x7a4>
30007720:	0000aea1 	.word	0x0000aea1
30007724:	0000aab9 	.word	0x0000aab9
30007728:	4200c000 	.word	0x4200c000
3000772c:	0000aaa5 	.word	0x0000aaa5
30007730:	3000dd44 	.word	0x3000dd44
30007734:	3000dd6c 	.word	0x3000dd6c
30007738:	000129bd 	.word	0x000129bd
3000773c:	3000dd70 	.word	0x3000dd70
30007740:	3000dd98 	.word	0x3000dd98
30007744:	3000cbdc 	.word	0x3000cbdc
30007748:	41000200 	.word	0x41000200
3000774c:	3000cb18 	.word	0x3000cb18
30007750:	3000cba0 	.word	0x3000cba0
30007754:	3000cc34 	.word	0x3000cc34
30007758:	3000d828 	.word	0x3000d828
3000775c:	3000e550 	.word	0x3000e550
30007760:	3000dd04 	.word	0x3000dd04
30007764:	3000cc14 	.word	0x3000cc14
30007768:	0000d3f9 	.word	0x0000d3f9
3000776c:	41000440 	.word	0x41000440
30007770:	0000d409 	.word	0x0000d409
30007774:	0000d489 	.word	0x0000d489
30007778:	3000cbec 	.word	0x3000cbec
3000777c:	3000cbc4 	.word	0x3000cbc4
30007780:	3000cc00 	.word	0x3000cc00
30007784:	0000c0f9 	.word	0x0000c0f9

30007788 <ymodem_inquiry>:
30007788:	2806      	cmp	r0, #6
3000778a:	b510      	push	{r4, lr}
3000778c:	4604      	mov	r4, r0
3000778e:	d003      	beq.n	30007798 <ymodem_inquiry+0x10>
30007790:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007794:	4b03      	ldr	r3, [pc, #12]	; (300077a4 <ymodem_inquiry+0x1c>)
30007796:	4798      	blx	r3
30007798:	4620      	mov	r0, r4
3000779a:	f000 fa07 	bl	30007bac <ymodem_uart_putc>
3000779e:	2001      	movs	r0, #1
300077a0:	bd10      	pop	{r4, pc}
300077a2:	bf00      	nop
300077a4:	00009b2d 	.word	0x00009b2d

300077a8 <ymodem_get_first>:
300077a8:	b530      	push	{r4, r5, lr}
300077aa:	b083      	sub	sp, #12
300077ac:	2300      	movs	r3, #0
300077ae:	4604      	mov	r4, r0
300077b0:	4936      	ldr	r1, [pc, #216]	; (3000788c <ymodem_get_first+0xe4>)
300077b2:	f10d 0006 	add.w	r0, sp, #6
300077b6:	f88d 3006 	strb.w	r3, [sp, #6]
300077ba:	f000 fa19 	bl	30007bf0 <ymodem_uart_getc_to>
300077be:	b9d0      	cbnz	r0, 300077f6 <ymodem_get_first+0x4e>
300077c0:	f89d 3006 	ldrb.w	r3, [sp, #6]
300077c4:	3b01      	subs	r3, #1
300077c6:	2b17      	cmp	r3, #23
300077c8:	d80d      	bhi.n	300077e6 <ymodem_get_first+0x3e>
300077ca:	e8df f003 	tbb	[pc, r3]
300077ce:	231e      	.short	0x231e
300077d0:	0c0c290c 	.word	0x0c0c290c
300077d4:	0c0c0c0c 	.word	0x0c0c0c0c
300077d8:	0c0c0c0c 	.word	0x0c0c0c0c
300077dc:	0c0c0c0c 	.word	0x0c0c0c0c
300077e0:	0c0c0c0c 	.word	0x0c0c0c0c
300077e4:	190c      	.short	0x190c
300077e6:	4d29      	ldr	r5, [pc, #164]	; (3000788c <ymodem_get_first+0xe4>)
300077e8:	4629      	mov	r1, r5
300077ea:	f10d 0007 	add.w	r0, sp, #7
300077ee:	f000 f9ff 	bl	30007bf0 <ymodem_uart_getc_to>
300077f2:	2800      	cmp	r0, #0
300077f4:	d0f8      	beq.n	300077e8 <ymodem_get_first+0x40>
300077f6:	2301      	movs	r3, #1
300077f8:	2008      	movs	r0, #8
300077fa:	60a3      	str	r3, [r4, #8]
300077fc:	b003      	add	sp, #12
300077fe:	bd30      	pop	{r4, r5, pc}
30007800:	2301      	movs	r3, #1
30007802:	2002      	movs	r0, #2
30007804:	60a3      	str	r3, [r4, #8]
30007806:	b003      	add	sp, #12
30007808:	bd30      	pop	{r4, r5, pc}
3000780a:	2380      	movs	r3, #128	; 0x80
3000780c:	2001      	movs	r0, #1
3000780e:	80a3      	strh	r3, [r4, #4]
30007810:	b003      	add	sp, #12
30007812:	bd30      	pop	{r4, r5, pc}
30007814:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007818:	2001      	movs	r0, #1
3000781a:	80a3      	strh	r3, [r4, #4]
3000781c:	b003      	add	sp, #12
3000781e:	bd30      	pop	{r4, r5, pc}
30007820:	2006      	movs	r0, #6
30007822:	4d1b      	ldr	r5, [pc, #108]	; (30007890 <ymodem_get_first+0xe8>)
30007824:	f000 f9c2 	bl	30007bac <ymodem_uart_putc>
30007828:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000782c:	47a8      	blx	r5
3000782e:	2043      	movs	r0, #67	; 0x43
30007830:	f000 f9bc 	bl	30007bac <ymodem_uart_putc>
30007834:	2006      	movs	r0, #6
30007836:	f000 f9b9 	bl	30007bac <ymodem_uart_putc>
3000783a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000783e:	47a8      	blx	r5
30007840:	2043      	movs	r0, #67	; 0x43
30007842:	f000 f9b3 	bl	30007bac <ymodem_uart_putc>
30007846:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000784a:	47a8      	blx	r5
3000784c:	2018      	movs	r0, #24
3000784e:	f000 f9ad 	bl	30007bac <ymodem_uart_putc>
30007852:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007856:	47a8      	blx	r5
30007858:	2018      	movs	r0, #24
3000785a:	f000 f9a7 	bl	30007bac <ymodem_uart_putc>
3000785e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007862:	47a8      	blx	r5
30007864:	2018      	movs	r0, #24
30007866:	f000 f9a1 	bl	30007bac <ymodem_uart_putc>
3000786a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000786e:	47a8      	blx	r5
30007870:	2018      	movs	r0, #24
30007872:	f000 f99b 	bl	30007bac <ymodem_uart_putc>
30007876:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000787a:	47a8      	blx	r5
3000787c:	2018      	movs	r0, #24
3000787e:	f000 f995 	bl	30007bac <ymodem_uart_putc>
30007882:	2301      	movs	r3, #1
30007884:	2005      	movs	r0, #5
30007886:	60a3      	str	r3, [r4, #8]
30007888:	b003      	add	sp, #12
3000788a:	bd30      	pop	{r4, r5, pc}
3000788c:	000caa30 	.word	0x000caa30
30007890:	00009b2d 	.word	0x00009b2d

30007894 <ymodem_get_others>:
30007894:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007898:	b083      	sub	sp, #12
3000789a:	2300      	movs	r3, #0
3000789c:	4605      	mov	r5, r0
3000789e:	4952      	ldr	r1, [pc, #328]	; (300079e8 <ymodem_get_others+0x154>)
300078a0:	a801      	add	r0, sp, #4
300078a2:	f88d 3006 	strb.w	r3, [sp, #6]
300078a6:	f88d 3007 	strb.w	r3, [sp, #7]
300078aa:	f000 f9a1 	bl	30007bf0 <ymodem_uart_getc_to>
300078ae:	bb38      	cbnz	r0, 30007900 <ymodem_get_others+0x6c>
300078b0:	494d      	ldr	r1, [pc, #308]	; (300079e8 <ymodem_get_others+0x154>)
300078b2:	f10d 0005 	add.w	r0, sp, #5
300078b6:	f000 f99b 	bl	30007bf0 <ymodem_uart_getc_to>
300078ba:	4607      	mov	r7, r0
300078bc:	bb00      	cbnz	r0, 30007900 <ymodem_get_others+0x6c>
300078be:	f89d 2004 	ldrb.w	r2, [sp, #4]
300078c2:	f89d 6005 	ldrb.w	r6, [sp, #5]
300078c6:	43d3      	mvns	r3, r2
300078c8:	b2db      	uxtb	r3, r3
300078ca:	429e      	cmp	r6, r3
300078cc:	bf0c      	ite	eq
300078ce:	2601      	moveq	r6, #1
300078d0:	2604      	movne	r6, #4
300078d2:	682b      	ldr	r3, [r5, #0]
300078d4:	429a      	cmp	r2, r3
300078d6:	d21a      	bcs.n	3000790e <ymodem_get_others+0x7a>
300078d8:	2701      	movs	r7, #1
300078da:	88ab      	ldrh	r3, [r5, #4]
300078dc:	b1d3      	cbz	r3, 30007914 <ymodem_get_others+0x80>
300078de:	2400      	movs	r4, #0
300078e0:	f8df 9108 	ldr.w	r9, [pc, #264]	; 300079ec <ymodem_get_others+0x158>
300078e4:	f8df 8100 	ldr.w	r8, [pc, #256]	; 300079e8 <ymodem_get_others+0x154>
300078e8:	e002      	b.n	300078f0 <ymodem_get_others+0x5c>
300078ea:	88ab      	ldrh	r3, [r5, #4]
300078ec:	42a3      	cmp	r3, r4
300078ee:	d911      	bls.n	30007914 <ymodem_get_others+0x80>
300078f0:	eb04 0009 	add.w	r0, r4, r9
300078f4:	4641      	mov	r1, r8
300078f6:	3401      	adds	r4, #1
300078f8:	f000 f97a 	bl	30007bf0 <ymodem_uart_getc_to>
300078fc:	2800      	cmp	r0, #0
300078fe:	d0f4      	beq.n	300078ea <ymodem_get_others+0x56>
30007900:	2301      	movs	r3, #1
30007902:	2608      	movs	r6, #8
30007904:	60ab      	str	r3, [r5, #8]
30007906:	4630      	mov	r0, r6
30007908:	b003      	add	sp, #12
3000790a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000790e:	d0e4      	beq.n	300078da <ymodem_get_others+0x46>
30007910:	2604      	movs	r6, #4
30007912:	e7e2      	b.n	300078da <ymodem_get_others+0x46>
30007914:	4934      	ldr	r1, [pc, #208]	; (300079e8 <ymodem_get_others+0x154>)
30007916:	f10d 0006 	add.w	r0, sp, #6
3000791a:	f000 f969 	bl	30007bf0 <ymodem_uart_getc_to>
3000791e:	2800      	cmp	r0, #0
30007920:	d1ee      	bne.n	30007900 <ymodem_get_others+0x6c>
30007922:	4931      	ldr	r1, [pc, #196]	; (300079e8 <ymodem_get_others+0x154>)
30007924:	f10d 0007 	add.w	r0, sp, #7
30007928:	f000 f962 	bl	30007bf0 <ymodem_uart_getc_to>
3000792c:	2800      	cmp	r0, #0
3000792e:	d1e7      	bne.n	30007900 <ymodem_get_others+0x6c>
30007930:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007934:	f89d e007 	ldrb.w	lr, [sp, #7]
30007938:	88aa      	ldrh	r2, [r5, #4]
3000793a:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
3000793e:	b36a      	cbz	r2, 3000799c <ymodem_get_others+0x108>
30007940:	4603      	mov	r3, r0
30007942:	482a      	ldr	r0, [pc, #168]	; (300079ec <ymodem_get_others+0x158>)
30007944:	f241 0c21 	movw	ip, #4129	; 0x1021
30007948:	eb02 0800 	add.w	r8, r2, r0
3000794c:	7801      	ldrb	r1, [r0, #0]
3000794e:	3001      	adds	r0, #1
30007950:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007954:	2108      	movs	r1, #8
30007956:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
3000795a:	005b      	lsls	r3, r3, #1
3000795c:	b10c      	cbz	r4, 30007962 <ymodem_get_others+0xce>
3000795e:	ea83 030c 	eor.w	r3, r3, ip
30007962:	3901      	subs	r1, #1
30007964:	d1f7      	bne.n	30007956 <ymodem_get_others+0xc2>
30007966:	4580      	cmp	r8, r0
30007968:	d1f0      	bne.n	3000794c <ymodem_get_others+0xb8>
3000796a:	b29b      	uxth	r3, r3
3000796c:	459e      	cmp	lr, r3
3000796e:	d00a      	beq.n	30007986 <ymodem_get_others+0xf2>
30007970:	4b1f      	ldr	r3, [pc, #124]	; (300079f0 <ymodem_get_others+0x15c>)
30007972:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007976:	4798      	blx	r3
30007978:	2015      	movs	r0, #21
3000797a:	f000 f917 	bl	30007bac <ymodem_uart_putc>
3000797e:	2301      	movs	r3, #1
30007980:	2604      	movs	r6, #4
30007982:	60ab      	str	r3, [r5, #8]
30007984:	e7bf      	b.n	30007906 <ymodem_get_others+0x72>
30007986:	b917      	cbnz	r7, 3000798e <ymodem_get_others+0xfa>
30007988:	4c1a      	ldr	r4, [pc, #104]	; (300079f4 <ymodem_get_others+0x160>)
3000798a:	6823      	ldr	r3, [r4, #0]
3000798c:	b9b3      	cbnz	r3, 300079bc <ymodem_get_others+0x128>
3000798e:	f000 f947 	bl	30007c20 <ymodem_uart_clean_rx>
30007992:	2e01      	cmp	r6, #1
30007994:	d004      	beq.n	300079a0 <ymodem_get_others+0x10c>
30007996:	2e04      	cmp	r6, #4
30007998:	d1b5      	bne.n	30007906 <ymodem_get_others+0x72>
3000799a:	e7e9      	b.n	30007970 <ymodem_get_others+0xdc>
3000799c:	4613      	mov	r3, r2
3000799e:	e7e5      	b.n	3000796c <ymodem_get_others+0xd8>
300079a0:	4c14      	ldr	r4, [pc, #80]	; (300079f4 <ymodem_get_others+0x160>)
300079a2:	2006      	movs	r0, #6
300079a4:	60ae      	str	r6, [r5, #8]
300079a6:	f000 f901 	bl	30007bac <ymodem_uart_putc>
300079aa:	6823      	ldr	r3, [r4, #0]
300079ac:	b193      	cbz	r3, 300079d4 <ymodem_get_others+0x140>
300079ae:	2f00      	cmp	r7, #0
300079b0:	f103 0301 	add.w	r3, r3, #1
300079b4:	bf18      	it	ne
300079b6:	2604      	movne	r6, #4
300079b8:	6023      	str	r3, [r4, #0]
300079ba:	e7a4      	b.n	30007906 <ymodem_get_others+0x72>
300079bc:	68eb      	ldr	r3, [r5, #12]
300079be:	6861      	ldr	r1, [r4, #4]
300079c0:	3301      	adds	r3, #1
300079c2:	480a      	ldr	r0, [pc, #40]	; (300079ec <ymodem_get_others+0x158>)
300079c4:	60eb      	str	r3, [r5, #12]
300079c6:	f000 f94f 	bl	30007c68 <UARTIMG_Write>
300079ca:	6862      	ldr	r2, [r4, #4]
300079cc:	88ab      	ldrh	r3, [r5, #4]
300079ce:	4413      	add	r3, r2
300079d0:	6063      	str	r3, [r4, #4]
300079d2:	e7dc      	b.n	3000798e <ymodem_get_others+0xfa>
300079d4:	4b06      	ldr	r3, [pc, #24]	; (300079f0 <ymodem_get_others+0x15c>)
300079d6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079da:	4798      	blx	r3
300079dc:	2043      	movs	r0, #67	; 0x43
300079de:	f000 f8e5 	bl	30007bac <ymodem_uart_putc>
300079e2:	6823      	ldr	r3, [r4, #0]
300079e4:	e7e3      	b.n	300079ae <ymodem_get_others+0x11a>
300079e6:	bf00      	nop
300079e8:	000caa30 	.word	0x000caa30
300079ec:	20016ffc 	.word	0x20016ffc
300079f0:	00009b2d 	.word	0x00009b2d
300079f4:	3000dda8 	.word	0x3000dda8

300079f8 <ymodem_rx_frame>:
300079f8:	2300      	movs	r3, #0
300079fa:	2201      	movs	r2, #1
300079fc:	b510      	push	{r4, lr}
300079fe:	4604      	mov	r4, r0
30007a00:	6083      	str	r3, [r0, #8]
30007a02:	8082      	strh	r2, [r0, #4]
30007a04:	f7ff fed0 	bl	300077a8 <ymodem_get_first>
30007a08:	68a3      	ldr	r3, [r4, #8]
30007a0a:	b92b      	cbnz	r3, 30007a18 <ymodem_rx_frame+0x20>
30007a0c:	4620      	mov	r0, r4
30007a0e:	f7ff ff41 	bl	30007894 <ymodem_get_others>
30007a12:	68a3      	ldr	r3, [r4, #8]
30007a14:	2b00      	cmp	r3, #0
30007a16:	d0f9      	beq.n	30007a0c <ymodem_rx_frame+0x14>
30007a18:	2801      	cmp	r0, #1
30007a1a:	d000      	beq.n	30007a1e <ymodem_rx_frame+0x26>
30007a1c:	bd10      	pop	{r4, pc}
30007a1e:	6823      	ldr	r3, [r4, #0]
30007a20:	3301      	adds	r3, #1
30007a22:	b2db      	uxtb	r3, r3
30007a24:	6023      	str	r3, [r4, #0]
30007a26:	bd10      	pop	{r4, pc}

30007a28 <ymodem_img_rxbuffer>:
30007a28:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007a2c:	2300      	movs	r3, #0
30007a2e:	4c28      	ldr	r4, [pc, #160]	; (30007ad0 <ymodem_img_rxbuffer+0xa8>)
30007a30:	4e28      	ldr	r6, [pc, #160]	; (30007ad4 <ymodem_img_rxbuffer+0xac>)
30007a32:	460d      	mov	r5, r1
30007a34:	60a3      	str	r3, [r4, #8]
30007a36:	6163      	str	r3, [r4, #20]
30007a38:	f000 f8f2 	bl	30007c20 <ymodem_uart_clean_rx>
30007a3c:	2043      	movs	r0, #67	; 0x43
30007a3e:	f000 f8b5 	bl	30007bac <ymodem_uart_putc>
30007a42:	f24c 3050 	movw	r0, #50000	; 0xc350
30007a46:	47b0      	blx	r6
30007a48:	f000 f89a 	bl	30007b80 <ymodem_uart_readable>
30007a4c:	2800      	cmp	r0, #0
30007a4e:	d0f5      	beq.n	30007a3c <ymodem_img_rxbuffer+0x14>
30007a50:	2700      	movs	r7, #0
30007a52:	f04f 0801 	mov.w	r8, #1
30007a56:	e9c4 7500 	strd	r7, r5, [r4]
30007a5a:	4d1f      	ldr	r5, [pc, #124]	; (30007ad8 <ymodem_img_rxbuffer+0xb0>)
30007a5c:	4628      	mov	r0, r5
30007a5e:	f8a4 800c 	strh.w	r8, [r4, #12]
30007a62:	6127      	str	r7, [r4, #16]
30007a64:	f7ff fea0 	bl	300077a8 <ymodem_get_first>
30007a68:	6923      	ldr	r3, [r4, #16]
30007a6a:	b92b      	cbnz	r3, 30007a78 <ymodem_img_rxbuffer+0x50>
30007a6c:	4628      	mov	r0, r5
30007a6e:	f7ff ff11 	bl	30007894 <ymodem_get_others>
30007a72:	6923      	ldr	r3, [r4, #16]
30007a74:	2b00      	cmp	r3, #0
30007a76:	d0f9      	beq.n	30007a6c <ymodem_img_rxbuffer+0x44>
30007a78:	2801      	cmp	r0, #1
30007a7a:	d024      	beq.n	30007ac6 <ymodem_img_rxbuffer+0x9e>
30007a7c:	3804      	subs	r0, #4
30007a7e:	2804      	cmp	r0, #4
30007a80:	d80c      	bhi.n	30007a9c <ymodem_img_rxbuffer+0x74>
30007a82:	a301      	add	r3, pc, #4	; (adr r3, 30007a88 <ymodem_img_rxbuffer+0x60>)
30007a84:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007a88:	30007a5d 	.word	0x30007a5d
30007a8c:	30007aa9 	.word	0x30007aa9
30007a90:	30007a9d 	.word	0x30007a9d
30007a94:	30007a9d 	.word	0x30007a9d
30007a98:	30007ab5 	.word	0x30007ab5
30007a9c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007aa0:	47b0      	blx	r6
30007aa2:	2018      	movs	r0, #24
30007aa4:	f000 f882 	bl	30007bac <ymodem_uart_putc>
30007aa8:	200a      	movs	r0, #10
30007aaa:	f000 f8c9 	bl	30007c40 <ymodem_uart_waitbusy>
30007aae:	2001      	movs	r0, #1
30007ab0:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007ab4:	f000 f8b4 	bl	30007c20 <ymodem_uart_clean_rx>
30007ab8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007abc:	47b0      	blx	r6
30007abe:	2015      	movs	r0, #21
30007ac0:	f000 f874 	bl	30007bac <ymodem_uart_putc>
30007ac4:	e7ca      	b.n	30007a5c <ymodem_img_rxbuffer+0x34>
30007ac6:	68a3      	ldr	r3, [r4, #8]
30007ac8:	3301      	adds	r3, #1
30007aca:	b2db      	uxtb	r3, r3
30007acc:	60a3      	str	r3, [r4, #8]
30007ace:	e7c5      	b.n	30007a5c <ymodem_img_rxbuffer+0x34>
30007ad0:	3000dda8 	.word	0x3000dda8
30007ad4:	00009b2d 	.word	0x00009b2d
30007ad8:	3000ddb0 	.word	0x3000ddb0

30007adc <ymodem_log_open>:
30007adc:	b510      	push	{r4, lr}
30007ade:	2303      	movs	r3, #3
30007ae0:	4c04      	ldr	r4, [pc, #16]	; (30007af4 <ymodem_log_open+0x18>)
30007ae2:	2200      	movs	r2, #0
30007ae4:	2108      	movs	r1, #8
30007ae6:	4804      	ldr	r0, [pc, #16]	; (30007af8 <ymodem_log_open+0x1c>)
30007ae8:	47a0      	blx	r4
30007aea:	2008      	movs	r0, #8
30007aec:	4b03      	ldr	r3, [pc, #12]	; (30007afc <ymodem_log_open+0x20>)
30007aee:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007af2:	4718      	bx	r3
30007af4:	3000a0d1 	.word	0x3000a0d1
30007af8:	0000ed39 	.word	0x0000ed39
30007afc:	0000e1bd 	.word	0x0000e1bd

30007b00 <ymodem_uart_port_init>:
30007b00:	b500      	push	{lr}
30007b02:	b083      	sub	sp, #12
30007b04:	2008      	movs	r0, #8
30007b06:	4b06      	ldr	r3, [pc, #24]	; (30007b20 <ymodem_uart_port_init+0x20>)
30007b08:	9201      	str	r2, [sp, #4]
30007b0a:	4798      	blx	r3
30007b0c:	4b05      	ldr	r3, [pc, #20]	; (30007b24 <ymodem_uart_port_init+0x24>)
30007b0e:	2008      	movs	r0, #8
30007b10:	4798      	blx	r3
30007b12:	9901      	ldr	r1, [sp, #4]
30007b14:	4804      	ldr	r0, [pc, #16]	; (30007b28 <ymodem_uart_port_init+0x28>)
30007b16:	4b05      	ldr	r3, [pc, #20]	; (30007b2c <ymodem_uart_port_init+0x2c>)
30007b18:	b003      	add	sp, #12
30007b1a:	f85d eb04 	ldr.w	lr, [sp], #4
30007b1e:	4718      	bx	r3
30007b20:	0000e1d9 	.word	0x0000e1d9
30007b24:	0000e2fd 	.word	0x0000e2fd
30007b28:	4200c000 	.word	0x4200c000
30007b2c:	0000a9e5 	.word	0x0000a9e5

30007b30 <ymodem_uart_port_deinit>:
30007b30:	2804      	cmp	r0, #4
30007b32:	d004      	beq.n	30007b3e <ymodem_uart_port_deinit+0xe>
30007b34:	4b0d      	ldr	r3, [pc, #52]	; (30007b6c <ymodem_uart_port_deinit+0x3c>)
30007b36:	681b      	ldr	r3, [r3, #0]
30007b38:	039b      	lsls	r3, r3, #14
30007b3a:	d40f      	bmi.n	30007b5c <ymodem_uart_port_deinit+0x2c>
30007b3c:	4770      	bx	lr
30007b3e:	2200      	movs	r2, #0
30007b40:	2303      	movs	r3, #3
30007b42:	2108      	movs	r1, #8
30007b44:	480a      	ldr	r0, [pc, #40]	; (30007b70 <ymodem_uart_port_deinit+0x40>)
30007b46:	b510      	push	{r4, lr}
30007b48:	4c0a      	ldr	r4, [pc, #40]	; (30007b74 <ymodem_uart_port_deinit+0x44>)
30007b4a:	47a0      	blx	r4
30007b4c:	4b0a      	ldr	r3, [pc, #40]	; (30007b78 <ymodem_uart_port_deinit+0x48>)
30007b4e:	2008      	movs	r0, #8
30007b50:	4798      	blx	r3
30007b52:	4b06      	ldr	r3, [pc, #24]	; (30007b6c <ymodem_uart_port_deinit+0x3c>)
30007b54:	681b      	ldr	r3, [r3, #0]
30007b56:	039a      	lsls	r2, r3, #14
30007b58:	d403      	bmi.n	30007b62 <ymodem_uart_port_deinit+0x32>
30007b5a:	bd10      	pop	{r4, pc}
30007b5c:	4807      	ldr	r0, [pc, #28]	; (30007b7c <ymodem_uart_port_deinit+0x4c>)
30007b5e:	f004 b967 	b.w	3000be30 <__DiagPrintf_veneer>
30007b62:	4806      	ldr	r0, [pc, #24]	; (30007b7c <ymodem_uart_port_deinit+0x4c>)
30007b64:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b68:	f004 b962 	b.w	3000be30 <__DiagPrintf_veneer>
30007b6c:	2001c00c 	.word	0x2001c00c
30007b70:	0000ed39 	.word	0x0000ed39
30007b74:	3000a0d1 	.word	0x3000a0d1
30007b78:	0000e1bd 	.word	0x0000e1bd
30007b7c:	3000cc6c 	.word	0x3000cc6c

30007b80 <ymodem_uart_readable>:
30007b80:	4b00      	ldr	r3, [pc, #0]	; (30007b84 <ymodem_uart_readable+0x4>)
30007b82:	4718      	bx	r3
30007b84:	0000ac39 	.word	0x0000ac39

30007b88 <ymodem_uart_writable>:
30007b88:	4a03      	ldr	r2, [pc, #12]	; (30007b98 <ymodem_uart_writable+0x10>)
30007b8a:	b508      	push	{r3, lr}
30007b8c:	4b03      	ldr	r3, [pc, #12]	; (30007b9c <ymodem_uart_writable+0x14>)
30007b8e:	6810      	ldr	r0, [r2, #0]
30007b90:	4798      	blx	r3
30007b92:	b2c0      	uxtb	r0, r0
30007b94:	bd08      	pop	{r3, pc}
30007b96:	bf00      	nop
30007b98:	3000ddc0 	.word	0x3000ddc0
30007b9c:	0000d1a9 	.word	0x0000d1a9

30007ba0 <ymodem_uart_getc>:
30007ba0:	2000      	movs	r0, #0
30007ba2:	4b01      	ldr	r3, [pc, #4]	; (30007ba8 <ymodem_uart_getc+0x8>)
30007ba4:	4718      	bx	r3
30007ba6:	bf00      	nop
30007ba8:	0000ac1d 	.word	0x0000ac1d

30007bac <ymodem_uart_putc>:
30007bac:	4b00      	ldr	r3, [pc, #0]	; (30007bb0 <ymodem_uart_putc+0x4>)
30007bae:	4718      	bx	r3
30007bb0:	0000abb5 	.word	0x0000abb5

30007bb4 <ymodem_uart_putdata>:
30007bb4:	b149      	cbz	r1, 30007bca <ymodem_uart_putdata+0x16>
30007bb6:	b570      	push	{r4, r5, r6, lr}
30007bb8:	4604      	mov	r4, r0
30007bba:	1845      	adds	r5, r0, r1
30007bbc:	4e03      	ldr	r6, [pc, #12]	; (30007bcc <ymodem_uart_putdata+0x18>)
30007bbe:	f814 0b01 	ldrb.w	r0, [r4], #1
30007bc2:	47b0      	blx	r6
30007bc4:	42ac      	cmp	r4, r5
30007bc6:	d1fa      	bne.n	30007bbe <ymodem_uart_putdata+0xa>
30007bc8:	bd70      	pop	{r4, r5, r6, pc}
30007bca:	4770      	bx	lr
30007bcc:	0000abb5 	.word	0x0000abb5

30007bd0 <ymodem_uart_getdata>:
30007bd0:	b151      	cbz	r1, 30007be8 <ymodem_uart_getdata+0x18>
30007bd2:	b570      	push	{r4, r5, r6, lr}
30007bd4:	4604      	mov	r4, r0
30007bd6:	1845      	adds	r5, r0, r1
30007bd8:	4e04      	ldr	r6, [pc, #16]	; (30007bec <ymodem_uart_getdata+0x1c>)
30007bda:	2001      	movs	r0, #1
30007bdc:	47b0      	blx	r6
30007bde:	f804 0b01 	strb.w	r0, [r4], #1
30007be2:	42ac      	cmp	r4, r5
30007be4:	d1f9      	bne.n	30007bda <ymodem_uart_getdata+0xa>
30007be6:	bd70      	pop	{r4, r5, r6, pc}
30007be8:	4770      	bx	lr
30007bea:	bf00      	nop
30007bec:	0000ac1d 	.word	0x0000ac1d

30007bf0 <ymodem_uart_getc_to>:
30007bf0:	b570      	push	{r4, r5, r6, lr}
30007bf2:	4d09      	ldr	r5, [pc, #36]	; (30007c18 <ymodem_uart_getc_to+0x28>)
30007bf4:	4606      	mov	r6, r0
30007bf6:	460c      	mov	r4, r1
30007bf8:	e001      	b.n	30007bfe <ymodem_uart_getc_to+0xe>
30007bfa:	3c01      	subs	r4, #1
30007bfc:	d009      	beq.n	30007c12 <ymodem_uart_getc_to+0x22>
30007bfe:	47a8      	blx	r5
30007c00:	2800      	cmp	r0, #0
30007c02:	d0fa      	beq.n	30007bfa <ymodem_uart_getc_to+0xa>
30007c04:	4b05      	ldr	r3, [pc, #20]	; (30007c1c <ymodem_uart_getc_to+0x2c>)
30007c06:	2000      	movs	r0, #0
30007c08:	4798      	blx	r3
30007c0a:	4603      	mov	r3, r0
30007c0c:	2000      	movs	r0, #0
30007c0e:	7033      	strb	r3, [r6, #0]
30007c10:	bd70      	pop	{r4, r5, r6, pc}
30007c12:	2001      	movs	r0, #1
30007c14:	bd70      	pop	{r4, r5, r6, pc}
30007c16:	bf00      	nop
30007c18:	0000ac39 	.word	0x0000ac39
30007c1c:	0000ac1d 	.word	0x0000ac1d

30007c20 <ymodem_uart_clean_rx>:
30007c20:	b538      	push	{r3, r4, r5, lr}
30007c22:	4c05      	ldr	r4, [pc, #20]	; (30007c38 <ymodem_uart_clean_rx+0x18>)
30007c24:	4d05      	ldr	r5, [pc, #20]	; (30007c3c <ymodem_uart_clean_rx+0x1c>)
30007c26:	e000      	b.n	30007c2a <ymodem_uart_clean_rx+0xa>
30007c28:	47a8      	blx	r5
30007c2a:	47a0      	blx	r4
30007c2c:	4603      	mov	r3, r0
30007c2e:	2000      	movs	r0, #0
30007c30:	2b00      	cmp	r3, #0
30007c32:	d1f9      	bne.n	30007c28 <ymodem_uart_clean_rx+0x8>
30007c34:	bd38      	pop	{r3, r4, r5, pc}
30007c36:	bf00      	nop
30007c38:	0000ac39 	.word	0x0000ac39
30007c3c:	0000ac1d 	.word	0x0000ac1d

30007c40 <ymodem_uart_waitbusy>:
30007c40:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007c42:	2400      	movs	r4, #0
30007c44:	4e06      	ldr	r6, [pc, #24]	; (30007c60 <ymodem_uart_waitbusy+0x20>)
30007c46:	4f07      	ldr	r7, [pc, #28]	; (30007c64 <ymodem_uart_waitbusy+0x24>)
30007c48:	4605      	mov	r5, r0
30007c4a:	e002      	b.n	30007c52 <ymodem_uart_waitbusy+0x12>
30007c4c:	47b8      	blx	r7
30007c4e:	42ac      	cmp	r4, r5
30007c50:	d804      	bhi.n	30007c5c <ymodem_uart_waitbusy+0x1c>
30007c52:	6973      	ldr	r3, [r6, #20]
30007c54:	3401      	adds	r4, #1
30007c56:	2064      	movs	r0, #100	; 0x64
30007c58:	03db      	lsls	r3, r3, #15
30007c5a:	d5f7      	bpl.n	30007c4c <ymodem_uart_waitbusy+0xc>
30007c5c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007c5e:	bf00      	nop
30007c60:	4200c000 	.word	0x4200c000
30007c64:	00009b2d 	.word	0x00009b2d

30007c68 <UARTIMG_Write>:
30007c68:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007c6c:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007c70:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007c74:	d21d      	bcs.n	30007cb2 <UARTIMG_Write+0x4a>
30007c76:	3a01      	subs	r2, #1
30007c78:	4f19      	ldr	r7, [pc, #100]	; (30007ce0 <UARTIMG_Write+0x78>)
30007c7a:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007cf0 <UARTIMG_Write+0x88>
30007c7e:	4688      	mov	r8, r1
30007c80:	1916      	adds	r6, r2, r4
30007c82:	4605      	mov	r5, r0
30007c84:	f8d9 3000 	ldr.w	r3, [r9]
30007c88:	403e      	ands	r6, r7
30007c8a:	4027      	ands	r7, r4
30007c8c:	42bb      	cmp	r3, r7
30007c8e:	d91b      	bls.n	30007cc8 <UARTIMG_Write+0x60>
30007c90:	42b3      	cmp	r3, r6
30007c92:	d910      	bls.n	30007cb6 <UARTIMG_Write+0x4e>
30007c94:	4e13      	ldr	r6, [pc, #76]	; (30007ce4 <UARTIMG_Write+0x7c>)
30007c96:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007c9a:	4f13      	ldr	r7, [pc, #76]	; (30007ce8 <UARTIMG_Write+0x80>)
30007c9c:	4446      	add	r6, r8
30007c9e:	eba3 0808 	sub.w	r8, r3, r8
30007ca2:	eb08 0204 	add.w	r2, r8, r4
30007ca6:	4620      	mov	r0, r4
30007ca8:	3408      	adds	r4, #8
30007caa:	2108      	movs	r1, #8
30007cac:	47b8      	blx	r7
30007cae:	42b4      	cmp	r4, r6
30007cb0:	d1f7      	bne.n	30007ca2 <UARTIMG_Write+0x3a>
30007cb2:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007cb6:	4631      	mov	r1, r6
30007cb8:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007cbc:	2002      	movs	r0, #2
30007cbe:	4b0b      	ldr	r3, [pc, #44]	; (30007cec <UARTIMG_Write+0x84>)
30007cc0:	4798      	blx	r3
30007cc2:	f8c9 6000 	str.w	r6, [r9]
30007cc6:	e7e5      	b.n	30007c94 <UARTIMG_Write+0x2c>
30007cc8:	4b08      	ldr	r3, [pc, #32]	; (30007cec <UARTIMG_Write+0x84>)
30007cca:	4639      	mov	r1, r7
30007ccc:	2002      	movs	r0, #2
30007cce:	4798      	blx	r3
30007cd0:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007cd4:	42b3      	cmp	r3, r6
30007cd6:	f8c9 3000 	str.w	r3, [r9]
30007cda:	d8db      	bhi.n	30007c94 <UARTIMG_Write+0x2c>
30007cdc:	e7eb      	b.n	30007cb6 <UARTIMG_Write+0x4e>
30007cde:	bf00      	nop
30007ce0:	fffff000 	.word	0xfffff000
30007ce4:	f8000400 	.word	0xf8000400
30007ce8:	0000a04d 	.word	0x0000a04d
30007cec:	0000a16d 	.word	0x0000a16d
30007cf0:	3000ddc4 	.word	0x3000ddc4

30007cf4 <ymodem_img_download>:
30007cf4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007cf8:	4616      	mov	r6, r2
30007cfa:	460d      	mov	r5, r1
30007cfc:	4a0a      	ldr	r2, [pc, #40]	; (30007d28 <ymodem_img_download+0x34>)
30007cfe:	2100      	movs	r1, #0
30007d00:	4680      	mov	r8, r0
30007d02:	f7ff fefd 	bl	30007b00 <ymodem_uart_port_init>
30007d06:	b156      	cbz	r6, 30007d1e <ymodem_img_download+0x2a>
30007d08:	3d04      	subs	r5, #4
30007d0a:	2400      	movs	r4, #0
30007d0c:	4f06      	ldr	r7, [pc, #24]	; (30007d28 <ymodem_img_download+0x34>)
30007d0e:	3401      	adds	r4, #1
30007d10:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007d14:	4638      	mov	r0, r7
30007d16:	f7ff fe87 	bl	30007a28 <ymodem_img_rxbuffer>
30007d1a:	42b4      	cmp	r4, r6
30007d1c:	d1f7      	bne.n	30007d0e <ymodem_img_download+0x1a>
30007d1e:	4640      	mov	r0, r8
30007d20:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007d24:	f7ff bf04 	b.w	30007b30 <ymodem_uart_port_deinit>
30007d28:	0016e360 	.word	0x0016e360

30007d2c <ChipInfo_Invalid>:
30007d2c:	4d07      	ldr	r5, [pc, #28]	; (30007d4c <ChipInfo_Invalid+0x20>)
30007d2e:	4c08      	ldr	r4, [pc, #32]	; (30007d50 <ChipInfo_Invalid+0x24>)
30007d30:	4e08      	ldr	r6, [pc, #32]	; (30007d54 <ChipInfo_Invalid+0x28>)
30007d32:	b508      	push	{r3, lr}
30007d34:	e002      	b.n	30007d3c <ChipInfo_Invalid+0x10>
30007d36:	f242 7010 	movw	r0, #10000	; 0x2710
30007d3a:	47a0      	blx	r4
30007d3c:	682b      	ldr	r3, [r5, #0]
30007d3e:	039b      	lsls	r3, r3, #14
30007d40:	d5f9      	bpl.n	30007d36 <ChipInfo_Invalid+0xa>
30007d42:	4630      	mov	r0, r6
30007d44:	f004 f874 	bl	3000be30 <__DiagPrintf_veneer>
30007d48:	e7f5      	b.n	30007d36 <ChipInfo_Invalid+0xa>
30007d4a:	bf00      	nop
30007d4c:	2001c00c 	.word	0x2001c00c
30007d50:	00009be5 	.word	0x00009be5
30007d54:	3000cc8c 	.word	0x3000cc8c

30007d58 <ChipInfo_Get>:
30007d58:	b570      	push	{r4, r5, r6, lr}
30007d5a:	4c11      	ldr	r4, [pc, #68]	; (30007da0 <ChipInfo_Get+0x48>)
30007d5c:	7820      	ldrb	r0, [r4, #0]
30007d5e:	28ff      	cmp	r0, #255	; 0xff
30007d60:	d104      	bne.n	30007d6c <ChipInfo_Get+0x14>
30007d62:	4d10      	ldr	r5, [pc, #64]	; (30007da4 <ChipInfo_Get+0x4c>)
30007d64:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007d68:	b108      	cbz	r0, 30007d6e <ChipInfo_Get+0x16>
30007d6a:	7020      	strb	r0, [r4, #0]
30007d6c:	bd70      	pop	{r4, r5, r6, pc}
30007d6e:	4621      	mov	r1, r4
30007d70:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007d74:	f001 f94e 	bl	30009014 <OTP_Read8>
30007d78:	7820      	ldrb	r0, [r4, #0]
30007d7a:	28ff      	cmp	r0, #255	; 0xff
30007d7c:	d002      	beq.n	30007d84 <ChipInfo_Get+0x2c>
30007d7e:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007d82:	bd70      	pop	{r4, r5, r6, pc}
30007d84:	4d08      	ldr	r5, [pc, #32]	; (30007da8 <ChipInfo_Get+0x50>)
30007d86:	4c09      	ldr	r4, [pc, #36]	; (30007dac <ChipInfo_Get+0x54>)
30007d88:	4e09      	ldr	r6, [pc, #36]	; (30007db0 <ChipInfo_Get+0x58>)
30007d8a:	e002      	b.n	30007d92 <ChipInfo_Get+0x3a>
30007d8c:	f242 7010 	movw	r0, #10000	; 0x2710
30007d90:	47a0      	blx	r4
30007d92:	682b      	ldr	r3, [r5, #0]
30007d94:	039b      	lsls	r3, r3, #14
30007d96:	d5f9      	bpl.n	30007d8c <ChipInfo_Get+0x34>
30007d98:	4630      	mov	r0, r6
30007d9a:	f004 f849 	bl	3000be30 <__DiagPrintf_veneer>
30007d9e:	e7f5      	b.n	30007d8c <ChipInfo_Get+0x34>
30007da0:	3000dec0 	.word	0x3000dec0
30007da4:	23020000 	.word	0x23020000
30007da8:	2001c00c 	.word	0x2001c00c
30007dac:	00009be5 	.word	0x00009be5
30007db0:	3000ccac 	.word	0x3000ccac

30007db4 <ChipInfo_MemoryType>:
30007db4:	b510      	push	{r4, lr}
30007db6:	4c11      	ldr	r4, [pc, #68]	; (30007dfc <ChipInfo_MemoryType+0x48>)
30007db8:	47a0      	blx	r4
30007dba:	2802      	cmp	r0, #2
30007dbc:	d019      	beq.n	30007df2 <ChipInfo_MemoryType+0x3e>
30007dbe:	47a0      	blx	r4
30007dc0:	2803      	cmp	r0, #3
30007dc2:	d016      	beq.n	30007df2 <ChipInfo_MemoryType+0x3e>
30007dc4:	4c0e      	ldr	r4, [pc, #56]	; (30007e00 <ChipInfo_MemoryType+0x4c>)
30007dc6:	4b0f      	ldr	r3, [pc, #60]	; (30007e04 <ChipInfo_MemoryType+0x50>)
30007dc8:	4798      	blx	r3
30007dca:	2100      	movs	r1, #0
30007dcc:	23e0      	movs	r3, #224	; 0xe0
30007dce:	4622      	mov	r2, r4
30007dd0:	e004      	b.n	30007ddc <ChipInfo_MemoryType+0x28>
30007dd2:	7a13      	ldrb	r3, [r2, #8]
30007dd4:	3101      	adds	r1, #1
30007dd6:	3206      	adds	r2, #6
30007dd8:	2bff      	cmp	r3, #255	; 0xff
30007dda:	d00c      	beq.n	30007df6 <ChipInfo_MemoryType+0x42>
30007ddc:	4298      	cmp	r0, r3
30007dde:	d1f8      	bne.n	30007dd2 <ChipInfo_MemoryType+0x1e>
30007de0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007de4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007de8:	78cb      	ldrb	r3, [r1, #3]
30007dea:	2b02      	cmp	r3, #2
30007dec:	d001      	beq.n	30007df2 <ChipInfo_MemoryType+0x3e>
30007dee:	2001      	movs	r0, #1
30007df0:	bd10      	pop	{r4, pc}
30007df2:	2002      	movs	r0, #2
30007df4:	bd10      	pop	{r4, pc}
30007df6:	f7ff ff99 	bl	30007d2c <ChipInfo_Invalid>
30007dfa:	bf00      	nop
30007dfc:	0000c0f9 	.word	0x0000c0f9
30007e00:	3000ccd0 	.word	0x3000ccd0
30007e04:	30007d59 	.word	0x30007d59

30007e08 <ChipInfo_DDRType>:
30007e08:	b510      	push	{r4, lr}
30007e0a:	4b0f      	ldr	r3, [pc, #60]	; (30007e48 <ChipInfo_DDRType+0x40>)
30007e0c:	4798      	blx	r3
30007e0e:	4b0f      	ldr	r3, [pc, #60]	; (30007e4c <ChipInfo_DDRType+0x44>)
30007e10:	4604      	mov	r4, r0
30007e12:	4798      	blx	r3
30007e14:	2801      	cmp	r0, #1
30007e16:	d112      	bne.n	30007e3e <ChipInfo_DDRType+0x36>
30007e18:	480d      	ldr	r0, [pc, #52]	; (30007e50 <ChipInfo_DDRType+0x48>)
30007e1a:	2100      	movs	r1, #0
30007e1c:	22e0      	movs	r2, #224	; 0xe0
30007e1e:	4603      	mov	r3, r0
30007e20:	e003      	b.n	30007e2a <ChipInfo_DDRType+0x22>
30007e22:	789a      	ldrb	r2, [r3, #2]
30007e24:	3101      	adds	r1, #1
30007e26:	2aff      	cmp	r2, #255	; 0xff
30007e28:	d00b      	beq.n	30007e42 <ChipInfo_DDRType+0x3a>
30007e2a:	4294      	cmp	r4, r2
30007e2c:	f103 0306 	add.w	r3, r3, #6
30007e30:	d1f7      	bne.n	30007e22 <ChipInfo_DDRType+0x1a>
30007e32:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e36:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007e3a:	78c8      	ldrb	r0, [r1, #3]
30007e3c:	bd10      	pop	{r4, pc}
30007e3e:	20ff      	movs	r0, #255	; 0xff
30007e40:	bd10      	pop	{r4, pc}
30007e42:	2000      	movs	r0, #0
30007e44:	bd10      	pop	{r4, pc}
30007e46:	bf00      	nop
30007e48:	30007d59 	.word	0x30007d59
30007e4c:	30007db5 	.word	0x30007db5
30007e50:	3000ccd0 	.word	0x3000ccd0

30007e54 <ChipInfo_MemorySize>:
30007e54:	b510      	push	{r4, lr}
30007e56:	4c0b      	ldr	r4, [pc, #44]	; (30007e84 <ChipInfo_MemorySize+0x30>)
30007e58:	4b0b      	ldr	r3, [pc, #44]	; (30007e88 <ChipInfo_MemorySize+0x34>)
30007e5a:	4798      	blx	r3
30007e5c:	2100      	movs	r1, #0
30007e5e:	23e0      	movs	r3, #224	; 0xe0
30007e60:	4622      	mov	r2, r4
30007e62:	e004      	b.n	30007e6e <ChipInfo_MemorySize+0x1a>
30007e64:	7a13      	ldrb	r3, [r2, #8]
30007e66:	3101      	adds	r1, #1
30007e68:	3206      	adds	r2, #6
30007e6a:	2bff      	cmp	r3, #255	; 0xff
30007e6c:	d007      	beq.n	30007e7e <ChipInfo_MemorySize+0x2a>
30007e6e:	4298      	cmp	r0, r3
30007e70:	d1f8      	bne.n	30007e64 <ChipInfo_MemorySize+0x10>
30007e72:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e76:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e7a:	7908      	ldrb	r0, [r1, #4]
30007e7c:	bd10      	pop	{r4, pc}
30007e7e:	f7ff ff55 	bl	30007d2c <ChipInfo_Invalid>
30007e82:	bf00      	nop
30007e84:	3000ccd0 	.word	0x3000ccd0
30007e88:	30007d59 	.word	0x30007d59

30007e8c <ChipInfo_MemoryVendor>:
30007e8c:	b510      	push	{r4, lr}
30007e8e:	4c0f      	ldr	r4, [pc, #60]	; (30007ecc <ChipInfo_MemoryVendor+0x40>)
30007e90:	47a0      	blx	r4
30007e92:	2803      	cmp	r0, #3
30007e94:	d016      	beq.n	30007ec4 <ChipInfo_MemoryVendor+0x38>
30007e96:	47a0      	blx	r4
30007e98:	2802      	cmp	r0, #2
30007e9a:	d012      	beq.n	30007ec2 <ChipInfo_MemoryVendor+0x36>
30007e9c:	4c0c      	ldr	r4, [pc, #48]	; (30007ed0 <ChipInfo_MemoryVendor+0x44>)
30007e9e:	4b0d      	ldr	r3, [pc, #52]	; (30007ed4 <ChipInfo_MemoryVendor+0x48>)
30007ea0:	4798      	blx	r3
30007ea2:	2100      	movs	r1, #0
30007ea4:	23e0      	movs	r3, #224	; 0xe0
30007ea6:	4622      	mov	r2, r4
30007ea8:	e004      	b.n	30007eb4 <ChipInfo_MemoryVendor+0x28>
30007eaa:	7a13      	ldrb	r3, [r2, #8]
30007eac:	3101      	adds	r1, #1
30007eae:	3206      	adds	r2, #6
30007eb0:	2bff      	cmp	r3, #255	; 0xff
30007eb2:	d009      	beq.n	30007ec8 <ChipInfo_MemoryVendor+0x3c>
30007eb4:	4298      	cmp	r0, r3
30007eb6:	d1f8      	bne.n	30007eaa <ChipInfo_MemoryVendor+0x1e>
30007eb8:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ebc:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007ec0:	7948      	ldrb	r0, [r1, #5]
30007ec2:	bd10      	pop	{r4, pc}
30007ec4:	2001      	movs	r0, #1
30007ec6:	bd10      	pop	{r4, pc}
30007ec8:	f7ff ff30 	bl	30007d2c <ChipInfo_Invalid>
30007ecc:	0000c0f9 	.word	0x0000c0f9
30007ed0:	3000ccd0 	.word	0x3000ccd0
30007ed4:	30007d59 	.word	0x30007d59

30007ed8 <SDM32K_Enable>:
30007ed8:	4a02      	ldr	r2, [pc, #8]	; (30007ee4 <SDM32K_Enable+0xc>)
30007eda:	4b03      	ldr	r3, [pc, #12]	; (30007ee8 <SDM32K_Enable+0x10>)
30007edc:	6811      	ldr	r1, [r2, #0]
30007ede:	430b      	orrs	r3, r1
30007ee0:	6013      	str	r3, [r2, #0]
30007ee2:	4770      	bx	lr
30007ee4:	42008e00 	.word	0x42008e00
30007ee8:	c0060000 	.word	0xc0060000

30007eec <CLK_SWITCH_XTAL>:
30007eec:	b508      	push	{r3, lr}
30007eee:	2801      	cmp	r0, #1
30007ef0:	4b0a      	ldr	r3, [pc, #40]	; (30007f1c <CLK_SWITCH_XTAL+0x30>)
30007ef2:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007ef6:	d00d      	beq.n	30007f14 <CLK_SWITCH_XTAL+0x28>
30007ef8:	4b09      	ldr	r3, [pc, #36]	; (30007f20 <CLK_SWITCH_XTAL+0x34>)
30007efa:	4313      	orrs	r3, r2
30007efc:	4907      	ldr	r1, [pc, #28]	; (30007f1c <CLK_SWITCH_XTAL+0x30>)
30007efe:	4a09      	ldr	r2, [pc, #36]	; (30007f24 <CLK_SWITCH_XTAL+0x38>)
30007f00:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007f04:	4790      	blx	r2
30007f06:	4a08      	ldr	r2, [pc, #32]	; (30007f28 <CLK_SWITCH_XTAL+0x3c>)
30007f08:	4b08      	ldr	r3, [pc, #32]	; (30007f2c <CLK_SWITCH_XTAL+0x40>)
30007f0a:	fba2 2000 	umull	r2, r0, r2, r0
30007f0e:	0c80      	lsrs	r0, r0, #18
30007f10:	6018      	str	r0, [r3, #0]
30007f12:	bd08      	pop	{r3, pc}
30007f14:	4b06      	ldr	r3, [pc, #24]	; (30007f30 <CLK_SWITCH_XTAL+0x44>)
30007f16:	4013      	ands	r3, r2
30007f18:	e7f0      	b.n	30007efc <CLK_SWITCH_XTAL+0x10>
30007f1a:	bf00      	nop
30007f1c:	42008000 	.word	0x42008000
30007f20:	80000500 	.word	0x80000500
30007f24:	30009da1 	.word	0x30009da1
30007f28:	431bde83 	.word	0x431bde83
30007f2c:	2001c700 	.word	0x2001c700
30007f30:	7ffffaff 	.word	0x7ffffaff

30007f34 <DDR_PHY_ChipInfo_ddrtype>:
30007f34:	b510      	push	{r4, lr}
30007f36:	4c04      	ldr	r4, [pc, #16]	; (30007f48 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007f38:	7820      	ldrb	r0, [r4, #0]
30007f3a:	b100      	cbz	r0, 30007f3e <DDR_PHY_ChipInfo_ddrtype+0xa>
30007f3c:	bd10      	pop	{r4, pc}
30007f3e:	4b03      	ldr	r3, [pc, #12]	; (30007f4c <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007f40:	4798      	blx	r3
30007f42:	7020      	strb	r0, [r4, #0]
30007f44:	bd10      	pop	{r4, pc}
30007f46:	bf00      	nop
30007f48:	3000e561 	.word	0x3000e561
30007f4c:	30007e09 	.word	0x30007e09

30007f50 <DDR_PHY_StructInit>:
30007f50:	f240 2315 	movw	r3, #533	; 0x215
30007f54:	6583      	str	r3, [r0, #88]	; 0x58
30007f56:	4770      	bx	lr

30007f58 <DDR_PHY_DLL_CLK_DIV>:
30007f58:	b570      	push	{r4, r5, r6, lr}
30007f5a:	4d96      	ldr	r5, [pc, #600]	; (300081b4 <DDR_PHY_DLL_CLK_DIV+0x25c>)
30007f5c:	4b96      	ldr	r3, [pc, #600]	; (300081b8 <DDR_PHY_DLL_CLK_DIV+0x260>)
30007f5e:	fba5 4200 	umull	r4, r2, r5, r0
30007f62:	4996      	ldr	r1, [pc, #600]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
30007f64:	4403      	add	r3, r0
30007f66:	4c96      	ldr	r4, [pc, #600]	; (300081c0 <DDR_PHY_DLL_CLK_DIV+0x268>)
30007f68:	0952      	lsrs	r2, r2, #5
30007f6a:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30007f6c:	4895      	ldr	r0, [pc, #596]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x26c>)
30007f6e:	3a03      	subs	r2, #3
30007f70:	4034      	ands	r4, r6
30007f72:	eb02 0c82 	add.w	ip, r2, r2, lsl #2
30007f76:	eba3 03cc 	sub.w	r3, r3, ip, lsl #3
30007f7a:	02db      	lsls	r3, r3, #11
30007f7c:	fba5 5303 	umull	r5, r3, r5, r3
30007f80:	2501      	movs	r5, #1
30007f82:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30007f86:	f3c3 134a 	ubfx	r3, r3, #5, #11
30007f8a:	4320      	orrs	r0, r4
30007f8c:	0214      	lsls	r4, r2, #8
30007f8e:	b2d2      	uxtb	r2, r2
30007f90:	6248      	str	r0, [r1, #36]	; 0x24
30007f92:	b2a4      	uxth	r4, r4
30007f94:	6a88      	ldr	r0, [r1, #40]	; 0x28
30007f96:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30007f9a:	4304      	orrs	r4, r0
30007f9c:	488a      	ldr	r0, [pc, #552]	; (300081c8 <DDR_PHY_DLL_CLK_DIV+0x270>)
30007f9e:	628c      	str	r4, [r1, #40]	; 0x28
30007fa0:	6a4c      	ldr	r4, [r1, #36]	; 0x24
30007fa2:	4020      	ands	r0, r4
30007fa4:	4c89      	ldr	r4, [pc, #548]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x274>)
30007fa6:	4303      	orrs	r3, r0
30007fa8:	624b      	str	r3, [r1, #36]	; 0x24
30007faa:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30007fac:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007fb0:	431a      	orrs	r2, r3
30007fb2:	628a      	str	r2, [r1, #40]	; 0x28
30007fb4:	678d      	str	r5, [r1, #120]	; 0x78
30007fb6:	7820      	ldrb	r0, [r4, #0]
30007fb8:	2800      	cmp	r0, #0
30007fba:	f000 8083 	beq.w	300080c4 <DDR_PHY_DLL_CLK_DIV+0x16c>
30007fbe:	28e0      	cmp	r0, #224	; 0xe0
30007fc0:	d15a      	bne.n	30008078 <DDR_PHY_DLL_CLK_DIV+0x120>
30007fc2:	4b7e      	ldr	r3, [pc, #504]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
30007fc4:	2108      	movs	r1, #8
30007fc6:	4c82      	ldr	r4, [pc, #520]	; (300081d0 <DDR_PHY_DLL_CLK_DIV+0x278>)
30007fc8:	4a82      	ldr	r2, [pc, #520]	; (300081d4 <DDR_PHY_DLL_CLK_DIV+0x27c>)
30007fca:	4883      	ldr	r0, [pc, #524]	; (300081d8 <DDR_PHY_DLL_CLK_DIV+0x280>)
30007fcc:	611c      	str	r4, [r3, #16]
30007fce:	615a      	str	r2, [r3, #20]
30007fd0:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30007fd4:	2022      	movs	r0, #34	; 0x22
30007fd6:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30007fda:	f04f 3122 	mov.w	r1, #572662306	; 0x22222222
30007fde:	689c      	ldr	r4, [r3, #8]
30007fe0:	4a7e      	ldr	r2, [pc, #504]	; (300081dc <DDR_PHY_DLL_CLK_DIV+0x284>)
30007fe2:	4022      	ands	r2, r4
30007fe4:	f042 0201 	orr.w	r2, r2, #1
30007fe8:	609a      	str	r2, [r3, #8]
30007fea:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30007fee:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30007ff2:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30007ff6:	f8c3 1080 	str.w	r1, [r3, #128]	; 0x80
30007ffa:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30007ffe:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008002:	f022 0277 	bic.w	r2, r2, #119	; 0x77
30008006:	4c76      	ldr	r4, [pc, #472]	; (300081e0 <DDR_PHY_DLL_CLK_DIV+0x288>)
30008008:	f042 0212 	orr.w	r2, r2, #18
3000800c:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008010:	f8c3 0088 	str.w	r0, [r3, #136]	; 0x88
30008014:	f8c3 1094 	str.w	r1, [r3, #148]	; 0x94
30008018:	7820      	ldrb	r0, [r4, #0]
3000801a:	4a68      	ldr	r2, [pc, #416]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000801c:	4971      	ldr	r1, [pc, #452]	; (300081e4 <DDR_PHY_DLL_CLK_DIV+0x28c>)
3000801e:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008022:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
30008026:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000802a:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000802e:	009b      	lsls	r3, r3, #2
30008030:	d519      	bpl.n	30008066 <DDR_PHY_DLL_CLK_DIV+0x10e>
30008032:	2800      	cmp	r0, #0
30008034:	d04e      	beq.n	300080d4 <DDR_PHY_DLL_CLK_DIV+0x17c>
30008036:	2801      	cmp	r0, #1
30008038:	4b60      	ldr	r3, [pc, #384]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000803a:	bf0c      	ite	eq
3000803c:	4a6a      	ldreq	r2, [pc, #424]	; (300081e8 <DDR_PHY_DLL_CLK_DIV+0x290>)
3000803e:	4a6b      	ldrne	r2, [pc, #428]	; (300081ec <DDR_PHY_DLL_CLK_DIV+0x294>)
30008040:	601a      	str	r2, [r3, #0]
30008042:	4b5e      	ldr	r3, [pc, #376]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
30008044:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008048:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000804c:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
30008050:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008054:	699a      	ldr	r2, [r3, #24]
30008056:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
3000805a:	619a      	str	r2, [r3, #24]
3000805c:	699a      	ldr	r2, [r3, #24]
3000805e:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008062:	619a      	str	r2, [r3, #24]
30008064:	bd70      	pop	{r4, r5, r6, pc}
30008066:	2800      	cmp	r0, #0
30008068:	d030      	beq.n	300080cc <DDR_PHY_DLL_CLK_DIV+0x174>
3000806a:	2801      	cmp	r0, #1
3000806c:	4b53      	ldr	r3, [pc, #332]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000806e:	bf0c      	ite	eq
30008070:	4a5f      	ldreq	r2, [pc, #380]	; (300081f0 <DDR_PHY_DLL_CLK_DIV+0x298>)
30008072:	4a60      	ldrne	r2, [pc, #384]	; (300081f4 <DDR_PHY_DLL_CLK_DIV+0x29c>)
30008074:	601a      	str	r2, [r3, #0]
30008076:	e7e4      	b.n	30008042 <DDR_PHY_DLL_CLK_DIV+0xea>
30008078:	2800      	cmp	r0, #0
3000807a:	d062      	beq.n	30008142 <DDR_PHY_DLL_CLK_DIV+0x1ea>
3000807c:	2802      	cmp	r0, #2
3000807e:	d12d      	bne.n	300080dc <DDR_PHY_DLL_CLK_DIV+0x184>
30008080:	4b4e      	ldr	r3, [pc, #312]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
30008082:	220e      	movs	r2, #14
30008084:	495c      	ldr	r1, [pc, #368]	; (300081f8 <DDR_PHY_DLL_CLK_DIV+0x2a0>)
30008086:	485d      	ldr	r0, [pc, #372]	; (300081fc <DDR_PHY_DLL_CLK_DIV+0x2a4>)
30008088:	4c5d      	ldr	r4, [pc, #372]	; (30008200 <DDR_PHY_DLL_CLK_DIV+0x2a8>)
3000808a:	611c      	str	r4, [r3, #16]
3000808c:	6159      	str	r1, [r3, #20]
3000808e:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008092:	4952      	ldr	r1, [pc, #328]	; (300081dc <DDR_PHY_DLL_CLK_DIV+0x284>)
30008094:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008098:	f240 32c1 	movw	r2, #961	; 0x3c1
3000809c:	6898      	ldr	r0, [r3, #8]
3000809e:	4001      	ands	r1, r0
300080a0:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300080a4:	2022      	movs	r0, #34	; 0x22
300080a6:	430a      	orrs	r2, r1
300080a8:	609a      	str	r2, [r3, #8]
300080aa:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
300080ae:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300080b2:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300080b6:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300080ba:	f8c3 4080 	str.w	r4, [r3, #128]	; 0x80
300080be:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300080c2:	e79e      	b.n	30008002 <DDR_PHY_DLL_CLK_DIV+0xaa>
300080c4:	4b4f      	ldr	r3, [pc, #316]	; (30008204 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
300080c6:	4798      	blx	r3
300080c8:	7020      	strb	r0, [r4, #0]
300080ca:	e778      	b.n	30007fbe <DDR_PHY_DLL_CLK_DIV+0x66>
300080cc:	4b4e      	ldr	r3, [pc, #312]	; (30008208 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
300080ce:	4798      	blx	r3
300080d0:	7020      	strb	r0, [r4, #0]
300080d2:	e7ca      	b.n	3000806a <DDR_PHY_DLL_CLK_DIV+0x112>
300080d4:	4b4c      	ldr	r3, [pc, #304]	; (30008208 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
300080d6:	4798      	blx	r3
300080d8:	7020      	strb	r0, [r4, #0]
300080da:	e7ac      	b.n	30008036 <DDR_PHY_DLL_CLK_DIV+0xde>
300080dc:	2805      	cmp	r0, #5
300080de:	d03b      	beq.n	30008158 <DDR_PHY_DLL_CLK_DIV+0x200>
300080e0:	2803      	cmp	r0, #3
300080e2:	f040 80e6 	bne.w	300082b2 <DDR_PHY_DLL_CLK_DIV+0x35a>
300080e6:	4a35      	ldr	r2, [pc, #212]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
300080e8:	230e      	movs	r3, #14
300080ea:	4c48      	ldr	r4, [pc, #288]	; (3000820c <DDR_PHY_DLL_CLK_DIV+0x2b4>)
300080ec:	4948      	ldr	r1, [pc, #288]	; (30008210 <DDR_PHY_DLL_CLK_DIV+0x2b8>)
300080ee:	4849      	ldr	r0, [pc, #292]	; (30008214 <DDR_PHY_DLL_CLK_DIV+0x2bc>)
300080f0:	6114      	str	r4, [r2, #16]
300080f2:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300080f6:	6151      	str	r1, [r2, #20]
300080f8:	f8c2 0098 	str.w	r0, [r2, #152]	; 0x98
300080fc:	4937      	ldr	r1, [pc, #220]	; (300081dc <DDR_PHY_DLL_CLK_DIV+0x284>)
300080fe:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
30008102:	f240 33c1 	movw	r3, #961	; 0x3c1
30008106:	6890      	ldr	r0, [r2, #8]
30008108:	4001      	ands	r1, r0
3000810a:	2033      	movs	r0, #51	; 0x33
3000810c:	430b      	orrs	r3, r1
3000810e:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30008112:	6093      	str	r3, [r2, #8]
30008114:	f8d2 309c 	ldr.w	r3, [r2, #156]	; 0x9c
30008118:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000811c:	f8c2 309c 	str.w	r3, [r2, #156]	; 0x9c
30008120:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008124:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008128:	4c2d      	ldr	r4, [pc, #180]	; (300081e0 <DDR_PHY_DLL_CLK_DIV+0x288>)
3000812a:	f023 0377 	bic.w	r3, r3, #119	; 0x77
3000812e:	f043 0312 	orr.w	r3, r3, #18
30008132:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008136:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
3000813a:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
3000813e:	7820      	ldrb	r0, [r4, #0]
30008140:	e76b      	b.n	3000801a <DDR_PHY_DLL_CLK_DIV+0xc2>
30008142:	4d30      	ldr	r5, [pc, #192]	; (30008204 <DDR_PHY_DLL_CLK_DIV+0x2ac>)
30008144:	47a8      	blx	r5
30008146:	2802      	cmp	r0, #2
30008148:	7020      	strb	r0, [r4, #0]
3000814a:	d099      	beq.n	30008080 <DDR_PHY_DLL_CLK_DIV+0x128>
3000814c:	2800      	cmp	r0, #0
3000814e:	d1c5      	bne.n	300080dc <DDR_PHY_DLL_CLK_DIV+0x184>
30008150:	47a8      	blx	r5
30008152:	2805      	cmp	r0, #5
30008154:	7020      	strb	r0, [r4, #0]
30008156:	d163      	bne.n	30008220 <DDR_PHY_DLL_CLK_DIV+0x2c8>
30008158:	4918      	ldr	r1, [pc, #96]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x264>)
3000815a:	230e      	movs	r3, #14
3000815c:	4c2e      	ldr	r4, [pc, #184]	; (30008218 <DDR_PHY_DLL_CLK_DIV+0x2c0>)
3000815e:	4a2f      	ldr	r2, [pc, #188]	; (3000821c <DDR_PHY_DLL_CLK_DIV+0x2c4>)
30008160:	4826      	ldr	r0, [pc, #152]	; (300081fc <DDR_PHY_DLL_CLK_DIV+0x2a4>)
30008162:	610c      	str	r4, [r1, #16]
30008164:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
30008168:	614a      	str	r2, [r1, #20]
3000816a:	f8c1 0098 	str.w	r0, [r1, #152]	; 0x98
3000816e:	4a1b      	ldr	r2, [pc, #108]	; (300081dc <DDR_PHY_DLL_CLK_DIV+0x284>)
30008170:	f8c1 30a0 	str.w	r3, [r1, #160]	; 0xa0
30008174:	f240 33c1 	movw	r3, #961	; 0x3c1
30008178:	6888      	ldr	r0, [r1, #8]
3000817a:	4002      	ands	r2, r0
3000817c:	2033      	movs	r0, #51	; 0x33
3000817e:	4313      	orrs	r3, r2
30008180:	f04f 5250 	mov.w	r2, #872415232	; 0x34000000
30008184:	608b      	str	r3, [r1, #8]
30008186:	f8d1 309c 	ldr.w	r3, [r1, #156]	; 0x9c
3000818a:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
3000818e:	f8c1 309c 	str.w	r3, [r1, #156]	; 0x9c
30008192:	f8c1 4080 	str.w	r4, [r1, #128]	; 0x80
30008196:	f8d1 3084 	ldr.w	r3, [r1, #132]	; 0x84
3000819a:	4c11      	ldr	r4, [pc, #68]	; (300081e0 <DDR_PHY_DLL_CLK_DIV+0x288>)
3000819c:	f023 0377 	bic.w	r3, r3, #119	; 0x77
300081a0:	f043 0312 	orr.w	r3, r3, #18
300081a4:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300081a8:	f8c1 0088 	str.w	r0, [r1, #136]	; 0x88
300081ac:	f8c1 2094 	str.w	r2, [r1, #148]	; 0x94
300081b0:	7820      	ldrb	r0, [r4, #0]
300081b2:	e732      	b.n	3000801a <DDR_PHY_DLL_CLK_DIV+0xc2>
300081b4:	cccccccd 	.word	0xcccccccd
300081b8:	001fff88 	.word	0x001fff88
300081bc:	41011000 	.word	0x41011000
300081c0:	f800ffff 	.word	0xf800ffff
300081c4:	07ff0000 	.word	0x07ff0000
300081c8:	fffff800 	.word	0xfffff800
300081cc:	3000e560 	.word	0x3000e560
300081d0:	07060012 	.word	0x07060012
300081d4:	000f0707 	.word	0x000f0707
300081d8:	080f0f0f 	.word	0x080f0f0f
300081dc:	ffffe000 	.word	0xffffe000
300081e0:	3000e561 	.word	0x3000e561
300081e4:	42008000 	.word	0x42008000
300081e8:	4c000903 	.word	0x4c000903
300081ec:	4c000821 	.word	0x4c000821
300081f0:	40000103 	.word	0x40000103
300081f4:	40000021 	.word	0x40000021
300081f8:	00130b0b 	.word	0x00130b0b
300081fc:	0e131313 	.word	0x0e131313
30008200:	0b0b0016 	.word	0x0b0b0016
30008204:	30007d59 	.word	0x30007d59
30008208:	30007e09 	.word	0x30007e09
3000820c:	0e0e0018 	.word	0x0e0e0018
30008210:	00170e0e 	.word	0x00170e0e
30008214:	0e171717 	.word	0x0e171717
30008218:	0a0a0018 	.word	0x0a0a0018
3000821c:	00130a0a 	.word	0x00130a0a
30008220:	2800      	cmp	r0, #0
30008222:	f47f af5d 	bne.w	300080e0 <DDR_PHY_DLL_CLK_DIV+0x188>
30008226:	47a8      	blx	r5
30008228:	2803      	cmp	r0, #3
3000822a:	7020      	strb	r0, [r4, #0]
3000822c:	f43f af5b 	beq.w	300080e6 <DDR_PHY_DLL_CLK_DIV+0x18e>
30008230:	2800      	cmp	r0, #0
30008232:	d13e      	bne.n	300082b2 <DDR_PHY_DLL_CLK_DIV+0x35a>
30008234:	47a8      	blx	r5
30008236:	280a      	cmp	r0, #10
30008238:	7020      	strb	r0, [r4, #0]
3000823a:	d03c      	beq.n	300082b6 <DDR_PHY_DLL_CLK_DIV+0x35e>
3000823c:	b908      	cbnz	r0, 30008242 <DDR_PHY_DLL_CLK_DIV+0x2ea>
3000823e:	47a8      	blx	r5
30008240:	7020      	strb	r0, [r4, #0]
30008242:	2809      	cmp	r0, #9
30008244:	d04c      	beq.n	300082e0 <DDR_PHY_DLL_CLK_DIV+0x388>
30008246:	4b32      	ldr	r3, [pc, #200]	; (30008310 <DDR_PHY_DLL_CLK_DIV+0x3b8>)
30008248:	681b      	ldr	r3, [r3, #0]
3000824a:	079a      	lsls	r2, r3, #30
3000824c:	d45c      	bmi.n	30008308 <DDR_PHY_DLL_CLK_DIV+0x3b0>
3000824e:	4b31      	ldr	r3, [pc, #196]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008250:	2100      	movs	r1, #0
30008252:	f44f 2400 	mov.w	r4, #524288	; 0x80000
30008256:	4830      	ldr	r0, [pc, #192]	; (30008318 <DDR_PHY_DLL_CLK_DIV+0x3c0>)
30008258:	6119      	str	r1, [r3, #16]
3000825a:	4a30      	ldr	r2, [pc, #192]	; (3000831c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
3000825c:	615c      	str	r4, [r3, #20]
3000825e:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008262:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30008266:	6899      	ldr	r1, [r3, #8]
30008268:	4c2d      	ldr	r4, [pc, #180]	; (30008320 <DDR_PHY_DLL_CLK_DIV+0x3c8>)
3000826a:	400a      	ands	r2, r1
3000826c:	609a      	str	r2, [r3, #8]
3000826e:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30008272:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008276:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000827a:	7820      	ldrb	r0, [r4, #0]
3000827c:	2800      	cmp	r0, #0
3000827e:	d03f      	beq.n	30008300 <DDR_PHY_DLL_CLK_DIV+0x3a8>
30008280:	2801      	cmp	r0, #1
30008282:	4b24      	ldr	r3, [pc, #144]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008284:	f04f 0100 	mov.w	r1, #0
30008288:	bf0c      	ite	eq
3000828a:	2233      	moveq	r2, #51	; 0x33
3000828c:	2222      	movne	r2, #34	; 0x22
3000828e:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
30008292:	f04f 3333 	mov.w	r3, #858993459	; 0x33333333
30008296:	4a1f      	ldr	r2, [pc, #124]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008298:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000829c:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300082a0:	f023 0377 	bic.w	r3, r3, #119	; 0x77
300082a4:	f043 0322 	orr.w	r3, r3, #34	; 0x22
300082a8:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300082ac:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
300082b0:	e6b3      	b.n	3000801a <DDR_PHY_DLL_CLK_DIV+0xc2>
300082b2:	280a      	cmp	r0, #10
300082b4:	d1c5      	bne.n	30008242 <DDR_PHY_DLL_CLK_DIV+0x2ea>
300082b6:	4b17      	ldr	r3, [pc, #92]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300082b8:	220e      	movs	r2, #14
300082ba:	4c1a      	ldr	r4, [pc, #104]	; (30008324 <DDR_PHY_DLL_CLK_DIV+0x3cc>)
300082bc:	491a      	ldr	r1, [pc, #104]	; (30008328 <DDR_PHY_DLL_CLK_DIV+0x3d0>)
300082be:	481b      	ldr	r0, [pc, #108]	; (3000832c <DDR_PHY_DLL_CLK_DIV+0x3d4>)
300082c0:	611c      	str	r4, [r3, #16]
300082c2:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300082c6:	6159      	str	r1, [r3, #20]
300082c8:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300082cc:	4913      	ldr	r1, [pc, #76]	; (3000831c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
300082ce:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
300082d2:	f240 32c1 	movw	r2, #961	; 0x3c1
300082d6:	6898      	ldr	r0, [r3, #8]
300082d8:	4001      	ands	r1, r0
300082da:	2033      	movs	r0, #51	; 0x33
300082dc:	430a      	orrs	r2, r1
300082de:	e6e3      	b.n	300080a8 <DDR_PHY_DLL_CLK_DIV+0x150>
300082e0:	4b0c      	ldr	r3, [pc, #48]	; (30008314 <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300082e2:	2212      	movs	r2, #18
300082e4:	4912      	ldr	r1, [pc, #72]	; (30008330 <DDR_PHY_DLL_CLK_DIV+0x3d8>)
300082e6:	4813      	ldr	r0, [pc, #76]	; (30008334 <DDR_PHY_DLL_CLK_DIV+0x3dc>)
300082e8:	4c13      	ldr	r4, [pc, #76]	; (30008338 <DDR_PHY_DLL_CLK_DIV+0x3e0>)
300082ea:	611c      	str	r4, [r3, #16]
300082ec:	6159      	str	r1, [r3, #20]
300082ee:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300082f2:	490a      	ldr	r1, [pc, #40]	; (3000831c <DDR_PHY_DLL_CLK_DIV+0x3c4>)
300082f4:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
300082f8:	f640 72c1 	movw	r2, #4033	; 0xfc1
300082fc:	6898      	ldr	r0, [r3, #8]
300082fe:	e6ce      	b.n	3000809e <DDR_PHY_DLL_CLK_DIV+0x146>
30008300:	4b0e      	ldr	r3, [pc, #56]	; (3000833c <DDR_PHY_DLL_CLK_DIV+0x3e4>)
30008302:	4798      	blx	r3
30008304:	7020      	strb	r0, [r4, #0]
30008306:	e7bb      	b.n	30008280 <DDR_PHY_DLL_CLK_DIV+0x328>
30008308:	480d      	ldr	r0, [pc, #52]	; (30008340 <DDR_PHY_DLL_CLK_DIV+0x3e8>)
3000830a:	f003 fd91 	bl	3000be30 <__DiagPrintf_veneer>
3000830e:	e79e      	b.n	3000824e <DDR_PHY_DLL_CLK_DIV+0x2f6>
30008310:	2001c00c 	.word	0x2001c00c
30008314:	41011000 	.word	0x41011000
30008318:	00080808 	.word	0x00080808
3000831c:	ffffe000 	.word	0xffffe000
30008320:	3000e561 	.word	0x3000e561
30008324:	0f0f0016 	.word	0x0f0f0016
30008328:	00180f0f 	.word	0x00180f0f
3000832c:	0e181818 	.word	0x0e181818
30008330:	00170e0e 	.word	0x00170e0e
30008334:	12171717 	.word	0x12171717
30008338:	0e0e001a 	.word	0x0e0e001a
3000833c:	30007e09 	.word	0x30007e09
30008340:	3000cd50 	.word	0x3000cd50

30008344 <DDR_PHY_CRT_Init>:
30008344:	492f      	ldr	r1, [pc, #188]	; (30008404 <DDR_PHY_CRT_Init+0xc0>)
30008346:	2005      	movs	r0, #5
30008348:	4a2f      	ldr	r2, [pc, #188]	; (30008408 <DDR_PHY_CRT_Init+0xc4>)
3000834a:	b510      	push	{r4, lr}
3000834c:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
30008350:	4c2e      	ldr	r4, [pc, #184]	; (3000840c <DDR_PHY_CRT_Init+0xc8>)
30008352:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008356:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
3000835a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000835e:	f043 0301 	orr.w	r3, r3, #1
30008362:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008366:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000836a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000836e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008372:	68e3      	ldr	r3, [r4, #12]
30008374:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008378:	60e3      	str	r3, [r4, #12]
3000837a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000837e:	f043 0302 	orr.w	r3, r3, #2
30008382:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008386:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000838a:	f023 0308 	bic.w	r3, r3, #8
3000838e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008392:	4790      	blx	r2
30008394:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008398:	f641 73ff 	movw	r3, #8191	; 0x1fff
3000839c:	491c      	ldr	r1, [pc, #112]	; (30008410 <DDR_PHY_CRT_Init+0xcc>)
3000839e:	f042 0204 	orr.w	r2, r2, #4
300083a2:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300083a6:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
300083aa:	f042 0208 	orr.w	r2, r2, #8
300083ae:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300083b2:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300083b4:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
300083b8:	62a2      	str	r2, [r4, #40]	; 0x28
300083ba:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300083bc:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
300083c0:	62a2      	str	r2, [r4, #40]	; 0x28
300083c2:	69e2      	ldr	r2, [r4, #28]
300083c4:	f042 0208 	orr.w	r2, r2, #8
300083c8:	61e2      	str	r2, [r4, #28]
300083ca:	68a2      	ldr	r2, [r4, #8]
300083cc:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
300083d0:	60a2      	str	r2, [r4, #8]
300083d2:	6862      	ldr	r2, [r4, #4]
300083d4:	4313      	orrs	r3, r2
300083d6:	6063      	str	r3, [r4, #4]
300083d8:	68a3      	ldr	r3, [r4, #8]
300083da:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
300083de:	60a3      	str	r3, [r4, #8]
300083e0:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
300083e4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300083e8:	f023 0303 	bic.w	r3, r3, #3
300083ec:	f043 0302 	orr.w	r3, r3, #2
300083f0:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300083f4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300083f8:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
300083fc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008400:	bd10      	pop	{r4, pc}
30008402:	bf00      	nop
30008404:	42008000 	.word	0x42008000
30008408:	00009b2d 	.word	0x00009b2d
3000840c:	41011000 	.word	0x41011000
30008410:	90161f00 	.word	0x90161f00

30008414 <DDR_PHY_R240_ZQ_CAL>:
30008414:	b538      	push	{r3, r4, r5, lr}
30008416:	4c84      	ldr	r4, [pc, #528]	; (30008628 <DDR_PHY_R240_ZQ_CAL+0x214>)
30008418:	7823      	ldrb	r3, [r4, #0]
3000841a:	2b00      	cmp	r3, #0
3000841c:	d05e      	beq.n	300084dc <DDR_PHY_R240_ZQ_CAL+0xc8>
3000841e:	4c83      	ldr	r4, [pc, #524]	; (3000862c <DDR_PHY_R240_ZQ_CAL+0x218>)
30008420:	2b03      	cmp	r3, #3
30008422:	7820      	ldrb	r0, [r4, #0]
30008424:	d136      	bne.n	30008494 <DDR_PHY_R240_ZQ_CAL+0x80>
30008426:	2800      	cmp	r0, #0
30008428:	d155      	bne.n	300084d6 <DDR_PHY_R240_ZQ_CAL+0xc2>
3000842a:	4d81      	ldr	r5, [pc, #516]	; (30008630 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000842c:	47a8      	blx	r5
3000842e:	28e0      	cmp	r0, #224	; 0xe0
30008430:	7020      	strb	r0, [r4, #0]
30008432:	f040 808f 	bne.w	30008554 <DDR_PHY_R240_ZQ_CAL+0x140>
30008436:	4b7f      	ldr	r3, [pc, #508]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008438:	2100      	movs	r1, #0
3000843a:	4c7f      	ldr	r4, [pc, #508]	; (30008638 <DDR_PHY_R240_ZQ_CAL+0x224>)
3000843c:	487f      	ldr	r0, [pc, #508]	; (3000863c <DDR_PHY_R240_ZQ_CAL+0x228>)
3000843e:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008442:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008446:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000844a:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000844e:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
30008452:	f504 2432 	add.w	r4, r4, #729088	; 0xb2000
30008456:	4a7a      	ldr	r2, [pc, #488]	; (30008640 <DDR_PHY_R240_ZQ_CAL+0x22c>)
30008458:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
3000845c:	f204 241a 	addw	r4, r4, #538	; 0x21a
30008460:	f200 201b 	addw	r0, r0, #539	; 0x21b
30008464:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
30008468:	f8c3 4190 	str.w	r4, [r3, #400]	; 0x190
3000846c:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
30008470:	f8c3 0198 	str.w	r0, [r3, #408]	; 0x198
30008474:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008478:	4b6e      	ldr	r3, [pc, #440]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000847a:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000847e:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
30008482:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008486:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000848a:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000848e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008492:	bd38      	pop	{r3, r4, r5, pc}
30008494:	2800      	cmp	r0, #0
30008496:	d040      	beq.n	3000851a <DDR_PHY_R240_ZQ_CAL+0x106>
30008498:	2805      	cmp	r0, #5
3000849a:	d124      	bne.n	300084e6 <DDR_PHY_R240_ZQ_CAL+0xd2>
3000849c:	4a65      	ldr	r2, [pc, #404]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000849e:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
300084a2:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
300084a6:	f043 0306 	orr.w	r3, r3, #6
300084aa:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300084ae:	4b61      	ldr	r3, [pc, #388]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
300084b0:	2100      	movs	r1, #0
300084b2:	4c64      	ldr	r4, [pc, #400]	; (30008644 <DDR_PHY_R240_ZQ_CAL+0x230>)
300084b4:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
300084b8:	4863      	ldr	r0, [pc, #396]	; (30008648 <DDR_PHY_R240_ZQ_CAL+0x234>)
300084ba:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
300084be:	f104 54e0 	add.w	r4, r4, #469762048	; 0x1c000000
300084c2:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
300084c6:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
300084ca:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300084ce:	485f      	ldr	r0, [pc, #380]	; (3000864c <DDR_PHY_R240_ZQ_CAL+0x238>)
300084d0:	f504 6422 	add.w	r4, r4, #2592	; 0xa20
300084d4:	e7c6      	b.n	30008464 <DDR_PHY_R240_ZQ_CAL+0x50>
300084d6:	28e0      	cmp	r0, #224	; 0xe0
300084d8:	d0ad      	beq.n	30008436 <DDR_PHY_R240_ZQ_CAL+0x22>
300084da:	e03e      	b.n	3000855a <DDR_PHY_R240_ZQ_CAL+0x146>
300084dc:	4b5c      	ldr	r3, [pc, #368]	; (30008650 <DDR_PHY_R240_ZQ_CAL+0x23c>)
300084de:	4798      	blx	r3
300084e0:	4603      	mov	r3, r0
300084e2:	7020      	strb	r0, [r4, #0]
300084e4:	e79b      	b.n	3000841e <DDR_PHY_R240_ZQ_CAL+0xa>
300084e6:	2803      	cmp	r0, #3
300084e8:	d125      	bne.n	30008536 <DDR_PHY_R240_ZQ_CAL+0x122>
300084ea:	4a52      	ldr	r2, [pc, #328]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
300084ec:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
300084f0:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
300084f4:	f043 030c 	orr.w	r3, r3, #12
300084f8:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300084fc:	4b4d      	ldr	r3, [pc, #308]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
300084fe:	2100      	movs	r1, #0
30008500:	4c54      	ldr	r4, [pc, #336]	; (30008654 <DDR_PHY_R240_ZQ_CAL+0x240>)
30008502:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008506:	4854      	ldr	r0, [pc, #336]	; (30008658 <DDR_PHY_R240_ZQ_CAL+0x244>)
30008508:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
3000850c:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008510:	4c52      	ldr	r4, [pc, #328]	; (3000865c <DDR_PHY_R240_ZQ_CAL+0x248>)
30008512:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008516:	4852      	ldr	r0, [pc, #328]	; (30008660 <DDR_PHY_R240_ZQ_CAL+0x24c>)
30008518:	e7a4      	b.n	30008464 <DDR_PHY_R240_ZQ_CAL+0x50>
3000851a:	4d45      	ldr	r5, [pc, #276]	; (30008630 <DDR_PHY_R240_ZQ_CAL+0x21c>)
3000851c:	47a8      	blx	r5
3000851e:	2805      	cmp	r0, #5
30008520:	7020      	strb	r0, [r4, #0]
30008522:	d0bb      	beq.n	3000849c <DDR_PHY_R240_ZQ_CAL+0x88>
30008524:	2800      	cmp	r0, #0
30008526:	d1de      	bne.n	300084e6 <DDR_PHY_R240_ZQ_CAL+0xd2>
30008528:	47a8      	blx	r5
3000852a:	2803      	cmp	r0, #3
3000852c:	7020      	strb	r0, [r4, #0]
3000852e:	d0dc      	beq.n	300084ea <DDR_PHY_R240_ZQ_CAL+0xd6>
30008530:	b908      	cbnz	r0, 30008536 <DDR_PHY_R240_ZQ_CAL+0x122>
30008532:	47a8      	blx	r5
30008534:	7020      	strb	r0, [r4, #0]
30008536:	4a3f      	ldr	r2, [pc, #252]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
30008538:	280a      	cmp	r0, #10
3000853a:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
3000853e:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
30008542:	d056      	beq.n	300085f2 <DDR_PHY_R240_ZQ_CAL+0x1de>
30008544:	f043 031e 	orr.w	r3, r3, #30
30008548:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
3000854c:	2800      	cmp	r0, #0
3000854e:	f43f af6c 	beq.w	3000842a <DDR_PHY_R240_ZQ_CAL+0x16>
30008552:	e7c0      	b.n	300084d6 <DDR_PHY_R240_ZQ_CAL+0xc2>
30008554:	b908      	cbnz	r0, 3000855a <DDR_PHY_R240_ZQ_CAL+0x146>
30008556:	47a8      	blx	r5
30008558:	7020      	strb	r0, [r4, #0]
3000855a:	2802      	cmp	r0, #2
3000855c:	f43f af6b 	beq.w	30008436 <DDR_PHY_R240_ZQ_CAL+0x22>
30008560:	b910      	cbnz	r0, 30008568 <DDR_PHY_R240_ZQ_CAL+0x154>
30008562:	4b33      	ldr	r3, [pc, #204]	; (30008630 <DDR_PHY_R240_ZQ_CAL+0x21c>)
30008564:	4798      	blx	r3
30008566:	7020      	strb	r0, [r4, #0]
30008568:	2805      	cmp	r0, #5
3000856a:	d0a0      	beq.n	300084ae <DDR_PHY_R240_ZQ_CAL+0x9a>
3000856c:	bb20      	cbnz	r0, 300085b8 <DDR_PHY_R240_ZQ_CAL+0x1a4>
3000856e:	4d30      	ldr	r5, [pc, #192]	; (30008630 <DDR_PHY_R240_ZQ_CAL+0x21c>)
30008570:	47a8      	blx	r5
30008572:	2803      	cmp	r0, #3
30008574:	7020      	strb	r0, [r4, #0]
30008576:	d0c1      	beq.n	300084fc <DDR_PHY_R240_ZQ_CAL+0xe8>
30008578:	bb00      	cbnz	r0, 300085bc <DDR_PHY_R240_ZQ_CAL+0x1a8>
3000857a:	47a8      	blx	r5
3000857c:	280a      	cmp	r0, #10
3000857e:	7020      	strb	r0, [r4, #0]
30008580:	d01e      	beq.n	300085c0 <DDR_PHY_R240_ZQ_CAL+0x1ac>
30008582:	b908      	cbnz	r0, 30008588 <DDR_PHY_R240_ZQ_CAL+0x174>
30008584:	47a8      	blx	r5
30008586:	7020      	strb	r0, [r4, #0]
30008588:	2809      	cmp	r0, #9
3000858a:	4b2a      	ldr	r3, [pc, #168]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
3000858c:	f04f 0100 	mov.w	r1, #0
30008590:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008594:	d032      	beq.n	300085fc <DDR_PHY_R240_ZQ_CAL+0x1e8>
30008596:	f8c3 1180 	str.w	r1, [r3, #384]	; 0x180
3000859a:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000859e:	f8c3 1188 	str.w	r1, [r3, #392]	; 0x188
300085a2:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
300085a6:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
300085aa:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300085ae:	f8c3 2198 	str.w	r2, [r3, #408]	; 0x198
300085b2:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300085b6:	e75f      	b.n	30008478 <DDR_PHY_R240_ZQ_CAL+0x64>
300085b8:	2803      	cmp	r0, #3
300085ba:	d09f      	beq.n	300084fc <DDR_PHY_R240_ZQ_CAL+0xe8>
300085bc:	280a      	cmp	r0, #10
300085be:	d1e3      	bne.n	30008588 <DDR_PHY_R240_ZQ_CAL+0x174>
300085c0:	4b1c      	ldr	r3, [pc, #112]	; (30008634 <DDR_PHY_R240_ZQ_CAL+0x220>)
300085c2:	2100      	movs	r1, #0
300085c4:	4c27      	ldr	r4, [pc, #156]	; (30008664 <DDR_PHY_R240_ZQ_CAL+0x250>)
300085c6:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
300085ca:	4827      	ldr	r0, [pc, #156]	; (30008668 <DDR_PHY_R240_ZQ_CAL+0x254>)
300085cc:	f8c3 41a0 	str.w	r4, [r3, #416]	; 0x1a0
300085d0:	f8c3 11a4 	str.w	r1, [r3, #420]	; 0x1a4
300085d4:	4c22      	ldr	r4, [pc, #136]	; (30008660 <DDR_PHY_R240_ZQ_CAL+0x24c>)
300085d6:	f8c3 01a8 	str.w	r0, [r3, #424]	; 0x1a8
300085da:	4824      	ldr	r0, [pc, #144]	; (3000866c <DDR_PHY_R240_ZQ_CAL+0x258>)
300085dc:	f8c3 11ac 	str.w	r1, [r3, #428]	; 0x1ac
300085e0:	f8c3 41b0 	str.w	r4, [r3, #432]	; 0x1b0
300085e4:	f8c3 21b4 	str.w	r2, [r3, #436]	; 0x1b4
300085e8:	f8c3 01b8 	str.w	r0, [r3, #440]	; 0x1b8
300085ec:	f8c3 21bc 	str.w	r2, [r3, #444]	; 0x1bc
300085f0:	e742      	b.n	30008478 <DDR_PHY_R240_ZQ_CAL+0x64>
300085f2:	f043 0308 	orr.w	r3, r3, #8
300085f6:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300085fa:	e7e1      	b.n	300085c0 <DDR_PHY_R240_ZQ_CAL+0x1ac>
300085fc:	4c0e      	ldr	r4, [pc, #56]	; (30008638 <DDR_PHY_R240_ZQ_CAL+0x224>)
300085fe:	480f      	ldr	r0, [pc, #60]	; (3000863c <DDR_PHY_R240_ZQ_CAL+0x228>)
30008600:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008604:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008608:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000860c:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008610:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
30008614:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
30008618:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
3000861c:	f504 6402 	add.w	r4, r4, #2080	; 0x820
30008620:	f200 201b 	addw	r0, r0, #539	; 0x21b
30008624:	e71e      	b.n	30008464 <DDR_PHY_R240_ZQ_CAL+0x50>
30008626:	bf00      	nop
30008628:	3000e561 	.word	0x3000e561
3000862c:	3000e560 	.word	0x3000e560
30008630:	30007d59 	.word	0x30007d59
30008634:	41011000 	.word	0x41011000
30008638:	0e141414 	.word	0x0e141414
3000863c:	140e0e0e 	.word	0x140e0e0e
30008640:	00262626 	.word	0x00262626
30008644:	0a0e0c0c 	.word	0x0a0e0c0c
30008648:	0c0b0a0a 	.word	0x0c0b0a0a
3000864c:	26162620 	.word	0x26162620
30008650:	30007e09 	.word	0x30007e09
30008654:	0a0c0b0b 	.word	0x0a0c0b0b
30008658:	0b0b0a0a 	.word	0x0b0b0a0a
3000865c:	26172721 	.word	0x26172721
30008660:	2615231e 	.word	0x2615231e
30008664:	090b0a0a 	.word	0x090b0a0a
30008668:	0a0a0909 	.word	0x0a0a0909
3000866c:	2613201b 	.word	0x2613201b

30008670 <DDR_PHY_ZQ_SET_SEL>:
30008670:	b410      	push	{r4}
30008672:	4b25      	ldr	r3, [pc, #148]	; (30008708 <DDR_PHY_ZQ_SET_SEL+0x98>)
30008674:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008678:	4a24      	ldr	r2, [pc, #144]	; (3000870c <DDR_PHY_ZQ_SET_SEL+0x9c>)
3000867a:	4c25      	ldr	r4, [pc, #148]	; (30008710 <DDR_PHY_ZQ_SET_SEL+0xa0>)
3000867c:	4925      	ldr	r1, [pc, #148]	; (30008714 <DDR_PHY_ZQ_SET_SEL+0xa4>)
3000867e:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
30008682:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
30008686:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
3000868a:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
3000868e:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
30008692:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
30008696:	f64f 74ff 	movw	r4, #65535	; 0xffff
3000869a:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
3000869e:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300086a2:	2200      	movs	r2, #0
300086a4:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
300086a8:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
300086ac:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
300086b0:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
300086b4:	f04f 31ff 	mov.w	r1, #4294967295
300086b8:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
300086bc:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
300086c0:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
300086c4:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
300086c8:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
300086cc:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
300086d0:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
300086d4:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
300086d8:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
300086dc:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
300086e0:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
300086e4:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
300086e8:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
300086ec:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
300086f0:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
300086f4:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
300086f8:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
300086fc:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30008700:	f85d 4b04 	ldr.w	r4, [sp], #4
30008704:	4770      	bx	lr
30008706:	bf00      	nop
30008708:	41011000 	.word	0x41011000
3000870c:	ffff1111 	.word	0xffff1111
30008710:	ffff3333 	.word	0xffff3333
30008714:	ffff0000 	.word	0xffff0000

30008718 <DDR_PHY_DELAY_TAP_SET>:
30008718:	b510      	push	{r4, lr}
3000871a:	4b15      	ldr	r3, [pc, #84]	; (30008770 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000871c:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
30008720:	4914      	ldr	r1, [pc, #80]	; (30008774 <DDR_PHY_DELAY_TAP_SET+0x5c>)
30008722:	4c15      	ldr	r4, [pc, #84]	; (30008778 <DDR_PHY_DELAY_TAP_SET+0x60>)
30008724:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30008728:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
3000872c:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008730:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30008734:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008738:	7820      	ldrb	r0, [r4, #0]
3000873a:	b1a0      	cbz	r0, 30008766 <DDR_PHY_DELAY_TAP_SET+0x4e>
3000873c:	2801      	cmp	r0, #1
3000873e:	4b0c      	ldr	r3, [pc, #48]	; (30008770 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008740:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30008744:	bf0c      	ite	eq
30008746:	4a0d      	ldreq	r2, [pc, #52]	; (3000877c <DDR_PHY_DELAY_TAP_SET+0x64>)
30008748:	4a0d      	ldrne	r2, [pc, #52]	; (30008780 <DDR_PHY_DELAY_TAP_SET+0x68>)
3000874a:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
3000874e:	f640 0288 	movw	r2, #2184	; 0x888
30008752:	4b07      	ldr	r3, [pc, #28]	; (30008770 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008754:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008758:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
3000875c:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008760:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30008764:	bd10      	pop	{r4, pc}
30008766:	4b07      	ldr	r3, [pc, #28]	; (30008784 <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008768:	4798      	blx	r3
3000876a:	7020      	strb	r0, [r4, #0]
3000876c:	e7e6      	b.n	3000873c <DDR_PHY_DELAY_TAP_SET+0x24>
3000876e:	bf00      	nop
30008770:	41011000 	.word	0x41011000
30008774:	20888888 	.word	0x20888888
30008778:	3000e561 	.word	0x3000e561
3000877c:	7c033333 	.word	0x7c033333
30008780:	7c011111 	.word	0x7c011111
30008784:	30007e09 	.word	0x30007e09

30008788 <DDR_PHY_READ_CTRL>:
30008788:	b538      	push	{r3, r4, r5, lr}
3000878a:	4c2a      	ldr	r4, [pc, #168]	; (30008834 <DDR_PHY_READ_CTRL+0xac>)
3000878c:	7820      	ldrb	r0, [r4, #0]
3000878e:	b330      	cbz	r0, 300087de <DDR_PHY_READ_CTRL+0x56>
30008790:	28e0      	cmp	r0, #224	; 0xe0
30008792:	d114      	bne.n	300087be <DDR_PHY_READ_CTRL+0x36>
30008794:	4b28      	ldr	r3, [pc, #160]	; (30008838 <DDR_PHY_READ_CTRL+0xb0>)
30008796:	2202      	movs	r2, #2
30008798:	2103      	movs	r1, #3
3000879a:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
3000879e:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300087a2:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
300087a6:	4b24      	ldr	r3, [pc, #144]	; (30008838 <DDR_PHY_READ_CTRL+0xb0>)
300087a8:	2108      	movs	r1, #8
300087aa:	22fe      	movs	r2, #254	; 0xfe
300087ac:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
300087b0:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
300087b4:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300087b8:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300087bc:	bd38      	pop	{r3, r4, r5, pc}
300087be:	2802      	cmp	r0, #2
300087c0:	d0e8      	beq.n	30008794 <DDR_PHY_READ_CTRL+0xc>
300087c2:	2805      	cmp	r0, #5
300087c4:	d001      	beq.n	300087ca <DDR_PHY_READ_CTRL+0x42>
300087c6:	2803      	cmp	r0, #3
300087c8:	d131      	bne.n	3000882e <DDR_PHY_READ_CTRL+0xa6>
300087ca:	4b1b      	ldr	r3, [pc, #108]	; (30008838 <DDR_PHY_READ_CTRL+0xb0>)
300087cc:	2202      	movs	r2, #2
300087ce:	2104      	movs	r1, #4
300087d0:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
300087d4:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300087d8:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
300087dc:	e7e3      	b.n	300087a6 <DDR_PHY_READ_CTRL+0x1e>
300087de:	4d17      	ldr	r5, [pc, #92]	; (3000883c <DDR_PHY_READ_CTRL+0xb4>)
300087e0:	47a8      	blx	r5
300087e2:	28e0      	cmp	r0, #224	; 0xe0
300087e4:	7020      	strb	r0, [r4, #0]
300087e6:	d0d5      	beq.n	30008794 <DDR_PHY_READ_CTRL+0xc>
300087e8:	2800      	cmp	r0, #0
300087ea:	d1e8      	bne.n	300087be <DDR_PHY_READ_CTRL+0x36>
300087ec:	47a8      	blx	r5
300087ee:	2802      	cmp	r0, #2
300087f0:	7020      	strb	r0, [r4, #0]
300087f2:	d0cf      	beq.n	30008794 <DDR_PHY_READ_CTRL+0xc>
300087f4:	2800      	cmp	r0, #0
300087f6:	d1e4      	bne.n	300087c2 <DDR_PHY_READ_CTRL+0x3a>
300087f8:	47a8      	blx	r5
300087fa:	2805      	cmp	r0, #5
300087fc:	7020      	strb	r0, [r4, #0]
300087fe:	d0e4      	beq.n	300087ca <DDR_PHY_READ_CTRL+0x42>
30008800:	2800      	cmp	r0, #0
30008802:	d1e0      	bne.n	300087c6 <DDR_PHY_READ_CTRL+0x3e>
30008804:	47a8      	blx	r5
30008806:	2803      	cmp	r0, #3
30008808:	7020      	strb	r0, [r4, #0]
3000880a:	d0de      	beq.n	300087ca <DDR_PHY_READ_CTRL+0x42>
3000880c:	b978      	cbnz	r0, 3000882e <DDR_PHY_READ_CTRL+0xa6>
3000880e:	47a8      	blx	r5
30008810:	280a      	cmp	r0, #10
30008812:	7020      	strb	r0, [r4, #0]
30008814:	d0d9      	beq.n	300087ca <DDR_PHY_READ_CTRL+0x42>
30008816:	b908      	cbnz	r0, 3000881c <DDR_PHY_READ_CTRL+0x94>
30008818:	47a8      	blx	r5
3000881a:	7020      	strb	r0, [r4, #0]
3000881c:	4b06      	ldr	r3, [pc, #24]	; (30008838 <DDR_PHY_READ_CTRL+0xb0>)
3000881e:	2203      	movs	r2, #3
30008820:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
30008824:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008828:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
3000882c:	e7bb      	b.n	300087a6 <DDR_PHY_READ_CTRL+0x1e>
3000882e:	280a      	cmp	r0, #10
30008830:	d0cb      	beq.n	300087ca <DDR_PHY_READ_CTRL+0x42>
30008832:	e7f3      	b.n	3000881c <DDR_PHY_READ_CTRL+0x94>
30008834:	3000e560 	.word	0x3000e560
30008838:	41011000 	.word	0x41011000
3000883c:	30007d59 	.word	0x30007d59

30008840 <DDR_PHY_READ_LEVELING>:
30008840:	b538      	push	{r3, r4, r5, lr}
30008842:	4c36      	ldr	r4, [pc, #216]	; (3000891c <DDR_PHY_READ_LEVELING+0xdc>)
30008844:	7820      	ldrb	r0, [r4, #0]
30008846:	b348      	cbz	r0, 3000889c <DDR_PHY_READ_LEVELING+0x5c>
30008848:	28e0      	cmp	r0, #224	; 0xe0
3000884a:	d11e      	bne.n	3000888a <DDR_PHY_READ_LEVELING+0x4a>
3000884c:	4b34      	ldr	r3, [pc, #208]	; (30008920 <DDR_PHY_READ_LEVELING+0xe0>)
3000884e:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30008852:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008856:	220f      	movs	r2, #15
30008858:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
3000885c:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30008860:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30008864:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30008868:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
3000886c:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30008870:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30008874:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30008878:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
3000887c:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30008880:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30008884:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30008888:	bd38      	pop	{r3, r4, r5, pc}
3000888a:	2802      	cmp	r0, #2
3000888c:	d0de      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
3000888e:	2805      	cmp	r0, #5
30008890:	d0dc      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
30008892:	2803      	cmp	r0, #3
30008894:	d0da      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
30008896:	280a      	cmp	r0, #10
30008898:	d0d8      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
3000889a:	e01f      	b.n	300088dc <DDR_PHY_READ_LEVELING+0x9c>
3000889c:	4d21      	ldr	r5, [pc, #132]	; (30008924 <DDR_PHY_READ_LEVELING+0xe4>)
3000889e:	47a8      	blx	r5
300088a0:	28e0      	cmp	r0, #224	; 0xe0
300088a2:	7020      	strb	r0, [r4, #0]
300088a4:	d0d2      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088a6:	2800      	cmp	r0, #0
300088a8:	d1ef      	bne.n	3000888a <DDR_PHY_READ_LEVELING+0x4a>
300088aa:	47a8      	blx	r5
300088ac:	2802      	cmp	r0, #2
300088ae:	7020      	strb	r0, [r4, #0]
300088b0:	d0cc      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088b2:	2800      	cmp	r0, #0
300088b4:	d1eb      	bne.n	3000888e <DDR_PHY_READ_LEVELING+0x4e>
300088b6:	47a8      	blx	r5
300088b8:	2805      	cmp	r0, #5
300088ba:	7020      	strb	r0, [r4, #0]
300088bc:	d0c6      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088be:	2800      	cmp	r0, #0
300088c0:	d1e7      	bne.n	30008892 <DDR_PHY_READ_LEVELING+0x52>
300088c2:	47a8      	blx	r5
300088c4:	2803      	cmp	r0, #3
300088c6:	7020      	strb	r0, [r4, #0]
300088c8:	d0c0      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088ca:	2800      	cmp	r0, #0
300088cc:	d1e3      	bne.n	30008896 <DDR_PHY_READ_LEVELING+0x56>
300088ce:	47a8      	blx	r5
300088d0:	280a      	cmp	r0, #10
300088d2:	7020      	strb	r0, [r4, #0]
300088d4:	d0ba      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088d6:	b908      	cbnz	r0, 300088dc <DDR_PHY_READ_LEVELING+0x9c>
300088d8:	47a8      	blx	r5
300088da:	7020      	strb	r0, [r4, #0]
300088dc:	2809      	cmp	r0, #9
300088de:	d0b5      	beq.n	3000884c <DDR_PHY_READ_LEVELING+0xc>
300088e0:	4b0f      	ldr	r3, [pc, #60]	; (30008920 <DDR_PHY_READ_LEVELING+0xe0>)
300088e2:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300088e6:	2110      	movs	r1, #16
300088e8:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300088ec:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300088f0:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300088f4:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300088f8:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
300088fc:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008900:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30008904:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008908:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
3000890c:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008910:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30008914:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008918:	bd38      	pop	{r3, r4, r5, pc}
3000891a:	bf00      	nop
3000891c:	3000e560 	.word	0x3000e560
30008920:	41011000 	.word	0x41011000
30008924:	30007d59 	.word	0x30007d59

30008928 <DDR_PHY_Init>:
30008928:	b510      	push	{r4, lr}
3000892a:	6d80      	ldr	r0, [r0, #88]	; 0x58
3000892c:	4c2f      	ldr	r4, [pc, #188]	; (300089ec <DDR_PHY_Init+0xc4>)
3000892e:	f7ff fb13 	bl	30007f58 <DDR_PHY_DLL_CLK_DIV>
30008932:	f7ff fd07 	bl	30008344 <DDR_PHY_CRT_Init>
30008936:	f7ff fd6d 	bl	30008414 <DDR_PHY_R240_ZQ_CAL>
3000893a:	f7ff fe99 	bl	30008670 <DDR_PHY_ZQ_SET_SEL>
3000893e:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30008942:	2300      	movs	r3, #0
30008944:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008948:	4829      	ldr	r0, [pc, #164]	; (300089f0 <DDR_PHY_Init+0xc8>)
3000894a:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
3000894e:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30008952:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30008956:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
3000895a:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
3000895e:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30008962:	f7ff fed9 	bl	30008718 <DDR_PHY_DELAY_TAP_SET>
30008966:	f7ff ff0f 	bl	30008788 <DDR_PHY_READ_CTRL>
3000896a:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
3000896e:	4a21      	ldr	r2, [pc, #132]	; (300089f4 <DDR_PHY_Init+0xcc>)
30008970:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30008974:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008978:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
3000897c:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008980:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008984:	f7ff ff5c 	bl	30008840 <DDR_PHY_READ_LEVELING>
30008988:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
3000898c:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008990:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008994:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008998:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
3000899c:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
300089a0:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
300089a4:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
300089a8:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
300089ac:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
300089b0:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
300089b4:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300089b8:	f043 0303 	orr.w	r3, r3, #3
300089bc:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300089c0:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300089c4:	f043 030c 	orr.w	r3, r3, #12
300089c8:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300089cc:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300089d0:	f023 0303 	bic.w	r3, r3, #3
300089d4:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300089d8:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300089dc:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300089e0:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300089e4:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300089e8:	bd10      	pop	{r4, pc}
300089ea:	bf00      	nop
300089ec:	41011000 	.word	0x41011000
300089f0:	00045500 	.word	0x00045500
300089f4:	11117777 	.word	0x11117777

300089f8 <DDR_PHY_CAL>:
300089f8:	4770      	bx	lr
300089fa:	bf00      	nop

300089fc <DDR_PHY_AutoGating>:
300089fc:	4a0a      	ldr	r2, [pc, #40]	; (30008a28 <DDR_PHY_AutoGating+0x2c>)
300089fe:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30008a02:	6893      	ldr	r3, [r2, #8]
30008a04:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008a08:	6093      	str	r3, [r2, #8]
30008a0a:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008a0e:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008a12:	f043 0314 	orr.w	r3, r3, #20
30008a16:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008a1a:	f8d1 31e0 	ldr.w	r3, [r1, #480]	; 0x1e0
30008a1e:	f043 0303 	orr.w	r3, r3, #3
30008a22:	f8c1 31e0 	str.w	r3, [r1, #480]	; 0x1e0
30008a26:	4770      	bx	lr
30008a28:	41011000 	.word	0x41011000

30008a2c <flash_init_userdef>:
30008a2c:	4b14      	ldr	r3, [pc, #80]	; (30008a80 <flash_init_userdef+0x54>)
30008a2e:	2201      	movs	r2, #1
30008a30:	2140      	movs	r1, #64	; 0x40
30008a32:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008a36:	b430      	push	{r4, r5}
30008a38:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008a3c:	2502      	movs	r5, #2
30008a3e:	2400      	movs	r4, #0
30008a40:	f640 0104 	movw	r1, #2052	; 0x804
30008a44:	601a      	str	r2, [r3, #0]
30008a46:	22bb      	movs	r2, #187	; 0xbb
30008a48:	83d8      	strh	r0, [r3, #30]
30008a4a:	8419      	strh	r1, [r3, #32]
30008a4c:	203b      	movs	r0, #59	; 0x3b
30008a4e:	21eb      	movs	r1, #235	; 0xeb
30008a50:	629a      	str	r2, [r3, #40]	; 0x28
30008a52:	4a0c      	ldr	r2, [pc, #48]	; (30008a84 <flash_init_userdef+0x58>)
30008a54:	6258      	str	r0, [r3, #36]	; 0x24
30008a56:	6319      	str	r1, [r3, #48]	; 0x30
30008a58:	f64a 3120 	movw	r1, #43808	; 0xab20
30008a5c:	655a      	str	r2, [r3, #84]	; 0x54
30008a5e:	22b9      	movs	r2, #185	; 0xb9
30008a60:	4809      	ldr	r0, [pc, #36]	; (30008a88 <flash_init_userdef+0x5c>)
30008a62:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008a66:	6598      	str	r0, [r3, #88]	; 0x58
30008a68:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008a6c:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008a70:	2406      	movs	r4, #6
30008a72:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008a76:	246b      	movs	r4, #107	; 0x6b
30008a78:	62dc      	str	r4, [r3, #44]	; 0x2c
30008a7a:	bc30      	pop	{r4, r5}
30008a7c:	4770      	bx	lr
30008a7e:	bf00      	nop
30008a80:	2001c01c 	.word	0x2001c01c
30008a84:	00059f06 	.word	0x00059f06
30008a88:	d8600001 	.word	0xd8600001

30008a8c <nand_init_userdef>:
30008a8c:	4b18      	ldr	r3, [pc, #96]	; (30008af0 <nand_init_userdef+0x64>)
30008a8e:	2001      	movs	r0, #1
30008a90:	2200      	movs	r2, #0
30008a92:	2102      	movs	r1, #2
30008a94:	b410      	push	{r4}
30008a96:	6018      	str	r0, [r3, #0]
30008a98:	f640 0408 	movw	r4, #2056	; 0x808
30008a9c:	6159      	str	r1, [r3, #20]
30008a9e:	83dc      	strh	r4, [r3, #30]
30008aa0:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008aa4:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008aa8:	841c      	strh	r4, [r3, #32]
30008aaa:	246b      	movs	r4, #107	; 0x6b
30008aac:	619a      	str	r2, [r3, #24]
30008aae:	62dc      	str	r4, [r3, #44]	; 0x2c
30008ab0:	4c10      	ldr	r4, [pc, #64]	; (30008af4 <nand_init_userdef+0x68>)
30008ab2:	775a      	strb	r2, [r3, #29]
30008ab4:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008ab8:	629a      	str	r2, [r3, #40]	; 0x28
30008aba:	631a      	str	r2, [r3, #48]	; 0x30
30008abc:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30008ac0:	65da      	str	r2, [r3, #92]	; 0x5c
30008ac2:	e9c3 0003 	strd	r0, r0, [r3, #12]
30008ac6:	203b      	movs	r0, #59	; 0x3b
30008ac8:	6258      	str	r0, [r3, #36]	; 0x24
30008aca:	200b      	movs	r0, #11
30008acc:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30008ad0:	6698      	str	r0, [r3, #104]	; 0x68
30008ad2:	2008      	movs	r0, #8
30008ad4:	4908      	ldr	r1, [pc, #32]	; (30008af8 <nand_init_userdef+0x6c>)
30008ad6:	64d8      	str	r0, [r3, #76]	; 0x4c
30008ad8:	f241 0013 	movw	r0, #4115	; 0x1013
30008adc:	6599      	str	r1, [r3, #88]	; 0x58
30008ade:	4907      	ldr	r1, [pc, #28]	; (30008afc <nand_init_userdef+0x70>)
30008ae0:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30008ae4:	6719      	str	r1, [r3, #112]	; 0x70
30008ae6:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30008aea:	f85d 4b04 	ldr.w	r4, [sp], #4
30008aee:	4770      	bx	lr
30008af0:	2001c01c 	.word	0x2001c01c
30008af4:	000f9f06 	.word	0x000f9f06
30008af8:	d800001f 	.word	0xd800001f
30008afc:	10300804 	.word	0x10300804

30008b00 <flash_layout_init>:
30008b00:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008b04:	4b65      	ldr	r3, [pc, #404]	; (30008c9c <flash_layout_init+0x19c>)
30008b06:	b08f      	sub	sp, #60	; 0x3c
30008b08:	4798      	blx	r3
30008b0a:	4b65      	ldr	r3, [pc, #404]	; (30008ca0 <flash_layout_init+0x1a0>)
30008b0c:	4a65      	ldr	r2, [pc, #404]	; (30008ca4 <flash_layout_init+0x1a4>)
30008b0e:	2800      	cmp	r0, #0
30008b10:	bf08      	it	eq
30008b12:	4613      	moveq	r3, r2
30008b14:	681f      	ldr	r7, [r3, #0]
30008b16:	2fff      	cmp	r7, #255	; 0xff
30008b18:	d05a      	beq.n	30008bd0 <flash_layout_init+0xd0>
30008b1a:	f8df b19c 	ldr.w	fp, [pc, #412]	; 30008cb8 <flash_layout_init+0x1b8>
30008b1e:	f04f 0900 	mov.w	r9, #0
30008b22:	4a61      	ldr	r2, [pc, #388]	; (30008ca8 <flash_layout_init+0x1a8>)
30008b24:	f8db 1000 	ldr.w	r1, [fp]
30008b28:	46c8      	mov	r8, r9
30008b2a:	f8df e190 	ldr.w	lr, [pc, #400]	; 30008cbc <flash_layout_init+0x1bc>
30008b2e:	464e      	mov	r6, r9
30008b30:	910d      	str	r1, [sp, #52]	; 0x34
30008b32:	464c      	mov	r4, r9
30008b34:	495d      	ldr	r1, [pc, #372]	; (30008cac <flash_layout_init+0x1ac>)
30008b36:	6812      	ldr	r2, [r2, #0]
30008b38:	6809      	ldr	r1, [r1, #0]
30008b3a:	f8df c184 	ldr.w	ip, [pc, #388]	; 30008cc0 <flash_layout_init+0x1c0>
30008b3e:	9109      	str	r1, [sp, #36]	; 0x24
30008b40:	495b      	ldr	r1, [pc, #364]	; (30008cb0 <flash_layout_init+0x1b0>)
30008b42:	920c      	str	r2, [sp, #48]	; 0x30
30008b44:	6809      	ldr	r1, [r1, #0]
30008b46:	4a5b      	ldr	r2, [pc, #364]	; (30008cb4 <flash_layout_init+0x1b4>)
30008b48:	910b      	str	r1, [sp, #44]	; 0x2c
30008b4a:	f8de 1000 	ldr.w	r1, [lr]
30008b4e:	f8cd 9028 	str.w	r9, [sp, #40]	; 0x28
30008b52:	9107      	str	r1, [sp, #28]
30008b54:	f89c 1000 	ldrb.w	r1, [ip]
30008b58:	f8cd 9018 	str.w	r9, [sp, #24]
30008b5c:	9108      	str	r1, [sp, #32]
30008b5e:	6951      	ldr	r1, [r2, #20]
30008b60:	f8cd 9010 	str.w	r9, [sp, #16]
30008b64:	9105      	str	r1, [sp, #20]
30008b66:	6911      	ldr	r1, [r2, #16]
30008b68:	f8cd 9008 	str.w	r9, [sp, #8]
30008b6c:	9103      	str	r1, [sp, #12]
30008b6e:	68d1      	ldr	r1, [r2, #12]
30008b70:	9101      	str	r1, [sp, #4]
30008b72:	6891      	ldr	r1, [r2, #8]
30008b74:	e9d2 0500 	ldrd	r0, r5, [r2]
30008b78:	9100      	str	r1, [sp, #0]
30008b7a:	4649      	mov	r1, r9
30008b7c:	2f00      	cmp	r7, #0
30008b7e:	d12a      	bne.n	30008bd6 <flash_layout_init+0xd6>
30008b80:	6858      	ldr	r0, [r3, #4]
30008b82:	2101      	movs	r1, #1
30008b84:	f853 7f0c 	ldr.w	r7, [r3, #12]!
30008b88:	2fff      	cmp	r7, #255	; 0xff
30008b8a:	d1f7      	bne.n	30008b7c <flash_layout_init+0x7c>
30008b8c:	b101      	cbz	r1, 30008b90 <flash_layout_init+0x90>
30008b8e:	6010      	str	r0, [r2, #0]
30008b90:	b104      	cbz	r4, 30008b94 <flash_layout_init+0x94>
30008b92:	6055      	str	r5, [r2, #4]
30008b94:	b10e      	cbz	r6, 30008b9a <flash_layout_init+0x9a>
30008b96:	9b00      	ldr	r3, [sp, #0]
30008b98:	6093      	str	r3, [r2, #8]
30008b9a:	f1b8 0f00 	cmp.w	r8, #0
30008b9e:	d001      	beq.n	30008ba4 <flash_layout_init+0xa4>
30008ba0:	9b01      	ldr	r3, [sp, #4]
30008ba2:	60d3      	str	r3, [r2, #12]
30008ba4:	9b02      	ldr	r3, [sp, #8]
30008ba6:	b10b      	cbz	r3, 30008bac <flash_layout_init+0xac>
30008ba8:	9b03      	ldr	r3, [sp, #12]
30008baa:	6113      	str	r3, [r2, #16]
30008bac:	9b04      	ldr	r3, [sp, #16]
30008bae:	2b00      	cmp	r3, #0
30008bb0:	d169      	bne.n	30008c86 <flash_layout_init+0x186>
30008bb2:	9b06      	ldr	r3, [sp, #24]
30008bb4:	2b00      	cmp	r3, #0
30008bb6:	d15f      	bne.n	30008c78 <flash_layout_init+0x178>
30008bb8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008bba:	2b00      	cmp	r3, #0
30008bbc:	d166      	bne.n	30008c8c <flash_layout_init+0x18c>
30008bbe:	f1b9 0f00 	cmp.w	r9, #0
30008bc2:	d005      	beq.n	30008bd0 <flash_layout_init+0xd0>
30008bc4:	4b38      	ldr	r3, [pc, #224]	; (30008ca8 <flash_layout_init+0x1a8>)
30008bc6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30008bc8:	601a      	str	r2, [r3, #0]
30008bca:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30008bcc:	f8cb 3000 	str.w	r3, [fp]
30008bd0:	b00f      	add	sp, #60	; 0x3c
30008bd2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008bd6:	2f01      	cmp	r7, #1
30008bd8:	d01a      	beq.n	30008c10 <flash_layout_init+0x110>
30008bda:	2f02      	cmp	r7, #2
30008bdc:	d01b      	beq.n	30008c16 <flash_layout_init+0x116>
30008bde:	2f03      	cmp	r7, #3
30008be0:	d01d      	beq.n	30008c1e <flash_layout_init+0x11e>
30008be2:	2f04      	cmp	r7, #4
30008be4:	d020      	beq.n	30008c28 <flash_layout_init+0x128>
30008be6:	2f05      	cmp	r7, #5
30008be8:	d023      	beq.n	30008c32 <flash_layout_init+0x132>
30008bea:	2f06      	cmp	r7, #6
30008bec:	d026      	beq.n	30008c3c <flash_layout_init+0x13c>
30008bee:	2f07      	cmp	r7, #7
30008bf0:	d035      	beq.n	30008c5e <flash_layout_init+0x15e>
30008bf2:	2f08      	cmp	r7, #8
30008bf4:	d1c6      	bne.n	30008b84 <flash_layout_init+0x84>
30008bf6:	689f      	ldr	r7, [r3, #8]
30008bf8:	f8d3 9004 	ldr.w	r9, [r3, #4]
30008bfc:	3701      	adds	r7, #1
30008bfe:	eba7 0709 	sub.w	r7, r7, r9
30008c02:	970d      	str	r7, [sp, #52]	; 0x34
30008c04:	f109 4778 	add.w	r7, r9, #4160749568	; 0xf8000000
30008c08:	f04f 0901 	mov.w	r9, #1
30008c0c:	970c      	str	r7, [sp, #48]	; 0x30
30008c0e:	e7b9      	b.n	30008b84 <flash_layout_init+0x84>
30008c10:	463c      	mov	r4, r7
30008c12:	685d      	ldr	r5, [r3, #4]
30008c14:	e7b6      	b.n	30008b84 <flash_layout_init+0x84>
30008c16:	685e      	ldr	r6, [r3, #4]
30008c18:	9600      	str	r6, [sp, #0]
30008c1a:	2601      	movs	r6, #1
30008c1c:	e7b2      	b.n	30008b84 <flash_layout_init+0x84>
30008c1e:	685f      	ldr	r7, [r3, #4]
30008c20:	f04f 0801 	mov.w	r8, #1
30008c24:	9701      	str	r7, [sp, #4]
30008c26:	e7ad      	b.n	30008b84 <flash_layout_init+0x84>
30008c28:	685f      	ldr	r7, [r3, #4]
30008c2a:	9703      	str	r7, [sp, #12]
30008c2c:	2701      	movs	r7, #1
30008c2e:	9702      	str	r7, [sp, #8]
30008c30:	e7a8      	b.n	30008b84 <flash_layout_init+0x84>
30008c32:	685f      	ldr	r7, [r3, #4]
30008c34:	9705      	str	r7, [sp, #20]
30008c36:	2701      	movs	r7, #1
30008c38:	9704      	str	r7, [sp, #16]
30008c3a:	e7a3      	b.n	30008b84 <flash_layout_init+0x84>
30008c3c:	689f      	ldr	r7, [r3, #8]
30008c3e:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008c42:	3701      	adds	r7, #1
30008c44:	eba7 070a 	sub.w	r7, r7, sl
30008c48:	9706      	str	r7, [sp, #24]
30008c4a:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008c4e:	9707      	str	r7, [sp, #28]
30008c50:	9f06      	ldr	r7, [sp, #24]
30008c52:	f3c7 3707 	ubfx	r7, r7, #12, #8
30008c56:	9708      	str	r7, [sp, #32]
30008c58:	2701      	movs	r7, #1
30008c5a:	9706      	str	r7, [sp, #24]
30008c5c:	e792      	b.n	30008b84 <flash_layout_init+0x84>
30008c5e:	689f      	ldr	r7, [r3, #8]
30008c60:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008c64:	3701      	adds	r7, #1
30008c66:	eba7 070a 	sub.w	r7, r7, sl
30008c6a:	970b      	str	r7, [sp, #44]	; 0x2c
30008c6c:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008c70:	9709      	str	r7, [sp, #36]	; 0x24
30008c72:	2701      	movs	r7, #1
30008c74:	970a      	str	r7, [sp, #40]	; 0x28
30008c76:	e785      	b.n	30008b84 <flash_layout_init+0x84>
30008c78:	9b07      	ldr	r3, [sp, #28]
30008c7a:	f8ce 3000 	str.w	r3, [lr]
30008c7e:	9b08      	ldr	r3, [sp, #32]
30008c80:	f88c 3000 	strb.w	r3, [ip]
30008c84:	e798      	b.n	30008bb8 <flash_layout_init+0xb8>
30008c86:	9b05      	ldr	r3, [sp, #20]
30008c88:	6153      	str	r3, [r2, #20]
30008c8a:	e792      	b.n	30008bb2 <flash_layout_init+0xb2>
30008c8c:	4b07      	ldr	r3, [pc, #28]	; (30008cac <flash_layout_init+0x1ac>)
30008c8e:	9a09      	ldr	r2, [sp, #36]	; 0x24
30008c90:	601a      	str	r2, [r3, #0]
30008c92:	4b07      	ldr	r3, [pc, #28]	; (30008cb0 <flash_layout_init+0x1b0>)
30008c94:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30008c96:	601a      	str	r2, [r3, #0]
30008c98:	e791      	b.n	30008bbe <flash_layout_init+0xbe>
30008c9a:	bf00      	nop
30008c9c:	3000a149 	.word	0x3000a149
30008ca0:	3000df3c 	.word	0x3000df3c
30008ca4:	3000dec4 	.word	0x3000dec4
30008ca8:	3000e584 	.word	0x3000e584
30008cac:	3000e57c 	.word	0x3000e57c
30008cb0:	3000e580 	.word	0x3000e580
30008cb4:	3000e564 	.word	0x3000e564
30008cb8:	3000e588 	.word	0x3000e588
30008cbc:	3000e58c 	.word	0x3000e58c
30008cc0:	3000df9c 	.word	0x3000df9c

30008cc4 <NAND_CHECK_IS_BAD_BLOCK>:
30008cc4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008cc8:	4e25      	ldr	r6, [pc, #148]	; (30008d60 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008cca:	0907      	lsrs	r7, r0, #4
30008ccc:	0045      	lsls	r5, r0, #1
30008cce:	b083      	sub	sp, #12
30008cd0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008cd4:	f005 051e 	and.w	r5, r5, #30
30008cd8:	40eb      	lsrs	r3, r5
30008cda:	f003 0303 	and.w	r3, r3, #3
30008cde:	2b02      	cmp	r3, #2
30008ce0:	d022      	beq.n	30008d28 <NAND_CHECK_IS_BAD_BLOCK+0x64>
30008ce2:	2b01      	cmp	r3, #1
30008ce4:	d024      	beq.n	30008d30 <NAND_CHECK_IS_BAD_BLOCK+0x6c>
30008ce6:	4b1f      	ldr	r3, [pc, #124]	; (30008d64 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008ce8:	f04f 0801 	mov.w	r8, #1
30008cec:	2204      	movs	r2, #4
30008cee:	4604      	mov	r4, r0
30008cf0:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008cf4:	0180      	lsls	r0, r0, #6
30008cf6:	ab01      	add	r3, sp, #4
30008cf8:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30008d70 <NAND_CHECK_IS_BAD_BLOCK+0xac>
30008cfc:	fa08 f101 	lsl.w	r1, r8, r1
30008d00:	47c8      	blx	r9
30008d02:	f89d 3004 	ldrb.w	r3, [sp, #4]
30008d06:	f89d 2005 	ldrb.w	r2, [sp, #5]
30008d0a:	4013      	ands	r3, r2
30008d0c:	2bff      	cmp	r3, #255	; 0xff
30008d0e:	d013      	beq.n	30008d38 <NAND_CHECK_IS_BAD_BLOCK+0x74>
30008d10:	2302      	movs	r3, #2
30008d12:	fa03 f505 	lsl.w	r5, r3, r5
30008d16:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008d1a:	432b      	orrs	r3, r5
30008d1c:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008d20:	4b11      	ldr	r3, [pc, #68]	; (30008d68 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008d22:	681b      	ldr	r3, [r3, #0]
30008d24:	079b      	lsls	r3, r3, #30
30008d26:	d412      	bmi.n	30008d4e <NAND_CHECK_IS_BAD_BLOCK+0x8a>
30008d28:	2001      	movs	r0, #1
30008d2a:	b003      	add	sp, #12
30008d2c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d30:	2000      	movs	r0, #0
30008d32:	b003      	add	sp, #12
30008d34:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d38:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008d3c:	fa08 f005 	lsl.w	r0, r8, r5
30008d40:	4303      	orrs	r3, r0
30008d42:	2000      	movs	r0, #0
30008d44:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008d48:	b003      	add	sp, #12
30008d4a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d4e:	4621      	mov	r1, r4
30008d50:	4806      	ldr	r0, [pc, #24]	; (30008d6c <NAND_CHECK_IS_BAD_BLOCK+0xa8>)
30008d52:	f003 f86d 	bl	3000be30 <__DiagPrintf_veneer>
30008d56:	4640      	mov	r0, r8
30008d58:	b003      	add	sp, #12
30008d5a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008d5e:	bf00      	nop
30008d60:	3000e598 	.word	0x3000e598
30008d64:	2001c01c 	.word	0x2001c01c
30008d68:	2001c00c 	.word	0x2001c00c
30008d6c:	3000cf50 	.word	0x3000cf50
30008d70:	3000a0a1 	.word	0x3000a0a1

30008d74 <Nand_Get_NandAddr>:
30008d74:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008d78:	4f10      	ldr	r7, [pc, #64]	; (30008dbc <Nand_Get_NandAddr+0x48>)
30008d7a:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008d7e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008d82:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008d86:	40e8      	lsrs	r0, r5
30008d88:	fa26 f505 	lsr.w	r5, r6, r5
30008d8c:	09ad      	lsrs	r5, r5, #6
30008d8e:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008d92:	d90f      	bls.n	30008db4 <Nand_Get_NandAddr+0x40>
30008d94:	0984      	lsrs	r4, r0, #6
30008d96:	f04f 0840 	mov.w	r8, #64	; 0x40
30008d9a:	4620      	mov	r0, r4
30008d9c:	3401      	adds	r4, #1
30008d9e:	f7ff ff91 	bl	30008cc4 <NAND_CHECK_IS_BAD_BLOCK>
30008da2:	b128      	cbz	r0, 30008db0 <Nand_Get_NandAddr+0x3c>
30008da4:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008da8:	3501      	adds	r5, #1
30008daa:	fa08 f303 	lsl.w	r3, r8, r3
30008dae:	441e      	add	r6, r3
30008db0:	42a5      	cmp	r5, r4
30008db2:	d8f2      	bhi.n	30008d9a <Nand_Get_NandAddr+0x26>
30008db4:	4630      	mov	r0, r6
30008db6:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008dba:	bf00      	nop
30008dbc:	2001c01c 	.word	0x2001c01c

30008dc0 <NandImgCopy>:
30008dc0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008dc4:	4b56      	ldr	r3, [pc, #344]	; (30008f20 <NandImgCopy+0x160>)
30008dc6:	b085      	sub	sp, #20
30008dc8:	4617      	mov	r7, r2
30008dca:	9003      	str	r0, [sp, #12]
30008dcc:	6898      	ldr	r0, [r3, #8]
30008dce:	4281      	cmp	r1, r0
30008dd0:	f0c0 8089 	bcc.w	30008ee6 <NandImgCopy+0x126>
30008dd4:	691b      	ldr	r3, [r3, #16]
30008dd6:	4299      	cmp	r1, r3
30008dd8:	d37b      	bcc.n	30008ed2 <NandImgCopy+0x112>
30008dda:	f8df 9154 	ldr.w	r9, [pc, #340]	; 30008f30 <NandImgCopy+0x170>
30008dde:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008de2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008de6:	fa25 f406 	lsr.w	r4, r5, r6
30008dea:	f107 38ff 	add.w	r8, r7, #4294967295
30008dee:	f1c4 0301 	rsb	r3, r4, #1
30008df2:	44a8      	add	r8, r5
30008df4:	fa28 f806 	lsr.w	r8, r8, r6
30008df8:	eb18 0803 	adds.w	r8, r8, r3
30008dfc:	d05e      	beq.n	30008ebc <NandImgCopy+0xfc>
30008dfe:	f108 38ff 	add.w	r8, r8, #4294967295
30008e02:	463b      	mov	r3, r7
30008e04:	f04f 0a00 	mov.w	sl, #0
30008e08:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008e0c:	4647      	mov	r7, r8
30008e0e:	4698      	mov	r8, r3
30008e10:	f8cd a008 	str.w	sl, [sp, #8]
30008e14:	e00a      	b.n	30008e2c <NandImgCopy+0x6c>
30008e16:	1ac9      	subs	r1, r1, r3
30008e18:	4b42      	ldr	r3, [pc, #264]	; (30008f24 <NandImgCopy+0x164>)
30008e1a:	689b      	ldr	r3, [r3, #8]
30008e1c:	4488      	add	r8, r1
30008e1e:	079a      	lsls	r2, r3, #30
30008e20:	d44f      	bmi.n	30008ec2 <NandImgCopy+0x102>
30008e22:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008e26:	3f01      	subs	r7, #1
30008e28:	fa25 f406 	lsr.w	r4, r5, r6
30008e2c:	9b02      	ldr	r3, [sp, #8]
30008e2e:	4632      	mov	r2, r6
30008e30:	09a6      	lsrs	r6, r4, #6
30008e32:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008e36:	d922      	bls.n	30008e7e <NandImgCopy+0xbe>
30008e38:	f1c2 0320 	rsb	r3, r2, #32
30008e3c:	f04f 31ff 	mov.w	r1, #4294967295
30008e40:	40d9      	lsrs	r1, r3
30008e42:	9b03      	ldr	r3, [sp, #12]
30008e44:	4029      	ands	r1, r5
30008e46:	4453      	add	r3, sl
30008e48:	b31f      	cbz	r7, 30008e92 <NandImgCopy+0xd2>
30008e4a:	2601      	movs	r6, #1
30008e4c:	4620      	mov	r0, r4
30008e4e:	9101      	str	r1, [sp, #4]
30008e50:	fa06 f202 	lsl.w	r2, r6, r2
30008e54:	4e34      	ldr	r6, [pc, #208]	; (30008f28 <NandImgCopy+0x168>)
30008e56:	9200      	str	r2, [sp, #0]
30008e58:	1a52      	subs	r2, r2, r1
30008e5a:	4492      	add	sl, r2
30008e5c:	4415      	add	r5, r2
30008e5e:	47b0      	blx	r6
30008e60:	f010 0f70 	tst.w	r0, #112	; 0x70
30008e64:	9b00      	ldr	r3, [sp, #0]
30008e66:	9901      	ldr	r1, [sp, #4]
30008e68:	d1d5      	bne.n	30008e16 <NandImgCopy+0x56>
30008e6a:	eba8 0803 	sub.w	r8, r8, r3
30008e6e:	4488      	add	r8, r1
30008e70:	e7d7      	b.n	30008e22 <NandImgCopy+0x62>
30008e72:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008e76:	3440      	adds	r4, #64	; 0x40
30008e78:	fa0b f303 	lsl.w	r3, fp, r3
30008e7c:	441d      	add	r5, r3
30008e7e:	4630      	mov	r0, r6
30008e80:	3601      	adds	r6, #1
30008e82:	f7ff ff1f 	bl	30008cc4 <NAND_CHECK_IS_BAD_BLOCK>
30008e86:	2800      	cmp	r0, #0
30008e88:	d1f3      	bne.n	30008e72 <NandImgCopy+0xb2>
30008e8a:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008e8e:	9602      	str	r6, [sp, #8]
30008e90:	e7d2      	b.n	30008e38 <NandImgCopy+0x78>
30008e92:	4642      	mov	r2, r8
30008e94:	4620      	mov	r0, r4
30008e96:	4d24      	ldr	r5, [pc, #144]	; (30008f28 <NandImgCopy+0x168>)
30008e98:	47a8      	blx	r5
30008e9a:	f010 0f70 	tst.w	r0, #112	; 0x70
30008e9e:	d00d      	beq.n	30008ebc <NandImgCopy+0xfc>
30008ea0:	4b20      	ldr	r3, [pc, #128]	; (30008f24 <NandImgCopy+0x164>)
30008ea2:	689b      	ldr	r3, [r3, #8]
30008ea4:	079b      	lsls	r3, r3, #30
30008ea6:	d509      	bpl.n	30008ebc <NandImgCopy+0xfc>
30008ea8:	4603      	mov	r3, r0
30008eaa:	09a2      	lsrs	r2, r4, #6
30008eac:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008eb0:	481e      	ldr	r0, [pc, #120]	; (30008f2c <NandImgCopy+0x16c>)
30008eb2:	b005      	add	sp, #20
30008eb4:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008eb8:	f002 bfba 	b.w	3000be30 <__DiagPrintf_veneer>
30008ebc:	b005      	add	sp, #20
30008ebe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008ec2:	4603      	mov	r3, r0
30008ec4:	09a2      	lsrs	r2, r4, #6
30008ec6:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008eca:	4818      	ldr	r0, [pc, #96]	; (30008f2c <NandImgCopy+0x16c>)
30008ecc:	f002 ffb0 	bl	3000be30 <__DiagPrintf_veneer>
30008ed0:	e7a7      	b.n	30008e22 <NandImgCopy+0x62>
30008ed2:	f8df 905c 	ldr.w	r9, [pc, #92]	; 30008f30 <NandImgCopy+0x170>
30008ed6:	f7ff ff4d 	bl	30008d74 <Nand_Get_NandAddr>
30008eda:	4605      	mov	r5, r0
30008edc:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008ee0:	fa20 f406 	lsr.w	r4, r0, r6
30008ee4:	e781      	b.n	30008dea <NandImgCopy+0x2a>
30008ee6:	f8df 9048 	ldr.w	r9, [pc, #72]	; 30008f30 <NandImgCopy+0x170>
30008eea:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008eee:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008ef2:	fa25 f406 	lsr.w	r4, r5, r6
30008ef6:	ea5f 1894 	movs.w	r8, r4, lsr #6
30008efa:	f43f af76 	beq.w	30008dea <NandImgCopy+0x2a>
30008efe:	2400      	movs	r4, #0
30008f00:	2640      	movs	r6, #64	; 0x40
30008f02:	4620      	mov	r0, r4
30008f04:	3401      	adds	r4, #1
30008f06:	f7ff fedd 	bl	30008cc4 <NAND_CHECK_IS_BAD_BLOCK>
30008f0a:	b130      	cbz	r0, 30008f1a <NandImgCopy+0x15a>
30008f0c:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008f10:	f108 0801 	add.w	r8, r8, #1
30008f14:	fa06 f303 	lsl.w	r3, r6, r3
30008f18:	441d      	add	r5, r3
30008f1a:	4544      	cmp	r4, r8
30008f1c:	d3f1      	bcc.n	30008f02 <NandImgCopy+0x142>
30008f1e:	e760      	b.n	30008de2 <NandImgCopy+0x22>
30008f20:	3000e8b8 	.word	0x3000e8b8
30008f24:	2001c00c 	.word	0x2001c00c
30008f28:	3000a0a1 	.word	0x3000a0a1
30008f2c:	3000cf7c 	.word	0x3000cf7c
30008f30:	2001c01c 	.word	0x2001c01c

30008f34 <OTP_Read8.part.0>:
30008f34:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008f38:	4b2f      	ldr	r3, [pc, #188]	; (30008ff8 <OTP_Read8.part.0+0xc4>)
30008f3a:	4680      	mov	r8, r0
30008f3c:	e843 f300 	tt	r3, r3
30008f40:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008f44:	4a2d      	ldr	r2, [pc, #180]	; (30008ffc <OTP_Read8.part.0+0xc8>)
30008f46:	4e2e      	ldr	r6, [pc, #184]	; (30009000 <OTP_Read8.part.0+0xcc>)
30008f48:	4689      	mov	r9, r1
30008f4a:	2b00      	cmp	r3, #0
30008f4c:	bf0e      	itee	eq
30008f4e:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008f52:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008f56:	4616      	movne	r6, r2
30008f58:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008f5a:	05d8      	lsls	r0, r3, #23
30008f5c:	d505      	bpl.n	30008f6a <OTP_Read8.part.0+0x36>
30008f5e:	4d29      	ldr	r5, [pc, #164]	; (30009004 <OTP_Read8.part.0+0xd0>)
30008f60:	2064      	movs	r0, #100	; 0x64
30008f62:	47a8      	blx	r5
30008f64:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008f66:	05d9      	lsls	r1, r3, #23
30008f68:	d4fa      	bmi.n	30008f60 <OTP_Read8.part.0+0x2c>
30008f6a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008f6c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008f70:	643b      	str	r3, [r7, #64]	; 0x40
30008f72:	4b25      	ldr	r3, [pc, #148]	; (30009008 <OTP_Read8.part.0+0xd4>)
30008f74:	681a      	ldr	r2, [r3, #0]
30008f76:	07d2      	lsls	r2, r2, #31
30008f78:	d403      	bmi.n	30008f82 <OTP_Read8.part.0+0x4e>
30008f7a:	681a      	ldr	r2, [r3, #0]
30008f7c:	f042 0201 	orr.w	r2, r2, #1
30008f80:	601a      	str	r2, [r3, #0]
30008f82:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008f86:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008f8a:	694a      	ldr	r2, [r1, #20]
30008f8c:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008f90:	614a      	str	r2, [r1, #20]
30008f92:	6033      	str	r3, [r6, #0]
30008f94:	6833      	ldr	r3, [r6, #0]
30008f96:	2b00      	cmp	r3, #0
30008f98:	db10      	blt.n	30008fbc <OTP_Read8.part.0+0x88>
30008f9a:	2400      	movs	r4, #0
30008f9c:	4d19      	ldr	r5, [pc, #100]	; (30009004 <OTP_Read8.part.0+0xd0>)
30008f9e:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008fa2:	e001      	b.n	30008fa8 <OTP_Read8.part.0+0x74>
30008fa4:	4554      	cmp	r4, sl
30008fa6:	d019      	beq.n	30008fdc <OTP_Read8.part.0+0xa8>
30008fa8:	2005      	movs	r0, #5
30008faa:	3401      	adds	r4, #1
30008fac:	47a8      	blx	r5
30008fae:	6833      	ldr	r3, [r6, #0]
30008fb0:	2b00      	cmp	r3, #0
30008fb2:	daf7      	bge.n	30008fa4 <OTP_Read8.part.0+0x70>
30008fb4:	f644 6320 	movw	r3, #20000	; 0x4e20
30008fb8:	429c      	cmp	r4, r3
30008fba:	d00f      	beq.n	30008fdc <OTP_Read8.part.0+0xa8>
30008fbc:	6833      	ldr	r3, [r6, #0]
30008fbe:	2001      	movs	r0, #1
30008fc0:	f889 3000 	strb.w	r3, [r9]
30008fc4:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008fc8:	6953      	ldr	r3, [r2, #20]
30008fca:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008fce:	6153      	str	r3, [r2, #20]
30008fd0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fd2:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008fd6:	643b      	str	r3, [r7, #64]	; 0x40
30008fd8:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008fdc:	22ff      	movs	r2, #255	; 0xff
30008fde:	4b0b      	ldr	r3, [pc, #44]	; (3000900c <OTP_Read8.part.0+0xd8>)
30008fe0:	f889 2000 	strb.w	r2, [r9]
30008fe4:	6818      	ldr	r0, [r3, #0]
30008fe6:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
30008fea:	d0eb      	beq.n	30008fc4 <OTP_Read8.part.0+0x90>
30008fec:	4641      	mov	r1, r8
30008fee:	4808      	ldr	r0, [pc, #32]	; (30009010 <OTP_Read8.part.0+0xdc>)
30008ff0:	f002 ff1e 	bl	3000be30 <__DiagPrintf_veneer>
30008ff4:	2000      	movs	r0, #0
30008ff6:	e7e5      	b.n	30008fc4 <OTP_Read8.part.0+0x90>
30008ff8:	0000e5f9 	.word	0x0000e5f9
30008ffc:	5200000c 	.word	0x5200000c
30009000:	42000008 	.word	0x42000008
30009004:	00009b2d 	.word	0x00009b2d
30009008:	42008000 	.word	0x42008000
3000900c:	2001c00c 	.word	0x2001c00c
30009010:	3000cfcc 	.word	0x3000cfcc

30009014 <OTP_Read8>:
30009014:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009018:	d201      	bcs.n	3000901e <OTP_Read8+0xa>
3000901a:	f7ff bf8b 	b.w	30008f34 <OTP_Read8.part.0>
3000901e:	22ff      	movs	r2, #255	; 0xff
30009020:	2000      	movs	r0, #0
30009022:	700a      	strb	r2, [r1, #0]
30009024:	4770      	bx	lr
30009026:	bf00      	nop

30009028 <EFUSE_PMAP_READ8>:
30009028:	4608      	mov	r0, r1
3000902a:	4611      	mov	r1, r2
3000902c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009030:	d201      	bcs.n	30009036 <EFUSE_PMAP_READ8+0xe>
30009032:	f7ff bf7f 	b.w	30008f34 <OTP_Read8.part.0>
30009036:	23ff      	movs	r3, #255	; 0xff
30009038:	2000      	movs	r0, #0
3000903a:	7013      	strb	r3, [r2, #0]
3000903c:	4770      	bx	lr
3000903e:	bf00      	nop

30009040 <PLL_NP_ClkSet>:
30009040:	b570      	push	{r4, r5, r6, lr}
30009042:	4604      	mov	r4, r0
30009044:	4b1b      	ldr	r3, [pc, #108]	; (300090b4 <PLL_NP_ClkSet+0x74>)
30009046:	4798      	blx	r3
30009048:	4d1b      	ldr	r5, [pc, #108]	; (300090b8 <PLL_NP_ClkSet+0x78>)
3000904a:	08c2      	lsrs	r2, r0, #3
3000904c:	fbb4 f3f0 	udiv	r3, r4, r0
30009050:	fb00 4c13 	mls	ip, r0, r3, r4
30009054:	1e9c      	subs	r4, r3, #2
30009056:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009058:	0164      	lsls	r4, r4, #5
3000905a:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
3000905e:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30009062:	431c      	orrs	r4, r3
30009064:	fbbc f3f2 	udiv	r3, ip, r2
30009068:	646c      	str	r4, [r5, #68]	; 0x44
3000906a:	041c      	lsls	r4, r3, #16
3000906c:	fb02 c013 	mls	r0, r2, r3, ip
30009070:	2300      	movs	r3, #0
30009072:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30009074:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30009078:	4619      	mov	r1, r3
3000907a:	fa1f fc86 	uxth.w	ip, r6
3000907e:	4e0f      	ldr	r6, [pc, #60]	; (300090bc <PLL_NP_ClkSet+0x7c>)
30009080:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30009084:	ea44 040c 	orr.w	r4, r4, ip
30009088:	f002 fd22 	bl	3000bad0 <__aeabi_uldivmod>
3000908c:	4603      	mov	r3, r0
3000908e:	2001      	movs	r0, #1
30009090:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30009094:	64ec      	str	r4, [r5, #76]	; 0x4c
30009096:	47b0      	blx	r6
30009098:	6c6b      	ldr	r3, [r5, #68]	; 0x44
3000909a:	203c      	movs	r0, #60	; 0x3c
3000909c:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300090a0:	646b      	str	r3, [r5, #68]	; 0x44
300090a2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300090a4:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300090a8:	646b      	str	r3, [r5, #68]	; 0x44
300090aa:	4633      	mov	r3, r6
300090ac:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
300090b0:	4718      	bx	r3
300090b2:	bf00      	nop
300090b4:	000099f5 	.word	0x000099f5
300090b8:	42008800 	.word	0x42008800
300090bc:	00009b2d 	.word	0x00009b2d

300090c0 <PLL_AP_ClkSet>:
300090c0:	b510      	push	{r4, lr}
300090c2:	4604      	mov	r4, r0
300090c4:	4b0b      	ldr	r3, [pc, #44]	; (300090f4 <PLL_AP_ClkSet+0x34>)
300090c6:	4798      	blx	r3
300090c8:	fbb4 f0f0 	udiv	r0, r4, r0
300090cc:	f1a0 0319 	sub.w	r3, r0, #25
300090d0:	1e84      	subs	r4, r0, #2
300090d2:	2b0f      	cmp	r3, #15
300090d4:	d807      	bhi.n	300090e6 <PLL_AP_ClkSet+0x26>
300090d6:	4b08      	ldr	r3, [pc, #32]	; (300090f8 <PLL_AP_ClkSet+0x38>)
300090d8:	b2e0      	uxtb	r0, r4
300090da:	695c      	ldr	r4, [r3, #20]
300090dc:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
300090e0:	4320      	orrs	r0, r4
300090e2:	6158      	str	r0, [r3, #20]
300090e4:	bd10      	pop	{r4, pc}
300090e6:	f240 1175 	movw	r1, #373	; 0x175
300090ea:	4804      	ldr	r0, [pc, #16]	; (300090fc <PLL_AP_ClkSet+0x3c>)
300090ec:	f002 fe90 	bl	3000be10 <__io_assert_failed_veneer>
300090f0:	e7f1      	b.n	300090d6 <PLL_AP_ClkSet+0x16>
300090f2:	bf00      	nop
300090f4:	000099f5 	.word	0x000099f5
300090f8:	41000300 	.word	0x41000300
300090fc:	3000cffc 	.word	0x3000cffc

30009100 <PLL_AP>:
30009100:	2801      	cmp	r0, #1
30009102:	b538      	push	{r3, r4, r5, lr}
30009104:	d009      	beq.n	3000911a <PLL_AP+0x1a>
30009106:	4b13      	ldr	r3, [pc, #76]	; (30009154 <PLL_AP+0x54>)
30009108:	681a      	ldr	r2, [r3, #0]
3000910a:	f022 0209 	bic.w	r2, r2, #9
3000910e:	601a      	str	r2, [r3, #0]
30009110:	681a      	ldr	r2, [r3, #0]
30009112:	f022 0206 	bic.w	r2, r2, #6
30009116:	601a      	str	r2, [r3, #0]
30009118:	bd38      	pop	{r3, r4, r5, pc}
3000911a:	4b0f      	ldr	r3, [pc, #60]	; (30009158 <PLL_AP+0x58>)
3000911c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
3000911e:	f012 0f05 	tst.w	r2, #5
30009122:	d00f      	beq.n	30009144 <PLL_AP+0x44>
30009124:	4d0d      	ldr	r5, [pc, #52]	; (3000915c <PLL_AP+0x5c>)
30009126:	4c0b      	ldr	r4, [pc, #44]	; (30009154 <PLL_AP+0x54>)
30009128:	2001      	movs	r0, #1
3000912a:	6823      	ldr	r3, [r4, #0]
3000912c:	f043 0306 	orr.w	r3, r3, #6
30009130:	6023      	str	r3, [r4, #0]
30009132:	47a8      	blx	r5
30009134:	6823      	ldr	r3, [r4, #0]
30009136:	f043 0309 	orr.w	r3, r3, #9
3000913a:	6023      	str	r3, [r4, #0]
3000913c:	6863      	ldr	r3, [r4, #4]
3000913e:	2b00      	cmp	r3, #0
30009140:	dafc      	bge.n	3000913c <PLL_AP+0x3c>
30009142:	bd38      	pop	{r3, r4, r5, pc}
30009144:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30009146:	20a0      	movs	r0, #160	; 0xa0
30009148:	4d04      	ldr	r5, [pc, #16]	; (3000915c <PLL_AP+0x5c>)
3000914a:	f042 0207 	orr.w	r2, r2, #7
3000914e:	679a      	str	r2, [r3, #120]	; 0x78
30009150:	47a8      	blx	r5
30009152:	e7e8      	b.n	30009126 <PLL_AP+0x26>
30009154:	41000300 	.word	0x41000300
30009158:	42008800 	.word	0x42008800
3000915c:	00009b2d 	.word	0x00009b2d

30009160 <PSRAM_INFO_Update>:
30009160:	4942      	ldr	r1, [pc, #264]	; (3000926c <PSRAM_INFO_Update+0x10c>)
30009162:	4a43      	ldr	r2, [pc, #268]	; (30009270 <PSRAM_INFO_Update+0x110>)
30009164:	b538      	push	{r3, r4, r5, lr}
30009166:	f8d1 1228 	ldr.w	r1, [r1, #552]	; 0x228
3000916a:	6815      	ldr	r5, [r2, #0]
3000916c:	4b41      	ldr	r3, [pc, #260]	; (30009274 <PSRAM_INFO_Update+0x114>)
3000916e:	f3c1 2102 	ubfx	r1, r1, #8, #3
30009172:	4c41      	ldr	r4, [pc, #260]	; (30009278 <PSRAM_INFO_Update+0x118>)
30009174:	3101      	adds	r1, #1
30009176:	fb03 f505 	mul.w	r5, r3, r5
3000917a:	4b40      	ldr	r3, [pc, #256]	; (3000927c <PSRAM_INFO_Update+0x11c>)
3000917c:	fbb5 f5f1 	udiv	r5, r5, r1
30009180:	086a      	lsrs	r2, r5, #1
30009182:	60e2      	str	r2, [r4, #12]
30009184:	4798      	blx	r3
30009186:	4b3e      	ldr	r3, [pc, #248]	; (30009280 <PSRAM_INFO_Update+0x120>)
30009188:	6020      	str	r0, [r4, #0]
3000918a:	4798      	blx	r3
3000918c:	4b3d      	ldr	r3, [pc, #244]	; (30009284 <PSRAM_INFO_Update+0x124>)
3000918e:	220a      	movs	r2, #10
30009190:	60a0      	str	r0, [r4, #8]
30009192:	6122      	str	r2, [r4, #16]
30009194:	2220      	movs	r2, #32
30009196:	fbb3 f3f5 	udiv	r3, r3, r5
3000919a:	6163      	str	r3, [r4, #20]
3000919c:	fbb2 f3f3 	udiv	r3, r2, r3
300091a0:	6822      	ldr	r2, [r4, #0]
300091a2:	6223      	str	r3, [r4, #32]
300091a4:	2a01      	cmp	r2, #1
300091a6:	68e3      	ldr	r3, [r4, #12]
300091a8:	4a37      	ldr	r2, [pc, #220]	; (30009288 <PSRAM_INFO_Update+0x128>)
300091aa:	d014      	beq.n	300091d6 <PSRAM_INFO_Update+0x76>
300091ac:	4293      	cmp	r3, r2
300091ae:	d808      	bhi.n	300091c2 <PSRAM_INFO_Update+0x62>
300091b0:	2204      	movs	r2, #4
300091b2:	2308      	movs	r3, #8
300091b4:	e9c4 2306 	strd	r2, r3, [r4, #24]
300091b8:	4b34      	ldr	r3, [pc, #208]	; (3000928c <PSRAM_INFO_Update+0x12c>)
300091ba:	689b      	ldr	r3, [r3, #8]
300091bc:	079b      	lsls	r3, r3, #30
300091be:	d414      	bmi.n	300091ea <PSRAM_INFO_Update+0x8a>
300091c0:	bd38      	pop	{r3, r4, r5, pc}
300091c2:	4a33      	ldr	r2, [pc, #204]	; (30009290 <PSRAM_INFO_Update+0x130>)
300091c4:	4293      	cmp	r3, r2
300091c6:	d91b      	bls.n	30009200 <PSRAM_INFO_Update+0xa0>
300091c8:	4a32      	ldr	r2, [pc, #200]	; (30009294 <PSRAM_INFO_Update+0x134>)
300091ca:	4293      	cmp	r3, r2
300091cc:	d828      	bhi.n	30009220 <PSRAM_INFO_Update+0xc0>
300091ce:	2306      	movs	r3, #6
300091d0:	e9c4 3306 	strd	r3, r3, [r4, #24]
300091d4:	e7f0      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
300091d6:	4293      	cmp	r3, r2
300091d8:	d90d      	bls.n	300091f6 <PSRAM_INFO_Update+0x96>
300091da:	4a2d      	ldr	r2, [pc, #180]	; (30009290 <PSRAM_INFO_Update+0x130>)
300091dc:	4293      	cmp	r3, r2
300091de:	d814      	bhi.n	3000920a <PSRAM_INFO_Update+0xaa>
300091e0:	2205      	movs	r2, #5
300091e2:	230f      	movs	r3, #15
300091e4:	e9c4 2306 	strd	r2, r3, [r4, #24]
300091e8:	e7e6      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
300091ea:	4629      	mov	r1, r5
300091ec:	482a      	ldr	r0, [pc, #168]	; (30009298 <PSRAM_INFO_Update+0x138>)
300091ee:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300091f2:	f002 be1d 	b.w	3000be30 <__DiagPrintf_veneer>
300091f6:	2204      	movs	r2, #4
300091f8:	230f      	movs	r3, #15
300091fa:	e9c4 2306 	strd	r2, r3, [r4, #24]
300091fe:	e7db      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
30009200:	2205      	movs	r2, #5
30009202:	2308      	movs	r3, #8
30009204:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009208:	e7d6      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
3000920a:	4a22      	ldr	r2, [pc, #136]	; (30009294 <PSRAM_INFO_Update+0x134>)
3000920c:	4293      	cmp	r3, r2
3000920e:	d91b      	bls.n	30009248 <PSRAM_INFO_Update+0xe8>
30009210:	4a22      	ldr	r2, [pc, #136]	; (3000929c <PSRAM_INFO_Update+0x13c>)
30009212:	4293      	cmp	r3, r2
30009214:	d822      	bhi.n	3000925c <PSRAM_INFO_Update+0xfc>
30009216:	2207      	movs	r2, #7
30009218:	2314      	movs	r3, #20
3000921a:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000921e:	e7cb      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
30009220:	4a1e      	ldr	r2, [pc, #120]	; (3000929c <PSRAM_INFO_Update+0x13c>)
30009222:	4293      	cmp	r3, r2
30009224:	d915      	bls.n	30009252 <PSRAM_INFO_Update+0xf2>
30009226:	4a1e      	ldr	r2, [pc, #120]	; (300092a0 <PSRAM_INFO_Update+0x140>)
30009228:	4293      	cmp	r3, r2
3000922a:	bf8c      	ite	hi
3000922c:	2300      	movhi	r3, #0
3000922e:	2301      	movls	r3, #1
30009230:	2804      	cmp	r0, #4
30009232:	d100      	bne.n	30009236 <PSRAM_INFO_Update+0xd6>
30009234:	b96b      	cbnz	r3, 30009252 <PSRAM_INFO_Update+0xf2>
30009236:	2803      	cmp	r0, #3
30009238:	d1be      	bne.n	300091b8 <PSRAM_INFO_Update+0x58>
3000923a:	2b00      	cmp	r3, #0
3000923c:	d0bc      	beq.n	300091b8 <PSRAM_INFO_Update+0x58>
3000923e:	220a      	movs	r2, #10
30009240:	2306      	movs	r3, #6
30009242:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009246:	e7b7      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
30009248:	2206      	movs	r2, #6
3000924a:	2312      	movs	r3, #18
3000924c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009250:	e7b2      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
30009252:	2207      	movs	r2, #7
30009254:	2306      	movs	r3, #6
30009256:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000925a:	e7ad      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
3000925c:	4a10      	ldr	r2, [pc, #64]	; (300092a0 <PSRAM_INFO_Update+0x140>)
3000925e:	4293      	cmp	r3, r2
30009260:	d8aa      	bhi.n	300091b8 <PSRAM_INFO_Update+0x58>
30009262:	2208      	movs	r2, #8
30009264:	231c      	movs	r3, #28
30009266:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000926a:	e7a5      	b.n	300091b8 <PSRAM_INFO_Update+0x58>
3000926c:	42008000 	.word	0x42008000
30009270:	3000deac 	.word	0x3000deac
30009274:	000f4240 	.word	0x000f4240
30009278:	3000e5a0 	.word	0x3000e5a0
3000927c:	30007e8d 	.word	0x30007e8d
30009280:	30007e55 	.word	0x30007e55
30009284:	3b9aca00 	.word	0x3b9aca00
30009288:	0632ea00 	.word	0x0632ea00
3000928c:	2001c00c 	.word	0x2001c00c
30009290:	07ed6b40 	.word	0x07ed6b40
30009294:	09e4f580 	.word	0x09e4f580
30009298:	3000d00c 	.word	0x3000d00c
3000929c:	0bebc200 	.word	0x0bebc200
300092a0:	0ee6b280 	.word	0x0ee6b280

300092a4 <PSRAM_PHY_StructInit>:
300092a4:	2203      	movs	r2, #3
300092a6:	2301      	movs	r3, #1
300092a8:	210a      	movs	r1, #10
300092aa:	b410      	push	{r4}
300092ac:	e9c0 2202 	strd	r2, r2, [r0, #8]
300092b0:	2402      	movs	r4, #2
300092b2:	2207      	movs	r2, #7
300092b4:	e9c0 3400 	strd	r3, r4, [r0]
300092b8:	e9c0 1304 	strd	r1, r3, [r0, #16]
300092bc:	f85d 4b04 	ldr.w	r4, [sp], #4
300092c0:	6182      	str	r2, [r0, #24]
300092c2:	4770      	bx	lr

300092c4 <PSRAM_PHY_Init>:
300092c4:	b538      	push	{r3, r4, r5, lr}
300092c6:	4604      	mov	r4, r0
300092c8:	6903      	ldr	r3, [r0, #16]
300092ca:	4d14      	ldr	r5, [pc, #80]	; (3000931c <PSRAM_PHY_Init+0x58>)
300092cc:	f003 031f 	and.w	r3, r3, #31
300092d0:	4913      	ldr	r1, [pc, #76]	; (30009320 <PSRAM_PHY_Init+0x5c>)
300092d2:	e9d0 0200 	ldrd	r0, r2, [r0]
300092d6:	0612      	lsls	r2, r2, #24
300092d8:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
300092dc:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
300092e0:	4313      	orrs	r3, r2
300092e2:	68a2      	ldr	r2, [r4, #8]
300092e4:	0412      	lsls	r2, r2, #16
300092e6:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
300092ea:	4313      	orrs	r3, r2
300092ec:	68e2      	ldr	r2, [r4, #12]
300092ee:	0212      	lsls	r2, r2, #8
300092f0:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300092f4:	4313      	orrs	r3, r2
300092f6:	606b      	str	r3, [r5, #4]
300092f8:	4788      	blx	r1
300092fa:	2801      	cmp	r0, #1
300092fc:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30009300:	bf08      	it	eq
30009302:	4a08      	ldreq	r2, [pc, #32]	; (30009324 <PSRAM_PHY_Init+0x60>)
30009304:	ea4f 1303 	mov.w	r3, r3, lsl #4
30009308:	f001 010f 	and.w	r1, r1, #15
3000930c:	bf18      	it	ne
3000930e:	4a06      	ldrne	r2, [pc, #24]	; (30009328 <PSRAM_PHY_Init+0x64>)
30009310:	b2db      	uxtb	r3, r3
30009312:	430b      	orrs	r3, r1
30009314:	431a      	orrs	r2, r3
30009316:	61aa      	str	r2, [r5, #24]
30009318:	bd38      	pop	{r3, r4, r5, pc}
3000931a:	bf00      	nop
3000931c:	41002000 	.word	0x41002000
30009320:	30007e8d 	.word	0x30007e8d
30009324:	80000e00 	.word	0x80000e00
30009328:	80000f00 	.word	0x80000f00

3000932c <PSRAM_CTRL_Init>:
3000932c:	b470      	push	{r4, r5, r6}
3000932e:	4b4c      	ldr	r3, [pc, #304]	; (30009460 <PSRAM_CTRL_Init+0x134>)
30009330:	2400      	movs	r4, #0
30009332:	4a4c      	ldr	r2, [pc, #304]	; (30009464 <PSRAM_CTRL_Init+0x138>)
30009334:	f24d 0101 	movw	r1, #53249	; 0xd001
30009338:	609c      	str	r4, [r3, #8]
3000933a:	2001      	movs	r0, #1
3000933c:	601a      	str	r2, [r3, #0]
3000933e:	4a4a      	ldr	r2, [pc, #296]	; (30009468 <PSRAM_CTRL_Init+0x13c>)
30009340:	6158      	str	r0, [r3, #20]
30009342:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30009346:	6811      	ldr	r1, [r2, #0]
30009348:	2902      	cmp	r1, #2
3000934a:	d04a      	beq.n	300093e2 <PSRAM_CTRL_Init+0xb6>
3000934c:	6954      	ldr	r4, [r2, #20]
3000934e:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30009352:	69d0      	ldr	r0, [r2, #28]
30009354:	6915      	ldr	r5, [r2, #16]
30009356:	fbb0 f0f4 	udiv	r0, r0, r4
3000935a:	fbb1 f1f4 	udiv	r1, r1, r4
3000935e:	6994      	ldr	r4, [r2, #24]
30009360:	f005 020f 	and.w	r2, r5, #15
30009364:	f241 5551 	movw	r5, #5457	; 0x1551
30009368:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
3000936c:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009370:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30009374:	094a      	lsrs	r2, r1, #5
30009376:	0181      	lsls	r1, r0, #6
30009378:	f000 003f 	and.w	r0, r0, #63	; 0x3f
3000937c:	4d3b      	ldr	r5, [pc, #236]	; (3000946c <PSRAM_CTRL_Init+0x140>)
3000937e:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30009382:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
30009386:	00a2      	lsls	r2, r4, #2
30009388:	4301      	orrs	r1, r0
3000938a:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
3000938e:	3a01      	subs	r2, #1
30009390:	430d      	orrs	r5, r1
30009392:	f242 0120 	movw	r1, #8224	; 0x2020
30009396:	f3c2 020b 	ubfx	r2, r2, #0, #12
3000939a:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
3000939e:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300093a2:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300093a6:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300093aa:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300093ae:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300093b2:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300093b6:	4420      	add	r0, r4
300093b8:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300093bc:	f640 71fe 	movw	r1, #4094	; 0xffe
300093c0:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300093c4:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300093c8:	432a      	orrs	r2, r5
300093ca:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300093ce:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300093d2:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300093d6:	4a22      	ldr	r2, [pc, #136]	; (30009460 <PSRAM_CTRL_Init+0x134>)
300093d8:	6a93      	ldr	r3, [r2, #40]	; 0x28
300093da:	061b      	lsls	r3, r3, #24
300093dc:	d5fc      	bpl.n	300093d8 <PSRAM_CTRL_Init+0xac>
300093de:	bc70      	pop	{r4, r5, r6}
300093e0:	4770      	bx	lr
300093e2:	6955      	ldr	r5, [r2, #20]
300093e4:	207d      	movs	r0, #125	; 0x7d
300093e6:	69d1      	ldr	r1, [r2, #28]
300093e8:	26a0      	movs	r6, #160	; 0xa0
300093ea:	6914      	ldr	r4, [r2, #16]
300093ec:	6992      	ldr	r2, [r2, #24]
300093ee:	f004 040f 	and.w	r4, r4, #15
300093f2:	0092      	lsls	r2, r2, #2
300093f4:	fbb1 f1f5 	udiv	r1, r1, r5
300093f8:	fbb0 f5f5 	udiv	r5, r0, r5
300093fc:	f245 5051 	movw	r0, #21841	; 0x5551
30009400:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30009404:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30009408:	0189      	lsls	r1, r1, #6
3000940a:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
3000940e:	f641 4570 	movw	r5, #7280	; 0x1c70
30009412:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30009416:	4325      	orrs	r5, r4
30009418:	2420      	movs	r4, #32
3000941a:	4308      	orrs	r0, r1
3000941c:	1e51      	subs	r1, r2, #1
3000941e:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30009422:	3a02      	subs	r2, #2
30009424:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30009428:	f3c1 010b 	ubfx	r1, r1, #0, #12
3000942c:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009430:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30009434:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30009438:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
3000943c:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009440:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30009444:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30009448:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
3000944c:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009450:	4301      	orrs	r1, r0
30009452:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30009456:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000945a:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
3000945e:	e7ba      	b.n	300093d6 <PSRAM_CTRL_Init+0xaa>
30009460:	41012000 	.word	0x41012000
30009464:	00bfe000 	.word	0x00bfe000
30009468:	3000e5a0 	.word	0x3000e5a0
3000946c:	00107000 	.word	0x00107000

30009470 <PSRAM_REG_Write>:
30009470:	b430      	push	{r4, r5}
30009472:	4c20      	ldr	r4, [pc, #128]	; (300094f4 <PSRAM_REG_Write+0x84>)
30009474:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30009476:	f013 0301 	ands.w	r3, r3, #1
3000947a:	d1fb      	bne.n	30009474 <PSRAM_REG_Write+0x4>
3000947c:	6825      	ldr	r5, [r4, #0]
3000947e:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009482:	6025      	str	r5, [r4, #0]
30009484:	60a3      	str	r3, [r4, #8]
30009486:	6825      	ldr	r5, [r4, #0]
30009488:	f425 7540 	bic.w	r5, r5, #768	; 0x300
3000948c:	6025      	str	r5, [r4, #0]
3000948e:	6063      	str	r3, [r4, #4]
30009490:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30009494:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30009498:	23c0      	movs	r3, #192	; 0xc0
3000949a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000949e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300094a2:	0e03      	lsrs	r3, r0, #24
300094a4:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300094a8:	f3c0 4307 	ubfx	r3, r0, #16, #8
300094ac:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300094b0:	f3c0 2307 	ubfx	r3, r0, #8, #8
300094b4:	b2c0      	uxtb	r0, r0
300094b6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300094ba:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300094be:	b141      	cbz	r1, 300094d2 <PSRAM_REG_Write+0x62>
300094c0:	3a01      	subs	r2, #1
300094c2:	480c      	ldr	r0, [pc, #48]	; (300094f4 <PSRAM_REG_Write+0x84>)
300094c4:	4411      	add	r1, r2
300094c6:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300094ca:	428a      	cmp	r2, r1
300094cc:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300094d0:	d1f9      	bne.n	300094c6 <PSRAM_REG_Write+0x56>
300094d2:	4a08      	ldr	r2, [pc, #32]	; (300094f4 <PSRAM_REG_Write+0x84>)
300094d4:	2301      	movs	r3, #1
300094d6:	6093      	str	r3, [r2, #8]
300094d8:	6893      	ldr	r3, [r2, #8]
300094da:	07d9      	lsls	r1, r3, #31
300094dc:	d4fc      	bmi.n	300094d8 <PSRAM_REG_Write+0x68>
300094de:	4a05      	ldr	r2, [pc, #20]	; (300094f4 <PSRAM_REG_Write+0x84>)
300094e0:	6a93      	ldr	r3, [r2, #40]	; 0x28
300094e2:	07db      	lsls	r3, r3, #31
300094e4:	d4fc      	bmi.n	300094e0 <PSRAM_REG_Write+0x70>
300094e6:	6813      	ldr	r3, [r2, #0]
300094e8:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300094ec:	6013      	str	r3, [r2, #0]
300094ee:	bc30      	pop	{r4, r5}
300094f0:	4770      	bx	lr
300094f2:	bf00      	nop
300094f4:	41012000 	.word	0x41012000

300094f8 <PSRAM_APM_DEVIC_Init>:
300094f8:	4b12      	ldr	r3, [pc, #72]	; (30009544 <PSRAM_APM_DEVIC_Init+0x4c>)
300094fa:	2102      	movs	r1, #2
300094fc:	2000      	movs	r0, #0
300094fe:	b530      	push	{r4, r5, lr}
30009500:	4c11      	ldr	r4, [pc, #68]	; (30009548 <PSRAM_APM_DEVIC_Init+0x50>)
30009502:	b083      	sub	sp, #12
30009504:	699d      	ldr	r5, [r3, #24]
30009506:	466a      	mov	r2, sp
30009508:	eb04 0c05 	add.w	ip, r4, r5
3000950c:	1eeb      	subs	r3, r5, #3
3000950e:	4c0f      	ldr	r4, [pc, #60]	; (3000954c <PSRAM_APM_DEVIC_Init+0x54>)
30009510:	408b      	lsls	r3, r1
30009512:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30009516:	f003 031c 	and.w	r3, r3, #28
3000951a:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000951e:	f043 0320 	orr.w	r3, r3, #32
30009522:	fa5f fc8c 	uxtb.w	ip, ip
30009526:	f88d 3000 	strb.w	r3, [sp]
3000952a:	f88d c004 	strb.w	ip, [sp, #4]
3000952e:	f88d 3001 	strb.w	r3, [sp, #1]
30009532:	f88d c005 	strb.w	ip, [sp, #5]
30009536:	47a0      	blx	r4
30009538:	aa01      	add	r2, sp, #4
3000953a:	2102      	movs	r1, #2
3000953c:	2004      	movs	r0, #4
3000953e:	47a0      	blx	r4
30009540:	b003      	add	sp, #12
30009542:	bd30      	pop	{r4, r5, pc}
30009544:	3000e5a0 	.word	0x3000e5a0
30009548:	3000dfa0 	.word	0x3000dfa0
3000954c:	30009471 	.word	0x30009471

30009550 <PSRAM_WB_REG_Write>:
30009550:	b430      	push	{r4, r5}
30009552:	4c1e      	ldr	r4, [pc, #120]	; (300095cc <PSRAM_WB_REG_Write+0x7c>)
30009554:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30009556:	f013 0301 	ands.w	r3, r3, #1
3000955a:	d1fb      	bne.n	30009554 <PSRAM_WB_REG_Write+0x4>
3000955c:	6825      	ldr	r5, [r4, #0]
3000955e:	b2c0      	uxtb	r0, r0
30009560:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009564:	6025      	str	r5, [r4, #0]
30009566:	60a3      	str	r3, [r4, #8]
30009568:	6825      	ldr	r5, [r4, #0]
3000956a:	f425 7540 	bic.w	r5, r5, #768	; 0x300
3000956e:	6025      	str	r5, [r4, #0]
30009570:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30009574:	6063      	str	r3, [r4, #4]
30009576:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000957a:	2560      	movs	r5, #96	; 0x60
3000957c:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009580:	2501      	movs	r5, #1
30009582:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009586:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
3000958a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000958e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009592:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30009596:	b141      	cbz	r1, 300095aa <PSRAM_WB_REG_Write+0x5a>
30009598:	3a01      	subs	r2, #1
3000959a:	480c      	ldr	r0, [pc, #48]	; (300095cc <PSRAM_WB_REG_Write+0x7c>)
3000959c:	4411      	add	r1, r2
3000959e:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300095a2:	428a      	cmp	r2, r1
300095a4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300095a8:	d1f9      	bne.n	3000959e <PSRAM_WB_REG_Write+0x4e>
300095aa:	4a08      	ldr	r2, [pc, #32]	; (300095cc <PSRAM_WB_REG_Write+0x7c>)
300095ac:	2301      	movs	r3, #1
300095ae:	6093      	str	r3, [r2, #8]
300095b0:	6893      	ldr	r3, [r2, #8]
300095b2:	07d9      	lsls	r1, r3, #31
300095b4:	d4fc      	bmi.n	300095b0 <PSRAM_WB_REG_Write+0x60>
300095b6:	4a05      	ldr	r2, [pc, #20]	; (300095cc <PSRAM_WB_REG_Write+0x7c>)
300095b8:	6a93      	ldr	r3, [r2, #40]	; 0x28
300095ba:	07db      	lsls	r3, r3, #31
300095bc:	d4fc      	bmi.n	300095b8 <PSRAM_WB_REG_Write+0x68>
300095be:	6813      	ldr	r3, [r2, #0]
300095c0:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300095c4:	6013      	str	r3, [r2, #0]
300095c6:	bc30      	pop	{r4, r5}
300095c8:	4770      	bx	lr
300095ca:	bf00      	nop
300095cc:	41012000 	.word	0x41012000

300095d0 <PSRAM_WB_DEVIC_Init>:
300095d0:	490b      	ldr	r1, [pc, #44]	; (30009600 <PSRAM_WB_DEVIC_Init+0x30>)
300095d2:	4b0c      	ldr	r3, [pc, #48]	; (30009604 <PSRAM_WB_DEVIC_Init+0x34>)
300095d4:	b510      	push	{r4, lr}
300095d6:	6988      	ldr	r0, [r1, #24]
300095d8:	b082      	sub	sp, #8
300095da:	248f      	movs	r4, #143	; 0x8f
300095dc:	2102      	movs	r1, #2
300095de:	4403      	add	r3, r0
300095e0:	aa01      	add	r2, sp, #4
300095e2:	f88d 4004 	strb.w	r4, [sp, #4]
300095e6:	2000      	movs	r0, #0
300095e8:	f813 3c03 	ldrb.w	r3, [r3, #-3]
300095ec:	4c06      	ldr	r4, [pc, #24]	; (30009608 <PSRAM_WB_DEVIC_Init+0x38>)
300095ee:	011b      	lsls	r3, r3, #4
300095f0:	f043 030c 	orr.w	r3, r3, #12
300095f4:	f88d 3005 	strb.w	r3, [sp, #5]
300095f8:	47a0      	blx	r4
300095fa:	b002      	add	sp, #8
300095fc:	bd10      	pop	{r4, pc}
300095fe:	bf00      	nop
30009600:	3000e5a0 	.word	0x3000e5a0
30009604:	3000dfc0 	.word	0x3000dfc0
30009608:	30009551 	.word	0x30009551

3000960c <PSRAM_calibration>:
3000960c:	4a9d      	ldr	r2, [pc, #628]	; (30009884 <PSRAM_calibration+0x278>)
3000960e:	2300      	movs	r3, #0
30009610:	499d      	ldr	r1, [pc, #628]	; (30009888 <PSRAM_calibration+0x27c>)
30009612:	489e      	ldr	r0, [pc, #632]	; (3000988c <PSRAM_calibration+0x280>)
30009614:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009618:	6852      	ldr	r2, [r2, #4]
3000961a:	b091      	sub	sp, #68	; 0x44
3000961c:	e9cd 2309 	strd	r2, r3, [sp, #36]	; 0x24
30009620:	400a      	ands	r2, r1
30009622:	930f      	str	r3, [sp, #60]	; 0x3c
30009624:	e9cd 330b 	strd	r3, r3, [sp, #44]	; 0x2c
30009628:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000962c:	9208      	str	r2, [sp, #32]
3000962e:	6942      	ldr	r2, [r0, #20]
30009630:	03d1      	lsls	r1, r2, #15
30009632:	d51e      	bpl.n	30009672 <PSRAM_calibration+0x66>
30009634:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009638:	f3bf 8f4f 	dsb	sy
3000963c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009640:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009644:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009648:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000964c:	0164      	lsls	r4, r4, #5
3000964e:	ea04 0106 	and.w	r1, r4, r6
30009652:	462b      	mov	r3, r5
30009654:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009658:	3b01      	subs	r3, #1
3000965a:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000965e:	1c5a      	adds	r2, r3, #1
30009660:	d1f8      	bne.n	30009654 <PSRAM_calibration+0x48>
30009662:	3c20      	subs	r4, #32
30009664:	f114 0f20 	cmn.w	r4, #32
30009668:	d1f1      	bne.n	3000964e <PSRAM_calibration+0x42>
3000966a:	f3bf 8f4f 	dsb	sy
3000966e:	f3bf 8f6f 	isb	sy
30009672:	4886      	ldr	r0, [pc, #536]	; (3000988c <PSRAM_calibration+0x280>)
30009674:	2300      	movs	r3, #0
30009676:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000967a:	f3bf 8f4f 	dsb	sy
3000967e:	6943      	ldr	r3, [r0, #20]
30009680:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009684:	6143      	str	r3, [r0, #20]
30009686:	f3bf 8f4f 	dsb	sy
3000968a:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000968e:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009692:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009696:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000969a:	0164      	lsls	r4, r4, #5
3000969c:	ea04 0106 	and.w	r1, r4, r6
300096a0:	462b      	mov	r3, r5
300096a2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300096a6:	3b01      	subs	r3, #1
300096a8:	1c5f      	adds	r7, r3, #1
300096aa:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300096ae:	d1f8      	bne.n	300096a2 <PSRAM_calibration+0x96>
300096b0:	3c20      	subs	r4, #32
300096b2:	f114 0f20 	cmn.w	r4, #32
300096b6:	d1f1      	bne.n	3000969c <PSRAM_calibration+0x90>
300096b8:	f3bf 8f4f 	dsb	sy
300096bc:	f3bf 8f6f 	isb	sy
300096c0:	2204      	movs	r2, #4
300096c2:	461e      	mov	r6, r3
300096c4:	9304      	str	r3, [sp, #16]
300096c6:	2700      	movs	r7, #0
300096c8:	9207      	str	r2, [sp, #28]
300096ca:	46b3      	mov	fp, r6
300096cc:	4a6d      	ldr	r2, [pc, #436]	; (30009884 <PSRAM_calibration+0x278>)
300096ce:	4c70      	ldr	r4, [pc, #448]	; (30009890 <PSRAM_calibration+0x284>)
300096d0:	6813      	ldr	r3, [r2, #0]
300096d2:	f8df 91d8 	ldr.w	r9, [pc, #472]	; 300098ac <PSRAM_calibration+0x2a0>
300096d6:	f023 0301 	bic.w	r3, r3, #1
300096da:	f8df a1d4 	ldr.w	sl, [pc, #468]	; 300098b0 <PSRAM_calibration+0x2a4>
300096de:	6013      	str	r3, [r2, #0]
300096e0:	2301      	movs	r3, #1
300096e2:	9303      	str	r3, [sp, #12]
300096e4:	9b08      	ldr	r3, [sp, #32]
300096e6:	9a03      	ldr	r2, [sp, #12]
300096e8:	9702      	str	r7, [sp, #8]
300096ea:	2700      	movs	r7, #0
300096ec:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
300096f0:	463d      	mov	r5, r7
300096f2:	e9cd 6b05 	strd	r6, fp, [sp, #20]
300096f6:	f04f 3bff 	mov.w	fp, #4294967295
300096fa:	465e      	mov	r6, fp
300096fc:	e010      	b.n	30009720 <PSRAM_calibration+0x114>
300096fe:	f1b6 3fff 	cmp.w	r6, #4294967295
30009702:	bf08      	it	eq
30009704:	462e      	moveq	r6, r5
30009706:	2d1f      	cmp	r5, #31
30009708:	eb06 0b07 	add.w	fp, r6, r7
3000970c:	f107 0701 	add.w	r7, r7, #1
30009710:	d102      	bne.n	30009718 <PSRAM_calibration+0x10c>
30009712:	9b02      	ldr	r3, [sp, #8]
30009714:	429f      	cmp	r7, r3
30009716:	dc46      	bgt.n	300097a6 <PSRAM_calibration+0x19a>
30009718:	3501      	adds	r5, #1
3000971a:	2d20      	cmp	r5, #32
3000971c:	f000 80ae 	beq.w	3000987c <PSRAM_calibration+0x270>
30009720:	ea48 0205 	orr.w	r2, r8, r5
30009724:	4b57      	ldr	r3, [pc, #348]	; (30009884 <PSRAM_calibration+0x278>)
30009726:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000972a:	495a      	ldr	r1, [pc, #360]	; (30009894 <PSRAM_calibration+0x288>)
3000972c:	605a      	str	r2, [r3, #4]
3000972e:	6822      	ldr	r2, [r4, #0]
30009730:	4b59      	ldr	r3, [pc, #356]	; (30009898 <PSRAM_calibration+0x28c>)
30009732:	f8ce 2000 	str.w	r2, [lr]
30009736:	6860      	ldr	r0, [r4, #4]
30009738:	4a58      	ldr	r2, [pc, #352]	; (3000989c <PSRAM_calibration+0x290>)
3000973a:	f8c9 0000 	str.w	r0, [r9]
3000973e:	68a0      	ldr	r0, [r4, #8]
30009740:	f8ca 0000 	str.w	r0, [sl]
30009744:	68e0      	ldr	r0, [r4, #12]
30009746:	6018      	str	r0, [r3, #0]
30009748:	6920      	ldr	r0, [r4, #16]
3000974a:	6008      	str	r0, [r1, #0]
3000974c:	6960      	ldr	r0, [r4, #20]
3000974e:	6010      	str	r0, [r2, #0]
30009750:	f8de 0000 	ldr.w	r0, [lr]
30009754:	900a      	str	r0, [sp, #40]	; 0x28
30009756:	f8d9 0000 	ldr.w	r0, [r9]
3000975a:	900b      	str	r0, [sp, #44]	; 0x2c
3000975c:	f8da 0000 	ldr.w	r0, [sl]
30009760:	900c      	str	r0, [sp, #48]	; 0x30
30009762:	6818      	ldr	r0, [r3, #0]
30009764:	4b4e      	ldr	r3, [pc, #312]	; (300098a0 <PSRAM_calibration+0x294>)
30009766:	900d      	str	r0, [sp, #52]	; 0x34
30009768:	a80a      	add	r0, sp, #40	; 0x28
3000976a:	6809      	ldr	r1, [r1, #0]
3000976c:	910e      	str	r1, [sp, #56]	; 0x38
3000976e:	6812      	ldr	r2, [r2, #0]
30009770:	4947      	ldr	r1, [pc, #284]	; (30009890 <PSRAM_calibration+0x284>)
30009772:	920f      	str	r2, [sp, #60]	; 0x3c
30009774:	2218      	movs	r2, #24
30009776:	4798      	blx	r3
30009778:	2800      	cmp	r0, #0
3000977a:	d0c0      	beq.n	300096fe <PSRAM_calibration+0xf2>
3000977c:	1c70      	adds	r0, r6, #1
3000977e:	d0cb      	beq.n	30009718 <PSRAM_calibration+0x10c>
30009780:	9b02      	ldr	r3, [sp, #8]
30009782:	42bb      	cmp	r3, r7
30009784:	da0a      	bge.n	3000979c <PSRAM_calibration+0x190>
30009786:	9b03      	ldr	r3, [sp, #12]
30009788:	f8cd b018 	str.w	fp, [sp, #24]
3000978c:	f04f 3bff 	mov.w	fp, #4294967295
30009790:	9702      	str	r7, [sp, #8]
30009792:	2700      	movs	r7, #0
30009794:	9605      	str	r6, [sp, #20]
30009796:	465e      	mov	r6, fp
30009798:	9304      	str	r3, [sp, #16]
3000979a:	e7bd      	b.n	30009718 <PSRAM_calibration+0x10c>
3000979c:	f04f 3bff 	mov.w	fp, #4294967295
300097a0:	2700      	movs	r7, #0
300097a2:	465e      	mov	r6, fp
300097a4:	e7b8      	b.n	30009718 <PSRAM_calibration+0x10c>
300097a6:	9b03      	ldr	r3, [sp, #12]
300097a8:	9304      	str	r3, [sp, #16]
300097aa:	9b03      	ldr	r3, [sp, #12]
300097ac:	005b      	lsls	r3, r3, #1
300097ae:	9303      	str	r3, [sp, #12]
300097b0:	9b07      	ldr	r3, [sp, #28]
300097b2:	3b01      	subs	r3, #1
300097b4:	9307      	str	r3, [sp, #28]
300097b6:	d195      	bne.n	300096e4 <PSRAM_calibration+0xd8>
300097b8:	4b3a      	ldr	r3, [pc, #232]	; (300098a4 <PSRAM_calibration+0x298>)
300097ba:	681b      	ldr	r3, [r3, #0]
300097bc:	0399      	lsls	r1, r3, #14
300097be:	d454      	bmi.n	3000986a <PSRAM_calibration+0x25e>
300097c0:	4832      	ldr	r0, [pc, #200]	; (3000988c <PSRAM_calibration+0x280>)
300097c2:	6943      	ldr	r3, [r0, #20]
300097c4:	f413 3380 	ands.w	r3, r3, #65536	; 0x10000
300097c8:	d124      	bne.n	30009814 <PSRAM_calibration+0x208>
300097ca:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300097ce:	f3bf 8f4f 	dsb	sy
300097d2:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300097d6:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
300097da:	f3c5 344e 	ubfx	r4, r5, #13, #15
300097de:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300097e2:	0164      	lsls	r4, r4, #5
300097e4:	ea04 010c 	and.w	r1, r4, ip
300097e8:	462b      	mov	r3, r5
300097ea:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300097ee:	3b01      	subs	r3, #1
300097f0:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300097f4:	1c5a      	adds	r2, r3, #1
300097f6:	d1f8      	bne.n	300097ea <PSRAM_calibration+0x1de>
300097f8:	3c20      	subs	r4, #32
300097fa:	f114 0f20 	cmn.w	r4, #32
300097fe:	d1f1      	bne.n	300097e4 <PSRAM_calibration+0x1d8>
30009800:	f3bf 8f4f 	dsb	sy
30009804:	6943      	ldr	r3, [r0, #20]
30009806:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
3000980a:	6143      	str	r3, [r0, #20]
3000980c:	f3bf 8f4f 	dsb	sy
30009810:	f3bf 8f6f 	isb	sy
30009814:	2f08      	cmp	r7, #8
30009816:	dd24      	ble.n	30009862 <PSRAM_calibration+0x256>
30009818:	ebbb 0306 	subs.w	r3, fp, r6
3000981c:	9804      	ldr	r0, [sp, #16]
3000981e:	bf48      	it	mi
30009820:	3301      	addmi	r3, #1
30009822:	eb1b 0606 	adds.w	r6, fp, r6
30009826:	ea4f 0363 	mov.w	r3, r3, asr #1
3000982a:	bf48      	it	mi
3000982c:	3601      	addmi	r6, #1
3000982e:	1e9a      	subs	r2, r3, #2
30009830:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009832:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009836:	0414      	lsls	r4, r2, #16
30009838:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000983c:	0212      	lsls	r2, r2, #8
3000983e:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009842:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009846:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000984a:	2001      	movs	r0, #1
3000984c:	430b      	orrs	r3, r1
3000984e:	490d      	ldr	r1, [pc, #52]	; (30009884 <PSRAM_calibration+0x278>)
30009850:	4323      	orrs	r3, r4
30009852:	4313      	orrs	r3, r2
30009854:	604b      	str	r3, [r1, #4]
30009856:	680b      	ldr	r3, [r1, #0]
30009858:	4303      	orrs	r3, r0
3000985a:	600b      	str	r3, [r1, #0]
3000985c:	b011      	add	sp, #68	; 0x44
3000985e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009862:	2000      	movs	r0, #0
30009864:	b011      	add	sp, #68	; 0x44
30009866:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000986a:	9b04      	ldr	r3, [sp, #16]
3000986c:	465a      	mov	r2, fp
3000986e:	4631      	mov	r1, r6
30009870:	480d      	ldr	r0, [pc, #52]	; (300098a8 <PSRAM_calibration+0x29c>)
30009872:	9300      	str	r3, [sp, #0]
30009874:	463b      	mov	r3, r7
30009876:	f002 fadb 	bl	3000be30 <__DiagPrintf_veneer>
3000987a:	e7a1      	b.n	300097c0 <PSRAM_calibration+0x1b4>
3000987c:	9f02      	ldr	r7, [sp, #8]
3000987e:	e9dd 6b05 	ldrd	r6, fp, [sp, #20]
30009882:	e792      	b.n	300097aa <PSRAM_calibration+0x19e>
30009884:	41002000 	.word	0x41002000
30009888:	0fffffe0 	.word	0x0fffffe0
3000988c:	e000ed00 	.word	0xe000ed00
30009890:	3000dfa8 	.word	0x3000dfa8
30009894:	60200000 	.word	0x60200000
30009898:	60150000 	.word	0x60150000
3000989c:	60250000 	.word	0x60250000
300098a0:	000129bd 	.word	0x000129bd
300098a4:	2001c00c 	.word	0x2001c00c
300098a8:	3000d040 	.word	0x3000d040
300098ac:	60050000 	.word	0x60050000
300098b0:	60100000 	.word	0x60100000

300098b4 <PSRAM_AutoGating>:
300098b4:	b1c0      	cbz	r0, 300098e8 <PSRAM_AutoGating+0x34>
300098b6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300098ba:	4810      	ldr	r0, [pc, #64]	; (300098fc <PSRAM_AutoGating+0x48>)
300098bc:	f3c1 0108 	ubfx	r1, r1, #0, #9
300098c0:	0252      	lsls	r2, r2, #9
300098c2:	b410      	push	{r4}
300098c4:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
300098c8:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300098cc:	4020      	ands	r0, r4
300098ce:	4301      	orrs	r1, r0
300098d0:	430a      	orrs	r2, r1
300098d2:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300098d6:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300098da:	f042 0201 	orr.w	r2, r2, #1
300098de:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300098e2:	f85d 4b04 	ldr.w	r4, [sp], #4
300098e6:	4770      	bx	lr
300098e8:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300098ec:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300098f0:	f023 0301 	bic.w	r3, r3, #1
300098f4:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300098f8:	4770      	bx	lr
300098fa:	bf00      	nop
300098fc:	ffffc000 	.word	0xffffc000

30009900 <BOOT_Reason>:
30009900:	4b02      	ldr	r3, [pc, #8]	; (3000990c <BOOT_Reason+0xc>)
30009902:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009906:	b280      	uxth	r0, r0
30009908:	4770      	bx	lr
3000990a:	bf00      	nop
3000990c:	42008200 	.word	0x42008200

30009910 <CRYPTO_SetSecurityModeAD>:
30009910:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009914:	460d      	mov	r5, r1
30009916:	4616      	mov	r6, r2
30009918:	4699      	mov	r9, r3
3000991a:	4604      	mov	r4, r0
3000991c:	e9dd 8709 	ldrd	r8, r7, [sp, #36]	; 0x24
30009920:	2800      	cmp	r0, #0
30009922:	f000 80a6 	beq.w	30009a72 <CRYPTO_SetSecurityModeAD+0x162>
30009926:	7823      	ldrb	r3, [r4, #0]
30009928:	2b01      	cmp	r3, #1
3000992a:	d003      	beq.n	30009934 <CRYPTO_SetSecurityModeAD+0x24>
3000992c:	2124      	movs	r1, #36	; 0x24
3000992e:	485d      	ldr	r0, [pc, #372]	; (30009aa4 <CRYPTO_SetSecurityModeAD+0x194>)
30009930:	f002 fa6e 	bl	3000be10 <__io_assert_failed_veneer>
30009934:	2300      	movs	r3, #0
30009936:	1c69      	adds	r1, r5, #1
30009938:	6065      	str	r5, [r4, #4]
3000993a:	8123      	strh	r3, [r4, #8]
3000993c:	d008      	beq.n	30009950 <CRYPTO_SetSecurityModeAD+0x40>
3000993e:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009942:	2b20      	cmp	r3, #32
30009944:	d078      	beq.n	30009a38 <CRYPTO_SetSecurityModeAD+0x128>
30009946:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000994a:	f3c5 15c0 	ubfx	r5, r5, #7, #1
3000994e:	7265      	strb	r5, [r4, #9]
30009950:	1c72      	adds	r2, r6, #1
30009952:	60e6      	str	r6, [r4, #12]
30009954:	d07f      	beq.n	30009a56 <CRYPTO_SetSecurityModeAD+0x146>
30009956:	f006 0303 	and.w	r3, r6, #3
3000995a:	f3c6 0280 	ubfx	r2, r6, #2, #1
3000995e:	7422      	strb	r2, [r4, #16]
30009960:	f1a3 0202 	sub.w	r2, r3, #2
30009964:	2b01      	cmp	r3, #1
30009966:	fab2 f282 	clz	r2, r2
3000996a:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000996e:	7462      	strb	r2, [r4, #17]
30009970:	fab3 f283 	clz	r2, r3
30009974:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009978:	74a2      	strb	r2, [r4, #18]
3000997a:	bf0c      	ite	eq
3000997c:	2201      	moveq	r2, #1
3000997e:	2200      	movne	r2, #0
30009980:	74e2      	strb	r2, [r4, #19]
30009982:	d13e      	bne.n	30009a02 <CRYPTO_SetSecurityModeAD+0xf2>
30009984:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009988:	2b40      	cmp	r3, #64	; 0x40
3000998a:	d07e      	beq.n	30009a8a <CRYPTO_SetSecurityModeAD+0x17a>
3000998c:	d869      	bhi.n	30009a62 <CRYPTO_SetSecurityModeAD+0x152>
3000998e:	2b10      	cmp	r3, #16
30009990:	d06c      	beq.n	30009a6c <CRYPTO_SetSecurityModeAD+0x15c>
30009992:	2b20      	cmp	r3, #32
30009994:	d172      	bne.n	30009a7c <CRYPTO_SetSecurityModeAD+0x16c>
30009996:	7523      	strb	r3, [r4, #20]
30009998:	62a3      	str	r3, [r4, #40]	; 0x28
3000999a:	9b08      	ldr	r3, [sp, #32]
3000999c:	6227      	str	r7, [r4, #32]
3000999e:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300099a2:	2f00      	cmp	r7, #0
300099a4:	d038      	beq.n	30009a18 <CRYPTO_SetSecurityModeAD+0x108>
300099a6:	7d23      	ldrb	r3, [r4, #20]
300099a8:	f104 0064 	add.w	r0, r4, #100	; 0x64
300099ac:	2b30      	cmp	r3, #48	; 0x30
300099ae:	65e0      	str	r0, [r4, #92]	; 0x5c
300099b0:	d045      	beq.n	30009a3e <CRYPTO_SetSecurityModeAD+0x12e>
300099b2:	2b40      	cmp	r3, #64	; 0x40
300099b4:	d043      	beq.n	30009a3e <CRYPTO_SetSecurityModeAD+0x12e>
300099b6:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
300099ba:	2240      	movs	r2, #64	; 0x40
300099bc:	2136      	movs	r1, #54	; 0x36
300099be:	4d3a      	ldr	r5, [pc, #232]	; (30009aa8 <CRYPTO_SetSecurityModeAD+0x198>)
300099c0:	6623      	str	r3, [r4, #96]	; 0x60
300099c2:	47a8      	blx	r5
300099c4:	2240      	movs	r2, #64	; 0x40
300099c6:	215c      	movs	r1, #92	; 0x5c
300099c8:	6e20      	ldr	r0, [r4, #96]	; 0x60
300099ca:	47a8      	blx	r5
300099cc:	4b37      	ldr	r3, [pc, #220]	; (30009aac <CRYPTO_SetSecurityModeAD+0x19c>)
300099ce:	681d      	ldr	r5, [r3, #0]
300099d0:	bb25      	cbnz	r5, 30009a1c <CRYPTO_SetSecurityModeAD+0x10c>
300099d2:	f1b8 0f00 	cmp.w	r8, #0
300099d6:	d060      	beq.n	30009a9a <CRYPTO_SetSecurityModeAD+0x18a>
300099d8:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
300099dc:	e001      	b.n	300099e2 <CRYPTO_SetSecurityModeAD+0xd2>
300099de:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
300099e2:	6de2      	ldr	r2, [r4, #92]	; 0x5c
300099e4:	f818 3005 	ldrb.w	r3, [r8, r5]
300099e8:	5d51      	ldrb	r1, [r2, r5]
300099ea:	404b      	eors	r3, r1
300099ec:	5553      	strb	r3, [r2, r5]
300099ee:	6e22      	ldr	r2, [r4, #96]	; 0x60
300099f0:	6a63      	ldr	r3, [r4, #36]	; 0x24
300099f2:	5d51      	ldrb	r1, [r2, r5]
300099f4:	5d5b      	ldrb	r3, [r3, r5]
300099f6:	404b      	eors	r3, r1
300099f8:	5553      	strb	r3, [r2, r5]
300099fa:	3501      	adds	r5, #1
300099fc:	42af      	cmp	r7, r5
300099fe:	d1ee      	bne.n	300099de <CRYPTO_SetSecurityModeAD+0xce>
30009a00:	e00c      	b.n	30009a1c <CRYPTO_SetSecurityModeAD+0x10c>
30009a02:	2200      	movs	r2, #0
30009a04:	7522      	strb	r2, [r4, #20]
30009a06:	b993      	cbnz	r3, 30009a2e <CRYPTO_SetSecurityModeAD+0x11e>
30009a08:	2314      	movs	r3, #20
30009a0a:	6227      	str	r7, [r4, #32]
30009a0c:	62a3      	str	r3, [r4, #40]	; 0x28
30009a0e:	9b08      	ldr	r3, [sp, #32]
30009a10:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009a14:	2f00      	cmp	r7, #0
30009a16:	d1c6      	bne.n	300099a6 <CRYPTO_SetSecurityModeAD+0x96>
30009a18:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009a1c:	3601      	adds	r6, #1
30009a1e:	d003      	beq.n	30009a28 <CRYPTO_SetSecurityModeAD+0x118>
30009a20:	2201      	movs	r2, #1
30009a22:	2300      	movs	r3, #0
30009a24:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009a26:	6323      	str	r3, [r4, #48]	; 0x30
30009a28:	2000      	movs	r0, #0
30009a2a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009a2e:	2b02      	cmp	r3, #2
30009a30:	d114      	bne.n	30009a5c <CRYPTO_SetSecurityModeAD+0x14c>
30009a32:	2310      	movs	r3, #16
30009a34:	62a3      	str	r3, [r4, #40]	; 0x28
30009a36:	e7b0      	b.n	3000999a <CRYPTO_SetSecurityModeAD+0x8a>
30009a38:	2301      	movs	r3, #1
30009a3a:	7223      	strb	r3, [r4, #8]
30009a3c:	e783      	b.n	30009946 <CRYPTO_SetSecurityModeAD+0x36>
30009a3e:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009a42:	2280      	movs	r2, #128	; 0x80
30009a44:	2136      	movs	r1, #54	; 0x36
30009a46:	4d18      	ldr	r5, [pc, #96]	; (30009aa8 <CRYPTO_SetSecurityModeAD+0x198>)
30009a48:	6623      	str	r3, [r4, #96]	; 0x60
30009a4a:	47a8      	blx	r5
30009a4c:	2280      	movs	r2, #128	; 0x80
30009a4e:	215c      	movs	r1, #92	; 0x5c
30009a50:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009a52:	47a8      	blx	r5
30009a54:	e7ba      	b.n	300099cc <CRYPTO_SetSecurityModeAD+0xbc>
30009a56:	2300      	movs	r3, #0
30009a58:	6123      	str	r3, [r4, #16]
30009a5a:	7523      	strb	r3, [r4, #20]
30009a5c:	2300      	movs	r3, #0
30009a5e:	62a3      	str	r3, [r4, #40]	; 0x28
30009a60:	e79b      	b.n	3000999a <CRYPTO_SetSecurityModeAD+0x8a>
30009a62:	2b80      	cmp	r3, #128	; 0x80
30009a64:	d10a      	bne.n	30009a7c <CRYPTO_SetSecurityModeAD+0x16c>
30009a66:	2340      	movs	r3, #64	; 0x40
30009a68:	7523      	strb	r3, [r4, #20]
30009a6a:	e795      	b.n	30009998 <CRYPTO_SetSecurityModeAD+0x88>
30009a6c:	231c      	movs	r3, #28
30009a6e:	7523      	strb	r3, [r4, #20]
30009a70:	e7f5      	b.n	30009a5e <CRYPTO_SetSecurityModeAD+0x14e>
30009a72:	2123      	movs	r1, #35	; 0x23
30009a74:	480b      	ldr	r0, [pc, #44]	; (30009aa4 <CRYPTO_SetSecurityModeAD+0x194>)
30009a76:	f002 f9cb 	bl	3000be10 <__io_assert_failed_veneer>
30009a7a:	e754      	b.n	30009926 <CRYPTO_SetSecurityModeAD+0x16>
30009a7c:	4b0c      	ldr	r3, [pc, #48]	; (30009ab0 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009a7e:	681b      	ldr	r3, [r3, #0]
30009a80:	049b      	lsls	r3, r3, #18
30009a82:	d405      	bmi.n	30009a90 <CRYPTO_SetSecurityModeAD+0x180>
30009a84:	2300      	movs	r3, #0
30009a86:	7523      	strb	r3, [r4, #20]
30009a88:	e7e9      	b.n	30009a5e <CRYPTO_SetSecurityModeAD+0x14e>
30009a8a:	2330      	movs	r3, #48	; 0x30
30009a8c:	7523      	strb	r3, [r4, #20]
30009a8e:	e7e6      	b.n	30009a5e <CRYPTO_SetSecurityModeAD+0x14e>
30009a90:	4631      	mov	r1, r6
30009a92:	4808      	ldr	r0, [pc, #32]	; (30009ab4 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009a94:	f002 f9cc 	bl	3000be30 <__DiagPrintf_veneer>
30009a98:	e7f4      	b.n	30009a84 <CRYPTO_SetSecurityModeAD+0x174>
30009a9a:	217f      	movs	r1, #127	; 0x7f
30009a9c:	4801      	ldr	r0, [pc, #4]	; (30009aa4 <CRYPTO_SetSecurityModeAD+0x194>)
30009a9e:	f002 f9b7 	bl	3000be10 <__io_assert_failed_veneer>
30009aa2:	e799      	b.n	300099d8 <CRYPTO_SetSecurityModeAD+0xc8>
30009aa4:	3000d118 	.word	0x3000d118
30009aa8:	00012be5 	.word	0x00012be5
30009aac:	2001c200 	.word	0x2001c200
30009ab0:	2001c00c 	.word	0x2001c00c
30009ab4:	3000d078 	.word	0x3000d078

30009ab8 <rtl_crypto_hmac_sha2_init>:
30009ab8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009abc:	4c97      	ldr	r4, [pc, #604]	; (30009d1c <rtl_crypto_hmac_sha2_init+0x264>)
30009abe:	4617      	mov	r7, r2
30009ac0:	e844 f400 	tt	r4, r4
30009ac4:	4a96      	ldr	r2, [pc, #600]	; (30009d20 <rtl_crypto_hmac_sha2_init+0x268>)
30009ac6:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30009aca:	461c      	mov	r4, r3
30009acc:	4b95      	ldr	r3, [pc, #596]	; (30009d24 <rtl_crypto_hmac_sha2_init+0x26c>)
30009ace:	bf18      	it	ne
30009ad0:	4613      	movne	r3, r2
30009ad2:	b085      	sub	sp, #20
30009ad4:	4605      	mov	r5, r0
30009ad6:	460e      	mov	r6, r1
30009ad8:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30009ada:	2b00      	cmp	r3, #0
30009adc:	db05      	blt.n	30009aea <rtl_crypto_hmac_sha2_init+0x32>
30009ade:	2900      	cmp	r1, #0
30009ae0:	f000 8118 	beq.w	30009d14 <rtl_crypto_hmac_sha2_init+0x25c>
30009ae4:	078b      	lsls	r3, r1, #30
30009ae6:	f040 8112 	bne.w	30009d0e <rtl_crypto_hmac_sha2_init+0x256>
30009aea:	2d20      	cmp	r5, #32
30009aec:	bf18      	it	ne
30009aee:	2d1c      	cmpne	r5, #28
30009af0:	bf0c      	ite	eq
30009af2:	f04f 0901 	moveq.w	r9, #1
30009af6:	f04f 0900 	movne.w	r9, #0
30009afa:	d04d      	beq.n	30009b98 <rtl_crypto_hmac_sha2_init+0xe0>
30009afc:	2d40      	cmp	r5, #64	; 0x40
30009afe:	bf18      	it	ne
30009b00:	2d30      	cmpne	r5, #48	; 0x30
30009b02:	d102      	bne.n	30009b0a <rtl_crypto_hmac_sha2_init+0x52>
30009b04:	2f80      	cmp	r7, #128	; 0x80
30009b06:	f200 80f9 	bhi.w	30009cfc <rtl_crypto_hmac_sha2_init+0x244>
30009b0a:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009d44 <rtl_crypto_hmac_sha2_init+0x28c>
30009b0e:	f898 3000 	ldrb.w	r3, [r8]
30009b12:	2b01      	cmp	r3, #1
30009b14:	d003      	beq.n	30009b1e <rtl_crypto_hmac_sha2_init+0x66>
30009b16:	21b3      	movs	r1, #179	; 0xb3
30009b18:	4883      	ldr	r0, [pc, #524]	; (30009d28 <rtl_crypto_hmac_sha2_init+0x270>)
30009b1a:	f002 f979 	bl	3000be10 <__io_assert_failed_veneer>
30009b1e:	2d30      	cmp	r5, #48	; 0x30
30009b20:	f000 809b 	beq.w	30009c5a <rtl_crypto_hmac_sha2_init+0x1a2>
30009b24:	d83f      	bhi.n	30009ba6 <rtl_crypto_hmac_sha2_init+0xee>
30009b26:	2d1c      	cmp	r5, #28
30009b28:	f000 80cb 	beq.w	30009cc2 <rtl_crypto_hmac_sha2_init+0x20a>
30009b2c:	f1b9 0f00 	cmp.w	r9, #0
30009b30:	d078      	beq.n	30009c24 <rtl_crypto_hmac_sha2_init+0x16c>
30009b32:	2c00      	cmp	r4, #0
30009b34:	f000 808f 	beq.w	30009c56 <rtl_crypto_hmac_sha2_init+0x19e>
30009b38:	4b7c      	ldr	r3, [pc, #496]	; (30009d2c <rtl_crypto_hmac_sha2_init+0x274>)
30009b3a:	f8df e210 	ldr.w	lr, [pc, #528]	; 30009d4c <rtl_crypto_hmac_sha2_init+0x294>
30009b3e:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30009d5c <rtl_crypto_hmac_sha2_init+0x2a4>
30009b42:	4a7b      	ldr	r2, [pc, #492]	; (30009d30 <rtl_crypto_hmac_sha2_init+0x278>)
30009b44:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009b48:	4b7a      	ldr	r3, [pc, #488]	; (30009d34 <rtl_crypto_hmac_sha2_init+0x27c>)
30009b4a:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009b4e:	4b7a      	ldr	r3, [pc, #488]	; (30009d38 <rtl_crypto_hmac_sha2_init+0x280>)
30009b50:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009b54:	4b79      	ldr	r3, [pc, #484]	; (30009d3c <rtl_crypto_hmac_sha2_init+0x284>)
30009b56:	2225      	movs	r2, #37	; 0x25
30009b58:	6223      	str	r3, [r4, #32]
30009b5a:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009b5e:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009b62:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009b66:	6263      	str	r3, [r4, #36]	; 0x24
30009b68:	2300      	movs	r3, #0
30009b6a:	f04f 31ff 	mov.w	r1, #4294967295
30009b6e:	4d74      	ldr	r5, [pc, #464]	; (30009d40 <rtl_crypto_hmac_sha2_init+0x288>)
30009b70:	9300      	str	r3, [sp, #0]
30009b72:	9702      	str	r7, [sp, #8]
30009b74:	9601      	str	r6, [sp, #4]
30009b76:	4873      	ldr	r0, [pc, #460]	; (30009d44 <rtl_crypto_hmac_sha2_init+0x28c>)
30009b78:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009b7c:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009b80:	47a8      	blx	r5
30009b82:	4605      	mov	r5, r0
30009b84:	4b70      	ldr	r3, [pc, #448]	; (30009d48 <rtl_crypto_hmac_sha2_init+0x290>)
30009b86:	4620      	mov	r0, r4
30009b88:	4798      	blx	r3
30009b8a:	2301      	movs	r3, #1
30009b8c:	4628      	mov	r0, r5
30009b8e:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009b92:	b005      	add	sp, #20
30009b94:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009b98:	2f40      	cmp	r7, #64	; 0x40
30009b9a:	d9b6      	bls.n	30009b0a <rtl_crypto_hmac_sha2_init+0x52>
30009b9c:	21ad      	movs	r1, #173	; 0xad
30009b9e:	4862      	ldr	r0, [pc, #392]	; (30009d28 <rtl_crypto_hmac_sha2_init+0x270>)
30009ba0:	f002 f936 	bl	3000be10 <__io_assert_failed_veneer>
30009ba4:	e7b1      	b.n	30009b0a <rtl_crypto_hmac_sha2_init+0x52>
30009ba6:	2d40      	cmp	r5, #64	; 0x40
30009ba8:	d13c      	bne.n	30009c24 <rtl_crypto_hmac_sha2_init+0x16c>
30009baa:	2c00      	cmp	r4, #0
30009bac:	d040      	beq.n	30009c30 <rtl_crypto_hmac_sha2_init+0x178>
30009bae:	4b67      	ldr	r3, [pc, #412]	; (30009d4c <rtl_crypto_hmac_sha2_init+0x294>)
30009bb0:	4a67      	ldr	r2, [pc, #412]	; (30009d50 <rtl_crypto_hmac_sha2_init+0x298>)
30009bb2:	4968      	ldr	r1, [pc, #416]	; (30009d54 <rtl_crypto_hmac_sha2_init+0x29c>)
30009bb4:	4d68      	ldr	r5, [pc, #416]	; (30009d58 <rtl_crypto_hmac_sha2_init+0x2a0>)
30009bb6:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009d9c <rtl_crypto_hmac_sha2_init+0x2e4>
30009bba:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009bbe:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009bc2:	2285      	movs	r2, #133	; 0x85
30009bc4:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30009bc8:	33d4      	adds	r3, #212	; 0xd4
30009bca:	6163      	str	r3, [r4, #20]
30009bcc:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009bd0:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009bd4:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30009bd8:	6123      	str	r3, [r4, #16]
30009bda:	4b60      	ldr	r3, [pc, #384]	; (30009d5c <rtl_crypto_hmac_sha2_init+0x2a4>)
30009bdc:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009be0:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009be4:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30009be8:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009bec:	6263      	str	r3, [r4, #36]	; 0x24
30009bee:	4b51      	ldr	r3, [pc, #324]	; (30009d34 <rtl_crypto_hmac_sha2_init+0x27c>)
30009bf0:	6223      	str	r3, [r4, #32]
30009bf2:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009bf6:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30009bfa:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009bfe:	62e3      	str	r3, [r4, #44]	; 0x2c
30009c00:	4b4b      	ldr	r3, [pc, #300]	; (30009d30 <rtl_crypto_hmac_sha2_init+0x278>)
30009c02:	62a3      	str	r3, [r4, #40]	; 0x28
30009c04:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30009c08:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009c0c:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009c10:	6363      	str	r3, [r4, #52]	; 0x34
30009c12:	4b49      	ldr	r3, [pc, #292]	; (30009d38 <rtl_crypto_hmac_sha2_init+0x280>)
30009c14:	6323      	str	r3, [r4, #48]	; 0x30
30009c16:	4b49      	ldr	r3, [pc, #292]	; (30009d3c <rtl_crypto_hmac_sha2_init+0x284>)
30009c18:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009c1c:	4b50      	ldr	r3, [pc, #320]	; (30009d60 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009c1e:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009c22:	e7a1      	b.n	30009b68 <rtl_crypto_hmac_sha2_init+0xb0>
30009c24:	f04f 35ff 	mov.w	r5, #4294967295
30009c28:	4628      	mov	r0, r5
30009c2a:	b005      	add	sp, #20
30009c2c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009c30:	2285      	movs	r2, #133	; 0x85
30009c32:	2300      	movs	r3, #0
30009c34:	f04f 31ff 	mov.w	r1, #4294967295
30009c38:	4c41      	ldr	r4, [pc, #260]	; (30009d40 <rtl_crypto_hmac_sha2_init+0x288>)
30009c3a:	9702      	str	r7, [sp, #8]
30009c3c:	4841      	ldr	r0, [pc, #260]	; (30009d44 <rtl_crypto_hmac_sha2_init+0x28c>)
30009c3e:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009c42:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009c46:	e9cd 3600 	strd	r3, r6, [sp]
30009c4a:	47a0      	blx	r4
30009c4c:	4605      	mov	r5, r0
30009c4e:	4628      	mov	r0, r5
30009c50:	b005      	add	sp, #20
30009c52:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009c56:	2225      	movs	r2, #37	; 0x25
30009c58:	e7eb      	b.n	30009c32 <rtl_crypto_hmac_sha2_init+0x17a>
30009c5a:	2c00      	cmp	r4, #0
30009c5c:	d053      	beq.n	30009d06 <rtl_crypto_hmac_sha2_init+0x24e>
30009c5e:	4b41      	ldr	r3, [pc, #260]	; (30009d64 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009c60:	f8df e130 	ldr.w	lr, [pc, #304]	; 30009d94 <rtl_crypto_hmac_sha2_init+0x2dc>
30009c64:	4a40      	ldr	r2, [pc, #256]	; (30009d68 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009c66:	4941      	ldr	r1, [pc, #260]	; (30009d6c <rtl_crypto_hmac_sha2_init+0x2b4>)
30009c68:	4d41      	ldr	r5, [pc, #260]	; (30009d70 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009c6a:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30009d98 <rtl_crypto_hmac_sha2_init+0x2e0>
30009c6e:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009c72:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30009c76:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30009c7a:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30009c7e:	6163      	str	r3, [r4, #20]
30009c80:	4b3c      	ldr	r3, [pc, #240]	; (30009d74 <rtl_crypto_hmac_sha2_init+0x2bc>)
30009c82:	6123      	str	r3, [r4, #16]
30009c84:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30009c88:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30009c8c:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30009c90:	61e3      	str	r3, [r4, #28]
30009c92:	4b39      	ldr	r3, [pc, #228]	; (30009d78 <rtl_crypto_hmac_sha2_init+0x2c0>)
30009c94:	61a3      	str	r3, [r4, #24]
30009c96:	4b39      	ldr	r3, [pc, #228]	; (30009d7c <rtl_crypto_hmac_sha2_init+0x2c4>)
30009c98:	e9c4 3208 	strd	r3, r2, [r4, #32]
30009c9c:	4b38      	ldr	r3, [pc, #224]	; (30009d80 <rtl_crypto_hmac_sha2_init+0x2c8>)
30009c9e:	2245      	movs	r2, #69	; 0x45
30009ca0:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30009ca4:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30009ca8:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30009cac:	33aa      	adds	r3, #170	; 0xaa
30009cae:	6363      	str	r3, [r4, #52]	; 0x34
30009cb0:	4b34      	ldr	r3, [pc, #208]	; (30009d84 <rtl_crypto_hmac_sha2_init+0x2cc>)
30009cb2:	6323      	str	r3, [r4, #48]	; 0x30
30009cb4:	4b34      	ldr	r3, [pc, #208]	; (30009d88 <rtl_crypto_hmac_sha2_init+0x2d0>)
30009cb6:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009cba:	4b34      	ldr	r3, [pc, #208]	; (30009d8c <rtl_crypto_hmac_sha2_init+0x2d4>)
30009cbc:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009cc0:	e752      	b.n	30009b68 <rtl_crypto_hmac_sha2_init+0xb0>
30009cc2:	b314      	cbz	r4, 30009d0a <rtl_crypto_hmac_sha2_init+0x252>
30009cc4:	4b32      	ldr	r3, [pc, #200]	; (30009d90 <rtl_crypto_hmac_sha2_init+0x2d8>)
30009cc6:	2215      	movs	r2, #21
30009cc8:	4932      	ldr	r1, [pc, #200]	; (30009d94 <rtl_crypto_hmac_sha2_init+0x2dc>)
30009cca:	4827      	ldr	r0, [pc, #156]	; (30009d68 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009ccc:	4d28      	ldr	r5, [pc, #160]	; (30009d70 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009cce:	e9c4 1302 	strd	r1, r3, [r4, #8]
30009cd2:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
30009cd6:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
30009cda:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30009cde:	6123      	str	r3, [r4, #16]
30009ce0:	4b22      	ldr	r3, [pc, #136]	; (30009d6c <rtl_crypto_hmac_sha2_init+0x2b4>)
30009ce2:	e9c4 0305 	strd	r0, r3, [r4, #20]
30009ce6:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
30009cea:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30009cee:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009cf2:	61e3      	str	r3, [r4, #28]
30009cf4:	4b28      	ldr	r3, [pc, #160]	; (30009d98 <rtl_crypto_hmac_sha2_init+0x2e0>)
30009cf6:	e9c4 5308 	strd	r5, r3, [r4, #32]
30009cfa:	e735      	b.n	30009b68 <rtl_crypto_hmac_sha2_init+0xb0>
30009cfc:	21b0      	movs	r1, #176	; 0xb0
30009cfe:	480a      	ldr	r0, [pc, #40]	; (30009d28 <rtl_crypto_hmac_sha2_init+0x270>)
30009d00:	f002 f886 	bl	3000be10 <__io_assert_failed_veneer>
30009d04:	e701      	b.n	30009b0a <rtl_crypto_hmac_sha2_init+0x52>
30009d06:	2245      	movs	r2, #69	; 0x45
30009d08:	e793      	b.n	30009c32 <rtl_crypto_hmac_sha2_init+0x17a>
30009d0a:	2215      	movs	r2, #21
30009d0c:	e791      	b.n	30009c32 <rtl_crypto_hmac_sha2_init+0x17a>
30009d0e:	f06f 0505 	mvn.w	r5, #5
30009d12:	e789      	b.n	30009c28 <rtl_crypto_hmac_sha2_init+0x170>
30009d14:	f06f 0503 	mvn.w	r5, #3
30009d18:	e786      	b.n	30009c28 <rtl_crypto_hmac_sha2_init+0x170>
30009d1a:	bf00      	nop
30009d1c:	0000e5f9 	.word	0x0000e5f9
30009d20:	500c8000 	.word	0x500c8000
30009d24:	400c8000 	.word	0x400c8000
30009d28:	3000d0fc 	.word	0x3000d0fc
30009d2c:	bb67ae85 	.word	0xbb67ae85
30009d30:	510e527f 	.word	0x510e527f
30009d34:	a54ff53a 	.word	0xa54ff53a
30009d38:	9b05688c 	.word	0x9b05688c
30009d3c:	1f83d9ab 	.word	0x1f83d9ab
30009d40:	30009911 	.word	0x30009911
30009d44:	2001c460 	.word	0x2001c460
30009d48:	00004c99 	.word	0x00004c99
30009d4c:	6a09e667 	.word	0x6a09e667
30009d50:	f3bcc908 	.word	0xf3bcc908
30009d54:	fe94f82b 	.word	0xfe94f82b
30009d58:	fb41bd6b 	.word	0xfb41bd6b
30009d5c:	3c6ef372 	.word	0x3c6ef372
30009d60:	5be0cd19 	.word	0x5be0cd19
30009d64:	cbbb9d5d 	.word	0xcbbb9d5d
30009d68:	f70e5939 	.word	0xf70e5939
30009d6c:	ffc00b31 	.word	0xffc00b31
30009d70:	64f98fa7 	.word	0x64f98fa7
30009d74:	629a292a 	.word	0x629a292a
30009d78:	9159015a 	.word	0x9159015a
30009d7c:	152fecd8 	.word	0x152fecd8
30009d80:	67332667 	.word	0x67332667
30009d84:	8eb44a87 	.word	0x8eb44a87
30009d88:	db0c2e0d 	.word	0xdb0c2e0d
30009d8c:	47b5481d 	.word	0x47b5481d
30009d90:	367cd507 	.word	0x367cd507
30009d94:	c1059ed8 	.word	0xc1059ed8
30009d98:	befa4fa4 	.word	0xbefa4fa4
30009d9c:	137e2179 	.word	0x137e2179

30009da0 <CPU_ClkGet>:
30009da0:	b570      	push	{r4, r5, r6, lr}
30009da2:	4d1e      	ldr	r5, [pc, #120]	; (30009e1c <CPU_ClkGet+0x7c>)
30009da4:	4b1e      	ldr	r3, [pc, #120]	; (30009e20 <CPU_ClkGet+0x80>)
30009da6:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30009daa:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009dae:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009db2:	4798      	blx	r3
30009db4:	b92c      	cbnz	r4, 30009dc2 <CPU_ClkGet+0x22>
30009db6:	2801      	cmp	r0, #1
30009db8:	d007      	beq.n	30009dca <CPU_ClkGet+0x2a>
30009dba:	07b2      	lsls	r2, r6, #30
30009dbc:	d407      	bmi.n	30009dce <CPU_ClkGet+0x2e>
30009dbe:	4819      	ldr	r0, [pc, #100]	; (30009e24 <CPU_ClkGet+0x84>)
30009dc0:	bd70      	pop	{r4, r5, r6, pc}
30009dc2:	2c01      	cmp	r4, #1
30009dc4:	d007      	beq.n	30009dd6 <CPU_ClkGet+0x36>
30009dc6:	2000      	movs	r0, #0
30009dc8:	bd70      	pop	{r4, r5, r6, pc}
30009dca:	4817      	ldr	r0, [pc, #92]	; (30009e28 <CPU_ClkGet+0x88>)
30009dcc:	bd70      	pop	{r4, r5, r6, pc}
30009dce:	4b17      	ldr	r3, [pc, #92]	; (30009e2c <CPU_ClkGet+0x8c>)
30009dd0:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009dd4:	4718      	bx	r3
30009dd6:	2801      	cmp	r0, #1
30009dd8:	d011      	beq.n	30009dfe <CPU_ClkGet+0x5e>
30009dda:	05f3      	lsls	r3, r6, #23
30009ddc:	d51c      	bpl.n	30009e18 <CPU_ClkGet+0x78>
30009dde:	4914      	ldr	r1, [pc, #80]	; (30009e30 <CPU_ClkGet+0x90>)
30009de0:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30009de4:	6c48      	ldr	r0, [r1, #68]	; 0x44
30009de6:	4a13      	ldr	r2, [pc, #76]	; (30009e34 <CPU_ClkGet+0x94>)
30009de8:	f003 0307 	and.w	r3, r3, #7
30009dec:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009df0:	3301      	adds	r3, #1
30009df2:	3002      	adds	r0, #2
30009df4:	fb02 f000 	mul.w	r0, r2, r0
30009df8:	fbb0 f0f3 	udiv	r0, r0, r3
30009dfc:	bd70      	pop	{r4, r5, r6, pc}
30009dfe:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009e02:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30009e06:	f3c3 4007 	ubfx	r0, r3, #16, #8
30009e0a:	d003      	beq.n	30009e14 <CPU_ClkGet+0x74>
30009e0c:	4b0a      	ldr	r3, [pc, #40]	; (30009e38 <CPU_ClkGet+0x98>)
30009e0e:	fb03 f000 	mul.w	r0, r3, r0
30009e12:	bd70      	pop	{r4, r5, r6, pc}
30009e14:	4809      	ldr	r0, [pc, #36]	; (30009e3c <CPU_ClkGet+0x9c>)
30009e16:	bd70      	pop	{r4, r5, r6, pc}
30009e18:	4806      	ldr	r0, [pc, #24]	; (30009e34 <CPU_ClkGet+0x94>)
30009e1a:	bd70      	pop	{r4, r5, r6, pc}
30009e1c:	42008000 	.word	0x42008000
30009e20:	0000c0f9 	.word	0x0000c0f9
30009e24:	003d0900 	.word	0x003d0900
30009e28:	01312d00 	.word	0x01312d00
30009e2c:	000099f5 	.word	0x000099f5
30009e30:	42008800 	.word	0x42008800
30009e34:	02625a00 	.word	0x02625a00
30009e38:	000f4240 	.word	0x000f4240
30009e3c:	02faf080 	.word	0x02faf080

30009e40 <RSIP_IV_Set>:
30009e40:	4b05      	ldr	r3, [pc, #20]	; (30009e58 <RSIP_IV_Set+0x18>)
30009e42:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30009e46:	680b      	ldr	r3, [r1, #0]
30009e48:	ba1b      	rev	r3, r3
30009e4a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009e4e:	684b      	ldr	r3, [r1, #4]
30009e50:	ba1b      	rev	r3, r3
30009e52:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009e56:	4770      	bx	lr
30009e58:	42008c00 	.word	0x42008c00

30009e5c <RSIP_OTF_Cmd>:
30009e5c:	4a04      	ldr	r2, [pc, #16]	; (30009e70 <RSIP_OTF_Cmd+0x14>)
30009e5e:	2801      	cmp	r0, #1
30009e60:	6953      	ldr	r3, [r2, #20]
30009e62:	bf0c      	ite	eq
30009e64:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009e68:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009e6c:	6153      	str	r3, [r2, #20]
30009e6e:	4770      	bx	lr
30009e70:	42008c00 	.word	0x42008c00

30009e74 <RSIP_OTF_Enable>:
30009e74:	b570      	push	{r4, r5, r6, lr}
30009e76:	4604      	mov	r4, r0
30009e78:	b1f3      	cbz	r3, 30009eb8 <RSIP_OTF_Enable+0x44>
30009e7a:	9b04      	ldr	r3, [sp, #16]
30009e7c:	460d      	mov	r5, r1
30009e7e:	4616      	mov	r6, r2
30009e80:	2b07      	cmp	r3, #7
30009e82:	d827      	bhi.n	30009ed4 <RSIP_OTF_Enable+0x60>
30009e84:	9b05      	ldr	r3, [sp, #20]
30009e86:	3b01      	subs	r3, #1
30009e88:	2b01      	cmp	r3, #1
30009e8a:	d81d      	bhi.n	30009ec8 <RSIP_OTF_Enable+0x54>
30009e8c:	9a04      	ldr	r2, [sp, #16]
30009e8e:	9b05      	ldr	r3, [sp, #20]
30009e90:	f002 0007 	and.w	r0, r2, #7
30009e94:	005b      	lsls	r3, r3, #1
30009e96:	4305      	orrs	r5, r0
30009e98:	4813      	ldr	r0, [pc, #76]	; (30009ee8 <RSIP_OTF_Enable+0x74>)
30009e9a:	f003 0306 	and.w	r3, r3, #6
30009e9e:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009ea2:	ea43 0206 	orr.w	r2, r3, r6
30009ea6:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009ea8:	f042 0201 	orr.w	r2, r2, #1
30009eac:	f023 0301 	bic.w	r3, r3, #1
30009eb0:	6463      	str	r3, [r4, #68]	; 0x44
30009eb2:	6425      	str	r5, [r4, #64]	; 0x40
30009eb4:	6462      	str	r2, [r4, #68]	; 0x44
30009eb6:	bd70      	pop	{r4, r5, r6, pc}
30009eb8:	480b      	ldr	r0, [pc, #44]	; (30009ee8 <RSIP_OTF_Enable+0x74>)
30009eba:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009ebe:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009ec0:	f023 0301 	bic.w	r3, r3, #1
30009ec4:	6463      	str	r3, [r4, #68]	; 0x44
30009ec6:	bd70      	pop	{r4, r5, r6, pc}
30009ec8:	f44f 71f3 	mov.w	r1, #486	; 0x1e6
30009ecc:	4807      	ldr	r0, [pc, #28]	; (30009eec <RSIP_OTF_Enable+0x78>)
30009ece:	f001 ff9f 	bl	3000be10 <__io_assert_failed_veneer>
30009ed2:	e7db      	b.n	30009e8c <RSIP_OTF_Enable+0x18>
30009ed4:	f240 11e5 	movw	r1, #485	; 0x1e5
30009ed8:	4804      	ldr	r0, [pc, #16]	; (30009eec <RSIP_OTF_Enable+0x78>)
30009eda:	f001 ff99 	bl	3000be10 <__io_assert_failed_veneer>
30009ede:	9b05      	ldr	r3, [sp, #20]
30009ee0:	3b01      	subs	r3, #1
30009ee2:	2b01      	cmp	r3, #1
30009ee4:	d9d2      	bls.n	30009e8c <RSIP_OTF_Enable+0x18>
30009ee6:	e7ef      	b.n	30009ec8 <RSIP_OTF_Enable+0x54>
30009ee8:	42008c00 	.word	0x42008c00
30009eec:	3000d0ec 	.word	0x3000d0ec

30009ef0 <RSIP_MMU_Config>:
30009ef0:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009ef4:	311f      	adds	r1, #31
30009ef6:	321f      	adds	r2, #31
30009ef8:	331f      	adds	r3, #31
30009efa:	0080      	lsls	r0, r0, #2
30009efc:	0949      	lsrs	r1, r1, #5
30009efe:	0952      	lsrs	r2, r2, #5
30009f00:	095b      	lsrs	r3, r3, #5
30009f02:	0249      	lsls	r1, r1, #9
30009f04:	0252      	lsls	r2, r2, #9
30009f06:	025b      	lsls	r3, r3, #9
30009f08:	b410      	push	{r4}
30009f0a:	4c08      	ldr	r4, [pc, #32]	; (30009f2c <RSIP_MMU_Config+0x3c>)
30009f0c:	4404      	add	r4, r0
30009f0e:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009f12:	f020 0001 	bic.w	r0, r0, #1
30009f16:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30009f1a:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30009f1e:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009f22:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009f26:	f85d 4b04 	ldr.w	r4, [sp], #4
30009f2a:	4770      	bx	lr
30009f2c:	42008c00 	.word	0x42008c00

30009f30 <RSIP_MMU_Cmd>:
30009f30:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009f34:	4b07      	ldr	r3, [pc, #28]	; (30009f54 <RSIP_MMU_Cmd+0x24>)
30009f36:	0080      	lsls	r0, r0, #2
30009f38:	4403      	add	r3, r0
30009f3a:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30009f3e:	b121      	cbz	r1, 30009f4a <RSIP_MMU_Cmd+0x1a>
30009f40:	f042 0201 	orr.w	r2, r2, #1
30009f44:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009f48:	4770      	bx	lr
30009f4a:	f022 0201 	bic.w	r2, r2, #1
30009f4e:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009f52:	4770      	bx	lr
30009f54:	42008c00 	.word	0x42008c00

30009f58 <RSIP_MMU_Cache_Clean>:
30009f58:	4a02      	ldr	r2, [pc, #8]	; (30009f64 <RSIP_MMU_Cache_Clean+0xc>)
30009f5a:	6993      	ldr	r3, [r2, #24]
30009f5c:	f043 0304 	orr.w	r3, r3, #4
30009f60:	6193      	str	r3, [r2, #24]
30009f62:	4770      	bx	lr
30009f64:	42008c00 	.word	0x42008c00

30009f68 <NAND_RxData>:
30009f68:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009f6c:	460c      	mov	r4, r1
30009f6e:	4616      	mov	r6, r2
30009f70:	461f      	mov	r7, r3
30009f72:	2800      	cmp	r0, #0
30009f74:	f000 808a 	beq.w	3000a08c <NAND_RxData+0x124>
30009f78:	4605      	mov	r5, r0
30009f7a:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000a098 <NAND_RxData+0x130>
30009f7e:	2000      	movs	r0, #0
30009f80:	f8df 8118 	ldr.w	r8, [pc, #280]	; 3000a09c <NAND_RxData+0x134>
30009f84:	47c0      	blx	r8
30009f86:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30009f8a:	2000      	movs	r0, #0
30009f8c:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
30009f90:	6819      	ldr	r1, [r3, #0]
30009f92:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30009f96:	6019      	str	r1, [r3, #0]
30009f98:	6819      	ldr	r1, [r3, #0]
30009f9a:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30009f9e:	6019      	str	r1, [r3, #0]
30009fa0:	605a      	str	r2, [r3, #4]
30009fa2:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009fa6:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009faa:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30009fae:	ea4f 420c 	mov.w	r2, ip, lsl #16
30009fb2:	f1bc 0f03 	cmp.w	ip, #3
30009fb6:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009fba:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009fbe:	ea42 0201 	orr.w	r2, r2, r1
30009fc2:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009fc6:	f04f 0201 	mov.w	r2, #1
30009fca:	611a      	str	r2, [r3, #16]
30009fcc:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30009fd0:	bf08      	it	eq
30009fd2:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009fd6:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009fda:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30009fde:	b2e4      	uxtb	r4, r4
30009fe0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009fe4:	2301      	movs	r3, #1
30009fe6:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009fea:	f017 0403 	ands.w	r4, r7, #3
30009fee:	6083      	str	r3, [r0, #8]
30009ff0:	d137      	bne.n	3000a062 <NAND_RxData+0xfa>
30009ff2:	463b      	mov	r3, r7
30009ff4:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009ff8:	42b4      	cmp	r4, r6
30009ffa:	d216      	bcs.n	3000a02a <NAND_RxData+0xc2>
30009ffc:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000a000:	f012 0f3c 	tst.w	r2, #60	; 0x3c
3000a004:	f3c2 0583 	ubfx	r5, r2, #2, #4
3000a008:	d00c      	beq.n	3000a024 <NAND_RxData+0xbc>
3000a00a:	00ad      	lsls	r5, r5, #2
3000a00c:	1f19      	subs	r1, r3, #4
3000a00e:	f1a5 0c04 	sub.w	ip, r5, #4
3000a012:	449c      	add	ip, r3
3000a014:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
3000a018:	f841 0f04 	str.w	r0, [r1, #4]!
3000a01c:	458c      	cmp	ip, r1
3000a01e:	d1f9      	bne.n	3000a014 <NAND_RxData+0xac>
3000a020:	442b      	add	r3, r5
3000a022:	442c      	add	r4, r5
3000a024:	1b32      	subs	r2, r6, r4
3000a026:	2a03      	cmp	r2, #3
3000a028:	d8e6      	bhi.n	30009ff8 <NAND_RxData+0x90>
3000a02a:	1e63      	subs	r3, r4, #1
3000a02c:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a030:	441f      	add	r7, r3
3000a032:	e002      	b.n	3000a03a <NAND_RxData+0xd2>
3000a034:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a036:	071b      	lsls	r3, r3, #28
3000a038:	d40d      	bmi.n	3000a056 <NAND_RxData+0xee>
3000a03a:	42b4      	cmp	r4, r6
3000a03c:	d3fa      	bcc.n	3000a034 <NAND_RxData+0xcc>
3000a03e:	2004      	movs	r0, #4
3000a040:	47c0      	blx	r8
3000a042:	2000      	movs	r0, #0
3000a044:	47c0      	blx	r8
3000a046:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a04a:	6813      	ldr	r3, [r2, #0]
3000a04c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000a050:	6013      	str	r3, [r2, #0]
3000a052:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
3000a056:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
3000a05a:	3401      	adds	r4, #1
3000a05c:	f807 3f01 	strb.w	r3, [r7, #1]!
3000a060:	e7eb      	b.n	3000a03a <NAND_RxData+0xd2>
3000a062:	f1c4 0404 	rsb	r4, r4, #4
3000a066:	2200      	movs	r2, #0
3000a068:	4639      	mov	r1, r7
3000a06a:	e00a      	b.n	3000a082 <NAND_RxData+0x11a>
3000a06c:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000a06e:	072d      	lsls	r5, r5, #28
3000a070:	d505      	bpl.n	3000a07e <NAND_RxData+0x116>
3000a072:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
3000a076:	3201      	adds	r2, #1
3000a078:	700b      	strb	r3, [r1, #0]
3000a07a:	18b9      	adds	r1, r7, r2
3000a07c:	460b      	mov	r3, r1
3000a07e:	4294      	cmp	r4, r2
3000a080:	d0b8      	beq.n	30009ff4 <NAND_RxData+0x8c>
3000a082:	42b2      	cmp	r2, r6
3000a084:	460b      	mov	r3, r1
3000a086:	d3f1      	bcc.n	3000a06c <NAND_RxData+0x104>
3000a088:	4614      	mov	r4, r2
3000a08a:	e7ce      	b.n	3000a02a <NAND_RxData+0xc2>
3000a08c:	f8df 9008 	ldr.w	r9, [pc, #8]	; 3000a098 <NAND_RxData+0x130>
3000a090:	f899 5008 	ldrb.w	r5, [r9, #8]
3000a094:	e773      	b.n	30009f7e <NAND_RxData+0x16>
3000a096:	bf00      	nop
3000a098:	2001c01c 	.word	0x2001c01c
3000a09c:	0000b9c1 	.word	0x0000b9c1

3000a0a0 <NAND_Page_Read>:
3000a0a0:	b530      	push	{r4, r5, lr}
3000a0a2:	b085      	sub	sp, #20
3000a0a4:	4c07      	ldr	r4, [pc, #28]	; (3000a0c4 <NAND_Page_Read+0x24>)
3000a0a6:	4d08      	ldr	r5, [pc, #32]	; (3000a0c8 <NAND_Page_Read+0x28>)
3000a0a8:	9301      	str	r3, [sp, #4]
3000a0aa:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000a0ae:	47a0      	blx	r4
3000a0b0:	4604      	mov	r4, r0
3000a0b2:	9903      	ldr	r1, [sp, #12]
3000a0b4:	7a28      	ldrb	r0, [r5, #8]
3000a0b6:	4d05      	ldr	r5, [pc, #20]	; (3000a0cc <NAND_Page_Read+0x2c>)
3000a0b8:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000a0bc:	47a8      	blx	r5
3000a0be:	4620      	mov	r0, r4
3000a0c0:	b005      	add	sp, #20
3000a0c2:	bd30      	pop	{r4, r5, pc}
3000a0c4:	0000bd85 	.word	0x0000bd85
3000a0c8:	2001c01c 	.word	0x2001c01c
3000a0cc:	30009f69 	.word	0x30009f69

3000a0d0 <irq_register>:
3000a0d0:	b538      	push	{r3, r4, r5, lr}
3000a0d2:	f040 0001 	orr.w	r0, r0, #1
3000a0d6:	461c      	mov	r4, r3
3000a0d8:	4b14      	ldr	r3, [pc, #80]	; (3000a12c <irq_register+0x5c>)
3000a0da:	460d      	mov	r5, r1
3000a0dc:	2c07      	cmp	r4, #7
3000a0de:	f843 0021 	str.w	r0, [r3, r1, lsl #2]
3000a0e2:	4b13      	ldr	r3, [pc, #76]	; (3000a130 <irq_register+0x60>)
3000a0e4:	f843 2021 	str.w	r2, [r3, r1, lsl #2]
3000a0e8:	d908      	bls.n	3000a0fc <irq_register+0x2c>
3000a0ea:	4b12      	ldr	r3, [pc, #72]	; (3000a134 <irq_register+0x64>)
3000a0ec:	681b      	ldr	r3, [r3, #0]
3000a0ee:	039b      	lsls	r3, r3, #14
3000a0f0:	d416      	bmi.n	3000a120 <irq_register+0x50>
3000a0f2:	f44f 7144 	mov.w	r1, #784	; 0x310
3000a0f6:	4810      	ldr	r0, [pc, #64]	; (3000a138 <irq_register+0x68>)
3000a0f8:	f001 fe8a 	bl	3000be10 <__io_assert_failed_veneer>
3000a0fc:	2c07      	cmp	r4, #7
3000a0fe:	bf28      	it	cs
3000a100:	2407      	movcs	r4, #7
3000a102:	2d00      	cmp	r5, #0
3000a104:	ea4f 1444 	mov.w	r4, r4, lsl #5
3000a108:	b2e4      	uxtb	r4, r4
3000a10a:	db03      	blt.n	3000a114 <irq_register+0x44>
3000a10c:	4b0b      	ldr	r3, [pc, #44]	; (3000a13c <irq_register+0x6c>)
3000a10e:	2001      	movs	r0, #1
3000a110:	555c      	strb	r4, [r3, r5]
3000a112:	bd38      	pop	{r3, r4, r5, pc}
3000a114:	f005 050f 	and.w	r5, r5, #15
3000a118:	4b09      	ldr	r3, [pc, #36]	; (3000a140 <irq_register+0x70>)
3000a11a:	2001      	movs	r0, #1
3000a11c:	555c      	strb	r4, [r3, r5]
3000a11e:	bd38      	pop	{r3, r4, r5, pc}
3000a120:	2307      	movs	r3, #7
3000a122:	4622      	mov	r2, r4
3000a124:	4807      	ldr	r0, [pc, #28]	; (3000a144 <irq_register+0x74>)
3000a126:	f001 fe83 	bl	3000be30 <__DiagPrintf_veneer>
3000a12a:	e7e2      	b.n	3000a0f2 <irq_register+0x22>
3000a12c:	30000184 	.word	0x30000184
3000a130:	300002c4 	.word	0x300002c4
3000a134:	2001c00c 	.word	0x2001c00c
3000a138:	3000d0d8 	.word	0x3000d0d8
3000a13c:	e000e400 	.word	0xe000e400
3000a140:	e000ed14 	.word	0xe000ed14
3000a144:	3000d0b4 	.word	0x3000d0b4

3000a148 <SYSCFG_BootFromNor>:
3000a148:	4b0e      	ldr	r3, [pc, #56]	; (3000a184 <SYSCFG_BootFromNor+0x3c>)
3000a14a:	b510      	push	{r4, lr}
3000a14c:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
3000a150:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
3000a154:	0392      	lsls	r2, r2, #14
3000a156:	d50a      	bpl.n	3000a16e <SYSCFG_BootFromNor+0x26>
3000a158:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
3000a15c:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000a160:	d105      	bne.n	3000a16e <SYSCFG_BootFromNor+0x26>
3000a162:	4b09      	ldr	r3, [pc, #36]	; (3000a188 <SYSCFG_BootFromNor+0x40>)
3000a164:	4798      	blx	r3
3000a166:	3800      	subs	r0, #0
3000a168:	bf18      	it	ne
3000a16a:	2001      	movne	r0, #1
3000a16c:	bd10      	pop	{r4, pc}
3000a16e:	4c07      	ldr	r4, [pc, #28]	; (3000a18c <SYSCFG_BootFromNor+0x44>)
3000a170:	47a0      	blx	r4
3000a172:	2802      	cmp	r0, #2
3000a174:	d004      	beq.n	3000a180 <SYSCFG_BootFromNor+0x38>
3000a176:	47a0      	blx	r4
3000a178:	2801      	cmp	r0, #1
3000a17a:	d1f2      	bne.n	3000a162 <SYSCFG_BootFromNor+0x1a>
3000a17c:	2000      	movs	r0, #0
3000a17e:	bd10      	pop	{r4, pc}
3000a180:	2001      	movs	r0, #1
3000a182:	bd10      	pop	{r4, pc}
3000a184:	42008000 	.word	0x42008000
3000a188:	0000c0bd 	.word	0x0000c0bd
3000a18c:	0000c1a9 	.word	0x0000c1a9

3000a190 <SYSCFG_RLVersion>:
3000a190:	4b07      	ldr	r3, [pc, #28]	; (3000a1b0 <SYSCFG_RLVersion+0x20>)
3000a192:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a196:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a19a:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000a19e:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000a1a2:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a1a6:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a1aa:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a1ae:	4770      	bx	lr
3000a1b0:	42008000 	.word	0x42008000

3000a1b4 <ddr_init_index>:
3000a1b4:	b570      	push	{r4, r5, r6, lr}
3000a1b6:	4b0f      	ldr	r3, [pc, #60]	; (3000a1f4 <ddr_init_index+0x40>)
3000a1b8:	4798      	blx	r3
3000a1ba:	4604      	mov	r4, r0
3000a1bc:	4b0e      	ldr	r3, [pc, #56]	; (3000a1f8 <ddr_init_index+0x44>)
3000a1be:	4798      	blx	r3
3000a1c0:	2c03      	cmp	r4, #3
3000a1c2:	d015      	beq.n	3000a1f0 <ddr_init_index+0x3c>
3000a1c4:	2c01      	cmp	r4, #1
3000a1c6:	d00d      	beq.n	3000a1e4 <ddr_init_index+0x30>
3000a1c8:	4d0c      	ldr	r5, [pc, #48]	; (3000a1fc <ddr_init_index+0x48>)
3000a1ca:	4c0d      	ldr	r4, [pc, #52]	; (3000a200 <ddr_init_index+0x4c>)
3000a1cc:	4e0d      	ldr	r6, [pc, #52]	; (3000a204 <ddr_init_index+0x50>)
3000a1ce:	e002      	b.n	3000a1d6 <ddr_init_index+0x22>
3000a1d0:	f242 7010 	movw	r0, #10000	; 0x2710
3000a1d4:	47a0      	blx	r4
3000a1d6:	682b      	ldr	r3, [r5, #0]
3000a1d8:	039b      	lsls	r3, r3, #14
3000a1da:	d5f9      	bpl.n	3000a1d0 <ddr_init_index+0x1c>
3000a1dc:	4630      	mov	r0, r6
3000a1de:	f001 fe27 	bl	3000be30 <__DiagPrintf_veneer>
3000a1e2:	e7f5      	b.n	3000a1d0 <ddr_init_index+0x1c>
3000a1e4:	2802      	cmp	r0, #2
3000a1e6:	d003      	beq.n	3000a1f0 <ddr_init_index+0x3c>
3000a1e8:	2803      	cmp	r0, #3
3000a1ea:	d1ed      	bne.n	3000a1c8 <ddr_init_index+0x14>
3000a1ec:	2000      	movs	r0, #0
3000a1ee:	bd70      	pop	{r4, r5, r6, pc}
3000a1f0:	4620      	mov	r0, r4
3000a1f2:	bd70      	pop	{r4, r5, r6, pc}
3000a1f4:	30007f35 	.word	0x30007f35
3000a1f8:	30007e55 	.word	0x30007e55
3000a1fc:	2001c00c 	.word	0x2001c00c
3000a200:	00009be5 	.word	0x00009be5
3000a204:	3000d134 	.word	0x3000d134

3000a208 <rxi316_perf_tune>:
3000a208:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a20c:	f8d0 e000 	ldr.w	lr, [r0]
3000a210:	b0bf      	sub	sp, #252	; 0xfc
3000a212:	4606      	mov	r6, r0
3000a214:	f89e 5006 	ldrb.w	r5, [lr, #6]
3000a218:	2d00      	cmp	r5, #0
3000a21a:	f000 833e 	beq.w	3000a89a <rxi316_perf_tune+0x692>
3000a21e:	2d01      	cmp	r5, #1
3000a220:	d003      	beq.n	3000a22a <rxi316_perf_tune+0x22>
3000a222:	2d03      	cmp	r5, #3
3000a224:	bf14      	ite	ne
3000a226:	2502      	movne	r5, #2
3000a228:	2500      	moveq	r5, #0
3000a22a:	68f3      	ldr	r3, [r6, #12]
3000a22c:	2420      	movs	r4, #32
3000a22e:	2100      	movs	r1, #0
3000a230:	685b      	ldr	r3, [r3, #4]
3000a232:	4627      	mov	r7, r4
3000a234:	46a0      	mov	r8, r4
3000a236:	46a1      	mov	r9, r4
3000a238:	785a      	ldrb	r2, [r3, #1]
3000a23a:	46a2      	mov	sl, r4
3000a23c:	7818      	ldrb	r0, [r3, #0]
3000a23e:	46a3      	mov	fp, r4
3000a240:	921f      	str	r2, [sp, #124]	; 0x7c
3000a242:	468c      	mov	ip, r1
3000a244:	789a      	ldrb	r2, [r3, #2]
3000a246:	9220      	str	r2, [sp, #128]	; 0x80
3000a248:	78da      	ldrb	r2, [r3, #3]
3000a24a:	9221      	str	r2, [sp, #132]	; 0x84
3000a24c:	791a      	ldrb	r2, [r3, #4]
3000a24e:	9222      	str	r2, [sp, #136]	; 0x88
3000a250:	795a      	ldrb	r2, [r3, #5]
3000a252:	9223      	str	r2, [sp, #140]	; 0x8c
3000a254:	799a      	ldrb	r2, [r3, #6]
3000a256:	9224      	str	r2, [sp, #144]	; 0x90
3000a258:	79da      	ldrb	r2, [r3, #7]
3000a25a:	9225      	str	r2, [sp, #148]	; 0x94
3000a25c:	7a1a      	ldrb	r2, [r3, #8]
3000a25e:	9226      	str	r2, [sp, #152]	; 0x98
3000a260:	7a5a      	ldrb	r2, [r3, #9]
3000a262:	9227      	str	r2, [sp, #156]	; 0x9c
3000a264:	7a9a      	ldrb	r2, [r3, #10]
3000a266:	9228      	str	r2, [sp, #160]	; 0xa0
3000a268:	7ada      	ldrb	r2, [r3, #11]
3000a26a:	9229      	str	r2, [sp, #164]	; 0xa4
3000a26c:	7b1a      	ldrb	r2, [r3, #12]
3000a26e:	922a      	str	r2, [sp, #168]	; 0xa8
3000a270:	7b5a      	ldrb	r2, [r3, #13]
3000a272:	922b      	str	r2, [sp, #172]	; 0xac
3000a274:	7b9a      	ldrb	r2, [r3, #14]
3000a276:	922c      	str	r2, [sp, #176]	; 0xb0
3000a278:	7bda      	ldrb	r2, [r3, #15]
3000a27a:	922d      	str	r2, [sp, #180]	; 0xb4
3000a27c:	7c1a      	ldrb	r2, [r3, #16]
3000a27e:	941a      	str	r4, [sp, #104]	; 0x68
3000a280:	922e      	str	r2, [sp, #184]	; 0xb8
3000a282:	7c5a      	ldrb	r2, [r3, #17]
3000a284:	922f      	str	r2, [sp, #188]	; 0xbc
3000a286:	7c9a      	ldrb	r2, [r3, #18]
3000a288:	9230      	str	r2, [sp, #192]	; 0xc0
3000a28a:	7cda      	ldrb	r2, [r3, #19]
3000a28c:	9231      	str	r2, [sp, #196]	; 0xc4
3000a28e:	7d1a      	ldrb	r2, [r3, #20]
3000a290:	9232      	str	r2, [sp, #200]	; 0xc8
3000a292:	7d5a      	ldrb	r2, [r3, #21]
3000a294:	9233      	str	r2, [sp, #204]	; 0xcc
3000a296:	7d9a      	ldrb	r2, [r3, #22]
3000a298:	9234      	str	r2, [sp, #208]	; 0xd0
3000a29a:	7dda      	ldrb	r2, [r3, #23]
3000a29c:	9235      	str	r2, [sp, #212]	; 0xd4
3000a29e:	7e1a      	ldrb	r2, [r3, #24]
3000a2a0:	9236      	str	r2, [sp, #216]	; 0xd8
3000a2a2:	7e5a      	ldrb	r2, [r3, #25]
3000a2a4:	9237      	str	r2, [sp, #220]	; 0xdc
3000a2a6:	7e9a      	ldrb	r2, [r3, #26]
3000a2a8:	9238      	str	r2, [sp, #224]	; 0xe0
3000a2aa:	7eda      	ldrb	r2, [r3, #27]
3000a2ac:	9239      	str	r2, [sp, #228]	; 0xe4
3000a2ae:	7f1a      	ldrb	r2, [r3, #28]
3000a2b0:	923a      	str	r2, [sp, #232]	; 0xe8
3000a2b2:	7f5a      	ldrb	r2, [r3, #29]
3000a2b4:	923b      	str	r2, [sp, #236]	; 0xec
3000a2b6:	7f9a      	ldrb	r2, [r3, #30]
3000a2b8:	7fdb      	ldrb	r3, [r3, #31]
3000a2ba:	923c      	str	r2, [sp, #240]	; 0xf0
3000a2bc:	460a      	mov	r2, r1
3000a2be:	933d      	str	r3, [sp, #244]	; 0xf4
3000a2c0:	4623      	mov	r3, r4
3000a2c2:	9406      	str	r4, [sp, #24]
3000a2c4:	9419      	str	r4, [sp, #100]	; 0x64
3000a2c6:	9418      	str	r4, [sp, #96]	; 0x60
3000a2c8:	9407      	str	r4, [sp, #28]
3000a2ca:	951b      	str	r5, [sp, #108]	; 0x6c
3000a2cc:	e9cd 4402 	strd	r4, r4, [sp, #8]
3000a2d0:	e9cd 4400 	strd	r4, r4, [sp]
3000a2d4:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a2d8:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a2dc:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a2e0:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a2e4:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a2e8:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a2ec:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a2f0:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a2f4:	e9cd 4408 	strd	r4, r4, [sp, #32]
3000a2f8:	b950      	cbnz	r0, 3000a310 <rxi316_perf_tune+0x108>
3000a2fa:	f10c 0c01 	add.w	ip, ip, #1
3000a2fe:	4613      	mov	r3, r2
3000a300:	3201      	adds	r2, #1
3000a302:	2a20      	cmp	r2, #32
3000a304:	d06f      	beq.n	3000a3e6 <rxi316_perf_tune+0x1de>
3000a306:	a81e      	add	r0, sp, #120	; 0x78
3000a308:	f850 0022 	ldr.w	r0, [r0, r2, lsl #2]
3000a30c:	2800      	cmp	r0, #0
3000a30e:	d0f4      	beq.n	3000a2fa <rxi316_perf_tune+0xf2>
3000a310:	2801      	cmp	r0, #1
3000a312:	d062      	beq.n	3000a3da <rxi316_perf_tune+0x1d2>
3000a314:	2802      	cmp	r0, #2
3000a316:	f000 8083 	beq.w	3000a420 <rxi316_perf_tune+0x218>
3000a31a:	2803      	cmp	r0, #3
3000a31c:	f000 8084 	beq.w	3000a428 <rxi316_perf_tune+0x220>
3000a320:	2804      	cmp	r0, #4
3000a322:	f000 8085 	beq.w	3000a430 <rxi316_perf_tune+0x228>
3000a326:	2805      	cmp	r0, #5
3000a328:	f000 8086 	beq.w	3000a438 <rxi316_perf_tune+0x230>
3000a32c:	2806      	cmp	r0, #6
3000a32e:	f000 8087 	beq.w	3000a440 <rxi316_perf_tune+0x238>
3000a332:	2807      	cmp	r0, #7
3000a334:	f000 82b3 	beq.w	3000a89e <rxi316_perf_tune+0x696>
3000a338:	2808      	cmp	r0, #8
3000a33a:	f000 8375 	beq.w	3000aa28 <rxi316_perf_tune+0x820>
3000a33e:	2809      	cmp	r0, #9
3000a340:	f000 8376 	beq.w	3000aa30 <rxi316_perf_tune+0x828>
3000a344:	280a      	cmp	r0, #10
3000a346:	f000 837c 	beq.w	3000aa42 <rxi316_perf_tune+0x83a>
3000a34a:	280b      	cmp	r0, #11
3000a34c:	f000 8376 	beq.w	3000aa3c <rxi316_perf_tune+0x834>
3000a350:	280c      	cmp	r0, #12
3000a352:	f000 8383 	beq.w	3000aa5c <rxi316_perf_tune+0x854>
3000a356:	280d      	cmp	r0, #13
3000a358:	f000 8386 	beq.w	3000aa68 <rxi316_perf_tune+0x860>
3000a35c:	280e      	cmp	r0, #14
3000a35e:	f000 8380 	beq.w	3000aa62 <rxi316_perf_tune+0x85a>
3000a362:	280f      	cmp	r0, #15
3000a364:	f000 8383 	beq.w	3000aa6e <rxi316_perf_tune+0x866>
3000a368:	2810      	cmp	r0, #16
3000a36a:	f000 8383 	beq.w	3000aa74 <rxi316_perf_tune+0x86c>
3000a36e:	2811      	cmp	r0, #17
3000a370:	f000 8382 	beq.w	3000aa78 <rxi316_perf_tune+0x870>
3000a374:	2812      	cmp	r0, #18
3000a376:	f000 8381 	beq.w	3000aa7c <rxi316_perf_tune+0x874>
3000a37a:	2813      	cmp	r0, #19
3000a37c:	f000 8382 	beq.w	3000aa84 <rxi316_perf_tune+0x87c>
3000a380:	2814      	cmp	r0, #20
3000a382:	f000 837d 	beq.w	3000aa80 <rxi316_perf_tune+0x878>
3000a386:	2815      	cmp	r0, #21
3000a388:	f000 8384 	beq.w	3000aa94 <rxi316_perf_tune+0x88c>
3000a38c:	2816      	cmp	r0, #22
3000a38e:	f000 837f 	beq.w	3000aa90 <rxi316_perf_tune+0x888>
3000a392:	2817      	cmp	r0, #23
3000a394:	f000 838e 	beq.w	3000aab4 <rxi316_perf_tune+0x8ac>
3000a398:	2818      	cmp	r0, #24
3000a39a:	f000 8389 	beq.w	3000aab0 <rxi316_perf_tune+0x8a8>
3000a39e:	2819      	cmp	r0, #25
3000a3a0:	f000 8384 	beq.w	3000aaac <rxi316_perf_tune+0x8a4>
3000a3a4:	281a      	cmp	r0, #26
3000a3a6:	f000 837f 	beq.w	3000aaa8 <rxi316_perf_tune+0x8a0>
3000a3aa:	281b      	cmp	r0, #27
3000a3ac:	f000 837a 	beq.w	3000aaa4 <rxi316_perf_tune+0x89c>
3000a3b0:	281c      	cmp	r0, #28
3000a3b2:	f000 8375 	beq.w	3000aaa0 <rxi316_perf_tune+0x898>
3000a3b6:	281d      	cmp	r0, #29
3000a3b8:	f000 8370 	beq.w	3000aa9c <rxi316_perf_tune+0x894>
3000a3bc:	281e      	cmp	r0, #30
3000a3be:	f000 836b 	beq.w	3000aa98 <rxi316_perf_tune+0x890>
3000a3c2:	281f      	cmp	r0, #31
3000a3c4:	f000 8362 	beq.w	3000aa8c <rxi316_perf_tune+0x884>
3000a3c8:	2820      	cmp	r0, #32
3000a3ca:	f000 835d 	beq.w	3000aa88 <rxi316_perf_tune+0x880>
3000a3ce:	9d06      	ldr	r5, [sp, #24]
3000a3d0:	2821      	cmp	r0, #33	; 0x21
3000a3d2:	bf08      	it	eq
3000a3d4:	4615      	moveq	r5, r2
3000a3d6:	9506      	str	r5, [sp, #24]
3000a3d8:	e792      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a3da:	4693      	mov	fp, r2
3000a3dc:	3201      	adds	r2, #1
3000a3de:	f10c 0c01 	add.w	ip, ip, #1
3000a3e2:	2a20      	cmp	r2, #32
3000a3e4:	d18f      	bne.n	3000a306 <rxi316_perf_tune+0xfe>
3000a3e6:	f89e 2002 	ldrb.w	r2, [lr, #2]
3000a3ea:	9d1b      	ldr	r5, [sp, #108]	; 0x6c
3000a3ec:	f8de 0008 	ldr.w	r0, [lr, #8]
3000a3f0:	b372      	cbz	r2, 3000a450 <rxi316_perf_tune+0x248>
3000a3f2:	2a01      	cmp	r2, #1
3000a3f4:	f000 8320 	beq.w	3000aa38 <rxi316_perf_tune+0x830>
3000a3f8:	2a03      	cmp	r2, #3
3000a3fa:	f1a0 0002 	sub.w	r0, r0, #2
3000a3fe:	bf0c      	ite	eq
3000a400:	2204      	moveq	r2, #4
3000a402:	2203      	movne	r2, #3
3000a404:	4560      	cmp	r0, ip
3000a406:	d127      	bne.n	3000a458 <rxi316_perf_tune+0x250>
3000a408:	4291      	cmp	r1, r2
3000a40a:	d02a      	beq.n	3000a462 <rxi316_perf_tune+0x25a>
3000a40c:	4a0e      	ldr	r2, [pc, #56]	; (3000a448 <rxi316_perf_tune+0x240>)
3000a40e:	6812      	ldr	r2, [r2, #0]
3000a410:	0392      	lsls	r2, r2, #14
3000a412:	d526      	bpl.n	3000a462 <rxi316_perf_tune+0x25a>
3000a414:	480d      	ldr	r0, [pc, #52]	; (3000a44c <rxi316_perf_tune+0x244>)
3000a416:	931b      	str	r3, [sp, #108]	; 0x6c
3000a418:	f001 fd0a 	bl	3000be30 <__DiagPrintf_veneer>
3000a41c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a41e:	e020      	b.n	3000a462 <rxi316_perf_tune+0x25a>
3000a420:	f10c 0c01 	add.w	ip, ip, #1
3000a424:	4692      	mov	sl, r2
3000a426:	e76b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a428:	f10c 0c01 	add.w	ip, ip, #1
3000a42c:	4691      	mov	r9, r2
3000a42e:	e767      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a430:	f10c 0c01 	add.w	ip, ip, #1
3000a434:	4690      	mov	r8, r2
3000a436:	e763      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a438:	f10c 0c01 	add.w	ip, ip, #1
3000a43c:	4617      	mov	r7, r2
3000a43e:	e75f      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a440:	f10c 0c01 	add.w	ip, ip, #1
3000a444:	9200      	str	r2, [sp, #0]
3000a446:	e75b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a448:	2001c00c 	.word	0x2001c00c
3000a44c:	3000d194 	.word	0x3000d194
3000a450:	2201      	movs	r2, #1
3000a452:	3802      	subs	r0, #2
3000a454:	4560      	cmp	r0, ip
3000a456:	d0d7      	beq.n	3000a408 <rxi316_perf_tune+0x200>
3000a458:	48c4      	ldr	r0, [pc, #784]	; (3000a76c <rxi316_perf_tune+0x564>)
3000a45a:	6800      	ldr	r0, [r0, #0]
3000a45c:	0380      	lsls	r0, r0, #14
3000a45e:	f100 82f3 	bmi.w	3000aa48 <rxi316_perf_tune+0x840>
3000a462:	9a07      	ldr	r2, [sp, #28]
3000a464:	2a20      	cmp	r2, #32
3000a466:	f000 82dc 	beq.w	3000aa22 <rxi316_perf_tune+0x81a>
3000a46a:	1b52      	subs	r2, r2, r5
3000a46c:	3a05      	subs	r2, #5
3000a46e:	f002 020f 	and.w	r2, r2, #15
3000a472:	9207      	str	r2, [sp, #28]
3000a474:	9a08      	ldr	r2, [sp, #32]
3000a476:	2a20      	cmp	r2, #32
3000a478:	f000 82d1 	beq.w	3000aa1e <rxi316_perf_tune+0x816>
3000a47c:	1b52      	subs	r2, r2, r5
3000a47e:	3a06      	subs	r2, #6
3000a480:	0112      	lsls	r2, r2, #4
3000a482:	b2d2      	uxtb	r2, r2
3000a484:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a486:	2920      	cmp	r1, #32
3000a488:	f000 82c5 	beq.w	3000aa16 <rxi316_perf_tune+0x80e>
3000a48c:	1b49      	subs	r1, r1, r5
3000a48e:	3907      	subs	r1, #7
3000a490:	0209      	lsls	r1, r1, #8
3000a492:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a496:	9108      	str	r1, [sp, #32]
3000a498:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a49a:	2920      	cmp	r1, #32
3000a49c:	f000 821a 	beq.w	3000a8d4 <rxi316_perf_tune+0x6cc>
3000a4a0:	1b49      	subs	r1, r1, r5
3000a4a2:	3908      	subs	r1, #8
3000a4a4:	0309      	lsls	r1, r1, #12
3000a4a6:	b289      	uxth	r1, r1
3000a4a8:	9109      	str	r1, [sp, #36]	; 0x24
3000a4aa:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a4ac:	2920      	cmp	r1, #32
3000a4ae:	f000 8218 	beq.w	3000a8e2 <rxi316_perf_tune+0x6da>
3000a4b2:	1b49      	subs	r1, r1, r5
3000a4b4:	3909      	subs	r1, #9
3000a4b6:	0409      	lsls	r1, r1, #16
3000a4b8:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a4bc:	910a      	str	r1, [sp, #40]	; 0x28
3000a4be:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a4c0:	2920      	cmp	r1, #32
3000a4c2:	f000 8215 	beq.w	3000a8f0 <rxi316_perf_tune+0x6e8>
3000a4c6:	1b49      	subs	r1, r1, r5
3000a4c8:	390a      	subs	r1, #10
3000a4ca:	0509      	lsls	r1, r1, #20
3000a4cc:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a4d0:	910b      	str	r1, [sp, #44]	; 0x2c
3000a4d2:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a4d4:	2920      	cmp	r1, #32
3000a4d6:	f000 8212 	beq.w	3000a8fe <rxi316_perf_tune+0x6f6>
3000a4da:	1b49      	subs	r1, r1, r5
3000a4dc:	390b      	subs	r1, #11
3000a4de:	0609      	lsls	r1, r1, #24
3000a4e0:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a4e4:	910c      	str	r1, [sp, #48]	; 0x30
3000a4e6:	990e      	ldr	r1, [sp, #56]	; 0x38
3000a4e8:	2920      	cmp	r1, #32
3000a4ea:	f000 820f 	beq.w	3000a90c <rxi316_perf_tune+0x704>
3000a4ee:	1b49      	subs	r1, r1, r5
3000a4f0:	390c      	subs	r1, #12
3000a4f2:	0709      	lsls	r1, r1, #28
3000a4f4:	910d      	str	r1, [sp, #52]	; 0x34
3000a4f6:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000a4f8:	2920      	cmp	r1, #32
3000a4fa:	f000 820e 	beq.w	3000a91a <rxi316_perf_tune+0x712>
3000a4fe:	1b49      	subs	r1, r1, r5
3000a500:	390d      	subs	r1, #13
3000a502:	f001 010f 	and.w	r1, r1, #15
3000a506:	910e      	str	r1, [sp, #56]	; 0x38
3000a508:	9910      	ldr	r1, [sp, #64]	; 0x40
3000a50a:	2920      	cmp	r1, #32
3000a50c:	f000 820b 	beq.w	3000a926 <rxi316_perf_tune+0x71e>
3000a510:	1b49      	subs	r1, r1, r5
3000a512:	390e      	subs	r1, #14
3000a514:	0109      	lsls	r1, r1, #4
3000a516:	b2c9      	uxtb	r1, r1
3000a518:	910f      	str	r1, [sp, #60]	; 0x3c
3000a51a:	9911      	ldr	r1, [sp, #68]	; 0x44
3000a51c:	2920      	cmp	r1, #32
3000a51e:	f000 8208 	beq.w	3000a932 <rxi316_perf_tune+0x72a>
3000a522:	1b49      	subs	r1, r1, r5
3000a524:	390f      	subs	r1, #15
3000a526:	0209      	lsls	r1, r1, #8
3000a528:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a52c:	9110      	str	r1, [sp, #64]	; 0x40
3000a52e:	9912      	ldr	r1, [sp, #72]	; 0x48
3000a530:	2920      	cmp	r1, #32
3000a532:	f000 8205 	beq.w	3000a940 <rxi316_perf_tune+0x738>
3000a536:	1b49      	subs	r1, r1, r5
3000a538:	0309      	lsls	r1, r1, #12
3000a53a:	f401 4170 	and.w	r1, r1, #61440	; 0xf000
3000a53e:	9111      	str	r1, [sp, #68]	; 0x44
3000a540:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a542:	2920      	cmp	r1, #32
3000a544:	f000 8203 	beq.w	3000a94e <rxi316_perf_tune+0x746>
3000a548:	1b49      	subs	r1, r1, r5
3000a54a:	3911      	subs	r1, #17
3000a54c:	0409      	lsls	r1, r1, #16
3000a54e:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a552:	9112      	str	r1, [sp, #72]	; 0x48
3000a554:	9914      	ldr	r1, [sp, #80]	; 0x50
3000a556:	2920      	cmp	r1, #32
3000a558:	f000 8200 	beq.w	3000a95c <rxi316_perf_tune+0x754>
3000a55c:	1b49      	subs	r1, r1, r5
3000a55e:	3912      	subs	r1, #18
3000a560:	0509      	lsls	r1, r1, #20
3000a562:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a566:	9113      	str	r1, [sp, #76]	; 0x4c
3000a568:	9915      	ldr	r1, [sp, #84]	; 0x54
3000a56a:	2920      	cmp	r1, #32
3000a56c:	f000 81fd 	beq.w	3000a96a <rxi316_perf_tune+0x762>
3000a570:	1b49      	subs	r1, r1, r5
3000a572:	3913      	subs	r1, #19
3000a574:	0609      	lsls	r1, r1, #24
3000a576:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a57a:	911d      	str	r1, [sp, #116]	; 0x74
3000a57c:	9916      	ldr	r1, [sp, #88]	; 0x58
3000a57e:	2920      	cmp	r1, #32
3000a580:	f000 81fa 	beq.w	3000a978 <rxi316_perf_tune+0x770>
3000a584:	1b49      	subs	r1, r1, r5
3000a586:	3914      	subs	r1, #20
3000a588:	0709      	lsls	r1, r1, #28
3000a58a:	911c      	str	r1, [sp, #112]	; 0x70
3000a58c:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000a58e:	2920      	cmp	r1, #32
3000a590:	f000 81f9 	beq.w	3000a986 <rxi316_perf_tune+0x77e>
3000a594:	1b49      	subs	r1, r1, r5
3000a596:	3915      	subs	r1, #21
3000a598:	f001 010f 	and.w	r1, r1, #15
3000a59c:	911b      	str	r1, [sp, #108]	; 0x6c
3000a59e:	9904      	ldr	r1, [sp, #16]
3000a5a0:	2920      	cmp	r1, #32
3000a5a2:	f000 81f6 	beq.w	3000a992 <rxi316_perf_tune+0x78a>
3000a5a6:	1b49      	subs	r1, r1, r5
3000a5a8:	3902      	subs	r1, #2
3000a5aa:	f001 011f 	and.w	r1, r1, #31
3000a5ae:	9117      	str	r1, [sp, #92]	; 0x5c
3000a5b0:	9905      	ldr	r1, [sp, #20]
3000a5b2:	2920      	cmp	r1, #32
3000a5b4:	f000 81f3 	beq.w	3000a99e <rxi316_perf_tune+0x796>
3000a5b8:	1b49      	subs	r1, r1, r5
3000a5ba:	3902      	subs	r1, #2
3000a5bc:	0149      	lsls	r1, r1, #5
3000a5be:	f401 7178 	and.w	r1, r1, #992	; 0x3e0
3000a5c2:	9116      	str	r1, [sp, #88]	; 0x58
3000a5c4:	9918      	ldr	r1, [sp, #96]	; 0x60
3000a5c6:	2920      	cmp	r1, #32
3000a5c8:	f000 81f0 	beq.w	3000a9ac <rxi316_perf_tune+0x7a4>
3000a5cc:	1b49      	subs	r1, r1, r5
3000a5ce:	2b20      	cmp	r3, #32
3000a5d0:	f1a1 0102 	sub.w	r1, r1, #2
3000a5d4:	ea4f 2181 	mov.w	r1, r1, lsl #10
3000a5d8:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000a5dc:	9115      	str	r1, [sp, #84]	; 0x54
3000a5de:	f000 81eb 	beq.w	3000a9b8 <rxi316_perf_tune+0x7b0>
3000a5e2:	1b59      	subs	r1, r3, r5
3000a5e4:	f1bb 0f20 	cmp.w	fp, #32
3000a5e8:	f1a1 0102 	sub.w	r1, r1, #2
3000a5ec:	f001 030f 	and.w	r3, r1, #15
3000a5f0:	9314      	str	r3, [sp, #80]	; 0x50
3000a5f2:	f000 81e7 	beq.w	3000a9c4 <rxi316_perf_tune+0x7bc>
3000a5f6:	ebab 0105 	sub.w	r1, fp, r5
3000a5fa:	f1ba 0f20 	cmp.w	sl, #32
3000a5fe:	f1a1 0103 	sub.w	r1, r1, #3
3000a602:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a606:	b2c9      	uxtb	r1, r1
3000a608:	f000 81e2 	beq.w	3000a9d0 <rxi316_perf_tune+0x7c8>
3000a60c:	ebaa 0305 	sub.w	r3, sl, r5
3000a610:	f1b9 0f20 	cmp.w	r9, #32
3000a614:	f1a3 0304 	sub.w	r3, r3, #4
3000a618:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a61c:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a620:	9305      	str	r3, [sp, #20]
3000a622:	f000 81dc 	beq.w	3000a9de <rxi316_perf_tune+0x7d6>
3000a626:	eba9 0305 	sub.w	r3, r9, r5
3000a62a:	f1b8 0f20 	cmp.w	r8, #32
3000a62e:	f1a3 0305 	sub.w	r3, r3, #5
3000a632:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a636:	b29b      	uxth	r3, r3
3000a638:	9304      	str	r3, [sp, #16]
3000a63a:	f000 81d7 	beq.w	3000a9ec <rxi316_perf_tune+0x7e4>
3000a63e:	eba8 0c05 	sub.w	ip, r8, r5
3000a642:	2f20      	cmp	r7, #32
3000a644:	f1ac 0c06 	sub.w	ip, ip, #6
3000a648:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a64c:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a650:	f000 81d1 	beq.w	3000a9f6 <rxi316_perf_tune+0x7ee>
3000a654:	eba7 0e05 	sub.w	lr, r7, r5
3000a658:	9b00      	ldr	r3, [sp, #0]
3000a65a:	f1ae 0e07 	sub.w	lr, lr, #7
3000a65e:	2b20      	cmp	r3, #32
3000a660:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a664:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a668:	f000 81cb 	beq.w	3000aa02 <rxi316_perf_tune+0x7fa>
3000a66c:	1b5f      	subs	r7, r3, r5
3000a66e:	9b01      	ldr	r3, [sp, #4]
3000a670:	3f08      	subs	r7, #8
3000a672:	2b20      	cmp	r3, #32
3000a674:	ea4f 6707 	mov.w	r7, r7, lsl #24
3000a678:	f007 6770 	and.w	r7, r7, #251658240	; 0xf000000
3000a67c:	f000 81c7 	beq.w	3000aa0e <rxi316_perf_tune+0x806>
3000a680:	1b5b      	subs	r3, r3, r5
3000a682:	3b09      	subs	r3, #9
3000a684:	071b      	lsls	r3, r3, #28
3000a686:	9301      	str	r3, [sp, #4]
3000a688:	9b02      	ldr	r3, [sp, #8]
3000a68a:	2b20      	cmp	r3, #32
3000a68c:	f000 811f 	beq.w	3000a8ce <rxi316_perf_tune+0x6c6>
3000a690:	eba3 0805 	sub.w	r8, r3, r5
3000a694:	f1a8 080a 	sub.w	r8, r8, #10
3000a698:	f008 080f 	and.w	r8, r8, #15
3000a69c:	9b03      	ldr	r3, [sp, #12]
3000a69e:	2b20      	cmp	r3, #32
3000a6a0:	f000 8113 	beq.w	3000a8ca <rxi316_perf_tune+0x6c2>
3000a6a4:	1b58      	subs	r0, r3, r5
3000a6a6:	380b      	subs	r0, #11
3000a6a8:	0100      	lsls	r0, r0, #4
3000a6aa:	b2c0      	uxtb	r0, r0
3000a6ac:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a6ae:	2b20      	cmp	r3, #32
3000a6b0:	f000 8108 	beq.w	3000a8c4 <rxi316_perf_tune+0x6bc>
3000a6b4:	eba3 0a05 	sub.w	sl, r3, r5
3000a6b8:	f1aa 0a02 	sub.w	sl, sl, #2
3000a6bc:	f00a 0a1f 	and.w	sl, sl, #31
3000a6c0:	9b06      	ldr	r3, [sp, #24]
3000a6c2:	2b20      	cmp	r3, #32
3000a6c4:	f000 80ef 	beq.w	3000a8a6 <rxi316_perf_tune+0x69e>
3000a6c8:	eba3 0905 	sub.w	r9, r3, r5
3000a6cc:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a6ce:	f1a9 0902 	sub.w	r9, r9, #2
3000a6d2:	2b20      	cmp	r3, #32
3000a6d4:	f009 091f 	and.w	r9, r9, #31
3000a6d8:	f000 80eb 	beq.w	3000a8b2 <rxi316_perf_tune+0x6aa>
3000a6dc:	eba3 0b05 	sub.w	fp, r3, r5
3000a6e0:	2c20      	cmp	r4, #32
3000a6e2:	f1ab 0b02 	sub.w	fp, fp, #2
3000a6e6:	f00b 0b1f 	and.w	fp, fp, #31
3000a6ea:	f000 80e7 	beq.w	3000a8bc <rxi316_perf_tune+0x6b4>
3000a6ee:	1b63      	subs	r3, r4, r5
3000a6f0:	3b02      	subs	r3, #2
3000a6f2:	015b      	lsls	r3, r3, #5
3000a6f4:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a6f8:	9300      	str	r3, [sp, #0]
3000a6fa:	9c07      	ldr	r4, [sp, #28]
3000a6fc:	ea40 0008 	orr.w	r0, r0, r8
3000a700:	4314      	orrs	r4, r2
3000a702:	4622      	mov	r2, r4
3000a704:	e9dd 450e 	ldrd	r4, r5, [sp, #56]	; 0x38
3000a708:	432c      	orrs	r4, r5
3000a70a:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000a70c:	4329      	orrs	r1, r5
3000a70e:	e9dd 5316 	ldrd	r5, r3, [sp, #88]	; 0x58
3000a712:	431d      	orrs	r5, r3
3000a714:	9b08      	ldr	r3, [sp, #32]
3000a716:	431a      	orrs	r2, r3
3000a718:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a71a:	431c      	orrs	r4, r3
3000a71c:	9b05      	ldr	r3, [sp, #20]
3000a71e:	4319      	orrs	r1, r3
3000a720:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a722:	431d      	orrs	r5, r3
3000a724:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a726:	431a      	orrs	r2, r3
3000a728:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a72a:	431c      	orrs	r4, r3
3000a72c:	9b04      	ldr	r3, [sp, #16]
3000a72e:	4319      	orrs	r1, r3
3000a730:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a732:	ea41 0c0c 	orr.w	ip, r1, ip
3000a736:	431a      	orrs	r2, r3
3000a738:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a73a:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a73c:	ea4c 0c0e 	orr.w	ip, ip, lr
3000a740:	430a      	orrs	r2, r1
3000a742:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a744:	431c      	orrs	r4, r3
3000a746:	ea4c 0c07 	orr.w	ip, ip, r7
3000a74a:	4f09      	ldr	r7, [pc, #36]	; (3000a770 <rxi316_perf_tune+0x568>)
3000a74c:	430c      	orrs	r4, r1
3000a74e:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a750:	9b00      	ldr	r3, [sp, #0]
3000a752:	430a      	orrs	r2, r1
3000a754:	991d      	ldr	r1, [sp, #116]	; 0x74
3000a756:	ea43 030b 	orr.w	r3, r3, fp
3000a75a:	430c      	orrs	r4, r1
3000a75c:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a75e:	430a      	orrs	r2, r1
3000a760:	991c      	ldr	r1, [sp, #112]	; 0x70
3000a762:	4321      	orrs	r1, r4
3000a764:	9c01      	ldr	r4, [sp, #4]
3000a766:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000a76a:	e003      	b.n	3000a774 <rxi316_perf_tune+0x56c>
3000a76c:	2001c00c 	.word	0x2001c00c
3000a770:	41100000 	.word	0x41100000
3000a774:	f8c7 1404 	str.w	r1, [r7, #1028]	; 0x404
3000a778:	ea4c 0404 	orr.w	r4, ip, r4
3000a77c:	9a1b      	ldr	r2, [sp, #108]	; 0x6c
3000a77e:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000a782:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000a786:	f8c7 4410 	str.w	r4, [r7, #1040]	; 0x410
3000a78a:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000a78e:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000a792:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000a796:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000a79a:	68f3      	ldr	r3, [r6, #12]
3000a79c:	689a      	ldr	r2, [r3, #8]
3000a79e:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a7a2:	f003 0301 	and.w	r3, r3, #1
3000a7a6:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a7aa:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000a7ae:	68f3      	ldr	r3, [r6, #12]
3000a7b0:	689a      	ldr	r2, [r3, #8]
3000a7b2:	7a13      	ldrb	r3, [r2, #8]
3000a7b4:	6951      	ldr	r1, [r2, #20]
3000a7b6:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000a7ba:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000a7be:	0209      	lsls	r1, r1, #8
3000a7c0:	0412      	lsls	r2, r2, #16
3000a7c2:	b289      	uxth	r1, r1
3000a7c4:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7c8:	430b      	orrs	r3, r1
3000a7ca:	4313      	orrs	r3, r2
3000a7cc:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000a7d0:	68f3      	ldr	r3, [r6, #12]
3000a7d2:	6899      	ldr	r1, [r3, #8]
3000a7d4:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000a7d8:	7e09      	ldrb	r1, [r1, #24]
3000a7da:	021b      	lsls	r3, r3, #8
3000a7dc:	0412      	lsls	r2, r2, #16
3000a7de:	b29b      	uxth	r3, r3
3000a7e0:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7e4:	4313      	orrs	r3, r2
3000a7e6:	430b      	orrs	r3, r1
3000a7e8:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000a7ec:	68f3      	ldr	r3, [r6, #12]
3000a7ee:	689a      	ldr	r2, [r3, #8]
3000a7f0:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a7f4:	041b      	lsls	r3, r3, #16
3000a7f6:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a7fa:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a7fe:	4313      	orrs	r3, r2
3000a800:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000a804:	68f3      	ldr	r3, [r6, #12]
3000a806:	6899      	ldr	r1, [r3, #8]
3000a808:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a80a:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a80c:	005b      	lsls	r3, r3, #1
3000a80e:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a810:	0412      	lsls	r2, r2, #16
3000a812:	f003 0302 	and.w	r3, r3, #2
3000a816:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a81a:	4313      	orrs	r3, r2
3000a81c:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000a81e:	f000 0101 	and.w	r1, r0, #1
3000a822:	0212      	lsls	r2, r2, #8
3000a824:	430b      	orrs	r3, r1
3000a826:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a82a:	4313      	orrs	r3, r2
3000a82c:	4aa2      	ldr	r2, [pc, #648]	; (3000aab8 <rxi316_perf_tune+0x8b0>)
3000a82e:	431a      	orrs	r2, r3
3000a830:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000a834:	68f3      	ldr	r3, [r6, #12]
3000a836:	689b      	ldr	r3, [r3, #8]
3000a838:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000a83a:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000a83e:	68f3      	ldr	r3, [r6, #12]
3000a840:	689b      	ldr	r3, [r3, #8]
3000a842:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000a844:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000a848:	68f3      	ldr	r3, [r6, #12]
3000a84a:	689b      	ldr	r3, [r3, #8]
3000a84c:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000a84e:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000a852:	68f3      	ldr	r3, [r6, #12]
3000a854:	689b      	ldr	r3, [r3, #8]
3000a856:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000a858:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000a85c:	68f3      	ldr	r3, [r6, #12]
3000a85e:	689b      	ldr	r3, [r3, #8]
3000a860:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000a862:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000a866:	68f3      	ldr	r3, [r6, #12]
3000a868:	689b      	ldr	r3, [r3, #8]
3000a86a:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000a86c:	f003 030f 	and.w	r3, r3, #15
3000a870:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000a874:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000a878:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000a87c:	f043 0304 	orr.w	r3, r3, #4
3000a880:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000a884:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000a888:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000a88c:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000a890:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000a894:	b03f      	add	sp, #252	; 0xfc
3000a896:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a89a:	2502      	movs	r5, #2
3000a89c:	e4c5      	b.n	3000a22a <rxi316_perf_tune+0x22>
3000a89e:	f10c 0c01 	add.w	ip, ip, #1
3000a8a2:	9201      	str	r2, [sp, #4]
3000a8a4:	e52c      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000a8a6:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a8a8:	f04f 091f 	mov.w	r9, #31
3000a8ac:	2b20      	cmp	r3, #32
3000a8ae:	f47f af15 	bne.w	3000a6dc <rxi316_perf_tune+0x4d4>
3000a8b2:	2c20      	cmp	r4, #32
3000a8b4:	f04f 0b1f 	mov.w	fp, #31
3000a8b8:	f47f af19 	bne.w	3000a6ee <rxi316_perf_tune+0x4e6>
3000a8bc:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a8c0:	9300      	str	r3, [sp, #0]
3000a8c2:	e71a      	b.n	3000a6fa <rxi316_perf_tune+0x4f2>
3000a8c4:	f04f 0a1f 	mov.w	sl, #31
3000a8c8:	e6fa      	b.n	3000a6c0 <rxi316_perf_tune+0x4b8>
3000a8ca:	20f0      	movs	r0, #240	; 0xf0
3000a8cc:	e6ee      	b.n	3000a6ac <rxi316_perf_tune+0x4a4>
3000a8ce:	f04f 080f 	mov.w	r8, #15
3000a8d2:	e6e3      	b.n	3000a69c <rxi316_perf_tune+0x494>
3000a8d4:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000a8d8:	9109      	str	r1, [sp, #36]	; 0x24
3000a8da:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a8dc:	2920      	cmp	r1, #32
3000a8de:	f47f ade8 	bne.w	3000a4b2 <rxi316_perf_tune+0x2aa>
3000a8e2:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000a8e6:	910a      	str	r1, [sp, #40]	; 0x28
3000a8e8:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a8ea:	2920      	cmp	r1, #32
3000a8ec:	f47f adeb 	bne.w	3000a4c6 <rxi316_perf_tune+0x2be>
3000a8f0:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000a8f4:	910b      	str	r1, [sp, #44]	; 0x2c
3000a8f6:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a8f8:	2920      	cmp	r1, #32
3000a8fa:	f47f adee 	bne.w	3000a4da <rxi316_perf_tune+0x2d2>
3000a8fe:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000a902:	910c      	str	r1, [sp, #48]	; 0x30
3000a904:	990e      	ldr	r1, [sp, #56]	; 0x38
3000a906:	2920      	cmp	r1, #32
3000a908:	f47f adf1 	bne.w	3000a4ee <rxi316_perf_tune+0x2e6>
3000a90c:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000a910:	910d      	str	r1, [sp, #52]	; 0x34
3000a912:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000a914:	2920      	cmp	r1, #32
3000a916:	f47f adf2 	bne.w	3000a4fe <rxi316_perf_tune+0x2f6>
3000a91a:	210f      	movs	r1, #15
3000a91c:	910e      	str	r1, [sp, #56]	; 0x38
3000a91e:	9910      	ldr	r1, [sp, #64]	; 0x40
3000a920:	2920      	cmp	r1, #32
3000a922:	f47f adf5 	bne.w	3000a510 <rxi316_perf_tune+0x308>
3000a926:	21f0      	movs	r1, #240	; 0xf0
3000a928:	910f      	str	r1, [sp, #60]	; 0x3c
3000a92a:	9911      	ldr	r1, [sp, #68]	; 0x44
3000a92c:	2920      	cmp	r1, #32
3000a92e:	f47f adf8 	bne.w	3000a522 <rxi316_perf_tune+0x31a>
3000a932:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000a936:	9110      	str	r1, [sp, #64]	; 0x40
3000a938:	9912      	ldr	r1, [sp, #72]	; 0x48
3000a93a:	2920      	cmp	r1, #32
3000a93c:	f47f adfb 	bne.w	3000a536 <rxi316_perf_tune+0x32e>
3000a940:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000a944:	9111      	str	r1, [sp, #68]	; 0x44
3000a946:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a948:	2920      	cmp	r1, #32
3000a94a:	f47f adfd 	bne.w	3000a548 <rxi316_perf_tune+0x340>
3000a94e:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000a952:	9112      	str	r1, [sp, #72]	; 0x48
3000a954:	9914      	ldr	r1, [sp, #80]	; 0x50
3000a956:	2920      	cmp	r1, #32
3000a958:	f47f ae00 	bne.w	3000a55c <rxi316_perf_tune+0x354>
3000a95c:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000a960:	9113      	str	r1, [sp, #76]	; 0x4c
3000a962:	9915      	ldr	r1, [sp, #84]	; 0x54
3000a964:	2920      	cmp	r1, #32
3000a966:	f47f ae03 	bne.w	3000a570 <rxi316_perf_tune+0x368>
3000a96a:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000a96e:	911d      	str	r1, [sp, #116]	; 0x74
3000a970:	9916      	ldr	r1, [sp, #88]	; 0x58
3000a972:	2920      	cmp	r1, #32
3000a974:	f47f ae06 	bne.w	3000a584 <rxi316_perf_tune+0x37c>
3000a978:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000a97c:	911c      	str	r1, [sp, #112]	; 0x70
3000a97e:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000a980:	2920      	cmp	r1, #32
3000a982:	f47f ae07 	bne.w	3000a594 <rxi316_perf_tune+0x38c>
3000a986:	210f      	movs	r1, #15
3000a988:	911b      	str	r1, [sp, #108]	; 0x6c
3000a98a:	9904      	ldr	r1, [sp, #16]
3000a98c:	2920      	cmp	r1, #32
3000a98e:	f47f ae0a 	bne.w	3000a5a6 <rxi316_perf_tune+0x39e>
3000a992:	211f      	movs	r1, #31
3000a994:	9117      	str	r1, [sp, #92]	; 0x5c
3000a996:	9905      	ldr	r1, [sp, #20]
3000a998:	2920      	cmp	r1, #32
3000a99a:	f47f ae0d 	bne.w	3000a5b8 <rxi316_perf_tune+0x3b0>
3000a99e:	f44f 7178 	mov.w	r1, #992	; 0x3e0
3000a9a2:	9116      	str	r1, [sp, #88]	; 0x58
3000a9a4:	9918      	ldr	r1, [sp, #96]	; 0x60
3000a9a6:	2920      	cmp	r1, #32
3000a9a8:	f47f ae10 	bne.w	3000a5cc <rxi316_perf_tune+0x3c4>
3000a9ac:	f44f 41f8 	mov.w	r1, #31744	; 0x7c00
3000a9b0:	2b20      	cmp	r3, #32
3000a9b2:	9115      	str	r1, [sp, #84]	; 0x54
3000a9b4:	f47f ae15 	bne.w	3000a5e2 <rxi316_perf_tune+0x3da>
3000a9b8:	230f      	movs	r3, #15
3000a9ba:	f1bb 0f20 	cmp.w	fp, #32
3000a9be:	9314      	str	r3, [sp, #80]	; 0x50
3000a9c0:	f47f ae19 	bne.w	3000a5f6 <rxi316_perf_tune+0x3ee>
3000a9c4:	f1ba 0f20 	cmp.w	sl, #32
3000a9c8:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a9cc:	f47f ae1e 	bne.w	3000a60c <rxi316_perf_tune+0x404>
3000a9d0:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a9d4:	f1b9 0f20 	cmp.w	r9, #32
3000a9d8:	9305      	str	r3, [sp, #20]
3000a9da:	f47f ae24 	bne.w	3000a626 <rxi316_perf_tune+0x41e>
3000a9de:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a9e2:	f1b8 0f20 	cmp.w	r8, #32
3000a9e6:	9304      	str	r3, [sp, #16]
3000a9e8:	f47f ae29 	bne.w	3000a63e <rxi316_perf_tune+0x436>
3000a9ec:	2f20      	cmp	r7, #32
3000a9ee:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a9f2:	f47f ae2f 	bne.w	3000a654 <rxi316_perf_tune+0x44c>
3000a9f6:	9b00      	ldr	r3, [sp, #0]
3000a9f8:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a9fc:	2b20      	cmp	r3, #32
3000a9fe:	f47f ae35 	bne.w	3000a66c <rxi316_perf_tune+0x464>
3000aa02:	9b01      	ldr	r3, [sp, #4]
3000aa04:	f04f 6770 	mov.w	r7, #251658240	; 0xf000000
3000aa08:	2b20      	cmp	r3, #32
3000aa0a:	f47f ae39 	bne.w	3000a680 <rxi316_perf_tune+0x478>
3000aa0e:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000aa12:	9301      	str	r3, [sp, #4]
3000aa14:	e638      	b.n	3000a688 <rxi316_perf_tune+0x480>
3000aa16:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000aa1a:	9108      	str	r1, [sp, #32]
3000aa1c:	e53c      	b.n	3000a498 <rxi316_perf_tune+0x290>
3000aa1e:	22f0      	movs	r2, #240	; 0xf0
3000aa20:	e530      	b.n	3000a484 <rxi316_perf_tune+0x27c>
3000aa22:	220f      	movs	r2, #15
3000aa24:	9207      	str	r2, [sp, #28]
3000aa26:	e525      	b.n	3000a474 <rxi316_perf_tune+0x26c>
3000aa28:	f10c 0c01 	add.w	ip, ip, #1
3000aa2c:	9202      	str	r2, [sp, #8]
3000aa2e:	e467      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa30:	f10c 0c01 	add.w	ip, ip, #1
3000aa34:	9203      	str	r2, [sp, #12]
3000aa36:	e463      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa38:	2202      	movs	r2, #2
3000aa3a:	e50a      	b.n	3000a452 <rxi316_perf_tune+0x24a>
3000aa3c:	3101      	adds	r1, #1
3000aa3e:	9205      	str	r2, [sp, #20]
3000aa40:	e45e      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa42:	3101      	adds	r1, #1
3000aa44:	9204      	str	r2, [sp, #16]
3000aa46:	e45b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa48:	481c      	ldr	r0, [pc, #112]	; (3000aabc <rxi316_perf_tune+0x8b4>)
3000aa4a:	911b      	str	r1, [sp, #108]	; 0x6c
3000aa4c:	e9cd 231c 	strd	r2, r3, [sp, #112]	; 0x70
3000aa50:	f001 f9ee 	bl	3000be30 <__DiagPrintf_veneer>
3000aa54:	991b      	ldr	r1, [sp, #108]	; 0x6c
3000aa56:	e9dd 231c 	ldrd	r2, r3, [sp, #112]	; 0x70
3000aa5a:	e4d5      	b.n	3000a408 <rxi316_perf_tune+0x200>
3000aa5c:	3101      	adds	r1, #1
3000aa5e:	9218      	str	r2, [sp, #96]	; 0x60
3000aa60:	e44e      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa62:	3101      	adds	r1, #1
3000aa64:	4614      	mov	r4, r2
3000aa66:	e44b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa68:	3101      	adds	r1, #1
3000aa6a:	921a      	str	r2, [sp, #104]	; 0x68
3000aa6c:	e448      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa6e:	3101      	adds	r1, #1
3000aa70:	9219      	str	r2, [sp, #100]	; 0x64
3000aa72:	e445      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa74:	9207      	str	r2, [sp, #28]
3000aa76:	e443      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa78:	9208      	str	r2, [sp, #32]
3000aa7a:	e441      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa7c:	9209      	str	r2, [sp, #36]	; 0x24
3000aa7e:	e43f      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa80:	920b      	str	r2, [sp, #44]	; 0x2c
3000aa82:	e43d      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa84:	920a      	str	r2, [sp, #40]	; 0x28
3000aa86:	e43b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa88:	9217      	str	r2, [sp, #92]	; 0x5c
3000aa8a:	e439      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa8c:	9216      	str	r2, [sp, #88]	; 0x58
3000aa8e:	e437      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa90:	920d      	str	r2, [sp, #52]	; 0x34
3000aa92:	e435      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa94:	920c      	str	r2, [sp, #48]	; 0x30
3000aa96:	e433      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa98:	9215      	str	r2, [sp, #84]	; 0x54
3000aa9a:	e431      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aa9c:	9214      	str	r2, [sp, #80]	; 0x50
3000aa9e:	e42f      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aaa0:	9213      	str	r2, [sp, #76]	; 0x4c
3000aaa2:	e42d      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aaa4:	9212      	str	r2, [sp, #72]	; 0x48
3000aaa6:	e42b      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aaa8:	9211      	str	r2, [sp, #68]	; 0x44
3000aaaa:	e429      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aaac:	9210      	str	r2, [sp, #64]	; 0x40
3000aaae:	e427      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aab0:	920f      	str	r2, [sp, #60]	; 0x3c
3000aab2:	e425      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aab4:	920e      	str	r2, [sp, #56]	; 0x38
3000aab6:	e423      	b.n	3000a300 <rxi316_perf_tune+0xf8>
3000aab8:	40000008 	.word	0x40000008
3000aabc:	3000d154 	.word	0x3000d154

3000aac0 <rxi316_dram_init>:
3000aac0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000aac4:	4604      	mov	r4, r0
3000aac6:	b08f      	sub	sp, #60	; 0x3c
3000aac8:	4b10      	ldr	r3, [pc, #64]	; (3000ab0c <rxi316_dram_init+0x4c>)
3000aaca:	4798      	blx	r3
3000aacc:	f894 b014 	ldrb.w	fp, [r4, #20]
3000aad0:	2101      	movs	r1, #1
3000aad2:	900c      	str	r0, [sp, #48]	; 0x30
3000aad4:	fa01 f30b 	lsl.w	r3, r1, fp
3000aad8:	9302      	str	r3, [sp, #8]
3000aada:	6823      	ldr	r3, [r4, #0]
3000aadc:	78da      	ldrb	r2, [r3, #3]
3000aade:	6923      	ldr	r3, [r4, #16]
3000aae0:	fa03 f30b 	lsl.w	r3, r3, fp
3000aae4:	b1a2      	cbz	r2, 3000ab10 <rxi316_dram_init+0x50>
3000aae6:	428a      	cmp	r2, r1
3000aae8:	f000 847c 	beq.w	3000b3e4 <rxi316_dram_init+0x924>
3000aaec:	2a02      	cmp	r2, #2
3000aaee:	bf0b      	itete	eq
3000aaf0:	2206      	moveq	r2, #6
3000aaf2:	220a      	movne	r2, #10
3000aaf4:	2540      	moveq	r5, #64	; 0x40
3000aaf6:	2580      	movne	r5, #128	; 0x80
3000aaf8:	9207      	str	r2, [sp, #28]
3000aafa:	bf0c      	ite	eq
3000aafc:	2204      	moveq	r2, #4
3000aafe:	2208      	movne	r2, #8
3000ab00:	9208      	str	r2, [sp, #32]
3000ab02:	bf0c      	ite	eq
3000ab04:	2208      	moveq	r2, #8
3000ab06:	2210      	movne	r2, #16
3000ab08:	9201      	str	r2, [sp, #4]
3000ab0a:	e007      	b.n	3000ab1c <rxi316_dram_init+0x5c>
3000ab0c:	30007f35 	.word	0x30007f35
3000ab10:	2203      	movs	r2, #3
3000ab12:	25c0      	movs	r5, #192	; 0xc0
3000ab14:	9108      	str	r1, [sp, #32]
3000ab16:	9207      	str	r2, [sp, #28]
3000ab18:	2202      	movs	r2, #2
3000ab1a:	9201      	str	r2, [sp, #4]
3000ab1c:	6860      	ldr	r0, [r4, #4]
3000ab1e:	f04f 31ff 	mov.w	r1, #4294967295
3000ab22:	4ac2      	ldr	r2, [pc, #776]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000ab24:	6806      	ldr	r6, [r0, #0]
3000ab26:	960d      	str	r6, [sp, #52]	; 0x34
3000ab28:	6846      	ldr	r6, [r0, #4]
3000ab2a:	6880      	ldr	r0, [r0, #8]
3000ab2c:	9609      	str	r6, [sp, #36]	; 0x24
3000ab2e:	9003      	str	r0, [sp, #12]
3000ab30:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000ab34:	6820      	ldr	r0, [r4, #0]
3000ab36:	7981      	ldrb	r1, [r0, #6]
3000ab38:	6882      	ldr	r2, [r0, #8]
3000ab3a:	b129      	cbz	r1, 3000ab48 <rxi316_dram_init+0x88>
3000ab3c:	2901      	cmp	r1, #1
3000ab3e:	f000 8449 	beq.w	3000b3d4 <rxi316_dram_init+0x914>
3000ab42:	2903      	cmp	r1, #3
3000ab44:	f000 8456 	beq.w	3000b3f4 <rxi316_dram_init+0x934>
3000ab48:	3a06      	subs	r2, #6
3000ab4a:	7881      	ldrb	r1, [r0, #2]
3000ab4c:	f002 020f 	and.w	r2, r2, #15
3000ab50:	7940      	ldrb	r0, [r0, #5]
3000ab52:	0109      	lsls	r1, r1, #4
3000ab54:	0280      	lsls	r0, r0, #10
3000ab56:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000ab5a:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000ab5e:	4301      	orrs	r1, r0
3000ab60:	430d      	orrs	r5, r1
3000ab62:	432a      	orrs	r2, r5
3000ab64:	4db1      	ldr	r5, [pc, #708]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000ab66:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000ab6a:	68e2      	ldr	r2, [r4, #12]
3000ab6c:	6812      	ldr	r2, [r2, #0]
3000ab6e:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000ab72:	f002 0207 	and.w	r2, r2, #7
3000ab76:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000ab7a:	68e2      	ldr	r2, [r4, #12]
3000ab7c:	6811      	ldr	r1, [r2, #0]
3000ab7e:	6822      	ldr	r2, [r4, #0]
3000ab80:	6848      	ldr	r0, [r1, #4]
3000ab82:	f892 9000 	ldrb.w	r9, [r2]
3000ab86:	688a      	ldr	r2, [r1, #8]
3000ab88:	9004      	str	r0, [sp, #16]
3000ab8a:	0300      	lsls	r0, r0, #12
3000ab8c:	0412      	lsls	r2, r2, #16
3000ab8e:	f009 070f 	and.w	r7, r9, #15
3000ab92:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000ab96:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000ab9a:	4302      	orrs	r2, r0
3000ab9c:	68c8      	ldr	r0, [r1, #12]
3000ab9e:	0440      	lsls	r0, r0, #17
3000aba0:	433a      	orrs	r2, r7
3000aba2:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000aba6:	6908      	ldr	r0, [r1, #16]
3000aba8:	0480      	lsls	r0, r0, #18
3000abaa:	4332      	orrs	r2, r6
3000abac:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000abb0:	4310      	orrs	r0, r2
3000abb2:	694a      	ldr	r2, [r1, #20]
3000abb4:	04d2      	lsls	r2, r2, #19
3000abb6:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000abba:	4310      	orrs	r0, r2
3000abbc:	698a      	ldr	r2, [r1, #24]
3000abbe:	0552      	lsls	r2, r2, #21
3000abc0:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000abc4:	4310      	orrs	r0, r2
3000abc6:	69ca      	ldr	r2, [r1, #28]
3000abc8:	0592      	lsls	r2, r2, #22
3000abca:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000abce:	4310      	orrs	r0, r2
3000abd0:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000abd2:	0612      	lsls	r2, r2, #24
3000abd4:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000abd8:	4302      	orrs	r2, r0
3000abda:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000abde:	06c0      	lsls	r0, r0, #27
3000abe0:	0789      	lsls	r1, r1, #30
3000abe2:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000abe6:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000abea:	4310      	orrs	r0, r2
3000abec:	9a02      	ldr	r2, [sp, #8]
3000abee:	0212      	lsls	r2, r2, #8
3000abf0:	4308      	orrs	r0, r1
3000abf2:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000abf6:	4310      	orrs	r0, r2
3000abf8:	6068      	str	r0, [r5, #4]
3000abfa:	6822      	ldr	r2, [r4, #0]
3000abfc:	7992      	ldrb	r2, [r2, #6]
3000abfe:	2a00      	cmp	r2, #0
3000ac00:	f040 8317 	bne.w	3000b232 <rxi316_dram_init+0x772>
3000ac04:	686a      	ldr	r2, [r5, #4]
3000ac06:	f042 0210 	orr.w	r2, r2, #16
3000ac0a:	606a      	str	r2, [r5, #4]
3000ac0c:	68e2      	ldr	r2, [r4, #12]
3000ac0e:	9802      	ldr	r0, [sp, #8]
3000ac10:	6812      	ldr	r2, [r2, #0]
3000ac12:	f100 3aff 	add.w	sl, r0, #4294967295
3000ac16:	6861      	ldr	r1, [r4, #4]
3000ac18:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000ac1a:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000ac1e:	9005      	str	r0, [sp, #20]
3000ac20:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000ac22:	fa2e f80b 	lsr.w	r8, lr, fp
3000ac26:	6949      	ldr	r1, [r1, #20]
3000ac28:	ea1e 0f0a 	tst.w	lr, sl
3000ac2c:	9006      	str	r0, [sp, #24]
3000ac2e:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000ac30:	fa21 fe0b 	lsr.w	lr, r1, fp
3000ac34:	bf18      	it	ne
3000ac36:	f108 0801 	addne.w	r8, r8, #1
3000ac3a:	ea11 0f0a 	tst.w	r1, sl
3000ac3e:	9906      	ldr	r1, [sp, #24]
3000ac40:	950a      	str	r5, [sp, #40]	; 0x28
3000ac42:	bf18      	it	ne
3000ac44:	f10e 0e01 	addne.w	lr, lr, #1
3000ac48:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000ac4a:	0049      	lsls	r1, r1, #1
3000ac4c:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000ac50:	950b      	str	r5, [sp, #44]	; 0x2c
3000ac52:	f001 0102 	and.w	r1, r1, #2
3000ac56:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000ac5a:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000ac5e:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000ac62:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000ac66:	0080      	lsls	r0, r0, #2
3000ac68:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000ac6c:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000ac70:	9006      	str	r0, [sp, #24]
3000ac72:	013f      	lsls	r7, r7, #4
3000ac74:	9805      	ldr	r0, [sp, #20]
3000ac76:	f00c 0c08 	and.w	ip, ip, #8
3000ac7a:	0176      	lsls	r6, r6, #5
3000ac7c:	f007 0710 	and.w	r7, r7, #16
3000ac80:	f000 0001 	and.w	r0, r0, #1
3000ac84:	f006 0620 	and.w	r6, r6, #32
3000ac88:	4605      	mov	r5, r0
3000ac8a:	9806      	ldr	r0, [sp, #24]
3000ac8c:	f000 0004 	and.w	r0, r0, #4
3000ac90:	4301      	orrs	r1, r0
3000ac92:	4329      	orrs	r1, r5
3000ac94:	ea41 0c0c 	orr.w	ip, r1, ip
3000ac98:	990a      	ldr	r1, [sp, #40]	; 0x28
3000ac9a:	ea4c 0707 	orr.w	r7, ip, r7
3000ac9e:	018d      	lsls	r5, r1, #6
3000aca0:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000aca2:	4337      	orrs	r7, r6
3000aca4:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000aca8:	0208      	lsls	r0, r1, #8
3000acaa:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000acac:	432f      	orrs	r7, r5
3000acae:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000acb2:	06c9      	lsls	r1, r1, #27
3000acb4:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000acb8:	4307      	orrs	r7, r0
3000acba:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000acbe:	0736      	lsls	r6, r6, #28
3000acc0:	430f      	orrs	r7, r1
3000acc2:	0752      	lsls	r2, r2, #29
3000acc4:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000acc8:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000accc:	433e      	orrs	r6, r7
3000acce:	4316      	orrs	r6, r2
3000acd0:	4a56      	ldr	r2, [pc, #344]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000acd2:	ea46 0608 	orr.w	r6, r6, r8
3000acd6:	ea46 060e 	orr.w	r6, r6, lr
3000acda:	6096      	str	r6, [r2, #8]
3000acdc:	6862      	ldr	r2, [r4, #4]
3000acde:	6990      	ldr	r0, [r2, #24]
3000ace0:	9a01      	ldr	r2, [sp, #4]
3000ace2:	2a10      	cmp	r2, #16
3000ace4:	6822      	ldr	r2, [r4, #0]
3000ace6:	7811      	ldrb	r1, [r2, #0]
3000ace8:	7916      	ldrb	r6, [r2, #4]
3000acea:	f000 833d 	beq.w	3000b368 <rxi316_dram_init+0x8a8>
3000acee:	f1a1 0502 	sub.w	r5, r1, #2
3000acf2:	f1a1 0e09 	sub.w	lr, r1, #9
3000acf6:	9f01      	ldr	r7, [sp, #4]
3000acf8:	fab5 f585 	clz	r5, r5
3000acfc:	fabe fe8e 	clz	lr, lr
3000ad00:	2f08      	cmp	r7, #8
3000ad02:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000ad06:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000ad0a:	ea45 020e 	orr.w	r2, r5, lr
3000ad0e:	f000 829e 	beq.w	3000b24e <rxi316_dram_init+0x78e>
3000ad12:	9f01      	ldr	r7, [sp, #4]
3000ad14:	2f04      	cmp	r7, #4
3000ad16:	f000 83a5 	beq.w	3000b464 <rxi316_dram_init+0x9a4>
3000ad1a:	9f01      	ldr	r7, [sp, #4]
3000ad1c:	2f02      	cmp	r7, #2
3000ad1e:	bf08      	it	eq
3000ad20:	4310      	orreq	r0, r2
3000ad22:	2a00      	cmp	r2, #0
3000ad24:	f040 8298 	bne.w	3000b258 <rxi316_dram_init+0x798>
3000ad28:	2903      	cmp	r1, #3
3000ad2a:	f000 8368 	beq.w	3000b3fe <rxi316_dram_init+0x93e>
3000ad2e:	2500      	movs	r5, #0
3000ad30:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000ad34:	462f      	mov	r7, r5
3000ad36:	433d      	orrs	r5, r7
3000ad38:	4f3c      	ldr	r7, [pc, #240]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000ad3a:	430d      	orrs	r5, r1
3000ad3c:	4305      	orrs	r5, r0
3000ad3e:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000ad42:	637e      	str	r6, [r7, #52]	; 0x34
3000ad44:	6862      	ldr	r2, [r4, #4]
3000ad46:	9903      	ldr	r1, [sp, #12]
3000ad48:	69d2      	ldr	r2, [r2, #28]
3000ad4a:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000ad4e:	63ba      	str	r2, [r7, #56]	; 0x38
3000ad50:	6822      	ldr	r2, [r4, #0]
3000ad52:	7810      	ldrb	r0, [r2, #0]
3000ad54:	6862      	ldr	r2, [r4, #4]
3000ad56:	2803      	cmp	r0, #3
3000ad58:	6a11      	ldr	r1, [r2, #32]
3000ad5a:	f000 82f8 	beq.w	3000b34e <rxi316_dram_init+0x88e>
3000ad5e:	2809      	cmp	r0, #9
3000ad60:	f000 830d 	beq.w	3000b37e <rxi316_dram_init+0x8be>
3000ad64:	4831      	ldr	r0, [pc, #196]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000ad66:	63c1      	str	r1, [r0, #60]	; 0x3c
3000ad68:	6862      	ldr	r2, [r4, #4]
3000ad6a:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000ad6c:	6402      	str	r2, [r0, #64]	; 0x40
3000ad6e:	6865      	ldr	r5, [r4, #4]
3000ad70:	68e8      	ldr	r0, [r5, #12]
3000ad72:	682a      	ldr	r2, [r5, #0]
3000ad74:	6869      	ldr	r1, [r5, #4]
3000ad76:	4402      	add	r2, r0
3000ad78:	68ae      	ldr	r6, [r5, #8]
3000ad7a:	4401      	add	r1, r0
3000ad7c:	ea1a 0f02 	tst.w	sl, r2
3000ad80:	f000 82b8 	beq.w	3000b2f4 <rxi316_dram_init+0x834>
3000ad84:	fa22 f20b 	lsr.w	r2, r2, fp
3000ad88:	1c50      	adds	r0, r2, #1
3000ad8a:	ea1a 0f01 	tst.w	sl, r1
3000ad8e:	fa21 f50b 	lsr.w	r5, r1, fp
3000ad92:	6822      	ldr	r2, [r4, #0]
3000ad94:	fa26 f10b 	lsr.w	r1, r6, fp
3000ad98:	bf18      	it	ne
3000ad9a:	3501      	addne	r5, #1
3000ad9c:	ea1a 0f06 	tst.w	sl, r6
3000ada0:	7816      	ldrb	r6, [r2, #0]
3000ada2:	bf18      	it	ne
3000ada4:	3101      	addne	r1, #1
3000ada6:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000adaa:	1eb2      	subs	r2, r6, #2
3000adac:	2a01      	cmp	r2, #1
3000adae:	f240 82a4 	bls.w	3000b2fa <rxi316_dram_init+0x83a>
3000adb2:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000adb6:	fbb7 f2f3 	udiv	r2, r7, r3
3000adba:	fb03 7712 	mls	r7, r3, r2, r7
3000adbe:	b107      	cbz	r7, 3000adc2 <rxi316_dram_init+0x302>
3000adc0:	3201      	adds	r2, #1
3000adc2:	2e09      	cmp	r6, #9
3000adc4:	f040 82b3 	bne.w	3000b32e <rxi316_dram_init+0x86e>
3000adc8:	9e08      	ldr	r6, [sp, #32]
3000adca:	fa26 f60b 	lsr.w	r6, r6, fp
3000adce:	016d      	lsls	r5, r5, #5
3000add0:	f000 001f 	and.w	r0, r0, #31
3000add4:	0289      	lsls	r1, r1, #10
3000add6:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000adda:	0512      	lsls	r2, r2, #20
3000addc:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000ade0:	4305      	orrs	r5, r0
3000ade2:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000ade6:	430d      	orrs	r5, r1
3000ade8:	4910      	ldr	r1, [pc, #64]	; (3000ae2c <rxi316_dram_init+0x36c>)
3000adea:	4315      	orrs	r5, r2
3000adec:	0672      	lsls	r2, r6, #25
3000adee:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000adf2:	432a      	orrs	r2, r5
3000adf4:	630a      	str	r2, [r1, #48]	; 0x30
3000adf6:	68a0      	ldr	r0, [r4, #8]
3000adf8:	6802      	ldr	r2, [r0, #0]
3000adfa:	fbb2 f1f3 	udiv	r1, r2, r3
3000adfe:	fb03 2211 	mls	r2, r3, r1, r2
3000ae02:	2a00      	cmp	r2, #0
3000ae04:	f000 8274 	beq.w	3000b2f0 <rxi316_dram_init+0x830>
3000ae08:	1c4a      	adds	r2, r1, #1
3000ae0a:	9205      	str	r2, [sp, #20]
3000ae0c:	6842      	ldr	r2, [r0, #4]
3000ae0e:	fbb2 f0f3 	udiv	r0, r2, r3
3000ae12:	fb03 2210 	mls	r2, r3, r0, r2
3000ae16:	2a00      	cmp	r2, #0
3000ae18:	f000 8268 	beq.w	3000b2ec <rxi316_dram_init+0x82c>
3000ae1c:	38c7      	subs	r0, #199	; 0xc7
3000ae1e:	68e2      	ldr	r2, [r4, #12]
3000ae20:	6811      	ldr	r1, [r2, #0]
3000ae22:	4a03      	ldr	r2, [pc, #12]	; (3000ae30 <rxi316_dram_init+0x370>)
3000ae24:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000ae28:	e004      	b.n	3000ae34 <rxi316_dram_init+0x374>
3000ae2a:	bf00      	nop
3000ae2c:	41100000 	.word	0x41100000
3000ae30:	00ffff00 	.word	0x00ffff00
3000ae34:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000ae38:	900a      	str	r0, [sp, #40]	; 0x28
3000ae3a:	0712      	lsls	r2, r2, #28
3000ae3c:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000ae40:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000ae44:	430a      	orrs	r2, r1
3000ae46:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000ae4a:	430a      	orrs	r2, r1
3000ae4c:	49c3      	ldr	r1, [pc, #780]	; (3000b15c <rxi316_dram_init+0x69c>)
3000ae4e:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000ae52:	610a      	str	r2, [r1, #16]
3000ae54:	68a2      	ldr	r2, [r4, #8]
3000ae56:	6911      	ldr	r1, [r2, #16]
3000ae58:	fbb1 fcf3 	udiv	ip, r1, r3
3000ae5c:	fb03 111c 	mls	r1, r3, ip, r1
3000ae60:	2900      	cmp	r1, #0
3000ae62:	f000 8240 	beq.w	3000b2e6 <rxi316_dram_init+0x826>
3000ae66:	f10c 0101 	add.w	r1, ip, #1
3000ae6a:	9106      	str	r1, [sp, #24]
3000ae6c:	6991      	ldr	r1, [r2, #24]
3000ae6e:	fbb1 f0f3 	udiv	r0, r1, r3
3000ae72:	fb03 1110 	mls	r1, r3, r0, r1
3000ae76:	b101      	cbz	r1, 3000ae7a <rxi316_dram_init+0x3ba>
3000ae78:	3001      	adds	r0, #1
3000ae7a:	68d1      	ldr	r1, [r2, #12]
3000ae7c:	fbb1 f5f3 	udiv	r5, r1, r3
3000ae80:	fb03 1115 	mls	r1, r3, r5, r1
3000ae84:	9503      	str	r5, [sp, #12]
3000ae86:	b109      	cbz	r1, 3000ae8c <rxi316_dram_init+0x3cc>
3000ae88:	3501      	adds	r5, #1
3000ae8a:	9503      	str	r5, [sp, #12]
3000ae8c:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000ae8e:	fbb5 f1f3 	udiv	r1, r5, r3
3000ae92:	fb03 5511 	mls	r5, r3, r1, r5
3000ae96:	b105      	cbz	r5, 3000ae9a <rxi316_dram_init+0x3da>
3000ae98:	3101      	adds	r1, #1
3000ae9a:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000ae9c:	0349      	lsls	r1, r1, #13
3000ae9e:	68e2      	ldr	r2, [r4, #12]
3000aea0:	fa25 f60b 	lsr.w	r6, r5, fp
3000aea4:	ea15 0f0a 	tst.w	r5, sl
3000aea8:	6812      	ldr	r2, [r2, #0]
3000aeaa:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000aeae:	bf18      	it	ne
3000aeb0:	3601      	addne	r6, #1
3000aeb2:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000aeb4:	05d2      	lsls	r2, r2, #23
3000aeb6:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000aeba:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000aebe:	9a06      	ldr	r2, [sp, #24]
3000aec0:	0112      	lsls	r2, r2, #4
3000aec2:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000aec6:	4332      	orrs	r2, r6
3000aec8:	0246      	lsls	r6, r0, #9
3000aeca:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000aece:	4316      	orrs	r6, r2
3000aed0:	9a03      	ldr	r2, [sp, #12]
3000aed2:	f002 020f 	and.w	r2, r2, #15
3000aed6:	4316      	orrs	r6, r2
3000aed8:	4aa0      	ldr	r2, [pc, #640]	; (3000b15c <rxi316_dram_init+0x69c>)
3000aeda:	4331      	orrs	r1, r6
3000aedc:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000aee0:	6151      	str	r1, [r2, #20]
3000aee2:	68a7      	ldr	r7, [r4, #8]
3000aee4:	697a      	ldr	r2, [r7, #20]
3000aee6:	fbb2 fef3 	udiv	lr, r2, r3
3000aeea:	fb03 221e 	mls	r2, r3, lr, r2
3000aeee:	b10a      	cbz	r2, 3000aef4 <rxi316_dram_init+0x434>
3000aef0:	f10e 0e01 	add.w	lr, lr, #1
3000aef4:	6939      	ldr	r1, [r7, #16]
3000aef6:	fbb1 f2f3 	udiv	r2, r1, r3
3000aefa:	fb03 1112 	mls	r1, r3, r2, r1
3000aefe:	b101      	cbz	r1, 3000af02 <rxi316_dram_init+0x442>
3000af00:	3201      	adds	r2, #1
3000af02:	68fd      	ldr	r5, [r7, #12]
3000af04:	fbb5 f1f3 	udiv	r1, r5, r3
3000af08:	fb03 5511 	mls	r5, r3, r1, r5
3000af0c:	b105      	cbz	r5, 3000af10 <rxi316_dram_init+0x450>
3000af0e:	3101      	adds	r1, #1
3000af10:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000af12:	fbb5 f6f3 	udiv	r6, r5, r3
3000af16:	fb03 5516 	mls	r5, r3, r6, r5
3000af1a:	b105      	cbz	r5, 3000af1e <rxi316_dram_init+0x45e>
3000af1c:	3601      	adds	r6, #1
3000af1e:	68bd      	ldr	r5, [r7, #8]
3000af20:	4411      	add	r1, r2
3000af22:	fbb5 f2f3 	udiv	r2, r5, r3
3000af26:	42b1      	cmp	r1, r6
3000af28:	fb03 5512 	mls	r5, r3, r2, r5
3000af2c:	bf38      	it	cc
3000af2e:	4631      	movcc	r1, r6
3000af30:	b905      	cbnz	r5, 3000af34 <rxi316_dram_init+0x474>
3000af32:	3a01      	subs	r2, #1
3000af34:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000af36:	ea15 0f0a 	tst.w	r5, sl
3000af3a:	fa25 fc0b 	lsr.w	ip, r5, fp
3000af3e:	69fd      	ldr	r5, [r7, #28]
3000af40:	bf18      	it	ne
3000af42:	f10c 0c01 	addne.w	ip, ip, #1
3000af46:	fa25 f60b 	lsr.w	r6, r5, fp
3000af4a:	ea15 0f0a 	tst.w	r5, sl
3000af4e:	bf18      	it	ne
3000af50:	3601      	addne	r6, #1
3000af52:	f1b9 0f09 	cmp.w	r9, #9
3000af56:	f000 81f5 	beq.w	3000b344 <rxi316_dram_init+0x884>
3000af5a:	f1b9 0f02 	cmp.w	r9, #2
3000af5e:	f000 8233 	beq.w	3000b3c8 <rxi316_dram_init+0x908>
3000af62:	f1b9 0f03 	cmp.w	r9, #3
3000af66:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000af68:	f000 8298 	beq.w	3000b49c <rxi316_dram_init+0x9dc>
3000af6c:	46a8      	mov	r8, r5
3000af6e:	9d01      	ldr	r5, [sp, #4]
3000af70:	44a8      	add	r8, r5
3000af72:	fa28 f50b 	lsr.w	r5, r8, fp
3000af76:	ea18 0f0a 	tst.w	r8, sl
3000af7a:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000af7e:	bf18      	it	ne
3000af80:	3501      	addne	r5, #1
3000af82:	fbb8 f7f3 	udiv	r7, r8, r3
3000af86:	fb03 8817 	mls	r8, r3, r7, r8
3000af8a:	f1b8 0f00 	cmp.w	r8, #0
3000af8e:	d000      	beq.n	3000af92 <rxi316_dram_init+0x4d2>
3000af90:	3701      	adds	r7, #1
3000af92:	0109      	lsls	r1, r1, #4
3000af94:	f00e 0e0f 	and.w	lr, lr, #15
3000af98:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000af9c:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000afa0:	063f      	lsls	r7, r7, #24
3000afa2:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000afa6:	ea41 0e0e 	orr.w	lr, r1, lr
3000afaa:	0291      	lsls	r1, r2, #10
3000afac:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000afb0:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000afb4:	ea4e 0e01 	orr.w	lr, lr, r1
3000afb8:	0471      	lsls	r1, r6, #17
3000afba:	ea4e 0c0c 	orr.w	ip, lr, ip
3000afbe:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000afc2:	ea4c 0c01 	orr.w	ip, ip, r1
3000afc6:	0529      	lsls	r1, r5, #20
3000afc8:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000afcc:	ea4c 0c01 	orr.w	ip, ip, r1
3000afd0:	4962      	ldr	r1, [pc, #392]	; (3000b15c <rxi316_dram_init+0x69c>)
3000afd2:	ea4c 0707 	orr.w	r7, ip, r7
3000afd6:	618f      	str	r7, [r1, #24]
3000afd8:	6821      	ldr	r1, [r4, #0]
3000afda:	780f      	ldrb	r7, [r1, #0]
3000afdc:	2f02      	cmp	r7, #2
3000afde:	f000 8152 	beq.w	3000b286 <rxi316_dram_init+0x7c6>
3000afe2:	2f09      	cmp	r7, #9
3000afe4:	f000 814f 	beq.w	3000b286 <rxi316_dram_init+0x7c6>
3000afe8:	68e1      	ldr	r1, [r4, #12]
3000afea:	2f03      	cmp	r7, #3
3000afec:	6809      	ldr	r1, [r1, #0]
3000afee:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000aff0:	f001 080f 	and.w	r8, r1, #15
3000aff4:	68a1      	ldr	r1, [r4, #8]
3000aff6:	6a09      	ldr	r1, [r1, #32]
3000aff8:	d005      	beq.n	3000b006 <rxi316_dram_init+0x546>
3000affa:	ea0a 0701 	and.w	r7, sl, r1
3000affe:	fa21 f10b 	lsr.w	r1, r1, fp
3000b002:	b107      	cbz	r7, 3000b006 <rxi316_dram_init+0x546>
3000b004:	3101      	adds	r1, #1
3000b006:	f1b9 0f02 	cmp.w	r9, #2
3000b00a:	f000 8154 	beq.w	3000b2b6 <rxi316_dram_init+0x7f6>
3000b00e:	f1b9 0f03 	cmp.w	r9, #3
3000b012:	f000 81b6 	beq.w	3000b382 <rxi316_dram_init+0x8c2>
3000b016:	f1b9 0f09 	cmp.w	r9, #9
3000b01a:	f000 822f 	beq.w	3000b47c <rxi316_dram_init+0x9bc>
3000b01e:	f04f 0e00 	mov.w	lr, #0
3000b022:	46f1      	mov	r9, lr
3000b024:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b028:	0109      	lsls	r1, r1, #4
3000b02a:	9f06      	ldr	r7, [sp, #24]
3000b02c:	b2c9      	uxtb	r1, r1
3000b02e:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b032:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b034:	ea41 0108 	orr.w	r1, r1, r8
3000b038:	4339      	orrs	r1, r7
3000b03a:	4f48      	ldr	r7, [pc, #288]	; (3000b15c <rxi316_dram_init+0x69c>)
3000b03c:	ea41 0109 	orr.w	r1, r1, r9
3000b040:	ea41 010e 	orr.w	r1, r1, lr
3000b044:	61f9      	str	r1, [r7, #28]
3000b046:	68a7      	ldr	r7, [r4, #8]
3000b048:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b04a:	fbb4 f1f3 	udiv	r1, r4, r3
3000b04e:	fb03 4311 	mls	r3, r3, r1, r4
3000b052:	b103      	cbz	r3, 3000b056 <rxi316_dram_init+0x596>
3000b054:	3101      	adds	r1, #1
3000b056:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b058:	094b      	lsrs	r3, r1, #5
3000b05a:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b05e:	0912      	lsrs	r2, r2, #4
3000b060:	ea14 0f0a 	tst.w	r4, sl
3000b064:	fa24 f80b 	lsr.w	r8, r4, fp
3000b068:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b06a:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b06e:	bf18      	it	ne
3000b070:	f108 0801 	addne.w	r8, r8, #1
3000b074:	9306      	str	r3, [sp, #24]
3000b076:	ea14 0f0a 	tst.w	r4, sl
3000b07a:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b07e:	9b01      	ldr	r3, [sp, #4]
3000b080:	fa29 f40b 	lsr.w	r4, r9, fp
3000b084:	bf18      	it	ne
3000b086:	f10e 0e01 	addne.w	lr, lr, #1
3000b08a:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b08c:	ea19 0f0a 	tst.w	r9, sl
3000b090:	fa23 f90b 	lsr.w	r9, r3, fp
3000b094:	9905      	ldr	r1, [sp, #20]
3000b096:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b09a:	bf18      	it	ne
3000b09c:	3401      	addne	r4, #1
3000b09e:	45b9      	cmp	r9, r7
3000b0a0:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b0a4:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b0a8:	f240 80eb 	bls.w	3000b282 <rxi316_dram_init+0x7c2>
3000b0ac:	eba9 0707 	sub.w	r7, r9, r7
3000b0b0:	fa27 f40b 	lsr.w	r4, r7, fp
3000b0b4:	ea17 070a 	ands.w	r7, r7, sl
3000b0b8:	d001      	beq.n	3000b0be <rxi316_dram_init+0x5fe>
3000b0ba:	3401      	adds	r4, #1
3000b0bc:	2700      	movs	r7, #0
3000b0be:	9b04      	ldr	r3, [sp, #16]
3000b0c0:	b10b      	cbz	r3, 3000b0c6 <rxi316_dram_init+0x606>
3000b0c2:	073b      	lsls	r3, r7, #28
3000b0c4:	9304      	str	r3, [sp, #16]
3000b0c6:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b0ca:	9b06      	ldr	r3, [sp, #24]
3000b0cc:	0289      	lsls	r1, r1, #10
3000b0ce:	02c0      	lsls	r0, r0, #11
3000b0d0:	f00c 0c04 	and.w	ip, ip, #4
3000b0d4:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b0d8:	01d2      	lsls	r2, r2, #7
3000b0da:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b0de:	0236      	lsls	r6, r6, #8
3000b0e0:	ea4c 0c01 	orr.w	ip, ip, r1
3000b0e4:	b2d2      	uxtb	r2, r2
3000b0e6:	026d      	lsls	r5, r5, #9
3000b0e8:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b0ec:	ea4c 0c00 	orr.w	ip, ip, r0
3000b0f0:	00df      	lsls	r7, r3, #3
3000b0f2:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b0f6:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b0fa:	ea4c 0202 	orr.w	r2, ip, r2
3000b0fe:	f007 0708 	and.w	r7, r7, #8
3000b102:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b106:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b10a:	4332      	orrs	r2, r6
3000b10c:	0524      	lsls	r4, r4, #20
3000b10e:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b112:	4912      	ldr	r1, [pc, #72]	; (3000b15c <rxi316_dram_init+0x69c>)
3000b114:	ea42 0305 	orr.w	r3, r2, r5
3000b118:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b11c:	9a04      	ldr	r2, [sp, #16]
3000b11e:	433b      	orrs	r3, r7
3000b120:	ea43 0308 	orr.w	r3, r3, r8
3000b124:	ea43 030e 	orr.w	r3, r3, lr
3000b128:	4323      	orrs	r3, r4
3000b12a:	4313      	orrs	r3, r2
3000b12c:	9a03      	ldr	r2, [sp, #12]
3000b12e:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b132:	620b      	str	r3, [r1, #32]
3000b134:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b138:	9b02      	ldr	r3, [sp, #8]
3000b13a:	2b01      	cmp	r3, #1
3000b13c:	d002      	beq.n	3000b144 <rxi316_dram_init+0x684>
3000b13e:	9b08      	ldr	r3, [sp, #32]
3000b140:	3303      	adds	r3, #3
3000b142:	9307      	str	r3, [sp, #28]
3000b144:	4b05      	ldr	r3, [pc, #20]	; (3000b15c <rxi316_dram_init+0x69c>)
3000b146:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b14a:	9a07      	ldr	r2, [sp, #28]
3000b14c:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b150:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b154:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b158:	e002      	b.n	3000b160 <rxi316_dram_init+0x6a0>
3000b15a:	bf00      	nop
3000b15c:	41100000 	.word	0x41100000
3000b160:	60da      	str	r2, [r3, #12]
3000b162:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b166:	430a      	orrs	r2, r1
3000b168:	49b3      	ldr	r1, [pc, #716]	; (3000b438 <rxi316_dram_init+0x978>)
3000b16a:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b16e:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b172:	0096      	lsls	r6, r2, #2
3000b174:	d57f      	bpl.n	3000b276 <rxi316_dram_init+0x7b6>
3000b176:	691a      	ldr	r2, [r3, #16]
3000b178:	4cb0      	ldr	r4, [pc, #704]	; (3000b43c <rxi316_dram_init+0x97c>)
3000b17a:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b17e:	611a      	str	r2, [r3, #16]
3000b180:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b184:	601a      	str	r2, [r3, #0]
3000b186:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b18a:	f043 0303 	orr.w	r3, r3, #3
3000b18e:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b192:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b196:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b198:	2b01      	cmp	r3, #1
3000b19a:	4ba9      	ldr	r3, [pc, #676]	; (3000b440 <rxi316_dram_init+0x980>)
3000b19c:	bf0c      	ite	eq
3000b19e:	4aa9      	ldreq	r2, [pc, #676]	; (3000b444 <rxi316_dram_init+0x984>)
3000b1a0:	4aa9      	ldrne	r2, [pc, #676]	; (3000b448 <rxi316_dram_init+0x988>)
3000b1a2:	601a      	str	r2, [r3, #0]
3000b1a4:	2203      	movs	r2, #3
3000b1a6:	68e0      	ldr	r0, [r4, #12]
3000b1a8:	4ba8      	ldr	r3, [pc, #672]	; (3000b44c <rxi316_dram_init+0x98c>)
3000b1aa:	4003      	ands	r3, r0
3000b1ac:	60e3      	str	r3, [r4, #12]
3000b1ae:	2308      	movs	r3, #8
3000b1b0:	62a1      	str	r1, [r4, #40]	; 0x28
3000b1b2:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b1b4:	6023      	str	r3, [r4, #0]
3000b1b6:	6823      	ldr	r3, [r4, #0]
3000b1b8:	071d      	lsls	r5, r3, #28
3000b1ba:	d5fc      	bpl.n	3000b1b6 <rxi316_dram_init+0x6f6>
3000b1bc:	4ba4      	ldr	r3, [pc, #656]	; (3000b450 <rxi316_dram_init+0x990>)
3000b1be:	2001      	movs	r0, #1
3000b1c0:	4798      	blx	r3
3000b1c2:	68e2      	ldr	r2, [r4, #12]
3000b1c4:	4ba1      	ldr	r3, [pc, #644]	; (3000b44c <rxi316_dram_init+0x98c>)
3000b1c6:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b1ca:	4013      	ands	r3, r2
3000b1cc:	2203      	movs	r2, #3
3000b1ce:	60e3      	str	r3, [r4, #12]
3000b1d0:	2308      	movs	r3, #8
3000b1d2:	62a1      	str	r1, [r4, #40]	; 0x28
3000b1d4:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b1d6:	4a99      	ldr	r2, [pc, #612]	; (3000b43c <rxi316_dram_init+0x97c>)
3000b1d8:	6023      	str	r3, [r4, #0]
3000b1da:	6813      	ldr	r3, [r2, #0]
3000b1dc:	071c      	lsls	r4, r3, #28
3000b1de:	d5fc      	bpl.n	3000b1da <rxi316_dram_init+0x71a>
3000b1e0:	68d0      	ldr	r0, [r2, #12]
3000b1e2:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b1e6:	4b99      	ldr	r3, [pc, #612]	; (3000b44c <rxi316_dram_init+0x98c>)
3000b1e8:	4003      	ands	r3, r0
3000b1ea:	2008      	movs	r0, #8
3000b1ec:	60d3      	str	r3, [r2, #12]
3000b1ee:	6291      	str	r1, [r2, #40]	; 0x28
3000b1f0:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b1f2:	4992      	ldr	r1, [pc, #584]	; (3000b43c <rxi316_dram_init+0x97c>)
3000b1f4:	f023 0302 	bic.w	r3, r3, #2
3000b1f8:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b1fa:	6010      	str	r0, [r2, #0]
3000b1fc:	680b      	ldr	r3, [r1, #0]
3000b1fe:	0718      	lsls	r0, r3, #28
3000b200:	d5fc      	bpl.n	3000b1fc <rxi316_dram_init+0x73c>
3000b202:	4a8e      	ldr	r2, [pc, #568]	; (3000b43c <rxi316_dram_init+0x97c>)
3000b204:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b208:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b20c:	60d3      	str	r3, [r2, #12]
3000b20e:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b210:	6913      	ldr	r3, [r2, #16]
3000b212:	4318      	orrs	r0, r3
3000b214:	6110      	str	r0, [r2, #16]
3000b216:	6913      	ldr	r3, [r2, #16]
3000b218:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b21c:	6113      	str	r3, [r2, #16]
3000b21e:	6011      	str	r1, [r2, #0]
3000b220:	68d3      	ldr	r3, [r2, #12]
3000b222:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b226:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b22a:	d1f9      	bne.n	3000b220 <rxi316_dram_init+0x760>
3000b22c:	b00f      	add	sp, #60	; 0x3c
3000b22e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b232:	2a01      	cmp	r2, #1
3000b234:	f000 80d1 	beq.w	3000b3da <rxi316_dram_init+0x91a>
3000b238:	2a02      	cmp	r2, #2
3000b23a:	f000 8139 	beq.w	3000b4b0 <rxi316_dram_init+0x9f0>
3000b23e:	2a03      	cmp	r2, #3
3000b240:	f47f ace4 	bne.w	3000ac0c <rxi316_dram_init+0x14c>
3000b244:	686a      	ldr	r2, [r5, #4]
3000b246:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b24a:	606a      	str	r2, [r5, #4]
3000b24c:	e4de      	b.n	3000ac0c <rxi316_dram_init+0x14c>
3000b24e:	2a00      	cmp	r2, #0
3000b250:	f43f ad6a 	beq.w	3000ad28 <rxi316_dram_init+0x268>
3000b254:	2203      	movs	r2, #3
3000b256:	4310      	orrs	r0, r2
3000b258:	68a2      	ldr	r2, [r4, #8]
3000b25a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b25c:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b25e:	013f      	lsls	r7, r7, #4
3000b260:	2d00      	cmp	r5, #0
3000b262:	f000 80d7 	beq.w	3000b414 <rxi316_dram_init+0x954>
3000b266:	1e91      	subs	r1, r2, #2
3000b268:	2906      	cmp	r1, #6
3000b26a:	f200 80b0 	bhi.w	3000b3ce <rxi316_dram_init+0x90e>
3000b26e:	1e55      	subs	r5, r2, #1
3000b270:	2100      	movs	r1, #0
3000b272:	026d      	lsls	r5, r5, #9
3000b274:	e55f      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b276:	2201      	movs	r2, #1
3000b278:	601a      	str	r2, [r3, #0]
3000b27a:	681a      	ldr	r2, [r3, #0]
3000b27c:	07d2      	lsls	r2, r2, #31
3000b27e:	d5fc      	bpl.n	3000b27a <rxi316_dram_init+0x7ba>
3000b280:	e7bf      	b.n	3000b202 <rxi316_dram_init+0x742>
3000b282:	2701      	movs	r7, #1
3000b284:	e71b      	b.n	3000b0be <rxi316_dram_init+0x5fe>
3000b286:	68a1      	ldr	r1, [r4, #8]
3000b288:	2f03      	cmp	r7, #3
3000b28a:	6a09      	ldr	r1, [r1, #32]
3000b28c:	f000 80bf 	beq.w	3000b40e <rxi316_dram_init+0x94e>
3000b290:	2f02      	cmp	r7, #2
3000b292:	f040 8118 	bne.w	3000b4c6 <rxi316_dram_init+0xa06>
3000b296:	fbb1 fcf3 	udiv	ip, r1, r3
3000b29a:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b29e:	4661      	mov	r1, ip
3000b2a0:	f1be 0f00 	cmp.w	lr, #0
3000b2a4:	f000 80a9 	beq.w	3000b3fa <rxi316_dram_init+0x93a>
3000b2a8:	f1b9 0f02 	cmp.w	r9, #2
3000b2ac:	f101 0101 	add.w	r1, r1, #1
3000b2b0:	46b8      	mov	r8, r7
3000b2b2:	f47f aeac 	bne.w	3000b00e <rxi316_dram_init+0x54e>
3000b2b6:	4f67      	ldr	r7, [pc, #412]	; (3000b454 <rxi316_dram_init+0x994>)
3000b2b8:	f04f 0e00 	mov.w	lr, #0
3000b2bc:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b458 <rxi316_dram_init+0x998>
3000b2c0:	fbb7 f7f3 	udiv	r7, r7, r3
3000b2c4:	fbb9 f9f3 	udiv	r9, r9, r3
3000b2c8:	09ff      	lsrs	r7, r7, #7
3000b2ca:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b2ce:	3701      	adds	r7, #1
3000b2d0:	f109 0c01 	add.w	ip, r9, #1
3000b2d4:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b45c <rxi316_dram_init+0x99c>
3000b2d8:	023f      	lsls	r7, r7, #8
3000b2da:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b2de:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b2e2:	9709      	str	r7, [sp, #36]	; 0x24
3000b2e4:	e6a0      	b.n	3000b028 <rxi316_dram_init+0x568>
3000b2e6:	f8cd c018 	str.w	ip, [sp, #24]
3000b2ea:	e5bf      	b.n	3000ae6c <rxi316_dram_init+0x3ac>
3000b2ec:	38c8      	subs	r0, #200	; 0xc8
3000b2ee:	e596      	b.n	3000ae1e <rxi316_dram_init+0x35e>
3000b2f0:	9105      	str	r1, [sp, #20]
3000b2f2:	e58b      	b.n	3000ae0c <rxi316_dram_init+0x34c>
3000b2f4:	fa22 f00b 	lsr.w	r0, r2, fp
3000b2f8:	e547      	b.n	3000ad8a <rxi316_dram_init+0x2ca>
3000b2fa:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b2fe:	ea1a 0f07 	tst.w	sl, r7
3000b302:	fa27 f20b 	lsr.w	r2, r7, fp
3000b306:	d000      	beq.n	3000b30a <rxi316_dram_init+0x84a>
3000b308:	3201      	adds	r2, #1
3000b30a:	2e02      	cmp	r6, #2
3000b30c:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b310:	d10f      	bne.n	3000b332 <rxi316_dram_init+0x872>
3000b312:	9e08      	ldr	r6, [sp, #32]
3000b314:	3e02      	subs	r6, #2
3000b316:	fa26 f60b 	lsr.w	r6, r6, fp
3000b31a:	4437      	add	r7, r6
3000b31c:	fbb7 f6f3 	udiv	r6, r7, r3
3000b320:	fb03 7716 	mls	r7, r3, r6, r7
3000b324:	2f00      	cmp	r7, #0
3000b326:	f43f ad52 	beq.w	3000adce <rxi316_dram_init+0x30e>
3000b32a:	3601      	adds	r6, #1
3000b32c:	e54f      	b.n	3000adce <rxi316_dram_init+0x30e>
3000b32e:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b332:	fbb7 f6f3 	udiv	r6, r7, r3
3000b336:	fb03 7716 	mls	r7, r3, r6, r7
3000b33a:	2f00      	cmp	r7, #0
3000b33c:	f43f ad47 	beq.w	3000adce <rxi316_dram_init+0x30e>
3000b340:	3601      	adds	r6, #1
3000b342:	e544      	b.n	3000adce <rxi316_dram_init+0x30e>
3000b344:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b346:	46a8      	mov	r8, r5
3000b348:	9d08      	ldr	r5, [sp, #32]
3000b34a:	44a8      	add	r8, r5
3000b34c:	e611      	b.n	3000af72 <rxi316_dram_init+0x4b2>
3000b34e:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b350:	1f50      	subs	r0, r2, #5
3000b352:	00c0      	lsls	r0, r0, #3
3000b354:	68e2      	ldr	r2, [r4, #12]
3000b356:	6812      	ldr	r2, [r2, #0]
3000b358:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b35a:	fab2 f282 	clz	r2, r2
3000b35e:	0952      	lsrs	r2, r2, #5
3000b360:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b364:	4301      	orrs	r1, r0
3000b366:	e4fd      	b.n	3000ad64 <rxi316_dram_init+0x2a4>
3000b368:	f1a1 0502 	sub.w	r5, r1, #2
3000b36c:	2909      	cmp	r1, #9
3000b36e:	fab5 f585 	clz	r5, r5
3000b372:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b376:	f000 8099 	beq.w	3000b4ac <rxi316_dram_init+0x9ec>
3000b37a:	462a      	mov	r2, r5
3000b37c:	e4d1      	b.n	3000ad22 <rxi316_dram_init+0x262>
3000b37e:	2000      	movs	r0, #0
3000b380:	e7e8      	b.n	3000b354 <rxi316_dram_init+0x894>
3000b382:	f242 7710 	movw	r7, #10000	; 0x2710
3000b386:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b460 <rxi316_dram_init+0x9a0>
3000b38a:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b458 <rxi316_dram_init+0x998>
3000b38e:	fbb7 fcf3 	udiv	ip, r7, r3
3000b392:	9f05      	ldr	r7, [sp, #20]
3000b394:	4467      	add	r7, ip
3000b396:	fbb9 f9f3 	udiv	r9, r9, r3
3000b39a:	fbbe fef3 	udiv	lr, lr, r3
3000b39e:	09ff      	lsrs	r7, r7, #7
3000b3a0:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b3a4:	3701      	adds	r7, #1
3000b3a6:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b3aa:	f109 0c01 	add.w	ip, r9, #1
3000b3ae:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b45c <rxi316_dram_init+0x99c>
3000b3b2:	023f      	lsls	r7, r7, #8
3000b3b4:	f10e 0e01 	add.w	lr, lr, #1
3000b3b8:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b3bc:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b3c0:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b3c4:	9709      	str	r7, [sp, #36]	; 0x24
3000b3c6:	e62f      	b.n	3000b028 <rxi316_dram_init+0x568>
3000b3c8:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b3cc:	e5d1      	b.n	3000af72 <rxi316_dram_init+0x4b2>
3000b3ce:	2100      	movs	r1, #0
3000b3d0:	460d      	mov	r5, r1
3000b3d2:	e4b0      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b3d4:	3a07      	subs	r2, #7
3000b3d6:	f7ff bbb8 	b.w	3000ab4a <rxi316_dram_init+0x8a>
3000b3da:	686a      	ldr	r2, [r5, #4]
3000b3dc:	f042 0220 	orr.w	r2, r2, #32
3000b3e0:	606a      	str	r2, [r5, #4]
3000b3e2:	e413      	b.n	3000ac0c <rxi316_dram_init+0x14c>
3000b3e4:	2204      	movs	r2, #4
3000b3e6:	2102      	movs	r1, #2
3000b3e8:	2500      	movs	r5, #0
3000b3ea:	9201      	str	r2, [sp, #4]
3000b3ec:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b3f0:	f7ff bb94 	b.w	3000ab1c <rxi316_dram_init+0x5c>
3000b3f4:	3a08      	subs	r2, #8
3000b3f6:	f7ff bba8 	b.w	3000ab4a <rxi316_dram_init+0x8a>
3000b3fa:	46b8      	mov	r8, r7
3000b3fc:	e603      	b.n	3000b006 <rxi316_dram_init+0x546>
3000b3fe:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b400:	2a04      	cmp	r2, #4
3000b402:	d90c      	bls.n	3000b41e <rxi316_dram_init+0x95e>
3000b404:	2a0b      	cmp	r2, #11
3000b406:	d86a      	bhi.n	3000b4de <rxi316_dram_init+0xa1e>
3000b408:	1f17      	subs	r7, r2, #4
3000b40a:	013f      	lsls	r7, r7, #4
3000b40c:	e008      	b.n	3000b420 <rxi316_dram_init+0x960>
3000b40e:	f04f 0802 	mov.w	r8, #2
3000b412:	e5f8      	b.n	3000b006 <rxi316_dram_init+0x546>
3000b414:	2903      	cmp	r1, #3
3000b416:	d005      	beq.n	3000b424 <rxi316_dram_init+0x964>
3000b418:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b41c:	e48b      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b41e:	2710      	movs	r7, #16
3000b420:	68a2      	ldr	r2, [r4, #8]
3000b422:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b424:	2a04      	cmp	r2, #4
3000b426:	d949      	bls.n	3000b4bc <rxi316_dram_init+0x9fc>
3000b428:	1f51      	subs	r1, r2, #5
3000b42a:	2903      	cmp	r1, #3
3000b42c:	d84e      	bhi.n	3000b4cc <rxi316_dram_init+0xa0c>
3000b42e:	1f15      	subs	r5, r2, #4
3000b430:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b434:	026d      	lsls	r5, r5, #9
3000b436:	e47e      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b438:	42008000 	.word	0x42008000
3000b43c:	41100000 	.word	0x41100000
3000b440:	41011000 	.word	0x41011000
3000b444:	44000103 	.word	0x44000103
3000b448:	44000021 	.word	0x44000021
3000b44c:	fff8fffc 	.word	0xfff8fffc
3000b450:	00009b2d 	.word	0x00009b2d
3000b454:	00061a80 	.word	0x00061a80
3000b458:	0bebc200 	.word	0x0bebc200
3000b45c:	003ff000 	.word	0x003ff000
3000b460:	1dcd6500 	.word	0x1dcd6500
3000b464:	f1a1 0c02 	sub.w	ip, r1, #2
3000b468:	f1bc 0f01 	cmp.w	ip, #1
3000b46c:	d903      	bls.n	3000b476 <rxi316_dram_init+0x9b6>
3000b46e:	f1be 0f00 	cmp.w	lr, #0
3000b472:	f43f ac56 	beq.w	3000ad22 <rxi316_dram_init+0x262>
3000b476:	f040 0002 	orr.w	r0, r0, #2
3000b47a:	e452      	b.n	3000ad22 <rxi316_dram_init+0x262>
3000b47c:	4f1d      	ldr	r7, [pc, #116]	; (3000b4f4 <rxi316_dram_init+0xa34>)
3000b47e:	f04f 0e00 	mov.w	lr, #0
3000b482:	fbb7 f9f3 	udiv	r9, r7, r3
3000b486:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b48a:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b48e:	f109 0701 	add.w	r7, r9, #1
3000b492:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b4f8 <rxi316_dram_init+0xa38>
3000b496:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b49a:	e5c5      	b.n	3000b028 <rxi316_dram_init+0x568>
3000b49c:	f105 0802 	add.w	r8, r5, #2
3000b4a0:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b4a2:	eba8 0805 	sub.w	r8, r8, r5
3000b4a6:	9d08      	ldr	r5, [sp, #32]
3000b4a8:	44a8      	add	r8, r5
3000b4aa:	e562      	b.n	3000af72 <rxi316_dram_init+0x4b2>
3000b4ac:	2204      	movs	r2, #4
3000b4ae:	e6d2      	b.n	3000b256 <rxi316_dram_init+0x796>
3000b4b0:	686a      	ldr	r2, [r5, #4]
3000b4b2:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b4b6:	606a      	str	r2, [r5, #4]
3000b4b8:	f7ff bba8 	b.w	3000ac0c <rxi316_dram_init+0x14c>
3000b4bc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4c0:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b4c4:	e437      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b4c6:	f04f 0802 	mov.w	r8, #2
3000b4ca:	e596      	b.n	3000affa <rxi316_dram_init+0x53a>
3000b4cc:	f1a2 0109 	sub.w	r1, r2, #9
3000b4d0:	2905      	cmp	r1, #5
3000b4d2:	d80b      	bhi.n	3000b4ec <rxi316_dram_init+0xa2c>
3000b4d4:	0855      	lsrs	r5, r2, #1
3000b4d6:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4da:	026d      	lsls	r5, r5, #9
3000b4dc:	e42b      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b4de:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b4e0:	f040 0004 	orr.w	r0, r0, #4
3000b4e4:	f1a2 070c 	sub.w	r7, r2, #12
3000b4e8:	013f      	lsls	r7, r7, #4
3000b4ea:	e799      	b.n	3000b420 <rxi316_dram_init+0x960>
3000b4ec:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4f0:	2500      	movs	r5, #0
3000b4f2:	e420      	b.n	3000ad36 <rxi316_dram_init+0x276>
3000b4f4:	0bebc200 	.word	0x0bebc200
3000b4f8:	003ff000 	.word	0x003ff000

3000b4fc <rxi316_ftend_init>:
3000b4fc:	2300      	movs	r3, #0
3000b4fe:	b510      	push	{r4, lr}
3000b500:	4c0c      	ldr	r4, [pc, #48]	; (3000b534 <rxi316_ftend_init+0x38>)
3000b502:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b506:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b50a:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b50e:	f7fe fe51 	bl	3000a1b4 <ddr_init_index>
3000b512:	4a09      	ldr	r2, [pc, #36]	; (3000b538 <rxi316_ftend_init+0x3c>)
3000b514:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b518:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b51c:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b520:	68cb      	ldr	r3, [r1, #12]
3000b522:	689b      	ldr	r3, [r3, #8]
3000b524:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b526:	3301      	adds	r3, #1
3000b528:	b2db      	uxtb	r3, r3
3000b52a:	4313      	orrs	r3, r2
3000b52c:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b530:	bd10      	pop	{r4, pc}
3000b532:	bf00      	nop
3000b534:	41101000 	.word	0x41101000
3000b538:	3000e3b4 	.word	0x3000e3b4

3000b53c <rxi316_DynSre_init>:
3000b53c:	2901      	cmp	r1, #1
3000b53e:	d013      	beq.n	3000b568 <rxi316_DynSre_init+0x2c>
3000b540:	4a12      	ldr	r2, [pc, #72]	; (3000b58c <rxi316_DynSre_init+0x50>)
3000b542:	6893      	ldr	r3, [r2, #8]
3000b544:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b548:	6093      	str	r3, [r2, #8]
3000b54a:	4b10      	ldr	r3, [pc, #64]	; (3000b58c <rxi316_DynSre_init+0x50>)
3000b54c:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b550:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b554:	625a      	str	r2, [r3, #36]	; 0x24
3000b556:	629a      	str	r2, [r3, #40]	; 0x28
3000b558:	69da      	ldr	r2, [r3, #28]
3000b55a:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b55e:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b562:	61da      	str	r2, [r3, #28]
3000b564:	6019      	str	r1, [r3, #0]
3000b566:	4770      	bx	lr
3000b568:	4b08      	ldr	r3, [pc, #32]	; (3000b58c <rxi316_DynSre_init+0x50>)
3000b56a:	b280      	uxth	r0, r0
3000b56c:	4a08      	ldr	r2, [pc, #32]	; (3000b590 <rxi316_DynSre_init+0x54>)
3000b56e:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b572:	400a      	ands	r2, r1
3000b574:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b578:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b57c:	4310      	orrs	r0, r2
3000b57e:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b582:	689a      	ldr	r2, [r3, #8]
3000b584:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b588:	609a      	str	r2, [r3, #8]
3000b58a:	e7de      	b.n	3000b54a <rxi316_DynSre_init+0xe>
3000b58c:	41100000 	.word	0x41100000
3000b590:	ffff0000 	.word	0xffff0000

3000b594 <ddr_init>:
3000b594:	b500      	push	{lr}
3000b596:	b083      	sub	sp, #12
3000b598:	f7fe fe0c 	bl	3000a1b4 <ddr_init_index>
3000b59c:	4b06      	ldr	r3, [pc, #24]	; (3000b5b8 <ddr_init+0x24>)
3000b59e:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b5a2:	9001      	str	r0, [sp, #4]
3000b5a4:	f7fe fe30 	bl	3000a208 <rxi316_perf_tune>
3000b5a8:	9801      	ldr	r0, [sp, #4]
3000b5aa:	f7ff fa89 	bl	3000aac0 <rxi316_dram_init>
3000b5ae:	b003      	add	sp, #12
3000b5b0:	f85d eb04 	ldr.w	lr, [sp], #4
3000b5b4:	f7ff bfa2 	b.w	3000b4fc <rxi316_ftend_init>
3000b5b8:	3000e3b4 	.word	0x3000e3b4

3000b5bc <SWR_MEM>:
3000b5bc:	2801      	cmp	r0, #1
3000b5be:	d00e      	beq.n	3000b5de <SWR_MEM+0x22>
3000b5c0:	4b31      	ldr	r3, [pc, #196]	; (3000b688 <SWR_MEM+0xcc>)
3000b5c2:	2000      	movs	r0, #0
3000b5c4:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b5c8:	f022 0201 	bic.w	r2, r2, #1
3000b5cc:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b5d0:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b5d4:	f022 0202 	bic.w	r2, r2, #2
3000b5d8:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b5dc:	4770      	bx	lr
3000b5de:	b510      	push	{r4, lr}
3000b5e0:	4c29      	ldr	r4, [pc, #164]	; (3000b688 <SWR_MEM+0xcc>)
3000b5e2:	4a2a      	ldr	r2, [pc, #168]	; (3000b68c <SWR_MEM+0xd0>)
3000b5e4:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b5e8:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b5ec:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b5f0:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b5f4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b5f8:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b5fc:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b600:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b604:	f023 0303 	bic.w	r3, r3, #3
3000b608:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b60c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b610:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b614:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b618:	4790      	blx	r2
3000b61a:	2803      	cmp	r0, #3
3000b61c:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b620:	d026      	beq.n	3000b670 <SWR_MEM+0xb4>
3000b622:	f043 0303 	orr.w	r3, r3, #3
3000b626:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b62a:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b62e:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b632:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b636:	4c14      	ldr	r4, [pc, #80]	; (3000b688 <SWR_MEM+0xcc>)
3000b638:	4b15      	ldr	r3, [pc, #84]	; (3000b690 <SWR_MEM+0xd4>)
3000b63a:	4798      	blx	r3
3000b63c:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b640:	4a14      	ldr	r2, [pc, #80]	; (3000b694 <SWR_MEM+0xd8>)
3000b642:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b646:	f043 0302 	orr.w	r3, r3, #2
3000b64a:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b64e:	4790      	blx	r2
3000b650:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b654:	4b10      	ldr	r3, [pc, #64]	; (3000b698 <SWR_MEM+0xdc>)
3000b656:	f042 0201 	orr.w	r2, r2, #1
3000b65a:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b65e:	e001      	b.n	3000b664 <SWR_MEM+0xa8>
3000b660:	3b01      	subs	r3, #1
3000b662:	d00e      	beq.n	3000b682 <SWR_MEM+0xc6>
3000b664:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b668:	03d2      	lsls	r2, r2, #15
3000b66a:	d5f9      	bpl.n	3000b660 <SWR_MEM+0xa4>
3000b66c:	2000      	movs	r0, #0
3000b66e:	bd10      	pop	{r4, pc}
3000b670:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b674:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b678:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b67c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b680:	e7d9      	b.n	3000b636 <SWR_MEM+0x7a>
3000b682:	2001      	movs	r0, #1
3000b684:	bd10      	pop	{r4, pc}
3000b686:	bf00      	nop
3000b688:	42008800 	.word	0x42008800
3000b68c:	30007e09 	.word	0x30007e09
3000b690:	3000b85d 	.word	0x3000b85d
3000b694:	00009b2d 	.word	0x00009b2d
3000b698:	000186a0 	.word	0x000186a0

3000b69c <SWR_MEM_Manual>:
3000b69c:	4a06      	ldr	r2, [pc, #24]	; (3000b6b8 <SWR_MEM_Manual+0x1c>)
3000b69e:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b6a2:	b120      	cbz	r0, 3000b6ae <SWR_MEM_Manual+0x12>
3000b6a4:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b6a8:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b6ac:	4770      	bx	lr
3000b6ae:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b6b2:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b6b6:	4770      	bx	lr
3000b6b8:	42008800 	.word	0x42008800

3000b6bc <SWR_MEM_Mode_Set>:
3000b6bc:	2801      	cmp	r0, #1
3000b6be:	b538      	push	{r3, r4, r5, lr}
3000b6c0:	d807      	bhi.n	3000b6d2 <SWR_MEM_Mode_Set+0x16>
3000b6c2:	d10a      	bne.n	3000b6da <SWR_MEM_Mode_Set+0x1e>
3000b6c4:	4c22      	ldr	r4, [pc, #136]	; (3000b750 <SWR_MEM_Mode_Set+0x94>)
3000b6c6:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b6ca:	035b      	lsls	r3, r3, #13
3000b6cc:	d51b      	bpl.n	3000b706 <SWR_MEM_Mode_Set+0x4a>
3000b6ce:	2000      	movs	r0, #0
3000b6d0:	bd38      	pop	{r3, r4, r5, pc}
3000b6d2:	217d      	movs	r1, #125	; 0x7d
3000b6d4:	481f      	ldr	r0, [pc, #124]	; (3000b754 <SWR_MEM_Mode_Set+0x98>)
3000b6d6:	f000 fb9b 	bl	3000be10 <__io_assert_failed_veneer>
3000b6da:	491d      	ldr	r1, [pc, #116]	; (3000b750 <SWR_MEM_Mode_Set+0x94>)
3000b6dc:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b6e0:	0358      	lsls	r0, r3, #13
3000b6e2:	d5f4      	bpl.n	3000b6ce <SWR_MEM_Mode_Set+0x12>
3000b6e4:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b6e8:	f242 7310 	movw	r3, #10000	; 0x2710
3000b6ec:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b6f0:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b6f4:	e001      	b.n	3000b6fa <SWR_MEM_Mode_Set+0x3e>
3000b6f6:	3b01      	subs	r3, #1
3000b6f8:	d027      	beq.n	3000b74a <SWR_MEM_Mode_Set+0x8e>
3000b6fa:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b6fe:	0352      	lsls	r2, r2, #13
3000b700:	d4f9      	bmi.n	3000b6f6 <SWR_MEM_Mode_Set+0x3a>
3000b702:	2000      	movs	r0, #0
3000b704:	e7e4      	b.n	3000b6d0 <SWR_MEM_Mode_Set+0x14>
3000b706:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b70a:	2064      	movs	r0, #100	; 0x64
3000b70c:	4d12      	ldr	r5, [pc, #72]	; (3000b758 <SWR_MEM_Mode_Set+0x9c>)
3000b70e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b712:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b716:	47a8      	blx	r5
3000b718:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b71c:	2064      	movs	r0, #100	; 0x64
3000b71e:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b722:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b726:	47a8      	blx	r5
3000b728:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b72c:	f242 7310 	movw	r3, #10000	; 0x2710
3000b730:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b734:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b738:	e001      	b.n	3000b73e <SWR_MEM_Mode_Set+0x82>
3000b73a:	3b01      	subs	r3, #1
3000b73c:	d005      	beq.n	3000b74a <SWR_MEM_Mode_Set+0x8e>
3000b73e:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b742:	0355      	lsls	r5, r2, #13
3000b744:	d5f9      	bpl.n	3000b73a <SWR_MEM_Mode_Set+0x7e>
3000b746:	2000      	movs	r0, #0
3000b748:	e7c2      	b.n	3000b6d0 <SWR_MEM_Mode_Set+0x14>
3000b74a:	2001      	movs	r0, #1
3000b74c:	bd38      	pop	{r3, r4, r5, pc}
3000b74e:	bf00      	nop
3000b750:	42008800 	.word	0x42008800
3000b754:	3000d1d4 	.word	0x3000d1d4
3000b758:	00009b2d 	.word	0x00009b2d

3000b75c <SWR_AUDIO>:
3000b75c:	2801      	cmp	r0, #1
3000b75e:	d00e      	beq.n	3000b77e <SWR_AUDIO+0x22>
3000b760:	4b1e      	ldr	r3, [pc, #120]	; (3000b7dc <SWR_AUDIO+0x80>)
3000b762:	2000      	movs	r0, #0
3000b764:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b768:	f022 0201 	bic.w	r2, r2, #1
3000b76c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b770:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b774:	f022 0202 	bic.w	r2, r2, #2
3000b778:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b77c:	4770      	bx	lr
3000b77e:	b510      	push	{r4, lr}
3000b780:	4c16      	ldr	r4, [pc, #88]	; (3000b7dc <SWR_AUDIO+0x80>)
3000b782:	4b17      	ldr	r3, [pc, #92]	; (3000b7e0 <SWR_AUDIO+0x84>)
3000b784:	4798      	blx	r3
3000b786:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b78a:	4a16      	ldr	r2, [pc, #88]	; (3000b7e4 <SWR_AUDIO+0x88>)
3000b78c:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b790:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b794:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b798:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b79c:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b7a0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b7a4:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b7a8:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b7ac:	f043 0302 	orr.w	r3, r3, #2
3000b7b0:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b7b4:	4790      	blx	r2
3000b7b6:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b7ba:	4b0b      	ldr	r3, [pc, #44]	; (3000b7e8 <SWR_AUDIO+0x8c>)
3000b7bc:	f042 0201 	orr.w	r2, r2, #1
3000b7c0:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b7c4:	e001      	b.n	3000b7ca <SWR_AUDIO+0x6e>
3000b7c6:	3b01      	subs	r3, #1
3000b7c8:	d005      	beq.n	3000b7d6 <SWR_AUDIO+0x7a>
3000b7ca:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b7ce:	03d2      	lsls	r2, r2, #15
3000b7d0:	d5f9      	bpl.n	3000b7c6 <SWR_AUDIO+0x6a>
3000b7d2:	2000      	movs	r0, #0
3000b7d4:	bd10      	pop	{r4, pc}
3000b7d6:	2001      	movs	r0, #1
3000b7d8:	bd10      	pop	{r4, pc}
3000b7da:	bf00      	nop
3000b7dc:	42008800 	.word	0x42008800
3000b7e0:	3000b9ad 	.word	0x3000b9ad
3000b7e4:	00009b2d 	.word	0x00009b2d
3000b7e8:	000186a0 	.word	0x000186a0

3000b7ec <SWR_AUDIO_Manual>:
3000b7ec:	4a06      	ldr	r2, [pc, #24]	; (3000b808 <SWR_AUDIO_Manual+0x1c>)
3000b7ee:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b7f2:	b120      	cbz	r0, 3000b7fe <SWR_AUDIO_Manual+0x12>
3000b7f4:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b7f8:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b7fc:	4770      	bx	lr
3000b7fe:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b802:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b806:	4770      	bx	lr
3000b808:	42008800 	.word	0x42008800

3000b80c <SWR_BST_MODE_Set>:
3000b80c:	4a06      	ldr	r2, [pc, #24]	; (3000b828 <SWR_BST_MODE_Set+0x1c>)
3000b80e:	6813      	ldr	r3, [r2, #0]
3000b810:	b118      	cbz	r0, 3000b81a <SWR_BST_MODE_Set+0xe>
3000b812:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b816:	6013      	str	r3, [r2, #0]
3000b818:	4770      	bx	lr
3000b81a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b81e:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b822:	6013      	str	r3, [r2, #0]
3000b824:	4770      	bx	lr
3000b826:	bf00      	nop
3000b828:	42008100 	.word	0x42008100

3000b82c <SWR_Calib_DCore>:
3000b82c:	4b09      	ldr	r3, [pc, #36]	; (3000b854 <SWR_Calib_DCore+0x28>)
3000b82e:	f240 3231 	movw	r2, #817	; 0x331
3000b832:	4909      	ldr	r1, [pc, #36]	; (3000b858 <SWR_Calib_DCore+0x2c>)
3000b834:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b836:	4001      	ands	r1, r0
3000b838:	430a      	orrs	r2, r1
3000b83a:	651a      	str	r2, [r3, #80]	; 0x50
3000b83c:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b83e:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b842:	641a      	str	r2, [r3, #64]	; 0x40
3000b844:	685a      	ldr	r2, [r3, #4]
3000b846:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b84a:	f042 0204 	orr.w	r2, r2, #4
3000b84e:	605a      	str	r2, [r3, #4]
3000b850:	4770      	bx	lr
3000b852:	bf00      	nop
3000b854:	42008100 	.word	0x42008100
3000b858:	fffffccc 	.word	0xfffffccc

3000b85c <SWR_Calib_MEM>:
3000b85c:	4b4e      	ldr	r3, [pc, #312]	; (3000b998 <SWR_Calib_MEM+0x13c>)
3000b85e:	f240 3231 	movw	r2, #817	; 0x331
3000b862:	494e      	ldr	r1, [pc, #312]	; (3000b99c <SWR_Calib_MEM+0x140>)
3000b864:	2000      	movs	r0, #0
3000b866:	b510      	push	{r4, lr}
3000b868:	f8d3 40e0 	ldr.w	r4, [r3, #224]	; 0xe0
3000b86c:	b082      	sub	sp, #8
3000b86e:	4021      	ands	r1, r4
3000b870:	430a      	orrs	r2, r1
3000b872:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000b876:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b87a:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b87e:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b882:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b886:	2307      	movs	r3, #7
3000b888:	f88d 0007 	strb.w	r0, [sp, #7]
3000b88c:	eb0d 0203 	add.w	r2, sp, r3
3000b890:	f7fd fbca 	bl	30009028 <EFUSE_PMAP_READ8>
3000b894:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b898:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b89c:	2af0      	cmp	r2, #240	; 0xf0
3000b89e:	d15b      	bne.n	3000b958 <SWR_Calib_MEM+0xfc>
3000b8a0:	f003 030f 	and.w	r3, r3, #15
3000b8a4:	2b0f      	cmp	r3, #15
3000b8a6:	d053      	beq.n	3000b950 <SWR_Calib_MEM+0xf4>
3000b8a8:	2102      	movs	r1, #2
3000b8aa:	4c3d      	ldr	r4, [pc, #244]	; (3000b9a0 <SWR_Calib_MEM+0x144>)
3000b8ac:	2307      	movs	r3, #7
3000b8ae:	f10d 0206 	add.w	r2, sp, #6
3000b8b2:	2000      	movs	r0, #0
3000b8b4:	7021      	strb	r1, [r4, #0]
3000b8b6:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000b8ba:	f7fd fbb5 	bl	30009028 <EFUSE_PMAP_READ8>
3000b8be:	7822      	ldrb	r2, [r4, #0]
3000b8c0:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b8c4:	2a03      	cmp	r2, #3
3000b8c6:	d05d      	beq.n	3000b984 <SWR_Calib_MEM+0x128>
3000b8c8:	2a02      	cmp	r2, #2
3000b8ca:	bf0c      	ite	eq
3000b8cc:	f003 030f 	andeq.w	r3, r3, #15
3000b8d0:	2300      	movne	r3, #0
3000b8d2:	4931      	ldr	r1, [pc, #196]	; (3000b998 <SWR_Calib_MEM+0x13c>)
3000b8d4:	4833      	ldr	r0, [pc, #204]	; (3000b9a4 <SWR_Calib_MEM+0x148>)
3000b8d6:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b8da:	f022 020f 	bic.w	r2, r2, #15
3000b8de:	4313      	orrs	r3, r2
3000b8e0:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b8e4:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b8e6:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b8ea:	2b07      	cmp	r3, #7
3000b8ec:	d03d      	beq.n	3000b96a <SWR_Calib_MEM+0x10e>
3000b8ee:	2307      	movs	r3, #7
3000b8f0:	2000      	movs	r0, #0
3000b8f2:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000b8f6:	eb0d 0203 	add.w	r2, sp, r3
3000b8fa:	f88d 0007 	strb.w	r0, [sp, #7]
3000b8fe:	f7fd fb93 	bl	30009028 <EFUSE_PMAP_READ8>
3000b902:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b906:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b90a:	2af0      	cmp	r2, #240	; 0xf0
3000b90c:	d126      	bne.n	3000b95c <SWR_Calib_MEM+0x100>
3000b90e:	f003 030f 	and.w	r3, r3, #15
3000b912:	2b0f      	cmp	r3, #15
3000b914:	d024      	beq.n	3000b960 <SWR_Calib_MEM+0x104>
3000b916:	2002      	movs	r0, #2
3000b918:	2307      	movs	r3, #7
3000b91a:	4c23      	ldr	r4, [pc, #140]	; (3000b9a8 <SWR_Calib_MEM+0x14c>)
3000b91c:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000b920:	eb0d 0203 	add.w	r2, sp, r3
3000b924:	7020      	strb	r0, [r4, #0]
3000b926:	2000      	movs	r0, #0
3000b928:	f7fd fb7e 	bl	30009028 <EFUSE_PMAP_READ8>
3000b92c:	7822      	ldrb	r2, [r4, #0]
3000b92e:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b932:	2a03      	cmp	r2, #3
3000b934:	d028      	beq.n	3000b988 <SWR_Calib_MEM+0x12c>
3000b936:	2a02      	cmp	r2, #2
3000b938:	d029      	beq.n	3000b98e <SWR_Calib_MEM+0x132>
3000b93a:	2200      	movs	r2, #0
3000b93c:	4916      	ldr	r1, [pc, #88]	; (3000b998 <SWR_Calib_MEM+0x13c>)
3000b93e:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b942:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b946:	4313      	orrs	r3, r2
3000b948:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b94c:	b002      	add	sp, #8
3000b94e:	bd10      	pop	{r4, pc}
3000b950:	4b13      	ldr	r3, [pc, #76]	; (3000b9a0 <SWR_Calib_MEM+0x144>)
3000b952:	2201      	movs	r2, #1
3000b954:	701a      	strb	r2, [r3, #0]
3000b956:	e7ca      	b.n	3000b8ee <SWR_Calib_MEM+0x92>
3000b958:	2103      	movs	r1, #3
3000b95a:	e7a6      	b.n	3000b8aa <SWR_Calib_MEM+0x4e>
3000b95c:	2003      	movs	r0, #3
3000b95e:	e7db      	b.n	3000b918 <SWR_Calib_MEM+0xbc>
3000b960:	4b11      	ldr	r3, [pc, #68]	; (3000b9a8 <SWR_Calib_MEM+0x14c>)
3000b962:	2201      	movs	r2, #1
3000b964:	701a      	strb	r2, [r3, #0]
3000b966:	b002      	add	sp, #8
3000b968:	bd10      	pop	{r4, pc}
3000b96a:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b96c:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b970:	d1bd      	bne.n	3000b8ee <SWR_Calib_MEM+0x92>
3000b972:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b976:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b97a:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b97e:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b982:	e7b4      	b.n	3000b8ee <SWR_Calib_MEM+0x92>
3000b984:	091b      	lsrs	r3, r3, #4
3000b986:	e7a4      	b.n	3000b8d2 <SWR_Calib_MEM+0x76>
3000b988:	091b      	lsrs	r3, r3, #4
3000b98a:	059a      	lsls	r2, r3, #22
3000b98c:	e7d6      	b.n	3000b93c <SWR_Calib_MEM+0xe0>
3000b98e:	f003 030f 	and.w	r3, r3, #15
3000b992:	059a      	lsls	r2, r3, #22
3000b994:	e7d2      	b.n	3000b93c <SWR_Calib_MEM+0xe0>
3000b996:	bf00      	nop
3000b998:	42008800 	.word	0x42008800
3000b99c:	fffffccc 	.word	0xfffffccc
3000b9a0:	3000e54f 	.word	0x3000e54f
3000b9a4:	42008100 	.word	0x42008100
3000b9a8:	3000e54e 	.word	0x3000e54e

3000b9ac <SWR_Calib_AUD>:
3000b9ac:	4b44      	ldr	r3, [pc, #272]	; (3000bac0 <SWR_Calib_AUD+0x114>)
3000b9ae:	f240 3231 	movw	r2, #817	; 0x331
3000b9b2:	4944      	ldr	r1, [pc, #272]	; (3000bac4 <SWR_Calib_AUD+0x118>)
3000b9b4:	2000      	movs	r0, #0
3000b9b6:	b510      	push	{r4, lr}
3000b9b8:	f8d3 40a0 	ldr.w	r4, [r3, #160]	; 0xa0
3000b9bc:	b082      	sub	sp, #8
3000b9be:	4021      	ands	r1, r4
3000b9c0:	430a      	orrs	r2, r1
3000b9c2:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000b9c6:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b9ca:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b9ce:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b9d2:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b9d6:	2307      	movs	r3, #7
3000b9d8:	f88d 0007 	strb.w	r0, [sp, #7]
3000b9dc:	eb0d 0203 	add.w	r2, sp, r3
3000b9e0:	f7fd fb22 	bl	30009028 <EFUSE_PMAP_READ8>
3000b9e4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b9e8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b9ec:	2af0      	cmp	r2, #240	; 0xf0
3000b9ee:	d155      	bne.n	3000ba9c <SWR_Calib_AUD+0xf0>
3000b9f0:	f003 030f 	and.w	r3, r3, #15
3000b9f4:	2b0f      	cmp	r3, #15
3000b9f6:	d04d      	beq.n	3000ba94 <SWR_Calib_AUD+0xe8>
3000b9f8:	2102      	movs	r1, #2
3000b9fa:	4c33      	ldr	r4, [pc, #204]	; (3000bac8 <SWR_Calib_AUD+0x11c>)
3000b9fc:	2307      	movs	r3, #7
3000b9fe:	f10d 0206 	add.w	r2, sp, #6
3000ba02:	2000      	movs	r0, #0
3000ba04:	7021      	strb	r1, [r4, #0]
3000ba06:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000ba0a:	f7fd fb0d 	bl	30009028 <EFUSE_PMAP_READ8>
3000ba0e:	7822      	ldrb	r2, [r4, #0]
3000ba10:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000ba14:	2a03      	cmp	r2, #3
3000ba16:	d04a      	beq.n	3000baae <SWR_Calib_AUD+0x102>
3000ba18:	2a02      	cmp	r2, #2
3000ba1a:	bf0c      	ite	eq
3000ba1c:	f003 030f 	andeq.w	r3, r3, #15
3000ba20:	2300      	movne	r3, #0
3000ba22:	4927      	ldr	r1, [pc, #156]	; (3000bac0 <SWR_Calib_AUD+0x114>)
3000ba24:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000ba28:	f022 020f 	bic.w	r2, r2, #15
3000ba2c:	4313      	orrs	r3, r2
3000ba2e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ba32:	2307      	movs	r3, #7
3000ba34:	2000      	movs	r0, #0
3000ba36:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000ba3a:	eb0d 0203 	add.w	r2, sp, r3
3000ba3e:	f88d 0007 	strb.w	r0, [sp, #7]
3000ba42:	f7fd faf1 	bl	30009028 <EFUSE_PMAP_READ8>
3000ba46:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ba4a:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ba4e:	2af0      	cmp	r2, #240	; 0xf0
3000ba50:	d126      	bne.n	3000baa0 <SWR_Calib_AUD+0xf4>
3000ba52:	f003 030f 	and.w	r3, r3, #15
3000ba56:	2b0f      	cmp	r3, #15
3000ba58:	d024      	beq.n	3000baa4 <SWR_Calib_AUD+0xf8>
3000ba5a:	2002      	movs	r0, #2
3000ba5c:	2307      	movs	r3, #7
3000ba5e:	4c1b      	ldr	r4, [pc, #108]	; (3000bacc <SWR_Calib_AUD+0x120>)
3000ba60:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000ba64:	eb0d 0203 	add.w	r2, sp, r3
3000ba68:	7020      	strb	r0, [r4, #0]
3000ba6a:	2000      	movs	r0, #0
3000ba6c:	f7fd fadc 	bl	30009028 <EFUSE_PMAP_READ8>
3000ba70:	7822      	ldrb	r2, [r4, #0]
3000ba72:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ba76:	2a03      	cmp	r2, #3
3000ba78:	d01b      	beq.n	3000bab2 <SWR_Calib_AUD+0x106>
3000ba7a:	2a02      	cmp	r2, #2
3000ba7c:	d01c      	beq.n	3000bab8 <SWR_Calib_AUD+0x10c>
3000ba7e:	2200      	movs	r2, #0
3000ba80:	490f      	ldr	r1, [pc, #60]	; (3000bac0 <SWR_Calib_AUD+0x114>)
3000ba82:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000ba86:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ba8a:	4313      	orrs	r3, r2
3000ba8c:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ba90:	b002      	add	sp, #8
3000ba92:	bd10      	pop	{r4, pc}
3000ba94:	4b0c      	ldr	r3, [pc, #48]	; (3000bac8 <SWR_Calib_AUD+0x11c>)
3000ba96:	2201      	movs	r2, #1
3000ba98:	701a      	strb	r2, [r3, #0]
3000ba9a:	e7ca      	b.n	3000ba32 <SWR_Calib_AUD+0x86>
3000ba9c:	2103      	movs	r1, #3
3000ba9e:	e7ac      	b.n	3000b9fa <SWR_Calib_AUD+0x4e>
3000baa0:	2003      	movs	r0, #3
3000baa2:	e7db      	b.n	3000ba5c <SWR_Calib_AUD+0xb0>
3000baa4:	4b09      	ldr	r3, [pc, #36]	; (3000bacc <SWR_Calib_AUD+0x120>)
3000baa6:	2201      	movs	r2, #1
3000baa8:	701a      	strb	r2, [r3, #0]
3000baaa:	b002      	add	sp, #8
3000baac:	bd10      	pop	{r4, pc}
3000baae:	091b      	lsrs	r3, r3, #4
3000bab0:	e7b7      	b.n	3000ba22 <SWR_Calib_AUD+0x76>
3000bab2:	091b      	lsrs	r3, r3, #4
3000bab4:	059a      	lsls	r2, r3, #22
3000bab6:	e7e3      	b.n	3000ba80 <SWR_Calib_AUD+0xd4>
3000bab8:	f003 030f 	and.w	r3, r3, #15
3000babc:	059a      	lsls	r2, r3, #22
3000babe:	e7df      	b.n	3000ba80 <SWR_Calib_AUD+0xd4>
3000bac0:	42008800 	.word	0x42008800
3000bac4:	fffffccc 	.word	0xfffffccc
3000bac8:	3000e54d 	.word	0x3000e54d
3000bacc:	3000e54c 	.word	0x3000e54c

3000bad0 <__aeabi_uldivmod>:
3000bad0:	b953      	cbnz	r3, 3000bae8 <__aeabi_uldivmod+0x18>
3000bad2:	b94a      	cbnz	r2, 3000bae8 <__aeabi_uldivmod+0x18>
3000bad4:	2900      	cmp	r1, #0
3000bad6:	bf08      	it	eq
3000bad8:	2800      	cmpeq	r0, #0
3000bada:	bf1c      	itt	ne
3000badc:	f04f 31ff 	movne.w	r1, #4294967295
3000bae0:	f04f 30ff 	movne.w	r0, #4294967295
3000bae4:	f000 b960 	b.w	3000bda8 <__aeabi_idiv0>
3000bae8:	f1ad 0c08 	sub.w	ip, sp, #8
3000baec:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000baf0:	f000 f806 	bl	3000bb00 <__udivmoddi4>
3000baf4:	f8dd e004 	ldr.w	lr, [sp, #4]
3000baf8:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000bafc:	b004      	add	sp, #16
3000bafe:	4770      	bx	lr

3000bb00 <__udivmoddi4>:
3000bb00:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bb04:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000bb06:	4688      	mov	r8, r1
3000bb08:	468e      	mov	lr, r1
3000bb0a:	4615      	mov	r5, r2
3000bb0c:	4604      	mov	r4, r0
3000bb0e:	4619      	mov	r1, r3
3000bb10:	2b00      	cmp	r3, #0
3000bb12:	f040 80c6 	bne.w	3000bca2 <__udivmoddi4+0x1a2>
3000bb16:	4542      	cmp	r2, r8
3000bb18:	fab2 f782 	clz	r7, r2
3000bb1c:	d946      	bls.n	3000bbac <__udivmoddi4+0xac>
3000bb1e:	b14f      	cbz	r7, 3000bb34 <__udivmoddi4+0x34>
3000bb20:	f1c7 0c20 	rsb	ip, r7, #32
3000bb24:	fa08 f307 	lsl.w	r3, r8, r7
3000bb28:	40bd      	lsls	r5, r7
3000bb2a:	40bc      	lsls	r4, r7
3000bb2c:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bb30:	ea4c 0e03 	orr.w	lr, ip, r3
3000bb34:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000bb38:	fa1f fc85 	uxth.w	ip, r5
3000bb3c:	0c22      	lsrs	r2, r4, #16
3000bb3e:	fbbe f9f8 	udiv	r9, lr, r8
3000bb42:	fb08 e319 	mls	r3, r8, r9, lr
3000bb46:	fb09 fa0c 	mul.w	sl, r9, ip
3000bb4a:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000bb4e:	459a      	cmp	sl, r3
3000bb50:	d928      	bls.n	3000bba4 <__udivmoddi4+0xa4>
3000bb52:	18eb      	adds	r3, r5, r3
3000bb54:	f109 30ff 	add.w	r0, r9, #4294967295
3000bb58:	d204      	bcs.n	3000bb64 <__udivmoddi4+0x64>
3000bb5a:	459a      	cmp	sl, r3
3000bb5c:	d902      	bls.n	3000bb64 <__udivmoddi4+0x64>
3000bb5e:	f1a9 0002 	sub.w	r0, r9, #2
3000bb62:	442b      	add	r3, r5
3000bb64:	eba3 030a 	sub.w	r3, r3, sl
3000bb68:	b2a4      	uxth	r4, r4
3000bb6a:	fbb3 f2f8 	udiv	r2, r3, r8
3000bb6e:	fb08 3312 	mls	r3, r8, r2, r3
3000bb72:	fb02 fc0c 	mul.w	ip, r2, ip
3000bb76:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bb7a:	45a4      	cmp	ip, r4
3000bb7c:	d914      	bls.n	3000bba8 <__udivmoddi4+0xa8>
3000bb7e:	192c      	adds	r4, r5, r4
3000bb80:	f102 33ff 	add.w	r3, r2, #4294967295
3000bb84:	d203      	bcs.n	3000bb8e <__udivmoddi4+0x8e>
3000bb86:	45a4      	cmp	ip, r4
3000bb88:	d901      	bls.n	3000bb8e <__udivmoddi4+0x8e>
3000bb8a:	1e93      	subs	r3, r2, #2
3000bb8c:	442c      	add	r4, r5
3000bb8e:	eba4 040c 	sub.w	r4, r4, ip
3000bb92:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000bb96:	b11e      	cbz	r6, 3000bba0 <__udivmoddi4+0xa0>
3000bb98:	40fc      	lsrs	r4, r7
3000bb9a:	2300      	movs	r3, #0
3000bb9c:	6034      	str	r4, [r6, #0]
3000bb9e:	6073      	str	r3, [r6, #4]
3000bba0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bba4:	4648      	mov	r0, r9
3000bba6:	e7dd      	b.n	3000bb64 <__udivmoddi4+0x64>
3000bba8:	4613      	mov	r3, r2
3000bbaa:	e7f0      	b.n	3000bb8e <__udivmoddi4+0x8e>
3000bbac:	b902      	cbnz	r2, 3000bbb0 <__udivmoddi4+0xb0>
3000bbae:	deff      	udf	#255	; 0xff
3000bbb0:	bb8f      	cbnz	r7, 3000bc16 <__udivmoddi4+0x116>
3000bbb2:	eba8 0302 	sub.w	r3, r8, r2
3000bbb6:	2101      	movs	r1, #1
3000bbb8:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bbbc:	b2aa      	uxth	r2, r5
3000bbbe:	0c20      	lsrs	r0, r4, #16
3000bbc0:	fbb3 fcfe 	udiv	ip, r3, lr
3000bbc4:	fb0e 331c 	mls	r3, lr, ip, r3
3000bbc8:	fb0c f802 	mul.w	r8, ip, r2
3000bbcc:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000bbd0:	4598      	cmp	r8, r3
3000bbd2:	d962      	bls.n	3000bc9a <__udivmoddi4+0x19a>
3000bbd4:	18eb      	adds	r3, r5, r3
3000bbd6:	f10c 30ff 	add.w	r0, ip, #4294967295
3000bbda:	d204      	bcs.n	3000bbe6 <__udivmoddi4+0xe6>
3000bbdc:	4598      	cmp	r8, r3
3000bbde:	d902      	bls.n	3000bbe6 <__udivmoddi4+0xe6>
3000bbe0:	f1ac 0002 	sub.w	r0, ip, #2
3000bbe4:	442b      	add	r3, r5
3000bbe6:	eba3 0308 	sub.w	r3, r3, r8
3000bbea:	b2a4      	uxth	r4, r4
3000bbec:	fbb3 fcfe 	udiv	ip, r3, lr
3000bbf0:	fb0e 331c 	mls	r3, lr, ip, r3
3000bbf4:	fb0c f202 	mul.w	r2, ip, r2
3000bbf8:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bbfc:	42a2      	cmp	r2, r4
3000bbfe:	d94e      	bls.n	3000bc9e <__udivmoddi4+0x19e>
3000bc00:	192c      	adds	r4, r5, r4
3000bc02:	f10c 33ff 	add.w	r3, ip, #4294967295
3000bc06:	d204      	bcs.n	3000bc12 <__udivmoddi4+0x112>
3000bc08:	42a2      	cmp	r2, r4
3000bc0a:	d902      	bls.n	3000bc12 <__udivmoddi4+0x112>
3000bc0c:	f1ac 0302 	sub.w	r3, ip, #2
3000bc10:	442c      	add	r4, r5
3000bc12:	1aa4      	subs	r4, r4, r2
3000bc14:	e7bd      	b.n	3000bb92 <__udivmoddi4+0x92>
3000bc16:	f1c7 0c20 	rsb	ip, r7, #32
3000bc1a:	40bd      	lsls	r5, r7
3000bc1c:	fa08 f307 	lsl.w	r3, r8, r7
3000bc20:	40bc      	lsls	r4, r7
3000bc22:	fa28 f90c 	lsr.w	r9, r8, ip
3000bc26:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bc2a:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bc2e:	ea4c 0203 	orr.w	r2, ip, r3
3000bc32:	b2ab      	uxth	r3, r5
3000bc34:	fbb9 fcfe 	udiv	ip, r9, lr
3000bc38:	0c11      	lsrs	r1, r2, #16
3000bc3a:	fb0e 901c 	mls	r0, lr, ip, r9
3000bc3e:	fb0c f803 	mul.w	r8, ip, r3
3000bc42:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000bc46:	4580      	cmp	r8, r0
3000bc48:	d923      	bls.n	3000bc92 <__udivmoddi4+0x192>
3000bc4a:	1828      	adds	r0, r5, r0
3000bc4c:	f10c 31ff 	add.w	r1, ip, #4294967295
3000bc50:	d204      	bcs.n	3000bc5c <__udivmoddi4+0x15c>
3000bc52:	4580      	cmp	r8, r0
3000bc54:	d902      	bls.n	3000bc5c <__udivmoddi4+0x15c>
3000bc56:	f1ac 0102 	sub.w	r1, ip, #2
3000bc5a:	4428      	add	r0, r5
3000bc5c:	eba0 0008 	sub.w	r0, r0, r8
3000bc60:	b292      	uxth	r2, r2
3000bc62:	fbb0 fcfe 	udiv	ip, r0, lr
3000bc66:	fb0e 001c 	mls	r0, lr, ip, r0
3000bc6a:	fb0c f803 	mul.w	r8, ip, r3
3000bc6e:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000bc72:	4598      	cmp	r8, r3
3000bc74:	d90f      	bls.n	3000bc96 <__udivmoddi4+0x196>
3000bc76:	18eb      	adds	r3, r5, r3
3000bc78:	f10c 32ff 	add.w	r2, ip, #4294967295
3000bc7c:	d204      	bcs.n	3000bc88 <__udivmoddi4+0x188>
3000bc7e:	4598      	cmp	r8, r3
3000bc80:	d902      	bls.n	3000bc88 <__udivmoddi4+0x188>
3000bc82:	f1ac 0202 	sub.w	r2, ip, #2
3000bc86:	442b      	add	r3, r5
3000bc88:	eba3 0308 	sub.w	r3, r3, r8
3000bc8c:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000bc90:	e792      	b.n	3000bbb8 <__udivmoddi4+0xb8>
3000bc92:	4661      	mov	r1, ip
3000bc94:	e7e2      	b.n	3000bc5c <__udivmoddi4+0x15c>
3000bc96:	4662      	mov	r2, ip
3000bc98:	e7f6      	b.n	3000bc88 <__udivmoddi4+0x188>
3000bc9a:	4660      	mov	r0, ip
3000bc9c:	e7a3      	b.n	3000bbe6 <__udivmoddi4+0xe6>
3000bc9e:	4663      	mov	r3, ip
3000bca0:	e7b7      	b.n	3000bc12 <__udivmoddi4+0x112>
3000bca2:	4543      	cmp	r3, r8
3000bca4:	d905      	bls.n	3000bcb2 <__udivmoddi4+0x1b2>
3000bca6:	b10e      	cbz	r6, 3000bcac <__udivmoddi4+0x1ac>
3000bca8:	e9c6 0800 	strd	r0, r8, [r6]
3000bcac:	2100      	movs	r1, #0
3000bcae:	4608      	mov	r0, r1
3000bcb0:	e776      	b.n	3000bba0 <__udivmoddi4+0xa0>
3000bcb2:	fab3 fc83 	clz	ip, r3
3000bcb6:	f1bc 0f00 	cmp.w	ip, #0
3000bcba:	d110      	bne.n	3000bcde <__udivmoddi4+0x1de>
3000bcbc:	4543      	cmp	r3, r8
3000bcbe:	d301      	bcc.n	3000bcc4 <__udivmoddi4+0x1c4>
3000bcc0:	4282      	cmp	r2, r0
3000bcc2:	d80a      	bhi.n	3000bcda <__udivmoddi4+0x1da>
3000bcc4:	1a84      	subs	r4, r0, r2
3000bcc6:	eb68 0303 	sbc.w	r3, r8, r3
3000bcca:	2001      	movs	r0, #1
3000bccc:	469e      	mov	lr, r3
3000bcce:	2e00      	cmp	r6, #0
3000bcd0:	d068      	beq.n	3000bda4 <__udivmoddi4+0x2a4>
3000bcd2:	e9c6 4e00 	strd	r4, lr, [r6]
3000bcd6:	2100      	movs	r1, #0
3000bcd8:	e762      	b.n	3000bba0 <__udivmoddi4+0xa0>
3000bcda:	4660      	mov	r0, ip
3000bcdc:	e7f7      	b.n	3000bcce <__udivmoddi4+0x1ce>
3000bcde:	f1cc 0e20 	rsb	lr, ip, #32
3000bce2:	fa03 f30c 	lsl.w	r3, r3, ip
3000bce6:	fa02 f50c 	lsl.w	r5, r2, ip
3000bcea:	fa00 f70c 	lsl.w	r7, r0, ip
3000bcee:	fa22 f40e 	lsr.w	r4, r2, lr
3000bcf2:	fa28 fb0e 	lsr.w	fp, r8, lr
3000bcf6:	fa20 f10e 	lsr.w	r1, r0, lr
3000bcfa:	431c      	orrs	r4, r3
3000bcfc:	fa08 f30c 	lsl.w	r3, r8, ip
3000bd00:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000bd04:	4319      	orrs	r1, r3
3000bd06:	b2a3      	uxth	r3, r4
3000bd08:	0c0a      	lsrs	r2, r1, #16
3000bd0a:	fbbb f8f9 	udiv	r8, fp, r9
3000bd0e:	fb09 bb18 	mls	fp, r9, r8, fp
3000bd12:	fb08 fa03 	mul.w	sl, r8, r3
3000bd16:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000bd1a:	4592      	cmp	sl, r2
3000bd1c:	d93e      	bls.n	3000bd9c <__udivmoddi4+0x29c>
3000bd1e:	18a2      	adds	r2, r4, r2
3000bd20:	f108 30ff 	add.w	r0, r8, #4294967295
3000bd24:	d204      	bcs.n	3000bd30 <__udivmoddi4+0x230>
3000bd26:	4592      	cmp	sl, r2
3000bd28:	d902      	bls.n	3000bd30 <__udivmoddi4+0x230>
3000bd2a:	f1a8 0002 	sub.w	r0, r8, #2
3000bd2e:	4422      	add	r2, r4
3000bd30:	eba2 020a 	sub.w	r2, r2, sl
3000bd34:	b289      	uxth	r1, r1
3000bd36:	fbb2 f8f9 	udiv	r8, r2, r9
3000bd3a:	fb09 2218 	mls	r2, r9, r8, r2
3000bd3e:	fb08 f303 	mul.w	r3, r8, r3
3000bd42:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000bd46:	4293      	cmp	r3, r2
3000bd48:	d92a      	bls.n	3000bda0 <__udivmoddi4+0x2a0>
3000bd4a:	18a2      	adds	r2, r4, r2
3000bd4c:	f108 31ff 	add.w	r1, r8, #4294967295
3000bd50:	d204      	bcs.n	3000bd5c <__udivmoddi4+0x25c>
3000bd52:	4293      	cmp	r3, r2
3000bd54:	d902      	bls.n	3000bd5c <__udivmoddi4+0x25c>
3000bd56:	f1a8 0102 	sub.w	r1, r8, #2
3000bd5a:	4422      	add	r2, r4
3000bd5c:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000bd60:	1ad2      	subs	r2, r2, r3
3000bd62:	fba0 9805 	umull	r9, r8, r0, r5
3000bd66:	4542      	cmp	r2, r8
3000bd68:	464b      	mov	r3, r9
3000bd6a:	4641      	mov	r1, r8
3000bd6c:	d302      	bcc.n	3000bd74 <__udivmoddi4+0x274>
3000bd6e:	d106      	bne.n	3000bd7e <__udivmoddi4+0x27e>
3000bd70:	454f      	cmp	r7, r9
3000bd72:	d204      	bcs.n	3000bd7e <__udivmoddi4+0x27e>
3000bd74:	3801      	subs	r0, #1
3000bd76:	ebb9 0305 	subs.w	r3, r9, r5
3000bd7a:	eb68 0104 	sbc.w	r1, r8, r4
3000bd7e:	b18e      	cbz	r6, 3000bda4 <__udivmoddi4+0x2a4>
3000bd80:	1afc      	subs	r4, r7, r3
3000bd82:	eb62 0701 	sbc.w	r7, r2, r1
3000bd86:	fa07 fe0e 	lsl.w	lr, r7, lr
3000bd8a:	fa24 f30c 	lsr.w	r3, r4, ip
3000bd8e:	fa27 f70c 	lsr.w	r7, r7, ip
3000bd92:	ea4e 0303 	orr.w	r3, lr, r3
3000bd96:	e9c6 3700 	strd	r3, r7, [r6]
3000bd9a:	e79c      	b.n	3000bcd6 <__udivmoddi4+0x1d6>
3000bd9c:	4640      	mov	r0, r8
3000bd9e:	e7c7      	b.n	3000bd30 <__udivmoddi4+0x230>
3000bda0:	4641      	mov	r1, r8
3000bda2:	e7db      	b.n	3000bd5c <__udivmoddi4+0x25c>
3000bda4:	4631      	mov	r1, r6
3000bda6:	e6fb      	b.n	3000bba0 <__udivmoddi4+0xa0>

3000bda8 <__aeabi_idiv0>:
3000bda8:	4770      	bx	lr
3000bdaa:	bf00      	nop
3000bdac:	0000      	movs	r0, r0
	...

3000bdb0 <____wrap_strlen_veneer>:
3000bdb0:	b401      	push	{r0}
3000bdb2:	4802      	ldr	r0, [pc, #8]	; (3000bdbc <____wrap_strlen_veneer+0xc>)
3000bdb4:	4684      	mov	ip, r0
3000bdb6:	bc01      	pop	{r0}
3000bdb8:	4760      	bx	ip
3000bdba:	bf00      	nop
3000bdbc:	00012f25 	.word	0x00012f25

3000bdc0 <__SBOOT_GetMdType_veneer>:
3000bdc0:	b401      	push	{r0}
3000bdc2:	4802      	ldr	r0, [pc, #8]	; (3000bdcc <__SBOOT_GetMdType_veneer+0xc>)
3000bdc4:	4684      	mov	ip, r0
3000bdc6:	bc01      	pop	{r0}
3000bdc8:	4760      	bx	ip
3000bdca:	bf00      	nop
3000bdcc:	0000319d 	.word	0x0000319d

3000bdd0 <__SBOOT_Validate_Algorithm_veneer>:
3000bdd0:	b401      	push	{r0}
3000bdd2:	4802      	ldr	r0, [pc, #8]	; (3000bddc <__SBOOT_Validate_Algorithm_veneer+0xc>)
3000bdd4:	4684      	mov	ip, r0
3000bdd6:	bc01      	pop	{r0}
3000bdd8:	4760      	bx	ip
3000bdda:	bf00      	nop
3000bddc:	000033ed 	.word	0x000033ed

3000bde0 <__SBOOT_Validate_Signature_veneer>:
3000bde0:	b401      	push	{r0}
3000bde2:	4802      	ldr	r0, [pc, #8]	; (3000bdec <__SBOOT_Validate_Signature_veneer+0xc>)
3000bde4:	4684      	mov	ip, r0
3000bde6:	bc01      	pop	{r0}
3000bde8:	4760      	bx	ip
3000bdea:	bf00      	nop
3000bdec:	00003431 	.word	0x00003431

3000bdf0 <____wrap_memset_veneer>:
3000bdf0:	b401      	push	{r0}
3000bdf2:	4802      	ldr	r0, [pc, #8]	; (3000bdfc <____wrap_memset_veneer+0xc>)
3000bdf4:	4684      	mov	ip, r0
3000bdf6:	bc01      	pop	{r0}
3000bdf8:	4760      	bx	ip
3000bdfa:	bf00      	nop
3000bdfc:	00012efd 	.word	0x00012efd

3000be00 <____wrap_memcpy_veneer>:
3000be00:	b401      	push	{r0}
3000be02:	4802      	ldr	r0, [pc, #8]	; (3000be0c <____wrap_memcpy_veneer+0xc>)
3000be04:	4684      	mov	ip, r0
3000be06:	bc01      	pop	{r0}
3000be08:	4760      	bx	ip
3000be0a:	bf00      	nop
3000be0c:	00012f05 	.word	0x00012f05

3000be10 <__io_assert_failed_veneer>:
3000be10:	b401      	push	{r0}
3000be12:	4802      	ldr	r0, [pc, #8]	; (3000be1c <__io_assert_failed_veneer+0xc>)
3000be14:	4684      	mov	ip, r0
3000be16:	bc01      	pop	{r0}
3000be18:	4760      	bx	ip
3000be1a:	bf00      	nop
3000be1c:	0000992d 	.word	0x0000992d

3000be20 <__SBOOT_Validate_ImgHash_veneer>:
3000be20:	b401      	push	{r0}
3000be22:	4802      	ldr	r0, [pc, #8]	; (3000be2c <__SBOOT_Validate_ImgHash_veneer+0xc>)
3000be24:	4684      	mov	ip, r0
3000be26:	bc01      	pop	{r0}
3000be28:	4760      	bx	ip
3000be2a:	bf00      	nop
3000be2c:	00003541 	.word	0x00003541

3000be30 <__DiagPrintf_veneer>:
3000be30:	b401      	push	{r0}
3000be32:	4802      	ldr	r0, [pc, #8]	; (3000be3c <__DiagPrintf_veneer+0xc>)
3000be34:	4684      	mov	ip, r0
3000be36:	bc01      	pop	{r0}
3000be38:	4760      	bx	ip
3000be3a:	bf00      	nop
3000be3c:	0000e5f9 	.word	0x0000e5f9

3000be40 <__SBOOT_Validate_PubKey_veneer>:
3000be40:	b401      	push	{r0}
3000be42:	4802      	ldr	r0, [pc, #8]	; (3000be4c <__SBOOT_Validate_PubKey_veneer+0xc>)
3000be44:	4684      	mov	ip, r0
3000be46:	bc01      	pop	{r0}
3000be48:	4760      	bx	ip
3000be4a:	bf00      	nop
3000be4c:	00003495 	.word	0x00003495

3000be50 <____wrap_memcmp_veneer>:
3000be50:	b401      	push	{r0}
3000be52:	4802      	ldr	r0, [pc, #8]	; (3000be5c <____wrap_memcmp_veneer+0xc>)
3000be54:	4684      	mov	ip, r0
3000be56:	bc01      	pop	{r0}
3000be58:	4760      	bx	ip
3000be5a:	bf00      	nop
3000be5c:	00012f0d 	.word	0x00012f0d
3000be60:	004f4931 	.word	0x004f4931
3000be64:	00004f32 	.word	0x00004f32
3000be68:	004f4932 	.word	0x004f4932
3000be6c:	00004f34 	.word	0x00004f34
3000be70:	004f4934 	.word	0x004f4934
3000be74:	414c460d 	.word	0x414c460d
3000be78:	435f4853 	.word	0x435f4853
3000be7c:	62696c61 	.word	0x62696c61
3000be80:	69746172 	.word	0x69746172
3000be84:	654e6e6f 	.word	0x654e6e6f
3000be88:	78252077 	.word	0x78252077
3000be8c:	2078253a 	.word	0x2078253a
3000be90:	73616870 	.word	0x73616870
3000be94:	68735f65 	.word	0x68735f65
3000be98:	3a746669 	.word	0x3a746669
3000be9c:	000a7825 	.word	0x000a7825
3000bea0:	414c460d 	.word	0x414c460d
3000bea4:	435f4853 	.word	0x435f4853
3000bea8:	62696c61 	.word	0x62696c61
3000beac:	69746172 	.word	0x69746172
3000beb0:	654e6e6f 	.word	0x654e6e6f
3000beb4:	694c2077 	.word	0x694c2077
3000beb8:	6544656e 	.word	0x6544656e
3000bebc:	3a79616c 	.word	0x3a79616c
3000bec0:	000a7825 	.word	0x000a7825
3000bec4:	414c460d 	.word	0x414c460d
3000bec8:	435f4853 	.word	0x435f4853
3000becc:	62696c61 	.word	0x62696c61
3000bed0:	69746172 	.word	0x69746172
3000bed4:	654e6e6f 	.word	0x654e6e6f
3000bed8:	69772077 	.word	0x69772077
3000bedc:	776f646e 	.word	0x776f646e
3000bee0:	74735f31 	.word	0x74735f31
3000bee4:	3a747261 	.word	0x3a747261
3000bee8:	77206425 	.word	0x77206425
3000beec:	6f646e69 	.word	0x6f646e69
3000bef0:	735f3177 	.word	0x735f3177
3000bef4:	3a657a69 	.word	0x3a657a69
3000bef8:	0a206425 	.word	0x0a206425
3000befc:	00000000 	.word	0x00000000
3000bf00:	414c460d 	.word	0x414c460d
3000bf04:	435f4853 	.word	0x435f4853
3000bf08:	62696c61 	.word	0x62696c61
3000bf0c:	69746172 	.word	0x69746172
3000bf10:	654e6e6f 	.word	0x654e6e6f
3000bf14:	68702077 	.word	0x68702077
3000bf18:	5f657361 	.word	0x5f657361
3000bf1c:	66696873 	.word	0x66696873
3000bf20:	64695f74 	.word	0x64695f74
3000bf24:	64253a78 	.word	0x64253a78
3000bf28:	00000a20 	.word	0x00000a20
3000bf2c:	414c460d 	.word	0x414c460d
3000bf30:	435f4853 	.word	0x435f4853
3000bf34:	62696c61 	.word	0x62696c61
3000bf38:	69746172 	.word	0x69746172
3000bf3c:	25206e6f 	.word	0x25206e6f
3000bf40:	61732078 	.word	0x61732078
3000bf44:	656c706d 	.word	0x656c706d
3000bf48:	6168705f 	.word	0x6168705f
3000bf4c:	253a6573 	.word	0x253a6573
3000bf50:	61722078 	.word	0x61722078
3000bf54:	253a6574 	.word	0x253a6574
3000bf58:	00000a78 	.word	0x00000a78
3000bf5c:	414c460d 	.word	0x414c460d
3000bf60:	435f4853 	.word	0x435f4853
3000bf64:	62696c61 	.word	0x62696c61
3000bf68:	69746172 	.word	0x69746172
3000bf6c:	73206e6f 	.word	0x73206e6f
3000bf70:	6c706d61 	.word	0x6c706d61
3000bf74:	68705f65 	.word	0x68705f65
3000bf78:	3a657361 	.word	0x3a657361
3000bf7c:	72207825 	.word	0x72207825
3000bf80:	3a657461 	.word	0x3a657461
3000bf84:	000a7825 	.word	0x000a7825
3000bf88:	444f4d5b 	.word	0x444f4d5b
3000bf8c:	5f454c55 	.word	0x5f454c55
3000bf90:	544f4f42 	.word	0x544f4f42
3000bf94:	56454c2d 	.word	0x56454c2d
3000bf98:	495f4c45 	.word	0x495f4c45
3000bf9c:	5d4f464e 	.word	0x5d4f464e
3000bfa0:	6c61633a 	.word	0x6c61633a
3000bfa4:	61726269 	.word	0x61726269
3000bfa8:	6e6f6974 	.word	0x6e6f6974
3000bfac:	3a6b6f5f 	.word	0x3a6b6f5f
3000bfb0:	3a64255b 	.word	0x3a64255b
3000bfb4:	253a6425 	.word	0x253a6425
3000bfb8:	0a205d64 	.word	0x0a205d64
3000bfbc:	00000000 	.word	0x00000000
3000bfc0:	444f4d5b 	.word	0x444f4d5b
3000bfc4:	5f454c55 	.word	0x5f454c55
3000bfc8:	544f4f42 	.word	0x544f4f42
3000bfcc:	56454c2d 	.word	0x56454c2d
3000bfd0:	495f4c45 	.word	0x495f4c45
3000bfd4:	5d4f464e 	.word	0x5d4f464e
3000bfd8:	414c463a 	.word	0x414c463a
3000bfdc:	43204853 	.word	0x43204853
3000bfe0:	42494c41 	.word	0x42494c41
3000bfe4:	2578305b 	.word	0x2578305b
3000bfe8:	4b4f2078 	.word	0x4b4f2078
3000bfec:	00000a5d 	.word	0x00000a5d
3000bff0:	444f4d5b 	.word	0x444f4d5b
3000bff4:	5f454c55 	.word	0x5f454c55
3000bff8:	544f4f42 	.word	0x544f4f42
3000bffc:	56454c2d 	.word	0x56454c2d
3000c000:	495f4c45 	.word	0x495f4c45
3000c004:	5d4f464e 	.word	0x5d4f464e
3000c008:	414c463a 	.word	0x414c463a
3000c00c:	43204853 	.word	0x43204853
3000c010:	42494c41 	.word	0x42494c41
3000c014:	2578305b 	.word	0x2578305b
3000c018:	41462078 	.word	0x41462078
3000c01c:	0a5d4c49 	.word	0x0a5d4c49
3000c020:	00000000 	.word	0x00000000
3000c024:	444f4d5b 	.word	0x444f4d5b
3000c028:	5f454c55 	.word	0x5f454c55
3000c02c:	544f4f42 	.word	0x544f4f42
3000c030:	56454c2d 	.word	0x56454c2d
3000c034:	495f4c45 	.word	0x495f4c45
3000c038:	5d4f464e 	.word	0x5d4f464e
3000c03c:	616c463a 	.word	0x616c463a
3000c040:	52206873 	.word	0x52206873
3000c044:	20646165 	.word	0x20646165
3000c048:	000a7325 	.word	0x000a7325
3000c04c:	444f4d5b 	.word	0x444f4d5b
3000c050:	5f454c55 	.word	0x5f454c55
3000c054:	544f4f42 	.word	0x544f4f42
3000c058:	56454c2d 	.word	0x56454c2d
3000c05c:	495f4c45 	.word	0x495f4c45
3000c060:	5d4f464e 	.word	0x5d4f464e
3000c064:	616c463a 	.word	0x616c463a
3000c068:	52206873 	.word	0x52206873
3000c06c:	20646165 	.word	0x20646165
3000c070:	202c7325 	.word	0x202c7325
3000c074:	4c494146 	.word	0x4c494146
3000c078:	0000000a 	.word	0x0000000a
3000c07c:	444f4d5b 	.word	0x444f4d5b
3000c080:	5f454c55 	.word	0x5f454c55
3000c084:	544f4f42 	.word	0x544f4f42
3000c088:	56454c2d 	.word	0x56454c2d
3000c08c:	455f4c45 	.word	0x455f4c45
3000c090:	524f5252 	.word	0x524f5252
3000c094:	6c463a5d 	.word	0x6c463a5d
3000c098:	20687361 	.word	0x20687361
3000c09c:	74697753 	.word	0x74697753
3000c0a0:	52206863 	.word	0x52206863
3000c0a4:	20646165 	.word	0x20646165
3000c0a8:	65646f4d 	.word	0x65646f4d
3000c0ac:	49414620 	.word	0x49414620
3000c0b0:	00000a4c 	.word	0x00000a4c
3000c0b4:	444f4d5b 	.word	0x444f4d5b
3000c0b8:	5f454c55 	.word	0x5f454c55
3000c0bc:	544f4f42 	.word	0x544f4f42
3000c0c0:	56454c2d 	.word	0x56454c2d
3000c0c4:	495f4c45 	.word	0x495f4c45
3000c0c8:	5d4f464e 	.word	0x5d4f464e
3000c0cc:	616c463a 	.word	0x616c463a
3000c0d0:	49206873 	.word	0x49206873
3000c0d4:	25203a44 	.word	0x25203a44
3000c0d8:	78252d78 	.word	0x78252d78
3000c0dc:	0a78252d 	.word	0x0a78252d
3000c0e0:	00000000 	.word	0x00000000
3000c0e4:	6968540d 	.word	0x6968540d
3000c0e8:	6c662073 	.word	0x6c662073
3000c0ec:	20687361 	.word	0x20687361
3000c0f0:	65707974 	.word	0x65707974
3000c0f4:	20736920 	.word	0x20736920
3000c0f8:	20746f6e 	.word	0x20746f6e
3000c0fc:	70707573 	.word	0x70707573
3000c100:	6574726f 	.word	0x6574726f
3000c104:	000a2164 	.word	0x000a2164
3000c108:	444f4d5b 	.word	0x444f4d5b
3000c10c:	5f454c55 	.word	0x5f454c55
3000c110:	544f4f42 	.word	0x544f4f42
3000c114:	56454c2d 	.word	0x56454c2d
3000c118:	495f4c45 	.word	0x495f4c45
3000c11c:	5d4f464e 	.word	0x5d4f464e
3000c120:	4e414e3a 	.word	0x4e414e3a
3000c124:	44492044 	.word	0x44492044
3000c128:	7825203a 	.word	0x7825203a
3000c12c:	0a78252d 	.word	0x0a78252d
3000c130:	00000000 	.word	0x00000000
3000c134:	20304d4b 	.word	0x20304d4b
3000c138:	20504958 	.word	0x20504958
3000c13c:	00474d49 	.word	0x00474d49
3000c140:	20304d4b 	.word	0x20304d4b
3000c144:	4d415253 	.word	0x4d415253
3000c148:	00000000 	.word	0x00000000
3000c14c:	20304d4b 	.word	0x20304d4b
3000c150:	4d415244 	.word	0x4d415244
3000c154:	00000000 	.word	0x00000000
3000c158:	20344d4b 	.word	0x20344d4b
3000c15c:	20504958 	.word	0x20504958
3000c160:	00474d49 	.word	0x00474d49
3000c164:	20344d4b 	.word	0x20344d4b
3000c168:	4d415253 	.word	0x4d415253
3000c16c:	00000000 	.word	0x00000000
3000c170:	20344d4b 	.word	0x20344d4b
3000c174:	4d415244 	.word	0x4d415244
3000c178:	00000000 	.word	0x00000000
3000c17c:	42205041 	.word	0x42205041
3000c180:	5320314c 	.word	0x5320314c
3000c184:	004d4152 	.word	0x004d4152
3000c188:	42205041 	.word	0x42205041
3000c18c:	4420314c 	.word	0x4420314c
3000c190:	004d4152 	.word	0x004d4152
3000c194:	46205041 	.word	0x46205041
3000c198:	00005049 	.word	0x00005049
3000c19c:	444f4d5b 	.word	0x444f4d5b
3000c1a0:	5f454c55 	.word	0x5f454c55
3000c1a4:	544f4f42 	.word	0x544f4f42
3000c1a8:	56454c2d 	.word	0x56454c2d
3000c1ac:	495f4c45 	.word	0x495f4c45
3000c1b0:	5d4f464e 	.word	0x5d4f464e
3000c1b4:	2073253a 	.word	0x2073253a
3000c1b8:	61766e49 	.word	0x61766e49
3000c1bc:	0a64696c 	.word	0x0a64696c
3000c1c0:	00000000 	.word	0x00000000
3000c1c4:	444f4d5b 	.word	0x444f4d5b
3000c1c8:	5f454c55 	.word	0x5f454c55
3000c1cc:	544f4f42 	.word	0x544f4f42
3000c1d0:	56454c2d 	.word	0x56454c2d
3000c1d4:	495f4c45 	.word	0x495f4c45
3000c1d8:	5d4f464e 	.word	0x5d4f464e
3000c1dc:	5b73253a 	.word	0x5b73253a
3000c1e0:	78383025 	.word	0x78383025
3000c1e4:	5d78253a 	.word	0x5d78253a
3000c1e8:	0000000a 	.word	0x0000000a
3000c1ec:	444f4d5b 	.word	0x444f4d5b
3000c1f0:	5f454c55 	.word	0x5f454c55
3000c1f4:	544f4f42 	.word	0x544f4f42
3000c1f8:	56454c2d 	.word	0x56454c2d
3000c1fc:	495f4c45 	.word	0x495f4c45
3000c200:	5d4f464e 	.word	0x5d4f464e
3000c204:	5044523a 	.word	0x5044523a
3000c208:	0a4e4520 	.word	0x0a4e4520
3000c20c:	00000000 	.word	0x00000000
3000c210:	444f4d5b 	.word	0x444f4d5b
3000c214:	5f454c55 	.word	0x5f454c55
3000c218:	544f4f42 	.word	0x544f4f42
3000c21c:	56454c2d 	.word	0x56454c2d
3000c220:	455f4c45 	.word	0x455f4c45
3000c224:	524f5252 	.word	0x524f5252
3000c228:	61463a5d 	.word	0x61463a5d
3000c22c:	74206c69 	.word	0x74206c69
3000c230:	6f6c206f 	.word	0x6f6c206f
3000c234:	52206461 	.word	0x52206461
3000c238:	69205044 	.word	0x69205044
3000c23c:	6567616d 	.word	0x6567616d
3000c240:	00000a21 	.word	0x00000a21
3000c244:	474d490d 	.word	0x474d490d
3000c248:	544f2032 	.word	0x544f2032
3000c24c:	4e452046 	.word	0x4e452046
3000c250:	0000000a 	.word	0x0000000a
3000c254:	46544f0d 	.word	0x46544f0d
3000c258:	646f4d20 	.word	0x646f4d20
3000c25c:	72652065 	.word	0x72652065
3000c260:	0d726f72 	.word	0x0d726f72
3000c264:	0000000a 	.word	0x0000000a
3000c268:	494c4156 	.word	0x494c4156
3000c26c:	00000044 	.word	0x00000044
3000c270:	41564e49 	.word	0x41564e49
3000c274:	0044494c 	.word	0x0044494c
3000c278:	74726563 	.word	0x74726563
3000c27c:	63696669 	.word	0x63696669
3000c280:	20657461 	.word	0x20657461
3000c284:	20434345 	.word	0x20434345
3000c288:	69726576 	.word	0x69726576
3000c28c:	66207966 	.word	0x66207966
3000c290:	2c6c6961 	.word	0x2c6c6961
3000c294:	6c6f7220 	.word	0x6c6f7220
3000c298:	6361626c 	.word	0x6361626c
3000c29c:	000a0d6b 	.word	0x000a0d6b
3000c2a0:	444f4d5b 	.word	0x444f4d5b
3000c2a4:	5f454c55 	.word	0x5f454c55
3000c2a8:	544f4f42 	.word	0x544f4f42
3000c2ac:	56454c2d 	.word	0x56454c2d
3000c2b0:	495f4c45 	.word	0x495f4c45
3000c2b4:	5d4f464e 	.word	0x5d4f464e
3000c2b8:	474d493a 	.word	0x474d493a
3000c2bc:	544f2832 	.word	0x544f2832
3000c2c0:	29642541 	.word	0x29642541
3000c2c4:	2c732520 	.word	0x2c732520
3000c2c8:	74657220 	.word	0x74657220
3000c2cc:	6425203a 	.word	0x6425203a
3000c2d0:	0000000a 	.word	0x0000000a
3000c2d4:	32474d49 	.word	0x32474d49
3000c2d8:	43434520 	.word	0x43434520
3000c2dc:	72657620 	.word	0x72657620
3000c2e0:	20796669 	.word	0x20796669
3000c2e4:	6c696166 	.word	0x6c696166
3000c2e8:	6f72202c 	.word	0x6f72202c
3000c2ec:	61626c6c 	.word	0x61626c6c
3000c2f0:	0a0d6b63 	.word	0x0a0d6b63
3000c2f4:	00000000 	.word	0x00000000
3000c2f8:	5f41544f 	.word	0x5f41544f
3000c2fc:	45205041 	.word	0x45205041
3000c300:	76204343 	.word	0x76204343
3000c304:	66697265 	.word	0x66697265
3000c308:	61662079 	.word	0x61662079
3000c30c:	202c6c69 	.word	0x202c6c69
3000c310:	6c6c6f72 	.word	0x6c6c6f72
3000c314:	6b636162 	.word	0x6b636162
3000c318:	00000a0d 	.word	0x00000a0d
3000c31c:	64616f4c 	.word	0x64616f4c
3000c320:	41544f20 	.word	0x41544f20
3000c324:	2050415f 	.word	0x2050415f
3000c328:	67616d69 	.word	0x67616d69
3000c32c:	61662065 	.word	0x61662065
3000c330:	202c6c69 	.word	0x202c6c69
3000c334:	6c6c6f72 	.word	0x6c6c6f72
3000c338:	6b636162 	.word	0x6b636162
3000c33c:	00000a0d 	.word	0x00000a0d
3000c340:	444f4d5b 	.word	0x444f4d5b
3000c344:	5f454c55 	.word	0x5f454c55
3000c348:	544f4f42 	.word	0x544f4f42
3000c34c:	56454c2d 	.word	0x56454c2d
3000c350:	455f4c45 	.word	0x455f4c45
3000c354:	524f5252 	.word	0x524f5252
3000c358:	61463a5d 	.word	0x61463a5d
3000c35c:	74206c69 	.word	0x74206c69
3000c360:	6f6c206f 	.word	0x6f6c206f
3000c364:	41206461 	.word	0x41206461
3000c368:	6d692050 	.word	0x6d692050
3000c36c:	21656761 	.word	0x21656761
3000c370:	0000000a 	.word	0x0000000a
3000c374:	20344d4b 	.word	0x20344d4b
3000c378:	33474d49 	.word	0x33474d49
3000c37c:	00000000 	.word	0x00000000
3000c380:	20344d4b 	.word	0x20344d4b
3000c384:	0043534e 	.word	0x0043534e
3000c388:	31474d49 	.word	0x31474d49
3000c38c:	00000000 	.word	0x00000000
3000c390:	32474d49 	.word	0x32474d49
3000c394:	00000000 	.word	0x00000000
3000c398:	42205041 	.word	0x42205041
3000c39c:	4626314c 	.word	0x4626314c
3000c3a0:	00005049 	.word	0x00005049
3000c3a4:	444f4d5b 	.word	0x444f4d5b
3000c3a8:	5f454c55 	.word	0x5f454c55
3000c3ac:	544f4f42 	.word	0x544f4f42
3000c3b0:	56454c2d 	.word	0x56454c2d
3000c3b4:	455f4c45 	.word	0x455f4c45
3000c3b8:	524f5252 	.word	0x524f5252
3000c3bc:	68543a5d 	.word	0x68543a5d
3000c3c0:	20657265 	.word	0x20657265
3000c3c4:	206e6163 	.word	0x206e6163
3000c3c8:	61206562 	.word	0x61206562
3000c3cc:	6f6d2074 	.word	0x6f6d2074
3000c3d0:	38207473 	.word	0x38207473
3000c3d4:	49535220 	.word	0x49535220
3000c3d8:	616d2050 	.word	0x616d2050
3000c3dc:	65206b73 	.word	0x65206b73
3000c3e0:	6972746e 	.word	0x6972746e
3000c3e4:	0a217365 	.word	0x0a217365
3000c3e8:	00000000 	.word	0x00000000
3000c3ec:	444f4d5b 	.word	0x444f4d5b
3000c3f0:	5f454c55 	.word	0x5f454c55
3000c3f4:	544f4f42 	.word	0x544f4f42
3000c3f8:	56454c2d 	.word	0x56454c2d
3000c3fc:	455f4c45 	.word	0x455f4c45
3000c400:	524f5252 	.word	0x524f5252
3000c404:	53523a5d 	.word	0x53523a5d
3000c408:	65205049 	.word	0x65205049
3000c40c:	7972746e 	.word	0x7972746e
3000c410:	64646120 	.word	0x64646120
3000c414:	78252072 	.word	0x78252072
3000c418:	766e6920 	.word	0x766e6920
3000c41c:	64696c61 	.word	0x64696c61
3000c420:	0000000a 	.word	0x0000000a
3000c424:	67452301 	.word	0x67452301
3000c428:	efcdab89 	.word	0xefcdab89
3000c42c:	67452301 	.word	0x67452301
3000c430:	efcdab89 	.word	0xefcdab89
3000c434:	00000000 	.word	0x00000000
3000c438:	42205041 	.word	0x42205041
3000c43c:	0032334c 	.word	0x0032334c
3000c440:	2073250d 	.word	0x2073250d
3000c444:	49524556 	.word	0x49524556
3000c448:	50205946 	.word	0x50205946
3000c44c:	0a535341 	.word	0x0a535341
3000c450:	00000000 	.word	0x00000000
3000c454:	2073250d 	.word	0x2073250d
3000c458:	49524556 	.word	0x49524556
3000c45c:	46205946 	.word	0x46205946
3000c460:	2c4c4941 	.word	0x2c4c4941
3000c464:	74657220 	.word	0x74657220
3000c468:	25203d20 	.word	0x25203d20
3000c46c:	00000a64 	.word	0x00000a64
3000c470:	7265430d 	.word	0x7265430d
3000c474:	69666974 	.word	0x69666974
3000c478:	65746163 	.word	0x65746163
3000c47c:	52455620 	.word	0x52455620
3000c480:	20594649 	.word	0x20594649
3000c484:	4c494146 	.word	0x4c494146
3000c488:	6572202c 	.word	0x6572202c
3000c48c:	203d2074 	.word	0x203d2074
3000c490:	000a6425 	.word	0x000a6425
3000c494:	444f4d5b 	.word	0x444f4d5b
3000c498:	5f454c55 	.word	0x5f454c55
3000c49c:	544f4f42 	.word	0x544f4f42
3000c4a0:	56454c2d 	.word	0x56454c2d
3000c4a4:	495f4c45 	.word	0x495f4c45
3000c4a8:	5d4f464e 	.word	0x5d4f464e
3000c4ac:	41544f3a 	.word	0x41544f3a
3000c4b0:	53552031 	.word	0x53552031
3000c4b4:	76202c45 	.word	0x76202c45
3000c4b8:	69737265 	.word	0x69737265
3000c4bc:	203a6e6f 	.word	0x203a6e6f
3000c4c0:	000a6425 	.word	0x000a6425
3000c4c4:	444f4d5b 	.word	0x444f4d5b
3000c4c8:	5f454c55 	.word	0x5f454c55
3000c4cc:	544f4f42 	.word	0x544f4f42
3000c4d0:	56454c2d 	.word	0x56454c2d
3000c4d4:	495f4c45 	.word	0x495f4c45
3000c4d8:	5d4f464e 	.word	0x5d4f464e
3000c4dc:	41544f3a 	.word	0x41544f3a
3000c4e0:	53552032 	.word	0x53552032
3000c4e4:	76202c45 	.word	0x76202c45
3000c4e8:	69737265 	.word	0x69737265
3000c4ec:	203a6e6f 	.word	0x203a6e6f
3000c4f0:	000a6425 	.word	0x000a6425
3000c4f4:	444f4d5b 	.word	0x444f4d5b
3000c4f8:	5f454c55 	.word	0x5f454c55
3000c4fc:	544f4f42 	.word	0x544f4f42
3000c500:	56454c2d 	.word	0x56454c2d
3000c504:	495f4c45 	.word	0x495f4c45
3000c508:	5d4f464e 	.word	0x5d4f464e
3000c50c:	746f423a 	.word	0x746f423a
3000c510:	544f2068 	.word	0x544f2068
3000c514:	6d692041 	.word	0x6d692041
3000c518:	20656761 	.word	0x20656761
3000c51c:	69726576 	.word	0x69726576
3000c520:	66207966 	.word	0x66207966
3000c524:	656c6961 	.word	0x656c6961
3000c528:	00000a64 	.word	0x00000a64
3000c52c:	444f4d5b 	.word	0x444f4d5b
3000c530:	5f454c55 	.word	0x5f454c55
3000c534:	544f4f42 	.word	0x544f4f42
3000c538:	56454c2d 	.word	0x56454c2d
3000c53c:	495f4c45 	.word	0x495f4c45
3000c540:	5d4f464e 	.word	0x5d4f464e
3000c544:	696e493a 	.word	0x696e493a
3000c548:	42572074 	.word	0x42572074
3000c54c:	00000a0d 	.word	0x00000a0d
3000c550:	444f4d5b 	.word	0x444f4d5b
3000c554:	5f454c55 	.word	0x5f454c55
3000c558:	544f4f42 	.word	0x544f4f42
3000c55c:	56454c2d 	.word	0x56454c2d
3000c560:	495f4c45 	.word	0x495f4c45
3000c564:	5d4f464e 	.word	0x5d4f464e
3000c568:	4965523a 	.word	0x4965523a
3000c56c:	2074696e 	.word	0x2074696e
3000c570:	0d524444 	.word	0x0d524444
3000c574:	0000000a 	.word	0x0000000a
3000c578:	20315042 	.word	0x20315042
3000c57c:	73726576 	.word	0x73726576
3000c580:	206e6f69 	.word	0x206e6f69
3000c584:	61766e69 	.word	0x61766e69
3000c588:	0a64696c 	.word	0x0a64696c
3000c58c:	00000000 	.word	0x00000000
3000c590:	20315042 	.word	0x20315042
3000c594:	6d726f66 	.word	0x6d726f66
3000c598:	69207461 	.word	0x69207461
3000c59c:	6c61766e 	.word	0x6c61766e
3000c5a0:	000a6469 	.word	0x000a6469
3000c5a4:	20315042 	.word	0x20315042
3000c5a8:	69746361 	.word	0x69746361
3000c5ac:	69206576 	.word	0x69206576
3000c5b0:	7865646e 	.word	0x7865646e
3000c5b4:	766e6920 	.word	0x766e6920
3000c5b8:	64696c61 	.word	0x64696c61
3000c5bc:	0000000a 	.word	0x0000000a
3000c5c0:	20315042 	.word	0x20315042
3000c5c4:	61746164 	.word	0x61746164
3000c5c8:	6c617620 	.word	0x6c617620
3000c5cc:	202c6469 	.word	0x202c6469
3000c5d0:	73726576 	.word	0x73726576
3000c5d4:	3a6e6f69 	.word	0x3a6e6f69
3000c5d8:	000a6425 	.word	0x000a6425
3000c5dc:	20325042 	.word	0x20325042
3000c5e0:	6d726f66 	.word	0x6d726f66
3000c5e4:	69207461 	.word	0x69207461
3000c5e8:	6c61766e 	.word	0x6c61766e
3000c5ec:	000a6469 	.word	0x000a6469
3000c5f0:	20315042 	.word	0x20315042
3000c5f4:	33435243 	.word	0x33435243
3000c5f8:	616d2032 	.word	0x616d2032
3000c5fc:	2c686374 	.word	0x2c686374
3000c600:	74746120 	.word	0x74746120
3000c604:	65686361 	.word	0x65686361
3000c608:	52432064 	.word	0x52432064
3000c60c:	30203a43 	.word	0x30203a43
3000c610:	2c782578 	.word	0x2c782578
3000c614:	6c616320 	.word	0x6c616320
3000c618:	616c7563 	.word	0x616c7563
3000c61c:	20646574 	.word	0x20646574
3000c620:	3a435243 	.word	0x3a435243
3000c624:	25783020 	.word	0x25783020
3000c628:	00000a78 	.word	0x00000a78
3000c62c:	20315042 	.word	0x20315042
3000c630:	33435243 	.word	0x33435243
3000c634:	6f6e2032 	.word	0x6f6e2032
3000c638:	616d2074 	.word	0x616d2074
3000c63c:	2c686374 	.word	0x2c686374
3000c640:	74746120 	.word	0x74746120
3000c644:	65686361 	.word	0x65686361
3000c648:	52432064 	.word	0x52432064
3000c64c:	30203a43 	.word	0x30203a43
3000c650:	2c782578 	.word	0x2c782578
3000c654:	6c616320 	.word	0x6c616320
3000c658:	616c7563 	.word	0x616c7563
3000c65c:	20646574 	.word	0x20646574
3000c660:	3a435243 	.word	0x3a435243
3000c664:	25783020 	.word	0x25783020
3000c668:	00000a78 	.word	0x00000a78
3000c66c:	20325042 	.word	0x20325042
3000c670:	33435243 	.word	0x33435243
3000c674:	616d2032 	.word	0x616d2032
3000c678:	2c686374 	.word	0x2c686374
3000c67c:	74746120 	.word	0x74746120
3000c680:	65686361 	.word	0x65686361
3000c684:	52432064 	.word	0x52432064
3000c688:	30203a43 	.word	0x30203a43
3000c68c:	2c782578 	.word	0x2c782578
3000c690:	6c616320 	.word	0x6c616320
3000c694:	616c7563 	.word	0x616c7563
3000c698:	20646574 	.word	0x20646574
3000c69c:	3a435243 	.word	0x3a435243
3000c6a0:	25783020 	.word	0x25783020
3000c6a4:	00000a78 	.word	0x00000a78
3000c6a8:	20325042 	.word	0x20325042
3000c6ac:	33435243 	.word	0x33435243
3000c6b0:	6f6e2032 	.word	0x6f6e2032
3000c6b4:	616d2074 	.word	0x616d2074
3000c6b8:	2c686374 	.word	0x2c686374
3000c6bc:	74746120 	.word	0x74746120
3000c6c0:	65686361 	.word	0x65686361
3000c6c4:	52432064 	.word	0x52432064
3000c6c8:	30203a43 	.word	0x30203a43
3000c6cc:	2c782578 	.word	0x2c782578
3000c6d0:	6c616320 	.word	0x6c616320
3000c6d4:	616c7563 	.word	0x616c7563
3000c6d8:	20646574 	.word	0x20646574
3000c6dc:	3a435243 	.word	0x3a435243
3000c6e0:	25783020 	.word	0x25783020
3000c6e4:	00000a78 	.word	0x00000a78
3000c6e8:	20315042 	.word	0x20315042
3000c6ec:	3161746f 	.word	0x3161746f
3000c6f0:	64646120 	.word	0x64646120
3000c6f4:	73736572 	.word	0x73736572
3000c6f8:	766e6920 	.word	0x766e6920
3000c6fc:	64696c61 	.word	0x64696c61
3000c700:	0000000a 	.word	0x0000000a
3000c704:	20315042 	.word	0x20315042
3000c708:	3261746f 	.word	0x3261746f
3000c70c:	64646120 	.word	0x64646120
3000c710:	73736572 	.word	0x73736572
3000c714:	766e6920 	.word	0x766e6920
3000c718:	64696c61 	.word	0x64696c61
3000c71c:	0000000a 	.word	0x0000000a
3000c720:	68746f42 	.word	0x68746f42
3000c724:	20504220 	.word	0x20504220
3000c728:	20435243 	.word	0x20435243
3000c72c:	696c6176 	.word	0x696c6176
3000c730:	00000a64 	.word	0x00000a64
3000c734:	20315042 	.word	0x20315042
3000c738:	656c6573 	.word	0x656c6573
3000c73c:	64657463 	.word	0x64657463
3000c740:	0000000a 	.word	0x0000000a
3000c744:	20325042 	.word	0x20325042
3000c748:	656c6573 	.word	0x656c6573
3000c74c:	64657463 	.word	0x64657463
3000c750:	0000000a 	.word	0x0000000a
3000c754:	68746f42 	.word	0x68746f42
3000c758:	20504220 	.word	0x20504220
3000c75c:	61766e69 	.word	0x61766e69
3000c760:	0a64696c 	.word	0x0a64696c
3000c764:	00000000 	.word	0x00000000
3000c768:	6f636552 	.word	0x6f636552
3000c76c:	79726576 	.word	0x79726576
3000c770:	61747320 	.word	0x61747320
3000c774:	000a7472 	.word	0x000a7472
3000c778:	69746341 	.word	0x69746341
3000c77c:	49206576 	.word	0x49206576
3000c780:	7865646e 	.word	0x7865646e
3000c784:	6425203a 	.word	0x6425203a
3000c788:	0000000a 	.word	0x0000000a
3000c78c:	3141544f 	.word	0x3141544f
3000c790:	766e6920 	.word	0x766e6920
3000c794:	64696c61 	.word	0x64696c61
3000c798:	0000000a 	.word	0x0000000a
3000c79c:	3241544f 	.word	0x3241544f
3000c7a0:	766e6920 	.word	0x766e6920
3000c7a4:	64696c61 	.word	0x64696c61
3000c7a8:	0000000a 	.word	0x0000000a
3000c7ac:	3141544f 	.word	0x3141544f
3000c7b0:	43524320 	.word	0x43524320
3000c7b4:	6d203233 	.word	0x6d203233
3000c7b8:	68637461 	.word	0x68637461
3000c7bc:	7461202c 	.word	0x7461202c
3000c7c0:	68636174 	.word	0x68636174
3000c7c4:	43206465 	.word	0x43206465
3000c7c8:	203a4352 	.word	0x203a4352
3000c7cc:	78257830 	.word	0x78257830
3000c7d0:	6163202c 	.word	0x6163202c
3000c7d4:	6c75636c 	.word	0x6c75636c
3000c7d8:	64657461 	.word	0x64657461
3000c7dc:	43524320 	.word	0x43524320
3000c7e0:	7830203a 	.word	0x7830203a
3000c7e4:	000a7825 	.word	0x000a7825
3000c7e8:	3141544f 	.word	0x3141544f
3000c7ec:	43524320 	.word	0x43524320
3000c7f0:	6e203233 	.word	0x6e203233
3000c7f4:	6d20746f 	.word	0x6d20746f
3000c7f8:	68637461 	.word	0x68637461
3000c7fc:	7461202c 	.word	0x7461202c
3000c800:	68636174 	.word	0x68636174
3000c804:	43206465 	.word	0x43206465
3000c808:	203a4352 	.word	0x203a4352
3000c80c:	78257830 	.word	0x78257830
3000c810:	6163202c 	.word	0x6163202c
3000c814:	6c75636c 	.word	0x6c75636c
3000c818:	64657461 	.word	0x64657461
3000c81c:	43524320 	.word	0x43524320
3000c820:	7830203a 	.word	0x7830203a
3000c824:	202c7825 	.word	0x202c7825
3000c828:	73617265 	.word	0x73617265
3000c82c:	544f2065 	.word	0x544f2065
3000c830:	000a3141 	.word	0x000a3141
3000c834:	3241544f 	.word	0x3241544f
3000c838:	43524320 	.word	0x43524320
3000c83c:	6d203233 	.word	0x6d203233
3000c840:	68637461 	.word	0x68637461
3000c844:	7461202c 	.word	0x7461202c
3000c848:	68636174 	.word	0x68636174
3000c84c:	43206465 	.word	0x43206465
3000c850:	203a4352 	.word	0x203a4352
3000c854:	78257830 	.word	0x78257830
3000c858:	6163202c 	.word	0x6163202c
3000c85c:	6c75636c 	.word	0x6c75636c
3000c860:	64657461 	.word	0x64657461
3000c864:	43524320 	.word	0x43524320
3000c868:	7830203a 	.word	0x7830203a
3000c86c:	000a7825 	.word	0x000a7825
3000c870:	3241544f 	.word	0x3241544f
3000c874:	43524320 	.word	0x43524320
3000c878:	6e203233 	.word	0x6e203233
3000c87c:	6d20746f 	.word	0x6d20746f
3000c880:	68637461 	.word	0x68637461
3000c884:	7461202c 	.word	0x7461202c
3000c888:	68636174 	.word	0x68636174
3000c88c:	43206465 	.word	0x43206465
3000c890:	203a4352 	.word	0x203a4352
3000c894:	78257830 	.word	0x78257830
3000c898:	6163202c 	.word	0x6163202c
3000c89c:	6c75636c 	.word	0x6c75636c
3000c8a0:	64657461 	.word	0x64657461
3000c8a4:	43524320 	.word	0x43524320
3000c8a8:	7830203a 	.word	0x7830203a
3000c8ac:	202c7825 	.word	0x202c7825
3000c8b0:	73617265 	.word	0x73617265
3000c8b4:	544f2065 	.word	0x544f2065
3000c8b8:	000a3241 	.word	0x000a3241
3000c8bc:	69746341 	.word	0x69746341
3000c8c0:	69206576 	.word	0x69206576
3000c8c4:	7865646e 	.word	0x7865646e
3000c8c8:	41544f20 	.word	0x41544f20
3000c8cc:	766e6920 	.word	0x766e6920
3000c8d0:	64696c61 	.word	0x64696c61
3000c8d4:	6572202c 	.word	0x6572202c
3000c8d8:	74697277 	.word	0x74697277
3000c8dc:	63612065 	.word	0x63612065
3000c8e0:	65766974 	.word	0x65766974
3000c8e4:	646e6920 	.word	0x646e6920
3000c8e8:	74207865 	.word	0x74207865
3000c8ec:	6425206f 	.word	0x6425206f
3000c8f0:	0000000a 	.word	0x0000000a
3000c8f4:	206c6c41 	.word	0x206c6c41
3000c8f8:	2041544f 	.word	0x2041544f
3000c8fc:	72646461 	.word	0x72646461
3000c900:	65737365 	.word	0x65737365
3000c904:	6f6e2073 	.word	0x6f6e2073
3000c908:	61762074 	.word	0x61762074
3000c90c:	2c64696c 	.word	0x2c64696c
3000c910:	63657220 	.word	0x63657220
3000c914:	7265766f 	.word	0x7265766f
3000c918:	61662079 	.word	0x61662079
3000c91c:	000a6c69 	.word	0x000a6c69
3000c920:	20325042 	.word	0x20325042
3000c924:	3261746f 	.word	0x3261746f
3000c928:	64646120 	.word	0x64646120
3000c92c:	73736572 	.word	0x73736572
3000c930:	766e6920 	.word	0x766e6920
3000c934:	64696c61 	.word	0x64696c61
3000c938:	0000000a 	.word	0x0000000a
3000c93c:	20325042 	.word	0x20325042
3000c940:	73726576 	.word	0x73726576
3000c944:	206e6f69 	.word	0x206e6f69
3000c948:	61766e69 	.word	0x61766e69
3000c94c:	0a64696c 	.word	0x0a64696c
3000c950:	00000000 	.word	0x00000000
3000c954:	20325042 	.word	0x20325042
3000c958:	69746361 	.word	0x69746361
3000c95c:	69206576 	.word	0x69206576
3000c960:	7865646e 	.word	0x7865646e
3000c964:	766e6920 	.word	0x766e6920
3000c968:	64696c61 	.word	0x64696c61
3000c96c:	0000000a 	.word	0x0000000a
3000c970:	20325042 	.word	0x20325042
3000c974:	3161746f 	.word	0x3161746f
3000c978:	64646120 	.word	0x64646120
3000c97c:	73736572 	.word	0x73736572
3000c980:	766e6920 	.word	0x766e6920
3000c984:	64696c61 	.word	0x64696c61
3000c988:	0000000a 	.word	0x0000000a
3000c98c:	20325042 	.word	0x20325042
3000c990:	61746164 	.word	0x61746164
3000c994:	6c617620 	.word	0x6c617620
3000c998:	202c6469 	.word	0x202c6469
3000c99c:	73726576 	.word	0x73726576
3000c9a0:	3a6e6f69 	.word	0x3a6e6f69
3000c9a4:	000a6425 	.word	0x000a6425
3000c9a8:	444f4d5b 	.word	0x444f4d5b
3000c9ac:	5f454c55 	.word	0x5f454c55
3000c9b0:	544f4f42 	.word	0x544f4f42
3000c9b4:	56454c2d 	.word	0x56454c2d
3000c9b8:	495f4c45 	.word	0x495f4c45
3000c9bc:	5d4f464e 	.word	0x5d4f464e
3000c9c0:	7369443a 	.word	0x7369443a
3000c9c4:	656c6261 	.word	0x656c6261
3000c9c8:	304d4b20 	.word	0x304d4b20
3000c9cc:	6f6f4220 	.word	0x6f6f4220
3000c9d0:	69202074 	.word	0x69202074
3000c9d4:	6554206e 	.word	0x6554206e
3000c9d8:	6d207473 	.word	0x6d207473
3000c9dc:	0d65646f 	.word	0x0d65646f
3000c9e0:	0000000a 	.word	0x0000000a
3000c9e4:	444f4d5b 	.word	0x444f4d5b
3000c9e8:	5f454c55 	.word	0x5f454c55
3000c9ec:	544f4f42 	.word	0x544f4f42
3000c9f0:	56454c2d 	.word	0x56454c2d
3000c9f4:	495f4c45 	.word	0x495f4c45
3000c9f8:	5d4f464e 	.word	0x5d4f464e
3000c9fc:	7079423a 	.word	0x7079423a
3000ca00:	20737361 	.word	0x20737361
3000ca04:	2050544f 	.word	0x2050544f
3000ca08:	6f747561 	.word	0x6f747561
3000ca0c:	64616f6c 	.word	0x64616f6c
3000ca10:	00000a0d 	.word	0x00000a0d
3000ca14:	444f4d5b 	.word	0x444f4d5b
3000ca18:	5f454c55 	.word	0x5f454c55
3000ca1c:	544f4f42 	.word	0x544f4f42
3000ca20:	56454c2d 	.word	0x56454c2d
3000ca24:	495f4c45 	.word	0x495f4c45
3000ca28:	5d4f464e 	.word	0x5d4f464e
3000ca2c:	20504e3a 	.word	0x20504e3a
3000ca30:	71657246 	.word	0x71657246
3000ca34:	20642520 	.word	0x20642520
3000ca38:	0a7a484d 	.word	0x0a7a484d
3000ca3c:	00000000 	.word	0x00000000
3000ca40:	444f4d5b 	.word	0x444f4d5b
3000ca44:	5f454c55 	.word	0x5f454c55
3000ca48:	544f4f42 	.word	0x544f4f42
3000ca4c:	56454c2d 	.word	0x56454c2d
3000ca50:	495f4c45 	.word	0x495f4c45
3000ca54:	5d4f464e 	.word	0x5d4f464e
3000ca58:	2050413a 	.word	0x2050413a
3000ca5c:	71657246 	.word	0x71657246
3000ca60:	20642520 	.word	0x20642520
3000ca64:	0a7a484d 	.word	0x0a7a484d
3000ca68:	00000000 	.word	0x00000000
3000ca6c:	7361700a 	.word	0x7361700a
3000ca70:	726f7773 	.word	0x726f7773
3000ca74:	65762064 	.word	0x65762064
3000ca78:	79666972 	.word	0x79666972
3000ca7c:	69616620 	.word	0x69616620
3000ca80:	00000a6c 	.word	0x00000a6c
3000ca84:	7361700a 	.word	0x7361700a
3000ca88:	726f7773 	.word	0x726f7773
3000ca8c:	65762064 	.word	0x65762064
3000ca90:	79666972 	.word	0x79666972
3000ca94:	63757320 	.word	0x63757320
3000ca98:	73736563 	.word	0x73736563
3000ca9c:	0000000a 	.word	0x0000000a
3000caa0:	746f6f62 	.word	0x746f6f62
3000caa4:	64616f6c 	.word	0x64616f6c
3000caa8:	765f7265 	.word	0x765f7265
3000caac:	69737265 	.word	0x69737265
3000cab0:	25206e6f 	.word	0x25206e6f
3000cab4:	00000a73 	.word	0x00000a73
3000cab8:	444f4d5b 	.word	0x444f4d5b
3000cabc:	5f454c55 	.word	0x5f454c55
3000cac0:	544f4f42 	.word	0x544f4f42
3000cac4:	56454c2d 	.word	0x56454c2d
3000cac8:	495f4c45 	.word	0x495f4c45
3000cacc:	5d4f464e 	.word	0x5d4f464e
3000cad0:	474d493a 	.word	0x474d493a
3000cad4:	4e452031 	.word	0x4e452031
3000cad8:	20524554 	.word	0x20524554
3000cadc:	3a50534d 	.word	0x3a50534d
3000cae0:	3830255b 	.word	0x3830255b
3000cae4:	000a5d78 	.word	0x000a5d78
3000cae8:	444f4d5b 	.word	0x444f4d5b
3000caec:	5f454c55 	.word	0x5f454c55
3000caf0:	544f4f42 	.word	0x544f4f42
3000caf4:	56454c2d 	.word	0x56454c2d
3000caf8:	495f4c45 	.word	0x495f4c45
3000cafc:	5d4f464e 	.word	0x5d4f464e
3000cb00:	474d493a 	.word	0x474d493a
3000cb04:	45532031 	.word	0x45532031
3000cb08:	45525543 	.word	0x45525543
3000cb0c:	41545320 	.word	0x41545320
3000cb10:	203a4554 	.word	0x203a4554
3000cb14:	000a6425 	.word	0x000a6425
3000cb18:	444f4d5b 	.word	0x444f4d5b
3000cb1c:	5f454c55 	.word	0x5f454c55
3000cb20:	544f4f42 	.word	0x544f4f42
3000cb24:	56454c2d 	.word	0x56454c2d
3000cb28:	495f4c45 	.word	0x495f4c45
3000cb2c:	5d4f464e 	.word	0x5d4f464e
3000cb30:	696e493a 	.word	0x696e493a
3000cb34:	53502074 	.word	0x53502074
3000cb38:	0d4d4152 	.word	0x0d4d4152
3000cb3c:	0000000a 	.word	0x0000000a
3000cb40:	7465730d 	.word	0x7465730d
3000cb44:	6d667020 	.word	0x6d667020
3000cb48:	69616620 	.word	0x69616620
3000cb4c:	000a0d6c 	.word	0x000a0d6c
3000cb50:	444f4d5b 	.word	0x444f4d5b
3000cb54:	5f454c55 	.word	0x5f454c55
3000cb58:	544f4f42 	.word	0x544f4f42
3000cb5c:	56454c2d 	.word	0x56454c2d
3000cb60:	495f4c45 	.word	0x495f4c45
3000cb64:	5d4f464e 	.word	0x5d4f464e
3000cb68:	5252453a 	.word	0x5252453a
3000cb6c:	2121524f 	.word	0x2121524f
3000cb70:	6f685320 	.word	0x6f685320
3000cb74:	20646c75 	.word	0x20646c75
3000cb78:	20746f4e 	.word	0x20746f4e
3000cb7c:	62616e65 	.word	0x62616e65
3000cb80:	4d20656c 	.word	0x4d20656c
3000cb84:	77536d65 	.word	0x77536d65
3000cb88:	6e4f5f72 	.word	0x6e4f5f72
3000cb8c:	6920796c 	.word	0x6920796c
3000cb90:	4444206e 	.word	0x4444206e
3000cb94:	68432052 	.word	0x68432052
3000cb98:	21217069 	.word	0x21217069
3000cb9c:	000a0d21 	.word	0x000a0d21
3000cba0:	444f4d5b 	.word	0x444f4d5b
3000cba4:	5f454c55 	.word	0x5f454c55
3000cba8:	544f4f42 	.word	0x544f4f42
3000cbac:	56454c2d 	.word	0x56454c2d
3000cbb0:	495f4c45 	.word	0x495f4c45
3000cbb4:	5d4f464e 	.word	0x5d4f464e
3000cbb8:	696e493a 	.word	0x696e493a
3000cbbc:	44442074 	.word	0x44442074
3000cbc0:	000a0d52 	.word	0x000a0d52
3000cbc4:	4848530a 	.word	0x4848530a
3000cbc8:	4946544f 	.word	0x4946544f
3000cbcc:	6f6e2058 	.word	0x6f6e2058
3000cbd0:	65722074 	.word	0x65722074
3000cbd4:	76696563 	.word	0x76696563
3000cbd8:	000a6465 	.word	0x000a6465
3000cbdc:	726f4e0a 	.word	0x726f4e0a
3000cbe0:	206c616d 	.word	0x206c616d
3000cbe4:	746f6f62 	.word	0x746f6f62
3000cbe8:	0000000a 	.word	0x0000000a
3000cbec:	5f6e6962 	.word	0x5f6e6962
3000cbf0:	313e6f6e 	.word	0x313e6f6e
3000cbf4:	3d202c30 	.word	0x3d202c30
3000cbf8:	0a642520 	.word	0x0a642520
3000cbfc:	00000000 	.word	0x00000000
3000cc00:	6e206164 	.word	0x6e206164
3000cc04:	7220746f 	.word	0x7220746f
3000cc08:	69656365 	.word	0x69656365
3000cc0c:	0a646576 	.word	0x0a646576
3000cc10:	00000000 	.word	0x00000000
3000cc14:	746f680a 	.word	0x746f680a
3000cc18:	20786966 	.word	0x20786966
3000cc1c:	656e6f64 	.word	0x656e6f64
3000cc20:	202e2e2e 	.word	0x202e2e2e
3000cc24:	6f626552 	.word	0x6f626552
3000cc28:	6e69746f 	.word	0x6e69746f
3000cc2c:	0a2e2e67 	.word	0x0a2e2e67
3000cc30:	00000000 	.word	0x00000000
3000cc34:	444f4d5b 	.word	0x444f4d5b
3000cc38:	5f454c55 	.word	0x5f454c55
3000cc3c:	544f4f42 	.word	0x544f4f42
3000cc40:	56454c2d 	.word	0x56454c2d
3000cc44:	495f4c45 	.word	0x495f4c45
3000cc48:	5d4f464e 	.word	0x5d4f464e
3000cc4c:	6174533a 	.word	0x6174533a
3000cc50:	4e207472 	.word	0x4e207472
3000cc54:	65536e6f 	.word	0x65536e6f
3000cc58:	65727563 	.word	0x65727563
3000cc5c:	30204020 	.word	0x30204020
3000cc60:	20782578 	.word	0x20782578
3000cc64:	0d2e2e2e 	.word	0x0d2e2e2e
3000cc68:	0000000a 	.word	0x0000000a
3000cc6c:	6c630a0d 	.word	0x6c630a0d
3000cc70:	2065736f 	.word	0x2065736f
3000cc74:	646f4d79 	.word	0x646f4d79
3000cc78:	54206d65 	.word	0x54206d65
3000cc7c:	736e6172 	.word	0x736e6172
3000cc80:	20726566 	.word	0x20726566
3000cc84:	0d2e2e2e 	.word	0x0d2e2e2e
3000cc88:	0000000a 	.word	0x0000000a
3000cc8c:	766e490d 	.word	0x766e490d
3000cc90:	64696c61 	.word	0x64696c61
3000cc94:	69684320 	.word	0x69684320
3000cc98:	666e696e 	.word	0x666e696e
3000cc9c:	4320216f 	.word	0x4320216f
3000cca0:	6b636568 	.word	0x6b636568
3000cca4:	50544f20 	.word	0x50544f20
3000cca8:	0000000a 	.word	0x0000000a
3000ccac:	656c500d 	.word	0x656c500d
3000ccb0:	20657361 	.word	0x20657361
3000ccb4:	676f7270 	.word	0x676f7270
3000ccb8:	206d6172 	.word	0x206d6172
3000ccbc:	70696863 	.word	0x70696863
3000ccc0:	6f666e69 	.word	0x6f666e69
3000ccc4:	206e6920 	.word	0x206e6920
3000ccc8:	2050544f 	.word	0x2050544f
3000cccc:	00000a21 	.word	0x00000a21

3000ccd0 <Memory_Info>:
3000ccd0:	03e00007 00060004 010202c0 02010100     ................
3000cce0:	02000102 00040302 02040400 01010102     ................
3000ccf0:	01020221 02240401 03000102 00030103     !.....$.........
3000cd00:	01050500 06000002 01020206 01070700     ................
3000cd10:	08000002 01020208 02280801 09000102     ..........(.....
3000cd20:	00040309 03290901 0a000004 0003010a     ......).........
3000cd30:	012a0a01 0a010003 0003012a 020b0b00     ..*.....*.......
3000cd40:	0c000102 0102020c ffffffff 0000ffff     ................
3000cd50:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000cd60:	455f4c45 524f5252 52443a5d 69204d41     EL_ERROR]:DRAM i
3000cd70:	6f6e2073 61432074 7262696c 6e6f6961     s not Calibraion
3000cd80:	00000a0d                                ....

3000cd84 <Flash_AVL>:
3000cd84:	000000ef 000000ff 00000000 000043fc     .............C..
3000cd94:	00000000 000000a1 000000ff 00000000     ................
3000cda4:	0000fffc 00000000 0000000b 000000ff     ................
3000cdb4:	00000000 000043fc 00000000 0000000e     .....C..........
3000cdc4:	000000ff 00000000 000043fc 00000000     .........C......
3000cdd4:	000000c8 000000ff 00000001 000043fc     .............C..
3000cde4:	00000000 000028c2 0000ffff 00000005     .....(..........
3000cdf4:	000200fc 00000000 000000c2 000000ff     ................
3000ce04:	00000002 000000fc 00000000 00000068     ............h...
3000ce14:	000000ff 00000002 000000fc 00000000     ................
3000ce24:	00000051 000000ff 00000002 000000fc     Q...............
3000ce34:	00000000 0000001c 000000ff 00000003     ................
3000ce44:	000000fc 00000000 00000020 000000ff     ........ .......
3000ce54:	00000004 000000fc 00000000 00000000     ................
3000ce64:	000000ff 000000fe ffffffff 30008a2d     ............-..0
3000ce74:	000000ff ffffffff 000000ff ffffffff     ................
3000ce84:	00000000                                ....

3000ce88 <Flash_ReadMode>:
3000ce88:	                                         ..

3000ce8a <Flash_Speed>:
3000ce8a:	                                         ..

3000ce8c <NAND_AVL>:
3000ce8c:	000000ef 000000ff 00000000 ffffffff     ................
3000ce9c:	00000000 000000c8 000000ff 00000001     ................
3000ceac:	ffffffff 00000000 000000e5 000000ff     ................
3000cebc:	00000001 ffffffff 00000000 000000c2     ................
3000cecc:	000000ff 00000002 ffffffff 00000000     ................
3000cedc:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000cef4:	000000ff 000000fe ffffffff 30008a8d     ...............0
3000cf04:	000000ff ffffffff 000000ff ffffffff     ................
3000cf14:	00000000 3000be60 3000be64 3000be68     ....`..0d..0h..0
3000cf24:	3000be6c 3000be70                       l..0p..0

3000cf2c <__FUNCTION__.0>:
3000cf2c:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000cf3c <__FUNCTION__.1>:
3000cf3c:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000cf4c:	00000000 444f4d5b 5f454c55 544f4f42     ....[MODULE_BOOT
3000cf5c:	56454c2d 455f4c45 524f5252 6c423a5d     -LEVEL_ERROR]:Bl
3000cf6c:	7830206b 69207825 42422073 00000a21     k 0x%x is BB!...
3000cf7c:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000cf8c:	495f4c45 5d4f464e 6165523a 61502064     EL_INFO]:Read Pa
3000cf9c:	30206567 20782578 4220666f 6b636f6c     ge 0x%x of Block
3000cfac:	25783020 614d2078 61462079 202c6c69      0x%x May Fail, 
3000cfbc:	74617473 30207375 21782578 0000000a     status 0x%x!....
3000cfcc:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000cfdc:	52455f4c 5d524f52 50544f3a 6165525f     L_ERROR]:OTP_Rea
3000cfec:	46203864 206c6961 0a207825 00000000     d8 Fail %x .....

3000cffc <__FUNCTION__.1>:
3000cffc:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d00c:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d01c:	495f4c45 5d4f464e 5253503a 43204d41     EL_INFO]:PSRAM C
3000d02c:	206c7274 3a4b4c43 20642520 0a207a48     trl CLK: %d Hz .
3000d03c:	00000000 6c61430d 6e696d4e 25203d20     .....CalNmin = %
3000d04c:	61432078 616d4e6c 203d2078 57207825     x CalNmax = %x W
3000d05c:	6f646e69 7a695377 203d2065 70207825     indowSize = %x p
3000d06c:	65736168 7825203a 00000a20 444f4d5b     hase: %x ...[MOD
3000d07c:	5f454c55 45535049 454c2d43 5f4c4556     ULE_IPSEC-LEVEL_
3000d08c:	4f525245 4e3a5d52 6874206f 61207369     ERROR]:No this a
3000d09c:	5f687475 65707974 29642528 726f6620     uth_type(%d) for
3000d0ac:	41485320 00000a32 7172690d 5d64255b      SHA2....irq[%d]
3000d0bc:	69727020 7469726f 64252079 61687320      priority %d sha
3000d0cc:	3c206c6c 6425203d 0000000a              ll <= %d....

3000d0d8 <__FUNCTION__.0>:
3000d0d8:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d0e8:	00000000                                ....

3000d0ec <__FUNCTION__.1>:
3000d0ec:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d0fc <__FUNCTION__.2>:
3000d0fc:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d10c:	32616873 696e695f 00000074              sha2_init...

3000d118 <__FUNCTION__.3>:
3000d118:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d128:	6f4d7974 44416564 00000000 5244440d     tyModeAD.....DDR
3000d138:	7a695320 73692065 746f6e20 6e6f6320      Size is not con
3000d148:	75676966 0a646572 00000000 5b203e0d     figured......> [
3000d158:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d168:	63203a5d 5f756c6f 616d6572 69625f70     ]: colu_remap_bi
3000d178:	21207374 7264203d 635f6d61 5f756c6f     ts != dram_colu_
3000d188:	73746962 0a212121 00000000 5b203e0d     bits!!!......> [
3000d198:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d1a8:	62203a5d 5f6b6e61 616d6572 69625f70     ]: bank_remap_bi
3000d1b8:	21207374 7264203d 625f6d61 5f6b6e61     ts != dram_bank_
3000d1c8:	73746962 0a212121 00000000              bits!!!.....

3000d1d4 <__FUNCTION__.1>:
3000d1d4:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d1e4:	00000000 3000c134 3000c140 3000c14c     ....4..0@..0L..0
3000d1f4:	3000c158 3000c164 3000c170 3000c17c     X..0d..0p..0|..0
3000d204:	3000c188 3000c194 3000c17c 3000c188     ...0...0|..0...0
3000d214:	3000c374 3000c380 3000c388 3000c390     t..0...0...0...0
3000d224:	3000c378 3000c398 00000208 0000020c     x..0...0........
3000d234:	00000210 00000008 00000214 00000218     ................
3000d244:	0000021c 0000000c                       ........

3000d24c <__FUNCTION__.0>:
3000d24c:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000d25c <km4_bootloader_rev>:
3000d25c:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000d26c:	635f7265 34393339 325f6362 2f333230     er_c9394bc_2023/
3000d27c:	312f3830 39302d38 3a31323a 00003531     08/18-09:21:15..
