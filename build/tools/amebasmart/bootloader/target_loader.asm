
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
300032c4:	30009b61 	.word	0x30009b61
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30009969 	.word	0x30009969
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
300033fc:	30009b61 	.word	0x30009b61
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30009969 	.word	0x30009969
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
3000349c:	f008 fcce 	bl	3000be3c <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000c4cc 	.word	0x3000c4cc
300034bc:	3000c4d8 	.word	0x3000c4d8

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
300034f0:	f008 fca4 	bl	3000be3c <rtk_log_write>
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
30003524:	f008 fc8a 	bl	3000be3c <rtk_log_write>
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
30003540:	3000c4cc 	.word	0x3000c4cc
30003544:	3000c51c 	.word	0x3000c51c
30003548:	30009b61 	.word	0x30009b61
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000c4fc 	.word	0x3000c4fc
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
3000365a:	f008 fbef 	bl	3000be3c <rtk_log_write>
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
3000367a:	f008 fbdf 	bl	3000be3c <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000d5c4 	.word	0x3000d5c4
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000c4cc 	.word	0x3000c4cc
300036a8:	3000c554 	.word	0x3000c554
300036ac:	3000c53c 	.word	0x3000c53c
300036b0:	30009b61 	.word	0x30009b61
300036b4:	30009969 	.word	0x30009969
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
3000373e:	f008 fb7d 	bl	3000be3c <rtk_log_write>
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
30003790:	f008 fb54 	bl	3000be3c <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 fe35 	bl	3000c408 <__io_assert_failed_veneer>
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
300037ca:	f008 fb37 	bl	3000be3c <rtk_log_write>
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
30003816:	f008 fb11 	bl	3000be3c <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 fdf2 	bl	3000c408 <__io_assert_failed_veneer>
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
30003958:	3000d534 	.word	0x3000d534
3000395c:	3000d536 	.word	0x3000d536
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000c4cc 	.word	0x3000c4cc
3000396c:	3000c578 	.word	0x3000c578
30003970:	3000d430 	.word	0x3000d430
30003974:	3000d538 	.word	0x3000d538
30003978:	3000c594 	.word	0x3000c594
3000397c:	3000d5e8 	.word	0x3000d5e8
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000c5c0 	.word	0x3000c5c0
30003988:	3000d5d8 	.word	0x3000d5d8
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000fba1 	.word	0x3000fba1
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30009b61 	.word	0x30009b61
300039b8:	3000fba4 	.word	0x3000fba4
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
30003a36:	f008 fce7 	bl	3000c408 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 fcde 	bl	3000c408 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000fba1 	.word	0x3000fba1
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000d5d8 	.word	0x3000d5d8
30003a78:	3000d5e8 	.word	0x3000d5e8
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
30003a9e:	f002 f873 	bl	30005b88 <BOOT_ImgCopy>
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
30003aee:	f008 f9a5 	bl	3000be3c <rtk_log_write>
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
30003b0c:	f002 f83c 	bl	30005b88 <BOOT_ImgCopy>
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
30003bc0:	f008 f93c 	bl	3000be3c <rtk_log_write>
30003bc4:	2000      	movs	r0, #0
30003bc6:	b00f      	add	sp, #60	; 0x3c
30003bc8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003bcc:	35393138 	.word	0x35393138
30003bd0:	f7ffffe0 	.word	0xf7ffffe0
30003bd4:	3000c690 	.word	0x3000c690
30003bd8:	3000c674 	.word	0x3000c674
30003bdc:	3000ed60 	.word	0x3000ed60
30003be0:	e000ed00 	.word	0xe000ed00
30003be4:	3000c67c 	.word	0x3000c67c

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
30003c0c:	f001 ffbc 	bl	30005b88 <BOOT_ImgCopy>
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
30003c66:	f008 f8e9 	bl	3000be3c <rtk_log_write>
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
30003ca8:	f008 f8c8 	bl	3000be3c <rtk_log_write>
30003cac:	2001      	movs	r0, #1
30003cae:	b00f      	add	sp, #60	; 0x3c
30003cb0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003cb4:	4a89      	ldr	r2, [pc, #548]	; (30003edc <BOOT_LoadAPSubImage+0x2f4>)
30003cb6:	2004      	movs	r0, #4
30003cb8:	4619      	mov	r1, r3
30003cba:	f008 f8bf 	bl	3000be3c <rtk_log_write>
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
30003cd4:	f008 f8b2 	bl	3000be3c <rtk_log_write>
30003cd8:	20ff      	movs	r0, #255	; 0xff
30003cda:	e7aa      	b.n	30003c32 <BOOT_LoadAPSubImage+0x4a>
30003cdc:	463a      	mov	r2, r7
30003cde:	4621      	mov	r1, r4
30003ce0:	4640      	mov	r0, r8
30003ce2:	f001 ff51 	bl	30005b88 <BOOT_ImgCopy>
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
30003d3c:	f001 ff24 	bl	30005b88 <BOOT_ImgCopy>
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
30003da2:	f008 f84b 	bl	3000be3c <rtk_log_write>
30003da6:	2000      	movs	r0, #0
30003da8:	b00f      	add	sp, #60	; 0x3c
30003daa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30003dae:	4639      	mov	r1, r7
30003db0:	4642      	mov	r2, r8
30003db2:	4628      	mov	r0, r5
30003db4:	f001 fee8 	bl	30005b88 <BOOT_ImgCopy>
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
30003ec0:	3000c66c 	.word	0x3000c66c
30003ec4:	35393138 	.word	0x35393138
30003ec8:	f7ffffe0 	.word	0xf7ffffe0
30003ecc:	0000c109 	.word	0x0000c109
30003ed0:	3000c674 	.word	0x3000c674
30003ed4:	3000c6d4 	.word	0x3000c6d4
30003ed8:	3000c690 	.word	0x3000c690
30003edc:	3000c6a4 	.word	0x3000c6a4
30003ee0:	3000c6b4 	.word	0x3000c6b4
30003ee4:	e000ed00 	.word	0xe000ed00
30003ee8:	3000c67c 	.word	0x3000c67c

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
30003f20:	30009851 	.word	0x30009851

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
30003f5a:	f007 ff6f 	bl	3000be3c <rtk_log_write>
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
30003f78:	f007 bf60 	b.w	3000be3c <rtk_log_write>
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
30003fa0:	3000c674 	.word	0x3000c674
30003fa4:	3000c6e4 	.word	0x3000c6e4
30003fa8:	0000c245 	.word	0x0000c245
30003fac:	3000c6f8 	.word	0x3000c6f8
30003fb0:	30009885 	.word	0x30009885
30003fb4:	3000986d 	.word	0x3000986d

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
30003fe8:	f001 fdce 	bl	30005b88 <BOOT_ImgCopy>
30003fec:	4641      	mov	r1, r8
30003fee:	4864      	ldr	r0, [pc, #400]	; (30004180 <BOOT_OTA_LoadIMG2+0x1c8>)
30003ff0:	f000 fd2a 	bl	30004a48 <BOOT_CertificateCheck>
30003ff4:	2800      	cmp	r0, #0
30003ff6:	f040 80b7 	bne.w	30004168 <BOOT_OTA_LoadIMG2+0x1b0>
30003ffa:	4604      	mov	r4, r0
30003ffc:	f44f 72c8 	mov.w	r2, #400	; 0x190
30004000:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30004004:	485f      	ldr	r0, [pc, #380]	; (30004184 <BOOT_OTA_LoadIMG2+0x1cc>)
30004006:	f001 fdbf 	bl	30005b88 <BOOT_ImgCopy>
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
3000413e:	f008 f923 	bl	3000c388 <__DiagPrintf_veneer>
30004142:	b11d      	cbz	r5, 3000414c <BOOT_OTA_LoadIMG2+0x194>
30004144:	481c      	ldr	r0, [pc, #112]	; (300041b8 <BOOT_OTA_LoadIMG2+0x200>)
30004146:	f008 f91f 	bl	3000c388 <__DiagPrintf_veneer>
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
3000416c:	f008 f90c 	bl	3000c388 <__DiagPrintf_veneer>
30004170:	4620      	mov	r0, r4
30004172:	b00d      	add	sp, #52	; 0x34
30004174:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004178:	3000e2c8 	.word	0x3000e2c8
3000417c:	3000ffa4 	.word	0x3000ffa4
30004180:	3000fc48 	.word	0x3000fc48
30004184:	3000fdac 	.word	0x3000fdac
30004188:	0000c129 	.word	0x0000c129
3000418c:	0cffffe0 	.word	0x0cffffe0
30004190:	30009901 	.word	0x30009901
30004194:	30009941 	.word	0x30009941
30004198:	3000ff54 	.word	0x3000ff54
3000419c:	fffff000 	.word	0xfffff000
300041a0:	0dffffe0 	.word	0x0dffffe0
300041a4:	3000ed60 	.word	0x3000ed60
300041a8:	2001c00c 	.word	0x2001c00c
300041ac:	3000c718 	.word	0x3000c718
300041b0:	3000c710 	.word	0x3000c710
300041b4:	3000c748 	.word	0x3000c748
300041b8:	3000c77c 	.word	0x3000c77c
300041bc:	3000fdcc 	.word	0x3000fdcc
300041c0:	00012a1d 	.word	0x00012a1d
300041c4:	30009851 	.word	0x30009851
300041c8:	3000c720 	.word	0x3000c720
300041cc:	0c000000 	.word	0x0c000000
300041d0:	30009b61 	.word	0x30009b61
300041d4:	0d000000 	.word	0x0d000000

300041d8 <BOOT_OTA_Region_Init>:
300041d8:	b510      	push	{r4, lr}
300041da:	4c10      	ldr	r4, [pc, #64]	; (3000421c <BOOT_OTA_Region_Init+0x44>)
300041dc:	2200      	movs	r2, #0
300041de:	2002      	movs	r0, #2
300041e0:	4621      	mov	r1, r4
300041e2:	f004 fa49 	bl	30008678 <flash_get_layout_info>
300041e6:	2200      	movs	r2, #0
300041e8:	1d21      	adds	r1, r4, #4
300041ea:	2003      	movs	r0, #3
300041ec:	f004 fa44 	bl	30008678 <flash_get_layout_info>
300041f0:	2200      	movs	r2, #0
300041f2:	f104 0110 	add.w	r1, r4, #16
300041f6:	2004      	movs	r0, #4
300041f8:	f004 fa3e 	bl	30008678 <flash_get_layout_info>
300041fc:	2200      	movs	r2, #0
300041fe:	f104 0114 	add.w	r1, r4, #20
30004202:	2005      	movs	r0, #5
30004204:	f004 fa38 	bl	30008678 <flash_get_layout_info>
30004208:	e9d4 2300 	ldrd	r2, r3, [r4]
3000420c:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004210:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30004214:	e9c4 2302 	strd	r2, r3, [r4, #8]
30004218:	bd10      	pop	{r4, pc}
3000421a:	bf00      	nop
3000421c:	3000ff3c 	.word	0x3000ff3c

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
30004246:	f001 fc9f 	bl	30005b88 <BOOT_ImgCopy>
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
300042d2:	f008 f859 	bl	3000c388 <__DiagPrintf_veneer>
300042d6:	4630      	mov	r0, r6
300042d8:	b06b      	add	sp, #428	; 0x1ac
300042da:	bdf0      	pop	{r4, r5, r6, r7, pc}
300042dc:	480f      	ldr	r0, [pc, #60]	; (3000431c <BOOT_OTA_AP+0xf8>)
300042de:	26ff      	movs	r6, #255	; 0xff
300042e0:	f008 f852 	bl	3000c388 <__DiagPrintf_veneer>
300042e4:	e7e8      	b.n	300042b8 <BOOT_OTA_AP+0x94>
300042e6:	480e      	ldr	r0, [pc, #56]	; (30004320 <BOOT_OTA_AP+0xfc>)
300042e8:	26ff      	movs	r6, #255	; 0xff
300042ea:	f008 f84d 	bl	3000c388 <__DiagPrintf_veneer>
300042ee:	e7e3      	b.n	300042b8 <BOOT_OTA_AP+0x94>
300042f0:	3000ed60 	.word	0x3000ed60
300042f4:	3000e2e0 	.word	0x3000e2e0
300042f8:	30009901 	.word	0x30009901
300042fc:	0e000000 	.word	0x0e000000
30004300:	0fffffe0 	.word	0x0fffffe0
30004304:	30009941 	.word	0x30009941
30004308:	30009b61 	.word	0x30009b61
3000430c:	3000ff54 	.word	0x3000ff54
30004310:	3000fc48 	.word	0x3000fc48
30004314:	2001c00c 	.word	0x2001c00c
30004318:	3000c7c4 	.word	0x3000c7c4
3000431c:	3000c7e8 	.word	0x3000c7e8
30004320:	3000c7a0 	.word	0x3000c7a0

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
3000434a:	f001 fc1d 	bl	30005b88 <BOOT_ImgCopy>
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
30004396:	f001 fbf7 	bl	30005b88 <BOOT_ImgCopy>
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
300043d4:	f007 fd32 	bl	3000be3c <rtk_log_write>
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
30004402:	f001 fbc1 	bl	30005b88 <BOOT_ImgCopy>
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
3000445a:	f007 fcef 	bl	3000be3c <rtk_log_write>
3000445e:	2000      	movs	r0, #0
30004460:	e7e8      	b.n	30004434 <BOOT_LoadRDPImg+0x110>
30004462:	bf00      	nop
30004464:	3000e2f0 	.word	0x3000e2f0
30004468:	00002429 	.word	0x00002429
3000446c:	00002479 	.word	0x00002479
30004470:	00004641 	.word	0x00004641
30004474:	35393138 	.word	0x35393138
30004478:	31313738 	.word	0x31313738
3000447c:	3000c674 	.word	0x3000c674
30004480:	3000c690 	.word	0x3000c690
30004484:	3000c67c 	.word	0x3000c67c
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
300044f8:	f001 fb46 	bl	30005b88 <BOOT_ImgCopy>
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
3000453c:	f007 ff24 	bl	3000c388 <__DiagPrintf_veneer>
30004540:	2000      	movs	r0, #0
30004542:	b051      	add	sp, #324	; 0x144
30004544:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004548:	4621      	mov	r1, r4
3000454a:	462b      	mov	r3, r5
3000454c:	4632      	mov	r2, r6
3000454e:	480c      	ldr	r0, [pc, #48]	; (30004580 <BOOT_LoadBL32RDPImg+0xf0>)
30004550:	f007 ff1a 	bl	3000c388 <__DiagPrintf_veneer>
30004554:	e7c1      	b.n	300044da <BOOT_LoadBL32RDPImg+0x4a>
30004556:	480b      	ldr	r0, [pc, #44]	; (30004584 <BOOT_LoadBL32RDPImg+0xf4>)
30004558:	f007 ff16 	bl	3000c388 <__DiagPrintf_veneer>
3000455c:	2100      	movs	r1, #0
3000455e:	4608      	mov	r0, r1
30004560:	47d0      	blx	sl
30004562:	2001      	movs	r0, #1
30004564:	b051      	add	sp, #324	; 0x144
30004566:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000456a:	bf00      	nop
3000456c:	3000c890 	.word	0x3000c890
30004570:	00004641 	.word	0x00004641
30004574:	2001c00c 	.word	0x2001c00c
30004578:	35393138 	.word	0x35393138
3000457c:	3000c874 	.word	0x3000c874
30004580:	3000c84c 	.word	0x3000c84c
30004584:	3000c880 	.word	0x3000c880
30004588:	3000c8a4 	.word	0x3000c8a4
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
300045b6:	f007 febf 	bl	3000c338 <____wrap_memset_veneer>
300045ba:	4628      	mov	r0, r5
300045bc:	f10d 021f 	add.w	r2, sp, #31
300045c0:	f10d 011e 	add.w	r1, sp, #30
300045c4:	f005 fb02 	bl	30009bcc <SBOOT_GetMdType>
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
30004736:	f001 fa27 	bl	30005b88 <BOOT_ImgCopy>
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
30004798:	f001 f9f6 	bl	30005b88 <BOOT_ImgCopy>
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
300047dc:	30009c09 	.word	0x30009c09
300047e0:	300094c9 	.word	0x300094c9
300047e4:	e000ed00 	.word	0xe000ed00
300047e8:	30009c85 	.word	0x30009c85
300047ec:	0000ea4d 	.word	0x0000ea4d
300047f0:	30009cc5 	.word	0x30009cc5
300047f4:	30009d41 	.word	0x30009d41
300047f8:	30009c49 	.word	0x30009c49
300047fc:	30009d05 	.word	0x30009d05

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
30004820:	f005 baf2 	b.w	30009e08 <SBOOT_Validate_ImgHash>
30004824:	4620      	mov	r0, r4
30004826:	b005      	add	sp, #20
30004828:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
3000482c:	f7ff beb6 	b.w	3000459c <_BOOT_Validate_ImgHash>
30004830:	30009b61 	.word	0x30009b61

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
30004872:	f005 fb03 	bl	30009e7c <SBOOT_Validate_Algorithm>
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
300048ac:	f005 fa68 	bl	30009d80 <SBOOT_Validate_PubKey>
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
300048d0:	f007 fab4 	bl	3000be3c <rtk_log_write>
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
300049c4:	f005 f9fa 	bl	30009dbc <SBOOT_Validate_Signature>
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
30004a0a:	f007 fa17 	bl	3000be3c <rtk_log_write>
30004a0e:	4620      	mov	r0, r4
30004a10:	e721      	b.n	30004856 <BOOT_SignatureCheck+0x22>
30004a12:	4620      	mov	r0, r4
30004a14:	463b      	mov	r3, r7
30004a16:	464a      	mov	r2, r9
30004a18:	f005 f9f6 	bl	30009e08 <SBOOT_Validate_ImgHash>
30004a1c:	4604      	mov	r4, r0
30004a1e:	e7e5      	b.n	300049ec <BOOT_SignatureCheck+0x1b8>
30004a20:	22b4      	movs	r2, #180	; 0xb4
30004a22:	e73d      	b.n	300048a0 <BOOT_SignatureCheck+0x6c>
30004a24:	3000e2f8 	.word	0x3000e2f8
30004a28:	0000c149 	.word	0x0000c149
30004a2c:	30009e45 	.word	0x30009e45
30004a30:	3000c674 	.word	0x3000c674
30004a34:	3000c8c4 	.word	0x3000c8c4
30004a38:	00012be5 	.word	0x00012be5
30004a3c:	e000ed00 	.word	0xe000ed00
30004a40:	30009b61 	.word	0x30009b61
30004a44:	3000c8ac 	.word	0x3000c8ac

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
30004a64:	f001 f890 	bl	30005b88 <BOOT_ImgCopy>
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
30004aa8:	f005 f9e8 	bl	30009e7c <SBOOT_Validate_Algorithm>
30004aac:	4605      	mov	r5, r0
30004aae:	b9c0      	cbnz	r0, 30004ae2 <BOOT_CertificateCheck+0x9a>
30004ab0:	f104 0620 	add.w	r6, r4, #32
30004ab4:	aa04      	add	r2, sp, #16
30004ab6:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004aba:	4631      	mov	r1, r6
30004abc:	f005 f960 	bl	30009d80 <SBOOT_Validate_PubKey>
30004ac0:	4605      	mov	r5, r0
30004ac2:	b970      	cbnz	r0, 30004ae2 <BOOT_CertificateCheck+0x9a>
30004ac4:	69a0      	ldr	r0, [r4, #24]
30004ac6:	4623      	mov	r3, r4
30004ac8:	ac0c      	add	r4, sp, #48	; 0x30
30004aca:	4632      	mov	r2, r6
30004acc:	f89d 100f 	ldrb.w	r1, [sp, #15]
30004ad0:	e9cd 0400 	strd	r0, r4, [sp]
30004ad4:	f89d 000e 	ldrb.w	r0, [sp, #14]
30004ad8:	f005 f970 	bl	30009dbc <SBOOT_Validate_Signature>
30004adc:	4605      	mov	r5, r0
30004ade:	2800      	cmp	r0, #0
30004ae0:	d0b9      	beq.n	30004a56 <BOOT_CertificateCheck+0xe>
30004ae2:	4b09      	ldr	r3, [pc, #36]	; (30004b08 <BOOT_CertificateCheck+0xc0>)
30004ae4:	2002      	movs	r0, #2
30004ae6:	4a09      	ldr	r2, [pc, #36]	; (30004b0c <BOOT_CertificateCheck+0xc4>)
30004ae8:	4619      	mov	r1, r3
30004aea:	9500      	str	r5, [sp, #0]
30004aec:	f007 f9a6 	bl	3000be3c <rtk_log_write>
30004af0:	20ff      	movs	r0, #255	; 0xff
30004af2:	b030      	add	sp, #192	; 0xc0
30004af4:	bd70      	pop	{r4, r5, r6, pc}
30004af6:	bf00      	nop
30004af8:	0000c149 	.word	0x0000c149
30004afc:	0000ea4d 	.word	0x0000ea4d
30004b00:	3000e628 	.word	0x3000e628
30004b04:	30009e45 	.word	0x30009e45
30004b08:	3000c674 	.word	0x3000c674
30004b0c:	3000c8e8 	.word	0x3000c8e8

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
30004c80:	3000e728 	.word	0x3000e728
30004c84:	3000e6c8 	.word	0x3000e6c8
30004c88:	51001b00 	.word	0x51001b00
30004c8c:	500e0000 	.word	0x500e0000
30004c90:	3000e788 	.word	0x3000e788
30004c94:	e000ed00 	.word	0xe000ed00
30004c98:	42008000 	.word	0x42008000
30004c9c:	05fa6000 	.word	0x05fa6000
30004ca0:	e000e100 	.word	0xe000e100
30004ca4:	3000e623 	.word	0x3000e623
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
30004d28:	f002 ff22 	bl	30007b70 <ymodem_uart_getdata>
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
30004df0:	4b3c      	ldr	r3, [pc, #240]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004df2:	b095      	sub	sp, #84	; 0x54
30004df4:	4616      	mov	r6, r2
30004df6:	4604      	mov	r4, r0
30004df8:	4a3b      	ldr	r2, [pc, #236]	; (30004ee8 <BOOT_FaultHandler+0xfc>)
30004dfa:	2001      	movs	r0, #1
30004dfc:	460d      	mov	r5, r1
30004dfe:	4619      	mov	r1, r3
30004e00:	f007 f81c 	bl	3000be3c <rtk_log_write>
30004e04:	2244      	movs	r2, #68	; 0x44
30004e06:	2100      	movs	r1, #0
30004e08:	a803      	add	r0, sp, #12
30004e0a:	f007 fa95 	bl	3000c338 <____wrap_memset_veneer>
30004e0e:	0672      	lsls	r2, r6, #25
30004e10:	4b34      	ldr	r3, [pc, #208]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004e12:	d555      	bpl.n	30004ec0 <BOOT_FaultHandler+0xd4>
30004e14:	4619      	mov	r1, r3
30004e16:	4a35      	ldr	r2, [pc, #212]	; (30004eec <BOOT_FaultHandler+0x100>)
30004e18:	2001      	movs	r0, #1
30004e1a:	f007 f80f 	bl	3000be3c <rtk_log_write>
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
30004e60:	4c23      	ldr	r4, [pc, #140]	; (30004ef0 <BOOT_FaultHandler+0x104>)
30004e62:	aa03      	add	r2, sp, #12
30004e64:	4629      	mov	r1, r5
30004e66:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30004e68:	f006 fd12 	bl	3000b890 <crash_dump>
30004e6c:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30004e70:	4b1c      	ldr	r3, [pc, #112]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004e72:	2001      	movs	r0, #1
30004e74:	9100      	str	r1, [sp, #0]
30004e76:	4a1f      	ldr	r2, [pc, #124]	; (30004ef4 <BOOT_FaultHandler+0x108>)
30004e78:	4619      	mov	r1, r3
30004e7a:	f006 ffdf 	bl	3000be3c <rtk_log_write>
30004e7e:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30004e82:	4b18      	ldr	r3, [pc, #96]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004e84:	2001      	movs	r0, #1
30004e86:	9200      	str	r2, [sp, #0]
30004e88:	4619      	mov	r1, r3
30004e8a:	4a1b      	ldr	r2, [pc, #108]	; (30004ef8 <BOOT_FaultHandler+0x10c>)
30004e8c:	f006 ffd6 	bl	3000be3c <rtk_log_write>
30004e90:	6a61      	ldr	r1, [r4, #36]	; 0x24
30004e92:	4b14      	ldr	r3, [pc, #80]	; (30004ee4 <BOOT_FaultHandler+0xf8>)
30004e94:	2001      	movs	r0, #1
30004e96:	9100      	str	r1, [sp, #0]
30004e98:	4a18      	ldr	r2, [pc, #96]	; (30004efc <BOOT_FaultHandler+0x110>)
30004e9a:	4619      	mov	r1, r3
30004e9c:	f006 ffce 	bl	3000be3c <rtk_log_write>
30004ea0:	f3ef 8488 	mrs	r4, MSP_NS
30004ea4:	f3ef 8589 	mrs	r5, PSP_NS
30004ea8:	2101      	movs	r1, #1
30004eaa:	2002      	movs	r0, #2
30004eac:	4b14      	ldr	r3, [pc, #80]	; (30004f00 <BOOT_FaultHandler+0x114>)
30004eae:	4798      	blx	r3
30004eb0:	463b      	mov	r3, r7
30004eb2:	4632      	mov	r2, r6
30004eb4:	4629      	mov	r1, r5
30004eb6:	4620      	mov	r0, r4
30004eb8:	f007 fa36 	bl	3000c328 <__INT_HardFault_C_veneer>
30004ebc:	b015      	add	sp, #84	; 0x54
30004ebe:	bdf0      	pop	{r4, r5, r6, r7, pc}
30004ec0:	4a10      	ldr	r2, [pc, #64]	; (30004f04 <BOOT_FaultHandler+0x118>)
30004ec2:	2001      	movs	r0, #1
30004ec4:	4619      	mov	r1, r3
30004ec6:	f006 ffb9 	bl	3000be3c <rtk_log_write>
30004eca:	f3ef 8394 	mrs	r3, CONTROL_NS
30004ece:	f3ef 8e88 	mrs	lr, MSP_NS
30004ed2:	f3ef 8589 	mrs	r5, PSP_NS
30004ed6:	f016 0108 	ands.w	r1, r6, #8
30004eda:	bf18      	it	ne
30004edc:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30004ee0:	e7a5      	b.n	30004e2e <BOOT_FaultHandler+0x42>
30004ee2:	bf00      	nop
30004ee4:	3000c674 	.word	0x3000c674
30004ee8:	3000c914 	.word	0x3000c914
30004eec:	3000c938 	.word	0x3000c938
30004ef0:	e000ed00 	.word	0xe000ed00
30004ef4:	3000c98c 	.word	0x3000c98c
30004ef8:	3000c9a4 	.word	0x3000c9a4
30004efc:	3000c9bc 	.word	0x3000c9bc
30004f00:	0000994d 	.word	0x0000994d
30004f04:	3000c960 	.word	0x3000c960

30004f08 <FLASH_Erase_With_Lock>:
30004f08:	4b11      	ldr	r3, [pc, #68]	; (30004f50 <FLASH_Erase_With_Lock+0x48>)
30004f0a:	b510      	push	{r4, lr}
30004f0c:	460c      	mov	r4, r1
30004f0e:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30004f12:	4798      	blx	r3
30004f14:	4b0f      	ldr	r3, [pc, #60]	; (30004f54 <FLASH_Erase_With_Lock+0x4c>)
30004f16:	695b      	ldr	r3, [r3, #20]
30004f18:	03db      	lsls	r3, r3, #15
30004f1a:	d512      	bpl.n	30004f42 <FLASH_Erase_With_Lock+0x3a>
30004f1c:	f014 021f 	ands.w	r2, r4, #31
30004f20:	d110      	bne.n	30004f44 <FLASH_Erase_With_Lock+0x3c>
30004f22:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30004f26:	f3bf 8f4f 	dsb	sy
30004f2a:	480a      	ldr	r0, [pc, #40]	; (30004f54 <FLASH_Erase_With_Lock+0x4c>)
30004f2c:	4422      	add	r2, r4
30004f2e:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30004f32:	3420      	adds	r4, #32
30004f34:	1b13      	subs	r3, r2, r4
30004f36:	2b00      	cmp	r3, #0
30004f38:	dcf9      	bgt.n	30004f2e <FLASH_Erase_With_Lock+0x26>
30004f3a:	f3bf 8f4f 	dsb	sy
30004f3e:	f3bf 8f6f 	isb	sy
30004f42:	bd10      	pop	{r4, pc}
30004f44:	f024 041f 	bic.w	r4, r4, #31
30004f48:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30004f4c:	e7eb      	b.n	30004f26 <FLASH_Erase_With_Lock+0x1e>
30004f4e:	bf00      	nop
30004f50:	0000a16d 	.word	0x0000a16d
30004f54:	e000ed00 	.word	0xe000ed00

30004f58 <FLASH_TxData_With_Lock>:
30004f58:	b538      	push	{r3, r4, r5, lr}
30004f5a:	4b13      	ldr	r3, [pc, #76]	; (30004fa8 <FLASH_TxData_With_Lock+0x50>)
30004f5c:	4604      	mov	r4, r0
30004f5e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30004f62:	460d      	mov	r5, r1
30004f64:	4798      	blx	r3
30004f66:	4b11      	ldr	r3, [pc, #68]	; (30004fac <FLASH_TxData_With_Lock+0x54>)
30004f68:	695b      	ldr	r3, [r3, #20]
30004f6a:	03db      	lsls	r3, r3, #15
30004f6c:	d517      	bpl.n	30004f9e <FLASH_TxData_With_Lock+0x46>
30004f6e:	f014 031f 	ands.w	r3, r4, #31
30004f72:	d115      	bne.n	30004fa0 <FLASH_TxData_With_Lock+0x48>
30004f74:	f3bf 8f4f 	dsb	sy
30004f78:	b16d      	cbz	r5, 30004f96 <FLASH_TxData_With_Lock+0x3e>
30004f7a:	1e6a      	subs	r2, r5, #1
30004f7c:	f104 0320 	add.w	r3, r4, #32
30004f80:	490a      	ldr	r1, [pc, #40]	; (30004fac <FLASH_TxData_With_Lock+0x54>)
30004f82:	f022 021f 	bic.w	r2, r2, #31
30004f86:	441a      	add	r2, r3
30004f88:	4293      	cmp	r3, r2
30004f8a:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30004f8e:	461c      	mov	r4, r3
30004f90:	f103 0320 	add.w	r3, r3, #32
30004f94:	d1f8      	bne.n	30004f88 <FLASH_TxData_With_Lock+0x30>
30004f96:	f3bf 8f4f 	dsb	sy
30004f9a:	f3bf 8f6f 	isb	sy
30004f9e:	bd38      	pop	{r3, r4, r5, pc}
30004fa0:	f024 041f 	bic.w	r4, r4, #31
30004fa4:	441d      	add	r5, r3
30004fa6:	e7e5      	b.n	30004f74 <FLASH_TxData_With_Lock+0x1c>
30004fa8:	0000a04d 	.word	0x0000a04d
30004fac:	e000ed00 	.word	0xe000ed00

30004fb0 <flash_read_id>:
30004fb0:	b538      	push	{r3, r4, r5, lr}
30004fb2:	4d04      	ldr	r5, [pc, #16]	; (30004fc4 <flash_read_id+0x14>)
30004fb4:	460c      	mov	r4, r1
30004fb6:	4602      	mov	r2, r0
30004fb8:	4b03      	ldr	r3, [pc, #12]	; (30004fc8 <flash_read_id+0x18>)
30004fba:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30004fbe:	4798      	blx	r3
30004fc0:	4620      	mov	r0, r4
30004fc2:	bd38      	pop	{r3, r4, r5, pc}
30004fc4:	2001c01c 	.word	0x2001c01c
30004fc8:	00009db5 	.word	0x00009db5

30004fcc <mcc_ReverseBit>:
30004fcc:	0843      	lsrs	r3, r0, #1
30004fce:	08c1      	lsrs	r1, r0, #3
30004fd0:	f003 0308 	and.w	r3, r3, #8
30004fd4:	0942      	lsrs	r2, r0, #5
30004fd6:	f001 0104 	and.w	r1, r1, #4
30004fda:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30004fde:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30004fe2:	f002 0202 	and.w	r2, r2, #2
30004fe6:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30004fea:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30004fee:	430b      	orrs	r3, r1
30004ff0:	00c1      	lsls	r1, r0, #3
30004ff2:	4313      	orrs	r3, r2
30004ff4:	f001 0120 	and.w	r1, r1, #32
30004ff8:	0042      	lsls	r2, r0, #1
30004ffa:	ea43 000c 	orr.w	r0, r3, ip
30004ffe:	f002 0310 	and.w	r3, r2, #16
30005002:	4308      	orrs	r0, r1
30005004:	4318      	orrs	r0, r3
30005006:	b2c0      	uxtb	r0, r0
30005008:	4770      	bx	lr
3000500a:	bf00      	nop

3000500c <mcc_init_crc32>:
3000500c:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
3000500e:	2600      	movs	r6, #0
30005010:	4f16      	ldr	r7, [pc, #88]	; (3000506c <mcc_init_crc32+0x60>)
30005012:	4605      	mov	r5, r0
30005014:	b2f0      	uxtb	r0, r6
30005016:	f7ff ffd9 	bl	30004fcc <mcc_ReverseBit>
3000501a:	2208      	movs	r2, #8
3000501c:	0604      	lsls	r4, r0, #24
3000501e:	2c00      	cmp	r4, #0
30005020:	ea4f 0444 	mov.w	r4, r4, lsl #1
30005024:	bfb8      	it	lt
30005026:	407c      	eorlt	r4, r7
30005028:	3a01      	subs	r2, #1
3000502a:	d1f8      	bne.n	3000501e <mcc_init_crc32+0x12>
3000502c:	0e20      	lsrs	r0, r4, #24
3000502e:	3504      	adds	r5, #4
30005030:	f7ff ffcc 	bl	30004fcc <mcc_ReverseBit>
30005034:	4603      	mov	r3, r0
30005036:	f3c4 4007 	ubfx	r0, r4, #16, #8
3000503a:	3601      	adds	r6, #1
3000503c:	f805 3c04 	strb.w	r3, [r5, #-4]
30005040:	f7ff ffc4 	bl	30004fcc <mcc_ReverseBit>
30005044:	4603      	mov	r3, r0
30005046:	f3c4 2007 	ubfx	r0, r4, #8, #8
3000504a:	f805 3c03 	strb.w	r3, [r5, #-3]
3000504e:	f7ff ffbd 	bl	30004fcc <mcc_ReverseBit>
30005052:	4603      	mov	r3, r0
30005054:	b2e0      	uxtb	r0, r4
30005056:	f805 3c02 	strb.w	r3, [r5, #-2]
3000505a:	f7ff ffb7 	bl	30004fcc <mcc_ReverseBit>
3000505e:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30005062:	f805 0c01 	strb.w	r0, [r5, #-1]
30005066:	d1d5      	bne.n	30005014 <mcc_init_crc32+0x8>
30005068:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
3000506a:	bf00      	nop
3000506c:	04c11db7 	.word	0x04c11db7

30005070 <mcc_crc32>:
30005070:	b179      	cbz	r1, 30005092 <mcc_crc32+0x22>
30005072:	eb00 0c01 	add.w	ip, r0, r1
30005076:	f04f 31ff 	mov.w	r1, #4294967295
3000507a:	f810 3b01 	ldrb.w	r3, [r0], #1
3000507e:	404b      	eors	r3, r1
30005080:	4560      	cmp	r0, ip
30005082:	b2db      	uxtb	r3, r3
30005084:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30005088:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
3000508c:	d1f5      	bne.n	3000507a <mcc_crc32+0xa>
3000508e:	43c8      	mvns	r0, r1
30005090:	4770      	bx	lr
30005092:	4608      	mov	r0, r1
30005094:	4770      	bx	lr
30005096:	bf00      	nop

30005098 <recovery_check>:
30005098:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
3000509c:	4283      	cmp	r3, r0
3000509e:	d322      	bcc.n	300050e6 <recovery_check+0x4e>
300050a0:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
300050a4:	d91f      	bls.n	300050e6 <recovery_check+0x4e>
300050a6:	b510      	push	{r4, lr}
300050a8:	f8d0 400a 	ldr.w	r4, [r0, #10]
300050ac:	f104 030c 	add.w	r3, r4, #12
300050b0:	4299      	cmp	r1, r3
300050b2:	d316      	bcc.n	300050e2 <recovery_check+0x4a>
300050b4:	f100 0c04 	add.w	ip, r0, #4
300050b8:	b183      	cbz	r3, 300050dc <recovery_check+0x44>
300050ba:	f100 0e10 	add.w	lr, r0, #16
300050be:	f04f 31ff 	mov.w	r1, #4294967295
300050c2:	44a6      	add	lr, r4
300050c4:	4c0b      	ldr	r4, [pc, #44]	; (300050f4 <recovery_check+0x5c>)
300050c6:	f81c 3b01 	ldrb.w	r3, [ip], #1
300050ca:	404b      	eors	r3, r1
300050cc:	45f4      	cmp	ip, lr
300050ce:	b2db      	uxtb	r3, r3
300050d0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300050d4:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
300050d8:	d1f5      	bne.n	300050c6 <recovery_check+0x2e>
300050da:	43cb      	mvns	r3, r1
300050dc:	6801      	ldr	r1, [r0, #0]
300050de:	4299      	cmp	r1, r3
300050e0:	d003      	beq.n	300050ea <recovery_check+0x52>
300050e2:	2000      	movs	r0, #0
300050e4:	bd10      	pop	{r4, pc}
300050e6:	2000      	movs	r0, #0
300050e8:	4770      	bx	lr
300050ea:	f8d0 3006 	ldr.w	r3, [r0, #6]
300050ee:	2001      	movs	r0, #1
300050f0:	6013      	str	r3, [r2, #0]
300050f2:	bd10      	pop	{r4, pc}
300050f4:	3000e7e8 	.word	0x3000e7e8

300050f8 <rewrite_bp>:
300050f8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300050fc:	b0a3      	sub	sp, #140	; 0x8c
300050fe:	4616      	mov	r6, r2
30005100:	4607      	mov	r7, r0
30005102:	460d      	mov	r5, r1
30005104:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
30005108:	4699      	mov	r9, r3
3000510a:	f10d 0808 	add.w	r8, sp, #8
3000510e:	2280      	movs	r2, #128	; 0x80
30005110:	2c00      	cmp	r4, #0
30005112:	f000 80b2 	beq.w	3000527a <rewrite_bp+0x182>
30005116:	4619      	mov	r1, r3
30005118:	4640      	mov	r0, r8
3000511a:	f007 f8f5 	bl	3000c308 <____wrap_memcpy_veneer>
3000511e:	2301      	movs	r3, #1
30005120:	f109 0404 	add.w	r4, r9, #4
30005124:	4649      	mov	r1, r9
30005126:	2002      	movs	r0, #2
30005128:	f88d 7014 	strb.w	r7, [sp, #20]
3000512c:	e9cd 3303 	strd	r3, r3, [sp, #12]
30005130:	f7ff feea 	bl	30004f08 <FLASH_Erase_With_Lock>
30005134:	4620      	mov	r0, r4
30005136:	217c      	movs	r1, #124	; 0x7c
30005138:	aa03      	add	r2, sp, #12
3000513a:	f7ff ff0d 	bl	30004f58 <FLASH_TxData_With_Lock>
3000513e:	4620      	mov	r0, r4
30005140:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30005144:	f04f 33ff 	mov.w	r3, #4294967295
30005148:	4c54      	ldr	r4, [pc, #336]	; (3000529c <rewrite_bp+0x1a4>)
3000514a:	f810 cb01 	ldrb.w	ip, [r0], #1
3000514e:	ea8c 0c03 	eor.w	ip, ip, r3
30005152:	4288      	cmp	r0, r1
30005154:	fa5f fc8c 	uxtb.w	ip, ip
30005158:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
3000515c:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30005160:	d1f3      	bne.n	3000514a <rewrite_bp+0x52>
30005162:	43db      	mvns	r3, r3
30005164:	4649      	mov	r1, r9
30005166:	2002      	movs	r0, #2
30005168:	9302      	str	r3, [sp, #8]
3000516a:	f7ff fecd 	bl	30004f08 <FLASH_Erase_With_Lock>
3000516e:	4642      	mov	r2, r8
30005170:	4648      	mov	r0, r9
30005172:	2180      	movs	r1, #128	; 0x80
30005174:	f7ff fef0 	bl	30004f58 <FLASH_TxData_With_Lock>
30005178:	4b49      	ldr	r3, [pc, #292]	; (300052a0 <rewrite_bp+0x1a8>)
3000517a:	aa01      	add	r2, sp, #4
3000517c:	2104      	movs	r1, #4
3000517e:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005182:	4b48      	ldr	r3, [pc, #288]	; (300052a4 <rewrite_bp+0x1ac>)
30005184:	4798      	blx	r3
30005186:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000518a:	f04f 0c01 	mov.w	ip, #1
3000518e:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30005192:	682a      	ldr	r2, [r5, #0]
30005194:	fa0c fc03 	lsl.w	ip, ip, r3
30005198:	f10e 0e0c 	add.w	lr, lr, #12
3000519c:	f8d6 300a 	ldr.w	r3, [r6, #10]
300051a0:	45e6      	cmp	lr, ip
300051a2:	6831      	ldr	r1, [r6, #0]
300051a4:	f103 030c 	add.w	r3, r3, #12
300051a8:	bf28      	it	cs
300051aa:	46e6      	movcs	lr, ip
300051ac:	459c      	cmp	ip, r3
300051ae:	bf28      	it	cs
300051b0:	469c      	movcs	ip, r3
300051b2:	bb6f      	cbnz	r7, 30005210 <rewrite_bp+0x118>
300051b4:	1d28      	adds	r0, r5, #4
300051b6:	f1be 0f00 	cmp.w	lr, #0
300051ba:	d00e      	beq.n	300051da <rewrite_bp+0xe2>
300051bc:	4486      	add	lr, r0
300051be:	f04f 37ff 	mov.w	r7, #4294967295
300051c2:	f810 3b01 	ldrb.w	r3, [r0], #1
300051c6:	407b      	eors	r3, r7
300051c8:	4570      	cmp	r0, lr
300051ca:	b2db      	uxtb	r3, r3
300051cc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051d0:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300051d4:	d1f5      	bne.n	300051c2 <rewrite_bp+0xca>
300051d6:	ea6f 0e07 	mvn.w	lr, r7
300051da:	4572      	cmp	r2, lr
300051dc:	d042      	beq.n	30005264 <rewrite_bp+0x16c>
300051de:	1d32      	adds	r2, r6, #4
300051e0:	f1bc 0f00 	cmp.w	ip, #0
300051e4:	d00e      	beq.n	30005204 <rewrite_bp+0x10c>
300051e6:	4494      	add	ip, r2
300051e8:	f04f 37ff 	mov.w	r7, #4294967295
300051ec:	f812 3b01 	ldrb.w	r3, [r2], #1
300051f0:	407b      	eors	r3, r7
300051f2:	4562      	cmp	r2, ip
300051f4:	b2db      	uxtb	r3, r3
300051f6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051fa:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
300051fe:	d1f5      	bne.n	300051ec <rewrite_bp+0xf4>
30005200:	ea6f 0c07 	mvn.w	ip, r7
30005204:	4561      	cmp	r1, ip
30005206:	d03d      	beq.n	30005284 <rewrite_bp+0x18c>
30005208:	4827      	ldr	r0, [pc, #156]	; (300052a8 <rewrite_bp+0x1b0>)
3000520a:	f007 f8bd 	bl	3000c388 <__DiagPrintf_veneer>
3000520e:	e7fe      	b.n	3000520e <rewrite_bp+0x116>
30005210:	1d30      	adds	r0, r6, #4
30005212:	f1bc 0f00 	cmp.w	ip, #0
30005216:	d00e      	beq.n	30005236 <rewrite_bp+0x13e>
30005218:	4484      	add	ip, r0
3000521a:	f04f 37ff 	mov.w	r7, #4294967295
3000521e:	f810 3b01 	ldrb.w	r3, [r0], #1
30005222:	407b      	eors	r3, r7
30005224:	4560      	cmp	r0, ip
30005226:	b2db      	uxtb	r3, r3
30005228:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000522c:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30005230:	d1f5      	bne.n	3000521e <rewrite_bp+0x126>
30005232:	ea6f 0c07 	mvn.w	ip, r7
30005236:	4561      	cmp	r1, ip
30005238:	d024      	beq.n	30005284 <rewrite_bp+0x18c>
3000523a:	1d29      	adds	r1, r5, #4
3000523c:	f1be 0f00 	cmp.w	lr, #0
30005240:	d00e      	beq.n	30005260 <rewrite_bp+0x168>
30005242:	448e      	add	lr, r1
30005244:	f04f 37ff 	mov.w	r7, #4294967295
30005248:	f811 3b01 	ldrb.w	r3, [r1], #1
3000524c:	407b      	eors	r3, r7
3000524e:	4571      	cmp	r1, lr
30005250:	b2db      	uxtb	r3, r3
30005252:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005256:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000525a:	d1f5      	bne.n	30005248 <rewrite_bp+0x150>
3000525c:	ea6f 0e07 	mvn.w	lr, r7
30005260:	4572      	cmp	r2, lr
30005262:	d1d1      	bne.n	30005208 <rewrite_bp+0x110>
30005264:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30005268:	4810      	ldr	r0, [pc, #64]	; (300052ac <rewrite_bp+0x1b4>)
3000526a:	f8d5 1006 	ldr.w	r1, [r5, #6]
3000526e:	f007 f88b 	bl	3000c388 <__DiagPrintf_veneer>
30005272:	2001      	movs	r0, #1
30005274:	b023      	add	sp, #140	; 0x8c
30005276:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000527a:	992a      	ldr	r1, [sp, #168]	; 0xa8
3000527c:	4640      	mov	r0, r8
3000527e:	f007 f843 	bl	3000c308 <____wrap_memcpy_veneer>
30005282:	e74c      	b.n	3000511e <rewrite_bp+0x26>
30005284:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
30005288:	4809      	ldr	r0, [pc, #36]	; (300052b0 <rewrite_bp+0x1b8>)
3000528a:	f8d6 1006 	ldr.w	r1, [r6, #6]
3000528e:	f007 f87b 	bl	3000c388 <__DiagPrintf_veneer>
30005292:	2002      	movs	r0, #2
30005294:	b023      	add	sp, #140	; 0x8c
30005296:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000529a:	bf00      	nop
3000529c:	3000e7e8 	.word	0x3000e7e8
300052a0:	2001c01c 	.word	0x2001c01c
300052a4:	00009db5 	.word	0x00009db5
300052a8:	3000ca04 	.word	0x3000ca04
300052ac:	3000c9d4 	.word	0x3000c9d4
300052b0:	3000c9ec 	.word	0x3000c9ec

300052b4 <BOOT_SectionInit>:
300052b4:	4800      	ldr	r0, [pc, #0]	; (300052b8 <BOOT_SectionInit+0x4>)
300052b6:	4770      	bx	lr
300052b8:	60000020 	.word	0x60000020

300052bc <BOOT_NsStart>:
300052bc:	b570      	push	{r4, r5, r6, lr}
300052be:	4c23      	ldr	r4, [pc, #140]	; (3000534c <BOOT_NsStart+0x90>)
300052c0:	4605      	mov	r5, r0
300052c2:	f384 8808 	msr	MSP, r4
300052c6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300052ca:	2100      	movs	r1, #0
300052cc:	4620      	mov	r0, r4
300052ce:	4e20      	ldr	r6, [pc, #128]	; (30005350 <BOOT_NsStart+0x94>)
300052d0:	47b0      	blx	r6
300052d2:	4a20      	ldr	r2, [pc, #128]	; (30005354 <BOOT_NsStart+0x98>)
300052d4:	6953      	ldr	r3, [r2, #20]
300052d6:	03d9      	lsls	r1, r3, #15
300052d8:	d50b      	bpl.n	300052f2 <BOOT_NsStart+0x36>
300052da:	f3bf 8f4f 	dsb	sy
300052de:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300052e2:	3420      	adds	r4, #32
300052e4:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300052e8:	d1f9      	bne.n	300052de <BOOT_NsStart+0x22>
300052ea:	f3bf 8f4f 	dsb	sy
300052ee:	f3bf 8f6f 	isb	sy
300052f2:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300052f6:	f383 8808 	msr	MSP, r3
300052fa:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300052fe:	2100      	movs	r1, #0
30005300:	4815      	ldr	r0, [pc, #84]	; (30005358 <BOOT_NsStart+0x9c>)
30005302:	47b0      	blx	r6
30005304:	4a13      	ldr	r2, [pc, #76]	; (30005354 <BOOT_NsStart+0x98>)
30005306:	6953      	ldr	r3, [r2, #20]
30005308:	03db      	lsls	r3, r3, #15
3000530a:	d50c      	bpl.n	30005326 <BOOT_NsStart+0x6a>
3000530c:	f3bf 8f4f 	dsb	sy
30005310:	4b11      	ldr	r3, [pc, #68]	; (30005358 <BOOT_NsStart+0x9c>)
30005312:	490e      	ldr	r1, [pc, #56]	; (3000534c <BOOT_NsStart+0x90>)
30005314:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30005318:	3320      	adds	r3, #32
3000531a:	428b      	cmp	r3, r1
3000531c:	d1fa      	bne.n	30005314 <BOOT_NsStart+0x58>
3000531e:	f3bf 8f4f 	dsb	sy
30005322:	f3bf 8f6f 	isb	sy
30005326:	f025 0501 	bic.w	r5, r5, #1
3000532a:	086d      	lsrs	r5, r5, #1
3000532c:	006d      	lsls	r5, r5, #1
3000532e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005332:	ed2d 8b10 	vpush	{d8-d15}
30005336:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
3000533a:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
3000533e:	47ac      	blxns	r5
30005340:	ecbd 8b10 	vpop	{d8-d15}
30005344:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30005348:	bd70      	pop	{r4, r5, r6, pc}
3000534a:	bf00      	nop
3000534c:	30002000 	.word	0x30002000
30005350:	00012be5 	.word	0x00012be5
30005354:	e000ed00 	.word	0xe000ed00
30005358:	30001000 	.word	0x30001000

3000535c <BOOT_RccConfig>:
3000535c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30005360:	4c2f      	ldr	r4, [pc, #188]	; (30005420 <BOOT_RccConfig+0xc4>)
30005362:	b090      	sub	sp, #64	; 0x40
30005364:	4f2f      	ldr	r7, [pc, #188]	; (30005424 <BOOT_RccConfig+0xc8>)
30005366:	466d      	mov	r5, sp
30005368:	f10d 0c20 	add.w	ip, sp, #32
3000536c:	683e      	ldr	r6, [r7, #0]
3000536e:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
30005372:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30005376:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
3000537a:	f104 0310 	add.w	r3, r4, #16
3000537e:	2400      	movs	r4, #0
30005380:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30005382:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
30005386:	1c72      	adds	r2, r6, #1
30005388:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000538c:	e9cd 4406 	strd	r4, r4, [sp, #24]
30005390:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
30005394:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30005398:	d03f      	beq.n	3000541a <BOOT_RccConfig+0xbe>
3000539a:	463a      	mov	r2, r7
3000539c:	6891      	ldr	r1, [r2, #8]
3000539e:	0fb3      	lsrs	r3, r6, #30
300053a0:	a810      	add	r0, sp, #64	; 0x40
300053a2:	2901      	cmp	r1, #1
300053a4:	eb00 0383 	add.w	r3, r0, r3, lsl #2
300053a8:	d10e      	bne.n	300053c8 <BOOT_RccConfig+0x6c>
300053aa:	f853 0c30 	ldr.w	r0, [r3, #-48]
300053ae:	6854      	ldr	r4, [r2, #4]
300053b0:	4330      	orrs	r0, r6
300053b2:	ae10      	add	r6, sp, #64	; 0x40
300053b4:	0fa1      	lsrs	r1, r4, #30
300053b6:	f843 0c30 	str.w	r0, [r3, #-48]
300053ba:	eb06 0181 	add.w	r1, r6, r1, lsl #2
300053be:	f851 3c10 	ldr.w	r3, [r1, #-16]
300053c2:	4323      	orrs	r3, r4
300053c4:	f841 3c10 	str.w	r3, [r1, #-16]
300053c8:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300053cc:	1c73      	adds	r3, r6, #1
300053ce:	d1e5      	bne.n	3000539c <BOOT_RccConfig+0x40>
300053d0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300053d2:	9c04      	ldr	r4, [sp, #16]
300053d4:	af0d      	add	r7, sp, #52	; 0x34
300053d6:	ae05      	add	r6, sp, #20
300053d8:	4a13      	ldr	r2, [pc, #76]	; (30005428 <BOOT_RccConfig+0xcc>)
300053da:	f85c 0b04 	ldr.w	r0, [ip], #4
300053de:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300053e2:	f855 1b04 	ldr.w	r1, [r5], #4
300053e6:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300053ea:	f850 8002 	ldr.w	r8, [r0, r2]
300053ee:	45e6      	cmp	lr, ip
300053f0:	ea43 0308 	orr.w	r3, r3, r8
300053f4:	5083      	str	r3, [r0, r2]
300053f6:	588b      	ldr	r3, [r1, r2]
300053f8:	ea44 0403 	orr.w	r4, r4, r3
300053fc:	508c      	str	r4, [r1, r2]
300053fe:	d004      	beq.n	3000540a <BOOT_RccConfig+0xae>
30005400:	f857 3b04 	ldr.w	r3, [r7], #4
30005404:	f856 4b04 	ldr.w	r4, [r6], #4
30005408:	e7e7      	b.n	300053da <BOOT_RccConfig+0x7e>
3000540a:	200a      	movs	r0, #10
3000540c:	4b07      	ldr	r3, [pc, #28]	; (3000542c <BOOT_RccConfig+0xd0>)
3000540e:	4798      	blx	r3
30005410:	4b07      	ldr	r3, [pc, #28]	; (30005430 <BOOT_RccConfig+0xd4>)
30005412:	b010      	add	sp, #64	; 0x40
30005414:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30005418:	4718      	bx	r3
3000541a:	4623      	mov	r3, r4
3000541c:	e7da      	b.n	300053d4 <BOOT_RccConfig+0x78>
3000541e:	bf00      	nop
30005420:	3000e308 	.word	0x3000e308
30005424:	3000e62c 	.word	0x3000e62c
30005428:	42008000 	.word	0x42008000
3000542c:	00009b2d 	.word	0x00009b2d
30005430:	30007e75 	.word	0x30007e75

30005434 <BOOT_CACHEWRR_Set>:
30005434:	4b14      	ldr	r3, [pc, #80]	; (30005488 <BOOT_CACHEWRR_Set+0x54>)
30005436:	b430      	push	{r4, r5}
30005438:	681a      	ldr	r2, [r3, #0]
3000543a:	2a01      	cmp	r2, #1
3000543c:	d10f      	bne.n	3000545e <BOOT_CACHEWRR_Set+0x2a>
3000543e:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
30005442:	685d      	ldr	r5, [r3, #4]
30005444:	689c      	ldr	r4, [r3, #8]
30005446:	6882      	ldr	r2, [r0, #8]
30005448:	4910      	ldr	r1, [pc, #64]	; (3000548c <BOOT_CACHEWRR_Set+0x58>)
3000544a:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
3000544e:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30005452:	6082      	str	r2, [r0, #8]
30005454:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30005458:	68da      	ldr	r2, [r3, #12]
3000545a:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
3000545e:	691a      	ldr	r2, [r3, #16]
30005460:	2a01      	cmp	r2, #1
30005462:	d10f      	bne.n	30005484 <BOOT_CACHEWRR_Set+0x50>
30005464:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30005468:	695d      	ldr	r5, [r3, #20]
3000546a:	4a08      	ldr	r2, [pc, #32]	; (3000548c <BOOT_CACHEWRR_Set+0x58>)
3000546c:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30005470:	688b      	ldr	r3, [r1, #8]
30005472:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
30005476:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
3000547a:	608b      	str	r3, [r1, #8]
3000547c:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30005480:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
30005484:	bc30      	pop	{r4, r5}
30005486:	4770      	bx	lr
30005488:	3000ee50 	.word	0x3000ee50
3000548c:	e0042000 	.word	0xe0042000

30005490 <BOOT_TCMSet>:
30005490:	2300      	movs	r3, #0
30005492:	b5f0      	push	{r4, r5, r6, r7, lr}
30005494:	4db3      	ldr	r5, [pc, #716]	; (30005764 <BOOT_TCMSet+0x2d4>)
30005496:	461c      	mov	r4, r3
30005498:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
3000549c:	3307      	adds	r3, #7
3000549e:	4282      	cmp	r2, r0
300054a0:	d03c      	beq.n	3000551c <BOOT_TCMSet+0x8c>
300054a2:	3401      	adds	r4, #1
300054a4:	2c07      	cmp	r4, #7
300054a6:	d1f7      	bne.n	30005498 <BOOT_TCMSet+0x8>
300054a8:	f3bf 8f4f 	dsb	sy
300054ac:	f3bf 8f6f 	isb	sy
300054b0:	48ad      	ldr	r0, [pc, #692]	; (30005768 <BOOT_TCMSet+0x2d8>)
300054b2:	2200      	movs	r2, #0
300054b4:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300054b8:	f3bf 8f4f 	dsb	sy
300054bc:	f3bf 8f6f 	isb	sy
300054c0:	6943      	ldr	r3, [r0, #20]
300054c2:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300054c6:	6143      	str	r3, [r0, #20]
300054c8:	f3bf 8f4f 	dsb	sy
300054cc:	f3bf 8f6f 	isb	sy
300054d0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300054d4:	f3bf 8f4f 	dsb	sy
300054d8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300054dc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300054e0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300054e4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300054e8:	0164      	lsls	r4, r4, #5
300054ea:	ea04 0106 	and.w	r1, r4, r6
300054ee:	462b      	mov	r3, r5
300054f0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300054f4:	3b01      	subs	r3, #1
300054f6:	1c5f      	adds	r7, r3, #1
300054f8:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300054fc:	d1f8      	bne.n	300054f0 <BOOT_TCMSet+0x60>
300054fe:	3c20      	subs	r4, #32
30005500:	f114 0f20 	cmn.w	r4, #32
30005504:	d1f1      	bne.n	300054ea <BOOT_TCMSet+0x5a>
30005506:	f3bf 8f4f 	dsb	sy
3000550a:	6943      	ldr	r3, [r0, #20]
3000550c:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30005510:	6143      	str	r3, [r0, #20]
30005512:	f3bf 8f4f 	dsb	sy
30005516:	f3bf 8f6f 	isb	sy
3000551a:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000551c:	2c00      	cmp	r4, #0
3000551e:	d0c3      	beq.n	300054a8 <BOOT_TCMSet+0x18>
30005520:	f3bf 8f4f 	dsb	sy
30005524:	f3bf 8f6f 	isb	sy
30005528:	4890      	ldr	r0, [pc, #576]	; (3000576c <BOOT_TCMSet+0x2dc>)
3000552a:	2300      	movs	r3, #0
3000552c:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30005530:	f3bf 8f4f 	dsb	sy
30005534:	f3bf 8f6f 	isb	sy
30005538:	6942      	ldr	r2, [r0, #20]
3000553a:	03d2      	lsls	r2, r2, #15
3000553c:	d520      	bpl.n	30005580 <BOOT_TCMSet+0xf0>
3000553e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005542:	f3bf 8f4f 	dsb	sy
30005546:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000554a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000554e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30005552:	f3c3 334e 	ubfx	r3, r3, #13, #15
30005556:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000555a:	ea0c 0106 	and.w	r1, ip, r6
3000555e:	4673      	mov	r3, lr
30005560:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30005564:	3b01      	subs	r3, #1
30005566:	1c5f      	adds	r7, r3, #1
30005568:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000556c:	d1f8      	bne.n	30005560 <BOOT_TCMSet+0xd0>
3000556e:	f1ac 0c20 	sub.w	ip, ip, #32
30005572:	f11c 0f20 	cmn.w	ip, #32
30005576:	d1f0      	bne.n	3000555a <BOOT_TCMSet+0xca>
30005578:	f3bf 8f4f 	dsb	sy
3000557c:	f3bf 8f6f 	isb	sy
30005580:	f3bf 8f4f 	dsb	sy
30005584:	f3bf 8f6f 	isb	sy
30005588:	4878      	ldr	r0, [pc, #480]	; (3000576c <BOOT_TCMSet+0x2dc>)
3000558a:	2200      	movs	r2, #0
3000558c:	6943      	ldr	r3, [r0, #20]
3000558e:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
30005592:	6143      	str	r3, [r0, #20]
30005594:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30005598:	f3bf 8f4f 	dsb	sy
3000559c:	f3bf 8f6f 	isb	sy
300055a0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300055a4:	f3bf 8f4f 	dsb	sy
300055a8:	6943      	ldr	r3, [r0, #20]
300055aa:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300055ae:	6143      	str	r3, [r0, #20]
300055b0:	f3bf 8f4f 	dsb	sy
300055b4:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300055b8:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300055bc:	f3c3 06c9 	ubfx	r6, r3, #3, #10
300055c0:	f3c3 334e 	ubfx	r3, r3, #13, #15
300055c4:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300055c8:	ea0c 010e 	and.w	r1, ip, lr
300055cc:	4633      	mov	r3, r6
300055ce:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300055d2:	3b01      	subs	r3, #1
300055d4:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300055d8:	1c5a      	adds	r2, r3, #1
300055da:	d1f8      	bne.n	300055ce <BOOT_TCMSet+0x13e>
300055dc:	f1ac 0c20 	sub.w	ip, ip, #32
300055e0:	f11c 0f20 	cmn.w	ip, #32
300055e4:	d1f0      	bne.n	300055c8 <BOOT_TCMSet+0x138>
300055e6:	f3bf 8f4f 	dsb	sy
300055ea:	f3bf 8f6f 	isb	sy
300055ee:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300055f2:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300055f6:	00e3      	lsls	r3, r4, #3
300055f8:	6b41      	ldr	r1, [r0, #52]	; 0x34
300055fa:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300055fe:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
30005602:	6341      	str	r1, [r0, #52]	; 0x34
30005604:	6911      	ldr	r1, [r2, #16]
30005606:	2903      	cmp	r1, #3
30005608:	d00f      	beq.n	3000562a <BOOT_TCMSet+0x19a>
3000560a:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000560e:	4858      	ldr	r0, [pc, #352]	; (30005770 <BOOT_TCMSet+0x2e0>)
30005610:	f04f 0c00 	mov.w	ip, #0
30005614:	4f57      	ldr	r7, [pc, #348]	; (30005774 <BOOT_TCMSet+0x2e4>)
30005616:	68b2      	ldr	r2, [r6, #8]
30005618:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
3000561c:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
30005620:	60b2      	str	r2, [r6, #8]
30005622:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
30005626:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
3000562a:	1b1a      	subs	r2, r3, r4
3000562c:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30005630:	68d1      	ldr	r1, [r2, #12]
30005632:	2903      	cmp	r1, #3
30005634:	d00f      	beq.n	30005656 <BOOT_TCMSet+0x1c6>
30005636:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000563a:	484d      	ldr	r0, [pc, #308]	; (30005770 <BOOT_TCMSet+0x2e0>)
3000563c:	f04f 0c00 	mov.w	ip, #0
30005640:	4f4c      	ldr	r7, [pc, #304]	; (30005774 <BOOT_TCMSet+0x2e4>)
30005642:	68b2      	ldr	r2, [r6, #8]
30005644:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30005648:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
3000564c:	60b2      	str	r2, [r6, #8]
3000564e:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
30005652:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
30005656:	1b1a      	subs	r2, r3, r4
30005658:	4945      	ldr	r1, [pc, #276]	; (30005770 <BOOT_TCMSet+0x2e0>)
3000565a:	eb05 0282 	add.w	r2, r5, r2, lsl #2
3000565e:	6950      	ldr	r0, [r2, #20]
30005660:	6608      	str	r0, [r1, #96]	; 0x60
30005662:	6990      	ldr	r0, [r2, #24]
30005664:	6648      	str	r0, [r1, #100]	; 0x64
30005666:	6852      	ldr	r2, [r2, #4]
30005668:	2a01      	cmp	r2, #1
3000566a:	d04b      	beq.n	30005704 <BOOT_TCMSet+0x274>
3000566c:	1b1b      	subs	r3, r3, r4
3000566e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30005672:	68ab      	ldr	r3, [r5, #8]
30005674:	2b01      	cmp	r3, #1
30005676:	f47f af50 	bne.w	3000551a <BOOT_TCMSet+0x8a>
3000567a:	483b      	ldr	r0, [pc, #236]	; (30005768 <BOOT_TCMSet+0x2d8>)
3000567c:	2300      	movs	r3, #0
3000567e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30005682:	f3bf 8f4f 	dsb	sy
30005686:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000568a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000568e:	f3c5 344e 	ubfx	r4, r5, #13, #15
30005692:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30005696:	0164      	lsls	r4, r4, #5
30005698:	ea04 0106 	and.w	r1, r4, r6
3000569c:	462b      	mov	r3, r5
3000569e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056a2:	3b01      	subs	r3, #1
300056a4:	1c5f      	adds	r7, r3, #1
300056a6:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300056aa:	d1f8      	bne.n	3000569e <BOOT_TCMSet+0x20e>
300056ac:	3c20      	subs	r4, #32
300056ae:	f114 0f20 	cmn.w	r4, #32
300056b2:	d1f1      	bne.n	30005698 <BOOT_TCMSet+0x208>
300056b4:	f3bf 8f4f 	dsb	sy
300056b8:	6943      	ldr	r3, [r0, #20]
300056ba:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300056be:	6143      	str	r3, [r0, #20]
300056c0:	f3bf 8f4f 	dsb	sy
300056c4:	f3bf 8f6f 	isb	sy
300056c8:	4828      	ldr	r0, [pc, #160]	; (3000576c <BOOT_TCMSet+0x2dc>)
300056ca:	2300      	movs	r3, #0
300056cc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300056d0:	f3bf 8f4f 	dsb	sy
300056d4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300056d8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300056dc:	f3c5 344e 	ubfx	r4, r5, #13, #15
300056e0:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300056e4:	0164      	lsls	r4, r4, #5
300056e6:	ea04 0106 	and.w	r1, r4, r6
300056ea:	462b      	mov	r3, r5
300056ec:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300056f0:	3b01      	subs	r3, #1
300056f2:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300056f6:	1c5a      	adds	r2, r3, #1
300056f8:	d1f8      	bne.n	300056ec <BOOT_TCMSet+0x25c>
300056fa:	3c20      	subs	r4, #32
300056fc:	f114 0f20 	cmn.w	r4, #32
30005700:	d1f1      	bne.n	300056e6 <BOOT_TCMSet+0x256>
30005702:	e700      	b.n	30005506 <BOOT_TCMSet+0x76>
30005704:	f3bf 8f4f 	dsb	sy
30005708:	f3bf 8f6f 	isb	sy
3000570c:	4a16      	ldr	r2, [pc, #88]	; (30005768 <BOOT_TCMSet+0x2d8>)
3000570e:	2000      	movs	r0, #0
30005710:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30005714:	f3bf 8f4f 	dsb	sy
30005718:	f3bf 8f6f 	isb	sy
3000571c:	6951      	ldr	r1, [r2, #20]
3000571e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30005722:	6151      	str	r1, [r2, #20]
30005724:	f3bf 8f4f 	dsb	sy
30005728:	f3bf 8f6f 	isb	sy
3000572c:	f3bf 8f4f 	dsb	sy
30005730:	f3bf 8f6f 	isb	sy
30005734:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30005738:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
3000573c:	f3bf 8f4f 	dsb	sy
30005740:	f3bf 8f6f 	isb	sy
30005744:	6951      	ldr	r1, [r2, #20]
30005746:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000574a:	6151      	str	r1, [r2, #20]
3000574c:	f3bf 8f4f 	dsb	sy
30005750:	f3bf 8f6f 	isb	sy
30005754:	1b1b      	subs	r3, r3, r4
30005756:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000575a:	68ab      	ldr	r3, [r5, #8]
3000575c:	2b01      	cmp	r3, #1
3000575e:	f47f aedc 	bne.w	3000551a <BOOT_TCMSet+0x8a>
30005762:	e78a      	b.n	3000567a <BOOT_TCMSet+0x1ea>
30005764:	3000ed8c 	.word	0x3000ed8c
30005768:	e002ed00 	.word	0xe002ed00
3000576c:	e000ed00 	.word	0xe000ed00
30005770:	e0042000 	.word	0xe0042000
30005774:	ffff0000 	.word	0xffff0000

30005778 <BOOT_GRstConfig>:
30005778:	b538      	push	{r3, r4, r5, lr}
3000577a:	4c09      	ldr	r4, [pc, #36]	; (300057a0 <BOOT_GRstConfig+0x28>)
3000577c:	f241 050a 	movw	r5, #4106	; 0x100a
30005780:	2028      	movs	r0, #40	; 0x28
30005782:	4a08      	ldr	r2, [pc, #32]	; (300057a4 <BOOT_GRstConfig+0x2c>)
30005784:	8823      	ldrh	r3, [r4, #0]
30005786:	b29b      	uxth	r3, r3
30005788:	431d      	orrs	r5, r3
3000578a:	8025      	strh	r5, [r4, #0]
3000578c:	4790      	blx	r2
3000578e:	88a3      	ldrh	r3, [r4, #4]
30005790:	b29b      	uxth	r3, r3
30005792:	ea23 0305 	bic.w	r3, r3, r5
30005796:	80a3      	strh	r3, [r4, #4]
30005798:	8863      	ldrh	r3, [r4, #2]
3000579a:	8065      	strh	r5, [r4, #2]
3000579c:	bd38      	pop	{r3, r4, r5, pc}
3000579e:	bf00      	nop
300057a0:	42008200 	.word	0x42008200
300057a4:	00009b2d 	.word	0x00009b2d

300057a8 <BOOT_DDR_LCDC_HPR>:
300057a8:	4a04      	ldr	r2, [pc, #16]	; (300057bc <BOOT_DDR_LCDC_HPR+0x14>)
300057aa:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300057ae:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300057b2:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300057b6:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300057ba:	4770      	bx	lr
300057bc:	42008000 	.word	0x42008000

300057c0 <BOOT_DDR_Init>:
300057c0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300057c4:	b530      	push	{r4, r5, lr}
300057c6:	4d15      	ldr	r5, [pc, #84]	; (3000581c <BOOT_DDR_Init+0x5c>)
300057c8:	b099      	sub	sp, #100	; 0x64
300057ca:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300057cc:	47a8      	blx	r5
300057ce:	2803      	cmp	r0, #3
300057d0:	d11e      	bne.n	30005810 <BOOT_DDR_Init+0x50>
300057d2:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300057d6:	4a12      	ldr	r2, [pc, #72]	; (30005820 <BOOT_DDR_Init+0x60>)
300057d8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300057dc:	4911      	ldr	r1, [pc, #68]	; (30005824 <BOOT_DDR_Init+0x64>)
300057de:	a801      	add	r0, sp, #4
300057e0:	631c      	str	r4, [r3, #48]	; 0x30
300057e2:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300057e6:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300057ea:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300057ee:	4788      	blx	r1
300057f0:	a801      	add	r0, sp, #4
300057f2:	4b0d      	ldr	r3, [pc, #52]	; (30005828 <BOOT_DDR_Init+0x68>)
300057f4:	4798      	blx	r3
300057f6:	4b0d      	ldr	r3, [pc, #52]	; (3000582c <BOOT_DDR_Init+0x6c>)
300057f8:	4798      	blx	r3
300057fa:	4b0d      	ldr	r3, [pc, #52]	; (30005830 <BOOT_DDR_Init+0x70>)
300057fc:	4798      	blx	r3
300057fe:	4b0d      	ldr	r3, [pc, #52]	; (30005834 <BOOT_DDR_Init+0x74>)
30005800:	4798      	blx	r3
30005802:	2101      	movs	r1, #1
30005804:	f240 10ff 	movw	r0, #511	; 0x1ff
30005808:	4b0b      	ldr	r3, [pc, #44]	; (30005838 <BOOT_DDR_Init+0x78>)
3000580a:	4798      	blx	r3
3000580c:	b019      	add	sp, #100	; 0x64
3000580e:	bd30      	pop	{r4, r5, pc}
30005810:	47a8      	blx	r5
30005812:	2802      	cmp	r0, #2
30005814:	d0dd      	beq.n	300057d2 <BOOT_DDR_Init+0x12>
30005816:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
3000581a:	e7dc      	b.n	300057d6 <BOOT_DDR_Init+0x16>
3000581c:	0000c0f9 	.word	0x0000c0f9
30005820:	42008000 	.word	0x42008000
30005824:	30007f91 	.word	0x30007f91
30005828:	30008495 	.word	0x30008495
3000582c:	3000b26d 	.word	0x3000b26d
30005830:	30008565 	.word	0x30008565
30005834:	30008569 	.word	0x30008569
30005838:	3000b215 	.word	0x3000b215

3000583c <BOOT_PSRAM_Init>:
3000583c:	b500      	push	{lr}
3000583e:	b089      	sub	sp, #36	; 0x24
30005840:	4b1b      	ldr	r3, [pc, #108]	; (300058b0 <BOOT_PSRAM_Init+0x74>)
30005842:	a801      	add	r0, sp, #4
30005844:	4798      	blx	r3
30005846:	a801      	add	r0, sp, #4
30005848:	4b1a      	ldr	r3, [pc, #104]	; (300058b4 <BOOT_PSRAM_Init+0x78>)
3000584a:	4798      	blx	r3
3000584c:	4b1a      	ldr	r3, [pc, #104]	; (300058b8 <BOOT_PSRAM_Init+0x7c>)
3000584e:	4798      	blx	r3
30005850:	4b1a      	ldr	r3, [pc, #104]	; (300058bc <BOOT_PSRAM_Init+0x80>)
30005852:	4798      	blx	r3
30005854:	2801      	cmp	r0, #1
30005856:	d01d      	beq.n	30005894 <BOOT_PSRAM_Init+0x58>
30005858:	4b19      	ldr	r3, [pc, #100]	; (300058c0 <BOOT_PSRAM_Init+0x84>)
3000585a:	2004      	movs	r0, #4
3000585c:	4a19      	ldr	r2, [pc, #100]	; (300058c4 <BOOT_PSRAM_Init+0x88>)
3000585e:	4619      	mov	r1, r3
30005860:	f006 faec 	bl	3000be3c <rtk_log_write>
30005864:	4b18      	ldr	r3, [pc, #96]	; (300058c8 <BOOT_PSRAM_Init+0x8c>)
30005866:	4798      	blx	r3
30005868:	4b18      	ldr	r3, [pc, #96]	; (300058cc <BOOT_PSRAM_Init+0x90>)
3000586a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000586e:	009b      	lsls	r3, r3, #2
30005870:	d517      	bpl.n	300058a2 <BOOT_PSRAM_Init+0x66>
30005872:	4a17      	ldr	r2, [pc, #92]	; (300058d0 <BOOT_PSRAM_Init+0x94>)
30005874:	4b17      	ldr	r3, [pc, #92]	; (300058d4 <BOOT_PSRAM_Init+0x98>)
30005876:	6812      	ldr	r2, [r2, #0]
30005878:	605a      	str	r2, [r3, #4]
3000587a:	4a17      	ldr	r2, [pc, #92]	; (300058d8 <BOOT_PSRAM_Init+0x9c>)
3000587c:	f04f 0c20 	mov.w	ip, #32
30005880:	2101      	movs	r1, #1
30005882:	4b16      	ldr	r3, [pc, #88]	; (300058dc <BOOT_PSRAM_Init+0xa0>)
30005884:	6952      	ldr	r2, [r2, #20]
30005886:	4608      	mov	r0, r1
30005888:	fbbc f2f2 	udiv	r2, ip, r2
3000588c:	4798      	blx	r3
3000588e:	b009      	add	sp, #36	; 0x24
30005890:	f85d fb04 	ldr.w	pc, [sp], #4
30005894:	4b12      	ldr	r3, [pc, #72]	; (300058e0 <BOOT_PSRAM_Init+0xa4>)
30005896:	4798      	blx	r3
30005898:	4b0c      	ldr	r3, [pc, #48]	; (300058cc <BOOT_PSRAM_Init+0x90>)
3000589a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000589e:	009b      	lsls	r3, r3, #2
300058a0:	d4e7      	bmi.n	30005872 <BOOT_PSRAM_Init+0x36>
300058a2:	4b10      	ldr	r3, [pc, #64]	; (300058e4 <BOOT_PSRAM_Init+0xa8>)
300058a4:	4798      	blx	r3
300058a6:	4a0b      	ldr	r2, [pc, #44]	; (300058d4 <BOOT_PSRAM_Init+0x98>)
300058a8:	4b09      	ldr	r3, [pc, #36]	; (300058d0 <BOOT_PSRAM_Init+0x94>)
300058aa:	6852      	ldr	r2, [r2, #4]
300058ac:	601a      	str	r2, [r3, #0]
300058ae:	e7e4      	b.n	3000587a <BOOT_PSRAM_Init+0x3e>
300058b0:	30008cb5 	.word	0x30008cb5
300058b4:	30008cd5 	.word	0x30008cd5
300058b8:	30008d3d 	.word	0x30008d3d
300058bc:	30007e29 	.word	0x30007e29
300058c0:	3000c674 	.word	0x3000c674
300058c4:	3000ca24 	.word	0x3000ca24
300058c8:	30008fe1 	.word	0x30008fe1
300058cc:	42008000 	.word	0x42008000
300058d0:	30010014 	.word	0x30010014
300058d4:	41002000 	.word	0x41002000
300058d8:	3000fbb0 	.word	0x3000fbb0
300058dc:	300092c1 	.word	0x300092c1
300058e0:	30008f09 	.word	0x30008f09
300058e4:	3000901d 	.word	0x3000901d

300058e8 <BOOT_WakeFromPG>:
300058e8:	b538      	push	{r3, r4, r5, lr}
300058ea:	4c49      	ldr	r4, [pc, #292]	; (30005a10 <BOOT_WakeFromPG+0x128>)
300058ec:	f7ff f910 	bl	30004b10 <BOOT_RAM_TZCfg>
300058f0:	69a3      	ldr	r3, [r4, #24]
300058f2:	b10b      	cbz	r3, 300058f8 <BOOT_WakeFromPG+0x10>
300058f4:	4a47      	ldr	r2, [pc, #284]	; (30005a14 <BOOT_WakeFromPG+0x12c>)
300058f6:	6093      	str	r3, [r2, #8]
300058f8:	4b47      	ldr	r3, [pc, #284]	; (30005a18 <BOOT_WakeFromPG+0x130>)
300058fa:	f06f 0c1a 	mvn.w	ip, #26
300058fe:	4847      	ldr	r0, [pc, #284]	; (30005a1c <BOOT_WakeFromPG+0x134>)
30005900:	f103 0e50 	add.w	lr, r3, #80	; 0x50
30005904:	ebac 0c04 	sub.w	ip, ip, r4
30005908:	eb0c 0203 	add.w	r2, ip, r3
3000590c:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005910:	4402      	add	r2, r0
30005912:	4573      	cmp	r3, lr
30005914:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005918:	d1f6      	bne.n	30005908 <BOOT_WakeFromPG+0x20>
3000591a:	6822      	ldr	r2, [r4, #0]
3000591c:	4b3d      	ldr	r3, [pc, #244]	; (30005a14 <BOOT_WakeFromPG+0x12c>)
3000591e:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30005922:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30005926:	6002      	str	r2, [r0, #0]
30005928:	6045      	str	r5, [r0, #4]
3000592a:	6081      	str	r1, [r0, #8]
3000592c:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000592e:	4a3c      	ldr	r2, [pc, #240]	; (30005a20 <BOOT_WakeFromPG+0x138>)
30005930:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005934:	4d3b      	ldr	r5, [pc, #236]	; (30005a24 <BOOT_WakeFromPG+0x13c>)
30005936:	6259      	str	r1, [r3, #36]	; 0x24
30005938:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000593a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
3000593e:	6251      	str	r1, [r2, #36]	; 0x24
30005940:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005944:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005948:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000594c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005950:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005954:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005958:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000595c:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005960:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005964:	f7ff fd66 	bl	30005434 <BOOT_CACHEWRR_Set>
30005968:	4b2f      	ldr	r3, [pc, #188]	; (30005a28 <BOOT_WakeFromPG+0x140>)
3000596a:	6818      	ldr	r0, [r3, #0]
3000596c:	f7ff fd90 	bl	30005490 <BOOT_TCMSet>
30005970:	2201      	movs	r2, #1
30005972:	492e      	ldr	r1, [pc, #184]	; (30005a2c <BOOT_WakeFromPG+0x144>)
30005974:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30005978:	47a8      	blx	r5
3000597a:	4b2d      	ldr	r3, [pc, #180]	; (30005a30 <BOOT_WakeFromPG+0x148>)
3000597c:	4798      	blx	r3
3000597e:	4b2d      	ldr	r3, [pc, #180]	; (30005a34 <BOOT_WakeFromPG+0x14c>)
30005980:	4798      	blx	r3
30005982:	2801      	cmp	r0, #1
30005984:	4604      	mov	r4, r0
30005986:	d026      	beq.n	300059d6 <BOOT_WakeFromPG+0xee>
30005988:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000598c:	2201      	movs	r2, #1
3000598e:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005992:	47a8      	blx	r5
30005994:	4a28      	ldr	r2, [pc, #160]	; (30005a38 <BOOT_WakeFromPG+0x150>)
30005996:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000599a:	f043 0302 	orr.w	r3, r3, #2
3000599e:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
300059a2:	f7ff ff4b 	bl	3000583c <BOOT_PSRAM_Init>
300059a6:	2096      	movs	r0, #150	; 0x96
300059a8:	4b24      	ldr	r3, [pc, #144]	; (30005a3c <BOOT_WakeFromPG+0x154>)
300059aa:	4798      	blx	r3
300059ac:	4a24      	ldr	r2, [pc, #144]	; (30005a40 <BOOT_WakeFromPG+0x158>)
300059ae:	4c1c      	ldr	r4, [pc, #112]	; (30005a20 <BOOT_WakeFromPG+0x138>)
300059b0:	6893      	ldr	r3, [r2, #8]
300059b2:	4924      	ldr	r1, [pc, #144]	; (30005a44 <BOOT_WakeFromPG+0x15c>)
300059b4:	6852      	ldr	r2, [r2, #4]
300059b6:	4824      	ldr	r0, [pc, #144]	; (30005a48 <BOOT_WakeFromPG+0x160>)
300059b8:	605a      	str	r2, [r3, #4]
300059ba:	60a3      	str	r3, [r4, #8]
300059bc:	4a23      	ldr	r2, [pc, #140]	; (30005a4c <BOOT_WakeFromPG+0x164>)
300059be:	61c8      	str	r0, [r1, #28]
300059c0:	f382 8888 	msr	MSP_NS, r2
300059c4:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
300059c8:	f382 8809 	msr	PSP, r2
300059cc:	6858      	ldr	r0, [r3, #4]
300059ce:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300059d2:	f7ff bc73 	b.w	300052bc <BOOT_NsStart>
300059d6:	4b1e      	ldr	r3, [pc, #120]	; (30005a50 <BOOT_WakeFromPG+0x168>)
300059d8:	2004      	movs	r0, #4
300059da:	4a1e      	ldr	r2, [pc, #120]	; (30005a54 <BOOT_WakeFromPG+0x16c>)
300059dc:	4619      	mov	r1, r3
300059de:	f006 fa2d 	bl	3000be3c <rtk_log_write>
300059e2:	4622      	mov	r2, r4
300059e4:	491c      	ldr	r1, [pc, #112]	; (30005a58 <BOOT_WakeFromPG+0x170>)
300059e6:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300059ea:	47a8      	blx	r5
300059ec:	4622      	mov	r2, r4
300059ee:	491b      	ldr	r1, [pc, #108]	; (30005a5c <BOOT_WakeFromPG+0x174>)
300059f0:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300059f4:	47a8      	blx	r5
300059f6:	f7ff fee3 	bl	300057c0 <BOOT_DDR_Init>
300059fa:	4a0f      	ldr	r2, [pc, #60]	; (30005a38 <BOOT_WakeFromPG+0x150>)
300059fc:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30005a00:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005a04:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005a08:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30005a0c:	e7ce      	b.n	300059ac <BOOT_WakeFromPG+0xc4>
30005a0e:	bf00      	nop
30005a10:	3000ffa8 	.word	0x3000ffa8
30005a14:	e000ed00 	.word	0xe000ed00
30005a18:	3000ffc3 	.word	0x3000ffc3
30005a1c:	e000e100 	.word	0xe000e100
30005a20:	e002ed00 	.word	0xe002ed00
30005a24:	0000b479 	.word	0x0000b479
30005a28:	3000e624 	.word	0x3000e624
30005a2c:	40001000 	.word	0x40001000
30005a30:	0000d835 	.word	0x0000d835
30005a34:	30007d51 	.word	0x30007d51
30005a38:	42008000 	.word	0x42008000
30005a3c:	00009b2d 	.word	0x00009b2d
30005a40:	60000020 	.word	0x60000020
30005a44:	30000000 	.word	0x30000000
30005a48:	30004cad 	.word	0x30004cad
30005a4c:	2001bffc 	.word	0x2001bffc
30005a50:	3000c674 	.word	0x3000c674
30005a54:	3000ca38 	.word	0x3000ca38
30005a58:	40080000 	.word	0x40080000
30005a5c:	40040000 	.word	0x40040000

30005a60 <BOOT_SCBConfig_HP>:
30005a60:	4b0e      	ldr	r3, [pc, #56]	; (30005a9c <BOOT_SCBConfig_HP+0x3c>)
30005a62:	4a0f      	ldr	r2, [pc, #60]	; (30005aa0 <BOOT_SCBConfig_HP+0x40>)
30005a64:	6a59      	ldr	r1, [r3, #36]	; 0x24
30005a66:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30005a6a:	6259      	str	r1, [r3, #36]	; 0x24
30005a6c:	6a51      	ldr	r1, [r2, #36]	; 0x24
30005a6e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30005a72:	6251      	str	r1, [r2, #36]	; 0x24
30005a74:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30005a78:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a7c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30005a80:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30005a84:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30005a88:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30005a8c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30005a90:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30005a94:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30005a98:	4770      	bx	lr
30005a9a:	bf00      	nop
30005a9c:	e000ed00 	.word	0xe000ed00
30005aa0:	e002ed00 	.word	0xe002ed00

30005aa4 <BOOT_SCBVTORBackup_HP>:
30005aa4:	4a02      	ldr	r2, [pc, #8]	; (30005ab0 <BOOT_SCBVTORBackup_HP+0xc>)
30005aa6:	4b03      	ldr	r3, [pc, #12]	; (30005ab4 <BOOT_SCBVTORBackup_HP+0x10>)
30005aa8:	6892      	ldr	r2, [r2, #8]
30005aaa:	619a      	str	r2, [r3, #24]
30005aac:	4770      	bx	lr
30005aae:	bf00      	nop
30005ab0:	e000ed00 	.word	0xe000ed00
30005ab4:	3000ffa8 	.word	0x3000ffa8

30005ab8 <BOOT_SCBVTORReFill_HP>:
30005ab8:	4a02      	ldr	r2, [pc, #8]	; (30005ac4 <BOOT_SCBVTORReFill_HP+0xc>)
30005aba:	4b03      	ldr	r3, [pc, #12]	; (30005ac8 <BOOT_SCBVTORReFill_HP+0x10>)
30005abc:	6992      	ldr	r2, [r2, #24]
30005abe:	609a      	str	r2, [r3, #8]
30005ac0:	4770      	bx	lr
30005ac2:	bf00      	nop
30005ac4:	3000ffa8 	.word	0x3000ffa8
30005ac8:	e000ed00 	.word	0xe000ed00

30005acc <BOOT_NVICBackup_HP>:
30005acc:	491a      	ldr	r1, [pc, #104]	; (30005b38 <BOOT_NVICBackup_HP+0x6c>)
30005ace:	f06f 001a 	mvn.w	r0, #26
30005ad2:	b410      	push	{r4}
30005ad4:	4c19      	ldr	r4, [pc, #100]	; (30005b3c <BOOT_NVICBackup_HP+0x70>)
30005ad6:	680a      	ldr	r2, [r1, #0]
30005ad8:	f104 031b 	add.w	r3, r4, #27
30005adc:	1b00      	subs	r0, r0, r4
30005ade:	6022      	str	r2, [r4, #0]
30005ae0:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
30005ae4:	684a      	ldr	r2, [r1, #4]
30005ae6:	6062      	str	r2, [r4, #4]
30005ae8:	688a      	ldr	r2, [r1, #8]
30005aea:	60a2      	str	r2, [r4, #8]
30005aec:	18c2      	adds	r2, r0, r3
30005aee:	440a      	add	r2, r1
30005af0:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
30005af4:	f803 2f01 	strb.w	r2, [r3, #1]!
30005af8:	4563      	cmp	r3, ip
30005afa:	d1f7      	bne.n	30005aec <BOOT_NVICBackup_HP+0x20>
30005afc:	4b10      	ldr	r3, [pc, #64]	; (30005b40 <BOOT_NVICBackup_HP+0x74>)
30005afe:	695b      	ldr	r3, [r3, #20]
30005b00:	03db      	lsls	r3, r3, #15
30005b02:	d512      	bpl.n	30005b2a <BOOT_NVICBackup_HP+0x5e>
30005b04:	f014 011f 	ands.w	r1, r4, #31
30005b08:	4b0c      	ldr	r3, [pc, #48]	; (30005b3c <BOOT_NVICBackup_HP+0x70>)
30005b0a:	d111      	bne.n	30005b30 <BOOT_NVICBackup_HP+0x64>
30005b0c:	216c      	movs	r1, #108	; 0x6c
30005b0e:	f3bf 8f4f 	dsb	sy
30005b12:	480b      	ldr	r0, [pc, #44]	; (30005b40 <BOOT_NVICBackup_HP+0x74>)
30005b14:	4419      	add	r1, r3
30005b16:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005b1a:	3320      	adds	r3, #32
30005b1c:	1aca      	subs	r2, r1, r3
30005b1e:	2a00      	cmp	r2, #0
30005b20:	dcf9      	bgt.n	30005b16 <BOOT_NVICBackup_HP+0x4a>
30005b22:	f3bf 8f4f 	dsb	sy
30005b26:	f3bf 8f6f 	isb	sy
30005b2a:	f85d 4b04 	ldr.w	r4, [sp], #4
30005b2e:	4770      	bx	lr
30005b30:	f024 031f 	bic.w	r3, r4, #31
30005b34:	316c      	adds	r1, #108	; 0x6c
30005b36:	e7ea      	b.n	30005b0e <BOOT_NVICBackup_HP+0x42>
30005b38:	e000e100 	.word	0xe000e100
30005b3c:	3000ffa8 	.word	0x3000ffa8
30005b40:	e000ed00 	.word	0xe000ed00

30005b44 <BOOT_NVICReFill_HP>:
30005b44:	b510      	push	{r4, lr}
30005b46:	f06f 0c1a 	mvn.w	ip, #26
30005b4a:	4c0d      	ldr	r4, [pc, #52]	; (30005b80 <BOOT_NVICReFill_HP+0x3c>)
30005b4c:	480d      	ldr	r0, [pc, #52]	; (30005b84 <BOOT_NVICReFill_HP+0x40>)
30005b4e:	f104 031b 	add.w	r3, r4, #27
30005b52:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30005b56:	ebac 0c04 	sub.w	ip, ip, r4
30005b5a:	eb0c 0203 	add.w	r2, ip, r3
30005b5e:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30005b62:	4402      	add	r2, r0
30005b64:	4573      	cmp	r3, lr
30005b66:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30005b6a:	d1f6      	bne.n	30005b5a <BOOT_NVICReFill_HP+0x16>
30005b6c:	e9d4 3100 	ldrd	r3, r1, [r4]
30005b70:	68a2      	ldr	r2, [r4, #8]
30005b72:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30005b76:	6003      	str	r3, [r0, #0]
30005b78:	6041      	str	r1, [r0, #4]
30005b7a:	6082      	str	r2, [r0, #8]
30005b7c:	bd10      	pop	{r4, pc}
30005b7e:	bf00      	nop
30005b80:	3000ffa8 	.word	0x3000ffa8
30005b84:	e000e100 	.word	0xe000e100

30005b88 <BOOT_ImgCopy>:
30005b88:	b510      	push	{r4, lr}
30005b8a:	b082      	sub	sp, #8
30005b8c:	4b09      	ldr	r3, [pc, #36]	; (30005bb4 <BOOT_ImgCopy+0x2c>)
30005b8e:	4604      	mov	r4, r0
30005b90:	e9cd 2100 	strd	r2, r1, [sp]
30005b94:	4798      	blx	r3
30005b96:	e9dd 2100 	ldrd	r2, r1, [sp]
30005b9a:	b128      	cbz	r0, 30005ba8 <BOOT_ImgCopy+0x20>
30005b9c:	4620      	mov	r0, r4
30005b9e:	4b06      	ldr	r3, [pc, #24]	; (30005bb8 <BOOT_ImgCopy+0x30>)
30005ba0:	b002      	add	sp, #8
30005ba2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005ba6:	4718      	bx	r3
30005ba8:	4620      	mov	r0, r4
30005baa:	b002      	add	sp, #8
30005bac:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30005bb0:	f002 be40 	b.w	30008834 <NandImgCopy>
30005bb4:	30009b61 	.word	0x30009b61
30005bb8:	00012a1d 	.word	0x00012a1d

30005bbc <BOOT_LoadImages>:
30005bbc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005bc0:	4b57      	ldr	r3, [pc, #348]	; (30005d20 <BOOT_LoadImages+0x164>)
30005bc2:	2700      	movs	r7, #0
30005bc4:	2104      	movs	r1, #4
30005bc6:	2601      	movs	r6, #1
30005bc8:	ed2d 8b02 	vpush	{d8}
30005bcc:	b0b7      	sub	sp, #220	; 0xdc
30005bce:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30005bd2:	4b54      	ldr	r3, [pc, #336]	; (30005d24 <BOOT_LoadImages+0x168>)
30005bd4:	aa11      	add	r2, sp, #68	; 0x44
30005bd6:	9703      	str	r7, [sp, #12]
30005bd8:	9705      	str	r7, [sp, #20]
30005bda:	4798      	blx	r3
30005bdc:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
30005be0:	409e      	lsls	r6, r3
30005be2:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
30005be6:	f000 823a 	beq.w	3000605e <BOOT_LoadImages+0x4a2>
30005bea:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
30005bee:	f000 8375 	beq.w	300062dc <BOOT_LoadImages+0x720>
30005bf2:	4c4d      	ldr	r4, [pc, #308]	; (30005d28 <BOOT_LoadImages+0x16c>)
30005bf4:	f04f 0800 	mov.w	r8, #0
30005bf8:	4d4c      	ldr	r5, [pc, #304]	; (30005d2c <BOOT_LoadImages+0x170>)
30005bfa:	eb06 0904 	add.w	r9, r6, r4
30005bfe:	484c      	ldr	r0, [pc, #304]	; (30005d30 <BOOT_LoadImages+0x174>)
30005c00:	4435      	add	r5, r6
30005c02:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
30005c06:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
30005c0a:	f7ff f9ff 	bl	3000500c <mcc_init_crc32>
30005c0e:	f8d9 3004 	ldr.w	r3, [r9, #4]
30005c12:	f8d5 a008 	ldr.w	sl, [r5, #8]
30005c16:	f899 200c 	ldrb.w	r2, [r9, #12]
30005c1a:	9308      	str	r3, [sp, #32]
30005c1c:	3b01      	subs	r3, #1
30005c1e:	9206      	str	r2, [sp, #24]
30005c20:	3303      	adds	r3, #3
30005c22:	686a      	ldr	r2, [r5, #4]
30005c24:	f895 b00c 	ldrb.w	fp, [r5, #12]
30005c28:	9204      	str	r2, [sp, #16]
30005c2a:	f200 81bc 	bhi.w	30005fa6 <BOOT_LoadImages+0x3ea>
30005c2e:	f8d9 2008 	ldr.w	r2, [r9, #8]
30005c32:	9b04      	ldr	r3, [sp, #16]
30005c34:	3a01      	subs	r2, #1
30005c36:	3b01      	subs	r3, #1
30005c38:	3203      	adds	r2, #3
30005c3a:	d868      	bhi.n	30005d0e <BOOT_LoadImages+0x152>
30005c3c:	9a06      	ldr	r2, [sp, #24]
30005c3e:	9307      	str	r3, [sp, #28]
30005c40:	2a01      	cmp	r2, #1
30005c42:	f240 8305 	bls.w	30006250 <BOOT_LoadImages+0x694>
30005c46:	483b      	ldr	r0, [pc, #236]	; (30005d34 <BOOT_LoadImages+0x178>)
30005c48:	f006 fb9e 	bl	3000c388 <__DiagPrintf_veneer>
30005c4c:	9b07      	ldr	r3, [sp, #28]
30005c4e:	3303      	adds	r3, #3
30005c50:	d864      	bhi.n	30005d1c <BOOT_LoadImages+0x160>
30005c52:	f10a 3aff 	add.w	sl, sl, #4294967295
30005c56:	f11a 0f03 	cmn.w	sl, #3
30005c5a:	f200 81aa 	bhi.w	30005fb2 <BOOT_LoadImages+0x3f6>
30005c5e:	f1bb 0f01 	cmp.w	fp, #1
30005c62:	f200 8347 	bhi.w	300062f4 <BOOT_LoadImages+0x738>
30005c66:	9904      	ldr	r1, [sp, #16]
30005c68:	4833      	ldr	r0, [pc, #204]	; (30005d38 <BOOT_LoadImages+0x17c>)
30005c6a:	f006 fb8d 	bl	3000c388 <__DiagPrintf_veneer>
30005c6e:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005c72:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005c76:	4c2e      	ldr	r4, [pc, #184]	; (30005d30 <BOOT_LoadImages+0x174>)
30005c78:	9307      	str	r3, [sp, #28]
30005c7a:	2300      	movs	r3, #0
30005c7c:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005c80:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005c84:	9309      	str	r3, [sp, #36]	; 0x24
30005c86:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30005d50 <BOOT_LoadImages+0x194>
30005c8a:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30005c8e:	6829      	ldr	r1, [r5, #0]
30005c90:	f04f 30ff 	mov.w	r0, #4294967295
30005c94:	44b4      	add	ip, r6
30005c96:	f81c 2b01 	ldrb.w	r2, [ip], #1
30005c9a:	4042      	eors	r2, r0
30005c9c:	459c      	cmp	ip, r3
30005c9e:	b2d2      	uxtb	r2, r2
30005ca0:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30005ca4:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30005ca8:	d1f5      	bne.n	30005c96 <BOOT_LoadImages+0xda>
30005caa:	43c2      	mvns	r2, r0
30005cac:	930f      	str	r3, [sp, #60]	; 0x3c
30005cae:	4291      	cmp	r1, r2
30005cb0:	f000 8301 	beq.w	300062b6 <BOOT_LoadImages+0x6fa>
30005cb4:	4821      	ldr	r0, [pc, #132]	; (30005d3c <BOOT_LoadImages+0x180>)
30005cb6:	f006 fb67 	bl	3000c388 <__DiagPrintf_veneer>
30005cba:	2200      	movs	r2, #0
30005cbc:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005cbe:	9909      	ldr	r1, [sp, #36]	; 0x24
30005cc0:	2900      	cmp	r1, #0
30005cc2:	d04e      	beq.n	30005d62 <BOOT_LoadImages+0x1a6>
30005cc4:	f899 100c 	ldrb.w	r1, [r9, #12]
30005cc8:	4553      	cmp	r3, sl
30005cca:	9106      	str	r1, [sp, #24]
30005ccc:	d342      	bcc.n	30005d54 <BOOT_LoadImages+0x198>
30005cce:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30005cd2:	45e2      	cmp	sl, ip
30005cd4:	d93e      	bls.n	30005d54 <BOOT_LoadImages+0x198>
30005cd6:	9807      	ldr	r0, [sp, #28]
30005cd8:	4560      	cmp	r0, ip
30005cda:	f240 8203 	bls.w	300060e4 <BOOT_LoadImages+0x528>
30005cde:	4283      	cmp	r3, r0
30005ce0:	f0c0 8200 	bcc.w	300060e4 <BOOT_LoadImages+0x528>
30005ce4:	2a00      	cmp	r2, #0
30005ce6:	f000 820d 	beq.w	30006104 <BOOT_LoadImages+0x548>
30005cea:	455b      	cmp	r3, fp
30005cec:	d301      	bcc.n	30005cf2 <BOOT_LoadImages+0x136>
30005cee:	45e3      	cmp	fp, ip
30005cf0:	d845      	bhi.n	30005d7e <BOOT_LoadImages+0x1c2>
30005cf2:	4813      	ldr	r0, [pc, #76]	; (30005d40 <BOOT_LoadImages+0x184>)
30005cf4:	f006 fb48 	bl	3000c388 <__DiagPrintf_veneer>
30005cf8:	9f06      	ldr	r7, [sp, #24]
30005cfa:	4812      	ldr	r0, [pc, #72]	; (30005d44 <BOOT_LoadImages+0x188>)
30005cfc:	464d      	mov	r5, r9
30005cfe:	f006 fb43 	bl	3000c388 <__DiagPrintf_veneer>
30005d02:	9b07      	ldr	r3, [sp, #28]
30005d04:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30005d08:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30005d0c:	e049      	b.n	30005da2 <BOOT_LoadImages+0x1e6>
30005d0e:	480e      	ldr	r0, [pc, #56]	; (30005d48 <BOOT_LoadImages+0x18c>)
30005d10:	9307      	str	r3, [sp, #28]
30005d12:	f006 fb39 	bl	3000c388 <__DiagPrintf_veneer>
30005d16:	9b07      	ldr	r3, [sp, #28]
30005d18:	3303      	adds	r3, #3
30005d1a:	d99a      	bls.n	30005c52 <BOOT_LoadImages+0x96>
30005d1c:	480b      	ldr	r0, [pc, #44]	; (30005d4c <BOOT_LoadImages+0x190>)
30005d1e:	e149      	b.n	30005fb4 <BOOT_LoadImages+0x3f8>
30005d20:	2001c01c 	.word	0x2001c01c
30005d24:	00009db5 	.word	0x00009db5
30005d28:	07ffe000 	.word	0x07ffe000
30005d2c:	07fff000 	.word	0x07fff000
30005d30:	3000e7e8 	.word	0x3000e7e8
30005d34:	3000ca9c 	.word	0x3000ca9c
30005d38:	3000ce84 	.word	0x3000ce84
30005d3c:	3000cba0 	.word	0x3000cba0
30005d40:	3000ce68 	.word	0x3000ce68
30005d44:	3000cc2c 	.word	0x3000cc2c
30005d48:	3000ca88 	.word	0x3000ca88
30005d4c:	3000ce34 	.word	0x3000ce34
30005d50:	07fff004 	.word	0x07fff004
30005d54:	48c4      	ldr	r0, [pc, #784]	; (30006068 <BOOT_LoadImages+0x4ac>)
30005d56:	930f      	str	r3, [sp, #60]	; 0x3c
30005d58:	9209      	str	r2, [sp, #36]	; 0x24
30005d5a:	f006 fb15 	bl	3000c388 <__DiagPrintf_veneer>
30005d5e:	9a09      	ldr	r2, [sp, #36]	; 0x24
30005d60:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30005d62:	2a00      	cmp	r2, #0
30005d64:	f000 8131 	beq.w	30005fca <BOOT_LoadImages+0x40e>
30005d68:	455b      	cmp	r3, fp
30005d6a:	f0c0 82c5 	bcc.w	300062f8 <BOOT_LoadImages+0x73c>
30005d6e:	f1bb 6f00 	cmp.w	fp, #134217728	; 0x8000000
30005d72:	bf2c      	ite	cs
30005d74:	2200      	movcs	r2, #0
30005d76:	2201      	movcc	r2, #1
30005d78:	2a00      	cmp	r2, #0
30005d7a:	f040 82bd 	bne.w	300062f8 <BOOT_LoadImages+0x73c>
30005d7e:	4598      	cmp	r8, r3
30005d80:	f200 81b8 	bhi.w	300060f4 <BOOT_LoadImages+0x538>
30005d84:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30005d88:	f0c0 81b4 	bcc.w	300060f4 <BOOT_LoadImages+0x538>
30005d8c:	7b2f      	ldrb	r7, [r5, #12]
30005d8e:	2a00      	cmp	r2, #0
30005d90:	f040 81de 	bne.w	30006150 <BOOT_LoadImages+0x594>
30005d94:	48b5      	ldr	r0, [pc, #724]	; (3000606c <BOOT_LoadImages+0x4b0>)
30005d96:	f006 faf7 	bl	3000c388 <__DiagPrintf_veneer>
30005d9a:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30005d9e:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30005da2:	4639      	mov	r1, r7
30005da4:	48b2      	ldr	r0, [pc, #712]	; (30006070 <BOOT_LoadImages+0x4b4>)
30005da6:	f006 faef 	bl	3000c388 <__DiagPrintf_veneer>
30005daa:	4bb2      	ldr	r3, [pc, #712]	; (30006074 <BOOT_LoadImages+0x4b8>)
30005dac:	4798      	blx	r3
30005dae:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005db2:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30005db6:	f8d1 300a 	ldr.w	r3, [r1, #10]
30005dba:	6809      	ldr	r1, [r1, #0]
30005dbc:	330c      	adds	r3, #12
30005dbe:	910b      	str	r1, [sp, #44]	; 0x2c
30005dc0:	42b3      	cmp	r3, r6
30005dc2:	bf28      	it	cs
30005dc4:	4633      	movcs	r3, r6
30005dc6:	930d      	str	r3, [sp, #52]	; 0x34
30005dc8:	f8d2 300a 	ldr.w	r3, [r2, #10]
30005dcc:	6812      	ldr	r2, [r2, #0]
30005dce:	330c      	adds	r3, #12
30005dd0:	920a      	str	r2, [sp, #40]	; 0x28
30005dd2:	429e      	cmp	r6, r3
30005dd4:	bf28      	it	cs
30005dd6:	461e      	movcs	r6, r3
30005dd8:	960c      	str	r6, [sp, #48]	; 0x30
30005dda:	2800      	cmp	r0, #0
30005ddc:	f040 8194 	bne.w	30006108 <BOOT_LoadImages+0x54c>
30005de0:	4ba5      	ldr	r3, [pc, #660]	; (30006078 <BOOT_LoadImages+0x4bc>)
30005de2:	4606      	mov	r6, r0
30005de4:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 300060a4 <BOOT_LoadImages+0x4e8>
30005de8:	4680      	mov	r8, r0
30005dea:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 300060a8 <BOOT_LoadImages+0x4ec>
30005dee:	4682      	mov	sl, r0
30005df0:	e9cd 0303 	strd	r0, r3, [sp, #12]
30005df4:	2f00      	cmp	r7, #0
30005df6:	f040 81c9 	bne.w	3000618c <BOOT_LoadImages+0x5d0>
30005dfa:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30005dfe:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005e00:	f10c 0104 	add.w	r1, ip, #4
30005e04:	2b00      	cmp	r3, #0
30005e06:	f000 83d4 	beq.w	300065b2 <BOOT_LoadImages+0x9f6>
30005e0a:	1d18      	adds	r0, r3, #4
30005e0c:	f04f 32ff 	mov.w	r2, #4294967295
30005e10:	4460      	add	r0, ip
30005e12:	f811 3b01 	ldrb.w	r3, [r1], #1
30005e16:	4053      	eors	r3, r2
30005e18:	4281      	cmp	r1, r0
30005e1a:	b2db      	uxtb	r3, r3
30005e1c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005e20:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005e24:	d1f5      	bne.n	30005e12 <BOOT_LoadImages+0x256>
30005e26:	43d2      	mvns	r2, r2
30005e28:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30005e2a:	4293      	cmp	r3, r2
30005e2c:	f000 838d 	beq.w	3000654a <BOOT_LoadImages+0x98e>
30005e30:	f8dc 100a 	ldr.w	r1, [ip, #10]
30005e34:	f088 0301 	eor.w	r3, r8, #1
30005e38:	3101      	adds	r1, #1
30005e3a:	f003 0301 	and.w	r3, r3, #1
30005e3e:	ea46 0603 	orr.w	r6, r6, r3
30005e42:	f000 8345 	beq.w	300064d0 <BOOT_LoadImages+0x914>
30005e46:	990b      	ldr	r1, [sp, #44]	; 0x2c
30005e48:	488c      	ldr	r0, [pc, #560]	; (3000607c <BOOT_LoadImages+0x4c0>)
30005e4a:	f006 fa9d 	bl	3000c388 <__DiagPrintf_veneer>
30005e4e:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30005e52:	2002      	movs	r0, #2
30005e54:	f7ff f858 	bl	30004f08 <FLASH_Erase_With_Lock>
30005e58:	2e00      	cmp	r6, #0
30005e5a:	f040 831f 	bne.w	3000649c <BOOT_LoadImages+0x8e0>
30005e5e:	9605      	str	r6, [sp, #20]
30005e60:	f08a 0601 	eor.w	r6, sl, #1
30005e64:	f04f 0801 	mov.w	r8, #1
30005e68:	f006 0601 	and.w	r6, r6, #1
30005e6c:	9b03      	ldr	r3, [sp, #12]
30005e6e:	2b01      	cmp	r3, #1
30005e70:	f000 8238 	beq.w	300062e4 <BOOT_LoadImages+0x728>
30005e74:	9b03      	ldr	r3, [sp, #12]
30005e76:	2b02      	cmp	r3, #2
30005e78:	f000 83a9 	beq.w	300065ce <BOOT_LoadImages+0xa12>
30005e7c:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30005e7e:	4650      	mov	r0, sl
30005e80:	f8c9 3000 	str.w	r3, [r9]
30005e84:	47d8      	blx	fp
30005e86:	1e03      	subs	r3, r0, #0
30005e88:	bf18      	it	ne
30005e8a:	2301      	movne	r3, #1
30005e8c:	2800      	cmp	r0, #0
30005e8e:	f000 8156 	beq.w	3000613e <BOOT_LoadImages+0x582>
30005e92:	2e00      	cmp	r6, #0
30005e94:	f000 8153 	beq.w	3000613e <BOOT_LoadImages+0x582>
30005e98:	fab7 f787 	clz	r7, r7
30005e9c:	9b05      	ldr	r3, [sp, #20]
30005e9e:	097f      	lsrs	r7, r7, #5
30005ea0:	2b00      	cmp	r3, #0
30005ea2:	f000 83e1 	beq.w	30006668 <BOOT_LoadImages+0xaac>
30005ea6:	469a      	mov	sl, r3
30005ea8:	4c75      	ldr	r4, [pc, #468]	; (30006080 <BOOT_LoadImages+0x4c4>)
30005eaa:	2f00      	cmp	r7, #0
30005eac:	f040 8138 	bne.w	30006120 <BOOT_LoadImages+0x564>
30005eb0:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005eb4:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005eb8:	3201      	adds	r2, #1
30005eba:	f000 82c5 	beq.w	30006448 <BOOT_LoadImages+0x88c>
30005ebe:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005ec2:	4870      	ldr	r0, [pc, #448]	; (30006084 <BOOT_LoadImages+0x4c8>)
30005ec4:	f006 fa60 	bl	3000c388 <__DiagPrintf_veneer>
30005ec8:	2301      	movs	r3, #1
30005eca:	9303      	str	r3, [sp, #12]
30005ecc:	f04f 0801 	mov.w	r8, #1
30005ed0:	4646      	mov	r6, r8
30005ed2:	f8cd 8014 	str.w	r8, [sp, #20]
30005ed6:	f1ba 0f00 	cmp.w	sl, #0
30005eda:	d0c7      	beq.n	30005e6c <BOOT_LoadImages+0x2b0>
30005edc:	ab16      	add	r3, sp, #88	; 0x58
30005ede:	2280      	movs	r2, #128	; 0x80
30005ee0:	21ff      	movs	r1, #255	; 0xff
30005ee2:	46d0      	mov	r8, sl
30005ee4:	461e      	mov	r6, r3
30005ee6:	4618      	mov	r0, r3
30005ee8:	ee08 3a10 	vmov	s16, r3
30005eec:	f006 fa24 	bl	3000c338 <____wrap_memset_veneer>
30005ef0:	2280      	movs	r2, #128	; 0x80
30005ef2:	4629      	mov	r1, r5
30005ef4:	4630      	mov	r0, r6
30005ef6:	f006 fa07 	bl	3000c308 <____wrap_memcpy_veneer>
30005efa:	2f01      	cmp	r7, #1
30005efc:	f000 831e 	beq.w	3000653c <BOOT_LoadImages+0x980>
30005f00:	2100      	movs	r1, #0
30005f02:	f8cd a014 	str.w	sl, [sp, #20]
30005f06:	f04f 0a01 	mov.w	sl, #1
30005f0a:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30005f0e:	1d2e      	adds	r6, r5, #4
30005f10:	485d      	ldr	r0, [pc, #372]	; (30006088 <BOOT_LoadImages+0x4cc>)
30005f12:	f006 fa39 	bl	3000c388 <__DiagPrintf_veneer>
30005f16:	4629      	mov	r1, r5
30005f18:	2002      	movs	r0, #2
30005f1a:	f7fe fff5 	bl	30004f08 <FLASH_Erase_With_Lock>
30005f1e:	aa17      	add	r2, sp, #92	; 0x5c
30005f20:	217c      	movs	r1, #124	; 0x7c
30005f22:	4630      	mov	r0, r6
30005f24:	f7ff f818 	bl	30004f58 <FLASH_TxData_With_Lock>
30005f28:	4630      	mov	r0, r6
30005f2a:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30005f2e:	f04f 32ff 	mov.w	r2, #4294967295
30005f32:	f810 3b01 	ldrb.w	r3, [r0], #1
30005f36:	4053      	eors	r3, r2
30005f38:	4288      	cmp	r0, r1
30005f3a:	b2db      	uxtb	r3, r3
30005f3c:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005f40:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005f44:	d1f5      	bne.n	30005f32 <BOOT_LoadImages+0x376>
30005f46:	43d2      	mvns	r2, r2
30005f48:	4629      	mov	r1, r5
30005f4a:	2002      	movs	r0, #2
30005f4c:	9216      	str	r2, [sp, #88]	; 0x58
30005f4e:	f7fe ffdb 	bl	30004f08 <FLASH_Erase_With_Lock>
30005f52:	ee18 2a10 	vmov	r2, s16
30005f56:	2180      	movs	r1, #128	; 0x80
30005f58:	4628      	mov	r0, r5
30005f5a:	f7fe fffd 	bl	30004f58 <FLASH_TxData_With_Lock>
30005f5e:	9b05      	ldr	r3, [sp, #20]
30005f60:	2b00      	cmp	r3, #0
30005f62:	f000 8143 	beq.w	300061ec <BOOT_LoadImages+0x630>
30005f66:	f1b8 0f00 	cmp.w	r8, #0
30005f6a:	f040 8376 	bne.w	3000665a <BOOT_LoadImages+0xa9e>
30005f6e:	2f01      	cmp	r7, #1
30005f70:	f000 8280 	beq.w	30006474 <BOOT_LoadImages+0x8b8>
30005f74:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005f78:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005f7c:	3201      	adds	r2, #1
30005f7e:	f000 81d0 	beq.w	30006322 <BOOT_LoadImages+0x766>
30005f82:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005f86:	4841      	ldr	r0, [pc, #260]	; (3000608c <BOOT_LoadImages+0x4d0>)
30005f88:	f006 f9fe 	bl	3000c388 <__DiagPrintf_veneer>
30005f8c:	f08a 0601 	eor.w	r6, sl, #1
30005f90:	f04f 0801 	mov.w	r8, #1
30005f94:	f006 0601 	and.w	r6, r6, #1
30005f98:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005f9c:	3310      	adds	r3, #16
30005f9e:	930e      	str	r3, [sp, #56]	; 0x38
30005fa0:	2302      	movs	r3, #2
30005fa2:	9303      	str	r3, [sp, #12]
30005fa4:	e76a      	b.n	30005e7c <BOOT_LoadImages+0x2c0>
30005fa6:	483a      	ldr	r0, [pc, #232]	; (30006090 <BOOT_LoadImages+0x4d4>)
30005fa8:	f006 f9ee 	bl	3000c388 <__DiagPrintf_veneer>
30005fac:	9b04      	ldr	r3, [sp, #16]
30005fae:	3b01      	subs	r3, #1
30005fb0:	e64d      	b.n	30005c4e <BOOT_LoadImages+0x92>
30005fb2:	4838      	ldr	r0, [pc, #224]	; (30006094 <BOOT_LoadImages+0x4d8>)
30005fb4:	f006 f9e8 	bl	3000c388 <__DiagPrintf_veneer>
30005fb8:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005fbc:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30005fc0:	9307      	str	r3, [sp, #28]
30005fc2:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30005fc6:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005fca:	4833      	ldr	r0, [pc, #204]	; (30006098 <BOOT_LoadImages+0x4dc>)
30005fcc:	f006 f9dc 	bl	3000c388 <__DiagPrintf_veneer>
30005fd0:	4832      	ldr	r0, [pc, #200]	; (3000609c <BOOT_LoadImages+0x4e0>)
30005fd2:	f006 f9d9 	bl	3000c388 <__DiagPrintf_veneer>
30005fd6:	4631      	mov	r1, r6
30005fd8:	aa12      	add	r2, sp, #72	; 0x48
30005fda:	4650      	mov	r0, sl
30005fdc:	f7ff f85c 	bl	30005098 <recovery_check>
30005fe0:	4631      	mov	r1, r6
30005fe2:	4604      	mov	r4, r0
30005fe4:	aa13      	add	r2, sp, #76	; 0x4c
30005fe6:	9807      	ldr	r0, [sp, #28]
30005fe8:	f7ff f856 	bl	30005098 <recovery_check>
30005fec:	4631      	mov	r1, r6
30005fee:	aa14      	add	r2, sp, #80	; 0x50
30005ff0:	9004      	str	r0, [sp, #16]
30005ff2:	4658      	mov	r0, fp
30005ff4:	f7ff f850 	bl	30005098 <recovery_check>
30005ff8:	4631      	mov	r1, r6
30005ffa:	aa15      	add	r2, sp, #84	; 0x54
30005ffc:	4606      	mov	r6, r0
30005ffe:	4640      	mov	r0, r8
30006000:	f7ff f84a 	bl	30005098 <recovery_check>
30006004:	9b04      	ldr	r3, [sp, #16]
30006006:	4306      	orrs	r6, r0
30006008:	431c      	orrs	r4, r3
3000600a:	d04f      	beq.n	300060ac <BOOT_LoadImages+0x4f0>
3000600c:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30006010:	2e00      	cmp	r6, #0
30006012:	f000 818a 	beq.w	3000632a <BOOT_LoadImages+0x76e>
30006016:	4293      	cmp	r3, r2
30006018:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
3000601c:	f0c0 8273 	bcc.w	30006506 <BOOT_LoadImages+0x94a>
30006020:	428b      	cmp	r3, r1
30006022:	d302      	bcc.n	3000602a <BOOT_LoadImages+0x46e>
30006024:	4283      	cmp	r3, r0
30006026:	f080 82fb 	bcs.w	30006620 <BOOT_LoadImages+0xa64>
3000602a:	4293      	cmp	r3, r2
3000602c:	f000 826b 	beq.w	30006506 <BOOT_LoadImages+0x94a>
30006030:	4293      	cmp	r3, r2
30006032:	bf38      	it	cc
30006034:	4613      	movcc	r3, r2
30006036:	428b      	cmp	r3, r1
30006038:	f240 82dc 	bls.w	300065f4 <BOOT_LoadImages+0xa38>
3000603c:	428b      	cmp	r3, r1
3000603e:	bf38      	it	cc
30006040:	460b      	movcc	r3, r1
30006042:	4283      	cmp	r3, r0
30006044:	f240 82e5 	bls.w	30006612 <BOOT_LoadImages+0xa56>
30006048:	f04f 0800 	mov.w	r8, #0
3000604c:	4b0a      	ldr	r3, [pc, #40]	; (30006078 <BOOT_LoadImages+0x4bc>)
3000604e:	2601      	movs	r6, #1
30006050:	f8df 9050 	ldr.w	r9, [pc, #80]	; 300060a4 <BOOT_LoadImages+0x4e8>
30006054:	f8df b050 	ldr.w	fp, [pc, #80]	; 300060a8 <BOOT_LoadImages+0x4ec>
30006058:	46c2      	mov	sl, r8
3000605a:	9304      	str	r3, [sp, #16]
3000605c:	e70e      	b.n	30005e7c <BOOT_LoadImages+0x2c0>
3000605e:	4810      	ldr	r0, [pc, #64]	; (300060a0 <BOOT_LoadImages+0x4e4>)
30006060:	f006 f992 	bl	3000c388 <__DiagPrintf_veneer>
30006064:	e5c5      	b.n	30005bf2 <BOOT_LoadImages+0x36>
30006066:	bf00      	nop
30006068:	3000cbe0 	.word	0x3000cbe0
3000606c:	3000cc3c 	.word	0x3000cc3c
30006070:	3000cc70 	.word	0x3000cc70
30006074:	0000c149 	.word	0x0000c149
30006078:	3000e623 	.word	0x3000e623
3000607c:	3000cce0 	.word	0x3000cce0
30006080:	3000e7e8 	.word	0x3000e7e8
30006084:	3000c9d4 	.word	0x3000c9d4
30006088:	3000cdb4 	.word	0x3000cdb4
3000608c:	3000c9ec 	.word	0x3000c9ec
30006090:	3000ca70 	.word	0x3000ca70
30006094:	3000cad4 	.word	0x3000cad4
30006098:	3000cc4c 	.word	0x3000cc4c
3000609c:	3000cc60 	.word	0x3000cc60
300060a0:	3000ca4c 	.word	0x3000ca4c
300060a4:	3000ffa4 	.word	0x3000ffa4
300060a8:	30004221 	.word	0x30004221
300060ac:	2e00      	cmp	r6, #0
300060ae:	f000 827c 	beq.w	300065aa <BOOT_LoadImages+0x9ee>
300060b2:	4659      	mov	r1, fp
300060b4:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
300060b8:	e9cd 5400 	strd	r5, r4, [sp]
300060bc:	429a      	cmp	r2, r3
300060be:	464b      	mov	r3, r9
300060c0:	4642      	mov	r2, r8
300060c2:	f080 821e 	bcs.w	30006502 <BOOT_LoadImages+0x946>
300060c6:	2001      	movs	r0, #1
300060c8:	f7ff f816 	bl	300050f8 <rewrite_bp>
300060cc:	46a0      	mov	r8, r4
300060ce:	b2c3      	uxtb	r3, r0
300060d0:	2601      	movs	r6, #1
300060d2:	46a2      	mov	sl, r4
300060d4:	f8df 9318 	ldr.w	r9, [pc, #792]	; 300063f0 <BOOT_LoadImages+0x834>
300060d8:	9303      	str	r3, [sp, #12]
300060da:	4bb0      	ldr	r3, [pc, #704]	; (3000639c <BOOT_LoadImages+0x7e0>)
300060dc:	f8df b314 	ldr.w	fp, [pc, #788]	; 300063f4 <BOOT_LoadImages+0x838>
300060e0:	9304      	str	r3, [sp, #16]
300060e2:	e6c3      	b.n	30005e6c <BOOT_LoadImages+0x2b0>
300060e4:	48ae      	ldr	r0, [pc, #696]	; (300063a0 <BOOT_LoadImages+0x7e4>)
300060e6:	930f      	str	r3, [sp, #60]	; 0x3c
300060e8:	9209      	str	r2, [sp, #36]	; 0x24
300060ea:	f006 f94d 	bl	3000c388 <__DiagPrintf_veneer>
300060ee:	9a09      	ldr	r2, [sp, #36]	; 0x24
300060f0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300060f2:	e636      	b.n	30005d62 <BOOT_LoadImages+0x1a6>
300060f4:	48ab      	ldr	r0, [pc, #684]	; (300063a4 <BOOT_LoadImages+0x7e8>)
300060f6:	9204      	str	r2, [sp, #16]
300060f8:	f006 f946 	bl	3000c388 <__DiagPrintf_veneer>
300060fc:	9a04      	ldr	r2, [sp, #16]
300060fe:	2a00      	cmp	r2, #0
30006100:	f43f af63 	beq.w	30005fca <BOOT_LoadImages+0x40e>
30006104:	9f06      	ldr	r7, [sp, #24]
30006106:	e5f8      	b.n	30005cfa <BOOT_LoadImages+0x13e>
30006108:	2300      	movs	r3, #0
3000610a:	4aa4      	ldr	r2, [pc, #656]	; (3000639c <BOOT_LoadImages+0x7e0>)
3000610c:	f8df 92e0 	ldr.w	r9, [pc, #736]	; 300063f0 <BOOT_LoadImages+0x834>
30006110:	f8df b2e0 	ldr.w	fp, [pc, #736]	; 300063f4 <BOOT_LoadImages+0x838>
30006114:	469a      	mov	sl, r3
30006116:	9303      	str	r3, [sp, #12]
30006118:	9204      	str	r2, [sp, #16]
3000611a:	2f00      	cmp	r7, #0
3000611c:	f43f aec8 	beq.w	30005eb0 <BOOT_LoadImages+0x2f4>
30006120:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30006124:	f8d3 200a 	ldr.w	r2, [r3, #10]
30006128:	3201      	adds	r2, #1
3000612a:	f000 8118 	beq.w	3000635e <BOOT_LoadImages+0x7a2>
3000612e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006132:	489d      	ldr	r0, [pc, #628]	; (300063a8 <BOOT_LoadImages+0x7ec>)
30006134:	f006 f928 	bl	3000c388 <__DiagPrintf_veneer>
30006138:	2302      	movs	r3, #2
3000613a:	9303      	str	r3, [sp, #12]
3000613c:	e6c6      	b.n	30005ecc <BOOT_LoadImages+0x310>
3000613e:	f00a 0401 	and.w	r4, sl, #1
30006142:	b1c3      	cbz	r3, 30006176 <BOOT_LoadImages+0x5ba>
30006144:	b1bc      	cbz	r4, 30006176 <BOOT_LoadImages+0x5ba>
30006146:	4c99      	ldr	r4, [pc, #612]	; (300063ac <BOOT_LoadImages+0x7f0>)
30006148:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000614c:	47a0      	blx	r4
3000614e:	e7fb      	b.n	30006148 <BOOT_LoadImages+0x58c>
30006150:	4897      	ldr	r0, [pc, #604]	; (300063b0 <BOOT_LoadImages+0x7f4>)
30006152:	f006 f919 	bl	3000c388 <__DiagPrintf_veneer>
30006156:	9b08      	ldr	r3, [sp, #32]
30006158:	9a04      	ldr	r2, [sp, #16]
3000615a:	4293      	cmp	r3, r2
3000615c:	f4ff ae1a 	bcc.w	30005d94 <BOOT_LoadImages+0x1d8>
30006160:	4894      	ldr	r0, [pc, #592]	; (300063b4 <BOOT_LoadImages+0x7f8>)
30006162:	464d      	mov	r5, r9
30006164:	f006 f910 	bl	3000c388 <__DiagPrintf_veneer>
30006168:	9b07      	ldr	r3, [sp, #28]
3000616a:	9f06      	ldr	r7, [sp, #24]
3000616c:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30006170:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30006174:	e615      	b.n	30005da2 <BOOT_LoadImages+0x1e6>
30006176:	9b04      	ldr	r3, [sp, #16]
30006178:	781b      	ldrb	r3, [r3, #0]
3000617a:	2b00      	cmp	r3, #0
3000617c:	f040 80c0 	bne.w	30006300 <BOOT_LoadImages+0x744>
30006180:	2001      	movs	r0, #1
30006182:	b037      	add	sp, #220	; 0xdc
30006184:	ecbd 8b02 	vpop	{d8}
30006188:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000618c:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30006190:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006192:	f10c 0104 	add.w	r1, ip, #4
30006196:	2b00      	cmp	r3, #0
30006198:	f000 820d 	beq.w	300065b6 <BOOT_LoadImages+0x9fa>
3000619c:	1d18      	adds	r0, r3, #4
3000619e:	f04f 32ff 	mov.w	r2, #4294967295
300061a2:	4460      	add	r0, ip
300061a4:	f811 3b01 	ldrb.w	r3, [r1], #1
300061a8:	4053      	eors	r3, r2
300061aa:	4281      	cmp	r1, r0
300061ac:	b2db      	uxtb	r3, r3
300061ae:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300061b2:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300061b6:	d1f5      	bne.n	300061a4 <BOOT_LoadImages+0x5e8>
300061b8:	43d2      	mvns	r2, r2
300061ba:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300061bc:	4293      	cmp	r3, r2
300061be:	f000 81d4 	beq.w	3000656a <BOOT_LoadImages+0x9ae>
300061c2:	f8dc 100a 	ldr.w	r1, [ip, #10]
300061c6:	f088 0301 	eor.w	r3, r8, #1
300061ca:	3101      	adds	r1, #1
300061cc:	f003 0301 	and.w	r3, r3, #1
300061d0:	ea46 0603 	orr.w	r6, r6, r3
300061d4:	f000 8178 	beq.w	300064c8 <BOOT_LoadImages+0x90c>
300061d8:	990a      	ldr	r1, [sp, #40]	; 0x28
300061da:	4877      	ldr	r0, [pc, #476]	; (300063b8 <BOOT_LoadImages+0x7fc>)
300061dc:	f006 f8d4 	bl	3000c388 <__DiagPrintf_veneer>
300061e0:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
300061e4:	2002      	movs	r0, #2
300061e6:	f7fe fe8f 	bl	30004f08 <FLASH_Erase_With_Lock>
300061ea:	e635      	b.n	30005e58 <BOOT_LoadImages+0x29c>
300061ec:	f1b8 0f00 	cmp.w	r8, #0
300061f0:	f040 822e 	bne.w	30006650 <BOOT_LoadImages+0xa94>
300061f4:	2f01      	cmp	r7, #1
300061f6:	f000 80ff 	beq.w	300063f8 <BOOT_LoadImages+0x83c>
300061fa:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
300061fe:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30006200:	1d31      	adds	r1, r6, #4
30006202:	2b00      	cmp	r3, #0
30006204:	f000 81e5 	beq.w	300065d2 <BOOT_LoadImages+0xa16>
30006208:	1d18      	adds	r0, r3, #4
3000620a:	f04f 32ff 	mov.w	r2, #4294967295
3000620e:	4430      	add	r0, r6
30006210:	f811 3b01 	ldrb.w	r3, [r1], #1
30006214:	4053      	eors	r3, r2
30006216:	4281      	cmp	r1, r0
30006218:	b2db      	uxtb	r3, r3
3000621a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000621e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006222:	d1f5      	bne.n	30006210 <BOOT_LoadImages+0x654>
30006224:	43d2      	mvns	r2, r2
30006226:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30006228:	4293      	cmp	r3, r2
3000622a:	f000 81af 	beq.w	3000658c <BOOT_LoadImages+0x9d0>
3000622e:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006232:	3301      	adds	r3, #1
30006234:	d075      	beq.n	30006322 <BOOT_LoadImages+0x766>
30006236:	990a      	ldr	r1, [sp, #40]	; 0x28
30006238:	485f      	ldr	r0, [pc, #380]	; (300063b8 <BOOT_LoadImages+0x7fc>)
3000623a:	f006 f8a5 	bl	3000c388 <__DiagPrintf_veneer>
3000623e:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30006242:	2002      	movs	r0, #2
30006244:	f7fe fe60 	bl	30004f08 <FLASH_Erase_With_Lock>
30006248:	485c      	ldr	r0, [pc, #368]	; (300063bc <BOOT_LoadImages+0x800>)
3000624a:	f006 f89d 	bl	3000c388 <__DiagPrintf_veneer>
3000624e:	e7fe      	b.n	3000624e <BOOT_LoadImages+0x692>
30006250:	9908      	ldr	r1, [sp, #32]
30006252:	485b      	ldr	r0, [pc, #364]	; (300063c0 <BOOT_LoadImages+0x804>)
30006254:	f006 f898 	bl	3000c388 <__DiagPrintf_veneer>
30006258:	9b07      	ldr	r3, [sp, #28]
3000625a:	3303      	adds	r3, #3
3000625c:	f240 81ad 	bls.w	300065ba <BOOT_LoadImages+0x9fe>
30006260:	4858      	ldr	r0, [pc, #352]	; (300063c4 <BOOT_LoadImages+0x808>)
30006262:	f006 f891 	bl	3000c388 <__DiagPrintf_veneer>
30006266:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
3000626a:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000626e:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006272:	9307      	str	r3, [sp, #28]
30006274:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30006278:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000627c:	4852      	ldr	r0, [pc, #328]	; (300063c8 <BOOT_LoadImages+0x80c>)
3000627e:	f04f 32ff 	mov.w	r2, #4294967295
30006282:	5931      	ldr	r1, [r6, r4]
30006284:	4430      	add	r0, r6
30006286:	4c51      	ldr	r4, [pc, #324]	; (300063cc <BOOT_LoadImages+0x810>)
30006288:	f810 3b01 	ldrb.w	r3, [r0], #1
3000628c:	4053      	eors	r3, r2
3000628e:	4285      	cmp	r5, r0
30006290:	b2db      	uxtb	r3, r3
30006292:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30006296:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000629a:	d1f5      	bne.n	30006288 <BOOT_LoadImages+0x6cc>
3000629c:	43d2      	mvns	r2, r2
3000629e:	4291      	cmp	r1, r2
300062a0:	d010      	beq.n	300062c4 <BOOT_LoadImages+0x708>
300062a2:	484b      	ldr	r0, [pc, #300]	; (300063d0 <BOOT_LoadImages+0x814>)
300062a4:	f006 f870 	bl	3000c388 <__DiagPrintf_veneer>
300062a8:	9b09      	ldr	r3, [sp, #36]	; 0x24
300062aa:	2b00      	cmp	r3, #0
300062ac:	f43f ae8d 	beq.w	30005fca <BOOT_LoadImages+0x40e>
300062b0:	2300      	movs	r3, #0
300062b2:	9309      	str	r3, [sp, #36]	; 0x24
300062b4:	e4e7      	b.n	30005c86 <BOOT_LoadImages+0xca>
300062b6:	460a      	mov	r2, r1
300062b8:	4846      	ldr	r0, [pc, #280]	; (300063d4 <BOOT_LoadImages+0x818>)
300062ba:	f006 f865 	bl	3000c388 <__DiagPrintf_veneer>
300062be:	2201      	movs	r2, #1
300062c0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300062c2:	e4fc      	b.n	30005cbe <BOOT_LoadImages+0x102>
300062c4:	460a      	mov	r2, r1
300062c6:	4844      	ldr	r0, [pc, #272]	; (300063d8 <BOOT_LoadImages+0x81c>)
300062c8:	f006 f85e 	bl	3000c388 <__DiagPrintf_veneer>
300062cc:	9b09      	ldr	r3, [sp, #36]	; 0x24
300062ce:	2b00      	cmp	r3, #0
300062d0:	f47f acd9 	bne.w	30005c86 <BOOT_LoadImages+0xca>
300062d4:	461a      	mov	r2, r3
300062d6:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300062da:	e4f3      	b.n	30005cc4 <BOOT_LoadImages+0x108>
300062dc:	483f      	ldr	r0, [pc, #252]	; (300063dc <BOOT_LoadImages+0x820>)
300062de:	f006 f853 	bl	3000c388 <__DiagPrintf_veneer>
300062e2:	e486      	b.n	30005bf2 <BOOT_LoadImages+0x36>
300062e4:	4c39      	ldr	r4, [pc, #228]	; (300063cc <BOOT_LoadImages+0x810>)
300062e6:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300062ea:	3310      	adds	r3, #16
300062ec:	930e      	str	r3, [sp, #56]	; 0x38
300062ee:	2301      	movs	r3, #1
300062f0:	9303      	str	r3, [sp, #12]
300062f2:	e5c3      	b.n	30005e7c <BOOT_LoadImages+0x2c0>
300062f4:	483a      	ldr	r0, [pc, #232]	; (300063e0 <BOOT_LoadImages+0x824>)
300062f6:	e65d      	b.n	30005fb4 <BOOT_LoadImages+0x3f8>
300062f8:	483a      	ldr	r0, [pc, #232]	; (300063e4 <BOOT_LoadImages+0x828>)
300062fa:	f006 f845 	bl	3000c388 <__DiagPrintf_veneer>
300062fe:	e664      	b.n	30005fca <BOOT_LoadImages+0x40e>
30006300:	4b39      	ldr	r3, [pc, #228]	; (300063e8 <BOOT_LoadImages+0x82c>)
30006302:	4650      	mov	r0, sl
30006304:	4798      	blx	r3
30006306:	1e03      	subs	r3, r0, #0
30006308:	bf18      	it	ne
3000630a:	2301      	movne	r3, #1
3000630c:	b110      	cbz	r0, 30006314 <BOOT_LoadImages+0x758>
3000630e:	2e00      	cmp	r6, #0
30006310:	f47f adc2 	bne.w	30005e98 <BOOT_LoadImages+0x2dc>
30006314:	2c00      	cmp	r4, #0
30006316:	f43f af33 	beq.w	30006180 <BOOT_LoadImages+0x5c4>
3000631a:	2b00      	cmp	r3, #0
3000631c:	f47f af13 	bne.w	30006146 <BOOT_LoadImages+0x58a>
30006320:	e72e      	b.n	30006180 <BOOT_LoadImages+0x5c4>
30006322:	4832      	ldr	r0, [pc, #200]	; (300063ec <BOOT_LoadImages+0x830>)
30006324:	f006 f830 	bl	3000c388 <__DiagPrintf_veneer>
30006328:	e78e      	b.n	30006248 <BOOT_LoadImages+0x68c>
3000632a:	4293      	cmp	r3, r2
3000632c:	f04f 0401 	mov.w	r4, #1
30006330:	464b      	mov	r3, r9
30006332:	4651      	mov	r1, sl
30006334:	bf28      	it	cs
30006336:	4630      	movcs	r0, r6
30006338:	9a07      	ldr	r2, [sp, #28]
3000633a:	bf38      	it	cc
3000633c:	4620      	movcc	r0, r4
3000633e:	9500      	str	r5, [sp, #0]
30006340:	9401      	str	r4, [sp, #4]
30006342:	46b0      	mov	r8, r6
30006344:	f7fe fed8 	bl	300050f8 <rewrite_bp>
30006348:	4626      	mov	r6, r4
3000634a:	b2c3      	uxtb	r3, r0
3000634c:	f8df 90a0 	ldr.w	r9, [pc, #160]	; 300063f0 <BOOT_LoadImages+0x834>
30006350:	46c2      	mov	sl, r8
30006352:	f8df b0a0 	ldr.w	fp, [pc, #160]	; 300063f4 <BOOT_LoadImages+0x838>
30006356:	9303      	str	r3, [sp, #12]
30006358:	4b10      	ldr	r3, [pc, #64]	; (3000639c <BOOT_LoadImages+0x7e0>)
3000635a:	9304      	str	r3, [sp, #16]
3000635c:	e586      	b.n	30005e6c <BOOT_LoadImages+0x2b0>
3000635e:	ab16      	add	r3, sp, #88	; 0x58
30006360:	4822      	ldr	r0, [pc, #136]	; (300063ec <BOOT_LoadImages+0x830>)
30006362:	461e      	mov	r6, r3
30006364:	ee08 3a10 	vmov	s16, r3
30006368:	f006 f80e 	bl	3000c388 <__DiagPrintf_veneer>
3000636c:	2280      	movs	r2, #128	; 0x80
3000636e:	21ff      	movs	r1, #255	; 0xff
30006370:	4630      	mov	r0, r6
30006372:	f005 ffe1 	bl	3000c338 <____wrap_memset_veneer>
30006376:	2280      	movs	r2, #128	; 0x80
30006378:	4629      	mov	r1, r5
3000637a:	4630      	mov	r0, r6
3000637c:	f005 ffc4 	bl	3000c308 <____wrap_memcpy_veneer>
30006380:	f1ba 0f00 	cmp.w	sl, #0
30006384:	f040 80a8 	bne.w	300064d8 <BOOT_LoadImages+0x91c>
30006388:	2301      	movs	r3, #1
3000638a:	9305      	str	r3, [sp, #20]
3000638c:	46d0      	mov	r8, sl
3000638e:	f04f 0a00 	mov.w	sl, #0
30006392:	4651      	mov	r1, sl
30006394:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
30006398:	e5b9      	b.n	30005f0e <BOOT_LoadImages+0x352>
3000639a:	bf00      	nop
3000639c:	3000e623 	.word	0x3000e623
300063a0:	3000cbfc 	.word	0x3000cbfc
300063a4:	3000ce18 	.word	0x3000ce18
300063a8:	3000c9ec 	.word	0x3000c9ec
300063ac:	00009be5 	.word	0x00009be5
300063b0:	3000cc18 	.word	0x3000cc18
300063b4:	3000cc2c 	.word	0x3000cc2c
300063b8:	3000cd68 	.word	0x3000cd68
300063bc:	3000ca04 	.word	0x3000ca04
300063c0:	3000cab8 	.word	0x3000cab8
300063c4:	3000ce34 	.word	0x3000ce34
300063c8:	07ffe004 	.word	0x07ffe004
300063cc:	3000e7e8 	.word	0x3000e7e8
300063d0:	3000cb24 	.word	0x3000cb24
300063d4:	3000cb64 	.word	0x3000cb64
300063d8:	3000cae8 	.word	0x3000cae8
300063dc:	3000ca5c 	.word	0x3000ca5c
300063e0:	3000ce4c 	.word	0x3000ce4c
300063e4:	3000ce68 	.word	0x3000ce68
300063e8:	30004225 	.word	0x30004225
300063ec:	3000cc94 	.word	0x3000cc94
300063f0:	3000ffa4 	.word	0x3000ffa4
300063f4:	30004221 	.word	0x30004221
300063f8:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
300063fc:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300063fe:	1d31      	adds	r1, r6, #4
30006400:	2b00      	cmp	r3, #0
30006402:	f000 8102 	beq.w	3000660a <BOOT_LoadImages+0xa4e>
30006406:	1d18      	adds	r0, r3, #4
30006408:	f04f 32ff 	mov.w	r2, #4294967295
3000640c:	4430      	add	r0, r6
3000640e:	f811 3b01 	ldrb.w	r3, [r1], #1
30006412:	4053      	eors	r3, r2
30006414:	4281      	cmp	r1, r0
30006416:	b2db      	uxtb	r3, r3
30006418:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000641c:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30006420:	d1f5      	bne.n	3000640e <BOOT_LoadImages+0x852>
30006422:	43d2      	mvns	r2, r2
30006424:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30006426:	4293      	cmp	r3, r2
30006428:	f000 80d5 	beq.w	300065d6 <BOOT_LoadImages+0xa1a>
3000642c:	f8d6 300a 	ldr.w	r3, [r6, #10]
30006430:	3301      	adds	r3, #1
30006432:	d062      	beq.n	300064fa <BOOT_LoadImages+0x93e>
30006434:	990b      	ldr	r1, [sp, #44]	; 0x2c
30006436:	4890      	ldr	r0, [pc, #576]	; (30006678 <BOOT_LoadImages+0xabc>)
30006438:	f005 ffa6 	bl	3000c388 <__DiagPrintf_veneer>
3000643c:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30006440:	2002      	movs	r0, #2
30006442:	f7fe fd61 	bl	30004f08 <FLASH_Erase_With_Lock>
30006446:	e6ff      	b.n	30006248 <BOOT_LoadImages+0x68c>
30006448:	ab16      	add	r3, sp, #88	; 0x58
3000644a:	488c      	ldr	r0, [pc, #560]	; (3000667c <BOOT_LoadImages+0xac0>)
3000644c:	461e      	mov	r6, r3
3000644e:	ee08 3a10 	vmov	s16, r3
30006452:	f005 ff99 	bl	3000c388 <__DiagPrintf_veneer>
30006456:	2280      	movs	r2, #128	; 0x80
30006458:	21ff      	movs	r1, #255	; 0xff
3000645a:	4630      	mov	r0, r6
3000645c:	f005 ff6c 	bl	3000c338 <____wrap_memset_veneer>
30006460:	2280      	movs	r2, #128	; 0x80
30006462:	4629      	mov	r1, r5
30006464:	4630      	mov	r0, r6
30006466:	f005 ff4f 	bl	3000c308 <____wrap_memcpy_veneer>
3000646a:	f1ba 0f00 	cmp.w	sl, #0
3000646e:	d036      	beq.n	300064de <BOOT_LoadImages+0x922>
30006470:	46b8      	mov	r8, r7
30006472:	e545      	b.n	30005f00 <BOOT_LoadImages+0x344>
30006474:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006478:	f8d3 200a 	ldr.w	r2, [r3, #10]
3000647c:	3201      	adds	r2, #1
3000647e:	d03c      	beq.n	300064fa <BOOT_LoadImages+0x93e>
30006480:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006484:	487e      	ldr	r0, [pc, #504]	; (30006680 <BOOT_LoadImages+0xac4>)
30006486:	f005 ff7f 	bl	3000c388 <__DiagPrintf_veneer>
3000648a:	9705      	str	r7, [sp, #20]
3000648c:	f08a 0601 	eor.w	r6, sl, #1
30006490:	f04f 0801 	mov.w	r8, #1
30006494:	f006 0601 	and.w	r6, r6, #1
30006498:	4647      	mov	r7, r8
3000649a:	e724      	b.n	300062e6 <BOOT_LoadImages+0x72a>
3000649c:	ab16      	add	r3, sp, #88	; 0x58
3000649e:	2280      	movs	r2, #128	; 0x80
300064a0:	21ff      	movs	r1, #255	; 0xff
300064a2:	461e      	mov	r6, r3
300064a4:	4618      	mov	r0, r3
300064a6:	ee08 3a10 	vmov	s16, r3
300064aa:	f005 ff45 	bl	3000c338 <____wrap_memset_veneer>
300064ae:	2280      	movs	r2, #128	; 0x80
300064b0:	4629      	mov	r1, r5
300064b2:	4630      	mov	r0, r6
300064b4:	f005 ff28 	bl	3000c308 <____wrap_memcpy_veneer>
300064b8:	f1ba 0f00 	cmp.w	sl, #0
300064bc:	f040 80d9 	bne.w	30006672 <BOOT_LoadImages+0xab6>
300064c0:	b9bf      	cbnz	r7, 300064f2 <BOOT_LoadImages+0x936>
300064c2:	46c2      	mov	sl, r8
300064c4:	9705      	str	r7, [sp, #20]
300064c6:	e00c      	b.n	300064e2 <BOOT_LoadImages+0x926>
300064c8:	486e      	ldr	r0, [pc, #440]	; (30006684 <BOOT_LoadImages+0xac8>)
300064ca:	f005 ff5d 	bl	3000c388 <__DiagPrintf_veneer>
300064ce:	e4c3      	b.n	30005e58 <BOOT_LoadImages+0x29c>
300064d0:	486a      	ldr	r0, [pc, #424]	; (3000667c <BOOT_LoadImages+0xac0>)
300064d2:	f005 ff59 	bl	3000c388 <__DiagPrintf_veneer>
300064d6:	e4bf      	b.n	30005e58 <BOOT_LoadImages+0x29c>
300064d8:	f04f 0800 	mov.w	r8, #0
300064dc:	e50d      	b.n	30005efa <BOOT_LoadImages+0x33e>
300064de:	2301      	movs	r3, #1
300064e0:	9305      	str	r3, [sp, #20]
300064e2:	46d0      	mov	r8, sl
300064e4:	2101      	movs	r1, #1
300064e6:	f04f 0a00 	mov.w	sl, #0
300064ea:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300064ee:	4657      	mov	r7, sl
300064f0:	e50d      	b.n	30005f0e <BOOT_LoadImages+0x352>
300064f2:	f8cd a014 	str.w	sl, [sp, #20]
300064f6:	46c2      	mov	sl, r8
300064f8:	e748      	b.n	3000638c <BOOT_LoadImages+0x7d0>
300064fa:	4860      	ldr	r0, [pc, #384]	; (3000667c <BOOT_LoadImages+0xac0>)
300064fc:	f005 ff44 	bl	3000c388 <__DiagPrintf_veneer>
30006500:	e6a2      	b.n	30006248 <BOOT_LoadImages+0x68c>
30006502:	4620      	mov	r0, r4
30006504:	e5e0      	b.n	300060c8 <BOOT_LoadImages+0x50c>
30006506:	428a      	cmp	r2, r1
30006508:	f4ff ad92 	bcc.w	30006030 <BOOT_LoadImages+0x474>
3000650c:	4282      	cmp	r2, r0
3000650e:	f4ff ad8f 	bcc.w	30006030 <BOOT_LoadImages+0x474>
30006512:	2601      	movs	r6, #1
30006514:	464b      	mov	r3, r9
30006516:	9a07      	ldr	r2, [sp, #28]
30006518:	4651      	mov	r1, sl
3000651a:	4630      	mov	r0, r6
3000651c:	9500      	str	r5, [sp, #0]
3000651e:	9601      	str	r6, [sp, #4]
30006520:	f7fe fdea 	bl	300050f8 <rewrite_bp>
30006524:	f04f 0800 	mov.w	r8, #0
30006528:	b2c3      	uxtb	r3, r0
3000652a:	f8df 9180 	ldr.w	r9, [pc, #384]	; 300066ac <BOOT_LoadImages+0xaf0>
3000652e:	f8df b180 	ldr.w	fp, [pc, #384]	; 300066b0 <BOOT_LoadImages+0xaf4>
30006532:	46c2      	mov	sl, r8
30006534:	9303      	str	r3, [sp, #12]
30006536:	4b54      	ldr	r3, [pc, #336]	; (30006688 <BOOT_LoadImages+0xacc>)
30006538:	9304      	str	r3, [sp, #16]
3000653a:	e497      	b.n	30005e6c <BOOT_LoadImages+0x2b0>
3000653c:	f8cd a014 	str.w	sl, [sp, #20]
30006540:	4639      	mov	r1, r7
30006542:	46ba      	mov	sl, r7
30006544:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
30006548:	e4e1      	b.n	30005f0e <BOOT_LoadImages+0x352>
3000654a:	461a      	mov	r2, r3
3000654c:	4619      	mov	r1, r3
3000654e:	484f      	ldr	r0, [pc, #316]	; (3000668c <BOOT_LoadImages+0xad0>)
30006550:	f005 ff1a 	bl	3000c388 <__DiagPrintf_veneer>
30006554:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30006558:	4849      	ldr	r0, [pc, #292]	; (30006680 <BOOT_LoadImages+0xac4>)
3000655a:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000655e:	f005 ff13 	bl	3000c388 <__DiagPrintf_veneer>
30006562:	2301      	movs	r3, #1
30006564:	4698      	mov	r8, r3
30006566:	9303      	str	r3, [sp, #12]
30006568:	e476      	b.n	30005e58 <BOOT_LoadImages+0x29c>
3000656a:	461a      	mov	r2, r3
3000656c:	4619      	mov	r1, r3
3000656e:	4848      	ldr	r0, [pc, #288]	; (30006690 <BOOT_LoadImages+0xad4>)
30006570:	f04f 0801 	mov.w	r8, #1
30006574:	f005 ff08 	bl	3000c388 <__DiagPrintf_veneer>
30006578:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000657c:	4845      	ldr	r0, [pc, #276]	; (30006694 <BOOT_LoadImages+0xad8>)
3000657e:	f8d3 1006 	ldr.w	r1, [r3, #6]
30006582:	f005 ff01 	bl	3000c388 <__DiagPrintf_veneer>
30006586:	2302      	movs	r3, #2
30006588:	9303      	str	r3, [sp, #12]
3000658a:	e465      	b.n	30005e58 <BOOT_LoadImages+0x29c>
3000658c:	461a      	mov	r2, r3
3000658e:	4619      	mov	r1, r3
30006590:	483f      	ldr	r0, [pc, #252]	; (30006690 <BOOT_LoadImages+0xad4>)
30006592:	f005 fef9 	bl	3000c388 <__DiagPrintf_veneer>
30006596:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
3000659a:	483e      	ldr	r0, [pc, #248]	; (30006694 <BOOT_LoadImages+0xad8>)
3000659c:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065a0:	f005 fef2 	bl	3000c388 <__DiagPrintf_veneer>
300065a4:	2300      	movs	r3, #0
300065a6:	9305      	str	r3, [sp, #20]
300065a8:	e4f0      	b.n	30005f8c <BOOT_LoadImages+0x3d0>
300065aa:	483b      	ldr	r0, [pc, #236]	; (30006698 <BOOT_LoadImages+0xadc>)
300065ac:	f005 feec 	bl	3000c388 <__DiagPrintf_veneer>
300065b0:	e7fe      	b.n	300065b0 <BOOT_LoadImages+0x9f4>
300065b2:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300065b4:	e438      	b.n	30005e28 <BOOT_LoadImages+0x26c>
300065b6:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300065b8:	e5ff      	b.n	300061ba <BOOT_LoadImages+0x5fe>
300065ba:	f10a 3aff 	add.w	sl, sl, #4294967295
300065be:	f11a 0f03 	cmn.w	sl, #3
300065c2:	d824      	bhi.n	3000660e <BOOT_LoadImages+0xa52>
300065c4:	f1bb 0f01 	cmp.w	fp, #1
300065c8:	d932      	bls.n	30006630 <BOOT_LoadImages+0xa74>
300065ca:	4834      	ldr	r0, [pc, #208]	; (3000669c <BOOT_LoadImages+0xae0>)
300065cc:	e649      	b.n	30006262 <BOOT_LoadImages+0x6a6>
300065ce:	4c34      	ldr	r4, [pc, #208]	; (300066a0 <BOOT_LoadImages+0xae4>)
300065d0:	e4e2      	b.n	30005f98 <BOOT_LoadImages+0x3dc>
300065d2:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300065d4:	e627      	b.n	30006226 <BOOT_LoadImages+0x66a>
300065d6:	461a      	mov	r2, r3
300065d8:	4619      	mov	r1, r3
300065da:	482c      	ldr	r0, [pc, #176]	; (3000668c <BOOT_LoadImages+0xad0>)
300065dc:	f005 fed4 	bl	3000c388 <__DiagPrintf_veneer>
300065e0:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300065e4:	4826      	ldr	r0, [pc, #152]	; (30006680 <BOOT_LoadImages+0xac4>)
300065e6:	f8d3 1006 	ldr.w	r1, [r3, #6]
300065ea:	f005 fecd 	bl	3000c388 <__DiagPrintf_veneer>
300065ee:	2300      	movs	r3, #0
300065f0:	9305      	str	r3, [sp, #20]
300065f2:	e74b      	b.n	3000648c <BOOT_LoadImages+0x8d0>
300065f4:	4281      	cmp	r1, r0
300065f6:	f4ff ad21 	bcc.w	3000603c <BOOT_LoadImages+0x480>
300065fa:	2400      	movs	r4, #0
300065fc:	464b      	mov	r3, r9
300065fe:	4642      	mov	r2, r8
30006600:	4659      	mov	r1, fp
30006602:	4620      	mov	r0, r4
30006604:	9500      	str	r5, [sp, #0]
30006606:	9401      	str	r4, [sp, #4]
30006608:	e55e      	b.n	300060c8 <BOOT_LoadImages+0x50c>
3000660a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000660c:	e70a      	b.n	30006424 <BOOT_LoadImages+0x868>
3000660e:	4825      	ldr	r0, [pc, #148]	; (300066a4 <BOOT_LoadImages+0xae8>)
30006610:	e627      	b.n	30006262 <BOOT_LoadImages+0x6a6>
30006612:	2400      	movs	r4, #0
30006614:	464b      	mov	r3, r9
30006616:	4642      	mov	r2, r8
30006618:	4659      	mov	r1, fp
3000661a:	9500      	str	r5, [sp, #0]
3000661c:	9401      	str	r4, [sp, #4]
3000661e:	e552      	b.n	300060c6 <BOOT_LoadImages+0x50a>
30006620:	2601      	movs	r6, #1
30006622:	464b      	mov	r3, r9
30006624:	9a07      	ldr	r2, [sp, #28]
30006626:	4651      	mov	r1, sl
30006628:	2000      	movs	r0, #0
3000662a:	9500      	str	r5, [sp, #0]
3000662c:	9601      	str	r6, [sp, #4]
3000662e:	e777      	b.n	30006520 <BOOT_LoadImages+0x964>
30006630:	9904      	ldr	r1, [sp, #16]
30006632:	481d      	ldr	r0, [pc, #116]	; (300066a8 <BOOT_LoadImages+0xaec>)
30006634:	f005 fea8 	bl	3000c388 <__DiagPrintf_veneer>
30006638:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000663c:	f8d9 a00d 	ldr.w	sl, [r9, #13]
30006640:	9307      	str	r3, [sp, #28]
30006642:	2301      	movs	r3, #1
30006644:	f8d5 b00d 	ldr.w	fp, [r5, #13]
30006648:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000664c:	9309      	str	r3, [sp, #36]	; 0x24
3000664e:	e615      	b.n	3000627c <BOOT_LoadImages+0x6c0>
30006650:	f08a 0601 	eor.w	r6, sl, #1
30006654:	f006 0601 	and.w	r6, r6, #1
30006658:	e408      	b.n	30005e6c <BOOT_LoadImages+0x2b0>
3000665a:	f08a 0601 	eor.w	r6, sl, #1
3000665e:	f8cd 8014 	str.w	r8, [sp, #20]
30006662:	f006 0601 	and.w	r6, r6, #1
30006666:	e401      	b.n	30005e6c <BOOT_LoadImages+0x2b0>
30006668:	2601      	movs	r6, #1
3000666a:	4c0d      	ldr	r4, [pc, #52]	; (300066a0 <BOOT_LoadImages+0xae4>)
3000666c:	46b2      	mov	sl, r6
3000666e:	f7ff bbc1 	b.w	30005df4 <BOOT_LoadImages+0x238>
30006672:	f04f 0a00 	mov.w	sl, #0
30006676:	e440      	b.n	30005efa <BOOT_LoadImages+0x33e>
30006678:	3000cce0 	.word	0x3000cce0
3000667c:	3000cc84 	.word	0x3000cc84
30006680:	3000c9d4 	.word	0x3000c9d4
30006684:	3000cc94 	.word	0x3000cc94
30006688:	3000e623 	.word	0x3000e623
3000668c:	3000cca4 	.word	0x3000cca4
30006690:	3000cd2c 	.word	0x3000cd2c
30006694:	3000c9ec 	.word	0x3000c9ec
30006698:	3000cdec 	.word	0x3000cdec
3000669c:	3000ce4c 	.word	0x3000ce4c
300066a0:	3000e7e8 	.word	0x3000e7e8
300066a4:	3000cad4 	.word	0x3000cad4
300066a8:	3000ce84 	.word	0x3000ce84
300066ac:	3000ffa4 	.word	0x3000ffa4
300066b0:	30004221 	.word	0x30004221

300066b4 <BOOT_ReasonSet>:
300066b4:	4a03      	ldr	r2, [pc, #12]	; (300066c4 <BOOT_ReasonSet+0x10>)
300066b6:	4904      	ldr	r1, [pc, #16]	; (300066c8 <BOOT_ReasonSet+0x14>)
300066b8:	8c13      	ldrh	r3, [r2, #32]
300066ba:	b29b      	uxth	r3, r3
300066bc:	8413      	strh	r3, [r2, #32]
300066be:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
300066c2:	4770      	bx	lr
300066c4:	42008000 	.word	0x42008000
300066c8:	42008200 	.word	0x42008200

300066cc <BOOT_Enable_KM0>:
300066cc:	b510      	push	{r4, lr}
300066ce:	4c12      	ldr	r4, [pc, #72]	; (30006718 <BOOT_Enable_KM0+0x4c>)
300066d0:	47a0      	blx	r4
300066d2:	b110      	cbz	r0, 300066da <BOOT_Enable_KM0+0xe>
300066d4:	4b11      	ldr	r3, [pc, #68]	; (3000671c <BOOT_Enable_KM0+0x50>)
300066d6:	4798      	blx	r3
300066d8:	b1a8      	cbz	r0, 30006706 <BOOT_Enable_KM0+0x3a>
300066da:	47a0      	blx	r4
300066dc:	b940      	cbnz	r0, 300066f0 <BOOT_Enable_KM0+0x24>
300066de:	4a10      	ldr	r2, [pc, #64]	; (30006720 <BOOT_Enable_KM0+0x54>)
300066e0:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300066e4:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300066e8:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300066ec:	bf40      	sev
300066ee:	bd10      	pop	{r4, pc}
300066f0:	4b0a      	ldr	r3, [pc, #40]	; (3000671c <BOOT_Enable_KM0+0x50>)
300066f2:	4798      	blx	r3
300066f4:	2800      	cmp	r0, #0
300066f6:	d0f2      	beq.n	300066de <BOOT_Enable_KM0+0x12>
300066f8:	4b0a      	ldr	r3, [pc, #40]	; (30006724 <BOOT_Enable_KM0+0x58>)
300066fa:	2004      	movs	r0, #4
300066fc:	4a0a      	ldr	r2, [pc, #40]	; (30006728 <BOOT_Enable_KM0+0x5c>)
300066fe:	4619      	mov	r1, r3
30006700:	f005 fb9c 	bl	3000be3c <rtk_log_write>
30006704:	e7eb      	b.n	300066de <BOOT_Enable_KM0+0x12>
30006706:	4b07      	ldr	r3, [pc, #28]	; (30006724 <BOOT_Enable_KM0+0x58>)
30006708:	2004      	movs	r0, #4
3000670a:	4a08      	ldr	r2, [pc, #32]	; (3000672c <BOOT_Enable_KM0+0x60>)
3000670c:	4619      	mov	r1, r3
3000670e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30006712:	f005 bb93 	b.w	3000be3c <rtk_log_write>
30006716:	bf00      	nop
30006718:	0000c09d 	.word	0x0000c09d
3000671c:	0000c0ad 	.word	0x0000c0ad
30006720:	42008000 	.word	0x42008000
30006724:	3000c674 	.word	0x3000c674
30006728:	3000cec8 	.word	0x3000cec8
3000672c:	3000cea0 	.word	0x3000cea0

30006730 <BOOT_AP_Clk_Get>:
30006730:	4a06      	ldr	r2, [pc, #24]	; (3000674c <BOOT_AP_Clk_Get+0x1c>)
30006732:	6913      	ldr	r3, [r2, #16]
30006734:	2b00      	cmp	r3, #0
30006736:	db03      	blt.n	30006740 <BOOT_AP_Clk_Get+0x10>
30006738:	6810      	ldr	r0, [r2, #0]
3000673a:	fbb0 f0f3 	udiv	r0, r0, r3
3000673e:	4770      	bx	lr
30006740:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30006744:	6850      	ldr	r0, [r2, #4]
30006746:	fbb0 f0f3 	udiv	r0, r0, r3
3000674a:	4770      	bx	lr
3000674c:	3000ee70 	.word	0x3000ee70

30006750 <BOOT_SOC_ClkSet>:
30006750:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006754:	4c5d      	ldr	r4, [pc, #372]	; (300068cc <BOOT_SOC_ClkSet+0x17c>)
30006756:	b083      	sub	sp, #12
30006758:	6926      	ldr	r6, [r4, #16]
3000675a:	6825      	ldr	r5, [r4, #0]
3000675c:	2e00      	cmp	r6, #0
3000675e:	db7a      	blt.n	30006856 <BOOT_SOC_ClkSet+0x106>
30006760:	fbb5 f6f6 	udiv	r6, r5, r6
30006764:	4b5a      	ldr	r3, [pc, #360]	; (300068d0 <BOOT_SOC_ClkSet+0x180>)
30006766:	68a7      	ldr	r7, [r4, #8]
30006768:	fba3 1305 	umull	r1, r3, r3, r5
3000676c:	68e2      	ldr	r2, [r4, #12]
3000676e:	2f01      	cmp	r7, #1
30006770:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
30006774:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30006778:	ea4f 1393 	mov.w	r3, r3, lsr #6
3000677c:	f10a 3aff 	add.w	sl, sl, #4294967295
30006780:	f10b 3bff 	add.w	fp, fp, #4294967295
30006784:	f103 39ff 	add.w	r9, r3, #4294967295
30006788:	fbb5 f5f2 	udiv	r5, r5, r2
3000678c:	d06f      	beq.n	3000686e <BOOT_SOC_ClkSet+0x11e>
3000678e:	4b51      	ldr	r3, [pc, #324]	; (300068d4 <BOOT_SOC_ClkSet+0x184>)
30006790:	2200      	movs	r2, #0
30006792:	2dfa      	cmp	r5, #250	; 0xfa
30006794:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30006798:	d863      	bhi.n	30006862 <BOOT_SOC_ClkSet+0x112>
3000679a:	4f4f      	ldr	r7, [pc, #316]	; (300068d8 <BOOT_SOC_ClkSet+0x188>)
3000679c:	783b      	ldrb	r3, [r7, #0]
3000679e:	2b01      	cmp	r3, #1
300067a0:	d103      	bne.n	300067aa <BOOT_SOC_ClkSet+0x5a>
300067a2:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
300067a6:	f200 8085 	bhi.w	300068b4 <BOOT_SOC_ClkSet+0x164>
300067aa:	2001      	movs	r0, #1
300067ac:	f8df 8154 	ldr.w	r8, [pc, #340]	; 30006904 <BOOT_SOC_ClkSet+0x1b4>
300067b0:	f001 fb6a 	bl	30007e88 <CLK_SWITCH_XTAL>
300067b4:	6820      	ldr	r0, [r4, #0]
300067b6:	4b49      	ldr	r3, [pc, #292]	; (300068dc <BOOT_SOC_ClkSet+0x18c>)
300067b8:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
300067bc:	fb08 f000 	mul.w	r0, r8, r0
300067c0:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
300067c4:	4798      	blx	r3
300067c6:	4a46      	ldr	r2, [pc, #280]	; (300068e0 <BOOT_SOC_ClkSet+0x190>)
300067c8:	68e1      	ldr	r1, [r4, #12]
300067ca:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300067ce:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300067d2:	ea4f 5909 	mov.w	r9, r9, lsl #20
300067d6:	1e4b      	subs	r3, r1, #1
300067d8:	4942      	ldr	r1, [pc, #264]	; (300068e4 <BOOT_SOC_ClkSet+0x194>)
300067da:	fa1f fb8b 	uxth.w	fp, fp
300067de:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300067e2:	4001      	ands	r1, r0
300067e4:	f003 0307 	and.w	r3, r3, #7
300067e8:	2000      	movs	r0, #0
300067ea:	430b      	orrs	r3, r1
300067ec:	ea4a 0303 	orr.w	r3, sl, r3
300067f0:	ea4b 0303 	orr.w	r3, fp, r3
300067f4:	ea49 0303 	orr.w	r3, r9, r3
300067f8:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300067fc:	f001 fb44 	bl	30007e88 <CLK_SWITCH_XTAL>
30006800:	4b39      	ldr	r3, [pc, #228]	; (300068e8 <BOOT_SOC_ClkSet+0x198>)
30006802:	4a3a      	ldr	r2, [pc, #232]	; (300068ec <BOOT_SOC_ClkSet+0x19c>)
30006804:	2004      	movs	r0, #4
30006806:	4619      	mov	r1, r3
30006808:	9500      	str	r5, [sp, #0]
3000680a:	f005 fb17 	bl	3000be3c <rtk_log_write>
3000680e:	783b      	ldrb	r3, [r7, #0]
30006810:	b1f3      	cbz	r3, 30006850 <BOOT_SOC_ClkSet+0x100>
30006812:	6923      	ldr	r3, [r4, #16]
30006814:	2b00      	cmp	r3, #0
30006816:	db40      	blt.n	3000689a <BOOT_SOC_ClkSet+0x14a>
30006818:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000681c:	492d      	ldr	r1, [pc, #180]	; (300068d4 <BOOT_SOC_ClkSet+0x184>)
3000681e:	2000      	movs	r0, #0
30006820:	6913      	ldr	r3, [r2, #16]
30006822:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
30006826:	f043 030c 	orr.w	r3, r3, #12
3000682a:	6113      	str	r3, [r2, #16]
3000682c:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30006830:	6921      	ldr	r1, [r4, #16]
30006832:	4b2d      	ldr	r3, [pc, #180]	; (300068e8 <BOOT_SOC_ClkSet+0x198>)
30006834:	6928      	ldr	r0, [r5, #16]
30006836:	3901      	subs	r1, #1
30006838:	4a2d      	ldr	r2, [pc, #180]	; (300068f0 <BOOT_SOC_ClkSet+0x1a0>)
3000683a:	f020 0003 	bic.w	r0, r0, #3
3000683e:	f001 0103 	and.w	r1, r1, #3
30006842:	4301      	orrs	r1, r0
30006844:	2004      	movs	r0, #4
30006846:	6129      	str	r1, [r5, #16]
30006848:	4619      	mov	r1, r3
3000684a:	9600      	str	r6, [sp, #0]
3000684c:	f005 faf6 	bl	3000be3c <rtk_log_write>
30006850:	b003      	add	sp, #12
30006852:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006856:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
3000685a:	6863      	ldr	r3, [r4, #4]
3000685c:	fbb3 f6f6 	udiv	r6, r3, r6
30006860:	e780      	b.n	30006764 <BOOT_SOC_ClkSet+0x14>
30006862:	f240 61b9 	movw	r1, #1721	; 0x6b9
30006866:	4823      	ldr	r0, [pc, #140]	; (300068f4 <BOOT_SOC_ClkSet+0x1a4>)
30006868:	f005 fdce 	bl	3000c408 <__io_assert_failed_veneer>
3000686c:	e795      	b.n	3000679a <BOOT_SOC_ClkSet+0x4a>
3000686e:	4b22      	ldr	r3, [pc, #136]	; (300068f8 <BOOT_SOC_ClkSet+0x1a8>)
30006870:	4638      	mov	r0, r7
30006872:	4798      	blx	r3
30006874:	4b17      	ldr	r3, [pc, #92]	; (300068d4 <BOOT_SOC_ClkSet+0x184>)
30006876:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
3000687a:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
3000687e:	d21f      	bcs.n	300068c0 <BOOT_SOC_ClkSet+0x170>
30006880:	4f15      	ldr	r7, [pc, #84]	; (300068d8 <BOOT_SOC_ClkSet+0x188>)
30006882:	783b      	ldrb	r3, [r7, #0]
30006884:	2b01      	cmp	r3, #1
30006886:	d190      	bne.n	300067aa <BOOT_SOC_ClkSet+0x5a>
30006888:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
3000688c:	d98d      	bls.n	300067aa <BOOT_SOC_ClkSet+0x5a>
3000688e:	f240 61b5 	movw	r1, #1717	; 0x6b5
30006892:	4818      	ldr	r0, [pc, #96]	; (300068f4 <BOOT_SOC_ClkSet+0x1a4>)
30006894:	f005 fdb8 	bl	3000c408 <__io_assert_failed_veneer>
30006898:	e787      	b.n	300067aa <BOOT_SOC_ClkSet+0x5a>
3000689a:	6860      	ldr	r0, [r4, #4]
3000689c:	4b17      	ldr	r3, [pc, #92]	; (300068fc <BOOT_SOC_ClkSet+0x1ac>)
3000689e:	fb08 f000 	mul.w	r0, r8, r0
300068a2:	4798      	blx	r3
300068a4:	4b16      	ldr	r3, [pc, #88]	; (30006900 <BOOT_SOC_ClkSet+0x1b0>)
300068a6:	2001      	movs	r0, #1
300068a8:	4798      	blx	r3
300068aa:	4b0a      	ldr	r3, [pc, #40]	; (300068d4 <BOOT_SOC_ClkSet+0x184>)
300068ac:	2201      	movs	r2, #1
300068ae:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300068b2:	e7bb      	b.n	3000682c <BOOT_SOC_ClkSet+0xdc>
300068b4:	f240 61bb 	movw	r1, #1723	; 0x6bb
300068b8:	480e      	ldr	r0, [pc, #56]	; (300068f4 <BOOT_SOC_ClkSet+0x1a4>)
300068ba:	f005 fda5 	bl	3000c408 <__io_assert_failed_veneer>
300068be:	e774      	b.n	300067aa <BOOT_SOC_ClkSet+0x5a>
300068c0:	f240 61b3 	movw	r1, #1715	; 0x6b3
300068c4:	480b      	ldr	r0, [pc, #44]	; (300068f4 <BOOT_SOC_ClkSet+0x1a4>)
300068c6:	f005 fd9f 	bl	3000c408 <__io_assert_failed_veneer>
300068ca:	e7d9      	b.n	30006880 <BOOT_SOC_ClkSet+0x130>
300068cc:	3000ee70 	.word	0x3000ee70
300068d0:	51eb851f 	.word	0x51eb851f
300068d4:	23020000 	.word	0x23020000
300068d8:	3000e623 	.word	0x3000e623
300068dc:	30008a3d 	.word	0x30008a3d
300068e0:	42008000 	.word	0x42008000
300068e4:	ff8f08f8 	.word	0xff8f08f8
300068e8:	3000c674 	.word	0x3000c674
300068ec:	3000cee8 	.word	0x3000cee8
300068f0:	3000cf00 	.word	0x3000cf00
300068f4:	3000e328 	.word	0x3000e328
300068f8:	3000b4e5 	.word	0x3000b4e5
300068fc:	30008abd 	.word	0x30008abd
30006900:	30008afd 	.word	0x30008afd
30006904:	000f4240 	.word	0x000f4240

30006908 <BOOT_Disable_AP>:
30006908:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000690c:	4906      	ldr	r1, [pc, #24]	; (30006928 <BOOT_Disable_AP+0x20>)
3000690e:	685a      	ldr	r2, [r3, #4]
30006910:	f042 0202 	orr.w	r2, r2, #2
30006914:	605a      	str	r2, [r3, #4]
30006916:	685a      	ldr	r2, [r3, #4]
30006918:	f042 0230 	orr.w	r2, r2, #48	; 0x30
3000691c:	605a      	str	r2, [r3, #4]
3000691e:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30006920:	f043 0303 	orr.w	r3, r3, #3
30006924:	678b      	str	r3, [r1, #120]	; 0x78
30006926:	4770      	bx	lr
30006928:	41000200 	.word	0x41000200

3000692c <BOOT_Enable_AP>:
3000692c:	b538      	push	{r3, r4, r5, lr}
3000692e:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
30006932:	2201      	movs	r2, #1
30006934:	2104      	movs	r1, #4
30006936:	4b2b      	ldr	r3, [pc, #172]	; (300069e4 <BOOT_Enable_AP+0xb8>)
30006938:	482b      	ldr	r0, [pc, #172]	; (300069e8 <BOOT_Enable_AP+0xbc>)
3000693a:	4798      	blx	r3
3000693c:	6863      	ldr	r3, [r4, #4]
3000693e:	4d2b      	ldr	r5, [pc, #172]	; (300069ec <BOOT_Enable_AP+0xc0>)
30006940:	2032      	movs	r0, #50	; 0x32
30006942:	f043 0302 	orr.w	r3, r3, #2
30006946:	6063      	str	r3, [r4, #4]
30006948:	6863      	ldr	r3, [r4, #4]
3000694a:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000694e:	6063      	str	r3, [r4, #4]
30006950:	6823      	ldr	r3, [r4, #0]
30006952:	f043 0302 	orr.w	r3, r3, #2
30006956:	6023      	str	r3, [r4, #0]
30006958:	47a8      	blx	r5
3000695a:	6823      	ldr	r3, [r4, #0]
3000695c:	2032      	movs	r0, #50	; 0x32
3000695e:	f043 0304 	orr.w	r3, r3, #4
30006962:	6023      	str	r3, [r4, #0]
30006964:	6823      	ldr	r3, [r4, #0]
30006966:	f043 0301 	orr.w	r3, r3, #1
3000696a:	6023      	str	r3, [r4, #0]
3000696c:	6823      	ldr	r3, [r4, #0]
3000696e:	f043 0310 	orr.w	r3, r3, #16
30006972:	6023      	str	r3, [r4, #0]
30006974:	47a8      	blx	r5
30006976:	6823      	ldr	r3, [r4, #0]
30006978:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
3000697c:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30006980:	6023      	str	r3, [r4, #0]
30006982:	47a8      	blx	r5
30006984:	6863      	ldr	r3, [r4, #4]
30006986:	4a1a      	ldr	r2, [pc, #104]	; (300069f0 <BOOT_Enable_AP+0xc4>)
30006988:	f023 0310 	bic.w	r3, r3, #16
3000698c:	6063      	str	r3, [r4, #4]
3000698e:	6863      	ldr	r3, [r4, #4]
30006990:	f023 0302 	bic.w	r3, r3, #2
30006994:	6063      	str	r3, [r4, #4]
30006996:	4790      	blx	r2
30006998:	b178      	cbz	r0, 300069ba <BOOT_Enable_AP+0x8e>
3000699a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000699e:	4815      	ldr	r0, [pc, #84]	; (300069f4 <BOOT_Enable_AP+0xc8>)
300069a0:	4a15      	ldr	r2, [pc, #84]	; (300069f8 <BOOT_Enable_AP+0xcc>)
300069a2:	68d9      	ldr	r1, [r3, #12]
300069a4:	f041 0101 	orr.w	r1, r1, #1
300069a8:	60d9      	str	r1, [r3, #12]
300069aa:	6899      	ldr	r1, [r3, #8]
300069ac:	f041 0101 	orr.w	r1, r1, #1
300069b0:	6099      	str	r1, [r3, #8]
300069b2:	6843      	ldr	r3, [r0, #4]
300069b4:	431a      	orrs	r2, r3
300069b6:	6042      	str	r2, [r0, #4]
300069b8:	bd38      	pop	{r3, r4, r5, pc}
300069ba:	4604      	mov	r4, r0
300069bc:	4d0f      	ldr	r5, [pc, #60]	; (300069fc <BOOT_Enable_AP+0xd0>)
300069be:	4601      	mov	r1, r0
300069c0:	2041      	movs	r0, #65	; 0x41
300069c2:	47a8      	blx	r5
300069c4:	4621      	mov	r1, r4
300069c6:	2042      	movs	r0, #66	; 0x42
300069c8:	47a8      	blx	r5
300069ca:	4621      	mov	r1, r4
300069cc:	2043      	movs	r0, #67	; 0x43
300069ce:	47a8      	blx	r5
300069d0:	4621      	mov	r1, r4
300069d2:	2044      	movs	r0, #68	; 0x44
300069d4:	47a8      	blx	r5
300069d6:	4621      	mov	r1, r4
300069d8:	2045      	movs	r0, #69	; 0x45
300069da:	47a8      	blx	r5
300069dc:	4621      	mov	r1, r4
300069de:	2046      	movs	r0, #70	; 0x46
300069e0:	47a8      	blx	r5
300069e2:	e7da      	b.n	3000699a <BOOT_Enable_AP+0x6e>
300069e4:	0000aab9 	.word	0x0000aab9
300069e8:	4200c000 	.word	0x4200c000
300069ec:	00009b2d 	.word	0x00009b2d
300069f0:	30009ba9 	.word	0x30009ba9
300069f4:	41000200 	.word	0x41000200
300069f8:	01001111 	.word	0x01001111
300069fc:	0000b20d 	.word	0x0000b20d

30006a00 <BOOT_RAM_KeyDeriveFunc>:
30006a00:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006a04:	ed2d 8b04 	vpush	{d8-d9}
30006a08:	b0cb      	sub	sp, #300	; 0x12c
30006a0a:	4c6a      	ldr	r4, [pc, #424]	; (30006bb4 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30006a0c:	4680      	mov	r8, r0
30006a0e:	4689      	mov	r9, r1
30006a10:	4869      	ldr	r0, [pc, #420]	; (30006bb8 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
30006a12:	ee08 2a90 	vmov	s17, r2
30006a16:	4969      	ldr	r1, [pc, #420]	; (30006bbc <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006a18:	2201      	movs	r2, #1
30006a1a:	ee09 3a10 	vmov	s18, r3
30006a1e:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
30006a22:	47a0      	blx	r4
30006a24:	2201      	movs	r2, #1
30006a26:	4966      	ldr	r1, [pc, #408]	; (30006bc0 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30006a28:	ae02      	add	r6, sp, #8
30006a2a:	4864      	ldr	r0, [pc, #400]	; (30006bbc <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30006a2c:	47a0      	blx	r4
30006a2e:	4b65      	ldr	r3, [pc, #404]	; (30006bc4 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30006a30:	2000      	movs	r0, #0
30006a32:	4798      	blx	r3
30006a34:	22e0      	movs	r2, #224	; 0xe0
30006a36:	2100      	movs	r1, #0
30006a38:	a812      	add	r0, sp, #72	; 0x48
30006a3a:	f005 fc7d 	bl	3000c338 <____wrap_memset_veneer>
30006a3e:	2300      	movs	r3, #0
30006a40:	f8ad 3004 	strh.w	r3, [sp, #4]
30006a44:	f88d 3006 	strb.w	r3, [sp, #6]
30006a48:	2301      	movs	r3, #1
30006a4a:	f88d 3007 	strb.w	r3, [sp, #7]
30006a4e:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006a50:	2b00      	cmp	r3, #0
30006a52:	f000 80ab 	beq.w	30006bac <BOOT_RAM_KeyDeriveFunc+0x1ac>
30006a56:	ab0a      	add	r3, sp, #40	; 0x28
30006a58:	f8df b17c 	ldr.w	fp, [pc, #380]	; 30006bd8 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30006a5c:	4f5a      	ldr	r7, [pc, #360]	; (30006bc8 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30006a5e:	ee08 3a10 	vmov	s16, r3
30006a62:	ab12      	add	r3, sp, #72	; 0x48
30006a64:	464a      	mov	r2, r9
30006a66:	4641      	mov	r1, r8
30006a68:	2020      	movs	r0, #32
30006a6a:	47d8      	blx	fp
30006a6c:	aa12      	add	r2, sp, #72	; 0x48
30006a6e:	ee19 1a10 	vmov	r1, s18
30006a72:	ee18 0a90 	vmov	r0, s17
30006a76:	47b8      	blx	r7
30006a78:	ee18 5a10 	vmov	r5, s16
30006a7c:	aa12      	add	r2, sp, #72	; 0x48
30006a7e:	2104      	movs	r1, #4
30006a80:	a801      	add	r0, sp, #4
30006a82:	4634      	mov	r4, r6
30006a84:	47b8      	blx	r7
30006a86:	a912      	add	r1, sp, #72	; 0x48
30006a88:	4b50      	ldr	r3, [pc, #320]	; (30006bcc <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006a8a:	ee18 0a10 	vmov	r0, s16
30006a8e:	4798      	blx	r3
30006a90:	f1ba 0f01 	cmp.w	sl, #1
30006a94:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30006a96:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30006a98:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30006a9c:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006aa0:	d91f      	bls.n	30006ae2 <BOOT_RAM_KeyDeriveFunc+0xe2>
30006aa2:	2501      	movs	r5, #1
30006aa4:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30006aa8:	ab12      	add	r3, sp, #72	; 0x48
30006aaa:	4641      	mov	r1, r8
30006aac:	464a      	mov	r2, r9
30006aae:	2020      	movs	r0, #32
30006ab0:	47d8      	blx	fp
30006ab2:	aa12      	add	r2, sp, #72	; 0x48
30006ab4:	2120      	movs	r1, #32
30006ab6:	4630      	mov	r0, r6
30006ab8:	47b8      	blx	r7
30006aba:	a912      	add	r1, sp, #72	; 0x48
30006abc:	4b43      	ldr	r3, [pc, #268]	; (30006bcc <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30006abe:	4630      	mov	r0, r6
30006ac0:	4798      	blx	r3
30006ac2:	f10d 0327 	add.w	r3, sp, #39	; 0x27
30006ac6:	f10d 0107 	add.w	r1, sp, #7
30006aca:	f813 2f01 	ldrb.w	r2, [r3, #1]!
30006ace:	f811 0f01 	ldrb.w	r0, [r1, #1]!
30006ad2:	42a3      	cmp	r3, r4
30006ad4:	ea82 0200 	eor.w	r2, r2, r0
30006ad8:	701a      	strb	r2, [r3, #0]
30006ada:	d1f6      	bne.n	30006aca <BOOT_RAM_KeyDeriveFunc+0xca>
30006adc:	3501      	adds	r5, #1
30006ade:	45aa      	cmp	sl, r5
30006ae0:	d1e2      	bne.n	30006aa8 <BOOT_RAM_KeyDeriveFunc+0xa8>
30006ae2:	9a59      	ldr	r2, [sp, #356]	; 0x164
30006ae4:	ee18 1a10 	vmov	r1, s16
30006ae8:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006aea:	2a20      	cmp	r2, #32
30006aec:	985a      	ldr	r0, [sp, #360]	; 0x168
30006aee:	bf28      	it	cs
30006af0:	2220      	movcs	r2, #32
30006af2:	1a9b      	subs	r3, r3, r2
30006af4:	9359      	str	r3, [sp, #356]	; 0x164
30006af6:	4603      	mov	r3, r0
30006af8:	4413      	add	r3, r2
30006afa:	935a      	str	r3, [sp, #360]	; 0x168
30006afc:	f005 fc04 	bl	3000c308 <____wrap_memcpy_veneer>
30006b00:	f10d 0207 	add.w	r2, sp, #7
30006b04:	2104      	movs	r1, #4
30006b06:	7813      	ldrb	r3, [r2, #0]
30006b08:	3901      	subs	r1, #1
30006b0a:	3301      	adds	r3, #1
30006b0c:	b2db      	uxtb	r3, r3
30006b0e:	f802 3901 	strb.w	r3, [r2], #-1
30006b12:	b90b      	cbnz	r3, 30006b18 <BOOT_RAM_KeyDeriveFunc+0x118>
30006b14:	2900      	cmp	r1, #0
30006b16:	d1f6      	bne.n	30006b06 <BOOT_RAM_KeyDeriveFunc+0x106>
30006b18:	9b59      	ldr	r3, [sp, #356]	; 0x164
30006b1a:	2b00      	cmp	r3, #0
30006b1c:	d1a1      	bne.n	30006a62 <BOOT_RAM_KeyDeriveFunc+0x62>
30006b1e:	2220      	movs	r2, #32
30006b20:	2100      	movs	r1, #0
30006b22:	4c2b      	ldr	r4, [pc, #172]	; (30006bd0 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30006b24:	4630      	mov	r0, r6
30006b26:	47a0      	blx	r4
30006b28:	2220      	movs	r2, #32
30006b2a:	2100      	movs	r1, #0
30006b2c:	ee18 0a10 	vmov	r0, s16
30006b30:	47a0      	blx	r4
30006b32:	4b28      	ldr	r3, [pc, #160]	; (30006bd4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b34:	695b      	ldr	r3, [r3, #20]
30006b36:	03da      	lsls	r2, r3, #15
30006b38:	d512      	bpl.n	30006b60 <BOOT_RAM_KeyDeriveFunc+0x160>
30006b3a:	f016 021f 	ands.w	r2, r6, #31
30006b3e:	4633      	mov	r3, r6
30006b40:	d130      	bne.n	30006ba4 <BOOT_RAM_KeyDeriveFunc+0x1a4>
30006b42:	2220      	movs	r2, #32
30006b44:	f3bf 8f4f 	dsb	sy
30006b48:	4822      	ldr	r0, [pc, #136]	; (30006bd4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b4a:	441a      	add	r2, r3
30006b4c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006b50:	3320      	adds	r3, #32
30006b52:	1ad1      	subs	r1, r2, r3
30006b54:	2900      	cmp	r1, #0
30006b56:	dcf9      	bgt.n	30006b4c <BOOT_RAM_KeyDeriveFunc+0x14c>
30006b58:	f3bf 8f4f 	dsb	sy
30006b5c:	f3bf 8f6f 	isb	sy
30006b60:	4b1c      	ldr	r3, [pc, #112]	; (30006bd4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b62:	695b      	ldr	r3, [r3, #20]
30006b64:	03db      	lsls	r3, r3, #15
30006b66:	d513      	bpl.n	30006b90 <BOOT_RAM_KeyDeriveFunc+0x190>
30006b68:	ee18 3a10 	vmov	r3, s16
30006b6c:	f013 021f 	ands.w	r2, r3, #31
30006b70:	d114      	bne.n	30006b9c <BOOT_RAM_KeyDeriveFunc+0x19c>
30006b72:	2220      	movs	r2, #32
30006b74:	f3bf 8f4f 	dsb	sy
30006b78:	4816      	ldr	r0, [pc, #88]	; (30006bd4 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30006b7a:	441a      	add	r2, r3
30006b7c:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30006b80:	3320      	adds	r3, #32
30006b82:	1ad1      	subs	r1, r2, r3
30006b84:	2900      	cmp	r1, #0
30006b86:	dcf9      	bgt.n	30006b7c <BOOT_RAM_KeyDeriveFunc+0x17c>
30006b88:	f3bf 8f4f 	dsb	sy
30006b8c:	f3bf 8f6f 	isb	sy
30006b90:	2000      	movs	r0, #0
30006b92:	b04b      	add	sp, #300	; 0x12c
30006b94:	ecbd 8b04 	vpop	{d8-d9}
30006b98:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006b9c:	f023 031f 	bic.w	r3, r3, #31
30006ba0:	3220      	adds	r2, #32
30006ba2:	e7e7      	b.n	30006b74 <BOOT_RAM_KeyDeriveFunc+0x174>
30006ba4:	f026 031f 	bic.w	r3, r6, #31
30006ba8:	3220      	adds	r2, #32
30006baa:	e7cb      	b.n	30006b44 <BOOT_RAM_KeyDeriveFunc+0x144>
30006bac:	ab0a      	add	r3, sp, #40	; 0x28
30006bae:	ee08 3a10 	vmov	s16, r3
30006bb2:	e7b4      	b.n	30006b1e <BOOT_RAM_KeyDeriveFunc+0x11e>
30006bb4:	0000b479 	.word	0x0000b479
30006bb8:	40000008 	.word	0x40000008
30006bbc:	40000004 	.word	0x40000004
30006bc0:	40000002 	.word	0x40000002
30006bc4:	30009e45 	.word	0x30009e45
30006bc8:	30009c49 	.word	0x30009c49
30006bcc:	30009c85 	.word	0x30009c85
30006bd0:	00012be5 	.word	0x00012be5
30006bd4:	e000ed00 	.word	0xe000ed00
30006bd8:	300094c9 	.word	0x300094c9

30006bdc <password_hash_check>:
30006bdc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006be0:	b08f      	sub	sp, #60	; 0x3c
30006be2:	4690      	mov	r8, r2
30006be4:	461d      	mov	r5, r3
30006be6:	4606      	mov	r6, r0
30006be8:	af04      	add	r7, sp, #16
30006bea:	3201      	adds	r2, #1
30006bec:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006bee:	6079      	str	r1, [r7, #4]
30006bf0:	2100      	movs	r1, #0
30006bf2:	f103 0008 	add.w	r0, r3, #8
30006bf6:	f108 0308 	add.w	r3, r8, #8
30006bfa:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
30006bfe:	f023 0307 	bic.w	r3, r3, #7
30006c02:	f020 0007 	bic.w	r0, r0, #7
30006c06:	ebad 0d03 	sub.w	sp, sp, r3
30006c0a:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006c0c:	f10d 0910 	add.w	r9, sp, #16
30006c10:	3301      	adds	r3, #1
30006c12:	ebad 0d00 	sub.w	sp, sp, r0
30006c16:	603b      	str	r3, [r7, #0]
30006c18:	4648      	mov	r0, r9
30006c1a:	4b3f      	ldr	r3, [pc, #252]	; (30006d18 <password_hash_check+0x13c>)
30006c1c:	f10d 0a10 	add.w	sl, sp, #16
30006c20:	4798      	blx	r3
30006c22:	683b      	ldr	r3, [r7, #0]
30006c24:	2100      	movs	r1, #0
30006c26:	4650      	mov	r0, sl
30006c28:	461a      	mov	r2, r3
30006c2a:	4b3b      	ldr	r3, [pc, #236]	; (30006d18 <password_hash_check+0x13c>)
30006c2c:	4798      	blx	r3
30006c2e:	f1b8 0001 	subs.w	r0, r8, #1
30006c32:	d01d      	beq.n	30006c70 <password_hash_check+0x94>
30006c34:	2200      	movs	r2, #0
30006c36:	f109 33ff 	add.w	r3, r9, #4294967295
30006c3a:	4611      	mov	r1, r2
30006c3c:	e00b      	b.n	30006c56 <password_hash_check+0x7a>
30006c3e:	f108 38ff 	add.w	r8, r8, #4294967295
30006c42:	2100      	movs	r1, #0
30006c44:	1c9c      	adds	r4, r3, #2
30006c46:	4282      	cmp	r2, r0
30006c48:	7059      	strb	r1, [r3, #1]
30006c4a:	f103 0301 	add.w	r3, r3, #1
30006c4e:	4611      	mov	r1, r2
30006c50:	eba4 0409 	sub.w	r4, r4, r9
30006c54:	d20b      	bcs.n	30006c6e <password_hash_check+0x92>
30006c56:	1874      	adds	r4, r6, r1
30006c58:	f102 0c01 	add.w	ip, r2, #1
30006c5c:	5c71      	ldrb	r1, [r6, r1]
30006c5e:	3202      	adds	r2, #2
30006c60:	7864      	ldrb	r4, [r4, #1]
30006c62:	2c30      	cmp	r4, #48	; 0x30
30006c64:	d101      	bne.n	30006c6a <password_hash_check+0x8e>
30006c66:	295c      	cmp	r1, #92	; 0x5c
30006c68:	d0e9      	beq.n	30006c3e <password_hash_check+0x62>
30006c6a:	4662      	mov	r2, ip
30006c6c:	e7ea      	b.n	30006c44 <password_hash_check+0x68>
30006c6e:	4406      	add	r6, r0
30006c70:	7833      	ldrb	r3, [r6, #0]
30006c72:	f809 3004 	strb.w	r3, [r9, r4]
30006c76:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30006c78:	1e5c      	subs	r4, r3, #1
30006c7a:	d04a      	beq.n	30006d12 <password_hash_check+0x136>
30006c7c:	2200      	movs	r2, #0
30006c7e:	f10a 33ff 	add.w	r3, sl, #4294967295
30006c82:	4611      	mov	r1, r2
30006c84:	e00b      	b.n	30006c9e <password_hash_check+0xc2>
30006c86:	f10b 3bff 	add.w	fp, fp, #4294967295
30006c8a:	2100      	movs	r1, #0
30006c8c:	1c98      	adds	r0, r3, #2
30006c8e:	42a2      	cmp	r2, r4
30006c90:	7059      	strb	r1, [r3, #1]
30006c92:	f103 0301 	add.w	r3, r3, #1
30006c96:	4611      	mov	r1, r2
30006c98:	eba0 000a 	sub.w	r0, r0, sl
30006c9c:	d20a      	bcs.n	30006cb4 <password_hash_check+0xd8>
30006c9e:	1868      	adds	r0, r5, r1
30006ca0:	1c56      	adds	r6, r2, #1
30006ca2:	5c69      	ldrb	r1, [r5, r1]
30006ca4:	3202      	adds	r2, #2
30006ca6:	7840      	ldrb	r0, [r0, #1]
30006ca8:	2830      	cmp	r0, #48	; 0x30
30006caa:	d101      	bne.n	30006cb0 <password_hash_check+0xd4>
30006cac:	295c      	cmp	r1, #92	; 0x5c
30006cae:	d0ea      	beq.n	30006c86 <password_hash_check+0xaa>
30006cb0:	4632      	mov	r2, r6
30006cb2:	e7eb      	b.n	30006c8c <password_hash_check+0xb0>
30006cb4:	4425      	add	r5, r4
30006cb6:	782b      	ldrb	r3, [r5, #0]
30006cb8:	f107 0408 	add.w	r4, r7, #8
30006cbc:	2520      	movs	r5, #32
30006cbe:	4641      	mov	r1, r8
30006cc0:	f80a 3000 	strb.w	r3, [sl, r0]
30006cc4:	4652      	mov	r2, sl
30006cc6:	687b      	ldr	r3, [r7, #4]
30006cc8:	4648      	mov	r0, r9
30006cca:	9402      	str	r4, [sp, #8]
30006ccc:	9300      	str	r3, [sp, #0]
30006cce:	465b      	mov	r3, fp
30006cd0:	9501      	str	r5, [sp, #4]
30006cd2:	f7ff fe95 	bl	30006a00 <BOOT_RAM_KeyDeriveFunc>
30006cd6:	4620      	mov	r0, r4
30006cd8:	462a      	mov	r2, r5
30006cda:	4910      	ldr	r1, [pc, #64]	; (30006d1c <password_hash_check+0x140>)
30006cdc:	f005 faf4 	bl	3000c2c8 <____wrap_memcmp_veneer>
30006ce0:	4604      	mov	r4, r0
30006ce2:	b968      	cbnz	r0, 30006d00 <password_hash_check+0x124>
30006ce4:	480e      	ldr	r0, [pc, #56]	; (30006d20 <password_hash_check+0x144>)
30006ce6:	f005 fb4f 	bl	3000c388 <__DiagPrintf_veneer>
30006cea:	462a      	mov	r2, r5
30006cec:	4621      	mov	r1, r4
30006cee:	f107 0008 	add.w	r0, r7, #8
30006cf2:	4b09      	ldr	r3, [pc, #36]	; (30006d18 <password_hash_check+0x13c>)
30006cf4:	4798      	blx	r3
30006cf6:	4620      	mov	r0, r4
30006cf8:	372c      	adds	r7, #44	; 0x2c
30006cfa:	46bd      	mov	sp, r7
30006cfc:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d00:	2401      	movs	r4, #1
30006d02:	4808      	ldr	r0, [pc, #32]	; (30006d24 <password_hash_check+0x148>)
30006d04:	f005 fb40 	bl	3000c388 <__DiagPrintf_veneer>
30006d08:	372c      	adds	r7, #44	; 0x2c
30006d0a:	4620      	mov	r0, r4
30006d0c:	46bd      	mov	sp, r7
30006d0e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006d12:	4620      	mov	r0, r4
30006d14:	e7cf      	b.n	30006cb6 <password_hash_check+0xda>
30006d16:	bf00      	nop
30006d18:	00012be5 	.word	0x00012be5
30006d1c:	3000ebf0 	.word	0x3000ebf0
30006d20:	3000cf30 	.word	0x3000cf30
30006d24:	3000cf18 	.word	0x3000cf18

30006d28 <BOOT_Share_Memory_Patch>:
30006d28:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30006d2c:	4817      	ldr	r0, [pc, #92]	; (30006d8c <BOOT_Share_Memory_Patch+0x64>)
30006d2e:	4918      	ldr	r1, [pc, #96]	; (30006d90 <BOOT_Share_Memory_Patch+0x68>)
30006d30:	b430      	push	{r4, r5}
30006d32:	6b13      	ldr	r3, [r2, #48]	; 0x30
30006d34:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30006d38:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30006d3c:	6313      	str	r3, [r2, #48]	; 0x30
30006d3e:	6823      	ldr	r3, [r4, #0]
30006d40:	6804      	ldr	r4, [r0, #0]
30006d42:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006d46:	6809      	ldr	r1, [r1, #0]
30006d48:	4423      	add	r3, r4
30006d4a:	6805      	ldr	r5, [r0, #0]
30006d4c:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30006d50:	440b      	add	r3, r1
30006d52:	4910      	ldr	r1, [pc, #64]	; (30006d94 <BOOT_Share_Memory_Patch+0x6c>)
30006d54:	6808      	ldr	r0, [r1, #0]
30006d56:	442b      	add	r3, r5
30006d58:	6b11      	ldr	r1, [r2, #48]	; 0x30
30006d5a:	4403      	add	r3, r0
30006d5c:	480e      	ldr	r0, [pc, #56]	; (30006d98 <BOOT_Share_Memory_Patch+0x70>)
30006d5e:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30006d62:	6311      	str	r1, [r2, #48]	; 0x30
30006d64:	6821      	ldr	r1, [r4, #0]
30006d66:	4a0d      	ldr	r2, [pc, #52]	; (30006d9c <BOOT_Share_Memory_Patch+0x74>)
30006d68:	6804      	ldr	r4, [r0, #0]
30006d6a:	440b      	add	r3, r1
30006d6c:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30006d70:	6811      	ldr	r1, [r2, #0]
30006d72:	4423      	add	r3, r4
30006d74:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30006d78:	6800      	ldr	r0, [r0, #0]
30006d7a:	440b      	add	r3, r1
30006d7c:	6812      	ldr	r2, [r2, #0]
30006d7e:	4908      	ldr	r1, [pc, #32]	; (30006da0 <BOOT_Share_Memory_Patch+0x78>)
30006d80:	4403      	add	r3, r0
30006d82:	6808      	ldr	r0, [r1, #0]
30006d84:	4413      	add	r3, r2
30006d86:	bc30      	pop	{r4, r5}
30006d88:	4418      	add	r0, r3
30006d8a:	4770      	bx	lr
30006d8c:	2200c000 	.word	0x2200c000
30006d90:	22018000 	.word	0x22018000
30006d94:	22100000 	.word	0x22100000
30006d98:	20010000 	.word	0x20010000
30006d9c:	20020000 	.word	0x20020000
30006da0:	23010000 	.word	0x23010000

30006da4 <BOOT_Image1>:
30006da4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006da8:	4ba7      	ldr	r3, [pc, #668]	; (30007048 <BOOT_Image1+0x2a4>)
30006daa:	b087      	sub	sp, #28
30006dac:	4798      	blx	r3
30006dae:	2800      	cmp	r0, #0
30006db0:	f040 82be 	bne.w	30007330 <BOOT_Image1+0x58c>
30006db4:	48a5      	ldr	r0, [pc, #660]	; (3000704c <BOOT_Image1+0x2a8>)
30006db6:	f005 fae7 	bl	3000c388 <__DiagPrintf_veneer>
30006dba:	49a5      	ldr	r1, [pc, #660]	; (30007050 <BOOT_Image1+0x2ac>)
30006dbc:	48a5      	ldr	r0, [pc, #660]	; (30007054 <BOOT_Image1+0x2b0>)
30006dbe:	f005 fae3 	bl	3000c388 <__DiagPrintf_veneer>
30006dc2:	48a5      	ldr	r0, [pc, #660]	; (30007058 <BOOT_Image1+0x2b4>)
30006dc4:	4aa5      	ldr	r2, [pc, #660]	; (3000705c <BOOT_Image1+0x2b8>)
30006dc6:	2100      	movs	r1, #0
30006dc8:	4ba5      	ldr	r3, [pc, #660]	; (30007060 <BOOT_Image1+0x2bc>)
30006dca:	1a12      	subs	r2, r2, r0
30006dcc:	4798      	blx	r3
30006dce:	4aa5      	ldr	r2, [pc, #660]	; (30007064 <BOOT_Image1+0x2c0>)
30006dd0:	49a5      	ldr	r1, [pc, #660]	; (30007068 <BOOT_Image1+0x2c4>)
30006dd2:	8c13      	ldrh	r3, [r2, #32]
30006dd4:	b29b      	uxth	r3, r3
30006dd6:	8413      	strh	r3, [r2, #32]
30006dd8:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30006ddc:	f002 fa96 	bl	3000930c <BOOT_Reason>
30006de0:	2800      	cmp	r0, #0
30006de2:	f000 82d3 	beq.w	3000738c <BOOT_Image1+0x5e8>
30006de6:	4da1      	ldr	r5, [pc, #644]	; (3000706c <BOOT_Image1+0x2c8>)
30006de8:	4ba1      	ldr	r3, [pc, #644]	; (30007070 <BOOT_Image1+0x2cc>)
30006dea:	4798      	blx	r3
30006dec:	782c      	ldrb	r4, [r5, #0]
30006dee:	2c00      	cmp	r4, #0
30006df0:	f000 8294 	beq.w	3000731c <BOOT_Image1+0x578>
30006df4:	2000      	movs	r0, #0
30006df6:	4b9f      	ldr	r3, [pc, #636]	; (30007074 <BOOT_Image1+0x2d0>)
30006df8:	4798      	blx	r3
30006dfa:	4b9f      	ldr	r3, [pc, #636]	; (30007078 <BOOT_Image1+0x2d4>)
30006dfc:	2001      	movs	r0, #1
30006dfe:	4798      	blx	r3
30006e00:	4c99      	ldr	r4, [pc, #612]	; (30007068 <BOOT_Image1+0x2c4>)
30006e02:	f7ff fca5 	bl	30006750 <BOOT_SOC_ClkSet>
30006e06:	f241 070a 	movw	r7, #4106	; 0x100a
30006e0a:	2028      	movs	r0, #40	; 0x28
30006e0c:	8823      	ldrh	r3, [r4, #0]
30006e0e:	4e9b      	ldr	r6, [pc, #620]	; (3000707c <BOOT_Image1+0x2d8>)
30006e10:	b29b      	uxth	r3, r3
30006e12:	431f      	orrs	r7, r3
30006e14:	4b9a      	ldr	r3, [pc, #616]	; (30007080 <BOOT_Image1+0x2dc>)
30006e16:	8027      	strh	r7, [r4, #0]
30006e18:	4798      	blx	r3
30006e1a:	88a3      	ldrh	r3, [r4, #4]
30006e1c:	b29b      	uxth	r3, r3
30006e1e:	ea23 0307 	bic.w	r3, r3, r7
30006e22:	80a3      	strh	r3, [r4, #4]
30006e24:	8863      	ldrh	r3, [r4, #2]
30006e26:	8067      	strh	r7, [r4, #2]
30006e28:	47b0      	blx	r6
30006e2a:	2802      	cmp	r0, #2
30006e2c:	f000 8295 	beq.w	3000735a <BOOT_Image1+0x5b6>
30006e30:	4b94      	ldr	r3, [pc, #592]	; (30007084 <BOOT_Image1+0x2e0>)
30006e32:	2401      	movs	r4, #1
30006e34:	4994      	ldr	r1, [pc, #592]	; (30007088 <BOOT_Image1+0x2e4>)
30006e36:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30006e3a:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30006e3e:	4622      	mov	r2, r4
30006e40:	4f92      	ldr	r7, [pc, #584]	; (3000708c <BOOT_Image1+0x2e8>)
30006e42:	47b8      	blx	r7
30006e44:	4622      	mov	r2, r4
30006e46:	4992      	ldr	r1, [pc, #584]	; (30007090 <BOOT_Image1+0x2ec>)
30006e48:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30006e4c:	47b8      	blx	r7
30006e4e:	4a85      	ldr	r2, [pc, #532]	; (30007064 <BOOT_Image1+0x2c0>)
30006e50:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30006e54:	f043 0303 	orr.w	r3, r3, #3
30006e58:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30006e5c:	4b8d      	ldr	r3, [pc, #564]	; (30007094 <BOOT_Image1+0x2f0>)
30006e5e:	4798      	blx	r3
30006e60:	4a80      	ldr	r2, [pc, #512]	; (30007064 <BOOT_Image1+0x2c0>)
30006e62:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30006e66:	f023 0303 	bic.w	r3, r3, #3
30006e6a:	b108      	cbz	r0, 30006e70 <BOOT_Image1+0xcc>
30006e6c:	f043 0302 	orr.w	r3, r3, #2
30006e70:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30006e74:	4b88      	ldr	r3, [pc, #544]	; (30007098 <BOOT_Image1+0x2f4>)
30006e76:	781b      	ldrb	r3, [r3, #0]
30006e78:	2b00      	cmp	r3, #0
30006e7a:	f040 8294 	bne.w	300073a6 <BOOT_Image1+0x602>
30006e7e:	4c87      	ldr	r4, [pc, #540]	; (3000709c <BOOT_Image1+0x2f8>)
30006e80:	2201      	movs	r2, #1
30006e82:	2102      	movs	r1, #2
30006e84:	4886      	ldr	r0, [pc, #536]	; (300070a0 <BOOT_Image1+0x2fc>)
30006e86:	47a0      	blx	r4
30006e88:	4c86      	ldr	r4, [pc, #536]	; (300070a4 <BOOT_Image1+0x300>)
30006e8a:	68a3      	ldr	r3, [r4, #8]
30006e8c:	079f      	lsls	r7, r3, #30
30006e8e:	f100 8284 	bmi.w	3000739a <BOOT_Image1+0x5f6>
30006e92:	4985      	ldr	r1, [pc, #532]	; (300070a8 <BOOT_Image1+0x304>)
30006e94:	e841 f100 	tt	r1, r1
30006e98:	68a3      	ldr	r3, [r4, #8]
30006e9a:	f3c1 5180 	ubfx	r1, r1, #22, #1
30006e9e:	0798      	lsls	r0, r3, #30
30006ea0:	f100 8270 	bmi.w	30007384 <BOOT_Image1+0x5e0>
30006ea4:	4b81      	ldr	r3, [pc, #516]	; (300070ac <BOOT_Image1+0x308>)
30006ea6:	4f82      	ldr	r7, [pc, #520]	; (300070b0 <BOOT_Image1+0x30c>)
30006ea8:	681b      	ldr	r3, [r3, #0]
30006eaa:	4798      	blx	r3
30006eac:	f7fe fa56 	bl	3000535c <BOOT_RccConfig>
30006eb0:	f7fe fac0 	bl	30005434 <BOOT_CACHEWRR_Set>
30006eb4:	4b7f      	ldr	r3, [pc, #508]	; (300070b4 <BOOT_Image1+0x310>)
30006eb6:	6818      	ldr	r0, [r3, #0]
30006eb8:	f7fe faea 	bl	30005490 <BOOT_TCMSet>
30006ebc:	4b7e      	ldr	r3, [pc, #504]	; (300070b8 <BOOT_Image1+0x314>)
30006ebe:	695b      	ldr	r3, [r3, #20]
30006ec0:	03da      	lsls	r2, r3, #15
30006ec2:	d513      	bpl.n	30006eec <BOOT_Image1+0x148>
30006ec4:	f017 011f 	ands.w	r1, r7, #31
30006ec8:	f040 8243 	bne.w	30007352 <BOOT_Image1+0x5ae>
30006ecc:	463b      	mov	r3, r7
30006ece:	2174      	movs	r1, #116	; 0x74
30006ed0:	f3bf 8f4f 	dsb	sy
30006ed4:	4878      	ldr	r0, [pc, #480]	; (300070b8 <BOOT_Image1+0x314>)
30006ed6:	4419      	add	r1, r3
30006ed8:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30006edc:	3320      	adds	r3, #32
30006ede:	1aca      	subs	r2, r1, r3
30006ee0:	2a00      	cmp	r2, #0
30006ee2:	dcf9      	bgt.n	30006ed8 <BOOT_Image1+0x134>
30006ee4:	f3bf 8f4f 	dsb	sy
30006ee8:	f3bf 8f6f 	isb	sy
30006eec:	4b5d      	ldr	r3, [pc, #372]	; (30007064 <BOOT_Image1+0x2c0>)
30006eee:	2200      	movs	r2, #0
30006ef0:	f8c3 7270 	str.w	r7, [r3, #624]	; 0x270
30006ef4:	4b63      	ldr	r3, [pc, #396]	; (30007084 <BOOT_Image1+0x2e0>)
30006ef6:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30006efa:	f7fc fbe1 	bl	300036c0 <flash_highspeed_setup>
30006efe:	47b0      	blx	r6
30006f00:	2802      	cmp	r0, #2
30006f02:	f000 82b5 	beq.w	30007470 <BOOT_Image1+0x6cc>
30006f06:	782b      	ldrb	r3, [r5, #0]
30006f08:	2b00      	cmp	r3, #0
30006f0a:	f040 8215 	bne.w	30007338 <BOOT_Image1+0x594>
30006f0e:	68a3      	ldr	r3, [r4, #8]
30006f10:	079b      	lsls	r3, r3, #30
30006f12:	f100 833a 	bmi.w	3000758a <BOOT_Image1+0x7e6>
30006f16:	f7fe fc53 	bl	300057c0 <BOOT_DDR_Init>
30006f1a:	4a52      	ldr	r2, [pc, #328]	; (30007064 <BOOT_Image1+0x2c0>)
30006f1c:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30006f20:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30006f24:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30006f28:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
30006f2c:	4b4d      	ldr	r3, [pc, #308]	; (30007064 <BOOT_Image1+0x2c0>)
30006f2e:	2200      	movs	r2, #0
30006f30:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30006f34:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30006f38:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30006f3c:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30006f40:	f7ff fef2 	bl	30006d28 <BOOT_Share_Memory_Patch>
30006f44:	4b5d      	ldr	r3, [pc, #372]	; (300070bc <BOOT_Image1+0x318>)
30006f46:	781b      	ldrb	r3, [r3, #0]
30006f48:	2b00      	cmp	r3, #0
30006f4a:	f040 8304 	bne.w	30007556 <BOOT_Image1+0x7b2>
30006f4e:	6823      	ldr	r3, [r4, #0]
30006f50:	f043 0302 	orr.w	r3, r3, #2
30006f54:	6023      	str	r3, [r4, #0]
30006f56:	4b5a      	ldr	r3, [pc, #360]	; (300070c0 <BOOT_Image1+0x31c>)
30006f58:	4798      	blx	r3
30006f5a:	2801      	cmp	r0, #1
30006f5c:	f04f 0100 	mov.w	r1, #0
30006f60:	f04f 0002 	mov.w	r0, #2
30006f64:	bf0c      	ite	eq
30006f66:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30006f6a:	4a56      	ldrne	r2, [pc, #344]	; (300070c4 <BOOT_Image1+0x320>)
30006f6c:	f000 fd98 	bl	30007aa0 <ymodem_uart_port_init>
30006f70:	4b53      	ldr	r3, [pc, #332]	; (300070c0 <BOOT_Image1+0x31c>)
30006f72:	4798      	blx	r3
30006f74:	2800      	cmp	r0, #0
30006f76:	f000 8230 	beq.w	300073da <BOOT_Image1+0x636>
30006f7a:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30006f7e:	4852      	ldr	r0, [pc, #328]	; (300070c8 <BOOT_Image1+0x324>)
30006f80:	f005 fa02 	bl	3000c388 <__DiagPrintf_veneer>
30006f84:	4851      	ldr	r0, [pc, #324]	; (300070cc <BOOT_Image1+0x328>)
30006f86:	2564      	movs	r5, #100	; 0x64
30006f88:	f005 fa26 	bl	3000c3d8 <____wrap_strlen_veneer>
30006f8c:	4601      	mov	r1, r0
30006f8e:	484f      	ldr	r0, [pc, #316]	; (300070cc <BOOT_Image1+0x328>)
30006f90:	4c4f      	ldr	r4, [pc, #316]	; (300070d0 <BOOT_Image1+0x32c>)
30006f92:	f000 fddf 	bl	30007b54 <ymodem_uart_putdata>
30006f96:	e002      	b.n	30006f9e <BOOT_Image1+0x1fa>
30006f98:	3d01      	subs	r5, #1
30006f9a:	f000 82fa 	beq.w	30007592 <BOOT_Image1+0x7ee>
30006f9e:	2001      	movs	r0, #1
30006fa0:	47a0      	blx	r4
30006fa2:	f000 fdbd 	bl	30007b20 <ymodem_uart_readable>
30006fa6:	2800      	cmp	r0, #0
30006fa8:	d0f6      	beq.n	30006f98 <BOOT_Image1+0x1f4>
30006faa:	f000 fdb9 	bl	30007b20 <ymodem_uart_readable>
30006fae:	2800      	cmp	r0, #0
30006fb0:	f000 82f5 	beq.w	3000759e <BOOT_Image1+0x7fa>
30006fb4:	2301      	movs	r3, #1
30006fb6:	9303      	str	r3, [sp, #12]
30006fb8:	2001      	movs	r0, #1
30006fba:	47a0      	blx	r4
30006fbc:	2108      	movs	r1, #8
30006fbe:	4845      	ldr	r0, [pc, #276]	; (300070d4 <BOOT_Image1+0x330>)
30006fc0:	f000 fdd6 	bl	30007b70 <ymodem_uart_getdata>
30006fc4:	4943      	ldr	r1, [pc, #268]	; (300070d4 <BOOT_Image1+0x330>)
30006fc6:	2208      	movs	r2, #8
30006fc8:	4b43      	ldr	r3, [pc, #268]	; (300070d8 <BOOT_Image1+0x334>)
30006fca:	f101 0080 	add.w	r0, r1, #128	; 0x80
30006fce:	4798      	blx	r3
30006fd0:	4605      	mov	r5, r0
30006fd2:	2800      	cmp	r0, #0
30006fd4:	f040 8357 	bne.w	30007686 <BOOT_Image1+0x8e2>
30006fd8:	f000 fda2 	bl	30007b20 <ymodem_uart_readable>
30006fdc:	2800      	cmp	r0, #0
30006fde:	d0fb      	beq.n	30006fd8 <BOOT_Image1+0x234>
30006fe0:	2101      	movs	r1, #1
30006fe2:	483e      	ldr	r0, [pc, #248]	; (300070dc <BOOT_Image1+0x338>)
30006fe4:	f000 fdc4 	bl	30007b70 <ymodem_uart_getdata>
30006fe8:	f000 fd9a 	bl	30007b20 <ymodem_uart_readable>
30006fec:	2800      	cmp	r0, #0
30006fee:	d0fb      	beq.n	30006fe8 <BOOT_Image1+0x244>
30006ff0:	4f3b      	ldr	r7, [pc, #236]	; (300070e0 <BOOT_Image1+0x33c>)
30006ff2:	2101      	movs	r1, #1
30006ff4:	f207 4051 	addw	r0, r7, #1105	; 0x451
30006ff8:	f000 fdba 	bl	30007b70 <ymodem_uart_getdata>
30006ffc:	f897 2451 	ldrb.w	r2, [r7, #1105]	; 0x451
30007000:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30007004:	429a      	cmp	r2, r3
30007006:	f000 834f 	beq.w	300076a8 <BOOT_Image1+0x904>
3000700a:	f207 4952 	addw	r9, r7, #1106	; 0x452
3000700e:	2600      	movs	r6, #0
30007010:	46b0      	mov	r8, r6
30007012:	3601      	adds	r6, #1
30007014:	f000 fd84 	bl	30007b20 <ymodem_uart_readable>
30007018:	2800      	cmp	r0, #0
3000701a:	d0fb      	beq.n	30007014 <BOOT_Image1+0x270>
3000701c:	4648      	mov	r0, r9
3000701e:	2101      	movs	r1, #1
30007020:	f000 fda6 	bl	30007b70 <ymodem_uart_getdata>
30007024:	f899 2000 	ldrb.w	r2, [r9]
30007028:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
3000702c:	f108 080b 	add.w	r8, r8, #11
30007030:	f109 0901 	add.w	r9, r9, #1
30007034:	46b3      	mov	fp, r6
30007036:	429a      	cmp	r2, r3
30007038:	d1ea      	bne.n	30007010 <BOOT_Image1+0x26c>
3000703a:	2e00      	cmp	r6, #0
3000703c:	dd61      	ble.n	30007102 <BOOT_Image1+0x35e>
3000703e:	f507 618a 	add.w	r1, r7, #1104	; 0x450
30007042:	2300      	movs	r3, #0
30007044:	1988      	adds	r0, r1, r6
30007046:	e04f      	b.n	300070e8 <BOOT_Image1+0x344>
30007048:	0000c149 	.word	0x0000c149
3000704c:	3000cf4c 	.word	0x3000cf4c
30007050:	3000e338 	.word	0x3000e338
30007054:	3000cf70 	.word	0x3000cf70
30007058:	3000fba0 	.word	0x3000fba0
3000705c:	30010044 	.word	0x30010044
30007060:	00012be5 	.word	0x00012be5
30007064:	42008000 	.word	0x42008000
30007068:	42008200 	.word	0x42008200
3000706c:	3000e622 	.word	0x3000e622
30007070:	3000b505 	.word	0x3000b505
30007074:	3000b375 	.word	0x3000b375
30007078:	3000b295 	.word	0x3000b295
3000707c:	30007d51 	.word	0x30007d51
30007080:	00009b2d 	.word	0x00009b2d
30007084:	23020000 	.word	0x23020000
30007088:	40080000 	.word	0x40080000
3000708c:	0000b479 	.word	0x0000b479
30007090:	40040000 	.word	0x40040000
30007094:	30009ba9 	.word	0x30009ba9
30007098:	3000e621 	.word	0x3000e621
3000709c:	0000aab9 	.word	0x0000aab9
300070a0:	4200c000 	.word	0x4200c000
300070a4:	2001c00c 	.word	0x2001c00c
300070a8:	30006da5 	.word	0x30006da5
300070ac:	00033000 	.word	0x00033000
300070b0:	2001c01c 	.word	0x2001c01c
300070b4:	3000e624 	.word	0x3000e624
300070b8:	e000ed00 	.word	0xe000ed00
300070bc:	3000e620 	.word	0x3000e620
300070c0:	0000c169 	.word	0x0000c169
300070c4:	0016e360 	.word	0x0016e360
300070c8:	3000d078 	.word	0x3000d078
300070cc:	3000ec10 	.word	0x3000ec10
300070d0:	00009be5 	.word	0x00009be5
300070d4:	3000ec30 	.word	0x3000ec30
300070d8:	000129bd 	.word	0x000129bd
300070dc:	3000ec38 	.word	0x3000ec38
300070e0:	3000e7e8 	.word	0x3000e7e8
300070e4:	4288      	cmp	r0, r1
300070e6:	d00d      	beq.n	30007104 <BOOT_Image1+0x360>
300070e8:	f811 2f01 	ldrb.w	r2, [r1, #1]!
300070ec:	eb03 0383 	add.w	r3, r3, r3, lsl #2
300070f0:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
300070f4:	eb02 0343 	add.w	r3, r2, r3, lsl #1
300070f8:	f1bc 0f09 	cmp.w	ip, #9
300070fc:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30007100:	d9f0      	bls.n	300070e4 <BOOT_Image1+0x340>
30007102:	2300      	movs	r3, #0
30007104:	f8c7 34d4 	str.w	r3, [r7, #1236]	; 0x4d4
30007108:	f000 fd0a 	bl	30007b20 <ymodem_uart_readable>
3000710c:	2800      	cmp	r0, #0
3000710e:	d0fb      	beq.n	30007108 <BOOT_Image1+0x364>
30007110:	49b4      	ldr	r1, [pc, #720]	; (300073e4 <BOOT_Image1+0x640>)
30007112:	460b      	mov	r3, r1
30007114:	2101      	movs	r1, #1
30007116:	eb03 0008 	add.w	r0, r3, r8
3000711a:	9302      	str	r3, [sp, #8]
3000711c:	f000 fd28 	bl	30007b70 <ymodem_uart_getdata>
30007120:	9902      	ldr	r1, [sp, #8]
30007122:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30007126:	f811 2008 	ldrb.w	r2, [r1, r8]
3000712a:	429a      	cmp	r2, r3
3000712c:	f000 82b8 	beq.w	300076a0 <BOOT_Image1+0x8fc>
30007130:	f10b 080b 	add.w	r8, fp, #11
30007134:	f04f 0900 	mov.w	r9, #0
30007138:	4488      	add	r8, r1
3000713a:	f109 0901 	add.w	r9, r9, #1
3000713e:	f000 fcef 	bl	30007b20 <ymodem_uart_readable>
30007142:	2800      	cmp	r0, #0
30007144:	d0fb      	beq.n	3000713e <BOOT_Image1+0x39a>
30007146:	4640      	mov	r0, r8
30007148:	2101      	movs	r1, #1
3000714a:	f000 fd11 	bl	30007b70 <ymodem_uart_getdata>
3000714e:	f898 2000 	ldrb.w	r2, [r8]
30007152:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30007156:	f108 0801 	add.w	r8, r8, #1
3000715a:	eb09 0a0b 	add.w	sl, r9, fp
3000715e:	429a      	cmp	r2, r3
30007160:	d1eb      	bne.n	3000713a <BOOT_Image1+0x396>
30007162:	f000 fcdd 	bl	30007b20 <ymodem_uart_readable>
30007166:	2800      	cmp	r0, #0
30007168:	d0fb      	beq.n	30007162 <BOOT_Image1+0x3be>
3000716a:	9b02      	ldr	r3, [sp, #8]
3000716c:	f10a 080b 	add.w	r8, sl, #11
30007170:	2101      	movs	r1, #1
30007172:	eb03 0008 	add.w	r0, r3, r8
30007176:	f000 fcfb 	bl	30007b70 <ymodem_uart_getdata>
3000717a:	9902      	ldr	r1, [sp, #8]
3000717c:	f897 24d1 	ldrb.w	r2, [r7, #1233]	; 0x4d1
30007180:	f811 3008 	ldrb.w	r3, [r1, r8]
30007184:	429a      	cmp	r2, r3
30007186:	f000 8289 	beq.w	3000769c <BOOT_Image1+0x8f8>
3000718a:	f10a 080c 	add.w	r8, sl, #12
3000718e:	4488      	add	r8, r1
30007190:	3501      	adds	r5, #1
30007192:	f000 fcc5 	bl	30007b20 <ymodem_uart_readable>
30007196:	2800      	cmp	r0, #0
30007198:	d0fb      	beq.n	30007192 <BOOT_Image1+0x3ee>
3000719a:	4640      	mov	r0, r8
3000719c:	2101      	movs	r1, #1
3000719e:	f000 fce7 	bl	30007b70 <ymodem_uart_getdata>
300071a2:	f898 2000 	ldrb.w	r2, [r8]
300071a6:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
300071aa:	f108 0801 	add.w	r8, r8, #1
300071ae:	eb05 0b0a 	add.w	fp, r5, sl
300071b2:	429a      	cmp	r2, r3
300071b4:	d1ec      	bne.n	30007190 <BOOT_Image1+0x3ec>
300071b6:	9b02      	ldr	r3, [sp, #8]
300071b8:	f10b 080c 	add.w	r8, fp, #12
300071bc:	2101      	movs	r1, #1
300071be:	eb03 0008 	add.w	r0, r3, r8
300071c2:	f000 fcd5 	bl	30007b70 <ymodem_uart_getdata>
300071c6:	9b02      	ldr	r3, [sp, #8]
300071c8:	f813 1008 	ldrb.w	r1, [r3, r8]
300071cc:	290a      	cmp	r1, #10
300071ce:	f8c7 14d8 	str.w	r1, [r7, #1240]	; 0x4d8
300071d2:	f200 8254 	bhi.w	3000767e <BOOT_Image1+0x8da>
300071d6:	9b02      	ldr	r3, [sp, #8]
300071d8:	f10b 000d 	add.w	r0, fp, #13
300071dc:	2101      	movs	r1, #1
300071de:	f10b 080e 	add.w	r8, fp, #14
300071e2:	4418      	add	r0, r3
300071e4:	f000 fcc4 	bl	30007b70 <ymodem_uart_getdata>
300071e8:	9b02      	ldr	r3, [sp, #8]
300071ea:	2102      	movs	r1, #2
300071ec:	4498      	add	r8, r3
300071ee:	4640      	mov	r0, r8
300071f0:	f000 fcbe 	bl	30007b70 <ymodem_uart_getdata>
300071f4:	4641      	mov	r1, r8
300071f6:	2202      	movs	r2, #2
300071f8:	487b      	ldr	r0, [pc, #492]	; (300073e8 <BOOT_Image1+0x644>)
300071fa:	4b7c      	ldr	r3, [pc, #496]	; (300073ec <BOOT_Image1+0x648>)
300071fc:	4798      	blx	r3
300071fe:	2800      	cmp	r0, #0
30007200:	f040 8248 	bne.w	30007694 <BOOT_Image1+0x8f0>
30007204:	9b02      	ldr	r3, [sp, #8]
30007206:	f10b 0010 	add.w	r0, fp, #16
3000720a:	2101      	movs	r1, #1
3000720c:	f10b 0811 	add.w	r8, fp, #17
30007210:	4418      	add	r0, r3
30007212:	f000 fcad 	bl	30007b70 <ymodem_uart_getdata>
30007216:	4b76      	ldr	r3, [pc, #472]	; (300073f0 <BOOT_Image1+0x64c>)
30007218:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000721c:	f103 0228 	add.w	r2, r3, #40	; 0x28
30007220:	f103 0130 	add.w	r1, r3, #48	; 0x30
30007224:	f7fd fd62 	bl	30004cec <hotfix_get_addr_size>
30007228:	9b02      	ldr	r3, [sp, #8]
3000722a:	2102      	movs	r1, #2
3000722c:	4498      	add	r8, r3
3000722e:	4640      	mov	r0, r8
30007230:	f000 fc9e 	bl	30007b70 <ymodem_uart_getdata>
30007234:	4641      	mov	r1, r8
30007236:	2202      	movs	r2, #2
30007238:	486e      	ldr	r0, [pc, #440]	; (300073f4 <BOOT_Image1+0x650>)
3000723a:	4b6c      	ldr	r3, [pc, #432]	; (300073ec <BOOT_Image1+0x648>)
3000723c:	4798      	blx	r3
3000723e:	2800      	cmp	r0, #0
30007240:	f000 8137 	beq.w	300074b2 <BOOT_Image1+0x70e>
30007244:	9b03      	ldr	r3, [sp, #12]
30007246:	2b00      	cmp	r3, #0
30007248:	f000 817e 	beq.w	30007548 <BOOT_Image1+0x7a4>
3000724c:	f004 fe32 	bl	3000beb4 <BOOT_VerCheck>
30007250:	f7fe fcb4 	bl	30005bbc <BOOT_LoadImages>
30007254:	2800      	cmp	r0, #0
30007256:	f000 8125 	beq.w	300074a4 <BOOT_Image1+0x700>
3000725a:	f7ff fa37 	bl	300066cc <BOOT_Enable_KM0>
3000725e:	2200      	movs	r2, #0
30007260:	2101      	movs	r1, #1
30007262:	4b65      	ldr	r3, [pc, #404]	; (300073f8 <BOOT_Image1+0x654>)
30007264:	4865      	ldr	r0, [pc, #404]	; (300073fc <BOOT_Image1+0x658>)
30007266:	4798      	blx	r3
30007268:	4b65      	ldr	r3, [pc, #404]	; (30007400 <BOOT_Image1+0x65c>)
3000726a:	2008      	movs	r0, #8
3000726c:	4798      	blx	r3
3000726e:	f7fd fc4f 	bl	30004b10 <BOOT_RAM_TZCfg>
30007272:	4b64      	ldr	r3, [pc, #400]	; (30007404 <BOOT_Image1+0x660>)
30007274:	781b      	ldrb	r3, [r3, #0]
30007276:	2b01      	cmp	r3, #1
30007278:	f000 81f1 	beq.w	3000765e <BOOT_Image1+0x8ba>
3000727c:	4b62      	ldr	r3, [pc, #392]	; (30007408 <BOOT_Image1+0x664>)
3000727e:	781b      	ldrb	r3, [r3, #0]
30007280:	2b00      	cmp	r3, #0
30007282:	f000 8172 	beq.w	3000756a <BOOT_Image1+0x7c6>
30007286:	f7ff fb51 	bl	3000692c <BOOT_Enable_AP>
3000728a:	4a60      	ldr	r2, [pc, #384]	; (3000740c <BOOT_Image1+0x668>)
3000728c:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30007290:	f043 0306 	orr.w	r3, r3, #6
30007294:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
30007298:	4b5a      	ldr	r3, [pc, #360]	; (30007404 <BOOT_Image1+0x660>)
3000729a:	781b      	ldrb	r3, [r3, #0]
3000729c:	2b01      	cmp	r3, #1
3000729e:	f000 81e6 	beq.w	3000766e <BOOT_Image1+0x8ca>
300072a2:	485a      	ldr	r0, [pc, #360]	; (3000740c <BOOT_Image1+0x668>)
300072a4:	495a      	ldr	r1, [pc, #360]	; (30007410 <BOOT_Image1+0x66c>)
300072a6:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
300072aa:	4a5a      	ldr	r2, [pc, #360]	; (30007414 <BOOT_Image1+0x670>)
300072ac:	f043 0301 	orr.w	r3, r3, #1
300072b0:	4d59      	ldr	r5, [pc, #356]	; (30007418 <BOOT_Image1+0x674>)
300072b2:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
300072b6:	688c      	ldr	r4, [r1, #8]
300072b8:	680b      	ldr	r3, [r1, #0]
300072ba:	4958      	ldr	r1, [pc, #352]	; (3000741c <BOOT_Image1+0x678>)
300072bc:	6063      	str	r3, [r4, #4]
300072be:	4b58      	ldr	r3, [pc, #352]	; (30007420 <BOOT_Image1+0x67c>)
300072c0:	6094      	str	r4, [r2, #8]
300072c2:	61e9      	str	r1, [r5, #28]
300072c4:	6a59      	ldr	r1, [r3, #36]	; 0x24
300072c6:	4857      	ldr	r0, [pc, #348]	; (30007424 <BOOT_Image1+0x680>)
300072c8:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300072cc:	6259      	str	r1, [r3, #36]	; 0x24
300072ce:	6a51      	ldr	r1, [r2, #36]	; 0x24
300072d0:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300072d4:	6251      	str	r1, [r2, #36]	; 0x24
300072d6:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300072da:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300072de:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300072e2:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300072e6:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300072ea:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300072ee:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
300072f2:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
300072f6:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
300072fa:	689b      	ldr	r3, [r3, #8]
300072fc:	6183      	str	r3, [r0, #24]
300072fe:	f7fe fbe5 	bl	30005acc <BOOT_NVICBackup_HP>
30007302:	4b49      	ldr	r3, [pc, #292]	; (30007428 <BOOT_Image1+0x684>)
30007304:	f383 8888 	msr	MSP_NS, r3
30007308:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
3000730c:	f383 8809 	msr	PSP, r3
30007310:	6860      	ldr	r0, [r4, #4]
30007312:	b007      	add	sp, #28
30007314:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007318:	f7fd bfd0 	b.w	300052bc <BOOT_NsStart>
3000731c:	4b43      	ldr	r3, [pc, #268]	; (3000742c <BOOT_Image1+0x688>)
3000731e:	2001      	movs	r0, #1
30007320:	4798      	blx	r3
30007322:	4620      	mov	r0, r4
30007324:	4b42      	ldr	r3, [pc, #264]	; (30007430 <BOOT_Image1+0x68c>)
30007326:	4798      	blx	r3
30007328:	4b42      	ldr	r3, [pc, #264]	; (30007434 <BOOT_Image1+0x690>)
3000732a:	2001      	movs	r0, #1
3000732c:	4798      	blx	r3
3000732e:	e567      	b.n	30006e00 <BOOT_Image1+0x5c>
30007330:	4841      	ldr	r0, [pc, #260]	; (30007438 <BOOT_Image1+0x694>)
30007332:	f005 f829 	bl	3000c388 <__DiagPrintf_veneer>
30007336:	e540      	b.n	30006dba <BOOT_Image1+0x16>
30007338:	4c40      	ldr	r4, [pc, #256]	; (3000743c <BOOT_Image1+0x698>)
3000733a:	4e41      	ldr	r6, [pc, #260]	; (30007440 <BOOT_Image1+0x69c>)
3000733c:	4d41      	ldr	r5, [pc, #260]	; (30007444 <BOOT_Image1+0x6a0>)
3000733e:	4633      	mov	r3, r6
30007340:	462a      	mov	r2, r5
30007342:	493f      	ldr	r1, [pc, #252]	; (30007440 <BOOT_Image1+0x69c>)
30007344:	2002      	movs	r0, #2
30007346:	f004 fd79 	bl	3000be3c <rtk_log_write>
3000734a:	f241 3088 	movw	r0, #5000	; 0x1388
3000734e:	47a0      	blx	r4
30007350:	e7f5      	b.n	3000733e <BOOT_Image1+0x59a>
30007352:	3174      	adds	r1, #116	; 0x74
30007354:	f027 031f 	bic.w	r3, r7, #31
30007358:	e5ba      	b.n	30006ed0 <BOOT_Image1+0x12c>
3000735a:	6de3      	ldr	r3, [r4, #92]	; 0x5c
3000735c:	2201      	movs	r2, #1
3000735e:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30007362:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30007366:	65e3      	str	r3, [r4, #92]	; 0x5c
30007368:	4b37      	ldr	r3, [pc, #220]	; (30007448 <BOOT_Image1+0x6a4>)
3000736a:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
3000736e:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30007372:	4b36      	ldr	r3, [pc, #216]	; (3000744c <BOOT_Image1+0x6a8>)
30007374:	4798      	blx	r3
30007376:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
3000737a:	f043 0302 	orr.w	r3, r3, #2
3000737e:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30007382:	e56b      	b.n	30006e5c <BOOT_Image1+0xb8>
30007384:	4832      	ldr	r0, [pc, #200]	; (30007450 <BOOT_Image1+0x6ac>)
30007386:	f004 ffff 	bl	3000c388 <__DiagPrintf_veneer>
3000738a:	e58b      	b.n	30006ea4 <BOOT_Image1+0x100>
3000738c:	4601      	mov	r1, r0
3000738e:	f44f 7201 	mov.w	r2, #516	; 0x204
30007392:	482d      	ldr	r0, [pc, #180]	; (30007448 <BOOT_Image1+0x6a4>)
30007394:	f004 ffd0 	bl	3000c338 <____wrap_memset_veneer>
30007398:	e525      	b.n	30006de6 <BOOT_Image1+0x42>
3000739a:	f3ef 8108 	mrs	r1, MSP
3000739e:	482d      	ldr	r0, [pc, #180]	; (30007454 <BOOT_Image1+0x6b0>)
300073a0:	f004 fff2 	bl	3000c388 <__DiagPrintf_veneer>
300073a4:	e575      	b.n	30006e92 <BOOT_Image1+0xee>
300073a6:	4f2c      	ldr	r7, [pc, #176]	; (30007458 <BOOT_Image1+0x6b4>)
300073a8:	47b8      	blx	r7
300073aa:	4604      	mov	r4, r0
300073ac:	47b8      	blx	r7
300073ae:	2c02      	cmp	r4, #2
300073b0:	f43f ad65 	beq.w	30006e7e <BOOT_Image1+0xda>
300073b4:	2803      	cmp	r0, #3
300073b6:	f43f ad62 	beq.w	30006e7e <BOOT_Image1+0xda>
300073ba:	4b28      	ldr	r3, [pc, #160]	; (3000745c <BOOT_Image1+0x6b8>)
300073bc:	4798      	blx	r3
300073be:	4c28      	ldr	r4, [pc, #160]	; (30007460 <BOOT_Image1+0x6bc>)
300073c0:	2200      	movs	r2, #0
300073c2:	2101      	movs	r1, #1
300073c4:	480d      	ldr	r0, [pc, #52]	; (300073fc <BOOT_Image1+0x658>)
300073c6:	47a0      	blx	r4
300073c8:	2101      	movs	r1, #1
300073ca:	480c      	ldr	r0, [pc, #48]	; (300073fc <BOOT_Image1+0x658>)
300073cc:	4b25      	ldr	r3, [pc, #148]	; (30007464 <BOOT_Image1+0x6c0>)
300073ce:	4798      	blx	r3
300073d0:	2201      	movs	r2, #1
300073d2:	480a      	ldr	r0, [pc, #40]	; (300073fc <BOOT_Image1+0x658>)
300073d4:	4611      	mov	r1, r2
300073d6:	47a0      	blx	r4
300073d8:	e552      	b.n	30006e80 <BOOT_Image1+0xdc>
300073da:	4923      	ldr	r1, [pc, #140]	; (30007468 <BOOT_Image1+0x6c4>)
300073dc:	4823      	ldr	r0, [pc, #140]	; (3000746c <BOOT_Image1+0x6c8>)
300073de:	f004 ffd3 	bl	3000c388 <__DiagPrintf_veneer>
300073e2:	e5cf      	b.n	30006f84 <BOOT_Image1+0x1e0>
300073e4:	3000ec30 	.word	0x3000ec30
300073e8:	3000ecc4 	.word	0x3000ecc4
300073ec:	000129bd 	.word	0x000129bd
300073f0:	3000ecc8 	.word	0x3000ecc8
300073f4:	3000ed04 	.word	0x3000ed04
300073f8:	0000ac6d 	.word	0x0000ac6d
300073fc:	4200c000 	.word	0x4200c000
30007400:	0000e1d9 	.word	0x0000e1d9
30007404:	3000e620 	.word	0x3000e620
30007408:	3000e623 	.word	0x3000e623
3000740c:	42008200 	.word	0x42008200
30007410:	60000020 	.word	0x60000020
30007414:	e002ed00 	.word	0xe002ed00
30007418:	30000000 	.word	0x30000000
3000741c:	30004cad 	.word	0x30004cad
30007420:	e000ed00 	.word	0xe000ed00
30007424:	3000ffa8 	.word	0x3000ffa8
30007428:	2001bffc 	.word	0x2001bffc
3000742c:	3000b295 	.word	0x3000b295
30007430:	3000b4c5 	.word	0x3000b4c5
30007434:	3000b435 	.word	0x3000b435
30007438:	3000cf5c 	.word	0x3000cf5c
3000743c:	00009be5 	.word	0x00009be5
30007440:	3000c674 	.word	0x3000c674
30007444:	3000d014 	.word	0x3000d014
30007448:	23020000 	.word	0x23020000
3000744c:	0000b479 	.word	0x0000b479
30007450:	3000cfb8 	.word	0x3000cfb8
30007454:	3000cf88 	.word	0x3000cf88
30007458:	0000c0f9 	.word	0x0000c0f9
3000745c:	0000aea1 	.word	0x0000aea1
30007460:	0000aab9 	.word	0x0000aab9
30007464:	0000aaa5 	.word	0x0000aaa5
30007468:	0016e360 	.word	0x0016e360
3000746c:	3000d078 	.word	0x3000d078
30007470:	4b93      	ldr	r3, [pc, #588]	; (300076c0 <BOOT_Image1+0x91c>)
30007472:	2004      	movs	r0, #4
30007474:	4a93      	ldr	r2, [pc, #588]	; (300076c4 <BOOT_Image1+0x920>)
30007476:	4619      	mov	r1, r3
30007478:	f004 fce0 	bl	3000be3c <rtk_log_write>
3000747c:	4b92      	ldr	r3, [pc, #584]	; (300076c8 <BOOT_Image1+0x924>)
3000747e:	4798      	blx	r3
30007480:	f7fe f9dc 	bl	3000583c <BOOT_PSRAM_Init>
30007484:	7828      	ldrb	r0, [r5, #0]
30007486:	2800      	cmp	r0, #0
30007488:	f47f ad50 	bne.w	30006f2c <BOOT_Image1+0x188>
3000748c:	4b8f      	ldr	r3, [pc, #572]	; (300076cc <BOOT_Image1+0x928>)
3000748e:	4798      	blx	r3
30007490:	2800      	cmp	r0, #0
30007492:	f43f ad4b 	beq.w	30006f2c <BOOT_Image1+0x188>
30007496:	4b8a      	ldr	r3, [pc, #552]	; (300076c0 <BOOT_Image1+0x91c>)
30007498:	2003      	movs	r0, #3
3000749a:	4a8d      	ldr	r2, [pc, #564]	; (300076d0 <BOOT_Image1+0x92c>)
3000749c:	4619      	mov	r1, r3
3000749e:	f004 fccd 	bl	3000be3c <rtk_log_write>
300074a2:	e543      	b.n	30006f2c <BOOT_Image1+0x188>
300074a4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300074a8:	47a0      	blx	r4
300074aa:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300074ae:	47a0      	blx	r4
300074b0:	e7f8      	b.n	300074a4 <BOOT_Image1+0x700>
300074b2:	9b02      	ldr	r3, [sp, #8]
300074b4:	f10b 0013 	add.w	r0, fp, #19
300074b8:	2101      	movs	r1, #1
300074ba:	f10b 0814 	add.w	r8, fp, #20
300074be:	4418      	add	r0, r3
300074c0:	f000 fb56 	bl	30007b70 <ymodem_uart_getdata>
300074c4:	4b83      	ldr	r3, [pc, #524]	; (300076d4 <BOOT_Image1+0x930>)
300074c6:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
300074ca:	f1a3 0218 	sub.w	r2, r3, #24
300074ce:	f1a3 0110 	sub.w	r1, r3, #16
300074d2:	f7fd fc0b 	bl	30004cec <hotfix_get_addr_size>
300074d6:	9b02      	ldr	r3, [sp, #8]
300074d8:	2102      	movs	r1, #2
300074da:	4498      	add	r8, r3
300074dc:	4640      	mov	r0, r8
300074de:	f000 fb47 	bl	30007b70 <ymodem_uart_getdata>
300074e2:	4641      	mov	r1, r8
300074e4:	2202      	movs	r2, #2
300074e6:	487c      	ldr	r0, [pc, #496]	; (300076d8 <BOOT_Image1+0x934>)
300074e8:	4b7c      	ldr	r3, [pc, #496]	; (300076dc <BOOT_Image1+0x938>)
300074ea:	4798      	blx	r3
300074ec:	2800      	cmp	r0, #0
300074ee:	f47f aea9 	bne.w	30007244 <BOOT_Image1+0x4a0>
300074f2:	9b02      	ldr	r3, [sp, #8]
300074f4:	f10b 0016 	add.w	r0, fp, #22
300074f8:	2101      	movs	r1, #1
300074fa:	f10b 0817 	add.w	r8, fp, #23
300074fe:	4418      	add	r0, r3
30007500:	f000 fb36 	bl	30007b70 <ymodem_uart_getdata>
30007504:	4b76      	ldr	r3, [pc, #472]	; (300076e0 <BOOT_Image1+0x93c>)
30007506:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000750a:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
3000750e:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
30007512:	f7fd fbeb 	bl	30004cec <hotfix_get_addr_size>
30007516:	9a02      	ldr	r2, [sp, #8]
30007518:	2102      	movs	r1, #2
3000751a:	4490      	add	r8, r2
3000751c:	4640      	mov	r0, r8
3000751e:	f000 fb27 	bl	30007b70 <ymodem_uart_getdata>
30007522:	4641      	mov	r1, r8
30007524:	2202      	movs	r2, #2
30007526:	486f      	ldr	r0, [pc, #444]	; (300076e4 <BOOT_Image1+0x940>)
30007528:	4b6c      	ldr	r3, [pc, #432]	; (300076dc <BOOT_Image1+0x938>)
3000752a:	4798      	blx	r3
3000752c:	2800      	cmp	r0, #0
3000752e:	f47f ae89 	bne.w	30007244 <BOOT_Image1+0x4a0>
30007532:	9b02      	ldr	r3, [sp, #8]
30007534:	f10b 0019 	add.w	r0, fp, #25
30007538:	2101      	movs	r1, #1
3000753a:	4418      	add	r0, r3
3000753c:	f000 fb18 	bl	30007b70 <ymodem_uart_getdata>
30007540:	9b03      	ldr	r3, [sp, #12]
30007542:	2b00      	cmp	r3, #0
30007544:	f040 80b6 	bne.w	300076b4 <BOOT_Image1+0x910>
30007548:	2002      	movs	r0, #2
3000754a:	f000 fac1 	bl	30007ad0 <ymodem_uart_port_deinit>
3000754e:	4866      	ldr	r0, [pc, #408]	; (300076e8 <BOOT_Image1+0x944>)
30007550:	f004 ff1a 	bl	3000c388 <__DiagPrintf_veneer>
30007554:	e67a      	b.n	3000724c <BOOT_Image1+0x4a8>
30007556:	4b65      	ldr	r3, [pc, #404]	; (300076ec <BOOT_Image1+0x948>)
30007558:	4798      	blx	r3
3000755a:	2800      	cmp	r0, #0
3000755c:	f47f acf7 	bne.w	30006f4e <BOOT_Image1+0x1aa>
30007560:	68a3      	ldr	r3, [r4, #8]
30007562:	f043 0302 	orr.w	r3, r3, #2
30007566:	60a3      	str	r3, [r4, #8]
30007568:	e4f1      	b.n	30006f4e <BOOT_Image1+0x1aa>
3000756a:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000756e:	685a      	ldr	r2, [r3, #4]
30007570:	f042 0202 	orr.w	r2, r2, #2
30007574:	605a      	str	r2, [r3, #4]
30007576:	685a      	ldr	r2, [r3, #4]
30007578:	f042 0230 	orr.w	r2, r2, #48	; 0x30
3000757c:	605a      	str	r2, [r3, #4]
3000757e:	4a5c      	ldr	r2, [pc, #368]	; (300076f0 <BOOT_Image1+0x94c>)
30007580:	6f93      	ldr	r3, [r2, #120]	; 0x78
30007582:	f043 0303 	orr.w	r3, r3, #3
30007586:	6793      	str	r3, [r2, #120]	; 0x78
30007588:	e686      	b.n	30007298 <BOOT_Image1+0x4f4>
3000758a:	485a      	ldr	r0, [pc, #360]	; (300076f4 <BOOT_Image1+0x950>)
3000758c:	f004 fefc 	bl	3000c388 <__DiagPrintf_veneer>
30007590:	e4c1      	b.n	30006f16 <BOOT_Image1+0x172>
30007592:	f000 fac5 	bl	30007b20 <ymodem_uart_readable>
30007596:	2800      	cmp	r0, #0
30007598:	d0d6      	beq.n	30007548 <BOOT_Image1+0x7a4>
3000759a:	9503      	str	r5, [sp, #12]
3000759c:	e50c      	b.n	30006fb8 <BOOT_Image1+0x214>
3000759e:	4f56      	ldr	r7, [pc, #344]	; (300076f8 <BOOT_Image1+0x954>)
300075a0:	4605      	mov	r5, r0
300075a2:	f04f 080b 	mov.w	r8, #11
300075a6:	260a      	movs	r6, #10
300075a8:	f507 6389 	add.w	r3, r7, #1096	; 0x448
300075ac:	4681      	mov	r9, r0
300075ae:	9302      	str	r3, [sp, #8]
300075b0:	2004      	movs	r0, #4
300075b2:	f000 fa8d 	bl	30007ad0 <ymodem_uart_port_deinit>
300075b6:	9a02      	ldr	r2, [sp, #8]
300075b8:	9500      	str	r5, [sp, #0]
300075ba:	4613      	mov	r3, r2
300075bc:	1990      	adds	r0, r2, r6
300075be:	f8d7 14d4 	ldr.w	r1, [r7, #1236]	; 0x4d4
300075c2:	464a      	mov	r2, r9
300075c4:	4443      	add	r3, r8
300075c6:	f7ff fb09 	bl	30006bdc <password_hash_check>
300075ca:	4680      	mov	r8, r0
300075cc:	2800      	cmp	r0, #0
300075ce:	d1bb      	bne.n	30007548 <BOOT_Image1+0x7a4>
300075d0:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300075d4:	47a0      	blx	r4
300075d6:	4849      	ldr	r0, [pc, #292]	; (300076fc <BOOT_Image1+0x958>)
300075d8:	f004 fefe 	bl	3000c3d8 <____wrap_strlen_veneer>
300075dc:	4601      	mov	r1, r0
300075de:	4847      	ldr	r0, [pc, #284]	; (300076fc <BOOT_Image1+0x958>)
300075e0:	f000 fab8 	bl	30007b54 <ymodem_uart_putdata>
300075e4:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300075e8:	47a0      	blx	r4
300075ea:	f8d7 24d8 	ldr.w	r2, [r7, #1240]	; 0x4d8
300075ee:	4944      	ldr	r1, [pc, #272]	; (30007700 <BOOT_Image1+0x95c>)
300075f0:	2002      	movs	r0, #2
300075f2:	f000 fb4f 	bl	30007c94 <ymodem_img_download>
300075f6:	2032      	movs	r0, #50	; 0x32
300075f8:	47a0      	blx	r4
300075fa:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
300075fe:	b1e3      	cbz	r3, 3000763a <BOOT_Image1+0x896>
30007600:	f8df 90dc 	ldr.w	r9, [pc, #220]	; 300076e0 <BOOT_Image1+0x93c>
30007604:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
30007608:	f859 6b04 	ldr.w	r6, [r9], #4
3000760c:	0b36      	lsrs	r6, r6, #12
3000760e:	d00c      	beq.n	3000762a <BOOT_Image1+0x886>
30007610:	2500      	movs	r5, #0
30007612:	f8da 1000 	ldr.w	r1, [sl]
30007616:	2002      	movs	r0, #2
30007618:	eb01 3105 	add.w	r1, r1, r5, lsl #12
3000761c:	3501      	adds	r5, #1
3000761e:	f7fd fc73 	bl	30004f08 <FLASH_Erase_With_Lock>
30007622:	2032      	movs	r0, #50	; 0x32
30007624:	47a0      	blx	r4
30007626:	42ae      	cmp	r6, r5
30007628:	d1f3      	bne.n	30007612 <BOOT_Image1+0x86e>
3000762a:	f108 0801 	add.w	r8, r8, #1
3000762e:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30007632:	f10a 0a04 	add.w	sl, sl, #4
30007636:	4543      	cmp	r3, r8
30007638:	d8e6      	bhi.n	30007608 <BOOT_Image1+0x864>
3000763a:	4832      	ldr	r0, [pc, #200]	; (30007704 <BOOT_Image1+0x960>)
3000763c:	f004 fea4 	bl	3000c388 <__DiagPrintf_veneer>
30007640:	4b31      	ldr	r3, [pc, #196]	; (30007708 <BOOT_Image1+0x964>)
30007642:	a804      	add	r0, sp, #16
30007644:	4798      	blx	r3
30007646:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
3000764a:	4830      	ldr	r0, [pc, #192]	; (3000770c <BOOT_Image1+0x968>)
3000764c:	a904      	add	r1, sp, #16
3000764e:	4b30      	ldr	r3, [pc, #192]	; (30007710 <BOOT_Image1+0x96c>)
30007650:	f8ad 2012 	strh.w	r2, [sp, #18]
30007654:	4798      	blx	r3
30007656:	482d      	ldr	r0, [pc, #180]	; (3000770c <BOOT_Image1+0x968>)
30007658:	4b2e      	ldr	r3, [pc, #184]	; (30007714 <BOOT_Image1+0x970>)
3000765a:	4798      	blx	r3
3000765c:	e7fe      	b.n	3000765c <BOOT_Image1+0x8b8>
3000765e:	4b2e      	ldr	r3, [pc, #184]	; (30007718 <BOOT_Image1+0x974>)
30007660:	4798      	blx	r3
30007662:	2801      	cmp	r0, #1
30007664:	bf0c      	ite	eq
30007666:	2019      	moveq	r0, #25
30007668:	2004      	movne	r0, #4
3000766a:	47a0      	blx	r4
3000766c:	e606      	b.n	3000727c <BOOT_Image1+0x4d8>
3000766e:	4b2a      	ldr	r3, [pc, #168]	; (30007718 <BOOT_Image1+0x974>)
30007670:	4798      	blx	r3
30007672:	2801      	cmp	r0, #1
30007674:	bf0c      	ite	eq
30007676:	20e6      	moveq	r0, #230	; 0xe6
30007678:	2019      	movne	r0, #25
3000767a:	47a0      	blx	r4
3000767c:	e611      	b.n	300072a2 <BOOT_Image1+0x4fe>
3000767e:	4827      	ldr	r0, [pc, #156]	; (3000771c <BOOT_Image1+0x978>)
30007680:	f004 fe82 	bl	3000c388 <__DiagPrintf_veneer>
30007684:	e5de      	b.n	30007244 <BOOT_Image1+0x4a0>
30007686:	4826      	ldr	r0, [pc, #152]	; (30007720 <BOOT_Image1+0x97c>)
30007688:	f004 fe7e 	bl	3000c388 <__DiagPrintf_veneer>
3000768c:	4816      	ldr	r0, [pc, #88]	; (300076e8 <BOOT_Image1+0x944>)
3000768e:	f004 fe7b 	bl	3000c388 <__DiagPrintf_veneer>
30007692:	e5d7      	b.n	30007244 <BOOT_Image1+0x4a0>
30007694:	4823      	ldr	r0, [pc, #140]	; (30007724 <BOOT_Image1+0x980>)
30007696:	f004 fe77 	bl	3000c388 <__DiagPrintf_veneer>
3000769a:	e5d3      	b.n	30007244 <BOOT_Image1+0x4a0>
3000769c:	46d3      	mov	fp, sl
3000769e:	e58a      	b.n	300071b6 <BOOT_Image1+0x412>
300076a0:	46da      	mov	sl, fp
300076a2:	f04f 0900 	mov.w	r9, #0
300076a6:	e55c      	b.n	30007162 <BOOT_Image1+0x3be>
300076a8:	2600      	movs	r6, #0
300076aa:	f04f 080a 	mov.w	r8, #10
300076ae:	4633      	mov	r3, r6
300076b0:	46b3      	mov	fp, r6
300076b2:	e527      	b.n	30007104 <BOOT_Image1+0x360>
300076b4:	f106 080b 	add.w	r8, r6, #11
300076b8:	360a      	adds	r6, #10
300076ba:	44c8      	add	r8, r9
300076bc:	e778      	b.n	300075b0 <BOOT_Image1+0x80c>
300076be:	bf00      	nop
300076c0:	3000c674 	.word	0x3000c674
300076c4:	3000cfe8 	.word	0x3000cfe8
300076c8:	30008b5d 	.word	0x30008b5d
300076cc:	3000b395 	.word	0x3000b395
300076d0:	3000cffc 	.word	0x3000cffc
300076d4:	3000ed08 	.word	0x3000ed08
300076d8:	3000ed30 	.word	0x3000ed30
300076dc:	000129bd 	.word	0x000129bd
300076e0:	3000ed34 	.word	0x3000ed34
300076e4:	3000ed5c 	.word	0x3000ed5c
300076e8:	3000d0a8 	.word	0x3000d0a8
300076ec:	0000c179 	.word	0x0000c179
300076f0:	41000200 	.word	0x41000200
300076f4:	3000d054 	.word	0x3000d054
300076f8:	3000e7e8 	.word	0x3000e7e8
300076fc:	3000f5e8 	.word	0x3000f5e8
30007700:	3000ecc8 	.word	0x3000ecc8
30007704:	3000d0e0 	.word	0x3000d0e0
30007708:	0000d3f9 	.word	0x0000d3f9
3000770c:	41000440 	.word	0x41000440
30007710:	0000d409 	.word	0x0000d409
30007714:	0000d489 	.word	0x0000d489
30007718:	0000c169 	.word	0x0000c169
3000771c:	3000d0b8 	.word	0x3000d0b8
30007720:	3000d090 	.word	0x3000d090
30007724:	3000d0cc 	.word	0x3000d0cc

30007728 <ymodem_inquiry>:
30007728:	2806      	cmp	r0, #6
3000772a:	b510      	push	{r4, lr}
3000772c:	4604      	mov	r4, r0
3000772e:	d003      	beq.n	30007738 <ymodem_inquiry+0x10>
30007730:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007734:	4b03      	ldr	r3, [pc, #12]	; (30007744 <ymodem_inquiry+0x1c>)
30007736:	4798      	blx	r3
30007738:	4620      	mov	r0, r4
3000773a:	f000 fa07 	bl	30007b4c <ymodem_uart_putc>
3000773e:	2001      	movs	r0, #1
30007740:	bd10      	pop	{r4, pc}
30007742:	bf00      	nop
30007744:	00009b2d 	.word	0x00009b2d

30007748 <ymodem_get_first>:
30007748:	b530      	push	{r4, r5, lr}
3000774a:	b083      	sub	sp, #12
3000774c:	2300      	movs	r3, #0
3000774e:	4604      	mov	r4, r0
30007750:	4936      	ldr	r1, [pc, #216]	; (3000782c <ymodem_get_first+0xe4>)
30007752:	f10d 0006 	add.w	r0, sp, #6
30007756:	f88d 3006 	strb.w	r3, [sp, #6]
3000775a:	f000 fa19 	bl	30007b90 <ymodem_uart_getc_to>
3000775e:	b9d0      	cbnz	r0, 30007796 <ymodem_get_first+0x4e>
30007760:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007764:	3b01      	subs	r3, #1
30007766:	2b17      	cmp	r3, #23
30007768:	d80d      	bhi.n	30007786 <ymodem_get_first+0x3e>
3000776a:	e8df f003 	tbb	[pc, r3]
3000776e:	231e      	.short	0x231e
30007770:	0c0c290c 	.word	0x0c0c290c
30007774:	0c0c0c0c 	.word	0x0c0c0c0c
30007778:	0c0c0c0c 	.word	0x0c0c0c0c
3000777c:	0c0c0c0c 	.word	0x0c0c0c0c
30007780:	0c0c0c0c 	.word	0x0c0c0c0c
30007784:	190c      	.short	0x190c
30007786:	4d29      	ldr	r5, [pc, #164]	; (3000782c <ymodem_get_first+0xe4>)
30007788:	4629      	mov	r1, r5
3000778a:	f10d 0007 	add.w	r0, sp, #7
3000778e:	f000 f9ff 	bl	30007b90 <ymodem_uart_getc_to>
30007792:	2800      	cmp	r0, #0
30007794:	d0f8      	beq.n	30007788 <ymodem_get_first+0x40>
30007796:	2301      	movs	r3, #1
30007798:	2008      	movs	r0, #8
3000779a:	60a3      	str	r3, [r4, #8]
3000779c:	b003      	add	sp, #12
3000779e:	bd30      	pop	{r4, r5, pc}
300077a0:	2301      	movs	r3, #1
300077a2:	2002      	movs	r0, #2
300077a4:	60a3      	str	r3, [r4, #8]
300077a6:	b003      	add	sp, #12
300077a8:	bd30      	pop	{r4, r5, pc}
300077aa:	2380      	movs	r3, #128	; 0x80
300077ac:	2001      	movs	r0, #1
300077ae:	80a3      	strh	r3, [r4, #4]
300077b0:	b003      	add	sp, #12
300077b2:	bd30      	pop	{r4, r5, pc}
300077b4:	f44f 6380 	mov.w	r3, #1024	; 0x400
300077b8:	2001      	movs	r0, #1
300077ba:	80a3      	strh	r3, [r4, #4]
300077bc:	b003      	add	sp, #12
300077be:	bd30      	pop	{r4, r5, pc}
300077c0:	2006      	movs	r0, #6
300077c2:	4d1b      	ldr	r5, [pc, #108]	; (30007830 <ymodem_get_first+0xe8>)
300077c4:	f000 f9c2 	bl	30007b4c <ymodem_uart_putc>
300077c8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077cc:	47a8      	blx	r5
300077ce:	2043      	movs	r0, #67	; 0x43
300077d0:	f000 f9bc 	bl	30007b4c <ymodem_uart_putc>
300077d4:	2006      	movs	r0, #6
300077d6:	f000 f9b9 	bl	30007b4c <ymodem_uart_putc>
300077da:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077de:	47a8      	blx	r5
300077e0:	2043      	movs	r0, #67	; 0x43
300077e2:	f000 f9b3 	bl	30007b4c <ymodem_uart_putc>
300077e6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077ea:	47a8      	blx	r5
300077ec:	2018      	movs	r0, #24
300077ee:	f000 f9ad 	bl	30007b4c <ymodem_uart_putc>
300077f2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077f6:	47a8      	blx	r5
300077f8:	2018      	movs	r0, #24
300077fa:	f000 f9a7 	bl	30007b4c <ymodem_uart_putc>
300077fe:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007802:	47a8      	blx	r5
30007804:	2018      	movs	r0, #24
30007806:	f000 f9a1 	bl	30007b4c <ymodem_uart_putc>
3000780a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000780e:	47a8      	blx	r5
30007810:	2018      	movs	r0, #24
30007812:	f000 f99b 	bl	30007b4c <ymodem_uart_putc>
30007816:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000781a:	47a8      	blx	r5
3000781c:	2018      	movs	r0, #24
3000781e:	f000 f995 	bl	30007b4c <ymodem_uart_putc>
30007822:	2301      	movs	r3, #1
30007824:	2005      	movs	r0, #5
30007826:	60a3      	str	r3, [r4, #8]
30007828:	b003      	add	sp, #12
3000782a:	bd30      	pop	{r4, r5, pc}
3000782c:	000caa30 	.word	0x000caa30
30007830:	00009b2d 	.word	0x00009b2d

30007834 <ymodem_get_others>:
30007834:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007838:	b083      	sub	sp, #12
3000783a:	2300      	movs	r3, #0
3000783c:	4605      	mov	r5, r0
3000783e:	4952      	ldr	r1, [pc, #328]	; (30007988 <ymodem_get_others+0x154>)
30007840:	a801      	add	r0, sp, #4
30007842:	f88d 3006 	strb.w	r3, [sp, #6]
30007846:	f88d 3007 	strb.w	r3, [sp, #7]
3000784a:	f000 f9a1 	bl	30007b90 <ymodem_uart_getc_to>
3000784e:	bb38      	cbnz	r0, 300078a0 <ymodem_get_others+0x6c>
30007850:	494d      	ldr	r1, [pc, #308]	; (30007988 <ymodem_get_others+0x154>)
30007852:	f10d 0005 	add.w	r0, sp, #5
30007856:	f000 f99b 	bl	30007b90 <ymodem_uart_getc_to>
3000785a:	4607      	mov	r7, r0
3000785c:	bb00      	cbnz	r0, 300078a0 <ymodem_get_others+0x6c>
3000785e:	f89d 2004 	ldrb.w	r2, [sp, #4]
30007862:	f89d 6005 	ldrb.w	r6, [sp, #5]
30007866:	43d3      	mvns	r3, r2
30007868:	b2db      	uxtb	r3, r3
3000786a:	429e      	cmp	r6, r3
3000786c:	bf0c      	ite	eq
3000786e:	2601      	moveq	r6, #1
30007870:	2604      	movne	r6, #4
30007872:	682b      	ldr	r3, [r5, #0]
30007874:	429a      	cmp	r2, r3
30007876:	d21a      	bcs.n	300078ae <ymodem_get_others+0x7a>
30007878:	2701      	movs	r7, #1
3000787a:	88ab      	ldrh	r3, [r5, #4]
3000787c:	b1d3      	cbz	r3, 300078b4 <ymodem_get_others+0x80>
3000787e:	2400      	movs	r4, #0
30007880:	f8df 9108 	ldr.w	r9, [pc, #264]	; 3000798c <ymodem_get_others+0x158>
30007884:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007988 <ymodem_get_others+0x154>
30007888:	e002      	b.n	30007890 <ymodem_get_others+0x5c>
3000788a:	88ab      	ldrh	r3, [r5, #4]
3000788c:	42a3      	cmp	r3, r4
3000788e:	d911      	bls.n	300078b4 <ymodem_get_others+0x80>
30007890:	eb04 0009 	add.w	r0, r4, r9
30007894:	4641      	mov	r1, r8
30007896:	3401      	adds	r4, #1
30007898:	f000 f97a 	bl	30007b90 <ymodem_uart_getc_to>
3000789c:	2800      	cmp	r0, #0
3000789e:	d0f4      	beq.n	3000788a <ymodem_get_others+0x56>
300078a0:	2301      	movs	r3, #1
300078a2:	2608      	movs	r6, #8
300078a4:	60ab      	str	r3, [r5, #8]
300078a6:	4630      	mov	r0, r6
300078a8:	b003      	add	sp, #12
300078aa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300078ae:	d0e4      	beq.n	3000787a <ymodem_get_others+0x46>
300078b0:	2604      	movs	r6, #4
300078b2:	e7e2      	b.n	3000787a <ymodem_get_others+0x46>
300078b4:	4934      	ldr	r1, [pc, #208]	; (30007988 <ymodem_get_others+0x154>)
300078b6:	f10d 0006 	add.w	r0, sp, #6
300078ba:	f000 f969 	bl	30007b90 <ymodem_uart_getc_to>
300078be:	2800      	cmp	r0, #0
300078c0:	d1ee      	bne.n	300078a0 <ymodem_get_others+0x6c>
300078c2:	4931      	ldr	r1, [pc, #196]	; (30007988 <ymodem_get_others+0x154>)
300078c4:	f10d 0007 	add.w	r0, sp, #7
300078c8:	f000 f962 	bl	30007b90 <ymodem_uart_getc_to>
300078cc:	2800      	cmp	r0, #0
300078ce:	d1e7      	bne.n	300078a0 <ymodem_get_others+0x6c>
300078d0:	f89d 3006 	ldrb.w	r3, [sp, #6]
300078d4:	f89d e007 	ldrb.w	lr, [sp, #7]
300078d8:	88aa      	ldrh	r2, [r5, #4]
300078da:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
300078de:	b36a      	cbz	r2, 3000793c <ymodem_get_others+0x108>
300078e0:	4603      	mov	r3, r0
300078e2:	482a      	ldr	r0, [pc, #168]	; (3000798c <ymodem_get_others+0x158>)
300078e4:	f241 0c21 	movw	ip, #4129	; 0x1021
300078e8:	eb02 0800 	add.w	r8, r2, r0
300078ec:	7801      	ldrb	r1, [r0, #0]
300078ee:	3001      	adds	r0, #1
300078f0:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
300078f4:	2108      	movs	r1, #8
300078f6:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
300078fa:	005b      	lsls	r3, r3, #1
300078fc:	b10c      	cbz	r4, 30007902 <ymodem_get_others+0xce>
300078fe:	ea83 030c 	eor.w	r3, r3, ip
30007902:	3901      	subs	r1, #1
30007904:	d1f7      	bne.n	300078f6 <ymodem_get_others+0xc2>
30007906:	4580      	cmp	r8, r0
30007908:	d1f0      	bne.n	300078ec <ymodem_get_others+0xb8>
3000790a:	b29b      	uxth	r3, r3
3000790c:	459e      	cmp	lr, r3
3000790e:	d00a      	beq.n	30007926 <ymodem_get_others+0xf2>
30007910:	4b1f      	ldr	r3, [pc, #124]	; (30007990 <ymodem_get_others+0x15c>)
30007912:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007916:	4798      	blx	r3
30007918:	2015      	movs	r0, #21
3000791a:	f000 f917 	bl	30007b4c <ymodem_uart_putc>
3000791e:	2301      	movs	r3, #1
30007920:	2604      	movs	r6, #4
30007922:	60ab      	str	r3, [r5, #8]
30007924:	e7bf      	b.n	300078a6 <ymodem_get_others+0x72>
30007926:	b917      	cbnz	r7, 3000792e <ymodem_get_others+0xfa>
30007928:	4c1a      	ldr	r4, [pc, #104]	; (30007994 <ymodem_get_others+0x160>)
3000792a:	6823      	ldr	r3, [r4, #0]
3000792c:	b9b3      	cbnz	r3, 3000795c <ymodem_get_others+0x128>
3000792e:	f000 f947 	bl	30007bc0 <ymodem_uart_clean_rx>
30007932:	2e01      	cmp	r6, #1
30007934:	d004      	beq.n	30007940 <ymodem_get_others+0x10c>
30007936:	2e04      	cmp	r6, #4
30007938:	d1b5      	bne.n	300078a6 <ymodem_get_others+0x72>
3000793a:	e7e9      	b.n	30007910 <ymodem_get_others+0xdc>
3000793c:	4613      	mov	r3, r2
3000793e:	e7e5      	b.n	3000790c <ymodem_get_others+0xd8>
30007940:	4c14      	ldr	r4, [pc, #80]	; (30007994 <ymodem_get_others+0x160>)
30007942:	2006      	movs	r0, #6
30007944:	60ae      	str	r6, [r5, #8]
30007946:	f000 f901 	bl	30007b4c <ymodem_uart_putc>
3000794a:	6823      	ldr	r3, [r4, #0]
3000794c:	b193      	cbz	r3, 30007974 <ymodem_get_others+0x140>
3000794e:	2f00      	cmp	r7, #0
30007950:	f103 0301 	add.w	r3, r3, #1
30007954:	bf18      	it	ne
30007956:	2604      	movne	r6, #4
30007958:	6023      	str	r3, [r4, #0]
3000795a:	e7a4      	b.n	300078a6 <ymodem_get_others+0x72>
3000795c:	68eb      	ldr	r3, [r5, #12]
3000795e:	6861      	ldr	r1, [r4, #4]
30007960:	3301      	adds	r3, #1
30007962:	480a      	ldr	r0, [pc, #40]	; (3000798c <ymodem_get_others+0x158>)
30007964:	60eb      	str	r3, [r5, #12]
30007966:	f000 f94f 	bl	30007c08 <UARTIMG_Write>
3000796a:	6862      	ldr	r2, [r4, #4]
3000796c:	88ab      	ldrh	r3, [r5, #4]
3000796e:	4413      	add	r3, r2
30007970:	6063      	str	r3, [r4, #4]
30007972:	e7dc      	b.n	3000792e <ymodem_get_others+0xfa>
30007974:	4b06      	ldr	r3, [pc, #24]	; (30007990 <ymodem_get_others+0x15c>)
30007976:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000797a:	4798      	blx	r3
3000797c:	2043      	movs	r0, #67	; 0x43
3000797e:	f000 f8e5 	bl	30007b4c <ymodem_uart_putc>
30007982:	6823      	ldr	r3, [r4, #0]
30007984:	e7e3      	b.n	3000794e <ymodem_get_others+0x11a>
30007986:	bf00      	nop
30007988:	000caa30 	.word	0x000caa30
3000798c:	20016ffc 	.word	0x20016ffc
30007990:	00009b2d 	.word	0x00009b2d
30007994:	3000ed6c 	.word	0x3000ed6c

30007998 <ymodem_rx_frame>:
30007998:	2300      	movs	r3, #0
3000799a:	2201      	movs	r2, #1
3000799c:	b510      	push	{r4, lr}
3000799e:	4604      	mov	r4, r0
300079a0:	6083      	str	r3, [r0, #8]
300079a2:	8082      	strh	r2, [r0, #4]
300079a4:	f7ff fed0 	bl	30007748 <ymodem_get_first>
300079a8:	68a3      	ldr	r3, [r4, #8]
300079aa:	b92b      	cbnz	r3, 300079b8 <ymodem_rx_frame+0x20>
300079ac:	4620      	mov	r0, r4
300079ae:	f7ff ff41 	bl	30007834 <ymodem_get_others>
300079b2:	68a3      	ldr	r3, [r4, #8]
300079b4:	2b00      	cmp	r3, #0
300079b6:	d0f9      	beq.n	300079ac <ymodem_rx_frame+0x14>
300079b8:	2801      	cmp	r0, #1
300079ba:	d000      	beq.n	300079be <ymodem_rx_frame+0x26>
300079bc:	bd10      	pop	{r4, pc}
300079be:	6823      	ldr	r3, [r4, #0]
300079c0:	3301      	adds	r3, #1
300079c2:	b2db      	uxtb	r3, r3
300079c4:	6023      	str	r3, [r4, #0]
300079c6:	bd10      	pop	{r4, pc}

300079c8 <ymodem_img_rxbuffer>:
300079c8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300079cc:	2300      	movs	r3, #0
300079ce:	4c28      	ldr	r4, [pc, #160]	; (30007a70 <ymodem_img_rxbuffer+0xa8>)
300079d0:	4e28      	ldr	r6, [pc, #160]	; (30007a74 <ymodem_img_rxbuffer+0xac>)
300079d2:	460d      	mov	r5, r1
300079d4:	60a3      	str	r3, [r4, #8]
300079d6:	6163      	str	r3, [r4, #20]
300079d8:	f000 f8f2 	bl	30007bc0 <ymodem_uart_clean_rx>
300079dc:	2043      	movs	r0, #67	; 0x43
300079de:	f000 f8b5 	bl	30007b4c <ymodem_uart_putc>
300079e2:	f24c 3050 	movw	r0, #50000	; 0xc350
300079e6:	47b0      	blx	r6
300079e8:	f000 f89a 	bl	30007b20 <ymodem_uart_readable>
300079ec:	2800      	cmp	r0, #0
300079ee:	d0f5      	beq.n	300079dc <ymodem_img_rxbuffer+0x14>
300079f0:	2700      	movs	r7, #0
300079f2:	f04f 0801 	mov.w	r8, #1
300079f6:	e9c4 7500 	strd	r7, r5, [r4]
300079fa:	4d1f      	ldr	r5, [pc, #124]	; (30007a78 <ymodem_img_rxbuffer+0xb0>)
300079fc:	4628      	mov	r0, r5
300079fe:	f8a4 800c 	strh.w	r8, [r4, #12]
30007a02:	6127      	str	r7, [r4, #16]
30007a04:	f7ff fea0 	bl	30007748 <ymodem_get_first>
30007a08:	6923      	ldr	r3, [r4, #16]
30007a0a:	b92b      	cbnz	r3, 30007a18 <ymodem_img_rxbuffer+0x50>
30007a0c:	4628      	mov	r0, r5
30007a0e:	f7ff ff11 	bl	30007834 <ymodem_get_others>
30007a12:	6923      	ldr	r3, [r4, #16]
30007a14:	2b00      	cmp	r3, #0
30007a16:	d0f9      	beq.n	30007a0c <ymodem_img_rxbuffer+0x44>
30007a18:	2801      	cmp	r0, #1
30007a1a:	d024      	beq.n	30007a66 <ymodem_img_rxbuffer+0x9e>
30007a1c:	3804      	subs	r0, #4
30007a1e:	2804      	cmp	r0, #4
30007a20:	d80c      	bhi.n	30007a3c <ymodem_img_rxbuffer+0x74>
30007a22:	a301      	add	r3, pc, #4	; (adr r3, 30007a28 <ymodem_img_rxbuffer+0x60>)
30007a24:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007a28:	300079fd 	.word	0x300079fd
30007a2c:	30007a49 	.word	0x30007a49
30007a30:	30007a3d 	.word	0x30007a3d
30007a34:	30007a3d 	.word	0x30007a3d
30007a38:	30007a55 	.word	0x30007a55
30007a3c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a40:	47b0      	blx	r6
30007a42:	2018      	movs	r0, #24
30007a44:	f000 f882 	bl	30007b4c <ymodem_uart_putc>
30007a48:	200a      	movs	r0, #10
30007a4a:	f000 f8c9 	bl	30007be0 <ymodem_uart_waitbusy>
30007a4e:	2001      	movs	r0, #1
30007a50:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007a54:	f000 f8b4 	bl	30007bc0 <ymodem_uart_clean_rx>
30007a58:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a5c:	47b0      	blx	r6
30007a5e:	2015      	movs	r0, #21
30007a60:	f000 f874 	bl	30007b4c <ymodem_uart_putc>
30007a64:	e7ca      	b.n	300079fc <ymodem_img_rxbuffer+0x34>
30007a66:	68a3      	ldr	r3, [r4, #8]
30007a68:	3301      	adds	r3, #1
30007a6a:	b2db      	uxtb	r3, r3
30007a6c:	60a3      	str	r3, [r4, #8]
30007a6e:	e7c5      	b.n	300079fc <ymodem_img_rxbuffer+0x34>
30007a70:	3000ed6c 	.word	0x3000ed6c
30007a74:	00009b2d 	.word	0x00009b2d
30007a78:	3000ed74 	.word	0x3000ed74

30007a7c <ymodem_log_open>:
30007a7c:	b510      	push	{r4, lr}
30007a7e:	2303      	movs	r3, #3
30007a80:	4c04      	ldr	r4, [pc, #16]	; (30007a94 <ymodem_log_open+0x18>)
30007a82:	2200      	movs	r2, #0
30007a84:	2108      	movs	r1, #8
30007a86:	4804      	ldr	r0, [pc, #16]	; (30007a98 <ymodem_log_open+0x1c>)
30007a88:	47a0      	blx	r4
30007a8a:	2008      	movs	r0, #8
30007a8c:	4b03      	ldr	r3, [pc, #12]	; (30007a9c <ymodem_log_open+0x20>)
30007a8e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007a92:	4718      	bx	r3
30007a94:	30009ae1 	.word	0x30009ae1
30007a98:	0000ed39 	.word	0x0000ed39
30007a9c:	0000e1bd 	.word	0x0000e1bd

30007aa0 <ymodem_uart_port_init>:
30007aa0:	b500      	push	{lr}
30007aa2:	b083      	sub	sp, #12
30007aa4:	2008      	movs	r0, #8
30007aa6:	4b06      	ldr	r3, [pc, #24]	; (30007ac0 <ymodem_uart_port_init+0x20>)
30007aa8:	9201      	str	r2, [sp, #4]
30007aaa:	4798      	blx	r3
30007aac:	4b05      	ldr	r3, [pc, #20]	; (30007ac4 <ymodem_uart_port_init+0x24>)
30007aae:	2008      	movs	r0, #8
30007ab0:	4798      	blx	r3
30007ab2:	9901      	ldr	r1, [sp, #4]
30007ab4:	4804      	ldr	r0, [pc, #16]	; (30007ac8 <ymodem_uart_port_init+0x28>)
30007ab6:	4b05      	ldr	r3, [pc, #20]	; (30007acc <ymodem_uart_port_init+0x2c>)
30007ab8:	b003      	add	sp, #12
30007aba:	f85d eb04 	ldr.w	lr, [sp], #4
30007abe:	4718      	bx	r3
30007ac0:	0000e1d9 	.word	0x0000e1d9
30007ac4:	0000e2fd 	.word	0x0000e2fd
30007ac8:	4200c000 	.word	0x4200c000
30007acc:	0000a9e5 	.word	0x0000a9e5

30007ad0 <ymodem_uart_port_deinit>:
30007ad0:	2804      	cmp	r0, #4
30007ad2:	d004      	beq.n	30007ade <ymodem_uart_port_deinit+0xe>
30007ad4:	4b0d      	ldr	r3, [pc, #52]	; (30007b0c <ymodem_uart_port_deinit+0x3c>)
30007ad6:	681b      	ldr	r3, [r3, #0]
30007ad8:	039b      	lsls	r3, r3, #14
30007ada:	d40f      	bmi.n	30007afc <ymodem_uart_port_deinit+0x2c>
30007adc:	4770      	bx	lr
30007ade:	2200      	movs	r2, #0
30007ae0:	2303      	movs	r3, #3
30007ae2:	2108      	movs	r1, #8
30007ae4:	480a      	ldr	r0, [pc, #40]	; (30007b10 <ymodem_uart_port_deinit+0x40>)
30007ae6:	b510      	push	{r4, lr}
30007ae8:	4c0a      	ldr	r4, [pc, #40]	; (30007b14 <ymodem_uart_port_deinit+0x44>)
30007aea:	47a0      	blx	r4
30007aec:	4b0a      	ldr	r3, [pc, #40]	; (30007b18 <ymodem_uart_port_deinit+0x48>)
30007aee:	2008      	movs	r0, #8
30007af0:	4798      	blx	r3
30007af2:	4b06      	ldr	r3, [pc, #24]	; (30007b0c <ymodem_uart_port_deinit+0x3c>)
30007af4:	681b      	ldr	r3, [r3, #0]
30007af6:	039a      	lsls	r2, r3, #14
30007af8:	d403      	bmi.n	30007b02 <ymodem_uart_port_deinit+0x32>
30007afa:	bd10      	pop	{r4, pc}
30007afc:	4807      	ldr	r0, [pc, #28]	; (30007b1c <ymodem_uart_port_deinit+0x4c>)
30007afe:	f004 bc43 	b.w	3000c388 <__DiagPrintf_veneer>
30007b02:	4806      	ldr	r0, [pc, #24]	; (30007b1c <ymodem_uart_port_deinit+0x4c>)
30007b04:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b08:	f004 bc3e 	b.w	3000c388 <__DiagPrintf_veneer>
30007b0c:	2001c00c 	.word	0x2001c00c
30007b10:	0000ed39 	.word	0x0000ed39
30007b14:	30009ae1 	.word	0x30009ae1
30007b18:	0000e1bd 	.word	0x0000e1bd
30007b1c:	3000d100 	.word	0x3000d100

30007b20 <ymodem_uart_readable>:
30007b20:	4b00      	ldr	r3, [pc, #0]	; (30007b24 <ymodem_uart_readable+0x4>)
30007b22:	4718      	bx	r3
30007b24:	0000ac39 	.word	0x0000ac39

30007b28 <ymodem_uart_writable>:
30007b28:	4a03      	ldr	r2, [pc, #12]	; (30007b38 <ymodem_uart_writable+0x10>)
30007b2a:	b508      	push	{r3, lr}
30007b2c:	4b03      	ldr	r3, [pc, #12]	; (30007b3c <ymodem_uart_writable+0x14>)
30007b2e:	6810      	ldr	r0, [r2, #0]
30007b30:	4798      	blx	r3
30007b32:	b2c0      	uxtb	r0, r0
30007b34:	bd08      	pop	{r3, pc}
30007b36:	bf00      	nop
30007b38:	3000ed84 	.word	0x3000ed84
30007b3c:	0000d1a9 	.word	0x0000d1a9

30007b40 <ymodem_uart_getc>:
30007b40:	2000      	movs	r0, #0
30007b42:	4b01      	ldr	r3, [pc, #4]	; (30007b48 <ymodem_uart_getc+0x8>)
30007b44:	4718      	bx	r3
30007b46:	bf00      	nop
30007b48:	0000ac1d 	.word	0x0000ac1d

30007b4c <ymodem_uart_putc>:
30007b4c:	4b00      	ldr	r3, [pc, #0]	; (30007b50 <ymodem_uart_putc+0x4>)
30007b4e:	4718      	bx	r3
30007b50:	0000abb5 	.word	0x0000abb5

30007b54 <ymodem_uart_putdata>:
30007b54:	b149      	cbz	r1, 30007b6a <ymodem_uart_putdata+0x16>
30007b56:	b570      	push	{r4, r5, r6, lr}
30007b58:	4604      	mov	r4, r0
30007b5a:	1845      	adds	r5, r0, r1
30007b5c:	4e03      	ldr	r6, [pc, #12]	; (30007b6c <ymodem_uart_putdata+0x18>)
30007b5e:	f814 0b01 	ldrb.w	r0, [r4], #1
30007b62:	47b0      	blx	r6
30007b64:	42ac      	cmp	r4, r5
30007b66:	d1fa      	bne.n	30007b5e <ymodem_uart_putdata+0xa>
30007b68:	bd70      	pop	{r4, r5, r6, pc}
30007b6a:	4770      	bx	lr
30007b6c:	0000abb5 	.word	0x0000abb5

30007b70 <ymodem_uart_getdata>:
30007b70:	b151      	cbz	r1, 30007b88 <ymodem_uart_getdata+0x18>
30007b72:	b570      	push	{r4, r5, r6, lr}
30007b74:	4604      	mov	r4, r0
30007b76:	1845      	adds	r5, r0, r1
30007b78:	4e04      	ldr	r6, [pc, #16]	; (30007b8c <ymodem_uart_getdata+0x1c>)
30007b7a:	2001      	movs	r0, #1
30007b7c:	47b0      	blx	r6
30007b7e:	f804 0b01 	strb.w	r0, [r4], #1
30007b82:	42ac      	cmp	r4, r5
30007b84:	d1f9      	bne.n	30007b7a <ymodem_uart_getdata+0xa>
30007b86:	bd70      	pop	{r4, r5, r6, pc}
30007b88:	4770      	bx	lr
30007b8a:	bf00      	nop
30007b8c:	0000ac1d 	.word	0x0000ac1d

30007b90 <ymodem_uart_getc_to>:
30007b90:	b570      	push	{r4, r5, r6, lr}
30007b92:	4d09      	ldr	r5, [pc, #36]	; (30007bb8 <ymodem_uart_getc_to+0x28>)
30007b94:	4606      	mov	r6, r0
30007b96:	460c      	mov	r4, r1
30007b98:	e001      	b.n	30007b9e <ymodem_uart_getc_to+0xe>
30007b9a:	3c01      	subs	r4, #1
30007b9c:	d009      	beq.n	30007bb2 <ymodem_uart_getc_to+0x22>
30007b9e:	47a8      	blx	r5
30007ba0:	2800      	cmp	r0, #0
30007ba2:	d0fa      	beq.n	30007b9a <ymodem_uart_getc_to+0xa>
30007ba4:	4b05      	ldr	r3, [pc, #20]	; (30007bbc <ymodem_uart_getc_to+0x2c>)
30007ba6:	2000      	movs	r0, #0
30007ba8:	4798      	blx	r3
30007baa:	4603      	mov	r3, r0
30007bac:	2000      	movs	r0, #0
30007bae:	7033      	strb	r3, [r6, #0]
30007bb0:	bd70      	pop	{r4, r5, r6, pc}
30007bb2:	2001      	movs	r0, #1
30007bb4:	bd70      	pop	{r4, r5, r6, pc}
30007bb6:	bf00      	nop
30007bb8:	0000ac39 	.word	0x0000ac39
30007bbc:	0000ac1d 	.word	0x0000ac1d

30007bc0 <ymodem_uart_clean_rx>:
30007bc0:	b538      	push	{r3, r4, r5, lr}
30007bc2:	4c05      	ldr	r4, [pc, #20]	; (30007bd8 <ymodem_uart_clean_rx+0x18>)
30007bc4:	4d05      	ldr	r5, [pc, #20]	; (30007bdc <ymodem_uart_clean_rx+0x1c>)
30007bc6:	e000      	b.n	30007bca <ymodem_uart_clean_rx+0xa>
30007bc8:	47a8      	blx	r5
30007bca:	47a0      	blx	r4
30007bcc:	4603      	mov	r3, r0
30007bce:	2000      	movs	r0, #0
30007bd0:	2b00      	cmp	r3, #0
30007bd2:	d1f9      	bne.n	30007bc8 <ymodem_uart_clean_rx+0x8>
30007bd4:	bd38      	pop	{r3, r4, r5, pc}
30007bd6:	bf00      	nop
30007bd8:	0000ac39 	.word	0x0000ac39
30007bdc:	0000ac1d 	.word	0x0000ac1d

30007be0 <ymodem_uart_waitbusy>:
30007be0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007be2:	2400      	movs	r4, #0
30007be4:	4e06      	ldr	r6, [pc, #24]	; (30007c00 <ymodem_uart_waitbusy+0x20>)
30007be6:	4f07      	ldr	r7, [pc, #28]	; (30007c04 <ymodem_uart_waitbusy+0x24>)
30007be8:	4605      	mov	r5, r0
30007bea:	e002      	b.n	30007bf2 <ymodem_uart_waitbusy+0x12>
30007bec:	47b8      	blx	r7
30007bee:	42ac      	cmp	r4, r5
30007bf0:	d804      	bhi.n	30007bfc <ymodem_uart_waitbusy+0x1c>
30007bf2:	6973      	ldr	r3, [r6, #20]
30007bf4:	3401      	adds	r4, #1
30007bf6:	2064      	movs	r0, #100	; 0x64
30007bf8:	03db      	lsls	r3, r3, #15
30007bfa:	d5f7      	bpl.n	30007bec <ymodem_uart_waitbusy+0xc>
30007bfc:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007bfe:	bf00      	nop
30007c00:	4200c000 	.word	0x4200c000
30007c04:	00009b2d 	.word	0x00009b2d

30007c08 <UARTIMG_Write>:
30007c08:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007c0c:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007c10:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007c14:	d21d      	bcs.n	30007c52 <UARTIMG_Write+0x4a>
30007c16:	3a01      	subs	r2, #1
30007c18:	4f19      	ldr	r7, [pc, #100]	; (30007c80 <UARTIMG_Write+0x78>)
30007c1a:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007c90 <UARTIMG_Write+0x88>
30007c1e:	4688      	mov	r8, r1
30007c20:	1916      	adds	r6, r2, r4
30007c22:	4605      	mov	r5, r0
30007c24:	f8d9 3000 	ldr.w	r3, [r9]
30007c28:	403e      	ands	r6, r7
30007c2a:	4027      	ands	r7, r4
30007c2c:	42bb      	cmp	r3, r7
30007c2e:	d91b      	bls.n	30007c68 <UARTIMG_Write+0x60>
30007c30:	42b3      	cmp	r3, r6
30007c32:	d910      	bls.n	30007c56 <UARTIMG_Write+0x4e>
30007c34:	4e13      	ldr	r6, [pc, #76]	; (30007c84 <UARTIMG_Write+0x7c>)
30007c36:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007c3a:	4f13      	ldr	r7, [pc, #76]	; (30007c88 <UARTIMG_Write+0x80>)
30007c3c:	4446      	add	r6, r8
30007c3e:	eba3 0808 	sub.w	r8, r3, r8
30007c42:	eb08 0204 	add.w	r2, r8, r4
30007c46:	4620      	mov	r0, r4
30007c48:	3408      	adds	r4, #8
30007c4a:	2108      	movs	r1, #8
30007c4c:	47b8      	blx	r7
30007c4e:	42b4      	cmp	r4, r6
30007c50:	d1f7      	bne.n	30007c42 <UARTIMG_Write+0x3a>
30007c52:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007c56:	4631      	mov	r1, r6
30007c58:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007c5c:	2002      	movs	r0, #2
30007c5e:	4b0b      	ldr	r3, [pc, #44]	; (30007c8c <UARTIMG_Write+0x84>)
30007c60:	4798      	blx	r3
30007c62:	f8c9 6000 	str.w	r6, [r9]
30007c66:	e7e5      	b.n	30007c34 <UARTIMG_Write+0x2c>
30007c68:	4b08      	ldr	r3, [pc, #32]	; (30007c8c <UARTIMG_Write+0x84>)
30007c6a:	4639      	mov	r1, r7
30007c6c:	2002      	movs	r0, #2
30007c6e:	4798      	blx	r3
30007c70:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007c74:	42b3      	cmp	r3, r6
30007c76:	f8c9 3000 	str.w	r3, [r9]
30007c7a:	d8db      	bhi.n	30007c34 <UARTIMG_Write+0x2c>
30007c7c:	e7eb      	b.n	30007c56 <UARTIMG_Write+0x4e>
30007c7e:	bf00      	nop
30007c80:	fffff000 	.word	0xfffff000
30007c84:	f8000400 	.word	0xf8000400
30007c88:	0000a04d 	.word	0x0000a04d
30007c8c:	0000a16d 	.word	0x0000a16d
30007c90:	3000ed88 	.word	0x3000ed88

30007c94 <ymodem_img_download>:
30007c94:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007c98:	4616      	mov	r6, r2
30007c9a:	460d      	mov	r5, r1
30007c9c:	4a0a      	ldr	r2, [pc, #40]	; (30007cc8 <ymodem_img_download+0x34>)
30007c9e:	2100      	movs	r1, #0
30007ca0:	4680      	mov	r8, r0
30007ca2:	f7ff fefd 	bl	30007aa0 <ymodem_uart_port_init>
30007ca6:	b156      	cbz	r6, 30007cbe <ymodem_img_download+0x2a>
30007ca8:	3d04      	subs	r5, #4
30007caa:	2400      	movs	r4, #0
30007cac:	4f06      	ldr	r7, [pc, #24]	; (30007cc8 <ymodem_img_download+0x34>)
30007cae:	3401      	adds	r4, #1
30007cb0:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007cb4:	4638      	mov	r0, r7
30007cb6:	f7ff fe87 	bl	300079c8 <ymodem_img_rxbuffer>
30007cba:	42b4      	cmp	r4, r6
30007cbc:	d1f7      	bne.n	30007cae <ymodem_img_download+0x1a>
30007cbe:	4640      	mov	r0, r8
30007cc0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007cc4:	f7ff bf04 	b.w	30007ad0 <ymodem_uart_port_deinit>
30007cc8:	0016e360 	.word	0x0016e360

30007ccc <ChipInfo_Invalid>:
30007ccc:	4e06      	ldr	r6, [pc, #24]	; (30007ce8 <ChipInfo_Invalid+0x1c>)
30007cce:	4d07      	ldr	r5, [pc, #28]	; (30007cec <ChipInfo_Invalid+0x20>)
30007cd0:	4c07      	ldr	r4, [pc, #28]	; (30007cf0 <ChipInfo_Invalid+0x24>)
30007cd2:	b508      	push	{r3, lr}
30007cd4:	462b      	mov	r3, r5
30007cd6:	4622      	mov	r2, r4
30007cd8:	4904      	ldr	r1, [pc, #16]	; (30007cec <ChipInfo_Invalid+0x20>)
30007cda:	2002      	movs	r0, #2
30007cdc:	f004 f8ae 	bl	3000be3c <rtk_log_write>
30007ce0:	f242 7010 	movw	r0, #10000	; 0x2710
30007ce4:	47b0      	blx	r6
30007ce6:	e7f5      	b.n	30007cd4 <ChipInfo_Invalid+0x8>
30007ce8:	00009be5 	.word	0x00009be5
30007cec:	3000d120 	.word	0x3000d120
30007cf0:	3000d12c 	.word	0x3000d12c

30007cf4 <ChipInfo_Get>:
30007cf4:	b570      	push	{r4, r5, r6, lr}
30007cf6:	4c11      	ldr	r4, [pc, #68]	; (30007d3c <ChipInfo_Get+0x48>)
30007cf8:	7820      	ldrb	r0, [r4, #0]
30007cfa:	28ff      	cmp	r0, #255	; 0xff
30007cfc:	d104      	bne.n	30007d08 <ChipInfo_Get+0x14>
30007cfe:	4d10      	ldr	r5, [pc, #64]	; (30007d40 <ChipInfo_Get+0x4c>)
30007d00:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007d04:	b108      	cbz	r0, 30007d0a <ChipInfo_Get+0x16>
30007d06:	7020      	strb	r0, [r4, #0]
30007d08:	bd70      	pop	{r4, r5, r6, pc}
30007d0a:	4621      	mov	r1, r4
30007d0c:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007d10:	f000 fe8a 	bl	30008a28 <OTP_Read8>
30007d14:	7820      	ldrb	r0, [r4, #0]
30007d16:	28ff      	cmp	r0, #255	; 0xff
30007d18:	d002      	beq.n	30007d20 <ChipInfo_Get+0x2c>
30007d1a:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007d1e:	bd70      	pop	{r4, r5, r6, pc}
30007d20:	4e08      	ldr	r6, [pc, #32]	; (30007d44 <ChipInfo_Get+0x50>)
30007d22:	4d09      	ldr	r5, [pc, #36]	; (30007d48 <ChipInfo_Get+0x54>)
30007d24:	4c09      	ldr	r4, [pc, #36]	; (30007d4c <ChipInfo_Get+0x58>)
30007d26:	462b      	mov	r3, r5
30007d28:	4622      	mov	r2, r4
30007d2a:	4907      	ldr	r1, [pc, #28]	; (30007d48 <ChipInfo_Get+0x54>)
30007d2c:	2003      	movs	r0, #3
30007d2e:	f004 f885 	bl	3000be3c <rtk_log_write>
30007d32:	f242 7010 	movw	r0, #10000	; 0x2710
30007d36:	47b0      	blx	r6
30007d38:	e7f5      	b.n	30007d26 <ChipInfo_Get+0x32>
30007d3a:	bf00      	nop
30007d3c:	3000ee84 	.word	0x3000ee84
30007d40:	23020000 	.word	0x23020000
30007d44:	00009be5 	.word	0x00009be5
30007d48:	3000d120 	.word	0x3000d120
30007d4c:	3000d150 	.word	0x3000d150

30007d50 <ChipInfo_MemoryType>:
30007d50:	b510      	push	{r4, lr}
30007d52:	4c11      	ldr	r4, [pc, #68]	; (30007d98 <ChipInfo_MemoryType+0x48>)
30007d54:	47a0      	blx	r4
30007d56:	2802      	cmp	r0, #2
30007d58:	d019      	beq.n	30007d8e <ChipInfo_MemoryType+0x3e>
30007d5a:	47a0      	blx	r4
30007d5c:	2803      	cmp	r0, #3
30007d5e:	d016      	beq.n	30007d8e <ChipInfo_MemoryType+0x3e>
30007d60:	4c0e      	ldr	r4, [pc, #56]	; (30007d9c <ChipInfo_MemoryType+0x4c>)
30007d62:	4b0f      	ldr	r3, [pc, #60]	; (30007da0 <ChipInfo_MemoryType+0x50>)
30007d64:	4798      	blx	r3
30007d66:	2100      	movs	r1, #0
30007d68:	23e0      	movs	r3, #224	; 0xe0
30007d6a:	4622      	mov	r2, r4
30007d6c:	e004      	b.n	30007d78 <ChipInfo_MemoryType+0x28>
30007d6e:	7a13      	ldrb	r3, [r2, #8]
30007d70:	3101      	adds	r1, #1
30007d72:	3206      	adds	r2, #6
30007d74:	2bff      	cmp	r3, #255	; 0xff
30007d76:	d00c      	beq.n	30007d92 <ChipInfo_MemoryType+0x42>
30007d78:	4298      	cmp	r0, r3
30007d7a:	d1f8      	bne.n	30007d6e <ChipInfo_MemoryType+0x1e>
30007d7c:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007d80:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007d84:	78cb      	ldrb	r3, [r1, #3]
30007d86:	2b02      	cmp	r3, #2
30007d88:	d001      	beq.n	30007d8e <ChipInfo_MemoryType+0x3e>
30007d8a:	2001      	movs	r0, #1
30007d8c:	bd10      	pop	{r4, pc}
30007d8e:	2002      	movs	r0, #2
30007d90:	bd10      	pop	{r4, pc}
30007d92:	f7ff ff9b 	bl	30007ccc <ChipInfo_Invalid>
30007d96:	bf00      	nop
30007d98:	0000c0f9 	.word	0x0000c0f9
30007d9c:	3000d17c 	.word	0x3000d17c
30007da0:	30007cf5 	.word	0x30007cf5

30007da4 <ChipInfo_DDRType>:
30007da4:	b510      	push	{r4, lr}
30007da6:	4b0f      	ldr	r3, [pc, #60]	; (30007de4 <ChipInfo_DDRType+0x40>)
30007da8:	4798      	blx	r3
30007daa:	4b0f      	ldr	r3, [pc, #60]	; (30007de8 <ChipInfo_DDRType+0x44>)
30007dac:	4604      	mov	r4, r0
30007dae:	4798      	blx	r3
30007db0:	2801      	cmp	r0, #1
30007db2:	d112      	bne.n	30007dda <ChipInfo_DDRType+0x36>
30007db4:	480d      	ldr	r0, [pc, #52]	; (30007dec <ChipInfo_DDRType+0x48>)
30007db6:	2100      	movs	r1, #0
30007db8:	22e0      	movs	r2, #224	; 0xe0
30007dba:	4603      	mov	r3, r0
30007dbc:	e003      	b.n	30007dc6 <ChipInfo_DDRType+0x22>
30007dbe:	789a      	ldrb	r2, [r3, #2]
30007dc0:	3101      	adds	r1, #1
30007dc2:	2aff      	cmp	r2, #255	; 0xff
30007dc4:	d00b      	beq.n	30007dde <ChipInfo_DDRType+0x3a>
30007dc6:	4294      	cmp	r4, r2
30007dc8:	f103 0306 	add.w	r3, r3, #6
30007dcc:	d1f7      	bne.n	30007dbe <ChipInfo_DDRType+0x1a>
30007dce:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007dd2:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007dd6:	78c8      	ldrb	r0, [r1, #3]
30007dd8:	bd10      	pop	{r4, pc}
30007dda:	20ff      	movs	r0, #255	; 0xff
30007ddc:	bd10      	pop	{r4, pc}
30007dde:	2000      	movs	r0, #0
30007de0:	bd10      	pop	{r4, pc}
30007de2:	bf00      	nop
30007de4:	30007cf5 	.word	0x30007cf5
30007de8:	30007d51 	.word	0x30007d51
30007dec:	3000d17c 	.word	0x3000d17c

30007df0 <ChipInfo_MemorySize>:
30007df0:	b510      	push	{r4, lr}
30007df2:	4c0b      	ldr	r4, [pc, #44]	; (30007e20 <ChipInfo_MemorySize+0x30>)
30007df4:	4b0b      	ldr	r3, [pc, #44]	; (30007e24 <ChipInfo_MemorySize+0x34>)
30007df6:	4798      	blx	r3
30007df8:	2100      	movs	r1, #0
30007dfa:	23e0      	movs	r3, #224	; 0xe0
30007dfc:	4622      	mov	r2, r4
30007dfe:	e004      	b.n	30007e0a <ChipInfo_MemorySize+0x1a>
30007e00:	7a13      	ldrb	r3, [r2, #8]
30007e02:	3101      	adds	r1, #1
30007e04:	3206      	adds	r2, #6
30007e06:	2bff      	cmp	r3, #255	; 0xff
30007e08:	d007      	beq.n	30007e1a <ChipInfo_MemorySize+0x2a>
30007e0a:	4298      	cmp	r0, r3
30007e0c:	d1f8      	bne.n	30007e00 <ChipInfo_MemorySize+0x10>
30007e0e:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e12:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e16:	7908      	ldrb	r0, [r1, #4]
30007e18:	bd10      	pop	{r4, pc}
30007e1a:	f7ff ff57 	bl	30007ccc <ChipInfo_Invalid>
30007e1e:	bf00      	nop
30007e20:	3000d17c 	.word	0x3000d17c
30007e24:	30007cf5 	.word	0x30007cf5

30007e28 <ChipInfo_MemoryVendor>:
30007e28:	b510      	push	{r4, lr}
30007e2a:	4c0f      	ldr	r4, [pc, #60]	; (30007e68 <ChipInfo_MemoryVendor+0x40>)
30007e2c:	47a0      	blx	r4
30007e2e:	2803      	cmp	r0, #3
30007e30:	d016      	beq.n	30007e60 <ChipInfo_MemoryVendor+0x38>
30007e32:	47a0      	blx	r4
30007e34:	2802      	cmp	r0, #2
30007e36:	d012      	beq.n	30007e5e <ChipInfo_MemoryVendor+0x36>
30007e38:	4c0c      	ldr	r4, [pc, #48]	; (30007e6c <ChipInfo_MemoryVendor+0x44>)
30007e3a:	4b0d      	ldr	r3, [pc, #52]	; (30007e70 <ChipInfo_MemoryVendor+0x48>)
30007e3c:	4798      	blx	r3
30007e3e:	2100      	movs	r1, #0
30007e40:	23e0      	movs	r3, #224	; 0xe0
30007e42:	4622      	mov	r2, r4
30007e44:	e004      	b.n	30007e50 <ChipInfo_MemoryVendor+0x28>
30007e46:	7a13      	ldrb	r3, [r2, #8]
30007e48:	3101      	adds	r1, #1
30007e4a:	3206      	adds	r2, #6
30007e4c:	2bff      	cmp	r3, #255	; 0xff
30007e4e:	d009      	beq.n	30007e64 <ChipInfo_MemoryVendor+0x3c>
30007e50:	4298      	cmp	r0, r3
30007e52:	d1f8      	bne.n	30007e46 <ChipInfo_MemoryVendor+0x1e>
30007e54:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e58:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e5c:	7948      	ldrb	r0, [r1, #5]
30007e5e:	bd10      	pop	{r4, pc}
30007e60:	2001      	movs	r0, #1
30007e62:	bd10      	pop	{r4, pc}
30007e64:	f7ff ff32 	bl	30007ccc <ChipInfo_Invalid>
30007e68:	0000c0f9 	.word	0x0000c0f9
30007e6c:	3000d17c 	.word	0x3000d17c
30007e70:	30007cf5 	.word	0x30007cf5

30007e74 <SDM32K_Enable>:
30007e74:	4a02      	ldr	r2, [pc, #8]	; (30007e80 <SDM32K_Enable+0xc>)
30007e76:	4b03      	ldr	r3, [pc, #12]	; (30007e84 <SDM32K_Enable+0x10>)
30007e78:	6811      	ldr	r1, [r2, #0]
30007e7a:	430b      	orrs	r3, r1
30007e7c:	6013      	str	r3, [r2, #0]
30007e7e:	4770      	bx	lr
30007e80:	42008e00 	.word	0x42008e00
30007e84:	c0060000 	.word	0xc0060000

30007e88 <CLK_SWITCH_XTAL>:
30007e88:	b508      	push	{r3, lr}
30007e8a:	2801      	cmp	r0, #1
30007e8c:	4b0a      	ldr	r3, [pc, #40]	; (30007eb8 <CLK_SWITCH_XTAL+0x30>)
30007e8e:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007e92:	d00d      	beq.n	30007eb0 <CLK_SWITCH_XTAL+0x28>
30007e94:	4b09      	ldr	r3, [pc, #36]	; (30007ebc <CLK_SWITCH_XTAL+0x34>)
30007e96:	4313      	orrs	r3, r2
30007e98:	4907      	ldr	r1, [pc, #28]	; (30007eb8 <CLK_SWITCH_XTAL+0x30>)
30007e9a:	4a09      	ldr	r2, [pc, #36]	; (30007ec0 <CLK_SWITCH_XTAL+0x38>)
30007e9c:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007ea0:	4790      	blx	r2
30007ea2:	4a08      	ldr	r2, [pc, #32]	; (30007ec4 <CLK_SWITCH_XTAL+0x3c>)
30007ea4:	4b08      	ldr	r3, [pc, #32]	; (30007ec8 <CLK_SWITCH_XTAL+0x40>)
30007ea6:	fba2 2000 	umull	r2, r0, r2, r0
30007eaa:	0c80      	lsrs	r0, r0, #18
30007eac:	6018      	str	r0, [r3, #0]
30007eae:	bd08      	pop	{r3, pc}
30007eb0:	4b06      	ldr	r3, [pc, #24]	; (30007ecc <CLK_SWITCH_XTAL+0x44>)
30007eb2:	4013      	ands	r3, r2
30007eb4:	e7f0      	b.n	30007e98 <CLK_SWITCH_XTAL+0x10>
30007eb6:	bf00      	nop
30007eb8:	42008000 	.word	0x42008000
30007ebc:	80000500 	.word	0x80000500
30007ec0:	300097b1 	.word	0x300097b1
30007ec4:	431bde83 	.word	0x431bde83
30007ec8:	2001c700 	.word	0x2001c700
30007ecc:	7ffffaff 	.word	0x7ffffaff

30007ed0 <DDR_PHY_ChipInfo.part.0>:
30007ed0:	b508      	push	{r3, lr}
30007ed2:	4b24      	ldr	r3, [pc, #144]	; (30007f64 <DDR_PHY_ChipInfo.part.0+0x94>)
30007ed4:	4798      	blx	r3
30007ed6:	280e      	cmp	r0, #14
30007ed8:	d80d      	bhi.n	30007ef6 <DDR_PHY_ChipInfo.part.0+0x26>
30007eda:	2801      	cmp	r0, #1
30007edc:	d912      	bls.n	30007f04 <DDR_PHY_ChipInfo.part.0+0x34>
30007ede:	1e83      	subs	r3, r0, #2
30007ee0:	2b0c      	cmp	r3, #12
30007ee2:	d80f      	bhi.n	30007f04 <DDR_PHY_ChipInfo.part.0+0x34>
30007ee4:	e8df f003 	tbb	[pc, r3]
30007ee8:	210e1d19 	.word	0x210e1d19
30007eec:	2a0e260e 	.word	0x2a0e260e
30007ef0:	340e0e2f 	.word	0x340e0e2f
30007ef4:	39          	.byte	0x39
30007ef5:	00          	.byte	0x00
30007ef6:	28e0      	cmp	r0, #224	; 0xe0
30007ef8:	d104      	bne.n	30007f04 <DDR_PHY_ChipInfo.part.0+0x34>
30007efa:	2301      	movs	r3, #1
30007efc:	4a1a      	ldr	r2, [pc, #104]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007efe:	4618      	mov	r0, r3
30007f00:	7013      	strb	r3, [r2, #0]
30007f02:	bd08      	pop	{r3, pc}
30007f04:	4b19      	ldr	r3, [pc, #100]	; (30007f6c <DDR_PHY_ChipInfo.part.0+0x9c>)
30007f06:	2002      	movs	r0, #2
30007f08:	4a19      	ldr	r2, [pc, #100]	; (30007f70 <DDR_PHY_ChipInfo.part.0+0xa0>)
30007f0a:	4619      	mov	r1, r3
30007f0c:	f003 ff96 	bl	3000be3c <rtk_log_write>
30007f10:	2300      	movs	r3, #0
30007f12:	4a15      	ldr	r2, [pc, #84]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f14:	4618      	mov	r0, r3
30007f16:	7013      	strb	r3, [r2, #0]
30007f18:	bd08      	pop	{r3, pc}
30007f1a:	4b13      	ldr	r3, [pc, #76]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f1c:	2202      	movs	r2, #2
30007f1e:	701a      	strb	r2, [r3, #0]
30007f20:	bd08      	pop	{r3, pc}
30007f22:	4b11      	ldr	r3, [pc, #68]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f24:	2203      	movs	r2, #3
30007f26:	701a      	strb	r2, [r3, #0]
30007f28:	bd08      	pop	{r3, pc}
30007f2a:	2304      	movs	r3, #4
30007f2c:	4a0e      	ldr	r2, [pc, #56]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f2e:	4618      	mov	r0, r3
30007f30:	7013      	strb	r3, [r2, #0]
30007f32:	bd08      	pop	{r3, pc}
30007f34:	4b0c      	ldr	r3, [pc, #48]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f36:	2207      	movs	r2, #7
30007f38:	701a      	strb	r2, [r3, #0]
30007f3a:	bd08      	pop	{r3, pc}
30007f3c:	2305      	movs	r3, #5
30007f3e:	4a0a      	ldr	r2, [pc, #40]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f40:	4618      	mov	r0, r3
30007f42:	7013      	strb	r3, [r2, #0]
30007f44:	bd08      	pop	{r3, pc}
30007f46:	2306      	movs	r3, #6
30007f48:	4a07      	ldr	r2, [pc, #28]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f4a:	4618      	mov	r0, r3
30007f4c:	7013      	strb	r3, [r2, #0]
30007f4e:	bd08      	pop	{r3, pc}
30007f50:	2308      	movs	r3, #8
30007f52:	4a05      	ldr	r2, [pc, #20]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f54:	4618      	mov	r0, r3
30007f56:	7013      	strb	r3, [r2, #0]
30007f58:	bd08      	pop	{r3, pc}
30007f5a:	2309      	movs	r3, #9
30007f5c:	4a02      	ldr	r2, [pc, #8]	; (30007f68 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f5e:	4618      	mov	r0, r3
30007f60:	7013      	strb	r3, [r2, #0]
30007f62:	bd08      	pop	{r3, pc}
30007f64:	30007cf5 	.word	0x30007cf5
30007f68:	3000ee85 	.word	0x3000ee85
30007f6c:	3000d200 	.word	0x3000d200
30007f70:	3000d208 	.word	0x3000d208

30007f74 <DDR_PHY_ChipInfo_ddrtype>:
30007f74:	b510      	push	{r4, lr}
30007f76:	4c04      	ldr	r4, [pc, #16]	; (30007f88 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007f78:	7820      	ldrb	r0, [r4, #0]
30007f7a:	b100      	cbz	r0, 30007f7e <DDR_PHY_ChipInfo_ddrtype+0xa>
30007f7c:	bd10      	pop	{r4, pc}
30007f7e:	4b03      	ldr	r3, [pc, #12]	; (30007f8c <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007f80:	4798      	blx	r3
30007f82:	7020      	strb	r0, [r4, #0]
30007f84:	bd10      	pop	{r4, pc}
30007f86:	bf00      	nop
30007f88:	3000fba0 	.word	0x3000fba0
30007f8c:	30007da5 	.word	0x30007da5

30007f90 <DDR_PHY_StructInit>:
30007f90:	f240 2315 	movw	r3, #533	; 0x215
30007f94:	6583      	str	r3, [r0, #88]	; 0x58
30007f96:	4770      	bx	lr

30007f98 <DDR_PHY_DLL_CLK_DIV>:
30007f98:	b570      	push	{r4, r5, r6, lr}
30007f9a:	4d5a      	ldr	r5, [pc, #360]	; (30008104 <DDR_PHY_DLL_CLK_DIV+0x16c>)
30007f9c:	495a      	ldr	r1, [pc, #360]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
30007f9e:	fba5 4200 	umull	r4, r2, r5, r0
30007fa2:	4b5a      	ldr	r3, [pc, #360]	; (3000810c <DDR_PHY_DLL_CLK_DIV+0x174>)
30007fa4:	6a4e      	ldr	r6, [r1, #36]	; 0x24
30007fa6:	0952      	lsrs	r2, r2, #5
30007fa8:	4c59      	ldr	r4, [pc, #356]	; (30008110 <DDR_PHY_DLL_CLK_DIV+0x178>)
30007faa:	4403      	add	r3, r0
30007fac:	4859      	ldr	r0, [pc, #356]	; (30008114 <DDR_PHY_DLL_CLK_DIV+0x17c>)
30007fae:	3a03      	subs	r2, #3
30007fb0:	4034      	ands	r4, r6
30007fb2:	eb02 0682 	add.w	r6, r2, r2, lsl #2
30007fb6:	eba3 03c6 	sub.w	r3, r3, r6, lsl #3
30007fba:	02db      	lsls	r3, r3, #11
30007fbc:	fba5 5303 	umull	r5, r3, r5, r3
30007fc0:	ea00 20c3 	and.w	r0, r0, r3, lsl #11
30007fc4:	f3c3 134a 	ubfx	r3, r3, #5, #11
30007fc8:	4320      	orrs	r0, r4
30007fca:	0214      	lsls	r4, r2, #8
30007fcc:	b2d2      	uxtb	r2, r2
30007fce:	6248      	str	r0, [r1, #36]	; 0x24
30007fd0:	b2a4      	uxth	r4, r4
30007fd2:	6a88      	ldr	r0, [r1, #40]	; 0x28
30007fd4:	f420 407f 	bic.w	r0, r0, #65280	; 0xff00
30007fd8:	4304      	orrs	r4, r0
30007fda:	484f      	ldr	r0, [pc, #316]	; (30008118 <DDR_PHY_DLL_CLK_DIV+0x180>)
30007fdc:	628c      	str	r4, [r1, #40]	; 0x28
30007fde:	6a4c      	ldr	r4, [r1, #36]	; 0x24
30007fe0:	4020      	ands	r0, r4
30007fe2:	2401      	movs	r4, #1
30007fe4:	4303      	orrs	r3, r0
30007fe6:	624b      	str	r3, [r1, #36]	; 0x24
30007fe8:	6a8b      	ldr	r3, [r1, #40]	; 0x28
30007fea:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30007fee:	431a      	orrs	r2, r3
30007ff0:	4b4a      	ldr	r3, [pc, #296]	; (3000811c <DDR_PHY_DLL_CLK_DIV+0x184>)
30007ff2:	628a      	str	r2, [r1, #40]	; 0x28
30007ff4:	678c      	str	r4, [r1, #120]	; 0x78
30007ff6:	7818      	ldrb	r0, [r3, #0]
30007ff8:	28ff      	cmp	r0, #255	; 0xff
30007ffa:	d076      	beq.n	300080ea <DDR_PHY_DLL_CLK_DIV+0x152>
30007ffc:	4c48      	ldr	r4, [pc, #288]	; (30008120 <DDR_PHY_DLL_CLK_DIV+0x188>)
30007ffe:	0145      	lsls	r5, r0, #5
30008000:	4b41      	ldr	r3, [pc, #260]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008002:	eb04 1040 	add.w	r0, r4, r0, lsl #5
30008006:	5962      	ldr	r2, [r4, r5]
30008008:	4e46      	ldr	r6, [pc, #280]	; (30008124 <DDR_PHY_DLL_CLK_DIV+0x18c>)
3000800a:	611a      	str	r2, [r3, #16]
3000800c:	6842      	ldr	r2, [r0, #4]
3000800e:	615a      	str	r2, [r3, #20]
30008010:	6882      	ldr	r2, [r0, #8]
30008012:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
30008016:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
3000801a:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
3000801e:	609a      	str	r2, [r3, #8]
30008020:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
30008024:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008028:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
3000802c:	7830      	ldrb	r0, [r6, #0]
3000802e:	2800      	cmp	r0, #0
30008030:	d057      	beq.n	300080e2 <DDR_PHY_DLL_CLK_DIV+0x14a>
30008032:	2801      	cmp	r0, #1
30008034:	eb04 0305 	add.w	r3, r4, r5
30008038:	4a33      	ldr	r2, [pc, #204]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
3000803a:	d037      	beq.n	300080ac <DDR_PHY_DLL_CLK_DIV+0x114>
3000803c:	2122      	movs	r1, #34	; 0x22
3000803e:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30008042:	6959      	ldr	r1, [r3, #20]
30008044:	f8c2 1080 	str.w	r1, [r2, #128]	; 0x80
30008048:	e9d3 1306 	ldrd	r1, r3, [r3, #24]
3000804c:	f8c2 1084 	str.w	r1, [r2, #132]	; 0x84
30008050:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
30008054:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
30008058:	4933      	ldr	r1, [pc, #204]	; (30008128 <DDR_PHY_DLL_CLK_DIV+0x190>)
3000805a:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
3000805e:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
30008062:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008066:	009b      	lsls	r3, r3, #2
30008068:	d516      	bpl.n	30008098 <DDR_PHY_DLL_CLK_DIV+0x100>
3000806a:	2800      	cmp	r0, #0
3000806c:	d044      	beq.n	300080f8 <DDR_PHY_DLL_CLK_DIV+0x160>
3000806e:	4b26      	ldr	r3, [pc, #152]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008070:	4a2e      	ldr	r2, [pc, #184]	; (3000812c <DDR_PHY_DLL_CLK_DIV+0x194>)
30008072:	601a      	str	r2, [r3, #0]
30008074:	4b24      	ldr	r3, [pc, #144]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
30008076:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000807a:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
3000807e:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
30008082:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
30008086:	699a      	ldr	r2, [r3, #24]
30008088:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
3000808c:	619a      	str	r2, [r3, #24]
3000808e:	699a      	ldr	r2, [r3, #24]
30008090:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
30008094:	619a      	str	r2, [r3, #24]
30008096:	bd70      	pop	{r4, r5, r6, pc}
30008098:	b920      	cbnz	r0, 300080a4 <DDR_PHY_DLL_CLK_DIV+0x10c>
3000809a:	4b25      	ldr	r3, [pc, #148]	; (30008130 <DDR_PHY_DLL_CLK_DIV+0x198>)
3000809c:	4798      	blx	r3
3000809e:	2801      	cmp	r0, #1
300080a0:	7030      	strb	r0, [r6, #0]
300080a2:	d025      	beq.n	300080f0 <DDR_PHY_DLL_CLK_DIV+0x158>
300080a4:	4b18      	ldr	r3, [pc, #96]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080a6:	4a23      	ldr	r2, [pc, #140]	; (30008134 <DDR_PHY_DLL_CLK_DIV+0x19c>)
300080a8:	601a      	str	r2, [r3, #0]
300080aa:	e7e3      	b.n	30008074 <DDR_PHY_DLL_CLK_DIV+0xdc>
300080ac:	2033      	movs	r0, #51	; 0x33
300080ae:	695c      	ldr	r4, [r3, #20]
300080b0:	491d      	ldr	r1, [pc, #116]	; (30008128 <DDR_PHY_DLL_CLK_DIV+0x190>)
300080b2:	f8c2 0088 	str.w	r0, [r2, #136]	; 0x88
300080b6:	f8c2 4080 	str.w	r4, [r2, #128]	; 0x80
300080ba:	e9d3 0306 	ldrd	r0, r3, [r3, #24]
300080be:	f8c2 0084 	str.w	r0, [r2, #132]	; 0x84
300080c2:	f8c2 3094 	str.w	r3, [r2, #148]	; 0x94
300080c6:	f8d2 3084 	ldr.w	r3, [r2, #132]	; 0x84
300080ca:	f423 437f 	bic.w	r3, r3, #65280	; 0xff00
300080ce:	f8c2 3084 	str.w	r3, [r2, #132]	; 0x84
300080d2:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300080d6:	009a      	lsls	r2, r3, #2
300080d8:	d50a      	bpl.n	300080f0 <DDR_PHY_DLL_CLK_DIV+0x158>
300080da:	4b0b      	ldr	r3, [pc, #44]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080dc:	4a16      	ldr	r2, [pc, #88]	; (30008138 <DDR_PHY_DLL_CLK_DIV+0x1a0>)
300080de:	601a      	str	r2, [r3, #0]
300080e0:	e7c8      	b.n	30008074 <DDR_PHY_DLL_CLK_DIV+0xdc>
300080e2:	4b13      	ldr	r3, [pc, #76]	; (30008130 <DDR_PHY_DLL_CLK_DIV+0x198>)
300080e4:	4798      	blx	r3
300080e6:	7030      	strb	r0, [r6, #0]
300080e8:	e7a3      	b.n	30008032 <DDR_PHY_DLL_CLK_DIV+0x9a>
300080ea:	f7ff fef1 	bl	30007ed0 <DDR_PHY_ChipInfo.part.0>
300080ee:	e785      	b.n	30007ffc <DDR_PHY_DLL_CLK_DIV+0x64>
300080f0:	4b05      	ldr	r3, [pc, #20]	; (30008108 <DDR_PHY_DLL_CLK_DIV+0x170>)
300080f2:	4a12      	ldr	r2, [pc, #72]	; (3000813c <DDR_PHY_DLL_CLK_DIV+0x1a4>)
300080f4:	601a      	str	r2, [r3, #0]
300080f6:	e7bd      	b.n	30008074 <DDR_PHY_DLL_CLK_DIV+0xdc>
300080f8:	4b0d      	ldr	r3, [pc, #52]	; (30008130 <DDR_PHY_DLL_CLK_DIV+0x198>)
300080fa:	4798      	blx	r3
300080fc:	2801      	cmp	r0, #1
300080fe:	7030      	strb	r0, [r6, #0]
30008100:	d1b5      	bne.n	3000806e <DDR_PHY_DLL_CLK_DIV+0xd6>
30008102:	e7ea      	b.n	300080da <DDR_PHY_DLL_CLK_DIV+0x142>
30008104:	cccccccd 	.word	0xcccccccd
30008108:	41011000 	.word	0x41011000
3000810c:	001fff88 	.word	0x001fff88
30008110:	f800ffff 	.word	0xf800ffff
30008114:	07ff0000 	.word	0x07ff0000
30008118:	fffff800 	.word	0xfffff800
3000811c:	3000ee85 	.word	0x3000ee85
30008120:	3000d228 	.word	0x3000d228
30008124:	3000fba0 	.word	0x3000fba0
30008128:	42008000 	.word	0x42008000
3000812c:	4c000821 	.word	0x4c000821
30008130:	30007da5 	.word	0x30007da5
30008134:	40000021 	.word	0x40000021
30008138:	4c000903 	.word	0x4c000903
3000813c:	40000103 	.word	0x40000103

30008140 <DDR_PHY_CRT_Init>:
30008140:	492f      	ldr	r1, [pc, #188]	; (30008200 <DDR_PHY_CRT_Init+0xc0>)
30008142:	2005      	movs	r0, #5
30008144:	4a2f      	ldr	r2, [pc, #188]	; (30008204 <DDR_PHY_CRT_Init+0xc4>)
30008146:	b510      	push	{r4, lr}
30008148:	f8d1 325c 	ldr.w	r3, [r1, #604]	; 0x25c
3000814c:	4c2e      	ldr	r4, [pc, #184]	; (30008208 <DDR_PHY_CRT_Init+0xc8>)
3000814e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008152:	f8c1 325c 	str.w	r3, [r1, #604]	; 0x25c
30008156:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000815a:	f043 0301 	orr.w	r3, r3, #1
3000815e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008162:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008166:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000816a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000816e:	68e3      	ldr	r3, [r4, #12]
30008170:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008174:	60e3      	str	r3, [r4, #12]
30008176:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000817a:	f043 0302 	orr.w	r3, r3, #2
3000817e:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008182:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008186:	f023 0308 	bic.w	r3, r3, #8
3000818a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000818e:	4790      	blx	r2
30008190:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
30008194:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008198:	491c      	ldr	r1, [pc, #112]	; (3000820c <DDR_PHY_CRT_Init+0xcc>)
3000819a:	f042 0204 	orr.w	r2, r2, #4
3000819e:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300081a2:	f8d4 208c 	ldr.w	r2, [r4, #140]	; 0x8c
300081a6:	f042 0208 	orr.w	r2, r2, #8
300081aa:	f8c4 208c 	str.w	r2, [r4, #140]	; 0x8c
300081ae:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300081b0:	f042 6280 	orr.w	r2, r2, #67108864	; 0x4000000
300081b4:	62a2      	str	r2, [r4, #40]	; 0x28
300081b6:	6aa2      	ldr	r2, [r4, #40]	; 0x28
300081b8:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
300081bc:	62a2      	str	r2, [r4, #40]	; 0x28
300081be:	69e2      	ldr	r2, [r4, #28]
300081c0:	f042 0208 	orr.w	r2, r2, #8
300081c4:	61e2      	str	r2, [r4, #28]
300081c6:	68a2      	ldr	r2, [r4, #8]
300081c8:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
300081cc:	60a2      	str	r2, [r4, #8]
300081ce:	6862      	ldr	r2, [r4, #4]
300081d0:	4313      	orrs	r3, r2
300081d2:	6063      	str	r3, [r4, #4]
300081d4:	68a3      	ldr	r3, [r4, #8]
300081d6:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
300081da:	60a3      	str	r3, [r4, #8]
300081dc:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
300081e0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300081e4:	f023 0303 	bic.w	r3, r3, #3
300081e8:	f043 0302 	orr.w	r3, r3, #2
300081ec:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300081f0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300081f4:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
300081f8:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300081fc:	bd10      	pop	{r4, pc}
300081fe:	bf00      	nop
30008200:	42008000 	.word	0x42008000
30008204:	00009b2d 	.word	0x00009b2d
30008208:	41011000 	.word	0x41011000
3000820c:	90161f00 	.word	0x90161f00

30008210 <DDR_PHY_R240_ZQ_CAL>:
30008210:	b538      	push	{r3, r4, r5, lr}
30008212:	4b1e      	ldr	r3, [pc, #120]	; (3000828c <DDR_PHY_R240_ZQ_CAL+0x7c>)
30008214:	7818      	ldrb	r0, [r3, #0]
30008216:	28ff      	cmp	r0, #255	; 0xff
30008218:	d034      	beq.n	30008284 <DDR_PHY_R240_ZQ_CAL+0x74>
3000821a:	4a1d      	ldr	r2, [pc, #116]	; (30008290 <DDR_PHY_R240_ZQ_CAL+0x80>)
3000821c:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008220:	4b1c      	ldr	r3, [pc, #112]	; (30008294 <DDR_PHY_R240_ZQ_CAL+0x84>)
30008222:	2400      	movs	r4, #0
30008224:	eb02 0180 	add.w	r1, r2, r0, lsl #2
30008228:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
3000822c:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
30008230:	7c0a      	ldrb	r2, [r1, #16]
30008232:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
30008236:	0052      	lsls	r2, r2, #1
30008238:	f002 023e 	and.w	r2, r2, #62	; 0x3e
3000823c:	4302      	orrs	r2, r0
3000823e:	6848      	ldr	r0, [r1, #4]
30008240:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
30008244:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
30008248:	4a13      	ldr	r2, [pc, #76]	; (30008298 <DDR_PHY_R240_ZQ_CAL+0x88>)
3000824a:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
3000824e:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
30008252:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
30008256:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
3000825a:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
3000825e:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
30008262:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
30008266:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
3000826a:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000826e:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
30008272:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008276:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
3000827a:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
3000827e:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
30008282:	bd38      	pop	{r3, r4, r5, pc}
30008284:	f7ff fe24 	bl	30007ed0 <DDR_PHY_ChipInfo.part.0>
30008288:	e7c7      	b.n	3000821a <DDR_PHY_R240_ZQ_CAL+0xa>
3000828a:	bf00      	nop
3000828c:	3000ee85 	.word	0x3000ee85
30008290:	3000d368 	.word	0x3000d368
30008294:	41011000 	.word	0x41011000
30008298:	00262626 	.word	0x00262626

3000829c <DDR_PHY_ZQ_SET_SEL>:
3000829c:	b410      	push	{r4}
3000829e:	4b25      	ldr	r3, [pc, #148]	; (30008334 <DDR_PHY_ZQ_SET_SEL+0x98>)
300082a0:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300082a4:	4a24      	ldr	r2, [pc, #144]	; (30008338 <DDR_PHY_ZQ_SET_SEL+0x9c>)
300082a6:	4c25      	ldr	r4, [pc, #148]	; (3000833c <DDR_PHY_ZQ_SET_SEL+0xa0>)
300082a8:	4925      	ldr	r1, [pc, #148]	; (30008340 <DDR_PHY_ZQ_SET_SEL+0xa4>)
300082aa:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
300082ae:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
300082b2:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
300082b6:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
300082ba:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
300082be:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
300082c2:	f64f 74ff 	movw	r4, #65535	; 0xffff
300082c6:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
300082ca:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
300082ce:	2200      	movs	r2, #0
300082d0:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
300082d4:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
300082d8:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
300082dc:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
300082e0:	f04f 31ff 	mov.w	r1, #4294967295
300082e4:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
300082e8:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
300082ec:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
300082f0:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
300082f4:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
300082f8:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
300082fc:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30008300:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008304:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
30008308:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
3000830c:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30008310:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30008314:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
30008318:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
3000831c:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30008320:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30008324:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
30008328:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
3000832c:	f85d 4b04 	ldr.w	r4, [sp], #4
30008330:	4770      	bx	lr
30008332:	bf00      	nop
30008334:	41011000 	.word	0x41011000
30008338:	ffff1111 	.word	0xffff1111
3000833c:	ffff3333 	.word	0xffff3333
30008340:	ffff0000 	.word	0xffff0000

30008344 <DDR_PHY_DELAY_TAP_SET>:
30008344:	b510      	push	{r4, lr}
30008346:	4b15      	ldr	r3, [pc, #84]	; (3000839c <DDR_PHY_DELAY_TAP_SET+0x58>)
30008348:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
3000834c:	4914      	ldr	r1, [pc, #80]	; (300083a0 <DDR_PHY_DELAY_TAP_SET+0x5c>)
3000834e:	4c15      	ldr	r4, [pc, #84]	; (300083a4 <DDR_PHY_DELAY_TAP_SET+0x60>)
30008350:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
30008354:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
30008358:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
3000835c:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30008360:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
30008364:	7820      	ldrb	r0, [r4, #0]
30008366:	b1a0      	cbz	r0, 30008392 <DDR_PHY_DELAY_TAP_SET+0x4e>
30008368:	2801      	cmp	r0, #1
3000836a:	4b0c      	ldr	r3, [pc, #48]	; (3000839c <DDR_PHY_DELAY_TAP_SET+0x58>)
3000836c:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30008370:	bf0c      	ite	eq
30008372:	4a0d      	ldreq	r2, [pc, #52]	; (300083a8 <DDR_PHY_DELAY_TAP_SET+0x64>)
30008374:	4a0d      	ldrne	r2, [pc, #52]	; (300083ac <DDR_PHY_DELAY_TAP_SET+0x68>)
30008376:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
3000837a:	f640 0288 	movw	r2, #2184	; 0x888
3000837e:	4b07      	ldr	r3, [pc, #28]	; (3000839c <DDR_PHY_DELAY_TAP_SET+0x58>)
30008380:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
30008384:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30008388:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
3000838c:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30008390:	bd10      	pop	{r4, pc}
30008392:	4b07      	ldr	r3, [pc, #28]	; (300083b0 <DDR_PHY_DELAY_TAP_SET+0x6c>)
30008394:	4798      	blx	r3
30008396:	7020      	strb	r0, [r4, #0]
30008398:	e7e6      	b.n	30008368 <DDR_PHY_DELAY_TAP_SET+0x24>
3000839a:	bf00      	nop
3000839c:	41011000 	.word	0x41011000
300083a0:	20888888 	.word	0x20888888
300083a4:	3000fba0 	.word	0x3000fba0
300083a8:	7c033333 	.word	0x7c033333
300083ac:	7c011111 	.word	0x7c011111
300083b0:	30007da5 	.word	0x30007da5

300083b4 <DDR_PHY_READ_CTRL>:
300083b4:	4b10      	ldr	r3, [pc, #64]	; (300083f8 <DDR_PHY_READ_CTRL+0x44>)
300083b6:	b510      	push	{r4, lr}
300083b8:	7818      	ldrb	r0, [r3, #0]
300083ba:	28ff      	cmp	r0, #255	; 0xff
300083bc:	d019      	beq.n	300083f2 <DDR_PHY_READ_CTRL+0x3e>
300083be:	4a0f      	ldr	r2, [pc, #60]	; (300083fc <DDR_PHY_READ_CTRL+0x48>)
300083c0:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300083c4:	4b0e      	ldr	r3, [pc, #56]	; (30008400 <DDR_PHY_READ_CTRL+0x4c>)
300083c6:	2108      	movs	r1, #8
300083c8:	eb02 0080 	add.w	r0, r2, r0, lsl #2
300083cc:	22fe      	movs	r2, #254	; 0xfe
300083ce:	7c44      	ldrb	r4, [r0, #17]
300083d0:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
300083d4:	7c84      	ldrb	r4, [r0, #18]
300083d6:	7cc0      	ldrb	r0, [r0, #19]
300083d8:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
300083dc:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
300083e0:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
300083e4:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
300083e8:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
300083ec:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
300083f0:	bd10      	pop	{r4, pc}
300083f2:	f7ff fd6d 	bl	30007ed0 <DDR_PHY_ChipInfo.part.0>
300083f6:	e7e2      	b.n	300083be <DDR_PHY_READ_CTRL+0xa>
300083f8:	3000ee85 	.word	0x3000ee85
300083fc:	3000d368 	.word	0x3000d368
30008400:	41011000 	.word	0x41011000

30008404 <DDR_PHY_READ_LEVELING>:
30008404:	b508      	push	{r3, lr}
30008406:	4b21      	ldr	r3, [pc, #132]	; (3000848c <DDR_PHY_READ_LEVELING+0x88>)
30008408:	7818      	ldrb	r0, [r3, #0]
3000840a:	28ff      	cmp	r0, #255	; 0xff
3000840c:	d03b      	beq.n	30008486 <DDR_PHY_READ_LEVELING+0x82>
3000840e:	4b20      	ldr	r3, [pc, #128]	; (30008490 <DDR_PHY_READ_LEVELING+0x8c>)
30008410:	b1e8      	cbz	r0, 3000844e <DDR_PHY_READ_LEVELING+0x4a>
30008412:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
30008416:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
3000841a:	220f      	movs	r2, #15
3000841c:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
30008420:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30008424:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
30008428:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
3000842c:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
30008430:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30008434:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
30008438:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
3000843c:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30008440:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30008444:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
30008448:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
3000844c:	bd08      	pop	{r3, pc}
3000844e:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
30008452:	2110      	movs	r1, #16
30008454:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30008458:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
3000845c:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30008460:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
30008464:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008468:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
3000846c:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30008470:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
30008474:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008478:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
3000847c:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30008480:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
30008484:	bd08      	pop	{r3, pc}
30008486:	f7ff fd23 	bl	30007ed0 <DDR_PHY_ChipInfo.part.0>
3000848a:	e7c0      	b.n	3000840e <DDR_PHY_READ_LEVELING+0xa>
3000848c:	3000ee85 	.word	0x3000ee85
30008490:	41011000 	.word	0x41011000

30008494 <DDR_PHY_Init>:
30008494:	b510      	push	{r4, lr}
30008496:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008498:	4c2f      	ldr	r4, [pc, #188]	; (30008558 <DDR_PHY_Init+0xc4>)
3000849a:	f7ff fd7d 	bl	30007f98 <DDR_PHY_DLL_CLK_DIV>
3000849e:	f7ff fe4f 	bl	30008140 <DDR_PHY_CRT_Init>
300084a2:	f7ff feb5 	bl	30008210 <DDR_PHY_R240_ZQ_CAL>
300084a6:	f7ff fef9 	bl	3000829c <DDR_PHY_ZQ_SET_SEL>
300084aa:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
300084ae:	2300      	movs	r3, #0
300084b0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
300084b4:	4829      	ldr	r0, [pc, #164]	; (3000855c <DDR_PHY_Init+0xc8>)
300084b6:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
300084ba:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
300084be:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
300084c2:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
300084c6:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
300084ca:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
300084ce:	f7ff ff39 	bl	30008344 <DDR_PHY_DELAY_TAP_SET>
300084d2:	f7ff ff6f 	bl	300083b4 <DDR_PHY_READ_CTRL>
300084d6:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
300084da:	4a21      	ldr	r2, [pc, #132]	; (30008560 <DDR_PHY_Init+0xcc>)
300084dc:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
300084e0:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
300084e4:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
300084e8:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
300084ec:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
300084f0:	f7ff ff88 	bl	30008404 <DDR_PHY_READ_LEVELING>
300084f4:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
300084f8:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
300084fc:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008500:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008504:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
30008508:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
3000850c:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30008510:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008514:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
30008518:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
3000851c:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30008520:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008524:	f043 0303 	orr.w	r3, r3, #3
30008528:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
3000852c:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008530:	f043 030c 	orr.w	r3, r3, #12
30008534:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008538:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000853c:	f023 0303 	bic.w	r3, r3, #3
30008540:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008544:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008548:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
3000854c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
30008550:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008554:	bd10      	pop	{r4, pc}
30008556:	bf00      	nop
30008558:	41011000 	.word	0x41011000
3000855c:	00045500 	.word	0x00045500
30008560:	11117777 	.word	0x11117777

30008564 <DDR_PHY_CAL>:
30008564:	4770      	bx	lr
30008566:	bf00      	nop

30008568 <DDR_PHY_AutoGating>:
30008568:	4a0c      	ldr	r2, [pc, #48]	; (3000859c <DDR_PHY_AutoGating+0x34>)
3000856a:	2032      	movs	r0, #50	; 0x32
3000856c:	490c      	ldr	r1, [pc, #48]	; (300085a0 <DDR_PHY_AutoGating+0x38>)
3000856e:	b508      	push	{r3, lr}
30008570:	6893      	ldr	r3, [r2, #8]
30008572:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
30008576:	6093      	str	r3, [r2, #8]
30008578:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
3000857c:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008580:	f043 0314 	orr.w	r3, r3, #20
30008584:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008588:	4788      	blx	r1
3000858a:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000858e:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30008592:	f043 0303 	orr.w	r3, r3, #3
30008596:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
3000859a:	bd08      	pop	{r3, pc}
3000859c:	41011000 	.word	0x41011000
300085a0:	00009b2d 	.word	0x00009b2d

300085a4 <flash_init_userdef>:
300085a4:	4b14      	ldr	r3, [pc, #80]	; (300085f8 <flash_init_userdef+0x54>)
300085a6:	2201      	movs	r2, #1
300085a8:	2140      	movs	r1, #64	; 0x40
300085aa:	f44f 6000 	mov.w	r0, #2048	; 0x800
300085ae:	b430      	push	{r4, r5}
300085b0:	e9c3 1203 	strd	r1, r2, [r3, #12]
300085b4:	2502      	movs	r5, #2
300085b6:	2400      	movs	r4, #0
300085b8:	f640 0104 	movw	r1, #2052	; 0x804
300085bc:	601a      	str	r2, [r3, #0]
300085be:	22bb      	movs	r2, #187	; 0xbb
300085c0:	83d8      	strh	r0, [r3, #30]
300085c2:	8419      	strh	r1, [r3, #32]
300085c4:	203b      	movs	r0, #59	; 0x3b
300085c6:	21eb      	movs	r1, #235	; 0xeb
300085c8:	629a      	str	r2, [r3, #40]	; 0x28
300085ca:	4a0c      	ldr	r2, [pc, #48]	; (300085fc <flash_init_userdef+0x58>)
300085cc:	6258      	str	r0, [r3, #36]	; 0x24
300085ce:	6319      	str	r1, [r3, #48]	; 0x30
300085d0:	f64a 3120 	movw	r1, #43808	; 0xab20
300085d4:	655a      	str	r2, [r3, #84]	; 0x54
300085d6:	22b9      	movs	r2, #185	; 0xb9
300085d8:	4809      	ldr	r0, [pc, #36]	; (30008600 <flash_init_userdef+0x5c>)
300085da:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
300085de:	6598      	str	r0, [r3, #88]	; 0x58
300085e0:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
300085e4:	e9c3 5405 	strd	r5, r4, [r3, #20]
300085e8:	2406      	movs	r4, #6
300085ea:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
300085ee:	246b      	movs	r4, #107	; 0x6b
300085f0:	62dc      	str	r4, [r3, #44]	; 0x2c
300085f2:	bc30      	pop	{r4, r5}
300085f4:	4770      	bx	lr
300085f6:	bf00      	nop
300085f8:	2001c01c 	.word	0x2001c01c
300085fc:	00059f06 	.word	0x00059f06
30008600:	d8600001 	.word	0xd8600001

30008604 <nand_init_userdef>:
30008604:	4b18      	ldr	r3, [pc, #96]	; (30008668 <nand_init_userdef+0x64>)
30008606:	2001      	movs	r0, #1
30008608:	2200      	movs	r2, #0
3000860a:	2102      	movs	r1, #2
3000860c:	b410      	push	{r4}
3000860e:	6018      	str	r0, [r3, #0]
30008610:	f640 0408 	movw	r4, #2056	; 0x808
30008614:	6159      	str	r1, [r3, #20]
30008616:	83dc      	strh	r4, [r3, #30]
30008618:	f44f 6400 	mov.w	r4, #2048	; 0x800
3000861c:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008620:	841c      	strh	r4, [r3, #32]
30008622:	246b      	movs	r4, #107	; 0x6b
30008624:	619a      	str	r2, [r3, #24]
30008626:	62dc      	str	r4, [r3, #44]	; 0x2c
30008628:	4c10      	ldr	r4, [pc, #64]	; (3000866c <nand_init_userdef+0x68>)
3000862a:	775a      	strb	r2, [r3, #29]
3000862c:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008630:	629a      	str	r2, [r3, #40]	; 0x28
30008632:	631a      	str	r2, [r3, #48]	; 0x30
30008634:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
30008638:	65da      	str	r2, [r3, #92]	; 0x5c
3000863a:	e9c3 0003 	strd	r0, r0, [r3, #12]
3000863e:	203b      	movs	r0, #59	; 0x3b
30008640:	6258      	str	r0, [r3, #36]	; 0x24
30008642:	200b      	movs	r0, #11
30008644:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
30008648:	6698      	str	r0, [r3, #104]	; 0x68
3000864a:	2008      	movs	r0, #8
3000864c:	4908      	ldr	r1, [pc, #32]	; (30008670 <nand_init_userdef+0x6c>)
3000864e:	64d8      	str	r0, [r3, #76]	; 0x4c
30008650:	f241 0013 	movw	r0, #4115	; 0x1013
30008654:	6599      	str	r1, [r3, #88]	; 0x58
30008656:	4907      	ldr	r1, [pc, #28]	; (30008674 <nand_init_userdef+0x70>)
30008658:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
3000865c:	6719      	str	r1, [r3, #112]	; 0x70
3000865e:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
30008662:	f85d 4b04 	ldr.w	r4, [sp], #4
30008666:	4770      	bx	lr
30008668:	2001c01c 	.word	0x2001c01c
3000866c:	000f9f06 	.word	0x000f9f06
30008670:	d800001f 	.word	0xd800001f
30008674:	10300804 	.word	0x10300804

30008678 <flash_get_layout_info>:
30008678:	b570      	push	{r4, r5, r6, lr}
3000867a:	4b0f      	ldr	r3, [pc, #60]	; (300086b8 <flash_get_layout_info+0x40>)
3000867c:	4615      	mov	r5, r2
3000867e:	4604      	mov	r4, r0
30008680:	460e      	mov	r6, r1
30008682:	4798      	blx	r3
30008684:	4b0d      	ldr	r3, [pc, #52]	; (300086bc <flash_get_layout_info+0x44>)
30008686:	4a0e      	ldr	r2, [pc, #56]	; (300086c0 <flash_get_layout_info+0x48>)
30008688:	2800      	cmp	r0, #0
3000868a:	bf14      	ite	ne
3000868c:	469c      	movne	ip, r3
3000868e:	4694      	moveq	ip, r2
30008690:	f8dc 3000 	ldr.w	r3, [ip]
30008694:	2bff      	cmp	r3, #255	; 0xff
30008696:	d104      	bne.n	300086a2 <flash_get_layout_info+0x2a>
30008698:	e00d      	b.n	300086b6 <flash_get_layout_info+0x3e>
3000869a:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
3000869e:	2bff      	cmp	r3, #255	; 0xff
300086a0:	d009      	beq.n	300086b6 <flash_get_layout_info+0x3e>
300086a2:	429c      	cmp	r4, r3
300086a4:	d1f9      	bne.n	3000869a <flash_get_layout_info+0x22>
300086a6:	b116      	cbz	r6, 300086ae <flash_get_layout_info+0x36>
300086a8:	f8dc 3004 	ldr.w	r3, [ip, #4]
300086ac:	6033      	str	r3, [r6, #0]
300086ae:	b115      	cbz	r5, 300086b6 <flash_get_layout_info+0x3e>
300086b0:	f8dc 3008 	ldr.w	r3, [ip, #8]
300086b4:	602b      	str	r3, [r5, #0]
300086b6:	bd70      	pop	{r4, r5, r6, pc}
300086b8:	30009b61 	.word	0x30009b61
300086bc:	3000ef60 	.word	0x3000ef60
300086c0:	3000ee88 	.word	0x3000ee88

300086c4 <NAND_CHECK_IS_BAD_BLOCK>:
300086c4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300086c8:	4e24      	ldr	r6, [pc, #144]	; (3000875c <NAND_CHECK_IS_BAD_BLOCK+0x98>)
300086ca:	0907      	lsrs	r7, r0, #4
300086cc:	0045      	lsls	r5, r0, #1
300086ce:	b085      	sub	sp, #20
300086d0:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
300086d4:	f005 051e 	and.w	r5, r5, #30
300086d8:	40eb      	lsrs	r3, r5
300086da:	f003 0303 	and.w	r3, r3, #3
300086de:	2b02      	cmp	r3, #2
300086e0:	d02c      	beq.n	3000873c <NAND_CHECK_IS_BAD_BLOCK+0x78>
300086e2:	2b01      	cmp	r3, #1
300086e4:	d026      	beq.n	30008734 <NAND_CHECK_IS_BAD_BLOCK+0x70>
300086e6:	4b1e      	ldr	r3, [pc, #120]	; (30008760 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
300086e8:	f04f 0801 	mov.w	r8, #1
300086ec:	2204      	movs	r2, #4
300086ee:	4604      	mov	r4, r0
300086f0:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
300086f4:	0180      	lsls	r0, r0, #6
300086f6:	ab03      	add	r3, sp, #12
300086f8:	f8df 9070 	ldr.w	r9, [pc, #112]	; 3000876c <NAND_CHECK_IS_BAD_BLOCK+0xa8>
300086fc:	fa08 f101 	lsl.w	r1, r8, r1
30008700:	47c8      	blx	r9
30008702:	f89d 300c 	ldrb.w	r3, [sp, #12]
30008706:	f89d 200d 	ldrb.w	r2, [sp, #13]
3000870a:	4013      	ands	r3, r2
3000870c:	2bff      	cmp	r3, #255	; 0xff
3000870e:	d019      	beq.n	30008744 <NAND_CHECK_IS_BAD_BLOCK+0x80>
30008710:	2002      	movs	r0, #2
30008712:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
30008716:	4b13      	ldr	r3, [pc, #76]	; (30008764 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
30008718:	fa00 f505 	lsl.w	r5, r0, r5
3000871c:	9400      	str	r4, [sp, #0]
3000871e:	4619      	mov	r1, r3
30008720:	432a      	orrs	r2, r5
30008722:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
30008726:	4a10      	ldr	r2, [pc, #64]	; (30008768 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
30008728:	f003 fb88 	bl	3000be3c <rtk_log_write>
3000872c:	4640      	mov	r0, r8
3000872e:	b005      	add	sp, #20
30008730:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008734:	2000      	movs	r0, #0
30008736:	b005      	add	sp, #20
30008738:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000873c:	2001      	movs	r0, #1
3000873e:	b005      	add	sp, #20
30008740:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008744:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008748:	fa08 f005 	lsl.w	r0, r8, r5
3000874c:	4303      	orrs	r3, r0
3000874e:	2000      	movs	r0, #0
30008750:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
30008754:	b005      	add	sp, #20
30008756:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000875a:	bf00      	nop
3000875c:	3000fba8 	.word	0x3000fba8
30008760:	2001c01c 	.word	0x2001c01c
30008764:	3000d5fc 	.word	0x3000d5fc
30008768:	3000d608 	.word	0x3000d608
3000876c:	30009ab1 	.word	0x30009ab1

30008770 <Nand_Get_NandAddr>:
30008770:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008774:	4f10      	ldr	r7, [pc, #64]	; (300087b8 <Nand_Get_NandAddr+0x48>)
30008776:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
3000877a:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
3000877e:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
30008782:	40e8      	lsrs	r0, r5
30008784:	fa26 f505 	lsr.w	r5, r6, r5
30008788:	09ad      	lsrs	r5, r5, #6
3000878a:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
3000878e:	d90f      	bls.n	300087b0 <Nand_Get_NandAddr+0x40>
30008790:	0984      	lsrs	r4, r0, #6
30008792:	f04f 0840 	mov.w	r8, #64	; 0x40
30008796:	4620      	mov	r0, r4
30008798:	3401      	adds	r4, #1
3000879a:	f7ff ff93 	bl	300086c4 <NAND_CHECK_IS_BAD_BLOCK>
3000879e:	b128      	cbz	r0, 300087ac <Nand_Get_NandAddr+0x3c>
300087a0:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
300087a4:	3501      	adds	r5, #1
300087a6:	fa08 f303 	lsl.w	r3, r8, r3
300087aa:	441e      	add	r6, r3
300087ac:	42a5      	cmp	r5, r4
300087ae:	d8f2      	bhi.n	30008796 <Nand_Get_NandAddr+0x26>
300087b0:	4630      	mov	r0, r6
300087b2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300087b6:	bf00      	nop
300087b8:	2001c01c 	.word	0x2001c01c

300087bc <Nand_L2P_Table>:
300087bc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300087c0:	b082      	sub	sp, #8
300087c2:	4604      	mov	r4, r0
300087c4:	2002      	movs	r0, #2
300087c6:	aa01      	add	r2, sp, #4
300087c8:	4669      	mov	r1, sp
300087ca:	f7ff ff55 	bl	30008678 <flash_get_layout_info>
300087ce:	9800      	ldr	r0, [sp, #0]
300087d0:	42a0      	cmp	r0, r4
300087d2:	d810      	bhi.n	300087f6 <Nand_L2P_Table+0x3a>
300087d4:	9b01      	ldr	r3, [sp, #4]
300087d6:	42a3      	cmp	r3, r4
300087d8:	d805      	bhi.n	300087e6 <Nand_L2P_Table+0x2a>
300087da:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
300087de:	4620      	mov	r0, r4
300087e0:	b002      	add	sp, #8
300087e2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300087e6:	4621      	mov	r1, r4
300087e8:	f7ff ffc2 	bl	30008770 <Nand_Get_NandAddr>
300087ec:	4604      	mov	r4, r0
300087ee:	4620      	mov	r0, r4
300087f0:	b002      	add	sp, #8
300087f2:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300087f6:	4f0e      	ldr	r7, [pc, #56]	; (30008830 <Nand_L2P_Table+0x74>)
300087f8:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
300087fc:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30008800:	fa24 f606 	lsr.w	r6, r4, r6
30008804:	09b6      	lsrs	r6, r6, #6
30008806:	d0ea      	beq.n	300087de <Nand_L2P_Table+0x22>
30008808:	2500      	movs	r5, #0
3000880a:	f04f 0840 	mov.w	r8, #64	; 0x40
3000880e:	4628      	mov	r0, r5
30008810:	3501      	adds	r5, #1
30008812:	f7ff ff57 	bl	300086c4 <NAND_CHECK_IS_BAD_BLOCK>
30008816:	b128      	cbz	r0, 30008824 <Nand_L2P_Table+0x68>
30008818:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
3000881c:	3601      	adds	r6, #1
3000881e:	fa08 f303 	lsl.w	r3, r8, r3
30008822:	441c      	add	r4, r3
30008824:	42b5      	cmp	r5, r6
30008826:	d3f2      	bcc.n	3000880e <Nand_L2P_Table+0x52>
30008828:	4620      	mov	r0, r4
3000882a:	b002      	add	sp, #8
3000882c:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008830:	2001c01c 	.word	0x2001c01c

30008834 <NandImgCopy>:
30008834:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30008838:	4617      	mov	r7, r2
3000883a:	4603      	mov	r3, r0
3000883c:	b089      	sub	sp, #36	; 0x24
3000883e:	f8df 9100 	ldr.w	r9, [pc, #256]	; 30008940 <NandImgCopy+0x10c>
30008842:	f107 38ff 	add.w	r8, r7, #4294967295
30008846:	4608      	mov	r0, r1
30008848:	9307      	str	r3, [sp, #28]
3000884a:	f7ff ffb7 	bl	300087bc <Nand_L2P_Table>
3000884e:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
30008852:	4480      	add	r8, r0
30008854:	fa20 f406 	lsr.w	r4, r0, r6
30008858:	fa28 f806 	lsr.w	r8, r8, r6
3000885c:	eba8 0804 	sub.w	r8, r8, r4
30008860:	f1b8 3fff 	cmp.w	r8, #4294967295
30008864:	d054      	beq.n	30008910 <NandImgCopy+0xdc>
30008866:	2300      	movs	r3, #0
30008868:	4605      	mov	r5, r0
3000886a:	f04f 0b40 	mov.w	fp, #64	; 0x40
3000886e:	469a      	mov	sl, r3
30008870:	9306      	str	r3, [sp, #24]
30008872:	463b      	mov	r3, r7
30008874:	4647      	mov	r7, r8
30008876:	4698      	mov	r8, r3
30008878:	e020      	b.n	300088bc <NandImgCopy+0x88>
3000887a:	f1c2 0320 	rsb	r3, r2, #32
3000887e:	f04f 31ff 	mov.w	r1, #4294967295
30008882:	40d9      	lsrs	r1, r3
30008884:	9b07      	ldr	r3, [sp, #28]
30008886:	4029      	ands	r1, r5
30008888:	4453      	add	r3, sl
3000888a:	b377      	cbz	r7, 300088ea <NandImgCopy+0xb6>
3000888c:	2601      	movs	r6, #1
3000888e:	4620      	mov	r0, r4
30008890:	9105      	str	r1, [sp, #20]
30008892:	fa06 f202 	lsl.w	r2, r6, r2
30008896:	4e27      	ldr	r6, [pc, #156]	; (30008934 <NandImgCopy+0x100>)
30008898:	9204      	str	r2, [sp, #16]
3000889a:	1a52      	subs	r2, r2, r1
3000889c:	4492      	add	sl, r2
3000889e:	4415      	add	r5, r2
300088a0:	47b0      	blx	r6
300088a2:	f010 0f70 	tst.w	r0, #112	; 0x70
300088a6:	9b04      	ldr	r3, [sp, #16]
300088a8:	9905      	ldr	r1, [sp, #20]
300088aa:	d134      	bne.n	30008916 <NandImgCopy+0xe2>
300088ac:	eba8 0803 	sub.w	r8, r8, r3
300088b0:	4488      	add	r8, r1
300088b2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
300088b6:	3f01      	subs	r7, #1
300088b8:	fa25 f406 	lsr.w	r4, r5, r6
300088bc:	9b06      	ldr	r3, [sp, #24]
300088be:	4632      	mov	r2, r6
300088c0:	09a6      	lsrs	r6, r4, #6
300088c2:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
300088c6:	d8d8      	bhi.n	3000887a <NandImgCopy+0x46>
300088c8:	e005      	b.n	300088d6 <NandImgCopy+0xa2>
300088ca:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
300088ce:	3440      	adds	r4, #64	; 0x40
300088d0:	fa0b f303 	lsl.w	r3, fp, r3
300088d4:	441d      	add	r5, r3
300088d6:	4630      	mov	r0, r6
300088d8:	3601      	adds	r6, #1
300088da:	f7ff fef3 	bl	300086c4 <NAND_CHECK_IS_BAD_BLOCK>
300088de:	2800      	cmp	r0, #0
300088e0:	d1f3      	bne.n	300088ca <NandImgCopy+0x96>
300088e2:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
300088e6:	9606      	str	r6, [sp, #24]
300088e8:	e7c7      	b.n	3000887a <NandImgCopy+0x46>
300088ea:	4642      	mov	r2, r8
300088ec:	4620      	mov	r0, r4
300088ee:	4d11      	ldr	r5, [pc, #68]	; (30008934 <NandImgCopy+0x100>)
300088f0:	47a8      	blx	r5
300088f2:	f010 0f70 	tst.w	r0, #112	; 0x70
300088f6:	d00b      	beq.n	30008910 <NandImgCopy+0xdc>
300088f8:	09a1      	lsrs	r1, r4, #6
300088fa:	4b0f      	ldr	r3, [pc, #60]	; (30008938 <NandImgCopy+0x104>)
300088fc:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008900:	4a0e      	ldr	r2, [pc, #56]	; (3000893c <NandImgCopy+0x108>)
30008902:	e9cd 1001 	strd	r1, r0, [sp, #4]
30008906:	9400      	str	r4, [sp, #0]
30008908:	4619      	mov	r1, r3
3000890a:	2004      	movs	r0, #4
3000890c:	f003 fa96 	bl	3000be3c <rtk_log_write>
30008910:	b009      	add	sp, #36	; 0x24
30008912:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30008916:	1ac9      	subs	r1, r1, r3
30008918:	09a3      	lsrs	r3, r4, #6
3000891a:	f004 043f 	and.w	r4, r4, #63	; 0x3f
3000891e:	9002      	str	r0, [sp, #8]
30008920:	9301      	str	r3, [sp, #4]
30008922:	4488      	add	r8, r1
30008924:	4b04      	ldr	r3, [pc, #16]	; (30008938 <NandImgCopy+0x104>)
30008926:	2004      	movs	r0, #4
30008928:	4a04      	ldr	r2, [pc, #16]	; (3000893c <NandImgCopy+0x108>)
3000892a:	4619      	mov	r1, r3
3000892c:	9400      	str	r4, [sp, #0]
3000892e:	f003 fa85 	bl	3000be3c <rtk_log_write>
30008932:	e7be      	b.n	300088b2 <NandImgCopy+0x7e>
30008934:	30009ab1 	.word	0x30009ab1
30008938:	3000d5fc 	.word	0x3000d5fc
3000893c:	3000d620 	.word	0x3000d620
30008940:	2001c01c 	.word	0x2001c01c

30008944 <OTP_Read8.part.0>:
30008944:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
30008948:	4b30      	ldr	r3, [pc, #192]	; (30008a0c <OTP_Read8.part.0+0xc8>)
3000894a:	4680      	mov	r8, r0
3000894c:	e843 f300 	tt	r3, r3
30008950:	f3c3 5380 	ubfx	r3, r3, #22, #1
30008954:	4a2e      	ldr	r2, [pc, #184]	; (30008a10 <OTP_Read8.part.0+0xcc>)
30008956:	4e2f      	ldr	r6, [pc, #188]	; (30008a14 <OTP_Read8.part.0+0xd0>)
30008958:	b082      	sub	sp, #8
3000895a:	2b00      	cmp	r3, #0
3000895c:	4689      	mov	r9, r1
3000895e:	bf0e      	itee	eq
30008960:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008964:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008968:	4616      	movne	r6, r2
3000896a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000896c:	05d8      	lsls	r0, r3, #23
3000896e:	d505      	bpl.n	3000897c <OTP_Read8.part.0+0x38>
30008970:	4d29      	ldr	r5, [pc, #164]	; (30008a18 <OTP_Read8.part.0+0xd4>)
30008972:	2064      	movs	r0, #100	; 0x64
30008974:	47a8      	blx	r5
30008976:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008978:	05d9      	lsls	r1, r3, #23
3000897a:	d4fa      	bmi.n	30008972 <OTP_Read8.part.0+0x2e>
3000897c:	6c3b      	ldr	r3, [r7, #64]	; 0x40
3000897e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008982:	643b      	str	r3, [r7, #64]	; 0x40
30008984:	4b25      	ldr	r3, [pc, #148]	; (30008a1c <OTP_Read8.part.0+0xd8>)
30008986:	681a      	ldr	r2, [r3, #0]
30008988:	07d2      	lsls	r2, r2, #31
3000898a:	d403      	bmi.n	30008994 <OTP_Read8.part.0+0x50>
3000898c:	681a      	ldr	r2, [r3, #0]
3000898e:	f042 0201 	orr.w	r2, r2, #1
30008992:	601a      	str	r2, [r3, #0]
30008994:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008998:	ea4f 2308 	mov.w	r3, r8, lsl #8
3000899c:	694a      	ldr	r2, [r1, #20]
3000899e:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300089a2:	614a      	str	r2, [r1, #20]
300089a4:	6033      	str	r3, [r6, #0]
300089a6:	6833      	ldr	r3, [r6, #0]
300089a8:	2b00      	cmp	r3, #0
300089aa:	db10      	blt.n	300089ce <OTP_Read8.part.0+0x8a>
300089ac:	2400      	movs	r4, #0
300089ae:	4d1a      	ldr	r5, [pc, #104]	; (30008a18 <OTP_Read8.part.0+0xd4>)
300089b0:	f644 6a20 	movw	sl, #20000	; 0x4e20
300089b4:	e001      	b.n	300089ba <OTP_Read8.part.0+0x76>
300089b6:	4554      	cmp	r4, sl
300089b8:	d01a      	beq.n	300089f0 <OTP_Read8.part.0+0xac>
300089ba:	2005      	movs	r0, #5
300089bc:	3401      	adds	r4, #1
300089be:	47a8      	blx	r5
300089c0:	6833      	ldr	r3, [r6, #0]
300089c2:	2b00      	cmp	r3, #0
300089c4:	daf7      	bge.n	300089b6 <OTP_Read8.part.0+0x72>
300089c6:	f644 6320 	movw	r3, #20000	; 0x4e20
300089ca:	429c      	cmp	r4, r3
300089cc:	d010      	beq.n	300089f0 <OTP_Read8.part.0+0xac>
300089ce:	6833      	ldr	r3, [r6, #0]
300089d0:	2001      	movs	r0, #1
300089d2:	f889 3000 	strb.w	r3, [r9]
300089d6:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
300089da:	6953      	ldr	r3, [r2, #20]
300089dc:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
300089e0:	6153      	str	r3, [r2, #20]
300089e2:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300089e4:	f423 7380 	bic.w	r3, r3, #256	; 0x100
300089e8:	643b      	str	r3, [r7, #64]	; 0x40
300089ea:	b002      	add	sp, #8
300089ec:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
300089f0:	22ff      	movs	r2, #255	; 0xff
300089f2:	4b0b      	ldr	r3, [pc, #44]	; (30008a20 <OTP_Read8.part.0+0xdc>)
300089f4:	2002      	movs	r0, #2
300089f6:	f8cd 8000 	str.w	r8, [sp]
300089fa:	4619      	mov	r1, r3
300089fc:	f889 2000 	strb.w	r2, [r9]
30008a00:	4a08      	ldr	r2, [pc, #32]	; (30008a24 <OTP_Read8.part.0+0xe0>)
30008a02:	f003 fa1b 	bl	3000be3c <rtk_log_write>
30008a06:	2000      	movs	r0, #0
30008a08:	e7e5      	b.n	300089d6 <OTP_Read8.part.0+0x92>
30008a0a:	bf00      	nop
30008a0c:	0000e5f9 	.word	0x0000e5f9
30008a10:	5200000c 	.word	0x5200000c
30008a14:	42000008 	.word	0x42000008
30008a18:	00009b2d 	.word	0x00009b2d
30008a1c:	42008000 	.word	0x42008000
30008a20:	3000d65c 	.word	0x3000d65c
30008a24:	3000d660 	.word	0x3000d660

30008a28 <OTP_Read8>:
30008a28:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008a2c:	d201      	bcs.n	30008a32 <OTP_Read8+0xa>
30008a2e:	f7ff bf89 	b.w	30008944 <OTP_Read8.part.0>
30008a32:	22ff      	movs	r2, #255	; 0xff
30008a34:	2000      	movs	r0, #0
30008a36:	700a      	strb	r2, [r1, #0]
30008a38:	4770      	bx	lr
30008a3a:	bf00      	nop

30008a3c <PLL_NP_ClkSet>:
30008a3c:	b570      	push	{r4, r5, r6, lr}
30008a3e:	4604      	mov	r4, r0
30008a40:	4b1b      	ldr	r3, [pc, #108]	; (30008ab0 <PLL_NP_ClkSet+0x74>)
30008a42:	4798      	blx	r3
30008a44:	4d1b      	ldr	r5, [pc, #108]	; (30008ab4 <PLL_NP_ClkSet+0x78>)
30008a46:	08c2      	lsrs	r2, r0, #3
30008a48:	fbb4 f3f0 	udiv	r3, r4, r0
30008a4c:	fb00 4c13 	mls	ip, r0, r3, r4
30008a50:	1e9c      	subs	r4, r3, #2
30008a52:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008a54:	0164      	lsls	r4, r4, #5
30008a56:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30008a5a:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30008a5e:	431c      	orrs	r4, r3
30008a60:	fbbc f3f2 	udiv	r3, ip, r2
30008a64:	646c      	str	r4, [r5, #68]	; 0x44
30008a66:	041c      	lsls	r4, r3, #16
30008a68:	fb02 c013 	mls	r0, r2, r3, ip
30008a6c:	2300      	movs	r3, #0
30008a6e:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008a70:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008a74:	4619      	mov	r1, r3
30008a76:	fa1f fc86 	uxth.w	ip, r6
30008a7a:	4e0f      	ldr	r6, [pc, #60]	; (30008ab8 <PLL_NP_ClkSet+0x7c>)
30008a7c:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008a80:	ea44 040c 	orr.w	r4, r4, ip
30008a84:	f003 fa64 	bl	3000bf50 <__aeabi_uldivmod>
30008a88:	4603      	mov	r3, r0
30008a8a:	2001      	movs	r0, #1
30008a8c:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008a90:	64ec      	str	r4, [r5, #76]	; 0x4c
30008a92:	47b0      	blx	r6
30008a94:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008a96:	203c      	movs	r0, #60	; 0x3c
30008a98:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008a9c:	646b      	str	r3, [r5, #68]	; 0x44
30008a9e:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008aa0:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008aa4:	646b      	str	r3, [r5, #68]	; 0x44
30008aa6:	4633      	mov	r3, r6
30008aa8:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008aac:	4718      	bx	r3
30008aae:	bf00      	nop
30008ab0:	000099f5 	.word	0x000099f5
30008ab4:	42008800 	.word	0x42008800
30008ab8:	00009b2d 	.word	0x00009b2d

30008abc <PLL_AP_ClkSet>:
30008abc:	b510      	push	{r4, lr}
30008abe:	4604      	mov	r4, r0
30008ac0:	4b0b      	ldr	r3, [pc, #44]	; (30008af0 <PLL_AP_ClkSet+0x34>)
30008ac2:	4798      	blx	r3
30008ac4:	fbb4 f0f0 	udiv	r0, r4, r0
30008ac8:	f1a0 0319 	sub.w	r3, r0, #25
30008acc:	1e84      	subs	r4, r0, #2
30008ace:	2b0f      	cmp	r3, #15
30008ad0:	d807      	bhi.n	30008ae2 <PLL_AP_ClkSet+0x26>
30008ad2:	4b08      	ldr	r3, [pc, #32]	; (30008af4 <PLL_AP_ClkSet+0x38>)
30008ad4:	b2e0      	uxtb	r0, r4
30008ad6:	695c      	ldr	r4, [r3, #20]
30008ad8:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008adc:	4320      	orrs	r0, r4
30008ade:	6158      	str	r0, [r3, #20]
30008ae0:	bd10      	pop	{r4, pc}
30008ae2:	f240 1181 	movw	r1, #385	; 0x181
30008ae6:	4804      	ldr	r0, [pc, #16]	; (30008af8 <PLL_AP_ClkSet+0x3c>)
30008ae8:	f003 fc8e 	bl	3000c408 <__io_assert_failed_veneer>
30008aec:	e7f1      	b.n	30008ad2 <PLL_AP_ClkSet+0x16>
30008aee:	bf00      	nop
30008af0:	000099f5 	.word	0x000099f5
30008af4:	41000300 	.word	0x41000300
30008af8:	3000d67c 	.word	0x3000d67c

30008afc <PLL_AP>:
30008afc:	2801      	cmp	r0, #1
30008afe:	b538      	push	{r3, r4, r5, lr}
30008b00:	d009      	beq.n	30008b16 <PLL_AP+0x1a>
30008b02:	4b13      	ldr	r3, [pc, #76]	; (30008b50 <PLL_AP+0x54>)
30008b04:	681a      	ldr	r2, [r3, #0]
30008b06:	f022 0209 	bic.w	r2, r2, #9
30008b0a:	601a      	str	r2, [r3, #0]
30008b0c:	681a      	ldr	r2, [r3, #0]
30008b0e:	f022 0206 	bic.w	r2, r2, #6
30008b12:	601a      	str	r2, [r3, #0]
30008b14:	bd38      	pop	{r3, r4, r5, pc}
30008b16:	4b0f      	ldr	r3, [pc, #60]	; (30008b54 <PLL_AP+0x58>)
30008b18:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008b1a:	f012 0f05 	tst.w	r2, #5
30008b1e:	d00f      	beq.n	30008b40 <PLL_AP+0x44>
30008b20:	4d0d      	ldr	r5, [pc, #52]	; (30008b58 <PLL_AP+0x5c>)
30008b22:	4c0b      	ldr	r4, [pc, #44]	; (30008b50 <PLL_AP+0x54>)
30008b24:	2001      	movs	r0, #1
30008b26:	6823      	ldr	r3, [r4, #0]
30008b28:	f043 0306 	orr.w	r3, r3, #6
30008b2c:	6023      	str	r3, [r4, #0]
30008b2e:	47a8      	blx	r5
30008b30:	6823      	ldr	r3, [r4, #0]
30008b32:	f043 0309 	orr.w	r3, r3, #9
30008b36:	6023      	str	r3, [r4, #0]
30008b38:	6863      	ldr	r3, [r4, #4]
30008b3a:	2b00      	cmp	r3, #0
30008b3c:	dafc      	bge.n	30008b38 <PLL_AP+0x3c>
30008b3e:	bd38      	pop	{r3, r4, r5, pc}
30008b40:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008b42:	20a0      	movs	r0, #160	; 0xa0
30008b44:	4d04      	ldr	r5, [pc, #16]	; (30008b58 <PLL_AP+0x5c>)
30008b46:	f042 0207 	orr.w	r2, r2, #7
30008b4a:	679a      	str	r2, [r3, #120]	; 0x78
30008b4c:	47a8      	blx	r5
30008b4e:	e7e8      	b.n	30008b22 <PLL_AP+0x26>
30008b50:	41000300 	.word	0x41000300
30008b54:	42008800 	.word	0x42008800
30008b58:	00009b2d 	.word	0x00009b2d

30008b5c <PSRAM_INFO_Update>:
30008b5c:	4b46      	ldr	r3, [pc, #280]	; (30008c78 <PSRAM_INFO_Update+0x11c>)
30008b5e:	4947      	ldr	r1, [pc, #284]	; (30008c7c <PSRAM_INFO_Update+0x120>)
30008b60:	4a47      	ldr	r2, [pc, #284]	; (30008c80 <PSRAM_INFO_Update+0x124>)
30008b62:	b530      	push	{r4, r5, lr}
30008b64:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008b68:	b083      	sub	sp, #12
30008b6a:	680d      	ldr	r5, [r1, #0]
30008b6c:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008b70:	4c44      	ldr	r4, [pc, #272]	; (30008c84 <PSRAM_INFO_Update+0x128>)
30008b72:	fb02 f505 	mul.w	r5, r2, r5
30008b76:	3301      	adds	r3, #1
30008b78:	fbb5 f5f3 	udiv	r5, r5, r3
30008b7c:	086a      	lsrs	r2, r5, #1
30008b7e:	4b42      	ldr	r3, [pc, #264]	; (30008c88 <PSRAM_INFO_Update+0x12c>)
30008b80:	60e2      	str	r2, [r4, #12]
30008b82:	4798      	blx	r3
30008b84:	4b41      	ldr	r3, [pc, #260]	; (30008c8c <PSRAM_INFO_Update+0x130>)
30008b86:	6020      	str	r0, [r4, #0]
30008b88:	4798      	blx	r3
30008b8a:	4b41      	ldr	r3, [pc, #260]	; (30008c90 <PSRAM_INFO_Update+0x134>)
30008b8c:	220a      	movs	r2, #10
30008b8e:	60a0      	str	r0, [r4, #8]
30008b90:	6122      	str	r2, [r4, #16]
30008b92:	2220      	movs	r2, #32
30008b94:	fbb3 f3f5 	udiv	r3, r3, r5
30008b98:	6163      	str	r3, [r4, #20]
30008b9a:	fbb2 f3f3 	udiv	r3, r2, r3
30008b9e:	6822      	ldr	r2, [r4, #0]
30008ba0:	6223      	str	r3, [r4, #32]
30008ba2:	2a01      	cmp	r2, #1
30008ba4:	68e3      	ldr	r3, [r4, #12]
30008ba6:	4a3b      	ldr	r2, [pc, #236]	; (30008c94 <PSRAM_INFO_Update+0x138>)
30008ba8:	d01d      	beq.n	30008be6 <PSRAM_INFO_Update+0x8a>
30008baa:	4293      	cmp	r3, r2
30008bac:	d810      	bhi.n	30008bd0 <PSRAM_INFO_Update+0x74>
30008bae:	2304      	movs	r3, #4
30008bb0:	2108      	movs	r1, #8
30008bb2:	461a      	mov	r2, r3
30008bb4:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008bb8:	4b37      	ldr	r3, [pc, #220]	; (30008c98 <PSRAM_INFO_Update+0x13c>)
30008bba:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
30008bbe:	4b37      	ldr	r3, [pc, #220]	; (30008c9c <PSRAM_INFO_Update+0x140>)
30008bc0:	2004      	movs	r0, #4
30008bc2:	4a37      	ldr	r2, [pc, #220]	; (30008ca0 <PSRAM_INFO_Update+0x144>)
30008bc4:	4619      	mov	r1, r3
30008bc6:	9500      	str	r5, [sp, #0]
30008bc8:	f003 f938 	bl	3000be3c <rtk_log_write>
30008bcc:	b003      	add	sp, #12
30008bce:	bd30      	pop	{r4, r5, pc}
30008bd0:	4a34      	ldr	r2, [pc, #208]	; (30008ca4 <PSRAM_INFO_Update+0x148>)
30008bd2:	4293      	cmp	r3, r2
30008bd4:	d916      	bls.n	30008c04 <PSRAM_INFO_Update+0xa8>
30008bd6:	4a34      	ldr	r2, [pc, #208]	; (30008ca8 <PSRAM_INFO_Update+0x14c>)
30008bd8:	4293      	cmp	r3, r2
30008bda:	d824      	bhi.n	30008c26 <PSRAM_INFO_Update+0xca>
30008bdc:	2306      	movs	r3, #6
30008bde:	461a      	mov	r2, r3
30008be0:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008be4:	e7e8      	b.n	30008bb8 <PSRAM_INFO_Update+0x5c>
30008be6:	4293      	cmp	r3, r2
30008be8:	d907      	bls.n	30008bfa <PSRAM_INFO_Update+0x9e>
30008bea:	4a2e      	ldr	r2, [pc, #184]	; (30008ca4 <PSRAM_INFO_Update+0x148>)
30008bec:	4293      	cmp	r3, r2
30008bee:	d80f      	bhi.n	30008c10 <PSRAM_INFO_Update+0xb4>
30008bf0:	2205      	movs	r2, #5
30008bf2:	230f      	movs	r3, #15
30008bf4:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008bf8:	e7e1      	b.n	30008bbe <PSRAM_INFO_Update+0x62>
30008bfa:	2204      	movs	r2, #4
30008bfc:	230f      	movs	r3, #15
30008bfe:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c02:	e7dc      	b.n	30008bbe <PSRAM_INFO_Update+0x62>
30008c04:	2305      	movs	r3, #5
30008c06:	2108      	movs	r1, #8
30008c08:	461a      	mov	r2, r3
30008c0a:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008c0e:	e7d3      	b.n	30008bb8 <PSRAM_INFO_Update+0x5c>
30008c10:	4a25      	ldr	r2, [pc, #148]	; (30008ca8 <PSRAM_INFO_Update+0x14c>)
30008c12:	4293      	cmp	r3, r2
30008c14:	d917      	bls.n	30008c46 <PSRAM_INFO_Update+0xea>
30008c16:	4a25      	ldr	r2, [pc, #148]	; (30008cac <PSRAM_INFO_Update+0x150>)
30008c18:	4293      	cmp	r3, r2
30008c1a:	d81f      	bhi.n	30008c5c <PSRAM_INFO_Update+0x100>
30008c1c:	2207      	movs	r2, #7
30008c1e:	2314      	movs	r3, #20
30008c20:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c24:	e7cb      	b.n	30008bbe <PSRAM_INFO_Update+0x62>
30008c26:	4a21      	ldr	r2, [pc, #132]	; (30008cac <PSRAM_INFO_Update+0x150>)
30008c28:	4293      	cmp	r3, r2
30008c2a:	d911      	bls.n	30008c50 <PSRAM_INFO_Update+0xf4>
30008c2c:	4a20      	ldr	r2, [pc, #128]	; (30008cb0 <PSRAM_INFO_Update+0x154>)
30008c2e:	4293      	cmp	r3, r2
30008c30:	bf8c      	ite	hi
30008c32:	2300      	movhi	r3, #0
30008c34:	2301      	movls	r3, #1
30008c36:	2804      	cmp	r0, #4
30008c38:	d100      	bne.n	30008c3c <PSRAM_INFO_Update+0xe0>
30008c3a:	b94b      	cbnz	r3, 30008c50 <PSRAM_INFO_Update+0xf4>
30008c3c:	2803      	cmp	r0, #3
30008c3e:	d100      	bne.n	30008c42 <PSRAM_INFO_Update+0xe6>
30008c40:	b9a3      	cbnz	r3, 30008c6c <PSRAM_INFO_Update+0x110>
30008c42:	7e22      	ldrb	r2, [r4, #24]
30008c44:	e7b8      	b.n	30008bb8 <PSRAM_INFO_Update+0x5c>
30008c46:	2206      	movs	r2, #6
30008c48:	2312      	movs	r3, #18
30008c4a:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c4e:	e7b6      	b.n	30008bbe <PSRAM_INFO_Update+0x62>
30008c50:	2307      	movs	r3, #7
30008c52:	2106      	movs	r1, #6
30008c54:	461a      	mov	r2, r3
30008c56:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008c5a:	e7ad      	b.n	30008bb8 <PSRAM_INFO_Update+0x5c>
30008c5c:	4a14      	ldr	r2, [pc, #80]	; (30008cb0 <PSRAM_INFO_Update+0x154>)
30008c5e:	4293      	cmp	r3, r2
30008c60:	d8ad      	bhi.n	30008bbe <PSRAM_INFO_Update+0x62>
30008c62:	2208      	movs	r2, #8
30008c64:	231c      	movs	r3, #28
30008c66:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c6a:	e7a8      	b.n	30008bbe <PSRAM_INFO_Update+0x62>
30008c6c:	230a      	movs	r3, #10
30008c6e:	2106      	movs	r1, #6
30008c70:	461a      	mov	r2, r3
30008c72:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008c76:	e79f      	b.n	30008bb8 <PSRAM_INFO_Update+0x5c>
30008c78:	42008000 	.word	0x42008000
30008c7c:	3000ee70 	.word	0x3000ee70
30008c80:	000f4240 	.word	0x000f4240
30008c84:	3000fbb0 	.word	0x3000fbb0
30008c88:	30007e29 	.word	0x30007e29
30008c8c:	30007df1 	.word	0x30007df1
30008c90:	3b9aca00 	.word	0x3b9aca00
30008c94:	0632ea00 	.word	0x0632ea00
30008c98:	23020000 	.word	0x23020000
30008c9c:	3000d68c 	.word	0x3000d68c
30008ca0:	3000d694 	.word	0x3000d694
30008ca4:	07ed6b40 	.word	0x07ed6b40
30008ca8:	09e4f580 	.word	0x09e4f580
30008cac:	0bebc200 	.word	0x0bebc200
30008cb0:	0ee6b280 	.word	0x0ee6b280

30008cb4 <PSRAM_PHY_StructInit>:
30008cb4:	2203      	movs	r2, #3
30008cb6:	2301      	movs	r3, #1
30008cb8:	210a      	movs	r1, #10
30008cba:	b410      	push	{r4}
30008cbc:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008cc0:	2402      	movs	r4, #2
30008cc2:	2207      	movs	r2, #7
30008cc4:	e9c0 3400 	strd	r3, r4, [r0]
30008cc8:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008ccc:	f85d 4b04 	ldr.w	r4, [sp], #4
30008cd0:	6182      	str	r2, [r0, #24]
30008cd2:	4770      	bx	lr

30008cd4 <PSRAM_PHY_Init>:
30008cd4:	b538      	push	{r3, r4, r5, lr}
30008cd6:	4604      	mov	r4, r0
30008cd8:	6903      	ldr	r3, [r0, #16]
30008cda:	4d14      	ldr	r5, [pc, #80]	; (30008d2c <PSRAM_PHY_Init+0x58>)
30008cdc:	f003 031f 	and.w	r3, r3, #31
30008ce0:	4913      	ldr	r1, [pc, #76]	; (30008d30 <PSRAM_PHY_Init+0x5c>)
30008ce2:	e9d0 0200 	ldrd	r0, r2, [r0]
30008ce6:	0612      	lsls	r2, r2, #24
30008ce8:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008cec:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30008cf0:	4313      	orrs	r3, r2
30008cf2:	68a2      	ldr	r2, [r4, #8]
30008cf4:	0412      	lsls	r2, r2, #16
30008cf6:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008cfa:	4313      	orrs	r3, r2
30008cfc:	68e2      	ldr	r2, [r4, #12]
30008cfe:	0212      	lsls	r2, r2, #8
30008d00:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008d04:	4313      	orrs	r3, r2
30008d06:	606b      	str	r3, [r5, #4]
30008d08:	4788      	blx	r1
30008d0a:	2801      	cmp	r0, #1
30008d0c:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30008d10:	bf08      	it	eq
30008d12:	4a08      	ldreq	r2, [pc, #32]	; (30008d34 <PSRAM_PHY_Init+0x60>)
30008d14:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008d18:	f001 010f 	and.w	r1, r1, #15
30008d1c:	bf18      	it	ne
30008d1e:	4a06      	ldrne	r2, [pc, #24]	; (30008d38 <PSRAM_PHY_Init+0x64>)
30008d20:	b2db      	uxtb	r3, r3
30008d22:	430b      	orrs	r3, r1
30008d24:	431a      	orrs	r2, r3
30008d26:	61aa      	str	r2, [r5, #24]
30008d28:	bd38      	pop	{r3, r4, r5, pc}
30008d2a:	bf00      	nop
30008d2c:	41002000 	.word	0x41002000
30008d30:	30007e29 	.word	0x30007e29
30008d34:	80000e00 	.word	0x80000e00
30008d38:	80000f00 	.word	0x80000f00

30008d3c <PSRAM_CTRL_Init>:
30008d3c:	b470      	push	{r4, r5, r6}
30008d3e:	4b4c      	ldr	r3, [pc, #304]	; (30008e70 <PSRAM_CTRL_Init+0x134>)
30008d40:	2400      	movs	r4, #0
30008d42:	4a4c      	ldr	r2, [pc, #304]	; (30008e74 <PSRAM_CTRL_Init+0x138>)
30008d44:	f24d 0101 	movw	r1, #53249	; 0xd001
30008d48:	609c      	str	r4, [r3, #8]
30008d4a:	2001      	movs	r0, #1
30008d4c:	601a      	str	r2, [r3, #0]
30008d4e:	4a4a      	ldr	r2, [pc, #296]	; (30008e78 <PSRAM_CTRL_Init+0x13c>)
30008d50:	6158      	str	r0, [r3, #20]
30008d52:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30008d56:	6811      	ldr	r1, [r2, #0]
30008d58:	2902      	cmp	r1, #2
30008d5a:	d04a      	beq.n	30008df2 <PSRAM_CTRL_Init+0xb6>
30008d5c:	6954      	ldr	r4, [r2, #20]
30008d5e:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30008d62:	69d0      	ldr	r0, [r2, #28]
30008d64:	6915      	ldr	r5, [r2, #16]
30008d66:	fbb0 f0f4 	udiv	r0, r0, r4
30008d6a:	fbb1 f1f4 	udiv	r1, r1, r4
30008d6e:	6994      	ldr	r4, [r2, #24]
30008d70:	f005 020f 	and.w	r2, r5, #15
30008d74:	f241 5551 	movw	r5, #5457	; 0x1551
30008d78:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008d7c:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30008d80:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008d84:	094a      	lsrs	r2, r1, #5
30008d86:	0181      	lsls	r1, r0, #6
30008d88:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30008d8c:	4d3b      	ldr	r5, [pc, #236]	; (30008e7c <PSRAM_CTRL_Init+0x140>)
30008d8e:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008d92:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
30008d96:	00a2      	lsls	r2, r4, #2
30008d98:	4301      	orrs	r1, r0
30008d9a:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
30008d9e:	3a01      	subs	r2, #1
30008da0:	430d      	orrs	r5, r1
30008da2:	f242 0120 	movw	r1, #8224	; 0x2020
30008da6:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008daa:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
30008dae:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
30008db2:	f44f 2180 	mov.w	r1, #262144	; 0x40000
30008db6:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
30008dba:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
30008dbe:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008dc2:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
30008dc6:	4420      	add	r0, r4
30008dc8:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30008dcc:	f640 71fe 	movw	r1, #4094	; 0xffe
30008dd0:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
30008dd4:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30008dd8:	432a      	orrs	r2, r5
30008dda:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
30008dde:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30008de2:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30008de6:	4a22      	ldr	r2, [pc, #136]	; (30008e70 <PSRAM_CTRL_Init+0x134>)
30008de8:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008dea:	061b      	lsls	r3, r3, #24
30008dec:	d5fc      	bpl.n	30008de8 <PSRAM_CTRL_Init+0xac>
30008dee:	bc70      	pop	{r4, r5, r6}
30008df0:	4770      	bx	lr
30008df2:	6955      	ldr	r5, [r2, #20]
30008df4:	207d      	movs	r0, #125	; 0x7d
30008df6:	69d1      	ldr	r1, [r2, #28]
30008df8:	26a0      	movs	r6, #160	; 0xa0
30008dfa:	6914      	ldr	r4, [r2, #16]
30008dfc:	6992      	ldr	r2, [r2, #24]
30008dfe:	f004 040f 	and.w	r4, r4, #15
30008e02:	0092      	lsls	r2, r2, #2
30008e04:	fbb1 f1f5 	udiv	r1, r1, r5
30008e08:	fbb0 f5f5 	udiv	r5, r0, r5
30008e0c:	f245 5051 	movw	r0, #21841	; 0x5551
30008e10:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30008e14:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30008e18:	0189      	lsls	r1, r1, #6
30008e1a:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30008e1e:	f641 4570 	movw	r5, #7280	; 0x1c70
30008e22:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008e26:	4325      	orrs	r5, r4
30008e28:	2420      	movs	r4, #32
30008e2a:	4308      	orrs	r0, r1
30008e2c:	1e51      	subs	r1, r2, #1
30008e2e:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30008e32:	3a02      	subs	r2, #2
30008e34:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30008e38:	f3c1 010b 	ubfx	r1, r1, #0, #12
30008e3c:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008e40:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30008e44:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30008e48:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30008e4c:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30008e50:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30008e54:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30008e58:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30008e5c:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30008e60:	4301      	orrs	r1, r0
30008e62:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30008e66:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008e6a:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30008e6e:	e7ba      	b.n	30008de6 <PSRAM_CTRL_Init+0xaa>
30008e70:	41012000 	.word	0x41012000
30008e74:	00bfe000 	.word	0x00bfe000
30008e78:	3000fbb0 	.word	0x3000fbb0
30008e7c:	00107000 	.word	0x00107000

30008e80 <PSRAM_REG_Write>:
30008e80:	b430      	push	{r4, r5}
30008e82:	4c20      	ldr	r4, [pc, #128]	; (30008f04 <PSRAM_REG_Write+0x84>)
30008e84:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008e86:	f013 0301 	ands.w	r3, r3, #1
30008e8a:	d1fb      	bne.n	30008e84 <PSRAM_REG_Write+0x4>
30008e8c:	6825      	ldr	r5, [r4, #0]
30008e8e:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008e92:	6025      	str	r5, [r4, #0]
30008e94:	60a3      	str	r3, [r4, #8]
30008e96:	6825      	ldr	r5, [r4, #0]
30008e98:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008e9c:	6025      	str	r5, [r4, #0]
30008e9e:	6063      	str	r3, [r4, #4]
30008ea0:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30008ea4:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30008ea8:	23c0      	movs	r3, #192	; 0xc0
30008eaa:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008eae:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008eb2:	0e03      	lsrs	r3, r0, #24
30008eb4:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008eb8:	f3c0 4307 	ubfx	r3, r0, #16, #8
30008ebc:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008ec0:	f3c0 2307 	ubfx	r3, r0, #8, #8
30008ec4:	b2c0      	uxtb	r0, r0
30008ec6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008eca:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008ece:	b141      	cbz	r1, 30008ee2 <PSRAM_REG_Write+0x62>
30008ed0:	3a01      	subs	r2, #1
30008ed2:	480c      	ldr	r0, [pc, #48]	; (30008f04 <PSRAM_REG_Write+0x84>)
30008ed4:	4411      	add	r1, r2
30008ed6:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008eda:	428a      	cmp	r2, r1
30008edc:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008ee0:	d1f9      	bne.n	30008ed6 <PSRAM_REG_Write+0x56>
30008ee2:	4a08      	ldr	r2, [pc, #32]	; (30008f04 <PSRAM_REG_Write+0x84>)
30008ee4:	2301      	movs	r3, #1
30008ee6:	6093      	str	r3, [r2, #8]
30008ee8:	6893      	ldr	r3, [r2, #8]
30008eea:	07d9      	lsls	r1, r3, #31
30008eec:	d4fc      	bmi.n	30008ee8 <PSRAM_REG_Write+0x68>
30008eee:	4a05      	ldr	r2, [pc, #20]	; (30008f04 <PSRAM_REG_Write+0x84>)
30008ef0:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008ef2:	07db      	lsls	r3, r3, #31
30008ef4:	d4fc      	bmi.n	30008ef0 <PSRAM_REG_Write+0x70>
30008ef6:	6813      	ldr	r3, [r2, #0]
30008ef8:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008efc:	6013      	str	r3, [r2, #0]
30008efe:	bc30      	pop	{r4, r5}
30008f00:	4770      	bx	lr
30008f02:	bf00      	nop
30008f04:	41012000 	.word	0x41012000

30008f08 <PSRAM_APM_DEVIC_Init>:
30008f08:	4b12      	ldr	r3, [pc, #72]	; (30008f54 <PSRAM_APM_DEVIC_Init+0x4c>)
30008f0a:	2102      	movs	r1, #2
30008f0c:	2000      	movs	r0, #0
30008f0e:	b530      	push	{r4, r5, lr}
30008f10:	4c11      	ldr	r4, [pc, #68]	; (30008f58 <PSRAM_APM_DEVIC_Init+0x50>)
30008f12:	b083      	sub	sp, #12
30008f14:	699d      	ldr	r5, [r3, #24]
30008f16:	466a      	mov	r2, sp
30008f18:	eb04 0c05 	add.w	ip, r4, r5
30008f1c:	1eeb      	subs	r3, r5, #3
30008f1e:	4c0f      	ldr	r4, [pc, #60]	; (30008f5c <PSRAM_APM_DEVIC_Init+0x54>)
30008f20:	408b      	lsls	r3, r1
30008f22:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30008f26:	f003 031c 	and.w	r3, r3, #28
30008f2a:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30008f2e:	f043 0320 	orr.w	r3, r3, #32
30008f32:	fa5f fc8c 	uxtb.w	ip, ip
30008f36:	f88d 3000 	strb.w	r3, [sp]
30008f3a:	f88d c004 	strb.w	ip, [sp, #4]
30008f3e:	f88d 3001 	strb.w	r3, [sp, #1]
30008f42:	f88d c005 	strb.w	ip, [sp, #5]
30008f46:	47a0      	blx	r4
30008f48:	aa01      	add	r2, sp, #4
30008f4a:	2102      	movs	r1, #2
30008f4c:	2004      	movs	r0, #4
30008f4e:	47a0      	blx	r4
30008f50:	b003      	add	sp, #12
30008f52:	bd30      	pop	{r4, r5, pc}
30008f54:	3000fbb0 	.word	0x3000fbb0
30008f58:	3000f038 	.word	0x3000f038
30008f5c:	30008e81 	.word	0x30008e81

30008f60 <PSRAM_WB_REG_Write>:
30008f60:	b430      	push	{r4, r5}
30008f62:	4c1e      	ldr	r4, [pc, #120]	; (30008fdc <PSRAM_WB_REG_Write+0x7c>)
30008f64:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008f66:	f013 0301 	ands.w	r3, r3, #1
30008f6a:	d1fb      	bne.n	30008f64 <PSRAM_WB_REG_Write+0x4>
30008f6c:	6825      	ldr	r5, [r4, #0]
30008f6e:	b2c0      	uxtb	r0, r0
30008f70:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008f74:	6025      	str	r5, [r4, #0]
30008f76:	60a3      	str	r3, [r4, #8]
30008f78:	6825      	ldr	r5, [r4, #0]
30008f7a:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008f7e:	6025      	str	r5, [r4, #0]
30008f80:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30008f84:	6063      	str	r3, [r4, #4]
30008f86:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
30008f8a:	2560      	movs	r5, #96	; 0x60
30008f8c:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30008f90:	2501      	movs	r5, #1
30008f92:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f96:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30008f9a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f9e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008fa2:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008fa6:	b141      	cbz	r1, 30008fba <PSRAM_WB_REG_Write+0x5a>
30008fa8:	3a01      	subs	r2, #1
30008faa:	480c      	ldr	r0, [pc, #48]	; (30008fdc <PSRAM_WB_REG_Write+0x7c>)
30008fac:	4411      	add	r1, r2
30008fae:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008fb2:	428a      	cmp	r2, r1
30008fb4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008fb8:	d1f9      	bne.n	30008fae <PSRAM_WB_REG_Write+0x4e>
30008fba:	4a08      	ldr	r2, [pc, #32]	; (30008fdc <PSRAM_WB_REG_Write+0x7c>)
30008fbc:	2301      	movs	r3, #1
30008fbe:	6093      	str	r3, [r2, #8]
30008fc0:	6893      	ldr	r3, [r2, #8]
30008fc2:	07d9      	lsls	r1, r3, #31
30008fc4:	d4fc      	bmi.n	30008fc0 <PSRAM_WB_REG_Write+0x60>
30008fc6:	4a05      	ldr	r2, [pc, #20]	; (30008fdc <PSRAM_WB_REG_Write+0x7c>)
30008fc8:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008fca:	07db      	lsls	r3, r3, #31
30008fcc:	d4fc      	bmi.n	30008fc8 <PSRAM_WB_REG_Write+0x68>
30008fce:	6813      	ldr	r3, [r2, #0]
30008fd0:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008fd4:	6013      	str	r3, [r2, #0]
30008fd6:	bc30      	pop	{r4, r5}
30008fd8:	4770      	bx	lr
30008fda:	bf00      	nop
30008fdc:	41012000 	.word	0x41012000

30008fe0 <PSRAM_WB_DEVIC_Init>:
30008fe0:	490b      	ldr	r1, [pc, #44]	; (30009010 <PSRAM_WB_DEVIC_Init+0x30>)
30008fe2:	4b0c      	ldr	r3, [pc, #48]	; (30009014 <PSRAM_WB_DEVIC_Init+0x34>)
30008fe4:	b510      	push	{r4, lr}
30008fe6:	6988      	ldr	r0, [r1, #24]
30008fe8:	b082      	sub	sp, #8
30008fea:	248f      	movs	r4, #143	; 0x8f
30008fec:	2102      	movs	r1, #2
30008fee:	4403      	add	r3, r0
30008ff0:	aa01      	add	r2, sp, #4
30008ff2:	f88d 4004 	strb.w	r4, [sp, #4]
30008ff6:	2000      	movs	r0, #0
30008ff8:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30008ffc:	4c06      	ldr	r4, [pc, #24]	; (30009018 <PSRAM_WB_DEVIC_Init+0x38>)
30008ffe:	011b      	lsls	r3, r3, #4
30009000:	f043 030c 	orr.w	r3, r3, #12
30009004:	f88d 3005 	strb.w	r3, [sp, #5]
30009008:	47a0      	blx	r4
3000900a:	b002      	add	sp, #8
3000900c:	bd10      	pop	{r4, pc}
3000900e:	bf00      	nop
30009010:	3000fbb0 	.word	0x3000fbb0
30009014:	3000f058 	.word	0x3000f058
30009018:	30008f61 	.word	0x30008f61

3000901c <PSRAM_calibration>:
3000901c:	4a9c      	ldr	r2, [pc, #624]	; (30009290 <PSRAM_calibration+0x274>)
3000901e:	2300      	movs	r3, #0
30009020:	499c      	ldr	r1, [pc, #624]	; (30009294 <PSRAM_calibration+0x278>)
30009022:	489d      	ldr	r0, [pc, #628]	; (30009298 <PSRAM_calibration+0x27c>)
30009024:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009028:	6852      	ldr	r2, [r2, #4]
3000902a:	b093      	sub	sp, #76	; 0x4c
3000902c:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
30009030:	400a      	ands	r2, r1
30009032:	9311      	str	r3, [sp, #68]	; 0x44
30009034:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
30009038:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
3000903c:	920a      	str	r2, [sp, #40]	; 0x28
3000903e:	6942      	ldr	r2, [r0, #20]
30009040:	03d1      	lsls	r1, r2, #15
30009042:	d51e      	bpl.n	30009082 <PSRAM_calibration+0x66>
30009044:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009048:	f3bf 8f4f 	dsb	sy
3000904c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009050:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009054:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009058:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000905c:	0164      	lsls	r4, r4, #5
3000905e:	ea04 0106 	and.w	r1, r4, r6
30009062:	462b      	mov	r3, r5
30009064:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009068:	3b01      	subs	r3, #1
3000906a:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000906e:	1c5a      	adds	r2, r3, #1
30009070:	d1f8      	bne.n	30009064 <PSRAM_calibration+0x48>
30009072:	3c20      	subs	r4, #32
30009074:	f114 0f20 	cmn.w	r4, #32
30009078:	d1f1      	bne.n	3000905e <PSRAM_calibration+0x42>
3000907a:	f3bf 8f4f 	dsb	sy
3000907e:	f3bf 8f6f 	isb	sy
30009082:	4885      	ldr	r0, [pc, #532]	; (30009298 <PSRAM_calibration+0x27c>)
30009084:	2300      	movs	r3, #0
30009086:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000908a:	f3bf 8f4f 	dsb	sy
3000908e:	6943      	ldr	r3, [r0, #20]
30009090:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009094:	6143      	str	r3, [r0, #20]
30009096:	f3bf 8f4f 	dsb	sy
3000909a:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000909e:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300090a2:	f3c5 344e 	ubfx	r4, r5, #13, #15
300090a6:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300090aa:	0164      	lsls	r4, r4, #5
300090ac:	ea04 0106 	and.w	r1, r4, r6
300090b0:	462b      	mov	r3, r5
300090b2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300090b6:	3b01      	subs	r3, #1
300090b8:	1c5f      	adds	r7, r3, #1
300090ba:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300090be:	d1f8      	bne.n	300090b2 <PSRAM_calibration+0x96>
300090c0:	3c20      	subs	r4, #32
300090c2:	f114 0f20 	cmn.w	r4, #32
300090c6:	d1f1      	bne.n	300090ac <PSRAM_calibration+0x90>
300090c8:	f3bf 8f4f 	dsb	sy
300090cc:	f3bf 8f6f 	isb	sy
300090d0:	2204      	movs	r2, #4
300090d2:	461e      	mov	r6, r3
300090d4:	9306      	str	r3, [sp, #24]
300090d6:	2700      	movs	r7, #0
300090d8:	9209      	str	r2, [sp, #36]	; 0x24
300090da:	46b3      	mov	fp, r6
300090dc:	4a6c      	ldr	r2, [pc, #432]	; (30009290 <PSRAM_calibration+0x274>)
300090de:	4c6f      	ldr	r4, [pc, #444]	; (3000929c <PSRAM_calibration+0x280>)
300090e0:	6813      	ldr	r3, [r2, #0]
300090e2:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 300092b8 <PSRAM_calibration+0x29c>
300090e6:	f023 0301 	bic.w	r3, r3, #1
300090ea:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 300092bc <PSRAM_calibration+0x2a0>
300090ee:	6013      	str	r3, [r2, #0]
300090f0:	2301      	movs	r3, #1
300090f2:	9305      	str	r3, [sp, #20]
300090f4:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300090f6:	9a05      	ldr	r2, [sp, #20]
300090f8:	9704      	str	r7, [sp, #16]
300090fa:	2700      	movs	r7, #0
300090fc:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009100:	463d      	mov	r5, r7
30009102:	e9cd b607 	strd	fp, r6, [sp, #28]
30009106:	f04f 3bff 	mov.w	fp, #4294967295
3000910a:	465e      	mov	r6, fp
3000910c:	e010      	b.n	30009130 <PSRAM_calibration+0x114>
3000910e:	f1b6 3fff 	cmp.w	r6, #4294967295
30009112:	bf08      	it	eq
30009114:	462e      	moveq	r6, r5
30009116:	2d1f      	cmp	r5, #31
30009118:	eb06 0b07 	add.w	fp, r6, r7
3000911c:	f107 0701 	add.w	r7, r7, #1
30009120:	d102      	bne.n	30009128 <PSRAM_calibration+0x10c>
30009122:	9b04      	ldr	r3, [sp, #16]
30009124:	429f      	cmp	r7, r3
30009126:	dc46      	bgt.n	300091b6 <PSRAM_calibration+0x19a>
30009128:	3501      	adds	r5, #1
3000912a:	2d20      	cmp	r5, #32
3000912c:	f000 80ac 	beq.w	30009288 <PSRAM_calibration+0x26c>
30009130:	ea48 0205 	orr.w	r2, r8, r5
30009134:	4b56      	ldr	r3, [pc, #344]	; (30009290 <PSRAM_calibration+0x274>)
30009136:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000913a:	4959      	ldr	r1, [pc, #356]	; (300092a0 <PSRAM_calibration+0x284>)
3000913c:	605a      	str	r2, [r3, #4]
3000913e:	6822      	ldr	r2, [r4, #0]
30009140:	4b58      	ldr	r3, [pc, #352]	; (300092a4 <PSRAM_calibration+0x288>)
30009142:	f8ce 2000 	str.w	r2, [lr]
30009146:	6860      	ldr	r0, [r4, #4]
30009148:	4a57      	ldr	r2, [pc, #348]	; (300092a8 <PSRAM_calibration+0x28c>)
3000914a:	f8c9 0000 	str.w	r0, [r9]
3000914e:	68a0      	ldr	r0, [r4, #8]
30009150:	f8ca 0000 	str.w	r0, [sl]
30009154:	68e0      	ldr	r0, [r4, #12]
30009156:	6018      	str	r0, [r3, #0]
30009158:	6920      	ldr	r0, [r4, #16]
3000915a:	6008      	str	r0, [r1, #0]
3000915c:	6960      	ldr	r0, [r4, #20]
3000915e:	6010      	str	r0, [r2, #0]
30009160:	f8de 0000 	ldr.w	r0, [lr]
30009164:	900c      	str	r0, [sp, #48]	; 0x30
30009166:	f8d9 0000 	ldr.w	r0, [r9]
3000916a:	900d      	str	r0, [sp, #52]	; 0x34
3000916c:	f8da 0000 	ldr.w	r0, [sl]
30009170:	900e      	str	r0, [sp, #56]	; 0x38
30009172:	6818      	ldr	r0, [r3, #0]
30009174:	4b4d      	ldr	r3, [pc, #308]	; (300092ac <PSRAM_calibration+0x290>)
30009176:	900f      	str	r0, [sp, #60]	; 0x3c
30009178:	a80c      	add	r0, sp, #48	; 0x30
3000917a:	6809      	ldr	r1, [r1, #0]
3000917c:	9110      	str	r1, [sp, #64]	; 0x40
3000917e:	6812      	ldr	r2, [r2, #0]
30009180:	4946      	ldr	r1, [pc, #280]	; (3000929c <PSRAM_calibration+0x280>)
30009182:	9211      	str	r2, [sp, #68]	; 0x44
30009184:	2218      	movs	r2, #24
30009186:	4798      	blx	r3
30009188:	2800      	cmp	r0, #0
3000918a:	d0c0      	beq.n	3000910e <PSRAM_calibration+0xf2>
3000918c:	1c70      	adds	r0, r6, #1
3000918e:	d0cb      	beq.n	30009128 <PSRAM_calibration+0x10c>
30009190:	9b04      	ldr	r3, [sp, #16]
30009192:	42bb      	cmp	r3, r7
30009194:	da0a      	bge.n	300091ac <PSRAM_calibration+0x190>
30009196:	9b05      	ldr	r3, [sp, #20]
30009198:	f8cd b01c 	str.w	fp, [sp, #28]
3000919c:	f04f 3bff 	mov.w	fp, #4294967295
300091a0:	9704      	str	r7, [sp, #16]
300091a2:	2700      	movs	r7, #0
300091a4:	9608      	str	r6, [sp, #32]
300091a6:	465e      	mov	r6, fp
300091a8:	9306      	str	r3, [sp, #24]
300091aa:	e7bd      	b.n	30009128 <PSRAM_calibration+0x10c>
300091ac:	f04f 3bff 	mov.w	fp, #4294967295
300091b0:	2700      	movs	r7, #0
300091b2:	465e      	mov	r6, fp
300091b4:	e7b8      	b.n	30009128 <PSRAM_calibration+0x10c>
300091b6:	9b05      	ldr	r3, [sp, #20]
300091b8:	9306      	str	r3, [sp, #24]
300091ba:	9b05      	ldr	r3, [sp, #20]
300091bc:	005b      	lsls	r3, r3, #1
300091be:	9305      	str	r3, [sp, #20]
300091c0:	9b09      	ldr	r3, [sp, #36]	; 0x24
300091c2:	3b01      	subs	r3, #1
300091c4:	9309      	str	r3, [sp, #36]	; 0x24
300091c6:	d195      	bne.n	300090f4 <PSRAM_calibration+0xd8>
300091c8:	9b06      	ldr	r3, [sp, #24]
300091ca:	2004      	movs	r0, #4
300091cc:	4a38      	ldr	r2, [pc, #224]	; (300092b0 <PSRAM_calibration+0x294>)
300091ce:	e9cd 7302 	strd	r7, r3, [sp, #8]
300091d2:	4b38      	ldr	r3, [pc, #224]	; (300092b4 <PSRAM_calibration+0x298>)
300091d4:	e9cd 6b00 	strd	r6, fp, [sp]
300091d8:	4619      	mov	r1, r3
300091da:	f002 fe2f 	bl	3000be3c <rtk_log_write>
300091de:	482e      	ldr	r0, [pc, #184]	; (30009298 <PSRAM_calibration+0x27c>)
300091e0:	6943      	ldr	r3, [r0, #20]
300091e2:	03d9      	lsls	r1, r3, #15
300091e4:	d425      	bmi.n	30009232 <PSRAM_calibration+0x216>
300091e6:	9b09      	ldr	r3, [sp, #36]	; 0x24
300091e8:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300091ec:	f3bf 8f4f 	dsb	sy
300091f0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300091f4:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
300091f8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300091fc:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009200:	0164      	lsls	r4, r4, #5
30009202:	ea04 010c 	and.w	r1, r4, ip
30009206:	462b      	mov	r3, r5
30009208:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000920c:	3b01      	subs	r3, #1
3000920e:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009212:	1c5a      	adds	r2, r3, #1
30009214:	d1f8      	bne.n	30009208 <PSRAM_calibration+0x1ec>
30009216:	3c20      	subs	r4, #32
30009218:	f114 0f20 	cmn.w	r4, #32
3000921c:	d1f1      	bne.n	30009202 <PSRAM_calibration+0x1e6>
3000921e:	f3bf 8f4f 	dsb	sy
30009222:	6943      	ldr	r3, [r0, #20]
30009224:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30009228:	6143      	str	r3, [r0, #20]
3000922a:	f3bf 8f4f 	dsb	sy
3000922e:	f3bf 8f6f 	isb	sy
30009232:	2f08      	cmp	r7, #8
30009234:	dd24      	ble.n	30009280 <PSRAM_calibration+0x264>
30009236:	ebbb 0306 	subs.w	r3, fp, r6
3000923a:	9806      	ldr	r0, [sp, #24]
3000923c:	bf48      	it	mi
3000923e:	3301      	addmi	r3, #1
30009240:	eb16 060b 	adds.w	r6, r6, fp
30009244:	ea4f 0363 	mov.w	r3, r3, asr #1
30009248:	bf48      	it	mi
3000924a:	3601      	addmi	r6, #1
3000924c:	1e9a      	subs	r2, r3, #2
3000924e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009250:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009254:	0414      	lsls	r4, r2, #16
30009256:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000925a:	0212      	lsls	r2, r2, #8
3000925c:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009260:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009264:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30009268:	2001      	movs	r0, #1
3000926a:	430b      	orrs	r3, r1
3000926c:	4908      	ldr	r1, [pc, #32]	; (30009290 <PSRAM_calibration+0x274>)
3000926e:	4323      	orrs	r3, r4
30009270:	4313      	orrs	r3, r2
30009272:	604b      	str	r3, [r1, #4]
30009274:	680b      	ldr	r3, [r1, #0]
30009276:	4303      	orrs	r3, r0
30009278:	600b      	str	r3, [r1, #0]
3000927a:	b013      	add	sp, #76	; 0x4c
3000927c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009280:	2000      	movs	r0, #0
30009282:	b013      	add	sp, #76	; 0x4c
30009284:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009288:	9f04      	ldr	r7, [sp, #16]
3000928a:	e9dd b607 	ldrd	fp, r6, [sp, #28]
3000928e:	e794      	b.n	300091ba <PSRAM_calibration+0x19e>
30009290:	41002000 	.word	0x41002000
30009294:	0fffffe0 	.word	0x0fffffe0
30009298:	e000ed00 	.word	0xe000ed00
3000929c:	3000f040 	.word	0x3000f040
300092a0:	60200000 	.word	0x60200000
300092a4:	60150000 	.word	0x60150000
300092a8:	60250000 	.word	0x60250000
300092ac:	000129bd 	.word	0x000129bd
300092b0:	3000d6b4 	.word	0x3000d6b4
300092b4:	3000d68c 	.word	0x3000d68c
300092b8:	60050000 	.word	0x60050000
300092bc:	60100000 	.word	0x60100000

300092c0 <PSRAM_AutoGating>:
300092c0:	b1c0      	cbz	r0, 300092f4 <PSRAM_AutoGating+0x34>
300092c2:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300092c6:	4810      	ldr	r0, [pc, #64]	; (30009308 <PSRAM_AutoGating+0x48>)
300092c8:	f3c1 0108 	ubfx	r1, r1, #0, #9
300092cc:	0252      	lsls	r2, r2, #9
300092ce:	b410      	push	{r4}
300092d0:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
300092d4:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
300092d8:	4020      	ands	r0, r4
300092da:	4301      	orrs	r1, r0
300092dc:	430a      	orrs	r2, r1
300092de:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
300092e2:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
300092e6:	f042 0201 	orr.w	r2, r2, #1
300092ea:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300092ee:	f85d 4b04 	ldr.w	r4, [sp], #4
300092f2:	4770      	bx	lr
300092f4:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300092f8:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300092fc:	f023 0301 	bic.w	r3, r3, #1
30009300:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009304:	4770      	bx	lr
30009306:	bf00      	nop
30009308:	ffffc000 	.word	0xffffc000

3000930c <BOOT_Reason>:
3000930c:	4b02      	ldr	r3, [pc, #8]	; (30009318 <BOOT_Reason+0xc>)
3000930e:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009312:	b280      	uxth	r0, r0
30009314:	4770      	bx	lr
30009316:	bf00      	nop
30009318:	42008200 	.word	0x42008200

3000931c <CRYPTO_SetSecurityModeAD>:
3000931c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009320:	b083      	sub	sp, #12
30009322:	460d      	mov	r5, r1
30009324:	4616      	mov	r6, r2
30009326:	4699      	mov	r9, r3
30009328:	4604      	mov	r4, r0
3000932a:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
3000932e:	2800      	cmp	r0, #0
30009330:	f000 80a9 	beq.w	30009486 <CRYPTO_SetSecurityModeAD+0x16a>
30009334:	7823      	ldrb	r3, [r4, #0]
30009336:	2b01      	cmp	r3, #1
30009338:	d003      	beq.n	30009342 <CRYPTO_SetSecurityModeAD+0x26>
3000933a:	2125      	movs	r1, #37	; 0x25
3000933c:	485d      	ldr	r0, [pc, #372]	; (300094b4 <CRYPTO_SetSecurityModeAD+0x198>)
3000933e:	f003 f863 	bl	3000c408 <__io_assert_failed_veneer>
30009342:	2300      	movs	r3, #0
30009344:	1c6a      	adds	r2, r5, #1
30009346:	6065      	str	r5, [r4, #4]
30009348:	8123      	strh	r3, [r4, #8]
3000934a:	d008      	beq.n	3000935e <CRYPTO_SetSecurityModeAD+0x42>
3000934c:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009350:	2b20      	cmp	r3, #32
30009352:	d07b      	beq.n	3000944c <CRYPTO_SetSecurityModeAD+0x130>
30009354:	f085 0580 	eor.w	r5, r5, #128	; 0x80
30009358:	f3c5 15c0 	ubfx	r5, r5, #7, #1
3000935c:	7265      	strb	r5, [r4, #9]
3000935e:	1c73      	adds	r3, r6, #1
30009360:	60e6      	str	r6, [r4, #12]
30009362:	f000 8082 	beq.w	3000946a <CRYPTO_SetSecurityModeAD+0x14e>
30009366:	f006 0303 	and.w	r3, r6, #3
3000936a:	f3c6 0280 	ubfx	r2, r6, #2, #1
3000936e:	7422      	strb	r2, [r4, #16]
30009370:	f1a3 0202 	sub.w	r2, r3, #2
30009374:	2b01      	cmp	r3, #1
30009376:	fab2 f282 	clz	r2, r2
3000937a:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000937e:	7462      	strb	r2, [r4, #17]
30009380:	fab3 f283 	clz	r2, r3
30009384:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009388:	74a2      	strb	r2, [r4, #18]
3000938a:	bf0c      	ite	eq
3000938c:	2201      	moveq	r2, #1
3000938e:	2200      	movne	r2, #0
30009390:	74e2      	strb	r2, [r4, #19]
30009392:	d13f      	bne.n	30009414 <CRYPTO_SetSecurityModeAD+0xf8>
30009394:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
30009398:	2b40      	cmp	r3, #64	; 0x40
3000939a:	f000 8083 	beq.w	300094a4 <CRYPTO_SetSecurityModeAD+0x188>
3000939e:	d86a      	bhi.n	30009476 <CRYPTO_SetSecurityModeAD+0x15a>
300093a0:	2b10      	cmp	r3, #16
300093a2:	d06d      	beq.n	30009480 <CRYPTO_SetSecurityModeAD+0x164>
300093a4:	2b20      	cmp	r3, #32
300093a6:	d173      	bne.n	30009490 <CRYPTO_SetSecurityModeAD+0x174>
300093a8:	7523      	strb	r3, [r4, #20]
300093aa:	62a3      	str	r3, [r4, #40]	; 0x28
300093ac:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300093ae:	6227      	str	r7, [r4, #32]
300093b0:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300093b4:	2f00      	cmp	r7, #0
300093b6:	d038      	beq.n	3000942a <CRYPTO_SetSecurityModeAD+0x10e>
300093b8:	7d23      	ldrb	r3, [r4, #20]
300093ba:	f104 0064 	add.w	r0, r4, #100	; 0x64
300093be:	2b30      	cmp	r3, #48	; 0x30
300093c0:	65e0      	str	r0, [r4, #92]	; 0x5c
300093c2:	d046      	beq.n	30009452 <CRYPTO_SetSecurityModeAD+0x136>
300093c4:	2b40      	cmp	r3, #64	; 0x40
300093c6:	d044      	beq.n	30009452 <CRYPTO_SetSecurityModeAD+0x136>
300093c8:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
300093cc:	2240      	movs	r2, #64	; 0x40
300093ce:	2136      	movs	r1, #54	; 0x36
300093d0:	4d39      	ldr	r5, [pc, #228]	; (300094b8 <CRYPTO_SetSecurityModeAD+0x19c>)
300093d2:	6623      	str	r3, [r4, #96]	; 0x60
300093d4:	47a8      	blx	r5
300093d6:	2240      	movs	r2, #64	; 0x40
300093d8:	215c      	movs	r1, #92	; 0x5c
300093da:	6e20      	ldr	r0, [r4, #96]	; 0x60
300093dc:	47a8      	blx	r5
300093de:	4b37      	ldr	r3, [pc, #220]	; (300094bc <CRYPTO_SetSecurityModeAD+0x1a0>)
300093e0:	681d      	ldr	r5, [r3, #0]
300093e2:	bb25      	cbnz	r5, 3000942e <CRYPTO_SetSecurityModeAD+0x112>
300093e4:	f1b8 0f00 	cmp.w	r8, #0
300093e8:	d05f      	beq.n	300094aa <CRYPTO_SetSecurityModeAD+0x18e>
300093ea:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
300093ee:	e001      	b.n	300093f4 <CRYPTO_SetSecurityModeAD+0xd8>
300093f0:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
300093f4:	6de2      	ldr	r2, [r4, #92]	; 0x5c
300093f6:	f818 3005 	ldrb.w	r3, [r8, r5]
300093fa:	5d51      	ldrb	r1, [r2, r5]
300093fc:	404b      	eors	r3, r1
300093fe:	5553      	strb	r3, [r2, r5]
30009400:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009402:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009404:	5d51      	ldrb	r1, [r2, r5]
30009406:	5d5b      	ldrb	r3, [r3, r5]
30009408:	404b      	eors	r3, r1
3000940a:	5553      	strb	r3, [r2, r5]
3000940c:	3501      	adds	r5, #1
3000940e:	42af      	cmp	r7, r5
30009410:	d1ee      	bne.n	300093f0 <CRYPTO_SetSecurityModeAD+0xd4>
30009412:	e00c      	b.n	3000942e <CRYPTO_SetSecurityModeAD+0x112>
30009414:	2200      	movs	r2, #0
30009416:	7522      	strb	r2, [r4, #20]
30009418:	b99b      	cbnz	r3, 30009442 <CRYPTO_SetSecurityModeAD+0x126>
3000941a:	2314      	movs	r3, #20
3000941c:	6227      	str	r7, [r4, #32]
3000941e:	62a3      	str	r3, [r4, #40]	; 0x28
30009420:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009422:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009426:	2f00      	cmp	r7, #0
30009428:	d1c6      	bne.n	300093b8 <CRYPTO_SetSecurityModeAD+0x9c>
3000942a:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
3000942e:	3601      	adds	r6, #1
30009430:	d003      	beq.n	3000943a <CRYPTO_SetSecurityModeAD+0x11e>
30009432:	2201      	movs	r2, #1
30009434:	2300      	movs	r3, #0
30009436:	85a2      	strh	r2, [r4, #44]	; 0x2c
30009438:	6323      	str	r3, [r4, #48]	; 0x30
3000943a:	2000      	movs	r0, #0
3000943c:	b003      	add	sp, #12
3000943e:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009442:	2b02      	cmp	r3, #2
30009444:	d114      	bne.n	30009470 <CRYPTO_SetSecurityModeAD+0x154>
30009446:	2310      	movs	r3, #16
30009448:	62a3      	str	r3, [r4, #40]	; 0x28
3000944a:	e7af      	b.n	300093ac <CRYPTO_SetSecurityModeAD+0x90>
3000944c:	2301      	movs	r3, #1
3000944e:	7223      	strb	r3, [r4, #8]
30009450:	e780      	b.n	30009354 <CRYPTO_SetSecurityModeAD+0x38>
30009452:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
30009456:	2280      	movs	r2, #128	; 0x80
30009458:	2136      	movs	r1, #54	; 0x36
3000945a:	4d17      	ldr	r5, [pc, #92]	; (300094b8 <CRYPTO_SetSecurityModeAD+0x19c>)
3000945c:	6623      	str	r3, [r4, #96]	; 0x60
3000945e:	47a8      	blx	r5
30009460:	2280      	movs	r2, #128	; 0x80
30009462:	215c      	movs	r1, #92	; 0x5c
30009464:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009466:	47a8      	blx	r5
30009468:	e7b9      	b.n	300093de <CRYPTO_SetSecurityModeAD+0xc2>
3000946a:	2300      	movs	r3, #0
3000946c:	6123      	str	r3, [r4, #16]
3000946e:	7523      	strb	r3, [r4, #20]
30009470:	2300      	movs	r3, #0
30009472:	62a3      	str	r3, [r4, #40]	; 0x28
30009474:	e79a      	b.n	300093ac <CRYPTO_SetSecurityModeAD+0x90>
30009476:	2b80      	cmp	r3, #128	; 0x80
30009478:	d10a      	bne.n	30009490 <CRYPTO_SetSecurityModeAD+0x174>
3000947a:	2340      	movs	r3, #64	; 0x40
3000947c:	7523      	strb	r3, [r4, #20]
3000947e:	e794      	b.n	300093aa <CRYPTO_SetSecurityModeAD+0x8e>
30009480:	231c      	movs	r3, #28
30009482:	7523      	strb	r3, [r4, #20]
30009484:	e7f5      	b.n	30009472 <CRYPTO_SetSecurityModeAD+0x156>
30009486:	2124      	movs	r1, #36	; 0x24
30009488:	480a      	ldr	r0, [pc, #40]	; (300094b4 <CRYPTO_SetSecurityModeAD+0x198>)
3000948a:	f002 ffbd 	bl	3000c408 <__io_assert_failed_veneer>
3000948e:	e751      	b.n	30009334 <CRYPTO_SetSecurityModeAD+0x18>
30009490:	4b0b      	ldr	r3, [pc, #44]	; (300094c0 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009492:	2002      	movs	r0, #2
30009494:	4a0b      	ldr	r2, [pc, #44]	; (300094c4 <CRYPTO_SetSecurityModeAD+0x1a8>)
30009496:	4619      	mov	r1, r3
30009498:	9600      	str	r6, [sp, #0]
3000949a:	f002 fccf 	bl	3000be3c <rtk_log_write>
3000949e:	2300      	movs	r3, #0
300094a0:	7523      	strb	r3, [r4, #20]
300094a2:	e7e6      	b.n	30009472 <CRYPTO_SetSecurityModeAD+0x156>
300094a4:	2330      	movs	r3, #48	; 0x30
300094a6:	7523      	strb	r3, [r4, #20]
300094a8:	e7e3      	b.n	30009472 <CRYPTO_SetSecurityModeAD+0x156>
300094aa:	2180      	movs	r1, #128	; 0x80
300094ac:	4801      	ldr	r0, [pc, #4]	; (300094b4 <CRYPTO_SetSecurityModeAD+0x198>)
300094ae:	f002 ffab 	bl	3000c408 <__io_assert_failed_veneer>
300094b2:	e79a      	b.n	300093ea <CRYPTO_SetSecurityModeAD+0xce>
300094b4:	3000d790 	.word	0x3000d790
300094b8:	00012be5 	.word	0x00012be5
300094bc:	2001c200 	.word	0x2001c200
300094c0:	3000d6f4 	.word	0x3000d6f4
300094c4:	3000d700 	.word	0x3000d700

300094c8 <rtl_crypto_hmac_sha2_init>:
300094c8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300094cc:	4c97      	ldr	r4, [pc, #604]	; (3000972c <rtl_crypto_hmac_sha2_init+0x264>)
300094ce:	4617      	mov	r7, r2
300094d0:	e844 f400 	tt	r4, r4
300094d4:	4a96      	ldr	r2, [pc, #600]	; (30009730 <rtl_crypto_hmac_sha2_init+0x268>)
300094d6:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
300094da:	461c      	mov	r4, r3
300094dc:	4b95      	ldr	r3, [pc, #596]	; (30009734 <rtl_crypto_hmac_sha2_init+0x26c>)
300094de:	bf18      	it	ne
300094e0:	4613      	movne	r3, r2
300094e2:	b085      	sub	sp, #20
300094e4:	4605      	mov	r5, r0
300094e6:	460e      	mov	r6, r1
300094e8:	6b5b      	ldr	r3, [r3, #52]	; 0x34
300094ea:	2b00      	cmp	r3, #0
300094ec:	db05      	blt.n	300094fa <rtl_crypto_hmac_sha2_init+0x32>
300094ee:	2900      	cmp	r1, #0
300094f0:	f000 8118 	beq.w	30009724 <rtl_crypto_hmac_sha2_init+0x25c>
300094f4:	078b      	lsls	r3, r1, #30
300094f6:	f040 8112 	bne.w	3000971e <rtl_crypto_hmac_sha2_init+0x256>
300094fa:	2d20      	cmp	r5, #32
300094fc:	bf18      	it	ne
300094fe:	2d1c      	cmpne	r5, #28
30009500:	bf0c      	ite	eq
30009502:	f04f 0901 	moveq.w	r9, #1
30009506:	f04f 0900 	movne.w	r9, #0
3000950a:	d04d      	beq.n	300095a8 <rtl_crypto_hmac_sha2_init+0xe0>
3000950c:	2d40      	cmp	r5, #64	; 0x40
3000950e:	bf18      	it	ne
30009510:	2d30      	cmpne	r5, #48	; 0x30
30009512:	d102      	bne.n	3000951a <rtl_crypto_hmac_sha2_init+0x52>
30009514:	2f80      	cmp	r7, #128	; 0x80
30009516:	f200 80f9 	bhi.w	3000970c <rtl_crypto_hmac_sha2_init+0x244>
3000951a:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009754 <rtl_crypto_hmac_sha2_init+0x28c>
3000951e:	f898 3000 	ldrb.w	r3, [r8]
30009522:	2b01      	cmp	r3, #1
30009524:	d003      	beq.n	3000952e <rtl_crypto_hmac_sha2_init+0x66>
30009526:	21b4      	movs	r1, #180	; 0xb4
30009528:	4883      	ldr	r0, [pc, #524]	; (30009738 <rtl_crypto_hmac_sha2_init+0x270>)
3000952a:	f002 ff6d 	bl	3000c408 <__io_assert_failed_veneer>
3000952e:	2d30      	cmp	r5, #48	; 0x30
30009530:	f000 809b 	beq.w	3000966a <rtl_crypto_hmac_sha2_init+0x1a2>
30009534:	d83f      	bhi.n	300095b6 <rtl_crypto_hmac_sha2_init+0xee>
30009536:	2d1c      	cmp	r5, #28
30009538:	f000 80cb 	beq.w	300096d2 <rtl_crypto_hmac_sha2_init+0x20a>
3000953c:	f1b9 0f00 	cmp.w	r9, #0
30009540:	d078      	beq.n	30009634 <rtl_crypto_hmac_sha2_init+0x16c>
30009542:	2c00      	cmp	r4, #0
30009544:	f000 808f 	beq.w	30009666 <rtl_crypto_hmac_sha2_init+0x19e>
30009548:	4b7c      	ldr	r3, [pc, #496]	; (3000973c <rtl_crypto_hmac_sha2_init+0x274>)
3000954a:	f8df e210 	ldr.w	lr, [pc, #528]	; 3000975c <rtl_crypto_hmac_sha2_init+0x294>
3000954e:	f8df 921c 	ldr.w	r9, [pc, #540]	; 3000976c <rtl_crypto_hmac_sha2_init+0x2a4>
30009552:	4a7b      	ldr	r2, [pc, #492]	; (30009740 <rtl_crypto_hmac_sha2_init+0x278>)
30009554:	e9c4 e302 	strd	lr, r3, [r4, #8]
30009558:	4b7a      	ldr	r3, [pc, #488]	; (30009744 <rtl_crypto_hmac_sha2_init+0x27c>)
3000955a:	e9c4 9304 	strd	r9, r3, [r4, #16]
3000955e:	4b7a      	ldr	r3, [pc, #488]	; (30009748 <rtl_crypto_hmac_sha2_init+0x280>)
30009560:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009564:	4b79      	ldr	r3, [pc, #484]	; (3000974c <rtl_crypto_hmac_sha2_init+0x284>)
30009566:	2225      	movs	r2, #37	; 0x25
30009568:	6223      	str	r3, [r4, #32]
3000956a:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
3000956e:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009572:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
30009576:	6263      	str	r3, [r4, #36]	; 0x24
30009578:	2300      	movs	r3, #0
3000957a:	f04f 31ff 	mov.w	r1, #4294967295
3000957e:	4d74      	ldr	r5, [pc, #464]	; (30009750 <rtl_crypto_hmac_sha2_init+0x288>)
30009580:	9300      	str	r3, [sp, #0]
30009582:	9702      	str	r7, [sp, #8]
30009584:	9601      	str	r6, [sp, #4]
30009586:	4873      	ldr	r0, [pc, #460]	; (30009754 <rtl_crypto_hmac_sha2_init+0x28c>)
30009588:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
3000958c:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009590:	47a8      	blx	r5
30009592:	4605      	mov	r5, r0
30009594:	4b70      	ldr	r3, [pc, #448]	; (30009758 <rtl_crypto_hmac_sha2_init+0x290>)
30009596:	4620      	mov	r0, r4
30009598:	4798      	blx	r3
3000959a:	2301      	movs	r3, #1
3000959c:	4628      	mov	r0, r5
3000959e:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
300095a2:	b005      	add	sp, #20
300095a4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300095a8:	2f40      	cmp	r7, #64	; 0x40
300095aa:	d9b6      	bls.n	3000951a <rtl_crypto_hmac_sha2_init+0x52>
300095ac:	21ae      	movs	r1, #174	; 0xae
300095ae:	4862      	ldr	r0, [pc, #392]	; (30009738 <rtl_crypto_hmac_sha2_init+0x270>)
300095b0:	f002 ff2a 	bl	3000c408 <__io_assert_failed_veneer>
300095b4:	e7b1      	b.n	3000951a <rtl_crypto_hmac_sha2_init+0x52>
300095b6:	2d40      	cmp	r5, #64	; 0x40
300095b8:	d13c      	bne.n	30009634 <rtl_crypto_hmac_sha2_init+0x16c>
300095ba:	2c00      	cmp	r4, #0
300095bc:	d040      	beq.n	30009640 <rtl_crypto_hmac_sha2_init+0x178>
300095be:	4b67      	ldr	r3, [pc, #412]	; (3000975c <rtl_crypto_hmac_sha2_init+0x294>)
300095c0:	4a67      	ldr	r2, [pc, #412]	; (30009760 <rtl_crypto_hmac_sha2_init+0x298>)
300095c2:	4968      	ldr	r1, [pc, #416]	; (30009764 <rtl_crypto_hmac_sha2_init+0x29c>)
300095c4:	4d68      	ldr	r5, [pc, #416]	; (30009768 <rtl_crypto_hmac_sha2_init+0x2a0>)
300095c6:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 300097ac <rtl_crypto_hmac_sha2_init+0x2e4>
300095ca:	e9c4 3202 	strd	r3, r2, [r4, #8]
300095ce:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
300095d2:	2285      	movs	r2, #133	; 0x85
300095d4:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
300095d8:	33d4      	adds	r3, #212	; 0xd4
300095da:	6163      	str	r3, [r4, #20]
300095dc:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
300095e0:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
300095e4:	f203 734a 	addw	r3, r3, #1866	; 0x74a
300095e8:	6123      	str	r3, [r4, #16]
300095ea:	4b60      	ldr	r3, [pc, #384]	; (3000976c <rtl_crypto_hmac_sha2_init+0x2a4>)
300095ec:	e9c4 3106 	strd	r3, r1, [r4, #24]
300095f0:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
300095f4:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
300095f8:	f203 337f 	addw	r3, r3, #895	; 0x37f
300095fc:	6263      	str	r3, [r4, #36]	; 0x24
300095fe:	4b51      	ldr	r3, [pc, #324]	; (30009744 <rtl_crypto_hmac_sha2_init+0x27c>)
30009600:	6223      	str	r3, [r4, #32]
30009602:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
30009606:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
3000960a:	f203 5397 	addw	r3, r3, #1431	; 0x597
3000960e:	62e3      	str	r3, [r4, #44]	; 0x2c
30009610:	4b4b      	ldr	r3, [pc, #300]	; (30009740 <rtl_crypto_hmac_sha2_init+0x278>)
30009612:	62a3      	str	r3, [r4, #40]	; 0x28
30009614:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
30009618:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
3000961c:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009620:	6363      	str	r3, [r4, #52]	; 0x34
30009622:	4b49      	ldr	r3, [pc, #292]	; (30009748 <rtl_crypto_hmac_sha2_init+0x280>)
30009624:	6323      	str	r3, [r4, #48]	; 0x30
30009626:	4b49      	ldr	r3, [pc, #292]	; (3000974c <rtl_crypto_hmac_sha2_init+0x284>)
30009628:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000962c:	4b50      	ldr	r3, [pc, #320]	; (30009770 <rtl_crypto_hmac_sha2_init+0x2a8>)
3000962e:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009632:	e7a1      	b.n	30009578 <rtl_crypto_hmac_sha2_init+0xb0>
30009634:	f04f 35ff 	mov.w	r5, #4294967295
30009638:	4628      	mov	r0, r5
3000963a:	b005      	add	sp, #20
3000963c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009640:	2285      	movs	r2, #133	; 0x85
30009642:	2300      	movs	r3, #0
30009644:	f04f 31ff 	mov.w	r1, #4294967295
30009648:	4c41      	ldr	r4, [pc, #260]	; (30009750 <rtl_crypto_hmac_sha2_init+0x288>)
3000964a:	9702      	str	r7, [sp, #8]
3000964c:	4841      	ldr	r0, [pc, #260]	; (30009754 <rtl_crypto_hmac_sha2_init+0x28c>)
3000964e:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009652:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009656:	e9cd 3600 	strd	r3, r6, [sp]
3000965a:	47a0      	blx	r4
3000965c:	4605      	mov	r5, r0
3000965e:	4628      	mov	r0, r5
30009660:	b005      	add	sp, #20
30009662:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009666:	2225      	movs	r2, #37	; 0x25
30009668:	e7eb      	b.n	30009642 <rtl_crypto_hmac_sha2_init+0x17a>
3000966a:	2c00      	cmp	r4, #0
3000966c:	d053      	beq.n	30009716 <rtl_crypto_hmac_sha2_init+0x24e>
3000966e:	4b41      	ldr	r3, [pc, #260]	; (30009774 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009670:	f8df e130 	ldr.w	lr, [pc, #304]	; 300097a4 <rtl_crypto_hmac_sha2_init+0x2dc>
30009674:	4a40      	ldr	r2, [pc, #256]	; (30009778 <rtl_crypto_hmac_sha2_init+0x2b0>)
30009676:	4941      	ldr	r1, [pc, #260]	; (3000977c <rtl_crypto_hmac_sha2_init+0x2b4>)
30009678:	4d41      	ldr	r5, [pc, #260]	; (30009780 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000967a:	f8df c12c 	ldr.w	ip, [pc, #300]	; 300097a8 <rtl_crypto_hmac_sha2_init+0x2e0>
3000967e:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009682:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
30009686:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000968a:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
3000968e:	6163      	str	r3, [r4, #20]
30009690:	4b3c      	ldr	r3, [pc, #240]	; (30009784 <rtl_crypto_hmac_sha2_init+0x2bc>)
30009692:	6123      	str	r3, [r4, #16]
30009694:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
30009698:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
3000969c:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
300096a0:	61e3      	str	r3, [r4, #28]
300096a2:	4b39      	ldr	r3, [pc, #228]	; (30009788 <rtl_crypto_hmac_sha2_init+0x2c0>)
300096a4:	61a3      	str	r3, [r4, #24]
300096a6:	4b39      	ldr	r3, [pc, #228]	; (3000978c <rtl_crypto_hmac_sha2_init+0x2c4>)
300096a8:	e9c4 3208 	strd	r3, r2, [r4, #32]
300096ac:	4b38      	ldr	r3, [pc, #224]	; (30009790 <rtl_crypto_hmac_sha2_init+0x2c8>)
300096ae:	2245      	movs	r2, #69	; 0x45
300096b0:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
300096b4:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
300096b8:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
300096bc:	33aa      	adds	r3, #170	; 0xaa
300096be:	6363      	str	r3, [r4, #52]	; 0x34
300096c0:	4b34      	ldr	r3, [pc, #208]	; (30009794 <rtl_crypto_hmac_sha2_init+0x2cc>)
300096c2:	6323      	str	r3, [r4, #48]	; 0x30
300096c4:	4b34      	ldr	r3, [pc, #208]	; (30009798 <rtl_crypto_hmac_sha2_init+0x2d0>)
300096c6:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
300096ca:	4b34      	ldr	r3, [pc, #208]	; (3000979c <rtl_crypto_hmac_sha2_init+0x2d4>)
300096cc:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
300096d0:	e752      	b.n	30009578 <rtl_crypto_hmac_sha2_init+0xb0>
300096d2:	b314      	cbz	r4, 3000971a <rtl_crypto_hmac_sha2_init+0x252>
300096d4:	4b32      	ldr	r3, [pc, #200]	; (300097a0 <rtl_crypto_hmac_sha2_init+0x2d8>)
300096d6:	2215      	movs	r2, #21
300096d8:	4932      	ldr	r1, [pc, #200]	; (300097a4 <rtl_crypto_hmac_sha2_init+0x2dc>)
300096da:	4827      	ldr	r0, [pc, #156]	; (30009778 <rtl_crypto_hmac_sha2_init+0x2b0>)
300096dc:	4d28      	ldr	r5, [pc, #160]	; (30009780 <rtl_crypto_hmac_sha2_init+0x2b8>)
300096de:	e9c4 1302 	strd	r1, r3, [r4, #8]
300096e2:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
300096e6:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
300096ea:	f503 6301 	add.w	r3, r3, #2064	; 0x810
300096ee:	6123      	str	r3, [r4, #16]
300096f0:	4b22      	ldr	r3, [pc, #136]	; (3000977c <rtl_crypto_hmac_sha2_init+0x2b4>)
300096f2:	e9c4 0305 	strd	r0, r3, [r4, #20]
300096f6:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
300096fa:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
300096fe:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009702:	61e3      	str	r3, [r4, #28]
30009704:	4b28      	ldr	r3, [pc, #160]	; (300097a8 <rtl_crypto_hmac_sha2_init+0x2e0>)
30009706:	e9c4 5308 	strd	r5, r3, [r4, #32]
3000970a:	e735      	b.n	30009578 <rtl_crypto_hmac_sha2_init+0xb0>
3000970c:	21b1      	movs	r1, #177	; 0xb1
3000970e:	480a      	ldr	r0, [pc, #40]	; (30009738 <rtl_crypto_hmac_sha2_init+0x270>)
30009710:	f002 fe7a 	bl	3000c408 <__io_assert_failed_veneer>
30009714:	e701      	b.n	3000951a <rtl_crypto_hmac_sha2_init+0x52>
30009716:	2245      	movs	r2, #69	; 0x45
30009718:	e793      	b.n	30009642 <rtl_crypto_hmac_sha2_init+0x17a>
3000971a:	2215      	movs	r2, #21
3000971c:	e791      	b.n	30009642 <rtl_crypto_hmac_sha2_init+0x17a>
3000971e:	f06f 0505 	mvn.w	r5, #5
30009722:	e789      	b.n	30009638 <rtl_crypto_hmac_sha2_init+0x170>
30009724:	f06f 0503 	mvn.w	r5, #3
30009728:	e786      	b.n	30009638 <rtl_crypto_hmac_sha2_init+0x170>
3000972a:	bf00      	nop
3000972c:	0000e5f9 	.word	0x0000e5f9
30009730:	500c8000 	.word	0x500c8000
30009734:	400c8000 	.word	0x400c8000
30009738:	3000d774 	.word	0x3000d774
3000973c:	bb67ae85 	.word	0xbb67ae85
30009740:	510e527f 	.word	0x510e527f
30009744:	a54ff53a 	.word	0xa54ff53a
30009748:	9b05688c 	.word	0x9b05688c
3000974c:	1f83d9ab 	.word	0x1f83d9ab
30009750:	3000931d 	.word	0x3000931d
30009754:	2001c460 	.word	0x2001c460
30009758:	00004c99 	.word	0x00004c99
3000975c:	6a09e667 	.word	0x6a09e667
30009760:	f3bcc908 	.word	0xf3bcc908
30009764:	fe94f82b 	.word	0xfe94f82b
30009768:	fb41bd6b 	.word	0xfb41bd6b
3000976c:	3c6ef372 	.word	0x3c6ef372
30009770:	5be0cd19 	.word	0x5be0cd19
30009774:	cbbb9d5d 	.word	0xcbbb9d5d
30009778:	f70e5939 	.word	0xf70e5939
3000977c:	ffc00b31 	.word	0xffc00b31
30009780:	64f98fa7 	.word	0x64f98fa7
30009784:	629a292a 	.word	0x629a292a
30009788:	9159015a 	.word	0x9159015a
3000978c:	152fecd8 	.word	0x152fecd8
30009790:	67332667 	.word	0x67332667
30009794:	8eb44a87 	.word	0x8eb44a87
30009798:	db0c2e0d 	.word	0xdb0c2e0d
3000979c:	47b5481d 	.word	0x47b5481d
300097a0:	367cd507 	.word	0x367cd507
300097a4:	c1059ed8 	.word	0xc1059ed8
300097a8:	befa4fa4 	.word	0xbefa4fa4
300097ac:	137e2179 	.word	0x137e2179

300097b0 <CPU_ClkGet>:
300097b0:	b570      	push	{r4, r5, r6, lr}
300097b2:	4d1e      	ldr	r5, [pc, #120]	; (3000982c <CPU_ClkGet+0x7c>)
300097b4:	4b1e      	ldr	r3, [pc, #120]	; (30009830 <CPU_ClkGet+0x80>)
300097b6:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
300097ba:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
300097be:	f3c4 1481 	ubfx	r4, r4, #6, #2
300097c2:	4798      	blx	r3
300097c4:	b92c      	cbnz	r4, 300097d2 <CPU_ClkGet+0x22>
300097c6:	2801      	cmp	r0, #1
300097c8:	d007      	beq.n	300097da <CPU_ClkGet+0x2a>
300097ca:	07b2      	lsls	r2, r6, #30
300097cc:	d407      	bmi.n	300097de <CPU_ClkGet+0x2e>
300097ce:	4819      	ldr	r0, [pc, #100]	; (30009834 <CPU_ClkGet+0x84>)
300097d0:	bd70      	pop	{r4, r5, r6, pc}
300097d2:	2c01      	cmp	r4, #1
300097d4:	d007      	beq.n	300097e6 <CPU_ClkGet+0x36>
300097d6:	2000      	movs	r0, #0
300097d8:	bd70      	pop	{r4, r5, r6, pc}
300097da:	4817      	ldr	r0, [pc, #92]	; (30009838 <CPU_ClkGet+0x88>)
300097dc:	bd70      	pop	{r4, r5, r6, pc}
300097de:	4b17      	ldr	r3, [pc, #92]	; (3000983c <CPU_ClkGet+0x8c>)
300097e0:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
300097e4:	4718      	bx	r3
300097e6:	2801      	cmp	r0, #1
300097e8:	d011      	beq.n	3000980e <CPU_ClkGet+0x5e>
300097ea:	05f3      	lsls	r3, r6, #23
300097ec:	d51c      	bpl.n	30009828 <CPU_ClkGet+0x78>
300097ee:	4914      	ldr	r1, [pc, #80]	; (30009840 <CPU_ClkGet+0x90>)
300097f0:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
300097f4:	6c48      	ldr	r0, [r1, #68]	; 0x44
300097f6:	4a13      	ldr	r2, [pc, #76]	; (30009844 <CPU_ClkGet+0x94>)
300097f8:	f003 0307 	and.w	r3, r3, #7
300097fc:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009800:	3301      	adds	r3, #1
30009802:	3002      	adds	r0, #2
30009804:	fb02 f000 	mul.w	r0, r2, r0
30009808:	fbb0 f0f3 	udiv	r0, r0, r3
3000980c:	bd70      	pop	{r4, r5, r6, pc}
3000980e:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009812:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
30009816:	f3c3 4007 	ubfx	r0, r3, #16, #8
3000981a:	d003      	beq.n	30009824 <CPU_ClkGet+0x74>
3000981c:	4b0a      	ldr	r3, [pc, #40]	; (30009848 <CPU_ClkGet+0x98>)
3000981e:	fb03 f000 	mul.w	r0, r3, r0
30009822:	bd70      	pop	{r4, r5, r6, pc}
30009824:	4809      	ldr	r0, [pc, #36]	; (3000984c <CPU_ClkGet+0x9c>)
30009826:	bd70      	pop	{r4, r5, r6, pc}
30009828:	4806      	ldr	r0, [pc, #24]	; (30009844 <CPU_ClkGet+0x94>)
3000982a:	bd70      	pop	{r4, r5, r6, pc}
3000982c:	42008000 	.word	0x42008000
30009830:	0000c0f9 	.word	0x0000c0f9
30009834:	003d0900 	.word	0x003d0900
30009838:	01312d00 	.word	0x01312d00
3000983c:	000099f5 	.word	0x000099f5
30009840:	42008800 	.word	0x42008800
30009844:	02625a00 	.word	0x02625a00
30009848:	000f4240 	.word	0x000f4240
3000984c:	02faf080 	.word	0x02faf080

30009850 <RSIP_IV_Set>:
30009850:	4b05      	ldr	r3, [pc, #20]	; (30009868 <RSIP_IV_Set+0x18>)
30009852:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
30009856:	680b      	ldr	r3, [r1, #0]
30009858:	ba1b      	rev	r3, r3
3000985a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000985e:	684b      	ldr	r3, [r1, #4]
30009860:	ba1b      	rev	r3, r3
30009862:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
30009866:	4770      	bx	lr
30009868:	42008c00 	.word	0x42008c00

3000986c <RSIP_OTF_Cmd>:
3000986c:	4a04      	ldr	r2, [pc, #16]	; (30009880 <RSIP_OTF_Cmd+0x14>)
3000986e:	2801      	cmp	r0, #1
30009870:	6953      	ldr	r3, [r2, #20]
30009872:	bf0c      	ite	eq
30009874:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
30009878:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
3000987c:	6153      	str	r3, [r2, #20]
3000987e:	4770      	bx	lr
30009880:	42008c00 	.word	0x42008c00

30009884 <RSIP_OTF_Enable>:
30009884:	b570      	push	{r4, r5, r6, lr}
30009886:	4604      	mov	r4, r0
30009888:	b1f3      	cbz	r3, 300098c8 <RSIP_OTF_Enable+0x44>
3000988a:	9b04      	ldr	r3, [sp, #16]
3000988c:	460d      	mov	r5, r1
3000988e:	4616      	mov	r6, r2
30009890:	2b07      	cmp	r3, #7
30009892:	d827      	bhi.n	300098e4 <RSIP_OTF_Enable+0x60>
30009894:	9b05      	ldr	r3, [sp, #20]
30009896:	3b01      	subs	r3, #1
30009898:	2b01      	cmp	r3, #1
3000989a:	d81d      	bhi.n	300098d8 <RSIP_OTF_Enable+0x54>
3000989c:	9a04      	ldr	r2, [sp, #16]
3000989e:	9b05      	ldr	r3, [sp, #20]
300098a0:	f002 0007 	and.w	r0, r2, #7
300098a4:	005b      	lsls	r3, r3, #1
300098a6:	4305      	orrs	r5, r0
300098a8:	4813      	ldr	r0, [pc, #76]	; (300098f8 <RSIP_OTF_Enable+0x74>)
300098aa:	f003 0306 	and.w	r3, r3, #6
300098ae:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
300098b2:	ea43 0206 	orr.w	r2, r3, r6
300098b6:	6c63      	ldr	r3, [r4, #68]	; 0x44
300098b8:	f042 0201 	orr.w	r2, r2, #1
300098bc:	f023 0301 	bic.w	r3, r3, #1
300098c0:	6463      	str	r3, [r4, #68]	; 0x44
300098c2:	6425      	str	r5, [r4, #64]	; 0x40
300098c4:	6462      	str	r2, [r4, #68]	; 0x44
300098c6:	bd70      	pop	{r4, r5, r6, pc}
300098c8:	480b      	ldr	r0, [pc, #44]	; (300098f8 <RSIP_OTF_Enable+0x74>)
300098ca:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
300098ce:	6c63      	ldr	r3, [r4, #68]	; 0x44
300098d0:	f023 0301 	bic.w	r3, r3, #1
300098d4:	6463      	str	r3, [r4, #68]	; 0x44
300098d6:	bd70      	pop	{r4, r5, r6, pc}
300098d8:	f240 2131 	movw	r1, #561	; 0x231
300098dc:	4807      	ldr	r0, [pc, #28]	; (300098fc <RSIP_OTF_Enable+0x78>)
300098de:	f002 fd93 	bl	3000c408 <__io_assert_failed_veneer>
300098e2:	e7db      	b.n	3000989c <RSIP_OTF_Enable+0x18>
300098e4:	f44f 710c 	mov.w	r1, #560	; 0x230
300098e8:	4804      	ldr	r0, [pc, #16]	; (300098fc <RSIP_OTF_Enable+0x78>)
300098ea:	f002 fd8d 	bl	3000c408 <__io_assert_failed_veneer>
300098ee:	9b05      	ldr	r3, [sp, #20]
300098f0:	3b01      	subs	r3, #1
300098f2:	2b01      	cmp	r3, #1
300098f4:	d9d2      	bls.n	3000989c <RSIP_OTF_Enable+0x18>
300098f6:	e7ef      	b.n	300098d8 <RSIP_OTF_Enable+0x54>
300098f8:	42008c00 	.word	0x42008c00
300098fc:	3000d764 	.word	0x3000d764

30009900 <RSIP_MMU_Config>:
30009900:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009904:	311f      	adds	r1, #31
30009906:	321f      	adds	r2, #31
30009908:	331f      	adds	r3, #31
3000990a:	0080      	lsls	r0, r0, #2
3000990c:	0949      	lsrs	r1, r1, #5
3000990e:	0952      	lsrs	r2, r2, #5
30009910:	095b      	lsrs	r3, r3, #5
30009912:	0249      	lsls	r1, r1, #9
30009914:	0252      	lsls	r2, r2, #9
30009916:	025b      	lsls	r3, r3, #9
30009918:	b410      	push	{r4}
3000991a:	4c08      	ldr	r4, [pc, #32]	; (3000993c <RSIP_MMU_Config+0x3c>)
3000991c:	4404      	add	r4, r0
3000991e:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009922:	f020 0001 	bic.w	r0, r0, #1
30009926:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
3000992a:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
3000992e:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009932:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
30009936:	f85d 4b04 	ldr.w	r4, [sp], #4
3000993a:	4770      	bx	lr
3000993c:	42008c00 	.word	0x42008c00

30009940 <RSIP_MMU_Cmd>:
30009940:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009944:	4b07      	ldr	r3, [pc, #28]	; (30009964 <RSIP_MMU_Cmd+0x24>)
30009946:	0080      	lsls	r0, r0, #2
30009948:	4403      	add	r3, r0
3000994a:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
3000994e:	b121      	cbz	r1, 3000995a <RSIP_MMU_Cmd+0x1a>
30009950:	f042 0201 	orr.w	r2, r2, #1
30009954:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009958:	4770      	bx	lr
3000995a:	f022 0201 	bic.w	r2, r2, #1
3000995e:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009962:	4770      	bx	lr
30009964:	42008c00 	.word	0x42008c00

30009968 <RSIP_MMU_Cache_Clean>:
30009968:	4a02      	ldr	r2, [pc, #8]	; (30009974 <RSIP_MMU_Cache_Clean+0xc>)
3000996a:	6993      	ldr	r3, [r2, #24]
3000996c:	f043 0304 	orr.w	r3, r3, #4
30009970:	6193      	str	r3, [r2, #24]
30009972:	4770      	bx	lr
30009974:	42008c00 	.word	0x42008c00

30009978 <NAND_RxData>:
30009978:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
3000997c:	460c      	mov	r4, r1
3000997e:	4616      	mov	r6, r2
30009980:	461f      	mov	r7, r3
30009982:	2800      	cmp	r0, #0
30009984:	f000 808a 	beq.w	30009a9c <NAND_RxData+0x124>
30009988:	4605      	mov	r5, r0
3000998a:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009aa8 <NAND_RxData+0x130>
3000998e:	2000      	movs	r0, #0
30009990:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009aac <NAND_RxData+0x134>
30009994:	47c0      	blx	r8
30009996:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
3000999a:	2000      	movs	r0, #0
3000999c:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
300099a0:	6819      	ldr	r1, [r3, #0]
300099a2:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
300099a6:	6019      	str	r1, [r3, #0]
300099a8:	6819      	ldr	r1, [r3, #0]
300099aa:	f441 7140 	orr.w	r1, r1, #768	; 0x300
300099ae:	6019      	str	r1, [r3, #0]
300099b0:	605a      	str	r2, [r3, #4]
300099b2:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
300099b6:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
300099ba:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
300099be:	ea4f 420c 	mov.w	r2, ip, lsl #16
300099c2:	f1bc 0f03 	cmp.w	ip, #3
300099c6:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
300099ca:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
300099ce:	ea42 0201 	orr.w	r2, r2, r1
300099d2:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
300099d6:	f04f 0201 	mov.w	r2, #1
300099da:	611a      	str	r2, [r3, #16]
300099dc:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
300099e0:	bf08      	it	eq
300099e2:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
300099e6:	f3c4 2307 	ubfx	r3, r4, #8, #8
300099ea:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
300099ee:	b2e4      	uxtb	r4, r4
300099f0:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
300099f4:	2301      	movs	r3, #1
300099f6:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
300099fa:	f017 0403 	ands.w	r4, r7, #3
300099fe:	6083      	str	r3, [r0, #8]
30009a00:	d137      	bne.n	30009a72 <NAND_RxData+0xfa>
30009a02:	463b      	mov	r3, r7
30009a04:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009a08:	42b4      	cmp	r4, r6
30009a0a:	d216      	bcs.n	30009a3a <NAND_RxData+0xc2>
30009a0c:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30009a10:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009a14:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009a18:	d00c      	beq.n	30009a34 <NAND_RxData+0xbc>
30009a1a:	00ad      	lsls	r5, r5, #2
30009a1c:	1f19      	subs	r1, r3, #4
30009a1e:	f1a5 0c04 	sub.w	ip, r5, #4
30009a22:	449c      	add	ip, r3
30009a24:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009a28:	f841 0f04 	str.w	r0, [r1, #4]!
30009a2c:	458c      	cmp	ip, r1
30009a2e:	d1f9      	bne.n	30009a24 <NAND_RxData+0xac>
30009a30:	442b      	add	r3, r5
30009a32:	442c      	add	r4, r5
30009a34:	1b32      	subs	r2, r6, r4
30009a36:	2a03      	cmp	r2, #3
30009a38:	d8e6      	bhi.n	30009a08 <NAND_RxData+0x90>
30009a3a:	1e63      	subs	r3, r4, #1
30009a3c:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009a40:	441f      	add	r7, r3
30009a42:	e002      	b.n	30009a4a <NAND_RxData+0xd2>
30009a44:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009a46:	071b      	lsls	r3, r3, #28
30009a48:	d40d      	bmi.n	30009a66 <NAND_RxData+0xee>
30009a4a:	42b4      	cmp	r4, r6
30009a4c:	d3fa      	bcc.n	30009a44 <NAND_RxData+0xcc>
30009a4e:	2004      	movs	r0, #4
30009a50:	47c0      	blx	r8
30009a52:	2000      	movs	r0, #0
30009a54:	47c0      	blx	r8
30009a56:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009a5a:	6813      	ldr	r3, [r2, #0]
30009a5c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009a60:	6013      	str	r3, [r2, #0]
30009a62:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009a66:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009a6a:	3401      	adds	r4, #1
30009a6c:	f807 3f01 	strb.w	r3, [r7, #1]!
30009a70:	e7eb      	b.n	30009a4a <NAND_RxData+0xd2>
30009a72:	f1c4 0404 	rsb	r4, r4, #4
30009a76:	2200      	movs	r2, #0
30009a78:	4639      	mov	r1, r7
30009a7a:	e00a      	b.n	30009a92 <NAND_RxData+0x11a>
30009a7c:	6a85      	ldr	r5, [r0, #40]	; 0x28
30009a7e:	072d      	lsls	r5, r5, #28
30009a80:	d505      	bpl.n	30009a8e <NAND_RxData+0x116>
30009a82:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009a86:	3201      	adds	r2, #1
30009a88:	700b      	strb	r3, [r1, #0]
30009a8a:	18b9      	adds	r1, r7, r2
30009a8c:	460b      	mov	r3, r1
30009a8e:	4294      	cmp	r4, r2
30009a90:	d0b8      	beq.n	30009a04 <NAND_RxData+0x8c>
30009a92:	42b2      	cmp	r2, r6
30009a94:	460b      	mov	r3, r1
30009a96:	d3f1      	bcc.n	30009a7c <NAND_RxData+0x104>
30009a98:	4614      	mov	r4, r2
30009a9a:	e7ce      	b.n	30009a3a <NAND_RxData+0xc2>
30009a9c:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009aa8 <NAND_RxData+0x130>
30009aa0:	f899 5008 	ldrb.w	r5, [r9, #8]
30009aa4:	e773      	b.n	3000998e <NAND_RxData+0x16>
30009aa6:	bf00      	nop
30009aa8:	2001c01c 	.word	0x2001c01c
30009aac:	0000b9c1 	.word	0x0000b9c1

30009ab0 <NAND_Page_Read>:
30009ab0:	b530      	push	{r4, r5, lr}
30009ab2:	b085      	sub	sp, #20
30009ab4:	4c07      	ldr	r4, [pc, #28]	; (30009ad4 <NAND_Page_Read+0x24>)
30009ab6:	4d08      	ldr	r5, [pc, #32]	; (30009ad8 <NAND_Page_Read+0x28>)
30009ab8:	9301      	str	r3, [sp, #4]
30009aba:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009abe:	47a0      	blx	r4
30009ac0:	4604      	mov	r4, r0
30009ac2:	9903      	ldr	r1, [sp, #12]
30009ac4:	7a28      	ldrb	r0, [r5, #8]
30009ac6:	4d05      	ldr	r5, [pc, #20]	; (30009adc <NAND_Page_Read+0x2c>)
30009ac8:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009acc:	47a8      	blx	r5
30009ace:	4620      	mov	r0, r4
30009ad0:	b005      	add	sp, #20
30009ad2:	bd30      	pop	{r4, r5, pc}
30009ad4:	0000bd85 	.word	0x0000bd85
30009ad8:	2001c01c 	.word	0x2001c01c
30009adc:	30009979 	.word	0x30009979

30009ae0 <irq_register>:
30009ae0:	b530      	push	{r4, r5, lr}
30009ae2:	461c      	mov	r4, r3
30009ae4:	460d      	mov	r5, r1
30009ae6:	f040 0001 	orr.w	r0, r0, #1
30009aea:	4916      	ldr	r1, [pc, #88]	; (30009b44 <irq_register+0x64>)
30009aec:	4b16      	ldr	r3, [pc, #88]	; (30009b48 <irq_register+0x68>)
30009aee:	2c07      	cmp	r4, #7
30009af0:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
30009af4:	b085      	sub	sp, #20
30009af6:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
30009afa:	d813      	bhi.n	30009b24 <irq_register+0x44>
30009afc:	2c07      	cmp	r4, #7
30009afe:	bf28      	it	cs
30009b00:	2407      	movcs	r4, #7
30009b02:	2d00      	cmp	r5, #0
30009b04:	ea4f 1444 	mov.w	r4, r4, lsl #5
30009b08:	b2e4      	uxtb	r4, r4
30009b0a:	db04      	blt.n	30009b16 <irq_register+0x36>
30009b0c:	4b0f      	ldr	r3, [pc, #60]	; (30009b4c <irq_register+0x6c>)
30009b0e:	2001      	movs	r0, #1
30009b10:	555c      	strb	r4, [r3, r5]
30009b12:	b005      	add	sp, #20
30009b14:	bd30      	pop	{r4, r5, pc}
30009b16:	f005 050f 	and.w	r5, r5, #15
30009b1a:	4b0d      	ldr	r3, [pc, #52]	; (30009b50 <irq_register+0x70>)
30009b1c:	2001      	movs	r0, #1
30009b1e:	555c      	strb	r4, [r3, r5]
30009b20:	b005      	add	sp, #20
30009b22:	bd30      	pop	{r4, r5, pc}
30009b24:	4b0b      	ldr	r3, [pc, #44]	; (30009b54 <irq_register+0x74>)
30009b26:	2107      	movs	r1, #7
30009b28:	4a0b      	ldr	r2, [pc, #44]	; (30009b58 <irq_register+0x78>)
30009b2a:	2003      	movs	r0, #3
30009b2c:	9500      	str	r5, [sp, #0]
30009b2e:	e9cd 4101 	strd	r4, r1, [sp, #4]
30009b32:	4619      	mov	r1, r3
30009b34:	f002 f982 	bl	3000be3c <rtk_log_write>
30009b38:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30009b3c:	4807      	ldr	r0, [pc, #28]	; (30009b5c <irq_register+0x7c>)
30009b3e:	f002 fc63 	bl	3000c408 <__io_assert_failed_veneer>
30009b42:	e7db      	b.n	30009afc <irq_register+0x1c>
30009b44:	30000184 	.word	0x30000184
30009b48:	300002c4 	.word	0x300002c4
30009b4c:	e000e400 	.word	0xe000e400
30009b50:	e000ed14 	.word	0xe000ed14
30009b54:	3000d6f4 	.word	0x3000d6f4
30009b58:	3000d728 	.word	0x3000d728
30009b5c:	3000d750 	.word	0x3000d750

30009b60 <SYSCFG_BootFromNor>:
30009b60:	4b0e      	ldr	r3, [pc, #56]	; (30009b9c <SYSCFG_BootFromNor+0x3c>)
30009b62:	b510      	push	{r4, lr}
30009b64:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009b68:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009b6c:	0392      	lsls	r2, r2, #14
30009b6e:	d50a      	bpl.n	30009b86 <SYSCFG_BootFromNor+0x26>
30009b70:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009b74:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009b78:	d105      	bne.n	30009b86 <SYSCFG_BootFromNor+0x26>
30009b7a:	4b09      	ldr	r3, [pc, #36]	; (30009ba0 <SYSCFG_BootFromNor+0x40>)
30009b7c:	4798      	blx	r3
30009b7e:	3800      	subs	r0, #0
30009b80:	bf18      	it	ne
30009b82:	2001      	movne	r0, #1
30009b84:	bd10      	pop	{r4, pc}
30009b86:	4c07      	ldr	r4, [pc, #28]	; (30009ba4 <SYSCFG_BootFromNor+0x44>)
30009b88:	47a0      	blx	r4
30009b8a:	2802      	cmp	r0, #2
30009b8c:	d004      	beq.n	30009b98 <SYSCFG_BootFromNor+0x38>
30009b8e:	47a0      	blx	r4
30009b90:	2801      	cmp	r0, #1
30009b92:	d1f2      	bne.n	30009b7a <SYSCFG_BootFromNor+0x1a>
30009b94:	2000      	movs	r0, #0
30009b96:	bd10      	pop	{r4, pc}
30009b98:	2001      	movs	r0, #1
30009b9a:	bd10      	pop	{r4, pc}
30009b9c:	42008000 	.word	0x42008000
30009ba0:	0000c0bd 	.word	0x0000c0bd
30009ba4:	0000c1a9 	.word	0x0000c1a9

30009ba8 <SYSCFG_RLVersion>:
30009ba8:	4b07      	ldr	r3, [pc, #28]	; (30009bc8 <SYSCFG_RLVersion+0x20>)
30009baa:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009bae:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009bb2:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009bb6:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009bba:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009bbe:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009bc2:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009bc6:	4770      	bx	lr
30009bc8:	42008000 	.word	0x42008000

30009bcc <SBOOT_GetMdType>:
30009bcc:	b470      	push	{r4, r5, r6}
30009bce:	4c0c      	ldr	r4, [pc, #48]	; (30009c00 <SBOOT_GetMdType+0x34>)
30009bd0:	4e0c      	ldr	r6, [pc, #48]	; (30009c04 <SBOOT_GetMdType+0x38>)
30009bd2:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009bd6:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009bda:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009bde:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009be2:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009be6:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009bea:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009bee:	6033      	str	r3, [r6, #0]
30009bf0:	b913      	cbnz	r3, 30009bf8 <SBOOT_GetMdType+0x2c>
30009bf2:	bc70      	pop	{r4, r5, r6}
30009bf4:	f002 bc38 	b.w	3000c468 <__SBOOT_GetMdType_A_veneer>
30009bf8:	bc70      	pop	{r4, r5, r6}
30009bfa:	f002 bc25 	b.w	3000c448 <__SBOOT_GetMdType_B_veneer>
30009bfe:	bf00      	nop
30009c00:	42008000 	.word	0x42008000
30009c04:	3000fbd4 	.word	0x3000fbd4

30009c08 <CRYPTO_OTPKey_SHA_Init>:
30009c08:	4a0d      	ldr	r2, [pc, #52]	; (30009c40 <CRYPTO_OTPKey_SHA_Init+0x38>)
30009c0a:	b410      	push	{r4}
30009c0c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009c10:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009c14:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009c18:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009c1c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009c20:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009c24:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009c28:	4a06      	ldr	r2, [pc, #24]	; (30009c44 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009c2a:	6013      	str	r3, [r2, #0]
30009c2c:	b91b      	cbnz	r3, 30009c36 <CRYPTO_OTPKey_SHA_Init+0x2e>
30009c2e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c32:	f002 bb61 	b.w	3000c2f8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009c36:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c3a:	f002 bb55 	b.w	3000c2e8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30009c3e:	bf00      	nop
30009c40:	42008000 	.word	0x42008000
30009c44:	3000fbd4 	.word	0x3000fbd4

30009c48 <rtl_crypto_hmac_sha2_update>:
30009c48:	b470      	push	{r4, r5, r6}
30009c4a:	4c0c      	ldr	r4, [pc, #48]	; (30009c7c <rtl_crypto_hmac_sha2_update+0x34>)
30009c4c:	4e0c      	ldr	r6, [pc, #48]	; (30009c80 <rtl_crypto_hmac_sha2_update+0x38>)
30009c4e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009c52:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009c56:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009c5a:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009c5e:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009c62:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009c66:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009c6a:	6033      	str	r3, [r6, #0]
30009c6c:	b913      	cbnz	r3, 30009c74 <rtl_crypto_hmac_sha2_update+0x2c>
30009c6e:	bc70      	pop	{r4, r5, r6}
30009c70:	f002 bc02 	b.w	3000c478 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009c74:	bc70      	pop	{r4, r5, r6}
30009c76:	f002 bb1f 	b.w	3000c2b8 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009c7a:	bf00      	nop
30009c7c:	42008000 	.word	0x42008000
30009c80:	3000fbd4 	.word	0x3000fbd4

30009c84 <rtl_crypto_hmac_sha2_final>:
30009c84:	4a0d      	ldr	r2, [pc, #52]	; (30009cbc <rtl_crypto_hmac_sha2_final+0x38>)
30009c86:	b410      	push	{r4}
30009c88:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009c8c:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009c90:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009c94:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009c98:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009c9c:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009ca0:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009ca4:	4a06      	ldr	r2, [pc, #24]	; (30009cc0 <rtl_crypto_hmac_sha2_final+0x3c>)
30009ca6:	6013      	str	r3, [r2, #0]
30009ca8:	b91b      	cbnz	r3, 30009cb2 <rtl_crypto_hmac_sha2_final+0x2e>
30009caa:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cae:	f002 bb33 	b.w	3000c318 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009cb2:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cb6:	f002 bbb7 	b.w	3000c428 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009cba:	bf00      	nop
30009cbc:	42008000 	.word	0x42008000
30009cc0:	3000fbd4 	.word	0x3000fbd4

30009cc4 <rtl_crypto_sha2_init>:
30009cc4:	4a0d      	ldr	r2, [pc, #52]	; (30009cfc <rtl_crypto_sha2_init+0x38>)
30009cc6:	b410      	push	{r4}
30009cc8:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009ccc:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009cd0:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009cd4:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009cd8:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009cdc:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009ce0:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009ce4:	4a06      	ldr	r2, [pc, #24]	; (30009d00 <rtl_crypto_sha2_init+0x3c>)
30009ce6:	6013      	str	r3, [r2, #0]
30009ce8:	b91b      	cbnz	r3, 30009cf2 <rtl_crypto_sha2_init+0x2e>
30009cea:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cee:	f002 bb5b 	b.w	3000c3a8 <__rtl_crypto_sha2_init_A_veneer>
30009cf2:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cf6:	f002 bb4f 	b.w	3000c398 <__rtl_crypto_sha2_init_B_veneer>
30009cfa:	bf00      	nop
30009cfc:	42008000 	.word	0x42008000
30009d00:	3000fbd4 	.word	0x3000fbd4

30009d04 <rtl_crypto_sha2_update>:
30009d04:	b470      	push	{r4, r5, r6}
30009d06:	4c0c      	ldr	r4, [pc, #48]	; (30009d38 <rtl_crypto_sha2_update+0x34>)
30009d08:	4e0c      	ldr	r6, [pc, #48]	; (30009d3c <rtl_crypto_sha2_update+0x38>)
30009d0a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009d0e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009d12:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009d16:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009d1a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009d1e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009d22:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d26:	6033      	str	r3, [r6, #0]
30009d28:	b913      	cbnz	r3, 30009d30 <rtl_crypto_sha2_update+0x2c>
30009d2a:	bc70      	pop	{r4, r5, r6}
30009d2c:	f002 bb94 	b.w	3000c458 <__rtl_crypto_sha2_update_A_veneer>
30009d30:	bc70      	pop	{r4, r5, r6}
30009d32:	f002 bb19 	b.w	3000c368 <__rtl_crypto_sha2_update_B_veneer>
30009d36:	bf00      	nop
30009d38:	42008000 	.word	0x42008000
30009d3c:	3000fbd4 	.word	0x3000fbd4

30009d40 <rtl_crypto_sha2_final>:
30009d40:	4a0d      	ldr	r2, [pc, #52]	; (30009d78 <rtl_crypto_sha2_final+0x38>)
30009d42:	b410      	push	{r4}
30009d44:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009d48:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009d4c:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009d50:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009d54:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009d58:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009d5c:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d60:	4a06      	ldr	r2, [pc, #24]	; (30009d7c <rtl_crypto_sha2_final+0x3c>)
30009d62:	6013      	str	r3, [r2, #0]
30009d64:	b91b      	cbnz	r3, 30009d6e <rtl_crypto_sha2_final+0x2e>
30009d66:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d6a:	f002 bb05 	b.w	3000c378 <__rtl_crypto_sha2_final_A_veneer>
30009d6e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d72:	f002 bb51 	b.w	3000c418 <__rtl_crypto_sha2_final_B_veneer>
30009d76:	bf00      	nop
30009d78:	42008000 	.word	0x42008000
30009d7c:	3000fbd4 	.word	0x3000fbd4

30009d80 <SBOOT_Validate_PubKey>:
30009d80:	b470      	push	{r4, r5, r6}
30009d82:	4c0c      	ldr	r4, [pc, #48]	; (30009db4 <SBOOT_Validate_PubKey+0x34>)
30009d84:	4e0c      	ldr	r6, [pc, #48]	; (30009db8 <SBOOT_Validate_PubKey+0x38>)
30009d86:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009d8a:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009d8e:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009d92:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009d96:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009d9a:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009d9e:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009da2:	6033      	str	r3, [r6, #0]
30009da4:	b913      	cbnz	r3, 30009dac <SBOOT_Validate_PubKey+0x2c>
30009da6:	bc70      	pop	{r4, r5, r6}
30009da8:	f002 bb1e 	b.w	3000c3e8 <__SBOOT_Validate_PubKey_A_veneer>
30009dac:	bc70      	pop	{r4, r5, r6}
30009dae:	f002 ba93 	b.w	3000c2d8 <__SBOOT_Validate_PubKey_B_veneer>
30009db2:	bf00      	nop
30009db4:	42008000 	.word	0x42008000
30009db8:	3000fbd4 	.word	0x3000fbd4

30009dbc <SBOOT_Validate_Signature>:
30009dbc:	b5f0      	push	{r4, r5, r6, r7, lr}
30009dbe:	4d10      	ldr	r5, [pc, #64]	; (30009e00 <SBOOT_Validate_Signature+0x44>)
30009dc0:	9f05      	ldr	r7, [sp, #20]
30009dc2:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009dc6:	f8dd c018 	ldr.w	ip, [sp, #24]
30009dca:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009dce:	f8df e034 	ldr.w	lr, [pc, #52]	; 30009e04 <SBOOT_Validate_Signature+0x48>
30009dd2:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009dd6:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009dda:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009dde:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009de2:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009de6:	e9cd 7c05 	strd	r7, ip, [sp, #20]
30009dea:	f8ce 4000 	str.w	r4, [lr]
30009dee:	b91c      	cbnz	r4, 30009df8 <SBOOT_Validate_Signature+0x3c>
30009df0:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009df4:	f002 bae0 	b.w	3000c3b8 <__SBOOT_Validate_Signature_A_veneer>
30009df8:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009dfc:	f002 bb54 	b.w	3000c4a8 <__SBOOT_Validate_Signature_B_veneer>
30009e00:	42008000 	.word	0x42008000
30009e04:	3000fbd4 	.word	0x3000fbd4

30009e08 <SBOOT_Validate_ImgHash>:
30009e08:	b4f0      	push	{r4, r5, r6, r7}
30009e0a:	4d0c      	ldr	r5, [pc, #48]	; (30009e3c <SBOOT_Validate_ImgHash+0x34>)
30009e0c:	4f0c      	ldr	r7, [pc, #48]	; (30009e40 <SBOOT_Validate_ImgHash+0x38>)
30009e0e:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009e12:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009e16:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009e1a:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009e1e:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009e22:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009e26:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009e2a:	603c      	str	r4, [r7, #0]
30009e2c:	b914      	cbnz	r4, 30009e34 <SBOOT_Validate_ImgHash+0x2c>
30009e2e:	bcf0      	pop	{r4, r5, r6, r7}
30009e30:	f002 bb02 	b.w	3000c438 <__SBOOT_Validate_ImgHash_A_veneer>
30009e34:	bcf0      	pop	{r4, r5, r6, r7}
30009e36:	f002 bac7 	b.w	3000c3c8 <__SBOOT_Validate_ImgHash_B_veneer>
30009e3a:	bf00      	nop
30009e3c:	42008000 	.word	0x42008000
30009e40:	3000fbd4 	.word	0x3000fbd4

30009e44 <CRYPTO_SHA_Init>:
30009e44:	4b0b      	ldr	r3, [pc, #44]	; (30009e74 <CRYPTO_SHA_Init+0x30>)
30009e46:	490c      	ldr	r1, [pc, #48]	; (30009e78 <CRYPTO_SHA_Init+0x34>)
30009e48:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009e4c:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009e50:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
30009e54:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
30009e58:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009e5c:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009e60:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009e64:	6008      	str	r0, [r1, #0]
30009e66:	b908      	cbnz	r0, 30009e6c <CRYPTO_SHA_Init+0x28>
30009e68:	f002 bb16 	b.w	3000c498 <__CRYPTO_SHA_Init_A_veneer>
30009e6c:	2000      	movs	r0, #0
30009e6e:	f002 ba6b 	b.w	3000c348 <__CRYPTO_SHA_Init_B_veneer>
30009e72:	bf00      	nop
30009e74:	42008000 	.word	0x42008000
30009e78:	3000fbd4 	.word	0x3000fbd4

30009e7c <SBOOT_Validate_Algorithm>:
30009e7c:	b4f0      	push	{r4, r5, r6, r7}
30009e7e:	4d0c      	ldr	r5, [pc, #48]	; (30009eb0 <SBOOT_Validate_Algorithm+0x34>)
30009e80:	4f0c      	ldr	r7, [pc, #48]	; (30009eb4 <SBOOT_Validate_Algorithm+0x38>)
30009e82:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009e86:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009e8a:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009e8e:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009e92:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009e96:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009e9a:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009e9e:	603c      	str	r4, [r7, #0]
30009ea0:	b914      	cbnz	r4, 30009ea8 <SBOOT_Validate_Algorithm+0x2c>
30009ea2:	bcf0      	pop	{r4, r5, r6, r7}
30009ea4:	f002 baa8 	b.w	3000c3f8 <__SBOOT_Validate_Algorithm_A_veneer>
30009ea8:	bcf0      	pop	{r4, r5, r6, r7}
30009eaa:	f002 baed 	b.w	3000c488 <__SBOOT_Validate_Algorithm_B_veneer>
30009eae:	bf00      	nop
30009eb0:	42008000 	.word	0x42008000
30009eb4:	3000fbd4 	.word	0x3000fbd4

30009eb8 <ddr_init_index>:
30009eb8:	b570      	push	{r4, r5, r6, lr}
30009eba:	4b0f      	ldr	r3, [pc, #60]	; (30009ef8 <ddr_init_index+0x40>)
30009ebc:	4798      	blx	r3
30009ebe:	4604      	mov	r4, r0
30009ec0:	4b0e      	ldr	r3, [pc, #56]	; (30009efc <ddr_init_index+0x44>)
30009ec2:	4798      	blx	r3
30009ec4:	2c03      	cmp	r4, #3
30009ec6:	d014      	beq.n	30009ef2 <ddr_init_index+0x3a>
30009ec8:	2c01      	cmp	r4, #1
30009eca:	d00c      	beq.n	30009ee6 <ddr_init_index+0x2e>
30009ecc:	4e0c      	ldr	r6, [pc, #48]	; (30009f00 <ddr_init_index+0x48>)
30009ece:	4d0d      	ldr	r5, [pc, #52]	; (30009f04 <ddr_init_index+0x4c>)
30009ed0:	4c0d      	ldr	r4, [pc, #52]	; (30009f08 <ddr_init_index+0x50>)
30009ed2:	462b      	mov	r3, r5
30009ed4:	4622      	mov	r2, r4
30009ed6:	490b      	ldr	r1, [pc, #44]	; (30009f04 <ddr_init_index+0x4c>)
30009ed8:	2002      	movs	r0, #2
30009eda:	f001 ffaf 	bl	3000be3c <rtk_log_write>
30009ede:	f242 7010 	movw	r0, #10000	; 0x2710
30009ee2:	47b0      	blx	r6
30009ee4:	e7f5      	b.n	30009ed2 <ddr_init_index+0x1a>
30009ee6:	2802      	cmp	r0, #2
30009ee8:	d003      	beq.n	30009ef2 <ddr_init_index+0x3a>
30009eea:	2803      	cmp	r0, #3
30009eec:	d1ee      	bne.n	30009ecc <ddr_init_index+0x14>
30009eee:	2000      	movs	r0, #0
30009ef0:	bd70      	pop	{r4, r5, r6, pc}
30009ef2:	4620      	mov	r0, r4
30009ef4:	bd70      	pop	{r4, r5, r6, pc}
30009ef6:	bf00      	nop
30009ef8:	30007f75 	.word	0x30007f75
30009efc:	30007df1 	.word	0x30007df1
30009f00:	00009be5 	.word	0x00009be5
30009f04:	3000d7ac 	.word	0x3000d7ac
30009f08:	3000d7b0 	.word	0x3000d7b0

30009f0c <rxi316_perf_tune>:
30009f0c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009f10:	4606      	mov	r6, r0
30009f12:	6800      	ldr	r0, [r0, #0]
30009f14:	b0c1      	sub	sp, #260	; 0x104
30009f16:	7985      	ldrb	r5, [r0, #6]
30009f18:	2d00      	cmp	r5, #0
30009f1a:	f000 832c 	beq.w	3000a576 <rxi316_perf_tune+0x66a>
30009f1e:	2d01      	cmp	r5, #1
30009f20:	d003      	beq.n	30009f2a <rxi316_perf_tune+0x1e>
30009f22:	2d03      	cmp	r5, #3
30009f24:	bf14      	ite	ne
30009f26:	2502      	movne	r5, #2
30009f28:	2500      	moveq	r5, #0
30009f2a:	68f3      	ldr	r3, [r6, #12]
30009f2c:	2420      	movs	r4, #32
30009f2e:	4684      	mov	ip, r0
30009f30:	6859      	ldr	r1, [r3, #4]
30009f32:	46a3      	mov	fp, r4
30009f34:	46a2      	mov	sl, r4
30009f36:	46a0      	mov	r8, r4
30009f38:	784b      	ldrb	r3, [r1, #1]
30009f3a:	46a1      	mov	r9, r4
30009f3c:	7bcf      	ldrb	r7, [r1, #15]
30009f3e:	9321      	str	r3, [sp, #132]	; 0x84
30009f40:	788b      	ldrb	r3, [r1, #2]
30009f42:	780a      	ldrb	r2, [r1, #0]
30009f44:	9322      	str	r3, [sp, #136]	; 0x88
30009f46:	78cb      	ldrb	r3, [r1, #3]
30009f48:	9323      	str	r3, [sp, #140]	; 0x8c
30009f4a:	790b      	ldrb	r3, [r1, #4]
30009f4c:	9324      	str	r3, [sp, #144]	; 0x90
30009f4e:	794b      	ldrb	r3, [r1, #5]
30009f50:	9325      	str	r3, [sp, #148]	; 0x94
30009f52:	798b      	ldrb	r3, [r1, #6]
30009f54:	9326      	str	r3, [sp, #152]	; 0x98
30009f56:	79cb      	ldrb	r3, [r1, #7]
30009f58:	9327      	str	r3, [sp, #156]	; 0x9c
30009f5a:	7a0b      	ldrb	r3, [r1, #8]
30009f5c:	9328      	str	r3, [sp, #160]	; 0xa0
30009f5e:	7a4b      	ldrb	r3, [r1, #9]
30009f60:	9329      	str	r3, [sp, #164]	; 0xa4
30009f62:	7a8b      	ldrb	r3, [r1, #10]
30009f64:	932a      	str	r3, [sp, #168]	; 0xa8
30009f66:	7acb      	ldrb	r3, [r1, #11]
30009f68:	932b      	str	r3, [sp, #172]	; 0xac
30009f6a:	7b0b      	ldrb	r3, [r1, #12]
30009f6c:	932c      	str	r3, [sp, #176]	; 0xb0
30009f6e:	7b4b      	ldrb	r3, [r1, #13]
30009f70:	932d      	str	r3, [sp, #180]	; 0xb4
30009f72:	7b8b      	ldrb	r3, [r1, #14]
30009f74:	932e      	str	r3, [sp, #184]	; 0xb8
30009f76:	2300      	movs	r3, #0
30009f78:	972f      	str	r7, [sp, #188]	; 0xbc
30009f7a:	7c0f      	ldrb	r7, [r1, #16]
30009f7c:	941c      	str	r4, [sp, #112]	; 0x70
30009f7e:	9730      	str	r7, [sp, #192]	; 0xc0
30009f80:	7c4f      	ldrb	r7, [r1, #17]
30009f82:	9731      	str	r7, [sp, #196]	; 0xc4
30009f84:	7c8f      	ldrb	r7, [r1, #18]
30009f86:	9732      	str	r7, [sp, #200]	; 0xc8
30009f88:	7ccf      	ldrb	r7, [r1, #19]
30009f8a:	9733      	str	r7, [sp, #204]	; 0xcc
30009f8c:	7d0f      	ldrb	r7, [r1, #20]
30009f8e:	9734      	str	r7, [sp, #208]	; 0xd0
30009f90:	7d4f      	ldrb	r7, [r1, #21]
30009f92:	9735      	str	r7, [sp, #212]	; 0xd4
30009f94:	7d8f      	ldrb	r7, [r1, #22]
30009f96:	9736      	str	r7, [sp, #216]	; 0xd8
30009f98:	7dcf      	ldrb	r7, [r1, #23]
30009f9a:	9737      	str	r7, [sp, #220]	; 0xdc
30009f9c:	7e0f      	ldrb	r7, [r1, #24]
30009f9e:	9738      	str	r7, [sp, #224]	; 0xe0
30009fa0:	7e4f      	ldrb	r7, [r1, #25]
30009fa2:	9739      	str	r7, [sp, #228]	; 0xe4
30009fa4:	7e8f      	ldrb	r7, [r1, #26]
30009fa6:	973a      	str	r7, [sp, #232]	; 0xe8
30009fa8:	7ecf      	ldrb	r7, [r1, #27]
30009faa:	973b      	str	r7, [sp, #236]	; 0xec
30009fac:	7f0f      	ldrb	r7, [r1, #28]
30009fae:	973c      	str	r7, [sp, #240]	; 0xf0
30009fb0:	7f4f      	ldrb	r7, [r1, #29]
30009fb2:	973d      	str	r7, [sp, #244]	; 0xf4
30009fb4:	7f8f      	ldrb	r7, [r1, #30]
30009fb6:	7fc9      	ldrb	r1, [r1, #31]
30009fb8:	973e      	str	r7, [sp, #248]	; 0xf8
30009fba:	4627      	mov	r7, r4
30009fbc:	913f      	str	r1, [sp, #252]	; 0xfc
30009fbe:	4619      	mov	r1, r3
30009fc0:	9408      	str	r4, [sp, #32]
30009fc2:	941b      	str	r4, [sp, #108]	; 0x6c
30009fc4:	9403      	str	r4, [sp, #12]
30009fc6:	941a      	str	r4, [sp, #104]	; 0x68
30009fc8:	9409      	str	r4, [sp, #36]	; 0x24
30009fca:	9302      	str	r3, [sp, #8]
30009fcc:	e9cd 4404 	strd	r4, r4, [sp, #16]
30009fd0:	e9cd 4400 	strd	r4, r4, [sp]
30009fd4:	e9cd 4406 	strd	r4, r4, [sp, #24]
30009fd8:	e9cd 4418 	strd	r4, r4, [sp, #96]	; 0x60
30009fdc:	e9cd 4416 	strd	r4, r4, [sp, #88]	; 0x58
30009fe0:	e9cd 4414 	strd	r4, r4, [sp, #80]	; 0x50
30009fe4:	e9cd 4412 	strd	r4, r4, [sp, #72]	; 0x48
30009fe8:	e9cd 4410 	strd	r4, r4, [sp, #64]	; 0x40
30009fec:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30009ff0:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
30009ff4:	e9cd 440a 	strd	r4, r4, [sp, #40]	; 0x28
30009ff8:	b94a      	cbnz	r2, 3000a00e <rxi316_perf_tune+0x102>
30009ffa:	3101      	adds	r1, #1
30009ffc:	4699      	mov	r9, r3
30009ffe:	3301      	adds	r3, #1
3000a000:	2b20      	cmp	r3, #32
3000a002:	d069      	beq.n	3000a0d8 <rxi316_perf_tune+0x1cc>
3000a004:	aa20      	add	r2, sp, #128	; 0x80
3000a006:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a00a:	2a00      	cmp	r2, #0
3000a00c:	d0f5      	beq.n	30009ffa <rxi316_perf_tune+0xee>
3000a00e:	2a01      	cmp	r2, #1
3000a010:	d05d      	beq.n	3000a0ce <rxi316_perf_tune+0x1c2>
3000a012:	2a02      	cmp	r2, #2
3000a014:	d06e      	beq.n	3000a0f4 <rxi316_perf_tune+0x1e8>
3000a016:	2a03      	cmp	r2, #3
3000a018:	d06f      	beq.n	3000a0fa <rxi316_perf_tune+0x1ee>
3000a01a:	2a04      	cmp	r2, #4
3000a01c:	d070      	beq.n	3000a100 <rxi316_perf_tune+0x1f4>
3000a01e:	2a05      	cmp	r2, #5
3000a020:	d071      	beq.n	3000a106 <rxi316_perf_tune+0x1fa>
3000a022:	2a06      	cmp	r2, #6
3000a024:	d072      	beq.n	3000a10c <rxi316_perf_tune+0x200>
3000a026:	2a07      	cmp	r2, #7
3000a028:	f000 82a7 	beq.w	3000a57a <rxi316_perf_tune+0x66e>
3000a02c:	2a08      	cmp	r2, #8
3000a02e:	f000 8364 	beq.w	3000a6fa <rxi316_perf_tune+0x7ee>
3000a032:	2a09      	cmp	r2, #9
3000a034:	f000 8364 	beq.w	3000a700 <rxi316_perf_tune+0x7f4>
3000a038:	2a0a      	cmp	r2, #10
3000a03a:	f000 836c 	beq.w	3000a716 <rxi316_perf_tune+0x80a>
3000a03e:	2a0b      	cmp	r2, #11
3000a040:	f000 8364 	beq.w	3000a70c <rxi316_perf_tune+0x800>
3000a044:	2a0c      	cmp	r2, #12
3000a046:	f000 836b 	beq.w	3000a720 <rxi316_perf_tune+0x814>
3000a04a:	2a0d      	cmp	r2, #13
3000a04c:	f000 8377 	beq.w	3000a73e <rxi316_perf_tune+0x832>
3000a050:	2a0e      	cmp	r2, #14
3000a052:	f000 836f 	beq.w	3000a734 <rxi316_perf_tune+0x828>
3000a056:	2a0f      	cmp	r2, #15
3000a058:	f000 8376 	beq.w	3000a748 <rxi316_perf_tune+0x83c>
3000a05c:	2a10      	cmp	r2, #16
3000a05e:	f000 8378 	beq.w	3000a752 <rxi316_perf_tune+0x846>
3000a062:	2a11      	cmp	r2, #17
3000a064:	f000 8377 	beq.w	3000a756 <rxi316_perf_tune+0x84a>
3000a068:	2a12      	cmp	r2, #18
3000a06a:	f000 8376 	beq.w	3000a75a <rxi316_perf_tune+0x84e>
3000a06e:	2a13      	cmp	r2, #19
3000a070:	f000 8377 	beq.w	3000a762 <rxi316_perf_tune+0x856>
3000a074:	2a14      	cmp	r2, #20
3000a076:	f000 8372 	beq.w	3000a75e <rxi316_perf_tune+0x852>
3000a07a:	2a15      	cmp	r2, #21
3000a07c:	f000 8379 	beq.w	3000a772 <rxi316_perf_tune+0x866>
3000a080:	2a16      	cmp	r2, #22
3000a082:	f000 8374 	beq.w	3000a76e <rxi316_perf_tune+0x862>
3000a086:	2a17      	cmp	r2, #23
3000a088:	f000 8383 	beq.w	3000a792 <rxi316_perf_tune+0x886>
3000a08c:	2a18      	cmp	r2, #24
3000a08e:	f000 837e 	beq.w	3000a78e <rxi316_perf_tune+0x882>
3000a092:	2a19      	cmp	r2, #25
3000a094:	f000 8379 	beq.w	3000a78a <rxi316_perf_tune+0x87e>
3000a098:	2a1a      	cmp	r2, #26
3000a09a:	f000 8374 	beq.w	3000a786 <rxi316_perf_tune+0x87a>
3000a09e:	2a1b      	cmp	r2, #27
3000a0a0:	f000 836f 	beq.w	3000a782 <rxi316_perf_tune+0x876>
3000a0a4:	2a1c      	cmp	r2, #28
3000a0a6:	f000 836a 	beq.w	3000a77e <rxi316_perf_tune+0x872>
3000a0aa:	2a1d      	cmp	r2, #29
3000a0ac:	f000 8365 	beq.w	3000a77a <rxi316_perf_tune+0x86e>
3000a0b0:	2a1e      	cmp	r2, #30
3000a0b2:	f000 8360 	beq.w	3000a776 <rxi316_perf_tune+0x86a>
3000a0b6:	2a1f      	cmp	r2, #31
3000a0b8:	f000 8357 	beq.w	3000a76a <rxi316_perf_tune+0x85e>
3000a0bc:	2a20      	cmp	r2, #32
3000a0be:	f000 8352 	beq.w	3000a766 <rxi316_perf_tune+0x85a>
3000a0c2:	9808      	ldr	r0, [sp, #32]
3000a0c4:	2a21      	cmp	r2, #33	; 0x21
3000a0c6:	bf08      	it	eq
3000a0c8:	4618      	moveq	r0, r3
3000a0ca:	9008      	str	r0, [sp, #32]
3000a0cc:	e797      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a0ce:	4698      	mov	r8, r3
3000a0d0:	3301      	adds	r3, #1
3000a0d2:	3101      	adds	r1, #1
3000a0d4:	2b20      	cmp	r3, #32
3000a0d6:	d195      	bne.n	3000a004 <rxi316_perf_tune+0xf8>
3000a0d8:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a0dc:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a0e0:	b1ba      	cbz	r2, 3000a112 <rxi316_perf_tune+0x206>
3000a0e2:	2a01      	cmp	r2, #1
3000a0e4:	f000 830f 	beq.w	3000a706 <rxi316_perf_tune+0x7fa>
3000a0e8:	2a03      	cmp	r2, #3
3000a0ea:	bf0c      	ite	eq
3000a0ec:	2204      	moveq	r2, #4
3000a0ee:	2203      	movne	r2, #3
3000a0f0:	921d      	str	r2, [sp, #116]	; 0x74
3000a0f2:	e010      	b.n	3000a116 <rxi316_perf_tune+0x20a>
3000a0f4:	3101      	adds	r1, #1
3000a0f6:	461f      	mov	r7, r3
3000a0f8:	e781      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a0fa:	3101      	adds	r1, #1
3000a0fc:	469a      	mov	sl, r3
3000a0fe:	e77e      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a100:	3101      	adds	r1, #1
3000a102:	469b      	mov	fp, r3
3000a104:	e77b      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a106:	3101      	adds	r1, #1
3000a108:	9300      	str	r3, [sp, #0]
3000a10a:	e778      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a10c:	3101      	adds	r1, #1
3000a10e:	9301      	str	r3, [sp, #4]
3000a110:	e775      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a112:	2201      	movs	r2, #1
3000a114:	921d      	str	r2, [sp, #116]	; 0x74
3000a116:	3b02      	subs	r3, #2
3000a118:	428b      	cmp	r3, r1
3000a11a:	d005      	beq.n	3000a128 <rxi316_perf_tune+0x21c>
3000a11c:	4b90      	ldr	r3, [pc, #576]	; (3000a360 <rxi316_perf_tune+0x454>)
3000a11e:	2002      	movs	r0, #2
3000a120:	4a90      	ldr	r2, [pc, #576]	; (3000a364 <rxi316_perf_tune+0x458>)
3000a122:	4619      	mov	r1, r3
3000a124:	f001 fe8a 	bl	3000be3c <rtk_log_write>
3000a128:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a12a:	9a02      	ldr	r2, [sp, #8]
3000a12c:	4293      	cmp	r3, r2
3000a12e:	d005      	beq.n	3000a13c <rxi316_perf_tune+0x230>
3000a130:	4b8b      	ldr	r3, [pc, #556]	; (3000a360 <rxi316_perf_tune+0x454>)
3000a132:	2002      	movs	r0, #2
3000a134:	4a8c      	ldr	r2, [pc, #560]	; (3000a368 <rxi316_perf_tune+0x45c>)
3000a136:	4619      	mov	r1, r3
3000a138:	f001 fe80 	bl	3000be3c <rtk_log_write>
3000a13c:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a13e:	2b20      	cmp	r3, #32
3000a140:	f000 82d8 	beq.w	3000a6f4 <rxi316_perf_tune+0x7e8>
3000a144:	1b5a      	subs	r2, r3, r5
3000a146:	3a05      	subs	r2, #5
3000a148:	f002 030f 	and.w	r3, r2, #15
3000a14c:	9302      	str	r3, [sp, #8]
3000a14e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a150:	2b20      	cmp	r3, #32
3000a152:	f000 82cd 	beq.w	3000a6f0 <rxi316_perf_tune+0x7e4>
3000a156:	1b5a      	subs	r2, r3, r5
3000a158:	3a06      	subs	r2, #6
3000a15a:	0112      	lsls	r2, r2, #4
3000a15c:	b2d2      	uxtb	r2, r2
3000a15e:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a160:	2b20      	cmp	r3, #32
3000a162:	f000 82c1 	beq.w	3000a6e8 <rxi316_perf_tune+0x7dc>
3000a166:	1b5b      	subs	r3, r3, r5
3000a168:	3b07      	subs	r3, #7
3000a16a:	021b      	lsls	r3, r3, #8
3000a16c:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a170:	930b      	str	r3, [sp, #44]	; 0x2c
3000a172:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a174:	2b20      	cmp	r3, #32
3000a176:	f000 8214 	beq.w	3000a5a2 <rxi316_perf_tune+0x696>
3000a17a:	1b5b      	subs	r3, r3, r5
3000a17c:	3b08      	subs	r3, #8
3000a17e:	031b      	lsls	r3, r3, #12
3000a180:	b29b      	uxth	r3, r3
3000a182:	930a      	str	r3, [sp, #40]	; 0x28
3000a184:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a186:	2b20      	cmp	r3, #32
3000a188:	f000 8212 	beq.w	3000a5b0 <rxi316_perf_tune+0x6a4>
3000a18c:	1b5b      	subs	r3, r3, r5
3000a18e:	3b09      	subs	r3, #9
3000a190:	041b      	lsls	r3, r3, #16
3000a192:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a196:	9309      	str	r3, [sp, #36]	; 0x24
3000a198:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a19a:	2b20      	cmp	r3, #32
3000a19c:	f000 820f 	beq.w	3000a5be <rxi316_perf_tune+0x6b2>
3000a1a0:	1b5b      	subs	r3, r3, r5
3000a1a2:	3b0a      	subs	r3, #10
3000a1a4:	051b      	lsls	r3, r3, #20
3000a1a6:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a1aa:	930c      	str	r3, [sp, #48]	; 0x30
3000a1ac:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a1ae:	2b20      	cmp	r3, #32
3000a1b0:	f000 820c 	beq.w	3000a5cc <rxi316_perf_tune+0x6c0>
3000a1b4:	1b5b      	subs	r3, r3, r5
3000a1b6:	3b0b      	subs	r3, #11
3000a1b8:	061b      	lsls	r3, r3, #24
3000a1ba:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a1be:	930d      	str	r3, [sp, #52]	; 0x34
3000a1c0:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a1c2:	2b20      	cmp	r3, #32
3000a1c4:	f000 8209 	beq.w	3000a5da <rxi316_perf_tune+0x6ce>
3000a1c8:	1b5b      	subs	r3, r3, r5
3000a1ca:	3b0c      	subs	r3, #12
3000a1cc:	071b      	lsls	r3, r3, #28
3000a1ce:	930e      	str	r3, [sp, #56]	; 0x38
3000a1d0:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a1d2:	2b20      	cmp	r3, #32
3000a1d4:	f000 8208 	beq.w	3000a5e8 <rxi316_perf_tune+0x6dc>
3000a1d8:	1b5b      	subs	r3, r3, r5
3000a1da:	3b0d      	subs	r3, #13
3000a1dc:	f003 030f 	and.w	r3, r3, #15
3000a1e0:	930f      	str	r3, [sp, #60]	; 0x3c
3000a1e2:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a1e4:	2b20      	cmp	r3, #32
3000a1e6:	f000 8205 	beq.w	3000a5f4 <rxi316_perf_tune+0x6e8>
3000a1ea:	1b5b      	subs	r3, r3, r5
3000a1ec:	3b0e      	subs	r3, #14
3000a1ee:	011b      	lsls	r3, r3, #4
3000a1f0:	b2db      	uxtb	r3, r3
3000a1f2:	9310      	str	r3, [sp, #64]	; 0x40
3000a1f4:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a1f6:	2b20      	cmp	r3, #32
3000a1f8:	f000 8202 	beq.w	3000a600 <rxi316_perf_tune+0x6f4>
3000a1fc:	1b5b      	subs	r3, r3, r5
3000a1fe:	3b0f      	subs	r3, #15
3000a200:	021b      	lsls	r3, r3, #8
3000a202:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a206:	9311      	str	r3, [sp, #68]	; 0x44
3000a208:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a20a:	2b20      	cmp	r3, #32
3000a20c:	f000 81ff 	beq.w	3000a60e <rxi316_perf_tune+0x702>
3000a210:	1b5b      	subs	r3, r3, r5
3000a212:	031b      	lsls	r3, r3, #12
3000a214:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a218:	9312      	str	r3, [sp, #72]	; 0x48
3000a21a:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a21c:	2b20      	cmp	r3, #32
3000a21e:	f000 81fd 	beq.w	3000a61c <rxi316_perf_tune+0x710>
3000a222:	1b5b      	subs	r3, r3, r5
3000a224:	3b11      	subs	r3, #17
3000a226:	041b      	lsls	r3, r3, #16
3000a228:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a22c:	9313      	str	r3, [sp, #76]	; 0x4c
3000a22e:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a230:	2b20      	cmp	r3, #32
3000a232:	f000 81fa 	beq.w	3000a62a <rxi316_perf_tune+0x71e>
3000a236:	1b5b      	subs	r3, r3, r5
3000a238:	3b12      	subs	r3, #18
3000a23a:	051b      	lsls	r3, r3, #20
3000a23c:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a240:	931f      	str	r3, [sp, #124]	; 0x7c
3000a242:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a244:	2b20      	cmp	r3, #32
3000a246:	f000 81f7 	beq.w	3000a638 <rxi316_perf_tune+0x72c>
3000a24a:	1b5b      	subs	r3, r3, r5
3000a24c:	3b13      	subs	r3, #19
3000a24e:	061b      	lsls	r3, r3, #24
3000a250:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a254:	931e      	str	r3, [sp, #120]	; 0x78
3000a256:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a258:	2b20      	cmp	r3, #32
3000a25a:	f000 81f4 	beq.w	3000a646 <rxi316_perf_tune+0x73a>
3000a25e:	1b5b      	subs	r3, r3, r5
3000a260:	3b14      	subs	r3, #20
3000a262:	071b      	lsls	r3, r3, #28
3000a264:	931d      	str	r3, [sp, #116]	; 0x74
3000a266:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a268:	2b20      	cmp	r3, #32
3000a26a:	f000 81f3 	beq.w	3000a654 <rxi316_perf_tune+0x748>
3000a26e:	1b5b      	subs	r3, r3, r5
3000a270:	3b15      	subs	r3, #21
3000a272:	f003 030f 	and.w	r3, r3, #15
3000a276:	9318      	str	r3, [sp, #96]	; 0x60
3000a278:	9b06      	ldr	r3, [sp, #24]
3000a27a:	2b20      	cmp	r3, #32
3000a27c:	f000 81f0 	beq.w	3000a660 <rxi316_perf_tune+0x754>
3000a280:	1b5b      	subs	r3, r3, r5
3000a282:	3b02      	subs	r3, #2
3000a284:	f003 031f 	and.w	r3, r3, #31
3000a288:	9317      	str	r3, [sp, #92]	; 0x5c
3000a28a:	9b07      	ldr	r3, [sp, #28]
3000a28c:	2b20      	cmp	r3, #32
3000a28e:	f000 81ed 	beq.w	3000a66c <rxi316_perf_tune+0x760>
3000a292:	1b5b      	subs	r3, r3, r5
3000a294:	3b02      	subs	r3, #2
3000a296:	015b      	lsls	r3, r3, #5
3000a298:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a29c:	9316      	str	r3, [sp, #88]	; 0x58
3000a29e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a2a0:	2b20      	cmp	r3, #32
3000a2a2:	f000 81ea 	beq.w	3000a67a <rxi316_perf_tune+0x76e>
3000a2a6:	1b5b      	subs	r3, r3, r5
3000a2a8:	f1b9 0f20 	cmp.w	r9, #32
3000a2ac:	f1a3 0302 	sub.w	r3, r3, #2
3000a2b0:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a2b4:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a2b8:	9315      	str	r3, [sp, #84]	; 0x54
3000a2ba:	f000 81e5 	beq.w	3000a688 <rxi316_perf_tune+0x77c>
3000a2be:	eba9 0105 	sub.w	r1, r9, r5
3000a2c2:	f1b8 0f20 	cmp.w	r8, #32
3000a2c6:	f1a1 0102 	sub.w	r1, r1, #2
3000a2ca:	f001 030f 	and.w	r3, r1, #15
3000a2ce:	9314      	str	r3, [sp, #80]	; 0x50
3000a2d0:	f000 81e0 	beq.w	3000a694 <rxi316_perf_tune+0x788>
3000a2d4:	eba8 0105 	sub.w	r1, r8, r5
3000a2d8:	2f20      	cmp	r7, #32
3000a2da:	f1a1 0103 	sub.w	r1, r1, #3
3000a2de:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a2e2:	b2c9      	uxtb	r1, r1
3000a2e4:	f000 81db 	beq.w	3000a69e <rxi316_perf_tune+0x792>
3000a2e8:	1b7b      	subs	r3, r7, r5
3000a2ea:	f1ba 0f20 	cmp.w	sl, #32
3000a2ee:	f1a3 0304 	sub.w	r3, r3, #4
3000a2f2:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a2f6:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a2fa:	9307      	str	r3, [sp, #28]
3000a2fc:	f000 81d6 	beq.w	3000a6ac <rxi316_perf_tune+0x7a0>
3000a300:	ebaa 0305 	sub.w	r3, sl, r5
3000a304:	f1bb 0f20 	cmp.w	fp, #32
3000a308:	f1a3 0305 	sub.w	r3, r3, #5
3000a30c:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a310:	b29b      	uxth	r3, r3
3000a312:	9306      	str	r3, [sp, #24]
3000a314:	f000 81d1 	beq.w	3000a6ba <rxi316_perf_tune+0x7ae>
3000a318:	ebab 0c05 	sub.w	ip, fp, r5
3000a31c:	9b00      	ldr	r3, [sp, #0]
3000a31e:	f1ac 0c06 	sub.w	ip, ip, #6
3000a322:	2b20      	cmp	r3, #32
3000a324:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a328:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a32c:	f000 81cb 	beq.w	3000a6c6 <rxi316_perf_tune+0x7ba>
3000a330:	eba3 0e05 	sub.w	lr, r3, r5
3000a334:	9b01      	ldr	r3, [sp, #4]
3000a336:	f1ae 0e07 	sub.w	lr, lr, #7
3000a33a:	2b20      	cmp	r3, #32
3000a33c:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a340:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a344:	f000 81c5 	beq.w	3000a6d2 <rxi316_perf_tune+0x7c6>
3000a348:	1b5f      	subs	r7, r3, r5
3000a34a:	3f08      	subs	r7, #8
3000a34c:	063f      	lsls	r7, r7, #24
3000a34e:	f007 6370 	and.w	r3, r7, #251658240	; 0xf000000
3000a352:	9301      	str	r3, [sp, #4]
3000a354:	9b03      	ldr	r3, [sp, #12]
3000a356:	2b20      	cmp	r3, #32
3000a358:	f000 81c2 	beq.w	3000a6e0 <rxi316_perf_tune+0x7d4>
3000a35c:	e006      	b.n	3000a36c <rxi316_perf_tune+0x460>
3000a35e:	bf00      	nop
3000a360:	3000d7ac 	.word	0x3000d7ac
3000a364:	3000d7d4 	.word	0x3000d7d4
3000a368:	3000d818 	.word	0x3000d818
3000a36c:	1b5b      	subs	r3, r3, r5
3000a36e:	3b09      	subs	r3, #9
3000a370:	071b      	lsls	r3, r3, #28
3000a372:	9300      	str	r3, [sp, #0]
3000a374:	9b04      	ldr	r3, [sp, #16]
3000a376:	2b20      	cmp	r3, #32
3000a378:	f000 8110 	beq.w	3000a59c <rxi316_perf_tune+0x690>
3000a37c:	eba3 0805 	sub.w	r8, r3, r5
3000a380:	f1a8 080a 	sub.w	r8, r8, #10
3000a384:	f008 080f 	and.w	r8, r8, #15
3000a388:	9b05      	ldr	r3, [sp, #20]
3000a38a:	2b20      	cmp	r3, #32
3000a38c:	f000 8104 	beq.w	3000a598 <rxi316_perf_tune+0x68c>
3000a390:	1b58      	subs	r0, r3, r5
3000a392:	380b      	subs	r0, #11
3000a394:	0100      	lsls	r0, r0, #4
3000a396:	b2c0      	uxtb	r0, r0
3000a398:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a39a:	2b20      	cmp	r3, #32
3000a39c:	f000 80f9 	beq.w	3000a592 <rxi316_perf_tune+0x686>
3000a3a0:	eba3 0a05 	sub.w	sl, r3, r5
3000a3a4:	f1aa 0a02 	sub.w	sl, sl, #2
3000a3a8:	f00a 0a1f 	and.w	sl, sl, #31
3000a3ac:	9b08      	ldr	r3, [sp, #32]
3000a3ae:	2b20      	cmp	r3, #32
3000a3b0:	f000 80ec 	beq.w	3000a58c <rxi316_perf_tune+0x680>
3000a3b4:	eba3 0905 	sub.w	r9, r3, r5
3000a3b8:	f1a9 0902 	sub.w	r9, r9, #2
3000a3bc:	f009 091f 	and.w	r9, r9, #31
3000a3c0:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a3c2:	2b20      	cmp	r3, #32
3000a3c4:	f000 80df 	beq.w	3000a586 <rxi316_perf_tune+0x67a>
3000a3c8:	eba3 0b05 	sub.w	fp, r3, r5
3000a3cc:	f1ab 0b02 	sub.w	fp, fp, #2
3000a3d0:	f00b 0b1f 	and.w	fp, fp, #31
3000a3d4:	2c20      	cmp	r4, #32
3000a3d6:	f000 80d3 	beq.w	3000a580 <rxi316_perf_tune+0x674>
3000a3da:	1b63      	subs	r3, r4, r5
3000a3dc:	3b02      	subs	r3, #2
3000a3de:	015b      	lsls	r3, r3, #5
3000a3e0:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a3e4:	9c02      	ldr	r4, [sp, #8]
3000a3e6:	ea40 0008 	orr.w	r0, r0, r8
3000a3ea:	ea43 030b 	orr.w	r3, r3, fp
3000a3ee:	4314      	orrs	r4, r2
3000a3f0:	4622      	mov	r2, r4
3000a3f2:	e9dd 450f 	ldrd	r4, r5, [sp, #60]	; 0x3c
3000a3f6:	432c      	orrs	r4, r5
3000a3f8:	9d14      	ldr	r5, [sp, #80]	; 0x50
3000a3fa:	4329      	orrs	r1, r5
3000a3fc:	e9dd 5716 	ldrd	r5, r7, [sp, #88]	; 0x58
3000a400:	433d      	orrs	r5, r7
3000a402:	9f0b      	ldr	r7, [sp, #44]	; 0x2c
3000a404:	433a      	orrs	r2, r7
3000a406:	9f11      	ldr	r7, [sp, #68]	; 0x44
3000a408:	433c      	orrs	r4, r7
3000a40a:	9f07      	ldr	r7, [sp, #28]
3000a40c:	4339      	orrs	r1, r7
3000a40e:	9f15      	ldr	r7, [sp, #84]	; 0x54
3000a410:	433d      	orrs	r5, r7
3000a412:	9f0a      	ldr	r7, [sp, #40]	; 0x28
3000a414:	433a      	orrs	r2, r7
3000a416:	9f12      	ldr	r7, [sp, #72]	; 0x48
3000a418:	433c      	orrs	r4, r7
3000a41a:	9f06      	ldr	r7, [sp, #24]
3000a41c:	4339      	orrs	r1, r7
3000a41e:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000a420:	433a      	orrs	r2, r7
3000a422:	9f13      	ldr	r7, [sp, #76]	; 0x4c
3000a424:	ea41 010c 	orr.w	r1, r1, ip
3000a428:	433c      	orrs	r4, r7
3000a42a:	9f0c      	ldr	r7, [sp, #48]	; 0x30
3000a42c:	ea41 010e 	orr.w	r1, r1, lr
3000a430:	433a      	orrs	r2, r7
3000a432:	9f1f      	ldr	r7, [sp, #124]	; 0x7c
3000a434:	433c      	orrs	r4, r7
3000a436:	9f0d      	ldr	r7, [sp, #52]	; 0x34
3000a438:	433a      	orrs	r2, r7
3000a43a:	9f1e      	ldr	r7, [sp, #120]	; 0x78
3000a43c:	433c      	orrs	r4, r7
3000a43e:	9f01      	ldr	r7, [sp, #4]
3000a440:	4339      	orrs	r1, r7
3000a442:	9f0e      	ldr	r7, [sp, #56]	; 0x38
3000a444:	433a      	orrs	r2, r7
3000a446:	9f1d      	ldr	r7, [sp, #116]	; 0x74
3000a448:	433c      	orrs	r4, r7
3000a44a:	9f00      	ldr	r7, [sp, #0]
3000a44c:	4339      	orrs	r1, r7
3000a44e:	4fb7      	ldr	r7, [pc, #732]	; (3000a72c <rxi316_perf_tune+0x820>)
3000a450:	f8c7 2400 	str.w	r2, [r7, #1024]	; 0x400
3000a454:	f8c7 4404 	str.w	r4, [r7, #1028]	; 0x404
3000a458:	9a18      	ldr	r2, [sp, #96]	; 0x60
3000a45a:	f8c7 2408 	str.w	r2, [r7, #1032]	; 0x408
3000a45e:	f8c7 540c 	str.w	r5, [r7, #1036]	; 0x40c
3000a462:	f8c7 1410 	str.w	r1, [r7, #1040]	; 0x410
3000a466:	f8c7 0414 	str.w	r0, [r7, #1044]	; 0x414
3000a46a:	f8c7 a418 	str.w	sl, [r7, #1048]	; 0x418
3000a46e:	f8c7 941c 	str.w	r9, [r7, #1052]	; 0x41c
3000a472:	f8c7 3420 	str.w	r3, [r7, #1056]	; 0x420
3000a476:	68f3      	ldr	r3, [r6, #12]
3000a478:	689a      	ldr	r2, [r3, #8]
3000a47a:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a47e:	f003 0301 	and.w	r3, r3, #1
3000a482:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a486:	f8c7 3460 	str.w	r3, [r7, #1120]	; 0x460
3000a48a:	68f3      	ldr	r3, [r6, #12]
3000a48c:	689a      	ldr	r2, [r3, #8]
3000a48e:	7a13      	ldrb	r3, [r2, #8]
3000a490:	6951      	ldr	r1, [r2, #20]
3000a492:	ea43 6301 	orr.w	r3, r3, r1, lsl #24
3000a496:	e9d2 1203 	ldrd	r1, r2, [r2, #12]
3000a49a:	0209      	lsls	r1, r1, #8
3000a49c:	0412      	lsls	r2, r2, #16
3000a49e:	b289      	uxth	r1, r1
3000a4a0:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a4a4:	430b      	orrs	r3, r1
3000a4a6:	4313      	orrs	r3, r2
3000a4a8:	f8c7 3464 	str.w	r3, [r7, #1124]	; 0x464
3000a4ac:	68f3      	ldr	r3, [r6, #12]
3000a4ae:	6899      	ldr	r1, [r3, #8]
3000a4b0:	e9d1 3207 	ldrd	r3, r2, [r1, #28]
3000a4b4:	7e09      	ldrb	r1, [r1, #24]
3000a4b6:	021b      	lsls	r3, r3, #8
3000a4b8:	0412      	lsls	r2, r2, #16
3000a4ba:	b29b      	uxth	r3, r3
3000a4bc:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a4c0:	4313      	orrs	r3, r2
3000a4c2:	430b      	orrs	r3, r1
3000a4c4:	f8c7 3468 	str.w	r3, [r7, #1128]	; 0x468
3000a4c8:	68f3      	ldr	r3, [r6, #12]
3000a4ca:	689a      	ldr	r2, [r3, #8]
3000a4cc:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a4d0:	041b      	lsls	r3, r3, #16
3000a4d2:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a4d6:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a4da:	4313      	orrs	r3, r2
3000a4dc:	f8c7 3480 	str.w	r3, [r7, #1152]	; 0x480
3000a4e0:	68f3      	ldr	r3, [r6, #12]
3000a4e2:	6899      	ldr	r1, [r3, #8]
3000a4e4:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a4e6:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a4e8:	005b      	lsls	r3, r3, #1
3000a4ea:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a4ec:	0412      	lsls	r2, r2, #16
3000a4ee:	f003 0302 	and.w	r3, r3, #2
3000a4f2:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a4f6:	4313      	orrs	r3, r2
3000a4f8:	6b4a      	ldr	r2, [r1, #52]	; 0x34
3000a4fa:	f000 0101 	and.w	r1, r0, #1
3000a4fe:	0212      	lsls	r2, r2, #8
3000a500:	430b      	orrs	r3, r1
3000a502:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000a506:	4313      	orrs	r3, r2
3000a508:	4a89      	ldr	r2, [pc, #548]	; (3000a730 <rxi316_perf_tune+0x824>)
3000a50a:	431a      	orrs	r2, r3
3000a50c:	f8c7 2484 	str.w	r2, [r7, #1156]	; 0x484
3000a510:	68f3      	ldr	r3, [r6, #12]
3000a512:	689b      	ldr	r3, [r3, #8]
3000a514:	6bdb      	ldr	r3, [r3, #60]	; 0x3c
3000a516:	f8c7 3488 	str.w	r3, [r7, #1160]	; 0x488
3000a51a:	68f3      	ldr	r3, [r6, #12]
3000a51c:	689b      	ldr	r3, [r3, #8]
3000a51e:	6c1b      	ldr	r3, [r3, #64]	; 0x40
3000a520:	f8c7 348c 	str.w	r3, [r7, #1164]	; 0x48c
3000a524:	68f3      	ldr	r3, [r6, #12]
3000a526:	689b      	ldr	r3, [r3, #8]
3000a528:	6c5b      	ldr	r3, [r3, #68]	; 0x44
3000a52a:	f8c7 3490 	str.w	r3, [r7, #1168]	; 0x490
3000a52e:	68f3      	ldr	r3, [r6, #12]
3000a530:	689b      	ldr	r3, [r3, #8]
3000a532:	6c9b      	ldr	r3, [r3, #72]	; 0x48
3000a534:	f8c7 3494 	str.w	r3, [r7, #1172]	; 0x494
3000a538:	68f3      	ldr	r3, [r6, #12]
3000a53a:	689b      	ldr	r3, [r3, #8]
3000a53c:	6cdb      	ldr	r3, [r3, #76]	; 0x4c
3000a53e:	f8c7 3498 	str.w	r3, [r7, #1176]	; 0x498
3000a542:	68f3      	ldr	r3, [r6, #12]
3000a544:	689b      	ldr	r3, [r3, #8]
3000a546:	6d1b      	ldr	r3, [r3, #80]	; 0x50
3000a548:	f003 030f 	and.w	r3, r3, #15
3000a54c:	f8c7 34a0 	str.w	r3, [r7, #1184]	; 0x4a0
3000a550:	f8d7 3470 	ldr.w	r3, [r7, #1136]	; 0x470
3000a554:	f023 033f 	bic.w	r3, r3, #63	; 0x3f
3000a558:	f043 0304 	orr.w	r3, r3, #4
3000a55c:	f8c7 3470 	str.w	r3, [r7, #1136]	; 0x470
3000a560:	f8d7 3474 	ldr.w	r3, [r7, #1140]	; 0x474
3000a564:	f423 4370 	bic.w	r3, r3, #61440	; 0xf000
3000a568:	f443 4380 	orr.w	r3, r3, #16384	; 0x4000
3000a56c:	f8c7 3474 	str.w	r3, [r7, #1140]	; 0x474
3000a570:	b041      	add	sp, #260	; 0x104
3000a572:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a576:	2502      	movs	r5, #2
3000a578:	e4d7      	b.n	30009f2a <rxi316_perf_tune+0x1e>
3000a57a:	3101      	adds	r1, #1
3000a57c:	9303      	str	r3, [sp, #12]
3000a57e:	e53e      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a580:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a584:	e72e      	b.n	3000a3e4 <rxi316_perf_tune+0x4d8>
3000a586:	f04f 0b1f 	mov.w	fp, #31
3000a58a:	e723      	b.n	3000a3d4 <rxi316_perf_tune+0x4c8>
3000a58c:	f04f 091f 	mov.w	r9, #31
3000a590:	e716      	b.n	3000a3c0 <rxi316_perf_tune+0x4b4>
3000a592:	f04f 0a1f 	mov.w	sl, #31
3000a596:	e709      	b.n	3000a3ac <rxi316_perf_tune+0x4a0>
3000a598:	20f0      	movs	r0, #240	; 0xf0
3000a59a:	e6fd      	b.n	3000a398 <rxi316_perf_tune+0x48c>
3000a59c:	f04f 080f 	mov.w	r8, #15
3000a5a0:	e6f2      	b.n	3000a388 <rxi316_perf_tune+0x47c>
3000a5a2:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a5a6:	930a      	str	r3, [sp, #40]	; 0x28
3000a5a8:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a5aa:	2b20      	cmp	r3, #32
3000a5ac:	f47f adee 	bne.w	3000a18c <rxi316_perf_tune+0x280>
3000a5b0:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a5b4:	9309      	str	r3, [sp, #36]	; 0x24
3000a5b6:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a5b8:	2b20      	cmp	r3, #32
3000a5ba:	f47f adf1 	bne.w	3000a1a0 <rxi316_perf_tune+0x294>
3000a5be:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a5c2:	930c      	str	r3, [sp, #48]	; 0x30
3000a5c4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a5c6:	2b20      	cmp	r3, #32
3000a5c8:	f47f adf4 	bne.w	3000a1b4 <rxi316_perf_tune+0x2a8>
3000a5cc:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a5d0:	930d      	str	r3, [sp, #52]	; 0x34
3000a5d2:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a5d4:	2b20      	cmp	r3, #32
3000a5d6:	f47f adf7 	bne.w	3000a1c8 <rxi316_perf_tune+0x2bc>
3000a5da:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a5de:	930e      	str	r3, [sp, #56]	; 0x38
3000a5e0:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a5e2:	2b20      	cmp	r3, #32
3000a5e4:	f47f adf8 	bne.w	3000a1d8 <rxi316_perf_tune+0x2cc>
3000a5e8:	230f      	movs	r3, #15
3000a5ea:	930f      	str	r3, [sp, #60]	; 0x3c
3000a5ec:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a5ee:	2b20      	cmp	r3, #32
3000a5f0:	f47f adfb 	bne.w	3000a1ea <rxi316_perf_tune+0x2de>
3000a5f4:	23f0      	movs	r3, #240	; 0xf0
3000a5f6:	9310      	str	r3, [sp, #64]	; 0x40
3000a5f8:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a5fa:	2b20      	cmp	r3, #32
3000a5fc:	f47f adfe 	bne.w	3000a1fc <rxi316_perf_tune+0x2f0>
3000a600:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a604:	9311      	str	r3, [sp, #68]	; 0x44
3000a606:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a608:	2b20      	cmp	r3, #32
3000a60a:	f47f ae01 	bne.w	3000a210 <rxi316_perf_tune+0x304>
3000a60e:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a612:	9312      	str	r3, [sp, #72]	; 0x48
3000a614:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a616:	2b20      	cmp	r3, #32
3000a618:	f47f ae03 	bne.w	3000a222 <rxi316_perf_tune+0x316>
3000a61c:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a620:	9313      	str	r3, [sp, #76]	; 0x4c
3000a622:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a624:	2b20      	cmp	r3, #32
3000a626:	f47f ae06 	bne.w	3000a236 <rxi316_perf_tune+0x32a>
3000a62a:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a62e:	931f      	str	r3, [sp, #124]	; 0x7c
3000a630:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a632:	2b20      	cmp	r3, #32
3000a634:	f47f ae09 	bne.w	3000a24a <rxi316_perf_tune+0x33e>
3000a638:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a63c:	931e      	str	r3, [sp, #120]	; 0x78
3000a63e:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a640:	2b20      	cmp	r3, #32
3000a642:	f47f ae0c 	bne.w	3000a25e <rxi316_perf_tune+0x352>
3000a646:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a64a:	931d      	str	r3, [sp, #116]	; 0x74
3000a64c:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a64e:	2b20      	cmp	r3, #32
3000a650:	f47f ae0d 	bne.w	3000a26e <rxi316_perf_tune+0x362>
3000a654:	230f      	movs	r3, #15
3000a656:	9318      	str	r3, [sp, #96]	; 0x60
3000a658:	9b06      	ldr	r3, [sp, #24]
3000a65a:	2b20      	cmp	r3, #32
3000a65c:	f47f ae10 	bne.w	3000a280 <rxi316_perf_tune+0x374>
3000a660:	231f      	movs	r3, #31
3000a662:	9317      	str	r3, [sp, #92]	; 0x5c
3000a664:	9b07      	ldr	r3, [sp, #28]
3000a666:	2b20      	cmp	r3, #32
3000a668:	f47f ae13 	bne.w	3000a292 <rxi316_perf_tune+0x386>
3000a66c:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a670:	9316      	str	r3, [sp, #88]	; 0x58
3000a672:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a674:	2b20      	cmp	r3, #32
3000a676:	f47f ae16 	bne.w	3000a2a6 <rxi316_perf_tune+0x39a>
3000a67a:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000a67e:	f1b9 0f20 	cmp.w	r9, #32
3000a682:	9315      	str	r3, [sp, #84]	; 0x54
3000a684:	f47f ae1b 	bne.w	3000a2be <rxi316_perf_tune+0x3b2>
3000a688:	230f      	movs	r3, #15
3000a68a:	f1b8 0f20 	cmp.w	r8, #32
3000a68e:	9314      	str	r3, [sp, #80]	; 0x50
3000a690:	f47f ae20 	bne.w	3000a2d4 <rxi316_perf_tune+0x3c8>
3000a694:	2f20      	cmp	r7, #32
3000a696:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a69a:	f47f ae25 	bne.w	3000a2e8 <rxi316_perf_tune+0x3dc>
3000a69e:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a6a2:	f1ba 0f20 	cmp.w	sl, #32
3000a6a6:	9307      	str	r3, [sp, #28]
3000a6a8:	f47f ae2a 	bne.w	3000a300 <rxi316_perf_tune+0x3f4>
3000a6ac:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a6b0:	f1bb 0f20 	cmp.w	fp, #32
3000a6b4:	9306      	str	r3, [sp, #24]
3000a6b6:	f47f ae2f 	bne.w	3000a318 <rxi316_perf_tune+0x40c>
3000a6ba:	9b00      	ldr	r3, [sp, #0]
3000a6bc:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a6c0:	2b20      	cmp	r3, #32
3000a6c2:	f47f ae35 	bne.w	3000a330 <rxi316_perf_tune+0x424>
3000a6c6:	9b01      	ldr	r3, [sp, #4]
3000a6c8:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a6cc:	2b20      	cmp	r3, #32
3000a6ce:	f47f ae3b 	bne.w	3000a348 <rxi316_perf_tune+0x43c>
3000a6d2:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a6d6:	9301      	str	r3, [sp, #4]
3000a6d8:	9b03      	ldr	r3, [sp, #12]
3000a6da:	2b20      	cmp	r3, #32
3000a6dc:	f47f ae46 	bne.w	3000a36c <rxi316_perf_tune+0x460>
3000a6e0:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a6e4:	9300      	str	r3, [sp, #0]
3000a6e6:	e645      	b.n	3000a374 <rxi316_perf_tune+0x468>
3000a6e8:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a6ec:	930b      	str	r3, [sp, #44]	; 0x2c
3000a6ee:	e540      	b.n	3000a172 <rxi316_perf_tune+0x266>
3000a6f0:	22f0      	movs	r2, #240	; 0xf0
3000a6f2:	e534      	b.n	3000a15e <rxi316_perf_tune+0x252>
3000a6f4:	230f      	movs	r3, #15
3000a6f6:	9302      	str	r3, [sp, #8]
3000a6f8:	e529      	b.n	3000a14e <rxi316_perf_tune+0x242>
3000a6fa:	3101      	adds	r1, #1
3000a6fc:	9304      	str	r3, [sp, #16]
3000a6fe:	e47e      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a700:	3101      	adds	r1, #1
3000a702:	9305      	str	r3, [sp, #20]
3000a704:	e47b      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a706:	2202      	movs	r2, #2
3000a708:	921d      	str	r2, [sp, #116]	; 0x74
3000a70a:	e504      	b.n	3000a116 <rxi316_perf_tune+0x20a>
3000a70c:	9a02      	ldr	r2, [sp, #8]
3000a70e:	9307      	str	r3, [sp, #28]
3000a710:	3201      	adds	r2, #1
3000a712:	9202      	str	r2, [sp, #8]
3000a714:	e473      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a716:	9a02      	ldr	r2, [sp, #8]
3000a718:	9306      	str	r3, [sp, #24]
3000a71a:	3201      	adds	r2, #1
3000a71c:	9202      	str	r2, [sp, #8]
3000a71e:	e46e      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a720:	9a02      	ldr	r2, [sp, #8]
3000a722:	931a      	str	r3, [sp, #104]	; 0x68
3000a724:	3201      	adds	r2, #1
3000a726:	9202      	str	r2, [sp, #8]
3000a728:	e469      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a72a:	bf00      	nop
3000a72c:	41100000 	.word	0x41100000
3000a730:	40000008 	.word	0x40000008
3000a734:	9a02      	ldr	r2, [sp, #8]
3000a736:	461c      	mov	r4, r3
3000a738:	3201      	adds	r2, #1
3000a73a:	9202      	str	r2, [sp, #8]
3000a73c:	e45f      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a73e:	9a02      	ldr	r2, [sp, #8]
3000a740:	931c      	str	r3, [sp, #112]	; 0x70
3000a742:	3201      	adds	r2, #1
3000a744:	9202      	str	r2, [sp, #8]
3000a746:	e45a      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a748:	9a02      	ldr	r2, [sp, #8]
3000a74a:	931b      	str	r3, [sp, #108]	; 0x6c
3000a74c:	3201      	adds	r2, #1
3000a74e:	9202      	str	r2, [sp, #8]
3000a750:	e455      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a752:	930d      	str	r3, [sp, #52]	; 0x34
3000a754:	e453      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a756:	930c      	str	r3, [sp, #48]	; 0x30
3000a758:	e451      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a75a:	930b      	str	r3, [sp, #44]	; 0x2c
3000a75c:	e44f      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a75e:	9309      	str	r3, [sp, #36]	; 0x24
3000a760:	e44d      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a762:	930a      	str	r3, [sp, #40]	; 0x28
3000a764:	e44b      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a766:	9319      	str	r3, [sp, #100]	; 0x64
3000a768:	e449      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a76a:	9318      	str	r3, [sp, #96]	; 0x60
3000a76c:	e447      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a76e:	930f      	str	r3, [sp, #60]	; 0x3c
3000a770:	e445      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a772:	930e      	str	r3, [sp, #56]	; 0x38
3000a774:	e443      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a776:	9317      	str	r3, [sp, #92]	; 0x5c
3000a778:	e441      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a77a:	9316      	str	r3, [sp, #88]	; 0x58
3000a77c:	e43f      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a77e:	9315      	str	r3, [sp, #84]	; 0x54
3000a780:	e43d      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a782:	9314      	str	r3, [sp, #80]	; 0x50
3000a784:	e43b      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a786:	9313      	str	r3, [sp, #76]	; 0x4c
3000a788:	e439      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a78a:	9312      	str	r3, [sp, #72]	; 0x48
3000a78c:	e437      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a78e:	9311      	str	r3, [sp, #68]	; 0x44
3000a790:	e435      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a792:	9310      	str	r3, [sp, #64]	; 0x40
3000a794:	e433      	b.n	30009ffe <rxi316_perf_tune+0xf2>
3000a796:	bf00      	nop

3000a798 <rxi316_dram_init>:
3000a798:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a79c:	4604      	mov	r4, r0
3000a79e:	b08f      	sub	sp, #60	; 0x3c
3000a7a0:	4b10      	ldr	r3, [pc, #64]	; (3000a7e4 <rxi316_dram_init+0x4c>)
3000a7a2:	4798      	blx	r3
3000a7a4:	f894 b014 	ldrb.w	fp, [r4, #20]
3000a7a8:	2101      	movs	r1, #1
3000a7aa:	900c      	str	r0, [sp, #48]	; 0x30
3000a7ac:	fa01 f30b 	lsl.w	r3, r1, fp
3000a7b0:	9302      	str	r3, [sp, #8]
3000a7b2:	6823      	ldr	r3, [r4, #0]
3000a7b4:	78da      	ldrb	r2, [r3, #3]
3000a7b6:	6923      	ldr	r3, [r4, #16]
3000a7b8:	fa03 f30b 	lsl.w	r3, r3, fp
3000a7bc:	b1a2      	cbz	r2, 3000a7e8 <rxi316_dram_init+0x50>
3000a7be:	428a      	cmp	r2, r1
3000a7c0:	f000 847c 	beq.w	3000b0bc <rxi316_dram_init+0x924>
3000a7c4:	2a02      	cmp	r2, #2
3000a7c6:	bf0b      	itete	eq
3000a7c8:	2206      	moveq	r2, #6
3000a7ca:	220a      	movne	r2, #10
3000a7cc:	2540      	moveq	r5, #64	; 0x40
3000a7ce:	2580      	movne	r5, #128	; 0x80
3000a7d0:	9207      	str	r2, [sp, #28]
3000a7d2:	bf0c      	ite	eq
3000a7d4:	2204      	moveq	r2, #4
3000a7d6:	2208      	movne	r2, #8
3000a7d8:	9208      	str	r2, [sp, #32]
3000a7da:	bf0c      	ite	eq
3000a7dc:	2208      	moveq	r2, #8
3000a7de:	2210      	movne	r2, #16
3000a7e0:	9201      	str	r2, [sp, #4]
3000a7e2:	e007      	b.n	3000a7f4 <rxi316_dram_init+0x5c>
3000a7e4:	30007f75 	.word	0x30007f75
3000a7e8:	2203      	movs	r2, #3
3000a7ea:	25c0      	movs	r5, #192	; 0xc0
3000a7ec:	9108      	str	r1, [sp, #32]
3000a7ee:	9207      	str	r2, [sp, #28]
3000a7f0:	2202      	movs	r2, #2
3000a7f2:	9201      	str	r2, [sp, #4]
3000a7f4:	6860      	ldr	r0, [r4, #4]
3000a7f6:	f04f 31ff 	mov.w	r1, #4294967295
3000a7fa:	4ac2      	ldr	r2, [pc, #776]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000a7fc:	6806      	ldr	r6, [r0, #0]
3000a7fe:	960d      	str	r6, [sp, #52]	; 0x34
3000a800:	6846      	ldr	r6, [r0, #4]
3000a802:	6880      	ldr	r0, [r0, #8]
3000a804:	9609      	str	r6, [sp, #36]	; 0x24
3000a806:	9003      	str	r0, [sp, #12]
3000a808:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000a80c:	6820      	ldr	r0, [r4, #0]
3000a80e:	7981      	ldrb	r1, [r0, #6]
3000a810:	6882      	ldr	r2, [r0, #8]
3000a812:	b129      	cbz	r1, 3000a820 <rxi316_dram_init+0x88>
3000a814:	2901      	cmp	r1, #1
3000a816:	f000 8449 	beq.w	3000b0ac <rxi316_dram_init+0x914>
3000a81a:	2903      	cmp	r1, #3
3000a81c:	f000 8456 	beq.w	3000b0cc <rxi316_dram_init+0x934>
3000a820:	3a06      	subs	r2, #6
3000a822:	7881      	ldrb	r1, [r0, #2]
3000a824:	f002 020f 	and.w	r2, r2, #15
3000a828:	7940      	ldrb	r0, [r0, #5]
3000a82a:	0109      	lsls	r1, r1, #4
3000a82c:	0280      	lsls	r0, r0, #10
3000a82e:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000a832:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000a836:	4301      	orrs	r1, r0
3000a838:	430d      	orrs	r5, r1
3000a83a:	432a      	orrs	r2, r5
3000a83c:	4db1      	ldr	r5, [pc, #708]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000a83e:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000a842:	68e2      	ldr	r2, [r4, #12]
3000a844:	6812      	ldr	r2, [r2, #0]
3000a846:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000a84a:	f002 0207 	and.w	r2, r2, #7
3000a84e:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000a852:	68e2      	ldr	r2, [r4, #12]
3000a854:	6811      	ldr	r1, [r2, #0]
3000a856:	6822      	ldr	r2, [r4, #0]
3000a858:	6848      	ldr	r0, [r1, #4]
3000a85a:	f892 9000 	ldrb.w	r9, [r2]
3000a85e:	688a      	ldr	r2, [r1, #8]
3000a860:	9004      	str	r0, [sp, #16]
3000a862:	0300      	lsls	r0, r0, #12
3000a864:	0412      	lsls	r2, r2, #16
3000a866:	f009 070f 	and.w	r7, r9, #15
3000a86a:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000a86e:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000a872:	4302      	orrs	r2, r0
3000a874:	68c8      	ldr	r0, [r1, #12]
3000a876:	0440      	lsls	r0, r0, #17
3000a878:	433a      	orrs	r2, r7
3000a87a:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000a87e:	6908      	ldr	r0, [r1, #16]
3000a880:	0480      	lsls	r0, r0, #18
3000a882:	4332      	orrs	r2, r6
3000a884:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000a888:	4310      	orrs	r0, r2
3000a88a:	694a      	ldr	r2, [r1, #20]
3000a88c:	04d2      	lsls	r2, r2, #19
3000a88e:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000a892:	4310      	orrs	r0, r2
3000a894:	698a      	ldr	r2, [r1, #24]
3000a896:	0552      	lsls	r2, r2, #21
3000a898:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000a89c:	4310      	orrs	r0, r2
3000a89e:	69ca      	ldr	r2, [r1, #28]
3000a8a0:	0592      	lsls	r2, r2, #22
3000a8a2:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000a8a6:	4310      	orrs	r0, r2
3000a8a8:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000a8aa:	0612      	lsls	r2, r2, #24
3000a8ac:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000a8b0:	4302      	orrs	r2, r0
3000a8b2:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000a8b6:	06c0      	lsls	r0, r0, #27
3000a8b8:	0789      	lsls	r1, r1, #30
3000a8ba:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000a8be:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000a8c2:	4310      	orrs	r0, r2
3000a8c4:	9a02      	ldr	r2, [sp, #8]
3000a8c6:	0212      	lsls	r2, r2, #8
3000a8c8:	4308      	orrs	r0, r1
3000a8ca:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000a8ce:	4310      	orrs	r0, r2
3000a8d0:	6068      	str	r0, [r5, #4]
3000a8d2:	6822      	ldr	r2, [r4, #0]
3000a8d4:	7992      	ldrb	r2, [r2, #6]
3000a8d6:	2a00      	cmp	r2, #0
3000a8d8:	f040 8317 	bne.w	3000af0a <rxi316_dram_init+0x772>
3000a8dc:	686a      	ldr	r2, [r5, #4]
3000a8de:	f042 0210 	orr.w	r2, r2, #16
3000a8e2:	606a      	str	r2, [r5, #4]
3000a8e4:	68e2      	ldr	r2, [r4, #12]
3000a8e6:	9802      	ldr	r0, [sp, #8]
3000a8e8:	6812      	ldr	r2, [r2, #0]
3000a8ea:	f100 3aff 	add.w	sl, r0, #4294967295
3000a8ee:	6861      	ldr	r1, [r4, #4]
3000a8f0:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000a8f2:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000a8f6:	9005      	str	r0, [sp, #20]
3000a8f8:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000a8fa:	fa2e f80b 	lsr.w	r8, lr, fp
3000a8fe:	6949      	ldr	r1, [r1, #20]
3000a900:	ea1e 0f0a 	tst.w	lr, sl
3000a904:	9006      	str	r0, [sp, #24]
3000a906:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000a908:	fa21 fe0b 	lsr.w	lr, r1, fp
3000a90c:	bf18      	it	ne
3000a90e:	f108 0801 	addne.w	r8, r8, #1
3000a912:	ea11 0f0a 	tst.w	r1, sl
3000a916:	9906      	ldr	r1, [sp, #24]
3000a918:	950a      	str	r5, [sp, #40]	; 0x28
3000a91a:	bf18      	it	ne
3000a91c:	f10e 0e01 	addne.w	lr, lr, #1
3000a920:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000a922:	0049      	lsls	r1, r1, #1
3000a924:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000a928:	950b      	str	r5, [sp, #44]	; 0x2c
3000a92a:	f001 0102 	and.w	r1, r1, #2
3000a92e:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a932:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000a936:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000a93a:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000a93e:	0080      	lsls	r0, r0, #2
3000a940:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000a944:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000a948:	9006      	str	r0, [sp, #24]
3000a94a:	013f      	lsls	r7, r7, #4
3000a94c:	9805      	ldr	r0, [sp, #20]
3000a94e:	f00c 0c08 	and.w	ip, ip, #8
3000a952:	0176      	lsls	r6, r6, #5
3000a954:	f007 0710 	and.w	r7, r7, #16
3000a958:	f000 0001 	and.w	r0, r0, #1
3000a95c:	f006 0620 	and.w	r6, r6, #32
3000a960:	4605      	mov	r5, r0
3000a962:	9806      	ldr	r0, [sp, #24]
3000a964:	f000 0004 	and.w	r0, r0, #4
3000a968:	4301      	orrs	r1, r0
3000a96a:	4329      	orrs	r1, r5
3000a96c:	ea41 0c0c 	orr.w	ip, r1, ip
3000a970:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a972:	ea4c 0707 	orr.w	r7, ip, r7
3000a976:	018d      	lsls	r5, r1, #6
3000a978:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000a97a:	4337      	orrs	r7, r6
3000a97c:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000a980:	0208      	lsls	r0, r1, #8
3000a982:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000a984:	432f      	orrs	r7, r5
3000a986:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000a98a:	06c9      	lsls	r1, r1, #27
3000a98c:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000a990:	4307      	orrs	r7, r0
3000a992:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000a996:	0736      	lsls	r6, r6, #28
3000a998:	430f      	orrs	r7, r1
3000a99a:	0752      	lsls	r2, r2, #29
3000a99c:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000a9a0:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000a9a4:	433e      	orrs	r6, r7
3000a9a6:	4316      	orrs	r6, r2
3000a9a8:	4a56      	ldr	r2, [pc, #344]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000a9aa:	ea46 0608 	orr.w	r6, r6, r8
3000a9ae:	ea46 060e 	orr.w	r6, r6, lr
3000a9b2:	6096      	str	r6, [r2, #8]
3000a9b4:	6862      	ldr	r2, [r4, #4]
3000a9b6:	6990      	ldr	r0, [r2, #24]
3000a9b8:	9a01      	ldr	r2, [sp, #4]
3000a9ba:	2a10      	cmp	r2, #16
3000a9bc:	6822      	ldr	r2, [r4, #0]
3000a9be:	7811      	ldrb	r1, [r2, #0]
3000a9c0:	7916      	ldrb	r6, [r2, #4]
3000a9c2:	f000 833d 	beq.w	3000b040 <rxi316_dram_init+0x8a8>
3000a9c6:	f1a1 0502 	sub.w	r5, r1, #2
3000a9ca:	f1a1 0e09 	sub.w	lr, r1, #9
3000a9ce:	9f01      	ldr	r7, [sp, #4]
3000a9d0:	fab5 f585 	clz	r5, r5
3000a9d4:	fabe fe8e 	clz	lr, lr
3000a9d8:	2f08      	cmp	r7, #8
3000a9da:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000a9de:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000a9e2:	ea45 020e 	orr.w	r2, r5, lr
3000a9e6:	f000 829e 	beq.w	3000af26 <rxi316_dram_init+0x78e>
3000a9ea:	9f01      	ldr	r7, [sp, #4]
3000a9ec:	2f04      	cmp	r7, #4
3000a9ee:	f000 83a5 	beq.w	3000b13c <rxi316_dram_init+0x9a4>
3000a9f2:	9f01      	ldr	r7, [sp, #4]
3000a9f4:	2f02      	cmp	r7, #2
3000a9f6:	bf08      	it	eq
3000a9f8:	4310      	orreq	r0, r2
3000a9fa:	2a00      	cmp	r2, #0
3000a9fc:	f040 8298 	bne.w	3000af30 <rxi316_dram_init+0x798>
3000aa00:	2903      	cmp	r1, #3
3000aa02:	f000 8368 	beq.w	3000b0d6 <rxi316_dram_init+0x93e>
3000aa06:	2500      	movs	r5, #0
3000aa08:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000aa0c:	462f      	mov	r7, r5
3000aa0e:	433d      	orrs	r5, r7
3000aa10:	4f3c      	ldr	r7, [pc, #240]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000aa12:	430d      	orrs	r5, r1
3000aa14:	4305      	orrs	r5, r0
3000aa16:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000aa1a:	637e      	str	r6, [r7, #52]	; 0x34
3000aa1c:	6862      	ldr	r2, [r4, #4]
3000aa1e:	9903      	ldr	r1, [sp, #12]
3000aa20:	69d2      	ldr	r2, [r2, #28]
3000aa22:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000aa26:	63ba      	str	r2, [r7, #56]	; 0x38
3000aa28:	6822      	ldr	r2, [r4, #0]
3000aa2a:	7810      	ldrb	r0, [r2, #0]
3000aa2c:	6862      	ldr	r2, [r4, #4]
3000aa2e:	2803      	cmp	r0, #3
3000aa30:	6a11      	ldr	r1, [r2, #32]
3000aa32:	f000 82f8 	beq.w	3000b026 <rxi316_dram_init+0x88e>
3000aa36:	2809      	cmp	r0, #9
3000aa38:	f000 830d 	beq.w	3000b056 <rxi316_dram_init+0x8be>
3000aa3c:	4831      	ldr	r0, [pc, #196]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000aa3e:	63c1      	str	r1, [r0, #60]	; 0x3c
3000aa40:	6862      	ldr	r2, [r4, #4]
3000aa42:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000aa44:	6402      	str	r2, [r0, #64]	; 0x40
3000aa46:	6865      	ldr	r5, [r4, #4]
3000aa48:	68e8      	ldr	r0, [r5, #12]
3000aa4a:	682a      	ldr	r2, [r5, #0]
3000aa4c:	6869      	ldr	r1, [r5, #4]
3000aa4e:	4402      	add	r2, r0
3000aa50:	68ae      	ldr	r6, [r5, #8]
3000aa52:	4401      	add	r1, r0
3000aa54:	ea1a 0f02 	tst.w	sl, r2
3000aa58:	f000 82b8 	beq.w	3000afcc <rxi316_dram_init+0x834>
3000aa5c:	fa22 f20b 	lsr.w	r2, r2, fp
3000aa60:	1c50      	adds	r0, r2, #1
3000aa62:	ea1a 0f01 	tst.w	sl, r1
3000aa66:	fa21 f50b 	lsr.w	r5, r1, fp
3000aa6a:	6822      	ldr	r2, [r4, #0]
3000aa6c:	fa26 f10b 	lsr.w	r1, r6, fp
3000aa70:	bf18      	it	ne
3000aa72:	3501      	addne	r5, #1
3000aa74:	ea1a 0f06 	tst.w	sl, r6
3000aa78:	7816      	ldrb	r6, [r2, #0]
3000aa7a:	bf18      	it	ne
3000aa7c:	3101      	addne	r1, #1
3000aa7e:	f8d4 c008 	ldr.w	ip, [r4, #8]
3000aa82:	1eb2      	subs	r2, r6, #2
3000aa84:	2a01      	cmp	r2, #1
3000aa86:	f240 82a4 	bls.w	3000afd2 <rxi316_dram_init+0x83a>
3000aa8a:	f8dc 7018 	ldr.w	r7, [ip, #24]
3000aa8e:	fbb7 f2f3 	udiv	r2, r7, r3
3000aa92:	fb03 7712 	mls	r7, r3, r2, r7
3000aa96:	b107      	cbz	r7, 3000aa9a <rxi316_dram_init+0x302>
3000aa98:	3201      	adds	r2, #1
3000aa9a:	2e09      	cmp	r6, #9
3000aa9c:	f040 82b3 	bne.w	3000b006 <rxi316_dram_init+0x86e>
3000aaa0:	9e08      	ldr	r6, [sp, #32]
3000aaa2:	fa26 f60b 	lsr.w	r6, r6, fp
3000aaa6:	016d      	lsls	r5, r5, #5
3000aaa8:	f000 001f 	and.w	r0, r0, #31
3000aaac:	0289      	lsls	r1, r1, #10
3000aaae:	f405 7578 	and.w	r5, r5, #992	; 0x3e0
3000aab2:	0512      	lsls	r2, r2, #20
3000aab4:	f401 41f8 	and.w	r1, r1, #31744	; 0x7c00
3000aab8:	4305      	orrs	r5, r0
3000aaba:	f002 72f8 	and.w	r2, r2, #32505856	; 0x1f00000
3000aabe:	430d      	orrs	r5, r1
3000aac0:	4910      	ldr	r1, [pc, #64]	; (3000ab04 <rxi316_dram_init+0x36c>)
3000aac2:	4315      	orrs	r5, r2
3000aac4:	0672      	lsls	r2, r6, #25
3000aac6:	f002 5278 	and.w	r2, r2, #1040187392	; 0x3e000000
3000aaca:	432a      	orrs	r2, r5
3000aacc:	630a      	str	r2, [r1, #48]	; 0x30
3000aace:	68a0      	ldr	r0, [r4, #8]
3000aad0:	6802      	ldr	r2, [r0, #0]
3000aad2:	fbb2 f1f3 	udiv	r1, r2, r3
3000aad6:	fb03 2211 	mls	r2, r3, r1, r2
3000aada:	2a00      	cmp	r2, #0
3000aadc:	f000 8274 	beq.w	3000afc8 <rxi316_dram_init+0x830>
3000aae0:	1c4a      	adds	r2, r1, #1
3000aae2:	9205      	str	r2, [sp, #20]
3000aae4:	6842      	ldr	r2, [r0, #4]
3000aae6:	fbb2 f0f3 	udiv	r0, r2, r3
3000aaea:	fb03 2210 	mls	r2, r3, r0, r2
3000aaee:	2a00      	cmp	r2, #0
3000aaf0:	f000 8268 	beq.w	3000afc4 <rxi316_dram_init+0x82c>
3000aaf4:	38c7      	subs	r0, #199	; 0xc7
3000aaf6:	68e2      	ldr	r2, [r4, #12]
3000aaf8:	6811      	ldr	r1, [r2, #0]
3000aafa:	4a03      	ldr	r2, [pc, #12]	; (3000ab08 <rxi316_dram_init+0x370>)
3000aafc:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000ab00:	e004      	b.n	3000ab0c <rxi316_dram_init+0x374>
3000ab02:	bf00      	nop
3000ab04:	41100000 	.word	0x41100000
3000ab08:	00ffff00 	.word	0x00ffff00
3000ab0c:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000ab10:	900a      	str	r0, [sp, #40]	; 0x28
3000ab12:	0712      	lsls	r2, r2, #28
3000ab14:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000ab18:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000ab1c:	430a      	orrs	r2, r1
3000ab1e:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000ab22:	430a      	orrs	r2, r1
3000ab24:	49c3      	ldr	r1, [pc, #780]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000ab26:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000ab2a:	610a      	str	r2, [r1, #16]
3000ab2c:	68a2      	ldr	r2, [r4, #8]
3000ab2e:	6911      	ldr	r1, [r2, #16]
3000ab30:	fbb1 fcf3 	udiv	ip, r1, r3
3000ab34:	fb03 111c 	mls	r1, r3, ip, r1
3000ab38:	2900      	cmp	r1, #0
3000ab3a:	f000 8240 	beq.w	3000afbe <rxi316_dram_init+0x826>
3000ab3e:	f10c 0101 	add.w	r1, ip, #1
3000ab42:	9106      	str	r1, [sp, #24]
3000ab44:	6991      	ldr	r1, [r2, #24]
3000ab46:	fbb1 f0f3 	udiv	r0, r1, r3
3000ab4a:	fb03 1110 	mls	r1, r3, r0, r1
3000ab4e:	b101      	cbz	r1, 3000ab52 <rxi316_dram_init+0x3ba>
3000ab50:	3001      	adds	r0, #1
3000ab52:	68d1      	ldr	r1, [r2, #12]
3000ab54:	fbb1 f5f3 	udiv	r5, r1, r3
3000ab58:	fb03 1115 	mls	r1, r3, r5, r1
3000ab5c:	9503      	str	r5, [sp, #12]
3000ab5e:	b109      	cbz	r1, 3000ab64 <rxi316_dram_init+0x3cc>
3000ab60:	3501      	adds	r5, #1
3000ab62:	9503      	str	r5, [sp, #12]
3000ab64:	6a55      	ldr	r5, [r2, #36]	; 0x24
3000ab66:	fbb5 f1f3 	udiv	r1, r5, r3
3000ab6a:	fb03 5511 	mls	r5, r3, r1, r5
3000ab6e:	b105      	cbz	r5, 3000ab72 <rxi316_dram_init+0x3da>
3000ab70:	3101      	adds	r1, #1
3000ab72:	6d15      	ldr	r5, [r2, #80]	; 0x50
3000ab74:	0349      	lsls	r1, r1, #13
3000ab76:	68e2      	ldr	r2, [r4, #12]
3000ab78:	fa25 f60b 	lsr.w	r6, r5, fp
3000ab7c:	ea15 0f0a 	tst.w	r5, sl
3000ab80:	6812      	ldr	r2, [r2, #0]
3000ab82:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000ab86:	bf18      	it	ne
3000ab88:	3601      	addne	r6, #1
3000ab8a:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000ab8c:	05d2      	lsls	r2, r2, #23
3000ab8e:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000ab92:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000ab96:	9a06      	ldr	r2, [sp, #24]
3000ab98:	0112      	lsls	r2, r2, #4
3000ab9a:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000ab9e:	4332      	orrs	r2, r6
3000aba0:	0246      	lsls	r6, r0, #9
3000aba2:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000aba6:	4316      	orrs	r6, r2
3000aba8:	9a03      	ldr	r2, [sp, #12]
3000abaa:	f002 020f 	and.w	r2, r2, #15
3000abae:	4316      	orrs	r6, r2
3000abb0:	4aa0      	ldr	r2, [pc, #640]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000abb2:	4331      	orrs	r1, r6
3000abb4:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000abb8:	6151      	str	r1, [r2, #20]
3000abba:	68a7      	ldr	r7, [r4, #8]
3000abbc:	697a      	ldr	r2, [r7, #20]
3000abbe:	fbb2 fef3 	udiv	lr, r2, r3
3000abc2:	fb03 221e 	mls	r2, r3, lr, r2
3000abc6:	b10a      	cbz	r2, 3000abcc <rxi316_dram_init+0x434>
3000abc8:	f10e 0e01 	add.w	lr, lr, #1
3000abcc:	6939      	ldr	r1, [r7, #16]
3000abce:	fbb1 f2f3 	udiv	r2, r1, r3
3000abd2:	fb03 1112 	mls	r1, r3, r2, r1
3000abd6:	b101      	cbz	r1, 3000abda <rxi316_dram_init+0x442>
3000abd8:	3201      	adds	r2, #1
3000abda:	68fd      	ldr	r5, [r7, #12]
3000abdc:	fbb5 f1f3 	udiv	r1, r5, r3
3000abe0:	fb03 5511 	mls	r5, r3, r1, r5
3000abe4:	b105      	cbz	r5, 3000abe8 <rxi316_dram_init+0x450>
3000abe6:	3101      	adds	r1, #1
3000abe8:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000abea:	fbb5 f6f3 	udiv	r6, r5, r3
3000abee:	fb03 5516 	mls	r5, r3, r6, r5
3000abf2:	b105      	cbz	r5, 3000abf6 <rxi316_dram_init+0x45e>
3000abf4:	3601      	adds	r6, #1
3000abf6:	68bd      	ldr	r5, [r7, #8]
3000abf8:	4411      	add	r1, r2
3000abfa:	fbb5 f2f3 	udiv	r2, r5, r3
3000abfe:	42b1      	cmp	r1, r6
3000ac00:	fb03 5512 	mls	r5, r3, r2, r5
3000ac04:	bf38      	it	cc
3000ac06:	4631      	movcc	r1, r6
3000ac08:	b905      	cbnz	r5, 3000ac0c <rxi316_dram_init+0x474>
3000ac0a:	3a01      	subs	r2, #1
3000ac0c:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000ac0e:	ea15 0f0a 	tst.w	r5, sl
3000ac12:	fa25 fc0b 	lsr.w	ip, r5, fp
3000ac16:	69fd      	ldr	r5, [r7, #28]
3000ac18:	bf18      	it	ne
3000ac1a:	f10c 0c01 	addne.w	ip, ip, #1
3000ac1e:	fa25 f60b 	lsr.w	r6, r5, fp
3000ac22:	ea15 0f0a 	tst.w	r5, sl
3000ac26:	bf18      	it	ne
3000ac28:	3601      	addne	r6, #1
3000ac2a:	f1b9 0f09 	cmp.w	r9, #9
3000ac2e:	f000 81f5 	beq.w	3000b01c <rxi316_dram_init+0x884>
3000ac32:	f1b9 0f02 	cmp.w	r9, #2
3000ac36:	f000 8233 	beq.w	3000b0a0 <rxi316_dram_init+0x908>
3000ac3a:	f1b9 0f03 	cmp.w	r9, #3
3000ac3e:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000ac40:	f000 8298 	beq.w	3000b174 <rxi316_dram_init+0x9dc>
3000ac44:	46a8      	mov	r8, r5
3000ac46:	9d01      	ldr	r5, [sp, #4]
3000ac48:	44a8      	add	r8, r5
3000ac4a:	fa28 f50b 	lsr.w	r5, r8, fp
3000ac4e:	ea18 0f0a 	tst.w	r8, sl
3000ac52:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000ac56:	bf18      	it	ne
3000ac58:	3501      	addne	r5, #1
3000ac5a:	fbb8 f7f3 	udiv	r7, r8, r3
3000ac5e:	fb03 8817 	mls	r8, r3, r7, r8
3000ac62:	f1b8 0f00 	cmp.w	r8, #0
3000ac66:	d000      	beq.n	3000ac6a <rxi316_dram_init+0x4d2>
3000ac68:	3701      	adds	r7, #1
3000ac6a:	0109      	lsls	r1, r1, #4
3000ac6c:	f00e 0e0f 	and.w	lr, lr, #15
3000ac70:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000ac74:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000ac78:	063f      	lsls	r7, r7, #24
3000ac7a:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000ac7e:	ea41 0e0e 	orr.w	lr, r1, lr
3000ac82:	0291      	lsls	r1, r2, #10
3000ac84:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000ac88:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000ac8c:	ea4e 0e01 	orr.w	lr, lr, r1
3000ac90:	0471      	lsls	r1, r6, #17
3000ac92:	ea4e 0c0c 	orr.w	ip, lr, ip
3000ac96:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000ac9a:	ea4c 0c01 	orr.w	ip, ip, r1
3000ac9e:	0529      	lsls	r1, r5, #20
3000aca0:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000aca4:	ea4c 0c01 	orr.w	ip, ip, r1
3000aca8:	4962      	ldr	r1, [pc, #392]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000acaa:	ea4c 0707 	orr.w	r7, ip, r7
3000acae:	618f      	str	r7, [r1, #24]
3000acb0:	6821      	ldr	r1, [r4, #0]
3000acb2:	780f      	ldrb	r7, [r1, #0]
3000acb4:	2f02      	cmp	r7, #2
3000acb6:	f000 8152 	beq.w	3000af5e <rxi316_dram_init+0x7c6>
3000acba:	2f09      	cmp	r7, #9
3000acbc:	f000 814f 	beq.w	3000af5e <rxi316_dram_init+0x7c6>
3000acc0:	68e1      	ldr	r1, [r4, #12]
3000acc2:	2f03      	cmp	r7, #3
3000acc4:	6809      	ldr	r1, [r1, #0]
3000acc6:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000acc8:	f001 080f 	and.w	r8, r1, #15
3000accc:	68a1      	ldr	r1, [r4, #8]
3000acce:	6a09      	ldr	r1, [r1, #32]
3000acd0:	d005      	beq.n	3000acde <rxi316_dram_init+0x546>
3000acd2:	ea0a 0701 	and.w	r7, sl, r1
3000acd6:	fa21 f10b 	lsr.w	r1, r1, fp
3000acda:	b107      	cbz	r7, 3000acde <rxi316_dram_init+0x546>
3000acdc:	3101      	adds	r1, #1
3000acde:	f1b9 0f02 	cmp.w	r9, #2
3000ace2:	f000 8154 	beq.w	3000af8e <rxi316_dram_init+0x7f6>
3000ace6:	f1b9 0f03 	cmp.w	r9, #3
3000acea:	f000 81b6 	beq.w	3000b05a <rxi316_dram_init+0x8c2>
3000acee:	f1b9 0f09 	cmp.w	r9, #9
3000acf2:	f000 822f 	beq.w	3000b154 <rxi316_dram_init+0x9bc>
3000acf6:	f04f 0e00 	mov.w	lr, #0
3000acfa:	46f1      	mov	r9, lr
3000acfc:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000ad00:	0109      	lsls	r1, r1, #4
3000ad02:	9f06      	ldr	r7, [sp, #24]
3000ad04:	b2c9      	uxtb	r1, r1
3000ad06:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000ad0a:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000ad0c:	ea41 0108 	orr.w	r1, r1, r8
3000ad10:	4339      	orrs	r1, r7
3000ad12:	4f48      	ldr	r7, [pc, #288]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000ad14:	ea41 0109 	orr.w	r1, r1, r9
3000ad18:	ea41 010e 	orr.w	r1, r1, lr
3000ad1c:	61f9      	str	r1, [r7, #28]
3000ad1e:	68a7      	ldr	r7, [r4, #8]
3000ad20:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000ad22:	fbb4 f1f3 	udiv	r1, r4, r3
3000ad26:	fb03 4311 	mls	r3, r3, r1, r4
3000ad2a:	b103      	cbz	r3, 3000ad2e <rxi316_dram_init+0x596>
3000ad2c:	3101      	adds	r1, #1
3000ad2e:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000ad30:	094b      	lsrs	r3, r1, #5
3000ad32:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000ad36:	0912      	lsrs	r2, r2, #4
3000ad38:	ea14 0f0a 	tst.w	r4, sl
3000ad3c:	fa24 f80b 	lsr.w	r8, r4, fp
3000ad40:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000ad42:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000ad46:	bf18      	it	ne
3000ad48:	f108 0801 	addne.w	r8, r8, #1
3000ad4c:	9306      	str	r3, [sp, #24]
3000ad4e:	ea14 0f0a 	tst.w	r4, sl
3000ad52:	fa24 fe0b 	lsr.w	lr, r4, fp
3000ad56:	9b01      	ldr	r3, [sp, #4]
3000ad58:	fa29 f40b 	lsr.w	r4, r9, fp
3000ad5c:	bf18      	it	ne
3000ad5e:	f10e 0e01 	addne.w	lr, lr, #1
3000ad62:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000ad64:	ea19 0f0a 	tst.w	r9, sl
3000ad68:	fa23 f90b 	lsr.w	r9, r3, fp
3000ad6c:	9905      	ldr	r1, [sp, #20]
3000ad6e:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000ad72:	bf18      	it	ne
3000ad74:	3401      	addne	r4, #1
3000ad76:	45b9      	cmp	r9, r7
3000ad78:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000ad7c:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000ad80:	f240 80eb 	bls.w	3000af5a <rxi316_dram_init+0x7c2>
3000ad84:	eba9 0707 	sub.w	r7, r9, r7
3000ad88:	fa27 f40b 	lsr.w	r4, r7, fp
3000ad8c:	ea17 070a 	ands.w	r7, r7, sl
3000ad90:	d001      	beq.n	3000ad96 <rxi316_dram_init+0x5fe>
3000ad92:	3401      	adds	r4, #1
3000ad94:	2700      	movs	r7, #0
3000ad96:	9b04      	ldr	r3, [sp, #16]
3000ad98:	b10b      	cbz	r3, 3000ad9e <rxi316_dram_init+0x606>
3000ad9a:	073b      	lsls	r3, r7, #28
3000ad9c:	9304      	str	r3, [sp, #16]
3000ad9e:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000ada2:	9b06      	ldr	r3, [sp, #24]
3000ada4:	0289      	lsls	r1, r1, #10
3000ada6:	02c0      	lsls	r0, r0, #11
3000ada8:	f00c 0c04 	and.w	ip, ip, #4
3000adac:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000adb0:	01d2      	lsls	r2, r2, #7
3000adb2:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000adb6:	0236      	lsls	r6, r6, #8
3000adb8:	ea4c 0c01 	orr.w	ip, ip, r1
3000adbc:	b2d2      	uxtb	r2, r2
3000adbe:	026d      	lsls	r5, r5, #9
3000adc0:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000adc4:	ea4c 0c00 	orr.w	ip, ip, r0
3000adc8:	00df      	lsls	r7, r3, #3
3000adca:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000adce:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000add2:	ea4c 0202 	orr.w	r2, ip, r2
3000add6:	f007 0708 	and.w	r7, r7, #8
3000adda:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000adde:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000ade2:	4332      	orrs	r2, r6
3000ade4:	0524      	lsls	r4, r4, #20
3000ade6:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000adea:	4912      	ldr	r1, [pc, #72]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000adec:	ea42 0305 	orr.w	r3, r2, r5
3000adf0:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000adf4:	9a04      	ldr	r2, [sp, #16]
3000adf6:	433b      	orrs	r3, r7
3000adf8:	ea43 0308 	orr.w	r3, r3, r8
3000adfc:	ea43 030e 	orr.w	r3, r3, lr
3000ae00:	4323      	orrs	r3, r4
3000ae02:	4313      	orrs	r3, r2
3000ae04:	9a03      	ldr	r2, [sp, #12]
3000ae06:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000ae0a:	620b      	str	r3, [r1, #32]
3000ae0c:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000ae10:	9b02      	ldr	r3, [sp, #8]
3000ae12:	2b01      	cmp	r3, #1
3000ae14:	d002      	beq.n	3000ae1c <rxi316_dram_init+0x684>
3000ae16:	9b08      	ldr	r3, [sp, #32]
3000ae18:	3303      	adds	r3, #3
3000ae1a:	9307      	str	r3, [sp, #28]
3000ae1c:	4b05      	ldr	r3, [pc, #20]	; (3000ae34 <rxi316_dram_init+0x69c>)
3000ae1e:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000ae22:	9a07      	ldr	r2, [sp, #28]
3000ae24:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000ae28:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000ae2c:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000ae30:	e002      	b.n	3000ae38 <rxi316_dram_init+0x6a0>
3000ae32:	bf00      	nop
3000ae34:	41100000 	.word	0x41100000
3000ae38:	60da      	str	r2, [r3, #12]
3000ae3a:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000ae3e:	430a      	orrs	r2, r1
3000ae40:	49b3      	ldr	r1, [pc, #716]	; (3000b110 <rxi316_dram_init+0x978>)
3000ae42:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000ae46:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000ae4a:	0096      	lsls	r6, r2, #2
3000ae4c:	d57f      	bpl.n	3000af4e <rxi316_dram_init+0x7b6>
3000ae4e:	691a      	ldr	r2, [r3, #16]
3000ae50:	4cb0      	ldr	r4, [pc, #704]	; (3000b114 <rxi316_dram_init+0x97c>)
3000ae52:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000ae56:	611a      	str	r2, [r3, #16]
3000ae58:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000ae5c:	601a      	str	r2, [r3, #0]
3000ae5e:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000ae62:	f043 0303 	orr.w	r3, r3, #3
3000ae66:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000ae6a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000ae6e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000ae70:	2b01      	cmp	r3, #1
3000ae72:	4ba9      	ldr	r3, [pc, #676]	; (3000b118 <rxi316_dram_init+0x980>)
3000ae74:	bf0c      	ite	eq
3000ae76:	4aa9      	ldreq	r2, [pc, #676]	; (3000b11c <rxi316_dram_init+0x984>)
3000ae78:	4aa9      	ldrne	r2, [pc, #676]	; (3000b120 <rxi316_dram_init+0x988>)
3000ae7a:	601a      	str	r2, [r3, #0]
3000ae7c:	2203      	movs	r2, #3
3000ae7e:	68e0      	ldr	r0, [r4, #12]
3000ae80:	4ba8      	ldr	r3, [pc, #672]	; (3000b124 <rxi316_dram_init+0x98c>)
3000ae82:	4003      	ands	r3, r0
3000ae84:	60e3      	str	r3, [r4, #12]
3000ae86:	2308      	movs	r3, #8
3000ae88:	62a1      	str	r1, [r4, #40]	; 0x28
3000ae8a:	62e2      	str	r2, [r4, #44]	; 0x2c
3000ae8c:	6023      	str	r3, [r4, #0]
3000ae8e:	6823      	ldr	r3, [r4, #0]
3000ae90:	071d      	lsls	r5, r3, #28
3000ae92:	d5fc      	bpl.n	3000ae8e <rxi316_dram_init+0x6f6>
3000ae94:	4ba4      	ldr	r3, [pc, #656]	; (3000b128 <rxi316_dram_init+0x990>)
3000ae96:	2001      	movs	r0, #1
3000ae98:	4798      	blx	r3
3000ae9a:	68e2      	ldr	r2, [r4, #12]
3000ae9c:	4ba1      	ldr	r3, [pc, #644]	; (3000b124 <rxi316_dram_init+0x98c>)
3000ae9e:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000aea2:	4013      	ands	r3, r2
3000aea4:	2203      	movs	r2, #3
3000aea6:	60e3      	str	r3, [r4, #12]
3000aea8:	2308      	movs	r3, #8
3000aeaa:	62a1      	str	r1, [r4, #40]	; 0x28
3000aeac:	62e2      	str	r2, [r4, #44]	; 0x2c
3000aeae:	4a99      	ldr	r2, [pc, #612]	; (3000b114 <rxi316_dram_init+0x97c>)
3000aeb0:	6023      	str	r3, [r4, #0]
3000aeb2:	6813      	ldr	r3, [r2, #0]
3000aeb4:	071c      	lsls	r4, r3, #28
3000aeb6:	d5fc      	bpl.n	3000aeb2 <rxi316_dram_init+0x71a>
3000aeb8:	68d0      	ldr	r0, [r2, #12]
3000aeba:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000aebe:	4b99      	ldr	r3, [pc, #612]	; (3000b124 <rxi316_dram_init+0x98c>)
3000aec0:	4003      	ands	r3, r0
3000aec2:	2008      	movs	r0, #8
3000aec4:	60d3      	str	r3, [r2, #12]
3000aec6:	6291      	str	r1, [r2, #40]	; 0x28
3000aec8:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000aeca:	4992      	ldr	r1, [pc, #584]	; (3000b114 <rxi316_dram_init+0x97c>)
3000aecc:	f023 0302 	bic.w	r3, r3, #2
3000aed0:	62d3      	str	r3, [r2, #44]	; 0x2c
3000aed2:	6010      	str	r0, [r2, #0]
3000aed4:	680b      	ldr	r3, [r1, #0]
3000aed6:	0718      	lsls	r0, r3, #28
3000aed8:	d5fc      	bpl.n	3000aed4 <rxi316_dram_init+0x73c>
3000aeda:	4a8e      	ldr	r2, [pc, #568]	; (3000b114 <rxi316_dram_init+0x97c>)
3000aedc:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000aee0:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000aee4:	60d3      	str	r3, [r2, #12]
3000aee6:	980a      	ldr	r0, [sp, #40]	; 0x28
3000aee8:	6913      	ldr	r3, [r2, #16]
3000aeea:	4318      	orrs	r0, r3
3000aeec:	6110      	str	r0, [r2, #16]
3000aeee:	6913      	ldr	r3, [r2, #16]
3000aef0:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000aef4:	6113      	str	r3, [r2, #16]
3000aef6:	6011      	str	r1, [r2, #0]
3000aef8:	68d3      	ldr	r3, [r2, #12]
3000aefa:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000aefe:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000af02:	d1f9      	bne.n	3000aef8 <rxi316_dram_init+0x760>
3000af04:	b00f      	add	sp, #60	; 0x3c
3000af06:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000af0a:	2a01      	cmp	r2, #1
3000af0c:	f000 80d1 	beq.w	3000b0b2 <rxi316_dram_init+0x91a>
3000af10:	2a02      	cmp	r2, #2
3000af12:	f000 8139 	beq.w	3000b188 <rxi316_dram_init+0x9f0>
3000af16:	2a03      	cmp	r2, #3
3000af18:	f47f ace4 	bne.w	3000a8e4 <rxi316_dram_init+0x14c>
3000af1c:	686a      	ldr	r2, [r5, #4]
3000af1e:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000af22:	606a      	str	r2, [r5, #4]
3000af24:	e4de      	b.n	3000a8e4 <rxi316_dram_init+0x14c>
3000af26:	2a00      	cmp	r2, #0
3000af28:	f43f ad6a 	beq.w	3000aa00 <rxi316_dram_init+0x268>
3000af2c:	2203      	movs	r2, #3
3000af2e:	4310      	orrs	r0, r2
3000af30:	68a2      	ldr	r2, [r4, #8]
3000af32:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000af34:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000af36:	013f      	lsls	r7, r7, #4
3000af38:	2d00      	cmp	r5, #0
3000af3a:	f000 80d7 	beq.w	3000b0ec <rxi316_dram_init+0x954>
3000af3e:	1e91      	subs	r1, r2, #2
3000af40:	2906      	cmp	r1, #6
3000af42:	f200 80b0 	bhi.w	3000b0a6 <rxi316_dram_init+0x90e>
3000af46:	1e55      	subs	r5, r2, #1
3000af48:	2100      	movs	r1, #0
3000af4a:	026d      	lsls	r5, r5, #9
3000af4c:	e55f      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000af4e:	2201      	movs	r2, #1
3000af50:	601a      	str	r2, [r3, #0]
3000af52:	681a      	ldr	r2, [r3, #0]
3000af54:	07d2      	lsls	r2, r2, #31
3000af56:	d5fc      	bpl.n	3000af52 <rxi316_dram_init+0x7ba>
3000af58:	e7bf      	b.n	3000aeda <rxi316_dram_init+0x742>
3000af5a:	2701      	movs	r7, #1
3000af5c:	e71b      	b.n	3000ad96 <rxi316_dram_init+0x5fe>
3000af5e:	68a1      	ldr	r1, [r4, #8]
3000af60:	2f03      	cmp	r7, #3
3000af62:	6a09      	ldr	r1, [r1, #32]
3000af64:	f000 80bf 	beq.w	3000b0e6 <rxi316_dram_init+0x94e>
3000af68:	2f02      	cmp	r7, #2
3000af6a:	f040 8118 	bne.w	3000b19e <rxi316_dram_init+0xa06>
3000af6e:	fbb1 fcf3 	udiv	ip, r1, r3
3000af72:	fb03 1e1c 	mls	lr, r3, ip, r1
3000af76:	4661      	mov	r1, ip
3000af78:	f1be 0f00 	cmp.w	lr, #0
3000af7c:	f000 80a9 	beq.w	3000b0d2 <rxi316_dram_init+0x93a>
3000af80:	f1b9 0f02 	cmp.w	r9, #2
3000af84:	f101 0101 	add.w	r1, r1, #1
3000af88:	46b8      	mov	r8, r7
3000af8a:	f47f aeac 	bne.w	3000ace6 <rxi316_dram_init+0x54e>
3000af8e:	4f67      	ldr	r7, [pc, #412]	; (3000b12c <rxi316_dram_init+0x994>)
3000af90:	f04f 0e00 	mov.w	lr, #0
3000af94:	f8df 9198 	ldr.w	r9, [pc, #408]	; 3000b130 <rxi316_dram_init+0x998>
3000af98:	fbb7 f7f3 	udiv	r7, r7, r3
3000af9c:	fbb9 f9f3 	udiv	r9, r9, r3
3000afa0:	09ff      	lsrs	r7, r7, #7
3000afa2:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000afa6:	3701      	adds	r7, #1
3000afa8:	f109 0c01 	add.w	ip, r9, #1
3000afac:	f8df 9184 	ldr.w	r9, [pc, #388]	; 3000b134 <rxi316_dram_init+0x99c>
3000afb0:	023f      	lsls	r7, r7, #8
3000afb2:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000afb6:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000afba:	9709      	str	r7, [sp, #36]	; 0x24
3000afbc:	e6a0      	b.n	3000ad00 <rxi316_dram_init+0x568>
3000afbe:	f8cd c018 	str.w	ip, [sp, #24]
3000afc2:	e5bf      	b.n	3000ab44 <rxi316_dram_init+0x3ac>
3000afc4:	38c8      	subs	r0, #200	; 0xc8
3000afc6:	e596      	b.n	3000aaf6 <rxi316_dram_init+0x35e>
3000afc8:	9105      	str	r1, [sp, #20]
3000afca:	e58b      	b.n	3000aae4 <rxi316_dram_init+0x34c>
3000afcc:	fa22 f00b 	lsr.w	r0, r2, fp
3000afd0:	e547      	b.n	3000aa62 <rxi316_dram_init+0x2ca>
3000afd2:	f8dc 705c 	ldr.w	r7, [ip, #92]	; 0x5c
3000afd6:	ea1a 0f07 	tst.w	sl, r7
3000afda:	fa27 f20b 	lsr.w	r2, r7, fp
3000afde:	d000      	beq.n	3000afe2 <rxi316_dram_init+0x84a>
3000afe0:	3201      	adds	r2, #1
3000afe2:	2e02      	cmp	r6, #2
3000afe4:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000afe8:	d10f      	bne.n	3000b00a <rxi316_dram_init+0x872>
3000afea:	9e08      	ldr	r6, [sp, #32]
3000afec:	3e02      	subs	r6, #2
3000afee:	fa26 f60b 	lsr.w	r6, r6, fp
3000aff2:	4437      	add	r7, r6
3000aff4:	fbb7 f6f3 	udiv	r6, r7, r3
3000aff8:	fb03 7716 	mls	r7, r3, r6, r7
3000affc:	2f00      	cmp	r7, #0
3000affe:	f43f ad52 	beq.w	3000aaa6 <rxi316_dram_init+0x30e>
3000b002:	3601      	adds	r6, #1
3000b004:	e54f      	b.n	3000aaa6 <rxi316_dram_init+0x30e>
3000b006:	f8dc 7024 	ldr.w	r7, [ip, #36]	; 0x24
3000b00a:	fbb7 f6f3 	udiv	r6, r7, r3
3000b00e:	fb03 7716 	mls	r7, r3, r6, r7
3000b012:	2f00      	cmp	r7, #0
3000b014:	f43f ad47 	beq.w	3000aaa6 <rxi316_dram_init+0x30e>
3000b018:	3601      	adds	r6, #1
3000b01a:	e544      	b.n	3000aaa6 <rxi316_dram_init+0x30e>
3000b01c:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b01e:	46a8      	mov	r8, r5
3000b020:	9d08      	ldr	r5, [sp, #32]
3000b022:	44a8      	add	r8, r5
3000b024:	e611      	b.n	3000ac4a <rxi316_dram_init+0x4b2>
3000b026:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b028:	1f50      	subs	r0, r2, #5
3000b02a:	00c0      	lsls	r0, r0, #3
3000b02c:	68e2      	ldr	r2, [r4, #12]
3000b02e:	6812      	ldr	r2, [r2, #0]
3000b030:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b032:	fab2 f282 	clz	r2, r2
3000b036:	0952      	lsrs	r2, r2, #5
3000b038:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b03c:	4301      	orrs	r1, r0
3000b03e:	e4fd      	b.n	3000aa3c <rxi316_dram_init+0x2a4>
3000b040:	f1a1 0502 	sub.w	r5, r1, #2
3000b044:	2909      	cmp	r1, #9
3000b046:	fab5 f585 	clz	r5, r5
3000b04a:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b04e:	f000 8099 	beq.w	3000b184 <rxi316_dram_init+0x9ec>
3000b052:	462a      	mov	r2, r5
3000b054:	e4d1      	b.n	3000a9fa <rxi316_dram_init+0x262>
3000b056:	2000      	movs	r0, #0
3000b058:	e7e8      	b.n	3000b02c <rxi316_dram_init+0x894>
3000b05a:	f242 7710 	movw	r7, #10000	; 0x2710
3000b05e:	f8df 90d8 	ldr.w	r9, [pc, #216]	; 3000b138 <rxi316_dram_init+0x9a0>
3000b062:	f8df e0cc 	ldr.w	lr, [pc, #204]	; 3000b130 <rxi316_dram_init+0x998>
3000b066:	fbb7 fcf3 	udiv	ip, r7, r3
3000b06a:	9f05      	ldr	r7, [sp, #20]
3000b06c:	4467      	add	r7, ip
3000b06e:	fbb9 f9f3 	udiv	r9, r9, r3
3000b072:	fbbe fef3 	udiv	lr, lr, r3
3000b076:	09ff      	lsrs	r7, r7, #7
3000b078:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b07c:	3701      	adds	r7, #1
3000b07e:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b082:	f109 0c01 	add.w	ip, r9, #1
3000b086:	f8df 90ac 	ldr.w	r9, [pc, #172]	; 3000b134 <rxi316_dram_init+0x99c>
3000b08a:	023f      	lsls	r7, r7, #8
3000b08c:	f10e 0e01 	add.w	lr, lr, #1
3000b090:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b094:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b098:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b09c:	9709      	str	r7, [sp, #36]	; 0x24
3000b09e:	e62f      	b.n	3000ad00 <rxi316_dram_init+0x568>
3000b0a0:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b0a4:	e5d1      	b.n	3000ac4a <rxi316_dram_init+0x4b2>
3000b0a6:	2100      	movs	r1, #0
3000b0a8:	460d      	mov	r5, r1
3000b0aa:	e4b0      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b0ac:	3a07      	subs	r2, #7
3000b0ae:	f7ff bbb8 	b.w	3000a822 <rxi316_dram_init+0x8a>
3000b0b2:	686a      	ldr	r2, [r5, #4]
3000b0b4:	f042 0220 	orr.w	r2, r2, #32
3000b0b8:	606a      	str	r2, [r5, #4]
3000b0ba:	e413      	b.n	3000a8e4 <rxi316_dram_init+0x14c>
3000b0bc:	2204      	movs	r2, #4
3000b0be:	2102      	movs	r1, #2
3000b0c0:	2500      	movs	r5, #0
3000b0c2:	9201      	str	r2, [sp, #4]
3000b0c4:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b0c8:	f7ff bb94 	b.w	3000a7f4 <rxi316_dram_init+0x5c>
3000b0cc:	3a08      	subs	r2, #8
3000b0ce:	f7ff bba8 	b.w	3000a822 <rxi316_dram_init+0x8a>
3000b0d2:	46b8      	mov	r8, r7
3000b0d4:	e603      	b.n	3000acde <rxi316_dram_init+0x546>
3000b0d6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b0d8:	2a04      	cmp	r2, #4
3000b0da:	d90c      	bls.n	3000b0f6 <rxi316_dram_init+0x95e>
3000b0dc:	2a0b      	cmp	r2, #11
3000b0de:	d86a      	bhi.n	3000b1b6 <rxi316_dram_init+0xa1e>
3000b0e0:	1f17      	subs	r7, r2, #4
3000b0e2:	013f      	lsls	r7, r7, #4
3000b0e4:	e008      	b.n	3000b0f8 <rxi316_dram_init+0x960>
3000b0e6:	f04f 0802 	mov.w	r8, #2
3000b0ea:	e5f8      	b.n	3000acde <rxi316_dram_init+0x546>
3000b0ec:	2903      	cmp	r1, #3
3000b0ee:	d005      	beq.n	3000b0fc <rxi316_dram_init+0x964>
3000b0f0:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b0f4:	e48b      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b0f6:	2710      	movs	r7, #16
3000b0f8:	68a2      	ldr	r2, [r4, #8]
3000b0fa:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b0fc:	2a04      	cmp	r2, #4
3000b0fe:	d949      	bls.n	3000b194 <rxi316_dram_init+0x9fc>
3000b100:	1f51      	subs	r1, r2, #5
3000b102:	2903      	cmp	r1, #3
3000b104:	d84e      	bhi.n	3000b1a4 <rxi316_dram_init+0xa0c>
3000b106:	1f15      	subs	r5, r2, #4
3000b108:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b10c:	026d      	lsls	r5, r5, #9
3000b10e:	e47e      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b110:	42008000 	.word	0x42008000
3000b114:	41100000 	.word	0x41100000
3000b118:	41011000 	.word	0x41011000
3000b11c:	44000103 	.word	0x44000103
3000b120:	44000021 	.word	0x44000021
3000b124:	fff8fffc 	.word	0xfff8fffc
3000b128:	00009b2d 	.word	0x00009b2d
3000b12c:	00061a80 	.word	0x00061a80
3000b130:	0bebc200 	.word	0x0bebc200
3000b134:	003ff000 	.word	0x003ff000
3000b138:	1dcd6500 	.word	0x1dcd6500
3000b13c:	f1a1 0c02 	sub.w	ip, r1, #2
3000b140:	f1bc 0f01 	cmp.w	ip, #1
3000b144:	d903      	bls.n	3000b14e <rxi316_dram_init+0x9b6>
3000b146:	f1be 0f00 	cmp.w	lr, #0
3000b14a:	f43f ac56 	beq.w	3000a9fa <rxi316_dram_init+0x262>
3000b14e:	f040 0002 	orr.w	r0, r0, #2
3000b152:	e452      	b.n	3000a9fa <rxi316_dram_init+0x262>
3000b154:	4f1d      	ldr	r7, [pc, #116]	; (3000b1cc <rxi316_dram_init+0xa34>)
3000b156:	f04f 0e00 	mov.w	lr, #0
3000b15a:	fbb7 f9f3 	udiv	r9, r7, r3
3000b15e:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b162:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b166:	f109 0701 	add.w	r7, r9, #1
3000b16a:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b1d0 <rxi316_dram_init+0xa38>
3000b16e:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b172:	e5c5      	b.n	3000ad00 <rxi316_dram_init+0x568>
3000b174:	f105 0802 	add.w	r8, r5, #2
3000b178:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b17a:	eba8 0805 	sub.w	r8, r8, r5
3000b17e:	9d08      	ldr	r5, [sp, #32]
3000b180:	44a8      	add	r8, r5
3000b182:	e562      	b.n	3000ac4a <rxi316_dram_init+0x4b2>
3000b184:	2204      	movs	r2, #4
3000b186:	e6d2      	b.n	3000af2e <rxi316_dram_init+0x796>
3000b188:	686a      	ldr	r2, [r5, #4]
3000b18a:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b18e:	606a      	str	r2, [r5, #4]
3000b190:	f7ff bba8 	b.w	3000a8e4 <rxi316_dram_init+0x14c>
3000b194:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b198:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b19c:	e437      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b19e:	f04f 0802 	mov.w	r8, #2
3000b1a2:	e596      	b.n	3000acd2 <rxi316_dram_init+0x53a>
3000b1a4:	f1a2 0109 	sub.w	r1, r2, #9
3000b1a8:	2905      	cmp	r1, #5
3000b1aa:	d80b      	bhi.n	3000b1c4 <rxi316_dram_init+0xa2c>
3000b1ac:	0855      	lsrs	r5, r2, #1
3000b1ae:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b1b2:	026d      	lsls	r5, r5, #9
3000b1b4:	e42b      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b1b6:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b1b8:	f040 0004 	orr.w	r0, r0, #4
3000b1bc:	f1a2 070c 	sub.w	r7, r2, #12
3000b1c0:	013f      	lsls	r7, r7, #4
3000b1c2:	e799      	b.n	3000b0f8 <rxi316_dram_init+0x960>
3000b1c4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b1c8:	2500      	movs	r5, #0
3000b1ca:	e420      	b.n	3000aa0e <rxi316_dram_init+0x276>
3000b1cc:	0bebc200 	.word	0x0bebc200
3000b1d0:	003ff000 	.word	0x003ff000

3000b1d4 <rxi316_ftend_init>:
3000b1d4:	2300      	movs	r3, #0
3000b1d6:	b510      	push	{r4, lr}
3000b1d8:	4c0c      	ldr	r4, [pc, #48]	; (3000b20c <rxi316_ftend_init+0x38>)
3000b1da:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b1de:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b1e2:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b1e6:	f7fe fe67 	bl	30009eb8 <ddr_init_index>
3000b1ea:	4a09      	ldr	r2, [pc, #36]	; (3000b210 <rxi316_ftend_init+0x3c>)
3000b1ec:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b1f0:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b1f4:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b1f8:	68cb      	ldr	r3, [r1, #12]
3000b1fa:	689b      	ldr	r3, [r3, #8]
3000b1fc:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b1fe:	3301      	adds	r3, #1
3000b200:	b2db      	uxtb	r3, r3
3000b202:	4313      	orrs	r3, r2
3000b204:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b208:	bd10      	pop	{r4, pc}
3000b20a:	bf00      	nop
3000b20c:	41101000 	.word	0x41101000
3000b210:	3000f44c 	.word	0x3000f44c

3000b214 <rxi316_DynSre_init>:
3000b214:	2901      	cmp	r1, #1
3000b216:	d013      	beq.n	3000b240 <rxi316_DynSre_init+0x2c>
3000b218:	4a12      	ldr	r2, [pc, #72]	; (3000b264 <rxi316_DynSre_init+0x50>)
3000b21a:	6893      	ldr	r3, [r2, #8]
3000b21c:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b220:	6093      	str	r3, [r2, #8]
3000b222:	4b10      	ldr	r3, [pc, #64]	; (3000b264 <rxi316_DynSre_init+0x50>)
3000b224:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b228:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b22c:	625a      	str	r2, [r3, #36]	; 0x24
3000b22e:	629a      	str	r2, [r3, #40]	; 0x28
3000b230:	69da      	ldr	r2, [r3, #28]
3000b232:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b236:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b23a:	61da      	str	r2, [r3, #28]
3000b23c:	6019      	str	r1, [r3, #0]
3000b23e:	4770      	bx	lr
3000b240:	4b08      	ldr	r3, [pc, #32]	; (3000b264 <rxi316_DynSre_init+0x50>)
3000b242:	b280      	uxth	r0, r0
3000b244:	4a08      	ldr	r2, [pc, #32]	; (3000b268 <rxi316_DynSre_init+0x54>)
3000b246:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b24a:	400a      	ands	r2, r1
3000b24c:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b250:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000b254:	4310      	orrs	r0, r2
3000b256:	f8c3 0100 	str.w	r0, [r3, #256]	; 0x100
3000b25a:	689a      	ldr	r2, [r3, #8]
3000b25c:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b260:	609a      	str	r2, [r3, #8]
3000b262:	e7de      	b.n	3000b222 <rxi316_DynSre_init+0xe>
3000b264:	41100000 	.word	0x41100000
3000b268:	ffff0000 	.word	0xffff0000

3000b26c <ddr_init>:
3000b26c:	b500      	push	{lr}
3000b26e:	b083      	sub	sp, #12
3000b270:	f7fe fe22 	bl	30009eb8 <ddr_init_index>
3000b274:	4b06      	ldr	r3, [pc, #24]	; (3000b290 <ddr_init+0x24>)
3000b276:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b27a:	9001      	str	r0, [sp, #4]
3000b27c:	f7fe fe46 	bl	30009f0c <rxi316_perf_tune>
3000b280:	9801      	ldr	r0, [sp, #4]
3000b282:	f7ff fa89 	bl	3000a798 <rxi316_dram_init>
3000b286:	b003      	add	sp, #12
3000b288:	f85d eb04 	ldr.w	lr, [sp], #4
3000b28c:	f7ff bfa2 	b.w	3000b1d4 <rxi316_ftend_init>
3000b290:	3000f44c 	.word	0x3000f44c

3000b294 <SWR_MEM>:
3000b294:	2801      	cmp	r0, #1
3000b296:	d00e      	beq.n	3000b2b6 <SWR_MEM+0x22>
3000b298:	4b31      	ldr	r3, [pc, #196]	; (3000b360 <SWR_MEM+0xcc>)
3000b29a:	2000      	movs	r0, #0
3000b29c:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b2a0:	f022 0201 	bic.w	r2, r2, #1
3000b2a4:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b2a8:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b2ac:	f022 0202 	bic.w	r2, r2, #2
3000b2b0:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b2b4:	4770      	bx	lr
3000b2b6:	b510      	push	{r4, lr}
3000b2b8:	4c29      	ldr	r4, [pc, #164]	; (3000b360 <SWR_MEM+0xcc>)
3000b2ba:	4a2a      	ldr	r2, [pc, #168]	; (3000b364 <SWR_MEM+0xd0>)
3000b2bc:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b2c0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b2c4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b2c8:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b2cc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b2d0:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b2d4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b2d8:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b2dc:	f023 0303 	bic.w	r3, r3, #3
3000b2e0:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b2e4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b2e8:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b2ec:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b2f0:	4790      	blx	r2
3000b2f2:	2803      	cmp	r0, #3
3000b2f4:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b2f8:	d026      	beq.n	3000b348 <SWR_MEM+0xb4>
3000b2fa:	f043 0303 	orr.w	r3, r3, #3
3000b2fe:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b302:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b306:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b30a:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b30e:	4c14      	ldr	r4, [pc, #80]	; (3000b360 <SWR_MEM+0xcc>)
3000b310:	4b15      	ldr	r3, [pc, #84]	; (3000b368 <SWR_MEM+0xd4>)
3000b312:	4798      	blx	r3
3000b314:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b318:	4a14      	ldr	r2, [pc, #80]	; (3000b36c <SWR_MEM+0xd8>)
3000b31a:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b31e:	f043 0302 	orr.w	r3, r3, #2
3000b322:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b326:	4790      	blx	r2
3000b328:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b32c:	4b10      	ldr	r3, [pc, #64]	; (3000b370 <SWR_MEM+0xdc>)
3000b32e:	f042 0201 	orr.w	r2, r2, #1
3000b332:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b336:	e001      	b.n	3000b33c <SWR_MEM+0xa8>
3000b338:	3b01      	subs	r3, #1
3000b33a:	d00e      	beq.n	3000b35a <SWR_MEM+0xc6>
3000b33c:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b340:	03d2      	lsls	r2, r2, #15
3000b342:	d5f9      	bpl.n	3000b338 <SWR_MEM+0xa4>
3000b344:	2000      	movs	r0, #0
3000b346:	bd10      	pop	{r4, pc}
3000b348:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b34c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b350:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b354:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b358:	e7d9      	b.n	3000b30e <SWR_MEM+0x7a>
3000b35a:	2001      	movs	r0, #1
3000b35c:	bd10      	pop	{r4, pc}
3000b35e:	bf00      	nop
3000b360:	42008800 	.word	0x42008800
3000b364:	30007da5 	.word	0x30007da5
3000b368:	3000b54d 	.word	0x3000b54d
3000b36c:	00009b2d 	.word	0x00009b2d
3000b370:	000186a0 	.word	0x000186a0

3000b374 <SWR_MEM_Manual>:
3000b374:	4a06      	ldr	r2, [pc, #24]	; (3000b390 <SWR_MEM_Manual+0x1c>)
3000b376:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b37a:	b120      	cbz	r0, 3000b386 <SWR_MEM_Manual+0x12>
3000b37c:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b380:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b384:	4770      	bx	lr
3000b386:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b38a:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b38e:	4770      	bx	lr
3000b390:	42008800 	.word	0x42008800

3000b394 <SWR_MEM_Mode_Set>:
3000b394:	2801      	cmp	r0, #1
3000b396:	b538      	push	{r3, r4, r5, lr}
3000b398:	d807      	bhi.n	3000b3aa <SWR_MEM_Mode_Set+0x16>
3000b39a:	d10a      	bne.n	3000b3b2 <SWR_MEM_Mode_Set+0x1e>
3000b39c:	4c22      	ldr	r4, [pc, #136]	; (3000b428 <SWR_MEM_Mode_Set+0x94>)
3000b39e:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b3a2:	035b      	lsls	r3, r3, #13
3000b3a4:	d51b      	bpl.n	3000b3de <SWR_MEM_Mode_Set+0x4a>
3000b3a6:	2000      	movs	r0, #0
3000b3a8:	bd38      	pop	{r3, r4, r5, pc}
3000b3aa:	217e      	movs	r1, #126	; 0x7e
3000b3ac:	481f      	ldr	r0, [pc, #124]	; (3000b42c <SWR_MEM_Mode_Set+0x98>)
3000b3ae:	f001 f82b 	bl	3000c408 <__io_assert_failed_veneer>
3000b3b2:	491d      	ldr	r1, [pc, #116]	; (3000b428 <SWR_MEM_Mode_Set+0x94>)
3000b3b4:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b3b8:	0358      	lsls	r0, r3, #13
3000b3ba:	d5f4      	bpl.n	3000b3a6 <SWR_MEM_Mode_Set+0x12>
3000b3bc:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b3c0:	f242 7310 	movw	r3, #10000	; 0x2710
3000b3c4:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b3c8:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b3cc:	e001      	b.n	3000b3d2 <SWR_MEM_Mode_Set+0x3e>
3000b3ce:	3b01      	subs	r3, #1
3000b3d0:	d027      	beq.n	3000b422 <SWR_MEM_Mode_Set+0x8e>
3000b3d2:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b3d6:	0352      	lsls	r2, r2, #13
3000b3d8:	d4f9      	bmi.n	3000b3ce <SWR_MEM_Mode_Set+0x3a>
3000b3da:	2000      	movs	r0, #0
3000b3dc:	e7e4      	b.n	3000b3a8 <SWR_MEM_Mode_Set+0x14>
3000b3de:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b3e2:	2064      	movs	r0, #100	; 0x64
3000b3e4:	4d12      	ldr	r5, [pc, #72]	; (3000b430 <SWR_MEM_Mode_Set+0x9c>)
3000b3e6:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b3ea:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b3ee:	47a8      	blx	r5
3000b3f0:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b3f4:	2064      	movs	r0, #100	; 0x64
3000b3f6:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b3fa:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b3fe:	47a8      	blx	r5
3000b400:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b404:	f242 7310 	movw	r3, #10000	; 0x2710
3000b408:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b40c:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b410:	e001      	b.n	3000b416 <SWR_MEM_Mode_Set+0x82>
3000b412:	3b01      	subs	r3, #1
3000b414:	d005      	beq.n	3000b422 <SWR_MEM_Mode_Set+0x8e>
3000b416:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b41a:	0355      	lsls	r5, r2, #13
3000b41c:	d5f9      	bpl.n	3000b412 <SWR_MEM_Mode_Set+0x7e>
3000b41e:	2000      	movs	r0, #0
3000b420:	e7c2      	b.n	3000b3a8 <SWR_MEM_Mode_Set+0x14>
3000b422:	2001      	movs	r0, #1
3000b424:	bd38      	pop	{r3, r4, r5, pc}
3000b426:	bf00      	nop
3000b428:	42008800 	.word	0x42008800
3000b42c:	3000d85c 	.word	0x3000d85c
3000b430:	00009b2d 	.word	0x00009b2d

3000b434 <SWR_AUDIO>:
3000b434:	2801      	cmp	r0, #1
3000b436:	d00e      	beq.n	3000b456 <SWR_AUDIO+0x22>
3000b438:	4b1e      	ldr	r3, [pc, #120]	; (3000b4b4 <SWR_AUDIO+0x80>)
3000b43a:	2000      	movs	r0, #0
3000b43c:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b440:	f022 0201 	bic.w	r2, r2, #1
3000b444:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b448:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b44c:	f022 0202 	bic.w	r2, r2, #2
3000b450:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b454:	4770      	bx	lr
3000b456:	b510      	push	{r4, lr}
3000b458:	4c16      	ldr	r4, [pc, #88]	; (3000b4b4 <SWR_AUDIO+0x80>)
3000b45a:	4b17      	ldr	r3, [pc, #92]	; (3000b4b8 <SWR_AUDIO+0x84>)
3000b45c:	4798      	blx	r3
3000b45e:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b462:	4a16      	ldr	r2, [pc, #88]	; (3000b4bc <SWR_AUDIO+0x88>)
3000b464:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b468:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b46c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b470:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b474:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b478:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b47c:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b480:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b484:	f043 0302 	orr.w	r3, r3, #2
3000b488:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b48c:	4790      	blx	r2
3000b48e:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b492:	4b0b      	ldr	r3, [pc, #44]	; (3000b4c0 <SWR_AUDIO+0x8c>)
3000b494:	f042 0201 	orr.w	r2, r2, #1
3000b498:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b49c:	e001      	b.n	3000b4a2 <SWR_AUDIO+0x6e>
3000b49e:	3b01      	subs	r3, #1
3000b4a0:	d005      	beq.n	3000b4ae <SWR_AUDIO+0x7a>
3000b4a2:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b4a6:	03d2      	lsls	r2, r2, #15
3000b4a8:	d5f9      	bpl.n	3000b49e <SWR_AUDIO+0x6a>
3000b4aa:	2000      	movs	r0, #0
3000b4ac:	bd10      	pop	{r4, pc}
3000b4ae:	2001      	movs	r0, #1
3000b4b0:	bd10      	pop	{r4, pc}
3000b4b2:	bf00      	nop
3000b4b4:	42008800 	.word	0x42008800
3000b4b8:	3000b6a9 	.word	0x3000b6a9
3000b4bc:	00009b2d 	.word	0x00009b2d
3000b4c0:	000186a0 	.word	0x000186a0

3000b4c4 <SWR_AUDIO_Manual>:
3000b4c4:	4a06      	ldr	r2, [pc, #24]	; (3000b4e0 <SWR_AUDIO_Manual+0x1c>)
3000b4c6:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b4ca:	b120      	cbz	r0, 3000b4d6 <SWR_AUDIO_Manual+0x12>
3000b4cc:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b4d0:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b4d4:	4770      	bx	lr
3000b4d6:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b4da:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b4de:	4770      	bx	lr
3000b4e0:	42008800 	.word	0x42008800

3000b4e4 <SWR_BST_MODE_Set>:
3000b4e4:	4a06      	ldr	r2, [pc, #24]	; (3000b500 <SWR_BST_MODE_Set+0x1c>)
3000b4e6:	6813      	ldr	r3, [r2, #0]
3000b4e8:	b118      	cbz	r0, 3000b4f2 <SWR_BST_MODE_Set+0xe>
3000b4ea:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b4ee:	6013      	str	r3, [r2, #0]
3000b4f0:	4770      	bx	lr
3000b4f2:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b4f6:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b4fa:	6013      	str	r3, [r2, #0]
3000b4fc:	4770      	bx	lr
3000b4fe:	bf00      	nop
3000b500:	42008100 	.word	0x42008100

3000b504 <SWR_Calib_DCore>:
3000b504:	4b0f      	ldr	r3, [pc, #60]	; (3000b544 <SWR_Calib_DCore+0x40>)
3000b506:	f240 3231 	movw	r2, #817	; 0x331
3000b50a:	490f      	ldr	r1, [pc, #60]	; (3000b548 <SWR_Calib_DCore+0x44>)
3000b50c:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b50e:	4001      	ands	r1, r0
3000b510:	430a      	orrs	r2, r1
3000b512:	651a      	str	r2, [r3, #80]	; 0x50
3000b514:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b516:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b51a:	641a      	str	r2, [r3, #64]	; 0x40
3000b51c:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b520:	f042 0201 	orr.w	r2, r2, #1
3000b524:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b528:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b52c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b530:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b534:	685a      	ldr	r2, [r3, #4]
3000b536:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b53a:	f042 0204 	orr.w	r2, r2, #4
3000b53e:	605a      	str	r2, [r3, #4]
3000b540:	4770      	bx	lr
3000b542:	bf00      	nop
3000b544:	42008100 	.word	0x42008100
3000b548:	fffffccc 	.word	0xfffffccc

3000b54c <SWR_Calib_MEM>:
3000b54c:	4b51      	ldr	r3, [pc, #324]	; (3000b694 <SWR_Calib_MEM+0x148>)
3000b54e:	f240 3231 	movw	r2, #817	; 0x331
3000b552:	4951      	ldr	r1, [pc, #324]	; (3000b698 <SWR_Calib_MEM+0x14c>)
3000b554:	f04f 0c00 	mov.w	ip, #0
3000b558:	b510      	push	{r4, lr}
3000b55a:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b55e:	b082      	sub	sp, #8
3000b560:	4001      	ands	r1, r0
3000b562:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b566:	430a      	orrs	r2, r1
3000b568:	f10d 0107 	add.w	r1, sp, #7
3000b56c:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b570:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b574:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b578:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b57c:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b580:	f042 0201 	orr.w	r2, r2, #1
3000b584:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b588:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b58c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b590:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b594:	f88d c007 	strb.w	ip, [sp, #7]
3000b598:	f7fd fa46 	bl	30008a28 <OTP_Read8>
3000b59c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b5a0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b5a4:	2af0      	cmp	r2, #240	; 0xf0
3000b5a6:	d156      	bne.n	3000b656 <SWR_Calib_MEM+0x10a>
3000b5a8:	f003 030f 	and.w	r3, r3, #15
3000b5ac:	2b0f      	cmp	r3, #15
3000b5ae:	d04e      	beq.n	3000b64e <SWR_Calib_MEM+0x102>
3000b5b0:	2302      	movs	r3, #2
3000b5b2:	4c3a      	ldr	r4, [pc, #232]	; (3000b69c <SWR_Calib_MEM+0x150>)
3000b5b4:	f10d 0106 	add.w	r1, sp, #6
3000b5b8:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b5bc:	7023      	strb	r3, [r4, #0]
3000b5be:	f7fd fa33 	bl	30008a28 <OTP_Read8>
3000b5c2:	7822      	ldrb	r2, [r4, #0]
3000b5c4:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b5c8:	2a03      	cmp	r2, #3
3000b5ca:	d05a      	beq.n	3000b682 <SWR_Calib_MEM+0x136>
3000b5cc:	2a02      	cmp	r2, #2
3000b5ce:	bf0c      	ite	eq
3000b5d0:	f003 030f 	andeq.w	r3, r3, #15
3000b5d4:	2300      	movne	r3, #0
3000b5d6:	492f      	ldr	r1, [pc, #188]	; (3000b694 <SWR_Calib_MEM+0x148>)
3000b5d8:	4831      	ldr	r0, [pc, #196]	; (3000b6a0 <SWR_Calib_MEM+0x154>)
3000b5da:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b5de:	f022 020f 	bic.w	r2, r2, #15
3000b5e2:	4313      	orrs	r3, r2
3000b5e4:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b5e8:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b5ea:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b5ee:	2b07      	cmp	r3, #7
3000b5f0:	d03a      	beq.n	3000b668 <SWR_Calib_MEM+0x11c>
3000b5f2:	2300      	movs	r3, #0
3000b5f4:	f10d 0107 	add.w	r1, sp, #7
3000b5f8:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b5fc:	f88d 3007 	strb.w	r3, [sp, #7]
3000b600:	f7fd fa12 	bl	30008a28 <OTP_Read8>
3000b604:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b608:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b60c:	2af0      	cmp	r2, #240	; 0xf0
3000b60e:	d124      	bne.n	3000b65a <SWR_Calib_MEM+0x10e>
3000b610:	f003 030f 	and.w	r3, r3, #15
3000b614:	2b0f      	cmp	r3, #15
3000b616:	d022      	beq.n	3000b65e <SWR_Calib_MEM+0x112>
3000b618:	2302      	movs	r3, #2
3000b61a:	4c22      	ldr	r4, [pc, #136]	; (3000b6a4 <SWR_Calib_MEM+0x158>)
3000b61c:	f10d 0107 	add.w	r1, sp, #7
3000b620:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b624:	7023      	strb	r3, [r4, #0]
3000b626:	f7fd f9ff 	bl	30008a28 <OTP_Read8>
3000b62a:	7822      	ldrb	r2, [r4, #0]
3000b62c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b630:	2a03      	cmp	r2, #3
3000b632:	d028      	beq.n	3000b686 <SWR_Calib_MEM+0x13a>
3000b634:	2a02      	cmp	r2, #2
3000b636:	d029      	beq.n	3000b68c <SWR_Calib_MEM+0x140>
3000b638:	2200      	movs	r2, #0
3000b63a:	4916      	ldr	r1, [pc, #88]	; (3000b694 <SWR_Calib_MEM+0x148>)
3000b63c:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b640:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b644:	4313      	orrs	r3, r2
3000b646:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b64a:	b002      	add	sp, #8
3000b64c:	bd10      	pop	{r4, pc}
3000b64e:	4b13      	ldr	r3, [pc, #76]	; (3000b69c <SWR_Calib_MEM+0x150>)
3000b650:	2201      	movs	r2, #1
3000b652:	701a      	strb	r2, [r3, #0]
3000b654:	e7cd      	b.n	3000b5f2 <SWR_Calib_MEM+0xa6>
3000b656:	2303      	movs	r3, #3
3000b658:	e7ab      	b.n	3000b5b2 <SWR_Calib_MEM+0x66>
3000b65a:	2303      	movs	r3, #3
3000b65c:	e7dd      	b.n	3000b61a <SWR_Calib_MEM+0xce>
3000b65e:	4b11      	ldr	r3, [pc, #68]	; (3000b6a4 <SWR_Calib_MEM+0x158>)
3000b660:	2201      	movs	r2, #1
3000b662:	701a      	strb	r2, [r3, #0]
3000b664:	b002      	add	sp, #8
3000b666:	bd10      	pop	{r4, pc}
3000b668:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b66a:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b66e:	d1c0      	bne.n	3000b5f2 <SWR_Calib_MEM+0xa6>
3000b670:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b674:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b678:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b67c:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b680:	e7b7      	b.n	3000b5f2 <SWR_Calib_MEM+0xa6>
3000b682:	091b      	lsrs	r3, r3, #4
3000b684:	e7a7      	b.n	3000b5d6 <SWR_Calib_MEM+0x8a>
3000b686:	091b      	lsrs	r3, r3, #4
3000b688:	059a      	lsls	r2, r3, #22
3000b68a:	e7d6      	b.n	3000b63a <SWR_Calib_MEM+0xee>
3000b68c:	f003 030f 	and.w	r3, r3, #15
3000b690:	059a      	lsls	r2, r3, #22
3000b692:	e7d2      	b.n	3000b63a <SWR_Calib_MEM+0xee>
3000b694:	42008800 	.word	0x42008800
3000b698:	fffffccc 	.word	0xfffffccc
3000b69c:	3000f5e7 	.word	0x3000f5e7
3000b6a0:	42008100 	.word	0x42008100
3000b6a4:	3000f5e6 	.word	0x3000f5e6

3000b6a8 <SWR_Calib_AUD>:
3000b6a8:	4b48      	ldr	r3, [pc, #288]	; (3000b7cc <SWR_Calib_AUD+0x124>)
3000b6aa:	f240 3231 	movw	r2, #817	; 0x331
3000b6ae:	4948      	ldr	r1, [pc, #288]	; (3000b7d0 <SWR_Calib_AUD+0x128>)
3000b6b0:	f04f 0c00 	mov.w	ip, #0
3000b6b4:	b510      	push	{r4, lr}
3000b6b6:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000b6ba:	b082      	sub	sp, #8
3000b6bc:	4001      	ands	r1, r0
3000b6be:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b6c2:	430a      	orrs	r2, r1
3000b6c4:	f10d 0107 	add.w	r1, sp, #7
3000b6c8:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b6cc:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b6d0:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b6d4:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b6d8:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000b6dc:	f042 0201 	orr.w	r2, r2, #1
3000b6e0:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000b6e4:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000b6e8:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b6ec:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000b6f0:	f88d c007 	strb.w	ip, [sp, #7]
3000b6f4:	f7fd f998 	bl	30008a28 <OTP_Read8>
3000b6f8:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b6fc:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b700:	2af0      	cmp	r2, #240	; 0xf0
3000b702:	d150      	bne.n	3000b7a6 <SWR_Calib_AUD+0xfe>
3000b704:	f003 030f 	and.w	r3, r3, #15
3000b708:	2b0f      	cmp	r3, #15
3000b70a:	d048      	beq.n	3000b79e <SWR_Calib_AUD+0xf6>
3000b70c:	2302      	movs	r3, #2
3000b70e:	4c31      	ldr	r4, [pc, #196]	; (3000b7d4 <SWR_Calib_AUD+0x12c>)
3000b710:	f10d 0106 	add.w	r1, sp, #6
3000b714:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b718:	7023      	strb	r3, [r4, #0]
3000b71a:	f7fd f985 	bl	30008a28 <OTP_Read8>
3000b71e:	7822      	ldrb	r2, [r4, #0]
3000b720:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b724:	2a03      	cmp	r2, #3
3000b726:	d047      	beq.n	3000b7b8 <SWR_Calib_AUD+0x110>
3000b728:	2a02      	cmp	r2, #2
3000b72a:	bf0c      	ite	eq
3000b72c:	f003 030f 	andeq.w	r3, r3, #15
3000b730:	2300      	movne	r3, #0
3000b732:	4926      	ldr	r1, [pc, #152]	; (3000b7cc <SWR_Calib_AUD+0x124>)
3000b734:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000b738:	f022 020f 	bic.w	r2, r2, #15
3000b73c:	4313      	orrs	r3, r2
3000b73e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b742:	2300      	movs	r3, #0
3000b744:	f10d 0107 	add.w	r1, sp, #7
3000b748:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b74c:	f88d 3007 	strb.w	r3, [sp, #7]
3000b750:	f7fd f96a 	bl	30008a28 <OTP_Read8>
3000b754:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b758:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b75c:	2af0      	cmp	r2, #240	; 0xf0
3000b75e:	d124      	bne.n	3000b7aa <SWR_Calib_AUD+0x102>
3000b760:	f003 030f 	and.w	r3, r3, #15
3000b764:	2b0f      	cmp	r3, #15
3000b766:	d022      	beq.n	3000b7ae <SWR_Calib_AUD+0x106>
3000b768:	2302      	movs	r3, #2
3000b76a:	4c1b      	ldr	r4, [pc, #108]	; (3000b7d8 <SWR_Calib_AUD+0x130>)
3000b76c:	f10d 0107 	add.w	r1, sp, #7
3000b770:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b774:	7023      	strb	r3, [r4, #0]
3000b776:	f7fd f957 	bl	30008a28 <OTP_Read8>
3000b77a:	7822      	ldrb	r2, [r4, #0]
3000b77c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b780:	2a03      	cmp	r2, #3
3000b782:	d01b      	beq.n	3000b7bc <SWR_Calib_AUD+0x114>
3000b784:	2a02      	cmp	r2, #2
3000b786:	d01c      	beq.n	3000b7c2 <SWR_Calib_AUD+0x11a>
3000b788:	2200      	movs	r2, #0
3000b78a:	4910      	ldr	r1, [pc, #64]	; (3000b7cc <SWR_Calib_AUD+0x124>)
3000b78c:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000b790:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b794:	4313      	orrs	r3, r2
3000b796:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b79a:	b002      	add	sp, #8
3000b79c:	bd10      	pop	{r4, pc}
3000b79e:	4b0d      	ldr	r3, [pc, #52]	; (3000b7d4 <SWR_Calib_AUD+0x12c>)
3000b7a0:	2201      	movs	r2, #1
3000b7a2:	701a      	strb	r2, [r3, #0]
3000b7a4:	e7cd      	b.n	3000b742 <SWR_Calib_AUD+0x9a>
3000b7a6:	2303      	movs	r3, #3
3000b7a8:	e7b1      	b.n	3000b70e <SWR_Calib_AUD+0x66>
3000b7aa:	2303      	movs	r3, #3
3000b7ac:	e7dd      	b.n	3000b76a <SWR_Calib_AUD+0xc2>
3000b7ae:	4b0a      	ldr	r3, [pc, #40]	; (3000b7d8 <SWR_Calib_AUD+0x130>)
3000b7b0:	2201      	movs	r2, #1
3000b7b2:	701a      	strb	r2, [r3, #0]
3000b7b4:	b002      	add	sp, #8
3000b7b6:	bd10      	pop	{r4, pc}
3000b7b8:	091b      	lsrs	r3, r3, #4
3000b7ba:	e7ba      	b.n	3000b732 <SWR_Calib_AUD+0x8a>
3000b7bc:	091b      	lsrs	r3, r3, #4
3000b7be:	059a      	lsls	r2, r3, #22
3000b7c0:	e7e3      	b.n	3000b78a <SWR_Calib_AUD+0xe2>
3000b7c2:	f003 030f 	and.w	r3, r3, #15
3000b7c6:	059a      	lsls	r2, r3, #22
3000b7c8:	e7df      	b.n	3000b78a <SWR_Calib_AUD+0xe2>
3000b7ca:	bf00      	nop
3000b7cc:	42008800 	.word	0x42008800
3000b7d0:	fffffccc 	.word	0xfffffccc
3000b7d4:	3000f5e5 	.word	0x3000f5e5
3000b7d8:	3000f5e4 	.word	0x3000f5e4

3000b7dc <crash_dump_memory>:
3000b7dc:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b7e0:	460e      	mov	r6, r1
3000b7e2:	4680      	mov	r8, r0
3000b7e4:	4b21      	ldr	r3, [pc, #132]	; (3000b86c <crash_dump_memory+0x90>)
3000b7e6:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000b7ea:	4a21      	ldr	r2, [pc, #132]	; (3000b870 <crash_dump_memory+0x94>)
3000b7ec:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000b7f0:	4644      	mov	r4, r8
3000b7f2:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000b7f6:	4b1f      	ldr	r3, [pc, #124]	; (3000b874 <crash_dump_memory+0x98>)
3000b7f8:	481f      	ldr	r0, [pc, #124]	; (3000b878 <crash_dump_memory+0x9c>)
3000b7fa:	bf88      	it	hi
3000b7fc:	2680      	movhi	r6, #128	; 0x80
3000b7fe:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000b802:	4f1e      	ldr	r7, [pc, #120]	; (3000b87c <crash_dump_memory+0xa0>)
3000b804:	bf14      	ite	ne
3000b806:	4611      	movne	r1, r2
3000b808:	4619      	moveq	r1, r3
3000b80a:	f000 fdbd 	bl	3000c388 <__DiagPrintf_veneer>
3000b80e:	4632      	mov	r2, r6
3000b810:	4641      	mov	r1, r8
3000b812:	481b      	ldr	r0, [pc, #108]	; (3000b880 <crash_dump_memory+0xa4>)
3000b814:	f000 fdb8 	bl	3000c388 <__DiagPrintf_veneer>
3000b818:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b888 <crash_dump_memory+0xac>
3000b81c:	2300      	movs	r3, #0
3000b81e:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000b88c <crash_dump_memory+0xb0>
3000b822:	e007      	b.n	3000b834 <crash_dump_memory+0x58>
3000b824:	6821      	ldr	r1, [r4, #0]
3000b826:	4638      	mov	r0, r7
3000b828:	f000 fdae 	bl	3000c388 <__DiagPrintf_veneer>
3000b82c:	42ae      	cmp	r6, r5
3000b82e:	d00f      	beq.n	3000b850 <crash_dump_memory+0x74>
3000b830:	3404      	adds	r4, #4
3000b832:	462b      	mov	r3, r5
3000b834:	1c5d      	adds	r5, r3, #1
3000b836:	b183      	cbz	r3, 3000b85a <crash_dump_memory+0x7e>
3000b838:	079b      	lsls	r3, r3, #30
3000b83a:	d1f3      	bne.n	3000b824 <crash_dump_memory+0x48>
3000b83c:	4621      	mov	r1, r4
3000b83e:	4648      	mov	r0, r9
3000b840:	f000 fda2 	bl	3000c388 <__DiagPrintf_veneer>
3000b844:	6821      	ldr	r1, [r4, #0]
3000b846:	4638      	mov	r0, r7
3000b848:	f000 fd9e 	bl	3000c388 <__DiagPrintf_veneer>
3000b84c:	42ae      	cmp	r6, r5
3000b84e:	d1ef      	bne.n	3000b830 <crash_dump_memory+0x54>
3000b850:	480c      	ldr	r0, [pc, #48]	; (3000b884 <crash_dump_memory+0xa8>)
3000b852:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b856:	f000 bd97 	b.w	3000c388 <__DiagPrintf_veneer>
3000b85a:	4641      	mov	r1, r8
3000b85c:	4650      	mov	r0, sl
3000b85e:	f000 fd93 	bl	3000c388 <__DiagPrintf_veneer>
3000b862:	6821      	ldr	r1, [r4, #0]
3000b864:	4638      	mov	r0, r7
3000b866:	f000 fd8f 	bl	3000c388 <__DiagPrintf_veneer>
3000b86a:	e7e1      	b.n	3000b830 <crash_dump_memory+0x54>
3000b86c:	42008000 	.word	0x42008000
3000b870:	3000d870 	.word	0x3000d870
3000b874:	3000d874 	.word	0x3000d874
3000b878:	3000d878 	.word	0x3000d878
3000b87c:	3000d8e0 	.word	0x3000d8e0
3000b880:	3000d89c 	.word	0x3000d89c
3000b884:	3000d050 	.word	0x3000d050
3000b888:	3000d8e8 	.word	0x3000d8e8
3000b88c:	3000d8d8 	.word	0x3000d8d8

3000b890 <crash_dump>:
3000b890:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b894:	4c4f      	ldr	r4, [pc, #316]	; (3000b9d4 <crash_dump+0x144>)
3000b896:	4615      	mov	r5, r2
3000b898:	4688      	mov	r8, r1
3000b89a:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000b89e:	484e      	ldr	r0, [pc, #312]	; (3000b9d8 <crash_dump+0x148>)
3000b8a0:	462f      	mov	r7, r5
3000b8a2:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000ba04 <crash_dump+0x174>
3000b8a6:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000b8aa:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000ba10 <crash_dump+0x180>
3000b8ae:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000b8b2:	e9c4 2100 	strd	r2, r1, [r4]
3000b8b6:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000b8b8:	60a2      	str	r2, [r4, #8]
3000b8ba:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000b8bc:	60e2      	str	r2, [r4, #12]
3000b8be:	686a      	ldr	r2, [r5, #4]
3000b8c0:	6122      	str	r2, [r4, #16]
3000b8c2:	68aa      	ldr	r2, [r5, #8]
3000b8c4:	6162      	str	r2, [r4, #20]
3000b8c6:	68ea      	ldr	r2, [r5, #12]
3000b8c8:	61a2      	str	r2, [r4, #24]
3000b8ca:	692a      	ldr	r2, [r5, #16]
3000b8cc:	61e2      	str	r2, [r4, #28]
3000b8ce:	696a      	ldr	r2, [r5, #20]
3000b8d0:	6222      	str	r2, [r4, #32]
3000b8d2:	69aa      	ldr	r2, [r5, #24]
3000b8d4:	6262      	str	r2, [r4, #36]	; 0x24
3000b8d6:	69ea      	ldr	r2, [r5, #28]
3000b8d8:	62a2      	str	r2, [r4, #40]	; 0x28
3000b8da:	6a2a      	ldr	r2, [r5, #32]
3000b8dc:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b8de:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b8e0:	6322      	str	r2, [r4, #48]	; 0x30
3000b8e2:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000b8e4:	6362      	str	r2, [r4, #52]	; 0x34
3000b8e6:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000b8e8:	63a2      	str	r2, [r4, #56]	; 0x38
3000b8ea:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000b8ec:	63e2      	str	r2, [r4, #60]	; 0x3c
3000b8ee:	682a      	ldr	r2, [r5, #0]
3000b8f0:	6422      	str	r2, [r4, #64]	; 0x40
3000b8f2:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000b8f6:	6462      	str	r2, [r4, #68]	; 0x44
3000b8f8:	4a38      	ldr	r2, [pc, #224]	; (3000b9dc <crash_dump+0x14c>)
3000b8fa:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000b8fe:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000b902:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000b906:	64e1      	str	r1, [r4, #76]	; 0x4c
3000b908:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000b90c:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000b910:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000b914:	6561      	str	r1, [r4, #84]	; 0x54
3000b916:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000b918:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000b91c:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000b920:	65e1      	str	r1, [r4, #92]	; 0x5c
3000b922:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000b924:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000b926:	b289      	uxth	r1, r1
3000b928:	b292      	uxth	r2, r2
3000b92a:	6621      	str	r1, [r4, #96]	; 0x60
3000b92c:	6662      	str	r2, [r4, #100]	; 0x64
3000b92e:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000b932:	66a2      	str	r2, [r4, #104]	; 0x68
3000b934:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000b938:	66e3      	str	r3, [r4, #108]	; 0x6c
3000b93a:	f000 fd25 	bl	3000c388 <__DiagPrintf_veneer>
3000b93e:	4620      	mov	r0, r4
3000b940:	462c      	mov	r4, r5
3000b942:	f000 f867 	bl	3000ba14 <fault_diagnosis>
3000b946:	4826      	ldr	r0, [pc, #152]	; (3000b9e0 <crash_dump+0x150>)
3000b948:	f000 fd1e 	bl	3000c388 <__DiagPrintf_veneer>
3000b94c:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000b94e:	4825      	ldr	r0, [pc, #148]	; (3000b9e4 <crash_dump+0x154>)
3000b950:	f000 fd1a 	bl	3000c388 <__DiagPrintf_veneer>
3000b954:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000b956:	4824      	ldr	r0, [pc, #144]	; (3000b9e8 <crash_dump+0x158>)
3000b958:	f000 fd16 	bl	3000c388 <__DiagPrintf_veneer>
3000b95c:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000b95e:	4823      	ldr	r0, [pc, #140]	; (3000b9ec <crash_dump+0x15c>)
3000b960:	f000 fd12 	bl	3000c388 <__DiagPrintf_veneer>
3000b964:	f857 1b20 	ldr.w	r1, [r7], #32
3000b968:	4821      	ldr	r0, [pc, #132]	; (3000b9f0 <crash_dump+0x160>)
3000b96a:	463e      	mov	r6, r7
3000b96c:	f000 fd0c 	bl	3000c388 <__DiagPrintf_veneer>
3000b970:	4920      	ldr	r1, [pc, #128]	; (3000b9f4 <crash_dump+0x164>)
3000b972:	4658      	mov	r0, fp
3000b974:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b978:	f000 fd06 	bl	3000c388 <__DiagPrintf_veneer>
3000b97c:	454e      	cmp	r6, r9
3000b97e:	d008      	beq.n	3000b992 <crash_dump+0x102>
3000b980:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000b984:	4658      	mov	r0, fp
3000b986:	f85a 1b04 	ldr.w	r1, [sl], #4
3000b98a:	f000 fcfd 	bl	3000c388 <__DiagPrintf_veneer>
3000b98e:	454e      	cmp	r6, r9
3000b990:	d1f6      	bne.n	3000b980 <crash_dump+0xf0>
3000b992:	4e19      	ldr	r6, [pc, #100]	; (3000b9f8 <crash_dump+0x168>)
3000b994:	4919      	ldr	r1, [pc, #100]	; (3000b9fc <crash_dump+0x16c>)
3000b996:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000ba04 <crash_dump+0x174>
3000b99a:	e001      	b.n	3000b9a0 <crash_dump+0x110>
3000b99c:	f856 1b04 	ldr.w	r1, [r6], #4
3000b9a0:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000b9a4:	4648      	mov	r0, r9
3000b9a6:	f000 fcef 	bl	3000c388 <__DiagPrintf_veneer>
3000b9aa:	42bc      	cmp	r4, r7
3000b9ac:	d1f6      	bne.n	3000b99c <crash_dump+0x10c>
3000b9ae:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b9b0:	4913      	ldr	r1, [pc, #76]	; (3000ba00 <crash_dump+0x170>)
3000b9b2:	4814      	ldr	r0, [pc, #80]	; (3000ba04 <crash_dump+0x174>)
3000b9b4:	f000 fce8 	bl	3000c388 <__DiagPrintf_veneer>
3000b9b8:	2180      	movs	r1, #128	; 0x80
3000b9ba:	4640      	mov	r0, r8
3000b9bc:	f7ff ff0e 	bl	3000b7dc <crash_dump_memory>
3000b9c0:	4811      	ldr	r0, [pc, #68]	; (3000ba08 <crash_dump+0x178>)
3000b9c2:	f000 fce1 	bl	3000c388 <__DiagPrintf_veneer>
3000b9c6:	4811      	ldr	r0, [pc, #68]	; (3000ba0c <crash_dump+0x17c>)
3000b9c8:	f000 fcde 	bl	3000c388 <__DiagPrintf_veneer>
3000b9cc:	2000      	movs	r0, #0
3000b9ce:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b9d2:	bf00      	nop
3000b9d4:	3000fbd8 	.word	0x3000fbd8
3000b9d8:	3000d8fc 	.word	0x3000d8fc
3000b9dc:	e000ed00 	.word	0xe000ed00
3000b9e0:	3000d920 	.word	0x3000d920
3000b9e4:	3000d948 	.word	0x3000d948
3000b9e8:	3000d958 	.word	0x3000d958
3000b9ec:	3000d968 	.word	0x3000d968
3000b9f0:	3000d978 	.word	0x3000d978
3000b9f4:	3000d8f4 	.word	0x3000d8f4
3000b9f8:	3000d9d8 	.word	0x3000d9d8
3000b9fc:	3000d8f8 	.word	0x3000d8f8
3000ba00:	3000d998 	.word	0x3000d998
3000ba04:	3000d988 	.word	0x3000d988
3000ba08:	3000d99c 	.word	0x3000d99c
3000ba0c:	3000d9c8 	.word	0x3000d9c8
3000ba10:	3000d9f8 	.word	0x3000d9f8

3000ba14 <fault_diagnosis>:
3000ba14:	b510      	push	{r4, lr}
3000ba16:	4604      	mov	r4, r0
3000ba18:	4888      	ldr	r0, [pc, #544]	; (3000bc3c <fault_diagnosis+0x228>)
3000ba1a:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000ba1c:	f000 fcb4 	bl	3000c388 <__DiagPrintf_veneer>
3000ba20:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000ba24:	079b      	lsls	r3, r3, #30
3000ba26:	f100 808a 	bmi.w	3000bb3e <fault_diagnosis+0x12a>
3000ba2a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000ba2c:	b1e1      	cbz	r1, 3000ba68 <fault_diagnosis+0x54>
3000ba2e:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000ba32:	07d8      	lsls	r0, r3, #31
3000ba34:	f100 8094 	bmi.w	3000bb60 <fault_diagnosis+0x14c>
3000ba38:	079a      	lsls	r2, r3, #30
3000ba3a:	f100 809a 	bmi.w	3000bb72 <fault_diagnosis+0x15e>
3000ba3e:	0758      	lsls	r0, r3, #29
3000ba40:	f100 80a1 	bmi.w	3000bb86 <fault_diagnosis+0x172>
3000ba44:	0719      	lsls	r1, r3, #28
3000ba46:	f100 80a8 	bmi.w	3000bb9a <fault_diagnosis+0x186>
3000ba4a:	06da      	lsls	r2, r3, #27
3000ba4c:	f100 80af 	bmi.w	3000bbae <fault_diagnosis+0x19a>
3000ba50:	0698      	lsls	r0, r3, #26
3000ba52:	f100 80b6 	bmi.w	3000bbc2 <fault_diagnosis+0x1ae>
3000ba56:	0619      	lsls	r1, r3, #24
3000ba58:	f100 80bd 	bmi.w	3000bbd6 <fault_diagnosis+0x1c2>
3000ba5c:	065a      	lsls	r2, r3, #25
3000ba5e:	d503      	bpl.n	3000ba68 <fault_diagnosis+0x54>
3000ba60:	f013 0f28 	tst.w	r3, #40	; 0x28
3000ba64:	f040 8186 	bne.w	3000bd74 <fault_diagnosis+0x360>
3000ba68:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000ba6c:	b199      	cbz	r1, 3000ba96 <fault_diagnosis+0x82>
3000ba6e:	07cb      	lsls	r3, r1, #31
3000ba70:	f100 80b9 	bmi.w	3000bbe6 <fault_diagnosis+0x1d2>
3000ba74:	0788      	lsls	r0, r1, #30
3000ba76:	f100 80bf 	bmi.w	3000bbf8 <fault_diagnosis+0x1e4>
3000ba7a:	070a      	lsls	r2, r1, #28
3000ba7c:	f100 80c5 	bmi.w	3000bc0a <fault_diagnosis+0x1f6>
3000ba80:	06cb      	lsls	r3, r1, #27
3000ba82:	f100 80cb 	bmi.w	3000bc1c <fault_diagnosis+0x208>
3000ba86:	0688      	lsls	r0, r1, #26
3000ba88:	f100 80d1 	bmi.w	3000bc2e <fault_diagnosis+0x21a>
3000ba8c:	060a      	lsls	r2, r1, #24
3000ba8e:	d502      	bpl.n	3000ba96 <fault_diagnosis+0x82>
3000ba90:	078b      	lsls	r3, r1, #30
3000ba92:	f040 8194 	bne.w	3000bdbe <fault_diagnosis+0x3aa>
3000ba96:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000ba9a:	b1b1      	cbz	r1, 3000baca <fault_diagnosis+0xb6>
3000ba9c:	07c8      	lsls	r0, r1, #31
3000ba9e:	f100 80f7 	bmi.w	3000bc90 <fault_diagnosis+0x27c>
3000baa2:	078a      	lsls	r2, r1, #30
3000baa4:	f100 80fd 	bmi.w	3000bca2 <fault_diagnosis+0x28e>
3000baa8:	074b      	lsls	r3, r1, #29
3000baaa:	f100 8103 	bmi.w	3000bcb4 <fault_diagnosis+0x2a0>
3000baae:	0708      	lsls	r0, r1, #28
3000bab0:	f100 8109 	bmi.w	3000bcc6 <fault_diagnosis+0x2b2>
3000bab4:	06ca      	lsls	r2, r1, #27
3000bab6:	f100 810f 	bmi.w	3000bcd8 <fault_diagnosis+0x2c4>
3000baba:	068b      	lsls	r3, r1, #26
3000babc:	f100 8115 	bmi.w	3000bcea <fault_diagnosis+0x2d6>
3000bac0:	0608      	lsls	r0, r1, #24
3000bac2:	d502      	bpl.n	3000baca <fault_diagnosis+0xb6>
3000bac4:	0789      	lsls	r1, r1, #30
3000bac6:	f100 8180 	bmi.w	3000bdca <fault_diagnosis+0x3b6>
3000baca:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bace:	b1b9      	cbz	r1, 3000bb00 <fault_diagnosis+0xec>
3000bad0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bad4:	07da      	lsls	r2, r3, #31
3000bad6:	f100 810f 	bmi.w	3000bcf8 <fault_diagnosis+0x2e4>
3000bada:	0798      	lsls	r0, r3, #30
3000badc:	f100 8115 	bmi.w	3000bd0a <fault_diagnosis+0x2f6>
3000bae0:	0759      	lsls	r1, r3, #29
3000bae2:	f100 811d 	bmi.w	3000bd20 <fault_diagnosis+0x30c>
3000bae6:	071a      	lsls	r2, r3, #28
3000bae8:	f100 8125 	bmi.w	3000bd36 <fault_diagnosis+0x322>
3000baec:	06db      	lsls	r3, r3, #27
3000baee:	f100 812d 	bmi.w	3000bd4c <fault_diagnosis+0x338>
3000baf2:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000baf6:	07d8      	lsls	r0, r3, #31
3000baf8:	f100 8133 	bmi.w	3000bd62 <fault_diagnosis+0x34e>
3000bafc:	0799      	lsls	r1, r3, #30
3000bafe:	d424      	bmi.n	3000bb4a <fault_diagnosis+0x136>
3000bb00:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bb04:	2b00      	cmp	r3, #0
3000bb06:	db00      	blt.n	3000bb0a <fault_diagnosis+0xf6>
3000bb08:	bd10      	pop	{r4, pc}
3000bb0a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bb0c:	2900      	cmp	r1, #0
3000bb0e:	d0fb      	beq.n	3000bb08 <fault_diagnosis+0xf4>
3000bb10:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bb14:	07da      	lsls	r2, r3, #31
3000bb16:	f100 8143 	bmi.w	3000bda0 <fault_diagnosis+0x38c>
3000bb1a:	0798      	lsls	r0, r3, #30
3000bb1c:	f100 8138 	bmi.w	3000bd90 <fault_diagnosis+0x37c>
3000bb20:	0759      	lsls	r1, r3, #29
3000bb22:	f100 812d 	bmi.w	3000bd80 <fault_diagnosis+0x36c>
3000bb26:	071a      	lsls	r2, r3, #28
3000bb28:	f100 8141 	bmi.w	3000bdae <fault_diagnosis+0x39a>
3000bb2c:	06db      	lsls	r3, r3, #27
3000bb2e:	d5eb      	bpl.n	3000bb08 <fault_diagnosis+0xf4>
3000bb30:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bb32:	4a43      	ldr	r2, [pc, #268]	; (3000bc40 <fault_diagnosis+0x22c>)
3000bb34:	4843      	ldr	r0, [pc, #268]	; (3000bc44 <fault_diagnosis+0x230>)
3000bb36:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000bb3a:	f000 bc25 	b.w	3000c388 <__DiagPrintf_veneer>
3000bb3e:	4a42      	ldr	r2, [pc, #264]	; (3000bc48 <fault_diagnosis+0x234>)
3000bb40:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000bb42:	4842      	ldr	r0, [pc, #264]	; (3000bc4c <fault_diagnosis+0x238>)
3000bb44:	f000 fc20 	bl	3000c388 <__DiagPrintf_veneer>
3000bb48:	e76f      	b.n	3000ba2a <fault_diagnosis+0x16>
3000bb4a:	4a41      	ldr	r2, [pc, #260]	; (3000bc50 <fault_diagnosis+0x23c>)
3000bb4c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bb50:	4840      	ldr	r0, [pc, #256]	; (3000bc54 <fault_diagnosis+0x240>)
3000bb52:	f000 fc19 	bl	3000c388 <__DiagPrintf_veneer>
3000bb56:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bb5a:	2b00      	cmp	r3, #0
3000bb5c:	dad4      	bge.n	3000bb08 <fault_diagnosis+0xf4>
3000bb5e:	e7d4      	b.n	3000bb0a <fault_diagnosis+0xf6>
3000bb60:	4a3d      	ldr	r2, [pc, #244]	; (3000bc58 <fault_diagnosis+0x244>)
3000bb62:	483e      	ldr	r0, [pc, #248]	; (3000bc5c <fault_diagnosis+0x248>)
3000bb64:	f000 fc10 	bl	3000c388 <__DiagPrintf_veneer>
3000bb68:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bb6c:	079a      	lsls	r2, r3, #30
3000bb6e:	f57f af66 	bpl.w	3000ba3e <fault_diagnosis+0x2a>
3000bb72:	4a3b      	ldr	r2, [pc, #236]	; (3000bc60 <fault_diagnosis+0x24c>)
3000bb74:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bb76:	4839      	ldr	r0, [pc, #228]	; (3000bc5c <fault_diagnosis+0x248>)
3000bb78:	f000 fc06 	bl	3000c388 <__DiagPrintf_veneer>
3000bb7c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bb80:	0758      	lsls	r0, r3, #29
3000bb82:	f57f af5f 	bpl.w	3000ba44 <fault_diagnosis+0x30>
3000bb86:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bb88:	4a36      	ldr	r2, [pc, #216]	; (3000bc64 <fault_diagnosis+0x250>)
3000bb8a:	4834      	ldr	r0, [pc, #208]	; (3000bc5c <fault_diagnosis+0x248>)
3000bb8c:	f000 fbfc 	bl	3000c388 <__DiagPrintf_veneer>
3000bb90:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bb94:	0719      	lsls	r1, r3, #28
3000bb96:	f57f af58 	bpl.w	3000ba4a <fault_diagnosis+0x36>
3000bb9a:	4a33      	ldr	r2, [pc, #204]	; (3000bc68 <fault_diagnosis+0x254>)
3000bb9c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bb9e:	482f      	ldr	r0, [pc, #188]	; (3000bc5c <fault_diagnosis+0x248>)
3000bba0:	f000 fbf2 	bl	3000c388 <__DiagPrintf_veneer>
3000bba4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bba8:	06da      	lsls	r2, r3, #27
3000bbaa:	f57f af51 	bpl.w	3000ba50 <fault_diagnosis+0x3c>
3000bbae:	4a2f      	ldr	r2, [pc, #188]	; (3000bc6c <fault_diagnosis+0x258>)
3000bbb0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bbb2:	482a      	ldr	r0, [pc, #168]	; (3000bc5c <fault_diagnosis+0x248>)
3000bbb4:	f000 fbe8 	bl	3000c388 <__DiagPrintf_veneer>
3000bbb8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bbbc:	0698      	lsls	r0, r3, #26
3000bbbe:	f57f af4a 	bpl.w	3000ba56 <fault_diagnosis+0x42>
3000bbc2:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bbc4:	4a2a      	ldr	r2, [pc, #168]	; (3000bc70 <fault_diagnosis+0x25c>)
3000bbc6:	4825      	ldr	r0, [pc, #148]	; (3000bc5c <fault_diagnosis+0x248>)
3000bbc8:	f000 fbde 	bl	3000c388 <__DiagPrintf_veneer>
3000bbcc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bbd0:	0619      	lsls	r1, r3, #24
3000bbd2:	f57f af43 	bpl.w	3000ba5c <fault_diagnosis+0x48>
3000bbd6:	4a27      	ldr	r2, [pc, #156]	; (3000bc74 <fault_diagnosis+0x260>)
3000bbd8:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bbda:	4820      	ldr	r0, [pc, #128]	; (3000bc5c <fault_diagnosis+0x248>)
3000bbdc:	f000 fbd4 	bl	3000c388 <__DiagPrintf_veneer>
3000bbe0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bbe4:	e73a      	b.n	3000ba5c <fault_diagnosis+0x48>
3000bbe6:	4a24      	ldr	r2, [pc, #144]	; (3000bc78 <fault_diagnosis+0x264>)
3000bbe8:	4824      	ldr	r0, [pc, #144]	; (3000bc7c <fault_diagnosis+0x268>)
3000bbea:	f000 fbcd 	bl	3000c388 <__DiagPrintf_veneer>
3000bbee:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bbf2:	0788      	lsls	r0, r1, #30
3000bbf4:	f57f af41 	bpl.w	3000ba7a <fault_diagnosis+0x66>
3000bbf8:	4a21      	ldr	r2, [pc, #132]	; (3000bc80 <fault_diagnosis+0x26c>)
3000bbfa:	4820      	ldr	r0, [pc, #128]	; (3000bc7c <fault_diagnosis+0x268>)
3000bbfc:	f000 fbc4 	bl	3000c388 <__DiagPrintf_veneer>
3000bc00:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bc04:	070a      	lsls	r2, r1, #28
3000bc06:	f57f af3b 	bpl.w	3000ba80 <fault_diagnosis+0x6c>
3000bc0a:	4a1e      	ldr	r2, [pc, #120]	; (3000bc84 <fault_diagnosis+0x270>)
3000bc0c:	481b      	ldr	r0, [pc, #108]	; (3000bc7c <fault_diagnosis+0x268>)
3000bc0e:	f000 fbbb 	bl	3000c388 <__DiagPrintf_veneer>
3000bc12:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bc16:	06cb      	lsls	r3, r1, #27
3000bc18:	f57f af35 	bpl.w	3000ba86 <fault_diagnosis+0x72>
3000bc1c:	4a1a      	ldr	r2, [pc, #104]	; (3000bc88 <fault_diagnosis+0x274>)
3000bc1e:	4817      	ldr	r0, [pc, #92]	; (3000bc7c <fault_diagnosis+0x268>)
3000bc20:	f000 fbb2 	bl	3000c388 <__DiagPrintf_veneer>
3000bc24:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bc28:	0688      	lsls	r0, r1, #26
3000bc2a:	f57f af2f 	bpl.w	3000ba8c <fault_diagnosis+0x78>
3000bc2e:	4a17      	ldr	r2, [pc, #92]	; (3000bc8c <fault_diagnosis+0x278>)
3000bc30:	4812      	ldr	r0, [pc, #72]	; (3000bc7c <fault_diagnosis+0x268>)
3000bc32:	f000 fba9 	bl	3000c388 <__DiagPrintf_veneer>
3000bc36:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bc3a:	e727      	b.n	3000ba8c <fault_diagnosis+0x78>
3000bc3c:	3000da08 	.word	0x3000da08
3000bc40:	3000e298 	.word	0x3000e298
3000bc44:	3000e1f0 	.word	0x3000e1f0
3000bc48:	3000da20 	.word	0x3000da20
3000bc4c:	3000da4c 	.word	0x3000da4c
3000bc50:	3000e154 	.word	0x3000e154
3000bc54:	3000df98 	.word	0x3000df98
3000bc58:	3000da64 	.word	0x3000da64
3000bc5c:	3000da94 	.word	0x3000da94
3000bc60:	3000daac 	.word	0x3000daac
3000bc64:	3000dae4 	.word	0x3000dae4
3000bc68:	3000db1c 	.word	0x3000db1c
3000bc6c:	3000db54 	.word	0x3000db54
3000bc70:	3000db84 	.word	0x3000db84
3000bc74:	3000dbc0 	.word	0x3000dbc0
3000bc78:	3000dc30 	.word	0x3000dc30
3000bc7c:	3000dc74 	.word	0x3000dc74
3000bc80:	3000dc8c 	.word	0x3000dc8c
3000bc84:	3000dcc8 	.word	0x3000dcc8
3000bc88:	3000dd00 	.word	0x3000dd00
3000bc8c:	3000dd34 	.word	0x3000dd34
3000bc90:	4a51      	ldr	r2, [pc, #324]	; (3000bdd8 <fault_diagnosis+0x3c4>)
3000bc92:	4852      	ldr	r0, [pc, #328]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bc94:	f000 fb78 	bl	3000c388 <__DiagPrintf_veneer>
3000bc98:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bc9c:	078a      	lsls	r2, r1, #30
3000bc9e:	f57f af03 	bpl.w	3000baa8 <fault_diagnosis+0x94>
3000bca2:	4a4f      	ldr	r2, [pc, #316]	; (3000bde0 <fault_diagnosis+0x3cc>)
3000bca4:	484d      	ldr	r0, [pc, #308]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bca6:	f000 fb6f 	bl	3000c388 <__DiagPrintf_veneer>
3000bcaa:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bcae:	074b      	lsls	r3, r1, #29
3000bcb0:	f57f aefd 	bpl.w	3000baae <fault_diagnosis+0x9a>
3000bcb4:	4a4b      	ldr	r2, [pc, #300]	; (3000bde4 <fault_diagnosis+0x3d0>)
3000bcb6:	4849      	ldr	r0, [pc, #292]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bcb8:	f000 fb66 	bl	3000c388 <__DiagPrintf_veneer>
3000bcbc:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bcc0:	0708      	lsls	r0, r1, #28
3000bcc2:	f57f aef7 	bpl.w	3000bab4 <fault_diagnosis+0xa0>
3000bcc6:	4a48      	ldr	r2, [pc, #288]	; (3000bde8 <fault_diagnosis+0x3d4>)
3000bcc8:	4844      	ldr	r0, [pc, #272]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bcca:	f000 fb5d 	bl	3000c388 <__DiagPrintf_veneer>
3000bcce:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bcd2:	06ca      	lsls	r2, r1, #27
3000bcd4:	f57f aef1 	bpl.w	3000baba <fault_diagnosis+0xa6>
3000bcd8:	4a44      	ldr	r2, [pc, #272]	; (3000bdec <fault_diagnosis+0x3d8>)
3000bcda:	4840      	ldr	r0, [pc, #256]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bcdc:	f000 fb54 	bl	3000c388 <__DiagPrintf_veneer>
3000bce0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bce4:	068b      	lsls	r3, r1, #26
3000bce6:	f57f aeeb 	bpl.w	3000bac0 <fault_diagnosis+0xac>
3000bcea:	4a41      	ldr	r2, [pc, #260]	; (3000bdf0 <fault_diagnosis+0x3dc>)
3000bcec:	483b      	ldr	r0, [pc, #236]	; (3000bddc <fault_diagnosis+0x3c8>)
3000bcee:	f000 fb4b 	bl	3000c388 <__DiagPrintf_veneer>
3000bcf2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bcf6:	e6e3      	b.n	3000bac0 <fault_diagnosis+0xac>
3000bcf8:	4a3e      	ldr	r2, [pc, #248]	; (3000bdf4 <fault_diagnosis+0x3e0>)
3000bcfa:	483f      	ldr	r0, [pc, #252]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bcfc:	f000 fb44 	bl	3000c388 <__DiagPrintf_veneer>
3000bd00:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bd04:	0798      	lsls	r0, r3, #30
3000bd06:	f57f aeeb 	bpl.w	3000bae0 <fault_diagnosis+0xcc>
3000bd0a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bd0e:	4a3b      	ldr	r2, [pc, #236]	; (3000bdfc <fault_diagnosis+0x3e8>)
3000bd10:	4839      	ldr	r0, [pc, #228]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bd12:	f000 fb39 	bl	3000c388 <__DiagPrintf_veneer>
3000bd16:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bd1a:	0759      	lsls	r1, r3, #29
3000bd1c:	f57f aee3 	bpl.w	3000bae6 <fault_diagnosis+0xd2>
3000bd20:	4a37      	ldr	r2, [pc, #220]	; (3000be00 <fault_diagnosis+0x3ec>)
3000bd22:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bd26:	4834      	ldr	r0, [pc, #208]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bd28:	f000 fb2e 	bl	3000c388 <__DiagPrintf_veneer>
3000bd2c:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bd30:	071a      	lsls	r2, r3, #28
3000bd32:	f57f aedb 	bpl.w	3000baec <fault_diagnosis+0xd8>
3000bd36:	4a33      	ldr	r2, [pc, #204]	; (3000be04 <fault_diagnosis+0x3f0>)
3000bd38:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bd3c:	482e      	ldr	r0, [pc, #184]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bd3e:	f000 fb23 	bl	3000c388 <__DiagPrintf_veneer>
3000bd42:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bd46:	06db      	lsls	r3, r3, #27
3000bd48:	f57f aed3 	bpl.w	3000baf2 <fault_diagnosis+0xde>
3000bd4c:	4a2e      	ldr	r2, [pc, #184]	; (3000be08 <fault_diagnosis+0x3f4>)
3000bd4e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bd52:	4829      	ldr	r0, [pc, #164]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bd54:	f000 fb18 	bl	3000c388 <__DiagPrintf_veneer>
3000bd58:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bd5c:	07d8      	lsls	r0, r3, #31
3000bd5e:	f57f aecd 	bpl.w	3000bafc <fault_diagnosis+0xe8>
3000bd62:	4a2a      	ldr	r2, [pc, #168]	; (3000be0c <fault_diagnosis+0x3f8>)
3000bd64:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bd68:	4823      	ldr	r0, [pc, #140]	; (3000bdf8 <fault_diagnosis+0x3e4>)
3000bd6a:	f000 fb0d 	bl	3000c388 <__DiagPrintf_veneer>
3000bd6e:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bd72:	e6c3      	b.n	3000bafc <fault_diagnosis+0xe8>
3000bd74:	4a26      	ldr	r2, [pc, #152]	; (3000be10 <fault_diagnosis+0x3fc>)
3000bd76:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000bd78:	4826      	ldr	r0, [pc, #152]	; (3000be14 <fault_diagnosis+0x400>)
3000bd7a:	f000 fb05 	bl	3000c388 <__DiagPrintf_veneer>
3000bd7e:	e673      	b.n	3000ba68 <fault_diagnosis+0x54>
3000bd80:	4a25      	ldr	r2, [pc, #148]	; (3000be18 <fault_diagnosis+0x404>)
3000bd82:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bd84:	4825      	ldr	r0, [pc, #148]	; (3000be1c <fault_diagnosis+0x408>)
3000bd86:	f000 faff 	bl	3000c388 <__DiagPrintf_veneer>
3000bd8a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bd8e:	e6ca      	b.n	3000bb26 <fault_diagnosis+0x112>
3000bd90:	4a23      	ldr	r2, [pc, #140]	; (3000be20 <fault_diagnosis+0x40c>)
3000bd92:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bd94:	4821      	ldr	r0, [pc, #132]	; (3000be1c <fault_diagnosis+0x408>)
3000bd96:	f000 faf7 	bl	3000c388 <__DiagPrintf_veneer>
3000bd9a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bd9e:	e6bf      	b.n	3000bb20 <fault_diagnosis+0x10c>
3000bda0:	4a20      	ldr	r2, [pc, #128]	; (3000be24 <fault_diagnosis+0x410>)
3000bda2:	481e      	ldr	r0, [pc, #120]	; (3000be1c <fault_diagnosis+0x408>)
3000bda4:	f000 faf0 	bl	3000c388 <__DiagPrintf_veneer>
3000bda8:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bdac:	e6b5      	b.n	3000bb1a <fault_diagnosis+0x106>
3000bdae:	4a1e      	ldr	r2, [pc, #120]	; (3000be28 <fault_diagnosis+0x414>)
3000bdb0:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bdb2:	481a      	ldr	r0, [pc, #104]	; (3000be1c <fault_diagnosis+0x408>)
3000bdb4:	f000 fae8 	bl	3000c388 <__DiagPrintf_veneer>
3000bdb8:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bdbc:	e6b6      	b.n	3000bb2c <fault_diagnosis+0x118>
3000bdbe:	4a1b      	ldr	r2, [pc, #108]	; (3000be2c <fault_diagnosis+0x418>)
3000bdc0:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000bdc2:	481b      	ldr	r0, [pc, #108]	; (3000be30 <fault_diagnosis+0x41c>)
3000bdc4:	f000 fae0 	bl	3000c388 <__DiagPrintf_veneer>
3000bdc8:	e665      	b.n	3000ba96 <fault_diagnosis+0x82>
3000bdca:	4a1a      	ldr	r2, [pc, #104]	; (3000be34 <fault_diagnosis+0x420>)
3000bdcc:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000bdce:	481a      	ldr	r0, [pc, #104]	; (3000be38 <fault_diagnosis+0x424>)
3000bdd0:	f000 fada 	bl	3000c388 <__DiagPrintf_veneer>
3000bdd4:	e679      	b.n	3000baca <fault_diagnosis+0xb6>
3000bdd6:	bf00      	nop
3000bdd8:	3000ddc8 	.word	0x3000ddc8
3000bddc:	3000ddfc 	.word	0x3000ddfc
3000bde0:	3000de14 	.word	0x3000de14
3000bde4:	3000de4c 	.word	0x3000de4c
3000bde8:	3000de84 	.word	0x3000de84
3000bdec:	3000deac 	.word	0x3000deac
3000bdf0:	3000ded4 	.word	0x3000ded4
3000bdf4:	3000df50 	.word	0x3000df50
3000bdf8:	3000df98 	.word	0x3000df98
3000bdfc:	3000dfb0 	.word	0x3000dfb0
3000be00:	3000dffc 	.word	0x3000dffc
3000be04:	3000e05c 	.word	0x3000e05c
3000be08:	3000e0a4 	.word	0x3000e0a4
3000be0c:	3000e100 	.word	0x3000e100
3000be10:	3000dbf0 	.word	0x3000dbf0
3000be14:	3000dc18 	.word	0x3000dc18
3000be18:	3000e23c 	.word	0x3000e23c
3000be1c:	3000e1f0 	.word	0x3000e1f0
3000be20:	3000e208 	.word	0x3000e208
3000be24:	3000e1c0 	.word	0x3000e1c0
3000be28:	3000e268 	.word	0x3000e268
3000be2c:	3000dd80 	.word	0x3000dd80
3000be30:	3000ddb0 	.word	0x3000ddb0
3000be34:	3000df14 	.word	0x3000df14
3000be38:	3000df38 	.word	0x3000df38

3000be3c <rtk_log_write>:
3000be3c:	b40c      	push	{r2, r3}
3000be3e:	4b18      	ldr	r3, [pc, #96]	; (3000bea0 <rtk_log_write+0x64>)
3000be40:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000be44:	681a      	ldr	r2, [r3, #0]
3000be46:	b083      	sub	sp, #12
3000be48:	4680      	mov	r8, r0
3000be4a:	460e      	mov	r6, r1
3000be4c:	2a02      	cmp	r2, #2
3000be4e:	d825      	bhi.n	3000be9c <rtk_log_write+0x60>
3000be50:	681f      	ldr	r7, [r3, #0]
3000be52:	b1a7      	cbz	r7, 3000be7e <rtk_log_write+0x42>
3000be54:	4d13      	ldr	r5, [pc, #76]	; (3000bea4 <rtk_log_write+0x68>)
3000be56:	2400      	movs	r4, #0
3000be58:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000beb0 <rtk_log_write+0x74>
3000be5c:	e002      	b.n	3000be64 <rtk_log_write+0x28>
3000be5e:	3401      	adds	r4, #1
3000be60:	42a7      	cmp	r7, r4
3000be62:	d90c      	bls.n	3000be7e <rtk_log_write+0x42>
3000be64:	4628      	mov	r0, r5
3000be66:	4631      	mov	r1, r6
3000be68:	350b      	adds	r5, #11
3000be6a:	47c8      	blx	r9
3000be6c:	2800      	cmp	r0, #0
3000be6e:	d1f6      	bne.n	3000be5e <rtk_log_write+0x22>
3000be70:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000be74:	4b0c      	ldr	r3, [pc, #48]	; (3000bea8 <rtk_log_write+0x6c>)
3000be76:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000be7a:	5d1b      	ldrb	r3, [r3, r4]
3000be7c:	e001      	b.n	3000be82 <rtk_log_write+0x46>
3000be7e:	4b0b      	ldr	r3, [pc, #44]	; (3000beac <rtk_log_write+0x70>)
3000be80:	781b      	ldrb	r3, [r3, #0]
3000be82:	4598      	cmp	r8, r3
3000be84:	d805      	bhi.n	3000be92 <rtk_log_write+0x56>
3000be86:	aa0b      	add	r2, sp, #44	; 0x2c
3000be88:	990a      	ldr	r1, [sp, #40]	; 0x28
3000be8a:	2000      	movs	r0, #0
3000be8c:	9201      	str	r2, [sp, #4]
3000be8e:	f000 fa63 	bl	3000c358 <__DiagVSprintf_veneer>
3000be92:	b003      	add	sp, #12
3000be94:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000be98:	b002      	add	sp, #8
3000be9a:	4770      	bx	lr
3000be9c:	2703      	movs	r7, #3
3000be9e:	e7d9      	b.n	3000be54 <rtk_log_write+0x18>
3000bea0:	30010018 	.word	0x30010018
3000bea4:	3001001d 	.word	0x3001001d
3000bea8:	3001001c 	.word	0x3001001c
3000beac:	3000f5f8 	.word	0x3000f5f8
3000beb0:	00012c89 	.word	0x00012c89

3000beb4 <BOOT_VerCheck>:
3000beb4:	b570      	push	{r4, r5, r6, lr}
3000beb6:	4b1d      	ldr	r3, [pc, #116]	; (3000bf2c <BOOT_VerCheck+0x78>)
3000beb8:	4798      	blx	r3
3000beba:	2800      	cmp	r0, #0
3000bebc:	d135      	bne.n	3000bf2a <BOOT_VerCheck+0x76>
3000bebe:	4b1c      	ldr	r3, [pc, #112]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bec0:	2403      	movs	r4, #3
3000bec2:	4d1c      	ldr	r5, [pc, #112]	; (3000bf34 <BOOT_VerCheck+0x80>)
3000bec4:	2003      	movs	r0, #3
3000bec6:	4a1c      	ldr	r2, [pc, #112]	; (3000bf38 <BOOT_VerCheck+0x84>)
3000bec8:	4619      	mov	r1, r3
3000beca:	f7ff ffb7 	bl	3000be3c <rtk_log_write>
3000bece:	4b18      	ldr	r3, [pc, #96]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bed0:	462a      	mov	r2, r5
3000bed2:	2003      	movs	r0, #3
3000bed4:	4619      	mov	r1, r3
3000bed6:	f7ff ffb1 	bl	3000be3c <rtk_log_write>
3000beda:	4b15      	ldr	r3, [pc, #84]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bedc:	4a17      	ldr	r2, [pc, #92]	; (3000bf3c <BOOT_VerCheck+0x88>)
3000bede:	2003      	movs	r0, #3
3000bee0:	4619      	mov	r1, r3
3000bee2:	f7ff ffab 	bl	3000be3c <rtk_log_write>
3000bee6:	4b12      	ldr	r3, [pc, #72]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bee8:	4a15      	ldr	r2, [pc, #84]	; (3000bf40 <BOOT_VerCheck+0x8c>)
3000beea:	2003      	movs	r0, #3
3000beec:	4619      	mov	r1, r3
3000beee:	f7ff ffa5 	bl	3000be3c <rtk_log_write>
3000bef2:	4b0f      	ldr	r3, [pc, #60]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bef4:	4a13      	ldr	r2, [pc, #76]	; (3000bf44 <BOOT_VerCheck+0x90>)
3000bef6:	2003      	movs	r0, #3
3000bef8:	4619      	mov	r1, r3
3000befa:	f7ff ff9f 	bl	3000be3c <rtk_log_write>
3000befe:	4b0c      	ldr	r3, [pc, #48]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bf00:	462a      	mov	r2, r5
3000bf02:	2003      	movs	r0, #3
3000bf04:	4619      	mov	r1, r3
3000bf06:	f7ff ff99 	bl	3000be3c <rtk_log_write>
3000bf0a:	4b09      	ldr	r3, [pc, #36]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bf0c:	4a0e      	ldr	r2, [pc, #56]	; (3000bf48 <BOOT_VerCheck+0x94>)
3000bf0e:	2003      	movs	r0, #3
3000bf10:	4619      	mov	r1, r3
3000bf12:	f7ff ff93 	bl	3000be3c <rtk_log_write>
3000bf16:	3c01      	subs	r4, #1
3000bf18:	d1d9      	bne.n	3000bece <BOOT_VerCheck+0x1a>
3000bf1a:	4b05      	ldr	r3, [pc, #20]	; (3000bf30 <BOOT_VerCheck+0x7c>)
3000bf1c:	2003      	movs	r0, #3
3000bf1e:	4a0b      	ldr	r2, [pc, #44]	; (3000bf4c <BOOT_VerCheck+0x98>)
3000bf20:	4619      	mov	r1, r3
3000bf22:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000bf26:	f7ff bf89 	b.w	3000be3c <rtk_log_write>
3000bf2a:	bd70      	pop	{r4, r5, r6, pc}
3000bf2c:	30009ba9 	.word	0x30009ba9
3000bf30:	3000e36a 	.word	0x3000e36a
3000bf34:	3000e3b2 	.word	0x3000e3b2
3000bf38:	3000e36f 	.word	0x3000e36f
3000bf3c:	3000e3f5 	.word	0x3000e3f5
3000bf40:	3000e438 	.word	0x3000e438
3000bf44:	3000e47b 	.word	0x3000e47b
3000bf48:	3000e4be 	.word	0x3000e4be
3000bf4c:	3000e501 	.word	0x3000e501

3000bf50 <__aeabi_uldivmod>:
3000bf50:	b953      	cbnz	r3, 3000bf68 <__aeabi_uldivmod+0x18>
3000bf52:	b94a      	cbnz	r2, 3000bf68 <__aeabi_uldivmod+0x18>
3000bf54:	2900      	cmp	r1, #0
3000bf56:	bf08      	it	eq
3000bf58:	2800      	cmpeq	r0, #0
3000bf5a:	bf1c      	itt	ne
3000bf5c:	f04f 31ff 	movne.w	r1, #4294967295
3000bf60:	f04f 30ff 	movne.w	r0, #4294967295
3000bf64:	f000 b960 	b.w	3000c228 <__aeabi_idiv0>
3000bf68:	f1ad 0c08 	sub.w	ip, sp, #8
3000bf6c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000bf70:	f000 f806 	bl	3000bf80 <__udivmoddi4>
3000bf74:	f8dd e004 	ldr.w	lr, [sp, #4]
3000bf78:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000bf7c:	b004      	add	sp, #16
3000bf7e:	4770      	bx	lr

3000bf80 <__udivmoddi4>:
3000bf80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000bf84:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000bf86:	4688      	mov	r8, r1
3000bf88:	468e      	mov	lr, r1
3000bf8a:	4615      	mov	r5, r2
3000bf8c:	4604      	mov	r4, r0
3000bf8e:	4619      	mov	r1, r3
3000bf90:	2b00      	cmp	r3, #0
3000bf92:	f040 80c6 	bne.w	3000c122 <__udivmoddi4+0x1a2>
3000bf96:	4542      	cmp	r2, r8
3000bf98:	fab2 f782 	clz	r7, r2
3000bf9c:	d946      	bls.n	3000c02c <__udivmoddi4+0xac>
3000bf9e:	b14f      	cbz	r7, 3000bfb4 <__udivmoddi4+0x34>
3000bfa0:	f1c7 0c20 	rsb	ip, r7, #32
3000bfa4:	fa08 f307 	lsl.w	r3, r8, r7
3000bfa8:	40bd      	lsls	r5, r7
3000bfaa:	40bc      	lsls	r4, r7
3000bfac:	fa20 fc0c 	lsr.w	ip, r0, ip
3000bfb0:	ea4c 0e03 	orr.w	lr, ip, r3
3000bfb4:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000bfb8:	fa1f fc85 	uxth.w	ip, r5
3000bfbc:	0c22      	lsrs	r2, r4, #16
3000bfbe:	fbbe f9f8 	udiv	r9, lr, r8
3000bfc2:	fb08 e319 	mls	r3, r8, r9, lr
3000bfc6:	fb09 fa0c 	mul.w	sl, r9, ip
3000bfca:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000bfce:	459a      	cmp	sl, r3
3000bfd0:	d928      	bls.n	3000c024 <__udivmoddi4+0xa4>
3000bfd2:	18eb      	adds	r3, r5, r3
3000bfd4:	f109 30ff 	add.w	r0, r9, #4294967295
3000bfd8:	d204      	bcs.n	3000bfe4 <__udivmoddi4+0x64>
3000bfda:	459a      	cmp	sl, r3
3000bfdc:	d902      	bls.n	3000bfe4 <__udivmoddi4+0x64>
3000bfde:	f1a9 0002 	sub.w	r0, r9, #2
3000bfe2:	442b      	add	r3, r5
3000bfe4:	eba3 030a 	sub.w	r3, r3, sl
3000bfe8:	b2a4      	uxth	r4, r4
3000bfea:	fbb3 f2f8 	udiv	r2, r3, r8
3000bfee:	fb08 3312 	mls	r3, r8, r2, r3
3000bff2:	fb02 fc0c 	mul.w	ip, r2, ip
3000bff6:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000bffa:	45a4      	cmp	ip, r4
3000bffc:	d914      	bls.n	3000c028 <__udivmoddi4+0xa8>
3000bffe:	192c      	adds	r4, r5, r4
3000c000:	f102 33ff 	add.w	r3, r2, #4294967295
3000c004:	d203      	bcs.n	3000c00e <__udivmoddi4+0x8e>
3000c006:	45a4      	cmp	ip, r4
3000c008:	d901      	bls.n	3000c00e <__udivmoddi4+0x8e>
3000c00a:	1e93      	subs	r3, r2, #2
3000c00c:	442c      	add	r4, r5
3000c00e:	eba4 040c 	sub.w	r4, r4, ip
3000c012:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c016:	b11e      	cbz	r6, 3000c020 <__udivmoddi4+0xa0>
3000c018:	40fc      	lsrs	r4, r7
3000c01a:	2300      	movs	r3, #0
3000c01c:	6034      	str	r4, [r6, #0]
3000c01e:	6073      	str	r3, [r6, #4]
3000c020:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c024:	4648      	mov	r0, r9
3000c026:	e7dd      	b.n	3000bfe4 <__udivmoddi4+0x64>
3000c028:	4613      	mov	r3, r2
3000c02a:	e7f0      	b.n	3000c00e <__udivmoddi4+0x8e>
3000c02c:	b902      	cbnz	r2, 3000c030 <__udivmoddi4+0xb0>
3000c02e:	deff      	udf	#255	; 0xff
3000c030:	bb8f      	cbnz	r7, 3000c096 <__udivmoddi4+0x116>
3000c032:	eba8 0302 	sub.w	r3, r8, r2
3000c036:	2101      	movs	r1, #1
3000c038:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c03c:	b2aa      	uxth	r2, r5
3000c03e:	0c20      	lsrs	r0, r4, #16
3000c040:	fbb3 fcfe 	udiv	ip, r3, lr
3000c044:	fb0e 331c 	mls	r3, lr, ip, r3
3000c048:	fb0c f802 	mul.w	r8, ip, r2
3000c04c:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c050:	4598      	cmp	r8, r3
3000c052:	d962      	bls.n	3000c11a <__udivmoddi4+0x19a>
3000c054:	18eb      	adds	r3, r5, r3
3000c056:	f10c 30ff 	add.w	r0, ip, #4294967295
3000c05a:	d204      	bcs.n	3000c066 <__udivmoddi4+0xe6>
3000c05c:	4598      	cmp	r8, r3
3000c05e:	d902      	bls.n	3000c066 <__udivmoddi4+0xe6>
3000c060:	f1ac 0002 	sub.w	r0, ip, #2
3000c064:	442b      	add	r3, r5
3000c066:	eba3 0308 	sub.w	r3, r3, r8
3000c06a:	b2a4      	uxth	r4, r4
3000c06c:	fbb3 fcfe 	udiv	ip, r3, lr
3000c070:	fb0e 331c 	mls	r3, lr, ip, r3
3000c074:	fb0c f202 	mul.w	r2, ip, r2
3000c078:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c07c:	42a2      	cmp	r2, r4
3000c07e:	d94e      	bls.n	3000c11e <__udivmoddi4+0x19e>
3000c080:	192c      	adds	r4, r5, r4
3000c082:	f10c 33ff 	add.w	r3, ip, #4294967295
3000c086:	d204      	bcs.n	3000c092 <__udivmoddi4+0x112>
3000c088:	42a2      	cmp	r2, r4
3000c08a:	d902      	bls.n	3000c092 <__udivmoddi4+0x112>
3000c08c:	f1ac 0302 	sub.w	r3, ip, #2
3000c090:	442c      	add	r4, r5
3000c092:	1aa4      	subs	r4, r4, r2
3000c094:	e7bd      	b.n	3000c012 <__udivmoddi4+0x92>
3000c096:	f1c7 0c20 	rsb	ip, r7, #32
3000c09a:	40bd      	lsls	r5, r7
3000c09c:	fa08 f307 	lsl.w	r3, r8, r7
3000c0a0:	40bc      	lsls	r4, r7
3000c0a2:	fa28 f90c 	lsr.w	r9, r8, ip
3000c0a6:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c0aa:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c0ae:	ea4c 0203 	orr.w	r2, ip, r3
3000c0b2:	b2ab      	uxth	r3, r5
3000c0b4:	fbb9 fcfe 	udiv	ip, r9, lr
3000c0b8:	0c11      	lsrs	r1, r2, #16
3000c0ba:	fb0e 901c 	mls	r0, lr, ip, r9
3000c0be:	fb0c f803 	mul.w	r8, ip, r3
3000c0c2:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c0c6:	4580      	cmp	r8, r0
3000c0c8:	d923      	bls.n	3000c112 <__udivmoddi4+0x192>
3000c0ca:	1828      	adds	r0, r5, r0
3000c0cc:	f10c 31ff 	add.w	r1, ip, #4294967295
3000c0d0:	d204      	bcs.n	3000c0dc <__udivmoddi4+0x15c>
3000c0d2:	4580      	cmp	r8, r0
3000c0d4:	d902      	bls.n	3000c0dc <__udivmoddi4+0x15c>
3000c0d6:	f1ac 0102 	sub.w	r1, ip, #2
3000c0da:	4428      	add	r0, r5
3000c0dc:	eba0 0008 	sub.w	r0, r0, r8
3000c0e0:	b292      	uxth	r2, r2
3000c0e2:	fbb0 fcfe 	udiv	ip, r0, lr
3000c0e6:	fb0e 001c 	mls	r0, lr, ip, r0
3000c0ea:	fb0c f803 	mul.w	r8, ip, r3
3000c0ee:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c0f2:	4598      	cmp	r8, r3
3000c0f4:	d90f      	bls.n	3000c116 <__udivmoddi4+0x196>
3000c0f6:	18eb      	adds	r3, r5, r3
3000c0f8:	f10c 32ff 	add.w	r2, ip, #4294967295
3000c0fc:	d204      	bcs.n	3000c108 <__udivmoddi4+0x188>
3000c0fe:	4598      	cmp	r8, r3
3000c100:	d902      	bls.n	3000c108 <__udivmoddi4+0x188>
3000c102:	f1ac 0202 	sub.w	r2, ip, #2
3000c106:	442b      	add	r3, r5
3000c108:	eba3 0308 	sub.w	r3, r3, r8
3000c10c:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c110:	e792      	b.n	3000c038 <__udivmoddi4+0xb8>
3000c112:	4661      	mov	r1, ip
3000c114:	e7e2      	b.n	3000c0dc <__udivmoddi4+0x15c>
3000c116:	4662      	mov	r2, ip
3000c118:	e7f6      	b.n	3000c108 <__udivmoddi4+0x188>
3000c11a:	4660      	mov	r0, ip
3000c11c:	e7a3      	b.n	3000c066 <__udivmoddi4+0xe6>
3000c11e:	4663      	mov	r3, ip
3000c120:	e7b7      	b.n	3000c092 <__udivmoddi4+0x112>
3000c122:	4543      	cmp	r3, r8
3000c124:	d905      	bls.n	3000c132 <__udivmoddi4+0x1b2>
3000c126:	b10e      	cbz	r6, 3000c12c <__udivmoddi4+0x1ac>
3000c128:	e9c6 0800 	strd	r0, r8, [r6]
3000c12c:	2100      	movs	r1, #0
3000c12e:	4608      	mov	r0, r1
3000c130:	e776      	b.n	3000c020 <__udivmoddi4+0xa0>
3000c132:	fab3 fc83 	clz	ip, r3
3000c136:	f1bc 0f00 	cmp.w	ip, #0
3000c13a:	d110      	bne.n	3000c15e <__udivmoddi4+0x1de>
3000c13c:	4543      	cmp	r3, r8
3000c13e:	d301      	bcc.n	3000c144 <__udivmoddi4+0x1c4>
3000c140:	4282      	cmp	r2, r0
3000c142:	d80a      	bhi.n	3000c15a <__udivmoddi4+0x1da>
3000c144:	1a84      	subs	r4, r0, r2
3000c146:	eb68 0303 	sbc.w	r3, r8, r3
3000c14a:	2001      	movs	r0, #1
3000c14c:	469e      	mov	lr, r3
3000c14e:	2e00      	cmp	r6, #0
3000c150:	d068      	beq.n	3000c224 <__udivmoddi4+0x2a4>
3000c152:	e9c6 4e00 	strd	r4, lr, [r6]
3000c156:	2100      	movs	r1, #0
3000c158:	e762      	b.n	3000c020 <__udivmoddi4+0xa0>
3000c15a:	4660      	mov	r0, ip
3000c15c:	e7f7      	b.n	3000c14e <__udivmoddi4+0x1ce>
3000c15e:	f1cc 0e20 	rsb	lr, ip, #32
3000c162:	fa03 f30c 	lsl.w	r3, r3, ip
3000c166:	fa02 f50c 	lsl.w	r5, r2, ip
3000c16a:	fa00 f70c 	lsl.w	r7, r0, ip
3000c16e:	fa22 f40e 	lsr.w	r4, r2, lr
3000c172:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c176:	fa20 f10e 	lsr.w	r1, r0, lr
3000c17a:	431c      	orrs	r4, r3
3000c17c:	fa08 f30c 	lsl.w	r3, r8, ip
3000c180:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c184:	4319      	orrs	r1, r3
3000c186:	b2a3      	uxth	r3, r4
3000c188:	0c0a      	lsrs	r2, r1, #16
3000c18a:	fbbb f8f9 	udiv	r8, fp, r9
3000c18e:	fb09 bb18 	mls	fp, r9, r8, fp
3000c192:	fb08 fa03 	mul.w	sl, r8, r3
3000c196:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c19a:	4592      	cmp	sl, r2
3000c19c:	d93e      	bls.n	3000c21c <__udivmoddi4+0x29c>
3000c19e:	18a2      	adds	r2, r4, r2
3000c1a0:	f108 30ff 	add.w	r0, r8, #4294967295
3000c1a4:	d204      	bcs.n	3000c1b0 <__udivmoddi4+0x230>
3000c1a6:	4592      	cmp	sl, r2
3000c1a8:	d902      	bls.n	3000c1b0 <__udivmoddi4+0x230>
3000c1aa:	f1a8 0002 	sub.w	r0, r8, #2
3000c1ae:	4422      	add	r2, r4
3000c1b0:	eba2 020a 	sub.w	r2, r2, sl
3000c1b4:	b289      	uxth	r1, r1
3000c1b6:	fbb2 f8f9 	udiv	r8, r2, r9
3000c1ba:	fb09 2218 	mls	r2, r9, r8, r2
3000c1be:	fb08 f303 	mul.w	r3, r8, r3
3000c1c2:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c1c6:	4293      	cmp	r3, r2
3000c1c8:	d92a      	bls.n	3000c220 <__udivmoddi4+0x2a0>
3000c1ca:	18a2      	adds	r2, r4, r2
3000c1cc:	f108 31ff 	add.w	r1, r8, #4294967295
3000c1d0:	d204      	bcs.n	3000c1dc <__udivmoddi4+0x25c>
3000c1d2:	4293      	cmp	r3, r2
3000c1d4:	d902      	bls.n	3000c1dc <__udivmoddi4+0x25c>
3000c1d6:	f1a8 0102 	sub.w	r1, r8, #2
3000c1da:	4422      	add	r2, r4
3000c1dc:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c1e0:	1ad2      	subs	r2, r2, r3
3000c1e2:	fba0 9805 	umull	r9, r8, r0, r5
3000c1e6:	4542      	cmp	r2, r8
3000c1e8:	464b      	mov	r3, r9
3000c1ea:	4641      	mov	r1, r8
3000c1ec:	d302      	bcc.n	3000c1f4 <__udivmoddi4+0x274>
3000c1ee:	d106      	bne.n	3000c1fe <__udivmoddi4+0x27e>
3000c1f0:	454f      	cmp	r7, r9
3000c1f2:	d204      	bcs.n	3000c1fe <__udivmoddi4+0x27e>
3000c1f4:	3801      	subs	r0, #1
3000c1f6:	ebb9 0305 	subs.w	r3, r9, r5
3000c1fa:	eb68 0104 	sbc.w	r1, r8, r4
3000c1fe:	b18e      	cbz	r6, 3000c224 <__udivmoddi4+0x2a4>
3000c200:	1afc      	subs	r4, r7, r3
3000c202:	eb62 0701 	sbc.w	r7, r2, r1
3000c206:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c20a:	fa24 f30c 	lsr.w	r3, r4, ip
3000c20e:	fa27 f70c 	lsr.w	r7, r7, ip
3000c212:	ea4e 0303 	orr.w	r3, lr, r3
3000c216:	e9c6 3700 	strd	r3, r7, [r6]
3000c21a:	e79c      	b.n	3000c156 <__udivmoddi4+0x1d6>
3000c21c:	4640      	mov	r0, r8
3000c21e:	e7c7      	b.n	3000c1b0 <__udivmoddi4+0x230>
3000c220:	4641      	mov	r1, r8
3000c222:	e7db      	b.n	3000c1dc <__udivmoddi4+0x25c>
3000c224:	4631      	mov	r1, r6
3000c226:	e6fb      	b.n	3000c020 <__udivmoddi4+0xa0>

3000c228 <__aeabi_idiv0>:
3000c228:	4770      	bx	lr
3000c22a:	bf00      	nop

3000c22c <_mbtowc_r>:
3000c22c:	b430      	push	{r4, r5}
3000c22e:	4c06      	ldr	r4, [pc, #24]	; (3000c248 <_mbtowc_r+0x1c>)
3000c230:	4d06      	ldr	r5, [pc, #24]	; (3000c24c <_mbtowc_r+0x20>)
3000c232:	6824      	ldr	r4, [r4, #0]
3000c234:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c236:	2c00      	cmp	r4, #0
3000c238:	bf08      	it	eq
3000c23a:	462c      	moveq	r4, r5
3000c23c:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c240:	46a4      	mov	ip, r4
3000c242:	bc30      	pop	{r4, r5}
3000c244:	4760      	bx	ip
3000c246:	bf00      	nop
3000c248:	3000f768 	.word	0x3000f768
3000c24c:	3000f5fc 	.word	0x3000f5fc

3000c250 <__ascii_mbtowc>:
3000c250:	b082      	sub	sp, #8
3000c252:	b901      	cbnz	r1, 3000c256 <__ascii_mbtowc+0x6>
3000c254:	a901      	add	r1, sp, #4
3000c256:	b142      	cbz	r2, 3000c26a <__ascii_mbtowc+0x1a>
3000c258:	b14b      	cbz	r3, 3000c26e <__ascii_mbtowc+0x1e>
3000c25a:	7813      	ldrb	r3, [r2, #0]
3000c25c:	600b      	str	r3, [r1, #0]
3000c25e:	7812      	ldrb	r2, [r2, #0]
3000c260:	1e10      	subs	r0, r2, #0
3000c262:	bf18      	it	ne
3000c264:	2001      	movne	r0, #1
3000c266:	b002      	add	sp, #8
3000c268:	4770      	bx	lr
3000c26a:	4610      	mov	r0, r2
3000c26c:	e7fb      	b.n	3000c266 <__ascii_mbtowc+0x16>
3000c26e:	f06f 0001 	mvn.w	r0, #1
3000c272:	e7f8      	b.n	3000c266 <__ascii_mbtowc+0x16>

3000c274 <_wctomb_r>:
3000c274:	b430      	push	{r4, r5}
3000c276:	4c06      	ldr	r4, [pc, #24]	; (3000c290 <_wctomb_r+0x1c>)
3000c278:	4d06      	ldr	r5, [pc, #24]	; (3000c294 <_wctomb_r+0x20>)
3000c27a:	6824      	ldr	r4, [r4, #0]
3000c27c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c27e:	2c00      	cmp	r4, #0
3000c280:	bf08      	it	eq
3000c282:	462c      	moveq	r4, r5
3000c284:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c288:	46a4      	mov	ip, r4
3000c28a:	bc30      	pop	{r4, r5}
3000c28c:	4760      	bx	ip
3000c28e:	bf00      	nop
3000c290:	3000f768 	.word	0x3000f768
3000c294:	3000f5fc 	.word	0x3000f5fc

3000c298 <__ascii_wctomb>:
3000c298:	b149      	cbz	r1, 3000c2ae <__ascii_wctomb+0x16>
3000c29a:	2aff      	cmp	r2, #255	; 0xff
3000c29c:	bf8d      	iteet	hi
3000c29e:	238a      	movhi	r3, #138	; 0x8a
3000c2a0:	2001      	movls	r0, #1
3000c2a2:	700a      	strbls	r2, [r1, #0]
3000c2a4:	6003      	strhi	r3, [r0, #0]
3000c2a6:	bf88      	it	hi
3000c2a8:	f04f 30ff 	movhi.w	r0, #4294967295
3000c2ac:	4770      	bx	lr
3000c2ae:	4608      	mov	r0, r1
3000c2b0:	4770      	bx	lr
3000c2b2:	bf00      	nop
3000c2b4:	0000      	movs	r0, r0
	...

3000c2b8 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000c2b8:	b401      	push	{r0}
3000c2ba:	4802      	ldr	r0, [pc, #8]	; (3000c2c4 <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000c2bc:	4684      	mov	ip, r0
3000c2be:	bc01      	pop	{r0}
3000c2c0:	4760      	bx	ip
3000c2c2:	bf00      	nop
3000c2c4:	00005b4d 	.word	0x00005b4d

3000c2c8 <____wrap_memcmp_veneer>:
3000c2c8:	b401      	push	{r0}
3000c2ca:	4802      	ldr	r0, [pc, #8]	; (3000c2d4 <____wrap_memcmp_veneer+0xc>)
3000c2cc:	4684      	mov	ip, r0
3000c2ce:	bc01      	pop	{r0}
3000c2d0:	4760      	bx	ip
3000c2d2:	bf00      	nop
3000c2d4:	00012f0d 	.word	0x00012f0d

3000c2d8 <__SBOOT_Validate_PubKey_B_veneer>:
3000c2d8:	b401      	push	{r0}
3000c2da:	4802      	ldr	r0, [pc, #8]	; (3000c2e4 <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000c2dc:	4684      	mov	ip, r0
3000c2de:	bc01      	pop	{r0}
3000c2e0:	4760      	bx	ip
3000c2e2:	bf00      	nop
3000c2e4:	00003495 	.word	0x00003495

3000c2e8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000c2e8:	b401      	push	{r0}
3000c2ea:	4802      	ldr	r0, [pc, #8]	; (3000c2f4 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000c2ec:	4684      	mov	ip, r0
3000c2ee:	bc01      	pop	{r0}
3000c2f0:	4760      	bx	ip
3000c2f2:	bf00      	nop
3000c2f4:	000024dd 	.word	0x000024dd

3000c2f8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000c2f8:	b401      	push	{r0}
3000c2fa:	4802      	ldr	r0, [pc, #8]	; (3000c304 <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000c2fc:	4684      	mov	ip, r0
3000c2fe:	bc01      	pop	{r0}
3000c300:	4760      	bx	ip
3000c302:	bf00      	nop
3000c304:	00002369 	.word	0x00002369

3000c308 <____wrap_memcpy_veneer>:
3000c308:	b401      	push	{r0}
3000c30a:	4802      	ldr	r0, [pc, #8]	; (3000c314 <____wrap_memcpy_veneer+0xc>)
3000c30c:	4684      	mov	ip, r0
3000c30e:	bc01      	pop	{r0}
3000c310:	4760      	bx	ip
3000c312:	bf00      	nop
3000c314:	00012f05 	.word	0x00012f05

3000c318 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000c318:	b401      	push	{r0}
3000c31a:	4802      	ldr	r0, [pc, #8]	; (3000c324 <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000c31c:	4684      	mov	ip, r0
3000c31e:	bc01      	pop	{r0}
3000c320:	4760      	bx	ip
3000c322:	bf00      	nop
3000c324:	00005a45 	.word	0x00005a45

3000c328 <__INT_HardFault_C_veneer>:
3000c328:	b401      	push	{r0}
3000c32a:	4802      	ldr	r0, [pc, #8]	; (3000c334 <__INT_HardFault_C_veneer+0xc>)
3000c32c:	4684      	mov	ip, r0
3000c32e:	bc01      	pop	{r0}
3000c330:	4760      	bx	ip
3000c332:	bf00      	nop
3000c334:	0000d8ed 	.word	0x0000d8ed

3000c338 <____wrap_memset_veneer>:
3000c338:	b401      	push	{r0}
3000c33a:	4802      	ldr	r0, [pc, #8]	; (3000c344 <____wrap_memset_veneer+0xc>)
3000c33c:	4684      	mov	ip, r0
3000c33e:	bc01      	pop	{r0}
3000c340:	4760      	bx	ip
3000c342:	bf00      	nop
3000c344:	00012efd 	.word	0x00012efd

3000c348 <__CRYPTO_SHA_Init_B_veneer>:
3000c348:	b401      	push	{r0}
3000c34a:	4802      	ldr	r0, [pc, #8]	; (3000c354 <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000c34c:	4684      	mov	ip, r0
3000c34e:	bc01      	pop	{r0}
3000c350:	4760      	bx	ip
3000c352:	bf00      	nop
3000c354:	00002451 	.word	0x00002451

3000c358 <__DiagVSprintf_veneer>:
3000c358:	b401      	push	{r0}
3000c35a:	4802      	ldr	r0, [pc, #8]	; (3000c364 <__DiagVSprintf_veneer+0xc>)
3000c35c:	4684      	mov	ip, r0
3000c35e:	bc01      	pop	{r0}
3000c360:	4760      	bx	ip
3000c362:	bf00      	nop
3000c364:	0000e331 	.word	0x0000e331

3000c368 <__rtl_crypto_sha2_update_B_veneer>:
3000c368:	b401      	push	{r0}
3000c36a:	4802      	ldr	r0, [pc, #8]	; (3000c374 <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000c36c:	4684      	mov	ip, r0
3000c36e:	bc01      	pop	{r0}
3000c370:	4760      	bx	ip
3000c372:	bf00      	nop
3000c374:	00005385 	.word	0x00005385

3000c378 <__rtl_crypto_sha2_final_A_veneer>:
3000c378:	b401      	push	{r0}
3000c37a:	4802      	ldr	r0, [pc, #8]	; (3000c384 <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000c37c:	4684      	mov	ip, r0
3000c37e:	bc01      	pop	{r0}
3000c380:	4760      	bx	ip
3000c382:	bf00      	nop
3000c384:	0000527d 	.word	0x0000527d

3000c388 <__DiagPrintf_veneer>:
3000c388:	b401      	push	{r0}
3000c38a:	4802      	ldr	r0, [pc, #8]	; (3000c394 <__DiagPrintf_veneer+0xc>)
3000c38c:	4684      	mov	ip, r0
3000c38e:	bc01      	pop	{r0}
3000c390:	4760      	bx	ip
3000c392:	bf00      	nop
3000c394:	0000e5f9 	.word	0x0000e5f9

3000c398 <__rtl_crypto_sha2_init_B_veneer>:
3000c398:	b401      	push	{r0}
3000c39a:	4802      	ldr	r0, [pc, #8]	; (3000c3a4 <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000c39c:	4684      	mov	ip, r0
3000c39e:	bc01      	pop	{r0}
3000c3a0:	4760      	bx	ip
3000c3a2:	bf00      	nop
3000c3a4:	00004ffd 	.word	0x00004ffd

3000c3a8 <__rtl_crypto_sha2_init_A_veneer>:
3000c3a8:	b401      	push	{r0}
3000c3aa:	4802      	ldr	r0, [pc, #8]	; (3000c3b4 <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000c3ac:	4684      	mov	ip, r0
3000c3ae:	bc01      	pop	{r0}
3000c3b0:	4760      	bx	ip
3000c3b2:	bf00      	nop
3000c3b4:	00004e89 	.word	0x00004e89

3000c3b8 <__SBOOT_Validate_Signature_A_veneer>:
3000c3b8:	b401      	push	{r0}
3000c3ba:	4802      	ldr	r0, [pc, #8]	; (3000c3c4 <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000c3bc:	4684      	mov	ip, r0
3000c3be:	bc01      	pop	{r0}
3000c3c0:	4760      	bx	ip
3000c3c2:	bf00      	nop
3000c3c4:	000032bd 	.word	0x000032bd

3000c3c8 <__SBOOT_Validate_ImgHash_B_veneer>:
3000c3c8:	b401      	push	{r0}
3000c3ca:	4802      	ldr	r0, [pc, #8]	; (3000c3d4 <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000c3cc:	4684      	mov	ip, r0
3000c3ce:	bc01      	pop	{r0}
3000c3d0:	4760      	bx	ip
3000c3d2:	bf00      	nop
3000c3d4:	00003541 	.word	0x00003541

3000c3d8 <____wrap_strlen_veneer>:
3000c3d8:	b401      	push	{r0}
3000c3da:	4802      	ldr	r0, [pc, #8]	; (3000c3e4 <____wrap_strlen_veneer+0xc>)
3000c3dc:	4684      	mov	ip, r0
3000c3de:	bc01      	pop	{r0}
3000c3e0:	4760      	bx	ip
3000c3e2:	bf00      	nop
3000c3e4:	00012f25 	.word	0x00012f25

3000c3e8 <__SBOOT_Validate_PubKey_A_veneer>:
3000c3e8:	b401      	push	{r0}
3000c3ea:	4802      	ldr	r0, [pc, #8]	; (3000c3f4 <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000c3ec:	4684      	mov	ip, r0
3000c3ee:	bc01      	pop	{r0}
3000c3f0:	4760      	bx	ip
3000c3f2:	bf00      	nop
3000c3f4:	00003321 	.word	0x00003321

3000c3f8 <__SBOOT_Validate_Algorithm_A_veneer>:
3000c3f8:	b401      	push	{r0}
3000c3fa:	4802      	ldr	r0, [pc, #8]	; (3000c404 <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000c3fc:	4684      	mov	ip, r0
3000c3fe:	bc01      	pop	{r0}
3000c400:	4760      	bx	ip
3000c402:	bf00      	nop
3000c404:	00003279 	.word	0x00003279

3000c408 <__io_assert_failed_veneer>:
3000c408:	b401      	push	{r0}
3000c40a:	4802      	ldr	r0, [pc, #8]	; (3000c414 <__io_assert_failed_veneer+0xc>)
3000c40c:	4684      	mov	ip, r0
3000c40e:	bc01      	pop	{r0}
3000c410:	4760      	bx	ip
3000c412:	bf00      	nop
3000c414:	0000992d 	.word	0x0000992d

3000c418 <__rtl_crypto_sha2_final_B_veneer>:
3000c418:	b401      	push	{r0}
3000c41a:	4802      	ldr	r0, [pc, #8]	; (3000c424 <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000c41c:	4684      	mov	ip, r0
3000c41e:	bc01      	pop	{r0}
3000c420:	4760      	bx	ip
3000c422:	bf00      	nop
3000c424:	000053f1 	.word	0x000053f1

3000c428 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000c428:	b401      	push	{r0}
3000c42a:	4802      	ldr	r0, [pc, #8]	; (3000c434 <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000c42c:	4684      	mov	ip, r0
3000c42e:	bc01      	pop	{r0}
3000c430:	4760      	bx	ip
3000c432:	bf00      	nop
3000c434:	00005bb9 	.word	0x00005bb9

3000c438 <__SBOOT_Validate_ImgHash_A_veneer>:
3000c438:	b401      	push	{r0}
3000c43a:	4802      	ldr	r0, [pc, #8]	; (3000c444 <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000c43c:	4684      	mov	ip, r0
3000c43e:	bc01      	pop	{r0}
3000c440:	4760      	bx	ip
3000c442:	bf00      	nop
3000c444:	000033cd 	.word	0x000033cd

3000c448 <__SBOOT_GetMdType_B_veneer>:
3000c448:	b401      	push	{r0}
3000c44a:	4802      	ldr	r0, [pc, #8]	; (3000c454 <__SBOOT_GetMdType_B_veneer+0xc>)
3000c44c:	4684      	mov	ip, r0
3000c44e:	bc01      	pop	{r0}
3000c450:	4760      	bx	ip
3000c452:	bf00      	nop
3000c454:	0000319d 	.word	0x0000319d

3000c458 <__rtl_crypto_sha2_update_A_veneer>:
3000c458:	b401      	push	{r0}
3000c45a:	4802      	ldr	r0, [pc, #8]	; (3000c464 <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000c45c:	4684      	mov	ip, r0
3000c45e:	bc01      	pop	{r0}
3000c460:	4760      	bx	ip
3000c462:	bf00      	nop
3000c464:	00005211 	.word	0x00005211

3000c468 <__SBOOT_GetMdType_A_veneer>:
3000c468:	b401      	push	{r0}
3000c46a:	4802      	ldr	r0, [pc, #8]	; (3000c474 <__SBOOT_GetMdType_A_veneer+0xc>)
3000c46c:	4684      	mov	ip, r0
3000c46e:	bc01      	pop	{r0}
3000c470:	4760      	bx	ip
3000c472:	bf00      	nop
3000c474:	00003029 	.word	0x00003029

3000c478 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000c478:	b401      	push	{r0}
3000c47a:	4802      	ldr	r0, [pc, #8]	; (3000c484 <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000c47c:	4684      	mov	ip, r0
3000c47e:	bc01      	pop	{r0}
3000c480:	4760      	bx	ip
3000c482:	bf00      	nop
3000c484:	000059d9 	.word	0x000059d9

3000c488 <__SBOOT_Validate_Algorithm_B_veneer>:
3000c488:	b401      	push	{r0}
3000c48a:	4802      	ldr	r0, [pc, #8]	; (3000c494 <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000c48c:	4684      	mov	ip, r0
3000c48e:	bc01      	pop	{r0}
3000c490:	4760      	bx	ip
3000c492:	bf00      	nop
3000c494:	000033ed 	.word	0x000033ed

3000c498 <__CRYPTO_SHA_Init_A_veneer>:
3000c498:	b401      	push	{r0}
3000c49a:	4802      	ldr	r0, [pc, #8]	; (3000c4a4 <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000c49c:	4684      	mov	ip, r0
3000c49e:	bc01      	pop	{r0}
3000c4a0:	4760      	bx	ip
3000c4a2:	bf00      	nop
3000c4a4:	000022dd 	.word	0x000022dd

3000c4a8 <__SBOOT_Validate_Signature_B_veneer>:
3000c4a8:	b401      	push	{r0}
3000c4aa:	4802      	ldr	r0, [pc, #8]	; (3000c4b4 <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000c4ac:	4684      	mov	ip, r0
3000c4ae:	bc01      	pop	{r0}
3000c4b0:	4760      	bx	ip
3000c4b2:	bf00      	nop
3000c4b4:	00003431 	.word	0x00003431
3000c4b8:	004f4931 	.word	0x004f4931
3000c4bc:	00004f32 	.word	0x00004f32
3000c4c0:	004f4932 	.word	0x004f4932
3000c4c4:	00004f34 	.word	0x00004f34
3000c4c8:	004f4934 	.word	0x004f4934
3000c4cc:	53414c46 	.word	0x53414c46
3000c4d0:	4b4c4348 	.word	0x4b4c4348
3000c4d4:	00000000 	.word	0x00000000
3000c4d8:	2d73255b 	.word	0x2d73255b
3000c4dc:	63205d49 	.word	0x63205d49
3000c4e0:	62696c61 	.word	0x62696c61
3000c4e4:	69746172 	.word	0x69746172
3000c4e8:	6f5f6e6f 	.word	0x6f5f6e6f
3000c4ec:	255b3a6b 	.word	0x255b3a6b
3000c4f0:	64253a64 	.word	0x64253a64
3000c4f4:	5d64253a 	.word	0x5d64253a
3000c4f8:	00000a20 	.word	0x00000a20
3000c4fc:	2d73255b 	.word	0x2d73255b
3000c500:	46205d49 	.word	0x46205d49
3000c504:	4853414c 	.word	0x4853414c
3000c508:	4c414320 	.word	0x4c414320
3000c50c:	305b4249 	.word	0x305b4249
3000c510:	20782578 	.word	0x20782578
3000c514:	0a5d4b4f 	.word	0x0a5d4b4f
3000c518:	00000000 	.word	0x00000000
3000c51c:	2d73255b 	.word	0x2d73255b
3000c520:	46205d45 	.word	0x46205d45
3000c524:	4853414c 	.word	0x4853414c
3000c528:	4c414320 	.word	0x4c414320
3000c52c:	305b4249 	.word	0x305b4249
3000c530:	20782578 	.word	0x20782578
3000c534:	4c494146 	.word	0x4c494146
3000c538:	00000a5d 	.word	0x00000a5d
3000c53c:	2d73255b 	.word	0x2d73255b
3000c540:	46205d49 	.word	0x46205d49
3000c544:	6873616c 	.word	0x6873616c
3000c548:	61655220 	.word	0x61655220
3000c54c:	73252064 	.word	0x73252064
3000c550:	0000000a 	.word	0x0000000a
3000c554:	2d73255b 	.word	0x2d73255b
3000c558:	46205d45 	.word	0x46205d45
3000c55c:	6873616c 	.word	0x6873616c
3000c560:	69775320 	.word	0x69775320
3000c564:	20686374 	.word	0x20686374
3000c568:	64616552 	.word	0x64616552
3000c56c:	646f4d20 	.word	0x646f4d20
3000c570:	41462065 	.word	0x41462065
3000c574:	000a4c49 	.word	0x000a4c49
3000c578:	2d73255b 	.word	0x2d73255b
3000c57c:	46205d49 	.word	0x46205d49
3000c580:	6873616c 	.word	0x6873616c
3000c584:	3a444920 	.word	0x3a444920
3000c588:	2d782520 	.word	0x2d782520
3000c58c:	252d7825 	.word	0x252d7825
3000c590:	00000a78 	.word	0x00000a78
3000c594:	2d73255b 	.word	0x2d73255b
3000c598:	54205d57 	.word	0x54205d57
3000c59c:	20736968 	.word	0x20736968
3000c5a0:	73616c66 	.word	0x73616c66
3000c5a4:	79742068 	.word	0x79742068
3000c5a8:	69206570 	.word	0x69206570
3000c5ac:	6f6e2073 	.word	0x6f6e2073
3000c5b0:	75732074 	.word	0x75732074
3000c5b4:	726f7070 	.word	0x726f7070
3000c5b8:	21646574 	.word	0x21646574
3000c5bc:	0000000a 	.word	0x0000000a
3000c5c0:	2d73255b 	.word	0x2d73255b
3000c5c4:	4e205d49 	.word	0x4e205d49
3000c5c8:	20444e41 	.word	0x20444e41
3000c5cc:	203a4449 	.word	0x203a4449
3000c5d0:	252d7825 	.word	0x252d7825
3000c5d4:	00000a78 	.word	0x00000a78
3000c5d8:	00003552 	.word	0x00003552
3000c5dc:	00003652 	.word	0x00003652
3000c5e0:	00003752 	.word	0x00003752
3000c5e4:	00003852 	.word	0x00003852
3000c5e8:	00003952 	.word	0x00003952
3000c5ec:	00303152 	.word	0x00303152
3000c5f0:	00313152 	.word	0x00313152
3000c5f4:	00003152 	.word	0x00003152
3000c5f8:	00003252 	.word	0x00003252
3000c5fc:	00003352 	.word	0x00003352
3000c600:	20304d4b 	.word	0x20304d4b
3000c604:	20504958 	.word	0x20504958
3000c608:	00474d49 	.word	0x00474d49
3000c60c:	20304d4b 	.word	0x20304d4b
3000c610:	4d415253 	.word	0x4d415253
3000c614:	00000000 	.word	0x00000000
3000c618:	20304d4b 	.word	0x20304d4b
3000c61c:	4d415244 	.word	0x4d415244
3000c620:	00000000 	.word	0x00000000
3000c624:	20344d4b 	.word	0x20344d4b
3000c628:	20504958 	.word	0x20504958
3000c62c:	00474d49 	.word	0x00474d49
3000c630:	20344d4b 	.word	0x20344d4b
3000c634:	4d415253 	.word	0x4d415253
3000c638:	00000000 	.word	0x00000000
3000c63c:	20344d4b 	.word	0x20344d4b
3000c640:	4d415244 	.word	0x4d415244
3000c644:	00000000 	.word	0x00000000
3000c648:	58205041 	.word	0x58205041
3000c64c:	49205049 	.word	0x49205049
3000c650:	0000474d 	.word	0x0000474d
3000c654:	42205041 	.word	0x42205041
3000c658:	5320314c 	.word	0x5320314c
3000c65c:	004d4152 	.word	0x004d4152
3000c660:	42205041 	.word	0x42205041
3000c664:	4420314c 	.word	0x4420314c
3000c668:	004d4152 	.word	0x004d4152
3000c66c:	46205041 	.word	0x46205041
3000c670:	00005049 	.word	0x00005049
3000c674:	544f4f42 	.word	0x544f4f42
3000c678:	00000000 	.word	0x00000000
3000c67c:	2d73255b 	.word	0x2d73255b
3000c680:	25205d49 	.word	0x25205d49
3000c684:	6e492073 	.word	0x6e492073
3000c688:	696c6176 	.word	0x696c6176
3000c68c:	00000a64 	.word	0x00000a64
3000c690:	2d73255b 	.word	0x2d73255b
3000c694:	25205d49 	.word	0x25205d49
3000c698:	30255b73 	.word	0x30255b73
3000c69c:	253a7838 	.word	0x253a7838
3000c6a0:	000a5d78 	.word	0x000a5d78
3000c6a4:	2d73255b 	.word	0x2d73255b
3000c6a8:	52205d49 	.word	0x52205d49
3000c6ac:	45205044 	.word	0x45205044
3000c6b0:	00000a4e 	.word	0x00000a4e
3000c6b4:	2d73255b 	.word	0x2d73255b
3000c6b8:	46205d49 	.word	0x46205d49
3000c6bc:	206c6961 	.word	0x206c6961
3000c6c0:	6c206f74 	.word	0x6c206f74
3000c6c4:	2064616f 	.word	0x2064616f
3000c6c8:	20504452 	.word	0x20504452
3000c6cc:	67616d69 	.word	0x67616d69
3000c6d0:	000a2165 	.word	0x000a2165
3000c6d4:	2d73255b 	.word	0x2d73255b
3000c6d8:	52205d49 	.word	0x52205d49
3000c6dc:	4f205044 	.word	0x4f205044
3000c6e0:	000a4646 	.word	0x000a4646
3000c6e4:	2d73255b 	.word	0x2d73255b
3000c6e8:	49205d49 	.word	0x49205d49
3000c6ec:	2032474d 	.word	0x2032474d
3000c6f0:	2046544f 	.word	0x2046544f
3000c6f4:	000a4e45 	.word	0x000a4e45
3000c6f8:	2d73255b 	.word	0x2d73255b
3000c6fc:	4f205d45 	.word	0x4f205d45
3000c700:	4d204654 	.word	0x4d204654
3000c704:	2065646f 	.word	0x2065646f
3000c708:	6f727265 	.word	0x6f727265
3000c70c:	000a0d72 	.word	0x000a0d72
3000c710:	494c4156 	.word	0x494c4156
3000c714:	00000044 	.word	0x00000044
3000c718:	41564e49 	.word	0x41564e49
3000c71c:	0044494c 	.word	0x0044494c
3000c720:	74726563 	.word	0x74726563
3000c724:	63696669 	.word	0x63696669
3000c728:	20657461 	.word	0x20657461
3000c72c:	20434345 	.word	0x20434345
3000c730:	69726576 	.word	0x69726576
3000c734:	66207966 	.word	0x66207966
3000c738:	2c6c6961 	.word	0x2c6c6961
3000c73c:	6c6f7220 	.word	0x6c6f7220
3000c740:	6361626c 	.word	0x6361626c
3000c744:	000a0d6b 	.word	0x000a0d6b
3000c748:	444f4d5b 	.word	0x444f4d5b
3000c74c:	5f454c55 	.word	0x5f454c55
3000c750:	544f4f42 	.word	0x544f4f42
3000c754:	56454c2d 	.word	0x56454c2d
3000c758:	495f4c45 	.word	0x495f4c45
3000c75c:	5d4f464e 	.word	0x5d4f464e
3000c760:	474d493a 	.word	0x474d493a
3000c764:	544f2832 	.word	0x544f2832
3000c768:	29642541 	.word	0x29642541
3000c76c:	2c732520 	.word	0x2c732520
3000c770:	74657220 	.word	0x74657220
3000c774:	6425203a 	.word	0x6425203a
3000c778:	0000000a 	.word	0x0000000a
3000c77c:	32474d49 	.word	0x32474d49
3000c780:	43434520 	.word	0x43434520
3000c784:	72657620 	.word	0x72657620
3000c788:	20796669 	.word	0x20796669
3000c78c:	6c696166 	.word	0x6c696166
3000c790:	6f72202c 	.word	0x6f72202c
3000c794:	61626c6c 	.word	0x61626c6c
3000c798:	0a0d6b63 	.word	0x0a0d6b63
3000c79c:	00000000 	.word	0x00000000
3000c7a0:	5f41544f 	.word	0x5f41544f
3000c7a4:	45205041 	.word	0x45205041
3000c7a8:	76204343 	.word	0x76204343
3000c7ac:	66697265 	.word	0x66697265
3000c7b0:	61662079 	.word	0x61662079
3000c7b4:	202c6c69 	.word	0x202c6c69
3000c7b8:	6c6c6f72 	.word	0x6c6c6f72
3000c7bc:	6b636162 	.word	0x6b636162
3000c7c0:	00000a0d 	.word	0x00000a0d
3000c7c4:	64616f4c 	.word	0x64616f4c
3000c7c8:	41544f20 	.word	0x41544f20
3000c7cc:	2050415f 	.word	0x2050415f
3000c7d0:	67616d69 	.word	0x67616d69
3000c7d4:	61662065 	.word	0x61662065
3000c7d8:	202c6c69 	.word	0x202c6c69
3000c7dc:	6c6c6f72 	.word	0x6c6c6f72
3000c7e0:	6b636162 	.word	0x6b636162
3000c7e4:	00000a0d 	.word	0x00000a0d
3000c7e8:	444f4d5b 	.word	0x444f4d5b
3000c7ec:	5f454c55 	.word	0x5f454c55
3000c7f0:	544f4f42 	.word	0x544f4f42
3000c7f4:	56454c2d 	.word	0x56454c2d
3000c7f8:	455f4c45 	.word	0x455f4c45
3000c7fc:	524f5252 	.word	0x524f5252
3000c800:	61463a5d 	.word	0x61463a5d
3000c804:	74206c69 	.word	0x74206c69
3000c808:	6f6c206f 	.word	0x6f6c206f
3000c80c:	41206461 	.word	0x41206461
3000c810:	6d692050 	.word	0x6d692050
3000c814:	21656761 	.word	0x21656761
3000c818:	0000000a 	.word	0x0000000a
3000c81c:	20344d4b 	.word	0x20344d4b
3000c820:	33474d49 	.word	0x33474d49
3000c824:	00000000 	.word	0x00000000
3000c828:	20344d4b 	.word	0x20344d4b
3000c82c:	0043534e 	.word	0x0043534e
3000c830:	31474d49 	.word	0x31474d49
3000c834:	00000000 	.word	0x00000000
3000c838:	32474d49 	.word	0x32474d49
3000c83c:	00000000 	.word	0x00000000
3000c840:	42205041 	.word	0x42205041
3000c844:	4626314c 	.word	0x4626314c
3000c848:	00005049 	.word	0x00005049
3000c84c:	444f4d5b 	.word	0x444f4d5b
3000c850:	5f454c55 	.word	0x5f454c55
3000c854:	544f4f42 	.word	0x544f4f42
3000c858:	56454c2d 	.word	0x56454c2d
3000c85c:	495f4c45 	.word	0x495f4c45
3000c860:	5d4f464e 	.word	0x5d4f464e
3000c864:	5b73253a 	.word	0x5b73253a
3000c868:	78383025 	.word	0x78383025
3000c86c:	5d78253a 	.word	0x5d78253a
3000c870:	0000000a 	.word	0x0000000a
3000c874:	20504452 	.word	0x20504452
3000c878:	4c494146 	.word	0x4c494146
3000c87c:	0000000a 	.word	0x0000000a
3000c880:	20504452 	.word	0x20504452
3000c884:	43435553 	.word	0x43435553
3000c888:	0a535345 	.word	0x0a535345
3000c88c:	00000000 	.word	0x00000000
3000c890:	059e4eb3 	.word	0x059e4eb3
3000c894:	4b11ea39 	.word	0x4b11ea39
3000c898:	2a1b920a 	.word	0x2a1b920a
3000c89c:	29939cee 	.word	0x29939cee
3000c8a0:	00000000 	.word	0x00000000
3000c8a4:	42205041 	.word	0x42205041
3000c8a8:	0032334c 	.word	0x0032334c
3000c8ac:	2d73255b 	.word	0x2d73255b
3000c8b0:	25205d49 	.word	0x25205d49
3000c8b4:	45562073 	.word	0x45562073
3000c8b8:	59464952 	.word	0x59464952
3000c8bc:	53415020 	.word	0x53415020
3000c8c0:	00000a53 	.word	0x00000a53
3000c8c4:	2d73255b 	.word	0x2d73255b
3000c8c8:	25205d45 	.word	0x25205d45
3000c8cc:	45562073 	.word	0x45562073
3000c8d0:	59464952 	.word	0x59464952
3000c8d4:	49414620 	.word	0x49414620
3000c8d8:	72202c4c 	.word	0x72202c4c
3000c8dc:	3d207465 	.word	0x3d207465
3000c8e0:	0a642520 	.word	0x0a642520
3000c8e4:	00000000 	.word	0x00000000
3000c8e8:	2d73255b 	.word	0x2d73255b
3000c8ec:	43205d45 	.word	0x43205d45
3000c8f0:	69747265 	.word	0x69747265
3000c8f4:	61636966 	.word	0x61636966
3000c8f8:	56206574 	.word	0x56206574
3000c8fc:	46495245 	.word	0x46495245
3000c900:	41462059 	.word	0x41462059
3000c904:	202c4c49 	.word	0x202c4c49
3000c908:	20746572 	.word	0x20746572
3000c90c:	6425203d 	.word	0x6425203d
3000c910:	0000000a 	.word	0x0000000a
3000c914:	2d73255b 	.word	0x2d73255b
3000c918:	48205d41 	.word	0x48205d41
3000c91c:	20647261 	.word	0x20647261
3000c920:	6c756146 	.word	0x6c756146
3000c924:	61502074 	.word	0x61502074
3000c928:	20686374 	.word	0x20686374
3000c92c:	63655328 	.word	0x63655328
3000c930:	29657275 	.word	0x29657275
3000c934:	00000a0d 	.word	0x00000a0d
3000c938:	2d73255b 	.word	0x2d73255b
3000c93c:	45205d41 	.word	0x45205d41
3000c940:	70656378 	.word	0x70656378
3000c944:	6e6f6974 	.word	0x6e6f6974
3000c948:	6f726620 	.word	0x6f726620
3000c94c:	6573206d 	.word	0x6573206d
3000c950:	65727563 	.word	0x65727563
3000c954:	726f7720 	.word	0x726f7720
3000c958:	0d21646c 	.word	0x0d21646c
3000c95c:	0000000a 	.word	0x0000000a
3000c960:	2d73255b 	.word	0x2d73255b
3000c964:	45205d41 	.word	0x45205d41
3000c968:	70656378 	.word	0x70656378
3000c96c:	6e6f6974 	.word	0x6e6f6974
3000c970:	6f726620 	.word	0x6f726620
3000c974:	6f6e206d 	.word	0x6f6e206d
3000c978:	65732d6e 	.word	0x65732d6e
3000c97c:	65727563 	.word	0x65727563
3000c980:	726f7720 	.word	0x726f7720
3000c984:	0d21646c 	.word	0x0d21646c
3000c988:	0000000a 	.word	0x0000000a
3000c98c:	2d73255b 	.word	0x2d73255b
3000c990:	53205d41 	.word	0x53205d41
3000c994:	20525346 	.word	0x20525346
3000c998:	30203d20 	.word	0x30203d20
3000c99c:	38302578 	.word	0x38302578
3000c9a0:	000a0d78 	.word	0x000a0d78
3000c9a4:	2d73255b 	.word	0x2d73255b
3000c9a8:	53205d41 	.word	0x53205d41
3000c9ac:	20524146 	.word	0x20524146
3000c9b0:	30203d20 	.word	0x30203d20
3000c9b4:	38302578 	.word	0x38302578
3000c9b8:	000a0d78 	.word	0x000a0d78
3000c9bc:	2d73255b 	.word	0x2d73255b
3000c9c0:	53205d41 	.word	0x53205d41
3000c9c4:	52534348 	.word	0x52534348
3000c9c8:	30203d20 	.word	0x30203d20
3000c9cc:	38302578 	.word	0x38302578
3000c9d0:	000a0d78 	.word	0x000a0d78
3000c9d4:	3141544f 	.word	0x3141544f
3000c9d8:	45535520 	.word	0x45535520
3000c9dc:	6576202c 	.word	0x6576202c
3000c9e0:	6f697372 	.word	0x6f697372
3000c9e4:	25203a6e 	.word	0x25203a6e
3000c9e8:	00000a64 	.word	0x00000a64
3000c9ec:	3241544f 	.word	0x3241544f
3000c9f0:	45535520 	.word	0x45535520
3000c9f4:	6576202c 	.word	0x6576202c
3000c9f8:	6f697372 	.word	0x6f697372
3000c9fc:	25203a6e 	.word	0x25203a6e
3000ca00:	00000a64 	.word	0x00000a64
3000ca04:	68746f42 	.word	0x68746f42
3000ca08:	41544f20 	.word	0x41544f20
3000ca0c:	616d6920 	.word	0x616d6920
3000ca10:	76206567 	.word	0x76206567
3000ca14:	66697265 	.word	0x66697265
3000ca18:	61662079 	.word	0x61662079
3000ca1c:	64656c69 	.word	0x64656c69
3000ca20:	0000000a 	.word	0x0000000a
3000ca24:	2d73255b 	.word	0x2d73255b
3000ca28:	49205d49 	.word	0x49205d49
3000ca2c:	2074696e 	.word	0x2074696e
3000ca30:	0a0d4257 	.word	0x0a0d4257
3000ca34:	00000000 	.word	0x00000000
3000ca38:	2d73255b 	.word	0x2d73255b
3000ca3c:	52205d49 	.word	0x52205d49
3000ca40:	696e4965 	.word	0x696e4965
3000ca44:	44442074 	.word	0x44442074
3000ca48:	000a0d52 	.word	0x000a0d52
3000ca4c:	73616c66 	.word	0x73616c66
3000ca50:	69735f68 	.word	0x69735f68
3000ca54:	203a657a 	.word	0x203a657a
3000ca58:	000a4d38 	.word	0x000a4d38
3000ca5c:	73616c66 	.word	0x73616c66
3000ca60:	69735f68 	.word	0x69735f68
3000ca64:	203a657a 	.word	0x203a657a
3000ca68:	0a4d3631 	.word	0x0a4d3631
3000ca6c:	00000000 	.word	0x00000000
3000ca70:	20315042 	.word	0x20315042
3000ca74:	73726576 	.word	0x73726576
3000ca78:	206e6f69 	.word	0x206e6f69
3000ca7c:	61766e69 	.word	0x61766e69
3000ca80:	0a64696c 	.word	0x0a64696c
3000ca84:	00000000 	.word	0x00000000
3000ca88:	20315042 	.word	0x20315042
3000ca8c:	6d726f66 	.word	0x6d726f66
3000ca90:	69207461 	.word	0x69207461
3000ca94:	6c61766e 	.word	0x6c61766e
3000ca98:	000a6469 	.word	0x000a6469
3000ca9c:	20315042 	.word	0x20315042
3000caa0:	69746361 	.word	0x69746361
3000caa4:	69206576 	.word	0x69206576
3000caa8:	7865646e 	.word	0x7865646e
3000caac:	766e6920 	.word	0x766e6920
3000cab0:	64696c61 	.word	0x64696c61
3000cab4:	0000000a 	.word	0x0000000a
3000cab8:	20315042 	.word	0x20315042
3000cabc:	61746164 	.word	0x61746164
3000cac0:	6c617620 	.word	0x6c617620
3000cac4:	202c6469 	.word	0x202c6469
3000cac8:	73726576 	.word	0x73726576
3000cacc:	3a6e6f69 	.word	0x3a6e6f69
3000cad0:	000a6425 	.word	0x000a6425
3000cad4:	20325042 	.word	0x20325042
3000cad8:	6d726f66 	.word	0x6d726f66
3000cadc:	69207461 	.word	0x69207461
3000cae0:	6c61766e 	.word	0x6c61766e
3000cae4:	000a6469 	.word	0x000a6469
3000cae8:	20315042 	.word	0x20315042
3000caec:	33435243 	.word	0x33435243
3000caf0:	616d2032 	.word	0x616d2032
3000caf4:	2c686374 	.word	0x2c686374
3000caf8:	74746120 	.word	0x74746120
3000cafc:	65686361 	.word	0x65686361
3000cb00:	52432064 	.word	0x52432064
3000cb04:	30203a43 	.word	0x30203a43
3000cb08:	2c782578 	.word	0x2c782578
3000cb0c:	6c616320 	.word	0x6c616320
3000cb10:	616c7563 	.word	0x616c7563
3000cb14:	20646574 	.word	0x20646574
3000cb18:	3a435243 	.word	0x3a435243
3000cb1c:	25783020 	.word	0x25783020
3000cb20:	00000a78 	.word	0x00000a78
3000cb24:	20315042 	.word	0x20315042
3000cb28:	33435243 	.word	0x33435243
3000cb2c:	6f6e2032 	.word	0x6f6e2032
3000cb30:	616d2074 	.word	0x616d2074
3000cb34:	2c686374 	.word	0x2c686374
3000cb38:	74746120 	.word	0x74746120
3000cb3c:	65686361 	.word	0x65686361
3000cb40:	52432064 	.word	0x52432064
3000cb44:	30203a43 	.word	0x30203a43
3000cb48:	2c782578 	.word	0x2c782578
3000cb4c:	6c616320 	.word	0x6c616320
3000cb50:	616c7563 	.word	0x616c7563
3000cb54:	20646574 	.word	0x20646574
3000cb58:	3a435243 	.word	0x3a435243
3000cb5c:	25783020 	.word	0x25783020
3000cb60:	00000a78 	.word	0x00000a78
3000cb64:	20325042 	.word	0x20325042
3000cb68:	33435243 	.word	0x33435243
3000cb6c:	616d2032 	.word	0x616d2032
3000cb70:	2c686374 	.word	0x2c686374
3000cb74:	74746120 	.word	0x74746120
3000cb78:	65686361 	.word	0x65686361
3000cb7c:	52432064 	.word	0x52432064
3000cb80:	30203a43 	.word	0x30203a43
3000cb84:	2c782578 	.word	0x2c782578
3000cb88:	6c616320 	.word	0x6c616320
3000cb8c:	616c7563 	.word	0x616c7563
3000cb90:	20646574 	.word	0x20646574
3000cb94:	3a435243 	.word	0x3a435243
3000cb98:	25783020 	.word	0x25783020
3000cb9c:	00000a78 	.word	0x00000a78
3000cba0:	20325042 	.word	0x20325042
3000cba4:	33435243 	.word	0x33435243
3000cba8:	6f6e2032 	.word	0x6f6e2032
3000cbac:	616d2074 	.word	0x616d2074
3000cbb0:	2c686374 	.word	0x2c686374
3000cbb4:	74746120 	.word	0x74746120
3000cbb8:	65686361 	.word	0x65686361
3000cbbc:	52432064 	.word	0x52432064
3000cbc0:	30203a43 	.word	0x30203a43
3000cbc4:	2c782578 	.word	0x2c782578
3000cbc8:	6c616320 	.word	0x6c616320
3000cbcc:	616c7563 	.word	0x616c7563
3000cbd0:	20646574 	.word	0x20646574
3000cbd4:	3a435243 	.word	0x3a435243
3000cbd8:	25783020 	.word	0x25783020
3000cbdc:	00000a78 	.word	0x00000a78
3000cbe0:	20315042 	.word	0x20315042
3000cbe4:	3161746f 	.word	0x3161746f
3000cbe8:	64646120 	.word	0x64646120
3000cbec:	73736572 	.word	0x73736572
3000cbf0:	766e6920 	.word	0x766e6920
3000cbf4:	64696c61 	.word	0x64696c61
3000cbf8:	0000000a 	.word	0x0000000a
3000cbfc:	20315042 	.word	0x20315042
3000cc00:	3261746f 	.word	0x3261746f
3000cc04:	64646120 	.word	0x64646120
3000cc08:	73736572 	.word	0x73736572
3000cc0c:	766e6920 	.word	0x766e6920
3000cc10:	64696c61 	.word	0x64696c61
3000cc14:	0000000a 	.word	0x0000000a
3000cc18:	68746f42 	.word	0x68746f42
3000cc1c:	20504220 	.word	0x20504220
3000cc20:	20435243 	.word	0x20435243
3000cc24:	696c6176 	.word	0x696c6176
3000cc28:	00000a64 	.word	0x00000a64
3000cc2c:	20315042 	.word	0x20315042
3000cc30:	656c6573 	.word	0x656c6573
3000cc34:	64657463 	.word	0x64657463
3000cc38:	0000000a 	.word	0x0000000a
3000cc3c:	20325042 	.word	0x20325042
3000cc40:	656c6573 	.word	0x656c6573
3000cc44:	64657463 	.word	0x64657463
3000cc48:	0000000a 	.word	0x0000000a
3000cc4c:	68746f42 	.word	0x68746f42
3000cc50:	20504220 	.word	0x20504220
3000cc54:	61766e69 	.word	0x61766e69
3000cc58:	0a64696c 	.word	0x0a64696c
3000cc5c:	00000000 	.word	0x00000000
3000cc60:	6f636552 	.word	0x6f636552
3000cc64:	79726576 	.word	0x79726576
3000cc68:	61747320 	.word	0x61747320
3000cc6c:	000a7472 	.word	0x000a7472
3000cc70:	69746341 	.word	0x69746341
3000cc74:	49206576 	.word	0x49206576
3000cc78:	7865646e 	.word	0x7865646e
3000cc7c:	6425203a 	.word	0x6425203a
3000cc80:	0000000a 	.word	0x0000000a
3000cc84:	3141544f 	.word	0x3141544f
3000cc88:	766e6920 	.word	0x766e6920
3000cc8c:	64696c61 	.word	0x64696c61
3000cc90:	0000000a 	.word	0x0000000a
3000cc94:	3241544f 	.word	0x3241544f
3000cc98:	766e6920 	.word	0x766e6920
3000cc9c:	64696c61 	.word	0x64696c61
3000cca0:	0000000a 	.word	0x0000000a
3000cca4:	3141544f 	.word	0x3141544f
3000cca8:	43524320 	.word	0x43524320
3000ccac:	6d203233 	.word	0x6d203233
3000ccb0:	68637461 	.word	0x68637461
3000ccb4:	7461202c 	.word	0x7461202c
3000ccb8:	68636174 	.word	0x68636174
3000ccbc:	43206465 	.word	0x43206465
3000ccc0:	203a4352 	.word	0x203a4352
3000ccc4:	78257830 	.word	0x78257830
3000ccc8:	6163202c 	.word	0x6163202c
3000cccc:	6c75636c 	.word	0x6c75636c
3000ccd0:	64657461 	.word	0x64657461
3000ccd4:	43524320 	.word	0x43524320
3000ccd8:	7830203a 	.word	0x7830203a
3000ccdc:	000a7825 	.word	0x000a7825
3000cce0:	3141544f 	.word	0x3141544f
3000cce4:	43524320 	.word	0x43524320
3000cce8:	6e203233 	.word	0x6e203233
3000ccec:	6d20746f 	.word	0x6d20746f
3000ccf0:	68637461 	.word	0x68637461
3000ccf4:	7461202c 	.word	0x7461202c
3000ccf8:	68636174 	.word	0x68636174
3000ccfc:	43206465 	.word	0x43206465
3000cd00:	203a4352 	.word	0x203a4352
3000cd04:	78257830 	.word	0x78257830
3000cd08:	6163202c 	.word	0x6163202c
3000cd0c:	6c75636c 	.word	0x6c75636c
3000cd10:	64657461 	.word	0x64657461
3000cd14:	43524320 	.word	0x43524320
3000cd18:	7830203a 	.word	0x7830203a
3000cd1c:	202c7825 	.word	0x202c7825
3000cd20:	73617265 	.word	0x73617265
3000cd24:	544f2065 	.word	0x544f2065
3000cd28:	000a3141 	.word	0x000a3141
3000cd2c:	3241544f 	.word	0x3241544f
3000cd30:	43524320 	.word	0x43524320
3000cd34:	6d203233 	.word	0x6d203233
3000cd38:	68637461 	.word	0x68637461
3000cd3c:	7461202c 	.word	0x7461202c
3000cd40:	68636174 	.word	0x68636174
3000cd44:	43206465 	.word	0x43206465
3000cd48:	203a4352 	.word	0x203a4352
3000cd4c:	78257830 	.word	0x78257830
3000cd50:	6163202c 	.word	0x6163202c
3000cd54:	6c75636c 	.word	0x6c75636c
3000cd58:	64657461 	.word	0x64657461
3000cd5c:	43524320 	.word	0x43524320
3000cd60:	7830203a 	.word	0x7830203a
3000cd64:	000a7825 	.word	0x000a7825
3000cd68:	3241544f 	.word	0x3241544f
3000cd6c:	43524320 	.word	0x43524320
3000cd70:	6e203233 	.word	0x6e203233
3000cd74:	6d20746f 	.word	0x6d20746f
3000cd78:	68637461 	.word	0x68637461
3000cd7c:	7461202c 	.word	0x7461202c
3000cd80:	68636174 	.word	0x68636174
3000cd84:	43206465 	.word	0x43206465
3000cd88:	203a4352 	.word	0x203a4352
3000cd8c:	78257830 	.word	0x78257830
3000cd90:	6163202c 	.word	0x6163202c
3000cd94:	6c75636c 	.word	0x6c75636c
3000cd98:	64657461 	.word	0x64657461
3000cd9c:	43524320 	.word	0x43524320
3000cda0:	7830203a 	.word	0x7830203a
3000cda4:	202c7825 	.word	0x202c7825
3000cda8:	73617265 	.word	0x73617265
3000cdac:	544f2065 	.word	0x544f2065
3000cdb0:	000a3241 	.word	0x000a3241
3000cdb4:	69746341 	.word	0x69746341
3000cdb8:	69206576 	.word	0x69206576
3000cdbc:	7865646e 	.word	0x7865646e
3000cdc0:	41544f20 	.word	0x41544f20
3000cdc4:	766e6920 	.word	0x766e6920
3000cdc8:	64696c61 	.word	0x64696c61
3000cdcc:	6572202c 	.word	0x6572202c
3000cdd0:	74697277 	.word	0x74697277
3000cdd4:	63612065 	.word	0x63612065
3000cdd8:	65766974 	.word	0x65766974
3000cddc:	646e6920 	.word	0x646e6920
3000cde0:	74207865 	.word	0x74207865
3000cde4:	6425206f 	.word	0x6425206f
3000cde8:	0000000a 	.word	0x0000000a
3000cdec:	206c6c41 	.word	0x206c6c41
3000cdf0:	2041544f 	.word	0x2041544f
3000cdf4:	72646461 	.word	0x72646461
3000cdf8:	65737365 	.word	0x65737365
3000cdfc:	6f6e2073 	.word	0x6f6e2073
3000ce00:	61762074 	.word	0x61762074
3000ce04:	2c64696c 	.word	0x2c64696c
3000ce08:	63657220 	.word	0x63657220
3000ce0c:	7265766f 	.word	0x7265766f
3000ce10:	61662079 	.word	0x61662079
3000ce14:	000a6c69 	.word	0x000a6c69
3000ce18:	20325042 	.word	0x20325042
3000ce1c:	3261746f 	.word	0x3261746f
3000ce20:	64646120 	.word	0x64646120
3000ce24:	73736572 	.word	0x73736572
3000ce28:	766e6920 	.word	0x766e6920
3000ce2c:	64696c61 	.word	0x64696c61
3000ce30:	0000000a 	.word	0x0000000a
3000ce34:	20325042 	.word	0x20325042
3000ce38:	73726576 	.word	0x73726576
3000ce3c:	206e6f69 	.word	0x206e6f69
3000ce40:	61766e69 	.word	0x61766e69
3000ce44:	0a64696c 	.word	0x0a64696c
3000ce48:	00000000 	.word	0x00000000
3000ce4c:	20325042 	.word	0x20325042
3000ce50:	69746361 	.word	0x69746361
3000ce54:	69206576 	.word	0x69206576
3000ce58:	7865646e 	.word	0x7865646e
3000ce5c:	766e6920 	.word	0x766e6920
3000ce60:	64696c61 	.word	0x64696c61
3000ce64:	0000000a 	.word	0x0000000a
3000ce68:	20325042 	.word	0x20325042
3000ce6c:	3161746f 	.word	0x3161746f
3000ce70:	64646120 	.word	0x64646120
3000ce74:	73736572 	.word	0x73736572
3000ce78:	766e6920 	.word	0x766e6920
3000ce7c:	64696c61 	.word	0x64696c61
3000ce80:	0000000a 	.word	0x0000000a
3000ce84:	20325042 	.word	0x20325042
3000ce88:	61746164 	.word	0x61746164
3000ce8c:	6c617620 	.word	0x6c617620
3000ce90:	202c6469 	.word	0x202c6469
3000ce94:	73726576 	.word	0x73726576
3000ce98:	3a6e6f69 	.word	0x3a6e6f69
3000ce9c:	000a6425 	.word	0x000a6425
3000cea0:	2d73255b 	.word	0x2d73255b
3000cea4:	44205d49 	.word	0x44205d49
3000cea8:	62617369 	.word	0x62617369
3000ceac:	4b20656c 	.word	0x4b20656c
3000ceb0:	4220304d 	.word	0x4220304d
3000ceb4:	20746f6f 	.word	0x20746f6f
3000ceb8:	206e6920 	.word	0x206e6920
3000cebc:	74736554 	.word	0x74736554
3000cec0:	646f6d20 	.word	0x646f6d20
3000cec4:	000a0d65 	.word	0x000a0d65
3000cec8:	2d73255b 	.word	0x2d73255b
3000cecc:	42205d49 	.word	0x42205d49
3000ced0:	73617079 	.word	0x73617079
3000ced4:	544f2073 	.word	0x544f2073
3000ced8:	75612050 	.word	0x75612050
3000cedc:	6f6c6f74 	.word	0x6f6c6f74
3000cee0:	0a0d6461 	.word	0x0a0d6461
3000cee4:	00000000 	.word	0x00000000
3000cee8:	2d73255b 	.word	0x2d73255b
3000ceec:	4e205d49 	.word	0x4e205d49
3000cef0:	72462050 	.word	0x72462050
3000cef4:	25207165 	.word	0x25207165
3000cef8:	484d2064 	.word	0x484d2064
3000cefc:	00000a7a 	.word	0x00000a7a
3000cf00:	2d73255b 	.word	0x2d73255b
3000cf04:	41205d49 	.word	0x41205d49
3000cf08:	72462050 	.word	0x72462050
3000cf0c:	25207165 	.word	0x25207165
3000cf10:	484d2064 	.word	0x484d2064
3000cf14:	00000a7a 	.word	0x00000a7a
3000cf18:	7361700a 	.word	0x7361700a
3000cf1c:	726f7773 	.word	0x726f7773
3000cf20:	65762064 	.word	0x65762064
3000cf24:	79666972 	.word	0x79666972
3000cf28:	69616620 	.word	0x69616620
3000cf2c:	00000a6c 	.word	0x00000a6c
3000cf30:	7361700a 	.word	0x7361700a
3000cf34:	726f7773 	.word	0x726f7773
3000cf38:	65762064 	.word	0x65762064
3000cf3c:	79666972 	.word	0x79666972
3000cf40:	63757320 	.word	0x63757320
3000cf44:	73736563 	.word	0x73736563
3000cf48:	0000000a 	.word	0x0000000a
3000cf4c:	31474d49 	.word	0x31474d49
3000cf50:	4f425320 	.word	0x4f425320
3000cf54:	4f20544f 	.word	0x4f20544f
3000cf58:	000a4646 	.word	0x000a4646
3000cf5c:	31474d49 	.word	0x31474d49
3000cf60:	4f425320 	.word	0x4f425320
3000cf64:	5320544f 	.word	0x5320544f
3000cf68:	45434355 	.word	0x45434355
3000cf6c:	000a5353 	.word	0x000a5353
3000cf70:	746f6f62 	.word	0x746f6f62
3000cf74:	64616f6c 	.word	0x64616f6c
3000cf78:	765f7265 	.word	0x765f7265
3000cf7c:	69737265 	.word	0x69737265
3000cf80:	25206e6f 	.word	0x25206e6f
3000cf84:	00000a73 	.word	0x00000a73
3000cf88:	444f4d5b 	.word	0x444f4d5b
3000cf8c:	5f454c55 	.word	0x5f454c55
3000cf90:	544f4f42 	.word	0x544f4f42
3000cf94:	56454c2d 	.word	0x56454c2d
3000cf98:	495f4c45 	.word	0x495f4c45
3000cf9c:	5d4f464e 	.word	0x5d4f464e
3000cfa0:	474d493a 	.word	0x474d493a
3000cfa4:	4e452031 	.word	0x4e452031
3000cfa8:	20524554 	.word	0x20524554
3000cfac:	3a50534d 	.word	0x3a50534d
3000cfb0:	3830255b 	.word	0x3830255b
3000cfb4:	000a5d78 	.word	0x000a5d78
3000cfb8:	444f4d5b 	.word	0x444f4d5b
3000cfbc:	5f454c55 	.word	0x5f454c55
3000cfc0:	544f4f42 	.word	0x544f4f42
3000cfc4:	56454c2d 	.word	0x56454c2d
3000cfc8:	495f4c45 	.word	0x495f4c45
3000cfcc:	5d4f464e 	.word	0x5d4f464e
3000cfd0:	474d493a 	.word	0x474d493a
3000cfd4:	45532031 	.word	0x45532031
3000cfd8:	45525543 	.word	0x45525543
3000cfdc:	41545320 	.word	0x41545320
3000cfe0:	203a4554 	.word	0x203a4554
3000cfe4:	000a6425 	.word	0x000a6425
3000cfe8:	2d73255b 	.word	0x2d73255b
3000cfec:	49205d49 	.word	0x49205d49
3000cff0:	2074696e 	.word	0x2074696e
3000cff4:	41525350 	.word	0x41525350
3000cff8:	000a0d4d 	.word	0x000a0d4d
3000cffc:	2d73255b 	.word	0x2d73255b
3000d000:	73205d57 	.word	0x73205d57
3000d004:	70207465 	.word	0x70207465
3000d008:	66206d66 	.word	0x66206d66
3000d00c:	0d6c6961 	.word	0x0d6c6961
3000d010:	0000000a 	.word	0x0000000a
3000d014:	2d73255b 	.word	0x2d73255b
3000d018:	45205d45 	.word	0x45205d45
3000d01c:	524f5252 	.word	0x524f5252
3000d020:	53202121 	.word	0x53202121
3000d024:	6c756f68 	.word	0x6c756f68
3000d028:	6f4e2064 	.word	0x6f4e2064
3000d02c:	6e652074 	.word	0x6e652074
3000d030:	656c6261 	.word	0x656c6261
3000d034:	6d654d20 	.word	0x6d654d20
3000d038:	5f727753 	.word	0x5f727753
3000d03c:	796c6e4f 	.word	0x796c6e4f
3000d040:	206e6920 	.word	0x206e6920
3000d044:	20524444 	.word	0x20524444
3000d048:	70696843 	.word	0x70696843
3000d04c:	0d212121 	.word	0x0d212121
3000d050:	0000000a 	.word	0x0000000a
3000d054:	444f4d5b 	.word	0x444f4d5b
3000d058:	5f454c55 	.word	0x5f454c55
3000d05c:	544f4f42 	.word	0x544f4f42
3000d060:	56454c2d 	.word	0x56454c2d
3000d064:	495f4c45 	.word	0x495f4c45
3000d068:	5d4f464e 	.word	0x5d4f464e
3000d06c:	696e493a 	.word	0x696e493a
3000d070:	44442074 	.word	0x44442074
3000d074:	000a0d52 	.word	0x000a0d52
3000d078:	55676f4c 	.word	0x55676f4c
3000d07c:	20747261 	.word	0x20747261
3000d080:	64756142 	.word	0x64756142
3000d084:	65746172 	.word	0x65746172
3000d088:	6425203a 	.word	0x6425203a
3000d08c:	0000000a 	.word	0x0000000a
3000d090:	4848530a 	.word	0x4848530a
3000d094:	4946544f 	.word	0x4946544f
3000d098:	6f6e2058 	.word	0x6f6e2058
3000d09c:	65722074 	.word	0x65722074
3000d0a0:	76696563 	.word	0x76696563
3000d0a4:	000a6465 	.word	0x000a6465
3000d0a8:	726f4e0a 	.word	0x726f4e0a
3000d0ac:	206c616d 	.word	0x206c616d
3000d0b0:	746f6f62 	.word	0x746f6f62
3000d0b4:	0000000a 	.word	0x0000000a
3000d0b8:	5f6e6962 	.word	0x5f6e6962
3000d0bc:	313e6f6e 	.word	0x313e6f6e
3000d0c0:	3d202c30 	.word	0x3d202c30
3000d0c4:	0a642520 	.word	0x0a642520
3000d0c8:	00000000 	.word	0x00000000
3000d0cc:	6e206164 	.word	0x6e206164
3000d0d0:	7220746f 	.word	0x7220746f
3000d0d4:	69656365 	.word	0x69656365
3000d0d8:	0a646576 	.word	0x0a646576
3000d0dc:	00000000 	.word	0x00000000
3000d0e0:	746f680a 	.word	0x746f680a
3000d0e4:	20786966 	.word	0x20786966
3000d0e8:	656e6f64 	.word	0x656e6f64
3000d0ec:	202e2e2e 	.word	0x202e2e2e
3000d0f0:	6f626552 	.word	0x6f626552
3000d0f4:	6e69746f 	.word	0x6e69746f
3000d0f8:	0a2e2e67 	.word	0x0a2e2e67
3000d0fc:	00000000 	.word	0x00000000
3000d100:	6c630a0d 	.word	0x6c630a0d
3000d104:	2065736f 	.word	0x2065736f
3000d108:	646f4d79 	.word	0x646f4d79
3000d10c:	54206d65 	.word	0x54206d65
3000d110:	736e6172 	.word	0x736e6172
3000d114:	20726566 	.word	0x20726566
3000d118:	0d2e2e2e 	.word	0x0d2e2e2e
3000d11c:	0000000a 	.word	0x0000000a
3000d120:	50494843 	.word	0x50494843
3000d124:	4f464e49 	.word	0x4f464e49
3000d128:	00000000 	.word	0x00000000
3000d12c:	2d73255b 	.word	0x2d73255b
3000d130:	49205d45 	.word	0x49205d45
3000d134:	6c61766e 	.word	0x6c61766e
3000d138:	43206469 	.word	0x43206469
3000d13c:	696e6968 	.word	0x696e6968
3000d140:	216f666e 	.word	0x216f666e
3000d144:	65684320 	.word	0x65684320
3000d148:	4f206b63 	.word	0x4f206b63
3000d14c:	000a5054 	.word	0x000a5054
3000d150:	2d73255b 	.word	0x2d73255b
3000d154:	50205d57 	.word	0x50205d57
3000d158:	7361656c 	.word	0x7361656c
3000d15c:	72702065 	.word	0x72702065
3000d160:	6172676f 	.word	0x6172676f
3000d164:	6863206d 	.word	0x6863206d
3000d168:	6e697069 	.word	0x6e697069
3000d16c:	69206f66 	.word	0x69206f66
3000d170:	544f206e 	.word	0x544f206e
3000d174:	0a212050 	.word	0x0a212050
3000d178:	00000000 	.word	0x00000000

3000d17c <Memory_Info>:
3000d17c:	03e00007 00060004 010202c0 02010100     ................
3000d18c:	02000102 00040302 02040400 01010102     ................
3000d19c:	01020221 02240401 03000102 00030103     !.....$.........
3000d1ac:	01050500 06000002 01020206 01070700     ................
3000d1bc:	08000002 01020208 02280801 09000102     ..........(.....
3000d1cc:	00040309 03290901 0a000004 0003010a     ......).........
3000d1dc:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d1ec:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d1fc:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d20c:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d21c:	62696c61 6f696172 000a0d6e              alibraion...

3000d228 <ddrphy_tx_scan>:
3000d228:	00000000 00080000 00080808 00000000     ................
3000d238:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d248:	07060012 000f0707 080f0f0f 00000008     ................
3000d258:	20000001 22222222 000c0012 34000000     ... """".......4
3000d268:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d278:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d288:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d298:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d2a8:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d2b8:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d2c8:	0e0e001a 00170e0e 12171717 00000012     ................
3000d2d8:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d2e8:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d2f8:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d308:	0f0f001b 00180f0f 13181818 00000013     ................
3000d318:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d328:	1110001b 00191111 13191919 00000013     ................
3000d338:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d348:	1313001c 001c1313 121c1c1c 00000012     ................
3000d358:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d368 <ddrphy_zq_rx_scan>:
	...
3000d370:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d380:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d390:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d3a0:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d3b0:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d3c0:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d3d0:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d3e0:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d3f0:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d400:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d410:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d420:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d430 <Flash_AVL>:
3000d430:	000000ef 000000ff 00000000 000043fc     .............C..
3000d440:	00000000 000000a1 000000ff 00000000     ................
3000d450:	0000fffc 00000000 0000000b 000000ff     ................
3000d460:	00000000 000043fc 00000000 0000000e     .....C..........
3000d470:	000000ff 00000000 000043fc 00000000     .........C......
3000d480:	000000c8 000000ff 00000001 000043fc     .............C..
3000d490:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d4a0:	000200fc 00000000 000000c2 000000ff     ................
3000d4b0:	00000002 000000fc 00000000 00000068     ............h...
3000d4c0:	000000ff 00000002 000000fc 00000000     ................
3000d4d0:	00000051 000000ff 00000002 000000fc     Q...............
3000d4e0:	00000000 0000001c 000000ff 00000003     ................
3000d4f0:	000000fc 00000000 00000020 000000ff     ........ .......
3000d500:	00000004 000000fc 00000000 00000000     ................
3000d510:	000000ff 000000fe ffffffff 300085a5     ...............0
3000d520:	000000ff ffffffff 000000ff ffffffff     ................
3000d530:	00000000                                ....

3000d534 <Flash_ReadMode>:
3000d534:	                                         ..

3000d536 <Flash_Speed>:
3000d536:	                                         ..

3000d538 <NAND_AVL>:
3000d538:	000000ef 000000ff 00000000 ffffffff     ................
3000d548:	00000000 000000c8 000000ff 00000001     ................
3000d558:	ffffffff 00000000 000000e5 000000ff     ................
3000d568:	00000001 ffffffff 00000000 000000c2     ................
3000d578:	000000ff 00000002 ffffffff 00000000     ................
3000d588:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d5a0:	000000ff 000000fe ffffffff 30008605     ...............0
3000d5b0:	000000ff ffffffff 000000ff ffffffff     ................
3000d5c0:	00000000 3000c4b8 3000c4bc 3000c4c0     .......0...0...0
3000d5d0:	3000c4c4 3000c4c8                       ...0...0

3000d5d8 <__FUNCTION__.0>:
3000d5d8:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d5e8 <__FUNCTION__.1>:
3000d5e8:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d5f8:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000d608:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000d618:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000d628:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000d638:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000d648:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000d658:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000d668:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d678:	00000a20                                 ...

3000d67c <__FUNCTION__.1>:
3000d67c:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d68c:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000d69c:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000d6ac:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000d6bc:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d6cc:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d6dc:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d6ec:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d6fc:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d70c:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d71c:	726f6620 41485320 00000a32 2d73255b      for SHA2...[%s-
3000d72c:	69205d57 255b7172 70205d64 726f6972     W] irq[%d] prior
3000d73c:	20797469 73206425 6c6c6168 203d3c20     ity %d shall <= 
3000d74c:	000a6425                                %d..

3000d750 <__FUNCTION__.0>:
3000d750:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d760:	00000000                                ....

3000d764 <__FUNCTION__.1>:
3000d764:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d774 <__FUNCTION__.3>:
3000d774:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d784:	32616873 696e695f 00000074              sha2_init...

3000d790 <__FUNCTION__.4>:
3000d790:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d7a0:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000d7b0:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000d7c0:	6e207369 6320746f 69666e6f 65727567     is not configure
3000d7d0:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000d7e0:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000d7f0:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000d800:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000d810:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000d820:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000d830:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000d840:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000d850:	69625f6b 21217374 00000a21              k_bits!!!...

3000d85c <__FUNCTION__.1>:
3000d85c:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d86c:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000d87c:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000d88c:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000d89c:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000d8ac:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000d8bc:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000d8cc:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000d8dc:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000d8ec:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000d8fc:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000d90c:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000d91c:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000d92c:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000d93c:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000d94c:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000d95c:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000d96c:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000d97c:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000d98c:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000d99c:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000d9ac:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000d9bc:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000d9cc:	00000000                                ....

3000d9d0 <register_string>:
3000d9d0:	00000000 3000d8f8 3000c5d8 3000c5dc     .......0...0...0
3000d9e0:	3000c5e0 3000c5e4 3000c5e8 3000c5ec     ...0...0...0...0
3000d9f0:	3000c5f0 3000d8f4 3000c5f4 3000c5f8     ...0...0...0...0
3000da00:	3000c5fc 3000d998 65637845 6f697470     ...0...0Exceptio
3000da10:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000da20:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000da30:	64657375 20796220 6c696166 76206465     used by failed v
3000da40:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000da50:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000da60:	00000000 75636553 79746972 75616620     ....Security fau
3000da70:	6920746c 61632073 64657375 20796220     lt is caused by 
3000da80:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000da90:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000daa0:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000dab0:	79746972 75616620 6920746c 61632073     rity fault is ca
3000dac0:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000dad0:	65746e69 74697267 69732079 74616e67     integrity signat
3000dae0:	00657275 75636553 79746972 75616620     ure.Security fau
3000daf0:	6920746c 61632073 64657375 20796220     lt is caused by 
3000db00:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000db10:	6572206e 6e727574 00000000 75636553     n return....Secu
3000db20:	79746972 75616620 6920746c 61632073     rity fault is ca
3000db30:	64657375 20796220 72747441 74756269     used by Attribut
3000db40:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000db50:	00006e6f 75636553 79746972 75616620     on..Security fau
3000db60:	6920746c 61632073 64657375 20796220     lt is caused by 
3000db70:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000db80:	00006e6f 75636553 79746972 75616620     on..Security fau
3000db90:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dba0:	797a614c 61747320 70206574 65736572     Lazy state prese
3000dbb0:	74617672 206e6f69 6f727265 00000072     rvation error...
3000dbc0:	75636553 79746972 75616620 6920746c     Security fault i
3000dbd0:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000dbe0:	61747320 65206574 726f7272 00000000      state error....
3000dbf0:	20656854 75636573 79746972 75616620     The security fau
3000dc00:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000dc10:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000dc20:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000dc30:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000dc40:	61662074 20746c75 63207369 65737561     t fault is cause
3000dc50:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000dc60:	63636120 20737365 6c6f6976 6f697461      access violatio
3000dc70:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000dc80:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000dc90:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000dca0:	20746c75 63207369 65737561 79622064     ult is caused by
3000dcb0:	74616420 63612061 73736563 6f697620      data access vio
3000dcc0:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000dcd0:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000dce0:	63207369 65737561 79622064 736e7520     is caused by uns
3000dcf0:	6b636174 20676e69 6f727265 00000072     tacking error...
3000dd00:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000dd10:	61662074 20746c75 63207369 65737561     t fault is cause
3000dd20:	79622064 61747320 6e696b63 72652067     d by stacking er
3000dd30:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000dd40:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000dd50:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000dd60:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000dd70:	70206574 65736572 74617672 006e6f69     te preservation.
3000dd80:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000dd90:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000dda0:	64657272 64646120 73736572 00736920     rred address is.
3000ddb0:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000ddc0:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000ddd0:	73692074 75616320 20646573 69207962     t is caused by i
3000dde0:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000ddf0:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000de00:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000de10:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000de20:	75616320 20646573 70207962 69636572      caused by preci
3000de30:	64206573 20617461 65636361 76207373     se data access v
3000de40:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000de50:	6c756166 73692074 75616320 20646573     fault is caused 
3000de60:	69207962 6572706d 65736963 74616420     by imprecise dat
3000de70:	63612061 73736563 6f697620 6974616c     a access violati
3000de80:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000de90:	75616320 20646573 75207962 6174736e      caused by unsta
3000dea0:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000deb0:	6c756166 73692074 75616320 20646573     fault is caused 
3000dec0:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000ded0:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000dee0:	75616320 20646573 66207962 74616f6c      caused by float
3000def0:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000df00:	65746174 65727020 76726573 6f697461     tate preservatio
3000df10:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000df20:	636f2074 72727563 61206465 65726464     t occurred addre
3000df30:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000df40:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000df50:	67617355 61662065 20746c75 63207369     Usage fault is c
3000df60:	65737561 79622064 74746120 74706d65     aused by attempt
3000df70:	6f742073 65786520 65747563 206e6120     s to execute an 
3000df80:	65646e75 656e6966 6e692064 75727473     undefined instru
3000df90:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000dfa0:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000dfb0:	67617355 61662065 20746c75 63207369     Usage fault is c
3000dfc0:	65737561 79622064 74746120 74706d65     aused by attempt
3000dfd0:	6f742073 69777320 20686374 61206f74     s to switch to a
3000dfe0:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000dff0:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e000:	61662065 20746c75 63207369 65737561     e fault is cause
3000e010:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e020:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e030:	74697720 20612068 20646162 756c6176      with a bad valu
3000e040:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e050:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e060:	61662065 20746c75 63207369 65737561     e fault is cause
3000e070:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e080:	65786520 65747563 63206120 6f72706f      execute a copro
3000e090:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e0a0:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e0b0:	63207369 65737561 79622064 646e6920     is caused by ind
3000e0c0:	74616369 74207365 20746168 74732061     icates that a st
3000e0d0:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e0e0:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e0f0:	61742073 206e656b 63616c70 00000065     s taken place...
3000e100:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e110:	65737561 79622064 646e6920 74616369     aused by indicat
3000e120:	74207365 20746168 75206e61 696c616e     es that an unali
3000e130:	64656e67 63636120 20737365 6c756166     gned access faul
3000e140:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e150:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e160:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e170:	74616369 61207365 76696420 20656469     icates a divide 
3000e180:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e190:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e1a0:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e1b0:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e1c0:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e1d0:	65737561 79622064 6c616820 65722074     aused by halt re
3000e1e0:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e1f0:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e200:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e210:	20746c75 63207369 65737561 79622064     ult is caused by
3000e220:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e230:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e240:	61662067 20746c75 63207369 65737561     g fault is cause
3000e250:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e260:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e270:	20746c75 63207369 65737561 79622064     ult is caused by
3000e280:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e290:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e2a0:	20746c75 63207369 65737561 79622064     ult is caused by
3000e2b0:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e2c0:	72657373 00646574 3000c600 3000c60c     sserted....0...0
3000e2d0:	3000c618 3000c624 3000c630 3000c63c     ...0$..00..0<..0
3000e2e0:	3000c648 3000c654 3000c660 3000c66c     H..0T..0`..0l..0
3000e2f0:	3000c81c 3000c828 3000c830 3000c838     ...0(..00..08..0
3000e300:	3000c820 3000c840 00000208 0000020c      ..0@..0........
3000e310:	00000210 00000008 00000214 00000218     ................
3000e320:	0000021c 0000000c                       ........

3000e328 <__FUNCTION__.0>:
3000e328:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.

3000e338 <km4_bootloader_rev>:
3000e338:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e348:	655f7265 64313063 62653231 30325f65     er_ec01d12ebe_20
3000e358:	302f3432 36312f37 3a30312d 343a3031     24/07/16-10:10:4
3000e368:	4f420039 5b00544f 572d7325 2a2a205d     9.BOOT.[%s-W] **
3000e378:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e388:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000e398:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000e3a8:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000e3b8:	20202a20 20202020 20202020 20202020      *              
3000e3c8:	20202020 20202020 20202020 20202020                     
3000e3d8:	20202020 20202020 20202020 20202020                     
3000e3e8:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000e3f8:	205d572d 2020202a 20202020 20202020     -W] *           
3000e408:	75432020 6e657272 68632074 76207069       Current chip v
3000e418:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000e428:	20202021 20202020 20202020 000a2a20     !            *..
3000e438:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000e448:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000e458:	2074726f 72727563 20746e65 73726576     ort current vers
3000e468:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000e478:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000e488:	656c5020 20657361 746e6f63 20746361      Please contact 
3000e498:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000e4a8:	646f7270 20746375 6f666e69 20202021     product info!   
3000e4b8:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000e4c8:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e4d8:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e4e8:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e4f8:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000e508:	0043000a 49534f50 002e0058              ..C.POSIX...

3000e514 <_ctype_>:
3000e514:	20202000 20202020 28282020 20282828     .         ((((( 
3000e524:	20202020 20202020 20202020 20202020                     
3000e534:	10108820 10101010 10101010 10101010      ...............
3000e544:	04040410 04040404 10040404 10101010     ................
3000e554:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000e564:	01010101 01010101 01010101 10101010     ................
3000e574:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000e584:	02020202 02020202 02020202 10101010     ................
3000e594:	00000020 00000000 00000000 00000000      ...............
	...
