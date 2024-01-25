
D:/Tizen-8730e/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
30003178:	0052      	lsls	r2, r2, #1
3000317a:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000317e:	3201      	adds	r2, #1
30003180:	b087      	sub	sp, #28
30003182:	461f      	mov	r7, r3
30003184:	4b4f      	ldr	r3, [pc, #316]	; (300032c4 <FLASH_CalibrationNew+0x14c>)
30003186:	9101      	str	r1, [sp, #4]
30003188:	fa5f f882 	uxtb.w	r8, r2
3000318c:	f89d 4040 	ldrb.w	r4, [sp, #64]	; 0x40
30003190:	f89d a044 	ldrb.w	sl, [sp, #68]	; 0x44
30003194:	9003      	str	r0, [sp, #12]
30003196:	4798      	blx	r3
30003198:	9901      	ldr	r1, [sp, #4]
3000319a:	2800      	cmp	r0, #0
3000319c:	f000 8083 	beq.w	300032a6 <FLASH_CalibrationNew+0x12e>
300031a0:	4849      	ldr	r0, [pc, #292]	; (300032c8 <FLASH_CalibrationNew+0x150>)
300031a2:	4a4a      	ldr	r2, [pc, #296]	; (300032cc <FLASH_CalibrationNew+0x154>)
300031a4:	4790      	blx	r2
300031a6:	4b48      	ldr	r3, [pc, #288]	; (300032c8 <FLASH_CalibrationNew+0x150>)
300031a8:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
300031ac:	0322      	lsls	r2, r4, #12
300031ae:	f04f 4182 	mov.w	r1, #1090519040	; 0x41000000
300031b2:	795c      	ldrb	r4, [r3, #5]
300031b4:	45c2      	cmp	sl, r8
300031b6:	b292      	uxth	r2, r2
300031b8:	6144      	str	r4, [r0, #20]
300031ba:	f8d0 411c 	ldr.w	r4, [r0, #284]	; 0x11c
300031be:	f424 4470 	bic.w	r4, r4, #61440	; 0xf000
300031c2:	ea42 0204 	orr.w	r2, r2, r4
300031c6:	f8c0 211c 	str.w	r2, [r0, #284]	; 0x11c
300031ca:	6c0a      	ldr	r2, [r1, #64]	; 0x40
300031cc:	f422 4200 	bic.w	r2, r2, #32768	; 0x8000
300031d0:	640a      	str	r2, [r1, #64]	; 0x40
300031d2:	6c0a      	ldr	r2, [r1, #64]	; 0x40
300031d4:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300031d8:	640a      	str	r2, [r1, #64]	; 0x40
300031da:	d871      	bhi.n	300032c0 <FLASH_CalibrationNew+0x148>
300031dc:	f04f 33ff 	mov.w	r3, #4294967295
300031e0:	2500      	movs	r5, #0
300031e2:	f8df b0fc 	ldr.w	fp, [pc, #252]	; 300032e0 <FLASH_CalibrationNew+0x168>
300031e6:	f04f 6400 	mov.w	r4, #134217728	; 0x8000000
300031ea:	4e39      	ldr	r6, [pc, #228]	; (300032d0 <FLASH_CalibrationNew+0x158>)
300031ec:	f8df 90f4 	ldr.w	r9, [pc, #244]	; 300032e4 <FLASH_CalibrationNew+0x16c>
300031f0:	9301      	str	r3, [sp, #4]
300031f2:	9302      	str	r3, [sp, #8]
300031f4:	e00e      	b.n	30003214 <FLASH_CalibrationNew+0x9c>
300031f6:	9b01      	ldr	r3, [sp, #4]
300031f8:	443d      	add	r5, r7
300031fa:	f8cd a008 	str.w	sl, [sp, #8]
300031fe:	3301      	adds	r3, #1
30003200:	d102      	bne.n	30003208 <FLASH_CalibrationNew+0x90>
30003202:	2501      	movs	r5, #1
30003204:	f8cd a004 	str.w	sl, [sp, #4]
30003208:	eb07 030a 	add.w	r3, r7, sl
3000320c:	fa5f fa83 	uxtb.w	sl, r3
30003210:	45c2      	cmp	sl, r8
30003212:	d83d      	bhi.n	30003290 <FLASH_CalibrationNew+0x118>
30003214:	4650      	mov	r0, sl
30003216:	f7ff ff63 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
3000321a:	6972      	ldr	r2, [r6, #20]
3000321c:	03d3      	lsls	r3, r2, #15
3000321e:	d507      	bpl.n	30003230 <FLASH_CalibrationNew+0xb8>
30003220:	f3bf 8f4f 	dsb	sy
30003224:	f8c6 425c 	str.w	r4, [r6, #604]	; 0x25c
30003228:	f3bf 8f4f 	dsb	sy
3000322c:	f3bf 8f6f 	isb	sy
30003230:	47d8      	blx	fp
30003232:	6821      	ldr	r1, [r4, #0]
30003234:	2208      	movs	r2, #8
30003236:	a804      	add	r0, sp, #16
30003238:	9104      	str	r1, [sp, #16]
3000323a:	6861      	ldr	r1, [r4, #4]
3000323c:	4b25      	ldr	r3, [pc, #148]	; (300032d4 <FLASH_CalibrationNew+0x15c>)
3000323e:	9105      	str	r1, [sp, #20]
30003240:	4649      	mov	r1, r9
30003242:	4798      	blx	r3
30003244:	2800      	cmp	r0, #0
30003246:	d0d6      	beq.n	300031f6 <FLASH_CalibrationNew+0x7e>
30003248:	2d00      	cmp	r5, #0
3000324a:	dddd      	ble.n	30003208 <FLASH_CalibrationNew+0x90>
3000324c:	9b02      	ldr	r3, [sp, #8]
3000324e:	061c      	lsls	r4, r3, #24
30003250:	9b01      	ldr	r3, [sp, #4]
30003252:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
30003256:	432c      	orrs	r4, r5
30003258:	9b01      	ldr	r3, [sp, #4]
3000325a:	eb03 0565 	add.w	r5, r3, r5, asr #1
3000325e:	9b03      	ldr	r3, [sp, #12]
30003260:	f883 5061 	strb.w	r5, [r3, #97]	; 0x61
30003264:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30003268:	4d17      	ldr	r5, [pc, #92]	; (300032c8 <FLASH_CalibrationNew+0x150>)
3000326a:	6c13      	ldr	r3, [r2, #64]	; 0x40
3000326c:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30003270:	6413      	str	r3, [r2, #64]	; 0x40
30003272:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30003276:	79aa      	ldrb	r2, [r5, #6]
30003278:	615a      	str	r2, [r3, #20]
3000327a:	4b12      	ldr	r3, [pc, #72]	; (300032c4 <FLASH_CalibrationNew+0x14c>)
3000327c:	4798      	blx	r3
3000327e:	b1b0      	cbz	r0, 300032ae <FLASH_CalibrationNew+0x136>
30003280:	4628      	mov	r0, r5
30003282:	7929      	ldrb	r1, [r5, #4]
30003284:	4b11      	ldr	r3, [pc, #68]	; (300032cc <FLASH_CalibrationNew+0x154>)
30003286:	4798      	blx	r3
30003288:	4620      	mov	r0, r4
3000328a:	b007      	add	sp, #28
3000328c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003290:	9b02      	ldr	r3, [sp, #8]
30003292:	2d00      	cmp	r5, #0
30003294:	ea4f 6403 	mov.w	r4, r3, lsl #24
30003298:	9b01      	ldr	r3, [sp, #4]
3000329a:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000329e:	ea44 0405 	orr.w	r4, r4, r5
300032a2:	dddf      	ble.n	30003264 <FLASH_CalibrationNew+0xec>
300032a4:	e7d8      	b.n	30003258 <FLASH_CalibrationNew+0xe0>
300032a6:	4808      	ldr	r0, [pc, #32]	; (300032c8 <FLASH_CalibrationNew+0x150>)
300032a8:	4a0b      	ldr	r2, [pc, #44]	; (300032d8 <FLASH_CalibrationNew+0x160>)
300032aa:	4790      	blx	r2
300032ac:	e77b      	b.n	300031a6 <FLASH_CalibrationNew+0x2e>
300032ae:	4b06      	ldr	r3, [pc, #24]	; (300032c8 <FLASH_CalibrationNew+0x150>)
300032b0:	7919      	ldrb	r1, [r3, #4]
300032b2:	4618      	mov	r0, r3
300032b4:	4b08      	ldr	r3, [pc, #32]	; (300032d8 <FLASH_CalibrationNew+0x160>)
300032b6:	4798      	blx	r3
300032b8:	4620      	mov	r0, r4
300032ba:	b007      	add	sp, #28
300032bc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300032c0:	4c06      	ldr	r4, [pc, #24]	; (300032dc <FLASH_CalibrationNew+0x164>)
300032c2:	e7cf      	b.n	30003264 <FLASH_CalibrationNew+0xec>
300032c4:	30009e65 	.word	0x30009e65
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30009c6d 	.word	0x30009c6d
300032e4:	00054060 	.word	0x00054060

300032e8 <FLASH_Calibration>:
300032e8:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300032ec:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300032f0:	695c      	ldr	r4, [r3, #20]
300032f2:	b087      	sub	sp, #28
300032f4:	4616      	mov	r6, r2
300032f6:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300032fa:	9403      	str	r4, [sp, #12]
300032fc:	f8d3 311c 	ldr.w	r3, [r3, #284]	; 0x11c
30003300:	9101      	str	r1, [sp, #4]
30003302:	9304      	str	r3, [sp, #16]
30003304:	9005      	str	r0, [sp, #20]
30003306:	6c13      	ldr	r3, [r2, #64]	; 0x40
30003308:	7944      	ldrb	r4, [r0, #5]
3000330a:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
3000330e:	6413      	str	r3, [r2, #64]	; 0x40
30003310:	4b3a      	ldr	r3, [pc, #232]	; (300033fc <FLASH_Calibration+0x114>)
30003312:	4798      	blx	r3
30003314:	9901      	ldr	r1, [sp, #4]
30003316:	2800      	cmp	r0, #0
30003318:	d05b      	beq.n	300033d2 <FLASH_Calibration+0xea>
3000331a:	4839      	ldr	r0, [pc, #228]	; (30003400 <FLASH_Calibration+0x118>)
3000331c:	f04f 4888 	mov.w	r8, #1140850688	; 0x44000000
30003320:	4b38      	ldr	r3, [pc, #224]	; (30003404 <FLASH_Calibration+0x11c>)
30003322:	4798      	blx	r3
30003324:	f8c8 4014 	str.w	r4, [r8, #20]
30003328:	eb16 0644 	adds.w	r6, r6, r4, lsl #1
3000332c:	d05b      	beq.n	300033e6 <FLASH_Calibration+0xfe>
3000332e:	f04f 0900 	mov.w	r9, #0
30003332:	f04f 33ff 	mov.w	r3, #4294967295
30003336:	4f34      	ldr	r7, [pc, #208]	; (30003408 <FLASH_Calibration+0x120>)
30003338:	f04f 6400 	mov.w	r4, #134217728	; 0x8000000
3000333c:	f8df a0d4 	ldr.w	sl, [pc, #212]	; 30003414 <FLASH_Calibration+0x12c>
30003340:	46cb      	mov	fp, r9
30003342:	4d32      	ldr	r5, [pc, #200]	; (3000340c <FLASH_Calibration+0x124>)
30003344:	9302      	str	r3, [sp, #8]
30003346:	f8cd 9004 	str.w	r9, [sp, #4]
3000334a:	f8d8 111c 	ldr.w	r1, [r8, #284]	; 0x11c
3000334e:	ea4f 330b 	mov.w	r3, fp, lsl #12
30003352:	f421 4170 	bic.w	r1, r1, #61440	; 0xf000
30003356:	b29b      	uxth	r3, r3
30003358:	430b      	orrs	r3, r1
3000335a:	f8c8 311c 	str.w	r3, [r8, #284]	; 0x11c
3000335e:	696b      	ldr	r3, [r5, #20]
30003360:	03db      	lsls	r3, r3, #15
30003362:	d507      	bpl.n	30003374 <FLASH_Calibration+0x8c>
30003364:	f3bf 8f4f 	dsb	sy
30003368:	f8c5 425c 	str.w	r4, [r5, #604]	; 0x25c
3000336c:	f3bf 8f4f 	dsb	sy
30003370:	f3bf 8f6f 	isb	sy
30003374:	47b8      	blx	r7
30003376:	6823      	ldr	r3, [r4, #0]
30003378:	f8da 1000 	ldr.w	r1, [sl]
3000337c:	4299      	cmp	r1, r3
3000337e:	d109      	bne.n	30003394 <FLASH_Calibration+0xac>
30003380:	9b01      	ldr	r3, [sp, #4]
30003382:	f04f 0901 	mov.w	r9, #1
30003386:	3301      	adds	r3, #1
30003388:	9301      	str	r3, [sp, #4]
3000338a:	9b02      	ldr	r3, [sp, #8]
3000338c:	3301      	adds	r3, #1
3000338e:	bf08      	it	eq
30003390:	f8cd b008 	streq.w	fp, [sp, #8]
30003394:	f10b 0b01 	add.w	fp, fp, #1
30003398:	45b3      	cmp	fp, r6
3000339a:	d1d6      	bne.n	3000334a <FLASH_Calibration+0x62>
3000339c:	9b01      	ldr	r3, [sp, #4]
3000339e:	2b00      	cmp	r3, #0
300033a0:	dd05      	ble.n	300033ae <FLASH_Calibration+0xc6>
300033a2:	9a02      	ldr	r2, [sp, #8]
300033a4:	eb02 0863 	add.w	r8, r2, r3, asr #1
300033a8:	9b05      	ldr	r3, [sp, #20]
300033aa:	f883 801d 	strb.w	r8, [r3, #29]
300033ae:	4b13      	ldr	r3, [pc, #76]	; (300033fc <FLASH_Calibration+0x114>)
300033b0:	4798      	blx	r3
300033b2:	b1e8      	cbz	r0, 300033f0 <FLASH_Calibration+0x108>
300033b4:	4812      	ldr	r0, [pc, #72]	; (30003400 <FLASH_Calibration+0x118>)
300033b6:	4b13      	ldr	r3, [pc, #76]	; (30003404 <FLASH_Calibration+0x11c>)
300033b8:	7901      	ldrb	r1, [r0, #4]
300033ba:	4798      	blx	r3
300033bc:	9a03      	ldr	r2, [sp, #12]
300033be:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300033c2:	4648      	mov	r0, r9
300033c4:	615a      	str	r2, [r3, #20]
300033c6:	9a04      	ldr	r2, [sp, #16]
300033c8:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300033cc:	b007      	add	sp, #28
300033ce:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300033d2:	480b      	ldr	r0, [pc, #44]	; (30003400 <FLASH_Calibration+0x118>)
300033d4:	f04f 4888 	mov.w	r8, #1140850688	; 0x44000000
300033d8:	4b0d      	ldr	r3, [pc, #52]	; (30003410 <FLASH_Calibration+0x128>)
300033da:	4798      	blx	r3
300033dc:	f8c8 4014 	str.w	r4, [r8, #20]
300033e0:	eb16 0644 	adds.w	r6, r6, r4, lsl #1
300033e4:	d1a3      	bne.n	3000332e <FLASH_Calibration+0x46>
300033e6:	4b05      	ldr	r3, [pc, #20]	; (300033fc <FLASH_Calibration+0x114>)
300033e8:	46b1      	mov	r9, r6
300033ea:	4798      	blx	r3
300033ec:	2800      	cmp	r0, #0
300033ee:	d1e1      	bne.n	300033b4 <FLASH_Calibration+0xcc>
300033f0:	4803      	ldr	r0, [pc, #12]	; (30003400 <FLASH_Calibration+0x118>)
300033f2:	4b07      	ldr	r3, [pc, #28]	; (30003410 <FLASH_Calibration+0x128>)
300033f4:	7901      	ldrb	r1, [r0, #4]
300033f6:	4798      	blx	r3
300033f8:	e7e0      	b.n	300033bc <FLASH_Calibration+0xd4>
300033fa:	bf00      	nop
300033fc:	30009e65 	.word	0x30009e65
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30009c6d 	.word	0x30009c6d
3000340c:	e000ed00 	.word	0xe000ed00
30003410:	0000b7e9 	.word	0x0000b7e9
30003414:	00054060 	.word	0x00054060

30003418 <_flash_calibration_highspeed>:
30003418:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000341c:	2500      	movs	r5, #0
3000341e:	b087      	sub	sp, #28
30003420:	4e24      	ldr	r6, [pc, #144]	; (300034b4 <_flash_calibration_highspeed+0x9c>)
30003422:	f04f 0801 	mov.w	r8, #1
30003426:	46ab      	mov	fp, r5
30003428:	462c      	mov	r4, r5
3000342a:	46a9      	mov	r9, r5
3000342c:	460a      	mov	r2, r1
3000342e:	462f      	mov	r7, r5
30003430:	4682      	mov	sl, r0
30003432:	b2e0      	uxtb	r0, r4
30003434:	2301      	movs	r3, #1
30003436:	4651      	mov	r1, sl
30003438:	9205      	str	r2, [sp, #20]
3000343a:	e9cd 0800 	strd	r0, r8, [sp]
3000343e:	4630      	mov	r0, r6
30003440:	f7ff fe9a 	bl	30003178 <FLASH_CalibrationNew>
30003444:	f896 3061 	ldrb.w	r3, [r6, #97]	; 0x61
30003448:	b2c0      	uxtb	r0, r0
3000344a:	9a05      	ldr	r2, [sp, #20]
3000344c:	b10b      	cbz	r3, 30003452 <_flash_calibration_highspeed+0x3a>
3000344e:	2802      	cmp	r0, #2
30003450:	d810      	bhi.n	30003474 <_flash_calibration_highspeed+0x5c>
30003452:	42a8      	cmp	r0, r5
30003454:	d812      	bhi.n	3000347c <_flash_calibration_highspeed+0x64>
30003456:	b105      	cbz	r5, 3000345a <_flash_calibration_highspeed+0x42>
30003458:	b1a0      	cbz	r0, 30003484 <_flash_calibration_highspeed+0x6c>
3000345a:	3401      	adds	r4, #1
3000345c:	f886 9061 	strb.w	r9, [r6, #97]	; 0x61
30003460:	2c05      	cmp	r4, #5
30003462:	d1e6      	bne.n	30003432 <_flash_calibration_highspeed+0x1a>
30003464:	46da      	mov	sl, fp
30003466:	46bb      	mov	fp, r7
30003468:	b975      	cbnz	r5, 30003488 <_flash_calibration_highspeed+0x70>
3000346a:	4628      	mov	r0, r5
3000346c:	7775      	strb	r5, [r6, #29]
3000346e:	b007      	add	sp, #28
30003470:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003474:	42a8      	cmp	r0, r5
30003476:	f043 0301 	orr.w	r3, r3, #1
3000347a:	d9ee      	bls.n	3000345a <_flash_calibration_highspeed+0x42>
3000347c:	461f      	mov	r7, r3
3000347e:	46a3      	mov	fp, r4
30003480:	4605      	mov	r5, r0
30003482:	e7ea      	b.n	3000345a <_flash_calibration_highspeed+0x42>
30003484:	46da      	mov	sl, fp
30003486:	46bb      	mov	fp, r7
30003488:	4b0b      	ldr	r3, [pc, #44]	; (300034b8 <_flash_calibration_highspeed+0xa0>)
3000348a:	2004      	movs	r0, #4
3000348c:	f8cd a000 	str.w	sl, [sp]
30003490:	fa5f fa8a 	uxtb.w	sl, sl
30003494:	4a09      	ldr	r2, [pc, #36]	; (300034bc <_flash_calibration_highspeed+0xa4>)
30003496:	4619      	mov	r1, r3
30003498:	e9cd 5b01 	strd	r5, fp, [sp, #4]
3000349c:	f008 fe50 	bl	3000c140 <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000c7cc 	.word	0x3000c7cc
300034bc:	3000c7d8 	.word	0x3000c7d8

300034c0 <flash_calibration_highspeed>:
300034c0:	b5f0      	push	{r4, r5, r6, r7, lr}
300034c2:	4e1d      	ldr	r6, [pc, #116]	; (30003538 <flash_calibration_highspeed+0x78>)
300034c4:	4604      	mov	r4, r0
300034c6:	2500      	movs	r5, #0
300034c8:	b083      	sub	sp, #12
300034ca:	7937      	ldrb	r7, [r6, #4]
300034cc:	f886 5060 	strb.w	r5, [r6, #96]	; 0x60
300034d0:	f7ff fdbe 	bl	30003050 <FLASH_ClockDiv>
300034d4:	4621      	mov	r1, r4
300034d6:	4638      	mov	r0, r7
300034d8:	f7ff ff9e 	bl	30003418 <_flash_calibration_highspeed>
300034dc:	2801      	cmp	r0, #1
300034de:	d00c      	beq.n	300034fa <flash_calibration_highspeed+0x3a>
300034e0:	2001      	movs	r0, #1
300034e2:	4b16      	ldr	r3, [pc, #88]	; (3000353c <flash_calibration_highspeed+0x7c>)
300034e4:	4798      	blx	r3
300034e6:	4b16      	ldr	r3, [pc, #88]	; (30003540 <flash_calibration_highspeed+0x80>)
300034e8:	4a16      	ldr	r2, [pc, #88]	; (30003544 <flash_calibration_highspeed+0x84>)
300034ea:	2002      	movs	r0, #2
300034ec:	4619      	mov	r1, r3
300034ee:	9400      	str	r4, [sp, #0]
300034f0:	f008 fe26 	bl	3000c140 <rtk_log_write>
300034f4:	4628      	mov	r0, r5
300034f6:	b003      	add	sp, #12
300034f8:	bdf0      	pop	{r4, r5, r6, r7, pc}
300034fa:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300034fe:	f896 0061 	ldrb.w	r0, [r6, #97]	; 0x61
30003502:	6c13      	ldr	r3, [r2, #64]	; 0x40
30003504:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30003508:	6413      	str	r3, [r2, #64]	; 0x40
3000350a:	f7ff fde9 	bl	300030e0 <FLASH_CalibrationPhaseIdx>
3000350e:	4b0e      	ldr	r3, [pc, #56]	; (30003548 <flash_calibration_highspeed+0x88>)
30003510:	4798      	blx	r3
30003512:	b960      	cbnz	r0, 3000352e <flash_calibration_highspeed+0x6e>
30003514:	4638      	mov	r0, r7
30003516:	4b0d      	ldr	r3, [pc, #52]	; (3000354c <flash_calibration_highspeed+0x8c>)
30003518:	4798      	blx	r3
3000351a:	4b09      	ldr	r3, [pc, #36]	; (30003540 <flash_calibration_highspeed+0x80>)
3000351c:	2004      	movs	r0, #4
3000351e:	4a0c      	ldr	r2, [pc, #48]	; (30003550 <flash_calibration_highspeed+0x90>)
30003520:	4619      	mov	r1, r3
30003522:	9400      	str	r4, [sp, #0]
30003524:	f008 fe0c 	bl	3000c140 <rtk_log_write>
30003528:	2001      	movs	r0, #1
3000352a:	b003      	add	sp, #12
3000352c:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000352e:	4638      	mov	r0, r7
30003530:	4b08      	ldr	r3, [pc, #32]	; (30003554 <flash_calibration_highspeed+0x94>)
30003532:	4798      	blx	r3
30003534:	e7f1      	b.n	3000351a <flash_calibration_highspeed+0x5a>
30003536:	bf00      	nop
30003538:	2001c01c 	.word	0x2001c01c
3000353c:	0000b569 	.word	0x0000b569
30003540:	3000c7cc 	.word	0x3000c7cc
30003544:	3000c81c 	.word	0x3000c81c
30003548:	30009e65 	.word	0x30009e65
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000c7fc 	.word	0x3000c7fc
30003554:	0000a53d 	.word	0x0000a53d

30003558 <flash_rx_mode_switch>:
30003558:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000355c:	2300      	movs	r3, #0
3000355e:	b08b      	sub	sp, #44	; 0x2c
30003560:	4d49      	ldr	r5, [pc, #292]	; (30003688 <flash_rx_mode_switch+0x130>)
30003562:	4684      	mov	ip, r0
30003564:	ac05      	add	r4, sp, #20
30003566:	f88d 300b 	strb.w	r3, [sp, #11]
3000356a:	f1bc 0f04 	cmp.w	ip, #4
3000356e:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30003570:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30003572:	682b      	ldr	r3, [r5, #0]
30003574:	6023      	str	r3, [r4, #0]
30003576:	d865      	bhi.n	30003644 <flash_rx_mode_switch+0xec>
30003578:	fa4f f08c 	sxtb.w	r0, ip
3000357c:	4e43      	ldr	r6, [pc, #268]	; (3000368c <flash_rx_mode_switch+0x134>)
3000357e:	f8df 9130 	ldr.w	r9, [pc, #304]	; 300036b0 <flash_rx_mode_switch+0x158>
30003582:	f1c0 0404 	rsb	r4, r0, #4
30003586:	f1c0 0022 	rsb	r0, r0, #34	; 0x22
3000358a:	f8df 8128 	ldr.w	r8, [pc, #296]	; 300036b4 <flash_rx_mode_switch+0x15c>
3000358e:	b2e4      	uxtb	r4, r4
30003590:	fa46 f580 	sxtab	r5, r6, r0
30003594:	4f3e      	ldr	r7, [pc, #248]	; (30003690 <flash_rx_mode_switch+0x138>)
30003596:	f8df b120 	ldr.w	fp, [pc, #288]	; 300036b8 <flash_rx_mode_switch+0x160>
3000359a:	f8df a120 	ldr.w	sl, [pc, #288]	; 300036bc <flash_rx_mode_switch+0x164>
3000359e:	e02d      	b.n	300035fc <flash_rx_mode_switch+0xa4>
300035a0:	2c04      	cmp	r4, #4
300035a2:	d032      	beq.n	3000360a <flash_rx_mode_switch+0xb2>
300035a4:	2c02      	cmp	r4, #2
300035a6:	d030      	beq.n	3000360a <flash_rx_mode_switch+0xb2>
300035a8:	6833      	ldr	r3, [r6, #0]
300035aa:	2b03      	cmp	r3, #3
300035ac:	d033      	beq.n	30003616 <flash_rx_mode_switch+0xbe>
300035ae:	2300      	movs	r3, #0
300035b0:	4620      	mov	r0, r4
300035b2:	7773      	strb	r3, [r6, #29]
300035b4:	47d0      	blx	sl
300035b6:	4b37      	ldr	r3, [pc, #220]	; (30003694 <flash_rx_mode_switch+0x13c>)
300035b8:	695a      	ldr	r2, [r3, #20]
300035ba:	03d2      	lsls	r2, r2, #15
300035bc:	d509      	bpl.n	300035d2 <flash_rx_mode_switch+0x7a>
300035be:	f3bf 8f4f 	dsb	sy
300035c2:	f04f 6200 	mov.w	r2, #134217728	; 0x8000000
300035c6:	f8c3 225c 	str.w	r2, [r3, #604]	; 0x25c
300035ca:	f3bf 8f4f 	dsb	sy
300035ce:	f3bf 8f6f 	isb	sy
300035d2:	47c0      	blx	r8
300035d4:	f04f 6300 	mov.w	r3, #134217728	; 0x8000000
300035d8:	2208      	movs	r2, #8
300035da:	492f      	ldr	r1, [pc, #188]	; (30003698 <flash_rx_mode_switch+0x140>)
300035dc:	6818      	ldr	r0, [r3, #0]
300035de:	9003      	str	r0, [sp, #12]
300035e0:	a803      	add	r0, sp, #12
300035e2:	685b      	ldr	r3, [r3, #4]
300035e4:	9304      	str	r3, [sp, #16]
300035e6:	47b8      	blx	r7
300035e8:	2800      	cmp	r0, #0
300035ea:	d03c      	beq.n	30003666 <flash_rx_mode_switch+0x10e>
300035ec:	1e62      	subs	r2, r4, #1
300035ee:	f1c4 0305 	rsb	r3, r4, #5
300035f2:	3d01      	subs	r5, #1
300035f4:	b2d4      	uxtb	r4, r2
300035f6:	b2db      	uxtb	r3, r3
300035f8:	2cff      	cmp	r4, #255	; 0xff
300035fa:	d022      	beq.n	30003642 <flash_rx_mode_switch+0xea>
300035fc:	47c8      	blx	r9
300035fe:	2800      	cmp	r0, #0
30003600:	d1ce      	bne.n	300035a0 <flash_rx_mode_switch+0x48>
30003602:	7770      	strb	r0, [r6, #29]
30003604:	4620      	mov	r0, r4
30003606:	47d8      	blx	fp
30003608:	e7d5      	b.n	300035b6 <flash_rx_mode_switch+0x5e>
3000360a:	2301      	movs	r3, #1
3000360c:	f886 3063 	strb.w	r3, [r6, #99]	; 0x63
30003610:	6833      	ldr	r3, [r6, #0]
30003612:	2b03      	cmp	r3, #3
30003614:	d1cb      	bne.n	300035ae <flash_rx_mode_switch+0x56>
30003616:	f10d 020b 	add.w	r2, sp, #11
3000361a:	2101      	movs	r1, #1
3000361c:	2085      	movs	r0, #133	; 0x85
3000361e:	4b1f      	ldr	r3, [pc, #124]	; (3000369c <flash_rx_mode_switch+0x144>)
30003620:	4798      	blx	r3
30003622:	f89d 300b 	ldrb.w	r3, [sp, #11]
30003626:	7829      	ldrb	r1, [r5, #0]
30003628:	f10d 020b 	add.w	r2, sp, #11
3000362c:	f003 030f 	and.w	r3, r3, #15
30003630:	2081      	movs	r0, #129	; 0x81
30003632:	ea43 1301 	orr.w	r3, r3, r1, lsl #4
30003636:	2101      	movs	r1, #1
30003638:	f88d 300b 	strb.w	r3, [sp, #11]
3000363c:	4b18      	ldr	r3, [pc, #96]	; (300036a0 <flash_rx_mode_switch+0x148>)
3000363e:	4798      	blx	r3
30003640:	e7b5      	b.n	300035ae <flash_rx_mode_switch+0x56>
30003642:	469c      	mov	ip, r3
30003644:	f1bc 0f05 	cmp.w	ip, #5
30003648:	d003      	beq.n	30003652 <flash_rx_mode_switch+0xfa>
3000364a:	2001      	movs	r0, #1
3000364c:	b00b      	add	sp, #44	; 0x2c
3000364e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003652:	4b14      	ldr	r3, [pc, #80]	; (300036a4 <flash_rx_mode_switch+0x14c>)
30003654:	2002      	movs	r0, #2
30003656:	4a14      	ldr	r2, [pc, #80]	; (300036a8 <flash_rx_mode_switch+0x150>)
30003658:	4619      	mov	r1, r3
3000365a:	f008 fd71 	bl	3000c140 <rtk_log_write>
3000365e:	2000      	movs	r0, #0
30003660:	b00b      	add	sp, #44	; 0x2c
30003662:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003666:	ab0a      	add	r3, sp, #40	; 0x28
30003668:	4a10      	ldr	r2, [pc, #64]	; (300036ac <flash_rx_mode_switch+0x154>)
3000366a:	2004      	movs	r0, #4
3000366c:	eb03 0484 	add.w	r4, r3, r4, lsl #2
30003670:	4b0c      	ldr	r3, [pc, #48]	; (300036a4 <flash_rx_mode_switch+0x14c>)
30003672:	f854 4c14 	ldr.w	r4, [r4, #-20]
30003676:	4619      	mov	r1, r3
30003678:	9400      	str	r4, [sp, #0]
3000367a:	f008 fd61 	bl	3000c140 <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000d920 	.word	0x3000d920
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000c7cc 	.word	0x3000c7cc
300036a8:	3000c854 	.word	0x3000c854
300036ac:	3000c83c 	.word	0x3000c83c
300036b0:	30009e65 	.word	0x30009e65
300036b4:	30009c6d 	.word	0x30009c6d
300036b8:	0000c045 	.word	0x0000c045
300036bc:	0000a53d 	.word	0x0000a53d

300036c0 <flash_highspeed_setup>:
300036c0:	4aa5      	ldr	r2, [pc, #660]	; (30003958 <flash_highspeed_setup+0x298>)
300036c2:	f44f 4300 	mov.w	r3, #32768	; 0x8000
300036c6:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300036ca:	8811      	ldrh	r1, [r2, #0]
300036cc:	b087      	sub	sp, #28
300036ce:	2500      	movs	r5, #0
300036d0:	4219      	tst	r1, r3
300036d2:	f105 0201 	add.w	r2, r5, #1
300036d6:	ea4f 0353 	mov.w	r3, r3, lsr #1
300036da:	d102      	bne.n	300036e2 <flash_highspeed_setup+0x22>
300036dc:	b2d5      	uxtb	r5, r2
300036de:	2d10      	cmp	r5, #16
300036e0:	d1f6      	bne.n	300036d0 <flash_highspeed_setup+0x10>
300036e2:	4a9e      	ldr	r2, [pc, #632]	; (3000395c <flash_highspeed_setup+0x29c>)
300036e4:	2400      	movs	r4, #0
300036e6:	f44f 4300 	mov.w	r3, #32768	; 0x8000
300036ea:	8811      	ldrh	r1, [r2, #0]
300036ec:	4219      	tst	r1, r3
300036ee:	f104 0201 	add.w	r2, r4, #1
300036f2:	ea4f 0353 	mov.w	r3, r3, lsr #1
300036f6:	d102      	bne.n	300036fe <flash_highspeed_setup+0x3e>
300036f8:	b2d4      	uxtb	r4, r2
300036fa:	2c10      	cmp	r4, #16
300036fc:	d1f6      	bne.n	300036ec <flash_highspeed_setup+0x2c>
300036fe:	b672      	cpsid	i
30003700:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30003704:	f8df 92ac 	ldr.w	r9, [pc, #684]	; 300039b4 <flash_highspeed_setup+0x2f4>
30003708:	4e95      	ldr	r6, [pc, #596]	; (30003960 <flash_highspeed_setup+0x2a0>)
3000370a:	6813      	ldr	r3, [r2, #0]
3000370c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30003710:	6013      	str	r3, [r2, #0]
30003712:	47c8      	blx	r9
30003714:	2800      	cmp	r0, #0
30003716:	d045      	beq.n	300037a4 <flash_highspeed_setup+0xe4>
30003718:	2103      	movs	r1, #3
3000371a:	aa05      	add	r2, sp, #20
3000371c:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
30003720:	4f90      	ldr	r7, [pc, #576]	; (30003964 <flash_highspeed_setup+0x2a4>)
30003722:	47b8      	blx	r7
30003724:	f89d 2016 	ldrb.w	r2, [sp, #22]
30003728:	4b8f      	ldr	r3, [pc, #572]	; (30003968 <flash_highspeed_setup+0x2a8>)
3000372a:	2004      	movs	r0, #4
3000372c:	9202      	str	r2, [sp, #8]
3000372e:	f89d 2015 	ldrb.w	r2, [sp, #21]
30003732:	4619      	mov	r1, r3
30003734:	9201      	str	r2, [sp, #4]
30003736:	f89d 2014 	ldrb.w	r2, [sp, #20]
3000373a:	9200      	str	r2, [sp, #0]
3000373c:	4a8b      	ldr	r2, [pc, #556]	; (3000396c <flash_highspeed_setup+0x2ac>)
3000373e:	f008 fcff 	bl	3000c140 <rtk_log_write>
30003742:	f89d 8015 	ldrb.w	r8, [sp, #21]
30003746:	f89d 3016 	ldrb.w	r3, [sp, #22]
3000374a:	ea4f 2808 	mov.w	r8, r8, lsl #8
3000374e:	ea48 4803 	orr.w	r8, r8, r3, lsl #16
30003752:	f89d 3014 	ldrb.w	r3, [sp, #20]
30003756:	ea48 0803 	orr.w	r8, r8, r3
3000375a:	47c8      	blx	r9
3000375c:	4b84      	ldr	r3, [pc, #528]	; (30003970 <flash_highspeed_setup+0x2b0>)
3000375e:	4a85      	ldr	r2, [pc, #532]	; (30003974 <flash_highspeed_setup+0x2b4>)
30003760:	2800      	cmp	r0, #0
30003762:	bf08      	it	eq
30003764:	4613      	moveq	r3, r2
30003766:	e007      	b.n	30003778 <flash_highspeed_setup+0xb8>
30003768:	e9d3 1200 	ldrd	r1, r2, [r3]
3000376c:	4618      	mov	r0, r3
3000376e:	3314      	adds	r3, #20
30003770:	ea08 0202 	and.w	r2, r8, r2
30003774:	428a      	cmp	r2, r1
30003776:	d058      	beq.n	3000382a <flash_highspeed_setup+0x16a>
30003778:	7a1a      	ldrb	r2, [r3, #8]
3000377a:	2aff      	cmp	r2, #255	; 0xff
3000377c:	d1f4      	bne.n	30003768 <flash_highspeed_setup+0xa8>
3000377e:	f8df 8238 	ldr.w	r8, [pc, #568]	; 300039b8 <flash_highspeed_setup+0x2f8>
30003782:	2300      	movs	r3, #0
30003784:	f8c8 3000 	str.w	r3, [r8]
30003788:	4b77      	ldr	r3, [pc, #476]	; (30003968 <flash_highspeed_setup+0x2a8>)
3000378a:	2003      	movs	r0, #3
3000378c:	4a7a      	ldr	r2, [pc, #488]	; (30003978 <flash_highspeed_setup+0x2b8>)
3000378e:	4619      	mov	r1, r3
30003790:	f008 fcd6 	bl	3000c140 <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 ffb5 	bl	3000c708 <__io_assert_failed_veneer>
3000379e:	f8d8 0000 	ldr.w	r0, [r8]
300037a2:	e048      	b.n	30003836 <flash_highspeed_setup+0x176>
300037a4:	ab05      	add	r3, sp, #20
300037a6:	4602      	mov	r2, r0
300037a8:	4601      	mov	r1, r0
300037aa:	4f75      	ldr	r7, [pc, #468]	; (30003980 <flash_highspeed_setup+0x2c0>)
300037ac:	9300      	str	r3, [sp, #0]
300037ae:	2302      	movs	r3, #2
300037b0:	f896 0055 	ldrb.w	r0, [r6, #85]	; 0x55
300037b4:	47b8      	blx	r7
300037b6:	f89d 3015 	ldrb.w	r3, [sp, #21]
300037ba:	4a72      	ldr	r2, [pc, #456]	; (30003984 <flash_highspeed_setup+0x2c4>)
300037bc:	2004      	movs	r0, #4
300037be:	9301      	str	r3, [sp, #4]
300037c0:	f89d 3014 	ldrb.w	r3, [sp, #20]
300037c4:	9300      	str	r3, [sp, #0]
300037c6:	4b68      	ldr	r3, [pc, #416]	; (30003968 <flash_highspeed_setup+0x2a8>)
300037c8:	4619      	mov	r1, r3
300037ca:	f008 fcb9 	bl	3000c140 <rtk_log_write>
300037ce:	f89d 7015 	ldrb.w	r7, [sp, #21]
300037d2:	f89d 3016 	ldrb.w	r3, [sp, #22]
300037d6:	023f      	lsls	r7, r7, #8
300037d8:	ea47 4703 	orr.w	r7, r7, r3, lsl #16
300037dc:	f89d 3014 	ldrb.w	r3, [sp, #20]
300037e0:	431f      	orrs	r7, r3
300037e2:	47c8      	blx	r9
300037e4:	4b62      	ldr	r3, [pc, #392]	; (30003970 <flash_highspeed_setup+0x2b0>)
300037e6:	4a63      	ldr	r2, [pc, #396]	; (30003974 <flash_highspeed_setup+0x2b4>)
300037e8:	2800      	cmp	r0, #0
300037ea:	bf08      	it	eq
300037ec:	4613      	moveq	r3, r2
300037ee:	e006      	b.n	300037fe <flash_highspeed_setup+0x13e>
300037f0:	e9d3 1200 	ldrd	r1, r2, [r3]
300037f4:	4618      	mov	r0, r3
300037f6:	3314      	adds	r3, #20
300037f8:	403a      	ands	r2, r7
300037fa:	428a      	cmp	r2, r1
300037fc:	d073      	beq.n	300038e6 <flash_highspeed_setup+0x226>
300037fe:	7a1a      	ldrb	r2, [r3, #8]
30003800:	2aff      	cmp	r2, #255	; 0xff
30003802:	d1f5      	bne.n	300037f0 <flash_highspeed_setup+0x130>
30003804:	f8df 81b0 	ldr.w	r8, [pc, #432]	; 300039b8 <flash_highspeed_setup+0x2f8>
30003808:	2300      	movs	r3, #0
3000380a:	f8c8 3000 	str.w	r3, [r8]
3000380e:	4b56      	ldr	r3, [pc, #344]	; (30003968 <flash_highspeed_setup+0x2a8>)
30003810:	2003      	movs	r0, #3
30003812:	4a59      	ldr	r2, [pc, #356]	; (30003978 <flash_highspeed_setup+0x2b8>)
30003814:	4619      	mov	r1, r3
30003816:	f008 fc93 	bl	3000c140 <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 ff72 	bl	3000c708 <__io_assert_failed_veneer>
30003824:	f8d8 0000 	ldr.w	r0, [r8]
30003828:	e063      	b.n	300038f2 <flash_highspeed_setup+0x232>
3000382a:	f8df 818c 	ldr.w	r8, [pc, #396]	; 300039b8 <flash_highspeed_setup+0x2f8>
3000382e:	f8c8 0000 	str.w	r0, [r8]
30003832:	2800      	cmp	r0, #0
30003834:	d0a8      	beq.n	30003788 <flash_highspeed_setup+0xc8>
30003836:	7a03      	ldrb	r3, [r0, #8]
30003838:	2b05      	cmp	r3, #5
3000383a:	d80b      	bhi.n	30003854 <flash_highspeed_setup+0x194>
3000383c:	3b01      	subs	r3, #1
3000383e:	4848      	ldr	r0, [pc, #288]	; (30003960 <flash_highspeed_setup+0x2a0>)
30003840:	2b04      	cmp	r3, #4
30003842:	f200 8107 	bhi.w	30003a54 <flash_highspeed_setup+0x394>
30003846:	e8df f013 	tbh	[pc, r3, lsl #1]
3000384a:	00ca      	.short	0x00ca
3000384c:	00c200c7 	.word	0x00c200c7
30003850:	00b900bf 	.word	0x00b900bf
30003854:	2bfe      	cmp	r3, #254	; 0xfe
30003856:	d104      	bne.n	30003862 <flash_highspeed_setup+0x1a2>
30003858:	6903      	ldr	r3, [r0, #16]
3000385a:	2b00      	cmp	r3, #0
3000385c:	f000 80f1 	beq.w	30003a42 <flash_highspeed_setup+0x382>
30003860:	4798      	blx	r3
30003862:	4b4a      	ldr	r3, [pc, #296]	; (3000398c <flash_highspeed_setup+0x2cc>)
30003864:	4798      	blx	r3
30003866:	b110      	cbz	r0, 3000386e <flash_highspeed_setup+0x1ae>
30003868:	2304      	movs	r3, #4
3000386a:	f886 3062 	strb.w	r3, [r6, #98]	; 0x62
3000386e:	f8d8 3000 	ldr.w	r3, [r8]
30003872:	aa05      	add	r2, sp, #20
30003874:	2101      	movs	r1, #1
30003876:	f896 0056 	ldrb.w	r0, [r6, #86]	; 0x56
3000387a:	f8d3 800c 	ldr.w	r8, [r3, #12]
3000387e:	68f3      	ldr	r3, [r6, #12]
30003880:	9304      	str	r3, [sp, #16]
30003882:	47b8      	blx	r7
30003884:	69b3      	ldr	r3, [r6, #24]
30003886:	2b00      	cmp	r3, #0
30003888:	f040 80bf 	bne.w	30003a0a <flash_highspeed_setup+0x34a>
3000388c:	4b40      	ldr	r3, [pc, #256]	; (30003990 <flash_highspeed_setup+0x2d0>)
3000388e:	781b      	ldrb	r3, [r3, #0]
30003890:	2b00      	cmp	r3, #0
30003892:	f040 80c2 	bne.w	30003a1a <flash_highspeed_setup+0x35a>
30003896:	2701      	movs	r7, #1
30003898:	9b05      	ldr	r3, [sp, #20]
3000389a:	a805      	add	r0, sp, #20
3000389c:	463a      	mov	r2, r7
3000389e:	a904      	add	r1, sp, #16
300038a0:	ea03 0308 	and.w	r3, r3, r8
300038a4:	9305      	str	r3, [sp, #20]
300038a6:	4b3b      	ldr	r3, [pc, #236]	; (30003994 <flash_highspeed_setup+0x2d4>)
300038a8:	4798      	blx	r3
300038aa:	b148      	cbz	r0, 300038c0 <flash_highspeed_setup+0x200>
300038ac:	f896 3059 	ldrb.w	r3, [r6, #89]	; 0x59
300038b0:	aa04      	add	r2, sp, #16
300038b2:	f896 0058 	ldrb.w	r0, [r6, #88]	; 0x58
300038b6:	2b00      	cmp	r3, #0
300038b8:	d134      	bne.n	30003924 <flash_highspeed_setup+0x264>
300038ba:	4639      	mov	r1, r7
300038bc:	4b36      	ldr	r3, [pc, #216]	; (30003998 <flash_highspeed_setup+0x2d8>)
300038be:	4798      	blx	r3
300038c0:	4628      	mov	r0, r5
300038c2:	f7ff fe49 	bl	30003558 <flash_rx_mode_switch>
300038c6:	4b35      	ldr	r3, [pc, #212]	; (3000399c <flash_highspeed_setup+0x2dc>)
300038c8:	4798      	blx	r3
300038ca:	2801      	cmp	r0, #1
300038cc:	d007      	beq.n	300038de <flash_highspeed_setup+0x21e>
300038ce:	2c06      	cmp	r4, #6
300038d0:	bf28      	it	cs
300038d2:	2406      	movcs	r4, #6
300038d4:	f1c4 0009 	rsb	r0, r4, #9
300038d8:	b2c0      	uxtb	r0, r0
300038da:	f7ff fdf1 	bl	300034c0 <flash_calibration_highspeed>
300038de:	b662      	cpsie	i
300038e0:	b007      	add	sp, #28
300038e2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300038e6:	f8df 80d0 	ldr.w	r8, [pc, #208]	; 300039b8 <flash_highspeed_setup+0x2f8>
300038ea:	f8c8 0000 	str.w	r0, [r8]
300038ee:	2800      	cmp	r0, #0
300038f0:	d08d      	beq.n	3000380e <flash_highspeed_setup+0x14e>
300038f2:	7a03      	ldrb	r3, [r0, #8]
300038f4:	2b04      	cmp	r3, #4
300038f6:	d806      	bhi.n	30003906 <flash_highspeed_setup+0x246>
300038f8:	2b04      	cmp	r3, #4
300038fa:	d80b      	bhi.n	30003914 <flash_highspeed_setup+0x254>
300038fc:	e8df f003 	tbb	[pc, r3]
30003900:	0a24281c 	.word	0x0a24281c
30003904:	20          	.byte	0x20
30003905:	00          	.byte	0x00
30003906:	2bfe      	cmp	r3, #254	; 0xfe
30003908:	d104      	bne.n	30003914 <flash_highspeed_setup+0x254>
3000390a:	6903      	ldr	r3, [r0, #16]
3000390c:	2b00      	cmp	r3, #0
3000390e:	f000 808f 	beq.w	30003a30 <flash_highspeed_setup+0x370>
30003912:	4798      	blx	r3
30003914:	68f3      	ldr	r3, [r6, #12]
30003916:	2b00      	cmp	r3, #0
30003918:	d16c      	bne.n	300039f4 <flash_highspeed_setup+0x334>
3000391a:	4e21      	ldr	r6, [pc, #132]	; (300039a0 <flash_highspeed_setup+0x2e0>)
3000391c:	2100      	movs	r1, #0
3000391e:	20a0      	movs	r0, #160	; 0xa0
30003920:	47b0      	blx	r6
30003922:	e7cd      	b.n	300038c0 <flash_highspeed_setup+0x200>
30003924:	2101      	movs	r1, #1
30003926:	4f1c      	ldr	r7, [pc, #112]	; (30003998 <flash_highspeed_setup+0x2d8>)
30003928:	47b8      	blx	r7
3000392a:	f10d 0211 	add.w	r2, sp, #17
3000392e:	f896 0059 	ldrb.w	r0, [r6, #89]	; 0x59
30003932:	2101      	movs	r1, #1
30003934:	47b8      	blx	r7
30003936:	e7c3      	b.n	300038c0 <flash_highspeed_setup+0x200>
30003938:	4809      	ldr	r0, [pc, #36]	; (30003960 <flash_highspeed_setup+0x2a0>)
3000393a:	4b1a      	ldr	r3, [pc, #104]	; (300039a4 <flash_highspeed_setup+0x2e4>)
3000393c:	4798      	blx	r3
3000393e:	e7e9      	b.n	30003914 <flash_highspeed_setup+0x254>
30003940:	4807      	ldr	r0, [pc, #28]	; (30003960 <flash_highspeed_setup+0x2a0>)
30003942:	4b19      	ldr	r3, [pc, #100]	; (300039a8 <flash_highspeed_setup+0x2e8>)
30003944:	4798      	blx	r3
30003946:	e7e5      	b.n	30003914 <flash_highspeed_setup+0x254>
30003948:	4805      	ldr	r0, [pc, #20]	; (30003960 <flash_highspeed_setup+0x2a0>)
3000394a:	4b18      	ldr	r3, [pc, #96]	; (300039ac <flash_highspeed_setup+0x2ec>)
3000394c:	4798      	blx	r3
3000394e:	e7e1      	b.n	30003914 <flash_highspeed_setup+0x254>
30003950:	4803      	ldr	r0, [pc, #12]	; (30003960 <flash_highspeed_setup+0x2a0>)
30003952:	4b17      	ldr	r3, [pc, #92]	; (300039b0 <flash_highspeed_setup+0x2f0>)
30003954:	4798      	blx	r3
30003956:	e7dd      	b.n	30003914 <flash_highspeed_setup+0x254>
30003958:	3000d890 	.word	0x3000d890
3000395c:	3000d892 	.word	0x3000d892
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000c7cc 	.word	0x3000c7cc
3000396c:	3000c878 	.word	0x3000c878
30003970:	3000d78c 	.word	0x3000d78c
30003974:	3000d894 	.word	0x3000d894
30003978:	3000c894 	.word	0x3000c894
3000397c:	3000d944 	.word	0x3000d944
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000c8c0 	.word	0x3000c8c0
30003988:	3000d934 	.word	0x3000d934
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000ff61 	.word	0x3000ff61
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30009e65 	.word	0x30009e65
300039b8:	3000ff64 	.word	0x3000ff64
300039bc:	4b27      	ldr	r3, [pc, #156]	; (30003a5c <flash_highspeed_setup+0x39c>)
300039be:	4798      	blx	r3
300039c0:	4b27      	ldr	r3, [pc, #156]	; (30003a60 <flash_highspeed_setup+0x3a0>)
300039c2:	2201      	movs	r2, #1
300039c4:	701a      	strb	r2, [r3, #0]
300039c6:	e74c      	b.n	30003862 <flash_highspeed_setup+0x1a2>
300039c8:	4b26      	ldr	r3, [pc, #152]	; (30003a64 <flash_highspeed_setup+0x3a4>)
300039ca:	4798      	blx	r3
300039cc:	e749      	b.n	30003862 <flash_highspeed_setup+0x1a2>
300039ce:	4b23      	ldr	r3, [pc, #140]	; (30003a5c <flash_highspeed_setup+0x39c>)
300039d0:	4798      	blx	r3
300039d2:	2300      	movs	r3, #0
300039d4:	60f3      	str	r3, [r6, #12]
300039d6:	e744      	b.n	30003862 <flash_highspeed_setup+0x1a2>
300039d8:	4b20      	ldr	r3, [pc, #128]	; (30003a5c <flash_highspeed_setup+0x39c>)
300039da:	4798      	blx	r3
300039dc:	e741      	b.n	30003862 <flash_highspeed_setup+0x1a2>
300039de:	4b22      	ldr	r3, [pc, #136]	; (30003a68 <flash_highspeed_setup+0x3a8>)
300039e0:	4798      	blx	r3
300039e2:	f89d 3016 	ldrb.w	r3, [sp, #22]
300039e6:	2b15      	cmp	r3, #21
300039e8:	f67f af3b 	bls.w	30003862 <flash_highspeed_setup+0x1a2>
300039ec:	2331      	movs	r3, #49	; 0x31
300039ee:	f886 3059 	strb.w	r3, [r6, #89]	; 0x59
300039f2:	e736      	b.n	30003862 <flash_highspeed_setup+0x1a2>
300039f4:	4b1d      	ldr	r3, [pc, #116]	; (30003a6c <flash_highspeed_setup+0x3ac>)
300039f6:	20b0      	movs	r0, #176	; 0xb0
300039f8:	4798      	blx	r3
300039fa:	4601      	mov	r1, r0
300039fc:	4e1c      	ldr	r6, [pc, #112]	; (30003a70 <flash_highspeed_setup+0x3b0>)
300039fe:	20b0      	movs	r0, #176	; 0xb0
30003a00:	f041 0101 	orr.w	r1, r1, #1
30003a04:	b2c9      	uxtb	r1, r1
30003a06:	47b0      	blx	r6
30003a08:	e788      	b.n	3000391c <flash_highspeed_setup+0x25c>
30003a0a:	f10d 0215 	add.w	r2, sp, #21
30003a0e:	2101      	movs	r1, #1
30003a10:	f896 0057 	ldrb.w	r0, [r6, #87]	; 0x57
30003a14:	47b8      	blx	r7
30003a16:	2702      	movs	r7, #2
30003a18:	e73e      	b.n	30003898 <flash_highspeed_setup+0x1d8>
30003a1a:	f10d 0215 	add.w	r2, sp, #21
30003a1e:	2102      	movs	r1, #2
30003a20:	2015      	movs	r0, #21
30003a22:	47b8      	blx	r7
30003a24:	9b04      	ldr	r3, [sp, #16]
30003a26:	2703      	movs	r7, #3
30003a28:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
30003a2c:	9304      	str	r3, [sp, #16]
30003a2e:	e733      	b.n	30003898 <flash_highspeed_setup+0x1d8>
30003a30:	f44f 711c 	mov.w	r1, #624	; 0x270
30003a34:	480f      	ldr	r0, [pc, #60]	; (30003a74 <flash_highspeed_setup+0x3b4>)
30003a36:	f008 fe67 	bl	3000c708 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 fe5e 	bl	3000c708 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000ff61 	.word	0x3000ff61
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000d934 	.word	0x3000d934
30003a78:	3000d944 	.word	0x3000d944
30003a7c:	0000a4c1 	.word	0x0000a4c1

30003a80 <BOOT_LoadSubImage>:
30003a80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003a84:	b08f      	sub	sp, #60	; 0x3c
30003a86:	469a      	mov	sl, r3
30003a88:	460f      	mov	r7, r1
30003a8a:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
30003a8e:	9305      	str	r3, [sp, #20]
30003a90:	b39a      	cbz	r2, 30003afa <BOOT_LoadSubImage+0x7a>
30003a92:	4691      	mov	r9, r2
30003a94:	2600      	movs	r6, #0
30003a96:	4605      	mov	r5, r0
30003a98:	2220      	movs	r2, #32
30003a9a:	4639      	mov	r1, r7
30003a9c:	a806      	add	r0, sp, #24
30003a9e:	f002 f86b 	bl	30005b78 <BOOT_ImgCopy>
30003aa2:	9b06      	ldr	r3, [sp, #24]
30003aa4:	4a49      	ldr	r2, [pc, #292]	; (30003bcc <BOOT_LoadSubImage+0x14c>)
30003aa6:	4293      	cmp	r3, r2
30003aa8:	d157      	bne.n	30003b5a <BOOT_LoadSubImage+0xda>
30003aaa:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
30003aae:	9b07      	ldr	r3, [sp, #28]
30003ab0:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
30003ab4:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
30003ab8:	4293      	cmp	r3, r2
30003aba:	d14e      	bne.n	30003b5a <BOOT_LoadSubImage+0xda>
30003abc:	4b44      	ldr	r3, [pc, #272]	; (30003bd0 <BOOT_LoadSubImage+0x150>)
30003abe:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
30003ac2:	18c3      	adds	r3, r0, r3
30003ac4:	f1a0 0820 	sub.w	r8, r0, #32
30003ac8:	3420      	adds	r4, #32
30003aca:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003ace:	d21a      	bcs.n	30003b06 <BOOT_LoadSubImage+0x86>
30003ad0:	b17d      	cbz	r5, 30003af2 <BOOT_LoadSubImage+0x72>
30003ad2:	1d2b      	adds	r3, r5, #4
30003ad4:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
30003ad8:	4a3e      	ldr	r2, [pc, #248]	; (30003bd4 <BOOT_LoadSubImage+0x154>)
30003ada:	2004      	movs	r0, #4
30003adc:	e9cd 8401 	strd	r8, r4, [sp, #4]
30003ae0:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30003ae4:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30003ae8:	9300      	str	r3, [sp, #0]
30003aea:	4b3b      	ldr	r3, [pc, #236]	; (30003bd8 <BOOT_LoadSubImage+0x158>)
30003aec:	4619      	mov	r1, r3
30003aee:	f008 fb27 	bl	3000c140 <rtk_log_write>
30003af2:	3601      	adds	r6, #1
30003af4:	4427      	add	r7, r4
30003af6:	454e      	cmp	r6, r9
30003af8:	d1ce      	bne.n	30003a98 <BOOT_LoadSubImage+0x18>
30003afa:	4b38      	ldr	r3, [pc, #224]	; (30003bdc <BOOT_LoadSubImage+0x15c>)
30003afc:	2001      	movs	r0, #1
30003afe:	601f      	str	r7, [r3, #0]
30003b00:	b00f      	add	sp, #60	; 0x3c
30003b02:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003b06:	4639      	mov	r1, r7
30003b08:	4622      	mov	r2, r4
30003b0a:	4640      	mov	r0, r8
30003b0c:	f002 f834 	bl	30005b78 <BOOT_ImgCopy>
30003b10:	4933      	ldr	r1, [pc, #204]	; (30003be0 <BOOT_LoadSubImage+0x160>)
30003b12:	694b      	ldr	r3, [r1, #20]
30003b14:	03db      	lsls	r3, r3, #15
30003b16:	d5db      	bpl.n	30003ad0 <BOOT_LoadSubImage+0x50>
30003b18:	ea08 0304 	and.w	r3, r8, r4
30003b1c:	3301      	adds	r3, #1
30003b1e:	d023      	beq.n	30003b68 <BOOT_LoadSubImage+0xe8>
30003b20:	f018 031f 	ands.w	r3, r8, #31
30003b24:	d042      	beq.n	30003bac <BOOT_LoadSubImage+0x12c>
30003b26:	4423      	add	r3, r4
30003b28:	f028 021f 	bic.w	r2, r8, #31
30003b2c:	f3bf 8f4f 	dsb	sy
30003b30:	2b00      	cmp	r3, #0
30003b32:	dd0d      	ble.n	30003b50 <BOOT_LoadSubImage+0xd0>
30003b34:	1e59      	subs	r1, r3, #1
30003b36:	f102 0320 	add.w	r3, r2, #32
30003b3a:	4829      	ldr	r0, [pc, #164]	; (30003be0 <BOOT_LoadSubImage+0x160>)
30003b3c:	f021 011f 	bic.w	r1, r1, #31
30003b40:	4419      	add	r1, r3
30003b42:	428b      	cmp	r3, r1
30003b44:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003b48:	461a      	mov	r2, r3
30003b4a:	f103 0320 	add.w	r3, r3, #32
30003b4e:	d1f8      	bne.n	30003b42 <BOOT_LoadSubImage+0xc2>
30003b50:	f3bf 8f4f 	dsb	sy
30003b54:	f3bf 8f6f 	isb	sy
30003b58:	e7ba      	b.n	30003ad0 <BOOT_LoadSubImage+0x50>
30003b5a:	9b05      	ldr	r3, [sp, #20]
30003b5c:	2b01      	cmp	r3, #1
30003b5e:	d028      	beq.n	30003bb2 <BOOT_LoadSubImage+0x132>
30003b60:	2000      	movs	r0, #0
30003b62:	b00f      	add	sp, #60	; 0x3c
30003b64:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003b68:	2300      	movs	r3, #0
30003b6a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003b6e:	f3bf 8f4f 	dsb	sy
30003b72:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003b76:	f643 7be0 	movw	fp, #16352	; 0x3fe0
30003b7a:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30003b7e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30003b82:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30003b86:	ea0c 000b 	and.w	r0, ip, fp
30003b8a:	4673      	mov	r3, lr
30003b8c:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003b90:	3b01      	subs	r3, #1
30003b92:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003b96:	d2f9      	bcs.n	30003b8c <BOOT_LoadSubImage+0x10c>
30003b98:	f1ac 0c20 	sub.w	ip, ip, #32
30003b9c:	f11c 0f20 	cmn.w	ip, #32
30003ba0:	d1f1      	bne.n	30003b86 <BOOT_LoadSubImage+0x106>
30003ba2:	f3bf 8f4f 	dsb	sy
30003ba6:	f3bf 8f6f 	isb	sy
30003baa:	e791      	b.n	30003ad0 <BOOT_LoadSubImage+0x50>
30003bac:	4623      	mov	r3, r4
30003bae:	4642      	mov	r2, r8
30003bb0:	e7bc      	b.n	30003b2c <BOOT_LoadSubImage+0xac>
30003bb2:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
30003bb6:	2004      	movs	r0, #4
30003bb8:	4b07      	ldr	r3, [pc, #28]	; (30003bd8 <BOOT_LoadSubImage+0x158>)
30003bba:	4a0a      	ldr	r2, [pc, #40]	; (30003be4 <BOOT_LoadSubImage+0x164>)
30003bbc:	4619      	mov	r1, r3
30003bbe:	9400      	str	r4, [sp, #0]
30003bc0:	f008 fabe 	bl	3000c140 <rtk_log_write>
30003bc4:	2000      	movs	r0, #0
30003bc6:	b00f      	add	sp, #60	; 0x3c
30003bc8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003bcc:	35393138 	.word	0x35393138
30003bd0:	f7ffffe0 	.word	0xf7ffffe0
30003bd4:	3000c990 	.word	0x3000c990
30003bd8:	3000c974 	.word	0x3000c974
30003bdc:	3000f128 	.word	0x3000f128
30003be0:	e000ed00 	.word	0xe000ed00
30003be4:	3000c97c 	.word	0x3000c97c

30003be8 <BOOT_LoadAPSubImage>:
30003be8:	4bb5      	ldr	r3, [pc, #724]	; (30003ec0 <BOOT_LoadAPSubImage+0x2d8>)
30003bea:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003bee:	460c      	mov	r4, r1
30003bf0:	4606      	mov	r6, r0
30003bf2:	b08f      	sub	sp, #60	; 0x3c
30003bf4:	4615      	mov	r5, r2
30003bf6:	2220      	movs	r2, #32
30003bf8:	e893 0003 	ldmia.w	r3, {r0, r1}
30003bfc:	0c0b      	lsrs	r3, r1, #16
30003bfe:	9004      	str	r0, [sp, #16]
30003c00:	f8ad 1014 	strh.w	r1, [sp, #20]
30003c04:	a806      	add	r0, sp, #24
30003c06:	4621      	mov	r1, r4
30003c08:	f88d 3016 	strb.w	r3, [sp, #22]
30003c0c:	f001 ffb4 	bl	30005b78 <BOOT_ImgCopy>
30003c10:	4bac      	ldr	r3, [pc, #688]	; (30003ec4 <BOOT_LoadAPSubImage+0x2dc>)
30003c12:	9a06      	ldr	r2, [sp, #24]
30003c14:	429a      	cmp	r2, r3
30003c16:	d108      	bne.n	30003c2a <BOOT_LoadAPSubImage+0x42>
30003c18:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30003c1c:	9a07      	ldr	r2, [sp, #28]
30003c1e:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30003c22:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30003c26:	429a      	cmp	r2, r3
30003c28:	d006      	beq.n	30003c38 <BOOT_LoadAPSubImage+0x50>
30003c2a:	2d01      	cmp	r5, #1
30003c2c:	f000 80b3 	beq.w	30003d96 <BOOT_LoadAPSubImage+0x1ae>
30003c30:	2000      	movs	r0, #0
30003c32:	b00f      	add	sp, #60	; 0x3c
30003c34:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003c38:	9a09      	ldr	r2, [sp, #36]	; 0x24
30003c3a:	4ba3      	ldr	r3, [pc, #652]	; (30003ec8 <BOOT_LoadAPSubImage+0x2e0>)
30003c3c:	6ca7      	ldr	r7, [r4, #72]	; 0x48
30003c3e:	f1a2 0820 	sub.w	r8, r2, #32
30003c42:	4413      	add	r3, r2
30003c44:	37d0      	adds	r7, #208	; 0xd0
30003c46:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003c4a:	d247      	bcs.n	30003cdc <BOOT_LoadAPSubImage+0xf4>
30003c4c:	4b9f      	ldr	r3, [pc, #636]	; (30003ecc <BOOT_LoadAPSubImage+0x2e4>)
30003c4e:	eb07 0508 	add.w	r5, r7, r8
30003c52:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
30003c56:	4798      	blx	r3
30003c58:	2801      	cmp	r0, #1
30003c5a:	4427      	add	r7, r4
30003c5c:	4b9c      	ldr	r3, [pc, #624]	; (30003ed0 <BOOT_LoadAPSubImage+0x2e8>)
30003c5e:	d029      	beq.n	30003cb4 <BOOT_LoadAPSubImage+0xcc>
30003c60:	4619      	mov	r1, r3
30003c62:	4a9c      	ldr	r2, [pc, #624]	; (30003ed4 <BOOT_LoadAPSubImage+0x2ec>)
30003c64:	2004      	movs	r0, #4
30003c66:	f008 fa6b 	bl	3000c140 <rtk_log_write>
30003c6a:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003c6e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003c72:	f080 809c 	bcs.w	30003dae <BOOT_LoadAPSubImage+0x1c6>
30003c76:	4445      	add	r5, r8
30003c78:	eb08 0107 	add.w	r1, r8, r7
30003c7c:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30003c80:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003c84:	d256      	bcs.n	30003d34 <BOOT_LoadAPSubImage+0x14c>
30003c86:	2e00      	cmp	r6, #0
30003c88:	f000 8081 	beq.w	30003d8e <BOOT_LoadAPSubImage+0x1a6>
30003c8c:	4b90      	ldr	r3, [pc, #576]	; (30003ed0 <BOOT_LoadAPSubImage+0x2e8>)
30003c8e:	aa04      	add	r2, sp, #16
30003c90:	2004      	movs	r0, #4
30003c92:	9200      	str	r2, [sp, #0]
30003c94:	4a90      	ldr	r2, [pc, #576]	; (30003ed8 <BOOT_LoadAPSubImage+0x2f0>)
30003c96:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
30003c9a:	3c20      	subs	r4, #32
30003c9c:	3120      	adds	r1, #32
30003c9e:	e9c6 4100 	strd	r4, r1, [r6]
30003ca2:	9102      	str	r1, [sp, #8]
30003ca4:	4619      	mov	r1, r3
30003ca6:	9401      	str	r4, [sp, #4]
30003ca8:	f008 fa4a 	bl	3000c140 <rtk_log_write>
30003cac:	2001      	movs	r0, #1
30003cae:	b00f      	add	sp, #60	; 0x3c
30003cb0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003cb4:	4a89      	ldr	r2, [pc, #548]	; (30003edc <BOOT_LoadAPSubImage+0x2f4>)
30003cb6:	2004      	movs	r0, #4
30003cb8:	4619      	mov	r1, r3
30003cba:	f008 fa41 	bl	3000c140 <rtk_log_write>
30003cbe:	4642      	mov	r2, r8
30003cc0:	4629      	mov	r1, r5
30003cc2:	4638      	mov	r0, r7
30003cc4:	f000 fbe4 	bl	30004490 <BOOT_LoadBL32RDPImg>
30003cc8:	2800      	cmp	r0, #0
30003cca:	d1d4      	bne.n	30003c76 <BOOT_LoadAPSubImage+0x8e>
30003ccc:	4b80      	ldr	r3, [pc, #512]	; (30003ed0 <BOOT_LoadAPSubImage+0x2e8>)
30003cce:	2004      	movs	r0, #4
30003cd0:	4a83      	ldr	r2, [pc, #524]	; (30003ee0 <BOOT_LoadAPSubImage+0x2f8>)
30003cd2:	4619      	mov	r1, r3
30003cd4:	f008 fa34 	bl	3000c140 <rtk_log_write>
30003cd8:	20ff      	movs	r0, #255	; 0xff
30003cda:	e7aa      	b.n	30003c32 <BOOT_LoadAPSubImage+0x4a>
30003cdc:	463a      	mov	r2, r7
30003cde:	4621      	mov	r1, r4
30003ce0:	4640      	mov	r0, r8
30003ce2:	f001 ff49 	bl	30005b78 <BOOT_ImgCopy>
30003ce6:	497f      	ldr	r1, [pc, #508]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003ce8:	694b      	ldr	r3, [r1, #20]
30003cea:	03da      	lsls	r2, r3, #15
30003cec:	d5ae      	bpl.n	30003c4c <BOOT_LoadAPSubImage+0x64>
30003cee:	ea07 0308 	and.w	r3, r7, r8
30003cf2:	3301      	adds	r3, #1
30003cf4:	f000 8086 	beq.w	30003e04 <BOOT_LoadAPSubImage+0x21c>
30003cf8:	f018 031f 	ands.w	r3, r8, #31
30003cfc:	f000 80bb 	beq.w	30003e76 <BOOT_LoadAPSubImage+0x28e>
30003d00:	443b      	add	r3, r7
30003d02:	f028 021f 	bic.w	r2, r8, #31
30003d06:	f3bf 8f4f 	dsb	sy
30003d0a:	2b00      	cmp	r3, #0
30003d0c:	dd0d      	ble.n	30003d2a <BOOT_LoadAPSubImage+0x142>
30003d0e:	1e59      	subs	r1, r3, #1
30003d10:	f102 0320 	add.w	r3, r2, #32
30003d14:	4873      	ldr	r0, [pc, #460]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003d16:	f021 011f 	bic.w	r1, r1, #31
30003d1a:	4419      	add	r1, r3
30003d1c:	428b      	cmp	r3, r1
30003d1e:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003d22:	461a      	mov	r2, r3
30003d24:	f103 0320 	add.w	r3, r3, #32
30003d28:	d1f8      	bne.n	30003d1c <BOOT_LoadAPSubImage+0x134>
30003d2a:	f3bf 8f4f 	dsb	sy
30003d2e:	f3bf 8f6f 	isb	sy
30003d32:	e78b      	b.n	30003c4c <BOOT_LoadAPSubImage+0x64>
30003d34:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30003d38:	4628      	mov	r0, r5
30003d3a:	4622      	mov	r2, r4
30003d3c:	f001 ff1c 	bl	30005b78 <BOOT_ImgCopy>
30003d40:	4968      	ldr	r1, [pc, #416]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003d42:	694b      	ldr	r3, [r1, #20]
30003d44:	03d8      	lsls	r0, r3, #15
30003d46:	d59e      	bpl.n	30003c86 <BOOT_LoadAPSubImage+0x9e>
30003d48:	ea05 0304 	and.w	r3, r5, r4
30003d4c:	3301      	adds	r3, #1
30003d4e:	d075      	beq.n	30003e3c <BOOT_LoadAPSubImage+0x254>
30003d50:	f015 031f 	ands.w	r3, r5, #31
30003d54:	d002      	beq.n	30003d5c <BOOT_LoadAPSubImage+0x174>
30003d56:	f025 051f 	bic.w	r5, r5, #31
30003d5a:	441c      	add	r4, r3
30003d5c:	f3bf 8f4f 	dsb	sy
30003d60:	2c00      	cmp	r4, #0
30003d62:	dd0d      	ble.n	30003d80 <BOOT_LoadAPSubImage+0x198>
30003d64:	1e62      	subs	r2, r4, #1
30003d66:	f105 0320 	add.w	r3, r5, #32
30003d6a:	495e      	ldr	r1, [pc, #376]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003d6c:	f022 021f 	bic.w	r2, r2, #31
30003d70:	441a      	add	r2, r3
30003d72:	4293      	cmp	r3, r2
30003d74:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30003d78:	461d      	mov	r5, r3
30003d7a:	f103 0320 	add.w	r3, r3, #32
30003d7e:	d1f8      	bne.n	30003d72 <BOOT_LoadAPSubImage+0x18a>
30003d80:	f3bf 8f4f 	dsb	sy
30003d84:	f3bf 8f6f 	isb	sy
30003d88:	2e00      	cmp	r6, #0
30003d8a:	f47f af7f 	bne.w	30003c8c <BOOT_LoadAPSubImage+0xa4>
30003d8e:	2001      	movs	r0, #1
30003d90:	b00f      	add	sp, #60	; 0x3c
30003d92:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003d96:	4b4e      	ldr	r3, [pc, #312]	; (30003ed0 <BOOT_LoadAPSubImage+0x2e8>)
30003d98:	ac04      	add	r4, sp, #16
30003d9a:	4a53      	ldr	r2, [pc, #332]	; (30003ee8 <BOOT_LoadAPSubImage+0x300>)
30003d9c:	2004      	movs	r0, #4
30003d9e:	4619      	mov	r1, r3
30003da0:	9400      	str	r4, [sp, #0]
30003da2:	f008 f9cd 	bl	3000c140 <rtk_log_write>
30003da6:	2000      	movs	r0, #0
30003da8:	b00f      	add	sp, #60	; 0x3c
30003daa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003dae:	4639      	mov	r1, r7
30003db0:	4642      	mov	r2, r8
30003db2:	4628      	mov	r0, r5
30003db4:	f001 fee0 	bl	30005b78 <BOOT_ImgCopy>
30003db8:	494a      	ldr	r1, [pc, #296]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003dba:	694b      	ldr	r3, [r1, #20]
30003dbc:	03db      	lsls	r3, r3, #15
30003dbe:	f57f af5a 	bpl.w	30003c76 <BOOT_LoadAPSubImage+0x8e>
30003dc2:	ea08 0305 	and.w	r3, r8, r5
30003dc6:	3301      	adds	r3, #1
30003dc8:	d058      	beq.n	30003e7c <BOOT_LoadAPSubImage+0x294>
30003dca:	f015 031f 	ands.w	r3, r5, #31
30003dce:	d073      	beq.n	30003eb8 <BOOT_LoadAPSubImage+0x2d0>
30003dd0:	4443      	add	r3, r8
30003dd2:	f025 021f 	bic.w	r2, r5, #31
30003dd6:	f3bf 8f4f 	dsb	sy
30003dda:	2b00      	cmp	r3, #0
30003ddc:	dd0d      	ble.n	30003dfa <BOOT_LoadAPSubImage+0x212>
30003dde:	1e59      	subs	r1, r3, #1
30003de0:	f102 0320 	add.w	r3, r2, #32
30003de4:	483f      	ldr	r0, [pc, #252]	; (30003ee4 <BOOT_LoadAPSubImage+0x2fc>)
30003de6:	f021 011f 	bic.w	r1, r1, #31
30003dea:	4419      	add	r1, r3
30003dec:	428b      	cmp	r3, r1
30003dee:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
30003df2:	461a      	mov	r2, r3
30003df4:	f103 0320 	add.w	r3, r3, #32
30003df8:	d1f8      	bne.n	30003dec <BOOT_LoadAPSubImage+0x204>
30003dfa:	f3bf 8f4f 	dsb	sy
30003dfe:	f3bf 8f6f 	isb	sy
30003e02:	e738      	b.n	30003c76 <BOOT_LoadAPSubImage+0x8e>
30003e04:	2300      	movs	r3, #0
30003e06:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003e0a:	f3bf 8f4f 	dsb	sy
30003e0e:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
30003e12:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30003e16:	f3c0 324e 	ubfx	r2, r0, #13, #15
30003e1a:	f3c0 00c9 	ubfx	r0, r0, #3, #10
30003e1e:	0152      	lsls	r2, r2, #5
30003e20:	ea02 0c0e 	and.w	ip, r2, lr
30003e24:	4603      	mov	r3, r0
30003e26:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
30003e2a:	3b01      	subs	r3, #1
30003e2c:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
30003e30:	d2f9      	bcs.n	30003e26 <BOOT_LoadAPSubImage+0x23e>
30003e32:	3a20      	subs	r2, #32
30003e34:	f112 0f20 	cmn.w	r2, #32
30003e38:	d1f2      	bne.n	30003e20 <BOOT_LoadAPSubImage+0x238>
30003e3a:	e776      	b.n	30003d2a <BOOT_LoadAPSubImage+0x142>
30003e3c:	2300      	movs	r3, #0
30003e3e:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003e42:	f3bf 8f4f 	dsb	sy
30003e46:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
30003e4a:	f643 77e0 	movw	r7, #16352	; 0x3fe0
30003e4e:	f3c5 344e 	ubfx	r4, r5, #13, #15
30003e52:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30003e56:	0164      	lsls	r4, r4, #5
30003e58:	ea04 0007 	and.w	r0, r4, r7
30003e5c:	462b      	mov	r3, r5
30003e5e:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003e62:	3b01      	subs	r3, #1
30003e64:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003e68:	1c5a      	adds	r2, r3, #1
30003e6a:	d1f8      	bne.n	30003e5e <BOOT_LoadAPSubImage+0x276>
30003e6c:	3c20      	subs	r4, #32
30003e6e:	f114 0f20 	cmn.w	r4, #32
30003e72:	d1f1      	bne.n	30003e58 <BOOT_LoadAPSubImage+0x270>
30003e74:	e784      	b.n	30003d80 <BOOT_LoadAPSubImage+0x198>
30003e76:	463b      	mov	r3, r7
30003e78:	4642      	mov	r2, r8
30003e7a:	e744      	b.n	30003d06 <BOOT_LoadAPSubImage+0x11e>
30003e7c:	2300      	movs	r3, #0
30003e7e:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30003e82:	f3bf 8f4f 	dsb	sy
30003e86:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30003e8a:	f643 79e0 	movw	r9, #16352	; 0x3fe0
30003e8e:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30003e92:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30003e96:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30003e9a:	ea0c 0009 	and.w	r0, ip, r9
30003e9e:	4673      	mov	r3, lr
30003ea0:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30003ea4:	3b01      	subs	r3, #1
30003ea6:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30003eaa:	d2f9      	bcs.n	30003ea0 <BOOT_LoadAPSubImage+0x2b8>
30003eac:	f1ac 0c20 	sub.w	ip, ip, #32
30003eb0:	f11c 0f20 	cmn.w	ip, #32
30003eb4:	d1f1      	bne.n	30003e9a <BOOT_LoadAPSubImage+0x2b2>
30003eb6:	e7a0      	b.n	30003dfa <BOOT_LoadAPSubImage+0x212>
30003eb8:	4643      	mov	r3, r8
30003eba:	462a      	mov	r2, r5
30003ebc:	e78b      	b.n	30003dd6 <BOOT_LoadAPSubImage+0x1ee>
30003ebe:	bf00      	nop
30003ec0:	3000c96c 	.word	0x3000c96c
30003ec4:	35393138 	.word	0x35393138
30003ec8:	f7ffffe0 	.word	0xf7ffffe0
30003ecc:	0000c109 	.word	0x0000c109
30003ed0:	3000c974 	.word	0x3000c974
30003ed4:	3000c9d4 	.word	0x3000c9d4
30003ed8:	3000c990 	.word	0x3000c990
30003edc:	3000c9a4 	.word	0x3000c9a4
30003ee0:	3000c9b4 	.word	0x3000c9b4
30003ee4:	e000ed00 	.word	0xe000ed00
30003ee8:	3000c97c 	.word	0x3000c97c

30003eec <BOOT_RSIPIvSet>:
30003eec:	b530      	push	{r4, r5, lr}
30003eee:	2300      	movs	r3, #0
30003ef0:	b083      	sub	sp, #12
30003ef2:	4a09      	ldr	r2, [pc, #36]	; (30003f18 <BOOT_RSIPIvSet+0x2c>)
30003ef4:	4605      	mov	r5, r0
30003ef6:	460c      	mov	r4, r1
30003ef8:	9300      	str	r3, [sp, #0]
30003efa:	9301      	str	r3, [sp, #4]
30003efc:	4790      	blx	r2
30003efe:	b148      	cbz	r0, 30003f14 <BOOT_RSIPIvSet+0x28>
30003f00:	f105 0120 	add.w	r1, r5, #32
30003f04:	4b05      	ldr	r3, [pc, #20]	; (30003f1c <BOOT_RSIPIvSet+0x30>)
30003f06:	2208      	movs	r2, #8
30003f08:	4668      	mov	r0, sp
30003f0a:	4798      	blx	r3
30003f0c:	4669      	mov	r1, sp
30003f0e:	b2e0      	uxtb	r0, r4
30003f10:	4b03      	ldr	r3, [pc, #12]	; (30003f20 <BOOT_RSIPIvSet+0x34>)
30003f12:	4798      	blx	r3
30003f14:	b003      	add	sp, #12
30003f16:	bd30      	pop	{r4, r5, pc}
30003f18:	0000c129 	.word	0x0000c129
30003f1c:	00012a1d 	.word	0x00012a1d
30003f20:	30009b55 	.word	0x30009b55

30003f24 <BOOT_OTFCheck>:
30003f24:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30003f28:	4d1c      	ldr	r5, [pc, #112]	; (30003f9c <BOOT_OTFCheck+0x78>)
30003f2a:	b082      	sub	sp, #8
30003f2c:	4604      	mov	r4, r0
30003f2e:	4688      	mov	r8, r1
30003f30:	4617      	mov	r7, r2
30003f32:	461e      	mov	r6, r3
30003f34:	47a8      	blx	r5
30003f36:	b148      	cbz	r0, 30003f4c <BOOT_OTFCheck+0x28>
30003f38:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30003f3c:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003f40:	d204      	bcs.n	30003f4c <BOOT_OTFCheck+0x28>
30003f42:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
30003f46:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30003f4a:	d302      	bcc.n	30003f52 <BOOT_OTFCheck+0x2e>
30003f4c:	b002      	add	sp, #8
30003f4e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30003f52:	4b13      	ldr	r3, [pc, #76]	; (30003fa0 <BOOT_OTFCheck+0x7c>)
30003f54:	2004      	movs	r0, #4
30003f56:	4a13      	ldr	r2, [pc, #76]	; (30003fa4 <BOOT_OTFCheck+0x80>)
30003f58:	4619      	mov	r1, r3
30003f5a:	f008 f8f1 	bl	3000c140 <rtk_log_write>
30003f5e:	4b12      	ldr	r3, [pc, #72]	; (30003fa8 <BOOT_OTFCheck+0x84>)
30003f60:	4798      	blx	r3
30003f62:	4603      	mov	r3, r0
30003f64:	b150      	cbz	r0, 30003f7c <BOOT_OTFCheck+0x58>
30003f66:	2801      	cmp	r0, #1
30003f68:	d009      	beq.n	30003f7e <BOOT_OTFCheck+0x5a>
30003f6a:	4b0d      	ldr	r3, [pc, #52]	; (30003fa0 <BOOT_OTFCheck+0x7c>)
30003f6c:	2002      	movs	r0, #2
30003f6e:	4a0f      	ldr	r2, [pc, #60]	; (30003fac <BOOT_OTFCheck+0x88>)
30003f70:	4619      	mov	r1, r3
30003f72:	b002      	add	sp, #8
30003f74:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30003f78:	f008 b8e2 	b.w	3000c140 <rtk_log_write>
30003f7c:	2302      	movs	r3, #2
30003f7e:	4630      	mov	r0, r6
30003f80:	4621      	mov	r1, r4
30003f82:	9301      	str	r3, [sp, #4]
30003f84:	4642      	mov	r2, r8
30003f86:	2301      	movs	r3, #1
30003f88:	4c09      	ldr	r4, [pc, #36]	; (30003fb0 <BOOT_OTFCheck+0x8c>)
30003f8a:	9700      	str	r7, [sp, #0]
30003f8c:	47a0      	blx	r4
30003f8e:	2001      	movs	r0, #1
30003f90:	4b08      	ldr	r3, [pc, #32]	; (30003fb4 <BOOT_OTFCheck+0x90>)
30003f92:	b002      	add	sp, #8
30003f94:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30003f98:	4718      	bx	r3
30003f9a:	bf00      	nop
30003f9c:	0000c129 	.word	0x0000c129
30003fa0:	3000c974 	.word	0x3000c974
30003fa4:	3000c9e4 	.word	0x3000c9e4
30003fa8:	0000c245 	.word	0x0000c245
30003fac:	3000c9f8 	.word	0x3000c9f8
30003fb0:	30009b89 	.word	0x30009b89
30003fb4:	30009b71 	.word	0x30009b71

30003fb8 <BOOT_OTA_LoadIMG2>:
30003fb8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003fbc:	4b6e      	ldr	r3, [pc, #440]	; (30004178 <BOOT_OTA_LoadIMG2+0x1c0>)
30003fbe:	b08d      	sub	sp, #52	; 0x34
30003fc0:	4a6e      	ldr	r2, [pc, #440]	; (3000417c <BOOT_OTA_LoadIMG2+0x1c4>)
30003fc2:	f103 040c 	add.w	r4, r3, #12
30003fc6:	9003      	str	r0, [sp, #12]
30003fc8:	f8d2 8000 	ldr.w	r8, [r2]
30003fcc:	ae06      	add	r6, sp, #24
30003fce:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30003fd2:	ab09      	add	r3, sp, #36	; 0x24
30003fd4:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30003fd8:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30003fdc:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30003fe0:	4641      	mov	r1, r8
30003fe2:	4867      	ldr	r0, [pc, #412]	; (30004180 <BOOT_OTA_LoadIMG2+0x1c8>)
30003fe4:	f44f 72b2 	mov.w	r2, #356	; 0x164
30003fe8:	f001 fdc6 	bl	30005b78 <BOOT_ImgCopy>
30003fec:	4641      	mov	r1, r8
30003fee:	4864      	ldr	r0, [pc, #400]	; (30004180 <BOOT_OTA_LoadIMG2+0x1c8>)
30003ff0:	f000 fd2a 	bl	30004a48 <BOOT_CertificateCheck>
30003ff4:	2800      	cmp	r0, #0
30003ff6:	f040 80b7 	bne.w	30004168 <BOOT_OTA_LoadIMG2+0x1b0>
30003ffa:	4604      	mov	r4, r0
30003ffc:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004000:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004004:	485f      	ldr	r0, [pc, #380]	; (30004184 <BOOT_OTA_LoadIMG2+0x1cc>)
30004006:	f001 fdb7 	bl	30005b78 <BOOT_ImgCopy>
3000400a:	4b5f      	ldr	r3, [pc, #380]	; (30004188 <BOOT_OTA_LoadIMG2+0x1d0>)
3000400c:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004010:	4798      	blx	r3
30004012:	2800      	cmp	r0, #0
30004014:	f040 809e 	bne.w	30004154 <BOOT_OTA_LoadIMG2+0x19c>
30004018:	f641 75f0 	movw	r5, #8176	; 0x1ff0
3000401c:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 300041cc <BOOT_OTA_LoadIMG2+0x214>
30004020:	4a5a      	ldr	r2, [pc, #360]	; (3000418c <BOOT_OTA_LoadIMG2+0x1d4>)
30004022:	2000      	movs	r0, #0
30004024:	4445      	add	r5, r8
30004026:	4659      	mov	r1, fp
30004028:	4f59      	ldr	r7, [pc, #356]	; (30004190 <BOOT_OTA_LoadIMG2+0x1d8>)
3000402a:	f04f 0801 	mov.w	r8, #1
3000402e:	462b      	mov	r3, r5
30004030:	f8df 919c 	ldr.w	r9, [pc, #412]	; 300041d0 <BOOT_OTA_LoadIMG2+0x218>
30004034:	47b8      	blx	r7
30004036:	2101      	movs	r1, #1
30004038:	2000      	movs	r0, #0
3000403a:	4b56      	ldr	r3, [pc, #344]	; (30004194 <BOOT_OTA_LoadIMG2+0x1dc>)
3000403c:	4798      	blx	r3
3000403e:	2301      	movs	r3, #1
30004040:	4952      	ldr	r1, [pc, #328]	; (3000418c <BOOT_OTA_LoadIMG2+0x1d4>)
30004042:	4658      	mov	r0, fp
30004044:	461a      	mov	r2, r3
30004046:	f7ff ff6d 	bl	30003f24 <BOOT_OTFCheck>
3000404a:	47c8      	blx	r9
3000404c:	2800      	cmp	r0, #0
3000404e:	bf0c      	ite	eq
30004050:	46aa      	moveq	sl, r5
30004052:	46da      	movne	sl, fp
30004054:	4633      	mov	r3, r6
30004056:	2203      	movs	r2, #3
30004058:	4651      	mov	r1, sl
3000405a:	484f      	ldr	r0, [pc, #316]	; (30004198 <BOOT_OTA_LoadIMG2+0x1e0>)
3000405c:	f8cd 8000 	str.w	r8, [sp]
30004060:	f7ff fd0e 	bl	30003a80 <BOOT_LoadSubImage>
30004064:	2800      	cmp	r0, #0
30004066:	d070      	beq.n	3000414a <BOOT_OTA_LoadIMG2+0x192>
30004068:	4e4b      	ldr	r6, [pc, #300]	; (30004198 <BOOT_OTA_LoadIMG2+0x1e0>)
3000406a:	47c8      	blx	r9
3000406c:	2800      	cmp	r0, #0
3000406e:	bf08      	it	eq
30004070:	46ab      	moveq	fp, r5
30004072:	4f47      	ldr	r7, [pc, #284]	; (30004190 <BOOT_OTA_LoadIMG2+0x1d8>)
30004074:	68f2      	ldr	r2, [r6, #12]
30004076:	4640      	mov	r0, r8
30004078:	6873      	ldr	r3, [r6, #4]
3000407a:	f8c6 b000 	str.w	fp, [r6]
3000407e:	4413      	add	r3, r2
30004080:	6972      	ldr	r2, [r6, #20]
30004082:	f8df b150 	ldr.w	fp, [pc, #336]	; 300041d4 <BOOT_OTA_LoadIMG2+0x21c>
30004086:	441a      	add	r2, r3
30004088:	4b44      	ldr	r3, [pc, #272]	; (3000419c <BOOT_OTA_LoadIMG2+0x1e4>)
3000408a:	eb0a 0102 	add.w	r1, sl, r2
3000408e:	61b1      	str	r1, [r6, #24]
30004090:	1e51      	subs	r1, r2, #1
30004092:	400b      	ands	r3, r1
30004094:	4659      	mov	r1, fp
30004096:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
3000409a:	441d      	add	r5, r3
3000409c:	1a9b      	subs	r3, r3, r2
3000409e:	4a40      	ldr	r2, [pc, #256]	; (300041a0 <BOOT_OTA_LoadIMG2+0x1e8>)
300040a0:	61f3      	str	r3, [r6, #28]
300040a2:	462b      	mov	r3, r5
300040a4:	47b8      	blx	r7
300040a6:	4641      	mov	r1, r8
300040a8:	4640      	mov	r0, r8
300040aa:	4b3a      	ldr	r3, [pc, #232]	; (30004194 <BOOT_OTA_LoadIMG2+0x1dc>)
300040ac:	4798      	blx	r3
300040ae:	4642      	mov	r2, r8
300040b0:	2302      	movs	r3, #2
300040b2:	493b      	ldr	r1, [pc, #236]	; (300041a0 <BOOT_OTA_LoadIMG2+0x1e8>)
300040b4:	4658      	mov	r0, fp
300040b6:	f7ff ff35 	bl	30003f24 <BOOT_OTFCheck>
300040ba:	47c8      	blx	r9
300040bc:	2800      	cmp	r0, #0
300040be:	bf0c      	ite	eq
300040c0:	462f      	moveq	r7, r5
300040c2:	465f      	movne	r7, fp
300040c4:	ab09      	add	r3, sp, #36	; 0x24
300040c6:	2203      	movs	r2, #3
300040c8:	f106 0020 	add.w	r0, r6, #32
300040cc:	4639      	mov	r1, r7
300040ce:	f8cd 8000 	str.w	r8, [sp]
300040d2:	f7ff fcd5 	bl	30003a80 <BOOT_LoadSubImage>
300040d6:	2800      	cmp	r0, #0
300040d8:	d037      	beq.n	3000414a <BOOT_OTA_LoadIMG2+0x192>
300040da:	47c8      	blx	r9
300040dc:	6af2      	ldr	r2, [r6, #44]	; 0x2c
300040de:	6a73      	ldr	r3, [r6, #36]	; 0x24
300040e0:	2800      	cmp	r0, #0
300040e2:	bf08      	it	eq
300040e4:	46ab      	moveq	fp, r5
300040e6:	4827      	ldr	r0, [pc, #156]	; (30004184 <BOOT_OTA_LoadIMG2+0x1cc>)
300040e8:	4413      	add	r3, r2
300040ea:	6b72      	ldr	r2, [r6, #52]	; 0x34
300040ec:	f8c6 b020 	str.w	fp, [r6, #32]
300040f0:	4413      	add	r3, r2
300040f2:	4a2a      	ldr	r2, [pc, #168]	; (3000419c <BOOT_OTA_LoadIMG2+0x1e4>)
300040f4:	1e59      	subs	r1, r3, #1
300040f6:	441f      	add	r7, r3
300040f8:	400a      	ands	r2, r1
300040fa:	2100      	movs	r1, #0
300040fc:	63b7      	str	r7, [r6, #56]	; 0x38
300040fe:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004102:	9100      	str	r1, [sp, #0]
30004104:	1ad1      	subs	r1, r2, r3
30004106:	2900      	cmp	r1, #0
30004108:	63f1      	str	r1, [r6, #60]	; 0x3c
3000410a:	4631      	mov	r1, r6
3000410c:	bf18      	it	ne
3000410e:	4613      	movne	r3, r2
30004110:	4a24      	ldr	r2, [pc, #144]	; (300041a4 <BOOT_OTA_LoadIMG2+0x1ec>)
30004112:	441d      	add	r5, r3
30004114:	4b1a      	ldr	r3, [pc, #104]	; (30004180 <BOOT_OTA_LoadIMG2+0x1c8>)
30004116:	6015      	str	r5, [r2, #0]
30004118:	bf0c      	ite	eq
3000411a:	2207      	moveq	r2, #7
3000411c:	2208      	movne	r2, #8
3000411e:	f000 fb89 	bl	30004834 <BOOT_SignatureCheck>
30004122:	4b21      	ldr	r3, [pc, #132]	; (300041a8 <BOOT_OTA_LoadIMG2+0x1f0>)
30004124:	4605      	mov	r5, r0
30004126:	689b      	ldr	r3, [r3, #8]
30004128:	079b      	lsls	r3, r3, #30
3000412a:	d50a      	bpl.n	30004142 <BOOT_OTA_LoadIMG2+0x18a>
3000412c:	4b1f      	ldr	r3, [pc, #124]	; (300041ac <BOOT_OTA_LoadIMG2+0x1f4>)
3000412e:	4a20      	ldr	r2, [pc, #128]	; (300041b0 <BOOT_OTA_LoadIMG2+0x1f8>)
30004130:	9903      	ldr	r1, [sp, #12]
30004132:	2800      	cmp	r0, #0
30004134:	bf18      	it	ne
30004136:	461a      	movne	r2, r3
30004138:	481e      	ldr	r0, [pc, #120]	; (300041b4 <BOOT_OTA_LoadIMG2+0x1fc>)
3000413a:	3101      	adds	r1, #1
3000413c:	462b      	mov	r3, r5
3000413e:	f008 faa3 	bl	3000c688 <__DiagPrintf_veneer>
30004142:	b11d      	cbz	r5, 3000414c <BOOT_OTA_LoadIMG2+0x194>
30004144:	481c      	ldr	r0, [pc, #112]	; (300041b8 <BOOT_OTA_LoadIMG2+0x200>)
30004146:	f008 fa9f 	bl	3000c688 <__DiagPrintf_veneer>
3000414a:	24ff      	movs	r4, #255	; 0xff
3000414c:	4620      	mov	r0, r4
3000414e:	b00d      	add	sp, #52	; 0x34
30004150:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004154:	2208      	movs	r2, #8
30004156:	4919      	ldr	r1, [pc, #100]	; (300041bc <BOOT_OTA_LoadIMG2+0x204>)
30004158:	4b19      	ldr	r3, [pc, #100]	; (300041c0 <BOOT_OTA_LoadIMG2+0x208>)
3000415a:	a804      	add	r0, sp, #16
3000415c:	4798      	blx	r3
3000415e:	a904      	add	r1, sp, #16
30004160:	2001      	movs	r0, #1
30004162:	4b18      	ldr	r3, [pc, #96]	; (300041c4 <BOOT_OTA_LoadIMG2+0x20c>)
30004164:	4798      	blx	r3
30004166:	e757      	b.n	30004018 <BOOT_OTA_LoadIMG2+0x60>
30004168:	24ff      	movs	r4, #255	; 0xff
3000416a:	4817      	ldr	r0, [pc, #92]	; (300041c8 <BOOT_OTA_LoadIMG2+0x210>)
3000416c:	f008 fa8c 	bl	3000c688 <__DiagPrintf_veneer>
30004170:	4620      	mov	r0, r4
30004172:	b00d      	add	sp, #52	; 0x34
30004174:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004178:	3000e690 	.word	0x3000e690
3000417c:	30010364 	.word	0x30010364
30004180:	30010008 	.word	0x30010008
30004184:	3001016c 	.word	0x3001016c
30004188:	0000c129 	.word	0x0000c129
3000418c:	0cffffe0 	.word	0x0cffffe0
30004190:	30009c05 	.word	0x30009c05
30004194:	30009c45 	.word	0x30009c45
30004198:	30010314 	.word	0x30010314
3000419c:	fffff000 	.word	0xfffff000
300041a0:	0dffffe0 	.word	0x0dffffe0
300041a4:	3000f128 	.word	0x3000f128
300041a8:	2001c00c 	.word	0x2001c00c
300041ac:	3000ca18 	.word	0x3000ca18
300041b0:	3000ca10 	.word	0x3000ca10
300041b4:	3000ca48 	.word	0x3000ca48
300041b8:	3000ca7c 	.word	0x3000ca7c
300041bc:	3001018c 	.word	0x3001018c
300041c0:	00012a1d 	.word	0x00012a1d
300041c4:	30009b55 	.word	0x30009b55
300041c8:	3000ca20 	.word	0x3000ca20
300041cc:	0c000000 	.word	0x0c000000
300041d0:	30009e65 	.word	0x30009e65
300041d4:	0d000000 	.word	0x0d000000

300041d8 <BOOT_OTA_Region_Init>:
300041d8:	b510      	push	{r4, lr}
300041da:	4c10      	ldr	r4, [pc, #64]	; (3000421c <BOOT_OTA_Region_Init+0x44>)
300041dc:	2200      	movs	r2, #0
300041de:	2002      	movs	r0, #2
300041e0:	4621      	mov	r1, r4
300041e2:	f004 fa8f 	bl	30008704 <flash_get_layout_info>
300041e6:	2200      	movs	r2, #0
300041e8:	1d21      	adds	r1, r4, #4
300041ea:	2003      	movs	r0, #3
300041ec:	f004 fa8a 	bl	30008704 <flash_get_layout_info>
300041f0:	2200      	movs	r2, #0
300041f2:	f104 0110 	add.w	r1, r4, #16
300041f6:	2004      	movs	r0, #4
300041f8:	f004 fa84 	bl	30008704 <flash_get_layout_info>
300041fc:	2200      	movs	r2, #0
300041fe:	f104 0114 	add.w	r1, r4, #20
30004202:	2005      	movs	r0, #5
30004204:	f004 fa7e 	bl	30008704 <flash_get_layout_info>
30004208:	e9d4 2300 	ldrd	r2, r3, [r4]
3000420c:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004210:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30004214:	e9c4 2302 	strd	r2, r3, [r4, #8]
30004218:	bd10      	pop	{r4, pc}
3000421a:	bf00      	nop
3000421c:	300102fc 	.word	0x300102fc

30004220 <BOOT_OTA_IMG2>:
30004220:	f7ff beca 	b.w	30003fb8 <BOOT_OTA_LoadIMG2>

30004224 <BOOT_OTA_AP>:
30004224:	b5f0      	push	{r4, r5, r6, r7, lr}
30004226:	4a32      	ldr	r2, [pc, #200]	; (300042f0 <BOOT_OTA_AP+0xcc>)
30004228:	b0eb      	sub	sp, #428	; 0x1ac
3000422a:	4b32      	ldr	r3, [pc, #200]	; (300042f4 <BOOT_OTA_AP+0xd0>)
3000422c:	6815      	ldr	r5, [r2, #0]
3000422e:	ac02      	add	r4, sp, #8
30004230:	4e31      	ldr	r6, [pc, #196]	; (300042f8 <BOOT_OTA_AP+0xd4>)
30004232:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30004236:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004238:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
3000423c:	4629      	mov	r1, r5
3000423e:	4d2f      	ldr	r5, [pc, #188]	; (300042fc <BOOT_OTA_AP+0xd8>)
30004240:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004244:	a806      	add	r0, sp, #24
30004246:	f001 fc97 	bl	30005b78 <BOOT_ImgCopy>
3000424a:	4a2d      	ldr	r2, [pc, #180]	; (30004300 <BOOT_OTA_AP+0xdc>)
3000424c:	463b      	mov	r3, r7
3000424e:	4629      	mov	r1, r5
30004250:	2003      	movs	r0, #3
30004252:	47b0      	blx	r6
30004254:	2101      	movs	r1, #1
30004256:	4b2b      	ldr	r3, [pc, #172]	; (30004304 <BOOT_OTA_AP+0xe0>)
30004258:	2003      	movs	r0, #3
3000425a:	4798      	blx	r3
3000425c:	2201      	movs	r2, #1
3000425e:	4928      	ldr	r1, [pc, #160]	; (30004300 <BOOT_OTA_AP+0xdc>)
30004260:	2303      	movs	r3, #3
30004262:	4628      	mov	r0, r5
30004264:	f7ff fe5e 	bl	30003f24 <BOOT_OTFCheck>
30004268:	4b27      	ldr	r3, [pc, #156]	; (30004308 <BOOT_OTA_AP+0xe4>)
3000426a:	4798      	blx	r3
3000426c:	4623      	mov	r3, r4
3000426e:	2201      	movs	r2, #1
30004270:	4c26      	ldr	r4, [pc, #152]	; (3000430c <BOOT_OTA_AP+0xe8>)
30004272:	2800      	cmp	r0, #0
30004274:	bf14      	ite	ne
30004276:	4629      	movne	r1, r5
30004278:	4639      	moveq	r1, r7
3000427a:	9200      	str	r2, [sp, #0]
3000427c:	4620      	mov	r0, r4
3000427e:	2204      	movs	r2, #4
30004280:	f7ff fbfe 	bl	30003a80 <BOOT_LoadSubImage>
30004284:	2801      	cmp	r0, #1
30004286:	d122      	bne.n	300042ce <BOOT_OTA_AP+0xaa>
30004288:	2302      	movs	r3, #2
3000428a:	4605      	mov	r5, r0
3000428c:	2204      	movs	r2, #4
3000428e:	a806      	add	r0, sp, #24
30004290:	9300      	str	r3, [sp, #0]
30004292:	4621      	mov	r1, r4
30004294:	4b1e      	ldr	r3, [pc, #120]	; (30004310 <BOOT_OTA_AP+0xec>)
30004296:	f000 facd 	bl	30004834 <BOOT_SignatureCheck>
3000429a:	4606      	mov	r6, r0
3000429c:	bb18      	cbnz	r0, 300042e6 <BOOT_OTA_AP+0xc2>
3000429e:	68e3      	ldr	r3, [r4, #12]
300042a0:	462a      	mov	r2, r5
300042a2:	6861      	ldr	r1, [r4, #4]
300042a4:	f104 0018 	add.w	r0, r4, #24
300042a8:	4419      	add	r1, r3
300042aa:	6963      	ldr	r3, [r4, #20]
300042ac:	4419      	add	r1, r3
300042ae:	4439      	add	r1, r7
300042b0:	f7ff fc9a 	bl	30003be8 <BOOT_LoadAPSubImage>
300042b4:	2801      	cmp	r0, #1
300042b6:	d102      	bne.n	300042be <BOOT_OTA_AP+0x9a>
300042b8:	4630      	mov	r0, r6
300042ba:	b06b      	add	sp, #428	; 0x1ac
300042bc:	bdf0      	pop	{r4, r5, r6, r7, pc}
300042be:	4b15      	ldr	r3, [pc, #84]	; (30004314 <BOOT_OTA_AP+0xf0>)
300042c0:	681b      	ldr	r3, [r3, #0]
300042c2:	079b      	lsls	r3, r3, #30
300042c4:	d40a      	bmi.n	300042dc <BOOT_OTA_AP+0xb8>
300042c6:	26ff      	movs	r6, #255	; 0xff
300042c8:	4630      	mov	r0, r6
300042ca:	b06b      	add	sp, #428	; 0x1ac
300042cc:	bdf0      	pop	{r4, r5, r6, r7, pc}
300042ce:	26ff      	movs	r6, #255	; 0xff
300042d0:	4811      	ldr	r0, [pc, #68]	; (30004318 <BOOT_OTA_AP+0xf4>)
300042d2:	f008 f9d9 	bl	3000c688 <__DiagPrintf_veneer>
300042d6:	4630      	mov	r0, r6
300042d8:	b06b      	add	sp, #428	; 0x1ac
300042da:	bdf0      	pop	{r4, r5, r6, r7, pc}
300042dc:	480f      	ldr	r0, [pc, #60]	; (3000431c <BOOT_OTA_AP+0xf8>)
300042de:	26ff      	movs	r6, #255	; 0xff
300042e0:	f008 f9d2 	bl	3000c688 <__DiagPrintf_veneer>
300042e4:	e7e8      	b.n	300042b8 <BOOT_OTA_AP+0x94>
300042e6:	480e      	ldr	r0, [pc, #56]	; (30004320 <BOOT_OTA_AP+0xfc>)
300042e8:	26ff      	movs	r6, #255	; 0xff
300042ea:	f008 f9cd 	bl	3000c688 <__DiagPrintf_veneer>
300042ee:	e7e3      	b.n	300042b8 <BOOT_OTA_AP+0x94>
300042f0:	3000f128 	.word	0x3000f128
300042f4:	3000e6a8 	.word	0x3000e6a8
300042f8:	30009c05 	.word	0x30009c05
300042fc:	0e000000 	.word	0x0e000000
30004300:	0fffffe0 	.word	0x0fffffe0
30004304:	30009c45 	.word	0x30009c45
30004308:	30009e65 	.word	0x30009e65
3000430c:	30010314 	.word	0x30010314
30004310:	30010008 	.word	0x30010008
30004314:	2001c00c 	.word	0x2001c00c
30004318:	3000cac4 	.word	0x3000cac4
3000431c:	3000cae8 	.word	0x3000cae8
30004320:	3000caa0 	.word	0x3000caa0

30004324 <BOOT_LoadRDPImg>:
30004324:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004328:	4d4e      	ldr	r5, [pc, #312]	; (30004464 <BOOT_LoadRDPImg+0x140>)
3000432a:	b0d9      	sub	sp, #356	; 0x164
3000432c:	4613      	mov	r3, r2
3000432e:	468b      	mov	fp, r1
30004330:	4604      	mov	r4, r0
30004332:	aa06      	add	r2, sp, #24
30004334:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30004338:	e895 0003 	ldmia.w	r5, {r0, r1}
3000433c:	2500      	movs	r5, #0
3000433e:	e882 0003 	stmia.w	r2, {r0, r1}
30004342:	4619      	mov	r1, r3
30004344:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004348:	4620      	mov	r0, r4
3000434a:	f001 fc15 	bl	30005b78 <BOOT_ImgCopy>
3000434e:	ab06      	add	r3, sp, #24
30004350:	2202      	movs	r2, #2
30004352:	4641      	mov	r1, r8
30004354:	4658      	mov	r0, fp
30004356:	9500      	str	r5, [sp, #0]
30004358:	f7ff fb92 	bl	30003a80 <BOOT_LoadSubImage>
3000435c:	2801      	cmp	r0, #1
3000435e:	d06f      	beq.n	30004440 <BOOT_LoadRDPImg+0x11c>
30004360:	4b41      	ldr	r3, [pc, #260]	; (30004468 <BOOT_LoadRDPImg+0x144>)
30004362:	4628      	mov	r0, r5
30004364:	4798      	blx	r3
30004366:	2101      	movs	r1, #1
30004368:	4b40      	ldr	r3, [pc, #256]	; (3000446c <BOOT_LoadRDPImg+0x148>)
3000436a:	4628      	mov	r0, r5
3000436c:	4798      	blx	r3
3000436e:	4b40      	ldr	r3, [pc, #256]	; (30004470 <BOOT_LoadRDPImg+0x14c>)
30004370:	2120      	movs	r1, #32
30004372:	a810      	add	r0, sp, #64	; 0x40
30004374:	4798      	blx	r3
30004376:	f8df a110 	ldr.w	sl, [pc, #272]	; 30004488 <BOOT_LoadRDPImg+0x164>
3000437a:	f104 0120 	add.w	r1, r4, #32
3000437e:	2210      	movs	r2, #16
30004380:	a80c      	add	r0, sp, #48	; 0x30
30004382:	af08      	add	r7, sp, #32
30004384:	47d0      	blx	sl
30004386:	f8df 9104 	ldr.w	r9, [pc, #260]	; 3000448c <BOOT_LoadRDPImg+0x168>
3000438a:	ab06      	add	r3, sp, #24
3000438c:	9504      	str	r5, [sp, #16]
3000438e:	9305      	str	r3, [sp, #20]
30004390:	4641      	mov	r1, r8
30004392:	a818      	add	r0, sp, #96	; 0x60
30004394:	2210      	movs	r2, #16
30004396:	f001 fbef 	bl	30005b78 <BOOT_ImgCopy>
3000439a:	2310      	movs	r3, #16
3000439c:	aa0c      	add	r2, sp, #48	; 0x30
3000439e:	a818      	add	r0, sp, #96	; 0x60
300043a0:	4619      	mov	r1, r3
300043a2:	9700      	str	r7, [sp, #0]
300043a4:	47c8      	blx	r9
300043a6:	683b      	ldr	r3, [r7, #0]
300043a8:	4a32      	ldr	r2, [pc, #200]	; (30004474 <BOOT_LoadRDPImg+0x150>)
300043aa:	4293      	cmp	r3, r2
300043ac:	d14c      	bne.n	30004448 <BOOT_LoadRDPImg+0x124>
300043ae:	9a05      	ldr	r2, [sp, #20]
300043b0:	687b      	ldr	r3, [r7, #4]
300043b2:	f852 1b04 	ldr.w	r1, [r2], #4
300043b6:	9205      	str	r2, [sp, #20]
300043b8:	4a2f      	ldr	r2, [pc, #188]	; (30004478 <BOOT_LoadRDPImg+0x154>)
300043ba:	4293      	cmp	r3, r2
300043bc:	d148      	bne.n	30004450 <BOOT_LoadRDPImg+0x12c>
300043be:	9100      	str	r1, [sp, #0]
300043c0:	2004      	movs	r0, #4
300043c2:	68fe      	ldr	r6, [r7, #12]
300043c4:	4b2d      	ldr	r3, [pc, #180]	; (3000447c <BOOT_LoadRDPImg+0x158>)
300043c6:	3e20      	subs	r6, #32
300043c8:	4a2d      	ldr	r2, [pc, #180]	; (30004480 <BOOT_LoadRDPImg+0x15c>)
300043ca:	4619      	mov	r1, r3
300043cc:	9601      	str	r6, [sp, #4]
300043ce:	68bd      	ldr	r5, [r7, #8]
300043d0:	3520      	adds	r5, #32
300043d2:	9502      	str	r5, [sp, #8]
300043d4:	f007 feb4 	bl	3000c140 <rtk_log_write>
300043d8:	f1bb 0f00 	cmp.w	fp, #0
300043dc:	d006      	beq.n	300043ec <BOOT_LoadRDPImg+0xc8>
300043de:	9a04      	ldr	r2, [sp, #16]
300043e0:	f10b 0304 	add.w	r3, fp, #4
300043e4:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
300043e8:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
300043ec:	b1dd      	cbz	r5, 30004426 <BOOT_LoadRDPImg+0x102>
300043ee:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300043f2:	462c      	mov	r4, r5
300043f4:	4641      	mov	r1, r8
300043f6:	a818      	add	r0, sp, #96	; 0x60
300043f8:	bf28      	it	cs
300043fa:	f44f 7480 	movcs.w	r4, #256	; 0x100
300043fe:	4622      	mov	r2, r4
30004400:	44a0      	add	r8, r4
30004402:	f001 fbb9 	bl	30005b78 <BOOT_ImgCopy>
30004406:	aa0c      	add	r2, sp, #48	; 0x30
30004408:	4621      	mov	r1, r4
3000440a:	2310      	movs	r3, #16
3000440c:	9600      	str	r6, [sp, #0]
3000440e:	a818      	add	r0, sp, #96	; 0x60
30004410:	47c8      	blx	r9
30004412:	ab18      	add	r3, sp, #96	; 0x60
30004414:	f1a4 0110 	sub.w	r1, r4, #16
30004418:	2210      	movs	r2, #16
3000441a:	a80c      	add	r0, sp, #48	; 0x30
3000441c:	4426      	add	r6, r4
3000441e:	4419      	add	r1, r3
30004420:	47d0      	blx	sl
30004422:	1b2d      	subs	r5, r5, r4
30004424:	d1e3      	bne.n	300043ee <BOOT_LoadRDPImg+0xca>
30004426:	9b04      	ldr	r3, [sp, #16]
30004428:	b13b      	cbz	r3, 3000443a <BOOT_LoadRDPImg+0x116>
3000442a:	2100      	movs	r1, #0
3000442c:	4b0f      	ldr	r3, [pc, #60]	; (3000446c <BOOT_LoadRDPImg+0x148>)
3000442e:	4608      	mov	r0, r1
30004430:	4798      	blx	r3
30004432:	2002      	movs	r0, #2
30004434:	b059      	add	sp, #356	; 0x164
30004436:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000443a:	2301      	movs	r3, #1
3000443c:	9304      	str	r3, [sp, #16]
3000443e:	e7a7      	b.n	30004390 <BOOT_LoadRDPImg+0x6c>
30004440:	2002      	movs	r0, #2
30004442:	b059      	add	sp, #356	; 0x164
30004444:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004448:	9b04      	ldr	r3, [sp, #16]
3000444a:	aa06      	add	r2, sp, #24
3000444c:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30004450:	4b0a      	ldr	r3, [pc, #40]	; (3000447c <BOOT_LoadRDPImg+0x158>)
30004452:	2004      	movs	r0, #4
30004454:	9100      	str	r1, [sp, #0]
30004456:	4a0b      	ldr	r2, [pc, #44]	; (30004484 <BOOT_LoadRDPImg+0x160>)
30004458:	4619      	mov	r1, r3
3000445a:	f007 fe71 	bl	3000c140 <rtk_log_write>
3000445e:	2000      	movs	r0, #0
30004460:	e7e8      	b.n	30004434 <BOOT_LoadRDPImg+0x110>
30004462:	bf00      	nop
30004464:	3000e6b8 	.word	0x3000e6b8
30004468:	00002429 	.word	0x00002429
3000446c:	00002479 	.word	0x00002479
30004470:	00004641 	.word	0x00004641
30004474:	35393138 	.word	0x35393138
30004478:	31313738 	.word	0x31313738
3000447c:	3000c974 	.word	0x3000c974
30004480:	3000c990 	.word	0x3000c990
30004484:	3000c97c 	.word	0x3000c97c
30004488:	00012a1d 	.word	0x00012a1d
3000448c:	000046f5 	.word	0x000046f5

30004490 <BOOT_LoadBL32RDPImg>:
30004490:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004494:	4b35      	ldr	r3, [pc, #212]	; (3000456c <BOOT_LoadBL32RDPImg+0xdc>)
30004496:	b0d1      	sub	sp, #324	; 0x144
30004498:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 30004588 <BOOT_LoadBL32RDPImg+0xf8>
3000449c:	4607      	mov	r7, r0
3000449e:	f10d 0810 	add.w	r8, sp, #16
300044a2:	460e      	mov	r6, r1
300044a4:	4615      	mov	r5, r2
300044a6:	ac02      	add	r4, sp, #8
300044a8:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 3000458c <BOOT_LoadBL32RDPImg+0xfc>
300044ac:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30004590 <BOOT_LoadBL32RDPImg+0x100>
300044b0:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300044b2:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
300044b6:	e89c 0003 	ldmia.w	ip, {r0, r1}
300044ba:	e884 0003 	stmia.w	r4, {r0, r1}
300044be:	2000      	movs	r0, #0
300044c0:	47c8      	blx	r9
300044c2:	2101      	movs	r1, #1
300044c4:	2000      	movs	r0, #0
300044c6:	47d0      	blx	sl
300044c8:	2120      	movs	r1, #32
300044ca:	4b29      	ldr	r3, [pc, #164]	; (30004570 <BOOT_LoadBL32RDPImg+0xe0>)
300044cc:	eb0d 0001 	add.w	r0, sp, r1
300044d0:	4798      	blx	r3
300044d2:	4b28      	ldr	r3, [pc, #160]	; (30004574 <BOOT_LoadBL32RDPImg+0xe4>)
300044d4:	689b      	ldr	r3, [r3, #8]
300044d6:	079b      	lsls	r3, r3, #30
300044d8:	d436      	bmi.n	30004548 <BOOT_LoadBL32RDPImg+0xb8>
300044da:	b1fd      	cbz	r5, 3000451c <BOOT_LoadBL32RDPImg+0x8c>
300044dc:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30004594 <BOOT_LoadBL32RDPImg+0x104>
300044e0:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 30004598 <BOOT_LoadBL32RDPImg+0x108>
300044e4:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
300044e8:	462c      	mov	r4, r5
300044ea:	4639      	mov	r1, r7
300044ec:	a810      	add	r0, sp, #64	; 0x40
300044ee:	bf28      	it	cs
300044f0:	f44f 7480 	movcs.w	r4, #256	; 0x100
300044f4:	4622      	mov	r2, r4
300044f6:	4427      	add	r7, r4
300044f8:	f001 fb3e 	bl	30005b78 <BOOT_ImgCopy>
300044fc:	4642      	mov	r2, r8
300044fe:	4621      	mov	r1, r4
30004500:	2310      	movs	r3, #16
30004502:	9600      	str	r6, [sp, #0]
30004504:	a810      	add	r0, sp, #64	; 0x40
30004506:	47d8      	blx	fp
30004508:	ab10      	add	r3, sp, #64	; 0x40
3000450a:	f1a4 0110 	sub.w	r1, r4, #16
3000450e:	2210      	movs	r2, #16
30004510:	4640      	mov	r0, r8
30004512:	4426      	add	r6, r4
30004514:	4419      	add	r1, r3
30004516:	47c8      	blx	r9
30004518:	1b2d      	subs	r5, r5, r4
3000451a:	d1e3      	bne.n	300044e4 <BOOT_LoadBL32RDPImg+0x54>
3000451c:	f856 2c20 	ldr.w	r2, [r6, #-32]
30004520:	3e20      	subs	r6, #32
30004522:	4b15      	ldr	r3, [pc, #84]	; (30004578 <BOOT_LoadBL32RDPImg+0xe8>)
30004524:	429a      	cmp	r2, r3
30004526:	d108      	bne.n	3000453a <BOOT_LoadBL32RDPImg+0xaa>
30004528:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
3000452c:	6872      	ldr	r2, [r6, #4]
3000452e:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30004532:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30004536:	429a      	cmp	r2, r3
30004538:	d00d      	beq.n	30004556 <BOOT_LoadBL32RDPImg+0xc6>
3000453a:	4810      	ldr	r0, [pc, #64]	; (3000457c <BOOT_LoadBL32RDPImg+0xec>)
3000453c:	f008 f8a4 	bl	3000c688 <__DiagPrintf_veneer>
30004540:	2000      	movs	r0, #0
30004542:	b051      	add	sp, #324	; 0x144
30004544:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004548:	4621      	mov	r1, r4
3000454a:	462b      	mov	r3, r5
3000454c:	4632      	mov	r2, r6
3000454e:	480c      	ldr	r0, [pc, #48]	; (30004580 <BOOT_LoadBL32RDPImg+0xf0>)
30004550:	f008 f89a 	bl	3000c688 <__DiagPrintf_veneer>
30004554:	e7c1      	b.n	300044da <BOOT_LoadBL32RDPImg+0x4a>
30004556:	480b      	ldr	r0, [pc, #44]	; (30004584 <BOOT_LoadBL32RDPImg+0xf4>)
30004558:	f008 f896 	bl	3000c688 <__DiagPrintf_veneer>
3000455c:	2100      	movs	r1, #0
3000455e:	4608      	mov	r0, r1
30004560:	47d0      	blx	sl
30004562:	2001      	movs	r0, #1
30004564:	b051      	add	sp, #324	; 0x144
30004566:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000456a:	bf00      	nop
3000456c:	3000cb90 	.word	0x3000cb90
30004570:	00004641 	.word	0x00004641
30004574:	2001c00c 	.word	0x2001c00c
30004578:	35393138 	.word	0x35393138
3000457c:	3000cb74 	.word	0x3000cb74
30004580:	3000cb4c 	.word	0x3000cb4c
30004584:	3000cb80 	.word	0x3000cb80
30004588:	3000cba4 	.word	0x3000cba4
3000458c:	00002429 	.word	0x00002429
30004590:	00002479 	.word	0x00002479
30004594:	000046f5 	.word	0x000046f5
30004598:	00012a1d 	.word	0x00012a1d

3000459c <_BOOT_Validate_ImgHash>:
3000459c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300045a0:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
300045a4:	4605      	mov	r5, r0
300045a6:	4690      	mov	r8, r2
300045a8:	22e0      	movs	r2, #224	; 0xe0
300045aa:	9103      	str	r1, [sp, #12]
300045ac:	2100      	movs	r1, #0
300045ae:	a818      	add	r0, sp, #96	; 0x60
300045b0:	461c      	mov	r4, r3
300045b2:	f88d 101f 	strb.w	r1, [sp, #31]
300045b6:	f008 f83f 	bl	3000c638 <____wrap_memset_veneer>
300045ba:	4628      	mov	r0, r5
300045bc:	f10d 021f 	add.w	r2, sp, #31
300045c0:	f10d 011e 	add.w	r1, sp, #30
300045c4:	f005 fc84 	bl	30009ed0 <SBOOT_GetMdType>
300045c8:	9001      	str	r0, [sp, #4]
300045ca:	2800      	cmp	r0, #0
300045cc:	d166      	bne.n	3000469c <_BOOT_Validate_ImgHash+0x100>
300045ce:	f89d 301f 	ldrb.w	r3, [sp, #31]
300045d2:	2b00      	cmp	r3, #0
300045d4:	d067      	beq.n	300046a6 <_BOOT_Validate_ImgHash+0x10a>
300045d6:	2101      	movs	r1, #1
300045d8:	4605      	mov	r5, r0
300045da:	4b80      	ldr	r3, [pc, #512]	; (300047dc <_BOOT_Validate_ImgHash+0x240>)
300045dc:	4608      	mov	r0, r1
300045de:	4798      	blx	r3
300045e0:	4629      	mov	r1, r5
300045e2:	ab18      	add	r3, sp, #96	; 0x60
300045e4:	2220      	movs	r2, #32
300045e6:	f89d 001e 	ldrb.w	r0, [sp, #30]
300045ea:	4d7d      	ldr	r5, [pc, #500]	; (300047e0 <_BOOT_Validate_ImgHash+0x244>)
300045ec:	47a8      	blx	r5
300045ee:	2800      	cmp	r0, #0
300045f0:	f040 808b 	bne.w	3000470a <_BOOT_Validate_ImgHash+0x16e>
300045f4:	b32c      	cbz	r4, 30004642 <_BOOT_Validate_ImgHash+0xa6>
300045f6:	aad0      	add	r2, sp, #832	; 0x340
300045f8:	1e63      	subs	r3, r4, #1
300045fa:	ae50      	add	r6, sp, #320	; 0x140
300045fc:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 300047f8 <_BOOT_Validate_ImgHash+0x25c>
30004600:	f012 0218 	ands.w	r2, r2, #24
30004604:	b2db      	uxtb	r3, r3
30004606:	f026 091f 	bic.w	r9, r6, #31
3000460a:	4d76      	ldr	r5, [pc, #472]	; (300047e4 <_BOOT_Validate_ImgHash+0x248>)
3000460c:	9204      	str	r2, [sp, #16]
3000460e:	f108 0208 	add.w	r2, r8, #8
30004612:	bf08      	it	eq
30004614:	46b1      	moveq	r9, r6
30004616:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
3000461a:	9302      	str	r3, [sp, #8]
3000461c:	e9d8 b700 	ldrd	fp, r7, [r8]
30004620:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30004624:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30004628:	f0c0 80a7 	bcc.w	3000477a <_BOOT_Validate_ImgHash+0x1de>
3000462c:	4639      	mov	r1, r7
3000462e:	4658      	mov	r0, fp
30004630:	aa18      	add	r2, sp, #96	; 0x60
30004632:	47d0      	blx	sl
30004634:	2800      	cmp	r0, #0
30004636:	d168      	bne.n	3000470a <_BOOT_Validate_ImgHash+0x16e>
30004638:	f108 0808 	add.w	r8, r8, #8
3000463c:	9b02      	ldr	r3, [sp, #8]
3000463e:	4543      	cmp	r3, r8
30004640:	d1ec      	bne.n	3000461c <_BOOT_Validate_ImgHash+0x80>
30004642:	ac08      	add	r4, sp, #32
30004644:	a918      	add	r1, sp, #96	; 0x60
30004646:	4b68      	ldr	r3, [pc, #416]	; (300047e8 <_BOOT_Validate_ImgHash+0x24c>)
30004648:	4620      	mov	r0, r4
3000464a:	4798      	blx	r3
3000464c:	4601      	mov	r1, r0
3000464e:	2800      	cmp	r0, #0
30004650:	d15b      	bne.n	3000470a <_BOOT_Validate_ImgHash+0x16e>
30004652:	2001      	movs	r0, #1
30004654:	4b61      	ldr	r3, [pc, #388]	; (300047dc <_BOOT_Validate_ImgHash+0x240>)
30004656:	4798      	blx	r3
30004658:	f89d 501e 	ldrb.w	r5, [sp, #30]
3000465c:	4b63      	ldr	r3, [pc, #396]	; (300047ec <_BOOT_Validate_ImgHash+0x250>)
3000465e:	4798      	blx	r3
30004660:	fbb0 f3f5 	udiv	r3, r0, r5
30004664:	fb03 0315 	mls	r3, r3, r5, r0
30004668:	b2db      	uxtb	r3, r3
3000466a:	b1bd      	cbz	r5, 3000469c <_BOOT_Validate_ImgHash+0x100>
3000466c:	2000      	movs	r0, #0
3000466e:	9f03      	ldr	r7, [sp, #12]
30004670:	4606      	mov	r6, r0
30004672:	1c5a      	adds	r2, r3, #1
30004674:	5cf9      	ldrb	r1, [r7, r3]
30004676:	5ce3      	ldrb	r3, [r4, r3]
30004678:	3001      	adds	r0, #1
3000467a:	b2d2      	uxtb	r2, r2
3000467c:	404b      	eors	r3, r1
3000467e:	431e      	orrs	r6, r3
30004680:	fbb2 f3f5 	udiv	r3, r2, r5
30004684:	fb05 2213 	mls	r2, r5, r3, r2
30004688:	b2d3      	uxtb	r3, r2
3000468a:	b2c2      	uxtb	r2, r0
3000468c:	4295      	cmp	r5, r2
3000468e:	d8f0      	bhi.n	30004672 <_BOOT_Validate_ImgHash+0xd6>
30004690:	2e00      	cmp	r6, #0
30004692:	9b01      	ldr	r3, [sp, #4]
30004694:	bf18      	it	ne
30004696:	f06f 0306 	mvnne.w	r3, #6
3000469a:	9301      	str	r3, [sp, #4]
3000469c:	9801      	ldr	r0, [sp, #4]
3000469e:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300046a2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300046a6:	a918      	add	r1, sp, #96	; 0x60
300046a8:	f89d 001e 	ldrb.w	r0, [sp, #30]
300046ac:	4b50      	ldr	r3, [pc, #320]	; (300047f0 <_BOOT_Validate_ImgHash+0x254>)
300046ae:	4798      	blx	r3
300046b0:	bb58      	cbnz	r0, 3000470a <_BOOT_Validate_ImgHash+0x16e>
300046b2:	b31c      	cbz	r4, 300046fc <_BOOT_Validate_ImgHash+0x160>
300046b4:	aad0      	add	r2, sp, #832	; 0x340
300046b6:	1e63      	subs	r3, r4, #1
300046b8:	ae50      	add	r6, sp, #320	; 0x140
300046ba:	f8df a140 	ldr.w	sl, [pc, #320]	; 300047fc <_BOOT_Validate_ImgHash+0x260>
300046be:	f012 0218 	ands.w	r2, r2, #24
300046c2:	b2db      	uxtb	r3, r3
300046c4:	f026 091f 	bic.w	r9, r6, #31
300046c8:	4d46      	ldr	r5, [pc, #280]	; (300047e4 <_BOOT_Validate_ImgHash+0x248>)
300046ca:	9204      	str	r2, [sp, #16]
300046cc:	f108 0208 	add.w	r2, r8, #8
300046d0:	bf08      	it	eq
300046d2:	46b1      	moveq	r9, r6
300046d4:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300046d8:	9302      	str	r3, [sp, #8]
300046da:	e9d8 b700 	ldrd	fp, r7, [r8]
300046de:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300046e2:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300046e6:	d318      	bcc.n	3000471a <_BOOT_Validate_ImgHash+0x17e>
300046e8:	4639      	mov	r1, r7
300046ea:	4658      	mov	r0, fp
300046ec:	aa18      	add	r2, sp, #96	; 0x60
300046ee:	47d0      	blx	sl
300046f0:	b958      	cbnz	r0, 3000470a <_BOOT_Validate_ImgHash+0x16e>
300046f2:	f108 0808 	add.w	r8, r8, #8
300046f6:	9b02      	ldr	r3, [sp, #8]
300046f8:	4543      	cmp	r3, r8
300046fa:	d1ee      	bne.n	300046da <_BOOT_Validate_ImgHash+0x13e>
300046fc:	ac08      	add	r4, sp, #32
300046fe:	a918      	add	r1, sp, #96	; 0x60
30004700:	4b3c      	ldr	r3, [pc, #240]	; (300047f4 <_BOOT_Validate_ImgHash+0x258>)
30004702:	4620      	mov	r0, r4
30004704:	4798      	blx	r3
30004706:	2800      	cmp	r0, #0
30004708:	d0a6      	beq.n	30004658 <_BOOT_Validate_ImgHash+0xbc>
3000470a:	f06f 030a 	mvn.w	r3, #10
3000470e:	9301      	str	r3, [sp, #4]
30004710:	9801      	ldr	r0, [sp, #4]
30004712:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30004716:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000471a:	2f00      	cmp	r7, #0
3000471c:	d0e9      	beq.n	300046f2 <_BOOT_Validate_ImgHash+0x156>
3000471e:	9c04      	ldr	r4, [sp, #16]
30004720:	f8cd 8014 	str.w	r8, [sp, #20]
30004724:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30004728:	46b8      	mov	r8, r7
3000472a:	4659      	mov	r1, fp
3000472c:	4630      	mov	r0, r6
3000472e:	bf28      	it	cs
30004730:	f44f 7800 	movcs.w	r8, #512	; 0x200
30004734:	4642      	mov	r2, r8
30004736:	f001 fa1f 	bl	30005b78 <BOOT_ImgCopy>
3000473a:	696a      	ldr	r2, [r5, #20]
3000473c:	03d3      	lsls	r3, r2, #15
3000473e:	d50f      	bpl.n	30004760 <_BOOT_Validate_ImgHash+0x1c4>
30004740:	eb08 0004 	add.w	r0, r8, r4
30004744:	f3bf 8f4f 	dsb	sy
30004748:	4448      	add	r0, r9
3000474a:	464a      	mov	r2, r9
3000474c:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30004750:	3220      	adds	r2, #32
30004752:	1a83      	subs	r3, r0, r2
30004754:	2b00      	cmp	r3, #0
30004756:	dcf9      	bgt.n	3000474c <_BOOT_Validate_ImgHash+0x1b0>
30004758:	f3bf 8f4f 	dsb	sy
3000475c:	f3bf 8f6f 	isb	sy
30004760:	aa18      	add	r2, sp, #96	; 0x60
30004762:	4641      	mov	r1, r8
30004764:	4630      	mov	r0, r6
30004766:	47d0      	blx	sl
30004768:	2800      	cmp	r0, #0
3000476a:	d1ce      	bne.n	3000470a <_BOOT_Validate_ImgHash+0x16e>
3000476c:	ebb7 0708 	subs.w	r7, r7, r8
30004770:	44c3      	add	fp, r8
30004772:	d1d7      	bne.n	30004724 <_BOOT_Validate_ImgHash+0x188>
30004774:	f8dd 8014 	ldr.w	r8, [sp, #20]
30004778:	e7bb      	b.n	300046f2 <_BOOT_Validate_ImgHash+0x156>
3000477a:	2f00      	cmp	r7, #0
3000477c:	f43f af5c 	beq.w	30004638 <_BOOT_Validate_ImgHash+0x9c>
30004780:	9c04      	ldr	r4, [sp, #16]
30004782:	f8cd 8014 	str.w	r8, [sp, #20]
30004786:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
3000478a:	46b8      	mov	r8, r7
3000478c:	4659      	mov	r1, fp
3000478e:	4630      	mov	r0, r6
30004790:	bf28      	it	cs
30004792:	f44f 7800 	movcs.w	r8, #512	; 0x200
30004796:	4642      	mov	r2, r8
30004798:	f001 f9ee 	bl	30005b78 <BOOT_ImgCopy>
3000479c:	696a      	ldr	r2, [r5, #20]
3000479e:	03d2      	lsls	r2, r2, #15
300047a0:	d50f      	bpl.n	300047c2 <_BOOT_Validate_ImgHash+0x226>
300047a2:	eb08 0004 	add.w	r0, r8, r4
300047a6:	f3bf 8f4f 	dsb	sy
300047aa:	4448      	add	r0, r9
300047ac:	464a      	mov	r2, r9
300047ae:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
300047b2:	3220      	adds	r2, #32
300047b4:	1a83      	subs	r3, r0, r2
300047b6:	2b00      	cmp	r3, #0
300047b8:	dcf9      	bgt.n	300047ae <_BOOT_Validate_ImgHash+0x212>
300047ba:	f3bf 8f4f 	dsb	sy
300047be:	f3bf 8f6f 	isb	sy
300047c2:	aa18      	add	r2, sp, #96	; 0x60
300047c4:	4641      	mov	r1, r8
300047c6:	4630      	mov	r0, r6
300047c8:	47d0      	blx	sl
300047ca:	2800      	cmp	r0, #0
300047cc:	d19d      	bne.n	3000470a <_BOOT_Validate_ImgHash+0x16e>
300047ce:	ebb7 0708 	subs.w	r7, r7, r8
300047d2:	44c3      	add	fp, r8
300047d4:	d1d7      	bne.n	30004786 <_BOOT_Validate_ImgHash+0x1ea>
300047d6:	f8dd 8014 	ldr.w	r8, [sp, #20]
300047da:	e72d      	b.n	30004638 <_BOOT_Validate_ImgHash+0x9c>
300047dc:	30009f0d 	.word	0x30009f0d
300047e0:	300097cd 	.word	0x300097cd
300047e4:	e000ed00 	.word	0xe000ed00
300047e8:	30009f89 	.word	0x30009f89
300047ec:	0000ea4d 	.word	0x0000ea4d
300047f0:	30009fc9 	.word	0x30009fc9
300047f4:	3000a045 	.word	0x3000a045
300047f8:	30009f4d 	.word	0x30009f4d
300047fc:	3000a009 	.word	0x3000a009

30004800 <BOOT_Validate_ImgHash>:
30004800:	b530      	push	{r4, r5, lr}
30004802:	b085      	sub	sp, #20
30004804:	4d0a      	ldr	r5, [pc, #40]	; (30004830 <BOOT_Validate_ImgHash+0x30>)
30004806:	4604      	mov	r4, r0
30004808:	9301      	str	r3, [sp, #4]
3000480a:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000480e:	47a8      	blx	r5
30004810:	9903      	ldr	r1, [sp, #12]
30004812:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30004816:	b128      	cbz	r0, 30004824 <BOOT_Validate_ImgHash+0x24>
30004818:	4620      	mov	r0, r4
3000481a:	b005      	add	sp, #20
3000481c:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30004820:	f005 bc74 	b.w	3000a10c <SBOOT_Validate_ImgHash>
30004824:	4620      	mov	r0, r4
30004826:	b005      	add	sp, #20
30004828:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
3000482c:	f7ff beb6 	b.w	3000459c <_BOOT_Validate_ImgHash>
30004830:	30009e65 	.word	0x30009e65

30004834 <BOOT_SignatureCheck>:
30004834:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30004838:	461e      	mov	r6, r3
3000483a:	b089      	sub	sp, #36	; 0x24
3000483c:	4b79      	ldr	r3, [pc, #484]	; (30004a24 <BOOT_SignatureCheck+0x1f0>)
3000483e:	4680      	mov	r8, r0
30004840:	4689      	mov	r9, r1
30004842:	4617      	mov	r7, r2
30004844:	f10d 0c20 	add.w	ip, sp, #32
30004848:	4c77      	ldr	r4, [pc, #476]	; (30004a28 <BOOT_SignatureCheck+0x1f4>)
3000484a:	9d10      	ldr	r5, [sp, #64]	; 0x40
3000484c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
3000484e:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30004852:	47a0      	blx	r4
30004854:	b910      	cbnz	r0, 3000485c <BOOT_SignatureCheck+0x28>
30004856:	b009      	add	sp, #36	; 0x24
30004858:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000485c:	4b73      	ldr	r3, [pc, #460]	; (30004a2c <BOOT_SignatureCheck+0x1f8>)
3000485e:	2000      	movs	r0, #0
30004860:	4798      	blx	r3
30004862:	f898 3013 	ldrb.w	r3, [r8, #19]
30004866:	f898 2012 	ldrb.w	r2, [r8, #18]
3000486a:	f10d 010f 	add.w	r1, sp, #15
3000486e:	f10d 000e 	add.w	r0, sp, #14
30004872:	f005 fc85 	bl	3000a180 <SBOOT_Validate_Algorithm>
30004876:	4604      	mov	r4, r0
30004878:	b9e8      	cbnz	r0, 300048b6 <BOOT_SignatureCheck+0x82>
3000487a:	69f1      	ldr	r1, [r6, #28]
3000487c:	b919      	cbnz	r1, 30004886 <BOOT_SignatureCheck+0x52>
3000487e:	e0cf      	b.n	30004a20 <BOOT_SignatureCheck+0x1ec>
30004880:	b2d0      	uxtb	r0, r2
30004882:	4288      	cmp	r0, r1
30004884:	d208      	bcs.n	30004898 <BOOT_SignatureCheck+0x64>
30004886:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
3000488a:	1c42      	adds	r2, r0, #1
3000488c:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30004890:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30004894:	42ab      	cmp	r3, r5
30004896:	d1f3      	bne.n	30004880 <BOOT_SignatureCheck+0x4c>
30004898:	2224      	movs	r2, #36	; 0x24
3000489a:	23b4      	movs	r3, #180	; 0xb4
3000489c:	fb12 3200 	smlabb	r2, r2, r0, r3
300048a0:	f108 0570 	add.w	r5, r8, #112	; 0x70
300048a4:	4432      	add	r2, r6
300048a6:	f89d 000e 	ldrb.w	r0, [sp, #14]
300048aa:	4629      	mov	r1, r5
300048ac:	f005 fbea 	bl	3000a084 <SBOOT_Validate_PubKey>
300048b0:	4604      	mov	r4, r0
300048b2:	2800      	cmp	r0, #0
300048b4:	d07a      	beq.n	300049ac <BOOT_SignatureCheck+0x178>
300048b6:	f898 3011 	ldrb.w	r3, [r8, #17]
300048ba:	aa08      	add	r2, sp, #32
300048bc:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300048c0:	f853 1c10 	ldr.w	r1, [r3, #-16]
300048c4:	4b5a      	ldr	r3, [pc, #360]	; (30004a30 <BOOT_SignatureCheck+0x1fc>)
300048c6:	2002      	movs	r0, #2
300048c8:	4a5a      	ldr	r2, [pc, #360]	; (30004a34 <BOOT_SignatureCheck+0x200>)
300048ca:	e9cd 1400 	strd	r1, r4, [sp]
300048ce:	4619      	mov	r1, r3
300048d0:	f007 fc36 	bl	3000c140 <rtk_log_write>
300048d4:	2f00      	cmp	r7, #0
300048d6:	d045      	beq.n	30004964 <BOOT_SignatureCheck+0x130>
300048d8:	464d      	mov	r5, r9
300048da:	f107 39ff 	add.w	r9, r7, #4294967295
300048de:	f04f 0800 	mov.w	r8, #0
300048e2:	4f55      	ldr	r7, [pc, #340]	; (30004a38 <BOOT_SignatureCheck+0x204>)
300048e4:	fa5f f389 	uxtb.w	r3, r9
300048e8:	f105 0908 	add.w	r9, r5, #8
300048ec:	1d2e      	adds	r6, r5, #4
300048ee:	4c53      	ldr	r4, [pc, #332]	; (30004a3c <BOOT_SignatureCheck+0x208>)
300048f0:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
300048f4:	e003      	b.n	300048fe <BOOT_SignatureCheck+0xca>
300048f6:	3508      	adds	r5, #8
300048f8:	3608      	adds	r6, #8
300048fa:	45a9      	cmp	r9, r5
300048fc:	d032      	beq.n	30004964 <BOOT_SignatureCheck+0x130>
300048fe:	f856 0c04 	ldr.w	r0, [r6, #-4]
30004902:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30004906:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000490a:	d3f4      	bcc.n	300048f6 <BOOT_SignatureCheck+0xc2>
3000490c:	2100      	movs	r1, #0
3000490e:	686a      	ldr	r2, [r5, #4]
30004910:	47b8      	blx	r7
30004912:	6963      	ldr	r3, [r4, #20]
30004914:	03d9      	lsls	r1, r3, #15
30004916:	d5ee      	bpl.n	300048f6 <BOOT_SignatureCheck+0xc2>
30004918:	f856 2c04 	ldr.w	r2, [r6, #-4]
3000491c:	686b      	ldr	r3, [r5, #4]
3000491e:	ea02 0103 	and.w	r1, r2, r3
30004922:	3101      	adds	r1, #1
30004924:	d022      	beq.n	3000496c <BOOT_SignatureCheck+0x138>
30004926:	f012 011f 	ands.w	r1, r2, #31
3000492a:	d002      	beq.n	30004932 <BOOT_SignatureCheck+0xfe>
3000492c:	440b      	add	r3, r1
3000492e:	f022 021f 	bic.w	r2, r2, #31
30004932:	f3bf 8f4f 	dsb	sy
30004936:	2b00      	cmp	r3, #0
30004938:	dd0c      	ble.n	30004954 <BOOT_SignatureCheck+0x120>
3000493a:	1e59      	subs	r1, r3, #1
3000493c:	f102 0320 	add.w	r3, r2, #32
30004940:	f021 011f 	bic.w	r1, r1, #31
30004944:	4419      	add	r1, r3
30004946:	428b      	cmp	r3, r1
30004948:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
3000494c:	461a      	mov	r2, r3
3000494e:	f103 0320 	add.w	r3, r3, #32
30004952:	d1f8      	bne.n	30004946 <BOOT_SignatureCheck+0x112>
30004954:	f3bf 8f4f 	dsb	sy
30004958:	f3bf 8f6f 	isb	sy
3000495c:	3508      	adds	r5, #8
3000495e:	3608      	adds	r6, #8
30004960:	45a9      	cmp	r9, r5
30004962:	d1cc      	bne.n	300048fe <BOOT_SignatureCheck+0xca>
30004964:	20ff      	movs	r0, #255	; 0xff
30004966:	b009      	add	sp, #36	; 0x24
30004968:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000496c:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30004970:	f3bf 8f4f 	dsb	sy
30004974:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30004978:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
3000497c:	f3c3 304e 	ubfx	r0, r3, #13, #15
30004980:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30004984:	0140      	lsls	r0, r0, #5
30004986:	ea00 010e 	and.w	r1, r0, lr
3000498a:	4663      	mov	r3, ip
3000498c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004990:	3b01      	subs	r3, #1
30004992:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30004996:	1c5a      	adds	r2, r3, #1
30004998:	d1f8      	bne.n	3000498c <BOOT_SignatureCheck+0x158>
3000499a:	3820      	subs	r0, #32
3000499c:	f110 0f20 	cmn.w	r0, #32
300049a0:	d1f1      	bne.n	30004986 <BOOT_SignatureCheck+0x152>
300049a2:	f3bf 8f4f 	dsb	sy
300049a6:	f3bf 8f6f 	isb	sy
300049aa:	e7d7      	b.n	3000495c <BOOT_SignatureCheck+0x128>
300049ac:	f508 7380 	add.w	r3, r8, #256	; 0x100
300049b0:	462a      	mov	r2, r5
300049b2:	f89d 100f 	ldrb.w	r1, [sp, #15]
300049b6:	9301      	str	r3, [sp, #4]
300049b8:	f44f 7380 	mov.w	r3, #256	; 0x100
300049bc:	f89d 000e 	ldrb.w	r0, [sp, #14]
300049c0:	9300      	str	r3, [sp, #0]
300049c2:	4643      	mov	r3, r8
300049c4:	f005 fb7c 	bl	3000a0c0 <SBOOT_Validate_Signature>
300049c8:	4604      	mov	r4, r0
300049ca:	2800      	cmp	r0, #0
300049cc:	f47f af73 	bne.w	300048b6 <BOOT_SignatureCheck+0x82>
300049d0:	f108 0530 	add.w	r5, r8, #48	; 0x30
300049d4:	4b1a      	ldr	r3, [pc, #104]	; (30004a40 <BOOT_SignatureCheck+0x20c>)
300049d6:	f89d 400f 	ldrb.w	r4, [sp, #15]
300049da:	4798      	blx	r3
300049dc:	4629      	mov	r1, r5
300049de:	b9c0      	cbnz	r0, 30004a12 <BOOT_SignatureCheck+0x1de>
300049e0:	4620      	mov	r0, r4
300049e2:	463b      	mov	r3, r7
300049e4:	464a      	mov	r2, r9
300049e6:	f7ff fdd9 	bl	3000459c <_BOOT_Validate_ImgHash>
300049ea:	4604      	mov	r4, r0
300049ec:	f898 3011 	ldrb.w	r3, [r8, #17]
300049f0:	aa08      	add	r2, sp, #32
300049f2:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300049f6:	f853 1c10 	ldr.w	r1, [r3, #-16]
300049fa:	2c00      	cmp	r4, #0
300049fc:	f47f af62 	bne.w	300048c4 <BOOT_SignatureCheck+0x90>
30004a00:	4b0b      	ldr	r3, [pc, #44]	; (30004a30 <BOOT_SignatureCheck+0x1fc>)
30004a02:	2004      	movs	r0, #4
30004a04:	9100      	str	r1, [sp, #0]
30004a06:	4a0f      	ldr	r2, [pc, #60]	; (30004a44 <BOOT_SignatureCheck+0x210>)
30004a08:	4619      	mov	r1, r3
30004a0a:	f007 fb99 	bl	3000c140 <rtk_log_write>
30004a0e:	4620      	mov	r0, r4
30004a10:	e721      	b.n	30004856 <BOOT_SignatureCheck+0x22>
30004a12:	4620      	mov	r0, r4
30004a14:	463b      	mov	r3, r7
30004a16:	464a      	mov	r2, r9
30004a18:	f005 fb78 	bl	3000a10c <SBOOT_Validate_ImgHash>
30004a1c:	4604      	mov	r4, r0
30004a1e:	e7e5      	b.n	300049ec <BOOT_SignatureCheck+0x1b8>
30004a20:	22b4      	movs	r2, #180	; 0xb4
30004a22:	e73d      	b.n	300048a0 <BOOT_SignatureCheck+0x6c>
30004a24:	3000e6c0 	.word	0x3000e6c0
30004a28:	0000c149 	.word	0x0000c149
30004a2c:	3000a149 	.word	0x3000a149
30004a30:	3000c974 	.word	0x3000c974
30004a34:	3000cbc4 	.word	0x3000cbc4
30004a38:	00012be5 	.word	0x00012be5
30004a3c:	e000ed00 	.word	0xe000ed00
30004a40:	30009e65 	.word	0x30009e65
30004a44:	3000cbac 	.word	0x3000cbac

30004a48 <BOOT_CertificateCheck>:
30004a48:	b570      	push	{r4, r5, r6, lr}
30004a4a:	4b2b      	ldr	r3, [pc, #172]	; (30004af8 <BOOT_CertificateCheck+0xb0>)
30004a4c:	b0b0      	sub	sp, #192	; 0xc0
30004a4e:	4604      	mov	r4, r0
30004a50:	460d      	mov	r5, r1
30004a52:	4798      	blx	r3
30004a54:	b910      	cbnz	r0, 30004a5c <BOOT_CertificateCheck+0x14>
30004a56:	2000      	movs	r0, #0
30004a58:	b030      	add	sp, #192	; 0xc0
30004a5a:	bd70      	pop	{r4, r5, r6, pc}
30004a5c:	69a1      	ldr	r1, [r4, #24]
30004a5e:	2290      	movs	r2, #144	; 0x90
30004a60:	a80c      	add	r0, sp, #48	; 0x30
30004a62:	4429      	add	r1, r5
30004a64:	f001 f888 	bl	30005b78 <BOOT_ImgCopy>
30004a68:	4b24      	ldr	r3, [pc, #144]	; (30004afc <BOOT_CertificateCheck+0xb4>)
30004a6a:	4798      	blx	r3
30004a6c:	4924      	ldr	r1, [pc, #144]	; (30004b00 <BOOT_CertificateCheck+0xb8>)
30004a6e:	f000 031f 	and.w	r3, r0, #31
30004a72:	2220      	movs	r2, #32
30004a74:	6808      	ldr	r0, [r1, #0]
30004a76:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
30004a7a:	a930      	add	r1, sp, #192	; 0xc0
30004a7c:	3a01      	subs	r2, #1
30004a7e:	f813 c000 	ldrb.w	ip, [r3, r0]
30004a82:	4419      	add	r1, r3
30004a84:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30004a88:	f103 0301 	add.w	r3, r3, #1
30004a8c:	f801 ccb0 	strb.w	ip, [r1, #-176]
30004a90:	f003 031f 	and.w	r3, r3, #31
30004a94:	d1f1      	bne.n	30004a7a <BOOT_CertificateCheck+0x32>
30004a96:	4610      	mov	r0, r2
30004a98:	4b1a      	ldr	r3, [pc, #104]	; (30004b04 <BOOT_CertificateCheck+0xbc>)
30004a9a:	4798      	blx	r3
30004a9c:	7ce3      	ldrb	r3, [r4, #19]
30004a9e:	7ca2      	ldrb	r2, [r4, #18]
30004aa0:	f10d 010f 	add.w	r1, sp, #15
30004aa4:	f10d 000e 	add.w	r0, sp, #14
30004aa8:	f005 fb6a 	bl	3000a180 <SBOOT_Validate_Algorithm>
30004aac:	4605      	mov	r5, r0
30004aae:	b9c0      	cbnz	r0, 30004ae2 <BOOT_CertificateCheck+0x9a>
30004ab0:	f104 0620 	add.w	r6, r4, #32
30004ab4:	aa04      	add	r2, sp, #16
30004ab6:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004aba:	4631      	mov	r1, r6
30004abc:	f005 fae2 	bl	3000a084 <SBOOT_Validate_PubKey>
30004ac0:	4605      	mov	r5, r0
30004ac2:	b970      	cbnz	r0, 30004ae2 <BOOT_CertificateCheck+0x9a>
30004ac4:	69a0      	ldr	r0, [r4, #24]
30004ac6:	4623      	mov	r3, r4
30004ac8:	ac0c      	add	r4, sp, #48	; 0x30
30004aca:	4632      	mov	r2, r6
30004acc:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004ad0:	e9cd 0400 	strd	r0, r4, [sp]
30004ad4:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004ad8:	f005 faf2 	bl	3000a0c0 <SBOOT_Validate_Signature>
30004adc:	4605      	mov	r5, r0
30004ade:	2800      	cmp	r0, #0
30004ae0:	d0b9      	beq.n	30004a56 <BOOT_CertificateCheck+0xe>
30004ae2:	4b09      	ldr	r3, [pc, #36]	; (30004b08 <BOOT_CertificateCheck+0xc0>)
30004ae4:	2002      	movs	r0, #2
30004ae6:	4a09      	ldr	r2, [pc, #36]	; (30004b0c <BOOT_CertificateCheck+0xc4>)
30004ae8:	4619      	mov	r1, r3
30004aea:	9500      	str	r5, [sp, #0]
30004aec:	f007 fb28 	bl	3000c140 <rtk_log_write>
30004af0:	20ff      	movs	r0, #255	; 0xff
30004af2:	b030      	add	sp, #192	; 0xc0
30004af4:	bd70      	pop	{r4, r5, r6, pc}
30004af6:	bf00      	nop
30004af8:	0000c149 	.word	0x0000c149
30004afc:	0000ea4d 	.word	0x0000ea4d
30004b00:	3000e9f0 	.word	0x3000e9f0
30004b04:	3000a149 	.word	0x3000a149
30004b08:	3000c974 	.word	0x3000c974
30004b0c:	3000cbe8 	.word	0x3000cbe8

30004b10 <BOOT_RAM_TZCfg>:
30004b10:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004b14:	b085      	sub	sp, #20
30004b16:	4e58      	ldr	r6, [pc, #352]	; (30004c78 <BOOT_RAM_TZCfg+0x168>)
30004b18:	4958      	ldr	r1, [pc, #352]	; (30004c7c <BOOT_RAM_TZCfg+0x16c>)
30004b1a:	2501      	movs	r5, #1
30004b1c:	f10d 0e08 	add.w	lr, sp, #8
30004b20:	4a57      	ldr	r2, [pc, #348]	; (30004c80 <BOOT_RAM_TZCfg+0x170>)
30004b22:	4b58      	ldr	r3, [pc, #352]	; (30004c84 <BOOT_RAM_TZCfg+0x174>)
30004b24:	46ec      	mov	ip, sp
30004b26:	4677      	mov	r7, lr
30004b28:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30004ca4 <BOOT_RAM_TZCfg+0x194>
30004b2c:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
30004b30:	f8df 9174 	ldr.w	r9, [pc, #372]	; 30004ca8 <BOOT_RAM_TZCfg+0x198>
30004b34:	9202      	str	r2, [sp, #8]
30004b36:	9303      	str	r3, [sp, #12]
30004b38:	e9cd 6100 	strd	r6, r1, [sp]
30004b3c:	f85c 2b04 	ldr.w	r2, [ip], #4
30004b40:	2300      	movs	r3, #0
30004b42:	f85e 1b04 	ldr.w	r1, [lr], #4
30004b46:	680c      	ldr	r4, [r1, #0]
30004b48:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
30004b4c:	fa05 f003 	lsl.w	r0, r5, r3
30004b50:	f1b4 3fff 	cmp.w	r4, #4294967295
30004b54:	d00c      	beq.n	30004b70 <BOOT_RAM_TZCfg+0x60>
30004b56:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
30004b5a:	3301      	adds	r3, #1
30004b5c:	684c      	ldr	r4, [r1, #4]
30004b5e:	310c      	adds	r1, #12
30004b60:	2b08      	cmp	r3, #8
30004b62:	f8cb 4004 	str.w	r4, [fp, #4]
30004b66:	6c14      	ldr	r4, [r2, #64]	; 0x40
30004b68:	ea40 0004 	orr.w	r0, r0, r4
30004b6c:	6410      	str	r0, [r2, #64]	; 0x40
30004b6e:	d1ea      	bne.n	30004b46 <BOOT_RAM_TZCfg+0x36>
30004b70:	42b2      	cmp	r2, r6
30004b72:	d072      	beq.n	30004c5a <BOOT_RAM_TZCfg+0x14a>
30004b74:	6455      	str	r5, [r2, #68]	; 0x44
30004b76:	45bc      	cmp	ip, r7
30004b78:	d1e0      	bne.n	30004b3c <BOOT_RAM_TZCfg+0x2c>
30004b7a:	4b43      	ldr	r3, [pc, #268]	; (30004c88 <BOOT_RAM_TZCfg+0x178>)
30004b7c:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
30004b80:	f04f 32ff 	mov.w	r2, #4294967295
30004b84:	4841      	ldr	r0, [pc, #260]	; (30004c8c <BOOT_RAM_TZCfg+0x17c>)
30004b86:	6019      	str	r1, [r3, #0]
30004b88:	2501      	movs	r5, #1
30004b8a:	605a      	str	r2, [r3, #4]
30004b8c:	2100      	movs	r1, #0
30004b8e:	4a40      	ldr	r2, [pc, #256]	; (30004c90 <BOOT_RAM_TZCfg+0x180>)
30004b90:	4c40      	ldr	r4, [pc, #256]	; (30004c94 <BOOT_RAM_TZCfg+0x184>)
30004b92:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
30004b96:	6813      	ldr	r3, [r2, #0]
30004b98:	3301      	adds	r3, #1
30004b9a:	d019      	beq.n	30004bd0 <BOOT_RAM_TZCfg+0xc0>
30004b9c:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30004ba0:	320c      	adds	r2, #12
30004ba2:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30004ba6:	3101      	adds	r1, #1
30004ba8:	f023 031f 	bic.w	r3, r3, #31
30004bac:	2908      	cmp	r1, #8
30004bae:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30004bb2:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30004bb6:	ea4f 0343 	mov.w	r3, r3, lsl #1
30004bba:	f020 001f 	bic.w	r0, r0, #31
30004bbe:	f003 0302 	and.w	r3, r3, #2
30004bc2:	ea43 0300 	orr.w	r3, r3, r0
30004bc6:	f043 0301 	orr.w	r3, r3, #1
30004bca:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
30004bce:	d1e2      	bne.n	30004b96 <BOOT_RAM_TZCfg+0x86>
30004bd0:	4830      	ldr	r0, [pc, #192]	; (30004c94 <BOOT_RAM_TZCfg+0x184>)
30004bd2:	2201      	movs	r2, #1
30004bd4:	4b30      	ldr	r3, [pc, #192]	; (30004c98 <BOOT_RAM_TZCfg+0x188>)
30004bd6:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30004bda:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004bde:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30004be2:	d11e      	bne.n	30004c22 <BOOT_RAM_TZCfg+0x112>
30004be4:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004be8:	f3bf 8f4f 	dsb	sy
30004bec:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30004bf0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004bf4:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004bf8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004bfc:	0164      	lsls	r4, r4, #5
30004bfe:	ea04 0106 	and.w	r1, r4, r6
30004c02:	462b      	mov	r3, r5
30004c04:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004c08:	3b01      	subs	r3, #1
30004c0a:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004c0e:	1c5a      	adds	r2, r3, #1
30004c10:	d1f8      	bne.n	30004c04 <BOOT_RAM_TZCfg+0xf4>
30004c12:	3c20      	subs	r4, #32
30004c14:	f114 0f20 	cmn.w	r4, #32
30004c18:	d1f1      	bne.n	30004bfe <BOOT_RAM_TZCfg+0xee>
30004c1a:	f3bf 8f4f 	dsb	sy
30004c1e:	f3bf 8f6f 	isb	sy
30004c22:	4b1c      	ldr	r3, [pc, #112]	; (30004c94 <BOOT_RAM_TZCfg+0x184>)
30004c24:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30004c28:	491c      	ldr	r1, [pc, #112]	; (30004c9c <BOOT_RAM_TZCfg+0x18c>)
30004c2a:	691c      	ldr	r4, [r3, #16]
30004c2c:	4a1c      	ldr	r2, [pc, #112]	; (30004ca0 <BOOT_RAM_TZCfg+0x190>)
30004c2e:	f044 0408 	orr.w	r4, r4, #8
30004c32:	611c      	str	r4, [r3, #16]
30004c34:	f04f 34ff 	mov.w	r4, #4294967295
30004c38:	68d8      	ldr	r0, [r3, #12]
30004c3a:	4028      	ands	r0, r5
30004c3c:	4301      	orrs	r1, r0
30004c3e:	60d9      	str	r1, [r3, #12]
30004c40:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
30004c44:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
30004c48:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
30004c4c:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30004c50:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30004c54:	b005      	add	sp, #20
30004c56:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004c5a:	f898 3000 	ldrb.w	r3, [r8]
30004c5e:	2b00      	cmp	r3, #0
30004c60:	d189      	bne.n	30004b76 <BOOT_RAM_TZCfg+0x66>
30004c62:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30004c66:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
30004c6a:	6c33      	ldr	r3, [r6, #64]	; 0x40
30004c6c:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30004c70:	6433      	str	r3, [r6, #64]	; 0x40
30004c72:	6475      	str	r5, [r6, #68]	; 0x44
30004c74:	e77f      	b.n	30004b76 <BOOT_RAM_TZCfg+0x66>
30004c76:	bf00      	nop
30004c78:	51001a00 	.word	0x51001a00
30004c7c:	51001200 	.word	0x51001200
30004c80:	3000eaf0 	.word	0x3000eaf0
30004c84:	3000ea90 	.word	0x3000ea90
30004c88:	51001b00 	.word	0x51001b00
30004c8c:	500e0000 	.word	0x500e0000
30004c90:	3000eb50 	.word	0x3000eb50
30004c94:	e000ed00 	.word	0xe000ed00
30004c98:	42008000 	.word	0x42008000
30004c9c:	05fa6000 	.word	0x05fa6000
30004ca0:	e000e100 	.word	0xe000e100
30004ca4:	3000e9eb 	.word	0x3000e9eb
30004ca8:	005fffff 	.word	0x005fffff

30004cac <BOOT_INT_SecureFault>:
30004cac:	f3ef 8008 	mrs	r0, MSP
30004cb0:	f3ef 8109 	mrs	r1, PSP
30004cb4:	4672      	mov	r2, lr
30004cb6:	f04f 0304 	mov.w	r3, #4
30004cba:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004cbe:	f000 b895 	b.w	30004dec <BOOT_FaultHandler>
30004cc2:	4770      	bx	lr

30004cc4 <AsciiToHex>:
30004cc4:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30004cc8:	2b05      	cmp	r3, #5
30004cca:	d802      	bhi.n	30004cd2 <AsciiToHex+0xe>
30004ccc:	3837      	subs	r0, #55	; 0x37
30004cce:	b2c0      	uxtb	r0, r0
30004cd0:	4770      	bx	lr
30004cd2:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30004cd6:	2b05      	cmp	r3, #5
30004cd8:	d802      	bhi.n	30004ce0 <AsciiToHex+0x1c>
30004cda:	3857      	subs	r0, #87	; 0x57
30004cdc:	b2c0      	uxtb	r0, r0
30004cde:	4770      	bx	lr
30004ce0:	3830      	subs	r0, #48	; 0x30
30004ce2:	b2c0      	uxtb	r0, r0
30004ce4:	280a      	cmp	r0, #10
30004ce6:	bf28      	it	cs
30004ce8:	20ff      	movcs	r0, #255	; 0xff
30004cea:	4770      	bx	lr

30004cec <hotfix_get_addr_size>:
30004cec:	2800      	cmp	r0, #0
30004cee:	d054      	beq.n	30004d9a <hotfix_get_addr_size+0xae>
30004cf0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004cf4:	f04f 0a00 	mov.w	sl, #0
30004cf8:	b083      	sub	sp, #12
30004cfa:	f1a3 0b04 	sub.w	fp, r3, #4
30004cfe:	1c4b      	adds	r3, r1, #1
30004d00:	4688      	mov	r8, r1
30004d02:	4681      	mov	r9, r0
30004d04:	9300      	str	r3, [sp, #0]
30004d06:	4617      	mov	r7, r2
30004d08:	1e53      	subs	r3, r2, #1
30004d0a:	f101 0409 	add.w	r4, r1, #9
30004d0e:	1dd5      	adds	r5, r2, #7
30004d10:	4656      	mov	r6, sl
30004d12:	9301      	str	r3, [sp, #4]
30004d14:	f8c8 6004 	str.w	r6, [r8, #4]
30004d18:	210b      	movs	r1, #11
30004d1a:	f8c8 6000 	str.w	r6, [r8]
30004d1e:	4640      	mov	r0, r8
30004d20:	f8c8 6007 	str.w	r6, [r8, #7]
30004d24:	603e      	str	r6, [r7, #0]
30004d26:	607e      	str	r6, [r7, #4]
30004d28:	f002 ff68 	bl	30007bfc <ymodem_uart_getdata>
30004d2c:	e9dd 1000 	ldrd	r1, r0, [sp]
30004d30:	e004      	b.n	30004d3c <hotfix_get_addr_size+0x50>
30004d32:	b2d3      	uxtb	r3, r2
30004d34:	428c      	cmp	r4, r1
30004d36:	f800 3f01 	strb.w	r3, [r0, #1]!
30004d3a:	d01a      	beq.n	30004d72 <hotfix_get_addr_size+0x86>
30004d3c:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30004d40:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30004d44:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30004d48:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30004d4c:	f1be 0f05 	cmp.w	lr, #5
30004d50:	b2db      	uxtb	r3, r3
30004d52:	d9ef      	bls.n	30004d34 <hotfix_get_addr_size+0x48>
30004d54:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30004d58:	f1bc 0f05 	cmp.w	ip, #5
30004d5c:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30004d60:	b2db      	uxtb	r3, r3
30004d62:	d9e6      	bls.n	30004d32 <hotfix_get_addr_size+0x46>
30004d64:	2b0a      	cmp	r3, #10
30004d66:	bf28      	it	cs
30004d68:	23ff      	movcs	r3, #255	; 0xff
30004d6a:	428c      	cmp	r4, r1
30004d6c:	f800 3f01 	strb.w	r3, [r0, #1]!
30004d70:	d1e4      	bne.n	30004d3c <hotfix_get_addr_size+0x50>
30004d72:	4639      	mov	r1, r7
30004d74:	2300      	movs	r3, #0
30004d76:	f811 2b01 	ldrb.w	r2, [r1], #1
30004d7a:	4313      	orrs	r3, r2
30004d7c:	42a9      	cmp	r1, r5
30004d7e:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004d82:	d1f8      	bne.n	30004d76 <hotfix_get_addr_size+0x8a>
30004d84:	79fa      	ldrb	r2, [r7, #7]
30004d86:	f10a 0a01 	add.w	sl, sl, #1
30004d8a:	4313      	orrs	r3, r2
30004d8c:	45ca      	cmp	sl, r9
30004d8e:	f84b 3f04 	str.w	r3, [fp, #4]!
30004d92:	d1bf      	bne.n	30004d14 <hotfix_get_addr_size+0x28>
30004d94:	b003      	add	sp, #12
30004d96:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004d9a:	4770      	bx	lr

30004d9c <ascii_to_integer>:
30004d9c:	2900      	cmp	r1, #0
30004d9e:	4684      	mov	ip, r0
30004da0:	dd12      	ble.n	30004dc8 <ascii_to_integer+0x2c>
30004da2:	3901      	subs	r1, #1
30004da4:	1e42      	subs	r2, r0, #1
30004da6:	2000      	movs	r0, #0
30004da8:	448c      	add	ip, r1
30004daa:	e001      	b.n	30004db0 <ascii_to_integer+0x14>
30004dac:	4562      	cmp	r2, ip
30004dae:	d00d      	beq.n	30004dcc <ascii_to_integer+0x30>
30004db0:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30004db4:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30004db8:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30004dbc:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30004dc0:	2909      	cmp	r1, #9
30004dc2:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30004dc6:	d9f1      	bls.n	30004dac <ascii_to_integer+0x10>
30004dc8:	2000      	movs	r0, #0
30004dca:	4770      	bx	lr
30004dcc:	4770      	bx	lr
30004dce:	bf00      	nop

30004dd0 <eight_4bit_to_int>:
30004dd0:	2300      	movs	r3, #0
30004dd2:	f100 0c07 	add.w	ip, r0, #7
30004dd6:	4602      	mov	r2, r0
30004dd8:	f812 1b01 	ldrb.w	r1, [r2], #1
30004ddc:	430b      	orrs	r3, r1
30004dde:	4562      	cmp	r2, ip
30004de0:	ea4f 1303 	mov.w	r3, r3, lsl #4
30004de4:	d1f8      	bne.n	30004dd8 <eight_4bit_to_int+0x8>
30004de6:	79c0      	ldrb	r0, [r0, #7]
30004de8:	4318      	orrs	r0, r3
30004dea:	4770      	bx	lr

30004dec <BOOT_FaultHandler>:
30004dec:	b5f0      	push	{r4, r5, r6, r7, lr}
30004dee:	461f      	mov	r7, r3
30004df0:	4b39      	ldr	r3, [pc, #228]	; (30004ed8 <BOOT_FaultHandler+0xec>)
30004df2:	b095      	sub	sp, #84	; 0x54
30004df4:	4616      	mov	r6, r2
30004df6:	4604      	mov	r4, r0
30004df8:	4a38      	ldr	r2, [pc, #224]	; (30004edc <BOOT_FaultHandler+0xf0>)
30004dfa:	2001      	movs	r0, #1
30004dfc:	460d      	mov	r5, r1
30004dfe:	4619      	mov	r1, r3
30004e00:	f007 f99e 	bl	3000c140 <rtk_log_write>
30004e04:	2244      	movs	r2, #68	; 0x44
30004e06:	2100      	movs	r1, #0
30004e08:	a803      	add	r0, sp, #12
30004e0a:	f007 fc15 	bl	3000c638 <____wrap_memset_veneer>
30004e0e:	0672      	lsls	r2, r6, #25
30004e10:	4b31      	ldr	r3, [pc, #196]	; (30004ed8 <BOOT_FaultHandler+0xec>)
30004e12:	d54f      	bpl.n	30004eb4 <BOOT_FaultHandler+0xc8>
30004e14:	4619      	mov	r1, r3
30004e16:	4a32      	ldr	r2, [pc, #200]	; (30004ee0 <BOOT_FaultHandler+0xf4>)
30004e18:	2001      	movs	r0, #1
30004e1a:	f007 f991 	bl	3000c140 <rtk_log_write>
30004e1e:	f006 010c 	and.w	r1, r6, #12
30004e22:	46a6      	mov	lr, r4
30004e24:	f1a1 010c 	sub.w	r1, r1, #12
30004e28:	fab1 f181 	clz	r1, r1
30004e2c:	0949      	lsrs	r1, r1, #5
30004e2e:	f1a4 0c24 	sub.w	ip, r4, #36	; 0x24
30004e32:	1f22      	subs	r2, r4, #4
30004e34:	a804      	add	r0, sp, #16
30004e36:	9603      	str	r6, [sp, #12]
30004e38:	f85c 3f04 	ldr.w	r3, [ip, #4]!
30004e3c:	4594      	cmp	ip, r2
30004e3e:	f840 3b04 	str.w	r3, [r0], #4
30004e42:	d1f9      	bne.n	30004e38 <BOOT_FaultHandler+0x4c>
30004e44:	2900      	cmp	r1, #0
30004e46:	bf08      	it	eq
30004e48:	4675      	moveq	r5, lr
30004e4a:	f10d 0c30 	add.w	ip, sp, #48	; 0x30
30004e4e:	1f28      	subs	r0, r5, #4
30004e50:	f105 031c 	add.w	r3, r5, #28
30004e54:	f850 4f04 	ldr.w	r4, [r0, #4]!
30004e58:	4283      	cmp	r3, r0
30004e5a:	f84c 4b04 	str.w	r4, [ip], #4
30004e5e:	d1f9      	bne.n	30004e54 <BOOT_FaultHandler+0x68>
30004e60:	4c20      	ldr	r4, [pc, #128]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004e62:	aa03      	add	r2, sp, #12
30004e64:	4629      	mov	r1, r5
30004e66:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30004e68:	f006 fe94 	bl	3000bb94 <crash_dump>
30004e6c:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30004e70:	4b19      	ldr	r3, [pc, #100]	; (30004ed8 <BOOT_FaultHandler+0xec>)
30004e72:	2001      	movs	r0, #1
30004e74:	9100      	str	r1, [sp, #0]
30004e76:	4a1c      	ldr	r2, [pc, #112]	; (30004ee8 <BOOT_FaultHandler+0xfc>)
30004e78:	4619      	mov	r1, r3
30004e7a:	f007 f961 	bl	3000c140 <rtk_log_write>
30004e7e:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30004e82:	4b15      	ldr	r3, [pc, #84]	; (30004ed8 <BOOT_FaultHandler+0xec>)
30004e84:	2001      	movs	r0, #1
30004e86:	9200      	str	r2, [sp, #0]
30004e88:	4619      	mov	r1, r3
30004e8a:	4a18      	ldr	r2, [pc, #96]	; (30004eec <BOOT_FaultHandler+0x100>)
30004e8c:	f007 f958 	bl	3000c140 <rtk_log_write>
30004e90:	6a61      	ldr	r1, [r4, #36]	; 0x24
30004e92:	4b11      	ldr	r3, [pc, #68]	; (30004ed8 <BOOT_FaultHandler+0xec>)
30004e94:	2001      	movs	r0, #1
30004e96:	9100      	str	r1, [sp, #0]
30004e98:	4a15      	ldr	r2, [pc, #84]	; (30004ef0 <BOOT_FaultHandler+0x104>)
30004e9a:	4619      	mov	r1, r3
30004e9c:	f007 f950 	bl	3000c140 <rtk_log_write>
30004ea0:	f3ef 8088 	mrs	r0, MSP_NS
30004ea4:	f3ef 8189 	mrs	r1, PSP_NS
30004ea8:	463b      	mov	r3, r7
30004eaa:	4632      	mov	r2, r6
30004eac:	f007 fbbc 	bl	3000c628 <__INT_HardFault_C_veneer>
30004eb0:	b015      	add	sp, #84	; 0x54
30004eb2:	bdf0      	pop	{r4, r5, r6, r7, pc}
30004eb4:	4a0f      	ldr	r2, [pc, #60]	; (30004ef4 <BOOT_FaultHandler+0x108>)
30004eb6:	2001      	movs	r0, #1
30004eb8:	4619      	mov	r1, r3
30004eba:	f007 f941 	bl	3000c140 <rtk_log_write>
30004ebe:	f3ef 8394 	mrs	r3, CONTROL_NS
30004ec2:	f3ef 8e88 	mrs	lr, MSP_NS
30004ec6:	f3ef 8589 	mrs	r5, PSP_NS
30004eca:	f016 0108 	ands.w	r1, r6, #8
30004ece:	bf18      	it	ne
30004ed0:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30004ed4:	e7ab      	b.n	30004e2e <BOOT_FaultHandler+0x42>
30004ed6:	bf00      	nop
30004ed8:	3000c974 	.word	0x3000c974
30004edc:	3000cc14 	.word	0x3000cc14
30004ee0:	3000cc38 	.word	0x3000cc38
30004ee4:	e000ed00 	.word	0xe000ed00
30004ee8:	3000cc8c 	.word	0x3000cc8c
30004eec:	3000cca4 	.word	0x3000cca4
30004ef0:	3000ccbc 	.word	0x3000ccbc
30004ef4:	3000cc60 	.word	0x3000cc60

30004ef8 <FLASH_Erase_With_Lock>:
30004ef8:	4b11      	ldr	r3, [pc, #68]	; (30004f40 <FLASH_Erase_With_Lock+0x48>)
30004efa:	b510      	push	{r4, lr}
30004efc:	460c      	mov	r4, r1
30004efe:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30004f02:	4798      	blx	r3
30004f04:	4b0f      	ldr	r3, [pc, #60]	; (30004f44 <FLASH_Erase_With_Lock+0x4c>)
30004f06:	695b      	ldr	r3, [r3, #20]
30004f08:	03db      	lsls	r3, r3, #15
30004f0a:	d512      	bpl.n	30004f32 <FLASH_Erase_With_Lock+0x3a>
30004f0c:	f014 021f 	ands.w	r2, r4, #31
30004f10:	d110      	bne.n	30004f34 <FLASH_Erase_With_Lock+0x3c>
30004f12:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30004f16:	f3bf 8f4f 	dsb	sy
30004f1a:	480a      	ldr	r0, [pc, #40]	; (30004f44 <FLASH_Erase_With_Lock+0x4c>)
30004f1c:	4422      	add	r2, r4
30004f1e:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30004f22:	3420      	adds	r4, #32
30004f24:	1b13      	subs	r3, r2, r4
30004f26:	2b00      	cmp	r3, #0
30004f28:	dcf9      	bgt.n	30004f1e <FLASH_Erase_With_Lock+0x26>
30004f2a:	f3bf 8f4f 	dsb	sy
30004f2e:	f3bf 8f6f 	isb	sy
30004f32:	bd10      	pop	{r4, pc}
30004f34:	f024 041f 	bic.w	r4, r4, #31
30004f38:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004f3c:	e7eb      	b.n	30004f16 <FLASH_Erase_With_Lock+0x1e>
30004f3e:	bf00      	nop
30004f40:	0000a16d 	.word	0x0000a16d
30004f44:	e000ed00 	.word	0xe000ed00

30004f48 <FLASH_TxData_With_Lock>:
30004f48:	b538      	push	{r3, r4, r5, lr}
30004f4a:	4b13      	ldr	r3, [pc, #76]	; (30004f98 <FLASH_TxData_With_Lock+0x50>)
30004f4c:	4604      	mov	r4, r0
30004f4e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30004f52:	460d      	mov	r5, r1
30004f54:	4798      	blx	r3
30004f56:	4b11      	ldr	r3, [pc, #68]	; (30004f9c <FLASH_TxData_With_Lock+0x54>)
30004f58:	695b      	ldr	r3, [r3, #20]
30004f5a:	03db      	lsls	r3, r3, #15
30004f5c:	d517      	bpl.n	30004f8e <FLASH_TxData_With_Lock+0x46>
30004f5e:	f014 031f 	ands.w	r3, r4, #31
30004f62:	d115      	bne.n	30004f90 <FLASH_TxData_With_Lock+0x48>
30004f64:	f3bf 8f4f 	dsb	sy
30004f68:	b16d      	cbz	r5, 30004f86 <FLASH_TxData_With_Lock+0x3e>
30004f6a:	1e6a      	subs	r2, r5, #1
30004f6c:	f104 0320 	add.w	r3, r4, #32
30004f70:	490a      	ldr	r1, [pc, #40]	; (30004f9c <FLASH_TxData_With_Lock+0x54>)
30004f72:	f022 021f 	bic.w	r2, r2, #31
30004f76:	441a      	add	r2, r3
30004f78:	4293      	cmp	r3, r2
30004f7a:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30004f7e:	461c      	mov	r4, r3
30004f80:	f103 0320 	add.w	r3, r3, #32
30004f84:	d1f8      	bne.n	30004f78 <FLASH_TxData_With_Lock+0x30>
30004f86:	f3bf 8f4f 	dsb	sy
30004f8a:	f3bf 8f6f 	isb	sy
30004f8e:	bd38      	pop	{r3, r4, r5, pc}
30004f90:	f024 041f 	bic.w	r4, r4, #31
30004f94:	441d      	add	r5, r3
30004f96:	e7e5      	b.n	30004f64 <FLASH_TxData_With_Lock+0x1c>
30004f98:	0000a04d 	.word	0x0000a04d
30004f9c:	e000ed00 	.word	0xe000ed00

30004fa0 <flash_read_id>:
30004fa0:	b538      	push	{r3, r4, r5, lr}
30004fa2:	4d04      	ldr	r5, [pc, #16]	; (30004fb4 <flash_read_id+0x14>)
30004fa4:	460c      	mov	r4, r1
30004fa6:	4602      	mov	r2, r0
30004fa8:	4b03      	ldr	r3, [pc, #12]	; (30004fb8 <flash_read_id+0x18>)
30004faa:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30004fae:	4798      	blx	r3
30004fb0:	4620      	mov	r0, r4
30004fb2:	bd38      	pop	{r3, r4, r5, pc}
30004fb4:	2001c01c 	.word	0x2001c01c
30004fb8:	00009db5 	.word	0x00009db5

30004fbc <mcc_ReverseBit>:
30004fbc:	0843      	lsrs	r3, r0, #1
30004fbe:	08c1      	lsrs	r1, r0, #3
30004fc0:	f003 0308 	and.w	r3, r3, #8
30004fc4:	0942      	lsrs	r2, r0, #5
30004fc6:	f001 0104 	and.w	r1, r1, #4
30004fca:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30004fce:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30004fd2:	f002 0202 	and.w	r2, r2, #2
30004fd6:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30004fda:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30004fde:	430b      	orrs	r3, r1
30004fe0:	00c1      	lsls	r1, r0, #3
30004fe2:	4313      	orrs	r3, r2
30004fe4:	f001 0120 	and.w	r1, r1, #32
30004fe8:	0042      	lsls	r2, r0, #1
30004fea:	ea43 000c 	orr.w	r0, r3, ip
30004fee:	f002 0310 	and.w	r3, r2, #16
30004ff2:	4308      	orrs	r0, r1
30004ff4:	4318      	orrs	r0, r3
30004ff6:	b2c0      	uxtb	r0, r0
30004ff8:	4770      	bx	lr
30004ffa:	bf00      	nop

30004ffc <mcc_init_crc32>:
30004ffc:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30004ffe:	2600      	movs	r6, #0
30005000:	4f16      	ldr	r7, [pc, #88]	; (3000505c <mcc_init_crc32+0x60>)
30005002:	4605      	mov	r5, r0
30005004:	b2f0      	uxtb	r0, r6
30005006:	f7ff ffd9 	bl	30004fbc <mcc_ReverseBit>
3000500a:	2208      	movs	r2, #8
3000500c:	0604      	lsls	r4, r0, #24
3000500e:	2c00      	cmp	r4, #0
30005010:	ea4f 0444 	mov.w	r4, r4, lsl #1
30005014:	bfb8      	it	lt
30005016:	407c      	eorlt	r4, r7
30005018:	3a01      	subs	r2, #1
3000501a:	d1f8      	bne.n	3000500e <mcc_init_crc32+0x12>
3000501c:	0e20      	lsrs	r0, r4, #24
3000501e:	3504      	adds	r5, #4
30005020:	f7ff ffcc 	bl	30004fbc <mcc_ReverseBit>
30005024:	4603      	mov	r3, r0
30005026:	f3c4 4007 	ubfx	r0, r4, #16, #8
3000502a:	3601      	adds	r6, #1
3000502c:	f805 3c04 	strb.w	r3, [r5, #-4]
30005030:	f7ff ffc4 	bl	30004fbc <mcc_ReverseBit>
30005034:	4603      	mov	r3, r0
30005036:	f3c4 2007 	ubfx	r0, r4, #8, #8
3000503a:	f805 3c03 	strb.w	r3, [r5, #-3]
3000503e:	f7ff ffbd 	bl	30004fbc <mcc_ReverseBit>
30005042:	4603      	mov	r3, r0
30005044:	b2e0      	uxtb	r0, r4
30005046:	f805 3c02 	strb.w	r3, [r5, #-2]
3000504a:	f7ff ffb7 	bl	30004fbc <mcc_ReverseBit>
3000504e:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30005052:	f805 0c01 	strb.w	r0, [r5, #-1]
30005056:	d1d5      	bne.n	30005004 <mcc_init_crc32+0x8>
30005058:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
3000505a:	bf00      	nop
3000505c:	04c11db7 	.word	0x04c11db7

30005060 <mcc_crc32>:
30005060:	b179      	cbz	r1, 30005082 <mcc_crc32+0x22>
30005062:	eb00 0c01 	add.w	ip, r0, r1
30005066:	f04f 31ff 	mov.w	r1, #4294967295
3000506a:	f810 3b01 	ldrb.w	r3, [r0], #1
3000506e:	404b      	eors	r3, r1
30005070:	4560      	cmp	r0, ip
30005072:	b2db      	uxtb	r3, r3
30005074:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30005078:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
3000507c:	d1f5      	bne.n	3000506a <mcc_crc32+0xa>
3000507e:	43c8      	mvns	r0, r1
30005080:	4770      	bx	lr
30005082:	4608      	mov	r0, r1
30005084:	4770      	bx	lr
30005086:	bf00      	nop

30005088 <recovery_check>:
30005088:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
3000508c:	4283      	cmp	r3, r0
3000508e:	d322      	bcc.n	300050d6 <recovery_check+0x4e>
30005090:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30005094:	d91f      	bls.n	300050d6 <recovery_check+0x4e>
30005096:	b510      	push	{r4, lr}
30005098:	f8d0 400a 	ldr.w	r4, [r0, #10]
3000509c:	f104 030c 	add.w	r3, r4, #12
300050a0:	4299      	cmp	r1, r3
300050a2:	d316      	bcc.n	300050d2 <recovery_check+0x4a>
300050a4:	f100 0c04 	add.w	ip, r0, #4
300050a8:	b183      	cbz	r3, 300050cc <recovery_check+0x44>
300050aa:	f100 0e10 	add.w	lr, r0, #16
300050ae:	f04f 31ff 	mov.w	r1, #4294967295
300050b2:	44a6      	add	lr, r4
300050b4:	4c0b      	ldr	r4, [pc, #44]	; (300050e4 <recovery_check+0x5c>)
300050b6:	f81c 3b01 	ldrb.w	r3, [ip], #1
300050ba:	404b      	eors	r3, r1
300050bc:	45f4      	cmp	ip, lr
300050be:	b2db      	uxtb	r3, r3
300050c0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300050c4:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
300050c8:	d1f5      	bne.n	300050b6 <recovery_check+0x2e>
300050ca:	43cb      	mvns	r3, r1
300050cc:	6801      	ldr	r1, [r0, #0]
300050ce:	4299      	cmp	r1, r3
300050d0:	d003      	beq.n	300050da <recovery_check+0x52>
300050d2:	2000      	movs	r0, #0
300050d4:	bd10      	pop	{r4, pc}
300050d6:	2000      	movs	r0, #0
300050d8:	4770      	bx	lr
300050da:	f8d0 3006 	ldr.w	r3, [r0, #6]
300050de:	2001      	movs	r0, #1
300050e0:	6013      	str	r3, [r2, #0]
300050e2:	bd10      	pop	{r4, pc}
300050e4:	3000ebb0 	.word	0x3000ebb0

300050e8 <rewrite_bp>:
300050e8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300050ec:	b0a3      	sub	sp, #140	; 0x8c
300050ee:	4616      	mov	r6, r2
300050f0:	4607      	mov	r7, r0
300050f2:	460d      	mov	r5, r1
300050f4:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
300050f8:	4699      	mov	r9, r3
300050fa:	f10d 0808 	add.w	r8, sp, #8
300050fe:	2280      	movs	r2, #128	; 0x80
30005100:	2c00      	cmp	r4, #0
30005102:	f000 80b2 	beq.w	3000526a <rewrite_bp+0x182>
30005106:	4619      	mov	r1, r3
30005108:	4640      	mov	r0, r8
3000510a:	f007 fa7d 	bl	3000c608 <____wrap_memcpy_veneer>
3000510e:	2301      	movs	r3, #1
30005110:	f109 0404 	add.w	r4, r9, #4
30005114:	4649      	mov	r1, r9
30005116:	2002      	movs	r0, #2
30005118:	f88d 7014 	strb.w	r7, [sp, #20]
3000511c:	e9cd 3303 	strd	r3, r3, [sp, #12]
30005120:	f7ff feea 	bl	30004ef8 <FLASH_Erase_With_Lock>
30005124:	4620      	mov	r0, r4
30005126:	217c      	movs	r1, #124	; 0x7c
30005128:	aa03      	add	r2, sp, #12
3000512a:	f7ff ff0d 	bl	30004f48 <FLASH_TxData_With_Lock>
3000512e:	4620      	mov	r0, r4
30005130:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30005134:	f04f 33ff 	mov.w	r3, #4294967295
30005138:	4c54      	ldr	r4, [pc, #336]	; (3000528c <rewrite_bp+0x1a4>)
3000513a:	f810 cb01 	ldrb.w	ip, [r0], #1
3000513e:	ea8c 0c03 	eor.w	ip, ip, r3
30005142:	4288      	cmp	r0, r1
30005144:	fa5f fc8c 	uxtb.w	ip, ip
30005148:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
3000514c:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005150:	d1f3      	bne.n	3000513a <rewrite_bp+0x52>
30005152:	43db      	mvns	r3, r3
30005154:	4649      	mov	r1, r9
30005156:	2002      	movs	r0, #2
30005158:	9302      	str	r3, [sp, #8]
3000515a:	f7ff fecd 	bl	30004ef8 <FLASH_Erase_With_Lock>
3000515e:	4642      	mov	r2, r8
30005160:	4648      	mov	r0, r9
30005162:	2180      	movs	r1, #128	; 0x80
30005164:	f7ff fef0 	bl	30004f48 <FLASH_TxData_With_Lock>
30005168:	4b49      	ldr	r3, [pc, #292]	; (30005290 <rewrite_bp+0x1a8>)
3000516a:	aa01      	add	r2, sp, #4
3000516c:	2104      	movs	r1, #4
3000516e:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005172:	4b48      	ldr	r3, [pc, #288]	; (30005294 <rewrite_bp+0x1ac>)
30005174:	4798      	blx	r3
30005176:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000517a:	f04f 0c01 	mov.w	ip, #1
3000517e:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30005182:	682a      	ldr	r2, [r5, #0]
30005184:	fa0c fc03 	lsl.w	ip, ip, r3
30005188:	f10e 0e0c 	add.w	lr, lr, #12
3000518c:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005190:	45e6      	cmp	lr, ip
30005192:	6831      	ldr	r1, [r6, #0]
30005194:	f103 030c 	add.w	r3, r3, #12
30005198:	bf28      	it	cs
3000519a:	46e6      	movcs	lr, ip
3000519c:	459c      	cmp	ip, r3
3000519e:	bf28      	it	cs
300051a0:	469c      	movcs	ip, r3
300051a2:	bb6f      	cbnz	r7, 30005200 <rewrite_bp+0x118>
300051a4:	1d28      	adds	r0, r5, #4
300051a6:	f1be 0f00 	cmp.w	lr, #0
300051aa:	d00e      	beq.n	300051ca <rewrite_bp+0xe2>
300051ac:	4486      	add	lr, r0
300051ae:	f04f 37ff 	mov.w	r7, #4294967295
300051b2:	f810 3b01 	ldrb.w	r3, [r0], #1
300051b6:	407b      	eors	r3, r7
300051b8:	4570      	cmp	r0, lr
300051ba:	b2db      	uxtb	r3, r3
300051bc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051c0:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300051c4:	d1f5      	bne.n	300051b2 <rewrite_bp+0xca>
300051c6:	ea6f 0e07 	mvn.w	lr, r7
300051ca:	4572      	cmp	r2, lr
300051cc:	d042      	beq.n	30005254 <rewrite_bp+0x16c>
300051ce:	1d32      	adds	r2, r6, #4
300051d0:	f1bc 0f00 	cmp.w	ip, #0
300051d4:	d00e      	beq.n	300051f4 <rewrite_bp+0x10c>
300051d6:	4494      	add	ip, r2
300051d8:	f04f 37ff 	mov.w	r7, #4294967295
300051dc:	f812 3b01 	ldrb.w	r3, [r2], #1
300051e0:	407b      	eors	r3, r7
300051e2:	4562      	cmp	r2, ip
300051e4:	b2db      	uxtb	r3, r3
300051e6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051ea:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300051ee:	d1f5      	bne.n	300051dc <rewrite_bp+0xf4>
300051f0:	ea6f 0c07 	mvn.w	ip, r7
300051f4:	4561      	cmp	r1, ip
300051f6:	d03d      	beq.n	30005274 <rewrite_bp+0x18c>
300051f8:	4827      	ldr	r0, [pc, #156]	; (30005298 <rewrite_bp+0x1b0>)
300051fa:	f007 fa45 	bl	3000c688 <__DiagPrintf_veneer>
300051fe:	e7fe      	b.n	300051fe <rewrite_bp+0x116>
30005200:	1d30      	adds	r0, r6, #4
30005202:	f1bc 0f00 	cmp.w	ip, #0
30005206:	d00e      	beq.n	30005226 <rewrite_bp+0x13e>
30005208:	4484      	add	ip, r0
3000520a:	f04f 37ff 	mov.w	r7, #4294967295
3000520e:	f810 3b01 	ldrb.w	r3, [r0], #1
30005212:	407b      	eors	r3, r7
30005214:	4560      	cmp	r0, ip
30005216:	b2db      	uxtb	r3, r3
30005218:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000521c:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005220:	d1f5      	bne.n	3000520e <rewrite_bp+0x126>
30005222:	ea6f 0c07 	mvn.w	ip, r7
30005226:	4561      	cmp	r1, ip
30005228:	d024      	beq.n	30005274 <rewrite_bp+0x18c>
3000522a:	1d29      	adds	r1, r5, #4
3000522c:	f1be 0f00 	cmp.w	lr, #0
30005230:	d00e      	beq.n	30005250 <rewrite_bp+0x168>
30005232:	448e      	add	lr, r1
30005234:	f04f 37ff 	mov.w	r7, #4294967295
30005238:	f811 3b01 	ldrb.w	r3, [r1], #1
3000523c:	407b      	eors	r3, r7
3000523e:	4571      	cmp	r1, lr
30005240:	b2db      	uxtb	r3, r3
30005242:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005246:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000524a:	d1f5      	bne.n	30005238 <rewrite_bp+0x150>
3000524c:	ea6f 0e07 	mvn.w	lr, r7
30005250:	4572      	cmp	r2, lr
30005252:	d1d1      	bne.n	300051f8 <rewrite_bp+0x110>
30005254:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30005258:	4810      	ldr	r0, [pc, #64]	; (3000529c <rewrite_bp+0x1b4>)
3000525a:	f8d5 1006 	ldr.w	r1, [r5, #6]
3000525e:	f007 fa13 	bl	3000c688 <__DiagPrintf_veneer>
30005262:	2001      	movs	r0, #1
30005264:	b023      	add	sp, #140	; 0x8c
30005266:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000526a:	992a      	ldr	r1, [sp, #168]	; 0xa8
3000526c:	4640      	mov	r0, r8
3000526e:	f007 f9cb 	bl	3000c608 <____wrap_memcpy_veneer>
30005272:	e74c      	b.n	3000510e <rewrite_bp+0x26>
30005274:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
30005278:	4809      	ldr	r0, [pc, #36]	; (300052a0 <rewrite_bp+0x1b8>)
3000527a:	f8d6 1006 	ldr.w	r1, [r6, #6]
3000527e:	f007 fa03 	bl	3000c688 <__DiagPrintf_veneer>
30005282:	2002      	movs	r0, #2
30005284:	b023      	add	sp, #140	; 0x8c
30005286:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000528a:	bf00      	nop
3000528c:	3000ebb0 	.word	0x3000ebb0
30005290:	2001c01c 	.word	0x2001c01c
30005294:	00009db5 	.word	0x00009db5
30005298:	3000cd04 	.word	0x3000cd04
3000529c:	3000ccd4 	.word	0x3000ccd4
300052a0:	3000ccec 	.word	0x3000ccec

300052a4 <BOOT_SectionInit>:
300052a4:	4800      	ldr	r0, [pc, #0]	; (300052a8 <BOOT_SectionInit+0x4>)
300052a6:	4770      	bx	lr
300052a8:	60000020 	.word	0x60000020

300052ac <BOOT_NsStart>:
300052ac:	b570      	push	{r4, r5, r6, lr}
300052ae:	4c23      	ldr	r4, [pc, #140]	; (3000533c <BOOT_NsStart+0x90>)
300052b0:	4605      	mov	r5, r0
300052b2:	f384 8808 	msr	MSP, r4
300052b6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300052ba:	2100      	movs	r1, #0
300052bc:	4620      	mov	r0, r4
300052be:	4e20      	ldr	r6, [pc, #128]	; (30005340 <BOOT_NsStart+0x94>)
300052c0:	47b0      	blx	r6
300052c2:	4a20      	ldr	r2, [pc, #128]	; (30005344 <BOOT_NsStart+0x98>)
300052c4:	6953      	ldr	r3, [r2, #20]
300052c6:	03d9      	lsls	r1, r3, #15
300052c8:	d50b      	bpl.n	300052e2 <BOOT_NsStart+0x36>
300052ca:	f3bf 8f4f 	dsb	sy
300052ce:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300052d2:	3420      	adds	r4, #32
300052d4:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300052d8:	d1f9      	bne.n	300052ce <BOOT_NsStart+0x22>
300052da:	f3bf 8f4f 	dsb	sy
300052de:	f3bf 8f6f 	isb	sy
300052e2:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300052e6:	f383 8808 	msr	MSP, r3
300052ea:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300052ee:	2100      	movs	r1, #0
300052f0:	4815      	ldr	r0, [pc, #84]	; (30005348 <BOOT_NsStart+0x9c>)
300052f2:	47b0      	blx	r6
300052f4:	4a13      	ldr	r2, [pc, #76]	; (30005344 <BOOT_NsStart+0x98>)
300052f6:	6953      	ldr	r3, [r2, #20]
300052f8:	03db      	lsls	r3, r3, #15
300052fa:	d50c      	bpl.n	30005316 <BOOT_NsStart+0x6a>
300052fc:	f3bf 8f4f 	dsb	sy
30005300:	4b11      	ldr	r3, [pc, #68]	; (30005348 <BOOT_NsStart+0x9c>)
30005302:	490e      	ldr	r1, [pc, #56]	; (3000533c <BOOT_NsStart+0x90>)
30005304:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30005308:	3320      	adds	r3, #32
3000530a:	428b      	cmp	r3, r1
3000530c:	d1fa      	bne.n	30005304 <BOOT_NsStart+0x58>
3000530e:	f3bf 8f4f 	dsb	sy
30005312:	f3bf 8f6f 	isb	sy
30005316:	f025 0501 	bic.w	r5, r5, #1
3000531a:	086d      	lsrs	r5, r5, #1
3000531c:	006d      	lsls	r5, r5, #1
3000531e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005322:	ed2d 8b10 	vpush	{d8-d15}
30005326:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
3000532a:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
3000532e:	47ac      	blxns	r5
30005330:	ecbd 8b10 	vpop	{d8-d15}
30005334:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005338:	bd70      	pop	{r4, r5, r6, pc}
3000533a:	bf00      	nop
3000533c:	30002000 	.word	0x30002000
30005340:	00012be5 	.word	0x00012be5
30005344:	e000ed00 	.word	0xe000ed00
30005348:	30001000 	.word	0x30001000

3000534c <BOOT_RccConfig>:
3000534c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005350:	4c2f      	ldr	r4, [pc, #188]	; (30005410 <BOOT_RccConfig+0xc4>)
30005352:	b090      	sub	sp, #64	; 0x40
30005354:	4f2f      	ldr	r7, [pc, #188]	; (30005414 <BOOT_RccConfig+0xc8>)
30005356:	466d      	mov	r5, sp
30005358:	f10d 0c20 	add.w	ip, sp, #32
3000535c:	683e      	ldr	r6, [r7, #0]
3000535e:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
30005362:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30005366:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
3000536a:	f104 0310 	add.w	r3, r4, #16
3000536e:	2400      	movs	r4, #0
30005370:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30005372:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
30005376:	1c72      	adds	r2, r6, #1
30005378:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000537c:	e9cd 4406 	strd	r4, r4, [sp, #24]
30005380:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
30005384:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30005388:	d03f      	beq.n	3000540a <BOOT_RccConfig+0xbe>
3000538a:	463a      	mov	r2, r7
3000538c:	6891      	ldr	r1, [r2, #8]
3000538e:	0fb3      	lsrs	r3, r6, #30
30005390:	a810      	add	r0, sp, #64	; 0x40
30005392:	2901      	cmp	r1, #1
30005394:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30005398:	d10e      	bne.n	300053b8 <BOOT_RccConfig+0x6c>
3000539a:	f853 0c30 	ldr.w	r0, [r3, #-48]
3000539e:	6854      	ldr	r4, [r2, #4]
300053a0:	4330      	orrs	r0, r6
300053a2:	ae10      	add	r6, sp, #64	; 0x40
300053a4:	0fa1      	lsrs	r1, r4, #30
300053a6:	f843 0c30 	str.w	r0, [r3, #-48]
300053aa:	eb06 0181 	add.w	r1, r6, r1, lsl #2
300053ae:	f851 3c10 	ldr.w	r3, [r1, #-16]
300053b2:	4323      	orrs	r3, r4
300053b4:	f841 3c10 	str.w	r3, [r1, #-16]
300053b8:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300053bc:	1c73      	adds	r3, r6, #1
300053be:	d1e5      	bne.n	3000538c <BOOT_RccConfig+0x40>
300053c0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300053c2:	9c04      	ldr	r4, [sp, #16]
300053c4:	af0d      	add	r7, sp, #52	; 0x34
300053c6:	ae05      	add	r6, sp, #20
300053c8:	4a13      	ldr	r2, [pc, #76]	; (30005418 <BOOT_RccConfig+0xcc>)
300053ca:	f85c 0b04 	ldr.w	r0, [ip], #4
300053ce:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300053d2:	f855 1b04 	ldr.w	r1, [r5], #4
300053d6:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300053da:	f850 8002 	ldr.w	r8, [r0, r2]
300053de:	45e6      	cmp	lr, ip
300053e0:	ea43 0308 	orr.w	r3, r3, r8
300053e4:	5083      	str	r3, [r0, r2]
300053e6:	588b      	ldr	r3, [r1, r2]
300053e8:	ea44 0403 	orr.w	r4, r4, r3
300053ec:	508c      	str	r4, [r1, r2]
300053ee:	d004      	beq.n	300053fa <BOOT_RccConfig+0xae>
300053f0:	f857 3b04 	ldr.w	r3, [r7], #4
300053f4:	f856 4b04 	ldr.w	r4, [r6], #4
300053f8:	e7e7      	b.n	300053ca <BOOT_RccConfig+0x7e>
300053fa:	200a      	movs	r0, #10
300053fc:	4b07      	ldr	r3, [pc, #28]	; (3000541c <BOOT_RccConfig+0xd0>)
300053fe:	4798      	blx	r3
30005400:	4b07      	ldr	r3, [pc, #28]	; (30005420 <BOOT_RccConfig+0xd4>)
30005402:	b010      	add	sp, #64	; 0x40
30005404:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30005408:	4718      	bx	r3
3000540a:	4623      	mov	r3, r4
3000540c:	e7da      	b.n	300053c4 <BOOT_RccConfig+0x78>
3000540e:	bf00      	nop
30005410:	3000e6d0 	.word	0x3000e6d0
30005414:	3000e9f4 	.word	0x3000e9f4
30005418:	42008000 	.word	0x42008000
3000541c:	00009b2d 	.word	0x00009b2d
30005420:	30007f01 	.word	0x30007f01

30005424 <BOOT_CACHEWRR_Set>:
30005424:	4b14      	ldr	r3, [pc, #80]	; (30005478 <BOOT_CACHEWRR_Set+0x54>)
30005426:	b430      	push	{r4, r5}
30005428:	681a      	ldr	r2, [r3, #0]
3000542a:	2a01      	cmp	r2, #1
3000542c:	d10f      	bne.n	3000544e <BOOT_CACHEWRR_Set+0x2a>
3000542e:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
30005432:	685d      	ldr	r5, [r3, #4]
30005434:	689c      	ldr	r4, [r3, #8]
30005436:	6882      	ldr	r2, [r0, #8]
30005438:	4910      	ldr	r1, [pc, #64]	; (3000547c <BOOT_CACHEWRR_Set+0x58>)
3000543a:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
3000543e:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30005442:	6082      	str	r2, [r0, #8]
30005444:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30005448:	68da      	ldr	r2, [r3, #12]
3000544a:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
3000544e:	691a      	ldr	r2, [r3, #16]
30005450:	2a01      	cmp	r2, #1
30005452:	d10f      	bne.n	30005474 <BOOT_CACHEWRR_Set+0x50>
30005454:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005458:	695d      	ldr	r5, [r3, #20]
3000545a:	4a08      	ldr	r2, [pc, #32]	; (3000547c <BOOT_CACHEWRR_Set+0x58>)
3000545c:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005460:	688b      	ldr	r3, [r1, #8]
30005462:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
30005466:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
3000546a:	608b      	str	r3, [r1, #8]
3000546c:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30005470:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
30005474:	bc30      	pop	{r4, r5}
30005476:	4770      	bx	lr
30005478:	3000f218 	.word	0x3000f218
3000547c:	e0042000 	.word	0xe0042000

30005480 <BOOT_TCMSet>:
30005480:	2300      	movs	r3, #0
30005482:	b5f0      	push	{r4, r5, r6, r7, lr}
30005484:	4db3      	ldr	r5, [pc, #716]	; (30005754 <BOOT_TCMSet+0x2d4>)
30005486:	461c      	mov	r4, r3
30005488:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
3000548c:	3307      	adds	r3, #7
3000548e:	4282      	cmp	r2, r0
30005490:	d03c      	beq.n	3000550c <BOOT_TCMSet+0x8c>
30005492:	3401      	adds	r4, #1
30005494:	2c07      	cmp	r4, #7
30005496:	d1f7      	bne.n	30005488 <BOOT_TCMSet+0x8>
30005498:	f3bf 8f4f 	dsb	sy
3000549c:	f3bf 8f6f 	isb	sy
300054a0:	48ad      	ldr	r0, [pc, #692]	; (30005758 <BOOT_TCMSet+0x2d8>)
300054a2:	2200      	movs	r2, #0
300054a4:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300054a8:	f3bf 8f4f 	dsb	sy
300054ac:	f3bf 8f6f 	isb	sy
300054b0:	6943      	ldr	r3, [r0, #20]
300054b2:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300054b6:	6143      	str	r3, [r0, #20]
300054b8:	f3bf 8f4f 	dsb	sy
300054bc:	f3bf 8f6f 	isb	sy
300054c0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300054c4:	f3bf 8f4f 	dsb	sy
300054c8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300054cc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300054d0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300054d4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300054d8:	0164      	lsls	r4, r4, #5
300054da:	ea04 0106 	and.w	r1, r4, r6
300054de:	462b      	mov	r3, r5
300054e0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300054e4:	3b01      	subs	r3, #1
300054e6:	1c5f      	adds	r7, r3, #1
300054e8:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300054ec:	d1f8      	bne.n	300054e0 <BOOT_TCMSet+0x60>
300054ee:	3c20      	subs	r4, #32
300054f0:	f114 0f20 	cmn.w	r4, #32
300054f4:	d1f1      	bne.n	300054da <BOOT_TCMSet+0x5a>
300054f6:	f3bf 8f4f 	dsb	sy
300054fa:	6943      	ldr	r3, [r0, #20]
300054fc:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005500:	6143      	str	r3, [r0, #20]
30005502:	f3bf 8f4f 	dsb	sy
30005506:	f3bf 8f6f 	isb	sy
3000550a:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000550c:	2c00      	cmp	r4, #0
3000550e:	d0c3      	beq.n	30005498 <BOOT_TCMSet+0x18>
30005510:	f3bf 8f4f 	dsb	sy
30005514:	f3bf 8f6f 	isb	sy
30005518:	4890      	ldr	r0, [pc, #576]	; (3000575c <BOOT_TCMSet+0x2dc>)
3000551a:	2300      	movs	r3, #0
3000551c:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30005520:	f3bf 8f4f 	dsb	sy
30005524:	f3bf 8f6f 	isb	sy
30005528:	6942      	ldr	r2, [r0, #20]
3000552a:	03d2      	lsls	r2, r2, #15
3000552c:	d520      	bpl.n	30005570 <BOOT_TCMSet+0xf0>
3000552e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005532:	f3bf 8f4f 	dsb	sy
30005536:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000553a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000553e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30005542:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005546:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000554a:	ea0c 0106 	and.w	r1, ip, r6
3000554e:	4673      	mov	r3, lr
30005550:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005554:	3b01      	subs	r3, #1
30005556:	1c5f      	adds	r7, r3, #1
30005558:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000555c:	d1f8      	bne.n	30005550 <BOOT_TCMSet+0xd0>
3000555e:	f1ac 0c20 	sub.w	ip, ip, #32
30005562:	f11c 0f20 	cmn.w	ip, #32
30005566:	d1f0      	bne.n	3000554a <BOOT_TCMSet+0xca>
30005568:	f3bf 8f4f 	dsb	sy
3000556c:	f3bf 8f6f 	isb	sy
30005570:	f3bf 8f4f 	dsb	sy
30005574:	f3bf 8f6f 	isb	sy
30005578:	4878      	ldr	r0, [pc, #480]	; (3000575c <BOOT_TCMSet+0x2dc>)
3000557a:	2200      	movs	r2, #0
3000557c:	6943      	ldr	r3, [r0, #20]
3000557e:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
30005582:	6143      	str	r3, [r0, #20]
30005584:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005588:	f3bf 8f4f 	dsb	sy
3000558c:	f3bf 8f6f 	isb	sy
30005590:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
30005594:	f3bf 8f4f 	dsb	sy
30005598:	6943      	ldr	r3, [r0, #20]
3000559a:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
3000559e:	6143      	str	r3, [r0, #20]
300055a0:	f3bf 8f4f 	dsb	sy
300055a4:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300055a8:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300055ac:	f3c3 06c9 	ubfx	r6, r3, #3, #10
300055b0:	f3c3 334e 	ubfx	r3, r3, #13, #15
300055b4:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300055b8:	ea0c 010e 	and.w	r1, ip, lr
300055bc:	4633      	mov	r3, r6
300055be:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300055c2:	3b01      	subs	r3, #1
300055c4:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300055c8:	1c5a      	adds	r2, r3, #1
300055ca:	d1f8      	bne.n	300055be <BOOT_TCMSet+0x13e>
300055cc:	f1ac 0c20 	sub.w	ip, ip, #32
300055d0:	f11c 0f20 	cmn.w	ip, #32
300055d4:	d1f0      	bne.n	300055b8 <BOOT_TCMSet+0x138>
300055d6:	f3bf 8f4f 	dsb	sy
300055da:	f3bf 8f6f 	isb	sy
300055de:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300055e2:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300055e6:	00e3      	lsls	r3, r4, #3
300055e8:	6b41      	ldr	r1, [r0, #52]	; 0x34
300055ea:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300055ee:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300055f2:	6341      	str	r1, [r0, #52]	; 0x34
300055f4:	6911      	ldr	r1, [r2, #16]
300055f6:	2903      	cmp	r1, #3
300055f8:	d00f      	beq.n	3000561a <BOOT_TCMSet+0x19a>
300055fa:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300055fe:	4858      	ldr	r0, [pc, #352]	; (30005760 <BOOT_TCMSet+0x2e0>)
30005600:	f04f 0c00 	mov.w	ip, #0
30005604:	4f57      	ldr	r7, [pc, #348]	; (30005764 <BOOT_TCMSet+0x2e4>)
30005606:	68b2      	ldr	r2, [r6, #8]
30005608:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
3000560c:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
30005610:	60b2      	str	r2, [r6, #8]
30005612:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
30005616:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
3000561a:	1b1a      	subs	r2, r3, r4
3000561c:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005620:	68d1      	ldr	r1, [r2, #12]
30005622:	2903      	cmp	r1, #3
30005624:	d00f      	beq.n	30005646 <BOOT_TCMSet+0x1c6>
30005626:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000562a:	484d      	ldr	r0, [pc, #308]	; (30005760 <BOOT_TCMSet+0x2e0>)
3000562c:	f04f 0c00 	mov.w	ip, #0
30005630:	4f4c      	ldr	r7, [pc, #304]	; (30005764 <BOOT_TCMSet+0x2e4>)
30005632:	68b2      	ldr	r2, [r6, #8]
30005634:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005638:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
3000563c:	60b2      	str	r2, [r6, #8]
3000563e:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
30005642:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
30005646:	1b1a      	subs	r2, r3, r4
30005648:	4945      	ldr	r1, [pc, #276]	; (30005760 <BOOT_TCMSet+0x2e0>)
3000564a:	eb05 0282 	add.w	r2, r5, r2, lsl #2
3000564e:	6950      	ldr	r0, [r2, #20]
30005650:	6608      	str	r0, [r1, #96]	; 0x60
30005652:	6990      	ldr	r0, [r2, #24]
30005654:	6648      	str	r0, [r1, #100]	; 0x64
30005656:	6852      	ldr	r2, [r2, #4]
30005658:	2a01      	cmp	r2, #1
3000565a:	d04b      	beq.n	300056f4 <BOOT_TCMSet+0x274>
3000565c:	1b1b      	subs	r3, r3, r4
3000565e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30005662:	68ab      	ldr	r3, [r5, #8]
30005664:	2b01      	cmp	r3, #1
30005666:	f47f af50 	bne.w	3000550a <BOOT_TCMSet+0x8a>
3000566a:	483b      	ldr	r0, [pc, #236]	; (30005758 <BOOT_TCMSet+0x2d8>)
3000566c:	2300      	movs	r3, #0
3000566e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005672:	f3bf 8f4f 	dsb	sy
30005676:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000567a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000567e:	f3c5 344e 	ubfx	r4, r5, #13, #15
30005682:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30005686:	0164      	lsls	r4, r4, #5
30005688:	ea04 0106 	and.w	r1, r4, r6
3000568c:	462b      	mov	r3, r5
3000568e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005692:	3b01      	subs	r3, #1
30005694:	1c5f      	adds	r7, r3, #1
30005696:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000569a:	d1f8      	bne.n	3000568e <BOOT_TCMSet+0x20e>
3000569c:	3c20      	subs	r4, #32
3000569e:	f114 0f20 	cmn.w	r4, #32
300056a2:	d1f1      	bne.n	30005688 <BOOT_TCMSet+0x208>
300056a4:	f3bf 8f4f 	dsb	sy
300056a8:	6943      	ldr	r3, [r0, #20]
300056aa:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300056ae:	6143      	str	r3, [r0, #20]
300056b0:	f3bf 8f4f 	dsb	sy
300056b4:	f3bf 8f6f 	isb	sy
300056b8:	4828      	ldr	r0, [pc, #160]	; (3000575c <BOOT_TCMSet+0x2dc>)
300056ba:	2300      	movs	r3, #0
300056bc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300056c0:	f3bf 8f4f 	dsb	sy
300056c4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300056c8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300056cc:	f3c5 344e 	ubfx	r4, r5, #13, #15
300056d0:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300056d4:	0164      	lsls	r4, r4, #5
300056d6:	ea04 0106 	and.w	r1, r4, r6
300056da:	462b      	mov	r3, r5
300056dc:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056e0:	3b01      	subs	r3, #1
300056e2:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300056e6:	1c5a      	adds	r2, r3, #1
300056e8:	d1f8      	bne.n	300056dc <BOOT_TCMSet+0x25c>
300056ea:	3c20      	subs	r4, #32
300056ec:	f114 0f20 	cmn.w	r4, #32
300056f0:	d1f1      	bne.n	300056d6 <BOOT_TCMSet+0x256>
300056f2:	e700      	b.n	300054f6 <BOOT_TCMSet+0x76>
300056f4:	f3bf 8f4f 	dsb	sy
300056f8:	f3bf 8f6f 	isb	sy
300056fc:	4a16      	ldr	r2, [pc, #88]	; (30005758 <BOOT_TCMSet+0x2d8>)
300056fe:	2000      	movs	r0, #0
30005700:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30005704:	f3bf 8f4f 	dsb	sy
30005708:	f3bf 8f6f 	isb	sy
3000570c:	6951      	ldr	r1, [r2, #20]
3000570e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30005712:	6151      	str	r1, [r2, #20]
30005714:	f3bf 8f4f 	dsb	sy
30005718:	f3bf 8f6f 	isb	sy
3000571c:	f3bf 8f4f 	dsb	sy
30005720:	f3bf 8f6f 	isb	sy
30005724:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30005728:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
3000572c:	f3bf 8f4f 	dsb	sy
30005730:	f3bf 8f6f 	isb	sy
30005734:	6951      	ldr	r1, [r2, #20]
30005736:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000573a:	6151      	str	r1, [r2, #20]
3000573c:	f3bf 8f4f 	dsb	sy
30005740:	f3bf 8f6f 	isb	sy
30005744:	1b1b      	subs	r3, r3, r4
30005746:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000574a:	68ab      	ldr	r3, [r5, #8]
3000574c:	2b01      	cmp	r3, #1
3000574e:	f47f aedc 	bne.w	3000550a <BOOT_TCMSet+0x8a>
30005752:	e78a      	b.n	3000566a <BOOT_TCMSet+0x1ea>
30005754:	3000f154 	.word	0x3000f154
30005758:	e002ed00 	.word	0xe002ed00
3000575c:	e000ed00 	.word	0xe000ed00
30005760:	e0042000 	.word	0xe0042000
30005764:	ffff0000 	.word	0xffff0000

30005768 <BOOT_GRstConfig>:
30005768:	b538      	push	{r3, r4, r5, lr}
3000576a:	4c09      	ldr	r4, [pc, #36]	; (30005790 <BOOT_GRstConfig+0x28>)
3000576c:	f241 050a 	movw	r5, #4106	; 0x100a
30005770:	2028      	movs	r0, #40	; 0x28
30005772:	4a08      	ldr	r2, [pc, #32]	; (30005794 <BOOT_GRstConfig+0x2c>)
30005774:	8823      	ldrh	r3, [r4, #0]
30005776:	b29b      	uxth	r3, r3
30005778:	431d      	orrs	r5, r3
3000577a:	8025      	strh	r5, [r4, #0]
3000577c:	4790      	blx	r2
3000577e:	88a3      	ldrh	r3, [r4, #4]
30005780:	b29b      	uxth	r3, r3
30005782:	ea23 0305 	bic.w	r3, r3, r5
30005786:	80a3      	strh	r3, [r4, #4]
30005788:	8863      	ldrh	r3, [r4, #2]
3000578a:	8065      	strh	r5, [r4, #2]
3000578c:	bd38      	pop	{r3, r4, r5, pc}
3000578e:	bf00      	nop
30005790:	42008200 	.word	0x42008200
30005794:	00009b2d 	.word	0x00009b2d

30005798 <BOOT_DDR_LCDC_HPR>:
30005798:	4a04      	ldr	r2, [pc, #16]	; (300057ac <BOOT_DDR_LCDC_HPR+0x14>)
3000579a:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
3000579e:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300057a2:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300057a6:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300057aa:	4770      	bx	lr
300057ac:	42008000 	.word	0x42008000

300057b0 <BOOT_DDR_Init>:
300057b0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300057b4:	b530      	push	{r4, r5, lr}
300057b6:	4d15      	ldr	r5, [pc, #84]	; (3000580c <BOOT_DDR_Init+0x5c>)
300057b8:	b099      	sub	sp, #100	; 0x64
300057ba:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300057bc:	47a8      	blx	r5
300057be:	2803      	cmp	r0, #3
300057c0:	d11e      	bne.n	30005800 <BOOT_DDR_Init+0x50>
300057c2:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300057c6:	4a12      	ldr	r2, [pc, #72]	; (30005810 <BOOT_DDR_Init+0x60>)
300057c8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300057cc:	4911      	ldr	r1, [pc, #68]	; (30005814 <BOOT_DDR_Init+0x64>)
300057ce:	a801      	add	r0, sp, #4
300057d0:	631c      	str	r4, [r3, #48]	; 0x30
300057d2:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300057d6:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300057da:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300057de:	4788      	blx	r1
300057e0:	a801      	add	r0, sp, #4
300057e2:	4b0d      	ldr	r3, [pc, #52]	; (30005818 <BOOT_DDR_Init+0x68>)
300057e4:	4798      	blx	r3
300057e6:	4b0d      	ldr	r3, [pc, #52]	; (3000581c <BOOT_DDR_Init+0x6c>)
300057e8:	4798      	blx	r3
300057ea:	4b0d      	ldr	r3, [pc, #52]	; (30005820 <BOOT_DDR_Init+0x70>)
300057ec:	4798      	blx	r3
300057ee:	4b0d      	ldr	r3, [pc, #52]	; (30005824 <BOOT_DDR_Init+0x74>)
300057f0:	4798      	blx	r3
300057f2:	2101      	movs	r1, #1
300057f4:	f240 10ff 	movw	r0, #511	; 0x1ff
300057f8:	4b0b      	ldr	r3, [pc, #44]	; (30005828 <BOOT_DDR_Init+0x78>)
300057fa:	4798      	blx	r3
300057fc:	b019      	add	sp, #100	; 0x64
300057fe:	bd30      	pop	{r4, r5, pc}
30005800:	47a8      	blx	r5
30005802:	2802      	cmp	r0, #2
30005804:	d0dd      	beq.n	300057c2 <BOOT_DDR_Init+0x12>
30005806:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
3000580a:	e7dc      	b.n	300057c6 <BOOT_DDR_Init+0x16>
3000580c:	0000c0f9 	.word	0x0000c0f9
30005810:	42008000 	.word	0x42008000
30005814:	3000801d 	.word	0x3000801d
30005818:	30008521 	.word	0x30008521
3000581c:	3000b571 	.word	0x3000b571
30005820:	300085f1 	.word	0x300085f1
30005824:	300085f5 	.word	0x300085f5
30005828:	3000b519 	.word	0x3000b519

3000582c <BOOT_PSRAM_Init>:
3000582c:	b500      	push	{lr}
3000582e:	b089      	sub	sp, #36	; 0x24
30005830:	4b1b      	ldr	r3, [pc, #108]	; (300058a0 <BOOT_PSRAM_Init+0x74>)
30005832:	a801      	add	r0, sp, #4
30005834:	4798      	blx	r3
30005836:	a801      	add	r0, sp, #4
30005838:	4b1a      	ldr	r3, [pc, #104]	; (300058a4 <BOOT_PSRAM_Init+0x78>)
3000583a:	4798      	blx	r3
3000583c:	4b1a      	ldr	r3, [pc, #104]	; (300058a8 <BOOT_PSRAM_Init+0x7c>)
3000583e:	4798      	blx	r3
30005840:	4b1a      	ldr	r3, [pc, #104]	; (300058ac <BOOT_PSRAM_Init+0x80>)
30005842:	4798      	blx	r3
30005844:	2801      	cmp	r0, #1
30005846:	d01d      	beq.n	30005884 <BOOT_PSRAM_Init+0x58>
30005848:	4b19      	ldr	r3, [pc, #100]	; (300058b0 <BOOT_PSRAM_Init+0x84>)
3000584a:	2004      	movs	r0, #4
3000584c:	4a19      	ldr	r2, [pc, #100]	; (300058b4 <BOOT_PSRAM_Init+0x88>)
3000584e:	4619      	mov	r1, r3
30005850:	f006 fc76 	bl	3000c140 <rtk_log_write>
30005854:	4b18      	ldr	r3, [pc, #96]	; (300058b8 <BOOT_PSRAM_Init+0x8c>)
30005856:	4798      	blx	r3
30005858:	4b18      	ldr	r3, [pc, #96]	; (300058bc <BOOT_PSRAM_Init+0x90>)
3000585a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000585e:	009b      	lsls	r3, r3, #2
30005860:	d517      	bpl.n	30005892 <BOOT_PSRAM_Init+0x66>
30005862:	4a17      	ldr	r2, [pc, #92]	; (300058c0 <BOOT_PSRAM_Init+0x94>)
30005864:	4b17      	ldr	r3, [pc, #92]	; (300058c4 <BOOT_PSRAM_Init+0x98>)
30005866:	6812      	ldr	r2, [r2, #0]
30005868:	605a      	str	r2, [r3, #4]
3000586a:	4a17      	ldr	r2, [pc, #92]	; (300058c8 <BOOT_PSRAM_Init+0x9c>)
3000586c:	f04f 0c20 	mov.w	ip, #32
30005870:	2101      	movs	r1, #1
30005872:	4b16      	ldr	r3, [pc, #88]	; (300058cc <BOOT_PSRAM_Init+0xa0>)
30005874:	6952      	ldr	r2, [r2, #20]
30005876:	4608      	mov	r0, r1
30005878:	fbbc f2f2 	udiv	r2, ip, r2
3000587c:	4798      	blx	r3
3000587e:	b009      	add	sp, #36	; 0x24
30005880:	f85d fb04 	ldr.w	pc, [sp], #4
30005884:	4b12      	ldr	r3, [pc, #72]	; (300058d0 <BOOT_PSRAM_Init+0xa4>)
30005886:	4798      	blx	r3
30005888:	4b0c      	ldr	r3, [pc, #48]	; (300058bc <BOOT_PSRAM_Init+0x90>)
3000588a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000588e:	009b      	lsls	r3, r3, #2
30005890:	d4e7      	bmi.n	30005862 <BOOT_PSRAM_Init+0x36>
30005892:	4b10      	ldr	r3, [pc, #64]	; (300058d4 <BOOT_PSRAM_Init+0xa8>)
30005894:	4798      	blx	r3
30005896:	4a0b      	ldr	r2, [pc, #44]	; (300058c4 <BOOT_PSRAM_Init+0x98>)
30005898:	4b09      	ldr	r3, [pc, #36]	; (300058c0 <BOOT_PSRAM_Init+0x94>)
3000589a:	6852      	ldr	r2, [r2, #4]
3000589c:	601a      	str	r2, [r3, #0]
3000589e:	e7e4      	b.n	3000586a <BOOT_PSRAM_Init+0x3e>
300058a0:	30008fb9 	.word	0x30008fb9
300058a4:	30008fd9 	.word	0x30008fd9
300058a8:	30009041 	.word	0x30009041
300058ac:	30007eb5 	.word	0x30007eb5
300058b0:	3000c974 	.word	0x3000c974
300058b4:	3000cd24 	.word	0x3000cd24
300058b8:	300092e5 	.word	0x300092e5
300058bc:	42008000 	.word	0x42008000
300058c0:	300103d4 	.word	0x300103d4
300058c4:	41002000 	.word	0x41002000
300058c8:	3000ff70 	.word	0x3000ff70
300058cc:	300095c5 	.word	0x300095c5
300058d0:	3000920d 	.word	0x3000920d
300058d4:	30009321 	.word	0x30009321

300058d8 <BOOT_WakeFromPG>:
300058d8:	b538      	push	{r3, r4, r5, lr}
300058da:	4c49      	ldr	r4, [pc, #292]	; (30005a00 <BOOT_WakeFromPG+0x128>)
300058dc:	f7ff f918 	bl	30004b10 <BOOT_RAM_TZCfg>
300058e0:	69a3      	ldr	r3, [r4, #24]
300058e2:	b10b      	cbz	r3, 300058e8 <BOOT_WakeFromPG+0x10>
300058e4:	4a47      	ldr	r2, [pc, #284]	; (30005a04 <BOOT_WakeFromPG+0x12c>)
300058e6:	6093      	str	r3, [r2, #8]
300058e8:	4b47      	ldr	r3, [pc, #284]	; (30005a08 <BOOT_WakeFromPG+0x130>)
300058ea:	f06f 0c1a 	mvn.w	ip, #26
300058ee:	4847      	ldr	r0, [pc, #284]	; (30005a0c <BOOT_WakeFromPG+0x134>)
300058f0:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300058f4:	ebac 0c04 	sub.w	ip, ip, r4
300058f8:	eb0c 0203 	add.w	r2, ip, r3
300058fc:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005900:	4402      	add	r2, r0
30005902:	4573      	cmp	r3, lr
30005904:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005908:	d1f6      	bne.n	300058f8 <BOOT_WakeFromPG+0x20>
3000590a:	6822      	ldr	r2, [r4, #0]
3000590c:	4b3d      	ldr	r3, [pc, #244]	; (30005a04 <BOOT_WakeFromPG+0x12c>)
3000590e:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005912:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005916:	6002      	str	r2, [r0, #0]
30005918:	6045      	str	r5, [r0, #4]
3000591a:	6081      	str	r1, [r0, #8]
3000591c:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000591e:	4a3c      	ldr	r2, [pc, #240]	; (30005a10 <BOOT_WakeFromPG+0x138>)
30005920:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005924:	4d3b      	ldr	r5, [pc, #236]	; (30005a14 <BOOT_WakeFromPG+0x13c>)
30005926:	6259      	str	r1, [r3, #36]	; 0x24
30005928:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000592a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
3000592e:	6251      	str	r1, [r2, #36]	; 0x24
30005930:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005934:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005938:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000593c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005940:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005944:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005948:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000594c:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005950:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005954:	f7ff fd66 	bl	30005424 <BOOT_CACHEWRR_Set>
30005958:	4b2f      	ldr	r3, [pc, #188]	; (30005a18 <BOOT_WakeFromPG+0x140>)
3000595a:	6818      	ldr	r0, [r3, #0]
3000595c:	f7ff fd90 	bl	30005480 <BOOT_TCMSet>
30005960:	2201      	movs	r2, #1
30005962:	492e      	ldr	r1, [pc, #184]	; (30005a1c <BOOT_WakeFromPG+0x144>)
30005964:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005968:	47a8      	blx	r5
3000596a:	4b2d      	ldr	r3, [pc, #180]	; (30005a20 <BOOT_WakeFromPG+0x148>)
3000596c:	4798      	blx	r3
3000596e:	4b2d      	ldr	r3, [pc, #180]	; (30005a24 <BOOT_WakeFromPG+0x14c>)
30005970:	4798      	blx	r3
30005972:	2801      	cmp	r0, #1
30005974:	4604      	mov	r4, r0
30005976:	d026      	beq.n	300059c6 <BOOT_WakeFromPG+0xee>
30005978:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000597c:	2201      	movs	r2, #1
3000597e:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005982:	47a8      	blx	r5
30005984:	4a28      	ldr	r2, [pc, #160]	; (30005a28 <BOOT_WakeFromPG+0x150>)
30005986:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000598a:	f043 0302 	orr.w	r3, r3, #2
3000598e:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005992:	f7ff ff4b 	bl	3000582c <BOOT_PSRAM_Init>
30005996:	2096      	movs	r0, #150	; 0x96
30005998:	4b24      	ldr	r3, [pc, #144]	; (30005a2c <BOOT_WakeFromPG+0x154>)
3000599a:	4798      	blx	r3
3000599c:	4a24      	ldr	r2, [pc, #144]	; (30005a30 <BOOT_WakeFromPG+0x158>)
3000599e:	4c1c      	ldr	r4, [pc, #112]	; (30005a10 <BOOT_WakeFromPG+0x138>)
300059a0:	6893      	ldr	r3, [r2, #8]
300059a2:	4924      	ldr	r1, [pc, #144]	; (30005a34 <BOOT_WakeFromPG+0x15c>)
300059a4:	6852      	ldr	r2, [r2, #4]
300059a6:	4824      	ldr	r0, [pc, #144]	; (30005a38 <BOOT_WakeFromPG+0x160>)
300059a8:	605a      	str	r2, [r3, #4]
300059aa:	60a3      	str	r3, [r4, #8]
300059ac:	4a23      	ldr	r2, [pc, #140]	; (30005a3c <BOOT_WakeFromPG+0x164>)
300059ae:	61c8      	str	r0, [r1, #28]
300059b0:	f382 8888 	msr	MSP_NS, r2
300059b4:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
300059b8:	f382 8809 	msr	PSP, r2
300059bc:	6858      	ldr	r0, [r3, #4]
300059be:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300059c2:	f7ff bc73 	b.w	300052ac <BOOT_NsStart>
300059c6:	4b1e      	ldr	r3, [pc, #120]	; (30005a40 <BOOT_WakeFromPG+0x168>)
300059c8:	2004      	movs	r0, #4
300059ca:	4a1e      	ldr	r2, [pc, #120]	; (30005a44 <BOOT_WakeFromPG+0x16c>)
300059cc:	4619      	mov	r1, r3
300059ce:	f006 fbb7 	bl	3000c140 <rtk_log_write>
300059d2:	4622      	mov	r2, r4
300059d4:	491c      	ldr	r1, [pc, #112]	; (30005a48 <BOOT_WakeFromPG+0x170>)
300059d6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300059da:	47a8      	blx	r5
300059dc:	4622      	mov	r2, r4
300059de:	491b      	ldr	r1, [pc, #108]	; (30005a4c <BOOT_WakeFromPG+0x174>)
300059e0:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300059e4:	47a8      	blx	r5
300059e6:	f7ff fee3 	bl	300057b0 <BOOT_DDR_Init>
300059ea:	4a0f      	ldr	r2, [pc, #60]	; (30005a28 <BOOT_WakeFromPG+0x150>)
300059ec:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300059f0:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300059f4:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300059f8:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300059fc:	e7ce      	b.n	3000599c <BOOT_WakeFromPG+0xc4>
300059fe:	bf00      	nop
30005a00:	30010368 	.word	0x30010368
30005a04:	e000ed00 	.word	0xe000ed00
30005a08:	30010383 	.word	0x30010383
30005a0c:	e000e100 	.word	0xe000e100
30005a10:	e002ed00 	.word	0xe002ed00
30005a14:	0000b479 	.word	0x0000b479
30005a18:	3000e9ec 	.word	0x3000e9ec
30005a1c:	40001000 	.word	0x40001000
30005a20:	0000d835 	.word	0x0000d835
30005a24:	30007ddd 	.word	0x30007ddd
30005a28:	42008000 	.word	0x42008000
30005a2c:	00009b2d 	.word	0x00009b2d
30005a30:	60000020 	.word	0x60000020
30005a34:	30000000 	.word	0x30000000
30005a38:	30004cad 	.word	0x30004cad
30005a3c:	2001bffc 	.word	0x2001bffc
30005a40:	3000c974 	.word	0x3000c974
30005a44:	3000cd38 	.word	0x3000cd38
30005a48:	40080000 	.word	0x40080000
30005a4c:	40040000 	.word	0x40040000

30005a50 <BOOT_SCBConfig_HP>:
30005a50:	4b0e      	ldr	r3, [pc, #56]	; (30005a8c <BOOT_SCBConfig_HP+0x3c>)
30005a52:	4a0f      	ldr	r2, [pc, #60]	; (30005a90 <BOOT_SCBConfig_HP+0x40>)
30005a54:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005a56:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005a5a:	6259      	str	r1, [r3, #36]	; 0x24
30005a5c:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005a5e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005a62:	6251      	str	r1, [r2, #36]	; 0x24
30005a64:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005a68:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a6c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005a70:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005a74:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a78:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005a7c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005a80:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005a84:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005a88:	4770      	bx	lr
30005a8a:	bf00      	nop
30005a8c:	e000ed00 	.word	0xe000ed00
30005a90:	e002ed00 	.word	0xe002ed00

30005a94 <BOOT_SCBVTORBackup_HP>:
30005a94:	4a02      	ldr	r2, [pc, #8]	; (30005aa0 <BOOT_SCBVTORBackup_HP+0xc>)
30005a96:	4b03      	ldr	r3, [pc, #12]	; (30005aa4 <BOOT_SCBVTORBackup_HP+0x10>)
30005a98:	6892      	ldr	r2, [r2, #8]
30005a9a:	619a      	str	r2, [r3, #24]
30005a9c:	4770      	bx	lr
30005a9e:	bf00      	nop
30005aa0:	e000ed00 	.word	0xe000ed00
30005aa4:	30010368 	.word	0x30010368

30005aa8 <BOOT_SCBVTORReFill_HP>:
30005aa8:	4a02      	ldr	r2, [pc, #8]	; (30005ab4 <BOOT_SCBVTORReFill_HP+0xc>)
30005aaa:	4b03      	ldr	r3, [pc, #12]	; (30005ab8 <BOOT_SCBVTORReFill_HP+0x10>)
30005aac:	6992      	ldr	r2, [r2, #24]
30005aae:	609a      	str	r2, [r3, #8]
30005ab0:	4770      	bx	lr
30005ab2:	bf00      	nop
30005ab4:	30010368 	.word	0x30010368
30005ab8:	e000ed00 	.word	0xe000ed00

30005abc <BOOT_NVICBackup_HP>:
30005abc:	491a      	ldr	r1, [pc, #104]	; (30005b28 <BOOT_NVICBackup_HP+0x6c>)
30005abe:	f06f 001a 	mvn.w	r0, #26
30005ac2:	b410      	push	{r4}
30005ac4:	4c19      	ldr	r4, [pc, #100]	; (30005b2c <BOOT_NVICBackup_HP+0x70>)
30005ac6:	680a      	ldr	r2, [r1, #0]
30005ac8:	f104 031b 	add.w	r3, r4, #27
30005acc:	1b00      	subs	r0, r0, r4
30005ace:	6022      	str	r2, [r4, #0]
30005ad0:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005ad4:	684a      	ldr	r2, [r1, #4]
30005ad6:	6062      	str	r2, [r4, #4]
30005ad8:	688a      	ldr	r2, [r1, #8]
30005ada:	60a2      	str	r2, [r4, #8]
30005adc:	18c2      	adds	r2, r0, r3
30005ade:	440a      	add	r2, r1
30005ae0:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005ae4:	f803 2f01 	strb.w	r2, [r3, #1]!
30005ae8:	4563      	cmp	r3, ip
30005aea:	d1f7      	bne.n	30005adc <BOOT_NVICBackup_HP+0x20>
30005aec:	4b10      	ldr	r3, [pc, #64]	; (30005b30 <BOOT_NVICBackup_HP+0x74>)
30005aee:	695b      	ldr	r3, [r3, #20]
30005af0:	03db      	lsls	r3, r3, #15
30005af2:	d512      	bpl.n	30005b1a <BOOT_NVICBackup_HP+0x5e>
30005af4:	f014 011f 	ands.w	r1, r4, #31
30005af8:	4b0c      	ldr	r3, [pc, #48]	; (30005b2c <BOOT_NVICBackup_HP+0x70>)
30005afa:	d111      	bne.n	30005b20 <BOOT_NVICBackup_HP+0x64>
30005afc:	216c      	movs	r1, #108	; 0x6c
30005afe:	f3bf 8f4f 	dsb	sy
30005b02:	480b      	ldr	r0, [pc, #44]	; (30005b30 <BOOT_NVICBackup_HP+0x74>)
30005b04:	4419      	add	r1, r3
30005b06:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005b0a:	3320      	adds	r3, #32
30005b0c:	1aca      	subs	r2, r1, r3
30005b0e:	2a00      	cmp	r2, #0
30005b10:	dcf9      	bgt.n	30005b06 <BOOT_NVICBackup_HP+0x4a>
30005b12:	f3bf 8f4f 	dsb	sy
30005b16:	f3bf 8f6f 	isb	sy
30005b1a:	f85d 4b04 	ldr.w	r4, [sp], #4
30005b1e:	4770      	bx	lr
30005b20:	f024 031f 	bic.w	r3, r4, #31
30005b24:	316c      	adds	r1, #108	; 0x6c
30005b26:	e7ea      	b.n	30005afe <BOOT_NVICBackup_HP+0x42>
30005b28:	e000e100 	.word	0xe000e100
30005b2c:	30010368 	.word	0x30010368
30005b30:	e000ed00 	.word	0xe000ed00

30005b34 <BOOT_NVICReFill_HP>:
30005b34:	b510      	push	{r4, lr}
30005b36:	f06f 0c1a 	mvn.w	ip, #26
30005b3a:	4c0d      	ldr	r4, [pc, #52]	; (30005b70 <BOOT_NVICReFill_HP+0x3c>)
30005b3c:	480d      	ldr	r0, [pc, #52]	; (30005b74 <BOOT_NVICReFill_HP+0x40>)
30005b3e:	f104 031b 	add.w	r3, r4, #27
30005b42:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005b46:	ebac 0c04 	sub.w	ip, ip, r4
30005b4a:	eb0c 0203 	add.w	r2, ip, r3
30005b4e:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005b52:	4402      	add	r2, r0
30005b54:	4573      	cmp	r3, lr
30005b56:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005b5a:	d1f6      	bne.n	30005b4a <BOOT_NVICReFill_HP+0x16>
30005b5c:	e9d4 3100 	ldrd	r3, r1, [r4]
30005b60:	68a2      	ldr	r2, [r4, #8]
30005b62:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005b66:	6003      	str	r3, [r0, #0]
30005b68:	6041      	str	r1, [r0, #4]
30005b6a:	6082      	str	r2, [r0, #8]
30005b6c:	bd10      	pop	{r4, pc}
30005b6e:	bf00      	nop
30005b70:	30010368 	.word	0x30010368
30005b74:	e000e100 	.word	0xe000e100

30005b78 <BOOT_ImgCopy>:
30005b78:	b510      	push	{r4, lr}
30005b7a:	b082      	sub	sp, #8
30005b7c:	4b09      	ldr	r3, [pc, #36]	; (30005ba4 <BOOT_ImgCopy+0x2c>)
30005b7e:	4604      	mov	r4, r0
30005b80:	e9cd 2100 	strd	r2, r1, [sp]
30005b84:	4798      	blx	r3
30005b86:	e9dd 2100 	ldrd	r2, r1, [sp]
30005b8a:	b128      	cbz	r0, 30005b98 <BOOT_ImgCopy+0x20>
30005b8c:	4620      	mov	r0, r4
30005b8e:	4b06      	ldr	r3, [pc, #24]	; (30005ba8 <BOOT_ImgCopy+0x30>)
30005b90:	b002      	add	sp, #8
30005b92:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005b96:	4718      	bx	r3
30005b98:	4620      	mov	r0, r4
30005b9a:	b002      	add	sp, #8
30005b9c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005ba0:	f002 be8e 	b.w	300088c0 <NandImgCopy>
30005ba4:	30009e65 	.word	0x30009e65
30005ba8:	00012a1d 	.word	0x00012a1d

30005bac <BOOT_LoadImages>:
30005bac:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005bb0:	4b57      	ldr	r3, [pc, #348]	; (30005d10 <BOOT_LoadImages+0x164>)
30005bb2:	2700      	movs	r7, #0
30005bb4:	2104      	movs	r1, #4
30005bb6:	2601      	movs	r6, #1
30005bb8:	ed2d 8b02 	vpush	{d8}
30005bbc:	b0b7      	sub	sp, #220	; 0xdc
30005bbe:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005bc2:	4b54      	ldr	r3, [pc, #336]	; (30005d14 <BOOT_LoadImages+0x168>)
30005bc4:	aa11      	add	r2, sp, #68	; 0x44
30005bc6:	9703      	str	r7, [sp, #12]
30005bc8:	9705      	str	r7, [sp, #20]
30005bca:	4798      	blx	r3
30005bcc:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30005bd0:	409e      	lsls	r6, r3
30005bd2:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
30005bd6:	f000 823a 	beq.w	3000604e <BOOT_LoadImages+0x4a2>
30005bda:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
30005bde:	f000 8375 	beq.w	300062cc <BOOT_LoadImages+0x720>
30005be2:	4c4d      	ldr	r4, [pc, #308]	; (30005d18 <BOOT_LoadImages+0x16c>)
30005be4:	f04f 0800 	mov.w	r8, #0
30005be8:	4d4c      	ldr	r5, [pc, #304]	; (30005d1c <BOOT_LoadImages+0x170>)
30005bea:	eb06 0904 	add.w	r9, r6, r4
30005bee:	484c      	ldr	r0, [pc, #304]	; (30005d20 <BOOT_LoadImages+0x174>)
30005bf0:	4435      	add	r5, r6
30005bf2:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
30005bf6:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
30005bfa:	f7ff f9ff 	bl	30004ffc <mcc_init_crc32>
30005bfe:	f8d9 3004 	ldr.w	r3, [r9, #4]
30005c02:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005c06:	f899 200c 	ldrb.w	r2, [r9, #12]
30005c0a:	9308      	str	r3, [sp, #32]
30005c0c:	3b01      	subs	r3, #1
30005c0e:	9206      	str	r2, [sp, #24]
30005c10:	3303      	adds	r3, #3
30005c12:	686a      	ldr	r2, [r5, #4]
30005c14:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005c18:	9204      	str	r2, [sp, #16]
30005c1a:	f200 81bc 	bhi.w	30005f96 <BOOT_LoadImages+0x3ea>
30005c1e:	f8d9 2008 	ldr.w	r2, [r9, #8]
30005c22:	9b04      	ldr	r3, [sp, #16]
30005c24:	3a01      	subs	r2, #1
30005c26:	3b01      	subs	r3, #1
30005c28:	3203      	adds	r2, #3
30005c2a:	d868      	bhi.n	30005cfe <BOOT_LoadImages+0x152>
30005c2c:	9a06      	ldr	r2, [sp, #24]
30005c2e:	9307      	str	r3, [sp, #28]
30005c30:	2a01      	cmp	r2, #1
30005c32:	f240 8305 	bls.w	30006240 <BOOT_LoadImages+0x694>
30005c36:	483b      	ldr	r0, [pc, #236]	; (30005d24 <BOOT_LoadImages+0x178>)
30005c38:	f006 fd26 	bl	3000c688 <__DiagPrintf_veneer>
30005c3c:	9b07      	ldr	r3, [sp, #28]
30005c3e:	3303      	adds	r3, #3
30005c40:	d864      	bhi.n	30005d0c <BOOT_LoadImages+0x160>
30005c42:	f10a 3aff 	add.w	sl, sl, #4294967295
30005c46:	f11a 0f03 	cmn.w	sl, #3
30005c4a:	f200 81aa 	bhi.w	30005fa2 <BOOT_LoadImages+0x3f6>
30005c4e:	f1bb 0f01 	cmp.w	fp, #1
30005c52:	f200 8347 	bhi.w	300062e4 <BOOT_LoadImages+0x738>
30005c56:	9904      	ldr	r1, [sp, #16]
30005c58:	4833      	ldr	r0, [pc, #204]	; (30005d28 <BOOT_LoadImages+0x17c>)
30005c5a:	f006 fd15 	bl	3000c688 <__DiagPrintf_veneer>
30005c5e:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005c62:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005c66:	4c2e      	ldr	r4, [pc, #184]	; (30005d20 <BOOT_LoadImages+0x174>)
30005c68:	9307      	str	r3, [sp, #28]
30005c6a:	2300      	movs	r3, #0
30005c6c:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005c70:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005c74:	9309      	str	r3, [sp, #36]	; 0x24
30005c76:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005d40 <BOOT_LoadImages+0x194>
30005c7a:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30005c7e:	6829      	ldr	r1, [r5, #0]
30005c80:	f04f 30ff 	mov.w	r0, #4294967295
30005c84:	44b4      	add	ip, r6
30005c86:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005c8a:	4042      	eors	r2, r0
30005c8c:	459c      	cmp	ip, r3
30005c8e:	b2d2      	uxtb	r2, r2
30005c90:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005c94:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005c98:	d1f5      	bne.n	30005c86 <BOOT_LoadImages+0xda>
30005c9a:	43c2      	mvns	r2, r0
30005c9c:	930f      	str	r3, [sp, #60]	; 0x3c
30005c9e:	4291      	cmp	r1, r2
30005ca0:	f000 8301 	beq.w	300062a6 <BOOT_LoadImages+0x6fa>
30005ca4:	4821      	ldr	r0, [pc, #132]	; (30005d2c <BOOT_LoadImages+0x180>)
30005ca6:	f006 fcef 	bl	3000c688 <__DiagPrintf_veneer>
30005caa:	2200      	movs	r2, #0
30005cac:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005cae:	9909      	ldr	r1, [sp, #36]	; 0x24
30005cb0:	2900      	cmp	r1, #0
30005cb2:	d04e      	beq.n	30005d52 <BOOT_LoadImages+0x1a6>
30005cb4:	f899 100c 	ldrb.w	r1, [r9, #12]
30005cb8:	4553      	cmp	r3, sl
30005cba:	9106      	str	r1, [sp, #24]
30005cbc:	d342      	bcc.n	30005d44 <BOOT_LoadImages+0x198>
30005cbe:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005cc2:	45e2      	cmp	sl, ip
30005cc4:	d93e      	bls.n	30005d44 <BOOT_LoadImages+0x198>
30005cc6:	9807      	ldr	r0, [sp, #28]
30005cc8:	4560      	cmp	r0, ip
30005cca:	f240 8203 	bls.w	300060d4 <BOOT_LoadImages+0x528>
30005cce:	4283      	cmp	r3, r0
30005cd0:	f0c0 8200 	bcc.w	300060d4 <BOOT_LoadImages+0x528>
30005cd4:	2a00      	cmp	r2, #0
30005cd6:	f000 820d 	beq.w	300060f4 <BOOT_LoadImages+0x548>
30005cda:	455b      	cmp	r3, fp
30005cdc:	d301      	bcc.n	30005ce2 <BOOT_LoadImages+0x136>
30005cde:	45e3      	cmp	fp, ip
30005ce0:	d845      	bhi.n	30005d6e <BOOT_LoadImages+0x1c2>
30005ce2:	4813      	ldr	r0, [pc, #76]	; (30005d30 <BOOT_LoadImages+0x184>)
30005ce4:	f006 fcd0 	bl	3000c688 <__DiagPrintf_veneer>
30005ce8:	9f06      	ldr	r7, [sp, #24]
30005cea:	4812      	ldr	r0, [pc, #72]	; (30005d34 <BOOT_LoadImages+0x188>)
30005cec:	464d      	mov	r5, r9
30005cee:	f006 fccb 	bl	3000c688 <__DiagPrintf_veneer>
30005cf2:	9b07      	ldr	r3, [sp, #28]
30005cf4:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005cf8:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005cfc:	e049      	b.n	30005d92 <BOOT_LoadImages+0x1e6>
30005cfe:	480e      	ldr	r0, [pc, #56]	; (30005d38 <BOOT_LoadImages+0x18c>)
30005d00:	9307      	str	r3, [sp, #28]
30005d02:	f006 fcc1 	bl	3000c688 <__DiagPrintf_veneer>
30005d06:	9b07      	ldr	r3, [sp, #28]
30005d08:	3303      	adds	r3, #3
30005d0a:	d99a      	bls.n	30005c42 <BOOT_LoadImages+0x96>
30005d0c:	480b      	ldr	r0, [pc, #44]	; (30005d3c <BOOT_LoadImages+0x190>)
30005d0e:	e149      	b.n	30005fa4 <BOOT_LoadImages+0x3f8>
30005d10:	2001c01c 	.word	0x2001c01c
30005d14:	00009db5 	.word	0x00009db5
30005d18:	07ffe000 	.word	0x07ffe000
30005d1c:	07fff000 	.word	0x07fff000
30005d20:	3000ebb0 	.word	0x3000ebb0
30005d24:	3000cd9c 	.word	0x3000cd9c
30005d28:	3000d184 	.word	0x3000d184
30005d2c:	3000cea0 	.word	0x3000cea0
30005d30:	3000d168 	.word	0x3000d168
30005d34:	3000cf2c 	.word	0x3000cf2c
30005d38:	3000cd88 	.word	0x3000cd88
30005d3c:	3000d134 	.word	0x3000d134
30005d40:	07fff004 	.word	0x07fff004
30005d44:	48c4      	ldr	r0, [pc, #784]	; (30006058 <BOOT_LoadImages+0x4ac>)
30005d46:	930f      	str	r3, [sp, #60]	; 0x3c
30005d48:	9209      	str	r2, [sp, #36]	; 0x24
30005d4a:	f006 fc9d 	bl	3000c688 <__DiagPrintf_veneer>
30005d4e:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005d50:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005d52:	2a00      	cmp	r2, #0
30005d54:	f000 8131 	beq.w	30005fba <BOOT_LoadImages+0x40e>
30005d58:	455b      	cmp	r3, fp
30005d5a:	f0c0 82c5 	bcc.w	300062e8 <BOOT_LoadImages+0x73c>
30005d5e:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005d62:	bf2c      	ite	cs
30005d64:	2200      	movcs	r2, #0
30005d66:	2201      	movcc	r2, #1
30005d68:	2a00      	cmp	r2, #0
30005d6a:	f040 82bd 	bne.w	300062e8 <BOOT_LoadImages+0x73c>
30005d6e:	4598      	cmp	r8, r3
30005d70:	f200 81b8 	bhi.w	300060e4 <BOOT_LoadImages+0x538>
30005d74:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30005d78:	f0c0 81b4 	bcc.w	300060e4 <BOOT_LoadImages+0x538>
30005d7c:	7b2f      	ldrb	r7, [r5, #12]
30005d7e:	2a00      	cmp	r2, #0
30005d80:	f040 81de 	bne.w	30006140 <BOOT_LoadImages+0x594>
30005d84:	48b5      	ldr	r0, [pc, #724]	; (3000605c <BOOT_LoadImages+0x4b0>)
30005d86:	f006 fc7f 	bl	3000c688 <__DiagPrintf_veneer>
30005d8a:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005d8e:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30005d92:	4639      	mov	r1, r7
30005d94:	48b2      	ldr	r0, [pc, #712]	; (30006060 <BOOT_LoadImages+0x4b4>)
30005d96:	f006 fc77 	bl	3000c688 <__DiagPrintf_veneer>
30005d9a:	4bb2      	ldr	r3, [pc, #712]	; (30006064 <BOOT_LoadImages+0x4b8>)
30005d9c:	4798      	blx	r3
30005d9e:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005da2:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005da6:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005daa:	6809      	ldr	r1, [r1, #0]
30005dac:	330c      	adds	r3, #12
30005dae:	910b      	str	r1, [sp, #44]	; 0x2c
30005db0:	42b3      	cmp	r3, r6
30005db2:	bf28      	it	cs
30005db4:	4633      	movcs	r3, r6
30005db6:	930d      	str	r3, [sp, #52]	; 0x34
30005db8:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005dbc:	6812      	ldr	r2, [r2, #0]
30005dbe:	330c      	adds	r3, #12
30005dc0:	920a      	str	r2, [sp, #40]	; 0x28
30005dc2:	429e      	cmp	r6, r3
30005dc4:	bf28      	it	cs
30005dc6:	461e      	movcs	r6, r3
30005dc8:	960c      	str	r6, [sp, #48]	; 0x30
30005dca:	2800      	cmp	r0, #0
30005dcc:	f040 8194 	bne.w	300060f8 <BOOT_LoadImages+0x54c>
30005dd0:	4ba5      	ldr	r3, [pc, #660]	; (30006068 <BOOT_LoadImages+0x4bc>)
30005dd2:	4606      	mov	r6, r0
30005dd4:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30006094 <BOOT_LoadImages+0x4e8>
30005dd8:	4680      	mov	r8, r0
30005dda:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 30006098 <BOOT_LoadImages+0x4ec>
30005dde:	4682      	mov	sl, r0
30005de0:	e9cd 0303 	strd	r0, r3, [sp, #12]
30005de4:	2f00      	cmp	r7, #0
30005de6:	f040 81c9 	bne.w	3000617c <BOOT_LoadImages+0x5d0>
30005dea:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005dee:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005df0:	f10c 0104 	add.w	r1, ip, #4
30005df4:	2b00      	cmp	r3, #0
30005df6:	f000 83d4 	beq.w	300065a2 <BOOT_LoadImages+0x9f6>
30005dfa:	1d18      	adds	r0, r3, #4
30005dfc:	f04f 32ff 	mov.w	r2, #4294967295
30005e00:	4460      	add	r0, ip
30005e02:	f811 3b01 	ldrb.w	r3, [r1], #1
30005e06:	4053      	eors	r3, r2
30005e08:	4281      	cmp	r1, r0
30005e0a:	b2db      	uxtb	r3, r3
30005e0c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005e10:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005e14:	d1f5      	bne.n	30005e02 <BOOT_LoadImages+0x256>
30005e16:	43d2      	mvns	r2, r2
30005e18:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005e1a:	4293      	cmp	r3, r2
30005e1c:	f000 838d 	beq.w	3000653a <BOOT_LoadImages+0x98e>
30005e20:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005e24:	f088 0301 	eor.w	r3, r8, #1
30005e28:	3101      	adds	r1, #1
30005e2a:	f003 0301 	and.w	r3, r3, #1
30005e2e:	ea46 0603 	orr.w	r6, r6, r3
30005e32:	f000 8345 	beq.w	300064c0 <BOOT_LoadImages+0x914>
30005e36:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005e38:	488c      	ldr	r0, [pc, #560]	; (3000606c <BOOT_LoadImages+0x4c0>)
30005e3a:	f006 fc25 	bl	3000c688 <__DiagPrintf_veneer>
30005e3e:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005e42:	2002      	movs	r0, #2
30005e44:	f7ff f858 	bl	30004ef8 <FLASH_Erase_With_Lock>
30005e48:	2e00      	cmp	r6, #0
30005e4a:	f040 831f 	bne.w	3000648c <BOOT_LoadImages+0x8e0>
30005e4e:	9605      	str	r6, [sp, #20]
30005e50:	f08a 0601 	eor.w	r6, sl, #1
30005e54:	f04f 0801 	mov.w	r8, #1
30005e58:	f006 0601 	and.w	r6, r6, #1
30005e5c:	9b03      	ldr	r3, [sp, #12]
30005e5e:	2b01      	cmp	r3, #1
30005e60:	f000 8238 	beq.w	300062d4 <BOOT_LoadImages+0x728>
30005e64:	9b03      	ldr	r3, [sp, #12]
30005e66:	2b02      	cmp	r3, #2
30005e68:	f000 83a9 	beq.w	300065be <BOOT_LoadImages+0xa12>
30005e6c:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30005e6e:	4650      	mov	r0, sl
30005e70:	f8c9 3000 	str.w	r3, [r9]
30005e74:	47d8      	blx	fp
30005e76:	1e03      	subs	r3, r0, #0
30005e78:	bf18      	it	ne
30005e7a:	2301      	movne	r3, #1
30005e7c:	2800      	cmp	r0, #0
30005e7e:	f000 8156 	beq.w	3000612e <BOOT_LoadImages+0x582>
30005e82:	2e00      	cmp	r6, #0
30005e84:	f000 8153 	beq.w	3000612e <BOOT_LoadImages+0x582>
30005e88:	fab7 f787 	clz	r7, r7
30005e8c:	9b05      	ldr	r3, [sp, #20]
30005e8e:	097f      	lsrs	r7, r7, #5
30005e90:	2b00      	cmp	r3, #0
30005e92:	f000 83e1 	beq.w	30006658 <BOOT_LoadImages+0xaac>
30005e96:	469a      	mov	sl, r3
30005e98:	4c75      	ldr	r4, [pc, #468]	; (30006070 <BOOT_LoadImages+0x4c4>)
30005e9a:	2f00      	cmp	r7, #0
30005e9c:	f040 8138 	bne.w	30006110 <BOOT_LoadImages+0x564>
30005ea0:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005ea4:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005ea8:	3201      	adds	r2, #1
30005eaa:	f000 82c5 	beq.w	30006438 <BOOT_LoadImages+0x88c>
30005eae:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005eb2:	4870      	ldr	r0, [pc, #448]	; (30006074 <BOOT_LoadImages+0x4c8>)
30005eb4:	f006 fbe8 	bl	3000c688 <__DiagPrintf_veneer>
30005eb8:	2301      	movs	r3, #1
30005eba:	9303      	str	r3, [sp, #12]
30005ebc:	f04f 0801 	mov.w	r8, #1
30005ec0:	4646      	mov	r6, r8
30005ec2:	f8cd 8014 	str.w	r8, [sp, #20]
30005ec6:	f1ba 0f00 	cmp.w	sl, #0
30005eca:	d0c7      	beq.n	30005e5c <BOOT_LoadImages+0x2b0>
30005ecc:	ab16      	add	r3, sp, #88	; 0x58
30005ece:	2280      	movs	r2, #128	; 0x80
30005ed0:	21ff      	movs	r1, #255	; 0xff
30005ed2:	46d0      	mov	r8, sl
30005ed4:	461e      	mov	r6, r3
30005ed6:	4618      	mov	r0, r3
30005ed8:	ee08 3a10 	vmov	s16, r3
30005edc:	f006 fbac 	bl	3000c638 <____wrap_memset_veneer>
30005ee0:	2280      	movs	r2, #128	; 0x80
30005ee2:	4629      	mov	r1, r5
30005ee4:	4630      	mov	r0, r6
30005ee6:	f006 fb8f 	bl	3000c608 <____wrap_memcpy_veneer>
30005eea:	2f01      	cmp	r7, #1
30005eec:	f000 831e 	beq.w	3000652c <BOOT_LoadImages+0x980>
30005ef0:	2100      	movs	r1, #0
30005ef2:	f8cd a014 	str.w	sl, [sp, #20]
30005ef6:	f04f 0a01 	mov.w	sl, #1
30005efa:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30005efe:	1d2e      	adds	r6, r5, #4
30005f00:	485d      	ldr	r0, [pc, #372]	; (30006078 <BOOT_LoadImages+0x4cc>)
30005f02:	f006 fbc1 	bl	3000c688 <__DiagPrintf_veneer>
30005f06:	4629      	mov	r1, r5
30005f08:	2002      	movs	r0, #2
30005f0a:	f7fe fff5 	bl	30004ef8 <FLASH_Erase_With_Lock>
30005f0e:	aa17      	add	r2, sp, #92	; 0x5c
30005f10:	217c      	movs	r1, #124	; 0x7c
30005f12:	4630      	mov	r0, r6
30005f14:	f7ff f818 	bl	30004f48 <FLASH_TxData_With_Lock>
30005f18:	4630      	mov	r0, r6
30005f1a:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30005f1e:	f04f 32ff 	mov.w	r2, #4294967295
30005f22:	f810 3b01 	ldrb.w	r3, [r0], #1
30005f26:	4053      	eors	r3, r2
30005f28:	4288      	cmp	r0, r1
30005f2a:	b2db      	uxtb	r3, r3
30005f2c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005f30:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005f34:	d1f5      	bne.n	30005f22 <BOOT_LoadImages+0x376>
30005f36:	43d2      	mvns	r2, r2
30005f38:	4629      	mov	r1, r5
30005f3a:	2002      	movs	r0, #2
30005f3c:	9216      	str	r2, [sp, #88]	; 0x58
30005f3e:	f7fe ffdb 	bl	30004ef8 <FLASH_Erase_With_Lock>
30005f42:	ee18 2a10 	vmov	r2, s16
30005f46:	2180      	movs	r1, #128	; 0x80
30005f48:	4628      	mov	r0, r5
30005f4a:	f7fe fffd 	bl	30004f48 <FLASH_TxData_With_Lock>
30005f4e:	9b05      	ldr	r3, [sp, #20]
30005f50:	2b00      	cmp	r3, #0
30005f52:	f000 8143 	beq.w	300061dc <BOOT_LoadImages+0x630>
30005f56:	f1b8 0f00 	cmp.w	r8, #0
30005f5a:	f040 8376 	bne.w	3000664a <BOOT_LoadImages+0xa9e>
30005f5e:	2f01      	cmp	r7, #1
30005f60:	f000 8280 	beq.w	30006464 <BOOT_LoadImages+0x8b8>
30005f64:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005f68:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005f6c:	3201      	adds	r2, #1
30005f6e:	f000 81d0 	beq.w	30006312 <BOOT_LoadImages+0x766>
30005f72:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005f76:	4841      	ldr	r0, [pc, #260]	; (3000607c <BOOT_LoadImages+0x4d0>)
30005f78:	f006 fb86 	bl	3000c688 <__DiagPrintf_veneer>
30005f7c:	f08a 0601 	eor.w	r6, sl, #1
30005f80:	f04f 0801 	mov.w	r8, #1
30005f84:	f006 0601 	and.w	r6, r6, #1
30005f88:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005f8c:	3310      	adds	r3, #16
30005f8e:	930e      	str	r3, [sp, #56]	; 0x38
30005f90:	2302      	movs	r3, #2
30005f92:	9303      	str	r3, [sp, #12]
30005f94:	e76a      	b.n	30005e6c <BOOT_LoadImages+0x2c0>
30005f96:	483a      	ldr	r0, [pc, #232]	; (30006080 <BOOT_LoadImages+0x4d4>)
30005f98:	f006 fb76 	bl	3000c688 <__DiagPrintf_veneer>
30005f9c:	9b04      	ldr	r3, [sp, #16]
30005f9e:	3b01      	subs	r3, #1
30005fa0:	e64d      	b.n	30005c3e <BOOT_LoadImages+0x92>
30005fa2:	4838      	ldr	r0, [pc, #224]	; (30006084 <BOOT_LoadImages+0x4d8>)
30005fa4:	f006 fb70 	bl	3000c688 <__DiagPrintf_veneer>
30005fa8:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005fac:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005fb0:	9307      	str	r3, [sp, #28]
30005fb2:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005fb6:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005fba:	4833      	ldr	r0, [pc, #204]	; (30006088 <BOOT_LoadImages+0x4dc>)
30005fbc:	f006 fb64 	bl	3000c688 <__DiagPrintf_veneer>
30005fc0:	4832      	ldr	r0, [pc, #200]	; (3000608c <BOOT_LoadImages+0x4e0>)
30005fc2:	f006 fb61 	bl	3000c688 <__DiagPrintf_veneer>
30005fc6:	4631      	mov	r1, r6
30005fc8:	aa12      	add	r2, sp, #72	; 0x48
30005fca:	4650      	mov	r0, sl
30005fcc:	f7ff f85c 	bl	30005088 <recovery_check>
30005fd0:	4631      	mov	r1, r6
30005fd2:	4604      	mov	r4, r0
30005fd4:	aa13      	add	r2, sp, #76	; 0x4c
30005fd6:	9807      	ldr	r0, [sp, #28]
30005fd8:	f7ff f856 	bl	30005088 <recovery_check>
30005fdc:	4631      	mov	r1, r6
30005fde:	aa14      	add	r2, sp, #80	; 0x50
30005fe0:	9004      	str	r0, [sp, #16]
30005fe2:	4658      	mov	r0, fp
30005fe4:	f7ff f850 	bl	30005088 <recovery_check>
30005fe8:	4631      	mov	r1, r6
30005fea:	aa15      	add	r2, sp, #84	; 0x54
30005fec:	4606      	mov	r6, r0
30005fee:	4640      	mov	r0, r8
30005ff0:	f7ff f84a 	bl	30005088 <recovery_check>
30005ff4:	9b04      	ldr	r3, [sp, #16]
30005ff6:	4306      	orrs	r6, r0
30005ff8:	431c      	orrs	r4, r3
30005ffa:	d04f      	beq.n	3000609c <BOOT_LoadImages+0x4f0>
30005ffc:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30006000:	2e00      	cmp	r6, #0
30006002:	f000 818a 	beq.w	3000631a <BOOT_LoadImages+0x76e>
30006006:	4293      	cmp	r3, r2
30006008:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
3000600c:	f0c0 8273 	bcc.w	300064f6 <BOOT_LoadImages+0x94a>
30006010:	428b      	cmp	r3, r1
30006012:	d302      	bcc.n	3000601a <BOOT_LoadImages+0x46e>
30006014:	4283      	cmp	r3, r0
30006016:	f080 82fb 	bcs.w	30006610 <BOOT_LoadImages+0xa64>
3000601a:	4293      	cmp	r3, r2
3000601c:	f000 826b 	beq.w	300064f6 <BOOT_LoadImages+0x94a>
30006020:	4293      	cmp	r3, r2
30006022:	bf38      	it	cc
30006024:	4613      	movcc	r3, r2
30006026:	428b      	cmp	r3, r1
30006028:	f240 82dc 	bls.w	300065e4 <BOOT_LoadImages+0xa38>
3000602c:	428b      	cmp	r3, r1
3000602e:	bf38      	it	cc
30006030:	460b      	movcc	r3, r1
30006032:	4283      	cmp	r3, r0
30006034:	f240 82e5 	bls.w	30006602 <BOOT_LoadImages+0xa56>
30006038:	f04f 0800 	mov.w	r8, #0
3000603c:	4b0a      	ldr	r3, [pc, #40]	; (30006068 <BOOT_LoadImages+0x4bc>)
3000603e:	2601      	movs	r6, #1
30006040:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30006094 <BOOT_LoadImages+0x4e8>
30006044:	f8df b050 	ldr.w	fp, [pc, #80]	; 30006098 <BOOT_LoadImages+0x4ec>
30006048:	46c2      	mov	sl, r8
3000604a:	9304      	str	r3, [sp, #16]
3000604c:	e70e      	b.n	30005e6c <BOOT_LoadImages+0x2c0>
3000604e:	4810      	ldr	r0, [pc, #64]	; (30006090 <BOOT_LoadImages+0x4e4>)
30006050:	f006 fb1a 	bl	3000c688 <__DiagPrintf_veneer>
30006054:	e5c5      	b.n	30005be2 <BOOT_LoadImages+0x36>
30006056:	bf00      	nop
30006058:	3000cee0 	.word	0x3000cee0
3000605c:	3000cf3c 	.word	0x3000cf3c
30006060:	3000cf70 	.word	0x3000cf70
30006064:	0000c149 	.word	0x0000c149
30006068:	3000e9eb 	.word	0x3000e9eb
3000606c:	3000cfe0 	.word	0x3000cfe0
30006070:	3000ebb0 	.word	0x3000ebb0
30006074:	3000ccd4 	.word	0x3000ccd4
30006078:	3000d0b4 	.word	0x3000d0b4
3000607c:	3000ccec 	.word	0x3000ccec
30006080:	3000cd70 	.word	0x3000cd70
30006084:	3000cdd4 	.word	0x3000cdd4
30006088:	3000cf4c 	.word	0x3000cf4c
3000608c:	3000cf60 	.word	0x3000cf60
30006090:	3000cd4c 	.word	0x3000cd4c
30006094:	30010364 	.word	0x30010364
30006098:	30004221 	.word	0x30004221
3000609c:	2e00      	cmp	r6, #0
3000609e:	f000 827c 	beq.w	3000659a <BOOT_LoadImages+0x9ee>
300060a2:	4659      	mov	r1, fp
300060a4:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
300060a8:	e9cd 5400 	strd	r5, r4, [sp]
300060ac:	429a      	cmp	r2, r3
300060ae:	464b      	mov	r3, r9
300060b0:	4642      	mov	r2, r8
300060b2:	f080 821e 	bcs.w	300064f2 <BOOT_LoadImages+0x946>
300060b6:	2001      	movs	r0, #1
300060b8:	f7ff f816 	bl	300050e8 <rewrite_bp>
300060bc:	46a0      	mov	r8, r4
300060be:	b2c3      	uxtb	r3, r0
300060c0:	2601      	movs	r6, #1
300060c2:	46a2      	mov	sl, r4
300060c4:	f8df 9318 	ldr.w	r9, [pc, #792]	; 300063e0 <BOOT_LoadImages+0x834>
300060c8:	9303      	str	r3, [sp, #12]
300060ca:	4bb0      	ldr	r3, [pc, #704]	; (3000638c <BOOT_LoadImages+0x7e0>)
300060cc:	f8df b314 	ldr.w	fp, [pc, #788]	; 300063e4 <BOOT_LoadImages+0x838>
300060d0:	9304      	str	r3, [sp, #16]
300060d2:	e6c3      	b.n	30005e5c <BOOT_LoadImages+0x2b0>
300060d4:	48ae      	ldr	r0, [pc, #696]	; (30006390 <BOOT_LoadImages+0x7e4>)
300060d6:	930f      	str	r3, [sp, #60]	; 0x3c
300060d8:	9209      	str	r2, [sp, #36]	; 0x24
300060da:	f006 fad5 	bl	3000c688 <__DiagPrintf_veneer>
300060de:	9a09      	ldr	r2, [sp, #36]	; 0x24
300060e0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300060e2:	e636      	b.n	30005d52 <BOOT_LoadImages+0x1a6>
300060e4:	48ab      	ldr	r0, [pc, #684]	; (30006394 <BOOT_LoadImages+0x7e8>)
300060e6:	9204      	str	r2, [sp, #16]
300060e8:	f006 face 	bl	3000c688 <__DiagPrintf_veneer>
300060ec:	9a04      	ldr	r2, [sp, #16]
300060ee:	2a00      	cmp	r2, #0
300060f0:	f43f af63 	beq.w	30005fba <BOOT_LoadImages+0x40e>
300060f4:	9f06      	ldr	r7, [sp, #24]
300060f6:	e5f8      	b.n	30005cea <BOOT_LoadImages+0x13e>
300060f8:	2300      	movs	r3, #0
300060fa:	4aa4      	ldr	r2, [pc, #656]	; (3000638c <BOOT_LoadImages+0x7e0>)
300060fc:	f8df 92e0 	ldr.w	r9, [pc, #736]	; 300063e0 <BOOT_LoadImages+0x834>
30006100:	f8df b2e0 	ldr.w	fp, [pc, #736]	; 300063e4 <BOOT_LoadImages+0x838>
30006104:	469a      	mov	sl, r3
30006106:	9303      	str	r3, [sp, #12]
30006108:	9204      	str	r2, [sp, #16]
3000610a:	2f00      	cmp	r7, #0
3000610c:	f43f aec8 	beq.w	30005ea0 <BOOT_LoadImages+0x2f4>
30006110:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006114:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006118:	3201      	adds	r2, #1
3000611a:	f000 8118 	beq.w	3000634e <BOOT_LoadImages+0x7a2>
3000611e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006122:	489d      	ldr	r0, [pc, #628]	; (30006398 <BOOT_LoadImages+0x7ec>)
30006124:	f006 fab0 	bl	3000c688 <__DiagPrintf_veneer>
30006128:	2302      	movs	r3, #2
3000612a:	9303      	str	r3, [sp, #12]
3000612c:	e6c6      	b.n	30005ebc <BOOT_LoadImages+0x310>
3000612e:	f00a 0401 	and.w	r4, sl, #1
30006132:	b1c3      	cbz	r3, 30006166 <BOOT_LoadImages+0x5ba>
30006134:	b1bc      	cbz	r4, 30006166 <BOOT_LoadImages+0x5ba>
30006136:	4c99      	ldr	r4, [pc, #612]	; (3000639c <BOOT_LoadImages+0x7f0>)
30006138:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000613c:	47a0      	blx	r4
3000613e:	e7fb      	b.n	30006138 <BOOT_LoadImages+0x58c>
30006140:	4897      	ldr	r0, [pc, #604]	; (300063a0 <BOOT_LoadImages+0x7f4>)
30006142:	f006 faa1 	bl	3000c688 <__DiagPrintf_veneer>
30006146:	9b08      	ldr	r3, [sp, #32]
30006148:	9a04      	ldr	r2, [sp, #16]
3000614a:	4293      	cmp	r3, r2
3000614c:	f4ff ae1a 	bcc.w	30005d84 <BOOT_LoadImages+0x1d8>
30006150:	4894      	ldr	r0, [pc, #592]	; (300063a4 <BOOT_LoadImages+0x7f8>)
30006152:	464d      	mov	r5, r9
30006154:	f006 fa98 	bl	3000c688 <__DiagPrintf_veneer>
30006158:	9b07      	ldr	r3, [sp, #28]
3000615a:	9f06      	ldr	r7, [sp, #24]
3000615c:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30006160:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30006164:	e615      	b.n	30005d92 <BOOT_LoadImages+0x1e6>
30006166:	9b04      	ldr	r3, [sp, #16]
30006168:	781b      	ldrb	r3, [r3, #0]
3000616a:	2b00      	cmp	r3, #0
3000616c:	f040 80c0 	bne.w	300062f0 <BOOT_LoadImages+0x744>
30006170:	2001      	movs	r0, #1
30006172:	b037      	add	sp, #220	; 0xdc
30006174:	ecbd 8b02 	vpop	{d8}
30006178:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000617c:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30006180:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006182:	f10c 0104 	add.w	r1, ip, #4
30006186:	2b00      	cmp	r3, #0
30006188:	f000 820d 	beq.w	300065a6 <BOOT_LoadImages+0x9fa>
3000618c:	1d18      	adds	r0, r3, #4
3000618e:	f04f 32ff 	mov.w	r2, #4294967295
30006192:	4460      	add	r0, ip
30006194:	f811 3b01 	ldrb.w	r3, [r1], #1
30006198:	4053      	eors	r3, r2
3000619a:	4281      	cmp	r1, r0
3000619c:	b2db      	uxtb	r3, r3
3000619e:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300061a2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300061a6:	d1f5      	bne.n	30006194 <BOOT_LoadImages+0x5e8>
300061a8:	43d2      	mvns	r2, r2
300061aa:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300061ac:	4293      	cmp	r3, r2
300061ae:	f000 81d4 	beq.w	3000655a <BOOT_LoadImages+0x9ae>
300061b2:	f8dc 100a 	ldr.w	r1, [ip, #10]
300061b6:	f088 0301 	eor.w	r3, r8, #1
300061ba:	3101      	adds	r1, #1
300061bc:	f003 0301 	and.w	r3, r3, #1
300061c0:	ea46 0603 	orr.w	r6, r6, r3
300061c4:	f000 8178 	beq.w	300064b8 <BOOT_LoadImages+0x90c>
300061c8:	990a      	ldr	r1, [sp, #40]	; 0x28
300061ca:	4877      	ldr	r0, [pc, #476]	; (300063a8 <BOOT_LoadImages+0x7fc>)
300061cc:	f006 fa5c 	bl	3000c688 <__DiagPrintf_veneer>
300061d0:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300061d4:	2002      	movs	r0, #2
300061d6:	f7fe fe8f 	bl	30004ef8 <FLASH_Erase_With_Lock>
300061da:	e635      	b.n	30005e48 <BOOT_LoadImages+0x29c>
300061dc:	f1b8 0f00 	cmp.w	r8, #0
300061e0:	f040 822e 	bne.w	30006640 <BOOT_LoadImages+0xa94>
300061e4:	2f01      	cmp	r7, #1
300061e6:	f000 80ff 	beq.w	300063e8 <BOOT_LoadImages+0x83c>
300061ea:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
300061ee:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300061f0:	1d31      	adds	r1, r6, #4
300061f2:	2b00      	cmp	r3, #0
300061f4:	f000 81e5 	beq.w	300065c2 <BOOT_LoadImages+0xa16>
300061f8:	1d18      	adds	r0, r3, #4
300061fa:	f04f 32ff 	mov.w	r2, #4294967295
300061fe:	4430      	add	r0, r6
30006200:	f811 3b01 	ldrb.w	r3, [r1], #1
30006204:	4053      	eors	r3, r2
30006206:	4281      	cmp	r1, r0
30006208:	b2db      	uxtb	r3, r3
3000620a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000620e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006212:	d1f5      	bne.n	30006200 <BOOT_LoadImages+0x654>
30006214:	43d2      	mvns	r2, r2
30006216:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006218:	4293      	cmp	r3, r2
3000621a:	f000 81af 	beq.w	3000657c <BOOT_LoadImages+0x9d0>
3000621e:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006222:	3301      	adds	r3, #1
30006224:	d075      	beq.n	30006312 <BOOT_LoadImages+0x766>
30006226:	990a      	ldr	r1, [sp, #40]	; 0x28
30006228:	485f      	ldr	r0, [pc, #380]	; (300063a8 <BOOT_LoadImages+0x7fc>)
3000622a:	f006 fa2d 	bl	3000c688 <__DiagPrintf_veneer>
3000622e:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006232:	2002      	movs	r0, #2
30006234:	f7fe fe60 	bl	30004ef8 <FLASH_Erase_With_Lock>
30006238:	485c      	ldr	r0, [pc, #368]	; (300063ac <BOOT_LoadImages+0x800>)
3000623a:	f006 fa25 	bl	3000c688 <__DiagPrintf_veneer>
3000623e:	e7fe      	b.n	3000623e <BOOT_LoadImages+0x692>
30006240:	9908      	ldr	r1, [sp, #32]
30006242:	485b      	ldr	r0, [pc, #364]	; (300063b0 <BOOT_LoadImages+0x804>)
30006244:	f006 fa20 	bl	3000c688 <__DiagPrintf_veneer>
30006248:	9b07      	ldr	r3, [sp, #28]
3000624a:	3303      	adds	r3, #3
3000624c:	f240 81ad 	bls.w	300065aa <BOOT_LoadImages+0x9fe>
30006250:	4858      	ldr	r0, [pc, #352]	; (300063b4 <BOOT_LoadImages+0x808>)
30006252:	f006 fa19 	bl	3000c688 <__DiagPrintf_veneer>
30006256:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
3000625a:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000625e:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006262:	9307      	str	r3, [sp, #28]
30006264:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30006268:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000626c:	4852      	ldr	r0, [pc, #328]	; (300063b8 <BOOT_LoadImages+0x80c>)
3000626e:	f04f 32ff 	mov.w	r2, #4294967295
30006272:	5931      	ldr	r1, [r6, r4]
30006274:	4430      	add	r0, r6
30006276:	4c51      	ldr	r4, [pc, #324]	; (300063bc <BOOT_LoadImages+0x810>)
30006278:	f810 3b01 	ldrb.w	r3, [r0], #1
3000627c:	4053      	eors	r3, r2
3000627e:	4285      	cmp	r5, r0
30006280:	b2db      	uxtb	r3, r3
30006282:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006286:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000628a:	d1f5      	bne.n	30006278 <BOOT_LoadImages+0x6cc>
3000628c:	43d2      	mvns	r2, r2
3000628e:	4291      	cmp	r1, r2
30006290:	d010      	beq.n	300062b4 <BOOT_LoadImages+0x708>
30006292:	484b      	ldr	r0, [pc, #300]	; (300063c0 <BOOT_LoadImages+0x814>)
30006294:	f006 f9f8 	bl	3000c688 <__DiagPrintf_veneer>
30006298:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000629a:	2b00      	cmp	r3, #0
3000629c:	f43f ae8d 	beq.w	30005fba <BOOT_LoadImages+0x40e>
300062a0:	2300      	movs	r3, #0
300062a2:	9309      	str	r3, [sp, #36]	; 0x24
300062a4:	e4e7      	b.n	30005c76 <BOOT_LoadImages+0xca>
300062a6:	460a      	mov	r2, r1
300062a8:	4846      	ldr	r0, [pc, #280]	; (300063c4 <BOOT_LoadImages+0x818>)
300062aa:	f006 f9ed 	bl	3000c688 <__DiagPrintf_veneer>
300062ae:	2201      	movs	r2, #1
300062b0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300062b2:	e4fc      	b.n	30005cae <BOOT_LoadImages+0x102>
300062b4:	460a      	mov	r2, r1
300062b6:	4844      	ldr	r0, [pc, #272]	; (300063c8 <BOOT_LoadImages+0x81c>)
300062b8:	f006 f9e6 	bl	3000c688 <__DiagPrintf_veneer>
300062bc:	9b09      	ldr	r3, [sp, #36]	; 0x24
300062be:	2b00      	cmp	r3, #0
300062c0:	f47f acd9 	bne.w	30005c76 <BOOT_LoadImages+0xca>
300062c4:	461a      	mov	r2, r3
300062c6:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300062ca:	e4f3      	b.n	30005cb4 <BOOT_LoadImages+0x108>
300062cc:	483f      	ldr	r0, [pc, #252]	; (300063cc <BOOT_LoadImages+0x820>)
300062ce:	f006 f9db 	bl	3000c688 <__DiagPrintf_veneer>
300062d2:	e486      	b.n	30005be2 <BOOT_LoadImages+0x36>
300062d4:	4c39      	ldr	r4, [pc, #228]	; (300063bc <BOOT_LoadImages+0x810>)
300062d6:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300062da:	3310      	adds	r3, #16
300062dc:	930e      	str	r3, [sp, #56]	; 0x38
300062de:	2301      	movs	r3, #1
300062e0:	9303      	str	r3, [sp, #12]
300062e2:	e5c3      	b.n	30005e6c <BOOT_LoadImages+0x2c0>
300062e4:	483a      	ldr	r0, [pc, #232]	; (300063d0 <BOOT_LoadImages+0x824>)
300062e6:	e65d      	b.n	30005fa4 <BOOT_LoadImages+0x3f8>
300062e8:	483a      	ldr	r0, [pc, #232]	; (300063d4 <BOOT_LoadImages+0x828>)
300062ea:	f006 f9cd 	bl	3000c688 <__DiagPrintf_veneer>
300062ee:	e664      	b.n	30005fba <BOOT_LoadImages+0x40e>
300062f0:	4b39      	ldr	r3, [pc, #228]	; (300063d8 <BOOT_LoadImages+0x82c>)
300062f2:	4650      	mov	r0, sl
300062f4:	4798      	blx	r3
300062f6:	1e03      	subs	r3, r0, #0
300062f8:	bf18      	it	ne
300062fa:	2301      	movne	r3, #1
300062fc:	b110      	cbz	r0, 30006304 <BOOT_LoadImages+0x758>
300062fe:	2e00      	cmp	r6, #0
30006300:	f47f adc2 	bne.w	30005e88 <BOOT_LoadImages+0x2dc>
30006304:	2c00      	cmp	r4, #0
30006306:	f43f af33 	beq.w	30006170 <BOOT_LoadImages+0x5c4>
3000630a:	2b00      	cmp	r3, #0
3000630c:	f47f af13 	bne.w	30006136 <BOOT_LoadImages+0x58a>
30006310:	e72e      	b.n	30006170 <BOOT_LoadImages+0x5c4>
30006312:	4832      	ldr	r0, [pc, #200]	; (300063dc <BOOT_LoadImages+0x830>)
30006314:	f006 f9b8 	bl	3000c688 <__DiagPrintf_veneer>
30006318:	e78e      	b.n	30006238 <BOOT_LoadImages+0x68c>
3000631a:	4293      	cmp	r3, r2
3000631c:	f04f 0401 	mov.w	r4, #1
30006320:	464b      	mov	r3, r9
30006322:	4651      	mov	r1, sl
30006324:	bf28      	it	cs
30006326:	4630      	movcs	r0, r6
30006328:	9a07      	ldr	r2, [sp, #28]
3000632a:	bf38      	it	cc
3000632c:	4620      	movcc	r0, r4
3000632e:	9500      	str	r5, [sp, #0]
30006330:	9401      	str	r4, [sp, #4]
30006332:	46b0      	mov	r8, r6
30006334:	f7fe fed8 	bl	300050e8 <rewrite_bp>
30006338:	4626      	mov	r6, r4
3000633a:	b2c3      	uxtb	r3, r0
3000633c:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 300063e0 <BOOT_LoadImages+0x834>
30006340:	46c2      	mov	sl, r8
30006342:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 300063e4 <BOOT_LoadImages+0x838>
30006346:	9303      	str	r3, [sp, #12]
30006348:	4b10      	ldr	r3, [pc, #64]	; (3000638c <BOOT_LoadImages+0x7e0>)
3000634a:	9304      	str	r3, [sp, #16]
3000634c:	e586      	b.n	30005e5c <BOOT_LoadImages+0x2b0>
3000634e:	ab16      	add	r3, sp, #88	; 0x58
30006350:	4822      	ldr	r0, [pc, #136]	; (300063dc <BOOT_LoadImages+0x830>)
30006352:	461e      	mov	r6, r3
30006354:	ee08 3a10 	vmov	s16, r3
30006358:	f006 f996 	bl	3000c688 <__DiagPrintf_veneer>
3000635c:	2280      	movs	r2, #128	; 0x80
3000635e:	21ff      	movs	r1, #255	; 0xff
30006360:	4630      	mov	r0, r6
30006362:	f006 f969 	bl	3000c638 <____wrap_memset_veneer>
30006366:	2280      	movs	r2, #128	; 0x80
30006368:	4629      	mov	r1, r5
3000636a:	4630      	mov	r0, r6
3000636c:	f006 f94c 	bl	3000c608 <____wrap_memcpy_veneer>
30006370:	f1ba 0f00 	cmp.w	sl, #0
30006374:	f040 80a8 	bne.w	300064c8 <BOOT_LoadImages+0x91c>
30006378:	2301      	movs	r3, #1
3000637a:	9305      	str	r3, [sp, #20]
3000637c:	46d0      	mov	r8, sl
3000637e:	f04f 0a00 	mov.w	sl, #0
30006382:	4651      	mov	r1, sl
30006384:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
30006388:	e5b9      	b.n	30005efe <BOOT_LoadImages+0x352>
3000638a:	bf00      	nop
3000638c:	3000e9eb 	.word	0x3000e9eb
30006390:	3000cefc 	.word	0x3000cefc
30006394:	3000d118 	.word	0x3000d118
30006398:	3000ccec 	.word	0x3000ccec
3000639c:	00009be5 	.word	0x00009be5
300063a0:	3000cf18 	.word	0x3000cf18
300063a4:	3000cf2c 	.word	0x3000cf2c
300063a8:	3000d068 	.word	0x3000d068
300063ac:	3000cd04 	.word	0x3000cd04
300063b0:	3000cdb8 	.word	0x3000cdb8
300063b4:	3000d134 	.word	0x3000d134
300063b8:	07ffe004 	.word	0x07ffe004
300063bc:	3000ebb0 	.word	0x3000ebb0
300063c0:	3000ce24 	.word	0x3000ce24
300063c4:	3000ce64 	.word	0x3000ce64
300063c8:	3000cde8 	.word	0x3000cde8
300063cc:	3000cd5c 	.word	0x3000cd5c
300063d0:	3000d14c 	.word	0x3000d14c
300063d4:	3000d168 	.word	0x3000d168
300063d8:	30004225 	.word	0x30004225
300063dc:	3000cf94 	.word	0x3000cf94
300063e0:	30010364 	.word	0x30010364
300063e4:	30004221 	.word	0x30004221
300063e8:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
300063ec:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300063ee:	1d31      	adds	r1, r6, #4
300063f0:	2b00      	cmp	r3, #0
300063f2:	f000 8102 	beq.w	300065fa <BOOT_LoadImages+0xa4e>
300063f6:	1d18      	adds	r0, r3, #4
300063f8:	f04f 32ff 	mov.w	r2, #4294967295
300063fc:	4430      	add	r0, r6
300063fe:	f811 3b01 	ldrb.w	r3, [r1], #1
30006402:	4053      	eors	r3, r2
30006404:	4281      	cmp	r1, r0
30006406:	b2db      	uxtb	r3, r3
30006408:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000640c:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006410:	d1f5      	bne.n	300063fe <BOOT_LoadImages+0x852>
30006412:	43d2      	mvns	r2, r2
30006414:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30006416:	4293      	cmp	r3, r2
30006418:	f000 80d5 	beq.w	300065c6 <BOOT_LoadImages+0xa1a>
3000641c:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006420:	3301      	adds	r3, #1
30006422:	d062      	beq.n	300064ea <BOOT_LoadImages+0x93e>
30006424:	990b      	ldr	r1, [sp, #44]	; 0x2c
30006426:	4890      	ldr	r0, [pc, #576]	; (30006668 <BOOT_LoadImages+0xabc>)
30006428:	f006 f92e 	bl	3000c688 <__DiagPrintf_veneer>
3000642c:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30006430:	2002      	movs	r0, #2
30006432:	f7fe fd61 	bl	30004ef8 <FLASH_Erase_With_Lock>
30006436:	e6ff      	b.n	30006238 <BOOT_LoadImages+0x68c>
30006438:	ab16      	add	r3, sp, #88	; 0x58
3000643a:	488c      	ldr	r0, [pc, #560]	; (3000666c <BOOT_LoadImages+0xac0>)
3000643c:	461e      	mov	r6, r3
3000643e:	ee08 3a10 	vmov	s16, r3
30006442:	f006 f921 	bl	3000c688 <__DiagPrintf_veneer>
30006446:	2280      	movs	r2, #128	; 0x80
30006448:	21ff      	movs	r1, #255	; 0xff
3000644a:	4630      	mov	r0, r6
3000644c:	f006 f8f4 	bl	3000c638 <____wrap_memset_veneer>
30006450:	2280      	movs	r2, #128	; 0x80
30006452:	4629      	mov	r1, r5
30006454:	4630      	mov	r0, r6
30006456:	f006 f8d7 	bl	3000c608 <____wrap_memcpy_veneer>
3000645a:	f1ba 0f00 	cmp.w	sl, #0
3000645e:	d036      	beq.n	300064ce <BOOT_LoadImages+0x922>
30006460:	46b8      	mov	r8, r7
30006462:	e545      	b.n	30005ef0 <BOOT_LoadImages+0x344>
30006464:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006468:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000646c:	3201      	adds	r2, #1
3000646e:	d03c      	beq.n	300064ea <BOOT_LoadImages+0x93e>
30006470:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006474:	487e      	ldr	r0, [pc, #504]	; (30006670 <BOOT_LoadImages+0xac4>)
30006476:	f006 f907 	bl	3000c688 <__DiagPrintf_veneer>
3000647a:	9705      	str	r7, [sp, #20]
3000647c:	f08a 0601 	eor.w	r6, sl, #1
30006480:	f04f 0801 	mov.w	r8, #1
30006484:	f006 0601 	and.w	r6, r6, #1
30006488:	4647      	mov	r7, r8
3000648a:	e724      	b.n	300062d6 <BOOT_LoadImages+0x72a>
3000648c:	ab16      	add	r3, sp, #88	; 0x58
3000648e:	2280      	movs	r2, #128	; 0x80
30006490:	21ff      	movs	r1, #255	; 0xff
30006492:	461e      	mov	r6, r3
30006494:	4618      	mov	r0, r3
30006496:	ee08 3a10 	vmov	s16, r3
3000649a:	f006 f8cd 	bl	3000c638 <____wrap_memset_veneer>
3000649e:	2280      	movs	r2, #128	; 0x80
300064a0:	4629      	mov	r1, r5
300064a2:	4630      	mov	r0, r6
300064a4:	f006 f8b0 	bl	3000c608 <____wrap_memcpy_veneer>
300064a8:	f1ba 0f00 	cmp.w	sl, #0
300064ac:	f040 80d9 	bne.w	30006662 <BOOT_LoadImages+0xab6>
300064b0:	b9bf      	cbnz	r7, 300064e2 <BOOT_LoadImages+0x936>
300064b2:	46c2      	mov	sl, r8
300064b4:	9705      	str	r7, [sp, #20]
300064b6:	e00c      	b.n	300064d2 <BOOT_LoadImages+0x926>
300064b8:	486e      	ldr	r0, [pc, #440]	; (30006674 <BOOT_LoadImages+0xac8>)
300064ba:	f006 f8e5 	bl	3000c688 <__DiagPrintf_veneer>
300064be:	e4c3      	b.n	30005e48 <BOOT_LoadImages+0x29c>
300064c0:	486a      	ldr	r0, [pc, #424]	; (3000666c <BOOT_LoadImages+0xac0>)
300064c2:	f006 f8e1 	bl	3000c688 <__DiagPrintf_veneer>
300064c6:	e4bf      	b.n	30005e48 <BOOT_LoadImages+0x29c>
300064c8:	f04f 0800 	mov.w	r8, #0
300064cc:	e50d      	b.n	30005eea <BOOT_LoadImages+0x33e>
300064ce:	2301      	movs	r3, #1
300064d0:	9305      	str	r3, [sp, #20]
300064d2:	46d0      	mov	r8, sl
300064d4:	2101      	movs	r1, #1
300064d6:	f04f 0a00 	mov.w	sl, #0
300064da:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300064de:	4657      	mov	r7, sl
300064e0:	e50d      	b.n	30005efe <BOOT_LoadImages+0x352>
300064e2:	f8cd a014 	str.w	sl, [sp, #20]
300064e6:	46c2      	mov	sl, r8
300064e8:	e748      	b.n	3000637c <BOOT_LoadImages+0x7d0>
300064ea:	4860      	ldr	r0, [pc, #384]	; (3000666c <BOOT_LoadImages+0xac0>)
300064ec:	f006 f8cc 	bl	3000c688 <__DiagPrintf_veneer>
300064f0:	e6a2      	b.n	30006238 <BOOT_LoadImages+0x68c>
300064f2:	4620      	mov	r0, r4
300064f4:	e5e0      	b.n	300060b8 <BOOT_LoadImages+0x50c>
300064f6:	428a      	cmp	r2, r1
300064f8:	f4ff ad92 	bcc.w	30006020 <BOOT_LoadImages+0x474>
300064fc:	4282      	cmp	r2, r0
300064fe:	f4ff ad8f 	bcc.w	30006020 <BOOT_LoadImages+0x474>
30006502:	2601      	movs	r6, #1
30006504:	464b      	mov	r3, r9
30006506:	9a07      	ldr	r2, [sp, #28]
30006508:	4651      	mov	r1, sl
3000650a:	4630      	mov	r0, r6
3000650c:	9500      	str	r5, [sp, #0]
3000650e:	9601      	str	r6, [sp, #4]
30006510:	f7fe fdea 	bl	300050e8 <rewrite_bp>
30006514:	f04f 0800 	mov.w	r8, #0
30006518:	b2c3      	uxtb	r3, r0
3000651a:	f8df 9180 	ldr.w	r9, [pc, #384]	; 3000669c <BOOT_LoadImages+0xaf0>
3000651e:	f8df b180 	ldr.w	fp, [pc, #384]	; 300066a0 <BOOT_LoadImages+0xaf4>
30006522:	46c2      	mov	sl, r8
30006524:	9303      	str	r3, [sp, #12]
30006526:	4b54      	ldr	r3, [pc, #336]	; (30006678 <BOOT_LoadImages+0xacc>)
30006528:	9304      	str	r3, [sp, #16]
3000652a:	e497      	b.n	30005e5c <BOOT_LoadImages+0x2b0>
3000652c:	f8cd a014 	str.w	sl, [sp, #20]
30006530:	4639      	mov	r1, r7
30006532:	46ba      	mov	sl, r7
30006534:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
30006538:	e4e1      	b.n	30005efe <BOOT_LoadImages+0x352>
3000653a:	461a      	mov	r2, r3
3000653c:	4619      	mov	r1, r3
3000653e:	484f      	ldr	r0, [pc, #316]	; (3000667c <BOOT_LoadImages+0xad0>)
30006540:	f006 f8a2 	bl	3000c688 <__DiagPrintf_veneer>
30006544:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006548:	4849      	ldr	r0, [pc, #292]	; (30006670 <BOOT_LoadImages+0xac4>)
3000654a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000654e:	f006 f89b 	bl	3000c688 <__DiagPrintf_veneer>
30006552:	2301      	movs	r3, #1
30006554:	4698      	mov	r8, r3
30006556:	9303      	str	r3, [sp, #12]
30006558:	e476      	b.n	30005e48 <BOOT_LoadImages+0x29c>
3000655a:	461a      	mov	r2, r3
3000655c:	4619      	mov	r1, r3
3000655e:	4848      	ldr	r0, [pc, #288]	; (30006680 <BOOT_LoadImages+0xad4>)
30006560:	f04f 0801 	mov.w	r8, #1
30006564:	f006 f890 	bl	3000c688 <__DiagPrintf_veneer>
30006568:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000656c:	4845      	ldr	r0, [pc, #276]	; (30006684 <BOOT_LoadImages+0xad8>)
3000656e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006572:	f006 f889 	bl	3000c688 <__DiagPrintf_veneer>
30006576:	2302      	movs	r3, #2
30006578:	9303      	str	r3, [sp, #12]
3000657a:	e465      	b.n	30005e48 <BOOT_LoadImages+0x29c>
3000657c:	461a      	mov	r2, r3
3000657e:	4619      	mov	r1, r3
30006580:	483f      	ldr	r0, [pc, #252]	; (30006680 <BOOT_LoadImages+0xad4>)
30006582:	f006 f881 	bl	3000c688 <__DiagPrintf_veneer>
30006586:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000658a:	483e      	ldr	r0, [pc, #248]	; (30006684 <BOOT_LoadImages+0xad8>)
3000658c:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006590:	f006 f87a 	bl	3000c688 <__DiagPrintf_veneer>
30006594:	2300      	movs	r3, #0
30006596:	9305      	str	r3, [sp, #20]
30006598:	e4f0      	b.n	30005f7c <BOOT_LoadImages+0x3d0>
3000659a:	483b      	ldr	r0, [pc, #236]	; (30006688 <BOOT_LoadImages+0xadc>)
3000659c:	f006 f874 	bl	3000c688 <__DiagPrintf_veneer>
300065a0:	e7fe      	b.n	300065a0 <BOOT_LoadImages+0x9f4>
300065a2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300065a4:	e438      	b.n	30005e18 <BOOT_LoadImages+0x26c>
300065a6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300065a8:	e5ff      	b.n	300061aa <BOOT_LoadImages+0x5fe>
300065aa:	f10a 3aff 	add.w	sl, sl, #4294967295
300065ae:	f11a 0f03 	cmn.w	sl, #3
300065b2:	d824      	bhi.n	300065fe <BOOT_LoadImages+0xa52>
300065b4:	f1bb 0f01 	cmp.w	fp, #1
300065b8:	d932      	bls.n	30006620 <BOOT_LoadImages+0xa74>
300065ba:	4834      	ldr	r0, [pc, #208]	; (3000668c <BOOT_LoadImages+0xae0>)
300065bc:	e649      	b.n	30006252 <BOOT_LoadImages+0x6a6>
300065be:	4c34      	ldr	r4, [pc, #208]	; (30006690 <BOOT_LoadImages+0xae4>)
300065c0:	e4e2      	b.n	30005f88 <BOOT_LoadImages+0x3dc>
300065c2:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300065c4:	e627      	b.n	30006216 <BOOT_LoadImages+0x66a>
300065c6:	461a      	mov	r2, r3
300065c8:	4619      	mov	r1, r3
300065ca:	482c      	ldr	r0, [pc, #176]	; (3000667c <BOOT_LoadImages+0xad0>)
300065cc:	f006 f85c 	bl	3000c688 <__DiagPrintf_veneer>
300065d0:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300065d4:	4826      	ldr	r0, [pc, #152]	; (30006670 <BOOT_LoadImages+0xac4>)
300065d6:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065da:	f006 f855 	bl	3000c688 <__DiagPrintf_veneer>
300065de:	2300      	movs	r3, #0
300065e0:	9305      	str	r3, [sp, #20]
300065e2:	e74b      	b.n	3000647c <BOOT_LoadImages+0x8d0>
300065e4:	4281      	cmp	r1, r0
300065e6:	f4ff ad21 	bcc.w	3000602c <BOOT_LoadImages+0x480>
300065ea:	2400      	movs	r4, #0
300065ec:	464b      	mov	r3, r9
300065ee:	4642      	mov	r2, r8
300065f0:	4659      	mov	r1, fp
300065f2:	4620      	mov	r0, r4
300065f4:	9500      	str	r5, [sp, #0]
300065f6:	9401      	str	r4, [sp, #4]
300065f8:	e55e      	b.n	300060b8 <BOOT_LoadImages+0x50c>
300065fa:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300065fc:	e70a      	b.n	30006414 <BOOT_LoadImages+0x868>
300065fe:	4825      	ldr	r0, [pc, #148]	; (30006694 <BOOT_LoadImages+0xae8>)
30006600:	e627      	b.n	30006252 <BOOT_LoadImages+0x6a6>
30006602:	2400      	movs	r4, #0
30006604:	464b      	mov	r3, r9
30006606:	4642      	mov	r2, r8
30006608:	4659      	mov	r1, fp
3000660a:	9500      	str	r5, [sp, #0]
3000660c:	9401      	str	r4, [sp, #4]
3000660e:	e552      	b.n	300060b6 <BOOT_LoadImages+0x50a>
30006610:	2601      	movs	r6, #1
30006612:	464b      	mov	r3, r9
30006614:	9a07      	ldr	r2, [sp, #28]
30006616:	4651      	mov	r1, sl
30006618:	2000      	movs	r0, #0
3000661a:	9500      	str	r5, [sp, #0]
3000661c:	9601      	str	r6, [sp, #4]
3000661e:	e777      	b.n	30006510 <BOOT_LoadImages+0x964>
30006620:	9904      	ldr	r1, [sp, #16]
30006622:	481d      	ldr	r0, [pc, #116]	; (30006698 <BOOT_LoadImages+0xaec>)
30006624:	f006 f830 	bl	3000c688 <__DiagPrintf_veneer>
30006628:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000662c:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006630:	9307      	str	r3, [sp, #28]
30006632:	2301      	movs	r3, #1
30006634:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30006638:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000663c:	9309      	str	r3, [sp, #36]	; 0x24
3000663e:	e615      	b.n	3000626c <BOOT_LoadImages+0x6c0>
30006640:	f08a 0601 	eor.w	r6, sl, #1
30006644:	f006 0601 	and.w	r6, r6, #1
30006648:	e408      	b.n	30005e5c <BOOT_LoadImages+0x2b0>
3000664a:	f08a 0601 	eor.w	r6, sl, #1
3000664e:	f8cd 8014 	str.w	r8, [sp, #20]
30006652:	f006 0601 	and.w	r6, r6, #1
30006656:	e401      	b.n	30005e5c <BOOT_LoadImages+0x2b0>
30006658:	2601      	movs	r6, #1
3000665a:	4c0d      	ldr	r4, [pc, #52]	; (30006690 <BOOT_LoadImages+0xae4>)
3000665c:	46b2      	mov	sl, r6
3000665e:	f7ff bbc1 	b.w	30005de4 <BOOT_LoadImages+0x238>
30006662:	f04f 0a00 	mov.w	sl, #0
30006666:	e440      	b.n	30005eea <BOOT_LoadImages+0x33e>
30006668:	3000cfe0 	.word	0x3000cfe0
3000666c:	3000cf84 	.word	0x3000cf84
30006670:	3000ccd4 	.word	0x3000ccd4
30006674:	3000cf94 	.word	0x3000cf94
30006678:	3000e9eb 	.word	0x3000e9eb
3000667c:	3000cfa4 	.word	0x3000cfa4
30006680:	3000d02c 	.word	0x3000d02c
30006684:	3000ccec 	.word	0x3000ccec
30006688:	3000d0ec 	.word	0x3000d0ec
3000668c:	3000d14c 	.word	0x3000d14c
30006690:	3000ebb0 	.word	0x3000ebb0
30006694:	3000cdd4 	.word	0x3000cdd4
30006698:	3000d184 	.word	0x3000d184
3000669c:	30010364 	.word	0x30010364
300066a0:	30004221 	.word	0x30004221

300066a4 <BOOT_ReasonSet>:
300066a4:	4a03      	ldr	r2, [pc, #12]	; (300066b4 <BOOT_ReasonSet+0x10>)
300066a6:	4904      	ldr	r1, [pc, #16]	; (300066b8 <BOOT_ReasonSet+0x14>)
300066a8:	8c13      	ldrh	r3, [r2, #32]
300066aa:	b29b      	uxth	r3, r3
300066ac:	8413      	strh	r3, [r2, #32]
300066ae:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
300066b2:	4770      	bx	lr
300066b4:	42008000 	.word	0x42008000
300066b8:	42008200 	.word	0x42008200

300066bc <BOOT_Enable_KM0>:
300066bc:	b510      	push	{r4, lr}
300066be:	4c12      	ldr	r4, [pc, #72]	; (30006708 <BOOT_Enable_KM0+0x4c>)
300066c0:	47a0      	blx	r4
300066c2:	b110      	cbz	r0, 300066ca <BOOT_Enable_KM0+0xe>
300066c4:	4b11      	ldr	r3, [pc, #68]	; (3000670c <BOOT_Enable_KM0+0x50>)
300066c6:	4798      	blx	r3
300066c8:	b1a8      	cbz	r0, 300066f6 <BOOT_Enable_KM0+0x3a>
300066ca:	47a0      	blx	r4
300066cc:	b940      	cbnz	r0, 300066e0 <BOOT_Enable_KM0+0x24>
300066ce:	4a10      	ldr	r2, [pc, #64]	; (30006710 <BOOT_Enable_KM0+0x54>)
300066d0:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300066d4:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300066d8:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300066dc:	bf40      	sev
300066de:	bd10      	pop	{r4, pc}
300066e0:	4b0a      	ldr	r3, [pc, #40]	; (3000670c <BOOT_Enable_KM0+0x50>)
300066e2:	4798      	blx	r3
300066e4:	2800      	cmp	r0, #0
300066e6:	d0f2      	beq.n	300066ce <BOOT_Enable_KM0+0x12>
300066e8:	4b0a      	ldr	r3, [pc, #40]	; (30006714 <BOOT_Enable_KM0+0x58>)
300066ea:	2004      	movs	r0, #4
300066ec:	4a0a      	ldr	r2, [pc, #40]	; (30006718 <BOOT_Enable_KM0+0x5c>)
300066ee:	4619      	mov	r1, r3
300066f0:	f005 fd26 	bl	3000c140 <rtk_log_write>
300066f4:	e7eb      	b.n	300066ce <BOOT_Enable_KM0+0x12>
300066f6:	4b07      	ldr	r3, [pc, #28]	; (30006714 <BOOT_Enable_KM0+0x58>)
300066f8:	2004      	movs	r0, #4
300066fa:	4a08      	ldr	r2, [pc, #32]	; (3000671c <BOOT_Enable_KM0+0x60>)
300066fc:	4619      	mov	r1, r3
300066fe:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006702:	f005 bd1d 	b.w	3000c140 <rtk_log_write>
30006706:	bf00      	nop
30006708:	0000c09d 	.word	0x0000c09d
3000670c:	0000c0ad 	.word	0x0000c0ad
30006710:	42008000 	.word	0x42008000
30006714:	3000c974 	.word	0x3000c974
30006718:	3000d1c8 	.word	0x3000d1c8
3000671c:	3000d1a0 	.word	0x3000d1a0

30006720 <BOOT_AP_Clk_Get>:
30006720:	4a06      	ldr	r2, [pc, #24]	; (3000673c <BOOT_AP_Clk_Get+0x1c>)
30006722:	6913      	ldr	r3, [r2, #16]
30006724:	2b00      	cmp	r3, #0
30006726:	db03      	blt.n	30006730 <BOOT_AP_Clk_Get+0x10>
30006728:	6810      	ldr	r0, [r2, #0]
3000672a:	fbb0 f0f3 	udiv	r0, r0, r3
3000672e:	4770      	bx	lr
30006730:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30006734:	6850      	ldr	r0, [r2, #4]
30006736:	fbb0 f0f3 	udiv	r0, r0, r3
3000673a:	4770      	bx	lr
3000673c:	3000f238 	.word	0x3000f238

30006740 <BOOT_SOC_ClkSet>:
30006740:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006744:	4c5d      	ldr	r4, [pc, #372]	; (300068bc <BOOT_SOC_ClkSet+0x17c>)
30006746:	b083      	sub	sp, #12
30006748:	6926      	ldr	r6, [r4, #16]
3000674a:	6825      	ldr	r5, [r4, #0]
3000674c:	2e00      	cmp	r6, #0
3000674e:	db7a      	blt.n	30006846 <BOOT_SOC_ClkSet+0x106>
30006750:	fbb5 f6f6 	udiv	r6, r5, r6
30006754:	4b5a      	ldr	r3, [pc, #360]	; (300068c0 <BOOT_SOC_ClkSet+0x180>)
30006756:	68a7      	ldr	r7, [r4, #8]
30006758:	fba3 1305 	umull	r1, r3, r3, r5
3000675c:	68e2      	ldr	r2, [r4, #12]
3000675e:	2f01      	cmp	r7, #1
30006760:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
30006764:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30006768:	ea4f 1393 	mov.w	r3, r3, lsr #6
3000676c:	f10a 3aff 	add.w	sl, sl, #4294967295
30006770:	f10b 3bff 	add.w	fp, fp, #4294967295
30006774:	f103 39ff 	add.w	r9, r3, #4294967295
30006778:	fbb5 f5f2 	udiv	r5, r5, r2
3000677c:	d06f      	beq.n	3000685e <BOOT_SOC_ClkSet+0x11e>
3000677e:	4b51      	ldr	r3, [pc, #324]	; (300068c4 <BOOT_SOC_ClkSet+0x184>)
30006780:	2200      	movs	r2, #0
30006782:	2dfa      	cmp	r5, #250	; 0xfa
30006784:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30006788:	d863      	bhi.n	30006852 <BOOT_SOC_ClkSet+0x112>
3000678a:	4f4f      	ldr	r7, [pc, #316]	; (300068c8 <BOOT_SOC_ClkSet+0x188>)
3000678c:	783b      	ldrb	r3, [r7, #0]
3000678e:	2b01      	cmp	r3, #1
30006790:	d103      	bne.n	3000679a <BOOT_SOC_ClkSet+0x5a>
30006792:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
30006796:	f200 8085 	bhi.w	300068a4 <BOOT_SOC_ClkSet+0x164>
3000679a:	2001      	movs	r0, #1
3000679c:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300068f4 <BOOT_SOC_ClkSet+0x1b4>
300067a0:	f001 fbb8 	bl	30007f14 <CLK_SWITCH_XTAL>
300067a4:	6820      	ldr	r0, [r4, #0]
300067a6:	4b49      	ldr	r3, [pc, #292]	; (300068cc <BOOT_SOC_ClkSet+0x18c>)
300067a8:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
300067ac:	fb08 f000 	mul.w	r0, r8, r0
300067b0:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
300067b4:	4798      	blx	r3
300067b6:	4a46      	ldr	r2, [pc, #280]	; (300068d0 <BOOT_SOC_ClkSet+0x190>)
300067b8:	68e1      	ldr	r1, [r4, #12]
300067ba:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300067be:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300067c2:	ea4f 5909 	mov.w	r9, r9, lsl #20
300067c6:	1e4b      	subs	r3, r1, #1
300067c8:	4942      	ldr	r1, [pc, #264]	; (300068d4 <BOOT_SOC_ClkSet+0x194>)
300067ca:	fa1f fb8b 	uxth.w	fp, fp
300067ce:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300067d2:	4001      	ands	r1, r0
300067d4:	f003 0307 	and.w	r3, r3, #7
300067d8:	2000      	movs	r0, #0
300067da:	430b      	orrs	r3, r1
300067dc:	ea4a 0303 	orr.w	r3, sl, r3
300067e0:	ea4b 0303 	orr.w	r3, fp, r3
300067e4:	ea49 0303 	orr.w	r3, r9, r3
300067e8:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300067ec:	f001 fb92 	bl	30007f14 <CLK_SWITCH_XTAL>
300067f0:	4b39      	ldr	r3, [pc, #228]	; (300068d8 <BOOT_SOC_ClkSet+0x198>)
300067f2:	4a3a      	ldr	r2, [pc, #232]	; (300068dc <BOOT_SOC_ClkSet+0x19c>)
300067f4:	2004      	movs	r0, #4
300067f6:	4619      	mov	r1, r3
300067f8:	9500      	str	r5, [sp, #0]
300067fa:	f005 fca1 	bl	3000c140 <rtk_log_write>
300067fe:	783b      	ldrb	r3, [r7, #0]
30006800:	b1f3      	cbz	r3, 30006840 <BOOT_SOC_ClkSet+0x100>
30006802:	6923      	ldr	r3, [r4, #16]
30006804:	2b00      	cmp	r3, #0
30006806:	db40      	blt.n	3000688a <BOOT_SOC_ClkSet+0x14a>
30006808:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000680c:	492d      	ldr	r1, [pc, #180]	; (300068c4 <BOOT_SOC_ClkSet+0x184>)
3000680e:	2000      	movs	r0, #0
30006810:	6913      	ldr	r3, [r2, #16]
30006812:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
30006816:	f043 030c 	orr.w	r3, r3, #12
3000681a:	6113      	str	r3, [r2, #16]
3000681c:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30006820:	6921      	ldr	r1, [r4, #16]
30006822:	4b2d      	ldr	r3, [pc, #180]	; (300068d8 <BOOT_SOC_ClkSet+0x198>)
30006824:	6928      	ldr	r0, [r5, #16]
30006826:	3901      	subs	r1, #1
30006828:	4a2d      	ldr	r2, [pc, #180]	; (300068e0 <BOOT_SOC_ClkSet+0x1a0>)
3000682a:	f020 0003 	bic.w	r0, r0, #3
3000682e:	f001 0103 	and.w	r1, r1, #3
30006832:	4301      	orrs	r1, r0
30006834:	2004      	movs	r0, #4
30006836:	6129      	str	r1, [r5, #16]
30006838:	4619      	mov	r1, r3
3000683a:	9600      	str	r6, [sp, #0]
3000683c:	f005 fc80 	bl	3000c140 <rtk_log_write>
30006840:	b003      	add	sp, #12
30006842:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006846:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
3000684a:	6863      	ldr	r3, [r4, #4]
3000684c:	fbb3 f6f6 	udiv	r6, r3, r6
30006850:	e780      	b.n	30006754 <BOOT_SOC_ClkSet+0x14>
30006852:	f240 61b6 	movw	r1, #1718	; 0x6b6
30006856:	4823      	ldr	r0, [pc, #140]	; (300068e4 <BOOT_SOC_ClkSet+0x1a4>)
30006858:	f005 ff56 	bl	3000c708 <__io_assert_failed_veneer>
3000685c:	e795      	b.n	3000678a <BOOT_SOC_ClkSet+0x4a>
3000685e:	4b22      	ldr	r3, [pc, #136]	; (300068e8 <BOOT_SOC_ClkSet+0x1a8>)
30006860:	4638      	mov	r0, r7
30006862:	4798      	blx	r3
30006864:	4b17      	ldr	r3, [pc, #92]	; (300068c4 <BOOT_SOC_ClkSet+0x184>)
30006866:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
3000686a:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
3000686e:	d21f      	bcs.n	300068b0 <BOOT_SOC_ClkSet+0x170>
30006870:	4f15      	ldr	r7, [pc, #84]	; (300068c8 <BOOT_SOC_ClkSet+0x188>)
30006872:	783b      	ldrb	r3, [r7, #0]
30006874:	2b01      	cmp	r3, #1
30006876:	d190      	bne.n	3000679a <BOOT_SOC_ClkSet+0x5a>
30006878:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
3000687c:	d98d      	bls.n	3000679a <BOOT_SOC_ClkSet+0x5a>
3000687e:	f240 61b2 	movw	r1, #1714	; 0x6b2
30006882:	4818      	ldr	r0, [pc, #96]	; (300068e4 <BOOT_SOC_ClkSet+0x1a4>)
30006884:	f005 ff40 	bl	3000c708 <__io_assert_failed_veneer>
30006888:	e787      	b.n	3000679a <BOOT_SOC_ClkSet+0x5a>
3000688a:	6860      	ldr	r0, [r4, #4]
3000688c:	4b17      	ldr	r3, [pc, #92]	; (300068ec <BOOT_SOC_ClkSet+0x1ac>)
3000688e:	fb08 f000 	mul.w	r0, r8, r0
30006892:	4798      	blx	r3
30006894:	4b16      	ldr	r3, [pc, #88]	; (300068f0 <BOOT_SOC_ClkSet+0x1b0>)
30006896:	2001      	movs	r0, #1
30006898:	4798      	blx	r3
3000689a:	4b0a      	ldr	r3, [pc, #40]	; (300068c4 <BOOT_SOC_ClkSet+0x184>)
3000689c:	2201      	movs	r2, #1
3000689e:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300068a2:	e7bb      	b.n	3000681c <BOOT_SOC_ClkSet+0xdc>
300068a4:	f44f 61d7 	mov.w	r1, #1720	; 0x6b8
300068a8:	480e      	ldr	r0, [pc, #56]	; (300068e4 <BOOT_SOC_ClkSet+0x1a4>)
300068aa:	f005 ff2d 	bl	3000c708 <__io_assert_failed_veneer>
300068ae:	e774      	b.n	3000679a <BOOT_SOC_ClkSet+0x5a>
300068b0:	f44f 61d6 	mov.w	r1, #1712	; 0x6b0
300068b4:	480b      	ldr	r0, [pc, #44]	; (300068e4 <BOOT_SOC_ClkSet+0x1a4>)
300068b6:	f005 ff27 	bl	3000c708 <__io_assert_failed_veneer>
300068ba:	e7d9      	b.n	30006870 <BOOT_SOC_ClkSet+0x130>
300068bc:	3000f238 	.word	0x3000f238
300068c0:	51eb851f 	.word	0x51eb851f
300068c4:	23020000 	.word	0x23020000
300068c8:	3000e9eb 	.word	0x3000e9eb
300068cc:	30008d41 	.word	0x30008d41
300068d0:	42008000 	.word	0x42008000
300068d4:	ff8f08f8 	.word	0xff8f08f8
300068d8:	3000c974 	.word	0x3000c974
300068dc:	3000d1e8 	.word	0x3000d1e8
300068e0:	3000d200 	.word	0x3000d200
300068e4:	3000e6f0 	.word	0x3000e6f0
300068e8:	3000b7e9 	.word	0x3000b7e9
300068ec:	30008dc1 	.word	0x30008dc1
300068f0:	30008e01 	.word	0x30008e01
300068f4:	000f4240 	.word	0x000f4240

300068f8 <BOOT_Disable_AP>:
300068f8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300068fc:	4906      	ldr	r1, [pc, #24]	; (30006918 <BOOT_Disable_AP+0x20>)
300068fe:	685a      	ldr	r2, [r3, #4]
30006900:	f042 0202 	orr.w	r2, r2, #2
30006904:	605a      	str	r2, [r3, #4]
30006906:	685a      	ldr	r2, [r3, #4]
30006908:	f042 0230 	orr.w	r2, r2, #48	; 0x30
3000690c:	605a      	str	r2, [r3, #4]
3000690e:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006910:	f043 0303 	orr.w	r3, r3, #3
30006914:	678b      	str	r3, [r1, #120]	; 0x78
30006916:	4770      	bx	lr
30006918:	41000200 	.word	0x41000200

3000691c <BOOT_Enable_AP>:
3000691c:	b538      	push	{r3, r4, r5, lr}
3000691e:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006922:	2201      	movs	r2, #1
30006924:	2104      	movs	r1, #4
30006926:	4b2b      	ldr	r3, [pc, #172]	; (300069d4 <BOOT_Enable_AP+0xb8>)
30006928:	482b      	ldr	r0, [pc, #172]	; (300069d8 <BOOT_Enable_AP+0xbc>)
3000692a:	4798      	blx	r3
3000692c:	6863      	ldr	r3, [r4, #4]
3000692e:	4d2b      	ldr	r5, [pc, #172]	; (300069dc <BOOT_Enable_AP+0xc0>)
30006930:	2032      	movs	r0, #50	; 0x32
30006932:	f043 0302 	orr.w	r3, r3, #2
30006936:	6063      	str	r3, [r4, #4]
30006938:	6863      	ldr	r3, [r4, #4]
3000693a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000693e:	6063      	str	r3, [r4, #4]
30006940:	6823      	ldr	r3, [r4, #0]
30006942:	f043 0302 	orr.w	r3, r3, #2
30006946:	6023      	str	r3, [r4, #0]
30006948:	47a8      	blx	r5
3000694a:	6823      	ldr	r3, [r4, #0]
3000694c:	2032      	movs	r0, #50	; 0x32
3000694e:	f043 0304 	orr.w	r3, r3, #4
30006952:	6023      	str	r3, [r4, #0]
30006954:	6823      	ldr	r3, [r4, #0]
30006956:	f043 0301 	orr.w	r3, r3, #1
3000695a:	6023      	str	r3, [r4, #0]
3000695c:	6823      	ldr	r3, [r4, #0]
3000695e:	f043 0310 	orr.w	r3, r3, #16
30006962:	6023      	str	r3, [r4, #0]
30006964:	47a8      	blx	r5
30006966:	6823      	ldr	r3, [r4, #0]
30006968:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
3000696c:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006970:	6023      	str	r3, [r4, #0]
30006972:	47a8      	blx	r5
30006974:	6863      	ldr	r3, [r4, #4]
30006976:	4a1a      	ldr	r2, [pc, #104]	; (300069e0 <BOOT_Enable_AP+0xc4>)
30006978:	f023 0310 	bic.w	r3, r3, #16
3000697c:	6063      	str	r3, [r4, #4]
3000697e:	6863      	ldr	r3, [r4, #4]
30006980:	f023 0302 	bic.w	r3, r3, #2
30006984:	6063      	str	r3, [r4, #4]
30006986:	4790      	blx	r2
30006988:	b178      	cbz	r0, 300069aa <BOOT_Enable_AP+0x8e>
3000698a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000698e:	4815      	ldr	r0, [pc, #84]	; (300069e4 <BOOT_Enable_AP+0xc8>)
30006990:	4a15      	ldr	r2, [pc, #84]	; (300069e8 <BOOT_Enable_AP+0xcc>)
30006992:	68d9      	ldr	r1, [r3, #12]
30006994:	f041 0101 	orr.w	r1, r1, #1
30006998:	60d9      	str	r1, [r3, #12]
3000699a:	6899      	ldr	r1, [r3, #8]
3000699c:	f041 0101 	orr.w	r1, r1, #1
300069a0:	6099      	str	r1, [r3, #8]
300069a2:	6843      	ldr	r3, [r0, #4]
300069a4:	431a      	orrs	r2, r3
300069a6:	6042      	str	r2, [r0, #4]
300069a8:	bd38      	pop	{r3, r4, r5, pc}
300069aa:	4604      	mov	r4, r0
300069ac:	4d0f      	ldr	r5, [pc, #60]	; (300069ec <BOOT_Enable_AP+0xd0>)
300069ae:	4601      	mov	r1, r0
300069b0:	2041      	movs	r0, #65	; 0x41
300069b2:	47a8      	blx	r5
300069b4:	4621      	mov	r1, r4
300069b6:	2042      	movs	r0, #66	; 0x42
300069b8:	47a8      	blx	r5
300069ba:	4621      	mov	r1, r4
300069bc:	2043      	movs	r0, #67	; 0x43
300069be:	47a8      	blx	r5
300069c0:	4621      	mov	r1, r4
300069c2:	2044      	movs	r0, #68	; 0x44
300069c4:	47a8      	blx	r5
300069c6:	4621      	mov	r1, r4
300069c8:	2045      	movs	r0, #69	; 0x45
300069ca:	47a8      	blx	r5
300069cc:	4621      	mov	r1, r4
300069ce:	2046      	movs	r0, #70	; 0x46
300069d0:	47a8      	blx	r5
300069d2:	e7da      	b.n	3000698a <BOOT_Enable_AP+0x6e>
300069d4:	0000aab9 	.word	0x0000aab9
300069d8:	4200c000 	.word	0x4200c000
300069dc:	00009b2d 	.word	0x00009b2d
300069e0:	30009ead 	.word	0x30009ead
300069e4:	41000200 	.word	0x41000200
300069e8:	01001111 	.word	0x01001111
300069ec:	0000b20d 	.word	0x0000b20d

300069f0 <BOOT_RAM_KeyDeriveFunc>:
300069f0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300069f4:	ed2d 8b04 	vpush	{d8-d9}
300069f8:	b0cb      	sub	sp, #300	; 0x12c
300069fa:	4c6a      	ldr	r4, [pc, #424]	; (30006ba4 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
300069fc:	4680      	mov	r8, r0
300069fe:	4689      	mov	r9, r1
30006a00:	4869      	ldr	r0, [pc, #420]	; (30006ba8 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006a02:	ee08 2a90 	vmov	s17, r2
30006a06:	4969      	ldr	r1, [pc, #420]	; (30006bac <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006a08:	2201      	movs	r2, #1
30006a0a:	ee09 3a10 	vmov	s18, r3
30006a0e:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006a12:	47a0      	blx	r4
30006a14:	2201      	movs	r2, #1
30006a16:	4966      	ldr	r1, [pc, #408]	; (30006bb0 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006a18:	ae02      	add	r6, sp, #8
30006a1a:	4864      	ldr	r0, [pc, #400]	; (30006bac <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006a1c:	47a0      	blx	r4
30006a1e:	4b65      	ldr	r3, [pc, #404]	; (30006bb4 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006a20:	2000      	movs	r0, #0
30006a22:	4798      	blx	r3
30006a24:	22e0      	movs	r2, #224	; 0xe0
30006a26:	2100      	movs	r1, #0
30006a28:	a812      	add	r0, sp, #72	; 0x48
30006a2a:	f005 fe05 	bl	3000c638 <____wrap_memset_veneer>
30006a2e:	2300      	movs	r3, #0
30006a30:	f8ad 3004 	strh.w	r3, [sp, #4]
30006a34:	f88d 3006 	strb.w	r3, [sp, #6]
30006a38:	2301      	movs	r3, #1
30006a3a:	f88d 3007 	strb.w	r3, [sp, #7]
30006a3e:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006a40:	2b00      	cmp	r3, #0
30006a42:	f000 80ab 	beq.w	30006b9c <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006a46:	ab0a      	add	r3, sp, #40	; 0x28
30006a48:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006bc8 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006a4c:	4f5a      	ldr	r7, [pc, #360]	; (30006bb8 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006a4e:	ee08 3a10 	vmov	s16, r3
30006a52:	ab12      	add	r3, sp, #72	; 0x48
30006a54:	464a      	mov	r2, r9
30006a56:	4641      	mov	r1, r8
30006a58:	2020      	movs	r0, #32
30006a5a:	47d8      	blx	fp
30006a5c:	aa12      	add	r2, sp, #72	; 0x48
30006a5e:	ee19 1a10 	vmov	r1, s18
30006a62:	ee18 0a90 	vmov	r0, s17
30006a66:	47b8      	blx	r7
30006a68:	ee18 5a10 	vmov	r5, s16
30006a6c:	aa12      	add	r2, sp, #72	; 0x48
30006a6e:	2104      	movs	r1, #4
30006a70:	a801      	add	r0, sp, #4
30006a72:	4634      	mov	r4, r6
30006a74:	47b8      	blx	r7
30006a76:	a912      	add	r1, sp, #72	; 0x48
30006a78:	4b50      	ldr	r3, [pc, #320]	; (30006bbc <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006a7a:	ee18 0a10 	vmov	r0, s16
30006a7e:	4798      	blx	r3
30006a80:	f1ba 0f01 	cmp.w	sl, #1
30006a84:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006a86:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006a88:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006a8c:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006a90:	d91f      	bls.n	30006ad2 <BOOT_RAM_KeyDeriveFunc+0xe2>
30006a92:	2501      	movs	r5, #1
30006a94:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006a98:	ab12      	add	r3, sp, #72	; 0x48
30006a9a:	4641      	mov	r1, r8
30006a9c:	464a      	mov	r2, r9
30006a9e:	2020      	movs	r0, #32
30006aa0:	47d8      	blx	fp
30006aa2:	aa12      	add	r2, sp, #72	; 0x48
30006aa4:	2120      	movs	r1, #32
30006aa6:	4630      	mov	r0, r6
30006aa8:	47b8      	blx	r7
30006aaa:	a912      	add	r1, sp, #72	; 0x48
30006aac:	4b43      	ldr	r3, [pc, #268]	; (30006bbc <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006aae:	4630      	mov	r0, r6
30006ab0:	4798      	blx	r3
30006ab2:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006ab6:	f10d 0107 	add.w	r1, sp, #7
30006aba:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006abe:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006ac2:	42a3      	cmp	r3, r4
30006ac4:	ea82 0200 	eor.w	r2, r2, r0
30006ac8:	701a      	strb	r2, [r3, #0]
30006aca:	d1f6      	bne.n	30006aba <BOOT_RAM_KeyDeriveFunc+0xca>
30006acc:	3501      	adds	r5, #1
30006ace:	45aa      	cmp	sl, r5
30006ad0:	d1e2      	bne.n	30006a98 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006ad2:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006ad4:	ee18 1a10 	vmov	r1, s16
30006ad8:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006ada:	2a20      	cmp	r2, #32
30006adc:	985a      	ldr	r0, [sp, #360]	; 0x168
30006ade:	bf28      	it	cs
30006ae0:	2220      	movcs	r2, #32
30006ae2:	1a9b      	subs	r3, r3, r2
30006ae4:	9359      	str	r3, [sp, #356]	; 0x164
30006ae6:	4603      	mov	r3, r0
30006ae8:	4413      	add	r3, r2
30006aea:	935a      	str	r3, [sp, #360]	; 0x168
30006aec:	f005 fd8c 	bl	3000c608 <____wrap_memcpy_veneer>
30006af0:	f10d 0207 	add.w	r2, sp, #7
30006af4:	2104      	movs	r1, #4
30006af6:	7813      	ldrb	r3, [r2, #0]
30006af8:	3901      	subs	r1, #1
30006afa:	3301      	adds	r3, #1
30006afc:	b2db      	uxtb	r3, r3
30006afe:	f802 3901 	strb.w	r3, [r2], #-1
30006b02:	b90b      	cbnz	r3, 30006b08 <BOOT_RAM_KeyDeriveFunc+0x118>
30006b04:	2900      	cmp	r1, #0
30006b06:	d1f6      	bne.n	30006af6 <BOOT_RAM_KeyDeriveFunc+0x106>
30006b08:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006b0a:	2b00      	cmp	r3, #0
30006b0c:	d1a1      	bne.n	30006a52 <BOOT_RAM_KeyDeriveFunc+0x62>
30006b0e:	2220      	movs	r2, #32
30006b10:	2100      	movs	r1, #0
30006b12:	4c2b      	ldr	r4, [pc, #172]	; (30006bc0 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006b14:	4630      	mov	r0, r6
30006b16:	47a0      	blx	r4
30006b18:	2220      	movs	r2, #32
30006b1a:	2100      	movs	r1, #0
30006b1c:	ee18 0a10 	vmov	r0, s16
30006b20:	47a0      	blx	r4
30006b22:	4b28      	ldr	r3, [pc, #160]	; (30006bc4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b24:	695b      	ldr	r3, [r3, #20]
30006b26:	03da      	lsls	r2, r3, #15
30006b28:	d512      	bpl.n	30006b50 <BOOT_RAM_KeyDeriveFunc+0x160>
30006b2a:	f016 021f 	ands.w	r2, r6, #31
30006b2e:	4633      	mov	r3, r6
30006b30:	d130      	bne.n	30006b94 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006b32:	2220      	movs	r2, #32
30006b34:	f3bf 8f4f 	dsb	sy
30006b38:	4822      	ldr	r0, [pc, #136]	; (30006bc4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b3a:	441a      	add	r2, r3
30006b3c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006b40:	3320      	adds	r3, #32
30006b42:	1ad1      	subs	r1, r2, r3
30006b44:	2900      	cmp	r1, #0
30006b46:	dcf9      	bgt.n	30006b3c <BOOT_RAM_KeyDeriveFunc+0x14c>
30006b48:	f3bf 8f4f 	dsb	sy
30006b4c:	f3bf 8f6f 	isb	sy
30006b50:	4b1c      	ldr	r3, [pc, #112]	; (30006bc4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b52:	695b      	ldr	r3, [r3, #20]
30006b54:	03db      	lsls	r3, r3, #15
30006b56:	d513      	bpl.n	30006b80 <BOOT_RAM_KeyDeriveFunc+0x190>
30006b58:	ee18 3a10 	vmov	r3, s16
30006b5c:	f013 021f 	ands.w	r2, r3, #31
30006b60:	d114      	bne.n	30006b8c <BOOT_RAM_KeyDeriveFunc+0x19c>
30006b62:	2220      	movs	r2, #32
30006b64:	f3bf 8f4f 	dsb	sy
30006b68:	4816      	ldr	r0, [pc, #88]	; (30006bc4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b6a:	441a      	add	r2, r3
30006b6c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006b70:	3320      	adds	r3, #32
30006b72:	1ad1      	subs	r1, r2, r3
30006b74:	2900      	cmp	r1, #0
30006b76:	dcf9      	bgt.n	30006b6c <BOOT_RAM_KeyDeriveFunc+0x17c>
30006b78:	f3bf 8f4f 	dsb	sy
30006b7c:	f3bf 8f6f 	isb	sy
30006b80:	2000      	movs	r0, #0
30006b82:	b04b      	add	sp, #300	; 0x12c
30006b84:	ecbd 8b04 	vpop	{d8-d9}
30006b88:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b8c:	f023 031f 	bic.w	r3, r3, #31
30006b90:	3220      	adds	r2, #32
30006b92:	e7e7      	b.n	30006b64 <BOOT_RAM_KeyDeriveFunc+0x174>
30006b94:	f026 031f 	bic.w	r3, r6, #31
30006b98:	3220      	adds	r2, #32
30006b9a:	e7cb      	b.n	30006b34 <BOOT_RAM_KeyDeriveFunc+0x144>
30006b9c:	ab0a      	add	r3, sp, #40	; 0x28
30006b9e:	ee08 3a10 	vmov	s16, r3
30006ba2:	e7b4      	b.n	30006b0e <BOOT_RAM_KeyDeriveFunc+0x11e>
30006ba4:	0000b479 	.word	0x0000b479
30006ba8:	40000008 	.word	0x40000008
30006bac:	40000004 	.word	0x40000004
30006bb0:	40000002 	.word	0x40000002
30006bb4:	3000a149 	.word	0x3000a149
30006bb8:	30009f4d 	.word	0x30009f4d
30006bbc:	30009f89 	.word	0x30009f89
30006bc0:	00012be5 	.word	0x00012be5
30006bc4:	e000ed00 	.word	0xe000ed00
30006bc8:	300097cd 	.word	0x300097cd

30006bcc <password_hash_check>:
30006bcc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006bd0:	b08f      	sub	sp, #60	; 0x3c
30006bd2:	4690      	mov	r8, r2
30006bd4:	461d      	mov	r5, r3
30006bd6:	4606      	mov	r6, r0
30006bd8:	af04      	add	r7, sp, #16
30006bda:	3201      	adds	r2, #1
30006bdc:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006bde:	6079      	str	r1, [r7, #4]
30006be0:	2100      	movs	r1, #0
30006be2:	f103 0008 	add.w	r0, r3, #8
30006be6:	f108 0308 	add.w	r3, r8, #8
30006bea:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006bee:	f023 0307 	bic.w	r3, r3, #7
30006bf2:	f020 0007 	bic.w	r0, r0, #7
30006bf6:	ebad 0d03 	sub.w	sp, sp, r3
30006bfa:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006bfc:	f10d 0910 	add.w	r9, sp, #16
30006c00:	3301      	adds	r3, #1
30006c02:	ebad 0d00 	sub.w	sp, sp, r0
30006c06:	603b      	str	r3, [r7, #0]
30006c08:	4648      	mov	r0, r9
30006c0a:	4b3f      	ldr	r3, [pc, #252]	; (30006d08 <password_hash_check+0x13c>)
30006c0c:	f10d 0a10 	add.w	sl, sp, #16
30006c10:	4798      	blx	r3
30006c12:	683b      	ldr	r3, [r7, #0]
30006c14:	2100      	movs	r1, #0
30006c16:	4650      	mov	r0, sl
30006c18:	461a      	mov	r2, r3
30006c1a:	4b3b      	ldr	r3, [pc, #236]	; (30006d08 <password_hash_check+0x13c>)
30006c1c:	4798      	blx	r3
30006c1e:	f1b8 0001 	subs.w	r0, r8, #1
30006c22:	d01d      	beq.n	30006c60 <password_hash_check+0x94>
30006c24:	2200      	movs	r2, #0
30006c26:	f109 33ff 	add.w	r3, r9, #4294967295
30006c2a:	4611      	mov	r1, r2
30006c2c:	e00b      	b.n	30006c46 <password_hash_check+0x7a>
30006c2e:	f108 38ff 	add.w	r8, r8, #4294967295
30006c32:	2100      	movs	r1, #0
30006c34:	1c9c      	adds	r4, r3, #2
30006c36:	4282      	cmp	r2, r0
30006c38:	7059      	strb	r1, [r3, #1]
30006c3a:	f103 0301 	add.w	r3, r3, #1
30006c3e:	4611      	mov	r1, r2
30006c40:	eba4 0409 	sub.w	r4, r4, r9
30006c44:	d20b      	bcs.n	30006c5e <password_hash_check+0x92>
30006c46:	1874      	adds	r4, r6, r1
30006c48:	f102 0c01 	add.w	ip, r2, #1
30006c4c:	5c71      	ldrb	r1, [r6, r1]
30006c4e:	3202      	adds	r2, #2
30006c50:	7864      	ldrb	r4, [r4, #1]
30006c52:	2c30      	cmp	r4, #48	; 0x30
30006c54:	d101      	bne.n	30006c5a <password_hash_check+0x8e>
30006c56:	295c      	cmp	r1, #92	; 0x5c
30006c58:	d0e9      	beq.n	30006c2e <password_hash_check+0x62>
30006c5a:	4662      	mov	r2, ip
30006c5c:	e7ea      	b.n	30006c34 <password_hash_check+0x68>
30006c5e:	4406      	add	r6, r0
30006c60:	7833      	ldrb	r3, [r6, #0]
30006c62:	f809 3004 	strb.w	r3, [r9, r4]
30006c66:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006c68:	1e5c      	subs	r4, r3, #1
30006c6a:	d04a      	beq.n	30006d02 <password_hash_check+0x136>
30006c6c:	2200      	movs	r2, #0
30006c6e:	f10a 33ff 	add.w	r3, sl, #4294967295
30006c72:	4611      	mov	r1, r2
30006c74:	e00b      	b.n	30006c8e <password_hash_check+0xc2>
30006c76:	f10b 3bff 	add.w	fp, fp, #4294967295
30006c7a:	2100      	movs	r1, #0
30006c7c:	1c98      	adds	r0, r3, #2
30006c7e:	42a2      	cmp	r2, r4
30006c80:	7059      	strb	r1, [r3, #1]
30006c82:	f103 0301 	add.w	r3, r3, #1
30006c86:	4611      	mov	r1, r2
30006c88:	eba0 000a 	sub.w	r0, r0, sl
30006c8c:	d20a      	bcs.n	30006ca4 <password_hash_check+0xd8>
30006c8e:	1868      	adds	r0, r5, r1
30006c90:	1c56      	adds	r6, r2, #1
30006c92:	5c69      	ldrb	r1, [r5, r1]
30006c94:	3202      	adds	r2, #2
30006c96:	7840      	ldrb	r0, [r0, #1]
30006c98:	2830      	cmp	r0, #48	; 0x30
30006c9a:	d101      	bne.n	30006ca0 <password_hash_check+0xd4>
30006c9c:	295c      	cmp	r1, #92	; 0x5c
30006c9e:	d0ea      	beq.n	30006c76 <password_hash_check+0xaa>
30006ca0:	4632      	mov	r2, r6
30006ca2:	e7eb      	b.n	30006c7c <password_hash_check+0xb0>
30006ca4:	4425      	add	r5, r4
30006ca6:	782b      	ldrb	r3, [r5, #0]
30006ca8:	f107 0408 	add.w	r4, r7, #8
30006cac:	2520      	movs	r5, #32
30006cae:	4641      	mov	r1, r8
30006cb0:	f80a 3000 	strb.w	r3, [sl, r0]
30006cb4:	4652      	mov	r2, sl
30006cb6:	687b      	ldr	r3, [r7, #4]
30006cb8:	4648      	mov	r0, r9
30006cba:	9402      	str	r4, [sp, #8]
30006cbc:	9300      	str	r3, [sp, #0]
30006cbe:	465b      	mov	r3, fp
30006cc0:	9501      	str	r5, [sp, #4]
30006cc2:	f7ff fe95 	bl	300069f0 <BOOT_RAM_KeyDeriveFunc>
30006cc6:	4620      	mov	r0, r4
30006cc8:	462a      	mov	r2, r5
30006cca:	4910      	ldr	r1, [pc, #64]	; (30006d0c <password_hash_check+0x140>)
30006ccc:	f005 fc7c 	bl	3000c5c8 <____wrap_memcmp_veneer>
30006cd0:	4604      	mov	r4, r0
30006cd2:	b968      	cbnz	r0, 30006cf0 <password_hash_check+0x124>
30006cd4:	480e      	ldr	r0, [pc, #56]	; (30006d10 <password_hash_check+0x144>)
30006cd6:	f005 fcd7 	bl	3000c688 <__DiagPrintf_veneer>
30006cda:	462a      	mov	r2, r5
30006cdc:	4621      	mov	r1, r4
30006cde:	f107 0008 	add.w	r0, r7, #8
30006ce2:	4b09      	ldr	r3, [pc, #36]	; (30006d08 <password_hash_check+0x13c>)
30006ce4:	4798      	blx	r3
30006ce6:	4620      	mov	r0, r4
30006ce8:	372c      	adds	r7, #44	; 0x2c
30006cea:	46bd      	mov	sp, r7
30006cec:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006cf0:	2401      	movs	r4, #1
30006cf2:	4808      	ldr	r0, [pc, #32]	; (30006d14 <password_hash_check+0x148>)
30006cf4:	f005 fcc8 	bl	3000c688 <__DiagPrintf_veneer>
30006cf8:	372c      	adds	r7, #44	; 0x2c
30006cfa:	4620      	mov	r0, r4
30006cfc:	46bd      	mov	sp, r7
30006cfe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d02:	4620      	mov	r0, r4
30006d04:	e7cf      	b.n	30006ca6 <password_hash_check+0xda>
30006d06:	bf00      	nop
30006d08:	00012be5 	.word	0x00012be5
30006d0c:	3000efb8 	.word	0x3000efb8
30006d10:	3000d230 	.word	0x3000d230
30006d14:	3000d218 	.word	0x3000d218

30006d18 <BOOT_Share_Memory_Patch>:
30006d18:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30006d1c:	4817      	ldr	r0, [pc, #92]	; (30006d7c <BOOT_Share_Memory_Patch+0x64>)
30006d1e:	4918      	ldr	r1, [pc, #96]	; (30006d80 <BOOT_Share_Memory_Patch+0x68>)
30006d20:	b430      	push	{r4, r5}
30006d22:	6b13      	ldr	r3, [r2, #48]	; 0x30
30006d24:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30006d28:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30006d2c:	6313      	str	r3, [r2, #48]	; 0x30
30006d2e:	6823      	ldr	r3, [r4, #0]
30006d30:	6804      	ldr	r4, [r0, #0]
30006d32:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006d36:	6809      	ldr	r1, [r1, #0]
30006d38:	4423      	add	r3, r4
30006d3a:	6805      	ldr	r5, [r0, #0]
30006d3c:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30006d40:	440b      	add	r3, r1
30006d42:	4910      	ldr	r1, [pc, #64]	; (30006d84 <BOOT_Share_Memory_Patch+0x6c>)
30006d44:	6808      	ldr	r0, [r1, #0]
30006d46:	442b      	add	r3, r5
30006d48:	6b11      	ldr	r1, [r2, #48]	; 0x30
30006d4a:	4403      	add	r3, r0
30006d4c:	480e      	ldr	r0, [pc, #56]	; (30006d88 <BOOT_Share_Memory_Patch+0x70>)
30006d4e:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30006d52:	6311      	str	r1, [r2, #48]	; 0x30
30006d54:	6821      	ldr	r1, [r4, #0]
30006d56:	4a0d      	ldr	r2, [pc, #52]	; (30006d8c <BOOT_Share_Memory_Patch+0x74>)
30006d58:	6804      	ldr	r4, [r0, #0]
30006d5a:	440b      	add	r3, r1
30006d5c:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006d60:	6811      	ldr	r1, [r2, #0]
30006d62:	4423      	add	r3, r4
30006d64:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30006d68:	6800      	ldr	r0, [r0, #0]
30006d6a:	440b      	add	r3, r1
30006d6c:	6812      	ldr	r2, [r2, #0]
30006d6e:	4908      	ldr	r1, [pc, #32]	; (30006d90 <BOOT_Share_Memory_Patch+0x78>)
30006d70:	4403      	add	r3, r0
30006d72:	6808      	ldr	r0, [r1, #0]
30006d74:	4413      	add	r3, r2
30006d76:	bc30      	pop	{r4, r5}
30006d78:	4418      	add	r0, r3
30006d7a:	4770      	bx	lr
30006d7c:	2200c000 	.word	0x2200c000
30006d80:	22018000 	.word	0x22018000
30006d84:	22100000 	.word	0x22100000
30006d88:	20010000 	.word	0x20010000
30006d8c:	20020000 	.word	0x20020000
30006d90:	23010000 	.word	0x23010000

30006d94 <BOOT_Image1>:
30006d94:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d98:	4b8e      	ldr	r3, [pc, #568]	; (30006fd4 <BOOT_Image1+0x240>)
30006d9a:	b089      	sub	sp, #36	; 0x24
30006d9c:	4798      	blx	r3
30006d9e:	2800      	cmp	r0, #0
30006da0:	f040 82dc 	bne.w	3000735c <BOOT_Image1+0x5c8>
30006da4:	488c      	ldr	r0, [pc, #560]	; (30006fd8 <BOOT_Image1+0x244>)
30006da6:	f005 fc6f 	bl	3000c688 <__DiagPrintf_veneer>
30006daa:	498c      	ldr	r1, [pc, #560]	; (30006fdc <BOOT_Image1+0x248>)
30006dac:	488c      	ldr	r0, [pc, #560]	; (30006fe0 <BOOT_Image1+0x24c>)
30006dae:	f005 fc6b 	bl	3000c688 <__DiagPrintf_veneer>
30006db2:	488c      	ldr	r0, [pc, #560]	; (30006fe4 <BOOT_Image1+0x250>)
30006db4:	4a8c      	ldr	r2, [pc, #560]	; (30006fe8 <BOOT_Image1+0x254>)
30006db6:	2100      	movs	r1, #0
30006db8:	4b8c      	ldr	r3, [pc, #560]	; (30006fec <BOOT_Image1+0x258>)
30006dba:	1a12      	subs	r2, r2, r0
30006dbc:	4798      	blx	r3
30006dbe:	4a8c      	ldr	r2, [pc, #560]	; (30006ff0 <BOOT_Image1+0x25c>)
30006dc0:	498c      	ldr	r1, [pc, #560]	; (30006ff4 <BOOT_Image1+0x260>)
30006dc2:	8c13      	ldrh	r3, [r2, #32]
30006dc4:	b29b      	uxth	r3, r3
30006dc6:	8413      	strh	r3, [r2, #32]
30006dc8:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006dcc:	f002 fc20 	bl	30009610 <BOOT_Reason>
30006dd0:	2800      	cmp	r0, #0
30006dd2:	f000 8328 	beq.w	30007426 <BOOT_Image1+0x692>
30006dd6:	4d88      	ldr	r5, [pc, #544]	; (30006ff8 <BOOT_Image1+0x264>)
30006dd8:	4b88      	ldr	r3, [pc, #544]	; (30006ffc <BOOT_Image1+0x268>)
30006dda:	4798      	blx	r3
30006ddc:	782c      	ldrb	r4, [r5, #0]
30006dde:	2c00      	cmp	r4, #0
30006de0:	f000 82b2 	beq.w	30007348 <BOOT_Image1+0x5b4>
30006de4:	2000      	movs	r0, #0
30006de6:	4b86      	ldr	r3, [pc, #536]	; (30007000 <BOOT_Image1+0x26c>)
30006de8:	4798      	blx	r3
30006dea:	4b86      	ldr	r3, [pc, #536]	; (30007004 <BOOT_Image1+0x270>)
30006dec:	2001      	movs	r0, #1
30006dee:	4798      	blx	r3
30006df0:	4c80      	ldr	r4, [pc, #512]	; (30006ff4 <BOOT_Image1+0x260>)
30006df2:	f7ff fca5 	bl	30006740 <BOOT_SOC_ClkSet>
30006df6:	f241 070a 	movw	r7, #4106	; 0x100a
30006dfa:	2028      	movs	r0, #40	; 0x28
30006dfc:	8823      	ldrh	r3, [r4, #0]
30006dfe:	4e82      	ldr	r6, [pc, #520]	; (30007008 <BOOT_Image1+0x274>)
30006e00:	b29b      	uxth	r3, r3
30006e02:	431f      	orrs	r7, r3
30006e04:	4b81      	ldr	r3, [pc, #516]	; (3000700c <BOOT_Image1+0x278>)
30006e06:	8027      	strh	r7, [r4, #0]
30006e08:	4798      	blx	r3
30006e0a:	88a3      	ldrh	r3, [r4, #4]
30006e0c:	b29b      	uxth	r3, r3
30006e0e:	ea23 0307 	bic.w	r3, r3, r7
30006e12:	80a3      	strh	r3, [r4, #4]
30006e14:	8863      	ldrh	r3, [r4, #2]
30006e16:	8067      	strh	r7, [r4, #2]
30006e18:	47b0      	blx	r6
30006e1a:	2802      	cmp	r0, #2
30006e1c:	f000 82ea 	beq.w	300073f4 <BOOT_Image1+0x660>
30006e20:	4b7b      	ldr	r3, [pc, #492]	; (30007010 <BOOT_Image1+0x27c>)
30006e22:	2401      	movs	r4, #1
30006e24:	497b      	ldr	r1, [pc, #492]	; (30007014 <BOOT_Image1+0x280>)
30006e26:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006e2a:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30006e2e:	4622      	mov	r2, r4
30006e30:	4f79      	ldr	r7, [pc, #484]	; (30007018 <BOOT_Image1+0x284>)
30006e32:	47b8      	blx	r7
30006e34:	4622      	mov	r2, r4
30006e36:	4979      	ldr	r1, [pc, #484]	; (3000701c <BOOT_Image1+0x288>)
30006e38:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006e3c:	47b8      	blx	r7
30006e3e:	4a6c      	ldr	r2, [pc, #432]	; (30006ff0 <BOOT_Image1+0x25c>)
30006e40:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006e44:	f043 0303 	orr.w	r3, r3, #3
30006e48:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006e4c:	4b74      	ldr	r3, [pc, #464]	; (30007020 <BOOT_Image1+0x28c>)
30006e4e:	4798      	blx	r3
30006e50:	4a67      	ldr	r2, [pc, #412]	; (30006ff0 <BOOT_Image1+0x25c>)
30006e52:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006e56:	f023 0303 	bic.w	r3, r3, #3
30006e5a:	b108      	cbz	r0, 30006e60 <BOOT_Image1+0xcc>
30006e5c:	f043 0302 	orr.w	r3, r3, #2
30006e60:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006e64:	4b6f      	ldr	r3, [pc, #444]	; (30007024 <BOOT_Image1+0x290>)
30006e66:	781b      	ldrb	r3, [r3, #0]
30006e68:	2b00      	cmp	r3, #0
30006e6a:	f040 82e9 	bne.w	30007440 <BOOT_Image1+0x6ac>
30006e6e:	4c6e      	ldr	r4, [pc, #440]	; (30007028 <BOOT_Image1+0x294>)
30006e70:	2201      	movs	r2, #1
30006e72:	2102      	movs	r1, #2
30006e74:	486d      	ldr	r0, [pc, #436]	; (3000702c <BOOT_Image1+0x298>)
30006e76:	47a0      	blx	r4
30006e78:	4c6d      	ldr	r4, [pc, #436]	; (30007030 <BOOT_Image1+0x29c>)
30006e7a:	68a3      	ldr	r3, [r4, #8]
30006e7c:	079f      	lsls	r7, r3, #30
30006e7e:	f100 82d9 	bmi.w	30007434 <BOOT_Image1+0x6a0>
30006e82:	496c      	ldr	r1, [pc, #432]	; (30007034 <BOOT_Image1+0x2a0>)
30006e84:	e841 f100 	tt	r1, r1
30006e88:	68a3      	ldr	r3, [r4, #8]
30006e8a:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006e8e:	0798      	lsls	r0, r3, #30
30006e90:	f100 82c5 	bmi.w	3000741e <BOOT_Image1+0x68a>
30006e94:	4b68      	ldr	r3, [pc, #416]	; (30007038 <BOOT_Image1+0x2a4>)
30006e96:	4f69      	ldr	r7, [pc, #420]	; (3000703c <BOOT_Image1+0x2a8>)
30006e98:	681b      	ldr	r3, [r3, #0]
30006e9a:	4798      	blx	r3
30006e9c:	f7fe fa56 	bl	3000534c <BOOT_RccConfig>
30006ea0:	f7fe fac0 	bl	30005424 <BOOT_CACHEWRR_Set>
30006ea4:	4b66      	ldr	r3, [pc, #408]	; (30007040 <BOOT_Image1+0x2ac>)
30006ea6:	6818      	ldr	r0, [r3, #0]
30006ea8:	f7fe faea 	bl	30005480 <BOOT_TCMSet>
30006eac:	4b65      	ldr	r3, [pc, #404]	; (30007044 <BOOT_Image1+0x2b0>)
30006eae:	695b      	ldr	r3, [r3, #20]
30006eb0:	03da      	lsls	r2, r3, #15
30006eb2:	d513      	bpl.n	30006edc <BOOT_Image1+0x148>
30006eb4:	f017 011f 	ands.w	r1, r7, #31
30006eb8:	f040 8261 	bne.w	3000737e <BOOT_Image1+0x5ea>
30006ebc:	463b      	mov	r3, r7
30006ebe:	2174      	movs	r1, #116	; 0x74
30006ec0:	f3bf 8f4f 	dsb	sy
30006ec4:	485f      	ldr	r0, [pc, #380]	; (30007044 <BOOT_Image1+0x2b0>)
30006ec6:	4419      	add	r1, r3
30006ec8:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006ecc:	3320      	adds	r3, #32
30006ece:	1aca      	subs	r2, r1, r3
30006ed0:	2a00      	cmp	r2, #0
30006ed2:	dcf9      	bgt.n	30006ec8 <BOOT_Image1+0x134>
30006ed4:	f3bf 8f4f 	dsb	sy
30006ed8:	f3bf 8f6f 	isb	sy
30006edc:	4b44      	ldr	r3, [pc, #272]	; (30006ff0 <BOOT_Image1+0x25c>)
30006ede:	2200      	movs	r2, #0
30006ee0:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30006ee4:	4b4a      	ldr	r3, [pc, #296]	; (30007010 <BOOT_Image1+0x27c>)
30006ee6:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30006eea:	f7fc fbe9 	bl	300036c0 <flash_highspeed_setup>
30006eee:	47b0      	blx	r6
30006ef0:	2802      	cmp	r0, #2
30006ef2:	f000 82c4 	beq.w	3000747e <BOOT_Image1+0x6ea>
30006ef6:	782b      	ldrb	r3, [r5, #0]
30006ef8:	2b00      	cmp	r3, #0
30006efa:	f040 8233 	bne.w	30007364 <BOOT_Image1+0x5d0>
30006efe:	68a3      	ldr	r3, [r4, #8]
30006f00:	079b      	lsls	r3, r3, #30
30006f02:	f100 836a 	bmi.w	300075da <BOOT_Image1+0x846>
30006f06:	f7fe fc53 	bl	300057b0 <BOOT_DDR_Init>
30006f0a:	4a39      	ldr	r2, [pc, #228]	; (30006ff0 <BOOT_Image1+0x25c>)
30006f0c:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30006f10:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30006f14:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30006f18:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30006f1c:	4b34      	ldr	r3, [pc, #208]	; (30006ff0 <BOOT_Image1+0x25c>)
30006f1e:	2500      	movs	r5, #0
30006f20:	f8c3 52d0 	str.w	r5, [r3, #720]	; 0x2d0
30006f24:	f8c3 52d4 	str.w	r5, [r3, #724]	; 0x2d4
30006f28:	f8c3 52d8 	str.w	r5, [r3, #728]	; 0x2d8
30006f2c:	f8c3 50d0 	str.w	r5, [r3, #208]	; 0xd0
30006f30:	f7ff fef2 	bl	30006d18 <BOOT_Share_Memory_Patch>
30006f34:	f10d 0115 	add.w	r1, sp, #21
30006f38:	f44f 7059 	mov.w	r0, #868	; 0x364
30006f3c:	f001 fe2e 	bl	30008b9c <OTP_Read8>
30006f40:	f10d 0117 	add.w	r1, sp, #23
30006f44:	f44f 705a 	mov.w	r0, #872	; 0x368
30006f48:	f88d 5016 	strb.w	r5, [sp, #22]
30006f4c:	f88d 5017 	strb.w	r5, [sp, #23]
30006f50:	f001 fe24 	bl	30008b9c <OTP_Read8>
30006f54:	f10d 0116 	add.w	r1, sp, #22
30006f58:	f240 3069 	movw	r0, #873	; 0x369
30006f5c:	f001 fe1e 	bl	30008b9c <OTP_Read8>
30006f60:	f89d 3015 	ldrb.w	r3, [sp, #21]
30006f64:	f013 0f1e 	tst.w	r3, #30
30006f68:	d10a      	bne.n	30006f80 <BOOT_Image1+0x1ec>
30006f6a:	f89d 2016 	ldrb.w	r2, [sp, #22]
30006f6e:	f89d 3017 	ldrb.w	r3, [sp, #23]
30006f72:	f002 0208 	and.w	r2, r2, #8
30006f76:	f003 0301 	and.w	r3, r3, #1
30006f7a:	4313      	orrs	r3, r2
30006f7c:	f040 8299 	bne.w	300074b2 <BOOT_Image1+0x71e>
30006f80:	4b31      	ldr	r3, [pc, #196]	; (30007048 <BOOT_Image1+0x2b4>)
30006f82:	781b      	ldrb	r3, [r3, #0]
30006f84:	2b00      	cmp	r3, #0
30006f86:	f040 82b5 	bne.w	300074f4 <BOOT_Image1+0x760>
30006f8a:	6823      	ldr	r3, [r4, #0]
30006f8c:	f043 0302 	orr.w	r3, r3, #2
30006f90:	6023      	str	r3, [r4, #0]
30006f92:	4b2e      	ldr	r3, [pc, #184]	; (3000704c <BOOT_Image1+0x2b8>)
30006f94:	4798      	blx	r3
30006f96:	2801      	cmp	r0, #1
30006f98:	f04f 0100 	mov.w	r1, #0
30006f9c:	f04f 0002 	mov.w	r0, #2
30006fa0:	bf0c      	ite	eq
30006fa2:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30006fa6:	4a2a      	ldrne	r2, [pc, #168]	; (30007050 <BOOT_Image1+0x2bc>)
30006fa8:	f000 fdc0 	bl	30007b2c <ymodem_uart_port_init>
30006fac:	4b27      	ldr	r3, [pc, #156]	; (3000704c <BOOT_Image1+0x2b8>)
30006fae:	4798      	blx	r3
30006fb0:	2800      	cmp	r0, #0
30006fb2:	f000 825f 	beq.w	30007474 <BOOT_Image1+0x6e0>
30006fb6:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30006fba:	4826      	ldr	r0, [pc, #152]	; (30007054 <BOOT_Image1+0x2c0>)
30006fbc:	f005 fb64 	bl	3000c688 <__DiagPrintf_veneer>
30006fc0:	4825      	ldr	r0, [pc, #148]	; (30007058 <BOOT_Image1+0x2c4>)
30006fc2:	2564      	movs	r5, #100	; 0x64
30006fc4:	f005 fb88 	bl	3000c6d8 <____wrap_strlen_veneer>
30006fc8:	4601      	mov	r1, r0
30006fca:	4823      	ldr	r0, [pc, #140]	; (30007058 <BOOT_Image1+0x2c4>)
30006fcc:	4c23      	ldr	r4, [pc, #140]	; (3000705c <BOOT_Image1+0x2c8>)
30006fce:	f000 fe07 	bl	30007be0 <ymodem_uart_putdata>
30006fd2:	e048      	b.n	30007066 <BOOT_Image1+0x2d2>
30006fd4:	0000c149 	.word	0x0000c149
30006fd8:	3000d24c 	.word	0x3000d24c
30006fdc:	3000e700 	.word	0x3000e700
30006fe0:	3000d270 	.word	0x3000d270
30006fe4:	3000ff60 	.word	0x3000ff60
30006fe8:	30010404 	.word	0x30010404
30006fec:	00012be5 	.word	0x00012be5
30006ff0:	42008000 	.word	0x42008000
30006ff4:	42008200 	.word	0x42008200
30006ff8:	3000e9ea 	.word	0x3000e9ea
30006ffc:	3000b809 	.word	0x3000b809
30007000:	3000b679 	.word	0x3000b679
30007004:	3000b599 	.word	0x3000b599
30007008:	30007ddd 	.word	0x30007ddd
3000700c:	00009b2d 	.word	0x00009b2d
30007010:	23020000 	.word	0x23020000
30007014:	40080000 	.word	0x40080000
30007018:	0000b479 	.word	0x0000b479
3000701c:	40040000 	.word	0x40040000
30007020:	30009ead 	.word	0x30009ead
30007024:	3000e9e9 	.word	0x3000e9e9
30007028:	0000aab9 	.word	0x0000aab9
3000702c:	4200c000 	.word	0x4200c000
30007030:	2001c00c 	.word	0x2001c00c
30007034:	30006d95 	.word	0x30006d95
30007038:	00033000 	.word	0x00033000
3000703c:	2001c01c 	.word	0x2001c01c
30007040:	3000e9ec 	.word	0x3000e9ec
30007044:	e000ed00 	.word	0xe000ed00
30007048:	3000e9e8 	.word	0x3000e9e8
3000704c:	0000c169 	.word	0x0000c169
30007050:	0016e360 	.word	0x0016e360
30007054:	3000d3d4 	.word	0x3000d3d4
30007058:	3000efd8 	.word	0x3000efd8
3000705c:	00009be5 	.word	0x00009be5
30007060:	3d01      	subs	r5, #1
30007062:	f000 82be 	beq.w	300075e2 <BOOT_Image1+0x84e>
30007066:	2001      	movs	r0, #1
30007068:	47a0      	blx	r4
3000706a:	f000 fd9f 	bl	30007bac <ymodem_uart_readable>
3000706e:	2800      	cmp	r0, #0
30007070:	d0f6      	beq.n	30007060 <BOOT_Image1+0x2cc>
30007072:	f000 fd9b 	bl	30007bac <ymodem_uart_readable>
30007076:	2800      	cmp	r0, #0
30007078:	f000 82b9 	beq.w	300075ee <BOOT_Image1+0x85a>
3000707c:	2301      	movs	r3, #1
3000707e:	9303      	str	r3, [sp, #12]
30007080:	2001      	movs	r0, #1
30007082:	47a0      	blx	r4
30007084:	2108      	movs	r1, #8
30007086:	48c0      	ldr	r0, [pc, #768]	; (30007388 <BOOT_Image1+0x5f4>)
30007088:	f000 fdb8 	bl	30007bfc <ymodem_uart_getdata>
3000708c:	49be      	ldr	r1, [pc, #760]	; (30007388 <BOOT_Image1+0x5f4>)
3000708e:	2208      	movs	r2, #8
30007090:	4bbe      	ldr	r3, [pc, #760]	; (3000738c <BOOT_Image1+0x5f8>)
30007092:	f101 0080 	add.w	r0, r1, #128	; 0x80
30007096:	4798      	blx	r3
30007098:	4605      	mov	r5, r0
3000709a:	2800      	cmp	r0, #0
3000709c:	f040 8368 	bne.w	30007770 <BOOT_Image1+0x9dc>
300070a0:	f000 fd84 	bl	30007bac <ymodem_uart_readable>
300070a4:	2800      	cmp	r0, #0
300070a6:	d0fb      	beq.n	300070a0 <BOOT_Image1+0x30c>
300070a8:	2101      	movs	r1, #1
300070aa:	48b9      	ldr	r0, [pc, #740]	; (30007390 <BOOT_Image1+0x5fc>)
300070ac:	f000 fda6 	bl	30007bfc <ymodem_uart_getdata>
300070b0:	f000 fd7c 	bl	30007bac <ymodem_uart_readable>
300070b4:	2800      	cmp	r0, #0
300070b6:	d0fb      	beq.n	300070b0 <BOOT_Image1+0x31c>
300070b8:	4fb6      	ldr	r7, [pc, #728]	; (30007394 <BOOT_Image1+0x600>)
300070ba:	2101      	movs	r1, #1
300070bc:	f207 4051 	addw	r0, r7, #1105	; 0x451
300070c0:	f000 fd9c 	bl	30007bfc <ymodem_uart_getdata>
300070c4:	f897 2451 	ldrb.w	r2, [r7, #1105]	; 0x451
300070c8:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
300070cc:	429a      	cmp	r2, r3
300070ce:	f000 8360 	beq.w	30007792 <BOOT_Image1+0x9fe>
300070d2:	f207 4952 	addw	r9, r7, #1106	; 0x452
300070d6:	2600      	movs	r6, #0
300070d8:	46b0      	mov	r8, r6
300070da:	3601      	adds	r6, #1
300070dc:	f000 fd66 	bl	30007bac <ymodem_uart_readable>
300070e0:	2800      	cmp	r0, #0
300070e2:	d0fb      	beq.n	300070dc <BOOT_Image1+0x348>
300070e4:	4648      	mov	r0, r9
300070e6:	2101      	movs	r1, #1
300070e8:	f000 fd88 	bl	30007bfc <ymodem_uart_getdata>
300070ec:	f899 2000 	ldrb.w	r2, [r9]
300070f0:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
300070f4:	f108 080b 	add.w	r8, r8, #11
300070f8:	f109 0901 	add.w	r9, r9, #1
300070fc:	46b3      	mov	fp, r6
300070fe:	429a      	cmp	r2, r3
30007100:	d1ea      	bne.n	300070d8 <BOOT_Image1+0x344>
30007102:	2e00      	cmp	r6, #0
30007104:	dd13      	ble.n	3000712e <BOOT_Image1+0x39a>
30007106:	f507 618a 	add.w	r1, r7, #1104	; 0x450
3000710a:	2300      	movs	r3, #0
3000710c:	1988      	adds	r0, r1, r6
3000710e:	e001      	b.n	30007114 <BOOT_Image1+0x380>
30007110:	4288      	cmp	r0, r1
30007112:	d00d      	beq.n	30007130 <BOOT_Image1+0x39c>
30007114:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30007118:	eb03 0383 	add.w	r3, r3, r3, lsl #2
3000711c:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30007120:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30007124:	f1bc 0f09 	cmp.w	ip, #9
30007128:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
3000712c:	d9f0      	bls.n	30007110 <BOOT_Image1+0x37c>
3000712e:	2300      	movs	r3, #0
30007130:	f8c7 34d4 	str.w	r3, [r7, #1236]	; 0x4d4
30007134:	f000 fd3a 	bl	30007bac <ymodem_uart_readable>
30007138:	2800      	cmp	r0, #0
3000713a:	d0fb      	beq.n	30007134 <BOOT_Image1+0x3a0>
3000713c:	4992      	ldr	r1, [pc, #584]	; (30007388 <BOOT_Image1+0x5f4>)
3000713e:	460b      	mov	r3, r1
30007140:	2101      	movs	r1, #1
30007142:	eb03 0008 	add.w	r0, r3, r8
30007146:	9302      	str	r3, [sp, #8]
30007148:	f000 fd58 	bl	30007bfc <ymodem_uart_getdata>
3000714c:	9902      	ldr	r1, [sp, #8]
3000714e:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30007152:	f811 2008 	ldrb.w	r2, [r1, r8]
30007156:	429a      	cmp	r2, r3
30007158:	f000 8317 	beq.w	3000778a <BOOT_Image1+0x9f6>
3000715c:	f10b 080b 	add.w	r8, fp, #11
30007160:	f04f 0900 	mov.w	r9, #0
30007164:	4488      	add	r8, r1
30007166:	f109 0901 	add.w	r9, r9, #1
3000716a:	f000 fd1f 	bl	30007bac <ymodem_uart_readable>
3000716e:	2800      	cmp	r0, #0
30007170:	d0fb      	beq.n	3000716a <BOOT_Image1+0x3d6>
30007172:	4640      	mov	r0, r8
30007174:	2101      	movs	r1, #1
30007176:	f000 fd41 	bl	30007bfc <ymodem_uart_getdata>
3000717a:	f898 2000 	ldrb.w	r2, [r8]
3000717e:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30007182:	f108 0801 	add.w	r8, r8, #1
30007186:	eb09 0a0b 	add.w	sl, r9, fp
3000718a:	429a      	cmp	r2, r3
3000718c:	d1eb      	bne.n	30007166 <BOOT_Image1+0x3d2>
3000718e:	f000 fd0d 	bl	30007bac <ymodem_uart_readable>
30007192:	2800      	cmp	r0, #0
30007194:	d0fb      	beq.n	3000718e <BOOT_Image1+0x3fa>
30007196:	9b02      	ldr	r3, [sp, #8]
30007198:	f10a 080b 	add.w	r8, sl, #11
3000719c:	2101      	movs	r1, #1
3000719e:	eb03 0008 	add.w	r0, r3, r8
300071a2:	f000 fd2b 	bl	30007bfc <ymodem_uart_getdata>
300071a6:	9902      	ldr	r1, [sp, #8]
300071a8:	f897 24d1 	ldrb.w	r2, [r7, #1233]	; 0x4d1
300071ac:	f811 3008 	ldrb.w	r3, [r1, r8]
300071b0:	429a      	cmp	r2, r3
300071b2:	f000 82e8 	beq.w	30007786 <BOOT_Image1+0x9f2>
300071b6:	f10a 080c 	add.w	r8, sl, #12
300071ba:	4488      	add	r8, r1
300071bc:	3501      	adds	r5, #1
300071be:	f000 fcf5 	bl	30007bac <ymodem_uart_readable>
300071c2:	2800      	cmp	r0, #0
300071c4:	d0fb      	beq.n	300071be <BOOT_Image1+0x42a>
300071c6:	4640      	mov	r0, r8
300071c8:	2101      	movs	r1, #1
300071ca:	f000 fd17 	bl	30007bfc <ymodem_uart_getdata>
300071ce:	f898 2000 	ldrb.w	r2, [r8]
300071d2:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
300071d6:	f108 0801 	add.w	r8, r8, #1
300071da:	eb05 0b0a 	add.w	fp, r5, sl
300071de:	429a      	cmp	r2, r3
300071e0:	d1ec      	bne.n	300071bc <BOOT_Image1+0x428>
300071e2:	9b02      	ldr	r3, [sp, #8]
300071e4:	f10b 080c 	add.w	r8, fp, #12
300071e8:	2101      	movs	r1, #1
300071ea:	eb03 0008 	add.w	r0, r3, r8
300071ee:	f000 fd05 	bl	30007bfc <ymodem_uart_getdata>
300071f2:	9b02      	ldr	r3, [sp, #8]
300071f4:	f813 1008 	ldrb.w	r1, [r3, r8]
300071f8:	290a      	cmp	r1, #10
300071fa:	f8c7 14d8 	str.w	r1, [r7, #1240]	; 0x4d8
300071fe:	f200 8266 	bhi.w	300076ce <BOOT_Image1+0x93a>
30007202:	9b02      	ldr	r3, [sp, #8]
30007204:	f10b 000d 	add.w	r0, fp, #13
30007208:	2101      	movs	r1, #1
3000720a:	f10b 080e 	add.w	r8, fp, #14
3000720e:	4418      	add	r0, r3
30007210:	f000 fcf4 	bl	30007bfc <ymodem_uart_getdata>
30007214:	9b02      	ldr	r3, [sp, #8]
30007216:	2102      	movs	r1, #2
30007218:	4498      	add	r8, r3
3000721a:	4640      	mov	r0, r8
3000721c:	f000 fcee 	bl	30007bfc <ymodem_uart_getdata>
30007220:	4641      	mov	r1, r8
30007222:	2202      	movs	r2, #2
30007224:	485c      	ldr	r0, [pc, #368]	; (30007398 <BOOT_Image1+0x604>)
30007226:	4b59      	ldr	r3, [pc, #356]	; (3000738c <BOOT_Image1+0x5f8>)
30007228:	4798      	blx	r3
3000722a:	2800      	cmp	r0, #0
3000722c:	f040 82a7 	bne.w	3000777e <BOOT_Image1+0x9ea>
30007230:	9b02      	ldr	r3, [sp, #8]
30007232:	f10b 0010 	add.w	r0, fp, #16
30007236:	2101      	movs	r1, #1
30007238:	f10b 0811 	add.w	r8, fp, #17
3000723c:	4418      	add	r0, r3
3000723e:	f000 fcdd 	bl	30007bfc <ymodem_uart_getdata>
30007242:	4b56      	ldr	r3, [pc, #344]	; (3000739c <BOOT_Image1+0x608>)
30007244:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
30007248:	f103 0228 	add.w	r2, r3, #40	; 0x28
3000724c:	f103 0130 	add.w	r1, r3, #48	; 0x30
30007250:	f7fd fd4c 	bl	30004cec <hotfix_get_addr_size>
30007254:	9b02      	ldr	r3, [sp, #8]
30007256:	2102      	movs	r1, #2
30007258:	4498      	add	r8, r3
3000725a:	4640      	mov	r0, r8
3000725c:	f000 fcce 	bl	30007bfc <ymodem_uart_getdata>
30007260:	4641      	mov	r1, r8
30007262:	2202      	movs	r2, #2
30007264:	484e      	ldr	r0, [pc, #312]	; (300073a0 <BOOT_Image1+0x60c>)
30007266:	4b49      	ldr	r3, [pc, #292]	; (3000738c <BOOT_Image1+0x5f8>)
30007268:	4798      	blx	r3
3000726a:	2800      	cmp	r0, #0
3000726c:	f000 8153 	beq.w	30007516 <BOOT_Image1+0x782>
30007270:	9b03      	ldr	r3, [sp, #12]
30007272:	2b00      	cmp	r3, #0
30007274:	f000 819a 	beq.w	300075ac <BOOT_Image1+0x818>
30007278:	f004 ff9e 	bl	3000c1b8 <BOOT_VerCheck>
3000727c:	f7fe fc96 	bl	30005bac <BOOT_LoadImages>
30007280:	2800      	cmp	r0, #0
30007282:	f000 8141 	beq.w	30007508 <BOOT_Image1+0x774>
30007286:	f7ff fa19 	bl	300066bc <BOOT_Enable_KM0>
3000728a:	2200      	movs	r2, #0
3000728c:	2101      	movs	r1, #1
3000728e:	4b45      	ldr	r3, [pc, #276]	; (300073a4 <BOOT_Image1+0x610>)
30007290:	4845      	ldr	r0, [pc, #276]	; (300073a8 <BOOT_Image1+0x614>)
30007292:	4798      	blx	r3
30007294:	4b45      	ldr	r3, [pc, #276]	; (300073ac <BOOT_Image1+0x618>)
30007296:	2008      	movs	r0, #8
30007298:	4798      	blx	r3
3000729a:	f7fd fc39 	bl	30004b10 <BOOT_RAM_TZCfg>
3000729e:	4b44      	ldr	r3, [pc, #272]	; (300073b0 <BOOT_Image1+0x61c>)
300072a0:	781b      	ldrb	r3, [r3, #0]
300072a2:	2b01      	cmp	r3, #1
300072a4:	f000 8203 	beq.w	300076ae <BOOT_Image1+0x91a>
300072a8:	4b42      	ldr	r3, [pc, #264]	; (300073b4 <BOOT_Image1+0x620>)
300072aa:	781b      	ldrb	r3, [r3, #0]
300072ac:	2b00      	cmp	r3, #0
300072ae:	f000 8184 	beq.w	300075ba <BOOT_Image1+0x826>
300072b2:	f7ff fb33 	bl	3000691c <BOOT_Enable_AP>
300072b6:	4a40      	ldr	r2, [pc, #256]	; (300073b8 <BOOT_Image1+0x624>)
300072b8:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
300072bc:	f043 0306 	orr.w	r3, r3, #6
300072c0:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300072c4:	4b3a      	ldr	r3, [pc, #232]	; (300073b0 <BOOT_Image1+0x61c>)
300072c6:	781b      	ldrb	r3, [r3, #0]
300072c8:	2b01      	cmp	r3, #1
300072ca:	f000 81f8 	beq.w	300076be <BOOT_Image1+0x92a>
300072ce:	483a      	ldr	r0, [pc, #232]	; (300073b8 <BOOT_Image1+0x624>)
300072d0:	493a      	ldr	r1, [pc, #232]	; (300073bc <BOOT_Image1+0x628>)
300072d2:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
300072d6:	4a3a      	ldr	r2, [pc, #232]	; (300073c0 <BOOT_Image1+0x62c>)
300072d8:	f043 0301 	orr.w	r3, r3, #1
300072dc:	4d39      	ldr	r5, [pc, #228]	; (300073c4 <BOOT_Image1+0x630>)
300072de:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
300072e2:	688c      	ldr	r4, [r1, #8]
300072e4:	680b      	ldr	r3, [r1, #0]
300072e6:	4938      	ldr	r1, [pc, #224]	; (300073c8 <BOOT_Image1+0x634>)
300072e8:	6063      	str	r3, [r4, #4]
300072ea:	4b38      	ldr	r3, [pc, #224]	; (300073cc <BOOT_Image1+0x638>)
300072ec:	6094      	str	r4, [r2, #8]
300072ee:	61e9      	str	r1, [r5, #28]
300072f0:	6a59      	ldr	r1, [r3, #36]	; 0x24
300072f2:	4837      	ldr	r0, [pc, #220]	; (300073d0 <BOOT_Image1+0x63c>)
300072f4:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300072f8:	6259      	str	r1, [r3, #36]	; 0x24
300072fa:	6a51      	ldr	r1, [r2, #36]	; 0x24
300072fc:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30007300:	6251      	str	r1, [r2, #36]	; 0x24
30007302:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30007306:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000730a:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000730e:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30007312:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30007316:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000731a:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000731e:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30007322:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30007326:	689b      	ldr	r3, [r3, #8]
30007328:	6183      	str	r3, [r0, #24]
3000732a:	f7fe fbc7 	bl	30005abc <BOOT_NVICBackup_HP>
3000732e:	4b29      	ldr	r3, [pc, #164]	; (300073d4 <BOOT_Image1+0x640>)
30007330:	f383 8888 	msr	MSP_NS, r3
30007334:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30007338:	f383 8809 	msr	PSP, r3
3000733c:	6860      	ldr	r0, [r4, #4]
3000733e:	f7fd ffb5 	bl	300052ac <BOOT_NsStart>
30007342:	b009      	add	sp, #36	; 0x24
30007344:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007348:	4b23      	ldr	r3, [pc, #140]	; (300073d8 <BOOT_Image1+0x644>)
3000734a:	2001      	movs	r0, #1
3000734c:	4798      	blx	r3
3000734e:	4620      	mov	r0, r4
30007350:	4b22      	ldr	r3, [pc, #136]	; (300073dc <BOOT_Image1+0x648>)
30007352:	4798      	blx	r3
30007354:	4b22      	ldr	r3, [pc, #136]	; (300073e0 <BOOT_Image1+0x64c>)
30007356:	2001      	movs	r0, #1
30007358:	4798      	blx	r3
3000735a:	e549      	b.n	30006df0 <BOOT_Image1+0x5c>
3000735c:	4821      	ldr	r0, [pc, #132]	; (300073e4 <BOOT_Image1+0x650>)
3000735e:	f005 f993 	bl	3000c688 <__DiagPrintf_veneer>
30007362:	e522      	b.n	30006daa <BOOT_Image1+0x16>
30007364:	4c20      	ldr	r4, [pc, #128]	; (300073e8 <BOOT_Image1+0x654>)
30007366:	4e21      	ldr	r6, [pc, #132]	; (300073ec <BOOT_Image1+0x658>)
30007368:	4d21      	ldr	r5, [pc, #132]	; (300073f0 <BOOT_Image1+0x65c>)
3000736a:	4633      	mov	r3, r6
3000736c:	462a      	mov	r2, r5
3000736e:	491f      	ldr	r1, [pc, #124]	; (300073ec <BOOT_Image1+0x658>)
30007370:	2002      	movs	r0, #2
30007372:	f004 fee5 	bl	3000c140 <rtk_log_write>
30007376:	f241 3088 	movw	r0, #5000	; 0x1388
3000737a:	47a0      	blx	r4
3000737c:	e7f5      	b.n	3000736a <BOOT_Image1+0x5d6>
3000737e:	3174      	adds	r1, #116	; 0x74
30007380:	f027 031f 	bic.w	r3, r7, #31
30007384:	e59c      	b.n	30006ec0 <BOOT_Image1+0x12c>
30007386:	bf00      	nop
30007388:	3000eff8 	.word	0x3000eff8
3000738c:	000129bd 	.word	0x000129bd
30007390:	3000f000 	.word	0x3000f000
30007394:	3000ebb0 	.word	0x3000ebb0
30007398:	3000f08c 	.word	0x3000f08c
3000739c:	3000f090 	.word	0x3000f090
300073a0:	3000f0cc 	.word	0x3000f0cc
300073a4:	0000ac6d 	.word	0x0000ac6d
300073a8:	4200c000 	.word	0x4200c000
300073ac:	0000e1d9 	.word	0x0000e1d9
300073b0:	3000e9e8 	.word	0x3000e9e8
300073b4:	3000e9eb 	.word	0x3000e9eb
300073b8:	42008200 	.word	0x42008200
300073bc:	60000020 	.word	0x60000020
300073c0:	e002ed00 	.word	0xe002ed00
300073c4:	30000000 	.word	0x30000000
300073c8:	30004cad 	.word	0x30004cad
300073cc:	e000ed00 	.word	0xe000ed00
300073d0:	30010368 	.word	0x30010368
300073d4:	2001bffc 	.word	0x2001bffc
300073d8:	3000b599 	.word	0x3000b599
300073dc:	3000b7c9 	.word	0x3000b7c9
300073e0:	3000b739 	.word	0x3000b739
300073e4:	3000d25c 	.word	0x3000d25c
300073e8:	00009be5 	.word	0x00009be5
300073ec:	3000c974 	.word	0x3000c974
300073f0:	3000d314 	.word	0x3000d314
300073f4:	6de3      	ldr	r3, [r4, #92]	; 0x5c
300073f6:	2201      	movs	r2, #1
300073f8:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
300073fc:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30007400:	65e3      	str	r3, [r4, #92]	; 0x5c
30007402:	4bb5      	ldr	r3, [pc, #724]	; (300076d8 <BOOT_Image1+0x944>)
30007404:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30007408:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000740c:	4bb3      	ldr	r3, [pc, #716]	; (300076dc <BOOT_Image1+0x948>)
3000740e:	4798      	blx	r3
30007410:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30007414:	f043 0302 	orr.w	r3, r3, #2
30007418:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
3000741c:	e516      	b.n	30006e4c <BOOT_Image1+0xb8>
3000741e:	48b0      	ldr	r0, [pc, #704]	; (300076e0 <BOOT_Image1+0x94c>)
30007420:	f005 f932 	bl	3000c688 <__DiagPrintf_veneer>
30007424:	e536      	b.n	30006e94 <BOOT_Image1+0x100>
30007426:	4601      	mov	r1, r0
30007428:	f44f 7201 	mov.w	r2, #516	; 0x204
3000742c:	48aa      	ldr	r0, [pc, #680]	; (300076d8 <BOOT_Image1+0x944>)
3000742e:	f005 f903 	bl	3000c638 <____wrap_memset_veneer>
30007432:	e4d0      	b.n	30006dd6 <BOOT_Image1+0x42>
30007434:	f3ef 8108 	mrs	r1, MSP
30007438:	48aa      	ldr	r0, [pc, #680]	; (300076e4 <BOOT_Image1+0x950>)
3000743a:	f005 f925 	bl	3000c688 <__DiagPrintf_veneer>
3000743e:	e520      	b.n	30006e82 <BOOT_Image1+0xee>
30007440:	4fa9      	ldr	r7, [pc, #676]	; (300076e8 <BOOT_Image1+0x954>)
30007442:	47b8      	blx	r7
30007444:	4604      	mov	r4, r0
30007446:	47b8      	blx	r7
30007448:	2c02      	cmp	r4, #2
3000744a:	f43f ad10 	beq.w	30006e6e <BOOT_Image1+0xda>
3000744e:	2803      	cmp	r0, #3
30007450:	f43f ad0d 	beq.w	30006e6e <BOOT_Image1+0xda>
30007454:	4ba5      	ldr	r3, [pc, #660]	; (300076ec <BOOT_Image1+0x958>)
30007456:	4798      	blx	r3
30007458:	4ca5      	ldr	r4, [pc, #660]	; (300076f0 <BOOT_Image1+0x95c>)
3000745a:	2200      	movs	r2, #0
3000745c:	2101      	movs	r1, #1
3000745e:	48a5      	ldr	r0, [pc, #660]	; (300076f4 <BOOT_Image1+0x960>)
30007460:	47a0      	blx	r4
30007462:	2101      	movs	r1, #1
30007464:	48a3      	ldr	r0, [pc, #652]	; (300076f4 <BOOT_Image1+0x960>)
30007466:	4ba4      	ldr	r3, [pc, #656]	; (300076f8 <BOOT_Image1+0x964>)
30007468:	4798      	blx	r3
3000746a:	2201      	movs	r2, #1
3000746c:	48a1      	ldr	r0, [pc, #644]	; (300076f4 <BOOT_Image1+0x960>)
3000746e:	4611      	mov	r1, r2
30007470:	47a0      	blx	r4
30007472:	e4fd      	b.n	30006e70 <BOOT_Image1+0xdc>
30007474:	49a1      	ldr	r1, [pc, #644]	; (300076fc <BOOT_Image1+0x968>)
30007476:	48a2      	ldr	r0, [pc, #648]	; (30007700 <BOOT_Image1+0x96c>)
30007478:	f005 f906 	bl	3000c688 <__DiagPrintf_veneer>
3000747c:	e5a0      	b.n	30006fc0 <BOOT_Image1+0x22c>
3000747e:	4ba1      	ldr	r3, [pc, #644]	; (30007704 <BOOT_Image1+0x970>)
30007480:	2004      	movs	r0, #4
30007482:	4aa1      	ldr	r2, [pc, #644]	; (30007708 <BOOT_Image1+0x974>)
30007484:	4619      	mov	r1, r3
30007486:	f004 fe5b 	bl	3000c140 <rtk_log_write>
3000748a:	4ba0      	ldr	r3, [pc, #640]	; (3000770c <BOOT_Image1+0x978>)
3000748c:	4798      	blx	r3
3000748e:	f7fe f9cd 	bl	3000582c <BOOT_PSRAM_Init>
30007492:	7828      	ldrb	r0, [r5, #0]
30007494:	2800      	cmp	r0, #0
30007496:	f47f ad41 	bne.w	30006f1c <BOOT_Image1+0x188>
3000749a:	4b9d      	ldr	r3, [pc, #628]	; (30007710 <BOOT_Image1+0x97c>)
3000749c:	4798      	blx	r3
3000749e:	2800      	cmp	r0, #0
300074a0:	f43f ad3c 	beq.w	30006f1c <BOOT_Image1+0x188>
300074a4:	4b97      	ldr	r3, [pc, #604]	; (30007704 <BOOT_Image1+0x970>)
300074a6:	2003      	movs	r0, #3
300074a8:	4a9a      	ldr	r2, [pc, #616]	; (30007714 <BOOT_Image1+0x980>)
300074aa:	4619      	mov	r1, r3
300074ac:	f004 fe48 	bl	3000c140 <rtk_log_write>
300074b0:	e534      	b.n	30006f1c <BOOT_Image1+0x188>
300074b2:	4899      	ldr	r0, [pc, #612]	; (30007718 <BOOT_Image1+0x984>)
300074b4:	f005 f8e8 	bl	3000c688 <__DiagPrintf_veneer>
300074b8:	f89d 1017 	ldrb.w	r1, [sp, #23]
300074bc:	f89d 3016 	ldrb.w	r3, [sp, #22]
300074c0:	f44f 705a 	mov.w	r0, #872	; 0x368
300074c4:	f001 01fe 	and.w	r1, r1, #254	; 0xfe
300074c8:	f023 0308 	bic.w	r3, r3, #8
300074cc:	f88d 1017 	strb.w	r1, [sp, #23]
300074d0:	f88d 3016 	strb.w	r3, [sp, #22]
300074d4:	f001 fb6c 	bl	30008bb0 <OTP_Write8>
300074d8:	f89d 1016 	ldrb.w	r1, [sp, #22]
300074dc:	f240 3069 	movw	r0, #873	; 0x369
300074e0:	f001 fb66 	bl	30008bb0 <OTP_Write8>
300074e4:	488d      	ldr	r0, [pc, #564]	; (3000771c <BOOT_Image1+0x988>)
300074e6:	f005 f8cf 	bl	3000c688 <__DiagPrintf_veneer>
300074ea:	4b8d      	ldr	r3, [pc, #564]	; (30007720 <BOOT_Image1+0x98c>)
300074ec:	781b      	ldrb	r3, [r3, #0]
300074ee:	2b00      	cmp	r3, #0
300074f0:	f43f ad4b 	beq.w	30006f8a <BOOT_Image1+0x1f6>
300074f4:	4b8b      	ldr	r3, [pc, #556]	; (30007724 <BOOT_Image1+0x990>)
300074f6:	4798      	blx	r3
300074f8:	2800      	cmp	r0, #0
300074fa:	f47f ad46 	bne.w	30006f8a <BOOT_Image1+0x1f6>
300074fe:	68a3      	ldr	r3, [r4, #8]
30007500:	f043 0302 	orr.w	r3, r3, #2
30007504:	60a3      	str	r3, [r4, #8]
30007506:	e540      	b.n	30006f8a <BOOT_Image1+0x1f6>
30007508:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000750c:	47a0      	blx	r4
3000750e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007512:	47a0      	blx	r4
30007514:	e7f8      	b.n	30007508 <BOOT_Image1+0x774>
30007516:	9b02      	ldr	r3, [sp, #8]
30007518:	f10b 0013 	add.w	r0, fp, #19
3000751c:	2101      	movs	r1, #1
3000751e:	f10b 0814 	add.w	r8, fp, #20
30007522:	4418      	add	r0, r3
30007524:	f000 fb6a 	bl	30007bfc <ymodem_uart_getdata>
30007528:	4b7f      	ldr	r3, [pc, #508]	; (30007728 <BOOT_Image1+0x994>)
3000752a:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000752e:	f1a3 0218 	sub.w	r2, r3, #24
30007532:	f1a3 0110 	sub.w	r1, r3, #16
30007536:	f7fd fbd9 	bl	30004cec <hotfix_get_addr_size>
3000753a:	9b02      	ldr	r3, [sp, #8]
3000753c:	2102      	movs	r1, #2
3000753e:	4498      	add	r8, r3
30007540:	4640      	mov	r0, r8
30007542:	f000 fb5b 	bl	30007bfc <ymodem_uart_getdata>
30007546:	4641      	mov	r1, r8
30007548:	2202      	movs	r2, #2
3000754a:	4878      	ldr	r0, [pc, #480]	; (3000772c <BOOT_Image1+0x998>)
3000754c:	4b78      	ldr	r3, [pc, #480]	; (30007730 <BOOT_Image1+0x99c>)
3000754e:	4798      	blx	r3
30007550:	2800      	cmp	r0, #0
30007552:	f47f ae8d 	bne.w	30007270 <BOOT_Image1+0x4dc>
30007556:	9b02      	ldr	r3, [sp, #8]
30007558:	f10b 0016 	add.w	r0, fp, #22
3000755c:	2101      	movs	r1, #1
3000755e:	f10b 0817 	add.w	r8, fp, #23
30007562:	4418      	add	r0, r3
30007564:	f000 fb4a 	bl	30007bfc <ymodem_uart_getdata>
30007568:	4b72      	ldr	r3, [pc, #456]	; (30007734 <BOOT_Image1+0x9a0>)
3000756a:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000756e:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
30007572:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30007576:	f7fd fbb9 	bl	30004cec <hotfix_get_addr_size>
3000757a:	9a02      	ldr	r2, [sp, #8]
3000757c:	2102      	movs	r1, #2
3000757e:	4490      	add	r8, r2
30007580:	4640      	mov	r0, r8
30007582:	f000 fb3b 	bl	30007bfc <ymodem_uart_getdata>
30007586:	4641      	mov	r1, r8
30007588:	2202      	movs	r2, #2
3000758a:	486b      	ldr	r0, [pc, #428]	; (30007738 <BOOT_Image1+0x9a4>)
3000758c:	4b68      	ldr	r3, [pc, #416]	; (30007730 <BOOT_Image1+0x99c>)
3000758e:	4798      	blx	r3
30007590:	2800      	cmp	r0, #0
30007592:	f47f ae6d 	bne.w	30007270 <BOOT_Image1+0x4dc>
30007596:	9b02      	ldr	r3, [sp, #8]
30007598:	f10b 0019 	add.w	r0, fp, #25
3000759c:	2101      	movs	r1, #1
3000759e:	4418      	add	r0, r3
300075a0:	f000 fb2c 	bl	30007bfc <ymodem_uart_getdata>
300075a4:	9b03      	ldr	r3, [sp, #12]
300075a6:	2b00      	cmp	r3, #0
300075a8:	f040 80f9 	bne.w	3000779e <BOOT_Image1+0xa0a>
300075ac:	2002      	movs	r0, #2
300075ae:	f000 fad5 	bl	30007b5c <ymodem_uart_port_deinit>
300075b2:	4862      	ldr	r0, [pc, #392]	; (3000773c <BOOT_Image1+0x9a8>)
300075b4:	f005 f868 	bl	3000c688 <__DiagPrintf_veneer>
300075b8:	e65e      	b.n	30007278 <BOOT_Image1+0x4e4>
300075ba:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300075be:	685a      	ldr	r2, [r3, #4]
300075c0:	f042 0202 	orr.w	r2, r2, #2
300075c4:	605a      	str	r2, [r3, #4]
300075c6:	685a      	ldr	r2, [r3, #4]
300075c8:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300075cc:	605a      	str	r2, [r3, #4]
300075ce:	4a5c      	ldr	r2, [pc, #368]	; (30007740 <BOOT_Image1+0x9ac>)
300075d0:	6f93      	ldr	r3, [r2, #120]	; 0x78
300075d2:	f043 0303 	orr.w	r3, r3, #3
300075d6:	6793      	str	r3, [r2, #120]	; 0x78
300075d8:	e674      	b.n	300072c4 <BOOT_Image1+0x530>
300075da:	485a      	ldr	r0, [pc, #360]	; (30007744 <BOOT_Image1+0x9b0>)
300075dc:	f005 f854 	bl	3000c688 <__DiagPrintf_veneer>
300075e0:	e491      	b.n	30006f06 <BOOT_Image1+0x172>
300075e2:	f000 fae3 	bl	30007bac <ymodem_uart_readable>
300075e6:	2800      	cmp	r0, #0
300075e8:	d0e0      	beq.n	300075ac <BOOT_Image1+0x818>
300075ea:	9503      	str	r5, [sp, #12]
300075ec:	e548      	b.n	30007080 <BOOT_Image1+0x2ec>
300075ee:	4f56      	ldr	r7, [pc, #344]	; (30007748 <BOOT_Image1+0x9b4>)
300075f0:	4605      	mov	r5, r0
300075f2:	f04f 080b 	mov.w	r8, #11
300075f6:	260a      	movs	r6, #10
300075f8:	f507 6389 	add.w	r3, r7, #1096	; 0x448
300075fc:	4681      	mov	r9, r0
300075fe:	9302      	str	r3, [sp, #8]
30007600:	2004      	movs	r0, #4
30007602:	f000 faab 	bl	30007b5c <ymodem_uart_port_deinit>
30007606:	9a02      	ldr	r2, [sp, #8]
30007608:	9500      	str	r5, [sp, #0]
3000760a:	4613      	mov	r3, r2
3000760c:	1990      	adds	r0, r2, r6
3000760e:	f8d7 14d4 	ldr.w	r1, [r7, #1236]	; 0x4d4
30007612:	464a      	mov	r2, r9
30007614:	4443      	add	r3, r8
30007616:	f7ff fad9 	bl	30006bcc <password_hash_check>
3000761a:	4680      	mov	r8, r0
3000761c:	2800      	cmp	r0, #0
3000761e:	d1c5      	bne.n	300075ac <BOOT_Image1+0x818>
30007620:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007624:	47a0      	blx	r4
30007626:	4849      	ldr	r0, [pc, #292]	; (3000774c <BOOT_Image1+0x9b8>)
30007628:	f005 f856 	bl	3000c6d8 <____wrap_strlen_veneer>
3000762c:	4601      	mov	r1, r0
3000762e:	4847      	ldr	r0, [pc, #284]	; (3000774c <BOOT_Image1+0x9b8>)
30007630:	f000 fad6 	bl	30007be0 <ymodem_uart_putdata>
30007634:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007638:	47a0      	blx	r4
3000763a:	f8d7 24d8 	ldr.w	r2, [r7, #1240]	; 0x4d8
3000763e:	4944      	ldr	r1, [pc, #272]	; (30007750 <BOOT_Image1+0x9bc>)
30007640:	2002      	movs	r0, #2
30007642:	f000 fb6d 	bl	30007d20 <ymodem_img_download>
30007646:	2032      	movs	r0, #50	; 0x32
30007648:	47a0      	blx	r4
3000764a:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
3000764e:	b1e3      	cbz	r3, 3000768a <BOOT_Image1+0x8f6>
30007650:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30007734 <BOOT_Image1+0x9a0>
30007654:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
30007658:	f859 6b04 	ldr.w	r6, [r9], #4
3000765c:	0b36      	lsrs	r6, r6, #12
3000765e:	d00c      	beq.n	3000767a <BOOT_Image1+0x8e6>
30007660:	2500      	movs	r5, #0
30007662:	f8da 1000 	ldr.w	r1, [sl]
30007666:	2002      	movs	r0, #2
30007668:	eb01 3105 	add.w	r1, r1, r5, lsl #12
3000766c:	3501      	adds	r5, #1
3000766e:	f7fd fc43 	bl	30004ef8 <FLASH_Erase_With_Lock>
30007672:	2032      	movs	r0, #50	; 0x32
30007674:	47a0      	blx	r4
30007676:	42ae      	cmp	r6, r5
30007678:	d1f3      	bne.n	30007662 <BOOT_Image1+0x8ce>
3000767a:	f108 0801 	add.w	r8, r8, #1
3000767e:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30007682:	f10a 0a04 	add.w	sl, sl, #4
30007686:	4543      	cmp	r3, r8
30007688:	d8e6      	bhi.n	30007658 <BOOT_Image1+0x8c4>
3000768a:	4832      	ldr	r0, [pc, #200]	; (30007754 <BOOT_Image1+0x9c0>)
3000768c:	f004 fffc 	bl	3000c688 <__DiagPrintf_veneer>
30007690:	4b31      	ldr	r3, [pc, #196]	; (30007758 <BOOT_Image1+0x9c4>)
30007692:	a806      	add	r0, sp, #24
30007694:	4798      	blx	r3
30007696:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
3000769a:	4830      	ldr	r0, [pc, #192]	; (3000775c <BOOT_Image1+0x9c8>)
3000769c:	a906      	add	r1, sp, #24
3000769e:	4b30      	ldr	r3, [pc, #192]	; (30007760 <BOOT_Image1+0x9cc>)
300076a0:	f8ad 201a 	strh.w	r2, [sp, #26]
300076a4:	4798      	blx	r3
300076a6:	482d      	ldr	r0, [pc, #180]	; (3000775c <BOOT_Image1+0x9c8>)
300076a8:	4b2e      	ldr	r3, [pc, #184]	; (30007764 <BOOT_Image1+0x9d0>)
300076aa:	4798      	blx	r3
300076ac:	e7fe      	b.n	300076ac <BOOT_Image1+0x918>
300076ae:	4b2e      	ldr	r3, [pc, #184]	; (30007768 <BOOT_Image1+0x9d4>)
300076b0:	4798      	blx	r3
300076b2:	2801      	cmp	r0, #1
300076b4:	bf0c      	ite	eq
300076b6:	2019      	moveq	r0, #25
300076b8:	2004      	movne	r0, #4
300076ba:	47a0      	blx	r4
300076bc:	e5f4      	b.n	300072a8 <BOOT_Image1+0x514>
300076be:	4b2a      	ldr	r3, [pc, #168]	; (30007768 <BOOT_Image1+0x9d4>)
300076c0:	4798      	blx	r3
300076c2:	2801      	cmp	r0, #1
300076c4:	bf0c      	ite	eq
300076c6:	20e6      	moveq	r0, #230	; 0xe6
300076c8:	2019      	movne	r0, #25
300076ca:	47a0      	blx	r4
300076cc:	e5ff      	b.n	300072ce <BOOT_Image1+0x53a>
300076ce:	4827      	ldr	r0, [pc, #156]	; (3000776c <BOOT_Image1+0x9d8>)
300076d0:	f004 ffda 	bl	3000c688 <__DiagPrintf_veneer>
300076d4:	e5cc      	b.n	30007270 <BOOT_Image1+0x4dc>
300076d6:	bf00      	nop
300076d8:	23020000 	.word	0x23020000
300076dc:	0000b479 	.word	0x0000b479
300076e0:	3000d2b8 	.word	0x3000d2b8
300076e4:	3000d288 	.word	0x3000d288
300076e8:	0000c0f9 	.word	0x0000c0f9
300076ec:	0000aea1 	.word	0x0000aea1
300076f0:	0000aab9 	.word	0x0000aab9
300076f4:	4200c000 	.word	0x4200c000
300076f8:	0000aaa5 	.word	0x0000aaa5
300076fc:	0016e360 	.word	0x0016e360
30007700:	3000d3d4 	.word	0x3000d3d4
30007704:	3000c974 	.word	0x3000c974
30007708:	3000d2e8 	.word	0x3000d2e8
3000770c:	30008e61 	.word	0x30008e61
30007710:	3000b699 	.word	0x3000b699
30007714:	3000d2fc 	.word	0x3000d2fc
30007718:	3000d378 	.word	0x3000d378
3000771c:	3000d3b0 	.word	0x3000d3b0
30007720:	3000e9e8 	.word	0x3000e9e8
30007724:	0000c179 	.word	0x0000c179
30007728:	3000f0d0 	.word	0x3000f0d0
3000772c:	3000f0f8 	.word	0x3000f0f8
30007730:	000129bd 	.word	0x000129bd
30007734:	3000f0fc 	.word	0x3000f0fc
30007738:	3000f124 	.word	0x3000f124
3000773c:	3000d404 	.word	0x3000d404
30007740:	41000200 	.word	0x41000200
30007744:	3000d354 	.word	0x3000d354
30007748:	3000ebb0 	.word	0x3000ebb0
3000774c:	3000f9b0 	.word	0x3000f9b0
30007750:	3000f090 	.word	0x3000f090
30007754:	3000d43c 	.word	0x3000d43c
30007758:	0000d3f9 	.word	0x0000d3f9
3000775c:	41000440 	.word	0x41000440
30007760:	0000d409 	.word	0x0000d409
30007764:	0000d489 	.word	0x0000d489
30007768:	0000c169 	.word	0x0000c169
3000776c:	3000d414 	.word	0x3000d414
30007770:	480d      	ldr	r0, [pc, #52]	; (300077a8 <BOOT_Image1+0xa14>)
30007772:	f004 ff89 	bl	3000c688 <__DiagPrintf_veneer>
30007776:	480d      	ldr	r0, [pc, #52]	; (300077ac <BOOT_Image1+0xa18>)
30007778:	f004 ff86 	bl	3000c688 <__DiagPrintf_veneer>
3000777c:	e578      	b.n	30007270 <BOOT_Image1+0x4dc>
3000777e:	480c      	ldr	r0, [pc, #48]	; (300077b0 <BOOT_Image1+0xa1c>)
30007780:	f004 ff82 	bl	3000c688 <__DiagPrintf_veneer>
30007784:	e574      	b.n	30007270 <BOOT_Image1+0x4dc>
30007786:	46d3      	mov	fp, sl
30007788:	e52b      	b.n	300071e2 <BOOT_Image1+0x44e>
3000778a:	46da      	mov	sl, fp
3000778c:	f04f 0900 	mov.w	r9, #0
30007790:	e4fd      	b.n	3000718e <BOOT_Image1+0x3fa>
30007792:	2600      	movs	r6, #0
30007794:	f04f 080a 	mov.w	r8, #10
30007798:	4633      	mov	r3, r6
3000779a:	46b3      	mov	fp, r6
3000779c:	e4c8      	b.n	30007130 <BOOT_Image1+0x39c>
3000779e:	f106 080b 	add.w	r8, r6, #11
300077a2:	360a      	adds	r6, #10
300077a4:	44c8      	add	r8, r9
300077a6:	e72b      	b.n	30007600 <BOOT_Image1+0x86c>
300077a8:	3000d3ec 	.word	0x3000d3ec
300077ac:	3000d404 	.word	0x3000d404
300077b0:	3000d428 	.word	0x3000d428

300077b4 <ymodem_inquiry>:
300077b4:	2806      	cmp	r0, #6
300077b6:	b510      	push	{r4, lr}
300077b8:	4604      	mov	r4, r0
300077ba:	d003      	beq.n	300077c4 <ymodem_inquiry+0x10>
300077bc:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077c0:	4b03      	ldr	r3, [pc, #12]	; (300077d0 <ymodem_inquiry+0x1c>)
300077c2:	4798      	blx	r3
300077c4:	4620      	mov	r0, r4
300077c6:	f000 fa07 	bl	30007bd8 <ymodem_uart_putc>
300077ca:	2001      	movs	r0, #1
300077cc:	bd10      	pop	{r4, pc}
300077ce:	bf00      	nop
300077d0:	00009b2d 	.word	0x00009b2d

300077d4 <ymodem_get_first>:
300077d4:	b530      	push	{r4, r5, lr}
300077d6:	b083      	sub	sp, #12
300077d8:	2300      	movs	r3, #0
300077da:	4604      	mov	r4, r0
300077dc:	4936      	ldr	r1, [pc, #216]	; (300078b8 <ymodem_get_first+0xe4>)
300077de:	f10d 0006 	add.w	r0, sp, #6
300077e2:	f88d 3006 	strb.w	r3, [sp, #6]
300077e6:	f000 fa19 	bl	30007c1c <ymodem_uart_getc_to>
300077ea:	b9d0      	cbnz	r0, 30007822 <ymodem_get_first+0x4e>
300077ec:	f89d 3006 	ldrb.w	r3, [sp, #6]
300077f0:	3b01      	subs	r3, #1
300077f2:	2b17      	cmp	r3, #23
300077f4:	d80d      	bhi.n	30007812 <ymodem_get_first+0x3e>
300077f6:	e8df f003 	tbb	[pc, r3]
300077fa:	231e      	.short	0x231e
300077fc:	0c0c290c 	.word	0x0c0c290c
30007800:	0c0c0c0c 	.word	0x0c0c0c0c
30007804:	0c0c0c0c 	.word	0x0c0c0c0c
30007808:	0c0c0c0c 	.word	0x0c0c0c0c
3000780c:	0c0c0c0c 	.word	0x0c0c0c0c
30007810:	190c      	.short	0x190c
30007812:	4d29      	ldr	r5, [pc, #164]	; (300078b8 <ymodem_get_first+0xe4>)
30007814:	4629      	mov	r1, r5
30007816:	f10d 0007 	add.w	r0, sp, #7
3000781a:	f000 f9ff 	bl	30007c1c <ymodem_uart_getc_to>
3000781e:	2800      	cmp	r0, #0
30007820:	d0f8      	beq.n	30007814 <ymodem_get_first+0x40>
30007822:	2301      	movs	r3, #1
30007824:	2008      	movs	r0, #8
30007826:	60a3      	str	r3, [r4, #8]
30007828:	b003      	add	sp, #12
3000782a:	bd30      	pop	{r4, r5, pc}
3000782c:	2301      	movs	r3, #1
3000782e:	2002      	movs	r0, #2
30007830:	60a3      	str	r3, [r4, #8]
30007832:	b003      	add	sp, #12
30007834:	bd30      	pop	{r4, r5, pc}
30007836:	2380      	movs	r3, #128	; 0x80
30007838:	2001      	movs	r0, #1
3000783a:	80a3      	strh	r3, [r4, #4]
3000783c:	b003      	add	sp, #12
3000783e:	bd30      	pop	{r4, r5, pc}
30007840:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007844:	2001      	movs	r0, #1
30007846:	80a3      	strh	r3, [r4, #4]
30007848:	b003      	add	sp, #12
3000784a:	bd30      	pop	{r4, r5, pc}
3000784c:	2006      	movs	r0, #6
3000784e:	4d1b      	ldr	r5, [pc, #108]	; (300078bc <ymodem_get_first+0xe8>)
30007850:	f000 f9c2 	bl	30007bd8 <ymodem_uart_putc>
30007854:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007858:	47a8      	blx	r5
3000785a:	2043      	movs	r0, #67	; 0x43
3000785c:	f000 f9bc 	bl	30007bd8 <ymodem_uart_putc>
30007860:	2006      	movs	r0, #6
30007862:	f000 f9b9 	bl	30007bd8 <ymodem_uart_putc>
30007866:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000786a:	47a8      	blx	r5
3000786c:	2043      	movs	r0, #67	; 0x43
3000786e:	f000 f9b3 	bl	30007bd8 <ymodem_uart_putc>
30007872:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007876:	47a8      	blx	r5
30007878:	2018      	movs	r0, #24
3000787a:	f000 f9ad 	bl	30007bd8 <ymodem_uart_putc>
3000787e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007882:	47a8      	blx	r5
30007884:	2018      	movs	r0, #24
30007886:	f000 f9a7 	bl	30007bd8 <ymodem_uart_putc>
3000788a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000788e:	47a8      	blx	r5
30007890:	2018      	movs	r0, #24
30007892:	f000 f9a1 	bl	30007bd8 <ymodem_uart_putc>
30007896:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000789a:	47a8      	blx	r5
3000789c:	2018      	movs	r0, #24
3000789e:	f000 f99b 	bl	30007bd8 <ymodem_uart_putc>
300078a2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078a6:	47a8      	blx	r5
300078a8:	2018      	movs	r0, #24
300078aa:	f000 f995 	bl	30007bd8 <ymodem_uart_putc>
300078ae:	2301      	movs	r3, #1
300078b0:	2005      	movs	r0, #5
300078b2:	60a3      	str	r3, [r4, #8]
300078b4:	b003      	add	sp, #12
300078b6:	bd30      	pop	{r4, r5, pc}
300078b8:	000caa30 	.word	0x000caa30
300078bc:	00009b2d 	.word	0x00009b2d

300078c0 <ymodem_get_others>:
300078c0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300078c4:	b083      	sub	sp, #12
300078c6:	2300      	movs	r3, #0
300078c8:	4605      	mov	r5, r0
300078ca:	4952      	ldr	r1, [pc, #328]	; (30007a14 <ymodem_get_others+0x154>)
300078cc:	a801      	add	r0, sp, #4
300078ce:	f88d 3006 	strb.w	r3, [sp, #6]
300078d2:	f88d 3007 	strb.w	r3, [sp, #7]
300078d6:	f000 f9a1 	bl	30007c1c <ymodem_uart_getc_to>
300078da:	bb38      	cbnz	r0, 3000792c <ymodem_get_others+0x6c>
300078dc:	494d      	ldr	r1, [pc, #308]	; (30007a14 <ymodem_get_others+0x154>)
300078de:	f10d 0005 	add.w	r0, sp, #5
300078e2:	f000 f99b 	bl	30007c1c <ymodem_uart_getc_to>
300078e6:	4607      	mov	r7, r0
300078e8:	bb00      	cbnz	r0, 3000792c <ymodem_get_others+0x6c>
300078ea:	f89d 2004 	ldrb.w	r2, [sp, #4]
300078ee:	f89d 6005 	ldrb.w	r6, [sp, #5]
300078f2:	43d3      	mvns	r3, r2
300078f4:	b2db      	uxtb	r3, r3
300078f6:	429e      	cmp	r6, r3
300078f8:	bf0c      	ite	eq
300078fa:	2601      	moveq	r6, #1
300078fc:	2604      	movne	r6, #4
300078fe:	682b      	ldr	r3, [r5, #0]
30007900:	429a      	cmp	r2, r3
30007902:	d21a      	bcs.n	3000793a <ymodem_get_others+0x7a>
30007904:	2701      	movs	r7, #1
30007906:	88ab      	ldrh	r3, [r5, #4]
30007908:	b1d3      	cbz	r3, 30007940 <ymodem_get_others+0x80>
3000790a:	2400      	movs	r4, #0
3000790c:	f8df 9108 	ldr.w	r9, [pc, #264]	; 30007a18 <ymodem_get_others+0x158>
30007910:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007a14 <ymodem_get_others+0x154>
30007914:	e002      	b.n	3000791c <ymodem_get_others+0x5c>
30007916:	88ab      	ldrh	r3, [r5, #4]
30007918:	42a3      	cmp	r3, r4
3000791a:	d911      	bls.n	30007940 <ymodem_get_others+0x80>
3000791c:	eb04 0009 	add.w	r0, r4, r9
30007920:	4641      	mov	r1, r8
30007922:	3401      	adds	r4, #1
30007924:	f000 f97a 	bl	30007c1c <ymodem_uart_getc_to>
30007928:	2800      	cmp	r0, #0
3000792a:	d0f4      	beq.n	30007916 <ymodem_get_others+0x56>
3000792c:	2301      	movs	r3, #1
3000792e:	2608      	movs	r6, #8
30007930:	60ab      	str	r3, [r5, #8]
30007932:	4630      	mov	r0, r6
30007934:	b003      	add	sp, #12
30007936:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000793a:	d0e4      	beq.n	30007906 <ymodem_get_others+0x46>
3000793c:	2604      	movs	r6, #4
3000793e:	e7e2      	b.n	30007906 <ymodem_get_others+0x46>
30007940:	4934      	ldr	r1, [pc, #208]	; (30007a14 <ymodem_get_others+0x154>)
30007942:	f10d 0006 	add.w	r0, sp, #6
30007946:	f000 f969 	bl	30007c1c <ymodem_uart_getc_to>
3000794a:	2800      	cmp	r0, #0
3000794c:	d1ee      	bne.n	3000792c <ymodem_get_others+0x6c>
3000794e:	4931      	ldr	r1, [pc, #196]	; (30007a14 <ymodem_get_others+0x154>)
30007950:	f10d 0007 	add.w	r0, sp, #7
30007954:	f000 f962 	bl	30007c1c <ymodem_uart_getc_to>
30007958:	2800      	cmp	r0, #0
3000795a:	d1e7      	bne.n	3000792c <ymodem_get_others+0x6c>
3000795c:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007960:	f89d e007 	ldrb.w	lr, [sp, #7]
30007964:	88aa      	ldrh	r2, [r5, #4]
30007966:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
3000796a:	b36a      	cbz	r2, 300079c8 <ymodem_get_others+0x108>
3000796c:	4603      	mov	r3, r0
3000796e:	482a      	ldr	r0, [pc, #168]	; (30007a18 <ymodem_get_others+0x158>)
30007970:	f241 0c21 	movw	ip, #4129	; 0x1021
30007974:	eb02 0800 	add.w	r8, r2, r0
30007978:	7801      	ldrb	r1, [r0, #0]
3000797a:	3001      	adds	r0, #1
3000797c:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007980:	2108      	movs	r1, #8
30007982:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
30007986:	005b      	lsls	r3, r3, #1
30007988:	b10c      	cbz	r4, 3000798e <ymodem_get_others+0xce>
3000798a:	ea83 030c 	eor.w	r3, r3, ip
3000798e:	3901      	subs	r1, #1
30007990:	d1f7      	bne.n	30007982 <ymodem_get_others+0xc2>
30007992:	4580      	cmp	r8, r0
30007994:	d1f0      	bne.n	30007978 <ymodem_get_others+0xb8>
30007996:	b29b      	uxth	r3, r3
30007998:	459e      	cmp	lr, r3
3000799a:	d00a      	beq.n	300079b2 <ymodem_get_others+0xf2>
3000799c:	4b1f      	ldr	r3, [pc, #124]	; (30007a1c <ymodem_get_others+0x15c>)
3000799e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079a2:	4798      	blx	r3
300079a4:	2015      	movs	r0, #21
300079a6:	f000 f917 	bl	30007bd8 <ymodem_uart_putc>
300079aa:	2301      	movs	r3, #1
300079ac:	2604      	movs	r6, #4
300079ae:	60ab      	str	r3, [r5, #8]
300079b0:	e7bf      	b.n	30007932 <ymodem_get_others+0x72>
300079b2:	b917      	cbnz	r7, 300079ba <ymodem_get_others+0xfa>
300079b4:	4c1a      	ldr	r4, [pc, #104]	; (30007a20 <ymodem_get_others+0x160>)
300079b6:	6823      	ldr	r3, [r4, #0]
300079b8:	b9b3      	cbnz	r3, 300079e8 <ymodem_get_others+0x128>
300079ba:	f000 f947 	bl	30007c4c <ymodem_uart_clean_rx>
300079be:	2e01      	cmp	r6, #1
300079c0:	d004      	beq.n	300079cc <ymodem_get_others+0x10c>
300079c2:	2e04      	cmp	r6, #4
300079c4:	d1b5      	bne.n	30007932 <ymodem_get_others+0x72>
300079c6:	e7e9      	b.n	3000799c <ymodem_get_others+0xdc>
300079c8:	4613      	mov	r3, r2
300079ca:	e7e5      	b.n	30007998 <ymodem_get_others+0xd8>
300079cc:	4c14      	ldr	r4, [pc, #80]	; (30007a20 <ymodem_get_others+0x160>)
300079ce:	2006      	movs	r0, #6
300079d0:	60ae      	str	r6, [r5, #8]
300079d2:	f000 f901 	bl	30007bd8 <ymodem_uart_putc>
300079d6:	6823      	ldr	r3, [r4, #0]
300079d8:	b193      	cbz	r3, 30007a00 <ymodem_get_others+0x140>
300079da:	2f00      	cmp	r7, #0
300079dc:	f103 0301 	add.w	r3, r3, #1
300079e0:	bf18      	it	ne
300079e2:	2604      	movne	r6, #4
300079e4:	6023      	str	r3, [r4, #0]
300079e6:	e7a4      	b.n	30007932 <ymodem_get_others+0x72>
300079e8:	68eb      	ldr	r3, [r5, #12]
300079ea:	6861      	ldr	r1, [r4, #4]
300079ec:	3301      	adds	r3, #1
300079ee:	480a      	ldr	r0, [pc, #40]	; (30007a18 <ymodem_get_others+0x158>)
300079f0:	60eb      	str	r3, [r5, #12]
300079f2:	f000 f94f 	bl	30007c94 <UARTIMG_Write>
300079f6:	6862      	ldr	r2, [r4, #4]
300079f8:	88ab      	ldrh	r3, [r5, #4]
300079fa:	4413      	add	r3, r2
300079fc:	6063      	str	r3, [r4, #4]
300079fe:	e7dc      	b.n	300079ba <ymodem_get_others+0xfa>
30007a00:	4b06      	ldr	r3, [pc, #24]	; (30007a1c <ymodem_get_others+0x15c>)
30007a02:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a06:	4798      	blx	r3
30007a08:	2043      	movs	r0, #67	; 0x43
30007a0a:	f000 f8e5 	bl	30007bd8 <ymodem_uart_putc>
30007a0e:	6823      	ldr	r3, [r4, #0]
30007a10:	e7e3      	b.n	300079da <ymodem_get_others+0x11a>
30007a12:	bf00      	nop
30007a14:	000caa30 	.word	0x000caa30
30007a18:	20016ffc 	.word	0x20016ffc
30007a1c:	00009b2d 	.word	0x00009b2d
30007a20:	3000f134 	.word	0x3000f134

30007a24 <ymodem_rx_frame>:
30007a24:	2300      	movs	r3, #0
30007a26:	2201      	movs	r2, #1
30007a28:	b510      	push	{r4, lr}
30007a2a:	4604      	mov	r4, r0
30007a2c:	6083      	str	r3, [r0, #8]
30007a2e:	8082      	strh	r2, [r0, #4]
30007a30:	f7ff fed0 	bl	300077d4 <ymodem_get_first>
30007a34:	68a3      	ldr	r3, [r4, #8]
30007a36:	b92b      	cbnz	r3, 30007a44 <ymodem_rx_frame+0x20>
30007a38:	4620      	mov	r0, r4
30007a3a:	f7ff ff41 	bl	300078c0 <ymodem_get_others>
30007a3e:	68a3      	ldr	r3, [r4, #8]
30007a40:	2b00      	cmp	r3, #0
30007a42:	d0f9      	beq.n	30007a38 <ymodem_rx_frame+0x14>
30007a44:	2801      	cmp	r0, #1
30007a46:	d000      	beq.n	30007a4a <ymodem_rx_frame+0x26>
30007a48:	bd10      	pop	{r4, pc}
30007a4a:	6823      	ldr	r3, [r4, #0]
30007a4c:	3301      	adds	r3, #1
30007a4e:	b2db      	uxtb	r3, r3
30007a50:	6023      	str	r3, [r4, #0]
30007a52:	bd10      	pop	{r4, pc}

30007a54 <ymodem_img_rxbuffer>:
30007a54:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007a58:	2300      	movs	r3, #0
30007a5a:	4c28      	ldr	r4, [pc, #160]	; (30007afc <ymodem_img_rxbuffer+0xa8>)
30007a5c:	4e28      	ldr	r6, [pc, #160]	; (30007b00 <ymodem_img_rxbuffer+0xac>)
30007a5e:	460d      	mov	r5, r1
30007a60:	60a3      	str	r3, [r4, #8]
30007a62:	6163      	str	r3, [r4, #20]
30007a64:	f000 f8f2 	bl	30007c4c <ymodem_uart_clean_rx>
30007a68:	2043      	movs	r0, #67	; 0x43
30007a6a:	f000 f8b5 	bl	30007bd8 <ymodem_uart_putc>
30007a6e:	f24c 3050 	movw	r0, #50000	; 0xc350
30007a72:	47b0      	blx	r6
30007a74:	f000 f89a 	bl	30007bac <ymodem_uart_readable>
30007a78:	2800      	cmp	r0, #0
30007a7a:	d0f5      	beq.n	30007a68 <ymodem_img_rxbuffer+0x14>
30007a7c:	2700      	movs	r7, #0
30007a7e:	f04f 0801 	mov.w	r8, #1
30007a82:	e9c4 7500 	strd	r7, r5, [r4]
30007a86:	4d1f      	ldr	r5, [pc, #124]	; (30007b04 <ymodem_img_rxbuffer+0xb0>)
30007a88:	4628      	mov	r0, r5
30007a8a:	f8a4 800c 	strh.w	r8, [r4, #12]
30007a8e:	6127      	str	r7, [r4, #16]
30007a90:	f7ff fea0 	bl	300077d4 <ymodem_get_first>
30007a94:	6923      	ldr	r3, [r4, #16]
30007a96:	b92b      	cbnz	r3, 30007aa4 <ymodem_img_rxbuffer+0x50>
30007a98:	4628      	mov	r0, r5
30007a9a:	f7ff ff11 	bl	300078c0 <ymodem_get_others>
30007a9e:	6923      	ldr	r3, [r4, #16]
30007aa0:	2b00      	cmp	r3, #0
30007aa2:	d0f9      	beq.n	30007a98 <ymodem_img_rxbuffer+0x44>
30007aa4:	2801      	cmp	r0, #1
30007aa6:	d024      	beq.n	30007af2 <ymodem_img_rxbuffer+0x9e>
30007aa8:	3804      	subs	r0, #4
30007aaa:	2804      	cmp	r0, #4
30007aac:	d80c      	bhi.n	30007ac8 <ymodem_img_rxbuffer+0x74>
30007aae:	a301      	add	r3, pc, #4	; (adr r3, 30007ab4 <ymodem_img_rxbuffer+0x60>)
30007ab0:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007ab4:	30007a89 	.word	0x30007a89
30007ab8:	30007ad5 	.word	0x30007ad5
30007abc:	30007ac9 	.word	0x30007ac9
30007ac0:	30007ac9 	.word	0x30007ac9
30007ac4:	30007ae1 	.word	0x30007ae1
30007ac8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007acc:	47b0      	blx	r6
30007ace:	2018      	movs	r0, #24
30007ad0:	f000 f882 	bl	30007bd8 <ymodem_uart_putc>
30007ad4:	200a      	movs	r0, #10
30007ad6:	f000 f8c9 	bl	30007c6c <ymodem_uart_waitbusy>
30007ada:	2001      	movs	r0, #1
30007adc:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007ae0:	f000 f8b4 	bl	30007c4c <ymodem_uart_clean_rx>
30007ae4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007ae8:	47b0      	blx	r6
30007aea:	2015      	movs	r0, #21
30007aec:	f000 f874 	bl	30007bd8 <ymodem_uart_putc>
30007af0:	e7ca      	b.n	30007a88 <ymodem_img_rxbuffer+0x34>
30007af2:	68a3      	ldr	r3, [r4, #8]
30007af4:	3301      	adds	r3, #1
30007af6:	b2db      	uxtb	r3, r3
30007af8:	60a3      	str	r3, [r4, #8]
30007afa:	e7c5      	b.n	30007a88 <ymodem_img_rxbuffer+0x34>
30007afc:	3000f134 	.word	0x3000f134
30007b00:	00009b2d 	.word	0x00009b2d
30007b04:	3000f13c 	.word	0x3000f13c

30007b08 <ymodem_log_open>:
30007b08:	b510      	push	{r4, lr}
30007b0a:	2303      	movs	r3, #3
30007b0c:	4c04      	ldr	r4, [pc, #16]	; (30007b20 <ymodem_log_open+0x18>)
30007b0e:	2200      	movs	r2, #0
30007b10:	2108      	movs	r1, #8
30007b12:	4804      	ldr	r0, [pc, #16]	; (30007b24 <ymodem_log_open+0x1c>)
30007b14:	47a0      	blx	r4
30007b16:	2008      	movs	r0, #8
30007b18:	4b03      	ldr	r3, [pc, #12]	; (30007b28 <ymodem_log_open+0x20>)
30007b1a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b1e:	4718      	bx	r3
30007b20:	30009de5 	.word	0x30009de5
30007b24:	0000ed39 	.word	0x0000ed39
30007b28:	0000e1bd 	.word	0x0000e1bd

30007b2c <ymodem_uart_port_init>:
30007b2c:	b500      	push	{lr}
30007b2e:	b083      	sub	sp, #12
30007b30:	2008      	movs	r0, #8
30007b32:	4b06      	ldr	r3, [pc, #24]	; (30007b4c <ymodem_uart_port_init+0x20>)
30007b34:	9201      	str	r2, [sp, #4]
30007b36:	4798      	blx	r3
30007b38:	4b05      	ldr	r3, [pc, #20]	; (30007b50 <ymodem_uart_port_init+0x24>)
30007b3a:	2008      	movs	r0, #8
30007b3c:	4798      	blx	r3
30007b3e:	9901      	ldr	r1, [sp, #4]
30007b40:	4804      	ldr	r0, [pc, #16]	; (30007b54 <ymodem_uart_port_init+0x28>)
30007b42:	4b05      	ldr	r3, [pc, #20]	; (30007b58 <ymodem_uart_port_init+0x2c>)
30007b44:	b003      	add	sp, #12
30007b46:	f85d eb04 	ldr.w	lr, [sp], #4
30007b4a:	4718      	bx	r3
30007b4c:	0000e1d9 	.word	0x0000e1d9
30007b50:	0000e2fd 	.word	0x0000e2fd
30007b54:	4200c000 	.word	0x4200c000
30007b58:	0000a9e5 	.word	0x0000a9e5

30007b5c <ymodem_uart_port_deinit>:
30007b5c:	2804      	cmp	r0, #4
30007b5e:	d004      	beq.n	30007b6a <ymodem_uart_port_deinit+0xe>
30007b60:	4b0d      	ldr	r3, [pc, #52]	; (30007b98 <ymodem_uart_port_deinit+0x3c>)
30007b62:	681b      	ldr	r3, [r3, #0]
30007b64:	039b      	lsls	r3, r3, #14
30007b66:	d40f      	bmi.n	30007b88 <ymodem_uart_port_deinit+0x2c>
30007b68:	4770      	bx	lr
30007b6a:	2200      	movs	r2, #0
30007b6c:	2303      	movs	r3, #3
30007b6e:	2108      	movs	r1, #8
30007b70:	480a      	ldr	r0, [pc, #40]	; (30007b9c <ymodem_uart_port_deinit+0x40>)
30007b72:	b510      	push	{r4, lr}
30007b74:	4c0a      	ldr	r4, [pc, #40]	; (30007ba0 <ymodem_uart_port_deinit+0x44>)
30007b76:	47a0      	blx	r4
30007b78:	4b0a      	ldr	r3, [pc, #40]	; (30007ba4 <ymodem_uart_port_deinit+0x48>)
30007b7a:	2008      	movs	r0, #8
30007b7c:	4798      	blx	r3
30007b7e:	4b06      	ldr	r3, [pc, #24]	; (30007b98 <ymodem_uart_port_deinit+0x3c>)
30007b80:	681b      	ldr	r3, [r3, #0]
30007b82:	039a      	lsls	r2, r3, #14
30007b84:	d403      	bmi.n	30007b8e <ymodem_uart_port_deinit+0x32>
30007b86:	bd10      	pop	{r4, pc}
30007b88:	4807      	ldr	r0, [pc, #28]	; (30007ba8 <ymodem_uart_port_deinit+0x4c>)
30007b8a:	f004 bd7d 	b.w	3000c688 <__DiagPrintf_veneer>
30007b8e:	4806      	ldr	r0, [pc, #24]	; (30007ba8 <ymodem_uart_port_deinit+0x4c>)
30007b90:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b94:	f004 bd78 	b.w	3000c688 <__DiagPrintf_veneer>
30007b98:	2001c00c 	.word	0x2001c00c
30007b9c:	0000ed39 	.word	0x0000ed39
30007ba0:	30009de5 	.word	0x30009de5
30007ba4:	0000e1bd 	.word	0x0000e1bd
30007ba8:	3000d45c 	.word	0x3000d45c

30007bac <ymodem_uart_readable>:
30007bac:	4b00      	ldr	r3, [pc, #0]	; (30007bb0 <ymodem_uart_readable+0x4>)
30007bae:	4718      	bx	r3
30007bb0:	0000ac39 	.word	0x0000ac39

30007bb4 <ymodem_uart_writable>:
30007bb4:	4a03      	ldr	r2, [pc, #12]	; (30007bc4 <ymodem_uart_writable+0x10>)
30007bb6:	b508      	push	{r3, lr}
30007bb8:	4b03      	ldr	r3, [pc, #12]	; (30007bc8 <ymodem_uart_writable+0x14>)
30007bba:	6810      	ldr	r0, [r2, #0]
30007bbc:	4798      	blx	r3
30007bbe:	b2c0      	uxtb	r0, r0
30007bc0:	bd08      	pop	{r3, pc}
30007bc2:	bf00      	nop
30007bc4:	3000f14c 	.word	0x3000f14c
30007bc8:	0000d1a9 	.word	0x0000d1a9

30007bcc <ymodem_uart_getc>:
30007bcc:	2000      	movs	r0, #0
30007bce:	4b01      	ldr	r3, [pc, #4]	; (30007bd4 <ymodem_uart_getc+0x8>)
30007bd0:	4718      	bx	r3
30007bd2:	bf00      	nop
30007bd4:	0000ac1d 	.word	0x0000ac1d

30007bd8 <ymodem_uart_putc>:
30007bd8:	4b00      	ldr	r3, [pc, #0]	; (30007bdc <ymodem_uart_putc+0x4>)
30007bda:	4718      	bx	r3
30007bdc:	0000abb5 	.word	0x0000abb5

30007be0 <ymodem_uart_putdata>:
30007be0:	b149      	cbz	r1, 30007bf6 <ymodem_uart_putdata+0x16>
30007be2:	b570      	push	{r4, r5, r6, lr}
30007be4:	4604      	mov	r4, r0
30007be6:	1845      	adds	r5, r0, r1
30007be8:	4e03      	ldr	r6, [pc, #12]	; (30007bf8 <ymodem_uart_putdata+0x18>)
30007bea:	f814 0b01 	ldrb.w	r0, [r4], #1
30007bee:	47b0      	blx	r6
30007bf0:	42ac      	cmp	r4, r5
30007bf2:	d1fa      	bne.n	30007bea <ymodem_uart_putdata+0xa>
30007bf4:	bd70      	pop	{r4, r5, r6, pc}
30007bf6:	4770      	bx	lr
30007bf8:	0000abb5 	.word	0x0000abb5

30007bfc <ymodem_uart_getdata>:
30007bfc:	b151      	cbz	r1, 30007c14 <ymodem_uart_getdata+0x18>
30007bfe:	b570      	push	{r4, r5, r6, lr}
30007c00:	4604      	mov	r4, r0
30007c02:	1845      	adds	r5, r0, r1
30007c04:	4e04      	ldr	r6, [pc, #16]	; (30007c18 <ymodem_uart_getdata+0x1c>)
30007c06:	2001      	movs	r0, #1
30007c08:	47b0      	blx	r6
30007c0a:	f804 0b01 	strb.w	r0, [r4], #1
30007c0e:	42ac      	cmp	r4, r5
30007c10:	d1f9      	bne.n	30007c06 <ymodem_uart_getdata+0xa>
30007c12:	bd70      	pop	{r4, r5, r6, pc}
30007c14:	4770      	bx	lr
30007c16:	bf00      	nop
30007c18:	0000ac1d 	.word	0x0000ac1d

30007c1c <ymodem_uart_getc_to>:
30007c1c:	b570      	push	{r4, r5, r6, lr}
30007c1e:	4d09      	ldr	r5, [pc, #36]	; (30007c44 <ymodem_uart_getc_to+0x28>)
30007c20:	4606      	mov	r6, r0
30007c22:	460c      	mov	r4, r1
30007c24:	e001      	b.n	30007c2a <ymodem_uart_getc_to+0xe>
30007c26:	3c01      	subs	r4, #1
30007c28:	d009      	beq.n	30007c3e <ymodem_uart_getc_to+0x22>
30007c2a:	47a8      	blx	r5
30007c2c:	2800      	cmp	r0, #0
30007c2e:	d0fa      	beq.n	30007c26 <ymodem_uart_getc_to+0xa>
30007c30:	4b05      	ldr	r3, [pc, #20]	; (30007c48 <ymodem_uart_getc_to+0x2c>)
30007c32:	2000      	movs	r0, #0
30007c34:	4798      	blx	r3
30007c36:	4603      	mov	r3, r0
30007c38:	2000      	movs	r0, #0
30007c3a:	7033      	strb	r3, [r6, #0]
30007c3c:	bd70      	pop	{r4, r5, r6, pc}
30007c3e:	2001      	movs	r0, #1
30007c40:	bd70      	pop	{r4, r5, r6, pc}
30007c42:	bf00      	nop
30007c44:	0000ac39 	.word	0x0000ac39
30007c48:	0000ac1d 	.word	0x0000ac1d

30007c4c <ymodem_uart_clean_rx>:
30007c4c:	b538      	push	{r3, r4, r5, lr}
30007c4e:	4c05      	ldr	r4, [pc, #20]	; (30007c64 <ymodem_uart_clean_rx+0x18>)
30007c50:	4d05      	ldr	r5, [pc, #20]	; (30007c68 <ymodem_uart_clean_rx+0x1c>)
30007c52:	e000      	b.n	30007c56 <ymodem_uart_clean_rx+0xa>
30007c54:	47a8      	blx	r5
30007c56:	47a0      	blx	r4
30007c58:	4603      	mov	r3, r0
30007c5a:	2000      	movs	r0, #0
30007c5c:	2b00      	cmp	r3, #0
30007c5e:	d1f9      	bne.n	30007c54 <ymodem_uart_clean_rx+0x8>
30007c60:	bd38      	pop	{r3, r4, r5, pc}
30007c62:	bf00      	nop
30007c64:	0000ac39 	.word	0x0000ac39
30007c68:	0000ac1d 	.word	0x0000ac1d

30007c6c <ymodem_uart_waitbusy>:
30007c6c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007c6e:	2400      	movs	r4, #0
30007c70:	4e06      	ldr	r6, [pc, #24]	; (30007c8c <ymodem_uart_waitbusy+0x20>)
30007c72:	4f07      	ldr	r7, [pc, #28]	; (30007c90 <ymodem_uart_waitbusy+0x24>)
30007c74:	4605      	mov	r5, r0
30007c76:	e002      	b.n	30007c7e <ymodem_uart_waitbusy+0x12>
30007c78:	47b8      	blx	r7
30007c7a:	42ac      	cmp	r4, r5
30007c7c:	d804      	bhi.n	30007c88 <ymodem_uart_waitbusy+0x1c>
30007c7e:	6973      	ldr	r3, [r6, #20]
30007c80:	3401      	adds	r4, #1
30007c82:	2064      	movs	r0, #100	; 0x64
30007c84:	03db      	lsls	r3, r3, #15
30007c86:	d5f7      	bpl.n	30007c78 <ymodem_uart_waitbusy+0xc>
30007c88:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007c8a:	bf00      	nop
30007c8c:	4200c000 	.word	0x4200c000
30007c90:	00009b2d 	.word	0x00009b2d

30007c94 <UARTIMG_Write>:
30007c94:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007c98:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007c9c:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007ca0:	d21d      	bcs.n	30007cde <UARTIMG_Write+0x4a>
30007ca2:	3a01      	subs	r2, #1
30007ca4:	4f19      	ldr	r7, [pc, #100]	; (30007d0c <UARTIMG_Write+0x78>)
30007ca6:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007d1c <UARTIMG_Write+0x88>
30007caa:	4688      	mov	r8, r1
30007cac:	1916      	adds	r6, r2, r4
30007cae:	4605      	mov	r5, r0
30007cb0:	f8d9 3000 	ldr.w	r3, [r9]
30007cb4:	403e      	ands	r6, r7
30007cb6:	4027      	ands	r7, r4
30007cb8:	42bb      	cmp	r3, r7
30007cba:	d91b      	bls.n	30007cf4 <UARTIMG_Write+0x60>
30007cbc:	42b3      	cmp	r3, r6
30007cbe:	d910      	bls.n	30007ce2 <UARTIMG_Write+0x4e>
30007cc0:	4e13      	ldr	r6, [pc, #76]	; (30007d10 <UARTIMG_Write+0x7c>)
30007cc2:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007cc6:	4f13      	ldr	r7, [pc, #76]	; (30007d14 <UARTIMG_Write+0x80>)
30007cc8:	4446      	add	r6, r8
30007cca:	eba3 0808 	sub.w	r8, r3, r8
30007cce:	eb08 0204 	add.w	r2, r8, r4
30007cd2:	4620      	mov	r0, r4
30007cd4:	3408      	adds	r4, #8
30007cd6:	2108      	movs	r1, #8
30007cd8:	47b8      	blx	r7
30007cda:	42b4      	cmp	r4, r6
30007cdc:	d1f7      	bne.n	30007cce <UARTIMG_Write+0x3a>
30007cde:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007ce2:	4631      	mov	r1, r6
30007ce4:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007ce8:	2002      	movs	r0, #2
30007cea:	4b0b      	ldr	r3, [pc, #44]	; (30007d18 <UARTIMG_Write+0x84>)
30007cec:	4798      	blx	r3
30007cee:	f8c9 6000 	str.w	r6, [r9]
30007cf2:	e7e5      	b.n	30007cc0 <UARTIMG_Write+0x2c>
30007cf4:	4b08      	ldr	r3, [pc, #32]	; (30007d18 <UARTIMG_Write+0x84>)
30007cf6:	4639      	mov	r1, r7
30007cf8:	2002      	movs	r0, #2
30007cfa:	4798      	blx	r3
30007cfc:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007d00:	42b3      	cmp	r3, r6
30007d02:	f8c9 3000 	str.w	r3, [r9]
30007d06:	d8db      	bhi.n	30007cc0 <UARTIMG_Write+0x2c>
30007d08:	e7eb      	b.n	30007ce2 <UARTIMG_Write+0x4e>
30007d0a:	bf00      	nop
30007d0c:	fffff000 	.word	0xfffff000
30007d10:	f8000400 	.word	0xf8000400
30007d14:	0000a04d 	.word	0x0000a04d
30007d18:	0000a16d 	.word	0x0000a16d
30007d1c:	3000f150 	.word	0x3000f150

30007d20 <ymodem_img_download>:
30007d20:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007d24:	4616      	mov	r6, r2
30007d26:	460d      	mov	r5, r1
30007d28:	4a0a      	ldr	r2, [pc, #40]	; (30007d54 <ymodem_img_download+0x34>)
30007d2a:	2100      	movs	r1, #0
30007d2c:	4680      	mov	r8, r0
30007d2e:	f7ff fefd 	bl	30007b2c <ymodem_uart_port_init>
30007d32:	b156      	cbz	r6, 30007d4a <ymodem_img_download+0x2a>
30007d34:	3d04      	subs	r5, #4
30007d36:	2400      	movs	r4, #0
30007d38:	4f06      	ldr	r7, [pc, #24]	; (30007d54 <ymodem_img_download+0x34>)
30007d3a:	3401      	adds	r4, #1
30007d3c:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007d40:	4638      	mov	r0, r7
30007d42:	f7ff fe87 	bl	30007a54 <ymodem_img_rxbuffer>
30007d46:	42b4      	cmp	r4, r6
30007d48:	d1f7      	bne.n	30007d3a <ymodem_img_download+0x1a>
30007d4a:	4640      	mov	r0, r8
30007d4c:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007d50:	f7ff bf04 	b.w	30007b5c <ymodem_uart_port_deinit>
30007d54:	0016e360 	.word	0x0016e360

30007d58 <ChipInfo_Invalid>:
30007d58:	4e06      	ldr	r6, [pc, #24]	; (30007d74 <ChipInfo_Invalid+0x1c>)
30007d5a:	4d07      	ldr	r5, [pc, #28]	; (30007d78 <ChipInfo_Invalid+0x20>)
30007d5c:	4c07      	ldr	r4, [pc, #28]	; (30007d7c <ChipInfo_Invalid+0x24>)
30007d5e:	b508      	push	{r3, lr}
30007d60:	462b      	mov	r3, r5
30007d62:	4622      	mov	r2, r4
30007d64:	4904      	ldr	r1, [pc, #16]	; (30007d78 <ChipInfo_Invalid+0x20>)
30007d66:	2002      	movs	r0, #2
30007d68:	f004 f9ea 	bl	3000c140 <rtk_log_write>
30007d6c:	f242 7010 	movw	r0, #10000	; 0x2710
30007d70:	47b0      	blx	r6
30007d72:	e7f5      	b.n	30007d60 <ChipInfo_Invalid+0x8>
30007d74:	00009be5 	.word	0x00009be5
30007d78:	3000d47c 	.word	0x3000d47c
30007d7c:	3000d488 	.word	0x3000d488

30007d80 <ChipInfo_Get>:
30007d80:	b570      	push	{r4, r5, r6, lr}
30007d82:	4c11      	ldr	r4, [pc, #68]	; (30007dc8 <ChipInfo_Get+0x48>)
30007d84:	7820      	ldrb	r0, [r4, #0]
30007d86:	28ff      	cmp	r0, #255	; 0xff
30007d88:	d104      	bne.n	30007d94 <ChipInfo_Get+0x14>
30007d8a:	4d10      	ldr	r5, [pc, #64]	; (30007dcc <ChipInfo_Get+0x4c>)
30007d8c:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007d90:	b108      	cbz	r0, 30007d96 <ChipInfo_Get+0x16>
30007d92:	7020      	strb	r0, [r4, #0]
30007d94:	bd70      	pop	{r4, r5, r6, pc}
30007d96:	4621      	mov	r1, r4
30007d98:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007d9c:	f000 fefe 	bl	30008b9c <OTP_Read8>
30007da0:	7820      	ldrb	r0, [r4, #0]
30007da2:	28ff      	cmp	r0, #255	; 0xff
30007da4:	d002      	beq.n	30007dac <ChipInfo_Get+0x2c>
30007da6:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007daa:	bd70      	pop	{r4, r5, r6, pc}
30007dac:	4e08      	ldr	r6, [pc, #32]	; (30007dd0 <ChipInfo_Get+0x50>)
30007dae:	4d09      	ldr	r5, [pc, #36]	; (30007dd4 <ChipInfo_Get+0x54>)
30007db0:	4c09      	ldr	r4, [pc, #36]	; (30007dd8 <ChipInfo_Get+0x58>)
30007db2:	462b      	mov	r3, r5
30007db4:	4622      	mov	r2, r4
30007db6:	4907      	ldr	r1, [pc, #28]	; (30007dd4 <ChipInfo_Get+0x54>)
30007db8:	2003      	movs	r0, #3
30007dba:	f004 f9c1 	bl	3000c140 <rtk_log_write>
30007dbe:	f242 7010 	movw	r0, #10000	; 0x2710
30007dc2:	47b0      	blx	r6
30007dc4:	e7f5      	b.n	30007db2 <ChipInfo_Get+0x32>
30007dc6:	bf00      	nop
30007dc8:	3000f24c 	.word	0x3000f24c
30007dcc:	23020000 	.word	0x23020000
30007dd0:	00009be5 	.word	0x00009be5
30007dd4:	3000d47c 	.word	0x3000d47c
30007dd8:	3000d4ac 	.word	0x3000d4ac

30007ddc <ChipInfo_MemoryType>:
30007ddc:	b510      	push	{r4, lr}
30007dde:	4c11      	ldr	r4, [pc, #68]	; (30007e24 <ChipInfo_MemoryType+0x48>)
30007de0:	47a0      	blx	r4
30007de2:	2802      	cmp	r0, #2
30007de4:	d019      	beq.n	30007e1a <ChipInfo_MemoryType+0x3e>
30007de6:	47a0      	blx	r4
30007de8:	2803      	cmp	r0, #3
30007dea:	d016      	beq.n	30007e1a <ChipInfo_MemoryType+0x3e>
30007dec:	4c0e      	ldr	r4, [pc, #56]	; (30007e28 <ChipInfo_MemoryType+0x4c>)
30007dee:	4b0f      	ldr	r3, [pc, #60]	; (30007e2c <ChipInfo_MemoryType+0x50>)
30007df0:	4798      	blx	r3
30007df2:	2100      	movs	r1, #0
30007df4:	23e0      	movs	r3, #224	; 0xe0
30007df6:	4622      	mov	r2, r4
30007df8:	e004      	b.n	30007e04 <ChipInfo_MemoryType+0x28>
30007dfa:	7a13      	ldrb	r3, [r2, #8]
30007dfc:	3101      	adds	r1, #1
30007dfe:	3206      	adds	r2, #6
30007e00:	2bff      	cmp	r3, #255	; 0xff
30007e02:	d00c      	beq.n	30007e1e <ChipInfo_MemoryType+0x42>
30007e04:	4298      	cmp	r0, r3
30007e06:	d1f8      	bne.n	30007dfa <ChipInfo_MemoryType+0x1e>
30007e08:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e0c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e10:	78cb      	ldrb	r3, [r1, #3]
30007e12:	2b02      	cmp	r3, #2
30007e14:	d001      	beq.n	30007e1a <ChipInfo_MemoryType+0x3e>
30007e16:	2001      	movs	r0, #1
30007e18:	bd10      	pop	{r4, pc}
30007e1a:	2002      	movs	r0, #2
30007e1c:	bd10      	pop	{r4, pc}
30007e1e:	f7ff ff9b 	bl	30007d58 <ChipInfo_Invalid>
30007e22:	bf00      	nop
30007e24:	0000c0f9 	.word	0x0000c0f9
30007e28:	3000d4d8 	.word	0x3000d4d8
30007e2c:	30007d81 	.word	0x30007d81

30007e30 <ChipInfo_DDRType>:
30007e30:	b510      	push	{r4, lr}
30007e32:	4b0f      	ldr	r3, [pc, #60]	; (30007e70 <ChipInfo_DDRType+0x40>)
30007e34:	4798      	blx	r3
30007e36:	4b0f      	ldr	r3, [pc, #60]	; (30007e74 <ChipInfo_DDRType+0x44>)
30007e38:	4604      	mov	r4, r0
30007e3a:	4798      	blx	r3
30007e3c:	2801      	cmp	r0, #1
30007e3e:	d112      	bne.n	30007e66 <ChipInfo_DDRType+0x36>
30007e40:	480d      	ldr	r0, [pc, #52]	; (30007e78 <ChipInfo_DDRType+0x48>)
30007e42:	2100      	movs	r1, #0
30007e44:	22e0      	movs	r2, #224	; 0xe0
30007e46:	4603      	mov	r3, r0
30007e48:	e003      	b.n	30007e52 <ChipInfo_DDRType+0x22>
30007e4a:	789a      	ldrb	r2, [r3, #2]
30007e4c:	3101      	adds	r1, #1
30007e4e:	2aff      	cmp	r2, #255	; 0xff
30007e50:	d00b      	beq.n	30007e6a <ChipInfo_DDRType+0x3a>
30007e52:	4294      	cmp	r4, r2
30007e54:	f103 0306 	add.w	r3, r3, #6
30007e58:	d1f7      	bne.n	30007e4a <ChipInfo_DDRType+0x1a>
30007e5a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e5e:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007e62:	78c8      	ldrb	r0, [r1, #3]
30007e64:	bd10      	pop	{r4, pc}
30007e66:	20ff      	movs	r0, #255	; 0xff
30007e68:	bd10      	pop	{r4, pc}
30007e6a:	2000      	movs	r0, #0
30007e6c:	bd10      	pop	{r4, pc}
30007e6e:	bf00      	nop
30007e70:	30007d81 	.word	0x30007d81
30007e74:	30007ddd 	.word	0x30007ddd
30007e78:	3000d4d8 	.word	0x3000d4d8

30007e7c <ChipInfo_MemorySize>:
30007e7c:	b510      	push	{r4, lr}
30007e7e:	4c0b      	ldr	r4, [pc, #44]	; (30007eac <ChipInfo_MemorySize+0x30>)
30007e80:	4b0b      	ldr	r3, [pc, #44]	; (30007eb0 <ChipInfo_MemorySize+0x34>)
30007e82:	4798      	blx	r3
30007e84:	2100      	movs	r1, #0
30007e86:	23e0      	movs	r3, #224	; 0xe0
30007e88:	4622      	mov	r2, r4
30007e8a:	e004      	b.n	30007e96 <ChipInfo_MemorySize+0x1a>
30007e8c:	7a13      	ldrb	r3, [r2, #8]
30007e8e:	3101      	adds	r1, #1
30007e90:	3206      	adds	r2, #6
30007e92:	2bff      	cmp	r3, #255	; 0xff
30007e94:	d007      	beq.n	30007ea6 <ChipInfo_MemorySize+0x2a>
30007e96:	4298      	cmp	r0, r3
30007e98:	d1f8      	bne.n	30007e8c <ChipInfo_MemorySize+0x10>
30007e9a:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e9e:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007ea2:	7908      	ldrb	r0, [r1, #4]
30007ea4:	bd10      	pop	{r4, pc}
30007ea6:	f7ff ff57 	bl	30007d58 <ChipInfo_Invalid>
30007eaa:	bf00      	nop
30007eac:	3000d4d8 	.word	0x3000d4d8
30007eb0:	30007d81 	.word	0x30007d81

30007eb4 <ChipInfo_MemoryVendor>:
30007eb4:	b510      	push	{r4, lr}
30007eb6:	4c0f      	ldr	r4, [pc, #60]	; (30007ef4 <ChipInfo_MemoryVendor+0x40>)
30007eb8:	47a0      	blx	r4
30007eba:	2803      	cmp	r0, #3
30007ebc:	d016      	beq.n	30007eec <ChipInfo_MemoryVendor+0x38>
30007ebe:	47a0      	blx	r4
30007ec0:	2802      	cmp	r0, #2
30007ec2:	d012      	beq.n	30007eea <ChipInfo_MemoryVendor+0x36>
30007ec4:	4c0c      	ldr	r4, [pc, #48]	; (30007ef8 <ChipInfo_MemoryVendor+0x44>)
30007ec6:	4b0d      	ldr	r3, [pc, #52]	; (30007efc <ChipInfo_MemoryVendor+0x48>)
30007ec8:	4798      	blx	r3
30007eca:	2100      	movs	r1, #0
30007ecc:	23e0      	movs	r3, #224	; 0xe0
30007ece:	4622      	mov	r2, r4
30007ed0:	e004      	b.n	30007edc <ChipInfo_MemoryVendor+0x28>
30007ed2:	7a13      	ldrb	r3, [r2, #8]
30007ed4:	3101      	adds	r1, #1
30007ed6:	3206      	adds	r2, #6
30007ed8:	2bff      	cmp	r3, #255	; 0xff
30007eda:	d009      	beq.n	30007ef0 <ChipInfo_MemoryVendor+0x3c>
30007edc:	4298      	cmp	r0, r3
30007ede:	d1f8      	bne.n	30007ed2 <ChipInfo_MemoryVendor+0x1e>
30007ee0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007ee4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007ee8:	7948      	ldrb	r0, [r1, #5]
30007eea:	bd10      	pop	{r4, pc}
30007eec:	2001      	movs	r0, #1
30007eee:	bd10      	pop	{r4, pc}
30007ef0:	f7ff ff32 	bl	30007d58 <ChipInfo_Invalid>
30007ef4:	0000c0f9 	.word	0x0000c0f9
30007ef8:	3000d4d8 	.word	0x3000d4d8
30007efc:	30007d81 	.word	0x30007d81

30007f00 <SDM32K_Enable>:
30007f00:	4a02      	ldr	r2, [pc, #8]	; (30007f0c <SDM32K_Enable+0xc>)
30007f02:	4b03      	ldr	r3, [pc, #12]	; (30007f10 <SDM32K_Enable+0x10>)
30007f04:	6811      	ldr	r1, [r2, #0]
30007f06:	430b      	orrs	r3, r1
30007f08:	6013      	str	r3, [r2, #0]
30007f0a:	4770      	bx	lr
30007f0c:	42008e00 	.word	0x42008e00
30007f10:	c0060000 	.word	0xc0060000

30007f14 <CLK_SWITCH_XTAL>:
30007f14:	b508      	push	{r3, lr}
30007f16:	2801      	cmp	r0, #1
30007f18:	4b0a      	ldr	r3, [pc, #40]	; (30007f44 <CLK_SWITCH_XTAL+0x30>)
30007f1a:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007f1e:	d00d      	beq.n	30007f3c <CLK_SWITCH_XTAL+0x28>
30007f20:	4b09      	ldr	r3, [pc, #36]	; (30007f48 <CLK_SWITCH_XTAL+0x34>)
30007f22:	4313      	orrs	r3, r2
30007f24:	4907      	ldr	r1, [pc, #28]	; (30007f44 <CLK_SWITCH_XTAL+0x30>)
30007f26:	4a09      	ldr	r2, [pc, #36]	; (30007f4c <CLK_SWITCH_XTAL+0x38>)
30007f28:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007f2c:	4790      	blx	r2
30007f2e:	4a08      	ldr	r2, [pc, #32]	; (30007f50 <CLK_SWITCH_XTAL+0x3c>)
30007f30:	4b08      	ldr	r3, [pc, #32]	; (30007f54 <CLK_SWITCH_XTAL+0x40>)
30007f32:	fba2 2000 	umull	r2, r0, r2, r0
30007f36:	0c80      	lsrs	r0, r0, #18
30007f38:	6018      	str	r0, [r3, #0]
30007f3a:	bd08      	pop	{r3, pc}
30007f3c:	4b06      	ldr	r3, [pc, #24]	; (30007f58 <CLK_SWITCH_XTAL+0x44>)
30007f3e:	4013      	ands	r3, r2
30007f40:	e7f0      	b.n	30007f24 <CLK_SWITCH_XTAL+0x10>
30007f42:	bf00      	nop
30007f44:	42008000 	.word	0x42008000
30007f48:	80000500 	.word	0x80000500
30007f4c:	30009ab5 	.word	0x30009ab5
30007f50:	431bde83 	.word	0x431bde83
30007f54:	2001c700 	.word	0x2001c700
30007f58:	7ffffaff 	.word	0x7ffffaff

30007f5c <DDR_PHY_ChipInfo.part.0>:
30007f5c:	b508      	push	{r3, lr}
30007f5e:	4b24      	ldr	r3, [pc, #144]	; (30007ff0 <DDR_PHY_ChipInfo.part.0+0x94>)
30007f60:	4798      	blx	r3
30007f62:	280e      	cmp	r0, #14
30007f64:	d80d      	bhi.n	30007f82 <DDR_PHY_ChipInfo.part.0+0x26>
30007f66:	2801      	cmp	r0, #1
30007f68:	d912      	bls.n	30007f90 <DDR_PHY_ChipInfo.part.0+0x34>
30007f6a:	1e83      	subs	r3, r0, #2
30007f6c:	2b0c      	cmp	r3, #12
30007f6e:	d80f      	bhi.n	30007f90 <DDR_PHY_ChipInfo.part.0+0x34>
30007f70:	e8df f003 	tbb	[pc, r3]
30007f74:	210e1d19 	.word	0x210e1d19
30007f78:	2a0e260e 	.word	0x2a0e260e
30007f7c:	340e0e2f 	.word	0x340e0e2f
30007f80:	39          	.byte	0x39
30007f81:	00          	.byte	0x00
30007f82:	28e0      	cmp	r0, #224	; 0xe0
30007f84:	d104      	bne.n	30007f90 <DDR_PHY_ChipInfo.part.0+0x34>
30007f86:	2301      	movs	r3, #1
30007f88:	4a1a      	ldr	r2, [pc, #104]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f8a:	4618      	mov	r0, r3
30007f8c:	7013      	strb	r3, [r2, #0]
30007f8e:	bd08      	pop	{r3, pc}
30007f90:	4b19      	ldr	r3, [pc, #100]	; (30007ff8 <DDR_PHY_ChipInfo.part.0+0x9c>)
30007f92:	2002      	movs	r0, #2
30007f94:	4a19      	ldr	r2, [pc, #100]	; (30007ffc <DDR_PHY_ChipInfo.part.0+0xa0>)
30007f96:	4619      	mov	r1, r3
30007f98:	f004 f8d2 	bl	3000c140 <rtk_log_write>
30007f9c:	2300      	movs	r3, #0
30007f9e:	4a15      	ldr	r2, [pc, #84]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fa0:	4618      	mov	r0, r3
30007fa2:	7013      	strb	r3, [r2, #0]
30007fa4:	bd08      	pop	{r3, pc}
30007fa6:	4b13      	ldr	r3, [pc, #76]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fa8:	2202      	movs	r2, #2
30007faa:	701a      	strb	r2, [r3, #0]
30007fac:	bd08      	pop	{r3, pc}
30007fae:	4b11      	ldr	r3, [pc, #68]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fb0:	2203      	movs	r2, #3
30007fb2:	701a      	strb	r2, [r3, #0]
30007fb4:	bd08      	pop	{r3, pc}
30007fb6:	2304      	movs	r3, #4
30007fb8:	4a0e      	ldr	r2, [pc, #56]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fba:	4618      	mov	r0, r3
30007fbc:	7013      	strb	r3, [r2, #0]
30007fbe:	bd08      	pop	{r3, pc}
30007fc0:	4b0c      	ldr	r3, [pc, #48]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fc2:	2207      	movs	r2, #7
30007fc4:	701a      	strb	r2, [r3, #0]
30007fc6:	bd08      	pop	{r3, pc}
30007fc8:	2305      	movs	r3, #5
30007fca:	4a0a      	ldr	r2, [pc, #40]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fcc:	4618      	mov	r0, r3
30007fce:	7013      	strb	r3, [r2, #0]
30007fd0:	bd08      	pop	{r3, pc}
30007fd2:	2306      	movs	r3, #6
30007fd4:	4a07      	ldr	r2, [pc, #28]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fd6:	4618      	mov	r0, r3
30007fd8:	7013      	strb	r3, [r2, #0]
30007fda:	bd08      	pop	{r3, pc}
30007fdc:	2308      	movs	r3, #8
30007fde:	4a05      	ldr	r2, [pc, #20]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fe0:	4618      	mov	r0, r3
30007fe2:	7013      	strb	r3, [r2, #0]
30007fe4:	bd08      	pop	{r3, pc}
30007fe6:	2309      	movs	r3, #9
30007fe8:	4a02      	ldr	r2, [pc, #8]	; (30007ff4 <DDR_PHY_ChipInfo.part.0+0x98>)
30007fea:	4618      	mov	r0, r3
30007fec:	7013      	strb	r3, [r2, #0]
30007fee:	bd08      	pop	{r3, pc}
30007ff0:	30007d81 	.word	0x30007d81
30007ff4:	3000f24d 	.word	0x3000f24d
30007ff8:	3000d55c 	.word	0x3000d55c
30007ffc:	3000d564 	.word	0x3000d564

30008000 <DDR_PHY_ChipInfo_ddrtype>:
30008000:	b510      	push	{r4, lr}
30008002:	4c04      	ldr	r4, [pc, #16]	; (30008014 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30008004:	7820      	ldrb	r0, [r4, #0]
30008006:	b100      	cbz	r0, 3000800a <DDR_PHY_ChipInfo_ddrtype+0xa>
30008008:	bd10      	pop	{r4, pc}
3000800a:	4b03      	ldr	r3, [pc, #12]	; (30008018 <DDR_PHY_ChipInfo_ddrtype+0x18>)
3000800c:	4798      	blx	r3
3000800e:	7020      	strb	r0, [r4, #0]
30008010:	bd10      	pop	{r4, pc}
30008012:	bf00      	nop
30008014:	3000ff60 	.word	0x3000ff60
30008018:	30007e31 	.word	0x30007e31

3000801c <DDR_PHY_StructInit>:
3000801c:	f240 2315 	movw	r3, #533	; 0x215
30008020:	6583      	str	r3, [r0, #88]	; 0x58
30008022:	4770      	bx	lr

30008024 <DDR_PHY_DLL_CLK_DIV>:
30008024:	b570      	push	{r4, r5, r6, lr}
30008026:	4d5a      	ldr	r5, [pc, #360]	; (30008190 <DDR_PHY_DLL_CLK_DIV+0x16c>)
30008028:	495a      	ldr	r1, [pc, #360]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000802a:	fba5 4200 	umull	r4, r2, r5, r0
3000802e:	4b5a      	ldr	r3, [pc, #360]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x174>)
30008030:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30008032:	0952      	lsrs	r2, r2, #5
30008034:	4c59      	ldr	r4, [pc, #356]	; (3000819c <DDR_PHY_DLL_CLK_DIV+0x178>)
30008036:	4403      	add	r3, r0
30008038:	4859      	ldr	r0, [pc, #356]	; (300081a0 <DDR_PHY_DLL_CLK_DIV+0x17c>)
3000803a:	3a03      	subs	r2, #3
3000803c:	4034      	ands	r4, r6
3000803e:	eb02 0682 	add.w	r6, r2, r2, lsl #2
30008042:	eba3 03c6 	sub.w	r3, r3, r6, lsl #3
30008046:	02db      	lsls	r3, r3, #11
30008048:	fba5 5303 	umull	r5, r3, r5, r3
3000804c:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30008050:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008054:	4320      	orrs	r0, r4
30008056:	0214      	lsls	r4, r2, #8
30008058:	b2d2      	uxtb	r2, r2
3000805a:	6248      	str	r0, [r1, #36]	; 0x24
3000805c:	b2a4      	uxth	r4, r4
3000805e:	6a88      	ldr	r0, [r1, #40]	; 0x28
30008060:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30008064:	4304      	orrs	r4, r0
30008066:	484f      	ldr	r0, [pc, #316]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x180>)
30008068:	628c      	str	r4, [r1, #40]	; 0x28
3000806a:	6a4c      	ldr	r4, [r1, #36]	; 0x24
3000806c:	4020      	ands	r0, r4
3000806e:	2401      	movs	r4, #1
30008070:	4303      	orrs	r3, r0
30008072:	624b      	str	r3, [r1, #36]	; 0x24
30008074:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30008076:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
3000807a:	431a      	orrs	r2, r3
3000807c:	4b4a      	ldr	r3, [pc, #296]	; (300081a8 <DDR_PHY_DLL_CLK_DIV+0x184>)
3000807e:	628a      	str	r2, [r1, #40]	; 0x28
30008080:	678c      	str	r4, [r1, #120]	; 0x78
30008082:	7818      	ldrb	r0, [r3, #0]
30008084:	28ff      	cmp	r0, #255	; 0xff
30008086:	d076      	beq.n	30008176 <DDR_PHY_DLL_CLK_DIV+0x152>
30008088:	4c48      	ldr	r4, [pc, #288]	; (300081ac <DDR_PHY_DLL_CLK_DIV+0x188>)
3000808a:	0145      	lsls	r5, r0, #5
3000808c:	4b41      	ldr	r3, [pc, #260]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000808e:	eb04 1040 	add.w	r0, r4, r0, lsl #5
30008092:	5962      	ldr	r2, [r4, r5]
30008094:	4e46      	ldr	r6, [pc, #280]	; (300081b0 <DDR_PHY_DLL_CLK_DIV+0x18c>)
30008096:	611a      	str	r2, [r3, #16]
30008098:	6842      	ldr	r2, [r0, #4]
3000809a:	615a      	str	r2, [r3, #20]
3000809c:	6882      	ldr	r2, [r0, #8]
3000809e:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
300080a2:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
300080a6:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300080aa:	609a      	str	r2, [r3, #8]
300080ac:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300080b0:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300080b4:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300080b8:	7830      	ldrb	r0, [r6, #0]
300080ba:	2800      	cmp	r0, #0
300080bc:	d057      	beq.n	3000816e <DDR_PHY_DLL_CLK_DIV+0x14a>
300080be:	2801      	cmp	r0, #1
300080c0:	eb04 0305 	add.w	r3, r4, r5
300080c4:	4a33      	ldr	r2, [pc, #204]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080c6:	d037      	beq.n	30008138 <DDR_PHY_DLL_CLK_DIV+0x114>
300080c8:	2122      	movs	r1, #34	; 0x22
300080ca:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300080ce:	6959      	ldr	r1, [r3, #20]
300080d0:	f8c2 1080 	str.w	r1, [r2, #128]	; 0x80
300080d4:	e9d3 1306 	ldrd	r1, r3, [r3, #24]
300080d8:	f8c2 1084 	str.w	r1, [r2, #132]	; 0x84
300080dc:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
300080e0:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300080e4:	4933      	ldr	r1, [pc, #204]	; (300081b4 <DDR_PHY_DLL_CLK_DIV+0x190>)
300080e6:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300080ea:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300080ee:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300080f2:	009b      	lsls	r3, r3, #2
300080f4:	d516      	bpl.n	30008124 <DDR_PHY_DLL_CLK_DIV+0x100>
300080f6:	2800      	cmp	r0, #0
300080f8:	d044      	beq.n	30008184 <DDR_PHY_DLL_CLK_DIV+0x160>
300080fa:	4b26      	ldr	r3, [pc, #152]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080fc:	4a2e      	ldr	r2, [pc, #184]	; (300081b8 <DDR_PHY_DLL_CLK_DIV+0x194>)
300080fe:	601a      	str	r2, [r3, #0]
30008100:	4b24      	ldr	r3, [pc, #144]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008102:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008106:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000810a:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
3000810e:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008112:	699a      	ldr	r2, [r3, #24]
30008114:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
30008118:	619a      	str	r2, [r3, #24]
3000811a:	699a      	ldr	r2, [r3, #24]
3000811c:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008120:	619a      	str	r2, [r3, #24]
30008122:	bd70      	pop	{r4, r5, r6, pc}
30008124:	b920      	cbnz	r0, 30008130 <DDR_PHY_DLL_CLK_DIV+0x10c>
30008126:	4b25      	ldr	r3, [pc, #148]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008128:	4798      	blx	r3
3000812a:	2801      	cmp	r0, #1
3000812c:	7030      	strb	r0, [r6, #0]
3000812e:	d025      	beq.n	3000817c <DDR_PHY_DLL_CLK_DIV+0x158>
30008130:	4b18      	ldr	r3, [pc, #96]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008132:	4a23      	ldr	r2, [pc, #140]	; (300081c0 <DDR_PHY_DLL_CLK_DIV+0x19c>)
30008134:	601a      	str	r2, [r3, #0]
30008136:	e7e3      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008138:	2033      	movs	r0, #51	; 0x33
3000813a:	695c      	ldr	r4, [r3, #20]
3000813c:	491d      	ldr	r1, [pc, #116]	; (300081b4 <DDR_PHY_DLL_CLK_DIV+0x190>)
3000813e:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
30008142:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
30008146:	e9d3 0306 	ldrd	r0, r3, [r3, #24]
3000814a:	f8c2 0084 	str.w	r0, [r2, #132]	; 0x84
3000814e:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
30008152:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008156:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
3000815a:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
3000815e:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008162:	009a      	lsls	r2, r3, #2
30008164:	d50a      	bpl.n	3000817c <DDR_PHY_DLL_CLK_DIV+0x158>
30008166:	4b0b      	ldr	r3, [pc, #44]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008168:	4a16      	ldr	r2, [pc, #88]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x1a0>)
3000816a:	601a      	str	r2, [r3, #0]
3000816c:	e7c8      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0xdc>
3000816e:	4b13      	ldr	r3, [pc, #76]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008170:	4798      	blx	r3
30008172:	7030      	strb	r0, [r6, #0]
30008174:	e7a3      	b.n	300080be <DDR_PHY_DLL_CLK_DIV+0x9a>
30008176:	f7ff fef1 	bl	30007f5c <DDR_PHY_ChipInfo.part.0>
3000817a:	e785      	b.n	30008088 <DDR_PHY_DLL_CLK_DIV+0x64>
3000817c:	4b05      	ldr	r3, [pc, #20]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000817e:	4a12      	ldr	r2, [pc, #72]	; (300081c8 <DDR_PHY_DLL_CLK_DIV+0x1a4>)
30008180:	601a      	str	r2, [r3, #0]
30008182:	e7bd      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0xdc>
30008184:	4b0d      	ldr	r3, [pc, #52]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x198>)
30008186:	4798      	blx	r3
30008188:	2801      	cmp	r0, #1
3000818a:	7030      	strb	r0, [r6, #0]
3000818c:	d1b5      	bne.n	300080fa <DDR_PHY_DLL_CLK_DIV+0xd6>
3000818e:	e7ea      	b.n	30008166 <DDR_PHY_DLL_CLK_DIV+0x142>
30008190:	cccccccd 	.word	0xcccccccd
30008194:	41011000 	.word	0x41011000
30008198:	001fff88 	.word	0x001fff88
3000819c:	f800ffff 	.word	0xf800ffff
300081a0:	07ff0000 	.word	0x07ff0000
300081a4:	fffff800 	.word	0xfffff800
300081a8:	3000f24d 	.word	0x3000f24d
300081ac:	3000d584 	.word	0x3000d584
300081b0:	3000ff60 	.word	0x3000ff60
300081b4:	42008000 	.word	0x42008000
300081b8:	4c000821 	.word	0x4c000821
300081bc:	30007e31 	.word	0x30007e31
300081c0:	40000021 	.word	0x40000021
300081c4:	4c000903 	.word	0x4c000903
300081c8:	40000103 	.word	0x40000103

300081cc <DDR_PHY_CRT_Init>:
300081cc:	492f      	ldr	r1, [pc, #188]	; (3000828c <DDR_PHY_CRT_Init+0xc0>)
300081ce:	2005      	movs	r0, #5
300081d0:	4a2f      	ldr	r2, [pc, #188]	; (30008290 <DDR_PHY_CRT_Init+0xc4>)
300081d2:	b510      	push	{r4, lr}
300081d4:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
300081d8:	4c2e      	ldr	r4, [pc, #184]	; (30008294 <DDR_PHY_CRT_Init+0xc8>)
300081da:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300081de:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
300081e2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081e6:	f043 0301 	orr.w	r3, r3, #1
300081ea:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081ee:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081f2:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300081f6:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081fa:	68e3      	ldr	r3, [r4, #12]
300081fc:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008200:	60e3      	str	r3, [r4, #12]
30008202:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008206:	f043 0302 	orr.w	r3, r3, #2
3000820a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000820e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008212:	f023 0308 	bic.w	r3, r3, #8
30008216:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000821a:	4790      	blx	r2
3000821c:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008220:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008224:	491c      	ldr	r1, [pc, #112]	; (30008298 <DDR_PHY_CRT_Init+0xcc>)
30008226:	f042 0204 	orr.w	r2, r2, #4
3000822a:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000822e:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008232:	f042 0208 	orr.w	r2, r2, #8
30008236:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
3000823a:	6aa2      	ldr	r2, [r4, #40]	; 0x28
3000823c:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
30008240:	62a2      	str	r2, [r4, #40]	; 0x28
30008242:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008244:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
30008248:	62a2      	str	r2, [r4, #40]	; 0x28
3000824a:	69e2      	ldr	r2, [r4, #28]
3000824c:	f042 0208 	orr.w	r2, r2, #8
30008250:	61e2      	str	r2, [r4, #28]
30008252:	68a2      	ldr	r2, [r4, #8]
30008254:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008258:	60a2      	str	r2, [r4, #8]
3000825a:	6862      	ldr	r2, [r4, #4]
3000825c:	4313      	orrs	r3, r2
3000825e:	6063      	str	r3, [r4, #4]
30008260:	68a3      	ldr	r3, [r4, #8]
30008262:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008266:	60a3      	str	r3, [r4, #8]
30008268:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
3000826c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008270:	f023 0303 	bic.w	r3, r3, #3
30008274:	f043 0302 	orr.w	r3, r3, #2
30008278:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000827c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008280:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30008284:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008288:	bd10      	pop	{r4, pc}
3000828a:	bf00      	nop
3000828c:	42008000 	.word	0x42008000
30008290:	00009b2d 	.word	0x00009b2d
30008294:	41011000 	.word	0x41011000
30008298:	90161f00 	.word	0x90161f00

3000829c <DDR_PHY_R240_ZQ_CAL>:
3000829c:	b538      	push	{r3, r4, r5, lr}
3000829e:	4b1e      	ldr	r3, [pc, #120]	; (30008318 <DDR_PHY_R240_ZQ_CAL+0x7c>)
300082a0:	7818      	ldrb	r0, [r3, #0]
300082a2:	28ff      	cmp	r0, #255	; 0xff
300082a4:	d034      	beq.n	30008310 <DDR_PHY_R240_ZQ_CAL+0x74>
300082a6:	4a1d      	ldr	r2, [pc, #116]	; (3000831c <DDR_PHY_R240_ZQ_CAL+0x80>)
300082a8:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300082ac:	4b1c      	ldr	r3, [pc, #112]	; (30008320 <DDR_PHY_R240_ZQ_CAL+0x84>)
300082ae:	2400      	movs	r4, #0
300082b0:	eb02 0180 	add.w	r1, r2, r0, lsl #2
300082b4:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
300082b8:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
300082bc:	7c0a      	ldrb	r2, [r1, #16]
300082be:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
300082c2:	0052      	lsls	r2, r2, #1
300082c4:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300082c8:	4302      	orrs	r2, r0
300082ca:	6848      	ldr	r0, [r1, #4]
300082cc:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300082d0:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
300082d4:	4a13      	ldr	r2, [pc, #76]	; (30008324 <DDR_PHY_R240_ZQ_CAL+0x88>)
300082d6:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300082da:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300082de:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300082e2:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
300082e6:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
300082ea:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300082ee:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
300082f2:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300082f6:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300082fa:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300082fe:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008302:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008306:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000830a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000830e:	bd38      	pop	{r3, r4, r5, pc}
30008310:	f7ff fe24 	bl	30007f5c <DDR_PHY_ChipInfo.part.0>
30008314:	e7c7      	b.n	300082a6 <DDR_PHY_R240_ZQ_CAL+0xa>
30008316:	bf00      	nop
30008318:	3000f24d 	.word	0x3000f24d
3000831c:	3000d6c4 	.word	0x3000d6c4
30008320:	41011000 	.word	0x41011000
30008324:	00262626 	.word	0x00262626

30008328 <DDR_PHY_ZQ_SET_SEL>:
30008328:	b410      	push	{r4}
3000832a:	4b25      	ldr	r3, [pc, #148]	; (300083c0 <DDR_PHY_ZQ_SET_SEL+0x98>)
3000832c:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
30008330:	4a24      	ldr	r2, [pc, #144]	; (300083c4 <DDR_PHY_ZQ_SET_SEL+0x9c>)
30008332:	4c25      	ldr	r4, [pc, #148]	; (300083c8 <DDR_PHY_ZQ_SET_SEL+0xa0>)
30008334:	4925      	ldr	r1, [pc, #148]	; (300083cc <DDR_PHY_ZQ_SET_SEL+0xa4>)
30008336:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
3000833a:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000833e:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
30008342:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30008346:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
3000834a:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
3000834e:	f64f 74ff 	movw	r4, #65535	; 0xffff
30008352:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30008356:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
3000835a:	2200      	movs	r2, #0
3000835c:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008360:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30008364:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008368:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
3000836c:	f04f 31ff 	mov.w	r1, #4294967295
30008370:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30008374:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008378:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
3000837c:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008380:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
30008384:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008388:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
3000838c:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008390:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30008394:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30008398:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
3000839c:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
300083a0:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
300083a4:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
300083a8:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
300083ac:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
300083b0:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
300083b4:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
300083b8:	f85d 4b04 	ldr.w	r4, [sp], #4
300083bc:	4770      	bx	lr
300083be:	bf00      	nop
300083c0:	41011000 	.word	0x41011000
300083c4:	ffff1111 	.word	0xffff1111
300083c8:	ffff3333 	.word	0xffff3333
300083cc:	ffff0000 	.word	0xffff0000

300083d0 <DDR_PHY_DELAY_TAP_SET>:
300083d0:	b510      	push	{r4, lr}
300083d2:	4b15      	ldr	r3, [pc, #84]	; (30008428 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083d4:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300083d8:	4914      	ldr	r1, [pc, #80]	; (3000842c <DDR_PHY_DELAY_TAP_SET+0x5c>)
300083da:	4c15      	ldr	r4, [pc, #84]	; (30008430 <DDR_PHY_DELAY_TAP_SET+0x60>)
300083dc:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300083e0:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
300083e4:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300083e8:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300083ec:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300083f0:	7820      	ldrb	r0, [r4, #0]
300083f2:	b1a0      	cbz	r0, 3000841e <DDR_PHY_DELAY_TAP_SET+0x4e>
300083f4:	2801      	cmp	r0, #1
300083f6:	4b0c      	ldr	r3, [pc, #48]	; (30008428 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083f8:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
300083fc:	bf0c      	ite	eq
300083fe:	4a0d      	ldreq	r2, [pc, #52]	; (30008434 <DDR_PHY_DELAY_TAP_SET+0x64>)
30008400:	4a0d      	ldrne	r2, [pc, #52]	; (30008438 <DDR_PHY_DELAY_TAP_SET+0x68>)
30008402:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30008406:	f640 0288 	movw	r2, #2184	; 0x888
3000840a:	4b07      	ldr	r3, [pc, #28]	; (30008428 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000840c:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008410:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30008414:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008418:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000841c:	bd10      	pop	{r4, pc}
3000841e:	4b07      	ldr	r3, [pc, #28]	; (3000843c <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008420:	4798      	blx	r3
30008422:	7020      	strb	r0, [r4, #0]
30008424:	e7e6      	b.n	300083f4 <DDR_PHY_DELAY_TAP_SET+0x24>
30008426:	bf00      	nop
30008428:	41011000 	.word	0x41011000
3000842c:	20888888 	.word	0x20888888
30008430:	3000ff60 	.word	0x3000ff60
30008434:	7c033333 	.word	0x7c033333
30008438:	7c011111 	.word	0x7c011111
3000843c:	30007e31 	.word	0x30007e31

30008440 <DDR_PHY_READ_CTRL>:
30008440:	4b10      	ldr	r3, [pc, #64]	; (30008484 <DDR_PHY_READ_CTRL+0x44>)
30008442:	b510      	push	{r4, lr}
30008444:	7818      	ldrb	r0, [r3, #0]
30008446:	28ff      	cmp	r0, #255	; 0xff
30008448:	d019      	beq.n	3000847e <DDR_PHY_READ_CTRL+0x3e>
3000844a:	4a0f      	ldr	r2, [pc, #60]	; (30008488 <DDR_PHY_READ_CTRL+0x48>)
3000844c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008450:	4b0e      	ldr	r3, [pc, #56]	; (3000848c <DDR_PHY_READ_CTRL+0x4c>)
30008452:	2108      	movs	r1, #8
30008454:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30008458:	22fe      	movs	r2, #254	; 0xfe
3000845a:	7c44      	ldrb	r4, [r0, #17]
3000845c:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30008460:	7c84      	ldrb	r4, [r0, #18]
30008462:	7cc0      	ldrb	r0, [r0, #19]
30008464:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30008468:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
3000846c:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008470:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30008474:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008478:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
3000847c:	bd10      	pop	{r4, pc}
3000847e:	f7ff fd6d 	bl	30007f5c <DDR_PHY_ChipInfo.part.0>
30008482:	e7e2      	b.n	3000844a <DDR_PHY_READ_CTRL+0xa>
30008484:	3000f24d 	.word	0x3000f24d
30008488:	3000d6c4 	.word	0x3000d6c4
3000848c:	41011000 	.word	0x41011000

30008490 <DDR_PHY_READ_LEVELING>:
30008490:	b508      	push	{r3, lr}
30008492:	4b21      	ldr	r3, [pc, #132]	; (30008518 <DDR_PHY_READ_LEVELING+0x88>)
30008494:	7818      	ldrb	r0, [r3, #0]
30008496:	28ff      	cmp	r0, #255	; 0xff
30008498:	d03b      	beq.n	30008512 <DDR_PHY_READ_LEVELING+0x82>
3000849a:	4b20      	ldr	r3, [pc, #128]	; (3000851c <DDR_PHY_READ_LEVELING+0x8c>)
3000849c:	b1e8      	cbz	r0, 300084da <DDR_PHY_READ_LEVELING+0x4a>
3000849e:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
300084a2:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300084a6:	220f      	movs	r2, #15
300084a8:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
300084ac:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300084b0:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300084b4:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300084b8:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300084bc:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300084c0:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300084c4:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
300084c8:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300084cc:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300084d0:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300084d4:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300084d8:	bd08      	pop	{r3, pc}
300084da:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300084de:	2110      	movs	r1, #16
300084e0:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300084e4:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300084e8:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300084ec:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300084f0:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
300084f4:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
300084f8:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
300084fc:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008500:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008504:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008508:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
3000850c:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008510:	bd08      	pop	{r3, pc}
30008512:	f7ff fd23 	bl	30007f5c <DDR_PHY_ChipInfo.part.0>
30008516:	e7c0      	b.n	3000849a <DDR_PHY_READ_LEVELING+0xa>
30008518:	3000f24d 	.word	0x3000f24d
3000851c:	41011000 	.word	0x41011000

30008520 <DDR_PHY_Init>:
30008520:	b510      	push	{r4, lr}
30008522:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008524:	4c2f      	ldr	r4, [pc, #188]	; (300085e4 <DDR_PHY_Init+0xc4>)
30008526:	f7ff fd7d 	bl	30008024 <DDR_PHY_DLL_CLK_DIV>
3000852a:	f7ff fe4f 	bl	300081cc <DDR_PHY_CRT_Init>
3000852e:	f7ff feb5 	bl	3000829c <DDR_PHY_R240_ZQ_CAL>
30008532:	f7ff fef9 	bl	30008328 <DDR_PHY_ZQ_SET_SEL>
30008536:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
3000853a:	2300      	movs	r3, #0
3000853c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008540:	4829      	ldr	r0, [pc, #164]	; (300085e8 <DDR_PHY_Init+0xc8>)
30008542:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008546:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
3000854a:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
3000854e:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
30008552:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008556:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
3000855a:	f7ff ff39 	bl	300083d0 <DDR_PHY_DELAY_TAP_SET>
3000855e:	f7ff ff6f 	bl	30008440 <DDR_PHY_READ_CTRL>
30008562:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008566:	4a21      	ldr	r2, [pc, #132]	; (300085ec <DDR_PHY_Init+0xcc>)
30008568:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
3000856c:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008570:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008574:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008578:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
3000857c:	f7ff ff88 	bl	30008490 <DDR_PHY_READ_LEVELING>
30008580:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
30008584:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008588:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
3000858c:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008590:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008594:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008598:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
3000859c:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
300085a0:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
300085a4:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
300085a8:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
300085ac:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085b0:	f043 0303 	orr.w	r3, r3, #3
300085b4:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085b8:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085bc:	f043 030c 	orr.w	r3, r3, #12
300085c0:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085c4:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085c8:	f023 0303 	bic.w	r3, r3, #3
300085cc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085d0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085d4:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300085d8:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300085dc:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085e0:	bd10      	pop	{r4, pc}
300085e2:	bf00      	nop
300085e4:	41011000 	.word	0x41011000
300085e8:	00045500 	.word	0x00045500
300085ec:	11117777 	.word	0x11117777

300085f0 <DDR_PHY_CAL>:
300085f0:	4770      	bx	lr
300085f2:	bf00      	nop

300085f4 <DDR_PHY_AutoGating>:
300085f4:	4a0c      	ldr	r2, [pc, #48]	; (30008628 <DDR_PHY_AutoGating+0x34>)
300085f6:	2032      	movs	r0, #50	; 0x32
300085f8:	490c      	ldr	r1, [pc, #48]	; (3000862c <DDR_PHY_AutoGating+0x38>)
300085fa:	b508      	push	{r3, lr}
300085fc:	6893      	ldr	r3, [r2, #8]
300085fe:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008602:	6093      	str	r3, [r2, #8]
30008604:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008608:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
3000860c:	f043 0314 	orr.w	r3, r3, #20
30008610:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008614:	4788      	blx	r1
30008616:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000861a:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000861e:	f043 0303 	orr.w	r3, r3, #3
30008622:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008626:	bd08      	pop	{r3, pc}
30008628:	41011000 	.word	0x41011000
3000862c:	00009b2d 	.word	0x00009b2d

30008630 <flash_init_userdef>:
30008630:	4b14      	ldr	r3, [pc, #80]	; (30008684 <flash_init_userdef+0x54>)
30008632:	2201      	movs	r2, #1
30008634:	2140      	movs	r1, #64	; 0x40
30008636:	f44f 6000 	mov.w	r0, #2048	; 0x800
3000863a:	b430      	push	{r4, r5}
3000863c:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008640:	2502      	movs	r5, #2
30008642:	2400      	movs	r4, #0
30008644:	f640 0104 	movw	r1, #2052	; 0x804
30008648:	601a      	str	r2, [r3, #0]
3000864a:	22bb      	movs	r2, #187	; 0xbb
3000864c:	83d8      	strh	r0, [r3, #30]
3000864e:	8419      	strh	r1, [r3, #32]
30008650:	203b      	movs	r0, #59	; 0x3b
30008652:	21eb      	movs	r1, #235	; 0xeb
30008654:	629a      	str	r2, [r3, #40]	; 0x28
30008656:	4a0c      	ldr	r2, [pc, #48]	; (30008688 <flash_init_userdef+0x58>)
30008658:	6258      	str	r0, [r3, #36]	; 0x24
3000865a:	6319      	str	r1, [r3, #48]	; 0x30
3000865c:	f64a 3120 	movw	r1, #43808	; 0xab20
30008660:	655a      	str	r2, [r3, #84]	; 0x54
30008662:	22b9      	movs	r2, #185	; 0xb9
30008664:	4809      	ldr	r0, [pc, #36]	; (3000868c <flash_init_userdef+0x5c>)
30008666:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
3000866a:	6598      	str	r0, [r3, #88]	; 0x58
3000866c:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008670:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008674:	2406      	movs	r4, #6
30008676:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
3000867a:	246b      	movs	r4, #107	; 0x6b
3000867c:	62dc      	str	r4, [r3, #44]	; 0x2c
3000867e:	bc30      	pop	{r4, r5}
30008680:	4770      	bx	lr
30008682:	bf00      	nop
30008684:	2001c01c 	.word	0x2001c01c
30008688:	00059f06 	.word	0x00059f06
3000868c:	d8600001 	.word	0xd8600001

30008690 <nand_init_userdef>:
30008690:	4b18      	ldr	r3, [pc, #96]	; (300086f4 <nand_init_userdef+0x64>)
30008692:	2001      	movs	r0, #1
30008694:	2200      	movs	r2, #0
30008696:	2102      	movs	r1, #2
30008698:	b410      	push	{r4}
3000869a:	6018      	str	r0, [r3, #0]
3000869c:	f640 0408 	movw	r4, #2056	; 0x808
300086a0:	6159      	str	r1, [r3, #20]
300086a2:	83dc      	strh	r4, [r3, #30]
300086a4:	f44f 6400 	mov.w	r4, #2048	; 0x800
300086a8:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
300086ac:	841c      	strh	r4, [r3, #32]
300086ae:	246b      	movs	r4, #107	; 0x6b
300086b0:	619a      	str	r2, [r3, #24]
300086b2:	62dc      	str	r4, [r3, #44]	; 0x2c
300086b4:	4c10      	ldr	r4, [pc, #64]	; (300086f8 <nand_init_userdef+0x68>)
300086b6:	775a      	strb	r2, [r3, #29]
300086b8:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
300086bc:	629a      	str	r2, [r3, #40]	; 0x28
300086be:	631a      	str	r2, [r3, #48]	; 0x30
300086c0:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
300086c4:	65da      	str	r2, [r3, #92]	; 0x5c
300086c6:	e9c3 0003 	strd	r0, r0, [r3, #12]
300086ca:	203b      	movs	r0, #59	; 0x3b
300086cc:	6258      	str	r0, [r3, #36]	; 0x24
300086ce:	200b      	movs	r0, #11
300086d0:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
300086d4:	6698      	str	r0, [r3, #104]	; 0x68
300086d6:	2008      	movs	r0, #8
300086d8:	4908      	ldr	r1, [pc, #32]	; (300086fc <nand_init_userdef+0x6c>)
300086da:	64d8      	str	r0, [r3, #76]	; 0x4c
300086dc:	f241 0013 	movw	r0, #4115	; 0x1013
300086e0:	6599      	str	r1, [r3, #88]	; 0x58
300086e2:	4907      	ldr	r1, [pc, #28]	; (30008700 <nand_init_userdef+0x70>)
300086e4:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
300086e8:	6719      	str	r1, [r3, #112]	; 0x70
300086ea:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
300086ee:	f85d 4b04 	ldr.w	r4, [sp], #4
300086f2:	4770      	bx	lr
300086f4:	2001c01c 	.word	0x2001c01c
300086f8:	000f9f06 	.word	0x000f9f06
300086fc:	d800001f 	.word	0xd800001f
30008700:	10300804 	.word	0x10300804

30008704 <flash_get_layout_info>:
30008704:	b570      	push	{r4, r5, r6, lr}
30008706:	4b0f      	ldr	r3, [pc, #60]	; (30008744 <flash_get_layout_info+0x40>)
30008708:	4615      	mov	r5, r2
3000870a:	4604      	mov	r4, r0
3000870c:	460e      	mov	r6, r1
3000870e:	4798      	blx	r3
30008710:	4b0d      	ldr	r3, [pc, #52]	; (30008748 <flash_get_layout_info+0x44>)
30008712:	4a0e      	ldr	r2, [pc, #56]	; (3000874c <flash_get_layout_info+0x48>)
30008714:	2800      	cmp	r0, #0
30008716:	bf14      	ite	ne
30008718:	469c      	movne	ip, r3
3000871a:	4694      	moveq	ip, r2
3000871c:	f8dc 3000 	ldr.w	r3, [ip]
30008720:	2bff      	cmp	r3, #255	; 0xff
30008722:	d104      	bne.n	3000872e <flash_get_layout_info+0x2a>
30008724:	e00d      	b.n	30008742 <flash_get_layout_info+0x3e>
30008726:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
3000872a:	2bff      	cmp	r3, #255	; 0xff
3000872c:	d009      	beq.n	30008742 <flash_get_layout_info+0x3e>
3000872e:	429c      	cmp	r4, r3
30008730:	d1f9      	bne.n	30008726 <flash_get_layout_info+0x22>
30008732:	b116      	cbz	r6, 3000873a <flash_get_layout_info+0x36>
30008734:	f8dc 3004 	ldr.w	r3, [ip, #4]
30008738:	6033      	str	r3, [r6, #0]
3000873a:	b115      	cbz	r5, 30008742 <flash_get_layout_info+0x3e>
3000873c:	f8dc 3008 	ldr.w	r3, [ip, #8]
30008740:	602b      	str	r3, [r5, #0]
30008742:	bd70      	pop	{r4, r5, r6, pc}
30008744:	30009e65 	.word	0x30009e65
30008748:	3000f328 	.word	0x3000f328
3000874c:	3000f250 	.word	0x3000f250

30008750 <NAND_CHECK_IS_BAD_BLOCK>:
30008750:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008754:	4e24      	ldr	r6, [pc, #144]	; (300087e8 <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30008756:	0907      	lsrs	r7, r0, #4
30008758:	0045      	lsls	r5, r0, #1
3000875a:	b085      	sub	sp, #20
3000875c:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008760:	f005 051e 	and.w	r5, r5, #30
30008764:	40eb      	lsrs	r3, r5
30008766:	f003 0303 	and.w	r3, r3, #3
3000876a:	2b02      	cmp	r3, #2
3000876c:	d02c      	beq.n	300087c8 <NAND_CHECK_IS_BAD_BLOCK+0x78>
3000876e:	2b01      	cmp	r3, #1
30008770:	d026      	beq.n	300087c0 <NAND_CHECK_IS_BAD_BLOCK+0x70>
30008772:	4b1e      	ldr	r3, [pc, #120]	; (300087ec <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008774:	f04f 0801 	mov.w	r8, #1
30008778:	2204      	movs	r2, #4
3000877a:	4604      	mov	r4, r0
3000877c:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008780:	0180      	lsls	r0, r0, #6
30008782:	ab03      	add	r3, sp, #12
30008784:	f8df 9070 	ldr.w	r9, [pc, #112]	; 300087f8 <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30008788:	fa08 f101 	lsl.w	r1, r8, r1
3000878c:	47c8      	blx	r9
3000878e:	f89d 300c 	ldrb.w	r3, [sp, #12]
30008792:	f89d 200d 	ldrb.w	r2, [sp, #13]
30008796:	4013      	ands	r3, r2
30008798:	2bff      	cmp	r3, #255	; 0xff
3000879a:	d019      	beq.n	300087d0 <NAND_CHECK_IS_BAD_BLOCK+0x80>
3000879c:	2002      	movs	r0, #2
3000879e:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
300087a2:	4b13      	ldr	r3, [pc, #76]	; (300087f0 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
300087a4:	fa00 f505 	lsl.w	r5, r0, r5
300087a8:	9400      	str	r4, [sp, #0]
300087aa:	4619      	mov	r1, r3
300087ac:	432a      	orrs	r2, r5
300087ae:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
300087b2:	4a10      	ldr	r2, [pc, #64]	; (300087f4 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
300087b4:	f003 fcc4 	bl	3000c140 <rtk_log_write>
300087b8:	4640      	mov	r0, r8
300087ba:	b005      	add	sp, #20
300087bc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087c0:	2000      	movs	r0, #0
300087c2:	b005      	add	sp, #20
300087c4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087c8:	2001      	movs	r0, #1
300087ca:	b005      	add	sp, #20
300087cc:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087d0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
300087d4:	fa08 f005 	lsl.w	r0, r8, r5
300087d8:	4303      	orrs	r3, r0
300087da:	2000      	movs	r0, #0
300087dc:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
300087e0:	b005      	add	sp, #20
300087e2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087e6:	bf00      	nop
300087e8:	3000ff68 	.word	0x3000ff68
300087ec:	2001c01c 	.word	0x2001c01c
300087f0:	3000d958 	.word	0x3000d958
300087f4:	3000d964 	.word	0x3000d964
300087f8:	30009db5 	.word	0x30009db5

300087fc <Nand_Get_NandAddr>:
300087fc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008800:	4f10      	ldr	r7, [pc, #64]	; (30008844 <Nand_Get_NandAddr+0x48>)
30008802:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008806:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
3000880a:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
3000880e:	40e8      	lsrs	r0, r5
30008810:	fa26 f505 	lsr.w	r5, r6, r5
30008814:	09ad      	lsrs	r5, r5, #6
30008816:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
3000881a:	d90f      	bls.n	3000883c <Nand_Get_NandAddr+0x40>
3000881c:	0984      	lsrs	r4, r0, #6
3000881e:	f04f 0840 	mov.w	r8, #64	; 0x40
30008822:	4620      	mov	r0, r4
30008824:	3401      	adds	r4, #1
30008826:	f7ff ff93 	bl	30008750 <NAND_CHECK_IS_BAD_BLOCK>
3000882a:	b128      	cbz	r0, 30008838 <Nand_Get_NandAddr+0x3c>
3000882c:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008830:	3501      	adds	r5, #1
30008832:	fa08 f303 	lsl.w	r3, r8, r3
30008836:	441e      	add	r6, r3
30008838:	42a5      	cmp	r5, r4
3000883a:	d8f2      	bhi.n	30008822 <Nand_Get_NandAddr+0x26>
3000883c:	4630      	mov	r0, r6
3000883e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008842:	bf00      	nop
30008844:	2001c01c 	.word	0x2001c01c

30008848 <Nand_L2P_Table>:
30008848:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000884c:	b082      	sub	sp, #8
3000884e:	4604      	mov	r4, r0
30008850:	2002      	movs	r0, #2
30008852:	aa01      	add	r2, sp, #4
30008854:	4669      	mov	r1, sp
30008856:	f7ff ff55 	bl	30008704 <flash_get_layout_info>
3000885a:	9800      	ldr	r0, [sp, #0]
3000885c:	42a0      	cmp	r0, r4
3000885e:	d810      	bhi.n	30008882 <Nand_L2P_Table+0x3a>
30008860:	9b01      	ldr	r3, [sp, #4]
30008862:	42a3      	cmp	r3, r4
30008864:	d805      	bhi.n	30008872 <Nand_L2P_Table+0x2a>
30008866:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
3000886a:	4620      	mov	r0, r4
3000886c:	b002      	add	sp, #8
3000886e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008872:	4621      	mov	r1, r4
30008874:	f7ff ffc2 	bl	300087fc <Nand_Get_NandAddr>
30008878:	4604      	mov	r4, r0
3000887a:	4620      	mov	r0, r4
3000887c:	b002      	add	sp, #8
3000887e:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008882:	4f0e      	ldr	r7, [pc, #56]	; (300088bc <Nand_L2P_Table+0x74>)
30008884:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008888:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
3000888c:	fa24 f606 	lsr.w	r6, r4, r6
30008890:	09b6      	lsrs	r6, r6, #6
30008892:	d0ea      	beq.n	3000886a <Nand_L2P_Table+0x22>
30008894:	2500      	movs	r5, #0
30008896:	f04f 0840 	mov.w	r8, #64	; 0x40
3000889a:	4628      	mov	r0, r5
3000889c:	3501      	adds	r5, #1
3000889e:	f7ff ff57 	bl	30008750 <NAND_CHECK_IS_BAD_BLOCK>
300088a2:	b128      	cbz	r0, 300088b0 <Nand_L2P_Table+0x68>
300088a4:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
300088a8:	3601      	adds	r6, #1
300088aa:	fa08 f303 	lsl.w	r3, r8, r3
300088ae:	441c      	add	r4, r3
300088b0:	42b5      	cmp	r5, r6
300088b2:	d3f2      	bcc.n	3000889a <Nand_L2P_Table+0x52>
300088b4:	4620      	mov	r0, r4
300088b6:	b002      	add	sp, #8
300088b8:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300088bc:	2001c01c 	.word	0x2001c01c

300088c0 <NandImgCopy>:
300088c0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300088c4:	4617      	mov	r7, r2
300088c6:	4603      	mov	r3, r0
300088c8:	b089      	sub	sp, #36	; 0x24
300088ca:	f8df 9100 	ldr.w	r9, [pc, #256]	; 300089cc <NandImgCopy+0x10c>
300088ce:	f107 38ff 	add.w	r8, r7, #4294967295
300088d2:	4608      	mov	r0, r1
300088d4:	9307      	str	r3, [sp, #28]
300088d6:	f7ff ffb7 	bl	30008848 <Nand_L2P_Table>
300088da:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
300088de:	4480      	add	r8, r0
300088e0:	fa20 f406 	lsr.w	r4, r0, r6
300088e4:	fa28 f806 	lsr.w	r8, r8, r6
300088e8:	eba8 0804 	sub.w	r8, r8, r4
300088ec:	f1b8 3fff 	cmp.w	r8, #4294967295
300088f0:	d054      	beq.n	3000899c <NandImgCopy+0xdc>
300088f2:	2300      	movs	r3, #0
300088f4:	4605      	mov	r5, r0
300088f6:	f04f 0b40 	mov.w	fp, #64	; 0x40
300088fa:	469a      	mov	sl, r3
300088fc:	9306      	str	r3, [sp, #24]
300088fe:	463b      	mov	r3, r7
30008900:	4647      	mov	r7, r8
30008902:	4698      	mov	r8, r3
30008904:	e020      	b.n	30008948 <NandImgCopy+0x88>
30008906:	f1c2 0320 	rsb	r3, r2, #32
3000890a:	f04f 31ff 	mov.w	r1, #4294967295
3000890e:	40d9      	lsrs	r1, r3
30008910:	9b07      	ldr	r3, [sp, #28]
30008912:	4029      	ands	r1, r5
30008914:	4453      	add	r3, sl
30008916:	b377      	cbz	r7, 30008976 <NandImgCopy+0xb6>
30008918:	2601      	movs	r6, #1
3000891a:	4620      	mov	r0, r4
3000891c:	9105      	str	r1, [sp, #20]
3000891e:	fa06 f202 	lsl.w	r2, r6, r2
30008922:	4e27      	ldr	r6, [pc, #156]	; (300089c0 <NandImgCopy+0x100>)
30008924:	9204      	str	r2, [sp, #16]
30008926:	1a52      	subs	r2, r2, r1
30008928:	4492      	add	sl, r2
3000892a:	4415      	add	r5, r2
3000892c:	47b0      	blx	r6
3000892e:	f010 0f70 	tst.w	r0, #112	; 0x70
30008932:	9b04      	ldr	r3, [sp, #16]
30008934:	9905      	ldr	r1, [sp, #20]
30008936:	d134      	bne.n	300089a2 <NandImgCopy+0xe2>
30008938:	eba8 0803 	sub.w	r8, r8, r3
3000893c:	4488      	add	r8, r1
3000893e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008942:	3f01      	subs	r7, #1
30008944:	fa25 f406 	lsr.w	r4, r5, r6
30008948:	9b06      	ldr	r3, [sp, #24]
3000894a:	4632      	mov	r2, r6
3000894c:	09a6      	lsrs	r6, r4, #6
3000894e:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
30008952:	d8d8      	bhi.n	30008906 <NandImgCopy+0x46>
30008954:	e005      	b.n	30008962 <NandImgCopy+0xa2>
30008956:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
3000895a:	3440      	adds	r4, #64	; 0x40
3000895c:	fa0b f303 	lsl.w	r3, fp, r3
30008960:	441d      	add	r5, r3
30008962:	4630      	mov	r0, r6
30008964:	3601      	adds	r6, #1
30008966:	f7ff fef3 	bl	30008750 <NAND_CHECK_IS_BAD_BLOCK>
3000896a:	2800      	cmp	r0, #0
3000896c:	d1f3      	bne.n	30008956 <NandImgCopy+0x96>
3000896e:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
30008972:	9606      	str	r6, [sp, #24]
30008974:	e7c7      	b.n	30008906 <NandImgCopy+0x46>
30008976:	4642      	mov	r2, r8
30008978:	4620      	mov	r0, r4
3000897a:	4d11      	ldr	r5, [pc, #68]	; (300089c0 <NandImgCopy+0x100>)
3000897c:	47a8      	blx	r5
3000897e:	f010 0f70 	tst.w	r0, #112	; 0x70
30008982:	d00b      	beq.n	3000899c <NandImgCopy+0xdc>
30008984:	09a1      	lsrs	r1, r4, #6
30008986:	4b0f      	ldr	r3, [pc, #60]	; (300089c4 <NandImgCopy+0x104>)
30008988:	f004 043f 	and.w	r4, r4, #63	; 0x3f
3000898c:	4a0e      	ldr	r2, [pc, #56]	; (300089c8 <NandImgCopy+0x108>)
3000898e:	e9cd 1001 	strd	r1, r0, [sp, #4]
30008992:	9400      	str	r4, [sp, #0]
30008994:	4619      	mov	r1, r3
30008996:	2004      	movs	r0, #4
30008998:	f003 fbd2 	bl	3000c140 <rtk_log_write>
3000899c:	b009      	add	sp, #36	; 0x24
3000899e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089a2:	1ac9      	subs	r1, r1, r3
300089a4:	09a3      	lsrs	r3, r4, #6
300089a6:	f004 043f 	and.w	r4, r4, #63	; 0x3f
300089aa:	9002      	str	r0, [sp, #8]
300089ac:	9301      	str	r3, [sp, #4]
300089ae:	4488      	add	r8, r1
300089b0:	4b04      	ldr	r3, [pc, #16]	; (300089c4 <NandImgCopy+0x104>)
300089b2:	2004      	movs	r0, #4
300089b4:	4a04      	ldr	r2, [pc, #16]	; (300089c8 <NandImgCopy+0x108>)
300089b6:	4619      	mov	r1, r3
300089b8:	9400      	str	r4, [sp, #0]
300089ba:	f003 fbc1 	bl	3000c140 <rtk_log_write>
300089be:	e7be      	b.n	3000893e <NandImgCopy+0x7e>
300089c0:	30009db5 	.word	0x30009db5
300089c4:	3000d958 	.word	0x3000d958
300089c8:	3000d97c 	.word	0x3000d97c
300089cc:	2001c01c 	.word	0x2001c01c

300089d0 <OTP_Read8.part.0>:
300089d0:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300089d4:	4b30      	ldr	r3, [pc, #192]	; (30008a98 <OTP_Read8.part.0+0xc8>)
300089d6:	4680      	mov	r8, r0
300089d8:	e843 f300 	tt	r3, r3
300089dc:	f3c3 5380 	ubfx	r3, r3, #22, #1
300089e0:	4a2e      	ldr	r2, [pc, #184]	; (30008a9c <OTP_Read8.part.0+0xcc>)
300089e2:	4e2f      	ldr	r6, [pc, #188]	; (30008aa0 <OTP_Read8.part.0+0xd0>)
300089e4:	b082      	sub	sp, #8
300089e6:	2b00      	cmp	r3, #0
300089e8:	4689      	mov	r9, r1
300089ea:	bf0e      	itee	eq
300089ec:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300089f0:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300089f4:	4616      	movne	r6, r2
300089f6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300089f8:	05d8      	lsls	r0, r3, #23
300089fa:	d505      	bpl.n	30008a08 <OTP_Read8.part.0+0x38>
300089fc:	4d29      	ldr	r5, [pc, #164]	; (30008aa4 <OTP_Read8.part.0+0xd4>)
300089fe:	2064      	movs	r0, #100	; 0x64
30008a00:	47a8      	blx	r5
30008a02:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a04:	05d9      	lsls	r1, r3, #23
30008a06:	d4fa      	bmi.n	300089fe <OTP_Read8.part.0+0x2e>
30008a08:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a0a:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008a0e:	643b      	str	r3, [r7, #64]	; 0x40
30008a10:	4b25      	ldr	r3, [pc, #148]	; (30008aa8 <OTP_Read8.part.0+0xd8>)
30008a12:	681a      	ldr	r2, [r3, #0]
30008a14:	07d2      	lsls	r2, r2, #31
30008a16:	d403      	bmi.n	30008a20 <OTP_Read8.part.0+0x50>
30008a18:	681a      	ldr	r2, [r3, #0]
30008a1a:	f042 0201 	orr.w	r2, r2, #1
30008a1e:	601a      	str	r2, [r3, #0]
30008a20:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008a24:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008a28:	694a      	ldr	r2, [r1, #20]
30008a2a:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008a2e:	614a      	str	r2, [r1, #20]
30008a30:	6033      	str	r3, [r6, #0]
30008a32:	6833      	ldr	r3, [r6, #0]
30008a34:	2b00      	cmp	r3, #0
30008a36:	db10      	blt.n	30008a5a <OTP_Read8.part.0+0x8a>
30008a38:	2400      	movs	r4, #0
30008a3a:	4d1a      	ldr	r5, [pc, #104]	; (30008aa4 <OTP_Read8.part.0+0xd4>)
30008a3c:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008a40:	e001      	b.n	30008a46 <OTP_Read8.part.0+0x76>
30008a42:	4554      	cmp	r4, sl
30008a44:	d01a      	beq.n	30008a7c <OTP_Read8.part.0+0xac>
30008a46:	2005      	movs	r0, #5
30008a48:	3401      	adds	r4, #1
30008a4a:	47a8      	blx	r5
30008a4c:	6833      	ldr	r3, [r6, #0]
30008a4e:	2b00      	cmp	r3, #0
30008a50:	daf7      	bge.n	30008a42 <OTP_Read8.part.0+0x72>
30008a52:	f644 6320 	movw	r3, #20000	; 0x4e20
30008a56:	429c      	cmp	r4, r3
30008a58:	d010      	beq.n	30008a7c <OTP_Read8.part.0+0xac>
30008a5a:	6833      	ldr	r3, [r6, #0]
30008a5c:	2001      	movs	r0, #1
30008a5e:	f889 3000 	strb.w	r3, [r9]
30008a62:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008a66:	6953      	ldr	r3, [r2, #20]
30008a68:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008a6c:	6153      	str	r3, [r2, #20]
30008a6e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a70:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008a74:	643b      	str	r3, [r7, #64]	; 0x40
30008a76:	b002      	add	sp, #8
30008a78:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008a7c:	22ff      	movs	r2, #255	; 0xff
30008a7e:	4b0b      	ldr	r3, [pc, #44]	; (30008aac <OTP_Read8.part.0+0xdc>)
30008a80:	2002      	movs	r0, #2
30008a82:	f8cd 8000 	str.w	r8, [sp]
30008a86:	4619      	mov	r1, r3
30008a88:	f889 2000 	strb.w	r2, [r9]
30008a8c:	4a08      	ldr	r2, [pc, #32]	; (30008ab0 <OTP_Read8.part.0+0xe0>)
30008a8e:	f003 fb57 	bl	3000c140 <rtk_log_write>
30008a92:	2000      	movs	r0, #0
30008a94:	e7e5      	b.n	30008a62 <OTP_Read8.part.0+0x92>
30008a96:	bf00      	nop
30008a98:	0000e5f9 	.word	0x0000e5f9
30008a9c:	5200000c 	.word	0x5200000c
30008aa0:	42000008 	.word	0x42000008
30008aa4:	00009b2d 	.word	0x00009b2d
30008aa8:	42008000 	.word	0x42008000
30008aac:	3000d9b8 	.word	0x3000d9b8
30008ab0:	3000d9bc 	.word	0x3000d9bc

30008ab4 <OTP_ProgramMarginRead8.part.0>:
30008ab4:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008ab8:	4b31      	ldr	r3, [pc, #196]	; (30008b80 <OTP_ProgramMarginRead8.part.0+0xcc>)
30008aba:	4680      	mov	r8, r0
30008abc:	e843 f300 	tt	r3, r3
30008ac0:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008ac4:	4a2f      	ldr	r2, [pc, #188]	; (30008b84 <OTP_ProgramMarginRead8.part.0+0xd0>)
30008ac6:	4e30      	ldr	r6, [pc, #192]	; (30008b88 <OTP_ProgramMarginRead8.part.0+0xd4>)
30008ac8:	b082      	sub	sp, #8
30008aca:	2b00      	cmp	r3, #0
30008acc:	4689      	mov	r9, r1
30008ace:	bf0e      	itee	eq
30008ad0:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008ad4:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008ad8:	4616      	movne	r6, r2
30008ada:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008adc:	05d8      	lsls	r0, r3, #23
30008ade:	d505      	bpl.n	30008aec <OTP_ProgramMarginRead8.part.0+0x38>
30008ae0:	4d2a      	ldr	r5, [pc, #168]	; (30008b8c <OTP_ProgramMarginRead8.part.0+0xd8>)
30008ae2:	2064      	movs	r0, #100	; 0x64
30008ae4:	47a8      	blx	r5
30008ae6:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008ae8:	05d9      	lsls	r1, r3, #23
30008aea:	d4fa      	bmi.n	30008ae2 <OTP_ProgramMarginRead8.part.0+0x2e>
30008aec:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008aee:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008af2:	643b      	str	r3, [r7, #64]	; 0x40
30008af4:	4b26      	ldr	r3, [pc, #152]	; (30008b90 <OTP_ProgramMarginRead8.part.0+0xdc>)
30008af6:	681a      	ldr	r2, [r3, #0]
30008af8:	07d2      	lsls	r2, r2, #31
30008afa:	d403      	bmi.n	30008b04 <OTP_ProgramMarginRead8.part.0+0x50>
30008afc:	681a      	ldr	r2, [r3, #0]
30008afe:	f042 0201 	orr.w	r2, r2, #1
30008b02:	601a      	str	r2, [r3, #0]
30008b04:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008b08:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008b0c:	694a      	ldr	r2, [r1, #20]
30008b0e:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
30008b12:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008b16:	614a      	str	r2, [r1, #20]
30008b18:	6033      	str	r3, [r6, #0]
30008b1a:	6833      	ldr	r3, [r6, #0]
30008b1c:	2b00      	cmp	r3, #0
30008b1e:	db10      	blt.n	30008b42 <OTP_ProgramMarginRead8.part.0+0x8e>
30008b20:	2400      	movs	r4, #0
30008b22:	4d1a      	ldr	r5, [pc, #104]	; (30008b8c <OTP_ProgramMarginRead8.part.0+0xd8>)
30008b24:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008b28:	e001      	b.n	30008b2e <OTP_ProgramMarginRead8.part.0+0x7a>
30008b2a:	4554      	cmp	r4, sl
30008b2c:	d01a      	beq.n	30008b64 <OTP_ProgramMarginRead8.part.0+0xb0>
30008b2e:	2005      	movs	r0, #5
30008b30:	3401      	adds	r4, #1
30008b32:	47a8      	blx	r5
30008b34:	6833      	ldr	r3, [r6, #0]
30008b36:	2b00      	cmp	r3, #0
30008b38:	daf7      	bge.n	30008b2a <OTP_ProgramMarginRead8.part.0+0x76>
30008b3a:	f644 6320 	movw	r3, #20000	; 0x4e20
30008b3e:	429c      	cmp	r4, r3
30008b40:	d010      	beq.n	30008b64 <OTP_ProgramMarginRead8.part.0+0xb0>
30008b42:	6833      	ldr	r3, [r6, #0]
30008b44:	2001      	movs	r0, #1
30008b46:	f889 3000 	strb.w	r3, [r9]
30008b4a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008b4e:	6953      	ldr	r3, [r2, #20]
30008b50:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008b54:	6153      	str	r3, [r2, #20]
30008b56:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008b58:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008b5c:	643b      	str	r3, [r7, #64]	; 0x40
30008b5e:	b002      	add	sp, #8
30008b60:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008b64:	22ff      	movs	r2, #255	; 0xff
30008b66:	4b0b      	ldr	r3, [pc, #44]	; (30008b94 <OTP_ProgramMarginRead8.part.0+0xe0>)
30008b68:	2002      	movs	r0, #2
30008b6a:	f8cd 8000 	str.w	r8, [sp]
30008b6e:	4619      	mov	r1, r3
30008b70:	f889 2000 	strb.w	r2, [r9]
30008b74:	4a08      	ldr	r2, [pc, #32]	; (30008b98 <OTP_ProgramMarginRead8.part.0+0xe4>)
30008b76:	f003 fae3 	bl	3000c140 <rtk_log_write>
30008b7a:	2000      	movs	r0, #0
30008b7c:	e7e5      	b.n	30008b4a <OTP_ProgramMarginRead8.part.0+0x96>
30008b7e:	bf00      	nop
30008b80:	0000e5f9 	.word	0x0000e5f9
30008b84:	5200000c 	.word	0x5200000c
30008b88:	42000008 	.word	0x42000008
30008b8c:	00009b2d 	.word	0x00009b2d
30008b90:	42008000 	.word	0x42008000
30008b94:	3000d9b8 	.word	0x3000d9b8
30008b98:	3000d9bc 	.word	0x3000d9bc

30008b9c <OTP_Read8>:
30008b9c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008ba0:	d201      	bcs.n	30008ba6 <OTP_Read8+0xa>
30008ba2:	f7ff bf15 	b.w	300089d0 <OTP_Read8.part.0>
30008ba6:	22ff      	movs	r2, #255	; 0xff
30008ba8:	2000      	movs	r0, #0
30008baa:	700a      	strb	r2, [r1, #0]
30008bac:	4770      	bx	lr
30008bae:	bf00      	nop

30008bb0 <OTP_Write8>:
30008bb0:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008bb4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008bb8:	b087      	sub	sp, #28
30008bba:	d278      	bcs.n	30008cae <OTP_Write8+0xfe>
30008bbc:	4689      	mov	r9, r1
30008bbe:	f10d 0117 	add.w	r1, sp, #23
30008bc2:	4680      	mov	r8, r0
30008bc4:	f7ff ff76 	bl	30008ab4 <OTP_ProgramMarginRead8.part.0>
30008bc8:	2800      	cmp	r0, #0
30008bca:	d073      	beq.n	30008cb4 <OTP_Write8+0x104>
30008bcc:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008bd0:	2202      	movs	r2, #2
30008bd2:	46ca      	mov	sl, r9
30008bd4:	9303      	str	r3, [sp, #12]
30008bd6:	f89d 3017 	ldrb.w	r3, [sp, #23]
30008bda:	9202      	str	r2, [sp, #8]
30008bdc:	ea6a 0a03 	orn	sl, sl, r3
30008be0:	fa5f fa8a 	uxtb.w	sl, sl
30008be4:	f1ba 0fff 	cmp.w	sl, #255	; 0xff
30008be8:	d04f      	beq.n	30008c8a <OTP_Write8+0xda>
30008bea:	4b4b      	ldr	r3, [pc, #300]	; (30008d18 <OTP_Write8+0x168>)
30008bec:	e843 f300 	tt	r3, r3
30008bf0:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008bf4:	4a49      	ldr	r2, [pc, #292]	; (30008d1c <OTP_Write8+0x16c>)
30008bf6:	2b00      	cmp	r3, #0
30008bf8:	4b49      	ldr	r3, [pc, #292]	; (30008d20 <OTP_Write8+0x170>)
30008bfa:	bf0b      	itete	eq
30008bfc:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008c00:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008c04:	461d      	moveq	r5, r3
30008c06:	4615      	movne	r5, r2
30008c08:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c0a:	05d8      	lsls	r0, r3, #23
30008c0c:	d505      	bpl.n	30008c1a <OTP_Write8+0x6a>
30008c0e:	4c45      	ldr	r4, [pc, #276]	; (30008d24 <OTP_Write8+0x174>)
30008c10:	2064      	movs	r0, #100	; 0x64
30008c12:	47a0      	blx	r4
30008c14:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c16:	05d9      	lsls	r1, r3, #23
30008c18:	d4fa      	bmi.n	30008c10 <OTP_Write8+0x60>
30008c1a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c1c:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008c20:	643b      	str	r3, [r7, #64]	; 0x40
30008c22:	4b41      	ldr	r3, [pc, #260]	; (30008d28 <OTP_Write8+0x178>)
30008c24:	681a      	ldr	r2, [r3, #0]
30008c26:	07d2      	lsls	r2, r2, #31
30008c28:	d403      	bmi.n	30008c32 <OTP_Write8+0x82>
30008c2a:	681a      	ldr	r2, [r3, #0]
30008c2c:	f042 0201 	orr.w	r2, r2, #1
30008c30:	601a      	str	r2, [r3, #0]
30008c32:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008c36:	9b03      	ldr	r3, [sp, #12]
30008c38:	694a      	ldr	r2, [r1, #20]
30008c3a:	ea4a 0303 	orr.w	r3, sl, r3
30008c3e:	f042 42d2 	orr.w	r2, r2, #1761607680	; 0x69000000
30008c42:	f043 4300 	orr.w	r3, r3, #2147483648	; 0x80000000
30008c46:	614a      	str	r2, [r1, #20]
30008c48:	602b      	str	r3, [r5, #0]
30008c4a:	682b      	ldr	r3, [r5, #0]
30008c4c:	2b00      	cmp	r3, #0
30008c4e:	da12      	bge.n	30008c76 <OTP_Write8+0xc6>
30008c50:	f04f 0b00 	mov.w	fp, #0
30008c54:	4c33      	ldr	r4, [pc, #204]	; (30008d24 <OTP_Write8+0x174>)
30008c56:	f644 6620 	movw	r6, #20000	; 0x4e20
30008c5a:	e001      	b.n	30008c60 <OTP_Write8+0xb0>
30008c5c:	45b3      	cmp	fp, r6
30008c5e:	d036      	beq.n	30008cce <OTP_Write8+0x11e>
30008c60:	2005      	movs	r0, #5
30008c62:	f10b 0b01 	add.w	fp, fp, #1
30008c66:	47a0      	blx	r4
30008c68:	682b      	ldr	r3, [r5, #0]
30008c6a:	2b00      	cmp	r3, #0
30008c6c:	dbf6      	blt.n	30008c5c <OTP_Write8+0xac>
30008c6e:	f644 6220 	movw	r2, #20000	; 0x4e20
30008c72:	4593      	cmp	fp, r2
30008c74:	d02b      	beq.n	30008cce <OTP_Write8+0x11e>
30008c76:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008c7a:	6953      	ldr	r3, [r2, #20]
30008c7c:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008c80:	6153      	str	r3, [r2, #20]
30008c82:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008c84:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008c88:	643b      	str	r3, [r7, #64]	; 0x40
30008c8a:	f10d 0117 	add.w	r1, sp, #23
30008c8e:	4640      	mov	r0, r8
30008c90:	f7ff ff10 	bl	30008ab4 <OTP_ProgramMarginRead8.part.0>
30008c94:	2800      	cmp	r0, #0
30008c96:	d034      	beq.n	30008d02 <OTP_Write8+0x152>
30008c98:	f89d 3017 	ldrb.w	r3, [sp, #23]
30008c9c:	4599      	cmp	r9, r3
30008c9e:	d039      	beq.n	30008d14 <OTP_Write8+0x164>
30008ca0:	9a02      	ldr	r2, [sp, #8]
30008ca2:	2a01      	cmp	r2, #1
30008ca4:	d110      	bne.n	30008cc8 <OTP_Write8+0x118>
30008ca6:	2000      	movs	r0, #0
30008ca8:	b007      	add	sp, #28
30008caa:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008cae:	23ff      	movs	r3, #255	; 0xff
30008cb0:	f88d 3017 	strb.w	r3, [sp, #23]
30008cb4:	4b1d      	ldr	r3, [pc, #116]	; (30008d2c <OTP_Write8+0x17c>)
30008cb6:	2002      	movs	r0, #2
30008cb8:	4a1d      	ldr	r2, [pc, #116]	; (30008d30 <OTP_Write8+0x180>)
30008cba:	4619      	mov	r1, r3
30008cbc:	f003 fa40 	bl	3000c140 <rtk_log_write>
30008cc0:	2000      	movs	r0, #0
30008cc2:	b007      	add	sp, #28
30008cc4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008cc8:	2201      	movs	r2, #1
30008cca:	9202      	str	r2, [sp, #8]
30008ccc:	e786      	b.n	30008bdc <OTP_Write8+0x2c>
30008cce:	4b17      	ldr	r3, [pc, #92]	; (30008d2c <OTP_Write8+0x17c>)
30008cd0:	2003      	movs	r0, #3
30008cd2:	4a18      	ldr	r2, [pc, #96]	; (30008d34 <OTP_Write8+0x184>)
30008cd4:	4619      	mov	r1, r3
30008cd6:	f8cd 8000 	str.w	r8, [sp]
30008cda:	f003 fa31 	bl	3000c140 <rtk_log_write>
30008cde:	f04f 4084 	mov.w	r0, #1107296256	; 0x42000000
30008ce2:	4b12      	ldr	r3, [pc, #72]	; (30008d2c <OTP_Write8+0x17c>)
30008ce4:	6941      	ldr	r1, [r0, #20]
30008ce6:	4a14      	ldr	r2, [pc, #80]	; (30008d38 <OTP_Write8+0x188>)
30008ce8:	f021 417f 	bic.w	r1, r1, #4278190080	; 0xff000000
30008cec:	6141      	str	r1, [r0, #20]
30008cee:	4619      	mov	r1, r3
30008cf0:	6c38      	ldr	r0, [r7, #64]	; 0x40
30008cf2:	f420 7080 	bic.w	r0, r0, #256	; 0x100
30008cf6:	6438      	str	r0, [r7, #64]	; 0x40
30008cf8:	2002      	movs	r0, #2
30008cfa:	f003 fa21 	bl	3000c140 <rtk_log_write>
30008cfe:	2000      	movs	r0, #0
30008d00:	e7df      	b.n	30008cc2 <OTP_Write8+0x112>
30008d02:	4b0a      	ldr	r3, [pc, #40]	; (30008d2c <OTP_Write8+0x17c>)
30008d04:	4604      	mov	r4, r0
30008d06:	4a0d      	ldr	r2, [pc, #52]	; (30008d3c <OTP_Write8+0x18c>)
30008d08:	2002      	movs	r0, #2
30008d0a:	4619      	mov	r1, r3
30008d0c:	f003 fa18 	bl	3000c140 <rtk_log_write>
30008d10:	4620      	mov	r0, r4
30008d12:	e7d6      	b.n	30008cc2 <OTP_Write8+0x112>
30008d14:	2001      	movs	r0, #1
30008d16:	e7d4      	b.n	30008cc2 <OTP_Write8+0x112>
30008d18:	0000e5f9 	.word	0x0000e5f9
30008d1c:	5200000c 	.word	0x5200000c
30008d20:	42000008 	.word	0x42000008
30008d24:	00009b2d 	.word	0x00009b2d
30008d28:	42008000 	.word	0x42008000
30008d2c:	3000d9b8 	.word	0x3000d9b8
30008d30:	3000d9d8 	.word	0x3000d9d8
30008d34:	3000d9f0 	.word	0x3000d9f0
30008d38:	3000da0c 	.word	0x3000da0c
30008d3c:	3000da28 	.word	0x3000da28

30008d40 <PLL_NP_ClkSet>:
30008d40:	b570      	push	{r4, r5, r6, lr}
30008d42:	4604      	mov	r4, r0
30008d44:	4b1b      	ldr	r3, [pc, #108]	; (30008db4 <PLL_NP_ClkSet+0x74>)
30008d46:	4798      	blx	r3
30008d48:	4d1b      	ldr	r5, [pc, #108]	; (30008db8 <PLL_NP_ClkSet+0x78>)
30008d4a:	08c2      	lsrs	r2, r0, #3
30008d4c:	fbb4 f3f0 	udiv	r3, r4, r0
30008d50:	fb00 4c13 	mls	ip, r0, r3, r4
30008d54:	1e9c      	subs	r4, r3, #2
30008d56:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008d58:	0164      	lsls	r4, r4, #5
30008d5a:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30008d5e:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30008d62:	431c      	orrs	r4, r3
30008d64:	fbbc f3f2 	udiv	r3, ip, r2
30008d68:	646c      	str	r4, [r5, #68]	; 0x44
30008d6a:	041c      	lsls	r4, r3, #16
30008d6c:	fb02 c013 	mls	r0, r2, r3, ip
30008d70:	2300      	movs	r3, #0
30008d72:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008d74:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008d78:	4619      	mov	r1, r3
30008d7a:	fa1f fc86 	uxth.w	ip, r6
30008d7e:	4e0f      	ldr	r6, [pc, #60]	; (30008dbc <PLL_NP_ClkSet+0x7c>)
30008d80:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008d84:	ea44 040c 	orr.w	r4, r4, ip
30008d88:	f003 fa64 	bl	3000c254 <__aeabi_uldivmod>
30008d8c:	4603      	mov	r3, r0
30008d8e:	2001      	movs	r0, #1
30008d90:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008d94:	64ec      	str	r4, [r5, #76]	; 0x4c
30008d96:	47b0      	blx	r6
30008d98:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008d9a:	203c      	movs	r0, #60	; 0x3c
30008d9c:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008da0:	646b      	str	r3, [r5, #68]	; 0x44
30008da2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008da4:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008da8:	646b      	str	r3, [r5, #68]	; 0x44
30008daa:	4633      	mov	r3, r6
30008dac:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008db0:	4718      	bx	r3
30008db2:	bf00      	nop
30008db4:	000099f5 	.word	0x000099f5
30008db8:	42008800 	.word	0x42008800
30008dbc:	00009b2d 	.word	0x00009b2d

30008dc0 <PLL_AP_ClkSet>:
30008dc0:	b510      	push	{r4, lr}
30008dc2:	4604      	mov	r4, r0
30008dc4:	4b0b      	ldr	r3, [pc, #44]	; (30008df4 <PLL_AP_ClkSet+0x34>)
30008dc6:	4798      	blx	r3
30008dc8:	fbb4 f0f0 	udiv	r0, r4, r0
30008dcc:	f1a0 0319 	sub.w	r3, r0, #25
30008dd0:	1e84      	subs	r4, r0, #2
30008dd2:	2b0f      	cmp	r3, #15
30008dd4:	d807      	bhi.n	30008de6 <PLL_AP_ClkSet+0x26>
30008dd6:	4b08      	ldr	r3, [pc, #32]	; (30008df8 <PLL_AP_ClkSet+0x38>)
30008dd8:	b2e0      	uxtb	r0, r4
30008dda:	695c      	ldr	r4, [r3, #20]
30008ddc:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008de0:	4320      	orrs	r0, r4
30008de2:	6158      	str	r0, [r3, #20]
30008de4:	bd10      	pop	{r4, pc}
30008de6:	f240 1181 	movw	r1, #385	; 0x181
30008dea:	4804      	ldr	r0, [pc, #16]	; (30008dfc <PLL_AP_ClkSet+0x3c>)
30008dec:	f003 fc8c 	bl	3000c708 <__io_assert_failed_veneer>
30008df0:	e7f1      	b.n	30008dd6 <PLL_AP_ClkSet+0x16>
30008df2:	bf00      	nop
30008df4:	000099f5 	.word	0x000099f5
30008df8:	41000300 	.word	0x41000300
30008dfc:	3000da44 	.word	0x3000da44

30008e00 <PLL_AP>:
30008e00:	2801      	cmp	r0, #1
30008e02:	b538      	push	{r3, r4, r5, lr}
30008e04:	d009      	beq.n	30008e1a <PLL_AP+0x1a>
30008e06:	4b13      	ldr	r3, [pc, #76]	; (30008e54 <PLL_AP+0x54>)
30008e08:	681a      	ldr	r2, [r3, #0]
30008e0a:	f022 0209 	bic.w	r2, r2, #9
30008e0e:	601a      	str	r2, [r3, #0]
30008e10:	681a      	ldr	r2, [r3, #0]
30008e12:	f022 0206 	bic.w	r2, r2, #6
30008e16:	601a      	str	r2, [r3, #0]
30008e18:	bd38      	pop	{r3, r4, r5, pc}
30008e1a:	4b0f      	ldr	r3, [pc, #60]	; (30008e58 <PLL_AP+0x58>)
30008e1c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008e1e:	f012 0f05 	tst.w	r2, #5
30008e22:	d00f      	beq.n	30008e44 <PLL_AP+0x44>
30008e24:	4d0d      	ldr	r5, [pc, #52]	; (30008e5c <PLL_AP+0x5c>)
30008e26:	4c0b      	ldr	r4, [pc, #44]	; (30008e54 <PLL_AP+0x54>)
30008e28:	2001      	movs	r0, #1
30008e2a:	6823      	ldr	r3, [r4, #0]
30008e2c:	f043 0306 	orr.w	r3, r3, #6
30008e30:	6023      	str	r3, [r4, #0]
30008e32:	47a8      	blx	r5
30008e34:	6823      	ldr	r3, [r4, #0]
30008e36:	f043 0309 	orr.w	r3, r3, #9
30008e3a:	6023      	str	r3, [r4, #0]
30008e3c:	6863      	ldr	r3, [r4, #4]
30008e3e:	2b00      	cmp	r3, #0
30008e40:	dafc      	bge.n	30008e3c <PLL_AP+0x3c>
30008e42:	bd38      	pop	{r3, r4, r5, pc}
30008e44:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008e46:	20a0      	movs	r0, #160	; 0xa0
30008e48:	4d04      	ldr	r5, [pc, #16]	; (30008e5c <PLL_AP+0x5c>)
30008e4a:	f042 0207 	orr.w	r2, r2, #7
30008e4e:	679a      	str	r2, [r3, #120]	; 0x78
30008e50:	47a8      	blx	r5
30008e52:	e7e8      	b.n	30008e26 <PLL_AP+0x26>
30008e54:	41000300 	.word	0x41000300
30008e58:	42008800 	.word	0x42008800
30008e5c:	00009b2d 	.word	0x00009b2d

30008e60 <PSRAM_INFO_Update>:
30008e60:	4b46      	ldr	r3, [pc, #280]	; (30008f7c <PSRAM_INFO_Update+0x11c>)
30008e62:	4947      	ldr	r1, [pc, #284]	; (30008f80 <PSRAM_INFO_Update+0x120>)
30008e64:	4a47      	ldr	r2, [pc, #284]	; (30008f84 <PSRAM_INFO_Update+0x124>)
30008e66:	b530      	push	{r4, r5, lr}
30008e68:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008e6c:	b083      	sub	sp, #12
30008e6e:	680d      	ldr	r5, [r1, #0]
30008e70:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008e74:	4c44      	ldr	r4, [pc, #272]	; (30008f88 <PSRAM_INFO_Update+0x128>)
30008e76:	fb02 f505 	mul.w	r5, r2, r5
30008e7a:	3301      	adds	r3, #1
30008e7c:	fbb5 f5f3 	udiv	r5, r5, r3
30008e80:	086a      	lsrs	r2, r5, #1
30008e82:	4b42      	ldr	r3, [pc, #264]	; (30008f8c <PSRAM_INFO_Update+0x12c>)
30008e84:	60e2      	str	r2, [r4, #12]
30008e86:	4798      	blx	r3
30008e88:	4b41      	ldr	r3, [pc, #260]	; (30008f90 <PSRAM_INFO_Update+0x130>)
30008e8a:	6020      	str	r0, [r4, #0]
30008e8c:	4798      	blx	r3
30008e8e:	4b41      	ldr	r3, [pc, #260]	; (30008f94 <PSRAM_INFO_Update+0x134>)
30008e90:	220a      	movs	r2, #10
30008e92:	60a0      	str	r0, [r4, #8]
30008e94:	6122      	str	r2, [r4, #16]
30008e96:	2220      	movs	r2, #32
30008e98:	fbb3 f3f5 	udiv	r3, r3, r5
30008e9c:	6163      	str	r3, [r4, #20]
30008e9e:	fbb2 f3f3 	udiv	r3, r2, r3
30008ea2:	6822      	ldr	r2, [r4, #0]
30008ea4:	6223      	str	r3, [r4, #32]
30008ea6:	2a01      	cmp	r2, #1
30008ea8:	68e3      	ldr	r3, [r4, #12]
30008eaa:	4a3b      	ldr	r2, [pc, #236]	; (30008f98 <PSRAM_INFO_Update+0x138>)
30008eac:	d01d      	beq.n	30008eea <PSRAM_INFO_Update+0x8a>
30008eae:	4293      	cmp	r3, r2
30008eb0:	d810      	bhi.n	30008ed4 <PSRAM_INFO_Update+0x74>
30008eb2:	2304      	movs	r3, #4
30008eb4:	2108      	movs	r1, #8
30008eb6:	461a      	mov	r2, r3
30008eb8:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008ebc:	4b37      	ldr	r3, [pc, #220]	; (30008f9c <PSRAM_INFO_Update+0x13c>)
30008ebe:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
30008ec2:	4b37      	ldr	r3, [pc, #220]	; (30008fa0 <PSRAM_INFO_Update+0x140>)
30008ec4:	2004      	movs	r0, #4
30008ec6:	4a37      	ldr	r2, [pc, #220]	; (30008fa4 <PSRAM_INFO_Update+0x144>)
30008ec8:	4619      	mov	r1, r3
30008eca:	9500      	str	r5, [sp, #0]
30008ecc:	f003 f938 	bl	3000c140 <rtk_log_write>
30008ed0:	b003      	add	sp, #12
30008ed2:	bd30      	pop	{r4, r5, pc}
30008ed4:	4a34      	ldr	r2, [pc, #208]	; (30008fa8 <PSRAM_INFO_Update+0x148>)
30008ed6:	4293      	cmp	r3, r2
30008ed8:	d916      	bls.n	30008f08 <PSRAM_INFO_Update+0xa8>
30008eda:	4a34      	ldr	r2, [pc, #208]	; (30008fac <PSRAM_INFO_Update+0x14c>)
30008edc:	4293      	cmp	r3, r2
30008ede:	d824      	bhi.n	30008f2a <PSRAM_INFO_Update+0xca>
30008ee0:	2306      	movs	r3, #6
30008ee2:	461a      	mov	r2, r3
30008ee4:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008ee8:	e7e8      	b.n	30008ebc <PSRAM_INFO_Update+0x5c>
30008eea:	4293      	cmp	r3, r2
30008eec:	d907      	bls.n	30008efe <PSRAM_INFO_Update+0x9e>
30008eee:	4a2e      	ldr	r2, [pc, #184]	; (30008fa8 <PSRAM_INFO_Update+0x148>)
30008ef0:	4293      	cmp	r3, r2
30008ef2:	d80f      	bhi.n	30008f14 <PSRAM_INFO_Update+0xb4>
30008ef4:	2205      	movs	r2, #5
30008ef6:	230f      	movs	r3, #15
30008ef8:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008efc:	e7e1      	b.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008efe:	2204      	movs	r2, #4
30008f00:	230f      	movs	r3, #15
30008f02:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f06:	e7dc      	b.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008f08:	2305      	movs	r3, #5
30008f0a:	2108      	movs	r1, #8
30008f0c:	461a      	mov	r2, r3
30008f0e:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f12:	e7d3      	b.n	30008ebc <PSRAM_INFO_Update+0x5c>
30008f14:	4a25      	ldr	r2, [pc, #148]	; (30008fac <PSRAM_INFO_Update+0x14c>)
30008f16:	4293      	cmp	r3, r2
30008f18:	d917      	bls.n	30008f4a <PSRAM_INFO_Update+0xea>
30008f1a:	4a25      	ldr	r2, [pc, #148]	; (30008fb0 <PSRAM_INFO_Update+0x150>)
30008f1c:	4293      	cmp	r3, r2
30008f1e:	d81f      	bhi.n	30008f60 <PSRAM_INFO_Update+0x100>
30008f20:	2207      	movs	r2, #7
30008f22:	2314      	movs	r3, #20
30008f24:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f28:	e7cb      	b.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008f2a:	4a21      	ldr	r2, [pc, #132]	; (30008fb0 <PSRAM_INFO_Update+0x150>)
30008f2c:	4293      	cmp	r3, r2
30008f2e:	d911      	bls.n	30008f54 <PSRAM_INFO_Update+0xf4>
30008f30:	4a20      	ldr	r2, [pc, #128]	; (30008fb4 <PSRAM_INFO_Update+0x154>)
30008f32:	4293      	cmp	r3, r2
30008f34:	bf8c      	ite	hi
30008f36:	2300      	movhi	r3, #0
30008f38:	2301      	movls	r3, #1
30008f3a:	2804      	cmp	r0, #4
30008f3c:	d100      	bne.n	30008f40 <PSRAM_INFO_Update+0xe0>
30008f3e:	b94b      	cbnz	r3, 30008f54 <PSRAM_INFO_Update+0xf4>
30008f40:	2803      	cmp	r0, #3
30008f42:	d100      	bne.n	30008f46 <PSRAM_INFO_Update+0xe6>
30008f44:	b9a3      	cbnz	r3, 30008f70 <PSRAM_INFO_Update+0x110>
30008f46:	7e22      	ldrb	r2, [r4, #24]
30008f48:	e7b8      	b.n	30008ebc <PSRAM_INFO_Update+0x5c>
30008f4a:	2206      	movs	r2, #6
30008f4c:	2312      	movs	r3, #18
30008f4e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f52:	e7b6      	b.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008f54:	2307      	movs	r3, #7
30008f56:	2106      	movs	r1, #6
30008f58:	461a      	mov	r2, r3
30008f5a:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f5e:	e7ad      	b.n	30008ebc <PSRAM_INFO_Update+0x5c>
30008f60:	4a14      	ldr	r2, [pc, #80]	; (30008fb4 <PSRAM_INFO_Update+0x154>)
30008f62:	4293      	cmp	r3, r2
30008f64:	d8ad      	bhi.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008f66:	2208      	movs	r2, #8
30008f68:	231c      	movs	r3, #28
30008f6a:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008f6e:	e7a8      	b.n	30008ec2 <PSRAM_INFO_Update+0x62>
30008f70:	230a      	movs	r3, #10
30008f72:	2106      	movs	r1, #6
30008f74:	461a      	mov	r2, r3
30008f76:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008f7a:	e79f      	b.n	30008ebc <PSRAM_INFO_Update+0x5c>
30008f7c:	42008000 	.word	0x42008000
30008f80:	3000f238 	.word	0x3000f238
30008f84:	000f4240 	.word	0x000f4240
30008f88:	3000ff70 	.word	0x3000ff70
30008f8c:	30007eb5 	.word	0x30007eb5
30008f90:	30007e7d 	.word	0x30007e7d
30008f94:	3b9aca00 	.word	0x3b9aca00
30008f98:	0632ea00 	.word	0x0632ea00
30008f9c:	23020000 	.word	0x23020000
30008fa0:	3000da54 	.word	0x3000da54
30008fa4:	3000da5c 	.word	0x3000da5c
30008fa8:	07ed6b40 	.word	0x07ed6b40
30008fac:	09e4f580 	.word	0x09e4f580
30008fb0:	0bebc200 	.word	0x0bebc200
30008fb4:	0ee6b280 	.word	0x0ee6b280

30008fb8 <PSRAM_PHY_StructInit>:
30008fb8:	2203      	movs	r2, #3
30008fba:	2301      	movs	r3, #1
30008fbc:	210a      	movs	r1, #10
30008fbe:	b410      	push	{r4}
30008fc0:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008fc4:	2402      	movs	r4, #2
30008fc6:	2207      	movs	r2, #7
30008fc8:	e9c0 3400 	strd	r3, r4, [r0]
30008fcc:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008fd0:	f85d 4b04 	ldr.w	r4, [sp], #4
30008fd4:	6182      	str	r2, [r0, #24]
30008fd6:	4770      	bx	lr

30008fd8 <PSRAM_PHY_Init>:
30008fd8:	b538      	push	{r3, r4, r5, lr}
30008fda:	4604      	mov	r4, r0
30008fdc:	6903      	ldr	r3, [r0, #16]
30008fde:	4d14      	ldr	r5, [pc, #80]	; (30009030 <PSRAM_PHY_Init+0x58>)
30008fe0:	f003 031f 	and.w	r3, r3, #31
30008fe4:	4913      	ldr	r1, [pc, #76]	; (30009034 <PSRAM_PHY_Init+0x5c>)
30008fe6:	e9d0 0200 	ldrd	r0, r2, [r0]
30008fea:	0612      	lsls	r2, r2, #24
30008fec:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008ff0:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30008ff4:	4313      	orrs	r3, r2
30008ff6:	68a2      	ldr	r2, [r4, #8]
30008ff8:	0412      	lsls	r2, r2, #16
30008ffa:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008ffe:	4313      	orrs	r3, r2
30009000:	68e2      	ldr	r2, [r4, #12]
30009002:	0212      	lsls	r2, r2, #8
30009004:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009008:	4313      	orrs	r3, r2
3000900a:	606b      	str	r3, [r5, #4]
3000900c:	4788      	blx	r1
3000900e:	2801      	cmp	r0, #1
30009010:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30009014:	bf08      	it	eq
30009016:	4a08      	ldreq	r2, [pc, #32]	; (30009038 <PSRAM_PHY_Init+0x60>)
30009018:	ea4f 1303 	mov.w	r3, r3, lsl #4
3000901c:	f001 010f 	and.w	r1, r1, #15
30009020:	bf18      	it	ne
30009022:	4a06      	ldrne	r2, [pc, #24]	; (3000903c <PSRAM_PHY_Init+0x64>)
30009024:	b2db      	uxtb	r3, r3
30009026:	430b      	orrs	r3, r1
30009028:	431a      	orrs	r2, r3
3000902a:	61aa      	str	r2, [r5, #24]
3000902c:	bd38      	pop	{r3, r4, r5, pc}
3000902e:	bf00      	nop
30009030:	41002000 	.word	0x41002000
30009034:	30007eb5 	.word	0x30007eb5
30009038:	80000e00 	.word	0x80000e00
3000903c:	80000f00 	.word	0x80000f00

30009040 <PSRAM_CTRL_Init>:
30009040:	b470      	push	{r4, r5, r6}
30009042:	4b4c      	ldr	r3, [pc, #304]	; (30009174 <PSRAM_CTRL_Init+0x134>)
30009044:	2400      	movs	r4, #0
30009046:	4a4c      	ldr	r2, [pc, #304]	; (30009178 <PSRAM_CTRL_Init+0x138>)
30009048:	f24d 0101 	movw	r1, #53249	; 0xd001
3000904c:	609c      	str	r4, [r3, #8]
3000904e:	2001      	movs	r0, #1
30009050:	601a      	str	r2, [r3, #0]
30009052:	4a4a      	ldr	r2, [pc, #296]	; (3000917c <PSRAM_CTRL_Init+0x13c>)
30009054:	6158      	str	r0, [r3, #20]
30009056:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
3000905a:	6811      	ldr	r1, [r2, #0]
3000905c:	2902      	cmp	r1, #2
3000905e:	d04a      	beq.n	300090f6 <PSRAM_CTRL_Init+0xb6>
30009060:	6954      	ldr	r4, [r2, #20]
30009062:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30009066:	69d0      	ldr	r0, [r2, #28]
30009068:	6915      	ldr	r5, [r2, #16]
3000906a:	fbb0 f0f4 	udiv	r0, r0, r4
3000906e:	fbb1 f1f4 	udiv	r1, r1, r4
30009072:	6994      	ldr	r4, [r2, #24]
30009074:	f005 020f 	and.w	r2, r5, #15
30009078:	f241 5551 	movw	r5, #5457	; 0x1551
3000907c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30009080:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30009084:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30009088:	094a      	lsrs	r2, r1, #5
3000908a:	0181      	lsls	r1, r0, #6
3000908c:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30009090:	4d3b      	ldr	r5, [pc, #236]	; (30009180 <PSRAM_CTRL_Init+0x140>)
30009092:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30009096:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
3000909a:	00a2      	lsls	r2, r4, #2
3000909c:	4301      	orrs	r1, r0
3000909e:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
300090a2:	3a01      	subs	r2, #1
300090a4:	430d      	orrs	r5, r1
300090a6:	f242 0120 	movw	r1, #8224	; 0x2020
300090aa:	f3c2 020b 	ubfx	r2, r2, #0, #12
300090ae:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
300090b2:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
300090b6:	f44f 2180 	mov.w	r1, #262144	; 0x40000
300090ba:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
300090be:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
300090c2:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
300090c6:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
300090ca:	4420      	add	r0, r4
300090cc:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
300090d0:	f640 71fe 	movw	r1, #4094	; 0xffe
300090d4:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
300090d8:	ea01 0140 	and.w	r1, r1, r0, lsl #1
300090dc:	432a      	orrs	r2, r5
300090de:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
300090e2:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
300090e6:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
300090ea:	4a22      	ldr	r2, [pc, #136]	; (30009174 <PSRAM_CTRL_Init+0x134>)
300090ec:	6a93      	ldr	r3, [r2, #40]	; 0x28
300090ee:	061b      	lsls	r3, r3, #24
300090f0:	d5fc      	bpl.n	300090ec <PSRAM_CTRL_Init+0xac>
300090f2:	bc70      	pop	{r4, r5, r6}
300090f4:	4770      	bx	lr
300090f6:	6955      	ldr	r5, [r2, #20]
300090f8:	207d      	movs	r0, #125	; 0x7d
300090fa:	69d1      	ldr	r1, [r2, #28]
300090fc:	26a0      	movs	r6, #160	; 0xa0
300090fe:	6914      	ldr	r4, [r2, #16]
30009100:	6992      	ldr	r2, [r2, #24]
30009102:	f004 040f 	and.w	r4, r4, #15
30009106:	0092      	lsls	r2, r2, #2
30009108:	fbb1 f1f5 	udiv	r1, r1, r5
3000910c:	fbb0 f5f5 	udiv	r5, r0, r5
30009110:	f245 5051 	movw	r0, #21841	; 0x5551
30009114:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30009118:	f001 003f 	and.w	r0, r1, #63	; 0x3f
3000911c:	0189      	lsls	r1, r1, #6
3000911e:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30009122:	f641 4570 	movw	r5, #7280	; 0x1c70
30009126:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
3000912a:	4325      	orrs	r5, r4
3000912c:	2420      	movs	r4, #32
3000912e:	4308      	orrs	r0, r1
30009130:	1e51      	subs	r1, r2, #1
30009132:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30009136:	3a02      	subs	r2, #2
30009138:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
3000913c:	f3c1 010b 	ubfx	r1, r1, #0, #12
30009140:	f3c2 020b 	ubfx	r2, r2, #0, #12
30009144:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30009148:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
3000914c:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30009150:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30009154:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30009158:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
3000915c:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30009160:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30009164:	4301      	orrs	r1, r0
30009166:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
3000916a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
3000916e:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30009172:	e7ba      	b.n	300090ea <PSRAM_CTRL_Init+0xaa>
30009174:	41012000 	.word	0x41012000
30009178:	00bfe000 	.word	0x00bfe000
3000917c:	3000ff70 	.word	0x3000ff70
30009180:	00107000 	.word	0x00107000

30009184 <PSRAM_REG_Write>:
30009184:	b430      	push	{r4, r5}
30009186:	4c20      	ldr	r4, [pc, #128]	; (30009208 <PSRAM_REG_Write+0x84>)
30009188:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000918a:	f013 0301 	ands.w	r3, r3, #1
3000918e:	d1fb      	bne.n	30009188 <PSRAM_REG_Write+0x4>
30009190:	6825      	ldr	r5, [r4, #0]
30009192:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009196:	6025      	str	r5, [r4, #0]
30009198:	60a3      	str	r3, [r4, #8]
3000919a:	6825      	ldr	r5, [r4, #0]
3000919c:	f425 7540 	bic.w	r5, r5, #768	; 0x300
300091a0:	6025      	str	r5, [r4, #0]
300091a2:	6063      	str	r3, [r4, #4]
300091a4:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
300091a8:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
300091ac:	23c0      	movs	r3, #192	; 0xc0
300091ae:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091b2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091b6:	0e03      	lsrs	r3, r0, #24
300091b8:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091bc:	f3c0 4307 	ubfx	r3, r0, #16, #8
300091c0:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091c4:	f3c0 2307 	ubfx	r3, r0, #8, #8
300091c8:	b2c0      	uxtb	r0, r0
300091ca:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300091ce:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300091d2:	b141      	cbz	r1, 300091e6 <PSRAM_REG_Write+0x62>
300091d4:	3a01      	subs	r2, #1
300091d6:	480c      	ldr	r0, [pc, #48]	; (30009208 <PSRAM_REG_Write+0x84>)
300091d8:	4411      	add	r1, r2
300091da:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300091de:	428a      	cmp	r2, r1
300091e0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300091e4:	d1f9      	bne.n	300091da <PSRAM_REG_Write+0x56>
300091e6:	4a08      	ldr	r2, [pc, #32]	; (30009208 <PSRAM_REG_Write+0x84>)
300091e8:	2301      	movs	r3, #1
300091ea:	6093      	str	r3, [r2, #8]
300091ec:	6893      	ldr	r3, [r2, #8]
300091ee:	07d9      	lsls	r1, r3, #31
300091f0:	d4fc      	bmi.n	300091ec <PSRAM_REG_Write+0x68>
300091f2:	4a05      	ldr	r2, [pc, #20]	; (30009208 <PSRAM_REG_Write+0x84>)
300091f4:	6a93      	ldr	r3, [r2, #40]	; 0x28
300091f6:	07db      	lsls	r3, r3, #31
300091f8:	d4fc      	bmi.n	300091f4 <PSRAM_REG_Write+0x70>
300091fa:	6813      	ldr	r3, [r2, #0]
300091fc:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009200:	6013      	str	r3, [r2, #0]
30009202:	bc30      	pop	{r4, r5}
30009204:	4770      	bx	lr
30009206:	bf00      	nop
30009208:	41012000 	.word	0x41012000

3000920c <PSRAM_APM_DEVIC_Init>:
3000920c:	4b12      	ldr	r3, [pc, #72]	; (30009258 <PSRAM_APM_DEVIC_Init+0x4c>)
3000920e:	2102      	movs	r1, #2
30009210:	2000      	movs	r0, #0
30009212:	b530      	push	{r4, r5, lr}
30009214:	4c11      	ldr	r4, [pc, #68]	; (3000925c <PSRAM_APM_DEVIC_Init+0x50>)
30009216:	b083      	sub	sp, #12
30009218:	699d      	ldr	r5, [r3, #24]
3000921a:	466a      	mov	r2, sp
3000921c:	eb04 0c05 	add.w	ip, r4, r5
30009220:	1eeb      	subs	r3, r5, #3
30009222:	4c0f      	ldr	r4, [pc, #60]	; (30009260 <PSRAM_APM_DEVIC_Init+0x54>)
30009224:	408b      	lsls	r3, r1
30009226:	f81c cc03 	ldrb.w	ip, [ip, #-3]
3000922a:	f003 031c 	and.w	r3, r3, #28
3000922e:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30009232:	f043 0320 	orr.w	r3, r3, #32
30009236:	fa5f fc8c 	uxtb.w	ip, ip
3000923a:	f88d 3000 	strb.w	r3, [sp]
3000923e:	f88d c004 	strb.w	ip, [sp, #4]
30009242:	f88d 3001 	strb.w	r3, [sp, #1]
30009246:	f88d c005 	strb.w	ip, [sp, #5]
3000924a:	47a0      	blx	r4
3000924c:	aa01      	add	r2, sp, #4
3000924e:	2102      	movs	r1, #2
30009250:	2004      	movs	r0, #4
30009252:	47a0      	blx	r4
30009254:	b003      	add	sp, #12
30009256:	bd30      	pop	{r4, r5, pc}
30009258:	3000ff70 	.word	0x3000ff70
3000925c:	3000f400 	.word	0x3000f400
30009260:	30009185 	.word	0x30009185

30009264 <PSRAM_WB_REG_Write>:
30009264:	b430      	push	{r4, r5}
30009266:	4c1e      	ldr	r4, [pc, #120]	; (300092e0 <PSRAM_WB_REG_Write+0x7c>)
30009268:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000926a:	f013 0301 	ands.w	r3, r3, #1
3000926e:	d1fb      	bne.n	30009268 <PSRAM_WB_REG_Write+0x4>
30009270:	6825      	ldr	r5, [r4, #0]
30009272:	b2c0      	uxtb	r0, r0
30009274:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009278:	6025      	str	r5, [r4, #0]
3000927a:	60a3      	str	r3, [r4, #8]
3000927c:	6825      	ldr	r5, [r4, #0]
3000927e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009282:	6025      	str	r5, [r4, #0]
30009284:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30009288:	6063      	str	r3, [r4, #4]
3000928a:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000928e:	2560      	movs	r5, #96	; 0x60
30009290:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009294:	2501      	movs	r5, #1
30009296:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000929a:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
3000929e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300092a2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
300092a6:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
300092aa:	b141      	cbz	r1, 300092be <PSRAM_WB_REG_Write+0x5a>
300092ac:	3a01      	subs	r2, #1
300092ae:	480c      	ldr	r0, [pc, #48]	; (300092e0 <PSRAM_WB_REG_Write+0x7c>)
300092b0:	4411      	add	r1, r2
300092b2:	f812 3f01 	ldrb.w	r3, [r2, #1]!
300092b6:	428a      	cmp	r2, r1
300092b8:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300092bc:	d1f9      	bne.n	300092b2 <PSRAM_WB_REG_Write+0x4e>
300092be:	4a08      	ldr	r2, [pc, #32]	; (300092e0 <PSRAM_WB_REG_Write+0x7c>)
300092c0:	2301      	movs	r3, #1
300092c2:	6093      	str	r3, [r2, #8]
300092c4:	6893      	ldr	r3, [r2, #8]
300092c6:	07d9      	lsls	r1, r3, #31
300092c8:	d4fc      	bmi.n	300092c4 <PSRAM_WB_REG_Write+0x60>
300092ca:	4a05      	ldr	r2, [pc, #20]	; (300092e0 <PSRAM_WB_REG_Write+0x7c>)
300092cc:	6a93      	ldr	r3, [r2, #40]	; 0x28
300092ce:	07db      	lsls	r3, r3, #31
300092d0:	d4fc      	bmi.n	300092cc <PSRAM_WB_REG_Write+0x68>
300092d2:	6813      	ldr	r3, [r2, #0]
300092d4:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
300092d8:	6013      	str	r3, [r2, #0]
300092da:	bc30      	pop	{r4, r5}
300092dc:	4770      	bx	lr
300092de:	bf00      	nop
300092e0:	41012000 	.word	0x41012000

300092e4 <PSRAM_WB_DEVIC_Init>:
300092e4:	490b      	ldr	r1, [pc, #44]	; (30009314 <PSRAM_WB_DEVIC_Init+0x30>)
300092e6:	4b0c      	ldr	r3, [pc, #48]	; (30009318 <PSRAM_WB_DEVIC_Init+0x34>)
300092e8:	b510      	push	{r4, lr}
300092ea:	6988      	ldr	r0, [r1, #24]
300092ec:	b082      	sub	sp, #8
300092ee:	248f      	movs	r4, #143	; 0x8f
300092f0:	2102      	movs	r1, #2
300092f2:	4403      	add	r3, r0
300092f4:	aa01      	add	r2, sp, #4
300092f6:	f88d 4004 	strb.w	r4, [sp, #4]
300092fa:	2000      	movs	r0, #0
300092fc:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30009300:	4c06      	ldr	r4, [pc, #24]	; (3000931c <PSRAM_WB_DEVIC_Init+0x38>)
30009302:	011b      	lsls	r3, r3, #4
30009304:	f043 030c 	orr.w	r3, r3, #12
30009308:	f88d 3005 	strb.w	r3, [sp, #5]
3000930c:	47a0      	blx	r4
3000930e:	b002      	add	sp, #8
30009310:	bd10      	pop	{r4, pc}
30009312:	bf00      	nop
30009314:	3000ff70 	.word	0x3000ff70
30009318:	3000f420 	.word	0x3000f420
3000931c:	30009265 	.word	0x30009265

30009320 <PSRAM_calibration>:
30009320:	4a9c      	ldr	r2, [pc, #624]	; (30009594 <PSRAM_calibration+0x274>)
30009322:	2300      	movs	r3, #0
30009324:	499c      	ldr	r1, [pc, #624]	; (30009598 <PSRAM_calibration+0x278>)
30009326:	489d      	ldr	r0, [pc, #628]	; (3000959c <PSRAM_calibration+0x27c>)
30009328:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000932c:	6852      	ldr	r2, [r2, #4]
3000932e:	b093      	sub	sp, #76	; 0x4c
30009330:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
30009334:	400a      	ands	r2, r1
30009336:	9311      	str	r3, [sp, #68]	; 0x44
30009338:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000933c:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
30009340:	920a      	str	r2, [sp, #40]	; 0x28
30009342:	6942      	ldr	r2, [r0, #20]
30009344:	03d1      	lsls	r1, r2, #15
30009346:	d51e      	bpl.n	30009386 <PSRAM_calibration+0x66>
30009348:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000934c:	f3bf 8f4f 	dsb	sy
30009350:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009354:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009358:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000935c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009360:	0164      	lsls	r4, r4, #5
30009362:	ea04 0106 	and.w	r1, r4, r6
30009366:	462b      	mov	r3, r5
30009368:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000936c:	3b01      	subs	r3, #1
3000936e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009372:	1c5a      	adds	r2, r3, #1
30009374:	d1f8      	bne.n	30009368 <PSRAM_calibration+0x48>
30009376:	3c20      	subs	r4, #32
30009378:	f114 0f20 	cmn.w	r4, #32
3000937c:	d1f1      	bne.n	30009362 <PSRAM_calibration+0x42>
3000937e:	f3bf 8f4f 	dsb	sy
30009382:	f3bf 8f6f 	isb	sy
30009386:	4885      	ldr	r0, [pc, #532]	; (3000959c <PSRAM_calibration+0x27c>)
30009388:	2300      	movs	r3, #0
3000938a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000938e:	f3bf 8f4f 	dsb	sy
30009392:	6943      	ldr	r3, [r0, #20]
30009394:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009398:	6143      	str	r3, [r0, #20]
3000939a:	f3bf 8f4f 	dsb	sy
3000939e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300093a2:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300093a6:	f3c5 344e 	ubfx	r4, r5, #13, #15
300093aa:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300093ae:	0164      	lsls	r4, r4, #5
300093b0:	ea04 0106 	and.w	r1, r4, r6
300093b4:	462b      	mov	r3, r5
300093b6:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300093ba:	3b01      	subs	r3, #1
300093bc:	1c5f      	adds	r7, r3, #1
300093be:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300093c2:	d1f8      	bne.n	300093b6 <PSRAM_calibration+0x96>
300093c4:	3c20      	subs	r4, #32
300093c6:	f114 0f20 	cmn.w	r4, #32
300093ca:	d1f1      	bne.n	300093b0 <PSRAM_calibration+0x90>
300093cc:	f3bf 8f4f 	dsb	sy
300093d0:	f3bf 8f6f 	isb	sy
300093d4:	2204      	movs	r2, #4
300093d6:	461e      	mov	r6, r3
300093d8:	9306      	str	r3, [sp, #24]
300093da:	2700      	movs	r7, #0
300093dc:	9209      	str	r2, [sp, #36]	; 0x24
300093de:	46b3      	mov	fp, r6
300093e0:	4a6c      	ldr	r2, [pc, #432]	; (30009594 <PSRAM_calibration+0x274>)
300093e2:	4c6f      	ldr	r4, [pc, #444]	; (300095a0 <PSRAM_calibration+0x280>)
300093e4:	6813      	ldr	r3, [r2, #0]
300093e6:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 300095bc <PSRAM_calibration+0x29c>
300093ea:	f023 0301 	bic.w	r3, r3, #1
300093ee:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 300095c0 <PSRAM_calibration+0x2a0>
300093f2:	6013      	str	r3, [r2, #0]
300093f4:	2301      	movs	r3, #1
300093f6:	9305      	str	r3, [sp, #20]
300093f8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300093fa:	9a05      	ldr	r2, [sp, #20]
300093fc:	9704      	str	r7, [sp, #16]
300093fe:	2700      	movs	r7, #0
30009400:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009404:	463d      	mov	r5, r7
30009406:	e9cd b607 	strd	fp, r6, [sp, #28]
3000940a:	f04f 3bff 	mov.w	fp, #4294967295
3000940e:	465e      	mov	r6, fp
30009410:	e010      	b.n	30009434 <PSRAM_calibration+0x114>
30009412:	f1b6 3fff 	cmp.w	r6, #4294967295
30009416:	bf08      	it	eq
30009418:	462e      	moveq	r6, r5
3000941a:	2d1f      	cmp	r5, #31
3000941c:	eb06 0b07 	add.w	fp, r6, r7
30009420:	f107 0701 	add.w	r7, r7, #1
30009424:	d102      	bne.n	3000942c <PSRAM_calibration+0x10c>
30009426:	9b04      	ldr	r3, [sp, #16]
30009428:	429f      	cmp	r7, r3
3000942a:	dc46      	bgt.n	300094ba <PSRAM_calibration+0x19a>
3000942c:	3501      	adds	r5, #1
3000942e:	2d20      	cmp	r5, #32
30009430:	f000 80ac 	beq.w	3000958c <PSRAM_calibration+0x26c>
30009434:	ea48 0205 	orr.w	r2, r8, r5
30009438:	4b56      	ldr	r3, [pc, #344]	; (30009594 <PSRAM_calibration+0x274>)
3000943a:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000943e:	4959      	ldr	r1, [pc, #356]	; (300095a4 <PSRAM_calibration+0x284>)
30009440:	605a      	str	r2, [r3, #4]
30009442:	6822      	ldr	r2, [r4, #0]
30009444:	4b58      	ldr	r3, [pc, #352]	; (300095a8 <PSRAM_calibration+0x288>)
30009446:	f8ce 2000 	str.w	r2, [lr]
3000944a:	6860      	ldr	r0, [r4, #4]
3000944c:	4a57      	ldr	r2, [pc, #348]	; (300095ac <PSRAM_calibration+0x28c>)
3000944e:	f8c9 0000 	str.w	r0, [r9]
30009452:	68a0      	ldr	r0, [r4, #8]
30009454:	f8ca 0000 	str.w	r0, [sl]
30009458:	68e0      	ldr	r0, [r4, #12]
3000945a:	6018      	str	r0, [r3, #0]
3000945c:	6920      	ldr	r0, [r4, #16]
3000945e:	6008      	str	r0, [r1, #0]
30009460:	6960      	ldr	r0, [r4, #20]
30009462:	6010      	str	r0, [r2, #0]
30009464:	f8de 0000 	ldr.w	r0, [lr]
30009468:	900c      	str	r0, [sp, #48]	; 0x30
3000946a:	f8d9 0000 	ldr.w	r0, [r9]
3000946e:	900d      	str	r0, [sp, #52]	; 0x34
30009470:	f8da 0000 	ldr.w	r0, [sl]
30009474:	900e      	str	r0, [sp, #56]	; 0x38
30009476:	6818      	ldr	r0, [r3, #0]
30009478:	4b4d      	ldr	r3, [pc, #308]	; (300095b0 <PSRAM_calibration+0x290>)
3000947a:	900f      	str	r0, [sp, #60]	; 0x3c
3000947c:	a80c      	add	r0, sp, #48	; 0x30
3000947e:	6809      	ldr	r1, [r1, #0]
30009480:	9110      	str	r1, [sp, #64]	; 0x40
30009482:	6812      	ldr	r2, [r2, #0]
30009484:	4946      	ldr	r1, [pc, #280]	; (300095a0 <PSRAM_calibration+0x280>)
30009486:	9211      	str	r2, [sp, #68]	; 0x44
30009488:	2218      	movs	r2, #24
3000948a:	4798      	blx	r3
3000948c:	2800      	cmp	r0, #0
3000948e:	d0c0      	beq.n	30009412 <PSRAM_calibration+0xf2>
30009490:	1c70      	adds	r0, r6, #1
30009492:	d0cb      	beq.n	3000942c <PSRAM_calibration+0x10c>
30009494:	9b04      	ldr	r3, [sp, #16]
30009496:	42bb      	cmp	r3, r7
30009498:	da0a      	bge.n	300094b0 <PSRAM_calibration+0x190>
3000949a:	9b05      	ldr	r3, [sp, #20]
3000949c:	f8cd b01c 	str.w	fp, [sp, #28]
300094a0:	f04f 3bff 	mov.w	fp, #4294967295
300094a4:	9704      	str	r7, [sp, #16]
300094a6:	2700      	movs	r7, #0
300094a8:	9608      	str	r6, [sp, #32]
300094aa:	465e      	mov	r6, fp
300094ac:	9306      	str	r3, [sp, #24]
300094ae:	e7bd      	b.n	3000942c <PSRAM_calibration+0x10c>
300094b0:	f04f 3bff 	mov.w	fp, #4294967295
300094b4:	2700      	movs	r7, #0
300094b6:	465e      	mov	r6, fp
300094b8:	e7b8      	b.n	3000942c <PSRAM_calibration+0x10c>
300094ba:	9b05      	ldr	r3, [sp, #20]
300094bc:	9306      	str	r3, [sp, #24]
300094be:	9b05      	ldr	r3, [sp, #20]
300094c0:	005b      	lsls	r3, r3, #1
300094c2:	9305      	str	r3, [sp, #20]
300094c4:	9b09      	ldr	r3, [sp, #36]	; 0x24
300094c6:	3b01      	subs	r3, #1
300094c8:	9309      	str	r3, [sp, #36]	; 0x24
300094ca:	d195      	bne.n	300093f8 <PSRAM_calibration+0xd8>
300094cc:	9b06      	ldr	r3, [sp, #24]
300094ce:	2004      	movs	r0, #4
300094d0:	4a38      	ldr	r2, [pc, #224]	; (300095b4 <PSRAM_calibration+0x294>)
300094d2:	e9cd 7302 	strd	r7, r3, [sp, #8]
300094d6:	4b38      	ldr	r3, [pc, #224]	; (300095b8 <PSRAM_calibration+0x298>)
300094d8:	e9cd 6b00 	strd	r6, fp, [sp]
300094dc:	4619      	mov	r1, r3
300094de:	f002 fe2f 	bl	3000c140 <rtk_log_write>
300094e2:	482e      	ldr	r0, [pc, #184]	; (3000959c <PSRAM_calibration+0x27c>)
300094e4:	6943      	ldr	r3, [r0, #20]
300094e6:	03d9      	lsls	r1, r3, #15
300094e8:	d425      	bmi.n	30009536 <PSRAM_calibration+0x216>
300094ea:	9b09      	ldr	r3, [sp, #36]	; 0x24
300094ec:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300094f0:	f3bf 8f4f 	dsb	sy
300094f4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300094f8:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
300094fc:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009500:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009504:	0164      	lsls	r4, r4, #5
30009506:	ea04 010c 	and.w	r1, r4, ip
3000950a:	462b      	mov	r3, r5
3000950c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009510:	3b01      	subs	r3, #1
30009512:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009516:	1c5a      	adds	r2, r3, #1
30009518:	d1f8      	bne.n	3000950c <PSRAM_calibration+0x1ec>
3000951a:	3c20      	subs	r4, #32
3000951c:	f114 0f20 	cmn.w	r4, #32
30009520:	d1f1      	bne.n	30009506 <PSRAM_calibration+0x1e6>
30009522:	f3bf 8f4f 	dsb	sy
30009526:	6943      	ldr	r3, [r0, #20]
30009528:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
3000952c:	6143      	str	r3, [r0, #20]
3000952e:	f3bf 8f4f 	dsb	sy
30009532:	f3bf 8f6f 	isb	sy
30009536:	2f08      	cmp	r7, #8
30009538:	dd24      	ble.n	30009584 <PSRAM_calibration+0x264>
3000953a:	ebbb 0306 	subs.w	r3, fp, r6
3000953e:	9806      	ldr	r0, [sp, #24]
30009540:	bf48      	it	mi
30009542:	3301      	addmi	r3, #1
30009544:	eb16 060b 	adds.w	r6, r6, fp
30009548:	ea4f 0363 	mov.w	r3, r3, asr #1
3000954c:	bf48      	it	mi
3000954e:	3601      	addmi	r6, #1
30009550:	1e9a      	subs	r2, r3, #2
30009552:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009554:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009558:	0414      	lsls	r4, r2, #16
3000955a:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000955e:	0212      	lsls	r2, r2, #8
30009560:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009564:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009568:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000956c:	2001      	movs	r0, #1
3000956e:	430b      	orrs	r3, r1
30009570:	4908      	ldr	r1, [pc, #32]	; (30009594 <PSRAM_calibration+0x274>)
30009572:	4323      	orrs	r3, r4
30009574:	4313      	orrs	r3, r2
30009576:	604b      	str	r3, [r1, #4]
30009578:	680b      	ldr	r3, [r1, #0]
3000957a:	4303      	orrs	r3, r0
3000957c:	600b      	str	r3, [r1, #0]
3000957e:	b013      	add	sp, #76	; 0x4c
30009580:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009584:	2000      	movs	r0, #0
30009586:	b013      	add	sp, #76	; 0x4c
30009588:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000958c:	9f04      	ldr	r7, [sp, #16]
3000958e:	e9dd b607 	ldrd	fp, r6, [sp, #28]
30009592:	e794      	b.n	300094be <PSRAM_calibration+0x19e>
30009594:	41002000 	.word	0x41002000
30009598:	0fffffe0 	.word	0x0fffffe0
3000959c:	e000ed00 	.word	0xe000ed00
300095a0:	3000f408 	.word	0x3000f408
300095a4:	60200000 	.word	0x60200000
300095a8:	60150000 	.word	0x60150000
300095ac:	60250000 	.word	0x60250000
300095b0:	000129bd 	.word	0x000129bd
300095b4:	3000da7c 	.word	0x3000da7c
300095b8:	3000da54 	.word	0x3000da54
300095bc:	60050000 	.word	0x60050000
300095c0:	60100000 	.word	0x60100000

300095c4 <PSRAM_AutoGating>:
300095c4:	b1c0      	cbz	r0, 300095f8 <PSRAM_AutoGating+0x34>
300095c6:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300095ca:	4810      	ldr	r0, [pc, #64]	; (3000960c <PSRAM_AutoGating+0x48>)
300095cc:	f3c1 0108 	ubfx	r1, r1, #0, #9
300095d0:	0252      	lsls	r2, r2, #9
300095d2:	b410      	push	{r4}
300095d4:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
300095d8:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300095dc:	4020      	ands	r0, r4
300095de:	4301      	orrs	r1, r0
300095e0:	430a      	orrs	r2, r1
300095e2:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300095e6:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300095ea:	f042 0201 	orr.w	r2, r2, #1
300095ee:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300095f2:	f85d 4b04 	ldr.w	r4, [sp], #4
300095f6:	4770      	bx	lr
300095f8:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300095fc:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009600:	f023 0301 	bic.w	r3, r3, #1
30009604:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009608:	4770      	bx	lr
3000960a:	bf00      	nop
3000960c:	ffffc000 	.word	0xffffc000

30009610 <BOOT_Reason>:
30009610:	4b02      	ldr	r3, [pc, #8]	; (3000961c <BOOT_Reason+0xc>)
30009612:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009616:	b280      	uxth	r0, r0
30009618:	4770      	bx	lr
3000961a:	bf00      	nop
3000961c:	42008200 	.word	0x42008200

30009620 <CRYPTO_SetSecurityModeAD>:
30009620:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009624:	b083      	sub	sp, #12
30009626:	460d      	mov	r5, r1
30009628:	4616      	mov	r6, r2
3000962a:	4699      	mov	r9, r3
3000962c:	4604      	mov	r4, r0
3000962e:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30009632:	2800      	cmp	r0, #0
30009634:	f000 80a9 	beq.w	3000978a <CRYPTO_SetSecurityModeAD+0x16a>
30009638:	7823      	ldrb	r3, [r4, #0]
3000963a:	2b01      	cmp	r3, #1
3000963c:	d003      	beq.n	30009646 <CRYPTO_SetSecurityModeAD+0x26>
3000963e:	2125      	movs	r1, #37	; 0x25
30009640:	485d      	ldr	r0, [pc, #372]	; (300097b8 <CRYPTO_SetSecurityModeAD+0x198>)
30009642:	f003 f861 	bl	3000c708 <__io_assert_failed_veneer>
30009646:	2300      	movs	r3, #0
30009648:	1c6a      	adds	r2, r5, #1
3000964a:	6065      	str	r5, [r4, #4]
3000964c:	8123      	strh	r3, [r4, #8]
3000964e:	d008      	beq.n	30009662 <CRYPTO_SetSecurityModeAD+0x42>
30009650:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009654:	2b20      	cmp	r3, #32
30009656:	d07b      	beq.n	30009750 <CRYPTO_SetSecurityModeAD+0x130>
30009658:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000965c:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009660:	7265      	strb	r5, [r4, #9]
30009662:	1c73      	adds	r3, r6, #1
30009664:	60e6      	str	r6, [r4, #12]
30009666:	f000 8082 	beq.w	3000976e <CRYPTO_SetSecurityModeAD+0x14e>
3000966a:	f006 0303 	and.w	r3, r6, #3
3000966e:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009672:	7422      	strb	r2, [r4, #16]
30009674:	f1a3 0202 	sub.w	r2, r3, #2
30009678:	2b01      	cmp	r3, #1
3000967a:	fab2 f282 	clz	r2, r2
3000967e:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009682:	7462      	strb	r2, [r4, #17]
30009684:	fab3 f283 	clz	r2, r3
30009688:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000968c:	74a2      	strb	r2, [r4, #18]
3000968e:	bf0c      	ite	eq
30009690:	2201      	moveq	r2, #1
30009692:	2200      	movne	r2, #0
30009694:	74e2      	strb	r2, [r4, #19]
30009696:	d13f      	bne.n	30009718 <CRYPTO_SetSecurityModeAD+0xf8>
30009698:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
3000969c:	2b40      	cmp	r3, #64	; 0x40
3000969e:	f000 8083 	beq.w	300097a8 <CRYPTO_SetSecurityModeAD+0x188>
300096a2:	d86a      	bhi.n	3000977a <CRYPTO_SetSecurityModeAD+0x15a>
300096a4:	2b10      	cmp	r3, #16
300096a6:	d06d      	beq.n	30009784 <CRYPTO_SetSecurityModeAD+0x164>
300096a8:	2b20      	cmp	r3, #32
300096aa:	d173      	bne.n	30009794 <CRYPTO_SetSecurityModeAD+0x174>
300096ac:	7523      	strb	r3, [r4, #20]
300096ae:	62a3      	str	r3, [r4, #40]	; 0x28
300096b0:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300096b2:	6227      	str	r7, [r4, #32]
300096b4:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300096b8:	2f00      	cmp	r7, #0
300096ba:	d038      	beq.n	3000972e <CRYPTO_SetSecurityModeAD+0x10e>
300096bc:	7d23      	ldrb	r3, [r4, #20]
300096be:	f104 0064 	add.w	r0, r4, #100	; 0x64
300096c2:	2b30      	cmp	r3, #48	; 0x30
300096c4:	65e0      	str	r0, [r4, #92]	; 0x5c
300096c6:	d046      	beq.n	30009756 <CRYPTO_SetSecurityModeAD+0x136>
300096c8:	2b40      	cmp	r3, #64	; 0x40
300096ca:	d044      	beq.n	30009756 <CRYPTO_SetSecurityModeAD+0x136>
300096cc:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
300096d0:	2240      	movs	r2, #64	; 0x40
300096d2:	2136      	movs	r1, #54	; 0x36
300096d4:	4d39      	ldr	r5, [pc, #228]	; (300097bc <CRYPTO_SetSecurityModeAD+0x19c>)
300096d6:	6623      	str	r3, [r4, #96]	; 0x60
300096d8:	47a8      	blx	r5
300096da:	2240      	movs	r2, #64	; 0x40
300096dc:	215c      	movs	r1, #92	; 0x5c
300096de:	6e20      	ldr	r0, [r4, #96]	; 0x60
300096e0:	47a8      	blx	r5
300096e2:	4b37      	ldr	r3, [pc, #220]	; (300097c0 <CRYPTO_SetSecurityModeAD+0x1a0>)
300096e4:	681d      	ldr	r5, [r3, #0]
300096e6:	bb25      	cbnz	r5, 30009732 <CRYPTO_SetSecurityModeAD+0x112>
300096e8:	f1b8 0f00 	cmp.w	r8, #0
300096ec:	d05f      	beq.n	300097ae <CRYPTO_SetSecurityModeAD+0x18e>
300096ee:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
300096f2:	e001      	b.n	300096f8 <CRYPTO_SetSecurityModeAD+0xd8>
300096f4:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
300096f8:	6de2      	ldr	r2, [r4, #92]	; 0x5c
300096fa:	f818 3005 	ldrb.w	r3, [r8, r5]
300096fe:	5d51      	ldrb	r1, [r2, r5]
30009700:	404b      	eors	r3, r1
30009702:	5553      	strb	r3, [r2, r5]
30009704:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009706:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009708:	5d51      	ldrb	r1, [r2, r5]
3000970a:	5d5b      	ldrb	r3, [r3, r5]
3000970c:	404b      	eors	r3, r1
3000970e:	5553      	strb	r3, [r2, r5]
30009710:	3501      	adds	r5, #1
30009712:	42af      	cmp	r7, r5
30009714:	d1ee      	bne.n	300096f4 <CRYPTO_SetSecurityModeAD+0xd4>
30009716:	e00c      	b.n	30009732 <CRYPTO_SetSecurityModeAD+0x112>
30009718:	2200      	movs	r2, #0
3000971a:	7522      	strb	r2, [r4, #20]
3000971c:	b99b      	cbnz	r3, 30009746 <CRYPTO_SetSecurityModeAD+0x126>
3000971e:	2314      	movs	r3, #20
30009720:	6227      	str	r7, [r4, #32]
30009722:	62a3      	str	r3, [r4, #40]	; 0x28
30009724:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009726:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
3000972a:	2f00      	cmp	r7, #0
3000972c:	d1c6      	bne.n	300096bc <CRYPTO_SetSecurityModeAD+0x9c>
3000972e:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009732:	3601      	adds	r6, #1
30009734:	d003      	beq.n	3000973e <CRYPTO_SetSecurityModeAD+0x11e>
30009736:	2201      	movs	r2, #1
30009738:	2300      	movs	r3, #0
3000973a:	85a2      	strh	r2, [r4, #44]	; 0x2c
3000973c:	6323      	str	r3, [r4, #48]	; 0x30
3000973e:	2000      	movs	r0, #0
30009740:	b003      	add	sp, #12
30009742:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009746:	2b02      	cmp	r3, #2
30009748:	d114      	bne.n	30009774 <CRYPTO_SetSecurityModeAD+0x154>
3000974a:	2310      	movs	r3, #16
3000974c:	62a3      	str	r3, [r4, #40]	; 0x28
3000974e:	e7af      	b.n	300096b0 <CRYPTO_SetSecurityModeAD+0x90>
30009750:	2301      	movs	r3, #1
30009752:	7223      	strb	r3, [r4, #8]
30009754:	e780      	b.n	30009658 <CRYPTO_SetSecurityModeAD+0x38>
30009756:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
3000975a:	2280      	movs	r2, #128	; 0x80
3000975c:	2136      	movs	r1, #54	; 0x36
3000975e:	4d17      	ldr	r5, [pc, #92]	; (300097bc <CRYPTO_SetSecurityModeAD+0x19c>)
30009760:	6623      	str	r3, [r4, #96]	; 0x60
30009762:	47a8      	blx	r5
30009764:	2280      	movs	r2, #128	; 0x80
30009766:	215c      	movs	r1, #92	; 0x5c
30009768:	6e20      	ldr	r0, [r4, #96]	; 0x60
3000976a:	47a8      	blx	r5
3000976c:	e7b9      	b.n	300096e2 <CRYPTO_SetSecurityModeAD+0xc2>
3000976e:	2300      	movs	r3, #0
30009770:	6123      	str	r3, [r4, #16]
30009772:	7523      	strb	r3, [r4, #20]
30009774:	2300      	movs	r3, #0
30009776:	62a3      	str	r3, [r4, #40]	; 0x28
30009778:	e79a      	b.n	300096b0 <CRYPTO_SetSecurityModeAD+0x90>
3000977a:	2b80      	cmp	r3, #128	; 0x80
3000977c:	d10a      	bne.n	30009794 <CRYPTO_SetSecurityModeAD+0x174>
3000977e:	2340      	movs	r3, #64	; 0x40
30009780:	7523      	strb	r3, [r4, #20]
30009782:	e794      	b.n	300096ae <CRYPTO_SetSecurityModeAD+0x8e>
30009784:	231c      	movs	r3, #28
30009786:	7523      	strb	r3, [r4, #20]
30009788:	e7f5      	b.n	30009776 <CRYPTO_SetSecurityModeAD+0x156>
3000978a:	2124      	movs	r1, #36	; 0x24
3000978c:	480a      	ldr	r0, [pc, #40]	; (300097b8 <CRYPTO_SetSecurityModeAD+0x198>)
3000978e:	f002 ffbb 	bl	3000c708 <__io_assert_failed_veneer>
30009792:	e751      	b.n	30009638 <CRYPTO_SetSecurityModeAD+0x18>
30009794:	4b0b      	ldr	r3, [pc, #44]	; (300097c4 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009796:	2002      	movs	r0, #2
30009798:	4a0b      	ldr	r2, [pc, #44]	; (300097c8 <CRYPTO_SetSecurityModeAD+0x1a8>)
3000979a:	4619      	mov	r1, r3
3000979c:	9600      	str	r6, [sp, #0]
3000979e:	f002 fccf 	bl	3000c140 <rtk_log_write>
300097a2:	2300      	movs	r3, #0
300097a4:	7523      	strb	r3, [r4, #20]
300097a6:	e7e6      	b.n	30009776 <CRYPTO_SetSecurityModeAD+0x156>
300097a8:	2330      	movs	r3, #48	; 0x30
300097aa:	7523      	strb	r3, [r4, #20]
300097ac:	e7e3      	b.n	30009776 <CRYPTO_SetSecurityModeAD+0x156>
300097ae:	2180      	movs	r1, #128	; 0x80
300097b0:	4801      	ldr	r0, [pc, #4]	; (300097b8 <CRYPTO_SetSecurityModeAD+0x198>)
300097b2:	f002 ffa9 	bl	3000c708 <__io_assert_failed_veneer>
300097b6:	e79a      	b.n	300096ee <CRYPTO_SetSecurityModeAD+0xce>
300097b8:	3000db58 	.word	0x3000db58
300097bc:	00012be5 	.word	0x00012be5
300097c0:	2001c200 	.word	0x2001c200
300097c4:	3000dabc 	.word	0x3000dabc
300097c8:	3000dac8 	.word	0x3000dac8

300097cc <rtl_crypto_hmac_sha2_init>:
300097cc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300097d0:	4c97      	ldr	r4, [pc, #604]	; (30009a30 <rtl_crypto_hmac_sha2_init+0x264>)
300097d2:	4617      	mov	r7, r2
300097d4:	e844 f400 	tt	r4, r4
300097d8:	4a96      	ldr	r2, [pc, #600]	; (30009a34 <rtl_crypto_hmac_sha2_init+0x268>)
300097da:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
300097de:	461c      	mov	r4, r3
300097e0:	4b95      	ldr	r3, [pc, #596]	; (30009a38 <rtl_crypto_hmac_sha2_init+0x26c>)
300097e2:	bf18      	it	ne
300097e4:	4613      	movne	r3, r2
300097e6:	b085      	sub	sp, #20
300097e8:	4605      	mov	r5, r0
300097ea:	460e      	mov	r6, r1
300097ec:	6b5b      	ldr	r3, [r3, #52]	; 0x34
300097ee:	2b00      	cmp	r3, #0
300097f0:	db05      	blt.n	300097fe <rtl_crypto_hmac_sha2_init+0x32>
300097f2:	2900      	cmp	r1, #0
300097f4:	f000 8118 	beq.w	30009a28 <rtl_crypto_hmac_sha2_init+0x25c>
300097f8:	078b      	lsls	r3, r1, #30
300097fa:	f040 8112 	bne.w	30009a22 <rtl_crypto_hmac_sha2_init+0x256>
300097fe:	2d20      	cmp	r5, #32
30009800:	bf18      	it	ne
30009802:	2d1c      	cmpne	r5, #28
30009804:	bf0c      	ite	eq
30009806:	f04f 0901 	moveq.w	r9, #1
3000980a:	f04f 0900 	movne.w	r9, #0
3000980e:	d04d      	beq.n	300098ac <rtl_crypto_hmac_sha2_init+0xe0>
30009810:	2d40      	cmp	r5, #64	; 0x40
30009812:	bf18      	it	ne
30009814:	2d30      	cmpne	r5, #48	; 0x30
30009816:	d102      	bne.n	3000981e <rtl_crypto_hmac_sha2_init+0x52>
30009818:	2f80      	cmp	r7, #128	; 0x80
3000981a:	f200 80f9 	bhi.w	30009a10 <rtl_crypto_hmac_sha2_init+0x244>
3000981e:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009a58 <rtl_crypto_hmac_sha2_init+0x28c>
30009822:	f898 3000 	ldrb.w	r3, [r8]
30009826:	2b01      	cmp	r3, #1
30009828:	d003      	beq.n	30009832 <rtl_crypto_hmac_sha2_init+0x66>
3000982a:	21b4      	movs	r1, #180	; 0xb4
3000982c:	4883      	ldr	r0, [pc, #524]	; (30009a3c <rtl_crypto_hmac_sha2_init+0x270>)
3000982e:	f002 ff6b 	bl	3000c708 <__io_assert_failed_veneer>
30009832:	2d30      	cmp	r5, #48	; 0x30
30009834:	f000 809b 	beq.w	3000996e <rtl_crypto_hmac_sha2_init+0x1a2>
30009838:	d83f      	bhi.n	300098ba <rtl_crypto_hmac_sha2_init+0xee>
3000983a:	2d1c      	cmp	r5, #28
3000983c:	f000 80cb 	beq.w	300099d6 <rtl_crypto_hmac_sha2_init+0x20a>
30009840:	f1b9 0f00 	cmp.w	r9, #0
30009844:	d078      	beq.n	30009938 <rtl_crypto_hmac_sha2_init+0x16c>
30009846:	2c00      	cmp	r4, #0
30009848:	f000 808f 	beq.w	3000996a <rtl_crypto_hmac_sha2_init+0x19e>
3000984c:	4b7c      	ldr	r3, [pc, #496]	; (30009a40 <rtl_crypto_hmac_sha2_init+0x274>)
3000984e:	f8df e210 	ldr.w	lr, [pc, #528]	; 30009a60 <rtl_crypto_hmac_sha2_init+0x294>
30009852:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30009a70 <rtl_crypto_hmac_sha2_init+0x2a4>
30009856:	4a7b      	ldr	r2, [pc, #492]	; (30009a44 <rtl_crypto_hmac_sha2_init+0x278>)
30009858:	e9c4 e302 	strd	lr, r3, [r4, #8]
3000985c:	4b7a      	ldr	r3, [pc, #488]	; (30009a48 <rtl_crypto_hmac_sha2_init+0x27c>)
3000985e:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009862:	4b7a      	ldr	r3, [pc, #488]	; (30009a4c <rtl_crypto_hmac_sha2_init+0x280>)
30009864:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009868:	4b79      	ldr	r3, [pc, #484]	; (30009a50 <rtl_crypto_hmac_sha2_init+0x284>)
3000986a:	2225      	movs	r2, #37	; 0x25
3000986c:	6223      	str	r3, [r4, #32]
3000986e:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009872:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009876:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
3000987a:	6263      	str	r3, [r4, #36]	; 0x24
3000987c:	2300      	movs	r3, #0
3000987e:	f04f 31ff 	mov.w	r1, #4294967295
30009882:	4d74      	ldr	r5, [pc, #464]	; (30009a54 <rtl_crypto_hmac_sha2_init+0x288>)
30009884:	9300      	str	r3, [sp, #0]
30009886:	9702      	str	r7, [sp, #8]
30009888:	9601      	str	r6, [sp, #4]
3000988a:	4873      	ldr	r0, [pc, #460]	; (30009a58 <rtl_crypto_hmac_sha2_init+0x28c>)
3000988c:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009890:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009894:	47a8      	blx	r5
30009896:	4605      	mov	r5, r0
30009898:	4b70      	ldr	r3, [pc, #448]	; (30009a5c <rtl_crypto_hmac_sha2_init+0x290>)
3000989a:	4620      	mov	r0, r4
3000989c:	4798      	blx	r3
3000989e:	2301      	movs	r3, #1
300098a0:	4628      	mov	r0, r5
300098a2:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
300098a6:	b005      	add	sp, #20
300098a8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300098ac:	2f40      	cmp	r7, #64	; 0x40
300098ae:	d9b6      	bls.n	3000981e <rtl_crypto_hmac_sha2_init+0x52>
300098b0:	21ae      	movs	r1, #174	; 0xae
300098b2:	4862      	ldr	r0, [pc, #392]	; (30009a3c <rtl_crypto_hmac_sha2_init+0x270>)
300098b4:	f002 ff28 	bl	3000c708 <__io_assert_failed_veneer>
300098b8:	e7b1      	b.n	3000981e <rtl_crypto_hmac_sha2_init+0x52>
300098ba:	2d40      	cmp	r5, #64	; 0x40
300098bc:	d13c      	bne.n	30009938 <rtl_crypto_hmac_sha2_init+0x16c>
300098be:	2c00      	cmp	r4, #0
300098c0:	d040      	beq.n	30009944 <rtl_crypto_hmac_sha2_init+0x178>
300098c2:	4b67      	ldr	r3, [pc, #412]	; (30009a60 <rtl_crypto_hmac_sha2_init+0x294>)
300098c4:	4a67      	ldr	r2, [pc, #412]	; (30009a64 <rtl_crypto_hmac_sha2_init+0x298>)
300098c6:	4968      	ldr	r1, [pc, #416]	; (30009a68 <rtl_crypto_hmac_sha2_init+0x29c>)
300098c8:	4d68      	ldr	r5, [pc, #416]	; (30009a6c <rtl_crypto_hmac_sha2_init+0x2a0>)
300098ca:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009ab0 <rtl_crypto_hmac_sha2_init+0x2e4>
300098ce:	e9c4 3202 	strd	r3, r2, [r4, #8]
300098d2:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
300098d6:	2285      	movs	r2, #133	; 0x85
300098d8:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
300098dc:	33d4      	adds	r3, #212	; 0xd4
300098de:	6163      	str	r3, [r4, #20]
300098e0:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
300098e4:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
300098e8:	f203 734a 	addw	r3, r3, #1866	; 0x74a
300098ec:	6123      	str	r3, [r4, #16]
300098ee:	4b60      	ldr	r3, [pc, #384]	; (30009a70 <rtl_crypto_hmac_sha2_init+0x2a4>)
300098f0:	e9c4 3106 	strd	r3, r1, [r4, #24]
300098f4:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
300098f8:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
300098fc:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009900:	6263      	str	r3, [r4, #36]	; 0x24
30009902:	4b51      	ldr	r3, [pc, #324]	; (30009a48 <rtl_crypto_hmac_sha2_init+0x27c>)
30009904:	6223      	str	r3, [r4, #32]
30009906:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
3000990a:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
3000990e:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009912:	62e3      	str	r3, [r4, #44]	; 0x2c
30009914:	4b4b      	ldr	r3, [pc, #300]	; (30009a44 <rtl_crypto_hmac_sha2_init+0x278>)
30009916:	62a3      	str	r3, [r4, #40]	; 0x28
30009918:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
3000991c:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009920:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009924:	6363      	str	r3, [r4, #52]	; 0x34
30009926:	4b49      	ldr	r3, [pc, #292]	; (30009a4c <rtl_crypto_hmac_sha2_init+0x280>)
30009928:	6323      	str	r3, [r4, #48]	; 0x30
3000992a:	4b49      	ldr	r3, [pc, #292]	; (30009a50 <rtl_crypto_hmac_sha2_init+0x284>)
3000992c:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009930:	4b50      	ldr	r3, [pc, #320]	; (30009a74 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009932:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009936:	e7a1      	b.n	3000987c <rtl_crypto_hmac_sha2_init+0xb0>
30009938:	f04f 35ff 	mov.w	r5, #4294967295
3000993c:	4628      	mov	r0, r5
3000993e:	b005      	add	sp, #20
30009940:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009944:	2285      	movs	r2, #133	; 0x85
30009946:	2300      	movs	r3, #0
30009948:	f04f 31ff 	mov.w	r1, #4294967295
3000994c:	4c41      	ldr	r4, [pc, #260]	; (30009a54 <rtl_crypto_hmac_sha2_init+0x288>)
3000994e:	9702      	str	r7, [sp, #8]
30009950:	4841      	ldr	r0, [pc, #260]	; (30009a58 <rtl_crypto_hmac_sha2_init+0x28c>)
30009952:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009956:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
3000995a:	e9cd 3600 	strd	r3, r6, [sp]
3000995e:	47a0      	blx	r4
30009960:	4605      	mov	r5, r0
30009962:	4628      	mov	r0, r5
30009964:	b005      	add	sp, #20
30009966:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000996a:	2225      	movs	r2, #37	; 0x25
3000996c:	e7eb      	b.n	30009946 <rtl_crypto_hmac_sha2_init+0x17a>
3000996e:	2c00      	cmp	r4, #0
30009970:	d053      	beq.n	30009a1a <rtl_crypto_hmac_sha2_init+0x24e>
30009972:	4b41      	ldr	r3, [pc, #260]	; (30009a78 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009974:	f8df e130 	ldr.w	lr, [pc, #304]	; 30009aa8 <rtl_crypto_hmac_sha2_init+0x2dc>
30009978:	4a40      	ldr	r2, [pc, #256]	; (30009a7c <rtl_crypto_hmac_sha2_init+0x2b0>)
3000997a:	4941      	ldr	r1, [pc, #260]	; (30009a80 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000997c:	4d41      	ldr	r5, [pc, #260]	; (30009a84 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000997e:	f8df c12c 	ldr.w	ip, [pc, #300]	; 30009aac <rtl_crypto_hmac_sha2_init+0x2e0>
30009982:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009986:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
3000998a:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000998e:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30009992:	6163      	str	r3, [r4, #20]
30009994:	4b3c      	ldr	r3, [pc, #240]	; (30009a88 <rtl_crypto_hmac_sha2_init+0x2bc>)
30009996:	6123      	str	r3, [r4, #16]
30009998:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
3000999c:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
300099a0:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
300099a4:	61e3      	str	r3, [r4, #28]
300099a6:	4b39      	ldr	r3, [pc, #228]	; (30009a8c <rtl_crypto_hmac_sha2_init+0x2c0>)
300099a8:	61a3      	str	r3, [r4, #24]
300099aa:	4b39      	ldr	r3, [pc, #228]	; (30009a90 <rtl_crypto_hmac_sha2_init+0x2c4>)
300099ac:	e9c4 3208 	strd	r3, r2, [r4, #32]
300099b0:	4b38      	ldr	r3, [pc, #224]	; (30009a94 <rtl_crypto_hmac_sha2_init+0x2c8>)
300099b2:	2245      	movs	r2, #69	; 0x45
300099b4:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
300099b8:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
300099bc:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
300099c0:	33aa      	adds	r3, #170	; 0xaa
300099c2:	6363      	str	r3, [r4, #52]	; 0x34
300099c4:	4b34      	ldr	r3, [pc, #208]	; (30009a98 <rtl_crypto_hmac_sha2_init+0x2cc>)
300099c6:	6323      	str	r3, [r4, #48]	; 0x30
300099c8:	4b34      	ldr	r3, [pc, #208]	; (30009a9c <rtl_crypto_hmac_sha2_init+0x2d0>)
300099ca:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
300099ce:	4b34      	ldr	r3, [pc, #208]	; (30009aa0 <rtl_crypto_hmac_sha2_init+0x2d4>)
300099d0:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
300099d4:	e752      	b.n	3000987c <rtl_crypto_hmac_sha2_init+0xb0>
300099d6:	b314      	cbz	r4, 30009a1e <rtl_crypto_hmac_sha2_init+0x252>
300099d8:	4b32      	ldr	r3, [pc, #200]	; (30009aa4 <rtl_crypto_hmac_sha2_init+0x2d8>)
300099da:	2215      	movs	r2, #21
300099dc:	4932      	ldr	r1, [pc, #200]	; (30009aa8 <rtl_crypto_hmac_sha2_init+0x2dc>)
300099de:	4827      	ldr	r0, [pc, #156]	; (30009a7c <rtl_crypto_hmac_sha2_init+0x2b0>)
300099e0:	4d28      	ldr	r5, [pc, #160]	; (30009a84 <rtl_crypto_hmac_sha2_init+0x2b8>)
300099e2:	e9c4 1302 	strd	r1, r3, [r4, #8]
300099e6:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
300099ea:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
300099ee:	f503 6301 	add.w	r3, r3, #2064	; 0x810
300099f2:	6123      	str	r3, [r4, #16]
300099f4:	4b22      	ldr	r3, [pc, #136]	; (30009a80 <rtl_crypto_hmac_sha2_init+0x2b4>)
300099f6:	e9c4 0305 	strd	r0, r3, [r4, #20]
300099fa:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
300099fe:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30009a02:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009a06:	61e3      	str	r3, [r4, #28]
30009a08:	4b28      	ldr	r3, [pc, #160]	; (30009aac <rtl_crypto_hmac_sha2_init+0x2e0>)
30009a0a:	e9c4 5308 	strd	r5, r3, [r4, #32]
30009a0e:	e735      	b.n	3000987c <rtl_crypto_hmac_sha2_init+0xb0>
30009a10:	21b1      	movs	r1, #177	; 0xb1
30009a12:	480a      	ldr	r0, [pc, #40]	; (30009a3c <rtl_crypto_hmac_sha2_init+0x270>)
30009a14:	f002 fe78 	bl	3000c708 <__io_assert_failed_veneer>
30009a18:	e701      	b.n	3000981e <rtl_crypto_hmac_sha2_init+0x52>
30009a1a:	2245      	movs	r2, #69	; 0x45
30009a1c:	e793      	b.n	30009946 <rtl_crypto_hmac_sha2_init+0x17a>
30009a1e:	2215      	movs	r2, #21
30009a20:	e791      	b.n	30009946 <rtl_crypto_hmac_sha2_init+0x17a>
30009a22:	f06f 0505 	mvn.w	r5, #5
30009a26:	e789      	b.n	3000993c <rtl_crypto_hmac_sha2_init+0x170>
30009a28:	f06f 0503 	mvn.w	r5, #3
30009a2c:	e786      	b.n	3000993c <rtl_crypto_hmac_sha2_init+0x170>
30009a2e:	bf00      	nop
30009a30:	0000e5f9 	.word	0x0000e5f9
30009a34:	500c8000 	.word	0x500c8000
30009a38:	400c8000 	.word	0x400c8000
30009a3c:	3000db3c 	.word	0x3000db3c
30009a40:	bb67ae85 	.word	0xbb67ae85
30009a44:	510e527f 	.word	0x510e527f
30009a48:	a54ff53a 	.word	0xa54ff53a
30009a4c:	9b05688c 	.word	0x9b05688c
30009a50:	1f83d9ab 	.word	0x1f83d9ab
30009a54:	30009621 	.word	0x30009621
30009a58:	2001c460 	.word	0x2001c460
30009a5c:	00004c99 	.word	0x00004c99
30009a60:	6a09e667 	.word	0x6a09e667
30009a64:	f3bcc908 	.word	0xf3bcc908
30009a68:	fe94f82b 	.word	0xfe94f82b
30009a6c:	fb41bd6b 	.word	0xfb41bd6b
30009a70:	3c6ef372 	.word	0x3c6ef372
30009a74:	5be0cd19 	.word	0x5be0cd19
30009a78:	cbbb9d5d 	.word	0xcbbb9d5d
30009a7c:	f70e5939 	.word	0xf70e5939
30009a80:	ffc00b31 	.word	0xffc00b31
30009a84:	64f98fa7 	.word	0x64f98fa7
30009a88:	629a292a 	.word	0x629a292a
30009a8c:	9159015a 	.word	0x9159015a
30009a90:	152fecd8 	.word	0x152fecd8
30009a94:	67332667 	.word	0x67332667
30009a98:	8eb44a87 	.word	0x8eb44a87
30009a9c:	db0c2e0d 	.word	0xdb0c2e0d
30009aa0:	47b5481d 	.word	0x47b5481d
30009aa4:	367cd507 	.word	0x367cd507
30009aa8:	c1059ed8 	.word	0xc1059ed8
30009aac:	befa4fa4 	.word	0xbefa4fa4
30009ab0:	137e2179 	.word	0x137e2179

30009ab4 <CPU_ClkGet>:
30009ab4:	b570      	push	{r4, r5, r6, lr}
30009ab6:	4d1e      	ldr	r5, [pc, #120]	; (30009b30 <CPU_ClkGet+0x7c>)
30009ab8:	4b1e      	ldr	r3, [pc, #120]	; (30009b34 <CPU_ClkGet+0x80>)
30009aba:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
30009abe:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009ac2:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009ac6:	4798      	blx	r3
30009ac8:	b92c      	cbnz	r4, 30009ad6 <CPU_ClkGet+0x22>
30009aca:	2801      	cmp	r0, #1
30009acc:	d007      	beq.n	30009ade <CPU_ClkGet+0x2a>
30009ace:	07b2      	lsls	r2, r6, #30
30009ad0:	d407      	bmi.n	30009ae2 <CPU_ClkGet+0x2e>
30009ad2:	4819      	ldr	r0, [pc, #100]	; (30009b38 <CPU_ClkGet+0x84>)
30009ad4:	bd70      	pop	{r4, r5, r6, pc}
30009ad6:	2c01      	cmp	r4, #1
30009ad8:	d007      	beq.n	30009aea <CPU_ClkGet+0x36>
30009ada:	2000      	movs	r0, #0
30009adc:	bd70      	pop	{r4, r5, r6, pc}
30009ade:	4817      	ldr	r0, [pc, #92]	; (30009b3c <CPU_ClkGet+0x88>)
30009ae0:	bd70      	pop	{r4, r5, r6, pc}
30009ae2:	4b17      	ldr	r3, [pc, #92]	; (30009b40 <CPU_ClkGet+0x8c>)
30009ae4:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009ae8:	4718      	bx	r3
30009aea:	2801      	cmp	r0, #1
30009aec:	d011      	beq.n	30009b12 <CPU_ClkGet+0x5e>
30009aee:	05f3      	lsls	r3, r6, #23
30009af0:	d51c      	bpl.n	30009b2c <CPU_ClkGet+0x78>
30009af2:	4914      	ldr	r1, [pc, #80]	; (30009b44 <CPU_ClkGet+0x90>)
30009af4:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30009af8:	6c48      	ldr	r0, [r1, #68]	; 0x44
30009afa:	4a13      	ldr	r2, [pc, #76]	; (30009b48 <CPU_ClkGet+0x94>)
30009afc:	f003 0307 	and.w	r3, r3, #7
30009b00:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009b04:	3301      	adds	r3, #1
30009b06:	3002      	adds	r0, #2
30009b08:	fb02 f000 	mul.w	r0, r2, r0
30009b0c:	fbb0 f0f3 	udiv	r0, r0, r3
30009b10:	bd70      	pop	{r4, r5, r6, pc}
30009b12:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009b16:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30009b1a:	f3c3 4007 	ubfx	r0, r3, #16, #8
30009b1e:	d003      	beq.n	30009b28 <CPU_ClkGet+0x74>
30009b20:	4b0a      	ldr	r3, [pc, #40]	; (30009b4c <CPU_ClkGet+0x98>)
30009b22:	fb03 f000 	mul.w	r0, r3, r0
30009b26:	bd70      	pop	{r4, r5, r6, pc}
30009b28:	4809      	ldr	r0, [pc, #36]	; (30009b50 <CPU_ClkGet+0x9c>)
30009b2a:	bd70      	pop	{r4, r5, r6, pc}
30009b2c:	4806      	ldr	r0, [pc, #24]	; (30009b48 <CPU_ClkGet+0x94>)
30009b2e:	bd70      	pop	{r4, r5, r6, pc}
30009b30:	42008000 	.word	0x42008000
30009b34:	0000c0f9 	.word	0x0000c0f9
30009b38:	003d0900 	.word	0x003d0900
30009b3c:	01312d00 	.word	0x01312d00
30009b40:	000099f5 	.word	0x000099f5
30009b44:	42008800 	.word	0x42008800
30009b48:	02625a00 	.word	0x02625a00
30009b4c:	000f4240 	.word	0x000f4240
30009b50:	02faf080 	.word	0x02faf080

30009b54 <RSIP_IV_Set>:
30009b54:	4b05      	ldr	r3, [pc, #20]	; (30009b6c <RSIP_IV_Set+0x18>)
30009b56:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30009b5a:	680b      	ldr	r3, [r1, #0]
30009b5c:	ba1b      	rev	r3, r3
30009b5e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009b62:	684b      	ldr	r3, [r1, #4]
30009b64:	ba1b      	rev	r3, r3
30009b66:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009b6a:	4770      	bx	lr
30009b6c:	42008c00 	.word	0x42008c00

30009b70 <RSIP_OTF_Cmd>:
30009b70:	4a04      	ldr	r2, [pc, #16]	; (30009b84 <RSIP_OTF_Cmd+0x14>)
30009b72:	2801      	cmp	r0, #1
30009b74:	6953      	ldr	r3, [r2, #20]
30009b76:	bf0c      	ite	eq
30009b78:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009b7c:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009b80:	6153      	str	r3, [r2, #20]
30009b82:	4770      	bx	lr
30009b84:	42008c00 	.word	0x42008c00

30009b88 <RSIP_OTF_Enable>:
30009b88:	b570      	push	{r4, r5, r6, lr}
30009b8a:	4604      	mov	r4, r0
30009b8c:	b1f3      	cbz	r3, 30009bcc <RSIP_OTF_Enable+0x44>
30009b8e:	9b04      	ldr	r3, [sp, #16]
30009b90:	460d      	mov	r5, r1
30009b92:	4616      	mov	r6, r2
30009b94:	2b07      	cmp	r3, #7
30009b96:	d827      	bhi.n	30009be8 <RSIP_OTF_Enable+0x60>
30009b98:	9b05      	ldr	r3, [sp, #20]
30009b9a:	3b01      	subs	r3, #1
30009b9c:	2b01      	cmp	r3, #1
30009b9e:	d81d      	bhi.n	30009bdc <RSIP_OTF_Enable+0x54>
30009ba0:	9a04      	ldr	r2, [sp, #16]
30009ba2:	9b05      	ldr	r3, [sp, #20]
30009ba4:	f002 0007 	and.w	r0, r2, #7
30009ba8:	005b      	lsls	r3, r3, #1
30009baa:	4305      	orrs	r5, r0
30009bac:	4813      	ldr	r0, [pc, #76]	; (30009bfc <RSIP_OTF_Enable+0x74>)
30009bae:	f003 0306 	and.w	r3, r3, #6
30009bb2:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009bb6:	ea43 0206 	orr.w	r2, r3, r6
30009bba:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009bbc:	f042 0201 	orr.w	r2, r2, #1
30009bc0:	f023 0301 	bic.w	r3, r3, #1
30009bc4:	6463      	str	r3, [r4, #68]	; 0x44
30009bc6:	6425      	str	r5, [r4, #64]	; 0x40
30009bc8:	6462      	str	r2, [r4, #68]	; 0x44
30009bca:	bd70      	pop	{r4, r5, r6, pc}
30009bcc:	480b      	ldr	r0, [pc, #44]	; (30009bfc <RSIP_OTF_Enable+0x74>)
30009bce:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009bd2:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009bd4:	f023 0301 	bic.w	r3, r3, #1
30009bd8:	6463      	str	r3, [r4, #68]	; 0x44
30009bda:	bd70      	pop	{r4, r5, r6, pc}
30009bdc:	f240 2131 	movw	r1, #561	; 0x231
30009be0:	4807      	ldr	r0, [pc, #28]	; (30009c00 <RSIP_OTF_Enable+0x78>)
30009be2:	f002 fd91 	bl	3000c708 <__io_assert_failed_veneer>
30009be6:	e7db      	b.n	30009ba0 <RSIP_OTF_Enable+0x18>
30009be8:	f44f 710c 	mov.w	r1, #560	; 0x230
30009bec:	4804      	ldr	r0, [pc, #16]	; (30009c00 <RSIP_OTF_Enable+0x78>)
30009bee:	f002 fd8b 	bl	3000c708 <__io_assert_failed_veneer>
30009bf2:	9b05      	ldr	r3, [sp, #20]
30009bf4:	3b01      	subs	r3, #1
30009bf6:	2b01      	cmp	r3, #1
30009bf8:	d9d2      	bls.n	30009ba0 <RSIP_OTF_Enable+0x18>
30009bfa:	e7ef      	b.n	30009bdc <RSIP_OTF_Enable+0x54>
30009bfc:	42008c00 	.word	0x42008c00
30009c00:	3000db2c 	.word	0x3000db2c

30009c04 <RSIP_MMU_Config>:
30009c04:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009c08:	311f      	adds	r1, #31
30009c0a:	321f      	adds	r2, #31
30009c0c:	331f      	adds	r3, #31
30009c0e:	0080      	lsls	r0, r0, #2
30009c10:	0949      	lsrs	r1, r1, #5
30009c12:	0952      	lsrs	r2, r2, #5
30009c14:	095b      	lsrs	r3, r3, #5
30009c16:	0249      	lsls	r1, r1, #9
30009c18:	0252      	lsls	r2, r2, #9
30009c1a:	025b      	lsls	r3, r3, #9
30009c1c:	b410      	push	{r4}
30009c1e:	4c08      	ldr	r4, [pc, #32]	; (30009c40 <RSIP_MMU_Config+0x3c>)
30009c20:	4404      	add	r4, r0
30009c22:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009c26:	f020 0001 	bic.w	r0, r0, #1
30009c2a:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
30009c2e:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30009c32:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009c36:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009c3a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c3e:	4770      	bx	lr
30009c40:	42008c00 	.word	0x42008c00

30009c44 <RSIP_MMU_Cmd>:
30009c44:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009c48:	4b07      	ldr	r3, [pc, #28]	; (30009c68 <RSIP_MMU_Cmd+0x24>)
30009c4a:	0080      	lsls	r0, r0, #2
30009c4c:	4403      	add	r3, r0
30009c4e:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30009c52:	b121      	cbz	r1, 30009c5e <RSIP_MMU_Cmd+0x1a>
30009c54:	f042 0201 	orr.w	r2, r2, #1
30009c58:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009c5c:	4770      	bx	lr
30009c5e:	f022 0201 	bic.w	r2, r2, #1
30009c62:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009c66:	4770      	bx	lr
30009c68:	42008c00 	.word	0x42008c00

30009c6c <RSIP_MMU_Cache_Clean>:
30009c6c:	4a02      	ldr	r2, [pc, #8]	; (30009c78 <RSIP_MMU_Cache_Clean+0xc>)
30009c6e:	6993      	ldr	r3, [r2, #24]
30009c70:	f043 0304 	orr.w	r3, r3, #4
30009c74:	6193      	str	r3, [r2, #24]
30009c76:	4770      	bx	lr
30009c78:	42008c00 	.word	0x42008c00

30009c7c <NAND_RxData>:
30009c7c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009c80:	460c      	mov	r4, r1
30009c82:	4616      	mov	r6, r2
30009c84:	461f      	mov	r7, r3
30009c86:	2800      	cmp	r0, #0
30009c88:	f000 808a 	beq.w	30009da0 <NAND_RxData+0x124>
30009c8c:	4605      	mov	r5, r0
30009c8e:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009dac <NAND_RxData+0x130>
30009c92:	2000      	movs	r0, #0
30009c94:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009db0 <NAND_RxData+0x134>
30009c98:	47c0      	blx	r8
30009c9a:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30009c9e:	2000      	movs	r0, #0
30009ca0:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
30009ca4:	6819      	ldr	r1, [r3, #0]
30009ca6:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30009caa:	6019      	str	r1, [r3, #0]
30009cac:	6819      	ldr	r1, [r3, #0]
30009cae:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30009cb2:	6019      	str	r1, [r3, #0]
30009cb4:	605a      	str	r2, [r3, #4]
30009cb6:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009cba:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009cbe:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30009cc2:	ea4f 420c 	mov.w	r2, ip, lsl #16
30009cc6:	f1bc 0f03 	cmp.w	ip, #3
30009cca:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009cce:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009cd2:	ea42 0201 	orr.w	r2, r2, r1
30009cd6:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009cda:	f04f 0201 	mov.w	r2, #1
30009cde:	611a      	str	r2, [r3, #16]
30009ce0:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30009ce4:	bf08      	it	eq
30009ce6:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009cea:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009cee:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30009cf2:	b2e4      	uxtb	r4, r4
30009cf4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009cf8:	2301      	movs	r3, #1
30009cfa:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009cfe:	f017 0403 	ands.w	r4, r7, #3
30009d02:	6083      	str	r3, [r0, #8]
30009d04:	d137      	bne.n	30009d76 <NAND_RxData+0xfa>
30009d06:	463b      	mov	r3, r7
30009d08:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009d0c:	42b4      	cmp	r4, r6
30009d0e:	d216      	bcs.n	30009d3e <NAND_RxData+0xc2>
30009d10:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30009d14:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009d18:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009d1c:	d00c      	beq.n	30009d38 <NAND_RxData+0xbc>
30009d1e:	00ad      	lsls	r5, r5, #2
30009d20:	1f19      	subs	r1, r3, #4
30009d22:	f1a5 0c04 	sub.w	ip, r5, #4
30009d26:	449c      	add	ip, r3
30009d28:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009d2c:	f841 0f04 	str.w	r0, [r1, #4]!
30009d30:	458c      	cmp	ip, r1
30009d32:	d1f9      	bne.n	30009d28 <NAND_RxData+0xac>
30009d34:	442b      	add	r3, r5
30009d36:	442c      	add	r4, r5
30009d38:	1b32      	subs	r2, r6, r4
30009d3a:	2a03      	cmp	r2, #3
30009d3c:	d8e6      	bhi.n	30009d0c <NAND_RxData+0x90>
30009d3e:	1e63      	subs	r3, r4, #1
30009d40:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009d44:	441f      	add	r7, r3
30009d46:	e002      	b.n	30009d4e <NAND_RxData+0xd2>
30009d48:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009d4a:	071b      	lsls	r3, r3, #28
30009d4c:	d40d      	bmi.n	30009d6a <NAND_RxData+0xee>
30009d4e:	42b4      	cmp	r4, r6
30009d50:	d3fa      	bcc.n	30009d48 <NAND_RxData+0xcc>
30009d52:	2004      	movs	r0, #4
30009d54:	47c0      	blx	r8
30009d56:	2000      	movs	r0, #0
30009d58:	47c0      	blx	r8
30009d5a:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009d5e:	6813      	ldr	r3, [r2, #0]
30009d60:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009d64:	6013      	str	r3, [r2, #0]
30009d66:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009d6a:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009d6e:	3401      	adds	r4, #1
30009d70:	f807 3f01 	strb.w	r3, [r7, #1]!
30009d74:	e7eb      	b.n	30009d4e <NAND_RxData+0xd2>
30009d76:	f1c4 0404 	rsb	r4, r4, #4
30009d7a:	2200      	movs	r2, #0
30009d7c:	4639      	mov	r1, r7
30009d7e:	e00a      	b.n	30009d96 <NAND_RxData+0x11a>
30009d80:	6a85      	ldr	r5, [r0, #40]	; 0x28
30009d82:	072d      	lsls	r5, r5, #28
30009d84:	d505      	bpl.n	30009d92 <NAND_RxData+0x116>
30009d86:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009d8a:	3201      	adds	r2, #1
30009d8c:	700b      	strb	r3, [r1, #0]
30009d8e:	18b9      	adds	r1, r7, r2
30009d90:	460b      	mov	r3, r1
30009d92:	4294      	cmp	r4, r2
30009d94:	d0b8      	beq.n	30009d08 <NAND_RxData+0x8c>
30009d96:	42b2      	cmp	r2, r6
30009d98:	460b      	mov	r3, r1
30009d9a:	d3f1      	bcc.n	30009d80 <NAND_RxData+0x104>
30009d9c:	4614      	mov	r4, r2
30009d9e:	e7ce      	b.n	30009d3e <NAND_RxData+0xc2>
30009da0:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009dac <NAND_RxData+0x130>
30009da4:	f899 5008 	ldrb.w	r5, [r9, #8]
30009da8:	e773      	b.n	30009c92 <NAND_RxData+0x16>
30009daa:	bf00      	nop
30009dac:	2001c01c 	.word	0x2001c01c
30009db0:	0000b9c1 	.word	0x0000b9c1

30009db4 <NAND_Page_Read>:
30009db4:	b530      	push	{r4, r5, lr}
30009db6:	b085      	sub	sp, #20
30009db8:	4c07      	ldr	r4, [pc, #28]	; (30009dd8 <NAND_Page_Read+0x24>)
30009dba:	4d08      	ldr	r5, [pc, #32]	; (30009ddc <NAND_Page_Read+0x28>)
30009dbc:	9301      	str	r3, [sp, #4]
30009dbe:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009dc2:	47a0      	blx	r4
30009dc4:	4604      	mov	r4, r0
30009dc6:	9903      	ldr	r1, [sp, #12]
30009dc8:	7a28      	ldrb	r0, [r5, #8]
30009dca:	4d05      	ldr	r5, [pc, #20]	; (30009de0 <NAND_Page_Read+0x2c>)
30009dcc:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009dd0:	47a8      	blx	r5
30009dd2:	4620      	mov	r0, r4
30009dd4:	b005      	add	sp, #20
30009dd6:	bd30      	pop	{r4, r5, pc}
30009dd8:	0000bd85 	.word	0x0000bd85
30009ddc:	2001c01c 	.word	0x2001c01c
30009de0:	30009c7d 	.word	0x30009c7d

30009de4 <irq_register>:
30009de4:	b530      	push	{r4, r5, lr}
30009de6:	461c      	mov	r4, r3
30009de8:	460d      	mov	r5, r1
30009dea:	f040 0001 	orr.w	r0, r0, #1
30009dee:	4916      	ldr	r1, [pc, #88]	; (30009e48 <irq_register+0x64>)
30009df0:	4b16      	ldr	r3, [pc, #88]	; (30009e4c <irq_register+0x68>)
30009df2:	2c07      	cmp	r4, #7
30009df4:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
30009df8:	b085      	sub	sp, #20
30009dfa:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
30009dfe:	d813      	bhi.n	30009e28 <irq_register+0x44>
30009e00:	2c07      	cmp	r4, #7
30009e02:	bf28      	it	cs
30009e04:	2407      	movcs	r4, #7
30009e06:	2d00      	cmp	r5, #0
30009e08:	ea4f 1444 	mov.w	r4, r4, lsl #5
30009e0c:	b2e4      	uxtb	r4, r4
30009e0e:	db04      	blt.n	30009e1a <irq_register+0x36>
30009e10:	4b0f      	ldr	r3, [pc, #60]	; (30009e50 <irq_register+0x6c>)
30009e12:	2001      	movs	r0, #1
30009e14:	555c      	strb	r4, [r3, r5]
30009e16:	b005      	add	sp, #20
30009e18:	bd30      	pop	{r4, r5, pc}
30009e1a:	f005 050f 	and.w	r5, r5, #15
30009e1e:	4b0d      	ldr	r3, [pc, #52]	; (30009e54 <irq_register+0x70>)
30009e20:	2001      	movs	r0, #1
30009e22:	555c      	strb	r4, [r3, r5]
30009e24:	b005      	add	sp, #20
30009e26:	bd30      	pop	{r4, r5, pc}
30009e28:	4b0b      	ldr	r3, [pc, #44]	; (30009e58 <irq_register+0x74>)
30009e2a:	2107      	movs	r1, #7
30009e2c:	4a0b      	ldr	r2, [pc, #44]	; (30009e5c <irq_register+0x78>)
30009e2e:	2003      	movs	r0, #3
30009e30:	9500      	str	r5, [sp, #0]
30009e32:	e9cd 4101 	strd	r4, r1, [sp, #4]
30009e36:	4619      	mov	r1, r3
30009e38:	f002 f982 	bl	3000c140 <rtk_log_write>
30009e3c:	f240 315b 	movw	r1, #859	; 0x35b
30009e40:	4807      	ldr	r0, [pc, #28]	; (30009e60 <irq_register+0x7c>)
30009e42:	f002 fc61 	bl	3000c708 <__io_assert_failed_veneer>
30009e46:	e7db      	b.n	30009e00 <irq_register+0x1c>
30009e48:	30000184 	.word	0x30000184
30009e4c:	300002c4 	.word	0x300002c4
30009e50:	e000e400 	.word	0xe000e400
30009e54:	e000ed14 	.word	0xe000ed14
30009e58:	3000dabc 	.word	0x3000dabc
30009e5c:	3000daf0 	.word	0x3000daf0
30009e60:	3000db18 	.word	0x3000db18

30009e64 <SYSCFG_BootFromNor>:
30009e64:	4b0e      	ldr	r3, [pc, #56]	; (30009ea0 <SYSCFG_BootFromNor+0x3c>)
30009e66:	b510      	push	{r4, lr}
30009e68:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009e6c:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009e70:	0392      	lsls	r2, r2, #14
30009e72:	d50a      	bpl.n	30009e8a <SYSCFG_BootFromNor+0x26>
30009e74:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009e78:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009e7c:	d105      	bne.n	30009e8a <SYSCFG_BootFromNor+0x26>
30009e7e:	4b09      	ldr	r3, [pc, #36]	; (30009ea4 <SYSCFG_BootFromNor+0x40>)
30009e80:	4798      	blx	r3
30009e82:	3800      	subs	r0, #0
30009e84:	bf18      	it	ne
30009e86:	2001      	movne	r0, #1
30009e88:	bd10      	pop	{r4, pc}
30009e8a:	4c07      	ldr	r4, [pc, #28]	; (30009ea8 <SYSCFG_BootFromNor+0x44>)
30009e8c:	47a0      	blx	r4
30009e8e:	2802      	cmp	r0, #2
30009e90:	d004      	beq.n	30009e9c <SYSCFG_BootFromNor+0x38>
30009e92:	47a0      	blx	r4
30009e94:	2801      	cmp	r0, #1
30009e96:	d1f2      	bne.n	30009e7e <SYSCFG_BootFromNor+0x1a>
30009e98:	2000      	movs	r0, #0
30009e9a:	bd10      	pop	{r4, pc}
30009e9c:	2001      	movs	r0, #1
30009e9e:	bd10      	pop	{r4, pc}
30009ea0:	42008000 	.word	0x42008000
30009ea4:	0000c0bd 	.word	0x0000c0bd
30009ea8:	0000c1a9 	.word	0x0000c1a9

30009eac <SYSCFG_RLVersion>:
30009eac:	4b07      	ldr	r3, [pc, #28]	; (30009ecc <SYSCFG_RLVersion+0x20>)
30009eae:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009eb2:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009eb6:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009eba:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009ebe:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009ec2:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009ec6:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009eca:	4770      	bx	lr
30009ecc:	42008000 	.word	0x42008000

30009ed0 <SBOOT_GetMdType>:
30009ed0:	b470      	push	{r4, r5, r6}
30009ed2:	4c0c      	ldr	r4, [pc, #48]	; (30009f04 <SBOOT_GetMdType+0x34>)
30009ed4:	4e0c      	ldr	r6, [pc, #48]	; (30009f08 <SBOOT_GetMdType+0x38>)
30009ed6:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009eda:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009ede:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009ee2:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009ee6:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009eea:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009eee:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009ef2:	6033      	str	r3, [r6, #0]
30009ef4:	b913      	cbnz	r3, 30009efc <SBOOT_GetMdType+0x2c>
30009ef6:	bc70      	pop	{r4, r5, r6}
30009ef8:	f002 bc36 	b.w	3000c768 <__SBOOT_GetMdType_A_veneer>
30009efc:	bc70      	pop	{r4, r5, r6}
30009efe:	f002 bc23 	b.w	3000c748 <__SBOOT_GetMdType_B_veneer>
30009f02:	bf00      	nop
30009f04:	42008000 	.word	0x42008000
30009f08:	3000ff94 	.word	0x3000ff94

30009f0c <CRYPTO_OTPKey_SHA_Init>:
30009f0c:	4a0d      	ldr	r2, [pc, #52]	; (30009f44 <CRYPTO_OTPKey_SHA_Init+0x38>)
30009f0e:	b410      	push	{r4}
30009f10:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009f14:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009f18:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009f1c:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009f20:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009f24:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009f28:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009f2c:	4a06      	ldr	r2, [pc, #24]	; (30009f48 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009f2e:	6013      	str	r3, [r2, #0]
30009f30:	b91b      	cbnz	r3, 30009f3a <CRYPTO_OTPKey_SHA_Init+0x2e>
30009f32:	f85d 4b04 	ldr.w	r4, [sp], #4
30009f36:	f002 bb5f 	b.w	3000c5f8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009f3a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009f3e:	f002 bb53 	b.w	3000c5e8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30009f42:	bf00      	nop
30009f44:	42008000 	.word	0x42008000
30009f48:	3000ff94 	.word	0x3000ff94

30009f4c <rtl_crypto_hmac_sha2_update>:
30009f4c:	b470      	push	{r4, r5, r6}
30009f4e:	4c0c      	ldr	r4, [pc, #48]	; (30009f80 <rtl_crypto_hmac_sha2_update+0x34>)
30009f50:	4e0c      	ldr	r6, [pc, #48]	; (30009f84 <rtl_crypto_hmac_sha2_update+0x38>)
30009f52:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009f56:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009f5a:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009f5e:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009f62:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009f66:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009f6a:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009f6e:	6033      	str	r3, [r6, #0]
30009f70:	b913      	cbnz	r3, 30009f78 <rtl_crypto_hmac_sha2_update+0x2c>
30009f72:	bc70      	pop	{r4, r5, r6}
30009f74:	f002 bc00 	b.w	3000c778 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009f78:	bc70      	pop	{r4, r5, r6}
30009f7a:	f002 bb1d 	b.w	3000c5b8 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009f7e:	bf00      	nop
30009f80:	42008000 	.word	0x42008000
30009f84:	3000ff94 	.word	0x3000ff94

30009f88 <rtl_crypto_hmac_sha2_final>:
30009f88:	4a0d      	ldr	r2, [pc, #52]	; (30009fc0 <rtl_crypto_hmac_sha2_final+0x38>)
30009f8a:	b410      	push	{r4}
30009f8c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009f90:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009f94:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009f98:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009f9c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009fa0:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009fa4:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009fa8:	4a06      	ldr	r2, [pc, #24]	; (30009fc4 <rtl_crypto_hmac_sha2_final+0x3c>)
30009faa:	6013      	str	r3, [r2, #0]
30009fac:	b91b      	cbnz	r3, 30009fb6 <rtl_crypto_hmac_sha2_final+0x2e>
30009fae:	f85d 4b04 	ldr.w	r4, [sp], #4
30009fb2:	f002 bb31 	b.w	3000c618 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009fb6:	f85d 4b04 	ldr.w	r4, [sp], #4
30009fba:	f002 bbb5 	b.w	3000c728 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009fbe:	bf00      	nop
30009fc0:	42008000 	.word	0x42008000
30009fc4:	3000ff94 	.word	0x3000ff94

30009fc8 <rtl_crypto_sha2_init>:
30009fc8:	4a0d      	ldr	r2, [pc, #52]	; (3000a000 <rtl_crypto_sha2_init+0x38>)
30009fca:	b410      	push	{r4}
30009fcc:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009fd0:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009fd4:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009fd8:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009fdc:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009fe0:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009fe4:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009fe8:	4a06      	ldr	r2, [pc, #24]	; (3000a004 <rtl_crypto_sha2_init+0x3c>)
30009fea:	6013      	str	r3, [r2, #0]
30009fec:	b91b      	cbnz	r3, 30009ff6 <rtl_crypto_sha2_init+0x2e>
30009fee:	f85d 4b04 	ldr.w	r4, [sp], #4
30009ff2:	f002 bb59 	b.w	3000c6a8 <__rtl_crypto_sha2_init_A_veneer>
30009ff6:	f85d 4b04 	ldr.w	r4, [sp], #4
30009ffa:	f002 bb4d 	b.w	3000c698 <__rtl_crypto_sha2_init_B_veneer>
30009ffe:	bf00      	nop
3000a000:	42008000 	.word	0x42008000
3000a004:	3000ff94 	.word	0x3000ff94

3000a008 <rtl_crypto_sha2_update>:
3000a008:	b470      	push	{r4, r5, r6}
3000a00a:	4c0c      	ldr	r4, [pc, #48]	; (3000a03c <rtl_crypto_sha2_update+0x34>)
3000a00c:	4e0c      	ldr	r6, [pc, #48]	; (3000a040 <rtl_crypto_sha2_update+0x38>)
3000a00e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000a012:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000a016:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000a01a:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000a01e:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000a022:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000a026:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a02a:	6033      	str	r3, [r6, #0]
3000a02c:	b913      	cbnz	r3, 3000a034 <rtl_crypto_sha2_update+0x2c>
3000a02e:	bc70      	pop	{r4, r5, r6}
3000a030:	f002 bb92 	b.w	3000c758 <__rtl_crypto_sha2_update_A_veneer>
3000a034:	bc70      	pop	{r4, r5, r6}
3000a036:	f002 bb17 	b.w	3000c668 <__rtl_crypto_sha2_update_B_veneer>
3000a03a:	bf00      	nop
3000a03c:	42008000 	.word	0x42008000
3000a040:	3000ff94 	.word	0x3000ff94

3000a044 <rtl_crypto_sha2_final>:
3000a044:	4a0d      	ldr	r2, [pc, #52]	; (3000a07c <rtl_crypto_sha2_final+0x38>)
3000a046:	b410      	push	{r4}
3000a048:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
3000a04c:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
3000a050:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
3000a054:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
3000a058:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
3000a05c:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
3000a060:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a064:	4a06      	ldr	r2, [pc, #24]	; (3000a080 <rtl_crypto_sha2_final+0x3c>)
3000a066:	6013      	str	r3, [r2, #0]
3000a068:	b91b      	cbnz	r3, 3000a072 <rtl_crypto_sha2_final+0x2e>
3000a06a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a06e:	f002 bb03 	b.w	3000c678 <__rtl_crypto_sha2_final_A_veneer>
3000a072:	f85d 4b04 	ldr.w	r4, [sp], #4
3000a076:	f002 bb4f 	b.w	3000c718 <__rtl_crypto_sha2_final_B_veneer>
3000a07a:	bf00      	nop
3000a07c:	42008000 	.word	0x42008000
3000a080:	3000ff94 	.word	0x3000ff94

3000a084 <SBOOT_Validate_PubKey>:
3000a084:	b470      	push	{r4, r5, r6}
3000a086:	4c0c      	ldr	r4, [pc, #48]	; (3000a0b8 <SBOOT_Validate_PubKey+0x34>)
3000a088:	4e0c      	ldr	r6, [pc, #48]	; (3000a0bc <SBOOT_Validate_PubKey+0x38>)
3000a08a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
3000a08e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
3000a092:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
3000a096:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
3000a09a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
3000a09e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
3000a0a2:	f3c3 4303 	ubfx	r3, r3, #16, #4
3000a0a6:	6033      	str	r3, [r6, #0]
3000a0a8:	b913      	cbnz	r3, 3000a0b0 <SBOOT_Validate_PubKey+0x2c>
3000a0aa:	bc70      	pop	{r4, r5, r6}
3000a0ac:	f002 bb1c 	b.w	3000c6e8 <__SBOOT_Validate_PubKey_A_veneer>
3000a0b0:	bc70      	pop	{r4, r5, r6}
3000a0b2:	f002 ba91 	b.w	3000c5d8 <__SBOOT_Validate_PubKey_B_veneer>
3000a0b6:	bf00      	nop
3000a0b8:	42008000 	.word	0x42008000
3000a0bc:	3000ff94 	.word	0x3000ff94

3000a0c0 <SBOOT_Validate_Signature>:
3000a0c0:	b5f0      	push	{r4, r5, r6, r7, lr}
3000a0c2:	4d10      	ldr	r5, [pc, #64]	; (3000a104 <SBOOT_Validate_Signature+0x44>)
3000a0c4:	9f05      	ldr	r7, [sp, #20]
3000a0c6:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a0ca:	f8dd c018 	ldr.w	ip, [sp, #24]
3000a0ce:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a0d2:	f8df e034 	ldr.w	lr, [pc, #52]	; 3000a108 <SBOOT_Validate_Signature+0x48>
3000a0d6:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a0da:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a0de:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a0e2:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a0e6:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a0ea:	e9cd 7c05 	strd	r7, ip, [sp, #20]
3000a0ee:	f8ce 4000 	str.w	r4, [lr]
3000a0f2:	b91c      	cbnz	r4, 3000a0fc <SBOOT_Validate_Signature+0x3c>
3000a0f4:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
3000a0f8:	f002 bade 	b.w	3000c6b8 <__SBOOT_Validate_Signature_A_veneer>
3000a0fc:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
3000a100:	f002 bb52 	b.w	3000c7a8 <__SBOOT_Validate_Signature_B_veneer>
3000a104:	42008000 	.word	0x42008000
3000a108:	3000ff94 	.word	0x3000ff94

3000a10c <SBOOT_Validate_ImgHash>:
3000a10c:	b4f0      	push	{r4, r5, r6, r7}
3000a10e:	4d0c      	ldr	r5, [pc, #48]	; (3000a140 <SBOOT_Validate_ImgHash+0x34>)
3000a110:	4f0c      	ldr	r7, [pc, #48]	; (3000a144 <SBOOT_Validate_ImgHash+0x38>)
3000a112:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a116:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a11a:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a11e:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a122:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a126:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a12a:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a12e:	603c      	str	r4, [r7, #0]
3000a130:	b914      	cbnz	r4, 3000a138 <SBOOT_Validate_ImgHash+0x2c>
3000a132:	bcf0      	pop	{r4, r5, r6, r7}
3000a134:	f002 bb00 	b.w	3000c738 <__SBOOT_Validate_ImgHash_A_veneer>
3000a138:	bcf0      	pop	{r4, r5, r6, r7}
3000a13a:	f002 bac5 	b.w	3000c6c8 <__SBOOT_Validate_ImgHash_B_veneer>
3000a13e:	bf00      	nop
3000a140:	42008000 	.word	0x42008000
3000a144:	3000ff94 	.word	0x3000ff94

3000a148 <CRYPTO_SHA_Init>:
3000a148:	4b0b      	ldr	r3, [pc, #44]	; (3000a178 <CRYPTO_SHA_Init+0x30>)
3000a14a:	490c      	ldr	r1, [pc, #48]	; (3000a17c <CRYPTO_SHA_Init+0x34>)
3000a14c:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
3000a150:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
3000a154:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
3000a158:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
3000a15c:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
3000a160:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
3000a164:	f3c0 4003 	ubfx	r0, r0, #16, #4
3000a168:	6008      	str	r0, [r1, #0]
3000a16a:	b908      	cbnz	r0, 3000a170 <CRYPTO_SHA_Init+0x28>
3000a16c:	f002 bb14 	b.w	3000c798 <__CRYPTO_SHA_Init_A_veneer>
3000a170:	2000      	movs	r0, #0
3000a172:	f002 ba69 	b.w	3000c648 <__CRYPTO_SHA_Init_B_veneer>
3000a176:	bf00      	nop
3000a178:	42008000 	.word	0x42008000
3000a17c:	3000ff94 	.word	0x3000ff94

3000a180 <SBOOT_Validate_Algorithm>:
3000a180:	b4f0      	push	{r4, r5, r6, r7}
3000a182:	4d0c      	ldr	r5, [pc, #48]	; (3000a1b4 <SBOOT_Validate_Algorithm+0x34>)
3000a184:	4f0c      	ldr	r7, [pc, #48]	; (3000a1b8 <SBOOT_Validate_Algorithm+0x38>)
3000a186:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
3000a18a:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
3000a18e:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
3000a192:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
3000a196:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
3000a19a:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
3000a19e:	f3c4 4403 	ubfx	r4, r4, #16, #4
3000a1a2:	603c      	str	r4, [r7, #0]
3000a1a4:	b914      	cbnz	r4, 3000a1ac <SBOOT_Validate_Algorithm+0x2c>
3000a1a6:	bcf0      	pop	{r4, r5, r6, r7}
3000a1a8:	f002 baa6 	b.w	3000c6f8 <__SBOOT_Validate_Algorithm_A_veneer>
3000a1ac:	bcf0      	pop	{r4, r5, r6, r7}
3000a1ae:	f002 baeb 	b.w	3000c788 <__SBOOT_Validate_Algorithm_B_veneer>
3000a1b2:	bf00      	nop
3000a1b4:	42008000 	.word	0x42008000
3000a1b8:	3000ff94 	.word	0x3000ff94

3000a1bc <ddr_init_index>:
3000a1bc:	b570      	push	{r4, r5, r6, lr}
3000a1be:	4b0f      	ldr	r3, [pc, #60]	; (3000a1fc <ddr_init_index+0x40>)
3000a1c0:	4798      	blx	r3
3000a1c2:	4604      	mov	r4, r0
3000a1c4:	4b0e      	ldr	r3, [pc, #56]	; (3000a200 <ddr_init_index+0x44>)
3000a1c6:	4798      	blx	r3
3000a1c8:	2c03      	cmp	r4, #3
3000a1ca:	d014      	beq.n	3000a1f6 <ddr_init_index+0x3a>
3000a1cc:	2c01      	cmp	r4, #1
3000a1ce:	d00c      	beq.n	3000a1ea <ddr_init_index+0x2e>
3000a1d0:	4e0c      	ldr	r6, [pc, #48]	; (3000a204 <ddr_init_index+0x48>)
3000a1d2:	4d0d      	ldr	r5, [pc, #52]	; (3000a208 <ddr_init_index+0x4c>)
3000a1d4:	4c0d      	ldr	r4, [pc, #52]	; (3000a20c <ddr_init_index+0x50>)
3000a1d6:	462b      	mov	r3, r5
3000a1d8:	4622      	mov	r2, r4
3000a1da:	490b      	ldr	r1, [pc, #44]	; (3000a208 <ddr_init_index+0x4c>)
3000a1dc:	2002      	movs	r0, #2
3000a1de:	f001 ffaf 	bl	3000c140 <rtk_log_write>
3000a1e2:	f242 7010 	movw	r0, #10000	; 0x2710
3000a1e6:	47b0      	blx	r6
3000a1e8:	e7f5      	b.n	3000a1d6 <ddr_init_index+0x1a>
3000a1ea:	2802      	cmp	r0, #2
3000a1ec:	d003      	beq.n	3000a1f6 <ddr_init_index+0x3a>
3000a1ee:	2803      	cmp	r0, #3
3000a1f0:	d1ee      	bne.n	3000a1d0 <ddr_init_index+0x14>
3000a1f2:	2000      	movs	r0, #0
3000a1f4:	bd70      	pop	{r4, r5, r6, pc}
3000a1f6:	4620      	mov	r0, r4
3000a1f8:	bd70      	pop	{r4, r5, r6, pc}
3000a1fa:	bf00      	nop
3000a1fc:	30008001 	.word	0x30008001
3000a200:	30007e7d 	.word	0x30007e7d
3000a204:	00009be5 	.word	0x00009be5
3000a208:	3000db74 	.word	0x3000db74
3000a20c:	3000db78 	.word	0x3000db78

3000a210 <rxi316_perf_tune>:
3000a210:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a214:	4606      	mov	r6, r0
3000a216:	6800      	ldr	r0, [r0, #0]
3000a218:	b0c1      	sub	sp, #260	; 0x104
3000a21a:	7985      	ldrb	r5, [r0, #6]
3000a21c:	2d00      	cmp	r5, #0
3000a21e:	f000 832c 	beq.w	3000a87a <rxi316_perf_tune+0x66a>
3000a222:	2d01      	cmp	r5, #1
3000a224:	d003      	beq.n	3000a22e <rxi316_perf_tune+0x1e>
3000a226:	2d03      	cmp	r5, #3
3000a228:	bf14      	ite	ne
3000a22a:	2502      	movne	r5, #2
3000a22c:	2500      	moveq	r5, #0
3000a22e:	68f3      	ldr	r3, [r6, #12]
3000a230:	2420      	movs	r4, #32
3000a232:	4684      	mov	ip, r0
3000a234:	6859      	ldr	r1, [r3, #4]
3000a236:	46a3      	mov	fp, r4
3000a238:	46a2      	mov	sl, r4
3000a23a:	46a0      	mov	r8, r4
3000a23c:	784b      	ldrb	r3, [r1, #1]
3000a23e:	46a1      	mov	r9, r4
3000a240:	7bcf      	ldrb	r7, [r1, #15]
3000a242:	9321      	str	r3, [sp, #132]	; 0x84
3000a244:	788b      	ldrb	r3, [r1, #2]
3000a246:	780a      	ldrb	r2, [r1, #0]
3000a248:	9322      	str	r3, [sp, #136]	; 0x88
3000a24a:	78cb      	ldrb	r3, [r1, #3]
3000a24c:	9323      	str	r3, [sp, #140]	; 0x8c
3000a24e:	790b      	ldrb	r3, [r1, #4]
3000a250:	9324      	str	r3, [sp, #144]	; 0x90
3000a252:	794b      	ldrb	r3, [r1, #5]
3000a254:	9325      	str	r3, [sp, #148]	; 0x94
3000a256:	798b      	ldrb	r3, [r1, #6]
3000a258:	9326      	str	r3, [sp, #152]	; 0x98
3000a25a:	79cb      	ldrb	r3, [r1, #7]
3000a25c:	9327      	str	r3, [sp, #156]	; 0x9c
3000a25e:	7a0b      	ldrb	r3, [r1, #8]
3000a260:	9328      	str	r3, [sp, #160]	; 0xa0
3000a262:	7a4b      	ldrb	r3, [r1, #9]
3000a264:	9329      	str	r3, [sp, #164]	; 0xa4
3000a266:	7a8b      	ldrb	r3, [r1, #10]
3000a268:	932a      	str	r3, [sp, #168]	; 0xa8
3000a26a:	7acb      	ldrb	r3, [r1, #11]
3000a26c:	932b      	str	r3, [sp, #172]	; 0xac
3000a26e:	7b0b      	ldrb	r3, [r1, #12]
3000a270:	932c      	str	r3, [sp, #176]	; 0xb0
3000a272:	7b4b      	ldrb	r3, [r1, #13]
3000a274:	932d      	str	r3, [sp, #180]	; 0xb4
3000a276:	7b8b      	ldrb	r3, [r1, #14]
3000a278:	932e      	str	r3, [sp, #184]	; 0xb8
3000a27a:	2300      	movs	r3, #0
3000a27c:	972f      	str	r7, [sp, #188]	; 0xbc
3000a27e:	7c0f      	ldrb	r7, [r1, #16]
3000a280:	941c      	str	r4, [sp, #112]	; 0x70
3000a282:	9730      	str	r7, [sp, #192]	; 0xc0
3000a284:	7c4f      	ldrb	r7, [r1, #17]
3000a286:	9731      	str	r7, [sp, #196]	; 0xc4
3000a288:	7c8f      	ldrb	r7, [r1, #18]
3000a28a:	9732      	str	r7, [sp, #200]	; 0xc8
3000a28c:	7ccf      	ldrb	r7, [r1, #19]
3000a28e:	9733      	str	r7, [sp, #204]	; 0xcc
3000a290:	7d0f      	ldrb	r7, [r1, #20]
3000a292:	9734      	str	r7, [sp, #208]	; 0xd0
3000a294:	7d4f      	ldrb	r7, [r1, #21]
3000a296:	9735      	str	r7, [sp, #212]	; 0xd4
3000a298:	7d8f      	ldrb	r7, [r1, #22]
3000a29a:	9736      	str	r7, [sp, #216]	; 0xd8
3000a29c:	7dcf      	ldrb	r7, [r1, #23]
3000a29e:	9737      	str	r7, [sp, #220]	; 0xdc
3000a2a0:	7e0f      	ldrb	r7, [r1, #24]
3000a2a2:	9738      	str	r7, [sp, #224]	; 0xe0
3000a2a4:	7e4f      	ldrb	r7, [r1, #25]
3000a2a6:	9739      	str	r7, [sp, #228]	; 0xe4
3000a2a8:	7e8f      	ldrb	r7, [r1, #26]
3000a2aa:	973a      	str	r7, [sp, #232]	; 0xe8
3000a2ac:	7ecf      	ldrb	r7, [r1, #27]
3000a2ae:	973b      	str	r7, [sp, #236]	; 0xec
3000a2b0:	7f0f      	ldrb	r7, [r1, #28]
3000a2b2:	973c      	str	r7, [sp, #240]	; 0xf0
3000a2b4:	7f4f      	ldrb	r7, [r1, #29]
3000a2b6:	973d      	str	r7, [sp, #244]	; 0xf4
3000a2b8:	7f8f      	ldrb	r7, [r1, #30]
3000a2ba:	7fc9      	ldrb	r1, [r1, #31]
3000a2bc:	973e      	str	r7, [sp, #248]	; 0xf8
3000a2be:	4627      	mov	r7, r4
3000a2c0:	913f      	str	r1, [sp, #252]	; 0xfc
3000a2c2:	4619      	mov	r1, r3
3000a2c4:	9408      	str	r4, [sp, #32]
3000a2c6:	941b      	str	r4, [sp, #108]	; 0x6c
3000a2c8:	9403      	str	r4, [sp, #12]
3000a2ca:	941a      	str	r4, [sp, #104]	; 0x68
3000a2cc:	9409      	str	r4, [sp, #36]	; 0x24
3000a2ce:	9302      	str	r3, [sp, #8]
3000a2d0:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000a2d4:	e9cd 4400 	strd	r4, r4, [sp]
3000a2d8:	e9cd 4406 	strd	r4, r4, [sp, #24]
3000a2dc:	e9cd 4418 	strd	r4, r4, [sp, #96]	; 0x60
3000a2e0:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
3000a2e4:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
3000a2e8:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
3000a2ec:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
3000a2f0:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000a2f4:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
3000a2f8:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
3000a2fc:	b94a      	cbnz	r2, 3000a312 <rxi316_perf_tune+0x102>
3000a2fe:	3101      	adds	r1, #1
3000a300:	4699      	mov	r9, r3
3000a302:	3301      	adds	r3, #1
3000a304:	2b20      	cmp	r3, #32
3000a306:	d069      	beq.n	3000a3dc <rxi316_perf_tune+0x1cc>
3000a308:	aa20      	add	r2, sp, #128	; 0x80
3000a30a:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a30e:	2a00      	cmp	r2, #0
3000a310:	d0f5      	beq.n	3000a2fe <rxi316_perf_tune+0xee>
3000a312:	2a01      	cmp	r2, #1
3000a314:	d05d      	beq.n	3000a3d2 <rxi316_perf_tune+0x1c2>
3000a316:	2a02      	cmp	r2, #2
3000a318:	d06e      	beq.n	3000a3f8 <rxi316_perf_tune+0x1e8>
3000a31a:	2a03      	cmp	r2, #3
3000a31c:	d06f      	beq.n	3000a3fe <rxi316_perf_tune+0x1ee>
3000a31e:	2a04      	cmp	r2, #4
3000a320:	d070      	beq.n	3000a404 <rxi316_perf_tune+0x1f4>
3000a322:	2a05      	cmp	r2, #5
3000a324:	d071      	beq.n	3000a40a <rxi316_perf_tune+0x1fa>
3000a326:	2a06      	cmp	r2, #6
3000a328:	d072      	beq.n	3000a410 <rxi316_perf_tune+0x200>
3000a32a:	2a07      	cmp	r2, #7
3000a32c:	f000 82a7 	beq.w	3000a87e <rxi316_perf_tune+0x66e>
3000a330:	2a08      	cmp	r2, #8
3000a332:	f000 8364 	beq.w	3000a9fe <rxi316_perf_tune+0x7ee>
3000a336:	2a09      	cmp	r2, #9
3000a338:	f000 8364 	beq.w	3000aa04 <rxi316_perf_tune+0x7f4>
3000a33c:	2a0a      	cmp	r2, #10
3000a33e:	f000 836c 	beq.w	3000aa1a <rxi316_perf_tune+0x80a>
3000a342:	2a0b      	cmp	r2, #11
3000a344:	f000 8364 	beq.w	3000aa10 <rxi316_perf_tune+0x800>
3000a348:	2a0c      	cmp	r2, #12
3000a34a:	f000 836b 	beq.w	3000aa24 <rxi316_perf_tune+0x814>
3000a34e:	2a0d      	cmp	r2, #13
3000a350:	f000 8377 	beq.w	3000aa42 <rxi316_perf_tune+0x832>
3000a354:	2a0e      	cmp	r2, #14
3000a356:	f000 836f 	beq.w	3000aa38 <rxi316_perf_tune+0x828>
3000a35a:	2a0f      	cmp	r2, #15
3000a35c:	f000 8376 	beq.w	3000aa4c <rxi316_perf_tune+0x83c>
3000a360:	2a10      	cmp	r2, #16
3000a362:	f000 8378 	beq.w	3000aa56 <rxi316_perf_tune+0x846>
3000a366:	2a11      	cmp	r2, #17
3000a368:	f000 8377 	beq.w	3000aa5a <rxi316_perf_tune+0x84a>
3000a36c:	2a12      	cmp	r2, #18
3000a36e:	f000 8376 	beq.w	3000aa5e <rxi316_perf_tune+0x84e>
3000a372:	2a13      	cmp	r2, #19
3000a374:	f000 8377 	beq.w	3000aa66 <rxi316_perf_tune+0x856>
3000a378:	2a14      	cmp	r2, #20
3000a37a:	f000 8372 	beq.w	3000aa62 <rxi316_perf_tune+0x852>
3000a37e:	2a15      	cmp	r2, #21
3000a380:	f000 8379 	beq.w	3000aa76 <rxi316_perf_tune+0x866>
3000a384:	2a16      	cmp	r2, #22
3000a386:	f000 8374 	beq.w	3000aa72 <rxi316_perf_tune+0x862>
3000a38a:	2a17      	cmp	r2, #23
3000a38c:	f000 8383 	beq.w	3000aa96 <rxi316_perf_tune+0x886>
3000a390:	2a18      	cmp	r2, #24
3000a392:	f000 837e 	beq.w	3000aa92 <rxi316_perf_tune+0x882>
3000a396:	2a19      	cmp	r2, #25
3000a398:	f000 8379 	beq.w	3000aa8e <rxi316_perf_tune+0x87e>
3000a39c:	2a1a      	cmp	r2, #26
3000a39e:	f000 8374 	beq.w	3000aa8a <rxi316_perf_tune+0x87a>
3000a3a2:	2a1b      	cmp	r2, #27
3000a3a4:	f000 836f 	beq.w	3000aa86 <rxi316_perf_tune+0x876>
3000a3a8:	2a1c      	cmp	r2, #28
3000a3aa:	f000 836a 	beq.w	3000aa82 <rxi316_perf_tune+0x872>
3000a3ae:	2a1d      	cmp	r2, #29
3000a3b0:	f000 8365 	beq.w	3000aa7e <rxi316_perf_tune+0x86e>
3000a3b4:	2a1e      	cmp	r2, #30
3000a3b6:	f000 8360 	beq.w	3000aa7a <rxi316_perf_tune+0x86a>
3000a3ba:	2a1f      	cmp	r2, #31
3000a3bc:	f000 8357 	beq.w	3000aa6e <rxi316_perf_tune+0x85e>
3000a3c0:	2a20      	cmp	r2, #32
3000a3c2:	f000 8352 	beq.w	3000aa6a <rxi316_perf_tune+0x85a>
3000a3c6:	9808      	ldr	r0, [sp, #32]
3000a3c8:	2a21      	cmp	r2, #33	; 0x21
3000a3ca:	bf08      	it	eq
3000a3cc:	4618      	moveq	r0, r3
3000a3ce:	9008      	str	r0, [sp, #32]
3000a3d0:	e797      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a3d2:	4698      	mov	r8, r3
3000a3d4:	3301      	adds	r3, #1
3000a3d6:	3101      	adds	r1, #1
3000a3d8:	2b20      	cmp	r3, #32
3000a3da:	d195      	bne.n	3000a308 <rxi316_perf_tune+0xf8>
3000a3dc:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a3e0:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a3e4:	b1ba      	cbz	r2, 3000a416 <rxi316_perf_tune+0x206>
3000a3e6:	2a01      	cmp	r2, #1
3000a3e8:	f000 830f 	beq.w	3000aa0a <rxi316_perf_tune+0x7fa>
3000a3ec:	2a03      	cmp	r2, #3
3000a3ee:	bf0c      	ite	eq
3000a3f0:	2204      	moveq	r2, #4
3000a3f2:	2203      	movne	r2, #3
3000a3f4:	921d      	str	r2, [sp, #116]	; 0x74
3000a3f6:	e010      	b.n	3000a41a <rxi316_perf_tune+0x20a>
3000a3f8:	3101      	adds	r1, #1
3000a3fa:	461f      	mov	r7, r3
3000a3fc:	e781      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a3fe:	3101      	adds	r1, #1
3000a400:	469a      	mov	sl, r3
3000a402:	e77e      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a404:	3101      	adds	r1, #1
3000a406:	469b      	mov	fp, r3
3000a408:	e77b      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a40a:	3101      	adds	r1, #1
3000a40c:	9300      	str	r3, [sp, #0]
3000a40e:	e778      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a410:	3101      	adds	r1, #1
3000a412:	9301      	str	r3, [sp, #4]
3000a414:	e775      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a416:	2201      	movs	r2, #1
3000a418:	921d      	str	r2, [sp, #116]	; 0x74
3000a41a:	3b02      	subs	r3, #2
3000a41c:	428b      	cmp	r3, r1
3000a41e:	d005      	beq.n	3000a42c <rxi316_perf_tune+0x21c>
3000a420:	4b90      	ldr	r3, [pc, #576]	; (3000a664 <rxi316_perf_tune+0x454>)
3000a422:	2002      	movs	r0, #2
3000a424:	4a90      	ldr	r2, [pc, #576]	; (3000a668 <rxi316_perf_tune+0x458>)
3000a426:	4619      	mov	r1, r3
3000a428:	f001 fe8a 	bl	3000c140 <rtk_log_write>
3000a42c:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a42e:	9a02      	ldr	r2, [sp, #8]
3000a430:	4293      	cmp	r3, r2
3000a432:	d005      	beq.n	3000a440 <rxi316_perf_tune+0x230>
3000a434:	4b8b      	ldr	r3, [pc, #556]	; (3000a664 <rxi316_perf_tune+0x454>)
3000a436:	2002      	movs	r0, #2
3000a438:	4a8c      	ldr	r2, [pc, #560]	; (3000a66c <rxi316_perf_tune+0x45c>)
3000a43a:	4619      	mov	r1, r3
3000a43c:	f001 fe80 	bl	3000c140 <rtk_log_write>
3000a440:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a442:	2b20      	cmp	r3, #32
3000a444:	f000 82d8 	beq.w	3000a9f8 <rxi316_perf_tune+0x7e8>
3000a448:	1b5a      	subs	r2, r3, r5
3000a44a:	3a05      	subs	r2, #5
3000a44c:	f002 030f 	and.w	r3, r2, #15
3000a450:	9302      	str	r3, [sp, #8]
3000a452:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a454:	2b20      	cmp	r3, #32
3000a456:	f000 82cd 	beq.w	3000a9f4 <rxi316_perf_tune+0x7e4>
3000a45a:	1b5a      	subs	r2, r3, r5
3000a45c:	3a06      	subs	r2, #6
3000a45e:	0112      	lsls	r2, r2, #4
3000a460:	b2d2      	uxtb	r2, r2
3000a462:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a464:	2b20      	cmp	r3, #32
3000a466:	f000 82c1 	beq.w	3000a9ec <rxi316_perf_tune+0x7dc>
3000a46a:	1b5b      	subs	r3, r3, r5
3000a46c:	3b07      	subs	r3, #7
3000a46e:	021b      	lsls	r3, r3, #8
3000a470:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a474:	930b      	str	r3, [sp, #44]	; 0x2c
3000a476:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a478:	2b20      	cmp	r3, #32
3000a47a:	f000 8214 	beq.w	3000a8a6 <rxi316_perf_tune+0x696>
3000a47e:	1b5b      	subs	r3, r3, r5
3000a480:	3b08      	subs	r3, #8
3000a482:	031b      	lsls	r3, r3, #12
3000a484:	b29b      	uxth	r3, r3
3000a486:	930a      	str	r3, [sp, #40]	; 0x28
3000a488:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a48a:	2b20      	cmp	r3, #32
3000a48c:	f000 8212 	beq.w	3000a8b4 <rxi316_perf_tune+0x6a4>
3000a490:	1b5b      	subs	r3, r3, r5
3000a492:	3b09      	subs	r3, #9
3000a494:	041b      	lsls	r3, r3, #16
3000a496:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a49a:	9309      	str	r3, [sp, #36]	; 0x24
3000a49c:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a49e:	2b20      	cmp	r3, #32
3000a4a0:	f000 820f 	beq.w	3000a8c2 <rxi316_perf_tune+0x6b2>
3000a4a4:	1b5b      	subs	r3, r3, r5
3000a4a6:	3b0a      	subs	r3, #10
3000a4a8:	051b      	lsls	r3, r3, #20
3000a4aa:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a4ae:	930c      	str	r3, [sp, #48]	; 0x30
3000a4b0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a4b2:	2b20      	cmp	r3, #32
3000a4b4:	f000 820c 	beq.w	3000a8d0 <rxi316_perf_tune+0x6c0>
3000a4b8:	1b5b      	subs	r3, r3, r5
3000a4ba:	3b0b      	subs	r3, #11
3000a4bc:	061b      	lsls	r3, r3, #24
3000a4be:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a4c2:	930d      	str	r3, [sp, #52]	; 0x34
3000a4c4:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a4c6:	2b20      	cmp	r3, #32
3000a4c8:	f000 8209 	beq.w	3000a8de <rxi316_perf_tune+0x6ce>
3000a4cc:	1b5b      	subs	r3, r3, r5
3000a4ce:	3b0c      	subs	r3, #12
3000a4d0:	071b      	lsls	r3, r3, #28
3000a4d2:	930e      	str	r3, [sp, #56]	; 0x38
3000a4d4:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a4d6:	2b20      	cmp	r3, #32
3000a4d8:	f000 8208 	beq.w	3000a8ec <rxi316_perf_tune+0x6dc>
3000a4dc:	1b5b      	subs	r3, r3, r5
3000a4de:	3b0d      	subs	r3, #13
3000a4e0:	f003 030f 	and.w	r3, r3, #15
3000a4e4:	930f      	str	r3, [sp, #60]	; 0x3c
3000a4e6:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a4e8:	2b20      	cmp	r3, #32
3000a4ea:	f000 8205 	beq.w	3000a8f8 <rxi316_perf_tune+0x6e8>
3000a4ee:	1b5b      	subs	r3, r3, r5
3000a4f0:	3b0e      	subs	r3, #14
3000a4f2:	011b      	lsls	r3, r3, #4
3000a4f4:	b2db      	uxtb	r3, r3
3000a4f6:	9310      	str	r3, [sp, #64]	; 0x40
3000a4f8:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a4fa:	2b20      	cmp	r3, #32
3000a4fc:	f000 8202 	beq.w	3000a904 <rxi316_perf_tune+0x6f4>
3000a500:	1b5b      	subs	r3, r3, r5
3000a502:	3b0f      	subs	r3, #15
3000a504:	021b      	lsls	r3, r3, #8
3000a506:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a50a:	9311      	str	r3, [sp, #68]	; 0x44
3000a50c:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a50e:	2b20      	cmp	r3, #32
3000a510:	f000 81ff 	beq.w	3000a912 <rxi316_perf_tune+0x702>
3000a514:	1b5b      	subs	r3, r3, r5
3000a516:	031b      	lsls	r3, r3, #12
3000a518:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a51c:	9312      	str	r3, [sp, #72]	; 0x48
3000a51e:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a520:	2b20      	cmp	r3, #32
3000a522:	f000 81fd 	beq.w	3000a920 <rxi316_perf_tune+0x710>
3000a526:	1b5b      	subs	r3, r3, r5
3000a528:	3b11      	subs	r3, #17
3000a52a:	041b      	lsls	r3, r3, #16
3000a52c:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a530:	9313      	str	r3, [sp, #76]	; 0x4c
3000a532:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a534:	2b20      	cmp	r3, #32
3000a536:	f000 81fa 	beq.w	3000a92e <rxi316_perf_tune+0x71e>
3000a53a:	1b5b      	subs	r3, r3, r5
3000a53c:	3b12      	subs	r3, #18
3000a53e:	051b      	lsls	r3, r3, #20
3000a540:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a544:	931f      	str	r3, [sp, #124]	; 0x7c
3000a546:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a548:	2b20      	cmp	r3, #32
3000a54a:	f000 81f7 	beq.w	3000a93c <rxi316_perf_tune+0x72c>
3000a54e:	1b5b      	subs	r3, r3, r5
3000a550:	3b13      	subs	r3, #19
3000a552:	061b      	lsls	r3, r3, #24
3000a554:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a558:	931e      	str	r3, [sp, #120]	; 0x78
3000a55a:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a55c:	2b20      	cmp	r3, #32
3000a55e:	f000 81f4 	beq.w	3000a94a <rxi316_perf_tune+0x73a>
3000a562:	1b5b      	subs	r3, r3, r5
3000a564:	3b14      	subs	r3, #20
3000a566:	071b      	lsls	r3, r3, #28
3000a568:	931d      	str	r3, [sp, #116]	; 0x74
3000a56a:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a56c:	2b20      	cmp	r3, #32
3000a56e:	f000 81f3 	beq.w	3000a958 <rxi316_perf_tune+0x748>
3000a572:	1b5b      	subs	r3, r3, r5
3000a574:	3b15      	subs	r3, #21
3000a576:	f003 030f 	and.w	r3, r3, #15
3000a57a:	9318      	str	r3, [sp, #96]	; 0x60
3000a57c:	9b06      	ldr	r3, [sp, #24]
3000a57e:	2b20      	cmp	r3, #32
3000a580:	f000 81f0 	beq.w	3000a964 <rxi316_perf_tune+0x754>
3000a584:	1b5b      	subs	r3, r3, r5
3000a586:	3b02      	subs	r3, #2
3000a588:	f003 031f 	and.w	r3, r3, #31
3000a58c:	9317      	str	r3, [sp, #92]	; 0x5c
3000a58e:	9b07      	ldr	r3, [sp, #28]
3000a590:	2b20      	cmp	r3, #32
3000a592:	f000 81ed 	beq.w	3000a970 <rxi316_perf_tune+0x760>
3000a596:	1b5b      	subs	r3, r3, r5
3000a598:	3b02      	subs	r3, #2
3000a59a:	015b      	lsls	r3, r3, #5
3000a59c:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a5a0:	9316      	str	r3, [sp, #88]	; 0x58
3000a5a2:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a5a4:	2b20      	cmp	r3, #32
3000a5a6:	f000 81ea 	beq.w	3000a97e <rxi316_perf_tune+0x76e>
3000a5aa:	1b5b      	subs	r3, r3, r5
3000a5ac:	f1b9 0f20 	cmp.w	r9, #32
3000a5b0:	f1a3 0302 	sub.w	r3, r3, #2
3000a5b4:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a5b8:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a5bc:	9315      	str	r3, [sp, #84]	; 0x54
3000a5be:	f000 81e5 	beq.w	3000a98c <rxi316_perf_tune+0x77c>
3000a5c2:	eba9 0105 	sub.w	r1, r9, r5
3000a5c6:	f1b8 0f20 	cmp.w	r8, #32
3000a5ca:	f1a1 0102 	sub.w	r1, r1, #2
3000a5ce:	f001 030f 	and.w	r3, r1, #15
3000a5d2:	9314      	str	r3, [sp, #80]	; 0x50
3000a5d4:	f000 81e0 	beq.w	3000a998 <rxi316_perf_tune+0x788>
3000a5d8:	eba8 0105 	sub.w	r1, r8, r5
3000a5dc:	2f20      	cmp	r7, #32
3000a5de:	f1a1 0103 	sub.w	r1, r1, #3
3000a5e2:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a5e6:	b2c9      	uxtb	r1, r1
3000a5e8:	f000 81db 	beq.w	3000a9a2 <rxi316_perf_tune+0x792>
3000a5ec:	1b7b      	subs	r3, r7, r5
3000a5ee:	f1ba 0f20 	cmp.w	sl, #32
3000a5f2:	f1a3 0304 	sub.w	r3, r3, #4
3000a5f6:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a5fa:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a5fe:	9307      	str	r3, [sp, #28]
3000a600:	f000 81d6 	beq.w	3000a9b0 <rxi316_perf_tune+0x7a0>
3000a604:	ebaa 0305 	sub.w	r3, sl, r5
3000a608:	f1bb 0f20 	cmp.w	fp, #32
3000a60c:	f1a3 0305 	sub.w	r3, r3, #5
3000a610:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a614:	b29b      	uxth	r3, r3
3000a616:	9306      	str	r3, [sp, #24]
3000a618:	f000 81d1 	beq.w	3000a9be <rxi316_perf_tune+0x7ae>
3000a61c:	ebab 0c05 	sub.w	ip, fp, r5
3000a620:	9b00      	ldr	r3, [sp, #0]
3000a622:	f1ac 0c06 	sub.w	ip, ip, #6
3000a626:	2b20      	cmp	r3, #32
3000a628:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a62c:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a630:	f000 81cb 	beq.w	3000a9ca <rxi316_perf_tune+0x7ba>
3000a634:	eba3 0e05 	sub.w	lr, r3, r5
3000a638:	9b01      	ldr	r3, [sp, #4]
3000a63a:	f1ae 0e07 	sub.w	lr, lr, #7
3000a63e:	2b20      	cmp	r3, #32
3000a640:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a644:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a648:	f000 81c5 	beq.w	3000a9d6 <rxi316_perf_tune+0x7c6>
3000a64c:	1b5f      	subs	r7, r3, r5
3000a64e:	3f08      	subs	r7, #8
3000a650:	063f      	lsls	r7, r7, #24
3000a652:	f007 6370 	and.w	r3, r7, #251658240	; 0xf000000
3000a656:	9301      	str	r3, [sp, #4]
3000a658:	9b03      	ldr	r3, [sp, #12]
3000a65a:	2b20      	cmp	r3, #32
3000a65c:	f000 81c2 	beq.w	3000a9e4 <rxi316_perf_tune+0x7d4>
3000a660:	e006      	b.n	3000a670 <rxi316_perf_tune+0x460>
3000a662:	bf00      	nop
3000a664:	3000db74 	.word	0x3000db74
3000a668:	3000db9c 	.word	0x3000db9c
3000a66c:	3000dbe0 	.word	0x3000dbe0
3000a670:	1b5b      	subs	r3, r3, r5
3000a672:	3b09      	subs	r3, #9
3000a674:	071b      	lsls	r3, r3, #28
3000a676:	9300      	str	r3, [sp, #0]
3000a678:	9b04      	ldr	r3, [sp, #16]
3000a67a:	2b20      	cmp	r3, #32
3000a67c:	f000 8110 	beq.w	3000a8a0 <rxi316_perf_tune+0x690>
3000a680:	eba3 0805 	sub.w	r8, r3, r5
3000a684:	f1a8 080a 	sub.w	r8, r8, #10
3000a688:	f008 080f 	and.w	r8, r8, #15
3000a68c:	9b05      	ldr	r3, [sp, #20]
3000a68e:	2b20      	cmp	r3, #32
3000a690:	f000 8104 	beq.w	3000a89c <rxi316_perf_tune+0x68c>
3000a694:	1b58      	subs	r0, r3, r5
3000a696:	380b      	subs	r0, #11
3000a698:	0100      	lsls	r0, r0, #4
3000a69a:	b2c0      	uxtb	r0, r0
3000a69c:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a69e:	2b20      	cmp	r3, #32
3000a6a0:	f000 80f9 	beq.w	3000a896 <rxi316_perf_tune+0x686>
3000a6a4:	eba3 0a05 	sub.w	sl, r3, r5
3000a6a8:	f1aa 0a02 	sub.w	sl, sl, #2
3000a6ac:	f00a 0a1f 	and.w	sl, sl, #31
3000a6b0:	9b08      	ldr	r3, [sp, #32]
3000a6b2:	2b20      	cmp	r3, #32
3000a6b4:	f000 80ec 	beq.w	3000a890 <rxi316_perf_tune+0x680>
3000a6b8:	eba3 0905 	sub.w	r9, r3, r5
3000a6bc:	f1a9 0902 	sub.w	r9, r9, #2
3000a6c0:	f009 091f 	and.w	r9, r9, #31
3000a6c4:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a6c6:	2b20      	cmp	r3, #32
3000a6c8:	f000 80df 	beq.w	3000a88a <rxi316_perf_tune+0x67a>
3000a6cc:	eba3 0b05 	sub.w	fp, r3, r5
3000a6d0:	f1ab 0b02 	sub.w	fp, fp, #2
3000a6d4:	f00b 0b1f 	and.w	fp, fp, #31
3000a6d8:	2c20      	cmp	r4, #32
3000a6da:	f000 80d3 	beq.w	3000a884 <rxi316_perf_tune+0x674>
3000a6de:	1b63      	subs	r3, r4, r5
3000a6e0:	3b02      	subs	r3, #2
3000a6e2:	015b      	lsls	r3, r3, #5
3000a6e4:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a6e8:	9c02      	ldr	r4, [sp, #8]
3000a6ea:	ea40 0008 	orr.w	r0, r0, r8
3000a6ee:	ea43 030b 	orr.w	r3, r3, fp
3000a6f2:	4314      	orrs	r4, r2
3000a6f4:	4622      	mov	r2, r4
3000a6f6:	e9dd 450f 	ldrd	r4, r5, [sp, #60]	; 0x3c
3000a6fa:	432c      	orrs	r4, r5
3000a6fc:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000a6fe:	4329      	orrs	r1, r5
3000a700:	e9dd 5716 	ldrd	r5, r7, [sp, #88]	; 0x58
3000a704:	433d      	orrs	r5, r7
3000a706:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
3000a708:	433a      	orrs	r2, r7
3000a70a:	9f11      	ldr	r7, [sp, #68]	; 0x44
3000a70c:	433c      	orrs	r4, r7
3000a70e:	9f07      	ldr	r7, [sp, #28]
3000a710:	4339      	orrs	r1, r7
3000a712:	9f15      	ldr	r7, [sp, #84]	; 0x54
3000a714:	433d      	orrs	r5, r7
3000a716:	9f0a      	ldr	r7, [sp, #40]	; 0x28
3000a718:	433a      	orrs	r2, r7
3000a71a:	9f12      	ldr	r7, [sp, #72]	; 0x48
3000a71c:	433c      	orrs	r4, r7
3000a71e:	9f06      	ldr	r7, [sp, #24]
3000a720:	4339      	orrs	r1, r7
3000a722:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a724:	433a      	orrs	r2, r7
3000a726:	9f13      	ldr	r7, [sp, #76]	; 0x4c
3000a728:	ea41 010c 	orr.w	r1, r1, ip
3000a72c:	433c      	orrs	r4, r7
3000a72e:	9f0c      	ldr	r7, [sp, #48]	; 0x30
3000a730:	ea41 010e 	orr.w	r1, r1, lr
3000a734:	433a      	orrs	r2, r7
3000a736:	9f1f      	ldr	r7, [sp, #124]	; 0x7c
3000a738:	433c      	orrs	r4, r7
3000a73a:	9f0d      	ldr	r7, [sp, #52]	; 0x34
3000a73c:	433a      	orrs	r2, r7
3000a73e:	9f1e      	ldr	r7, [sp, #120]	; 0x78
3000a740:	433c      	orrs	r4, r7
3000a742:	9f01      	ldr	r7, [sp, #4]
3000a744:	4339      	orrs	r1, r7
3000a746:	9f0e      	ldr	r7, [sp, #56]	; 0x38
3000a748:	433a      	orrs	r2, r7
3000a74a:	9f1d      	ldr	r7, [sp, #116]	; 0x74
3000a74c:	433c      	orrs	r4, r7
3000a74e:	9f00      	ldr	r7, [sp, #0]
3000a750:	4339      	orrs	r1, r7
3000a752:	4fb7      	ldr	r7, [pc, #732]	; (3000aa30 <rxi316_perf_tune+0x820>)
3000a754:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000a758:	f8c7 4404 	str.w	r4, [r7, #1028]	; 0x404
3000a75c:	9a18      	ldr	r2, [sp, #96]	; 0x60
3000a75e:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000a762:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000a766:	f8c7 1410 	str.w	r1, [r7, #1040]	; 0x410
3000a76a:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000a76e:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000a772:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000a776:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000a77a:	68f3      	ldr	r3, [r6, #12]
3000a77c:	689a      	ldr	r2, [r3, #8]
3000a77e:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a782:	f003 0301 	and.w	r3, r3, #1
3000a786:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a78a:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000a78e:	68f3      	ldr	r3, [r6, #12]
3000a790:	689a      	ldr	r2, [r3, #8]
3000a792:	7a13      	ldrb	r3, [r2, #8]
3000a794:	6951      	ldr	r1, [r2, #20]
3000a796:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000a79a:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000a79e:	0209      	lsls	r1, r1, #8
3000a7a0:	0412      	lsls	r2, r2, #16
3000a7a2:	b289      	uxth	r1, r1
3000a7a4:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7a8:	430b      	orrs	r3, r1
3000a7aa:	4313      	orrs	r3, r2
3000a7ac:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000a7b0:	68f3      	ldr	r3, [r6, #12]
3000a7b2:	6899      	ldr	r1, [r3, #8]
3000a7b4:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000a7b8:	7e09      	ldrb	r1, [r1, #24]
3000a7ba:	021b      	lsls	r3, r3, #8
3000a7bc:	0412      	lsls	r2, r2, #16
3000a7be:	b29b      	uxth	r3, r3
3000a7c0:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a7c4:	4313      	orrs	r3, r2
3000a7c6:	430b      	orrs	r3, r1
3000a7c8:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000a7cc:	68f3      	ldr	r3, [r6, #12]
3000a7ce:	689a      	ldr	r2, [r3, #8]
3000a7d0:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a7d4:	041b      	lsls	r3, r3, #16
3000a7d6:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a7da:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a7de:	4313      	orrs	r3, r2
3000a7e0:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000a7e4:	68f3      	ldr	r3, [r6, #12]
3000a7e6:	6899      	ldr	r1, [r3, #8]
3000a7e8:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a7ea:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a7ec:	005b      	lsls	r3, r3, #1
3000a7ee:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a7f0:	0412      	lsls	r2, r2, #16
3000a7f2:	f003 0302 	and.w	r3, r3, #2
3000a7f6:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a7fa:	4313      	orrs	r3, r2
3000a7fc:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000a7fe:	f000 0101 	and.w	r1, r0, #1
3000a802:	0212      	lsls	r2, r2, #8
3000a804:	430b      	orrs	r3, r1
3000a806:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a80a:	4313      	orrs	r3, r2
3000a80c:	4a89      	ldr	r2, [pc, #548]	; (3000aa34 <rxi316_perf_tune+0x824>)
3000a80e:	431a      	orrs	r2, r3
3000a810:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000a814:	68f3      	ldr	r3, [r6, #12]
3000a816:	689b      	ldr	r3, [r3, #8]
3000a818:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000a81a:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000a81e:	68f3      	ldr	r3, [r6, #12]
3000a820:	689b      	ldr	r3, [r3, #8]
3000a822:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000a824:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000a828:	68f3      	ldr	r3, [r6, #12]
3000a82a:	689b      	ldr	r3, [r3, #8]
3000a82c:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000a82e:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000a832:	68f3      	ldr	r3, [r6, #12]
3000a834:	689b      	ldr	r3, [r3, #8]
3000a836:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000a838:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000a83c:	68f3      	ldr	r3, [r6, #12]
3000a83e:	689b      	ldr	r3, [r3, #8]
3000a840:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000a842:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000a846:	68f3      	ldr	r3, [r6, #12]
3000a848:	689b      	ldr	r3, [r3, #8]
3000a84a:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000a84c:	f003 030f 	and.w	r3, r3, #15
3000a850:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000a854:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000a858:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000a85c:	f043 0304 	orr.w	r3, r3, #4
3000a860:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000a864:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000a868:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000a86c:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000a870:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000a874:	b041      	add	sp, #260	; 0x104
3000a876:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a87a:	2502      	movs	r5, #2
3000a87c:	e4d7      	b.n	3000a22e <rxi316_perf_tune+0x1e>
3000a87e:	3101      	adds	r1, #1
3000a880:	9303      	str	r3, [sp, #12]
3000a882:	e53e      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000a884:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a888:	e72e      	b.n	3000a6e8 <rxi316_perf_tune+0x4d8>
3000a88a:	f04f 0b1f 	mov.w	fp, #31
3000a88e:	e723      	b.n	3000a6d8 <rxi316_perf_tune+0x4c8>
3000a890:	f04f 091f 	mov.w	r9, #31
3000a894:	e716      	b.n	3000a6c4 <rxi316_perf_tune+0x4b4>
3000a896:	f04f 0a1f 	mov.w	sl, #31
3000a89a:	e709      	b.n	3000a6b0 <rxi316_perf_tune+0x4a0>
3000a89c:	20f0      	movs	r0, #240	; 0xf0
3000a89e:	e6fd      	b.n	3000a69c <rxi316_perf_tune+0x48c>
3000a8a0:	f04f 080f 	mov.w	r8, #15
3000a8a4:	e6f2      	b.n	3000a68c <rxi316_perf_tune+0x47c>
3000a8a6:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a8aa:	930a      	str	r3, [sp, #40]	; 0x28
3000a8ac:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a8ae:	2b20      	cmp	r3, #32
3000a8b0:	f47f adee 	bne.w	3000a490 <rxi316_perf_tune+0x280>
3000a8b4:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a8b8:	9309      	str	r3, [sp, #36]	; 0x24
3000a8ba:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a8bc:	2b20      	cmp	r3, #32
3000a8be:	f47f adf1 	bne.w	3000a4a4 <rxi316_perf_tune+0x294>
3000a8c2:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a8c6:	930c      	str	r3, [sp, #48]	; 0x30
3000a8c8:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a8ca:	2b20      	cmp	r3, #32
3000a8cc:	f47f adf4 	bne.w	3000a4b8 <rxi316_perf_tune+0x2a8>
3000a8d0:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a8d4:	930d      	str	r3, [sp, #52]	; 0x34
3000a8d6:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a8d8:	2b20      	cmp	r3, #32
3000a8da:	f47f adf7 	bne.w	3000a4cc <rxi316_perf_tune+0x2bc>
3000a8de:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a8e2:	930e      	str	r3, [sp, #56]	; 0x38
3000a8e4:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a8e6:	2b20      	cmp	r3, #32
3000a8e8:	f47f adf8 	bne.w	3000a4dc <rxi316_perf_tune+0x2cc>
3000a8ec:	230f      	movs	r3, #15
3000a8ee:	930f      	str	r3, [sp, #60]	; 0x3c
3000a8f0:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a8f2:	2b20      	cmp	r3, #32
3000a8f4:	f47f adfb 	bne.w	3000a4ee <rxi316_perf_tune+0x2de>
3000a8f8:	23f0      	movs	r3, #240	; 0xf0
3000a8fa:	9310      	str	r3, [sp, #64]	; 0x40
3000a8fc:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a8fe:	2b20      	cmp	r3, #32
3000a900:	f47f adfe 	bne.w	3000a500 <rxi316_perf_tune+0x2f0>
3000a904:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a908:	9311      	str	r3, [sp, #68]	; 0x44
3000a90a:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a90c:	2b20      	cmp	r3, #32
3000a90e:	f47f ae01 	bne.w	3000a514 <rxi316_perf_tune+0x304>
3000a912:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a916:	9312      	str	r3, [sp, #72]	; 0x48
3000a918:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a91a:	2b20      	cmp	r3, #32
3000a91c:	f47f ae03 	bne.w	3000a526 <rxi316_perf_tune+0x316>
3000a920:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a924:	9313      	str	r3, [sp, #76]	; 0x4c
3000a926:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a928:	2b20      	cmp	r3, #32
3000a92a:	f47f ae06 	bne.w	3000a53a <rxi316_perf_tune+0x32a>
3000a92e:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a932:	931f      	str	r3, [sp, #124]	; 0x7c
3000a934:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a936:	2b20      	cmp	r3, #32
3000a938:	f47f ae09 	bne.w	3000a54e <rxi316_perf_tune+0x33e>
3000a93c:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a940:	931e      	str	r3, [sp, #120]	; 0x78
3000a942:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a944:	2b20      	cmp	r3, #32
3000a946:	f47f ae0c 	bne.w	3000a562 <rxi316_perf_tune+0x352>
3000a94a:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a94e:	931d      	str	r3, [sp, #116]	; 0x74
3000a950:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a952:	2b20      	cmp	r3, #32
3000a954:	f47f ae0d 	bne.w	3000a572 <rxi316_perf_tune+0x362>
3000a958:	230f      	movs	r3, #15
3000a95a:	9318      	str	r3, [sp, #96]	; 0x60
3000a95c:	9b06      	ldr	r3, [sp, #24]
3000a95e:	2b20      	cmp	r3, #32
3000a960:	f47f ae10 	bne.w	3000a584 <rxi316_perf_tune+0x374>
3000a964:	231f      	movs	r3, #31
3000a966:	9317      	str	r3, [sp, #92]	; 0x5c
3000a968:	9b07      	ldr	r3, [sp, #28]
3000a96a:	2b20      	cmp	r3, #32
3000a96c:	f47f ae13 	bne.w	3000a596 <rxi316_perf_tune+0x386>
3000a970:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a974:	9316      	str	r3, [sp, #88]	; 0x58
3000a976:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a978:	2b20      	cmp	r3, #32
3000a97a:	f47f ae16 	bne.w	3000a5aa <rxi316_perf_tune+0x39a>
3000a97e:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000a982:	f1b9 0f20 	cmp.w	r9, #32
3000a986:	9315      	str	r3, [sp, #84]	; 0x54
3000a988:	f47f ae1b 	bne.w	3000a5c2 <rxi316_perf_tune+0x3b2>
3000a98c:	230f      	movs	r3, #15
3000a98e:	f1b8 0f20 	cmp.w	r8, #32
3000a992:	9314      	str	r3, [sp, #80]	; 0x50
3000a994:	f47f ae20 	bne.w	3000a5d8 <rxi316_perf_tune+0x3c8>
3000a998:	2f20      	cmp	r7, #32
3000a99a:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a99e:	f47f ae25 	bne.w	3000a5ec <rxi316_perf_tune+0x3dc>
3000a9a2:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a9a6:	f1ba 0f20 	cmp.w	sl, #32
3000a9aa:	9307      	str	r3, [sp, #28]
3000a9ac:	f47f ae2a 	bne.w	3000a604 <rxi316_perf_tune+0x3f4>
3000a9b0:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a9b4:	f1bb 0f20 	cmp.w	fp, #32
3000a9b8:	9306      	str	r3, [sp, #24]
3000a9ba:	f47f ae2f 	bne.w	3000a61c <rxi316_perf_tune+0x40c>
3000a9be:	9b00      	ldr	r3, [sp, #0]
3000a9c0:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a9c4:	2b20      	cmp	r3, #32
3000a9c6:	f47f ae35 	bne.w	3000a634 <rxi316_perf_tune+0x424>
3000a9ca:	9b01      	ldr	r3, [sp, #4]
3000a9cc:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a9d0:	2b20      	cmp	r3, #32
3000a9d2:	f47f ae3b 	bne.w	3000a64c <rxi316_perf_tune+0x43c>
3000a9d6:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a9da:	9301      	str	r3, [sp, #4]
3000a9dc:	9b03      	ldr	r3, [sp, #12]
3000a9de:	2b20      	cmp	r3, #32
3000a9e0:	f47f ae46 	bne.w	3000a670 <rxi316_perf_tune+0x460>
3000a9e4:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a9e8:	9300      	str	r3, [sp, #0]
3000a9ea:	e645      	b.n	3000a678 <rxi316_perf_tune+0x468>
3000a9ec:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a9f0:	930b      	str	r3, [sp, #44]	; 0x2c
3000a9f2:	e540      	b.n	3000a476 <rxi316_perf_tune+0x266>
3000a9f4:	22f0      	movs	r2, #240	; 0xf0
3000a9f6:	e534      	b.n	3000a462 <rxi316_perf_tune+0x252>
3000a9f8:	230f      	movs	r3, #15
3000a9fa:	9302      	str	r3, [sp, #8]
3000a9fc:	e529      	b.n	3000a452 <rxi316_perf_tune+0x242>
3000a9fe:	3101      	adds	r1, #1
3000aa00:	9304      	str	r3, [sp, #16]
3000aa02:	e47e      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa04:	3101      	adds	r1, #1
3000aa06:	9305      	str	r3, [sp, #20]
3000aa08:	e47b      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa0a:	2202      	movs	r2, #2
3000aa0c:	921d      	str	r2, [sp, #116]	; 0x74
3000aa0e:	e504      	b.n	3000a41a <rxi316_perf_tune+0x20a>
3000aa10:	9a02      	ldr	r2, [sp, #8]
3000aa12:	9307      	str	r3, [sp, #28]
3000aa14:	3201      	adds	r2, #1
3000aa16:	9202      	str	r2, [sp, #8]
3000aa18:	e473      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa1a:	9a02      	ldr	r2, [sp, #8]
3000aa1c:	9306      	str	r3, [sp, #24]
3000aa1e:	3201      	adds	r2, #1
3000aa20:	9202      	str	r2, [sp, #8]
3000aa22:	e46e      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa24:	9a02      	ldr	r2, [sp, #8]
3000aa26:	931a      	str	r3, [sp, #104]	; 0x68
3000aa28:	3201      	adds	r2, #1
3000aa2a:	9202      	str	r2, [sp, #8]
3000aa2c:	e469      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa2e:	bf00      	nop
3000aa30:	41100000 	.word	0x41100000
3000aa34:	40000008 	.word	0x40000008
3000aa38:	9a02      	ldr	r2, [sp, #8]
3000aa3a:	461c      	mov	r4, r3
3000aa3c:	3201      	adds	r2, #1
3000aa3e:	9202      	str	r2, [sp, #8]
3000aa40:	e45f      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa42:	9a02      	ldr	r2, [sp, #8]
3000aa44:	931c      	str	r3, [sp, #112]	; 0x70
3000aa46:	3201      	adds	r2, #1
3000aa48:	9202      	str	r2, [sp, #8]
3000aa4a:	e45a      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa4c:	9a02      	ldr	r2, [sp, #8]
3000aa4e:	931b      	str	r3, [sp, #108]	; 0x6c
3000aa50:	3201      	adds	r2, #1
3000aa52:	9202      	str	r2, [sp, #8]
3000aa54:	e455      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa56:	930d      	str	r3, [sp, #52]	; 0x34
3000aa58:	e453      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa5a:	930c      	str	r3, [sp, #48]	; 0x30
3000aa5c:	e451      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa5e:	930b      	str	r3, [sp, #44]	; 0x2c
3000aa60:	e44f      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa62:	9309      	str	r3, [sp, #36]	; 0x24
3000aa64:	e44d      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa66:	930a      	str	r3, [sp, #40]	; 0x28
3000aa68:	e44b      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa6a:	9319      	str	r3, [sp, #100]	; 0x64
3000aa6c:	e449      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa6e:	9318      	str	r3, [sp, #96]	; 0x60
3000aa70:	e447      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa72:	930f      	str	r3, [sp, #60]	; 0x3c
3000aa74:	e445      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa76:	930e      	str	r3, [sp, #56]	; 0x38
3000aa78:	e443      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa7a:	9317      	str	r3, [sp, #92]	; 0x5c
3000aa7c:	e441      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa7e:	9316      	str	r3, [sp, #88]	; 0x58
3000aa80:	e43f      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa82:	9315      	str	r3, [sp, #84]	; 0x54
3000aa84:	e43d      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa86:	9314      	str	r3, [sp, #80]	; 0x50
3000aa88:	e43b      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa8a:	9313      	str	r3, [sp, #76]	; 0x4c
3000aa8c:	e439      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa8e:	9312      	str	r3, [sp, #72]	; 0x48
3000aa90:	e437      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa92:	9311      	str	r3, [sp, #68]	; 0x44
3000aa94:	e435      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa96:	9310      	str	r3, [sp, #64]	; 0x40
3000aa98:	e433      	b.n	3000a302 <rxi316_perf_tune+0xf2>
3000aa9a:	bf00      	nop

3000aa9c <rxi316_dram_init>:
3000aa9c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000aaa0:	4604      	mov	r4, r0
3000aaa2:	b08f      	sub	sp, #60	; 0x3c
3000aaa4:	4b10      	ldr	r3, [pc, #64]	; (3000aae8 <rxi316_dram_init+0x4c>)
3000aaa6:	4798      	blx	r3
3000aaa8:	f894 b014 	ldrb.w	fp, [r4, #20]
3000aaac:	2101      	movs	r1, #1
3000aaae:	900c      	str	r0, [sp, #48]	; 0x30
3000aab0:	fa01 f30b 	lsl.w	r3, r1, fp
3000aab4:	9302      	str	r3, [sp, #8]
3000aab6:	6823      	ldr	r3, [r4, #0]
3000aab8:	78da      	ldrb	r2, [r3, #3]
3000aaba:	6923      	ldr	r3, [r4, #16]
3000aabc:	fa03 f30b 	lsl.w	r3, r3, fp
3000aac0:	b1a2      	cbz	r2, 3000aaec <rxi316_dram_init+0x50>
3000aac2:	428a      	cmp	r2, r1
3000aac4:	f000 847c 	beq.w	3000b3c0 <rxi316_dram_init+0x924>
3000aac8:	2a02      	cmp	r2, #2
3000aaca:	bf0b      	itete	eq
3000aacc:	2206      	moveq	r2, #6
3000aace:	220a      	movne	r2, #10
3000aad0:	2540      	moveq	r5, #64	; 0x40
3000aad2:	2580      	movne	r5, #128	; 0x80
3000aad4:	9207      	str	r2, [sp, #28]
3000aad6:	bf0c      	ite	eq
3000aad8:	2204      	moveq	r2, #4
3000aada:	2208      	movne	r2, #8
3000aadc:	9208      	str	r2, [sp, #32]
3000aade:	bf0c      	ite	eq
3000aae0:	2208      	moveq	r2, #8
3000aae2:	2210      	movne	r2, #16
3000aae4:	9201      	str	r2, [sp, #4]
3000aae6:	e007      	b.n	3000aaf8 <rxi316_dram_init+0x5c>
3000aae8:	30008001 	.word	0x30008001
3000aaec:	2203      	movs	r2, #3
3000aaee:	25c0      	movs	r5, #192	; 0xc0
3000aaf0:	9108      	str	r1, [sp, #32]
3000aaf2:	9207      	str	r2, [sp, #28]
3000aaf4:	2202      	movs	r2, #2
3000aaf6:	9201      	str	r2, [sp, #4]
3000aaf8:	6860      	ldr	r0, [r4, #4]
3000aafa:	f04f 31ff 	mov.w	r1, #4294967295
3000aafe:	4ac2      	ldr	r2, [pc, #776]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000ab00:	6806      	ldr	r6, [r0, #0]
3000ab02:	960d      	str	r6, [sp, #52]	; 0x34
3000ab04:	6846      	ldr	r6, [r0, #4]
3000ab06:	6880      	ldr	r0, [r0, #8]
3000ab08:	9609      	str	r6, [sp, #36]	; 0x24
3000ab0a:	9003      	str	r0, [sp, #12]
3000ab0c:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000ab10:	6820      	ldr	r0, [r4, #0]
3000ab12:	7981      	ldrb	r1, [r0, #6]
3000ab14:	6882      	ldr	r2, [r0, #8]
3000ab16:	b129      	cbz	r1, 3000ab24 <rxi316_dram_init+0x88>
3000ab18:	2901      	cmp	r1, #1
3000ab1a:	f000 8449 	beq.w	3000b3b0 <rxi316_dram_init+0x914>
3000ab1e:	2903      	cmp	r1, #3
3000ab20:	f000 8456 	beq.w	3000b3d0 <rxi316_dram_init+0x934>
3000ab24:	3a06      	subs	r2, #6
3000ab26:	7881      	ldrb	r1, [r0, #2]
3000ab28:	f002 020f 	and.w	r2, r2, #15
3000ab2c:	7940      	ldrb	r0, [r0, #5]
3000ab2e:	0109      	lsls	r1, r1, #4
3000ab30:	0280      	lsls	r0, r0, #10
3000ab32:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000ab36:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000ab3a:	4301      	orrs	r1, r0
3000ab3c:	430d      	orrs	r5, r1
3000ab3e:	432a      	orrs	r2, r5
3000ab40:	4db1      	ldr	r5, [pc, #708]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000ab42:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000ab46:	68e2      	ldr	r2, [r4, #12]
3000ab48:	6812      	ldr	r2, [r2, #0]
3000ab4a:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000ab4e:	f002 0207 	and.w	r2, r2, #7
3000ab52:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000ab56:	68e2      	ldr	r2, [r4, #12]
3000ab58:	6811      	ldr	r1, [r2, #0]
3000ab5a:	6822      	ldr	r2, [r4, #0]
3000ab5c:	6848      	ldr	r0, [r1, #4]
3000ab5e:	f892 9000 	ldrb.w	r9, [r2]
3000ab62:	688a      	ldr	r2, [r1, #8]
3000ab64:	9004      	str	r0, [sp, #16]
3000ab66:	0300      	lsls	r0, r0, #12
3000ab68:	0412      	lsls	r2, r2, #16
3000ab6a:	f009 070f 	and.w	r7, r9, #15
3000ab6e:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000ab72:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000ab76:	4302      	orrs	r2, r0
3000ab78:	68c8      	ldr	r0, [r1, #12]
3000ab7a:	0440      	lsls	r0, r0, #17
3000ab7c:	433a      	orrs	r2, r7
3000ab7e:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000ab82:	6908      	ldr	r0, [r1, #16]
3000ab84:	0480      	lsls	r0, r0, #18
3000ab86:	4332      	orrs	r2, r6
3000ab88:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000ab8c:	4310      	orrs	r0, r2
3000ab8e:	694a      	ldr	r2, [r1, #20]
3000ab90:	04d2      	lsls	r2, r2, #19
3000ab92:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000ab96:	4310      	orrs	r0, r2
3000ab98:	698a      	ldr	r2, [r1, #24]
3000ab9a:	0552      	lsls	r2, r2, #21
3000ab9c:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000aba0:	4310      	orrs	r0, r2
3000aba2:	69ca      	ldr	r2, [r1, #28]
3000aba4:	0592      	lsls	r2, r2, #22
3000aba6:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000abaa:	4310      	orrs	r0, r2
3000abac:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000abae:	0612      	lsls	r2, r2, #24
3000abb0:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000abb4:	4302      	orrs	r2, r0
3000abb6:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000abba:	06c0      	lsls	r0, r0, #27
3000abbc:	0789      	lsls	r1, r1, #30
3000abbe:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000abc2:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000abc6:	4310      	orrs	r0, r2
3000abc8:	9a02      	ldr	r2, [sp, #8]
3000abca:	0212      	lsls	r2, r2, #8
3000abcc:	4308      	orrs	r0, r1
3000abce:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000abd2:	4310      	orrs	r0, r2
3000abd4:	6068      	str	r0, [r5, #4]
3000abd6:	6822      	ldr	r2, [r4, #0]
3000abd8:	7992      	ldrb	r2, [r2, #6]
3000abda:	2a00      	cmp	r2, #0
3000abdc:	f040 8317 	bne.w	3000b20e <rxi316_dram_init+0x772>
3000abe0:	686a      	ldr	r2, [r5, #4]
3000abe2:	f042 0210 	orr.w	r2, r2, #16
3000abe6:	606a      	str	r2, [r5, #4]
3000abe8:	68e2      	ldr	r2, [r4, #12]
3000abea:	9802      	ldr	r0, [sp, #8]
3000abec:	6812      	ldr	r2, [r2, #0]
3000abee:	f100 3aff 	add.w	sl, r0, #4294967295
3000abf2:	6861      	ldr	r1, [r4, #4]
3000abf4:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000abf6:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000abfa:	9005      	str	r0, [sp, #20]
3000abfc:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000abfe:	fa2e f80b 	lsr.w	r8, lr, fp
3000ac02:	6949      	ldr	r1, [r1, #20]
3000ac04:	ea1e 0f0a 	tst.w	lr, sl
3000ac08:	9006      	str	r0, [sp, #24]
3000ac0a:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000ac0c:	fa21 fe0b 	lsr.w	lr, r1, fp
3000ac10:	bf18      	it	ne
3000ac12:	f108 0801 	addne.w	r8, r8, #1
3000ac16:	ea11 0f0a 	tst.w	r1, sl
3000ac1a:	9906      	ldr	r1, [sp, #24]
3000ac1c:	950a      	str	r5, [sp, #40]	; 0x28
3000ac1e:	bf18      	it	ne
3000ac20:	f10e 0e01 	addne.w	lr, lr, #1
3000ac24:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000ac26:	0049      	lsls	r1, r1, #1
3000ac28:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000ac2c:	950b      	str	r5, [sp, #44]	; 0x2c
3000ac2e:	f001 0102 	and.w	r1, r1, #2
3000ac32:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000ac36:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000ac3a:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000ac3e:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000ac42:	0080      	lsls	r0, r0, #2
3000ac44:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000ac48:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000ac4c:	9006      	str	r0, [sp, #24]
3000ac4e:	013f      	lsls	r7, r7, #4
3000ac50:	9805      	ldr	r0, [sp, #20]
3000ac52:	f00c 0c08 	and.w	ip, ip, #8
3000ac56:	0176      	lsls	r6, r6, #5
3000ac58:	f007 0710 	and.w	r7, r7, #16
3000ac5c:	f000 0001 	and.w	r0, r0, #1
3000ac60:	f006 0620 	and.w	r6, r6, #32
3000ac64:	4605      	mov	r5, r0
3000ac66:	9806      	ldr	r0, [sp, #24]
3000ac68:	f000 0004 	and.w	r0, r0, #4
3000ac6c:	4301      	orrs	r1, r0
3000ac6e:	4329      	orrs	r1, r5
3000ac70:	ea41 0c0c 	orr.w	ip, r1, ip
3000ac74:	990a      	ldr	r1, [sp, #40]	; 0x28
3000ac76:	ea4c 0707 	orr.w	r7, ip, r7
3000ac7a:	018d      	lsls	r5, r1, #6
3000ac7c:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000ac7e:	4337      	orrs	r7, r6
3000ac80:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000ac84:	0208      	lsls	r0, r1, #8
3000ac86:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000ac88:	432f      	orrs	r7, r5
3000ac8a:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000ac8e:	06c9      	lsls	r1, r1, #27
3000ac90:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000ac94:	4307      	orrs	r7, r0
3000ac96:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000ac9a:	0736      	lsls	r6, r6, #28
3000ac9c:	430f      	orrs	r7, r1
3000ac9e:	0752      	lsls	r2, r2, #29
3000aca0:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000aca4:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000aca8:	433e      	orrs	r6, r7
3000acaa:	4316      	orrs	r6, r2
3000acac:	4a56      	ldr	r2, [pc, #344]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000acae:	ea46 0608 	orr.w	r6, r6, r8
3000acb2:	ea46 060e 	orr.w	r6, r6, lr
3000acb6:	6096      	str	r6, [r2, #8]
3000acb8:	6862      	ldr	r2, [r4, #4]
3000acba:	6990      	ldr	r0, [r2, #24]
3000acbc:	9a01      	ldr	r2, [sp, #4]
3000acbe:	2a10      	cmp	r2, #16
3000acc0:	6822      	ldr	r2, [r4, #0]
3000acc2:	7811      	ldrb	r1, [r2, #0]
3000acc4:	7916      	ldrb	r6, [r2, #4]
3000acc6:	f000 833d 	beq.w	3000b344 <rxi316_dram_init+0x8a8>
3000acca:	f1a1 0502 	sub.w	r5, r1, #2
3000acce:	f1a1 0e09 	sub.w	lr, r1, #9
3000acd2:	9f01      	ldr	r7, [sp, #4]
3000acd4:	fab5 f585 	clz	r5, r5
3000acd8:	fabe fe8e 	clz	lr, lr
3000acdc:	2f08      	cmp	r7, #8
3000acde:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000ace2:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000ace6:	ea45 020e 	orr.w	r2, r5, lr
3000acea:	f000 829e 	beq.w	3000b22a <rxi316_dram_init+0x78e>
3000acee:	9f01      	ldr	r7, [sp, #4]
3000acf0:	2f04      	cmp	r7, #4
3000acf2:	f000 83a5 	beq.w	3000b440 <rxi316_dram_init+0x9a4>
3000acf6:	9f01      	ldr	r7, [sp, #4]
3000acf8:	2f02      	cmp	r7, #2
3000acfa:	bf08      	it	eq
3000acfc:	4310      	orreq	r0, r2
3000acfe:	2a00      	cmp	r2, #0
3000ad00:	f040 8298 	bne.w	3000b234 <rxi316_dram_init+0x798>
3000ad04:	2903      	cmp	r1, #3
3000ad06:	f000 8368 	beq.w	3000b3da <rxi316_dram_init+0x93e>
3000ad0a:	2500      	movs	r5, #0
3000ad0c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000ad10:	462f      	mov	r7, r5
3000ad12:	433d      	orrs	r5, r7
3000ad14:	4f3c      	ldr	r7, [pc, #240]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000ad16:	430d      	orrs	r5, r1
3000ad18:	4305      	orrs	r5, r0
3000ad1a:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000ad1e:	637e      	str	r6, [r7, #52]	; 0x34
3000ad20:	6862      	ldr	r2, [r4, #4]
3000ad22:	9903      	ldr	r1, [sp, #12]
3000ad24:	69d2      	ldr	r2, [r2, #28]
3000ad26:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000ad2a:	63ba      	str	r2, [r7, #56]	; 0x38
3000ad2c:	6822      	ldr	r2, [r4, #0]
3000ad2e:	7810      	ldrb	r0, [r2, #0]
3000ad30:	6862      	ldr	r2, [r4, #4]
3000ad32:	2803      	cmp	r0, #3
3000ad34:	6a11      	ldr	r1, [r2, #32]
3000ad36:	f000 82f8 	beq.w	3000b32a <rxi316_dram_init+0x88e>
3000ad3a:	2809      	cmp	r0, #9
3000ad3c:	f000 830d 	beq.w	3000b35a <rxi316_dram_init+0x8be>
3000ad40:	4831      	ldr	r0, [pc, #196]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000ad42:	63c1      	str	r1, [r0, #60]	; 0x3c
3000ad44:	6862      	ldr	r2, [r4, #4]
3000ad46:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000ad48:	6402      	str	r2, [r0, #64]	; 0x40
3000ad4a:	6865      	ldr	r5, [r4, #4]
3000ad4c:	68e8      	ldr	r0, [r5, #12]
3000ad4e:	682a      	ldr	r2, [r5, #0]
3000ad50:	6869      	ldr	r1, [r5, #4]
3000ad52:	4402      	add	r2, r0
3000ad54:	68ae      	ldr	r6, [r5, #8]
3000ad56:	4401      	add	r1, r0
3000ad58:	ea1a 0f02 	tst.w	sl, r2
3000ad5c:	f000 82b8 	beq.w	3000b2d0 <rxi316_dram_init+0x834>
3000ad60:	fa22 f20b 	lsr.w	r2, r2, fp
3000ad64:	1c50      	adds	r0, r2, #1
3000ad66:	ea1a 0f01 	tst.w	sl, r1
3000ad6a:	fa21 f50b 	lsr.w	r5, r1, fp
3000ad6e:	6822      	ldr	r2, [r4, #0]
3000ad70:	fa26 f10b 	lsr.w	r1, r6, fp
3000ad74:	bf18      	it	ne
3000ad76:	3501      	addne	r5, #1
3000ad78:	ea1a 0f06 	tst.w	sl, r6
3000ad7c:	7816      	ldrb	r6, [r2, #0]
3000ad7e:	bf18      	it	ne
3000ad80:	3101      	addne	r1, #1
3000ad82:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000ad86:	1eb2      	subs	r2, r6, #2
3000ad88:	2a01      	cmp	r2, #1
3000ad8a:	f240 82a4 	bls.w	3000b2d6 <rxi316_dram_init+0x83a>
3000ad8e:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000ad92:	fbb7 f2f3 	udiv	r2, r7, r3
3000ad96:	fb03 7712 	mls	r7, r3, r2, r7
3000ad9a:	b107      	cbz	r7, 3000ad9e <rxi316_dram_init+0x302>
3000ad9c:	3201      	adds	r2, #1
3000ad9e:	2e09      	cmp	r6, #9
3000ada0:	f040 82b3 	bne.w	3000b30a <rxi316_dram_init+0x86e>
3000ada4:	9e08      	ldr	r6, [sp, #32]
3000ada6:	fa26 f60b 	lsr.w	r6, r6, fp
3000adaa:	016d      	lsls	r5, r5, #5
3000adac:	f000 001f 	and.w	r0, r0, #31
3000adb0:	0289      	lsls	r1, r1, #10
3000adb2:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000adb6:	0512      	lsls	r2, r2, #20
3000adb8:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000adbc:	4305      	orrs	r5, r0
3000adbe:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000adc2:	430d      	orrs	r5, r1
3000adc4:	4910      	ldr	r1, [pc, #64]	; (3000ae08 <rxi316_dram_init+0x36c>)
3000adc6:	4315      	orrs	r5, r2
3000adc8:	0672      	lsls	r2, r6, #25
3000adca:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000adce:	432a      	orrs	r2, r5
3000add0:	630a      	str	r2, [r1, #48]	; 0x30
3000add2:	68a0      	ldr	r0, [r4, #8]
3000add4:	6802      	ldr	r2, [r0, #0]
3000add6:	fbb2 f1f3 	udiv	r1, r2, r3
3000adda:	fb03 2211 	mls	r2, r3, r1, r2
3000adde:	2a00      	cmp	r2, #0
3000ade0:	f000 8274 	beq.w	3000b2cc <rxi316_dram_init+0x830>
3000ade4:	1c4a      	adds	r2, r1, #1
3000ade6:	9205      	str	r2, [sp, #20]
3000ade8:	6842      	ldr	r2, [r0, #4]
3000adea:	fbb2 f0f3 	udiv	r0, r2, r3
3000adee:	fb03 2210 	mls	r2, r3, r0, r2
3000adf2:	2a00      	cmp	r2, #0
3000adf4:	f000 8268 	beq.w	3000b2c8 <rxi316_dram_init+0x82c>
3000adf8:	38c7      	subs	r0, #199	; 0xc7
3000adfa:	68e2      	ldr	r2, [r4, #12]
3000adfc:	6811      	ldr	r1, [r2, #0]
3000adfe:	4a03      	ldr	r2, [pc, #12]	; (3000ae0c <rxi316_dram_init+0x370>)
3000ae00:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000ae04:	e004      	b.n	3000ae10 <rxi316_dram_init+0x374>
3000ae06:	bf00      	nop
3000ae08:	41100000 	.word	0x41100000
3000ae0c:	00ffff00 	.word	0x00ffff00
3000ae10:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000ae14:	900a      	str	r0, [sp, #40]	; 0x28
3000ae16:	0712      	lsls	r2, r2, #28
3000ae18:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000ae1c:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000ae20:	430a      	orrs	r2, r1
3000ae22:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000ae26:	430a      	orrs	r2, r1
3000ae28:	49c3      	ldr	r1, [pc, #780]	; (3000b138 <rxi316_dram_init+0x69c>)
3000ae2a:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000ae2e:	610a      	str	r2, [r1, #16]
3000ae30:	68a2      	ldr	r2, [r4, #8]
3000ae32:	6911      	ldr	r1, [r2, #16]
3000ae34:	fbb1 fcf3 	udiv	ip, r1, r3
3000ae38:	fb03 111c 	mls	r1, r3, ip, r1
3000ae3c:	2900      	cmp	r1, #0
3000ae3e:	f000 8240 	beq.w	3000b2c2 <rxi316_dram_init+0x826>
3000ae42:	f10c 0101 	add.w	r1, ip, #1
3000ae46:	9106      	str	r1, [sp, #24]
3000ae48:	6991      	ldr	r1, [r2, #24]
3000ae4a:	fbb1 f0f3 	udiv	r0, r1, r3
3000ae4e:	fb03 1110 	mls	r1, r3, r0, r1
3000ae52:	b101      	cbz	r1, 3000ae56 <rxi316_dram_init+0x3ba>
3000ae54:	3001      	adds	r0, #1
3000ae56:	68d1      	ldr	r1, [r2, #12]
3000ae58:	fbb1 f5f3 	udiv	r5, r1, r3
3000ae5c:	fb03 1115 	mls	r1, r3, r5, r1
3000ae60:	9503      	str	r5, [sp, #12]
3000ae62:	b109      	cbz	r1, 3000ae68 <rxi316_dram_init+0x3cc>
3000ae64:	3501      	adds	r5, #1
3000ae66:	9503      	str	r5, [sp, #12]
3000ae68:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000ae6a:	fbb5 f1f3 	udiv	r1, r5, r3
3000ae6e:	fb03 5511 	mls	r5, r3, r1, r5
3000ae72:	b105      	cbz	r5, 3000ae76 <rxi316_dram_init+0x3da>
3000ae74:	3101      	adds	r1, #1
3000ae76:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000ae78:	0349      	lsls	r1, r1, #13
3000ae7a:	68e2      	ldr	r2, [r4, #12]
3000ae7c:	fa25 f60b 	lsr.w	r6, r5, fp
3000ae80:	ea15 0f0a 	tst.w	r5, sl
3000ae84:	6812      	ldr	r2, [r2, #0]
3000ae86:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000ae8a:	bf18      	it	ne
3000ae8c:	3601      	addne	r6, #1
3000ae8e:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000ae90:	05d2      	lsls	r2, r2, #23
3000ae92:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000ae96:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000ae9a:	9a06      	ldr	r2, [sp, #24]
3000ae9c:	0112      	lsls	r2, r2, #4
3000ae9e:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000aea2:	4332      	orrs	r2, r6
3000aea4:	0246      	lsls	r6, r0, #9
3000aea6:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000aeaa:	4316      	orrs	r6, r2
3000aeac:	9a03      	ldr	r2, [sp, #12]
3000aeae:	f002 020f 	and.w	r2, r2, #15
3000aeb2:	4316      	orrs	r6, r2
3000aeb4:	4aa0      	ldr	r2, [pc, #640]	; (3000b138 <rxi316_dram_init+0x69c>)
3000aeb6:	4331      	orrs	r1, r6
3000aeb8:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000aebc:	6151      	str	r1, [r2, #20]
3000aebe:	68a7      	ldr	r7, [r4, #8]
3000aec0:	697a      	ldr	r2, [r7, #20]
3000aec2:	fbb2 fef3 	udiv	lr, r2, r3
3000aec6:	fb03 221e 	mls	r2, r3, lr, r2
3000aeca:	b10a      	cbz	r2, 3000aed0 <rxi316_dram_init+0x434>
3000aecc:	f10e 0e01 	add.w	lr, lr, #1
3000aed0:	6939      	ldr	r1, [r7, #16]
3000aed2:	fbb1 f2f3 	udiv	r2, r1, r3
3000aed6:	fb03 1112 	mls	r1, r3, r2, r1
3000aeda:	b101      	cbz	r1, 3000aede <rxi316_dram_init+0x442>
3000aedc:	3201      	adds	r2, #1
3000aede:	68fd      	ldr	r5, [r7, #12]
3000aee0:	fbb5 f1f3 	udiv	r1, r5, r3
3000aee4:	fb03 5511 	mls	r5, r3, r1, r5
3000aee8:	b105      	cbz	r5, 3000aeec <rxi316_dram_init+0x450>
3000aeea:	3101      	adds	r1, #1
3000aeec:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000aeee:	fbb5 f6f3 	udiv	r6, r5, r3
3000aef2:	fb03 5516 	mls	r5, r3, r6, r5
3000aef6:	b105      	cbz	r5, 3000aefa <rxi316_dram_init+0x45e>
3000aef8:	3601      	adds	r6, #1
3000aefa:	68bd      	ldr	r5, [r7, #8]
3000aefc:	4411      	add	r1, r2
3000aefe:	fbb5 f2f3 	udiv	r2, r5, r3
3000af02:	42b1      	cmp	r1, r6
3000af04:	fb03 5512 	mls	r5, r3, r2, r5
3000af08:	bf38      	it	cc
3000af0a:	4631      	movcc	r1, r6
3000af0c:	b905      	cbnz	r5, 3000af10 <rxi316_dram_init+0x474>
3000af0e:	3a01      	subs	r2, #1
3000af10:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000af12:	ea15 0f0a 	tst.w	r5, sl
3000af16:	fa25 fc0b 	lsr.w	ip, r5, fp
3000af1a:	69fd      	ldr	r5, [r7, #28]
3000af1c:	bf18      	it	ne
3000af1e:	f10c 0c01 	addne.w	ip, ip, #1
3000af22:	fa25 f60b 	lsr.w	r6, r5, fp
3000af26:	ea15 0f0a 	tst.w	r5, sl
3000af2a:	bf18      	it	ne
3000af2c:	3601      	addne	r6, #1
3000af2e:	f1b9 0f09 	cmp.w	r9, #9
3000af32:	f000 81f5 	beq.w	3000b320 <rxi316_dram_init+0x884>
3000af36:	f1b9 0f02 	cmp.w	r9, #2
3000af3a:	f000 8233 	beq.w	3000b3a4 <rxi316_dram_init+0x908>
3000af3e:	f1b9 0f03 	cmp.w	r9, #3
3000af42:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000af44:	f000 8298 	beq.w	3000b478 <rxi316_dram_init+0x9dc>
3000af48:	46a8      	mov	r8, r5
3000af4a:	9d01      	ldr	r5, [sp, #4]
3000af4c:	44a8      	add	r8, r5
3000af4e:	fa28 f50b 	lsr.w	r5, r8, fp
3000af52:	ea18 0f0a 	tst.w	r8, sl
3000af56:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000af5a:	bf18      	it	ne
3000af5c:	3501      	addne	r5, #1
3000af5e:	fbb8 f7f3 	udiv	r7, r8, r3
3000af62:	fb03 8817 	mls	r8, r3, r7, r8
3000af66:	f1b8 0f00 	cmp.w	r8, #0
3000af6a:	d000      	beq.n	3000af6e <rxi316_dram_init+0x4d2>
3000af6c:	3701      	adds	r7, #1
3000af6e:	0109      	lsls	r1, r1, #4
3000af70:	f00e 0e0f 	and.w	lr, lr, #15
3000af74:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000af78:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000af7c:	063f      	lsls	r7, r7, #24
3000af7e:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000af82:	ea41 0e0e 	orr.w	lr, r1, lr
3000af86:	0291      	lsls	r1, r2, #10
3000af88:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000af8c:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000af90:	ea4e 0e01 	orr.w	lr, lr, r1
3000af94:	0471      	lsls	r1, r6, #17
3000af96:	ea4e 0c0c 	orr.w	ip, lr, ip
3000af9a:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000af9e:	ea4c 0c01 	orr.w	ip, ip, r1
3000afa2:	0529      	lsls	r1, r5, #20
3000afa4:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000afa8:	ea4c 0c01 	orr.w	ip, ip, r1
3000afac:	4962      	ldr	r1, [pc, #392]	; (3000b138 <rxi316_dram_init+0x69c>)
3000afae:	ea4c 0707 	orr.w	r7, ip, r7
3000afb2:	618f      	str	r7, [r1, #24]
3000afb4:	6821      	ldr	r1, [r4, #0]
3000afb6:	780f      	ldrb	r7, [r1, #0]
3000afb8:	2f02      	cmp	r7, #2
3000afba:	f000 8152 	beq.w	3000b262 <rxi316_dram_init+0x7c6>
3000afbe:	2f09      	cmp	r7, #9
3000afc0:	f000 814f 	beq.w	3000b262 <rxi316_dram_init+0x7c6>
3000afc4:	68e1      	ldr	r1, [r4, #12]
3000afc6:	2f03      	cmp	r7, #3
3000afc8:	6809      	ldr	r1, [r1, #0]
3000afca:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000afcc:	f001 080f 	and.w	r8, r1, #15
3000afd0:	68a1      	ldr	r1, [r4, #8]
3000afd2:	6a09      	ldr	r1, [r1, #32]
3000afd4:	d005      	beq.n	3000afe2 <rxi316_dram_init+0x546>
3000afd6:	ea0a 0701 	and.w	r7, sl, r1
3000afda:	fa21 f10b 	lsr.w	r1, r1, fp
3000afde:	b107      	cbz	r7, 3000afe2 <rxi316_dram_init+0x546>
3000afe0:	3101      	adds	r1, #1
3000afe2:	f1b9 0f02 	cmp.w	r9, #2
3000afe6:	f000 8154 	beq.w	3000b292 <rxi316_dram_init+0x7f6>
3000afea:	f1b9 0f03 	cmp.w	r9, #3
3000afee:	f000 81b6 	beq.w	3000b35e <rxi316_dram_init+0x8c2>
3000aff2:	f1b9 0f09 	cmp.w	r9, #9
3000aff6:	f000 822f 	beq.w	3000b458 <rxi316_dram_init+0x9bc>
3000affa:	f04f 0e00 	mov.w	lr, #0
3000affe:	46f1      	mov	r9, lr
3000b000:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b004:	0109      	lsls	r1, r1, #4
3000b006:	9f06      	ldr	r7, [sp, #24]
3000b008:	b2c9      	uxtb	r1, r1
3000b00a:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000b00e:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b010:	ea41 0108 	orr.w	r1, r1, r8
3000b014:	4339      	orrs	r1, r7
3000b016:	4f48      	ldr	r7, [pc, #288]	; (3000b138 <rxi316_dram_init+0x69c>)
3000b018:	ea41 0109 	orr.w	r1, r1, r9
3000b01c:	ea41 010e 	orr.w	r1, r1, lr
3000b020:	61f9      	str	r1, [r7, #28]
3000b022:	68a7      	ldr	r7, [r4, #8]
3000b024:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000b026:	fbb4 f1f3 	udiv	r1, r4, r3
3000b02a:	fb03 4311 	mls	r3, r3, r1, r4
3000b02e:	b103      	cbz	r3, 3000b032 <rxi316_dram_init+0x596>
3000b030:	3101      	adds	r1, #1
3000b032:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000b034:	094b      	lsrs	r3, r1, #5
3000b036:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000b03a:	0912      	lsrs	r2, r2, #4
3000b03c:	ea14 0f0a 	tst.w	r4, sl
3000b040:	fa24 f80b 	lsr.w	r8, r4, fp
3000b044:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000b046:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000b04a:	bf18      	it	ne
3000b04c:	f108 0801 	addne.w	r8, r8, #1
3000b050:	9306      	str	r3, [sp, #24]
3000b052:	ea14 0f0a 	tst.w	r4, sl
3000b056:	fa24 fe0b 	lsr.w	lr, r4, fp
3000b05a:	9b01      	ldr	r3, [sp, #4]
3000b05c:	fa29 f40b 	lsr.w	r4, r9, fp
3000b060:	bf18      	it	ne
3000b062:	f10e 0e01 	addne.w	lr, lr, #1
3000b066:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000b068:	ea19 0f0a 	tst.w	r9, sl
3000b06c:	fa23 f90b 	lsr.w	r9, r3, fp
3000b070:	9905      	ldr	r1, [sp, #20]
3000b072:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000b076:	bf18      	it	ne
3000b078:	3401      	addne	r4, #1
3000b07a:	45b9      	cmp	r9, r7
3000b07c:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000b080:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000b084:	f240 80eb 	bls.w	3000b25e <rxi316_dram_init+0x7c2>
3000b088:	eba9 0707 	sub.w	r7, r9, r7
3000b08c:	fa27 f40b 	lsr.w	r4, r7, fp
3000b090:	ea17 070a 	ands.w	r7, r7, sl
3000b094:	d001      	beq.n	3000b09a <rxi316_dram_init+0x5fe>
3000b096:	3401      	adds	r4, #1
3000b098:	2700      	movs	r7, #0
3000b09a:	9b04      	ldr	r3, [sp, #16]
3000b09c:	b10b      	cbz	r3, 3000b0a2 <rxi316_dram_init+0x606>
3000b09e:	073b      	lsls	r3, r7, #28
3000b0a0:	9304      	str	r3, [sp, #16]
3000b0a2:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000b0a6:	9b06      	ldr	r3, [sp, #24]
3000b0a8:	0289      	lsls	r1, r1, #10
3000b0aa:	02c0      	lsls	r0, r0, #11
3000b0ac:	f00c 0c04 	and.w	ip, ip, #4
3000b0b0:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000b0b4:	01d2      	lsls	r2, r2, #7
3000b0b6:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000b0ba:	0236      	lsls	r6, r6, #8
3000b0bc:	ea4c 0c01 	orr.w	ip, ip, r1
3000b0c0:	b2d2      	uxtb	r2, r2
3000b0c2:	026d      	lsls	r5, r5, #9
3000b0c4:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000b0c8:	ea4c 0c00 	orr.w	ip, ip, r0
3000b0cc:	00df      	lsls	r7, r3, #3
3000b0ce:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000b0d2:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000b0d6:	ea4c 0202 	orr.w	r2, ip, r2
3000b0da:	f007 0708 	and.w	r7, r7, #8
3000b0de:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000b0e2:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000b0e6:	4332      	orrs	r2, r6
3000b0e8:	0524      	lsls	r4, r4, #20
3000b0ea:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000b0ee:	4912      	ldr	r1, [pc, #72]	; (3000b138 <rxi316_dram_init+0x69c>)
3000b0f0:	ea42 0305 	orr.w	r3, r2, r5
3000b0f4:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000b0f8:	9a04      	ldr	r2, [sp, #16]
3000b0fa:	433b      	orrs	r3, r7
3000b0fc:	ea43 0308 	orr.w	r3, r3, r8
3000b100:	ea43 030e 	orr.w	r3, r3, lr
3000b104:	4323      	orrs	r3, r4
3000b106:	4313      	orrs	r3, r2
3000b108:	9a03      	ldr	r2, [sp, #12]
3000b10a:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000b10e:	620b      	str	r3, [r1, #32]
3000b110:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000b114:	9b02      	ldr	r3, [sp, #8]
3000b116:	2b01      	cmp	r3, #1
3000b118:	d002      	beq.n	3000b120 <rxi316_dram_init+0x684>
3000b11a:	9b08      	ldr	r3, [sp, #32]
3000b11c:	3303      	adds	r3, #3
3000b11e:	9307      	str	r3, [sp, #28]
3000b120:	4b05      	ldr	r3, [pc, #20]	; (3000b138 <rxi316_dram_init+0x69c>)
3000b122:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000b126:	9a07      	ldr	r2, [sp, #28]
3000b128:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000b12c:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000b130:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000b134:	e002      	b.n	3000b13c <rxi316_dram_init+0x6a0>
3000b136:	bf00      	nop
3000b138:	41100000 	.word	0x41100000
3000b13c:	60da      	str	r2, [r3, #12]
3000b13e:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b142:	430a      	orrs	r2, r1
3000b144:	49b3      	ldr	r1, [pc, #716]	; (3000b414 <rxi316_dram_init+0x978>)
3000b146:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b14a:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000b14e:	0096      	lsls	r6, r2, #2
3000b150:	d57f      	bpl.n	3000b252 <rxi316_dram_init+0x7b6>
3000b152:	691a      	ldr	r2, [r3, #16]
3000b154:	4cb0      	ldr	r4, [pc, #704]	; (3000b418 <rxi316_dram_init+0x97c>)
3000b156:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000b15a:	611a      	str	r2, [r3, #16]
3000b15c:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000b160:	601a      	str	r2, [r3, #0]
3000b162:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000b166:	f043 0303 	orr.w	r3, r3, #3
3000b16a:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000b16e:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000b172:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000b174:	2b01      	cmp	r3, #1
3000b176:	4ba9      	ldr	r3, [pc, #676]	; (3000b41c <rxi316_dram_init+0x980>)
3000b178:	bf0c      	ite	eq
3000b17a:	4aa9      	ldreq	r2, [pc, #676]	; (3000b420 <rxi316_dram_init+0x984>)
3000b17c:	4aa9      	ldrne	r2, [pc, #676]	; (3000b424 <rxi316_dram_init+0x988>)
3000b17e:	601a      	str	r2, [r3, #0]
3000b180:	2203      	movs	r2, #3
3000b182:	68e0      	ldr	r0, [r4, #12]
3000b184:	4ba8      	ldr	r3, [pc, #672]	; (3000b428 <rxi316_dram_init+0x98c>)
3000b186:	4003      	ands	r3, r0
3000b188:	60e3      	str	r3, [r4, #12]
3000b18a:	2308      	movs	r3, #8
3000b18c:	62a1      	str	r1, [r4, #40]	; 0x28
3000b18e:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b190:	6023      	str	r3, [r4, #0]
3000b192:	6823      	ldr	r3, [r4, #0]
3000b194:	071d      	lsls	r5, r3, #28
3000b196:	d5fc      	bpl.n	3000b192 <rxi316_dram_init+0x6f6>
3000b198:	4ba4      	ldr	r3, [pc, #656]	; (3000b42c <rxi316_dram_init+0x990>)
3000b19a:	2001      	movs	r0, #1
3000b19c:	4798      	blx	r3
3000b19e:	68e2      	ldr	r2, [r4, #12]
3000b1a0:	4ba1      	ldr	r3, [pc, #644]	; (3000b428 <rxi316_dram_init+0x98c>)
3000b1a2:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000b1a6:	4013      	ands	r3, r2
3000b1a8:	2203      	movs	r2, #3
3000b1aa:	60e3      	str	r3, [r4, #12]
3000b1ac:	2308      	movs	r3, #8
3000b1ae:	62a1      	str	r1, [r4, #40]	; 0x28
3000b1b0:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b1b2:	4a99      	ldr	r2, [pc, #612]	; (3000b418 <rxi316_dram_init+0x97c>)
3000b1b4:	6023      	str	r3, [r4, #0]
3000b1b6:	6813      	ldr	r3, [r2, #0]
3000b1b8:	071c      	lsls	r4, r3, #28
3000b1ba:	d5fc      	bpl.n	3000b1b6 <rxi316_dram_init+0x71a>
3000b1bc:	68d0      	ldr	r0, [r2, #12]
3000b1be:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000b1c2:	4b99      	ldr	r3, [pc, #612]	; (3000b428 <rxi316_dram_init+0x98c>)
3000b1c4:	4003      	ands	r3, r0
3000b1c6:	2008      	movs	r0, #8
3000b1c8:	60d3      	str	r3, [r2, #12]
3000b1ca:	6291      	str	r1, [r2, #40]	; 0x28
3000b1cc:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000b1ce:	4992      	ldr	r1, [pc, #584]	; (3000b418 <rxi316_dram_init+0x97c>)
3000b1d0:	f023 0302 	bic.w	r3, r3, #2
3000b1d4:	62d3      	str	r3, [r2, #44]	; 0x2c
3000b1d6:	6010      	str	r0, [r2, #0]
3000b1d8:	680b      	ldr	r3, [r1, #0]
3000b1da:	0718      	lsls	r0, r3, #28
3000b1dc:	d5fc      	bpl.n	3000b1d8 <rxi316_dram_init+0x73c>
3000b1de:	4a8e      	ldr	r2, [pc, #568]	; (3000b418 <rxi316_dram_init+0x97c>)
3000b1e0:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000b1e4:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b1e8:	60d3      	str	r3, [r2, #12]
3000b1ea:	980a      	ldr	r0, [sp, #40]	; 0x28
3000b1ec:	6913      	ldr	r3, [r2, #16]
3000b1ee:	4318      	orrs	r0, r3
3000b1f0:	6110      	str	r0, [r2, #16]
3000b1f2:	6913      	ldr	r3, [r2, #16]
3000b1f4:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000b1f8:	6113      	str	r3, [r2, #16]
3000b1fa:	6011      	str	r1, [r2, #0]
3000b1fc:	68d3      	ldr	r3, [r2, #12]
3000b1fe:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000b202:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b206:	d1f9      	bne.n	3000b1fc <rxi316_dram_init+0x760>
3000b208:	b00f      	add	sp, #60	; 0x3c
3000b20a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b20e:	2a01      	cmp	r2, #1
3000b210:	f000 80d1 	beq.w	3000b3b6 <rxi316_dram_init+0x91a>
3000b214:	2a02      	cmp	r2, #2
3000b216:	f000 8139 	beq.w	3000b48c <rxi316_dram_init+0x9f0>
3000b21a:	2a03      	cmp	r2, #3
3000b21c:	f47f ace4 	bne.w	3000abe8 <rxi316_dram_init+0x14c>
3000b220:	686a      	ldr	r2, [r5, #4]
3000b222:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b226:	606a      	str	r2, [r5, #4]
3000b228:	e4de      	b.n	3000abe8 <rxi316_dram_init+0x14c>
3000b22a:	2a00      	cmp	r2, #0
3000b22c:	f43f ad6a 	beq.w	3000ad04 <rxi316_dram_init+0x268>
3000b230:	2203      	movs	r2, #3
3000b232:	4310      	orrs	r0, r2
3000b234:	68a2      	ldr	r2, [r4, #8]
3000b236:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b238:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b23a:	013f      	lsls	r7, r7, #4
3000b23c:	2d00      	cmp	r5, #0
3000b23e:	f000 80d7 	beq.w	3000b3f0 <rxi316_dram_init+0x954>
3000b242:	1e91      	subs	r1, r2, #2
3000b244:	2906      	cmp	r1, #6
3000b246:	f200 80b0 	bhi.w	3000b3aa <rxi316_dram_init+0x90e>
3000b24a:	1e55      	subs	r5, r2, #1
3000b24c:	2100      	movs	r1, #0
3000b24e:	026d      	lsls	r5, r5, #9
3000b250:	e55f      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b252:	2201      	movs	r2, #1
3000b254:	601a      	str	r2, [r3, #0]
3000b256:	681a      	ldr	r2, [r3, #0]
3000b258:	07d2      	lsls	r2, r2, #31
3000b25a:	d5fc      	bpl.n	3000b256 <rxi316_dram_init+0x7ba>
3000b25c:	e7bf      	b.n	3000b1de <rxi316_dram_init+0x742>
3000b25e:	2701      	movs	r7, #1
3000b260:	e71b      	b.n	3000b09a <rxi316_dram_init+0x5fe>
3000b262:	68a1      	ldr	r1, [r4, #8]
3000b264:	2f03      	cmp	r7, #3
3000b266:	6a09      	ldr	r1, [r1, #32]
3000b268:	f000 80bf 	beq.w	3000b3ea <rxi316_dram_init+0x94e>
3000b26c:	2f02      	cmp	r7, #2
3000b26e:	f040 8118 	bne.w	3000b4a2 <rxi316_dram_init+0xa06>
3000b272:	fbb1 fcf3 	udiv	ip, r1, r3
3000b276:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b27a:	4661      	mov	r1, ip
3000b27c:	f1be 0f00 	cmp.w	lr, #0
3000b280:	f000 80a9 	beq.w	3000b3d6 <rxi316_dram_init+0x93a>
3000b284:	f1b9 0f02 	cmp.w	r9, #2
3000b288:	f101 0101 	add.w	r1, r1, #1
3000b28c:	46b8      	mov	r8, r7
3000b28e:	f47f aeac 	bne.w	3000afea <rxi316_dram_init+0x54e>
3000b292:	4f67      	ldr	r7, [pc, #412]	; (3000b430 <rxi316_dram_init+0x994>)
3000b294:	f04f 0e00 	mov.w	lr, #0
3000b298:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b434 <rxi316_dram_init+0x998>
3000b29c:	fbb7 f7f3 	udiv	r7, r7, r3
3000b2a0:	fbb9 f9f3 	udiv	r9, r9, r3
3000b2a4:	09ff      	lsrs	r7, r7, #7
3000b2a6:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b2aa:	3701      	adds	r7, #1
3000b2ac:	f109 0c01 	add.w	ip, r9, #1
3000b2b0:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b438 <rxi316_dram_init+0x99c>
3000b2b4:	023f      	lsls	r7, r7, #8
3000b2b6:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b2ba:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b2be:	9709      	str	r7, [sp, #36]	; 0x24
3000b2c0:	e6a0      	b.n	3000b004 <rxi316_dram_init+0x568>
3000b2c2:	f8cd c018 	str.w	ip, [sp, #24]
3000b2c6:	e5bf      	b.n	3000ae48 <rxi316_dram_init+0x3ac>
3000b2c8:	38c8      	subs	r0, #200	; 0xc8
3000b2ca:	e596      	b.n	3000adfa <rxi316_dram_init+0x35e>
3000b2cc:	9105      	str	r1, [sp, #20]
3000b2ce:	e58b      	b.n	3000ade8 <rxi316_dram_init+0x34c>
3000b2d0:	fa22 f00b 	lsr.w	r0, r2, fp
3000b2d4:	e547      	b.n	3000ad66 <rxi316_dram_init+0x2ca>
3000b2d6:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000b2da:	ea1a 0f07 	tst.w	sl, r7
3000b2de:	fa27 f20b 	lsr.w	r2, r7, fp
3000b2e2:	d000      	beq.n	3000b2e6 <rxi316_dram_init+0x84a>
3000b2e4:	3201      	adds	r2, #1
3000b2e6:	2e02      	cmp	r6, #2
3000b2e8:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b2ec:	d10f      	bne.n	3000b30e <rxi316_dram_init+0x872>
3000b2ee:	9e08      	ldr	r6, [sp, #32]
3000b2f0:	3e02      	subs	r6, #2
3000b2f2:	fa26 f60b 	lsr.w	r6, r6, fp
3000b2f6:	4437      	add	r7, r6
3000b2f8:	fbb7 f6f3 	udiv	r6, r7, r3
3000b2fc:	fb03 7716 	mls	r7, r3, r6, r7
3000b300:	2f00      	cmp	r7, #0
3000b302:	f43f ad52 	beq.w	3000adaa <rxi316_dram_init+0x30e>
3000b306:	3601      	adds	r6, #1
3000b308:	e54f      	b.n	3000adaa <rxi316_dram_init+0x30e>
3000b30a:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b30e:	fbb7 f6f3 	udiv	r6, r7, r3
3000b312:	fb03 7716 	mls	r7, r3, r6, r7
3000b316:	2f00      	cmp	r7, #0
3000b318:	f43f ad47 	beq.w	3000adaa <rxi316_dram_init+0x30e>
3000b31c:	3601      	adds	r6, #1
3000b31e:	e544      	b.n	3000adaa <rxi316_dram_init+0x30e>
3000b320:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b322:	46a8      	mov	r8, r5
3000b324:	9d08      	ldr	r5, [sp, #32]
3000b326:	44a8      	add	r8, r5
3000b328:	e611      	b.n	3000af4e <rxi316_dram_init+0x4b2>
3000b32a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b32c:	1f50      	subs	r0, r2, #5
3000b32e:	00c0      	lsls	r0, r0, #3
3000b330:	68e2      	ldr	r2, [r4, #12]
3000b332:	6812      	ldr	r2, [r2, #0]
3000b334:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b336:	fab2 f282 	clz	r2, r2
3000b33a:	0952      	lsrs	r2, r2, #5
3000b33c:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b340:	4301      	orrs	r1, r0
3000b342:	e4fd      	b.n	3000ad40 <rxi316_dram_init+0x2a4>
3000b344:	f1a1 0502 	sub.w	r5, r1, #2
3000b348:	2909      	cmp	r1, #9
3000b34a:	fab5 f585 	clz	r5, r5
3000b34e:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b352:	f000 8099 	beq.w	3000b488 <rxi316_dram_init+0x9ec>
3000b356:	462a      	mov	r2, r5
3000b358:	e4d1      	b.n	3000acfe <rxi316_dram_init+0x262>
3000b35a:	2000      	movs	r0, #0
3000b35c:	e7e8      	b.n	3000b330 <rxi316_dram_init+0x894>
3000b35e:	f242 7710 	movw	r7, #10000	; 0x2710
3000b362:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b43c <rxi316_dram_init+0x9a0>
3000b366:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b434 <rxi316_dram_init+0x998>
3000b36a:	fbb7 fcf3 	udiv	ip, r7, r3
3000b36e:	9f05      	ldr	r7, [sp, #20]
3000b370:	4467      	add	r7, ip
3000b372:	fbb9 f9f3 	udiv	r9, r9, r3
3000b376:	fbbe fef3 	udiv	lr, lr, r3
3000b37a:	09ff      	lsrs	r7, r7, #7
3000b37c:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b380:	3701      	adds	r7, #1
3000b382:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b386:	f109 0c01 	add.w	ip, r9, #1
3000b38a:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b438 <rxi316_dram_init+0x99c>
3000b38e:	023f      	lsls	r7, r7, #8
3000b390:	f10e 0e01 	add.w	lr, lr, #1
3000b394:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b398:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b39c:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b3a0:	9709      	str	r7, [sp, #36]	; 0x24
3000b3a2:	e62f      	b.n	3000b004 <rxi316_dram_init+0x568>
3000b3a4:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b3a8:	e5d1      	b.n	3000af4e <rxi316_dram_init+0x4b2>
3000b3aa:	2100      	movs	r1, #0
3000b3ac:	460d      	mov	r5, r1
3000b3ae:	e4b0      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b3b0:	3a07      	subs	r2, #7
3000b3b2:	f7ff bbb8 	b.w	3000ab26 <rxi316_dram_init+0x8a>
3000b3b6:	686a      	ldr	r2, [r5, #4]
3000b3b8:	f042 0220 	orr.w	r2, r2, #32
3000b3bc:	606a      	str	r2, [r5, #4]
3000b3be:	e413      	b.n	3000abe8 <rxi316_dram_init+0x14c>
3000b3c0:	2204      	movs	r2, #4
3000b3c2:	2102      	movs	r1, #2
3000b3c4:	2500      	movs	r5, #0
3000b3c6:	9201      	str	r2, [sp, #4]
3000b3c8:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b3cc:	f7ff bb94 	b.w	3000aaf8 <rxi316_dram_init+0x5c>
3000b3d0:	3a08      	subs	r2, #8
3000b3d2:	f7ff bba8 	b.w	3000ab26 <rxi316_dram_init+0x8a>
3000b3d6:	46b8      	mov	r8, r7
3000b3d8:	e603      	b.n	3000afe2 <rxi316_dram_init+0x546>
3000b3da:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b3dc:	2a04      	cmp	r2, #4
3000b3de:	d90c      	bls.n	3000b3fa <rxi316_dram_init+0x95e>
3000b3e0:	2a0b      	cmp	r2, #11
3000b3e2:	d86a      	bhi.n	3000b4ba <rxi316_dram_init+0xa1e>
3000b3e4:	1f17      	subs	r7, r2, #4
3000b3e6:	013f      	lsls	r7, r7, #4
3000b3e8:	e008      	b.n	3000b3fc <rxi316_dram_init+0x960>
3000b3ea:	f04f 0802 	mov.w	r8, #2
3000b3ee:	e5f8      	b.n	3000afe2 <rxi316_dram_init+0x546>
3000b3f0:	2903      	cmp	r1, #3
3000b3f2:	d005      	beq.n	3000b400 <rxi316_dram_init+0x964>
3000b3f4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b3f8:	e48b      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b3fa:	2710      	movs	r7, #16
3000b3fc:	68a2      	ldr	r2, [r4, #8]
3000b3fe:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b400:	2a04      	cmp	r2, #4
3000b402:	d949      	bls.n	3000b498 <rxi316_dram_init+0x9fc>
3000b404:	1f51      	subs	r1, r2, #5
3000b406:	2903      	cmp	r1, #3
3000b408:	d84e      	bhi.n	3000b4a8 <rxi316_dram_init+0xa0c>
3000b40a:	1f15      	subs	r5, r2, #4
3000b40c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b410:	026d      	lsls	r5, r5, #9
3000b412:	e47e      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b414:	42008000 	.word	0x42008000
3000b418:	41100000 	.word	0x41100000
3000b41c:	41011000 	.word	0x41011000
3000b420:	44000103 	.word	0x44000103
3000b424:	44000021 	.word	0x44000021
3000b428:	fff8fffc 	.word	0xfff8fffc
3000b42c:	00009b2d 	.word	0x00009b2d
3000b430:	00061a80 	.word	0x00061a80
3000b434:	0bebc200 	.word	0x0bebc200
3000b438:	003ff000 	.word	0x003ff000
3000b43c:	1dcd6500 	.word	0x1dcd6500
3000b440:	f1a1 0c02 	sub.w	ip, r1, #2
3000b444:	f1bc 0f01 	cmp.w	ip, #1
3000b448:	d903      	bls.n	3000b452 <rxi316_dram_init+0x9b6>
3000b44a:	f1be 0f00 	cmp.w	lr, #0
3000b44e:	f43f ac56 	beq.w	3000acfe <rxi316_dram_init+0x262>
3000b452:	f040 0002 	orr.w	r0, r0, #2
3000b456:	e452      	b.n	3000acfe <rxi316_dram_init+0x262>
3000b458:	4f1d      	ldr	r7, [pc, #116]	; (3000b4d0 <rxi316_dram_init+0xa34>)
3000b45a:	f04f 0e00 	mov.w	lr, #0
3000b45e:	fbb7 f9f3 	udiv	r9, r7, r3
3000b462:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b466:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b46a:	f109 0701 	add.w	r7, r9, #1
3000b46e:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b4d4 <rxi316_dram_init+0xa38>
3000b472:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b476:	e5c5      	b.n	3000b004 <rxi316_dram_init+0x568>
3000b478:	f105 0802 	add.w	r8, r5, #2
3000b47c:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b47e:	eba8 0805 	sub.w	r8, r8, r5
3000b482:	9d08      	ldr	r5, [sp, #32]
3000b484:	44a8      	add	r8, r5
3000b486:	e562      	b.n	3000af4e <rxi316_dram_init+0x4b2>
3000b488:	2204      	movs	r2, #4
3000b48a:	e6d2      	b.n	3000b232 <rxi316_dram_init+0x796>
3000b48c:	686a      	ldr	r2, [r5, #4]
3000b48e:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b492:	606a      	str	r2, [r5, #4]
3000b494:	f7ff bba8 	b.w	3000abe8 <rxi316_dram_init+0x14c>
3000b498:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b49c:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b4a0:	e437      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b4a2:	f04f 0802 	mov.w	r8, #2
3000b4a6:	e596      	b.n	3000afd6 <rxi316_dram_init+0x53a>
3000b4a8:	f1a2 0109 	sub.w	r1, r2, #9
3000b4ac:	2905      	cmp	r1, #5
3000b4ae:	d80b      	bhi.n	3000b4c8 <rxi316_dram_init+0xa2c>
3000b4b0:	0855      	lsrs	r5, r2, #1
3000b4b2:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4b6:	026d      	lsls	r5, r5, #9
3000b4b8:	e42b      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b4ba:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b4bc:	f040 0004 	orr.w	r0, r0, #4
3000b4c0:	f1a2 070c 	sub.w	r7, r2, #12
3000b4c4:	013f      	lsls	r7, r7, #4
3000b4c6:	e799      	b.n	3000b3fc <rxi316_dram_init+0x960>
3000b4c8:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b4cc:	2500      	movs	r5, #0
3000b4ce:	e420      	b.n	3000ad12 <rxi316_dram_init+0x276>
3000b4d0:	0bebc200 	.word	0x0bebc200
3000b4d4:	003ff000 	.word	0x003ff000

3000b4d8 <rxi316_ftend_init>:
3000b4d8:	2300      	movs	r3, #0
3000b4da:	b510      	push	{r4, lr}
3000b4dc:	4c0c      	ldr	r4, [pc, #48]	; (3000b510 <rxi316_ftend_init+0x38>)
3000b4de:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b4e2:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b4e6:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b4ea:	f7fe fe67 	bl	3000a1bc <ddr_init_index>
3000b4ee:	4a09      	ldr	r2, [pc, #36]	; (3000b514 <rxi316_ftend_init+0x3c>)
3000b4f0:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b4f4:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b4f8:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b4fc:	68cb      	ldr	r3, [r1, #12]
3000b4fe:	689b      	ldr	r3, [r3, #8]
3000b500:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b502:	3301      	adds	r3, #1
3000b504:	b2db      	uxtb	r3, r3
3000b506:	4313      	orrs	r3, r2
3000b508:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b50c:	bd10      	pop	{r4, pc}
3000b50e:	bf00      	nop
3000b510:	41101000 	.word	0x41101000
3000b514:	3000f814 	.word	0x3000f814

3000b518 <rxi316_DynSre_init>:
3000b518:	2901      	cmp	r1, #1
3000b51a:	d013      	beq.n	3000b544 <rxi316_DynSre_init+0x2c>
3000b51c:	4a12      	ldr	r2, [pc, #72]	; (3000b568 <rxi316_DynSre_init+0x50>)
3000b51e:	6893      	ldr	r3, [r2, #8]
3000b520:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b524:	6093      	str	r3, [r2, #8]
3000b526:	4b10      	ldr	r3, [pc, #64]	; (3000b568 <rxi316_DynSre_init+0x50>)
3000b528:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b52c:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b530:	625a      	str	r2, [r3, #36]	; 0x24
3000b532:	629a      	str	r2, [r3, #40]	; 0x28
3000b534:	69da      	ldr	r2, [r3, #28]
3000b536:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b53a:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b53e:	61da      	str	r2, [r3, #28]
3000b540:	6019      	str	r1, [r3, #0]
3000b542:	4770      	bx	lr
3000b544:	4b08      	ldr	r3, [pc, #32]	; (3000b568 <rxi316_DynSre_init+0x50>)
3000b546:	b280      	uxth	r0, r0
3000b548:	4a08      	ldr	r2, [pc, #32]	; (3000b56c <rxi316_DynSre_init+0x54>)
3000b54a:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b54e:	400a      	ands	r2, r1
3000b550:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b554:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b558:	4310      	orrs	r0, r2
3000b55a:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b55e:	689a      	ldr	r2, [r3, #8]
3000b560:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b564:	609a      	str	r2, [r3, #8]
3000b566:	e7de      	b.n	3000b526 <rxi316_DynSre_init+0xe>
3000b568:	41100000 	.word	0x41100000
3000b56c:	ffff0000 	.word	0xffff0000

3000b570 <ddr_init>:
3000b570:	b500      	push	{lr}
3000b572:	b083      	sub	sp, #12
3000b574:	f7fe fe22 	bl	3000a1bc <ddr_init_index>
3000b578:	4b06      	ldr	r3, [pc, #24]	; (3000b594 <ddr_init+0x24>)
3000b57a:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b57e:	9001      	str	r0, [sp, #4]
3000b580:	f7fe fe46 	bl	3000a210 <rxi316_perf_tune>
3000b584:	9801      	ldr	r0, [sp, #4]
3000b586:	f7ff fa89 	bl	3000aa9c <rxi316_dram_init>
3000b58a:	b003      	add	sp, #12
3000b58c:	f85d eb04 	ldr.w	lr, [sp], #4
3000b590:	f7ff bfa2 	b.w	3000b4d8 <rxi316_ftend_init>
3000b594:	3000f814 	.word	0x3000f814

3000b598 <SWR_MEM>:
3000b598:	2801      	cmp	r0, #1
3000b59a:	d00e      	beq.n	3000b5ba <SWR_MEM+0x22>
3000b59c:	4b31      	ldr	r3, [pc, #196]	; (3000b664 <SWR_MEM+0xcc>)
3000b59e:	2000      	movs	r0, #0
3000b5a0:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b5a4:	f022 0201 	bic.w	r2, r2, #1
3000b5a8:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b5ac:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b5b0:	f022 0202 	bic.w	r2, r2, #2
3000b5b4:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b5b8:	4770      	bx	lr
3000b5ba:	b510      	push	{r4, lr}
3000b5bc:	4c29      	ldr	r4, [pc, #164]	; (3000b664 <SWR_MEM+0xcc>)
3000b5be:	4a2a      	ldr	r2, [pc, #168]	; (3000b668 <SWR_MEM+0xd0>)
3000b5c0:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b5c4:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b5c8:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b5cc:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b5d0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b5d4:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b5d8:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b5dc:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b5e0:	f023 0303 	bic.w	r3, r3, #3
3000b5e4:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b5e8:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b5ec:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b5f0:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b5f4:	4790      	blx	r2
3000b5f6:	2803      	cmp	r0, #3
3000b5f8:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b5fc:	d026      	beq.n	3000b64c <SWR_MEM+0xb4>
3000b5fe:	f043 0303 	orr.w	r3, r3, #3
3000b602:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b606:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b60a:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b60e:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b612:	4c14      	ldr	r4, [pc, #80]	; (3000b664 <SWR_MEM+0xcc>)
3000b614:	4b15      	ldr	r3, [pc, #84]	; (3000b66c <SWR_MEM+0xd4>)
3000b616:	4798      	blx	r3
3000b618:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b61c:	4a14      	ldr	r2, [pc, #80]	; (3000b670 <SWR_MEM+0xd8>)
3000b61e:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b622:	f043 0302 	orr.w	r3, r3, #2
3000b626:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b62a:	4790      	blx	r2
3000b62c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b630:	4b10      	ldr	r3, [pc, #64]	; (3000b674 <SWR_MEM+0xdc>)
3000b632:	f042 0201 	orr.w	r2, r2, #1
3000b636:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b63a:	e001      	b.n	3000b640 <SWR_MEM+0xa8>
3000b63c:	3b01      	subs	r3, #1
3000b63e:	d00e      	beq.n	3000b65e <SWR_MEM+0xc6>
3000b640:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b644:	03d2      	lsls	r2, r2, #15
3000b646:	d5f9      	bpl.n	3000b63c <SWR_MEM+0xa4>
3000b648:	2000      	movs	r0, #0
3000b64a:	bd10      	pop	{r4, pc}
3000b64c:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b650:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b654:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b658:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b65c:	e7d9      	b.n	3000b612 <SWR_MEM+0x7a>
3000b65e:	2001      	movs	r0, #1
3000b660:	bd10      	pop	{r4, pc}
3000b662:	bf00      	nop
3000b664:	42008800 	.word	0x42008800
3000b668:	30007e31 	.word	0x30007e31
3000b66c:	3000b851 	.word	0x3000b851
3000b670:	00009b2d 	.word	0x00009b2d
3000b674:	000186a0 	.word	0x000186a0

3000b678 <SWR_MEM_Manual>:
3000b678:	4a06      	ldr	r2, [pc, #24]	; (3000b694 <SWR_MEM_Manual+0x1c>)
3000b67a:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b67e:	b120      	cbz	r0, 3000b68a <SWR_MEM_Manual+0x12>
3000b680:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b684:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b688:	4770      	bx	lr
3000b68a:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b68e:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b692:	4770      	bx	lr
3000b694:	42008800 	.word	0x42008800

3000b698 <SWR_MEM_Mode_Set>:
3000b698:	2801      	cmp	r0, #1
3000b69a:	b538      	push	{r3, r4, r5, lr}
3000b69c:	d807      	bhi.n	3000b6ae <SWR_MEM_Mode_Set+0x16>
3000b69e:	d10a      	bne.n	3000b6b6 <SWR_MEM_Mode_Set+0x1e>
3000b6a0:	4c22      	ldr	r4, [pc, #136]	; (3000b72c <SWR_MEM_Mode_Set+0x94>)
3000b6a2:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b6a6:	035b      	lsls	r3, r3, #13
3000b6a8:	d51b      	bpl.n	3000b6e2 <SWR_MEM_Mode_Set+0x4a>
3000b6aa:	2000      	movs	r0, #0
3000b6ac:	bd38      	pop	{r3, r4, r5, pc}
3000b6ae:	217e      	movs	r1, #126	; 0x7e
3000b6b0:	481f      	ldr	r0, [pc, #124]	; (3000b730 <SWR_MEM_Mode_Set+0x98>)
3000b6b2:	f001 f829 	bl	3000c708 <__io_assert_failed_veneer>
3000b6b6:	491d      	ldr	r1, [pc, #116]	; (3000b72c <SWR_MEM_Mode_Set+0x94>)
3000b6b8:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b6bc:	0358      	lsls	r0, r3, #13
3000b6be:	d5f4      	bpl.n	3000b6aa <SWR_MEM_Mode_Set+0x12>
3000b6c0:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b6c4:	f242 7310 	movw	r3, #10000	; 0x2710
3000b6c8:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b6cc:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b6d0:	e001      	b.n	3000b6d6 <SWR_MEM_Mode_Set+0x3e>
3000b6d2:	3b01      	subs	r3, #1
3000b6d4:	d027      	beq.n	3000b726 <SWR_MEM_Mode_Set+0x8e>
3000b6d6:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b6da:	0352      	lsls	r2, r2, #13
3000b6dc:	d4f9      	bmi.n	3000b6d2 <SWR_MEM_Mode_Set+0x3a>
3000b6de:	2000      	movs	r0, #0
3000b6e0:	e7e4      	b.n	3000b6ac <SWR_MEM_Mode_Set+0x14>
3000b6e2:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b6e6:	2064      	movs	r0, #100	; 0x64
3000b6e8:	4d12      	ldr	r5, [pc, #72]	; (3000b734 <SWR_MEM_Mode_Set+0x9c>)
3000b6ea:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b6ee:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b6f2:	47a8      	blx	r5
3000b6f4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6f8:	2064      	movs	r0, #100	; 0x64
3000b6fa:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b6fe:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b702:	47a8      	blx	r5
3000b704:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b708:	f242 7310 	movw	r3, #10000	; 0x2710
3000b70c:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b710:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b714:	e001      	b.n	3000b71a <SWR_MEM_Mode_Set+0x82>
3000b716:	3b01      	subs	r3, #1
3000b718:	d005      	beq.n	3000b726 <SWR_MEM_Mode_Set+0x8e>
3000b71a:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b71e:	0355      	lsls	r5, r2, #13
3000b720:	d5f9      	bpl.n	3000b716 <SWR_MEM_Mode_Set+0x7e>
3000b722:	2000      	movs	r0, #0
3000b724:	e7c2      	b.n	3000b6ac <SWR_MEM_Mode_Set+0x14>
3000b726:	2001      	movs	r0, #1
3000b728:	bd38      	pop	{r3, r4, r5, pc}
3000b72a:	bf00      	nop
3000b72c:	42008800 	.word	0x42008800
3000b730:	3000dc24 	.word	0x3000dc24
3000b734:	00009b2d 	.word	0x00009b2d

3000b738 <SWR_AUDIO>:
3000b738:	2801      	cmp	r0, #1
3000b73a:	d00e      	beq.n	3000b75a <SWR_AUDIO+0x22>
3000b73c:	4b1e      	ldr	r3, [pc, #120]	; (3000b7b8 <SWR_AUDIO+0x80>)
3000b73e:	2000      	movs	r0, #0
3000b740:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b744:	f022 0201 	bic.w	r2, r2, #1
3000b748:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b74c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b750:	f022 0202 	bic.w	r2, r2, #2
3000b754:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b758:	4770      	bx	lr
3000b75a:	b510      	push	{r4, lr}
3000b75c:	4c16      	ldr	r4, [pc, #88]	; (3000b7b8 <SWR_AUDIO+0x80>)
3000b75e:	4b17      	ldr	r3, [pc, #92]	; (3000b7bc <SWR_AUDIO+0x84>)
3000b760:	4798      	blx	r3
3000b762:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b766:	4a16      	ldr	r2, [pc, #88]	; (3000b7c0 <SWR_AUDIO+0x88>)
3000b768:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b76c:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b770:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b774:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b778:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b77c:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b780:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b784:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b788:	f043 0302 	orr.w	r3, r3, #2
3000b78c:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b790:	4790      	blx	r2
3000b792:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b796:	4b0b      	ldr	r3, [pc, #44]	; (3000b7c4 <SWR_AUDIO+0x8c>)
3000b798:	f042 0201 	orr.w	r2, r2, #1
3000b79c:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b7a0:	e001      	b.n	3000b7a6 <SWR_AUDIO+0x6e>
3000b7a2:	3b01      	subs	r3, #1
3000b7a4:	d005      	beq.n	3000b7b2 <SWR_AUDIO+0x7a>
3000b7a6:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b7aa:	03d2      	lsls	r2, r2, #15
3000b7ac:	d5f9      	bpl.n	3000b7a2 <SWR_AUDIO+0x6a>
3000b7ae:	2000      	movs	r0, #0
3000b7b0:	bd10      	pop	{r4, pc}
3000b7b2:	2001      	movs	r0, #1
3000b7b4:	bd10      	pop	{r4, pc}
3000b7b6:	bf00      	nop
3000b7b8:	42008800 	.word	0x42008800
3000b7bc:	3000b9ad 	.word	0x3000b9ad
3000b7c0:	00009b2d 	.word	0x00009b2d
3000b7c4:	000186a0 	.word	0x000186a0

3000b7c8 <SWR_AUDIO_Manual>:
3000b7c8:	4a06      	ldr	r2, [pc, #24]	; (3000b7e4 <SWR_AUDIO_Manual+0x1c>)
3000b7ca:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b7ce:	b120      	cbz	r0, 3000b7da <SWR_AUDIO_Manual+0x12>
3000b7d0:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b7d4:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b7d8:	4770      	bx	lr
3000b7da:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b7de:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b7e2:	4770      	bx	lr
3000b7e4:	42008800 	.word	0x42008800

3000b7e8 <SWR_BST_MODE_Set>:
3000b7e8:	4a06      	ldr	r2, [pc, #24]	; (3000b804 <SWR_BST_MODE_Set+0x1c>)
3000b7ea:	6813      	ldr	r3, [r2, #0]
3000b7ec:	b118      	cbz	r0, 3000b7f6 <SWR_BST_MODE_Set+0xe>
3000b7ee:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b7f2:	6013      	str	r3, [r2, #0]
3000b7f4:	4770      	bx	lr
3000b7f6:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b7fa:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b7fe:	6013      	str	r3, [r2, #0]
3000b800:	4770      	bx	lr
3000b802:	bf00      	nop
3000b804:	42008100 	.word	0x42008100

3000b808 <SWR_Calib_DCore>:
3000b808:	4b0f      	ldr	r3, [pc, #60]	; (3000b848 <SWR_Calib_DCore+0x40>)
3000b80a:	f240 3231 	movw	r2, #817	; 0x331
3000b80e:	490f      	ldr	r1, [pc, #60]	; (3000b84c <SWR_Calib_DCore+0x44>)
3000b810:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b812:	4001      	ands	r1, r0
3000b814:	430a      	orrs	r2, r1
3000b816:	651a      	str	r2, [r3, #80]	; 0x50
3000b818:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b81a:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b81e:	641a      	str	r2, [r3, #64]	; 0x40
3000b820:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b824:	f042 0201 	orr.w	r2, r2, #1
3000b828:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b82c:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b830:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b834:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b838:	685a      	ldr	r2, [r3, #4]
3000b83a:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b83e:	f042 0204 	orr.w	r2, r2, #4
3000b842:	605a      	str	r2, [r3, #4]
3000b844:	4770      	bx	lr
3000b846:	bf00      	nop
3000b848:	42008100 	.word	0x42008100
3000b84c:	fffffccc 	.word	0xfffffccc

3000b850 <SWR_Calib_MEM>:
3000b850:	4b51      	ldr	r3, [pc, #324]	; (3000b998 <SWR_Calib_MEM+0x148>)
3000b852:	f240 3231 	movw	r2, #817	; 0x331
3000b856:	4951      	ldr	r1, [pc, #324]	; (3000b99c <SWR_Calib_MEM+0x14c>)
3000b858:	f04f 0c00 	mov.w	ip, #0
3000b85c:	b510      	push	{r4, lr}
3000b85e:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b862:	b082      	sub	sp, #8
3000b864:	4001      	ands	r1, r0
3000b866:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b86a:	430a      	orrs	r2, r1
3000b86c:	f10d 0107 	add.w	r1, sp, #7
3000b870:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b874:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b878:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b87c:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b880:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b884:	f042 0201 	orr.w	r2, r2, #1
3000b888:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b88c:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b890:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b894:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b898:	f88d c007 	strb.w	ip, [sp, #7]
3000b89c:	f7fd f97e 	bl	30008b9c <OTP_Read8>
3000b8a0:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b8a4:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b8a8:	2af0      	cmp	r2, #240	; 0xf0
3000b8aa:	d156      	bne.n	3000b95a <SWR_Calib_MEM+0x10a>
3000b8ac:	f003 030f 	and.w	r3, r3, #15
3000b8b0:	2b0f      	cmp	r3, #15
3000b8b2:	d04e      	beq.n	3000b952 <SWR_Calib_MEM+0x102>
3000b8b4:	2302      	movs	r3, #2
3000b8b6:	4c3a      	ldr	r4, [pc, #232]	; (3000b9a0 <SWR_Calib_MEM+0x150>)
3000b8b8:	f10d 0106 	add.w	r1, sp, #6
3000b8bc:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b8c0:	7023      	strb	r3, [r4, #0]
3000b8c2:	f7fd f96b 	bl	30008b9c <OTP_Read8>
3000b8c6:	7822      	ldrb	r2, [r4, #0]
3000b8c8:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b8cc:	2a03      	cmp	r2, #3
3000b8ce:	d05a      	beq.n	3000b986 <SWR_Calib_MEM+0x136>
3000b8d0:	2a02      	cmp	r2, #2
3000b8d2:	bf0c      	ite	eq
3000b8d4:	f003 030f 	andeq.w	r3, r3, #15
3000b8d8:	2300      	movne	r3, #0
3000b8da:	492f      	ldr	r1, [pc, #188]	; (3000b998 <SWR_Calib_MEM+0x148>)
3000b8dc:	4831      	ldr	r0, [pc, #196]	; (3000b9a4 <SWR_Calib_MEM+0x154>)
3000b8de:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b8e2:	f022 020f 	bic.w	r2, r2, #15
3000b8e6:	4313      	orrs	r3, r2
3000b8e8:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b8ec:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b8ee:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b8f2:	2b07      	cmp	r3, #7
3000b8f4:	d03a      	beq.n	3000b96c <SWR_Calib_MEM+0x11c>
3000b8f6:	2300      	movs	r3, #0
3000b8f8:	f10d 0107 	add.w	r1, sp, #7
3000b8fc:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b900:	f88d 3007 	strb.w	r3, [sp, #7]
3000b904:	f7fd f94a 	bl	30008b9c <OTP_Read8>
3000b908:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b90c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b910:	2af0      	cmp	r2, #240	; 0xf0
3000b912:	d124      	bne.n	3000b95e <SWR_Calib_MEM+0x10e>
3000b914:	f003 030f 	and.w	r3, r3, #15
3000b918:	2b0f      	cmp	r3, #15
3000b91a:	d022      	beq.n	3000b962 <SWR_Calib_MEM+0x112>
3000b91c:	2302      	movs	r3, #2
3000b91e:	4c22      	ldr	r4, [pc, #136]	; (3000b9a8 <SWR_Calib_MEM+0x158>)
3000b920:	f10d 0107 	add.w	r1, sp, #7
3000b924:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b928:	7023      	strb	r3, [r4, #0]
3000b92a:	f7fd f937 	bl	30008b9c <OTP_Read8>
3000b92e:	7822      	ldrb	r2, [r4, #0]
3000b930:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b934:	2a03      	cmp	r2, #3
3000b936:	d028      	beq.n	3000b98a <SWR_Calib_MEM+0x13a>
3000b938:	2a02      	cmp	r2, #2
3000b93a:	d029      	beq.n	3000b990 <SWR_Calib_MEM+0x140>
3000b93c:	2200      	movs	r2, #0
3000b93e:	4916      	ldr	r1, [pc, #88]	; (3000b998 <SWR_Calib_MEM+0x148>)
3000b940:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b944:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b948:	4313      	orrs	r3, r2
3000b94a:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b94e:	b002      	add	sp, #8
3000b950:	bd10      	pop	{r4, pc}
3000b952:	4b13      	ldr	r3, [pc, #76]	; (3000b9a0 <SWR_Calib_MEM+0x150>)
3000b954:	2201      	movs	r2, #1
3000b956:	701a      	strb	r2, [r3, #0]
3000b958:	e7cd      	b.n	3000b8f6 <SWR_Calib_MEM+0xa6>
3000b95a:	2303      	movs	r3, #3
3000b95c:	e7ab      	b.n	3000b8b6 <SWR_Calib_MEM+0x66>
3000b95e:	2303      	movs	r3, #3
3000b960:	e7dd      	b.n	3000b91e <SWR_Calib_MEM+0xce>
3000b962:	4b11      	ldr	r3, [pc, #68]	; (3000b9a8 <SWR_Calib_MEM+0x158>)
3000b964:	2201      	movs	r2, #1
3000b966:	701a      	strb	r2, [r3, #0]
3000b968:	b002      	add	sp, #8
3000b96a:	bd10      	pop	{r4, pc}
3000b96c:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b96e:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b972:	d1c0      	bne.n	3000b8f6 <SWR_Calib_MEM+0xa6>
3000b974:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b978:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b97c:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b980:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b984:	e7b7      	b.n	3000b8f6 <SWR_Calib_MEM+0xa6>
3000b986:	091b      	lsrs	r3, r3, #4
3000b988:	e7a7      	b.n	3000b8da <SWR_Calib_MEM+0x8a>
3000b98a:	091b      	lsrs	r3, r3, #4
3000b98c:	059a      	lsls	r2, r3, #22
3000b98e:	e7d6      	b.n	3000b93e <SWR_Calib_MEM+0xee>
3000b990:	f003 030f 	and.w	r3, r3, #15
3000b994:	059a      	lsls	r2, r3, #22
3000b996:	e7d2      	b.n	3000b93e <SWR_Calib_MEM+0xee>
3000b998:	42008800 	.word	0x42008800
3000b99c:	fffffccc 	.word	0xfffffccc
3000b9a0:	3000f9af 	.word	0x3000f9af
3000b9a4:	42008100 	.word	0x42008100
3000b9a8:	3000f9ae 	.word	0x3000f9ae

3000b9ac <SWR_Calib_AUD>:
3000b9ac:	4b48      	ldr	r3, [pc, #288]	; (3000bad0 <SWR_Calib_AUD+0x124>)
3000b9ae:	f240 3231 	movw	r2, #817	; 0x331
3000b9b2:	4948      	ldr	r1, [pc, #288]	; (3000bad4 <SWR_Calib_AUD+0x128>)
3000b9b4:	f04f 0c00 	mov.w	ip, #0
3000b9b8:	b510      	push	{r4, lr}
3000b9ba:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000b9be:	b082      	sub	sp, #8
3000b9c0:	4001      	ands	r1, r0
3000b9c2:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b9c6:	430a      	orrs	r2, r1
3000b9c8:	f10d 0107 	add.w	r1, sp, #7
3000b9cc:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b9d0:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b9d4:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b9d8:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b9dc:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000b9e0:	f042 0201 	orr.w	r2, r2, #1
3000b9e4:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000b9e8:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000b9ec:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b9f0:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000b9f4:	f88d c007 	strb.w	ip, [sp, #7]
3000b9f8:	f7fd f8d0 	bl	30008b9c <OTP_Read8>
3000b9fc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ba00:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ba04:	2af0      	cmp	r2, #240	; 0xf0
3000ba06:	d150      	bne.n	3000baaa <SWR_Calib_AUD+0xfe>
3000ba08:	f003 030f 	and.w	r3, r3, #15
3000ba0c:	2b0f      	cmp	r3, #15
3000ba0e:	d048      	beq.n	3000baa2 <SWR_Calib_AUD+0xf6>
3000ba10:	2302      	movs	r3, #2
3000ba12:	4c31      	ldr	r4, [pc, #196]	; (3000bad8 <SWR_Calib_AUD+0x12c>)
3000ba14:	f10d 0106 	add.w	r1, sp, #6
3000ba18:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000ba1c:	7023      	strb	r3, [r4, #0]
3000ba1e:	f7fd f8bd 	bl	30008b9c <OTP_Read8>
3000ba22:	7822      	ldrb	r2, [r4, #0]
3000ba24:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000ba28:	2a03      	cmp	r2, #3
3000ba2a:	d047      	beq.n	3000babc <SWR_Calib_AUD+0x110>
3000ba2c:	2a02      	cmp	r2, #2
3000ba2e:	bf0c      	ite	eq
3000ba30:	f003 030f 	andeq.w	r3, r3, #15
3000ba34:	2300      	movne	r3, #0
3000ba36:	4926      	ldr	r1, [pc, #152]	; (3000bad0 <SWR_Calib_AUD+0x124>)
3000ba38:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000ba3c:	f022 020f 	bic.w	r2, r2, #15
3000ba40:	4313      	orrs	r3, r2
3000ba42:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ba46:	2300      	movs	r3, #0
3000ba48:	f10d 0107 	add.w	r1, sp, #7
3000ba4c:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000ba50:	f88d 3007 	strb.w	r3, [sp, #7]
3000ba54:	f7fd f8a2 	bl	30008b9c <OTP_Read8>
3000ba58:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ba5c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000ba60:	2af0      	cmp	r2, #240	; 0xf0
3000ba62:	d124      	bne.n	3000baae <SWR_Calib_AUD+0x102>
3000ba64:	f003 030f 	and.w	r3, r3, #15
3000ba68:	2b0f      	cmp	r3, #15
3000ba6a:	d022      	beq.n	3000bab2 <SWR_Calib_AUD+0x106>
3000ba6c:	2302      	movs	r3, #2
3000ba6e:	4c1b      	ldr	r4, [pc, #108]	; (3000badc <SWR_Calib_AUD+0x130>)
3000ba70:	f10d 0107 	add.w	r1, sp, #7
3000ba74:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000ba78:	7023      	strb	r3, [r4, #0]
3000ba7a:	f7fd f88f 	bl	30008b9c <OTP_Read8>
3000ba7e:	7822      	ldrb	r2, [r4, #0]
3000ba80:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000ba84:	2a03      	cmp	r2, #3
3000ba86:	d01b      	beq.n	3000bac0 <SWR_Calib_AUD+0x114>
3000ba88:	2a02      	cmp	r2, #2
3000ba8a:	d01c      	beq.n	3000bac6 <SWR_Calib_AUD+0x11a>
3000ba8c:	2200      	movs	r2, #0
3000ba8e:	4910      	ldr	r1, [pc, #64]	; (3000bad0 <SWR_Calib_AUD+0x124>)
3000ba90:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000ba94:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000ba98:	4313      	orrs	r3, r2
3000ba9a:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000ba9e:	b002      	add	sp, #8
3000baa0:	bd10      	pop	{r4, pc}
3000baa2:	4b0d      	ldr	r3, [pc, #52]	; (3000bad8 <SWR_Calib_AUD+0x12c>)
3000baa4:	2201      	movs	r2, #1
3000baa6:	701a      	strb	r2, [r3, #0]
3000baa8:	e7cd      	b.n	3000ba46 <SWR_Calib_AUD+0x9a>
3000baaa:	2303      	movs	r3, #3
3000baac:	e7b1      	b.n	3000ba12 <SWR_Calib_AUD+0x66>
3000baae:	2303      	movs	r3, #3
3000bab0:	e7dd      	b.n	3000ba6e <SWR_Calib_AUD+0xc2>
3000bab2:	4b0a      	ldr	r3, [pc, #40]	; (3000badc <SWR_Calib_AUD+0x130>)
3000bab4:	2201      	movs	r2, #1
3000bab6:	701a      	strb	r2, [r3, #0]
3000bab8:	b002      	add	sp, #8
3000baba:	bd10      	pop	{r4, pc}
3000babc:	091b      	lsrs	r3, r3, #4
3000babe:	e7ba      	b.n	3000ba36 <SWR_Calib_AUD+0x8a>
3000bac0:	091b      	lsrs	r3, r3, #4
3000bac2:	059a      	lsls	r2, r3, #22
3000bac4:	e7e3      	b.n	3000ba8e <SWR_Calib_AUD+0xe2>
3000bac6:	f003 030f 	and.w	r3, r3, #15
3000baca:	059a      	lsls	r2, r3, #22
3000bacc:	e7df      	b.n	3000ba8e <SWR_Calib_AUD+0xe2>
3000bace:	bf00      	nop
3000bad0:	42008800 	.word	0x42008800
3000bad4:	fffffccc 	.word	0xfffffccc
3000bad8:	3000f9ad 	.word	0x3000f9ad
3000badc:	3000f9ac 	.word	0x3000f9ac

3000bae0 <crash_dump_memory>:
3000bae0:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000bae4:	460e      	mov	r6, r1
3000bae6:	4680      	mov	r8, r0
3000bae8:	4b21      	ldr	r3, [pc, #132]	; (3000bb70 <crash_dump_memory+0x90>)
3000baea:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000baee:	4a21      	ldr	r2, [pc, #132]	; (3000bb74 <crash_dump_memory+0x94>)
3000baf0:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000baf4:	4644      	mov	r4, r8
3000baf6:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000bafa:	4b1f      	ldr	r3, [pc, #124]	; (3000bb78 <crash_dump_memory+0x98>)
3000bafc:	481f      	ldr	r0, [pc, #124]	; (3000bb7c <crash_dump_memory+0x9c>)
3000bafe:	bf88      	it	hi
3000bb00:	2680      	movhi	r6, #128	; 0x80
3000bb02:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000bb06:	4f1e      	ldr	r7, [pc, #120]	; (3000bb80 <crash_dump_memory+0xa0>)
3000bb08:	bf14      	ite	ne
3000bb0a:	4611      	movne	r1, r2
3000bb0c:	4619      	moveq	r1, r3
3000bb0e:	f000 fdbb 	bl	3000c688 <__DiagPrintf_veneer>
3000bb12:	4632      	mov	r2, r6
3000bb14:	4641      	mov	r1, r8
3000bb16:	481b      	ldr	r0, [pc, #108]	; (3000bb84 <crash_dump_memory+0xa4>)
3000bb18:	f000 fdb6 	bl	3000c688 <__DiagPrintf_veneer>
3000bb1c:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bb8c <crash_dump_memory+0xac>
3000bb20:	2300      	movs	r3, #0
3000bb22:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000bb90 <crash_dump_memory+0xb0>
3000bb26:	e007      	b.n	3000bb38 <crash_dump_memory+0x58>
3000bb28:	6821      	ldr	r1, [r4, #0]
3000bb2a:	4638      	mov	r0, r7
3000bb2c:	f000 fdac 	bl	3000c688 <__DiagPrintf_veneer>
3000bb30:	42ae      	cmp	r6, r5
3000bb32:	d00f      	beq.n	3000bb54 <crash_dump_memory+0x74>
3000bb34:	3404      	adds	r4, #4
3000bb36:	462b      	mov	r3, r5
3000bb38:	1c5d      	adds	r5, r3, #1
3000bb3a:	b183      	cbz	r3, 3000bb5e <crash_dump_memory+0x7e>
3000bb3c:	079b      	lsls	r3, r3, #30
3000bb3e:	d1f3      	bne.n	3000bb28 <crash_dump_memory+0x48>
3000bb40:	4621      	mov	r1, r4
3000bb42:	4648      	mov	r0, r9
3000bb44:	f000 fda0 	bl	3000c688 <__DiagPrintf_veneer>
3000bb48:	6821      	ldr	r1, [r4, #0]
3000bb4a:	4638      	mov	r0, r7
3000bb4c:	f000 fd9c 	bl	3000c688 <__DiagPrintf_veneer>
3000bb50:	42ae      	cmp	r6, r5
3000bb52:	d1ef      	bne.n	3000bb34 <crash_dump_memory+0x54>
3000bb54:	480c      	ldr	r0, [pc, #48]	; (3000bb88 <crash_dump_memory+0xa8>)
3000bb56:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000bb5a:	f000 bd95 	b.w	3000c688 <__DiagPrintf_veneer>
3000bb5e:	4641      	mov	r1, r8
3000bb60:	4650      	mov	r0, sl
3000bb62:	f000 fd91 	bl	3000c688 <__DiagPrintf_veneer>
3000bb66:	6821      	ldr	r1, [r4, #0]
3000bb68:	4638      	mov	r0, r7
3000bb6a:	f000 fd8d 	bl	3000c688 <__DiagPrintf_veneer>
3000bb6e:	e7e1      	b.n	3000bb34 <crash_dump_memory+0x54>
3000bb70:	42008000 	.word	0x42008000
3000bb74:	3000dc38 	.word	0x3000dc38
3000bb78:	3000dc3c 	.word	0x3000dc3c
3000bb7c:	3000dc40 	.word	0x3000dc40
3000bb80:	3000dca8 	.word	0x3000dca8
3000bb84:	3000dc64 	.word	0x3000dc64
3000bb88:	3000d350 	.word	0x3000d350
3000bb8c:	3000dcb0 	.word	0x3000dcb0
3000bb90:	3000dca0 	.word	0x3000dca0

3000bb94 <crash_dump>:
3000bb94:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bb98:	4c4f      	ldr	r4, [pc, #316]	; (3000bcd8 <crash_dump+0x144>)
3000bb9a:	4615      	mov	r5, r2
3000bb9c:	4688      	mov	r8, r1
3000bb9e:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000bba2:	484e      	ldr	r0, [pc, #312]	; (3000bcdc <crash_dump+0x148>)
3000bba4:	462f      	mov	r7, r5
3000bba6:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000bd08 <crash_dump+0x174>
3000bbaa:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000bbae:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000bd14 <crash_dump+0x180>
3000bbb2:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000bbb6:	e9c4 2100 	strd	r2, r1, [r4]
3000bbba:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000bbbc:	60a2      	str	r2, [r4, #8]
3000bbbe:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000bbc0:	60e2      	str	r2, [r4, #12]
3000bbc2:	686a      	ldr	r2, [r5, #4]
3000bbc4:	6122      	str	r2, [r4, #16]
3000bbc6:	68aa      	ldr	r2, [r5, #8]
3000bbc8:	6162      	str	r2, [r4, #20]
3000bbca:	68ea      	ldr	r2, [r5, #12]
3000bbcc:	61a2      	str	r2, [r4, #24]
3000bbce:	692a      	ldr	r2, [r5, #16]
3000bbd0:	61e2      	str	r2, [r4, #28]
3000bbd2:	696a      	ldr	r2, [r5, #20]
3000bbd4:	6222      	str	r2, [r4, #32]
3000bbd6:	69aa      	ldr	r2, [r5, #24]
3000bbd8:	6262      	str	r2, [r4, #36]	; 0x24
3000bbda:	69ea      	ldr	r2, [r5, #28]
3000bbdc:	62a2      	str	r2, [r4, #40]	; 0x28
3000bbde:	6a2a      	ldr	r2, [r5, #32]
3000bbe0:	62e2      	str	r2, [r4, #44]	; 0x2c
3000bbe2:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bbe4:	6322      	str	r2, [r4, #48]	; 0x30
3000bbe6:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000bbe8:	6362      	str	r2, [r4, #52]	; 0x34
3000bbea:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000bbec:	63a2      	str	r2, [r4, #56]	; 0x38
3000bbee:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000bbf0:	63e2      	str	r2, [r4, #60]	; 0x3c
3000bbf2:	682a      	ldr	r2, [r5, #0]
3000bbf4:	6422      	str	r2, [r4, #64]	; 0x40
3000bbf6:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000bbfa:	6462      	str	r2, [r4, #68]	; 0x44
3000bbfc:	4a38      	ldr	r2, [pc, #224]	; (3000bce0 <crash_dump+0x14c>)
3000bbfe:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000bc02:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000bc06:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000bc0a:	64e1      	str	r1, [r4, #76]	; 0x4c
3000bc0c:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000bc10:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000bc14:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000bc18:	6561      	str	r1, [r4, #84]	; 0x54
3000bc1a:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000bc1c:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000bc20:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000bc24:	65e1      	str	r1, [r4, #92]	; 0x5c
3000bc26:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000bc28:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000bc2a:	b289      	uxth	r1, r1
3000bc2c:	b292      	uxth	r2, r2
3000bc2e:	6621      	str	r1, [r4, #96]	; 0x60
3000bc30:	6662      	str	r2, [r4, #100]	; 0x64
3000bc32:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000bc36:	66a2      	str	r2, [r4, #104]	; 0x68
3000bc38:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000bc3c:	66e3      	str	r3, [r4, #108]	; 0x6c
3000bc3e:	f000 fd23 	bl	3000c688 <__DiagPrintf_veneer>
3000bc42:	4620      	mov	r0, r4
3000bc44:	462c      	mov	r4, r5
3000bc46:	f000 f867 	bl	3000bd18 <fault_diagnosis>
3000bc4a:	4826      	ldr	r0, [pc, #152]	; (3000bce4 <crash_dump+0x150>)
3000bc4c:	f000 fd1c 	bl	3000c688 <__DiagPrintf_veneer>
3000bc50:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000bc52:	4825      	ldr	r0, [pc, #148]	; (3000bce8 <crash_dump+0x154>)
3000bc54:	f000 fd18 	bl	3000c688 <__DiagPrintf_veneer>
3000bc58:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000bc5a:	4824      	ldr	r0, [pc, #144]	; (3000bcec <crash_dump+0x158>)
3000bc5c:	f000 fd14 	bl	3000c688 <__DiagPrintf_veneer>
3000bc60:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000bc62:	4823      	ldr	r0, [pc, #140]	; (3000bcf0 <crash_dump+0x15c>)
3000bc64:	f000 fd10 	bl	3000c688 <__DiagPrintf_veneer>
3000bc68:	f857 1b20 	ldr.w	r1, [r7], #32
3000bc6c:	4821      	ldr	r0, [pc, #132]	; (3000bcf4 <crash_dump+0x160>)
3000bc6e:	463e      	mov	r6, r7
3000bc70:	f000 fd0a 	bl	3000c688 <__DiagPrintf_veneer>
3000bc74:	4920      	ldr	r1, [pc, #128]	; (3000bcf8 <crash_dump+0x164>)
3000bc76:	4658      	mov	r0, fp
3000bc78:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bc7c:	f000 fd04 	bl	3000c688 <__DiagPrintf_veneer>
3000bc80:	454e      	cmp	r6, r9
3000bc82:	d008      	beq.n	3000bc96 <crash_dump+0x102>
3000bc84:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000bc88:	4658      	mov	r0, fp
3000bc8a:	f85a 1b04 	ldr.w	r1, [sl], #4
3000bc8e:	f000 fcfb 	bl	3000c688 <__DiagPrintf_veneer>
3000bc92:	454e      	cmp	r6, r9
3000bc94:	d1f6      	bne.n	3000bc84 <crash_dump+0xf0>
3000bc96:	4e19      	ldr	r6, [pc, #100]	; (3000bcfc <crash_dump+0x168>)
3000bc98:	4919      	ldr	r1, [pc, #100]	; (3000bd00 <crash_dump+0x16c>)
3000bc9a:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bd08 <crash_dump+0x174>
3000bc9e:	e001      	b.n	3000bca4 <crash_dump+0x110>
3000bca0:	f856 1b04 	ldr.w	r1, [r6], #4
3000bca4:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000bca8:	4648      	mov	r0, r9
3000bcaa:	f000 fced 	bl	3000c688 <__DiagPrintf_veneer>
3000bcae:	42bc      	cmp	r4, r7
3000bcb0:	d1f6      	bne.n	3000bca0 <crash_dump+0x10c>
3000bcb2:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bcb4:	4913      	ldr	r1, [pc, #76]	; (3000bd04 <crash_dump+0x170>)
3000bcb6:	4814      	ldr	r0, [pc, #80]	; (3000bd08 <crash_dump+0x174>)
3000bcb8:	f000 fce6 	bl	3000c688 <__DiagPrintf_veneer>
3000bcbc:	2180      	movs	r1, #128	; 0x80
3000bcbe:	4640      	mov	r0, r8
3000bcc0:	f7ff ff0e 	bl	3000bae0 <crash_dump_memory>
3000bcc4:	4811      	ldr	r0, [pc, #68]	; (3000bd0c <crash_dump+0x178>)
3000bcc6:	f000 fcdf 	bl	3000c688 <__DiagPrintf_veneer>
3000bcca:	4811      	ldr	r0, [pc, #68]	; (3000bd10 <crash_dump+0x17c>)
3000bccc:	f000 fcdc 	bl	3000c688 <__DiagPrintf_veneer>
3000bcd0:	2000      	movs	r0, #0
3000bcd2:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bcd6:	bf00      	nop
3000bcd8:	3000ff98 	.word	0x3000ff98
3000bcdc:	3000dcc4 	.word	0x3000dcc4
3000bce0:	e000ed00 	.word	0xe000ed00
3000bce4:	3000dce8 	.word	0x3000dce8
3000bce8:	3000dd10 	.word	0x3000dd10
3000bcec:	3000dd20 	.word	0x3000dd20
3000bcf0:	3000dd30 	.word	0x3000dd30
3000bcf4:	3000dd40 	.word	0x3000dd40
3000bcf8:	3000dcbc 	.word	0x3000dcbc
3000bcfc:	3000dda0 	.word	0x3000dda0
3000bd00:	3000dcc0 	.word	0x3000dcc0
3000bd04:	3000dd60 	.word	0x3000dd60
3000bd08:	3000dd50 	.word	0x3000dd50
3000bd0c:	3000dd64 	.word	0x3000dd64
3000bd10:	3000dd90 	.word	0x3000dd90
3000bd14:	3000ddc0 	.word	0x3000ddc0

3000bd18 <fault_diagnosis>:
3000bd18:	b510      	push	{r4, lr}
3000bd1a:	4604      	mov	r4, r0
3000bd1c:	4888      	ldr	r0, [pc, #544]	; (3000bf40 <fault_diagnosis+0x228>)
3000bd1e:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000bd20:	f000 fcb2 	bl	3000c688 <__DiagPrintf_veneer>
3000bd24:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000bd28:	079b      	lsls	r3, r3, #30
3000bd2a:	f100 808a 	bmi.w	3000be42 <fault_diagnosis+0x12a>
3000bd2e:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bd30:	b1e1      	cbz	r1, 3000bd6c <fault_diagnosis+0x54>
3000bd32:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bd36:	07d8      	lsls	r0, r3, #31
3000bd38:	f100 8094 	bmi.w	3000be64 <fault_diagnosis+0x14c>
3000bd3c:	079a      	lsls	r2, r3, #30
3000bd3e:	f100 809a 	bmi.w	3000be76 <fault_diagnosis+0x15e>
3000bd42:	0758      	lsls	r0, r3, #29
3000bd44:	f100 80a1 	bmi.w	3000be8a <fault_diagnosis+0x172>
3000bd48:	0719      	lsls	r1, r3, #28
3000bd4a:	f100 80a8 	bmi.w	3000be9e <fault_diagnosis+0x186>
3000bd4e:	06da      	lsls	r2, r3, #27
3000bd50:	f100 80af 	bmi.w	3000beb2 <fault_diagnosis+0x19a>
3000bd54:	0698      	lsls	r0, r3, #26
3000bd56:	f100 80b6 	bmi.w	3000bec6 <fault_diagnosis+0x1ae>
3000bd5a:	0619      	lsls	r1, r3, #24
3000bd5c:	f100 80bd 	bmi.w	3000beda <fault_diagnosis+0x1c2>
3000bd60:	065a      	lsls	r2, r3, #25
3000bd62:	d503      	bpl.n	3000bd6c <fault_diagnosis+0x54>
3000bd64:	f013 0f28 	tst.w	r3, #40	; 0x28
3000bd68:	f040 8186 	bne.w	3000c078 <fault_diagnosis+0x360>
3000bd6c:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd70:	b199      	cbz	r1, 3000bd9a <fault_diagnosis+0x82>
3000bd72:	07cb      	lsls	r3, r1, #31
3000bd74:	f100 80b9 	bmi.w	3000beea <fault_diagnosis+0x1d2>
3000bd78:	0788      	lsls	r0, r1, #30
3000bd7a:	f100 80bf 	bmi.w	3000befc <fault_diagnosis+0x1e4>
3000bd7e:	070a      	lsls	r2, r1, #28
3000bd80:	f100 80c5 	bmi.w	3000bf0e <fault_diagnosis+0x1f6>
3000bd84:	06cb      	lsls	r3, r1, #27
3000bd86:	f100 80cb 	bmi.w	3000bf20 <fault_diagnosis+0x208>
3000bd8a:	0688      	lsls	r0, r1, #26
3000bd8c:	f100 80d1 	bmi.w	3000bf32 <fault_diagnosis+0x21a>
3000bd90:	060a      	lsls	r2, r1, #24
3000bd92:	d502      	bpl.n	3000bd9a <fault_diagnosis+0x82>
3000bd94:	078b      	lsls	r3, r1, #30
3000bd96:	f040 8194 	bne.w	3000c0c2 <fault_diagnosis+0x3aa>
3000bd9a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd9e:	b1b1      	cbz	r1, 3000bdce <fault_diagnosis+0xb6>
3000bda0:	07c8      	lsls	r0, r1, #31
3000bda2:	f100 80f7 	bmi.w	3000bf94 <fault_diagnosis+0x27c>
3000bda6:	078a      	lsls	r2, r1, #30
3000bda8:	f100 80fd 	bmi.w	3000bfa6 <fault_diagnosis+0x28e>
3000bdac:	074b      	lsls	r3, r1, #29
3000bdae:	f100 8103 	bmi.w	3000bfb8 <fault_diagnosis+0x2a0>
3000bdb2:	0708      	lsls	r0, r1, #28
3000bdb4:	f100 8109 	bmi.w	3000bfca <fault_diagnosis+0x2b2>
3000bdb8:	06ca      	lsls	r2, r1, #27
3000bdba:	f100 810f 	bmi.w	3000bfdc <fault_diagnosis+0x2c4>
3000bdbe:	068b      	lsls	r3, r1, #26
3000bdc0:	f100 8115 	bmi.w	3000bfee <fault_diagnosis+0x2d6>
3000bdc4:	0608      	lsls	r0, r1, #24
3000bdc6:	d502      	bpl.n	3000bdce <fault_diagnosis+0xb6>
3000bdc8:	0789      	lsls	r1, r1, #30
3000bdca:	f100 8180 	bmi.w	3000c0ce <fault_diagnosis+0x3b6>
3000bdce:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bdd2:	b1b9      	cbz	r1, 3000be04 <fault_diagnosis+0xec>
3000bdd4:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bdd8:	07da      	lsls	r2, r3, #31
3000bdda:	f100 810f 	bmi.w	3000bffc <fault_diagnosis+0x2e4>
3000bdde:	0798      	lsls	r0, r3, #30
3000bde0:	f100 8115 	bmi.w	3000c00e <fault_diagnosis+0x2f6>
3000bde4:	0759      	lsls	r1, r3, #29
3000bde6:	f100 811d 	bmi.w	3000c024 <fault_diagnosis+0x30c>
3000bdea:	071a      	lsls	r2, r3, #28
3000bdec:	f100 8125 	bmi.w	3000c03a <fault_diagnosis+0x322>
3000bdf0:	06db      	lsls	r3, r3, #27
3000bdf2:	f100 812d 	bmi.w	3000c050 <fault_diagnosis+0x338>
3000bdf6:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bdfa:	07d8      	lsls	r0, r3, #31
3000bdfc:	f100 8133 	bmi.w	3000c066 <fault_diagnosis+0x34e>
3000be00:	0799      	lsls	r1, r3, #30
3000be02:	d424      	bmi.n	3000be4e <fault_diagnosis+0x136>
3000be04:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000be08:	2b00      	cmp	r3, #0
3000be0a:	db00      	blt.n	3000be0e <fault_diagnosis+0xf6>
3000be0c:	bd10      	pop	{r4, pc}
3000be0e:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be10:	2900      	cmp	r1, #0
3000be12:	d0fb      	beq.n	3000be0c <fault_diagnosis+0xf4>
3000be14:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be18:	07da      	lsls	r2, r3, #31
3000be1a:	f100 8143 	bmi.w	3000c0a4 <fault_diagnosis+0x38c>
3000be1e:	0798      	lsls	r0, r3, #30
3000be20:	f100 8138 	bmi.w	3000c094 <fault_diagnosis+0x37c>
3000be24:	0759      	lsls	r1, r3, #29
3000be26:	f100 812d 	bmi.w	3000c084 <fault_diagnosis+0x36c>
3000be2a:	071a      	lsls	r2, r3, #28
3000be2c:	f100 8141 	bmi.w	3000c0b2 <fault_diagnosis+0x39a>
3000be30:	06db      	lsls	r3, r3, #27
3000be32:	d5eb      	bpl.n	3000be0c <fault_diagnosis+0xf4>
3000be34:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be36:	4a43      	ldr	r2, [pc, #268]	; (3000bf44 <fault_diagnosis+0x22c>)
3000be38:	4843      	ldr	r0, [pc, #268]	; (3000bf48 <fault_diagnosis+0x230>)
3000be3a:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000be3e:	f000 bc23 	b.w	3000c688 <__DiagPrintf_veneer>
3000be42:	4a42      	ldr	r2, [pc, #264]	; (3000bf4c <fault_diagnosis+0x234>)
3000be44:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000be46:	4842      	ldr	r0, [pc, #264]	; (3000bf50 <fault_diagnosis+0x238>)
3000be48:	f000 fc1e 	bl	3000c688 <__DiagPrintf_veneer>
3000be4c:	e76f      	b.n	3000bd2e <fault_diagnosis+0x16>
3000be4e:	4a41      	ldr	r2, [pc, #260]	; (3000bf54 <fault_diagnosis+0x23c>)
3000be50:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be54:	4840      	ldr	r0, [pc, #256]	; (3000bf58 <fault_diagnosis+0x240>)
3000be56:	f000 fc17 	bl	3000c688 <__DiagPrintf_veneer>
3000be5a:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000be5e:	2b00      	cmp	r3, #0
3000be60:	dad4      	bge.n	3000be0c <fault_diagnosis+0xf4>
3000be62:	e7d4      	b.n	3000be0e <fault_diagnosis+0xf6>
3000be64:	4a3d      	ldr	r2, [pc, #244]	; (3000bf5c <fault_diagnosis+0x244>)
3000be66:	483e      	ldr	r0, [pc, #248]	; (3000bf60 <fault_diagnosis+0x248>)
3000be68:	f000 fc0e 	bl	3000c688 <__DiagPrintf_veneer>
3000be6c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000be70:	079a      	lsls	r2, r3, #30
3000be72:	f57f af66 	bpl.w	3000bd42 <fault_diagnosis+0x2a>
3000be76:	4a3b      	ldr	r2, [pc, #236]	; (3000bf64 <fault_diagnosis+0x24c>)
3000be78:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000be7a:	4839      	ldr	r0, [pc, #228]	; (3000bf60 <fault_diagnosis+0x248>)
3000be7c:	f000 fc04 	bl	3000c688 <__DiagPrintf_veneer>
3000be80:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000be84:	0758      	lsls	r0, r3, #29
3000be86:	f57f af5f 	bpl.w	3000bd48 <fault_diagnosis+0x30>
3000be8a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000be8c:	4a36      	ldr	r2, [pc, #216]	; (3000bf68 <fault_diagnosis+0x250>)
3000be8e:	4834      	ldr	r0, [pc, #208]	; (3000bf60 <fault_diagnosis+0x248>)
3000be90:	f000 fbfa 	bl	3000c688 <__DiagPrintf_veneer>
3000be94:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000be98:	0719      	lsls	r1, r3, #28
3000be9a:	f57f af58 	bpl.w	3000bd4e <fault_diagnosis+0x36>
3000be9e:	4a33      	ldr	r2, [pc, #204]	; (3000bf6c <fault_diagnosis+0x254>)
3000bea0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bea2:	482f      	ldr	r0, [pc, #188]	; (3000bf60 <fault_diagnosis+0x248>)
3000bea4:	f000 fbf0 	bl	3000c688 <__DiagPrintf_veneer>
3000bea8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000beac:	06da      	lsls	r2, r3, #27
3000beae:	f57f af51 	bpl.w	3000bd54 <fault_diagnosis+0x3c>
3000beb2:	4a2f      	ldr	r2, [pc, #188]	; (3000bf70 <fault_diagnosis+0x258>)
3000beb4:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000beb6:	482a      	ldr	r0, [pc, #168]	; (3000bf60 <fault_diagnosis+0x248>)
3000beb8:	f000 fbe6 	bl	3000c688 <__DiagPrintf_veneer>
3000bebc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bec0:	0698      	lsls	r0, r3, #26
3000bec2:	f57f af4a 	bpl.w	3000bd5a <fault_diagnosis+0x42>
3000bec6:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bec8:	4a2a      	ldr	r2, [pc, #168]	; (3000bf74 <fault_diagnosis+0x25c>)
3000beca:	4825      	ldr	r0, [pc, #148]	; (3000bf60 <fault_diagnosis+0x248>)
3000becc:	f000 fbdc 	bl	3000c688 <__DiagPrintf_veneer>
3000bed0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bed4:	0619      	lsls	r1, r3, #24
3000bed6:	f57f af43 	bpl.w	3000bd60 <fault_diagnosis+0x48>
3000beda:	4a27      	ldr	r2, [pc, #156]	; (3000bf78 <fault_diagnosis+0x260>)
3000bedc:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bede:	4820      	ldr	r0, [pc, #128]	; (3000bf60 <fault_diagnosis+0x248>)
3000bee0:	f000 fbd2 	bl	3000c688 <__DiagPrintf_veneer>
3000bee4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bee8:	e73a      	b.n	3000bd60 <fault_diagnosis+0x48>
3000beea:	4a24      	ldr	r2, [pc, #144]	; (3000bf7c <fault_diagnosis+0x264>)
3000beec:	4824      	ldr	r0, [pc, #144]	; (3000bf80 <fault_diagnosis+0x268>)
3000beee:	f000 fbcb 	bl	3000c688 <__DiagPrintf_veneer>
3000bef2:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bef6:	0788      	lsls	r0, r1, #30
3000bef8:	f57f af41 	bpl.w	3000bd7e <fault_diagnosis+0x66>
3000befc:	4a21      	ldr	r2, [pc, #132]	; (3000bf84 <fault_diagnosis+0x26c>)
3000befe:	4820      	ldr	r0, [pc, #128]	; (3000bf80 <fault_diagnosis+0x268>)
3000bf00:	f000 fbc2 	bl	3000c688 <__DiagPrintf_veneer>
3000bf04:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf08:	070a      	lsls	r2, r1, #28
3000bf0a:	f57f af3b 	bpl.w	3000bd84 <fault_diagnosis+0x6c>
3000bf0e:	4a1e      	ldr	r2, [pc, #120]	; (3000bf88 <fault_diagnosis+0x270>)
3000bf10:	481b      	ldr	r0, [pc, #108]	; (3000bf80 <fault_diagnosis+0x268>)
3000bf12:	f000 fbb9 	bl	3000c688 <__DiagPrintf_veneer>
3000bf16:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf1a:	06cb      	lsls	r3, r1, #27
3000bf1c:	f57f af35 	bpl.w	3000bd8a <fault_diagnosis+0x72>
3000bf20:	4a1a      	ldr	r2, [pc, #104]	; (3000bf8c <fault_diagnosis+0x274>)
3000bf22:	4817      	ldr	r0, [pc, #92]	; (3000bf80 <fault_diagnosis+0x268>)
3000bf24:	f000 fbb0 	bl	3000c688 <__DiagPrintf_veneer>
3000bf28:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf2c:	0688      	lsls	r0, r1, #26
3000bf2e:	f57f af2f 	bpl.w	3000bd90 <fault_diagnosis+0x78>
3000bf32:	4a17      	ldr	r2, [pc, #92]	; (3000bf90 <fault_diagnosis+0x278>)
3000bf34:	4812      	ldr	r0, [pc, #72]	; (3000bf80 <fault_diagnosis+0x268>)
3000bf36:	f000 fba7 	bl	3000c688 <__DiagPrintf_veneer>
3000bf3a:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bf3e:	e727      	b.n	3000bd90 <fault_diagnosis+0x78>
3000bf40:	3000ddd0 	.word	0x3000ddd0
3000bf44:	3000e660 	.word	0x3000e660
3000bf48:	3000e5b8 	.word	0x3000e5b8
3000bf4c:	3000dde8 	.word	0x3000dde8
3000bf50:	3000de14 	.word	0x3000de14
3000bf54:	3000e51c 	.word	0x3000e51c
3000bf58:	3000e360 	.word	0x3000e360
3000bf5c:	3000de2c 	.word	0x3000de2c
3000bf60:	3000de5c 	.word	0x3000de5c
3000bf64:	3000de74 	.word	0x3000de74
3000bf68:	3000deac 	.word	0x3000deac
3000bf6c:	3000dee4 	.word	0x3000dee4
3000bf70:	3000df1c 	.word	0x3000df1c
3000bf74:	3000df4c 	.word	0x3000df4c
3000bf78:	3000df88 	.word	0x3000df88
3000bf7c:	3000dff8 	.word	0x3000dff8
3000bf80:	3000e03c 	.word	0x3000e03c
3000bf84:	3000e054 	.word	0x3000e054
3000bf88:	3000e090 	.word	0x3000e090
3000bf8c:	3000e0c8 	.word	0x3000e0c8
3000bf90:	3000e0fc 	.word	0x3000e0fc
3000bf94:	4a51      	ldr	r2, [pc, #324]	; (3000c0dc <fault_diagnosis+0x3c4>)
3000bf96:	4852      	ldr	r0, [pc, #328]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bf98:	f000 fb76 	bl	3000c688 <__DiagPrintf_veneer>
3000bf9c:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfa0:	078a      	lsls	r2, r1, #30
3000bfa2:	f57f af03 	bpl.w	3000bdac <fault_diagnosis+0x94>
3000bfa6:	4a4f      	ldr	r2, [pc, #316]	; (3000c0e4 <fault_diagnosis+0x3cc>)
3000bfa8:	484d      	ldr	r0, [pc, #308]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bfaa:	f000 fb6d 	bl	3000c688 <__DiagPrintf_veneer>
3000bfae:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfb2:	074b      	lsls	r3, r1, #29
3000bfb4:	f57f aefd 	bpl.w	3000bdb2 <fault_diagnosis+0x9a>
3000bfb8:	4a4b      	ldr	r2, [pc, #300]	; (3000c0e8 <fault_diagnosis+0x3d0>)
3000bfba:	4849      	ldr	r0, [pc, #292]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bfbc:	f000 fb64 	bl	3000c688 <__DiagPrintf_veneer>
3000bfc0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfc4:	0708      	lsls	r0, r1, #28
3000bfc6:	f57f aef7 	bpl.w	3000bdb8 <fault_diagnosis+0xa0>
3000bfca:	4a48      	ldr	r2, [pc, #288]	; (3000c0ec <fault_diagnosis+0x3d4>)
3000bfcc:	4844      	ldr	r0, [pc, #272]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bfce:	f000 fb5b 	bl	3000c688 <__DiagPrintf_veneer>
3000bfd2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfd6:	06ca      	lsls	r2, r1, #27
3000bfd8:	f57f aef1 	bpl.w	3000bdbe <fault_diagnosis+0xa6>
3000bfdc:	4a44      	ldr	r2, [pc, #272]	; (3000c0f0 <fault_diagnosis+0x3d8>)
3000bfde:	4840      	ldr	r0, [pc, #256]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bfe0:	f000 fb52 	bl	3000c688 <__DiagPrintf_veneer>
3000bfe4:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bfe8:	068b      	lsls	r3, r1, #26
3000bfea:	f57f aeeb 	bpl.w	3000bdc4 <fault_diagnosis+0xac>
3000bfee:	4a41      	ldr	r2, [pc, #260]	; (3000c0f4 <fault_diagnosis+0x3dc>)
3000bff0:	483b      	ldr	r0, [pc, #236]	; (3000c0e0 <fault_diagnosis+0x3c8>)
3000bff2:	f000 fb49 	bl	3000c688 <__DiagPrintf_veneer>
3000bff6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bffa:	e6e3      	b.n	3000bdc4 <fault_diagnosis+0xac>
3000bffc:	4a3e      	ldr	r2, [pc, #248]	; (3000c0f8 <fault_diagnosis+0x3e0>)
3000bffe:	483f      	ldr	r0, [pc, #252]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c000:	f000 fb42 	bl	3000c688 <__DiagPrintf_veneer>
3000c004:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c008:	0798      	lsls	r0, r3, #30
3000c00a:	f57f aeeb 	bpl.w	3000bde4 <fault_diagnosis+0xcc>
3000c00e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c012:	4a3b      	ldr	r2, [pc, #236]	; (3000c100 <fault_diagnosis+0x3e8>)
3000c014:	4839      	ldr	r0, [pc, #228]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c016:	f000 fb37 	bl	3000c688 <__DiagPrintf_veneer>
3000c01a:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c01e:	0759      	lsls	r1, r3, #29
3000c020:	f57f aee3 	bpl.w	3000bdea <fault_diagnosis+0xd2>
3000c024:	4a37      	ldr	r2, [pc, #220]	; (3000c104 <fault_diagnosis+0x3ec>)
3000c026:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c02a:	4834      	ldr	r0, [pc, #208]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c02c:	f000 fb2c 	bl	3000c688 <__DiagPrintf_veneer>
3000c030:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c034:	071a      	lsls	r2, r3, #28
3000c036:	f57f aedb 	bpl.w	3000bdf0 <fault_diagnosis+0xd8>
3000c03a:	4a33      	ldr	r2, [pc, #204]	; (3000c108 <fault_diagnosis+0x3f0>)
3000c03c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c040:	482e      	ldr	r0, [pc, #184]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c042:	f000 fb21 	bl	3000c688 <__DiagPrintf_veneer>
3000c046:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000c04a:	06db      	lsls	r3, r3, #27
3000c04c:	f57f aed3 	bpl.w	3000bdf6 <fault_diagnosis+0xde>
3000c050:	4a2e      	ldr	r2, [pc, #184]	; (3000c10c <fault_diagnosis+0x3f4>)
3000c052:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c056:	4829      	ldr	r0, [pc, #164]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c058:	f000 fb16 	bl	3000c688 <__DiagPrintf_veneer>
3000c05c:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c060:	07d8      	lsls	r0, r3, #31
3000c062:	f57f aecd 	bpl.w	3000be00 <fault_diagnosis+0xe8>
3000c066:	4a2a      	ldr	r2, [pc, #168]	; (3000c110 <fault_diagnosis+0x3f8>)
3000c068:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000c06c:	4823      	ldr	r0, [pc, #140]	; (3000c0fc <fault_diagnosis+0x3e4>)
3000c06e:	f000 fb0b 	bl	3000c688 <__DiagPrintf_veneer>
3000c072:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000c076:	e6c3      	b.n	3000be00 <fault_diagnosis+0xe8>
3000c078:	4a26      	ldr	r2, [pc, #152]	; (3000c114 <fault_diagnosis+0x3fc>)
3000c07a:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000c07c:	4826      	ldr	r0, [pc, #152]	; (3000c118 <fault_diagnosis+0x400>)
3000c07e:	f000 fb03 	bl	3000c688 <__DiagPrintf_veneer>
3000c082:	e673      	b.n	3000bd6c <fault_diagnosis+0x54>
3000c084:	4a25      	ldr	r2, [pc, #148]	; (3000c11c <fault_diagnosis+0x404>)
3000c086:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c088:	4825      	ldr	r0, [pc, #148]	; (3000c120 <fault_diagnosis+0x408>)
3000c08a:	f000 fafd 	bl	3000c688 <__DiagPrintf_veneer>
3000c08e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c092:	e6ca      	b.n	3000be2a <fault_diagnosis+0x112>
3000c094:	4a23      	ldr	r2, [pc, #140]	; (3000c124 <fault_diagnosis+0x40c>)
3000c096:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c098:	4821      	ldr	r0, [pc, #132]	; (3000c120 <fault_diagnosis+0x408>)
3000c09a:	f000 faf5 	bl	3000c688 <__DiagPrintf_veneer>
3000c09e:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0a2:	e6bf      	b.n	3000be24 <fault_diagnosis+0x10c>
3000c0a4:	4a20      	ldr	r2, [pc, #128]	; (3000c128 <fault_diagnosis+0x410>)
3000c0a6:	481e      	ldr	r0, [pc, #120]	; (3000c120 <fault_diagnosis+0x408>)
3000c0a8:	f000 faee 	bl	3000c688 <__DiagPrintf_veneer>
3000c0ac:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0b0:	e6b5      	b.n	3000be1e <fault_diagnosis+0x106>
3000c0b2:	4a1e      	ldr	r2, [pc, #120]	; (3000c12c <fault_diagnosis+0x414>)
3000c0b4:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000c0b6:	481a      	ldr	r0, [pc, #104]	; (3000c120 <fault_diagnosis+0x408>)
3000c0b8:	f000 fae6 	bl	3000c688 <__DiagPrintf_veneer>
3000c0bc:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000c0c0:	e6b6      	b.n	3000be30 <fault_diagnosis+0x118>
3000c0c2:	4a1b      	ldr	r2, [pc, #108]	; (3000c130 <fault_diagnosis+0x418>)
3000c0c4:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000c0c6:	481b      	ldr	r0, [pc, #108]	; (3000c134 <fault_diagnosis+0x41c>)
3000c0c8:	f000 fade 	bl	3000c688 <__DiagPrintf_veneer>
3000c0cc:	e665      	b.n	3000bd9a <fault_diagnosis+0x82>
3000c0ce:	4a1a      	ldr	r2, [pc, #104]	; (3000c138 <fault_diagnosis+0x420>)
3000c0d0:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000c0d2:	481a      	ldr	r0, [pc, #104]	; (3000c13c <fault_diagnosis+0x424>)
3000c0d4:	f000 fad8 	bl	3000c688 <__DiagPrintf_veneer>
3000c0d8:	e679      	b.n	3000bdce <fault_diagnosis+0xb6>
3000c0da:	bf00      	nop
3000c0dc:	3000e190 	.word	0x3000e190
3000c0e0:	3000e1c4 	.word	0x3000e1c4
3000c0e4:	3000e1dc 	.word	0x3000e1dc
3000c0e8:	3000e214 	.word	0x3000e214
3000c0ec:	3000e24c 	.word	0x3000e24c
3000c0f0:	3000e274 	.word	0x3000e274
3000c0f4:	3000e29c 	.word	0x3000e29c
3000c0f8:	3000e318 	.word	0x3000e318
3000c0fc:	3000e360 	.word	0x3000e360
3000c100:	3000e378 	.word	0x3000e378
3000c104:	3000e3c4 	.word	0x3000e3c4
3000c108:	3000e424 	.word	0x3000e424
3000c10c:	3000e46c 	.word	0x3000e46c
3000c110:	3000e4c8 	.word	0x3000e4c8
3000c114:	3000dfb8 	.word	0x3000dfb8
3000c118:	3000dfe0 	.word	0x3000dfe0
3000c11c:	3000e604 	.word	0x3000e604
3000c120:	3000e5b8 	.word	0x3000e5b8
3000c124:	3000e5d0 	.word	0x3000e5d0
3000c128:	3000e588 	.word	0x3000e588
3000c12c:	3000e630 	.word	0x3000e630
3000c130:	3000e148 	.word	0x3000e148
3000c134:	3000e178 	.word	0x3000e178
3000c138:	3000e2dc 	.word	0x3000e2dc
3000c13c:	3000e300 	.word	0x3000e300

3000c140 <rtk_log_write>:
3000c140:	b40c      	push	{r2, r3}
3000c142:	4b18      	ldr	r3, [pc, #96]	; (3000c1a4 <rtk_log_write+0x64>)
3000c144:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c148:	681a      	ldr	r2, [r3, #0]
3000c14a:	b083      	sub	sp, #12
3000c14c:	4680      	mov	r8, r0
3000c14e:	460e      	mov	r6, r1
3000c150:	2a02      	cmp	r2, #2
3000c152:	d825      	bhi.n	3000c1a0 <rtk_log_write+0x60>
3000c154:	681f      	ldr	r7, [r3, #0]
3000c156:	b1a7      	cbz	r7, 3000c182 <rtk_log_write+0x42>
3000c158:	4d13      	ldr	r5, [pc, #76]	; (3000c1a8 <rtk_log_write+0x68>)
3000c15a:	2400      	movs	r4, #0
3000c15c:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000c1b4 <rtk_log_write+0x74>
3000c160:	e002      	b.n	3000c168 <rtk_log_write+0x28>
3000c162:	3401      	adds	r4, #1
3000c164:	42a7      	cmp	r7, r4
3000c166:	d90c      	bls.n	3000c182 <rtk_log_write+0x42>
3000c168:	4628      	mov	r0, r5
3000c16a:	4631      	mov	r1, r6
3000c16c:	350b      	adds	r5, #11
3000c16e:	47c8      	blx	r9
3000c170:	2800      	cmp	r0, #0
3000c172:	d1f6      	bne.n	3000c162 <rtk_log_write+0x22>
3000c174:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000c178:	4b0c      	ldr	r3, [pc, #48]	; (3000c1ac <rtk_log_write+0x6c>)
3000c17a:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000c17e:	5d1b      	ldrb	r3, [r3, r4]
3000c180:	e001      	b.n	3000c186 <rtk_log_write+0x46>
3000c182:	4b0b      	ldr	r3, [pc, #44]	; (3000c1b0 <rtk_log_write+0x70>)
3000c184:	781b      	ldrb	r3, [r3, #0]
3000c186:	4598      	cmp	r8, r3
3000c188:	d805      	bhi.n	3000c196 <rtk_log_write+0x56>
3000c18a:	aa0b      	add	r2, sp, #44	; 0x2c
3000c18c:	990a      	ldr	r1, [sp, #40]	; 0x28
3000c18e:	2000      	movs	r0, #0
3000c190:	9201      	str	r2, [sp, #4]
3000c192:	f000 fa61 	bl	3000c658 <__DiagVSprintf_veneer>
3000c196:	b003      	add	sp, #12
3000c198:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000c19c:	b002      	add	sp, #8
3000c19e:	4770      	bx	lr
3000c1a0:	2703      	movs	r7, #3
3000c1a2:	e7d9      	b.n	3000c158 <rtk_log_write+0x18>
3000c1a4:	300103d8 	.word	0x300103d8
3000c1a8:	300103dd 	.word	0x300103dd
3000c1ac:	300103dc 	.word	0x300103dc
3000c1b0:	3000f9c0 	.word	0x3000f9c0
3000c1b4:	00012c89 	.word	0x00012c89

3000c1b8 <BOOT_VerCheck>:
3000c1b8:	b570      	push	{r4, r5, r6, lr}
3000c1ba:	4b1d      	ldr	r3, [pc, #116]	; (3000c230 <BOOT_VerCheck+0x78>)
3000c1bc:	4798      	blx	r3
3000c1be:	b100      	cbz	r0, 3000c1c2 <BOOT_VerCheck+0xa>
3000c1c0:	bd70      	pop	{r4, r5, r6, pc}
3000c1c2:	4b1c      	ldr	r3, [pc, #112]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c1c4:	2503      	movs	r5, #3
3000c1c6:	4c1b      	ldr	r4, [pc, #108]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c1c8:	2003      	movs	r0, #3
3000c1ca:	4e1b      	ldr	r6, [pc, #108]	; (3000c238 <BOOT_VerCheck+0x80>)
3000c1cc:	4619      	mov	r1, r3
3000c1ce:	4a1b      	ldr	r2, [pc, #108]	; (3000c23c <BOOT_VerCheck+0x84>)
3000c1d0:	f7ff ffb6 	bl	3000c140 <rtk_log_write>
3000c1d4:	4632      	mov	r2, r6
3000c1d6:	4623      	mov	r3, r4
3000c1d8:	4916      	ldr	r1, [pc, #88]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c1da:	2003      	movs	r0, #3
3000c1dc:	f7ff ffb0 	bl	3000c140 <rtk_log_write>
3000c1e0:	4623      	mov	r3, r4
3000c1e2:	4a17      	ldr	r2, [pc, #92]	; (3000c240 <BOOT_VerCheck+0x88>)
3000c1e4:	2003      	movs	r0, #3
3000c1e6:	4913      	ldr	r1, [pc, #76]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c1e8:	f7ff ffaa 	bl	3000c140 <rtk_log_write>
3000c1ec:	4623      	mov	r3, r4
3000c1ee:	4a15      	ldr	r2, [pc, #84]	; (3000c244 <BOOT_VerCheck+0x8c>)
3000c1f0:	2003      	movs	r0, #3
3000c1f2:	4910      	ldr	r1, [pc, #64]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c1f4:	f7ff ffa4 	bl	3000c140 <rtk_log_write>
3000c1f8:	4623      	mov	r3, r4
3000c1fa:	4a13      	ldr	r2, [pc, #76]	; (3000c248 <BOOT_VerCheck+0x90>)
3000c1fc:	2003      	movs	r0, #3
3000c1fe:	490d      	ldr	r1, [pc, #52]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c200:	f7ff ff9e 	bl	3000c140 <rtk_log_write>
3000c204:	4623      	mov	r3, r4
3000c206:	4632      	mov	r2, r6
3000c208:	490a      	ldr	r1, [pc, #40]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c20a:	2003      	movs	r0, #3
3000c20c:	f7ff ff98 	bl	3000c140 <rtk_log_write>
3000c210:	4623      	mov	r3, r4
3000c212:	4a0e      	ldr	r2, [pc, #56]	; (3000c24c <BOOT_VerCheck+0x94>)
3000c214:	2003      	movs	r0, #3
3000c216:	4907      	ldr	r1, [pc, #28]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c218:	f7ff ff92 	bl	3000c140 <rtk_log_write>
3000c21c:	3d01      	subs	r5, #1
3000c21e:	d1d9      	bne.n	3000c1d4 <BOOT_VerCheck+0x1c>
3000c220:	4b04      	ldr	r3, [pc, #16]	; (3000c234 <BOOT_VerCheck+0x7c>)
3000c222:	2003      	movs	r0, #3
3000c224:	4a0a      	ldr	r2, [pc, #40]	; (3000c250 <BOOT_VerCheck+0x98>)
3000c226:	4619      	mov	r1, r3
3000c228:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000c22c:	f7ff bf88 	b.w	3000c140 <rtk_log_write>
3000c230:	30009ead 	.word	0x30009ead
3000c234:	3000c974 	.word	0x3000c974
3000c238:	3000e778 	.word	0x3000e778
3000c23c:	3000e734 	.word	0x3000e734
3000c240:	3000e7bc 	.word	0x3000e7bc
3000c244:	3000e800 	.word	0x3000e800
3000c248:	3000e844 	.word	0x3000e844
3000c24c:	3000e888 	.word	0x3000e888
3000c250:	3000e8cc 	.word	0x3000e8cc

3000c254 <__aeabi_uldivmod>:
3000c254:	b953      	cbnz	r3, 3000c26c <__aeabi_uldivmod+0x18>
3000c256:	b94a      	cbnz	r2, 3000c26c <__aeabi_uldivmod+0x18>
3000c258:	2900      	cmp	r1, #0
3000c25a:	bf08      	it	eq
3000c25c:	2800      	cmpeq	r0, #0
3000c25e:	bf1c      	itt	ne
3000c260:	f04f 31ff 	movne.w	r1, #4294967295
3000c264:	f04f 30ff 	movne.w	r0, #4294967295
3000c268:	f000 b960 	b.w	3000c52c <__aeabi_idiv0>
3000c26c:	f1ad 0c08 	sub.w	ip, sp, #8
3000c270:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000c274:	f000 f806 	bl	3000c284 <__udivmoddi4>
3000c278:	f8dd e004 	ldr.w	lr, [sp, #4]
3000c27c:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000c280:	b004      	add	sp, #16
3000c282:	4770      	bx	lr

3000c284 <__udivmoddi4>:
3000c284:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000c288:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000c28a:	4688      	mov	r8, r1
3000c28c:	468e      	mov	lr, r1
3000c28e:	4615      	mov	r5, r2
3000c290:	4604      	mov	r4, r0
3000c292:	4619      	mov	r1, r3
3000c294:	2b00      	cmp	r3, #0
3000c296:	f040 80c6 	bne.w	3000c426 <__udivmoddi4+0x1a2>
3000c29a:	4542      	cmp	r2, r8
3000c29c:	fab2 f782 	clz	r7, r2
3000c2a0:	d946      	bls.n	3000c330 <__udivmoddi4+0xac>
3000c2a2:	b14f      	cbz	r7, 3000c2b8 <__udivmoddi4+0x34>
3000c2a4:	f1c7 0c20 	rsb	ip, r7, #32
3000c2a8:	fa08 f307 	lsl.w	r3, r8, r7
3000c2ac:	40bd      	lsls	r5, r7
3000c2ae:	40bc      	lsls	r4, r7
3000c2b0:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c2b4:	ea4c 0e03 	orr.w	lr, ip, r3
3000c2b8:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000c2bc:	fa1f fc85 	uxth.w	ip, r5
3000c2c0:	0c22      	lsrs	r2, r4, #16
3000c2c2:	fbbe f9f8 	udiv	r9, lr, r8
3000c2c6:	fb08 e319 	mls	r3, r8, r9, lr
3000c2ca:	fb09 fa0c 	mul.w	sl, r9, ip
3000c2ce:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000c2d2:	459a      	cmp	sl, r3
3000c2d4:	d928      	bls.n	3000c328 <__udivmoddi4+0xa4>
3000c2d6:	18eb      	adds	r3, r5, r3
3000c2d8:	f109 30ff 	add.w	r0, r9, #4294967295
3000c2dc:	d204      	bcs.n	3000c2e8 <__udivmoddi4+0x64>
3000c2de:	459a      	cmp	sl, r3
3000c2e0:	d902      	bls.n	3000c2e8 <__udivmoddi4+0x64>
3000c2e2:	f1a9 0002 	sub.w	r0, r9, #2
3000c2e6:	442b      	add	r3, r5
3000c2e8:	eba3 030a 	sub.w	r3, r3, sl
3000c2ec:	b2a4      	uxth	r4, r4
3000c2ee:	fbb3 f2f8 	udiv	r2, r3, r8
3000c2f2:	fb08 3312 	mls	r3, r8, r2, r3
3000c2f6:	fb02 fc0c 	mul.w	ip, r2, ip
3000c2fa:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c2fe:	45a4      	cmp	ip, r4
3000c300:	d914      	bls.n	3000c32c <__udivmoddi4+0xa8>
3000c302:	192c      	adds	r4, r5, r4
3000c304:	f102 33ff 	add.w	r3, r2, #4294967295
3000c308:	d203      	bcs.n	3000c312 <__udivmoddi4+0x8e>
3000c30a:	45a4      	cmp	ip, r4
3000c30c:	d901      	bls.n	3000c312 <__udivmoddi4+0x8e>
3000c30e:	1e93      	subs	r3, r2, #2
3000c310:	442c      	add	r4, r5
3000c312:	eba4 040c 	sub.w	r4, r4, ip
3000c316:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c31a:	b11e      	cbz	r6, 3000c324 <__udivmoddi4+0xa0>
3000c31c:	40fc      	lsrs	r4, r7
3000c31e:	2300      	movs	r3, #0
3000c320:	6034      	str	r4, [r6, #0]
3000c322:	6073      	str	r3, [r6, #4]
3000c324:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c328:	4648      	mov	r0, r9
3000c32a:	e7dd      	b.n	3000c2e8 <__udivmoddi4+0x64>
3000c32c:	4613      	mov	r3, r2
3000c32e:	e7f0      	b.n	3000c312 <__udivmoddi4+0x8e>
3000c330:	b902      	cbnz	r2, 3000c334 <__udivmoddi4+0xb0>
3000c332:	deff      	udf	#255	; 0xff
3000c334:	bb8f      	cbnz	r7, 3000c39a <__udivmoddi4+0x116>
3000c336:	eba8 0302 	sub.w	r3, r8, r2
3000c33a:	2101      	movs	r1, #1
3000c33c:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c340:	b2aa      	uxth	r2, r5
3000c342:	0c20      	lsrs	r0, r4, #16
3000c344:	fbb3 fcfe 	udiv	ip, r3, lr
3000c348:	fb0e 331c 	mls	r3, lr, ip, r3
3000c34c:	fb0c f802 	mul.w	r8, ip, r2
3000c350:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c354:	4598      	cmp	r8, r3
3000c356:	d962      	bls.n	3000c41e <__udivmoddi4+0x19a>
3000c358:	18eb      	adds	r3, r5, r3
3000c35a:	f10c 30ff 	add.w	r0, ip, #4294967295
3000c35e:	d204      	bcs.n	3000c36a <__udivmoddi4+0xe6>
3000c360:	4598      	cmp	r8, r3
3000c362:	d902      	bls.n	3000c36a <__udivmoddi4+0xe6>
3000c364:	f1ac 0002 	sub.w	r0, ip, #2
3000c368:	442b      	add	r3, r5
3000c36a:	eba3 0308 	sub.w	r3, r3, r8
3000c36e:	b2a4      	uxth	r4, r4
3000c370:	fbb3 fcfe 	udiv	ip, r3, lr
3000c374:	fb0e 331c 	mls	r3, lr, ip, r3
3000c378:	fb0c f202 	mul.w	r2, ip, r2
3000c37c:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c380:	42a2      	cmp	r2, r4
3000c382:	d94e      	bls.n	3000c422 <__udivmoddi4+0x19e>
3000c384:	192c      	adds	r4, r5, r4
3000c386:	f10c 33ff 	add.w	r3, ip, #4294967295
3000c38a:	d204      	bcs.n	3000c396 <__udivmoddi4+0x112>
3000c38c:	42a2      	cmp	r2, r4
3000c38e:	d902      	bls.n	3000c396 <__udivmoddi4+0x112>
3000c390:	f1ac 0302 	sub.w	r3, ip, #2
3000c394:	442c      	add	r4, r5
3000c396:	1aa4      	subs	r4, r4, r2
3000c398:	e7bd      	b.n	3000c316 <__udivmoddi4+0x92>
3000c39a:	f1c7 0c20 	rsb	ip, r7, #32
3000c39e:	40bd      	lsls	r5, r7
3000c3a0:	fa08 f307 	lsl.w	r3, r8, r7
3000c3a4:	40bc      	lsls	r4, r7
3000c3a6:	fa28 f90c 	lsr.w	r9, r8, ip
3000c3aa:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c3ae:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c3b2:	ea4c 0203 	orr.w	r2, ip, r3
3000c3b6:	b2ab      	uxth	r3, r5
3000c3b8:	fbb9 fcfe 	udiv	ip, r9, lr
3000c3bc:	0c11      	lsrs	r1, r2, #16
3000c3be:	fb0e 901c 	mls	r0, lr, ip, r9
3000c3c2:	fb0c f803 	mul.w	r8, ip, r3
3000c3c6:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c3ca:	4580      	cmp	r8, r0
3000c3cc:	d923      	bls.n	3000c416 <__udivmoddi4+0x192>
3000c3ce:	1828      	adds	r0, r5, r0
3000c3d0:	f10c 31ff 	add.w	r1, ip, #4294967295
3000c3d4:	d204      	bcs.n	3000c3e0 <__udivmoddi4+0x15c>
3000c3d6:	4580      	cmp	r8, r0
3000c3d8:	d902      	bls.n	3000c3e0 <__udivmoddi4+0x15c>
3000c3da:	f1ac 0102 	sub.w	r1, ip, #2
3000c3de:	4428      	add	r0, r5
3000c3e0:	eba0 0008 	sub.w	r0, r0, r8
3000c3e4:	b292      	uxth	r2, r2
3000c3e6:	fbb0 fcfe 	udiv	ip, r0, lr
3000c3ea:	fb0e 001c 	mls	r0, lr, ip, r0
3000c3ee:	fb0c f803 	mul.w	r8, ip, r3
3000c3f2:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c3f6:	4598      	cmp	r8, r3
3000c3f8:	d90f      	bls.n	3000c41a <__udivmoddi4+0x196>
3000c3fa:	18eb      	adds	r3, r5, r3
3000c3fc:	f10c 32ff 	add.w	r2, ip, #4294967295
3000c400:	d204      	bcs.n	3000c40c <__udivmoddi4+0x188>
3000c402:	4598      	cmp	r8, r3
3000c404:	d902      	bls.n	3000c40c <__udivmoddi4+0x188>
3000c406:	f1ac 0202 	sub.w	r2, ip, #2
3000c40a:	442b      	add	r3, r5
3000c40c:	eba3 0308 	sub.w	r3, r3, r8
3000c410:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c414:	e792      	b.n	3000c33c <__udivmoddi4+0xb8>
3000c416:	4661      	mov	r1, ip
3000c418:	e7e2      	b.n	3000c3e0 <__udivmoddi4+0x15c>
3000c41a:	4662      	mov	r2, ip
3000c41c:	e7f6      	b.n	3000c40c <__udivmoddi4+0x188>
3000c41e:	4660      	mov	r0, ip
3000c420:	e7a3      	b.n	3000c36a <__udivmoddi4+0xe6>
3000c422:	4663      	mov	r3, ip
3000c424:	e7b7      	b.n	3000c396 <__udivmoddi4+0x112>
3000c426:	4543      	cmp	r3, r8
3000c428:	d905      	bls.n	3000c436 <__udivmoddi4+0x1b2>
3000c42a:	b10e      	cbz	r6, 3000c430 <__udivmoddi4+0x1ac>
3000c42c:	e9c6 0800 	strd	r0, r8, [r6]
3000c430:	2100      	movs	r1, #0
3000c432:	4608      	mov	r0, r1
3000c434:	e776      	b.n	3000c324 <__udivmoddi4+0xa0>
3000c436:	fab3 fc83 	clz	ip, r3
3000c43a:	f1bc 0f00 	cmp.w	ip, #0
3000c43e:	d110      	bne.n	3000c462 <__udivmoddi4+0x1de>
3000c440:	4543      	cmp	r3, r8
3000c442:	d301      	bcc.n	3000c448 <__udivmoddi4+0x1c4>
3000c444:	4282      	cmp	r2, r0
3000c446:	d80a      	bhi.n	3000c45e <__udivmoddi4+0x1da>
3000c448:	1a84      	subs	r4, r0, r2
3000c44a:	eb68 0303 	sbc.w	r3, r8, r3
3000c44e:	2001      	movs	r0, #1
3000c450:	469e      	mov	lr, r3
3000c452:	2e00      	cmp	r6, #0
3000c454:	d068      	beq.n	3000c528 <__udivmoddi4+0x2a4>
3000c456:	e9c6 4e00 	strd	r4, lr, [r6]
3000c45a:	2100      	movs	r1, #0
3000c45c:	e762      	b.n	3000c324 <__udivmoddi4+0xa0>
3000c45e:	4660      	mov	r0, ip
3000c460:	e7f7      	b.n	3000c452 <__udivmoddi4+0x1ce>
3000c462:	f1cc 0e20 	rsb	lr, ip, #32
3000c466:	fa03 f30c 	lsl.w	r3, r3, ip
3000c46a:	fa02 f50c 	lsl.w	r5, r2, ip
3000c46e:	fa00 f70c 	lsl.w	r7, r0, ip
3000c472:	fa22 f40e 	lsr.w	r4, r2, lr
3000c476:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c47a:	fa20 f10e 	lsr.w	r1, r0, lr
3000c47e:	431c      	orrs	r4, r3
3000c480:	fa08 f30c 	lsl.w	r3, r8, ip
3000c484:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c488:	4319      	orrs	r1, r3
3000c48a:	b2a3      	uxth	r3, r4
3000c48c:	0c0a      	lsrs	r2, r1, #16
3000c48e:	fbbb f8f9 	udiv	r8, fp, r9
3000c492:	fb09 bb18 	mls	fp, r9, r8, fp
3000c496:	fb08 fa03 	mul.w	sl, r8, r3
3000c49a:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c49e:	4592      	cmp	sl, r2
3000c4a0:	d93e      	bls.n	3000c520 <__udivmoddi4+0x29c>
3000c4a2:	18a2      	adds	r2, r4, r2
3000c4a4:	f108 30ff 	add.w	r0, r8, #4294967295
3000c4a8:	d204      	bcs.n	3000c4b4 <__udivmoddi4+0x230>
3000c4aa:	4592      	cmp	sl, r2
3000c4ac:	d902      	bls.n	3000c4b4 <__udivmoddi4+0x230>
3000c4ae:	f1a8 0002 	sub.w	r0, r8, #2
3000c4b2:	4422      	add	r2, r4
3000c4b4:	eba2 020a 	sub.w	r2, r2, sl
3000c4b8:	b289      	uxth	r1, r1
3000c4ba:	fbb2 f8f9 	udiv	r8, r2, r9
3000c4be:	fb09 2218 	mls	r2, r9, r8, r2
3000c4c2:	fb08 f303 	mul.w	r3, r8, r3
3000c4c6:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c4ca:	4293      	cmp	r3, r2
3000c4cc:	d92a      	bls.n	3000c524 <__udivmoddi4+0x2a0>
3000c4ce:	18a2      	adds	r2, r4, r2
3000c4d0:	f108 31ff 	add.w	r1, r8, #4294967295
3000c4d4:	d204      	bcs.n	3000c4e0 <__udivmoddi4+0x25c>
3000c4d6:	4293      	cmp	r3, r2
3000c4d8:	d902      	bls.n	3000c4e0 <__udivmoddi4+0x25c>
3000c4da:	f1a8 0102 	sub.w	r1, r8, #2
3000c4de:	4422      	add	r2, r4
3000c4e0:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c4e4:	1ad2      	subs	r2, r2, r3
3000c4e6:	fba0 9805 	umull	r9, r8, r0, r5
3000c4ea:	4542      	cmp	r2, r8
3000c4ec:	464b      	mov	r3, r9
3000c4ee:	4641      	mov	r1, r8
3000c4f0:	d302      	bcc.n	3000c4f8 <__udivmoddi4+0x274>
3000c4f2:	d106      	bne.n	3000c502 <__udivmoddi4+0x27e>
3000c4f4:	454f      	cmp	r7, r9
3000c4f6:	d204      	bcs.n	3000c502 <__udivmoddi4+0x27e>
3000c4f8:	3801      	subs	r0, #1
3000c4fa:	ebb9 0305 	subs.w	r3, r9, r5
3000c4fe:	eb68 0104 	sbc.w	r1, r8, r4
3000c502:	b18e      	cbz	r6, 3000c528 <__udivmoddi4+0x2a4>
3000c504:	1afc      	subs	r4, r7, r3
3000c506:	eb62 0701 	sbc.w	r7, r2, r1
3000c50a:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c50e:	fa24 f30c 	lsr.w	r3, r4, ip
3000c512:	fa27 f70c 	lsr.w	r7, r7, ip
3000c516:	ea4e 0303 	orr.w	r3, lr, r3
3000c51a:	e9c6 3700 	strd	r3, r7, [r6]
3000c51e:	e79c      	b.n	3000c45a <__udivmoddi4+0x1d6>
3000c520:	4640      	mov	r0, r8
3000c522:	e7c7      	b.n	3000c4b4 <__udivmoddi4+0x230>
3000c524:	4641      	mov	r1, r8
3000c526:	e7db      	b.n	3000c4e0 <__udivmoddi4+0x25c>
3000c528:	4631      	mov	r1, r6
3000c52a:	e6fb      	b.n	3000c324 <__udivmoddi4+0xa0>

3000c52c <__aeabi_idiv0>:
3000c52c:	4770      	bx	lr
3000c52e:	bf00      	nop

3000c530 <_mbtowc_r>:
3000c530:	b430      	push	{r4, r5}
3000c532:	4c06      	ldr	r4, [pc, #24]	; (3000c54c <_mbtowc_r+0x1c>)
3000c534:	4d06      	ldr	r5, [pc, #24]	; (3000c550 <_mbtowc_r+0x20>)
3000c536:	6824      	ldr	r4, [r4, #0]
3000c538:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c53a:	2c00      	cmp	r4, #0
3000c53c:	bf08      	it	eq
3000c53e:	462c      	moveq	r4, r5
3000c540:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c544:	46a4      	mov	ip, r4
3000c546:	bc30      	pop	{r4, r5}
3000c548:	4760      	bx	ip
3000c54a:	bf00      	nop
3000c54c:	3000fb30 	.word	0x3000fb30
3000c550:	3000f9c4 	.word	0x3000f9c4

3000c554 <__ascii_mbtowc>:
3000c554:	b082      	sub	sp, #8
3000c556:	b901      	cbnz	r1, 3000c55a <__ascii_mbtowc+0x6>
3000c558:	a901      	add	r1, sp, #4
3000c55a:	b142      	cbz	r2, 3000c56e <__ascii_mbtowc+0x1a>
3000c55c:	b14b      	cbz	r3, 3000c572 <__ascii_mbtowc+0x1e>
3000c55e:	7813      	ldrb	r3, [r2, #0]
3000c560:	600b      	str	r3, [r1, #0]
3000c562:	7812      	ldrb	r2, [r2, #0]
3000c564:	1e10      	subs	r0, r2, #0
3000c566:	bf18      	it	ne
3000c568:	2001      	movne	r0, #1
3000c56a:	b002      	add	sp, #8
3000c56c:	4770      	bx	lr
3000c56e:	4610      	mov	r0, r2
3000c570:	e7fb      	b.n	3000c56a <__ascii_mbtowc+0x16>
3000c572:	f06f 0001 	mvn.w	r0, #1
3000c576:	e7f8      	b.n	3000c56a <__ascii_mbtowc+0x16>

3000c578 <_wctomb_r>:
3000c578:	b430      	push	{r4, r5}
3000c57a:	4c06      	ldr	r4, [pc, #24]	; (3000c594 <_wctomb_r+0x1c>)
3000c57c:	4d06      	ldr	r5, [pc, #24]	; (3000c598 <_wctomb_r+0x20>)
3000c57e:	6824      	ldr	r4, [r4, #0]
3000c580:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c582:	2c00      	cmp	r4, #0
3000c584:	bf08      	it	eq
3000c586:	462c      	moveq	r4, r5
3000c588:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c58c:	46a4      	mov	ip, r4
3000c58e:	bc30      	pop	{r4, r5}
3000c590:	4760      	bx	ip
3000c592:	bf00      	nop
3000c594:	3000fb30 	.word	0x3000fb30
3000c598:	3000f9c4 	.word	0x3000f9c4

3000c59c <__ascii_wctomb>:
3000c59c:	b149      	cbz	r1, 3000c5b2 <__ascii_wctomb+0x16>
3000c59e:	2aff      	cmp	r2, #255	; 0xff
3000c5a0:	bf8d      	iteet	hi
3000c5a2:	238a      	movhi	r3, #138	; 0x8a
3000c5a4:	2001      	movls	r0, #1
3000c5a6:	700a      	strbls	r2, [r1, #0]
3000c5a8:	6003      	strhi	r3, [r0, #0]
3000c5aa:	bf88      	it	hi
3000c5ac:	f04f 30ff 	movhi.w	r0, #4294967295
3000c5b0:	4770      	bx	lr
3000c5b2:	4608      	mov	r0, r1
3000c5b4:	4770      	bx	lr
3000c5b6:	bf00      	nop

3000c5b8 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000c5b8:	b401      	push	{r0}
3000c5ba:	4802      	ldr	r0, [pc, #8]	; (3000c5c4 <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000c5bc:	4684      	mov	ip, r0
3000c5be:	bc01      	pop	{r0}
3000c5c0:	4760      	bx	ip
3000c5c2:	bf00      	nop
3000c5c4:	00005b4d 	.word	0x00005b4d

3000c5c8 <____wrap_memcmp_veneer>:
3000c5c8:	b401      	push	{r0}
3000c5ca:	4802      	ldr	r0, [pc, #8]	; (3000c5d4 <____wrap_memcmp_veneer+0xc>)
3000c5cc:	4684      	mov	ip, r0
3000c5ce:	bc01      	pop	{r0}
3000c5d0:	4760      	bx	ip
3000c5d2:	bf00      	nop
3000c5d4:	00012f0d 	.word	0x00012f0d

3000c5d8 <__SBOOT_Validate_PubKey_B_veneer>:
3000c5d8:	b401      	push	{r0}
3000c5da:	4802      	ldr	r0, [pc, #8]	; (3000c5e4 <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000c5dc:	4684      	mov	ip, r0
3000c5de:	bc01      	pop	{r0}
3000c5e0:	4760      	bx	ip
3000c5e2:	bf00      	nop
3000c5e4:	00003495 	.word	0x00003495

3000c5e8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000c5e8:	b401      	push	{r0}
3000c5ea:	4802      	ldr	r0, [pc, #8]	; (3000c5f4 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000c5ec:	4684      	mov	ip, r0
3000c5ee:	bc01      	pop	{r0}
3000c5f0:	4760      	bx	ip
3000c5f2:	bf00      	nop
3000c5f4:	000024dd 	.word	0x000024dd

3000c5f8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000c5f8:	b401      	push	{r0}
3000c5fa:	4802      	ldr	r0, [pc, #8]	; (3000c604 <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000c5fc:	4684      	mov	ip, r0
3000c5fe:	bc01      	pop	{r0}
3000c600:	4760      	bx	ip
3000c602:	bf00      	nop
3000c604:	00002369 	.word	0x00002369

3000c608 <____wrap_memcpy_veneer>:
3000c608:	b401      	push	{r0}
3000c60a:	4802      	ldr	r0, [pc, #8]	; (3000c614 <____wrap_memcpy_veneer+0xc>)
3000c60c:	4684      	mov	ip, r0
3000c60e:	bc01      	pop	{r0}
3000c610:	4760      	bx	ip
3000c612:	bf00      	nop
3000c614:	00012f05 	.word	0x00012f05

3000c618 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000c618:	b401      	push	{r0}
3000c61a:	4802      	ldr	r0, [pc, #8]	; (3000c624 <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000c61c:	4684      	mov	ip, r0
3000c61e:	bc01      	pop	{r0}
3000c620:	4760      	bx	ip
3000c622:	bf00      	nop
3000c624:	00005a45 	.word	0x00005a45

3000c628 <__INT_HardFault_C_veneer>:
3000c628:	b401      	push	{r0}
3000c62a:	4802      	ldr	r0, [pc, #8]	; (3000c634 <__INT_HardFault_C_veneer+0xc>)
3000c62c:	4684      	mov	ip, r0
3000c62e:	bc01      	pop	{r0}
3000c630:	4760      	bx	ip
3000c632:	bf00      	nop
3000c634:	0000d8ed 	.word	0x0000d8ed

3000c638 <____wrap_memset_veneer>:
3000c638:	b401      	push	{r0}
3000c63a:	4802      	ldr	r0, [pc, #8]	; (3000c644 <____wrap_memset_veneer+0xc>)
3000c63c:	4684      	mov	ip, r0
3000c63e:	bc01      	pop	{r0}
3000c640:	4760      	bx	ip
3000c642:	bf00      	nop
3000c644:	00012efd 	.word	0x00012efd

3000c648 <__CRYPTO_SHA_Init_B_veneer>:
3000c648:	b401      	push	{r0}
3000c64a:	4802      	ldr	r0, [pc, #8]	; (3000c654 <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000c64c:	4684      	mov	ip, r0
3000c64e:	bc01      	pop	{r0}
3000c650:	4760      	bx	ip
3000c652:	bf00      	nop
3000c654:	00002451 	.word	0x00002451

3000c658 <__DiagVSprintf_veneer>:
3000c658:	b401      	push	{r0}
3000c65a:	4802      	ldr	r0, [pc, #8]	; (3000c664 <__DiagVSprintf_veneer+0xc>)
3000c65c:	4684      	mov	ip, r0
3000c65e:	bc01      	pop	{r0}
3000c660:	4760      	bx	ip
3000c662:	bf00      	nop
3000c664:	0000e331 	.word	0x0000e331

3000c668 <__rtl_crypto_sha2_update_B_veneer>:
3000c668:	b401      	push	{r0}
3000c66a:	4802      	ldr	r0, [pc, #8]	; (3000c674 <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000c66c:	4684      	mov	ip, r0
3000c66e:	bc01      	pop	{r0}
3000c670:	4760      	bx	ip
3000c672:	bf00      	nop
3000c674:	00005385 	.word	0x00005385

3000c678 <__rtl_crypto_sha2_final_A_veneer>:
3000c678:	b401      	push	{r0}
3000c67a:	4802      	ldr	r0, [pc, #8]	; (3000c684 <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000c67c:	4684      	mov	ip, r0
3000c67e:	bc01      	pop	{r0}
3000c680:	4760      	bx	ip
3000c682:	bf00      	nop
3000c684:	0000527d 	.word	0x0000527d

3000c688 <__DiagPrintf_veneer>:
3000c688:	b401      	push	{r0}
3000c68a:	4802      	ldr	r0, [pc, #8]	; (3000c694 <__DiagPrintf_veneer+0xc>)
3000c68c:	4684      	mov	ip, r0
3000c68e:	bc01      	pop	{r0}
3000c690:	4760      	bx	ip
3000c692:	bf00      	nop
3000c694:	0000e5f9 	.word	0x0000e5f9

3000c698 <__rtl_crypto_sha2_init_B_veneer>:
3000c698:	b401      	push	{r0}
3000c69a:	4802      	ldr	r0, [pc, #8]	; (3000c6a4 <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000c69c:	4684      	mov	ip, r0
3000c69e:	bc01      	pop	{r0}
3000c6a0:	4760      	bx	ip
3000c6a2:	bf00      	nop
3000c6a4:	00004ffd 	.word	0x00004ffd

3000c6a8 <__rtl_crypto_sha2_init_A_veneer>:
3000c6a8:	b401      	push	{r0}
3000c6aa:	4802      	ldr	r0, [pc, #8]	; (3000c6b4 <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000c6ac:	4684      	mov	ip, r0
3000c6ae:	bc01      	pop	{r0}
3000c6b0:	4760      	bx	ip
3000c6b2:	bf00      	nop
3000c6b4:	00004e89 	.word	0x00004e89

3000c6b8 <__SBOOT_Validate_Signature_A_veneer>:
3000c6b8:	b401      	push	{r0}
3000c6ba:	4802      	ldr	r0, [pc, #8]	; (3000c6c4 <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000c6bc:	4684      	mov	ip, r0
3000c6be:	bc01      	pop	{r0}
3000c6c0:	4760      	bx	ip
3000c6c2:	bf00      	nop
3000c6c4:	000032bd 	.word	0x000032bd

3000c6c8 <__SBOOT_Validate_ImgHash_B_veneer>:
3000c6c8:	b401      	push	{r0}
3000c6ca:	4802      	ldr	r0, [pc, #8]	; (3000c6d4 <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000c6cc:	4684      	mov	ip, r0
3000c6ce:	bc01      	pop	{r0}
3000c6d0:	4760      	bx	ip
3000c6d2:	bf00      	nop
3000c6d4:	00003541 	.word	0x00003541

3000c6d8 <____wrap_strlen_veneer>:
3000c6d8:	b401      	push	{r0}
3000c6da:	4802      	ldr	r0, [pc, #8]	; (3000c6e4 <____wrap_strlen_veneer+0xc>)
3000c6dc:	4684      	mov	ip, r0
3000c6de:	bc01      	pop	{r0}
3000c6e0:	4760      	bx	ip
3000c6e2:	bf00      	nop
3000c6e4:	00012f25 	.word	0x00012f25

3000c6e8 <__SBOOT_Validate_PubKey_A_veneer>:
3000c6e8:	b401      	push	{r0}
3000c6ea:	4802      	ldr	r0, [pc, #8]	; (3000c6f4 <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000c6ec:	4684      	mov	ip, r0
3000c6ee:	bc01      	pop	{r0}
3000c6f0:	4760      	bx	ip
3000c6f2:	bf00      	nop
3000c6f4:	00003321 	.word	0x00003321

3000c6f8 <__SBOOT_Validate_Algorithm_A_veneer>:
3000c6f8:	b401      	push	{r0}
3000c6fa:	4802      	ldr	r0, [pc, #8]	; (3000c704 <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000c6fc:	4684      	mov	ip, r0
3000c6fe:	bc01      	pop	{r0}
3000c700:	4760      	bx	ip
3000c702:	bf00      	nop
3000c704:	00003279 	.word	0x00003279

3000c708 <__io_assert_failed_veneer>:
3000c708:	b401      	push	{r0}
3000c70a:	4802      	ldr	r0, [pc, #8]	; (3000c714 <__io_assert_failed_veneer+0xc>)
3000c70c:	4684      	mov	ip, r0
3000c70e:	bc01      	pop	{r0}
3000c710:	4760      	bx	ip
3000c712:	bf00      	nop
3000c714:	0000992d 	.word	0x0000992d

3000c718 <__rtl_crypto_sha2_final_B_veneer>:
3000c718:	b401      	push	{r0}
3000c71a:	4802      	ldr	r0, [pc, #8]	; (3000c724 <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000c71c:	4684      	mov	ip, r0
3000c71e:	bc01      	pop	{r0}
3000c720:	4760      	bx	ip
3000c722:	bf00      	nop
3000c724:	000053f1 	.word	0x000053f1

3000c728 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000c728:	b401      	push	{r0}
3000c72a:	4802      	ldr	r0, [pc, #8]	; (3000c734 <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000c72c:	4684      	mov	ip, r0
3000c72e:	bc01      	pop	{r0}
3000c730:	4760      	bx	ip
3000c732:	bf00      	nop
3000c734:	00005bb9 	.word	0x00005bb9

3000c738 <__SBOOT_Validate_ImgHash_A_veneer>:
3000c738:	b401      	push	{r0}
3000c73a:	4802      	ldr	r0, [pc, #8]	; (3000c744 <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000c73c:	4684      	mov	ip, r0
3000c73e:	bc01      	pop	{r0}
3000c740:	4760      	bx	ip
3000c742:	bf00      	nop
3000c744:	000033cd 	.word	0x000033cd

3000c748 <__SBOOT_GetMdType_B_veneer>:
3000c748:	b401      	push	{r0}
3000c74a:	4802      	ldr	r0, [pc, #8]	; (3000c754 <__SBOOT_GetMdType_B_veneer+0xc>)
3000c74c:	4684      	mov	ip, r0
3000c74e:	bc01      	pop	{r0}
3000c750:	4760      	bx	ip
3000c752:	bf00      	nop
3000c754:	0000319d 	.word	0x0000319d

3000c758 <__rtl_crypto_sha2_update_A_veneer>:
3000c758:	b401      	push	{r0}
3000c75a:	4802      	ldr	r0, [pc, #8]	; (3000c764 <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000c75c:	4684      	mov	ip, r0
3000c75e:	bc01      	pop	{r0}
3000c760:	4760      	bx	ip
3000c762:	bf00      	nop
3000c764:	00005211 	.word	0x00005211

3000c768 <__SBOOT_GetMdType_A_veneer>:
3000c768:	b401      	push	{r0}
3000c76a:	4802      	ldr	r0, [pc, #8]	; (3000c774 <__SBOOT_GetMdType_A_veneer+0xc>)
3000c76c:	4684      	mov	ip, r0
3000c76e:	bc01      	pop	{r0}
3000c770:	4760      	bx	ip
3000c772:	bf00      	nop
3000c774:	00003029 	.word	0x00003029

3000c778 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000c778:	b401      	push	{r0}
3000c77a:	4802      	ldr	r0, [pc, #8]	; (3000c784 <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000c77c:	4684      	mov	ip, r0
3000c77e:	bc01      	pop	{r0}
3000c780:	4760      	bx	ip
3000c782:	bf00      	nop
3000c784:	000059d9 	.word	0x000059d9

3000c788 <__SBOOT_Validate_Algorithm_B_veneer>:
3000c788:	b401      	push	{r0}
3000c78a:	4802      	ldr	r0, [pc, #8]	; (3000c794 <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000c78c:	4684      	mov	ip, r0
3000c78e:	bc01      	pop	{r0}
3000c790:	4760      	bx	ip
3000c792:	bf00      	nop
3000c794:	000033ed 	.word	0x000033ed

3000c798 <__CRYPTO_SHA_Init_A_veneer>:
3000c798:	b401      	push	{r0}
3000c79a:	4802      	ldr	r0, [pc, #8]	; (3000c7a4 <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000c79c:	4684      	mov	ip, r0
3000c79e:	bc01      	pop	{r0}
3000c7a0:	4760      	bx	ip
3000c7a2:	bf00      	nop
3000c7a4:	000022dd 	.word	0x000022dd

3000c7a8 <__SBOOT_Validate_Signature_B_veneer>:
3000c7a8:	b401      	push	{r0}
3000c7aa:	4802      	ldr	r0, [pc, #8]	; (3000c7b4 <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000c7ac:	4684      	mov	ip, r0
3000c7ae:	bc01      	pop	{r0}
3000c7b0:	4760      	bx	ip
3000c7b2:	bf00      	nop
3000c7b4:	00003431 	.word	0x00003431
3000c7b8:	004f4931 	.word	0x004f4931
3000c7bc:	00004f32 	.word	0x00004f32
3000c7c0:	004f4932 	.word	0x004f4932
3000c7c4:	00004f34 	.word	0x00004f34
3000c7c8:	004f4934 	.word	0x004f4934
3000c7cc:	53414c46 	.word	0x53414c46
3000c7d0:	4b4c4348 	.word	0x4b4c4348
3000c7d4:	00000000 	.word	0x00000000
3000c7d8:	2d73255b 	.word	0x2d73255b
3000c7dc:	63205d49 	.word	0x63205d49
3000c7e0:	62696c61 	.word	0x62696c61
3000c7e4:	69746172 	.word	0x69746172
3000c7e8:	6f5f6e6f 	.word	0x6f5f6e6f
3000c7ec:	255b3a6b 	.word	0x255b3a6b
3000c7f0:	64253a64 	.word	0x64253a64
3000c7f4:	5d64253a 	.word	0x5d64253a
3000c7f8:	00000a20 	.word	0x00000a20
3000c7fc:	2d73255b 	.word	0x2d73255b
3000c800:	46205d49 	.word	0x46205d49
3000c804:	4853414c 	.word	0x4853414c
3000c808:	4c414320 	.word	0x4c414320
3000c80c:	305b4249 	.word	0x305b4249
3000c810:	20782578 	.word	0x20782578
3000c814:	0a5d4b4f 	.word	0x0a5d4b4f
3000c818:	00000000 	.word	0x00000000
3000c81c:	2d73255b 	.word	0x2d73255b
3000c820:	46205d45 	.word	0x46205d45
3000c824:	4853414c 	.word	0x4853414c
3000c828:	4c414320 	.word	0x4c414320
3000c82c:	305b4249 	.word	0x305b4249
3000c830:	20782578 	.word	0x20782578
3000c834:	4c494146 	.word	0x4c494146
3000c838:	00000a5d 	.word	0x00000a5d
3000c83c:	2d73255b 	.word	0x2d73255b
3000c840:	46205d49 	.word	0x46205d49
3000c844:	6873616c 	.word	0x6873616c
3000c848:	61655220 	.word	0x61655220
3000c84c:	73252064 	.word	0x73252064
3000c850:	0000000a 	.word	0x0000000a
3000c854:	2d73255b 	.word	0x2d73255b
3000c858:	46205d45 	.word	0x46205d45
3000c85c:	6873616c 	.word	0x6873616c
3000c860:	69775320 	.word	0x69775320
3000c864:	20686374 	.word	0x20686374
3000c868:	64616552 	.word	0x64616552
3000c86c:	646f4d20 	.word	0x646f4d20
3000c870:	41462065 	.word	0x41462065
3000c874:	000a4c49 	.word	0x000a4c49
3000c878:	2d73255b 	.word	0x2d73255b
3000c87c:	46205d49 	.word	0x46205d49
3000c880:	6873616c 	.word	0x6873616c
3000c884:	3a444920 	.word	0x3a444920
3000c888:	2d782520 	.word	0x2d782520
3000c88c:	252d7825 	.word	0x252d7825
3000c890:	00000a78 	.word	0x00000a78
3000c894:	2d73255b 	.word	0x2d73255b
3000c898:	54205d57 	.word	0x54205d57
3000c89c:	20736968 	.word	0x20736968
3000c8a0:	73616c66 	.word	0x73616c66
3000c8a4:	79742068 	.word	0x79742068
3000c8a8:	69206570 	.word	0x69206570
3000c8ac:	6f6e2073 	.word	0x6f6e2073
3000c8b0:	75732074 	.word	0x75732074
3000c8b4:	726f7070 	.word	0x726f7070
3000c8b8:	21646574 	.word	0x21646574
3000c8bc:	0000000a 	.word	0x0000000a
3000c8c0:	2d73255b 	.word	0x2d73255b
3000c8c4:	4e205d49 	.word	0x4e205d49
3000c8c8:	20444e41 	.word	0x20444e41
3000c8cc:	203a4449 	.word	0x203a4449
3000c8d0:	252d7825 	.word	0x252d7825
3000c8d4:	00000a78 	.word	0x00000a78
3000c8d8:	00003552 	.word	0x00003552
3000c8dc:	00003652 	.word	0x00003652
3000c8e0:	00003752 	.word	0x00003752
3000c8e4:	00003852 	.word	0x00003852
3000c8e8:	00003952 	.word	0x00003952
3000c8ec:	00303152 	.word	0x00303152
3000c8f0:	00313152 	.word	0x00313152
3000c8f4:	00003152 	.word	0x00003152
3000c8f8:	00003252 	.word	0x00003252
3000c8fc:	00003352 	.word	0x00003352
3000c900:	20304d4b 	.word	0x20304d4b
3000c904:	20504958 	.word	0x20504958
3000c908:	00474d49 	.word	0x00474d49
3000c90c:	20304d4b 	.word	0x20304d4b
3000c910:	4d415253 	.word	0x4d415253
3000c914:	00000000 	.word	0x00000000
3000c918:	20304d4b 	.word	0x20304d4b
3000c91c:	4d415244 	.word	0x4d415244
3000c920:	00000000 	.word	0x00000000
3000c924:	20344d4b 	.word	0x20344d4b
3000c928:	20504958 	.word	0x20504958
3000c92c:	00474d49 	.word	0x00474d49
3000c930:	20344d4b 	.word	0x20344d4b
3000c934:	4d415253 	.word	0x4d415253
3000c938:	00000000 	.word	0x00000000
3000c93c:	20344d4b 	.word	0x20344d4b
3000c940:	4d415244 	.word	0x4d415244
3000c944:	00000000 	.word	0x00000000
3000c948:	58205041 	.word	0x58205041
3000c94c:	49205049 	.word	0x49205049
3000c950:	0000474d 	.word	0x0000474d
3000c954:	42205041 	.word	0x42205041
3000c958:	5320314c 	.word	0x5320314c
3000c95c:	004d4152 	.word	0x004d4152
3000c960:	42205041 	.word	0x42205041
3000c964:	4420314c 	.word	0x4420314c
3000c968:	004d4152 	.word	0x004d4152
3000c96c:	46205041 	.word	0x46205041
3000c970:	00005049 	.word	0x00005049
3000c974:	544f4f42 	.word	0x544f4f42
3000c978:	00000000 	.word	0x00000000
3000c97c:	2d73255b 	.word	0x2d73255b
3000c980:	25205d49 	.word	0x25205d49
3000c984:	6e492073 	.word	0x6e492073
3000c988:	696c6176 	.word	0x696c6176
3000c98c:	00000a64 	.word	0x00000a64
3000c990:	2d73255b 	.word	0x2d73255b
3000c994:	25205d49 	.word	0x25205d49
3000c998:	30255b73 	.word	0x30255b73
3000c99c:	253a7838 	.word	0x253a7838
3000c9a0:	000a5d78 	.word	0x000a5d78
3000c9a4:	2d73255b 	.word	0x2d73255b
3000c9a8:	52205d49 	.word	0x52205d49
3000c9ac:	45205044 	.word	0x45205044
3000c9b0:	00000a4e 	.word	0x00000a4e
3000c9b4:	2d73255b 	.word	0x2d73255b
3000c9b8:	46205d49 	.word	0x46205d49
3000c9bc:	206c6961 	.word	0x206c6961
3000c9c0:	6c206f74 	.word	0x6c206f74
3000c9c4:	2064616f 	.word	0x2064616f
3000c9c8:	20504452 	.word	0x20504452
3000c9cc:	67616d69 	.word	0x67616d69
3000c9d0:	000a2165 	.word	0x000a2165
3000c9d4:	2d73255b 	.word	0x2d73255b
3000c9d8:	52205d49 	.word	0x52205d49
3000c9dc:	4f205044 	.word	0x4f205044
3000c9e0:	000a4646 	.word	0x000a4646
3000c9e4:	2d73255b 	.word	0x2d73255b
3000c9e8:	49205d49 	.word	0x49205d49
3000c9ec:	2032474d 	.word	0x2032474d
3000c9f0:	2046544f 	.word	0x2046544f
3000c9f4:	000a4e45 	.word	0x000a4e45
3000c9f8:	2d73255b 	.word	0x2d73255b
3000c9fc:	4f205d45 	.word	0x4f205d45
3000ca00:	4d204654 	.word	0x4d204654
3000ca04:	2065646f 	.word	0x2065646f
3000ca08:	6f727265 	.word	0x6f727265
3000ca0c:	000a0d72 	.word	0x000a0d72
3000ca10:	494c4156 	.word	0x494c4156
3000ca14:	00000044 	.word	0x00000044
3000ca18:	41564e49 	.word	0x41564e49
3000ca1c:	0044494c 	.word	0x0044494c
3000ca20:	74726563 	.word	0x74726563
3000ca24:	63696669 	.word	0x63696669
3000ca28:	20657461 	.word	0x20657461
3000ca2c:	20434345 	.word	0x20434345
3000ca30:	69726576 	.word	0x69726576
3000ca34:	66207966 	.word	0x66207966
3000ca38:	2c6c6961 	.word	0x2c6c6961
3000ca3c:	6c6f7220 	.word	0x6c6f7220
3000ca40:	6361626c 	.word	0x6361626c
3000ca44:	000a0d6b 	.word	0x000a0d6b
3000ca48:	444f4d5b 	.word	0x444f4d5b
3000ca4c:	5f454c55 	.word	0x5f454c55
3000ca50:	544f4f42 	.word	0x544f4f42
3000ca54:	56454c2d 	.word	0x56454c2d
3000ca58:	495f4c45 	.word	0x495f4c45
3000ca5c:	5d4f464e 	.word	0x5d4f464e
3000ca60:	474d493a 	.word	0x474d493a
3000ca64:	544f2832 	.word	0x544f2832
3000ca68:	29642541 	.word	0x29642541
3000ca6c:	2c732520 	.word	0x2c732520
3000ca70:	74657220 	.word	0x74657220
3000ca74:	6425203a 	.word	0x6425203a
3000ca78:	0000000a 	.word	0x0000000a
3000ca7c:	32474d49 	.word	0x32474d49
3000ca80:	43434520 	.word	0x43434520
3000ca84:	72657620 	.word	0x72657620
3000ca88:	20796669 	.word	0x20796669
3000ca8c:	6c696166 	.word	0x6c696166
3000ca90:	6f72202c 	.word	0x6f72202c
3000ca94:	61626c6c 	.word	0x61626c6c
3000ca98:	0a0d6b63 	.word	0x0a0d6b63
3000ca9c:	00000000 	.word	0x00000000
3000caa0:	5f41544f 	.word	0x5f41544f
3000caa4:	45205041 	.word	0x45205041
3000caa8:	76204343 	.word	0x76204343
3000caac:	66697265 	.word	0x66697265
3000cab0:	61662079 	.word	0x61662079
3000cab4:	202c6c69 	.word	0x202c6c69
3000cab8:	6c6c6f72 	.word	0x6c6c6f72
3000cabc:	6b636162 	.word	0x6b636162
3000cac0:	00000a0d 	.word	0x00000a0d
3000cac4:	64616f4c 	.word	0x64616f4c
3000cac8:	41544f20 	.word	0x41544f20
3000cacc:	2050415f 	.word	0x2050415f
3000cad0:	67616d69 	.word	0x67616d69
3000cad4:	61662065 	.word	0x61662065
3000cad8:	202c6c69 	.word	0x202c6c69
3000cadc:	6c6c6f72 	.word	0x6c6c6f72
3000cae0:	6b636162 	.word	0x6b636162
3000cae4:	00000a0d 	.word	0x00000a0d
3000cae8:	444f4d5b 	.word	0x444f4d5b
3000caec:	5f454c55 	.word	0x5f454c55
3000caf0:	544f4f42 	.word	0x544f4f42
3000caf4:	56454c2d 	.word	0x56454c2d
3000caf8:	455f4c45 	.word	0x455f4c45
3000cafc:	524f5252 	.word	0x524f5252
3000cb00:	61463a5d 	.word	0x61463a5d
3000cb04:	74206c69 	.word	0x74206c69
3000cb08:	6f6c206f 	.word	0x6f6c206f
3000cb0c:	41206461 	.word	0x41206461
3000cb10:	6d692050 	.word	0x6d692050
3000cb14:	21656761 	.word	0x21656761
3000cb18:	0000000a 	.word	0x0000000a
3000cb1c:	20344d4b 	.word	0x20344d4b
3000cb20:	33474d49 	.word	0x33474d49
3000cb24:	00000000 	.word	0x00000000
3000cb28:	20344d4b 	.word	0x20344d4b
3000cb2c:	0043534e 	.word	0x0043534e
3000cb30:	31474d49 	.word	0x31474d49
3000cb34:	00000000 	.word	0x00000000
3000cb38:	32474d49 	.word	0x32474d49
3000cb3c:	00000000 	.word	0x00000000
3000cb40:	42205041 	.word	0x42205041
3000cb44:	4626314c 	.word	0x4626314c
3000cb48:	00005049 	.word	0x00005049
3000cb4c:	444f4d5b 	.word	0x444f4d5b
3000cb50:	5f454c55 	.word	0x5f454c55
3000cb54:	544f4f42 	.word	0x544f4f42
3000cb58:	56454c2d 	.word	0x56454c2d
3000cb5c:	495f4c45 	.word	0x495f4c45
3000cb60:	5d4f464e 	.word	0x5d4f464e
3000cb64:	5b73253a 	.word	0x5b73253a
3000cb68:	78383025 	.word	0x78383025
3000cb6c:	5d78253a 	.word	0x5d78253a
3000cb70:	0000000a 	.word	0x0000000a
3000cb74:	20504452 	.word	0x20504452
3000cb78:	4c494146 	.word	0x4c494146
3000cb7c:	0000000a 	.word	0x0000000a
3000cb80:	20504452 	.word	0x20504452
3000cb84:	43435553 	.word	0x43435553
3000cb88:	0a535345 	.word	0x0a535345
3000cb8c:	00000000 	.word	0x00000000
3000cb90:	059e4eb3 	.word	0x059e4eb3
3000cb94:	4b11ea39 	.word	0x4b11ea39
3000cb98:	2a1b920a 	.word	0x2a1b920a
3000cb9c:	29939cee 	.word	0x29939cee
3000cba0:	00000000 	.word	0x00000000
3000cba4:	42205041 	.word	0x42205041
3000cba8:	0032334c 	.word	0x0032334c
3000cbac:	2d73255b 	.word	0x2d73255b
3000cbb0:	25205d49 	.word	0x25205d49
3000cbb4:	45562073 	.word	0x45562073
3000cbb8:	59464952 	.word	0x59464952
3000cbbc:	53415020 	.word	0x53415020
3000cbc0:	00000a53 	.word	0x00000a53
3000cbc4:	2d73255b 	.word	0x2d73255b
3000cbc8:	25205d45 	.word	0x25205d45
3000cbcc:	45562073 	.word	0x45562073
3000cbd0:	59464952 	.word	0x59464952
3000cbd4:	49414620 	.word	0x49414620
3000cbd8:	72202c4c 	.word	0x72202c4c
3000cbdc:	3d207465 	.word	0x3d207465
3000cbe0:	0a642520 	.word	0x0a642520
3000cbe4:	00000000 	.word	0x00000000
3000cbe8:	2d73255b 	.word	0x2d73255b
3000cbec:	43205d45 	.word	0x43205d45
3000cbf0:	69747265 	.word	0x69747265
3000cbf4:	61636966 	.word	0x61636966
3000cbf8:	56206574 	.word	0x56206574
3000cbfc:	46495245 	.word	0x46495245
3000cc00:	41462059 	.word	0x41462059
3000cc04:	202c4c49 	.word	0x202c4c49
3000cc08:	20746572 	.word	0x20746572
3000cc0c:	6425203d 	.word	0x6425203d
3000cc10:	0000000a 	.word	0x0000000a
3000cc14:	2d73255b 	.word	0x2d73255b
3000cc18:	48205d41 	.word	0x48205d41
3000cc1c:	20647261 	.word	0x20647261
3000cc20:	6c756146 	.word	0x6c756146
3000cc24:	61502074 	.word	0x61502074
3000cc28:	20686374 	.word	0x20686374
3000cc2c:	63655328 	.word	0x63655328
3000cc30:	29657275 	.word	0x29657275
3000cc34:	00000a0d 	.word	0x00000a0d
3000cc38:	2d73255b 	.word	0x2d73255b
3000cc3c:	45205d41 	.word	0x45205d41
3000cc40:	70656378 	.word	0x70656378
3000cc44:	6e6f6974 	.word	0x6e6f6974
3000cc48:	6f726620 	.word	0x6f726620
3000cc4c:	6573206d 	.word	0x6573206d
3000cc50:	65727563 	.word	0x65727563
3000cc54:	726f7720 	.word	0x726f7720
3000cc58:	0d21646c 	.word	0x0d21646c
3000cc5c:	0000000a 	.word	0x0000000a
3000cc60:	2d73255b 	.word	0x2d73255b
3000cc64:	45205d41 	.word	0x45205d41
3000cc68:	70656378 	.word	0x70656378
3000cc6c:	6e6f6974 	.word	0x6e6f6974
3000cc70:	6f726620 	.word	0x6f726620
3000cc74:	6f6e206d 	.word	0x6f6e206d
3000cc78:	65732d6e 	.word	0x65732d6e
3000cc7c:	65727563 	.word	0x65727563
3000cc80:	726f7720 	.word	0x726f7720
3000cc84:	0d21646c 	.word	0x0d21646c
3000cc88:	0000000a 	.word	0x0000000a
3000cc8c:	2d73255b 	.word	0x2d73255b
3000cc90:	53205d41 	.word	0x53205d41
3000cc94:	20525346 	.word	0x20525346
3000cc98:	30203d20 	.word	0x30203d20
3000cc9c:	38302578 	.word	0x38302578
3000cca0:	000a0d78 	.word	0x000a0d78
3000cca4:	2d73255b 	.word	0x2d73255b
3000cca8:	53205d41 	.word	0x53205d41
3000ccac:	20524146 	.word	0x20524146
3000ccb0:	30203d20 	.word	0x30203d20
3000ccb4:	38302578 	.word	0x38302578
3000ccb8:	000a0d78 	.word	0x000a0d78
3000ccbc:	2d73255b 	.word	0x2d73255b
3000ccc0:	53205d41 	.word	0x53205d41
3000ccc4:	52534348 	.word	0x52534348
3000ccc8:	30203d20 	.word	0x30203d20
3000cccc:	38302578 	.word	0x38302578
3000ccd0:	000a0d78 	.word	0x000a0d78
3000ccd4:	3141544f 	.word	0x3141544f
3000ccd8:	45535520 	.word	0x45535520
3000ccdc:	6576202c 	.word	0x6576202c
3000cce0:	6f697372 	.word	0x6f697372
3000cce4:	25203a6e 	.word	0x25203a6e
3000cce8:	00000a64 	.word	0x00000a64
3000ccec:	3241544f 	.word	0x3241544f
3000ccf0:	45535520 	.word	0x45535520
3000ccf4:	6576202c 	.word	0x6576202c
3000ccf8:	6f697372 	.word	0x6f697372
3000ccfc:	25203a6e 	.word	0x25203a6e
3000cd00:	00000a64 	.word	0x00000a64
3000cd04:	68746f42 	.word	0x68746f42
3000cd08:	41544f20 	.word	0x41544f20
3000cd0c:	616d6920 	.word	0x616d6920
3000cd10:	76206567 	.word	0x76206567
3000cd14:	66697265 	.word	0x66697265
3000cd18:	61662079 	.word	0x61662079
3000cd1c:	64656c69 	.word	0x64656c69
3000cd20:	0000000a 	.word	0x0000000a
3000cd24:	2d73255b 	.word	0x2d73255b
3000cd28:	49205d49 	.word	0x49205d49
3000cd2c:	2074696e 	.word	0x2074696e
3000cd30:	0a0d4257 	.word	0x0a0d4257
3000cd34:	00000000 	.word	0x00000000
3000cd38:	2d73255b 	.word	0x2d73255b
3000cd3c:	52205d49 	.word	0x52205d49
3000cd40:	696e4965 	.word	0x696e4965
3000cd44:	44442074 	.word	0x44442074
3000cd48:	000a0d52 	.word	0x000a0d52
3000cd4c:	73616c66 	.word	0x73616c66
3000cd50:	69735f68 	.word	0x69735f68
3000cd54:	203a657a 	.word	0x203a657a
3000cd58:	000a4d38 	.word	0x000a4d38
3000cd5c:	73616c66 	.word	0x73616c66
3000cd60:	69735f68 	.word	0x69735f68
3000cd64:	203a657a 	.word	0x203a657a
3000cd68:	0a4d3631 	.word	0x0a4d3631
3000cd6c:	00000000 	.word	0x00000000
3000cd70:	20315042 	.word	0x20315042
3000cd74:	73726576 	.word	0x73726576
3000cd78:	206e6f69 	.word	0x206e6f69
3000cd7c:	61766e69 	.word	0x61766e69
3000cd80:	0a64696c 	.word	0x0a64696c
3000cd84:	00000000 	.word	0x00000000
3000cd88:	20315042 	.word	0x20315042
3000cd8c:	6d726f66 	.word	0x6d726f66
3000cd90:	69207461 	.word	0x69207461
3000cd94:	6c61766e 	.word	0x6c61766e
3000cd98:	000a6469 	.word	0x000a6469
3000cd9c:	20315042 	.word	0x20315042
3000cda0:	69746361 	.word	0x69746361
3000cda4:	69206576 	.word	0x69206576
3000cda8:	7865646e 	.word	0x7865646e
3000cdac:	766e6920 	.word	0x766e6920
3000cdb0:	64696c61 	.word	0x64696c61
3000cdb4:	0000000a 	.word	0x0000000a
3000cdb8:	20315042 	.word	0x20315042
3000cdbc:	61746164 	.word	0x61746164
3000cdc0:	6c617620 	.word	0x6c617620
3000cdc4:	202c6469 	.word	0x202c6469
3000cdc8:	73726576 	.word	0x73726576
3000cdcc:	3a6e6f69 	.word	0x3a6e6f69
3000cdd0:	000a6425 	.word	0x000a6425
3000cdd4:	20325042 	.word	0x20325042
3000cdd8:	6d726f66 	.word	0x6d726f66
3000cddc:	69207461 	.word	0x69207461
3000cde0:	6c61766e 	.word	0x6c61766e
3000cde4:	000a6469 	.word	0x000a6469
3000cde8:	20315042 	.word	0x20315042
3000cdec:	33435243 	.word	0x33435243
3000cdf0:	616d2032 	.word	0x616d2032
3000cdf4:	2c686374 	.word	0x2c686374
3000cdf8:	74746120 	.word	0x74746120
3000cdfc:	65686361 	.word	0x65686361
3000ce00:	52432064 	.word	0x52432064
3000ce04:	30203a43 	.word	0x30203a43
3000ce08:	2c782578 	.word	0x2c782578
3000ce0c:	6c616320 	.word	0x6c616320
3000ce10:	616c7563 	.word	0x616c7563
3000ce14:	20646574 	.word	0x20646574
3000ce18:	3a435243 	.word	0x3a435243
3000ce1c:	25783020 	.word	0x25783020
3000ce20:	00000a78 	.word	0x00000a78
3000ce24:	20315042 	.word	0x20315042
3000ce28:	33435243 	.word	0x33435243
3000ce2c:	6f6e2032 	.word	0x6f6e2032
3000ce30:	616d2074 	.word	0x616d2074
3000ce34:	2c686374 	.word	0x2c686374
3000ce38:	74746120 	.word	0x74746120
3000ce3c:	65686361 	.word	0x65686361
3000ce40:	52432064 	.word	0x52432064
3000ce44:	30203a43 	.word	0x30203a43
3000ce48:	2c782578 	.word	0x2c782578
3000ce4c:	6c616320 	.word	0x6c616320
3000ce50:	616c7563 	.word	0x616c7563
3000ce54:	20646574 	.word	0x20646574
3000ce58:	3a435243 	.word	0x3a435243
3000ce5c:	25783020 	.word	0x25783020
3000ce60:	00000a78 	.word	0x00000a78
3000ce64:	20325042 	.word	0x20325042
3000ce68:	33435243 	.word	0x33435243
3000ce6c:	616d2032 	.word	0x616d2032
3000ce70:	2c686374 	.word	0x2c686374
3000ce74:	74746120 	.word	0x74746120
3000ce78:	65686361 	.word	0x65686361
3000ce7c:	52432064 	.word	0x52432064
3000ce80:	30203a43 	.word	0x30203a43
3000ce84:	2c782578 	.word	0x2c782578
3000ce88:	6c616320 	.word	0x6c616320
3000ce8c:	616c7563 	.word	0x616c7563
3000ce90:	20646574 	.word	0x20646574
3000ce94:	3a435243 	.word	0x3a435243
3000ce98:	25783020 	.word	0x25783020
3000ce9c:	00000a78 	.word	0x00000a78
3000cea0:	20325042 	.word	0x20325042
3000cea4:	33435243 	.word	0x33435243
3000cea8:	6f6e2032 	.word	0x6f6e2032
3000ceac:	616d2074 	.word	0x616d2074
3000ceb0:	2c686374 	.word	0x2c686374
3000ceb4:	74746120 	.word	0x74746120
3000ceb8:	65686361 	.word	0x65686361
3000cebc:	52432064 	.word	0x52432064
3000cec0:	30203a43 	.word	0x30203a43
3000cec4:	2c782578 	.word	0x2c782578
3000cec8:	6c616320 	.word	0x6c616320
3000cecc:	616c7563 	.word	0x616c7563
3000ced0:	20646574 	.word	0x20646574
3000ced4:	3a435243 	.word	0x3a435243
3000ced8:	25783020 	.word	0x25783020
3000cedc:	00000a78 	.word	0x00000a78
3000cee0:	20315042 	.word	0x20315042
3000cee4:	3161746f 	.word	0x3161746f
3000cee8:	64646120 	.word	0x64646120
3000ceec:	73736572 	.word	0x73736572
3000cef0:	766e6920 	.word	0x766e6920
3000cef4:	64696c61 	.word	0x64696c61
3000cef8:	0000000a 	.word	0x0000000a
3000cefc:	20315042 	.word	0x20315042
3000cf00:	3261746f 	.word	0x3261746f
3000cf04:	64646120 	.word	0x64646120
3000cf08:	73736572 	.word	0x73736572
3000cf0c:	766e6920 	.word	0x766e6920
3000cf10:	64696c61 	.word	0x64696c61
3000cf14:	0000000a 	.word	0x0000000a
3000cf18:	68746f42 	.word	0x68746f42
3000cf1c:	20504220 	.word	0x20504220
3000cf20:	20435243 	.word	0x20435243
3000cf24:	696c6176 	.word	0x696c6176
3000cf28:	00000a64 	.word	0x00000a64
3000cf2c:	20315042 	.word	0x20315042
3000cf30:	656c6573 	.word	0x656c6573
3000cf34:	64657463 	.word	0x64657463
3000cf38:	0000000a 	.word	0x0000000a
3000cf3c:	20325042 	.word	0x20325042
3000cf40:	656c6573 	.word	0x656c6573
3000cf44:	64657463 	.word	0x64657463
3000cf48:	0000000a 	.word	0x0000000a
3000cf4c:	68746f42 	.word	0x68746f42
3000cf50:	20504220 	.word	0x20504220
3000cf54:	61766e69 	.word	0x61766e69
3000cf58:	0a64696c 	.word	0x0a64696c
3000cf5c:	00000000 	.word	0x00000000
3000cf60:	6f636552 	.word	0x6f636552
3000cf64:	79726576 	.word	0x79726576
3000cf68:	61747320 	.word	0x61747320
3000cf6c:	000a7472 	.word	0x000a7472
3000cf70:	69746341 	.word	0x69746341
3000cf74:	49206576 	.word	0x49206576
3000cf78:	7865646e 	.word	0x7865646e
3000cf7c:	6425203a 	.word	0x6425203a
3000cf80:	0000000a 	.word	0x0000000a
3000cf84:	3141544f 	.word	0x3141544f
3000cf88:	766e6920 	.word	0x766e6920
3000cf8c:	64696c61 	.word	0x64696c61
3000cf90:	0000000a 	.word	0x0000000a
3000cf94:	3241544f 	.word	0x3241544f
3000cf98:	766e6920 	.word	0x766e6920
3000cf9c:	64696c61 	.word	0x64696c61
3000cfa0:	0000000a 	.word	0x0000000a
3000cfa4:	3141544f 	.word	0x3141544f
3000cfa8:	43524320 	.word	0x43524320
3000cfac:	6d203233 	.word	0x6d203233
3000cfb0:	68637461 	.word	0x68637461
3000cfb4:	7461202c 	.word	0x7461202c
3000cfb8:	68636174 	.word	0x68636174
3000cfbc:	43206465 	.word	0x43206465
3000cfc0:	203a4352 	.word	0x203a4352
3000cfc4:	78257830 	.word	0x78257830
3000cfc8:	6163202c 	.word	0x6163202c
3000cfcc:	6c75636c 	.word	0x6c75636c
3000cfd0:	64657461 	.word	0x64657461
3000cfd4:	43524320 	.word	0x43524320
3000cfd8:	7830203a 	.word	0x7830203a
3000cfdc:	000a7825 	.word	0x000a7825
3000cfe0:	3141544f 	.word	0x3141544f
3000cfe4:	43524320 	.word	0x43524320
3000cfe8:	6e203233 	.word	0x6e203233
3000cfec:	6d20746f 	.word	0x6d20746f
3000cff0:	68637461 	.word	0x68637461
3000cff4:	7461202c 	.word	0x7461202c
3000cff8:	68636174 	.word	0x68636174
3000cffc:	43206465 	.word	0x43206465
3000d000:	203a4352 	.word	0x203a4352
3000d004:	78257830 	.word	0x78257830
3000d008:	6163202c 	.word	0x6163202c
3000d00c:	6c75636c 	.word	0x6c75636c
3000d010:	64657461 	.word	0x64657461
3000d014:	43524320 	.word	0x43524320
3000d018:	7830203a 	.word	0x7830203a
3000d01c:	202c7825 	.word	0x202c7825
3000d020:	73617265 	.word	0x73617265
3000d024:	544f2065 	.word	0x544f2065
3000d028:	000a3141 	.word	0x000a3141
3000d02c:	3241544f 	.word	0x3241544f
3000d030:	43524320 	.word	0x43524320
3000d034:	6d203233 	.word	0x6d203233
3000d038:	68637461 	.word	0x68637461
3000d03c:	7461202c 	.word	0x7461202c
3000d040:	68636174 	.word	0x68636174
3000d044:	43206465 	.word	0x43206465
3000d048:	203a4352 	.word	0x203a4352
3000d04c:	78257830 	.word	0x78257830
3000d050:	6163202c 	.word	0x6163202c
3000d054:	6c75636c 	.word	0x6c75636c
3000d058:	64657461 	.word	0x64657461
3000d05c:	43524320 	.word	0x43524320
3000d060:	7830203a 	.word	0x7830203a
3000d064:	000a7825 	.word	0x000a7825
3000d068:	3241544f 	.word	0x3241544f
3000d06c:	43524320 	.word	0x43524320
3000d070:	6e203233 	.word	0x6e203233
3000d074:	6d20746f 	.word	0x6d20746f
3000d078:	68637461 	.word	0x68637461
3000d07c:	7461202c 	.word	0x7461202c
3000d080:	68636174 	.word	0x68636174
3000d084:	43206465 	.word	0x43206465
3000d088:	203a4352 	.word	0x203a4352
3000d08c:	78257830 	.word	0x78257830
3000d090:	6163202c 	.word	0x6163202c
3000d094:	6c75636c 	.word	0x6c75636c
3000d098:	64657461 	.word	0x64657461
3000d09c:	43524320 	.word	0x43524320
3000d0a0:	7830203a 	.word	0x7830203a
3000d0a4:	202c7825 	.word	0x202c7825
3000d0a8:	73617265 	.word	0x73617265
3000d0ac:	544f2065 	.word	0x544f2065
3000d0b0:	000a3241 	.word	0x000a3241
3000d0b4:	69746341 	.word	0x69746341
3000d0b8:	69206576 	.word	0x69206576
3000d0bc:	7865646e 	.word	0x7865646e
3000d0c0:	41544f20 	.word	0x41544f20
3000d0c4:	766e6920 	.word	0x766e6920
3000d0c8:	64696c61 	.word	0x64696c61
3000d0cc:	6572202c 	.word	0x6572202c
3000d0d0:	74697277 	.word	0x74697277
3000d0d4:	63612065 	.word	0x63612065
3000d0d8:	65766974 	.word	0x65766974
3000d0dc:	646e6920 	.word	0x646e6920
3000d0e0:	74207865 	.word	0x74207865
3000d0e4:	6425206f 	.word	0x6425206f
3000d0e8:	0000000a 	.word	0x0000000a
3000d0ec:	206c6c41 	.word	0x206c6c41
3000d0f0:	2041544f 	.word	0x2041544f
3000d0f4:	72646461 	.word	0x72646461
3000d0f8:	65737365 	.word	0x65737365
3000d0fc:	6f6e2073 	.word	0x6f6e2073
3000d100:	61762074 	.word	0x61762074
3000d104:	2c64696c 	.word	0x2c64696c
3000d108:	63657220 	.word	0x63657220
3000d10c:	7265766f 	.word	0x7265766f
3000d110:	61662079 	.word	0x61662079
3000d114:	000a6c69 	.word	0x000a6c69
3000d118:	20325042 	.word	0x20325042
3000d11c:	3261746f 	.word	0x3261746f
3000d120:	64646120 	.word	0x64646120
3000d124:	73736572 	.word	0x73736572
3000d128:	766e6920 	.word	0x766e6920
3000d12c:	64696c61 	.word	0x64696c61
3000d130:	0000000a 	.word	0x0000000a
3000d134:	20325042 	.word	0x20325042
3000d138:	73726576 	.word	0x73726576
3000d13c:	206e6f69 	.word	0x206e6f69
3000d140:	61766e69 	.word	0x61766e69
3000d144:	0a64696c 	.word	0x0a64696c
3000d148:	00000000 	.word	0x00000000
3000d14c:	20325042 	.word	0x20325042
3000d150:	69746361 	.word	0x69746361
3000d154:	69206576 	.word	0x69206576
3000d158:	7865646e 	.word	0x7865646e
3000d15c:	766e6920 	.word	0x766e6920
3000d160:	64696c61 	.word	0x64696c61
3000d164:	0000000a 	.word	0x0000000a
3000d168:	20325042 	.word	0x20325042
3000d16c:	3161746f 	.word	0x3161746f
3000d170:	64646120 	.word	0x64646120
3000d174:	73736572 	.word	0x73736572
3000d178:	766e6920 	.word	0x766e6920
3000d17c:	64696c61 	.word	0x64696c61
3000d180:	0000000a 	.word	0x0000000a
3000d184:	20325042 	.word	0x20325042
3000d188:	61746164 	.word	0x61746164
3000d18c:	6c617620 	.word	0x6c617620
3000d190:	202c6469 	.word	0x202c6469
3000d194:	73726576 	.word	0x73726576
3000d198:	3a6e6f69 	.word	0x3a6e6f69
3000d19c:	000a6425 	.word	0x000a6425
3000d1a0:	2d73255b 	.word	0x2d73255b
3000d1a4:	44205d49 	.word	0x44205d49
3000d1a8:	62617369 	.word	0x62617369
3000d1ac:	4b20656c 	.word	0x4b20656c
3000d1b0:	4220304d 	.word	0x4220304d
3000d1b4:	20746f6f 	.word	0x20746f6f
3000d1b8:	206e6920 	.word	0x206e6920
3000d1bc:	74736554 	.word	0x74736554
3000d1c0:	646f6d20 	.word	0x646f6d20
3000d1c4:	000a0d65 	.word	0x000a0d65
3000d1c8:	2d73255b 	.word	0x2d73255b
3000d1cc:	42205d49 	.word	0x42205d49
3000d1d0:	73617079 	.word	0x73617079
3000d1d4:	544f2073 	.word	0x544f2073
3000d1d8:	75612050 	.word	0x75612050
3000d1dc:	6f6c6f74 	.word	0x6f6c6f74
3000d1e0:	0a0d6461 	.word	0x0a0d6461
3000d1e4:	00000000 	.word	0x00000000
3000d1e8:	2d73255b 	.word	0x2d73255b
3000d1ec:	4e205d49 	.word	0x4e205d49
3000d1f0:	72462050 	.word	0x72462050
3000d1f4:	25207165 	.word	0x25207165
3000d1f8:	484d2064 	.word	0x484d2064
3000d1fc:	00000a7a 	.word	0x00000a7a
3000d200:	2d73255b 	.word	0x2d73255b
3000d204:	41205d49 	.word	0x41205d49
3000d208:	72462050 	.word	0x72462050
3000d20c:	25207165 	.word	0x25207165
3000d210:	484d2064 	.word	0x484d2064
3000d214:	00000a7a 	.word	0x00000a7a
3000d218:	7361700a 	.word	0x7361700a
3000d21c:	726f7773 	.word	0x726f7773
3000d220:	65762064 	.word	0x65762064
3000d224:	79666972 	.word	0x79666972
3000d228:	69616620 	.word	0x69616620
3000d22c:	00000a6c 	.word	0x00000a6c
3000d230:	7361700a 	.word	0x7361700a
3000d234:	726f7773 	.word	0x726f7773
3000d238:	65762064 	.word	0x65762064
3000d23c:	79666972 	.word	0x79666972
3000d240:	63757320 	.word	0x63757320
3000d244:	73736563 	.word	0x73736563
3000d248:	0000000a 	.word	0x0000000a
3000d24c:	31474d49 	.word	0x31474d49
3000d250:	4f425320 	.word	0x4f425320
3000d254:	4f20544f 	.word	0x4f20544f
3000d258:	000a4646 	.word	0x000a4646
3000d25c:	31474d49 	.word	0x31474d49
3000d260:	4f425320 	.word	0x4f425320
3000d264:	5320544f 	.word	0x5320544f
3000d268:	45434355 	.word	0x45434355
3000d26c:	000a5353 	.word	0x000a5353
3000d270:	746f6f62 	.word	0x746f6f62
3000d274:	64616f6c 	.word	0x64616f6c
3000d278:	765f7265 	.word	0x765f7265
3000d27c:	69737265 	.word	0x69737265
3000d280:	25206e6f 	.word	0x25206e6f
3000d284:	00000a73 	.word	0x00000a73
3000d288:	444f4d5b 	.word	0x444f4d5b
3000d28c:	5f454c55 	.word	0x5f454c55
3000d290:	544f4f42 	.word	0x544f4f42
3000d294:	56454c2d 	.word	0x56454c2d
3000d298:	495f4c45 	.word	0x495f4c45
3000d29c:	5d4f464e 	.word	0x5d4f464e
3000d2a0:	474d493a 	.word	0x474d493a
3000d2a4:	4e452031 	.word	0x4e452031
3000d2a8:	20524554 	.word	0x20524554
3000d2ac:	3a50534d 	.word	0x3a50534d
3000d2b0:	3830255b 	.word	0x3830255b
3000d2b4:	000a5d78 	.word	0x000a5d78
3000d2b8:	444f4d5b 	.word	0x444f4d5b
3000d2bc:	5f454c55 	.word	0x5f454c55
3000d2c0:	544f4f42 	.word	0x544f4f42
3000d2c4:	56454c2d 	.word	0x56454c2d
3000d2c8:	495f4c45 	.word	0x495f4c45
3000d2cc:	5d4f464e 	.word	0x5d4f464e
3000d2d0:	474d493a 	.word	0x474d493a
3000d2d4:	45532031 	.word	0x45532031
3000d2d8:	45525543 	.word	0x45525543
3000d2dc:	41545320 	.word	0x41545320
3000d2e0:	203a4554 	.word	0x203a4554
3000d2e4:	000a6425 	.word	0x000a6425
3000d2e8:	2d73255b 	.word	0x2d73255b
3000d2ec:	49205d49 	.word	0x49205d49
3000d2f0:	2074696e 	.word	0x2074696e
3000d2f4:	41525350 	.word	0x41525350
3000d2f8:	000a0d4d 	.word	0x000a0d4d
3000d2fc:	2d73255b 	.word	0x2d73255b
3000d300:	73205d57 	.word	0x73205d57
3000d304:	70207465 	.word	0x70207465
3000d308:	66206d66 	.word	0x66206d66
3000d30c:	0d6c6961 	.word	0x0d6c6961
3000d310:	0000000a 	.word	0x0000000a
3000d314:	2d73255b 	.word	0x2d73255b
3000d318:	45205d45 	.word	0x45205d45
3000d31c:	524f5252 	.word	0x524f5252
3000d320:	53202121 	.word	0x53202121
3000d324:	6c756f68 	.word	0x6c756f68
3000d328:	6f4e2064 	.word	0x6f4e2064
3000d32c:	6e652074 	.word	0x6e652074
3000d330:	656c6261 	.word	0x656c6261
3000d334:	6d654d20 	.word	0x6d654d20
3000d338:	5f727753 	.word	0x5f727753
3000d33c:	796c6e4f 	.word	0x796c6e4f
3000d340:	206e6920 	.word	0x206e6920
3000d344:	20524444 	.word	0x20524444
3000d348:	70696843 	.word	0x70696843
3000d34c:	0d212121 	.word	0x0d212121
3000d350:	0000000a 	.word	0x0000000a
3000d354:	444f4d5b 	.word	0x444f4d5b
3000d358:	5f454c55 	.word	0x5f454c55
3000d35c:	544f4f42 	.word	0x544f4f42
3000d360:	56454c2d 	.word	0x56454c2d
3000d364:	495f4c45 	.word	0x495f4c45
3000d368:	5d4f464e 	.word	0x5d4f464e
3000d36c:	696e493a 	.word	0x696e493a
3000d370:	44442074 	.word	0x44442074
3000d374:	000a0d52 	.word	0x000a0d52
3000d378:	20445753 	.word	0x20445753
3000d37c:	64207369 	.word	0x64207369
3000d380:	62617369 	.word	0x62617369
3000d384:	2c64656c 	.word	0x2c64656c
3000d388:	6f727020 	.word	0x6f727020
3000d38c:	64656563 	.word	0x64656563
3000d390:	206f7420 	.word	0x206f7420
3000d394:	61736964 	.word	0x61736964
3000d398:	20656c62 	.word	0x20656c62
3000d39c:	74726155 	.word	0x74726155
3000d3a0:	776f4420 	.word	0x776f4420
3000d3a4:	616f6c6e 	.word	0x616f6c6e
3000d3a8:	2e2e2e64 	.word	0x2e2e2e64
3000d3ac:	0000000a 	.word	0x0000000a
3000d3b0:	74726155 	.word	0x74726155
3000d3b4:	776f4420 	.word	0x776f4420
3000d3b8:	616f6c6e 	.word	0x616f6c6e
3000d3bc:	61682064 	.word	0x61682064
3000d3c0:	65622073 	.word	0x65622073
3000d3c4:	64206e65 	.word	0x64206e65
3000d3c8:	62617369 	.word	0x62617369
3000d3cc:	0a64656c 	.word	0x0a64656c
3000d3d0:	00000000 	.word	0x00000000
3000d3d4:	55676f4c 	.word	0x55676f4c
3000d3d8:	20747261 	.word	0x20747261
3000d3dc:	64756142 	.word	0x64756142
3000d3e0:	65746172 	.word	0x65746172
3000d3e4:	6425203a 	.word	0x6425203a
3000d3e8:	0000000a 	.word	0x0000000a
3000d3ec:	4848530a 	.word	0x4848530a
3000d3f0:	4946544f 	.word	0x4946544f
3000d3f4:	6f6e2058 	.word	0x6f6e2058
3000d3f8:	65722074 	.word	0x65722074
3000d3fc:	76696563 	.word	0x76696563
3000d400:	000a6465 	.word	0x000a6465
3000d404:	726f4e0a 	.word	0x726f4e0a
3000d408:	206c616d 	.word	0x206c616d
3000d40c:	746f6f62 	.word	0x746f6f62
3000d410:	0000000a 	.word	0x0000000a
3000d414:	5f6e6962 	.word	0x5f6e6962
3000d418:	313e6f6e 	.word	0x313e6f6e
3000d41c:	3d202c30 	.word	0x3d202c30
3000d420:	0a642520 	.word	0x0a642520
3000d424:	00000000 	.word	0x00000000
3000d428:	6e206164 	.word	0x6e206164
3000d42c:	7220746f 	.word	0x7220746f
3000d430:	69656365 	.word	0x69656365
3000d434:	0a646576 	.word	0x0a646576
3000d438:	00000000 	.word	0x00000000
3000d43c:	746f680a 	.word	0x746f680a
3000d440:	20786966 	.word	0x20786966
3000d444:	656e6f64 	.word	0x656e6f64
3000d448:	202e2e2e 	.word	0x202e2e2e
3000d44c:	6f626552 	.word	0x6f626552
3000d450:	6e69746f 	.word	0x6e69746f
3000d454:	0a2e2e67 	.word	0x0a2e2e67
3000d458:	00000000 	.word	0x00000000
3000d45c:	6c630a0d 	.word	0x6c630a0d
3000d460:	2065736f 	.word	0x2065736f
3000d464:	646f4d79 	.word	0x646f4d79
3000d468:	54206d65 	.word	0x54206d65
3000d46c:	736e6172 	.word	0x736e6172
3000d470:	20726566 	.word	0x20726566
3000d474:	0d2e2e2e 	.word	0x0d2e2e2e
3000d478:	0000000a 	.word	0x0000000a
3000d47c:	50494843 	.word	0x50494843
3000d480:	4f464e49 	.word	0x4f464e49
3000d484:	00000000 	.word	0x00000000
3000d488:	2d73255b 	.word	0x2d73255b
3000d48c:	49205d45 	.word	0x49205d45
3000d490:	6c61766e 	.word	0x6c61766e
3000d494:	43206469 	.word	0x43206469
3000d498:	696e6968 	.word	0x696e6968
3000d49c:	216f666e 	.word	0x216f666e
3000d4a0:	65684320 	.word	0x65684320
3000d4a4:	4f206b63 	.word	0x4f206b63
3000d4a8:	000a5054 	.word	0x000a5054
3000d4ac:	2d73255b 	.word	0x2d73255b
3000d4b0:	50205d57 	.word	0x50205d57
3000d4b4:	7361656c 	.word	0x7361656c
3000d4b8:	72702065 	.word	0x72702065
3000d4bc:	6172676f 	.word	0x6172676f
3000d4c0:	6863206d 	.word	0x6863206d
3000d4c4:	6e697069 	.word	0x6e697069
3000d4c8:	69206f66 	.word	0x69206f66
3000d4cc:	544f206e 	.word	0x544f206e
3000d4d0:	0a212050 	.word	0x0a212050
3000d4d4:	00000000 	.word	0x00000000

3000d4d8 <Memory_Info>:
3000d4d8:	03e00007 00060004 010202c0 02010100     ................
3000d4e8:	02000102 00040302 02040400 01010102     ................
3000d4f8:	01020221 02240401 03000102 00030103     !.....$.........
3000d508:	01050500 06000002 01020206 01070700     ................
3000d518:	08000002 01020208 02280801 09000102     ..........(.....
3000d528:	00040309 03290901 0a000004 0003010a     ......).........
3000d538:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d548:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d558:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d568:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d578:	62696c61 6f696172 000a0d6e              alibraion...

3000d584 <ddrphy_tx_scan>:
3000d584:	00000000 00080000 00080808 00000000     ................
3000d594:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d5a4:	07060012 000f0707 080f0f0f 00000008     ................
3000d5b4:	20000001 22222222 000c0012 34000000     ... """".......4
3000d5c4:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d5d4:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d5e4:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d5f4:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d604:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d614:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d624:	0e0e001a 00170e0e 12171717 00000012     ................
3000d634:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d644:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d654:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d664:	0f0f001b 00180f0f 13181818 00000013     ................
3000d674:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d684:	1110001b 00191111 13191919 00000013     ................
3000d694:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d6a4:	1313001c 001c1313 121c1c1c 00000012     ................
3000d6b4:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d6c4 <ddrphy_zq_rx_scan>:
	...
3000d6cc:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d6dc:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d6ec:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d6fc:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d70c:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d71c:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d72c:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d73c:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d74c:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d75c:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d76c:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d77c:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d78c <Flash_AVL>:
3000d78c:	000000ef 000000ff 00000000 000043fc     .............C..
3000d79c:	00000000 000000a1 000000ff 00000000     ................
3000d7ac:	0000fffc 00000000 0000000b 000000ff     ................
3000d7bc:	00000000 000043fc 00000000 0000000e     .....C..........
3000d7cc:	000000ff 00000000 000043fc 00000000     .........C......
3000d7dc:	000000c8 000000ff 00000001 000043fc     .............C..
3000d7ec:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d7fc:	000200fc 00000000 000000c2 000000ff     ................
3000d80c:	00000002 000000fc 00000000 00000068     ............h...
3000d81c:	000000ff 00000002 000000fc 00000000     ................
3000d82c:	00000051 000000ff 00000002 000000fc     Q...............
3000d83c:	00000000 0000001c 000000ff 00000003     ................
3000d84c:	000000fc 00000000 00000020 000000ff     ........ .......
3000d85c:	00000004 000000fc 00000000 00000000     ................
3000d86c:	000000ff 000000fe ffffffff 30008631     ............1..0
3000d87c:	000000ff ffffffff 000000ff ffffffff     ................
3000d88c:	00000000                                ....

3000d890 <Flash_ReadMode>:
3000d890:	                                         ..

3000d892 <Flash_Speed>:
3000d892:	                                         ..

3000d894 <NAND_AVL>:
3000d894:	000000ef 000000ff 00000000 ffffffff     ................
3000d8a4:	00000000 000000c8 000000ff 00000001     ................
3000d8b4:	ffffffff 00000000 000000e5 000000ff     ................
3000d8c4:	00000001 ffffffff 00000000 000000c2     ................
3000d8d4:	000000ff 00000002 ffffffff 00000000     ................
3000d8e4:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d8fc:	000000ff 000000fe ffffffff 30008691     ...............0
3000d90c:	000000ff ffffffff 000000ff ffffffff     ................
3000d91c:	00000000 3000c7b8 3000c7bc 3000c7c0     .......0...0...0
3000d92c:	3000c7c4 3000c7c8                       ...0...0

3000d934 <__FUNCTION__.0>:
3000d934:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d944 <__FUNCTION__.1>:
3000d944:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d954:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000d964:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000d974:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000d984:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000d994:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000d9a4:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000d9b4:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000d9c4:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d9d4:	00000a20 2d73255b 50205d45 5220524d      ...[%s-E] PMR R
3000d9e4:	20646165 6f727265 000a2172 2d73255b     ead error!..[%s-
3000d9f4:	4f205d57 575f5054 65746972 61462038     W] OTP_Write8 Fa
3000da04:	25206c69 000a2078 2d73255b 4f205d45     il %x ..[%s-E] O
3000da14:	70205054 72676f72 65206d61 726f7272     TP program error
3000da24:	00000a21 2d73255b 50205d45 2032524d     !...[%s-E] PMR2 
3000da34:	64616552 72726520 0a21726f 00000000     Read error!.....

3000da44 <__FUNCTION__.1>:
3000da44:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000da54:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000da64:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000da74:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000da84:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000da94:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000daa4:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000dab4:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000dac4:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000dad4:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000dae4:	726f6620 41485320 00000a32 2d73255b      for SHA2...[%s-
3000daf4:	69205d57 255b7172 70205d64 726f6972     W] irq[%d] prior
3000db04:	20797469 73206425 6c6c6168 203d3c20     ity %d shall <= 
3000db14:	000a6425                                %d..

3000db18 <__FUNCTION__.0>:
3000db18:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000db28:	00000000                                ....

3000db2c <__FUNCTION__.1>:
3000db2c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000db3c <__FUNCTION__.3>:
3000db3c:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000db4c:	32616873 696e695f 00000074              sha2_init...

3000db58 <__FUNCTION__.4>:
3000db58:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000db68:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000db78:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000db88:	6e207369 6320746f 69666e6f 65727567     is not configure
3000db98:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000dba8:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000dbb8:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000dbc8:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000dbd8:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000dbe8:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000dbf8:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000dc08:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000dc18:	69625f6b 21217374 00000a21              k_bits!!!...

3000dc24 <__FUNCTION__.1>:
3000dc24:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000dc34:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000dc44:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000dc54:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000dc64:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000dc74:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000dc84:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000dc94:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000dca4:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000dcb4:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000dcc4:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000dcd4:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000dce4:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000dcf4:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000dd04:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000dd14:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000dd24:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000dd34:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000dd44:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000dd54:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000dd64:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000dd74:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000dd84:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000dd94:	00000000                                ....

3000dd98 <register_string>:
3000dd98:	00000000 3000dcc0 3000c8d8 3000c8dc     .......0...0...0
3000dda8:	3000c8e0 3000c8e4 3000c8e8 3000c8ec     ...0...0...0...0
3000ddb8:	3000c8f0 3000dcbc 3000c8f4 3000c8f8     ...0...0...0...0
3000ddc8:	3000c8fc 3000dd60 65637845 6f697470     ...0`..0Exceptio
3000ddd8:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000dde8:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000ddf8:	64657375 20796220 6c696166 76206465     used by failed v
3000de08:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000de18:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000de28:	00000000 75636553 79746972 75616620     ....Security fau
3000de38:	6920746c 61632073 64657375 20796220     lt is caused by 
3000de48:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000de58:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000de68:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000de78:	79746972 75616620 6920746c 61632073     rity fault is ca
3000de88:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000de98:	65746e69 74697267 69732079 74616e67     integrity signat
3000dea8:	00657275 75636553 79746972 75616620     ure.Security fau
3000deb8:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dec8:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000ded8:	6572206e 6e727574 00000000 75636553     n return....Secu
3000dee8:	79746972 75616620 6920746c 61632073     rity fault is ca
3000def8:	64657375 20796220 72747441 74756269     used by Attribut
3000df08:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000df18:	00006e6f 75636553 79746972 75616620     on..Security fau
3000df28:	6920746c 61632073 64657375 20796220     lt is caused by 
3000df38:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000df48:	00006e6f 75636553 79746972 75616620     on..Security fau
3000df58:	6920746c 61632073 64657375 20796220     lt is caused by 
3000df68:	797a614c 61747320 70206574 65736572     Lazy state prese
3000df78:	74617672 206e6f69 6f727265 00000072     rvation error...
3000df88:	75636553 79746972 75616620 6920746c     Security fault i
3000df98:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000dfa8:	61747320 65206574 726f7272 00000000      state error....
3000dfb8:	20656854 75636573 79746972 75616620     The security fau
3000dfc8:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000dfd8:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000dfe8:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000dff8:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000e008:	61662074 20746c75 63207369 65737561     t fault is cause
3000e018:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000e028:	63636120 20737365 6c6f6976 6f697461      access violatio
3000e038:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000e048:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000e058:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000e068:	20746c75 63207369 65737561 79622064     ult is caused by
3000e078:	74616420 63612061 73736563 6f697620      data access vio
3000e088:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000e098:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000e0a8:	63207369 65737561 79622064 736e7520     is caused by uns
3000e0b8:	6b636174 20676e69 6f727265 00000072     tacking error...
3000e0c8:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000e0d8:	61662074 20746c75 63207369 65737561     t fault is cause
3000e0e8:	79622064 61747320 6e696b63 72652067     d by stacking er
3000e0f8:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000e108:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000e118:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000e128:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000e138:	70206574 65736572 74617672 006e6f69     te preservation.
3000e148:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000e158:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000e168:	64657272 64646120 73736572 00736920     rred address is.
3000e178:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000e188:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000e198:	73692074 75616320 20646573 69207962     t is caused by i
3000e1a8:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000e1b8:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000e1c8:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000e1d8:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000e1e8:	75616320 20646573 70207962 69636572      caused by preci
3000e1f8:	64206573 20617461 65636361 76207373     se data access v
3000e208:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000e218:	6c756166 73692074 75616320 20646573     fault is caused 
3000e228:	69207962 6572706d 65736963 74616420     by imprecise dat
3000e238:	63612061 73736563 6f697620 6974616c     a access violati
3000e248:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000e258:	75616320 20646573 75207962 6174736e      caused by unsta
3000e268:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000e278:	6c756166 73692074 75616320 20646573     fault is caused 
3000e288:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000e298:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000e2a8:	75616320 20646573 66207962 74616f6c      caused by float
3000e2b8:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000e2c8:	65746174 65727020 76726573 6f697461     tate preservatio
3000e2d8:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000e2e8:	636f2074 72727563 61206465 65726464     t occurred addre
3000e2f8:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000e308:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e318:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e328:	65737561 79622064 74746120 74706d65     aused by attempt
3000e338:	6f742073 65786520 65747563 206e6120     s to execute an 
3000e348:	65646e75 656e6966 6e692064 75727473     undefined instru
3000e358:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000e368:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e378:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e388:	65737561 79622064 74746120 74706d65     aused by attempt
3000e398:	6f742073 69777320 20686374 61206f74     s to switch to a
3000e3a8:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000e3b8:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e3c8:	61662065 20746c75 63207369 65737561     e fault is cause
3000e3d8:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e3e8:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e3f8:	74697720 20612068 20646162 756c6176      with a bad valu
3000e408:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e418:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e428:	61662065 20746c75 63207369 65737561     e fault is cause
3000e438:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e448:	65786520 65747563 63206120 6f72706f      execute a copro
3000e458:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e468:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e478:	63207369 65737561 79622064 646e6920     is caused by ind
3000e488:	74616369 74207365 20746168 74732061     icates that a st
3000e498:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e4a8:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e4b8:	61742073 206e656b 63616c70 00000065     s taken place...
3000e4c8:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e4d8:	65737561 79622064 646e6920 74616369     aused by indicat
3000e4e8:	74207365 20746168 75206e61 696c616e     es that an unali
3000e4f8:	64656e67 63636120 20737365 6c756166     gned access faul
3000e508:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e518:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e528:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e538:	74616369 61207365 76696420 20656469     icates a divide 
3000e548:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e558:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e568:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e578:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e588:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e598:	65737561 79622064 6c616820 65722074     aused by halt re
3000e5a8:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e5b8:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e5c8:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e5d8:	20746c75 63207369 65737561 79622064     ult is caused by
3000e5e8:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e5f8:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e608:	61662067 20746c75 63207369 65737561     g fault is cause
3000e618:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e628:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e638:	20746c75 63207369 65737561 79622064     ult is caused by
3000e648:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e658:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e668:	20746c75 63207369 65737561 79622064     ult is caused by
3000e678:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e688:	72657373 00646574 3000c900 3000c90c     sserted....0...0
3000e698:	3000c918 3000c924 3000c930 3000c93c     ...0$..00..0<..0
3000e6a8:	3000c948 3000c954 3000c960 3000c96c     H..0T..0`..0l..0
3000e6b8:	3000cb1c 3000cb28 3000cb30 3000cb38     ...0(..00..08..0
3000e6c8:	3000cb20 3000cb40 00000208 0000020c      ..0@..0........
3000e6d8:	00000210 00000008 00000214 00000218     ................
3000e6e8:	0000021c 0000000c                       ........

3000e6f0 <__FUNCTION__.0>:
3000e6f0:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000e700 <km4_bootloader_rev>:
3000e700:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e710:	625f7265 34353063 66323736 30325f33     er_bc054672f3_20
3000e720:	302f3432 32322f31 3a37312d 313a3235     24/01/22-17:52:1
3000e730:	00000036 2d73255b 2a205d57 2a2a2a2a     6...[%s-W] *****
3000e740:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e750:	2a2a2a2a 4157202a 4e494e52 2a2a2047     ***** WARNING **
3000e760:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e770:	2a2a2a2a 00000a2a 2d73255b 2a205d57     *****...[%s-W] *
3000e780:	20202020 20202020 20202020 20202020                     
3000e790:	20202020 20202020 20202020 20202020                     
3000e7a0:	20202020 20202020 20202020 20202020                     
3000e7b0:	20202020 20202020 00000a2a 2d73255b             *...[%s-
3000e7c0:	2a205d57 20202020 20202020 20202020     W] *            
3000e7d0:	72754320 746e6572 69686320 65762070      Current chip ve
3000e7e0:	6f697372 7369206e 75434120 21212174     rsion is ACut!!!
3000e7f0:	20202020 20202020 20202020 00000a2a                 *...
3000e800:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000e810:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000e820:	2074726f 72727563 20746e65 73726576     ort current vers
3000e830:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000e840:	00000a2a 2d73255b 2a205d57 20202020     *...[%s-W] *    
3000e850:	6c502020 65736165 6e6f6320 74636174       Please contact
3000e860:	4b545220 204d5020 20726f66 65726f6d      RTK PM for more
3000e870:	6f727020 74637564 666e6920 2020216f      product info!  
3000e880:	20202020 00000a2a 2d73255b 2a205d57         *...[%s-W] *
3000e890:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e8a0:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e8b0:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e8c0:	2a2a2a2a 2a2a2a2a 00000a2a 2d73255b     *********...[%s-
3000e8d0:	0a205d57 50004300 5849534f               W] ..C.POSIX...

3000e8df <_ctype_>:
3000e8df:	20202000 20202020 28282020 20282828     .         ((((( 
3000e8ef:	20202020 20202020 20202020 20202020                     
3000e8ff:	10108820 10101010 10101010 10101010      ...............
3000e90f:	04040410 04040404 10040404 10101010     ................
3000e91f:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000e92f:	01010101 01010101 01010101 10101010     ................
3000e93f:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000e94f:	02020202 02020202 02020202 10101010     ................
3000e95f:	00000020 00000000 00000000 00000000      ...............
	...
