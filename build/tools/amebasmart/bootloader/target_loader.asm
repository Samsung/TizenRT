
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
30003272:	f008 ff5d 	bl	3000c130 <__DiagPrintf_veneer>
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
300032ea:	f008 ff21 	bl	3000c130 <__DiagPrintf_veneer>
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
30003316:	f008 ff0b 	bl	3000c130 <__DiagPrintf_veneer>
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
30003342:	f008 fef5 	bl	3000c130 <__DiagPrintf_veneer>
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
3000335c:	f008 fee8 	bl	3000c130 <__DiagPrintf_veneer>
30003360:	e7be      	b.n	300032e0 <FLASH_CalibrationNew+0x168>
30003362:	480e      	ldr	r0, [pc, #56]	; (3000339c <FLASH_CalibrationNew+0x224>)
30003364:	f008 fee4 	bl	3000c130 <__DiagPrintf_veneer>
30003368:	e795      	b.n	30003296 <FLASH_CalibrationNew+0x11e>
3000336a:	bf00      	nop
3000336c:	3000a479 	.word	0x3000a479
30003370:	2001c01c 	.word	0x2001c01c
30003374:	00009c99 	.word	0x00009c99
30003378:	e000ed00 	.word	0xe000ed00
3000337c:	2001c00c 	.word	0x2001c00c
30003380:	00054060 	.word	0x00054060
30003384:	000129bd 	.word	0x000129bd
30003388:	3000c1d0 	.word	0x3000c1d0
3000338c:	3000c1a4 	.word	0x3000c1a4
30003390:	0000b7e9 	.word	0x0000b7e9
30003394:	ffff0000 	.word	0xffff0000
30003398:	3000c1f4 	.word	0x3000c1f4
3000339c:	3000c230 	.word	0x3000c230
300033a0:	3000a289 	.word	0x3000a289

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
300034c6:	f008 fe33 	bl	3000c130 <__DiagPrintf_veneer>
300034ca:	e7db      	b.n	30003484 <FLASH_Calibration+0xe0>
300034cc:	4652      	mov	r2, sl
300034ce:	9b02      	ldr	r3, [sp, #8]
300034d0:	480b      	ldr	r0, [pc, #44]	; (30003500 <FLASH_Calibration+0x15c>)
300034d2:	9105      	str	r1, [sp, #20]
300034d4:	f008 fe2c 	bl	3000c130 <__DiagPrintf_veneer>
300034d8:	9905      	ldr	r1, [sp, #20]
300034da:	e7b7      	b.n	3000344c <FLASH_Calibration+0xa8>
300034dc:	9600      	str	r6, [sp, #0]
300034de:	e7d1      	b.n	30003484 <FLASH_Calibration+0xe0>
300034e0:	3000a479 	.word	0x3000a479
300034e4:	2001c01c 	.word	0x2001c01c
300034e8:	00009c99 	.word	0x00009c99
300034ec:	e000ed00 	.word	0xe000ed00
300034f0:	3000a289 	.word	0x3000a289
300034f4:	0000b7e9 	.word	0x0000b7e9
300034f8:	2001c00c 	.word	0x2001c00c
300034fc:	3000c28c 	.word	0x3000c28c
30003500:	3000c25c 	.word	0x3000c25c
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
3000359c:	f008 fdc8 	bl	3000c130 <__DiagPrintf_veneer>
300035a0:	e7ec      	b.n	3000357c <_flash_calibration_highspeed+0x74>
300035a2:	bf00      	nop
300035a4:	2001c01c 	.word	0x2001c01c
300035a8:	2001c00c 	.word	0x2001c00c
300035ac:	3000c2b8 	.word	0x3000c2b8

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
300035e4:	f008 fda4 	bl	3000c130 <__DiagPrintf_veneer>
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
3000362a:	f008 fd81 	bl	3000c130 <__DiagPrintf_veneer>
3000362e:	2001      	movs	r0, #1
30003630:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003632:	bf00      	nop
30003634:	2001c01c 	.word	0x2001c01c
30003638:	0000b569 	.word	0x0000b569
3000363c:	2001c00c 	.word	0x2001c00c
30003640:	3000c320 	.word	0x3000c320
30003644:	3000a479 	.word	0x3000a479
30003648:	0000c045 	.word	0x0000c045
3000364c:	0000a53d 	.word	0x0000a53d
30003650:	3000c2f0 	.word	0x3000c2f0

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
3000375a:	f008 fce9 	bl	3000c130 <__DiagPrintf_veneer>
3000375e:	e7cd      	b.n	300036fc <flash_rx_mode_switch+0xa8>
30003760:	469c      	mov	ip, r3
30003762:	f1bc 0f05 	cmp.w	ip, #5
30003766:	d10f      	bne.n	30003788 <flash_rx_mode_switch+0x134>
30003768:	4b12      	ldr	r3, [pc, #72]	; (300037b4 <flash_rx_mode_switch+0x160>)
3000376a:	6818      	ldr	r0, [r3, #0]
3000376c:	f010 0002 	ands.w	r0, r0, #2
30003770:	d00b      	beq.n	3000378a <flash_rx_mode_switch+0x136>
30003772:	4815      	ldr	r0, [pc, #84]	; (300037c8 <flash_rx_mode_switch+0x174>)
30003774:	f008 fcdc 	bl	3000c130 <__DiagPrintf_veneer>
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
3000379c:	f008 fcc8 	bl	3000c130 <__DiagPrintf_veneer>
300037a0:	e7f2      	b.n	30003788 <flash_rx_mode_switch+0x134>
300037a2:	bf00      	nop
300037a4:	3000d2a4 	.word	0x3000d2a4
300037a8:	2001c01c 	.word	0x2001c01c
300037ac:	e000ed00 	.word	0xe000ed00
300037b0:	00054060 	.word	0x00054060
300037b4:	2001c00c 	.word	0x2001c00c
300037b8:	0000c045 	.word	0x0000c045
300037bc:	00009db5 	.word	0x00009db5
300037c0:	0000a149 	.word	0x0000a149
300037c4:	3000c37c 	.word	0x3000c37c
300037c8:	3000c3ac 	.word	0x3000c3ac
300037cc:	3000c354 	.word	0x3000c354
300037d0:	3000a479 	.word	0x3000a479
300037d4:	3000a289 	.word	0x3000a289
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
300038a4:	f008 fc64 	bl	3000c170 <__io_assert_failed_veneer>
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
30003920:	f008 fc26 	bl	3000c170 <__io_assert_failed_veneer>
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
30003a4a:	f008 fb71 	bl	3000c130 <__DiagPrintf_veneer>
30003a4e:	e6ff      	b.n	30003850 <flash_highspeed_setup+0x70>
30003a50:	f89d 200d 	ldrb.w	r2, [sp, #13]
30003a54:	f89d 100c 	ldrb.w	r1, [sp, #12]
30003a58:	481c      	ldr	r0, [pc, #112]	; (30003acc <flash_highspeed_setup+0x2ec>)
30003a5a:	f008 fb69 	bl	3000c130 <__DiagPrintf_veneer>
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
30003a8c:	3000d214 	.word	0x3000d214
30003a90:	3000d216 	.word	0x3000d216
30003a94:	2001c01c 	.word	0x2001c01c
30003a98:	2001c00c 	.word	0x2001c00c
30003a9c:	3000d110 	.word	0x3000d110
30003aa0:	3000d218 	.word	0x3000d218
30003aa4:	3000d2c8 	.word	0x3000d2c8
30003aa8:	0000b8c1 	.word	0x0000b8c1
30003aac:	3000d2b8 	.word	0x3000d2b8
30003ab0:	0000bd0d 	.word	0x0000bd0d
30003ab4:	0000c0f9 	.word	0x0000c0f9
30003ab8:	0000c1d9 	.word	0x0000c1d9
30003abc:	3000e9e0 	.word	0x3000e9e0
30003ac0:	000129bd 	.word	0x000129bd
30003ac4:	0000a149 	.word	0x0000a149
30003ac8:	3000c3e4 	.word	0x3000c3e4
30003acc:	3000c438 	.word	0x3000c438
30003ad0:	0000be71 	.word	0x0000be71
30003ad4:	0000bee9 	.word	0x0000bee9
30003ad8:	0000bfcd 	.word	0x0000bfcd
30003adc:	3000a479 	.word	0x3000a479
30003ae0:	00009db5 	.word	0x00009db5
30003ae4:	3000e9e4 	.word	0x3000e9e4
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
30003b6a:	f008 fb01 	bl	3000c170 <__io_assert_failed_veneer>
30003b6e:	f8d9 3000 	ldr.w	r3, [r9]
30003b72:	691b      	ldr	r3, [r3, #16]
30003b74:	e6e1      	b.n	3000393a <flash_highspeed_setup+0x15a>
30003b76:	f240 2149 	movw	r1, #585	; 0x249
30003b7a:	4812      	ldr	r0, [pc, #72]	; (30003bc4 <flash_highspeed_setup+0x3e4>)
30003b7c:	f008 faf8 	bl	3000c170 <__io_assert_failed_veneer>
30003b80:	f8d9 3000 	ldr.w	r3, [r9]
30003b84:	691b      	ldr	r3, [r3, #16]
30003b86:	e70f      	b.n	300039a8 <flash_highspeed_setup+0x1c8>
30003b88:	480f      	ldr	r0, [pc, #60]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b8a:	f008 fad1 	bl	3000c130 <__DiagPrintf_veneer>
30003b8e:	e686      	b.n	3000389e <flash_highspeed_setup+0xbe>
30003b90:	480d      	ldr	r0, [pc, #52]	; (30003bc8 <flash_highspeed_setup+0x3e8>)
30003b92:	f008 facd 	bl	3000c130 <__DiagPrintf_veneer>
30003b96:	e6c0      	b.n	3000391a <flash_highspeed_setup+0x13a>
30003b98:	4b0c      	ldr	r3, [pc, #48]	; (30003bcc <flash_highspeed_setup+0x3ec>)
30003b9a:	4798      	blx	r3
30003b9c:	e705      	b.n	300039aa <flash_highspeed_setup+0x1ca>
30003b9e:	bf00      	nop
30003ba0:	2001c01c 	.word	0x2001c01c
30003ba4:	0000bf59 	.word	0x0000bf59
30003ba8:	0000a3c5 	.word	0x0000a3c5
30003bac:	0000a441 	.word	0x0000a441
30003bb0:	3000e9e0 	.word	0x3000e9e0
30003bb4:	0000a345 	.word	0x0000a345
30003bb8:	0000b991 	.word	0x0000b991
30003bbc:	0000bd0d 	.word	0x0000bd0d
30003bc0:	3000d2b8 	.word	0x3000d2b8
30003bc4:	3000d2c8 	.word	0x3000d2c8
30003bc8:	3000c414 	.word	0x3000c414
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
30003ca2:	f008 fa45 	bl	3000c130 <__DiagPrintf_veneer>
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
30003d0e:	f008 fa0f 	bl	3000c130 <__DiagPrintf_veneer>
30003d12:	2000      	movs	r0, #0
30003d14:	e7cc      	b.n	30003cb0 <BOOT_LoadSubImage+0xe0>
30003d16:	bf00      	nop
30003d18:	31313738 	.word	0x31313738
30003d1c:	f7ffffe0 	.word	0xf7ffffe0
30003d20:	2001c00c 	.word	0x2001c00c
30003d24:	3000e1e0 	.word	0x3000e1e0
30003d28:	e000ed00 	.word	0xe000ed00
30003d2c:	3000c4f4 	.word	0x3000c4f4
30003d30:	3000c4cc 	.word	0x3000c4cc
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
30003d94:	f008 f9cc 	bl	3000c130 <__DiagPrintf_veneer>
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
30003f66:	f008 f8e3 	bl	3000c130 <__DiagPrintf_veneer>
30003f6a:	e744      	b.n	30003df6 <BOOT_LoadAPSubImage+0xbe>
30003f6c:	482b      	ldr	r0, [pc, #172]	; (3000401c <BOOT_LoadAPSubImage+0x2e4>)
30003f6e:	f008 f8df 	bl	3000c130 <__DiagPrintf_veneer>
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
30003fb4:	f008 f8bc 	bl	3000c130 <__DiagPrintf_veneer>
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
30003ff8:	3000c4c4 	.word	0x3000c4c4
30003ffc:	35393138 	.word	0x35393138
30004000:	2001c00c 	.word	0x2001c00c
30004004:	3000c4cc 	.word	0x3000c4cc
30004008:	f7ffffe0 	.word	0xf7ffffe0
3000400c:	0000c109 	.word	0x0000c109
30004010:	00009be5 	.word	0x00009be5
30004014:	e000ed00 	.word	0xe000ed00
30004018:	3000c4f4 	.word	0x3000c4f4
3000401c:	3000c51c 	.word	0x3000c51c
30004020:	3000c540 	.word	0x3000c540

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
30004058:	3000a171 	.word	0x3000a171

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
300040ca:	f008 b831 	b.w	3000c130 <__DiagPrintf_veneer>
300040ce:	4808      	ldr	r0, [pc, #32]	; (300040f0 <BOOT_OTFCheck+0x94>)
300040d0:	f008 f82e 	bl	3000c130 <__DiagPrintf_veneer>
300040d4:	e7da      	b.n	3000408c <BOOT_OTFCheck+0x30>
300040d6:	bf00      	nop
300040d8:	0000c129 	.word	0x0000c129
300040dc:	2001c00c 	.word	0x2001c00c
300040e0:	0000c245 	.word	0x0000c245
300040e4:	3000a1a5 	.word	0x3000a1a5
300040e8:	3000a18d 	.word	0x3000a18d
300040ec:	3000c584 	.word	0x3000c584
300040f0:	3000c574 	.word	0x3000c574

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
3000427a:	f007 ff59 	bl	3000c130 <__DiagPrintf_veneer>
3000427e:	b11d      	cbz	r5, 30004288 <BOOT_OTA_LoadIMG2+0x194>
30004280:	481c      	ldr	r0, [pc, #112]	; (300042f4 <BOOT_OTA_LoadIMG2+0x200>)
30004282:	f007 ff55 	bl	3000c130 <__DiagPrintf_veneer>
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
300042a8:	f007 ff42 	bl	3000c130 <__DiagPrintf_veneer>
300042ac:	4620      	mov	r0, r4
300042ae:	b00d      	add	sp, #52	; 0x34
300042b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300042b4:	3000d62c 	.word	0x3000d62c
300042b8:	3000ed70 	.word	0x3000ed70
300042bc:	3000ea14 	.word	0x3000ea14
300042c0:	3000eb78 	.word	0x3000eb78
300042c4:	0000c129 	.word	0x0000c129
300042c8:	0cffffe0 	.word	0x0cffffe0
300042cc:	3000a221 	.word	0x3000a221
300042d0:	3000a261 	.word	0x3000a261
300042d4:	3000ed20 	.word	0x3000ed20
300042d8:	fffff000 	.word	0xfffff000
300042dc:	0effffe0 	.word	0x0effffe0
300042e0:	3000e1e0 	.word	0x3000e1e0
300042e4:	2001c00c 	.word	0x2001c00c
300042e8:	3000c5a0 	.word	0x3000c5a0
300042ec:	3000c598 	.word	0x3000c598
300042f0:	3000c5d0 	.word	0x3000c5d0
300042f4:	3000c604 	.word	0x3000c604
300042f8:	3000eb98 	.word	0x3000eb98
300042fc:	00012a1d 	.word	0x00012a1d
30004300:	3000a171 	.word	0x3000a171
30004304:	3000c5a8 	.word	0x3000c5a8
30004308:	0c000000 	.word	0x0c000000
3000430c:	3000a479 	.word	0x3000a479
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
300043a8:	3000a479 	.word	0x3000a479
300043ac:	3000e308 	.word	0x3000e308
300043b0:	3000e380 	.word	0x3000e380
300043b4:	3000ed08 	.word	0x3000ed08

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
3000444e:	f007 fe6f 	bl	3000c130 <__DiagPrintf_veneer>
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
30004470:	f007 fe5e 	bl	3000c130 <__DiagPrintf_veneer>
30004474:	4628      	mov	r0, r5
30004476:	b06d      	add	sp, #436	; 0x1b4
30004478:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000447c:	4809      	ldr	r0, [pc, #36]	; (300044a4 <BOOT_OTA_AP+0xe8>)
3000447e:	25ff      	movs	r5, #255	; 0xff
30004480:	f007 fe56 	bl	3000c130 <__DiagPrintf_veneer>
30004484:	e7dd      	b.n	30004442 <BOOT_OTA_AP+0x86>
30004486:	bf00      	nop
30004488:	3000d644 	.word	0x3000d644
3000448c:	3000e1e0 	.word	0x3000e1e0
30004490:	3000ed20 	.word	0x3000ed20
30004494:	3000ea14 	.word	0x3000ea14
30004498:	3000c64c 	.word	0x3000c64c
3000449c:	2001c00c 	.word	0x2001c00c
300044a0:	3000c628 	.word	0x3000c628
300044a4:	3000c670 	.word	0x3000c670

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
30004500:	f007 fe16 	bl	3000c130 <__DiagPrintf_veneer>
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
3000451c:	f007 be08 	b.w	3000c130 <__DiagPrintf_veneer>
30004520:	3000d6f0 	.word	0x3000d6f0
30004524:	2001c00c 	.word	0x2001c00c
30004528:	3000a1a5 	.word	0x3000a1a5
3000452c:	3000c6d4 	.word	0x3000c6d4
30004530:	3000c71c 	.word	0x3000c71c

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
3000464c:	f007 fd70 	bl	3000c130 <__DiagPrintf_veneer>
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
3000466e:	f007 fd5f 	bl	3000c130 <__DiagPrintf_veneer>
30004672:	2000      	movs	r0, #0
30004674:	e7dd      	b.n	30004632 <BOOT_LoadRDPImg+0xfe>
30004676:	4618      	mov	r0, r3
30004678:	e7f2      	b.n	30004660 <BOOT_LoadRDPImg+0x12c>
3000467a:	bf00      	nop
3000467c:	3000d658 	.word	0x3000d658
30004680:	00002429 	.word	0x00002429
30004684:	00002479 	.word	0x00002479
30004688:	00004641 	.word	0x00004641
3000468c:	35393138 	.word	0x35393138
30004690:	2001c00c 	.word	0x2001c00c
30004694:	31313738 	.word	0x31313738
30004698:	3000c4f4 	.word	0x3000c4f4
3000469c:	3000c4cc 	.word	0x3000c4cc
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
3000474a:	f007 fcf1 	bl	3000c130 <__DiagPrintf_veneer>
3000474e:	e7d0      	b.n	300046f2 <BOOT_LoadBL32RDPImg+0x4a>
30004750:	3000c754 	.word	0x3000c754
30004754:	00004641 	.word	0x00004641
30004758:	2001c00c 	.word	0x2001c00c
3000475c:	3000c4f4 	.word	0x3000c4f4
30004760:	3000c768 	.word	0x3000c768
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
3000478e:	f007 fcaf 	bl	3000c0f0 <____wrap_memset_veneer>
30004792:	4628      	mov	r0, r5
30004794:	f10d 021f 	add.w	r2, sp, #31
30004798:	f10d 011e 	add.w	r1, sp, #30
3000479c:	f007 fce0 	bl	3000c160 <__SBOOT_GetMdType_veneer>
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
300049b8:	30009de9 	.word	0x30009de9
300049bc:	e000ed00 	.word	0xe000ed00
300049c0:	00005bb9 	.word	0x00005bb9
300049c4:	0000ea4d 	.word	0x0000ea4d
300049c8:	00004ffd 	.word	0x00004ffd
300049cc:	000053f1 	.word	0x000053f1
300049d0:	00005b4d 	.word	0x00005b4d
300049d4:	00005385 	.word	0x00005385

300049d8 <BOOT_Validate_PubKey>:
300049d8:	f007 bbd2 	b.w	3000c180 <__SBOOT_Validate_PubKey_veneer>

300049dc <BOOT_Validate_Signature>:
300049dc:	f007 bbb0 	b.w	3000c140 <__SBOOT_Validate_Signature_veneer>

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
30004a00:	f007 bb8e 	b.w	3000c120 <__SBOOT_Validate_ImgHash_veneer>
30004a04:	4620      	mov	r0, r4
30004a06:	b005      	add	sp, #20
30004a08:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004a0c:	f7ff beb2 	b.w	30004774 <_BOOT_Validate_ImgHash>
30004a10:	3000a479 	.word	0x3000a479

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
30004a54:	f007 fb7c 	bl	3000c150 <__SBOOT_Validate_Algorithm_veneer>
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
30004a8e:	f007 fb77 	bl	3000c180 <__SBOOT_Validate_PubKey_veneer>
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
30004b96:	f007 fad3 	bl	3000c140 <__SBOOT_Validate_Signature_veneer>
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
30004be4:	f007 faa4 	bl	3000c130 <__DiagPrintf_veneer>
30004be8:	9a03      	ldr	r2, [sp, #12]
30004bea:	4610      	mov	r0, r2
30004bec:	e724      	b.n	30004a38 <BOOT_SignatureCheck+0x24>
30004bee:	a90a      	add	r1, sp, #40	; 0x28
30004bf0:	f898 3011 	ldrb.w	r3, [r8, #17]
30004bf4:	4810      	ldr	r0, [pc, #64]	; (30004c38 <BOOT_SignatureCheck+0x224>)
30004bf6:	eb01 0383 	add.w	r3, r1, r3, lsl #2
30004bfa:	f853 1c10 	ldr.w	r1, [r3, #-16]
30004bfe:	f007 fa97 	bl	3000c130 <__DiagPrintf_veneer>
30004c02:	e750      	b.n	30004aa6 <BOOT_SignatureCheck+0x92>
30004c04:	464a      	mov	r2, r9
30004c06:	4620      	mov	r0, r4
30004c08:	463b      	mov	r3, r7
30004c0a:	f007 fa89 	bl	3000c120 <__SBOOT_Validate_ImgHash_veneer>
30004c0e:	4602      	mov	r2, r0
30004c10:	e7d5      	b.n	30004bbe <BOOT_SignatureCheck+0x1aa>
30004c12:	22b4      	movs	r2, #180	; 0xb4
30004c14:	e735      	b.n	30004a82 <BOOT_SignatureCheck+0x6e>
30004c16:	bf00      	nop
30004c18:	3000d660 	.word	0x3000d660
30004c1c:	0000c149 	.word	0x0000c149
30004c20:	00002451 	.word	0x00002451
30004c24:	2001c00c 	.word	0x2001c00c
30004c28:	00012be5 	.word	0x00012be5
30004c2c:	e000ed00 	.word	0xe000ed00
30004c30:	3000a479 	.word	0x3000a479
30004c34:	3000c770 	.word	0x3000c770
30004c38:	3000c784 	.word	0x3000c784

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
30004c9c:	f007 fa58 	bl	3000c150 <__SBOOT_Validate_Algorithm_veneer>
30004ca0:	b940      	cbnz	r0, 30004cb4 <BOOT_CertificateCheck+0x78>
30004ca2:	f104 0520 	add.w	r5, r4, #32
30004ca6:	aa04      	add	r2, sp, #16
30004ca8:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004cac:	4629      	mov	r1, r5
30004cae:	f007 fa67 	bl	3000c180 <__SBOOT_Validate_PubKey_veneer>
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
30004cd6:	f007 fa33 	bl	3000c140 <__SBOOT_Validate_Signature_veneer>
30004cda:	2800      	cmp	r0, #0
30004cdc:	d0b5      	beq.n	30004c4a <BOOT_CertificateCheck+0xe>
30004cde:	e7e9      	b.n	30004cb4 <BOOT_CertificateCheck+0x78>
30004ce0:	4601      	mov	r1, r0
30004ce2:	4807      	ldr	r0, [pc, #28]	; (30004d00 <BOOT_CertificateCheck+0xc4>)
30004ce4:	f007 fa24 	bl	3000c130 <__DiagPrintf_veneer>
30004ce8:	20ff      	movs	r0, #255	; 0xff
30004cea:	e7af      	b.n	30004c4c <BOOT_CertificateCheck+0x10>
30004cec:	0000c149 	.word	0x0000c149
30004cf0:	0000ea4d 	.word	0x0000ea4d
30004cf4:	3000d6e0 	.word	0x3000d6e0
30004cf8:	00002451 	.word	0x00002451
30004cfc:	2001c00c 	.word	0x2001c00c
30004d00:	3000c7a0 	.word	0x3000c7a0

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
30004e6c:	3000dbac 	.word	0x3000dbac
30004e70:	3000db4c 	.word	0x3000db4c
30004e74:	51001b00 	.word	0x51001b00
30004e78:	3000dc0c 	.word	0x3000dc0c
30004e7c:	e000ed00 	.word	0xe000ed00
30004e80:	42008000 	.word	0x42008000
30004e84:	05fa6000 	.word	0x05fa6000
30004e88:	e000e100 	.word	0xe000e100
30004e8c:	3000d6da 	.word	0x3000d6da
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
30004ef8:	f002 feae 	bl	30007c58 <ymodem_uart_getdata>
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
300051a8:	3000dc6c 	.word	0x3000dc6c

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
300051ce:	f006 ff87 	bl	3000c0e0 <____wrap_memcpy_veneer>
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
30005332:	f006 fed5 	bl	3000c0e0 <____wrap_memcpy_veneer>
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
3000534e:	f006 feef 	bl	3000c130 <__DiagPrintf_veneer>
30005352:	e7eb      	b.n	3000532c <rewrite_bp+0x180>
30005354:	f8d5 1006 	ldr.w	r1, [r5, #6]
30005358:	480a      	ldr	r0, [pc, #40]	; (30005384 <rewrite_bp+0x1d8>)
3000535a:	f006 fee9 	bl	3000c130 <__DiagPrintf_veneer>
3000535e:	2001      	movs	r0, #1
30005360:	e7b3      	b.n	300052ca <rewrite_bp+0x11e>
30005362:	f8d6 1006 	ldr.w	r1, [r6, #6]
30005366:	4808      	ldr	r0, [pc, #32]	; (30005388 <rewrite_bp+0x1dc>)
30005368:	f006 fee2 	bl	3000c130 <__DiagPrintf_veneer>
3000536c:	e7ac      	b.n	300052c8 <rewrite_bp+0x11c>
3000536e:	bf00      	nop
30005370:	3000dc6c 	.word	0x3000dc6c
30005374:	2001c01c 	.word	0x2001c01c
30005378:	00009db5 	.word	0x00009db5
3000537c:	2001c00c 	.word	0x2001c00c
30005380:	3000c824 	.word	0x3000c824
30005384:	3000c7c4 	.word	0x3000c7c4
30005388:	3000c7f4 	.word	0x3000c7f4

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
300054f8:	3000d670 	.word	0x3000d670
300054fc:	3000d7c4 	.word	0x3000d7c4
30005500:	42008000 	.word	0x42008000
30005504:	00009b2d 	.word	0x00009b2d
30005508:	30007f61 	.word	0x30007f61

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
30005560:	3000e2d0 	.word	0x3000e2d0
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
3000583c:	3000e20c 	.word	0x3000e20c
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
300058a0:	3000d7a4 	.word	0x3000d7a4
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
3000592c:	30007fd9 	.word	0x30007fd9
30005930:	300089b1 	.word	0x300089b1
30005934:	3000b8c5 	.word	0x3000b8c5
30005938:	30008a81 	.word	0x30008a81
3000593c:	30008a85 	.word	0x30008a85
30005940:	3000b86d 	.word	0x3000b86d

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
300059b6:	f006 fbbb 	bl	3000c130 <__DiagPrintf_veneer>
300059ba:	e7dc      	b.n	30005976 <BOOT_PSRAM_Init+0x32>
300059bc:	4b0e      	ldr	r3, [pc, #56]	; (300059f8 <BOOT_PSRAM_Init+0xb4>)
300059be:	4798      	blx	r3
300059c0:	e7db      	b.n	3000597a <BOOT_PSRAM_Init+0x36>
300059c2:	bf00      	nop
300059c4:	42008000 	.word	0x42008000
300059c8:	300095d5 	.word	0x300095d5
300059cc:	300095f5 	.word	0x300095f5
300059d0:	3000965d 	.word	0x3000965d
300059d4:	30007f15 	.word	0x30007f15
300059d8:	2001c00c 	.word	0x2001c00c
300059dc:	30009901 	.word	0x30009901
300059e0:	3000ede0 	.word	0x3000ede0
300059e4:	41002000 	.word	0x41002000
300059e8:	3000e9f0 	.word	0x3000e9f0
300059ec:	30009be5 	.word	0x30009be5
300059f0:	3000993d 	.word	0x3000993d
300059f4:	3000c85c 	.word	0x3000c85c
300059f8:	30009829 	.word	0x30009829

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
30005b24:	f006 fb04 	bl	3000c130 <__DiagPrintf_veneer>
30005b28:	e7bb      	b.n	30005aa2 <BOOT_WakeFromPG+0xa6>
30005b2a:	bf00      	nop
30005b2c:	3000ed74 	.word	0x3000ed74
30005b30:	e000ed00 	.word	0xe000ed00
30005b34:	3000ed8f 	.word	0x3000ed8f
30005b38:	e000e100 	.word	0xe000e100
30005b3c:	e002ed00 	.word	0xe002ed00
30005b40:	0000b479 	.word	0x0000b479
30005b44:	3000d6dc 	.word	0x3000d6dc
30005b48:	40001000 	.word	0x40001000
30005b4c:	0000d835 	.word	0x0000d835
30005b50:	30007e3d 	.word	0x30007e3d
30005b54:	2001c00c 	.word	0x2001c00c
30005b58:	40080000 	.word	0x40080000
30005b5c:	40040000 	.word	0x40040000
30005b60:	42008000 	.word	0x42008000
30005b64:	42008200 	.word	0x42008200
30005b68:	60000020 	.word	0x60000020
30005b6c:	2001bffc 	.word	0x2001bffc
30005b70:	2001b7fc 	.word	0x2001b7fc
30005b74:	00009b2d 	.word	0x00009b2d
30005b78:	3000c880 	.word	0x3000c880

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
30005bd0:	3000ed74 	.word	0x3000ed74

30005bd4 <BOOT_SCBVTORReFill_HP>:
30005bd4:	4a02      	ldr	r2, [pc, #8]	; (30005be0 <BOOT_SCBVTORReFill_HP+0xc>)
30005bd6:	4b03      	ldr	r3, [pc, #12]	; (30005be4 <BOOT_SCBVTORReFill_HP+0x10>)
30005bd8:	6992      	ldr	r2, [r2, #24]
30005bda:	609a      	str	r2, [r3, #8]
30005bdc:	4770      	bx	lr
30005bde:	bf00      	nop
30005be0:	3000ed74 	.word	0x3000ed74
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
30005c58:	3000ed74 	.word	0x3000ed74
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
30005c9c:	3000ed74 	.word	0x3000ed74
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
30005ccc:	f003 b8bc 	b.w	30008e48 <NandImgCopy>
30005cd0:	3000a479 	.word	0x3000a479
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
30005d50:	f006 f9ee 	bl	3000c130 <__DiagPrintf_veneer>
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
30005d72:	f006 f9dd 	bl	3000c130 <__DiagPrintf_veneer>
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
30005dbe:	f006 f9b7 	bl	3000c130 <__DiagPrintf_veneer>
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
30005dfa:	f006 f999 	bl	3000c130 <__DiagPrintf_veneer>
30005dfe:	9e06      	ldr	r6, [sp, #24]
30005e00:	4812      	ldr	r0, [pc, #72]	; (30005e4c <BOOT_LoadImages+0x174>)
30005e02:	4645      	mov	r5, r8
30005e04:	f006 f994 	bl	3000c130 <__DiagPrintf_veneer>
30005e08:	9b07      	ldr	r3, [sp, #28]
30005e0a:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005e0e:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005e12:	e04a      	b.n	30005eaa <BOOT_LoadImages+0x1d2>
30005e14:	480e      	ldr	r0, [pc, #56]	; (30005e50 <BOOT_LoadImages+0x178>)
30005e16:	9207      	str	r2, [sp, #28]
30005e18:	f006 f98a 	bl	3000c130 <__DiagPrintf_veneer>
30005e1c:	9a07      	ldr	r2, [sp, #28]
30005e1e:	3203      	adds	r2, #3
30005e20:	d99b      	bls.n	30005d5a <BOOT_LoadImages+0x82>
30005e22:	480c      	ldr	r0, [pc, #48]	; (30005e54 <BOOT_LoadImages+0x17c>)
30005e24:	e146      	b.n	300060b4 <BOOT_LoadImages+0x3dc>
30005e26:	bf00      	nop
30005e28:	2001c01c 	.word	0x2001c01c
30005e2c:	00009db5 	.word	0x00009db5
30005e30:	07ffe000 	.word	0x07ffe000
30005e34:	3000dc6c 	.word	0x3000dc6c
30005e38:	07fff000 	.word	0x07fff000
30005e3c:	3000c8d4 	.word	0x3000c8d4
30005e40:	3000ccbc 	.word	0x3000ccbc
30005e44:	3000c9d8 	.word	0x3000c9d8
30005e48:	3000cca0 	.word	0x3000cca0
30005e4c:	3000ca64 	.word	0x3000ca64
30005e50:	3000c8c0 	.word	0x3000c8c0
30005e54:	3000cc6c 	.word	0x3000cc6c
30005e58:	07fff004 	.word	0x07fff004
30005e5c:	48c0      	ldr	r0, [pc, #768]	; (30006160 <BOOT_LoadImages+0x488>)
30005e5e:	930f      	str	r3, [sp, #60]	; 0x3c
30005e60:	9209      	str	r2, [sp, #36]	; 0x24
30005e62:	f006 f965 	bl	3000c130 <__DiagPrintf_veneer>
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
30005e9e:	f006 f947 	bl	3000c130 <__DiagPrintf_veneer>
30005ea2:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005ea6:	f8c4 9404 	str.w	r9, [r4, #1028]	; 0x404
30005eaa:	4631      	mov	r1, r6
30005eac:	48ae      	ldr	r0, [pc, #696]	; (30006168 <BOOT_LoadImages+0x490>)
30005eae:	f006 f93f 	bl	3000c130 <__DiagPrintf_veneer>
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
30005f52:	f006 f8ed 	bl	3000c130 <__DiagPrintf_veneer>
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
30005ff2:	f006 f87d 	bl	3000c0f0 <____wrap_memset_veneer>
30005ff6:	2280      	movs	r2, #128	; 0x80
30005ff8:	4629      	mov	r1, r5
30005ffa:	4650      	mov	r0, sl
30005ffc:	f006 f870 	bl	3000c0e0 <____wrap_memcpy_veneer>
30006000:	2e01      	cmp	r6, #1
30006002:	f000 8304 	beq.w	3000660e <BOOT_LoadImages+0x936>
30006006:	2100      	movs	r1, #0
30006008:	f8cd 8014 	str.w	r8, [sp, #20]
3000600c:	f04f 0801 	mov.w	r8, #1
30006010:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30006014:	1d2f      	adds	r7, r5, #4
30006016:	485b      	ldr	r0, [pc, #364]	; (30006184 <BOOT_LoadImages+0x4ac>)
30006018:	f006 f88a 	bl	3000c130 <__DiagPrintf_veneer>
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
300060a8:	f006 f842 	bl	3000c130 <__DiagPrintf_veneer>
300060ac:	9b04      	ldr	r3, [sp, #16]
300060ae:	1e5a      	subs	r2, r3, #1
300060b0:	e651      	b.n	30005d56 <BOOT_LoadImages+0x7e>
300060b2:	4836      	ldr	r0, [pc, #216]	; (3000618c <BOOT_LoadImages+0x4b4>)
300060b4:	f006 f83c 	bl	3000c130 <__DiagPrintf_veneer>
300060b8:	f8d8 3011 	ldr.w	r3, [r8, #17]
300060bc:	f8d8 a00d 	ldr.w	sl, [r8, #13]
300060c0:	9307      	str	r3, [sp, #28]
300060c2:	f8d5 b00d 	ldr.w	fp, [r5, #13]
300060c6:	f8d5 9011 	ldr.w	r9, [r5, #17]
300060ca:	4831      	ldr	r0, [pc, #196]	; (30006190 <BOOT_LoadImages+0x4b8>)
300060cc:	f006 f830 	bl	3000c130 <__DiagPrintf_veneer>
300060d0:	4830      	ldr	r0, [pc, #192]	; (30006194 <BOOT_LoadImages+0x4bc>)
300060d2:	f006 f82d 	bl	3000c130 <__DiagPrintf_veneer>
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
30006160:	3000ca18 	.word	0x3000ca18
30006164:	3000ca74 	.word	0x3000ca74
30006168:	3000caa8 	.word	0x3000caa8
3000616c:	0000c149 	.word	0x0000c149
30006170:	300043b9 	.word	0x300043b9
30006174:	3000d6da 	.word	0x3000d6da
30006178:	3000cb18 	.word	0x3000cb18
3000617c:	3000dc6c 	.word	0x3000dc6c
30006180:	2001c00c 	.word	0x2001c00c
30006184:	3000cbec 	.word	0x3000cbec
30006188:	3000c8a8 	.word	0x3000c8a8
3000618c:	3000c90c 	.word	0x3000c90c
30006190:	3000ca84 	.word	0x3000ca84
30006194:	3000ca98 	.word	0x3000ca98
30006198:	3000ed70 	.word	0x3000ed70
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
300061da:	f005 ffa9 	bl	3000c130 <__DiagPrintf_veneer>
300061de:	9a09      	ldr	r2, [sp, #36]	; 0x24
300061e0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300061e2:	e642      	b.n	30005e6a <BOOT_LoadImages+0x192>
300061e4:	48a8      	ldr	r0, [pc, #672]	; (30006488 <BOOT_LoadImages+0x7b0>)
300061e6:	9204      	str	r2, [sp, #16]
300061e8:	f005 ffa2 	bl	3000c130 <__DiagPrintf_veneer>
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
30006242:	f005 ff75 	bl	3000c130 <__DiagPrintf_veneer>
30006246:	9b08      	ldr	r3, [sp, #32]
30006248:	9a04      	ldr	r2, [sp, #16]
3000624a:	4293      	cmp	r3, r2
3000624c:	f4ff ae26 	bcc.w	30005e9c <BOOT_LoadImages+0x1c4>
30006250:	4891      	ldr	r0, [pc, #580]	; (30006498 <BOOT_LoadImages+0x7c0>)
30006252:	4645      	mov	r5, r8
30006254:	f005 ff6c 	bl	3000c130 <__DiagPrintf_veneer>
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
300062c8:	f005 ff32 	bl	3000c130 <__DiagPrintf_veneer>
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
30006328:	f005 ff02 	bl	3000c130 <__DiagPrintf_veneer>
3000632c:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006330:	2002      	movs	r0, #2
30006332:	f7fe fe43 	bl	30004fbc <FLASH_Erase_With_Lock>
30006336:	4b55      	ldr	r3, [pc, #340]	; (3000648c <BOOT_LoadImages+0x7b4>)
30006338:	689b      	ldr	r3, [r3, #8]
3000633a:	079b      	lsls	r3, r3, #30
3000633c:	d400      	bmi.n	30006340 <BOOT_LoadImages+0x668>
3000633e:	e7fe      	b.n	3000633e <BOOT_LoadImages+0x666>
30006340:	4857      	ldr	r0, [pc, #348]	; (300064a0 <BOOT_LoadImages+0x7c8>)
30006342:	f005 fef5 	bl	3000c130 <__DiagPrintf_veneer>
30006346:	e7fa      	b.n	3000633e <BOOT_LoadImages+0x666>
30006348:	9908      	ldr	r1, [sp, #32]
3000634a:	4856      	ldr	r0, [pc, #344]	; (300064a4 <BOOT_LoadImages+0x7cc>)
3000634c:	f005 fef0 	bl	3000c130 <__DiagPrintf_veneer>
30006350:	9a07      	ldr	r2, [sp, #28]
30006352:	3203      	adds	r2, #3
30006354:	f240 81b4 	bls.w	300066c0 <BOOT_LoadImages+0x9e8>
30006358:	4853      	ldr	r0, [pc, #332]	; (300064a8 <BOOT_LoadImages+0x7d0>)
3000635a:	f005 fee9 	bl	3000c130 <__DiagPrintf_veneer>
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
3000639a:	f005 fec9 	bl	3000c130 <__DiagPrintf_veneer>
3000639e:	9b09      	ldr	r3, [sp, #36]	; 0x24
300063a0:	2b00      	cmp	r3, #0
300063a2:	f43f ae92 	beq.w	300060ca <BOOT_LoadImages+0x3f2>
300063a6:	2300      	movs	r3, #0
300063a8:	9309      	str	r3, [sp, #36]	; 0x24
300063aa:	e4f0      	b.n	30005d8e <BOOT_LoadImages+0xb6>
300063ac:	460a      	mov	r2, r1
300063ae:	4842      	ldr	r0, [pc, #264]	; (300064b8 <BOOT_LoadImages+0x7e0>)
300063b0:	f005 febe 	bl	3000c130 <__DiagPrintf_veneer>
300063b4:	2201      	movs	r2, #1
300063b6:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300063b8:	e505      	b.n	30005dc6 <BOOT_LoadImages+0xee>
300063ba:	460a      	mov	r2, r1
300063bc:	483f      	ldr	r0, [pc, #252]	; (300064bc <BOOT_LoadImages+0x7e4>)
300063be:	f005 feb7 	bl	3000c130 <__DiagPrintf_veneer>
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
300063e8:	f005 fea2 	bl	3000c130 <__DiagPrintf_veneer>
300063ec:	e66d      	b.n	300060ca <BOOT_LoadImages+0x3f2>
300063ee:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
300063f2:	2280      	movs	r2, #128	; 0x80
300063f4:	21ff      	movs	r1, #255	; 0xff
300063f6:	4650      	mov	r0, sl
300063f8:	f005 fe7a 	bl	3000c0f0 <____wrap_memset_veneer>
300063fc:	2280      	movs	r2, #128	; 0x80
300063fe:	4629      	mov	r1, r5
30006400:	4650      	mov	r0, sl
30006402:	f005 fe6d 	bl	3000c0e0 <____wrap_memcpy_veneer>
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
30006426:	f005 fe83 	bl	3000c130 <__DiagPrintf_veneer>
3000642a:	2280      	movs	r2, #128	; 0x80
3000642c:	21ff      	movs	r1, #255	; 0xff
3000642e:	4650      	mov	r0, sl
30006430:	f005 fe5e 	bl	3000c0f0 <____wrap_memset_veneer>
30006434:	2280      	movs	r2, #128	; 0x80
30006436:	4629      	mov	r1, r5
30006438:	4650      	mov	r0, sl
3000643a:	46b1      	mov	r9, r6
3000643c:	f005 fe50 	bl	3000c0e0 <____wrap_memcpy_veneer>
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
30006480:	3000d6da 	.word	0x3000d6da
30006484:	3000ca34 	.word	0x3000ca34
30006488:	3000cc50 	.word	0x3000cc50
3000648c:	2001c00c 	.word	0x2001c00c
30006490:	00009be5 	.word	0x00009be5
30006494:	3000ca50 	.word	0x3000ca50
30006498:	3000ca64 	.word	0x3000ca64
3000649c:	3000cba0 	.word	0x3000cba0
300064a0:	3000c824 	.word	0x3000c824
300064a4:	3000c8f0 	.word	0x3000c8f0
300064a8:	3000cc6c 	.word	0x3000cc6c
300064ac:	07ffe004 	.word	0x07ffe004
300064b0:	3000dc6c 	.word	0x3000dc6c
300064b4:	3000c95c 	.word	0x3000c95c
300064b8:	3000c99c 	.word	0x3000c99c
300064bc:	3000c920 	.word	0x3000c920
300064c0:	3000cc84 	.word	0x3000cc84
300064c4:	3000cca0 	.word	0x3000cca0
300064c8:	3000cabc 	.word	0x3000cabc
300064cc:	300043bd 	.word	0x300043bd
300064d0:	3000ed70 	.word	0x3000ed70
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
30006548:	f005 fdf2 	bl	3000c130 <__DiagPrintf_veneer>
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
3000657e:	f005 fdd7 	bl	3000c130 <__DiagPrintf_veneer>
30006582:	e6d8      	b.n	30006336 <BOOT_LoadImages+0x65e>
30006584:	4898      	ldr	r0, [pc, #608]	; (300067e8 <BOOT_LoadImages+0xb10>)
30006586:	f005 fdd3 	bl	3000c130 <__DiagPrintf_veneer>
3000658a:	e4e9      	b.n	30005f60 <BOOT_LoadImages+0x288>
3000658c:	4895      	ldr	r0, [pc, #596]	; (300067e4 <BOOT_LoadImages+0xb0c>)
3000658e:	f005 fdcf 	bl	3000c130 <__DiagPrintf_veneer>
30006592:	e4e5      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006594:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006598:	4894      	ldr	r0, [pc, #592]	; (300067ec <BOOT_LoadImages+0xb14>)
3000659a:	f005 fdc9 	bl	3000c130 <__DiagPrintf_veneer>
3000659e:	2302      	movs	r3, #2
300065a0:	9303      	str	r3, [sp, #12]
300065a2:	e518      	b.n	30005fd6 <BOOT_LoadImages+0x2fe>
300065a4:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065a8:	4891      	ldr	r0, [pc, #580]	; (300067f0 <BOOT_LoadImages+0xb18>)
300065aa:	f005 fdc1 	bl	3000c130 <__DiagPrintf_veneer>
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
300065f8:	f005 fd9a 	bl	3000c130 <__DiagPrintf_veneer>
300065fc:	e69b      	b.n	30006336 <BOOT_LoadImages+0x65e>
300065fe:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006602:	487b      	ldr	r0, [pc, #492]	; (300067f0 <BOOT_LoadImages+0xb18>)
30006604:	f005 fd94 	bl	3000c130 <__DiagPrintf_veneer>
30006608:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000660c:	e7ae      	b.n	3000656c <BOOT_LoadImages+0x894>
3000660e:	f8cd 8014 	str.w	r8, [sp, #20]
30006612:	4631      	mov	r1, r6
30006614:	46b0      	mov	r8, r6
30006616:	f88d 6064 	strb.w	r6, [sp, #100]	; 0x64
3000661a:	e4fb      	b.n	30006014 <BOOT_LoadImages+0x33c>
3000661c:	f10d 0a58 	add.w	sl, sp, #88	; 0x58
30006620:	4870      	ldr	r0, [pc, #448]	; (300067e4 <BOOT_LoadImages+0xb0c>)
30006622:	f005 fd85 	bl	3000c130 <__DiagPrintf_veneer>
30006626:	2280      	movs	r2, #128	; 0x80
30006628:	21ff      	movs	r1, #255	; 0xff
3000662a:	4650      	mov	r0, sl
3000662c:	f005 fd60 	bl	3000c0f0 <____wrap_memset_veneer>
30006630:	2280      	movs	r2, #128	; 0x80
30006632:	4629      	mov	r1, r5
30006634:	4650      	mov	r0, sl
30006636:	f005 fd53 	bl	3000c0e0 <____wrap_memcpy_veneer>
3000663a:	f1b8 0f00 	cmp.w	r8, #0
3000663e:	d13c      	bne.n	300066ba <BOOT_LoadImages+0x9e2>
30006640:	46c1      	mov	r9, r8
30006642:	f04f 0801 	mov.w	r8, #1
30006646:	e6e3      	b.n	30006410 <BOOT_LoadImages+0x738>
30006648:	461a      	mov	r2, r3
3000664a:	4619      	mov	r1, r3
3000664c:	4869      	ldr	r0, [pc, #420]	; (300067f4 <BOOT_LoadImages+0xb1c>)
3000664e:	f005 fd6f 	bl	3000c130 <__DiagPrintf_veneer>
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
3000666a:	f005 fd61 	bl	3000c130 <__DiagPrintf_veneer>
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
30006684:	f005 fd54 	bl	3000c130 <__DiagPrintf_veneer>
30006688:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000668c:	e500      	b.n	30006090 <BOOT_LoadImages+0x3b8>
3000668e:	461a      	mov	r2, r3
30006690:	4619      	mov	r1, r3
30006692:	4858      	ldr	r0, [pc, #352]	; (300067f4 <BOOT_LoadImages+0xb1c>)
30006694:	f005 fd4c 	bl	3000c130 <__DiagPrintf_veneer>
30006698:	4b51      	ldr	r3, [pc, #324]	; (300067e0 <BOOT_LoadImages+0xb08>)
3000669a:	689a      	ldr	r2, [r3, #8]
3000669c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
300066a0:	f012 0202 	ands.w	r2, r2, #2
300066a4:	d150      	bne.n	30006748 <BOOT_LoadImages+0xa70>
300066a6:	9205      	str	r2, [sp, #20]
300066a8:	e4f2      	b.n	30006090 <BOOT_LoadImages+0x3b8>
300066aa:	4854      	ldr	r0, [pc, #336]	; (300067fc <BOOT_LoadImages+0xb24>)
300066ac:	f005 fd40 	bl	3000c130 <__DiagPrintf_veneer>
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
300066da:	f005 fd29 	bl	3000c130 <__DiagPrintf_veneer>
300066de:	4b40      	ldr	r3, [pc, #256]	; (300067e0 <BOOT_LoadImages+0xb08>)
300066e0:	689e      	ldr	r6, [r3, #8]
300066e2:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066e6:	f016 0602 	ands.w	r6, r6, #2
300066ea:	f43f af3f 	beq.w	3000656c <BOOT_LoadImages+0x894>
300066ee:	f8d3 1006 	ldr.w	r1, [r3, #6]
300066f2:	2600      	movs	r6, #0
300066f4:	483e      	ldr	r0, [pc, #248]	; (300067f0 <BOOT_LoadImages+0xb18>)
300066f6:	f005 fd1b 	bl	3000c130 <__DiagPrintf_veneer>
300066fa:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300066fe:	e735      	b.n	3000656c <BOOT_LoadImages+0x894>
30006700:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006704:	f04f 0901 	mov.w	r9, #1
30006708:	4838      	ldr	r0, [pc, #224]	; (300067ec <BOOT_LoadImages+0xb14>)
3000670a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000670e:	f005 fd0f 	bl	3000c130 <__DiagPrintf_veneer>
30006712:	2302      	movs	r3, #2
30006714:	9303      	str	r3, [sp, #12]
30006716:	e423      	b.n	30005f60 <BOOT_LoadImages+0x288>
30006718:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000671a:	e5fa      	b.n	30006312 <BOOT_LoadImages+0x63a>
3000671c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006720:	4833      	ldr	r0, [pc, #204]	; (300067f0 <BOOT_LoadImages+0xb18>)
30006722:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006726:	f005 fd03 	bl	3000c130 <__DiagPrintf_veneer>
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
3000674e:	f005 fcef 	bl	3000c130 <__DiagPrintf_veneer>
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
30006796:	f005 fccb 	bl	3000c130 <__DiagPrintf_veneer>
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
300067d8:	3000d6da 	.word	0x3000d6da
300067dc:	3000cb18 	.word	0x3000cb18
300067e0:	2001c00c 	.word	0x2001c00c
300067e4:	3000cacc 	.word	0x3000cacc
300067e8:	3000cabc 	.word	0x3000cabc
300067ec:	3000c7f4 	.word	0x3000c7f4
300067f0:	3000c7c4 	.word	0x3000c7c4
300067f4:	3000cb64 	.word	0x3000cb64
300067f8:	3000cadc 	.word	0x3000cadc
300067fc:	3000cc24 	.word	0x3000cc24
30006800:	3000cc84 	.word	0x3000cc84
30006804:	3000c90c 	.word	0x3000c90c
30006808:	3000dc6c 	.word	0x3000dc6c
3000680c:	3000ccbc 	.word	0x3000ccbc
30006810:	3000ed70 	.word	0x3000ed70

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
30006862:	f005 fc65 	bl	3000c130 <__DiagPrintf_veneer>
30006866:	e7ea      	b.n	3000683e <BOOT_Enable_KM0+0x12>
30006868:	4b07      	ldr	r3, [pc, #28]	; (30006888 <BOOT_Enable_KM0+0x5c>)
3000686a:	689b      	ldr	r3, [r3, #8]
3000686c:	079a      	lsls	r2, r3, #30
3000686e:	d5ee      	bpl.n	3000684e <BOOT_Enable_KM0+0x22>
30006870:	4807      	ldr	r0, [pc, #28]	; (30006890 <BOOT_Enable_KM0+0x64>)
30006872:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006876:	f005 bc5b 	b.w	3000c130 <__DiagPrintf_veneer>
3000687a:	bf00      	nop
3000687c:	0000c09d 	.word	0x0000c09d
30006880:	0000c0ad 	.word	0x0000c0ad
30006884:	42008000 	.word	0x42008000
30006888:	2001c00c 	.word	0x2001c00c
3000688c:	3000cd14 	.word	0x3000cd14
30006890:	3000ccd8 	.word	0x3000ccd8

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
300068b0:	3000e2f0 	.word	0x3000e2f0

300068b4 <BOOT_SOC_ClkSet>:
300068b4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300068b8:	4f5a      	ldr	r7, [pc, #360]	; (30006a24 <BOOT_SOC_ClkSet+0x170>)
300068ba:	693d      	ldr	r5, [r7, #16]
300068bc:	683e      	ldr	r6, [r7, #0]
300068be:	2d00      	cmp	r5, #0
300068c0:	db73      	blt.n	300069aa <BOOT_SOC_ClkSet+0xf6>
300068c2:	fbb6 f5f5 	udiv	r5, r6, r5
300068c6:	4b58      	ldr	r3, [pc, #352]	; (30006a28 <BOOT_SOC_ClkSet+0x174>)
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
300068f2:	d071      	beq.n	300069d8 <BOOT_SOC_ClkSet+0x124>
300068f4:	4b4d      	ldr	r3, [pc, #308]	; (30006a2c <BOOT_SOC_ClkSet+0x178>)
300068f6:	2200      	movs	r2, #0
300068f8:	2efa      	cmp	r6, #250	; 0xfa
300068fa:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
300068fe:	d865      	bhi.n	300069cc <BOOT_SOC_ClkSet+0x118>
30006900:	f5b5 7f66 	cmp.w	r5, #920	; 0x398
30006904:	d85c      	bhi.n	300069c0 <BOOT_SOC_ClkSet+0x10c>
30006906:	2001      	movs	r0, #1
30006908:	ea4f 2909 	mov.w	r9, r9, lsl #8
3000690c:	f001 fb32 	bl	30007f74 <CLK_SWITCH_XTAL>
30006910:	683b      	ldr	r3, [r7, #0]
30006912:	4847      	ldr	r0, [pc, #284]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
30006914:	ea4f 3808 	mov.w	r8, r8, lsl #12
30006918:	f409 69e0 	and.w	r9, r9, #1792	; 0x700
3000691c:	0524      	lsls	r4, r4, #20
3000691e:	fb03 f000 	mul.w	r0, r3, r0
30006922:	4b44      	ldr	r3, [pc, #272]	; (30006a34 <BOOT_SOC_ClkSet+0x180>)
30006924:	4798      	blx	r3
30006926:	4944      	ldr	r1, [pc, #272]	; (30006a38 <BOOT_SOC_ClkSet+0x184>)
30006928:	68fb      	ldr	r3, [r7, #12]
3000692a:	fa1f f888 	uxth.w	r8, r8
3000692e:	f8d1 0228 	ldr.w	r0, [r1, #552]	; 0x228
30006932:	f404 04e0 	and.w	r4, r4, #7340032	; 0x700000
30006936:	3b01      	subs	r3, #1
30006938:	4a40      	ldr	r2, [pc, #256]	; (30006a3c <BOOT_SOC_ClkSet+0x188>)
3000693a:	f003 0307 	and.w	r3, r3, #7
3000693e:	4002      	ands	r2, r0
30006940:	2000      	movs	r0, #0
30006942:	4313      	orrs	r3, r2
30006944:	ea49 0303 	orr.w	r3, r9, r3
30006948:	ea48 0303 	orr.w	r3, r8, r3
3000694c:	4323      	orrs	r3, r4
3000694e:	4c3c      	ldr	r4, [pc, #240]	; (30006a40 <BOOT_SOC_ClkSet+0x18c>)
30006950:	f8c1 3228 	str.w	r3, [r1, #552]	; 0x228
30006954:	f001 fb0e 	bl	30007f74 <CLK_SWITCH_XTAL>
30006958:	68a3      	ldr	r3, [r4, #8]
3000695a:	079a      	lsls	r2, r3, #30
3000695c:	d42b      	bmi.n	300069b6 <BOOT_SOC_ClkSet+0x102>
3000695e:	4b39      	ldr	r3, [pc, #228]	; (30006a44 <BOOT_SOC_ClkSet+0x190>)
30006960:	781b      	ldrb	r3, [r3, #0]
30006962:	b303      	cbz	r3, 300069a6 <BOOT_SOC_ClkSet+0xf2>
30006964:	693b      	ldr	r3, [r7, #16]
30006966:	2b00      	cmp	r3, #0
30006968:	db48      	blt.n	300069fc <BOOT_SOC_ClkSet+0x148>
3000696a:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000696e:	6913      	ldr	r3, [r2, #16]
30006970:	f043 030c 	orr.w	r3, r3, #12
30006974:	6113      	str	r3, [r2, #16]
30006976:	2200      	movs	r2, #0
30006978:	4b2c      	ldr	r3, [pc, #176]	; (30006a2c <BOOT_SOC_ClkSet+0x178>)
3000697a:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
3000697e:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30006982:	693b      	ldr	r3, [r7, #16]
30006984:	68a0      	ldr	r0, [r4, #8]
30006986:	690a      	ldr	r2, [r1, #16]
30006988:	3b01      	subs	r3, #1
3000698a:	f022 0203 	bic.w	r2, r2, #3
3000698e:	f003 0303 	and.w	r3, r3, #3
30006992:	4313      	orrs	r3, r2
30006994:	610b      	str	r3, [r1, #16]
30006996:	0783      	lsls	r3, r0, #30
30006998:	d505      	bpl.n	300069a6 <BOOT_SOC_ClkSet+0xf2>
3000699a:	4629      	mov	r1, r5
3000699c:	482a      	ldr	r0, [pc, #168]	; (30006a48 <BOOT_SOC_ClkSet+0x194>)
3000699e:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300069a2:	f005 bbc5 	b.w	3000c130 <__DiagPrintf_veneer>
300069a6:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300069aa:	f025 4500 	bic.w	r5, r5, #2147483648	; 0x80000000
300069ae:	687b      	ldr	r3, [r7, #4]
300069b0:	fbb3 f5f5 	udiv	r5, r3, r5
300069b4:	e787      	b.n	300068c6 <BOOT_SOC_ClkSet+0x12>
300069b6:	4631      	mov	r1, r6
300069b8:	4824      	ldr	r0, [pc, #144]	; (30006a4c <BOOT_SOC_ClkSet+0x198>)
300069ba:	f005 fbb9 	bl	3000c130 <__DiagPrintf_veneer>
300069be:	e7ce      	b.n	3000695e <BOOT_SOC_ClkSet+0xaa>
300069c0:	f240 6163 	movw	r1, #1635	; 0x663
300069c4:	4822      	ldr	r0, [pc, #136]	; (30006a50 <BOOT_SOC_ClkSet+0x19c>)
300069c6:	f005 fbd3 	bl	3000c170 <__io_assert_failed_veneer>
300069ca:	e79c      	b.n	30006906 <BOOT_SOC_ClkSet+0x52>
300069cc:	f240 6162 	movw	r1, #1634	; 0x662
300069d0:	481f      	ldr	r0, [pc, #124]	; (30006a50 <BOOT_SOC_ClkSet+0x19c>)
300069d2:	f005 fbcd 	bl	3000c170 <__io_assert_failed_veneer>
300069d6:	e793      	b.n	30006900 <BOOT_SOC_ClkSet+0x4c>
300069d8:	4b1e      	ldr	r3, [pc, #120]	; (30006a54 <BOOT_SOC_ClkSet+0x1a0>)
300069da:	4650      	mov	r0, sl
300069dc:	4798      	blx	r3
300069de:	4b13      	ldr	r3, [pc, #76]	; (30006a2c <BOOT_SOC_ClkSet+0x178>)
300069e0:	f5b6 7fa7 	cmp.w	r6, #334	; 0x14e
300069e4:	f883 a10c 	strb.w	sl, [r3, #268]	; 0x10c
300069e8:	d216      	bcs.n	30006a18 <BOOT_SOC_ClkSet+0x164>
300069ea:	f5b5 6f96 	cmp.w	r5, #1200	; 0x4b0
300069ee:	d98a      	bls.n	30006906 <BOOT_SOC_ClkSet+0x52>
300069f0:	f240 615f 	movw	r1, #1631	; 0x65f
300069f4:	4816      	ldr	r0, [pc, #88]	; (30006a50 <BOOT_SOC_ClkSet+0x19c>)
300069f6:	f005 fbbb 	bl	3000c170 <__io_assert_failed_veneer>
300069fa:	e784      	b.n	30006906 <BOOT_SOC_ClkSet+0x52>
300069fc:	687b      	ldr	r3, [r7, #4]
300069fe:	480c      	ldr	r0, [pc, #48]	; (30006a30 <BOOT_SOC_ClkSet+0x17c>)
30006a00:	fb03 f000 	mul.w	r0, r3, r0
30006a04:	4b14      	ldr	r3, [pc, #80]	; (30006a58 <BOOT_SOC_ClkSet+0x1a4>)
30006a06:	4798      	blx	r3
30006a08:	4b14      	ldr	r3, [pc, #80]	; (30006a5c <BOOT_SOC_ClkSet+0x1a8>)
30006a0a:	2001      	movs	r0, #1
30006a0c:	4798      	blx	r3
30006a0e:	4b07      	ldr	r3, [pc, #28]	; (30006a2c <BOOT_SOC_ClkSet+0x178>)
30006a10:	2201      	movs	r2, #1
30006a12:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
30006a16:	e7b2      	b.n	3000697e <BOOT_SOC_ClkSet+0xca>
30006a18:	f240 615e 	movw	r1, #1630	; 0x65e
30006a1c:	480c      	ldr	r0, [pc, #48]	; (30006a50 <BOOT_SOC_ClkSet+0x19c>)
30006a1e:	f005 fba7 	bl	3000c170 <__io_assert_failed_veneer>
30006a22:	e7e2      	b.n	300069ea <BOOT_SOC_ClkSet+0x136>
30006a24:	3000e2f0 	.word	0x3000e2f0
30006a28:	51eb851f 	.word	0x51eb851f
30006a2c:	23020000 	.word	0x23020000
30006a30:	000f4240 	.word	0x000f4240
30006a34:	30009371 	.word	0x30009371
30006a38:	42008000 	.word	0x42008000
30006a3c:	ff8f08f8 	.word	0xff8f08f8
30006a40:	2001c00c 	.word	0x2001c00c
30006a44:	3000d6da 	.word	0x3000d6da
30006a48:	3000cd70 	.word	0x3000cd70
30006a4c:	3000cd44 	.word	0x3000cd44
30006a50:	3000d690 	.word	0x3000d690
30006a54:	3000bb3d 	.word	0x3000bb3d
30006a58:	300093f1 	.word	0x300093f1
30006a5c:	30009431 	.word	0x30009431

30006a60 <BOOT_Disable_AP>:
30006a60:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006a64:	4906      	ldr	r1, [pc, #24]	; (30006a80 <BOOT_Disable_AP+0x20>)
30006a66:	685a      	ldr	r2, [r3, #4]
30006a68:	f042 0202 	orr.w	r2, r2, #2
30006a6c:	605a      	str	r2, [r3, #4]
30006a6e:	685a      	ldr	r2, [r3, #4]
30006a70:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006a74:	605a      	str	r2, [r3, #4]
30006a76:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006a78:	f043 0303 	orr.w	r3, r3, #3
30006a7c:	678b      	str	r3, [r1, #120]	; 0x78
30006a7e:	4770      	bx	lr
30006a80:	41000200 	.word	0x41000200

30006a84 <BOOT_Enable_AP>:
30006a84:	b538      	push	{r3, r4, r5, lr}
30006a86:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006a8a:	2201      	movs	r2, #1
30006a8c:	2104      	movs	r1, #4
30006a8e:	4b2b      	ldr	r3, [pc, #172]	; (30006b3c <BOOT_Enable_AP+0xb8>)
30006a90:	482b      	ldr	r0, [pc, #172]	; (30006b40 <BOOT_Enable_AP+0xbc>)
30006a92:	4798      	blx	r3
30006a94:	6863      	ldr	r3, [r4, #4]
30006a96:	4d2b      	ldr	r5, [pc, #172]	; (30006b44 <BOOT_Enable_AP+0xc0>)
30006a98:	2032      	movs	r0, #50	; 0x32
30006a9a:	f043 0302 	orr.w	r3, r3, #2
30006a9e:	6063      	str	r3, [r4, #4]
30006aa0:	6863      	ldr	r3, [r4, #4]
30006aa2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30006aa6:	6063      	str	r3, [r4, #4]
30006aa8:	6823      	ldr	r3, [r4, #0]
30006aaa:	f043 0302 	orr.w	r3, r3, #2
30006aae:	6023      	str	r3, [r4, #0]
30006ab0:	47a8      	blx	r5
30006ab2:	6823      	ldr	r3, [r4, #0]
30006ab4:	2032      	movs	r0, #50	; 0x32
30006ab6:	f043 0304 	orr.w	r3, r3, #4
30006aba:	6023      	str	r3, [r4, #0]
30006abc:	6823      	ldr	r3, [r4, #0]
30006abe:	f043 0301 	orr.w	r3, r3, #1
30006ac2:	6023      	str	r3, [r4, #0]
30006ac4:	6823      	ldr	r3, [r4, #0]
30006ac6:	f043 0310 	orr.w	r3, r3, #16
30006aca:	6023      	str	r3, [r4, #0]
30006acc:	47a8      	blx	r5
30006ace:	6823      	ldr	r3, [r4, #0]
30006ad0:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30006ad4:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006ad8:	6023      	str	r3, [r4, #0]
30006ada:	47a8      	blx	r5
30006adc:	6863      	ldr	r3, [r4, #4]
30006ade:	4a1a      	ldr	r2, [pc, #104]	; (30006b48 <BOOT_Enable_AP+0xc4>)
30006ae0:	f023 0310 	bic.w	r3, r3, #16
30006ae4:	6063      	str	r3, [r4, #4]
30006ae6:	6863      	ldr	r3, [r4, #4]
30006ae8:	f023 0302 	bic.w	r3, r3, #2
30006aec:	6063      	str	r3, [r4, #4]
30006aee:	4790      	blx	r2
30006af0:	b178      	cbz	r0, 30006b12 <BOOT_Enable_AP+0x8e>
30006af2:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006af6:	4815      	ldr	r0, [pc, #84]	; (30006b4c <BOOT_Enable_AP+0xc8>)
30006af8:	4a15      	ldr	r2, [pc, #84]	; (30006b50 <BOOT_Enable_AP+0xcc>)
30006afa:	68d9      	ldr	r1, [r3, #12]
30006afc:	f041 0101 	orr.w	r1, r1, #1
30006b00:	60d9      	str	r1, [r3, #12]
30006b02:	6899      	ldr	r1, [r3, #8]
30006b04:	f041 0101 	orr.w	r1, r1, #1
30006b08:	6099      	str	r1, [r3, #8]
30006b0a:	6843      	ldr	r3, [r0, #4]
30006b0c:	431a      	orrs	r2, r3
30006b0e:	6042      	str	r2, [r0, #4]
30006b10:	bd38      	pop	{r3, r4, r5, pc}
30006b12:	4604      	mov	r4, r0
30006b14:	4d0f      	ldr	r5, [pc, #60]	; (30006b54 <BOOT_Enable_AP+0xd0>)
30006b16:	4601      	mov	r1, r0
30006b18:	2041      	movs	r0, #65	; 0x41
30006b1a:	47a8      	blx	r5
30006b1c:	4621      	mov	r1, r4
30006b1e:	2042      	movs	r0, #66	; 0x42
30006b20:	47a8      	blx	r5
30006b22:	4621      	mov	r1, r4
30006b24:	2043      	movs	r0, #67	; 0x43
30006b26:	47a8      	blx	r5
30006b28:	4621      	mov	r1, r4
30006b2a:	2044      	movs	r0, #68	; 0x44
30006b2c:	47a8      	blx	r5
30006b2e:	4621      	mov	r1, r4
30006b30:	2045      	movs	r0, #69	; 0x45
30006b32:	47a8      	blx	r5
30006b34:	4621      	mov	r1, r4
30006b36:	2046      	movs	r0, #70	; 0x46
30006b38:	47a8      	blx	r5
30006b3a:	e7da      	b.n	30006af2 <BOOT_Enable_AP+0x6e>
30006b3c:	0000aab9 	.word	0x0000aab9
30006b40:	4200c000 	.word	0x4200c000
30006b44:	00009b2d 	.word	0x00009b2d
30006b48:	3000a4c1 	.word	0x3000a4c1
30006b4c:	41000200 	.word	0x41000200
30006b50:	01001111 	.word	0x01001111
30006b54:	0000b20d 	.word	0x0000b20d

30006b58 <BOOT_RAM_KeyDeriveFunc>:
30006b58:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006b5c:	ed2d 8b04 	vpush	{d8-d9}
30006b60:	b0cb      	sub	sp, #300	; 0x12c
30006b62:	4c6a      	ldr	r4, [pc, #424]	; (30006d0c <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006b64:	4680      	mov	r8, r0
30006b66:	4689      	mov	r9, r1
30006b68:	4869      	ldr	r0, [pc, #420]	; (30006d10 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006b6a:	ee08 2a90 	vmov	s17, r2
30006b6e:	4969      	ldr	r1, [pc, #420]	; (30006d14 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006b70:	2201      	movs	r2, #1
30006b72:	ee09 3a10 	vmov	s18, r3
30006b76:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006b7a:	47a0      	blx	r4
30006b7c:	2201      	movs	r2, #1
30006b7e:	4966      	ldr	r1, [pc, #408]	; (30006d18 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006b80:	ae02      	add	r6, sp, #8
30006b82:	4864      	ldr	r0, [pc, #400]	; (30006d14 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006b84:	47a0      	blx	r4
30006b86:	4b65      	ldr	r3, [pc, #404]	; (30006d1c <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006b88:	2000      	movs	r0, #0
30006b8a:	4798      	blx	r3
30006b8c:	22e0      	movs	r2, #224	; 0xe0
30006b8e:	2100      	movs	r1, #0
30006b90:	a812      	add	r0, sp, #72	; 0x48
30006b92:	f005 faad 	bl	3000c0f0 <____wrap_memset_veneer>
30006b96:	2300      	movs	r3, #0
30006b98:	f8ad 3004 	strh.w	r3, [sp, #4]
30006b9c:	f88d 3006 	strb.w	r3, [sp, #6]
30006ba0:	2301      	movs	r3, #1
30006ba2:	f88d 3007 	strb.w	r3, [sp, #7]
30006ba6:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006ba8:	2b00      	cmp	r3, #0
30006baa:	f000 80ab 	beq.w	30006d04 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006bae:	ab0a      	add	r3, sp, #40	; 0x28
30006bb0:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006d30 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006bb4:	4f5a      	ldr	r7, [pc, #360]	; (30006d20 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006bb6:	ee08 3a10 	vmov	s16, r3
30006bba:	ab12      	add	r3, sp, #72	; 0x48
30006bbc:	464a      	mov	r2, r9
30006bbe:	4641      	mov	r1, r8
30006bc0:	2020      	movs	r0, #32
30006bc2:	47d8      	blx	fp
30006bc4:	aa12      	add	r2, sp, #72	; 0x48
30006bc6:	ee19 1a10 	vmov	r1, s18
30006bca:	ee18 0a90 	vmov	r0, s17
30006bce:	47b8      	blx	r7
30006bd0:	ee18 5a10 	vmov	r5, s16
30006bd4:	aa12      	add	r2, sp, #72	; 0x48
30006bd6:	2104      	movs	r1, #4
30006bd8:	a801      	add	r0, sp, #4
30006bda:	4634      	mov	r4, r6
30006bdc:	47b8      	blx	r7
30006bde:	a912      	add	r1, sp, #72	; 0x48
30006be0:	4b50      	ldr	r3, [pc, #320]	; (30006d24 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006be2:	ee18 0a10 	vmov	r0, s16
30006be6:	4798      	blx	r3
30006be8:	f1ba 0f01 	cmp.w	sl, #1
30006bec:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006bee:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006bf0:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006bf4:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006bf8:	d91f      	bls.n	30006c3a <BOOT_RAM_KeyDeriveFunc+0xe2>
30006bfa:	2501      	movs	r5, #1
30006bfc:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006c00:	ab12      	add	r3, sp, #72	; 0x48
30006c02:	4641      	mov	r1, r8
30006c04:	464a      	mov	r2, r9
30006c06:	2020      	movs	r0, #32
30006c08:	47d8      	blx	fp
30006c0a:	aa12      	add	r2, sp, #72	; 0x48
30006c0c:	2120      	movs	r1, #32
30006c0e:	4630      	mov	r0, r6
30006c10:	47b8      	blx	r7
30006c12:	a912      	add	r1, sp, #72	; 0x48
30006c14:	4b43      	ldr	r3, [pc, #268]	; (30006d24 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006c16:	4630      	mov	r0, r6
30006c18:	4798      	blx	r3
30006c1a:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006c1e:	f10d 0107 	add.w	r1, sp, #7
30006c22:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006c26:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006c2a:	42a3      	cmp	r3, r4
30006c2c:	ea82 0200 	eor.w	r2, r2, r0
30006c30:	701a      	strb	r2, [r3, #0]
30006c32:	d1f6      	bne.n	30006c22 <BOOT_RAM_KeyDeriveFunc+0xca>
30006c34:	3501      	adds	r5, #1
30006c36:	45aa      	cmp	sl, r5
30006c38:	d1e2      	bne.n	30006c00 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006c3a:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006c3c:	ee18 1a10 	vmov	r1, s16
30006c40:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c42:	2a20      	cmp	r2, #32
30006c44:	985a      	ldr	r0, [sp, #360]	; 0x168
30006c46:	bf28      	it	cs
30006c48:	2220      	movcs	r2, #32
30006c4a:	1a9b      	subs	r3, r3, r2
30006c4c:	9359      	str	r3, [sp, #356]	; 0x164
30006c4e:	4603      	mov	r3, r0
30006c50:	4413      	add	r3, r2
30006c52:	935a      	str	r3, [sp, #360]	; 0x168
30006c54:	f005 fa44 	bl	3000c0e0 <____wrap_memcpy_veneer>
30006c58:	f10d 0207 	add.w	r2, sp, #7
30006c5c:	2104      	movs	r1, #4
30006c5e:	7813      	ldrb	r3, [r2, #0]
30006c60:	3901      	subs	r1, #1
30006c62:	3301      	adds	r3, #1
30006c64:	b2db      	uxtb	r3, r3
30006c66:	f802 3901 	strb.w	r3, [r2], #-1
30006c6a:	b90b      	cbnz	r3, 30006c70 <BOOT_RAM_KeyDeriveFunc+0x118>
30006c6c:	2900      	cmp	r1, #0
30006c6e:	d1f6      	bne.n	30006c5e <BOOT_RAM_KeyDeriveFunc+0x106>
30006c70:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006c72:	2b00      	cmp	r3, #0
30006c74:	d1a1      	bne.n	30006bba <BOOT_RAM_KeyDeriveFunc+0x62>
30006c76:	2220      	movs	r2, #32
30006c78:	2100      	movs	r1, #0
30006c7a:	4c2b      	ldr	r4, [pc, #172]	; (30006d28 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006c7c:	4630      	mov	r0, r6
30006c7e:	47a0      	blx	r4
30006c80:	2220      	movs	r2, #32
30006c82:	2100      	movs	r1, #0
30006c84:	ee18 0a10 	vmov	r0, s16
30006c88:	47a0      	blx	r4
30006c8a:	4b28      	ldr	r3, [pc, #160]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006c8c:	695b      	ldr	r3, [r3, #20]
30006c8e:	03da      	lsls	r2, r3, #15
30006c90:	d512      	bpl.n	30006cb8 <BOOT_RAM_KeyDeriveFunc+0x160>
30006c92:	f016 021f 	ands.w	r2, r6, #31
30006c96:	4633      	mov	r3, r6
30006c98:	d130      	bne.n	30006cfc <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006c9a:	2220      	movs	r2, #32
30006c9c:	f3bf 8f4f 	dsb	sy
30006ca0:	4822      	ldr	r0, [pc, #136]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006ca2:	441a      	add	r2, r3
30006ca4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006ca8:	3320      	adds	r3, #32
30006caa:	1ad1      	subs	r1, r2, r3
30006cac:	2900      	cmp	r1, #0
30006cae:	dcf9      	bgt.n	30006ca4 <BOOT_RAM_KeyDeriveFunc+0x14c>
30006cb0:	f3bf 8f4f 	dsb	sy
30006cb4:	f3bf 8f6f 	isb	sy
30006cb8:	4b1c      	ldr	r3, [pc, #112]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cba:	695b      	ldr	r3, [r3, #20]
30006cbc:	03db      	lsls	r3, r3, #15
30006cbe:	d513      	bpl.n	30006ce8 <BOOT_RAM_KeyDeriveFunc+0x190>
30006cc0:	ee18 3a10 	vmov	r3, s16
30006cc4:	f013 021f 	ands.w	r2, r3, #31
30006cc8:	d114      	bne.n	30006cf4 <BOOT_RAM_KeyDeriveFunc+0x19c>
30006cca:	2220      	movs	r2, #32
30006ccc:	f3bf 8f4f 	dsb	sy
30006cd0:	4816      	ldr	r0, [pc, #88]	; (30006d2c <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006cd2:	441a      	add	r2, r3
30006cd4:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006cd8:	3320      	adds	r3, #32
30006cda:	1ad1      	subs	r1, r2, r3
30006cdc:	2900      	cmp	r1, #0
30006cde:	dcf9      	bgt.n	30006cd4 <BOOT_RAM_KeyDeriveFunc+0x17c>
30006ce0:	f3bf 8f4f 	dsb	sy
30006ce4:	f3bf 8f6f 	isb	sy
30006ce8:	2000      	movs	r0, #0
30006cea:	b04b      	add	sp, #300	; 0x12c
30006cec:	ecbd 8b04 	vpop	{d8-d9}
30006cf0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006cf4:	f023 031f 	bic.w	r3, r3, #31
30006cf8:	3220      	adds	r2, #32
30006cfa:	e7e7      	b.n	30006ccc <BOOT_RAM_KeyDeriveFunc+0x174>
30006cfc:	f026 031f 	bic.w	r3, r6, #31
30006d00:	3220      	adds	r2, #32
30006d02:	e7cb      	b.n	30006c9c <BOOT_RAM_KeyDeriveFunc+0x144>
30006d04:	ab0a      	add	r3, sp, #40	; 0x28
30006d06:	ee08 3a10 	vmov	s16, r3
30006d0a:	e7b4      	b.n	30006c76 <BOOT_RAM_KeyDeriveFunc+0x11e>
30006d0c:	0000b479 	.word	0x0000b479
30006d10:	40000008 	.word	0x40000008
30006d14:	40000004 	.word	0x40000004
30006d18:	40000002 	.word	0x40000002
30006d1c:	00002451 	.word	0x00002451
30006d20:	00005b4d 	.word	0x00005b4d
30006d24:	00005bb9 	.word	0x00005bb9
30006d28:	00012be5 	.word	0x00012be5
30006d2c:	e000ed00 	.word	0xe000ed00
30006d30:	30009de9 	.word	0x30009de9

30006d34 <password_hash_check>:
30006d34:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d38:	b08f      	sub	sp, #60	; 0x3c
30006d3a:	4690      	mov	r8, r2
30006d3c:	461d      	mov	r5, r3
30006d3e:	4606      	mov	r6, r0
30006d40:	af04      	add	r7, sp, #16
30006d42:	3201      	adds	r2, #1
30006d44:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d46:	6079      	str	r1, [r7, #4]
30006d48:	2100      	movs	r1, #0
30006d4a:	f103 0008 	add.w	r0, r3, #8
30006d4e:	f108 0308 	add.w	r3, r8, #8
30006d52:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006d56:	f023 0307 	bic.w	r3, r3, #7
30006d5a:	f020 0007 	bic.w	r0, r0, #7
30006d5e:	ebad 0d03 	sub.w	sp, sp, r3
30006d62:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006d64:	f10d 0910 	add.w	r9, sp, #16
30006d68:	3301      	adds	r3, #1
30006d6a:	ebad 0d00 	sub.w	sp, sp, r0
30006d6e:	603b      	str	r3, [r7, #0]
30006d70:	4648      	mov	r0, r9
30006d72:	4b3f      	ldr	r3, [pc, #252]	; (30006e70 <password_hash_check+0x13c>)
30006d74:	f10d 0a10 	add.w	sl, sp, #16
30006d78:	4798      	blx	r3
30006d7a:	683b      	ldr	r3, [r7, #0]
30006d7c:	2100      	movs	r1, #0
30006d7e:	4650      	mov	r0, sl
30006d80:	461a      	mov	r2, r3
30006d82:	4b3b      	ldr	r3, [pc, #236]	; (30006e70 <password_hash_check+0x13c>)
30006d84:	4798      	blx	r3
30006d86:	f1b8 0001 	subs.w	r0, r8, #1
30006d8a:	d01d      	beq.n	30006dc8 <password_hash_check+0x94>
30006d8c:	2200      	movs	r2, #0
30006d8e:	f109 33ff 	add.w	r3, r9, #4294967295
30006d92:	4611      	mov	r1, r2
30006d94:	e00b      	b.n	30006dae <password_hash_check+0x7a>
30006d96:	f108 38ff 	add.w	r8, r8, #4294967295
30006d9a:	2100      	movs	r1, #0
30006d9c:	1c9c      	adds	r4, r3, #2
30006d9e:	4282      	cmp	r2, r0
30006da0:	7059      	strb	r1, [r3, #1]
30006da2:	f103 0301 	add.w	r3, r3, #1
30006da6:	4611      	mov	r1, r2
30006da8:	eba4 0409 	sub.w	r4, r4, r9
30006dac:	d20b      	bcs.n	30006dc6 <password_hash_check+0x92>
30006dae:	1874      	adds	r4, r6, r1
30006db0:	f102 0c01 	add.w	ip, r2, #1
30006db4:	5c71      	ldrb	r1, [r6, r1]
30006db6:	3202      	adds	r2, #2
30006db8:	7864      	ldrb	r4, [r4, #1]
30006dba:	2c30      	cmp	r4, #48	; 0x30
30006dbc:	d101      	bne.n	30006dc2 <password_hash_check+0x8e>
30006dbe:	295c      	cmp	r1, #92	; 0x5c
30006dc0:	d0e9      	beq.n	30006d96 <password_hash_check+0x62>
30006dc2:	4662      	mov	r2, ip
30006dc4:	e7ea      	b.n	30006d9c <password_hash_check+0x68>
30006dc6:	4406      	add	r6, r0
30006dc8:	7833      	ldrb	r3, [r6, #0]
30006dca:	f809 3004 	strb.w	r3, [r9, r4]
30006dce:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006dd0:	1e5c      	subs	r4, r3, #1
30006dd2:	d04a      	beq.n	30006e6a <password_hash_check+0x136>
30006dd4:	2200      	movs	r2, #0
30006dd6:	f10a 33ff 	add.w	r3, sl, #4294967295
30006dda:	4611      	mov	r1, r2
30006ddc:	e00b      	b.n	30006df6 <password_hash_check+0xc2>
30006dde:	f10b 3bff 	add.w	fp, fp, #4294967295
30006de2:	2100      	movs	r1, #0
30006de4:	1c98      	adds	r0, r3, #2
30006de6:	42a2      	cmp	r2, r4
30006de8:	7059      	strb	r1, [r3, #1]
30006dea:	f103 0301 	add.w	r3, r3, #1
30006dee:	4611      	mov	r1, r2
30006df0:	eba0 000a 	sub.w	r0, r0, sl
30006df4:	d20a      	bcs.n	30006e0c <password_hash_check+0xd8>
30006df6:	1868      	adds	r0, r5, r1
30006df8:	1c56      	adds	r6, r2, #1
30006dfa:	5c69      	ldrb	r1, [r5, r1]
30006dfc:	3202      	adds	r2, #2
30006dfe:	7840      	ldrb	r0, [r0, #1]
30006e00:	2830      	cmp	r0, #48	; 0x30
30006e02:	d101      	bne.n	30006e08 <password_hash_check+0xd4>
30006e04:	295c      	cmp	r1, #92	; 0x5c
30006e06:	d0ea      	beq.n	30006dde <password_hash_check+0xaa>
30006e08:	4632      	mov	r2, r6
30006e0a:	e7eb      	b.n	30006de4 <password_hash_check+0xb0>
30006e0c:	4425      	add	r5, r4
30006e0e:	782b      	ldrb	r3, [r5, #0]
30006e10:	f107 0408 	add.w	r4, r7, #8
30006e14:	2520      	movs	r5, #32
30006e16:	4641      	mov	r1, r8
30006e18:	f80a 3000 	strb.w	r3, [sl, r0]
30006e1c:	4652      	mov	r2, sl
30006e1e:	687b      	ldr	r3, [r7, #4]
30006e20:	4648      	mov	r0, r9
30006e22:	9402      	str	r4, [sp, #8]
30006e24:	9300      	str	r3, [sp, #0]
30006e26:	465b      	mov	r3, fp
30006e28:	9501      	str	r5, [sp, #4]
30006e2a:	f7ff fe95 	bl	30006b58 <BOOT_RAM_KeyDeriveFunc>
30006e2e:	4620      	mov	r0, r4
30006e30:	462a      	mov	r2, r5
30006e32:	4910      	ldr	r1, [pc, #64]	; (30006e74 <password_hash_check+0x140>)
30006e34:	f005 f964 	bl	3000c100 <____wrap_memcmp_veneer>
30006e38:	4604      	mov	r4, r0
30006e3a:	b968      	cbnz	r0, 30006e58 <password_hash_check+0x124>
30006e3c:	480e      	ldr	r0, [pc, #56]	; (30006e78 <password_hash_check+0x144>)
30006e3e:	f005 f977 	bl	3000c130 <__DiagPrintf_veneer>
30006e42:	462a      	mov	r2, r5
30006e44:	4621      	mov	r1, r4
30006e46:	f107 0008 	add.w	r0, r7, #8
30006e4a:	4b09      	ldr	r3, [pc, #36]	; (30006e70 <password_hash_check+0x13c>)
30006e4c:	4798      	blx	r3
30006e4e:	4620      	mov	r0, r4
30006e50:	372c      	adds	r7, #44	; 0x2c
30006e52:	46bd      	mov	sp, r7
30006e54:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e58:	2401      	movs	r4, #1
30006e5a:	4808      	ldr	r0, [pc, #32]	; (30006e7c <password_hash_check+0x148>)
30006e5c:	f005 f968 	bl	3000c130 <__DiagPrintf_veneer>
30006e60:	372c      	adds	r7, #44	; 0x2c
30006e62:	4620      	mov	r0, r4
30006e64:	46bd      	mov	sp, r7
30006e66:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e6a:	4620      	mov	r0, r4
30006e6c:	e7cf      	b.n	30006e0e <password_hash_check+0xda>
30006e6e:	bf00      	nop
30006e70:	00012be5 	.word	0x00012be5
30006e74:	3000e074 	.word	0x3000e074
30006e78:	3000cdb4 	.word	0x3000cdb4
30006e7c:	3000cd9c 	.word	0x3000cd9c

30006e80 <BOOT_Image1>:
30006e80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006e84:	49ae      	ldr	r1, [pc, #696]	; (30007140 <BOOT_Image1+0x2c0>)
30006e86:	b08b      	sub	sp, #44	; 0x2c
30006e88:	48ae      	ldr	r0, [pc, #696]	; (30007144 <BOOT_Image1+0x2c4>)
30006e8a:	f005 f951 	bl	3000c130 <__DiagPrintf_veneer>
30006e8e:	48ae      	ldr	r0, [pc, #696]	; (30007148 <BOOT_Image1+0x2c8>)
30006e90:	4aae      	ldr	r2, [pc, #696]	; (3000714c <BOOT_Image1+0x2cc>)
30006e92:	2100      	movs	r1, #0
30006e94:	4bae      	ldr	r3, [pc, #696]	; (30007150 <BOOT_Image1+0x2d0>)
30006e96:	1a12      	subs	r2, r2, r0
30006e98:	4798      	blx	r3
30006e9a:	4aae      	ldr	r2, [pc, #696]	; (30007154 <BOOT_Image1+0x2d4>)
30006e9c:	49ae      	ldr	r1, [pc, #696]	; (30007158 <BOOT_Image1+0x2d8>)
30006e9e:	8c13      	ldrh	r3, [r2, #32]
30006ea0:	b29b      	uxth	r3, r3
30006ea2:	8413      	strh	r3, [r2, #32]
30006ea4:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006ea8:	f002 fec2 	bl	30009c30 <BOOT_Reason>
30006eac:	2800      	cmp	r0, #0
30006eae:	f000 8300 	beq.w	300074b2 <BOOT_Image1+0x632>
30006eb2:	4baa      	ldr	r3, [pc, #680]	; (3000715c <BOOT_Image1+0x2dc>)
30006eb4:	781b      	ldrb	r3, [r3, #0]
30006eb6:	2b00      	cmp	r3, #0
30006eb8:	f040 8306 	bne.w	300074c8 <BOOT_Image1+0x648>
30006ebc:	4da8      	ldr	r5, [pc, #672]	; (30007160 <BOOT_Image1+0x2e0>)
30006ebe:	682b      	ldr	r3, [r5, #0]
30006ec0:	4ea8      	ldr	r6, [pc, #672]	; (30007164 <BOOT_Image1+0x2e4>)
30006ec2:	f043 0302 	orr.w	r3, r3, #2
30006ec6:	4aa8      	ldr	r2, [pc, #672]	; (30007168 <BOOT_Image1+0x2e8>)
30006ec8:	602b      	str	r3, [r5, #0]
30006eca:	4790      	blx	r2
30006ecc:	7834      	ldrb	r4, [r6, #0]
30006ece:	2c00      	cmp	r4, #0
30006ed0:	f000 82b6 	beq.w	30007440 <BOOT_Image1+0x5c0>
30006ed4:	2000      	movs	r0, #0
30006ed6:	4ba5      	ldr	r3, [pc, #660]	; (3000716c <BOOT_Image1+0x2ec>)
30006ed8:	4798      	blx	r3
30006eda:	4ba5      	ldr	r3, [pc, #660]	; (30007170 <BOOT_Image1+0x2f0>)
30006edc:	2001      	movs	r0, #1
30006ede:	4798      	blx	r3
30006ee0:	4ca4      	ldr	r4, [pc, #656]	; (30007174 <BOOT_Image1+0x2f4>)
30006ee2:	f7ff fce7 	bl	300068b4 <BOOT_SOC_ClkSet>
30006ee6:	f7fe fcb3 	bl	30005850 <BOOT_GRstConfig>
30006eea:	47a0      	blx	r4
30006eec:	2802      	cmp	r0, #2
30006eee:	f000 82b1 	beq.w	30007454 <BOOT_Image1+0x5d4>
30006ef2:	4ba1      	ldr	r3, [pc, #644]	; (30007178 <BOOT_Image1+0x2f8>)
30006ef4:	2701      	movs	r7, #1
30006ef6:	49a1      	ldr	r1, [pc, #644]	; (3000717c <BOOT_Image1+0x2fc>)
30006ef8:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006efc:	f883 710e 	strb.w	r7, [r3, #270]	; 0x10e
30006f00:	463a      	mov	r2, r7
30006f02:	f8df 82c4 	ldr.w	r8, [pc, #708]	; 300071c8 <BOOT_Image1+0x348>
30006f06:	47c0      	blx	r8
30006f08:	463a      	mov	r2, r7
30006f0a:	499d      	ldr	r1, [pc, #628]	; (30007180 <BOOT_Image1+0x300>)
30006f0c:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006f10:	47c0      	blx	r8
30006f12:	4a90      	ldr	r2, [pc, #576]	; (30007154 <BOOT_Image1+0x2d4>)
30006f14:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006f18:	f043 0303 	orr.w	r3, r3, #3
30006f1c:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006f20:	4b98      	ldr	r3, [pc, #608]	; (30007184 <BOOT_Image1+0x304>)
30006f22:	4798      	blx	r3
30006f24:	b930      	cbnz	r0, 30006f34 <BOOT_Image1+0xb4>
30006f26:	4a8b      	ldr	r2, [pc, #556]	; (30007154 <BOOT_Image1+0x2d4>)
30006f28:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006f2c:	f023 0303 	bic.w	r3, r3, #3
30006f30:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006f34:	4b94      	ldr	r3, [pc, #592]	; (30007188 <BOOT_Image1+0x308>)
30006f36:	781b      	ldrb	r3, [r3, #0]
30006f38:	2b00      	cmp	r3, #0
30006f3a:	f040 8313 	bne.w	30007564 <BOOT_Image1+0x6e4>
30006f3e:	4f93      	ldr	r7, [pc, #588]	; (3000718c <BOOT_Image1+0x30c>)
30006f40:	2102      	movs	r1, #2
30006f42:	2201      	movs	r2, #1
30006f44:	4892      	ldr	r0, [pc, #584]	; (30007190 <BOOT_Image1+0x310>)
30006f46:	47b8      	blx	r7
30006f48:	68ab      	ldr	r3, [r5, #8]
30006f4a:	0799      	lsls	r1, r3, #30
30006f4c:	f100 82c7 	bmi.w	300074de <BOOT_Image1+0x65e>
30006f50:	4990      	ldr	r1, [pc, #576]	; (30007194 <BOOT_Image1+0x314>)
30006f52:	e841 f100 	tt	r1, r1
30006f56:	68ab      	ldr	r3, [r5, #8]
30006f58:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006f5c:	079a      	lsls	r2, r3, #30
30006f5e:	f100 82a4 	bmi.w	300074aa <BOOT_Image1+0x62a>
30006f62:	4b8d      	ldr	r3, [pc, #564]	; (30007198 <BOOT_Image1+0x318>)
30006f64:	4f8d      	ldr	r7, [pc, #564]	; (3000719c <BOOT_Image1+0x31c>)
30006f66:	681b      	ldr	r3, [r3, #0]
30006f68:	4798      	blx	r3
30006f6a:	f7fe fa63 	bl	30005434 <BOOT_RccConfig>
30006f6e:	f7fe facd 	bl	3000550c <BOOT_CACHEWRR_Set>
30006f72:	4b8b      	ldr	r3, [pc, #556]	; (300071a0 <BOOT_Image1+0x320>)
30006f74:	6818      	ldr	r0, [r3, #0]
30006f76:	f7fe faf7 	bl	30005568 <BOOT_TCMSet>
30006f7a:	4b8a      	ldr	r3, [pc, #552]	; (300071a4 <BOOT_Image1+0x324>)
30006f7c:	695b      	ldr	r3, [r3, #20]
30006f7e:	03db      	lsls	r3, r3, #15
30006f80:	d513      	bpl.n	30006faa <BOOT_Image1+0x12a>
30006f82:	f017 011f 	ands.w	r1, r7, #31
30006f86:	f040 823a 	bne.w	300073fe <BOOT_Image1+0x57e>
30006f8a:	463b      	mov	r3, r7
30006f8c:	2174      	movs	r1, #116	; 0x74
30006f8e:	f3bf 8f4f 	dsb	sy
30006f92:	4884      	ldr	r0, [pc, #528]	; (300071a4 <BOOT_Image1+0x324>)
30006f94:	4419      	add	r1, r3
30006f96:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006f9a:	3320      	adds	r3, #32
30006f9c:	1aca      	subs	r2, r1, r3
30006f9e:	2a00      	cmp	r2, #0
30006fa0:	dcf9      	bgt.n	30006f96 <BOOT_Image1+0x116>
30006fa2:	f3bf 8f4f 	dsb	sy
30006fa6:	f3bf 8f6f 	isb	sy
30006faa:	4b6a      	ldr	r3, [pc, #424]	; (30007154 <BOOT_Image1+0x2d4>)
30006fac:	2200      	movs	r2, #0
30006fae:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30006fb2:	4b71      	ldr	r3, [pc, #452]	; (30007178 <BOOT_Image1+0x2f8>)
30006fb4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30006fb8:	f7fc fc12 	bl	300037e0 <flash_highspeed_setup>
30006fbc:	f001 fde4 	bl	30008b88 <flash_layout_init>
30006fc0:	47a0      	blx	r4
30006fc2:	2802      	cmp	r0, #2
30006fc4:	f000 8258 	beq.w	30007478 <BOOT_Image1+0x5f8>
30006fc8:	7832      	ldrb	r2, [r6, #0]
30006fca:	68ab      	ldr	r3, [r5, #8]
30006fcc:	b162      	cbz	r2, 30006fe8 <BOOT_Image1+0x168>
30006fce:	4c76      	ldr	r4, [pc, #472]	; (300071a8 <BOOT_Image1+0x328>)
30006fd0:	4e76      	ldr	r6, [pc, #472]	; (300071ac <BOOT_Image1+0x32c>)
30006fd2:	f013 0f02 	tst.w	r3, #2
30006fd6:	4630      	mov	r0, r6
30006fd8:	d001      	beq.n	30006fde <BOOT_Image1+0x15e>
30006fda:	f005 f8a9 	bl	3000c130 <__DiagPrintf_veneer>
30006fde:	f241 3088 	movw	r0, #5000	; 0x1388
30006fe2:	47a0      	blx	r4
30006fe4:	68ab      	ldr	r3, [r5, #8]
30006fe6:	e7f4      	b.n	30006fd2 <BOOT_Image1+0x152>
30006fe8:	079a      	lsls	r2, r3, #30
30006fea:	f100 8349 	bmi.w	30007680 <BOOT_Image1+0x800>
30006fee:	f7fe fc6b 	bl	300058c8 <BOOT_DDR_Init>
30006ff2:	4a58      	ldr	r2, [pc, #352]	; (30007154 <BOOT_Image1+0x2d4>)
30006ff4:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30006ff8:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30006ffc:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30007000:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30007004:	2400      	movs	r4, #0
30007006:	4953      	ldr	r1, [pc, #332]	; (30007154 <BOOT_Image1+0x2d4>)
30007008:	2307      	movs	r3, #7
3000700a:	f10d 021d 	add.w	r2, sp, #29
3000700e:	f8c1 42d0 	str.w	r4, [r1, #720]	; 0x2d0
30007012:	4620      	mov	r0, r4
30007014:	f8c1 42d4 	str.w	r4, [r1, #724]	; 0x2d4
30007018:	f8c1 42d8 	str.w	r4, [r1, #728]	; 0x2d8
3000701c:	f8c1 40d0 	str.w	r4, [r1, #208]	; 0xd0
30007020:	f44f 7159 	mov.w	r1, #868	; 0x364
30007024:	f002 f98c 	bl	30009340 <EFUSERead8>
30007028:	f10d 011f 	add.w	r1, sp, #31
3000702c:	f44f 705a 	mov.w	r0, #872	; 0x368
30007030:	f88d 401e 	strb.w	r4, [sp, #30]
30007034:	f88d 401f 	strb.w	r4, [sp, #31]
30007038:	f002 f8a2 	bl	30009180 <OTP_Read8>
3000703c:	f10d 011e 	add.w	r1, sp, #30
30007040:	f240 3069 	movw	r0, #873	; 0x369
30007044:	f002 f89c 	bl	30009180 <OTP_Read8>
30007048:	f89d 301d 	ldrb.w	r3, [sp, #29]
3000704c:	f013 0f1e 	tst.w	r3, #30
30007050:	d10a      	bne.n	30007068 <BOOT_Image1+0x1e8>
30007052:	f89d 201e 	ldrb.w	r2, [sp, #30]
30007056:	f89d 301f 	ldrb.w	r3, [sp, #31]
3000705a:	f002 0208 	and.w	r2, r2, #8
3000705e:	f003 0301 	and.w	r3, r3, #1
30007062:	4313      	orrs	r3, r2
30007064:	f040 81cf 	bne.w	30007406 <BOOT_Image1+0x586>
30007068:	4a51      	ldr	r2, [pc, #324]	; (300071b0 <BOOT_Image1+0x330>)
3000706a:	2100      	movs	r1, #0
3000706c:	2002      	movs	r0, #2
3000706e:	2632      	movs	r6, #50	; 0x32
30007070:	f000 fd8a 	bl	30007b88 <ymodem_uart_port_init>
30007074:	484f      	ldr	r0, [pc, #316]	; (300071b4 <BOOT_Image1+0x334>)
30007076:	f005 f84b 	bl	3000c110 <____wrap_strlen_veneer>
3000707a:	4601      	mov	r1, r0
3000707c:	484d      	ldr	r0, [pc, #308]	; (300071b4 <BOOT_Image1+0x334>)
3000707e:	4c4a      	ldr	r4, [pc, #296]	; (300071a8 <BOOT_Image1+0x328>)
30007080:	f000 fddc 	bl	30007c3c <ymodem_uart_putdata>
30007084:	e002      	b.n	3000708c <BOOT_Image1+0x20c>
30007086:	3e01      	subs	r6, #1
30007088:	f000 82e0 	beq.w	3000764c <BOOT_Image1+0x7cc>
3000708c:	2001      	movs	r0, #1
3000708e:	47a0      	blx	r4
30007090:	f000 fdba 	bl	30007c08 <ymodem_uart_readable>
30007094:	2800      	cmp	r0, #0
30007096:	d0f6      	beq.n	30007086 <BOOT_Image1+0x206>
30007098:	f000 fdb6 	bl	30007c08 <ymodem_uart_readable>
3000709c:	2800      	cmp	r0, #0
3000709e:	f000 82fd 	beq.w	3000769c <BOOT_Image1+0x81c>
300070a2:	2301      	movs	r3, #1
300070a4:	9303      	str	r3, [sp, #12]
300070a6:	2001      	movs	r0, #1
300070a8:	47a0      	blx	r4
300070aa:	2108      	movs	r1, #8
300070ac:	4842      	ldr	r0, [pc, #264]	; (300071b8 <BOOT_Image1+0x338>)
300070ae:	f000 fdd3 	bl	30007c58 <ymodem_uart_getdata>
300070b2:	4941      	ldr	r1, [pc, #260]	; (300071b8 <BOOT_Image1+0x338>)
300070b4:	2208      	movs	r2, #8
300070b6:	4b41      	ldr	r3, [pc, #260]	; (300071bc <BOOT_Image1+0x33c>)
300070b8:	f101 0080 	add.w	r0, r1, #128	; 0x80
300070bc:	4798      	blx	r3
300070be:	4606      	mov	r6, r0
300070c0:	2800      	cmp	r0, #0
300070c2:	f040 8351 	bne.w	30007768 <BOOT_Image1+0x8e8>
300070c6:	f000 fd9f 	bl	30007c08 <ymodem_uart_readable>
300070ca:	2800      	cmp	r0, #0
300070cc:	d0fb      	beq.n	300070c6 <BOOT_Image1+0x246>
300070ce:	2101      	movs	r1, #1
300070d0:	483b      	ldr	r0, [pc, #236]	; (300071c0 <BOOT_Image1+0x340>)
300070d2:	f000 fdc1 	bl	30007c58 <ymodem_uart_getdata>
300070d6:	f000 fd97 	bl	30007c08 <ymodem_uart_readable>
300070da:	2800      	cmp	r0, #0
300070dc:	d0fb      	beq.n	300070d6 <BOOT_Image1+0x256>
300070de:	4f39      	ldr	r7, [pc, #228]	; (300071c4 <BOOT_Image1+0x344>)
300070e0:	2101      	movs	r1, #1
300070e2:	f207 404d 	addw	r0, r7, #1101	; 0x44d
300070e6:	f000 fdb7 	bl	30007c58 <ymodem_uart_getdata>
300070ea:	f897 244d 	ldrb.w	r2, [r7, #1101]	; 0x44d
300070ee:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
300070f2:	429a      	cmp	r2, r3
300070f4:	f000 8343 	beq.w	3000777e <BOOT_Image1+0x8fe>
300070f8:	f207 4a4e 	addw	sl, r7, #1102	; 0x44e
300070fc:	f04f 0800 	mov.w	r8, #0
30007100:	46c1      	mov	r9, r8
30007102:	f108 0801 	add.w	r8, r8, #1
30007106:	f000 fd7f 	bl	30007c08 <ymodem_uart_readable>
3000710a:	2800      	cmp	r0, #0
3000710c:	d0fb      	beq.n	30007106 <BOOT_Image1+0x286>
3000710e:	4650      	mov	r0, sl
30007110:	2101      	movs	r1, #1
30007112:	f000 fda1 	bl	30007c58 <ymodem_uart_getdata>
30007116:	f89a 2000 	ldrb.w	r2, [sl]
3000711a:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
3000711e:	f109 090b 	add.w	r9, r9, #11
30007122:	f10a 0a01 	add.w	sl, sl, #1
30007126:	46c3      	mov	fp, r8
30007128:	429a      	cmp	r2, r3
3000712a:	d1e9      	bne.n	30007100 <BOOT_Image1+0x280>
3000712c:	f1b8 0f00 	cmp.w	r8, #0
30007130:	dd5b      	ble.n	300071ea <BOOT_Image1+0x36a>
30007132:	f207 414c 	addw	r1, r7, #1100	; 0x44c
30007136:	2300      	movs	r3, #0
30007138:	eb01 0008 	add.w	r0, r1, r8
3000713c:	e048      	b.n	300071d0 <BOOT_Image1+0x350>
3000713e:	bf00      	nop
30007140:	3000d6a0 	.word	0x3000d6a0
30007144:	3000cdd0 	.word	0x3000cdd0
30007148:	3000e9b0 	.word	0x3000e9b0
3000714c:	3000ede4 	.word	0x3000ede4
30007150:	00012be5 	.word	0x00012be5
30007154:	42008000 	.word	0x42008000
30007158:	42008200 	.word	0x42008200
3000715c:	3000d6ed 	.word	0x3000d6ed
30007160:	2001c00c 	.word	0x2001c00c
30007164:	3000d6d9 	.word	0x3000d6d9
30007168:	3000bb5d 	.word	0x3000bb5d
3000716c:	3000b9cd 	.word	0x3000b9cd
30007170:	3000b8ed 	.word	0x3000b8ed
30007174:	30007e3d 	.word	0x30007e3d
30007178:	23020000 	.word	0x23020000
3000717c:	40080000 	.word	0x40080000
30007180:	40040000 	.word	0x40040000
30007184:	3000a4c1 	.word	0x3000a4c1
30007188:	3000d6d8 	.word	0x3000d6d8
3000718c:	0000aab9 	.word	0x0000aab9
30007190:	4200c000 	.word	0x4200c000
30007194:	30006e81 	.word	0x30006e81
30007198:	00033000 	.word	0x00033000
3000719c:	2001c01c 	.word	0x2001c01c
300071a0:	3000d6dc 	.word	0x3000d6dc
300071a4:	e000ed00 	.word	0xe000ed00
300071a8:	00009be5 	.word	0x00009be5
300071ac:	3000ce80 	.word	0x3000ce80
300071b0:	0016e360 	.word	0x0016e360
300071b4:	3000e094 	.word	0x3000e094
300071b8:	3000e0b0 	.word	0x3000e0b0
300071bc:	000129bd 	.word	0x000129bd
300071c0:	3000e0b8 	.word	0x3000e0b8
300071c4:	3000dc6c 	.word	0x3000dc6c
300071c8:	0000b479 	.word	0x0000b479
300071cc:	4288      	cmp	r0, r1
300071ce:	d00d      	beq.n	300071ec <BOOT_Image1+0x36c>
300071d0:	f811 2f01 	ldrb.w	r2, [r1, #1]!
300071d4:	eb03 0383 	add.w	r3, r3, r3, lsl #2
300071d8:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
300071dc:	eb02 0343 	add.w	r3, r2, r3, lsl #1
300071e0:	f1bc 0f09 	cmp.w	ip, #9
300071e4:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
300071e8:	d9f0      	bls.n	300071cc <BOOT_Image1+0x34c>
300071ea:	2300      	movs	r3, #0
300071ec:	f8c7 34d0 	str.w	r3, [r7, #1232]	; 0x4d0
300071f0:	f000 fd0a 	bl	30007c08 <ymodem_uart_readable>
300071f4:	2800      	cmp	r0, #0
300071f6:	d0fb      	beq.n	300071f0 <BOOT_Image1+0x370>
300071f8:	49bc      	ldr	r1, [pc, #752]	; (300074ec <BOOT_Image1+0x66c>)
300071fa:	460b      	mov	r3, r1
300071fc:	2101      	movs	r1, #1
300071fe:	eb03 0009 	add.w	r0, r3, r9
30007202:	9302      	str	r3, [sp, #8]
30007204:	f000 fd28 	bl	30007c58 <ymodem_uart_getdata>
30007208:	9902      	ldr	r1, [sp, #8]
3000720a:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
3000720e:	f811 2009 	ldrb.w	r2, [r1, r9]
30007212:	429a      	cmp	r2, r3
30007214:	f000 82bc 	beq.w	30007790 <BOOT_Image1+0x910>
30007218:	f10b 090b 	add.w	r9, fp, #11
3000721c:	2300      	movs	r3, #0
3000721e:	9404      	str	r4, [sp, #16]
30007220:	4489      	add	r9, r1
30007222:	469a      	mov	sl, r3
30007224:	f10a 0a01 	add.w	sl, sl, #1
30007228:	f000 fcee 	bl	30007c08 <ymodem_uart_readable>
3000722c:	2800      	cmp	r0, #0
3000722e:	d0fb      	beq.n	30007228 <BOOT_Image1+0x3a8>
30007230:	4648      	mov	r0, r9
30007232:	2101      	movs	r1, #1
30007234:	f000 fd10 	bl	30007c58 <ymodem_uart_getdata>
30007238:	f899 2000 	ldrb.w	r2, [r9]
3000723c:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
30007240:	f109 0901 	add.w	r9, r9, #1
30007244:	eb0a 010b 	add.w	r1, sl, fp
30007248:	429a      	cmp	r2, r3
3000724a:	d1eb      	bne.n	30007224 <BOOT_Image1+0x3a4>
3000724c:	f8cd a014 	str.w	sl, [sp, #20]
30007250:	468a      	mov	sl, r1
30007252:	9c04      	ldr	r4, [sp, #16]
30007254:	f000 fcd8 	bl	30007c08 <ymodem_uart_readable>
30007258:	2800      	cmp	r0, #0
3000725a:	d0fb      	beq.n	30007254 <BOOT_Image1+0x3d4>
3000725c:	9b02      	ldr	r3, [sp, #8]
3000725e:	f10a 090b 	add.w	r9, sl, #11
30007262:	2101      	movs	r1, #1
30007264:	eb03 0009 	add.w	r0, r3, r9
30007268:	f000 fcf6 	bl	30007c58 <ymodem_uart_getdata>
3000726c:	9902      	ldr	r1, [sp, #8]
3000726e:	f897 24cd 	ldrb.w	r2, [r7, #1229]	; 0x4cd
30007272:	f811 3009 	ldrb.w	r3, [r1, r9]
30007276:	429a      	cmp	r2, r3
30007278:	f000 8288 	beq.w	3000778c <BOOT_Image1+0x90c>
3000727c:	f10a 090c 	add.w	r9, sl, #12
30007280:	4489      	add	r9, r1
30007282:	3601      	adds	r6, #1
30007284:	f000 fcc0 	bl	30007c08 <ymodem_uart_readable>
30007288:	2800      	cmp	r0, #0
3000728a:	d0fb      	beq.n	30007284 <BOOT_Image1+0x404>
3000728c:	4648      	mov	r0, r9
3000728e:	2101      	movs	r1, #1
30007290:	f000 fce2 	bl	30007c58 <ymodem_uart_getdata>
30007294:	f899 2000 	ldrb.w	r2, [r9]
30007298:	f897 34cd 	ldrb.w	r3, [r7, #1229]	; 0x4cd
3000729c:	f109 0901 	add.w	r9, r9, #1
300072a0:	eb06 0b0a 	add.w	fp, r6, sl
300072a4:	429a      	cmp	r2, r3
300072a6:	d1ec      	bne.n	30007282 <BOOT_Image1+0x402>
300072a8:	9b02      	ldr	r3, [sp, #8]
300072aa:	f10b 090c 	add.w	r9, fp, #12
300072ae:	2101      	movs	r1, #1
300072b0:	eb03 0009 	add.w	r0, r3, r9
300072b4:	f000 fcd0 	bl	30007c58 <ymodem_uart_getdata>
300072b8:	9b02      	ldr	r3, [sp, #8]
300072ba:	f813 1009 	ldrb.w	r1, [r3, r9]
300072be:	290a      	cmp	r1, #10
300072c0:	f8c7 14d4 	str.w	r1, [r7, #1236]	; 0x4d4
300072c4:	f200 824c 	bhi.w	30007760 <BOOT_Image1+0x8e0>
300072c8:	9b02      	ldr	r3, [sp, #8]
300072ca:	f10b 000d 	add.w	r0, fp, #13
300072ce:	2101      	movs	r1, #1
300072d0:	f10b 090e 	add.w	r9, fp, #14
300072d4:	4418      	add	r0, r3
300072d6:	f000 fcbf 	bl	30007c58 <ymodem_uart_getdata>
300072da:	9b02      	ldr	r3, [sp, #8]
300072dc:	2102      	movs	r1, #2
300072de:	4499      	add	r9, r3
300072e0:	4648      	mov	r0, r9
300072e2:	f000 fcb9 	bl	30007c58 <ymodem_uart_getdata>
300072e6:	4649      	mov	r1, r9
300072e8:	2202      	movs	r2, #2
300072ea:	4881      	ldr	r0, [pc, #516]	; (300074f0 <BOOT_Image1+0x670>)
300072ec:	4b81      	ldr	r3, [pc, #516]	; (300074f4 <BOOT_Image1+0x674>)
300072ee:	4798      	blx	r3
300072f0:	2800      	cmp	r0, #0
300072f2:	f040 8240 	bne.w	30007776 <BOOT_Image1+0x8f6>
300072f6:	9b02      	ldr	r3, [sp, #8]
300072f8:	f10b 0010 	add.w	r0, fp, #16
300072fc:	2101      	movs	r1, #1
300072fe:	f10b 0911 	add.w	r9, fp, #17
30007302:	4418      	add	r0, r3
30007304:	f000 fca8 	bl	30007c58 <ymodem_uart_getdata>
30007308:	4b7b      	ldr	r3, [pc, #492]	; (300074f8 <BOOT_Image1+0x678>)
3000730a:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
3000730e:	f103 0228 	add.w	r2, r3, #40	; 0x28
30007312:	f103 0130 	add.w	r1, r3, #48	; 0x30
30007316:	f7fd fdd1 	bl	30004ebc <hotfix_get_addr_size>
3000731a:	9b02      	ldr	r3, [sp, #8]
3000731c:	2102      	movs	r1, #2
3000731e:	4499      	add	r9, r3
30007320:	4648      	mov	r0, r9
30007322:	f000 fc99 	bl	30007c58 <ymodem_uart_getdata>
30007326:	4649      	mov	r1, r9
30007328:	2202      	movs	r2, #2
3000732a:	4874      	ldr	r0, [pc, #464]	; (300074fc <BOOT_Image1+0x67c>)
3000732c:	4b71      	ldr	r3, [pc, #452]	; (300074f4 <BOOT_Image1+0x674>)
3000732e:	4798      	blx	r3
30007330:	2800      	cmp	r0, #0
30007332:	f000 8139 	beq.w	300075a8 <BOOT_Image1+0x728>
30007336:	9b03      	ldr	r3, [sp, #12]
30007338:	2b00      	cmp	r3, #0
3000733a:	f000 8180 	beq.w	3000763e <BOOT_Image1+0x7be>
3000733e:	f7fe fccb 	bl	30005cd8 <BOOT_LoadImages>
30007342:	2800      	cmp	r0, #0
30007344:	f000 8129 	beq.w	3000759a <BOOT_Image1+0x71a>
30007348:	f7ff fa70 	bl	3000682c <BOOT_Enable_KM0>
3000734c:	2008      	movs	r0, #8
3000734e:	4b6c      	ldr	r3, [pc, #432]	; (30007500 <BOOT_Image1+0x680>)
30007350:	4798      	blx	r3
30007352:	4b6c      	ldr	r3, [pc, #432]	; (30007504 <BOOT_Image1+0x684>)
30007354:	2201      	movs	r2, #1
30007356:	f8c3 2398 	str.w	r2, [r3, #920]	; 0x398
3000735a:	f7fd fcd3 	bl	30004d04 <BOOT_RAM_TZCfg>
3000735e:	2004      	movs	r0, #4
30007360:	47a0      	blx	r4
30007362:	4b69      	ldr	r3, [pc, #420]	; (30007508 <BOOT_Image1+0x688>)
30007364:	781b      	ldrb	r3, [r3, #0]
30007366:	2b00      	cmp	r3, #0
30007368:	f000 8176 	beq.w	30007658 <BOOT_Image1+0x7d8>
3000736c:	f7ff fb8a 	bl	30006a84 <BOOT_Enable_AP>
30007370:	4a66      	ldr	r2, [pc, #408]	; (3000750c <BOOT_Image1+0x68c>)
30007372:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30007376:	f043 0302 	orr.w	r3, r3, #2
3000737a:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
3000737e:	2019      	movs	r0, #25
30007380:	47a0      	blx	r4
30007382:	4962      	ldr	r1, [pc, #392]	; (3000750c <BOOT_Image1+0x68c>)
30007384:	4a62      	ldr	r2, [pc, #392]	; (30007510 <BOOT_Image1+0x690>)
30007386:	f891 3067 	ldrb.w	r3, [r1, #103]	; 0x67
3000738a:	6894      	ldr	r4, [r2, #8]
3000738c:	f043 0301 	orr.w	r3, r3, #1
30007390:	f881 3067 	strb.w	r3, [r1, #103]	; 0x67
30007394:	4b5f      	ldr	r3, [pc, #380]	; (30007514 <BOOT_Image1+0x694>)
30007396:	6811      	ldr	r1, [r2, #0]
30007398:	4a5f      	ldr	r2, [pc, #380]	; (30007518 <BOOT_Image1+0x698>)
3000739a:	6061      	str	r1, [r4, #4]
3000739c:	6094      	str	r4, [r2, #8]
3000739e:	6a59      	ldr	r1, [r3, #36]	; 0x24
300073a0:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300073a4:	6259      	str	r1, [r3, #36]	; 0x24
300073a6:	6a51      	ldr	r1, [r2, #36]	; 0x24
300073a8:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300073ac:	6251      	str	r1, [r2, #36]	; 0x24
300073ae:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300073b2:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300073b6:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300073ba:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300073be:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300073c2:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300073c6:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
300073ca:	4954      	ldr	r1, [pc, #336]	; (3000751c <BOOT_Image1+0x69c>)
300073cc:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
300073d0:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
300073d4:	689b      	ldr	r3, [r3, #8]
300073d6:	618b      	str	r3, [r1, #24]
300073d8:	f7fe fc06 	bl	30005be8 <BOOT_NVICBackup_HP>
300073dc:	4b50      	ldr	r3, [pc, #320]	; (30007520 <BOOT_Image1+0x6a0>)
300073de:	f383 8888 	msr	MSP_NS, r3
300073e2:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
300073e6:	f383 8809 	msr	PSP, r3
300073ea:	68ab      	ldr	r3, [r5, #8]
300073ec:	079b      	lsls	r3, r3, #30
300073ee:	f100 814b 	bmi.w	30007688 <BOOT_Image1+0x808>
300073f2:	6860      	ldr	r0, [r4, #4]
300073f4:	f7fd ffce 	bl	30005394 <BOOT_NsStart>
300073f8:	b00b      	add	sp, #44	; 0x2c
300073fa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300073fe:	3174      	adds	r1, #116	; 0x74
30007400:	f027 031f 	bic.w	r3, r7, #31
30007404:	e5c3      	b.n	30006f8e <BOOT_Image1+0x10e>
30007406:	4847      	ldr	r0, [pc, #284]	; (30007524 <BOOT_Image1+0x6a4>)
30007408:	f004 fe92 	bl	3000c130 <__DiagPrintf_veneer>
3000740c:	f89d 101f 	ldrb.w	r1, [sp, #31]
30007410:	f89d 301e 	ldrb.w	r3, [sp, #30]
30007414:	f44f 705a 	mov.w	r0, #872	; 0x368
30007418:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
3000741c:	f023 0308 	bic.w	r3, r3, #8
30007420:	f88d 101f 	strb.w	r1, [sp, #31]
30007424:	f88d 301e 	strb.w	r3, [sp, #30]
30007428:	f001 feb4 	bl	30009194 <OTP_Write8>
3000742c:	f89d 101e 	ldrb.w	r1, [sp, #30]
30007430:	f240 3069 	movw	r0, #873	; 0x369
30007434:	f001 feae 	bl	30009194 <OTP_Write8>
30007438:	483b      	ldr	r0, [pc, #236]	; (30007528 <BOOT_Image1+0x6a8>)
3000743a:	f004 fe79 	bl	3000c130 <__DiagPrintf_veneer>
3000743e:	e613      	b.n	30007068 <BOOT_Image1+0x1e8>
30007440:	4b3a      	ldr	r3, [pc, #232]	; (3000752c <BOOT_Image1+0x6ac>)
30007442:	2001      	movs	r0, #1
30007444:	4798      	blx	r3
30007446:	4620      	mov	r0, r4
30007448:	4b39      	ldr	r3, [pc, #228]	; (30007530 <BOOT_Image1+0x6b0>)
3000744a:	4798      	blx	r3
3000744c:	4b39      	ldr	r3, [pc, #228]	; (30007534 <BOOT_Image1+0x6b4>)
3000744e:	2001      	movs	r0, #1
30007450:	4798      	blx	r3
30007452:	e545      	b.n	30006ee0 <BOOT_Image1+0x60>
30007454:	4b38      	ldr	r3, [pc, #224]	; (30007538 <BOOT_Image1+0x6b8>)
30007456:	2201      	movs	r2, #1
30007458:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
3000745c:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30007460:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30007464:	4b35      	ldr	r3, [pc, #212]	; (3000753c <BOOT_Image1+0x6bc>)
30007466:	4798      	blx	r3
30007468:	4a35      	ldr	r2, [pc, #212]	; (30007540 <BOOT_Image1+0x6c0>)
3000746a:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000746e:	f043 0302 	orr.w	r3, r3, #2
30007472:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30007476:	e553      	b.n	30006f20 <BOOT_Image1+0xa0>
30007478:	68ab      	ldr	r3, [r5, #8]
3000747a:	0798      	lsls	r0, r3, #30
3000747c:	f100 80fc 	bmi.w	30007678 <BOOT_Image1+0x7f8>
30007480:	4b30      	ldr	r3, [pc, #192]	; (30007544 <BOOT_Image1+0x6c4>)
30007482:	4798      	blx	r3
30007484:	f7fe fa5e 	bl	30005944 <BOOT_PSRAM_Init>
30007488:	7830      	ldrb	r0, [r6, #0]
3000748a:	2800      	cmp	r0, #0
3000748c:	f47f adba 	bne.w	30007004 <BOOT_Image1+0x184>
30007490:	4b2d      	ldr	r3, [pc, #180]	; (30007548 <BOOT_Image1+0x6c8>)
30007492:	4798      	blx	r3
30007494:	2800      	cmp	r0, #0
30007496:	f43f adb5 	beq.w	30007004 <BOOT_Image1+0x184>
3000749a:	682b      	ldr	r3, [r5, #0]
3000749c:	0399      	lsls	r1, r3, #14
3000749e:	f57f adb1 	bpl.w	30007004 <BOOT_Image1+0x184>
300074a2:	482a      	ldr	r0, [pc, #168]	; (3000754c <BOOT_Image1+0x6cc>)
300074a4:	f004 fe44 	bl	3000c130 <__DiagPrintf_veneer>
300074a8:	e5ac      	b.n	30007004 <BOOT_Image1+0x184>
300074aa:	4829      	ldr	r0, [pc, #164]	; (30007550 <BOOT_Image1+0x6d0>)
300074ac:	f004 fe40 	bl	3000c130 <__DiagPrintf_veneer>
300074b0:	e557      	b.n	30006f62 <BOOT_Image1+0xe2>
300074b2:	4601      	mov	r1, r0
300074b4:	f44f 7201 	mov.w	r2, #516	; 0x204
300074b8:	481f      	ldr	r0, [pc, #124]	; (30007538 <BOOT_Image1+0x6b8>)
300074ba:	f004 fe19 	bl	3000c0f0 <____wrap_memset_veneer>
300074be:	4b25      	ldr	r3, [pc, #148]	; (30007554 <BOOT_Image1+0x6d4>)
300074c0:	781b      	ldrb	r3, [r3, #0]
300074c2:	2b00      	cmp	r3, #0
300074c4:	f43f acfa 	beq.w	30006ebc <BOOT_Image1+0x3c>
300074c8:	4b23      	ldr	r3, [pc, #140]	; (30007558 <BOOT_Image1+0x6d8>)
300074ca:	4d24      	ldr	r5, [pc, #144]	; (3000755c <BOOT_Image1+0x6dc>)
300074cc:	4798      	blx	r3
300074ce:	2800      	cmp	r0, #0
300074d0:	f47f acf5 	bne.w	30006ebe <BOOT_Image1+0x3e>
300074d4:	68ab      	ldr	r3, [r5, #8]
300074d6:	f043 0302 	orr.w	r3, r3, #2
300074da:	60ab      	str	r3, [r5, #8]
300074dc:	e4ef      	b.n	30006ebe <BOOT_Image1+0x3e>
300074de:	f3ef 8108 	mrs	r1, MSP
300074e2:	481f      	ldr	r0, [pc, #124]	; (30007560 <BOOT_Image1+0x6e0>)
300074e4:	f004 fe24 	bl	3000c130 <__DiagPrintf_veneer>
300074e8:	e532      	b.n	30006f50 <BOOT_Image1+0xd0>
300074ea:	bf00      	nop
300074ec:	3000e0b0 	.word	0x3000e0b0
300074f0:	3000e144 	.word	0x3000e144
300074f4:	000129bd 	.word	0x000129bd
300074f8:	3000e148 	.word	0x3000e148
300074fc:	3000e184 	.word	0x3000e184
30007500:	0000e1d9 	.word	0x0000e1d9
30007504:	400e0000 	.word	0x400e0000
30007508:	3000d6da 	.word	0x3000d6da
3000750c:	42008200 	.word	0x42008200
30007510:	60000020 	.word	0x60000020
30007514:	e000ed00 	.word	0xe000ed00
30007518:	e002ed00 	.word	0xe002ed00
3000751c:	3000ed74 	.word	0x3000ed74
30007520:	2001bffc 	.word	0x2001bffc
30007524:	3000cef4 	.word	0x3000cef4
30007528:	3000cf2c 	.word	0x3000cf2c
3000752c:	3000b8ed 	.word	0x3000b8ed
30007530:	3000bb1d 	.word	0x3000bb1d
30007534:	3000ba8d 	.word	0x3000ba8d
30007538:	23020000 	.word	0x23020000
3000753c:	0000b479 	.word	0x0000b479
30007540:	42008000 	.word	0x42008000
30007544:	30009491 	.word	0x30009491
30007548:	3000b9ed 	.word	0x3000b9ed
3000754c:	3000ce70 	.word	0x3000ce70
30007550:	3000ce18 	.word	0x3000ce18
30007554:	3000d6ed 	.word	0x3000d6ed
30007558:	0000c179 	.word	0x0000c179
3000755c:	2001c00c 	.word	0x2001c00c
30007560:	3000cde8 	.word	0x3000cde8
30007564:	f8df 82a4 	ldr.w	r8, [pc, #676]	; 3000780c <BOOT_Image1+0x98c>
30007568:	47c0      	blx	r8
3000756a:	4607      	mov	r7, r0
3000756c:	47c0      	blx	r8
3000756e:	2f02      	cmp	r7, #2
30007570:	f43f ace5 	beq.w	30006f3e <BOOT_Image1+0xbe>
30007574:	2803      	cmp	r0, #3
30007576:	f43f ace2 	beq.w	30006f3e <BOOT_Image1+0xbe>
3000757a:	4b8b      	ldr	r3, [pc, #556]	; (300077a8 <BOOT_Image1+0x928>)
3000757c:	4798      	blx	r3
3000757e:	4f8b      	ldr	r7, [pc, #556]	; (300077ac <BOOT_Image1+0x92c>)
30007580:	2200      	movs	r2, #0
30007582:	2101      	movs	r1, #1
30007584:	488a      	ldr	r0, [pc, #552]	; (300077b0 <BOOT_Image1+0x930>)
30007586:	47b8      	blx	r7
30007588:	2101      	movs	r1, #1
3000758a:	4889      	ldr	r0, [pc, #548]	; (300077b0 <BOOT_Image1+0x930>)
3000758c:	4b89      	ldr	r3, [pc, #548]	; (300077b4 <BOOT_Image1+0x934>)
3000758e:	4798      	blx	r3
30007590:	2201      	movs	r2, #1
30007592:	4887      	ldr	r0, [pc, #540]	; (300077b0 <BOOT_Image1+0x930>)
30007594:	4611      	mov	r1, r2
30007596:	47b8      	blx	r7
30007598:	e4d2      	b.n	30006f40 <BOOT_Image1+0xc0>
3000759a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000759e:	47a0      	blx	r4
300075a0:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300075a4:	47a0      	blx	r4
300075a6:	e7f8      	b.n	3000759a <BOOT_Image1+0x71a>
300075a8:	9b02      	ldr	r3, [sp, #8]
300075aa:	f10b 0013 	add.w	r0, fp, #19
300075ae:	2101      	movs	r1, #1
300075b0:	f10b 0914 	add.w	r9, fp, #20
300075b4:	4418      	add	r0, r3
300075b6:	f000 fb4f 	bl	30007c58 <ymodem_uart_getdata>
300075ba:	4b7f      	ldr	r3, [pc, #508]	; (300077b8 <BOOT_Image1+0x938>)
300075bc:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
300075c0:	f1a3 0218 	sub.w	r2, r3, #24
300075c4:	f1a3 0110 	sub.w	r1, r3, #16
300075c8:	f7fd fc78 	bl	30004ebc <hotfix_get_addr_size>
300075cc:	9b02      	ldr	r3, [sp, #8]
300075ce:	2102      	movs	r1, #2
300075d0:	4499      	add	r9, r3
300075d2:	4648      	mov	r0, r9
300075d4:	f000 fb40 	bl	30007c58 <ymodem_uart_getdata>
300075d8:	4649      	mov	r1, r9
300075da:	2202      	movs	r2, #2
300075dc:	4877      	ldr	r0, [pc, #476]	; (300077bc <BOOT_Image1+0x93c>)
300075de:	4b78      	ldr	r3, [pc, #480]	; (300077c0 <BOOT_Image1+0x940>)
300075e0:	4798      	blx	r3
300075e2:	2800      	cmp	r0, #0
300075e4:	f47f aea7 	bne.w	30007336 <BOOT_Image1+0x4b6>
300075e8:	9b02      	ldr	r3, [sp, #8]
300075ea:	f10b 0016 	add.w	r0, fp, #22
300075ee:	2101      	movs	r1, #1
300075f0:	f10b 0917 	add.w	r9, fp, #23
300075f4:	4418      	add	r0, r3
300075f6:	f000 fb2f 	bl	30007c58 <ymodem_uart_getdata>
300075fa:	4b72      	ldr	r3, [pc, #456]	; (300077c4 <BOOT_Image1+0x944>)
300075fc:	f8d7 04d4 	ldr.w	r0, [r7, #1236]	; 0x4d4
30007600:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30007604:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30007608:	f7fd fc58 	bl	30004ebc <hotfix_get_addr_size>
3000760c:	9a02      	ldr	r2, [sp, #8]
3000760e:	2102      	movs	r1, #2
30007610:	4491      	add	r9, r2
30007612:	4648      	mov	r0, r9
30007614:	f000 fb20 	bl	30007c58 <ymodem_uart_getdata>
30007618:	4649      	mov	r1, r9
3000761a:	2202      	movs	r2, #2
3000761c:	486a      	ldr	r0, [pc, #424]	; (300077c8 <BOOT_Image1+0x948>)
3000761e:	4b68      	ldr	r3, [pc, #416]	; (300077c0 <BOOT_Image1+0x940>)
30007620:	4798      	blx	r3
30007622:	2800      	cmp	r0, #0
30007624:	f47f ae87 	bne.w	30007336 <BOOT_Image1+0x4b6>
30007628:	9b02      	ldr	r3, [sp, #8]
3000762a:	f10b 0019 	add.w	r0, fp, #25
3000762e:	2101      	movs	r1, #1
30007630:	4418      	add	r0, r3
30007632:	f000 fb11 	bl	30007c58 <ymodem_uart_getdata>
30007636:	9b03      	ldr	r3, [sp, #12]
30007638:	2b00      	cmp	r3, #0
3000763a:	f040 80ad 	bne.w	30007798 <BOOT_Image1+0x918>
3000763e:	2002      	movs	r0, #2
30007640:	f000 faba 	bl	30007bb8 <ymodem_uart_port_deinit>
30007644:	4861      	ldr	r0, [pc, #388]	; (300077cc <BOOT_Image1+0x94c>)
30007646:	f004 fd73 	bl	3000c130 <__DiagPrintf_veneer>
3000764a:	e678      	b.n	3000733e <BOOT_Image1+0x4be>
3000764c:	f000 fadc 	bl	30007c08 <ymodem_uart_readable>
30007650:	2800      	cmp	r0, #0
30007652:	d0f4      	beq.n	3000763e <BOOT_Image1+0x7be>
30007654:	9603      	str	r6, [sp, #12]
30007656:	e526      	b.n	300070a6 <BOOT_Image1+0x226>
30007658:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000765c:	495c      	ldr	r1, [pc, #368]	; (300077d0 <BOOT_Image1+0x950>)
3000765e:	685a      	ldr	r2, [r3, #4]
30007660:	f042 0202 	orr.w	r2, r2, #2
30007664:	605a      	str	r2, [r3, #4]
30007666:	685a      	ldr	r2, [r3, #4]
30007668:	f042 0230 	orr.w	r2, r2, #48	; 0x30
3000766c:	605a      	str	r2, [r3, #4]
3000766e:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30007670:	f043 0303 	orr.w	r3, r3, #3
30007674:	678b      	str	r3, [r1, #120]	; 0x78
30007676:	e682      	b.n	3000737e <BOOT_Image1+0x4fe>
30007678:	4856      	ldr	r0, [pc, #344]	; (300077d4 <BOOT_Image1+0x954>)
3000767a:	f004 fd59 	bl	3000c130 <__DiagPrintf_veneer>
3000767e:	e6ff      	b.n	30007480 <BOOT_Image1+0x600>
30007680:	4855      	ldr	r0, [pc, #340]	; (300077d8 <BOOT_Image1+0x958>)
30007682:	f004 fd55 	bl	3000c130 <__DiagPrintf_veneer>
30007686:	e4b2      	b.n	30006fee <BOOT_Image1+0x16e>
30007688:	6861      	ldr	r1, [r4, #4]
3000768a:	4854      	ldr	r0, [pc, #336]	; (300077dc <BOOT_Image1+0x95c>)
3000768c:	f004 fd50 	bl	3000c130 <__DiagPrintf_veneer>
30007690:	6860      	ldr	r0, [r4, #4]
30007692:	f7fd fe7f 	bl	30005394 <BOOT_NsStart>
30007696:	b00b      	add	sp, #44	; 0x2c
30007698:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000769c:	4f50      	ldr	r7, [pc, #320]	; (300077e0 <BOOT_Image1+0x960>)
3000769e:	4606      	mov	r6, r0
300076a0:	f04f 090b 	mov.w	r9, #11
300076a4:	4682      	mov	sl, r0
300076a6:	f207 4344 	addw	r3, r7, #1092	; 0x444
300076aa:	f04f 080a 	mov.w	r8, #10
300076ae:	9302      	str	r3, [sp, #8]
300076b0:	2004      	movs	r0, #4
300076b2:	f000 fa81 	bl	30007bb8 <ymodem_uart_port_deinit>
300076b6:	9a02      	ldr	r2, [sp, #8]
300076b8:	9600      	str	r6, [sp, #0]
300076ba:	4613      	mov	r3, r2
300076bc:	eb02 0008 	add.w	r0, r2, r8
300076c0:	f8d7 14d0 	ldr.w	r1, [r7, #1232]	; 0x4d0
300076c4:	4652      	mov	r2, sl
300076c6:	444b      	add	r3, r9
300076c8:	f7ff fb34 	bl	30006d34 <password_hash_check>
300076cc:	4680      	mov	r8, r0
300076ce:	2800      	cmp	r0, #0
300076d0:	d1b5      	bne.n	3000763e <BOOT_Image1+0x7be>
300076d2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300076d6:	47a0      	blx	r4
300076d8:	4842      	ldr	r0, [pc, #264]	; (300077e4 <BOOT_Image1+0x964>)
300076da:	f004 fd19 	bl	3000c110 <____wrap_strlen_veneer>
300076de:	4601      	mov	r1, r0
300076e0:	4840      	ldr	r0, [pc, #256]	; (300077e4 <BOOT_Image1+0x964>)
300076e2:	f000 faab 	bl	30007c3c <ymodem_uart_putdata>
300076e6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300076ea:	47a0      	blx	r4
300076ec:	f8d7 24d4 	ldr.w	r2, [r7, #1236]	; 0x4d4
300076f0:	493d      	ldr	r1, [pc, #244]	; (300077e8 <BOOT_Image1+0x968>)
300076f2:	2002      	movs	r0, #2
300076f4:	f000 fb42 	bl	30007d7c <ymodem_img_download>
300076f8:	2032      	movs	r0, #50	; 0x32
300076fa:	47a0      	blx	r4
300076fc:	f8d7 34d4 	ldr.w	r3, [r7, #1236]	; 0x4d4
30007700:	b1e3      	cbz	r3, 3000773c <BOOT_Image1+0x8bc>
30007702:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 300077c4 <BOOT_Image1+0x944>
30007706:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
3000770a:	f859 6b04 	ldr.w	r6, [r9], #4
3000770e:	0b36      	lsrs	r6, r6, #12
30007710:	d00c      	beq.n	3000772c <BOOT_Image1+0x8ac>
30007712:	2500      	movs	r5, #0
30007714:	f8da 1000 	ldr.w	r1, [sl]
30007718:	2002      	movs	r0, #2
3000771a:	eb01 3105 	add.w	r1, r1, r5, lsl #12
3000771e:	3501      	adds	r5, #1
30007720:	f7fd fc4c 	bl	30004fbc <FLASH_Erase_With_Lock>
30007724:	2032      	movs	r0, #50	; 0x32
30007726:	47a0      	blx	r4
30007728:	42ae      	cmp	r6, r5
3000772a:	d1f3      	bne.n	30007714 <BOOT_Image1+0x894>
3000772c:	f108 0801 	add.w	r8, r8, #1
30007730:	f8d7 34d4 	ldr.w	r3, [r7, #1236]	; 0x4d4
30007734:	f10a 0a04 	add.w	sl, sl, #4
30007738:	4543      	cmp	r3, r8
3000773a:	d8e6      	bhi.n	3000770a <BOOT_Image1+0x88a>
3000773c:	482b      	ldr	r0, [pc, #172]	; (300077ec <BOOT_Image1+0x96c>)
3000773e:	f004 fcf7 	bl	3000c130 <__DiagPrintf_veneer>
30007742:	4b2b      	ldr	r3, [pc, #172]	; (300077f0 <BOOT_Image1+0x970>)
30007744:	a808      	add	r0, sp, #32
30007746:	4798      	blx	r3
30007748:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
3000774c:	4829      	ldr	r0, [pc, #164]	; (300077f4 <BOOT_Image1+0x974>)
3000774e:	a908      	add	r1, sp, #32
30007750:	4b29      	ldr	r3, [pc, #164]	; (300077f8 <BOOT_Image1+0x978>)
30007752:	f8ad 2022 	strh.w	r2, [sp, #34]	; 0x22
30007756:	4798      	blx	r3
30007758:	4826      	ldr	r0, [pc, #152]	; (300077f4 <BOOT_Image1+0x974>)
3000775a:	4b28      	ldr	r3, [pc, #160]	; (300077fc <BOOT_Image1+0x97c>)
3000775c:	4798      	blx	r3
3000775e:	e7fe      	b.n	3000775e <BOOT_Image1+0x8de>
30007760:	4827      	ldr	r0, [pc, #156]	; (30007800 <BOOT_Image1+0x980>)
30007762:	f004 fce5 	bl	3000c130 <__DiagPrintf_veneer>
30007766:	e5e6      	b.n	30007336 <BOOT_Image1+0x4b6>
30007768:	4826      	ldr	r0, [pc, #152]	; (30007804 <BOOT_Image1+0x984>)
3000776a:	f004 fce1 	bl	3000c130 <__DiagPrintf_veneer>
3000776e:	4817      	ldr	r0, [pc, #92]	; (300077cc <BOOT_Image1+0x94c>)
30007770:	f004 fcde 	bl	3000c130 <__DiagPrintf_veneer>
30007774:	e5df      	b.n	30007336 <BOOT_Image1+0x4b6>
30007776:	4824      	ldr	r0, [pc, #144]	; (30007808 <BOOT_Image1+0x988>)
30007778:	f004 fcda 	bl	3000c130 <__DiagPrintf_veneer>
3000777c:	e5db      	b.n	30007336 <BOOT_Image1+0x4b6>
3000777e:	f04f 0800 	mov.w	r8, #0
30007782:	f04f 090a 	mov.w	r9, #10
30007786:	4643      	mov	r3, r8
30007788:	46c3      	mov	fp, r8
3000778a:	e52f      	b.n	300071ec <BOOT_Image1+0x36c>
3000778c:	46d3      	mov	fp, sl
3000778e:	e58b      	b.n	300072a8 <BOOT_Image1+0x428>
30007790:	2300      	movs	r3, #0
30007792:	46da      	mov	sl, fp
30007794:	9305      	str	r3, [sp, #20]
30007796:	e55d      	b.n	30007254 <BOOT_Image1+0x3d4>
30007798:	f108 090b 	add.w	r9, r8, #11
3000779c:	f8dd a014 	ldr.w	sl, [sp, #20]
300077a0:	f108 080a 	add.w	r8, r8, #10
300077a4:	44d1      	add	r9, sl
300077a6:	e783      	b.n	300076b0 <BOOT_Image1+0x830>
300077a8:	0000aea1 	.word	0x0000aea1
300077ac:	0000aab9 	.word	0x0000aab9
300077b0:	4200c000 	.word	0x4200c000
300077b4:	0000aaa5 	.word	0x0000aaa5
300077b8:	3000e188 	.word	0x3000e188
300077bc:	3000e1b0 	.word	0x3000e1b0
300077c0:	000129bd 	.word	0x000129bd
300077c4:	3000e1b4 	.word	0x3000e1b4
300077c8:	3000e1dc 	.word	0x3000e1dc
300077cc:	3000cf68 	.word	0x3000cf68
300077d0:	41000200 	.word	0x41000200
300077d4:	3000ce48 	.word	0x3000ce48
300077d8:	3000ced0 	.word	0x3000ced0
300077dc:	3000cfc0 	.word	0x3000cfc0
300077e0:	3000dc6c 	.word	0x3000dc6c
300077e4:	3000e994 	.word	0x3000e994
300077e8:	3000e148 	.word	0x3000e148
300077ec:	3000cfa0 	.word	0x3000cfa0
300077f0:	0000d3f9 	.word	0x0000d3f9
300077f4:	41000440 	.word	0x41000440
300077f8:	0000d409 	.word	0x0000d409
300077fc:	0000d489 	.word	0x0000d489
30007800:	3000cf78 	.word	0x3000cf78
30007804:	3000cf50 	.word	0x3000cf50
30007808:	3000cf8c 	.word	0x3000cf8c
3000780c:	0000c0f9 	.word	0x0000c0f9

30007810 <ymodem_inquiry>:
30007810:	2806      	cmp	r0, #6
30007812:	b510      	push	{r4, lr}
30007814:	4604      	mov	r4, r0
30007816:	d003      	beq.n	30007820 <ymodem_inquiry+0x10>
30007818:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000781c:	4b03      	ldr	r3, [pc, #12]	; (3000782c <ymodem_inquiry+0x1c>)
3000781e:	4798      	blx	r3
30007820:	4620      	mov	r0, r4
30007822:	f000 fa07 	bl	30007c34 <ymodem_uart_putc>
30007826:	2001      	movs	r0, #1
30007828:	bd10      	pop	{r4, pc}
3000782a:	bf00      	nop
3000782c:	00009b2d 	.word	0x00009b2d

30007830 <ymodem_get_first>:
30007830:	b530      	push	{r4, r5, lr}
30007832:	b083      	sub	sp, #12
30007834:	2300      	movs	r3, #0
30007836:	4604      	mov	r4, r0
30007838:	4936      	ldr	r1, [pc, #216]	; (30007914 <ymodem_get_first+0xe4>)
3000783a:	f10d 0006 	add.w	r0, sp, #6
3000783e:	f88d 3006 	strb.w	r3, [sp, #6]
30007842:	f000 fa19 	bl	30007c78 <ymodem_uart_getc_to>
30007846:	b9d0      	cbnz	r0, 3000787e <ymodem_get_first+0x4e>
30007848:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000784c:	3b01      	subs	r3, #1
3000784e:	2b17      	cmp	r3, #23
30007850:	d80d      	bhi.n	3000786e <ymodem_get_first+0x3e>
30007852:	e8df f003 	tbb	[pc, r3]
30007856:	231e      	.short	0x231e
30007858:	0c0c290c 	.word	0x0c0c290c
3000785c:	0c0c0c0c 	.word	0x0c0c0c0c
30007860:	0c0c0c0c 	.word	0x0c0c0c0c
30007864:	0c0c0c0c 	.word	0x0c0c0c0c
30007868:	0c0c0c0c 	.word	0x0c0c0c0c
3000786c:	190c      	.short	0x190c
3000786e:	4d29      	ldr	r5, [pc, #164]	; (30007914 <ymodem_get_first+0xe4>)
30007870:	4629      	mov	r1, r5
30007872:	f10d 0007 	add.w	r0, sp, #7
30007876:	f000 f9ff 	bl	30007c78 <ymodem_uart_getc_to>
3000787a:	2800      	cmp	r0, #0
3000787c:	d0f8      	beq.n	30007870 <ymodem_get_first+0x40>
3000787e:	2301      	movs	r3, #1
30007880:	2008      	movs	r0, #8
30007882:	60a3      	str	r3, [r4, #8]
30007884:	b003      	add	sp, #12
30007886:	bd30      	pop	{r4, r5, pc}
30007888:	2301      	movs	r3, #1
3000788a:	2002      	movs	r0, #2
3000788c:	60a3      	str	r3, [r4, #8]
3000788e:	b003      	add	sp, #12
30007890:	bd30      	pop	{r4, r5, pc}
30007892:	2380      	movs	r3, #128	; 0x80
30007894:	2001      	movs	r0, #1
30007896:	80a3      	strh	r3, [r4, #4]
30007898:	b003      	add	sp, #12
3000789a:	bd30      	pop	{r4, r5, pc}
3000789c:	f44f 6380 	mov.w	r3, #1024	; 0x400
300078a0:	2001      	movs	r0, #1
300078a2:	80a3      	strh	r3, [r4, #4]
300078a4:	b003      	add	sp, #12
300078a6:	bd30      	pop	{r4, r5, pc}
300078a8:	2006      	movs	r0, #6
300078aa:	4d1b      	ldr	r5, [pc, #108]	; (30007918 <ymodem_get_first+0xe8>)
300078ac:	f000 f9c2 	bl	30007c34 <ymodem_uart_putc>
300078b0:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078b4:	47a8      	blx	r5
300078b6:	2043      	movs	r0, #67	; 0x43
300078b8:	f000 f9bc 	bl	30007c34 <ymodem_uart_putc>
300078bc:	2006      	movs	r0, #6
300078be:	f000 f9b9 	bl	30007c34 <ymodem_uart_putc>
300078c2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078c6:	47a8      	blx	r5
300078c8:	2043      	movs	r0, #67	; 0x43
300078ca:	f000 f9b3 	bl	30007c34 <ymodem_uart_putc>
300078ce:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078d2:	47a8      	blx	r5
300078d4:	2018      	movs	r0, #24
300078d6:	f000 f9ad 	bl	30007c34 <ymodem_uart_putc>
300078da:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078de:	47a8      	blx	r5
300078e0:	2018      	movs	r0, #24
300078e2:	f000 f9a7 	bl	30007c34 <ymodem_uart_putc>
300078e6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078ea:	47a8      	blx	r5
300078ec:	2018      	movs	r0, #24
300078ee:	f000 f9a1 	bl	30007c34 <ymodem_uart_putc>
300078f2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078f6:	47a8      	blx	r5
300078f8:	2018      	movs	r0, #24
300078fa:	f000 f99b 	bl	30007c34 <ymodem_uart_putc>
300078fe:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007902:	47a8      	blx	r5
30007904:	2018      	movs	r0, #24
30007906:	f000 f995 	bl	30007c34 <ymodem_uart_putc>
3000790a:	2301      	movs	r3, #1
3000790c:	2005      	movs	r0, #5
3000790e:	60a3      	str	r3, [r4, #8]
30007910:	b003      	add	sp, #12
30007912:	bd30      	pop	{r4, r5, pc}
30007914:	000caa30 	.word	0x000caa30
30007918:	00009b2d 	.word	0x00009b2d

3000791c <ymodem_get_others>:
3000791c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007920:	b083      	sub	sp, #12
30007922:	2300      	movs	r3, #0
30007924:	4605      	mov	r5, r0
30007926:	4952      	ldr	r1, [pc, #328]	; (30007a70 <ymodem_get_others+0x154>)
30007928:	a801      	add	r0, sp, #4
3000792a:	f88d 3006 	strb.w	r3, [sp, #6]
3000792e:	f88d 3007 	strb.w	r3, [sp, #7]
30007932:	f000 f9a1 	bl	30007c78 <ymodem_uart_getc_to>
30007936:	bb38      	cbnz	r0, 30007988 <ymodem_get_others+0x6c>
30007938:	494d      	ldr	r1, [pc, #308]	; (30007a70 <ymodem_get_others+0x154>)
3000793a:	f10d 0005 	add.w	r0, sp, #5
3000793e:	f000 f99b 	bl	30007c78 <ymodem_uart_getc_to>
30007942:	4607      	mov	r7, r0
30007944:	bb00      	cbnz	r0, 30007988 <ymodem_get_others+0x6c>
30007946:	f89d 2004 	ldrb.w	r2, [sp, #4]
3000794a:	f89d 6005 	ldrb.w	r6, [sp, #5]
3000794e:	43d3      	mvns	r3, r2
30007950:	b2db      	uxtb	r3, r3
30007952:	429e      	cmp	r6, r3
30007954:	bf0c      	ite	eq
30007956:	2601      	moveq	r6, #1
30007958:	2604      	movne	r6, #4
3000795a:	682b      	ldr	r3, [r5, #0]
3000795c:	429a      	cmp	r2, r3
3000795e:	d21a      	bcs.n	30007996 <ymodem_get_others+0x7a>
30007960:	2701      	movs	r7, #1
30007962:	88ab      	ldrh	r3, [r5, #4]
30007964:	b1d3      	cbz	r3, 3000799c <ymodem_get_others+0x80>
30007966:	2400      	movs	r4, #0
30007968:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007a74 <ymodem_get_others+0x158>
3000796c:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007a70 <ymodem_get_others+0x154>
30007970:	e002      	b.n	30007978 <ymodem_get_others+0x5c>
30007972:	88ab      	ldrh	r3, [r5, #4]
30007974:	42a3      	cmp	r3, r4
30007976:	d911      	bls.n	3000799c <ymodem_get_others+0x80>
30007978:	eb04 0009 	add.w	r0, r4, r9
3000797c:	4641      	mov	r1, r8
3000797e:	3401      	adds	r4, #1
30007980:	f000 f97a 	bl	30007c78 <ymodem_uart_getc_to>
30007984:	2800      	cmp	r0, #0
30007986:	d0f4      	beq.n	30007972 <ymodem_get_others+0x56>
30007988:	2301      	movs	r3, #1
3000798a:	2608      	movs	r6, #8
3000798c:	60ab      	str	r3, [r5, #8]
3000798e:	4630      	mov	r0, r6
30007990:	b003      	add	sp, #12
30007992:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30007996:	d0e4      	beq.n	30007962 <ymodem_get_others+0x46>
30007998:	2604      	movs	r6, #4
3000799a:	e7e2      	b.n	30007962 <ymodem_get_others+0x46>
3000799c:	4934      	ldr	r1, [pc, #208]	; (30007a70 <ymodem_get_others+0x154>)
3000799e:	f10d 0006 	add.w	r0, sp, #6
300079a2:	f000 f969 	bl	30007c78 <ymodem_uart_getc_to>
300079a6:	2800      	cmp	r0, #0
300079a8:	d1ee      	bne.n	30007988 <ymodem_get_others+0x6c>
300079aa:	4931      	ldr	r1, [pc, #196]	; (30007a70 <ymodem_get_others+0x154>)
300079ac:	f10d 0007 	add.w	r0, sp, #7
300079b0:	f000 f962 	bl	30007c78 <ymodem_uart_getc_to>
300079b4:	2800      	cmp	r0, #0
300079b6:	d1e7      	bne.n	30007988 <ymodem_get_others+0x6c>
300079b8:	f89d 3006 	ldrb.w	r3, [sp, #6]
300079bc:	f89d e007 	ldrb.w	lr, [sp, #7]
300079c0:	88aa      	ldrh	r2, [r5, #4]
300079c2:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
300079c6:	b36a      	cbz	r2, 30007a24 <ymodem_get_others+0x108>
300079c8:	4603      	mov	r3, r0
300079ca:	482a      	ldr	r0, [pc, #168]	; (30007a74 <ymodem_get_others+0x158>)
300079cc:	f241 0c21 	movw	ip, #4129	; 0x1021
300079d0:	eb02 0800 	add.w	r8, r2, r0
300079d4:	7801      	ldrb	r1, [r0, #0]
300079d6:	3001      	adds	r0, #1
300079d8:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
300079dc:	2108      	movs	r1, #8
300079de:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
300079e2:	005b      	lsls	r3, r3, #1
300079e4:	b10c      	cbz	r4, 300079ea <ymodem_get_others+0xce>
300079e6:	ea83 030c 	eor.w	r3, r3, ip
300079ea:	3901      	subs	r1, #1
300079ec:	d1f7      	bne.n	300079de <ymodem_get_others+0xc2>
300079ee:	4580      	cmp	r8, r0
300079f0:	d1f0      	bne.n	300079d4 <ymodem_get_others+0xb8>
300079f2:	b29b      	uxth	r3, r3
300079f4:	459e      	cmp	lr, r3
300079f6:	d00a      	beq.n	30007a0e <ymodem_get_others+0xf2>
300079f8:	4b1f      	ldr	r3, [pc, #124]	; (30007a78 <ymodem_get_others+0x15c>)
300079fa:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079fe:	4798      	blx	r3
30007a00:	2015      	movs	r0, #21
30007a02:	f000 f917 	bl	30007c34 <ymodem_uart_putc>
30007a06:	2301      	movs	r3, #1
30007a08:	2604      	movs	r6, #4
30007a0a:	60ab      	str	r3, [r5, #8]
30007a0c:	e7bf      	b.n	3000798e <ymodem_get_others+0x72>
30007a0e:	b917      	cbnz	r7, 30007a16 <ymodem_get_others+0xfa>
30007a10:	4c1a      	ldr	r4, [pc, #104]	; (30007a7c <ymodem_get_others+0x160>)
30007a12:	6823      	ldr	r3, [r4, #0]
30007a14:	b9b3      	cbnz	r3, 30007a44 <ymodem_get_others+0x128>
30007a16:	f000 f947 	bl	30007ca8 <ymodem_uart_clean_rx>
30007a1a:	2e01      	cmp	r6, #1
30007a1c:	d004      	beq.n	30007a28 <ymodem_get_others+0x10c>
30007a1e:	2e04      	cmp	r6, #4
30007a20:	d1b5      	bne.n	3000798e <ymodem_get_others+0x72>
30007a22:	e7e9      	b.n	300079f8 <ymodem_get_others+0xdc>
30007a24:	4613      	mov	r3, r2
30007a26:	e7e5      	b.n	300079f4 <ymodem_get_others+0xd8>
30007a28:	4c14      	ldr	r4, [pc, #80]	; (30007a7c <ymodem_get_others+0x160>)
30007a2a:	2006      	movs	r0, #6
30007a2c:	60ae      	str	r6, [r5, #8]
30007a2e:	f000 f901 	bl	30007c34 <ymodem_uart_putc>
30007a32:	6823      	ldr	r3, [r4, #0]
30007a34:	b193      	cbz	r3, 30007a5c <ymodem_get_others+0x140>
30007a36:	2f00      	cmp	r7, #0
30007a38:	f103 0301 	add.w	r3, r3, #1
30007a3c:	bf18      	it	ne
30007a3e:	2604      	movne	r6, #4
30007a40:	6023      	str	r3, [r4, #0]
30007a42:	e7a4      	b.n	3000798e <ymodem_get_others+0x72>
30007a44:	68eb      	ldr	r3, [r5, #12]
30007a46:	6861      	ldr	r1, [r4, #4]
30007a48:	3301      	adds	r3, #1
30007a4a:	480a      	ldr	r0, [pc, #40]	; (30007a74 <ymodem_get_others+0x158>)
30007a4c:	60eb      	str	r3, [r5, #12]
30007a4e:	f000 f94f 	bl	30007cf0 <UARTIMG_Write>
30007a52:	6862      	ldr	r2, [r4, #4]
30007a54:	88ab      	ldrh	r3, [r5, #4]
30007a56:	4413      	add	r3, r2
30007a58:	6063      	str	r3, [r4, #4]
30007a5a:	e7dc      	b.n	30007a16 <ymodem_get_others+0xfa>
30007a5c:	4b06      	ldr	r3, [pc, #24]	; (30007a78 <ymodem_get_others+0x15c>)
30007a5e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a62:	4798      	blx	r3
30007a64:	2043      	movs	r0, #67	; 0x43
30007a66:	f000 f8e5 	bl	30007c34 <ymodem_uart_putc>
30007a6a:	6823      	ldr	r3, [r4, #0]
30007a6c:	e7e3      	b.n	30007a36 <ymodem_get_others+0x11a>
30007a6e:	bf00      	nop
30007a70:	000caa30 	.word	0x000caa30
30007a74:	20016ffc 	.word	0x20016ffc
30007a78:	00009b2d 	.word	0x00009b2d
30007a7c:	3000e1ec 	.word	0x3000e1ec

30007a80 <ymodem_rx_frame>:
30007a80:	2300      	movs	r3, #0
30007a82:	2201      	movs	r2, #1
30007a84:	b510      	push	{r4, lr}
30007a86:	4604      	mov	r4, r0
30007a88:	6083      	str	r3, [r0, #8]
30007a8a:	8082      	strh	r2, [r0, #4]
30007a8c:	f7ff fed0 	bl	30007830 <ymodem_get_first>
30007a90:	68a3      	ldr	r3, [r4, #8]
30007a92:	b92b      	cbnz	r3, 30007aa0 <ymodem_rx_frame+0x20>
30007a94:	4620      	mov	r0, r4
30007a96:	f7ff ff41 	bl	3000791c <ymodem_get_others>
30007a9a:	68a3      	ldr	r3, [r4, #8]
30007a9c:	2b00      	cmp	r3, #0
30007a9e:	d0f9      	beq.n	30007a94 <ymodem_rx_frame+0x14>
30007aa0:	2801      	cmp	r0, #1
30007aa2:	d000      	beq.n	30007aa6 <ymodem_rx_frame+0x26>
30007aa4:	bd10      	pop	{r4, pc}
30007aa6:	6823      	ldr	r3, [r4, #0]
30007aa8:	3301      	adds	r3, #1
30007aaa:	b2db      	uxtb	r3, r3
30007aac:	6023      	str	r3, [r4, #0]
30007aae:	bd10      	pop	{r4, pc}

30007ab0 <ymodem_img_rxbuffer>:
30007ab0:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007ab4:	2300      	movs	r3, #0
30007ab6:	4c28      	ldr	r4, [pc, #160]	; (30007b58 <ymodem_img_rxbuffer+0xa8>)
30007ab8:	4e28      	ldr	r6, [pc, #160]	; (30007b5c <ymodem_img_rxbuffer+0xac>)
30007aba:	460d      	mov	r5, r1
30007abc:	60a3      	str	r3, [r4, #8]
30007abe:	6163      	str	r3, [r4, #20]
30007ac0:	f000 f8f2 	bl	30007ca8 <ymodem_uart_clean_rx>
30007ac4:	2043      	movs	r0, #67	; 0x43
30007ac6:	f000 f8b5 	bl	30007c34 <ymodem_uart_putc>
30007aca:	f24c 3050 	movw	r0, #50000	; 0xc350
30007ace:	47b0      	blx	r6
30007ad0:	f000 f89a 	bl	30007c08 <ymodem_uart_readable>
30007ad4:	2800      	cmp	r0, #0
30007ad6:	d0f5      	beq.n	30007ac4 <ymodem_img_rxbuffer+0x14>
30007ad8:	2700      	movs	r7, #0
30007ada:	f04f 0801 	mov.w	r8, #1
30007ade:	e9c4 7500 	strd	r7, r5, [r4]
30007ae2:	4d1f      	ldr	r5, [pc, #124]	; (30007b60 <ymodem_img_rxbuffer+0xb0>)
30007ae4:	4628      	mov	r0, r5
30007ae6:	f8a4 800c 	strh.w	r8, [r4, #12]
30007aea:	6127      	str	r7, [r4, #16]
30007aec:	f7ff fea0 	bl	30007830 <ymodem_get_first>
30007af0:	6923      	ldr	r3, [r4, #16]
30007af2:	b92b      	cbnz	r3, 30007b00 <ymodem_img_rxbuffer+0x50>
30007af4:	4628      	mov	r0, r5
30007af6:	f7ff ff11 	bl	3000791c <ymodem_get_others>
30007afa:	6923      	ldr	r3, [r4, #16]
30007afc:	2b00      	cmp	r3, #0
30007afe:	d0f9      	beq.n	30007af4 <ymodem_img_rxbuffer+0x44>
30007b00:	2801      	cmp	r0, #1
30007b02:	d024      	beq.n	30007b4e <ymodem_img_rxbuffer+0x9e>
30007b04:	3804      	subs	r0, #4
30007b06:	2804      	cmp	r0, #4
30007b08:	d80c      	bhi.n	30007b24 <ymodem_img_rxbuffer+0x74>
30007b0a:	a301      	add	r3, pc, #4	; (adr r3, 30007b10 <ymodem_img_rxbuffer+0x60>)
30007b0c:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007b10:	30007ae5 	.word	0x30007ae5
30007b14:	30007b31 	.word	0x30007b31
30007b18:	30007b25 	.word	0x30007b25
30007b1c:	30007b25 	.word	0x30007b25
30007b20:	30007b3d 	.word	0x30007b3d
30007b24:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007b28:	47b0      	blx	r6
30007b2a:	2018      	movs	r0, #24
30007b2c:	f000 f882 	bl	30007c34 <ymodem_uart_putc>
30007b30:	200a      	movs	r0, #10
30007b32:	f000 f8c9 	bl	30007cc8 <ymodem_uart_waitbusy>
30007b36:	2001      	movs	r0, #1
30007b38:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007b3c:	f000 f8b4 	bl	30007ca8 <ymodem_uart_clean_rx>
30007b40:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007b44:	47b0      	blx	r6
30007b46:	2015      	movs	r0, #21
30007b48:	f000 f874 	bl	30007c34 <ymodem_uart_putc>
30007b4c:	e7ca      	b.n	30007ae4 <ymodem_img_rxbuffer+0x34>
30007b4e:	68a3      	ldr	r3, [r4, #8]
30007b50:	3301      	adds	r3, #1
30007b52:	b2db      	uxtb	r3, r3
30007b54:	60a3      	str	r3, [r4, #8]
30007b56:	e7c5      	b.n	30007ae4 <ymodem_img_rxbuffer+0x34>
30007b58:	3000e1ec 	.word	0x3000e1ec
30007b5c:	00009b2d 	.word	0x00009b2d
30007b60:	3000e1f4 	.word	0x3000e1f4

30007b64 <ymodem_log_open>:
30007b64:	b510      	push	{r4, lr}
30007b66:	2303      	movs	r3, #3
30007b68:	4c04      	ldr	r4, [pc, #16]	; (30007b7c <ymodem_log_open+0x18>)
30007b6a:	2200      	movs	r2, #0
30007b6c:	2108      	movs	r1, #8
30007b6e:	4804      	ldr	r0, [pc, #16]	; (30007b80 <ymodem_log_open+0x1c>)
30007b70:	47a0      	blx	r4
30007b72:	2008      	movs	r0, #8
30007b74:	4b03      	ldr	r3, [pc, #12]	; (30007b84 <ymodem_log_open+0x20>)
30007b76:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b7a:	4718      	bx	r3
30007b7c:	3000a401 	.word	0x3000a401
30007b80:	0000ed39 	.word	0x0000ed39
30007b84:	0000e1bd 	.word	0x0000e1bd

30007b88 <ymodem_uart_port_init>:
30007b88:	b500      	push	{lr}
30007b8a:	b083      	sub	sp, #12
30007b8c:	2008      	movs	r0, #8
30007b8e:	4b06      	ldr	r3, [pc, #24]	; (30007ba8 <ymodem_uart_port_init+0x20>)
30007b90:	9201      	str	r2, [sp, #4]
30007b92:	4798      	blx	r3
30007b94:	4b05      	ldr	r3, [pc, #20]	; (30007bac <ymodem_uart_port_init+0x24>)
30007b96:	2008      	movs	r0, #8
30007b98:	4798      	blx	r3
30007b9a:	9901      	ldr	r1, [sp, #4]
30007b9c:	4804      	ldr	r0, [pc, #16]	; (30007bb0 <ymodem_uart_port_init+0x28>)
30007b9e:	4b05      	ldr	r3, [pc, #20]	; (30007bb4 <ymodem_uart_port_init+0x2c>)
30007ba0:	b003      	add	sp, #12
30007ba2:	f85d eb04 	ldr.w	lr, [sp], #4
30007ba6:	4718      	bx	r3
30007ba8:	0000e1d9 	.word	0x0000e1d9
30007bac:	0000e2fd 	.word	0x0000e2fd
30007bb0:	4200c000 	.word	0x4200c000
30007bb4:	0000a9e5 	.word	0x0000a9e5

30007bb8 <ymodem_uart_port_deinit>:
30007bb8:	2804      	cmp	r0, #4
30007bba:	d004      	beq.n	30007bc6 <ymodem_uart_port_deinit+0xe>
30007bbc:	4b0d      	ldr	r3, [pc, #52]	; (30007bf4 <ymodem_uart_port_deinit+0x3c>)
30007bbe:	681b      	ldr	r3, [r3, #0]
30007bc0:	039b      	lsls	r3, r3, #14
30007bc2:	d40f      	bmi.n	30007be4 <ymodem_uart_port_deinit+0x2c>
30007bc4:	4770      	bx	lr
30007bc6:	2200      	movs	r2, #0
30007bc8:	2303      	movs	r3, #3
30007bca:	2108      	movs	r1, #8
30007bcc:	480a      	ldr	r0, [pc, #40]	; (30007bf8 <ymodem_uart_port_deinit+0x40>)
30007bce:	b510      	push	{r4, lr}
30007bd0:	4c0a      	ldr	r4, [pc, #40]	; (30007bfc <ymodem_uart_port_deinit+0x44>)
30007bd2:	47a0      	blx	r4
30007bd4:	4b0a      	ldr	r3, [pc, #40]	; (30007c00 <ymodem_uart_port_deinit+0x48>)
30007bd6:	2008      	movs	r0, #8
30007bd8:	4798      	blx	r3
30007bda:	4b06      	ldr	r3, [pc, #24]	; (30007bf4 <ymodem_uart_port_deinit+0x3c>)
30007bdc:	681b      	ldr	r3, [r3, #0]
30007bde:	039a      	lsls	r2, r3, #14
30007be0:	d403      	bmi.n	30007bea <ymodem_uart_port_deinit+0x32>
30007be2:	bd10      	pop	{r4, pc}
30007be4:	4807      	ldr	r0, [pc, #28]	; (30007c04 <ymodem_uart_port_deinit+0x4c>)
30007be6:	f004 baa3 	b.w	3000c130 <__DiagPrintf_veneer>
30007bea:	4806      	ldr	r0, [pc, #24]	; (30007c04 <ymodem_uart_port_deinit+0x4c>)
30007bec:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007bf0:	f004 ba9e 	b.w	3000c130 <__DiagPrintf_veneer>
30007bf4:	2001c00c 	.word	0x2001c00c
30007bf8:	0000ed39 	.word	0x0000ed39
30007bfc:	3000a401 	.word	0x3000a401
30007c00:	0000e1bd 	.word	0x0000e1bd
30007c04:	3000cff8 	.word	0x3000cff8

30007c08 <ymodem_uart_readable>:
30007c08:	4b00      	ldr	r3, [pc, #0]	; (30007c0c <ymodem_uart_readable+0x4>)
30007c0a:	4718      	bx	r3
30007c0c:	0000ac39 	.word	0x0000ac39

30007c10 <ymodem_uart_writable>:
30007c10:	4a03      	ldr	r2, [pc, #12]	; (30007c20 <ymodem_uart_writable+0x10>)
30007c12:	b508      	push	{r3, lr}
30007c14:	4b03      	ldr	r3, [pc, #12]	; (30007c24 <ymodem_uart_writable+0x14>)
30007c16:	6810      	ldr	r0, [r2, #0]
30007c18:	4798      	blx	r3
30007c1a:	b2c0      	uxtb	r0, r0
30007c1c:	bd08      	pop	{r3, pc}
30007c1e:	bf00      	nop
30007c20:	3000e204 	.word	0x3000e204
30007c24:	0000d1a9 	.word	0x0000d1a9

30007c28 <ymodem_uart_getc>:
30007c28:	2000      	movs	r0, #0
30007c2a:	4b01      	ldr	r3, [pc, #4]	; (30007c30 <ymodem_uart_getc+0x8>)
30007c2c:	4718      	bx	r3
30007c2e:	bf00      	nop
30007c30:	0000ac1d 	.word	0x0000ac1d

30007c34 <ymodem_uart_putc>:
30007c34:	4b00      	ldr	r3, [pc, #0]	; (30007c38 <ymodem_uart_putc+0x4>)
30007c36:	4718      	bx	r3
30007c38:	0000abb5 	.word	0x0000abb5

30007c3c <ymodem_uart_putdata>:
30007c3c:	b149      	cbz	r1, 30007c52 <ymodem_uart_putdata+0x16>
30007c3e:	b570      	push	{r4, r5, r6, lr}
30007c40:	4604      	mov	r4, r0
30007c42:	1845      	adds	r5, r0, r1
30007c44:	4e03      	ldr	r6, [pc, #12]	; (30007c54 <ymodem_uart_putdata+0x18>)
30007c46:	f814 0b01 	ldrb.w	r0, [r4], #1
30007c4a:	47b0      	blx	r6
30007c4c:	42ac      	cmp	r4, r5
30007c4e:	d1fa      	bne.n	30007c46 <ymodem_uart_putdata+0xa>
30007c50:	bd70      	pop	{r4, r5, r6, pc}
30007c52:	4770      	bx	lr
30007c54:	0000abb5 	.word	0x0000abb5

30007c58 <ymodem_uart_getdata>:
30007c58:	b151      	cbz	r1, 30007c70 <ymodem_uart_getdata+0x18>
30007c5a:	b570      	push	{r4, r5, r6, lr}
30007c5c:	4604      	mov	r4, r0
30007c5e:	1845      	adds	r5, r0, r1
30007c60:	4e04      	ldr	r6, [pc, #16]	; (30007c74 <ymodem_uart_getdata+0x1c>)
30007c62:	2001      	movs	r0, #1
30007c64:	47b0      	blx	r6
30007c66:	f804 0b01 	strb.w	r0, [r4], #1
30007c6a:	42ac      	cmp	r4, r5
30007c6c:	d1f9      	bne.n	30007c62 <ymodem_uart_getdata+0xa>
30007c6e:	bd70      	pop	{r4, r5, r6, pc}
30007c70:	4770      	bx	lr
30007c72:	bf00      	nop
30007c74:	0000ac1d 	.word	0x0000ac1d

30007c78 <ymodem_uart_getc_to>:
30007c78:	b570      	push	{r4, r5, r6, lr}
30007c7a:	4d09      	ldr	r5, [pc, #36]	; (30007ca0 <ymodem_uart_getc_to+0x28>)
30007c7c:	4606      	mov	r6, r0
30007c7e:	460c      	mov	r4, r1
30007c80:	e001      	b.n	30007c86 <ymodem_uart_getc_to+0xe>
30007c82:	3c01      	subs	r4, #1
30007c84:	d009      	beq.n	30007c9a <ymodem_uart_getc_to+0x22>
30007c86:	47a8      	blx	r5
30007c88:	2800      	cmp	r0, #0
30007c8a:	d0fa      	beq.n	30007c82 <ymodem_uart_getc_to+0xa>
30007c8c:	4b05      	ldr	r3, [pc, #20]	; (30007ca4 <ymodem_uart_getc_to+0x2c>)
30007c8e:	2000      	movs	r0, #0
30007c90:	4798      	blx	r3
30007c92:	4603      	mov	r3, r0
30007c94:	2000      	movs	r0, #0
30007c96:	7033      	strb	r3, [r6, #0]
30007c98:	bd70      	pop	{r4, r5, r6, pc}
30007c9a:	2001      	movs	r0, #1
30007c9c:	bd70      	pop	{r4, r5, r6, pc}
30007c9e:	bf00      	nop
30007ca0:	0000ac39 	.word	0x0000ac39
30007ca4:	0000ac1d 	.word	0x0000ac1d

30007ca8 <ymodem_uart_clean_rx>:
30007ca8:	b538      	push	{r3, r4, r5, lr}
30007caa:	4c05      	ldr	r4, [pc, #20]	; (30007cc0 <ymodem_uart_clean_rx+0x18>)
30007cac:	4d05      	ldr	r5, [pc, #20]	; (30007cc4 <ymodem_uart_clean_rx+0x1c>)
30007cae:	e000      	b.n	30007cb2 <ymodem_uart_clean_rx+0xa>
30007cb0:	47a8      	blx	r5
30007cb2:	47a0      	blx	r4
30007cb4:	4603      	mov	r3, r0
30007cb6:	2000      	movs	r0, #0
30007cb8:	2b00      	cmp	r3, #0
30007cba:	d1f9      	bne.n	30007cb0 <ymodem_uart_clean_rx+0x8>
30007cbc:	bd38      	pop	{r3, r4, r5, pc}
30007cbe:	bf00      	nop
30007cc0:	0000ac39 	.word	0x0000ac39
30007cc4:	0000ac1d 	.word	0x0000ac1d

30007cc8 <ymodem_uart_waitbusy>:
30007cc8:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007cca:	2400      	movs	r4, #0
30007ccc:	4e06      	ldr	r6, [pc, #24]	; (30007ce8 <ymodem_uart_waitbusy+0x20>)
30007cce:	4f07      	ldr	r7, [pc, #28]	; (30007cec <ymodem_uart_waitbusy+0x24>)
30007cd0:	4605      	mov	r5, r0
30007cd2:	e002      	b.n	30007cda <ymodem_uart_waitbusy+0x12>
30007cd4:	47b8      	blx	r7
30007cd6:	42ac      	cmp	r4, r5
30007cd8:	d804      	bhi.n	30007ce4 <ymodem_uart_waitbusy+0x1c>
30007cda:	6973      	ldr	r3, [r6, #20]
30007cdc:	3401      	adds	r4, #1
30007cde:	2064      	movs	r0, #100	; 0x64
30007ce0:	03db      	lsls	r3, r3, #15
30007ce2:	d5f7      	bpl.n	30007cd4 <ymodem_uart_waitbusy+0xc>
30007ce4:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007ce6:	bf00      	nop
30007ce8:	4200c000 	.word	0x4200c000
30007cec:	00009b2d 	.word	0x00009b2d

30007cf0 <UARTIMG_Write>:
30007cf0:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007cf4:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007cf8:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007cfc:	d21d      	bcs.n	30007d3a <UARTIMG_Write+0x4a>
30007cfe:	3a01      	subs	r2, #1
30007d00:	4f19      	ldr	r7, [pc, #100]	; (30007d68 <UARTIMG_Write+0x78>)
30007d02:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007d78 <UARTIMG_Write+0x88>
30007d06:	4688      	mov	r8, r1
30007d08:	1916      	adds	r6, r2, r4
30007d0a:	4605      	mov	r5, r0
30007d0c:	f8d9 3000 	ldr.w	r3, [r9]
30007d10:	403e      	ands	r6, r7
30007d12:	4027      	ands	r7, r4
30007d14:	42bb      	cmp	r3, r7
30007d16:	d91b      	bls.n	30007d50 <UARTIMG_Write+0x60>
30007d18:	42b3      	cmp	r3, r6
30007d1a:	d910      	bls.n	30007d3e <UARTIMG_Write+0x4e>
30007d1c:	4e13      	ldr	r6, [pc, #76]	; (30007d6c <UARTIMG_Write+0x7c>)
30007d1e:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007d22:	4f13      	ldr	r7, [pc, #76]	; (30007d70 <UARTIMG_Write+0x80>)
30007d24:	4446      	add	r6, r8
30007d26:	eba3 0808 	sub.w	r8, r3, r8
30007d2a:	eb08 0204 	add.w	r2, r8, r4
30007d2e:	4620      	mov	r0, r4
30007d30:	3408      	adds	r4, #8
30007d32:	2108      	movs	r1, #8
30007d34:	47b8      	blx	r7
30007d36:	42b4      	cmp	r4, r6
30007d38:	d1f7      	bne.n	30007d2a <UARTIMG_Write+0x3a>
30007d3a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007d3e:	4631      	mov	r1, r6
30007d40:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007d44:	2002      	movs	r0, #2
30007d46:	4b0b      	ldr	r3, [pc, #44]	; (30007d74 <UARTIMG_Write+0x84>)
30007d48:	4798      	blx	r3
30007d4a:	f8c9 6000 	str.w	r6, [r9]
30007d4e:	e7e5      	b.n	30007d1c <UARTIMG_Write+0x2c>
30007d50:	4b08      	ldr	r3, [pc, #32]	; (30007d74 <UARTIMG_Write+0x84>)
30007d52:	4639      	mov	r1, r7
30007d54:	2002      	movs	r0, #2
30007d56:	4798      	blx	r3
30007d58:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007d5c:	42b3      	cmp	r3, r6
30007d5e:	f8c9 3000 	str.w	r3, [r9]
30007d62:	d8db      	bhi.n	30007d1c <UARTIMG_Write+0x2c>
30007d64:	e7eb      	b.n	30007d3e <UARTIMG_Write+0x4e>
30007d66:	bf00      	nop
30007d68:	fffff000 	.word	0xfffff000
30007d6c:	f8000400 	.word	0xf8000400
30007d70:	0000a04d 	.word	0x0000a04d
30007d74:	0000a16d 	.word	0x0000a16d
30007d78:	3000e208 	.word	0x3000e208

30007d7c <ymodem_img_download>:
30007d7c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007d80:	4616      	mov	r6, r2
30007d82:	460d      	mov	r5, r1
30007d84:	4a0a      	ldr	r2, [pc, #40]	; (30007db0 <ymodem_img_download+0x34>)
30007d86:	2100      	movs	r1, #0
30007d88:	4680      	mov	r8, r0
30007d8a:	f7ff fefd 	bl	30007b88 <ymodem_uart_port_init>
30007d8e:	b156      	cbz	r6, 30007da6 <ymodem_img_download+0x2a>
30007d90:	3d04      	subs	r5, #4
30007d92:	2400      	movs	r4, #0
30007d94:	4f06      	ldr	r7, [pc, #24]	; (30007db0 <ymodem_img_download+0x34>)
30007d96:	3401      	adds	r4, #1
30007d98:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007d9c:	4638      	mov	r0, r7
30007d9e:	f7ff fe87 	bl	30007ab0 <ymodem_img_rxbuffer>
30007da2:	42b4      	cmp	r4, r6
30007da4:	d1f7      	bne.n	30007d96 <ymodem_img_download+0x1a>
30007da6:	4640      	mov	r0, r8
30007da8:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007dac:	f7ff bf04 	b.w	30007bb8 <ymodem_uart_port_deinit>
30007db0:	0016e360 	.word	0x0016e360

30007db4 <ChipInfo_Invalid>:
30007db4:	4d07      	ldr	r5, [pc, #28]	; (30007dd4 <ChipInfo_Invalid+0x20>)
30007db6:	4c08      	ldr	r4, [pc, #32]	; (30007dd8 <ChipInfo_Invalid+0x24>)
30007db8:	4e08      	ldr	r6, [pc, #32]	; (30007ddc <ChipInfo_Invalid+0x28>)
30007dba:	b508      	push	{r3, lr}
30007dbc:	e002      	b.n	30007dc4 <ChipInfo_Invalid+0x10>
30007dbe:	f242 7010 	movw	r0, #10000	; 0x2710
30007dc2:	47a0      	blx	r4
30007dc4:	682b      	ldr	r3, [r5, #0]
30007dc6:	039b      	lsls	r3, r3, #14
30007dc8:	d5f9      	bpl.n	30007dbe <ChipInfo_Invalid+0xa>
30007dca:	4630      	mov	r0, r6
30007dcc:	f004 f9b0 	bl	3000c130 <__DiagPrintf_veneer>
30007dd0:	e7f5      	b.n	30007dbe <ChipInfo_Invalid+0xa>
30007dd2:	bf00      	nop
30007dd4:	2001c00c 	.word	0x2001c00c
30007dd8:	00009be5 	.word	0x00009be5
30007ddc:	3000d018 	.word	0x3000d018

30007de0 <ChipInfo_Get>:
30007de0:	b570      	push	{r4, r5, r6, lr}
30007de2:	4c11      	ldr	r4, [pc, #68]	; (30007e28 <ChipInfo_Get+0x48>)
30007de4:	7820      	ldrb	r0, [r4, #0]
30007de6:	28ff      	cmp	r0, #255	; 0xff
30007de8:	d104      	bne.n	30007df4 <ChipInfo_Get+0x14>
30007dea:	4d10      	ldr	r5, [pc, #64]	; (30007e2c <ChipInfo_Get+0x4c>)
30007dec:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007df0:	b108      	cbz	r0, 30007df6 <ChipInfo_Get+0x16>
30007df2:	7020      	strb	r0, [r4, #0]
30007df4:	bd70      	pop	{r4, r5, r6, pc}
30007df6:	4621      	mov	r1, r4
30007df8:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007dfc:	f001 f9c0 	bl	30009180 <OTP_Read8>
30007e00:	7820      	ldrb	r0, [r4, #0]
30007e02:	28ff      	cmp	r0, #255	; 0xff
30007e04:	d002      	beq.n	30007e0c <ChipInfo_Get+0x2c>
30007e06:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007e0a:	bd70      	pop	{r4, r5, r6, pc}
30007e0c:	4d08      	ldr	r5, [pc, #32]	; (30007e30 <ChipInfo_Get+0x50>)
30007e0e:	4c09      	ldr	r4, [pc, #36]	; (30007e34 <ChipInfo_Get+0x54>)
30007e10:	4e09      	ldr	r6, [pc, #36]	; (30007e38 <ChipInfo_Get+0x58>)
30007e12:	e002      	b.n	30007e1a <ChipInfo_Get+0x3a>
30007e14:	f242 7010 	movw	r0, #10000	; 0x2710
30007e18:	47a0      	blx	r4
30007e1a:	682b      	ldr	r3, [r5, #0]
30007e1c:	039b      	lsls	r3, r3, #14
30007e1e:	d5f9      	bpl.n	30007e14 <ChipInfo_Get+0x34>
30007e20:	4630      	mov	r0, r6
30007e22:	f004 f985 	bl	3000c130 <__DiagPrintf_veneer>
30007e26:	e7f5      	b.n	30007e14 <ChipInfo_Get+0x34>
30007e28:	3000e304 	.word	0x3000e304
30007e2c:	23020000 	.word	0x23020000
30007e30:	2001c00c 	.word	0x2001c00c
30007e34:	00009be5 	.word	0x00009be5
30007e38:	3000d038 	.word	0x3000d038

30007e3c <ChipInfo_MemoryType>:
30007e3c:	b510      	push	{r4, lr}
30007e3e:	4c11      	ldr	r4, [pc, #68]	; (30007e84 <ChipInfo_MemoryType+0x48>)
30007e40:	47a0      	blx	r4
30007e42:	2802      	cmp	r0, #2
30007e44:	d019      	beq.n	30007e7a <ChipInfo_MemoryType+0x3e>
30007e46:	47a0      	blx	r4
30007e48:	2803      	cmp	r0, #3
30007e4a:	d016      	beq.n	30007e7a <ChipInfo_MemoryType+0x3e>
30007e4c:	4c0e      	ldr	r4, [pc, #56]	; (30007e88 <ChipInfo_MemoryType+0x4c>)
30007e4e:	4b0f      	ldr	r3, [pc, #60]	; (30007e8c <ChipInfo_MemoryType+0x50>)
30007e50:	4798      	blx	r3
30007e52:	2100      	movs	r1, #0
30007e54:	23e0      	movs	r3, #224	; 0xe0
30007e56:	4622      	mov	r2, r4
30007e58:	e004      	b.n	30007e64 <ChipInfo_MemoryType+0x28>
30007e5a:	7a13      	ldrb	r3, [r2, #8]
30007e5c:	3101      	adds	r1, #1
30007e5e:	3206      	adds	r2, #6
30007e60:	2bff      	cmp	r3, #255	; 0xff
30007e62:	d00c      	beq.n	30007e7e <ChipInfo_MemoryType+0x42>
30007e64:	4298      	cmp	r0, r3
30007e66:	d1f8      	bne.n	30007e5a <ChipInfo_MemoryType+0x1e>
30007e68:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e6c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e70:	78cb      	ldrb	r3, [r1, #3]
30007e72:	2b02      	cmp	r3, #2
30007e74:	d001      	beq.n	30007e7a <ChipInfo_MemoryType+0x3e>
30007e76:	2001      	movs	r0, #1
30007e78:	bd10      	pop	{r4, pc}
30007e7a:	2002      	movs	r0, #2
30007e7c:	bd10      	pop	{r4, pc}
30007e7e:	f7ff ff99 	bl	30007db4 <ChipInfo_Invalid>
30007e82:	bf00      	nop
30007e84:	0000c0f9 	.word	0x0000c0f9
30007e88:	3000d05c 	.word	0x3000d05c
30007e8c:	30007de1 	.word	0x30007de1

30007e90 <ChipInfo_DDRType>:
30007e90:	b510      	push	{r4, lr}
30007e92:	4b0f      	ldr	r3, [pc, #60]	; (30007ed0 <ChipInfo_DDRType+0x40>)
30007e94:	4798      	blx	r3
30007e96:	4b0f      	ldr	r3, [pc, #60]	; (30007ed4 <ChipInfo_DDRType+0x44>)
30007e98:	4604      	mov	r4, r0
30007e9a:	4798      	blx	r3
30007e9c:	2801      	cmp	r0, #1
30007e9e:	d112      	bne.n	30007ec6 <ChipInfo_DDRType+0x36>
30007ea0:	480d      	ldr	r0, [pc, #52]	; (30007ed8 <ChipInfo_DDRType+0x48>)
30007ea2:	2100      	movs	r1, #0
30007ea4:	22e0      	movs	r2, #224	; 0xe0
30007ea6:	4603      	mov	r3, r0
30007ea8:	e003      	b.n	30007eb2 <ChipInfo_DDRType+0x22>
30007eaa:	789a      	ldrb	r2, [r3, #2]
30007eac:	3101      	adds	r1, #1
30007eae:	2aff      	cmp	r2, #255	; 0xff
30007eb0:	d00b      	beq.n	30007eca <ChipInfo_DDRType+0x3a>
30007eb2:	4294      	cmp	r4, r2
30007eb4:	f103 0306 	add.w	r3, r3, #6
30007eb8:	d1f7      	bne.n	30007eaa <ChipInfo_DDRType+0x1a>
30007eba:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ebe:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007ec2:	78c8      	ldrb	r0, [r1, #3]
30007ec4:	bd10      	pop	{r4, pc}
30007ec6:	20ff      	movs	r0, #255	; 0xff
30007ec8:	bd10      	pop	{r4, pc}
30007eca:	2000      	movs	r0, #0
30007ecc:	bd10      	pop	{r4, pc}
30007ece:	bf00      	nop
30007ed0:	30007de1 	.word	0x30007de1
30007ed4:	30007e3d 	.word	0x30007e3d
30007ed8:	3000d05c 	.word	0x3000d05c

30007edc <ChipInfo_MemorySize>:
30007edc:	b510      	push	{r4, lr}
30007ede:	4c0b      	ldr	r4, [pc, #44]	; (30007f0c <ChipInfo_MemorySize+0x30>)
30007ee0:	4b0b      	ldr	r3, [pc, #44]	; (30007f10 <ChipInfo_MemorySize+0x34>)
30007ee2:	4798      	blx	r3
30007ee4:	2100      	movs	r1, #0
30007ee6:	23e0      	movs	r3, #224	; 0xe0
30007ee8:	4622      	mov	r2, r4
30007eea:	e004      	b.n	30007ef6 <ChipInfo_MemorySize+0x1a>
30007eec:	7a13      	ldrb	r3, [r2, #8]
30007eee:	3101      	adds	r1, #1
30007ef0:	3206      	adds	r2, #6
30007ef2:	2bff      	cmp	r3, #255	; 0xff
30007ef4:	d007      	beq.n	30007f06 <ChipInfo_MemorySize+0x2a>
30007ef6:	4298      	cmp	r0, r3
30007ef8:	d1f8      	bne.n	30007eec <ChipInfo_MemorySize+0x10>
30007efa:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007efe:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007f02:	7908      	ldrb	r0, [r1, #4]
30007f04:	bd10      	pop	{r4, pc}
30007f06:	f7ff ff55 	bl	30007db4 <ChipInfo_Invalid>
30007f0a:	bf00      	nop
30007f0c:	3000d05c 	.word	0x3000d05c
30007f10:	30007de1 	.word	0x30007de1

30007f14 <ChipInfo_MemoryVendor>:
30007f14:	b510      	push	{r4, lr}
30007f16:	4c0f      	ldr	r4, [pc, #60]	; (30007f54 <ChipInfo_MemoryVendor+0x40>)
30007f18:	47a0      	blx	r4
30007f1a:	2803      	cmp	r0, #3
30007f1c:	d016      	beq.n	30007f4c <ChipInfo_MemoryVendor+0x38>
30007f1e:	47a0      	blx	r4
30007f20:	2802      	cmp	r0, #2
30007f22:	d012      	beq.n	30007f4a <ChipInfo_MemoryVendor+0x36>
30007f24:	4c0c      	ldr	r4, [pc, #48]	; (30007f58 <ChipInfo_MemoryVendor+0x44>)
30007f26:	4b0d      	ldr	r3, [pc, #52]	; (30007f5c <ChipInfo_MemoryVendor+0x48>)
30007f28:	4798      	blx	r3
30007f2a:	2100      	movs	r1, #0
30007f2c:	23e0      	movs	r3, #224	; 0xe0
30007f2e:	4622      	mov	r2, r4
30007f30:	e004      	b.n	30007f3c <ChipInfo_MemoryVendor+0x28>
30007f32:	7a13      	ldrb	r3, [r2, #8]
30007f34:	3101      	adds	r1, #1
30007f36:	3206      	adds	r2, #6
30007f38:	2bff      	cmp	r3, #255	; 0xff
30007f3a:	d009      	beq.n	30007f50 <ChipInfo_MemoryVendor+0x3c>
30007f3c:	4298      	cmp	r0, r3
30007f3e:	d1f8      	bne.n	30007f32 <ChipInfo_MemoryVendor+0x1e>
30007f40:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007f44:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007f48:	7948      	ldrb	r0, [r1, #5]
30007f4a:	bd10      	pop	{r4, pc}
30007f4c:	2001      	movs	r0, #1
30007f4e:	bd10      	pop	{r4, pc}
30007f50:	f7ff ff30 	bl	30007db4 <ChipInfo_Invalid>
30007f54:	0000c0f9 	.word	0x0000c0f9
30007f58:	3000d05c 	.word	0x3000d05c
30007f5c:	30007de1 	.word	0x30007de1

30007f60 <SDM32K_Enable>:
30007f60:	4a02      	ldr	r2, [pc, #8]	; (30007f6c <SDM32K_Enable+0xc>)
30007f62:	4b03      	ldr	r3, [pc, #12]	; (30007f70 <SDM32K_Enable+0x10>)
30007f64:	6811      	ldr	r1, [r2, #0]
30007f66:	430b      	orrs	r3, r1
30007f68:	6013      	str	r3, [r2, #0]
30007f6a:	4770      	bx	lr
30007f6c:	42008e00 	.word	0x42008e00
30007f70:	c0060000 	.word	0xc0060000

30007f74 <CLK_SWITCH_XTAL>:
30007f74:	b508      	push	{r3, lr}
30007f76:	2801      	cmp	r0, #1
30007f78:	4b0a      	ldr	r3, [pc, #40]	; (30007fa4 <CLK_SWITCH_XTAL+0x30>)
30007f7a:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007f7e:	d00d      	beq.n	30007f9c <CLK_SWITCH_XTAL+0x28>
30007f80:	4b09      	ldr	r3, [pc, #36]	; (30007fa8 <CLK_SWITCH_XTAL+0x34>)
30007f82:	4313      	orrs	r3, r2
30007f84:	4907      	ldr	r1, [pc, #28]	; (30007fa4 <CLK_SWITCH_XTAL+0x30>)
30007f86:	4a09      	ldr	r2, [pc, #36]	; (30007fac <CLK_SWITCH_XTAL+0x38>)
30007f88:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007f8c:	4790      	blx	r2
30007f8e:	4a08      	ldr	r2, [pc, #32]	; (30007fb0 <CLK_SWITCH_XTAL+0x3c>)
30007f90:	4b08      	ldr	r3, [pc, #32]	; (30007fb4 <CLK_SWITCH_XTAL+0x40>)
30007f92:	fba2 2000 	umull	r2, r0, r2, r0
30007f96:	0c80      	lsrs	r0, r0, #18
30007f98:	6018      	str	r0, [r3, #0]
30007f9a:	bd08      	pop	{r3, pc}
30007f9c:	4b06      	ldr	r3, [pc, #24]	; (30007fb8 <CLK_SWITCH_XTAL+0x44>)
30007f9e:	4013      	ands	r3, r2
30007fa0:	e7f0      	b.n	30007f84 <CLK_SWITCH_XTAL+0x10>
30007fa2:	bf00      	nop
30007fa4:	42008000 	.word	0x42008000
30007fa8:	80000500 	.word	0x80000500
30007fac:	3000a0d1 	.word	0x3000a0d1
30007fb0:	431bde83 	.word	0x431bde83
30007fb4:	2001c700 	.word	0x2001c700
30007fb8:	7ffffaff 	.word	0x7ffffaff

30007fbc <DDR_PHY_ChipInfo_ddrtype>:
30007fbc:	b510      	push	{r4, lr}
30007fbe:	4c04      	ldr	r4, [pc, #16]	; (30007fd0 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007fc0:	7820      	ldrb	r0, [r4, #0]
30007fc2:	b100      	cbz	r0, 30007fc6 <DDR_PHY_ChipInfo_ddrtype+0xa>
30007fc4:	bd10      	pop	{r4, pc}
30007fc6:	4b03      	ldr	r3, [pc, #12]	; (30007fd4 <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007fc8:	4798      	blx	r3
30007fca:	7020      	strb	r0, [r4, #0]
30007fcc:	bd10      	pop	{r4, pc}
30007fce:	bf00      	nop
30007fd0:	3000e9b1 	.word	0x3000e9b1
30007fd4:	30007e91 	.word	0x30007e91

30007fd8 <DDR_PHY_StructInit>:
30007fd8:	f240 2315 	movw	r3, #533	; 0x215
30007fdc:	6583      	str	r3, [r0, #88]	; 0x58
30007fde:	4770      	bx	lr

30007fe0 <DDR_PHY_DLL_CLK_DIV>:
30007fe0:	b570      	push	{r4, r5, r6, lr}
30007fe2:	4d96      	ldr	r5, [pc, #600]	; (3000823c <DDR_PHY_DLL_CLK_DIV+0x25c>)
30007fe4:	4b96      	ldr	r3, [pc, #600]	; (30008240 <DDR_PHY_DLL_CLK_DIV+0x260>)
30007fe6:	fba5 4200 	umull	r4, r2, r5, r0
30007fea:	4996      	ldr	r1, [pc, #600]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
30007fec:	4403      	add	r3, r0
30007fee:	4c96      	ldr	r4, [pc, #600]	; (30008248 <DDR_PHY_DLL_CLK_DIV+0x268>)
30007ff0:	0952      	lsrs	r2, r2, #5
30007ff2:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30007ff4:	4895      	ldr	r0, [pc, #596]	; (3000824c <DDR_PHY_DLL_CLK_DIV+0x26c>)
30007ff6:	3a03      	subs	r2, #3
30007ff8:	4034      	ands	r4, r6
30007ffa:	eb02 0c82 	add.w	ip, r2, r2, lsl #2
30007ffe:	eba3 03cc 	sub.w	r3, r3, ip, lsl #3
30008002:	02db      	lsls	r3, r3, #11
30008004:	fba5 5303 	umull	r5, r3, r5, r3
30008008:	2501      	movs	r5, #1
3000800a:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
3000800e:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008012:	4320      	orrs	r0, r4
30008014:	0214      	lsls	r4, r2, #8
30008016:	b2d2      	uxtb	r2, r2
30008018:	6248      	str	r0, [r1, #36]	; 0x24
3000801a:	b2a4      	uxth	r4, r4
3000801c:	6a88      	ldr	r0, [r1, #40]	; 0x28
3000801e:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30008022:	4304      	orrs	r4, r0
30008024:	488a      	ldr	r0, [pc, #552]	; (30008250 <DDR_PHY_DLL_CLK_DIV+0x270>)
30008026:	628c      	str	r4, [r1, #40]	; 0x28
30008028:	6a4c      	ldr	r4, [r1, #36]	; 0x24
3000802a:	4020      	ands	r0, r4
3000802c:	4c89      	ldr	r4, [pc, #548]	; (30008254 <DDR_PHY_DLL_CLK_DIV+0x274>)
3000802e:	4303      	orrs	r3, r0
30008030:	624b      	str	r3, [r1, #36]	; 0x24
30008032:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30008034:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008038:	431a      	orrs	r2, r3
3000803a:	628a      	str	r2, [r1, #40]	; 0x28
3000803c:	678d      	str	r5, [r1, #120]	; 0x78
3000803e:	7820      	ldrb	r0, [r4, #0]
30008040:	2800      	cmp	r0, #0
30008042:	f000 8083 	beq.w	3000814c <DDR_PHY_DLL_CLK_DIV+0x16c>
30008046:	28e0      	cmp	r0, #224	; 0xe0
30008048:	d15a      	bne.n	30008100 <DDR_PHY_DLL_CLK_DIV+0x120>
3000804a:	4b7e      	ldr	r3, [pc, #504]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
3000804c:	2108      	movs	r1, #8
3000804e:	4c82      	ldr	r4, [pc, #520]	; (30008258 <DDR_PHY_DLL_CLK_DIV+0x278>)
30008050:	4a82      	ldr	r2, [pc, #520]	; (3000825c <DDR_PHY_DLL_CLK_DIV+0x27c>)
30008052:	4883      	ldr	r0, [pc, #524]	; (30008260 <DDR_PHY_DLL_CLK_DIV+0x280>)
30008054:	611c      	str	r4, [r3, #16]
30008056:	615a      	str	r2, [r3, #20]
30008058:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
3000805c:	2022      	movs	r0, #34	; 0x22
3000805e:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30008062:	f04f 3122 	mov.w	r1, #572662306	; 0x22222222
30008066:	689c      	ldr	r4, [r3, #8]
30008068:	4a7e      	ldr	r2, [pc, #504]	; (30008264 <DDR_PHY_DLL_CLK_DIV+0x284>)
3000806a:	4022      	ands	r2, r4
3000806c:	f042 0201 	orr.w	r2, r2, #1
30008070:	609a      	str	r2, [r3, #8]
30008072:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30008076:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
3000807a:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000807e:	f8c3 1080 	str.w	r1, [r3, #128]	; 0x80
30008082:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30008086:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000808a:	f022 0277 	bic.w	r2, r2, #119	; 0x77
3000808e:	4c76      	ldr	r4, [pc, #472]	; (30008268 <DDR_PHY_DLL_CLK_DIV+0x288>)
30008090:	f042 0212 	orr.w	r2, r2, #18
30008094:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008098:	f8c3 0088 	str.w	r0, [r3, #136]	; 0x88
3000809c:	f8c3 1094 	str.w	r1, [r3, #148]	; 0x94
300080a0:	7820      	ldrb	r0, [r4, #0]
300080a2:	4a68      	ldr	r2, [pc, #416]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
300080a4:	4971      	ldr	r1, [pc, #452]	; (3000826c <DDR_PHY_DLL_CLK_DIV+0x28c>)
300080a6:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300080aa:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300080ae:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300080b2:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300080b6:	009b      	lsls	r3, r3, #2
300080b8:	d519      	bpl.n	300080ee <DDR_PHY_DLL_CLK_DIV+0x10e>
300080ba:	2800      	cmp	r0, #0
300080bc:	d04e      	beq.n	3000815c <DDR_PHY_DLL_CLK_DIV+0x17c>
300080be:	2801      	cmp	r0, #1
300080c0:	4b60      	ldr	r3, [pc, #384]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
300080c2:	bf0c      	ite	eq
300080c4:	4a6a      	ldreq	r2, [pc, #424]	; (30008270 <DDR_PHY_DLL_CLK_DIV+0x290>)
300080c6:	4a6b      	ldrne	r2, [pc, #428]	; (30008274 <DDR_PHY_DLL_CLK_DIV+0x294>)
300080c8:	601a      	str	r2, [r3, #0]
300080ca:	4b5e      	ldr	r3, [pc, #376]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
300080cc:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300080d0:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
300080d4:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
300080d8:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300080dc:	699a      	ldr	r2, [r3, #24]
300080de:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
300080e2:	619a      	str	r2, [r3, #24]
300080e4:	699a      	ldr	r2, [r3, #24]
300080e6:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
300080ea:	619a      	str	r2, [r3, #24]
300080ec:	bd70      	pop	{r4, r5, r6, pc}
300080ee:	2800      	cmp	r0, #0
300080f0:	d030      	beq.n	30008154 <DDR_PHY_DLL_CLK_DIV+0x174>
300080f2:	2801      	cmp	r0, #1
300080f4:	4b53      	ldr	r3, [pc, #332]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
300080f6:	bf0c      	ite	eq
300080f8:	4a5f      	ldreq	r2, [pc, #380]	; (30008278 <DDR_PHY_DLL_CLK_DIV+0x298>)
300080fa:	4a60      	ldrne	r2, [pc, #384]	; (3000827c <DDR_PHY_DLL_CLK_DIV+0x29c>)
300080fc:	601a      	str	r2, [r3, #0]
300080fe:	e7e4      	b.n	300080ca <DDR_PHY_DLL_CLK_DIV+0xea>
30008100:	2800      	cmp	r0, #0
30008102:	d062      	beq.n	300081ca <DDR_PHY_DLL_CLK_DIV+0x1ea>
30008104:	2802      	cmp	r0, #2
30008106:	d12d      	bne.n	30008164 <DDR_PHY_DLL_CLK_DIV+0x184>
30008108:	4b4e      	ldr	r3, [pc, #312]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
3000810a:	220e      	movs	r2, #14
3000810c:	495c      	ldr	r1, [pc, #368]	; (30008280 <DDR_PHY_DLL_CLK_DIV+0x2a0>)
3000810e:	485d      	ldr	r0, [pc, #372]	; (30008284 <DDR_PHY_DLL_CLK_DIV+0x2a4>)
30008110:	4c5d      	ldr	r4, [pc, #372]	; (30008288 <DDR_PHY_DLL_CLK_DIV+0x2a8>)
30008112:	611c      	str	r4, [r3, #16]
30008114:	6159      	str	r1, [r3, #20]
30008116:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
3000811a:	4952      	ldr	r1, [pc, #328]	; (30008264 <DDR_PHY_DLL_CLK_DIV+0x284>)
3000811c:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008120:	f240 32c1 	movw	r2, #961	; 0x3c1
30008124:	6898      	ldr	r0, [r3, #8]
30008126:	4001      	ands	r1, r0
30008128:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
3000812c:	2022      	movs	r0, #34	; 0x22
3000812e:	430a      	orrs	r2, r1
30008130:	609a      	str	r2, [r3, #8]
30008132:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
30008136:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
3000813a:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
3000813e:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30008142:	f8c3 4080 	str.w	r4, [r3, #128]	; 0x80
30008146:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000814a:	e79e      	b.n	3000808a <DDR_PHY_DLL_CLK_DIV+0xaa>
3000814c:	4b4f      	ldr	r3, [pc, #316]	; (3000828c <DDR_PHY_DLL_CLK_DIV+0x2ac>)
3000814e:	4798      	blx	r3
30008150:	7020      	strb	r0, [r4, #0]
30008152:	e778      	b.n	30008046 <DDR_PHY_DLL_CLK_DIV+0x66>
30008154:	4b4e      	ldr	r3, [pc, #312]	; (30008290 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
30008156:	4798      	blx	r3
30008158:	7020      	strb	r0, [r4, #0]
3000815a:	e7ca      	b.n	300080f2 <DDR_PHY_DLL_CLK_DIV+0x112>
3000815c:	4b4c      	ldr	r3, [pc, #304]	; (30008290 <DDR_PHY_DLL_CLK_DIV+0x2b0>)
3000815e:	4798      	blx	r3
30008160:	7020      	strb	r0, [r4, #0]
30008162:	e7ac      	b.n	300080be <DDR_PHY_DLL_CLK_DIV+0xde>
30008164:	2805      	cmp	r0, #5
30008166:	d03b      	beq.n	300081e0 <DDR_PHY_DLL_CLK_DIV+0x200>
30008168:	2803      	cmp	r0, #3
3000816a:	f040 80e6 	bne.w	3000833a <DDR_PHY_DLL_CLK_DIV+0x35a>
3000816e:	4a35      	ldr	r2, [pc, #212]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
30008170:	230e      	movs	r3, #14
30008172:	4c48      	ldr	r4, [pc, #288]	; (30008294 <DDR_PHY_DLL_CLK_DIV+0x2b4>)
30008174:	4948      	ldr	r1, [pc, #288]	; (30008298 <DDR_PHY_DLL_CLK_DIV+0x2b8>)
30008176:	4849      	ldr	r0, [pc, #292]	; (3000829c <DDR_PHY_DLL_CLK_DIV+0x2bc>)
30008178:	6114      	str	r4, [r2, #16]
3000817a:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
3000817e:	6151      	str	r1, [r2, #20]
30008180:	f8c2 0098 	str.w	r0, [r2, #152]	; 0x98
30008184:	4937      	ldr	r1, [pc, #220]	; (30008264 <DDR_PHY_DLL_CLK_DIV+0x284>)
30008186:	f8c2 30a0 	str.w	r3, [r2, #160]	; 0xa0
3000818a:	f240 33c1 	movw	r3, #961	; 0x3c1
3000818e:	6890      	ldr	r0, [r2, #8]
30008190:	4001      	ands	r1, r0
30008192:	2033      	movs	r0, #51	; 0x33
30008194:	430b      	orrs	r3, r1
30008196:	f04f 5150 	mov.w	r1, #872415232	; 0x34000000
3000819a:	6093      	str	r3, [r2, #8]
3000819c:	f8d2 309c 	ldr.w	r3, [r2, #156]	; 0x9c
300081a0:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
300081a4:	f8c2 309c 	str.w	r3, [r2, #156]	; 0x9c
300081a8:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
300081ac:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300081b0:	4c2d      	ldr	r4, [pc, #180]	; (30008268 <DDR_PHY_DLL_CLK_DIV+0x288>)
300081b2:	f023 0377 	bic.w	r3, r3, #119	; 0x77
300081b6:	f043 0312 	orr.w	r3, r3, #18
300081ba:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300081be:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
300081c2:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
300081c6:	7820      	ldrb	r0, [r4, #0]
300081c8:	e76b      	b.n	300080a2 <DDR_PHY_DLL_CLK_DIV+0xc2>
300081ca:	4d30      	ldr	r5, [pc, #192]	; (3000828c <DDR_PHY_DLL_CLK_DIV+0x2ac>)
300081cc:	47a8      	blx	r5
300081ce:	2802      	cmp	r0, #2
300081d0:	7020      	strb	r0, [r4, #0]
300081d2:	d099      	beq.n	30008108 <DDR_PHY_DLL_CLK_DIV+0x128>
300081d4:	2800      	cmp	r0, #0
300081d6:	d1c5      	bne.n	30008164 <DDR_PHY_DLL_CLK_DIV+0x184>
300081d8:	47a8      	blx	r5
300081da:	2805      	cmp	r0, #5
300081dc:	7020      	strb	r0, [r4, #0]
300081de:	d163      	bne.n	300082a8 <DDR_PHY_DLL_CLK_DIV+0x2c8>
300081e0:	4918      	ldr	r1, [pc, #96]	; (30008244 <DDR_PHY_DLL_CLK_DIV+0x264>)
300081e2:	230e      	movs	r3, #14
300081e4:	4c2e      	ldr	r4, [pc, #184]	; (300082a0 <DDR_PHY_DLL_CLK_DIV+0x2c0>)
300081e6:	4a2f      	ldr	r2, [pc, #188]	; (300082a4 <DDR_PHY_DLL_CLK_DIV+0x2c4>)
300081e8:	4826      	ldr	r0, [pc, #152]	; (30008284 <DDR_PHY_DLL_CLK_DIV+0x2a4>)
300081ea:	610c      	str	r4, [r1, #16]
300081ec:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
300081f0:	614a      	str	r2, [r1, #20]
300081f2:	f8c1 0098 	str.w	r0, [r1, #152]	; 0x98
300081f6:	4a1b      	ldr	r2, [pc, #108]	; (30008264 <DDR_PHY_DLL_CLK_DIV+0x284>)
300081f8:	f8c1 30a0 	str.w	r3, [r1, #160]	; 0xa0
300081fc:	f240 33c1 	movw	r3, #961	; 0x3c1
30008200:	6888      	ldr	r0, [r1, #8]
30008202:	4002      	ands	r2, r0
30008204:	2033      	movs	r0, #51	; 0x33
30008206:	4313      	orrs	r3, r2
30008208:	f04f 5250 	mov.w	r2, #872415232	; 0x34000000
3000820c:	608b      	str	r3, [r1, #8]
3000820e:	f8d1 309c 	ldr.w	r3, [r1, #156]	; 0x9c
30008212:	f023 53f8 	bic.w	r3, r3, #520093696	; 0x1f000000
30008216:	f8c1 309c 	str.w	r3, [r1, #156]	; 0x9c
3000821a:	f8c1 4080 	str.w	r4, [r1, #128]	; 0x80
3000821e:	f8d1 3084 	ldr.w	r3, [r1, #132]	; 0x84
30008222:	4c11      	ldr	r4, [pc, #68]	; (30008268 <DDR_PHY_DLL_CLK_DIV+0x288>)
30008224:	f023 0377 	bic.w	r3, r3, #119	; 0x77
30008228:	f043 0312 	orr.w	r3, r3, #18
3000822c:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30008230:	f8c1 0088 	str.w	r0, [r1, #136]	; 0x88
30008234:	f8c1 2094 	str.w	r2, [r1, #148]	; 0x94
30008238:	7820      	ldrb	r0, [r4, #0]
3000823a:	e732      	b.n	300080a2 <DDR_PHY_DLL_CLK_DIV+0xc2>
3000823c:	cccccccd 	.word	0xcccccccd
30008240:	001fff88 	.word	0x001fff88
30008244:	41011000 	.word	0x41011000
30008248:	f800ffff 	.word	0xf800ffff
3000824c:	07ff0000 	.word	0x07ff0000
30008250:	fffff800 	.word	0xfffff800
30008254:	3000e9b0 	.word	0x3000e9b0
30008258:	07060012 	.word	0x07060012
3000825c:	000f0707 	.word	0x000f0707
30008260:	080f0f0f 	.word	0x080f0f0f
30008264:	ffffe000 	.word	0xffffe000
30008268:	3000e9b1 	.word	0x3000e9b1
3000826c:	42008000 	.word	0x42008000
30008270:	4c000903 	.word	0x4c000903
30008274:	4c000821 	.word	0x4c000821
30008278:	40000103 	.word	0x40000103
3000827c:	40000021 	.word	0x40000021
30008280:	00130b0b 	.word	0x00130b0b
30008284:	0e131313 	.word	0x0e131313
30008288:	0b0b0016 	.word	0x0b0b0016
3000828c:	30007de1 	.word	0x30007de1
30008290:	30007e91 	.word	0x30007e91
30008294:	0e0e0018 	.word	0x0e0e0018
30008298:	00170e0e 	.word	0x00170e0e
3000829c:	0e171717 	.word	0x0e171717
300082a0:	0a0a0018 	.word	0x0a0a0018
300082a4:	00130a0a 	.word	0x00130a0a
300082a8:	2800      	cmp	r0, #0
300082aa:	f47f af5d 	bne.w	30008168 <DDR_PHY_DLL_CLK_DIV+0x188>
300082ae:	47a8      	blx	r5
300082b0:	2803      	cmp	r0, #3
300082b2:	7020      	strb	r0, [r4, #0]
300082b4:	f43f af5b 	beq.w	3000816e <DDR_PHY_DLL_CLK_DIV+0x18e>
300082b8:	2800      	cmp	r0, #0
300082ba:	d13e      	bne.n	3000833a <DDR_PHY_DLL_CLK_DIV+0x35a>
300082bc:	47a8      	blx	r5
300082be:	280a      	cmp	r0, #10
300082c0:	7020      	strb	r0, [r4, #0]
300082c2:	d03c      	beq.n	3000833e <DDR_PHY_DLL_CLK_DIV+0x35e>
300082c4:	b908      	cbnz	r0, 300082ca <DDR_PHY_DLL_CLK_DIV+0x2ea>
300082c6:	47a8      	blx	r5
300082c8:	7020      	strb	r0, [r4, #0]
300082ca:	2809      	cmp	r0, #9
300082cc:	d04c      	beq.n	30008368 <DDR_PHY_DLL_CLK_DIV+0x388>
300082ce:	4b32      	ldr	r3, [pc, #200]	; (30008398 <DDR_PHY_DLL_CLK_DIV+0x3b8>)
300082d0:	681b      	ldr	r3, [r3, #0]
300082d2:	079a      	lsls	r2, r3, #30
300082d4:	d45c      	bmi.n	30008390 <DDR_PHY_DLL_CLK_DIV+0x3b0>
300082d6:	4b31      	ldr	r3, [pc, #196]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x3bc>)
300082d8:	2100      	movs	r1, #0
300082da:	f44f 2400 	mov.w	r4, #524288	; 0x80000
300082de:	4830      	ldr	r0, [pc, #192]	; (300083a0 <DDR_PHY_DLL_CLK_DIV+0x3c0>)
300082e0:	6119      	str	r1, [r3, #16]
300082e2:	4a30      	ldr	r2, [pc, #192]	; (300083a4 <DDR_PHY_DLL_CLK_DIV+0x3c4>)
300082e4:	615c      	str	r4, [r3, #20]
300082e6:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
300082ea:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300082ee:	6899      	ldr	r1, [r3, #8]
300082f0:	4c2d      	ldr	r4, [pc, #180]	; (300083a8 <DDR_PHY_DLL_CLK_DIV+0x3c8>)
300082f2:	400a      	ands	r2, r1
300082f4:	609a      	str	r2, [r3, #8]
300082f6:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300082fa:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300082fe:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30008302:	7820      	ldrb	r0, [r4, #0]
30008304:	2800      	cmp	r0, #0
30008306:	d03f      	beq.n	30008388 <DDR_PHY_DLL_CLK_DIV+0x3a8>
30008308:	2801      	cmp	r0, #1
3000830a:	4b24      	ldr	r3, [pc, #144]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x3bc>)
3000830c:	f04f 0100 	mov.w	r1, #0
30008310:	bf0c      	ite	eq
30008312:	2233      	moveq	r2, #51	; 0x33
30008314:	2222      	movne	r2, #34	; 0x22
30008316:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
3000831a:	f04f 3333 	mov.w	r3, #858993459	; 0x33333333
3000831e:	4a1f      	ldr	r2, [pc, #124]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008320:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
30008324:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008328:	f023 0377 	bic.w	r3, r3, #119	; 0x77
3000832c:	f043 0322 	orr.w	r3, r3, #34	; 0x22
30008330:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008334:	f8c2 1094 	str.w	r1, [r2, #148]	; 0x94
30008338:	e6b3      	b.n	300080a2 <DDR_PHY_DLL_CLK_DIV+0xc2>
3000833a:	280a      	cmp	r0, #10
3000833c:	d1c5      	bne.n	300082ca <DDR_PHY_DLL_CLK_DIV+0x2ea>
3000833e:	4b17      	ldr	r3, [pc, #92]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x3bc>)
30008340:	220e      	movs	r2, #14
30008342:	4c1a      	ldr	r4, [pc, #104]	; (300083ac <DDR_PHY_DLL_CLK_DIV+0x3cc>)
30008344:	491a      	ldr	r1, [pc, #104]	; (300083b0 <DDR_PHY_DLL_CLK_DIV+0x3d0>)
30008346:	481b      	ldr	r0, [pc, #108]	; (300083b4 <DDR_PHY_DLL_CLK_DIV+0x3d4>)
30008348:	611c      	str	r4, [r3, #16]
3000834a:	f04f 3422 	mov.w	r4, #572662306	; 0x22222222
3000834e:	6159      	str	r1, [r3, #20]
30008350:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
30008354:	4913      	ldr	r1, [pc, #76]	; (300083a4 <DDR_PHY_DLL_CLK_DIV+0x3c4>)
30008356:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000835a:	f240 32c1 	movw	r2, #961	; 0x3c1
3000835e:	6898      	ldr	r0, [r3, #8]
30008360:	4001      	ands	r1, r0
30008362:	2033      	movs	r0, #51	; 0x33
30008364:	430a      	orrs	r2, r1
30008366:	e6e3      	b.n	30008130 <DDR_PHY_DLL_CLK_DIV+0x150>
30008368:	4b0c      	ldr	r3, [pc, #48]	; (3000839c <DDR_PHY_DLL_CLK_DIV+0x3bc>)
3000836a:	2212      	movs	r2, #18
3000836c:	4912      	ldr	r1, [pc, #72]	; (300083b8 <DDR_PHY_DLL_CLK_DIV+0x3d8>)
3000836e:	4813      	ldr	r0, [pc, #76]	; (300083bc <DDR_PHY_DLL_CLK_DIV+0x3dc>)
30008370:	4c13      	ldr	r4, [pc, #76]	; (300083c0 <DDR_PHY_DLL_CLK_DIV+0x3e0>)
30008372:	611c      	str	r4, [r3, #16]
30008374:	6159      	str	r1, [r3, #20]
30008376:	f8c3 0098 	str.w	r0, [r3, #152]	; 0x98
3000837a:	490a      	ldr	r1, [pc, #40]	; (300083a4 <DDR_PHY_DLL_CLK_DIV+0x3c4>)
3000837c:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
30008380:	f640 72c1 	movw	r2, #4033	; 0xfc1
30008384:	6898      	ldr	r0, [r3, #8]
30008386:	e6ce      	b.n	30008126 <DDR_PHY_DLL_CLK_DIV+0x146>
30008388:	4b0e      	ldr	r3, [pc, #56]	; (300083c4 <DDR_PHY_DLL_CLK_DIV+0x3e4>)
3000838a:	4798      	blx	r3
3000838c:	7020      	strb	r0, [r4, #0]
3000838e:	e7bb      	b.n	30008308 <DDR_PHY_DLL_CLK_DIV+0x328>
30008390:	480d      	ldr	r0, [pc, #52]	; (300083c8 <DDR_PHY_DLL_CLK_DIV+0x3e8>)
30008392:	f003 fecd 	bl	3000c130 <__DiagPrintf_veneer>
30008396:	e79e      	b.n	300082d6 <DDR_PHY_DLL_CLK_DIV+0x2f6>
30008398:	2001c00c 	.word	0x2001c00c
3000839c:	41011000 	.word	0x41011000
300083a0:	00080808 	.word	0x00080808
300083a4:	ffffe000 	.word	0xffffe000
300083a8:	3000e9b1 	.word	0x3000e9b1
300083ac:	0f0f0016 	.word	0x0f0f0016
300083b0:	00180f0f 	.word	0x00180f0f
300083b4:	0e181818 	.word	0x0e181818
300083b8:	00170e0e 	.word	0x00170e0e
300083bc:	12171717 	.word	0x12171717
300083c0:	0e0e001a 	.word	0x0e0e001a
300083c4:	30007e91 	.word	0x30007e91
300083c8:	3000d0dc 	.word	0x3000d0dc

300083cc <DDR_PHY_CRT_Init>:
300083cc:	492f      	ldr	r1, [pc, #188]	; (3000848c <DDR_PHY_CRT_Init+0xc0>)
300083ce:	2005      	movs	r0, #5
300083d0:	4a2f      	ldr	r2, [pc, #188]	; (30008490 <DDR_PHY_CRT_Init+0xc4>)
300083d2:	b510      	push	{r4, lr}
300083d4:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
300083d8:	4c2e      	ldr	r4, [pc, #184]	; (30008494 <DDR_PHY_CRT_Init+0xc8>)
300083da:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300083de:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
300083e2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300083e6:	f043 0301 	orr.w	r3, r3, #1
300083ea:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300083ee:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300083f2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300083f6:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300083fa:	68e3      	ldr	r3, [r4, #12]
300083fc:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008400:	60e3      	str	r3, [r4, #12]
30008402:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008406:	f043 0302 	orr.w	r3, r3, #2
3000840a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000840e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008412:	f023 0308 	bic.w	r3, r3, #8
30008416:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000841a:	4790      	blx	r2
3000841c:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008420:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008424:	491c      	ldr	r1, [pc, #112]	; (30008498 <DDR_PHY_CRT_Init+0xcc>)
30008426:	f042 0204 	orr.w	r2, r2, #4
3000842a:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000842e:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008432:	f042 0208 	orr.w	r2, r2, #8
30008436:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000843a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000843c:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
30008440:	62a2      	str	r2, [r4, #40]	; 0x28
30008442:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008444:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30008448:	62a2      	str	r2, [r4, #40]	; 0x28
3000844a:	69e2      	ldr	r2, [r4, #28]
3000844c:	f042 0208 	orr.w	r2, r2, #8
30008450:	61e2      	str	r2, [r4, #28]
30008452:	68a2      	ldr	r2, [r4, #8]
30008454:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008458:	60a2      	str	r2, [r4, #8]
3000845a:	6862      	ldr	r2, [r4, #4]
3000845c:	4313      	orrs	r3, r2
3000845e:	6063      	str	r3, [r4, #4]
30008460:	68a3      	ldr	r3, [r4, #8]
30008462:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008466:	60a3      	str	r3, [r4, #8]
30008468:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
3000846c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008470:	f023 0303 	bic.w	r3, r3, #3
30008474:	f043 0302 	orr.w	r3, r3, #2
30008478:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000847c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008480:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30008484:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008488:	bd10      	pop	{r4, pc}
3000848a:	bf00      	nop
3000848c:	42008000 	.word	0x42008000
30008490:	00009b2d 	.word	0x00009b2d
30008494:	41011000 	.word	0x41011000
30008498:	90161f00 	.word	0x90161f00

3000849c <DDR_PHY_R240_ZQ_CAL>:
3000849c:	b538      	push	{r3, r4, r5, lr}
3000849e:	4c84      	ldr	r4, [pc, #528]	; (300086b0 <DDR_PHY_R240_ZQ_CAL+0x214>)
300084a0:	7823      	ldrb	r3, [r4, #0]
300084a2:	2b00      	cmp	r3, #0
300084a4:	d05e      	beq.n	30008564 <DDR_PHY_R240_ZQ_CAL+0xc8>
300084a6:	4c83      	ldr	r4, [pc, #524]	; (300086b4 <DDR_PHY_R240_ZQ_CAL+0x218>)
300084a8:	2b03      	cmp	r3, #3
300084aa:	7820      	ldrb	r0, [r4, #0]
300084ac:	d136      	bne.n	3000851c <DDR_PHY_R240_ZQ_CAL+0x80>
300084ae:	2800      	cmp	r0, #0
300084b0:	d155      	bne.n	3000855e <DDR_PHY_R240_ZQ_CAL+0xc2>
300084b2:	4d81      	ldr	r5, [pc, #516]	; (300086b8 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300084b4:	47a8      	blx	r5
300084b6:	28e0      	cmp	r0, #224	; 0xe0
300084b8:	7020      	strb	r0, [r4, #0]
300084ba:	f040 808f 	bne.w	300085dc <DDR_PHY_R240_ZQ_CAL+0x140>
300084be:	4b7f      	ldr	r3, [pc, #508]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
300084c0:	2100      	movs	r1, #0
300084c2:	4c7f      	ldr	r4, [pc, #508]	; (300086c0 <DDR_PHY_R240_ZQ_CAL+0x224>)
300084c4:	487f      	ldr	r0, [pc, #508]	; (300086c4 <DDR_PHY_R240_ZQ_CAL+0x228>)
300084c6:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
300084ca:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
300084ce:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
300084d2:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300084d6:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
300084da:	f504 2432 	add.w	r4, r4, #729088	; 0xb2000
300084de:	4a7a      	ldr	r2, [pc, #488]	; (300086c8 <DDR_PHY_R240_ZQ_CAL+0x22c>)
300084e0:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
300084e4:	f204 241a 	addw	r4, r4, #538	; 0x21a
300084e8:	f200 201b 	addw	r0, r0, #539	; 0x21b
300084ec:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
300084f0:	f8c3 4190 	str.w	r4, [r3, #400]	; 0x190
300084f4:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300084f8:	f8c3 0198 	str.w	r0, [r3, #408]	; 0x198
300084fc:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008500:	4b6e      	ldr	r3, [pc, #440]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008502:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008506:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
3000850a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000850e:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008512:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
30008516:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000851a:	bd38      	pop	{r3, r4, r5, pc}
3000851c:	2800      	cmp	r0, #0
3000851e:	d040      	beq.n	300085a2 <DDR_PHY_R240_ZQ_CAL+0x106>
30008520:	2805      	cmp	r0, #5
30008522:	d124      	bne.n	3000856e <DDR_PHY_R240_ZQ_CAL+0xd2>
30008524:	4a65      	ldr	r2, [pc, #404]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008526:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
3000852a:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
3000852e:	f043 0306 	orr.w	r3, r3, #6
30008532:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
30008536:	4b61      	ldr	r3, [pc, #388]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008538:	2100      	movs	r1, #0
3000853a:	4c64      	ldr	r4, [pc, #400]	; (300086cc <DDR_PHY_R240_ZQ_CAL+0x230>)
3000853c:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008540:	4863      	ldr	r0, [pc, #396]	; (300086d0 <DDR_PHY_R240_ZQ_CAL+0x234>)
30008542:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008546:	f104 54e0 	add.w	r4, r4, #469762048	; 0x1c000000
3000854a:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
3000854e:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
30008552:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008556:	485f      	ldr	r0, [pc, #380]	; (300086d4 <DDR_PHY_R240_ZQ_CAL+0x238>)
30008558:	f504 6422 	add.w	r4, r4, #2592	; 0xa20
3000855c:	e7c6      	b.n	300084ec <DDR_PHY_R240_ZQ_CAL+0x50>
3000855e:	28e0      	cmp	r0, #224	; 0xe0
30008560:	d0ad      	beq.n	300084be <DDR_PHY_R240_ZQ_CAL+0x22>
30008562:	e03e      	b.n	300085e2 <DDR_PHY_R240_ZQ_CAL+0x146>
30008564:	4b5c      	ldr	r3, [pc, #368]	; (300086d8 <DDR_PHY_R240_ZQ_CAL+0x23c>)
30008566:	4798      	blx	r3
30008568:	4603      	mov	r3, r0
3000856a:	7020      	strb	r0, [r4, #0]
3000856c:	e79b      	b.n	300084a6 <DDR_PHY_R240_ZQ_CAL+0xa>
3000856e:	2803      	cmp	r0, #3
30008570:	d125      	bne.n	300085be <DDR_PHY_R240_ZQ_CAL+0x122>
30008572:	4a52      	ldr	r2, [pc, #328]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008574:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
30008578:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
3000857c:	f043 030c 	orr.w	r3, r3, #12
30008580:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
30008584:	4b4d      	ldr	r3, [pc, #308]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008586:	2100      	movs	r1, #0
30008588:	4c54      	ldr	r4, [pc, #336]	; (300086dc <DDR_PHY_R240_ZQ_CAL+0x240>)
3000858a:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
3000858e:	4854      	ldr	r0, [pc, #336]	; (300086e0 <DDR_PHY_R240_ZQ_CAL+0x244>)
30008590:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
30008594:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008598:	4c52      	ldr	r4, [pc, #328]	; (300086e4 <DDR_PHY_R240_ZQ_CAL+0x248>)
3000859a:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
3000859e:	4852      	ldr	r0, [pc, #328]	; (300086e8 <DDR_PHY_R240_ZQ_CAL+0x24c>)
300085a0:	e7a4      	b.n	300084ec <DDR_PHY_R240_ZQ_CAL+0x50>
300085a2:	4d45      	ldr	r5, [pc, #276]	; (300086b8 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300085a4:	47a8      	blx	r5
300085a6:	2805      	cmp	r0, #5
300085a8:	7020      	strb	r0, [r4, #0]
300085aa:	d0bb      	beq.n	30008524 <DDR_PHY_R240_ZQ_CAL+0x88>
300085ac:	2800      	cmp	r0, #0
300085ae:	d1de      	bne.n	3000856e <DDR_PHY_R240_ZQ_CAL+0xd2>
300085b0:	47a8      	blx	r5
300085b2:	2803      	cmp	r0, #3
300085b4:	7020      	strb	r0, [r4, #0]
300085b6:	d0dc      	beq.n	30008572 <DDR_PHY_R240_ZQ_CAL+0xd6>
300085b8:	b908      	cbnz	r0, 300085be <DDR_PHY_R240_ZQ_CAL+0x122>
300085ba:	47a8      	blx	r5
300085bc:	7020      	strb	r0, [r4, #0]
300085be:	4a3f      	ldr	r2, [pc, #252]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
300085c0:	280a      	cmp	r0, #10
300085c2:	f8d2 314c 	ldr.w	r3, [r2, #332]	; 0x14c
300085c6:	f023 033e 	bic.w	r3, r3, #62	; 0x3e
300085ca:	d056      	beq.n	3000867a <DDR_PHY_R240_ZQ_CAL+0x1de>
300085cc:	f043 031e 	orr.w	r3, r3, #30
300085d0:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
300085d4:	2800      	cmp	r0, #0
300085d6:	f43f af6c 	beq.w	300084b2 <DDR_PHY_R240_ZQ_CAL+0x16>
300085da:	e7c0      	b.n	3000855e <DDR_PHY_R240_ZQ_CAL+0xc2>
300085dc:	b908      	cbnz	r0, 300085e2 <DDR_PHY_R240_ZQ_CAL+0x146>
300085de:	47a8      	blx	r5
300085e0:	7020      	strb	r0, [r4, #0]
300085e2:	2802      	cmp	r0, #2
300085e4:	f43f af6b 	beq.w	300084be <DDR_PHY_R240_ZQ_CAL+0x22>
300085e8:	b910      	cbnz	r0, 300085f0 <DDR_PHY_R240_ZQ_CAL+0x154>
300085ea:	4b33      	ldr	r3, [pc, #204]	; (300086b8 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300085ec:	4798      	blx	r3
300085ee:	7020      	strb	r0, [r4, #0]
300085f0:	2805      	cmp	r0, #5
300085f2:	d0a0      	beq.n	30008536 <DDR_PHY_R240_ZQ_CAL+0x9a>
300085f4:	bb20      	cbnz	r0, 30008640 <DDR_PHY_R240_ZQ_CAL+0x1a4>
300085f6:	4d30      	ldr	r5, [pc, #192]	; (300086b8 <DDR_PHY_R240_ZQ_CAL+0x21c>)
300085f8:	47a8      	blx	r5
300085fa:	2803      	cmp	r0, #3
300085fc:	7020      	strb	r0, [r4, #0]
300085fe:	d0c1      	beq.n	30008584 <DDR_PHY_R240_ZQ_CAL+0xe8>
30008600:	bb00      	cbnz	r0, 30008644 <DDR_PHY_R240_ZQ_CAL+0x1a8>
30008602:	47a8      	blx	r5
30008604:	280a      	cmp	r0, #10
30008606:	7020      	strb	r0, [r4, #0]
30008608:	d01e      	beq.n	30008648 <DDR_PHY_R240_ZQ_CAL+0x1ac>
3000860a:	b908      	cbnz	r0, 30008610 <DDR_PHY_R240_ZQ_CAL+0x174>
3000860c:	47a8      	blx	r5
3000860e:	7020      	strb	r0, [r4, #0]
30008610:	2809      	cmp	r0, #9
30008612:	4b2a      	ldr	r3, [pc, #168]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
30008614:	f04f 0100 	mov.w	r1, #0
30008618:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
3000861c:	d032      	beq.n	30008684 <DDR_PHY_R240_ZQ_CAL+0x1e8>
3000861e:	f8c3 1180 	str.w	r1, [r3, #384]	; 0x180
30008622:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008626:	f8c3 1188 	str.w	r1, [r3, #392]	; 0x188
3000862a:	f8c3 118c 	str.w	r1, [r3, #396]	; 0x18c
3000862e:	f8c3 2190 	str.w	r2, [r3, #400]	; 0x190
30008632:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
30008636:	f8c3 2198 	str.w	r2, [r3, #408]	; 0x198
3000863a:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
3000863e:	e75f      	b.n	30008500 <DDR_PHY_R240_ZQ_CAL+0x64>
30008640:	2803      	cmp	r0, #3
30008642:	d09f      	beq.n	30008584 <DDR_PHY_R240_ZQ_CAL+0xe8>
30008644:	280a      	cmp	r0, #10
30008646:	d1e3      	bne.n	30008610 <DDR_PHY_R240_ZQ_CAL+0x174>
30008648:	4b1c      	ldr	r3, [pc, #112]	; (300086bc <DDR_PHY_R240_ZQ_CAL+0x220>)
3000864a:	2100      	movs	r1, #0
3000864c:	4c27      	ldr	r4, [pc, #156]	; (300086ec <DDR_PHY_R240_ZQ_CAL+0x250>)
3000864e:	f04f 3226 	mov.w	r2, #640034342	; 0x26262626
30008652:	4827      	ldr	r0, [pc, #156]	; (300086f0 <DDR_PHY_R240_ZQ_CAL+0x254>)
30008654:	f8c3 41a0 	str.w	r4, [r3, #416]	; 0x1a0
30008658:	f8c3 11a4 	str.w	r1, [r3, #420]	; 0x1a4
3000865c:	4c22      	ldr	r4, [pc, #136]	; (300086e8 <DDR_PHY_R240_ZQ_CAL+0x24c>)
3000865e:	f8c3 01a8 	str.w	r0, [r3, #424]	; 0x1a8
30008662:	4824      	ldr	r0, [pc, #144]	; (300086f4 <DDR_PHY_R240_ZQ_CAL+0x258>)
30008664:	f8c3 11ac 	str.w	r1, [r3, #428]	; 0x1ac
30008668:	f8c3 41b0 	str.w	r4, [r3, #432]	; 0x1b0
3000866c:	f8c3 21b4 	str.w	r2, [r3, #436]	; 0x1b4
30008670:	f8c3 01b8 	str.w	r0, [r3, #440]	; 0x1b8
30008674:	f8c3 21bc 	str.w	r2, [r3, #444]	; 0x1bc
30008678:	e742      	b.n	30008500 <DDR_PHY_R240_ZQ_CAL+0x64>
3000867a:	f043 0308 	orr.w	r3, r3, #8
3000867e:	f8c2 314c 	str.w	r3, [r2, #332]	; 0x14c
30008682:	e7e1      	b.n	30008648 <DDR_PHY_R240_ZQ_CAL+0x1ac>
30008684:	4c0e      	ldr	r4, [pc, #56]	; (300086c0 <DDR_PHY_R240_ZQ_CAL+0x224>)
30008686:	480f      	ldr	r0, [pc, #60]	; (300086c4 <DDR_PHY_R240_ZQ_CAL+0x228>)
30008688:	f8c3 4180 	str.w	r4, [r3, #384]	; 0x180
3000868c:	f104 54c0 	add.w	r4, r4, #402653184	; 0x18000000
30008690:	f8c3 1184 	str.w	r1, [r3, #388]	; 0x184
30008694:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008698:	f100 5090 	add.w	r0, r0, #301989888	; 0x12000000
3000869c:	f504 1481 	add.w	r4, r4, #1056768	; 0x102000
300086a0:	f500 2062 	add.w	r0, r0, #925696	; 0xe2000
300086a4:	f504 6402 	add.w	r4, r4, #2080	; 0x820
300086a8:	f200 201b 	addw	r0, r0, #539	; 0x21b
300086ac:	e71e      	b.n	300084ec <DDR_PHY_R240_ZQ_CAL+0x50>
300086ae:	bf00      	nop
300086b0:	3000e9b1 	.word	0x3000e9b1
300086b4:	3000e9b0 	.word	0x3000e9b0
300086b8:	30007de1 	.word	0x30007de1
300086bc:	41011000 	.word	0x41011000
300086c0:	0e141414 	.word	0x0e141414
300086c4:	140e0e0e 	.word	0x140e0e0e
300086c8:	00262626 	.word	0x00262626
300086cc:	0a0e0c0c 	.word	0x0a0e0c0c
300086d0:	0c0b0a0a 	.word	0x0c0b0a0a
300086d4:	26162620 	.word	0x26162620
300086d8:	30007e91 	.word	0x30007e91
300086dc:	0a0c0b0b 	.word	0x0a0c0b0b
300086e0:	0b0b0a0a 	.word	0x0b0b0a0a
300086e4:	26172721 	.word	0x26172721
300086e8:	2615231e 	.word	0x2615231e
300086ec:	090b0a0a 	.word	0x090b0a0a
300086f0:	0a0a0909 	.word	0x0a0a0909
300086f4:	2613201b 	.word	0x2613201b

300086f8 <DDR_PHY_ZQ_SET_SEL>:
300086f8:	b410      	push	{r4}
300086fa:	4b25      	ldr	r3, [pc, #148]	; (30008790 <DDR_PHY_ZQ_SET_SEL+0x98>)
300086fc:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008700:	4a24      	ldr	r2, [pc, #144]	; (30008794 <DDR_PHY_ZQ_SET_SEL+0x9c>)
30008702:	4c25      	ldr	r4, [pc, #148]	; (30008798 <DDR_PHY_ZQ_SET_SEL+0xa0>)
30008704:	4925      	ldr	r1, [pc, #148]	; (3000879c <DDR_PHY_ZQ_SET_SEL+0xa4>)
30008706:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
3000870a:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000870e:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30008712:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30008716:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
3000871a:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
3000871e:	f64f 74ff 	movw	r4, #65535	; 0xffff
30008722:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30008726:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
3000872a:	2200      	movs	r2, #0
3000872c:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008730:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30008734:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008738:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
3000873c:	f04f 31ff 	mov.w	r1, #4294967295
30008740:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30008744:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008748:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
3000874c:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008750:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30008754:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008758:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
3000875c:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008760:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30008764:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30008768:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
3000876c:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30008770:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30008774:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30008778:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
3000877c:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30008780:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
30008784:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30008788:	f85d 4b04 	ldr.w	r4, [sp], #4
3000878c:	4770      	bx	lr
3000878e:	bf00      	nop
30008790:	41011000 	.word	0x41011000
30008794:	ffff1111 	.word	0xffff1111
30008798:	ffff3333 	.word	0xffff3333
3000879c:	ffff0000 	.word	0xffff0000

300087a0 <DDR_PHY_DELAY_TAP_SET>:
300087a0:	b510      	push	{r4, lr}
300087a2:	4b15      	ldr	r3, [pc, #84]	; (300087f8 <DDR_PHY_DELAY_TAP_SET+0x58>)
300087a4:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300087a8:	4914      	ldr	r1, [pc, #80]	; (300087fc <DDR_PHY_DELAY_TAP_SET+0x5c>)
300087aa:	4c15      	ldr	r4, [pc, #84]	; (30008800 <DDR_PHY_DELAY_TAP_SET+0x60>)
300087ac:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300087b0:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
300087b4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300087b8:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300087bc:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300087c0:	7820      	ldrb	r0, [r4, #0]
300087c2:	b1a0      	cbz	r0, 300087ee <DDR_PHY_DELAY_TAP_SET+0x4e>
300087c4:	2801      	cmp	r0, #1
300087c6:	4b0c      	ldr	r3, [pc, #48]	; (300087f8 <DDR_PHY_DELAY_TAP_SET+0x58>)
300087c8:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
300087cc:	bf0c      	ite	eq
300087ce:	4a0d      	ldreq	r2, [pc, #52]	; (30008804 <DDR_PHY_DELAY_TAP_SET+0x64>)
300087d0:	4a0d      	ldrne	r2, [pc, #52]	; (30008808 <DDR_PHY_DELAY_TAP_SET+0x68>)
300087d2:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
300087d6:	f640 0288 	movw	r2, #2184	; 0x888
300087da:	4b07      	ldr	r3, [pc, #28]	; (300087f8 <DDR_PHY_DELAY_TAP_SET+0x58>)
300087dc:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
300087e0:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
300087e4:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
300087e8:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
300087ec:	bd10      	pop	{r4, pc}
300087ee:	4b07      	ldr	r3, [pc, #28]	; (3000880c <DDR_PHY_DELAY_TAP_SET+0x6c>)
300087f0:	4798      	blx	r3
300087f2:	7020      	strb	r0, [r4, #0]
300087f4:	e7e6      	b.n	300087c4 <DDR_PHY_DELAY_TAP_SET+0x24>
300087f6:	bf00      	nop
300087f8:	41011000 	.word	0x41011000
300087fc:	20888888 	.word	0x20888888
30008800:	3000e9b1 	.word	0x3000e9b1
30008804:	7c033333 	.word	0x7c033333
30008808:	7c011111 	.word	0x7c011111
3000880c:	30007e91 	.word	0x30007e91

30008810 <DDR_PHY_READ_CTRL>:
30008810:	b538      	push	{r3, r4, r5, lr}
30008812:	4c2a      	ldr	r4, [pc, #168]	; (300088bc <DDR_PHY_READ_CTRL+0xac>)
30008814:	7820      	ldrb	r0, [r4, #0]
30008816:	b330      	cbz	r0, 30008866 <DDR_PHY_READ_CTRL+0x56>
30008818:	28e0      	cmp	r0, #224	; 0xe0
3000881a:	d114      	bne.n	30008846 <DDR_PHY_READ_CTRL+0x36>
3000881c:	4b28      	ldr	r3, [pc, #160]	; (300088c0 <DDR_PHY_READ_CTRL+0xb0>)
3000881e:	2202      	movs	r2, #2
30008820:	2103      	movs	r1, #3
30008822:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
30008826:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
3000882a:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
3000882e:	4b24      	ldr	r3, [pc, #144]	; (300088c0 <DDR_PHY_READ_CTRL+0xb0>)
30008830:	2108      	movs	r1, #8
30008832:	22fe      	movs	r2, #254	; 0xfe
30008834:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008838:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
3000883c:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008840:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
30008844:	bd38      	pop	{r3, r4, r5, pc}
30008846:	2802      	cmp	r0, #2
30008848:	d0e8      	beq.n	3000881c <DDR_PHY_READ_CTRL+0xc>
3000884a:	2805      	cmp	r0, #5
3000884c:	d001      	beq.n	30008852 <DDR_PHY_READ_CTRL+0x42>
3000884e:	2803      	cmp	r0, #3
30008850:	d131      	bne.n	300088b6 <DDR_PHY_READ_CTRL+0xa6>
30008852:	4b1b      	ldr	r3, [pc, #108]	; (300088c0 <DDR_PHY_READ_CTRL+0xb0>)
30008854:	2202      	movs	r2, #2
30008856:	2104      	movs	r1, #4
30008858:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
3000885c:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
30008860:	f8c3 1170 	str.w	r1, [r3, #368]	; 0x170
30008864:	e7e3      	b.n	3000882e <DDR_PHY_READ_CTRL+0x1e>
30008866:	4d17      	ldr	r5, [pc, #92]	; (300088c4 <DDR_PHY_READ_CTRL+0xb4>)
30008868:	47a8      	blx	r5
3000886a:	28e0      	cmp	r0, #224	; 0xe0
3000886c:	7020      	strb	r0, [r4, #0]
3000886e:	d0d5      	beq.n	3000881c <DDR_PHY_READ_CTRL+0xc>
30008870:	2800      	cmp	r0, #0
30008872:	d1e8      	bne.n	30008846 <DDR_PHY_READ_CTRL+0x36>
30008874:	47a8      	blx	r5
30008876:	2802      	cmp	r0, #2
30008878:	7020      	strb	r0, [r4, #0]
3000887a:	d0cf      	beq.n	3000881c <DDR_PHY_READ_CTRL+0xc>
3000887c:	2800      	cmp	r0, #0
3000887e:	d1e4      	bne.n	3000884a <DDR_PHY_READ_CTRL+0x3a>
30008880:	47a8      	blx	r5
30008882:	2805      	cmp	r0, #5
30008884:	7020      	strb	r0, [r4, #0]
30008886:	d0e4      	beq.n	30008852 <DDR_PHY_READ_CTRL+0x42>
30008888:	2800      	cmp	r0, #0
3000888a:	d1e0      	bne.n	3000884e <DDR_PHY_READ_CTRL+0x3e>
3000888c:	47a8      	blx	r5
3000888e:	2803      	cmp	r0, #3
30008890:	7020      	strb	r0, [r4, #0]
30008892:	d0de      	beq.n	30008852 <DDR_PHY_READ_CTRL+0x42>
30008894:	b978      	cbnz	r0, 300088b6 <DDR_PHY_READ_CTRL+0xa6>
30008896:	47a8      	blx	r5
30008898:	280a      	cmp	r0, #10
3000889a:	7020      	strb	r0, [r4, #0]
3000889c:	d0d9      	beq.n	30008852 <DDR_PHY_READ_CTRL+0x42>
3000889e:	b908      	cbnz	r0, 300088a4 <DDR_PHY_READ_CTRL+0x94>
300088a0:	47a8      	blx	r5
300088a2:	7020      	strb	r0, [r4, #0]
300088a4:	4b06      	ldr	r3, [pc, #24]	; (300088c0 <DDR_PHY_READ_CTRL+0xb0>)
300088a6:	2203      	movs	r2, #3
300088a8:	f8c3 25f8 	str.w	r2, [r3, #1528]	; 0x5f8
300088ac:	f8c3 25fc 	str.w	r2, [r3, #1532]	; 0x5fc
300088b0:	f8c3 2170 	str.w	r2, [r3, #368]	; 0x170
300088b4:	e7bb      	b.n	3000882e <DDR_PHY_READ_CTRL+0x1e>
300088b6:	280a      	cmp	r0, #10
300088b8:	d0cb      	beq.n	30008852 <DDR_PHY_READ_CTRL+0x42>
300088ba:	e7f3      	b.n	300088a4 <DDR_PHY_READ_CTRL+0x94>
300088bc:	3000e9b0 	.word	0x3000e9b0
300088c0:	41011000 	.word	0x41011000
300088c4:	30007de1 	.word	0x30007de1

300088c8 <DDR_PHY_READ_LEVELING>:
300088c8:	b538      	push	{r3, r4, r5, lr}
300088ca:	4c36      	ldr	r4, [pc, #216]	; (300089a4 <DDR_PHY_READ_LEVELING+0xdc>)
300088cc:	7820      	ldrb	r0, [r4, #0]
300088ce:	b348      	cbz	r0, 30008924 <DDR_PHY_READ_LEVELING+0x5c>
300088d0:	28e0      	cmp	r0, #224	; 0xe0
300088d2:	d11e      	bne.n	30008912 <DDR_PHY_READ_LEVELING+0x4a>
300088d4:	4b34      	ldr	r3, [pc, #208]	; (300089a8 <DDR_PHY_READ_LEVELING+0xe0>)
300088d6:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
300088da:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300088de:	220f      	movs	r2, #15
300088e0:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
300088e4:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300088e8:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300088ec:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300088f0:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300088f4:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300088f8:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300088fc:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30008900:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30008904:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30008908:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
3000890c:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
30008910:	bd38      	pop	{r3, r4, r5, pc}
30008912:	2802      	cmp	r0, #2
30008914:	d0de      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
30008916:	2805      	cmp	r0, #5
30008918:	d0dc      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
3000891a:	2803      	cmp	r0, #3
3000891c:	d0da      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
3000891e:	280a      	cmp	r0, #10
30008920:	d0d8      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
30008922:	e01f      	b.n	30008964 <DDR_PHY_READ_LEVELING+0x9c>
30008924:	4d21      	ldr	r5, [pc, #132]	; (300089ac <DDR_PHY_READ_LEVELING+0xe4>)
30008926:	47a8      	blx	r5
30008928:	28e0      	cmp	r0, #224	; 0xe0
3000892a:	7020      	strb	r0, [r4, #0]
3000892c:	d0d2      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
3000892e:	2800      	cmp	r0, #0
30008930:	d1ef      	bne.n	30008912 <DDR_PHY_READ_LEVELING+0x4a>
30008932:	47a8      	blx	r5
30008934:	2802      	cmp	r0, #2
30008936:	7020      	strb	r0, [r4, #0]
30008938:	d0cc      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
3000893a:	2800      	cmp	r0, #0
3000893c:	d1eb      	bne.n	30008916 <DDR_PHY_READ_LEVELING+0x4e>
3000893e:	47a8      	blx	r5
30008940:	2805      	cmp	r0, #5
30008942:	7020      	strb	r0, [r4, #0]
30008944:	d0c6      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
30008946:	2800      	cmp	r0, #0
30008948:	d1e7      	bne.n	3000891a <DDR_PHY_READ_LEVELING+0x52>
3000894a:	47a8      	blx	r5
3000894c:	2803      	cmp	r0, #3
3000894e:	7020      	strb	r0, [r4, #0]
30008950:	d0c0      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
30008952:	2800      	cmp	r0, #0
30008954:	d1e3      	bne.n	3000891e <DDR_PHY_READ_LEVELING+0x56>
30008956:	47a8      	blx	r5
30008958:	280a      	cmp	r0, #10
3000895a:	7020      	strb	r0, [r4, #0]
3000895c:	d0ba      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
3000895e:	b908      	cbnz	r0, 30008964 <DDR_PHY_READ_LEVELING+0x9c>
30008960:	47a8      	blx	r5
30008962:	7020      	strb	r0, [r4, #0]
30008964:	2809      	cmp	r0, #9
30008966:	d0b5      	beq.n	300088d4 <DDR_PHY_READ_LEVELING+0xc>
30008968:	4b0f      	ldr	r3, [pc, #60]	; (300089a8 <DDR_PHY_READ_LEVELING+0xe0>)
3000896a:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
3000896e:	2110      	movs	r1, #16
30008970:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30008974:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30008978:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
3000897c:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30008980:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008984:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008988:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
3000898c:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008990:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008994:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008998:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
3000899c:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
300089a0:	bd38      	pop	{r3, r4, r5, pc}
300089a2:	bf00      	nop
300089a4:	3000e9b0 	.word	0x3000e9b0
300089a8:	41011000 	.word	0x41011000
300089ac:	30007de1 	.word	0x30007de1

300089b0 <DDR_PHY_Init>:
300089b0:	b510      	push	{r4, lr}
300089b2:	6d80      	ldr	r0, [r0, #88]	; 0x58
300089b4:	4c2f      	ldr	r4, [pc, #188]	; (30008a74 <DDR_PHY_Init+0xc4>)
300089b6:	f7ff fb13 	bl	30007fe0 <DDR_PHY_DLL_CLK_DIV>
300089ba:	f7ff fd07 	bl	300083cc <DDR_PHY_CRT_Init>
300089be:	f7ff fd6d 	bl	3000849c <DDR_PHY_R240_ZQ_CAL>
300089c2:	f7ff fe99 	bl	300086f8 <DDR_PHY_ZQ_SET_SEL>
300089c6:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
300089ca:	2300      	movs	r3, #0
300089cc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
300089d0:	4829      	ldr	r0, [pc, #164]	; (30008a78 <DDR_PHY_Init+0xc8>)
300089d2:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
300089d6:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
300089da:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
300089de:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
300089e2:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
300089e6:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
300089ea:	f7ff fed9 	bl	300087a0 <DDR_PHY_DELAY_TAP_SET>
300089ee:	f7ff ff0f 	bl	30008810 <DDR_PHY_READ_CTRL>
300089f2:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
300089f6:	4a21      	ldr	r2, [pc, #132]	; (30008a7c <DDR_PHY_Init+0xcc>)
300089f8:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
300089fc:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008a00:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008a04:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008a08:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008a0c:	f7ff ff5c 	bl	300088c8 <DDR_PHY_READ_LEVELING>
30008a10:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008a14:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008a18:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008a1c:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008a20:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008a24:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008a28:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30008a2c:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008a30:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30008a34:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30008a38:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30008a3c:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008a40:	f043 0303 	orr.w	r3, r3, #3
30008a44:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008a48:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008a4c:	f043 030c 	orr.w	r3, r3, #12
30008a50:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008a54:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008a58:	f023 0303 	bic.w	r3, r3, #3
30008a5c:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008a60:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008a64:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
30008a68:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30008a6c:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008a70:	bd10      	pop	{r4, pc}
30008a72:	bf00      	nop
30008a74:	41011000 	.word	0x41011000
30008a78:	00045500 	.word	0x00045500
30008a7c:	11117777 	.word	0x11117777

30008a80 <DDR_PHY_CAL>:
30008a80:	4770      	bx	lr
30008a82:	bf00      	nop

30008a84 <DDR_PHY_AutoGating>:
30008a84:	4a0a      	ldr	r2, [pc, #40]	; (30008ab0 <DDR_PHY_AutoGating+0x2c>)
30008a86:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
30008a8a:	6893      	ldr	r3, [r2, #8]
30008a8c:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008a90:	6093      	str	r3, [r2, #8]
30008a92:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008a96:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008a9a:	f043 0314 	orr.w	r3, r3, #20
30008a9e:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008aa2:	f8d1 31e0 	ldr.w	r3, [r1, #480]	; 0x1e0
30008aa6:	f043 0303 	orr.w	r3, r3, #3
30008aaa:	f8c1 31e0 	str.w	r3, [r1, #480]	; 0x1e0
30008aae:	4770      	bx	lr
30008ab0:	41011000 	.word	0x41011000

30008ab4 <flash_init_userdef>:
30008ab4:	4b14      	ldr	r3, [pc, #80]	; (30008b08 <flash_init_userdef+0x54>)
30008ab6:	2201      	movs	r2, #1
30008ab8:	2140      	movs	r1, #64	; 0x40
30008aba:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008abe:	b430      	push	{r4, r5}
30008ac0:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008ac4:	2502      	movs	r5, #2
30008ac6:	2400      	movs	r4, #0
30008ac8:	f640 0104 	movw	r1, #2052	; 0x804
30008acc:	601a      	str	r2, [r3, #0]
30008ace:	22bb      	movs	r2, #187	; 0xbb
30008ad0:	83d8      	strh	r0, [r3, #30]
30008ad2:	8419      	strh	r1, [r3, #32]
30008ad4:	203b      	movs	r0, #59	; 0x3b
30008ad6:	21eb      	movs	r1, #235	; 0xeb
30008ad8:	629a      	str	r2, [r3, #40]	; 0x28
30008ada:	4a0c      	ldr	r2, [pc, #48]	; (30008b0c <flash_init_userdef+0x58>)
30008adc:	6258      	str	r0, [r3, #36]	; 0x24
30008ade:	6319      	str	r1, [r3, #48]	; 0x30
30008ae0:	f64a 3120 	movw	r1, #43808	; 0xab20
30008ae4:	655a      	str	r2, [r3, #84]	; 0x54
30008ae6:	22b9      	movs	r2, #185	; 0xb9
30008ae8:	4809      	ldr	r0, [pc, #36]	; (30008b10 <flash_init_userdef+0x5c>)
30008aea:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008aee:	6598      	str	r0, [r3, #88]	; 0x58
30008af0:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008af4:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008af8:	2406      	movs	r4, #6
30008afa:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008afe:	246b      	movs	r4, #107	; 0x6b
30008b00:	62dc      	str	r4, [r3, #44]	; 0x2c
30008b02:	bc30      	pop	{r4, r5}
30008b04:	4770      	bx	lr
30008b06:	bf00      	nop
30008b08:	2001c01c 	.word	0x2001c01c
30008b0c:	00059f06 	.word	0x00059f06
30008b10:	d8600001 	.word	0xd8600001

30008b14 <nand_init_userdef>:
30008b14:	4b18      	ldr	r3, [pc, #96]	; (30008b78 <nand_init_userdef+0x64>)
30008b16:	2001      	movs	r0, #1
30008b18:	2200      	movs	r2, #0
30008b1a:	2102      	movs	r1, #2
30008b1c:	b410      	push	{r4}
30008b1e:	6018      	str	r0, [r3, #0]
30008b20:	f640 0408 	movw	r4, #2056	; 0x808
30008b24:	6159      	str	r1, [r3, #20]
30008b26:	83dc      	strh	r4, [r3, #30]
30008b28:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008b2c:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008b30:	841c      	strh	r4, [r3, #32]
30008b32:	246b      	movs	r4, #107	; 0x6b
30008b34:	619a      	str	r2, [r3, #24]
30008b36:	62dc      	str	r4, [r3, #44]	; 0x2c
30008b38:	4c10      	ldr	r4, [pc, #64]	; (30008b7c <nand_init_userdef+0x68>)
30008b3a:	775a      	strb	r2, [r3, #29]
30008b3c:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008b40:	629a      	str	r2, [r3, #40]	; 0x28
30008b42:	631a      	str	r2, [r3, #48]	; 0x30
30008b44:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30008b48:	65da      	str	r2, [r3, #92]	; 0x5c
30008b4a:	e9c3 0003 	strd	r0, r0, [r3, #12]
30008b4e:	203b      	movs	r0, #59	; 0x3b
30008b50:	6258      	str	r0, [r3, #36]	; 0x24
30008b52:	200b      	movs	r0, #11
30008b54:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30008b58:	6698      	str	r0, [r3, #104]	; 0x68
30008b5a:	2008      	movs	r0, #8
30008b5c:	4908      	ldr	r1, [pc, #32]	; (30008b80 <nand_init_userdef+0x6c>)
30008b5e:	64d8      	str	r0, [r3, #76]	; 0x4c
30008b60:	f241 0013 	movw	r0, #4115	; 0x1013
30008b64:	6599      	str	r1, [r3, #88]	; 0x58
30008b66:	4907      	ldr	r1, [pc, #28]	; (30008b84 <nand_init_userdef+0x70>)
30008b68:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30008b6c:	6719      	str	r1, [r3, #112]	; 0x70
30008b6e:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30008b72:	f85d 4b04 	ldr.w	r4, [sp], #4
30008b76:	4770      	bx	lr
30008b78:	2001c01c 	.word	0x2001c01c
30008b7c:	000f9f06 	.word	0x000f9f06
30008b80:	d800001f 	.word	0xd800001f
30008b84:	10300804 	.word	0x10300804

30008b88 <flash_layout_init>:
30008b88:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008b8c:	4b65      	ldr	r3, [pc, #404]	; (30008d24 <flash_layout_init+0x19c>)
30008b8e:	b08f      	sub	sp, #60	; 0x3c
30008b90:	4798      	blx	r3
30008b92:	4b65      	ldr	r3, [pc, #404]	; (30008d28 <flash_layout_init+0x1a0>)
30008b94:	4a65      	ldr	r2, [pc, #404]	; (30008d2c <flash_layout_init+0x1a4>)
30008b96:	2800      	cmp	r0, #0
30008b98:	bf08      	it	eq
30008b9a:	4613      	moveq	r3, r2
30008b9c:	681f      	ldr	r7, [r3, #0]
30008b9e:	2fff      	cmp	r7, #255	; 0xff
30008ba0:	d05a      	beq.n	30008c58 <flash_layout_init+0xd0>
30008ba2:	f8df b19c 	ldr.w	fp, [pc, #412]	; 30008d40 <flash_layout_init+0x1b8>
30008ba6:	f04f 0900 	mov.w	r9, #0
30008baa:	4a61      	ldr	r2, [pc, #388]	; (30008d30 <flash_layout_init+0x1a8>)
30008bac:	f8db 1000 	ldr.w	r1, [fp]
30008bb0:	46c8      	mov	r8, r9
30008bb2:	f8df e190 	ldr.w	lr, [pc, #400]	; 30008d44 <flash_layout_init+0x1bc>
30008bb6:	464e      	mov	r6, r9
30008bb8:	910d      	str	r1, [sp, #52]	; 0x34
30008bba:	464c      	mov	r4, r9
30008bbc:	495d      	ldr	r1, [pc, #372]	; (30008d34 <flash_layout_init+0x1ac>)
30008bbe:	6812      	ldr	r2, [r2, #0]
30008bc0:	6809      	ldr	r1, [r1, #0]
30008bc2:	f8df c184 	ldr.w	ip, [pc, #388]	; 30008d48 <flash_layout_init+0x1c0>
30008bc6:	9109      	str	r1, [sp, #36]	; 0x24
30008bc8:	495b      	ldr	r1, [pc, #364]	; (30008d38 <flash_layout_init+0x1b0>)
30008bca:	920c      	str	r2, [sp, #48]	; 0x30
30008bcc:	6809      	ldr	r1, [r1, #0]
30008bce:	4a5b      	ldr	r2, [pc, #364]	; (30008d3c <flash_layout_init+0x1b4>)
30008bd0:	910b      	str	r1, [sp, #44]	; 0x2c
30008bd2:	f8de 1000 	ldr.w	r1, [lr]
30008bd6:	f8cd 9028 	str.w	r9, [sp, #40]	; 0x28
30008bda:	9107      	str	r1, [sp, #28]
30008bdc:	f89c 1000 	ldrb.w	r1, [ip]
30008be0:	f8cd 9018 	str.w	r9, [sp, #24]
30008be4:	9108      	str	r1, [sp, #32]
30008be6:	6951      	ldr	r1, [r2, #20]
30008be8:	f8cd 9010 	str.w	r9, [sp, #16]
30008bec:	9105      	str	r1, [sp, #20]
30008bee:	6911      	ldr	r1, [r2, #16]
30008bf0:	f8cd 9008 	str.w	r9, [sp, #8]
30008bf4:	9103      	str	r1, [sp, #12]
30008bf6:	68d1      	ldr	r1, [r2, #12]
30008bf8:	9101      	str	r1, [sp, #4]
30008bfa:	6891      	ldr	r1, [r2, #8]
30008bfc:	e9d2 0500 	ldrd	r0, r5, [r2]
30008c00:	9100      	str	r1, [sp, #0]
30008c02:	4649      	mov	r1, r9
30008c04:	2f00      	cmp	r7, #0
30008c06:	d12a      	bne.n	30008c5e <flash_layout_init+0xd6>
30008c08:	6858      	ldr	r0, [r3, #4]
30008c0a:	2101      	movs	r1, #1
30008c0c:	f853 7f0c 	ldr.w	r7, [r3, #12]!
30008c10:	2fff      	cmp	r7, #255	; 0xff
30008c12:	d1f7      	bne.n	30008c04 <flash_layout_init+0x7c>
30008c14:	b101      	cbz	r1, 30008c18 <flash_layout_init+0x90>
30008c16:	6010      	str	r0, [r2, #0]
30008c18:	b104      	cbz	r4, 30008c1c <flash_layout_init+0x94>
30008c1a:	6055      	str	r5, [r2, #4]
30008c1c:	b10e      	cbz	r6, 30008c22 <flash_layout_init+0x9a>
30008c1e:	9b00      	ldr	r3, [sp, #0]
30008c20:	6093      	str	r3, [r2, #8]
30008c22:	f1b8 0f00 	cmp.w	r8, #0
30008c26:	d001      	beq.n	30008c2c <flash_layout_init+0xa4>
30008c28:	9b01      	ldr	r3, [sp, #4]
30008c2a:	60d3      	str	r3, [r2, #12]
30008c2c:	9b02      	ldr	r3, [sp, #8]
30008c2e:	b10b      	cbz	r3, 30008c34 <flash_layout_init+0xac>
30008c30:	9b03      	ldr	r3, [sp, #12]
30008c32:	6113      	str	r3, [r2, #16]
30008c34:	9b04      	ldr	r3, [sp, #16]
30008c36:	2b00      	cmp	r3, #0
30008c38:	d169      	bne.n	30008d0e <flash_layout_init+0x186>
30008c3a:	9b06      	ldr	r3, [sp, #24]
30008c3c:	2b00      	cmp	r3, #0
30008c3e:	d15f      	bne.n	30008d00 <flash_layout_init+0x178>
30008c40:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30008c42:	2b00      	cmp	r3, #0
30008c44:	d166      	bne.n	30008d14 <flash_layout_init+0x18c>
30008c46:	f1b9 0f00 	cmp.w	r9, #0
30008c4a:	d005      	beq.n	30008c58 <flash_layout_init+0xd0>
30008c4c:	4b38      	ldr	r3, [pc, #224]	; (30008d30 <flash_layout_init+0x1a8>)
30008c4e:	9a0c      	ldr	r2, [sp, #48]	; 0x30
30008c50:	601a      	str	r2, [r3, #0]
30008c52:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30008c54:	f8cb 3000 	str.w	r3, [fp]
30008c58:	b00f      	add	sp, #60	; 0x3c
30008c5a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008c5e:	2f01      	cmp	r7, #1
30008c60:	d01a      	beq.n	30008c98 <flash_layout_init+0x110>
30008c62:	2f02      	cmp	r7, #2
30008c64:	d01b      	beq.n	30008c9e <flash_layout_init+0x116>
30008c66:	2f03      	cmp	r7, #3
30008c68:	d01d      	beq.n	30008ca6 <flash_layout_init+0x11e>
30008c6a:	2f04      	cmp	r7, #4
30008c6c:	d020      	beq.n	30008cb0 <flash_layout_init+0x128>
30008c6e:	2f05      	cmp	r7, #5
30008c70:	d023      	beq.n	30008cba <flash_layout_init+0x132>
30008c72:	2f06      	cmp	r7, #6
30008c74:	d026      	beq.n	30008cc4 <flash_layout_init+0x13c>
30008c76:	2f07      	cmp	r7, #7
30008c78:	d035      	beq.n	30008ce6 <flash_layout_init+0x15e>
30008c7a:	2f08      	cmp	r7, #8
30008c7c:	d1c6      	bne.n	30008c0c <flash_layout_init+0x84>
30008c7e:	689f      	ldr	r7, [r3, #8]
30008c80:	f8d3 9004 	ldr.w	r9, [r3, #4]
30008c84:	3701      	adds	r7, #1
30008c86:	eba7 0709 	sub.w	r7, r7, r9
30008c8a:	970d      	str	r7, [sp, #52]	; 0x34
30008c8c:	f109 4778 	add.w	r7, r9, #4160749568	; 0xf8000000
30008c90:	f04f 0901 	mov.w	r9, #1
30008c94:	970c      	str	r7, [sp, #48]	; 0x30
30008c96:	e7b9      	b.n	30008c0c <flash_layout_init+0x84>
30008c98:	463c      	mov	r4, r7
30008c9a:	685d      	ldr	r5, [r3, #4]
30008c9c:	e7b6      	b.n	30008c0c <flash_layout_init+0x84>
30008c9e:	685e      	ldr	r6, [r3, #4]
30008ca0:	9600      	str	r6, [sp, #0]
30008ca2:	2601      	movs	r6, #1
30008ca4:	e7b2      	b.n	30008c0c <flash_layout_init+0x84>
30008ca6:	685f      	ldr	r7, [r3, #4]
30008ca8:	f04f 0801 	mov.w	r8, #1
30008cac:	9701      	str	r7, [sp, #4]
30008cae:	e7ad      	b.n	30008c0c <flash_layout_init+0x84>
30008cb0:	685f      	ldr	r7, [r3, #4]
30008cb2:	9703      	str	r7, [sp, #12]
30008cb4:	2701      	movs	r7, #1
30008cb6:	9702      	str	r7, [sp, #8]
30008cb8:	e7a8      	b.n	30008c0c <flash_layout_init+0x84>
30008cba:	685f      	ldr	r7, [r3, #4]
30008cbc:	9705      	str	r7, [sp, #20]
30008cbe:	2701      	movs	r7, #1
30008cc0:	9704      	str	r7, [sp, #16]
30008cc2:	e7a3      	b.n	30008c0c <flash_layout_init+0x84>
30008cc4:	689f      	ldr	r7, [r3, #8]
30008cc6:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008cca:	3701      	adds	r7, #1
30008ccc:	eba7 070a 	sub.w	r7, r7, sl
30008cd0:	9706      	str	r7, [sp, #24]
30008cd2:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008cd6:	9707      	str	r7, [sp, #28]
30008cd8:	9f06      	ldr	r7, [sp, #24]
30008cda:	f3c7 3707 	ubfx	r7, r7, #12, #8
30008cde:	9708      	str	r7, [sp, #32]
30008ce0:	2701      	movs	r7, #1
30008ce2:	9706      	str	r7, [sp, #24]
30008ce4:	e792      	b.n	30008c0c <flash_layout_init+0x84>
30008ce6:	689f      	ldr	r7, [r3, #8]
30008ce8:	f8d3 a004 	ldr.w	sl, [r3, #4]
30008cec:	3701      	adds	r7, #1
30008cee:	eba7 070a 	sub.w	r7, r7, sl
30008cf2:	970b      	str	r7, [sp, #44]	; 0x2c
30008cf4:	f10a 4778 	add.w	r7, sl, #4160749568	; 0xf8000000
30008cf8:	9709      	str	r7, [sp, #36]	; 0x24
30008cfa:	2701      	movs	r7, #1
30008cfc:	970a      	str	r7, [sp, #40]	; 0x28
30008cfe:	e785      	b.n	30008c0c <flash_layout_init+0x84>
30008d00:	9b07      	ldr	r3, [sp, #28]
30008d02:	f8ce 3000 	str.w	r3, [lr]
30008d06:	9b08      	ldr	r3, [sp, #32]
30008d08:	f88c 3000 	strb.w	r3, [ip]
30008d0c:	e798      	b.n	30008c40 <flash_layout_init+0xb8>
30008d0e:	9b05      	ldr	r3, [sp, #20]
30008d10:	6153      	str	r3, [r2, #20]
30008d12:	e792      	b.n	30008c3a <flash_layout_init+0xb2>
30008d14:	4b07      	ldr	r3, [pc, #28]	; (30008d34 <flash_layout_init+0x1ac>)
30008d16:	9a09      	ldr	r2, [sp, #36]	; 0x24
30008d18:	601a      	str	r2, [r3, #0]
30008d1a:	4b07      	ldr	r3, [pc, #28]	; (30008d38 <flash_layout_init+0x1b0>)
30008d1c:	9a0b      	ldr	r2, [sp, #44]	; 0x2c
30008d1e:	601a      	str	r2, [r3, #0]
30008d20:	e791      	b.n	30008c46 <flash_layout_init+0xbe>
30008d22:	bf00      	nop
30008d24:	3000a479 	.word	0x3000a479
30008d28:	3000e380 	.word	0x3000e380
30008d2c:	3000e308 	.word	0x3000e308
30008d30:	3000e9d4 	.word	0x3000e9d4
30008d34:	3000e9cc 	.word	0x3000e9cc
30008d38:	3000e9d0 	.word	0x3000e9d0
30008d3c:	3000e9b4 	.word	0x3000e9b4
30008d40:	3000e9d8 	.word	0x3000e9d8
30008d44:	3000e9dc 	.word	0x3000e9dc
30008d48:	3000e3e0 	.word	0x3000e3e0

30008d4c <NAND_CHECK_IS_BAD_BLOCK>:
30008d4c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008d50:	4e25      	ldr	r6, [pc, #148]	; (30008de8 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008d52:	0907      	lsrs	r7, r0, #4
30008d54:	0045      	lsls	r5, r0, #1
30008d56:	b083      	sub	sp, #12
30008d58:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008d5c:	f005 051e 	and.w	r5, r5, #30
30008d60:	40eb      	lsrs	r3, r5
30008d62:	f003 0303 	and.w	r3, r3, #3
30008d66:	2b02      	cmp	r3, #2
30008d68:	d022      	beq.n	30008db0 <NAND_CHECK_IS_BAD_BLOCK+0x64>
30008d6a:	2b01      	cmp	r3, #1
30008d6c:	d024      	beq.n	30008db8 <NAND_CHECK_IS_BAD_BLOCK+0x6c>
30008d6e:	4b1f      	ldr	r3, [pc, #124]	; (30008dec <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008d70:	f04f 0801 	mov.w	r8, #1
30008d74:	2204      	movs	r2, #4
30008d76:	4604      	mov	r4, r0
30008d78:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008d7c:	0180      	lsls	r0, r0, #6
30008d7e:	ab01      	add	r3, sp, #4
30008d80:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30008df8 <NAND_CHECK_IS_BAD_BLOCK+0xac>
30008d84:	fa08 f101 	lsl.w	r1, r8, r1
30008d88:	47c8      	blx	r9
30008d8a:	f89d 3004 	ldrb.w	r3, [sp, #4]
30008d8e:	f89d 2005 	ldrb.w	r2, [sp, #5]
30008d92:	4013      	ands	r3, r2
30008d94:	2bff      	cmp	r3, #255	; 0xff
30008d96:	d013      	beq.n	30008dc0 <NAND_CHECK_IS_BAD_BLOCK+0x74>
30008d98:	2302      	movs	r3, #2
30008d9a:	fa03 f505 	lsl.w	r5, r3, r5
30008d9e:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008da2:	432b      	orrs	r3, r5
30008da4:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008da8:	4b11      	ldr	r3, [pc, #68]	; (30008df0 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008daa:	681b      	ldr	r3, [r3, #0]
30008dac:	079b      	lsls	r3, r3, #30
30008dae:	d412      	bmi.n	30008dd6 <NAND_CHECK_IS_BAD_BLOCK+0x8a>
30008db0:	2001      	movs	r0, #1
30008db2:	b003      	add	sp, #12
30008db4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008db8:	2000      	movs	r0, #0
30008dba:	b003      	add	sp, #12
30008dbc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008dc0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008dc4:	fa08 f005 	lsl.w	r0, r8, r5
30008dc8:	4303      	orrs	r3, r0
30008dca:	2000      	movs	r0, #0
30008dcc:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008dd0:	b003      	add	sp, #12
30008dd2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008dd6:	4621      	mov	r1, r4
30008dd8:	4806      	ldr	r0, [pc, #24]	; (30008df4 <NAND_CHECK_IS_BAD_BLOCK+0xa8>)
30008dda:	f003 f9a9 	bl	3000c130 <__DiagPrintf_veneer>
30008dde:	4640      	mov	r0, r8
30008de0:	b003      	add	sp, #12
30008de2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008de6:	bf00      	nop
30008de8:	3000e9e8 	.word	0x3000e9e8
30008dec:	2001c01c 	.word	0x2001c01c
30008df0:	2001c00c 	.word	0x2001c00c
30008df4:	3000d2dc 	.word	0x3000d2dc
30008df8:	3000a3d1 	.word	0x3000a3d1

30008dfc <Nand_Get_NandAddr>:
30008dfc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008e00:	4f10      	ldr	r7, [pc, #64]	; (30008e44 <Nand_Get_NandAddr+0x48>)
30008e02:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008e06:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008e0a:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008e0e:	40e8      	lsrs	r0, r5
30008e10:	fa26 f505 	lsr.w	r5, r6, r5
30008e14:	09ad      	lsrs	r5, r5, #6
30008e16:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008e1a:	d90f      	bls.n	30008e3c <Nand_Get_NandAddr+0x40>
30008e1c:	0984      	lsrs	r4, r0, #6
30008e1e:	f04f 0840 	mov.w	r8, #64	; 0x40
30008e22:	4620      	mov	r0, r4
30008e24:	3401      	adds	r4, #1
30008e26:	f7ff ff91 	bl	30008d4c <NAND_CHECK_IS_BAD_BLOCK>
30008e2a:	b128      	cbz	r0, 30008e38 <Nand_Get_NandAddr+0x3c>
30008e2c:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008e30:	3501      	adds	r5, #1
30008e32:	fa08 f303 	lsl.w	r3, r8, r3
30008e36:	441e      	add	r6, r3
30008e38:	42a5      	cmp	r5, r4
30008e3a:	d8f2      	bhi.n	30008e22 <Nand_Get_NandAddr+0x26>
30008e3c:	4630      	mov	r0, r6
30008e3e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008e42:	bf00      	nop
30008e44:	2001c01c 	.word	0x2001c01c

30008e48 <NandImgCopy>:
30008e48:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008e4c:	4b56      	ldr	r3, [pc, #344]	; (30008fa8 <NandImgCopy+0x160>)
30008e4e:	b085      	sub	sp, #20
30008e50:	4617      	mov	r7, r2
30008e52:	9003      	str	r0, [sp, #12]
30008e54:	6898      	ldr	r0, [r3, #8]
30008e56:	4281      	cmp	r1, r0
30008e58:	f0c0 8089 	bcc.w	30008f6e <NandImgCopy+0x126>
30008e5c:	691b      	ldr	r3, [r3, #16]
30008e5e:	4299      	cmp	r1, r3
30008e60:	d37b      	bcc.n	30008f5a <NandImgCopy+0x112>
30008e62:	f8df 9154 	ldr.w	r9, [pc, #340]	; 30008fb8 <NandImgCopy+0x170>
30008e66:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008e6a:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008e6e:	fa25 f406 	lsr.w	r4, r5, r6
30008e72:	f107 38ff 	add.w	r8, r7, #4294967295
30008e76:	f1c4 0301 	rsb	r3, r4, #1
30008e7a:	44a8      	add	r8, r5
30008e7c:	fa28 f806 	lsr.w	r8, r8, r6
30008e80:	eb18 0803 	adds.w	r8, r8, r3
30008e84:	d05e      	beq.n	30008f44 <NandImgCopy+0xfc>
30008e86:	f108 38ff 	add.w	r8, r8, #4294967295
30008e8a:	463b      	mov	r3, r7
30008e8c:	f04f 0a00 	mov.w	sl, #0
30008e90:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008e94:	4647      	mov	r7, r8
30008e96:	4698      	mov	r8, r3
30008e98:	f8cd a008 	str.w	sl, [sp, #8]
30008e9c:	e00a      	b.n	30008eb4 <NandImgCopy+0x6c>
30008e9e:	1ac9      	subs	r1, r1, r3
30008ea0:	4b42      	ldr	r3, [pc, #264]	; (30008fac <NandImgCopy+0x164>)
30008ea2:	689b      	ldr	r3, [r3, #8]
30008ea4:	4488      	add	r8, r1
30008ea6:	079a      	lsls	r2, r3, #30
30008ea8:	d44f      	bmi.n	30008f4a <NandImgCopy+0x102>
30008eaa:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008eae:	3f01      	subs	r7, #1
30008eb0:	fa25 f406 	lsr.w	r4, r5, r6
30008eb4:	9b02      	ldr	r3, [sp, #8]
30008eb6:	4632      	mov	r2, r6
30008eb8:	09a6      	lsrs	r6, r4, #6
30008eba:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008ebe:	d922      	bls.n	30008f06 <NandImgCopy+0xbe>
30008ec0:	f1c2 0320 	rsb	r3, r2, #32
30008ec4:	f04f 31ff 	mov.w	r1, #4294967295
30008ec8:	40d9      	lsrs	r1, r3
30008eca:	9b03      	ldr	r3, [sp, #12]
30008ecc:	4029      	ands	r1, r5
30008ece:	4453      	add	r3, sl
30008ed0:	b31f      	cbz	r7, 30008f1a <NandImgCopy+0xd2>
30008ed2:	2601      	movs	r6, #1
30008ed4:	4620      	mov	r0, r4
30008ed6:	9101      	str	r1, [sp, #4]
30008ed8:	fa06 f202 	lsl.w	r2, r6, r2
30008edc:	4e34      	ldr	r6, [pc, #208]	; (30008fb0 <NandImgCopy+0x168>)
30008ede:	9200      	str	r2, [sp, #0]
30008ee0:	1a52      	subs	r2, r2, r1
30008ee2:	4492      	add	sl, r2
30008ee4:	4415      	add	r5, r2
30008ee6:	47b0      	blx	r6
30008ee8:	f010 0f70 	tst.w	r0, #112	; 0x70
30008eec:	9b00      	ldr	r3, [sp, #0]
30008eee:	9901      	ldr	r1, [sp, #4]
30008ef0:	d1d5      	bne.n	30008e9e <NandImgCopy+0x56>
30008ef2:	eba8 0803 	sub.w	r8, r8, r3
30008ef6:	4488      	add	r8, r1
30008ef8:	e7d7      	b.n	30008eaa <NandImgCopy+0x62>
30008efa:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008efe:	3440      	adds	r4, #64	; 0x40
30008f00:	fa0b f303 	lsl.w	r3, fp, r3
30008f04:	441d      	add	r5, r3
30008f06:	4630      	mov	r0, r6
30008f08:	3601      	adds	r6, #1
30008f0a:	f7ff ff1f 	bl	30008d4c <NAND_CHECK_IS_BAD_BLOCK>
30008f0e:	2800      	cmp	r0, #0
30008f10:	d1f3      	bne.n	30008efa <NandImgCopy+0xb2>
30008f12:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008f16:	9602      	str	r6, [sp, #8]
30008f18:	e7d2      	b.n	30008ec0 <NandImgCopy+0x78>
30008f1a:	4642      	mov	r2, r8
30008f1c:	4620      	mov	r0, r4
30008f1e:	4d24      	ldr	r5, [pc, #144]	; (30008fb0 <NandImgCopy+0x168>)
30008f20:	47a8      	blx	r5
30008f22:	f010 0f70 	tst.w	r0, #112	; 0x70
30008f26:	d00d      	beq.n	30008f44 <NandImgCopy+0xfc>
30008f28:	4b20      	ldr	r3, [pc, #128]	; (30008fac <NandImgCopy+0x164>)
30008f2a:	689b      	ldr	r3, [r3, #8]
30008f2c:	079b      	lsls	r3, r3, #30
30008f2e:	d509      	bpl.n	30008f44 <NandImgCopy+0xfc>
30008f30:	4603      	mov	r3, r0
30008f32:	09a2      	lsrs	r2, r4, #6
30008f34:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008f38:	481e      	ldr	r0, [pc, #120]	; (30008fb4 <NandImgCopy+0x16c>)
30008f3a:	b005      	add	sp, #20
30008f3c:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008f40:	f003 b8f6 	b.w	3000c130 <__DiagPrintf_veneer>
30008f44:	b005      	add	sp, #20
30008f46:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008f4a:	4603      	mov	r3, r0
30008f4c:	09a2      	lsrs	r2, r4, #6
30008f4e:	f004 013f 	and.w	r1, r4, #63	; 0x3f
30008f52:	4818      	ldr	r0, [pc, #96]	; (30008fb4 <NandImgCopy+0x16c>)
30008f54:	f003 f8ec 	bl	3000c130 <__DiagPrintf_veneer>
30008f58:	e7a7      	b.n	30008eaa <NandImgCopy+0x62>
30008f5a:	f8df 905c 	ldr.w	r9, [pc, #92]	; 30008fb8 <NandImgCopy+0x170>
30008f5e:	f7ff ff4d 	bl	30008dfc <Nand_Get_NandAddr>
30008f62:	4605      	mov	r5, r0
30008f64:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008f68:	fa20 f406 	lsr.w	r4, r0, r6
30008f6c:	e781      	b.n	30008e72 <NandImgCopy+0x2a>
30008f6e:	f8df 9048 	ldr.w	r9, [pc, #72]	; 30008fb8 <NandImgCopy+0x170>
30008f72:	f101 4578 	add.w	r5, r1, #4160749568	; 0xf8000000
30008f76:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008f7a:	fa25 f406 	lsr.w	r4, r5, r6
30008f7e:	ea5f 1894 	movs.w	r8, r4, lsr #6
30008f82:	f43f af76 	beq.w	30008e72 <NandImgCopy+0x2a>
30008f86:	2400      	movs	r4, #0
30008f88:	2640      	movs	r6, #64	; 0x40
30008f8a:	4620      	mov	r0, r4
30008f8c:	3401      	adds	r4, #1
30008f8e:	f7ff fedd 	bl	30008d4c <NAND_CHECK_IS_BAD_BLOCK>
30008f92:	b130      	cbz	r0, 30008fa2 <NandImgCopy+0x15a>
30008f94:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008f98:	f108 0801 	add.w	r8, r8, #1
30008f9c:	fa06 f303 	lsl.w	r3, r6, r3
30008fa0:	441d      	add	r5, r3
30008fa2:	4544      	cmp	r4, r8
30008fa4:	d3f1      	bcc.n	30008f8a <NandImgCopy+0x142>
30008fa6:	e760      	b.n	30008e6a <NandImgCopy+0x22>
30008fa8:	3000ed08 	.word	0x3000ed08
30008fac:	2001c00c 	.word	0x2001c00c
30008fb0:	3000a3d1 	.word	0x3000a3d1
30008fb4:	3000d308 	.word	0x3000d308
30008fb8:	2001c01c 	.word	0x2001c01c

30008fbc <OTP_Read8.part.0>:
30008fbc:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008fc0:	4b2f      	ldr	r3, [pc, #188]	; (30009080 <OTP_Read8.part.0+0xc4>)
30008fc2:	4680      	mov	r8, r0
30008fc4:	e843 f300 	tt	r3, r3
30008fc8:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008fcc:	4a2d      	ldr	r2, [pc, #180]	; (30009084 <OTP_Read8.part.0+0xc8>)
30008fce:	4e2e      	ldr	r6, [pc, #184]	; (30009088 <OTP_Read8.part.0+0xcc>)
30008fd0:	4689      	mov	r9, r1
30008fd2:	2b00      	cmp	r3, #0
30008fd4:	bf0e      	itee	eq
30008fd6:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008fda:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008fde:	4616      	movne	r6, r2
30008fe0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fe2:	05d8      	lsls	r0, r3, #23
30008fe4:	d505      	bpl.n	30008ff2 <OTP_Read8.part.0+0x36>
30008fe6:	4d29      	ldr	r5, [pc, #164]	; (3000908c <OTP_Read8.part.0+0xd0>)
30008fe8:	2064      	movs	r0, #100	; 0x64
30008fea:	47a8      	blx	r5
30008fec:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008fee:	05d9      	lsls	r1, r3, #23
30008ff0:	d4fa      	bmi.n	30008fe8 <OTP_Read8.part.0+0x2c>
30008ff2:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008ff4:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008ff8:	643b      	str	r3, [r7, #64]	; 0x40
30008ffa:	4b25      	ldr	r3, [pc, #148]	; (30009090 <OTP_Read8.part.0+0xd4>)
30008ffc:	681a      	ldr	r2, [r3, #0]
30008ffe:	07d2      	lsls	r2, r2, #31
30009000:	d403      	bmi.n	3000900a <OTP_Read8.part.0+0x4e>
30009002:	681a      	ldr	r2, [r3, #0]
30009004:	f042 0201 	orr.w	r2, r2, #1
30009008:	601a      	str	r2, [r3, #0]
3000900a:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
3000900e:	ea4f 2308 	mov.w	r3, r8, lsl #8
30009012:	694a      	ldr	r2, [r1, #20]
30009014:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30009018:	614a      	str	r2, [r1, #20]
3000901a:	6033      	str	r3, [r6, #0]
3000901c:	6833      	ldr	r3, [r6, #0]
3000901e:	2b00      	cmp	r3, #0
30009020:	db10      	blt.n	30009044 <OTP_Read8.part.0+0x88>
30009022:	2400      	movs	r4, #0
30009024:	4d19      	ldr	r5, [pc, #100]	; (3000908c <OTP_Read8.part.0+0xd0>)
30009026:	f644 6a20 	movw	sl, #20000	; 0x4e20
3000902a:	e001      	b.n	30009030 <OTP_Read8.part.0+0x74>
3000902c:	4554      	cmp	r4, sl
3000902e:	d019      	beq.n	30009064 <OTP_Read8.part.0+0xa8>
30009030:	2005      	movs	r0, #5
30009032:	3401      	adds	r4, #1
30009034:	47a8      	blx	r5
30009036:	6833      	ldr	r3, [r6, #0]
30009038:	2b00      	cmp	r3, #0
3000903a:	daf7      	bge.n	3000902c <OTP_Read8.part.0+0x70>
3000903c:	f644 6320 	movw	r3, #20000	; 0x4e20
30009040:	429c      	cmp	r4, r3
30009042:	d00f      	beq.n	30009064 <OTP_Read8.part.0+0xa8>
30009044:	6833      	ldr	r3, [r6, #0]
30009046:	2001      	movs	r0, #1
30009048:	f889 3000 	strb.w	r3, [r9]
3000904c:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30009050:	6953      	ldr	r3, [r2, #20]
30009052:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30009056:	6153      	str	r3, [r2, #20]
30009058:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000905a:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000905e:	643b      	str	r3, [r7, #64]	; 0x40
30009060:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30009064:	22ff      	movs	r2, #255	; 0xff
30009066:	4b0b      	ldr	r3, [pc, #44]	; (30009094 <OTP_Read8.part.0+0xd8>)
30009068:	f889 2000 	strb.w	r2, [r9]
3000906c:	6818      	ldr	r0, [r3, #0]
3000906e:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
30009072:	d0eb      	beq.n	3000904c <OTP_Read8.part.0+0x90>
30009074:	4641      	mov	r1, r8
30009076:	4808      	ldr	r0, [pc, #32]	; (30009098 <OTP_Read8.part.0+0xdc>)
30009078:	f003 f85a 	bl	3000c130 <__DiagPrintf_veneer>
3000907c:	2000      	movs	r0, #0
3000907e:	e7e5      	b.n	3000904c <OTP_Read8.part.0+0x90>
30009080:	0000e5f9 	.word	0x0000e5f9
30009084:	5200000c 	.word	0x5200000c
30009088:	42000008 	.word	0x42000008
3000908c:	00009b2d 	.word	0x00009b2d
30009090:	42008000 	.word	0x42008000
30009094:	2001c00c 	.word	0x2001c00c
30009098:	3000d358 	.word	0x3000d358

3000909c <OTP_ProgramMarginRead8.part.0>:
3000909c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300090a0:	4b30      	ldr	r3, [pc, #192]	; (30009164 <OTP_ProgramMarginRead8.part.0+0xc8>)
300090a2:	4680      	mov	r8, r0
300090a4:	e843 f300 	tt	r3, r3
300090a8:	f3c3 5380 	ubfx	r3, r3, #22, #1
300090ac:	4a2e      	ldr	r2, [pc, #184]	; (30009168 <OTP_ProgramMarginRead8.part.0+0xcc>)
300090ae:	4e2f      	ldr	r6, [pc, #188]	; (3000916c <OTP_ProgramMarginRead8.part.0+0xd0>)
300090b0:	4689      	mov	r9, r1
300090b2:	2b00      	cmp	r3, #0
300090b4:	bf0e      	itee	eq
300090b6:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300090ba:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300090be:	4616      	movne	r6, r2
300090c0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090c2:	05d8      	lsls	r0, r3, #23
300090c4:	d505      	bpl.n	300090d2 <OTP_ProgramMarginRead8.part.0+0x36>
300090c6:	4d2a      	ldr	r5, [pc, #168]	; (30009170 <OTP_ProgramMarginRead8.part.0+0xd4>)
300090c8:	2064      	movs	r0, #100	; 0x64
300090ca:	47a8      	blx	r5
300090cc:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090ce:	05d9      	lsls	r1, r3, #23
300090d0:	d4fa      	bmi.n	300090c8 <OTP_ProgramMarginRead8.part.0+0x2c>
300090d2:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300090d4:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300090d8:	643b      	str	r3, [r7, #64]	; 0x40
300090da:	4b26      	ldr	r3, [pc, #152]	; (30009174 <OTP_ProgramMarginRead8.part.0+0xd8>)
300090dc:	681a      	ldr	r2, [r3, #0]
300090de:	07d2      	lsls	r2, r2, #31
300090e0:	d403      	bmi.n	300090ea <OTP_ProgramMarginRead8.part.0+0x4e>
300090e2:	681a      	ldr	r2, [r3, #0]
300090e4:	f042 0201 	orr.w	r2, r2, #1
300090e8:	601a      	str	r2, [r3, #0]
300090ea:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300090ee:	ea4f 2308 	mov.w	r3, r8, lsl #8
300090f2:	694a      	ldr	r2, [r1, #20]
300090f4:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
300090f8:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300090fc:	614a      	str	r2, [r1, #20]
300090fe:	6033      	str	r3, [r6, #0]
30009100:	6833      	ldr	r3, [r6, #0]
30009102:	2b00      	cmp	r3, #0
30009104:	db10      	blt.n	30009128 <OTP_ProgramMarginRead8.part.0+0x8c>
30009106:	2400      	movs	r4, #0
30009108:	4d19      	ldr	r5, [pc, #100]	; (30009170 <OTP_ProgramMarginRead8.part.0+0xd4>)
3000910a:	f644 6a20 	movw	sl, #20000	; 0x4e20
3000910e:	e001      	b.n	30009114 <OTP_ProgramMarginRead8.part.0+0x78>
30009110:	4554      	cmp	r4, sl
30009112:	d019      	beq.n	30009148 <OTP_ProgramMarginRead8.part.0+0xac>
30009114:	2005      	movs	r0, #5
30009116:	3401      	adds	r4, #1
30009118:	47a8      	blx	r5
3000911a:	6833      	ldr	r3, [r6, #0]
3000911c:	2b00      	cmp	r3, #0
3000911e:	daf7      	bge.n	30009110 <OTP_ProgramMarginRead8.part.0+0x74>
30009120:	f644 6320 	movw	r3, #20000	; 0x4e20
30009124:	429c      	cmp	r4, r3
30009126:	d00f      	beq.n	30009148 <OTP_ProgramMarginRead8.part.0+0xac>
30009128:	6833      	ldr	r3, [r6, #0]
3000912a:	2001      	movs	r0, #1
3000912c:	f889 3000 	strb.w	r3, [r9]
30009130:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30009134:	6953      	ldr	r3, [r2, #20]
30009136:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
3000913a:	6153      	str	r3, [r2, #20]
3000913c:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000913e:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30009142:	643b      	str	r3, [r7, #64]	; 0x40
30009144:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30009148:	22ff      	movs	r2, #255	; 0xff
3000914a:	4b0b      	ldr	r3, [pc, #44]	; (30009178 <OTP_ProgramMarginRead8.part.0+0xdc>)
3000914c:	f889 2000 	strb.w	r2, [r9]
30009150:	6818      	ldr	r0, [r3, #0]
30009152:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
30009156:	d0eb      	beq.n	30009130 <OTP_ProgramMarginRead8.part.0+0x94>
30009158:	4641      	mov	r1, r8
3000915a:	4808      	ldr	r0, [pc, #32]	; (3000917c <OTP_ProgramMarginRead8.part.0+0xe0>)
3000915c:	f002 ffe8 	bl	3000c130 <__DiagPrintf_veneer>
30009160:	2000      	movs	r0, #0
30009162:	e7e5      	b.n	30009130 <OTP_ProgramMarginRead8.part.0+0x94>
30009164:	0000e5f9 	.word	0x0000e5f9
30009168:	5200000c 	.word	0x5200000c
3000916c:	42000008 	.word	0x42000008
30009170:	00009b2d 	.word	0x00009b2d
30009174:	42008000 	.word	0x42008000
30009178:	2001c00c 	.word	0x2001c00c
3000917c:	3000d358 	.word	0x3000d358

30009180 <OTP_Read8>:
30009180:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009184:	d201      	bcs.n	3000918a <OTP_Read8+0xa>
30009186:	f7ff bf19 	b.w	30008fbc <OTP_Read8.part.0>
3000918a:	22ff      	movs	r2, #255	; 0xff
3000918c:	2000      	movs	r0, #0
3000918e:	700a      	strb	r2, [r1, #0]
30009190:	4770      	bx	lr
30009192:	bf00      	nop

30009194 <OTP_Write8>:
30009194:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009198:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000919c:	b085      	sub	sp, #20
3000919e:	d277      	bcs.n	30009290 <OTP_Write8+0xfc>
300091a0:	4689      	mov	r9, r1
300091a2:	f10d 010f 	add.w	r1, sp, #15
300091a6:	4680      	mov	r8, r0
300091a8:	f7ff ff78 	bl	3000909c <OTP_ProgramMarginRead8.part.0>
300091ac:	2800      	cmp	r0, #0
300091ae:	d072      	beq.n	30009296 <OTP_Write8+0x102>
300091b0:	ea4f 2308 	mov.w	r3, r8, lsl #8
300091b4:	2202      	movs	r2, #2
300091b6:	46ca      	mov	sl, r9
300091b8:	9301      	str	r3, [sp, #4]
300091ba:	f89d 300f 	ldrb.w	r3, [sp, #15]
300091be:	9200      	str	r2, [sp, #0]
300091c0:	ea6a 0a03 	orn	sl, sl, r3
300091c4:	fa5f fa8a 	uxtb.w	sl, sl
300091c8:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
300091cc:	d04f      	beq.n	3000926e <OTP_Write8+0xda>
300091ce:	4b52      	ldr	r3, [pc, #328]	; (30009318 <OTP_Write8+0x184>)
300091d0:	e843 f300 	tt	r3, r3
300091d4:	f3c3 5380 	ubfx	r3, r3, #22, #1
300091d8:	4a50      	ldr	r2, [pc, #320]	; (3000931c <OTP_Write8+0x188>)
300091da:	2b00      	cmp	r3, #0
300091dc:	4b50      	ldr	r3, [pc, #320]	; (30009320 <OTP_Write8+0x18c>)
300091de:	bf0b      	itete	eq
300091e0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300091e4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300091e8:	461d      	moveq	r5, r3
300091ea:	4615      	movne	r5, r2
300091ec:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091ee:	05dc      	lsls	r4, r3, #23
300091f0:	d505      	bpl.n	300091fe <OTP_Write8+0x6a>
300091f2:	4c4c      	ldr	r4, [pc, #304]	; (30009324 <OTP_Write8+0x190>)
300091f4:	2064      	movs	r0, #100	; 0x64
300091f6:	47a0      	blx	r4
300091f8:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300091fa:	05d8      	lsls	r0, r3, #23
300091fc:	d4fa      	bmi.n	300091f4 <OTP_Write8+0x60>
300091fe:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009200:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30009204:	643b      	str	r3, [r7, #64]	; 0x40
30009206:	4b48      	ldr	r3, [pc, #288]	; (30009328 <OTP_Write8+0x194>)
30009208:	681a      	ldr	r2, [r3, #0]
3000920a:	07d1      	lsls	r1, r2, #31
3000920c:	d403      	bmi.n	30009216 <OTP_Write8+0x82>
3000920e:	681a      	ldr	r2, [r3, #0]
30009210:	f042 0201 	orr.w	r2, r2, #1
30009214:	601a      	str	r2, [r3, #0]
30009216:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
3000921a:	9b01      	ldr	r3, [sp, #4]
3000921c:	694a      	ldr	r2, [r1, #20]
3000921e:	ea4a 0303 	orr.w	r3, sl, r3
30009222:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30009226:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
3000922a:	614a      	str	r2, [r1, #20]
3000922c:	602b      	str	r3, [r5, #0]
3000922e:	682b      	ldr	r3, [r5, #0]
30009230:	2b00      	cmp	r3, #0
30009232:	da12      	bge.n	3000925a <OTP_Write8+0xc6>
30009234:	f04f 0b00 	mov.w	fp, #0
30009238:	4c3a      	ldr	r4, [pc, #232]	; (30009324 <OTP_Write8+0x190>)
3000923a:	f644 6620 	movw	r6, #20000	; 0x4e20
3000923e:	e001      	b.n	30009244 <OTP_Write8+0xb0>
30009240:	45b3      	cmp	fp, r6
30009242:	d037      	beq.n	300092b4 <OTP_Write8+0x120>
30009244:	2005      	movs	r0, #5
30009246:	f10b 0b01 	add.w	fp, fp, #1
3000924a:	47a0      	blx	r4
3000924c:	682b      	ldr	r3, [r5, #0]
3000924e:	2b00      	cmp	r3, #0
30009250:	dbf6      	blt.n	30009240 <OTP_Write8+0xac>
30009252:	f644 6220 	movw	r2, #20000	; 0x4e20
30009256:	4593      	cmp	fp, r2
30009258:	d02c      	beq.n	300092b4 <OTP_Write8+0x120>
3000925a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
3000925e:	6953      	ldr	r3, [r2, #20]
30009260:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30009264:	6153      	str	r3, [r2, #20]
30009266:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30009268:	f423 7380 	bic.w	r3, r3, #256	; 0x100
3000926c:	643b      	str	r3, [r7, #64]	; 0x40
3000926e:	f10d 010f 	add.w	r1, sp, #15
30009272:	4640      	mov	r0, r8
30009274:	f7ff ff12 	bl	3000909c <OTP_ProgramMarginRead8.part.0>
30009278:	2800      	cmp	r0, #0
3000927a:	d040      	beq.n	300092fe <OTP_Write8+0x16a>
3000927c:	f89d 300f 	ldrb.w	r3, [sp, #15]
30009280:	454b      	cmp	r3, r9
30009282:	d046      	beq.n	30009312 <OTP_Write8+0x17e>
30009284:	9a00      	ldr	r2, [sp, #0]
30009286:	2a01      	cmp	r2, #1
30009288:	d009      	beq.n	3000929e <OTP_Write8+0x10a>
3000928a:	2201      	movs	r2, #1
3000928c:	9200      	str	r2, [sp, #0]
3000928e:	e797      	b.n	300091c0 <OTP_Write8+0x2c>
30009290:	23ff      	movs	r3, #255	; 0xff
30009292:	f88d 300f 	strb.w	r3, [sp, #15]
30009296:	4b25      	ldr	r3, [pc, #148]	; (3000932c <OTP_Write8+0x198>)
30009298:	681b      	ldr	r3, [r3, #0]
3000929a:	041d      	lsls	r5, r3, #16
3000929c:	d403      	bmi.n	300092a6 <OTP_Write8+0x112>
3000929e:	2000      	movs	r0, #0
300092a0:	b005      	add	sp, #20
300092a2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300092a6:	4822      	ldr	r0, [pc, #136]	; (30009330 <OTP_Write8+0x19c>)
300092a8:	f002 ff42 	bl	3000c130 <__DiagPrintf_veneer>
300092ac:	2000      	movs	r0, #0
300092ae:	b005      	add	sp, #20
300092b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300092b4:	4c1d      	ldr	r4, [pc, #116]	; (3000932c <OTP_Write8+0x198>)
300092b6:	6820      	ldr	r0, [r4, #0]
300092b8:	f410 4000 	ands.w	r0, r0, #32768	; 0x8000
300092bc:	d10a      	bne.n	300092d4 <OTP_Write8+0x140>
300092be:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300092c2:	6953      	ldr	r3, [r2, #20]
300092c4:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300092c8:	6153      	str	r3, [r2, #20]
300092ca:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092cc:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300092d0:	643b      	str	r3, [r7, #64]	; 0x40
300092d2:	e7e5      	b.n	300092a0 <OTP_Write8+0x10c>
300092d4:	4641      	mov	r1, r8
300092d6:	4817      	ldr	r0, [pc, #92]	; (30009334 <OTP_Write8+0x1a0>)
300092d8:	f002 ff2a 	bl	3000c130 <__DiagPrintf_veneer>
300092dc:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300092e0:	6953      	ldr	r3, [r2, #20]
300092e2:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300092e6:	6153      	str	r3, [r2, #20]
300092e8:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300092ea:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300092ee:	643b      	str	r3, [r7, #64]	; 0x40
300092f0:	6823      	ldr	r3, [r4, #0]
300092f2:	041a      	lsls	r2, r3, #16
300092f4:	d5d3      	bpl.n	3000929e <OTP_Write8+0x10a>
300092f6:	4810      	ldr	r0, [pc, #64]	; (30009338 <OTP_Write8+0x1a4>)
300092f8:	f002 ff1a 	bl	3000c130 <__DiagPrintf_veneer>
300092fc:	e7cf      	b.n	3000929e <OTP_Write8+0x10a>
300092fe:	4b0b      	ldr	r3, [pc, #44]	; (3000932c <OTP_Write8+0x198>)
30009300:	4604      	mov	r4, r0
30009302:	681b      	ldr	r3, [r3, #0]
30009304:	041b      	lsls	r3, r3, #16
30009306:	d5ca      	bpl.n	3000929e <OTP_Write8+0x10a>
30009308:	480c      	ldr	r0, [pc, #48]	; (3000933c <OTP_Write8+0x1a8>)
3000930a:	f002 ff11 	bl	3000c130 <__DiagPrintf_veneer>
3000930e:	4620      	mov	r0, r4
30009310:	e7c6      	b.n	300092a0 <OTP_Write8+0x10c>
30009312:	2001      	movs	r0, #1
30009314:	e7c4      	b.n	300092a0 <OTP_Write8+0x10c>
30009316:	bf00      	nop
30009318:	0000e5f9 	.word	0x0000e5f9
3000931c:	5200000c 	.word	0x5200000c
30009320:	42000008 	.word	0x42000008
30009324:	00009b2d 	.word	0x00009b2d
30009328:	42008000 	.word	0x42008000
3000932c:	2001c00c 	.word	0x2001c00c
30009330:	3000d388 	.word	0x3000d388
30009334:	3000d3b4 	.word	0x3000d3b4
30009338:	3000d3e4 	.word	0x3000d3e4
3000933c:	3000d414 	.word	0x3000d414

30009340 <EFUSERead8>:
30009340:	4608      	mov	r0, r1
30009342:	4611      	mov	r1, r2
30009344:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009348:	d201      	bcs.n	3000934e <EFUSERead8+0xe>
3000934a:	f7ff be37 	b.w	30008fbc <OTP_Read8.part.0>
3000934e:	23ff      	movs	r3, #255	; 0xff
30009350:	2000      	movs	r0, #0
30009352:	7013      	strb	r3, [r2, #0]
30009354:	4770      	bx	lr
30009356:	bf00      	nop

30009358 <EFUSE_PMAP_READ8>:
30009358:	4608      	mov	r0, r1
3000935a:	4611      	mov	r1, r2
3000935c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30009360:	d201      	bcs.n	30009366 <EFUSE_PMAP_READ8+0xe>
30009362:	f7ff be2b 	b.w	30008fbc <OTP_Read8.part.0>
30009366:	23ff      	movs	r3, #255	; 0xff
30009368:	2000      	movs	r0, #0
3000936a:	7013      	strb	r3, [r2, #0]
3000936c:	4770      	bx	lr
3000936e:	bf00      	nop

30009370 <PLL_NP_ClkSet>:
30009370:	b570      	push	{r4, r5, r6, lr}
30009372:	4604      	mov	r4, r0
30009374:	4b1b      	ldr	r3, [pc, #108]	; (300093e4 <PLL_NP_ClkSet+0x74>)
30009376:	4798      	blx	r3
30009378:	4d1b      	ldr	r5, [pc, #108]	; (300093e8 <PLL_NP_ClkSet+0x78>)
3000937a:	08c2      	lsrs	r2, r0, #3
3000937c:	fbb4 f3f0 	udiv	r3, r4, r0
30009380:	fb00 4c13 	mls	ip, r0, r3, r4
30009384:	1e9c      	subs	r4, r3, #2
30009386:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30009388:	0164      	lsls	r4, r4, #5
3000938a:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
3000938e:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30009392:	431c      	orrs	r4, r3
30009394:	fbbc f3f2 	udiv	r3, ip, r2
30009398:	646c      	str	r4, [r5, #68]	; 0x44
3000939a:	041c      	lsls	r4, r3, #16
3000939c:	fb02 c013 	mls	r0, r2, r3, ip
300093a0:	2300      	movs	r3, #0
300093a2:	6cee      	ldr	r6, [r5, #76]	; 0x4c
300093a4:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
300093a8:	4619      	mov	r1, r3
300093aa:	fa1f fc86 	uxth.w	ip, r6
300093ae:	4e0f      	ldr	r6, [pc, #60]	; (300093ec <PLL_NP_ClkSet+0x7c>)
300093b0:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
300093b4:	ea44 040c 	orr.w	r4, r4, ip
300093b8:	f002 fd22 	bl	3000be00 <__aeabi_uldivmod>
300093bc:	4603      	mov	r3, r0
300093be:	2001      	movs	r0, #1
300093c0:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
300093c4:	64ec      	str	r4, [r5, #76]	; 0x4c
300093c6:	47b0      	blx	r6
300093c8:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300093ca:	203c      	movs	r0, #60	; 0x3c
300093cc:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
300093d0:	646b      	str	r3, [r5, #68]	; 0x44
300093d2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
300093d4:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
300093d8:	646b      	str	r3, [r5, #68]	; 0x44
300093da:	4633      	mov	r3, r6
300093dc:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
300093e0:	4718      	bx	r3
300093e2:	bf00      	nop
300093e4:	000099f5 	.word	0x000099f5
300093e8:	42008800 	.word	0x42008800
300093ec:	00009b2d 	.word	0x00009b2d

300093f0 <PLL_AP_ClkSet>:
300093f0:	b510      	push	{r4, lr}
300093f2:	4604      	mov	r4, r0
300093f4:	4b0b      	ldr	r3, [pc, #44]	; (30009424 <PLL_AP_ClkSet+0x34>)
300093f6:	4798      	blx	r3
300093f8:	fbb4 f0f0 	udiv	r0, r4, r0
300093fc:	f1a0 0319 	sub.w	r3, r0, #25
30009400:	1e84      	subs	r4, r0, #2
30009402:	2b0f      	cmp	r3, #15
30009404:	d807      	bhi.n	30009416 <PLL_AP_ClkSet+0x26>
30009406:	4b08      	ldr	r3, [pc, #32]	; (30009428 <PLL_AP_ClkSet+0x38>)
30009408:	b2e0      	uxtb	r0, r4
3000940a:	695c      	ldr	r4, [r3, #20]
3000940c:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30009410:	4320      	orrs	r0, r4
30009412:	6158      	str	r0, [r3, #20]
30009414:	bd10      	pop	{r4, pc}
30009416:	f240 1175 	movw	r1, #373	; 0x175
3000941a:	4804      	ldr	r0, [pc, #16]	; (3000942c <PLL_AP_ClkSet+0x3c>)
3000941c:	f002 fea8 	bl	3000c170 <__io_assert_failed_veneer>
30009420:	e7f1      	b.n	30009406 <PLL_AP_ClkSet+0x16>
30009422:	bf00      	nop
30009424:	000099f5 	.word	0x000099f5
30009428:	41000300 	.word	0x41000300
3000942c:	3000d440 	.word	0x3000d440

30009430 <PLL_AP>:
30009430:	2801      	cmp	r0, #1
30009432:	b538      	push	{r3, r4, r5, lr}
30009434:	d009      	beq.n	3000944a <PLL_AP+0x1a>
30009436:	4b13      	ldr	r3, [pc, #76]	; (30009484 <PLL_AP+0x54>)
30009438:	681a      	ldr	r2, [r3, #0]
3000943a:	f022 0209 	bic.w	r2, r2, #9
3000943e:	601a      	str	r2, [r3, #0]
30009440:	681a      	ldr	r2, [r3, #0]
30009442:	f022 0206 	bic.w	r2, r2, #6
30009446:	601a      	str	r2, [r3, #0]
30009448:	bd38      	pop	{r3, r4, r5, pc}
3000944a:	4b0f      	ldr	r3, [pc, #60]	; (30009488 <PLL_AP+0x58>)
3000944c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
3000944e:	f012 0f05 	tst.w	r2, #5
30009452:	d00f      	beq.n	30009474 <PLL_AP+0x44>
30009454:	4d0d      	ldr	r5, [pc, #52]	; (3000948c <PLL_AP+0x5c>)
30009456:	4c0b      	ldr	r4, [pc, #44]	; (30009484 <PLL_AP+0x54>)
30009458:	2001      	movs	r0, #1
3000945a:	6823      	ldr	r3, [r4, #0]
3000945c:	f043 0306 	orr.w	r3, r3, #6
30009460:	6023      	str	r3, [r4, #0]
30009462:	47a8      	blx	r5
30009464:	6823      	ldr	r3, [r4, #0]
30009466:	f043 0309 	orr.w	r3, r3, #9
3000946a:	6023      	str	r3, [r4, #0]
3000946c:	6863      	ldr	r3, [r4, #4]
3000946e:	2b00      	cmp	r3, #0
30009470:	dafc      	bge.n	3000946c <PLL_AP+0x3c>
30009472:	bd38      	pop	{r3, r4, r5, pc}
30009474:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30009476:	20a0      	movs	r0, #160	; 0xa0
30009478:	4d04      	ldr	r5, [pc, #16]	; (3000948c <PLL_AP+0x5c>)
3000947a:	f042 0207 	orr.w	r2, r2, #7
3000947e:	679a      	str	r2, [r3, #120]	; 0x78
30009480:	47a8      	blx	r5
30009482:	e7e8      	b.n	30009456 <PLL_AP+0x26>
30009484:	41000300 	.word	0x41000300
30009488:	42008800 	.word	0x42008800
3000948c:	00009b2d 	.word	0x00009b2d

30009490 <PSRAM_INFO_Update>:
30009490:	4942      	ldr	r1, [pc, #264]	; (3000959c <PSRAM_INFO_Update+0x10c>)
30009492:	4a43      	ldr	r2, [pc, #268]	; (300095a0 <PSRAM_INFO_Update+0x110>)
30009494:	b538      	push	{r3, r4, r5, lr}
30009496:	f8d1 1228 	ldr.w	r1, [r1, #552]	; 0x228
3000949a:	6815      	ldr	r5, [r2, #0]
3000949c:	4b41      	ldr	r3, [pc, #260]	; (300095a4 <PSRAM_INFO_Update+0x114>)
3000949e:	f3c1 2102 	ubfx	r1, r1, #8, #3
300094a2:	4c41      	ldr	r4, [pc, #260]	; (300095a8 <PSRAM_INFO_Update+0x118>)
300094a4:	3101      	adds	r1, #1
300094a6:	fb03 f505 	mul.w	r5, r3, r5
300094aa:	4b40      	ldr	r3, [pc, #256]	; (300095ac <PSRAM_INFO_Update+0x11c>)
300094ac:	fbb5 f5f1 	udiv	r5, r5, r1
300094b0:	086a      	lsrs	r2, r5, #1
300094b2:	60e2      	str	r2, [r4, #12]
300094b4:	4798      	blx	r3
300094b6:	4b3e      	ldr	r3, [pc, #248]	; (300095b0 <PSRAM_INFO_Update+0x120>)
300094b8:	6020      	str	r0, [r4, #0]
300094ba:	4798      	blx	r3
300094bc:	4b3d      	ldr	r3, [pc, #244]	; (300095b4 <PSRAM_INFO_Update+0x124>)
300094be:	220a      	movs	r2, #10
300094c0:	60a0      	str	r0, [r4, #8]
300094c2:	6122      	str	r2, [r4, #16]
300094c4:	2220      	movs	r2, #32
300094c6:	fbb3 f3f5 	udiv	r3, r3, r5
300094ca:	6163      	str	r3, [r4, #20]
300094cc:	fbb2 f3f3 	udiv	r3, r2, r3
300094d0:	6822      	ldr	r2, [r4, #0]
300094d2:	6223      	str	r3, [r4, #32]
300094d4:	2a01      	cmp	r2, #1
300094d6:	68e3      	ldr	r3, [r4, #12]
300094d8:	4a37      	ldr	r2, [pc, #220]	; (300095b8 <PSRAM_INFO_Update+0x128>)
300094da:	d014      	beq.n	30009506 <PSRAM_INFO_Update+0x76>
300094dc:	4293      	cmp	r3, r2
300094de:	d808      	bhi.n	300094f2 <PSRAM_INFO_Update+0x62>
300094e0:	2204      	movs	r2, #4
300094e2:	2308      	movs	r3, #8
300094e4:	e9c4 2306 	strd	r2, r3, [r4, #24]
300094e8:	4b34      	ldr	r3, [pc, #208]	; (300095bc <PSRAM_INFO_Update+0x12c>)
300094ea:	689b      	ldr	r3, [r3, #8]
300094ec:	079b      	lsls	r3, r3, #30
300094ee:	d414      	bmi.n	3000951a <PSRAM_INFO_Update+0x8a>
300094f0:	bd38      	pop	{r3, r4, r5, pc}
300094f2:	4a33      	ldr	r2, [pc, #204]	; (300095c0 <PSRAM_INFO_Update+0x130>)
300094f4:	4293      	cmp	r3, r2
300094f6:	d91b      	bls.n	30009530 <PSRAM_INFO_Update+0xa0>
300094f8:	4a32      	ldr	r2, [pc, #200]	; (300095c4 <PSRAM_INFO_Update+0x134>)
300094fa:	4293      	cmp	r3, r2
300094fc:	d828      	bhi.n	30009550 <PSRAM_INFO_Update+0xc0>
300094fe:	2306      	movs	r3, #6
30009500:	e9c4 3306 	strd	r3, r3, [r4, #24]
30009504:	e7f0      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
30009506:	4293      	cmp	r3, r2
30009508:	d90d      	bls.n	30009526 <PSRAM_INFO_Update+0x96>
3000950a:	4a2d      	ldr	r2, [pc, #180]	; (300095c0 <PSRAM_INFO_Update+0x130>)
3000950c:	4293      	cmp	r3, r2
3000950e:	d814      	bhi.n	3000953a <PSRAM_INFO_Update+0xaa>
30009510:	2205      	movs	r2, #5
30009512:	230f      	movs	r3, #15
30009514:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009518:	e7e6      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
3000951a:	4629      	mov	r1, r5
3000951c:	482a      	ldr	r0, [pc, #168]	; (300095c8 <PSRAM_INFO_Update+0x138>)
3000951e:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
30009522:	f002 be05 	b.w	3000c130 <__DiagPrintf_veneer>
30009526:	2204      	movs	r2, #4
30009528:	230f      	movs	r3, #15
3000952a:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000952e:	e7db      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
30009530:	2205      	movs	r2, #5
30009532:	2308      	movs	r3, #8
30009534:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009538:	e7d6      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
3000953a:	4a22      	ldr	r2, [pc, #136]	; (300095c4 <PSRAM_INFO_Update+0x134>)
3000953c:	4293      	cmp	r3, r2
3000953e:	d91b      	bls.n	30009578 <PSRAM_INFO_Update+0xe8>
30009540:	4a22      	ldr	r2, [pc, #136]	; (300095cc <PSRAM_INFO_Update+0x13c>)
30009542:	4293      	cmp	r3, r2
30009544:	d822      	bhi.n	3000958c <PSRAM_INFO_Update+0xfc>
30009546:	2207      	movs	r2, #7
30009548:	2314      	movs	r3, #20
3000954a:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000954e:	e7cb      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
30009550:	4a1e      	ldr	r2, [pc, #120]	; (300095cc <PSRAM_INFO_Update+0x13c>)
30009552:	4293      	cmp	r3, r2
30009554:	d915      	bls.n	30009582 <PSRAM_INFO_Update+0xf2>
30009556:	4a1e      	ldr	r2, [pc, #120]	; (300095d0 <PSRAM_INFO_Update+0x140>)
30009558:	4293      	cmp	r3, r2
3000955a:	bf8c      	ite	hi
3000955c:	2300      	movhi	r3, #0
3000955e:	2301      	movls	r3, #1
30009560:	2804      	cmp	r0, #4
30009562:	d100      	bne.n	30009566 <PSRAM_INFO_Update+0xd6>
30009564:	b96b      	cbnz	r3, 30009582 <PSRAM_INFO_Update+0xf2>
30009566:	2803      	cmp	r0, #3
30009568:	d1be      	bne.n	300094e8 <PSRAM_INFO_Update+0x58>
3000956a:	2b00      	cmp	r3, #0
3000956c:	d0bc      	beq.n	300094e8 <PSRAM_INFO_Update+0x58>
3000956e:	220a      	movs	r2, #10
30009570:	2306      	movs	r3, #6
30009572:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009576:	e7b7      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
30009578:	2206      	movs	r2, #6
3000957a:	2312      	movs	r3, #18
3000957c:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009580:	e7b2      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
30009582:	2207      	movs	r2, #7
30009584:	2306      	movs	r3, #6
30009586:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000958a:	e7ad      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
3000958c:	4a10      	ldr	r2, [pc, #64]	; (300095d0 <PSRAM_INFO_Update+0x140>)
3000958e:	4293      	cmp	r3, r2
30009590:	d8aa      	bhi.n	300094e8 <PSRAM_INFO_Update+0x58>
30009592:	2208      	movs	r2, #8
30009594:	231c      	movs	r3, #28
30009596:	e9c4 2306 	strd	r2, r3, [r4, #24]
3000959a:	e7a5      	b.n	300094e8 <PSRAM_INFO_Update+0x58>
3000959c:	42008000 	.word	0x42008000
300095a0:	3000e2f0 	.word	0x3000e2f0
300095a4:	000f4240 	.word	0x000f4240
300095a8:	3000e9f0 	.word	0x3000e9f0
300095ac:	30007f15 	.word	0x30007f15
300095b0:	30007edd 	.word	0x30007edd
300095b4:	3b9aca00 	.word	0x3b9aca00
300095b8:	0632ea00 	.word	0x0632ea00
300095bc:	2001c00c 	.word	0x2001c00c
300095c0:	07ed6b40 	.word	0x07ed6b40
300095c4:	09e4f580 	.word	0x09e4f580
300095c8:	3000d450 	.word	0x3000d450
300095cc:	0bebc200 	.word	0x0bebc200
300095d0:	0ee6b280 	.word	0x0ee6b280

300095d4 <PSRAM_PHY_StructInit>:
300095d4:	2203      	movs	r2, #3
300095d6:	2301      	movs	r3, #1
300095d8:	210a      	movs	r1, #10
300095da:	b410      	push	{r4}
300095dc:	e9c0 2202 	strd	r2, r2, [r0, #8]
300095e0:	2402      	movs	r4, #2
300095e2:	2207      	movs	r2, #7
300095e4:	e9c0 3400 	strd	r3, r4, [r0]
300095e8:	e9c0 1304 	strd	r1, r3, [r0, #16]
300095ec:	f85d 4b04 	ldr.w	r4, [sp], #4
300095f0:	6182      	str	r2, [r0, #24]
300095f2:	4770      	bx	lr

300095f4 <PSRAM_PHY_Init>:
300095f4:	b538      	push	{r3, r4, r5, lr}
300095f6:	4604      	mov	r4, r0
300095f8:	6903      	ldr	r3, [r0, #16]
300095fa:	4d14      	ldr	r5, [pc, #80]	; (3000964c <PSRAM_PHY_Init+0x58>)
300095fc:	f003 031f 	and.w	r3, r3, #31
30009600:	4913      	ldr	r1, [pc, #76]	; (30009650 <PSRAM_PHY_Init+0x5c>)
30009602:	e9d0 0200 	ldrd	r0, r2, [r0]
30009606:	0612      	lsls	r2, r2, #24
30009608:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
3000960c:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30009610:	4313      	orrs	r3, r2
30009612:	68a2      	ldr	r2, [r4, #8]
30009614:	0412      	lsls	r2, r2, #16
30009616:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000961a:	4313      	orrs	r3, r2
3000961c:	68e2      	ldr	r2, [r4, #12]
3000961e:	0212      	lsls	r2, r2, #8
30009620:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009624:	4313      	orrs	r3, r2
30009626:	606b      	str	r3, [r5, #4]
30009628:	4788      	blx	r1
3000962a:	2801      	cmp	r0, #1
3000962c:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30009630:	bf08      	it	eq
30009632:	4a08      	ldreq	r2, [pc, #32]	; (30009654 <PSRAM_PHY_Init+0x60>)
30009634:	ea4f 1303 	mov.w	r3, r3, lsl #4
30009638:	f001 010f 	and.w	r1, r1, #15
3000963c:	bf18      	it	ne
3000963e:	4a06      	ldrne	r2, [pc, #24]	; (30009658 <PSRAM_PHY_Init+0x64>)
30009640:	b2db      	uxtb	r3, r3
30009642:	430b      	orrs	r3, r1
30009644:	431a      	orrs	r2, r3
30009646:	61aa      	str	r2, [r5, #24]
30009648:	bd38      	pop	{r3, r4, r5, pc}
3000964a:	bf00      	nop
3000964c:	41002000 	.word	0x41002000
30009650:	30007f15 	.word	0x30007f15
30009654:	80000e00 	.word	0x80000e00
30009658:	80000f00 	.word	0x80000f00

3000965c <PSRAM_CTRL_Init>:
3000965c:	b470      	push	{r4, r5, r6}
3000965e:	4b4c      	ldr	r3, [pc, #304]	; (30009790 <PSRAM_CTRL_Init+0x134>)
30009660:	2400      	movs	r4, #0
30009662:	4a4c      	ldr	r2, [pc, #304]	; (30009794 <PSRAM_CTRL_Init+0x138>)
30009664:	f24d 0101 	movw	r1, #53249	; 0xd001
30009668:	609c      	str	r4, [r3, #8]
3000966a:	2001      	movs	r0, #1
3000966c:	601a      	str	r2, [r3, #0]
3000966e:	4a4a      	ldr	r2, [pc, #296]	; (30009798 <PSRAM_CTRL_Init+0x13c>)
30009670:	6158      	str	r0, [r3, #20]
30009672:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30009676:	6811      	ldr	r1, [r2, #0]
30009678:	2902      	cmp	r1, #2
3000967a:	d04a      	beq.n	30009712 <PSRAM_CTRL_Init+0xb6>
3000967c:	6954      	ldr	r4, [r2, #20]
3000967e:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30009682:	69d0      	ldr	r0, [r2, #28]
30009684:	6915      	ldr	r5, [r2, #16]
30009686:	fbb0 f0f4 	udiv	r0, r0, r4
3000968a:	fbb1 f1f4 	udiv	r1, r1, r4
3000968e:	6994      	ldr	r4, [r2, #24]
30009690:	f005 020f 	and.w	r2, r5, #15
30009694:	f241 5551 	movw	r5, #5457	; 0x1551
30009698:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
3000969c:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
300096a0:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
300096a4:	094a      	lsrs	r2, r1, #5
300096a6:	0181      	lsls	r1, r0, #6
300096a8:	f000 003f 	and.w	r0, r0, #63	; 0x3f
300096ac:	4d3b      	ldr	r5, [pc, #236]	; (3000979c <PSRAM_CTRL_Init+0x140>)
300096ae:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
300096b2:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
300096b6:	00a2      	lsls	r2, r4, #2
300096b8:	4301      	orrs	r1, r0
300096ba:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300096be:	3a01      	subs	r2, #1
300096c0:	430d      	orrs	r5, r1
300096c2:	f242 0120 	movw	r1, #8224	; 0x2020
300096c6:	f3c2 020b 	ubfx	r2, r2, #0, #12
300096ca:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300096ce:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300096d2:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300096d6:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300096da:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300096de:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300096e2:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300096e6:	4420      	add	r0, r4
300096e8:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300096ec:	f640 71fe 	movw	r1, #4094	; 0xffe
300096f0:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300096f4:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300096f8:	432a      	orrs	r2, r5
300096fa:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300096fe:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30009702:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30009706:	4a22      	ldr	r2, [pc, #136]	; (30009790 <PSRAM_CTRL_Init+0x134>)
30009708:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000970a:	061b      	lsls	r3, r3, #24
3000970c:	d5fc      	bpl.n	30009708 <PSRAM_CTRL_Init+0xac>
3000970e:	bc70      	pop	{r4, r5, r6}
30009710:	4770      	bx	lr
30009712:	6955      	ldr	r5, [r2, #20]
30009714:	207d      	movs	r0, #125	; 0x7d
30009716:	69d1      	ldr	r1, [r2, #28]
30009718:	26a0      	movs	r6, #160	; 0xa0
3000971a:	6914      	ldr	r4, [r2, #16]
3000971c:	6992      	ldr	r2, [r2, #24]
3000971e:	f004 040f 	and.w	r4, r4, #15
30009722:	0092      	lsls	r2, r2, #2
30009724:	fbb1 f1f5 	udiv	r1, r1, r5
30009728:	fbb0 f5f5 	udiv	r5, r0, r5
3000972c:	f245 5051 	movw	r0, #21841	; 0x5551
30009730:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30009734:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30009738:	0189      	lsls	r1, r1, #6
3000973a:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
3000973e:	f641 4570 	movw	r5, #7280	; 0x1c70
30009742:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30009746:	4325      	orrs	r5, r4
30009748:	2420      	movs	r4, #32
3000974a:	4308      	orrs	r0, r1
3000974c:	1e51      	subs	r1, r2, #1
3000974e:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30009752:	3a02      	subs	r2, #2
30009754:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30009758:	f3c1 010b 	ubfx	r1, r1, #0, #12
3000975c:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009760:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30009764:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30009768:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
3000976c:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009770:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30009774:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30009778:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
3000977c:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009780:	4301      	orrs	r1, r0
30009782:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30009786:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000978a:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
3000978e:	e7ba      	b.n	30009706 <PSRAM_CTRL_Init+0xaa>
30009790:	41012000 	.word	0x41012000
30009794:	00bfe000 	.word	0x00bfe000
30009798:	3000e9f0 	.word	0x3000e9f0
3000979c:	00107000 	.word	0x00107000

300097a0 <PSRAM_REG_Write>:
300097a0:	b430      	push	{r4, r5}
300097a2:	4c20      	ldr	r4, [pc, #128]	; (30009824 <PSRAM_REG_Write+0x84>)
300097a4:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300097a6:	f013 0301 	ands.w	r3, r3, #1
300097aa:	d1fb      	bne.n	300097a4 <PSRAM_REG_Write+0x4>
300097ac:	6825      	ldr	r5, [r4, #0]
300097ae:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
300097b2:	6025      	str	r5, [r4, #0]
300097b4:	60a3      	str	r3, [r4, #8]
300097b6:	6825      	ldr	r5, [r4, #0]
300097b8:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300097bc:	6025      	str	r5, [r4, #0]
300097be:	6063      	str	r3, [r4, #4]
300097c0:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300097c4:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300097c8:	23c0      	movs	r3, #192	; 0xc0
300097ca:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300097ce:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300097d2:	0e03      	lsrs	r3, r0, #24
300097d4:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300097d8:	f3c0 4307 	ubfx	r3, r0, #16, #8
300097dc:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300097e0:	f3c0 2307 	ubfx	r3, r0, #8, #8
300097e4:	b2c0      	uxtb	r0, r0
300097e6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300097ea:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300097ee:	b141      	cbz	r1, 30009802 <PSRAM_REG_Write+0x62>
300097f0:	3a01      	subs	r2, #1
300097f2:	480c      	ldr	r0, [pc, #48]	; (30009824 <PSRAM_REG_Write+0x84>)
300097f4:	4411      	add	r1, r2
300097f6:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300097fa:	428a      	cmp	r2, r1
300097fc:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009800:	d1f9      	bne.n	300097f6 <PSRAM_REG_Write+0x56>
30009802:	4a08      	ldr	r2, [pc, #32]	; (30009824 <PSRAM_REG_Write+0x84>)
30009804:	2301      	movs	r3, #1
30009806:	6093      	str	r3, [r2, #8]
30009808:	6893      	ldr	r3, [r2, #8]
3000980a:	07d9      	lsls	r1, r3, #31
3000980c:	d4fc      	bmi.n	30009808 <PSRAM_REG_Write+0x68>
3000980e:	4a05      	ldr	r2, [pc, #20]	; (30009824 <PSRAM_REG_Write+0x84>)
30009810:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009812:	07db      	lsls	r3, r3, #31
30009814:	d4fc      	bmi.n	30009810 <PSRAM_REG_Write+0x70>
30009816:	6813      	ldr	r3, [r2, #0]
30009818:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000981c:	6013      	str	r3, [r2, #0]
3000981e:	bc30      	pop	{r4, r5}
30009820:	4770      	bx	lr
30009822:	bf00      	nop
30009824:	41012000 	.word	0x41012000

30009828 <PSRAM_APM_DEVIC_Init>:
30009828:	4b12      	ldr	r3, [pc, #72]	; (30009874 <PSRAM_APM_DEVIC_Init+0x4c>)
3000982a:	2102      	movs	r1, #2
3000982c:	2000      	movs	r0, #0
3000982e:	b530      	push	{r4, r5, lr}
30009830:	4c11      	ldr	r4, [pc, #68]	; (30009878 <PSRAM_APM_DEVIC_Init+0x50>)
30009832:	b083      	sub	sp, #12
30009834:	699d      	ldr	r5, [r3, #24]
30009836:	466a      	mov	r2, sp
30009838:	eb04 0c05 	add.w	ip, r4, r5
3000983c:	1eeb      	subs	r3, r5, #3
3000983e:	4c0f      	ldr	r4, [pc, #60]	; (3000987c <PSRAM_APM_DEVIC_Init+0x54>)
30009840:	408b      	lsls	r3, r1
30009842:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30009846:	f003 031c 	and.w	r3, r3, #28
3000984a:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000984e:	f043 0320 	orr.w	r3, r3, #32
30009852:	fa5f fc8c 	uxtb.w	ip, ip
30009856:	f88d 3000 	strb.w	r3, [sp]
3000985a:	f88d c004 	strb.w	ip, [sp, #4]
3000985e:	f88d 3001 	strb.w	r3, [sp, #1]
30009862:	f88d c005 	strb.w	ip, [sp, #5]
30009866:	47a0      	blx	r4
30009868:	aa01      	add	r2, sp, #4
3000986a:	2102      	movs	r1, #2
3000986c:	2004      	movs	r0, #4
3000986e:	47a0      	blx	r4
30009870:	b003      	add	sp, #12
30009872:	bd30      	pop	{r4, r5, pc}
30009874:	3000e9f0 	.word	0x3000e9f0
30009878:	3000e3e4 	.word	0x3000e3e4
3000987c:	300097a1 	.word	0x300097a1

30009880 <PSRAM_WB_REG_Write>:
30009880:	b430      	push	{r4, r5}
30009882:	4c1e      	ldr	r4, [pc, #120]	; (300098fc <PSRAM_WB_REG_Write+0x7c>)
30009884:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30009886:	f013 0301 	ands.w	r3, r3, #1
3000988a:	d1fb      	bne.n	30009884 <PSRAM_WB_REG_Write+0x4>
3000988c:	6825      	ldr	r5, [r4, #0]
3000988e:	b2c0      	uxtb	r0, r0
30009890:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009894:	6025      	str	r5, [r4, #0]
30009896:	60a3      	str	r3, [r4, #8]
30009898:	6825      	ldr	r5, [r4, #0]
3000989a:	f425 7540 	bic.w	r5, r5, #768	; 0x300
3000989e:	6025      	str	r5, [r4, #0]
300098a0:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
300098a4:	6063      	str	r3, [r4, #4]
300098a6:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
300098aa:	2560      	movs	r5, #96	; 0x60
300098ac:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300098b0:	2501      	movs	r5, #1
300098b2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098b6:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
300098ba:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098be:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300098c2:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300098c6:	b141      	cbz	r1, 300098da <PSRAM_WB_REG_Write+0x5a>
300098c8:	3a01      	subs	r2, #1
300098ca:	480c      	ldr	r0, [pc, #48]	; (300098fc <PSRAM_WB_REG_Write+0x7c>)
300098cc:	4411      	add	r1, r2
300098ce:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300098d2:	428a      	cmp	r2, r1
300098d4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300098d8:	d1f9      	bne.n	300098ce <PSRAM_WB_REG_Write+0x4e>
300098da:	4a08      	ldr	r2, [pc, #32]	; (300098fc <PSRAM_WB_REG_Write+0x7c>)
300098dc:	2301      	movs	r3, #1
300098de:	6093      	str	r3, [r2, #8]
300098e0:	6893      	ldr	r3, [r2, #8]
300098e2:	07d9      	lsls	r1, r3, #31
300098e4:	d4fc      	bmi.n	300098e0 <PSRAM_WB_REG_Write+0x60>
300098e6:	4a05      	ldr	r2, [pc, #20]	; (300098fc <PSRAM_WB_REG_Write+0x7c>)
300098e8:	6a93      	ldr	r3, [r2, #40]	; 0x28
300098ea:	07db      	lsls	r3, r3, #31
300098ec:	d4fc      	bmi.n	300098e8 <PSRAM_WB_REG_Write+0x68>
300098ee:	6813      	ldr	r3, [r2, #0]
300098f0:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300098f4:	6013      	str	r3, [r2, #0]
300098f6:	bc30      	pop	{r4, r5}
300098f8:	4770      	bx	lr
300098fa:	bf00      	nop
300098fc:	41012000 	.word	0x41012000

30009900 <PSRAM_WB_DEVIC_Init>:
30009900:	490b      	ldr	r1, [pc, #44]	; (30009930 <PSRAM_WB_DEVIC_Init+0x30>)
30009902:	4b0c      	ldr	r3, [pc, #48]	; (30009934 <PSRAM_WB_DEVIC_Init+0x34>)
30009904:	b510      	push	{r4, lr}
30009906:	6988      	ldr	r0, [r1, #24]
30009908:	b082      	sub	sp, #8
3000990a:	248f      	movs	r4, #143	; 0x8f
3000990c:	2102      	movs	r1, #2
3000990e:	4403      	add	r3, r0
30009910:	aa01      	add	r2, sp, #4
30009912:	f88d 4004 	strb.w	r4, [sp, #4]
30009916:	2000      	movs	r0, #0
30009918:	f813 3c03 	ldrb.w	r3, [r3, #-3]
3000991c:	4c06      	ldr	r4, [pc, #24]	; (30009938 <PSRAM_WB_DEVIC_Init+0x38>)
3000991e:	011b      	lsls	r3, r3, #4
30009920:	f043 030c 	orr.w	r3, r3, #12
30009924:	f88d 3005 	strb.w	r3, [sp, #5]
30009928:	47a0      	blx	r4
3000992a:	b002      	add	sp, #8
3000992c:	bd10      	pop	{r4, pc}
3000992e:	bf00      	nop
30009930:	3000e9f0 	.word	0x3000e9f0
30009934:	3000e404 	.word	0x3000e404
30009938:	30009881 	.word	0x30009881

3000993c <PSRAM_calibration>:
3000993c:	4a9d      	ldr	r2, [pc, #628]	; (30009bb4 <PSRAM_calibration+0x278>)
3000993e:	2300      	movs	r3, #0
30009940:	499d      	ldr	r1, [pc, #628]	; (30009bb8 <PSRAM_calibration+0x27c>)
30009942:	489e      	ldr	r0, [pc, #632]	; (30009bbc <PSRAM_calibration+0x280>)
30009944:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009948:	6852      	ldr	r2, [r2, #4]
3000994a:	b091      	sub	sp, #68	; 0x44
3000994c:	e9cd 2309 	strd	r2, r3, [sp, #36]	; 0x24
30009950:	400a      	ands	r2, r1
30009952:	930f      	str	r3, [sp, #60]	; 0x3c
30009954:	e9cd 330b 	strd	r3, r3, [sp, #44]	; 0x2c
30009958:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000995c:	9208      	str	r2, [sp, #32]
3000995e:	6942      	ldr	r2, [r0, #20]
30009960:	03d1      	lsls	r1, r2, #15
30009962:	d51e      	bpl.n	300099a2 <PSRAM_calibration+0x66>
30009964:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009968:	f3bf 8f4f 	dsb	sy
3000996c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009970:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009974:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009978:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000997c:	0164      	lsls	r4, r4, #5
3000997e:	ea04 0106 	and.w	r1, r4, r6
30009982:	462b      	mov	r3, r5
30009984:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009988:	3b01      	subs	r3, #1
3000998a:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000998e:	1c5a      	adds	r2, r3, #1
30009990:	d1f8      	bne.n	30009984 <PSRAM_calibration+0x48>
30009992:	3c20      	subs	r4, #32
30009994:	f114 0f20 	cmn.w	r4, #32
30009998:	d1f1      	bne.n	3000997e <PSRAM_calibration+0x42>
3000999a:	f3bf 8f4f 	dsb	sy
3000999e:	f3bf 8f6f 	isb	sy
300099a2:	4886      	ldr	r0, [pc, #536]	; (30009bbc <PSRAM_calibration+0x280>)
300099a4:	2300      	movs	r3, #0
300099a6:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300099aa:	f3bf 8f4f 	dsb	sy
300099ae:	6943      	ldr	r3, [r0, #20]
300099b0:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300099b4:	6143      	str	r3, [r0, #20]
300099b6:	f3bf 8f4f 	dsb	sy
300099ba:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300099be:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300099c2:	f3c5 344e 	ubfx	r4, r5, #13, #15
300099c6:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300099ca:	0164      	lsls	r4, r4, #5
300099cc:	ea04 0106 	and.w	r1, r4, r6
300099d0:	462b      	mov	r3, r5
300099d2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300099d6:	3b01      	subs	r3, #1
300099d8:	1c5f      	adds	r7, r3, #1
300099da:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300099de:	d1f8      	bne.n	300099d2 <PSRAM_calibration+0x96>
300099e0:	3c20      	subs	r4, #32
300099e2:	f114 0f20 	cmn.w	r4, #32
300099e6:	d1f1      	bne.n	300099cc <PSRAM_calibration+0x90>
300099e8:	f3bf 8f4f 	dsb	sy
300099ec:	f3bf 8f6f 	isb	sy
300099f0:	2204      	movs	r2, #4
300099f2:	461e      	mov	r6, r3
300099f4:	9304      	str	r3, [sp, #16]
300099f6:	2700      	movs	r7, #0
300099f8:	9207      	str	r2, [sp, #28]
300099fa:	46b3      	mov	fp, r6
300099fc:	4a6d      	ldr	r2, [pc, #436]	; (30009bb4 <PSRAM_calibration+0x278>)
300099fe:	4c70      	ldr	r4, [pc, #448]	; (30009bc0 <PSRAM_calibration+0x284>)
30009a00:	6813      	ldr	r3, [r2, #0]
30009a02:	f8df 91d8 	ldr.w	r9, [pc, #472]	; 30009bdc <PSRAM_calibration+0x2a0>
30009a06:	f023 0301 	bic.w	r3, r3, #1
30009a0a:	f8df a1d4 	ldr.w	sl, [pc, #468]	; 30009be0 <PSRAM_calibration+0x2a4>
30009a0e:	6013      	str	r3, [r2, #0]
30009a10:	2301      	movs	r3, #1
30009a12:	9303      	str	r3, [sp, #12]
30009a14:	9b08      	ldr	r3, [sp, #32]
30009a16:	9a03      	ldr	r2, [sp, #12]
30009a18:	9702      	str	r7, [sp, #8]
30009a1a:	2700      	movs	r7, #0
30009a1c:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009a20:	463d      	mov	r5, r7
30009a22:	e9cd 6b05 	strd	r6, fp, [sp, #20]
30009a26:	f04f 3bff 	mov.w	fp, #4294967295
30009a2a:	465e      	mov	r6, fp
30009a2c:	e010      	b.n	30009a50 <PSRAM_calibration+0x114>
30009a2e:	f1b6 3fff 	cmp.w	r6, #4294967295
30009a32:	bf08      	it	eq
30009a34:	462e      	moveq	r6, r5
30009a36:	2d1f      	cmp	r5, #31
30009a38:	eb06 0b07 	add.w	fp, r6, r7
30009a3c:	f107 0701 	add.w	r7, r7, #1
30009a40:	d102      	bne.n	30009a48 <PSRAM_calibration+0x10c>
30009a42:	9b02      	ldr	r3, [sp, #8]
30009a44:	429f      	cmp	r7, r3
30009a46:	dc46      	bgt.n	30009ad6 <PSRAM_calibration+0x19a>
30009a48:	3501      	adds	r5, #1
30009a4a:	2d20      	cmp	r5, #32
30009a4c:	f000 80ae 	beq.w	30009bac <PSRAM_calibration+0x270>
30009a50:	ea48 0205 	orr.w	r2, r8, r5
30009a54:	4b57      	ldr	r3, [pc, #348]	; (30009bb4 <PSRAM_calibration+0x278>)
30009a56:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
30009a5a:	495a      	ldr	r1, [pc, #360]	; (30009bc4 <PSRAM_calibration+0x288>)
30009a5c:	605a      	str	r2, [r3, #4]
30009a5e:	6822      	ldr	r2, [r4, #0]
30009a60:	4b59      	ldr	r3, [pc, #356]	; (30009bc8 <PSRAM_calibration+0x28c>)
30009a62:	f8ce 2000 	str.w	r2, [lr]
30009a66:	6860      	ldr	r0, [r4, #4]
30009a68:	4a58      	ldr	r2, [pc, #352]	; (30009bcc <PSRAM_calibration+0x290>)
30009a6a:	f8c9 0000 	str.w	r0, [r9]
30009a6e:	68a0      	ldr	r0, [r4, #8]
30009a70:	f8ca 0000 	str.w	r0, [sl]
30009a74:	68e0      	ldr	r0, [r4, #12]
30009a76:	6018      	str	r0, [r3, #0]
30009a78:	6920      	ldr	r0, [r4, #16]
30009a7a:	6008      	str	r0, [r1, #0]
30009a7c:	6960      	ldr	r0, [r4, #20]
30009a7e:	6010      	str	r0, [r2, #0]
30009a80:	f8de 0000 	ldr.w	r0, [lr]
30009a84:	900a      	str	r0, [sp, #40]	; 0x28
30009a86:	f8d9 0000 	ldr.w	r0, [r9]
30009a8a:	900b      	str	r0, [sp, #44]	; 0x2c
30009a8c:	f8da 0000 	ldr.w	r0, [sl]
30009a90:	900c      	str	r0, [sp, #48]	; 0x30
30009a92:	6818      	ldr	r0, [r3, #0]
30009a94:	4b4e      	ldr	r3, [pc, #312]	; (30009bd0 <PSRAM_calibration+0x294>)
30009a96:	900d      	str	r0, [sp, #52]	; 0x34
30009a98:	a80a      	add	r0, sp, #40	; 0x28
30009a9a:	6809      	ldr	r1, [r1, #0]
30009a9c:	910e      	str	r1, [sp, #56]	; 0x38
30009a9e:	6812      	ldr	r2, [r2, #0]
30009aa0:	4947      	ldr	r1, [pc, #284]	; (30009bc0 <PSRAM_calibration+0x284>)
30009aa2:	920f      	str	r2, [sp, #60]	; 0x3c
30009aa4:	2218      	movs	r2, #24
30009aa6:	4798      	blx	r3
30009aa8:	2800      	cmp	r0, #0
30009aaa:	d0c0      	beq.n	30009a2e <PSRAM_calibration+0xf2>
30009aac:	1c70      	adds	r0, r6, #1
30009aae:	d0cb      	beq.n	30009a48 <PSRAM_calibration+0x10c>
30009ab0:	9b02      	ldr	r3, [sp, #8]
30009ab2:	42bb      	cmp	r3, r7
30009ab4:	da0a      	bge.n	30009acc <PSRAM_calibration+0x190>
30009ab6:	9b03      	ldr	r3, [sp, #12]
30009ab8:	f8cd b018 	str.w	fp, [sp, #24]
30009abc:	f04f 3bff 	mov.w	fp, #4294967295
30009ac0:	9702      	str	r7, [sp, #8]
30009ac2:	2700      	movs	r7, #0
30009ac4:	9605      	str	r6, [sp, #20]
30009ac6:	465e      	mov	r6, fp
30009ac8:	9304      	str	r3, [sp, #16]
30009aca:	e7bd      	b.n	30009a48 <PSRAM_calibration+0x10c>
30009acc:	f04f 3bff 	mov.w	fp, #4294967295
30009ad0:	2700      	movs	r7, #0
30009ad2:	465e      	mov	r6, fp
30009ad4:	e7b8      	b.n	30009a48 <PSRAM_calibration+0x10c>
30009ad6:	9b03      	ldr	r3, [sp, #12]
30009ad8:	9304      	str	r3, [sp, #16]
30009ada:	9b03      	ldr	r3, [sp, #12]
30009adc:	005b      	lsls	r3, r3, #1
30009ade:	9303      	str	r3, [sp, #12]
30009ae0:	9b07      	ldr	r3, [sp, #28]
30009ae2:	3b01      	subs	r3, #1
30009ae4:	9307      	str	r3, [sp, #28]
30009ae6:	d195      	bne.n	30009a14 <PSRAM_calibration+0xd8>
30009ae8:	4b3a      	ldr	r3, [pc, #232]	; (30009bd4 <PSRAM_calibration+0x298>)
30009aea:	681b      	ldr	r3, [r3, #0]
30009aec:	0399      	lsls	r1, r3, #14
30009aee:	d454      	bmi.n	30009b9a <PSRAM_calibration+0x25e>
30009af0:	4832      	ldr	r0, [pc, #200]	; (30009bbc <PSRAM_calibration+0x280>)
30009af2:	6943      	ldr	r3, [r0, #20]
30009af4:	f413 3380 	ands.w	r3, r3, #65536	; 0x10000
30009af8:	d124      	bne.n	30009b44 <PSRAM_calibration+0x208>
30009afa:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009afe:	f3bf 8f4f 	dsb	sy
30009b02:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009b06:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
30009b0a:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009b0e:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009b12:	0164      	lsls	r4, r4, #5
30009b14:	ea04 010c 	and.w	r1, r4, ip
30009b18:	462b      	mov	r3, r5
30009b1a:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009b1e:	3b01      	subs	r3, #1
30009b20:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009b24:	1c5a      	adds	r2, r3, #1
30009b26:	d1f8      	bne.n	30009b1a <PSRAM_calibration+0x1de>
30009b28:	3c20      	subs	r4, #32
30009b2a:	f114 0f20 	cmn.w	r4, #32
30009b2e:	d1f1      	bne.n	30009b14 <PSRAM_calibration+0x1d8>
30009b30:	f3bf 8f4f 	dsb	sy
30009b34:	6943      	ldr	r3, [r0, #20]
30009b36:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30009b3a:	6143      	str	r3, [r0, #20]
30009b3c:	f3bf 8f4f 	dsb	sy
30009b40:	f3bf 8f6f 	isb	sy
30009b44:	2f08      	cmp	r7, #8
30009b46:	dd24      	ble.n	30009b92 <PSRAM_calibration+0x256>
30009b48:	ebbb 0306 	subs.w	r3, fp, r6
30009b4c:	9804      	ldr	r0, [sp, #16]
30009b4e:	bf48      	it	mi
30009b50:	3301      	addmi	r3, #1
30009b52:	eb1b 0606 	adds.w	r6, fp, r6
30009b56:	ea4f 0363 	mov.w	r3, r3, asr #1
30009b5a:	bf48      	it	mi
30009b5c:	3601      	addmi	r6, #1
30009b5e:	1e9a      	subs	r2, r3, #2
30009b60:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009b62:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009b66:	0414      	lsls	r4, r2, #16
30009b68:	f3c6 0344 	ubfx	r3, r6, #1, #5
30009b6c:	0212      	lsls	r2, r2, #8
30009b6e:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009b72:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009b76:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009b7a:	2001      	movs	r0, #1
30009b7c:	430b      	orrs	r3, r1
30009b7e:	490d      	ldr	r1, [pc, #52]	; (30009bb4 <PSRAM_calibration+0x278>)
30009b80:	4323      	orrs	r3, r4
30009b82:	4313      	orrs	r3, r2
30009b84:	604b      	str	r3, [r1, #4]
30009b86:	680b      	ldr	r3, [r1, #0]
30009b88:	4303      	orrs	r3, r0
30009b8a:	600b      	str	r3, [r1, #0]
30009b8c:	b011      	add	sp, #68	; 0x44
30009b8e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009b92:	2000      	movs	r0, #0
30009b94:	b011      	add	sp, #68	; 0x44
30009b96:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009b9a:	9b04      	ldr	r3, [sp, #16]
30009b9c:	465a      	mov	r2, fp
30009b9e:	4631      	mov	r1, r6
30009ba0:	480d      	ldr	r0, [pc, #52]	; (30009bd8 <PSRAM_calibration+0x29c>)
30009ba2:	9300      	str	r3, [sp, #0]
30009ba4:	463b      	mov	r3, r7
30009ba6:	f002 fac3 	bl	3000c130 <__DiagPrintf_veneer>
30009baa:	e7a1      	b.n	30009af0 <PSRAM_calibration+0x1b4>
30009bac:	9f02      	ldr	r7, [sp, #8]
30009bae:	e9dd 6b05 	ldrd	r6, fp, [sp, #20]
30009bb2:	e792      	b.n	30009ada <PSRAM_calibration+0x19e>
30009bb4:	41002000 	.word	0x41002000
30009bb8:	0fffffe0 	.word	0x0fffffe0
30009bbc:	e000ed00 	.word	0xe000ed00
30009bc0:	3000e3ec 	.word	0x3000e3ec
30009bc4:	60200000 	.word	0x60200000
30009bc8:	60150000 	.word	0x60150000
30009bcc:	60250000 	.word	0x60250000
30009bd0:	000129bd 	.word	0x000129bd
30009bd4:	2001c00c 	.word	0x2001c00c
30009bd8:	3000d484 	.word	0x3000d484
30009bdc:	60050000 	.word	0x60050000
30009be0:	60100000 	.word	0x60100000

30009be4 <PSRAM_AutoGating>:
30009be4:	b1c0      	cbz	r0, 30009c18 <PSRAM_AutoGating+0x34>
30009be6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30009bea:	4810      	ldr	r0, [pc, #64]	; (30009c2c <PSRAM_AutoGating+0x48>)
30009bec:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009bf0:	0252      	lsls	r2, r2, #9
30009bf2:	b410      	push	{r4}
30009bf4:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30009bf8:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
30009bfc:	4020      	ands	r0, r4
30009bfe:	4301      	orrs	r1, r0
30009c00:	430a      	orrs	r2, r1
30009c02:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30009c06:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
30009c0a:	f042 0201 	orr.w	r2, r2, #1
30009c0e:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30009c12:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c16:	4770      	bx	lr
30009c18:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30009c1c:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009c20:	f023 0301 	bic.w	r3, r3, #1
30009c24:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009c28:	4770      	bx	lr
30009c2a:	bf00      	nop
30009c2c:	ffffc000 	.word	0xffffc000

30009c30 <BOOT_Reason>:
30009c30:	4b02      	ldr	r3, [pc, #8]	; (30009c3c <BOOT_Reason+0xc>)
30009c32:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009c36:	b280      	uxth	r0, r0
30009c38:	4770      	bx	lr
30009c3a:	bf00      	nop
30009c3c:	42008200 	.word	0x42008200

30009c40 <CRYPTO_SetSecurityModeAD>:
30009c40:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009c44:	460d      	mov	r5, r1
30009c46:	4616      	mov	r6, r2
30009c48:	4699      	mov	r9, r3
30009c4a:	4604      	mov	r4, r0
30009c4c:	e9dd 8709 	ldrd	r8, r7, [sp, #36]	; 0x24
30009c50:	2800      	cmp	r0, #0
30009c52:	f000 80a6 	beq.w	30009da2 <CRYPTO_SetSecurityModeAD+0x162>
30009c56:	7823      	ldrb	r3, [r4, #0]
30009c58:	2b01      	cmp	r3, #1
30009c5a:	d003      	beq.n	30009c64 <CRYPTO_SetSecurityModeAD+0x24>
30009c5c:	2124      	movs	r1, #36	; 0x24
30009c5e:	485d      	ldr	r0, [pc, #372]	; (30009dd4 <CRYPTO_SetSecurityModeAD+0x194>)
30009c60:	f002 fa86 	bl	3000c170 <__io_assert_failed_veneer>
30009c64:	2300      	movs	r3, #0
30009c66:	1c69      	adds	r1, r5, #1
30009c68:	6065      	str	r5, [r4, #4]
30009c6a:	8123      	strh	r3, [r4, #8]
30009c6c:	d008      	beq.n	30009c80 <CRYPTO_SetSecurityModeAD+0x40>
30009c6e:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009c72:	2b20      	cmp	r3, #32
30009c74:	d078      	beq.n	30009d68 <CRYPTO_SetSecurityModeAD+0x128>
30009c76:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30009c7a:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009c7e:	7265      	strb	r5, [r4, #9]
30009c80:	1c72      	adds	r2, r6, #1
30009c82:	60e6      	str	r6, [r4, #12]
30009c84:	d07f      	beq.n	30009d86 <CRYPTO_SetSecurityModeAD+0x146>
30009c86:	f006 0303 	and.w	r3, r6, #3
30009c8a:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009c8e:	7422      	strb	r2, [r4, #16]
30009c90:	f1a3 0202 	sub.w	r2, r3, #2
30009c94:	2b01      	cmp	r3, #1
30009c96:	fab2 f282 	clz	r2, r2
30009c9a:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009c9e:	7462      	strb	r2, [r4, #17]
30009ca0:	fab3 f283 	clz	r2, r3
30009ca4:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009ca8:	74a2      	strb	r2, [r4, #18]
30009caa:	bf0c      	ite	eq
30009cac:	2201      	moveq	r2, #1
30009cae:	2200      	movne	r2, #0
30009cb0:	74e2      	strb	r2, [r4, #19]
30009cb2:	d13e      	bne.n	30009d32 <CRYPTO_SetSecurityModeAD+0xf2>
30009cb4:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009cb8:	2b40      	cmp	r3, #64	; 0x40
30009cba:	d07e      	beq.n	30009dba <CRYPTO_SetSecurityModeAD+0x17a>
30009cbc:	d869      	bhi.n	30009d92 <CRYPTO_SetSecurityModeAD+0x152>
30009cbe:	2b10      	cmp	r3, #16
30009cc0:	d06c      	beq.n	30009d9c <CRYPTO_SetSecurityModeAD+0x15c>
30009cc2:	2b20      	cmp	r3, #32
30009cc4:	d172      	bne.n	30009dac <CRYPTO_SetSecurityModeAD+0x16c>
30009cc6:	7523      	strb	r3, [r4, #20]
30009cc8:	62a3      	str	r3, [r4, #40]	; 0x28
30009cca:	9b08      	ldr	r3, [sp, #32]
30009ccc:	6227      	str	r7, [r4, #32]
30009cce:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009cd2:	2f00      	cmp	r7, #0
30009cd4:	d038      	beq.n	30009d48 <CRYPTO_SetSecurityModeAD+0x108>
30009cd6:	7d23      	ldrb	r3, [r4, #20]
30009cd8:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009cdc:	2b30      	cmp	r3, #48	; 0x30
30009cde:	65e0      	str	r0, [r4, #92]	; 0x5c
30009ce0:	d045      	beq.n	30009d6e <CRYPTO_SetSecurityModeAD+0x12e>
30009ce2:	2b40      	cmp	r3, #64	; 0x40
30009ce4:	d043      	beq.n	30009d6e <CRYPTO_SetSecurityModeAD+0x12e>
30009ce6:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009cea:	2240      	movs	r2, #64	; 0x40
30009cec:	2136      	movs	r1, #54	; 0x36
30009cee:	4d3a      	ldr	r5, [pc, #232]	; (30009dd8 <CRYPTO_SetSecurityModeAD+0x198>)
30009cf0:	6623      	str	r3, [r4, #96]	; 0x60
30009cf2:	47a8      	blx	r5
30009cf4:	2240      	movs	r2, #64	; 0x40
30009cf6:	215c      	movs	r1, #92	; 0x5c
30009cf8:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009cfa:	47a8      	blx	r5
30009cfc:	4b37      	ldr	r3, [pc, #220]	; (30009ddc <CRYPTO_SetSecurityModeAD+0x19c>)
30009cfe:	681d      	ldr	r5, [r3, #0]
30009d00:	bb25      	cbnz	r5, 30009d4c <CRYPTO_SetSecurityModeAD+0x10c>
30009d02:	f1b8 0f00 	cmp.w	r8, #0
30009d06:	d060      	beq.n	30009dca <CRYPTO_SetSecurityModeAD+0x18a>
30009d08:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009d0c:	e001      	b.n	30009d12 <CRYPTO_SetSecurityModeAD+0xd2>
30009d0e:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009d12:	6de2      	ldr	r2, [r4, #92]	; 0x5c
30009d14:	f818 3005 	ldrb.w	r3, [r8, r5]
30009d18:	5d51      	ldrb	r1, [r2, r5]
30009d1a:	404b      	eors	r3, r1
30009d1c:	5553      	strb	r3, [r2, r5]
30009d1e:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009d20:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009d22:	5d51      	ldrb	r1, [r2, r5]
30009d24:	5d5b      	ldrb	r3, [r3, r5]
30009d26:	404b      	eors	r3, r1
30009d28:	5553      	strb	r3, [r2, r5]
30009d2a:	3501      	adds	r5, #1
30009d2c:	42af      	cmp	r7, r5
30009d2e:	d1ee      	bne.n	30009d0e <CRYPTO_SetSecurityModeAD+0xce>
30009d30:	e00c      	b.n	30009d4c <CRYPTO_SetSecurityModeAD+0x10c>
30009d32:	2200      	movs	r2, #0
30009d34:	7522      	strb	r2, [r4, #20]
30009d36:	b993      	cbnz	r3, 30009d5e <CRYPTO_SetSecurityModeAD+0x11e>
30009d38:	2314      	movs	r3, #20
30009d3a:	6227      	str	r7, [r4, #32]
30009d3c:	62a3      	str	r3, [r4, #40]	; 0x28
30009d3e:	9b08      	ldr	r3, [sp, #32]
30009d40:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009d44:	2f00      	cmp	r7, #0
30009d46:	d1c6      	bne.n	30009cd6 <CRYPTO_SetSecurityModeAD+0x96>
30009d48:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009d4c:	3601      	adds	r6, #1
30009d4e:	d003      	beq.n	30009d58 <CRYPTO_SetSecurityModeAD+0x118>
30009d50:	2201      	movs	r2, #1
30009d52:	2300      	movs	r3, #0
30009d54:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009d56:	6323      	str	r3, [r4, #48]	; 0x30
30009d58:	2000      	movs	r0, #0
30009d5a:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009d5e:	2b02      	cmp	r3, #2
30009d60:	d114      	bne.n	30009d8c <CRYPTO_SetSecurityModeAD+0x14c>
30009d62:	2310      	movs	r3, #16
30009d64:	62a3      	str	r3, [r4, #40]	; 0x28
30009d66:	e7b0      	b.n	30009cca <CRYPTO_SetSecurityModeAD+0x8a>
30009d68:	2301      	movs	r3, #1
30009d6a:	7223      	strb	r3, [r4, #8]
30009d6c:	e783      	b.n	30009c76 <CRYPTO_SetSecurityModeAD+0x36>
30009d6e:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009d72:	2280      	movs	r2, #128	; 0x80
30009d74:	2136      	movs	r1, #54	; 0x36
30009d76:	4d18      	ldr	r5, [pc, #96]	; (30009dd8 <CRYPTO_SetSecurityModeAD+0x198>)
30009d78:	6623      	str	r3, [r4, #96]	; 0x60
30009d7a:	47a8      	blx	r5
30009d7c:	2280      	movs	r2, #128	; 0x80
30009d7e:	215c      	movs	r1, #92	; 0x5c
30009d80:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009d82:	47a8      	blx	r5
30009d84:	e7ba      	b.n	30009cfc <CRYPTO_SetSecurityModeAD+0xbc>
30009d86:	2300      	movs	r3, #0
30009d88:	6123      	str	r3, [r4, #16]
30009d8a:	7523      	strb	r3, [r4, #20]
30009d8c:	2300      	movs	r3, #0
30009d8e:	62a3      	str	r3, [r4, #40]	; 0x28
30009d90:	e79b      	b.n	30009cca <CRYPTO_SetSecurityModeAD+0x8a>
30009d92:	2b80      	cmp	r3, #128	; 0x80
30009d94:	d10a      	bne.n	30009dac <CRYPTO_SetSecurityModeAD+0x16c>
30009d96:	2340      	movs	r3, #64	; 0x40
30009d98:	7523      	strb	r3, [r4, #20]
30009d9a:	e795      	b.n	30009cc8 <CRYPTO_SetSecurityModeAD+0x88>
30009d9c:	231c      	movs	r3, #28
30009d9e:	7523      	strb	r3, [r4, #20]
30009da0:	e7f5      	b.n	30009d8e <CRYPTO_SetSecurityModeAD+0x14e>
30009da2:	2123      	movs	r1, #35	; 0x23
30009da4:	480b      	ldr	r0, [pc, #44]	; (30009dd4 <CRYPTO_SetSecurityModeAD+0x194>)
30009da6:	f002 f9e3 	bl	3000c170 <__io_assert_failed_veneer>
30009daa:	e754      	b.n	30009c56 <CRYPTO_SetSecurityModeAD+0x16>
30009dac:	4b0c      	ldr	r3, [pc, #48]	; (30009de0 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009dae:	681b      	ldr	r3, [r3, #0]
30009db0:	049b      	lsls	r3, r3, #18
30009db2:	d405      	bmi.n	30009dc0 <CRYPTO_SetSecurityModeAD+0x180>
30009db4:	2300      	movs	r3, #0
30009db6:	7523      	strb	r3, [r4, #20]
30009db8:	e7e9      	b.n	30009d8e <CRYPTO_SetSecurityModeAD+0x14e>
30009dba:	2330      	movs	r3, #48	; 0x30
30009dbc:	7523      	strb	r3, [r4, #20]
30009dbe:	e7e6      	b.n	30009d8e <CRYPTO_SetSecurityModeAD+0x14e>
30009dc0:	4631      	mov	r1, r6
30009dc2:	4808      	ldr	r0, [pc, #32]	; (30009de4 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009dc4:	f002 f9b4 	bl	3000c130 <__DiagPrintf_veneer>
30009dc8:	e7f4      	b.n	30009db4 <CRYPTO_SetSecurityModeAD+0x174>
30009dca:	217f      	movs	r1, #127	; 0x7f
30009dcc:	4801      	ldr	r0, [pc, #4]	; (30009dd4 <CRYPTO_SetSecurityModeAD+0x194>)
30009dce:	f002 f9cf 	bl	3000c170 <__io_assert_failed_veneer>
30009dd2:	e799      	b.n	30009d08 <CRYPTO_SetSecurityModeAD+0xc8>
30009dd4:	3000d55c 	.word	0x3000d55c
30009dd8:	00012be5 	.word	0x00012be5
30009ddc:	2001c200 	.word	0x2001c200
30009de0:	2001c00c 	.word	0x2001c00c
30009de4:	3000d4bc 	.word	0x3000d4bc

30009de8 <rtl_crypto_hmac_sha2_init>:
30009de8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009dec:	4c97      	ldr	r4, [pc, #604]	; (3000a04c <rtl_crypto_hmac_sha2_init+0x264>)
30009dee:	4617      	mov	r7, r2
30009df0:	e844 f400 	tt	r4, r4
30009df4:	4a96      	ldr	r2, [pc, #600]	; (3000a050 <rtl_crypto_hmac_sha2_init+0x268>)
30009df6:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
30009dfa:	461c      	mov	r4, r3
30009dfc:	4b95      	ldr	r3, [pc, #596]	; (3000a054 <rtl_crypto_hmac_sha2_init+0x26c>)
30009dfe:	bf18      	it	ne
30009e00:	4613      	movne	r3, r2
30009e02:	b085      	sub	sp, #20
30009e04:	4605      	mov	r5, r0
30009e06:	460e      	mov	r6, r1
30009e08:	6b5b      	ldr	r3, [r3, #52]	; 0x34
30009e0a:	2b00      	cmp	r3, #0
30009e0c:	db05      	blt.n	30009e1a <rtl_crypto_hmac_sha2_init+0x32>
30009e0e:	2900      	cmp	r1, #0
30009e10:	f000 8118 	beq.w	3000a044 <rtl_crypto_hmac_sha2_init+0x25c>
30009e14:	078b      	lsls	r3, r1, #30
30009e16:	f040 8112 	bne.w	3000a03e <rtl_crypto_hmac_sha2_init+0x256>
30009e1a:	2d20      	cmp	r5, #32
30009e1c:	bf18      	it	ne
30009e1e:	2d1c      	cmpne	r5, #28
30009e20:	bf0c      	ite	eq
30009e22:	f04f 0901 	moveq.w	r9, #1
30009e26:	f04f 0900 	movne.w	r9, #0
30009e2a:	d04d      	beq.n	30009ec8 <rtl_crypto_hmac_sha2_init+0xe0>
30009e2c:	2d40      	cmp	r5, #64	; 0x40
30009e2e:	bf18      	it	ne
30009e30:	2d30      	cmpne	r5, #48	; 0x30
30009e32:	d102      	bne.n	30009e3a <rtl_crypto_hmac_sha2_init+0x52>
30009e34:	2f80      	cmp	r7, #128	; 0x80
30009e36:	f200 80f9 	bhi.w	3000a02c <rtl_crypto_hmac_sha2_init+0x244>
30009e3a:	f8df 8238 	ldr.w	r8, [pc, #568]	; 3000a074 <rtl_crypto_hmac_sha2_init+0x28c>
30009e3e:	f898 3000 	ldrb.w	r3, [r8]
30009e42:	2b01      	cmp	r3, #1
30009e44:	d003      	beq.n	30009e4e <rtl_crypto_hmac_sha2_init+0x66>
30009e46:	21b3      	movs	r1, #179	; 0xb3
30009e48:	4883      	ldr	r0, [pc, #524]	; (3000a058 <rtl_crypto_hmac_sha2_init+0x270>)
30009e4a:	f002 f991 	bl	3000c170 <__io_assert_failed_veneer>
30009e4e:	2d30      	cmp	r5, #48	; 0x30
30009e50:	f000 809b 	beq.w	30009f8a <rtl_crypto_hmac_sha2_init+0x1a2>
30009e54:	d83f      	bhi.n	30009ed6 <rtl_crypto_hmac_sha2_init+0xee>
30009e56:	2d1c      	cmp	r5, #28
30009e58:	f000 80cb 	beq.w	30009ff2 <rtl_crypto_hmac_sha2_init+0x20a>
30009e5c:	f1b9 0f00 	cmp.w	r9, #0
30009e60:	d078      	beq.n	30009f54 <rtl_crypto_hmac_sha2_init+0x16c>
30009e62:	2c00      	cmp	r4, #0
30009e64:	f000 808f 	beq.w	30009f86 <rtl_crypto_hmac_sha2_init+0x19e>
30009e68:	4b7c      	ldr	r3, [pc, #496]	; (3000a05c <rtl_crypto_hmac_sha2_init+0x274>)
30009e6a:	f8df e210 	ldr.w	lr, [pc, #528]	; 3000a07c <rtl_crypto_hmac_sha2_init+0x294>
30009e6e:	f8df 921c 	ldr.w	r9, [pc, #540]	; 3000a08c <rtl_crypto_hmac_sha2_init+0x2a4>
30009e72:	4a7b      	ldr	r2, [pc, #492]	; (3000a060 <rtl_crypto_hmac_sha2_init+0x278>)
30009e74:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009e78:	4b7a      	ldr	r3, [pc, #488]	; (3000a064 <rtl_crypto_hmac_sha2_init+0x27c>)
30009e7a:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009e7e:	4b7a      	ldr	r3, [pc, #488]	; (3000a068 <rtl_crypto_hmac_sha2_init+0x280>)
30009e80:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009e84:	4b79      	ldr	r3, [pc, #484]	; (3000a06c <rtl_crypto_hmac_sha2_init+0x284>)
30009e86:	2225      	movs	r2, #37	; 0x25
30009e88:	6223      	str	r3, [r4, #32]
30009e8a:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009e8e:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009e92:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009e96:	6263      	str	r3, [r4, #36]	; 0x24
30009e98:	2300      	movs	r3, #0
30009e9a:	f04f 31ff 	mov.w	r1, #4294967295
30009e9e:	4d74      	ldr	r5, [pc, #464]	; (3000a070 <rtl_crypto_hmac_sha2_init+0x288>)
30009ea0:	9300      	str	r3, [sp, #0]
30009ea2:	9702      	str	r7, [sp, #8]
30009ea4:	9601      	str	r6, [sp, #4]
30009ea6:	4873      	ldr	r0, [pc, #460]	; (3000a074 <rtl_crypto_hmac_sha2_init+0x28c>)
30009ea8:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009eac:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009eb0:	47a8      	blx	r5
30009eb2:	4605      	mov	r5, r0
30009eb4:	4b70      	ldr	r3, [pc, #448]	; (3000a078 <rtl_crypto_hmac_sha2_init+0x290>)
30009eb6:	4620      	mov	r0, r4
30009eb8:	4798      	blx	r3
30009eba:	2301      	movs	r3, #1
30009ebc:	4628      	mov	r0, r5
30009ebe:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009ec2:	b005      	add	sp, #20
30009ec4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009ec8:	2f40      	cmp	r7, #64	; 0x40
30009eca:	d9b6      	bls.n	30009e3a <rtl_crypto_hmac_sha2_init+0x52>
30009ecc:	21ad      	movs	r1, #173	; 0xad
30009ece:	4862      	ldr	r0, [pc, #392]	; (3000a058 <rtl_crypto_hmac_sha2_init+0x270>)
30009ed0:	f002 f94e 	bl	3000c170 <__io_assert_failed_veneer>
30009ed4:	e7b1      	b.n	30009e3a <rtl_crypto_hmac_sha2_init+0x52>
30009ed6:	2d40      	cmp	r5, #64	; 0x40
30009ed8:	d13c      	bne.n	30009f54 <rtl_crypto_hmac_sha2_init+0x16c>
30009eda:	2c00      	cmp	r4, #0
30009edc:	d040      	beq.n	30009f60 <rtl_crypto_hmac_sha2_init+0x178>
30009ede:	4b67      	ldr	r3, [pc, #412]	; (3000a07c <rtl_crypto_hmac_sha2_init+0x294>)
30009ee0:	4a67      	ldr	r2, [pc, #412]	; (3000a080 <rtl_crypto_hmac_sha2_init+0x298>)
30009ee2:	4968      	ldr	r1, [pc, #416]	; (3000a084 <rtl_crypto_hmac_sha2_init+0x29c>)
30009ee4:	4d68      	ldr	r5, [pc, #416]	; (3000a088 <rtl_crypto_hmac_sha2_init+0x2a0>)
30009ee6:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 3000a0cc <rtl_crypto_hmac_sha2_init+0x2e4>
30009eea:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009eee:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009ef2:	2285      	movs	r2, #133	; 0x85
30009ef4:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
30009ef8:	33d4      	adds	r3, #212	; 0xd4
30009efa:	6163      	str	r3, [r4, #20]
30009efc:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009f00:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009f04:	f203 734a 	addw	r3, r3, #1866	; 0x74a
30009f08:	6123      	str	r3, [r4, #16]
30009f0a:	4b60      	ldr	r3, [pc, #384]	; (3000a08c <rtl_crypto_hmac_sha2_init+0x2a4>)
30009f0c:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009f10:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009f14:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
30009f18:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009f1c:	6263      	str	r3, [r4, #36]	; 0x24
30009f1e:	4b51      	ldr	r3, [pc, #324]	; (3000a064 <rtl_crypto_hmac_sha2_init+0x27c>)
30009f20:	6223      	str	r3, [r4, #32]
30009f22:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009f26:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
30009f2a:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009f2e:	62e3      	str	r3, [r4, #44]	; 0x2c
30009f30:	4b4b      	ldr	r3, [pc, #300]	; (3000a060 <rtl_crypto_hmac_sha2_init+0x278>)
30009f32:	62a3      	str	r3, [r4, #40]	; 0x28
30009f34:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30009f38:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009f3c:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009f40:	6363      	str	r3, [r4, #52]	; 0x34
30009f42:	4b49      	ldr	r3, [pc, #292]	; (3000a068 <rtl_crypto_hmac_sha2_init+0x280>)
30009f44:	6323      	str	r3, [r4, #48]	; 0x30
30009f46:	4b49      	ldr	r3, [pc, #292]	; (3000a06c <rtl_crypto_hmac_sha2_init+0x284>)
30009f48:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009f4c:	4b50      	ldr	r3, [pc, #320]	; (3000a090 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009f4e:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009f52:	e7a1      	b.n	30009e98 <rtl_crypto_hmac_sha2_init+0xb0>
30009f54:	f04f 35ff 	mov.w	r5, #4294967295
30009f58:	4628      	mov	r0, r5
30009f5a:	b005      	add	sp, #20
30009f5c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009f60:	2285      	movs	r2, #133	; 0x85
30009f62:	2300      	movs	r3, #0
30009f64:	f04f 31ff 	mov.w	r1, #4294967295
30009f68:	4c41      	ldr	r4, [pc, #260]	; (3000a070 <rtl_crypto_hmac_sha2_init+0x288>)
30009f6a:	9702      	str	r7, [sp, #8]
30009f6c:	4841      	ldr	r0, [pc, #260]	; (3000a074 <rtl_crypto_hmac_sha2_init+0x28c>)
30009f6e:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009f72:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009f76:	e9cd 3600 	strd	r3, r6, [sp]
30009f7a:	47a0      	blx	r4
30009f7c:	4605      	mov	r5, r0
30009f7e:	4628      	mov	r0, r5
30009f80:	b005      	add	sp, #20
30009f82:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009f86:	2225      	movs	r2, #37	; 0x25
30009f88:	e7eb      	b.n	30009f62 <rtl_crypto_hmac_sha2_init+0x17a>
30009f8a:	2c00      	cmp	r4, #0
30009f8c:	d053      	beq.n	3000a036 <rtl_crypto_hmac_sha2_init+0x24e>
30009f8e:	4b41      	ldr	r3, [pc, #260]	; (3000a094 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009f90:	f8df e130 	ldr.w	lr, [pc, #304]	; 3000a0c4 <rtl_crypto_hmac_sha2_init+0x2dc>
30009f94:	4a40      	ldr	r2, [pc, #256]	; (3000a098 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009f96:	4941      	ldr	r1, [pc, #260]	; (3000a09c <rtl_crypto_hmac_sha2_init+0x2b4>)
30009f98:	4d41      	ldr	r5, [pc, #260]	; (3000a0a0 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009f9a:	f8df c12c 	ldr.w	ip, [pc, #300]	; 3000a0c8 <rtl_crypto_hmac_sha2_init+0x2e0>
30009f9e:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009fa2:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30009fa6:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
30009faa:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30009fae:	6163      	str	r3, [r4, #20]
30009fb0:	4b3c      	ldr	r3, [pc, #240]	; (3000a0a4 <rtl_crypto_hmac_sha2_init+0x2bc>)
30009fb2:	6123      	str	r3, [r4, #16]
30009fb4:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30009fb8:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30009fbc:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30009fc0:	61e3      	str	r3, [r4, #28]
30009fc2:	4b39      	ldr	r3, [pc, #228]	; (3000a0a8 <rtl_crypto_hmac_sha2_init+0x2c0>)
30009fc4:	61a3      	str	r3, [r4, #24]
30009fc6:	4b39      	ldr	r3, [pc, #228]	; (3000a0ac <rtl_crypto_hmac_sha2_init+0x2c4>)
30009fc8:	e9c4 3208 	strd	r3, r2, [r4, #32]
30009fcc:	4b38      	ldr	r3, [pc, #224]	; (3000a0b0 <rtl_crypto_hmac_sha2_init+0x2c8>)
30009fce:	2245      	movs	r2, #69	; 0x45
30009fd0:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30009fd4:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
30009fd8:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30009fdc:	33aa      	adds	r3, #170	; 0xaa
30009fde:	6363      	str	r3, [r4, #52]	; 0x34
30009fe0:	4b34      	ldr	r3, [pc, #208]	; (3000a0b4 <rtl_crypto_hmac_sha2_init+0x2cc>)
30009fe2:	6323      	str	r3, [r4, #48]	; 0x30
30009fe4:	4b34      	ldr	r3, [pc, #208]	; (3000a0b8 <rtl_crypto_hmac_sha2_init+0x2d0>)
30009fe6:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009fea:	4b34      	ldr	r3, [pc, #208]	; (3000a0bc <rtl_crypto_hmac_sha2_init+0x2d4>)
30009fec:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009ff0:	e752      	b.n	30009e98 <rtl_crypto_hmac_sha2_init+0xb0>
30009ff2:	b314      	cbz	r4, 3000a03a <rtl_crypto_hmac_sha2_init+0x252>
30009ff4:	4b32      	ldr	r3, [pc, #200]	; (3000a0c0 <rtl_crypto_hmac_sha2_init+0x2d8>)
30009ff6:	2215      	movs	r2, #21
30009ff8:	4932      	ldr	r1, [pc, #200]	; (3000a0c4 <rtl_crypto_hmac_sha2_init+0x2dc>)
30009ffa:	4827      	ldr	r0, [pc, #156]	; (3000a098 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009ffc:	4d28      	ldr	r5, [pc, #160]	; (3000a0a0 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009ffe:	e9c4 1302 	strd	r1, r3, [r4, #8]
3000a002:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
3000a006:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
3000a00a:	f503 6301 	add.w	r3, r3, #2064	; 0x810
3000a00e:	6123      	str	r3, [r4, #16]
3000a010:	4b22      	ldr	r3, [pc, #136]	; (3000a09c <rtl_crypto_hmac_sha2_init+0x2b4>)
3000a012:	e9c4 0305 	strd	r0, r3, [r4, #20]
3000a016:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
3000a01a:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
3000a01e:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
3000a022:	61e3      	str	r3, [r4, #28]
3000a024:	4b28      	ldr	r3, [pc, #160]	; (3000a0c8 <rtl_crypto_hmac_sha2_init+0x2e0>)
3000a026:	e9c4 5308 	strd	r5, r3, [r4, #32]
3000a02a:	e735      	b.n	30009e98 <rtl_crypto_hmac_sha2_init+0xb0>
3000a02c:	21b0      	movs	r1, #176	; 0xb0
3000a02e:	480a      	ldr	r0, [pc, #40]	; (3000a058 <rtl_crypto_hmac_sha2_init+0x270>)
3000a030:	f002 f89e 	bl	3000c170 <__io_assert_failed_veneer>
3000a034:	e701      	b.n	30009e3a <rtl_crypto_hmac_sha2_init+0x52>
3000a036:	2245      	movs	r2, #69	; 0x45
3000a038:	e793      	b.n	30009f62 <rtl_crypto_hmac_sha2_init+0x17a>
3000a03a:	2215      	movs	r2, #21
3000a03c:	e791      	b.n	30009f62 <rtl_crypto_hmac_sha2_init+0x17a>
3000a03e:	f06f 0505 	mvn.w	r5, #5
3000a042:	e789      	b.n	30009f58 <rtl_crypto_hmac_sha2_init+0x170>
3000a044:	f06f 0503 	mvn.w	r5, #3
3000a048:	e786      	b.n	30009f58 <rtl_crypto_hmac_sha2_init+0x170>
3000a04a:	bf00      	nop
3000a04c:	0000e5f9 	.word	0x0000e5f9
3000a050:	500c8000 	.word	0x500c8000
3000a054:	400c8000 	.word	0x400c8000
3000a058:	3000d540 	.word	0x3000d540
3000a05c:	bb67ae85 	.word	0xbb67ae85
3000a060:	510e527f 	.word	0x510e527f
3000a064:	a54ff53a 	.word	0xa54ff53a
3000a068:	9b05688c 	.word	0x9b05688c
3000a06c:	1f83d9ab 	.word	0x1f83d9ab
3000a070:	30009c41 	.word	0x30009c41
3000a074:	2001c460 	.word	0x2001c460
3000a078:	00004c99 	.word	0x00004c99
3000a07c:	6a09e667 	.word	0x6a09e667
3000a080:	f3bcc908 	.word	0xf3bcc908
3000a084:	fe94f82b 	.word	0xfe94f82b
3000a088:	fb41bd6b 	.word	0xfb41bd6b
3000a08c:	3c6ef372 	.word	0x3c6ef372
3000a090:	5be0cd19 	.word	0x5be0cd19
3000a094:	cbbb9d5d 	.word	0xcbbb9d5d
3000a098:	f70e5939 	.word	0xf70e5939
3000a09c:	ffc00b31 	.word	0xffc00b31
3000a0a0:	64f98fa7 	.word	0x64f98fa7
3000a0a4:	629a292a 	.word	0x629a292a
3000a0a8:	9159015a 	.word	0x9159015a
3000a0ac:	152fecd8 	.word	0x152fecd8
3000a0b0:	67332667 	.word	0x67332667
3000a0b4:	8eb44a87 	.word	0x8eb44a87
3000a0b8:	db0c2e0d 	.word	0xdb0c2e0d
3000a0bc:	47b5481d 	.word	0x47b5481d
3000a0c0:	367cd507 	.word	0x367cd507
3000a0c4:	c1059ed8 	.word	0xc1059ed8
3000a0c8:	befa4fa4 	.word	0xbefa4fa4
3000a0cc:	137e2179 	.word	0x137e2179

3000a0d0 <CPU_ClkGet>:
3000a0d0:	b570      	push	{r4, r5, r6, lr}
3000a0d2:	4d1e      	ldr	r5, [pc, #120]	; (3000a14c <CPU_ClkGet+0x7c>)
3000a0d4:	4b1e      	ldr	r3, [pc, #120]	; (3000a150 <CPU_ClkGet+0x80>)
3000a0d6:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
3000a0da:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
3000a0de:	f3c4 1481 	ubfx	r4, r4, #6, #2
3000a0e2:	4798      	blx	r3
3000a0e4:	b92c      	cbnz	r4, 3000a0f2 <CPU_ClkGet+0x22>
3000a0e6:	2801      	cmp	r0, #1
3000a0e8:	d007      	beq.n	3000a0fa <CPU_ClkGet+0x2a>
3000a0ea:	07b2      	lsls	r2, r6, #30
3000a0ec:	d407      	bmi.n	3000a0fe <CPU_ClkGet+0x2e>
3000a0ee:	4819      	ldr	r0, [pc, #100]	; (3000a154 <CPU_ClkGet+0x84>)
3000a0f0:	bd70      	pop	{r4, r5, r6, pc}
3000a0f2:	2c01      	cmp	r4, #1
3000a0f4:	d007      	beq.n	3000a106 <CPU_ClkGet+0x36>
3000a0f6:	2000      	movs	r0, #0
3000a0f8:	bd70      	pop	{r4, r5, r6, pc}
3000a0fa:	4817      	ldr	r0, [pc, #92]	; (3000a158 <CPU_ClkGet+0x88>)
3000a0fc:	bd70      	pop	{r4, r5, r6, pc}
3000a0fe:	4b17      	ldr	r3, [pc, #92]	; (3000a15c <CPU_ClkGet+0x8c>)
3000a100:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000a104:	4718      	bx	r3
3000a106:	2801      	cmp	r0, #1
3000a108:	d011      	beq.n	3000a12e <CPU_ClkGet+0x5e>
3000a10a:	05f3      	lsls	r3, r6, #23
3000a10c:	d51c      	bpl.n	3000a148 <CPU_ClkGet+0x78>
3000a10e:	4914      	ldr	r1, [pc, #80]	; (3000a160 <CPU_ClkGet+0x90>)
3000a110:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
3000a114:	6c48      	ldr	r0, [r1, #68]	; 0x44
3000a116:	4a13      	ldr	r2, [pc, #76]	; (3000a164 <CPU_ClkGet+0x94>)
3000a118:	f003 0307 	and.w	r3, r3, #7
3000a11c:	f3c0 1045 	ubfx	r0, r0, #5, #6
3000a120:	3301      	adds	r3, #1
3000a122:	3002      	adds	r0, #2
3000a124:	fb02 f000 	mul.w	r0, r2, r0
3000a128:	fbb0 f0f3 	udiv	r0, r0, r3
3000a12c:	bd70      	pop	{r4, r5, r6, pc}
3000a12e:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
3000a132:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
3000a136:	f3c3 4007 	ubfx	r0, r3, #16, #8
3000a13a:	d003      	beq.n	3000a144 <CPU_ClkGet+0x74>
3000a13c:	4b0a      	ldr	r3, [pc, #40]	; (3000a168 <CPU_ClkGet+0x98>)
3000a13e:	fb03 f000 	mul.w	r0, r3, r0
3000a142:	bd70      	pop	{r4, r5, r6, pc}
3000a144:	4809      	ldr	r0, [pc, #36]	; (3000a16c <CPU_ClkGet+0x9c>)
3000a146:	bd70      	pop	{r4, r5, r6, pc}
3000a148:	4806      	ldr	r0, [pc, #24]	; (3000a164 <CPU_ClkGet+0x94>)
3000a14a:	bd70      	pop	{r4, r5, r6, pc}
3000a14c:	42008000 	.word	0x42008000
3000a150:	0000c0f9 	.word	0x0000c0f9
3000a154:	003d0900 	.word	0x003d0900
3000a158:	01312d00 	.word	0x01312d00
3000a15c:	000099f5 	.word	0x000099f5
3000a160:	42008800 	.word	0x42008800
3000a164:	02625a00 	.word	0x02625a00
3000a168:	000f4240 	.word	0x000f4240
3000a16c:	02faf080 	.word	0x02faf080

3000a170 <RSIP_IV_Set>:
3000a170:	4b05      	ldr	r3, [pc, #20]	; (3000a188 <RSIP_IV_Set+0x18>)
3000a172:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
3000a176:	680b      	ldr	r3, [r1, #0]
3000a178:	ba1b      	rev	r3, r3
3000a17a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000a17e:	684b      	ldr	r3, [r1, #4]
3000a180:	ba1b      	rev	r3, r3
3000a182:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
3000a186:	4770      	bx	lr
3000a188:	42008c00 	.word	0x42008c00

3000a18c <RSIP_OTF_Cmd>:
3000a18c:	4a04      	ldr	r2, [pc, #16]	; (3000a1a0 <RSIP_OTF_Cmd+0x14>)
3000a18e:	2801      	cmp	r0, #1
3000a190:	6953      	ldr	r3, [r2, #20]
3000a192:	bf0c      	ite	eq
3000a194:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
3000a198:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
3000a19c:	6153      	str	r3, [r2, #20]
3000a19e:	4770      	bx	lr
3000a1a0:	42008c00 	.word	0x42008c00

3000a1a4 <RSIP_OTF_Enable>:
3000a1a4:	b570      	push	{r4, r5, r6, lr}
3000a1a6:	4604      	mov	r4, r0
3000a1a8:	b1f3      	cbz	r3, 3000a1e8 <RSIP_OTF_Enable+0x44>
3000a1aa:	9b04      	ldr	r3, [sp, #16]
3000a1ac:	460d      	mov	r5, r1
3000a1ae:	4616      	mov	r6, r2
3000a1b0:	2b07      	cmp	r3, #7
3000a1b2:	d827      	bhi.n	3000a204 <RSIP_OTF_Enable+0x60>
3000a1b4:	9b05      	ldr	r3, [sp, #20]
3000a1b6:	3b01      	subs	r3, #1
3000a1b8:	2b01      	cmp	r3, #1
3000a1ba:	d81d      	bhi.n	3000a1f8 <RSIP_OTF_Enable+0x54>
3000a1bc:	9a04      	ldr	r2, [sp, #16]
3000a1be:	9b05      	ldr	r3, [sp, #20]
3000a1c0:	f002 0007 	and.w	r0, r2, #7
3000a1c4:	005b      	lsls	r3, r3, #1
3000a1c6:	4305      	orrs	r5, r0
3000a1c8:	4813      	ldr	r0, [pc, #76]	; (3000a218 <RSIP_OTF_Enable+0x74>)
3000a1ca:	f003 0306 	and.w	r3, r3, #6
3000a1ce:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a1d2:	ea43 0206 	orr.w	r2, r3, r6
3000a1d6:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a1d8:	f042 0201 	orr.w	r2, r2, #1
3000a1dc:	f023 0301 	bic.w	r3, r3, #1
3000a1e0:	6463      	str	r3, [r4, #68]	; 0x44
3000a1e2:	6425      	str	r5, [r4, #64]	; 0x40
3000a1e4:	6462      	str	r2, [r4, #68]	; 0x44
3000a1e6:	bd70      	pop	{r4, r5, r6, pc}
3000a1e8:	480b      	ldr	r0, [pc, #44]	; (3000a218 <RSIP_OTF_Enable+0x74>)
3000a1ea:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
3000a1ee:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000a1f0:	f023 0301 	bic.w	r3, r3, #1
3000a1f4:	6463      	str	r3, [r4, #68]	; 0x44
3000a1f6:	bd70      	pop	{r4, r5, r6, pc}
3000a1f8:	f44f 71f3 	mov.w	r1, #486	; 0x1e6
3000a1fc:	4807      	ldr	r0, [pc, #28]	; (3000a21c <RSIP_OTF_Enable+0x78>)
3000a1fe:	f001 ffb7 	bl	3000c170 <__io_assert_failed_veneer>
3000a202:	e7db      	b.n	3000a1bc <RSIP_OTF_Enable+0x18>
3000a204:	f240 11e5 	movw	r1, #485	; 0x1e5
3000a208:	4804      	ldr	r0, [pc, #16]	; (3000a21c <RSIP_OTF_Enable+0x78>)
3000a20a:	f001 ffb1 	bl	3000c170 <__io_assert_failed_veneer>
3000a20e:	9b05      	ldr	r3, [sp, #20]
3000a210:	3b01      	subs	r3, #1
3000a212:	2b01      	cmp	r3, #1
3000a214:	d9d2      	bls.n	3000a1bc <RSIP_OTF_Enable+0x18>
3000a216:	e7ef      	b.n	3000a1f8 <RSIP_OTF_Enable+0x54>
3000a218:	42008c00 	.word	0x42008c00
3000a21c:	3000d530 	.word	0x3000d530

3000a220 <RSIP_MMU_Config>:
3000a220:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a224:	311f      	adds	r1, #31
3000a226:	321f      	adds	r2, #31
3000a228:	331f      	adds	r3, #31
3000a22a:	0080      	lsls	r0, r0, #2
3000a22c:	0949      	lsrs	r1, r1, #5
3000a22e:	0952      	lsrs	r2, r2, #5
3000a230:	095b      	lsrs	r3, r3, #5
3000a232:	0249      	lsls	r1, r1, #9
3000a234:	0252      	lsls	r2, r2, #9
3000a236:	025b      	lsls	r3, r3, #9
3000a238:	b410      	push	{r4}
3000a23a:	4c08      	ldr	r4, [pc, #32]	; (3000a25c <RSIP_MMU_Config+0x3c>)
3000a23c:	4404      	add	r4, r0
3000a23e:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
3000a242:	f020 0001 	bic.w	r0, r0, #1
3000a246:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
3000a24a:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
3000a24e:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
3000a252:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
3000a256:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a25a:	4770      	bx	lr
3000a25c:	42008c00 	.word	0x42008c00

3000a260 <RSIP_MMU_Cmd>:
3000a260:	eb00 0040 	add.w	r0, r0, r0, lsl #1
3000a264:	4b07      	ldr	r3, [pc, #28]	; (3000a284 <RSIP_MMU_Cmd+0x24>)
3000a266:	0080      	lsls	r0, r0, #2
3000a268:	4403      	add	r3, r0
3000a26a:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000a26e:	b121      	cbz	r1, 3000a27a <RSIP_MMU_Cmd+0x1a>
3000a270:	f042 0201 	orr.w	r2, r2, #1
3000a274:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a278:	4770      	bx	lr
3000a27a:	f022 0201 	bic.w	r2, r2, #1
3000a27e:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000a282:	4770      	bx	lr
3000a284:	42008c00 	.word	0x42008c00

3000a288 <RSIP_MMU_Cache_Clean>:
3000a288:	4a02      	ldr	r2, [pc, #8]	; (3000a294 <RSIP_MMU_Cache_Clean+0xc>)
3000a28a:	6993      	ldr	r3, [r2, #24]
3000a28c:	f043 0304 	orr.w	r3, r3, #4
3000a290:	6193      	str	r3, [r2, #24]
3000a292:	4770      	bx	lr
3000a294:	42008c00 	.word	0x42008c00

3000a298 <NAND_RxData>:
3000a298:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
3000a29c:	460c      	mov	r4, r1
3000a29e:	4616      	mov	r6, r2
3000a2a0:	461f      	mov	r7, r3
3000a2a2:	2800      	cmp	r0, #0
3000a2a4:	f000 808a 	beq.w	3000a3bc <NAND_RxData+0x124>
3000a2a8:	4605      	mov	r5, r0
3000a2aa:	f8df 911c 	ldr.w	r9, [pc, #284]	; 3000a3c8 <NAND_RxData+0x130>
3000a2ae:	2000      	movs	r0, #0
3000a2b0:	f8df 8118 	ldr.w	r8, [pc, #280]	; 3000a3cc <NAND_RxData+0x134>
3000a2b4:	47c0      	blx	r8
3000a2b6:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
3000a2ba:	2000      	movs	r0, #0
3000a2bc:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
3000a2c0:	6819      	ldr	r1, [r3, #0]
3000a2c2:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
3000a2c6:	6019      	str	r1, [r3, #0]
3000a2c8:	6819      	ldr	r1, [r3, #0]
3000a2ca:	f441 7140 	orr.w	r1, r1, #768	; 0x300
3000a2ce:	6019      	str	r1, [r3, #0]
3000a2d0:	605a      	str	r2, [r3, #4]
3000a2d2:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
3000a2d6:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
3000a2da:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
3000a2de:	ea4f 420c 	mov.w	r2, ip, lsl #16
3000a2e2:	f1bc 0f03 	cmp.w	ip, #3
3000a2e6:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
3000a2ea:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a2ee:	ea42 0201 	orr.w	r2, r2, r1
3000a2f2:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
3000a2f6:	f04f 0201 	mov.w	r2, #1
3000a2fa:	611a      	str	r2, [r3, #16]
3000a2fc:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
3000a300:	bf08      	it	eq
3000a302:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
3000a306:	f3c4 2307 	ubfx	r3, r4, #8, #8
3000a30a:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
3000a30e:	b2e4      	uxtb	r4, r4
3000a310:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000a314:	2301      	movs	r3, #1
3000a316:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
3000a31a:	f017 0403 	ands.w	r4, r7, #3
3000a31e:	6083      	str	r3, [r0, #8]
3000a320:	d137      	bne.n	3000a392 <NAND_RxData+0xfa>
3000a322:	463b      	mov	r3, r7
3000a324:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
3000a328:	42b4      	cmp	r4, r6
3000a32a:	d216      	bcs.n	3000a35a <NAND_RxData+0xc2>
3000a32c:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
3000a330:	f012 0f3c 	tst.w	r2, #60	; 0x3c
3000a334:	f3c2 0583 	ubfx	r5, r2, #2, #4
3000a338:	d00c      	beq.n	3000a354 <NAND_RxData+0xbc>
3000a33a:	00ad      	lsls	r5, r5, #2
3000a33c:	1f19      	subs	r1, r3, #4
3000a33e:	f1a5 0c04 	sub.w	ip, r5, #4
3000a342:	449c      	add	ip, r3
3000a344:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
3000a348:	f841 0f04 	str.w	r0, [r1, #4]!
3000a34c:	458c      	cmp	ip, r1
3000a34e:	d1f9      	bne.n	3000a344 <NAND_RxData+0xac>
3000a350:	442b      	add	r3, r5
3000a352:	442c      	add	r4, r5
3000a354:	1b32      	subs	r2, r6, r4
3000a356:	2a03      	cmp	r2, #3
3000a358:	d8e6      	bhi.n	3000a328 <NAND_RxData+0x90>
3000a35a:	1e63      	subs	r3, r4, #1
3000a35c:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a360:	441f      	add	r7, r3
3000a362:	e002      	b.n	3000a36a <NAND_RxData+0xd2>
3000a364:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000a366:	071b      	lsls	r3, r3, #28
3000a368:	d40d      	bmi.n	3000a386 <NAND_RxData+0xee>
3000a36a:	42b4      	cmp	r4, r6
3000a36c:	d3fa      	bcc.n	3000a364 <NAND_RxData+0xcc>
3000a36e:	2004      	movs	r0, #4
3000a370:	47c0      	blx	r8
3000a372:	2000      	movs	r0, #0
3000a374:	47c0      	blx	r8
3000a376:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
3000a37a:	6813      	ldr	r3, [r2, #0]
3000a37c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000a380:	6013      	str	r3, [r2, #0]
3000a382:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
3000a386:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
3000a38a:	3401      	adds	r4, #1
3000a38c:	f807 3f01 	strb.w	r3, [r7, #1]!
3000a390:	e7eb      	b.n	3000a36a <NAND_RxData+0xd2>
3000a392:	f1c4 0404 	rsb	r4, r4, #4
3000a396:	2200      	movs	r2, #0
3000a398:	4639      	mov	r1, r7
3000a39a:	e00a      	b.n	3000a3b2 <NAND_RxData+0x11a>
3000a39c:	6a85      	ldr	r5, [r0, #40]	; 0x28
3000a39e:	072d      	lsls	r5, r5, #28
3000a3a0:	d505      	bpl.n	3000a3ae <NAND_RxData+0x116>
3000a3a2:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
3000a3a6:	3201      	adds	r2, #1
3000a3a8:	700b      	strb	r3, [r1, #0]
3000a3aa:	18b9      	adds	r1, r7, r2
3000a3ac:	460b      	mov	r3, r1
3000a3ae:	4294      	cmp	r4, r2
3000a3b0:	d0b8      	beq.n	3000a324 <NAND_RxData+0x8c>
3000a3b2:	42b2      	cmp	r2, r6
3000a3b4:	460b      	mov	r3, r1
3000a3b6:	d3f1      	bcc.n	3000a39c <NAND_RxData+0x104>
3000a3b8:	4614      	mov	r4, r2
3000a3ba:	e7ce      	b.n	3000a35a <NAND_RxData+0xc2>
3000a3bc:	f8df 9008 	ldr.w	r9, [pc, #8]	; 3000a3c8 <NAND_RxData+0x130>
3000a3c0:	f899 5008 	ldrb.w	r5, [r9, #8]
3000a3c4:	e773      	b.n	3000a2ae <NAND_RxData+0x16>
3000a3c6:	bf00      	nop
3000a3c8:	2001c01c 	.word	0x2001c01c
3000a3cc:	0000b9c1 	.word	0x0000b9c1

3000a3d0 <NAND_Page_Read>:
3000a3d0:	b530      	push	{r4, r5, lr}
3000a3d2:	b085      	sub	sp, #20
3000a3d4:	4c07      	ldr	r4, [pc, #28]	; (3000a3f4 <NAND_Page_Read+0x24>)
3000a3d6:	4d08      	ldr	r5, [pc, #32]	; (3000a3f8 <NAND_Page_Read+0x28>)
3000a3d8:	9301      	str	r3, [sp, #4]
3000a3da:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000a3de:	47a0      	blx	r4
3000a3e0:	4604      	mov	r4, r0
3000a3e2:	9903      	ldr	r1, [sp, #12]
3000a3e4:	7a28      	ldrb	r0, [r5, #8]
3000a3e6:	4d05      	ldr	r5, [pc, #20]	; (3000a3fc <NAND_Page_Read+0x2c>)
3000a3e8:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
3000a3ec:	47a8      	blx	r5
3000a3ee:	4620      	mov	r0, r4
3000a3f0:	b005      	add	sp, #20
3000a3f2:	bd30      	pop	{r4, r5, pc}
3000a3f4:	0000bd85 	.word	0x0000bd85
3000a3f8:	2001c01c 	.word	0x2001c01c
3000a3fc:	3000a299 	.word	0x3000a299

3000a400 <irq_register>:
3000a400:	b538      	push	{r3, r4, r5, lr}
3000a402:	f040 0001 	orr.w	r0, r0, #1
3000a406:	461c      	mov	r4, r3
3000a408:	4b14      	ldr	r3, [pc, #80]	; (3000a45c <irq_register+0x5c>)
3000a40a:	460d      	mov	r5, r1
3000a40c:	2c07      	cmp	r4, #7
3000a40e:	f843 0021 	str.w	r0, [r3, r1, lsl #2]
3000a412:	4b13      	ldr	r3, [pc, #76]	; (3000a460 <irq_register+0x60>)
3000a414:	f843 2021 	str.w	r2, [r3, r1, lsl #2]
3000a418:	d908      	bls.n	3000a42c <irq_register+0x2c>
3000a41a:	4b12      	ldr	r3, [pc, #72]	; (3000a464 <irq_register+0x64>)
3000a41c:	681b      	ldr	r3, [r3, #0]
3000a41e:	039b      	lsls	r3, r3, #14
3000a420:	d416      	bmi.n	3000a450 <irq_register+0x50>
3000a422:	f44f 7144 	mov.w	r1, #784	; 0x310
3000a426:	4810      	ldr	r0, [pc, #64]	; (3000a468 <irq_register+0x68>)
3000a428:	f001 fea2 	bl	3000c170 <__io_assert_failed_veneer>
3000a42c:	2c07      	cmp	r4, #7
3000a42e:	bf28      	it	cs
3000a430:	2407      	movcs	r4, #7
3000a432:	2d00      	cmp	r5, #0
3000a434:	ea4f 1444 	mov.w	r4, r4, lsl #5
3000a438:	b2e4      	uxtb	r4, r4
3000a43a:	db03      	blt.n	3000a444 <irq_register+0x44>
3000a43c:	4b0b      	ldr	r3, [pc, #44]	; (3000a46c <irq_register+0x6c>)
3000a43e:	2001      	movs	r0, #1
3000a440:	555c      	strb	r4, [r3, r5]
3000a442:	bd38      	pop	{r3, r4, r5, pc}
3000a444:	f005 050f 	and.w	r5, r5, #15
3000a448:	4b09      	ldr	r3, [pc, #36]	; (3000a470 <irq_register+0x70>)
3000a44a:	2001      	movs	r0, #1
3000a44c:	555c      	strb	r4, [r3, r5]
3000a44e:	bd38      	pop	{r3, r4, r5, pc}
3000a450:	2307      	movs	r3, #7
3000a452:	4622      	mov	r2, r4
3000a454:	4807      	ldr	r0, [pc, #28]	; (3000a474 <irq_register+0x74>)
3000a456:	f001 fe6b 	bl	3000c130 <__DiagPrintf_veneer>
3000a45a:	e7e2      	b.n	3000a422 <irq_register+0x22>
3000a45c:	30000184 	.word	0x30000184
3000a460:	300002c4 	.word	0x300002c4
3000a464:	2001c00c 	.word	0x2001c00c
3000a468:	3000d51c 	.word	0x3000d51c
3000a46c:	e000e400 	.word	0xe000e400
3000a470:	e000ed14 	.word	0xe000ed14
3000a474:	3000d4f8 	.word	0x3000d4f8

3000a478 <SYSCFG_BootFromNor>:
3000a478:	4b0e      	ldr	r3, [pc, #56]	; (3000a4b4 <SYSCFG_BootFromNor+0x3c>)
3000a47a:	b510      	push	{r4, lr}
3000a47c:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
3000a480:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
3000a484:	0392      	lsls	r2, r2, #14
3000a486:	d50a      	bpl.n	3000a49e <SYSCFG_BootFromNor+0x26>
3000a488:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
3000a48c:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
3000a490:	d105      	bne.n	3000a49e <SYSCFG_BootFromNor+0x26>
3000a492:	4b09      	ldr	r3, [pc, #36]	; (3000a4b8 <SYSCFG_BootFromNor+0x40>)
3000a494:	4798      	blx	r3
3000a496:	3800      	subs	r0, #0
3000a498:	bf18      	it	ne
3000a49a:	2001      	movne	r0, #1
3000a49c:	bd10      	pop	{r4, pc}
3000a49e:	4c07      	ldr	r4, [pc, #28]	; (3000a4bc <SYSCFG_BootFromNor+0x44>)
3000a4a0:	47a0      	blx	r4
3000a4a2:	2802      	cmp	r0, #2
3000a4a4:	d004      	beq.n	3000a4b0 <SYSCFG_BootFromNor+0x38>
3000a4a6:	47a0      	blx	r4
3000a4a8:	2801      	cmp	r0, #1
3000a4aa:	d1f2      	bne.n	3000a492 <SYSCFG_BootFromNor+0x1a>
3000a4ac:	2000      	movs	r0, #0
3000a4ae:	bd10      	pop	{r4, pc}
3000a4b0:	2001      	movs	r0, #1
3000a4b2:	bd10      	pop	{r4, pc}
3000a4b4:	42008000 	.word	0x42008000
3000a4b8:	0000c0bd 	.word	0x0000c0bd
3000a4bc:	0000c1a9 	.word	0x0000c1a9

3000a4c0 <SYSCFG_RLVersion>:
3000a4c0:	4b07      	ldr	r3, [pc, #28]	; (3000a4e0 <SYSCFG_RLVersion+0x20>)
3000a4c2:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a4c6:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a4ca:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
3000a4ce:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
3000a4d2:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a4d6:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a4da:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a4de:	4770      	bx	lr
3000a4e0:	42008000 	.word	0x42008000

3000a4e4 <ddr_init_index>:
3000a4e4:	b570      	push	{r4, r5, r6, lr}
3000a4e6:	4b0f      	ldr	r3, [pc, #60]	; (3000a524 <ddr_init_index+0x40>)
3000a4e8:	4798      	blx	r3
3000a4ea:	4604      	mov	r4, r0
3000a4ec:	4b0e      	ldr	r3, [pc, #56]	; (3000a528 <ddr_init_index+0x44>)
3000a4ee:	4798      	blx	r3
3000a4f0:	2c03      	cmp	r4, #3
3000a4f2:	d015      	beq.n	3000a520 <ddr_init_index+0x3c>
3000a4f4:	2c01      	cmp	r4, #1
3000a4f6:	d00d      	beq.n	3000a514 <ddr_init_index+0x30>
3000a4f8:	4d0c      	ldr	r5, [pc, #48]	; (3000a52c <ddr_init_index+0x48>)
3000a4fa:	4c0d      	ldr	r4, [pc, #52]	; (3000a530 <ddr_init_index+0x4c>)
3000a4fc:	4e0d      	ldr	r6, [pc, #52]	; (3000a534 <ddr_init_index+0x50>)
3000a4fe:	e002      	b.n	3000a506 <ddr_init_index+0x22>
3000a500:	f242 7010 	movw	r0, #10000	; 0x2710
3000a504:	47a0      	blx	r4
3000a506:	682b      	ldr	r3, [r5, #0]
3000a508:	039b      	lsls	r3, r3, #14
3000a50a:	d5f9      	bpl.n	3000a500 <ddr_init_index+0x1c>
3000a50c:	4630      	mov	r0, r6
3000a50e:	f001 fe0f 	bl	3000c130 <__DiagPrintf_veneer>
3000a512:	e7f5      	b.n	3000a500 <ddr_init_index+0x1c>
3000a514:	2802      	cmp	r0, #2
3000a516:	d003      	beq.n	3000a520 <ddr_init_index+0x3c>
3000a518:	2803      	cmp	r0, #3
3000a51a:	d1ed      	bne.n	3000a4f8 <ddr_init_index+0x14>
3000a51c:	2000      	movs	r0, #0
3000a51e:	bd70      	pop	{r4, r5, r6, pc}
3000a520:	4620      	mov	r0, r4
3000a522:	bd70      	pop	{r4, r5, r6, pc}
3000a524:	30007fbd 	.word	0x30007fbd
3000a528:	30007edd 	.word	0x30007edd
3000a52c:	2001c00c 	.word	0x2001c00c
3000a530:	00009be5 	.word	0x00009be5
3000a534:	3000d578 	.word	0x3000d578

3000a538 <rxi316_perf_tune>:
3000a538:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a53c:	f8d0 e000 	ldr.w	lr, [r0]
3000a540:	b0bf      	sub	sp, #252	; 0xfc
3000a542:	4606      	mov	r6, r0
3000a544:	f89e 5006 	ldrb.w	r5, [lr, #6]
3000a548:	2d00      	cmp	r5, #0
3000a54a:	f000 833e 	beq.w	3000abca <rxi316_perf_tune+0x692>
3000a54e:	2d01      	cmp	r5, #1
3000a550:	d003      	beq.n	3000a55a <rxi316_perf_tune+0x22>
3000a552:	2d03      	cmp	r5, #3
3000a554:	bf14      	ite	ne
3000a556:	2502      	movne	r5, #2
3000a558:	2500      	moveq	r5, #0
3000a55a:	68f3      	ldr	r3, [r6, #12]
3000a55c:	2420      	movs	r4, #32
3000a55e:	2100      	movs	r1, #0
3000a560:	685b      	ldr	r3, [r3, #4]
3000a562:	4627      	mov	r7, r4
3000a564:	46a0      	mov	r8, r4
3000a566:	46a1      	mov	r9, r4
3000a568:	785a      	ldrb	r2, [r3, #1]
3000a56a:	46a2      	mov	sl, r4
3000a56c:	7818      	ldrb	r0, [r3, #0]
3000a56e:	46a3      	mov	fp, r4
3000a570:	921f      	str	r2, [sp, #124]	; 0x7c
3000a572:	468c      	mov	ip, r1
3000a574:	789a      	ldrb	r2, [r3, #2]
3000a576:	9220      	str	r2, [sp, #128]	; 0x80
3000a578:	78da      	ldrb	r2, [r3, #3]
3000a57a:	9221      	str	r2, [sp, #132]	; 0x84
3000a57c:	791a      	ldrb	r2, [r3, #4]
3000a57e:	9222      	str	r2, [sp, #136]	; 0x88
3000a580:	795a      	ldrb	r2, [r3, #5]
3000a582:	9223      	str	r2, [sp, #140]	; 0x8c
3000a584:	799a      	ldrb	r2, [r3, #6]
3000a586:	9224      	str	r2, [sp, #144]	; 0x90
3000a588:	79da      	ldrb	r2, [r3, #7]
3000a58a:	9225      	str	r2, [sp, #148]	; 0x94
3000a58c:	7a1a      	ldrb	r2, [r3, #8]
3000a58e:	9226      	str	r2, [sp, #152]	; 0x98
3000a590:	7a5a      	ldrb	r2, [r3, #9]
3000a592:	9227      	str	r2, [sp, #156]	; 0x9c
3000a594:	7a9a      	ldrb	r2, [r3, #10]
3000a596:	9228      	str	r2, [sp, #160]	; 0xa0
3000a598:	7ada      	ldrb	r2, [r3, #11]
3000a59a:	9229      	str	r2, [sp, #164]	; 0xa4
3000a59c:	7b1a      	ldrb	r2, [r3, #12]
3000a59e:	922a      	str	r2, [sp, #168]	; 0xa8
3000a5a0:	7b5a      	ldrb	r2, [r3, #13]
3000a5a2:	922b      	str	r2, [sp, #172]	; 0xac
3000a5a4:	7b9a      	ldrb	r2, [r3, #14]
3000a5a6:	922c      	str	r2, [sp, #176]	; 0xb0
3000a5a8:	7bda      	ldrb	r2, [r3, #15]
3000a5aa:	922d      	str	r2, [sp, #180]	; 0xb4
3000a5ac:	7c1a      	ldrb	r2, [r3, #16]
3000a5ae:	941a      	str	r4, [sp, #104]	; 0x68
3000a5b0:	922e      	str	r2, [sp, #184]	; 0xb8
3000a5b2:	7c5a      	ldrb	r2, [r3, #17]
3000a5b4:	922f      	str	r2, [sp, #188]	; 0xbc
3000a5b6:	7c9a      	ldrb	r2, [r3, #18]
3000a5b8:	9230      	str	r2, [sp, #192]	; 0xc0
3000a5ba:	7cda      	ldrb	r2, [r3, #19]
3000a5bc:	9231      	str	r2, [sp, #196]	; 0xc4
3000a5be:	7d1a      	ldrb	r2, [r3, #20]
3000a5c0:	9232      	str	r2, [sp, #200]	; 0xc8
3000a5c2:	7d5a      	ldrb	r2, [r3, #21]
3000a5c4:	9233      	str	r2, [sp, #204]	; 0xcc
3000a5c6:	7d9a      	ldrb	r2, [r3, #22]
3000a5c8:	9234      	str	r2, [sp, #208]	; 0xd0
3000a5ca:	7dda      	ldrb	r2, [r3, #23]
3000a5cc:	9235      	str	r2, [sp, #212]	; 0xd4
3000a5ce:	7e1a      	ldrb	r2, [r3, #24]
3000a5d0:	9236      	str	r2, [sp, #216]	; 0xd8
3000a5d2:	7e5a      	ldrb	r2, [r3, #25]
3000a5d4:	9237      	str	r2, [sp, #220]	; 0xdc
3000a5d6:	7e9a      	ldrb	r2, [r3, #26]
3000a5d8:	9238      	str	r2, [sp, #224]	; 0xe0
3000a5da:	7eda      	ldrb	r2, [r3, #27]
3000a5dc:	9239      	str	r2, [sp, #228]	; 0xe4
3000a5de:	7f1a      	ldrb	r2, [r3, #28]
3000a5e0:	923a      	str	r2, [sp, #232]	; 0xe8
3000a5e2:	7f5a      	ldrb	r2, [r3, #29]
3000a5e4:	923b      	str	r2, [sp, #236]	; 0xec
3000a5e6:	7f9a      	ldrb	r2, [r3, #30]
3000a5e8:	7fdb      	ldrb	r3, [r3, #31]
3000a5ea:	923c      	str	r2, [sp, #240]	; 0xf0
3000a5ec:	460a      	mov	r2, r1
3000a5ee:	933d      	str	r3, [sp, #244]	; 0xf4
3000a5f0:	4623      	mov	r3, r4
3000a5f2:	9406      	str	r4, [sp, #24]
3000a5f4:	9419      	str	r4, [sp, #100]	; 0x64
3000a5f6:	9418      	str	r4, [sp, #96]	; 0x60
3000a5f8:	9407      	str	r4, [sp, #28]
3000a5fa:	951b      	str	r5, [sp, #108]	; 0x6c
3000a5fc:	e9cd 4402 	strd	r4, r4, [sp, #8]
3000a600:	e9cd 4400 	strd	r4, r4, [sp]
3000a604:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a608:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a60c:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a610:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a614:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a618:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a61c:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a620:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a624:	e9cd 4408 	strd	r4, r4, [sp, #32]
3000a628:	b950      	cbnz	r0, 3000a640 <rxi316_perf_tune+0x108>
3000a62a:	f10c 0c01 	add.w	ip, ip, #1
3000a62e:	4613      	mov	r3, r2
3000a630:	3201      	adds	r2, #1
3000a632:	2a20      	cmp	r2, #32
3000a634:	d06f      	beq.n	3000a716 <rxi316_perf_tune+0x1de>
3000a636:	a81e      	add	r0, sp, #120	; 0x78
3000a638:	f850 0022 	ldr.w	r0, [r0, r2, lsl #2]
3000a63c:	2800      	cmp	r0, #0
3000a63e:	d0f4      	beq.n	3000a62a <rxi316_perf_tune+0xf2>
3000a640:	2801      	cmp	r0, #1
3000a642:	d062      	beq.n	3000a70a <rxi316_perf_tune+0x1d2>
3000a644:	2802      	cmp	r0, #2
3000a646:	f000 8083 	beq.w	3000a750 <rxi316_perf_tune+0x218>
3000a64a:	2803      	cmp	r0, #3
3000a64c:	f000 8084 	beq.w	3000a758 <rxi316_perf_tune+0x220>
3000a650:	2804      	cmp	r0, #4
3000a652:	f000 8085 	beq.w	3000a760 <rxi316_perf_tune+0x228>
3000a656:	2805      	cmp	r0, #5
3000a658:	f000 8086 	beq.w	3000a768 <rxi316_perf_tune+0x230>
3000a65c:	2806      	cmp	r0, #6
3000a65e:	f000 8087 	beq.w	3000a770 <rxi316_perf_tune+0x238>
3000a662:	2807      	cmp	r0, #7
3000a664:	f000 82b3 	beq.w	3000abce <rxi316_perf_tune+0x696>
3000a668:	2808      	cmp	r0, #8
3000a66a:	f000 8375 	beq.w	3000ad58 <rxi316_perf_tune+0x820>
3000a66e:	2809      	cmp	r0, #9
3000a670:	f000 8376 	beq.w	3000ad60 <rxi316_perf_tune+0x828>
3000a674:	280a      	cmp	r0, #10
3000a676:	f000 837c 	beq.w	3000ad72 <rxi316_perf_tune+0x83a>
3000a67a:	280b      	cmp	r0, #11
3000a67c:	f000 8376 	beq.w	3000ad6c <rxi316_perf_tune+0x834>
3000a680:	280c      	cmp	r0, #12
3000a682:	f000 8383 	beq.w	3000ad8c <rxi316_perf_tune+0x854>
3000a686:	280d      	cmp	r0, #13
3000a688:	f000 8386 	beq.w	3000ad98 <rxi316_perf_tune+0x860>
3000a68c:	280e      	cmp	r0, #14
3000a68e:	f000 8380 	beq.w	3000ad92 <rxi316_perf_tune+0x85a>
3000a692:	280f      	cmp	r0, #15
3000a694:	f000 8383 	beq.w	3000ad9e <rxi316_perf_tune+0x866>
3000a698:	2810      	cmp	r0, #16
3000a69a:	f000 8383 	beq.w	3000ada4 <rxi316_perf_tune+0x86c>
3000a69e:	2811      	cmp	r0, #17
3000a6a0:	f000 8382 	beq.w	3000ada8 <rxi316_perf_tune+0x870>
3000a6a4:	2812      	cmp	r0, #18
3000a6a6:	f000 8381 	beq.w	3000adac <rxi316_perf_tune+0x874>
3000a6aa:	2813      	cmp	r0, #19
3000a6ac:	f000 8382 	beq.w	3000adb4 <rxi316_perf_tune+0x87c>
3000a6b0:	2814      	cmp	r0, #20
3000a6b2:	f000 837d 	beq.w	3000adb0 <rxi316_perf_tune+0x878>
3000a6b6:	2815      	cmp	r0, #21
3000a6b8:	f000 8384 	beq.w	3000adc4 <rxi316_perf_tune+0x88c>
3000a6bc:	2816      	cmp	r0, #22
3000a6be:	f000 837f 	beq.w	3000adc0 <rxi316_perf_tune+0x888>
3000a6c2:	2817      	cmp	r0, #23
3000a6c4:	f000 838e 	beq.w	3000ade4 <rxi316_perf_tune+0x8ac>
3000a6c8:	2818      	cmp	r0, #24
3000a6ca:	f000 8389 	beq.w	3000ade0 <rxi316_perf_tune+0x8a8>
3000a6ce:	2819      	cmp	r0, #25
3000a6d0:	f000 8384 	beq.w	3000addc <rxi316_perf_tune+0x8a4>
3000a6d4:	281a      	cmp	r0, #26
3000a6d6:	f000 837f 	beq.w	3000add8 <rxi316_perf_tune+0x8a0>
3000a6da:	281b      	cmp	r0, #27
3000a6dc:	f000 837a 	beq.w	3000add4 <rxi316_perf_tune+0x89c>
3000a6e0:	281c      	cmp	r0, #28
3000a6e2:	f000 8375 	beq.w	3000add0 <rxi316_perf_tune+0x898>
3000a6e6:	281d      	cmp	r0, #29
3000a6e8:	f000 8370 	beq.w	3000adcc <rxi316_perf_tune+0x894>
3000a6ec:	281e      	cmp	r0, #30
3000a6ee:	f000 836b 	beq.w	3000adc8 <rxi316_perf_tune+0x890>
3000a6f2:	281f      	cmp	r0, #31
3000a6f4:	f000 8362 	beq.w	3000adbc <rxi316_perf_tune+0x884>
3000a6f8:	2820      	cmp	r0, #32
3000a6fa:	f000 835d 	beq.w	3000adb8 <rxi316_perf_tune+0x880>
3000a6fe:	9d06      	ldr	r5, [sp, #24]
3000a700:	2821      	cmp	r0, #33	; 0x21
3000a702:	bf08      	it	eq
3000a704:	4615      	moveq	r5, r2
3000a706:	9506      	str	r5, [sp, #24]
3000a708:	e792      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a70a:	4693      	mov	fp, r2
3000a70c:	3201      	adds	r2, #1
3000a70e:	f10c 0c01 	add.w	ip, ip, #1
3000a712:	2a20      	cmp	r2, #32
3000a714:	d18f      	bne.n	3000a636 <rxi316_perf_tune+0xfe>
3000a716:	f89e 2002 	ldrb.w	r2, [lr, #2]
3000a71a:	9d1b      	ldr	r5, [sp, #108]	; 0x6c
3000a71c:	f8de 0008 	ldr.w	r0, [lr, #8]
3000a720:	b372      	cbz	r2, 3000a780 <rxi316_perf_tune+0x248>
3000a722:	2a01      	cmp	r2, #1
3000a724:	f000 8320 	beq.w	3000ad68 <rxi316_perf_tune+0x830>
3000a728:	2a03      	cmp	r2, #3
3000a72a:	f1a0 0002 	sub.w	r0, r0, #2
3000a72e:	bf0c      	ite	eq
3000a730:	2204      	moveq	r2, #4
3000a732:	2203      	movne	r2, #3
3000a734:	4560      	cmp	r0, ip
3000a736:	d127      	bne.n	3000a788 <rxi316_perf_tune+0x250>
3000a738:	4291      	cmp	r1, r2
3000a73a:	d02a      	beq.n	3000a792 <rxi316_perf_tune+0x25a>
3000a73c:	4a0e      	ldr	r2, [pc, #56]	; (3000a778 <rxi316_perf_tune+0x240>)
3000a73e:	6812      	ldr	r2, [r2, #0]
3000a740:	0392      	lsls	r2, r2, #14
3000a742:	d526      	bpl.n	3000a792 <rxi316_perf_tune+0x25a>
3000a744:	480d      	ldr	r0, [pc, #52]	; (3000a77c <rxi316_perf_tune+0x244>)
3000a746:	931b      	str	r3, [sp, #108]	; 0x6c
3000a748:	f001 fcf2 	bl	3000c130 <__DiagPrintf_veneer>
3000a74c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a74e:	e020      	b.n	3000a792 <rxi316_perf_tune+0x25a>
3000a750:	f10c 0c01 	add.w	ip, ip, #1
3000a754:	4692      	mov	sl, r2
3000a756:	e76b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a758:	f10c 0c01 	add.w	ip, ip, #1
3000a75c:	4691      	mov	r9, r2
3000a75e:	e767      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a760:	f10c 0c01 	add.w	ip, ip, #1
3000a764:	4690      	mov	r8, r2
3000a766:	e763      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a768:	f10c 0c01 	add.w	ip, ip, #1
3000a76c:	4617      	mov	r7, r2
3000a76e:	e75f      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a770:	f10c 0c01 	add.w	ip, ip, #1
3000a774:	9200      	str	r2, [sp, #0]
3000a776:	e75b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000a778:	2001c00c 	.word	0x2001c00c
3000a77c:	3000d5d8 	.word	0x3000d5d8
3000a780:	2201      	movs	r2, #1
3000a782:	3802      	subs	r0, #2
3000a784:	4560      	cmp	r0, ip
3000a786:	d0d7      	beq.n	3000a738 <rxi316_perf_tune+0x200>
3000a788:	48c4      	ldr	r0, [pc, #784]	; (3000aa9c <rxi316_perf_tune+0x564>)
3000a78a:	6800      	ldr	r0, [r0, #0]
3000a78c:	0380      	lsls	r0, r0, #14
3000a78e:	f100 82f3 	bmi.w	3000ad78 <rxi316_perf_tune+0x840>
3000a792:	9a07      	ldr	r2, [sp, #28]
3000a794:	2a20      	cmp	r2, #32
3000a796:	f000 82dc 	beq.w	3000ad52 <rxi316_perf_tune+0x81a>
3000a79a:	1b52      	subs	r2, r2, r5
3000a79c:	3a05      	subs	r2, #5
3000a79e:	f002 020f 	and.w	r2, r2, #15
3000a7a2:	9207      	str	r2, [sp, #28]
3000a7a4:	9a08      	ldr	r2, [sp, #32]
3000a7a6:	2a20      	cmp	r2, #32
3000a7a8:	f000 82d1 	beq.w	3000ad4e <rxi316_perf_tune+0x816>
3000a7ac:	1b52      	subs	r2, r2, r5
3000a7ae:	3a06      	subs	r2, #6
3000a7b0:	0112      	lsls	r2, r2, #4
3000a7b2:	b2d2      	uxtb	r2, r2
3000a7b4:	9909      	ldr	r1, [sp, #36]	; 0x24
3000a7b6:	2920      	cmp	r1, #32
3000a7b8:	f000 82c5 	beq.w	3000ad46 <rxi316_perf_tune+0x80e>
3000a7bc:	1b49      	subs	r1, r1, r5
3000a7be:	3907      	subs	r1, #7
3000a7c0:	0209      	lsls	r1, r1, #8
3000a7c2:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a7c6:	9108      	str	r1, [sp, #32]
3000a7c8:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a7ca:	2920      	cmp	r1, #32
3000a7cc:	f000 821a 	beq.w	3000ac04 <rxi316_perf_tune+0x6cc>
3000a7d0:	1b49      	subs	r1, r1, r5
3000a7d2:	3908      	subs	r1, #8
3000a7d4:	0309      	lsls	r1, r1, #12
3000a7d6:	b289      	uxth	r1, r1
3000a7d8:	9109      	str	r1, [sp, #36]	; 0x24
3000a7da:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a7dc:	2920      	cmp	r1, #32
3000a7de:	f000 8218 	beq.w	3000ac12 <rxi316_perf_tune+0x6da>
3000a7e2:	1b49      	subs	r1, r1, r5
3000a7e4:	3909      	subs	r1, #9
3000a7e6:	0409      	lsls	r1, r1, #16
3000a7e8:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a7ec:	910a      	str	r1, [sp, #40]	; 0x28
3000a7ee:	990c      	ldr	r1, [sp, #48]	; 0x30
3000a7f0:	2920      	cmp	r1, #32
3000a7f2:	f000 8215 	beq.w	3000ac20 <rxi316_perf_tune+0x6e8>
3000a7f6:	1b49      	subs	r1, r1, r5
3000a7f8:	390a      	subs	r1, #10
3000a7fa:	0509      	lsls	r1, r1, #20
3000a7fc:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a800:	910b      	str	r1, [sp, #44]	; 0x2c
3000a802:	990d      	ldr	r1, [sp, #52]	; 0x34
3000a804:	2920      	cmp	r1, #32
3000a806:	f000 8212 	beq.w	3000ac2e <rxi316_perf_tune+0x6f6>
3000a80a:	1b49      	subs	r1, r1, r5
3000a80c:	390b      	subs	r1, #11
3000a80e:	0609      	lsls	r1, r1, #24
3000a810:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a814:	910c      	str	r1, [sp, #48]	; 0x30
3000a816:	990e      	ldr	r1, [sp, #56]	; 0x38
3000a818:	2920      	cmp	r1, #32
3000a81a:	f000 820f 	beq.w	3000ac3c <rxi316_perf_tune+0x704>
3000a81e:	1b49      	subs	r1, r1, r5
3000a820:	390c      	subs	r1, #12
3000a822:	0709      	lsls	r1, r1, #28
3000a824:	910d      	str	r1, [sp, #52]	; 0x34
3000a826:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000a828:	2920      	cmp	r1, #32
3000a82a:	f000 820e 	beq.w	3000ac4a <rxi316_perf_tune+0x712>
3000a82e:	1b49      	subs	r1, r1, r5
3000a830:	390d      	subs	r1, #13
3000a832:	f001 010f 	and.w	r1, r1, #15
3000a836:	910e      	str	r1, [sp, #56]	; 0x38
3000a838:	9910      	ldr	r1, [sp, #64]	; 0x40
3000a83a:	2920      	cmp	r1, #32
3000a83c:	f000 820b 	beq.w	3000ac56 <rxi316_perf_tune+0x71e>
3000a840:	1b49      	subs	r1, r1, r5
3000a842:	390e      	subs	r1, #14
3000a844:	0109      	lsls	r1, r1, #4
3000a846:	b2c9      	uxtb	r1, r1
3000a848:	910f      	str	r1, [sp, #60]	; 0x3c
3000a84a:	9911      	ldr	r1, [sp, #68]	; 0x44
3000a84c:	2920      	cmp	r1, #32
3000a84e:	f000 8208 	beq.w	3000ac62 <rxi316_perf_tune+0x72a>
3000a852:	1b49      	subs	r1, r1, r5
3000a854:	390f      	subs	r1, #15
3000a856:	0209      	lsls	r1, r1, #8
3000a858:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a85c:	9110      	str	r1, [sp, #64]	; 0x40
3000a85e:	9912      	ldr	r1, [sp, #72]	; 0x48
3000a860:	2920      	cmp	r1, #32
3000a862:	f000 8205 	beq.w	3000ac70 <rxi316_perf_tune+0x738>
3000a866:	1b49      	subs	r1, r1, r5
3000a868:	0309      	lsls	r1, r1, #12
3000a86a:	f401 4170 	and.w	r1, r1, #61440	; 0xf000
3000a86e:	9111      	str	r1, [sp, #68]	; 0x44
3000a870:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000a872:	2920      	cmp	r1, #32
3000a874:	f000 8203 	beq.w	3000ac7e <rxi316_perf_tune+0x746>
3000a878:	1b49      	subs	r1, r1, r5
3000a87a:	3911      	subs	r1, #17
3000a87c:	0409      	lsls	r1, r1, #16
3000a87e:	f401 2170 	and.w	r1, r1, #983040	; 0xf0000
3000a882:	9112      	str	r1, [sp, #72]	; 0x48
3000a884:	9914      	ldr	r1, [sp, #80]	; 0x50
3000a886:	2920      	cmp	r1, #32
3000a888:	f000 8200 	beq.w	3000ac8c <rxi316_perf_tune+0x754>
3000a88c:	1b49      	subs	r1, r1, r5
3000a88e:	3912      	subs	r1, #18
3000a890:	0509      	lsls	r1, r1, #20
3000a892:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000a896:	9113      	str	r1, [sp, #76]	; 0x4c
3000a898:	9915      	ldr	r1, [sp, #84]	; 0x54
3000a89a:	2920      	cmp	r1, #32
3000a89c:	f000 81fd 	beq.w	3000ac9a <rxi316_perf_tune+0x762>
3000a8a0:	1b49      	subs	r1, r1, r5
3000a8a2:	3913      	subs	r1, #19
3000a8a4:	0609      	lsls	r1, r1, #24
3000a8a6:	f001 6170 	and.w	r1, r1, #251658240	; 0xf000000
3000a8aa:	911d      	str	r1, [sp, #116]	; 0x74
3000a8ac:	9916      	ldr	r1, [sp, #88]	; 0x58
3000a8ae:	2920      	cmp	r1, #32
3000a8b0:	f000 81fa 	beq.w	3000aca8 <rxi316_perf_tune+0x770>
3000a8b4:	1b49      	subs	r1, r1, r5
3000a8b6:	3914      	subs	r1, #20
3000a8b8:	0709      	lsls	r1, r1, #28
3000a8ba:	911c      	str	r1, [sp, #112]	; 0x70
3000a8bc:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000a8be:	2920      	cmp	r1, #32
3000a8c0:	f000 81f9 	beq.w	3000acb6 <rxi316_perf_tune+0x77e>
3000a8c4:	1b49      	subs	r1, r1, r5
3000a8c6:	3915      	subs	r1, #21
3000a8c8:	f001 010f 	and.w	r1, r1, #15
3000a8cc:	911b      	str	r1, [sp, #108]	; 0x6c
3000a8ce:	9904      	ldr	r1, [sp, #16]
3000a8d0:	2920      	cmp	r1, #32
3000a8d2:	f000 81f6 	beq.w	3000acc2 <rxi316_perf_tune+0x78a>
3000a8d6:	1b49      	subs	r1, r1, r5
3000a8d8:	3902      	subs	r1, #2
3000a8da:	f001 011f 	and.w	r1, r1, #31
3000a8de:	9117      	str	r1, [sp, #92]	; 0x5c
3000a8e0:	9905      	ldr	r1, [sp, #20]
3000a8e2:	2920      	cmp	r1, #32
3000a8e4:	f000 81f3 	beq.w	3000acce <rxi316_perf_tune+0x796>
3000a8e8:	1b49      	subs	r1, r1, r5
3000a8ea:	3902      	subs	r1, #2
3000a8ec:	0149      	lsls	r1, r1, #5
3000a8ee:	f401 7178 	and.w	r1, r1, #992	; 0x3e0
3000a8f2:	9116      	str	r1, [sp, #88]	; 0x58
3000a8f4:	9918      	ldr	r1, [sp, #96]	; 0x60
3000a8f6:	2920      	cmp	r1, #32
3000a8f8:	f000 81f0 	beq.w	3000acdc <rxi316_perf_tune+0x7a4>
3000a8fc:	1b49      	subs	r1, r1, r5
3000a8fe:	2b20      	cmp	r3, #32
3000a900:	f1a1 0102 	sub.w	r1, r1, #2
3000a904:	ea4f 2181 	mov.w	r1, r1, lsl #10
3000a908:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000a90c:	9115      	str	r1, [sp, #84]	; 0x54
3000a90e:	f000 81eb 	beq.w	3000ace8 <rxi316_perf_tune+0x7b0>
3000a912:	1b59      	subs	r1, r3, r5
3000a914:	f1bb 0f20 	cmp.w	fp, #32
3000a918:	f1a1 0102 	sub.w	r1, r1, #2
3000a91c:	f001 030f 	and.w	r3, r1, #15
3000a920:	9314      	str	r3, [sp, #80]	; 0x50
3000a922:	f000 81e7 	beq.w	3000acf4 <rxi316_perf_tune+0x7bc>
3000a926:	ebab 0105 	sub.w	r1, fp, r5
3000a92a:	f1ba 0f20 	cmp.w	sl, #32
3000a92e:	f1a1 0103 	sub.w	r1, r1, #3
3000a932:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a936:	b2c9      	uxtb	r1, r1
3000a938:	f000 81e2 	beq.w	3000ad00 <rxi316_perf_tune+0x7c8>
3000a93c:	ebaa 0305 	sub.w	r3, sl, r5
3000a940:	f1b9 0f20 	cmp.w	r9, #32
3000a944:	f1a3 0304 	sub.w	r3, r3, #4
3000a948:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a94c:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a950:	9305      	str	r3, [sp, #20]
3000a952:	f000 81dc 	beq.w	3000ad0e <rxi316_perf_tune+0x7d6>
3000a956:	eba9 0305 	sub.w	r3, r9, r5
3000a95a:	f1b8 0f20 	cmp.w	r8, #32
3000a95e:	f1a3 0305 	sub.w	r3, r3, #5
3000a962:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a966:	b29b      	uxth	r3, r3
3000a968:	9304      	str	r3, [sp, #16]
3000a96a:	f000 81d7 	beq.w	3000ad1c <rxi316_perf_tune+0x7e4>
3000a96e:	eba8 0c05 	sub.w	ip, r8, r5
3000a972:	2f20      	cmp	r7, #32
3000a974:	f1ac 0c06 	sub.w	ip, ip, #6
3000a978:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a97c:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a980:	f000 81d1 	beq.w	3000ad26 <rxi316_perf_tune+0x7ee>
3000a984:	eba7 0e05 	sub.w	lr, r7, r5
3000a988:	9b00      	ldr	r3, [sp, #0]
3000a98a:	f1ae 0e07 	sub.w	lr, lr, #7
3000a98e:	2b20      	cmp	r3, #32
3000a990:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a994:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a998:	f000 81cb 	beq.w	3000ad32 <rxi316_perf_tune+0x7fa>
3000a99c:	1b5f      	subs	r7, r3, r5
3000a99e:	9b01      	ldr	r3, [sp, #4]
3000a9a0:	3f08      	subs	r7, #8
3000a9a2:	2b20      	cmp	r3, #32
3000a9a4:	ea4f 6707 	mov.w	r7, r7, lsl #24
3000a9a8:	f007 6770 	and.w	r7, r7, #251658240	; 0xf000000
3000a9ac:	f000 81c7 	beq.w	3000ad3e <rxi316_perf_tune+0x806>
3000a9b0:	1b5b      	subs	r3, r3, r5
3000a9b2:	3b09      	subs	r3, #9
3000a9b4:	071b      	lsls	r3, r3, #28
3000a9b6:	9301      	str	r3, [sp, #4]
3000a9b8:	9b02      	ldr	r3, [sp, #8]
3000a9ba:	2b20      	cmp	r3, #32
3000a9bc:	f000 811f 	beq.w	3000abfe <rxi316_perf_tune+0x6c6>
3000a9c0:	eba3 0805 	sub.w	r8, r3, r5
3000a9c4:	f1a8 080a 	sub.w	r8, r8, #10
3000a9c8:	f008 080f 	and.w	r8, r8, #15
3000a9cc:	9b03      	ldr	r3, [sp, #12]
3000a9ce:	2b20      	cmp	r3, #32
3000a9d0:	f000 8113 	beq.w	3000abfa <rxi316_perf_tune+0x6c2>
3000a9d4:	1b58      	subs	r0, r3, r5
3000a9d6:	380b      	subs	r0, #11
3000a9d8:	0100      	lsls	r0, r0, #4
3000a9da:	b2c0      	uxtb	r0, r0
3000a9dc:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a9de:	2b20      	cmp	r3, #32
3000a9e0:	f000 8108 	beq.w	3000abf4 <rxi316_perf_tune+0x6bc>
3000a9e4:	eba3 0a05 	sub.w	sl, r3, r5
3000a9e8:	f1aa 0a02 	sub.w	sl, sl, #2
3000a9ec:	f00a 0a1f 	and.w	sl, sl, #31
3000a9f0:	9b06      	ldr	r3, [sp, #24]
3000a9f2:	2b20      	cmp	r3, #32
3000a9f4:	f000 80ef 	beq.w	3000abd6 <rxi316_perf_tune+0x69e>
3000a9f8:	eba3 0905 	sub.w	r9, r3, r5
3000a9fc:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a9fe:	f1a9 0902 	sub.w	r9, r9, #2
3000aa02:	2b20      	cmp	r3, #32
3000aa04:	f009 091f 	and.w	r9, r9, #31
3000aa08:	f000 80eb 	beq.w	3000abe2 <rxi316_perf_tune+0x6aa>
3000aa0c:	eba3 0b05 	sub.w	fp, r3, r5
3000aa10:	2c20      	cmp	r4, #32
3000aa12:	f1ab 0b02 	sub.w	fp, fp, #2
3000aa16:	f00b 0b1f 	and.w	fp, fp, #31
3000aa1a:	f000 80e7 	beq.w	3000abec <rxi316_perf_tune+0x6b4>
3000aa1e:	1b63      	subs	r3, r4, r5
3000aa20:	3b02      	subs	r3, #2
3000aa22:	015b      	lsls	r3, r3, #5
3000aa24:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000aa28:	9300      	str	r3, [sp, #0]
3000aa2a:	9c07      	ldr	r4, [sp, #28]
3000aa2c:	ea40 0008 	orr.w	r0, r0, r8
3000aa30:	4314      	orrs	r4, r2
3000aa32:	4622      	mov	r2, r4
3000aa34:	e9dd 450e 	ldrd	r4, r5, [sp, #56]	; 0x38
3000aa38:	432c      	orrs	r4, r5
3000aa3a:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000aa3c:	4329      	orrs	r1, r5
3000aa3e:	e9dd 5316 	ldrd	r5, r3, [sp, #88]	; 0x58
3000aa42:	431d      	orrs	r5, r3
3000aa44:	9b08      	ldr	r3, [sp, #32]
3000aa46:	431a      	orrs	r2, r3
3000aa48:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000aa4a:	431c      	orrs	r4, r3
3000aa4c:	9b05      	ldr	r3, [sp, #20]
3000aa4e:	4319      	orrs	r1, r3
3000aa50:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000aa52:	431d      	orrs	r5, r3
3000aa54:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000aa56:	431a      	orrs	r2, r3
3000aa58:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000aa5a:	431c      	orrs	r4, r3
3000aa5c:	9b04      	ldr	r3, [sp, #16]
3000aa5e:	4319      	orrs	r1, r3
3000aa60:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000aa62:	ea41 0c0c 	orr.w	ip, r1, ip
3000aa66:	431a      	orrs	r2, r3
3000aa68:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000aa6a:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000aa6c:	ea4c 0c0e 	orr.w	ip, ip, lr
3000aa70:	430a      	orrs	r2, r1
3000aa72:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000aa74:	431c      	orrs	r4, r3
3000aa76:	ea4c 0c07 	orr.w	ip, ip, r7
3000aa7a:	4f09      	ldr	r7, [pc, #36]	; (3000aaa0 <rxi316_perf_tune+0x568>)
3000aa7c:	430c      	orrs	r4, r1
3000aa7e:	990c      	ldr	r1, [sp, #48]	; 0x30
3000aa80:	9b00      	ldr	r3, [sp, #0]
3000aa82:	430a      	orrs	r2, r1
3000aa84:	991d      	ldr	r1, [sp, #116]	; 0x74
3000aa86:	ea43 030b 	orr.w	r3, r3, fp
3000aa8a:	430c      	orrs	r4, r1
3000aa8c:	990d      	ldr	r1, [sp, #52]	; 0x34
3000aa8e:	430a      	orrs	r2, r1
3000aa90:	991c      	ldr	r1, [sp, #112]	; 0x70
3000aa92:	4321      	orrs	r1, r4
3000aa94:	9c01      	ldr	r4, [sp, #4]
3000aa96:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000aa9a:	e003      	b.n	3000aaa4 <rxi316_perf_tune+0x56c>
3000aa9c:	2001c00c 	.word	0x2001c00c
3000aaa0:	41100000 	.word	0x41100000
3000aaa4:	f8c7 1404 	str.w	r1, [r7, #1028]	; 0x404
3000aaa8:	ea4c 0404 	orr.w	r4, ip, r4
3000aaac:	9a1b      	ldr	r2, [sp, #108]	; 0x6c
3000aaae:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000aab2:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000aab6:	f8c7 4410 	str.w	r4, [r7, #1040]	; 0x410
3000aaba:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000aabe:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000aac2:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000aac6:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000aaca:	68f3      	ldr	r3, [r6, #12]
3000aacc:	689a      	ldr	r2, [r3, #8]
3000aace:	e9d2 3200 	ldrd	r3, r2, [r2]
3000aad2:	f003 0301 	and.w	r3, r3, #1
3000aad6:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000aada:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000aade:	68f3      	ldr	r3, [r6, #12]
3000aae0:	689a      	ldr	r2, [r3, #8]
3000aae2:	7a13      	ldrb	r3, [r2, #8]
3000aae4:	6951      	ldr	r1, [r2, #20]
3000aae6:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000aaea:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000aaee:	0209      	lsls	r1, r1, #8
3000aaf0:	0412      	lsls	r2, r2, #16
3000aaf2:	b289      	uxth	r1, r1
3000aaf4:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000aaf8:	430b      	orrs	r3, r1
3000aafa:	4313      	orrs	r3, r2
3000aafc:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000ab00:	68f3      	ldr	r3, [r6, #12]
3000ab02:	6899      	ldr	r1, [r3, #8]
3000ab04:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000ab08:	7e09      	ldrb	r1, [r1, #24]
3000ab0a:	021b      	lsls	r3, r3, #8
3000ab0c:	0412      	lsls	r2, r2, #16
3000ab0e:	b29b      	uxth	r3, r3
3000ab10:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000ab14:	4313      	orrs	r3, r2
3000ab16:	430b      	orrs	r3, r1
3000ab18:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000ab1c:	68f3      	ldr	r3, [r6, #12]
3000ab1e:	689a      	ldr	r2, [r3, #8]
3000ab20:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000ab24:	041b      	lsls	r3, r3, #16
3000ab26:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000ab2a:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000ab2e:	4313      	orrs	r3, r2
3000ab30:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000ab34:	68f3      	ldr	r3, [r6, #12]
3000ab36:	6899      	ldr	r1, [r3, #8]
3000ab38:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000ab3a:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000ab3c:	005b      	lsls	r3, r3, #1
3000ab3e:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000ab40:	0412      	lsls	r2, r2, #16
3000ab42:	f003 0302 	and.w	r3, r3, #2
3000ab46:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000ab4a:	4313      	orrs	r3, r2
3000ab4c:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000ab4e:	f000 0101 	and.w	r1, r0, #1
3000ab52:	0212      	lsls	r2, r2, #8
3000ab54:	430b      	orrs	r3, r1
3000ab56:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000ab5a:	4313      	orrs	r3, r2
3000ab5c:	4aa2      	ldr	r2, [pc, #648]	; (3000ade8 <rxi316_perf_tune+0x8b0>)
3000ab5e:	431a      	orrs	r2, r3
3000ab60:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000ab64:	68f3      	ldr	r3, [r6, #12]
3000ab66:	689b      	ldr	r3, [r3, #8]
3000ab68:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000ab6a:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000ab6e:	68f3      	ldr	r3, [r6, #12]
3000ab70:	689b      	ldr	r3, [r3, #8]
3000ab72:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000ab74:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000ab78:	68f3      	ldr	r3, [r6, #12]
3000ab7a:	689b      	ldr	r3, [r3, #8]
3000ab7c:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000ab7e:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000ab82:	68f3      	ldr	r3, [r6, #12]
3000ab84:	689b      	ldr	r3, [r3, #8]
3000ab86:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000ab88:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000ab8c:	68f3      	ldr	r3, [r6, #12]
3000ab8e:	689b      	ldr	r3, [r3, #8]
3000ab90:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000ab92:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000ab96:	68f3      	ldr	r3, [r6, #12]
3000ab98:	689b      	ldr	r3, [r3, #8]
3000ab9a:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000ab9c:	f003 030f 	and.w	r3, r3, #15
3000aba0:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000aba4:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000aba8:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000abac:	f043 0304 	orr.w	r3, r3, #4
3000abb0:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000abb4:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000abb8:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000abbc:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000abc0:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000abc4:	b03f      	add	sp, #252	; 0xfc
3000abc6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000abca:	2502      	movs	r5, #2
3000abcc:	e4c5      	b.n	3000a55a <rxi316_perf_tune+0x22>
3000abce:	f10c 0c01 	add.w	ip, ip, #1
3000abd2:	9201      	str	r2, [sp, #4]
3000abd4:	e52c      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000abd6:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000abd8:	f04f 091f 	mov.w	r9, #31
3000abdc:	2b20      	cmp	r3, #32
3000abde:	f47f af15 	bne.w	3000aa0c <rxi316_perf_tune+0x4d4>
3000abe2:	2c20      	cmp	r4, #32
3000abe4:	f04f 0b1f 	mov.w	fp, #31
3000abe8:	f47f af19 	bne.w	3000aa1e <rxi316_perf_tune+0x4e6>
3000abec:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000abf0:	9300      	str	r3, [sp, #0]
3000abf2:	e71a      	b.n	3000aa2a <rxi316_perf_tune+0x4f2>
3000abf4:	f04f 0a1f 	mov.w	sl, #31
3000abf8:	e6fa      	b.n	3000a9f0 <rxi316_perf_tune+0x4b8>
3000abfa:	20f0      	movs	r0, #240	; 0xf0
3000abfc:	e6ee      	b.n	3000a9dc <rxi316_perf_tune+0x4a4>
3000abfe:	f04f 080f 	mov.w	r8, #15
3000ac02:	e6e3      	b.n	3000a9cc <rxi316_perf_tune+0x494>
3000ac04:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000ac08:	9109      	str	r1, [sp, #36]	; 0x24
3000ac0a:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ac0c:	2920      	cmp	r1, #32
3000ac0e:	f47f ade8 	bne.w	3000a7e2 <rxi316_perf_tune+0x2aa>
3000ac12:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000ac16:	910a      	str	r1, [sp, #40]	; 0x28
3000ac18:	990c      	ldr	r1, [sp, #48]	; 0x30
3000ac1a:	2920      	cmp	r1, #32
3000ac1c:	f47f adeb 	bne.w	3000a7f6 <rxi316_perf_tune+0x2be>
3000ac20:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000ac24:	910b      	str	r1, [sp, #44]	; 0x2c
3000ac26:	990d      	ldr	r1, [sp, #52]	; 0x34
3000ac28:	2920      	cmp	r1, #32
3000ac2a:	f47f adee 	bne.w	3000a80a <rxi316_perf_tune+0x2d2>
3000ac2e:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000ac32:	910c      	str	r1, [sp, #48]	; 0x30
3000ac34:	990e      	ldr	r1, [sp, #56]	; 0x38
3000ac36:	2920      	cmp	r1, #32
3000ac38:	f47f adf1 	bne.w	3000a81e <rxi316_perf_tune+0x2e6>
3000ac3c:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000ac40:	910d      	str	r1, [sp, #52]	; 0x34
3000ac42:	990f      	ldr	r1, [sp, #60]	; 0x3c
3000ac44:	2920      	cmp	r1, #32
3000ac46:	f47f adf2 	bne.w	3000a82e <rxi316_perf_tune+0x2f6>
3000ac4a:	210f      	movs	r1, #15
3000ac4c:	910e      	str	r1, [sp, #56]	; 0x38
3000ac4e:	9910      	ldr	r1, [sp, #64]	; 0x40
3000ac50:	2920      	cmp	r1, #32
3000ac52:	f47f adf5 	bne.w	3000a840 <rxi316_perf_tune+0x308>
3000ac56:	21f0      	movs	r1, #240	; 0xf0
3000ac58:	910f      	str	r1, [sp, #60]	; 0x3c
3000ac5a:	9911      	ldr	r1, [sp, #68]	; 0x44
3000ac5c:	2920      	cmp	r1, #32
3000ac5e:	f47f adf8 	bne.w	3000a852 <rxi316_perf_tune+0x31a>
3000ac62:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000ac66:	9110      	str	r1, [sp, #64]	; 0x40
3000ac68:	9912      	ldr	r1, [sp, #72]	; 0x48
3000ac6a:	2920      	cmp	r1, #32
3000ac6c:	f47f adfb 	bne.w	3000a866 <rxi316_perf_tune+0x32e>
3000ac70:	f44f 4170 	mov.w	r1, #61440	; 0xf000
3000ac74:	9111      	str	r1, [sp, #68]	; 0x44
3000ac76:	9913      	ldr	r1, [sp, #76]	; 0x4c
3000ac78:	2920      	cmp	r1, #32
3000ac7a:	f47f adfd 	bne.w	3000a878 <rxi316_perf_tune+0x340>
3000ac7e:	f44f 2170 	mov.w	r1, #983040	; 0xf0000
3000ac82:	9112      	str	r1, [sp, #72]	; 0x48
3000ac84:	9914      	ldr	r1, [sp, #80]	; 0x50
3000ac86:	2920      	cmp	r1, #32
3000ac88:	f47f ae00 	bne.w	3000a88c <rxi316_perf_tune+0x354>
3000ac8c:	f44f 0170 	mov.w	r1, #15728640	; 0xf00000
3000ac90:	9113      	str	r1, [sp, #76]	; 0x4c
3000ac92:	9915      	ldr	r1, [sp, #84]	; 0x54
3000ac94:	2920      	cmp	r1, #32
3000ac96:	f47f ae03 	bne.w	3000a8a0 <rxi316_perf_tune+0x368>
3000ac9a:	f04f 6170 	mov.w	r1, #251658240	; 0xf000000
3000ac9e:	911d      	str	r1, [sp, #116]	; 0x74
3000aca0:	9916      	ldr	r1, [sp, #88]	; 0x58
3000aca2:	2920      	cmp	r1, #32
3000aca4:	f47f ae06 	bne.w	3000a8b4 <rxi316_perf_tune+0x37c>
3000aca8:	f04f 4170 	mov.w	r1, #4026531840	; 0xf0000000
3000acac:	911c      	str	r1, [sp, #112]	; 0x70
3000acae:	9917      	ldr	r1, [sp, #92]	; 0x5c
3000acb0:	2920      	cmp	r1, #32
3000acb2:	f47f ae07 	bne.w	3000a8c4 <rxi316_perf_tune+0x38c>
3000acb6:	210f      	movs	r1, #15
3000acb8:	911b      	str	r1, [sp, #108]	; 0x6c
3000acba:	9904      	ldr	r1, [sp, #16]
3000acbc:	2920      	cmp	r1, #32
3000acbe:	f47f ae0a 	bne.w	3000a8d6 <rxi316_perf_tune+0x39e>
3000acc2:	211f      	movs	r1, #31
3000acc4:	9117      	str	r1, [sp, #92]	; 0x5c
3000acc6:	9905      	ldr	r1, [sp, #20]
3000acc8:	2920      	cmp	r1, #32
3000acca:	f47f ae0d 	bne.w	3000a8e8 <rxi316_perf_tune+0x3b0>
3000acce:	f44f 7178 	mov.w	r1, #992	; 0x3e0
3000acd2:	9116      	str	r1, [sp, #88]	; 0x58
3000acd4:	9918      	ldr	r1, [sp, #96]	; 0x60
3000acd6:	2920      	cmp	r1, #32
3000acd8:	f47f ae10 	bne.w	3000a8fc <rxi316_perf_tune+0x3c4>
3000acdc:	f44f 41f8 	mov.w	r1, #31744	; 0x7c00
3000ace0:	2b20      	cmp	r3, #32
3000ace2:	9115      	str	r1, [sp, #84]	; 0x54
3000ace4:	f47f ae15 	bne.w	3000a912 <rxi316_perf_tune+0x3da>
3000ace8:	230f      	movs	r3, #15
3000acea:	f1bb 0f20 	cmp.w	fp, #32
3000acee:	9314      	str	r3, [sp, #80]	; 0x50
3000acf0:	f47f ae19 	bne.w	3000a926 <rxi316_perf_tune+0x3ee>
3000acf4:	f1ba 0f20 	cmp.w	sl, #32
3000acf8:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000acfc:	f47f ae1e 	bne.w	3000a93c <rxi316_perf_tune+0x404>
3000ad00:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000ad04:	f1b9 0f20 	cmp.w	r9, #32
3000ad08:	9305      	str	r3, [sp, #20]
3000ad0a:	f47f ae24 	bne.w	3000a956 <rxi316_perf_tune+0x41e>
3000ad0e:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000ad12:	f1b8 0f20 	cmp.w	r8, #32
3000ad16:	9304      	str	r3, [sp, #16]
3000ad18:	f47f ae29 	bne.w	3000a96e <rxi316_perf_tune+0x436>
3000ad1c:	2f20      	cmp	r7, #32
3000ad1e:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000ad22:	f47f ae2f 	bne.w	3000a984 <rxi316_perf_tune+0x44c>
3000ad26:	9b00      	ldr	r3, [sp, #0]
3000ad28:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000ad2c:	2b20      	cmp	r3, #32
3000ad2e:	f47f ae35 	bne.w	3000a99c <rxi316_perf_tune+0x464>
3000ad32:	9b01      	ldr	r3, [sp, #4]
3000ad34:	f04f 6770 	mov.w	r7, #251658240	; 0xf000000
3000ad38:	2b20      	cmp	r3, #32
3000ad3a:	f47f ae39 	bne.w	3000a9b0 <rxi316_perf_tune+0x478>
3000ad3e:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000ad42:	9301      	str	r3, [sp, #4]
3000ad44:	e638      	b.n	3000a9b8 <rxi316_perf_tune+0x480>
3000ad46:	f44f 6170 	mov.w	r1, #3840	; 0xf00
3000ad4a:	9108      	str	r1, [sp, #32]
3000ad4c:	e53c      	b.n	3000a7c8 <rxi316_perf_tune+0x290>
3000ad4e:	22f0      	movs	r2, #240	; 0xf0
3000ad50:	e530      	b.n	3000a7b4 <rxi316_perf_tune+0x27c>
3000ad52:	220f      	movs	r2, #15
3000ad54:	9207      	str	r2, [sp, #28]
3000ad56:	e525      	b.n	3000a7a4 <rxi316_perf_tune+0x26c>
3000ad58:	f10c 0c01 	add.w	ip, ip, #1
3000ad5c:	9202      	str	r2, [sp, #8]
3000ad5e:	e467      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad60:	f10c 0c01 	add.w	ip, ip, #1
3000ad64:	9203      	str	r2, [sp, #12]
3000ad66:	e463      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad68:	2202      	movs	r2, #2
3000ad6a:	e50a      	b.n	3000a782 <rxi316_perf_tune+0x24a>
3000ad6c:	3101      	adds	r1, #1
3000ad6e:	9205      	str	r2, [sp, #20]
3000ad70:	e45e      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad72:	3101      	adds	r1, #1
3000ad74:	9204      	str	r2, [sp, #16]
3000ad76:	e45b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad78:	481c      	ldr	r0, [pc, #112]	; (3000adec <rxi316_perf_tune+0x8b4>)
3000ad7a:	911b      	str	r1, [sp, #108]	; 0x6c
3000ad7c:	e9cd 231c 	strd	r2, r3, [sp, #112]	; 0x70
3000ad80:	f001 f9d6 	bl	3000c130 <__DiagPrintf_veneer>
3000ad84:	991b      	ldr	r1, [sp, #108]	; 0x6c
3000ad86:	e9dd 231c 	ldrd	r2, r3, [sp, #112]	; 0x70
3000ad8a:	e4d5      	b.n	3000a738 <rxi316_perf_tune+0x200>
3000ad8c:	3101      	adds	r1, #1
3000ad8e:	9218      	str	r2, [sp, #96]	; 0x60
3000ad90:	e44e      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad92:	3101      	adds	r1, #1
3000ad94:	4614      	mov	r4, r2
3000ad96:	e44b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad98:	3101      	adds	r1, #1
3000ad9a:	921a      	str	r2, [sp, #104]	; 0x68
3000ad9c:	e448      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ad9e:	3101      	adds	r1, #1
3000ada0:	9219      	str	r2, [sp, #100]	; 0x64
3000ada2:	e445      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ada4:	9207      	str	r2, [sp, #28]
3000ada6:	e443      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ada8:	9208      	str	r2, [sp, #32]
3000adaa:	e441      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adac:	9209      	str	r2, [sp, #36]	; 0x24
3000adae:	e43f      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adb0:	920b      	str	r2, [sp, #44]	; 0x2c
3000adb2:	e43d      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adb4:	920a      	str	r2, [sp, #40]	; 0x28
3000adb6:	e43b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adb8:	9217      	str	r2, [sp, #92]	; 0x5c
3000adba:	e439      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adbc:	9216      	str	r2, [sp, #88]	; 0x58
3000adbe:	e437      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adc0:	920d      	str	r2, [sp, #52]	; 0x34
3000adc2:	e435      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adc4:	920c      	str	r2, [sp, #48]	; 0x30
3000adc6:	e433      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adc8:	9215      	str	r2, [sp, #84]	; 0x54
3000adca:	e431      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000adcc:	9214      	str	r2, [sp, #80]	; 0x50
3000adce:	e42f      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000add0:	9213      	str	r2, [sp, #76]	; 0x4c
3000add2:	e42d      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000add4:	9212      	str	r2, [sp, #72]	; 0x48
3000add6:	e42b      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000add8:	9211      	str	r2, [sp, #68]	; 0x44
3000adda:	e429      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000addc:	9210      	str	r2, [sp, #64]	; 0x40
3000adde:	e427      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ade0:	920f      	str	r2, [sp, #60]	; 0x3c
3000ade2:	e425      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ade4:	920e      	str	r2, [sp, #56]	; 0x38
3000ade6:	e423      	b.n	3000a630 <rxi316_perf_tune+0xf8>
3000ade8:	40000008 	.word	0x40000008
3000adec:	3000d598 	.word	0x3000d598

3000adf0 <rxi316_dram_init>:
3000adf0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000adf4:	4604      	mov	r4, r0
3000adf6:	b08f      	sub	sp, #60	; 0x3c
3000adf8:	4b10      	ldr	r3, [pc, #64]	; (3000ae3c <rxi316_dram_init+0x4c>)
3000adfa:	4798      	blx	r3
3000adfc:	f894 b014 	ldrb.w	fp, [r4, #20]
3000ae00:	2101      	movs	r1, #1
3000ae02:	900c      	str	r0, [sp, #48]	; 0x30
3000ae04:	fa01 f30b 	lsl.w	r3, r1, fp
3000ae08:	9302      	str	r3, [sp, #8]
3000ae0a:	6823      	ldr	r3, [r4, #0]
3000ae0c:	78da      	ldrb	r2, [r3, #3]
3000ae0e:	6923      	ldr	r3, [r4, #16]
3000ae10:	fa03 f30b 	lsl.w	r3, r3, fp
3000ae14:	b1a2      	cbz	r2, 3000ae40 <rxi316_dram_init+0x50>
3000ae16:	428a      	cmp	r2, r1
3000ae18:	f000 847c 	beq.w	3000b714 <rxi316_dram_init+0x924>
3000ae1c:	2a02      	cmp	r2, #2
3000ae1e:	bf0b      	itete	eq
3000ae20:	2206      	moveq	r2, #6
3000ae22:	220a      	movne	r2, #10
3000ae24:	2540      	moveq	r5, #64	; 0x40
3000ae26:	2580      	movne	r5, #128	; 0x80
3000ae28:	9207      	str	r2, [sp, #28]
3000ae2a:	bf0c      	ite	eq
3000ae2c:	2204      	moveq	r2, #4
3000ae2e:	2208      	movne	r2, #8
3000ae30:	9208      	str	r2, [sp, #32]
3000ae32:	bf0c      	ite	eq
3000ae34:	2208      	moveq	r2, #8
3000ae36:	2210      	movne	r2, #16
3000ae38:	9201      	str	r2, [sp, #4]
3000ae3a:	e007      	b.n	3000ae4c <rxi316_dram_init+0x5c>
3000ae3c:	30007fbd 	.word	0x30007fbd
3000ae40:	2203      	movs	r2, #3
3000ae42:	25c0      	movs	r5, #192	; 0xc0
3000ae44:	9108      	str	r1, [sp, #32]
3000ae46:	9207      	str	r2, [sp, #28]
3000ae48:	2202      	movs	r2, #2
3000ae4a:	9201      	str	r2, [sp, #4]
3000ae4c:	6860      	ldr	r0, [r4, #4]
3000ae4e:	f04f 31ff 	mov.w	r1, #4294967295
3000ae52:	4ac2      	ldr	r2, [pc, #776]	; (3000b15c <rxi316_dram_init+0x36c>)
3000ae54:	6806      	ldr	r6, [r0, #0]
3000ae56:	960d      	str	r6, [sp, #52]	; 0x34
3000ae58:	6846      	ldr	r6, [r0, #4]
3000ae5a:	6880      	ldr	r0, [r0, #8]
3000ae5c:	9609      	str	r6, [sp, #36]	; 0x24
3000ae5e:	9003      	str	r0, [sp, #12]
3000ae60:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000ae64:	6820      	ldr	r0, [r4, #0]
3000ae66:	7981      	ldrb	r1, [r0, #6]
3000ae68:	6882      	ldr	r2, [r0, #8]
3000ae6a:	b129      	cbz	r1, 3000ae78 <rxi316_dram_init+0x88>
3000ae6c:	2901      	cmp	r1, #1
3000ae6e:	f000 8449 	beq.w	3000b704 <rxi316_dram_init+0x914>
3000ae72:	2903      	cmp	r1, #3
3000ae74:	f000 8456 	beq.w	3000b724 <rxi316_dram_init+0x934>
3000ae78:	3a06      	subs	r2, #6
3000ae7a:	7881      	ldrb	r1, [r0, #2]
3000ae7c:	f002 020f 	and.w	r2, r2, #15
3000ae80:	7940      	ldrb	r0, [r0, #5]
3000ae82:	0109      	lsls	r1, r1, #4
3000ae84:	0280      	lsls	r0, r0, #10
3000ae86:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000ae8a:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000ae8e:	4301      	orrs	r1, r0
3000ae90:	430d      	orrs	r5, r1
3000ae92:	432a      	orrs	r2, r5
3000ae94:	4db1      	ldr	r5, [pc, #708]	; (3000b15c <rxi316_dram_init+0x36c>)
3000ae96:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000ae9a:	68e2      	ldr	r2, [r4, #12]
3000ae9c:	6812      	ldr	r2, [r2, #0]
3000ae9e:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000aea2:	f002 0207 	and.w	r2, r2, #7
3000aea6:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000aeaa:	68e2      	ldr	r2, [r4, #12]
3000aeac:	6811      	ldr	r1, [r2, #0]
3000aeae:	6822      	ldr	r2, [r4, #0]
3000aeb0:	6848      	ldr	r0, [r1, #4]
3000aeb2:	f892 9000 	ldrb.w	r9, [r2]
3000aeb6:	688a      	ldr	r2, [r1, #8]
3000aeb8:	9004      	str	r0, [sp, #16]
3000aeba:	0300      	lsls	r0, r0, #12
3000aebc:	0412      	lsls	r2, r2, #16
3000aebe:	f009 070f 	and.w	r7, r9, #15
3000aec2:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000aec6:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000aeca:	4302      	orrs	r2, r0
3000aecc:	68c8      	ldr	r0, [r1, #12]
3000aece:	0440      	lsls	r0, r0, #17
3000aed0:	433a      	orrs	r2, r7
3000aed2:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000aed6:	6908      	ldr	r0, [r1, #16]
3000aed8:	0480      	lsls	r0, r0, #18
3000aeda:	4332      	orrs	r2, r6
3000aedc:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000aee0:	4310      	orrs	r0, r2
3000aee2:	694a      	ldr	r2, [r1, #20]
3000aee4:	04d2      	lsls	r2, r2, #19
3000aee6:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000aeea:	4310      	orrs	r0, r2
3000aeec:	698a      	ldr	r2, [r1, #24]
3000aeee:	0552      	lsls	r2, r2, #21
3000aef0:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000aef4:	4310      	orrs	r0, r2
3000aef6:	69ca      	ldr	r2, [r1, #28]
3000aef8:	0592      	lsls	r2, r2, #22
3000aefa:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000aefe:	4310      	orrs	r0, r2
3000af00:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000af02:	0612      	lsls	r2, r2, #24
3000af04:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000af08:	4302      	orrs	r2, r0
3000af0a:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000af0e:	06c0      	lsls	r0, r0, #27
3000af10:	0789      	lsls	r1, r1, #30
3000af12:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000af16:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000af1a:	4310      	orrs	r0, r2
3000af1c:	9a02      	ldr	r2, [sp, #8]
3000af1e:	0212      	lsls	r2, r2, #8
3000af20:	4308      	orrs	r0, r1
3000af22:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000af26:	4310      	orrs	r0, r2
3000af28:	6068      	str	r0, [r5, #4]
3000af2a:	6822      	ldr	r2, [r4, #0]
3000af2c:	7992      	ldrb	r2, [r2, #6]
3000af2e:	2a00      	cmp	r2, #0
3000af30:	f040 8317 	bne.w	3000b562 <rxi316_dram_init+0x772>
3000af34:	686a      	ldr	r2, [r5, #4]
3000af36:	f042 0210 	orr.w	r2, r2, #16
3000af3a:	606a      	str	r2, [r5, #4]
3000af3c:	68e2      	ldr	r2, [r4, #12]
3000af3e:	9802      	ldr	r0, [sp, #8]
3000af40:	6812      	ldr	r2, [r2, #0]
3000af42:	f100 3aff 	add.w	sl, r0, #4294967295
3000af46:	6861      	ldr	r1, [r4, #4]
3000af48:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000af4a:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000af4e:	9005      	str	r0, [sp, #20]
3000af50:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000af52:	fa2e f80b 	lsr.w	r8, lr, fp
3000af56:	6949      	ldr	r1, [r1, #20]
3000af58:	ea1e 0f0a 	tst.w	lr, sl
3000af5c:	9006      	str	r0, [sp, #24]
3000af5e:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000af60:	fa21 fe0b 	lsr.w	lr, r1, fp
3000af64:	bf18      	it	ne
3000af66:	f108 0801 	addne.w	r8, r8, #1
3000af6a:	ea11 0f0a 	tst.w	r1, sl
3000af6e:	9906      	ldr	r1, [sp, #24]
3000af70:	950a      	str	r5, [sp, #40]	; 0x28
3000af72:	bf18      	it	ne
3000af74:	f10e 0e01 	addne.w	lr, lr, #1
3000af78:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000af7a:	0049      	lsls	r1, r1, #1
3000af7c:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000af80:	950b      	str	r5, [sp, #44]	; 0x2c
3000af82:	f001 0102 	and.w	r1, r1, #2
3000af86:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000af8a:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000af8e:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000af92:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000af96:	0080      	lsls	r0, r0, #2
3000af98:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000af9c:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000afa0:	9006      	str	r0, [sp, #24]
3000afa2:	013f      	lsls	r7, r7, #4
3000afa4:	9805      	ldr	r0, [sp, #20]
3000afa6:	f00c 0c08 	and.w	ip, ip, #8
3000afaa:	0176      	lsls	r6, r6, #5
3000afac:	f007 0710 	and.w	r7, r7, #16
3000afb0:	f000 0001 	and.w	r0, r0, #1
3000afb4:	f006 0620 	and.w	r6, r6, #32
3000afb8:	4605      	mov	r5, r0
3000afba:	9806      	ldr	r0, [sp, #24]
3000afbc:	f000 0004 	and.w	r0, r0, #4
3000afc0:	4301      	orrs	r1, r0
3000afc2:	4329      	orrs	r1, r5
3000afc4:	ea41 0c0c 	orr.w	ip, r1, ip
3000afc8:	990a      	ldr	r1, [sp, #40]	; 0x28
3000afca:	ea4c 0707 	orr.w	r7, ip, r7
3000afce:	018d      	lsls	r5, r1, #6
3000afd0:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000afd2:	4337      	orrs	r7, r6
3000afd4:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000afd8:	0208      	lsls	r0, r1, #8
3000afda:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000afdc:	432f      	orrs	r7, r5
3000afde:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000afe2:	06c9      	lsls	r1, r1, #27
3000afe4:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000afe8:	4307      	orrs	r7, r0
3000afea:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000afee:	0736      	lsls	r6, r6, #28
3000aff0:	430f      	orrs	r7, r1
3000aff2:	0752      	lsls	r2, r2, #29
3000aff4:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000aff8:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000affc:	433e      	orrs	r6, r7
3000affe:	4316      	orrs	r6, r2
3000b000:	4a56      	ldr	r2, [pc, #344]	; (3000b15c <rxi316_dram_init+0x36c>)
3000b002:	ea46 0608 	orr.w	r6, r6, r8
3000b006:	ea46 060e 	orr.w	r6, r6, lr
3000b00a:	6096      	str	r6, [r2, #8]
3000b00c:	6862      	ldr	r2, [r4, #4]
3000b00e:	6990      	ldr	r0, [r2, #24]
3000b010:	9a01      	ldr	r2, [sp, #4]
3000b012:	2a10      	cmp	r2, #16
3000b014:	6822      	ldr	r2, [r4, #0]
3000b016:	7811      	ldrb	r1, [r2, #0]
3000b018:	7916      	ldrb	r6, [r2, #4]
3000b01a:	f000 833d 	beq.w	3000b698 <rxi316_dram_init+0x8a8>
3000b01e:	f1a1 0502 	sub.w	r5, r1, #2
3000b022:	f1a1 0e09 	sub.w	lr, r1, #9
3000b026:	9f01      	ldr	r7, [sp, #4]
3000b028:	fab5 f585 	clz	r5, r5
3000b02c:	fabe fe8e 	clz	lr, lr
3000b030:	2f08      	cmp	r7, #8
3000b032:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b036:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000b03a:	ea45 020e 	orr.w	r2, r5, lr
3000b03e:	f000 829e 	beq.w	3000b57e <rxi316_dram_init+0x78e>
3000b042:	9f01      	ldr	r7, [sp, #4]
3000b044:	2f04      	cmp	r7, #4
3000b046:	f000 83a5 	beq.w	3000b794 <rxi316_dram_init+0x9a4>
3000b04a:	9f01      	ldr	r7, [sp, #4]
3000b04c:	2f02      	cmp	r7, #2
3000b04e:	bf08      	it	eq
3000b050:	4310      	orreq	r0, r2
3000b052:	2a00      	cmp	r2, #0
3000b054:	f040 8298 	bne.w	3000b588 <rxi316_dram_init+0x798>
3000b058:	2903      	cmp	r1, #3
3000b05a:	f000 8368 	beq.w	3000b72e <rxi316_dram_init+0x93e>
3000b05e:	2500      	movs	r5, #0
3000b060:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b064:	462f      	mov	r7, r5
3000b066:	433d      	orrs	r5, r7
3000b068:	4f3c      	ldr	r7, [pc, #240]	; (3000b15c <rxi316_dram_init+0x36c>)
3000b06a:	430d      	orrs	r5, r1
3000b06c:	4305      	orrs	r5, r0
3000b06e:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000b072:	637e      	str	r6, [r7, #52]	; 0x34
3000b074:	6862      	ldr	r2, [r4, #4]
3000b076:	9903      	ldr	r1, [sp, #12]
3000b078:	69d2      	ldr	r2, [r2, #28]
3000b07a:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000b07e:	63ba      	str	r2, [r7, #56]	; 0x38
3000b080:	6822      	ldr	r2, [r4, #0]
3000b082:	7810      	ldrb	r0, [r2, #0]
3000b084:	6862      	ldr	r2, [r4, #4]
3000b086:	2803      	cmp	r0, #3
3000b088:	6a11      	ldr	r1, [r2, #32]
3000b08a:	f000 82f8 	beq.w	3000b67e <rxi316_dram_init+0x88e>
3000b08e:	2809      	cmp	r0, #9
3000b090:	f000 830d 	beq.w	3000b6ae <rxi316_dram_init+0x8be>
3000b094:	4831      	ldr	r0, [pc, #196]	; (3000b15c <rxi316_dram_init+0x36c>)
3000b096:	63c1      	str	r1, [r0, #60]	; 0x3c
3000b098:	6862      	ldr	r2, [r4, #4]
3000b09a:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000b09c:	6402      	str	r2, [r0, #64]	; 0x40
3000b09e:	6865      	ldr	r5, [r4, #4]
3000b0a0:	68e8      	ldr	r0, [r5, #12]
3000b0a2:	682a      	ldr	r2, [r5, #0]
3000b0a4:	6869      	ldr	r1, [r5, #4]
3000b0a6:	4402      	add	r2, r0
3000b0a8:	68ae      	ldr	r6, [r5, #8]
3000b0aa:	4401      	add	r1, r0
3000b0ac:	ea1a 0f02 	tst.w	sl, r2
3000b0b0:	f000 82b8 	beq.w	3000b624 <rxi316_dram_init+0x834>
3000b0b4:	fa22 f20b 	lsr.w	r2, r2, fp
3000b0b8:	1c50      	adds	r0, r2, #1
3000b0ba:	ea1a 0f01 	tst.w	sl, r1
3000b0be:	fa21 f50b 	lsr.w	r5, r1, fp
3000b0c2:	6822      	ldr	r2, [r4, #0]
3000b0c4:	fa26 f10b 	lsr.w	r1, r6, fp
3000b0c8:	bf18      	it	ne
3000b0ca:	3501      	addne	r5, #1
3000b0cc:	ea1a 0f06 	tst.w	sl, r6
3000b0d0:	7816      	ldrb	r6, [r2, #0]
3000b0d2:	bf18      	it	ne
3000b0d4:	3101      	addne	r1, #1
3000b0d6:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000b0da:	1eb2      	subs	r2, r6, #2
3000b0dc:	2a01      	cmp	r2, #1
3000b0de:	f240 82a4 	bls.w	3000b62a <rxi316_dram_init+0x83a>
3000b0e2:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000b0e6:	fbb7 f2f3 	udiv	r2, r7, r3
3000b0ea:	fb03 7712 	mls	r7, r3, r2, r7
3000b0ee:	b107      	cbz	r7, 3000b0f2 <rxi316_dram_init+0x302>
3000b0f0:	3201      	adds	r2, #1
3000b0f2:	2e09      	cmp	r6, #9
3000b0f4:	f040 82b3 	bne.w	3000b65e <rxi316_dram_init+0x86e>
3000b0f8:	9e08      	ldr	r6, [sp, #32]
3000b0fa:	fa26 f60b 	lsr.w	r6, r6, fp
3000b0fe:	016d      	lsls	r5, r5, #5
3000b100:	f000 001f 	and.w	r0, r0, #31
3000b104:	0289      	lsls	r1, r1, #10
3000b106:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000b10a:	0512      	lsls	r2, r2, #20
3000b10c:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000b110:	4305      	orrs	r5, r0
3000b112:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000b116:	430d      	orrs	r5, r1
3000b118:	4910      	ldr	r1, [pc, #64]	; (3000b15c <rxi316_dram_init+0x36c>)
3000b11a:	4315      	orrs	r5, r2
3000b11c:	0672      	lsls	r2, r6, #25
3000b11e:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000b122:	432a      	orrs	r2, r5
3000b124:	630a      	str	r2, [r1, #48]	; 0x30
3000b126:	68a0      	ldr	r0, [r4, #8]
3000b128:	6802      	ldr	r2, [r0, #0]
3000b12a:	fbb2 f1f3 	udiv	r1, r2, r3
3000b12e:	fb03 2211 	mls	r2, r3, r1, r2
3000b132:	2a00      	cmp	r2, #0
3000b134:	f000 8274 	beq.w	3000b620 <rxi316_dram_init+0x830>
3000b138:	1c4a      	adds	r2, r1, #1
3000b13a:	9205      	str	r2, [sp, #20]
3000b13c:	6842      	ldr	r2, [r0, #4]
3000b13e:	fbb2 f0f3 	udiv	r0, r2, r3
3000b142:	fb03 2210 	mls	r2, r3, r0, r2
3000b146:	2a00      	cmp	r2, #0
3000b148:	f000 8268 	beq.w	3000b61c <rxi316_dram_init+0x82c>
3000b14c:	38c7      	subs	r0, #199	; 0xc7
3000b14e:	68e2      	ldr	r2, [r4, #12]
3000b150:	6811      	ldr	r1, [r2, #0]
3000b152:	4a03      	ldr	r2, [pc, #12]	; (3000b160 <rxi316_dram_init+0x370>)
3000b154:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000b158:	e004      	b.n	3000b164 <rxi316_dram_init+0x374>
3000b15a:	bf00      	nop
3000b15c:	41100000 	.word	0x41100000
3000b160:	00ffff00 	.word	0x00ffff00
3000b164:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000b168:	900a      	str	r0, [sp, #40]	; 0x28
3000b16a:	0712      	lsls	r2, r2, #28
3000b16c:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000b170:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000b174:	430a      	orrs	r2, r1
3000b176:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000b17a:	430a      	orrs	r2, r1
3000b17c:	49c3      	ldr	r1, [pc, #780]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b17e:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000b182:	610a      	str	r2, [r1, #16]
3000b184:	68a2      	ldr	r2, [r4, #8]
3000b186:	6911      	ldr	r1, [r2, #16]
3000b188:	fbb1 fcf3 	udiv	ip, r1, r3
3000b18c:	fb03 111c 	mls	r1, r3, ip, r1
3000b190:	2900      	cmp	r1, #0
3000b192:	f000 8240 	beq.w	3000b616 <rxi316_dram_init+0x826>
3000b196:	f10c 0101 	add.w	r1, ip, #1
3000b19a:	9106      	str	r1, [sp, #24]
3000b19c:	6991      	ldr	r1, [r2, #24]
3000b19e:	fbb1 f0f3 	udiv	r0, r1, r3
3000b1a2:	fb03 1110 	mls	r1, r3, r0, r1
3000b1a6:	b101      	cbz	r1, 3000b1aa <rxi316_dram_init+0x3ba>
3000b1a8:	3001      	adds	r0, #1
3000b1aa:	68d1      	ldr	r1, [r2, #12]
3000b1ac:	fbb1 f5f3 	udiv	r5, r1, r3
3000b1b0:	fb03 1115 	mls	r1, r3, r5, r1
3000b1b4:	9503      	str	r5, [sp, #12]
3000b1b6:	b109      	cbz	r1, 3000b1bc <rxi316_dram_init+0x3cc>
3000b1b8:	3501      	adds	r5, #1
3000b1ba:	9503      	str	r5, [sp, #12]
3000b1bc:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000b1be:	fbb5 f1f3 	udiv	r1, r5, r3
3000b1c2:	fb03 5511 	mls	r5, r3, r1, r5
3000b1c6:	b105      	cbz	r5, 3000b1ca <rxi316_dram_init+0x3da>
3000b1c8:	3101      	adds	r1, #1
3000b1ca:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000b1cc:	0349      	lsls	r1, r1, #13
3000b1ce:	68e2      	ldr	r2, [r4, #12]
3000b1d0:	fa25 f60b 	lsr.w	r6, r5, fp
3000b1d4:	ea15 0f0a 	tst.w	r5, sl
3000b1d8:	6812      	ldr	r2, [r2, #0]
3000b1da:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000b1de:	bf18      	it	ne
3000b1e0:	3601      	addne	r6, #1
3000b1e2:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000b1e4:	05d2      	lsls	r2, r2, #23
3000b1e6:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000b1ea:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000b1ee:	9a06      	ldr	r2, [sp, #24]
3000b1f0:	0112      	lsls	r2, r2, #4
3000b1f2:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000b1f6:	4332      	orrs	r2, r6
3000b1f8:	0246      	lsls	r6, r0, #9
3000b1fa:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000b1fe:	4316      	orrs	r6, r2
3000b200:	9a03      	ldr	r2, [sp, #12]
3000b202:	f002 020f 	and.w	r2, r2, #15
3000b206:	4316      	orrs	r6, r2
3000b208:	4aa0      	ldr	r2, [pc, #640]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b20a:	4331      	orrs	r1, r6
3000b20c:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000b210:	6151      	str	r1, [r2, #20]
3000b212:	68a7      	ldr	r7, [r4, #8]
3000b214:	697a      	ldr	r2, [r7, #20]
3000b216:	fbb2 fef3 	udiv	lr, r2, r3
3000b21a:	fb03 221e 	mls	r2, r3, lr, r2
3000b21e:	b10a      	cbz	r2, 3000b224 <rxi316_dram_init+0x434>
3000b220:	f10e 0e01 	add.w	lr, lr, #1
3000b224:	6939      	ldr	r1, [r7, #16]
3000b226:	fbb1 f2f3 	udiv	r2, r1, r3
3000b22a:	fb03 1112 	mls	r1, r3, r2, r1
3000b22e:	b101      	cbz	r1, 3000b232 <rxi316_dram_init+0x442>
3000b230:	3201      	adds	r2, #1
3000b232:	68fd      	ldr	r5, [r7, #12]
3000b234:	fbb5 f1f3 	udiv	r1, r5, r3
3000b238:	fb03 5511 	mls	r5, r3, r1, r5
3000b23c:	b105      	cbz	r5, 3000b240 <rxi316_dram_init+0x450>
3000b23e:	3101      	adds	r1, #1
3000b240:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000b242:	fbb5 f6f3 	udiv	r6, r5, r3
3000b246:	fb03 5516 	mls	r5, r3, r6, r5
3000b24a:	b105      	cbz	r5, 3000b24e <rxi316_dram_init+0x45e>
3000b24c:	3601      	adds	r6, #1
3000b24e:	68bd      	ldr	r5, [r7, #8]
3000b250:	4411      	add	r1, r2
3000b252:	fbb5 f2f3 	udiv	r2, r5, r3
3000b256:	42b1      	cmp	r1, r6
3000b258:	fb03 5512 	mls	r5, r3, r2, r5
3000b25c:	bf38      	it	cc
3000b25e:	4631      	movcc	r1, r6
3000b260:	b905      	cbnz	r5, 3000b264 <rxi316_dram_init+0x474>
3000b262:	3a01      	subs	r2, #1
3000b264:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000b266:	ea15 0f0a 	tst.w	r5, sl
3000b26a:	fa25 fc0b 	lsr.w	ip, r5, fp
3000b26e:	69fd      	ldr	r5, [r7, #28]
3000b270:	bf18      	it	ne
3000b272:	f10c 0c01 	addne.w	ip, ip, #1
3000b276:	fa25 f60b 	lsr.w	r6, r5, fp
3000b27a:	ea15 0f0a 	tst.w	r5, sl
3000b27e:	bf18      	it	ne
3000b280:	3601      	addne	r6, #1
3000b282:	f1b9 0f09 	cmp.w	r9, #9
3000b286:	f000 81f5 	beq.w	3000b674 <rxi316_dram_init+0x884>
3000b28a:	f1b9 0f02 	cmp.w	r9, #2
3000b28e:	f000 8233 	beq.w	3000b6f8 <rxi316_dram_init+0x908>
3000b292:	f1b9 0f03 	cmp.w	r9, #3
3000b296:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b298:	f000 8298 	beq.w	3000b7cc <rxi316_dram_init+0x9dc>
3000b29c:	46a8      	mov	r8, r5
3000b29e:	9d01      	ldr	r5, [sp, #4]
3000b2a0:	44a8      	add	r8, r5
3000b2a2:	fa28 f50b 	lsr.w	r5, r8, fp
3000b2a6:	ea18 0f0a 	tst.w	r8, sl
3000b2aa:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000b2ae:	bf18      	it	ne
3000b2b0:	3501      	addne	r5, #1
3000b2b2:	fbb8 f7f3 	udiv	r7, r8, r3
3000b2b6:	fb03 8817 	mls	r8, r3, r7, r8
3000b2ba:	f1b8 0f00 	cmp.w	r8, #0
3000b2be:	d000      	beq.n	3000b2c2 <rxi316_dram_init+0x4d2>
3000b2c0:	3701      	adds	r7, #1
3000b2c2:	0109      	lsls	r1, r1, #4
3000b2c4:	f00e 0e0f 	and.w	lr, lr, #15
3000b2c8:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000b2cc:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000b2d0:	063f      	lsls	r7, r7, #24
3000b2d2:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000b2d6:	ea41 0e0e 	orr.w	lr, r1, lr
3000b2da:	0291      	lsls	r1, r2, #10
3000b2dc:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000b2e0:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000b2e4:	ea4e 0e01 	orr.w	lr, lr, r1
3000b2e8:	0471      	lsls	r1, r6, #17
3000b2ea:	ea4e 0c0c 	orr.w	ip, lr, ip
3000b2ee:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000b2f2:	ea4c 0c01 	orr.w	ip, ip, r1
3000b2f6:	0529      	lsls	r1, r5, #20
3000b2f8:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000b2fc:	ea4c 0c01 	orr.w	ip, ip, r1
3000b300:	4962      	ldr	r1, [pc, #392]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b302:	ea4c 0707 	orr.w	r7, ip, r7
3000b306:	618f      	str	r7, [r1, #24]
3000b308:	6821      	ldr	r1, [r4, #0]
3000b30a:	780f      	ldrb	r7, [r1, #0]
3000b30c:	2f02      	cmp	r7, #2
3000b30e:	f000 8152 	beq.w	3000b5b6 <rxi316_dram_init+0x7c6>
3000b312:	2f09      	cmp	r7, #9
3000b314:	f000 814f 	beq.w	3000b5b6 <rxi316_dram_init+0x7c6>
3000b318:	68e1      	ldr	r1, [r4, #12]
3000b31a:	2f03      	cmp	r7, #3
3000b31c:	6809      	ldr	r1, [r1, #0]
3000b31e:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000b320:	f001 080f 	and.w	r8, r1, #15
3000b324:	68a1      	ldr	r1, [r4, #8]
3000b326:	6a09      	ldr	r1, [r1, #32]
3000b328:	d005      	beq.n	3000b336 <rxi316_dram_init+0x546>
3000b32a:	ea0a 0701 	and.w	r7, sl, r1
3000b32e:	fa21 f10b 	lsr.w	r1, r1, fp
3000b332:	b107      	cbz	r7, 3000b336 <rxi316_dram_init+0x546>
3000b334:	3101      	adds	r1, #1
3000b336:	f1b9 0f02 	cmp.w	r9, #2
3000b33a:	f000 8154 	beq.w	3000b5e6 <rxi316_dram_init+0x7f6>
3000b33e:	f1b9 0f03 	cmp.w	r9, #3
3000b342:	f000 81b6 	beq.w	3000b6b2 <rxi316_dram_init+0x8c2>
3000b346:	f1b9 0f09 	cmp.w	r9, #9
3000b34a:	f000 822f 	beq.w	3000b7ac <rxi316_dram_init+0x9bc>
3000b34e:	f04f 0e00 	mov.w	lr, #0
3000b352:	46f1      	mov	r9, lr
3000b354:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b358:	0109      	lsls	r1, r1, #4
3000b35a:	9f06      	ldr	r7, [sp, #24]
3000b35c:	b2c9      	uxtb	r1, r1
3000b35e:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b362:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b364:	ea41 0108 	orr.w	r1, r1, r8
3000b368:	4339      	orrs	r1, r7
3000b36a:	4f48      	ldr	r7, [pc, #288]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b36c:	ea41 0109 	orr.w	r1, r1, r9
3000b370:	ea41 010e 	orr.w	r1, r1, lr
3000b374:	61f9      	str	r1, [r7, #28]
3000b376:	68a7      	ldr	r7, [r4, #8]
3000b378:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b37a:	fbb4 f1f3 	udiv	r1, r4, r3
3000b37e:	fb03 4311 	mls	r3, r3, r1, r4
3000b382:	b103      	cbz	r3, 3000b386 <rxi316_dram_init+0x596>
3000b384:	3101      	adds	r1, #1
3000b386:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b388:	094b      	lsrs	r3, r1, #5
3000b38a:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b38e:	0912      	lsrs	r2, r2, #4
3000b390:	ea14 0f0a 	tst.w	r4, sl
3000b394:	fa24 f80b 	lsr.w	r8, r4, fp
3000b398:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b39a:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b39e:	bf18      	it	ne
3000b3a0:	f108 0801 	addne.w	r8, r8, #1
3000b3a4:	9306      	str	r3, [sp, #24]
3000b3a6:	ea14 0f0a 	tst.w	r4, sl
3000b3aa:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b3ae:	9b01      	ldr	r3, [sp, #4]
3000b3b0:	fa29 f40b 	lsr.w	r4, r9, fp
3000b3b4:	bf18      	it	ne
3000b3b6:	f10e 0e01 	addne.w	lr, lr, #1
3000b3ba:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b3bc:	ea19 0f0a 	tst.w	r9, sl
3000b3c0:	fa23 f90b 	lsr.w	r9, r3, fp
3000b3c4:	9905      	ldr	r1, [sp, #20]
3000b3c6:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b3ca:	bf18      	it	ne
3000b3cc:	3401      	addne	r4, #1
3000b3ce:	45b9      	cmp	r9, r7
3000b3d0:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b3d4:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b3d8:	f240 80eb 	bls.w	3000b5b2 <rxi316_dram_init+0x7c2>
3000b3dc:	eba9 0707 	sub.w	r7, r9, r7
3000b3e0:	fa27 f40b 	lsr.w	r4, r7, fp
3000b3e4:	ea17 070a 	ands.w	r7, r7, sl
3000b3e8:	d001      	beq.n	3000b3ee <rxi316_dram_init+0x5fe>
3000b3ea:	3401      	adds	r4, #1
3000b3ec:	2700      	movs	r7, #0
3000b3ee:	9b04      	ldr	r3, [sp, #16]
3000b3f0:	b10b      	cbz	r3, 3000b3f6 <rxi316_dram_init+0x606>
3000b3f2:	073b      	lsls	r3, r7, #28
3000b3f4:	9304      	str	r3, [sp, #16]
3000b3f6:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b3fa:	9b06      	ldr	r3, [sp, #24]
3000b3fc:	0289      	lsls	r1, r1, #10
3000b3fe:	02c0      	lsls	r0, r0, #11
3000b400:	f00c 0c04 	and.w	ip, ip, #4
3000b404:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b408:	01d2      	lsls	r2, r2, #7
3000b40a:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b40e:	0236      	lsls	r6, r6, #8
3000b410:	ea4c 0c01 	orr.w	ip, ip, r1
3000b414:	b2d2      	uxtb	r2, r2
3000b416:	026d      	lsls	r5, r5, #9
3000b418:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b41c:	ea4c 0c00 	orr.w	ip, ip, r0
3000b420:	00df      	lsls	r7, r3, #3
3000b422:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b426:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b42a:	ea4c 0202 	orr.w	r2, ip, r2
3000b42e:	f007 0708 	and.w	r7, r7, #8
3000b432:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b436:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b43a:	4332      	orrs	r2, r6
3000b43c:	0524      	lsls	r4, r4, #20
3000b43e:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b442:	4912      	ldr	r1, [pc, #72]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b444:	ea42 0305 	orr.w	r3, r2, r5
3000b448:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b44c:	9a04      	ldr	r2, [sp, #16]
3000b44e:	433b      	orrs	r3, r7
3000b450:	ea43 0308 	orr.w	r3, r3, r8
3000b454:	ea43 030e 	orr.w	r3, r3, lr
3000b458:	4323      	orrs	r3, r4
3000b45a:	4313      	orrs	r3, r2
3000b45c:	9a03      	ldr	r2, [sp, #12]
3000b45e:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b462:	620b      	str	r3, [r1, #32]
3000b464:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b468:	9b02      	ldr	r3, [sp, #8]
3000b46a:	2b01      	cmp	r3, #1
3000b46c:	d002      	beq.n	3000b474 <rxi316_dram_init+0x684>
3000b46e:	9b08      	ldr	r3, [sp, #32]
3000b470:	3303      	adds	r3, #3
3000b472:	9307      	str	r3, [sp, #28]
3000b474:	4b05      	ldr	r3, [pc, #20]	; (3000b48c <rxi316_dram_init+0x69c>)
3000b476:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b47a:	9a07      	ldr	r2, [sp, #28]
3000b47c:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b480:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b484:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b488:	e002      	b.n	3000b490 <rxi316_dram_init+0x6a0>
3000b48a:	bf00      	nop
3000b48c:	41100000 	.word	0x41100000
3000b490:	60da      	str	r2, [r3, #12]
3000b492:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b496:	430a      	orrs	r2, r1
3000b498:	49b3      	ldr	r1, [pc, #716]	; (3000b768 <rxi316_dram_init+0x978>)
3000b49a:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b49e:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b4a2:	0096      	lsls	r6, r2, #2
3000b4a4:	d57f      	bpl.n	3000b5a6 <rxi316_dram_init+0x7b6>
3000b4a6:	691a      	ldr	r2, [r3, #16]
3000b4a8:	4cb0      	ldr	r4, [pc, #704]	; (3000b76c <rxi316_dram_init+0x97c>)
3000b4aa:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b4ae:	611a      	str	r2, [r3, #16]
3000b4b0:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b4b4:	601a      	str	r2, [r3, #0]
3000b4b6:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b4ba:	f043 0303 	orr.w	r3, r3, #3
3000b4be:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b4c2:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b4c6:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b4c8:	2b01      	cmp	r3, #1
3000b4ca:	4ba9      	ldr	r3, [pc, #676]	; (3000b770 <rxi316_dram_init+0x980>)
3000b4cc:	bf0c      	ite	eq
3000b4ce:	4aa9      	ldreq	r2, [pc, #676]	; (3000b774 <rxi316_dram_init+0x984>)
3000b4d0:	4aa9      	ldrne	r2, [pc, #676]	; (3000b778 <rxi316_dram_init+0x988>)
3000b4d2:	601a      	str	r2, [r3, #0]
3000b4d4:	2203      	movs	r2, #3
3000b4d6:	68e0      	ldr	r0, [r4, #12]
3000b4d8:	4ba8      	ldr	r3, [pc, #672]	; (3000b77c <rxi316_dram_init+0x98c>)
3000b4da:	4003      	ands	r3, r0
3000b4dc:	60e3      	str	r3, [r4, #12]
3000b4de:	2308      	movs	r3, #8
3000b4e0:	62a1      	str	r1, [r4, #40]	; 0x28
3000b4e2:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b4e4:	6023      	str	r3, [r4, #0]
3000b4e6:	6823      	ldr	r3, [r4, #0]
3000b4e8:	071d      	lsls	r5, r3, #28
3000b4ea:	d5fc      	bpl.n	3000b4e6 <rxi316_dram_init+0x6f6>
3000b4ec:	4ba4      	ldr	r3, [pc, #656]	; (3000b780 <rxi316_dram_init+0x990>)
3000b4ee:	2001      	movs	r0, #1
3000b4f0:	4798      	blx	r3
3000b4f2:	68e2      	ldr	r2, [r4, #12]
3000b4f4:	4ba1      	ldr	r3, [pc, #644]	; (3000b77c <rxi316_dram_init+0x98c>)
3000b4f6:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b4fa:	4013      	ands	r3, r2
3000b4fc:	2203      	movs	r2, #3
3000b4fe:	60e3      	str	r3, [r4, #12]
3000b500:	2308      	movs	r3, #8
3000b502:	62a1      	str	r1, [r4, #40]	; 0x28
3000b504:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b506:	4a99      	ldr	r2, [pc, #612]	; (3000b76c <rxi316_dram_init+0x97c>)
3000b508:	6023      	str	r3, [r4, #0]
3000b50a:	6813      	ldr	r3, [r2, #0]
3000b50c:	071c      	lsls	r4, r3, #28
3000b50e:	d5fc      	bpl.n	3000b50a <rxi316_dram_init+0x71a>
3000b510:	68d0      	ldr	r0, [r2, #12]
3000b512:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b516:	4b99      	ldr	r3, [pc, #612]	; (3000b77c <rxi316_dram_init+0x98c>)
3000b518:	4003      	ands	r3, r0
3000b51a:	2008      	movs	r0, #8
3000b51c:	60d3      	str	r3, [r2, #12]
3000b51e:	6291      	str	r1, [r2, #40]	; 0x28
3000b520:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b522:	4992      	ldr	r1, [pc, #584]	; (3000b76c <rxi316_dram_init+0x97c>)
3000b524:	f023 0302 	bic.w	r3, r3, #2
3000b528:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b52a:	6010      	str	r0, [r2, #0]
3000b52c:	680b      	ldr	r3, [r1, #0]
3000b52e:	0718      	lsls	r0, r3, #28
3000b530:	d5fc      	bpl.n	3000b52c <rxi316_dram_init+0x73c>
3000b532:	4a8e      	ldr	r2, [pc, #568]	; (3000b76c <rxi316_dram_init+0x97c>)
3000b534:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b538:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b53c:	60d3      	str	r3, [r2, #12]
3000b53e:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b540:	6913      	ldr	r3, [r2, #16]
3000b542:	4318      	orrs	r0, r3
3000b544:	6110      	str	r0, [r2, #16]
3000b546:	6913      	ldr	r3, [r2, #16]
3000b548:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b54c:	6113      	str	r3, [r2, #16]
3000b54e:	6011      	str	r1, [r2, #0]
3000b550:	68d3      	ldr	r3, [r2, #12]
3000b552:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b556:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b55a:	d1f9      	bne.n	3000b550 <rxi316_dram_init+0x760>
3000b55c:	b00f      	add	sp, #60	; 0x3c
3000b55e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b562:	2a01      	cmp	r2, #1
3000b564:	f000 80d1 	beq.w	3000b70a <rxi316_dram_init+0x91a>
3000b568:	2a02      	cmp	r2, #2
3000b56a:	f000 8139 	beq.w	3000b7e0 <rxi316_dram_init+0x9f0>
3000b56e:	2a03      	cmp	r2, #3
3000b570:	f47f ace4 	bne.w	3000af3c <rxi316_dram_init+0x14c>
3000b574:	686a      	ldr	r2, [r5, #4]
3000b576:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b57a:	606a      	str	r2, [r5, #4]
3000b57c:	e4de      	b.n	3000af3c <rxi316_dram_init+0x14c>
3000b57e:	2a00      	cmp	r2, #0
3000b580:	f43f ad6a 	beq.w	3000b058 <rxi316_dram_init+0x268>
3000b584:	2203      	movs	r2, #3
3000b586:	4310      	orrs	r0, r2
3000b588:	68a2      	ldr	r2, [r4, #8]
3000b58a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b58c:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b58e:	013f      	lsls	r7, r7, #4
3000b590:	2d00      	cmp	r5, #0
3000b592:	f000 80d7 	beq.w	3000b744 <rxi316_dram_init+0x954>
3000b596:	1e91      	subs	r1, r2, #2
3000b598:	2906      	cmp	r1, #6
3000b59a:	f200 80b0 	bhi.w	3000b6fe <rxi316_dram_init+0x90e>
3000b59e:	1e55      	subs	r5, r2, #1
3000b5a0:	2100      	movs	r1, #0
3000b5a2:	026d      	lsls	r5, r5, #9
3000b5a4:	e55f      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b5a6:	2201      	movs	r2, #1
3000b5a8:	601a      	str	r2, [r3, #0]
3000b5aa:	681a      	ldr	r2, [r3, #0]
3000b5ac:	07d2      	lsls	r2, r2, #31
3000b5ae:	d5fc      	bpl.n	3000b5aa <rxi316_dram_init+0x7ba>
3000b5b0:	e7bf      	b.n	3000b532 <rxi316_dram_init+0x742>
3000b5b2:	2701      	movs	r7, #1
3000b5b4:	e71b      	b.n	3000b3ee <rxi316_dram_init+0x5fe>
3000b5b6:	68a1      	ldr	r1, [r4, #8]
3000b5b8:	2f03      	cmp	r7, #3
3000b5ba:	6a09      	ldr	r1, [r1, #32]
3000b5bc:	f000 80bf 	beq.w	3000b73e <rxi316_dram_init+0x94e>
3000b5c0:	2f02      	cmp	r7, #2
3000b5c2:	f040 8118 	bne.w	3000b7f6 <rxi316_dram_init+0xa06>
3000b5c6:	fbb1 fcf3 	udiv	ip, r1, r3
3000b5ca:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b5ce:	4661      	mov	r1, ip
3000b5d0:	f1be 0f00 	cmp.w	lr, #0
3000b5d4:	f000 80a9 	beq.w	3000b72a <rxi316_dram_init+0x93a>
3000b5d8:	f1b9 0f02 	cmp.w	r9, #2
3000b5dc:	f101 0101 	add.w	r1, r1, #1
3000b5e0:	46b8      	mov	r8, r7
3000b5e2:	f47f aeac 	bne.w	3000b33e <rxi316_dram_init+0x54e>
3000b5e6:	4f67      	ldr	r7, [pc, #412]	; (3000b784 <rxi316_dram_init+0x994>)
3000b5e8:	f04f 0e00 	mov.w	lr, #0
3000b5ec:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b788 <rxi316_dram_init+0x998>
3000b5f0:	fbb7 f7f3 	udiv	r7, r7, r3
3000b5f4:	fbb9 f9f3 	udiv	r9, r9, r3
3000b5f8:	09ff      	lsrs	r7, r7, #7
3000b5fa:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b5fe:	3701      	adds	r7, #1
3000b600:	f109 0c01 	add.w	ip, r9, #1
3000b604:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b78c <rxi316_dram_init+0x99c>
3000b608:	023f      	lsls	r7, r7, #8
3000b60a:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b60e:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b612:	9709      	str	r7, [sp, #36]	; 0x24
3000b614:	e6a0      	b.n	3000b358 <rxi316_dram_init+0x568>
3000b616:	f8cd c018 	str.w	ip, [sp, #24]
3000b61a:	e5bf      	b.n	3000b19c <rxi316_dram_init+0x3ac>
3000b61c:	38c8      	subs	r0, #200	; 0xc8
3000b61e:	e596      	b.n	3000b14e <rxi316_dram_init+0x35e>
3000b620:	9105      	str	r1, [sp, #20]
3000b622:	e58b      	b.n	3000b13c <rxi316_dram_init+0x34c>
3000b624:	fa22 f00b 	lsr.w	r0, r2, fp
3000b628:	e547      	b.n	3000b0ba <rxi316_dram_init+0x2ca>
3000b62a:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b62e:	ea1a 0f07 	tst.w	sl, r7
3000b632:	fa27 f20b 	lsr.w	r2, r7, fp
3000b636:	d000      	beq.n	3000b63a <rxi316_dram_init+0x84a>
3000b638:	3201      	adds	r2, #1
3000b63a:	2e02      	cmp	r6, #2
3000b63c:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b640:	d10f      	bne.n	3000b662 <rxi316_dram_init+0x872>
3000b642:	9e08      	ldr	r6, [sp, #32]
3000b644:	3e02      	subs	r6, #2
3000b646:	fa26 f60b 	lsr.w	r6, r6, fp
3000b64a:	4437      	add	r7, r6
3000b64c:	fbb7 f6f3 	udiv	r6, r7, r3
3000b650:	fb03 7716 	mls	r7, r3, r6, r7
3000b654:	2f00      	cmp	r7, #0
3000b656:	f43f ad52 	beq.w	3000b0fe <rxi316_dram_init+0x30e>
3000b65a:	3601      	adds	r6, #1
3000b65c:	e54f      	b.n	3000b0fe <rxi316_dram_init+0x30e>
3000b65e:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b662:	fbb7 f6f3 	udiv	r6, r7, r3
3000b666:	fb03 7716 	mls	r7, r3, r6, r7
3000b66a:	2f00      	cmp	r7, #0
3000b66c:	f43f ad47 	beq.w	3000b0fe <rxi316_dram_init+0x30e>
3000b670:	3601      	adds	r6, #1
3000b672:	e544      	b.n	3000b0fe <rxi316_dram_init+0x30e>
3000b674:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b676:	46a8      	mov	r8, r5
3000b678:	9d08      	ldr	r5, [sp, #32]
3000b67a:	44a8      	add	r8, r5
3000b67c:	e611      	b.n	3000b2a2 <rxi316_dram_init+0x4b2>
3000b67e:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b680:	1f50      	subs	r0, r2, #5
3000b682:	00c0      	lsls	r0, r0, #3
3000b684:	68e2      	ldr	r2, [r4, #12]
3000b686:	6812      	ldr	r2, [r2, #0]
3000b688:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b68a:	fab2 f282 	clz	r2, r2
3000b68e:	0952      	lsrs	r2, r2, #5
3000b690:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b694:	4301      	orrs	r1, r0
3000b696:	e4fd      	b.n	3000b094 <rxi316_dram_init+0x2a4>
3000b698:	f1a1 0502 	sub.w	r5, r1, #2
3000b69c:	2909      	cmp	r1, #9
3000b69e:	fab5 f585 	clz	r5, r5
3000b6a2:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b6a6:	f000 8099 	beq.w	3000b7dc <rxi316_dram_init+0x9ec>
3000b6aa:	462a      	mov	r2, r5
3000b6ac:	e4d1      	b.n	3000b052 <rxi316_dram_init+0x262>
3000b6ae:	2000      	movs	r0, #0
3000b6b0:	e7e8      	b.n	3000b684 <rxi316_dram_init+0x894>
3000b6b2:	f242 7710 	movw	r7, #10000	; 0x2710
3000b6b6:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b790 <rxi316_dram_init+0x9a0>
3000b6ba:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b788 <rxi316_dram_init+0x998>
3000b6be:	fbb7 fcf3 	udiv	ip, r7, r3
3000b6c2:	9f05      	ldr	r7, [sp, #20]
3000b6c4:	4467      	add	r7, ip
3000b6c6:	fbb9 f9f3 	udiv	r9, r9, r3
3000b6ca:	fbbe fef3 	udiv	lr, lr, r3
3000b6ce:	09ff      	lsrs	r7, r7, #7
3000b6d0:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b6d4:	3701      	adds	r7, #1
3000b6d6:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b6da:	f109 0c01 	add.w	ip, r9, #1
3000b6de:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b78c <rxi316_dram_init+0x99c>
3000b6e2:	023f      	lsls	r7, r7, #8
3000b6e4:	f10e 0e01 	add.w	lr, lr, #1
3000b6e8:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b6ec:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b6f0:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b6f4:	9709      	str	r7, [sp, #36]	; 0x24
3000b6f6:	e62f      	b.n	3000b358 <rxi316_dram_init+0x568>
3000b6f8:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b6fc:	e5d1      	b.n	3000b2a2 <rxi316_dram_init+0x4b2>
3000b6fe:	2100      	movs	r1, #0
3000b700:	460d      	mov	r5, r1
3000b702:	e4b0      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b704:	3a07      	subs	r2, #7
3000b706:	f7ff bbb8 	b.w	3000ae7a <rxi316_dram_init+0x8a>
3000b70a:	686a      	ldr	r2, [r5, #4]
3000b70c:	f042 0220 	orr.w	r2, r2, #32
3000b710:	606a      	str	r2, [r5, #4]
3000b712:	e413      	b.n	3000af3c <rxi316_dram_init+0x14c>
3000b714:	2204      	movs	r2, #4
3000b716:	2102      	movs	r1, #2
3000b718:	2500      	movs	r5, #0
3000b71a:	9201      	str	r2, [sp, #4]
3000b71c:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b720:	f7ff bb94 	b.w	3000ae4c <rxi316_dram_init+0x5c>
3000b724:	3a08      	subs	r2, #8
3000b726:	f7ff bba8 	b.w	3000ae7a <rxi316_dram_init+0x8a>
3000b72a:	46b8      	mov	r8, r7
3000b72c:	e603      	b.n	3000b336 <rxi316_dram_init+0x546>
3000b72e:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b730:	2a04      	cmp	r2, #4
3000b732:	d90c      	bls.n	3000b74e <rxi316_dram_init+0x95e>
3000b734:	2a0b      	cmp	r2, #11
3000b736:	d86a      	bhi.n	3000b80e <rxi316_dram_init+0xa1e>
3000b738:	1f17      	subs	r7, r2, #4
3000b73a:	013f      	lsls	r7, r7, #4
3000b73c:	e008      	b.n	3000b750 <rxi316_dram_init+0x960>
3000b73e:	f04f 0802 	mov.w	r8, #2
3000b742:	e5f8      	b.n	3000b336 <rxi316_dram_init+0x546>
3000b744:	2903      	cmp	r1, #3
3000b746:	d005      	beq.n	3000b754 <rxi316_dram_init+0x964>
3000b748:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b74c:	e48b      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b74e:	2710      	movs	r7, #16
3000b750:	68a2      	ldr	r2, [r4, #8]
3000b752:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b754:	2a04      	cmp	r2, #4
3000b756:	d949      	bls.n	3000b7ec <rxi316_dram_init+0x9fc>
3000b758:	1f51      	subs	r1, r2, #5
3000b75a:	2903      	cmp	r1, #3
3000b75c:	d84e      	bhi.n	3000b7fc <rxi316_dram_init+0xa0c>
3000b75e:	1f15      	subs	r5, r2, #4
3000b760:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b764:	026d      	lsls	r5, r5, #9
3000b766:	e47e      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b768:	42008000 	.word	0x42008000
3000b76c:	41100000 	.word	0x41100000
3000b770:	41011000 	.word	0x41011000
3000b774:	44000103 	.word	0x44000103
3000b778:	44000021 	.word	0x44000021
3000b77c:	fff8fffc 	.word	0xfff8fffc
3000b780:	00009b2d 	.word	0x00009b2d
3000b784:	00061a80 	.word	0x00061a80
3000b788:	0bebc200 	.word	0x0bebc200
3000b78c:	003ff000 	.word	0x003ff000
3000b790:	1dcd6500 	.word	0x1dcd6500
3000b794:	f1a1 0c02 	sub.w	ip, r1, #2
3000b798:	f1bc 0f01 	cmp.w	ip, #1
3000b79c:	d903      	bls.n	3000b7a6 <rxi316_dram_init+0x9b6>
3000b79e:	f1be 0f00 	cmp.w	lr, #0
3000b7a2:	f43f ac56 	beq.w	3000b052 <rxi316_dram_init+0x262>
3000b7a6:	f040 0002 	orr.w	r0, r0, #2
3000b7aa:	e452      	b.n	3000b052 <rxi316_dram_init+0x262>
3000b7ac:	4f1d      	ldr	r7, [pc, #116]	; (3000b824 <rxi316_dram_init+0xa34>)
3000b7ae:	f04f 0e00 	mov.w	lr, #0
3000b7b2:	fbb7 f9f3 	udiv	r9, r7, r3
3000b7b6:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b7ba:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b7be:	f109 0701 	add.w	r7, r9, #1
3000b7c2:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b828 <rxi316_dram_init+0xa38>
3000b7c6:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b7ca:	e5c5      	b.n	3000b358 <rxi316_dram_init+0x568>
3000b7cc:	f105 0802 	add.w	r8, r5, #2
3000b7d0:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b7d2:	eba8 0805 	sub.w	r8, r8, r5
3000b7d6:	9d08      	ldr	r5, [sp, #32]
3000b7d8:	44a8      	add	r8, r5
3000b7da:	e562      	b.n	3000b2a2 <rxi316_dram_init+0x4b2>
3000b7dc:	2204      	movs	r2, #4
3000b7de:	e6d2      	b.n	3000b586 <rxi316_dram_init+0x796>
3000b7e0:	686a      	ldr	r2, [r5, #4]
3000b7e2:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b7e6:	606a      	str	r2, [r5, #4]
3000b7e8:	f7ff bba8 	b.w	3000af3c <rxi316_dram_init+0x14c>
3000b7ec:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b7f0:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b7f4:	e437      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b7f6:	f04f 0802 	mov.w	r8, #2
3000b7fa:	e596      	b.n	3000b32a <rxi316_dram_init+0x53a>
3000b7fc:	f1a2 0109 	sub.w	r1, r2, #9
3000b800:	2905      	cmp	r1, #5
3000b802:	d80b      	bhi.n	3000b81c <rxi316_dram_init+0xa2c>
3000b804:	0855      	lsrs	r5, r2, #1
3000b806:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b80a:	026d      	lsls	r5, r5, #9
3000b80c:	e42b      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b80e:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b810:	f040 0004 	orr.w	r0, r0, #4
3000b814:	f1a2 070c 	sub.w	r7, r2, #12
3000b818:	013f      	lsls	r7, r7, #4
3000b81a:	e799      	b.n	3000b750 <rxi316_dram_init+0x960>
3000b81c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b820:	2500      	movs	r5, #0
3000b822:	e420      	b.n	3000b066 <rxi316_dram_init+0x276>
3000b824:	0bebc200 	.word	0x0bebc200
3000b828:	003ff000 	.word	0x003ff000

3000b82c <rxi316_ftend_init>:
3000b82c:	2300      	movs	r3, #0
3000b82e:	b510      	push	{r4, lr}
3000b830:	4c0c      	ldr	r4, [pc, #48]	; (3000b864 <rxi316_ftend_init+0x38>)
3000b832:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b836:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b83a:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b83e:	f7fe fe51 	bl	3000a4e4 <ddr_init_index>
3000b842:	4a09      	ldr	r2, [pc, #36]	; (3000b868 <rxi316_ftend_init+0x3c>)
3000b844:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b848:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b84c:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b850:	68cb      	ldr	r3, [r1, #12]
3000b852:	689b      	ldr	r3, [r3, #8]
3000b854:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b856:	3301      	adds	r3, #1
3000b858:	b2db      	uxtb	r3, r3
3000b85a:	4313      	orrs	r3, r2
3000b85c:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b860:	bd10      	pop	{r4, pc}
3000b862:	bf00      	nop
3000b864:	41101000 	.word	0x41101000
3000b868:	3000e7f8 	.word	0x3000e7f8

3000b86c <rxi316_DynSre_init>:
3000b86c:	2901      	cmp	r1, #1
3000b86e:	d013      	beq.n	3000b898 <rxi316_DynSre_init+0x2c>
3000b870:	4a12      	ldr	r2, [pc, #72]	; (3000b8bc <rxi316_DynSre_init+0x50>)
3000b872:	6893      	ldr	r3, [r2, #8]
3000b874:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b878:	6093      	str	r3, [r2, #8]
3000b87a:	4b10      	ldr	r3, [pc, #64]	; (3000b8bc <rxi316_DynSre_init+0x50>)
3000b87c:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b880:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b884:	625a      	str	r2, [r3, #36]	; 0x24
3000b886:	629a      	str	r2, [r3, #40]	; 0x28
3000b888:	69da      	ldr	r2, [r3, #28]
3000b88a:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b88e:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b892:	61da      	str	r2, [r3, #28]
3000b894:	6019      	str	r1, [r3, #0]
3000b896:	4770      	bx	lr
3000b898:	4b08      	ldr	r3, [pc, #32]	; (3000b8bc <rxi316_DynSre_init+0x50>)
3000b89a:	b280      	uxth	r0, r0
3000b89c:	4a08      	ldr	r2, [pc, #32]	; (3000b8c0 <rxi316_DynSre_init+0x54>)
3000b89e:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b8a2:	400a      	ands	r2, r1
3000b8a4:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b8a8:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b8ac:	4310      	orrs	r0, r2
3000b8ae:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b8b2:	689a      	ldr	r2, [r3, #8]
3000b8b4:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b8b8:	609a      	str	r2, [r3, #8]
3000b8ba:	e7de      	b.n	3000b87a <rxi316_DynSre_init+0xe>
3000b8bc:	41100000 	.word	0x41100000
3000b8c0:	ffff0000 	.word	0xffff0000

3000b8c4 <ddr_init>:
3000b8c4:	b500      	push	{lr}
3000b8c6:	b083      	sub	sp, #12
3000b8c8:	f7fe fe0c 	bl	3000a4e4 <ddr_init_index>
3000b8cc:	4b06      	ldr	r3, [pc, #24]	; (3000b8e8 <ddr_init+0x24>)
3000b8ce:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b8d2:	9001      	str	r0, [sp, #4]
3000b8d4:	f7fe fe30 	bl	3000a538 <rxi316_perf_tune>
3000b8d8:	9801      	ldr	r0, [sp, #4]
3000b8da:	f7ff fa89 	bl	3000adf0 <rxi316_dram_init>
3000b8de:	b003      	add	sp, #12
3000b8e0:	f85d eb04 	ldr.w	lr, [sp], #4
3000b8e4:	f7ff bfa2 	b.w	3000b82c <rxi316_ftend_init>
3000b8e8:	3000e7f8 	.word	0x3000e7f8

3000b8ec <SWR_MEM>:
3000b8ec:	2801      	cmp	r0, #1
3000b8ee:	d00e      	beq.n	3000b90e <SWR_MEM+0x22>
3000b8f0:	4b31      	ldr	r3, [pc, #196]	; (3000b9b8 <SWR_MEM+0xcc>)
3000b8f2:	2000      	movs	r0, #0
3000b8f4:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b8f8:	f022 0201 	bic.w	r2, r2, #1
3000b8fc:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b900:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b904:	f022 0202 	bic.w	r2, r2, #2
3000b908:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b90c:	4770      	bx	lr
3000b90e:	b510      	push	{r4, lr}
3000b910:	4c29      	ldr	r4, [pc, #164]	; (3000b9b8 <SWR_MEM+0xcc>)
3000b912:	4a2a      	ldr	r2, [pc, #168]	; (3000b9bc <SWR_MEM+0xd0>)
3000b914:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b918:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b91c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b920:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b924:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b928:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b92c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b930:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b934:	f023 0303 	bic.w	r3, r3, #3
3000b938:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b93c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b940:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b944:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b948:	4790      	blx	r2
3000b94a:	2803      	cmp	r0, #3
3000b94c:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b950:	d026      	beq.n	3000b9a0 <SWR_MEM+0xb4>
3000b952:	f043 0303 	orr.w	r3, r3, #3
3000b956:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b95a:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b95e:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b962:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b966:	4c14      	ldr	r4, [pc, #80]	; (3000b9b8 <SWR_MEM+0xcc>)
3000b968:	4b15      	ldr	r3, [pc, #84]	; (3000b9c0 <SWR_MEM+0xd4>)
3000b96a:	4798      	blx	r3
3000b96c:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b970:	4a14      	ldr	r2, [pc, #80]	; (3000b9c4 <SWR_MEM+0xd8>)
3000b972:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b976:	f043 0302 	orr.w	r3, r3, #2
3000b97a:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b97e:	4790      	blx	r2
3000b980:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b984:	4b10      	ldr	r3, [pc, #64]	; (3000b9c8 <SWR_MEM+0xdc>)
3000b986:	f042 0201 	orr.w	r2, r2, #1
3000b98a:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b98e:	e001      	b.n	3000b994 <SWR_MEM+0xa8>
3000b990:	3b01      	subs	r3, #1
3000b992:	d00e      	beq.n	3000b9b2 <SWR_MEM+0xc6>
3000b994:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b998:	03d2      	lsls	r2, r2, #15
3000b99a:	d5f9      	bpl.n	3000b990 <SWR_MEM+0xa4>
3000b99c:	2000      	movs	r0, #0
3000b99e:	bd10      	pop	{r4, pc}
3000b9a0:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b9a4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b9a8:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b9ac:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b9b0:	e7d9      	b.n	3000b966 <SWR_MEM+0x7a>
3000b9b2:	2001      	movs	r0, #1
3000b9b4:	bd10      	pop	{r4, pc}
3000b9b6:	bf00      	nop
3000b9b8:	42008800 	.word	0x42008800
3000b9bc:	30007e91 	.word	0x30007e91
3000b9c0:	3000bb8d 	.word	0x3000bb8d
3000b9c4:	00009b2d 	.word	0x00009b2d
3000b9c8:	000186a0 	.word	0x000186a0

3000b9cc <SWR_MEM_Manual>:
3000b9cc:	4a06      	ldr	r2, [pc, #24]	; (3000b9e8 <SWR_MEM_Manual+0x1c>)
3000b9ce:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b9d2:	b120      	cbz	r0, 3000b9de <SWR_MEM_Manual+0x12>
3000b9d4:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b9d8:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b9dc:	4770      	bx	lr
3000b9de:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b9e2:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b9e6:	4770      	bx	lr
3000b9e8:	42008800 	.word	0x42008800

3000b9ec <SWR_MEM_Mode_Set>:
3000b9ec:	2801      	cmp	r0, #1
3000b9ee:	b538      	push	{r3, r4, r5, lr}
3000b9f0:	d807      	bhi.n	3000ba02 <SWR_MEM_Mode_Set+0x16>
3000b9f2:	d10a      	bne.n	3000ba0a <SWR_MEM_Mode_Set+0x1e>
3000b9f4:	4c22      	ldr	r4, [pc, #136]	; (3000ba80 <SWR_MEM_Mode_Set+0x94>)
3000b9f6:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b9fa:	035b      	lsls	r3, r3, #13
3000b9fc:	d51b      	bpl.n	3000ba36 <SWR_MEM_Mode_Set+0x4a>
3000b9fe:	2000      	movs	r0, #0
3000ba00:	bd38      	pop	{r3, r4, r5, pc}
3000ba02:	217d      	movs	r1, #125	; 0x7d
3000ba04:	481f      	ldr	r0, [pc, #124]	; (3000ba84 <SWR_MEM_Mode_Set+0x98>)
3000ba06:	f000 fbb3 	bl	3000c170 <__io_assert_failed_veneer>
3000ba0a:	491d      	ldr	r1, [pc, #116]	; (3000ba80 <SWR_MEM_Mode_Set+0x94>)
3000ba0c:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000ba10:	0358      	lsls	r0, r3, #13
3000ba12:	d5f4      	bpl.n	3000b9fe <SWR_MEM_Mode_Set+0x12>
3000ba14:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000ba18:	f242 7310 	movw	r3, #10000	; 0x2710
3000ba1c:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000ba20:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000ba24:	e001      	b.n	3000ba2a <SWR_MEM_Mode_Set+0x3e>
3000ba26:	3b01      	subs	r3, #1
3000ba28:	d027      	beq.n	3000ba7a <SWR_MEM_Mode_Set+0x8e>
3000ba2a:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000ba2e:	0352      	lsls	r2, r2, #13
3000ba30:	d4f9      	bmi.n	3000ba26 <SWR_MEM_Mode_Set+0x3a>
3000ba32:	2000      	movs	r0, #0
3000ba34:	e7e4      	b.n	3000ba00 <SWR_MEM_Mode_Set+0x14>
3000ba36:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000ba3a:	2064      	movs	r0, #100	; 0x64
3000ba3c:	4d12      	ldr	r5, [pc, #72]	; (3000ba88 <SWR_MEM_Mode_Set+0x9c>)
3000ba3e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000ba42:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000ba46:	47a8      	blx	r5
3000ba48:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000ba4c:	2064      	movs	r0, #100	; 0x64
3000ba4e:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000ba52:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000ba56:	47a8      	blx	r5
3000ba58:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000ba5c:	f242 7310 	movw	r3, #10000	; 0x2710
3000ba60:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000ba64:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000ba68:	e001      	b.n	3000ba6e <SWR_MEM_Mode_Set+0x82>
3000ba6a:	3b01      	subs	r3, #1
3000ba6c:	d005      	beq.n	3000ba7a <SWR_MEM_Mode_Set+0x8e>
3000ba6e:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000ba72:	0355      	lsls	r5, r2, #13
3000ba74:	d5f9      	bpl.n	3000ba6a <SWR_MEM_Mode_Set+0x7e>
3000ba76:	2000      	movs	r0, #0
3000ba78:	e7c2      	b.n	3000ba00 <SWR_MEM_Mode_Set+0x14>
3000ba7a:	2001      	movs	r0, #1
3000ba7c:	bd38      	pop	{r3, r4, r5, pc}
3000ba7e:	bf00      	nop
3000ba80:	42008800 	.word	0x42008800
3000ba84:	3000d618 	.word	0x3000d618
3000ba88:	00009b2d 	.word	0x00009b2d

3000ba8c <SWR_AUDIO>:
3000ba8c:	2801      	cmp	r0, #1
3000ba8e:	d00e      	beq.n	3000baae <SWR_AUDIO+0x22>
3000ba90:	4b1e      	ldr	r3, [pc, #120]	; (3000bb0c <SWR_AUDIO+0x80>)
3000ba92:	2000      	movs	r0, #0
3000ba94:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000ba98:	f022 0201 	bic.w	r2, r2, #1
3000ba9c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000baa0:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000baa4:	f022 0202 	bic.w	r2, r2, #2
3000baa8:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000baac:	4770      	bx	lr
3000baae:	b510      	push	{r4, lr}
3000bab0:	4c16      	ldr	r4, [pc, #88]	; (3000bb0c <SWR_AUDIO+0x80>)
3000bab2:	4b17      	ldr	r3, [pc, #92]	; (3000bb10 <SWR_AUDIO+0x84>)
3000bab4:	4798      	blx	r3
3000bab6:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000baba:	4a16      	ldr	r2, [pc, #88]	; (3000bb14 <SWR_AUDIO+0x88>)
3000babc:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000bac0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000bac4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000bac8:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000bacc:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000bad0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000bad4:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000bad8:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000badc:	f043 0302 	orr.w	r3, r3, #2
3000bae0:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000bae4:	4790      	blx	r2
3000bae6:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000baea:	4b0b      	ldr	r3, [pc, #44]	; (3000bb18 <SWR_AUDIO+0x8c>)
3000baec:	f042 0201 	orr.w	r2, r2, #1
3000baf0:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000baf4:	e001      	b.n	3000bafa <SWR_AUDIO+0x6e>
3000baf6:	3b01      	subs	r3, #1
3000baf8:	d005      	beq.n	3000bb06 <SWR_AUDIO+0x7a>
3000bafa:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000bafe:	03d2      	lsls	r2, r2, #15
3000bb00:	d5f9      	bpl.n	3000baf6 <SWR_AUDIO+0x6a>
3000bb02:	2000      	movs	r0, #0
3000bb04:	bd10      	pop	{r4, pc}
3000bb06:	2001      	movs	r0, #1
3000bb08:	bd10      	pop	{r4, pc}
3000bb0a:	bf00      	nop
3000bb0c:	42008800 	.word	0x42008800
3000bb10:	3000bcdd 	.word	0x3000bcdd
3000bb14:	00009b2d 	.word	0x00009b2d
3000bb18:	000186a0 	.word	0x000186a0

3000bb1c <SWR_AUDIO_Manual>:
3000bb1c:	4a06      	ldr	r2, [pc, #24]	; (3000bb38 <SWR_AUDIO_Manual+0x1c>)
3000bb1e:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000bb22:	b120      	cbz	r0, 3000bb2e <SWR_AUDIO_Manual+0x12>
3000bb24:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000bb28:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bb2c:	4770      	bx	lr
3000bb2e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000bb32:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000bb36:	4770      	bx	lr
3000bb38:	42008800 	.word	0x42008800

3000bb3c <SWR_BST_MODE_Set>:
3000bb3c:	4a06      	ldr	r2, [pc, #24]	; (3000bb58 <SWR_BST_MODE_Set+0x1c>)
3000bb3e:	6813      	ldr	r3, [r2, #0]
3000bb40:	b118      	cbz	r0, 3000bb4a <SWR_BST_MODE_Set+0xe>
3000bb42:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000bb46:	6013      	str	r3, [r2, #0]
3000bb48:	4770      	bx	lr
3000bb4a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000bb4e:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000bb52:	6013      	str	r3, [r2, #0]
3000bb54:	4770      	bx	lr
3000bb56:	bf00      	nop
3000bb58:	42008100 	.word	0x42008100

3000bb5c <SWR_Calib_DCore>:
3000bb5c:	4b09      	ldr	r3, [pc, #36]	; (3000bb84 <SWR_Calib_DCore+0x28>)
3000bb5e:	f240 3231 	movw	r2, #817	; 0x331
3000bb62:	4909      	ldr	r1, [pc, #36]	; (3000bb88 <SWR_Calib_DCore+0x2c>)
3000bb64:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000bb66:	4001      	ands	r1, r0
3000bb68:	430a      	orrs	r2, r1
3000bb6a:	651a      	str	r2, [r3, #80]	; 0x50
3000bb6c:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000bb6e:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bb72:	641a      	str	r2, [r3, #64]	; 0x40
3000bb74:	685a      	ldr	r2, [r3, #4]
3000bb76:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000bb7a:	f042 0204 	orr.w	r2, r2, #4
3000bb7e:	605a      	str	r2, [r3, #4]
3000bb80:	4770      	bx	lr
3000bb82:	bf00      	nop
3000bb84:	42008100 	.word	0x42008100
3000bb88:	fffffccc 	.word	0xfffffccc

3000bb8c <SWR_Calib_MEM>:
3000bb8c:	4b4e      	ldr	r3, [pc, #312]	; (3000bcc8 <SWR_Calib_MEM+0x13c>)
3000bb8e:	f240 3231 	movw	r2, #817	; 0x331
3000bb92:	494e      	ldr	r1, [pc, #312]	; (3000bccc <SWR_Calib_MEM+0x140>)
3000bb94:	2000      	movs	r0, #0
3000bb96:	b510      	push	{r4, lr}
3000bb98:	f8d3 40e0 	ldr.w	r4, [r3, #224]	; 0xe0
3000bb9c:	b082      	sub	sp, #8
3000bb9e:	4021      	ands	r1, r4
3000bba0:	430a      	orrs	r2, r1
3000bba2:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bba6:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000bbaa:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000bbae:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bbb2:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000bbb6:	2307      	movs	r3, #7
3000bbb8:	f88d 0007 	strb.w	r0, [sp, #7]
3000bbbc:	eb0d 0203 	add.w	r2, sp, r3
3000bbc0:	f7fd fbca 	bl	30009358 <EFUSE_PMAP_READ8>
3000bbc4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bbc8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bbcc:	2af0      	cmp	r2, #240	; 0xf0
3000bbce:	d15b      	bne.n	3000bc88 <SWR_Calib_MEM+0xfc>
3000bbd0:	f003 030f 	and.w	r3, r3, #15
3000bbd4:	2b0f      	cmp	r3, #15
3000bbd6:	d053      	beq.n	3000bc80 <SWR_Calib_MEM+0xf4>
3000bbd8:	2102      	movs	r1, #2
3000bbda:	4c3d      	ldr	r4, [pc, #244]	; (3000bcd0 <SWR_Calib_MEM+0x144>)
3000bbdc:	2307      	movs	r3, #7
3000bbde:	f10d 0206 	add.w	r2, sp, #6
3000bbe2:	2000      	movs	r0, #0
3000bbe4:	7021      	strb	r1, [r4, #0]
3000bbe6:	f240 71e4 	movw	r1, #2020	; 0x7e4
3000bbea:	f7fd fbb5 	bl	30009358 <EFUSE_PMAP_READ8>
3000bbee:	7822      	ldrb	r2, [r4, #0]
3000bbf0:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000bbf4:	2a03      	cmp	r2, #3
3000bbf6:	d05d      	beq.n	3000bcb4 <SWR_Calib_MEM+0x128>
3000bbf8:	2a02      	cmp	r2, #2
3000bbfa:	bf0c      	ite	eq
3000bbfc:	f003 030f 	andeq.w	r3, r3, #15
3000bc00:	2300      	movne	r3, #0
3000bc02:	4931      	ldr	r1, [pc, #196]	; (3000bcc8 <SWR_Calib_MEM+0x13c>)
3000bc04:	4833      	ldr	r0, [pc, #204]	; (3000bcd4 <SWR_Calib_MEM+0x148>)
3000bc06:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000bc0a:	f022 020f 	bic.w	r2, r2, #15
3000bc0e:	4313      	orrs	r3, r2
3000bc10:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bc14:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bc16:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000bc1a:	2b07      	cmp	r3, #7
3000bc1c:	d03d      	beq.n	3000bc9a <SWR_Calib_MEM+0x10e>
3000bc1e:	2307      	movs	r3, #7
3000bc20:	2000      	movs	r0, #0
3000bc22:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bc26:	eb0d 0203 	add.w	r2, sp, r3
3000bc2a:	f88d 0007 	strb.w	r0, [sp, #7]
3000bc2e:	f7fd fb93 	bl	30009358 <EFUSE_PMAP_READ8>
3000bc32:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bc36:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bc3a:	2af0      	cmp	r2, #240	; 0xf0
3000bc3c:	d126      	bne.n	3000bc8c <SWR_Calib_MEM+0x100>
3000bc3e:	f003 030f 	and.w	r3, r3, #15
3000bc42:	2b0f      	cmp	r3, #15
3000bc44:	d024      	beq.n	3000bc90 <SWR_Calib_MEM+0x104>
3000bc46:	2002      	movs	r0, #2
3000bc48:	2307      	movs	r3, #7
3000bc4a:	4c23      	ldr	r4, [pc, #140]	; (3000bcd8 <SWR_Calib_MEM+0x14c>)
3000bc4c:	f240 71e5 	movw	r1, #2021	; 0x7e5
3000bc50:	eb0d 0203 	add.w	r2, sp, r3
3000bc54:	7020      	strb	r0, [r4, #0]
3000bc56:	2000      	movs	r0, #0
3000bc58:	f7fd fb7e 	bl	30009358 <EFUSE_PMAP_READ8>
3000bc5c:	7822      	ldrb	r2, [r4, #0]
3000bc5e:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bc62:	2a03      	cmp	r2, #3
3000bc64:	d028      	beq.n	3000bcb8 <SWR_Calib_MEM+0x12c>
3000bc66:	2a02      	cmp	r2, #2
3000bc68:	d029      	beq.n	3000bcbe <SWR_Calib_MEM+0x132>
3000bc6a:	2200      	movs	r2, #0
3000bc6c:	4916      	ldr	r1, [pc, #88]	; (3000bcc8 <SWR_Calib_MEM+0x13c>)
3000bc6e:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000bc72:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000bc76:	4313      	orrs	r3, r2
3000bc78:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000bc7c:	b002      	add	sp, #8
3000bc7e:	bd10      	pop	{r4, pc}
3000bc80:	4b13      	ldr	r3, [pc, #76]	; (3000bcd0 <SWR_Calib_MEM+0x144>)
3000bc82:	2201      	movs	r2, #1
3000bc84:	701a      	strb	r2, [r3, #0]
3000bc86:	e7ca      	b.n	3000bc1e <SWR_Calib_MEM+0x92>
3000bc88:	2103      	movs	r1, #3
3000bc8a:	e7a6      	b.n	3000bbda <SWR_Calib_MEM+0x4e>
3000bc8c:	2003      	movs	r0, #3
3000bc8e:	e7db      	b.n	3000bc48 <SWR_Calib_MEM+0xbc>
3000bc90:	4b11      	ldr	r3, [pc, #68]	; (3000bcd8 <SWR_Calib_MEM+0x14c>)
3000bc92:	2201      	movs	r2, #1
3000bc94:	701a      	strb	r2, [r3, #0]
3000bc96:	b002      	add	sp, #8
3000bc98:	bd10      	pop	{r4, pc}
3000bc9a:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000bc9c:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000bca0:	d1bd      	bne.n	3000bc1e <SWR_Calib_MEM+0x92>
3000bca2:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000bca6:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000bcaa:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000bcae:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000bcb2:	e7b4      	b.n	3000bc1e <SWR_Calib_MEM+0x92>
3000bcb4:	091b      	lsrs	r3, r3, #4
3000bcb6:	e7a4      	b.n	3000bc02 <SWR_Calib_MEM+0x76>
3000bcb8:	091b      	lsrs	r3, r3, #4
3000bcba:	059a      	lsls	r2, r3, #22
3000bcbc:	e7d6      	b.n	3000bc6c <SWR_Calib_MEM+0xe0>
3000bcbe:	f003 030f 	and.w	r3, r3, #15
3000bcc2:	059a      	lsls	r2, r3, #22
3000bcc4:	e7d2      	b.n	3000bc6c <SWR_Calib_MEM+0xe0>
3000bcc6:	bf00      	nop
3000bcc8:	42008800 	.word	0x42008800
3000bccc:	fffffccc 	.word	0xfffffccc
3000bcd0:	3000e993 	.word	0x3000e993
3000bcd4:	42008100 	.word	0x42008100
3000bcd8:	3000e992 	.word	0x3000e992

3000bcdc <SWR_Calib_AUD>:
3000bcdc:	4b44      	ldr	r3, [pc, #272]	; (3000bdf0 <SWR_Calib_AUD+0x114>)
3000bcde:	f240 3231 	movw	r2, #817	; 0x331
3000bce2:	4944      	ldr	r1, [pc, #272]	; (3000bdf4 <SWR_Calib_AUD+0x118>)
3000bce4:	2000      	movs	r0, #0
3000bce6:	b510      	push	{r4, lr}
3000bce8:	f8d3 40a0 	ldr.w	r4, [r3, #160]	; 0xa0
3000bcec:	b082      	sub	sp, #8
3000bcee:	4021      	ands	r1, r4
3000bcf0:	430a      	orrs	r2, r1
3000bcf2:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000bcf6:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000bcfa:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000bcfe:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000bd02:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000bd06:	2307      	movs	r3, #7
3000bd08:	f88d 0007 	strb.w	r0, [sp, #7]
3000bd0c:	eb0d 0203 	add.w	r2, sp, r3
3000bd10:	f7fd fb22 	bl	30009358 <EFUSE_PMAP_READ8>
3000bd14:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bd18:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bd1c:	2af0      	cmp	r2, #240	; 0xf0
3000bd1e:	d155      	bne.n	3000bdcc <SWR_Calib_AUD+0xf0>
3000bd20:	f003 030f 	and.w	r3, r3, #15
3000bd24:	2b0f      	cmp	r3, #15
3000bd26:	d04d      	beq.n	3000bdc4 <SWR_Calib_AUD+0xe8>
3000bd28:	2102      	movs	r1, #2
3000bd2a:	4c33      	ldr	r4, [pc, #204]	; (3000bdf8 <SWR_Calib_AUD+0x11c>)
3000bd2c:	2307      	movs	r3, #7
3000bd2e:	f10d 0206 	add.w	r2, sp, #6
3000bd32:	2000      	movs	r0, #0
3000bd34:	7021      	strb	r1, [r4, #0]
3000bd36:	f240 71e6 	movw	r1, #2022	; 0x7e6
3000bd3a:	f7fd fb0d 	bl	30009358 <EFUSE_PMAP_READ8>
3000bd3e:	7822      	ldrb	r2, [r4, #0]
3000bd40:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000bd44:	2a03      	cmp	r2, #3
3000bd46:	d04a      	beq.n	3000bdde <SWR_Calib_AUD+0x102>
3000bd48:	2a02      	cmp	r2, #2
3000bd4a:	bf0c      	ite	eq
3000bd4c:	f003 030f 	andeq.w	r3, r3, #15
3000bd50:	2300      	movne	r3, #0
3000bd52:	4927      	ldr	r1, [pc, #156]	; (3000bdf0 <SWR_Calib_AUD+0x114>)
3000bd54:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000bd58:	f022 020f 	bic.w	r2, r2, #15
3000bd5c:	4313      	orrs	r3, r2
3000bd5e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000bd62:	2307      	movs	r3, #7
3000bd64:	2000      	movs	r0, #0
3000bd66:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000bd6a:	eb0d 0203 	add.w	r2, sp, r3
3000bd6e:	f88d 0007 	strb.w	r0, [sp, #7]
3000bd72:	f7fd faf1 	bl	30009358 <EFUSE_PMAP_READ8>
3000bd76:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bd7a:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000bd7e:	2af0      	cmp	r2, #240	; 0xf0
3000bd80:	d126      	bne.n	3000bdd0 <SWR_Calib_AUD+0xf4>
3000bd82:	f003 030f 	and.w	r3, r3, #15
3000bd86:	2b0f      	cmp	r3, #15
3000bd88:	d024      	beq.n	3000bdd4 <SWR_Calib_AUD+0xf8>
3000bd8a:	2002      	movs	r0, #2
3000bd8c:	2307      	movs	r3, #7
3000bd8e:	4c1b      	ldr	r4, [pc, #108]	; (3000bdfc <SWR_Calib_AUD+0x120>)
3000bd90:	f240 71e7 	movw	r1, #2023	; 0x7e7
3000bd94:	eb0d 0203 	add.w	r2, sp, r3
3000bd98:	7020      	strb	r0, [r4, #0]
3000bd9a:	2000      	movs	r0, #0
3000bd9c:	f7fd fadc 	bl	30009358 <EFUSE_PMAP_READ8>
3000bda0:	7822      	ldrb	r2, [r4, #0]
3000bda2:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000bda6:	2a03      	cmp	r2, #3
3000bda8:	d01b      	beq.n	3000bde2 <SWR_Calib_AUD+0x106>
3000bdaa:	2a02      	cmp	r2, #2
3000bdac:	d01c      	beq.n	3000bde8 <SWR_Calib_AUD+0x10c>
3000bdae:	2200      	movs	r2, #0
3000bdb0:	490f      	ldr	r1, [pc, #60]	; (3000bdf0 <SWR_Calib_AUD+0x114>)
3000bdb2:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000bdb6:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000bdba:	4313      	orrs	r3, r2
3000bdbc:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000bdc0:	b002      	add	sp, #8
3000bdc2:	bd10      	pop	{r4, pc}
3000bdc4:	4b0c      	ldr	r3, [pc, #48]	; (3000bdf8 <SWR_Calib_AUD+0x11c>)
3000bdc6:	2201      	movs	r2, #1
3000bdc8:	701a      	strb	r2, [r3, #0]
3000bdca:	e7ca      	b.n	3000bd62 <SWR_Calib_AUD+0x86>
3000bdcc:	2103      	movs	r1, #3
3000bdce:	e7ac      	b.n	3000bd2a <SWR_Calib_AUD+0x4e>
3000bdd0:	2003      	movs	r0, #3
3000bdd2:	e7db      	b.n	3000bd8c <SWR_Calib_AUD+0xb0>
3000bdd4:	4b09      	ldr	r3, [pc, #36]	; (3000bdfc <SWR_Calib_AUD+0x120>)
3000bdd6:	2201      	movs	r2, #1
3000bdd8:	701a      	strb	r2, [r3, #0]
3000bdda:	b002      	add	sp, #8
3000bddc:	bd10      	pop	{r4, pc}
3000bdde:	091b      	lsrs	r3, r3, #4
3000bde0:	e7b7      	b.n	3000bd52 <SWR_Calib_AUD+0x76>
3000bde2:	091b      	lsrs	r3, r3, #4
3000bde4:	059a      	lsls	r2, r3, #22
3000bde6:	e7e3      	b.n	3000bdb0 <SWR_Calib_AUD+0xd4>
3000bde8:	f003 030f 	and.w	r3, r3, #15
3000bdec:	059a      	lsls	r2, r3, #22
3000bdee:	e7df      	b.n	3000bdb0 <SWR_Calib_AUD+0xd4>
3000bdf0:	42008800 	.word	0x42008800
3000bdf4:	fffffccc 	.word	0xfffffccc
3000bdf8:	3000e991 	.word	0x3000e991
3000bdfc:	3000e990 	.word	0x3000e990

3000be00 <__aeabi_uldivmod>:
3000be00:	b953      	cbnz	r3, 3000be18 <__aeabi_uldivmod+0x18>
3000be02:	b94a      	cbnz	r2, 3000be18 <__aeabi_uldivmod+0x18>
3000be04:	2900      	cmp	r1, #0
3000be06:	bf08      	it	eq
3000be08:	2800      	cmpeq	r0, #0
3000be0a:	bf1c      	itt	ne
3000be0c:	f04f 31ff 	movne.w	r1, #4294967295
3000be10:	f04f 30ff 	movne.w	r0, #4294967295
3000be14:	f000 b960 	b.w	3000c0d8 <__aeabi_idiv0>
3000be18:	f1ad 0c08 	sub.w	ip, sp, #8
3000be1c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000be20:	f000 f806 	bl	3000be30 <__udivmoddi4>
3000be24:	f8dd e004 	ldr.w	lr, [sp, #4]
3000be28:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000be2c:	b004      	add	sp, #16
3000be2e:	4770      	bx	lr

3000be30 <__udivmoddi4>:
3000be30:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000be34:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000be36:	4688      	mov	r8, r1
3000be38:	468e      	mov	lr, r1
3000be3a:	4615      	mov	r5, r2
3000be3c:	4604      	mov	r4, r0
3000be3e:	4619      	mov	r1, r3
3000be40:	2b00      	cmp	r3, #0
3000be42:	f040 80c6 	bne.w	3000bfd2 <__udivmoddi4+0x1a2>
3000be46:	4542      	cmp	r2, r8
3000be48:	fab2 f782 	clz	r7, r2
3000be4c:	d946      	bls.n	3000bedc <__udivmoddi4+0xac>
3000be4e:	b14f      	cbz	r7, 3000be64 <__udivmoddi4+0x34>
3000be50:	f1c7 0c20 	rsb	ip, r7, #32
3000be54:	fa08 f307 	lsl.w	r3, r8, r7
3000be58:	40bd      	lsls	r5, r7
3000be5a:	40bc      	lsls	r4, r7
3000be5c:	fa20 fc0c 	lsr.w	ip, r0, ip
3000be60:	ea4c 0e03 	orr.w	lr, ip, r3
3000be64:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000be68:	fa1f fc85 	uxth.w	ip, r5
3000be6c:	0c22      	lsrs	r2, r4, #16
3000be6e:	fbbe f9f8 	udiv	r9, lr, r8
3000be72:	fb08 e319 	mls	r3, r8, r9, lr
3000be76:	fb09 fa0c 	mul.w	sl, r9, ip
3000be7a:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000be7e:	459a      	cmp	sl, r3
3000be80:	d928      	bls.n	3000bed4 <__udivmoddi4+0xa4>
3000be82:	18eb      	adds	r3, r5, r3
3000be84:	f109 30ff 	add.w	r0, r9, #4294967295
3000be88:	d204      	bcs.n	3000be94 <__udivmoddi4+0x64>
3000be8a:	459a      	cmp	sl, r3
3000be8c:	d902      	bls.n	3000be94 <__udivmoddi4+0x64>
3000be8e:	f1a9 0002 	sub.w	r0, r9, #2
3000be92:	442b      	add	r3, r5
3000be94:	eba3 030a 	sub.w	r3, r3, sl
3000be98:	b2a4      	uxth	r4, r4
3000be9a:	fbb3 f2f8 	udiv	r2, r3, r8
3000be9e:	fb08 3312 	mls	r3, r8, r2, r3
3000bea2:	fb02 fc0c 	mul.w	ip, r2, ip
3000bea6:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000beaa:	45a4      	cmp	ip, r4
3000beac:	d914      	bls.n	3000bed8 <__udivmoddi4+0xa8>
3000beae:	192c      	adds	r4, r5, r4
3000beb0:	f102 33ff 	add.w	r3, r2, #4294967295
3000beb4:	d203      	bcs.n	3000bebe <__udivmoddi4+0x8e>
3000beb6:	45a4      	cmp	ip, r4
3000beb8:	d901      	bls.n	3000bebe <__udivmoddi4+0x8e>
3000beba:	1e93      	subs	r3, r2, #2
3000bebc:	442c      	add	r4, r5
3000bebe:	eba4 040c 	sub.w	r4, r4, ip
3000bec2:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000bec6:	b11e      	cbz	r6, 3000bed0 <__udivmoddi4+0xa0>
3000bec8:	40fc      	lsrs	r4, r7
3000beca:	2300      	movs	r3, #0
3000becc:	6034      	str	r4, [r6, #0]
3000bece:	6073      	str	r3, [r6, #4]
3000bed0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bed4:	4648      	mov	r0, r9
3000bed6:	e7dd      	b.n	3000be94 <__udivmoddi4+0x64>
3000bed8:	4613      	mov	r3, r2
3000beda:	e7f0      	b.n	3000bebe <__udivmoddi4+0x8e>
3000bedc:	b902      	cbnz	r2, 3000bee0 <__udivmoddi4+0xb0>
3000bede:	deff      	udf	#255	; 0xff
3000bee0:	bb8f      	cbnz	r7, 3000bf46 <__udivmoddi4+0x116>
3000bee2:	eba8 0302 	sub.w	r3, r8, r2
3000bee6:	2101      	movs	r1, #1
3000bee8:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000beec:	b2aa      	uxth	r2, r5
3000beee:	0c20      	lsrs	r0, r4, #16
3000bef0:	fbb3 fcfe 	udiv	ip, r3, lr
3000bef4:	fb0e 331c 	mls	r3, lr, ip, r3
3000bef8:	fb0c f802 	mul.w	r8, ip, r2
3000befc:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000bf00:	4598      	cmp	r8, r3
3000bf02:	d962      	bls.n	3000bfca <__udivmoddi4+0x19a>
3000bf04:	18eb      	adds	r3, r5, r3
3000bf06:	f10c 30ff 	add.w	r0, ip, #4294967295
3000bf0a:	d204      	bcs.n	3000bf16 <__udivmoddi4+0xe6>
3000bf0c:	4598      	cmp	r8, r3
3000bf0e:	d902      	bls.n	3000bf16 <__udivmoddi4+0xe6>
3000bf10:	f1ac 0002 	sub.w	r0, ip, #2
3000bf14:	442b      	add	r3, r5
3000bf16:	eba3 0308 	sub.w	r3, r3, r8
3000bf1a:	b2a4      	uxth	r4, r4
3000bf1c:	fbb3 fcfe 	udiv	ip, r3, lr
3000bf20:	fb0e 331c 	mls	r3, lr, ip, r3
3000bf24:	fb0c f202 	mul.w	r2, ip, r2
3000bf28:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bf2c:	42a2      	cmp	r2, r4
3000bf2e:	d94e      	bls.n	3000bfce <__udivmoddi4+0x19e>
3000bf30:	192c      	adds	r4, r5, r4
3000bf32:	f10c 33ff 	add.w	r3, ip, #4294967295
3000bf36:	d204      	bcs.n	3000bf42 <__udivmoddi4+0x112>
3000bf38:	42a2      	cmp	r2, r4
3000bf3a:	d902      	bls.n	3000bf42 <__udivmoddi4+0x112>
3000bf3c:	f1ac 0302 	sub.w	r3, ip, #2
3000bf40:	442c      	add	r4, r5
3000bf42:	1aa4      	subs	r4, r4, r2
3000bf44:	e7bd      	b.n	3000bec2 <__udivmoddi4+0x92>
3000bf46:	f1c7 0c20 	rsb	ip, r7, #32
3000bf4a:	40bd      	lsls	r5, r7
3000bf4c:	fa08 f307 	lsl.w	r3, r8, r7
3000bf50:	40bc      	lsls	r4, r7
3000bf52:	fa28 f90c 	lsr.w	r9, r8, ip
3000bf56:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000bf5a:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bf5e:	ea4c 0203 	orr.w	r2, ip, r3
3000bf62:	b2ab      	uxth	r3, r5
3000bf64:	fbb9 fcfe 	udiv	ip, r9, lr
3000bf68:	0c11      	lsrs	r1, r2, #16
3000bf6a:	fb0e 901c 	mls	r0, lr, ip, r9
3000bf6e:	fb0c f803 	mul.w	r8, ip, r3
3000bf72:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000bf76:	4580      	cmp	r8, r0
3000bf78:	d923      	bls.n	3000bfc2 <__udivmoddi4+0x192>
3000bf7a:	1828      	adds	r0, r5, r0
3000bf7c:	f10c 31ff 	add.w	r1, ip, #4294967295
3000bf80:	d204      	bcs.n	3000bf8c <__udivmoddi4+0x15c>
3000bf82:	4580      	cmp	r8, r0
3000bf84:	d902      	bls.n	3000bf8c <__udivmoddi4+0x15c>
3000bf86:	f1ac 0102 	sub.w	r1, ip, #2
3000bf8a:	4428      	add	r0, r5
3000bf8c:	eba0 0008 	sub.w	r0, r0, r8
3000bf90:	b292      	uxth	r2, r2
3000bf92:	fbb0 fcfe 	udiv	ip, r0, lr
3000bf96:	fb0e 001c 	mls	r0, lr, ip, r0
3000bf9a:	fb0c f803 	mul.w	r8, ip, r3
3000bf9e:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000bfa2:	4598      	cmp	r8, r3
3000bfa4:	d90f      	bls.n	3000bfc6 <__udivmoddi4+0x196>
3000bfa6:	18eb      	adds	r3, r5, r3
3000bfa8:	f10c 32ff 	add.w	r2, ip, #4294967295
3000bfac:	d204      	bcs.n	3000bfb8 <__udivmoddi4+0x188>
3000bfae:	4598      	cmp	r8, r3
3000bfb0:	d902      	bls.n	3000bfb8 <__udivmoddi4+0x188>
3000bfb2:	f1ac 0202 	sub.w	r2, ip, #2
3000bfb6:	442b      	add	r3, r5
3000bfb8:	eba3 0308 	sub.w	r3, r3, r8
3000bfbc:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000bfc0:	e792      	b.n	3000bee8 <__udivmoddi4+0xb8>
3000bfc2:	4661      	mov	r1, ip
3000bfc4:	e7e2      	b.n	3000bf8c <__udivmoddi4+0x15c>
3000bfc6:	4662      	mov	r2, ip
3000bfc8:	e7f6      	b.n	3000bfb8 <__udivmoddi4+0x188>
3000bfca:	4660      	mov	r0, ip
3000bfcc:	e7a3      	b.n	3000bf16 <__udivmoddi4+0xe6>
3000bfce:	4663      	mov	r3, ip
3000bfd0:	e7b7      	b.n	3000bf42 <__udivmoddi4+0x112>
3000bfd2:	4543      	cmp	r3, r8
3000bfd4:	d905      	bls.n	3000bfe2 <__udivmoddi4+0x1b2>
3000bfd6:	b10e      	cbz	r6, 3000bfdc <__udivmoddi4+0x1ac>
3000bfd8:	e9c6 0800 	strd	r0, r8, [r6]
3000bfdc:	2100      	movs	r1, #0
3000bfde:	4608      	mov	r0, r1
3000bfe0:	e776      	b.n	3000bed0 <__udivmoddi4+0xa0>
3000bfe2:	fab3 fc83 	clz	ip, r3
3000bfe6:	f1bc 0f00 	cmp.w	ip, #0
3000bfea:	d110      	bne.n	3000c00e <__udivmoddi4+0x1de>
3000bfec:	4543      	cmp	r3, r8
3000bfee:	d301      	bcc.n	3000bff4 <__udivmoddi4+0x1c4>
3000bff0:	4282      	cmp	r2, r0
3000bff2:	d80a      	bhi.n	3000c00a <__udivmoddi4+0x1da>
3000bff4:	1a84      	subs	r4, r0, r2
3000bff6:	eb68 0303 	sbc.w	r3, r8, r3
3000bffa:	2001      	movs	r0, #1
3000bffc:	469e      	mov	lr, r3
3000bffe:	2e00      	cmp	r6, #0
3000c000:	d068      	beq.n	3000c0d4 <__udivmoddi4+0x2a4>
3000c002:	e9c6 4e00 	strd	r4, lr, [r6]
3000c006:	2100      	movs	r1, #0
3000c008:	e762      	b.n	3000bed0 <__udivmoddi4+0xa0>
3000c00a:	4660      	mov	r0, ip
3000c00c:	e7f7      	b.n	3000bffe <__udivmoddi4+0x1ce>
3000c00e:	f1cc 0e20 	rsb	lr, ip, #32
3000c012:	fa03 f30c 	lsl.w	r3, r3, ip
3000c016:	fa02 f50c 	lsl.w	r5, r2, ip
3000c01a:	fa00 f70c 	lsl.w	r7, r0, ip
3000c01e:	fa22 f40e 	lsr.w	r4, r2, lr
3000c022:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c026:	fa20 f10e 	lsr.w	r1, r0, lr
3000c02a:	431c      	orrs	r4, r3
3000c02c:	fa08 f30c 	lsl.w	r3, r8, ip
3000c030:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c034:	4319      	orrs	r1, r3
3000c036:	b2a3      	uxth	r3, r4
3000c038:	0c0a      	lsrs	r2, r1, #16
3000c03a:	fbbb f8f9 	udiv	r8, fp, r9
3000c03e:	fb09 bb18 	mls	fp, r9, r8, fp
3000c042:	fb08 fa03 	mul.w	sl, r8, r3
3000c046:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c04a:	4592      	cmp	sl, r2
3000c04c:	d93e      	bls.n	3000c0cc <__udivmoddi4+0x29c>
3000c04e:	18a2      	adds	r2, r4, r2
3000c050:	f108 30ff 	add.w	r0, r8, #4294967295
3000c054:	d204      	bcs.n	3000c060 <__udivmoddi4+0x230>
3000c056:	4592      	cmp	sl, r2
3000c058:	d902      	bls.n	3000c060 <__udivmoddi4+0x230>
3000c05a:	f1a8 0002 	sub.w	r0, r8, #2
3000c05e:	4422      	add	r2, r4
3000c060:	eba2 020a 	sub.w	r2, r2, sl
3000c064:	b289      	uxth	r1, r1
3000c066:	fbb2 f8f9 	udiv	r8, r2, r9
3000c06a:	fb09 2218 	mls	r2, r9, r8, r2
3000c06e:	fb08 f303 	mul.w	r3, r8, r3
3000c072:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c076:	4293      	cmp	r3, r2
3000c078:	d92a      	bls.n	3000c0d0 <__udivmoddi4+0x2a0>
3000c07a:	18a2      	adds	r2, r4, r2
3000c07c:	f108 31ff 	add.w	r1, r8, #4294967295
3000c080:	d204      	bcs.n	3000c08c <__udivmoddi4+0x25c>
3000c082:	4293      	cmp	r3, r2
3000c084:	d902      	bls.n	3000c08c <__udivmoddi4+0x25c>
3000c086:	f1a8 0102 	sub.w	r1, r8, #2
3000c08a:	4422      	add	r2, r4
3000c08c:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c090:	1ad2      	subs	r2, r2, r3
3000c092:	fba0 9805 	umull	r9, r8, r0, r5
3000c096:	4542      	cmp	r2, r8
3000c098:	464b      	mov	r3, r9
3000c09a:	4641      	mov	r1, r8
3000c09c:	d302      	bcc.n	3000c0a4 <__udivmoddi4+0x274>
3000c09e:	d106      	bne.n	3000c0ae <__udivmoddi4+0x27e>
3000c0a0:	454f      	cmp	r7, r9
3000c0a2:	d204      	bcs.n	3000c0ae <__udivmoddi4+0x27e>
3000c0a4:	3801      	subs	r0, #1
3000c0a6:	ebb9 0305 	subs.w	r3, r9, r5
3000c0aa:	eb68 0104 	sbc.w	r1, r8, r4
3000c0ae:	b18e      	cbz	r6, 3000c0d4 <__udivmoddi4+0x2a4>
3000c0b0:	1afc      	subs	r4, r7, r3
3000c0b2:	eb62 0701 	sbc.w	r7, r2, r1
3000c0b6:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c0ba:	fa24 f30c 	lsr.w	r3, r4, ip
3000c0be:	fa27 f70c 	lsr.w	r7, r7, ip
3000c0c2:	ea4e 0303 	orr.w	r3, lr, r3
3000c0c6:	e9c6 3700 	strd	r3, r7, [r6]
3000c0ca:	e79c      	b.n	3000c006 <__udivmoddi4+0x1d6>
3000c0cc:	4640      	mov	r0, r8
3000c0ce:	e7c7      	b.n	3000c060 <__udivmoddi4+0x230>
3000c0d0:	4641      	mov	r1, r8
3000c0d2:	e7db      	b.n	3000c08c <__udivmoddi4+0x25c>
3000c0d4:	4631      	mov	r1, r6
3000c0d6:	e6fb      	b.n	3000bed0 <__udivmoddi4+0xa0>

3000c0d8 <__aeabi_idiv0>:
3000c0d8:	4770      	bx	lr
3000c0da:	bf00      	nop
3000c0dc:	0000      	movs	r0, r0
	...

3000c0e0 <____wrap_memcpy_veneer>:
3000c0e0:	b401      	push	{r0}
3000c0e2:	4802      	ldr	r0, [pc, #8]	; (3000c0ec <____wrap_memcpy_veneer+0xc>)
3000c0e4:	4684      	mov	ip, r0
3000c0e6:	bc01      	pop	{r0}
3000c0e8:	4760      	bx	ip
3000c0ea:	bf00      	nop
3000c0ec:	00012f05 	.word	0x00012f05

3000c0f0 <____wrap_memset_veneer>:
3000c0f0:	b401      	push	{r0}
3000c0f2:	4802      	ldr	r0, [pc, #8]	; (3000c0fc <____wrap_memset_veneer+0xc>)
3000c0f4:	4684      	mov	ip, r0
3000c0f6:	bc01      	pop	{r0}
3000c0f8:	4760      	bx	ip
3000c0fa:	bf00      	nop
3000c0fc:	00012efd 	.word	0x00012efd

3000c100 <____wrap_memcmp_veneer>:
3000c100:	b401      	push	{r0}
3000c102:	4802      	ldr	r0, [pc, #8]	; (3000c10c <____wrap_memcmp_veneer+0xc>)
3000c104:	4684      	mov	ip, r0
3000c106:	bc01      	pop	{r0}
3000c108:	4760      	bx	ip
3000c10a:	bf00      	nop
3000c10c:	00012f0d 	.word	0x00012f0d

3000c110 <____wrap_strlen_veneer>:
3000c110:	b401      	push	{r0}
3000c112:	4802      	ldr	r0, [pc, #8]	; (3000c11c <____wrap_strlen_veneer+0xc>)
3000c114:	4684      	mov	ip, r0
3000c116:	bc01      	pop	{r0}
3000c118:	4760      	bx	ip
3000c11a:	bf00      	nop
3000c11c:	00012f25 	.word	0x00012f25

3000c120 <__SBOOT_Validate_ImgHash_veneer>:
3000c120:	b401      	push	{r0}
3000c122:	4802      	ldr	r0, [pc, #8]	; (3000c12c <__SBOOT_Validate_ImgHash_veneer+0xc>)
3000c124:	4684      	mov	ip, r0
3000c126:	bc01      	pop	{r0}
3000c128:	4760      	bx	ip
3000c12a:	bf00      	nop
3000c12c:	00003541 	.word	0x00003541

3000c130 <__DiagPrintf_veneer>:
3000c130:	b401      	push	{r0}
3000c132:	4802      	ldr	r0, [pc, #8]	; (3000c13c <__DiagPrintf_veneer+0xc>)
3000c134:	4684      	mov	ip, r0
3000c136:	bc01      	pop	{r0}
3000c138:	4760      	bx	ip
3000c13a:	bf00      	nop
3000c13c:	0000e5f9 	.word	0x0000e5f9

3000c140 <__SBOOT_Validate_Signature_veneer>:
3000c140:	b401      	push	{r0}
3000c142:	4802      	ldr	r0, [pc, #8]	; (3000c14c <__SBOOT_Validate_Signature_veneer+0xc>)
3000c144:	4684      	mov	ip, r0
3000c146:	bc01      	pop	{r0}
3000c148:	4760      	bx	ip
3000c14a:	bf00      	nop
3000c14c:	00003431 	.word	0x00003431

3000c150 <__SBOOT_Validate_Algorithm_veneer>:
3000c150:	b401      	push	{r0}
3000c152:	4802      	ldr	r0, [pc, #8]	; (3000c15c <__SBOOT_Validate_Algorithm_veneer+0xc>)
3000c154:	4684      	mov	ip, r0
3000c156:	bc01      	pop	{r0}
3000c158:	4760      	bx	ip
3000c15a:	bf00      	nop
3000c15c:	000033ed 	.word	0x000033ed

3000c160 <__SBOOT_GetMdType_veneer>:
3000c160:	b401      	push	{r0}
3000c162:	4802      	ldr	r0, [pc, #8]	; (3000c16c <__SBOOT_GetMdType_veneer+0xc>)
3000c164:	4684      	mov	ip, r0
3000c166:	bc01      	pop	{r0}
3000c168:	4760      	bx	ip
3000c16a:	bf00      	nop
3000c16c:	0000319d 	.word	0x0000319d

3000c170 <__io_assert_failed_veneer>:
3000c170:	b401      	push	{r0}
3000c172:	4802      	ldr	r0, [pc, #8]	; (3000c17c <__io_assert_failed_veneer+0xc>)
3000c174:	4684      	mov	ip, r0
3000c176:	bc01      	pop	{r0}
3000c178:	4760      	bx	ip
3000c17a:	bf00      	nop
3000c17c:	0000992d 	.word	0x0000992d

3000c180 <__SBOOT_Validate_PubKey_veneer>:
3000c180:	b401      	push	{r0}
3000c182:	4802      	ldr	r0, [pc, #8]	; (3000c18c <__SBOOT_Validate_PubKey_veneer+0xc>)
3000c184:	4684      	mov	ip, r0
3000c186:	bc01      	pop	{r0}
3000c188:	4760      	bx	ip
3000c18a:	bf00      	nop
3000c18c:	00003495 	.word	0x00003495
3000c190:	004f4931 	.word	0x004f4931
3000c194:	00004f32 	.word	0x00004f32
3000c198:	004f4932 	.word	0x004f4932
3000c19c:	00004f34 	.word	0x00004f34
3000c1a0:	004f4934 	.word	0x004f4934
3000c1a4:	414c460d 	.word	0x414c460d
3000c1a8:	435f4853 	.word	0x435f4853
3000c1ac:	62696c61 	.word	0x62696c61
3000c1b0:	69746172 	.word	0x69746172
3000c1b4:	654e6e6f 	.word	0x654e6e6f
3000c1b8:	78252077 	.word	0x78252077
3000c1bc:	2078253a 	.word	0x2078253a
3000c1c0:	73616870 	.word	0x73616870
3000c1c4:	68735f65 	.word	0x68735f65
3000c1c8:	3a746669 	.word	0x3a746669
3000c1cc:	000a7825 	.word	0x000a7825
3000c1d0:	414c460d 	.word	0x414c460d
3000c1d4:	435f4853 	.word	0x435f4853
3000c1d8:	62696c61 	.word	0x62696c61
3000c1dc:	69746172 	.word	0x69746172
3000c1e0:	654e6e6f 	.word	0x654e6e6f
3000c1e4:	694c2077 	.word	0x694c2077
3000c1e8:	6544656e 	.word	0x6544656e
3000c1ec:	3a79616c 	.word	0x3a79616c
3000c1f0:	000a7825 	.word	0x000a7825
3000c1f4:	414c460d 	.word	0x414c460d
3000c1f8:	435f4853 	.word	0x435f4853
3000c1fc:	62696c61 	.word	0x62696c61
3000c200:	69746172 	.word	0x69746172
3000c204:	654e6e6f 	.word	0x654e6e6f
3000c208:	69772077 	.word	0x69772077
3000c20c:	776f646e 	.word	0x776f646e
3000c210:	74735f31 	.word	0x74735f31
3000c214:	3a747261 	.word	0x3a747261
3000c218:	77206425 	.word	0x77206425
3000c21c:	6f646e69 	.word	0x6f646e69
3000c220:	735f3177 	.word	0x735f3177
3000c224:	3a657a69 	.word	0x3a657a69
3000c228:	0a206425 	.word	0x0a206425
3000c22c:	00000000 	.word	0x00000000
3000c230:	414c460d 	.word	0x414c460d
3000c234:	435f4853 	.word	0x435f4853
3000c238:	62696c61 	.word	0x62696c61
3000c23c:	69746172 	.word	0x69746172
3000c240:	654e6e6f 	.word	0x654e6e6f
3000c244:	68702077 	.word	0x68702077
3000c248:	5f657361 	.word	0x5f657361
3000c24c:	66696873 	.word	0x66696873
3000c250:	64695f74 	.word	0x64695f74
3000c254:	64253a78 	.word	0x64253a78
3000c258:	00000a20 	.word	0x00000a20
3000c25c:	414c460d 	.word	0x414c460d
3000c260:	435f4853 	.word	0x435f4853
3000c264:	62696c61 	.word	0x62696c61
3000c268:	69746172 	.word	0x69746172
3000c26c:	25206e6f 	.word	0x25206e6f
3000c270:	61732078 	.word	0x61732078
3000c274:	656c706d 	.word	0x656c706d
3000c278:	6168705f 	.word	0x6168705f
3000c27c:	253a6573 	.word	0x253a6573
3000c280:	61722078 	.word	0x61722078
3000c284:	253a6574 	.word	0x253a6574
3000c288:	00000a78 	.word	0x00000a78
3000c28c:	414c460d 	.word	0x414c460d
3000c290:	435f4853 	.word	0x435f4853
3000c294:	62696c61 	.word	0x62696c61
3000c298:	69746172 	.word	0x69746172
3000c29c:	73206e6f 	.word	0x73206e6f
3000c2a0:	6c706d61 	.word	0x6c706d61
3000c2a4:	68705f65 	.word	0x68705f65
3000c2a8:	3a657361 	.word	0x3a657361
3000c2ac:	72207825 	.word	0x72207825
3000c2b0:	3a657461 	.word	0x3a657461
3000c2b4:	000a7825 	.word	0x000a7825
3000c2b8:	444f4d5b 	.word	0x444f4d5b
3000c2bc:	5f454c55 	.word	0x5f454c55
3000c2c0:	544f4f42 	.word	0x544f4f42
3000c2c4:	56454c2d 	.word	0x56454c2d
3000c2c8:	495f4c45 	.word	0x495f4c45
3000c2cc:	5d4f464e 	.word	0x5d4f464e
3000c2d0:	6c61633a 	.word	0x6c61633a
3000c2d4:	61726269 	.word	0x61726269
3000c2d8:	6e6f6974 	.word	0x6e6f6974
3000c2dc:	3a6b6f5f 	.word	0x3a6b6f5f
3000c2e0:	3a64255b 	.word	0x3a64255b
3000c2e4:	253a6425 	.word	0x253a6425
3000c2e8:	0a205d64 	.word	0x0a205d64
3000c2ec:	00000000 	.word	0x00000000
3000c2f0:	444f4d5b 	.word	0x444f4d5b
3000c2f4:	5f454c55 	.word	0x5f454c55
3000c2f8:	544f4f42 	.word	0x544f4f42
3000c2fc:	56454c2d 	.word	0x56454c2d
3000c300:	495f4c45 	.word	0x495f4c45
3000c304:	5d4f464e 	.word	0x5d4f464e
3000c308:	414c463a 	.word	0x414c463a
3000c30c:	43204853 	.word	0x43204853
3000c310:	42494c41 	.word	0x42494c41
3000c314:	2578305b 	.word	0x2578305b
3000c318:	4b4f2078 	.word	0x4b4f2078
3000c31c:	00000a5d 	.word	0x00000a5d
3000c320:	444f4d5b 	.word	0x444f4d5b
3000c324:	5f454c55 	.word	0x5f454c55
3000c328:	544f4f42 	.word	0x544f4f42
3000c32c:	56454c2d 	.word	0x56454c2d
3000c330:	495f4c45 	.word	0x495f4c45
3000c334:	5d4f464e 	.word	0x5d4f464e
3000c338:	414c463a 	.word	0x414c463a
3000c33c:	43204853 	.word	0x43204853
3000c340:	42494c41 	.word	0x42494c41
3000c344:	2578305b 	.word	0x2578305b
3000c348:	41462078 	.word	0x41462078
3000c34c:	0a5d4c49 	.word	0x0a5d4c49
3000c350:	00000000 	.word	0x00000000
3000c354:	444f4d5b 	.word	0x444f4d5b
3000c358:	5f454c55 	.word	0x5f454c55
3000c35c:	544f4f42 	.word	0x544f4f42
3000c360:	56454c2d 	.word	0x56454c2d
3000c364:	495f4c45 	.word	0x495f4c45
3000c368:	5d4f464e 	.word	0x5d4f464e
3000c36c:	616c463a 	.word	0x616c463a
3000c370:	52206873 	.word	0x52206873
3000c374:	20646165 	.word	0x20646165
3000c378:	000a7325 	.word	0x000a7325
3000c37c:	444f4d5b 	.word	0x444f4d5b
3000c380:	5f454c55 	.word	0x5f454c55
3000c384:	544f4f42 	.word	0x544f4f42
3000c388:	56454c2d 	.word	0x56454c2d
3000c38c:	495f4c45 	.word	0x495f4c45
3000c390:	5d4f464e 	.word	0x5d4f464e
3000c394:	616c463a 	.word	0x616c463a
3000c398:	52206873 	.word	0x52206873
3000c39c:	20646165 	.word	0x20646165
3000c3a0:	202c7325 	.word	0x202c7325
3000c3a4:	4c494146 	.word	0x4c494146
3000c3a8:	0000000a 	.word	0x0000000a
3000c3ac:	444f4d5b 	.word	0x444f4d5b
3000c3b0:	5f454c55 	.word	0x5f454c55
3000c3b4:	544f4f42 	.word	0x544f4f42
3000c3b8:	56454c2d 	.word	0x56454c2d
3000c3bc:	455f4c45 	.word	0x455f4c45
3000c3c0:	524f5252 	.word	0x524f5252
3000c3c4:	6c463a5d 	.word	0x6c463a5d
3000c3c8:	20687361 	.word	0x20687361
3000c3cc:	74697753 	.word	0x74697753
3000c3d0:	52206863 	.word	0x52206863
3000c3d4:	20646165 	.word	0x20646165
3000c3d8:	65646f4d 	.word	0x65646f4d
3000c3dc:	49414620 	.word	0x49414620
3000c3e0:	00000a4c 	.word	0x00000a4c
3000c3e4:	444f4d5b 	.word	0x444f4d5b
3000c3e8:	5f454c55 	.word	0x5f454c55
3000c3ec:	544f4f42 	.word	0x544f4f42
3000c3f0:	56454c2d 	.word	0x56454c2d
3000c3f4:	495f4c45 	.word	0x495f4c45
3000c3f8:	5d4f464e 	.word	0x5d4f464e
3000c3fc:	616c463a 	.word	0x616c463a
3000c400:	49206873 	.word	0x49206873
3000c404:	25203a44 	.word	0x25203a44
3000c408:	78252d78 	.word	0x78252d78
3000c40c:	0a78252d 	.word	0x0a78252d
3000c410:	00000000 	.word	0x00000000
3000c414:	6968540d 	.word	0x6968540d
3000c418:	6c662073 	.word	0x6c662073
3000c41c:	20687361 	.word	0x20687361
3000c420:	65707974 	.word	0x65707974
3000c424:	20736920 	.word	0x20736920
3000c428:	20746f6e 	.word	0x20746f6e
3000c42c:	70707573 	.word	0x70707573
3000c430:	6574726f 	.word	0x6574726f
3000c434:	000a2164 	.word	0x000a2164
3000c438:	444f4d5b 	.word	0x444f4d5b
3000c43c:	5f454c55 	.word	0x5f454c55
3000c440:	544f4f42 	.word	0x544f4f42
3000c444:	56454c2d 	.word	0x56454c2d
3000c448:	495f4c45 	.word	0x495f4c45
3000c44c:	5d4f464e 	.word	0x5d4f464e
3000c450:	4e414e3a 	.word	0x4e414e3a
3000c454:	44492044 	.word	0x44492044
3000c458:	7825203a 	.word	0x7825203a
3000c45c:	0a78252d 	.word	0x0a78252d
3000c460:	00000000 	.word	0x00000000
3000c464:	20304d4b 	.word	0x20304d4b
3000c468:	20504958 	.word	0x20504958
3000c46c:	00474d49 	.word	0x00474d49
3000c470:	20304d4b 	.word	0x20304d4b
3000c474:	4d415253 	.word	0x4d415253
3000c478:	00000000 	.word	0x00000000
3000c47c:	20304d4b 	.word	0x20304d4b
3000c480:	4d415244 	.word	0x4d415244
3000c484:	00000000 	.word	0x00000000
3000c488:	20344d4b 	.word	0x20344d4b
3000c48c:	20504958 	.word	0x20504958
3000c490:	00474d49 	.word	0x00474d49
3000c494:	20344d4b 	.word	0x20344d4b
3000c498:	4d415253 	.word	0x4d415253
3000c49c:	00000000 	.word	0x00000000
3000c4a0:	20344d4b 	.word	0x20344d4b
3000c4a4:	4d415244 	.word	0x4d415244
3000c4a8:	00000000 	.word	0x00000000
3000c4ac:	42205041 	.word	0x42205041
3000c4b0:	5320314c 	.word	0x5320314c
3000c4b4:	004d4152 	.word	0x004d4152
3000c4b8:	42205041 	.word	0x42205041
3000c4bc:	4420314c 	.word	0x4420314c
3000c4c0:	004d4152 	.word	0x004d4152
3000c4c4:	46205041 	.word	0x46205041
3000c4c8:	00005049 	.word	0x00005049
3000c4cc:	444f4d5b 	.word	0x444f4d5b
3000c4d0:	5f454c55 	.word	0x5f454c55
3000c4d4:	544f4f42 	.word	0x544f4f42
3000c4d8:	56454c2d 	.word	0x56454c2d
3000c4dc:	495f4c45 	.word	0x495f4c45
3000c4e0:	5d4f464e 	.word	0x5d4f464e
3000c4e4:	2073253a 	.word	0x2073253a
3000c4e8:	61766e49 	.word	0x61766e49
3000c4ec:	0a64696c 	.word	0x0a64696c
3000c4f0:	00000000 	.word	0x00000000
3000c4f4:	444f4d5b 	.word	0x444f4d5b
3000c4f8:	5f454c55 	.word	0x5f454c55
3000c4fc:	544f4f42 	.word	0x544f4f42
3000c500:	56454c2d 	.word	0x56454c2d
3000c504:	495f4c45 	.word	0x495f4c45
3000c508:	5d4f464e 	.word	0x5d4f464e
3000c50c:	5b73253a 	.word	0x5b73253a
3000c510:	78383025 	.word	0x78383025
3000c514:	5d78253a 	.word	0x5d78253a
3000c518:	0000000a 	.word	0x0000000a
3000c51c:	444f4d5b 	.word	0x444f4d5b
3000c520:	5f454c55 	.word	0x5f454c55
3000c524:	544f4f42 	.word	0x544f4f42
3000c528:	56454c2d 	.word	0x56454c2d
3000c52c:	495f4c45 	.word	0x495f4c45
3000c530:	5d4f464e 	.word	0x5d4f464e
3000c534:	5044523a 	.word	0x5044523a
3000c538:	0a4e4520 	.word	0x0a4e4520
3000c53c:	00000000 	.word	0x00000000
3000c540:	444f4d5b 	.word	0x444f4d5b
3000c544:	5f454c55 	.word	0x5f454c55
3000c548:	544f4f42 	.word	0x544f4f42
3000c54c:	56454c2d 	.word	0x56454c2d
3000c550:	455f4c45 	.word	0x455f4c45
3000c554:	524f5252 	.word	0x524f5252
3000c558:	61463a5d 	.word	0x61463a5d
3000c55c:	74206c69 	.word	0x74206c69
3000c560:	6f6c206f 	.word	0x6f6c206f
3000c564:	52206461 	.word	0x52206461
3000c568:	69205044 	.word	0x69205044
3000c56c:	6567616d 	.word	0x6567616d
3000c570:	00000a21 	.word	0x00000a21
3000c574:	474d490d 	.word	0x474d490d
3000c578:	544f2032 	.word	0x544f2032
3000c57c:	4e452046 	.word	0x4e452046
3000c580:	0000000a 	.word	0x0000000a
3000c584:	46544f0d 	.word	0x46544f0d
3000c588:	646f4d20 	.word	0x646f4d20
3000c58c:	72652065 	.word	0x72652065
3000c590:	0d726f72 	.word	0x0d726f72
3000c594:	0000000a 	.word	0x0000000a
3000c598:	494c4156 	.word	0x494c4156
3000c59c:	00000044 	.word	0x00000044
3000c5a0:	41564e49 	.word	0x41564e49
3000c5a4:	0044494c 	.word	0x0044494c
3000c5a8:	74726563 	.word	0x74726563
3000c5ac:	63696669 	.word	0x63696669
3000c5b0:	20657461 	.word	0x20657461
3000c5b4:	20434345 	.word	0x20434345
3000c5b8:	69726576 	.word	0x69726576
3000c5bc:	66207966 	.word	0x66207966
3000c5c0:	2c6c6961 	.word	0x2c6c6961
3000c5c4:	6c6f7220 	.word	0x6c6f7220
3000c5c8:	6361626c 	.word	0x6361626c
3000c5cc:	000a0d6b 	.word	0x000a0d6b
3000c5d0:	444f4d5b 	.word	0x444f4d5b
3000c5d4:	5f454c55 	.word	0x5f454c55
3000c5d8:	544f4f42 	.word	0x544f4f42
3000c5dc:	56454c2d 	.word	0x56454c2d
3000c5e0:	495f4c45 	.word	0x495f4c45
3000c5e4:	5d4f464e 	.word	0x5d4f464e
3000c5e8:	474d493a 	.word	0x474d493a
3000c5ec:	544f2832 	.word	0x544f2832
3000c5f0:	29642541 	.word	0x29642541
3000c5f4:	2c732520 	.word	0x2c732520
3000c5f8:	74657220 	.word	0x74657220
3000c5fc:	6425203a 	.word	0x6425203a
3000c600:	0000000a 	.word	0x0000000a
3000c604:	32474d49 	.word	0x32474d49
3000c608:	43434520 	.word	0x43434520
3000c60c:	72657620 	.word	0x72657620
3000c610:	20796669 	.word	0x20796669
3000c614:	6c696166 	.word	0x6c696166
3000c618:	6f72202c 	.word	0x6f72202c
3000c61c:	61626c6c 	.word	0x61626c6c
3000c620:	0a0d6b63 	.word	0x0a0d6b63
3000c624:	00000000 	.word	0x00000000
3000c628:	5f41544f 	.word	0x5f41544f
3000c62c:	45205041 	.word	0x45205041
3000c630:	76204343 	.word	0x76204343
3000c634:	66697265 	.word	0x66697265
3000c638:	61662079 	.word	0x61662079
3000c63c:	202c6c69 	.word	0x202c6c69
3000c640:	6c6c6f72 	.word	0x6c6c6f72
3000c644:	6b636162 	.word	0x6b636162
3000c648:	00000a0d 	.word	0x00000a0d
3000c64c:	64616f4c 	.word	0x64616f4c
3000c650:	41544f20 	.word	0x41544f20
3000c654:	2050415f 	.word	0x2050415f
3000c658:	67616d69 	.word	0x67616d69
3000c65c:	61662065 	.word	0x61662065
3000c660:	202c6c69 	.word	0x202c6c69
3000c664:	6c6c6f72 	.word	0x6c6c6f72
3000c668:	6b636162 	.word	0x6b636162
3000c66c:	00000a0d 	.word	0x00000a0d
3000c670:	444f4d5b 	.word	0x444f4d5b
3000c674:	5f454c55 	.word	0x5f454c55
3000c678:	544f4f42 	.word	0x544f4f42
3000c67c:	56454c2d 	.word	0x56454c2d
3000c680:	455f4c45 	.word	0x455f4c45
3000c684:	524f5252 	.word	0x524f5252
3000c688:	61463a5d 	.word	0x61463a5d
3000c68c:	74206c69 	.word	0x74206c69
3000c690:	6f6c206f 	.word	0x6f6c206f
3000c694:	41206461 	.word	0x41206461
3000c698:	6d692050 	.word	0x6d692050
3000c69c:	21656761 	.word	0x21656761
3000c6a0:	0000000a 	.word	0x0000000a
3000c6a4:	20344d4b 	.word	0x20344d4b
3000c6a8:	33474d49 	.word	0x33474d49
3000c6ac:	00000000 	.word	0x00000000
3000c6b0:	20344d4b 	.word	0x20344d4b
3000c6b4:	0043534e 	.word	0x0043534e
3000c6b8:	31474d49 	.word	0x31474d49
3000c6bc:	00000000 	.word	0x00000000
3000c6c0:	32474d49 	.word	0x32474d49
3000c6c4:	00000000 	.word	0x00000000
3000c6c8:	42205041 	.word	0x42205041
3000c6cc:	4626314c 	.word	0x4626314c
3000c6d0:	00005049 	.word	0x00005049
3000c6d4:	444f4d5b 	.word	0x444f4d5b
3000c6d8:	5f454c55 	.word	0x5f454c55
3000c6dc:	544f4f42 	.word	0x544f4f42
3000c6e0:	56454c2d 	.word	0x56454c2d
3000c6e4:	455f4c45 	.word	0x455f4c45
3000c6e8:	524f5252 	.word	0x524f5252
3000c6ec:	68543a5d 	.word	0x68543a5d
3000c6f0:	20657265 	.word	0x20657265
3000c6f4:	206e6163 	.word	0x206e6163
3000c6f8:	61206562 	.word	0x61206562
3000c6fc:	6f6d2074 	.word	0x6f6d2074
3000c700:	38207473 	.word	0x38207473
3000c704:	49535220 	.word	0x49535220
3000c708:	616d2050 	.word	0x616d2050
3000c70c:	65206b73 	.word	0x65206b73
3000c710:	6972746e 	.word	0x6972746e
3000c714:	0a217365 	.word	0x0a217365
3000c718:	00000000 	.word	0x00000000
3000c71c:	444f4d5b 	.word	0x444f4d5b
3000c720:	5f454c55 	.word	0x5f454c55
3000c724:	544f4f42 	.word	0x544f4f42
3000c728:	56454c2d 	.word	0x56454c2d
3000c72c:	455f4c45 	.word	0x455f4c45
3000c730:	524f5252 	.word	0x524f5252
3000c734:	53523a5d 	.word	0x53523a5d
3000c738:	65205049 	.word	0x65205049
3000c73c:	7972746e 	.word	0x7972746e
3000c740:	64646120 	.word	0x64646120
3000c744:	78252072 	.word	0x78252072
3000c748:	766e6920 	.word	0x766e6920
3000c74c:	64696c61 	.word	0x64696c61
3000c750:	0000000a 	.word	0x0000000a
3000c754:	059e4eb3 	.word	0x059e4eb3
3000c758:	4b11ea39 	.word	0x4b11ea39
3000c75c:	2a1b920a 	.word	0x2a1b920a
3000c760:	1d939cee 	.word	0x1d939cee
3000c764:	00000000 	.word	0x00000000
3000c768:	42205041 	.word	0x42205041
3000c76c:	0032334c 	.word	0x0032334c
3000c770:	2073250d 	.word	0x2073250d
3000c774:	49524556 	.word	0x49524556
3000c778:	50205946 	.word	0x50205946
3000c77c:	0a535341 	.word	0x0a535341
3000c780:	00000000 	.word	0x00000000
3000c784:	2073250d 	.word	0x2073250d
3000c788:	49524556 	.word	0x49524556
3000c78c:	46205946 	.word	0x46205946
3000c790:	2c4c4941 	.word	0x2c4c4941
3000c794:	74657220 	.word	0x74657220
3000c798:	25203d20 	.word	0x25203d20
3000c79c:	00000a64 	.word	0x00000a64
3000c7a0:	7265430d 	.word	0x7265430d
3000c7a4:	69666974 	.word	0x69666974
3000c7a8:	65746163 	.word	0x65746163
3000c7ac:	52455620 	.word	0x52455620
3000c7b0:	20594649 	.word	0x20594649
3000c7b4:	4c494146 	.word	0x4c494146
3000c7b8:	6572202c 	.word	0x6572202c
3000c7bc:	203d2074 	.word	0x203d2074
3000c7c0:	000a6425 	.word	0x000a6425
3000c7c4:	444f4d5b 	.word	0x444f4d5b
3000c7c8:	5f454c55 	.word	0x5f454c55
3000c7cc:	544f4f42 	.word	0x544f4f42
3000c7d0:	56454c2d 	.word	0x56454c2d
3000c7d4:	495f4c45 	.word	0x495f4c45
3000c7d8:	5d4f464e 	.word	0x5d4f464e
3000c7dc:	41544f3a 	.word	0x41544f3a
3000c7e0:	53552031 	.word	0x53552031
3000c7e4:	76202c45 	.word	0x76202c45
3000c7e8:	69737265 	.word	0x69737265
3000c7ec:	203a6e6f 	.word	0x203a6e6f
3000c7f0:	000a6425 	.word	0x000a6425
3000c7f4:	444f4d5b 	.word	0x444f4d5b
3000c7f8:	5f454c55 	.word	0x5f454c55
3000c7fc:	544f4f42 	.word	0x544f4f42
3000c800:	56454c2d 	.word	0x56454c2d
3000c804:	495f4c45 	.word	0x495f4c45
3000c808:	5d4f464e 	.word	0x5d4f464e
3000c80c:	41544f3a 	.word	0x41544f3a
3000c810:	53552032 	.word	0x53552032
3000c814:	76202c45 	.word	0x76202c45
3000c818:	69737265 	.word	0x69737265
3000c81c:	203a6e6f 	.word	0x203a6e6f
3000c820:	000a6425 	.word	0x000a6425
3000c824:	444f4d5b 	.word	0x444f4d5b
3000c828:	5f454c55 	.word	0x5f454c55
3000c82c:	544f4f42 	.word	0x544f4f42
3000c830:	56454c2d 	.word	0x56454c2d
3000c834:	495f4c45 	.word	0x495f4c45
3000c838:	5d4f464e 	.word	0x5d4f464e
3000c83c:	746f423a 	.word	0x746f423a
3000c840:	544f2068 	.word	0x544f2068
3000c844:	6d692041 	.word	0x6d692041
3000c848:	20656761 	.word	0x20656761
3000c84c:	69726576 	.word	0x69726576
3000c850:	66207966 	.word	0x66207966
3000c854:	656c6961 	.word	0x656c6961
3000c858:	00000a64 	.word	0x00000a64
3000c85c:	444f4d5b 	.word	0x444f4d5b
3000c860:	5f454c55 	.word	0x5f454c55
3000c864:	544f4f42 	.word	0x544f4f42
3000c868:	56454c2d 	.word	0x56454c2d
3000c86c:	495f4c45 	.word	0x495f4c45
3000c870:	5d4f464e 	.word	0x5d4f464e
3000c874:	696e493a 	.word	0x696e493a
3000c878:	42572074 	.word	0x42572074
3000c87c:	00000a0d 	.word	0x00000a0d
3000c880:	444f4d5b 	.word	0x444f4d5b
3000c884:	5f454c55 	.word	0x5f454c55
3000c888:	544f4f42 	.word	0x544f4f42
3000c88c:	56454c2d 	.word	0x56454c2d
3000c890:	495f4c45 	.word	0x495f4c45
3000c894:	5d4f464e 	.word	0x5d4f464e
3000c898:	4965523a 	.word	0x4965523a
3000c89c:	2074696e 	.word	0x2074696e
3000c8a0:	0d524444 	.word	0x0d524444
3000c8a4:	0000000a 	.word	0x0000000a
3000c8a8:	20315042 	.word	0x20315042
3000c8ac:	73726576 	.word	0x73726576
3000c8b0:	206e6f69 	.word	0x206e6f69
3000c8b4:	61766e69 	.word	0x61766e69
3000c8b8:	0a64696c 	.word	0x0a64696c
3000c8bc:	00000000 	.word	0x00000000
3000c8c0:	20315042 	.word	0x20315042
3000c8c4:	6d726f66 	.word	0x6d726f66
3000c8c8:	69207461 	.word	0x69207461
3000c8cc:	6c61766e 	.word	0x6c61766e
3000c8d0:	000a6469 	.word	0x000a6469
3000c8d4:	20315042 	.word	0x20315042
3000c8d8:	69746361 	.word	0x69746361
3000c8dc:	69206576 	.word	0x69206576
3000c8e0:	7865646e 	.word	0x7865646e
3000c8e4:	766e6920 	.word	0x766e6920
3000c8e8:	64696c61 	.word	0x64696c61
3000c8ec:	0000000a 	.word	0x0000000a
3000c8f0:	20315042 	.word	0x20315042
3000c8f4:	61746164 	.word	0x61746164
3000c8f8:	6c617620 	.word	0x6c617620
3000c8fc:	202c6469 	.word	0x202c6469
3000c900:	73726576 	.word	0x73726576
3000c904:	3a6e6f69 	.word	0x3a6e6f69
3000c908:	000a6425 	.word	0x000a6425
3000c90c:	20325042 	.word	0x20325042
3000c910:	6d726f66 	.word	0x6d726f66
3000c914:	69207461 	.word	0x69207461
3000c918:	6c61766e 	.word	0x6c61766e
3000c91c:	000a6469 	.word	0x000a6469
3000c920:	20315042 	.word	0x20315042
3000c924:	33435243 	.word	0x33435243
3000c928:	616d2032 	.word	0x616d2032
3000c92c:	2c686374 	.word	0x2c686374
3000c930:	74746120 	.word	0x74746120
3000c934:	65686361 	.word	0x65686361
3000c938:	52432064 	.word	0x52432064
3000c93c:	30203a43 	.word	0x30203a43
3000c940:	2c782578 	.word	0x2c782578
3000c944:	6c616320 	.word	0x6c616320
3000c948:	616c7563 	.word	0x616c7563
3000c94c:	20646574 	.word	0x20646574
3000c950:	3a435243 	.word	0x3a435243
3000c954:	25783020 	.word	0x25783020
3000c958:	00000a78 	.word	0x00000a78
3000c95c:	20315042 	.word	0x20315042
3000c960:	33435243 	.word	0x33435243
3000c964:	6f6e2032 	.word	0x6f6e2032
3000c968:	616d2074 	.word	0x616d2074
3000c96c:	2c686374 	.word	0x2c686374
3000c970:	74746120 	.word	0x74746120
3000c974:	65686361 	.word	0x65686361
3000c978:	52432064 	.word	0x52432064
3000c97c:	30203a43 	.word	0x30203a43
3000c980:	2c782578 	.word	0x2c782578
3000c984:	6c616320 	.word	0x6c616320
3000c988:	616c7563 	.word	0x616c7563
3000c98c:	20646574 	.word	0x20646574
3000c990:	3a435243 	.word	0x3a435243
3000c994:	25783020 	.word	0x25783020
3000c998:	00000a78 	.word	0x00000a78
3000c99c:	20325042 	.word	0x20325042
3000c9a0:	33435243 	.word	0x33435243
3000c9a4:	616d2032 	.word	0x616d2032
3000c9a8:	2c686374 	.word	0x2c686374
3000c9ac:	74746120 	.word	0x74746120
3000c9b0:	65686361 	.word	0x65686361
3000c9b4:	52432064 	.word	0x52432064
3000c9b8:	30203a43 	.word	0x30203a43
3000c9bc:	2c782578 	.word	0x2c782578
3000c9c0:	6c616320 	.word	0x6c616320
3000c9c4:	616c7563 	.word	0x616c7563
3000c9c8:	20646574 	.word	0x20646574
3000c9cc:	3a435243 	.word	0x3a435243
3000c9d0:	25783020 	.word	0x25783020
3000c9d4:	00000a78 	.word	0x00000a78
3000c9d8:	20325042 	.word	0x20325042
3000c9dc:	33435243 	.word	0x33435243
3000c9e0:	6f6e2032 	.word	0x6f6e2032
3000c9e4:	616d2074 	.word	0x616d2074
3000c9e8:	2c686374 	.word	0x2c686374
3000c9ec:	74746120 	.word	0x74746120
3000c9f0:	65686361 	.word	0x65686361
3000c9f4:	52432064 	.word	0x52432064
3000c9f8:	30203a43 	.word	0x30203a43
3000c9fc:	2c782578 	.word	0x2c782578
3000ca00:	6c616320 	.word	0x6c616320
3000ca04:	616c7563 	.word	0x616c7563
3000ca08:	20646574 	.word	0x20646574
3000ca0c:	3a435243 	.word	0x3a435243
3000ca10:	25783020 	.word	0x25783020
3000ca14:	00000a78 	.word	0x00000a78
3000ca18:	20315042 	.word	0x20315042
3000ca1c:	3161746f 	.word	0x3161746f
3000ca20:	64646120 	.word	0x64646120
3000ca24:	73736572 	.word	0x73736572
3000ca28:	766e6920 	.word	0x766e6920
3000ca2c:	64696c61 	.word	0x64696c61
3000ca30:	0000000a 	.word	0x0000000a
3000ca34:	20315042 	.word	0x20315042
3000ca38:	3261746f 	.word	0x3261746f
3000ca3c:	64646120 	.word	0x64646120
3000ca40:	73736572 	.word	0x73736572
3000ca44:	766e6920 	.word	0x766e6920
3000ca48:	64696c61 	.word	0x64696c61
3000ca4c:	0000000a 	.word	0x0000000a
3000ca50:	68746f42 	.word	0x68746f42
3000ca54:	20504220 	.word	0x20504220
3000ca58:	20435243 	.word	0x20435243
3000ca5c:	696c6176 	.word	0x696c6176
3000ca60:	00000a64 	.word	0x00000a64
3000ca64:	20315042 	.word	0x20315042
3000ca68:	656c6573 	.word	0x656c6573
3000ca6c:	64657463 	.word	0x64657463
3000ca70:	0000000a 	.word	0x0000000a
3000ca74:	20325042 	.word	0x20325042
3000ca78:	656c6573 	.word	0x656c6573
3000ca7c:	64657463 	.word	0x64657463
3000ca80:	0000000a 	.word	0x0000000a
3000ca84:	68746f42 	.word	0x68746f42
3000ca88:	20504220 	.word	0x20504220
3000ca8c:	61766e69 	.word	0x61766e69
3000ca90:	0a64696c 	.word	0x0a64696c
3000ca94:	00000000 	.word	0x00000000
3000ca98:	6f636552 	.word	0x6f636552
3000ca9c:	79726576 	.word	0x79726576
3000caa0:	61747320 	.word	0x61747320
3000caa4:	000a7472 	.word	0x000a7472
3000caa8:	69746341 	.word	0x69746341
3000caac:	49206576 	.word	0x49206576
3000cab0:	7865646e 	.word	0x7865646e
3000cab4:	6425203a 	.word	0x6425203a
3000cab8:	0000000a 	.word	0x0000000a
3000cabc:	3141544f 	.word	0x3141544f
3000cac0:	766e6920 	.word	0x766e6920
3000cac4:	64696c61 	.word	0x64696c61
3000cac8:	0000000a 	.word	0x0000000a
3000cacc:	3241544f 	.word	0x3241544f
3000cad0:	766e6920 	.word	0x766e6920
3000cad4:	64696c61 	.word	0x64696c61
3000cad8:	0000000a 	.word	0x0000000a
3000cadc:	3141544f 	.word	0x3141544f
3000cae0:	43524320 	.word	0x43524320
3000cae4:	6d203233 	.word	0x6d203233
3000cae8:	68637461 	.word	0x68637461
3000caec:	7461202c 	.word	0x7461202c
3000caf0:	68636174 	.word	0x68636174
3000caf4:	43206465 	.word	0x43206465
3000caf8:	203a4352 	.word	0x203a4352
3000cafc:	78257830 	.word	0x78257830
3000cb00:	6163202c 	.word	0x6163202c
3000cb04:	6c75636c 	.word	0x6c75636c
3000cb08:	64657461 	.word	0x64657461
3000cb0c:	43524320 	.word	0x43524320
3000cb10:	7830203a 	.word	0x7830203a
3000cb14:	000a7825 	.word	0x000a7825
3000cb18:	3141544f 	.word	0x3141544f
3000cb1c:	43524320 	.word	0x43524320
3000cb20:	6e203233 	.word	0x6e203233
3000cb24:	6d20746f 	.word	0x6d20746f
3000cb28:	68637461 	.word	0x68637461
3000cb2c:	7461202c 	.word	0x7461202c
3000cb30:	68636174 	.word	0x68636174
3000cb34:	43206465 	.word	0x43206465
3000cb38:	203a4352 	.word	0x203a4352
3000cb3c:	78257830 	.word	0x78257830
3000cb40:	6163202c 	.word	0x6163202c
3000cb44:	6c75636c 	.word	0x6c75636c
3000cb48:	64657461 	.word	0x64657461
3000cb4c:	43524320 	.word	0x43524320
3000cb50:	7830203a 	.word	0x7830203a
3000cb54:	202c7825 	.word	0x202c7825
3000cb58:	73617265 	.word	0x73617265
3000cb5c:	544f2065 	.word	0x544f2065
3000cb60:	000a3141 	.word	0x000a3141
3000cb64:	3241544f 	.word	0x3241544f
3000cb68:	43524320 	.word	0x43524320
3000cb6c:	6d203233 	.word	0x6d203233
3000cb70:	68637461 	.word	0x68637461
3000cb74:	7461202c 	.word	0x7461202c
3000cb78:	68636174 	.word	0x68636174
3000cb7c:	43206465 	.word	0x43206465
3000cb80:	203a4352 	.word	0x203a4352
3000cb84:	78257830 	.word	0x78257830
3000cb88:	6163202c 	.word	0x6163202c
3000cb8c:	6c75636c 	.word	0x6c75636c
3000cb90:	64657461 	.word	0x64657461
3000cb94:	43524320 	.word	0x43524320
3000cb98:	7830203a 	.word	0x7830203a
3000cb9c:	000a7825 	.word	0x000a7825
3000cba0:	3241544f 	.word	0x3241544f
3000cba4:	43524320 	.word	0x43524320
3000cba8:	6e203233 	.word	0x6e203233
3000cbac:	6d20746f 	.word	0x6d20746f
3000cbb0:	68637461 	.word	0x68637461
3000cbb4:	7461202c 	.word	0x7461202c
3000cbb8:	68636174 	.word	0x68636174
3000cbbc:	43206465 	.word	0x43206465
3000cbc0:	203a4352 	.word	0x203a4352
3000cbc4:	78257830 	.word	0x78257830
3000cbc8:	6163202c 	.word	0x6163202c
3000cbcc:	6c75636c 	.word	0x6c75636c
3000cbd0:	64657461 	.word	0x64657461
3000cbd4:	43524320 	.word	0x43524320
3000cbd8:	7830203a 	.word	0x7830203a
3000cbdc:	202c7825 	.word	0x202c7825
3000cbe0:	73617265 	.word	0x73617265
3000cbe4:	544f2065 	.word	0x544f2065
3000cbe8:	000a3241 	.word	0x000a3241
3000cbec:	69746341 	.word	0x69746341
3000cbf0:	69206576 	.word	0x69206576
3000cbf4:	7865646e 	.word	0x7865646e
3000cbf8:	41544f20 	.word	0x41544f20
3000cbfc:	766e6920 	.word	0x766e6920
3000cc00:	64696c61 	.word	0x64696c61
3000cc04:	6572202c 	.word	0x6572202c
3000cc08:	74697277 	.word	0x74697277
3000cc0c:	63612065 	.word	0x63612065
3000cc10:	65766974 	.word	0x65766974
3000cc14:	646e6920 	.word	0x646e6920
3000cc18:	74207865 	.word	0x74207865
3000cc1c:	6425206f 	.word	0x6425206f
3000cc20:	0000000a 	.word	0x0000000a
3000cc24:	206c6c41 	.word	0x206c6c41
3000cc28:	2041544f 	.word	0x2041544f
3000cc2c:	72646461 	.word	0x72646461
3000cc30:	65737365 	.word	0x65737365
3000cc34:	6f6e2073 	.word	0x6f6e2073
3000cc38:	61762074 	.word	0x61762074
3000cc3c:	2c64696c 	.word	0x2c64696c
3000cc40:	63657220 	.word	0x63657220
3000cc44:	7265766f 	.word	0x7265766f
3000cc48:	61662079 	.word	0x61662079
3000cc4c:	000a6c69 	.word	0x000a6c69
3000cc50:	20325042 	.word	0x20325042
3000cc54:	3261746f 	.word	0x3261746f
3000cc58:	64646120 	.word	0x64646120
3000cc5c:	73736572 	.word	0x73736572
3000cc60:	766e6920 	.word	0x766e6920
3000cc64:	64696c61 	.word	0x64696c61
3000cc68:	0000000a 	.word	0x0000000a
3000cc6c:	20325042 	.word	0x20325042
3000cc70:	73726576 	.word	0x73726576
3000cc74:	206e6f69 	.word	0x206e6f69
3000cc78:	61766e69 	.word	0x61766e69
3000cc7c:	0a64696c 	.word	0x0a64696c
3000cc80:	00000000 	.word	0x00000000
3000cc84:	20325042 	.word	0x20325042
3000cc88:	69746361 	.word	0x69746361
3000cc8c:	69206576 	.word	0x69206576
3000cc90:	7865646e 	.word	0x7865646e
3000cc94:	766e6920 	.word	0x766e6920
3000cc98:	64696c61 	.word	0x64696c61
3000cc9c:	0000000a 	.word	0x0000000a
3000cca0:	20325042 	.word	0x20325042
3000cca4:	3161746f 	.word	0x3161746f
3000cca8:	64646120 	.word	0x64646120
3000ccac:	73736572 	.word	0x73736572
3000ccb0:	766e6920 	.word	0x766e6920
3000ccb4:	64696c61 	.word	0x64696c61
3000ccb8:	0000000a 	.word	0x0000000a
3000ccbc:	20325042 	.word	0x20325042
3000ccc0:	61746164 	.word	0x61746164
3000ccc4:	6c617620 	.word	0x6c617620
3000ccc8:	202c6469 	.word	0x202c6469
3000cccc:	73726576 	.word	0x73726576
3000ccd0:	3a6e6f69 	.word	0x3a6e6f69
3000ccd4:	000a6425 	.word	0x000a6425
3000ccd8:	444f4d5b 	.word	0x444f4d5b
3000ccdc:	5f454c55 	.word	0x5f454c55
3000cce0:	544f4f42 	.word	0x544f4f42
3000cce4:	56454c2d 	.word	0x56454c2d
3000cce8:	495f4c45 	.word	0x495f4c45
3000ccec:	5d4f464e 	.word	0x5d4f464e
3000ccf0:	7369443a 	.word	0x7369443a
3000ccf4:	656c6261 	.word	0x656c6261
3000ccf8:	304d4b20 	.word	0x304d4b20
3000ccfc:	6f6f4220 	.word	0x6f6f4220
3000cd00:	69202074 	.word	0x69202074
3000cd04:	6554206e 	.word	0x6554206e
3000cd08:	6d207473 	.word	0x6d207473
3000cd0c:	0d65646f 	.word	0x0d65646f
3000cd10:	0000000a 	.word	0x0000000a
3000cd14:	444f4d5b 	.word	0x444f4d5b
3000cd18:	5f454c55 	.word	0x5f454c55
3000cd1c:	544f4f42 	.word	0x544f4f42
3000cd20:	56454c2d 	.word	0x56454c2d
3000cd24:	495f4c45 	.word	0x495f4c45
3000cd28:	5d4f464e 	.word	0x5d4f464e
3000cd2c:	7079423a 	.word	0x7079423a
3000cd30:	20737361 	.word	0x20737361
3000cd34:	2050544f 	.word	0x2050544f
3000cd38:	6f747561 	.word	0x6f747561
3000cd3c:	64616f6c 	.word	0x64616f6c
3000cd40:	00000a0d 	.word	0x00000a0d
3000cd44:	444f4d5b 	.word	0x444f4d5b
3000cd48:	5f454c55 	.word	0x5f454c55
3000cd4c:	544f4f42 	.word	0x544f4f42
3000cd50:	56454c2d 	.word	0x56454c2d
3000cd54:	495f4c45 	.word	0x495f4c45
3000cd58:	5d4f464e 	.word	0x5d4f464e
3000cd5c:	20504e3a 	.word	0x20504e3a
3000cd60:	71657246 	.word	0x71657246
3000cd64:	20642520 	.word	0x20642520
3000cd68:	0a7a484d 	.word	0x0a7a484d
3000cd6c:	00000000 	.word	0x00000000
3000cd70:	444f4d5b 	.word	0x444f4d5b
3000cd74:	5f454c55 	.word	0x5f454c55
3000cd78:	544f4f42 	.word	0x544f4f42
3000cd7c:	56454c2d 	.word	0x56454c2d
3000cd80:	495f4c45 	.word	0x495f4c45
3000cd84:	5d4f464e 	.word	0x5d4f464e
3000cd88:	2050413a 	.word	0x2050413a
3000cd8c:	71657246 	.word	0x71657246
3000cd90:	20642520 	.word	0x20642520
3000cd94:	0a7a484d 	.word	0x0a7a484d
3000cd98:	00000000 	.word	0x00000000
3000cd9c:	7361700a 	.word	0x7361700a
3000cda0:	726f7773 	.word	0x726f7773
3000cda4:	65762064 	.word	0x65762064
3000cda8:	79666972 	.word	0x79666972
3000cdac:	69616620 	.word	0x69616620
3000cdb0:	00000a6c 	.word	0x00000a6c
3000cdb4:	7361700a 	.word	0x7361700a
3000cdb8:	726f7773 	.word	0x726f7773
3000cdbc:	65762064 	.word	0x65762064
3000cdc0:	79666972 	.word	0x79666972
3000cdc4:	63757320 	.word	0x63757320
3000cdc8:	73736563 	.word	0x73736563
3000cdcc:	0000000a 	.word	0x0000000a
3000cdd0:	746f6f62 	.word	0x746f6f62
3000cdd4:	64616f6c 	.word	0x64616f6c
3000cdd8:	765f7265 	.word	0x765f7265
3000cddc:	69737265 	.word	0x69737265
3000cde0:	25206e6f 	.word	0x25206e6f
3000cde4:	00000a73 	.word	0x00000a73
3000cde8:	444f4d5b 	.word	0x444f4d5b
3000cdec:	5f454c55 	.word	0x5f454c55
3000cdf0:	544f4f42 	.word	0x544f4f42
3000cdf4:	56454c2d 	.word	0x56454c2d
3000cdf8:	495f4c45 	.word	0x495f4c45
3000cdfc:	5d4f464e 	.word	0x5d4f464e
3000ce00:	474d493a 	.word	0x474d493a
3000ce04:	4e452031 	.word	0x4e452031
3000ce08:	20524554 	.word	0x20524554
3000ce0c:	3a50534d 	.word	0x3a50534d
3000ce10:	3830255b 	.word	0x3830255b
3000ce14:	000a5d78 	.word	0x000a5d78
3000ce18:	444f4d5b 	.word	0x444f4d5b
3000ce1c:	5f454c55 	.word	0x5f454c55
3000ce20:	544f4f42 	.word	0x544f4f42
3000ce24:	56454c2d 	.word	0x56454c2d
3000ce28:	495f4c45 	.word	0x495f4c45
3000ce2c:	5d4f464e 	.word	0x5d4f464e
3000ce30:	474d493a 	.word	0x474d493a
3000ce34:	45532031 	.word	0x45532031
3000ce38:	45525543 	.word	0x45525543
3000ce3c:	41545320 	.word	0x41545320
3000ce40:	203a4554 	.word	0x203a4554
3000ce44:	000a6425 	.word	0x000a6425
3000ce48:	444f4d5b 	.word	0x444f4d5b
3000ce4c:	5f454c55 	.word	0x5f454c55
3000ce50:	544f4f42 	.word	0x544f4f42
3000ce54:	56454c2d 	.word	0x56454c2d
3000ce58:	495f4c45 	.word	0x495f4c45
3000ce5c:	5d4f464e 	.word	0x5d4f464e
3000ce60:	696e493a 	.word	0x696e493a
3000ce64:	53502074 	.word	0x53502074
3000ce68:	0d4d4152 	.word	0x0d4d4152
3000ce6c:	0000000a 	.word	0x0000000a
3000ce70:	7465730d 	.word	0x7465730d
3000ce74:	6d667020 	.word	0x6d667020
3000ce78:	69616620 	.word	0x69616620
3000ce7c:	000a0d6c 	.word	0x000a0d6c
3000ce80:	444f4d5b 	.word	0x444f4d5b
3000ce84:	5f454c55 	.word	0x5f454c55
3000ce88:	544f4f42 	.word	0x544f4f42
3000ce8c:	56454c2d 	.word	0x56454c2d
3000ce90:	495f4c45 	.word	0x495f4c45
3000ce94:	5d4f464e 	.word	0x5d4f464e
3000ce98:	5252453a 	.word	0x5252453a
3000ce9c:	2121524f 	.word	0x2121524f
3000cea0:	6f685320 	.word	0x6f685320
3000cea4:	20646c75 	.word	0x20646c75
3000cea8:	20746f4e 	.word	0x20746f4e
3000ceac:	62616e65 	.word	0x62616e65
3000ceb0:	4d20656c 	.word	0x4d20656c
3000ceb4:	77536d65 	.word	0x77536d65
3000ceb8:	6e4f5f72 	.word	0x6e4f5f72
3000cebc:	6920796c 	.word	0x6920796c
3000cec0:	4444206e 	.word	0x4444206e
3000cec4:	68432052 	.word	0x68432052
3000cec8:	21217069 	.word	0x21217069
3000cecc:	000a0d21 	.word	0x000a0d21
3000ced0:	444f4d5b 	.word	0x444f4d5b
3000ced4:	5f454c55 	.word	0x5f454c55
3000ced8:	544f4f42 	.word	0x544f4f42
3000cedc:	56454c2d 	.word	0x56454c2d
3000cee0:	495f4c45 	.word	0x495f4c45
3000cee4:	5d4f464e 	.word	0x5d4f464e
3000cee8:	696e493a 	.word	0x696e493a
3000ceec:	44442074 	.word	0x44442074
3000cef0:	000a0d52 	.word	0x000a0d52
3000cef4:	20445753 	.word	0x20445753
3000cef8:	64207369 	.word	0x64207369
3000cefc:	62617369 	.word	0x62617369
3000cf00:	2c64656c 	.word	0x2c64656c
3000cf04:	6f727020 	.word	0x6f727020
3000cf08:	64656563 	.word	0x64656563
3000cf0c:	206f7420 	.word	0x206f7420
3000cf10:	61736964 	.word	0x61736964
3000cf14:	20656c62 	.word	0x20656c62
3000cf18:	74726155 	.word	0x74726155
3000cf1c:	776f4420 	.word	0x776f4420
3000cf20:	616f6c6e 	.word	0x616f6c6e
3000cf24:	2e2e2e64 	.word	0x2e2e2e64
3000cf28:	0000000a 	.word	0x0000000a
3000cf2c:	74726155 	.word	0x74726155
3000cf30:	776f4420 	.word	0x776f4420
3000cf34:	616f6c6e 	.word	0x616f6c6e
3000cf38:	61682064 	.word	0x61682064
3000cf3c:	65622073 	.word	0x65622073
3000cf40:	64206e65 	.word	0x64206e65
3000cf44:	62617369 	.word	0x62617369
3000cf48:	0a64656c 	.word	0x0a64656c
3000cf4c:	00000000 	.word	0x00000000
3000cf50:	4848530a 	.word	0x4848530a
3000cf54:	4946544f 	.word	0x4946544f
3000cf58:	6f6e2058 	.word	0x6f6e2058
3000cf5c:	65722074 	.word	0x65722074
3000cf60:	76696563 	.word	0x76696563
3000cf64:	000a6465 	.word	0x000a6465
3000cf68:	726f4e0a 	.word	0x726f4e0a
3000cf6c:	206c616d 	.word	0x206c616d
3000cf70:	746f6f62 	.word	0x746f6f62
3000cf74:	0000000a 	.word	0x0000000a
3000cf78:	5f6e6962 	.word	0x5f6e6962
3000cf7c:	313e6f6e 	.word	0x313e6f6e
3000cf80:	3d202c30 	.word	0x3d202c30
3000cf84:	0a642520 	.word	0x0a642520
3000cf88:	00000000 	.word	0x00000000
3000cf8c:	6e206164 	.word	0x6e206164
3000cf90:	7220746f 	.word	0x7220746f
3000cf94:	69656365 	.word	0x69656365
3000cf98:	0a646576 	.word	0x0a646576
3000cf9c:	00000000 	.word	0x00000000
3000cfa0:	746f680a 	.word	0x746f680a
3000cfa4:	20786966 	.word	0x20786966
3000cfa8:	656e6f64 	.word	0x656e6f64
3000cfac:	202e2e2e 	.word	0x202e2e2e
3000cfb0:	6f626552 	.word	0x6f626552
3000cfb4:	6e69746f 	.word	0x6e69746f
3000cfb8:	0a2e2e67 	.word	0x0a2e2e67
3000cfbc:	00000000 	.word	0x00000000
3000cfc0:	444f4d5b 	.word	0x444f4d5b
3000cfc4:	5f454c55 	.word	0x5f454c55
3000cfc8:	544f4f42 	.word	0x544f4f42
3000cfcc:	56454c2d 	.word	0x56454c2d
3000cfd0:	495f4c45 	.word	0x495f4c45
3000cfd4:	5d4f464e 	.word	0x5d4f464e
3000cfd8:	6174533a 	.word	0x6174533a
3000cfdc:	4e207472 	.word	0x4e207472
3000cfe0:	65536e6f 	.word	0x65536e6f
3000cfe4:	65727563 	.word	0x65727563
3000cfe8:	30204020 	.word	0x30204020
3000cfec:	20782578 	.word	0x20782578
3000cff0:	0d2e2e2e 	.word	0x0d2e2e2e
3000cff4:	0000000a 	.word	0x0000000a
3000cff8:	6c630a0d 	.word	0x6c630a0d
3000cffc:	2065736f 	.word	0x2065736f
3000d000:	646f4d79 	.word	0x646f4d79
3000d004:	54206d65 	.word	0x54206d65
3000d008:	736e6172 	.word	0x736e6172
3000d00c:	20726566 	.word	0x20726566
3000d010:	0d2e2e2e 	.word	0x0d2e2e2e
3000d014:	0000000a 	.word	0x0000000a
3000d018:	766e490d 	.word	0x766e490d
3000d01c:	64696c61 	.word	0x64696c61
3000d020:	69684320 	.word	0x69684320
3000d024:	666e696e 	.word	0x666e696e
3000d028:	4320216f 	.word	0x4320216f
3000d02c:	6b636568 	.word	0x6b636568
3000d030:	50544f20 	.word	0x50544f20
3000d034:	0000000a 	.word	0x0000000a
3000d038:	656c500d 	.word	0x656c500d
3000d03c:	20657361 	.word	0x20657361
3000d040:	676f7270 	.word	0x676f7270
3000d044:	206d6172 	.word	0x206d6172
3000d048:	70696863 	.word	0x70696863
3000d04c:	6f666e69 	.word	0x6f666e69
3000d050:	206e6920 	.word	0x206e6920
3000d054:	2050544f 	.word	0x2050544f
3000d058:	00000a21 	.word	0x00000a21

3000d05c <Memory_Info>:
3000d05c:	03e00007 00060004 010202c0 02010100     ................
3000d06c:	02000102 00040302 02040400 01010102     ................
3000d07c:	01020221 02240401 03000102 00030103     !.....$.........
3000d08c:	01050500 06000002 01020206 01070700     ................
3000d09c:	08000002 01020208 02280801 09000102     ..........(.....
3000d0ac:	00040309 03290901 0a000004 0003010a     ......).........
3000d0bc:	012a0a01 0a010003 0003012a 020b0b00     ..*.....*.......
3000d0cc:	0c000102 0102020c ffffffff 0000ffff     ................
3000d0dc:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d0ec:	455f4c45 524f5252 52443a5d 69204d41     EL_ERROR]:DRAM i
3000d0fc:	6f6e2073 61432074 7262696c 6e6f6961     s not Calibraion
3000d10c:	00000a0d                                ....

3000d110 <Flash_AVL>:
3000d110:	000000ef 000000ff 00000000 000043fc     .............C..
3000d120:	00000000 000000a1 000000ff 00000000     ................
3000d130:	0000fffc 00000000 0000000b 000000ff     ................
3000d140:	00000000 000043fc 00000000 0000000e     .....C..........
3000d150:	000000ff 00000000 000043fc 00000000     .........C......
3000d160:	000000c8 000000ff 00000001 000043fc     .............C..
3000d170:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d180:	000200fc 00000000 000000c2 000000ff     ................
3000d190:	00000002 000000fc 00000000 00000068     ............h...
3000d1a0:	000000ff 00000002 000000fc 00000000     ................
3000d1b0:	00000051 000000ff 00000002 000000fc     Q...............
3000d1c0:	00000000 0000001c 000000ff 00000003     ................
3000d1d0:	000000fc 00000000 00000020 000000ff     ........ .......
3000d1e0:	00000004 000000fc 00000000 00000000     ................
3000d1f0:	000000ff 000000fe ffffffff 30008ab5     ...............0
3000d200:	000000ff ffffffff 000000ff ffffffff     ................
3000d210:	00000000                                ....

3000d214 <Flash_ReadMode>:
3000d214:	                                         ..

3000d216 <Flash_Speed>:
3000d216:	                                         ..

3000d218 <NAND_AVL>:
3000d218:	000000ef 000000ff 00000000 ffffffff     ................
3000d228:	00000000 000000c8 000000ff 00000001     ................
3000d238:	ffffffff 00000000 000000e5 000000ff     ................
3000d248:	00000001 ffffffff 00000000 000000c2     ................
3000d258:	000000ff 00000002 ffffffff 00000000     ................
3000d268:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d280:	000000ff 000000fe ffffffff 30008b15     ...............0
3000d290:	000000ff ffffffff 000000ff ffffffff     ................
3000d2a0:	00000000 3000c190 3000c194 3000c198     .......0...0...0
3000d2b0:	3000c19c 3000c1a0                       ...0...0

3000d2b8 <__FUNCTION__.0>:
3000d2b8:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d2c8 <__FUNCTION__.1>:
3000d2c8:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d2d8:	00000000 444f4d5b 5f454c55 544f4f42     ....[MODULE_BOOT
3000d2e8:	56454c2d 455f4c45 524f5252 6c423a5d     -LEVEL_ERROR]:Bl
3000d2f8:	7830206b 69207825 42422073 00000a21     k 0x%x is BB!...
3000d308:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d318:	495f4c45 5d4f464e 6165523a 61502064     EL_INFO]:Read Pa
3000d328:	30206567 20782578 4220666f 6b636f6c     ge 0x%x of Block
3000d338:	25783020 614d2078 61462079 202c6c69      0x%x May Fail, 
3000d348:	74617473 30207375 21782578 0000000a     status 0x%x!....
3000d358:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d368:	52455f4c 5d524f52 50544f3a 6165525f     L_ERROR]:OTP_Rea
3000d378:	46203864 206c6961 0a207825 00000000     d8 Fail %x .....
3000d388:	444f4d5b 5f454c55 2d50544f 4556454c     [MODULE_OTP-LEVE
3000d398:	52455f4c 5d524f52 524d503a 61655220     L_ERROR]:PMR Rea
3000d3a8:	72652064 21726f72 0000000a 444f4d5b     d error!....[MOD
3000d3b8:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d3c8:	5d524f52 50544f3a 6972575f 20386574     ROR]:OTP_Write8 
3000d3d8:	6c696146 20782520 0000000a 444f4d5b     Fail %x ....[MOD
3000d3e8:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d3f8:	5d524f52 50544f3a 6f727020 6d617267     ROR]:OTP program
3000d408:	72726520 0a21726f 00000000 444f4d5b      error!.....[MOD
3000d418:	5f454c55 2d50544f 4556454c 52455f4c     ULE_OTP-LEVEL_ER
3000d428:	5d524f52 524d503a 65522032 65206461     ROR]:PMR2 Read e
3000d438:	726f7272 00000a21                       rror!...

3000d440 <__FUNCTION__.1>:
3000d440:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d450:	444f4d5b 5f454c55 544f4f42 56454c2d     [MODULE_BOOT-LEV
3000d460:	495f4c45 5d4f464e 5253503a 43204d41     EL_INFO]:PSRAM C
3000d470:	206c7274 3a4b4c43 20642520 0a207a48     trl CLK: %d Hz .
3000d480:	00000000 6c61430d 6e696d4e 25203d20     .....CalNmin = %
3000d490:	61432078 616d4e6c 203d2078 57207825     x CalNmax = %x W
3000d4a0:	6f646e69 7a695377 203d2065 70207825     indowSize = %x p
3000d4b0:	65736168 7825203a 00000a20 444f4d5b     hase: %x ...[MOD
3000d4c0:	5f454c55 45535049 454c2d43 5f4c4556     ULE_IPSEC-LEVEL_
3000d4d0:	4f525245 4e3a5d52 6874206f 61207369     ERROR]:No this a
3000d4e0:	5f687475 65707974 29642528 726f6620     uth_type(%d) for
3000d4f0:	41485320 00000a32 7172690d 5d64255b      SHA2....irq[%d]
3000d500:	69727020 7469726f 64252079 61687320      priority %d sha
3000d510:	3c206c6c 6425203d 0000000a              ll <= %d....

3000d51c <__FUNCTION__.0>:
3000d51c:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d52c:	00000000                                ....

3000d530 <__FUNCTION__.1>:
3000d530:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d540 <__FUNCTION__.2>:
3000d540:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d550:	32616873 696e695f 00000074              sha2_init...

3000d55c <__FUNCTION__.3>:
3000d55c:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d56c:	6f4d7974 44416564 00000000 5244440d     tyModeAD.....DDR
3000d57c:	7a695320 73692065 746f6e20 6e6f6320      Size is not con
3000d58c:	75676966 0a646572 00000000 5b203e0d     figured......> [
3000d59c:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d5ac:	63203a5d 5f756c6f 616d6572 69625f70     ]: colu_remap_bi
3000d5bc:	21207374 7264203d 635f6d61 5f756c6f     ts != dram_colu_
3000d5cc:	73746962 0a212121 00000000 5b203e0d     bits!!!......> [
3000d5dc:	52444441 4d455220 45205041 524f5252     ADDR REMAP ERROR
3000d5ec:	62203a5d 5f6b6e61 616d6572 69625f70     ]: bank_remap_bi
3000d5fc:	21207374 7264203d 625f6d61 5f6b6e61     ts != dram_bank_
3000d60c:	73746962 0a212121 00000000              bits!!!.....

3000d618 <__FUNCTION__.1>:
3000d618:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d628:	00000000 3000c464 3000c470 3000c47c     ....d..0p..0|..0
3000d638:	3000c488 3000c494 3000c4a0 3000c4ac     ...0...0...0...0
3000d648:	3000c4b8 3000c4c4 3000c4ac 3000c4b8     ...0...0...0...0
3000d658:	3000c6a4 3000c6b0 3000c6b8 3000c6c0     ...0...0...0...0
3000d668:	3000c6a8 3000c6c8 00000208 0000020c     ...0...0........
3000d678:	00000210 00000008 00000214 00000218     ................
3000d688:	0000021c 0000000c                       ........

3000d690 <__FUNCTION__.0>:
3000d690:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000d6a0 <km4_bootloader_rev>:
3000d6a0:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000d6b0:	355f7265 66316631 325f3866 2f333230     er_51f1ff8_2023/
3000d6c0:	302f3930 30312d36 3a37343a 00003233     09/06-10:47:32..
