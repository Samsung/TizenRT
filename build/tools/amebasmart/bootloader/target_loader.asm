
/home/sin/Desktop/tizenrt_8730e/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
300031dc:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
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
300032c4:	30009c15 	.word	0x30009c15
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	30009a1d 	.word	0x30009a1d
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
30003332:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
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
300033fc:	30009c15 	.word	0x30009c15
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	30009a1d 	.word	0x30009a1d
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
3000349c:	f008 fd56 	bl	3000bf4c <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000c5dc 	.word	0x3000c5dc
300034bc:	3000c5e8 	.word	0x3000c5e8

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
300034f0:	f008 fd2c 	bl	3000bf4c <rtk_log_write>
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
30003524:	f008 fd12 	bl	3000bf4c <rtk_log_write>
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
30003540:	3000c5dc 	.word	0x3000c5dc
30003544:	3000c62c 	.word	0x3000c62c
30003548:	30009c15 	.word	0x30009c15
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000c60c 	.word	0x3000c60c
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
3000365a:	f008 fc77 	bl	3000bf4c <rtk_log_write>
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
3000367a:	f008 fc67 	bl	3000bf4c <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000d6fc 	.word	0x3000d6fc
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000c5dc 	.word	0x3000c5dc
300036a8:	3000c664 	.word	0x3000c664
300036ac:	3000c64c 	.word	0x3000c64c
300036b0:	30009c15 	.word	0x30009c15
300036b4:	30009a1d 	.word	0x30009a1d
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
3000373e:	f008 fc05 	bl	3000bf4c <rtk_log_write>
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
30003790:	f008 fbdc 	bl	3000bf4c <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 fecd 	bl	3000c538 <__io_assert_failed_veneer>
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
300037ca:	f008 fbbf 	bl	3000bf4c <rtk_log_write>
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
30003816:	f008 fb99 	bl	3000bf4c <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 fe8a 	bl	3000c538 <__io_assert_failed_veneer>
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
30003958:	3000d66c 	.word	0x3000d66c
3000395c:	3000d66e 	.word	0x3000d66e
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000c5dc 	.word	0x3000c5dc
3000396c:	3000c688 	.word	0x3000c688
30003970:	3000d568 	.word	0x3000d568
30003974:	3000d670 	.word	0x3000d670
30003978:	3000c6a4 	.word	0x3000c6a4
3000397c:	3000d720 	.word	0x3000d720
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000c6d0 	.word	0x3000c6d0
30003988:	3000d710 	.word	0x3000d710
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000fcf1 	.word	0x3000fcf1
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30009c15 	.word	0x30009c15
300039b8:	3000fcf4 	.word	0x3000fcf4
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
30003a36:	f008 fd7f 	bl	3000c538 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 fd76 	bl	3000c538 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000fcf1 	.word	0x3000fcf1
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000d710 	.word	0x3000d710
30003a78:	3000d720 	.word	0x3000d720
30003a7c:	0000a4c1 	.word	0x0000a4c1

30003a80 <BOOT_INT_SecureFault>:
30003a80:	f3ef 8008 	mrs	r0, MSP
30003a84:	f3ef 8109 	mrs	r1, PSP
30003a88:	4672      	mov	r2, lr
30003a8a:	f04f 0304 	mov.w	r3, #4
30003a8e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30003a92:	f000 b895 	b.w	30003bc0 <BOOT_FaultHandler>
30003a96:	4770      	bx	lr

30003a98 <AsciiToHex>:
30003a98:	f1a0 0341 	sub.w	r3, r0, #65	; 0x41
30003a9c:	2b05      	cmp	r3, #5
30003a9e:	d802      	bhi.n	30003aa6 <AsciiToHex+0xe>
30003aa0:	3837      	subs	r0, #55	; 0x37
30003aa2:	b2c0      	uxtb	r0, r0
30003aa4:	4770      	bx	lr
30003aa6:	f1a0 0361 	sub.w	r3, r0, #97	; 0x61
30003aaa:	2b05      	cmp	r3, #5
30003aac:	d802      	bhi.n	30003ab4 <AsciiToHex+0x1c>
30003aae:	3857      	subs	r0, #87	; 0x57
30003ab0:	b2c0      	uxtb	r0, r0
30003ab2:	4770      	bx	lr
30003ab4:	3830      	subs	r0, #48	; 0x30
30003ab6:	b2c0      	uxtb	r0, r0
30003ab8:	280a      	cmp	r0, #10
30003aba:	bf28      	it	cs
30003abc:	20ff      	movcs	r0, #255	; 0xff
30003abe:	4770      	bx	lr

30003ac0 <hotfix_get_addr_size>:
30003ac0:	2800      	cmp	r0, #0
30003ac2:	d054      	beq.n	30003b6e <hotfix_get_addr_size+0xae>
30003ac4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30003ac8:	f04f 0a00 	mov.w	sl, #0
30003acc:	b083      	sub	sp, #12
30003ace:	f1a3 0b04 	sub.w	fp, r3, #4
30003ad2:	1c4b      	adds	r3, r1, #1
30003ad4:	4688      	mov	r8, r1
30003ad6:	4681      	mov	r9, r0
30003ad8:	9300      	str	r3, [sp, #0]
30003ada:	4617      	mov	r7, r2
30003adc:	1e53      	subs	r3, r2, #1
30003ade:	f101 0409 	add.w	r4, r1, #9
30003ae2:	1dd5      	adds	r5, r2, #7
30003ae4:	4656      	mov	r6, sl
30003ae6:	9301      	str	r3, [sp, #4]
30003ae8:	f8c8 6004 	str.w	r6, [r8, #4]
30003aec:	210b      	movs	r1, #11
30003aee:	f8c8 6000 	str.w	r6, [r8]
30003af2:	4640      	mov	r0, r8
30003af4:	f8c8 6007 	str.w	r6, [r8, #7]
30003af8:	603e      	str	r6, [r7, #0]
30003afa:	607e      	str	r6, [r7, #4]
30003afc:	f004 f852 	bl	30007ba4 <ymodem_uart_getdata>
30003b00:	e9dd 1000 	ldrd	r1, r0, [sp]
30003b04:	e004      	b.n	30003b10 <hotfix_get_addr_size+0x50>
30003b06:	b2d3      	uxtb	r3, r2
30003b08:	428c      	cmp	r4, r1
30003b0a:	f800 3f01 	strb.w	r3, [r0, #1]!
30003b0e:	d01a      	beq.n	30003b46 <hotfix_get_addr_size+0x86>
30003b10:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30003b14:	f1a2 0e41 	sub.w	lr, r2, #65	; 0x41
30003b18:	f1a2 0337 	sub.w	r3, r2, #55	; 0x37
30003b1c:	f1a2 0c61 	sub.w	ip, r2, #97	; 0x61
30003b20:	f1be 0f05 	cmp.w	lr, #5
30003b24:	b2db      	uxtb	r3, r3
30003b26:	d9ef      	bls.n	30003b08 <hotfix_get_addr_size+0x48>
30003b28:	f1a2 0330 	sub.w	r3, r2, #48	; 0x30
30003b2c:	f1bc 0f05 	cmp.w	ip, #5
30003b30:	f1a2 0257 	sub.w	r2, r2, #87	; 0x57
30003b34:	b2db      	uxtb	r3, r3
30003b36:	d9e6      	bls.n	30003b06 <hotfix_get_addr_size+0x46>
30003b38:	2b0a      	cmp	r3, #10
30003b3a:	bf28      	it	cs
30003b3c:	23ff      	movcs	r3, #255	; 0xff
30003b3e:	428c      	cmp	r4, r1
30003b40:	f800 3f01 	strb.w	r3, [r0, #1]!
30003b44:	d1e4      	bne.n	30003b10 <hotfix_get_addr_size+0x50>
30003b46:	4639      	mov	r1, r7
30003b48:	2300      	movs	r3, #0
30003b4a:	f811 2b01 	ldrb.w	r2, [r1], #1
30003b4e:	4313      	orrs	r3, r2
30003b50:	42a9      	cmp	r1, r5
30003b52:	ea4f 1303 	mov.w	r3, r3, lsl #4
30003b56:	d1f8      	bne.n	30003b4a <hotfix_get_addr_size+0x8a>
30003b58:	79fa      	ldrb	r2, [r7, #7]
30003b5a:	f10a 0a01 	add.w	sl, sl, #1
30003b5e:	4313      	orrs	r3, r2
30003b60:	45ca      	cmp	sl, r9
30003b62:	f84b 3f04 	str.w	r3, [fp, #4]!
30003b66:	d1bf      	bne.n	30003ae8 <hotfix_get_addr_size+0x28>
30003b68:	b003      	add	sp, #12
30003b6a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003b6e:	4770      	bx	lr

30003b70 <ascii_to_integer>:
30003b70:	2900      	cmp	r1, #0
30003b72:	4684      	mov	ip, r0
30003b74:	dd12      	ble.n	30003b9c <ascii_to_integer+0x2c>
30003b76:	3901      	subs	r1, #1
30003b78:	1e42      	subs	r2, r0, #1
30003b7a:	2000      	movs	r0, #0
30003b7c:	448c      	add	ip, r1
30003b7e:	e001      	b.n	30003b84 <ascii_to_integer+0x14>
30003b80:	4562      	cmp	r2, ip
30003b82:	d00d      	beq.n	30003ba0 <ascii_to_integer+0x30>
30003b84:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30003b88:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30003b8c:	f1a3 0130 	sub.w	r1, r3, #48	; 0x30
30003b90:	eb03 0040 	add.w	r0, r3, r0, lsl #1
30003b94:	2909      	cmp	r1, #9
30003b96:	f1a0 0030 	sub.w	r0, r0, #48	; 0x30
30003b9a:	d9f1      	bls.n	30003b80 <ascii_to_integer+0x10>
30003b9c:	2000      	movs	r0, #0
30003b9e:	4770      	bx	lr
30003ba0:	4770      	bx	lr
30003ba2:	bf00      	nop

30003ba4 <eight_4bit_to_int>:
30003ba4:	2300      	movs	r3, #0
30003ba6:	f100 0c07 	add.w	ip, r0, #7
30003baa:	4602      	mov	r2, r0
30003bac:	f812 1b01 	ldrb.w	r1, [r2], #1
30003bb0:	430b      	orrs	r3, r1
30003bb2:	4562      	cmp	r2, ip
30003bb4:	ea4f 1303 	mov.w	r3, r3, lsl #4
30003bb8:	d1f8      	bne.n	30003bac <eight_4bit_to_int+0x8>
30003bba:	79c0      	ldrb	r0, [r0, #7]
30003bbc:	4318      	orrs	r0, r3
30003bbe:	4770      	bx	lr

30003bc0 <BOOT_FaultHandler>:
30003bc0:	b5f0      	push	{r4, r5, r6, r7, lr}
30003bc2:	461f      	mov	r7, r3
30003bc4:	4b41      	ldr	r3, [pc, #260]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003bc6:	b095      	sub	sp, #84	; 0x54
30003bc8:	4616      	mov	r6, r2
30003bca:	4604      	mov	r4, r0
30003bcc:	4a40      	ldr	r2, [pc, #256]	; (30003cd0 <BOOT_FaultHandler+0x110>)
30003bce:	2001      	movs	r0, #1
30003bd0:	460d      	mov	r5, r1
30003bd2:	4619      	mov	r1, r3
30003bd4:	f008 f9ba 	bl	3000bf4c <rtk_log_write>
30003bd8:	2244      	movs	r2, #68	; 0x44
30003bda:	2100      	movs	r1, #0
30003bdc:	a803      	add	r0, sp, #12
30003bde:	f008 fbfb 	bl	3000c3d8 <____wrap_memset_veneer>
30003be2:	0672      	lsls	r2, r6, #25
30003be4:	4b39      	ldr	r3, [pc, #228]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003be6:	d55f      	bpl.n	30003ca8 <BOOT_FaultHandler+0xe8>
30003be8:	4619      	mov	r1, r3
30003bea:	4a3a      	ldr	r2, [pc, #232]	; (30003cd4 <BOOT_FaultHandler+0x114>)
30003bec:	2001      	movs	r0, #1
30003bee:	f008 f9ad 	bl	3000bf4c <rtk_log_write>
30003bf2:	f006 010c 	and.w	r1, r6, #12
30003bf6:	46a6      	mov	lr, r4
30003bf8:	f1a1 010c 	sub.w	r1, r1, #12
30003bfc:	fab1 f181 	clz	r1, r1
30003c00:	0949      	lsrs	r1, r1, #5
30003c02:	f1a4 0c24 	sub.w	ip, r4, #36	; 0x24
30003c06:	1f22      	subs	r2, r4, #4
30003c08:	a804      	add	r0, sp, #16
30003c0a:	9603      	str	r6, [sp, #12]
30003c0c:	f85c 3f04 	ldr.w	r3, [ip, #4]!
30003c10:	4594      	cmp	ip, r2
30003c12:	f840 3b04 	str.w	r3, [r0], #4
30003c16:	d1f9      	bne.n	30003c0c <BOOT_FaultHandler+0x4c>
30003c18:	2900      	cmp	r1, #0
30003c1a:	bf08      	it	eq
30003c1c:	4675      	moveq	r5, lr
30003c1e:	f10d 0c30 	add.w	ip, sp, #48	; 0x30
30003c22:	1f28      	subs	r0, r5, #4
30003c24:	f105 031c 	add.w	r3, r5, #28
30003c28:	f850 4f04 	ldr.w	r4, [r0, #4]!
30003c2c:	4283      	cmp	r3, r0
30003c2e:	f84c 4b04 	str.w	r4, [ip], #4
30003c32:	d1f9      	bne.n	30003c28 <BOOT_FaultHandler+0x68>
30003c34:	4c28      	ldr	r4, [pc, #160]	; (30003cd8 <BOOT_FaultHandler+0x118>)
30003c36:	aa03      	add	r2, sp, #12
30003c38:	4629      	mov	r1, r5
30003c3a:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30003c3c:	f007 feb0 	bl	3000b9a0 <crash_dump>
30003c40:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003c44:	4b21      	ldr	r3, [pc, #132]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c46:	2001      	movs	r0, #1
30003c48:	9100      	str	r1, [sp, #0]
30003c4a:	4a24      	ldr	r2, [pc, #144]	; (30003cdc <BOOT_FaultHandler+0x11c>)
30003c4c:	4619      	mov	r1, r3
30003c4e:	f008 f97d 	bl	3000bf4c <rtk_log_write>
30003c52:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003c56:	4b1d      	ldr	r3, [pc, #116]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c58:	2001      	movs	r0, #1
30003c5a:	9200      	str	r2, [sp, #0]
30003c5c:	4619      	mov	r1, r3
30003c5e:	4a20      	ldr	r2, [pc, #128]	; (30003ce0 <BOOT_FaultHandler+0x120>)
30003c60:	f008 f974 	bl	3000bf4c <rtk_log_write>
30003c64:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003c66:	4b19      	ldr	r3, [pc, #100]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c68:	2001      	movs	r0, #1
30003c6a:	9100      	str	r1, [sp, #0]
30003c6c:	4a1d      	ldr	r2, [pc, #116]	; (30003ce4 <BOOT_FaultHandler+0x124>)
30003c6e:	4619      	mov	r1, r3
30003c70:	f008 f96c 	bl	3000bf4c <rtk_log_write>
30003c74:	f3ef 8488 	mrs	r4, MSP_NS
30003c78:	f3ef 8589 	mrs	r5, PSP_NS
30003c7c:	2101      	movs	r1, #1
30003c7e:	4b1a      	ldr	r3, [pc, #104]	; (30003ce8 <BOOT_FaultHandler+0x128>)
30003c80:	2002      	movs	r0, #2
30003c82:	4798      	blx	r3
30003c84:	4b19      	ldr	r3, [pc, #100]	; (30003cec <BOOT_FaultHandler+0x12c>)
30003c86:	2002      	movs	r0, #2
30003c88:	4798      	blx	r3
30003c8a:	4b10      	ldr	r3, [pc, #64]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c8c:	9000      	str	r0, [sp, #0]
30003c8e:	2001      	movs	r0, #1
30003c90:	4619      	mov	r1, r3
30003c92:	4a17      	ldr	r2, [pc, #92]	; (30003cf0 <BOOT_FaultHandler+0x130>)
30003c94:	f008 f95a 	bl	3000bf4c <rtk_log_write>
30003c98:	463b      	mov	r3, r7
30003c9a:	4632      	mov	r2, r6
30003c9c:	4629      	mov	r1, r5
30003c9e:	4620      	mov	r0, r4
30003ca0:	f008 fba2 	bl	3000c3e8 <__INT_HardFault_C_veneer>
30003ca4:	b015      	add	sp, #84	; 0x54
30003ca6:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003ca8:	4a12      	ldr	r2, [pc, #72]	; (30003cf4 <BOOT_FaultHandler+0x134>)
30003caa:	2001      	movs	r0, #1
30003cac:	4619      	mov	r1, r3
30003cae:	f008 f94d 	bl	3000bf4c <rtk_log_write>
30003cb2:	f3ef 8394 	mrs	r3, CONTROL_NS
30003cb6:	f3ef 8e88 	mrs	lr, MSP_NS
30003cba:	f3ef 8589 	mrs	r5, PSP_NS
30003cbe:	f016 0108 	ands.w	r1, r6, #8
30003cc2:	bf18      	it	ne
30003cc4:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003cc8:	e79b      	b.n	30003c02 <BOOT_FaultHandler+0x42>
30003cca:	bf00      	nop
30003ccc:	3000c710 	.word	0x3000c710
30003cd0:	3000c718 	.word	0x3000c718
30003cd4:	3000c73c 	.word	0x3000c73c
30003cd8:	e000ed00 	.word	0xe000ed00
30003cdc:	3000c790 	.word	0x3000c790
30003ce0:	3000c7a8 	.word	0x3000c7a8
30003ce4:	3000c7c0 	.word	0x3000c7c0
30003ce8:	0000994d 	.word	0x0000994d
30003cec:	00009979 	.word	0x00009979
30003cf0:	3000c7d8 	.word	0x3000c7d8
30003cf4:	3000c764 	.word	0x3000c764

30003cf8 <FLASH_Erase_With_Lock>:
30003cf8:	4b11      	ldr	r3, [pc, #68]	; (30003d40 <FLASH_Erase_With_Lock+0x48>)
30003cfa:	b510      	push	{r4, lr}
30003cfc:	460c      	mov	r4, r1
30003cfe:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30003d02:	4798      	blx	r3
30003d04:	4b0f      	ldr	r3, [pc, #60]	; (30003d44 <FLASH_Erase_With_Lock+0x4c>)
30003d06:	695b      	ldr	r3, [r3, #20]
30003d08:	03db      	lsls	r3, r3, #15
30003d0a:	d512      	bpl.n	30003d32 <FLASH_Erase_With_Lock+0x3a>
30003d0c:	f014 021f 	ands.w	r2, r4, #31
30003d10:	d110      	bne.n	30003d34 <FLASH_Erase_With_Lock+0x3c>
30003d12:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003d16:	f3bf 8f4f 	dsb	sy
30003d1a:	480a      	ldr	r0, [pc, #40]	; (30003d44 <FLASH_Erase_With_Lock+0x4c>)
30003d1c:	4422      	add	r2, r4
30003d1e:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30003d22:	3420      	adds	r4, #32
30003d24:	1b13      	subs	r3, r2, r4
30003d26:	2b00      	cmp	r3, #0
30003d28:	dcf9      	bgt.n	30003d1e <FLASH_Erase_With_Lock+0x26>
30003d2a:	f3bf 8f4f 	dsb	sy
30003d2e:	f3bf 8f6f 	isb	sy
30003d32:	bd10      	pop	{r4, pc}
30003d34:	f024 041f 	bic.w	r4, r4, #31
30003d38:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30003d3c:	e7eb      	b.n	30003d16 <FLASH_Erase_With_Lock+0x1e>
30003d3e:	bf00      	nop
30003d40:	0000a16d 	.word	0x0000a16d
30003d44:	e000ed00 	.word	0xe000ed00

30003d48 <FLASH_TxData_With_Lock>:
30003d48:	b538      	push	{r3, r4, r5, lr}
30003d4a:	4b13      	ldr	r3, [pc, #76]	; (30003d98 <FLASH_TxData_With_Lock+0x50>)
30003d4c:	4604      	mov	r4, r0
30003d4e:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30003d52:	460d      	mov	r5, r1
30003d54:	4798      	blx	r3
30003d56:	4b11      	ldr	r3, [pc, #68]	; (30003d9c <FLASH_TxData_With_Lock+0x54>)
30003d58:	695b      	ldr	r3, [r3, #20]
30003d5a:	03db      	lsls	r3, r3, #15
30003d5c:	d517      	bpl.n	30003d8e <FLASH_TxData_With_Lock+0x46>
30003d5e:	f014 031f 	ands.w	r3, r4, #31
30003d62:	d115      	bne.n	30003d90 <FLASH_TxData_With_Lock+0x48>
30003d64:	f3bf 8f4f 	dsb	sy
30003d68:	b16d      	cbz	r5, 30003d86 <FLASH_TxData_With_Lock+0x3e>
30003d6a:	1e6a      	subs	r2, r5, #1
30003d6c:	f104 0320 	add.w	r3, r4, #32
30003d70:	490a      	ldr	r1, [pc, #40]	; (30003d9c <FLASH_TxData_With_Lock+0x54>)
30003d72:	f022 021f 	bic.w	r2, r2, #31
30003d76:	441a      	add	r2, r3
30003d78:	4293      	cmp	r3, r2
30003d7a:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30003d7e:	461c      	mov	r4, r3
30003d80:	f103 0320 	add.w	r3, r3, #32
30003d84:	d1f8      	bne.n	30003d78 <FLASH_TxData_With_Lock+0x30>
30003d86:	f3bf 8f4f 	dsb	sy
30003d8a:	f3bf 8f6f 	isb	sy
30003d8e:	bd38      	pop	{r3, r4, r5, pc}
30003d90:	f024 041f 	bic.w	r4, r4, #31
30003d94:	441d      	add	r5, r3
30003d96:	e7e5      	b.n	30003d64 <FLASH_TxData_With_Lock+0x1c>
30003d98:	0000a04d 	.word	0x0000a04d
30003d9c:	e000ed00 	.word	0xe000ed00

30003da0 <flash_read_id>:
30003da0:	b538      	push	{r3, r4, r5, lr}
30003da2:	4d04      	ldr	r5, [pc, #16]	; (30003db4 <flash_read_id+0x14>)
30003da4:	460c      	mov	r4, r1
30003da6:	4602      	mov	r2, r0
30003da8:	4b03      	ldr	r3, [pc, #12]	; (30003db8 <flash_read_id+0x18>)
30003daa:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30003dae:	4798      	blx	r3
30003db0:	4620      	mov	r0, r4
30003db2:	bd38      	pop	{r3, r4, r5, pc}
30003db4:	2001c01c 	.word	0x2001c01c
30003db8:	00009db5 	.word	0x00009db5

30003dbc <mcc_ReverseBit>:
30003dbc:	0843      	lsrs	r3, r0, #1
30003dbe:	08c1      	lsrs	r1, r0, #3
30003dc0:	f003 0308 	and.w	r3, r3, #8
30003dc4:	0942      	lsrs	r2, r0, #5
30003dc6:	f001 0104 	and.w	r1, r1, #4
30003dca:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30003dce:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30003dd2:	f002 0202 	and.w	r2, r2, #2
30003dd6:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30003dda:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30003dde:	430b      	orrs	r3, r1
30003de0:	00c1      	lsls	r1, r0, #3
30003de2:	4313      	orrs	r3, r2
30003de4:	f001 0120 	and.w	r1, r1, #32
30003de8:	0042      	lsls	r2, r0, #1
30003dea:	ea43 000c 	orr.w	r0, r3, ip
30003dee:	f002 0310 	and.w	r3, r2, #16
30003df2:	4308      	orrs	r0, r1
30003df4:	4318      	orrs	r0, r3
30003df6:	b2c0      	uxtb	r0, r0
30003df8:	4770      	bx	lr
30003dfa:	bf00      	nop

30003dfc <mcc_init_crc32>:
30003dfc:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30003dfe:	2600      	movs	r6, #0
30003e00:	4f16      	ldr	r7, [pc, #88]	; (30003e5c <mcc_init_crc32+0x60>)
30003e02:	4605      	mov	r5, r0
30003e04:	b2f0      	uxtb	r0, r6
30003e06:	f7ff ffd9 	bl	30003dbc <mcc_ReverseBit>
30003e0a:	2208      	movs	r2, #8
30003e0c:	0604      	lsls	r4, r0, #24
30003e0e:	2c00      	cmp	r4, #0
30003e10:	ea4f 0444 	mov.w	r4, r4, lsl #1
30003e14:	bfb8      	it	lt
30003e16:	407c      	eorlt	r4, r7
30003e18:	3a01      	subs	r2, #1
30003e1a:	d1f8      	bne.n	30003e0e <mcc_init_crc32+0x12>
30003e1c:	0e20      	lsrs	r0, r4, #24
30003e1e:	3504      	adds	r5, #4
30003e20:	f7ff ffcc 	bl	30003dbc <mcc_ReverseBit>
30003e24:	4603      	mov	r3, r0
30003e26:	f3c4 4007 	ubfx	r0, r4, #16, #8
30003e2a:	3601      	adds	r6, #1
30003e2c:	f805 3c04 	strb.w	r3, [r5, #-4]
30003e30:	f7ff ffc4 	bl	30003dbc <mcc_ReverseBit>
30003e34:	4603      	mov	r3, r0
30003e36:	f3c4 2007 	ubfx	r0, r4, #8, #8
30003e3a:	f805 3c03 	strb.w	r3, [r5, #-3]
30003e3e:	f7ff ffbd 	bl	30003dbc <mcc_ReverseBit>
30003e42:	4603      	mov	r3, r0
30003e44:	b2e0      	uxtb	r0, r4
30003e46:	f805 3c02 	strb.w	r3, [r5, #-2]
30003e4a:	f7ff ffb7 	bl	30003dbc <mcc_ReverseBit>
30003e4e:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30003e52:	f805 0c01 	strb.w	r0, [r5, #-1]
30003e56:	d1d5      	bne.n	30003e04 <mcc_init_crc32+0x8>
30003e58:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003e5a:	bf00      	nop
30003e5c:	04c11db7 	.word	0x04c11db7

30003e60 <mcc_crc32>:
30003e60:	b179      	cbz	r1, 30003e82 <mcc_crc32+0x22>
30003e62:	eb00 0c01 	add.w	ip, r0, r1
30003e66:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003e6a:	f810 3b01 	ldrb.w	r3, [r0], #1
30003e6e:	404b      	eors	r3, r1
30003e70:	4560      	cmp	r0, ip
30003e72:	b2db      	uxtb	r3, r3
30003e74:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30003e78:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003e7c:	d1f5      	bne.n	30003e6a <mcc_crc32+0xa>
30003e7e:	43c8      	mvns	r0, r1
30003e80:	4770      	bx	lr
30003e82:	4608      	mov	r0, r1
30003e84:	4770      	bx	lr
30003e86:	bf00      	nop

30003e88 <recovery_check>:
30003e88:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30003e8c:	4283      	cmp	r3, r0
30003e8e:	d322      	bcc.n	30003ed6 <recovery_check+0x4e>
30003e90:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30003e94:	d91f      	bls.n	30003ed6 <recovery_check+0x4e>
30003e96:	b510      	push	{r4, lr}
30003e98:	f8d0 400a 	ldr.w	r4, [r0, #10]
30003e9c:	f104 030c 	add.w	r3, r4, #12
30003ea0:	4299      	cmp	r1, r3
30003ea2:	d316      	bcc.n	30003ed2 <recovery_check+0x4a>
30003ea4:	f100 0c04 	add.w	ip, r0, #4
30003ea8:	b183      	cbz	r3, 30003ecc <recovery_check+0x44>
30003eaa:	f100 0e10 	add.w	lr, r0, #16
30003eae:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003eb2:	44a6      	add	lr, r4
30003eb4:	4c0b      	ldr	r4, [pc, #44]	; (30003ee4 <recovery_check+0x5c>)
30003eb6:	f81c 3b01 	ldrb.w	r3, [ip], #1
30003eba:	404b      	eors	r3, r1
30003ebc:	45f4      	cmp	ip, lr
30003ebe:	b2db      	uxtb	r3, r3
30003ec0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003ec4:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003ec8:	d1f5      	bne.n	30003eb6 <recovery_check+0x2e>
30003eca:	43cb      	mvns	r3, r1
30003ecc:	6801      	ldr	r1, [r0, #0]
30003ece:	4299      	cmp	r1, r3
30003ed0:	d003      	beq.n	30003eda <recovery_check+0x52>
30003ed2:	2000      	movs	r0, #0
30003ed4:	bd10      	pop	{r4, pc}
30003ed6:	2000      	movs	r0, #0
30003ed8:	4770      	bx	lr
30003eda:	f8d0 3006 	ldr.w	r3, [r0, #6]
30003ede:	2001      	movs	r0, #1
30003ee0:	6013      	str	r3, [r2, #0]
30003ee2:	bd10      	pop	{r4, pc}
30003ee4:	3000e934 	.word	0x3000e934

30003ee8 <rewrite_bp>:
30003ee8:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003eec:	b0a3      	sub	sp, #140	; 0x8c
30003eee:	4616      	mov	r6, r2
30003ef0:	4607      	mov	r7, r0
30003ef2:	460d      	mov	r5, r1
30003ef4:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
30003ef8:	4699      	mov	r9, r3
30003efa:	f10d 0808 	add.w	r8, sp, #8
30003efe:	2280      	movs	r2, #128	; 0x80
30003f00:	2c00      	cmp	r4, #0
30003f02:	f000 80b2 	beq.w	3000406a <rewrite_bp+0x182>
30003f06:	4619      	mov	r1, r3
30003f08:	4640      	mov	r0, r8
30003f0a:	f008 fa75 	bl	3000c3f8 <____wrap_memcpy_veneer>
30003f0e:	2301      	movs	r3, #1
30003f10:	f109 0404 	add.w	r4, r9, #4
30003f14:	4649      	mov	r1, r9
30003f16:	2002      	movs	r0, #2
30003f18:	f88d 7014 	strb.w	r7, [sp, #20]
30003f1c:	e9cd 3303 	strd	r3, r3, [sp, #12]
30003f20:	f7ff feea 	bl	30003cf8 <FLASH_Erase_With_Lock>
30003f24:	4620      	mov	r0, r4
30003f26:	217c      	movs	r1, #124	; 0x7c
30003f28:	aa03      	add	r2, sp, #12
30003f2a:	f7ff ff0d 	bl	30003d48 <FLASH_TxData_With_Lock>
30003f2e:	4620      	mov	r0, r4
30003f30:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30003f34:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30003f38:	4c54      	ldr	r4, [pc, #336]	; (3000408c <rewrite_bp+0x1a4>)
30003f3a:	f810 cb01 	ldrb.w	ip, [r0], #1
30003f3e:	ea8c 0c03 	eor.w	ip, ip, r3
30003f42:	4288      	cmp	r0, r1
30003f44:	fa5f fc8c 	uxtb.w	ip, ip
30003f48:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30003f4c:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30003f50:	d1f3      	bne.n	30003f3a <rewrite_bp+0x52>
30003f52:	43db      	mvns	r3, r3
30003f54:	4649      	mov	r1, r9
30003f56:	2002      	movs	r0, #2
30003f58:	9302      	str	r3, [sp, #8]
30003f5a:	f7ff fecd 	bl	30003cf8 <FLASH_Erase_With_Lock>
30003f5e:	4642      	mov	r2, r8
30003f60:	4648      	mov	r0, r9
30003f62:	2180      	movs	r1, #128	; 0x80
30003f64:	f7ff fef0 	bl	30003d48 <FLASH_TxData_With_Lock>
30003f68:	4b49      	ldr	r3, [pc, #292]	; (30004090 <rewrite_bp+0x1a8>)
30003f6a:	aa01      	add	r2, sp, #4
30003f6c:	2104      	movs	r1, #4
30003f6e:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30003f72:	4b48      	ldr	r3, [pc, #288]	; (30004094 <rewrite_bp+0x1ac>)
30003f74:	4798      	blx	r3
30003f76:	f89d 3006 	ldrb.w	r3, [sp, #6]
30003f7a:	f04f 0c01 	mov.w	ip, #1
30003f7e:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30003f82:	682a      	ldr	r2, [r5, #0]
30003f84:	fa0c fc03 	lsl.w	ip, ip, r3
30003f88:	f10e 0e0c 	add.w	lr, lr, #12
30003f8c:	f8d6 300a 	ldr.w	r3, [r6, #10]
30003f90:	45e6      	cmp	lr, ip
30003f92:	6831      	ldr	r1, [r6, #0]
30003f94:	f103 030c 	add.w	r3, r3, #12
30003f98:	bf28      	it	cs
30003f9a:	46e6      	movcs	lr, ip
30003f9c:	459c      	cmp	ip, r3
30003f9e:	bf28      	it	cs
30003fa0:	469c      	movcs	ip, r3
30003fa2:	bb6f      	cbnz	r7, 30004000 <rewrite_bp+0x118>
30003fa4:	1d28      	adds	r0, r5, #4
30003fa6:	f1be 0f00 	cmp.w	lr, #0
30003faa:	d00e      	beq.n	30003fca <rewrite_bp+0xe2>
30003fac:	4486      	add	lr, r0
30003fae:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003fb2:	f810 3b01 	ldrb.w	r3, [r0], #1
30003fb6:	407b      	eors	r3, r7
30003fb8:	4570      	cmp	r0, lr
30003fba:	b2db      	uxtb	r3, r3
30003fbc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003fc0:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003fc4:	d1f5      	bne.n	30003fb2 <rewrite_bp+0xca>
30003fc6:	ea6f 0e07 	mvn.w	lr, r7
30003fca:	4572      	cmp	r2, lr
30003fcc:	d042      	beq.n	30004054 <rewrite_bp+0x16c>
30003fce:	1d32      	adds	r2, r6, #4
30003fd0:	f1bc 0f00 	cmp.w	ip, #0
30003fd4:	d00e      	beq.n	30003ff4 <rewrite_bp+0x10c>
30003fd6:	4494      	add	ip, r2
30003fd8:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003fdc:	f812 3b01 	ldrb.w	r3, [r2], #1
30003fe0:	407b      	eors	r3, r7
30003fe2:	4562      	cmp	r2, ip
30003fe4:	b2db      	uxtb	r3, r3
30003fe6:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003fea:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003fee:	d1f5      	bne.n	30003fdc <rewrite_bp+0xf4>
30003ff0:	ea6f 0c07 	mvn.w	ip, r7
30003ff4:	4561      	cmp	r1, ip
30003ff6:	d03d      	beq.n	30004074 <rewrite_bp+0x18c>
30003ff8:	4827      	ldr	r0, [pc, #156]	; (30004098 <rewrite_bp+0x1b0>)
30003ffa:	f008 fa1d 	bl	3000c438 <__DiagPrintf_veneer>
30003ffe:	e7fe      	b.n	30003ffe <rewrite_bp+0x116>
30004000:	1d30      	adds	r0, r6, #4
30004002:	f1bc 0f00 	cmp.w	ip, #0
30004006:	d00e      	beq.n	30004026 <rewrite_bp+0x13e>
30004008:	4484      	add	ip, r0
3000400a:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
3000400e:	f810 3b01 	ldrb.w	r3, [r0], #1
30004012:	407b      	eors	r3, r7
30004014:	4560      	cmp	r0, ip
30004016:	b2db      	uxtb	r3, r3
30004018:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000401c:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30004020:	d1f5      	bne.n	3000400e <rewrite_bp+0x126>
30004022:	ea6f 0c07 	mvn.w	ip, r7
30004026:	4561      	cmp	r1, ip
30004028:	d024      	beq.n	30004074 <rewrite_bp+0x18c>
3000402a:	1d29      	adds	r1, r5, #4
3000402c:	f1be 0f00 	cmp.w	lr, #0
30004030:	d00e      	beq.n	30004050 <rewrite_bp+0x168>
30004032:	448e      	add	lr, r1
30004034:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30004038:	f811 3b01 	ldrb.w	r3, [r1], #1
3000403c:	407b      	eors	r3, r7
3000403e:	4571      	cmp	r1, lr
30004040:	b2db      	uxtb	r3, r3
30004042:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004046:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000404a:	d1f5      	bne.n	30004038 <rewrite_bp+0x150>
3000404c:	ea6f 0e07 	mvn.w	lr, r7
30004050:	4572      	cmp	r2, lr
30004052:	d1d1      	bne.n	30003ff8 <rewrite_bp+0x110>
30004054:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
30004058:	4810      	ldr	r0, [pc, #64]	; (3000409c <rewrite_bp+0x1b4>)
3000405a:	f8d5 1006 	ldr.w	r1, [r5, #6]
3000405e:	f008 f9eb 	bl	3000c438 <__DiagPrintf_veneer>
30004062:	2001      	movs	r0, #1
30004064:	b023      	add	sp, #140	; 0x8c
30004066:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000406a:	992a      	ldr	r1, [sp, #168]	; 0xa8
3000406c:	4640      	mov	r0, r8
3000406e:	f008 f9c3 	bl	3000c3f8 <____wrap_memcpy_veneer>
30004072:	e74c      	b.n	30003f0e <rewrite_bp+0x26>
30004074:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
30004078:	4809      	ldr	r0, [pc, #36]	; (300040a0 <rewrite_bp+0x1b8>)
3000407a:	f8d6 1006 	ldr.w	r1, [r6, #6]
3000407e:	f008 f9db 	bl	3000c438 <__DiagPrintf_veneer>
30004082:	2002      	movs	r0, #2
30004084:	b023      	add	sp, #140	; 0x8c
30004086:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000408a:	bf00      	nop
3000408c:	3000e934 	.word	0x3000e934
30004090:	2001c01c 	.word	0x2001c01c
30004094:	00009db5 	.word	0x00009db5
30004098:	3000c824 	.word	0x3000c824
3000409c:	3000c7f4 	.word	0x3000c7f4
300040a0:	3000c80c 	.word	0x3000c80c

300040a4 <BOOT_SectionInit>:
300040a4:	4800      	ldr	r0, [pc, #0]	; (300040a8 <BOOT_SectionInit+0x4>)
300040a6:	4770      	bx	lr
300040a8:	60000020 	.word	0x60000020

300040ac <BOOT_NsStart>:
300040ac:	b570      	push	{r4, r5, r6, lr}
300040ae:	4c23      	ldr	r4, [pc, #140]	; (3000413c <BOOT_NsStart+0x90>)
300040b0:	4605      	mov	r5, r0
300040b2:	f384 8808 	msr	MSP, r4
300040b6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300040ba:	2100      	movs	r1, #0
300040bc:	4620      	mov	r0, r4
300040be:	4e20      	ldr	r6, [pc, #128]	; (30004140 <BOOT_NsStart+0x94>)
300040c0:	47b0      	blx	r6
300040c2:	4a20      	ldr	r2, [pc, #128]	; (30004144 <BOOT_NsStart+0x98>)
300040c4:	6953      	ldr	r3, [r2, #20]
300040c6:	03d9      	lsls	r1, r3, #15
300040c8:	d50b      	bpl.n	300040e2 <BOOT_NsStart+0x36>
300040ca:	f3bf 8f4f 	dsb	sy
300040ce:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300040d2:	3420      	adds	r4, #32
300040d4:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300040d8:	d1f9      	bne.n	300040ce <BOOT_NsStart+0x22>
300040da:	f3bf 8f4f 	dsb	sy
300040de:	f3bf 8f6f 	isb	sy
300040e2:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300040e6:	f383 8808 	msr	MSP, r3
300040ea:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300040ee:	2100      	movs	r1, #0
300040f0:	4815      	ldr	r0, [pc, #84]	; (30004148 <BOOT_NsStart+0x9c>)
300040f2:	47b0      	blx	r6
300040f4:	4a13      	ldr	r2, [pc, #76]	; (30004144 <BOOT_NsStart+0x98>)
300040f6:	6953      	ldr	r3, [r2, #20]
300040f8:	03db      	lsls	r3, r3, #15
300040fa:	d50c      	bpl.n	30004116 <BOOT_NsStart+0x6a>
300040fc:	f3bf 8f4f 	dsb	sy
30004100:	4b11      	ldr	r3, [pc, #68]	; (30004148 <BOOT_NsStart+0x9c>)
30004102:	490e      	ldr	r1, [pc, #56]	; (3000413c <BOOT_NsStart+0x90>)
30004104:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
30004108:	3320      	adds	r3, #32
3000410a:	428b      	cmp	r3, r1
3000410c:	d1fa      	bne.n	30004104 <BOOT_NsStart+0x58>
3000410e:	f3bf 8f4f 	dsb	sy
30004112:	f3bf 8f6f 	isb	sy
30004116:	f025 0501 	bic.w	r5, r5, #1
3000411a:	086d      	lsrs	r5, r5, #1
3000411c:	006d      	lsls	r5, r5, #1
3000411e:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004122:	ed2d 8b10 	vpush	{d8-d15}
30004126:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
3000412a:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
3000412e:	47ac      	blxns	r5
30004130:	ecbd 8b10 	vpop	{d8-d15}
30004134:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004138:	bd70      	pop	{r4, r5, r6, pc}
3000413a:	bf00      	nop
3000413c:	30002000 	.word	0x30002000
30004140:	00012be5 	.word	0x00012be5
30004144:	e000ed00 	.word	0xe000ed00
30004148:	30001000 	.word	0x30001000

3000414c <BOOT_RccConfig>:
3000414c:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004150:	4c2f      	ldr	r4, [pc, #188]	; (30004210 <BOOT_RccConfig+0xc4>)
30004152:	b090      	sub	sp, #64	; 0x40
30004154:	4f2f      	ldr	r7, [pc, #188]	; (30004214 <BOOT_RccConfig+0xc8>)
30004156:	466d      	mov	r5, sp
30004158:	f10d 0c20 	add.w	ip, sp, #32
3000415c:	683e      	ldr	r6, [r7, #0]
3000415e:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
30004162:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
30004166:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
3000416a:	f104 0310 	add.w	r3, r4, #16
3000416e:	2400      	movs	r4, #0
30004170:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004172:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
30004176:	1c72      	adds	r2, r6, #1
30004178:	e9cd 4404 	strd	r4, r4, [sp, #16]
3000417c:	e9cd 4406 	strd	r4, r4, [sp, #24]
30004180:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
30004184:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
30004188:	d03f      	beq.n	3000420a <BOOT_RccConfig+0xbe>
3000418a:	463a      	mov	r2, r7
3000418c:	6891      	ldr	r1, [r2, #8]
3000418e:	0fb3      	lsrs	r3, r6, #30
30004190:	a810      	add	r0, sp, #64	; 0x40
30004192:	2901      	cmp	r1, #1
30004194:	eb00 0383 	add.w	r3, r0, r3, lsl #2
30004198:	d10e      	bne.n	300041b8 <BOOT_RccConfig+0x6c>
3000419a:	f853 0c30 	ldr.w	r0, [r3, #-48]
3000419e:	6854      	ldr	r4, [r2, #4]
300041a0:	4330      	orrs	r0, r6
300041a2:	ae10      	add	r6, sp, #64	; 0x40
300041a4:	0fa1      	lsrs	r1, r4, #30
300041a6:	f843 0c30 	str.w	r0, [r3, #-48]
300041aa:	eb06 0181 	add.w	r1, r6, r1, lsl #2
300041ae:	f851 3c10 	ldr.w	r3, [r1, #-16]
300041b2:	4323      	orrs	r3, r4
300041b4:	f841 3c10 	str.w	r3, [r1, #-16]
300041b8:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300041bc:	1c73      	adds	r3, r6, #1
300041be:	d1e5      	bne.n	3000418c <BOOT_RccConfig+0x40>
300041c0:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300041c2:	9c04      	ldr	r4, [sp, #16]
300041c4:	af0d      	add	r7, sp, #52	; 0x34
300041c6:	ae05      	add	r6, sp, #20
300041c8:	4a13      	ldr	r2, [pc, #76]	; (30004218 <BOOT_RccConfig+0xcc>)
300041ca:	f85c 0b04 	ldr.w	r0, [ip], #4
300041ce:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300041d2:	f855 1b04 	ldr.w	r1, [r5], #4
300041d6:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300041da:	f850 8002 	ldr.w	r8, [r0, r2]
300041de:	45e6      	cmp	lr, ip
300041e0:	ea43 0308 	orr.w	r3, r3, r8
300041e4:	5083      	str	r3, [r0, r2]
300041e6:	588b      	ldr	r3, [r1, r2]
300041e8:	ea44 0403 	orr.w	r4, r4, r3
300041ec:	508c      	str	r4, [r1, r2]
300041ee:	d004      	beq.n	300041fa <BOOT_RccConfig+0xae>
300041f0:	f857 3b04 	ldr.w	r3, [r7], #4
300041f4:	f856 4b04 	ldr.w	r4, [r6], #4
300041f8:	e7e7      	b.n	300041ca <BOOT_RccConfig+0x7e>
300041fa:	200a      	movs	r0, #10
300041fc:	4b07      	ldr	r3, [pc, #28]	; (3000421c <BOOT_RccConfig+0xd0>)
300041fe:	4798      	blx	r3
30004200:	4b07      	ldr	r3, [pc, #28]	; (30004220 <BOOT_RccConfig+0xd4>)
30004202:	b010      	add	sp, #64	; 0x40
30004204:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30004208:	4718      	bx	r3
3000420a:	4623      	mov	r3, r4
3000420c:	e7da      	b.n	300041c4 <BOOT_RccConfig+0x78>
3000420e:	bf00      	nop
30004210:	3000e414 	.word	0x3000e414
30004214:	3000e778 	.word	0x3000e778
30004218:	42008000 	.word	0x42008000
3000421c:	00009b2d 	.word	0x00009b2d
30004220:	30007ea9 	.word	0x30007ea9

30004224 <BOOT_CACHEWRR_Set>:
30004224:	4b14      	ldr	r3, [pc, #80]	; (30004278 <BOOT_CACHEWRR_Set+0x54>)
30004226:	b430      	push	{r4, r5}
30004228:	681a      	ldr	r2, [r3, #0]
3000422a:	2a01      	cmp	r2, #1
3000422c:	d10f      	bne.n	3000424e <BOOT_CACHEWRR_Set+0x2a>
3000422e:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
30004232:	685d      	ldr	r5, [r3, #4]
30004234:	689c      	ldr	r4, [r3, #8]
30004236:	6882      	ldr	r2, [r0, #8]
30004238:	4910      	ldr	r1, [pc, #64]	; (3000427c <BOOT_CACHEWRR_Set+0x58>)
3000423a:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
3000423e:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30004242:	6082      	str	r2, [r0, #8]
30004244:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
30004248:	68da      	ldr	r2, [r3, #12]
3000424a:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
3000424e:	691a      	ldr	r2, [r3, #16]
30004250:	2a01      	cmp	r2, #1
30004252:	d10f      	bne.n	30004274 <BOOT_CACHEWRR_Set+0x50>
30004254:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
30004258:	695d      	ldr	r5, [r3, #20]
3000425a:	4a08      	ldr	r2, [pc, #32]	; (3000427c <BOOT_CACHEWRR_Set+0x58>)
3000425c:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30004260:	688b      	ldr	r3, [r1, #8]
30004262:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
30004266:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
3000426a:	608b      	str	r3, [r1, #8]
3000426c:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30004270:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
30004274:	bc30      	pop	{r4, r5}
30004276:	4770      	bx	lr
30004278:	3000ef9c 	.word	0x3000ef9c
3000427c:	e0042000 	.word	0xe0042000

30004280 <BOOT_TCMSet>:
30004280:	2300      	movs	r3, #0
30004282:	b5f0      	push	{r4, r5, r6, r7, lr}
30004284:	4db3      	ldr	r5, [pc, #716]	; (30004554 <BOOT_TCMSet+0x2d4>)
30004286:	461c      	mov	r4, r3
30004288:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
3000428c:	3307      	adds	r3, #7
3000428e:	4282      	cmp	r2, r0
30004290:	d03c      	beq.n	3000430c <BOOT_TCMSet+0x8c>
30004292:	3401      	adds	r4, #1
30004294:	2c07      	cmp	r4, #7
30004296:	d1f7      	bne.n	30004288 <BOOT_TCMSet+0x8>
30004298:	f3bf 8f4f 	dsb	sy
3000429c:	f3bf 8f6f 	isb	sy
300042a0:	48ad      	ldr	r0, [pc, #692]	; (30004558 <BOOT_TCMSet+0x2d8>)
300042a2:	2200      	movs	r2, #0
300042a4:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
300042a8:	f3bf 8f4f 	dsb	sy
300042ac:	f3bf 8f6f 	isb	sy
300042b0:	6943      	ldr	r3, [r0, #20]
300042b2:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
300042b6:	6143      	str	r3, [r0, #20]
300042b8:	f3bf 8f4f 	dsb	sy
300042bc:	f3bf 8f6f 	isb	sy
300042c0:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300042c4:	f3bf 8f4f 	dsb	sy
300042c8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300042cc:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300042d0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300042d4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300042d8:	0164      	lsls	r4, r4, #5
300042da:	ea04 0106 	and.w	r1, r4, r6
300042de:	462b      	mov	r3, r5
300042e0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300042e4:	3b01      	subs	r3, #1
300042e6:	1c5f      	adds	r7, r3, #1
300042e8:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300042ec:	d1f8      	bne.n	300042e0 <BOOT_TCMSet+0x60>
300042ee:	3c20      	subs	r4, #32
300042f0:	f114 0f20 	cmn.w	r4, #32
300042f4:	d1f1      	bne.n	300042da <BOOT_TCMSet+0x5a>
300042f6:	f3bf 8f4f 	dsb	sy
300042fa:	6943      	ldr	r3, [r0, #20]
300042fc:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004300:	6143      	str	r3, [r0, #20]
30004302:	f3bf 8f4f 	dsb	sy
30004306:	f3bf 8f6f 	isb	sy
3000430a:	bdf0      	pop	{r4, r5, r6, r7, pc}
3000430c:	2c00      	cmp	r4, #0
3000430e:	d0c3      	beq.n	30004298 <BOOT_TCMSet+0x18>
30004310:	f3bf 8f4f 	dsb	sy
30004314:	f3bf 8f6f 	isb	sy
30004318:	4890      	ldr	r0, [pc, #576]	; (3000455c <BOOT_TCMSet+0x2dc>)
3000431a:	2300      	movs	r3, #0
3000431c:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30004320:	f3bf 8f4f 	dsb	sy
30004324:	f3bf 8f6f 	isb	sy
30004328:	6942      	ldr	r2, [r0, #20]
3000432a:	03d2      	lsls	r2, r2, #15
3000432c:	d520      	bpl.n	30004370 <BOOT_TCMSet+0xf0>
3000432e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004332:	f3bf 8f4f 	dsb	sy
30004336:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000433a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000433e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30004342:	f3c3 334e 	ubfx	r3, r3, #13, #15
30004346:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000434a:	ea0c 0106 	and.w	r1, ip, r6
3000434e:	4673      	mov	r3, lr
30004350:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004354:	3b01      	subs	r3, #1
30004356:	1c5f      	adds	r7, r3, #1
30004358:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000435c:	d1f8      	bne.n	30004350 <BOOT_TCMSet+0xd0>
3000435e:	f1ac 0c20 	sub.w	ip, ip, #32
30004362:	f11c 0f20 	cmn.w	ip, #32
30004366:	d1f0      	bne.n	3000434a <BOOT_TCMSet+0xca>
30004368:	f3bf 8f4f 	dsb	sy
3000436c:	f3bf 8f6f 	isb	sy
30004370:	f3bf 8f4f 	dsb	sy
30004374:	f3bf 8f6f 	isb	sy
30004378:	4878      	ldr	r0, [pc, #480]	; (3000455c <BOOT_TCMSet+0x2dc>)
3000437a:	2200      	movs	r2, #0
3000437c:	6943      	ldr	r3, [r0, #20]
3000437e:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
30004382:	6143      	str	r3, [r0, #20]
30004384:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
30004388:	f3bf 8f4f 	dsb	sy
3000438c:	f3bf 8f6f 	isb	sy
30004390:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
30004394:	f3bf 8f4f 	dsb	sy
30004398:	6943      	ldr	r3, [r0, #20]
3000439a:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
3000439e:	6143      	str	r3, [r0, #20]
300043a0:	f3bf 8f4f 	dsb	sy
300043a4:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
300043a8:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300043ac:	f3c3 06c9 	ubfx	r6, r3, #3, #10
300043b0:	f3c3 334e 	ubfx	r3, r3, #13, #15
300043b4:	ea4f 1c43 	mov.w	ip, r3, lsl #5
300043b8:	ea0c 010e 	and.w	r1, ip, lr
300043bc:	4633      	mov	r3, r6
300043be:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043c2:	3b01      	subs	r3, #1
300043c4:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300043c8:	1c5a      	adds	r2, r3, #1
300043ca:	d1f8      	bne.n	300043be <BOOT_TCMSet+0x13e>
300043cc:	f1ac 0c20 	sub.w	ip, ip, #32
300043d0:	f11c 0f20 	cmn.w	ip, #32
300043d4:	d1f0      	bne.n	300043b8 <BOOT_TCMSet+0x138>
300043d6:	f3bf 8f4f 	dsb	sy
300043da:	f3bf 8f6f 	isb	sy
300043de:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300043e2:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300043e6:	00e3      	lsls	r3, r4, #3
300043e8:	6b41      	ldr	r1, [r0, #52]	; 0x34
300043ea:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300043ee:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300043f2:	6341      	str	r1, [r0, #52]	; 0x34
300043f4:	6911      	ldr	r1, [r2, #16]
300043f6:	2903      	cmp	r1, #3
300043f8:	d00f      	beq.n	3000441a <BOOT_TCMSet+0x19a>
300043fa:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300043fe:	4858      	ldr	r0, [pc, #352]	; (30004560 <BOOT_TCMSet+0x2e0>)
30004400:	f04f 0c00 	mov.w	ip, #0
30004404:	4f57      	ldr	r7, [pc, #348]	; (30004564 <BOOT_TCMSet+0x2e4>)
30004406:	68b2      	ldr	r2, [r6, #8]
30004408:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
3000440c:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
30004410:	60b2      	str	r2, [r6, #8]
30004412:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
30004416:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
3000441a:	1b1a      	subs	r2, r3, r4
3000441c:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004420:	68d1      	ldr	r1, [r2, #12]
30004422:	2903      	cmp	r1, #3
30004424:	d00f      	beq.n	30004446 <BOOT_TCMSet+0x1c6>
30004426:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000442a:	484d      	ldr	r0, [pc, #308]	; (30004560 <BOOT_TCMSet+0x2e0>)
3000442c:	f04f 0c00 	mov.w	ip, #0
30004430:	4f4c      	ldr	r7, [pc, #304]	; (30004564 <BOOT_TCMSet+0x2e4>)
30004432:	68b2      	ldr	r2, [r6, #8]
30004434:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004438:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
3000443c:	60b2      	str	r2, [r6, #8]
3000443e:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
30004442:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
30004446:	1b1a      	subs	r2, r3, r4
30004448:	4945      	ldr	r1, [pc, #276]	; (30004560 <BOOT_TCMSet+0x2e0>)
3000444a:	eb05 0282 	add.w	r2, r5, r2, lsl #2
3000444e:	6950      	ldr	r0, [r2, #20]
30004450:	6608      	str	r0, [r1, #96]	; 0x60
30004452:	6990      	ldr	r0, [r2, #24]
30004454:	6648      	str	r0, [r1, #100]	; 0x64
30004456:	6852      	ldr	r2, [r2, #4]
30004458:	2a01      	cmp	r2, #1
3000445a:	d04b      	beq.n	300044f4 <BOOT_TCMSet+0x274>
3000445c:	1b1b      	subs	r3, r3, r4
3000445e:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30004462:	68ab      	ldr	r3, [r5, #8]
30004464:	2b01      	cmp	r3, #1
30004466:	f47f af50 	bne.w	3000430a <BOOT_TCMSet+0x8a>
3000446a:	483b      	ldr	r0, [pc, #236]	; (30004558 <BOOT_TCMSet+0x2d8>)
3000446c:	2300      	movs	r3, #0
3000446e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004472:	f3bf 8f4f 	dsb	sy
30004476:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000447a:	f643 76e0 	movw	r6, #16352	; 0x3fe0
3000447e:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004482:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30004486:	0164      	lsls	r4, r4, #5
30004488:	ea04 0106 	and.w	r1, r4, r6
3000448c:	462b      	mov	r3, r5
3000448e:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004492:	3b01      	subs	r3, #1
30004494:	1c5f      	adds	r7, r3, #1
30004496:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000449a:	d1f8      	bne.n	3000448e <BOOT_TCMSet+0x20e>
3000449c:	3c20      	subs	r4, #32
3000449e:	f114 0f20 	cmn.w	r4, #32
300044a2:	d1f1      	bne.n	30004488 <BOOT_TCMSet+0x208>
300044a4:	f3bf 8f4f 	dsb	sy
300044a8:	6943      	ldr	r3, [r0, #20]
300044aa:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300044ae:	6143      	str	r3, [r0, #20]
300044b0:	f3bf 8f4f 	dsb	sy
300044b4:	f3bf 8f6f 	isb	sy
300044b8:	4828      	ldr	r0, [pc, #160]	; (3000455c <BOOT_TCMSet+0x2dc>)
300044ba:	2300      	movs	r3, #0
300044bc:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300044c0:	f3bf 8f4f 	dsb	sy
300044c4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300044c8:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300044cc:	f3c5 344e 	ubfx	r4, r5, #13, #15
300044d0:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300044d4:	0164      	lsls	r4, r4, #5
300044d6:	ea04 0106 	and.w	r1, r4, r6
300044da:	462b      	mov	r3, r5
300044dc:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300044e0:	3b01      	subs	r3, #1
300044e2:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300044e6:	1c5a      	adds	r2, r3, #1
300044e8:	d1f8      	bne.n	300044dc <BOOT_TCMSet+0x25c>
300044ea:	3c20      	subs	r4, #32
300044ec:	f114 0f20 	cmn.w	r4, #32
300044f0:	d1f1      	bne.n	300044d6 <BOOT_TCMSet+0x256>
300044f2:	e700      	b.n	300042f6 <BOOT_TCMSet+0x76>
300044f4:	f3bf 8f4f 	dsb	sy
300044f8:	f3bf 8f6f 	isb	sy
300044fc:	4a16      	ldr	r2, [pc, #88]	; (30004558 <BOOT_TCMSet+0x2d8>)
300044fe:	2000      	movs	r0, #0
30004500:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004504:	f3bf 8f4f 	dsb	sy
30004508:	f3bf 8f6f 	isb	sy
3000450c:	6951      	ldr	r1, [r2, #20]
3000450e:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
30004512:	6151      	str	r1, [r2, #20]
30004514:	f3bf 8f4f 	dsb	sy
30004518:	f3bf 8f6f 	isb	sy
3000451c:	f3bf 8f4f 	dsb	sy
30004520:	f3bf 8f6f 	isb	sy
30004524:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
30004528:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
3000452c:	f3bf 8f4f 	dsb	sy
30004530:	f3bf 8f6f 	isb	sy
30004534:	6951      	ldr	r1, [r2, #20]
30004536:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000453a:	6151      	str	r1, [r2, #20]
3000453c:	f3bf 8f4f 	dsb	sy
30004540:	f3bf 8f6f 	isb	sy
30004544:	1b1b      	subs	r3, r3, r4
30004546:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000454a:	68ab      	ldr	r3, [r5, #8]
3000454c:	2b01      	cmp	r3, #1
3000454e:	f47f aedc 	bne.w	3000430a <BOOT_TCMSet+0x8a>
30004552:	e78a      	b.n	3000446a <BOOT_TCMSet+0x1ea>
30004554:	3000eed8 	.word	0x3000eed8
30004558:	e002ed00 	.word	0xe002ed00
3000455c:	e000ed00 	.word	0xe000ed00
30004560:	e0042000 	.word	0xe0042000
30004564:	ffff0000 	.word	0xffff0000

30004568 <BOOT_GRstConfig>:
30004568:	b538      	push	{r3, r4, r5, lr}
3000456a:	4c09      	ldr	r4, [pc, #36]	; (30004590 <BOOT_GRstConfig+0x28>)
3000456c:	f241 050a 	movw	r5, #4106	; 0x100a
30004570:	2028      	movs	r0, #40	; 0x28
30004572:	4a08      	ldr	r2, [pc, #32]	; (30004594 <BOOT_GRstConfig+0x2c>)
30004574:	8823      	ldrh	r3, [r4, #0]
30004576:	b29b      	uxth	r3, r3
30004578:	431d      	orrs	r5, r3
3000457a:	8025      	strh	r5, [r4, #0]
3000457c:	4790      	blx	r2
3000457e:	88a3      	ldrh	r3, [r4, #4]
30004580:	b29b      	uxth	r3, r3
30004582:	ea23 0305 	bic.w	r3, r3, r5
30004586:	80a3      	strh	r3, [r4, #4]
30004588:	8863      	ldrh	r3, [r4, #2]
3000458a:	8065      	strh	r5, [r4, #2]
3000458c:	bd38      	pop	{r3, r4, r5, pc}
3000458e:	bf00      	nop
30004590:	42008200 	.word	0x42008200
30004594:	00009b2d 	.word	0x00009b2d

30004598 <BOOT_DDR_LCDC_HPR>:
30004598:	4a04      	ldr	r2, [pc, #16]	; (300045ac <BOOT_DDR_LCDC_HPR+0x14>)
3000459a:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
3000459e:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300045a2:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300045a6:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300045aa:	4770      	bx	lr
300045ac:	42008000 	.word	0x42008000

300045b0 <BOOT_DDR_Init>:
300045b0:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300045b4:	b530      	push	{r4, r5, lr}
300045b6:	4d15      	ldr	r5, [pc, #84]	; (3000460c <BOOT_DDR_Init+0x5c>)
300045b8:	b099      	sub	sp, #100	; 0x64
300045ba:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300045bc:	47a8      	blx	r5
300045be:	2803      	cmp	r0, #3
300045c0:	d11e      	bne.n	30004600 <BOOT_DDR_Init+0x50>
300045c2:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300045c6:	4a12      	ldr	r2, [pc, #72]	; (30004610 <BOOT_DDR_Init+0x60>)
300045c8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300045cc:	4911      	ldr	r1, [pc, #68]	; (30004614 <BOOT_DDR_Init+0x64>)
300045ce:	a801      	add	r0, sp, #4
300045d0:	631c      	str	r4, [r3, #48]	; 0x30
300045d2:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300045d6:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300045da:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300045de:	4788      	blx	r1
300045e0:	a801      	add	r0, sp, #4
300045e2:	4b0d      	ldr	r3, [pc, #52]	; (30004618 <BOOT_DDR_Init+0x68>)
300045e4:	4798      	blx	r3
300045e6:	4b0d      	ldr	r3, [pc, #52]	; (3000461c <BOOT_DDR_Init+0x6c>)
300045e8:	4798      	blx	r3
300045ea:	4b0d      	ldr	r3, [pc, #52]	; (30004620 <BOOT_DDR_Init+0x70>)
300045ec:	4798      	blx	r3
300045ee:	4b0d      	ldr	r3, [pc, #52]	; (30004624 <BOOT_DDR_Init+0x74>)
300045f0:	4798      	blx	r3
300045f2:	2101      	movs	r1, #1
300045f4:	f44f 60e0 	mov.w	r0, #1792	; 0x700
300045f8:	4b0b      	ldr	r3, [pc, #44]	; (30004628 <BOOT_DDR_Init+0x78>)
300045fa:	4798      	blx	r3
300045fc:	b019      	add	sp, #100	; 0x64
300045fe:	bd30      	pop	{r4, r5, pc}
30004600:	47a8      	blx	r5
30004602:	2802      	cmp	r0, #2
30004604:	d0dd      	beq.n	300045c2 <BOOT_DDR_Init+0x12>
30004606:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
3000460a:	e7dc      	b.n	300045c6 <BOOT_DDR_Init+0x16>
3000460c:	0000c0f9 	.word	0x0000c0f9
30004610:	42008000 	.word	0x42008000
30004614:	30007fc5 	.word	0x30007fc5
30004618:	3000853d 	.word	0x3000853d
3000461c:	3000b37d 	.word	0x3000b37d
30004620:	3000860d 	.word	0x3000860d
30004624:	30008611 	.word	0x30008611
30004628:	3000b2dd 	.word	0x3000b2dd

3000462c <BOOT_PSRAM_Init>:
3000462c:	b500      	push	{lr}
3000462e:	b089      	sub	sp, #36	; 0x24
30004630:	4b1b      	ldr	r3, [pc, #108]	; (300046a0 <BOOT_PSRAM_Init+0x74>)
30004632:	a801      	add	r0, sp, #4
30004634:	4798      	blx	r3
30004636:	a801      	add	r0, sp, #4
30004638:	4b1a      	ldr	r3, [pc, #104]	; (300046a4 <BOOT_PSRAM_Init+0x78>)
3000463a:	4798      	blx	r3
3000463c:	4b1a      	ldr	r3, [pc, #104]	; (300046a8 <BOOT_PSRAM_Init+0x7c>)
3000463e:	4798      	blx	r3
30004640:	4b1a      	ldr	r3, [pc, #104]	; (300046ac <BOOT_PSRAM_Init+0x80>)
30004642:	4798      	blx	r3
30004644:	2801      	cmp	r0, #1
30004646:	d01d      	beq.n	30004684 <BOOT_PSRAM_Init+0x58>
30004648:	4b19      	ldr	r3, [pc, #100]	; (300046b0 <BOOT_PSRAM_Init+0x84>)
3000464a:	2004      	movs	r0, #4
3000464c:	4a19      	ldr	r2, [pc, #100]	; (300046b4 <BOOT_PSRAM_Init+0x88>)
3000464e:	4619      	mov	r1, r3
30004650:	f007 fc7c 	bl	3000bf4c <rtk_log_write>
30004654:	4b18      	ldr	r3, [pc, #96]	; (300046b8 <BOOT_PSRAM_Init+0x8c>)
30004656:	4798      	blx	r3
30004658:	4b18      	ldr	r3, [pc, #96]	; (300046bc <BOOT_PSRAM_Init+0x90>)
3000465a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000465e:	009b      	lsls	r3, r3, #2
30004660:	d517      	bpl.n	30004692 <BOOT_PSRAM_Init+0x66>
30004662:	4a17      	ldr	r2, [pc, #92]	; (300046c0 <BOOT_PSRAM_Init+0x94>)
30004664:	4b17      	ldr	r3, [pc, #92]	; (300046c4 <BOOT_PSRAM_Init+0x98>)
30004666:	6812      	ldr	r2, [r2, #0]
30004668:	605a      	str	r2, [r3, #4]
3000466a:	4a17      	ldr	r2, [pc, #92]	; (300046c8 <BOOT_PSRAM_Init+0x9c>)
3000466c:	f04f 0c20 	mov.w	ip, #32
30004670:	2101      	movs	r1, #1
30004672:	4b16      	ldr	r3, [pc, #88]	; (300046cc <BOOT_PSRAM_Init+0xa0>)
30004674:	6952      	ldr	r2, [r2, #20]
30004676:	4608      	mov	r0, r1
30004678:	fbbc f2f2 	udiv	r2, ip, r2
3000467c:	4798      	blx	r3
3000467e:	b009      	add	sp, #36	; 0x24
30004680:	f85d fb04 	ldr.w	pc, [sp], #4
30004684:	4b12      	ldr	r3, [pc, #72]	; (300046d0 <BOOT_PSRAM_Init+0xa4>)
30004686:	4798      	blx	r3
30004688:	4b0c      	ldr	r3, [pc, #48]	; (300046bc <BOOT_PSRAM_Init+0x90>)
3000468a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000468e:	009b      	lsls	r3, r3, #2
30004690:	d4e7      	bmi.n	30004662 <BOOT_PSRAM_Init+0x36>
30004692:	4b10      	ldr	r3, [pc, #64]	; (300046d4 <BOOT_PSRAM_Init+0xa8>)
30004694:	4798      	blx	r3
30004696:	4a0b      	ldr	r2, [pc, #44]	; (300046c4 <BOOT_PSRAM_Init+0x98>)
30004698:	4b09      	ldr	r3, [pc, #36]	; (300046c0 <BOOT_PSRAM_Init+0x94>)
3000469a:	6852      	ldr	r2, [r2, #4]
3000469c:	601a      	str	r2, [r3, #0]
3000469e:	e7e4      	b.n	3000466a <BOOT_PSRAM_Init+0x3e>
300046a0:	30008d69 	.word	0x30008d69
300046a4:	30008d89 	.word	0x30008d89
300046a8:	30008df1 	.word	0x30008df1
300046ac:	30007e5d 	.word	0x30007e5d
300046b0:	3000c710 	.word	0x3000c710
300046b4:	3000c844 	.word	0x3000c844
300046b8:	30009095 	.word	0x30009095
300046bc:	42008000 	.word	0x42008000
300046c0:	3000fe08 	.word	0x3000fe08
300046c4:	41002000 	.word	0x41002000
300046c8:	3000fd00 	.word	0x3000fd00
300046cc:	30009375 	.word	0x30009375
300046d0:	30008fbd 	.word	0x30008fbd
300046d4:	300090d1 	.word	0x300090d1

300046d8 <BOOT_WakeFromPG>:
300046d8:	b538      	push	{r3, r4, r5, lr}
300046da:	4c46      	ldr	r4, [pc, #280]	; (300047f4 <BOOT_WakeFromPG+0x11c>)
300046dc:	f002 ff70 	bl	300075c0 <BOOT_RAM_TZCfg>
300046e0:	69a3      	ldr	r3, [r4, #24]
300046e2:	b10b      	cbz	r3, 300046e8 <BOOT_WakeFromPG+0x10>
300046e4:	4a44      	ldr	r2, [pc, #272]	; (300047f8 <BOOT_WakeFromPG+0x120>)
300046e6:	6093      	str	r3, [r2, #8]
300046e8:	4b44      	ldr	r3, [pc, #272]	; (300047fc <BOOT_WakeFromPG+0x124>)
300046ea:	f06f 0c1a 	mvn.w	ip, #26
300046ee:	4844      	ldr	r0, [pc, #272]	; (30004800 <BOOT_WakeFromPG+0x128>)
300046f0:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300046f4:	ebac 0c04 	sub.w	ip, ip, r4
300046f8:	eb0c 0203 	add.w	r2, ip, r3
300046fc:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004700:	4402      	add	r2, r0
30004702:	4573      	cmp	r3, lr
30004704:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30004708:	d1f6      	bne.n	300046f8 <BOOT_WakeFromPG+0x20>
3000470a:	6822      	ldr	r2, [r4, #0]
3000470c:	4b3a      	ldr	r3, [pc, #232]	; (300047f8 <BOOT_WakeFromPG+0x120>)
3000470e:	f422 7280 	bic.w	r2, r2, #256	; 0x100
30004712:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
30004716:	6002      	str	r2, [r0, #0]
30004718:	6045      	str	r5, [r0, #4]
3000471a:	6081      	str	r1, [r0, #8]
3000471c:	6a59      	ldr	r1, [r3, #36]	; 0x24
3000471e:	4a39      	ldr	r2, [pc, #228]	; (30004804 <BOOT_WakeFromPG+0x12c>)
30004720:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30004724:	4d38      	ldr	r5, [pc, #224]	; (30004808 <BOOT_WakeFromPG+0x130>)
30004726:	6259      	str	r1, [r3, #36]	; 0x24
30004728:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000472a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
3000472e:	6251      	str	r1, [r2, #36]	; 0x24
30004730:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30004734:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004738:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000473c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004740:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004744:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004748:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000474c:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004750:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30004754:	f7ff fd66 	bl	30004224 <BOOT_CACHEWRR_Set>
30004758:	4b2c      	ldr	r3, [pc, #176]	; (3000480c <BOOT_WakeFromPG+0x134>)
3000475a:	6818      	ldr	r0, [r3, #0]
3000475c:	f7ff fd90 	bl	30004280 <BOOT_TCMSet>
30004760:	2201      	movs	r2, #1
30004762:	492b      	ldr	r1, [pc, #172]	; (30004810 <BOOT_WakeFromPG+0x138>)
30004764:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
30004768:	47a8      	blx	r5
3000476a:	4b2a      	ldr	r3, [pc, #168]	; (30004814 <BOOT_WakeFromPG+0x13c>)
3000476c:	4798      	blx	r3
3000476e:	4b2a      	ldr	r3, [pc, #168]	; (30004818 <BOOT_WakeFromPG+0x140>)
30004770:	4798      	blx	r3
30004772:	2801      	cmp	r0, #1
30004774:	4604      	mov	r4, r0
30004776:	d026      	beq.n	300047c6 <BOOT_WakeFromPG+0xee>
30004778:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
3000477c:	2201      	movs	r2, #1
3000477e:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30004782:	47a8      	blx	r5
30004784:	4a25      	ldr	r2, [pc, #148]	; (3000481c <BOOT_WakeFromPG+0x144>)
30004786:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000478a:	f043 0302 	orr.w	r3, r3, #2
3000478e:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30004792:	f7ff ff4b 	bl	3000462c <BOOT_PSRAM_Init>
30004796:	2096      	movs	r0, #150	; 0x96
30004798:	4b21      	ldr	r3, [pc, #132]	; (30004820 <BOOT_WakeFromPG+0x148>)
3000479a:	4798      	blx	r3
3000479c:	4a21      	ldr	r2, [pc, #132]	; (30004824 <BOOT_WakeFromPG+0x14c>)
3000479e:	4c19      	ldr	r4, [pc, #100]	; (30004804 <BOOT_WakeFromPG+0x12c>)
300047a0:	6893      	ldr	r3, [r2, #8]
300047a2:	4921      	ldr	r1, [pc, #132]	; (30004828 <BOOT_WakeFromPG+0x150>)
300047a4:	6852      	ldr	r2, [r2, #4]
300047a6:	4821      	ldr	r0, [pc, #132]	; (3000482c <BOOT_WakeFromPG+0x154>)
300047a8:	605a      	str	r2, [r3, #4]
300047aa:	60a3      	str	r3, [r4, #8]
300047ac:	4a20      	ldr	r2, [pc, #128]	; (30004830 <BOOT_WakeFromPG+0x158>)
300047ae:	61c8      	str	r0, [r1, #28]
300047b0:	f382 8888 	msr	MSP_NS, r2
300047b4:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
300047b8:	f382 8809 	msr	PSP, r2
300047bc:	6858      	ldr	r0, [r3, #4]
300047be:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300047c2:	f7ff bc73 	b.w	300040ac <BOOT_NsStart>
300047c6:	4602      	mov	r2, r0
300047c8:	491a      	ldr	r1, [pc, #104]	; (30004834 <BOOT_WakeFromPG+0x15c>)
300047ca:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300047ce:	47a8      	blx	r5
300047d0:	4622      	mov	r2, r4
300047d2:	4919      	ldr	r1, [pc, #100]	; (30004838 <BOOT_WakeFromPG+0x160>)
300047d4:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300047d8:	47a8      	blx	r5
300047da:	f7ff fee9 	bl	300045b0 <BOOT_DDR_Init>
300047de:	4a0f      	ldr	r2, [pc, #60]	; (3000481c <BOOT_WakeFromPG+0x144>)
300047e0:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300047e4:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300047e8:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300047ec:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300047f0:	e7d4      	b.n	3000479c <BOOT_WakeFromPG+0xc4>
300047f2:	bf00      	nop
300047f4:	3000fd9c 	.word	0x3000fd9c
300047f8:	e000ed00 	.word	0xe000ed00
300047fc:	3000fdb7 	.word	0x3000fdb7
30004800:	e000e100 	.word	0xe000e100
30004804:	e002ed00 	.word	0xe002ed00
30004808:	0000b479 	.word	0x0000b479
3000480c:	3000e770 	.word	0x3000e770
30004810:	40001000 	.word	0x40001000
30004814:	0000d835 	.word	0x0000d835
30004818:	30007d85 	.word	0x30007d85
3000481c:	42008000 	.word	0x42008000
30004820:	00009b2d 	.word	0x00009b2d
30004824:	60000020 	.word	0x60000020
30004828:	30000000 	.word	0x30000000
3000482c:	30003a81 	.word	0x30003a81
30004830:	2001bffc 	.word	0x2001bffc
30004834:	40080000 	.word	0x40080000
30004838:	40040000 	.word	0x40040000

3000483c <BOOT_SCBConfig_HP>:
3000483c:	4b0e      	ldr	r3, [pc, #56]	; (30004878 <BOOT_SCBConfig_HP+0x3c>)
3000483e:	4a0f      	ldr	r2, [pc, #60]	; (3000487c <BOOT_SCBConfig_HP+0x40>)
30004840:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004842:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30004846:	6259      	str	r1, [r3, #36]	; 0x24
30004848:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000484a:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
3000484e:	6251      	str	r1, [r2, #36]	; 0x24
30004850:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30004854:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004858:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
3000485c:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004860:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004864:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
30004868:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
3000486c:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004870:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30004874:	4770      	bx	lr
30004876:	bf00      	nop
30004878:	e000ed00 	.word	0xe000ed00
3000487c:	e002ed00 	.word	0xe002ed00

30004880 <BOOT_SCBVTORBackup_HP>:
30004880:	4a02      	ldr	r2, [pc, #8]	; (3000488c <BOOT_SCBVTORBackup_HP+0xc>)
30004882:	4b03      	ldr	r3, [pc, #12]	; (30004890 <BOOT_SCBVTORBackup_HP+0x10>)
30004884:	6892      	ldr	r2, [r2, #8]
30004886:	619a      	str	r2, [r3, #24]
30004888:	4770      	bx	lr
3000488a:	bf00      	nop
3000488c:	e000ed00 	.word	0xe000ed00
30004890:	3000fd9c 	.word	0x3000fd9c

30004894 <BOOT_SCBVTORReFill_HP>:
30004894:	4a02      	ldr	r2, [pc, #8]	; (300048a0 <BOOT_SCBVTORReFill_HP+0xc>)
30004896:	4b03      	ldr	r3, [pc, #12]	; (300048a4 <BOOT_SCBVTORReFill_HP+0x10>)
30004898:	6992      	ldr	r2, [r2, #24]
3000489a:	609a      	str	r2, [r3, #8]
3000489c:	4770      	bx	lr
3000489e:	bf00      	nop
300048a0:	3000fd9c 	.word	0x3000fd9c
300048a4:	e000ed00 	.word	0xe000ed00

300048a8 <BOOT_NVICBackup_HP>:
300048a8:	491a      	ldr	r1, [pc, #104]	; (30004914 <BOOT_NVICBackup_HP+0x6c>)
300048aa:	f06f 001a 	mvn.w	r0, #26
300048ae:	b410      	push	{r4}
300048b0:	4c19      	ldr	r4, [pc, #100]	; (30004918 <BOOT_NVICBackup_HP+0x70>)
300048b2:	680a      	ldr	r2, [r1, #0]
300048b4:	f104 031b 	add.w	r3, r4, #27
300048b8:	1b00      	subs	r0, r0, r4
300048ba:	6022      	str	r2, [r4, #0]
300048bc:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
300048c0:	684a      	ldr	r2, [r1, #4]
300048c2:	6062      	str	r2, [r4, #4]
300048c4:	688a      	ldr	r2, [r1, #8]
300048c6:	60a2      	str	r2, [r4, #8]
300048c8:	18c2      	adds	r2, r0, r3
300048ca:	440a      	add	r2, r1
300048cc:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300048d0:	f803 2f01 	strb.w	r2, [r3, #1]!
300048d4:	4563      	cmp	r3, ip
300048d6:	d1f7      	bne.n	300048c8 <BOOT_NVICBackup_HP+0x20>
300048d8:	4b10      	ldr	r3, [pc, #64]	; (3000491c <BOOT_NVICBackup_HP+0x74>)
300048da:	695b      	ldr	r3, [r3, #20]
300048dc:	03db      	lsls	r3, r3, #15
300048de:	d512      	bpl.n	30004906 <BOOT_NVICBackup_HP+0x5e>
300048e0:	f014 011f 	ands.w	r1, r4, #31
300048e4:	4b0c      	ldr	r3, [pc, #48]	; (30004918 <BOOT_NVICBackup_HP+0x70>)
300048e6:	d111      	bne.n	3000490c <BOOT_NVICBackup_HP+0x64>
300048e8:	216c      	movs	r1, #108	; 0x6c
300048ea:	f3bf 8f4f 	dsb	sy
300048ee:	480b      	ldr	r0, [pc, #44]	; (3000491c <BOOT_NVICBackup_HP+0x74>)
300048f0:	4419      	add	r1, r3
300048f2:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
300048f6:	3320      	adds	r3, #32
300048f8:	1aca      	subs	r2, r1, r3
300048fa:	2a00      	cmp	r2, #0
300048fc:	dcf9      	bgt.n	300048f2 <BOOT_NVICBackup_HP+0x4a>
300048fe:	f3bf 8f4f 	dsb	sy
30004902:	f3bf 8f6f 	isb	sy
30004906:	f85d 4b04 	ldr.w	r4, [sp], #4
3000490a:	4770      	bx	lr
3000490c:	f024 031f 	bic.w	r3, r4, #31
30004910:	316c      	adds	r1, #108	; 0x6c
30004912:	e7ea      	b.n	300048ea <BOOT_NVICBackup_HP+0x42>
30004914:	e000e100 	.word	0xe000e100
30004918:	3000fd9c 	.word	0x3000fd9c
3000491c:	e000ed00 	.word	0xe000ed00

30004920 <BOOT_NVICReFill_HP>:
30004920:	b510      	push	{r4, lr}
30004922:	f06f 0c1a 	mvn.w	ip, #26
30004926:	4c0d      	ldr	r4, [pc, #52]	; (3000495c <BOOT_NVICReFill_HP+0x3c>)
30004928:	480d      	ldr	r0, [pc, #52]	; (30004960 <BOOT_NVICReFill_HP+0x40>)
3000492a:	f104 031b 	add.w	r3, r4, #27
3000492e:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30004932:	ebac 0c04 	sub.w	ip, ip, r4
30004936:	eb0c 0203 	add.w	r2, ip, r3
3000493a:	f813 1f01 	ldrb.w	r1, [r3, #1]!
3000493e:	4402      	add	r2, r0
30004940:	4573      	cmp	r3, lr
30004942:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
30004946:	d1f6      	bne.n	30004936 <BOOT_NVICReFill_HP+0x16>
30004948:	e9d4 3100 	ldrd	r3, r1, [r4]
3000494c:	68a2      	ldr	r2, [r4, #8]
3000494e:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30004952:	6003      	str	r3, [r0, #0]
30004954:	6041      	str	r1, [r0, #4]
30004956:	6082      	str	r2, [r0, #8]
30004958:	bd10      	pop	{r4, pc}
3000495a:	bf00      	nop
3000495c:	3000fd9c 	.word	0x3000fd9c
30004960:	e000e100 	.word	0xe000e100

30004964 <BOOT_ImgCopy>:
30004964:	b510      	push	{r4, lr}
30004966:	b082      	sub	sp, #8
30004968:	4b09      	ldr	r3, [pc, #36]	; (30004990 <BOOT_ImgCopy+0x2c>)
3000496a:	4604      	mov	r4, r0
3000496c:	e9cd 2100 	strd	r2, r1, [sp]
30004970:	4798      	blx	r3
30004972:	e9dd 2100 	ldrd	r2, r1, [sp]
30004976:	b128      	cbz	r0, 30004984 <BOOT_ImgCopy+0x20>
30004978:	4620      	mov	r0, r4
3000497a:	4b06      	ldr	r3, [pc, #24]	; (30004994 <BOOT_ImgCopy+0x30>)
3000497c:	b002      	add	sp, #8
3000497e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004982:	4718      	bx	r3
30004984:	4620      	mov	r0, r4
30004986:	b002      	add	sp, #8
30004988:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000498c:	f003 bfa6 	b.w	300088dc <NandImgCopy>
30004990:	30009c15 	.word	0x30009c15
30004994:	00012a1d 	.word	0x00012a1d

30004998 <BOOT_LoadImages>:
30004998:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000499c:	4b59      	ldr	r3, [pc, #356]	; (30004b04 <BOOT_LoadImages+0x16c>)
3000499e:	2700      	movs	r7, #0
300049a0:	2104      	movs	r1, #4
300049a2:	2601      	movs	r6, #1
300049a4:	ed2d 8b02 	vpush	{d8}
300049a8:	b0b7      	sub	sp, #220	; 0xdc
300049aa:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
300049ae:	4b56      	ldr	r3, [pc, #344]	; (30004b08 <BOOT_LoadImages+0x170>)
300049b0:	aa11      	add	r2, sp, #68	; 0x44
300049b2:	9703      	str	r7, [sp, #12]
300049b4:	9705      	str	r7, [sp, #20]
300049b6:	4798      	blx	r3
300049b8:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
300049bc:	409e      	lsls	r6, r3
300049be:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
300049c2:	f000 823e 	beq.w	30004e42 <BOOT_LoadImages+0x4aa>
300049c6:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
300049ca:	f000 8379 	beq.w	300050c0 <BOOT_LoadImages+0x728>
300049ce:	f1b6 7f00 	cmp.w	r6, #33554432	; 0x2000000
300049d2:	f000 8383 	beq.w	300050dc <BOOT_LoadImages+0x744>
300049d6:	4c4d      	ldr	r4, [pc, #308]	; (30004b0c <BOOT_LoadImages+0x174>)
300049d8:	f04f 0800 	mov.w	r8, #0
300049dc:	4d4c      	ldr	r5, [pc, #304]	; (30004b10 <BOOT_LoadImages+0x178>)
300049de:	eb06 0904 	add.w	r9, r6, r4
300049e2:	484c      	ldr	r0, [pc, #304]	; (30004b14 <BOOT_LoadImages+0x17c>)
300049e4:	4435      	add	r5, r6
300049e6:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
300049ea:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
300049ee:	f7ff fa05 	bl	30003dfc <mcc_init_crc32>
300049f2:	f8d9 3004 	ldr.w	r3, [r9, #4]
300049f6:	f8d5 a008 	ldr.w	sl, [r5, #8]
300049fa:	f899 200c 	ldrb.w	r2, [r9, #12]
300049fe:	9308      	str	r3, [sp, #32]
30004a00:	3b01      	subs	r3, #1
30004a02:	9206      	str	r2, [sp, #24]
30004a04:	3303      	adds	r3, #3
30004a06:	686a      	ldr	r2, [r5, #4]
30004a08:	f895 b00c 	ldrb.w	fp, [r5, #12]
30004a0c:	9204      	str	r2, [sp, #16]
30004a0e:	f200 81bc 	bhi.w	30004d8a <BOOT_LoadImages+0x3f2>
30004a12:	f8d9 2008 	ldr.w	r2, [r9, #8]
30004a16:	9b04      	ldr	r3, [sp, #16]
30004a18:	3a01      	subs	r2, #1
30004a1a:	3b01      	subs	r3, #1
30004a1c:	3203      	adds	r2, #3
30004a1e:	d867      	bhi.n	30004af0 <BOOT_LoadImages+0x158>
30004a20:	9a06      	ldr	r2, [sp, #24]
30004a22:	9307      	str	r3, [sp, #28]
30004a24:	2a01      	cmp	r2, #1
30004a26:	f240 8305 	bls.w	30005034 <BOOT_LoadImages+0x69c>
30004a2a:	483b      	ldr	r0, [pc, #236]	; (30004b18 <BOOT_LoadImages+0x180>)
30004a2c:	f007 fd04 	bl	3000c438 <__DiagPrintf_veneer>
30004a30:	9b07      	ldr	r3, [sp, #28]
30004a32:	3303      	adds	r3, #3
30004a34:	d863      	bhi.n	30004afe <BOOT_LoadImages+0x166>
30004a36:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30004a3a:	f11a 0f03 	cmn.w	sl, #3
30004a3e:	f200 81aa 	bhi.w	30004d96 <BOOT_LoadImages+0x3fe>
30004a42:	f1bb 0f01 	cmp.w	fp, #1
30004a46:	f200 8347 	bhi.w	300050d8 <BOOT_LoadImages+0x740>
30004a4a:	9904      	ldr	r1, [sp, #16]
30004a4c:	4833      	ldr	r0, [pc, #204]	; (30004b1c <BOOT_LoadImages+0x184>)
30004a4e:	f007 fcf3 	bl	3000c438 <__DiagPrintf_veneer>
30004a52:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004a56:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004a5a:	4c2e      	ldr	r4, [pc, #184]	; (30004b14 <BOOT_LoadImages+0x17c>)
30004a5c:	9307      	str	r3, [sp, #28]
30004a5e:	2300      	movs	r3, #0
30004a60:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004a64:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004a68:	9309      	str	r3, [sp, #36]	; 0x24
30004a6a:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30004b34 <BOOT_LoadImages+0x19c>
30004a6e:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30004a72:	6829      	ldr	r1, [r5, #0]
30004a74:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
30004a78:	44b4      	add	ip, r6
30004a7a:	f81c 2b01 	ldrb.w	r2, [ip], #1
30004a7e:	4042      	eors	r2, r0
30004a80:	459c      	cmp	ip, r3
30004a82:	b2d2      	uxtb	r2, r2
30004a84:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30004a88:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30004a8c:	d1f5      	bne.n	30004a7a <BOOT_LoadImages+0xe2>
30004a8e:	43c2      	mvns	r2, r0
30004a90:	930f      	str	r3, [sp, #60]	; 0x3c
30004a92:	4291      	cmp	r1, r2
30004a94:	f000 8301 	beq.w	3000509a <BOOT_LoadImages+0x702>
30004a98:	4821      	ldr	r0, [pc, #132]	; (30004b20 <BOOT_LoadImages+0x188>)
30004a9a:	f007 fccd 	bl	3000c438 <__DiagPrintf_veneer>
30004a9e:	2200      	movs	r2, #0
30004aa0:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004aa2:	9909      	ldr	r1, [sp, #36]	; 0x24
30004aa4:	2900      	cmp	r1, #0
30004aa6:	d04e      	beq.n	30004b46 <BOOT_LoadImages+0x1ae>
30004aa8:	f899 100c 	ldrb.w	r1, [r9, #12]
30004aac:	455b      	cmp	r3, fp
30004aae:	9106      	str	r1, [sp, #24]
30004ab0:	d342      	bcc.n	30004b38 <BOOT_LoadImages+0x1a0>
30004ab2:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30004ab6:	45e3      	cmp	fp, ip
30004ab8:	d93e      	bls.n	30004b38 <BOOT_LoadImages+0x1a0>
30004aba:	9807      	ldr	r0, [sp, #28]
30004abc:	4560      	cmp	r0, ip
30004abe:	f240 8203 	bls.w	30004ec8 <BOOT_LoadImages+0x530>
30004ac2:	4283      	cmp	r3, r0
30004ac4:	f0c0 8200 	bcc.w	30004ec8 <BOOT_LoadImages+0x530>
30004ac8:	2a00      	cmp	r2, #0
30004aca:	f000 820d 	beq.w	30004ee8 <BOOT_LoadImages+0x550>
30004ace:	4553      	cmp	r3, sl
30004ad0:	f080 8410 	bcs.w	300052f4 <BOOT_LoadImages+0x95c>
30004ad4:	4813      	ldr	r0, [pc, #76]	; (30004b24 <BOOT_LoadImages+0x18c>)
30004ad6:	f007 fcaf 	bl	3000c438 <__DiagPrintf_veneer>
30004ada:	9f06      	ldr	r7, [sp, #24]
30004adc:	4812      	ldr	r0, [pc, #72]	; (30004b28 <BOOT_LoadImages+0x190>)
30004ade:	464d      	mov	r5, r9
30004ae0:	f007 fcaa 	bl	3000c438 <__DiagPrintf_veneer>
30004ae4:	9b07      	ldr	r3, [sp, #28]
30004ae6:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004aea:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004aee:	e04a      	b.n	30004b86 <BOOT_LoadImages+0x1ee>
30004af0:	480e      	ldr	r0, [pc, #56]	; (30004b2c <BOOT_LoadImages+0x194>)
30004af2:	9307      	str	r3, [sp, #28]
30004af4:	f007 fca0 	bl	3000c438 <__DiagPrintf_veneer>
30004af8:	9b07      	ldr	r3, [sp, #28]
30004afa:	3303      	adds	r3, #3
30004afc:	d99b      	bls.n	30004a36 <BOOT_LoadImages+0x9e>
30004afe:	480c      	ldr	r0, [pc, #48]	; (30004b30 <BOOT_LoadImages+0x198>)
30004b00:	e14a      	b.n	30004d98 <BOOT_LoadImages+0x400>
30004b02:	bf00      	nop
30004b04:	2001c01c 	.word	0x2001c01c
30004b08:	00009db5 	.word	0x00009db5
30004b0c:	07ffe000 	.word	0x07ffe000
30004b10:	07fff000 	.word	0x07fff000
30004b14:	3000e934 	.word	0x3000e934
30004b18:	3000c8bc 	.word	0x3000c8bc
30004b1c:	3000cca4 	.word	0x3000cca4
30004b20:	3000c9c0 	.word	0x3000c9c0
30004b24:	3000cc88 	.word	0x3000cc88
30004b28:	3000ca4c 	.word	0x3000ca4c
30004b2c:	3000c8a8 	.word	0x3000c8a8
30004b30:	3000cc54 	.word	0x3000cc54
30004b34:	07fff004 	.word	0x07fff004
30004b38:	48c4      	ldr	r0, [pc, #784]	; (30004e4c <BOOT_LoadImages+0x4b4>)
30004b3a:	930f      	str	r3, [sp, #60]	; 0x3c
30004b3c:	9209      	str	r2, [sp, #36]	; 0x24
30004b3e:	f007 fc7b 	bl	3000c438 <__DiagPrintf_veneer>
30004b42:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004b44:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004b46:	2a00      	cmp	r2, #0
30004b48:	f000 8131 	beq.w	30004dae <BOOT_LoadImages+0x416>
30004b4c:	459a      	cmp	sl, r3
30004b4e:	f200 82c9 	bhi.w	300050e4 <BOOT_LoadImages+0x74c>
30004b52:	f1ba 6f00 	cmp.w	sl, #134217728	; 0x8000000
30004b56:	bf2c      	ite	cs
30004b58:	2200      	movcs	r2, #0
30004b5a:	2201      	movcc	r2, #1
30004b5c:	2a00      	cmp	r2, #0
30004b5e:	f040 82c1 	bne.w	300050e4 <BOOT_LoadImages+0x74c>
30004b62:	4598      	cmp	r8, r3
30004b64:	f200 81b8 	bhi.w	30004ed8 <BOOT_LoadImages+0x540>
30004b68:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30004b6c:	f0c0 81b4 	bcc.w	30004ed8 <BOOT_LoadImages+0x540>
30004b70:	7b2f      	ldrb	r7, [r5, #12]
30004b72:	2a00      	cmp	r2, #0
30004b74:	f040 81de 	bne.w	30004f34 <BOOT_LoadImages+0x59c>
30004b78:	48b5      	ldr	r0, [pc, #724]	; (30004e50 <BOOT_LoadImages+0x4b8>)
30004b7a:	f007 fc5d 	bl	3000c438 <__DiagPrintf_veneer>
30004b7e:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30004b82:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30004b86:	4639      	mov	r1, r7
30004b88:	48b2      	ldr	r0, [pc, #712]	; (30004e54 <BOOT_LoadImages+0x4bc>)
30004b8a:	f007 fc55 	bl	3000c438 <__DiagPrintf_veneer>
30004b8e:	4bb2      	ldr	r3, [pc, #712]	; (30004e58 <BOOT_LoadImages+0x4c0>)
30004b90:	4798      	blx	r3
30004b92:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004b96:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30004b9a:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004b9e:	6809      	ldr	r1, [r1, #0]
30004ba0:	330c      	adds	r3, #12
30004ba2:	910b      	str	r1, [sp, #44]	; 0x2c
30004ba4:	42b3      	cmp	r3, r6
30004ba6:	bf28      	it	cs
30004ba8:	4633      	movcs	r3, r6
30004baa:	930d      	str	r3, [sp, #52]	; 0x34
30004bac:	f8d2 300a 	ldr.w	r3, [r2, #10]
30004bb0:	6812      	ldr	r2, [r2, #0]
30004bb2:	330c      	adds	r3, #12
30004bb4:	920a      	str	r2, [sp, #40]	; 0x28
30004bb6:	429e      	cmp	r6, r3
30004bb8:	bf28      	it	cs
30004bba:	461e      	movcs	r6, r3
30004bbc:	960c      	str	r6, [sp, #48]	; 0x30
30004bbe:	2800      	cmp	r0, #0
30004bc0:	f040 8194 	bne.w	30004eec <BOOT_LoadImages+0x554>
30004bc4:	4ba5      	ldr	r3, [pc, #660]	; (30004e5c <BOOT_LoadImages+0x4c4>)
30004bc6:	4606      	mov	r6, r0
30004bc8:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30004e88 <BOOT_LoadImages+0x4f0>
30004bcc:	4680      	mov	r8, r0
30004bce:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 30004e8c <BOOT_LoadImages+0x4f4>
30004bd2:	4682      	mov	sl, r0
30004bd4:	e9cd 0303 	strd	r0, r3, [sp, #12]
30004bd8:	2f00      	cmp	r7, #0
30004bda:	f040 81c9 	bne.w	30004f70 <BOOT_LoadImages+0x5d8>
30004bde:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30004be2:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004be4:	f10c 0104 	add.w	r1, ip, #4
30004be8:	2b00      	cmp	r3, #0
30004bea:	f000 83df 	beq.w	300053ac <BOOT_LoadImages+0xa14>
30004bee:	1d18      	adds	r0, r3, #4
30004bf0:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004bf4:	4460      	add	r0, ip
30004bf6:	f811 3b01 	ldrb.w	r3, [r1], #1
30004bfa:	4053      	eors	r3, r2
30004bfc:	4281      	cmp	r1, r0
30004bfe:	b2db      	uxtb	r3, r3
30004c00:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004c04:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004c08:	d1f5      	bne.n	30004bf6 <BOOT_LoadImages+0x25e>
30004c0a:	43d2      	mvns	r2, r2
30004c0c:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004c0e:	4293      	cmp	r3, r2
30004c10:	f000 83a7 	beq.w	30005362 <BOOT_LoadImages+0x9ca>
30004c14:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004c18:	f088 0301 	eor.w	r3, r8, #1
30004c1c:	3101      	adds	r1, #1
30004c1e:	f003 0301 	and.w	r3, r3, #1
30004c22:	ea46 0603 	orr.w	r6, r6, r3
30004c26:	f000 8346 	beq.w	300052b6 <BOOT_LoadImages+0x91e>
30004c2a:	990b      	ldr	r1, [sp, #44]	; 0x2c
30004c2c:	488c      	ldr	r0, [pc, #560]	; (30004e60 <BOOT_LoadImages+0x4c8>)
30004c2e:	f007 fc03 	bl	3000c438 <__DiagPrintf_veneer>
30004c32:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004c36:	2002      	movs	r0, #2
30004c38:	f7ff f85e 	bl	30003cf8 <FLASH_Erase_With_Lock>
30004c3c:	2e00      	cmp	r6, #0
30004c3e:	f040 8324 	bne.w	3000528a <BOOT_LoadImages+0x8f2>
30004c42:	9605      	str	r6, [sp, #20]
30004c44:	f08a 0601 	eor.w	r6, sl, #1
30004c48:	f04f 0801 	mov.w	r8, #1
30004c4c:	f006 0601 	and.w	r6, r6, #1
30004c50:	9b03      	ldr	r3, [sp, #12]
30004c52:	2b01      	cmp	r3, #1
30004c54:	f000 8238 	beq.w	300050c8 <BOOT_LoadImages+0x730>
30004c58:	9b03      	ldr	r3, [sp, #12]
30004c5a:	2b02      	cmp	r3, #2
30004c5c:	f000 83b2 	beq.w	300053c4 <BOOT_LoadImages+0xa2c>
30004c60:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30004c62:	4650      	mov	r0, sl
30004c64:	f8c9 3000 	str.w	r3, [r9]
30004c68:	47d8      	blx	fp
30004c6a:	1e03      	subs	r3, r0, #0
30004c6c:	bf18      	it	ne
30004c6e:	2301      	movne	r3, #1
30004c70:	2800      	cmp	r0, #0
30004c72:	f000 8156 	beq.w	30004f22 <BOOT_LoadImages+0x58a>
30004c76:	2e00      	cmp	r6, #0
30004c78:	f000 8153 	beq.w	30004f22 <BOOT_LoadImages+0x58a>
30004c7c:	fab7 f787 	clz	r7, r7
30004c80:	9b05      	ldr	r3, [sp, #20]
30004c82:	097f      	lsrs	r7, r7, #5
30004c84:	2b00      	cmp	r3, #0
30004c86:	f000 83ee 	beq.w	30005466 <BOOT_LoadImages+0xace>
30004c8a:	469a      	mov	sl, r3
30004c8c:	4c75      	ldr	r4, [pc, #468]	; (30004e64 <BOOT_LoadImages+0x4cc>)
30004c8e:	2f00      	cmp	r7, #0
30004c90:	f040 8138 	bne.w	30004f04 <BOOT_LoadImages+0x56c>
30004c94:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004c98:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c9c:	3201      	adds	r2, #1
30004c9e:	f000 82ad 	beq.w	300051fc <BOOT_LoadImages+0x864>
30004ca2:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004ca6:	4870      	ldr	r0, [pc, #448]	; (30004e68 <BOOT_LoadImages+0x4d0>)
30004ca8:	f007 fbc6 	bl	3000c438 <__DiagPrintf_veneer>
30004cac:	2301      	movs	r3, #1
30004cae:	9303      	str	r3, [sp, #12]
30004cb0:	f04f 0801 	mov.w	r8, #1
30004cb4:	4646      	mov	r6, r8
30004cb6:	f8cd 8014 	str.w	r8, [sp, #20]
30004cba:	f1ba 0f00 	cmp.w	sl, #0
30004cbe:	d0c7      	beq.n	30004c50 <BOOT_LoadImages+0x2b8>
30004cc0:	ab16      	add	r3, sp, #88	; 0x58
30004cc2:	2280      	movs	r2, #128	; 0x80
30004cc4:	21ff      	movs	r1, #255	; 0xff
30004cc6:	46d0      	mov	r8, sl
30004cc8:	461e      	mov	r6, r3
30004cca:	4618      	mov	r0, r3
30004ccc:	ee08 3a10 	vmov	s16, r3
30004cd0:	f007 fb82 	bl	3000c3d8 <____wrap_memset_veneer>
30004cd4:	2280      	movs	r2, #128	; 0x80
30004cd6:	4629      	mov	r1, r5
30004cd8:	4630      	mov	r0, r6
30004cda:	f007 fb8d 	bl	3000c3f8 <____wrap_memcpy_veneer>
30004cde:	2f01      	cmp	r7, #1
30004ce0:	f000 8327 	beq.w	30005332 <BOOT_LoadImages+0x99a>
30004ce4:	2100      	movs	r1, #0
30004ce6:	f8cd a014 	str.w	sl, [sp, #20]
30004cea:	f04f 0a01 	mov.w	sl, #1
30004cee:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30004cf2:	1d2e      	adds	r6, r5, #4
30004cf4:	485d      	ldr	r0, [pc, #372]	; (30004e6c <BOOT_LoadImages+0x4d4>)
30004cf6:	f007 fb9f 	bl	3000c438 <__DiagPrintf_veneer>
30004cfa:	4629      	mov	r1, r5
30004cfc:	2002      	movs	r0, #2
30004cfe:	f7fe fffb 	bl	30003cf8 <FLASH_Erase_With_Lock>
30004d02:	aa17      	add	r2, sp, #92	; 0x5c
30004d04:	217c      	movs	r1, #124	; 0x7c
30004d06:	4630      	mov	r0, r6
30004d08:	f7ff f81e 	bl	30003d48 <FLASH_TxData_With_Lock>
30004d0c:	4630      	mov	r0, r6
30004d0e:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30004d12:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004d16:	f810 3b01 	ldrb.w	r3, [r0], #1
30004d1a:	4053      	eors	r3, r2
30004d1c:	4288      	cmp	r0, r1
30004d1e:	b2db      	uxtb	r3, r3
30004d20:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004d24:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004d28:	d1f5      	bne.n	30004d16 <BOOT_LoadImages+0x37e>
30004d2a:	43d2      	mvns	r2, r2
30004d2c:	4629      	mov	r1, r5
30004d2e:	2002      	movs	r0, #2
30004d30:	9216      	str	r2, [sp, #88]	; 0x58
30004d32:	f7fe ffe1 	bl	30003cf8 <FLASH_Erase_With_Lock>
30004d36:	ee18 2a10 	vmov	r2, s16
30004d3a:	2180      	movs	r1, #128	; 0x80
30004d3c:	4628      	mov	r0, r5
30004d3e:	f7ff f803 	bl	30003d48 <FLASH_TxData_With_Lock>
30004d42:	9b05      	ldr	r3, [sp, #20]
30004d44:	2b00      	cmp	r3, #0
30004d46:	f000 8143 	beq.w	30004fd0 <BOOT_LoadImages+0x638>
30004d4a:	f1b8 0f00 	cmp.w	r8, #0
30004d4e:	f040 837f 	bne.w	30005450 <BOOT_LoadImages+0xab8>
30004d52:	2f01      	cmp	r7, #1
30004d54:	f000 8268 	beq.w	30005228 <BOOT_LoadImages+0x890>
30004d58:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d5c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004d60:	3201      	adds	r2, #1
30004d62:	f000 81d4 	beq.w	3000510e <BOOT_LoadImages+0x776>
30004d66:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004d6a:	4841      	ldr	r0, [pc, #260]	; (30004e70 <BOOT_LoadImages+0x4d8>)
30004d6c:	f007 fb64 	bl	3000c438 <__DiagPrintf_veneer>
30004d70:	f08a 0601 	eor.w	r6, sl, #1
30004d74:	f04f 0801 	mov.w	r8, #1
30004d78:	f006 0601 	and.w	r6, r6, #1
30004d7c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d80:	3310      	adds	r3, #16
30004d82:	930e      	str	r3, [sp, #56]	; 0x38
30004d84:	2302      	movs	r3, #2
30004d86:	9303      	str	r3, [sp, #12]
30004d88:	e76a      	b.n	30004c60 <BOOT_LoadImages+0x2c8>
30004d8a:	483a      	ldr	r0, [pc, #232]	; (30004e74 <BOOT_LoadImages+0x4dc>)
30004d8c:	f007 fb54 	bl	3000c438 <__DiagPrintf_veneer>
30004d90:	9b04      	ldr	r3, [sp, #16]
30004d92:	3b01      	subs	r3, #1
30004d94:	e64d      	b.n	30004a32 <BOOT_LoadImages+0x9a>
30004d96:	4838      	ldr	r0, [pc, #224]	; (30004e78 <BOOT_LoadImages+0x4e0>)
30004d98:	f007 fb4e 	bl	3000c438 <__DiagPrintf_veneer>
30004d9c:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004da0:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004da4:	9307      	str	r3, [sp, #28]
30004da6:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004daa:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004dae:	4833      	ldr	r0, [pc, #204]	; (30004e7c <BOOT_LoadImages+0x4e4>)
30004db0:	f007 fb42 	bl	3000c438 <__DiagPrintf_veneer>
30004db4:	4832      	ldr	r0, [pc, #200]	; (30004e80 <BOOT_LoadImages+0x4e8>)
30004db6:	f007 fb3f 	bl	3000c438 <__DiagPrintf_veneer>
30004dba:	4631      	mov	r1, r6
30004dbc:	aa12      	add	r2, sp, #72	; 0x48
30004dbe:	4658      	mov	r0, fp
30004dc0:	f7ff f862 	bl	30003e88 <recovery_check>
30004dc4:	4631      	mov	r1, r6
30004dc6:	4604      	mov	r4, r0
30004dc8:	aa13      	add	r2, sp, #76	; 0x4c
30004dca:	9807      	ldr	r0, [sp, #28]
30004dcc:	f7ff f85c 	bl	30003e88 <recovery_check>
30004dd0:	4631      	mov	r1, r6
30004dd2:	aa14      	add	r2, sp, #80	; 0x50
30004dd4:	9004      	str	r0, [sp, #16]
30004dd6:	4650      	mov	r0, sl
30004dd8:	f7ff f856 	bl	30003e88 <recovery_check>
30004ddc:	4631      	mov	r1, r6
30004dde:	aa15      	add	r2, sp, #84	; 0x54
30004de0:	4606      	mov	r6, r0
30004de2:	4640      	mov	r0, r8
30004de4:	f7ff f850 	bl	30003e88 <recovery_check>
30004de8:	9b04      	ldr	r3, [sp, #16]
30004dea:	4306      	orrs	r6, r0
30004dec:	431c      	orrs	r4, r3
30004dee:	d04f      	beq.n	30004e90 <BOOT_LoadImages+0x4f8>
30004df0:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30004df4:	2e00      	cmp	r6, #0
30004df6:	f000 818e 	beq.w	30005116 <BOOT_LoadImages+0x77e>
30004dfa:	4293      	cmp	r3, r2
30004dfc:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
30004e00:	f0c0 827c 	bcc.w	300052fc <BOOT_LoadImages+0x964>
30004e04:	428b      	cmp	r3, r1
30004e06:	d302      	bcc.n	30004e0e <BOOT_LoadImages+0x476>
30004e08:	4283      	cmp	r3, r0
30004e0a:	f080 82fd 	bcs.w	30005408 <BOOT_LoadImages+0xa70>
30004e0e:	4293      	cmp	r3, r2
30004e10:	f000 8274 	beq.w	300052fc <BOOT_LoadImages+0x964>
30004e14:	4293      	cmp	r3, r2
30004e16:	bf38      	it	cc
30004e18:	4613      	movcc	r3, r2
30004e1a:	428b      	cmp	r3, r1
30004e1c:	f240 82e5 	bls.w	300053ea <BOOT_LoadImages+0xa52>
30004e20:	428b      	cmp	r3, r1
30004e22:	bf38      	it	cc
30004e24:	460b      	movcc	r3, r1
30004e26:	4283      	cmp	r3, r0
30004e28:	f240 82f6 	bls.w	30005418 <BOOT_LoadImages+0xa80>
30004e2c:	f04f 0800 	mov.w	r8, #0
30004e30:	4b0a      	ldr	r3, [pc, #40]	; (30004e5c <BOOT_LoadImages+0x4c4>)
30004e32:	2601      	movs	r6, #1
30004e34:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30004e88 <BOOT_LoadImages+0x4f0>
30004e38:	f8df b050 	ldr.w	fp, [pc, #80]	; 30004e8c <BOOT_LoadImages+0x4f4>
30004e3c:	46c2      	mov	sl, r8
30004e3e:	9304      	str	r3, [sp, #16]
30004e40:	e70e      	b.n	30004c60 <BOOT_LoadImages+0x2c8>
30004e42:	4810      	ldr	r0, [pc, #64]	; (30004e84 <BOOT_LoadImages+0x4ec>)
30004e44:	f007 faf8 	bl	3000c438 <__DiagPrintf_veneer>
30004e48:	e5c5      	b.n	300049d6 <BOOT_LoadImages+0x3e>
30004e4a:	bf00      	nop
30004e4c:	3000ca00 	.word	0x3000ca00
30004e50:	3000ca5c 	.word	0x3000ca5c
30004e54:	3000ca90 	.word	0x3000ca90
30004e58:	0000c149 	.word	0x0000c149
30004e5c:	3000e76f 	.word	0x3000e76f
30004e60:	3000cb00 	.word	0x3000cb00
30004e64:	3000e934 	.word	0x3000e934
30004e68:	3000c7f4 	.word	0x3000c7f4
30004e6c:	3000cbd4 	.word	0x3000cbd4
30004e70:	3000c80c 	.word	0x3000c80c
30004e74:	3000c890 	.word	0x3000c890
30004e78:	3000c8f4 	.word	0x3000c8f4
30004e7c:	3000ca6c 	.word	0x3000ca6c
30004e80:	3000ca80 	.word	0x3000ca80
30004e84:	3000c858 	.word	0x3000c858
30004e88:	3000fd98 	.word	0x3000fd98
30004e8c:	30006cd1 	.word	0x30006cd1
30004e90:	2e00      	cmp	r6, #0
30004e92:	f000 8285 	beq.w	300053a0 <BOOT_LoadImages+0xa08>
30004e96:	4651      	mov	r1, sl
30004e98:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
30004e9c:	e9cd 5400 	strd	r5, r4, [sp]
30004ea0:	429a      	cmp	r2, r3
30004ea2:	464b      	mov	r3, r9
30004ea4:	4642      	mov	r2, r8
30004ea6:	f080 8223 	bcs.w	300052f0 <BOOT_LoadImages+0x958>
30004eaa:	2001      	movs	r0, #1
30004eac:	f7ff f81c 	bl	30003ee8 <rewrite_bp>
30004eb0:	46a0      	mov	r8, r4
30004eb2:	b2c3      	uxtb	r3, r0
30004eb4:	2601      	movs	r6, #1
30004eb6:	46a2      	mov	sl, r4
30004eb8:	f8df 92e8 	ldr.w	r9, [pc, #744]	; 300051a4 <BOOT_LoadImages+0x80c>
30004ebc:	9303      	str	r3, [sp, #12]
30004ebe:	4ba3      	ldr	r3, [pc, #652]	; (3000514c <BOOT_LoadImages+0x7b4>)
30004ec0:	f8df b2e4 	ldr.w	fp, [pc, #740]	; 300051a8 <BOOT_LoadImages+0x810>
30004ec4:	9304      	str	r3, [sp, #16]
30004ec6:	e6c3      	b.n	30004c50 <BOOT_LoadImages+0x2b8>
30004ec8:	48a1      	ldr	r0, [pc, #644]	; (30005150 <BOOT_LoadImages+0x7b8>)
30004eca:	930f      	str	r3, [sp, #60]	; 0x3c
30004ecc:	9209      	str	r2, [sp, #36]	; 0x24
30004ece:	f007 fab3 	bl	3000c438 <__DiagPrintf_veneer>
30004ed2:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004ed4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004ed6:	e636      	b.n	30004b46 <BOOT_LoadImages+0x1ae>
30004ed8:	489e      	ldr	r0, [pc, #632]	; (30005154 <BOOT_LoadImages+0x7bc>)
30004eda:	9204      	str	r2, [sp, #16]
30004edc:	f007 faac 	bl	3000c438 <__DiagPrintf_veneer>
30004ee0:	9a04      	ldr	r2, [sp, #16]
30004ee2:	2a00      	cmp	r2, #0
30004ee4:	f43f af63 	beq.w	30004dae <BOOT_LoadImages+0x416>
30004ee8:	9f06      	ldr	r7, [sp, #24]
30004eea:	e5f7      	b.n	30004adc <BOOT_LoadImages+0x144>
30004eec:	2300      	movs	r3, #0
30004eee:	4a97      	ldr	r2, [pc, #604]	; (3000514c <BOOT_LoadImages+0x7b4>)
30004ef0:	f8df 92b0 	ldr.w	r9, [pc, #688]	; 300051a4 <BOOT_LoadImages+0x80c>
30004ef4:	f8df b2b0 	ldr.w	fp, [pc, #688]	; 300051a8 <BOOT_LoadImages+0x810>
30004ef8:	469a      	mov	sl, r3
30004efa:	9303      	str	r3, [sp, #12]
30004efc:	9204      	str	r2, [sp, #16]
30004efe:	2f00      	cmp	r7, #0
30004f00:	f43f aec8 	beq.w	30004c94 <BOOT_LoadImages+0x2fc>
30004f04:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004f08:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004f0c:	3201      	adds	r2, #1
30004f0e:	f000 819f 	beq.w	30005250 <BOOT_LoadImages+0x8b8>
30004f12:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004f16:	4890      	ldr	r0, [pc, #576]	; (30005158 <BOOT_LoadImages+0x7c0>)
30004f18:	f007 fa8e 	bl	3000c438 <__DiagPrintf_veneer>
30004f1c:	2302      	movs	r3, #2
30004f1e:	9303      	str	r3, [sp, #12]
30004f20:	e6c6      	b.n	30004cb0 <BOOT_LoadImages+0x318>
30004f22:	f00a 0401 	and.w	r4, sl, #1
30004f26:	b1c3      	cbz	r3, 30004f5a <BOOT_LoadImages+0x5c2>
30004f28:	b1bc      	cbz	r4, 30004f5a <BOOT_LoadImages+0x5c2>
30004f2a:	4c8c      	ldr	r4, [pc, #560]	; (3000515c <BOOT_LoadImages+0x7c4>)
30004f2c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004f30:	47a0      	blx	r4
30004f32:	e7fb      	b.n	30004f2c <BOOT_LoadImages+0x594>
30004f34:	488a      	ldr	r0, [pc, #552]	; (30005160 <BOOT_LoadImages+0x7c8>)
30004f36:	f007 fa7f 	bl	3000c438 <__DiagPrintf_veneer>
30004f3a:	9b08      	ldr	r3, [sp, #32]
30004f3c:	9a04      	ldr	r2, [sp, #16]
30004f3e:	4293      	cmp	r3, r2
30004f40:	f4ff ae1a 	bcc.w	30004b78 <BOOT_LoadImages+0x1e0>
30004f44:	4887      	ldr	r0, [pc, #540]	; (30005164 <BOOT_LoadImages+0x7cc>)
30004f46:	464d      	mov	r5, r9
30004f48:	f007 fa76 	bl	3000c438 <__DiagPrintf_veneer>
30004f4c:	9b07      	ldr	r3, [sp, #28]
30004f4e:	9f06      	ldr	r7, [sp, #24]
30004f50:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004f54:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004f58:	e615      	b.n	30004b86 <BOOT_LoadImages+0x1ee>
30004f5a:	9b04      	ldr	r3, [sp, #16]
30004f5c:	781b      	ldrb	r3, [r3, #0]
30004f5e:	2b00      	cmp	r3, #0
30004f60:	f040 80c4 	bne.w	300050ec <BOOT_LoadImages+0x754>
30004f64:	2001      	movs	r0, #1
30004f66:	b037      	add	sp, #220	; 0xdc
30004f68:	ecbd 8b02 	vpop	{d8}
30004f6c:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f70:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30004f74:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004f76:	f10c 0104 	add.w	r1, ip, #4
30004f7a:	2b00      	cmp	r3, #0
30004f7c:	f000 8214 	beq.w	300053a8 <BOOT_LoadImages+0xa10>
30004f80:	1d18      	adds	r0, r3, #4
30004f82:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f86:	4460      	add	r0, ip
30004f88:	f811 3b01 	ldrb.w	r3, [r1], #1
30004f8c:	4053      	eors	r3, r2
30004f8e:	4281      	cmp	r1, r0
30004f90:	b2db      	uxtb	r3, r3
30004f92:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004f96:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004f9a:	d1f5      	bne.n	30004f88 <BOOT_LoadImages+0x5f0>
30004f9c:	43d2      	mvns	r2, r2
30004f9e:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004fa0:	4293      	cmp	r3, r2
30004fa2:	f000 81cd 	beq.w	30005340 <BOOT_LoadImages+0x9a8>
30004fa6:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004faa:	f088 0301 	eor.w	r3, r8, #1
30004fae:	3101      	adds	r1, #1
30004fb0:	f003 0301 	and.w	r3, r3, #1
30004fb4:	ea46 0603 	orr.w	r6, r6, r3
30004fb8:	f000 8181 	beq.w	300052be <BOOT_LoadImages+0x926>
30004fbc:	990a      	ldr	r1, [sp, #40]	; 0x28
30004fbe:	486a      	ldr	r0, [pc, #424]	; (30005168 <BOOT_LoadImages+0x7d0>)
30004fc0:	f007 fa3a 	bl	3000c438 <__DiagPrintf_veneer>
30004fc4:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004fc8:	2002      	movs	r0, #2
30004fca:	f7fe fe95 	bl	30003cf8 <FLASH_Erase_With_Lock>
30004fce:	e635      	b.n	30004c3c <BOOT_LoadImages+0x2a4>
30004fd0:	f1b8 0f00 	cmp.w	r8, #0
30004fd4:	f040 8227 	bne.w	30005426 <BOOT_LoadImages+0xa8e>
30004fd8:	2f01      	cmp	r7, #1
30004fda:	f000 80e7 	beq.w	300051ac <BOOT_LoadImages+0x814>
30004fde:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30004fe2:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004fe4:	1d31      	adds	r1, r6, #4
30004fe6:	2b00      	cmp	r3, #0
30004fe8:	f000 81ee 	beq.w	300053c8 <BOOT_LoadImages+0xa30>
30004fec:	1d18      	adds	r0, r3, #4
30004fee:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004ff2:	4430      	add	r0, r6
30004ff4:	f811 3b01 	ldrb.w	r3, [r1], #1
30004ff8:	4053      	eors	r3, r2
30004ffa:	4281      	cmp	r1, r0
30004ffc:	b2db      	uxtb	r3, r3
30004ffe:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30005002:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005006:	d1f5      	bne.n	30004ff4 <BOOT_LoadImages+0x65c>
30005008:	43d2      	mvns	r2, r2
3000500a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000500c:	4293      	cmp	r3, r2
3000500e:	f000 81b8 	beq.w	30005382 <BOOT_LoadImages+0x9ea>
30005012:	f8d6 300a 	ldr.w	r3, [r6, #10]
30005016:	3301      	adds	r3, #1
30005018:	d079      	beq.n	3000510e <BOOT_LoadImages+0x776>
3000501a:	990a      	ldr	r1, [sp, #40]	; 0x28
3000501c:	4852      	ldr	r0, [pc, #328]	; (30005168 <BOOT_LoadImages+0x7d0>)
3000501e:	f007 fa0b 	bl	3000c438 <__DiagPrintf_veneer>
30005022:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30005026:	2002      	movs	r0, #2
30005028:	f7fe fe66 	bl	30003cf8 <FLASH_Erase_With_Lock>
3000502c:	484f      	ldr	r0, [pc, #316]	; (3000516c <BOOT_LoadImages+0x7d4>)
3000502e:	f007 fa03 	bl	3000c438 <__DiagPrintf_veneer>
30005032:	e7fe      	b.n	30005032 <BOOT_LoadImages+0x69a>
30005034:	9908      	ldr	r1, [sp, #32]
30005036:	484e      	ldr	r0, [pc, #312]	; (30005170 <BOOT_LoadImages+0x7d8>)
30005038:	f007 f9fe 	bl	3000c438 <__DiagPrintf_veneer>
3000503c:	9b07      	ldr	r3, [sp, #28]
3000503e:	3303      	adds	r3, #3
30005040:	f240 81b6 	bls.w	300053b0 <BOOT_LoadImages+0xa18>
30005044:	484b      	ldr	r0, [pc, #300]	; (30005174 <BOOT_LoadImages+0x7dc>)
30005046:	f007 f9f7 	bl	3000c438 <__DiagPrintf_veneer>
3000504a:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
3000504e:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005052:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30005056:	9307      	str	r3, [sp, #28]
30005058:	f8d5 a00d 	ldr.w	sl, [r5, #13]
3000505c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005060:	4845      	ldr	r0, [pc, #276]	; (30005178 <BOOT_LoadImages+0x7e0>)
30005062:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30005066:	5931      	ldr	r1, [r6, r4]
30005068:	4430      	add	r0, r6
3000506a:	4c44      	ldr	r4, [pc, #272]	; (3000517c <BOOT_LoadImages+0x7e4>)
3000506c:	f810 3b01 	ldrb.w	r3, [r0], #1
30005070:	4053      	eors	r3, r2
30005072:	4285      	cmp	r5, r0
30005074:	b2db      	uxtb	r3, r3
30005076:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000507a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
3000507e:	d1f5      	bne.n	3000506c <BOOT_LoadImages+0x6d4>
30005080:	43d2      	mvns	r2, r2
30005082:	4291      	cmp	r1, r2
30005084:	d010      	beq.n	300050a8 <BOOT_LoadImages+0x710>
30005086:	483e      	ldr	r0, [pc, #248]	; (30005180 <BOOT_LoadImages+0x7e8>)
30005088:	f007 f9d6 	bl	3000c438 <__DiagPrintf_veneer>
3000508c:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000508e:	2b00      	cmp	r3, #0
30005090:	f43f ae8d 	beq.w	30004dae <BOOT_LoadImages+0x416>
30005094:	2300      	movs	r3, #0
30005096:	9309      	str	r3, [sp, #36]	; 0x24
30005098:	e4e7      	b.n	30004a6a <BOOT_LoadImages+0xd2>
3000509a:	460a      	mov	r2, r1
3000509c:	4839      	ldr	r0, [pc, #228]	; (30005184 <BOOT_LoadImages+0x7ec>)
3000509e:	f007 f9cb 	bl	3000c438 <__DiagPrintf_veneer>
300050a2:	2201      	movs	r2, #1
300050a4:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
300050a6:	e4fc      	b.n	30004aa2 <BOOT_LoadImages+0x10a>
300050a8:	460a      	mov	r2, r1
300050aa:	4837      	ldr	r0, [pc, #220]	; (30005188 <BOOT_LoadImages+0x7f0>)
300050ac:	f007 f9c4 	bl	3000c438 <__DiagPrintf_veneer>
300050b0:	9b09      	ldr	r3, [sp, #36]	; 0x24
300050b2:	2b00      	cmp	r3, #0
300050b4:	f47f acd9 	bne.w	30004a6a <BOOT_LoadImages+0xd2>
300050b8:	461a      	mov	r2, r3
300050ba:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300050be:	e4f3      	b.n	30004aa8 <BOOT_LoadImages+0x110>
300050c0:	4832      	ldr	r0, [pc, #200]	; (3000518c <BOOT_LoadImages+0x7f4>)
300050c2:	f007 f9b9 	bl	3000c438 <__DiagPrintf_veneer>
300050c6:	e486      	b.n	300049d6 <BOOT_LoadImages+0x3e>
300050c8:	4c2c      	ldr	r4, [pc, #176]	; (3000517c <BOOT_LoadImages+0x7e4>)
300050ca:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300050ce:	3310      	adds	r3, #16
300050d0:	930e      	str	r3, [sp, #56]	; 0x38
300050d2:	2301      	movs	r3, #1
300050d4:	9303      	str	r3, [sp, #12]
300050d6:	e5c3      	b.n	30004c60 <BOOT_LoadImages+0x2c8>
300050d8:	482d      	ldr	r0, [pc, #180]	; (30005190 <BOOT_LoadImages+0x7f8>)
300050da:	e65d      	b.n	30004d98 <BOOT_LoadImages+0x400>
300050dc:	482d      	ldr	r0, [pc, #180]	; (30005194 <BOOT_LoadImages+0x7fc>)
300050de:	f007 f9ab 	bl	3000c438 <__DiagPrintf_veneer>
300050e2:	e478      	b.n	300049d6 <BOOT_LoadImages+0x3e>
300050e4:	482c      	ldr	r0, [pc, #176]	; (30005198 <BOOT_LoadImages+0x800>)
300050e6:	f007 f9a7 	bl	3000c438 <__DiagPrintf_veneer>
300050ea:	e660      	b.n	30004dae <BOOT_LoadImages+0x416>
300050ec:	4b2b      	ldr	r3, [pc, #172]	; (3000519c <BOOT_LoadImages+0x804>)
300050ee:	4650      	mov	r0, sl
300050f0:	4798      	blx	r3
300050f2:	1e03      	subs	r3, r0, #0
300050f4:	bf18      	it	ne
300050f6:	2301      	movne	r3, #1
300050f8:	b110      	cbz	r0, 30005100 <BOOT_LoadImages+0x768>
300050fa:	2e00      	cmp	r6, #0
300050fc:	f47f adbe 	bne.w	30004c7c <BOOT_LoadImages+0x2e4>
30005100:	2c00      	cmp	r4, #0
30005102:	f43f af2f 	beq.w	30004f64 <BOOT_LoadImages+0x5cc>
30005106:	2b00      	cmp	r3, #0
30005108:	f47f af0f 	bne.w	30004f2a <BOOT_LoadImages+0x592>
3000510c:	e72a      	b.n	30004f64 <BOOT_LoadImages+0x5cc>
3000510e:	4824      	ldr	r0, [pc, #144]	; (300051a0 <BOOT_LoadImages+0x808>)
30005110:	f007 f992 	bl	3000c438 <__DiagPrintf_veneer>
30005114:	e78a      	b.n	3000502c <BOOT_LoadImages+0x694>
30005116:	4293      	cmp	r3, r2
30005118:	f04f 0401 	mov.w	r4, #1
3000511c:	464b      	mov	r3, r9
3000511e:	4659      	mov	r1, fp
30005120:	bf28      	it	cs
30005122:	4630      	movcs	r0, r6
30005124:	9a07      	ldr	r2, [sp, #28]
30005126:	bf38      	it	cc
30005128:	4620      	movcc	r0, r4
3000512a:	9500      	str	r5, [sp, #0]
3000512c:	9401      	str	r4, [sp, #4]
3000512e:	46b0      	mov	r8, r6
30005130:	f7fe feda 	bl	30003ee8 <rewrite_bp>
30005134:	4626      	mov	r6, r4
30005136:	b2c3      	uxtb	r3, r0
30005138:	f8df 9068 	ldr.w	r9, [pc, #104]	; 300051a4 <BOOT_LoadImages+0x80c>
3000513c:	46c2      	mov	sl, r8
3000513e:	f8df b068 	ldr.w	fp, [pc, #104]	; 300051a8 <BOOT_LoadImages+0x810>
30005142:	9303      	str	r3, [sp, #12]
30005144:	4b01      	ldr	r3, [pc, #4]	; (3000514c <BOOT_LoadImages+0x7b4>)
30005146:	9304      	str	r3, [sp, #16]
30005148:	e582      	b.n	30004c50 <BOOT_LoadImages+0x2b8>
3000514a:	bf00      	nop
3000514c:	3000e76f 	.word	0x3000e76f
30005150:	3000ca1c 	.word	0x3000ca1c
30005154:	3000cc38 	.word	0x3000cc38
30005158:	3000c80c 	.word	0x3000c80c
3000515c:	00009be5 	.word	0x00009be5
30005160:	3000ca38 	.word	0x3000ca38
30005164:	3000ca4c 	.word	0x3000ca4c
30005168:	3000cb88 	.word	0x3000cb88
3000516c:	3000c824 	.word	0x3000c824
30005170:	3000c8d8 	.word	0x3000c8d8
30005174:	3000cc54 	.word	0x3000cc54
30005178:	07ffe004 	.word	0x07ffe004
3000517c:	3000e934 	.word	0x3000e934
30005180:	3000c944 	.word	0x3000c944
30005184:	3000c984 	.word	0x3000c984
30005188:	3000c908 	.word	0x3000c908
3000518c:	3000c868 	.word	0x3000c868
30005190:	3000cc6c 	.word	0x3000cc6c
30005194:	3000c87c 	.word	0x3000c87c
30005198:	3000cc88 	.word	0x3000cc88
3000519c:	30006cd5 	.word	0x30006cd5
300051a0:	3000cab4 	.word	0x3000cab4
300051a4:	3000fd98 	.word	0x3000fd98
300051a8:	30006cd1 	.word	0x30006cd1
300051ac:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
300051b0:	9b0d      	ldr	r3, [sp, #52]	; 0x34
300051b2:	1d31      	adds	r1, r6, #4
300051b4:	2b00      	cmp	r3, #0
300051b6:	f000 8123 	beq.w	30005400 <BOOT_LoadImages+0xa68>
300051ba:	1d18      	adds	r0, r3, #4
300051bc:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300051c0:	4430      	add	r0, r6
300051c2:	f811 3b01 	ldrb.w	r3, [r1], #1
300051c6:	4053      	eors	r3, r2
300051c8:	4281      	cmp	r1, r0
300051ca:	b2db      	uxtb	r3, r3
300051cc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051d0:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300051d4:	d1f5      	bne.n	300051c2 <BOOT_LoadImages+0x82a>
300051d6:	43d2      	mvns	r2, r2
300051d8:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300051da:	4293      	cmp	r3, r2
300051dc:	f000 80f6 	beq.w	300053cc <BOOT_LoadImages+0xa34>
300051e0:	f8d6 300a 	ldr.w	r3, [r6, #10]
300051e4:	3301      	adds	r3, #1
300051e6:	d07f      	beq.n	300052e8 <BOOT_LoadImages+0x950>
300051e8:	990b      	ldr	r1, [sp, #44]	; 0x2c
300051ea:	48a1      	ldr	r0, [pc, #644]	; (30005470 <BOOT_LoadImages+0xad8>)
300051ec:	f007 f924 	bl	3000c438 <__DiagPrintf_veneer>
300051f0:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
300051f4:	2002      	movs	r0, #2
300051f6:	f7fe fd7f 	bl	30003cf8 <FLASH_Erase_With_Lock>
300051fa:	e717      	b.n	3000502c <BOOT_LoadImages+0x694>
300051fc:	ab16      	add	r3, sp, #88	; 0x58
300051fe:	489d      	ldr	r0, [pc, #628]	; (30005474 <BOOT_LoadImages+0xadc>)
30005200:	461e      	mov	r6, r3
30005202:	ee08 3a10 	vmov	s16, r3
30005206:	f007 f917 	bl	3000c438 <__DiagPrintf_veneer>
3000520a:	2280      	movs	r2, #128	; 0x80
3000520c:	21ff      	movs	r1, #255	; 0xff
3000520e:	4630      	mov	r0, r6
30005210:	f007 f8e2 	bl	3000c3d8 <____wrap_memset_veneer>
30005214:	2280      	movs	r2, #128	; 0x80
30005216:	4629      	mov	r1, r5
30005218:	4630      	mov	r0, r6
3000521a:	f007 f8ed 	bl	3000c3f8 <____wrap_memcpy_veneer>
3000521e:	f1ba 0f00 	cmp.w	sl, #0
30005222:	d050      	beq.n	300052c6 <BOOT_LoadImages+0x92e>
30005224:	46b8      	mov	r8, r7
30005226:	e55d      	b.n	30004ce4 <BOOT_LoadImages+0x34c>
30005228:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
3000522c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005230:	3201      	adds	r2, #1
30005232:	d059      	beq.n	300052e8 <BOOT_LoadImages+0x950>
30005234:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005238:	488f      	ldr	r0, [pc, #572]	; (30005478 <BOOT_LoadImages+0xae0>)
3000523a:	f007 f8fd 	bl	3000c438 <__DiagPrintf_veneer>
3000523e:	9705      	str	r7, [sp, #20]
30005240:	f08a 0601 	eor.w	r6, sl, #1
30005244:	f04f 0801 	mov.w	r8, #1
30005248:	f006 0601 	and.w	r6, r6, #1
3000524c:	4647      	mov	r7, r8
3000524e:	e73c      	b.n	300050ca <BOOT_LoadImages+0x732>
30005250:	ab16      	add	r3, sp, #88	; 0x58
30005252:	488a      	ldr	r0, [pc, #552]	; (3000547c <BOOT_LoadImages+0xae4>)
30005254:	461e      	mov	r6, r3
30005256:	ee08 3a10 	vmov	s16, r3
3000525a:	f007 f8ed 	bl	3000c438 <__DiagPrintf_veneer>
3000525e:	2280      	movs	r2, #128	; 0x80
30005260:	21ff      	movs	r1, #255	; 0xff
30005262:	4630      	mov	r0, r6
30005264:	f007 f8b8 	bl	3000c3d8 <____wrap_memset_veneer>
30005268:	2280      	movs	r2, #128	; 0x80
3000526a:	4629      	mov	r1, r5
3000526c:	4630      	mov	r0, r6
3000526e:	f007 f8c3 	bl	3000c3f8 <____wrap_memcpy_veneer>
30005272:	f1ba 0f00 	cmp.w	sl, #0
30005276:	d130      	bne.n	300052da <BOOT_LoadImages+0x942>
30005278:	2301      	movs	r3, #1
3000527a:	9305      	str	r3, [sp, #20]
3000527c:	46d0      	mov	r8, sl
3000527e:	f04f 0a00 	mov.w	sl, #0
30005282:	4651      	mov	r1, sl
30005284:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
30005288:	e533      	b.n	30004cf2 <BOOT_LoadImages+0x35a>
3000528a:	ab16      	add	r3, sp, #88	; 0x58
3000528c:	2280      	movs	r2, #128	; 0x80
3000528e:	21ff      	movs	r1, #255	; 0xff
30005290:	461e      	mov	r6, r3
30005292:	4618      	mov	r0, r3
30005294:	ee08 3a10 	vmov	s16, r3
30005298:	f007 f89e 	bl	3000c3d8 <____wrap_memset_veneer>
3000529c:	2280      	movs	r2, #128	; 0x80
3000529e:	4629      	mov	r1, r5
300052a0:	4630      	mov	r0, r6
300052a2:	f007 f8a9 	bl	3000c3f8 <____wrap_memcpy_veneer>
300052a6:	f1ba 0f00 	cmp.w	sl, #0
300052aa:	f040 80d9 	bne.w	30005460 <BOOT_LoadImages+0xac8>
300052ae:	b9bf      	cbnz	r7, 300052e0 <BOOT_LoadImages+0x948>
300052b0:	46c2      	mov	sl, r8
300052b2:	9705      	str	r7, [sp, #20]
300052b4:	e009      	b.n	300052ca <BOOT_LoadImages+0x932>
300052b6:	486f      	ldr	r0, [pc, #444]	; (30005474 <BOOT_LoadImages+0xadc>)
300052b8:	f007 f8be 	bl	3000c438 <__DiagPrintf_veneer>
300052bc:	e4be      	b.n	30004c3c <BOOT_LoadImages+0x2a4>
300052be:	486f      	ldr	r0, [pc, #444]	; (3000547c <BOOT_LoadImages+0xae4>)
300052c0:	f007 f8ba 	bl	3000c438 <__DiagPrintf_veneer>
300052c4:	e4ba      	b.n	30004c3c <BOOT_LoadImages+0x2a4>
300052c6:	2301      	movs	r3, #1
300052c8:	9305      	str	r3, [sp, #20]
300052ca:	46d0      	mov	r8, sl
300052cc:	2101      	movs	r1, #1
300052ce:	f04f 0a00 	mov.w	sl, #0
300052d2:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300052d6:	4657      	mov	r7, sl
300052d8:	e50b      	b.n	30004cf2 <BOOT_LoadImages+0x35a>
300052da:	f04f 0800 	mov.w	r8, #0
300052de:	e4fe      	b.n	30004cde <BOOT_LoadImages+0x346>
300052e0:	f8cd a014 	str.w	sl, [sp, #20]
300052e4:	46c2      	mov	sl, r8
300052e6:	e7c9      	b.n	3000527c <BOOT_LoadImages+0x8e4>
300052e8:	4862      	ldr	r0, [pc, #392]	; (30005474 <BOOT_LoadImages+0xadc>)
300052ea:	f007 f8a5 	bl	3000c438 <__DiagPrintf_veneer>
300052ee:	e69d      	b.n	3000502c <BOOT_LoadImages+0x694>
300052f0:	4620      	mov	r0, r4
300052f2:	e5db      	b.n	30004eac <BOOT_LoadImages+0x514>
300052f4:	45e2      	cmp	sl, ip
300052f6:	f67f abed 	bls.w	30004ad4 <BOOT_LoadImages+0x13c>
300052fa:	e432      	b.n	30004b62 <BOOT_LoadImages+0x1ca>
300052fc:	428a      	cmp	r2, r1
300052fe:	f4ff ad89 	bcc.w	30004e14 <BOOT_LoadImages+0x47c>
30005302:	4282      	cmp	r2, r0
30005304:	f4ff ad86 	bcc.w	30004e14 <BOOT_LoadImages+0x47c>
30005308:	2601      	movs	r6, #1
3000530a:	464b      	mov	r3, r9
3000530c:	9a07      	ldr	r2, [sp, #28]
3000530e:	4659      	mov	r1, fp
30005310:	4630      	mov	r0, r6
30005312:	9500      	str	r5, [sp, #0]
30005314:	9601      	str	r6, [sp, #4]
30005316:	f7fe fde7 	bl	30003ee8 <rewrite_bp>
3000531a:	f04f 0800 	mov.w	r8, #0
3000531e:	b2c3      	uxtb	r3, r0
30005320:	f8df 9180 	ldr.w	r9, [pc, #384]	; 300054a4 <BOOT_LoadImages+0xb0c>
30005324:	f8df b180 	ldr.w	fp, [pc, #384]	; 300054a8 <BOOT_LoadImages+0xb10>
30005328:	46c2      	mov	sl, r8
3000532a:	9303      	str	r3, [sp, #12]
3000532c:	4b54      	ldr	r3, [pc, #336]	; (30005480 <BOOT_LoadImages+0xae8>)
3000532e:	9304      	str	r3, [sp, #16]
30005330:	e48e      	b.n	30004c50 <BOOT_LoadImages+0x2b8>
30005332:	f8cd a014 	str.w	sl, [sp, #20]
30005336:	4639      	mov	r1, r7
30005338:	46ba      	mov	sl, r7
3000533a:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
3000533e:	e4d8      	b.n	30004cf2 <BOOT_LoadImages+0x35a>
30005340:	461a      	mov	r2, r3
30005342:	4619      	mov	r1, r3
30005344:	484f      	ldr	r0, [pc, #316]	; (30005484 <BOOT_LoadImages+0xaec>)
30005346:	f04f 0801 	mov.w	r8, #1
3000534a:	f007 f875 	bl	3000c438 <__DiagPrintf_veneer>
3000534e:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005352:	484d      	ldr	r0, [pc, #308]	; (30005488 <BOOT_LoadImages+0xaf0>)
30005354:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005358:	f007 f86e 	bl	3000c438 <__DiagPrintf_veneer>
3000535c:	2302      	movs	r3, #2
3000535e:	9303      	str	r3, [sp, #12]
30005360:	e46c      	b.n	30004c3c <BOOT_LoadImages+0x2a4>
30005362:	461a      	mov	r2, r3
30005364:	4619      	mov	r1, r3
30005366:	4849      	ldr	r0, [pc, #292]	; (3000548c <BOOT_LoadImages+0xaf4>)
30005368:	f007 f866 	bl	3000c438 <__DiagPrintf_veneer>
3000536c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005370:	4841      	ldr	r0, [pc, #260]	; (30005478 <BOOT_LoadImages+0xae0>)
30005372:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005376:	f007 f85f 	bl	3000c438 <__DiagPrintf_veneer>
3000537a:	2301      	movs	r3, #1
3000537c:	4698      	mov	r8, r3
3000537e:	9303      	str	r3, [sp, #12]
30005380:	e45c      	b.n	30004c3c <BOOT_LoadImages+0x2a4>
30005382:	461a      	mov	r2, r3
30005384:	4619      	mov	r1, r3
30005386:	483f      	ldr	r0, [pc, #252]	; (30005484 <BOOT_LoadImages+0xaec>)
30005388:	f007 f856 	bl	3000c438 <__DiagPrintf_veneer>
3000538c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005390:	483d      	ldr	r0, [pc, #244]	; (30005488 <BOOT_LoadImages+0xaf0>)
30005392:	f8d3 1006 	ldr.w	r1, [r3, #6]
30005396:	f007 f84f 	bl	3000c438 <__DiagPrintf_veneer>
3000539a:	2300      	movs	r3, #0
3000539c:	9305      	str	r3, [sp, #20]
3000539e:	e4e7      	b.n	30004d70 <BOOT_LoadImages+0x3d8>
300053a0:	483b      	ldr	r0, [pc, #236]	; (30005490 <BOOT_LoadImages+0xaf8>)
300053a2:	f007 f849 	bl	3000c438 <__DiagPrintf_veneer>
300053a6:	e7fe      	b.n	300053a6 <BOOT_LoadImages+0xa0e>
300053a8:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300053aa:	e5f8      	b.n	30004f9e <BOOT_LoadImages+0x606>
300053ac:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300053ae:	e42d      	b.n	30004c0c <BOOT_LoadImages+0x274>
300053b0:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
300053b4:	f11a 0f03 	cmn.w	sl, #3
300053b8:	d824      	bhi.n	30005404 <BOOT_LoadImages+0xa6c>
300053ba:	f1bb 0f01 	cmp.w	fp, #1
300053be:	d937      	bls.n	30005430 <BOOT_LoadImages+0xa98>
300053c0:	4834      	ldr	r0, [pc, #208]	; (30005494 <BOOT_LoadImages+0xafc>)
300053c2:	e640      	b.n	30005046 <BOOT_LoadImages+0x6ae>
300053c4:	4c34      	ldr	r4, [pc, #208]	; (30005498 <BOOT_LoadImages+0xb00>)
300053c6:	e4d9      	b.n	30004d7c <BOOT_LoadImages+0x3e4>
300053c8:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300053ca:	e61e      	b.n	3000500a <BOOT_LoadImages+0x672>
300053cc:	461a      	mov	r2, r3
300053ce:	4619      	mov	r1, r3
300053d0:	482e      	ldr	r0, [pc, #184]	; (3000548c <BOOT_LoadImages+0xaf4>)
300053d2:	f007 f831 	bl	3000c438 <__DiagPrintf_veneer>
300053d6:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300053da:	4827      	ldr	r0, [pc, #156]	; (30005478 <BOOT_LoadImages+0xae0>)
300053dc:	f8d3 1006 	ldr.w	r1, [r3, #6]
300053e0:	f007 f82a 	bl	3000c438 <__DiagPrintf_veneer>
300053e4:	2300      	movs	r3, #0
300053e6:	9305      	str	r3, [sp, #20]
300053e8:	e72a      	b.n	30005240 <BOOT_LoadImages+0x8a8>
300053ea:	4281      	cmp	r1, r0
300053ec:	f4ff ad18 	bcc.w	30004e20 <BOOT_LoadImages+0x488>
300053f0:	2400      	movs	r4, #0
300053f2:	464b      	mov	r3, r9
300053f4:	4642      	mov	r2, r8
300053f6:	4651      	mov	r1, sl
300053f8:	4620      	mov	r0, r4
300053fa:	9500      	str	r5, [sp, #0]
300053fc:	9401      	str	r4, [sp, #4]
300053fe:	e555      	b.n	30004eac <BOOT_LoadImages+0x514>
30005400:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30005402:	e6e9      	b.n	300051d8 <BOOT_LoadImages+0x840>
30005404:	4825      	ldr	r0, [pc, #148]	; (3000549c <BOOT_LoadImages+0xb04>)
30005406:	e61e      	b.n	30005046 <BOOT_LoadImages+0x6ae>
30005408:	2601      	movs	r6, #1
3000540a:	464b      	mov	r3, r9
3000540c:	9a07      	ldr	r2, [sp, #28]
3000540e:	4659      	mov	r1, fp
30005410:	2000      	movs	r0, #0
30005412:	9500      	str	r5, [sp, #0]
30005414:	9601      	str	r6, [sp, #4]
30005416:	e77e      	b.n	30005316 <BOOT_LoadImages+0x97e>
30005418:	2400      	movs	r4, #0
3000541a:	464b      	mov	r3, r9
3000541c:	4642      	mov	r2, r8
3000541e:	4651      	mov	r1, sl
30005420:	9500      	str	r5, [sp, #0]
30005422:	9401      	str	r4, [sp, #4]
30005424:	e541      	b.n	30004eaa <BOOT_LoadImages+0x512>
30005426:	f08a 0601 	eor.w	r6, sl, #1
3000542a:	f006 0601 	and.w	r6, r6, #1
3000542e:	e40f      	b.n	30004c50 <BOOT_LoadImages+0x2b8>
30005430:	9904      	ldr	r1, [sp, #16]
30005432:	481b      	ldr	r0, [pc, #108]	; (300054a0 <BOOT_LoadImages+0xb08>)
30005434:	f007 f800 	bl	3000c438 <__DiagPrintf_veneer>
30005438:	f8d9 3011 	ldr.w	r3, [r9, #17]
3000543c:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30005440:	9307      	str	r3, [sp, #28]
30005442:	2301      	movs	r3, #1
30005444:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30005448:	f8d5 8011 	ldr.w	r8, [r5, #17]
3000544c:	9309      	str	r3, [sp, #36]	; 0x24
3000544e:	e607      	b.n	30005060 <BOOT_LoadImages+0x6c8>
30005450:	f08a 0601 	eor.w	r6, sl, #1
30005454:	f8cd 8014 	str.w	r8, [sp, #20]
30005458:	f006 0601 	and.w	r6, r6, #1
3000545c:	f7ff bbf8 	b.w	30004c50 <BOOT_LoadImages+0x2b8>
30005460:	f04f 0a00 	mov.w	sl, #0
30005464:	e43b      	b.n	30004cde <BOOT_LoadImages+0x346>
30005466:	2601      	movs	r6, #1
30005468:	4c0b      	ldr	r4, [pc, #44]	; (30005498 <BOOT_LoadImages+0xb00>)
3000546a:	46b2      	mov	sl, r6
3000546c:	f7ff bbb4 	b.w	30004bd8 <BOOT_LoadImages+0x240>
30005470:	3000cb00 	.word	0x3000cb00
30005474:	3000caa4 	.word	0x3000caa4
30005478:	3000c7f4 	.word	0x3000c7f4
3000547c:	3000cab4 	.word	0x3000cab4
30005480:	3000e76f 	.word	0x3000e76f
30005484:	3000cb4c 	.word	0x3000cb4c
30005488:	3000c80c 	.word	0x3000c80c
3000548c:	3000cac4 	.word	0x3000cac4
30005490:	3000cc0c 	.word	0x3000cc0c
30005494:	3000cc6c 	.word	0x3000cc6c
30005498:	3000e934 	.word	0x3000e934
3000549c:	3000c8f4 	.word	0x3000c8f4
300054a0:	3000cca4 	.word	0x3000cca4
300054a4:	3000fd98 	.word	0x3000fd98
300054a8:	30006cd1 	.word	0x30006cd1

300054ac <BOOT_ReasonSet>:
300054ac:	4a03      	ldr	r2, [pc, #12]	; (300054bc <BOOT_ReasonSet+0x10>)
300054ae:	4904      	ldr	r1, [pc, #16]	; (300054c0 <BOOT_ReasonSet+0x14>)
300054b0:	8c13      	ldrh	r3, [r2, #32]
300054b2:	b29b      	uxth	r3, r3
300054b4:	8413      	strh	r3, [r2, #32]
300054b6:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
300054ba:	4770      	bx	lr
300054bc:	42008000 	.word	0x42008000
300054c0:	42008200 	.word	0x42008200

300054c4 <BOOT_Enable_KM0>:
300054c4:	b510      	push	{r4, lr}
300054c6:	4c12      	ldr	r4, [pc, #72]	; (30005510 <BOOT_Enable_KM0+0x4c>)
300054c8:	47a0      	blx	r4
300054ca:	b110      	cbz	r0, 300054d2 <BOOT_Enable_KM0+0xe>
300054cc:	4b11      	ldr	r3, [pc, #68]	; (30005514 <BOOT_Enable_KM0+0x50>)
300054ce:	4798      	blx	r3
300054d0:	b1a8      	cbz	r0, 300054fe <BOOT_Enable_KM0+0x3a>
300054d2:	47a0      	blx	r4
300054d4:	b940      	cbnz	r0, 300054e8 <BOOT_Enable_KM0+0x24>
300054d6:	4a10      	ldr	r2, [pc, #64]	; (30005518 <BOOT_Enable_KM0+0x54>)
300054d8:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300054dc:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300054e0:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300054e4:	bf40      	sev
300054e6:	bd10      	pop	{r4, pc}
300054e8:	4b0a      	ldr	r3, [pc, #40]	; (30005514 <BOOT_Enable_KM0+0x50>)
300054ea:	4798      	blx	r3
300054ec:	2800      	cmp	r0, #0
300054ee:	d0f2      	beq.n	300054d6 <BOOT_Enable_KM0+0x12>
300054f0:	4b0a      	ldr	r3, [pc, #40]	; (3000551c <BOOT_Enable_KM0+0x58>)
300054f2:	2004      	movs	r0, #4
300054f4:	4a0a      	ldr	r2, [pc, #40]	; (30005520 <BOOT_Enable_KM0+0x5c>)
300054f6:	4619      	mov	r1, r3
300054f8:	f006 fd28 	bl	3000bf4c <rtk_log_write>
300054fc:	e7eb      	b.n	300054d6 <BOOT_Enable_KM0+0x12>
300054fe:	4b07      	ldr	r3, [pc, #28]	; (3000551c <BOOT_Enable_KM0+0x58>)
30005500:	2004      	movs	r0, #4
30005502:	4a08      	ldr	r2, [pc, #32]	; (30005524 <BOOT_Enable_KM0+0x60>)
30005504:	4619      	mov	r1, r3
30005506:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000550a:	f006 bd1f 	b.w	3000bf4c <rtk_log_write>
3000550e:	bf00      	nop
30005510:	0000c09d 	.word	0x0000c09d
30005514:	0000c0ad 	.word	0x0000c0ad
30005518:	42008000 	.word	0x42008000
3000551c:	3000c710 	.word	0x3000c710
30005520:	3000cce8 	.word	0x3000cce8
30005524:	3000ccc0 	.word	0x3000ccc0

30005528 <BOOT_AP_Clk_Get>:
30005528:	4a06      	ldr	r2, [pc, #24]	; (30005544 <BOOT_AP_Clk_Get+0x1c>)
3000552a:	6913      	ldr	r3, [r2, #16]
3000552c:	2b00      	cmp	r3, #0
3000552e:	db03      	blt.n	30005538 <BOOT_AP_Clk_Get+0x10>
30005530:	6810      	ldr	r0, [r2, #0]
30005532:	fbb0 f0f3 	udiv	r0, r0, r3
30005536:	4770      	bx	lr
30005538:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
3000553c:	6850      	ldr	r0, [r2, #4]
3000553e:	fbb0 f0f3 	udiv	r0, r0, r3
30005542:	4770      	bx	lr
30005544:	3000efbc 	.word	0x3000efbc

30005548 <BOOT_SOC_ClkSet>:
30005548:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000554c:	4c5d      	ldr	r4, [pc, #372]	; (300056c4 <BOOT_SOC_ClkSet+0x17c>)
3000554e:	b083      	sub	sp, #12
30005550:	6926      	ldr	r6, [r4, #16]
30005552:	6825      	ldr	r5, [r4, #0]
30005554:	2e00      	cmp	r6, #0
30005556:	db7a      	blt.n	3000564e <BOOT_SOC_ClkSet+0x106>
30005558:	fbb5 f6f6 	udiv	r6, r5, r6
3000555c:	4b5a      	ldr	r3, [pc, #360]	; (300056c8 <BOOT_SOC_ClkSet+0x180>)
3000555e:	68a7      	ldr	r7, [r4, #8]
30005560:	fba3 1305 	umull	r1, r3, r3, r5
30005564:	68e2      	ldr	r2, [r4, #12]
30005566:	2f01      	cmp	r7, #1
30005568:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
3000556c:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30005570:	ea4f 1393 	mov.w	r3, r3, lsr #6
30005574:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005578:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
3000557c:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
30005580:	fbb5 f5f2 	udiv	r5, r5, r2
30005584:	d06f      	beq.n	30005666 <BOOT_SOC_ClkSet+0x11e>
30005586:	4b51      	ldr	r3, [pc, #324]	; (300056cc <BOOT_SOC_ClkSet+0x184>)
30005588:	2200      	movs	r2, #0
3000558a:	2dfa      	cmp	r5, #250	; 0xfa
3000558c:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30005590:	d863      	bhi.n	3000565a <BOOT_SOC_ClkSet+0x112>
30005592:	4f4f      	ldr	r7, [pc, #316]	; (300056d0 <BOOT_SOC_ClkSet+0x188>)
30005594:	783b      	ldrb	r3, [r7, #0]
30005596:	2b01      	cmp	r3, #1
30005598:	d103      	bne.n	300055a2 <BOOT_SOC_ClkSet+0x5a>
3000559a:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
3000559e:	f200 8085 	bhi.w	300056ac <BOOT_SOC_ClkSet+0x164>
300055a2:	2001      	movs	r0, #1
300055a4:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300056fc <BOOT_SOC_ClkSet+0x1b4>
300055a8:	f002 fc88 	bl	30007ebc <CLK_SWITCH_XTAL>
300055ac:	6820      	ldr	r0, [r4, #0]
300055ae:	4b49      	ldr	r3, [pc, #292]	; (300056d4 <BOOT_SOC_ClkSet+0x18c>)
300055b0:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
300055b4:	fb08 f000 	mul.w	r0, r8, r0
300055b8:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
300055bc:	4798      	blx	r3
300055be:	4a46      	ldr	r2, [pc, #280]	; (300056d8 <BOOT_SOC_ClkSet+0x190>)
300055c0:	68e1      	ldr	r1, [r4, #12]
300055c2:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300055c6:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300055ca:	ea4f 5909 	mov.w	r9, r9, lsl #20
300055ce:	1e4b      	subs	r3, r1, #1
300055d0:	4942      	ldr	r1, [pc, #264]	; (300056dc <BOOT_SOC_ClkSet+0x194>)
300055d2:	fa1f fb8b 	uxth.w	fp, fp
300055d6:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300055da:	4001      	ands	r1, r0
300055dc:	f003 0307 	and.w	r3, r3, #7
300055e0:	2000      	movs	r0, #0
300055e2:	430b      	orrs	r3, r1
300055e4:	ea4a 0303 	orr.w	r3, sl, r3
300055e8:	ea4b 0303 	orr.w	r3, fp, r3
300055ec:	ea49 0303 	orr.w	r3, r9, r3
300055f0:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300055f4:	f002 fc62 	bl	30007ebc <CLK_SWITCH_XTAL>
300055f8:	4b39      	ldr	r3, [pc, #228]	; (300056e0 <BOOT_SOC_ClkSet+0x198>)
300055fa:	4a3a      	ldr	r2, [pc, #232]	; (300056e4 <BOOT_SOC_ClkSet+0x19c>)
300055fc:	2004      	movs	r0, #4
300055fe:	4619      	mov	r1, r3
30005600:	9500      	str	r5, [sp, #0]
30005602:	f006 fca3 	bl	3000bf4c <rtk_log_write>
30005606:	783b      	ldrb	r3, [r7, #0]
30005608:	b1f3      	cbz	r3, 30005648 <BOOT_SOC_ClkSet+0x100>
3000560a:	6923      	ldr	r3, [r4, #16]
3000560c:	2b00      	cmp	r3, #0
3000560e:	db40      	blt.n	30005692 <BOOT_SOC_ClkSet+0x14a>
30005610:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005614:	492d      	ldr	r1, [pc, #180]	; (300056cc <BOOT_SOC_ClkSet+0x184>)
30005616:	2000      	movs	r0, #0
30005618:	6913      	ldr	r3, [r2, #16]
3000561a:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
3000561e:	f043 030c 	orr.w	r3, r3, #12
30005622:	6113      	str	r3, [r2, #16]
30005624:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
30005628:	6921      	ldr	r1, [r4, #16]
3000562a:	4b2d      	ldr	r3, [pc, #180]	; (300056e0 <BOOT_SOC_ClkSet+0x198>)
3000562c:	6928      	ldr	r0, [r5, #16]
3000562e:	3901      	subs	r1, #1
30005630:	4a2d      	ldr	r2, [pc, #180]	; (300056e8 <BOOT_SOC_ClkSet+0x1a0>)
30005632:	f020 0003 	bic.w	r0, r0, #3
30005636:	f001 0103 	and.w	r1, r1, #3
3000563a:	4301      	orrs	r1, r0
3000563c:	2004      	movs	r0, #4
3000563e:	6129      	str	r1, [r5, #16]
30005640:	4619      	mov	r1, r3
30005642:	9600      	str	r6, [sp, #0]
30005644:	f006 fc82 	bl	3000bf4c <rtk_log_write>
30005648:	b003      	add	sp, #12
3000564a:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000564e:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
30005652:	6863      	ldr	r3, [r4, #4]
30005654:	fbb3 f6f6 	udiv	r6, r3, r6
30005658:	e780      	b.n	3000555c <BOOT_SOC_ClkSet+0x14>
3000565a:	f240 61bf 	movw	r1, #1727	; 0x6bf
3000565e:	4823      	ldr	r0, [pc, #140]	; (300056ec <BOOT_SOC_ClkSet+0x1a4>)
30005660:	f006 ff6a 	bl	3000c538 <__io_assert_failed_veneer>
30005664:	e795      	b.n	30005592 <BOOT_SOC_ClkSet+0x4a>
30005666:	4b22      	ldr	r3, [pc, #136]	; (300056f0 <BOOT_SOC_ClkSet+0x1a8>)
30005668:	4638      	mov	r0, r7
3000566a:	4798      	blx	r3
3000566c:	4b17      	ldr	r3, [pc, #92]	; (300056cc <BOOT_SOC_ClkSet+0x184>)
3000566e:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
30005672:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
30005676:	d21f      	bcs.n	300056b8 <BOOT_SOC_ClkSet+0x170>
30005678:	4f15      	ldr	r7, [pc, #84]	; (300056d0 <BOOT_SOC_ClkSet+0x188>)
3000567a:	783b      	ldrb	r3, [r7, #0]
3000567c:	2b01      	cmp	r3, #1
3000567e:	d190      	bne.n	300055a2 <BOOT_SOC_ClkSet+0x5a>
30005680:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30005684:	d98d      	bls.n	300055a2 <BOOT_SOC_ClkSet+0x5a>
30005686:	f240 61bb 	movw	r1, #1723	; 0x6bb
3000568a:	4818      	ldr	r0, [pc, #96]	; (300056ec <BOOT_SOC_ClkSet+0x1a4>)
3000568c:	f006 ff54 	bl	3000c538 <__io_assert_failed_veneer>
30005690:	e787      	b.n	300055a2 <BOOT_SOC_ClkSet+0x5a>
30005692:	6860      	ldr	r0, [r4, #4]
30005694:	4b17      	ldr	r3, [pc, #92]	; (300056f4 <BOOT_SOC_ClkSet+0x1ac>)
30005696:	fb08 f000 	mul.w	r0, r8, r0
3000569a:	4798      	blx	r3
3000569c:	4b16      	ldr	r3, [pc, #88]	; (300056f8 <BOOT_SOC_ClkSet+0x1b0>)
3000569e:	2001      	movs	r0, #1
300056a0:	4798      	blx	r3
300056a2:	4b0a      	ldr	r3, [pc, #40]	; (300056cc <BOOT_SOC_ClkSet+0x184>)
300056a4:	2201      	movs	r2, #1
300056a6:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
300056aa:	e7bb      	b.n	30005624 <BOOT_SOC_ClkSet+0xdc>
300056ac:	f240 61c1 	movw	r1, #1729	; 0x6c1
300056b0:	480e      	ldr	r0, [pc, #56]	; (300056ec <BOOT_SOC_ClkSet+0x1a4>)
300056b2:	f006 ff41 	bl	3000c538 <__io_assert_failed_veneer>
300056b6:	e774      	b.n	300055a2 <BOOT_SOC_ClkSet+0x5a>
300056b8:	f240 61b9 	movw	r1, #1721	; 0x6b9
300056bc:	480b      	ldr	r0, [pc, #44]	; (300056ec <BOOT_SOC_ClkSet+0x1a4>)
300056be:	f006 ff3b 	bl	3000c538 <__io_assert_failed_veneer>
300056c2:	e7d9      	b.n	30005678 <BOOT_SOC_ClkSet+0x130>
300056c4:	3000efbc 	.word	0x3000efbc
300056c8:	51eb851f 	.word	0x51eb851f
300056cc:	23020000 	.word	0x23020000
300056d0:	3000e76f 	.word	0x3000e76f
300056d4:	30008ae5 	.word	0x30008ae5
300056d8:	42008000 	.word	0x42008000
300056dc:	ff8f08f8 	.word	0xff8f08f8
300056e0:	3000c710 	.word	0x3000c710
300056e4:	3000cd08 	.word	0x3000cd08
300056e8:	3000cd20 	.word	0x3000cd20
300056ec:	3000e434 	.word	0x3000e434
300056f0:	3000b8cd 	.word	0x3000b8cd
300056f4:	30008b65 	.word	0x30008b65
300056f8:	30008ba5 	.word	0x30008ba5
300056fc:	000f4240 	.word	0x000f4240

30005700 <BOOT_Disable_AP>:
30005700:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005704:	4906      	ldr	r1, [pc, #24]	; (30005720 <BOOT_Disable_AP+0x20>)
30005706:	685a      	ldr	r2, [r3, #4]
30005708:	f042 0202 	orr.w	r2, r2, #2
3000570c:	605a      	str	r2, [r3, #4]
3000570e:	685a      	ldr	r2, [r3, #4]
30005710:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30005714:	605a      	str	r2, [r3, #4]
30005716:	6f8b      	ldr	r3, [r1, #120]	; 0x78
30005718:	f043 0303 	orr.w	r3, r3, #3
3000571c:	678b      	str	r3, [r1, #120]	; 0x78
3000571e:	4770      	bx	lr
30005720:	41000200 	.word	0x41000200

30005724 <BOOT_Enable_AP>:
30005724:	b538      	push	{r3, r4, r5, lr}
30005726:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
3000572a:	2201      	movs	r2, #1
3000572c:	2104      	movs	r1, #4
3000572e:	4b2b      	ldr	r3, [pc, #172]	; (300057dc <BOOT_Enable_AP+0xb8>)
30005730:	482b      	ldr	r0, [pc, #172]	; (300057e0 <BOOT_Enable_AP+0xbc>)
30005732:	4798      	blx	r3
30005734:	6863      	ldr	r3, [r4, #4]
30005736:	4d2b      	ldr	r5, [pc, #172]	; (300057e4 <BOOT_Enable_AP+0xc0>)
30005738:	2032      	movs	r0, #50	; 0x32
3000573a:	f043 0302 	orr.w	r3, r3, #2
3000573e:	6063      	str	r3, [r4, #4]
30005740:	6863      	ldr	r3, [r4, #4]
30005742:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30005746:	6063      	str	r3, [r4, #4]
30005748:	6823      	ldr	r3, [r4, #0]
3000574a:	f043 0302 	orr.w	r3, r3, #2
3000574e:	6023      	str	r3, [r4, #0]
30005750:	47a8      	blx	r5
30005752:	6823      	ldr	r3, [r4, #0]
30005754:	2032      	movs	r0, #50	; 0x32
30005756:	f043 0304 	orr.w	r3, r3, #4
3000575a:	6023      	str	r3, [r4, #0]
3000575c:	6823      	ldr	r3, [r4, #0]
3000575e:	f043 0301 	orr.w	r3, r3, #1
30005762:	6023      	str	r3, [r4, #0]
30005764:	6823      	ldr	r3, [r4, #0]
30005766:	f043 0310 	orr.w	r3, r3, #16
3000576a:	6023      	str	r3, [r4, #0]
3000576c:	47a8      	blx	r5
3000576e:	6823      	ldr	r3, [r4, #0]
30005770:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30005774:	f043 0340 	orr.w	r3, r3, #64	; 0x40
30005778:	6023      	str	r3, [r4, #0]
3000577a:	47a8      	blx	r5
3000577c:	6863      	ldr	r3, [r4, #4]
3000577e:	4a1a      	ldr	r2, [pc, #104]	; (300057e8 <BOOT_Enable_AP+0xc4>)
30005780:	f023 0310 	bic.w	r3, r3, #16
30005784:	6063      	str	r3, [r4, #4]
30005786:	6863      	ldr	r3, [r4, #4]
30005788:	f023 0302 	bic.w	r3, r3, #2
3000578c:	6063      	str	r3, [r4, #4]
3000578e:	4790      	blx	r2
30005790:	b178      	cbz	r0, 300057b2 <BOOT_Enable_AP+0x8e>
30005792:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30005796:	4815      	ldr	r0, [pc, #84]	; (300057ec <BOOT_Enable_AP+0xc8>)
30005798:	4a15      	ldr	r2, [pc, #84]	; (300057f0 <BOOT_Enable_AP+0xcc>)
3000579a:	68d9      	ldr	r1, [r3, #12]
3000579c:	f041 0101 	orr.w	r1, r1, #1
300057a0:	60d9      	str	r1, [r3, #12]
300057a2:	6899      	ldr	r1, [r3, #8]
300057a4:	f041 0101 	orr.w	r1, r1, #1
300057a8:	6099      	str	r1, [r3, #8]
300057aa:	6843      	ldr	r3, [r0, #4]
300057ac:	431a      	orrs	r2, r3
300057ae:	6042      	str	r2, [r0, #4]
300057b0:	bd38      	pop	{r3, r4, r5, pc}
300057b2:	4604      	mov	r4, r0
300057b4:	4d0f      	ldr	r5, [pc, #60]	; (300057f4 <BOOT_Enable_AP+0xd0>)
300057b6:	4601      	mov	r1, r0
300057b8:	2041      	movs	r0, #65	; 0x41
300057ba:	47a8      	blx	r5
300057bc:	4621      	mov	r1, r4
300057be:	2042      	movs	r0, #66	; 0x42
300057c0:	47a8      	blx	r5
300057c2:	4621      	mov	r1, r4
300057c4:	2043      	movs	r0, #67	; 0x43
300057c6:	47a8      	blx	r5
300057c8:	4621      	mov	r1, r4
300057ca:	2044      	movs	r0, #68	; 0x44
300057cc:	47a8      	blx	r5
300057ce:	4621      	mov	r1, r4
300057d0:	2045      	movs	r0, #69	; 0x45
300057d2:	47a8      	blx	r5
300057d4:	4621      	mov	r1, r4
300057d6:	2046      	movs	r0, #70	; 0x46
300057d8:	47a8      	blx	r5
300057da:	e7da      	b.n	30005792 <BOOT_Enable_AP+0x6e>
300057dc:	0000aab9 	.word	0x0000aab9
300057e0:	4200c000 	.word	0x4200c000
300057e4:	00009b2d 	.word	0x00009b2d
300057e8:	30009c5d 	.word	0x30009c5d
300057ec:	41000200 	.word	0x41000200
300057f0:	01001111 	.word	0x01001111
300057f4:	0000b20d 	.word	0x0000b20d

300057f8 <BOOT_RAM_KeyDeriveFunc>:
300057f8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300057fc:	ed2d 8b04 	vpush	{d8-d9}
30005800:	b0cb      	sub	sp, #300	; 0x12c
30005802:	4c6a      	ldr	r4, [pc, #424]	; (300059ac <BOOT_RAM_KeyDeriveFunc+0x1b4>)
30005804:	4680      	mov	r8, r0
30005806:	4689      	mov	r9, r1
30005808:	4869      	ldr	r0, [pc, #420]	; (300059b0 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
3000580a:	ee08 2a90 	vmov	s17, r2
3000580e:	4969      	ldr	r1, [pc, #420]	; (300059b4 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005810:	2201      	movs	r2, #1
30005812:	ee09 3a10 	vmov	s18, r3
30005816:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
3000581a:	47a0      	blx	r4
3000581c:	2201      	movs	r2, #1
3000581e:	4966      	ldr	r1, [pc, #408]	; (300059b8 <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30005820:	ae02      	add	r6, sp, #8
30005822:	4864      	ldr	r0, [pc, #400]	; (300059b4 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005824:	47a0      	blx	r4
30005826:	4b65      	ldr	r3, [pc, #404]	; (300059bc <BOOT_RAM_KeyDeriveFunc+0x1c4>)
30005828:	2000      	movs	r0, #0
3000582a:	4798      	blx	r3
3000582c:	22e0      	movs	r2, #224	; 0xe0
3000582e:	2100      	movs	r1, #0
30005830:	a812      	add	r0, sp, #72	; 0x48
30005832:	f006 fdd1 	bl	3000c3d8 <____wrap_memset_veneer>
30005836:	2300      	movs	r3, #0
30005838:	f8ad 3004 	strh.w	r3, [sp, #4]
3000583c:	f88d 3006 	strb.w	r3, [sp, #6]
30005840:	2301      	movs	r3, #1
30005842:	f88d 3007 	strb.w	r3, [sp, #7]
30005846:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005848:	2b00      	cmp	r3, #0
3000584a:	f000 80ab 	beq.w	300059a4 <BOOT_RAM_KeyDeriveFunc+0x1ac>
3000584e:	ab0a      	add	r3, sp, #40	; 0x28
30005850:	f8df b17c 	ldr.w	fp, [pc, #380]	; 300059d0 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30005854:	4f5a      	ldr	r7, [pc, #360]	; (300059c0 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
30005856:	ee08 3a10 	vmov	s16, r3
3000585a:	ab12      	add	r3, sp, #72	; 0x48
3000585c:	464a      	mov	r2, r9
3000585e:	4641      	mov	r1, r8
30005860:	2020      	movs	r0, #32
30005862:	47d8      	blx	fp
30005864:	aa12      	add	r2, sp, #72	; 0x48
30005866:	ee19 1a10 	vmov	r1, s18
3000586a:	ee18 0a90 	vmov	r0, s17
3000586e:	47b8      	blx	r7
30005870:	ee18 5a10 	vmov	r5, s16
30005874:	aa12      	add	r2, sp, #72	; 0x48
30005876:	2104      	movs	r1, #4
30005878:	a801      	add	r0, sp, #4
3000587a:	4634      	mov	r4, r6
3000587c:	47b8      	blx	r7
3000587e:	a912      	add	r1, sp, #72	; 0x48
30005880:	4b50      	ldr	r3, [pc, #320]	; (300059c4 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005882:	ee18 0a10 	vmov	r0, s16
30005886:	4798      	blx	r3
30005888:	f1ba 0f01 	cmp.w	sl, #1
3000588c:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
3000588e:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30005890:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30005894:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30005898:	d91f      	bls.n	300058da <BOOT_RAM_KeyDeriveFunc+0xe2>
3000589a:	2501      	movs	r5, #1
3000589c:	f10d 0447 	add.w	r4, sp, #71	; 0x47
300058a0:	ab12      	add	r3, sp, #72	; 0x48
300058a2:	4641      	mov	r1, r8
300058a4:	464a      	mov	r2, r9
300058a6:	2020      	movs	r0, #32
300058a8:	47d8      	blx	fp
300058aa:	aa12      	add	r2, sp, #72	; 0x48
300058ac:	2120      	movs	r1, #32
300058ae:	4630      	mov	r0, r6
300058b0:	47b8      	blx	r7
300058b2:	a912      	add	r1, sp, #72	; 0x48
300058b4:	4b43      	ldr	r3, [pc, #268]	; (300059c4 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
300058b6:	4630      	mov	r0, r6
300058b8:	4798      	blx	r3
300058ba:	f10d 0327 	add.w	r3, sp, #39	; 0x27
300058be:	f10d 0107 	add.w	r1, sp, #7
300058c2:	f813 2f01 	ldrb.w	r2, [r3, #1]!
300058c6:	f811 0f01 	ldrb.w	r0, [r1, #1]!
300058ca:	42a3      	cmp	r3, r4
300058cc:	ea82 0200 	eor.w	r2, r2, r0
300058d0:	701a      	strb	r2, [r3, #0]
300058d2:	d1f6      	bne.n	300058c2 <BOOT_RAM_KeyDeriveFunc+0xca>
300058d4:	3501      	adds	r5, #1
300058d6:	45aa      	cmp	sl, r5
300058d8:	d1e2      	bne.n	300058a0 <BOOT_RAM_KeyDeriveFunc+0xa8>
300058da:	9a59      	ldr	r2, [sp, #356]	; 0x164
300058dc:	ee18 1a10 	vmov	r1, s16
300058e0:	9b59      	ldr	r3, [sp, #356]	; 0x164
300058e2:	2a20      	cmp	r2, #32
300058e4:	985a      	ldr	r0, [sp, #360]	; 0x168
300058e6:	bf28      	it	cs
300058e8:	2220      	movcs	r2, #32
300058ea:	1a9b      	subs	r3, r3, r2
300058ec:	9359      	str	r3, [sp, #356]	; 0x164
300058ee:	4603      	mov	r3, r0
300058f0:	4413      	add	r3, r2
300058f2:	935a      	str	r3, [sp, #360]	; 0x168
300058f4:	f006 fd80 	bl	3000c3f8 <____wrap_memcpy_veneer>
300058f8:	f10d 0207 	add.w	r2, sp, #7
300058fc:	2104      	movs	r1, #4
300058fe:	7813      	ldrb	r3, [r2, #0]
30005900:	3901      	subs	r1, #1
30005902:	3301      	adds	r3, #1
30005904:	b2db      	uxtb	r3, r3
30005906:	f802 3901 	strb.w	r3, [r2], #-1
3000590a:	b90b      	cbnz	r3, 30005910 <BOOT_RAM_KeyDeriveFunc+0x118>
3000590c:	2900      	cmp	r1, #0
3000590e:	d1f6      	bne.n	300058fe <BOOT_RAM_KeyDeriveFunc+0x106>
30005910:	9b59      	ldr	r3, [sp, #356]	; 0x164
30005912:	2b00      	cmp	r3, #0
30005914:	d1a1      	bne.n	3000585a <BOOT_RAM_KeyDeriveFunc+0x62>
30005916:	2220      	movs	r2, #32
30005918:	2100      	movs	r1, #0
3000591a:	4c2b      	ldr	r4, [pc, #172]	; (300059c8 <BOOT_RAM_KeyDeriveFunc+0x1d0>)
3000591c:	4630      	mov	r0, r6
3000591e:	47a0      	blx	r4
30005920:	2220      	movs	r2, #32
30005922:	2100      	movs	r1, #0
30005924:	ee18 0a10 	vmov	r0, s16
30005928:	47a0      	blx	r4
3000592a:	4b28      	ldr	r3, [pc, #160]	; (300059cc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000592c:	695b      	ldr	r3, [r3, #20]
3000592e:	03da      	lsls	r2, r3, #15
30005930:	d512      	bpl.n	30005958 <BOOT_RAM_KeyDeriveFunc+0x160>
30005932:	f016 021f 	ands.w	r2, r6, #31
30005936:	4633      	mov	r3, r6
30005938:	d130      	bne.n	3000599c <BOOT_RAM_KeyDeriveFunc+0x1a4>
3000593a:	2220      	movs	r2, #32
3000593c:	f3bf 8f4f 	dsb	sy
30005940:	4822      	ldr	r0, [pc, #136]	; (300059cc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005942:	441a      	add	r2, r3
30005944:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005948:	3320      	adds	r3, #32
3000594a:	1ad1      	subs	r1, r2, r3
3000594c:	2900      	cmp	r1, #0
3000594e:	dcf9      	bgt.n	30005944 <BOOT_RAM_KeyDeriveFunc+0x14c>
30005950:	f3bf 8f4f 	dsb	sy
30005954:	f3bf 8f6f 	isb	sy
30005958:	4b1c      	ldr	r3, [pc, #112]	; (300059cc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000595a:	695b      	ldr	r3, [r3, #20]
3000595c:	03db      	lsls	r3, r3, #15
3000595e:	d513      	bpl.n	30005988 <BOOT_RAM_KeyDeriveFunc+0x190>
30005960:	ee18 3a10 	vmov	r3, s16
30005964:	f013 021f 	ands.w	r2, r3, #31
30005968:	d114      	bne.n	30005994 <BOOT_RAM_KeyDeriveFunc+0x19c>
3000596a:	2220      	movs	r2, #32
3000596c:	f3bf 8f4f 	dsb	sy
30005970:	4816      	ldr	r0, [pc, #88]	; (300059cc <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005972:	441a      	add	r2, r3
30005974:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
30005978:	3320      	adds	r3, #32
3000597a:	1ad1      	subs	r1, r2, r3
3000597c:	2900      	cmp	r1, #0
3000597e:	dcf9      	bgt.n	30005974 <BOOT_RAM_KeyDeriveFunc+0x17c>
30005980:	f3bf 8f4f 	dsb	sy
30005984:	f3bf 8f6f 	isb	sy
30005988:	2000      	movs	r0, #0
3000598a:	b04b      	add	sp, #300	; 0x12c
3000598c:	ecbd 8b04 	vpop	{d8-d9}
30005990:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005994:	f023 031f 	bic.w	r3, r3, #31
30005998:	3220      	adds	r2, #32
3000599a:	e7e7      	b.n	3000596c <BOOT_RAM_KeyDeriveFunc+0x174>
3000599c:	f026 031f 	bic.w	r3, r6, #31
300059a0:	3220      	adds	r2, #32
300059a2:	e7cb      	b.n	3000593c <BOOT_RAM_KeyDeriveFunc+0x144>
300059a4:	ab0a      	add	r3, sp, #40	; 0x28
300059a6:	ee08 3a10 	vmov	s16, r3
300059aa:	e7b4      	b.n	30005916 <BOOT_RAM_KeyDeriveFunc+0x11e>
300059ac:	0000b479 	.word	0x0000b479
300059b0:	40000008 	.word	0x40000008
300059b4:	40000004 	.word	0x40000004
300059b8:	40000002 	.word	0x40000002
300059bc:	30009ef9 	.word	0x30009ef9
300059c0:	30009cfd 	.word	0x30009cfd
300059c4:	30009d39 	.word	0x30009d39
300059c8:	00012be5 	.word	0x00012be5
300059cc:	e000ed00 	.word	0xe000ed00
300059d0:	3000957d 	.word	0x3000957d

300059d4 <password_hash_check>:
300059d4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300059d8:	b08f      	sub	sp, #60	; 0x3c
300059da:	4690      	mov	r8, r2
300059dc:	461d      	mov	r5, r3
300059de:	4606      	mov	r6, r0
300059e0:	af04      	add	r7, sp, #16
300059e2:	3201      	adds	r2, #1
300059e4:	6d3b      	ldr	r3, [r7, #80]	; 0x50
300059e6:	6079      	str	r1, [r7, #4]
300059e8:	2100      	movs	r1, #0
300059ea:	f103 0008 	add.w	r0, r3, #8
300059ee:	f108 0308 	add.w	r3, r8, #8
300059f2:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
300059f6:	f023 0307 	bic.w	r3, r3, #7
300059fa:	f020 0007 	bic.w	r0, r0, #7
300059fe:	ebad 0d03 	sub.w	sp, sp, r3
30005a02:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30005a04:	f10d 0910 	add.w	r9, sp, #16
30005a08:	3301      	adds	r3, #1
30005a0a:	ebad 0d00 	sub.w	sp, sp, r0
30005a0e:	603b      	str	r3, [r7, #0]
30005a10:	4648      	mov	r0, r9
30005a12:	4b3f      	ldr	r3, [pc, #252]	; (30005b10 <password_hash_check+0x13c>)
30005a14:	f10d 0a10 	add.w	sl, sp, #16
30005a18:	4798      	blx	r3
30005a1a:	683b      	ldr	r3, [r7, #0]
30005a1c:	2100      	movs	r1, #0
30005a1e:	4650      	mov	r0, sl
30005a20:	461a      	mov	r2, r3
30005a22:	4b3b      	ldr	r3, [pc, #236]	; (30005b10 <password_hash_check+0x13c>)
30005a24:	4798      	blx	r3
30005a26:	f1b8 0001 	subs.w	r0, r8, #1
30005a2a:	d01d      	beq.n	30005a68 <password_hash_check+0x94>
30005a2c:	2200      	movs	r2, #0
30005a2e:	f109 33ff 	add.w	r3, r9, #4294967295	; 0xffffffff
30005a32:	4611      	mov	r1, r2
30005a34:	e00b      	b.n	30005a4e <password_hash_check+0x7a>
30005a36:	f108 38ff 	add.w	r8, r8, #4294967295	; 0xffffffff
30005a3a:	2100      	movs	r1, #0
30005a3c:	1c9c      	adds	r4, r3, #2
30005a3e:	4282      	cmp	r2, r0
30005a40:	7059      	strb	r1, [r3, #1]
30005a42:	f103 0301 	add.w	r3, r3, #1
30005a46:	4611      	mov	r1, r2
30005a48:	eba4 0409 	sub.w	r4, r4, r9
30005a4c:	d20b      	bcs.n	30005a66 <password_hash_check+0x92>
30005a4e:	1874      	adds	r4, r6, r1
30005a50:	f102 0c01 	add.w	ip, r2, #1
30005a54:	5c71      	ldrb	r1, [r6, r1]
30005a56:	3202      	adds	r2, #2
30005a58:	7864      	ldrb	r4, [r4, #1]
30005a5a:	2c30      	cmp	r4, #48	; 0x30
30005a5c:	d101      	bne.n	30005a62 <password_hash_check+0x8e>
30005a5e:	295c      	cmp	r1, #92	; 0x5c
30005a60:	d0e9      	beq.n	30005a36 <password_hash_check+0x62>
30005a62:	4662      	mov	r2, ip
30005a64:	e7ea      	b.n	30005a3c <password_hash_check+0x68>
30005a66:	4406      	add	r6, r0
30005a68:	7833      	ldrb	r3, [r6, #0]
30005a6a:	f809 3004 	strb.w	r3, [r9, r4]
30005a6e:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30005a70:	1e5c      	subs	r4, r3, #1
30005a72:	d04a      	beq.n	30005b0a <password_hash_check+0x136>
30005a74:	2200      	movs	r2, #0
30005a76:	f10a 33ff 	add.w	r3, sl, #4294967295	; 0xffffffff
30005a7a:	4611      	mov	r1, r2
30005a7c:	e00b      	b.n	30005a96 <password_hash_check+0xc2>
30005a7e:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005a82:	2100      	movs	r1, #0
30005a84:	1c98      	adds	r0, r3, #2
30005a86:	42a2      	cmp	r2, r4
30005a88:	7059      	strb	r1, [r3, #1]
30005a8a:	f103 0301 	add.w	r3, r3, #1
30005a8e:	4611      	mov	r1, r2
30005a90:	eba0 000a 	sub.w	r0, r0, sl
30005a94:	d20a      	bcs.n	30005aac <password_hash_check+0xd8>
30005a96:	1868      	adds	r0, r5, r1
30005a98:	1c56      	adds	r6, r2, #1
30005a9a:	5c69      	ldrb	r1, [r5, r1]
30005a9c:	3202      	adds	r2, #2
30005a9e:	7840      	ldrb	r0, [r0, #1]
30005aa0:	2830      	cmp	r0, #48	; 0x30
30005aa2:	d101      	bne.n	30005aa8 <password_hash_check+0xd4>
30005aa4:	295c      	cmp	r1, #92	; 0x5c
30005aa6:	d0ea      	beq.n	30005a7e <password_hash_check+0xaa>
30005aa8:	4632      	mov	r2, r6
30005aaa:	e7eb      	b.n	30005a84 <password_hash_check+0xb0>
30005aac:	4425      	add	r5, r4
30005aae:	782b      	ldrb	r3, [r5, #0]
30005ab0:	f107 0408 	add.w	r4, r7, #8
30005ab4:	2520      	movs	r5, #32
30005ab6:	4641      	mov	r1, r8
30005ab8:	f80a 3000 	strb.w	r3, [sl, r0]
30005abc:	4652      	mov	r2, sl
30005abe:	687b      	ldr	r3, [r7, #4]
30005ac0:	4648      	mov	r0, r9
30005ac2:	9402      	str	r4, [sp, #8]
30005ac4:	9300      	str	r3, [sp, #0]
30005ac6:	465b      	mov	r3, fp
30005ac8:	9501      	str	r5, [sp, #4]
30005aca:	f7ff fe95 	bl	300057f8 <BOOT_RAM_KeyDeriveFunc>
30005ace:	4620      	mov	r0, r4
30005ad0:	462a      	mov	r2, r5
30005ad2:	4910      	ldr	r1, [pc, #64]	; (30005b14 <password_hash_check+0x140>)
30005ad4:	f006 fcf8 	bl	3000c4c8 <____wrap_memcmp_veneer>
30005ad8:	4604      	mov	r4, r0
30005ada:	b968      	cbnz	r0, 30005af8 <password_hash_check+0x124>
30005adc:	480e      	ldr	r0, [pc, #56]	; (30005b18 <password_hash_check+0x144>)
30005ade:	f006 fcab 	bl	3000c438 <__DiagPrintf_veneer>
30005ae2:	462a      	mov	r2, r5
30005ae4:	4621      	mov	r1, r4
30005ae6:	f107 0008 	add.w	r0, r7, #8
30005aea:	4b09      	ldr	r3, [pc, #36]	; (30005b10 <password_hash_check+0x13c>)
30005aec:	4798      	blx	r3
30005aee:	4620      	mov	r0, r4
30005af0:	372c      	adds	r7, #44	; 0x2c
30005af2:	46bd      	mov	sp, r7
30005af4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005af8:	2401      	movs	r4, #1
30005afa:	4808      	ldr	r0, [pc, #32]	; (30005b1c <password_hash_check+0x148>)
30005afc:	f006 fc9c 	bl	3000c438 <__DiagPrintf_veneer>
30005b00:	372c      	adds	r7, #44	; 0x2c
30005b02:	4620      	mov	r0, r4
30005b04:	46bd      	mov	sp, r7
30005b06:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005b0a:	4620      	mov	r0, r4
30005b0c:	e7cf      	b.n	30005aae <password_hash_check+0xda>
30005b0e:	bf00      	nop
30005b10:	00012be5 	.word	0x00012be5
30005b14:	3000ed3c 	.word	0x3000ed3c
30005b18:	3000cd50 	.word	0x3000cd50
30005b1c:	3000cd38 	.word	0x3000cd38

30005b20 <BOOT_Share_Memory_Patch>:
30005b20:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005b24:	4817      	ldr	r0, [pc, #92]	; (30005b84 <BOOT_Share_Memory_Patch+0x64>)
30005b26:	4918      	ldr	r1, [pc, #96]	; (30005b88 <BOOT_Share_Memory_Patch+0x68>)
30005b28:	b430      	push	{r4, r5}
30005b2a:	6b13      	ldr	r3, [r2, #48]	; 0x30
30005b2c:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30005b30:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30005b34:	6313      	str	r3, [r2, #48]	; 0x30
30005b36:	6823      	ldr	r3, [r4, #0]
30005b38:	6804      	ldr	r4, [r0, #0]
30005b3a:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b3e:	6809      	ldr	r1, [r1, #0]
30005b40:	4423      	add	r3, r4
30005b42:	6805      	ldr	r5, [r0, #0]
30005b44:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30005b48:	440b      	add	r3, r1
30005b4a:	4910      	ldr	r1, [pc, #64]	; (30005b8c <BOOT_Share_Memory_Patch+0x6c>)
30005b4c:	6808      	ldr	r0, [r1, #0]
30005b4e:	442b      	add	r3, r5
30005b50:	6b11      	ldr	r1, [r2, #48]	; 0x30
30005b52:	4403      	add	r3, r0
30005b54:	480e      	ldr	r0, [pc, #56]	; (30005b90 <BOOT_Share_Memory_Patch+0x70>)
30005b56:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30005b5a:	6311      	str	r1, [r2, #48]	; 0x30
30005b5c:	6821      	ldr	r1, [r4, #0]
30005b5e:	4a0d      	ldr	r2, [pc, #52]	; (30005b94 <BOOT_Share_Memory_Patch+0x74>)
30005b60:	6804      	ldr	r4, [r0, #0]
30005b62:	440b      	add	r3, r1
30005b64:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b68:	6811      	ldr	r1, [r2, #0]
30005b6a:	4423      	add	r3, r4
30005b6c:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30005b70:	6800      	ldr	r0, [r0, #0]
30005b72:	440b      	add	r3, r1
30005b74:	6812      	ldr	r2, [r2, #0]
30005b76:	4908      	ldr	r1, [pc, #32]	; (30005b98 <BOOT_Share_Memory_Patch+0x78>)
30005b78:	4403      	add	r3, r0
30005b7a:	6808      	ldr	r0, [r1, #0]
30005b7c:	4413      	add	r3, r2
30005b7e:	bc30      	pop	{r4, r5}
30005b80:	4418      	add	r0, r3
30005b82:	4770      	bx	lr
30005b84:	2200c000 	.word	0x2200c000
30005b88:	22018000 	.word	0x22018000
30005b8c:	22100000 	.word	0x22100000
30005b90:	20010000 	.word	0x20010000
30005b94:	20020000 	.word	0x20020000
30005b98:	23010000 	.word	0x23010000

30005b9c <BOOT_Image1>:
30005b9c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005ba0:	4b88      	ldr	r3, [pc, #544]	; (30005dc4 <BOOT_Image1+0x228>)
30005ba2:	b087      	sub	sp, #28
30005ba4:	4798      	blx	r3
30005ba6:	2800      	cmp	r0, #0
30005ba8:	f040 80c3 	bne.w	30005d32 <BOOT_Image1+0x196>
30005bac:	4886      	ldr	r0, [pc, #536]	; (30005dc8 <BOOT_Image1+0x22c>)
30005bae:	f006 fc43 	bl	3000c438 <__DiagPrintf_veneer>
30005bb2:	4986      	ldr	r1, [pc, #536]	; (30005dcc <BOOT_Image1+0x230>)
30005bb4:	4886      	ldr	r0, [pc, #536]	; (30005dd0 <BOOT_Image1+0x234>)
30005bb6:	f006 fc3f 	bl	3000c438 <__DiagPrintf_veneer>
30005bba:	4886      	ldr	r0, [pc, #536]	; (30005dd4 <BOOT_Image1+0x238>)
30005bbc:	4a86      	ldr	r2, [pc, #536]	; (30005dd8 <BOOT_Image1+0x23c>)
30005bbe:	2100      	movs	r1, #0
30005bc0:	4b86      	ldr	r3, [pc, #536]	; (30005ddc <BOOT_Image1+0x240>)
30005bc2:	1a12      	subs	r2, r2, r0
30005bc4:	4798      	blx	r3
30005bc6:	4a86      	ldr	r2, [pc, #536]	; (30005de0 <BOOT_Image1+0x244>)
30005bc8:	4986      	ldr	r1, [pc, #536]	; (30005de4 <BOOT_Image1+0x248>)
30005bca:	8c13      	ldrh	r3, [r2, #32]
30005bcc:	b29b      	uxth	r3, r3
30005bce:	8413      	strh	r3, [r2, #32]
30005bd0:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005bd4:	f003 fbf4 	bl	300093c0 <BOOT_Reason>
30005bd8:	2800      	cmp	r0, #0
30005bda:	f000 80eb 	beq.w	30005db4 <BOOT_Image1+0x218>
30005bde:	4e82      	ldr	r6, [pc, #520]	; (30005de8 <BOOT_Image1+0x24c>)
30005be0:	4b82      	ldr	r3, [pc, #520]	; (30005dec <BOOT_Image1+0x250>)
30005be2:	4798      	blx	r3
30005be4:	7834      	ldrb	r4, [r6, #0]
30005be6:	2c00      	cmp	r4, #0
30005be8:	f000 8099 	beq.w	30005d1e <BOOT_Image1+0x182>
30005bec:	2000      	movs	r0, #0
30005bee:	4b80      	ldr	r3, [pc, #512]	; (30005df0 <BOOT_Image1+0x254>)
30005bf0:	4798      	blx	r3
30005bf2:	4b80      	ldr	r3, [pc, #512]	; (30005df4 <BOOT_Image1+0x258>)
30005bf4:	2001      	movs	r0, #1
30005bf6:	4798      	blx	r3
30005bf8:	4c7a      	ldr	r4, [pc, #488]	; (30005de4 <BOOT_Image1+0x248>)
30005bfa:	f7ff fca5 	bl	30005548 <BOOT_SOC_ClkSet>
30005bfe:	f241 070a 	movw	r7, #4106	; 0x100a
30005c02:	2028      	movs	r0, #40	; 0x28
30005c04:	8823      	ldrh	r3, [r4, #0]
30005c06:	4d7c      	ldr	r5, [pc, #496]	; (30005df8 <BOOT_Image1+0x25c>)
30005c08:	b29b      	uxth	r3, r3
30005c0a:	431f      	orrs	r7, r3
30005c0c:	4b7b      	ldr	r3, [pc, #492]	; (30005dfc <BOOT_Image1+0x260>)
30005c0e:	8027      	strh	r7, [r4, #0]
30005c10:	4798      	blx	r3
30005c12:	88a3      	ldrh	r3, [r4, #4]
30005c14:	b29b      	uxth	r3, r3
30005c16:	ea23 0307 	bic.w	r3, r3, r7
30005c1a:	80a3      	strh	r3, [r4, #4]
30005c1c:	8863      	ldrh	r3, [r4, #2]
30005c1e:	8067      	strh	r7, [r4, #2]
30005c20:	47a8      	blx	r5
30005c22:	2802      	cmp	r0, #2
30005c24:	f000 808d 	beq.w	30005d42 <BOOT_Image1+0x1a6>
30005c28:	4b75      	ldr	r3, [pc, #468]	; (30005e00 <BOOT_Image1+0x264>)
30005c2a:	2401      	movs	r4, #1
30005c2c:	4975      	ldr	r1, [pc, #468]	; (30005e04 <BOOT_Image1+0x268>)
30005c2e:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005c32:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30005c36:	4622      	mov	r2, r4
30005c38:	4f73      	ldr	r7, [pc, #460]	; (30005e08 <BOOT_Image1+0x26c>)
30005c3a:	47b8      	blx	r7
30005c3c:	4622      	mov	r2, r4
30005c3e:	4973      	ldr	r1, [pc, #460]	; (30005e0c <BOOT_Image1+0x270>)
30005c40:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005c44:	47b8      	blx	r7
30005c46:	4a66      	ldr	r2, [pc, #408]	; (30005de0 <BOOT_Image1+0x244>)
30005c48:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005c4c:	f043 0303 	orr.w	r3, r3, #3
30005c50:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005c54:	4b6e      	ldr	r3, [pc, #440]	; (30005e10 <BOOT_Image1+0x274>)
30005c56:	4798      	blx	r3
30005c58:	4a61      	ldr	r2, [pc, #388]	; (30005de0 <BOOT_Image1+0x244>)
30005c5a:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005c5e:	f023 0303 	bic.w	r3, r3, #3
30005c62:	b108      	cbz	r0, 30005c68 <BOOT_Image1+0xcc>
30005c64:	f043 0302 	orr.w	r3, r3, #2
30005c68:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005c6c:	4b69      	ldr	r3, [pc, #420]	; (30005e14 <BOOT_Image1+0x278>)
30005c6e:	781b      	ldrb	r3, [r3, #0]
30005c70:	2b00      	cmp	r3, #0
30005c72:	f040 8081 	bne.w	30005d78 <BOOT_Image1+0x1dc>
30005c76:	4c68      	ldr	r4, [pc, #416]	; (30005e18 <BOOT_Image1+0x27c>)
30005c78:	2102      	movs	r1, #2
30005c7a:	2201      	movs	r2, #1
30005c7c:	4867      	ldr	r0, [pc, #412]	; (30005e1c <BOOT_Image1+0x280>)
30005c7e:	47a0      	blx	r4
30005c80:	4c67      	ldr	r4, [pc, #412]	; (30005e20 <BOOT_Image1+0x284>)
30005c82:	68a3      	ldr	r3, [r4, #8]
30005c84:	0799      	lsls	r1, r3, #30
30005c86:	d471      	bmi.n	30005d6c <BOOT_Image1+0x1d0>
30005c88:	4966      	ldr	r1, [pc, #408]	; (30005e24 <BOOT_Image1+0x288>)
30005c8a:	e841 f100 	tt	r1, r1
30005c8e:	68a3      	ldr	r3, [r4, #8]
30005c90:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005c94:	079a      	lsls	r2, r3, #30
30005c96:	f100 8089 	bmi.w	30005dac <BOOT_Image1+0x210>
30005c9a:	4b63      	ldr	r3, [pc, #396]	; (30005e28 <BOOT_Image1+0x28c>)
30005c9c:	4f63      	ldr	r7, [pc, #396]	; (30005e2c <BOOT_Image1+0x290>)
30005c9e:	681b      	ldr	r3, [r3, #0]
30005ca0:	4798      	blx	r3
30005ca2:	f7fe fa53 	bl	3000414c <BOOT_RccConfig>
30005ca6:	f7fe fabd 	bl	30004224 <BOOT_CACHEWRR_Set>
30005caa:	4b61      	ldr	r3, [pc, #388]	; (30005e30 <BOOT_Image1+0x294>)
30005cac:	6818      	ldr	r0, [r3, #0]
30005cae:	f7fe fae7 	bl	30004280 <BOOT_TCMSet>
30005cb2:	4b60      	ldr	r3, [pc, #384]	; (30005e34 <BOOT_Image1+0x298>)
30005cb4:	695b      	ldr	r3, [r3, #20]
30005cb6:	03db      	lsls	r3, r3, #15
30005cb8:	d512      	bpl.n	30005ce0 <BOOT_Image1+0x144>
30005cba:	f017 011f 	ands.w	r1, r7, #31
30005cbe:	d13c      	bne.n	30005d3a <BOOT_Image1+0x19e>
30005cc0:	463b      	mov	r3, r7
30005cc2:	2174      	movs	r1, #116	; 0x74
30005cc4:	f3bf 8f4f 	dsb	sy
30005cc8:	485a      	ldr	r0, [pc, #360]	; (30005e34 <BOOT_Image1+0x298>)
30005cca:	4419      	add	r1, r3
30005ccc:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005cd0:	3320      	adds	r3, #32
30005cd2:	1aca      	subs	r2, r1, r3
30005cd4:	2a00      	cmp	r2, #0
30005cd6:	dcf9      	bgt.n	30005ccc <BOOT_Image1+0x130>
30005cd8:	f3bf 8f4f 	dsb	sy
30005cdc:	f3bf 8f6f 	isb	sy
30005ce0:	4b47      	ldr	r3, [pc, #284]	; (30005e00 <BOOT_Image1+0x264>)
30005ce2:	2200      	movs	r2, #0
30005ce4:	f8df 80f8 	ldr.w	r8, [pc, #248]	; 30005de0 <BOOT_Image1+0x244>
30005ce8:	f8c8 7270 	str.w	r7, [r8, #624]	; 0x270
30005cec:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005cf0:	f7fd fce6 	bl	300036c0 <flash_highspeed_setup>
30005cf4:	47a8      	blx	r5
30005cf6:	2802      	cmp	r0, #2
30005cf8:	f000 829a 	beq.w	30006230 <BOOT_Image1+0x694>
30005cfc:	7833      	ldrb	r3, [r6, #0]
30005cfe:	2b00      	cmp	r3, #0
30005d00:	f000 80b0 	beq.w	30005e64 <BOOT_Image1+0x2c8>
30005d04:	4c4c      	ldr	r4, [pc, #304]	; (30005e38 <BOOT_Image1+0x29c>)
30005d06:	4e4d      	ldr	r6, [pc, #308]	; (30005e3c <BOOT_Image1+0x2a0>)
30005d08:	4d4d      	ldr	r5, [pc, #308]	; (30005e40 <BOOT_Image1+0x2a4>)
30005d0a:	4633      	mov	r3, r6
30005d0c:	462a      	mov	r2, r5
30005d0e:	494b      	ldr	r1, [pc, #300]	; (30005e3c <BOOT_Image1+0x2a0>)
30005d10:	2002      	movs	r0, #2
30005d12:	f006 f91b 	bl	3000bf4c <rtk_log_write>
30005d16:	f241 3088 	movw	r0, #5000	; 0x1388
30005d1a:	47a0      	blx	r4
30005d1c:	e7f5      	b.n	30005d0a <BOOT_Image1+0x16e>
30005d1e:	4b35      	ldr	r3, [pc, #212]	; (30005df4 <BOOT_Image1+0x258>)
30005d20:	2001      	movs	r0, #1
30005d22:	4798      	blx	r3
30005d24:	4620      	mov	r0, r4
30005d26:	4b47      	ldr	r3, [pc, #284]	; (30005e44 <BOOT_Image1+0x2a8>)
30005d28:	4798      	blx	r3
30005d2a:	4b47      	ldr	r3, [pc, #284]	; (30005e48 <BOOT_Image1+0x2ac>)
30005d2c:	2001      	movs	r0, #1
30005d2e:	4798      	blx	r3
30005d30:	e762      	b.n	30005bf8 <BOOT_Image1+0x5c>
30005d32:	4846      	ldr	r0, [pc, #280]	; (30005e4c <BOOT_Image1+0x2b0>)
30005d34:	f006 fb80 	bl	3000c438 <__DiagPrintf_veneer>
30005d38:	e73b      	b.n	30005bb2 <BOOT_Image1+0x16>
30005d3a:	3174      	adds	r1, #116	; 0x74
30005d3c:	f027 031f 	bic.w	r3, r7, #31
30005d40:	e7c0      	b.n	30005cc4 <BOOT_Image1+0x128>
30005d42:	6de3      	ldr	r3, [r4, #92]	; 0x5c
30005d44:	2201      	movs	r2, #1
30005d46:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005d4a:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30005d4e:	65e3      	str	r3, [r4, #92]	; 0x5c
30005d50:	4b2b      	ldr	r3, [pc, #172]	; (30005e00 <BOOT_Image1+0x264>)
30005d52:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30005d56:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005d5a:	4b2b      	ldr	r3, [pc, #172]	; (30005e08 <BOOT_Image1+0x26c>)
30005d5c:	4798      	blx	r3
30005d5e:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30005d62:	f043 0302 	orr.w	r3, r3, #2
30005d66:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30005d6a:	e773      	b.n	30005c54 <BOOT_Image1+0xb8>
30005d6c:	f3ef 8108 	mrs	r1, MSP
30005d70:	4837      	ldr	r0, [pc, #220]	; (30005e50 <BOOT_Image1+0x2b4>)
30005d72:	f006 fb61 	bl	3000c438 <__DiagPrintf_veneer>
30005d76:	e787      	b.n	30005c88 <BOOT_Image1+0xec>
30005d78:	4f36      	ldr	r7, [pc, #216]	; (30005e54 <BOOT_Image1+0x2b8>)
30005d7a:	47b8      	blx	r7
30005d7c:	4604      	mov	r4, r0
30005d7e:	47b8      	blx	r7
30005d80:	2c02      	cmp	r4, #2
30005d82:	f43f af78 	beq.w	30005c76 <BOOT_Image1+0xda>
30005d86:	2803      	cmp	r0, #3
30005d88:	f43f af75 	beq.w	30005c76 <BOOT_Image1+0xda>
30005d8c:	4b32      	ldr	r3, [pc, #200]	; (30005e58 <BOOT_Image1+0x2bc>)
30005d8e:	4798      	blx	r3
30005d90:	4c21      	ldr	r4, [pc, #132]	; (30005e18 <BOOT_Image1+0x27c>)
30005d92:	2200      	movs	r2, #0
30005d94:	2101      	movs	r1, #1
30005d96:	4821      	ldr	r0, [pc, #132]	; (30005e1c <BOOT_Image1+0x280>)
30005d98:	47a0      	blx	r4
30005d9a:	2101      	movs	r1, #1
30005d9c:	481f      	ldr	r0, [pc, #124]	; (30005e1c <BOOT_Image1+0x280>)
30005d9e:	4b2f      	ldr	r3, [pc, #188]	; (30005e5c <BOOT_Image1+0x2c0>)
30005da0:	4798      	blx	r3
30005da2:	2201      	movs	r2, #1
30005da4:	481d      	ldr	r0, [pc, #116]	; (30005e1c <BOOT_Image1+0x280>)
30005da6:	4611      	mov	r1, r2
30005da8:	47a0      	blx	r4
30005daa:	e765      	b.n	30005c78 <BOOT_Image1+0xdc>
30005dac:	482c      	ldr	r0, [pc, #176]	; (30005e60 <BOOT_Image1+0x2c4>)
30005dae:	f006 fb43 	bl	3000c438 <__DiagPrintf_veneer>
30005db2:	e772      	b.n	30005c9a <BOOT_Image1+0xfe>
30005db4:	4601      	mov	r1, r0
30005db6:	f44f 7201 	mov.w	r2, #516	; 0x204
30005dba:	4811      	ldr	r0, [pc, #68]	; (30005e00 <BOOT_Image1+0x264>)
30005dbc:	f006 fb0c 	bl	3000c3d8 <____wrap_memset_veneer>
30005dc0:	e70d      	b.n	30005bde <BOOT_Image1+0x42>
30005dc2:	bf00      	nop
30005dc4:	0000c149 	.word	0x0000c149
30005dc8:	3000cd6c 	.word	0x3000cd6c
30005dcc:	3000e484 	.word	0x3000e484
30005dd0:	3000cd90 	.word	0x3000cd90
30005dd4:	3000fcf0 	.word	0x3000fcf0
30005dd8:	30010194 	.word	0x30010194
30005ddc:	00012be5 	.word	0x00012be5
30005de0:	42008000 	.word	0x42008000
30005de4:	42008200 	.word	0x42008200
30005de8:	3000e76e 	.word	0x3000e76e
30005dec:	3000b3a5 	.word	0x3000b3a5
30005df0:	3000b75d 	.word	0x3000b75d
30005df4:	3000b67d 	.word	0x3000b67d
30005df8:	30007d85 	.word	0x30007d85
30005dfc:	00009b2d 	.word	0x00009b2d
30005e00:	23020000 	.word	0x23020000
30005e04:	40080000 	.word	0x40080000
30005e08:	0000b479 	.word	0x0000b479
30005e0c:	40040000 	.word	0x40040000
30005e10:	30009c5d 	.word	0x30009c5d
30005e14:	3000e76d 	.word	0x3000e76d
30005e18:	0000aab9 	.word	0x0000aab9
30005e1c:	4200c000 	.word	0x4200c000
30005e20:	2001c00c 	.word	0x2001c00c
30005e24:	30005b9d 	.word	0x30005b9d
30005e28:	00033000 	.word	0x00033000
30005e2c:	2001c01c 	.word	0x2001c01c
30005e30:	3000e770 	.word	0x3000e770
30005e34:	e000ed00 	.word	0xe000ed00
30005e38:	00009be5 	.word	0x00009be5
30005e3c:	3000c710 	.word	0x3000c710
30005e40:	3000ce34 	.word	0x3000ce34
30005e44:	3000b8ad 	.word	0x3000b8ad
30005e48:	3000b81d 	.word	0x3000b81d
30005e4c:	3000cd7c 	.word	0x3000cd7c
30005e50:	3000cda8 	.word	0x3000cda8
30005e54:	0000c0f9 	.word	0x0000c0f9
30005e58:	0000aea1 	.word	0x0000aea1
30005e5c:	0000aaa5 	.word	0x0000aaa5
30005e60:	3000cdd8 	.word	0x3000cdd8
30005e64:	4b53      	ldr	r3, [pc, #332]	; (30005fb4 <BOOT_Image1+0x418>)
30005e66:	2004      	movs	r0, #4
30005e68:	4a53      	ldr	r2, [pc, #332]	; (30005fb8 <BOOT_Image1+0x41c>)
30005e6a:	4619      	mov	r1, r3
30005e6c:	f006 f86e 	bl	3000bf4c <rtk_log_write>
30005e70:	f7fe fb9e 	bl	300045b0 <BOOT_DDR_Init>
30005e74:	f8d8 3290 	ldr.w	r3, [r8, #656]	; 0x290
30005e78:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005e7c:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005e80:	f8c8 3290 	str.w	r3, [r8, #656]	; 0x290
30005e84:	2200      	movs	r2, #0
30005e86:	4b4d      	ldr	r3, [pc, #308]	; (30005fbc <BOOT_Image1+0x420>)
30005e88:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30005e8c:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30005e90:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30005e94:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30005e98:	f7ff fe42 	bl	30005b20 <BOOT_Share_Memory_Patch>
30005e9c:	4b48      	ldr	r3, [pc, #288]	; (30005fc0 <BOOT_Image1+0x424>)
30005e9e:	2002      	movs	r0, #2
30005ea0:	4798      	blx	r3
30005ea2:	4b44      	ldr	r3, [pc, #272]	; (30005fb4 <BOOT_Image1+0x418>)
30005ea4:	9000      	str	r0, [sp, #0]
30005ea6:	2004      	movs	r0, #4
30005ea8:	4619      	mov	r1, r3
30005eaa:	4a46      	ldr	r2, [pc, #280]	; (30005fc4 <BOOT_Image1+0x428>)
30005eac:	f006 f84e 	bl	3000bf4c <rtk_log_write>
30005eb0:	4b45      	ldr	r3, [pc, #276]	; (30005fc8 <BOOT_Image1+0x42c>)
30005eb2:	781b      	ldrb	r3, [r3, #0]
30005eb4:	2b00      	cmp	r3, #0
30005eb6:	f040 822e 	bne.w	30006316 <BOOT_Image1+0x77a>
30005eba:	6823      	ldr	r3, [r4, #0]
30005ebc:	f043 0302 	orr.w	r3, r3, #2
30005ec0:	6023      	str	r3, [r4, #0]
30005ec2:	4b42      	ldr	r3, [pc, #264]	; (30005fcc <BOOT_Image1+0x430>)
30005ec4:	4798      	blx	r3
30005ec6:	2801      	cmp	r0, #1
30005ec8:	f04f 0100 	mov.w	r1, #0
30005ecc:	f04f 0002 	mov.w	r0, #2
30005ed0:	bf0c      	ite	eq
30005ed2:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30005ed6:	4a3e      	ldrne	r2, [pc, #248]	; (30005fd0 <BOOT_Image1+0x434>)
30005ed8:	f001 fdfc 	bl	30007ad4 <ymodem_uart_port_init>
30005edc:	4b3b      	ldr	r3, [pc, #236]	; (30005fcc <BOOT_Image1+0x430>)
30005ede:	4798      	blx	r3
30005ee0:	2800      	cmp	r0, #0
30005ee2:	f000 81a0 	beq.w	30006226 <BOOT_Image1+0x68a>
30005ee6:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005eea:	483a      	ldr	r0, [pc, #232]	; (30005fd4 <BOOT_Image1+0x438>)
30005eec:	f006 faa4 	bl	3000c438 <__DiagPrintf_veneer>
30005ef0:	4839      	ldr	r0, [pc, #228]	; (30005fd8 <BOOT_Image1+0x43c>)
30005ef2:	2564      	movs	r5, #100	; 0x64
30005ef4:	f006 faf0 	bl	3000c4d8 <____wrap_strlen_veneer>
30005ef8:	4601      	mov	r1, r0
30005efa:	4837      	ldr	r0, [pc, #220]	; (30005fd8 <BOOT_Image1+0x43c>)
30005efc:	4c37      	ldr	r4, [pc, #220]	; (30005fdc <BOOT_Image1+0x440>)
30005efe:	f001 fe43 	bl	30007b88 <ymodem_uart_putdata>
30005f02:	e002      	b.n	30005f0a <BOOT_Image1+0x36e>
30005f04:	3d01      	subs	r5, #1
30005f06:	f000 825f 	beq.w	300063c8 <BOOT_Image1+0x82c>
30005f0a:	2001      	movs	r0, #1
30005f0c:	47a0      	blx	r4
30005f0e:	f001 fe21 	bl	30007b54 <ymodem_uart_readable>
30005f12:	2800      	cmp	r0, #0
30005f14:	d0f6      	beq.n	30005f04 <BOOT_Image1+0x368>
30005f16:	f001 fe1d 	bl	30007b54 <ymodem_uart_readable>
30005f1a:	2800      	cmp	r0, #0
30005f1c:	f000 825a 	beq.w	300063d4 <BOOT_Image1+0x838>
30005f20:	2301      	movs	r3, #1
30005f22:	9303      	str	r3, [sp, #12]
30005f24:	2001      	movs	r0, #1
30005f26:	47a0      	blx	r4
30005f28:	2108      	movs	r1, #8
30005f2a:	482d      	ldr	r0, [pc, #180]	; (30005fe0 <BOOT_Image1+0x444>)
30005f2c:	f001 fe3a 	bl	30007ba4 <ymodem_uart_getdata>
30005f30:	492b      	ldr	r1, [pc, #172]	; (30005fe0 <BOOT_Image1+0x444>)
30005f32:	2208      	movs	r2, #8
30005f34:	4b2b      	ldr	r3, [pc, #172]	; (30005fe4 <BOOT_Image1+0x448>)
30005f36:	f101 0080 	add.w	r0, r1, #128	; 0x80
30005f3a:	4798      	blx	r3
30005f3c:	4605      	mov	r5, r0
30005f3e:	2800      	cmp	r0, #0
30005f40:	f040 82a8 	bne.w	30006494 <BOOT_Image1+0x8f8>
30005f44:	f001 fe06 	bl	30007b54 <ymodem_uart_readable>
30005f48:	2800      	cmp	r0, #0
30005f4a:	d0fb      	beq.n	30005f44 <BOOT_Image1+0x3a8>
30005f4c:	2101      	movs	r1, #1
30005f4e:	4826      	ldr	r0, [pc, #152]	; (30005fe8 <BOOT_Image1+0x44c>)
30005f50:	f001 fe28 	bl	30007ba4 <ymodem_uart_getdata>
30005f54:	f001 fdfe 	bl	30007b54 <ymodem_uart_readable>
30005f58:	2800      	cmp	r0, #0
30005f5a:	d0fb      	beq.n	30005f54 <BOOT_Image1+0x3b8>
30005f5c:	4f23      	ldr	r7, [pc, #140]	; (30005fec <BOOT_Image1+0x450>)
30005f5e:	2101      	movs	r1, #1
30005f60:	f207 4051 	addw	r0, r7, #1105	; 0x451
30005f64:	f001 fe1e 	bl	30007ba4 <ymodem_uart_getdata>
30005f68:	f897 2451 	ldrb.w	r2, [r7, #1105]	; 0x451
30005f6c:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f70:	429a      	cmp	r2, r3
30005f72:	f000 82b4 	beq.w	300064de <BOOT_Image1+0x942>
30005f76:	f207 4952 	addw	r9, r7, #1106	; 0x452
30005f7a:	2600      	movs	r6, #0
30005f7c:	46b0      	mov	r8, r6
30005f7e:	3601      	adds	r6, #1
30005f80:	f001 fde8 	bl	30007b54 <ymodem_uart_readable>
30005f84:	2800      	cmp	r0, #0
30005f86:	d0fb      	beq.n	30005f80 <BOOT_Image1+0x3e4>
30005f88:	4648      	mov	r0, r9
30005f8a:	2101      	movs	r1, #1
30005f8c:	f001 fe0a 	bl	30007ba4 <ymodem_uart_getdata>
30005f90:	f899 2000 	ldrb.w	r2, [r9]
30005f94:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f98:	f108 080b 	add.w	r8, r8, #11
30005f9c:	f109 0901 	add.w	r9, r9, #1
30005fa0:	46b3      	mov	fp, r6
30005fa2:	429a      	cmp	r2, r3
30005fa4:	d1ea      	bne.n	30005f7c <BOOT_Image1+0x3e0>
30005fa6:	2e00      	cmp	r6, #0
30005fa8:	dd31      	ble.n	3000600e <BOOT_Image1+0x472>
30005faa:	f507 618a 	add.w	r1, r7, #1104	; 0x450
30005fae:	2300      	movs	r3, #0
30005fb0:	1988      	adds	r0, r1, r6
30005fb2:	e01f      	b.n	30005ff4 <BOOT_Image1+0x458>
30005fb4:	3000c710 	.word	0x3000c710
30005fb8:	3000ce74 	.word	0x3000ce74
30005fbc:	42008000 	.word	0x42008000
30005fc0:	00009979 	.word	0x00009979
30005fc4:	3000ce88 	.word	0x3000ce88
30005fc8:	3000e76c 	.word	0x3000e76c
30005fcc:	0000c169 	.word	0x0000c169
30005fd0:	0016e360 	.word	0x0016e360
30005fd4:	3000cea4 	.word	0x3000cea4
30005fd8:	3000ed5c 	.word	0x3000ed5c
30005fdc:	00009be5 	.word	0x00009be5
30005fe0:	3000ed7c 	.word	0x3000ed7c
30005fe4:	000129bd 	.word	0x000129bd
30005fe8:	3000ed84 	.word	0x3000ed84
30005fec:	3000e934 	.word	0x3000e934
30005ff0:	4288      	cmp	r0, r1
30005ff2:	d00d      	beq.n	30006010 <BOOT_Image1+0x474>
30005ff4:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30005ff8:	eb03 0383 	add.w	r3, r3, r3, lsl #2
30005ffc:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30006000:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30006004:	f1bc 0f09 	cmp.w	ip, #9
30006008:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
3000600c:	d9f0      	bls.n	30005ff0 <BOOT_Image1+0x454>
3000600e:	2300      	movs	r3, #0
30006010:	f8c7 34d4 	str.w	r3, [r7, #1236]	; 0x4d4
30006014:	f001 fd9e 	bl	30007b54 <ymodem_uart_readable>
30006018:	2800      	cmp	r0, #0
3000601a:	d0fb      	beq.n	30006014 <BOOT_Image1+0x478>
3000601c:	49c3      	ldr	r1, [pc, #780]	; (3000632c <BOOT_Image1+0x790>)
3000601e:	460b      	mov	r3, r1
30006020:	2101      	movs	r1, #1
30006022:	eb03 0008 	add.w	r0, r3, r8
30006026:	9302      	str	r3, [sp, #8]
30006028:	f001 fdbc 	bl	30007ba4 <ymodem_uart_getdata>
3000602c:	9902      	ldr	r1, [sp, #8]
3000602e:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30006032:	f811 2008 	ldrb.w	r2, [r1, r8]
30006036:	429a      	cmp	r2, r3
30006038:	f000 824d 	beq.w	300064d6 <BOOT_Image1+0x93a>
3000603c:	f10b 080b 	add.w	r8, fp, #11
30006040:	f04f 0900 	mov.w	r9, #0
30006044:	4488      	add	r8, r1
30006046:	f109 0901 	add.w	r9, r9, #1
3000604a:	f001 fd83 	bl	30007b54 <ymodem_uart_readable>
3000604e:	2800      	cmp	r0, #0
30006050:	d0fb      	beq.n	3000604a <BOOT_Image1+0x4ae>
30006052:	4640      	mov	r0, r8
30006054:	2101      	movs	r1, #1
30006056:	f001 fda5 	bl	30007ba4 <ymodem_uart_getdata>
3000605a:	f898 2000 	ldrb.w	r2, [r8]
3000605e:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30006062:	f108 0801 	add.w	r8, r8, #1
30006066:	eb09 0a0b 	add.w	sl, r9, fp
3000606a:	429a      	cmp	r2, r3
3000606c:	d1eb      	bne.n	30006046 <BOOT_Image1+0x4aa>
3000606e:	f001 fd71 	bl	30007b54 <ymodem_uart_readable>
30006072:	2800      	cmp	r0, #0
30006074:	d0fb      	beq.n	3000606e <BOOT_Image1+0x4d2>
30006076:	9b02      	ldr	r3, [sp, #8]
30006078:	f10a 080b 	add.w	r8, sl, #11
3000607c:	2101      	movs	r1, #1
3000607e:	eb03 0008 	add.w	r0, r3, r8
30006082:	f001 fd8f 	bl	30007ba4 <ymodem_uart_getdata>
30006086:	9902      	ldr	r1, [sp, #8]
30006088:	f897 24d1 	ldrb.w	r2, [r7, #1233]	; 0x4d1
3000608c:	f811 3008 	ldrb.w	r3, [r1, r8]
30006090:	429a      	cmp	r2, r3
30006092:	f000 821e 	beq.w	300064d2 <BOOT_Image1+0x936>
30006096:	f10a 080c 	add.w	r8, sl, #12
3000609a:	4488      	add	r8, r1
3000609c:	3501      	adds	r5, #1
3000609e:	f001 fd59 	bl	30007b54 <ymodem_uart_readable>
300060a2:	2800      	cmp	r0, #0
300060a4:	d0fb      	beq.n	3000609e <BOOT_Image1+0x502>
300060a6:	4640      	mov	r0, r8
300060a8:	2101      	movs	r1, #1
300060aa:	f001 fd7b 	bl	30007ba4 <ymodem_uart_getdata>
300060ae:	f898 2000 	ldrb.w	r2, [r8]
300060b2:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
300060b6:	f108 0801 	add.w	r8, r8, #1
300060ba:	eb05 0b0a 	add.w	fp, r5, sl
300060be:	429a      	cmp	r2, r3
300060c0:	d1ec      	bne.n	3000609c <BOOT_Image1+0x500>
300060c2:	9b02      	ldr	r3, [sp, #8]
300060c4:	f10b 080c 	add.w	r8, fp, #12
300060c8:	2101      	movs	r1, #1
300060ca:	eb03 0008 	add.w	r0, r3, r8
300060ce:	f001 fd69 	bl	30007ba4 <ymodem_uart_getdata>
300060d2:	9b02      	ldr	r3, [sp, #8]
300060d4:	f813 1008 	ldrb.w	r1, [r3, r8]
300060d8:	290a      	cmp	r1, #10
300060da:	f8c7 14d8 	str.w	r1, [r7, #1240]	; 0x4d8
300060de:	f200 81e0 	bhi.w	300064a2 <BOOT_Image1+0x906>
300060e2:	9b02      	ldr	r3, [sp, #8]
300060e4:	f10b 000d 	add.w	r0, fp, #13
300060e8:	2101      	movs	r1, #1
300060ea:	f10b 080e 	add.w	r8, fp, #14
300060ee:	4418      	add	r0, r3
300060f0:	f001 fd58 	bl	30007ba4 <ymodem_uart_getdata>
300060f4:	9b02      	ldr	r3, [sp, #8]
300060f6:	2102      	movs	r1, #2
300060f8:	4498      	add	r8, r3
300060fa:	4640      	mov	r0, r8
300060fc:	f001 fd52 	bl	30007ba4 <ymodem_uart_getdata>
30006100:	4641      	mov	r1, r8
30006102:	2202      	movs	r2, #2
30006104:	488a      	ldr	r0, [pc, #552]	; (30006330 <BOOT_Image1+0x794>)
30006106:	4b8b      	ldr	r3, [pc, #556]	; (30006334 <BOOT_Image1+0x798>)
30006108:	4798      	blx	r3
3000610a:	2800      	cmp	r0, #0
3000610c:	f040 81dd 	bne.w	300064ca <BOOT_Image1+0x92e>
30006110:	9b02      	ldr	r3, [sp, #8]
30006112:	f10b 0010 	add.w	r0, fp, #16
30006116:	2101      	movs	r1, #1
30006118:	f10b 0811 	add.w	r8, fp, #17
3000611c:	4418      	add	r0, r3
3000611e:	f001 fd41 	bl	30007ba4 <ymodem_uart_getdata>
30006122:	4b85      	ldr	r3, [pc, #532]	; (30006338 <BOOT_Image1+0x79c>)
30006124:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
30006128:	f103 0228 	add.w	r2, r3, #40	; 0x28
3000612c:	f103 0130 	add.w	r1, r3, #48	; 0x30
30006130:	f7fd fcc6 	bl	30003ac0 <hotfix_get_addr_size>
30006134:	9b02      	ldr	r3, [sp, #8]
30006136:	2102      	movs	r1, #2
30006138:	4498      	add	r8, r3
3000613a:	4640      	mov	r0, r8
3000613c:	f001 fd32 	bl	30007ba4 <ymodem_uart_getdata>
30006140:	4641      	mov	r1, r8
30006142:	2202      	movs	r2, #2
30006144:	487d      	ldr	r0, [pc, #500]	; (3000633c <BOOT_Image1+0x7a0>)
30006146:	4b7b      	ldr	r3, [pc, #492]	; (30006334 <BOOT_Image1+0x798>)
30006148:	4798      	blx	r3
3000614a:	2800      	cmp	r0, #0
3000614c:	f000 8091 	beq.w	30006272 <BOOT_Image1+0x6d6>
30006150:	9b03      	ldr	r3, [sp, #12]
30006152:	2b00      	cmp	r3, #0
30006154:	f000 80d8 	beq.w	30006308 <BOOT_Image1+0x76c>
30006158:	f005 ff34 	bl	3000bfc4 <BOOT_VerCheck>
3000615c:	f7fe fc1c 	bl	30004998 <BOOT_LoadImages>
30006160:	2800      	cmp	r0, #0
30006162:	d07f      	beq.n	30006264 <BOOT_Image1+0x6c8>
30006164:	f7ff f9ae 	bl	300054c4 <BOOT_Enable_KM0>
30006168:	2200      	movs	r2, #0
3000616a:	2101      	movs	r1, #1
3000616c:	4b74      	ldr	r3, [pc, #464]	; (30006340 <BOOT_Image1+0x7a4>)
3000616e:	4875      	ldr	r0, [pc, #468]	; (30006344 <BOOT_Image1+0x7a8>)
30006170:	4798      	blx	r3
30006172:	4b75      	ldr	r3, [pc, #468]	; (30006348 <BOOT_Image1+0x7ac>)
30006174:	2008      	movs	r0, #8
30006176:	4798      	blx	r3
30006178:	f001 fa22 	bl	300075c0 <BOOT_RAM_TZCfg>
3000617c:	4b73      	ldr	r3, [pc, #460]	; (3000634c <BOOT_Image1+0x7b0>)
3000617e:	781b      	ldrb	r3, [r3, #0]
30006180:	2b01      	cmp	r3, #1
30006182:	f000 819a 	beq.w	300064ba <BOOT_Image1+0x91e>
30006186:	4b72      	ldr	r3, [pc, #456]	; (30006350 <BOOT_Image1+0x7b4>)
30006188:	781b      	ldrb	r3, [r3, #0]
3000618a:	2b00      	cmp	r3, #0
3000618c:	f000 810c 	beq.w	300063a8 <BOOT_Image1+0x80c>
30006190:	f7ff fac8 	bl	30005724 <BOOT_Enable_AP>
30006194:	4a6f      	ldr	r2, [pc, #444]	; (30006354 <BOOT_Image1+0x7b8>)
30006196:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
3000619a:	f043 0306 	orr.w	r3, r3, #6
3000619e:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
300061a2:	4b6a      	ldr	r3, [pc, #424]	; (3000634c <BOOT_Image1+0x7b0>)
300061a4:	781b      	ldrb	r3, [r3, #0]
300061a6:	2b01      	cmp	r3, #1
300061a8:	f000 817f 	beq.w	300064aa <BOOT_Image1+0x90e>
300061ac:	4869      	ldr	r0, [pc, #420]	; (30006354 <BOOT_Image1+0x7b8>)
300061ae:	496a      	ldr	r1, [pc, #424]	; (30006358 <BOOT_Image1+0x7bc>)
300061b0:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
300061b4:	4a69      	ldr	r2, [pc, #420]	; (3000635c <BOOT_Image1+0x7c0>)
300061b6:	f043 0301 	orr.w	r3, r3, #1
300061ba:	4d69      	ldr	r5, [pc, #420]	; (30006360 <BOOT_Image1+0x7c4>)
300061bc:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
300061c0:	688c      	ldr	r4, [r1, #8]
300061c2:	680b      	ldr	r3, [r1, #0]
300061c4:	4967      	ldr	r1, [pc, #412]	; (30006364 <BOOT_Image1+0x7c8>)
300061c6:	6063      	str	r3, [r4, #4]
300061c8:	4b67      	ldr	r3, [pc, #412]	; (30006368 <BOOT_Image1+0x7cc>)
300061ca:	6094      	str	r4, [r2, #8]
300061cc:	61e9      	str	r1, [r5, #28]
300061ce:	6a59      	ldr	r1, [r3, #36]	; 0x24
300061d0:	4866      	ldr	r0, [pc, #408]	; (3000636c <BOOT_Image1+0x7d0>)
300061d2:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
300061d6:	6259      	str	r1, [r3, #36]	; 0x24
300061d8:	6a51      	ldr	r1, [r2, #36]	; 0x24
300061da:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
300061de:	6251      	str	r1, [r2, #36]	; 0x24
300061e0:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
300061e4:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300061e8:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
300061ec:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
300061f0:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
300061f4:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300061f8:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
300061fc:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30006200:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30006204:	689b      	ldr	r3, [r3, #8]
30006206:	6183      	str	r3, [r0, #24]
30006208:	f7fe fb4e 	bl	300048a8 <BOOT_NVICBackup_HP>
3000620c:	4b58      	ldr	r3, [pc, #352]	; (30006370 <BOOT_Image1+0x7d4>)
3000620e:	f383 8888 	msr	MSP_NS, r3
30006212:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
30006216:	f383 8809 	msr	PSP, r3
3000621a:	6860      	ldr	r0, [r4, #4]
3000621c:	b007      	add	sp, #28
3000621e:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006222:	f7fd bf43 	b.w	300040ac <BOOT_NsStart>
30006226:	4953      	ldr	r1, [pc, #332]	; (30006374 <BOOT_Image1+0x7d8>)
30006228:	4853      	ldr	r0, [pc, #332]	; (30006378 <BOOT_Image1+0x7dc>)
3000622a:	f006 f905 	bl	3000c438 <__DiagPrintf_veneer>
3000622e:	e65f      	b.n	30005ef0 <BOOT_Image1+0x354>
30006230:	4b52      	ldr	r3, [pc, #328]	; (3000637c <BOOT_Image1+0x7e0>)
30006232:	2004      	movs	r0, #4
30006234:	4a52      	ldr	r2, [pc, #328]	; (30006380 <BOOT_Image1+0x7e4>)
30006236:	4619      	mov	r1, r3
30006238:	f005 fe88 	bl	3000bf4c <rtk_log_write>
3000623c:	4b51      	ldr	r3, [pc, #324]	; (30006384 <BOOT_Image1+0x7e8>)
3000623e:	4798      	blx	r3
30006240:	f7fe f9f4 	bl	3000462c <BOOT_PSRAM_Init>
30006244:	7830      	ldrb	r0, [r6, #0]
30006246:	2800      	cmp	r0, #0
30006248:	f47f ae1c 	bne.w	30005e84 <BOOT_Image1+0x2e8>
3000624c:	4b4e      	ldr	r3, [pc, #312]	; (30006388 <BOOT_Image1+0x7ec>)
3000624e:	4798      	blx	r3
30006250:	2800      	cmp	r0, #0
30006252:	f43f ae17 	beq.w	30005e84 <BOOT_Image1+0x2e8>
30006256:	4b49      	ldr	r3, [pc, #292]	; (3000637c <BOOT_Image1+0x7e0>)
30006258:	2003      	movs	r0, #3
3000625a:	4a4c      	ldr	r2, [pc, #304]	; (3000638c <BOOT_Image1+0x7f0>)
3000625c:	4619      	mov	r1, r3
3000625e:	f005 fe75 	bl	3000bf4c <rtk_log_write>
30006262:	e60f      	b.n	30005e84 <BOOT_Image1+0x2e8>
30006264:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006268:	47a0      	blx	r4
3000626a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000626e:	47a0      	blx	r4
30006270:	e7f8      	b.n	30006264 <BOOT_Image1+0x6c8>
30006272:	9b02      	ldr	r3, [sp, #8]
30006274:	f10b 0013 	add.w	r0, fp, #19
30006278:	2101      	movs	r1, #1
3000627a:	f10b 0814 	add.w	r8, fp, #20
3000627e:	4418      	add	r0, r3
30006280:	f001 fc90 	bl	30007ba4 <ymodem_uart_getdata>
30006284:	4b42      	ldr	r3, [pc, #264]	; (30006390 <BOOT_Image1+0x7f4>)
30006286:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
3000628a:	f1a3 0218 	sub.w	r2, r3, #24
3000628e:	f1a3 0110 	sub.w	r1, r3, #16
30006292:	f7fd fc15 	bl	30003ac0 <hotfix_get_addr_size>
30006296:	9b02      	ldr	r3, [sp, #8]
30006298:	2102      	movs	r1, #2
3000629a:	4498      	add	r8, r3
3000629c:	4640      	mov	r0, r8
3000629e:	f001 fc81 	bl	30007ba4 <ymodem_uart_getdata>
300062a2:	4641      	mov	r1, r8
300062a4:	2202      	movs	r2, #2
300062a6:	483b      	ldr	r0, [pc, #236]	; (30006394 <BOOT_Image1+0x7f8>)
300062a8:	4b22      	ldr	r3, [pc, #136]	; (30006334 <BOOT_Image1+0x798>)
300062aa:	4798      	blx	r3
300062ac:	2800      	cmp	r0, #0
300062ae:	f47f af4f 	bne.w	30006150 <BOOT_Image1+0x5b4>
300062b2:	9b02      	ldr	r3, [sp, #8]
300062b4:	f10b 0016 	add.w	r0, fp, #22
300062b8:	2101      	movs	r1, #1
300062ba:	f10b 0817 	add.w	r8, fp, #23
300062be:	4418      	add	r0, r3
300062c0:	f001 fc70 	bl	30007ba4 <ymodem_uart_getdata>
300062c4:	4b34      	ldr	r3, [pc, #208]	; (30006398 <BOOT_Image1+0x7fc>)
300062c6:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
300062ca:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
300062ce:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
300062d2:	f7fd fbf5 	bl	30003ac0 <hotfix_get_addr_size>
300062d6:	9a02      	ldr	r2, [sp, #8]
300062d8:	2102      	movs	r1, #2
300062da:	4490      	add	r8, r2
300062dc:	4640      	mov	r0, r8
300062de:	f001 fc61 	bl	30007ba4 <ymodem_uart_getdata>
300062e2:	4641      	mov	r1, r8
300062e4:	2202      	movs	r2, #2
300062e6:	482d      	ldr	r0, [pc, #180]	; (3000639c <BOOT_Image1+0x800>)
300062e8:	4b12      	ldr	r3, [pc, #72]	; (30006334 <BOOT_Image1+0x798>)
300062ea:	4798      	blx	r3
300062ec:	2800      	cmp	r0, #0
300062ee:	f47f af2f 	bne.w	30006150 <BOOT_Image1+0x5b4>
300062f2:	9b02      	ldr	r3, [sp, #8]
300062f4:	f10b 0019 	add.w	r0, fp, #25
300062f8:	2101      	movs	r1, #1
300062fa:	4418      	add	r0, r3
300062fc:	f001 fc52 	bl	30007ba4 <ymodem_uart_getdata>
30006300:	9b03      	ldr	r3, [sp, #12]
30006302:	2b00      	cmp	r3, #0
30006304:	f040 80f1 	bne.w	300064ea <BOOT_Image1+0x94e>
30006308:	2002      	movs	r0, #2
3000630a:	f001 fbfb 	bl	30007b04 <ymodem_uart_port_deinit>
3000630e:	4824      	ldr	r0, [pc, #144]	; (300063a0 <BOOT_Image1+0x804>)
30006310:	f006 f892 	bl	3000c438 <__DiagPrintf_veneer>
30006314:	e720      	b.n	30006158 <BOOT_Image1+0x5bc>
30006316:	4b23      	ldr	r3, [pc, #140]	; (300063a4 <BOOT_Image1+0x808>)
30006318:	4798      	blx	r3
3000631a:	2800      	cmp	r0, #0
3000631c:	f47f adcd 	bne.w	30005eba <BOOT_Image1+0x31e>
30006320:	68a3      	ldr	r3, [r4, #8]
30006322:	f043 0302 	orr.w	r3, r3, #2
30006326:	60a3      	str	r3, [r4, #8]
30006328:	e5c7      	b.n	30005eba <BOOT_Image1+0x31e>
3000632a:	bf00      	nop
3000632c:	3000ed7c 	.word	0x3000ed7c
30006330:	3000ee10 	.word	0x3000ee10
30006334:	000129bd 	.word	0x000129bd
30006338:	3000ee14 	.word	0x3000ee14
3000633c:	3000ee50 	.word	0x3000ee50
30006340:	0000ac6d 	.word	0x0000ac6d
30006344:	4200c000 	.word	0x4200c000
30006348:	0000e1d9 	.word	0x0000e1d9
3000634c:	3000e76c 	.word	0x3000e76c
30006350:	3000e76f 	.word	0x3000e76f
30006354:	42008200 	.word	0x42008200
30006358:	60000020 	.word	0x60000020
3000635c:	e002ed00 	.word	0xe002ed00
30006360:	30000000 	.word	0x30000000
30006364:	30003a81 	.word	0x30003a81
30006368:	e000ed00 	.word	0xe000ed00
3000636c:	3000fd9c 	.word	0x3000fd9c
30006370:	2001bffc 	.word	0x2001bffc
30006374:	0016e360 	.word	0x0016e360
30006378:	3000cea4 	.word	0x3000cea4
3000637c:	3000c710 	.word	0x3000c710
30006380:	3000ce08 	.word	0x3000ce08
30006384:	30008c05 	.word	0x30008c05
30006388:	3000b77d 	.word	0x3000b77d
3000638c:	3000ce1c 	.word	0x3000ce1c
30006390:	3000ee54 	.word	0x3000ee54
30006394:	3000ee7c 	.word	0x3000ee7c
30006398:	3000ee80 	.word	0x3000ee80
3000639c:	3000eea8 	.word	0x3000eea8
300063a0:	3000ced4 	.word	0x3000ced4
300063a4:	0000c179 	.word	0x0000c179
300063a8:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300063ac:	685a      	ldr	r2, [r3, #4]
300063ae:	f042 0202 	orr.w	r2, r2, #2
300063b2:	605a      	str	r2, [r3, #4]
300063b4:	685a      	ldr	r2, [r3, #4]
300063b6:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300063ba:	605a      	str	r2, [r3, #4]
300063bc:	4a4d      	ldr	r2, [pc, #308]	; (300064f4 <BOOT_Image1+0x958>)
300063be:	6f93      	ldr	r3, [r2, #120]	; 0x78
300063c0:	f043 0303 	orr.w	r3, r3, #3
300063c4:	6793      	str	r3, [r2, #120]	; 0x78
300063c6:	e6ec      	b.n	300061a2 <BOOT_Image1+0x606>
300063c8:	f001 fbc4 	bl	30007b54 <ymodem_uart_readable>
300063cc:	2800      	cmp	r0, #0
300063ce:	d09b      	beq.n	30006308 <BOOT_Image1+0x76c>
300063d0:	9503      	str	r5, [sp, #12]
300063d2:	e5a7      	b.n	30005f24 <BOOT_Image1+0x388>
300063d4:	4f48      	ldr	r7, [pc, #288]	; (300064f8 <BOOT_Image1+0x95c>)
300063d6:	4605      	mov	r5, r0
300063d8:	f04f 080b 	mov.w	r8, #11
300063dc:	260a      	movs	r6, #10
300063de:	f507 6389 	add.w	r3, r7, #1096	; 0x448
300063e2:	4681      	mov	r9, r0
300063e4:	9302      	str	r3, [sp, #8]
300063e6:	2004      	movs	r0, #4
300063e8:	f001 fb8c 	bl	30007b04 <ymodem_uart_port_deinit>
300063ec:	9a02      	ldr	r2, [sp, #8]
300063ee:	9500      	str	r5, [sp, #0]
300063f0:	4613      	mov	r3, r2
300063f2:	1990      	adds	r0, r2, r6
300063f4:	f8d7 14d4 	ldr.w	r1, [r7, #1236]	; 0x4d4
300063f8:	464a      	mov	r2, r9
300063fa:	4443      	add	r3, r8
300063fc:	f7ff faea 	bl	300059d4 <password_hash_check>
30006400:	4680      	mov	r8, r0
30006402:	2800      	cmp	r0, #0
30006404:	d180      	bne.n	30006308 <BOOT_Image1+0x76c>
30006406:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000640a:	47a0      	blx	r4
3000640c:	483b      	ldr	r0, [pc, #236]	; (300064fc <BOOT_Image1+0x960>)
3000640e:	f006 f863 	bl	3000c4d8 <____wrap_strlen_veneer>
30006412:	4601      	mov	r1, r0
30006414:	4839      	ldr	r0, [pc, #228]	; (300064fc <BOOT_Image1+0x960>)
30006416:	f001 fbb7 	bl	30007b88 <ymodem_uart_putdata>
3000641a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000641e:	47a0      	blx	r4
30006420:	f8d7 24d8 	ldr.w	r2, [r7, #1240]	; 0x4d8
30006424:	4936      	ldr	r1, [pc, #216]	; (30006500 <BOOT_Image1+0x964>)
30006426:	2002      	movs	r0, #2
30006428:	f001 fc4e 	bl	30007cc8 <ymodem_img_download>
3000642c:	2032      	movs	r0, #50	; 0x32
3000642e:	47a0      	blx	r4
30006430:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30006434:	b1e3      	cbz	r3, 30006470 <BOOT_Image1+0x8d4>
30006436:	f8df 90f4 	ldr.w	r9, [pc, #244]	; 3000652c <BOOT_Image1+0x990>
3000643a:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
3000643e:	f859 6b04 	ldr.w	r6, [r9], #4
30006442:	0b36      	lsrs	r6, r6, #12
30006444:	d00c      	beq.n	30006460 <BOOT_Image1+0x8c4>
30006446:	2500      	movs	r5, #0
30006448:	f8da 1000 	ldr.w	r1, [sl]
3000644c:	2002      	movs	r0, #2
3000644e:	eb01 3105 	add.w	r1, r1, r5, lsl #12
30006452:	3501      	adds	r5, #1
30006454:	f7fd fc50 	bl	30003cf8 <FLASH_Erase_With_Lock>
30006458:	2032      	movs	r0, #50	; 0x32
3000645a:	47a0      	blx	r4
3000645c:	42ae      	cmp	r6, r5
3000645e:	d1f3      	bne.n	30006448 <BOOT_Image1+0x8ac>
30006460:	f108 0801 	add.w	r8, r8, #1
30006464:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30006468:	f10a 0a04 	add.w	sl, sl, #4
3000646c:	4543      	cmp	r3, r8
3000646e:	d8e6      	bhi.n	3000643e <BOOT_Image1+0x8a2>
30006470:	4824      	ldr	r0, [pc, #144]	; (30006504 <BOOT_Image1+0x968>)
30006472:	f005 ffe1 	bl	3000c438 <__DiagPrintf_veneer>
30006476:	4b24      	ldr	r3, [pc, #144]	; (30006508 <BOOT_Image1+0x96c>)
30006478:	a804      	add	r0, sp, #16
3000647a:	4798      	blx	r3
3000647c:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
30006480:	4822      	ldr	r0, [pc, #136]	; (3000650c <BOOT_Image1+0x970>)
30006482:	a904      	add	r1, sp, #16
30006484:	4b22      	ldr	r3, [pc, #136]	; (30006510 <BOOT_Image1+0x974>)
30006486:	f8ad 2012 	strh.w	r2, [sp, #18]
3000648a:	4798      	blx	r3
3000648c:	481f      	ldr	r0, [pc, #124]	; (3000650c <BOOT_Image1+0x970>)
3000648e:	4b21      	ldr	r3, [pc, #132]	; (30006514 <BOOT_Image1+0x978>)
30006490:	4798      	blx	r3
30006492:	e7fe      	b.n	30006492 <BOOT_Image1+0x8f6>
30006494:	4820      	ldr	r0, [pc, #128]	; (30006518 <BOOT_Image1+0x97c>)
30006496:	f005 ffcf 	bl	3000c438 <__DiagPrintf_veneer>
3000649a:	4820      	ldr	r0, [pc, #128]	; (3000651c <BOOT_Image1+0x980>)
3000649c:	f005 ffcc 	bl	3000c438 <__DiagPrintf_veneer>
300064a0:	e656      	b.n	30006150 <BOOT_Image1+0x5b4>
300064a2:	481f      	ldr	r0, [pc, #124]	; (30006520 <BOOT_Image1+0x984>)
300064a4:	f005 ffc8 	bl	3000c438 <__DiagPrintf_veneer>
300064a8:	e652      	b.n	30006150 <BOOT_Image1+0x5b4>
300064aa:	4b1e      	ldr	r3, [pc, #120]	; (30006524 <BOOT_Image1+0x988>)
300064ac:	4798      	blx	r3
300064ae:	2801      	cmp	r0, #1
300064b0:	bf0c      	ite	eq
300064b2:	20e6      	moveq	r0, #230	; 0xe6
300064b4:	2019      	movne	r0, #25
300064b6:	47a0      	blx	r4
300064b8:	e678      	b.n	300061ac <BOOT_Image1+0x610>
300064ba:	4b1a      	ldr	r3, [pc, #104]	; (30006524 <BOOT_Image1+0x988>)
300064bc:	4798      	blx	r3
300064be:	2801      	cmp	r0, #1
300064c0:	bf0c      	ite	eq
300064c2:	2019      	moveq	r0, #25
300064c4:	2004      	movne	r0, #4
300064c6:	47a0      	blx	r4
300064c8:	e65d      	b.n	30006186 <BOOT_Image1+0x5ea>
300064ca:	4817      	ldr	r0, [pc, #92]	; (30006528 <BOOT_Image1+0x98c>)
300064cc:	f005 ffb4 	bl	3000c438 <__DiagPrintf_veneer>
300064d0:	e63e      	b.n	30006150 <BOOT_Image1+0x5b4>
300064d2:	46d3      	mov	fp, sl
300064d4:	e5f5      	b.n	300060c2 <BOOT_Image1+0x526>
300064d6:	46da      	mov	sl, fp
300064d8:	f04f 0900 	mov.w	r9, #0
300064dc:	e5c7      	b.n	3000606e <BOOT_Image1+0x4d2>
300064de:	2600      	movs	r6, #0
300064e0:	f04f 080a 	mov.w	r8, #10
300064e4:	4633      	mov	r3, r6
300064e6:	46b3      	mov	fp, r6
300064e8:	e592      	b.n	30006010 <BOOT_Image1+0x474>
300064ea:	f106 080b 	add.w	r8, r6, #11
300064ee:	360a      	adds	r6, #10
300064f0:	44c8      	add	r8, r9
300064f2:	e778      	b.n	300063e6 <BOOT_Image1+0x84a>
300064f4:	41000200 	.word	0x41000200
300064f8:	3000e934 	.word	0x3000e934
300064fc:	3000f734 	.word	0x3000f734
30006500:	3000ee14 	.word	0x3000ee14
30006504:	3000cf0c 	.word	0x3000cf0c
30006508:	0000d3f9 	.word	0x0000d3f9
3000650c:	41000440 	.word	0x41000440
30006510:	0000d409 	.word	0x0000d409
30006514:	0000d489 	.word	0x0000d489
30006518:	3000cebc 	.word	0x3000cebc
3000651c:	3000ced4 	.word	0x3000ced4
30006520:	3000cee4 	.word	0x3000cee4
30006524:	0000c169 	.word	0x0000c169
30006528:	3000cef8 	.word	0x3000cef8
3000652c:	3000ee80 	.word	0x3000ee80

30006530 <BOOT_LoadSubImage>:
30006530:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006534:	b08f      	sub	sp, #60	; 0x3c
30006536:	469a      	mov	sl, r3
30006538:	460f      	mov	r7, r1
3000653a:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
3000653e:	9305      	str	r3, [sp, #20]
30006540:	b39a      	cbz	r2, 300065aa <BOOT_LoadSubImage+0x7a>
30006542:	4691      	mov	r9, r2
30006544:	2600      	movs	r6, #0
30006546:	4605      	mov	r5, r0
30006548:	2220      	movs	r2, #32
3000654a:	4639      	mov	r1, r7
3000654c:	a806      	add	r0, sp, #24
3000654e:	f7fe fa09 	bl	30004964 <BOOT_ImgCopy>
30006552:	9b06      	ldr	r3, [sp, #24]
30006554:	4a49      	ldr	r2, [pc, #292]	; (3000667c <BOOT_LoadSubImage+0x14c>)
30006556:	4293      	cmp	r3, r2
30006558:	d157      	bne.n	3000660a <BOOT_LoadSubImage+0xda>
3000655a:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
3000655e:	9b07      	ldr	r3, [sp, #28]
30006560:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
30006564:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
30006568:	4293      	cmp	r3, r2
3000656a:	d14e      	bne.n	3000660a <BOOT_LoadSubImage+0xda>
3000656c:	4b44      	ldr	r3, [pc, #272]	; (30006680 <BOOT_LoadSubImage+0x150>)
3000656e:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
30006572:	18c3      	adds	r3, r0, r3
30006574:	f1a0 0820 	sub.w	r8, r0, #32
30006578:	3420      	adds	r4, #32
3000657a:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000657e:	d21a      	bcs.n	300065b6 <BOOT_LoadSubImage+0x86>
30006580:	b17d      	cbz	r5, 300065a2 <BOOT_LoadSubImage+0x72>
30006582:	1d2b      	adds	r3, r5, #4
30006584:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
30006588:	4a3e      	ldr	r2, [pc, #248]	; (30006684 <BOOT_LoadSubImage+0x154>)
3000658a:	2004      	movs	r0, #4
3000658c:	e9cd 8401 	strd	r8, r4, [sp, #4]
30006590:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30006594:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30006598:	9300      	str	r3, [sp, #0]
3000659a:	4b3b      	ldr	r3, [pc, #236]	; (30006688 <BOOT_LoadSubImage+0x158>)
3000659c:	4619      	mov	r1, r3
3000659e:	f005 fcd5 	bl	3000bf4c <rtk_log_write>
300065a2:	3601      	adds	r6, #1
300065a4:	4427      	add	r7, r4
300065a6:	454e      	cmp	r6, r9
300065a8:	d1ce      	bne.n	30006548 <BOOT_LoadSubImage+0x18>
300065aa:	4b38      	ldr	r3, [pc, #224]	; (3000668c <BOOT_LoadSubImage+0x15c>)
300065ac:	2001      	movs	r0, #1
300065ae:	601f      	str	r7, [r3, #0]
300065b0:	b00f      	add	sp, #60	; 0x3c
300065b2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300065b6:	4639      	mov	r1, r7
300065b8:	4622      	mov	r2, r4
300065ba:	4640      	mov	r0, r8
300065bc:	f7fe f9d2 	bl	30004964 <BOOT_ImgCopy>
300065c0:	4933      	ldr	r1, [pc, #204]	; (30006690 <BOOT_LoadSubImage+0x160>)
300065c2:	694b      	ldr	r3, [r1, #20]
300065c4:	03db      	lsls	r3, r3, #15
300065c6:	d5db      	bpl.n	30006580 <BOOT_LoadSubImage+0x50>
300065c8:	ea08 0304 	and.w	r3, r8, r4
300065cc:	3301      	adds	r3, #1
300065ce:	d023      	beq.n	30006618 <BOOT_LoadSubImage+0xe8>
300065d0:	f018 031f 	ands.w	r3, r8, #31
300065d4:	d042      	beq.n	3000665c <BOOT_LoadSubImage+0x12c>
300065d6:	4423      	add	r3, r4
300065d8:	f028 021f 	bic.w	r2, r8, #31
300065dc:	f3bf 8f4f 	dsb	sy
300065e0:	2b00      	cmp	r3, #0
300065e2:	dd0d      	ble.n	30006600 <BOOT_LoadSubImage+0xd0>
300065e4:	1e59      	subs	r1, r3, #1
300065e6:	f102 0320 	add.w	r3, r2, #32
300065ea:	4829      	ldr	r0, [pc, #164]	; (30006690 <BOOT_LoadSubImage+0x160>)
300065ec:	f021 011f 	bic.w	r1, r1, #31
300065f0:	4419      	add	r1, r3
300065f2:	428b      	cmp	r3, r1
300065f4:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300065f8:	461a      	mov	r2, r3
300065fa:	f103 0320 	add.w	r3, r3, #32
300065fe:	d1f8      	bne.n	300065f2 <BOOT_LoadSubImage+0xc2>
30006600:	f3bf 8f4f 	dsb	sy
30006604:	f3bf 8f6f 	isb	sy
30006608:	e7ba      	b.n	30006580 <BOOT_LoadSubImage+0x50>
3000660a:	9b05      	ldr	r3, [sp, #20]
3000660c:	2b01      	cmp	r3, #1
3000660e:	d028      	beq.n	30006662 <BOOT_LoadSubImage+0x132>
30006610:	2000      	movs	r0, #0
30006612:	b00f      	add	sp, #60	; 0x3c
30006614:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006618:	2300      	movs	r3, #0
3000661a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000661e:	f3bf 8f4f 	dsb	sy
30006622:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
30006626:	f643 7be0 	movw	fp, #16352	; 0x3fe0
3000662a:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
3000662e:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30006632:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30006636:	ea0c 000b 	and.w	r0, ip, fp
3000663a:	4673      	mov	r3, lr
3000663c:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006640:	3b01      	subs	r3, #1
30006642:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006646:	d2f9      	bcs.n	3000663c <BOOT_LoadSubImage+0x10c>
30006648:	f1ac 0c20 	sub.w	ip, ip, #32
3000664c:	f11c 0f20 	cmn.w	ip, #32
30006650:	d1f1      	bne.n	30006636 <BOOT_LoadSubImage+0x106>
30006652:	f3bf 8f4f 	dsb	sy
30006656:	f3bf 8f6f 	isb	sy
3000665a:	e791      	b.n	30006580 <BOOT_LoadSubImage+0x50>
3000665c:	4623      	mov	r3, r4
3000665e:	4642      	mov	r2, r8
30006660:	e7bc      	b.n	300065dc <BOOT_LoadSubImage+0xac>
30006662:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
30006666:	2004      	movs	r0, #4
30006668:	4b07      	ldr	r3, [pc, #28]	; (30006688 <BOOT_LoadSubImage+0x158>)
3000666a:	4a0a      	ldr	r2, [pc, #40]	; (30006694 <BOOT_LoadSubImage+0x164>)
3000666c:	4619      	mov	r1, r3
3000666e:	9400      	str	r4, [sp, #0]
30006670:	f005 fc6c 	bl	3000bf4c <rtk_log_write>
30006674:	2000      	movs	r0, #0
30006676:	b00f      	add	sp, #60	; 0x3c
30006678:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000667c:	35393138 	.word	0x35393138
30006680:	f7ffffe0 	.word	0xf7ffffe0
30006684:	3000cfb4 	.word	0x3000cfb4
30006688:	3000c710 	.word	0x3000c710
3000668c:	3000eeac 	.word	0x3000eeac
30006690:	e000ed00 	.word	0xe000ed00
30006694:	3000cfa0 	.word	0x3000cfa0

30006698 <BOOT_LoadAPSubImage>:
30006698:	4bb5      	ldr	r3, [pc, #724]	; (30006970 <BOOT_LoadAPSubImage+0x2d8>)
3000669a:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000669e:	460c      	mov	r4, r1
300066a0:	4606      	mov	r6, r0
300066a2:	b08f      	sub	sp, #60	; 0x3c
300066a4:	4615      	mov	r5, r2
300066a6:	2220      	movs	r2, #32
300066a8:	e893 0003 	ldmia.w	r3, {r0, r1}
300066ac:	0c0b      	lsrs	r3, r1, #16
300066ae:	9004      	str	r0, [sp, #16]
300066b0:	f8ad 1014 	strh.w	r1, [sp, #20]
300066b4:	a806      	add	r0, sp, #24
300066b6:	4621      	mov	r1, r4
300066b8:	f88d 3016 	strb.w	r3, [sp, #22]
300066bc:	f7fe f952 	bl	30004964 <BOOT_ImgCopy>
300066c0:	4bac      	ldr	r3, [pc, #688]	; (30006974 <BOOT_LoadAPSubImage+0x2dc>)
300066c2:	9a06      	ldr	r2, [sp, #24]
300066c4:	429a      	cmp	r2, r3
300066c6:	d108      	bne.n	300066da <BOOT_LoadAPSubImage+0x42>
300066c8:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
300066cc:	9a07      	ldr	r2, [sp, #28]
300066ce:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
300066d2:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
300066d6:	429a      	cmp	r2, r3
300066d8:	d006      	beq.n	300066e8 <BOOT_LoadAPSubImage+0x50>
300066da:	2d01      	cmp	r5, #1
300066dc:	f000 80b3 	beq.w	30006846 <BOOT_LoadAPSubImage+0x1ae>
300066e0:	2000      	movs	r0, #0
300066e2:	b00f      	add	sp, #60	; 0x3c
300066e4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300066e8:	9a09      	ldr	r2, [sp, #36]	; 0x24
300066ea:	4ba3      	ldr	r3, [pc, #652]	; (30006978 <BOOT_LoadAPSubImage+0x2e0>)
300066ec:	6ca7      	ldr	r7, [r4, #72]	; 0x48
300066ee:	f1a2 0820 	sub.w	r8, r2, #32
300066f2:	4413      	add	r3, r2
300066f4:	37d0      	adds	r7, #208	; 0xd0
300066f6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300066fa:	d247      	bcs.n	3000678c <BOOT_LoadAPSubImage+0xf4>
300066fc:	4b9f      	ldr	r3, [pc, #636]	; (3000697c <BOOT_LoadAPSubImage+0x2e4>)
300066fe:	eb07 0508 	add.w	r5, r7, r8
30006702:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
30006706:	4798      	blx	r3
30006708:	2801      	cmp	r0, #1
3000670a:	4427      	add	r7, r4
3000670c:	4b9c      	ldr	r3, [pc, #624]	; (30006980 <BOOT_LoadAPSubImage+0x2e8>)
3000670e:	d029      	beq.n	30006764 <BOOT_LoadAPSubImage+0xcc>
30006710:	4619      	mov	r1, r3
30006712:	4a9c      	ldr	r2, [pc, #624]	; (30006984 <BOOT_LoadAPSubImage+0x2ec>)
30006714:	2004      	movs	r0, #4
30006716:	f005 fc19 	bl	3000bf4c <rtk_log_write>
3000671a:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
3000671e:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006722:	f080 809c 	bcs.w	3000685e <BOOT_LoadAPSubImage+0x1c6>
30006726:	4445      	add	r5, r8
30006728:	eb08 0107 	add.w	r1, r8, r7
3000672c:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
30006730:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30006734:	d256      	bcs.n	300067e4 <BOOT_LoadAPSubImage+0x14c>
30006736:	2e00      	cmp	r6, #0
30006738:	f000 8081 	beq.w	3000683e <BOOT_LoadAPSubImage+0x1a6>
3000673c:	4b90      	ldr	r3, [pc, #576]	; (30006980 <BOOT_LoadAPSubImage+0x2e8>)
3000673e:	aa04      	add	r2, sp, #16
30006740:	2004      	movs	r0, #4
30006742:	9200      	str	r2, [sp, #0]
30006744:	4a90      	ldr	r2, [pc, #576]	; (30006988 <BOOT_LoadAPSubImage+0x2f0>)
30006746:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
3000674a:	3c20      	subs	r4, #32
3000674c:	3120      	adds	r1, #32
3000674e:	e9c6 4100 	strd	r4, r1, [r6]
30006752:	9102      	str	r1, [sp, #8]
30006754:	4619      	mov	r1, r3
30006756:	9401      	str	r4, [sp, #4]
30006758:	f005 fbf8 	bl	3000bf4c <rtk_log_write>
3000675c:	2001      	movs	r0, #1
3000675e:	b00f      	add	sp, #60	; 0x3c
30006760:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006764:	4a89      	ldr	r2, [pc, #548]	; (3000698c <BOOT_LoadAPSubImage+0x2f4>)
30006766:	2004      	movs	r0, #4
30006768:	4619      	mov	r1, r3
3000676a:	f005 fbef 	bl	3000bf4c <rtk_log_write>
3000676e:	4642      	mov	r2, r8
30006770:	4629      	mov	r1, r5
30006772:	4638      	mov	r0, r7
30006774:	f000 fbe4 	bl	30006f40 <BOOT_LoadBL32RDPImg>
30006778:	2800      	cmp	r0, #0
3000677a:	d1d4      	bne.n	30006726 <BOOT_LoadAPSubImage+0x8e>
3000677c:	4b80      	ldr	r3, [pc, #512]	; (30006980 <BOOT_LoadAPSubImage+0x2e8>)
3000677e:	2004      	movs	r0, #4
30006780:	4a83      	ldr	r2, [pc, #524]	; (30006990 <BOOT_LoadAPSubImage+0x2f8>)
30006782:	4619      	mov	r1, r3
30006784:	f005 fbe2 	bl	3000bf4c <rtk_log_write>
30006788:	20ff      	movs	r0, #255	; 0xff
3000678a:	e7aa      	b.n	300066e2 <BOOT_LoadAPSubImage+0x4a>
3000678c:	463a      	mov	r2, r7
3000678e:	4621      	mov	r1, r4
30006790:	4640      	mov	r0, r8
30006792:	f7fe f8e7 	bl	30004964 <BOOT_ImgCopy>
30006796:	497f      	ldr	r1, [pc, #508]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
30006798:	694b      	ldr	r3, [r1, #20]
3000679a:	03da      	lsls	r2, r3, #15
3000679c:	d5ae      	bpl.n	300066fc <BOOT_LoadAPSubImage+0x64>
3000679e:	ea07 0308 	and.w	r3, r7, r8
300067a2:	3301      	adds	r3, #1
300067a4:	f000 8086 	beq.w	300068b4 <BOOT_LoadAPSubImage+0x21c>
300067a8:	f018 031f 	ands.w	r3, r8, #31
300067ac:	f000 80bb 	beq.w	30006926 <BOOT_LoadAPSubImage+0x28e>
300067b0:	443b      	add	r3, r7
300067b2:	f028 021f 	bic.w	r2, r8, #31
300067b6:	f3bf 8f4f 	dsb	sy
300067ba:	2b00      	cmp	r3, #0
300067bc:	dd0d      	ble.n	300067da <BOOT_LoadAPSubImage+0x142>
300067be:	1e59      	subs	r1, r3, #1
300067c0:	f102 0320 	add.w	r3, r2, #32
300067c4:	4873      	ldr	r0, [pc, #460]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
300067c6:	f021 011f 	bic.w	r1, r1, #31
300067ca:	4419      	add	r1, r3
300067cc:	428b      	cmp	r3, r1
300067ce:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300067d2:	461a      	mov	r2, r3
300067d4:	f103 0320 	add.w	r3, r3, #32
300067d8:	d1f8      	bne.n	300067cc <BOOT_LoadAPSubImage+0x134>
300067da:	f3bf 8f4f 	dsb	sy
300067de:	f3bf 8f6f 	isb	sy
300067e2:	e78b      	b.n	300066fc <BOOT_LoadAPSubImage+0x64>
300067e4:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
300067e8:	4628      	mov	r0, r5
300067ea:	4622      	mov	r2, r4
300067ec:	f7fe f8ba 	bl	30004964 <BOOT_ImgCopy>
300067f0:	4968      	ldr	r1, [pc, #416]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
300067f2:	694b      	ldr	r3, [r1, #20]
300067f4:	03d8      	lsls	r0, r3, #15
300067f6:	d59e      	bpl.n	30006736 <BOOT_LoadAPSubImage+0x9e>
300067f8:	ea05 0304 	and.w	r3, r5, r4
300067fc:	3301      	adds	r3, #1
300067fe:	d075      	beq.n	300068ec <BOOT_LoadAPSubImage+0x254>
30006800:	f015 031f 	ands.w	r3, r5, #31
30006804:	d002      	beq.n	3000680c <BOOT_LoadAPSubImage+0x174>
30006806:	f025 051f 	bic.w	r5, r5, #31
3000680a:	441c      	add	r4, r3
3000680c:	f3bf 8f4f 	dsb	sy
30006810:	2c00      	cmp	r4, #0
30006812:	dd0d      	ble.n	30006830 <BOOT_LoadAPSubImage+0x198>
30006814:	1e62      	subs	r2, r4, #1
30006816:	f105 0320 	add.w	r3, r5, #32
3000681a:	495e      	ldr	r1, [pc, #376]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
3000681c:	f022 021f 	bic.w	r2, r2, #31
30006820:	441a      	add	r2, r3
30006822:	4293      	cmp	r3, r2
30006824:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
30006828:	461d      	mov	r5, r3
3000682a:	f103 0320 	add.w	r3, r3, #32
3000682e:	d1f8      	bne.n	30006822 <BOOT_LoadAPSubImage+0x18a>
30006830:	f3bf 8f4f 	dsb	sy
30006834:	f3bf 8f6f 	isb	sy
30006838:	2e00      	cmp	r6, #0
3000683a:	f47f af7f 	bne.w	3000673c <BOOT_LoadAPSubImage+0xa4>
3000683e:	2001      	movs	r0, #1
30006840:	b00f      	add	sp, #60	; 0x3c
30006842:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006846:	4b4e      	ldr	r3, [pc, #312]	; (30006980 <BOOT_LoadAPSubImage+0x2e8>)
30006848:	ac04      	add	r4, sp, #16
3000684a:	4a53      	ldr	r2, [pc, #332]	; (30006998 <BOOT_LoadAPSubImage+0x300>)
3000684c:	2004      	movs	r0, #4
3000684e:	4619      	mov	r1, r3
30006850:	9400      	str	r4, [sp, #0]
30006852:	f005 fb7b 	bl	3000bf4c <rtk_log_write>
30006856:	2000      	movs	r0, #0
30006858:	b00f      	add	sp, #60	; 0x3c
3000685a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000685e:	4639      	mov	r1, r7
30006860:	4642      	mov	r2, r8
30006862:	4628      	mov	r0, r5
30006864:	f7fe f87e 	bl	30004964 <BOOT_ImgCopy>
30006868:	494a      	ldr	r1, [pc, #296]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
3000686a:	694b      	ldr	r3, [r1, #20]
3000686c:	03db      	lsls	r3, r3, #15
3000686e:	f57f af5a 	bpl.w	30006726 <BOOT_LoadAPSubImage+0x8e>
30006872:	ea08 0305 	and.w	r3, r8, r5
30006876:	3301      	adds	r3, #1
30006878:	d058      	beq.n	3000692c <BOOT_LoadAPSubImage+0x294>
3000687a:	f015 031f 	ands.w	r3, r5, #31
3000687e:	d073      	beq.n	30006968 <BOOT_LoadAPSubImage+0x2d0>
30006880:	4443      	add	r3, r8
30006882:	f025 021f 	bic.w	r2, r5, #31
30006886:	f3bf 8f4f 	dsb	sy
3000688a:	2b00      	cmp	r3, #0
3000688c:	dd0d      	ble.n	300068aa <BOOT_LoadAPSubImage+0x212>
3000688e:	1e59      	subs	r1, r3, #1
30006890:	f102 0320 	add.w	r3, r2, #32
30006894:	483f      	ldr	r0, [pc, #252]	; (30006994 <BOOT_LoadAPSubImage+0x2fc>)
30006896:	f021 011f 	bic.w	r1, r1, #31
3000689a:	4419      	add	r1, r3
3000689c:	428b      	cmp	r3, r1
3000689e:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300068a2:	461a      	mov	r2, r3
300068a4:	f103 0320 	add.w	r3, r3, #32
300068a8:	d1f8      	bne.n	3000689c <BOOT_LoadAPSubImage+0x204>
300068aa:	f3bf 8f4f 	dsb	sy
300068ae:	f3bf 8f6f 	isb	sy
300068b2:	e738      	b.n	30006726 <BOOT_LoadAPSubImage+0x8e>
300068b4:	2300      	movs	r3, #0
300068b6:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300068ba:	f3bf 8f4f 	dsb	sy
300068be:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
300068c2:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300068c6:	f3c0 324e 	ubfx	r2, r0, #13, #15
300068ca:	f3c0 00c9 	ubfx	r0, r0, #3, #10
300068ce:	0152      	lsls	r2, r2, #5
300068d0:	ea02 0c0e 	and.w	ip, r2, lr
300068d4:	4603      	mov	r3, r0
300068d6:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
300068da:	3b01      	subs	r3, #1
300068dc:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
300068e0:	d2f9      	bcs.n	300068d6 <BOOT_LoadAPSubImage+0x23e>
300068e2:	3a20      	subs	r2, #32
300068e4:	f112 0f20 	cmn.w	r2, #32
300068e8:	d1f2      	bne.n	300068d0 <BOOT_LoadAPSubImage+0x238>
300068ea:	e776      	b.n	300067da <BOOT_LoadAPSubImage+0x142>
300068ec:	2300      	movs	r3, #0
300068ee:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300068f2:	f3bf 8f4f 	dsb	sy
300068f6:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
300068fa:	f643 77e0 	movw	r7, #16352	; 0x3fe0
300068fe:	f3c5 344e 	ubfx	r4, r5, #13, #15
30006902:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30006906:	0164      	lsls	r4, r4, #5
30006908:	ea04 0007 	and.w	r0, r4, r7
3000690c:	462b      	mov	r3, r5
3000690e:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006912:	3b01      	subs	r3, #1
30006914:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006918:	1c5a      	adds	r2, r3, #1
3000691a:	d1f8      	bne.n	3000690e <BOOT_LoadAPSubImage+0x276>
3000691c:	3c20      	subs	r4, #32
3000691e:	f114 0f20 	cmn.w	r4, #32
30006922:	d1f1      	bne.n	30006908 <BOOT_LoadAPSubImage+0x270>
30006924:	e784      	b.n	30006830 <BOOT_LoadAPSubImage+0x198>
30006926:	463b      	mov	r3, r7
30006928:	4642      	mov	r2, r8
3000692a:	e744      	b.n	300067b6 <BOOT_LoadAPSubImage+0x11e>
3000692c:	2300      	movs	r3, #0
3000692e:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006932:	f3bf 8f4f 	dsb	sy
30006936:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
3000693a:	f643 79e0 	movw	r9, #16352	; 0x3fe0
3000693e:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
30006942:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30006946:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
3000694a:	ea0c 0009 	and.w	r0, ip, r9
3000694e:	4673      	mov	r3, lr
30006950:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006954:	3b01      	subs	r3, #1
30006956:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
3000695a:	d2f9      	bcs.n	30006950 <BOOT_LoadAPSubImage+0x2b8>
3000695c:	f1ac 0c20 	sub.w	ip, ip, #32
30006960:	f11c 0f20 	cmn.w	ip, #32
30006964:	d1f1      	bne.n	3000694a <BOOT_LoadAPSubImage+0x2b2>
30006966:	e7a0      	b.n	300068aa <BOOT_LoadAPSubImage+0x212>
30006968:	4643      	mov	r3, r8
3000696a:	462a      	mov	r2, r5
3000696c:	e78b      	b.n	30006886 <BOOT_LoadAPSubImage+0x1ee>
3000696e:	bf00      	nop
30006970:	3000cf98 	.word	0x3000cf98
30006974:	35393138 	.word	0x35393138
30006978:	f7ffffe0 	.word	0xf7ffffe0
3000697c:	0000c109 	.word	0x0000c109
30006980:	3000c710 	.word	0x3000c710
30006984:	3000cff8 	.word	0x3000cff8
30006988:	3000cfb4 	.word	0x3000cfb4
3000698c:	3000cfc8 	.word	0x3000cfc8
30006990:	3000cfd8 	.word	0x3000cfd8
30006994:	e000ed00 	.word	0xe000ed00
30006998:	3000cfa0 	.word	0x3000cfa0

3000699c <BOOT_RSIPIvSet>:
3000699c:	b530      	push	{r4, r5, lr}
3000699e:	2300      	movs	r3, #0
300069a0:	b083      	sub	sp, #12
300069a2:	4a09      	ldr	r2, [pc, #36]	; (300069c8 <BOOT_RSIPIvSet+0x2c>)
300069a4:	4605      	mov	r5, r0
300069a6:	460c      	mov	r4, r1
300069a8:	9300      	str	r3, [sp, #0]
300069aa:	9301      	str	r3, [sp, #4]
300069ac:	4790      	blx	r2
300069ae:	b148      	cbz	r0, 300069c4 <BOOT_RSIPIvSet+0x28>
300069b0:	f105 0120 	add.w	r1, r5, #32
300069b4:	4b05      	ldr	r3, [pc, #20]	; (300069cc <BOOT_RSIPIvSet+0x30>)
300069b6:	2208      	movs	r2, #8
300069b8:	4668      	mov	r0, sp
300069ba:	4798      	blx	r3
300069bc:	4669      	mov	r1, sp
300069be:	b2e0      	uxtb	r0, r4
300069c0:	4b03      	ldr	r3, [pc, #12]	; (300069d0 <BOOT_RSIPIvSet+0x34>)
300069c2:	4798      	blx	r3
300069c4:	b003      	add	sp, #12
300069c6:	bd30      	pop	{r4, r5, pc}
300069c8:	0000c129 	.word	0x0000c129
300069cc:	00012a1d 	.word	0x00012a1d
300069d0:	30009905 	.word	0x30009905

300069d4 <BOOT_OTFCheck>:
300069d4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300069d8:	4d1c      	ldr	r5, [pc, #112]	; (30006a4c <BOOT_OTFCheck+0x78>)
300069da:	b082      	sub	sp, #8
300069dc:	4604      	mov	r4, r0
300069de:	4688      	mov	r8, r1
300069e0:	4617      	mov	r7, r2
300069e2:	461e      	mov	r6, r3
300069e4:	47a8      	blx	r5
300069e6:	b148      	cbz	r0, 300069fc <BOOT_OTFCheck+0x28>
300069e8:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
300069ec:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300069f0:	d204      	bcs.n	300069fc <BOOT_OTFCheck+0x28>
300069f2:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
300069f6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300069fa:	d302      	bcc.n	30006a02 <BOOT_OTFCheck+0x2e>
300069fc:	b002      	add	sp, #8
300069fe:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30006a02:	4b13      	ldr	r3, [pc, #76]	; (30006a50 <BOOT_OTFCheck+0x7c>)
30006a04:	2004      	movs	r0, #4
30006a06:	4a13      	ldr	r2, [pc, #76]	; (30006a54 <BOOT_OTFCheck+0x80>)
30006a08:	4619      	mov	r1, r3
30006a0a:	f005 fa9f 	bl	3000bf4c <rtk_log_write>
30006a0e:	4b12      	ldr	r3, [pc, #72]	; (30006a58 <BOOT_OTFCheck+0x84>)
30006a10:	4798      	blx	r3
30006a12:	4603      	mov	r3, r0
30006a14:	b150      	cbz	r0, 30006a2c <BOOT_OTFCheck+0x58>
30006a16:	2801      	cmp	r0, #1
30006a18:	d009      	beq.n	30006a2e <BOOT_OTFCheck+0x5a>
30006a1a:	4b0d      	ldr	r3, [pc, #52]	; (30006a50 <BOOT_OTFCheck+0x7c>)
30006a1c:	2002      	movs	r0, #2
30006a1e:	4a0f      	ldr	r2, [pc, #60]	; (30006a5c <BOOT_OTFCheck+0x88>)
30006a20:	4619      	mov	r1, r3
30006a22:	b002      	add	sp, #8
30006a24:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006a28:	f005 ba90 	b.w	3000bf4c <rtk_log_write>
30006a2c:	2302      	movs	r3, #2
30006a2e:	4630      	mov	r0, r6
30006a30:	4621      	mov	r1, r4
30006a32:	9301      	str	r3, [sp, #4]
30006a34:	4642      	mov	r2, r8
30006a36:	2301      	movs	r3, #1
30006a38:	4c09      	ldr	r4, [pc, #36]	; (30006a60 <BOOT_OTFCheck+0x8c>)
30006a3a:	9700      	str	r7, [sp, #0]
30006a3c:	47a0      	blx	r4
30006a3e:	2001      	movs	r0, #1
30006a40:	4b08      	ldr	r3, [pc, #32]	; (30006a64 <BOOT_OTFCheck+0x90>)
30006a42:	b002      	add	sp, #8
30006a44:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30006a48:	4718      	bx	r3
30006a4a:	bf00      	nop
30006a4c:	0000c129 	.word	0x0000c129
30006a50:	3000c710 	.word	0x3000c710
30006a54:	3000d008 	.word	0x3000d008
30006a58:	0000c245 	.word	0x0000c245
30006a5c:	3000d01c 	.word	0x3000d01c
30006a60:	30009939 	.word	0x30009939
30006a64:	30009921 	.word	0x30009921

30006a68 <BOOT_OTA_LoadIMG2>:
30006a68:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006a6c:	4b6e      	ldr	r3, [pc, #440]	; (30006c28 <BOOT_OTA_LoadIMG2+0x1c0>)
30006a6e:	b08d      	sub	sp, #52	; 0x34
30006a70:	4a6e      	ldr	r2, [pc, #440]	; (30006c2c <BOOT_OTA_LoadIMG2+0x1c4>)
30006a72:	f103 040c 	add.w	r4, r3, #12
30006a76:	9003      	str	r0, [sp, #12]
30006a78:	f8d2 8000 	ldr.w	r8, [r2]
30006a7c:	ae06      	add	r6, sp, #24
30006a7e:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006a82:	ab09      	add	r3, sp, #36	; 0x24
30006a84:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30006a88:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30006a8c:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30006a90:	4641      	mov	r1, r8
30006a92:	4867      	ldr	r0, [pc, #412]	; (30006c30 <BOOT_OTA_LoadIMG2+0x1c8>)
30006a94:	f44f 72b2 	mov.w	r2, #356	; 0x164
30006a98:	f7fd ff64 	bl	30004964 <BOOT_ImgCopy>
30006a9c:	4641      	mov	r1, r8
30006a9e:	4864      	ldr	r0, [pc, #400]	; (30006c30 <BOOT_OTA_LoadIMG2+0x1c8>)
30006aa0:	f000 fd2a 	bl	300074f8 <BOOT_CertificateCheck>
30006aa4:	2800      	cmp	r0, #0
30006aa6:	f040 80b7 	bne.w	30006c18 <BOOT_OTA_LoadIMG2+0x1b0>
30006aaa:	4604      	mov	r4, r0
30006aac:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006ab0:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30006ab4:	485f      	ldr	r0, [pc, #380]	; (30006c34 <BOOT_OTA_LoadIMG2+0x1cc>)
30006ab6:	f7fd ff55 	bl	30004964 <BOOT_ImgCopy>
30006aba:	4b5f      	ldr	r3, [pc, #380]	; (30006c38 <BOOT_OTA_LoadIMG2+0x1d0>)
30006abc:	e9cd 4404 	strd	r4, r4, [sp, #16]
30006ac0:	4798      	blx	r3
30006ac2:	2800      	cmp	r0, #0
30006ac4:	f040 809e 	bne.w	30006c04 <BOOT_OTA_LoadIMG2+0x19c>
30006ac8:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30006acc:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30006c7c <BOOT_OTA_LoadIMG2+0x214>
30006ad0:	4a5a      	ldr	r2, [pc, #360]	; (30006c3c <BOOT_OTA_LoadIMG2+0x1d4>)
30006ad2:	2000      	movs	r0, #0
30006ad4:	4445      	add	r5, r8
30006ad6:	4659      	mov	r1, fp
30006ad8:	4f59      	ldr	r7, [pc, #356]	; (30006c40 <BOOT_OTA_LoadIMG2+0x1d8>)
30006ada:	f04f 0801 	mov.w	r8, #1
30006ade:	462b      	mov	r3, r5
30006ae0:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30006c80 <BOOT_OTA_LoadIMG2+0x218>
30006ae4:	47b8      	blx	r7
30006ae6:	2101      	movs	r1, #1
30006ae8:	2000      	movs	r0, #0
30006aea:	4b56      	ldr	r3, [pc, #344]	; (30006c44 <BOOT_OTA_LoadIMG2+0x1dc>)
30006aec:	4798      	blx	r3
30006aee:	2301      	movs	r3, #1
30006af0:	4952      	ldr	r1, [pc, #328]	; (30006c3c <BOOT_OTA_LoadIMG2+0x1d4>)
30006af2:	4658      	mov	r0, fp
30006af4:	461a      	mov	r2, r3
30006af6:	f7ff ff6d 	bl	300069d4 <BOOT_OTFCheck>
30006afa:	47c8      	blx	r9
30006afc:	2800      	cmp	r0, #0
30006afe:	bf0c      	ite	eq
30006b00:	46aa      	moveq	sl, r5
30006b02:	46da      	movne	sl, fp
30006b04:	4633      	mov	r3, r6
30006b06:	2203      	movs	r2, #3
30006b08:	4651      	mov	r1, sl
30006b0a:	484f      	ldr	r0, [pc, #316]	; (30006c48 <BOOT_OTA_LoadIMG2+0x1e0>)
30006b0c:	f8cd 8000 	str.w	r8, [sp]
30006b10:	f7ff fd0e 	bl	30006530 <BOOT_LoadSubImage>
30006b14:	2800      	cmp	r0, #0
30006b16:	d070      	beq.n	30006bfa <BOOT_OTA_LoadIMG2+0x192>
30006b18:	4e4b      	ldr	r6, [pc, #300]	; (30006c48 <BOOT_OTA_LoadIMG2+0x1e0>)
30006b1a:	47c8      	blx	r9
30006b1c:	2800      	cmp	r0, #0
30006b1e:	bf08      	it	eq
30006b20:	46ab      	moveq	fp, r5
30006b22:	4f47      	ldr	r7, [pc, #284]	; (30006c40 <BOOT_OTA_LoadIMG2+0x1d8>)
30006b24:	68f2      	ldr	r2, [r6, #12]
30006b26:	4640      	mov	r0, r8
30006b28:	6873      	ldr	r3, [r6, #4]
30006b2a:	f8c6 b000 	str.w	fp, [r6]
30006b2e:	4413      	add	r3, r2
30006b30:	6972      	ldr	r2, [r6, #20]
30006b32:	f8df b150 	ldr.w	fp, [pc, #336]	; 30006c84 <BOOT_OTA_LoadIMG2+0x21c>
30006b36:	441a      	add	r2, r3
30006b38:	4b44      	ldr	r3, [pc, #272]	; (30006c4c <BOOT_OTA_LoadIMG2+0x1e4>)
30006b3a:	eb0a 0102 	add.w	r1, sl, r2
30006b3e:	61b1      	str	r1, [r6, #24]
30006b40:	1e51      	subs	r1, r2, #1
30006b42:	400b      	ands	r3, r1
30006b44:	4659      	mov	r1, fp
30006b46:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006b4a:	441d      	add	r5, r3
30006b4c:	1a9b      	subs	r3, r3, r2
30006b4e:	4a40      	ldr	r2, [pc, #256]	; (30006c50 <BOOT_OTA_LoadIMG2+0x1e8>)
30006b50:	61f3      	str	r3, [r6, #28]
30006b52:	462b      	mov	r3, r5
30006b54:	47b8      	blx	r7
30006b56:	4641      	mov	r1, r8
30006b58:	4640      	mov	r0, r8
30006b5a:	4b3a      	ldr	r3, [pc, #232]	; (30006c44 <BOOT_OTA_LoadIMG2+0x1dc>)
30006b5c:	4798      	blx	r3
30006b5e:	4642      	mov	r2, r8
30006b60:	2302      	movs	r3, #2
30006b62:	493b      	ldr	r1, [pc, #236]	; (30006c50 <BOOT_OTA_LoadIMG2+0x1e8>)
30006b64:	4658      	mov	r0, fp
30006b66:	f7ff ff35 	bl	300069d4 <BOOT_OTFCheck>
30006b6a:	47c8      	blx	r9
30006b6c:	2800      	cmp	r0, #0
30006b6e:	bf0c      	ite	eq
30006b70:	462f      	moveq	r7, r5
30006b72:	465f      	movne	r7, fp
30006b74:	ab09      	add	r3, sp, #36	; 0x24
30006b76:	2203      	movs	r2, #3
30006b78:	f106 0020 	add.w	r0, r6, #32
30006b7c:	4639      	mov	r1, r7
30006b7e:	f8cd 8000 	str.w	r8, [sp]
30006b82:	f7ff fcd5 	bl	30006530 <BOOT_LoadSubImage>
30006b86:	2800      	cmp	r0, #0
30006b88:	d037      	beq.n	30006bfa <BOOT_OTA_LoadIMG2+0x192>
30006b8a:	47c8      	blx	r9
30006b8c:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30006b8e:	6a73      	ldr	r3, [r6, #36]	; 0x24
30006b90:	2800      	cmp	r0, #0
30006b92:	bf08      	it	eq
30006b94:	46ab      	moveq	fp, r5
30006b96:	4827      	ldr	r0, [pc, #156]	; (30006c34 <BOOT_OTA_LoadIMG2+0x1cc>)
30006b98:	4413      	add	r3, r2
30006b9a:	6b72      	ldr	r2, [r6, #52]	; 0x34
30006b9c:	f8c6 b020 	str.w	fp, [r6, #32]
30006ba0:	4413      	add	r3, r2
30006ba2:	4a2a      	ldr	r2, [pc, #168]	; (30006c4c <BOOT_OTA_LoadIMG2+0x1e4>)
30006ba4:	1e59      	subs	r1, r3, #1
30006ba6:	441f      	add	r7, r3
30006ba8:	400a      	ands	r2, r1
30006baa:	2100      	movs	r1, #0
30006bac:	63b7      	str	r7, [r6, #56]	; 0x38
30006bae:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006bb2:	9100      	str	r1, [sp, #0]
30006bb4:	1ad1      	subs	r1, r2, r3
30006bb6:	2900      	cmp	r1, #0
30006bb8:	63f1      	str	r1, [r6, #60]	; 0x3c
30006bba:	4631      	mov	r1, r6
30006bbc:	bf18      	it	ne
30006bbe:	4613      	movne	r3, r2
30006bc0:	4a24      	ldr	r2, [pc, #144]	; (30006c54 <BOOT_OTA_LoadIMG2+0x1ec>)
30006bc2:	441d      	add	r5, r3
30006bc4:	4b1a      	ldr	r3, [pc, #104]	; (30006c30 <BOOT_OTA_LoadIMG2+0x1c8>)
30006bc6:	6015      	str	r5, [r2, #0]
30006bc8:	bf0c      	ite	eq
30006bca:	2207      	moveq	r2, #7
30006bcc:	2208      	movne	r2, #8
30006bce:	f000 fb89 	bl	300072e4 <BOOT_SignatureCheck>
30006bd2:	4b21      	ldr	r3, [pc, #132]	; (30006c58 <BOOT_OTA_LoadIMG2+0x1f0>)
30006bd4:	4605      	mov	r5, r0
30006bd6:	689b      	ldr	r3, [r3, #8]
30006bd8:	079b      	lsls	r3, r3, #30
30006bda:	d50a      	bpl.n	30006bf2 <BOOT_OTA_LoadIMG2+0x18a>
30006bdc:	4b1f      	ldr	r3, [pc, #124]	; (30006c5c <BOOT_OTA_LoadIMG2+0x1f4>)
30006bde:	4a20      	ldr	r2, [pc, #128]	; (30006c60 <BOOT_OTA_LoadIMG2+0x1f8>)
30006be0:	9903      	ldr	r1, [sp, #12]
30006be2:	2800      	cmp	r0, #0
30006be4:	bf18      	it	ne
30006be6:	461a      	movne	r2, r3
30006be8:	481e      	ldr	r0, [pc, #120]	; (30006c64 <BOOT_OTA_LoadIMG2+0x1fc>)
30006bea:	3101      	adds	r1, #1
30006bec:	462b      	mov	r3, r5
30006bee:	f005 fc23 	bl	3000c438 <__DiagPrintf_veneer>
30006bf2:	b11d      	cbz	r5, 30006bfc <BOOT_OTA_LoadIMG2+0x194>
30006bf4:	481c      	ldr	r0, [pc, #112]	; (30006c68 <BOOT_OTA_LoadIMG2+0x200>)
30006bf6:	f005 fc1f 	bl	3000c438 <__DiagPrintf_veneer>
30006bfa:	24ff      	movs	r4, #255	; 0xff
30006bfc:	4620      	mov	r0, r4
30006bfe:	b00d      	add	sp, #52	; 0x34
30006c00:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c04:	2208      	movs	r2, #8
30006c06:	4919      	ldr	r1, [pc, #100]	; (30006c6c <BOOT_OTA_LoadIMG2+0x204>)
30006c08:	4b19      	ldr	r3, [pc, #100]	; (30006c70 <BOOT_OTA_LoadIMG2+0x208>)
30006c0a:	a804      	add	r0, sp, #16
30006c0c:	4798      	blx	r3
30006c0e:	a904      	add	r1, sp, #16
30006c10:	2001      	movs	r0, #1
30006c12:	4b18      	ldr	r3, [pc, #96]	; (30006c74 <BOOT_OTA_LoadIMG2+0x20c>)
30006c14:	4798      	blx	r3
30006c16:	e757      	b.n	30006ac8 <BOOT_OTA_LoadIMG2+0x60>
30006c18:	24ff      	movs	r4, #255	; 0xff
30006c1a:	4817      	ldr	r0, [pc, #92]	; (30006c78 <BOOT_OTA_LoadIMG2+0x210>)
30006c1c:	f005 fc0c 	bl	3000c438 <__DiagPrintf_veneer>
30006c20:	4620      	mov	r0, r4
30006c22:	b00d      	add	sp, #52	; 0x34
30006c24:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006c28:	3000e444 	.word	0x3000e444
30006c2c:	3000fd98 	.word	0x3000fd98
30006c30:	3000fe0c 	.word	0x3000fe0c
30006c34:	3000ff70 	.word	0x3000ff70
30006c38:	0000c129 	.word	0x0000c129
30006c3c:	0cffffe0 	.word	0x0cffffe0
30006c40:	300099b5 	.word	0x300099b5
30006c44:	300099f5 	.word	0x300099f5
30006c48:	30010118 	.word	0x30010118
30006c4c:	fffff000 	.word	0xfffff000
30006c50:	0dffffe0 	.word	0x0dffffe0
30006c54:	3000eeac 	.word	0x3000eeac
30006c58:	2001c00c 	.word	0x2001c00c
30006c5c:	3000d03c 	.word	0x3000d03c
30006c60:	3000d034 	.word	0x3000d034
30006c64:	3000d06c 	.word	0x3000d06c
30006c68:	3000d0a0 	.word	0x3000d0a0
30006c6c:	3000ff90 	.word	0x3000ff90
30006c70:	00012a1d 	.word	0x00012a1d
30006c74:	30009905 	.word	0x30009905
30006c78:	3000d044 	.word	0x3000d044
30006c7c:	0c000000 	.word	0x0c000000
30006c80:	30009c15 	.word	0x30009c15
30006c84:	0d000000 	.word	0x0d000000

30006c88 <BOOT_OTA_Region_Init>:
30006c88:	b510      	push	{r4, lr}
30006c8a:	4c10      	ldr	r4, [pc, #64]	; (30006ccc <BOOT_OTA_Region_Init+0x44>)
30006c8c:	2200      	movs	r2, #0
30006c8e:	2002      	movs	r0, #2
30006c90:	4621      	mov	r1, r4
30006c92:	f001 fd45 	bl	30008720 <flash_get_layout_info>
30006c96:	2200      	movs	r2, #0
30006c98:	1d21      	adds	r1, r4, #4
30006c9a:	2003      	movs	r0, #3
30006c9c:	f001 fd40 	bl	30008720 <flash_get_layout_info>
30006ca0:	2200      	movs	r2, #0
30006ca2:	f104 0110 	add.w	r1, r4, #16
30006ca6:	2004      	movs	r0, #4
30006ca8:	f001 fd3a 	bl	30008720 <flash_get_layout_info>
30006cac:	2200      	movs	r2, #0
30006cae:	f104 0114 	add.w	r1, r4, #20
30006cb2:	2005      	movs	r0, #5
30006cb4:	f001 fd34 	bl	30008720 <flash_get_layout_info>
30006cb8:	e9d4 2300 	ldrd	r2, r3, [r4]
30006cbc:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006cc0:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006cc4:	e9c4 2302 	strd	r2, r3, [r4, #8]
30006cc8:	bd10      	pop	{r4, pc}
30006cca:	bf00      	nop
30006ccc:	30010100 	.word	0x30010100

30006cd0 <BOOT_OTA_IMG2>:
30006cd0:	f7ff beca 	b.w	30006a68 <BOOT_OTA_LoadIMG2>

30006cd4 <BOOT_OTA_AP>:
30006cd4:	b5f0      	push	{r4, r5, r6, r7, lr}
30006cd6:	4a32      	ldr	r2, [pc, #200]	; (30006da0 <BOOT_OTA_AP+0xcc>)
30006cd8:	b0eb      	sub	sp, #428	; 0x1ac
30006cda:	4b32      	ldr	r3, [pc, #200]	; (30006da4 <BOOT_OTA_AP+0xd0>)
30006cdc:	6815      	ldr	r5, [r2, #0]
30006cde:	ac02      	add	r4, sp, #8
30006ce0:	4e31      	ldr	r6, [pc, #196]	; (30006da8 <BOOT_OTA_AP+0xd4>)
30006ce2:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30006ce6:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006ce8:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006cec:	4629      	mov	r1, r5
30006cee:	4d2f      	ldr	r5, [pc, #188]	; (30006dac <BOOT_OTA_AP+0xd8>)
30006cf0:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006cf4:	a806      	add	r0, sp, #24
30006cf6:	f7fd fe35 	bl	30004964 <BOOT_ImgCopy>
30006cfa:	4a2d      	ldr	r2, [pc, #180]	; (30006db0 <BOOT_OTA_AP+0xdc>)
30006cfc:	463b      	mov	r3, r7
30006cfe:	4629      	mov	r1, r5
30006d00:	2003      	movs	r0, #3
30006d02:	47b0      	blx	r6
30006d04:	2101      	movs	r1, #1
30006d06:	4b2b      	ldr	r3, [pc, #172]	; (30006db4 <BOOT_OTA_AP+0xe0>)
30006d08:	2003      	movs	r0, #3
30006d0a:	4798      	blx	r3
30006d0c:	2201      	movs	r2, #1
30006d0e:	4928      	ldr	r1, [pc, #160]	; (30006db0 <BOOT_OTA_AP+0xdc>)
30006d10:	2303      	movs	r3, #3
30006d12:	4628      	mov	r0, r5
30006d14:	f7ff fe5e 	bl	300069d4 <BOOT_OTFCheck>
30006d18:	4b27      	ldr	r3, [pc, #156]	; (30006db8 <BOOT_OTA_AP+0xe4>)
30006d1a:	4798      	blx	r3
30006d1c:	4623      	mov	r3, r4
30006d1e:	2201      	movs	r2, #1
30006d20:	4c26      	ldr	r4, [pc, #152]	; (30006dbc <BOOT_OTA_AP+0xe8>)
30006d22:	2800      	cmp	r0, #0
30006d24:	bf14      	ite	ne
30006d26:	4629      	movne	r1, r5
30006d28:	4639      	moveq	r1, r7
30006d2a:	9200      	str	r2, [sp, #0]
30006d2c:	4620      	mov	r0, r4
30006d2e:	2204      	movs	r2, #4
30006d30:	f7ff fbfe 	bl	30006530 <BOOT_LoadSubImage>
30006d34:	2801      	cmp	r0, #1
30006d36:	d122      	bne.n	30006d7e <BOOT_OTA_AP+0xaa>
30006d38:	2302      	movs	r3, #2
30006d3a:	4605      	mov	r5, r0
30006d3c:	2204      	movs	r2, #4
30006d3e:	a806      	add	r0, sp, #24
30006d40:	9300      	str	r3, [sp, #0]
30006d42:	4621      	mov	r1, r4
30006d44:	4b1e      	ldr	r3, [pc, #120]	; (30006dc0 <BOOT_OTA_AP+0xec>)
30006d46:	f000 facd 	bl	300072e4 <BOOT_SignatureCheck>
30006d4a:	4606      	mov	r6, r0
30006d4c:	bb18      	cbnz	r0, 30006d96 <BOOT_OTA_AP+0xc2>
30006d4e:	68e3      	ldr	r3, [r4, #12]
30006d50:	462a      	mov	r2, r5
30006d52:	6861      	ldr	r1, [r4, #4]
30006d54:	f104 0018 	add.w	r0, r4, #24
30006d58:	4419      	add	r1, r3
30006d5a:	6963      	ldr	r3, [r4, #20]
30006d5c:	4419      	add	r1, r3
30006d5e:	4439      	add	r1, r7
30006d60:	f7ff fc9a 	bl	30006698 <BOOT_LoadAPSubImage>
30006d64:	2801      	cmp	r0, #1
30006d66:	d102      	bne.n	30006d6e <BOOT_OTA_AP+0x9a>
30006d68:	4630      	mov	r0, r6
30006d6a:	b06b      	add	sp, #428	; 0x1ac
30006d6c:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d6e:	4b15      	ldr	r3, [pc, #84]	; (30006dc4 <BOOT_OTA_AP+0xf0>)
30006d70:	681b      	ldr	r3, [r3, #0]
30006d72:	079b      	lsls	r3, r3, #30
30006d74:	d40a      	bmi.n	30006d8c <BOOT_OTA_AP+0xb8>
30006d76:	26ff      	movs	r6, #255	; 0xff
30006d78:	4630      	mov	r0, r6
30006d7a:	b06b      	add	sp, #428	; 0x1ac
30006d7c:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d7e:	26ff      	movs	r6, #255	; 0xff
30006d80:	4811      	ldr	r0, [pc, #68]	; (30006dc8 <BOOT_OTA_AP+0xf4>)
30006d82:	f005 fb59 	bl	3000c438 <__DiagPrintf_veneer>
30006d86:	4630      	mov	r0, r6
30006d88:	b06b      	add	sp, #428	; 0x1ac
30006d8a:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d8c:	480f      	ldr	r0, [pc, #60]	; (30006dcc <BOOT_OTA_AP+0xf8>)
30006d8e:	26ff      	movs	r6, #255	; 0xff
30006d90:	f005 fb52 	bl	3000c438 <__DiagPrintf_veneer>
30006d94:	e7e8      	b.n	30006d68 <BOOT_OTA_AP+0x94>
30006d96:	480e      	ldr	r0, [pc, #56]	; (30006dd0 <BOOT_OTA_AP+0xfc>)
30006d98:	26ff      	movs	r6, #255	; 0xff
30006d9a:	f005 fb4d 	bl	3000c438 <__DiagPrintf_veneer>
30006d9e:	e7e3      	b.n	30006d68 <BOOT_OTA_AP+0x94>
30006da0:	3000eeac 	.word	0x3000eeac
30006da4:	3000e45c 	.word	0x3000e45c
30006da8:	300099b5 	.word	0x300099b5
30006dac:	0e000000 	.word	0x0e000000
30006db0:	0fffffe0 	.word	0x0fffffe0
30006db4:	300099f5 	.word	0x300099f5
30006db8:	30009c15 	.word	0x30009c15
30006dbc:	30010118 	.word	0x30010118
30006dc0:	3000fe0c 	.word	0x3000fe0c
30006dc4:	2001c00c 	.word	0x2001c00c
30006dc8:	3000d0e8 	.word	0x3000d0e8
30006dcc:	3000d10c 	.word	0x3000d10c
30006dd0:	3000d0c4 	.word	0x3000d0c4

30006dd4 <BOOT_LoadRDPImg>:
30006dd4:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006dd8:	4d4e      	ldr	r5, [pc, #312]	; (30006f14 <BOOT_LoadRDPImg+0x140>)
30006dda:	b0d9      	sub	sp, #356	; 0x164
30006ddc:	4613      	mov	r3, r2
30006dde:	468b      	mov	fp, r1
30006de0:	4604      	mov	r4, r0
30006de2:	aa06      	add	r2, sp, #24
30006de4:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30006de8:	e895 0003 	ldmia.w	r5, {r0, r1}
30006dec:	2500      	movs	r5, #0
30006dee:	e882 0003 	stmia.w	r2, {r0, r1}
30006df2:	4619      	mov	r1, r3
30006df4:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006df8:	4620      	mov	r0, r4
30006dfa:	f7fd fdb3 	bl	30004964 <BOOT_ImgCopy>
30006dfe:	ab06      	add	r3, sp, #24
30006e00:	2202      	movs	r2, #2
30006e02:	4641      	mov	r1, r8
30006e04:	4658      	mov	r0, fp
30006e06:	9500      	str	r5, [sp, #0]
30006e08:	f7ff fb92 	bl	30006530 <BOOT_LoadSubImage>
30006e0c:	2801      	cmp	r0, #1
30006e0e:	d06f      	beq.n	30006ef0 <BOOT_LoadRDPImg+0x11c>
30006e10:	4b41      	ldr	r3, [pc, #260]	; (30006f18 <BOOT_LoadRDPImg+0x144>)
30006e12:	4628      	mov	r0, r5
30006e14:	4798      	blx	r3
30006e16:	2101      	movs	r1, #1
30006e18:	4b40      	ldr	r3, [pc, #256]	; (30006f1c <BOOT_LoadRDPImg+0x148>)
30006e1a:	4628      	mov	r0, r5
30006e1c:	4798      	blx	r3
30006e1e:	4b40      	ldr	r3, [pc, #256]	; (30006f20 <BOOT_LoadRDPImg+0x14c>)
30006e20:	2120      	movs	r1, #32
30006e22:	a810      	add	r0, sp, #64	; 0x40
30006e24:	4798      	blx	r3
30006e26:	f8df a110 	ldr.w	sl, [pc, #272]	; 30006f38 <BOOT_LoadRDPImg+0x164>
30006e2a:	f104 0120 	add.w	r1, r4, #32
30006e2e:	2210      	movs	r2, #16
30006e30:	a80c      	add	r0, sp, #48	; 0x30
30006e32:	af08      	add	r7, sp, #32
30006e34:	47d0      	blx	sl
30006e36:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006f3c <BOOT_LoadRDPImg+0x168>
30006e3a:	ab06      	add	r3, sp, #24
30006e3c:	9504      	str	r5, [sp, #16]
30006e3e:	9305      	str	r3, [sp, #20]
30006e40:	4641      	mov	r1, r8
30006e42:	a818      	add	r0, sp, #96	; 0x60
30006e44:	2210      	movs	r2, #16
30006e46:	f7fd fd8d 	bl	30004964 <BOOT_ImgCopy>
30006e4a:	2310      	movs	r3, #16
30006e4c:	aa0c      	add	r2, sp, #48	; 0x30
30006e4e:	a818      	add	r0, sp, #96	; 0x60
30006e50:	4619      	mov	r1, r3
30006e52:	9700      	str	r7, [sp, #0]
30006e54:	47c8      	blx	r9
30006e56:	683b      	ldr	r3, [r7, #0]
30006e58:	4a32      	ldr	r2, [pc, #200]	; (30006f24 <BOOT_LoadRDPImg+0x150>)
30006e5a:	4293      	cmp	r3, r2
30006e5c:	d14c      	bne.n	30006ef8 <BOOT_LoadRDPImg+0x124>
30006e5e:	9a05      	ldr	r2, [sp, #20]
30006e60:	687b      	ldr	r3, [r7, #4]
30006e62:	f852 1b04 	ldr.w	r1, [r2], #4
30006e66:	9205      	str	r2, [sp, #20]
30006e68:	4a2f      	ldr	r2, [pc, #188]	; (30006f28 <BOOT_LoadRDPImg+0x154>)
30006e6a:	4293      	cmp	r3, r2
30006e6c:	d148      	bne.n	30006f00 <BOOT_LoadRDPImg+0x12c>
30006e6e:	9100      	str	r1, [sp, #0]
30006e70:	2004      	movs	r0, #4
30006e72:	68fe      	ldr	r6, [r7, #12]
30006e74:	4b2d      	ldr	r3, [pc, #180]	; (30006f2c <BOOT_LoadRDPImg+0x158>)
30006e76:	3e20      	subs	r6, #32
30006e78:	4a2d      	ldr	r2, [pc, #180]	; (30006f30 <BOOT_LoadRDPImg+0x15c>)
30006e7a:	4619      	mov	r1, r3
30006e7c:	9601      	str	r6, [sp, #4]
30006e7e:	68bd      	ldr	r5, [r7, #8]
30006e80:	3520      	adds	r5, #32
30006e82:	9502      	str	r5, [sp, #8]
30006e84:	f005 f862 	bl	3000bf4c <rtk_log_write>
30006e88:	f1bb 0f00 	cmp.w	fp, #0
30006e8c:	d006      	beq.n	30006e9c <BOOT_LoadRDPImg+0xc8>
30006e8e:	9a04      	ldr	r2, [sp, #16]
30006e90:	f10b 0304 	add.w	r3, fp, #4
30006e94:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30006e98:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30006e9c:	b1dd      	cbz	r5, 30006ed6 <BOOT_LoadRDPImg+0x102>
30006e9e:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006ea2:	462c      	mov	r4, r5
30006ea4:	4641      	mov	r1, r8
30006ea6:	a818      	add	r0, sp, #96	; 0x60
30006ea8:	bf28      	it	cs
30006eaa:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006eae:	4622      	mov	r2, r4
30006eb0:	44a0      	add	r8, r4
30006eb2:	f7fd fd57 	bl	30004964 <BOOT_ImgCopy>
30006eb6:	aa0c      	add	r2, sp, #48	; 0x30
30006eb8:	4621      	mov	r1, r4
30006eba:	2310      	movs	r3, #16
30006ebc:	9600      	str	r6, [sp, #0]
30006ebe:	a818      	add	r0, sp, #96	; 0x60
30006ec0:	47c8      	blx	r9
30006ec2:	ab18      	add	r3, sp, #96	; 0x60
30006ec4:	f1a4 0110 	sub.w	r1, r4, #16
30006ec8:	2210      	movs	r2, #16
30006eca:	a80c      	add	r0, sp, #48	; 0x30
30006ecc:	4426      	add	r6, r4
30006ece:	4419      	add	r1, r3
30006ed0:	47d0      	blx	sl
30006ed2:	1b2d      	subs	r5, r5, r4
30006ed4:	d1e3      	bne.n	30006e9e <BOOT_LoadRDPImg+0xca>
30006ed6:	9b04      	ldr	r3, [sp, #16]
30006ed8:	b13b      	cbz	r3, 30006eea <BOOT_LoadRDPImg+0x116>
30006eda:	2100      	movs	r1, #0
30006edc:	4b0f      	ldr	r3, [pc, #60]	; (30006f1c <BOOT_LoadRDPImg+0x148>)
30006ede:	4608      	mov	r0, r1
30006ee0:	4798      	blx	r3
30006ee2:	2002      	movs	r0, #2
30006ee4:	b059      	add	sp, #356	; 0x164
30006ee6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006eea:	2301      	movs	r3, #1
30006eec:	9304      	str	r3, [sp, #16]
30006eee:	e7a7      	b.n	30006e40 <BOOT_LoadRDPImg+0x6c>
30006ef0:	2002      	movs	r0, #2
30006ef2:	b059      	add	sp, #356	; 0x164
30006ef4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ef8:	9b04      	ldr	r3, [sp, #16]
30006efa:	aa06      	add	r2, sp, #24
30006efc:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006f00:	4b0a      	ldr	r3, [pc, #40]	; (30006f2c <BOOT_LoadRDPImg+0x158>)
30006f02:	2004      	movs	r0, #4
30006f04:	9100      	str	r1, [sp, #0]
30006f06:	4a0b      	ldr	r2, [pc, #44]	; (30006f34 <BOOT_LoadRDPImg+0x160>)
30006f08:	4619      	mov	r1, r3
30006f0a:	f005 f81f 	bl	3000bf4c <rtk_log_write>
30006f0e:	2000      	movs	r0, #0
30006f10:	e7e8      	b.n	30006ee4 <BOOT_LoadRDPImg+0x110>
30006f12:	bf00      	nop
30006f14:	3000e46c 	.word	0x3000e46c
30006f18:	00002429 	.word	0x00002429
30006f1c:	00002479 	.word	0x00002479
30006f20:	00004641 	.word	0x00004641
30006f24:	35393138 	.word	0x35393138
30006f28:	31313738 	.word	0x31313738
30006f2c:	3000c710 	.word	0x3000c710
30006f30:	3000cfb4 	.word	0x3000cfb4
30006f34:	3000cfa0 	.word	0x3000cfa0
30006f38:	00012a1d 	.word	0x00012a1d
30006f3c:	000046f5 	.word	0x000046f5

30006f40 <BOOT_LoadBL32RDPImg>:
30006f40:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006f44:	4b35      	ldr	r3, [pc, #212]	; (3000701c <BOOT_LoadBL32RDPImg+0xdc>)
30006f46:	b0d1      	sub	sp, #324	; 0x144
30006f48:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 30007038 <BOOT_LoadBL32RDPImg+0xf8>
30006f4c:	4607      	mov	r7, r0
30006f4e:	f10d 0810 	add.w	r8, sp, #16
30006f52:	460e      	mov	r6, r1
30006f54:	4615      	mov	r5, r2
30006f56:	ac02      	add	r4, sp, #8
30006f58:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 3000703c <BOOT_LoadBL32RDPImg+0xfc>
30006f5c:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30007040 <BOOT_LoadBL32RDPImg+0x100>
30006f60:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006f62:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
30006f66:	e89c 0003 	ldmia.w	ip, {r0, r1}
30006f6a:	e884 0003 	stmia.w	r4, {r0, r1}
30006f6e:	2000      	movs	r0, #0
30006f70:	47c8      	blx	r9
30006f72:	2101      	movs	r1, #1
30006f74:	2000      	movs	r0, #0
30006f76:	47d0      	blx	sl
30006f78:	2120      	movs	r1, #32
30006f7a:	4b29      	ldr	r3, [pc, #164]	; (30007020 <BOOT_LoadBL32RDPImg+0xe0>)
30006f7c:	eb0d 0001 	add.w	r0, sp, r1
30006f80:	4798      	blx	r3
30006f82:	4b28      	ldr	r3, [pc, #160]	; (30007024 <BOOT_LoadBL32RDPImg+0xe4>)
30006f84:	689b      	ldr	r3, [r3, #8]
30006f86:	079b      	lsls	r3, r3, #30
30006f88:	d436      	bmi.n	30006ff8 <BOOT_LoadBL32RDPImg+0xb8>
30006f8a:	b1fd      	cbz	r5, 30006fcc <BOOT_LoadBL32RDPImg+0x8c>
30006f8c:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30007044 <BOOT_LoadBL32RDPImg+0x104>
30006f90:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 30007048 <BOOT_LoadBL32RDPImg+0x108>
30006f94:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006f98:	462c      	mov	r4, r5
30006f9a:	4639      	mov	r1, r7
30006f9c:	a810      	add	r0, sp, #64	; 0x40
30006f9e:	bf28      	it	cs
30006fa0:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006fa4:	4622      	mov	r2, r4
30006fa6:	4427      	add	r7, r4
30006fa8:	f7fd fcdc 	bl	30004964 <BOOT_ImgCopy>
30006fac:	4642      	mov	r2, r8
30006fae:	4621      	mov	r1, r4
30006fb0:	2310      	movs	r3, #16
30006fb2:	9600      	str	r6, [sp, #0]
30006fb4:	a810      	add	r0, sp, #64	; 0x40
30006fb6:	47d8      	blx	fp
30006fb8:	ab10      	add	r3, sp, #64	; 0x40
30006fba:	f1a4 0110 	sub.w	r1, r4, #16
30006fbe:	2210      	movs	r2, #16
30006fc0:	4640      	mov	r0, r8
30006fc2:	4426      	add	r6, r4
30006fc4:	4419      	add	r1, r3
30006fc6:	47c8      	blx	r9
30006fc8:	1b2d      	subs	r5, r5, r4
30006fca:	d1e3      	bne.n	30006f94 <BOOT_LoadBL32RDPImg+0x54>
30006fcc:	f856 2c20 	ldr.w	r2, [r6, #-32]
30006fd0:	3e20      	subs	r6, #32
30006fd2:	4b15      	ldr	r3, [pc, #84]	; (30007028 <BOOT_LoadBL32RDPImg+0xe8>)
30006fd4:	429a      	cmp	r2, r3
30006fd6:	d108      	bne.n	30006fea <BOOT_LoadBL32RDPImg+0xaa>
30006fd8:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006fdc:	6872      	ldr	r2, [r6, #4]
30006fde:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30006fe2:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006fe6:	429a      	cmp	r2, r3
30006fe8:	d00d      	beq.n	30007006 <BOOT_LoadBL32RDPImg+0xc6>
30006fea:	4810      	ldr	r0, [pc, #64]	; (3000702c <BOOT_LoadBL32RDPImg+0xec>)
30006fec:	f005 fa24 	bl	3000c438 <__DiagPrintf_veneer>
30006ff0:	2000      	movs	r0, #0
30006ff2:	b051      	add	sp, #324	; 0x144
30006ff4:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ff8:	4621      	mov	r1, r4
30006ffa:	462b      	mov	r3, r5
30006ffc:	4632      	mov	r2, r6
30006ffe:	480c      	ldr	r0, [pc, #48]	; (30007030 <BOOT_LoadBL32RDPImg+0xf0>)
30007000:	f005 fa1a 	bl	3000c438 <__DiagPrintf_veneer>
30007004:	e7c1      	b.n	30006f8a <BOOT_LoadBL32RDPImg+0x4a>
30007006:	480b      	ldr	r0, [pc, #44]	; (30007034 <BOOT_LoadBL32RDPImg+0xf4>)
30007008:	f005 fa16 	bl	3000c438 <__DiagPrintf_veneer>
3000700c:	2100      	movs	r1, #0
3000700e:	4608      	mov	r0, r1
30007010:	47d0      	blx	sl
30007012:	2001      	movs	r0, #1
30007014:	b051      	add	sp, #324	; 0x144
30007016:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000701a:	bf00      	nop
3000701c:	3000d1b4 	.word	0x3000d1b4
30007020:	00004641 	.word	0x00004641
30007024:	2001c00c 	.word	0x2001c00c
30007028:	35393138 	.word	0x35393138
3000702c:	3000d198 	.word	0x3000d198
30007030:	3000d170 	.word	0x3000d170
30007034:	3000d1a4 	.word	0x3000d1a4
30007038:	3000d1c8 	.word	0x3000d1c8
3000703c:	00002429 	.word	0x00002429
30007040:	00002479 	.word	0x00002479
30007044:	000046f5 	.word	0x000046f5
30007048:	00012a1d 	.word	0x00012a1d

3000704c <_BOOT_Validate_ImgHash>:
3000704c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007050:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30007054:	4605      	mov	r5, r0
30007056:	4690      	mov	r8, r2
30007058:	22e0      	movs	r2, #224	; 0xe0
3000705a:	9103      	str	r1, [sp, #12]
3000705c:	2100      	movs	r1, #0
3000705e:	a818      	add	r0, sp, #96	; 0x60
30007060:	461c      	mov	r4, r3
30007062:	f88d 101f 	strb.w	r1, [sp, #31]
30007066:	f005 f9b7 	bl	3000c3d8 <____wrap_memset_veneer>
3000706a:	4628      	mov	r0, r5
3000706c:	f10d 021f 	add.w	r2, sp, #31
30007070:	f10d 011e 	add.w	r1, sp, #30
30007074:	f002 fe04 	bl	30009c80 <SBOOT_GetMdType>
30007078:	9001      	str	r0, [sp, #4]
3000707a:	2800      	cmp	r0, #0
3000707c:	d166      	bne.n	3000714c <_BOOT_Validate_ImgHash+0x100>
3000707e:	f89d 301f 	ldrb.w	r3, [sp, #31]
30007082:	2b00      	cmp	r3, #0
30007084:	d067      	beq.n	30007156 <_BOOT_Validate_ImgHash+0x10a>
30007086:	2101      	movs	r1, #1
30007088:	4605      	mov	r5, r0
3000708a:	4b80      	ldr	r3, [pc, #512]	; (3000728c <_BOOT_Validate_ImgHash+0x240>)
3000708c:	4608      	mov	r0, r1
3000708e:	4798      	blx	r3
30007090:	4629      	mov	r1, r5
30007092:	ab18      	add	r3, sp, #96	; 0x60
30007094:	2220      	movs	r2, #32
30007096:	f89d 001e 	ldrb.w	r0, [sp, #30]
3000709a:	4d7d      	ldr	r5, [pc, #500]	; (30007290 <_BOOT_Validate_ImgHash+0x244>)
3000709c:	47a8      	blx	r5
3000709e:	2800      	cmp	r0, #0
300070a0:	f040 808b 	bne.w	300071ba <_BOOT_Validate_ImgHash+0x16e>
300070a4:	b32c      	cbz	r4, 300070f2 <_BOOT_Validate_ImgHash+0xa6>
300070a6:	aad0      	add	r2, sp, #832	; 0x340
300070a8:	1e63      	subs	r3, r4, #1
300070aa:	ae50      	add	r6, sp, #320	; 0x140
300070ac:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 300072a8 <_BOOT_Validate_ImgHash+0x25c>
300070b0:	f012 0218 	ands.w	r2, r2, #24
300070b4:	b2db      	uxtb	r3, r3
300070b6:	f026 091f 	bic.w	r9, r6, #31
300070ba:	4d76      	ldr	r5, [pc, #472]	; (30007294 <_BOOT_Validate_ImgHash+0x248>)
300070bc:	9204      	str	r2, [sp, #16]
300070be:	f108 0208 	add.w	r2, r8, #8
300070c2:	bf08      	it	eq
300070c4:	46b1      	moveq	r9, r6
300070c6:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
300070ca:	9302      	str	r3, [sp, #8]
300070cc:	e9d8 b700 	ldrd	fp, r7, [r8]
300070d0:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
300070d4:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
300070d8:	f0c0 80a7 	bcc.w	3000722a <_BOOT_Validate_ImgHash+0x1de>
300070dc:	4639      	mov	r1, r7
300070de:	4658      	mov	r0, fp
300070e0:	aa18      	add	r2, sp, #96	; 0x60
300070e2:	47d0      	blx	sl
300070e4:	2800      	cmp	r0, #0
300070e6:	d168      	bne.n	300071ba <_BOOT_Validate_ImgHash+0x16e>
300070e8:	f108 0808 	add.w	r8, r8, #8
300070ec:	9b02      	ldr	r3, [sp, #8]
300070ee:	4543      	cmp	r3, r8
300070f0:	d1ec      	bne.n	300070cc <_BOOT_Validate_ImgHash+0x80>
300070f2:	ac08      	add	r4, sp, #32
300070f4:	a918      	add	r1, sp, #96	; 0x60
300070f6:	4b68      	ldr	r3, [pc, #416]	; (30007298 <_BOOT_Validate_ImgHash+0x24c>)
300070f8:	4620      	mov	r0, r4
300070fa:	4798      	blx	r3
300070fc:	4601      	mov	r1, r0
300070fe:	2800      	cmp	r0, #0
30007100:	d15b      	bne.n	300071ba <_BOOT_Validate_ImgHash+0x16e>
30007102:	2001      	movs	r0, #1
30007104:	4b61      	ldr	r3, [pc, #388]	; (3000728c <_BOOT_Validate_ImgHash+0x240>)
30007106:	4798      	blx	r3
30007108:	f89d 501e 	ldrb.w	r5, [sp, #30]
3000710c:	4b63      	ldr	r3, [pc, #396]	; (3000729c <_BOOT_Validate_ImgHash+0x250>)
3000710e:	4798      	blx	r3
30007110:	fbb0 f3f5 	udiv	r3, r0, r5
30007114:	fb03 0315 	mls	r3, r3, r5, r0
30007118:	b2db      	uxtb	r3, r3
3000711a:	b1bd      	cbz	r5, 3000714c <_BOOT_Validate_ImgHash+0x100>
3000711c:	2000      	movs	r0, #0
3000711e:	9f03      	ldr	r7, [sp, #12]
30007120:	4606      	mov	r6, r0
30007122:	1c5a      	adds	r2, r3, #1
30007124:	5cf9      	ldrb	r1, [r7, r3]
30007126:	5ce3      	ldrb	r3, [r4, r3]
30007128:	3001      	adds	r0, #1
3000712a:	b2d2      	uxtb	r2, r2
3000712c:	404b      	eors	r3, r1
3000712e:	431e      	orrs	r6, r3
30007130:	fbb2 f3f5 	udiv	r3, r2, r5
30007134:	fb05 2213 	mls	r2, r5, r3, r2
30007138:	b2d3      	uxtb	r3, r2
3000713a:	b2c2      	uxtb	r2, r0
3000713c:	4295      	cmp	r5, r2
3000713e:	d8f0      	bhi.n	30007122 <_BOOT_Validate_ImgHash+0xd6>
30007140:	2e00      	cmp	r6, #0
30007142:	9b01      	ldr	r3, [sp, #4]
30007144:	bf18      	it	ne
30007146:	f06f 0306 	mvnne.w	r3, #6
3000714a:	9301      	str	r3, [sp, #4]
3000714c:	9801      	ldr	r0, [sp, #4]
3000714e:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30007152:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007156:	a918      	add	r1, sp, #96	; 0x60
30007158:	f89d 001e 	ldrb.w	r0, [sp, #30]
3000715c:	4b50      	ldr	r3, [pc, #320]	; (300072a0 <_BOOT_Validate_ImgHash+0x254>)
3000715e:	4798      	blx	r3
30007160:	bb58      	cbnz	r0, 300071ba <_BOOT_Validate_ImgHash+0x16e>
30007162:	b31c      	cbz	r4, 300071ac <_BOOT_Validate_ImgHash+0x160>
30007164:	aad0      	add	r2, sp, #832	; 0x340
30007166:	1e63      	subs	r3, r4, #1
30007168:	ae50      	add	r6, sp, #320	; 0x140
3000716a:	f8df a140 	ldr.w	sl, [pc, #320]	; 300072ac <_BOOT_Validate_ImgHash+0x260>
3000716e:	f012 0218 	ands.w	r2, r2, #24
30007172:	b2db      	uxtb	r3, r3
30007174:	f026 091f 	bic.w	r9, r6, #31
30007178:	4d46      	ldr	r5, [pc, #280]	; (30007294 <_BOOT_Validate_ImgHash+0x248>)
3000717a:	9204      	str	r2, [sp, #16]
3000717c:	f108 0208 	add.w	r2, r8, #8
30007180:	bf08      	it	eq
30007182:	46b1      	moveq	r9, r6
30007184:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30007188:	9302      	str	r3, [sp, #8]
3000718a:	e9d8 b700 	ldrd	fp, r7, [r8]
3000718e:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30007192:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30007196:	d318      	bcc.n	300071ca <_BOOT_Validate_ImgHash+0x17e>
30007198:	4639      	mov	r1, r7
3000719a:	4658      	mov	r0, fp
3000719c:	aa18      	add	r2, sp, #96	; 0x60
3000719e:	47d0      	blx	sl
300071a0:	b958      	cbnz	r0, 300071ba <_BOOT_Validate_ImgHash+0x16e>
300071a2:	f108 0808 	add.w	r8, r8, #8
300071a6:	9b02      	ldr	r3, [sp, #8]
300071a8:	4543      	cmp	r3, r8
300071aa:	d1ee      	bne.n	3000718a <_BOOT_Validate_ImgHash+0x13e>
300071ac:	ac08      	add	r4, sp, #32
300071ae:	a918      	add	r1, sp, #96	; 0x60
300071b0:	4b3c      	ldr	r3, [pc, #240]	; (300072a4 <_BOOT_Validate_ImgHash+0x258>)
300071b2:	4620      	mov	r0, r4
300071b4:	4798      	blx	r3
300071b6:	2800      	cmp	r0, #0
300071b8:	d0a6      	beq.n	30007108 <_BOOT_Validate_ImgHash+0xbc>
300071ba:	f06f 030a 	mvn.w	r3, #10
300071be:	9301      	str	r3, [sp, #4]
300071c0:	9801      	ldr	r0, [sp, #4]
300071c2:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300071c6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300071ca:	2f00      	cmp	r7, #0
300071cc:	d0e9      	beq.n	300071a2 <_BOOT_Validate_ImgHash+0x156>
300071ce:	9c04      	ldr	r4, [sp, #16]
300071d0:	f8cd 8014 	str.w	r8, [sp, #20]
300071d4:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
300071d8:	46b8      	mov	r8, r7
300071da:	4659      	mov	r1, fp
300071dc:	4630      	mov	r0, r6
300071de:	bf28      	it	cs
300071e0:	f44f 7800 	movcs.w	r8, #512	; 0x200
300071e4:	4642      	mov	r2, r8
300071e6:	f7fd fbbd 	bl	30004964 <BOOT_ImgCopy>
300071ea:	696a      	ldr	r2, [r5, #20]
300071ec:	03d3      	lsls	r3, r2, #15
300071ee:	d50f      	bpl.n	30007210 <_BOOT_Validate_ImgHash+0x1c4>
300071f0:	eb08 0004 	add.w	r0, r8, r4
300071f4:	f3bf 8f4f 	dsb	sy
300071f8:	4448      	add	r0, r9
300071fa:	464a      	mov	r2, r9
300071fc:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30007200:	3220      	adds	r2, #32
30007202:	1a83      	subs	r3, r0, r2
30007204:	2b00      	cmp	r3, #0
30007206:	dcf9      	bgt.n	300071fc <_BOOT_Validate_ImgHash+0x1b0>
30007208:	f3bf 8f4f 	dsb	sy
3000720c:	f3bf 8f6f 	isb	sy
30007210:	aa18      	add	r2, sp, #96	; 0x60
30007212:	4641      	mov	r1, r8
30007214:	4630      	mov	r0, r6
30007216:	47d0      	blx	sl
30007218:	2800      	cmp	r0, #0
3000721a:	d1ce      	bne.n	300071ba <_BOOT_Validate_ImgHash+0x16e>
3000721c:	ebb7 0708 	subs.w	r7, r7, r8
30007220:	44c3      	add	fp, r8
30007222:	d1d7      	bne.n	300071d4 <_BOOT_Validate_ImgHash+0x188>
30007224:	f8dd 8014 	ldr.w	r8, [sp, #20]
30007228:	e7bb      	b.n	300071a2 <_BOOT_Validate_ImgHash+0x156>
3000722a:	2f00      	cmp	r7, #0
3000722c:	f43f af5c 	beq.w	300070e8 <_BOOT_Validate_ImgHash+0x9c>
30007230:	9c04      	ldr	r4, [sp, #16]
30007232:	f8cd 8014 	str.w	r8, [sp, #20]
30007236:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
3000723a:	46b8      	mov	r8, r7
3000723c:	4659      	mov	r1, fp
3000723e:	4630      	mov	r0, r6
30007240:	bf28      	it	cs
30007242:	f44f 7800 	movcs.w	r8, #512	; 0x200
30007246:	4642      	mov	r2, r8
30007248:	f7fd fb8c 	bl	30004964 <BOOT_ImgCopy>
3000724c:	696a      	ldr	r2, [r5, #20]
3000724e:	03d2      	lsls	r2, r2, #15
30007250:	d50f      	bpl.n	30007272 <_BOOT_Validate_ImgHash+0x226>
30007252:	eb08 0004 	add.w	r0, r8, r4
30007256:	f3bf 8f4f 	dsb	sy
3000725a:	4448      	add	r0, r9
3000725c:	464a      	mov	r2, r9
3000725e:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
30007262:	3220      	adds	r2, #32
30007264:	1a83      	subs	r3, r0, r2
30007266:	2b00      	cmp	r3, #0
30007268:	dcf9      	bgt.n	3000725e <_BOOT_Validate_ImgHash+0x212>
3000726a:	f3bf 8f4f 	dsb	sy
3000726e:	f3bf 8f6f 	isb	sy
30007272:	aa18      	add	r2, sp, #96	; 0x60
30007274:	4641      	mov	r1, r8
30007276:	4630      	mov	r0, r6
30007278:	47d0      	blx	sl
3000727a:	2800      	cmp	r0, #0
3000727c:	d19d      	bne.n	300071ba <_BOOT_Validate_ImgHash+0x16e>
3000727e:	ebb7 0708 	subs.w	r7, r7, r8
30007282:	44c3      	add	fp, r8
30007284:	d1d7      	bne.n	30007236 <_BOOT_Validate_ImgHash+0x1ea>
30007286:	f8dd 8014 	ldr.w	r8, [sp, #20]
3000728a:	e72d      	b.n	300070e8 <_BOOT_Validate_ImgHash+0x9c>
3000728c:	30009cbd 	.word	0x30009cbd
30007290:	3000957d 	.word	0x3000957d
30007294:	e000ed00 	.word	0xe000ed00
30007298:	30009d39 	.word	0x30009d39
3000729c:	0000ea4d 	.word	0x0000ea4d
300072a0:	30009d79 	.word	0x30009d79
300072a4:	30009df5 	.word	0x30009df5
300072a8:	30009cfd 	.word	0x30009cfd
300072ac:	30009db9 	.word	0x30009db9

300072b0 <BOOT_Validate_ImgHash>:
300072b0:	b530      	push	{r4, r5, lr}
300072b2:	b085      	sub	sp, #20
300072b4:	4d0a      	ldr	r5, [pc, #40]	; (300072e0 <BOOT_Validate_ImgHash+0x30>)
300072b6:	4604      	mov	r4, r0
300072b8:	9301      	str	r3, [sp, #4]
300072ba:	e9cd 2102 	strd	r2, r1, [sp, #8]
300072be:	47a8      	blx	r5
300072c0:	9903      	ldr	r1, [sp, #12]
300072c2:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
300072c6:	b128      	cbz	r0, 300072d4 <BOOT_Validate_ImgHash+0x24>
300072c8:	4620      	mov	r0, r4
300072ca:	b005      	add	sp, #20
300072cc:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300072d0:	f002 bdf4 	b.w	30009ebc <SBOOT_Validate_ImgHash>
300072d4:	4620      	mov	r0, r4
300072d6:	b005      	add	sp, #20
300072d8:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
300072dc:	f7ff beb6 	b.w	3000704c <_BOOT_Validate_ImgHash>
300072e0:	30009c15 	.word	0x30009c15

300072e4 <BOOT_SignatureCheck>:
300072e4:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300072e8:	461e      	mov	r6, r3
300072ea:	b089      	sub	sp, #36	; 0x24
300072ec:	4b79      	ldr	r3, [pc, #484]	; (300074d4 <BOOT_SignatureCheck+0x1f0>)
300072ee:	4680      	mov	r8, r0
300072f0:	4689      	mov	r9, r1
300072f2:	4617      	mov	r7, r2
300072f4:	f10d 0c20 	add.w	ip, sp, #32
300072f8:	4c77      	ldr	r4, [pc, #476]	; (300074d8 <BOOT_SignatureCheck+0x1f4>)
300072fa:	9d10      	ldr	r5, [sp, #64]	; 0x40
300072fc:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300072fe:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
30007302:	47a0      	blx	r4
30007304:	b910      	cbnz	r0, 3000730c <BOOT_SignatureCheck+0x28>
30007306:	b009      	add	sp, #36	; 0x24
30007308:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000730c:	4b73      	ldr	r3, [pc, #460]	; (300074dc <BOOT_SignatureCheck+0x1f8>)
3000730e:	2000      	movs	r0, #0
30007310:	4798      	blx	r3
30007312:	f898 3013 	ldrb.w	r3, [r8, #19]
30007316:	f898 2012 	ldrb.w	r2, [r8, #18]
3000731a:	f10d 010f 	add.w	r1, sp, #15
3000731e:	f10d 000e 	add.w	r0, sp, #14
30007322:	f002 fe05 	bl	30009f30 <SBOOT_Validate_Algorithm>
30007326:	4604      	mov	r4, r0
30007328:	b9e8      	cbnz	r0, 30007366 <BOOT_SignatureCheck+0x82>
3000732a:	69f1      	ldr	r1, [r6, #28]
3000732c:	b919      	cbnz	r1, 30007336 <BOOT_SignatureCheck+0x52>
3000732e:	e0cf      	b.n	300074d0 <BOOT_SignatureCheck+0x1ec>
30007330:	b2d0      	uxtb	r0, r2
30007332:	4288      	cmp	r0, r1
30007334:	d208      	bcs.n	30007348 <BOOT_SignatureCheck+0x64>
30007336:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
3000733a:	1c42      	adds	r2, r0, #1
3000733c:	eb06 0484 	add.w	r4, r6, r4, lsl #2
30007340:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
30007344:	42ab      	cmp	r3, r5
30007346:	d1f3      	bne.n	30007330 <BOOT_SignatureCheck+0x4c>
30007348:	2224      	movs	r2, #36	; 0x24
3000734a:	23b4      	movs	r3, #180	; 0xb4
3000734c:	fb12 3200 	smlabb	r2, r2, r0, r3
30007350:	f108 0570 	add.w	r5, r8, #112	; 0x70
30007354:	4432      	add	r2, r6
30007356:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000735a:	4629      	mov	r1, r5
3000735c:	f002 fd6a 	bl	30009e34 <SBOOT_Validate_PubKey>
30007360:	4604      	mov	r4, r0
30007362:	2800      	cmp	r0, #0
30007364:	d07a      	beq.n	3000745c <BOOT_SignatureCheck+0x178>
30007366:	f898 3011 	ldrb.w	r3, [r8, #17]
3000736a:	aa08      	add	r2, sp, #32
3000736c:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30007370:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007374:	4b5a      	ldr	r3, [pc, #360]	; (300074e0 <BOOT_SignatureCheck+0x1fc>)
30007376:	2002      	movs	r0, #2
30007378:	4a5a      	ldr	r2, [pc, #360]	; (300074e4 <BOOT_SignatureCheck+0x200>)
3000737a:	e9cd 1400 	strd	r1, r4, [sp]
3000737e:	4619      	mov	r1, r3
30007380:	f004 fde4 	bl	3000bf4c <rtk_log_write>
30007384:	2f00      	cmp	r7, #0
30007386:	d045      	beq.n	30007414 <BOOT_SignatureCheck+0x130>
30007388:	464d      	mov	r5, r9
3000738a:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
3000738e:	f04f 0800 	mov.w	r8, #0
30007392:	4f55      	ldr	r7, [pc, #340]	; (300074e8 <BOOT_SignatureCheck+0x204>)
30007394:	fa5f f389 	uxtb.w	r3, r9
30007398:	f105 0908 	add.w	r9, r5, #8
3000739c:	1d2e      	adds	r6, r5, #4
3000739e:	4c53      	ldr	r4, [pc, #332]	; (300074ec <BOOT_SignatureCheck+0x208>)
300073a0:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
300073a4:	e003      	b.n	300073ae <BOOT_SignatureCheck+0xca>
300073a6:	3508      	adds	r5, #8
300073a8:	3608      	adds	r6, #8
300073aa:	45a9      	cmp	r9, r5
300073ac:	d032      	beq.n	30007414 <BOOT_SignatureCheck+0x130>
300073ae:	f856 0c04 	ldr.w	r0, [r6, #-4]
300073b2:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
300073b6:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300073ba:	d3f4      	bcc.n	300073a6 <BOOT_SignatureCheck+0xc2>
300073bc:	2100      	movs	r1, #0
300073be:	686a      	ldr	r2, [r5, #4]
300073c0:	47b8      	blx	r7
300073c2:	6963      	ldr	r3, [r4, #20]
300073c4:	03d9      	lsls	r1, r3, #15
300073c6:	d5ee      	bpl.n	300073a6 <BOOT_SignatureCheck+0xc2>
300073c8:	f856 2c04 	ldr.w	r2, [r6, #-4]
300073cc:	686b      	ldr	r3, [r5, #4]
300073ce:	ea02 0103 	and.w	r1, r2, r3
300073d2:	3101      	adds	r1, #1
300073d4:	d022      	beq.n	3000741c <BOOT_SignatureCheck+0x138>
300073d6:	f012 011f 	ands.w	r1, r2, #31
300073da:	d002      	beq.n	300073e2 <BOOT_SignatureCheck+0xfe>
300073dc:	440b      	add	r3, r1
300073de:	f022 021f 	bic.w	r2, r2, #31
300073e2:	f3bf 8f4f 	dsb	sy
300073e6:	2b00      	cmp	r3, #0
300073e8:	dd0c      	ble.n	30007404 <BOOT_SignatureCheck+0x120>
300073ea:	1e59      	subs	r1, r3, #1
300073ec:	f102 0320 	add.w	r3, r2, #32
300073f0:	f021 011f 	bic.w	r1, r1, #31
300073f4:	4419      	add	r1, r3
300073f6:	428b      	cmp	r3, r1
300073f8:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
300073fc:	461a      	mov	r2, r3
300073fe:	f103 0320 	add.w	r3, r3, #32
30007402:	d1f8      	bne.n	300073f6 <BOOT_SignatureCheck+0x112>
30007404:	f3bf 8f4f 	dsb	sy
30007408:	f3bf 8f6f 	isb	sy
3000740c:	3508      	adds	r5, #8
3000740e:	3608      	adds	r6, #8
30007410:	45a9      	cmp	r9, r5
30007412:	d1cc      	bne.n	300073ae <BOOT_SignatureCheck+0xca>
30007414:	20ff      	movs	r0, #255	; 0xff
30007416:	b009      	add	sp, #36	; 0x24
30007418:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000741c:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
30007420:	f3bf 8f4f 	dsb	sy
30007424:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
30007428:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
3000742c:	f3c3 304e 	ubfx	r0, r3, #13, #15
30007430:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
30007434:	0140      	lsls	r0, r0, #5
30007436:	ea00 010e 	and.w	r1, r0, lr
3000743a:	4663      	mov	r3, ip
3000743c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30007440:	3b01      	subs	r3, #1
30007442:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
30007446:	1c5a      	adds	r2, r3, #1
30007448:	d1f8      	bne.n	3000743c <BOOT_SignatureCheck+0x158>
3000744a:	3820      	subs	r0, #32
3000744c:	f110 0f20 	cmn.w	r0, #32
30007450:	d1f1      	bne.n	30007436 <BOOT_SignatureCheck+0x152>
30007452:	f3bf 8f4f 	dsb	sy
30007456:	f3bf 8f6f 	isb	sy
3000745a:	e7d7      	b.n	3000740c <BOOT_SignatureCheck+0x128>
3000745c:	f508 7380 	add.w	r3, r8, #256	; 0x100
30007460:	462a      	mov	r2, r5
30007462:	f89d 100f 	ldrb.w	r1, [sp, #15]
30007466:	9301      	str	r3, [sp, #4]
30007468:	f44f 7380 	mov.w	r3, #256	; 0x100
3000746c:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007470:	9300      	str	r3, [sp, #0]
30007472:	4643      	mov	r3, r8
30007474:	f002 fcfc 	bl	30009e70 <SBOOT_Validate_Signature>
30007478:	4604      	mov	r4, r0
3000747a:	2800      	cmp	r0, #0
3000747c:	f47f af73 	bne.w	30007366 <BOOT_SignatureCheck+0x82>
30007480:	f108 0530 	add.w	r5, r8, #48	; 0x30
30007484:	4b1a      	ldr	r3, [pc, #104]	; (300074f0 <BOOT_SignatureCheck+0x20c>)
30007486:	f89d 400f 	ldrb.w	r4, [sp, #15]
3000748a:	4798      	blx	r3
3000748c:	4629      	mov	r1, r5
3000748e:	b9c0      	cbnz	r0, 300074c2 <BOOT_SignatureCheck+0x1de>
30007490:	4620      	mov	r0, r4
30007492:	463b      	mov	r3, r7
30007494:	464a      	mov	r2, r9
30007496:	f7ff fdd9 	bl	3000704c <_BOOT_Validate_ImgHash>
3000749a:	4604      	mov	r4, r0
3000749c:	f898 3011 	ldrb.w	r3, [r8, #17]
300074a0:	aa08      	add	r2, sp, #32
300074a2:	eb02 0383 	add.w	r3, r2, r3, lsl #2
300074a6:	f853 1c10 	ldr.w	r1, [r3, #-16]
300074aa:	2c00      	cmp	r4, #0
300074ac:	f47f af62 	bne.w	30007374 <BOOT_SignatureCheck+0x90>
300074b0:	4b0b      	ldr	r3, [pc, #44]	; (300074e0 <BOOT_SignatureCheck+0x1fc>)
300074b2:	2004      	movs	r0, #4
300074b4:	9100      	str	r1, [sp, #0]
300074b6:	4a0f      	ldr	r2, [pc, #60]	; (300074f4 <BOOT_SignatureCheck+0x210>)
300074b8:	4619      	mov	r1, r3
300074ba:	f004 fd47 	bl	3000bf4c <rtk_log_write>
300074be:	4620      	mov	r0, r4
300074c0:	e721      	b.n	30007306 <BOOT_SignatureCheck+0x22>
300074c2:	4620      	mov	r0, r4
300074c4:	463b      	mov	r3, r7
300074c6:	464a      	mov	r2, r9
300074c8:	f002 fcf8 	bl	30009ebc <SBOOT_Validate_ImgHash>
300074cc:	4604      	mov	r4, r0
300074ce:	e7e5      	b.n	3000749c <BOOT_SignatureCheck+0x1b8>
300074d0:	22b4      	movs	r2, #180	; 0xb4
300074d2:	e73d      	b.n	30007350 <BOOT_SignatureCheck+0x6c>
300074d4:	3000e474 	.word	0x3000e474
300074d8:	0000c149 	.word	0x0000c149
300074dc:	30009ef9 	.word	0x30009ef9
300074e0:	3000c710 	.word	0x3000c710
300074e4:	3000d1e8 	.word	0x3000d1e8
300074e8:	00012be5 	.word	0x00012be5
300074ec:	e000ed00 	.word	0xe000ed00
300074f0:	30009c15 	.word	0x30009c15
300074f4:	3000d1d0 	.word	0x3000d1d0

300074f8 <BOOT_CertificateCheck>:
300074f8:	b570      	push	{r4, r5, r6, lr}
300074fa:	4b2b      	ldr	r3, [pc, #172]	; (300075a8 <BOOT_CertificateCheck+0xb0>)
300074fc:	b0b0      	sub	sp, #192	; 0xc0
300074fe:	4604      	mov	r4, r0
30007500:	460d      	mov	r5, r1
30007502:	4798      	blx	r3
30007504:	b910      	cbnz	r0, 3000750c <BOOT_CertificateCheck+0x14>
30007506:	2000      	movs	r0, #0
30007508:	b030      	add	sp, #192	; 0xc0
3000750a:	bd70      	pop	{r4, r5, r6, pc}
3000750c:	69a1      	ldr	r1, [r4, #24]
3000750e:	2290      	movs	r2, #144	; 0x90
30007510:	a80c      	add	r0, sp, #48	; 0x30
30007512:	4429      	add	r1, r5
30007514:	f7fd fa26 	bl	30004964 <BOOT_ImgCopy>
30007518:	4b24      	ldr	r3, [pc, #144]	; (300075ac <BOOT_CertificateCheck+0xb4>)
3000751a:	4798      	blx	r3
3000751c:	4924      	ldr	r1, [pc, #144]	; (300075b0 <BOOT_CertificateCheck+0xb8>)
3000751e:	f000 031f 	and.w	r3, r0, #31
30007522:	2220      	movs	r2, #32
30007524:	6808      	ldr	r0, [r1, #0]
30007526:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
3000752a:	a930      	add	r1, sp, #192	; 0xc0
3000752c:	3a01      	subs	r2, #1
3000752e:	f813 c000 	ldrb.w	ip, [r3, r0]
30007532:	4419      	add	r1, r3
30007534:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
30007538:	f103 0301 	add.w	r3, r3, #1
3000753c:	f801 ccb0 	strb.w	ip, [r1, #-176]
30007540:	f003 031f 	and.w	r3, r3, #31
30007544:	d1f1      	bne.n	3000752a <BOOT_CertificateCheck+0x32>
30007546:	4610      	mov	r0, r2
30007548:	4b1a      	ldr	r3, [pc, #104]	; (300075b4 <BOOT_CertificateCheck+0xbc>)
3000754a:	4798      	blx	r3
3000754c:	7ce3      	ldrb	r3, [r4, #19]
3000754e:	7ca2      	ldrb	r2, [r4, #18]
30007550:	f10d 010f 	add.w	r1, sp, #15
30007554:	f10d 000e 	add.w	r0, sp, #14
30007558:	f002 fcea 	bl	30009f30 <SBOOT_Validate_Algorithm>
3000755c:	4605      	mov	r5, r0
3000755e:	b9c0      	cbnz	r0, 30007592 <BOOT_CertificateCheck+0x9a>
30007560:	f104 0620 	add.w	r6, r4, #32
30007564:	aa04      	add	r2, sp, #16
30007566:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000756a:	4631      	mov	r1, r6
3000756c:	f002 fc62 	bl	30009e34 <SBOOT_Validate_PubKey>
30007570:	4605      	mov	r5, r0
30007572:	b970      	cbnz	r0, 30007592 <BOOT_CertificateCheck+0x9a>
30007574:	69a0      	ldr	r0, [r4, #24]
30007576:	4623      	mov	r3, r4
30007578:	ac0c      	add	r4, sp, #48	; 0x30
3000757a:	4632      	mov	r2, r6
3000757c:	f89d 100f 	ldrb.w	r1, [sp, #15]
30007580:	e9cd 0400 	strd	r0, r4, [sp]
30007584:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007588:	f002 fc72 	bl	30009e70 <SBOOT_Validate_Signature>
3000758c:	4605      	mov	r5, r0
3000758e:	2800      	cmp	r0, #0
30007590:	d0b9      	beq.n	30007506 <BOOT_CertificateCheck+0xe>
30007592:	4b09      	ldr	r3, [pc, #36]	; (300075b8 <BOOT_CertificateCheck+0xc0>)
30007594:	2002      	movs	r0, #2
30007596:	4a09      	ldr	r2, [pc, #36]	; (300075bc <BOOT_CertificateCheck+0xc4>)
30007598:	4619      	mov	r1, r3
3000759a:	9500      	str	r5, [sp, #0]
3000759c:	f004 fcd6 	bl	3000bf4c <rtk_log_write>
300075a0:	20ff      	movs	r0, #255	; 0xff
300075a2:	b030      	add	sp, #192	; 0xc0
300075a4:	bd70      	pop	{r4, r5, r6, pc}
300075a6:	bf00      	nop
300075a8:	0000c149 	.word	0x0000c149
300075ac:	0000ea4d 	.word	0x0000ea4d
300075b0:	3000e774 	.word	0x3000e774
300075b4:	30009ef9 	.word	0x30009ef9
300075b8:	3000c710 	.word	0x3000c710
300075bc:	3000d20c 	.word	0x3000d20c

300075c0 <BOOT_RAM_TZCfg>:
300075c0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300075c4:	b085      	sub	sp, #20
300075c6:	4e58      	ldr	r6, [pc, #352]	; (30007728 <BOOT_RAM_TZCfg+0x168>)
300075c8:	4958      	ldr	r1, [pc, #352]	; (3000772c <BOOT_RAM_TZCfg+0x16c>)
300075ca:	2501      	movs	r5, #1
300075cc:	f10d 0e08 	add.w	lr, sp, #8
300075d0:	4a57      	ldr	r2, [pc, #348]	; (30007730 <BOOT_RAM_TZCfg+0x170>)
300075d2:	4b58      	ldr	r3, [pc, #352]	; (30007734 <BOOT_RAM_TZCfg+0x174>)
300075d4:	46ec      	mov	ip, sp
300075d6:	4677      	mov	r7, lr
300075d8:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30007754 <BOOT_RAM_TZCfg+0x194>
300075dc:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
300075e0:	f8df 9174 	ldr.w	r9, [pc, #372]	; 30007758 <BOOT_RAM_TZCfg+0x198>
300075e4:	9202      	str	r2, [sp, #8]
300075e6:	9303      	str	r3, [sp, #12]
300075e8:	e9cd 6100 	strd	r6, r1, [sp]
300075ec:	f85c 2b04 	ldr.w	r2, [ip], #4
300075f0:	2300      	movs	r3, #0
300075f2:	f85e 1b04 	ldr.w	r1, [lr], #4
300075f6:	680c      	ldr	r4, [r1, #0]
300075f8:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
300075fc:	fa05 f003 	lsl.w	r0, r5, r3
30007600:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
30007604:	d00c      	beq.n	30007620 <BOOT_RAM_TZCfg+0x60>
30007606:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
3000760a:	3301      	adds	r3, #1
3000760c:	684c      	ldr	r4, [r1, #4]
3000760e:	310c      	adds	r1, #12
30007610:	2b08      	cmp	r3, #8
30007612:	f8cb 4004 	str.w	r4, [fp, #4]
30007616:	6c14      	ldr	r4, [r2, #64]	; 0x40
30007618:	ea40 0004 	orr.w	r0, r0, r4
3000761c:	6410      	str	r0, [r2, #64]	; 0x40
3000761e:	d1ea      	bne.n	300075f6 <BOOT_RAM_TZCfg+0x36>
30007620:	42b2      	cmp	r2, r6
30007622:	d072      	beq.n	3000770a <BOOT_RAM_TZCfg+0x14a>
30007624:	6455      	str	r5, [r2, #68]	; 0x44
30007626:	45bc      	cmp	ip, r7
30007628:	d1e0      	bne.n	300075ec <BOOT_RAM_TZCfg+0x2c>
3000762a:	4b43      	ldr	r3, [pc, #268]	; (30007738 <BOOT_RAM_TZCfg+0x178>)
3000762c:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
30007630:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30007634:	4841      	ldr	r0, [pc, #260]	; (3000773c <BOOT_RAM_TZCfg+0x17c>)
30007636:	6019      	str	r1, [r3, #0]
30007638:	2501      	movs	r5, #1
3000763a:	605a      	str	r2, [r3, #4]
3000763c:	2100      	movs	r1, #0
3000763e:	4a40      	ldr	r2, [pc, #256]	; (30007740 <BOOT_RAM_TZCfg+0x180>)
30007640:	4c40      	ldr	r4, [pc, #256]	; (30007744 <BOOT_RAM_TZCfg+0x184>)
30007642:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
30007646:	6813      	ldr	r3, [r2, #0]
30007648:	3301      	adds	r3, #1
3000764a:	d019      	beq.n	30007680 <BOOT_RAM_TZCfg+0xc0>
3000764c:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
30007650:	320c      	adds	r2, #12
30007652:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30007656:	3101      	adds	r1, #1
30007658:	f023 031f 	bic.w	r3, r3, #31
3000765c:	2908      	cmp	r1, #8
3000765e:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
30007662:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30007666:	ea4f 0343 	mov.w	r3, r3, lsl #1
3000766a:	f020 001f 	bic.w	r0, r0, #31
3000766e:	f003 0302 	and.w	r3, r3, #2
30007672:	ea43 0300 	orr.w	r3, r3, r0
30007676:	f043 0301 	orr.w	r3, r3, #1
3000767a:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
3000767e:	d1e2      	bne.n	30007646 <BOOT_RAM_TZCfg+0x86>
30007680:	4830      	ldr	r0, [pc, #192]	; (30007744 <BOOT_RAM_TZCfg+0x184>)
30007682:	2201      	movs	r2, #1
30007684:	4b30      	ldr	r3, [pc, #192]	; (30007748 <BOOT_RAM_TZCfg+0x188>)
30007686:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
3000768a:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000768e:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
30007692:	d11e      	bne.n	300076d2 <BOOT_RAM_TZCfg+0x112>
30007694:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30007698:	f3bf 8f4f 	dsb	sy
3000769c:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300076a0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300076a4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300076a8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300076ac:	0164      	lsls	r4, r4, #5
300076ae:	ea04 0106 	and.w	r1, r4, r6
300076b2:	462b      	mov	r3, r5
300076b4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300076b8:	3b01      	subs	r3, #1
300076ba:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300076be:	1c5a      	adds	r2, r3, #1
300076c0:	d1f8      	bne.n	300076b4 <BOOT_RAM_TZCfg+0xf4>
300076c2:	3c20      	subs	r4, #32
300076c4:	f114 0f20 	cmn.w	r4, #32
300076c8:	d1f1      	bne.n	300076ae <BOOT_RAM_TZCfg+0xee>
300076ca:	f3bf 8f4f 	dsb	sy
300076ce:	f3bf 8f6f 	isb	sy
300076d2:	4b1c      	ldr	r3, [pc, #112]	; (30007744 <BOOT_RAM_TZCfg+0x184>)
300076d4:	f649 75f7 	movw	r5, #40951	; 0x9ff7
300076d8:	491c      	ldr	r1, [pc, #112]	; (3000774c <BOOT_RAM_TZCfg+0x18c>)
300076da:	691c      	ldr	r4, [r3, #16]
300076dc:	4a1c      	ldr	r2, [pc, #112]	; (30007750 <BOOT_RAM_TZCfg+0x190>)
300076de:	f044 0408 	orr.w	r4, r4, #8
300076e2:	611c      	str	r4, [r3, #16]
300076e4:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
300076e8:	68d8      	ldr	r0, [r3, #12]
300076ea:	4028      	ands	r0, r5
300076ec:	4301      	orrs	r1, r0
300076ee:	60d9      	str	r1, [r3, #12]
300076f0:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
300076f4:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
300076f8:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
300076fc:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
30007700:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
30007704:	b005      	add	sp, #20
30007706:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000770a:	f898 3000 	ldrb.w	r3, [r8]
3000770e:	2b00      	cmp	r3, #0
30007710:	d189      	bne.n	30007626 <BOOT_RAM_TZCfg+0x66>
30007712:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
30007716:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
3000771a:	6c33      	ldr	r3, [r6, #64]	; 0x40
3000771c:	f043 0380 	orr.w	r3, r3, #128	; 0x80
30007720:	6433      	str	r3, [r6, #64]	; 0x40
30007722:	6475      	str	r5, [r6, #68]	; 0x44
30007724:	e77f      	b.n	30007626 <BOOT_RAM_TZCfg+0x66>
30007726:	bf00      	nop
30007728:	51001a00 	.word	0x51001a00
3000772c:	51001200 	.word	0x51001200
30007730:	3000e874 	.word	0x3000e874
30007734:	3000e814 	.word	0x3000e814
30007738:	51001b00 	.word	0x51001b00
3000773c:	500e0000 	.word	0x500e0000
30007740:	3000e8d4 	.word	0x3000e8d4
30007744:	e000ed00 	.word	0xe000ed00
30007748:	42008000 	.word	0x42008000
3000774c:	05fa6000 	.word	0x05fa6000
30007750:	e000e100 	.word	0xe000e100
30007754:	3000e76f 	.word	0x3000e76f
30007758:	005fffff 	.word	0x005fffff

3000775c <ymodem_inquiry>:
3000775c:	2806      	cmp	r0, #6
3000775e:	b510      	push	{r4, lr}
30007760:	4604      	mov	r4, r0
30007762:	d003      	beq.n	3000776c <ymodem_inquiry+0x10>
30007764:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007768:	4b03      	ldr	r3, [pc, #12]	; (30007778 <ymodem_inquiry+0x1c>)
3000776a:	4798      	blx	r3
3000776c:	4620      	mov	r0, r4
3000776e:	f000 fa07 	bl	30007b80 <ymodem_uart_putc>
30007772:	2001      	movs	r0, #1
30007774:	bd10      	pop	{r4, pc}
30007776:	bf00      	nop
30007778:	00009b2d 	.word	0x00009b2d

3000777c <ymodem_get_first>:
3000777c:	b530      	push	{r4, r5, lr}
3000777e:	b083      	sub	sp, #12
30007780:	2300      	movs	r3, #0
30007782:	4604      	mov	r4, r0
30007784:	4936      	ldr	r1, [pc, #216]	; (30007860 <ymodem_get_first+0xe4>)
30007786:	f10d 0006 	add.w	r0, sp, #6
3000778a:	f88d 3006 	strb.w	r3, [sp, #6]
3000778e:	f000 fa19 	bl	30007bc4 <ymodem_uart_getc_to>
30007792:	b9d0      	cbnz	r0, 300077ca <ymodem_get_first+0x4e>
30007794:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007798:	3b01      	subs	r3, #1
3000779a:	2b17      	cmp	r3, #23
3000779c:	d80d      	bhi.n	300077ba <ymodem_get_first+0x3e>
3000779e:	e8df f003 	tbb	[pc, r3]
300077a2:	231e      	.short	0x231e
300077a4:	0c0c290c 	.word	0x0c0c290c
300077a8:	0c0c0c0c 	.word	0x0c0c0c0c
300077ac:	0c0c0c0c 	.word	0x0c0c0c0c
300077b0:	0c0c0c0c 	.word	0x0c0c0c0c
300077b4:	0c0c0c0c 	.word	0x0c0c0c0c
300077b8:	190c      	.short	0x190c
300077ba:	4d29      	ldr	r5, [pc, #164]	; (30007860 <ymodem_get_first+0xe4>)
300077bc:	4629      	mov	r1, r5
300077be:	f10d 0007 	add.w	r0, sp, #7
300077c2:	f000 f9ff 	bl	30007bc4 <ymodem_uart_getc_to>
300077c6:	2800      	cmp	r0, #0
300077c8:	d0f8      	beq.n	300077bc <ymodem_get_first+0x40>
300077ca:	2301      	movs	r3, #1
300077cc:	2008      	movs	r0, #8
300077ce:	60a3      	str	r3, [r4, #8]
300077d0:	b003      	add	sp, #12
300077d2:	bd30      	pop	{r4, r5, pc}
300077d4:	2301      	movs	r3, #1
300077d6:	2002      	movs	r0, #2
300077d8:	60a3      	str	r3, [r4, #8]
300077da:	b003      	add	sp, #12
300077dc:	bd30      	pop	{r4, r5, pc}
300077de:	2380      	movs	r3, #128	; 0x80
300077e0:	2001      	movs	r0, #1
300077e2:	80a3      	strh	r3, [r4, #4]
300077e4:	b003      	add	sp, #12
300077e6:	bd30      	pop	{r4, r5, pc}
300077e8:	f44f 6380 	mov.w	r3, #1024	; 0x400
300077ec:	2001      	movs	r0, #1
300077ee:	80a3      	strh	r3, [r4, #4]
300077f0:	b003      	add	sp, #12
300077f2:	bd30      	pop	{r4, r5, pc}
300077f4:	2006      	movs	r0, #6
300077f6:	4d1b      	ldr	r5, [pc, #108]	; (30007864 <ymodem_get_first+0xe8>)
300077f8:	f000 f9c2 	bl	30007b80 <ymodem_uart_putc>
300077fc:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007800:	47a8      	blx	r5
30007802:	2043      	movs	r0, #67	; 0x43
30007804:	f000 f9bc 	bl	30007b80 <ymodem_uart_putc>
30007808:	2006      	movs	r0, #6
3000780a:	f000 f9b9 	bl	30007b80 <ymodem_uart_putc>
3000780e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007812:	47a8      	blx	r5
30007814:	2043      	movs	r0, #67	; 0x43
30007816:	f000 f9b3 	bl	30007b80 <ymodem_uart_putc>
3000781a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000781e:	47a8      	blx	r5
30007820:	2018      	movs	r0, #24
30007822:	f000 f9ad 	bl	30007b80 <ymodem_uart_putc>
30007826:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000782a:	47a8      	blx	r5
3000782c:	2018      	movs	r0, #24
3000782e:	f000 f9a7 	bl	30007b80 <ymodem_uart_putc>
30007832:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007836:	47a8      	blx	r5
30007838:	2018      	movs	r0, #24
3000783a:	f000 f9a1 	bl	30007b80 <ymodem_uart_putc>
3000783e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007842:	47a8      	blx	r5
30007844:	2018      	movs	r0, #24
30007846:	f000 f99b 	bl	30007b80 <ymodem_uart_putc>
3000784a:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000784e:	47a8      	blx	r5
30007850:	2018      	movs	r0, #24
30007852:	f000 f995 	bl	30007b80 <ymodem_uart_putc>
30007856:	2301      	movs	r3, #1
30007858:	2005      	movs	r0, #5
3000785a:	60a3      	str	r3, [r4, #8]
3000785c:	b003      	add	sp, #12
3000785e:	bd30      	pop	{r4, r5, pc}
30007860:	000caa30 	.word	0x000caa30
30007864:	00009b2d 	.word	0x00009b2d

30007868 <ymodem_get_others>:
30007868:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000786c:	b083      	sub	sp, #12
3000786e:	2300      	movs	r3, #0
30007870:	4605      	mov	r5, r0
30007872:	4952      	ldr	r1, [pc, #328]	; (300079bc <ymodem_get_others+0x154>)
30007874:	a801      	add	r0, sp, #4
30007876:	f88d 3006 	strb.w	r3, [sp, #6]
3000787a:	f88d 3007 	strb.w	r3, [sp, #7]
3000787e:	f000 f9a1 	bl	30007bc4 <ymodem_uart_getc_to>
30007882:	bb38      	cbnz	r0, 300078d4 <ymodem_get_others+0x6c>
30007884:	494d      	ldr	r1, [pc, #308]	; (300079bc <ymodem_get_others+0x154>)
30007886:	f10d 0005 	add.w	r0, sp, #5
3000788a:	f000 f99b 	bl	30007bc4 <ymodem_uart_getc_to>
3000788e:	4607      	mov	r7, r0
30007890:	bb00      	cbnz	r0, 300078d4 <ymodem_get_others+0x6c>
30007892:	f89d 2004 	ldrb.w	r2, [sp, #4]
30007896:	f89d 6005 	ldrb.w	r6, [sp, #5]
3000789a:	43d3      	mvns	r3, r2
3000789c:	b2db      	uxtb	r3, r3
3000789e:	429e      	cmp	r6, r3
300078a0:	bf0c      	ite	eq
300078a2:	2601      	moveq	r6, #1
300078a4:	2604      	movne	r6, #4
300078a6:	682b      	ldr	r3, [r5, #0]
300078a8:	429a      	cmp	r2, r3
300078aa:	d21a      	bcs.n	300078e2 <ymodem_get_others+0x7a>
300078ac:	2701      	movs	r7, #1
300078ae:	88ab      	ldrh	r3, [r5, #4]
300078b0:	b1d3      	cbz	r3, 300078e8 <ymodem_get_others+0x80>
300078b2:	2400      	movs	r4, #0
300078b4:	f8df 9108 	ldr.w	r9, [pc, #264]	; 300079c0 <ymodem_get_others+0x158>
300078b8:	f8df 8100 	ldr.w	r8, [pc, #256]	; 300079bc <ymodem_get_others+0x154>
300078bc:	e002      	b.n	300078c4 <ymodem_get_others+0x5c>
300078be:	88ab      	ldrh	r3, [r5, #4]
300078c0:	42a3      	cmp	r3, r4
300078c2:	d911      	bls.n	300078e8 <ymodem_get_others+0x80>
300078c4:	eb04 0009 	add.w	r0, r4, r9
300078c8:	4641      	mov	r1, r8
300078ca:	3401      	adds	r4, #1
300078cc:	f000 f97a 	bl	30007bc4 <ymodem_uart_getc_to>
300078d0:	2800      	cmp	r0, #0
300078d2:	d0f4      	beq.n	300078be <ymodem_get_others+0x56>
300078d4:	2301      	movs	r3, #1
300078d6:	2608      	movs	r6, #8
300078d8:	60ab      	str	r3, [r5, #8]
300078da:	4630      	mov	r0, r6
300078dc:	b003      	add	sp, #12
300078de:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300078e2:	d0e4      	beq.n	300078ae <ymodem_get_others+0x46>
300078e4:	2604      	movs	r6, #4
300078e6:	e7e2      	b.n	300078ae <ymodem_get_others+0x46>
300078e8:	4934      	ldr	r1, [pc, #208]	; (300079bc <ymodem_get_others+0x154>)
300078ea:	f10d 0006 	add.w	r0, sp, #6
300078ee:	f000 f969 	bl	30007bc4 <ymodem_uart_getc_to>
300078f2:	2800      	cmp	r0, #0
300078f4:	d1ee      	bne.n	300078d4 <ymodem_get_others+0x6c>
300078f6:	4931      	ldr	r1, [pc, #196]	; (300079bc <ymodem_get_others+0x154>)
300078f8:	f10d 0007 	add.w	r0, sp, #7
300078fc:	f000 f962 	bl	30007bc4 <ymodem_uart_getc_to>
30007900:	2800      	cmp	r0, #0
30007902:	d1e7      	bne.n	300078d4 <ymodem_get_others+0x6c>
30007904:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007908:	f89d e007 	ldrb.w	lr, [sp, #7]
3000790c:	88aa      	ldrh	r2, [r5, #4]
3000790e:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
30007912:	b36a      	cbz	r2, 30007970 <ymodem_get_others+0x108>
30007914:	4603      	mov	r3, r0
30007916:	482a      	ldr	r0, [pc, #168]	; (300079c0 <ymodem_get_others+0x158>)
30007918:	f241 0c21 	movw	ip, #4129	; 0x1021
3000791c:	eb02 0800 	add.w	r8, r2, r0
30007920:	7801      	ldrb	r1, [r0, #0]
30007922:	3001      	adds	r0, #1
30007924:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
30007928:	2108      	movs	r1, #8
3000792a:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
3000792e:	005b      	lsls	r3, r3, #1
30007930:	b10c      	cbz	r4, 30007936 <ymodem_get_others+0xce>
30007932:	ea83 030c 	eor.w	r3, r3, ip
30007936:	3901      	subs	r1, #1
30007938:	d1f7      	bne.n	3000792a <ymodem_get_others+0xc2>
3000793a:	4580      	cmp	r8, r0
3000793c:	d1f0      	bne.n	30007920 <ymodem_get_others+0xb8>
3000793e:	b29b      	uxth	r3, r3
30007940:	459e      	cmp	lr, r3
30007942:	d00a      	beq.n	3000795a <ymodem_get_others+0xf2>
30007944:	4b1f      	ldr	r3, [pc, #124]	; (300079c4 <ymodem_get_others+0x15c>)
30007946:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000794a:	4798      	blx	r3
3000794c:	2015      	movs	r0, #21
3000794e:	f000 f917 	bl	30007b80 <ymodem_uart_putc>
30007952:	2301      	movs	r3, #1
30007954:	2604      	movs	r6, #4
30007956:	60ab      	str	r3, [r5, #8]
30007958:	e7bf      	b.n	300078da <ymodem_get_others+0x72>
3000795a:	b917      	cbnz	r7, 30007962 <ymodem_get_others+0xfa>
3000795c:	4c1a      	ldr	r4, [pc, #104]	; (300079c8 <ymodem_get_others+0x160>)
3000795e:	6823      	ldr	r3, [r4, #0]
30007960:	b9b3      	cbnz	r3, 30007990 <ymodem_get_others+0x128>
30007962:	f000 f947 	bl	30007bf4 <ymodem_uart_clean_rx>
30007966:	2e01      	cmp	r6, #1
30007968:	d004      	beq.n	30007974 <ymodem_get_others+0x10c>
3000796a:	2e04      	cmp	r6, #4
3000796c:	d1b5      	bne.n	300078da <ymodem_get_others+0x72>
3000796e:	e7e9      	b.n	30007944 <ymodem_get_others+0xdc>
30007970:	4613      	mov	r3, r2
30007972:	e7e5      	b.n	30007940 <ymodem_get_others+0xd8>
30007974:	4c14      	ldr	r4, [pc, #80]	; (300079c8 <ymodem_get_others+0x160>)
30007976:	2006      	movs	r0, #6
30007978:	60ae      	str	r6, [r5, #8]
3000797a:	f000 f901 	bl	30007b80 <ymodem_uart_putc>
3000797e:	6823      	ldr	r3, [r4, #0]
30007980:	b193      	cbz	r3, 300079a8 <ymodem_get_others+0x140>
30007982:	2f00      	cmp	r7, #0
30007984:	f103 0301 	add.w	r3, r3, #1
30007988:	bf18      	it	ne
3000798a:	2604      	movne	r6, #4
3000798c:	6023      	str	r3, [r4, #0]
3000798e:	e7a4      	b.n	300078da <ymodem_get_others+0x72>
30007990:	68eb      	ldr	r3, [r5, #12]
30007992:	6861      	ldr	r1, [r4, #4]
30007994:	3301      	adds	r3, #1
30007996:	480a      	ldr	r0, [pc, #40]	; (300079c0 <ymodem_get_others+0x158>)
30007998:	60eb      	str	r3, [r5, #12]
3000799a:	f000 f94f 	bl	30007c3c <UARTIMG_Write>
3000799e:	6862      	ldr	r2, [r4, #4]
300079a0:	88ab      	ldrh	r3, [r5, #4]
300079a2:	4413      	add	r3, r2
300079a4:	6063      	str	r3, [r4, #4]
300079a6:	e7dc      	b.n	30007962 <ymodem_get_others+0xfa>
300079a8:	4b06      	ldr	r3, [pc, #24]	; (300079c4 <ymodem_get_others+0x15c>)
300079aa:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300079ae:	4798      	blx	r3
300079b0:	2043      	movs	r0, #67	; 0x43
300079b2:	f000 f8e5 	bl	30007b80 <ymodem_uart_putc>
300079b6:	6823      	ldr	r3, [r4, #0]
300079b8:	e7e3      	b.n	30007982 <ymodem_get_others+0x11a>
300079ba:	bf00      	nop
300079bc:	000caa30 	.word	0x000caa30
300079c0:	20016ffc 	.word	0x20016ffc
300079c4:	00009b2d 	.word	0x00009b2d
300079c8:	3000eeb8 	.word	0x3000eeb8

300079cc <ymodem_rx_frame>:
300079cc:	2300      	movs	r3, #0
300079ce:	2201      	movs	r2, #1
300079d0:	b510      	push	{r4, lr}
300079d2:	4604      	mov	r4, r0
300079d4:	6083      	str	r3, [r0, #8]
300079d6:	8082      	strh	r2, [r0, #4]
300079d8:	f7ff fed0 	bl	3000777c <ymodem_get_first>
300079dc:	68a3      	ldr	r3, [r4, #8]
300079de:	b92b      	cbnz	r3, 300079ec <ymodem_rx_frame+0x20>
300079e0:	4620      	mov	r0, r4
300079e2:	f7ff ff41 	bl	30007868 <ymodem_get_others>
300079e6:	68a3      	ldr	r3, [r4, #8]
300079e8:	2b00      	cmp	r3, #0
300079ea:	d0f9      	beq.n	300079e0 <ymodem_rx_frame+0x14>
300079ec:	2801      	cmp	r0, #1
300079ee:	d000      	beq.n	300079f2 <ymodem_rx_frame+0x26>
300079f0:	bd10      	pop	{r4, pc}
300079f2:	6823      	ldr	r3, [r4, #0]
300079f4:	3301      	adds	r3, #1
300079f6:	b2db      	uxtb	r3, r3
300079f8:	6023      	str	r3, [r4, #0]
300079fa:	bd10      	pop	{r4, pc}

300079fc <ymodem_img_rxbuffer>:
300079fc:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007a00:	2300      	movs	r3, #0
30007a02:	4c28      	ldr	r4, [pc, #160]	; (30007aa4 <ymodem_img_rxbuffer+0xa8>)
30007a04:	4e28      	ldr	r6, [pc, #160]	; (30007aa8 <ymodem_img_rxbuffer+0xac>)
30007a06:	460d      	mov	r5, r1
30007a08:	60a3      	str	r3, [r4, #8]
30007a0a:	6163      	str	r3, [r4, #20]
30007a0c:	f000 f8f2 	bl	30007bf4 <ymodem_uart_clean_rx>
30007a10:	2043      	movs	r0, #67	; 0x43
30007a12:	f000 f8b5 	bl	30007b80 <ymodem_uart_putc>
30007a16:	f24c 3050 	movw	r0, #50000	; 0xc350
30007a1a:	47b0      	blx	r6
30007a1c:	f000 f89a 	bl	30007b54 <ymodem_uart_readable>
30007a20:	2800      	cmp	r0, #0
30007a22:	d0f5      	beq.n	30007a10 <ymodem_img_rxbuffer+0x14>
30007a24:	2700      	movs	r7, #0
30007a26:	f04f 0801 	mov.w	r8, #1
30007a2a:	e9c4 7500 	strd	r7, r5, [r4]
30007a2e:	4d1f      	ldr	r5, [pc, #124]	; (30007aac <ymodem_img_rxbuffer+0xb0>)
30007a30:	4628      	mov	r0, r5
30007a32:	f8a4 800c 	strh.w	r8, [r4, #12]
30007a36:	6127      	str	r7, [r4, #16]
30007a38:	f7ff fea0 	bl	3000777c <ymodem_get_first>
30007a3c:	6923      	ldr	r3, [r4, #16]
30007a3e:	b92b      	cbnz	r3, 30007a4c <ymodem_img_rxbuffer+0x50>
30007a40:	4628      	mov	r0, r5
30007a42:	f7ff ff11 	bl	30007868 <ymodem_get_others>
30007a46:	6923      	ldr	r3, [r4, #16]
30007a48:	2b00      	cmp	r3, #0
30007a4a:	d0f9      	beq.n	30007a40 <ymodem_img_rxbuffer+0x44>
30007a4c:	2801      	cmp	r0, #1
30007a4e:	d024      	beq.n	30007a9a <ymodem_img_rxbuffer+0x9e>
30007a50:	3804      	subs	r0, #4
30007a52:	2804      	cmp	r0, #4
30007a54:	d80c      	bhi.n	30007a70 <ymodem_img_rxbuffer+0x74>
30007a56:	a301      	add	r3, pc, #4	; (adr r3, 30007a5c <ymodem_img_rxbuffer+0x60>)
30007a58:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007a5c:	30007a31 	.word	0x30007a31
30007a60:	30007a7d 	.word	0x30007a7d
30007a64:	30007a71 	.word	0x30007a71
30007a68:	30007a71 	.word	0x30007a71
30007a6c:	30007a89 	.word	0x30007a89
30007a70:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a74:	47b0      	blx	r6
30007a76:	2018      	movs	r0, #24
30007a78:	f000 f882 	bl	30007b80 <ymodem_uart_putc>
30007a7c:	200a      	movs	r0, #10
30007a7e:	f000 f8c9 	bl	30007c14 <ymodem_uart_waitbusy>
30007a82:	2001      	movs	r0, #1
30007a84:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007a88:	f000 f8b4 	bl	30007bf4 <ymodem_uart_clean_rx>
30007a8c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a90:	47b0      	blx	r6
30007a92:	2015      	movs	r0, #21
30007a94:	f000 f874 	bl	30007b80 <ymodem_uart_putc>
30007a98:	e7ca      	b.n	30007a30 <ymodem_img_rxbuffer+0x34>
30007a9a:	68a3      	ldr	r3, [r4, #8]
30007a9c:	3301      	adds	r3, #1
30007a9e:	b2db      	uxtb	r3, r3
30007aa0:	60a3      	str	r3, [r4, #8]
30007aa2:	e7c5      	b.n	30007a30 <ymodem_img_rxbuffer+0x34>
30007aa4:	3000eeb8 	.word	0x3000eeb8
30007aa8:	00009b2d 	.word	0x00009b2d
30007aac:	3000eec0 	.word	0x3000eec0

30007ab0 <ymodem_log_open>:
30007ab0:	b510      	push	{r4, lr}
30007ab2:	2303      	movs	r3, #3
30007ab4:	4c04      	ldr	r4, [pc, #16]	; (30007ac8 <ymodem_log_open+0x18>)
30007ab6:	2200      	movs	r2, #0
30007ab8:	2108      	movs	r1, #8
30007aba:	4804      	ldr	r0, [pc, #16]	; (30007acc <ymodem_log_open+0x1c>)
30007abc:	47a0      	blx	r4
30007abe:	2008      	movs	r0, #8
30007ac0:	4b03      	ldr	r3, [pc, #12]	; (30007ad0 <ymodem_log_open+0x20>)
30007ac2:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007ac6:	4718      	bx	r3
30007ac8:	30009b95 	.word	0x30009b95
30007acc:	0000ed39 	.word	0x0000ed39
30007ad0:	0000e1bd 	.word	0x0000e1bd

30007ad4 <ymodem_uart_port_init>:
30007ad4:	b500      	push	{lr}
30007ad6:	b083      	sub	sp, #12
30007ad8:	2008      	movs	r0, #8
30007ada:	4b06      	ldr	r3, [pc, #24]	; (30007af4 <ymodem_uart_port_init+0x20>)
30007adc:	9201      	str	r2, [sp, #4]
30007ade:	4798      	blx	r3
30007ae0:	4b05      	ldr	r3, [pc, #20]	; (30007af8 <ymodem_uart_port_init+0x24>)
30007ae2:	2008      	movs	r0, #8
30007ae4:	4798      	blx	r3
30007ae6:	9901      	ldr	r1, [sp, #4]
30007ae8:	4804      	ldr	r0, [pc, #16]	; (30007afc <ymodem_uart_port_init+0x28>)
30007aea:	4b05      	ldr	r3, [pc, #20]	; (30007b00 <ymodem_uart_port_init+0x2c>)
30007aec:	b003      	add	sp, #12
30007aee:	f85d eb04 	ldr.w	lr, [sp], #4
30007af2:	4718      	bx	r3
30007af4:	0000e1d9 	.word	0x0000e1d9
30007af8:	0000e2fd 	.word	0x0000e2fd
30007afc:	4200c000 	.word	0x4200c000
30007b00:	0000a9e5 	.word	0x0000a9e5

30007b04 <ymodem_uart_port_deinit>:
30007b04:	2804      	cmp	r0, #4
30007b06:	d004      	beq.n	30007b12 <ymodem_uart_port_deinit+0xe>
30007b08:	4b0d      	ldr	r3, [pc, #52]	; (30007b40 <ymodem_uart_port_deinit+0x3c>)
30007b0a:	681b      	ldr	r3, [r3, #0]
30007b0c:	039b      	lsls	r3, r3, #14
30007b0e:	d40f      	bmi.n	30007b30 <ymodem_uart_port_deinit+0x2c>
30007b10:	4770      	bx	lr
30007b12:	2200      	movs	r2, #0
30007b14:	2303      	movs	r3, #3
30007b16:	2108      	movs	r1, #8
30007b18:	480a      	ldr	r0, [pc, #40]	; (30007b44 <ymodem_uart_port_deinit+0x40>)
30007b1a:	b510      	push	{r4, lr}
30007b1c:	4c0a      	ldr	r4, [pc, #40]	; (30007b48 <ymodem_uart_port_deinit+0x44>)
30007b1e:	47a0      	blx	r4
30007b20:	4b0a      	ldr	r3, [pc, #40]	; (30007b4c <ymodem_uart_port_deinit+0x48>)
30007b22:	2008      	movs	r0, #8
30007b24:	4798      	blx	r3
30007b26:	4b06      	ldr	r3, [pc, #24]	; (30007b40 <ymodem_uart_port_deinit+0x3c>)
30007b28:	681b      	ldr	r3, [r3, #0]
30007b2a:	039a      	lsls	r2, r3, #14
30007b2c:	d403      	bmi.n	30007b36 <ymodem_uart_port_deinit+0x32>
30007b2e:	bd10      	pop	{r4, pc}
30007b30:	4807      	ldr	r0, [pc, #28]	; (30007b50 <ymodem_uart_port_deinit+0x4c>)
30007b32:	f004 bc81 	b.w	3000c438 <__DiagPrintf_veneer>
30007b36:	4806      	ldr	r0, [pc, #24]	; (30007b50 <ymodem_uart_port_deinit+0x4c>)
30007b38:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007b3c:	f004 bc7c 	b.w	3000c438 <__DiagPrintf_veneer>
30007b40:	2001c00c 	.word	0x2001c00c
30007b44:	0000ed39 	.word	0x0000ed39
30007b48:	30009b95 	.word	0x30009b95
30007b4c:	0000e1bd 	.word	0x0000e1bd
30007b50:	3000d238 	.word	0x3000d238

30007b54 <ymodem_uart_readable>:
30007b54:	4b00      	ldr	r3, [pc, #0]	; (30007b58 <ymodem_uart_readable+0x4>)
30007b56:	4718      	bx	r3
30007b58:	0000ac39 	.word	0x0000ac39

30007b5c <ymodem_uart_writable>:
30007b5c:	4a03      	ldr	r2, [pc, #12]	; (30007b6c <ymodem_uart_writable+0x10>)
30007b5e:	b508      	push	{r3, lr}
30007b60:	4b03      	ldr	r3, [pc, #12]	; (30007b70 <ymodem_uart_writable+0x14>)
30007b62:	6810      	ldr	r0, [r2, #0]
30007b64:	4798      	blx	r3
30007b66:	b2c0      	uxtb	r0, r0
30007b68:	bd08      	pop	{r3, pc}
30007b6a:	bf00      	nop
30007b6c:	3000eed0 	.word	0x3000eed0
30007b70:	0000d1a9 	.word	0x0000d1a9

30007b74 <ymodem_uart_getc>:
30007b74:	2000      	movs	r0, #0
30007b76:	4b01      	ldr	r3, [pc, #4]	; (30007b7c <ymodem_uart_getc+0x8>)
30007b78:	4718      	bx	r3
30007b7a:	bf00      	nop
30007b7c:	0000ac1d 	.word	0x0000ac1d

30007b80 <ymodem_uart_putc>:
30007b80:	4b00      	ldr	r3, [pc, #0]	; (30007b84 <ymodem_uart_putc+0x4>)
30007b82:	4718      	bx	r3
30007b84:	0000abb5 	.word	0x0000abb5

30007b88 <ymodem_uart_putdata>:
30007b88:	b149      	cbz	r1, 30007b9e <ymodem_uart_putdata+0x16>
30007b8a:	b570      	push	{r4, r5, r6, lr}
30007b8c:	4604      	mov	r4, r0
30007b8e:	1845      	adds	r5, r0, r1
30007b90:	4e03      	ldr	r6, [pc, #12]	; (30007ba0 <ymodem_uart_putdata+0x18>)
30007b92:	f814 0b01 	ldrb.w	r0, [r4], #1
30007b96:	47b0      	blx	r6
30007b98:	42ac      	cmp	r4, r5
30007b9a:	d1fa      	bne.n	30007b92 <ymodem_uart_putdata+0xa>
30007b9c:	bd70      	pop	{r4, r5, r6, pc}
30007b9e:	4770      	bx	lr
30007ba0:	0000abb5 	.word	0x0000abb5

30007ba4 <ymodem_uart_getdata>:
30007ba4:	b151      	cbz	r1, 30007bbc <ymodem_uart_getdata+0x18>
30007ba6:	b570      	push	{r4, r5, r6, lr}
30007ba8:	4604      	mov	r4, r0
30007baa:	1845      	adds	r5, r0, r1
30007bac:	4e04      	ldr	r6, [pc, #16]	; (30007bc0 <ymodem_uart_getdata+0x1c>)
30007bae:	2001      	movs	r0, #1
30007bb0:	47b0      	blx	r6
30007bb2:	f804 0b01 	strb.w	r0, [r4], #1
30007bb6:	42ac      	cmp	r4, r5
30007bb8:	d1f9      	bne.n	30007bae <ymodem_uart_getdata+0xa>
30007bba:	bd70      	pop	{r4, r5, r6, pc}
30007bbc:	4770      	bx	lr
30007bbe:	bf00      	nop
30007bc0:	0000ac1d 	.word	0x0000ac1d

30007bc4 <ymodem_uart_getc_to>:
30007bc4:	b570      	push	{r4, r5, r6, lr}
30007bc6:	4d09      	ldr	r5, [pc, #36]	; (30007bec <ymodem_uart_getc_to+0x28>)
30007bc8:	4606      	mov	r6, r0
30007bca:	460c      	mov	r4, r1
30007bcc:	e001      	b.n	30007bd2 <ymodem_uart_getc_to+0xe>
30007bce:	3c01      	subs	r4, #1
30007bd0:	d009      	beq.n	30007be6 <ymodem_uart_getc_to+0x22>
30007bd2:	47a8      	blx	r5
30007bd4:	2800      	cmp	r0, #0
30007bd6:	d0fa      	beq.n	30007bce <ymodem_uart_getc_to+0xa>
30007bd8:	4b05      	ldr	r3, [pc, #20]	; (30007bf0 <ymodem_uart_getc_to+0x2c>)
30007bda:	2000      	movs	r0, #0
30007bdc:	4798      	blx	r3
30007bde:	4603      	mov	r3, r0
30007be0:	2000      	movs	r0, #0
30007be2:	7033      	strb	r3, [r6, #0]
30007be4:	bd70      	pop	{r4, r5, r6, pc}
30007be6:	2001      	movs	r0, #1
30007be8:	bd70      	pop	{r4, r5, r6, pc}
30007bea:	bf00      	nop
30007bec:	0000ac39 	.word	0x0000ac39
30007bf0:	0000ac1d 	.word	0x0000ac1d

30007bf4 <ymodem_uart_clean_rx>:
30007bf4:	b538      	push	{r3, r4, r5, lr}
30007bf6:	4c05      	ldr	r4, [pc, #20]	; (30007c0c <ymodem_uart_clean_rx+0x18>)
30007bf8:	4d05      	ldr	r5, [pc, #20]	; (30007c10 <ymodem_uart_clean_rx+0x1c>)
30007bfa:	e000      	b.n	30007bfe <ymodem_uart_clean_rx+0xa>
30007bfc:	47a8      	blx	r5
30007bfe:	47a0      	blx	r4
30007c00:	4603      	mov	r3, r0
30007c02:	2000      	movs	r0, #0
30007c04:	2b00      	cmp	r3, #0
30007c06:	d1f9      	bne.n	30007bfc <ymodem_uart_clean_rx+0x8>
30007c08:	bd38      	pop	{r3, r4, r5, pc}
30007c0a:	bf00      	nop
30007c0c:	0000ac39 	.word	0x0000ac39
30007c10:	0000ac1d 	.word	0x0000ac1d

30007c14 <ymodem_uart_waitbusy>:
30007c14:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007c16:	2400      	movs	r4, #0
30007c18:	4e06      	ldr	r6, [pc, #24]	; (30007c34 <ymodem_uart_waitbusy+0x20>)
30007c1a:	4f07      	ldr	r7, [pc, #28]	; (30007c38 <ymodem_uart_waitbusy+0x24>)
30007c1c:	4605      	mov	r5, r0
30007c1e:	e002      	b.n	30007c26 <ymodem_uart_waitbusy+0x12>
30007c20:	47b8      	blx	r7
30007c22:	42ac      	cmp	r4, r5
30007c24:	d804      	bhi.n	30007c30 <ymodem_uart_waitbusy+0x1c>
30007c26:	6973      	ldr	r3, [r6, #20]
30007c28:	3401      	adds	r4, #1
30007c2a:	2064      	movs	r0, #100	; 0x64
30007c2c:	03db      	lsls	r3, r3, #15
30007c2e:	d5f7      	bpl.n	30007c20 <ymodem_uart_waitbusy+0xc>
30007c30:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007c32:	bf00      	nop
30007c34:	4200c000 	.word	0x4200c000
30007c38:	00009b2d 	.word	0x00009b2d

30007c3c <UARTIMG_Write>:
30007c3c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007c40:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007c44:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007c48:	d21d      	bcs.n	30007c86 <UARTIMG_Write+0x4a>
30007c4a:	3a01      	subs	r2, #1
30007c4c:	4f19      	ldr	r7, [pc, #100]	; (30007cb4 <UARTIMG_Write+0x78>)
30007c4e:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007cc4 <UARTIMG_Write+0x88>
30007c52:	4688      	mov	r8, r1
30007c54:	1916      	adds	r6, r2, r4
30007c56:	4605      	mov	r5, r0
30007c58:	f8d9 3000 	ldr.w	r3, [r9]
30007c5c:	403e      	ands	r6, r7
30007c5e:	4027      	ands	r7, r4
30007c60:	42bb      	cmp	r3, r7
30007c62:	d91b      	bls.n	30007c9c <UARTIMG_Write+0x60>
30007c64:	42b3      	cmp	r3, r6
30007c66:	d910      	bls.n	30007c8a <UARTIMG_Write+0x4e>
30007c68:	4e13      	ldr	r6, [pc, #76]	; (30007cb8 <UARTIMG_Write+0x7c>)
30007c6a:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007c6e:	4f13      	ldr	r7, [pc, #76]	; (30007cbc <UARTIMG_Write+0x80>)
30007c70:	4446      	add	r6, r8
30007c72:	eba3 0808 	sub.w	r8, r3, r8
30007c76:	eb08 0204 	add.w	r2, r8, r4
30007c7a:	4620      	mov	r0, r4
30007c7c:	3408      	adds	r4, #8
30007c7e:	2108      	movs	r1, #8
30007c80:	47b8      	blx	r7
30007c82:	42b4      	cmp	r4, r6
30007c84:	d1f7      	bne.n	30007c76 <UARTIMG_Write+0x3a>
30007c86:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007c8a:	4631      	mov	r1, r6
30007c8c:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007c90:	2002      	movs	r0, #2
30007c92:	4b0b      	ldr	r3, [pc, #44]	; (30007cc0 <UARTIMG_Write+0x84>)
30007c94:	4798      	blx	r3
30007c96:	f8c9 6000 	str.w	r6, [r9]
30007c9a:	e7e5      	b.n	30007c68 <UARTIMG_Write+0x2c>
30007c9c:	4b08      	ldr	r3, [pc, #32]	; (30007cc0 <UARTIMG_Write+0x84>)
30007c9e:	4639      	mov	r1, r7
30007ca0:	2002      	movs	r0, #2
30007ca2:	4798      	blx	r3
30007ca4:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007ca8:	42b3      	cmp	r3, r6
30007caa:	f8c9 3000 	str.w	r3, [r9]
30007cae:	d8db      	bhi.n	30007c68 <UARTIMG_Write+0x2c>
30007cb0:	e7eb      	b.n	30007c8a <UARTIMG_Write+0x4e>
30007cb2:	bf00      	nop
30007cb4:	fffff000 	.word	0xfffff000
30007cb8:	f8000400 	.word	0xf8000400
30007cbc:	0000a04d 	.word	0x0000a04d
30007cc0:	0000a16d 	.word	0x0000a16d
30007cc4:	3000eed4 	.word	0x3000eed4

30007cc8 <ymodem_img_download>:
30007cc8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007ccc:	4616      	mov	r6, r2
30007cce:	460d      	mov	r5, r1
30007cd0:	4a0a      	ldr	r2, [pc, #40]	; (30007cfc <ymodem_img_download+0x34>)
30007cd2:	2100      	movs	r1, #0
30007cd4:	4680      	mov	r8, r0
30007cd6:	f7ff fefd 	bl	30007ad4 <ymodem_uart_port_init>
30007cda:	b156      	cbz	r6, 30007cf2 <ymodem_img_download+0x2a>
30007cdc:	3d04      	subs	r5, #4
30007cde:	2400      	movs	r4, #0
30007ce0:	4f06      	ldr	r7, [pc, #24]	; (30007cfc <ymodem_img_download+0x34>)
30007ce2:	3401      	adds	r4, #1
30007ce4:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007ce8:	4638      	mov	r0, r7
30007cea:	f7ff fe87 	bl	300079fc <ymodem_img_rxbuffer>
30007cee:	42b4      	cmp	r4, r6
30007cf0:	d1f7      	bne.n	30007ce2 <ymodem_img_download+0x1a>
30007cf2:	4640      	mov	r0, r8
30007cf4:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007cf8:	f7ff bf04 	b.w	30007b04 <ymodem_uart_port_deinit>
30007cfc:	0016e360 	.word	0x0016e360

30007d00 <ChipInfo_Invalid>:
30007d00:	4e06      	ldr	r6, [pc, #24]	; (30007d1c <ChipInfo_Invalid+0x1c>)
30007d02:	4d07      	ldr	r5, [pc, #28]	; (30007d20 <ChipInfo_Invalid+0x20>)
30007d04:	4c07      	ldr	r4, [pc, #28]	; (30007d24 <ChipInfo_Invalid+0x24>)
30007d06:	b508      	push	{r3, lr}
30007d08:	462b      	mov	r3, r5
30007d0a:	4622      	mov	r2, r4
30007d0c:	4904      	ldr	r1, [pc, #16]	; (30007d20 <ChipInfo_Invalid+0x20>)
30007d0e:	2002      	movs	r0, #2
30007d10:	f004 f91c 	bl	3000bf4c <rtk_log_write>
30007d14:	f242 7010 	movw	r0, #10000	; 0x2710
30007d18:	47b0      	blx	r6
30007d1a:	e7f5      	b.n	30007d08 <ChipInfo_Invalid+0x8>
30007d1c:	00009be5 	.word	0x00009be5
30007d20:	3000d258 	.word	0x3000d258
30007d24:	3000d264 	.word	0x3000d264

30007d28 <ChipInfo_Get>:
30007d28:	b570      	push	{r4, r5, r6, lr}
30007d2a:	4c11      	ldr	r4, [pc, #68]	; (30007d70 <ChipInfo_Get+0x48>)
30007d2c:	7820      	ldrb	r0, [r4, #0]
30007d2e:	28ff      	cmp	r0, #255	; 0xff
30007d30:	d104      	bne.n	30007d3c <ChipInfo_Get+0x14>
30007d32:	4d10      	ldr	r5, [pc, #64]	; (30007d74 <ChipInfo_Get+0x4c>)
30007d34:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007d38:	b108      	cbz	r0, 30007d3e <ChipInfo_Get+0x16>
30007d3a:	7020      	strb	r0, [r4, #0]
30007d3c:	bd70      	pop	{r4, r5, r6, pc}
30007d3e:	4621      	mov	r1, r4
30007d40:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007d44:	f000 fec4 	bl	30008ad0 <OTP_Read8>
30007d48:	7820      	ldrb	r0, [r4, #0]
30007d4a:	28ff      	cmp	r0, #255	; 0xff
30007d4c:	d002      	beq.n	30007d54 <ChipInfo_Get+0x2c>
30007d4e:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007d52:	bd70      	pop	{r4, r5, r6, pc}
30007d54:	4e08      	ldr	r6, [pc, #32]	; (30007d78 <ChipInfo_Get+0x50>)
30007d56:	4d09      	ldr	r5, [pc, #36]	; (30007d7c <ChipInfo_Get+0x54>)
30007d58:	4c09      	ldr	r4, [pc, #36]	; (30007d80 <ChipInfo_Get+0x58>)
30007d5a:	462b      	mov	r3, r5
30007d5c:	4622      	mov	r2, r4
30007d5e:	4907      	ldr	r1, [pc, #28]	; (30007d7c <ChipInfo_Get+0x54>)
30007d60:	2003      	movs	r0, #3
30007d62:	f004 f8f3 	bl	3000bf4c <rtk_log_write>
30007d66:	f242 7010 	movw	r0, #10000	; 0x2710
30007d6a:	47b0      	blx	r6
30007d6c:	e7f5      	b.n	30007d5a <ChipInfo_Get+0x32>
30007d6e:	bf00      	nop
30007d70:	3000efd0 	.word	0x3000efd0
30007d74:	23020000 	.word	0x23020000
30007d78:	00009be5 	.word	0x00009be5
30007d7c:	3000d258 	.word	0x3000d258
30007d80:	3000d288 	.word	0x3000d288

30007d84 <ChipInfo_MemoryType>:
30007d84:	b510      	push	{r4, lr}
30007d86:	4c11      	ldr	r4, [pc, #68]	; (30007dcc <ChipInfo_MemoryType+0x48>)
30007d88:	47a0      	blx	r4
30007d8a:	2802      	cmp	r0, #2
30007d8c:	d019      	beq.n	30007dc2 <ChipInfo_MemoryType+0x3e>
30007d8e:	47a0      	blx	r4
30007d90:	2803      	cmp	r0, #3
30007d92:	d016      	beq.n	30007dc2 <ChipInfo_MemoryType+0x3e>
30007d94:	4c0e      	ldr	r4, [pc, #56]	; (30007dd0 <ChipInfo_MemoryType+0x4c>)
30007d96:	4b0f      	ldr	r3, [pc, #60]	; (30007dd4 <ChipInfo_MemoryType+0x50>)
30007d98:	4798      	blx	r3
30007d9a:	2100      	movs	r1, #0
30007d9c:	23e0      	movs	r3, #224	; 0xe0
30007d9e:	4622      	mov	r2, r4
30007da0:	e004      	b.n	30007dac <ChipInfo_MemoryType+0x28>
30007da2:	7a13      	ldrb	r3, [r2, #8]
30007da4:	3101      	adds	r1, #1
30007da6:	3206      	adds	r2, #6
30007da8:	2bff      	cmp	r3, #255	; 0xff
30007daa:	d00c      	beq.n	30007dc6 <ChipInfo_MemoryType+0x42>
30007dac:	4298      	cmp	r0, r3
30007dae:	d1f8      	bne.n	30007da2 <ChipInfo_MemoryType+0x1e>
30007db0:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007db4:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007db8:	78cb      	ldrb	r3, [r1, #3]
30007dba:	2b02      	cmp	r3, #2
30007dbc:	d001      	beq.n	30007dc2 <ChipInfo_MemoryType+0x3e>
30007dbe:	2001      	movs	r0, #1
30007dc0:	bd10      	pop	{r4, pc}
30007dc2:	2002      	movs	r0, #2
30007dc4:	bd10      	pop	{r4, pc}
30007dc6:	f7ff ff9b 	bl	30007d00 <ChipInfo_Invalid>
30007dca:	bf00      	nop
30007dcc:	0000c0f9 	.word	0x0000c0f9
30007dd0:	3000d2b4 	.word	0x3000d2b4
30007dd4:	30007d29 	.word	0x30007d29

30007dd8 <ChipInfo_DDRType>:
30007dd8:	b510      	push	{r4, lr}
30007dda:	4b0f      	ldr	r3, [pc, #60]	; (30007e18 <ChipInfo_DDRType+0x40>)
30007ddc:	4798      	blx	r3
30007dde:	4b0f      	ldr	r3, [pc, #60]	; (30007e1c <ChipInfo_DDRType+0x44>)
30007de0:	4604      	mov	r4, r0
30007de2:	4798      	blx	r3
30007de4:	2801      	cmp	r0, #1
30007de6:	d112      	bne.n	30007e0e <ChipInfo_DDRType+0x36>
30007de8:	480d      	ldr	r0, [pc, #52]	; (30007e20 <ChipInfo_DDRType+0x48>)
30007dea:	2100      	movs	r1, #0
30007dec:	22e0      	movs	r2, #224	; 0xe0
30007dee:	4603      	mov	r3, r0
30007df0:	e003      	b.n	30007dfa <ChipInfo_DDRType+0x22>
30007df2:	789a      	ldrb	r2, [r3, #2]
30007df4:	3101      	adds	r1, #1
30007df6:	2aff      	cmp	r2, #255	; 0xff
30007df8:	d00b      	beq.n	30007e12 <ChipInfo_DDRType+0x3a>
30007dfa:	4294      	cmp	r4, r2
30007dfc:	f103 0306 	add.w	r3, r3, #6
30007e00:	d1f7      	bne.n	30007df2 <ChipInfo_DDRType+0x1a>
30007e02:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e06:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007e0a:	78c8      	ldrb	r0, [r1, #3]
30007e0c:	bd10      	pop	{r4, pc}
30007e0e:	20ff      	movs	r0, #255	; 0xff
30007e10:	bd10      	pop	{r4, pc}
30007e12:	2000      	movs	r0, #0
30007e14:	bd10      	pop	{r4, pc}
30007e16:	bf00      	nop
30007e18:	30007d29 	.word	0x30007d29
30007e1c:	30007d85 	.word	0x30007d85
30007e20:	3000d2b4 	.word	0x3000d2b4

30007e24 <ChipInfo_MemorySize>:
30007e24:	b510      	push	{r4, lr}
30007e26:	4c0b      	ldr	r4, [pc, #44]	; (30007e54 <ChipInfo_MemorySize+0x30>)
30007e28:	4b0b      	ldr	r3, [pc, #44]	; (30007e58 <ChipInfo_MemorySize+0x34>)
30007e2a:	4798      	blx	r3
30007e2c:	2100      	movs	r1, #0
30007e2e:	23e0      	movs	r3, #224	; 0xe0
30007e30:	4622      	mov	r2, r4
30007e32:	e004      	b.n	30007e3e <ChipInfo_MemorySize+0x1a>
30007e34:	7a13      	ldrb	r3, [r2, #8]
30007e36:	3101      	adds	r1, #1
30007e38:	3206      	adds	r2, #6
30007e3a:	2bff      	cmp	r3, #255	; 0xff
30007e3c:	d007      	beq.n	30007e4e <ChipInfo_MemorySize+0x2a>
30007e3e:	4298      	cmp	r0, r3
30007e40:	d1f8      	bne.n	30007e34 <ChipInfo_MemorySize+0x10>
30007e42:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e46:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e4a:	7908      	ldrb	r0, [r1, #4]
30007e4c:	bd10      	pop	{r4, pc}
30007e4e:	f7ff ff57 	bl	30007d00 <ChipInfo_Invalid>
30007e52:	bf00      	nop
30007e54:	3000d2b4 	.word	0x3000d2b4
30007e58:	30007d29 	.word	0x30007d29

30007e5c <ChipInfo_MemoryVendor>:
30007e5c:	b510      	push	{r4, lr}
30007e5e:	4c0f      	ldr	r4, [pc, #60]	; (30007e9c <ChipInfo_MemoryVendor+0x40>)
30007e60:	47a0      	blx	r4
30007e62:	2803      	cmp	r0, #3
30007e64:	d016      	beq.n	30007e94 <ChipInfo_MemoryVendor+0x38>
30007e66:	47a0      	blx	r4
30007e68:	2802      	cmp	r0, #2
30007e6a:	d012      	beq.n	30007e92 <ChipInfo_MemoryVendor+0x36>
30007e6c:	4c0c      	ldr	r4, [pc, #48]	; (30007ea0 <ChipInfo_MemoryVendor+0x44>)
30007e6e:	4b0d      	ldr	r3, [pc, #52]	; (30007ea4 <ChipInfo_MemoryVendor+0x48>)
30007e70:	4798      	blx	r3
30007e72:	2100      	movs	r1, #0
30007e74:	23e0      	movs	r3, #224	; 0xe0
30007e76:	4622      	mov	r2, r4
30007e78:	e004      	b.n	30007e84 <ChipInfo_MemoryVendor+0x28>
30007e7a:	7a13      	ldrb	r3, [r2, #8]
30007e7c:	3101      	adds	r1, #1
30007e7e:	3206      	adds	r2, #6
30007e80:	2bff      	cmp	r3, #255	; 0xff
30007e82:	d009      	beq.n	30007e98 <ChipInfo_MemoryVendor+0x3c>
30007e84:	4298      	cmp	r0, r3
30007e86:	d1f8      	bne.n	30007e7a <ChipInfo_MemoryVendor+0x1e>
30007e88:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e8c:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e90:	7948      	ldrb	r0, [r1, #5]
30007e92:	bd10      	pop	{r4, pc}
30007e94:	2001      	movs	r0, #1
30007e96:	bd10      	pop	{r4, pc}
30007e98:	f7ff ff32 	bl	30007d00 <ChipInfo_Invalid>
30007e9c:	0000c0f9 	.word	0x0000c0f9
30007ea0:	3000d2b4 	.word	0x3000d2b4
30007ea4:	30007d29 	.word	0x30007d29

30007ea8 <SDM32K_Enable>:
30007ea8:	4a02      	ldr	r2, [pc, #8]	; (30007eb4 <SDM32K_Enable+0xc>)
30007eaa:	4b03      	ldr	r3, [pc, #12]	; (30007eb8 <SDM32K_Enable+0x10>)
30007eac:	6811      	ldr	r1, [r2, #0]
30007eae:	430b      	orrs	r3, r1
30007eb0:	6013      	str	r3, [r2, #0]
30007eb2:	4770      	bx	lr
30007eb4:	42008e00 	.word	0x42008e00
30007eb8:	c0060000 	.word	0xc0060000

30007ebc <CLK_SWITCH_XTAL>:
30007ebc:	b508      	push	{r3, lr}
30007ebe:	2801      	cmp	r0, #1
30007ec0:	4b0a      	ldr	r3, [pc, #40]	; (30007eec <CLK_SWITCH_XTAL+0x30>)
30007ec2:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007ec6:	d00d      	beq.n	30007ee4 <CLK_SWITCH_XTAL+0x28>
30007ec8:	4b09      	ldr	r3, [pc, #36]	; (30007ef0 <CLK_SWITCH_XTAL+0x34>)
30007eca:	4313      	orrs	r3, r2
30007ecc:	4907      	ldr	r1, [pc, #28]	; (30007eec <CLK_SWITCH_XTAL+0x30>)
30007ece:	4a09      	ldr	r2, [pc, #36]	; (30007ef4 <CLK_SWITCH_XTAL+0x38>)
30007ed0:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007ed4:	4790      	blx	r2
30007ed6:	4a08      	ldr	r2, [pc, #32]	; (30007ef8 <CLK_SWITCH_XTAL+0x3c>)
30007ed8:	4b08      	ldr	r3, [pc, #32]	; (30007efc <CLK_SWITCH_XTAL+0x40>)
30007eda:	fba2 2000 	umull	r2, r0, r2, r0
30007ede:	0c80      	lsrs	r0, r0, #18
30007ee0:	6018      	str	r0, [r3, #0]
30007ee2:	bd08      	pop	{r3, pc}
30007ee4:	4b06      	ldr	r3, [pc, #24]	; (30007f00 <CLK_SWITCH_XTAL+0x44>)
30007ee6:	4013      	ands	r3, r2
30007ee8:	e7f0      	b.n	30007ecc <CLK_SWITCH_XTAL+0x10>
30007eea:	bf00      	nop
30007eec:	42008000 	.word	0x42008000
30007ef0:	80000500 	.word	0x80000500
30007ef4:	30009865 	.word	0x30009865
30007ef8:	431bde83 	.word	0x431bde83
30007efc:	2001c700 	.word	0x2001c700
30007f00:	7ffffaff 	.word	0x7ffffaff

30007f04 <DDR_PHY_ChipInfo.part.0>:
30007f04:	b508      	push	{r3, lr}
30007f06:	4b24      	ldr	r3, [pc, #144]	; (30007f98 <DDR_PHY_ChipInfo.part.0+0x94>)
30007f08:	4798      	blx	r3
30007f0a:	280e      	cmp	r0, #14
30007f0c:	d80d      	bhi.n	30007f2a <DDR_PHY_ChipInfo.part.0+0x26>
30007f0e:	2801      	cmp	r0, #1
30007f10:	d912      	bls.n	30007f38 <DDR_PHY_ChipInfo.part.0+0x34>
30007f12:	1e83      	subs	r3, r0, #2
30007f14:	2b0c      	cmp	r3, #12
30007f16:	d80f      	bhi.n	30007f38 <DDR_PHY_ChipInfo.part.0+0x34>
30007f18:	e8df f003 	tbb	[pc, r3]
30007f1c:	210e1d19 	.word	0x210e1d19
30007f20:	2a0e260e 	.word	0x2a0e260e
30007f24:	340e0e2f 	.word	0x340e0e2f
30007f28:	39          	.byte	0x39
30007f29:	00          	.byte	0x00
30007f2a:	28e0      	cmp	r0, #224	; 0xe0
30007f2c:	d104      	bne.n	30007f38 <DDR_PHY_ChipInfo.part.0+0x34>
30007f2e:	2301      	movs	r3, #1
30007f30:	4a1a      	ldr	r2, [pc, #104]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f32:	4618      	mov	r0, r3
30007f34:	7013      	strb	r3, [r2, #0]
30007f36:	bd08      	pop	{r3, pc}
30007f38:	4b19      	ldr	r3, [pc, #100]	; (30007fa0 <DDR_PHY_ChipInfo.part.0+0x9c>)
30007f3a:	2002      	movs	r0, #2
30007f3c:	4a19      	ldr	r2, [pc, #100]	; (30007fa4 <DDR_PHY_ChipInfo.part.0+0xa0>)
30007f3e:	4619      	mov	r1, r3
30007f40:	f004 f804 	bl	3000bf4c <rtk_log_write>
30007f44:	2300      	movs	r3, #0
30007f46:	4a15      	ldr	r2, [pc, #84]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f48:	4618      	mov	r0, r3
30007f4a:	7013      	strb	r3, [r2, #0]
30007f4c:	bd08      	pop	{r3, pc}
30007f4e:	4b13      	ldr	r3, [pc, #76]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f50:	2202      	movs	r2, #2
30007f52:	701a      	strb	r2, [r3, #0]
30007f54:	bd08      	pop	{r3, pc}
30007f56:	4b11      	ldr	r3, [pc, #68]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f58:	2203      	movs	r2, #3
30007f5a:	701a      	strb	r2, [r3, #0]
30007f5c:	bd08      	pop	{r3, pc}
30007f5e:	2304      	movs	r3, #4
30007f60:	4a0e      	ldr	r2, [pc, #56]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f62:	4618      	mov	r0, r3
30007f64:	7013      	strb	r3, [r2, #0]
30007f66:	bd08      	pop	{r3, pc}
30007f68:	4b0c      	ldr	r3, [pc, #48]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f6a:	2207      	movs	r2, #7
30007f6c:	701a      	strb	r2, [r3, #0]
30007f6e:	bd08      	pop	{r3, pc}
30007f70:	2305      	movs	r3, #5
30007f72:	4a0a      	ldr	r2, [pc, #40]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f74:	4618      	mov	r0, r3
30007f76:	7013      	strb	r3, [r2, #0]
30007f78:	bd08      	pop	{r3, pc}
30007f7a:	2306      	movs	r3, #6
30007f7c:	4a07      	ldr	r2, [pc, #28]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f7e:	4618      	mov	r0, r3
30007f80:	7013      	strb	r3, [r2, #0]
30007f82:	bd08      	pop	{r3, pc}
30007f84:	2308      	movs	r3, #8
30007f86:	4a05      	ldr	r2, [pc, #20]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f88:	4618      	mov	r0, r3
30007f8a:	7013      	strb	r3, [r2, #0]
30007f8c:	bd08      	pop	{r3, pc}
30007f8e:	2309      	movs	r3, #9
30007f90:	4a02      	ldr	r2, [pc, #8]	; (30007f9c <DDR_PHY_ChipInfo.part.0+0x98>)
30007f92:	4618      	mov	r0, r3
30007f94:	7013      	strb	r3, [r2, #0]
30007f96:	bd08      	pop	{r3, pc}
30007f98:	30007d29 	.word	0x30007d29
30007f9c:	3000efd1 	.word	0x3000efd1
30007fa0:	3000d338 	.word	0x3000d338
30007fa4:	3000d340 	.word	0x3000d340

30007fa8 <DDR_PHY_ChipInfo_ddrtype>:
30007fa8:	b510      	push	{r4, lr}
30007faa:	4c04      	ldr	r4, [pc, #16]	; (30007fbc <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007fac:	7820      	ldrb	r0, [r4, #0]
30007fae:	b100      	cbz	r0, 30007fb2 <DDR_PHY_ChipInfo_ddrtype+0xa>
30007fb0:	bd10      	pop	{r4, pc}
30007fb2:	4b03      	ldr	r3, [pc, #12]	; (30007fc0 <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007fb4:	4798      	blx	r3
30007fb6:	7020      	strb	r0, [r4, #0]
30007fb8:	bd10      	pop	{r4, pc}
30007fba:	bf00      	nop
30007fbc:	3000fcf0 	.word	0x3000fcf0
30007fc0:	30007dd9 	.word	0x30007dd9

30007fc4 <DDR_PHY_StructInit>:
30007fc4:	f240 2315 	movw	r3, #533	; 0x215
30007fc8:	6583      	str	r3, [r0, #88]	; 0x58
30007fca:	4770      	bx	lr

30007fcc <DDR_PHY_DLL_CLK_DIV>:
30007fcc:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007fce:	4d70      	ldr	r5, [pc, #448]	; (30008190 <DDR_PHY_DLL_CLK_DIV+0x1c4>)
30007fd0:	4c70      	ldr	r4, [pc, #448]	; (30008194 <DDR_PHY_DLL_CLK_DIV+0x1c8>)
30007fd2:	fba5 3200 	umull	r3, r2, r5, r0
30007fd6:	2365      	movs	r3, #101	; 0x65
30007fd8:	eb00 0e04 	add.w	lr, r0, r4
30007fdc:	0952      	lsrs	r2, r2, #5
30007fde:	fb00 f303 	mul.w	r3, r0, r3
30007fe2:	486d      	ldr	r0, [pc, #436]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007fe4:	f1a2 0c03 	sub.w	ip, r2, #3
30007fe8:	6a47      	ldr	r7, [r0, #36]	; 0x24
30007fea:	eb0c 018c 	add.w	r1, ip, ip, lsl #2
30007fee:	ea4f 2c0c 	mov.w	ip, ip, lsl #8
30007ff2:	ebae 0ec1 	sub.w	lr, lr, r1, lsl #3
30007ff6:	fa1f fc8c 	uxth.w	ip, ip
30007ffa:	ea4f 2ece 	mov.w	lr, lr, lsl #11
30007ffe:	fba5 1e0e 	umull	r1, lr, r5, lr
30008002:	4966      	ldr	r1, [pc, #408]	; (3000819c <DDR_PHY_DLL_CLK_DIV+0x1d0>)
30008004:	fba1 6103 	umull	r6, r1, r1, r3
30008008:	4e65      	ldr	r6, [pc, #404]	; (300081a0 <DDR_PHY_DLL_CLK_DIV+0x1d4>)
3000800a:	0a09      	lsrs	r1, r1, #8
3000800c:	1a52      	subs	r2, r2, r1
3000800e:	3903      	subs	r1, #3
30008010:	02d2      	lsls	r2, r2, #11
30008012:	eb02 125e 	add.w	r2, r2, lr, lsr #5
30008016:	ea06 2ece 	and.w	lr, r6, lr, lsl #11
3000801a:	4e62      	ldr	r6, [pc, #392]	; (300081a4 <DDR_PHY_DLL_CLK_DIV+0x1d8>)
3000801c:	403e      	ands	r6, r7
3000801e:	ea4e 0606 	orr.w	r6, lr, r6
30008022:	6246      	str	r6, [r0, #36]	; 0x24
30008024:	4e60      	ldr	r6, [pc, #384]	; (300081a8 <DDR_PHY_DLL_CLK_DIV+0x1dc>)
30008026:	fba6 6303 	umull	r6, r3, r6, r3
3000802a:	eb04 1353 	add.w	r3, r4, r3, lsr #5
3000802e:	6a84      	ldr	r4, [r0, #40]	; 0x28
30008030:	f424 447f 	bic.w	r4, r4, #65280	; 0xff00
30008034:	ea4c 0404 	orr.w	r4, ip, r4
30008038:	6284      	str	r4, [r0, #40]	; 0x28
3000803a:	eb01 0481 	add.w	r4, r1, r1, lsl #2
3000803e:	b2c9      	uxtb	r1, r1
30008040:	eba3 03c4 	sub.w	r3, r3, r4, lsl #3
30008044:	02db      	lsls	r3, r3, #11
30008046:	fba5 4303 	umull	r4, r3, r5, r3
3000804a:	6a45      	ldr	r5, [r0, #36]	; 0x24
3000804c:	4c57      	ldr	r4, [pc, #348]	; (300081ac <DDR_PHY_DLL_CLK_DIV+0x1e0>)
3000804e:	eba2 1253 	sub.w	r2, r2, r3, lsr #5
30008052:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008056:	402c      	ands	r4, r5
30008058:	4d55      	ldr	r5, [pc, #340]	; (300081b0 <DDR_PHY_DLL_CLK_DIV+0x1e4>)
3000805a:	eb02 1242 	add.w	r2, r2, r2, lsl #5
3000805e:	4323      	orrs	r3, r4
30008060:	02d2      	lsls	r2, r2, #11
30008062:	6243      	str	r3, [r0, #36]	; 0x24
30008064:	6a84      	ldr	r4, [r0, #40]	; 0x28
30008066:	fba5 3202 	umull	r3, r2, r5, r2
3000806a:	4b52      	ldr	r3, [pc, #328]	; (300081b4 <DDR_PHY_DLL_CLK_DIV+0x1e8>)
3000806c:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008070:	4321      	orrs	r1, r4
30008072:	2401      	movs	r4, #1
30008074:	6281      	str	r1, [r0, #40]	; 0x28
30008076:	6a01      	ldr	r1, [r0, #32]
30008078:	400b      	ands	r3, r1
3000807a:	494f      	ldr	r1, [pc, #316]	; (300081b8 <DDR_PHY_DLL_CLK_DIV+0x1ec>)
3000807c:	ea43 3292 	orr.w	r2, r3, r2, lsr #14
30008080:	6202      	str	r2, [r0, #32]
30008082:	6784      	str	r4, [r0, #120]	; 0x78
30008084:	7808      	ldrb	r0, [r1, #0]
30008086:	28ff      	cmp	r0, #255	; 0xff
30008088:	d075      	beq.n	30008176 <DDR_PHY_DLL_CLK_DIV+0x1aa>
3000808a:	4c4c      	ldr	r4, [pc, #304]	; (300081bc <DDR_PHY_DLL_CLK_DIV+0x1f0>)
3000808c:	0145      	lsls	r5, r0, #5
3000808e:	4b42      	ldr	r3, [pc, #264]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30008090:	eb04 1040 	add.w	r0, r4, r0, lsl #5
30008094:	5962      	ldr	r2, [r4, r5]
30008096:	4e4a      	ldr	r6, [pc, #296]	; (300081c0 <DDR_PHY_DLL_CLK_DIV+0x1f4>)
30008098:	611a      	str	r2, [r3, #16]
3000809a:	6842      	ldr	r2, [r0, #4]
3000809c:	615a      	str	r2, [r3, #20]
3000809e:	6882      	ldr	r2, [r0, #8]
300080a0:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
300080a4:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
300080a8:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
300080ac:	609a      	str	r2, [r3, #8]
300080ae:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
300080b2:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
300080b6:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
300080ba:	7830      	ldrb	r0, [r6, #0]
300080bc:	2800      	cmp	r0, #0
300080be:	d056      	beq.n	3000816e <DDR_PHY_DLL_CLK_DIV+0x1a2>
300080c0:	2801      	cmp	r0, #1
300080c2:	442c      	add	r4, r5
300080c4:	4b34      	ldr	r3, [pc, #208]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300080c6:	d037      	beq.n	30008138 <DDR_PHY_DLL_CLK_DIV+0x16c>
300080c8:	2222      	movs	r2, #34	; 0x22
300080ca:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300080ce:	6962      	ldr	r2, [r4, #20]
300080d0:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
300080d4:	e9d4 1206 	ldrd	r1, r2, [r4, #24]
300080d8:	f8c3 1084 	str.w	r1, [r3, #132]	; 0x84
300080dc:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
300080e0:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300080e4:	4937      	ldr	r1, [pc, #220]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
300080e6:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
300080ea:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300080ee:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
300080f2:	009b      	lsls	r3, r3, #2
300080f4:	d516      	bpl.n	30008124 <DDR_PHY_DLL_CLK_DIV+0x158>
300080f6:	2800      	cmp	r0, #0
300080f8:	d044      	beq.n	30008184 <DDR_PHY_DLL_CLK_DIV+0x1b8>
300080fa:	4b27      	ldr	r3, [pc, #156]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300080fc:	4a32      	ldr	r2, [pc, #200]	; (300081c8 <DDR_PHY_DLL_CLK_DIV+0x1fc>)
300080fe:	601a      	str	r2, [r3, #0]
30008100:	4b25      	ldr	r3, [pc, #148]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
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
30008122:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30008124:	b920      	cbnz	r0, 30008130 <DDR_PHY_DLL_CLK_DIV+0x164>
30008126:	4b29      	ldr	r3, [pc, #164]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x200>)
30008128:	4798      	blx	r3
3000812a:	2801      	cmp	r0, #1
3000812c:	7030      	strb	r0, [r6, #0]
3000812e:	d025      	beq.n	3000817c <DDR_PHY_DLL_CLK_DIV+0x1b0>
30008130:	4b19      	ldr	r3, [pc, #100]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30008132:	4a27      	ldr	r2, [pc, #156]	; (300081d0 <DDR_PHY_DLL_CLK_DIV+0x204>)
30008134:	601a      	str	r2, [r3, #0]
30008136:	e7e3      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0x134>
30008138:	2233      	movs	r2, #51	; 0x33
3000813a:	6965      	ldr	r5, [r4, #20]
3000813c:	4921      	ldr	r1, [pc, #132]	; (300081c4 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
3000813e:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
30008142:	f8c3 5080 	str.w	r5, [r3, #128]	; 0x80
30008146:	e9d4 0206 	ldrd	r0, r2, [r4, #24]
3000814a:	f8c3 0084 	str.w	r0, [r3, #132]	; 0x84
3000814e:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
30008152:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008156:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
3000815a:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000815e:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
30008162:	009a      	lsls	r2, r3, #2
30008164:	d50a      	bpl.n	3000817c <DDR_PHY_DLL_CLK_DIV+0x1b0>
30008166:	4b0c      	ldr	r3, [pc, #48]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30008168:	4a1a      	ldr	r2, [pc, #104]	; (300081d4 <DDR_PHY_DLL_CLK_DIV+0x208>)
3000816a:	601a      	str	r2, [r3, #0]
3000816c:	e7c8      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0x134>
3000816e:	4b17      	ldr	r3, [pc, #92]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x200>)
30008170:	4798      	blx	r3
30008172:	7030      	strb	r0, [r6, #0]
30008174:	e7a4      	b.n	300080c0 <DDR_PHY_DLL_CLK_DIV+0xf4>
30008176:	f7ff fec5 	bl	30007f04 <DDR_PHY_ChipInfo.part.0>
3000817a:	e786      	b.n	3000808a <DDR_PHY_DLL_CLK_DIV+0xbe>
3000817c:	4b06      	ldr	r3, [pc, #24]	; (30008198 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000817e:	4a16      	ldr	r2, [pc, #88]	; (300081d8 <DDR_PHY_DLL_CLK_DIV+0x20c>)
30008180:	601a      	str	r2, [r3, #0]
30008182:	e7bd      	b.n	30008100 <DDR_PHY_DLL_CLK_DIV+0x134>
30008184:	4b11      	ldr	r3, [pc, #68]	; (300081cc <DDR_PHY_DLL_CLK_DIV+0x200>)
30008186:	4798      	blx	r3
30008188:	2801      	cmp	r0, #1
3000818a:	7030      	strb	r0, [r6, #0]
3000818c:	d1b5      	bne.n	300080fa <DDR_PHY_DLL_CLK_DIV+0x12e>
3000818e:	e7ea      	b.n	30008166 <DDR_PHY_DLL_CLK_DIV+0x19a>
30008190:	cccccccd 	.word	0xcccccccd
30008194:	001fff88 	.word	0x001fff88
30008198:	41011000 	.word	0x41011000
3000819c:	10624dd3 	.word	0x10624dd3
300081a0:	07ff0000 	.word	0x07ff0000
300081a4:	f800ffff 	.word	0xf800ffff
300081a8:	51eb851f 	.word	0x51eb851f
300081ac:	fffff800 	.word	0xfffff800
300081b0:	d1b71759 	.word	0xd1b71759
300081b4:	fff80000 	.word	0xfff80000
300081b8:	3000efd1 	.word	0x3000efd1
300081bc:	3000d360 	.word	0x3000d360
300081c0:	3000fcf0 	.word	0x3000fcf0
300081c4:	42008000 	.word	0x42008000
300081c8:	4c000821 	.word	0x4c000821
300081cc:	30007dd9 	.word	0x30007dd9
300081d0:	40000021 	.word	0x40000021
300081d4:	4c000903 	.word	0x4c000903
300081d8:	40000103 	.word	0x40000103

300081dc <DDR_PHY_CRT_Init>:
300081dc:	4a32      	ldr	r2, [pc, #200]	; (300082a8 <DDR_PHY_CRT_Init+0xcc>)
300081de:	2005      	movs	r0, #5
300081e0:	b538      	push	{r3, r4, r5, lr}
300081e2:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
300081e6:	4c31      	ldr	r4, [pc, #196]	; (300082ac <DDR_PHY_CRT_Init+0xd0>)
300081e8:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300081ec:	4d30      	ldr	r5, [pc, #192]	; (300082b0 <DDR_PHY_CRT_Init+0xd4>)
300081ee:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
300081f2:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081f6:	f043 0301 	orr.w	r3, r3, #1
300081fa:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081fe:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008202:	f043 0330 	orr.w	r3, r3, #48	; 0x30
30008206:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000820a:	68e3      	ldr	r3, [r4, #12]
3000820c:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
30008210:	60e3      	str	r3, [r4, #12]
30008212:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008216:	f043 0302 	orr.w	r3, r3, #2
3000821a:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000821e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
30008222:	f023 0308 	bic.w	r3, r3, #8
30008226:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
3000822a:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000822e:	f043 0304 	orr.w	r3, r3, #4
30008232:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008236:	47a8      	blx	r5
30008238:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
3000823c:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30008240:	f043 0308 	orr.w	r3, r3, #8
30008244:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
30008248:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000824a:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
3000824e:	62a3      	str	r3, [r4, #40]	; 0x28
30008250:	47a8      	blx	r5
30008252:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008254:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008258:	4916      	ldr	r1, [pc, #88]	; (300082b4 <DDR_PHY_CRT_Init+0xd8>)
3000825a:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
3000825e:	62a2      	str	r2, [r4, #40]	; 0x28
30008260:	69e2      	ldr	r2, [r4, #28]
30008262:	f042 0202 	orr.w	r2, r2, #2
30008266:	61e2      	str	r2, [r4, #28]
30008268:	69e2      	ldr	r2, [r4, #28]
3000826a:	f042 0208 	orr.w	r2, r2, #8
3000826e:	61e2      	str	r2, [r4, #28]
30008270:	68a2      	ldr	r2, [r4, #8]
30008272:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008276:	60a2      	str	r2, [r4, #8]
30008278:	6862      	ldr	r2, [r4, #4]
3000827a:	4313      	orrs	r3, r2
3000827c:	6063      	str	r3, [r4, #4]
3000827e:	68a3      	ldr	r3, [r4, #8]
30008280:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008284:	60a3      	str	r3, [r4, #8]
30008286:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
3000828a:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000828e:	f023 0303 	bic.w	r3, r3, #3
30008292:	f043 0302 	orr.w	r3, r3, #2
30008296:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
3000829a:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000829e:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
300082a2:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300082a6:	bd38      	pop	{r3, r4, r5, pc}
300082a8:	42008000 	.word	0x42008000
300082ac:	41011000 	.word	0x41011000
300082b0:	00009b2d 	.word	0x00009b2d
300082b4:	90161f00 	.word	0x90161f00

300082b8 <DDR_PHY_R240_ZQ_CAL>:
300082b8:	b538      	push	{r3, r4, r5, lr}
300082ba:	4b1e      	ldr	r3, [pc, #120]	; (30008334 <DDR_PHY_R240_ZQ_CAL+0x7c>)
300082bc:	7818      	ldrb	r0, [r3, #0]
300082be:	28ff      	cmp	r0, #255	; 0xff
300082c0:	d034      	beq.n	3000832c <DDR_PHY_R240_ZQ_CAL+0x74>
300082c2:	4a1d      	ldr	r2, [pc, #116]	; (30008338 <DDR_PHY_R240_ZQ_CAL+0x80>)
300082c4:	eb00 0080 	add.w	r0, r0, r0, lsl #2
300082c8:	4b1c      	ldr	r3, [pc, #112]	; (3000833c <DDR_PHY_R240_ZQ_CAL+0x84>)
300082ca:	2400      	movs	r4, #0
300082cc:	eb02 0180 	add.w	r1, r2, r0, lsl #2
300082d0:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
300082d4:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
300082d8:	7c0a      	ldrb	r2, [r1, #16]
300082da:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
300082de:	0052      	lsls	r2, r2, #1
300082e0:	f002 023e 	and.w	r2, r2, #62	; 0x3e
300082e4:	4302      	orrs	r2, r0
300082e6:	6848      	ldr	r0, [r1, #4]
300082e8:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
300082ec:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
300082f0:	4a13      	ldr	r2, [pc, #76]	; (30008340 <DDR_PHY_R240_ZQ_CAL+0x88>)
300082f2:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300082f6:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300082fa:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300082fe:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
30008302:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
30008306:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
3000830a:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
3000830e:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
30008312:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008316:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
3000831a:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000831e:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
30008322:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
30008326:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
3000832a:	bd38      	pop	{r3, r4, r5, pc}
3000832c:	f7ff fdea 	bl	30007f04 <DDR_PHY_ChipInfo.part.0>
30008330:	e7c7      	b.n	300082c2 <DDR_PHY_R240_ZQ_CAL+0xa>
30008332:	bf00      	nop
30008334:	3000efd1 	.word	0x3000efd1
30008338:	3000d4a0 	.word	0x3000d4a0
3000833c:	41011000 	.word	0x41011000
30008340:	00262626 	.word	0x00262626

30008344 <DDR_PHY_ZQ_SET_SEL>:
30008344:	b410      	push	{r4}
30008346:	4b25      	ldr	r3, [pc, #148]	; (300083dc <DDR_PHY_ZQ_SET_SEL+0x98>)
30008348:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
3000834c:	4a24      	ldr	r2, [pc, #144]	; (300083e0 <DDR_PHY_ZQ_SET_SEL+0x9c>)
3000834e:	4c25      	ldr	r4, [pc, #148]	; (300083e4 <DDR_PHY_ZQ_SET_SEL+0xa0>)
30008350:	4925      	ldr	r1, [pc, #148]	; (300083e8 <DDR_PHY_ZQ_SET_SEL+0xa4>)
30008352:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
30008356:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
3000835a:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
3000835e:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
30008362:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
30008366:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
3000836a:	f64f 74ff 	movw	r4, #65535	; 0xffff
3000836e:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
30008372:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
30008376:	2200      	movs	r2, #0
30008378:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
3000837c:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
30008380:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008384:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
30008388:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000838c:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
30008390:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008394:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
30008398:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
3000839c:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
300083a0:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
300083a4:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
300083a8:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
300083ac:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
300083b0:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
300083b4:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
300083b8:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
300083bc:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
300083c0:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
300083c4:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
300083c8:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
300083cc:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
300083d0:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
300083d4:	f85d 4b04 	ldr.w	r4, [sp], #4
300083d8:	4770      	bx	lr
300083da:	bf00      	nop
300083dc:	41011000 	.word	0x41011000
300083e0:	ffff1111 	.word	0xffff1111
300083e4:	ffff3333 	.word	0xffff3333
300083e8:	ffff0000 	.word	0xffff0000

300083ec <DDR_PHY_DELAY_TAP_SET>:
300083ec:	b510      	push	{r4, lr}
300083ee:	4b15      	ldr	r3, [pc, #84]	; (30008444 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083f0:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300083f4:	4914      	ldr	r1, [pc, #80]	; (30008448 <DDR_PHY_DELAY_TAP_SET+0x5c>)
300083f6:	4c15      	ldr	r4, [pc, #84]	; (3000844c <DDR_PHY_DELAY_TAP_SET+0x60>)
300083f8:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300083fc:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
30008400:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30008404:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
30008408:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
3000840c:	7820      	ldrb	r0, [r4, #0]
3000840e:	b1a0      	cbz	r0, 3000843a <DDR_PHY_DELAY_TAP_SET+0x4e>
30008410:	2801      	cmp	r0, #1
30008412:	4b0c      	ldr	r3, [pc, #48]	; (30008444 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008414:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
30008418:	bf0c      	ite	eq
3000841a:	4a0d      	ldreq	r2, [pc, #52]	; (30008450 <DDR_PHY_DELAY_TAP_SET+0x64>)
3000841c:	4a0d      	ldrne	r2, [pc, #52]	; (30008454 <DDR_PHY_DELAY_TAP_SET+0x68>)
3000841e:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
30008422:	f640 0288 	movw	r2, #2184	; 0x888
30008426:	4b07      	ldr	r3, [pc, #28]	; (30008444 <DDR_PHY_DELAY_TAP_SET+0x58>)
30008428:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
3000842c:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
30008430:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
30008434:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
30008438:	bd10      	pop	{r4, pc}
3000843a:	4b07      	ldr	r3, [pc, #28]	; (30008458 <DDR_PHY_DELAY_TAP_SET+0x6c>)
3000843c:	4798      	blx	r3
3000843e:	7020      	strb	r0, [r4, #0]
30008440:	e7e6      	b.n	30008410 <DDR_PHY_DELAY_TAP_SET+0x24>
30008442:	bf00      	nop
30008444:	41011000 	.word	0x41011000
30008448:	20888888 	.word	0x20888888
3000844c:	3000fcf0 	.word	0x3000fcf0
30008450:	7c033333 	.word	0x7c033333
30008454:	7c011111 	.word	0x7c011111
30008458:	30007dd9 	.word	0x30007dd9

3000845c <DDR_PHY_READ_CTRL>:
3000845c:	4b10      	ldr	r3, [pc, #64]	; (300084a0 <DDR_PHY_READ_CTRL+0x44>)
3000845e:	b510      	push	{r4, lr}
30008460:	7818      	ldrb	r0, [r3, #0]
30008462:	28ff      	cmp	r0, #255	; 0xff
30008464:	d019      	beq.n	3000849a <DDR_PHY_READ_CTRL+0x3e>
30008466:	4a0f      	ldr	r2, [pc, #60]	; (300084a4 <DDR_PHY_READ_CTRL+0x48>)
30008468:	eb00 0080 	add.w	r0, r0, r0, lsl #2
3000846c:	4b0e      	ldr	r3, [pc, #56]	; (300084a8 <DDR_PHY_READ_CTRL+0x4c>)
3000846e:	2108      	movs	r1, #8
30008470:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30008474:	22fe      	movs	r2, #254	; 0xfe
30008476:	7c44      	ldrb	r4, [r0, #17]
30008478:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
3000847c:	7c84      	ldrb	r4, [r0, #18]
3000847e:	7cc0      	ldrb	r0, [r0, #19]
30008480:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30008484:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
30008488:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
3000848c:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
30008490:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008494:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
30008498:	bd10      	pop	{r4, pc}
3000849a:	f7ff fd33 	bl	30007f04 <DDR_PHY_ChipInfo.part.0>
3000849e:	e7e2      	b.n	30008466 <DDR_PHY_READ_CTRL+0xa>
300084a0:	3000efd1 	.word	0x3000efd1
300084a4:	3000d4a0 	.word	0x3000d4a0
300084a8:	41011000 	.word	0x41011000

300084ac <DDR_PHY_READ_LEVELING>:
300084ac:	b508      	push	{r3, lr}
300084ae:	4b21      	ldr	r3, [pc, #132]	; (30008534 <DDR_PHY_READ_LEVELING+0x88>)
300084b0:	7818      	ldrb	r0, [r3, #0]
300084b2:	28ff      	cmp	r0, #255	; 0xff
300084b4:	d03b      	beq.n	3000852e <DDR_PHY_READ_LEVELING+0x82>
300084b6:	4b20      	ldr	r3, [pc, #128]	; (30008538 <DDR_PHY_READ_LEVELING+0x8c>)
300084b8:	b1e8      	cbz	r0, 300084f6 <DDR_PHY_READ_LEVELING+0x4a>
300084ba:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
300084be:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300084c2:	220f      	movs	r2, #15
300084c4:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
300084c8:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
300084cc:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
300084d0:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
300084d4:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
300084d8:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
300084dc:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
300084e0:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
300084e4:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
300084e8:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
300084ec:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
300084f0:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300084f4:	bd08      	pop	{r3, pc}
300084f6:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300084fa:	2110      	movs	r1, #16
300084fc:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
30008500:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
30008504:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
30008508:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
3000850c:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
30008510:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
30008514:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
30008518:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
3000851c:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
30008520:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
30008524:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
30008528:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
3000852c:	bd08      	pop	{r3, pc}
3000852e:	f7ff fce9 	bl	30007f04 <DDR_PHY_ChipInfo.part.0>
30008532:	e7c0      	b.n	300084b6 <DDR_PHY_READ_LEVELING+0xa>
30008534:	3000efd1 	.word	0x3000efd1
30008538:	41011000 	.word	0x41011000

3000853c <DDR_PHY_Init>:
3000853c:	b510      	push	{r4, lr}
3000853e:	6d80      	ldr	r0, [r0, #88]	; 0x58
30008540:	4c2f      	ldr	r4, [pc, #188]	; (30008600 <DDR_PHY_Init+0xc4>)
30008542:	f7ff fd43 	bl	30007fcc <DDR_PHY_DLL_CLK_DIV>
30008546:	f7ff fe49 	bl	300081dc <DDR_PHY_CRT_Init>
3000854a:	f7ff feb5 	bl	300082b8 <DDR_PHY_R240_ZQ_CAL>
3000854e:	f7ff fef9 	bl	30008344 <DDR_PHY_ZQ_SET_SEL>
30008552:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30008556:	2300      	movs	r3, #0
30008558:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000855c:	4829      	ldr	r0, [pc, #164]	; (30008604 <DDR_PHY_Init+0xc8>)
3000855e:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
30008562:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30008566:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
3000856a:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
3000856e:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
30008572:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30008576:	f7ff ff39 	bl	300083ec <DDR_PHY_DELAY_TAP_SET>
3000857a:	f7ff ff6f 	bl	3000845c <DDR_PHY_READ_CTRL>
3000857e:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
30008582:	4a21      	ldr	r2, [pc, #132]	; (30008608 <DDR_PHY_Init+0xcc>)
30008584:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30008588:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
3000858c:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
30008590:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008594:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008598:	f7ff ff88 	bl	300084ac <DDR_PHY_READ_LEVELING>
3000859c:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
300085a0:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
300085a4:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
300085a8:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
300085ac:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
300085b0:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
300085b4:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
300085b8:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
300085bc:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
300085c0:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
300085c4:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
300085c8:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085cc:	f043 0303 	orr.w	r3, r3, #3
300085d0:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085d4:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
300085d8:	f043 030c 	orr.w	r3, r3, #12
300085dc:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
300085e0:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085e4:	f023 0303 	bic.w	r3, r3, #3
300085e8:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085ec:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
300085f0:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300085f4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300085f8:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085fc:	bd10      	pop	{r4, pc}
300085fe:	bf00      	nop
30008600:	41011000 	.word	0x41011000
30008604:	00045500 	.word	0x00045500
30008608:	11117777 	.word	0x11117777

3000860c <DDR_PHY_CAL>:
3000860c:	4770      	bx	lr
3000860e:	bf00      	nop

30008610 <DDR_PHY_AutoGating>:
30008610:	4a0c      	ldr	r2, [pc, #48]	; (30008644 <DDR_PHY_AutoGating+0x34>)
30008612:	2032      	movs	r0, #50	; 0x32
30008614:	490c      	ldr	r1, [pc, #48]	; (30008648 <DDR_PHY_AutoGating+0x38>)
30008616:	b508      	push	{r3, lr}
30008618:	6893      	ldr	r3, [r2, #8]
3000861a:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
3000861e:	6093      	str	r3, [r2, #8]
30008620:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
30008624:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
30008628:	f043 0314 	orr.w	r3, r3, #20
3000862c:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
30008630:	4788      	blx	r1
30008632:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30008636:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
3000863a:	f043 0303 	orr.w	r3, r3, #3
3000863e:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30008642:	bd08      	pop	{r3, pc}
30008644:	41011000 	.word	0x41011000
30008648:	00009b2d 	.word	0x00009b2d

3000864c <flash_init_userdef>:
3000864c:	4b14      	ldr	r3, [pc, #80]	; (300086a0 <flash_init_userdef+0x54>)
3000864e:	2201      	movs	r2, #1
30008650:	2140      	movs	r1, #64	; 0x40
30008652:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008656:	b430      	push	{r4, r5}
30008658:	e9c3 1203 	strd	r1, r2, [r3, #12]
3000865c:	2502      	movs	r5, #2
3000865e:	2400      	movs	r4, #0
30008660:	f640 0104 	movw	r1, #2052	; 0x804
30008664:	601a      	str	r2, [r3, #0]
30008666:	22bb      	movs	r2, #187	; 0xbb
30008668:	83d8      	strh	r0, [r3, #30]
3000866a:	8419      	strh	r1, [r3, #32]
3000866c:	203b      	movs	r0, #59	; 0x3b
3000866e:	21eb      	movs	r1, #235	; 0xeb
30008670:	629a      	str	r2, [r3, #40]	; 0x28
30008672:	4a0c      	ldr	r2, [pc, #48]	; (300086a4 <flash_init_userdef+0x58>)
30008674:	6258      	str	r0, [r3, #36]	; 0x24
30008676:	6319      	str	r1, [r3, #48]	; 0x30
30008678:	f64a 3120 	movw	r1, #43808	; 0xab20
3000867c:	655a      	str	r2, [r3, #84]	; 0x54
3000867e:	22b9      	movs	r2, #185	; 0xb9
30008680:	4809      	ldr	r0, [pc, #36]	; (300086a8 <flash_init_userdef+0x5c>)
30008682:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008686:	6598      	str	r0, [r3, #88]	; 0x58
30008688:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
3000868c:	e9c3 5405 	strd	r5, r4, [r3, #20]
30008690:	2406      	movs	r4, #6
30008692:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008696:	246b      	movs	r4, #107	; 0x6b
30008698:	62dc      	str	r4, [r3, #44]	; 0x2c
3000869a:	bc30      	pop	{r4, r5}
3000869c:	4770      	bx	lr
3000869e:	bf00      	nop
300086a0:	2001c01c 	.word	0x2001c01c
300086a4:	00059f06 	.word	0x00059f06
300086a8:	d8600001 	.word	0xd8600001

300086ac <nand_init_userdef>:
300086ac:	4b18      	ldr	r3, [pc, #96]	; (30008710 <nand_init_userdef+0x64>)
300086ae:	2001      	movs	r0, #1
300086b0:	2200      	movs	r2, #0
300086b2:	2102      	movs	r1, #2
300086b4:	b410      	push	{r4}
300086b6:	6018      	str	r0, [r3, #0]
300086b8:	f640 0408 	movw	r4, #2056	; 0x808
300086bc:	6159      	str	r1, [r3, #20]
300086be:	83dc      	strh	r4, [r3, #30]
300086c0:	f44f 6400 	mov.w	r4, #2048	; 0x800
300086c4:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
300086c8:	841c      	strh	r4, [r3, #32]
300086ca:	246b      	movs	r4, #107	; 0x6b
300086cc:	619a      	str	r2, [r3, #24]
300086ce:	62dc      	str	r4, [r3, #44]	; 0x2c
300086d0:	4c10      	ldr	r4, [pc, #64]	; (30008714 <nand_init_userdef+0x68>)
300086d2:	775a      	strb	r2, [r3, #29]
300086d4:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
300086d8:	629a      	str	r2, [r3, #40]	; 0x28
300086da:	631a      	str	r2, [r3, #48]	; 0x30
300086dc:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
300086e0:	65da      	str	r2, [r3, #92]	; 0x5c
300086e2:	e9c3 0003 	strd	r0, r0, [r3, #12]
300086e6:	203b      	movs	r0, #59	; 0x3b
300086e8:	6258      	str	r0, [r3, #36]	; 0x24
300086ea:	200b      	movs	r0, #11
300086ec:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
300086f0:	6698      	str	r0, [r3, #104]	; 0x68
300086f2:	2008      	movs	r0, #8
300086f4:	4908      	ldr	r1, [pc, #32]	; (30008718 <nand_init_userdef+0x6c>)
300086f6:	64d8      	str	r0, [r3, #76]	; 0x4c
300086f8:	f241 0013 	movw	r0, #4115	; 0x1013
300086fc:	6599      	str	r1, [r3, #88]	; 0x58
300086fe:	4907      	ldr	r1, [pc, #28]	; (3000871c <nand_init_userdef+0x70>)
30008700:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
30008704:	6719      	str	r1, [r3, #112]	; 0x70
30008706:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
3000870a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000870e:	4770      	bx	lr
30008710:	2001c01c 	.word	0x2001c01c
30008714:	000f9f06 	.word	0x000f9f06
30008718:	d800001f 	.word	0xd800001f
3000871c:	10300804 	.word	0x10300804

30008720 <flash_get_layout_info>:
30008720:	b570      	push	{r4, r5, r6, lr}
30008722:	4b0f      	ldr	r3, [pc, #60]	; (30008760 <flash_get_layout_info+0x40>)
30008724:	4615      	mov	r5, r2
30008726:	4604      	mov	r4, r0
30008728:	460e      	mov	r6, r1
3000872a:	4798      	blx	r3
3000872c:	4b0d      	ldr	r3, [pc, #52]	; (30008764 <flash_get_layout_info+0x44>)
3000872e:	4a0e      	ldr	r2, [pc, #56]	; (30008768 <flash_get_layout_info+0x48>)
30008730:	2800      	cmp	r0, #0
30008732:	bf14      	ite	ne
30008734:	469c      	movne	ip, r3
30008736:	4694      	moveq	ip, r2
30008738:	f8dc 3000 	ldr.w	r3, [ip]
3000873c:	2bff      	cmp	r3, #255	; 0xff
3000873e:	d104      	bne.n	3000874a <flash_get_layout_info+0x2a>
30008740:	e00d      	b.n	3000875e <flash_get_layout_info+0x3e>
30008742:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
30008746:	2bff      	cmp	r3, #255	; 0xff
30008748:	d009      	beq.n	3000875e <flash_get_layout_info+0x3e>
3000874a:	429c      	cmp	r4, r3
3000874c:	d1f9      	bne.n	30008742 <flash_get_layout_info+0x22>
3000874e:	b116      	cbz	r6, 30008756 <flash_get_layout_info+0x36>
30008750:	f8dc 3004 	ldr.w	r3, [ip, #4]
30008754:	6033      	str	r3, [r6, #0]
30008756:	b115      	cbz	r5, 3000875e <flash_get_layout_info+0x3e>
30008758:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000875c:	602b      	str	r3, [r5, #0]
3000875e:	bd70      	pop	{r4, r5, r6, pc}
30008760:	30009c15 	.word	0x30009c15
30008764:	3000f0ac 	.word	0x3000f0ac
30008768:	3000efd4 	.word	0x3000efd4

3000876c <NAND_CHECK_IS_BAD_BLOCK>:
3000876c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30008770:	4e24      	ldr	r6, [pc, #144]	; (30008804 <NAND_CHECK_IS_BAD_BLOCK+0x98>)
30008772:	0907      	lsrs	r7, r0, #4
30008774:	0045      	lsls	r5, r0, #1
30008776:	b085      	sub	sp, #20
30008778:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
3000877c:	f005 051e 	and.w	r5, r5, #30
30008780:	40eb      	lsrs	r3, r5
30008782:	f003 0303 	and.w	r3, r3, #3
30008786:	2b02      	cmp	r3, #2
30008788:	d02c      	beq.n	300087e4 <NAND_CHECK_IS_BAD_BLOCK+0x78>
3000878a:	2b01      	cmp	r3, #1
3000878c:	d026      	beq.n	300087dc <NAND_CHECK_IS_BAD_BLOCK+0x70>
3000878e:	4b1e      	ldr	r3, [pc, #120]	; (30008808 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
30008790:	f04f 0801 	mov.w	r8, #1
30008794:	2204      	movs	r2, #4
30008796:	4604      	mov	r4, r0
30008798:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
3000879c:	0180      	lsls	r0, r0, #6
3000879e:	ab03      	add	r3, sp, #12
300087a0:	f8df 9070 	ldr.w	r9, [pc, #112]	; 30008814 <NAND_CHECK_IS_BAD_BLOCK+0xa8>
300087a4:	fa08 f101 	lsl.w	r1, r8, r1
300087a8:	47c8      	blx	r9
300087aa:	f89d 300c 	ldrb.w	r3, [sp, #12]
300087ae:	f89d 200d 	ldrb.w	r2, [sp, #13]
300087b2:	4013      	ands	r3, r2
300087b4:	2bff      	cmp	r3, #255	; 0xff
300087b6:	d019      	beq.n	300087ec <NAND_CHECK_IS_BAD_BLOCK+0x80>
300087b8:	2002      	movs	r0, #2
300087ba:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
300087be:	4b13      	ldr	r3, [pc, #76]	; (3000880c <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
300087c0:	fa00 f505 	lsl.w	r5, r0, r5
300087c4:	9400      	str	r4, [sp, #0]
300087c6:	4619      	mov	r1, r3
300087c8:	432a      	orrs	r2, r5
300087ca:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
300087ce:	4a10      	ldr	r2, [pc, #64]	; (30008810 <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
300087d0:	f003 fbbc 	bl	3000bf4c <rtk_log_write>
300087d4:	4640      	mov	r0, r8
300087d6:	b005      	add	sp, #20
300087d8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087dc:	2000      	movs	r0, #0
300087de:	b005      	add	sp, #20
300087e0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087e4:	2001      	movs	r0, #1
300087e6:	b005      	add	sp, #20
300087e8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087ec:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
300087f0:	fa08 f005 	lsl.w	r0, r8, r5
300087f4:	4303      	orrs	r3, r0
300087f6:	2000      	movs	r0, #0
300087f8:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
300087fc:	b005      	add	sp, #20
300087fe:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008802:	bf00      	nop
30008804:	3000fcf8 	.word	0x3000fcf8
30008808:	2001c01c 	.word	0x2001c01c
3000880c:	3000d734 	.word	0x3000d734
30008810:	3000d740 	.word	0x3000d740
30008814:	30009b65 	.word	0x30009b65

30008818 <Nand_Get_NandAddr>:
30008818:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
3000881c:	4f10      	ldr	r7, [pc, #64]	; (30008860 <Nand_Get_NandAddr+0x48>)
3000881e:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
30008822:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30008826:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
3000882a:	40e8      	lsrs	r0, r5
3000882c:	fa26 f505 	lsr.w	r5, r6, r5
30008830:	09ad      	lsrs	r5, r5, #6
30008832:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
30008836:	d90f      	bls.n	30008858 <Nand_Get_NandAddr+0x40>
30008838:	0984      	lsrs	r4, r0, #6
3000883a:	f04f 0840 	mov.w	r8, #64	; 0x40
3000883e:	4620      	mov	r0, r4
30008840:	3401      	adds	r4, #1
30008842:	f7ff ff93 	bl	3000876c <NAND_CHECK_IS_BAD_BLOCK>
30008846:	b128      	cbz	r0, 30008854 <Nand_Get_NandAddr+0x3c>
30008848:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
3000884c:	3501      	adds	r5, #1
3000884e:	fa08 f303 	lsl.w	r3, r8, r3
30008852:	441e      	add	r6, r3
30008854:	42a5      	cmp	r5, r4
30008856:	d8f2      	bhi.n	3000883e <Nand_Get_NandAddr+0x26>
30008858:	4630      	mov	r0, r6
3000885a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000885e:	bf00      	nop
30008860:	2001c01c 	.word	0x2001c01c

30008864 <Nand_L2P_Table>:
30008864:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008868:	b082      	sub	sp, #8
3000886a:	4604      	mov	r4, r0
3000886c:	2002      	movs	r0, #2
3000886e:	aa01      	add	r2, sp, #4
30008870:	4669      	mov	r1, sp
30008872:	f7ff ff55 	bl	30008720 <flash_get_layout_info>
30008876:	9800      	ldr	r0, [sp, #0]
30008878:	42a0      	cmp	r0, r4
3000887a:	d810      	bhi.n	3000889e <Nand_L2P_Table+0x3a>
3000887c:	9b01      	ldr	r3, [sp, #4]
3000887e:	42a3      	cmp	r3, r4
30008880:	d805      	bhi.n	3000888e <Nand_L2P_Table+0x2a>
30008882:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008886:	4620      	mov	r0, r4
30008888:	b002      	add	sp, #8
3000888a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000888e:	4621      	mov	r1, r4
30008890:	f7ff ffc2 	bl	30008818 <Nand_Get_NandAddr>
30008894:	4604      	mov	r4, r0
30008896:	4620      	mov	r0, r4
30008898:	b002      	add	sp, #8
3000889a:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000889e:	4f0e      	ldr	r7, [pc, #56]	; (300088d8 <Nand_L2P_Table+0x74>)
300088a0:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
300088a4:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
300088a8:	fa24 f606 	lsr.w	r6, r4, r6
300088ac:	09b6      	lsrs	r6, r6, #6
300088ae:	d0ea      	beq.n	30008886 <Nand_L2P_Table+0x22>
300088b0:	2500      	movs	r5, #0
300088b2:	f04f 0840 	mov.w	r8, #64	; 0x40
300088b6:	4628      	mov	r0, r5
300088b8:	3501      	adds	r5, #1
300088ba:	f7ff ff57 	bl	3000876c <NAND_CHECK_IS_BAD_BLOCK>
300088be:	b128      	cbz	r0, 300088cc <Nand_L2P_Table+0x68>
300088c0:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
300088c4:	3601      	adds	r6, #1
300088c6:	fa08 f303 	lsl.w	r3, r8, r3
300088ca:	441c      	add	r4, r3
300088cc:	42b5      	cmp	r5, r6
300088ce:	d3f2      	bcc.n	300088b6 <Nand_L2P_Table+0x52>
300088d0:	4620      	mov	r0, r4
300088d2:	b002      	add	sp, #8
300088d4:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300088d8:	2001c01c 	.word	0x2001c01c

300088dc <NandImgCopy>:
300088dc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300088e0:	4617      	mov	r7, r2
300088e2:	4603      	mov	r3, r0
300088e4:	b089      	sub	sp, #36	; 0x24
300088e6:	f8df 9100 	ldr.w	r9, [pc, #256]	; 300089e8 <NandImgCopy+0x10c>
300088ea:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
300088ee:	4608      	mov	r0, r1
300088f0:	9307      	str	r3, [sp, #28]
300088f2:	f7ff ffb7 	bl	30008864 <Nand_L2P_Table>
300088f6:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
300088fa:	4480      	add	r8, r0
300088fc:	fa20 f406 	lsr.w	r4, r0, r6
30008900:	fa28 f806 	lsr.w	r8, r8, r6
30008904:	eba8 0804 	sub.w	r8, r8, r4
30008908:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
3000890c:	d054      	beq.n	300089b8 <NandImgCopy+0xdc>
3000890e:	2300      	movs	r3, #0
30008910:	4605      	mov	r5, r0
30008912:	f04f 0b40 	mov.w	fp, #64	; 0x40
30008916:	469a      	mov	sl, r3
30008918:	9306      	str	r3, [sp, #24]
3000891a:	463b      	mov	r3, r7
3000891c:	4647      	mov	r7, r8
3000891e:	4698      	mov	r8, r3
30008920:	e020      	b.n	30008964 <NandImgCopy+0x88>
30008922:	f1c2 0320 	rsb	r3, r2, #32
30008926:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000892a:	40d9      	lsrs	r1, r3
3000892c:	9b07      	ldr	r3, [sp, #28]
3000892e:	4029      	ands	r1, r5
30008930:	4453      	add	r3, sl
30008932:	b377      	cbz	r7, 30008992 <NandImgCopy+0xb6>
30008934:	2601      	movs	r6, #1
30008936:	4620      	mov	r0, r4
30008938:	9105      	str	r1, [sp, #20]
3000893a:	fa06 f202 	lsl.w	r2, r6, r2
3000893e:	4e27      	ldr	r6, [pc, #156]	; (300089dc <NandImgCopy+0x100>)
30008940:	9204      	str	r2, [sp, #16]
30008942:	1a52      	subs	r2, r2, r1
30008944:	4492      	add	sl, r2
30008946:	4415      	add	r5, r2
30008948:	47b0      	blx	r6
3000894a:	f010 0f70 	tst.w	r0, #112	; 0x70
3000894e:	9b04      	ldr	r3, [sp, #16]
30008950:	9905      	ldr	r1, [sp, #20]
30008952:	d134      	bne.n	300089be <NandImgCopy+0xe2>
30008954:	eba8 0803 	sub.w	r8, r8, r3
30008958:	4488      	add	r8, r1
3000895a:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
3000895e:	3f01      	subs	r7, #1
30008960:	fa25 f406 	lsr.w	r4, r5, r6
30008964:	9b06      	ldr	r3, [sp, #24]
30008966:	4632      	mov	r2, r6
30008968:	09a6      	lsrs	r6, r4, #6
3000896a:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
3000896e:	d8d8      	bhi.n	30008922 <NandImgCopy+0x46>
30008970:	e005      	b.n	3000897e <NandImgCopy+0xa2>
30008972:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008976:	3440      	adds	r4, #64	; 0x40
30008978:	fa0b f303 	lsl.w	r3, fp, r3
3000897c:	441d      	add	r5, r3
3000897e:	4630      	mov	r0, r6
30008980:	3601      	adds	r6, #1
30008982:	f7ff fef3 	bl	3000876c <NAND_CHECK_IS_BAD_BLOCK>
30008986:	2800      	cmp	r0, #0
30008988:	d1f3      	bne.n	30008972 <NandImgCopy+0x96>
3000898a:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
3000898e:	9606      	str	r6, [sp, #24]
30008990:	e7c7      	b.n	30008922 <NandImgCopy+0x46>
30008992:	4642      	mov	r2, r8
30008994:	4620      	mov	r0, r4
30008996:	4d11      	ldr	r5, [pc, #68]	; (300089dc <NandImgCopy+0x100>)
30008998:	47a8      	blx	r5
3000899a:	f010 0f70 	tst.w	r0, #112	; 0x70
3000899e:	d00b      	beq.n	300089b8 <NandImgCopy+0xdc>
300089a0:	09a1      	lsrs	r1, r4, #6
300089a2:	4b0f      	ldr	r3, [pc, #60]	; (300089e0 <NandImgCopy+0x104>)
300089a4:	f004 043f 	and.w	r4, r4, #63	; 0x3f
300089a8:	4a0e      	ldr	r2, [pc, #56]	; (300089e4 <NandImgCopy+0x108>)
300089aa:	e9cd 1001 	strd	r1, r0, [sp, #4]
300089ae:	9400      	str	r4, [sp, #0]
300089b0:	4619      	mov	r1, r3
300089b2:	2004      	movs	r0, #4
300089b4:	f003 faca 	bl	3000bf4c <rtk_log_write>
300089b8:	b009      	add	sp, #36	; 0x24
300089ba:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300089be:	1ac9      	subs	r1, r1, r3
300089c0:	09a3      	lsrs	r3, r4, #6
300089c2:	f004 043f 	and.w	r4, r4, #63	; 0x3f
300089c6:	9002      	str	r0, [sp, #8]
300089c8:	9301      	str	r3, [sp, #4]
300089ca:	4488      	add	r8, r1
300089cc:	4b04      	ldr	r3, [pc, #16]	; (300089e0 <NandImgCopy+0x104>)
300089ce:	2004      	movs	r0, #4
300089d0:	4a04      	ldr	r2, [pc, #16]	; (300089e4 <NandImgCopy+0x108>)
300089d2:	4619      	mov	r1, r3
300089d4:	9400      	str	r4, [sp, #0]
300089d6:	f003 fab9 	bl	3000bf4c <rtk_log_write>
300089da:	e7be      	b.n	3000895a <NandImgCopy+0x7e>
300089dc:	30009b65 	.word	0x30009b65
300089e0:	3000d734 	.word	0x3000d734
300089e4:	3000d758 	.word	0x3000d758
300089e8:	2001c01c 	.word	0x2001c01c

300089ec <OTP_Read8.part.0>:
300089ec:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
300089f0:	4b30      	ldr	r3, [pc, #192]	; (30008ab4 <OTP_Read8.part.0+0xc8>)
300089f2:	4680      	mov	r8, r0
300089f4:	e843 f300 	tt	r3, r3
300089f8:	f3c3 5380 	ubfx	r3, r3, #22, #1
300089fc:	4a2e      	ldr	r2, [pc, #184]	; (30008ab8 <OTP_Read8.part.0+0xcc>)
300089fe:	4e2f      	ldr	r6, [pc, #188]	; (30008abc <OTP_Read8.part.0+0xd0>)
30008a00:	b082      	sub	sp, #8
30008a02:	2b00      	cmp	r3, #0
30008a04:	4689      	mov	r9, r1
30008a06:	bf0e      	itee	eq
30008a08:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
30008a0c:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
30008a10:	4616      	movne	r6, r2
30008a12:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a14:	05d8      	lsls	r0, r3, #23
30008a16:	d505      	bpl.n	30008a24 <OTP_Read8.part.0+0x38>
30008a18:	4d29      	ldr	r5, [pc, #164]	; (30008ac0 <OTP_Read8.part.0+0xd4>)
30008a1a:	2064      	movs	r0, #100	; 0x64
30008a1c:	47a8      	blx	r5
30008a1e:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a20:	05d9      	lsls	r1, r3, #23
30008a22:	d4fa      	bmi.n	30008a1a <OTP_Read8.part.0+0x2e>
30008a24:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a26:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008a2a:	643b      	str	r3, [r7, #64]	; 0x40
30008a2c:	4b25      	ldr	r3, [pc, #148]	; (30008ac4 <OTP_Read8.part.0+0xd8>)
30008a2e:	681a      	ldr	r2, [r3, #0]
30008a30:	07d2      	lsls	r2, r2, #31
30008a32:	d403      	bmi.n	30008a3c <OTP_Read8.part.0+0x50>
30008a34:	681a      	ldr	r2, [r3, #0]
30008a36:	f042 0201 	orr.w	r2, r2, #1
30008a3a:	601a      	str	r2, [r3, #0]
30008a3c:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
30008a40:	ea4f 2308 	mov.w	r3, r8, lsl #8
30008a44:	694a      	ldr	r2, [r1, #20]
30008a46:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
30008a4a:	614a      	str	r2, [r1, #20]
30008a4c:	6033      	str	r3, [r6, #0]
30008a4e:	6833      	ldr	r3, [r6, #0]
30008a50:	2b00      	cmp	r3, #0
30008a52:	db10      	blt.n	30008a76 <OTP_Read8.part.0+0x8a>
30008a54:	2400      	movs	r4, #0
30008a56:	4d1a      	ldr	r5, [pc, #104]	; (30008ac0 <OTP_Read8.part.0+0xd4>)
30008a58:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008a5c:	e001      	b.n	30008a62 <OTP_Read8.part.0+0x76>
30008a5e:	4554      	cmp	r4, sl
30008a60:	d01a      	beq.n	30008a98 <OTP_Read8.part.0+0xac>
30008a62:	2005      	movs	r0, #5
30008a64:	3401      	adds	r4, #1
30008a66:	47a8      	blx	r5
30008a68:	6833      	ldr	r3, [r6, #0]
30008a6a:	2b00      	cmp	r3, #0
30008a6c:	daf7      	bge.n	30008a5e <OTP_Read8.part.0+0x72>
30008a6e:	f644 6320 	movw	r3, #20000	; 0x4e20
30008a72:	429c      	cmp	r4, r3
30008a74:	d010      	beq.n	30008a98 <OTP_Read8.part.0+0xac>
30008a76:	6833      	ldr	r3, [r6, #0]
30008a78:	2001      	movs	r0, #1
30008a7a:	f889 3000 	strb.w	r3, [r9]
30008a7e:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008a82:	6953      	ldr	r3, [r2, #20]
30008a84:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008a88:	6153      	str	r3, [r2, #20]
30008a8a:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a8c:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008a90:	643b      	str	r3, [r7, #64]	; 0x40
30008a92:	b002      	add	sp, #8
30008a94:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008a98:	22ff      	movs	r2, #255	; 0xff
30008a9a:	4b0b      	ldr	r3, [pc, #44]	; (30008ac8 <OTP_Read8.part.0+0xdc>)
30008a9c:	2002      	movs	r0, #2
30008a9e:	f8cd 8000 	str.w	r8, [sp]
30008aa2:	4619      	mov	r1, r3
30008aa4:	f889 2000 	strb.w	r2, [r9]
30008aa8:	4a08      	ldr	r2, [pc, #32]	; (30008acc <OTP_Read8.part.0+0xe0>)
30008aaa:	f003 fa4f 	bl	3000bf4c <rtk_log_write>
30008aae:	2000      	movs	r0, #0
30008ab0:	e7e5      	b.n	30008a7e <OTP_Read8.part.0+0x92>
30008ab2:	bf00      	nop
30008ab4:	0000e5f9 	.word	0x0000e5f9
30008ab8:	5200000c 	.word	0x5200000c
30008abc:	42000008 	.word	0x42000008
30008ac0:	00009b2d 	.word	0x00009b2d
30008ac4:	42008000 	.word	0x42008000
30008ac8:	3000d794 	.word	0x3000d794
30008acc:	3000d798 	.word	0x3000d798

30008ad0 <OTP_Read8>:
30008ad0:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008ad4:	d201      	bcs.n	30008ada <OTP_Read8+0xa>
30008ad6:	f7ff bf89 	b.w	300089ec <OTP_Read8.part.0>
30008ada:	22ff      	movs	r2, #255	; 0xff
30008adc:	2000      	movs	r0, #0
30008ade:	700a      	strb	r2, [r1, #0]
30008ae0:	4770      	bx	lr
30008ae2:	bf00      	nop

30008ae4 <PLL_NP_ClkSet>:
30008ae4:	b570      	push	{r4, r5, r6, lr}
30008ae6:	4604      	mov	r4, r0
30008ae8:	4b1b      	ldr	r3, [pc, #108]	; (30008b58 <PLL_NP_ClkSet+0x74>)
30008aea:	4798      	blx	r3
30008aec:	4d1b      	ldr	r5, [pc, #108]	; (30008b5c <PLL_NP_ClkSet+0x78>)
30008aee:	08c2      	lsrs	r2, r0, #3
30008af0:	fbb4 f3f0 	udiv	r3, r4, r0
30008af4:	fb00 4c13 	mls	ip, r0, r3, r4
30008af8:	1e9c      	subs	r4, r3, #2
30008afa:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008afc:	0164      	lsls	r4, r4, #5
30008afe:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30008b02:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30008b06:	431c      	orrs	r4, r3
30008b08:	fbbc f3f2 	udiv	r3, ip, r2
30008b0c:	646c      	str	r4, [r5, #68]	; 0x44
30008b0e:	041c      	lsls	r4, r3, #16
30008b10:	fb02 c013 	mls	r0, r2, r3, ip
30008b14:	2300      	movs	r3, #0
30008b16:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008b18:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008b1c:	4619      	mov	r1, r3
30008b1e:	fa1f fc86 	uxth.w	ip, r6
30008b22:	4e0f      	ldr	r6, [pc, #60]	; (30008b60 <PLL_NP_ClkSet+0x7c>)
30008b24:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008b28:	ea44 040c 	orr.w	r4, r4, ip
30008b2c:	f003 fa98 	bl	3000c060 <__aeabi_uldivmod>
30008b30:	4603      	mov	r3, r0
30008b32:	2001      	movs	r0, #1
30008b34:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008b38:	64ec      	str	r4, [r5, #76]	; 0x4c
30008b3a:	47b0      	blx	r6
30008b3c:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008b3e:	203c      	movs	r0, #60	; 0x3c
30008b40:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008b44:	646b      	str	r3, [r5, #68]	; 0x44
30008b46:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008b48:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008b4c:	646b      	str	r3, [r5, #68]	; 0x44
30008b4e:	4633      	mov	r3, r6
30008b50:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008b54:	4718      	bx	r3
30008b56:	bf00      	nop
30008b58:	000099f5 	.word	0x000099f5
30008b5c:	42008800 	.word	0x42008800
30008b60:	00009b2d 	.word	0x00009b2d

30008b64 <PLL_AP_ClkSet>:
30008b64:	b510      	push	{r4, lr}
30008b66:	4604      	mov	r4, r0
30008b68:	4b0b      	ldr	r3, [pc, #44]	; (30008b98 <PLL_AP_ClkSet+0x34>)
30008b6a:	4798      	blx	r3
30008b6c:	fbb4 f0f0 	udiv	r0, r4, r0
30008b70:	f1a0 0319 	sub.w	r3, r0, #25
30008b74:	1e84      	subs	r4, r0, #2
30008b76:	2b0f      	cmp	r3, #15
30008b78:	d807      	bhi.n	30008b8a <PLL_AP_ClkSet+0x26>
30008b7a:	4b08      	ldr	r3, [pc, #32]	; (30008b9c <PLL_AP_ClkSet+0x38>)
30008b7c:	b2e0      	uxtb	r0, r4
30008b7e:	695c      	ldr	r4, [r3, #20]
30008b80:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008b84:	4320      	orrs	r0, r4
30008b86:	6158      	str	r0, [r3, #20]
30008b88:	bd10      	pop	{r4, pc}
30008b8a:	f240 1181 	movw	r1, #385	; 0x181
30008b8e:	4804      	ldr	r0, [pc, #16]	; (30008ba0 <PLL_AP_ClkSet+0x3c>)
30008b90:	f003 fcd2 	bl	3000c538 <__io_assert_failed_veneer>
30008b94:	e7f1      	b.n	30008b7a <PLL_AP_ClkSet+0x16>
30008b96:	bf00      	nop
30008b98:	000099f5 	.word	0x000099f5
30008b9c:	41000300 	.word	0x41000300
30008ba0:	3000d7b4 	.word	0x3000d7b4

30008ba4 <PLL_AP>:
30008ba4:	2801      	cmp	r0, #1
30008ba6:	b538      	push	{r3, r4, r5, lr}
30008ba8:	d009      	beq.n	30008bbe <PLL_AP+0x1a>
30008baa:	4b13      	ldr	r3, [pc, #76]	; (30008bf8 <PLL_AP+0x54>)
30008bac:	681a      	ldr	r2, [r3, #0]
30008bae:	f022 0209 	bic.w	r2, r2, #9
30008bb2:	601a      	str	r2, [r3, #0]
30008bb4:	681a      	ldr	r2, [r3, #0]
30008bb6:	f022 0206 	bic.w	r2, r2, #6
30008bba:	601a      	str	r2, [r3, #0]
30008bbc:	bd38      	pop	{r3, r4, r5, pc}
30008bbe:	4b0f      	ldr	r3, [pc, #60]	; (30008bfc <PLL_AP+0x58>)
30008bc0:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008bc2:	f012 0f05 	tst.w	r2, #5
30008bc6:	d00f      	beq.n	30008be8 <PLL_AP+0x44>
30008bc8:	4d0d      	ldr	r5, [pc, #52]	; (30008c00 <PLL_AP+0x5c>)
30008bca:	4c0b      	ldr	r4, [pc, #44]	; (30008bf8 <PLL_AP+0x54>)
30008bcc:	2001      	movs	r0, #1
30008bce:	6823      	ldr	r3, [r4, #0]
30008bd0:	f043 0306 	orr.w	r3, r3, #6
30008bd4:	6023      	str	r3, [r4, #0]
30008bd6:	47a8      	blx	r5
30008bd8:	6823      	ldr	r3, [r4, #0]
30008bda:	f043 0309 	orr.w	r3, r3, #9
30008bde:	6023      	str	r3, [r4, #0]
30008be0:	6863      	ldr	r3, [r4, #4]
30008be2:	2b00      	cmp	r3, #0
30008be4:	dafc      	bge.n	30008be0 <PLL_AP+0x3c>
30008be6:	bd38      	pop	{r3, r4, r5, pc}
30008be8:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008bea:	20a0      	movs	r0, #160	; 0xa0
30008bec:	4d04      	ldr	r5, [pc, #16]	; (30008c00 <PLL_AP+0x5c>)
30008bee:	f042 0207 	orr.w	r2, r2, #7
30008bf2:	679a      	str	r2, [r3, #120]	; 0x78
30008bf4:	47a8      	blx	r5
30008bf6:	e7e8      	b.n	30008bca <PLL_AP+0x26>
30008bf8:	41000300 	.word	0x41000300
30008bfc:	42008800 	.word	0x42008800
30008c00:	00009b2d 	.word	0x00009b2d

30008c04 <PSRAM_INFO_Update>:
30008c04:	4b49      	ldr	r3, [pc, #292]	; (30008d2c <PSRAM_INFO_Update+0x128>)
30008c06:	494a      	ldr	r1, [pc, #296]	; (30008d30 <PSRAM_INFO_Update+0x12c>)
30008c08:	4a4a      	ldr	r2, [pc, #296]	; (30008d34 <PSRAM_INFO_Update+0x130>)
30008c0a:	b530      	push	{r4, r5, lr}
30008c0c:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008c10:	b083      	sub	sp, #12
30008c12:	680d      	ldr	r5, [r1, #0]
30008c14:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008c18:	4c47      	ldr	r4, [pc, #284]	; (30008d38 <PSRAM_INFO_Update+0x134>)
30008c1a:	fb02 f505 	mul.w	r5, r2, r5
30008c1e:	3301      	adds	r3, #1
30008c20:	fbb5 f5f3 	udiv	r5, r5, r3
30008c24:	086a      	lsrs	r2, r5, #1
30008c26:	4b45      	ldr	r3, [pc, #276]	; (30008d3c <PSRAM_INFO_Update+0x138>)
30008c28:	60e2      	str	r2, [r4, #12]
30008c2a:	4798      	blx	r3
30008c2c:	4b44      	ldr	r3, [pc, #272]	; (30008d40 <PSRAM_INFO_Update+0x13c>)
30008c2e:	6020      	str	r0, [r4, #0]
30008c30:	4798      	blx	r3
30008c32:	4b44      	ldr	r3, [pc, #272]	; (30008d44 <PSRAM_INFO_Update+0x140>)
30008c34:	220a      	movs	r2, #10
30008c36:	60a0      	str	r0, [r4, #8]
30008c38:	6122      	str	r2, [r4, #16]
30008c3a:	2220      	movs	r2, #32
30008c3c:	fbb3 f3f5 	udiv	r3, r3, r5
30008c40:	6163      	str	r3, [r4, #20]
30008c42:	fbb2 f3f3 	udiv	r3, r2, r3
30008c46:	6822      	ldr	r2, [r4, #0]
30008c48:	6223      	str	r3, [r4, #32]
30008c4a:	2a01      	cmp	r2, #1
30008c4c:	68e3      	ldr	r3, [r4, #12]
30008c4e:	4a3e      	ldr	r2, [pc, #248]	; (30008d48 <PSRAM_INFO_Update+0x144>)
30008c50:	d01d      	beq.n	30008c8e <PSRAM_INFO_Update+0x8a>
30008c52:	4293      	cmp	r3, r2
30008c54:	d810      	bhi.n	30008c78 <PSRAM_INFO_Update+0x74>
30008c56:	2304      	movs	r3, #4
30008c58:	2208      	movs	r2, #8
30008c5a:	4618      	mov	r0, r3
30008c5c:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008c60:	4b3a      	ldr	r3, [pc, #232]	; (30008d4c <PSRAM_INFO_Update+0x148>)
30008c62:	493b      	ldr	r1, [pc, #236]	; (30008d50 <PSRAM_INFO_Update+0x14c>)
30008c64:	4a3b      	ldr	r2, [pc, #236]	; (30008d54 <PSRAM_INFO_Update+0x150>)
30008c66:	f881 0110 	strb.w	r0, [r1, #272]	; 0x110
30008c6a:	4619      	mov	r1, r3
30008c6c:	2004      	movs	r0, #4
30008c6e:	9500      	str	r5, [sp, #0]
30008c70:	f003 f96c 	bl	3000bf4c <rtk_log_write>
30008c74:	b003      	add	sp, #12
30008c76:	bd30      	pop	{r4, r5, pc}
30008c78:	4a37      	ldr	r2, [pc, #220]	; (30008d58 <PSRAM_INFO_Update+0x154>)
30008c7a:	4293      	cmp	r3, r2
30008c7c:	d918      	bls.n	30008cb0 <PSRAM_INFO_Update+0xac>
30008c7e:	4a37      	ldr	r2, [pc, #220]	; (30008d5c <PSRAM_INFO_Update+0x158>)
30008c80:	4293      	cmp	r3, r2
30008c82:	d827      	bhi.n	30008cd4 <PSRAM_INFO_Update+0xd0>
30008c84:	2306      	movs	r3, #6
30008c86:	4618      	mov	r0, r3
30008c88:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008c8c:	e7e8      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008c8e:	4293      	cmp	r3, r2
30008c90:	d908      	bls.n	30008ca4 <PSRAM_INFO_Update+0xa0>
30008c92:	4a31      	ldr	r2, [pc, #196]	; (30008d58 <PSRAM_INFO_Update+0x154>)
30008c94:	4293      	cmp	r3, r2
30008c96:	d811      	bhi.n	30008cbc <PSRAM_INFO_Update+0xb8>
30008c98:	2305      	movs	r3, #5
30008c9a:	220f      	movs	r2, #15
30008c9c:	4618      	mov	r0, r3
30008c9e:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008ca2:	e7dd      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008ca4:	2304      	movs	r3, #4
30008ca6:	220f      	movs	r2, #15
30008ca8:	4618      	mov	r0, r3
30008caa:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008cae:	e7d7      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008cb0:	2305      	movs	r3, #5
30008cb2:	2208      	movs	r2, #8
30008cb4:	4618      	mov	r0, r3
30008cb6:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008cba:	e7d1      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008cbc:	4a27      	ldr	r2, [pc, #156]	; (30008d5c <PSRAM_INFO_Update+0x158>)
30008cbe:	4293      	cmp	r3, r2
30008cc0:	d918      	bls.n	30008cf4 <PSRAM_INFO_Update+0xf0>
30008cc2:	4a27      	ldr	r2, [pc, #156]	; (30008d60 <PSRAM_INFO_Update+0x15c>)
30008cc4:	4293      	cmp	r3, r2
30008cc6:	d821      	bhi.n	30008d0c <PSRAM_INFO_Update+0x108>
30008cc8:	2307      	movs	r3, #7
30008cca:	2214      	movs	r2, #20
30008ccc:	4618      	mov	r0, r3
30008cce:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008cd2:	e7c5      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008cd4:	4a22      	ldr	r2, [pc, #136]	; (30008d60 <PSRAM_INFO_Update+0x15c>)
30008cd6:	4293      	cmp	r3, r2
30008cd8:	d912      	bls.n	30008d00 <PSRAM_INFO_Update+0xfc>
30008cda:	4a22      	ldr	r2, [pc, #136]	; (30008d64 <PSRAM_INFO_Update+0x160>)
30008cdc:	4293      	cmp	r3, r2
30008cde:	bf8c      	ite	hi
30008ce0:	2300      	movhi	r3, #0
30008ce2:	2301      	movls	r3, #1
30008ce4:	2804      	cmp	r0, #4
30008ce6:	d100      	bne.n	30008cea <PSRAM_INFO_Update+0xe6>
30008ce8:	b953      	cbnz	r3, 30008d00 <PSRAM_INFO_Update+0xfc>
30008cea:	2803      	cmp	r0, #3
30008cec:	d100      	bne.n	30008cf0 <PSRAM_INFO_Update+0xec>
30008cee:	b9b3      	cbnz	r3, 30008d1e <PSRAM_INFO_Update+0x11a>
30008cf0:	7e20      	ldrb	r0, [r4, #24]
30008cf2:	e7b5      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008cf4:	2306      	movs	r3, #6
30008cf6:	2212      	movs	r2, #18
30008cf8:	4618      	mov	r0, r3
30008cfa:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008cfe:	e7af      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008d00:	2307      	movs	r3, #7
30008d02:	2206      	movs	r2, #6
30008d04:	4618      	mov	r0, r3
30008d06:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008d0a:	e7a9      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008d0c:	4a15      	ldr	r2, [pc, #84]	; (30008d64 <PSRAM_INFO_Update+0x160>)
30008d0e:	4293      	cmp	r3, r2
30008d10:	d8ee      	bhi.n	30008cf0 <PSRAM_INFO_Update+0xec>
30008d12:	2308      	movs	r3, #8
30008d14:	221c      	movs	r2, #28
30008d16:	4618      	mov	r0, r3
30008d18:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008d1c:	e7a0      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008d1e:	230a      	movs	r3, #10
30008d20:	2206      	movs	r2, #6
30008d22:	4618      	mov	r0, r3
30008d24:	e9c4 3206 	strd	r3, r2, [r4, #24]
30008d28:	e79a      	b.n	30008c60 <PSRAM_INFO_Update+0x5c>
30008d2a:	bf00      	nop
30008d2c:	42008000 	.word	0x42008000
30008d30:	3000efbc 	.word	0x3000efbc
30008d34:	000f4240 	.word	0x000f4240
30008d38:	3000fd00 	.word	0x3000fd00
30008d3c:	30007e5d 	.word	0x30007e5d
30008d40:	30007e25 	.word	0x30007e25
30008d44:	3b9aca00 	.word	0x3b9aca00
30008d48:	0632ea00 	.word	0x0632ea00
30008d4c:	3000d7c4 	.word	0x3000d7c4
30008d50:	23020000 	.word	0x23020000
30008d54:	3000d7cc 	.word	0x3000d7cc
30008d58:	07ed6b40 	.word	0x07ed6b40
30008d5c:	09e4f580 	.word	0x09e4f580
30008d60:	0bebc200 	.word	0x0bebc200
30008d64:	0ee6b280 	.word	0x0ee6b280

30008d68 <PSRAM_PHY_StructInit>:
30008d68:	2203      	movs	r2, #3
30008d6a:	2301      	movs	r3, #1
30008d6c:	210a      	movs	r1, #10
30008d6e:	b410      	push	{r4}
30008d70:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008d74:	2402      	movs	r4, #2
30008d76:	2207      	movs	r2, #7
30008d78:	e9c0 3400 	strd	r3, r4, [r0]
30008d7c:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008d80:	f85d 4b04 	ldr.w	r4, [sp], #4
30008d84:	6182      	str	r2, [r0, #24]
30008d86:	4770      	bx	lr

30008d88 <PSRAM_PHY_Init>:
30008d88:	b538      	push	{r3, r4, r5, lr}
30008d8a:	4604      	mov	r4, r0
30008d8c:	6903      	ldr	r3, [r0, #16]
30008d8e:	4d14      	ldr	r5, [pc, #80]	; (30008de0 <PSRAM_PHY_Init+0x58>)
30008d90:	f003 031f 	and.w	r3, r3, #31
30008d94:	4913      	ldr	r1, [pc, #76]	; (30008de4 <PSRAM_PHY_Init+0x5c>)
30008d96:	e9d0 0200 	ldrd	r0, r2, [r0]
30008d9a:	0612      	lsls	r2, r2, #24
30008d9c:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008da0:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30008da4:	4313      	orrs	r3, r2
30008da6:	68a2      	ldr	r2, [r4, #8]
30008da8:	0412      	lsls	r2, r2, #16
30008daa:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008dae:	4313      	orrs	r3, r2
30008db0:	68e2      	ldr	r2, [r4, #12]
30008db2:	0212      	lsls	r2, r2, #8
30008db4:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008db8:	4313      	orrs	r3, r2
30008dba:	606b      	str	r3, [r5, #4]
30008dbc:	4788      	blx	r1
30008dbe:	2801      	cmp	r0, #1
30008dc0:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30008dc4:	bf08      	it	eq
30008dc6:	4a08      	ldreq	r2, [pc, #32]	; (30008de8 <PSRAM_PHY_Init+0x60>)
30008dc8:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008dcc:	f001 010f 	and.w	r1, r1, #15
30008dd0:	bf18      	it	ne
30008dd2:	4a06      	ldrne	r2, [pc, #24]	; (30008dec <PSRAM_PHY_Init+0x64>)
30008dd4:	b2db      	uxtb	r3, r3
30008dd6:	430b      	orrs	r3, r1
30008dd8:	431a      	orrs	r2, r3
30008dda:	61aa      	str	r2, [r5, #24]
30008ddc:	bd38      	pop	{r3, r4, r5, pc}
30008dde:	bf00      	nop
30008de0:	41002000 	.word	0x41002000
30008de4:	30007e5d 	.word	0x30007e5d
30008de8:	80000e00 	.word	0x80000e00
30008dec:	80000f00 	.word	0x80000f00

30008df0 <PSRAM_CTRL_Init>:
30008df0:	b470      	push	{r4, r5, r6}
30008df2:	4b4c      	ldr	r3, [pc, #304]	; (30008f24 <PSRAM_CTRL_Init+0x134>)
30008df4:	2400      	movs	r4, #0
30008df6:	4a4c      	ldr	r2, [pc, #304]	; (30008f28 <PSRAM_CTRL_Init+0x138>)
30008df8:	f24d 0101 	movw	r1, #53249	; 0xd001
30008dfc:	609c      	str	r4, [r3, #8]
30008dfe:	2001      	movs	r0, #1
30008e00:	601a      	str	r2, [r3, #0]
30008e02:	4a4a      	ldr	r2, [pc, #296]	; (30008f2c <PSRAM_CTRL_Init+0x13c>)
30008e04:	6158      	str	r0, [r3, #20]
30008e06:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30008e0a:	6811      	ldr	r1, [r2, #0]
30008e0c:	2902      	cmp	r1, #2
30008e0e:	d04a      	beq.n	30008ea6 <PSRAM_CTRL_Init+0xb6>
30008e10:	6954      	ldr	r4, [r2, #20]
30008e12:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30008e16:	69d0      	ldr	r0, [r2, #28]
30008e18:	6915      	ldr	r5, [r2, #16]
30008e1a:	fbb0 f0f4 	udiv	r0, r0, r4
30008e1e:	fbb1 f1f4 	udiv	r1, r1, r4
30008e22:	6994      	ldr	r4, [r2, #24]
30008e24:	f005 020f 	and.w	r2, r5, #15
30008e28:	f241 5551 	movw	r5, #5457	; 0x1551
30008e2c:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008e30:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30008e34:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008e38:	094a      	lsrs	r2, r1, #5
30008e3a:	0181      	lsls	r1, r0, #6
30008e3c:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30008e40:	4d3b      	ldr	r5, [pc, #236]	; (30008f30 <PSRAM_CTRL_Init+0x140>)
30008e42:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008e46:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
30008e4a:	00a2      	lsls	r2, r4, #2
30008e4c:	4301      	orrs	r1, r0
30008e4e:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
30008e52:	3a01      	subs	r2, #1
30008e54:	430d      	orrs	r5, r1
30008e56:	f242 0120 	movw	r1, #8224	; 0x2020
30008e5a:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008e5e:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
30008e62:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
30008e66:	f44f 2180 	mov.w	r1, #262144	; 0x40000
30008e6a:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
30008e6e:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
30008e72:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008e76:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
30008e7a:	4420      	add	r0, r4
30008e7c:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30008e80:	f640 71fe 	movw	r1, #4094	; 0xffe
30008e84:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
30008e88:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30008e8c:	432a      	orrs	r2, r5
30008e8e:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
30008e92:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30008e96:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30008e9a:	4a22      	ldr	r2, [pc, #136]	; (30008f24 <PSRAM_CTRL_Init+0x134>)
30008e9c:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008e9e:	061b      	lsls	r3, r3, #24
30008ea0:	d5fc      	bpl.n	30008e9c <PSRAM_CTRL_Init+0xac>
30008ea2:	bc70      	pop	{r4, r5, r6}
30008ea4:	4770      	bx	lr
30008ea6:	6955      	ldr	r5, [r2, #20]
30008ea8:	207d      	movs	r0, #125	; 0x7d
30008eaa:	69d1      	ldr	r1, [r2, #28]
30008eac:	26a0      	movs	r6, #160	; 0xa0
30008eae:	6914      	ldr	r4, [r2, #16]
30008eb0:	6992      	ldr	r2, [r2, #24]
30008eb2:	f004 040f 	and.w	r4, r4, #15
30008eb6:	0092      	lsls	r2, r2, #2
30008eb8:	fbb1 f1f5 	udiv	r1, r1, r5
30008ebc:	fbb0 f5f5 	udiv	r5, r0, r5
30008ec0:	f245 5051 	movw	r0, #21841	; 0x5551
30008ec4:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30008ec8:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30008ecc:	0189      	lsls	r1, r1, #6
30008ece:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30008ed2:	f641 4570 	movw	r5, #7280	; 0x1c70
30008ed6:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008eda:	4325      	orrs	r5, r4
30008edc:	2420      	movs	r4, #32
30008ede:	4308      	orrs	r0, r1
30008ee0:	1e51      	subs	r1, r2, #1
30008ee2:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30008ee6:	3a02      	subs	r2, #2
30008ee8:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30008eec:	f3c1 010b 	ubfx	r1, r1, #0, #12
30008ef0:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008ef4:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30008ef8:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30008efc:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30008f00:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30008f04:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30008f08:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30008f0c:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30008f10:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30008f14:	4301      	orrs	r1, r0
30008f16:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30008f1a:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008f1e:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30008f22:	e7ba      	b.n	30008e9a <PSRAM_CTRL_Init+0xaa>
30008f24:	41012000 	.word	0x41012000
30008f28:	00bfe000 	.word	0x00bfe000
30008f2c:	3000fd00 	.word	0x3000fd00
30008f30:	00107000 	.word	0x00107000

30008f34 <PSRAM_REG_Write>:
30008f34:	b430      	push	{r4, r5}
30008f36:	4c20      	ldr	r4, [pc, #128]	; (30008fb8 <PSRAM_REG_Write+0x84>)
30008f38:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008f3a:	f013 0301 	ands.w	r3, r3, #1
30008f3e:	d1fb      	bne.n	30008f38 <PSRAM_REG_Write+0x4>
30008f40:	6825      	ldr	r5, [r4, #0]
30008f42:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008f46:	6025      	str	r5, [r4, #0]
30008f48:	60a3      	str	r3, [r4, #8]
30008f4a:	6825      	ldr	r5, [r4, #0]
30008f4c:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008f50:	6025      	str	r5, [r4, #0]
30008f52:	6063      	str	r3, [r4, #4]
30008f54:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30008f58:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30008f5c:	23c0      	movs	r3, #192	; 0xc0
30008f5e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f62:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f66:	0e03      	lsrs	r3, r0, #24
30008f68:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f6c:	f3c0 4307 	ubfx	r3, r0, #16, #8
30008f70:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f74:	f3c0 2307 	ubfx	r3, r0, #8, #8
30008f78:	b2c0      	uxtb	r0, r0
30008f7a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f7e:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008f82:	b141      	cbz	r1, 30008f96 <PSRAM_REG_Write+0x62>
30008f84:	3a01      	subs	r2, #1
30008f86:	480c      	ldr	r0, [pc, #48]	; (30008fb8 <PSRAM_REG_Write+0x84>)
30008f88:	4411      	add	r1, r2
30008f8a:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008f8e:	428a      	cmp	r2, r1
30008f90:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008f94:	d1f9      	bne.n	30008f8a <PSRAM_REG_Write+0x56>
30008f96:	4a08      	ldr	r2, [pc, #32]	; (30008fb8 <PSRAM_REG_Write+0x84>)
30008f98:	2301      	movs	r3, #1
30008f9a:	6093      	str	r3, [r2, #8]
30008f9c:	6893      	ldr	r3, [r2, #8]
30008f9e:	07d9      	lsls	r1, r3, #31
30008fa0:	d4fc      	bmi.n	30008f9c <PSRAM_REG_Write+0x68>
30008fa2:	4a05      	ldr	r2, [pc, #20]	; (30008fb8 <PSRAM_REG_Write+0x84>)
30008fa4:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008fa6:	07db      	lsls	r3, r3, #31
30008fa8:	d4fc      	bmi.n	30008fa4 <PSRAM_REG_Write+0x70>
30008faa:	6813      	ldr	r3, [r2, #0]
30008fac:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008fb0:	6013      	str	r3, [r2, #0]
30008fb2:	bc30      	pop	{r4, r5}
30008fb4:	4770      	bx	lr
30008fb6:	bf00      	nop
30008fb8:	41012000 	.word	0x41012000

30008fbc <PSRAM_APM_DEVIC_Init>:
30008fbc:	4b12      	ldr	r3, [pc, #72]	; (30009008 <PSRAM_APM_DEVIC_Init+0x4c>)
30008fbe:	2102      	movs	r1, #2
30008fc0:	2000      	movs	r0, #0
30008fc2:	b530      	push	{r4, r5, lr}
30008fc4:	4c11      	ldr	r4, [pc, #68]	; (3000900c <PSRAM_APM_DEVIC_Init+0x50>)
30008fc6:	b083      	sub	sp, #12
30008fc8:	699d      	ldr	r5, [r3, #24]
30008fca:	466a      	mov	r2, sp
30008fcc:	eb04 0c05 	add.w	ip, r4, r5
30008fd0:	1eeb      	subs	r3, r5, #3
30008fd2:	4c0f      	ldr	r4, [pc, #60]	; (30009010 <PSRAM_APM_DEVIC_Init+0x54>)
30008fd4:	408b      	lsls	r3, r1
30008fd6:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30008fda:	f003 031c 	and.w	r3, r3, #28
30008fde:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30008fe2:	f043 0320 	orr.w	r3, r3, #32
30008fe6:	fa5f fc8c 	uxtb.w	ip, ip
30008fea:	f88d 3000 	strb.w	r3, [sp]
30008fee:	f88d c004 	strb.w	ip, [sp, #4]
30008ff2:	f88d 3001 	strb.w	r3, [sp, #1]
30008ff6:	f88d c005 	strb.w	ip, [sp, #5]
30008ffa:	47a0      	blx	r4
30008ffc:	aa01      	add	r2, sp, #4
30008ffe:	2102      	movs	r1, #2
30009000:	2004      	movs	r0, #4
30009002:	47a0      	blx	r4
30009004:	b003      	add	sp, #12
30009006:	bd30      	pop	{r4, r5, pc}
30009008:	3000fd00 	.word	0x3000fd00
3000900c:	3000f184 	.word	0x3000f184
30009010:	30008f35 	.word	0x30008f35

30009014 <PSRAM_WB_REG_Write>:
30009014:	b430      	push	{r4, r5}
30009016:	4c1e      	ldr	r4, [pc, #120]	; (30009090 <PSRAM_WB_REG_Write+0x7c>)
30009018:	6aa3      	ldr	r3, [r4, #40]	; 0x28
3000901a:	f013 0301 	ands.w	r3, r3, #1
3000901e:	d1fb      	bne.n	30009018 <PSRAM_WB_REG_Write+0x4>
30009020:	6825      	ldr	r5, [r4, #0]
30009022:	b2c0      	uxtb	r0, r0
30009024:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30009028:	6025      	str	r5, [r4, #0]
3000902a:	60a3      	str	r3, [r4, #8]
3000902c:	6825      	ldr	r5, [r4, #0]
3000902e:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30009032:	6025      	str	r5, [r4, #0]
30009034:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30009038:	6063      	str	r3, [r4, #4]
3000903a:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
3000903e:	2560      	movs	r5, #96	; 0x60
30009040:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30009044:	2501      	movs	r5, #1
30009046:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
3000904a:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
3000904e:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009052:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30009056:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
3000905a:	b141      	cbz	r1, 3000906e <PSRAM_WB_REG_Write+0x5a>
3000905c:	3a01      	subs	r2, #1
3000905e:	480c      	ldr	r0, [pc, #48]	; (30009090 <PSRAM_WB_REG_Write+0x7c>)
30009060:	4411      	add	r1, r2
30009062:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30009066:	428a      	cmp	r2, r1
30009068:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000906c:	d1f9      	bne.n	30009062 <PSRAM_WB_REG_Write+0x4e>
3000906e:	4a08      	ldr	r2, [pc, #32]	; (30009090 <PSRAM_WB_REG_Write+0x7c>)
30009070:	2301      	movs	r3, #1
30009072:	6093      	str	r3, [r2, #8]
30009074:	6893      	ldr	r3, [r2, #8]
30009076:	07d9      	lsls	r1, r3, #31
30009078:	d4fc      	bmi.n	30009074 <PSRAM_WB_REG_Write+0x60>
3000907a:	4a05      	ldr	r2, [pc, #20]	; (30009090 <PSRAM_WB_REG_Write+0x7c>)
3000907c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000907e:	07db      	lsls	r3, r3, #31
30009080:	d4fc      	bmi.n	3000907c <PSRAM_WB_REG_Write+0x68>
30009082:	6813      	ldr	r3, [r2, #0]
30009084:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009088:	6013      	str	r3, [r2, #0]
3000908a:	bc30      	pop	{r4, r5}
3000908c:	4770      	bx	lr
3000908e:	bf00      	nop
30009090:	41012000 	.word	0x41012000

30009094 <PSRAM_WB_DEVIC_Init>:
30009094:	490b      	ldr	r1, [pc, #44]	; (300090c4 <PSRAM_WB_DEVIC_Init+0x30>)
30009096:	4b0c      	ldr	r3, [pc, #48]	; (300090c8 <PSRAM_WB_DEVIC_Init+0x34>)
30009098:	b510      	push	{r4, lr}
3000909a:	6988      	ldr	r0, [r1, #24]
3000909c:	b082      	sub	sp, #8
3000909e:	248f      	movs	r4, #143	; 0x8f
300090a0:	2102      	movs	r1, #2
300090a2:	4403      	add	r3, r0
300090a4:	aa01      	add	r2, sp, #4
300090a6:	f88d 4004 	strb.w	r4, [sp, #4]
300090aa:	2000      	movs	r0, #0
300090ac:	f813 3c03 	ldrb.w	r3, [r3, #-3]
300090b0:	4c06      	ldr	r4, [pc, #24]	; (300090cc <PSRAM_WB_DEVIC_Init+0x38>)
300090b2:	011b      	lsls	r3, r3, #4
300090b4:	f043 030c 	orr.w	r3, r3, #12
300090b8:	f88d 3005 	strb.w	r3, [sp, #5]
300090bc:	47a0      	blx	r4
300090be:	b002      	add	sp, #8
300090c0:	bd10      	pop	{r4, pc}
300090c2:	bf00      	nop
300090c4:	3000fd00 	.word	0x3000fd00
300090c8:	3000f1a4 	.word	0x3000f1a4
300090cc:	30009015 	.word	0x30009015

300090d0 <PSRAM_calibration>:
300090d0:	4a9c      	ldr	r2, [pc, #624]	; (30009344 <PSRAM_calibration+0x274>)
300090d2:	2300      	movs	r3, #0
300090d4:	499c      	ldr	r1, [pc, #624]	; (30009348 <PSRAM_calibration+0x278>)
300090d6:	489d      	ldr	r0, [pc, #628]	; (3000934c <PSRAM_calibration+0x27c>)
300090d8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300090dc:	6852      	ldr	r2, [r2, #4]
300090de:	b093      	sub	sp, #76	; 0x4c
300090e0:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
300090e4:	400a      	ands	r2, r1
300090e6:	9311      	str	r3, [sp, #68]	; 0x44
300090e8:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
300090ec:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
300090f0:	920a      	str	r2, [sp, #40]	; 0x28
300090f2:	6942      	ldr	r2, [r0, #20]
300090f4:	03d1      	lsls	r1, r2, #15
300090f6:	d51e      	bpl.n	30009136 <PSRAM_calibration+0x66>
300090f8:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300090fc:	f3bf 8f4f 	dsb	sy
30009100:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009104:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009108:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000910c:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009110:	0164      	lsls	r4, r4, #5
30009112:	ea04 0106 	and.w	r1, r4, r6
30009116:	462b      	mov	r3, r5
30009118:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000911c:	3b01      	subs	r3, #1
3000911e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009122:	1c5a      	adds	r2, r3, #1
30009124:	d1f8      	bne.n	30009118 <PSRAM_calibration+0x48>
30009126:	3c20      	subs	r4, #32
30009128:	f114 0f20 	cmn.w	r4, #32
3000912c:	d1f1      	bne.n	30009112 <PSRAM_calibration+0x42>
3000912e:	f3bf 8f4f 	dsb	sy
30009132:	f3bf 8f6f 	isb	sy
30009136:	4885      	ldr	r0, [pc, #532]	; (3000934c <PSRAM_calibration+0x27c>)
30009138:	2300      	movs	r3, #0
3000913a:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000913e:	f3bf 8f4f 	dsb	sy
30009142:	6943      	ldr	r3, [r0, #20]
30009144:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30009148:	6143      	str	r3, [r0, #20]
3000914a:	f3bf 8f4f 	dsb	sy
3000914e:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009152:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30009156:	f3c5 344e 	ubfx	r4, r5, #13, #15
3000915a:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000915e:	0164      	lsls	r4, r4, #5
30009160:	ea04 0106 	and.w	r1, r4, r6
30009164:	462b      	mov	r3, r5
30009166:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000916a:	3b01      	subs	r3, #1
3000916c:	1c5f      	adds	r7, r3, #1
3000916e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009172:	d1f8      	bne.n	30009166 <PSRAM_calibration+0x96>
30009174:	3c20      	subs	r4, #32
30009176:	f114 0f20 	cmn.w	r4, #32
3000917a:	d1f1      	bne.n	30009160 <PSRAM_calibration+0x90>
3000917c:	f3bf 8f4f 	dsb	sy
30009180:	f3bf 8f6f 	isb	sy
30009184:	2204      	movs	r2, #4
30009186:	461e      	mov	r6, r3
30009188:	9306      	str	r3, [sp, #24]
3000918a:	2700      	movs	r7, #0
3000918c:	9209      	str	r2, [sp, #36]	; 0x24
3000918e:	46b3      	mov	fp, r6
30009190:	4a6c      	ldr	r2, [pc, #432]	; (30009344 <PSRAM_calibration+0x274>)
30009192:	4c6f      	ldr	r4, [pc, #444]	; (30009350 <PSRAM_calibration+0x280>)
30009194:	6813      	ldr	r3, [r2, #0]
30009196:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 3000936c <PSRAM_calibration+0x29c>
3000919a:	f023 0301 	bic.w	r3, r3, #1
3000919e:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 30009370 <PSRAM_calibration+0x2a0>
300091a2:	6013      	str	r3, [r2, #0]
300091a4:	2301      	movs	r3, #1
300091a6:	9305      	str	r3, [sp, #20]
300091a8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300091aa:	9a05      	ldr	r2, [sp, #20]
300091ac:	9704      	str	r7, [sp, #16]
300091ae:	2700      	movs	r7, #0
300091b0:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
300091b4:	463d      	mov	r5, r7
300091b6:	e9cd b607 	strd	fp, r6, [sp, #28]
300091ba:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300091be:	465e      	mov	r6, fp
300091c0:	e010      	b.n	300091e4 <PSRAM_calibration+0x114>
300091c2:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
300091c6:	bf08      	it	eq
300091c8:	462e      	moveq	r6, r5
300091ca:	2d1f      	cmp	r5, #31
300091cc:	eb06 0b07 	add.w	fp, r6, r7
300091d0:	f107 0701 	add.w	r7, r7, #1
300091d4:	d102      	bne.n	300091dc <PSRAM_calibration+0x10c>
300091d6:	9b04      	ldr	r3, [sp, #16]
300091d8:	429f      	cmp	r7, r3
300091da:	dc46      	bgt.n	3000926a <PSRAM_calibration+0x19a>
300091dc:	3501      	adds	r5, #1
300091de:	2d20      	cmp	r5, #32
300091e0:	f000 80ac 	beq.w	3000933c <PSRAM_calibration+0x26c>
300091e4:	ea48 0205 	orr.w	r2, r8, r5
300091e8:	4b56      	ldr	r3, [pc, #344]	; (30009344 <PSRAM_calibration+0x274>)
300091ea:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
300091ee:	4959      	ldr	r1, [pc, #356]	; (30009354 <PSRAM_calibration+0x284>)
300091f0:	605a      	str	r2, [r3, #4]
300091f2:	6822      	ldr	r2, [r4, #0]
300091f4:	4b58      	ldr	r3, [pc, #352]	; (30009358 <PSRAM_calibration+0x288>)
300091f6:	f8ce 2000 	str.w	r2, [lr]
300091fa:	6860      	ldr	r0, [r4, #4]
300091fc:	4a57      	ldr	r2, [pc, #348]	; (3000935c <PSRAM_calibration+0x28c>)
300091fe:	f8c9 0000 	str.w	r0, [r9]
30009202:	68a0      	ldr	r0, [r4, #8]
30009204:	f8ca 0000 	str.w	r0, [sl]
30009208:	68e0      	ldr	r0, [r4, #12]
3000920a:	6018      	str	r0, [r3, #0]
3000920c:	6920      	ldr	r0, [r4, #16]
3000920e:	6008      	str	r0, [r1, #0]
30009210:	6960      	ldr	r0, [r4, #20]
30009212:	6010      	str	r0, [r2, #0]
30009214:	f8de 0000 	ldr.w	r0, [lr]
30009218:	900c      	str	r0, [sp, #48]	; 0x30
3000921a:	f8d9 0000 	ldr.w	r0, [r9]
3000921e:	900d      	str	r0, [sp, #52]	; 0x34
30009220:	f8da 0000 	ldr.w	r0, [sl]
30009224:	900e      	str	r0, [sp, #56]	; 0x38
30009226:	6818      	ldr	r0, [r3, #0]
30009228:	4b4d      	ldr	r3, [pc, #308]	; (30009360 <PSRAM_calibration+0x290>)
3000922a:	900f      	str	r0, [sp, #60]	; 0x3c
3000922c:	a80c      	add	r0, sp, #48	; 0x30
3000922e:	6809      	ldr	r1, [r1, #0]
30009230:	9110      	str	r1, [sp, #64]	; 0x40
30009232:	6812      	ldr	r2, [r2, #0]
30009234:	4946      	ldr	r1, [pc, #280]	; (30009350 <PSRAM_calibration+0x280>)
30009236:	9211      	str	r2, [sp, #68]	; 0x44
30009238:	2218      	movs	r2, #24
3000923a:	4798      	blx	r3
3000923c:	2800      	cmp	r0, #0
3000923e:	d0c0      	beq.n	300091c2 <PSRAM_calibration+0xf2>
30009240:	1c70      	adds	r0, r6, #1
30009242:	d0cb      	beq.n	300091dc <PSRAM_calibration+0x10c>
30009244:	9b04      	ldr	r3, [sp, #16]
30009246:	42bb      	cmp	r3, r7
30009248:	da0a      	bge.n	30009260 <PSRAM_calibration+0x190>
3000924a:	9b05      	ldr	r3, [sp, #20]
3000924c:	f8cd b01c 	str.w	fp, [sp, #28]
30009250:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
30009254:	9704      	str	r7, [sp, #16]
30009256:	2700      	movs	r7, #0
30009258:	9608      	str	r6, [sp, #32]
3000925a:	465e      	mov	r6, fp
3000925c:	9306      	str	r3, [sp, #24]
3000925e:	e7bd      	b.n	300091dc <PSRAM_calibration+0x10c>
30009260:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
30009264:	2700      	movs	r7, #0
30009266:	465e      	mov	r6, fp
30009268:	e7b8      	b.n	300091dc <PSRAM_calibration+0x10c>
3000926a:	9b05      	ldr	r3, [sp, #20]
3000926c:	9306      	str	r3, [sp, #24]
3000926e:	9b05      	ldr	r3, [sp, #20]
30009270:	005b      	lsls	r3, r3, #1
30009272:	9305      	str	r3, [sp, #20]
30009274:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009276:	3b01      	subs	r3, #1
30009278:	9309      	str	r3, [sp, #36]	; 0x24
3000927a:	d195      	bne.n	300091a8 <PSRAM_calibration+0xd8>
3000927c:	9b06      	ldr	r3, [sp, #24]
3000927e:	2004      	movs	r0, #4
30009280:	4a38      	ldr	r2, [pc, #224]	; (30009364 <PSRAM_calibration+0x294>)
30009282:	e9cd 7302 	strd	r7, r3, [sp, #8]
30009286:	4b38      	ldr	r3, [pc, #224]	; (30009368 <PSRAM_calibration+0x298>)
30009288:	e9cd 6b00 	strd	r6, fp, [sp]
3000928c:	4619      	mov	r1, r3
3000928e:	f002 fe5d 	bl	3000bf4c <rtk_log_write>
30009292:	482e      	ldr	r0, [pc, #184]	; (3000934c <PSRAM_calibration+0x27c>)
30009294:	6943      	ldr	r3, [r0, #20]
30009296:	03d9      	lsls	r1, r3, #15
30009298:	d425      	bmi.n	300092e6 <PSRAM_calibration+0x216>
3000929a:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000929c:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300092a0:	f3bf 8f4f 	dsb	sy
300092a4:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300092a8:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
300092ac:	f3c5 344e 	ubfx	r4, r5, #13, #15
300092b0:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300092b4:	0164      	lsls	r4, r4, #5
300092b6:	ea04 010c 	and.w	r1, r4, ip
300092ba:	462b      	mov	r3, r5
300092bc:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300092c0:	3b01      	subs	r3, #1
300092c2:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300092c6:	1c5a      	adds	r2, r3, #1
300092c8:	d1f8      	bne.n	300092bc <PSRAM_calibration+0x1ec>
300092ca:	3c20      	subs	r4, #32
300092cc:	f114 0f20 	cmn.w	r4, #32
300092d0:	d1f1      	bne.n	300092b6 <PSRAM_calibration+0x1e6>
300092d2:	f3bf 8f4f 	dsb	sy
300092d6:	6943      	ldr	r3, [r0, #20]
300092d8:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300092dc:	6143      	str	r3, [r0, #20]
300092de:	f3bf 8f4f 	dsb	sy
300092e2:	f3bf 8f6f 	isb	sy
300092e6:	2f08      	cmp	r7, #8
300092e8:	dd24      	ble.n	30009334 <PSRAM_calibration+0x264>
300092ea:	ebbb 0306 	subs.w	r3, fp, r6
300092ee:	9806      	ldr	r0, [sp, #24]
300092f0:	bf48      	it	mi
300092f2:	3301      	addmi	r3, #1
300092f4:	eb16 060b 	adds.w	r6, r6, fp
300092f8:	ea4f 0363 	mov.w	r3, r3, asr #1
300092fc:	bf48      	it	mi
300092fe:	3601      	addmi	r6, #1
30009300:	1e9a      	subs	r2, r3, #2
30009302:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30009304:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
30009308:	0414      	lsls	r4, r2, #16
3000930a:	f3c6 0344 	ubfx	r3, r6, #1, #5
3000930e:	0212      	lsls	r2, r2, #8
30009310:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
30009314:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
30009318:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
3000931c:	2001      	movs	r0, #1
3000931e:	430b      	orrs	r3, r1
30009320:	4908      	ldr	r1, [pc, #32]	; (30009344 <PSRAM_calibration+0x274>)
30009322:	4323      	orrs	r3, r4
30009324:	4313      	orrs	r3, r2
30009326:	604b      	str	r3, [r1, #4]
30009328:	680b      	ldr	r3, [r1, #0]
3000932a:	4303      	orrs	r3, r0
3000932c:	600b      	str	r3, [r1, #0]
3000932e:	b013      	add	sp, #76	; 0x4c
30009330:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30009334:	2000      	movs	r0, #0
30009336:	b013      	add	sp, #76	; 0x4c
30009338:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000933c:	9f04      	ldr	r7, [sp, #16]
3000933e:	e9dd b607 	ldrd	fp, r6, [sp, #28]
30009342:	e794      	b.n	3000926e <PSRAM_calibration+0x19e>
30009344:	41002000 	.word	0x41002000
30009348:	0fffffe0 	.word	0x0fffffe0
3000934c:	e000ed00 	.word	0xe000ed00
30009350:	3000f18c 	.word	0x3000f18c
30009354:	60200000 	.word	0x60200000
30009358:	60150000 	.word	0x60150000
3000935c:	60250000 	.word	0x60250000
30009360:	000129bd 	.word	0x000129bd
30009364:	3000d7ec 	.word	0x3000d7ec
30009368:	3000d7c4 	.word	0x3000d7c4
3000936c:	60050000 	.word	0x60050000
30009370:	60100000 	.word	0x60100000

30009374 <PSRAM_AutoGating>:
30009374:	b1c0      	cbz	r0, 300093a8 <PSRAM_AutoGating+0x34>
30009376:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000937a:	4810      	ldr	r0, [pc, #64]	; (300093bc <PSRAM_AutoGating+0x48>)
3000937c:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009380:	0252      	lsls	r2, r2, #9
30009382:	b410      	push	{r4}
30009384:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30009388:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
3000938c:	4020      	ands	r0, r4
3000938e:	4301      	orrs	r1, r0
30009390:	430a      	orrs	r2, r1
30009392:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30009396:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
3000939a:	f042 0201 	orr.w	r2, r2, #1
3000939e:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
300093a2:	f85d 4b04 	ldr.w	r4, [sp], #4
300093a6:	4770      	bx	lr
300093a8:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300093ac:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300093b0:	f023 0301 	bic.w	r3, r3, #1
300093b4:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300093b8:	4770      	bx	lr
300093ba:	bf00      	nop
300093bc:	ffffc000 	.word	0xffffc000

300093c0 <BOOT_Reason>:
300093c0:	4b02      	ldr	r3, [pc, #8]	; (300093cc <BOOT_Reason+0xc>)
300093c2:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
300093c6:	b280      	uxth	r0, r0
300093c8:	4770      	bx	lr
300093ca:	bf00      	nop
300093cc:	42008200 	.word	0x42008200

300093d0 <CRYPTO_SetSecurityModeAD>:
300093d0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
300093d4:	b083      	sub	sp, #12
300093d6:	460d      	mov	r5, r1
300093d8:	4616      	mov	r6, r2
300093da:	4699      	mov	r9, r3
300093dc:	4604      	mov	r4, r0
300093de:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
300093e2:	2800      	cmp	r0, #0
300093e4:	f000 80a9 	beq.w	3000953a <CRYPTO_SetSecurityModeAD+0x16a>
300093e8:	7823      	ldrb	r3, [r4, #0]
300093ea:	2b01      	cmp	r3, #1
300093ec:	d003      	beq.n	300093f6 <CRYPTO_SetSecurityModeAD+0x26>
300093ee:	2125      	movs	r1, #37	; 0x25
300093f0:	485d      	ldr	r0, [pc, #372]	; (30009568 <CRYPTO_SetSecurityModeAD+0x198>)
300093f2:	f003 f8a1 	bl	3000c538 <__io_assert_failed_veneer>
300093f6:	2300      	movs	r3, #0
300093f8:	1c6a      	adds	r2, r5, #1
300093fa:	6065      	str	r5, [r4, #4]
300093fc:	8123      	strh	r3, [r4, #8]
300093fe:	d008      	beq.n	30009412 <CRYPTO_SetSecurityModeAD+0x42>
30009400:	f005 0330 	and.w	r3, r5, #48	; 0x30
30009404:	2b20      	cmp	r3, #32
30009406:	d07b      	beq.n	30009500 <CRYPTO_SetSecurityModeAD+0x130>
30009408:	f085 0580 	eor.w	r5, r5, #128	; 0x80
3000940c:	f3c5 15c0 	ubfx	r5, r5, #7, #1
30009410:	7265      	strb	r5, [r4, #9]
30009412:	1c73      	adds	r3, r6, #1
30009414:	60e6      	str	r6, [r4, #12]
30009416:	f000 8082 	beq.w	3000951e <CRYPTO_SetSecurityModeAD+0x14e>
3000941a:	f006 0303 	and.w	r3, r6, #3
3000941e:	f3c6 0280 	ubfx	r2, r6, #2, #1
30009422:	7422      	strb	r2, [r4, #16]
30009424:	f1a3 0202 	sub.w	r2, r3, #2
30009428:	2b01      	cmp	r3, #1
3000942a:	fab2 f282 	clz	r2, r2
3000942e:	ea4f 1252 	mov.w	r2, r2, lsr #5
30009432:	7462      	strb	r2, [r4, #17]
30009434:	fab3 f283 	clz	r2, r3
30009438:	ea4f 1252 	mov.w	r2, r2, lsr #5
3000943c:	74a2      	strb	r2, [r4, #18]
3000943e:	bf0c      	ite	eq
30009440:	2201      	moveq	r2, #1
30009442:	2200      	movne	r2, #0
30009444:	74e2      	strb	r2, [r4, #19]
30009446:	d13f      	bne.n	300094c8 <CRYPTO_SetSecurityModeAD+0xf8>
30009448:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
3000944c:	2b40      	cmp	r3, #64	; 0x40
3000944e:	f000 8083 	beq.w	30009558 <CRYPTO_SetSecurityModeAD+0x188>
30009452:	d86a      	bhi.n	3000952a <CRYPTO_SetSecurityModeAD+0x15a>
30009454:	2b10      	cmp	r3, #16
30009456:	d06d      	beq.n	30009534 <CRYPTO_SetSecurityModeAD+0x164>
30009458:	2b20      	cmp	r3, #32
3000945a:	d173      	bne.n	30009544 <CRYPTO_SetSecurityModeAD+0x174>
3000945c:	7523      	strb	r3, [r4, #20]
3000945e:	62a3      	str	r3, [r4, #40]	; 0x28
30009460:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009462:	6227      	str	r7, [r4, #32]
30009464:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009468:	2f00      	cmp	r7, #0
3000946a:	d038      	beq.n	300094de <CRYPTO_SetSecurityModeAD+0x10e>
3000946c:	7d23      	ldrb	r3, [r4, #20]
3000946e:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009472:	2b30      	cmp	r3, #48	; 0x30
30009474:	65e0      	str	r0, [r4, #92]	; 0x5c
30009476:	d046      	beq.n	30009506 <CRYPTO_SetSecurityModeAD+0x136>
30009478:	2b40      	cmp	r3, #64	; 0x40
3000947a:	d044      	beq.n	30009506 <CRYPTO_SetSecurityModeAD+0x136>
3000947c:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009480:	2240      	movs	r2, #64	; 0x40
30009482:	2136      	movs	r1, #54	; 0x36
30009484:	4d39      	ldr	r5, [pc, #228]	; (3000956c <CRYPTO_SetSecurityModeAD+0x19c>)
30009486:	6623      	str	r3, [r4, #96]	; 0x60
30009488:	47a8      	blx	r5
3000948a:	2240      	movs	r2, #64	; 0x40
3000948c:	215c      	movs	r1, #92	; 0x5c
3000948e:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009490:	47a8      	blx	r5
30009492:	4b37      	ldr	r3, [pc, #220]	; (30009570 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009494:	681d      	ldr	r5, [r3, #0]
30009496:	bb25      	cbnz	r5, 300094e2 <CRYPTO_SetSecurityModeAD+0x112>
30009498:	f1b8 0f00 	cmp.w	r8, #0
3000949c:	d05f      	beq.n	3000955e <CRYPTO_SetSecurityModeAD+0x18e>
3000949e:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
300094a2:	e001      	b.n	300094a8 <CRYPTO_SetSecurityModeAD+0xd8>
300094a4:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
300094a8:	6de2      	ldr	r2, [r4, #92]	; 0x5c
300094aa:	f818 3005 	ldrb.w	r3, [r8, r5]
300094ae:	5d51      	ldrb	r1, [r2, r5]
300094b0:	404b      	eors	r3, r1
300094b2:	5553      	strb	r3, [r2, r5]
300094b4:	6e22      	ldr	r2, [r4, #96]	; 0x60
300094b6:	6a63      	ldr	r3, [r4, #36]	; 0x24
300094b8:	5d51      	ldrb	r1, [r2, r5]
300094ba:	5d5b      	ldrb	r3, [r3, r5]
300094bc:	404b      	eors	r3, r1
300094be:	5553      	strb	r3, [r2, r5]
300094c0:	3501      	adds	r5, #1
300094c2:	42af      	cmp	r7, r5
300094c4:	d1ee      	bne.n	300094a4 <CRYPTO_SetSecurityModeAD+0xd4>
300094c6:	e00c      	b.n	300094e2 <CRYPTO_SetSecurityModeAD+0x112>
300094c8:	2200      	movs	r2, #0
300094ca:	7522      	strb	r2, [r4, #20]
300094cc:	b99b      	cbnz	r3, 300094f6 <CRYPTO_SetSecurityModeAD+0x126>
300094ce:	2314      	movs	r3, #20
300094d0:	6227      	str	r7, [r4, #32]
300094d2:	62a3      	str	r3, [r4, #40]	; 0x28
300094d4:	9b0a      	ldr	r3, [sp, #40]	; 0x28
300094d6:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
300094da:	2f00      	cmp	r7, #0
300094dc:	d1c6      	bne.n	3000946c <CRYPTO_SetSecurityModeAD+0x9c>
300094de:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
300094e2:	3601      	adds	r6, #1
300094e4:	d003      	beq.n	300094ee <CRYPTO_SetSecurityModeAD+0x11e>
300094e6:	2201      	movs	r2, #1
300094e8:	2300      	movs	r3, #0
300094ea:	85a2      	strh	r2, [r4, #44]	; 0x2c
300094ec:	6323      	str	r3, [r4, #48]	; 0x30
300094ee:	2000      	movs	r0, #0
300094f0:	b003      	add	sp, #12
300094f2:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300094f6:	2b02      	cmp	r3, #2
300094f8:	d114      	bne.n	30009524 <CRYPTO_SetSecurityModeAD+0x154>
300094fa:	2310      	movs	r3, #16
300094fc:	62a3      	str	r3, [r4, #40]	; 0x28
300094fe:	e7af      	b.n	30009460 <CRYPTO_SetSecurityModeAD+0x90>
30009500:	2301      	movs	r3, #1
30009502:	7223      	strb	r3, [r4, #8]
30009504:	e780      	b.n	30009408 <CRYPTO_SetSecurityModeAD+0x38>
30009506:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
3000950a:	2280      	movs	r2, #128	; 0x80
3000950c:	2136      	movs	r1, #54	; 0x36
3000950e:	4d17      	ldr	r5, [pc, #92]	; (3000956c <CRYPTO_SetSecurityModeAD+0x19c>)
30009510:	6623      	str	r3, [r4, #96]	; 0x60
30009512:	47a8      	blx	r5
30009514:	2280      	movs	r2, #128	; 0x80
30009516:	215c      	movs	r1, #92	; 0x5c
30009518:	6e20      	ldr	r0, [r4, #96]	; 0x60
3000951a:	47a8      	blx	r5
3000951c:	e7b9      	b.n	30009492 <CRYPTO_SetSecurityModeAD+0xc2>
3000951e:	2300      	movs	r3, #0
30009520:	6123      	str	r3, [r4, #16]
30009522:	7523      	strb	r3, [r4, #20]
30009524:	2300      	movs	r3, #0
30009526:	62a3      	str	r3, [r4, #40]	; 0x28
30009528:	e79a      	b.n	30009460 <CRYPTO_SetSecurityModeAD+0x90>
3000952a:	2b80      	cmp	r3, #128	; 0x80
3000952c:	d10a      	bne.n	30009544 <CRYPTO_SetSecurityModeAD+0x174>
3000952e:	2340      	movs	r3, #64	; 0x40
30009530:	7523      	strb	r3, [r4, #20]
30009532:	e794      	b.n	3000945e <CRYPTO_SetSecurityModeAD+0x8e>
30009534:	231c      	movs	r3, #28
30009536:	7523      	strb	r3, [r4, #20]
30009538:	e7f5      	b.n	30009526 <CRYPTO_SetSecurityModeAD+0x156>
3000953a:	2124      	movs	r1, #36	; 0x24
3000953c:	480a      	ldr	r0, [pc, #40]	; (30009568 <CRYPTO_SetSecurityModeAD+0x198>)
3000953e:	f002 fffb 	bl	3000c538 <__io_assert_failed_veneer>
30009542:	e751      	b.n	300093e8 <CRYPTO_SetSecurityModeAD+0x18>
30009544:	4b0b      	ldr	r3, [pc, #44]	; (30009574 <CRYPTO_SetSecurityModeAD+0x1a4>)
30009546:	2002      	movs	r0, #2
30009548:	4a0b      	ldr	r2, [pc, #44]	; (30009578 <CRYPTO_SetSecurityModeAD+0x1a8>)
3000954a:	4619      	mov	r1, r3
3000954c:	9600      	str	r6, [sp, #0]
3000954e:	f002 fcfd 	bl	3000bf4c <rtk_log_write>
30009552:	2300      	movs	r3, #0
30009554:	7523      	strb	r3, [r4, #20]
30009556:	e7e6      	b.n	30009526 <CRYPTO_SetSecurityModeAD+0x156>
30009558:	2330      	movs	r3, #48	; 0x30
3000955a:	7523      	strb	r3, [r4, #20]
3000955c:	e7e3      	b.n	30009526 <CRYPTO_SetSecurityModeAD+0x156>
3000955e:	2180      	movs	r1, #128	; 0x80
30009560:	4801      	ldr	r0, [pc, #4]	; (30009568 <CRYPTO_SetSecurityModeAD+0x198>)
30009562:	f002 ffe9 	bl	3000c538 <__io_assert_failed_veneer>
30009566:	e79a      	b.n	3000949e <CRYPTO_SetSecurityModeAD+0xce>
30009568:	3000d8c8 	.word	0x3000d8c8
3000956c:	00012be5 	.word	0x00012be5
30009570:	2001c200 	.word	0x2001c200
30009574:	3000d82c 	.word	0x3000d82c
30009578:	3000d838 	.word	0x3000d838

3000957c <rtl_crypto_hmac_sha2_init>:
3000957c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009580:	4c97      	ldr	r4, [pc, #604]	; (300097e0 <rtl_crypto_hmac_sha2_init+0x264>)
30009582:	4617      	mov	r7, r2
30009584:	e844 f400 	tt	r4, r4
30009588:	4a96      	ldr	r2, [pc, #600]	; (300097e4 <rtl_crypto_hmac_sha2_init+0x268>)
3000958a:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
3000958e:	461c      	mov	r4, r3
30009590:	4b95      	ldr	r3, [pc, #596]	; (300097e8 <rtl_crypto_hmac_sha2_init+0x26c>)
30009592:	bf18      	it	ne
30009594:	4613      	movne	r3, r2
30009596:	b085      	sub	sp, #20
30009598:	4605      	mov	r5, r0
3000959a:	460e      	mov	r6, r1
3000959c:	6b5b      	ldr	r3, [r3, #52]	; 0x34
3000959e:	2b00      	cmp	r3, #0
300095a0:	db05      	blt.n	300095ae <rtl_crypto_hmac_sha2_init+0x32>
300095a2:	2900      	cmp	r1, #0
300095a4:	f000 8118 	beq.w	300097d8 <rtl_crypto_hmac_sha2_init+0x25c>
300095a8:	078b      	lsls	r3, r1, #30
300095aa:	f040 8112 	bne.w	300097d2 <rtl_crypto_hmac_sha2_init+0x256>
300095ae:	2d20      	cmp	r5, #32
300095b0:	bf18      	it	ne
300095b2:	2d1c      	cmpne	r5, #28
300095b4:	bf0c      	ite	eq
300095b6:	f04f 0901 	moveq.w	r9, #1
300095ba:	f04f 0900 	movne.w	r9, #0
300095be:	d04d      	beq.n	3000965c <rtl_crypto_hmac_sha2_init+0xe0>
300095c0:	2d40      	cmp	r5, #64	; 0x40
300095c2:	bf18      	it	ne
300095c4:	2d30      	cmpne	r5, #48	; 0x30
300095c6:	d102      	bne.n	300095ce <rtl_crypto_hmac_sha2_init+0x52>
300095c8:	2f80      	cmp	r7, #128	; 0x80
300095ca:	f200 80f9 	bhi.w	300097c0 <rtl_crypto_hmac_sha2_init+0x244>
300095ce:	f8df 8238 	ldr.w	r8, [pc, #568]	; 30009808 <rtl_crypto_hmac_sha2_init+0x28c>
300095d2:	f898 3000 	ldrb.w	r3, [r8]
300095d6:	2b01      	cmp	r3, #1
300095d8:	d003      	beq.n	300095e2 <rtl_crypto_hmac_sha2_init+0x66>
300095da:	21b4      	movs	r1, #180	; 0xb4
300095dc:	4883      	ldr	r0, [pc, #524]	; (300097ec <rtl_crypto_hmac_sha2_init+0x270>)
300095de:	f002 ffab 	bl	3000c538 <__io_assert_failed_veneer>
300095e2:	2d30      	cmp	r5, #48	; 0x30
300095e4:	f000 809b 	beq.w	3000971e <rtl_crypto_hmac_sha2_init+0x1a2>
300095e8:	d83f      	bhi.n	3000966a <rtl_crypto_hmac_sha2_init+0xee>
300095ea:	2d1c      	cmp	r5, #28
300095ec:	f000 80cb 	beq.w	30009786 <rtl_crypto_hmac_sha2_init+0x20a>
300095f0:	f1b9 0f00 	cmp.w	r9, #0
300095f4:	d078      	beq.n	300096e8 <rtl_crypto_hmac_sha2_init+0x16c>
300095f6:	2c00      	cmp	r4, #0
300095f8:	f000 808f 	beq.w	3000971a <rtl_crypto_hmac_sha2_init+0x19e>
300095fc:	4b7c      	ldr	r3, [pc, #496]	; (300097f0 <rtl_crypto_hmac_sha2_init+0x274>)
300095fe:	f8df e210 	ldr.w	lr, [pc, #528]	; 30009810 <rtl_crypto_hmac_sha2_init+0x294>
30009602:	f8df 921c 	ldr.w	r9, [pc, #540]	; 30009820 <rtl_crypto_hmac_sha2_init+0x2a4>
30009606:	4a7b      	ldr	r2, [pc, #492]	; (300097f4 <rtl_crypto_hmac_sha2_init+0x278>)
30009608:	e9c4 e302 	strd	lr, r3, [r4, #8]
3000960c:	4b7a      	ldr	r3, [pc, #488]	; (300097f8 <rtl_crypto_hmac_sha2_init+0x27c>)
3000960e:	e9c4 9304 	strd	r9, r3, [r4, #16]
30009612:	4b7a      	ldr	r3, [pc, #488]	; (300097fc <rtl_crypto_hmac_sha2_init+0x280>)
30009614:	e9c4 2306 	strd	r2, r3, [r4, #24]
30009618:	4b79      	ldr	r3, [pc, #484]	; (30009800 <rtl_crypto_hmac_sha2_init+0x284>)
3000961a:	2225      	movs	r2, #37	; 0x25
3000961c:	6223      	str	r3, [r4, #32]
3000961e:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
30009622:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
30009626:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
3000962a:	6263      	str	r3, [r4, #36]	; 0x24
3000962c:	2300      	movs	r3, #0
3000962e:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30009632:	4d74      	ldr	r5, [pc, #464]	; (30009804 <rtl_crypto_hmac_sha2_init+0x288>)
30009634:	9300      	str	r3, [sp, #0]
30009636:	9702      	str	r7, [sp, #8]
30009638:	9601      	str	r6, [sp, #4]
3000963a:	4873      	ldr	r0, [pc, #460]	; (30009808 <rtl_crypto_hmac_sha2_init+0x28c>)
3000963c:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009640:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
30009644:	47a8      	blx	r5
30009646:	4605      	mov	r5, r0
30009648:	4b70      	ldr	r3, [pc, #448]	; (3000980c <rtl_crypto_hmac_sha2_init+0x290>)
3000964a:	4620      	mov	r0, r4
3000964c:	4798      	blx	r3
3000964e:	2301      	movs	r3, #1
30009650:	4628      	mov	r0, r5
30009652:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
30009656:	b005      	add	sp, #20
30009658:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000965c:	2f40      	cmp	r7, #64	; 0x40
3000965e:	d9b6      	bls.n	300095ce <rtl_crypto_hmac_sha2_init+0x52>
30009660:	21ae      	movs	r1, #174	; 0xae
30009662:	4862      	ldr	r0, [pc, #392]	; (300097ec <rtl_crypto_hmac_sha2_init+0x270>)
30009664:	f002 ff68 	bl	3000c538 <__io_assert_failed_veneer>
30009668:	e7b1      	b.n	300095ce <rtl_crypto_hmac_sha2_init+0x52>
3000966a:	2d40      	cmp	r5, #64	; 0x40
3000966c:	d13c      	bne.n	300096e8 <rtl_crypto_hmac_sha2_init+0x16c>
3000966e:	2c00      	cmp	r4, #0
30009670:	d040      	beq.n	300096f4 <rtl_crypto_hmac_sha2_init+0x178>
30009672:	4b67      	ldr	r3, [pc, #412]	; (30009810 <rtl_crypto_hmac_sha2_init+0x294>)
30009674:	4a67      	ldr	r2, [pc, #412]	; (30009814 <rtl_crypto_hmac_sha2_init+0x298>)
30009676:	4968      	ldr	r1, [pc, #416]	; (30009818 <rtl_crypto_hmac_sha2_init+0x29c>)
30009678:	4d68      	ldr	r5, [pc, #416]	; (3000981c <rtl_crypto_hmac_sha2_init+0x2a0>)
3000967a:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009860 <rtl_crypto_hmac_sha2_init+0x2e4>
3000967e:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009682:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009686:	2285      	movs	r2, #133	; 0x85
30009688:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
3000968c:	33d4      	adds	r3, #212	; 0xd4
3000968e:	6163      	str	r3, [r4, #20]
30009690:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009694:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009698:	f203 734a 	addw	r3, r3, #1866	; 0x74a
3000969c:	6123      	str	r3, [r4, #16]
3000969e:	4b60      	ldr	r3, [pc, #384]	; (30009820 <rtl_crypto_hmac_sha2_init+0x2a4>)
300096a0:	e9c4 3106 	strd	r3, r1, [r4, #24]
300096a4:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
300096a8:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
300096ac:	f203 337f 	addw	r3, r3, #895	; 0x37f
300096b0:	6263      	str	r3, [r4, #36]	; 0x24
300096b2:	4b51      	ldr	r3, [pc, #324]	; (300097f8 <rtl_crypto_hmac_sha2_init+0x27c>)
300096b4:	6223      	str	r3, [r4, #32]
300096b6:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
300096ba:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
300096be:	f203 5397 	addw	r3, r3, #1431	; 0x597
300096c2:	62e3      	str	r3, [r4, #44]	; 0x2c
300096c4:	4b4b      	ldr	r3, [pc, #300]	; (300097f4 <rtl_crypto_hmac_sha2_init+0x278>)
300096c6:	62a3      	str	r3, [r4, #40]	; 0x28
300096c8:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
300096cc:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
300096d0:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
300096d4:	6363      	str	r3, [r4, #52]	; 0x34
300096d6:	4b49      	ldr	r3, [pc, #292]	; (300097fc <rtl_crypto_hmac_sha2_init+0x280>)
300096d8:	6323      	str	r3, [r4, #48]	; 0x30
300096da:	4b49      	ldr	r3, [pc, #292]	; (30009800 <rtl_crypto_hmac_sha2_init+0x284>)
300096dc:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
300096e0:	4b50      	ldr	r3, [pc, #320]	; (30009824 <rtl_crypto_hmac_sha2_init+0x2a8>)
300096e2:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
300096e6:	e7a1      	b.n	3000962c <rtl_crypto_hmac_sha2_init+0xb0>
300096e8:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
300096ec:	4628      	mov	r0, r5
300096ee:	b005      	add	sp, #20
300096f0:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300096f4:	2285      	movs	r2, #133	; 0x85
300096f6:	2300      	movs	r3, #0
300096f8:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
300096fc:	4c41      	ldr	r4, [pc, #260]	; (30009804 <rtl_crypto_hmac_sha2_init+0x288>)
300096fe:	9702      	str	r7, [sp, #8]
30009700:	4841      	ldr	r0, [pc, #260]	; (30009808 <rtl_crypto_hmac_sha2_init+0x28c>)
30009702:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
30009706:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
3000970a:	e9cd 3600 	strd	r3, r6, [sp]
3000970e:	47a0      	blx	r4
30009710:	4605      	mov	r5, r0
30009712:	4628      	mov	r0, r5
30009714:	b005      	add	sp, #20
30009716:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000971a:	2225      	movs	r2, #37	; 0x25
3000971c:	e7eb      	b.n	300096f6 <rtl_crypto_hmac_sha2_init+0x17a>
3000971e:	2c00      	cmp	r4, #0
30009720:	d053      	beq.n	300097ca <rtl_crypto_hmac_sha2_init+0x24e>
30009722:	4b41      	ldr	r3, [pc, #260]	; (30009828 <rtl_crypto_hmac_sha2_init+0x2ac>)
30009724:	f8df e130 	ldr.w	lr, [pc, #304]	; 30009858 <rtl_crypto_hmac_sha2_init+0x2dc>
30009728:	4a40      	ldr	r2, [pc, #256]	; (3000982c <rtl_crypto_hmac_sha2_init+0x2b0>)
3000972a:	4941      	ldr	r1, [pc, #260]	; (30009830 <rtl_crypto_hmac_sha2_init+0x2b4>)
3000972c:	4d41      	ldr	r5, [pc, #260]	; (30009834 <rtl_crypto_hmac_sha2_init+0x2b8>)
3000972e:	f8df c12c 	ldr.w	ip, [pc, #300]	; 3000985c <rtl_crypto_hmac_sha2_init+0x2e0>
30009732:	e9c4 3e02 	strd	r3, lr, [r4, #8]
30009736:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
3000973a:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
3000973e:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
30009742:	6163      	str	r3, [r4, #20]
30009744:	4b3c      	ldr	r3, [pc, #240]	; (30009838 <rtl_crypto_hmac_sha2_init+0x2bc>)
30009746:	6123      	str	r3, [r4, #16]
30009748:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
3000974c:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
30009750:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
30009754:	61e3      	str	r3, [r4, #28]
30009756:	4b39      	ldr	r3, [pc, #228]	; (3000983c <rtl_crypto_hmac_sha2_init+0x2c0>)
30009758:	61a3      	str	r3, [r4, #24]
3000975a:	4b39      	ldr	r3, [pc, #228]	; (30009840 <rtl_crypto_hmac_sha2_init+0x2c4>)
3000975c:	e9c4 3208 	strd	r3, r2, [r4, #32]
30009760:	4b38      	ldr	r3, [pc, #224]	; (30009844 <rtl_crypto_hmac_sha2_init+0x2c8>)
30009762:	2245      	movs	r2, #69	; 0x45
30009764:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30009768:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
3000976c:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30009770:	33aa      	adds	r3, #170	; 0xaa
30009772:	6363      	str	r3, [r4, #52]	; 0x34
30009774:	4b34      	ldr	r3, [pc, #208]	; (30009848 <rtl_crypto_hmac_sha2_init+0x2cc>)
30009776:	6323      	str	r3, [r4, #48]	; 0x30
30009778:	4b34      	ldr	r3, [pc, #208]	; (3000984c <rtl_crypto_hmac_sha2_init+0x2d0>)
3000977a:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000977e:	4b34      	ldr	r3, [pc, #208]	; (30009850 <rtl_crypto_hmac_sha2_init+0x2d4>)
30009780:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009784:	e752      	b.n	3000962c <rtl_crypto_hmac_sha2_init+0xb0>
30009786:	b314      	cbz	r4, 300097ce <rtl_crypto_hmac_sha2_init+0x252>
30009788:	4b32      	ldr	r3, [pc, #200]	; (30009854 <rtl_crypto_hmac_sha2_init+0x2d8>)
3000978a:	2215      	movs	r2, #21
3000978c:	4932      	ldr	r1, [pc, #200]	; (30009858 <rtl_crypto_hmac_sha2_init+0x2dc>)
3000978e:	4827      	ldr	r0, [pc, #156]	; (3000982c <rtl_crypto_hmac_sha2_init+0x2b0>)
30009790:	4d28      	ldr	r5, [pc, #160]	; (30009834 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009792:	e9c4 1302 	strd	r1, r3, [r4, #8]
30009796:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
3000979a:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
3000979e:	f503 6301 	add.w	r3, r3, #2064	; 0x810
300097a2:	6123      	str	r3, [r4, #16]
300097a4:	4b22      	ldr	r3, [pc, #136]	; (30009830 <rtl_crypto_hmac_sha2_init+0x2b4>)
300097a6:	e9c4 0305 	strd	r0, r3, [r4, #20]
300097aa:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
300097ae:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
300097b2:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
300097b6:	61e3      	str	r3, [r4, #28]
300097b8:	4b28      	ldr	r3, [pc, #160]	; (3000985c <rtl_crypto_hmac_sha2_init+0x2e0>)
300097ba:	e9c4 5308 	strd	r5, r3, [r4, #32]
300097be:	e735      	b.n	3000962c <rtl_crypto_hmac_sha2_init+0xb0>
300097c0:	21b1      	movs	r1, #177	; 0xb1
300097c2:	480a      	ldr	r0, [pc, #40]	; (300097ec <rtl_crypto_hmac_sha2_init+0x270>)
300097c4:	f002 feb8 	bl	3000c538 <__io_assert_failed_veneer>
300097c8:	e701      	b.n	300095ce <rtl_crypto_hmac_sha2_init+0x52>
300097ca:	2245      	movs	r2, #69	; 0x45
300097cc:	e793      	b.n	300096f6 <rtl_crypto_hmac_sha2_init+0x17a>
300097ce:	2215      	movs	r2, #21
300097d0:	e791      	b.n	300096f6 <rtl_crypto_hmac_sha2_init+0x17a>
300097d2:	f06f 0505 	mvn.w	r5, #5
300097d6:	e789      	b.n	300096ec <rtl_crypto_hmac_sha2_init+0x170>
300097d8:	f06f 0503 	mvn.w	r5, #3
300097dc:	e786      	b.n	300096ec <rtl_crypto_hmac_sha2_init+0x170>
300097de:	bf00      	nop
300097e0:	0000e5f9 	.word	0x0000e5f9
300097e4:	500c8000 	.word	0x500c8000
300097e8:	400c8000 	.word	0x400c8000
300097ec:	3000d8ac 	.word	0x3000d8ac
300097f0:	bb67ae85 	.word	0xbb67ae85
300097f4:	510e527f 	.word	0x510e527f
300097f8:	a54ff53a 	.word	0xa54ff53a
300097fc:	9b05688c 	.word	0x9b05688c
30009800:	1f83d9ab 	.word	0x1f83d9ab
30009804:	300093d1 	.word	0x300093d1
30009808:	2001c460 	.word	0x2001c460
3000980c:	00004c99 	.word	0x00004c99
30009810:	6a09e667 	.word	0x6a09e667
30009814:	f3bcc908 	.word	0xf3bcc908
30009818:	fe94f82b 	.word	0xfe94f82b
3000981c:	fb41bd6b 	.word	0xfb41bd6b
30009820:	3c6ef372 	.word	0x3c6ef372
30009824:	5be0cd19 	.word	0x5be0cd19
30009828:	cbbb9d5d 	.word	0xcbbb9d5d
3000982c:	f70e5939 	.word	0xf70e5939
30009830:	ffc00b31 	.word	0xffc00b31
30009834:	64f98fa7 	.word	0x64f98fa7
30009838:	629a292a 	.word	0x629a292a
3000983c:	9159015a 	.word	0x9159015a
30009840:	152fecd8 	.word	0x152fecd8
30009844:	67332667 	.word	0x67332667
30009848:	8eb44a87 	.word	0x8eb44a87
3000984c:	db0c2e0d 	.word	0xdb0c2e0d
30009850:	47b5481d 	.word	0x47b5481d
30009854:	367cd507 	.word	0x367cd507
30009858:	c1059ed8 	.word	0xc1059ed8
3000985c:	befa4fa4 	.word	0xbefa4fa4
30009860:	137e2179 	.word	0x137e2179

30009864 <CPU_ClkGet>:
30009864:	b570      	push	{r4, r5, r6, lr}
30009866:	4d1e      	ldr	r5, [pc, #120]	; (300098e0 <CPU_ClkGet+0x7c>)
30009868:	4b1e      	ldr	r3, [pc, #120]	; (300098e4 <CPU_ClkGet+0x80>)
3000986a:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
3000986e:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009872:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009876:	4798      	blx	r3
30009878:	b92c      	cbnz	r4, 30009886 <CPU_ClkGet+0x22>
3000987a:	2801      	cmp	r0, #1
3000987c:	d007      	beq.n	3000988e <CPU_ClkGet+0x2a>
3000987e:	07b2      	lsls	r2, r6, #30
30009880:	d407      	bmi.n	30009892 <CPU_ClkGet+0x2e>
30009882:	4819      	ldr	r0, [pc, #100]	; (300098e8 <CPU_ClkGet+0x84>)
30009884:	bd70      	pop	{r4, r5, r6, pc}
30009886:	2c01      	cmp	r4, #1
30009888:	d007      	beq.n	3000989a <CPU_ClkGet+0x36>
3000988a:	2000      	movs	r0, #0
3000988c:	bd70      	pop	{r4, r5, r6, pc}
3000988e:	4817      	ldr	r0, [pc, #92]	; (300098ec <CPU_ClkGet+0x88>)
30009890:	bd70      	pop	{r4, r5, r6, pc}
30009892:	4b17      	ldr	r3, [pc, #92]	; (300098f0 <CPU_ClkGet+0x8c>)
30009894:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009898:	4718      	bx	r3
3000989a:	2801      	cmp	r0, #1
3000989c:	d011      	beq.n	300098c2 <CPU_ClkGet+0x5e>
3000989e:	05f3      	lsls	r3, r6, #23
300098a0:	d51c      	bpl.n	300098dc <CPU_ClkGet+0x78>
300098a2:	4914      	ldr	r1, [pc, #80]	; (300098f4 <CPU_ClkGet+0x90>)
300098a4:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
300098a8:	6c48      	ldr	r0, [r1, #68]	; 0x44
300098aa:	4a13      	ldr	r2, [pc, #76]	; (300098f8 <CPU_ClkGet+0x94>)
300098ac:	f003 0307 	and.w	r3, r3, #7
300098b0:	f3c0 1045 	ubfx	r0, r0, #5, #6
300098b4:	3301      	adds	r3, #1
300098b6:	3002      	adds	r0, #2
300098b8:	fb02 f000 	mul.w	r0, r2, r0
300098bc:	fbb0 f0f3 	udiv	r0, r0, r3
300098c0:	bd70      	pop	{r4, r5, r6, pc}
300098c2:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
300098c6:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
300098ca:	f3c3 4007 	ubfx	r0, r3, #16, #8
300098ce:	d003      	beq.n	300098d8 <CPU_ClkGet+0x74>
300098d0:	4b0a      	ldr	r3, [pc, #40]	; (300098fc <CPU_ClkGet+0x98>)
300098d2:	fb03 f000 	mul.w	r0, r3, r0
300098d6:	bd70      	pop	{r4, r5, r6, pc}
300098d8:	4809      	ldr	r0, [pc, #36]	; (30009900 <CPU_ClkGet+0x9c>)
300098da:	bd70      	pop	{r4, r5, r6, pc}
300098dc:	4806      	ldr	r0, [pc, #24]	; (300098f8 <CPU_ClkGet+0x94>)
300098de:	bd70      	pop	{r4, r5, r6, pc}
300098e0:	42008000 	.word	0x42008000
300098e4:	0000c0f9 	.word	0x0000c0f9
300098e8:	003d0900 	.word	0x003d0900
300098ec:	01312d00 	.word	0x01312d00
300098f0:	000099f5 	.word	0x000099f5
300098f4:	42008800 	.word	0x42008800
300098f8:	02625a00 	.word	0x02625a00
300098fc:	000f4240 	.word	0x000f4240
30009900:	02faf080 	.word	0x02faf080

30009904 <RSIP_IV_Set>:
30009904:	4b05      	ldr	r3, [pc, #20]	; (3000991c <RSIP_IV_Set+0x18>)
30009906:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
3000990a:	680b      	ldr	r3, [r1, #0]
3000990c:	ba1b      	rev	r3, r3
3000990e:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009912:	684b      	ldr	r3, [r1, #4]
30009914:	ba1b      	rev	r3, r3
30009916:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
3000991a:	4770      	bx	lr
3000991c:	42008c00 	.word	0x42008c00

30009920 <RSIP_OTF_Cmd>:
30009920:	4a04      	ldr	r2, [pc, #16]	; (30009934 <RSIP_OTF_Cmd+0x14>)
30009922:	2801      	cmp	r0, #1
30009924:	6953      	ldr	r3, [r2, #20]
30009926:	bf0c      	ite	eq
30009928:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
3000992c:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
30009930:	6153      	str	r3, [r2, #20]
30009932:	4770      	bx	lr
30009934:	42008c00 	.word	0x42008c00

30009938 <RSIP_OTF_Enable>:
30009938:	b570      	push	{r4, r5, r6, lr}
3000993a:	4604      	mov	r4, r0
3000993c:	b1f3      	cbz	r3, 3000997c <RSIP_OTF_Enable+0x44>
3000993e:	9b04      	ldr	r3, [sp, #16]
30009940:	460d      	mov	r5, r1
30009942:	4616      	mov	r6, r2
30009944:	2b07      	cmp	r3, #7
30009946:	d827      	bhi.n	30009998 <RSIP_OTF_Enable+0x60>
30009948:	9b05      	ldr	r3, [sp, #20]
3000994a:	3b01      	subs	r3, #1
3000994c:	2b01      	cmp	r3, #1
3000994e:	d81d      	bhi.n	3000998c <RSIP_OTF_Enable+0x54>
30009950:	9a04      	ldr	r2, [sp, #16]
30009952:	9b05      	ldr	r3, [sp, #20]
30009954:	f002 0007 	and.w	r0, r2, #7
30009958:	005b      	lsls	r3, r3, #1
3000995a:	4305      	orrs	r5, r0
3000995c:	4813      	ldr	r0, [pc, #76]	; (300099ac <RSIP_OTF_Enable+0x74>)
3000995e:	f003 0306 	and.w	r3, r3, #6
30009962:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009966:	ea43 0206 	orr.w	r2, r3, r6
3000996a:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000996c:	f042 0201 	orr.w	r2, r2, #1
30009970:	f023 0301 	bic.w	r3, r3, #1
30009974:	6463      	str	r3, [r4, #68]	; 0x44
30009976:	6425      	str	r5, [r4, #64]	; 0x40
30009978:	6462      	str	r2, [r4, #68]	; 0x44
3000997a:	bd70      	pop	{r4, r5, r6, pc}
3000997c:	480b      	ldr	r0, [pc, #44]	; (300099ac <RSIP_OTF_Enable+0x74>)
3000997e:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009982:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009984:	f023 0301 	bic.w	r3, r3, #1
30009988:	6463      	str	r3, [r4, #68]	; 0x44
3000998a:	bd70      	pop	{r4, r5, r6, pc}
3000998c:	f240 2131 	movw	r1, #561	; 0x231
30009990:	4807      	ldr	r0, [pc, #28]	; (300099b0 <RSIP_OTF_Enable+0x78>)
30009992:	f002 fdd1 	bl	3000c538 <__io_assert_failed_veneer>
30009996:	e7db      	b.n	30009950 <RSIP_OTF_Enable+0x18>
30009998:	f44f 710c 	mov.w	r1, #560	; 0x230
3000999c:	4804      	ldr	r0, [pc, #16]	; (300099b0 <RSIP_OTF_Enable+0x78>)
3000999e:	f002 fdcb 	bl	3000c538 <__io_assert_failed_veneer>
300099a2:	9b05      	ldr	r3, [sp, #20]
300099a4:	3b01      	subs	r3, #1
300099a6:	2b01      	cmp	r3, #1
300099a8:	d9d2      	bls.n	30009950 <RSIP_OTF_Enable+0x18>
300099aa:	e7ef      	b.n	3000998c <RSIP_OTF_Enable+0x54>
300099ac:	42008c00 	.word	0x42008c00
300099b0:	3000d89c 	.word	0x3000d89c

300099b4 <RSIP_MMU_Config>:
300099b4:	eb00 0040 	add.w	r0, r0, r0, lsl #1
300099b8:	311f      	adds	r1, #31
300099ba:	321f      	adds	r2, #31
300099bc:	331f      	adds	r3, #31
300099be:	0080      	lsls	r0, r0, #2
300099c0:	0949      	lsrs	r1, r1, #5
300099c2:	0952      	lsrs	r2, r2, #5
300099c4:	095b      	lsrs	r3, r3, #5
300099c6:	0249      	lsls	r1, r1, #9
300099c8:	0252      	lsls	r2, r2, #9
300099ca:	025b      	lsls	r3, r3, #9
300099cc:	b410      	push	{r4}
300099ce:	4c08      	ldr	r4, [pc, #32]	; (300099f0 <RSIP_MMU_Config+0x3c>)
300099d0:	4404      	add	r4, r0
300099d2:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
300099d6:	f020 0001 	bic.w	r0, r0, #1
300099da:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
300099de:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
300099e2:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
300099e6:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
300099ea:	f85d 4b04 	ldr.w	r4, [sp], #4
300099ee:	4770      	bx	lr
300099f0:	42008c00 	.word	0x42008c00

300099f4 <RSIP_MMU_Cmd>:
300099f4:	eb00 0040 	add.w	r0, r0, r0, lsl #1
300099f8:	4b07      	ldr	r3, [pc, #28]	; (30009a18 <RSIP_MMU_Cmd+0x24>)
300099fa:	0080      	lsls	r0, r0, #2
300099fc:	4403      	add	r3, r0
300099fe:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
30009a02:	b121      	cbz	r1, 30009a0e <RSIP_MMU_Cmd+0x1a>
30009a04:	f042 0201 	orr.w	r2, r2, #1
30009a08:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009a0c:	4770      	bx	lr
30009a0e:	f022 0201 	bic.w	r2, r2, #1
30009a12:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30009a16:	4770      	bx	lr
30009a18:	42008c00 	.word	0x42008c00

30009a1c <RSIP_MMU_Cache_Clean>:
30009a1c:	4a02      	ldr	r2, [pc, #8]	; (30009a28 <RSIP_MMU_Cache_Clean+0xc>)
30009a1e:	6993      	ldr	r3, [r2, #24]
30009a20:	f043 0304 	orr.w	r3, r3, #4
30009a24:	6193      	str	r3, [r2, #24]
30009a26:	4770      	bx	lr
30009a28:	42008c00 	.word	0x42008c00

30009a2c <NAND_RxData>:
30009a2c:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30009a30:	460c      	mov	r4, r1
30009a32:	4616      	mov	r6, r2
30009a34:	461f      	mov	r7, r3
30009a36:	2800      	cmp	r0, #0
30009a38:	f000 808a 	beq.w	30009b50 <NAND_RxData+0x124>
30009a3c:	4605      	mov	r5, r0
30009a3e:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009b5c <NAND_RxData+0x130>
30009a42:	2000      	movs	r0, #0
30009a44:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009b60 <NAND_RxData+0x134>
30009a48:	47c0      	blx	r8
30009a4a:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
30009a4e:	2000      	movs	r0, #0
30009a50:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
30009a54:	6819      	ldr	r1, [r3, #0]
30009a56:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
30009a5a:	6019      	str	r1, [r3, #0]
30009a5c:	6819      	ldr	r1, [r3, #0]
30009a5e:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30009a62:	6019      	str	r1, [r3, #0]
30009a64:	605a      	str	r2, [r3, #4]
30009a66:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009a6a:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009a6e:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30009a72:	ea4f 420c 	mov.w	r2, ip, lsl #16
30009a76:	f1bc 0f03 	cmp.w	ip, #3
30009a7a:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009a7e:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009a82:	ea42 0201 	orr.w	r2, r2, r1
30009a86:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009a8a:	f04f 0201 	mov.w	r2, #1
30009a8e:	611a      	str	r2, [r3, #16]
30009a90:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30009a94:	bf08      	it	eq
30009a96:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009a9a:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009a9e:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30009aa2:	b2e4      	uxtb	r4, r4
30009aa4:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009aa8:	2301      	movs	r3, #1
30009aaa:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009aae:	f017 0403 	ands.w	r4, r7, #3
30009ab2:	6083      	str	r3, [r0, #8]
30009ab4:	d137      	bne.n	30009b26 <NAND_RxData+0xfa>
30009ab6:	463b      	mov	r3, r7
30009ab8:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009abc:	42b4      	cmp	r4, r6
30009abe:	d216      	bcs.n	30009aee <NAND_RxData+0xc2>
30009ac0:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30009ac4:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009ac8:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009acc:	d00c      	beq.n	30009ae8 <NAND_RxData+0xbc>
30009ace:	00ad      	lsls	r5, r5, #2
30009ad0:	1f19      	subs	r1, r3, #4
30009ad2:	f1a5 0c04 	sub.w	ip, r5, #4
30009ad6:	449c      	add	ip, r3
30009ad8:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009adc:	f841 0f04 	str.w	r0, [r1, #4]!
30009ae0:	458c      	cmp	ip, r1
30009ae2:	d1f9      	bne.n	30009ad8 <NAND_RxData+0xac>
30009ae4:	442b      	add	r3, r5
30009ae6:	442c      	add	r4, r5
30009ae8:	1b32      	subs	r2, r6, r4
30009aea:	2a03      	cmp	r2, #3
30009aec:	d8e6      	bhi.n	30009abc <NAND_RxData+0x90>
30009aee:	1e63      	subs	r3, r4, #1
30009af0:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009af4:	441f      	add	r7, r3
30009af6:	e002      	b.n	30009afe <NAND_RxData+0xd2>
30009af8:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009afa:	071b      	lsls	r3, r3, #28
30009afc:	d40d      	bmi.n	30009b1a <NAND_RxData+0xee>
30009afe:	42b4      	cmp	r4, r6
30009b00:	d3fa      	bcc.n	30009af8 <NAND_RxData+0xcc>
30009b02:	2004      	movs	r0, #4
30009b04:	47c0      	blx	r8
30009b06:	2000      	movs	r0, #0
30009b08:	47c0      	blx	r8
30009b0a:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009b0e:	6813      	ldr	r3, [r2, #0]
30009b10:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009b14:	6013      	str	r3, [r2, #0]
30009b16:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009b1a:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009b1e:	3401      	adds	r4, #1
30009b20:	f807 3f01 	strb.w	r3, [r7, #1]!
30009b24:	e7eb      	b.n	30009afe <NAND_RxData+0xd2>
30009b26:	f1c4 0404 	rsb	r4, r4, #4
30009b2a:	2200      	movs	r2, #0
30009b2c:	4639      	mov	r1, r7
30009b2e:	e00a      	b.n	30009b46 <NAND_RxData+0x11a>
30009b30:	6a85      	ldr	r5, [r0, #40]	; 0x28
30009b32:	072d      	lsls	r5, r5, #28
30009b34:	d505      	bpl.n	30009b42 <NAND_RxData+0x116>
30009b36:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009b3a:	3201      	adds	r2, #1
30009b3c:	700b      	strb	r3, [r1, #0]
30009b3e:	18b9      	adds	r1, r7, r2
30009b40:	460b      	mov	r3, r1
30009b42:	4294      	cmp	r4, r2
30009b44:	d0b8      	beq.n	30009ab8 <NAND_RxData+0x8c>
30009b46:	42b2      	cmp	r2, r6
30009b48:	460b      	mov	r3, r1
30009b4a:	d3f1      	bcc.n	30009b30 <NAND_RxData+0x104>
30009b4c:	4614      	mov	r4, r2
30009b4e:	e7ce      	b.n	30009aee <NAND_RxData+0xc2>
30009b50:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009b5c <NAND_RxData+0x130>
30009b54:	f899 5008 	ldrb.w	r5, [r9, #8]
30009b58:	e773      	b.n	30009a42 <NAND_RxData+0x16>
30009b5a:	bf00      	nop
30009b5c:	2001c01c 	.word	0x2001c01c
30009b60:	0000b9c1 	.word	0x0000b9c1

30009b64 <NAND_Page_Read>:
30009b64:	b530      	push	{r4, r5, lr}
30009b66:	b085      	sub	sp, #20
30009b68:	4c07      	ldr	r4, [pc, #28]	; (30009b88 <NAND_Page_Read+0x24>)
30009b6a:	4d08      	ldr	r5, [pc, #32]	; (30009b8c <NAND_Page_Read+0x28>)
30009b6c:	9301      	str	r3, [sp, #4]
30009b6e:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009b72:	47a0      	blx	r4
30009b74:	4604      	mov	r4, r0
30009b76:	9903      	ldr	r1, [sp, #12]
30009b78:	7a28      	ldrb	r0, [r5, #8]
30009b7a:	4d05      	ldr	r5, [pc, #20]	; (30009b90 <NAND_Page_Read+0x2c>)
30009b7c:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009b80:	47a8      	blx	r5
30009b82:	4620      	mov	r0, r4
30009b84:	b005      	add	sp, #20
30009b86:	bd30      	pop	{r4, r5, pc}
30009b88:	0000bd85 	.word	0x0000bd85
30009b8c:	2001c01c 	.word	0x2001c01c
30009b90:	30009a2d 	.word	0x30009a2d

30009b94 <irq_register>:
30009b94:	b530      	push	{r4, r5, lr}
30009b96:	461c      	mov	r4, r3
30009b98:	460d      	mov	r5, r1
30009b9a:	f040 0001 	orr.w	r0, r0, #1
30009b9e:	4916      	ldr	r1, [pc, #88]	; (30009bf8 <irq_register+0x64>)
30009ba0:	4b16      	ldr	r3, [pc, #88]	; (30009bfc <irq_register+0x68>)
30009ba2:	2c07      	cmp	r4, #7
30009ba4:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
30009ba8:	b085      	sub	sp, #20
30009baa:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
30009bae:	d813      	bhi.n	30009bd8 <irq_register+0x44>
30009bb0:	2c07      	cmp	r4, #7
30009bb2:	bf28      	it	cs
30009bb4:	2407      	movcs	r4, #7
30009bb6:	2d00      	cmp	r5, #0
30009bb8:	ea4f 1444 	mov.w	r4, r4, lsl #5
30009bbc:	b2e4      	uxtb	r4, r4
30009bbe:	db04      	blt.n	30009bca <irq_register+0x36>
30009bc0:	4b0f      	ldr	r3, [pc, #60]	; (30009c00 <irq_register+0x6c>)
30009bc2:	2001      	movs	r0, #1
30009bc4:	555c      	strb	r4, [r3, r5]
30009bc6:	b005      	add	sp, #20
30009bc8:	bd30      	pop	{r4, r5, pc}
30009bca:	f005 050f 	and.w	r5, r5, #15
30009bce:	4b0d      	ldr	r3, [pc, #52]	; (30009c04 <irq_register+0x70>)
30009bd0:	2001      	movs	r0, #1
30009bd2:	555c      	strb	r4, [r3, r5]
30009bd4:	b005      	add	sp, #20
30009bd6:	bd30      	pop	{r4, r5, pc}
30009bd8:	4b0b      	ldr	r3, [pc, #44]	; (30009c08 <irq_register+0x74>)
30009bda:	2107      	movs	r1, #7
30009bdc:	4a0b      	ldr	r2, [pc, #44]	; (30009c0c <irq_register+0x78>)
30009bde:	2003      	movs	r0, #3
30009be0:	9500      	str	r5, [sp, #0]
30009be2:	e9cd 4101 	strd	r4, r1, [sp, #4]
30009be6:	4619      	mov	r1, r3
30009be8:	f002 f9b0 	bl	3000bf4c <rtk_log_write>
30009bec:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30009bf0:	4807      	ldr	r0, [pc, #28]	; (30009c10 <irq_register+0x7c>)
30009bf2:	f002 fca1 	bl	3000c538 <__io_assert_failed_veneer>
30009bf6:	e7db      	b.n	30009bb0 <irq_register+0x1c>
30009bf8:	30000184 	.word	0x30000184
30009bfc:	300002c4 	.word	0x300002c4
30009c00:	e000e400 	.word	0xe000e400
30009c04:	e000ed14 	.word	0xe000ed14
30009c08:	3000d82c 	.word	0x3000d82c
30009c0c:	3000d860 	.word	0x3000d860
30009c10:	3000d888 	.word	0x3000d888

30009c14 <SYSCFG_BootFromNor>:
30009c14:	4b0e      	ldr	r3, [pc, #56]	; (30009c50 <SYSCFG_BootFromNor+0x3c>)
30009c16:	b510      	push	{r4, lr}
30009c18:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009c1c:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009c20:	0392      	lsls	r2, r2, #14
30009c22:	d50a      	bpl.n	30009c3a <SYSCFG_BootFromNor+0x26>
30009c24:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009c28:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009c2c:	d105      	bne.n	30009c3a <SYSCFG_BootFromNor+0x26>
30009c2e:	4b09      	ldr	r3, [pc, #36]	; (30009c54 <SYSCFG_BootFromNor+0x40>)
30009c30:	4798      	blx	r3
30009c32:	3800      	subs	r0, #0
30009c34:	bf18      	it	ne
30009c36:	2001      	movne	r0, #1
30009c38:	bd10      	pop	{r4, pc}
30009c3a:	4c07      	ldr	r4, [pc, #28]	; (30009c58 <SYSCFG_BootFromNor+0x44>)
30009c3c:	47a0      	blx	r4
30009c3e:	2802      	cmp	r0, #2
30009c40:	d004      	beq.n	30009c4c <SYSCFG_BootFromNor+0x38>
30009c42:	47a0      	blx	r4
30009c44:	2801      	cmp	r0, #1
30009c46:	d1f2      	bne.n	30009c2e <SYSCFG_BootFromNor+0x1a>
30009c48:	2000      	movs	r0, #0
30009c4a:	bd10      	pop	{r4, pc}
30009c4c:	2001      	movs	r0, #1
30009c4e:	bd10      	pop	{r4, pc}
30009c50:	42008000 	.word	0x42008000
30009c54:	0000c0bd 	.word	0x0000c0bd
30009c58:	0000c1a9 	.word	0x0000c1a9

30009c5c <SYSCFG_RLVersion>:
30009c5c:	4b07      	ldr	r3, [pc, #28]	; (30009c7c <SYSCFG_RLVersion+0x20>)
30009c5e:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009c62:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009c66:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009c6a:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009c6e:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009c72:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009c76:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009c7a:	4770      	bx	lr
30009c7c:	42008000 	.word	0x42008000

30009c80 <SBOOT_GetMdType>:
30009c80:	b470      	push	{r4, r5, r6}
30009c82:	4c0c      	ldr	r4, [pc, #48]	; (30009cb4 <SBOOT_GetMdType+0x34>)
30009c84:	4e0c      	ldr	r6, [pc, #48]	; (30009cb8 <SBOOT_GetMdType+0x38>)
30009c86:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009c8a:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009c8e:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009c92:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009c96:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009c9a:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009c9e:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009ca2:	6033      	str	r3, [r6, #0]
30009ca4:	b913      	cbnz	r3, 30009cac <SBOOT_GetMdType+0x2c>
30009ca6:	bc70      	pop	{r4, r5, r6}
30009ca8:	f002 bc6e 	b.w	3000c588 <__SBOOT_GetMdType_A_veneer>
30009cac:	bc70      	pop	{r4, r5, r6}
30009cae:	f002 bc33 	b.w	3000c518 <__SBOOT_GetMdType_B_veneer>
30009cb2:	bf00      	nop
30009cb4:	42008000 	.word	0x42008000
30009cb8:	3000fd24 	.word	0x3000fd24

30009cbc <CRYPTO_OTPKey_SHA_Init>:
30009cbc:	4a0d      	ldr	r2, [pc, #52]	; (30009cf4 <CRYPTO_OTPKey_SHA_Init+0x38>)
30009cbe:	b410      	push	{r4}
30009cc0:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009cc4:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009cc8:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009ccc:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009cd0:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009cd4:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009cd8:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009cdc:	4a06      	ldr	r2, [pc, #24]	; (30009cf8 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009cde:	6013      	str	r3, [r2, #0]
30009ce0:	b91b      	cbnz	r3, 30009cea <CRYPTO_OTPKey_SHA_Init+0x2e>
30009ce2:	f85d 4b04 	ldr.w	r4, [sp], #4
30009ce6:	f002 bb97 	b.w	3000c418 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009cea:	f85d 4b04 	ldr.w	r4, [sp], #4
30009cee:	f002 bb6b 	b.w	3000c3c8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30009cf2:	bf00      	nop
30009cf4:	42008000 	.word	0x42008000
30009cf8:	3000fd24 	.word	0x3000fd24

30009cfc <rtl_crypto_hmac_sha2_update>:
30009cfc:	b470      	push	{r4, r5, r6}
30009cfe:	4c0c      	ldr	r4, [pc, #48]	; (30009d30 <rtl_crypto_hmac_sha2_update+0x34>)
30009d00:	4e0c      	ldr	r6, [pc, #48]	; (30009d34 <rtl_crypto_hmac_sha2_update+0x38>)
30009d02:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009d06:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009d0a:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009d0e:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009d12:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009d16:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009d1a:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d1e:	6033      	str	r3, [r6, #0]
30009d20:	b913      	cbnz	r3, 30009d28 <rtl_crypto_hmac_sha2_update+0x2c>
30009d22:	bc70      	pop	{r4, r5, r6}
30009d24:	f002 bbe0 	b.w	3000c4e8 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009d28:	bc70      	pop	{r4, r5, r6}
30009d2a:	f002 bb95 	b.w	3000c458 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009d2e:	bf00      	nop
30009d30:	42008000 	.word	0x42008000
30009d34:	3000fd24 	.word	0x3000fd24

30009d38 <rtl_crypto_hmac_sha2_final>:
30009d38:	4a0d      	ldr	r2, [pc, #52]	; (30009d70 <rtl_crypto_hmac_sha2_final+0x38>)
30009d3a:	b410      	push	{r4}
30009d3c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009d40:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009d44:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009d48:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009d4c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009d50:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009d54:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d58:	4a06      	ldr	r2, [pc, #24]	; (30009d74 <rtl_crypto_hmac_sha2_final+0x3c>)
30009d5a:	6013      	str	r3, [r2, #0]
30009d5c:	b91b      	cbnz	r3, 30009d66 <rtl_crypto_hmac_sha2_final+0x2e>
30009d5e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d62:	f002 bc01 	b.w	3000c568 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009d66:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d6a:	f002 bc15 	b.w	3000c598 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009d6e:	bf00      	nop
30009d70:	42008000 	.word	0x42008000
30009d74:	3000fd24 	.word	0x3000fd24

30009d78 <rtl_crypto_sha2_init>:
30009d78:	4a0d      	ldr	r2, [pc, #52]	; (30009db0 <rtl_crypto_sha2_init+0x38>)
30009d7a:	b410      	push	{r4}
30009d7c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009d80:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009d84:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009d88:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009d8c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009d90:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009d94:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d98:	4a06      	ldr	r2, [pc, #24]	; (30009db4 <rtl_crypto_sha2_init+0x3c>)
30009d9a:	6013      	str	r3, [r2, #0]
30009d9c:	b91b      	cbnz	r3, 30009da6 <rtl_crypto_sha2_init+0x2e>
30009d9e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009da2:	f002 bbb1 	b.w	3000c508 <__rtl_crypto_sha2_init_A_veneer>
30009da6:	f85d 4b04 	ldr.w	r4, [sp], #4
30009daa:	f002 bbcd 	b.w	3000c548 <__rtl_crypto_sha2_init_B_veneer>
30009dae:	bf00      	nop
30009db0:	42008000 	.word	0x42008000
30009db4:	3000fd24 	.word	0x3000fd24

30009db8 <rtl_crypto_sha2_update>:
30009db8:	b470      	push	{r4, r5, r6}
30009dba:	4c0c      	ldr	r4, [pc, #48]	; (30009dec <rtl_crypto_sha2_update+0x34>)
30009dbc:	4e0c      	ldr	r6, [pc, #48]	; (30009df0 <rtl_crypto_sha2_update+0x38>)
30009dbe:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009dc2:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009dc6:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009dca:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009dce:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009dd2:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009dd6:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009dda:	6033      	str	r3, [r6, #0]
30009ddc:	b913      	cbnz	r3, 30009de4 <rtl_crypto_sha2_update+0x2c>
30009dde:	bc70      	pop	{r4, r5, r6}
30009de0:	f002 bbea 	b.w	3000c5b8 <__rtl_crypto_sha2_update_A_veneer>
30009de4:	bc70      	pop	{r4, r5, r6}
30009de6:	f002 bbdf 	b.w	3000c5a8 <__rtl_crypto_sha2_update_B_veneer>
30009dea:	bf00      	nop
30009dec:	42008000 	.word	0x42008000
30009df0:	3000fd24 	.word	0x3000fd24

30009df4 <rtl_crypto_sha2_final>:
30009df4:	4a0d      	ldr	r2, [pc, #52]	; (30009e2c <rtl_crypto_sha2_final+0x38>)
30009df6:	b410      	push	{r4}
30009df8:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009dfc:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009e00:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009e04:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009e08:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009e0c:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009e10:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009e14:	4a06      	ldr	r2, [pc, #24]	; (30009e30 <rtl_crypto_sha2_final+0x3c>)
30009e16:	6013      	str	r3, [r2, #0]
30009e18:	b91b      	cbnz	r3, 30009e22 <rtl_crypto_sha2_final+0x2e>
30009e1a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009e1e:	f002 bb83 	b.w	3000c528 <__rtl_crypto_sha2_final_A_veneer>
30009e22:	f85d 4b04 	ldr.w	r4, [sp], #4
30009e26:	f002 baff 	b.w	3000c428 <__rtl_crypto_sha2_final_B_veneer>
30009e2a:	bf00      	nop
30009e2c:	42008000 	.word	0x42008000
30009e30:	3000fd24 	.word	0x3000fd24

30009e34 <SBOOT_Validate_PubKey>:
30009e34:	b470      	push	{r4, r5, r6}
30009e36:	4c0c      	ldr	r4, [pc, #48]	; (30009e68 <SBOOT_Validate_PubKey+0x34>)
30009e38:	4e0c      	ldr	r6, [pc, #48]	; (30009e6c <SBOOT_Validate_PubKey+0x38>)
30009e3a:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009e3e:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009e42:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009e46:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009e4a:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009e4e:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009e52:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009e56:	6033      	str	r3, [r6, #0]
30009e58:	b913      	cbnz	r3, 30009e60 <SBOOT_Validate_PubKey+0x2c>
30009e5a:	bc70      	pop	{r4, r5, r6}
30009e5c:	f002 bb24 	b.w	3000c4a8 <__SBOOT_Validate_PubKey_A_veneer>
30009e60:	bc70      	pop	{r4, r5, r6}
30009e62:	f002 bb11 	b.w	3000c488 <__SBOOT_Validate_PubKey_B_veneer>
30009e66:	bf00      	nop
30009e68:	42008000 	.word	0x42008000
30009e6c:	3000fd24 	.word	0x3000fd24

30009e70 <SBOOT_Validate_Signature>:
30009e70:	b5f0      	push	{r4, r5, r6, r7, lr}
30009e72:	4d10      	ldr	r5, [pc, #64]	; (30009eb4 <SBOOT_Validate_Signature+0x44>)
30009e74:	9f05      	ldr	r7, [sp, #20]
30009e76:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009e7a:	f8dd c018 	ldr.w	ip, [sp, #24]
30009e7e:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009e82:	f8df e034 	ldr.w	lr, [pc, #52]	; 30009eb8 <SBOOT_Validate_Signature+0x48>
30009e86:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009e8a:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009e8e:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009e92:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009e96:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009e9a:	e9cd 7c05 	strd	r7, ip, [sp, #20]
30009e9e:	f8ce 4000 	str.w	r4, [lr]
30009ea2:	b91c      	cbnz	r4, 30009eac <SBOOT_Validate_Signature+0x3c>
30009ea4:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009ea8:	f002 bb26 	b.w	3000c4f8 <__SBOOT_Validate_Signature_A_veneer>
30009eac:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009eb0:	f002 baf2 	b.w	3000c498 <__SBOOT_Validate_Signature_B_veneer>
30009eb4:	42008000 	.word	0x42008000
30009eb8:	3000fd24 	.word	0x3000fd24

30009ebc <SBOOT_Validate_ImgHash>:
30009ebc:	b4f0      	push	{r4, r5, r6, r7}
30009ebe:	4d0c      	ldr	r5, [pc, #48]	; (30009ef0 <SBOOT_Validate_ImgHash+0x34>)
30009ec0:	4f0c      	ldr	r7, [pc, #48]	; (30009ef4 <SBOOT_Validate_ImgHash+0x38>)
30009ec2:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009ec6:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009eca:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009ece:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009ed2:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009ed6:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009eda:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009ede:	603c      	str	r4, [r7, #0]
30009ee0:	b914      	cbnz	r4, 30009ee8 <SBOOT_Validate_ImgHash+0x2c>
30009ee2:	bcf0      	pop	{r4, r5, r6, r7}
30009ee4:	f002 bb38 	b.w	3000c558 <__SBOOT_Validate_ImgHash_A_veneer>
30009ee8:	bcf0      	pop	{r4, r5, r6, r7}
30009eea:	f002 bae5 	b.w	3000c4b8 <__SBOOT_Validate_ImgHash_B_veneer>
30009eee:	bf00      	nop
30009ef0:	42008000 	.word	0x42008000
30009ef4:	3000fd24 	.word	0x3000fd24

30009ef8 <CRYPTO_SHA_Init>:
30009ef8:	4b0b      	ldr	r3, [pc, #44]	; (30009f28 <CRYPTO_SHA_Init+0x30>)
30009efa:	490c      	ldr	r1, [pc, #48]	; (30009f2c <CRYPTO_SHA_Init+0x34>)
30009efc:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009f00:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009f04:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
30009f08:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
30009f0c:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009f10:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009f14:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009f18:	6008      	str	r0, [r1, #0]
30009f1a:	b908      	cbnz	r0, 30009f20 <CRYPTO_SHA_Init+0x28>
30009f1c:	f002 bb2c 	b.w	3000c578 <__CRYPTO_SHA_Init_A_veneer>
30009f20:	2000      	movs	r0, #0
30009f22:	f002 ba71 	b.w	3000c408 <__CRYPTO_SHA_Init_B_veneer>
30009f26:	bf00      	nop
30009f28:	42008000 	.word	0x42008000
30009f2c:	3000fd24 	.word	0x3000fd24

30009f30 <SBOOT_Validate_Algorithm>:
30009f30:	b4f0      	push	{r4, r5, r6, r7}
30009f32:	4d0c      	ldr	r5, [pc, #48]	; (30009f64 <SBOOT_Validate_Algorithm+0x34>)
30009f34:	4f0c      	ldr	r7, [pc, #48]	; (30009f68 <SBOOT_Validate_Algorithm+0x38>)
30009f36:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009f3a:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009f3e:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009f42:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009f46:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009f4a:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009f4e:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009f52:	603c      	str	r4, [r7, #0]
30009f54:	b914      	cbnz	r4, 30009f5c <SBOOT_Validate_Algorithm+0x2c>
30009f56:	bcf0      	pop	{r4, r5, r6, r7}
30009f58:	f002 ba76 	b.w	3000c448 <__SBOOT_Validate_Algorithm_A_veneer>
30009f5c:	bcf0      	pop	{r4, r5, r6, r7}
30009f5e:	f002 ba83 	b.w	3000c468 <__SBOOT_Validate_Algorithm_B_veneer>
30009f62:	bf00      	nop
30009f64:	42008000 	.word	0x42008000
30009f68:	3000fd24 	.word	0x3000fd24

30009f6c <ddr_init_index>:
30009f6c:	b570      	push	{r4, r5, r6, lr}
30009f6e:	4b0f      	ldr	r3, [pc, #60]	; (30009fac <ddr_init_index+0x40>)
30009f70:	4798      	blx	r3
30009f72:	4604      	mov	r4, r0
30009f74:	4b0e      	ldr	r3, [pc, #56]	; (30009fb0 <ddr_init_index+0x44>)
30009f76:	4798      	blx	r3
30009f78:	2c03      	cmp	r4, #3
30009f7a:	d014      	beq.n	30009fa6 <ddr_init_index+0x3a>
30009f7c:	2c01      	cmp	r4, #1
30009f7e:	d00c      	beq.n	30009f9a <ddr_init_index+0x2e>
30009f80:	4e0c      	ldr	r6, [pc, #48]	; (30009fb4 <ddr_init_index+0x48>)
30009f82:	4d0d      	ldr	r5, [pc, #52]	; (30009fb8 <ddr_init_index+0x4c>)
30009f84:	4c0d      	ldr	r4, [pc, #52]	; (30009fbc <ddr_init_index+0x50>)
30009f86:	462b      	mov	r3, r5
30009f88:	4622      	mov	r2, r4
30009f8a:	490b      	ldr	r1, [pc, #44]	; (30009fb8 <ddr_init_index+0x4c>)
30009f8c:	2002      	movs	r0, #2
30009f8e:	f001 ffdd 	bl	3000bf4c <rtk_log_write>
30009f92:	f242 7010 	movw	r0, #10000	; 0x2710
30009f96:	47b0      	blx	r6
30009f98:	e7f5      	b.n	30009f86 <ddr_init_index+0x1a>
30009f9a:	2802      	cmp	r0, #2
30009f9c:	d003      	beq.n	30009fa6 <ddr_init_index+0x3a>
30009f9e:	2803      	cmp	r0, #3
30009fa0:	d1ee      	bne.n	30009f80 <ddr_init_index+0x14>
30009fa2:	2000      	movs	r0, #0
30009fa4:	bd70      	pop	{r4, r5, r6, pc}
30009fa6:	4620      	mov	r0, r4
30009fa8:	bd70      	pop	{r4, r5, r6, pc}
30009faa:	bf00      	nop
30009fac:	30007fa9 	.word	0x30007fa9
30009fb0:	30007e25 	.word	0x30007e25
30009fb4:	00009be5 	.word	0x00009be5
30009fb8:	3000d8e4 	.word	0x3000d8e4
30009fbc:	3000d8e8 	.word	0x3000d8e8

30009fc0 <rxi316_perf_tune>:
30009fc0:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009fc4:	4606      	mov	r6, r0
30009fc6:	6800      	ldr	r0, [r0, #0]
30009fc8:	b0c3      	sub	sp, #268	; 0x10c
30009fca:	7985      	ldrb	r5, [r0, #6]
30009fcc:	2d00      	cmp	r5, #0
30009fce:	f000 8334 	beq.w	3000a63a <rxi316_perf_tune+0x67a>
30009fd2:	2d01      	cmp	r5, #1
30009fd4:	d003      	beq.n	30009fde <rxi316_perf_tune+0x1e>
30009fd6:	2d03      	cmp	r5, #3
30009fd8:	bf14      	ite	ne
30009fda:	2502      	movne	r5, #2
30009fdc:	2500      	moveq	r5, #0
30009fde:	68f3      	ldr	r3, [r6, #12]
30009fe0:	2420      	movs	r4, #32
30009fe2:	4684      	mov	ip, r0
30009fe4:	6859      	ldr	r1, [r3, #4]
30009fe6:	46a3      	mov	fp, r4
30009fe8:	46a2      	mov	sl, r4
30009fea:	46a0      	mov	r8, r4
30009fec:	784b      	ldrb	r3, [r1, #1]
30009fee:	46a1      	mov	r9, r4
30009ff0:	7bcf      	ldrb	r7, [r1, #15]
30009ff2:	9323      	str	r3, [sp, #140]	; 0x8c
30009ff4:	788b      	ldrb	r3, [r1, #2]
30009ff6:	780a      	ldrb	r2, [r1, #0]
30009ff8:	9324      	str	r3, [sp, #144]	; 0x90
30009ffa:	78cb      	ldrb	r3, [r1, #3]
30009ffc:	9325      	str	r3, [sp, #148]	; 0x94
30009ffe:	790b      	ldrb	r3, [r1, #4]
3000a000:	9326      	str	r3, [sp, #152]	; 0x98
3000a002:	794b      	ldrb	r3, [r1, #5]
3000a004:	9327      	str	r3, [sp, #156]	; 0x9c
3000a006:	798b      	ldrb	r3, [r1, #6]
3000a008:	9328      	str	r3, [sp, #160]	; 0xa0
3000a00a:	79cb      	ldrb	r3, [r1, #7]
3000a00c:	9329      	str	r3, [sp, #164]	; 0xa4
3000a00e:	7a0b      	ldrb	r3, [r1, #8]
3000a010:	932a      	str	r3, [sp, #168]	; 0xa8
3000a012:	7a4b      	ldrb	r3, [r1, #9]
3000a014:	932b      	str	r3, [sp, #172]	; 0xac
3000a016:	7a8b      	ldrb	r3, [r1, #10]
3000a018:	932c      	str	r3, [sp, #176]	; 0xb0
3000a01a:	7acb      	ldrb	r3, [r1, #11]
3000a01c:	932d      	str	r3, [sp, #180]	; 0xb4
3000a01e:	7b0b      	ldrb	r3, [r1, #12]
3000a020:	932e      	str	r3, [sp, #184]	; 0xb8
3000a022:	7b4b      	ldrb	r3, [r1, #13]
3000a024:	932f      	str	r3, [sp, #188]	; 0xbc
3000a026:	7b8b      	ldrb	r3, [r1, #14]
3000a028:	9330      	str	r3, [sp, #192]	; 0xc0
3000a02a:	2300      	movs	r3, #0
3000a02c:	9731      	str	r7, [sp, #196]	; 0xc4
3000a02e:	7c0f      	ldrb	r7, [r1, #16]
3000a030:	941d      	str	r4, [sp, #116]	; 0x74
3000a032:	9732      	str	r7, [sp, #200]	; 0xc8
3000a034:	7c4f      	ldrb	r7, [r1, #17]
3000a036:	9733      	str	r7, [sp, #204]	; 0xcc
3000a038:	7c8f      	ldrb	r7, [r1, #18]
3000a03a:	9734      	str	r7, [sp, #208]	; 0xd0
3000a03c:	7ccf      	ldrb	r7, [r1, #19]
3000a03e:	9735      	str	r7, [sp, #212]	; 0xd4
3000a040:	7d0f      	ldrb	r7, [r1, #20]
3000a042:	9736      	str	r7, [sp, #216]	; 0xd8
3000a044:	7d4f      	ldrb	r7, [r1, #21]
3000a046:	9737      	str	r7, [sp, #220]	; 0xdc
3000a048:	7d8f      	ldrb	r7, [r1, #22]
3000a04a:	9738      	str	r7, [sp, #224]	; 0xe0
3000a04c:	7dcf      	ldrb	r7, [r1, #23]
3000a04e:	9739      	str	r7, [sp, #228]	; 0xe4
3000a050:	7e0f      	ldrb	r7, [r1, #24]
3000a052:	973a      	str	r7, [sp, #232]	; 0xe8
3000a054:	7e4f      	ldrb	r7, [r1, #25]
3000a056:	973b      	str	r7, [sp, #236]	; 0xec
3000a058:	7e8f      	ldrb	r7, [r1, #26]
3000a05a:	973c      	str	r7, [sp, #240]	; 0xf0
3000a05c:	7ecf      	ldrb	r7, [r1, #27]
3000a05e:	973d      	str	r7, [sp, #244]	; 0xf4
3000a060:	7f0f      	ldrb	r7, [r1, #28]
3000a062:	973e      	str	r7, [sp, #248]	; 0xf8
3000a064:	7f4f      	ldrb	r7, [r1, #29]
3000a066:	973f      	str	r7, [sp, #252]	; 0xfc
3000a068:	7f8f      	ldrb	r7, [r1, #30]
3000a06a:	7fc9      	ldrb	r1, [r1, #31]
3000a06c:	9740      	str	r7, [sp, #256]	; 0x100
3000a06e:	4627      	mov	r7, r4
3000a070:	9141      	str	r1, [sp, #260]	; 0x104
3000a072:	4619      	mov	r1, r3
3000a074:	9409      	str	r4, [sp, #36]	; 0x24
3000a076:	941c      	str	r4, [sp, #112]	; 0x70
3000a078:	9404      	str	r4, [sp, #16]
3000a07a:	941b      	str	r4, [sp, #108]	; 0x6c
3000a07c:	940a      	str	r4, [sp, #40]	; 0x28
3000a07e:	9303      	str	r3, [sp, #12]
3000a080:	e9cd 4405 	strd	r4, r4, [sp, #20]
3000a084:	e9cd 4401 	strd	r4, r4, [sp, #4]
3000a088:	e9cd 4407 	strd	r4, r4, [sp, #28]
3000a08c:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
3000a090:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
3000a094:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
3000a098:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
3000a09c:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
3000a0a0:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
3000a0a4:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
3000a0a8:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
3000a0ac:	b94a      	cbnz	r2, 3000a0c2 <rxi316_perf_tune+0x102>
3000a0ae:	3101      	adds	r1, #1
3000a0b0:	4699      	mov	r9, r3
3000a0b2:	3301      	adds	r3, #1
3000a0b4:	2b20      	cmp	r3, #32
3000a0b6:	d069      	beq.n	3000a18c <rxi316_perf_tune+0x1cc>
3000a0b8:	aa22      	add	r2, sp, #136	; 0x88
3000a0ba:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a0be:	2a00      	cmp	r2, #0
3000a0c0:	d0f5      	beq.n	3000a0ae <rxi316_perf_tune+0xee>
3000a0c2:	2a01      	cmp	r2, #1
3000a0c4:	d05d      	beq.n	3000a182 <rxi316_perf_tune+0x1c2>
3000a0c6:	2a02      	cmp	r2, #2
3000a0c8:	d06e      	beq.n	3000a1a8 <rxi316_perf_tune+0x1e8>
3000a0ca:	2a03      	cmp	r2, #3
3000a0cc:	d06f      	beq.n	3000a1ae <rxi316_perf_tune+0x1ee>
3000a0ce:	2a04      	cmp	r2, #4
3000a0d0:	d070      	beq.n	3000a1b4 <rxi316_perf_tune+0x1f4>
3000a0d2:	2a05      	cmp	r2, #5
3000a0d4:	d071      	beq.n	3000a1ba <rxi316_perf_tune+0x1fa>
3000a0d6:	2a06      	cmp	r2, #6
3000a0d8:	d072      	beq.n	3000a1c0 <rxi316_perf_tune+0x200>
3000a0da:	2a07      	cmp	r2, #7
3000a0dc:	f000 82af 	beq.w	3000a63e <rxi316_perf_tune+0x67e>
3000a0e0:	2a08      	cmp	r2, #8
3000a0e2:	f000 8381 	beq.w	3000a7e8 <rxi316_perf_tune+0x828>
3000a0e6:	2a09      	cmp	r2, #9
3000a0e8:	f000 8386 	beq.w	3000a7f8 <rxi316_perf_tune+0x838>
3000a0ec:	2a0a      	cmp	r2, #10
3000a0ee:	f000 838e 	beq.w	3000a80e <rxi316_perf_tune+0x84e>
3000a0f2:	2a0b      	cmp	r2, #11
3000a0f4:	f000 8386 	beq.w	3000a804 <rxi316_perf_tune+0x844>
3000a0f8:	2a0c      	cmp	r2, #12
3000a0fa:	f000 838d 	beq.w	3000a818 <rxi316_perf_tune+0x858>
3000a0fe:	2a0d      	cmp	r2, #13
3000a100:	f000 8394 	beq.w	3000a82c <rxi316_perf_tune+0x86c>
3000a104:	2a0e      	cmp	r2, #14
3000a106:	f000 838c 	beq.w	3000a822 <rxi316_perf_tune+0x862>
3000a10a:	2a0f      	cmp	r2, #15
3000a10c:	f000 8393 	beq.w	3000a836 <rxi316_perf_tune+0x876>
3000a110:	2a10      	cmp	r2, #16
3000a112:	f000 8395 	beq.w	3000a840 <rxi316_perf_tune+0x880>
3000a116:	2a11      	cmp	r2, #17
3000a118:	f000 8394 	beq.w	3000a844 <rxi316_perf_tune+0x884>
3000a11c:	2a12      	cmp	r2, #18
3000a11e:	f000 8393 	beq.w	3000a848 <rxi316_perf_tune+0x888>
3000a122:	2a13      	cmp	r2, #19
3000a124:	f000 8394 	beq.w	3000a850 <rxi316_perf_tune+0x890>
3000a128:	2a14      	cmp	r2, #20
3000a12a:	f000 838f 	beq.w	3000a84c <rxi316_perf_tune+0x88c>
3000a12e:	2a15      	cmp	r2, #21
3000a130:	f000 8396 	beq.w	3000a860 <rxi316_perf_tune+0x8a0>
3000a134:	2a16      	cmp	r2, #22
3000a136:	f000 8391 	beq.w	3000a85c <rxi316_perf_tune+0x89c>
3000a13a:	2a17      	cmp	r2, #23
3000a13c:	f000 83a0 	beq.w	3000a880 <rxi316_perf_tune+0x8c0>
3000a140:	2a18      	cmp	r2, #24
3000a142:	f000 839b 	beq.w	3000a87c <rxi316_perf_tune+0x8bc>
3000a146:	2a19      	cmp	r2, #25
3000a148:	f000 8396 	beq.w	3000a878 <rxi316_perf_tune+0x8b8>
3000a14c:	2a1a      	cmp	r2, #26
3000a14e:	f000 8391 	beq.w	3000a874 <rxi316_perf_tune+0x8b4>
3000a152:	2a1b      	cmp	r2, #27
3000a154:	f000 838c 	beq.w	3000a870 <rxi316_perf_tune+0x8b0>
3000a158:	2a1c      	cmp	r2, #28
3000a15a:	f000 8387 	beq.w	3000a86c <rxi316_perf_tune+0x8ac>
3000a15e:	2a1d      	cmp	r2, #29
3000a160:	f000 8382 	beq.w	3000a868 <rxi316_perf_tune+0x8a8>
3000a164:	2a1e      	cmp	r2, #30
3000a166:	f000 837d 	beq.w	3000a864 <rxi316_perf_tune+0x8a4>
3000a16a:	2a1f      	cmp	r2, #31
3000a16c:	f000 8374 	beq.w	3000a858 <rxi316_perf_tune+0x898>
3000a170:	2a20      	cmp	r2, #32
3000a172:	f000 836f 	beq.w	3000a854 <rxi316_perf_tune+0x894>
3000a176:	9809      	ldr	r0, [sp, #36]	; 0x24
3000a178:	2a21      	cmp	r2, #33	; 0x21
3000a17a:	bf08      	it	eq
3000a17c:	4618      	moveq	r0, r3
3000a17e:	9009      	str	r0, [sp, #36]	; 0x24
3000a180:	e797      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a182:	4698      	mov	r8, r3
3000a184:	3301      	adds	r3, #1
3000a186:	3101      	adds	r1, #1
3000a188:	2b20      	cmp	r3, #32
3000a18a:	d195      	bne.n	3000a0b8 <rxi316_perf_tune+0xf8>
3000a18c:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a190:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a194:	b1ba      	cbz	r2, 3000a1c6 <rxi316_perf_tune+0x206>
3000a196:	2a01      	cmp	r2, #1
3000a198:	f000 8331 	beq.w	3000a7fe <rxi316_perf_tune+0x83e>
3000a19c:	2a03      	cmp	r2, #3
3000a19e:	bf0c      	ite	eq
3000a1a0:	2204      	moveq	r2, #4
3000a1a2:	2203      	movne	r2, #3
3000a1a4:	921e      	str	r2, [sp, #120]	; 0x78
3000a1a6:	e010      	b.n	3000a1ca <rxi316_perf_tune+0x20a>
3000a1a8:	3101      	adds	r1, #1
3000a1aa:	461f      	mov	r7, r3
3000a1ac:	e781      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a1ae:	3101      	adds	r1, #1
3000a1b0:	469a      	mov	sl, r3
3000a1b2:	e77e      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a1b4:	3101      	adds	r1, #1
3000a1b6:	469b      	mov	fp, r3
3000a1b8:	e77b      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a1ba:	3101      	adds	r1, #1
3000a1bc:	9301      	str	r3, [sp, #4]
3000a1be:	e778      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a1c0:	3101      	adds	r1, #1
3000a1c2:	9302      	str	r3, [sp, #8]
3000a1c4:	e775      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a1c6:	2201      	movs	r2, #1
3000a1c8:	921e      	str	r2, [sp, #120]	; 0x78
3000a1ca:	3b02      	subs	r3, #2
3000a1cc:	428b      	cmp	r3, r1
3000a1ce:	d005      	beq.n	3000a1dc <rxi316_perf_tune+0x21c>
3000a1d0:	4bac      	ldr	r3, [pc, #688]	; (3000a484 <rxi316_perf_tune+0x4c4>)
3000a1d2:	2002      	movs	r0, #2
3000a1d4:	4aac      	ldr	r2, [pc, #688]	; (3000a488 <rxi316_perf_tune+0x4c8>)
3000a1d6:	4619      	mov	r1, r3
3000a1d8:	f001 feb8 	bl	3000bf4c <rtk_log_write>
3000a1dc:	9b1e      	ldr	r3, [sp, #120]	; 0x78
3000a1de:	9a03      	ldr	r2, [sp, #12]
3000a1e0:	4293      	cmp	r3, r2
3000a1e2:	d005      	beq.n	3000a1f0 <rxi316_perf_tune+0x230>
3000a1e4:	4ba7      	ldr	r3, [pc, #668]	; (3000a484 <rxi316_perf_tune+0x4c4>)
3000a1e6:	2002      	movs	r0, #2
3000a1e8:	4aa8      	ldr	r2, [pc, #672]	; (3000a48c <rxi316_perf_tune+0x4cc>)
3000a1ea:	4619      	mov	r1, r3
3000a1ec:	f001 feae 	bl	3000bf4c <rtk_log_write>
3000a1f0:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a1f2:	2b20      	cmp	r3, #32
3000a1f4:	f000 82f1 	beq.w	3000a7da <rxi316_perf_tune+0x81a>
3000a1f8:	1b5a      	subs	r2, r3, r5
3000a1fa:	3a05      	subs	r2, #5
3000a1fc:	f002 030f 	and.w	r3, r2, #15
3000a200:	9303      	str	r3, [sp, #12]
3000a202:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a204:	2b20      	cmp	r3, #32
3000a206:	f000 82e6 	beq.w	3000a7d6 <rxi316_perf_tune+0x816>
3000a20a:	1b5a      	subs	r2, r3, r5
3000a20c:	3a06      	subs	r2, #6
3000a20e:	0112      	lsls	r2, r2, #4
3000a210:	b2d2      	uxtb	r2, r2
3000a212:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a214:	2b20      	cmp	r3, #32
3000a216:	f000 82da 	beq.w	3000a7ce <rxi316_perf_tune+0x80e>
3000a21a:	1b5b      	subs	r3, r3, r5
3000a21c:	3b07      	subs	r3, #7
3000a21e:	021b      	lsls	r3, r3, #8
3000a220:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a224:	930c      	str	r3, [sp, #48]	; 0x30
3000a226:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a228:	2b20      	cmp	r3, #32
3000a22a:	f000 820b 	beq.w	3000a644 <rxi316_perf_tune+0x684>
3000a22e:	1b5b      	subs	r3, r3, r5
3000a230:	3b08      	subs	r3, #8
3000a232:	031b      	lsls	r3, r3, #12
3000a234:	b29b      	uxth	r3, r3
3000a236:	930b      	str	r3, [sp, #44]	; 0x2c
3000a238:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a23a:	2b20      	cmp	r3, #32
3000a23c:	f000 8209 	beq.w	3000a652 <rxi316_perf_tune+0x692>
3000a240:	1b5b      	subs	r3, r3, r5
3000a242:	3b09      	subs	r3, #9
3000a244:	041b      	lsls	r3, r3, #16
3000a246:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a24a:	930a      	str	r3, [sp, #40]	; 0x28
3000a24c:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a24e:	2b20      	cmp	r3, #32
3000a250:	f000 8206 	beq.w	3000a660 <rxi316_perf_tune+0x6a0>
3000a254:	1b5b      	subs	r3, r3, r5
3000a256:	3b0a      	subs	r3, #10
3000a258:	051b      	lsls	r3, r3, #20
3000a25a:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a25e:	930d      	str	r3, [sp, #52]	; 0x34
3000a260:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a262:	2b20      	cmp	r3, #32
3000a264:	f000 8203 	beq.w	3000a66e <rxi316_perf_tune+0x6ae>
3000a268:	1b5b      	subs	r3, r3, r5
3000a26a:	3b0b      	subs	r3, #11
3000a26c:	061b      	lsls	r3, r3, #24
3000a26e:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a272:	930e      	str	r3, [sp, #56]	; 0x38
3000a274:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a276:	2b20      	cmp	r3, #32
3000a278:	f000 8200 	beq.w	3000a67c <rxi316_perf_tune+0x6bc>
3000a27c:	1b5b      	subs	r3, r3, r5
3000a27e:	3b0c      	subs	r3, #12
3000a280:	071b      	lsls	r3, r3, #28
3000a282:	930f      	str	r3, [sp, #60]	; 0x3c
3000a284:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a286:	2b20      	cmp	r3, #32
3000a288:	f000 81ff 	beq.w	3000a68a <rxi316_perf_tune+0x6ca>
3000a28c:	1b5b      	subs	r3, r3, r5
3000a28e:	3b0d      	subs	r3, #13
3000a290:	f003 030f 	and.w	r3, r3, #15
3000a294:	9310      	str	r3, [sp, #64]	; 0x40
3000a296:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a298:	2b20      	cmp	r3, #32
3000a29a:	f000 81fc 	beq.w	3000a696 <rxi316_perf_tune+0x6d6>
3000a29e:	1b5b      	subs	r3, r3, r5
3000a2a0:	3b0e      	subs	r3, #14
3000a2a2:	011b      	lsls	r3, r3, #4
3000a2a4:	b2db      	uxtb	r3, r3
3000a2a6:	9311      	str	r3, [sp, #68]	; 0x44
3000a2a8:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a2aa:	2b20      	cmp	r3, #32
3000a2ac:	f000 81f9 	beq.w	3000a6a2 <rxi316_perf_tune+0x6e2>
3000a2b0:	1b5b      	subs	r3, r3, r5
3000a2b2:	3b0f      	subs	r3, #15
3000a2b4:	021b      	lsls	r3, r3, #8
3000a2b6:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a2ba:	9312      	str	r3, [sp, #72]	; 0x48
3000a2bc:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a2be:	2b20      	cmp	r3, #32
3000a2c0:	f000 81f6 	beq.w	3000a6b0 <rxi316_perf_tune+0x6f0>
3000a2c4:	1b5b      	subs	r3, r3, r5
3000a2c6:	031b      	lsls	r3, r3, #12
3000a2c8:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a2cc:	9321      	str	r3, [sp, #132]	; 0x84
3000a2ce:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a2d0:	2b20      	cmp	r3, #32
3000a2d2:	f000 81f4 	beq.w	3000a6be <rxi316_perf_tune+0x6fe>
3000a2d6:	1b5b      	subs	r3, r3, r5
3000a2d8:	3b11      	subs	r3, #17
3000a2da:	041b      	lsls	r3, r3, #16
3000a2dc:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a2e0:	9320      	str	r3, [sp, #128]	; 0x80
3000a2e2:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a2e4:	2b20      	cmp	r3, #32
3000a2e6:	f000 81f1 	beq.w	3000a6cc <rxi316_perf_tune+0x70c>
3000a2ea:	1b5b      	subs	r3, r3, r5
3000a2ec:	3b12      	subs	r3, #18
3000a2ee:	051b      	lsls	r3, r3, #20
3000a2f0:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a2f4:	931f      	str	r3, [sp, #124]	; 0x7c
3000a2f6:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a2f8:	2b20      	cmp	r3, #32
3000a2fa:	f000 81ee 	beq.w	3000a6da <rxi316_perf_tune+0x71a>
3000a2fe:	1b5b      	subs	r3, r3, r5
3000a300:	3b13      	subs	r3, #19
3000a302:	061b      	lsls	r3, r3, #24
3000a304:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a308:	931e      	str	r3, [sp, #120]	; 0x78
3000a30a:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a30c:	2b20      	cmp	r3, #32
3000a30e:	f000 81eb 	beq.w	3000a6e8 <rxi316_perf_tune+0x728>
3000a312:	1b5b      	subs	r3, r3, r5
3000a314:	3b14      	subs	r3, #20
3000a316:	071b      	lsls	r3, r3, #28
3000a318:	9318      	str	r3, [sp, #96]	; 0x60
3000a31a:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a31c:	2b20      	cmp	r3, #32
3000a31e:	f000 81ea 	beq.w	3000a6f6 <rxi316_perf_tune+0x736>
3000a322:	1b5b      	subs	r3, r3, r5
3000a324:	3b15      	subs	r3, #21
3000a326:	f003 030f 	and.w	r3, r3, #15
3000a32a:	9317      	str	r3, [sp, #92]	; 0x5c
3000a32c:	9b07      	ldr	r3, [sp, #28]
3000a32e:	2b20      	cmp	r3, #32
3000a330:	f000 81e7 	beq.w	3000a702 <rxi316_perf_tune+0x742>
3000a334:	1b5b      	subs	r3, r3, r5
3000a336:	3b02      	subs	r3, #2
3000a338:	f003 031f 	and.w	r3, r3, #31
3000a33c:	9316      	str	r3, [sp, #88]	; 0x58
3000a33e:	9b08      	ldr	r3, [sp, #32]
3000a340:	2b20      	cmp	r3, #32
3000a342:	f000 81e4 	beq.w	3000a70e <rxi316_perf_tune+0x74e>
3000a346:	1b5b      	subs	r3, r3, r5
3000a348:	3b02      	subs	r3, #2
3000a34a:	015b      	lsls	r3, r3, #5
3000a34c:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a350:	9315      	str	r3, [sp, #84]	; 0x54
3000a352:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a354:	2b20      	cmp	r3, #32
3000a356:	f000 81e1 	beq.w	3000a71c <rxi316_perf_tune+0x75c>
3000a35a:	1b5b      	subs	r3, r3, r5
3000a35c:	f1b9 0f20 	cmp.w	r9, #32
3000a360:	f1a3 0302 	sub.w	r3, r3, #2
3000a364:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a368:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a36c:	9314      	str	r3, [sp, #80]	; 0x50
3000a36e:	f000 81dc 	beq.w	3000a72a <rxi316_perf_tune+0x76a>
3000a372:	eba9 0105 	sub.w	r1, r9, r5
3000a376:	f1b8 0f20 	cmp.w	r8, #32
3000a37a:	f1a1 0102 	sub.w	r1, r1, #2
3000a37e:	f001 030f 	and.w	r3, r1, #15
3000a382:	9313      	str	r3, [sp, #76]	; 0x4c
3000a384:	f000 81d7 	beq.w	3000a736 <rxi316_perf_tune+0x776>
3000a388:	eba8 0105 	sub.w	r1, r8, r5
3000a38c:	2f20      	cmp	r7, #32
3000a38e:	f1a1 0103 	sub.w	r1, r1, #3
3000a392:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a396:	b2c9      	uxtb	r1, r1
3000a398:	f000 81d2 	beq.w	3000a740 <rxi316_perf_tune+0x780>
3000a39c:	1b7b      	subs	r3, r7, r5
3000a39e:	f1ba 0f20 	cmp.w	sl, #32
3000a3a2:	f1a3 0304 	sub.w	r3, r3, #4
3000a3a6:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a3aa:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a3ae:	9308      	str	r3, [sp, #32]
3000a3b0:	f000 81cd 	beq.w	3000a74e <rxi316_perf_tune+0x78e>
3000a3b4:	ebaa 0305 	sub.w	r3, sl, r5
3000a3b8:	f1bb 0f20 	cmp.w	fp, #32
3000a3bc:	f1a3 0305 	sub.w	r3, r3, #5
3000a3c0:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a3c4:	b29b      	uxth	r3, r3
3000a3c6:	9307      	str	r3, [sp, #28]
3000a3c8:	f000 81c8 	beq.w	3000a75c <rxi316_perf_tune+0x79c>
3000a3cc:	ebab 0c05 	sub.w	ip, fp, r5
3000a3d0:	9b01      	ldr	r3, [sp, #4]
3000a3d2:	f1ac 0c06 	sub.w	ip, ip, #6
3000a3d6:	2b20      	cmp	r3, #32
3000a3d8:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a3dc:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a3e0:	f000 81c2 	beq.w	3000a768 <rxi316_perf_tune+0x7a8>
3000a3e4:	eba3 0e05 	sub.w	lr, r3, r5
3000a3e8:	9b02      	ldr	r3, [sp, #8]
3000a3ea:	f1ae 0e07 	sub.w	lr, lr, #7
3000a3ee:	2b20      	cmp	r3, #32
3000a3f0:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a3f4:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a3f8:	f000 81bc 	beq.w	3000a774 <rxi316_perf_tune+0x7b4>
3000a3fc:	eba3 0805 	sub.w	r8, r3, r5
3000a400:	9b04      	ldr	r3, [sp, #16]
3000a402:	f1a8 0808 	sub.w	r8, r8, #8
3000a406:	2b20      	cmp	r3, #32
3000a408:	ea4f 6808 	mov.w	r8, r8, lsl #24
3000a40c:	f008 6870 	and.w	r8, r8, #251658240	; 0xf000000
3000a410:	f000 81b6 	beq.w	3000a780 <rxi316_perf_tune+0x7c0>
3000a414:	1b5b      	subs	r3, r3, r5
3000a416:	3b09      	subs	r3, #9
3000a418:	071b      	lsls	r3, r3, #28
3000a41a:	9302      	str	r3, [sp, #8]
3000a41c:	9b05      	ldr	r3, [sp, #20]
3000a41e:	2b20      	cmp	r3, #32
3000a420:	f000 81b5 	beq.w	3000a78e <rxi316_perf_tune+0x7ce>
3000a424:	eba3 0905 	sub.w	r9, r3, r5
3000a428:	9b06      	ldr	r3, [sp, #24]
3000a42a:	f1a9 090a 	sub.w	r9, r9, #10
3000a42e:	2b20      	cmp	r3, #32
3000a430:	f009 090f 	and.w	r9, r9, #15
3000a434:	f000 81b1 	beq.w	3000a79a <rxi316_perf_tune+0x7da>
3000a438:	1b58      	subs	r0, r3, r5
3000a43a:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a43c:	380b      	subs	r0, #11
3000a43e:	2b20      	cmp	r3, #32
3000a440:	ea4f 1000 	mov.w	r0, r0, lsl #4
3000a444:	b2c0      	uxtb	r0, r0
3000a446:	f000 81ad 	beq.w	3000a7a4 <rxi316_perf_tune+0x7e4>
3000a44a:	eba3 0b05 	sub.w	fp, r3, r5
3000a44e:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a450:	f1ab 0b02 	sub.w	fp, fp, #2
3000a454:	2b20      	cmp	r3, #32
3000a456:	f00b 0b1f 	and.w	fp, fp, #31
3000a45a:	f000 81a9 	beq.w	3000a7b0 <rxi316_perf_tune+0x7f0>
3000a45e:	eba3 0a05 	sub.w	sl, r3, r5
3000a462:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a464:	f1aa 0a02 	sub.w	sl, sl, #2
3000a468:	2b20      	cmp	r3, #32
3000a46a:	f00a 0a1f 	and.w	sl, sl, #31
3000a46e:	f000 81a5 	beq.w	3000a7bc <rxi316_perf_tune+0x7fc>
3000a472:	1b5f      	subs	r7, r3, r5
3000a474:	2c20      	cmp	r4, #32
3000a476:	f1a7 0702 	sub.w	r7, r7, #2
3000a47a:	f007 071f 	and.w	r7, r7, #31
3000a47e:	f000 81a2 	beq.w	3000a7c6 <rxi316_perf_tune+0x806>
3000a482:	e005      	b.n	3000a490 <rxi316_perf_tune+0x4d0>
3000a484:	3000d8e4 	.word	0x3000d8e4
3000a488:	3000d90c 	.word	0x3000d90c
3000a48c:	3000d950 	.word	0x3000d950
3000a490:	1b63      	subs	r3, r4, r5
3000a492:	3b02      	subs	r3, #2
3000a494:	015b      	lsls	r3, r3, #5
3000a496:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a49a:	9301      	str	r3, [sp, #4]
3000a49c:	9c03      	ldr	r4, [sp, #12]
3000a49e:	ea40 0009 	orr.w	r0, r0, r9
3000a4a2:	4314      	orrs	r4, r2
3000a4a4:	4622      	mov	r2, r4
3000a4a6:	e9dd 4510 	ldrd	r4, r5, [sp, #64]	; 0x40
3000a4aa:	432c      	orrs	r4, r5
3000a4ac:	9d13      	ldr	r5, [sp, #76]	; 0x4c
3000a4ae:	4329      	orrs	r1, r5
3000a4b0:	e9dd 5315 	ldrd	r5, r3, [sp, #84]	; 0x54
3000a4b4:	431d      	orrs	r5, r3
3000a4b6:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a4b8:	431a      	orrs	r2, r3
3000a4ba:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a4bc:	431c      	orrs	r4, r3
3000a4be:	9b08      	ldr	r3, [sp, #32]
3000a4c0:	4319      	orrs	r1, r3
3000a4c2:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a4c4:	431d      	orrs	r5, r3
3000a4c6:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a4c8:	431a      	orrs	r2, r3
3000a4ca:	9b21      	ldr	r3, [sp, #132]	; 0x84
3000a4cc:	431c      	orrs	r4, r3
3000a4ce:	9b07      	ldr	r3, [sp, #28]
3000a4d0:	4319      	orrs	r1, r3
3000a4d2:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a4d4:	431a      	orrs	r2, r3
3000a4d6:	9b20      	ldr	r3, [sp, #128]	; 0x80
3000a4d8:	ea41 010c 	orr.w	r1, r1, ip
3000a4dc:	431c      	orrs	r4, r3
3000a4de:	9b01      	ldr	r3, [sp, #4]
3000a4e0:	ea41 010e 	orr.w	r1, r1, lr
3000a4e4:	431f      	orrs	r7, r3
3000a4e6:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a4e8:	ea41 0108 	orr.w	r1, r1, r8
3000a4ec:	431a      	orrs	r2, r3
3000a4ee:	9b1f      	ldr	r3, [sp, #124]	; 0x7c
3000a4f0:	431c      	orrs	r4, r3
3000a4f2:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a4f4:	431a      	orrs	r2, r3
3000a4f6:	9b1e      	ldr	r3, [sp, #120]	; 0x78
3000a4f8:	4323      	orrs	r3, r4
3000a4fa:	9c0f      	ldr	r4, [sp, #60]	; 0x3c
3000a4fc:	4322      	orrs	r2, r4
3000a4fe:	9c18      	ldr	r4, [sp, #96]	; 0x60
3000a500:	4323      	orrs	r3, r4
3000a502:	9c02      	ldr	r4, [sp, #8]
3000a504:	430c      	orrs	r4, r1
3000a506:	49ba      	ldr	r1, [pc, #744]	; (3000a7f0 <rxi316_perf_tune+0x830>)
3000a508:	f8c1 2400 	str.w	r2, [r1, #1024]	; 0x400
3000a50c:	f8c1 3404 	str.w	r3, [r1, #1028]	; 0x404
3000a510:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a512:	f8c1 3408 	str.w	r3, [r1, #1032]	; 0x408
3000a516:	f8c1 540c 	str.w	r5, [r1, #1036]	; 0x40c
3000a51a:	f8c1 4410 	str.w	r4, [r1, #1040]	; 0x410
3000a51e:	f8c1 0414 	str.w	r0, [r1, #1044]	; 0x414
3000a522:	f8c1 b418 	str.w	fp, [r1, #1048]	; 0x418
3000a526:	f8c1 a41c 	str.w	sl, [r1, #1052]	; 0x41c
3000a52a:	f8c1 7420 	str.w	r7, [r1, #1056]	; 0x420
3000a52e:	68f3      	ldr	r3, [r6, #12]
3000a530:	689a      	ldr	r2, [r3, #8]
3000a532:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a536:	f003 0301 	and.w	r3, r3, #1
3000a53a:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a53e:	f8c1 3460 	str.w	r3, [r1, #1120]	; 0x460
3000a542:	68f3      	ldr	r3, [r6, #12]
3000a544:	689a      	ldr	r2, [r3, #8]
3000a546:	7a13      	ldrb	r3, [r2, #8]
3000a548:	6950      	ldr	r0, [r2, #20]
3000a54a:	ea43 6300 	orr.w	r3, r3, r0, lsl #24
3000a54e:	e9d2 0203 	ldrd	r0, r2, [r2, #12]
3000a552:	0200      	lsls	r0, r0, #8
3000a554:	0412      	lsls	r2, r2, #16
3000a556:	b280      	uxth	r0, r0
3000a558:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a55c:	4303      	orrs	r3, r0
3000a55e:	4313      	orrs	r3, r2
3000a560:	f8c1 3464 	str.w	r3, [r1, #1124]	; 0x464
3000a564:	68f3      	ldr	r3, [r6, #12]
3000a566:	6898      	ldr	r0, [r3, #8]
3000a568:	e9d0 3207 	ldrd	r3, r2, [r0, #28]
3000a56c:	7e00      	ldrb	r0, [r0, #24]
3000a56e:	021b      	lsls	r3, r3, #8
3000a570:	0412      	lsls	r2, r2, #16
3000a572:	b29b      	uxth	r3, r3
3000a574:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a578:	4313      	orrs	r3, r2
3000a57a:	4303      	orrs	r3, r0
3000a57c:	f8c1 3468 	str.w	r3, [r1, #1128]	; 0x468
3000a580:	68f3      	ldr	r3, [r6, #12]
3000a582:	689a      	ldr	r2, [r3, #8]
3000a584:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a588:	041b      	lsls	r3, r3, #16
3000a58a:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a58e:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a592:	4313      	orrs	r3, r2
3000a594:	f8c1 3480 	str.w	r3, [r1, #1152]	; 0x480
3000a598:	6833      	ldr	r3, [r6, #0]
3000a59a:	781b      	ldrb	r3, [r3, #0]
3000a59c:	2b03      	cmp	r3, #3
3000a59e:	f000 811f 	beq.w	3000a7e0 <rxi316_perf_tune+0x820>
3000a5a2:	68f3      	ldr	r3, [r6, #12]
3000a5a4:	6899      	ldr	r1, [r3, #8]
3000a5a6:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a5a8:	005b      	lsls	r3, r3, #1
3000a5aa:	f003 0302 	and.w	r3, r3, #2
3000a5ae:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a5b0:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a5b2:	0412      	lsls	r2, r2, #16
3000a5b4:	6b49      	ldr	r1, [r1, #52]	; 0x34
3000a5b6:	f000 0001 	and.w	r0, r0, #1
3000a5ba:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a5be:	0209      	lsls	r1, r1, #8
3000a5c0:	431a      	orrs	r2, r3
3000a5c2:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a5c6:	4b8a      	ldr	r3, [pc, #552]	; (3000a7f0 <rxi316_perf_tune+0x830>)
3000a5c8:	4302      	orrs	r2, r0
3000a5ca:	488a      	ldr	r0, [pc, #552]	; (3000a7f4 <rxi316_perf_tune+0x834>)
3000a5cc:	430a      	orrs	r2, r1
3000a5ce:	4310      	orrs	r0, r2
3000a5d0:	f8c3 0484 	str.w	r0, [r3, #1156]	; 0x484
3000a5d4:	68f2      	ldr	r2, [r6, #12]
3000a5d6:	6892      	ldr	r2, [r2, #8]
3000a5d8:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
3000a5da:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
3000a5de:	68f2      	ldr	r2, [r6, #12]
3000a5e0:	6892      	ldr	r2, [r2, #8]
3000a5e2:	6c12      	ldr	r2, [r2, #64]	; 0x40
3000a5e4:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000a5e8:	68f2      	ldr	r2, [r6, #12]
3000a5ea:	6892      	ldr	r2, [r2, #8]
3000a5ec:	6c52      	ldr	r2, [r2, #68]	; 0x44
3000a5ee:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
3000a5f2:	68f2      	ldr	r2, [r6, #12]
3000a5f4:	6892      	ldr	r2, [r2, #8]
3000a5f6:	6c92      	ldr	r2, [r2, #72]	; 0x48
3000a5f8:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
3000a5fc:	68f2      	ldr	r2, [r6, #12]
3000a5fe:	6892      	ldr	r2, [r2, #8]
3000a600:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
3000a602:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
3000a606:	68f2      	ldr	r2, [r6, #12]
3000a608:	6892      	ldr	r2, [r2, #8]
3000a60a:	6d12      	ldr	r2, [r2, #80]	; 0x50
3000a60c:	f002 020f 	and.w	r2, r2, #15
3000a610:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
3000a614:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
3000a618:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
3000a61c:	f042 0204 	orr.w	r2, r2, #4
3000a620:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
3000a624:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
3000a628:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
3000a62c:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
3000a630:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
3000a634:	b043      	add	sp, #268	; 0x10c
3000a636:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a63a:	2502      	movs	r5, #2
3000a63c:	e4cf      	b.n	30009fde <rxi316_perf_tune+0x1e>
3000a63e:	3101      	adds	r1, #1
3000a640:	9304      	str	r3, [sp, #16]
3000a642:	e536      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a644:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a648:	930b      	str	r3, [sp, #44]	; 0x2c
3000a64a:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a64c:	2b20      	cmp	r3, #32
3000a64e:	f47f adf7 	bne.w	3000a240 <rxi316_perf_tune+0x280>
3000a652:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a656:	930a      	str	r3, [sp, #40]	; 0x28
3000a658:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a65a:	2b20      	cmp	r3, #32
3000a65c:	f47f adfa 	bne.w	3000a254 <rxi316_perf_tune+0x294>
3000a660:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a664:	930d      	str	r3, [sp, #52]	; 0x34
3000a666:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a668:	2b20      	cmp	r3, #32
3000a66a:	f47f adfd 	bne.w	3000a268 <rxi316_perf_tune+0x2a8>
3000a66e:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a672:	930e      	str	r3, [sp, #56]	; 0x38
3000a674:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a676:	2b20      	cmp	r3, #32
3000a678:	f47f ae00 	bne.w	3000a27c <rxi316_perf_tune+0x2bc>
3000a67c:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a680:	930f      	str	r3, [sp, #60]	; 0x3c
3000a682:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a684:	2b20      	cmp	r3, #32
3000a686:	f47f ae01 	bne.w	3000a28c <rxi316_perf_tune+0x2cc>
3000a68a:	230f      	movs	r3, #15
3000a68c:	9310      	str	r3, [sp, #64]	; 0x40
3000a68e:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a690:	2b20      	cmp	r3, #32
3000a692:	f47f ae04 	bne.w	3000a29e <rxi316_perf_tune+0x2de>
3000a696:	23f0      	movs	r3, #240	; 0xf0
3000a698:	9311      	str	r3, [sp, #68]	; 0x44
3000a69a:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a69c:	2b20      	cmp	r3, #32
3000a69e:	f47f ae07 	bne.w	3000a2b0 <rxi316_perf_tune+0x2f0>
3000a6a2:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a6a6:	9312      	str	r3, [sp, #72]	; 0x48
3000a6a8:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a6aa:	2b20      	cmp	r3, #32
3000a6ac:	f47f ae0a 	bne.w	3000a2c4 <rxi316_perf_tune+0x304>
3000a6b0:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a6b4:	9321      	str	r3, [sp, #132]	; 0x84
3000a6b6:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a6b8:	2b20      	cmp	r3, #32
3000a6ba:	f47f ae0c 	bne.w	3000a2d6 <rxi316_perf_tune+0x316>
3000a6be:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a6c2:	9320      	str	r3, [sp, #128]	; 0x80
3000a6c4:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a6c6:	2b20      	cmp	r3, #32
3000a6c8:	f47f ae0f 	bne.w	3000a2ea <rxi316_perf_tune+0x32a>
3000a6cc:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a6d0:	931f      	str	r3, [sp, #124]	; 0x7c
3000a6d2:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a6d4:	2b20      	cmp	r3, #32
3000a6d6:	f47f ae12 	bne.w	3000a2fe <rxi316_perf_tune+0x33e>
3000a6da:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a6de:	931e      	str	r3, [sp, #120]	; 0x78
3000a6e0:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a6e2:	2b20      	cmp	r3, #32
3000a6e4:	f47f ae15 	bne.w	3000a312 <rxi316_perf_tune+0x352>
3000a6e8:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a6ec:	9318      	str	r3, [sp, #96]	; 0x60
3000a6ee:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a6f0:	2b20      	cmp	r3, #32
3000a6f2:	f47f ae16 	bne.w	3000a322 <rxi316_perf_tune+0x362>
3000a6f6:	230f      	movs	r3, #15
3000a6f8:	9317      	str	r3, [sp, #92]	; 0x5c
3000a6fa:	9b07      	ldr	r3, [sp, #28]
3000a6fc:	2b20      	cmp	r3, #32
3000a6fe:	f47f ae19 	bne.w	3000a334 <rxi316_perf_tune+0x374>
3000a702:	231f      	movs	r3, #31
3000a704:	9316      	str	r3, [sp, #88]	; 0x58
3000a706:	9b08      	ldr	r3, [sp, #32]
3000a708:	2b20      	cmp	r3, #32
3000a70a:	f47f ae1c 	bne.w	3000a346 <rxi316_perf_tune+0x386>
3000a70e:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a712:	9315      	str	r3, [sp, #84]	; 0x54
3000a714:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a716:	2b20      	cmp	r3, #32
3000a718:	f47f ae1f 	bne.w	3000a35a <rxi316_perf_tune+0x39a>
3000a71c:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000a720:	f1b9 0f20 	cmp.w	r9, #32
3000a724:	9314      	str	r3, [sp, #80]	; 0x50
3000a726:	f47f ae24 	bne.w	3000a372 <rxi316_perf_tune+0x3b2>
3000a72a:	230f      	movs	r3, #15
3000a72c:	f1b8 0f20 	cmp.w	r8, #32
3000a730:	9313      	str	r3, [sp, #76]	; 0x4c
3000a732:	f47f ae29 	bne.w	3000a388 <rxi316_perf_tune+0x3c8>
3000a736:	2f20      	cmp	r7, #32
3000a738:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a73c:	f47f ae2e 	bne.w	3000a39c <rxi316_perf_tune+0x3dc>
3000a740:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a744:	f1ba 0f20 	cmp.w	sl, #32
3000a748:	9308      	str	r3, [sp, #32]
3000a74a:	f47f ae33 	bne.w	3000a3b4 <rxi316_perf_tune+0x3f4>
3000a74e:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a752:	f1bb 0f20 	cmp.w	fp, #32
3000a756:	9307      	str	r3, [sp, #28]
3000a758:	f47f ae38 	bne.w	3000a3cc <rxi316_perf_tune+0x40c>
3000a75c:	9b01      	ldr	r3, [sp, #4]
3000a75e:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a762:	2b20      	cmp	r3, #32
3000a764:	f47f ae3e 	bne.w	3000a3e4 <rxi316_perf_tune+0x424>
3000a768:	9b02      	ldr	r3, [sp, #8]
3000a76a:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a76e:	2b20      	cmp	r3, #32
3000a770:	f47f ae44 	bne.w	3000a3fc <rxi316_perf_tune+0x43c>
3000a774:	9b04      	ldr	r3, [sp, #16]
3000a776:	f04f 6870 	mov.w	r8, #251658240	; 0xf000000
3000a77a:	2b20      	cmp	r3, #32
3000a77c:	f47f ae4a 	bne.w	3000a414 <rxi316_perf_tune+0x454>
3000a780:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a784:	9302      	str	r3, [sp, #8]
3000a786:	9b05      	ldr	r3, [sp, #20]
3000a788:	2b20      	cmp	r3, #32
3000a78a:	f47f ae4b 	bne.w	3000a424 <rxi316_perf_tune+0x464>
3000a78e:	9b06      	ldr	r3, [sp, #24]
3000a790:	f04f 090f 	mov.w	r9, #15
3000a794:	2b20      	cmp	r3, #32
3000a796:	f47f ae4f 	bne.w	3000a438 <rxi316_perf_tune+0x478>
3000a79a:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a79c:	20f0      	movs	r0, #240	; 0xf0
3000a79e:	2b20      	cmp	r3, #32
3000a7a0:	f47f ae53 	bne.w	3000a44a <rxi316_perf_tune+0x48a>
3000a7a4:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a7a6:	f04f 0b1f 	mov.w	fp, #31
3000a7aa:	2b20      	cmp	r3, #32
3000a7ac:	f47f ae57 	bne.w	3000a45e <rxi316_perf_tune+0x49e>
3000a7b0:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a7b2:	f04f 0a1f 	mov.w	sl, #31
3000a7b6:	2b20      	cmp	r3, #32
3000a7b8:	f47f ae5b 	bne.w	3000a472 <rxi316_perf_tune+0x4b2>
3000a7bc:	2c20      	cmp	r4, #32
3000a7be:	f04f 071f 	mov.w	r7, #31
3000a7c2:	f47f ae65 	bne.w	3000a490 <rxi316_perf_tune+0x4d0>
3000a7c6:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a7ca:	9301      	str	r3, [sp, #4]
3000a7cc:	e666      	b.n	3000a49c <rxi316_perf_tune+0x4dc>
3000a7ce:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a7d2:	930c      	str	r3, [sp, #48]	; 0x30
3000a7d4:	e527      	b.n	3000a226 <rxi316_perf_tune+0x266>
3000a7d6:	22f0      	movs	r2, #240	; 0xf0
3000a7d8:	e51b      	b.n	3000a212 <rxi316_perf_tune+0x252>
3000a7da:	230f      	movs	r3, #15
3000a7dc:	9303      	str	r3, [sp, #12]
3000a7de:	e510      	b.n	3000a202 <rxi316_perf_tune+0x242>
3000a7e0:	68f2      	ldr	r2, [r6, #12]
3000a7e2:	2302      	movs	r3, #2
3000a7e4:	6891      	ldr	r1, [r2, #8]
3000a7e6:	e6e2      	b.n	3000a5ae <rxi316_perf_tune+0x5ee>
3000a7e8:	3101      	adds	r1, #1
3000a7ea:	9305      	str	r3, [sp, #20]
3000a7ec:	e461      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a7ee:	bf00      	nop
3000a7f0:	41100000 	.word	0x41100000
3000a7f4:	40000008 	.word	0x40000008
3000a7f8:	3101      	adds	r1, #1
3000a7fa:	9306      	str	r3, [sp, #24]
3000a7fc:	e459      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a7fe:	2202      	movs	r2, #2
3000a800:	921e      	str	r2, [sp, #120]	; 0x78
3000a802:	e4e2      	b.n	3000a1ca <rxi316_perf_tune+0x20a>
3000a804:	9a03      	ldr	r2, [sp, #12]
3000a806:	9308      	str	r3, [sp, #32]
3000a808:	3201      	adds	r2, #1
3000a80a:	9203      	str	r2, [sp, #12]
3000a80c:	e451      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a80e:	9a03      	ldr	r2, [sp, #12]
3000a810:	9307      	str	r3, [sp, #28]
3000a812:	3201      	adds	r2, #1
3000a814:	9203      	str	r2, [sp, #12]
3000a816:	e44c      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a818:	9a03      	ldr	r2, [sp, #12]
3000a81a:	931b      	str	r3, [sp, #108]	; 0x6c
3000a81c:	3201      	adds	r2, #1
3000a81e:	9203      	str	r2, [sp, #12]
3000a820:	e447      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a822:	9a03      	ldr	r2, [sp, #12]
3000a824:	461c      	mov	r4, r3
3000a826:	3201      	adds	r2, #1
3000a828:	9203      	str	r2, [sp, #12]
3000a82a:	e442      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a82c:	9a03      	ldr	r2, [sp, #12]
3000a82e:	931d      	str	r3, [sp, #116]	; 0x74
3000a830:	3201      	adds	r2, #1
3000a832:	9203      	str	r2, [sp, #12]
3000a834:	e43d      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a836:	9a03      	ldr	r2, [sp, #12]
3000a838:	931c      	str	r3, [sp, #112]	; 0x70
3000a83a:	3201      	adds	r2, #1
3000a83c:	9203      	str	r2, [sp, #12]
3000a83e:	e438      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a840:	930e      	str	r3, [sp, #56]	; 0x38
3000a842:	e436      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a844:	930d      	str	r3, [sp, #52]	; 0x34
3000a846:	e434      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a848:	930c      	str	r3, [sp, #48]	; 0x30
3000a84a:	e432      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a84c:	930a      	str	r3, [sp, #40]	; 0x28
3000a84e:	e430      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a850:	930b      	str	r3, [sp, #44]	; 0x2c
3000a852:	e42e      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a854:	931a      	str	r3, [sp, #104]	; 0x68
3000a856:	e42c      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a858:	9319      	str	r3, [sp, #100]	; 0x64
3000a85a:	e42a      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a85c:	9310      	str	r3, [sp, #64]	; 0x40
3000a85e:	e428      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a860:	930f      	str	r3, [sp, #60]	; 0x3c
3000a862:	e426      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a864:	9318      	str	r3, [sp, #96]	; 0x60
3000a866:	e424      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a868:	9317      	str	r3, [sp, #92]	; 0x5c
3000a86a:	e422      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a86c:	9316      	str	r3, [sp, #88]	; 0x58
3000a86e:	e420      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a870:	9315      	str	r3, [sp, #84]	; 0x54
3000a872:	e41e      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a874:	9314      	str	r3, [sp, #80]	; 0x50
3000a876:	e41c      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a878:	9313      	str	r3, [sp, #76]	; 0x4c
3000a87a:	e41a      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a87c:	9312      	str	r3, [sp, #72]	; 0x48
3000a87e:	e418      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>
3000a880:	9311      	str	r3, [sp, #68]	; 0x44
3000a882:	e416      	b.n	3000a0b2 <rxi316_perf_tune+0xf2>

3000a884 <rxi316_dram_init>:
3000a884:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a888:	4604      	mov	r4, r0
3000a88a:	b08f      	sub	sp, #60	; 0x3c
3000a88c:	4b10      	ldr	r3, [pc, #64]	; (3000a8d0 <rxi316_dram_init+0x4c>)
3000a88e:	4798      	blx	r3
3000a890:	f894 b014 	ldrb.w	fp, [r4, #20]
3000a894:	2101      	movs	r1, #1
3000a896:	900c      	str	r0, [sp, #48]	; 0x30
3000a898:	fa01 f30b 	lsl.w	r3, r1, fp
3000a89c:	9302      	str	r3, [sp, #8]
3000a89e:	6823      	ldr	r3, [r4, #0]
3000a8a0:	78da      	ldrb	r2, [r3, #3]
3000a8a2:	6923      	ldr	r3, [r4, #16]
3000a8a4:	fa03 f30b 	lsl.w	r3, r3, fp
3000a8a8:	b1a2      	cbz	r2, 3000a8d4 <rxi316_dram_init+0x50>
3000a8aa:	428a      	cmp	r2, r1
3000a8ac:	f000 8469 	beq.w	3000b182 <rxi316_dram_init+0x8fe>
3000a8b0:	2a02      	cmp	r2, #2
3000a8b2:	bf0b      	itete	eq
3000a8b4:	2206      	moveq	r2, #6
3000a8b6:	220a      	movne	r2, #10
3000a8b8:	2540      	moveq	r5, #64	; 0x40
3000a8ba:	2580      	movne	r5, #128	; 0x80
3000a8bc:	9207      	str	r2, [sp, #28]
3000a8be:	bf0c      	ite	eq
3000a8c0:	2204      	moveq	r2, #4
3000a8c2:	2208      	movne	r2, #8
3000a8c4:	9208      	str	r2, [sp, #32]
3000a8c6:	bf0c      	ite	eq
3000a8c8:	2208      	moveq	r2, #8
3000a8ca:	2210      	movne	r2, #16
3000a8cc:	9201      	str	r2, [sp, #4]
3000a8ce:	e007      	b.n	3000a8e0 <rxi316_dram_init+0x5c>
3000a8d0:	30007fa9 	.word	0x30007fa9
3000a8d4:	2203      	movs	r2, #3
3000a8d6:	25c0      	movs	r5, #192	; 0xc0
3000a8d8:	9108      	str	r1, [sp, #32]
3000a8da:	9207      	str	r2, [sp, #28]
3000a8dc:	2202      	movs	r2, #2
3000a8de:	9201      	str	r2, [sp, #4]
3000a8e0:	6860      	ldr	r0, [r4, #4]
3000a8e2:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000a8e6:	4abf      	ldr	r2, [pc, #764]	; (3000abe4 <rxi316_dram_init+0x360>)
3000a8e8:	6806      	ldr	r6, [r0, #0]
3000a8ea:	960d      	str	r6, [sp, #52]	; 0x34
3000a8ec:	6846      	ldr	r6, [r0, #4]
3000a8ee:	6880      	ldr	r0, [r0, #8]
3000a8f0:	9609      	str	r6, [sp, #36]	; 0x24
3000a8f2:	9003      	str	r0, [sp, #12]
3000a8f4:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000a8f8:	6820      	ldr	r0, [r4, #0]
3000a8fa:	7981      	ldrb	r1, [r0, #6]
3000a8fc:	6882      	ldr	r2, [r0, #8]
3000a8fe:	b129      	cbz	r1, 3000a90c <rxi316_dram_init+0x88>
3000a900:	2901      	cmp	r1, #1
3000a902:	f000 8436 	beq.w	3000b172 <rxi316_dram_init+0x8ee>
3000a906:	2903      	cmp	r1, #3
3000a908:	f000 8443 	beq.w	3000b192 <rxi316_dram_init+0x90e>
3000a90c:	3a06      	subs	r2, #6
3000a90e:	7881      	ldrb	r1, [r0, #2]
3000a910:	f002 020f 	and.w	r2, r2, #15
3000a914:	7940      	ldrb	r0, [r0, #5]
3000a916:	0109      	lsls	r1, r1, #4
3000a918:	0280      	lsls	r0, r0, #10
3000a91a:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000a91e:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000a922:	4301      	orrs	r1, r0
3000a924:	430d      	orrs	r5, r1
3000a926:	432a      	orrs	r2, r5
3000a928:	4dae      	ldr	r5, [pc, #696]	; (3000abe4 <rxi316_dram_init+0x360>)
3000a92a:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000a92e:	68e2      	ldr	r2, [r4, #12]
3000a930:	6812      	ldr	r2, [r2, #0]
3000a932:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000a936:	f002 0207 	and.w	r2, r2, #7
3000a93a:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000a93e:	68e2      	ldr	r2, [r4, #12]
3000a940:	6811      	ldr	r1, [r2, #0]
3000a942:	6822      	ldr	r2, [r4, #0]
3000a944:	6848      	ldr	r0, [r1, #4]
3000a946:	f892 9000 	ldrb.w	r9, [r2]
3000a94a:	688a      	ldr	r2, [r1, #8]
3000a94c:	9004      	str	r0, [sp, #16]
3000a94e:	0300      	lsls	r0, r0, #12
3000a950:	0412      	lsls	r2, r2, #16
3000a952:	f009 070f 	and.w	r7, r9, #15
3000a956:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000a95a:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000a95e:	4302      	orrs	r2, r0
3000a960:	68c8      	ldr	r0, [r1, #12]
3000a962:	0440      	lsls	r0, r0, #17
3000a964:	433a      	orrs	r2, r7
3000a966:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000a96a:	6908      	ldr	r0, [r1, #16]
3000a96c:	0480      	lsls	r0, r0, #18
3000a96e:	4332      	orrs	r2, r6
3000a970:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000a974:	4310      	orrs	r0, r2
3000a976:	694a      	ldr	r2, [r1, #20]
3000a978:	04d2      	lsls	r2, r2, #19
3000a97a:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000a97e:	4310      	orrs	r0, r2
3000a980:	698a      	ldr	r2, [r1, #24]
3000a982:	0552      	lsls	r2, r2, #21
3000a984:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000a988:	4310      	orrs	r0, r2
3000a98a:	69ca      	ldr	r2, [r1, #28]
3000a98c:	0592      	lsls	r2, r2, #22
3000a98e:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000a992:	4310      	orrs	r0, r2
3000a994:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000a996:	0612      	lsls	r2, r2, #24
3000a998:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000a99c:	4302      	orrs	r2, r0
3000a99e:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000a9a2:	06c0      	lsls	r0, r0, #27
3000a9a4:	0789      	lsls	r1, r1, #30
3000a9a6:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000a9aa:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000a9ae:	4310      	orrs	r0, r2
3000a9b0:	9a02      	ldr	r2, [sp, #8]
3000a9b2:	0212      	lsls	r2, r2, #8
3000a9b4:	4308      	orrs	r0, r1
3000a9b6:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000a9ba:	4310      	orrs	r0, r2
3000a9bc:	6068      	str	r0, [r5, #4]
3000a9be:	6822      	ldr	r2, [r4, #0]
3000a9c0:	7992      	ldrb	r2, [r2, #6]
3000a9c2:	2a00      	cmp	r2, #0
3000a9c4:	f040 8321 	bne.w	3000b00a <rxi316_dram_init+0x786>
3000a9c8:	686a      	ldr	r2, [r5, #4]
3000a9ca:	f042 0210 	orr.w	r2, r2, #16
3000a9ce:	606a      	str	r2, [r5, #4]
3000a9d0:	68e2      	ldr	r2, [r4, #12]
3000a9d2:	9802      	ldr	r0, [sp, #8]
3000a9d4:	6812      	ldr	r2, [r2, #0]
3000a9d6:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
3000a9da:	6861      	ldr	r1, [r4, #4]
3000a9dc:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000a9de:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000a9e2:	9005      	str	r0, [sp, #20]
3000a9e4:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000a9e6:	fa2e f80b 	lsr.w	r8, lr, fp
3000a9ea:	6949      	ldr	r1, [r1, #20]
3000a9ec:	ea1e 0f0a 	tst.w	lr, sl
3000a9f0:	9006      	str	r0, [sp, #24]
3000a9f2:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000a9f4:	fa21 fe0b 	lsr.w	lr, r1, fp
3000a9f8:	bf18      	it	ne
3000a9fa:	f108 0801 	addne.w	r8, r8, #1
3000a9fe:	ea11 0f0a 	tst.w	r1, sl
3000aa02:	9906      	ldr	r1, [sp, #24]
3000aa04:	950a      	str	r5, [sp, #40]	; 0x28
3000aa06:	bf18      	it	ne
3000aa08:	f10e 0e01 	addne.w	lr, lr, #1
3000aa0c:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000aa0e:	0049      	lsls	r1, r1, #1
3000aa10:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000aa14:	950b      	str	r5, [sp, #44]	; 0x2c
3000aa16:	f001 0102 	and.w	r1, r1, #2
3000aa1a:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000aa1e:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000aa22:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000aa26:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000aa2a:	0080      	lsls	r0, r0, #2
3000aa2c:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000aa30:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000aa34:	9006      	str	r0, [sp, #24]
3000aa36:	013f      	lsls	r7, r7, #4
3000aa38:	9805      	ldr	r0, [sp, #20]
3000aa3a:	f00c 0c08 	and.w	ip, ip, #8
3000aa3e:	0176      	lsls	r6, r6, #5
3000aa40:	f007 0710 	and.w	r7, r7, #16
3000aa44:	f000 0001 	and.w	r0, r0, #1
3000aa48:	f006 0620 	and.w	r6, r6, #32
3000aa4c:	4605      	mov	r5, r0
3000aa4e:	9806      	ldr	r0, [sp, #24]
3000aa50:	f000 0004 	and.w	r0, r0, #4
3000aa54:	4301      	orrs	r1, r0
3000aa56:	4329      	orrs	r1, r5
3000aa58:	ea41 0c0c 	orr.w	ip, r1, ip
3000aa5c:	990a      	ldr	r1, [sp, #40]	; 0x28
3000aa5e:	ea4c 0707 	orr.w	r7, ip, r7
3000aa62:	018d      	lsls	r5, r1, #6
3000aa64:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000aa66:	4337      	orrs	r7, r6
3000aa68:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000aa6c:	0208      	lsls	r0, r1, #8
3000aa6e:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000aa70:	432f      	orrs	r7, r5
3000aa72:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000aa76:	06c9      	lsls	r1, r1, #27
3000aa78:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000aa7c:	4307      	orrs	r7, r0
3000aa7e:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000aa82:	0736      	lsls	r6, r6, #28
3000aa84:	430f      	orrs	r7, r1
3000aa86:	0752      	lsls	r2, r2, #29
3000aa88:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000aa8c:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000aa90:	433e      	orrs	r6, r7
3000aa92:	4316      	orrs	r6, r2
3000aa94:	4a53      	ldr	r2, [pc, #332]	; (3000abe4 <rxi316_dram_init+0x360>)
3000aa96:	ea46 0608 	orr.w	r6, r6, r8
3000aa9a:	ea46 060e 	orr.w	r6, r6, lr
3000aa9e:	6096      	str	r6, [r2, #8]
3000aaa0:	6862      	ldr	r2, [r4, #4]
3000aaa2:	6990      	ldr	r0, [r2, #24]
3000aaa4:	9a01      	ldr	r2, [sp, #4]
3000aaa6:	2a10      	cmp	r2, #16
3000aaa8:	6822      	ldr	r2, [r4, #0]
3000aaaa:	7811      	ldrb	r1, [r2, #0]
3000aaac:	7916      	ldrb	r6, [r2, #4]
3000aaae:	f000 8321 	beq.w	3000b0f4 <rxi316_dram_init+0x870>
3000aab2:	f1a1 0502 	sub.w	r5, r1, #2
3000aab6:	f1a1 0e09 	sub.w	lr, r1, #9
3000aaba:	9f01      	ldr	r7, [sp, #4]
3000aabc:	fab5 f585 	clz	r5, r5
3000aac0:	fabe fe8e 	clz	lr, lr
3000aac4:	2f08      	cmp	r7, #8
3000aac6:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000aaca:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000aace:	ea45 020e 	orr.w	r2, r5, lr
3000aad2:	f000 82a8 	beq.w	3000b026 <rxi316_dram_init+0x7a2>
3000aad6:	9f01      	ldr	r7, [sp, #4]
3000aad8:	2f04      	cmp	r7, #4
3000aada:	f000 837c 	beq.w	3000b1d6 <rxi316_dram_init+0x952>
3000aade:	9f01      	ldr	r7, [sp, #4]
3000aae0:	2f02      	cmp	r7, #2
3000aae2:	bf08      	it	eq
3000aae4:	4310      	orreq	r0, r2
3000aae6:	2a00      	cmp	r2, #0
3000aae8:	f040 82a2 	bne.w	3000b030 <rxi316_dram_init+0x7ac>
3000aaec:	2903      	cmp	r1, #3
3000aaee:	f000 8355 	beq.w	3000b19c <rxi316_dram_init+0x918>
3000aaf2:	2500      	movs	r5, #0
3000aaf4:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000aaf8:	462f      	mov	r7, r5
3000aafa:	433d      	orrs	r5, r7
3000aafc:	4f39      	ldr	r7, [pc, #228]	; (3000abe4 <rxi316_dram_init+0x360>)
3000aafe:	430d      	orrs	r5, r1
3000ab00:	4305      	orrs	r5, r0
3000ab02:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000ab06:	637e      	str	r6, [r7, #52]	; 0x34
3000ab08:	6862      	ldr	r2, [r4, #4]
3000ab0a:	9903      	ldr	r1, [sp, #12]
3000ab0c:	69d2      	ldr	r2, [r2, #28]
3000ab0e:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000ab12:	63ba      	str	r2, [r7, #56]	; 0x38
3000ab14:	6822      	ldr	r2, [r4, #0]
3000ab16:	7810      	ldrb	r0, [r2, #0]
3000ab18:	6862      	ldr	r2, [r4, #4]
3000ab1a:	2803      	cmp	r0, #3
3000ab1c:	6a11      	ldr	r1, [r2, #32]
3000ab1e:	f000 82dc 	beq.w	3000b0da <rxi316_dram_init+0x856>
3000ab22:	2809      	cmp	r0, #9
3000ab24:	f000 82fa 	beq.w	3000b11c <rxi316_dram_init+0x898>
3000ab28:	482e      	ldr	r0, [pc, #184]	; (3000abe4 <rxi316_dram_init+0x360>)
3000ab2a:	63c1      	str	r1, [r0, #60]	; 0x3c
3000ab2c:	6862      	ldr	r2, [r4, #4]
3000ab2e:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000ab30:	6402      	str	r2, [r0, #64]	; 0x40
3000ab32:	6860      	ldr	r0, [r4, #4]
3000ab34:	68c5      	ldr	r5, [r0, #12]
3000ab36:	6802      	ldr	r2, [r0, #0]
3000ab38:	6841      	ldr	r1, [r0, #4]
3000ab3a:	442a      	add	r2, r5
3000ab3c:	6880      	ldr	r0, [r0, #8]
3000ab3e:	4429      	add	r1, r5
3000ab40:	ea1a 0f02 	tst.w	sl, r2
3000ab44:	fa22 f20b 	lsr.w	r2, r2, fp
3000ab48:	fa21 f60b 	lsr.w	r6, r1, fp
3000ab4c:	fa20 f50b 	lsr.w	r5, r0, fp
3000ab50:	bf18      	it	ne
3000ab52:	3201      	addne	r2, #1
3000ab54:	ea1a 0f01 	tst.w	sl, r1
3000ab58:	6821      	ldr	r1, [r4, #0]
3000ab5a:	bf18      	it	ne
3000ab5c:	3601      	addne	r6, #1
3000ab5e:	ea1a 0f00 	tst.w	sl, r0
3000ab62:	7808      	ldrb	r0, [r1, #0]
3000ab64:	bf18      	it	ne
3000ab66:	3501      	addne	r5, #1
3000ab68:	68a1      	ldr	r1, [r4, #8]
3000ab6a:	3802      	subs	r0, #2
3000ab6c:	6dcf      	ldr	r7, [r1, #92]	; 0x5c
3000ab6e:	2801      	cmp	r0, #1
3000ab70:	f240 82ac 	bls.w	3000b0cc <rxi316_dram_init+0x848>
3000ab74:	698f      	ldr	r7, [r1, #24]
3000ab76:	fbb7 f0f3 	udiv	r0, r7, r3
3000ab7a:	fb03 7710 	mls	r7, r3, r0, r7
3000ab7e:	b107      	cbz	r7, 3000ab82 <rxi316_dram_init+0x2fe>
3000ab80:	3001      	adds	r0, #1
3000ab82:	6a4f      	ldr	r7, [r1, #36]	; 0x24
3000ab84:	fbb7 f1f3 	udiv	r1, r7, r3
3000ab88:	fb03 7711 	mls	r7, r3, r1, r7
3000ab8c:	b107      	cbz	r7, 3000ab90 <rxi316_dram_init+0x30c>
3000ab8e:	3101      	adds	r1, #1
3000ab90:	0176      	lsls	r6, r6, #5
3000ab92:	f002 021f 	and.w	r2, r2, #31
3000ab96:	02ad      	lsls	r5, r5, #10
3000ab98:	f406 7678 	and.w	r6, r6, #992	; 0x3e0
3000ab9c:	0500      	lsls	r0, r0, #20
3000ab9e:	f405 45f8 	and.w	r5, r5, #31744	; 0x7c00
3000aba2:	4316      	orrs	r6, r2
3000aba4:	f000 70f8 	and.w	r0, r0, #32505856	; 0x1f00000
3000aba8:	4a0e      	ldr	r2, [pc, #56]	; (3000abe4 <rxi316_dram_init+0x360>)
3000abaa:	432e      	orrs	r6, r5
3000abac:	4306      	orrs	r6, r0
3000abae:	0648      	lsls	r0, r1, #25
3000abb0:	f000 5078 	and.w	r0, r0, #1040187392	; 0x3e000000
3000abb4:	4330      	orrs	r0, r6
3000abb6:	6310      	str	r0, [r2, #48]	; 0x30
3000abb8:	68a0      	ldr	r0, [r4, #8]
3000abba:	6802      	ldr	r2, [r0, #0]
3000abbc:	fbb2 f1f3 	udiv	r1, r2, r3
3000abc0:	fb03 2211 	mls	r2, r3, r1, r2
3000abc4:	2a00      	cmp	r2, #0
3000abc6:	f000 827f 	beq.w	3000b0c8 <rxi316_dram_init+0x844>
3000abca:	1c4a      	adds	r2, r1, #1
3000abcc:	9205      	str	r2, [sp, #20]
3000abce:	6842      	ldr	r2, [r0, #4]
3000abd0:	fbb2 f0f3 	udiv	r0, r2, r3
3000abd4:	fb03 2210 	mls	r2, r3, r0, r2
3000abd8:	2a00      	cmp	r2, #0
3000abda:	f000 8273 	beq.w	3000b0c4 <rxi316_dram_init+0x840>
3000abde:	38c7      	subs	r0, #199	; 0xc7
3000abe0:	e002      	b.n	3000abe8 <rxi316_dram_init+0x364>
3000abe2:	bf00      	nop
3000abe4:	41100000 	.word	0x41100000
3000abe8:	68e2      	ldr	r2, [r4, #12]
3000abea:	6811      	ldr	r1, [r2, #0]
3000abec:	4ac3      	ldr	r2, [pc, #780]	; (3000aefc <rxi316_dram_init+0x678>)
3000abee:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000abf2:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000abf6:	900a      	str	r0, [sp, #40]	; 0x28
3000abf8:	0712      	lsls	r2, r2, #28
3000abfa:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000abfe:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000ac02:	430a      	orrs	r2, r1
3000ac04:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000ac08:	430a      	orrs	r2, r1
3000ac0a:	49bd      	ldr	r1, [pc, #756]	; (3000af00 <rxi316_dram_init+0x67c>)
3000ac0c:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000ac10:	610a      	str	r2, [r1, #16]
3000ac12:	68a5      	ldr	r5, [r4, #8]
3000ac14:	692a      	ldr	r2, [r5, #16]
3000ac16:	fbb2 fcf3 	udiv	ip, r2, r3
3000ac1a:	fb03 221c 	mls	r2, r3, ip, r2
3000ac1e:	2a00      	cmp	r2, #0
3000ac20:	f000 824d 	beq.w	3000b0be <rxi316_dram_init+0x83a>
3000ac24:	f10c 0201 	add.w	r2, ip, #1
3000ac28:	9206      	str	r2, [sp, #24]
3000ac2a:	69aa      	ldr	r2, [r5, #24]
3000ac2c:	fbb2 f0f3 	udiv	r0, r2, r3
3000ac30:	fb03 2210 	mls	r2, r3, r0, r2
3000ac34:	b102      	cbz	r2, 3000ac38 <rxi316_dram_init+0x3b4>
3000ac36:	3001      	adds	r0, #1
3000ac38:	68ea      	ldr	r2, [r5, #12]
3000ac3a:	fbb2 f1f3 	udiv	r1, r2, r3
3000ac3e:	fb03 2211 	mls	r2, r3, r1, r2
3000ac42:	9103      	str	r1, [sp, #12]
3000ac44:	b10a      	cbz	r2, 3000ac4a <rxi316_dram_init+0x3c6>
3000ac46:	3101      	adds	r1, #1
3000ac48:	9103      	str	r1, [sp, #12]
3000ac4a:	6a6a      	ldr	r2, [r5, #36]	; 0x24
3000ac4c:	fbb2 f1f3 	udiv	r1, r2, r3
3000ac50:	fb03 2211 	mls	r2, r3, r1, r2
3000ac54:	b102      	cbz	r2, 3000ac58 <rxi316_dram_init+0x3d4>
3000ac56:	3101      	adds	r1, #1
3000ac58:	6822      	ldr	r2, [r4, #0]
3000ac5a:	7812      	ldrb	r2, [r2, #0]
3000ac5c:	2a09      	cmp	r2, #9
3000ac5e:	f000 8259 	beq.w	3000b114 <rxi316_dram_init+0x890>
3000ac62:	2a02      	cmp	r2, #2
3000ac64:	d104      	bne.n	3000ac70 <rxi316_dram_init+0x3ec>
3000ac66:	9a08      	ldr	r2, [sp, #32]
3000ac68:	3a02      	subs	r2, #2
3000ac6a:	fa22 f20b 	lsr.w	r2, r2, fp
3000ac6e:	4411      	add	r1, r2
3000ac70:	68e2      	ldr	r2, [r4, #12]
3000ac72:	0349      	lsls	r1, r1, #13
3000ac74:	6d2d      	ldr	r5, [r5, #80]	; 0x50
3000ac76:	6812      	ldr	r2, [r2, #0]
3000ac78:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000ac7c:	fa25 f60b 	lsr.w	r6, r5, fp
3000ac80:	ea15 0f0a 	tst.w	r5, sl
3000ac84:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000ac86:	bf18      	it	ne
3000ac88:	3601      	addne	r6, #1
3000ac8a:	05d2      	lsls	r2, r2, #23
3000ac8c:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000ac90:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000ac94:	9a06      	ldr	r2, [sp, #24]
3000ac96:	0112      	lsls	r2, r2, #4
3000ac98:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000ac9c:	4332      	orrs	r2, r6
3000ac9e:	0246      	lsls	r6, r0, #9
3000aca0:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000aca4:	4316      	orrs	r6, r2
3000aca6:	9a03      	ldr	r2, [sp, #12]
3000aca8:	f002 020f 	and.w	r2, r2, #15
3000acac:	4316      	orrs	r6, r2
3000acae:	4a94      	ldr	r2, [pc, #592]	; (3000af00 <rxi316_dram_init+0x67c>)
3000acb0:	4331      	orrs	r1, r6
3000acb2:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000acb6:	6151      	str	r1, [r2, #20]
3000acb8:	68a7      	ldr	r7, [r4, #8]
3000acba:	697a      	ldr	r2, [r7, #20]
3000acbc:	fbb2 fef3 	udiv	lr, r2, r3
3000acc0:	fb03 221e 	mls	r2, r3, lr, r2
3000acc4:	b10a      	cbz	r2, 3000acca <rxi316_dram_init+0x446>
3000acc6:	f10e 0e01 	add.w	lr, lr, #1
3000acca:	6939      	ldr	r1, [r7, #16]
3000accc:	fbb1 f2f3 	udiv	r2, r1, r3
3000acd0:	fb03 1112 	mls	r1, r3, r2, r1
3000acd4:	b101      	cbz	r1, 3000acd8 <rxi316_dram_init+0x454>
3000acd6:	3201      	adds	r2, #1
3000acd8:	68fd      	ldr	r5, [r7, #12]
3000acda:	fbb5 f1f3 	udiv	r1, r5, r3
3000acde:	fb03 5511 	mls	r5, r3, r1, r5
3000ace2:	b105      	cbz	r5, 3000ace6 <rxi316_dram_init+0x462>
3000ace4:	3101      	adds	r1, #1
3000ace6:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000ace8:	fbb5 f6f3 	udiv	r6, r5, r3
3000acec:	fb03 5516 	mls	r5, r3, r6, r5
3000acf0:	b105      	cbz	r5, 3000acf4 <rxi316_dram_init+0x470>
3000acf2:	3601      	adds	r6, #1
3000acf4:	68bd      	ldr	r5, [r7, #8]
3000acf6:	4411      	add	r1, r2
3000acf8:	fbb5 f2f3 	udiv	r2, r5, r3
3000acfc:	42b1      	cmp	r1, r6
3000acfe:	fb03 5512 	mls	r5, r3, r2, r5
3000ad02:	bf38      	it	cc
3000ad04:	4631      	movcc	r1, r6
3000ad06:	b905      	cbnz	r5, 3000ad0a <rxi316_dram_init+0x486>
3000ad08:	3a01      	subs	r2, #1
3000ad0a:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000ad0c:	ea15 0f0a 	tst.w	r5, sl
3000ad10:	fa25 fc0b 	lsr.w	ip, r5, fp
3000ad14:	69fd      	ldr	r5, [r7, #28]
3000ad16:	bf18      	it	ne
3000ad18:	f10c 0c01 	addne.w	ip, ip, #1
3000ad1c:	fa25 f60b 	lsr.w	r6, r5, fp
3000ad20:	ea15 0f0a 	tst.w	r5, sl
3000ad24:	bf18      	it	ne
3000ad26:	3601      	addne	r6, #1
3000ad28:	f1b9 0f09 	cmp.w	r9, #9
3000ad2c:	f000 81ed 	beq.w	3000b10a <rxi316_dram_init+0x886>
3000ad30:	f1b9 0f02 	cmp.w	r9, #2
3000ad34:	f000 81f4 	beq.w	3000b120 <rxi316_dram_init+0x89c>
3000ad38:	f1b9 0f03 	cmp.w	r9, #3
3000ad3c:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000ad3e:	f000 827d 	beq.w	3000b23c <rxi316_dram_init+0x9b8>
3000ad42:	46a8      	mov	r8, r5
3000ad44:	9d01      	ldr	r5, [sp, #4]
3000ad46:	44a8      	add	r8, r5
3000ad48:	fa28 f50b 	lsr.w	r5, r8, fp
3000ad4c:	ea18 0f0a 	tst.w	r8, sl
3000ad50:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000ad54:	bf18      	it	ne
3000ad56:	3501      	addne	r5, #1
3000ad58:	fbb8 f7f3 	udiv	r7, r8, r3
3000ad5c:	fb03 8817 	mls	r8, r3, r7, r8
3000ad60:	f1b8 0f00 	cmp.w	r8, #0
3000ad64:	d000      	beq.n	3000ad68 <rxi316_dram_init+0x4e4>
3000ad66:	3701      	adds	r7, #1
3000ad68:	0109      	lsls	r1, r1, #4
3000ad6a:	f00e 0e0f 	and.w	lr, lr, #15
3000ad6e:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000ad72:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000ad76:	063f      	lsls	r7, r7, #24
3000ad78:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000ad7c:	ea41 0e0e 	orr.w	lr, r1, lr
3000ad80:	0291      	lsls	r1, r2, #10
3000ad82:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000ad86:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000ad8a:	ea4e 0e01 	orr.w	lr, lr, r1
3000ad8e:	0471      	lsls	r1, r6, #17
3000ad90:	ea4e 0c0c 	orr.w	ip, lr, ip
3000ad94:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000ad98:	ea4c 0c01 	orr.w	ip, ip, r1
3000ad9c:	0529      	lsls	r1, r5, #20
3000ad9e:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000ada2:	ea4c 0c01 	orr.w	ip, ip, r1
3000ada6:	4956      	ldr	r1, [pc, #344]	; (3000af00 <rxi316_dram_init+0x67c>)
3000ada8:	ea4c 0707 	orr.w	r7, ip, r7
3000adac:	618f      	str	r7, [r1, #24]
3000adae:	6821      	ldr	r1, [r4, #0]
3000adb0:	780f      	ldrb	r7, [r1, #0]
3000adb2:	2f02      	cmp	r7, #2
3000adb4:	f000 8153 	beq.w	3000b05e <rxi316_dram_init+0x7da>
3000adb8:	2f09      	cmp	r7, #9
3000adba:	f000 8150 	beq.w	3000b05e <rxi316_dram_init+0x7da>
3000adbe:	68e1      	ldr	r1, [r4, #12]
3000adc0:	2f03      	cmp	r7, #3
3000adc2:	6809      	ldr	r1, [r1, #0]
3000adc4:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000adc6:	f001 080f 	and.w	r8, r1, #15
3000adca:	68a1      	ldr	r1, [r4, #8]
3000adcc:	6a09      	ldr	r1, [r1, #32]
3000adce:	d005      	beq.n	3000addc <rxi316_dram_init+0x558>
3000add0:	ea0a 0701 	and.w	r7, sl, r1
3000add4:	fa21 f10b 	lsr.w	r1, r1, fp
3000add8:	b107      	cbz	r7, 3000addc <rxi316_dram_init+0x558>
3000adda:	3101      	adds	r1, #1
3000addc:	f1b9 0f02 	cmp.w	r9, #2
3000ade0:	f000 8155 	beq.w	3000b08e <rxi316_dram_init+0x80a>
3000ade4:	f1b9 0f03 	cmp.w	r9, #3
3000ade8:	f000 819d 	beq.w	3000b126 <rxi316_dram_init+0x8a2>
3000adec:	f1b9 0f09 	cmp.w	r9, #9
3000adf0:	f000 8214 	beq.w	3000b21c <rxi316_dram_init+0x998>
3000adf4:	f04f 0e00 	mov.w	lr, #0
3000adf8:	46f1      	mov	r9, lr
3000adfa:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000adfe:	0109      	lsls	r1, r1, #4
3000ae00:	9f06      	ldr	r7, [sp, #24]
3000ae02:	b2c9      	uxtb	r1, r1
3000ae04:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000ae08:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000ae0a:	ea41 0108 	orr.w	r1, r1, r8
3000ae0e:	4339      	orrs	r1, r7
3000ae10:	4f3b      	ldr	r7, [pc, #236]	; (3000af00 <rxi316_dram_init+0x67c>)
3000ae12:	ea41 0109 	orr.w	r1, r1, r9
3000ae16:	ea41 010e 	orr.w	r1, r1, lr
3000ae1a:	61f9      	str	r1, [r7, #28]
3000ae1c:	68a7      	ldr	r7, [r4, #8]
3000ae1e:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000ae20:	fbb4 f1f3 	udiv	r1, r4, r3
3000ae24:	fb03 4311 	mls	r3, r3, r1, r4
3000ae28:	b103      	cbz	r3, 3000ae2c <rxi316_dram_init+0x5a8>
3000ae2a:	3101      	adds	r1, #1
3000ae2c:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000ae2e:	094b      	lsrs	r3, r1, #5
3000ae30:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000ae34:	0912      	lsrs	r2, r2, #4
3000ae36:	ea14 0f0a 	tst.w	r4, sl
3000ae3a:	fa24 f80b 	lsr.w	r8, r4, fp
3000ae3e:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000ae40:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000ae44:	bf18      	it	ne
3000ae46:	f108 0801 	addne.w	r8, r8, #1
3000ae4a:	9306      	str	r3, [sp, #24]
3000ae4c:	ea14 0f0a 	tst.w	r4, sl
3000ae50:	fa24 fe0b 	lsr.w	lr, r4, fp
3000ae54:	9b01      	ldr	r3, [sp, #4]
3000ae56:	fa29 f40b 	lsr.w	r4, r9, fp
3000ae5a:	bf18      	it	ne
3000ae5c:	f10e 0e01 	addne.w	lr, lr, #1
3000ae60:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000ae62:	ea19 0f0a 	tst.w	r9, sl
3000ae66:	fa23 f90b 	lsr.w	r9, r3, fp
3000ae6a:	9905      	ldr	r1, [sp, #20]
3000ae6c:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000ae70:	bf18      	it	ne
3000ae72:	3401      	addne	r4, #1
3000ae74:	45b9      	cmp	r9, r7
3000ae76:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000ae7a:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000ae7e:	f240 80ec 	bls.w	3000b05a <rxi316_dram_init+0x7d6>
3000ae82:	eba9 0707 	sub.w	r7, r9, r7
3000ae86:	fa27 f40b 	lsr.w	r4, r7, fp
3000ae8a:	ea17 070a 	ands.w	r7, r7, sl
3000ae8e:	d001      	beq.n	3000ae94 <rxi316_dram_init+0x610>
3000ae90:	3401      	adds	r4, #1
3000ae92:	2700      	movs	r7, #0
3000ae94:	9b04      	ldr	r3, [sp, #16]
3000ae96:	b10b      	cbz	r3, 3000ae9c <rxi316_dram_init+0x618>
3000ae98:	073b      	lsls	r3, r7, #28
3000ae9a:	9304      	str	r3, [sp, #16]
3000ae9c:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000aea0:	9b06      	ldr	r3, [sp, #24]
3000aea2:	0289      	lsls	r1, r1, #10
3000aea4:	02c0      	lsls	r0, r0, #11
3000aea6:	f00c 0c04 	and.w	ip, ip, #4
3000aeaa:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000aeae:	01d2      	lsls	r2, r2, #7
3000aeb0:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000aeb4:	0236      	lsls	r6, r6, #8
3000aeb6:	ea4c 0c01 	orr.w	ip, ip, r1
3000aeba:	b2d2      	uxtb	r2, r2
3000aebc:	026d      	lsls	r5, r5, #9
3000aebe:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000aec2:	ea4c 0c00 	orr.w	ip, ip, r0
3000aec6:	00df      	lsls	r7, r3, #3
3000aec8:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000aecc:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000aed0:	ea4c 0202 	orr.w	r2, ip, r2
3000aed4:	f007 0708 	and.w	r7, r7, #8
3000aed8:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000aedc:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000aee0:	4332      	orrs	r2, r6
3000aee2:	0524      	lsls	r4, r4, #20
3000aee4:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000aee8:	4905      	ldr	r1, [pc, #20]	; (3000af00 <rxi316_dram_init+0x67c>)
3000aeea:	ea42 0305 	orr.w	r3, r2, r5
3000aeee:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000aef2:	9a04      	ldr	r2, [sp, #16]
3000aef4:	433b      	orrs	r3, r7
3000aef6:	ea43 0308 	orr.w	r3, r3, r8
3000aefa:	e003      	b.n	3000af04 <rxi316_dram_init+0x680>
3000aefc:	00ffff00 	.word	0x00ffff00
3000af00:	41100000 	.word	0x41100000
3000af04:	ea43 030e 	orr.w	r3, r3, lr
3000af08:	4323      	orrs	r3, r4
3000af0a:	4313      	orrs	r3, r2
3000af0c:	9a03      	ldr	r2, [sp, #12]
3000af0e:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000af12:	620b      	str	r3, [r1, #32]
3000af14:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000af18:	9b02      	ldr	r3, [sp, #8]
3000af1a:	2b01      	cmp	r3, #1
3000af1c:	d002      	beq.n	3000af24 <rxi316_dram_init+0x6a0>
3000af1e:	9b08      	ldr	r3, [sp, #32]
3000af20:	3303      	adds	r3, #3
3000af22:	9307      	str	r3, [sp, #28]
3000af24:	4bb2      	ldr	r3, [pc, #712]	; (3000b1f0 <rxi316_dram_init+0x96c>)
3000af26:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000af2a:	9a07      	ldr	r2, [sp, #28]
3000af2c:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000af30:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000af34:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000af38:	60da      	str	r2, [r3, #12]
3000af3a:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000af3e:	430a      	orrs	r2, r1
3000af40:	49ac      	ldr	r1, [pc, #688]	; (3000b1f4 <rxi316_dram_init+0x970>)
3000af42:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000af46:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000af4a:	0096      	lsls	r6, r2, #2
3000af4c:	d57f      	bpl.n	3000b04e <rxi316_dram_init+0x7ca>
3000af4e:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000af52:	4ca7      	ldr	r4, [pc, #668]	; (3000b1f0 <rxi316_dram_init+0x96c>)
3000af54:	601a      	str	r2, [r3, #0]
3000af56:	691a      	ldr	r2, [r3, #16]
3000af58:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000af5c:	611a      	str	r2, [r3, #16]
3000af5e:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000af62:	f043 0303 	orr.w	r3, r3, #3
3000af66:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000af6a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000af6e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000af70:	2b01      	cmp	r3, #1
3000af72:	4ba1      	ldr	r3, [pc, #644]	; (3000b1f8 <rxi316_dram_init+0x974>)
3000af74:	bf0c      	ite	eq
3000af76:	4aa1      	ldreq	r2, [pc, #644]	; (3000b1fc <rxi316_dram_init+0x978>)
3000af78:	4aa1      	ldrne	r2, [pc, #644]	; (3000b200 <rxi316_dram_init+0x97c>)
3000af7a:	601a      	str	r2, [r3, #0]
3000af7c:	2203      	movs	r2, #3
3000af7e:	68e0      	ldr	r0, [r4, #12]
3000af80:	4ba0      	ldr	r3, [pc, #640]	; (3000b204 <rxi316_dram_init+0x980>)
3000af82:	4003      	ands	r3, r0
3000af84:	60e3      	str	r3, [r4, #12]
3000af86:	2308      	movs	r3, #8
3000af88:	62a1      	str	r1, [r4, #40]	; 0x28
3000af8a:	62e2      	str	r2, [r4, #44]	; 0x2c
3000af8c:	6023      	str	r3, [r4, #0]
3000af8e:	6823      	ldr	r3, [r4, #0]
3000af90:	071d      	lsls	r5, r3, #28
3000af92:	d5fc      	bpl.n	3000af8e <rxi316_dram_init+0x70a>
3000af94:	4b9c      	ldr	r3, [pc, #624]	; (3000b208 <rxi316_dram_init+0x984>)
3000af96:	2001      	movs	r0, #1
3000af98:	4798      	blx	r3
3000af9a:	68e2      	ldr	r2, [r4, #12]
3000af9c:	4b99      	ldr	r3, [pc, #612]	; (3000b204 <rxi316_dram_init+0x980>)
3000af9e:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000afa2:	4013      	ands	r3, r2
3000afa4:	2203      	movs	r2, #3
3000afa6:	60e3      	str	r3, [r4, #12]
3000afa8:	2308      	movs	r3, #8
3000afaa:	62a1      	str	r1, [r4, #40]	; 0x28
3000afac:	62e2      	str	r2, [r4, #44]	; 0x2c
3000afae:	4a90      	ldr	r2, [pc, #576]	; (3000b1f0 <rxi316_dram_init+0x96c>)
3000afb0:	6023      	str	r3, [r4, #0]
3000afb2:	6813      	ldr	r3, [r2, #0]
3000afb4:	071c      	lsls	r4, r3, #28
3000afb6:	d5fc      	bpl.n	3000afb2 <rxi316_dram_init+0x72e>
3000afb8:	68d0      	ldr	r0, [r2, #12]
3000afba:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000afbe:	4b91      	ldr	r3, [pc, #580]	; (3000b204 <rxi316_dram_init+0x980>)
3000afc0:	4003      	ands	r3, r0
3000afc2:	2008      	movs	r0, #8
3000afc4:	60d3      	str	r3, [r2, #12]
3000afc6:	6291      	str	r1, [r2, #40]	; 0x28
3000afc8:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000afca:	4989      	ldr	r1, [pc, #548]	; (3000b1f0 <rxi316_dram_init+0x96c>)
3000afcc:	f023 0302 	bic.w	r3, r3, #2
3000afd0:	62d3      	str	r3, [r2, #44]	; 0x2c
3000afd2:	6010      	str	r0, [r2, #0]
3000afd4:	680b      	ldr	r3, [r1, #0]
3000afd6:	0718      	lsls	r0, r3, #28
3000afd8:	d5fc      	bpl.n	3000afd4 <rxi316_dram_init+0x750>
3000afda:	4a85      	ldr	r2, [pc, #532]	; (3000b1f0 <rxi316_dram_init+0x96c>)
3000afdc:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000afe0:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000afe4:	60d3      	str	r3, [r2, #12]
3000afe6:	980a      	ldr	r0, [sp, #40]	; 0x28
3000afe8:	6913      	ldr	r3, [r2, #16]
3000afea:	4318      	orrs	r0, r3
3000afec:	6110      	str	r0, [r2, #16]
3000afee:	6913      	ldr	r3, [r2, #16]
3000aff0:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000aff4:	6113      	str	r3, [r2, #16]
3000aff6:	6011      	str	r1, [r2, #0]
3000aff8:	68d3      	ldr	r3, [r2, #12]
3000affa:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000affe:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000b002:	d1f9      	bne.n	3000aff8 <rxi316_dram_init+0x774>
3000b004:	b00f      	add	sp, #60	; 0x3c
3000b006:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000b00a:	2a01      	cmp	r2, #1
3000b00c:	f000 80b4 	beq.w	3000b178 <rxi316_dram_init+0x8f4>
3000b010:	2a02      	cmp	r2, #2
3000b012:	f000 811d 	beq.w	3000b250 <rxi316_dram_init+0x9cc>
3000b016:	2a03      	cmp	r2, #3
3000b018:	f47f acda 	bne.w	3000a9d0 <rxi316_dram_init+0x14c>
3000b01c:	686a      	ldr	r2, [r5, #4]
3000b01e:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b022:	606a      	str	r2, [r5, #4]
3000b024:	e4d4      	b.n	3000a9d0 <rxi316_dram_init+0x14c>
3000b026:	2a00      	cmp	r2, #0
3000b028:	f43f ad60 	beq.w	3000aaec <rxi316_dram_init+0x268>
3000b02c:	2203      	movs	r2, #3
3000b02e:	4310      	orrs	r0, r2
3000b030:	68a2      	ldr	r2, [r4, #8]
3000b032:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000b034:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b036:	013f      	lsls	r7, r7, #4
3000b038:	2d00      	cmp	r5, #0
3000b03a:	f000 80ba 	beq.w	3000b1b2 <rxi316_dram_init+0x92e>
3000b03e:	1e91      	subs	r1, r2, #2
3000b040:	2906      	cmp	r1, #6
3000b042:	f200 8093 	bhi.w	3000b16c <rxi316_dram_init+0x8e8>
3000b046:	1e55      	subs	r5, r2, #1
3000b048:	2100      	movs	r1, #0
3000b04a:	026d      	lsls	r5, r5, #9
3000b04c:	e555      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b04e:	2201      	movs	r2, #1
3000b050:	601a      	str	r2, [r3, #0]
3000b052:	681a      	ldr	r2, [r3, #0]
3000b054:	07d2      	lsls	r2, r2, #31
3000b056:	d5fc      	bpl.n	3000b052 <rxi316_dram_init+0x7ce>
3000b058:	e7bf      	b.n	3000afda <rxi316_dram_init+0x756>
3000b05a:	2701      	movs	r7, #1
3000b05c:	e71a      	b.n	3000ae94 <rxi316_dram_init+0x610>
3000b05e:	68a1      	ldr	r1, [r4, #8]
3000b060:	2f03      	cmp	r7, #3
3000b062:	6a09      	ldr	r1, [r1, #32]
3000b064:	f000 80a2 	beq.w	3000b1ac <rxi316_dram_init+0x928>
3000b068:	2f02      	cmp	r7, #2
3000b06a:	f040 80fc 	bne.w	3000b266 <rxi316_dram_init+0x9e2>
3000b06e:	fbb1 fcf3 	udiv	ip, r1, r3
3000b072:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b076:	4661      	mov	r1, ip
3000b078:	f1be 0f00 	cmp.w	lr, #0
3000b07c:	f000 808c 	beq.w	3000b198 <rxi316_dram_init+0x914>
3000b080:	f1b9 0f02 	cmp.w	r9, #2
3000b084:	f101 0101 	add.w	r1, r1, #1
3000b088:	46b8      	mov	r8, r7
3000b08a:	f47f aeab 	bne.w	3000ade4 <rxi316_dram_init+0x560>
3000b08e:	4f5f      	ldr	r7, [pc, #380]	; (3000b20c <rxi316_dram_init+0x988>)
3000b090:	f04f 0e00 	mov.w	lr, #0
3000b094:	f8df 9178 	ldr.w	r9, [pc, #376]	; 3000b210 <rxi316_dram_init+0x98c>
3000b098:	fbb7 f7f3 	udiv	r7, r7, r3
3000b09c:	fbb9 f9f3 	udiv	r9, r9, r3
3000b0a0:	09ff      	lsrs	r7, r7, #7
3000b0a2:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b0a6:	3701      	adds	r7, #1
3000b0a8:	f109 0c01 	add.w	ip, r9, #1
3000b0ac:	f8df 9164 	ldr.w	r9, [pc, #356]	; 3000b214 <rxi316_dram_init+0x990>
3000b0b0:	023f      	lsls	r7, r7, #8
3000b0b2:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b0b6:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b0ba:	9709      	str	r7, [sp, #36]	; 0x24
3000b0bc:	e69f      	b.n	3000adfe <rxi316_dram_init+0x57a>
3000b0be:	f8cd c018 	str.w	ip, [sp, #24]
3000b0c2:	e5b2      	b.n	3000ac2a <rxi316_dram_init+0x3a6>
3000b0c4:	38c8      	subs	r0, #200	; 0xc8
3000b0c6:	e58f      	b.n	3000abe8 <rxi316_dram_init+0x364>
3000b0c8:	9105      	str	r1, [sp, #20]
3000b0ca:	e580      	b.n	3000abce <rxi316_dram_init+0x34a>
3000b0cc:	ea1a 0f07 	tst.w	sl, r7
3000b0d0:	fa27 f00b 	lsr.w	r0, r7, fp
3000b0d4:	f43f ad55 	beq.w	3000ab82 <rxi316_dram_init+0x2fe>
3000b0d8:	e552      	b.n	3000ab80 <rxi316_dram_init+0x2fc>
3000b0da:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b0dc:	1f50      	subs	r0, r2, #5
3000b0de:	00c0      	lsls	r0, r0, #3
3000b0e0:	68e2      	ldr	r2, [r4, #12]
3000b0e2:	6812      	ldr	r2, [r2, #0]
3000b0e4:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b0e6:	fab2 f282 	clz	r2, r2
3000b0ea:	0952      	lsrs	r2, r2, #5
3000b0ec:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b0f0:	4301      	orrs	r1, r0
3000b0f2:	e519      	b.n	3000ab28 <rxi316_dram_init+0x2a4>
3000b0f4:	f1a1 0502 	sub.w	r5, r1, #2
3000b0f8:	2909      	cmp	r1, #9
3000b0fa:	fab5 f585 	clz	r5, r5
3000b0fe:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b102:	f000 80a3 	beq.w	3000b24c <rxi316_dram_init+0x9c8>
3000b106:	462a      	mov	r2, r5
3000b108:	e4ed      	b.n	3000aae6 <rxi316_dram_init+0x262>
3000b10a:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b10c:	46a8      	mov	r8, r5
3000b10e:	9d08      	ldr	r5, [sp, #32]
3000b110:	44a8      	add	r8, r5
3000b112:	e619      	b.n	3000ad48 <rxi316_dram_init+0x4c4>
3000b114:	9a08      	ldr	r2, [sp, #32]
3000b116:	fa22 f10b 	lsr.w	r1, r2, fp
3000b11a:	e5a9      	b.n	3000ac70 <rxi316_dram_init+0x3ec>
3000b11c:	2000      	movs	r0, #0
3000b11e:	e7df      	b.n	3000b0e0 <rxi316_dram_init+0x85c>
3000b120:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b124:	e610      	b.n	3000ad48 <rxi316_dram_init+0x4c4>
3000b126:	f242 7710 	movw	r7, #10000	; 0x2710
3000b12a:	f8df 90ec 	ldr.w	r9, [pc, #236]	; 3000b218 <rxi316_dram_init+0x994>
3000b12e:	f8df e0e0 	ldr.w	lr, [pc, #224]	; 3000b210 <rxi316_dram_init+0x98c>
3000b132:	fbb7 fcf3 	udiv	ip, r7, r3
3000b136:	9f05      	ldr	r7, [sp, #20]
3000b138:	4467      	add	r7, ip
3000b13a:	fbb9 f9f3 	udiv	r9, r9, r3
3000b13e:	fbbe fef3 	udiv	lr, lr, r3
3000b142:	09ff      	lsrs	r7, r7, #7
3000b144:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b148:	3701      	adds	r7, #1
3000b14a:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b14e:	f109 0c01 	add.w	ip, r9, #1
3000b152:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000b214 <rxi316_dram_init+0x990>
3000b156:	023f      	lsls	r7, r7, #8
3000b158:	f10e 0e01 	add.w	lr, lr, #1
3000b15c:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b160:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b164:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b168:	9709      	str	r7, [sp, #36]	; 0x24
3000b16a:	e648      	b.n	3000adfe <rxi316_dram_init+0x57a>
3000b16c:	2100      	movs	r1, #0
3000b16e:	460d      	mov	r5, r1
3000b170:	e4c3      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b172:	3a07      	subs	r2, #7
3000b174:	f7ff bbcb 	b.w	3000a90e <rxi316_dram_init+0x8a>
3000b178:	686a      	ldr	r2, [r5, #4]
3000b17a:	f042 0220 	orr.w	r2, r2, #32
3000b17e:	606a      	str	r2, [r5, #4]
3000b180:	e426      	b.n	3000a9d0 <rxi316_dram_init+0x14c>
3000b182:	2204      	movs	r2, #4
3000b184:	2102      	movs	r1, #2
3000b186:	2500      	movs	r5, #0
3000b188:	9201      	str	r2, [sp, #4]
3000b18a:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b18e:	f7ff bba7 	b.w	3000a8e0 <rxi316_dram_init+0x5c>
3000b192:	3a08      	subs	r2, #8
3000b194:	f7ff bbbb 	b.w	3000a90e <rxi316_dram_init+0x8a>
3000b198:	46b8      	mov	r8, r7
3000b19a:	e61f      	b.n	3000addc <rxi316_dram_init+0x558>
3000b19c:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b19e:	2a04      	cmp	r2, #4
3000b1a0:	d90c      	bls.n	3000b1bc <rxi316_dram_init+0x938>
3000b1a2:	2a0b      	cmp	r2, #11
3000b1a4:	d86b      	bhi.n	3000b27e <rxi316_dram_init+0x9fa>
3000b1a6:	1f17      	subs	r7, r2, #4
3000b1a8:	013f      	lsls	r7, r7, #4
3000b1aa:	e008      	b.n	3000b1be <rxi316_dram_init+0x93a>
3000b1ac:	f04f 0802 	mov.w	r8, #2
3000b1b0:	e614      	b.n	3000addc <rxi316_dram_init+0x558>
3000b1b2:	2903      	cmp	r1, #3
3000b1b4:	d005      	beq.n	3000b1c2 <rxi316_dram_init+0x93e>
3000b1b6:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b1ba:	e49e      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b1bc:	2710      	movs	r7, #16
3000b1be:	68a2      	ldr	r2, [r4, #8]
3000b1c0:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b1c2:	2a04      	cmp	r2, #4
3000b1c4:	d94a      	bls.n	3000b25c <rxi316_dram_init+0x9d8>
3000b1c6:	1f51      	subs	r1, r2, #5
3000b1c8:	2903      	cmp	r1, #3
3000b1ca:	d84f      	bhi.n	3000b26c <rxi316_dram_init+0x9e8>
3000b1cc:	1f15      	subs	r5, r2, #4
3000b1ce:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b1d2:	026d      	lsls	r5, r5, #9
3000b1d4:	e491      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b1d6:	f1a1 0c02 	sub.w	ip, r1, #2
3000b1da:	f1bc 0f01 	cmp.w	ip, #1
3000b1de:	d903      	bls.n	3000b1e8 <rxi316_dram_init+0x964>
3000b1e0:	f1be 0f00 	cmp.w	lr, #0
3000b1e4:	f43f ac7f 	beq.w	3000aae6 <rxi316_dram_init+0x262>
3000b1e8:	f040 0002 	orr.w	r0, r0, #2
3000b1ec:	e47b      	b.n	3000aae6 <rxi316_dram_init+0x262>
3000b1ee:	bf00      	nop
3000b1f0:	41100000 	.word	0x41100000
3000b1f4:	42008000 	.word	0x42008000
3000b1f8:	41011000 	.word	0x41011000
3000b1fc:	44000103 	.word	0x44000103
3000b200:	44000021 	.word	0x44000021
3000b204:	fff8fffc 	.word	0xfff8fffc
3000b208:	00009b2d 	.word	0x00009b2d
3000b20c:	00061a80 	.word	0x00061a80
3000b210:	0bebc200 	.word	0x0bebc200
3000b214:	003ff000 	.word	0x003ff000
3000b218:	1dcd6500 	.word	0x1dcd6500
3000b21c:	4f1d      	ldr	r7, [pc, #116]	; (3000b294 <rxi316_dram_init+0xa10>)
3000b21e:	f04f 0e00 	mov.w	lr, #0
3000b222:	fbb7 f9f3 	udiv	r9, r7, r3
3000b226:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b22a:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b22e:	f109 0701 	add.w	r7, r9, #1
3000b232:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b298 <rxi316_dram_init+0xa14>
3000b236:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b23a:	e5e0      	b.n	3000adfe <rxi316_dram_init+0x57a>
3000b23c:	f105 0802 	add.w	r8, r5, #2
3000b240:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b242:	eba8 0805 	sub.w	r8, r8, r5
3000b246:	9d08      	ldr	r5, [sp, #32]
3000b248:	44a8      	add	r8, r5
3000b24a:	e57d      	b.n	3000ad48 <rxi316_dram_init+0x4c4>
3000b24c:	2204      	movs	r2, #4
3000b24e:	e6ee      	b.n	3000b02e <rxi316_dram_init+0x7aa>
3000b250:	686a      	ldr	r2, [r5, #4]
3000b252:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b256:	606a      	str	r2, [r5, #4]
3000b258:	f7ff bbba 	b.w	3000a9d0 <rxi316_dram_init+0x14c>
3000b25c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b260:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b264:	e449      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b266:	f04f 0802 	mov.w	r8, #2
3000b26a:	e5b1      	b.n	3000add0 <rxi316_dram_init+0x54c>
3000b26c:	f1a2 0109 	sub.w	r1, r2, #9
3000b270:	2905      	cmp	r1, #5
3000b272:	d80b      	bhi.n	3000b28c <rxi316_dram_init+0xa08>
3000b274:	0855      	lsrs	r5, r2, #1
3000b276:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b27a:	026d      	lsls	r5, r5, #9
3000b27c:	e43d      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b27e:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b280:	f040 0004 	orr.w	r0, r0, #4
3000b284:	f1a2 070c 	sub.w	r7, r2, #12
3000b288:	013f      	lsls	r7, r7, #4
3000b28a:	e798      	b.n	3000b1be <rxi316_dram_init+0x93a>
3000b28c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b290:	2500      	movs	r5, #0
3000b292:	e432      	b.n	3000aafa <rxi316_dram_init+0x276>
3000b294:	0bebc200 	.word	0x0bebc200
3000b298:	003ff000 	.word	0x003ff000

3000b29c <rxi316_ftend_init>:
3000b29c:	2300      	movs	r3, #0
3000b29e:	b510      	push	{r4, lr}
3000b2a0:	4c0c      	ldr	r4, [pc, #48]	; (3000b2d4 <rxi316_ftend_init+0x38>)
3000b2a2:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b2a6:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b2aa:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b2ae:	f7fe fe5d 	bl	30009f6c <ddr_init_index>
3000b2b2:	4a09      	ldr	r2, [pc, #36]	; (3000b2d8 <rxi316_ftend_init+0x3c>)
3000b2b4:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b2b8:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b2bc:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b2c0:	68cb      	ldr	r3, [r1, #12]
3000b2c2:	689b      	ldr	r3, [r3, #8]
3000b2c4:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b2c6:	3301      	adds	r3, #1
3000b2c8:	b2db      	uxtb	r3, r3
3000b2ca:	4313      	orrs	r3, r2
3000b2cc:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b2d0:	bd10      	pop	{r4, pc}
3000b2d2:	bf00      	nop
3000b2d4:	41101000 	.word	0x41101000
3000b2d8:	3000f598 	.word	0x3000f598

3000b2dc <rxi316_DynSre_init>:
3000b2dc:	b538      	push	{r3, r4, r5, lr}
3000b2de:	4b22      	ldr	r3, [pc, #136]	; (3000b368 <rxi316_DynSre_init+0x8c>)
3000b2e0:	4604      	mov	r4, r0
3000b2e2:	460d      	mov	r5, r1
3000b2e4:	4798      	blx	r3
3000b2e6:	2802      	cmp	r0, #2
3000b2e8:	d937      	bls.n	3000b35a <rxi316_DynSre_init+0x7e>
3000b2ea:	4b20      	ldr	r3, [pc, #128]	; (3000b36c <rxi316_DynSre_init+0x90>)
3000b2ec:	691b      	ldr	r3, [r3, #16]
3000b2ee:	f3c3 230f 	ubfx	r3, r3, #8, #16
3000b2f2:	3b46      	subs	r3, #70	; 0x46
3000b2f4:	42a3      	cmp	r3, r4
3000b2f6:	d915      	bls.n	3000b324 <rxi316_DynSre_init+0x48>
3000b2f8:	2d01      	cmp	r5, #1
3000b2fa:	d01a      	beq.n	3000b332 <rxi316_DynSre_init+0x56>
3000b2fc:	4a1b      	ldr	r2, [pc, #108]	; (3000b36c <rxi316_DynSre_init+0x90>)
3000b2fe:	6893      	ldr	r3, [r2, #8]
3000b300:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b304:	6093      	str	r3, [r2, #8]
3000b306:	4b19      	ldr	r3, [pc, #100]	; (3000b36c <rxi316_DynSre_init+0x90>)
3000b308:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b30c:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b310:	625a      	str	r2, [r3, #36]	; 0x24
3000b312:	629a      	str	r2, [r3, #40]	; 0x28
3000b314:	69da      	ldr	r2, [r3, #28]
3000b316:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b31a:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b31e:	61da      	str	r2, [r3, #28]
3000b320:	6019      	str	r1, [r3, #0]
3000b322:	bd38      	pop	{r3, r4, r5, pc}
3000b324:	f240 411e 	movw	r1, #1054	; 0x41e
3000b328:	4811      	ldr	r0, [pc, #68]	; (3000b370 <rxi316_DynSre_init+0x94>)
3000b32a:	f001 f905 	bl	3000c538 <__io_assert_failed_veneer>
3000b32e:	2d01      	cmp	r5, #1
3000b330:	d1e4      	bne.n	3000b2fc <rxi316_DynSre_init+0x20>
3000b332:	4b0e      	ldr	r3, [pc, #56]	; (3000b36c <rxi316_DynSre_init+0x90>)
3000b334:	f64f 70fe 	movw	r0, #65534	; 0xfffe
3000b338:	4a0e      	ldr	r2, [pc, #56]	; (3000b374 <rxi316_DynSre_init+0x98>)
3000b33a:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b33e:	4004      	ands	r4, r0
3000b340:	400a      	ands	r2, r1
3000b342:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b346:	f8d3 0100 	ldr.w	r0, [r3, #256]	; 0x100
3000b34a:	4304      	orrs	r4, r0
3000b34c:	f8c3 4100 	str.w	r4, [r3, #256]	; 0x100
3000b350:	689a      	ldr	r2, [r3, #8]
3000b352:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b356:	609a      	str	r2, [r3, #8]
3000b358:	e7d5      	b.n	3000b306 <rxi316_DynSre_init+0x2a>
3000b35a:	4b07      	ldr	r3, [pc, #28]	; (3000b378 <rxi316_DynSre_init+0x9c>)
3000b35c:	4798      	blx	r3
3000b35e:	2801      	cmp	r0, #1
3000b360:	bf08      	it	eq
3000b362:	2400      	moveq	r4, #0
3000b364:	e7c8      	b.n	3000b2f8 <rxi316_DynSre_init+0x1c>
3000b366:	bf00      	nop
3000b368:	30009c5d 	.word	0x30009c5d
3000b36c:	41100000 	.word	0x41100000
3000b370:	3000d994 	.word	0x3000d994
3000b374:	ffff0000 	.word	0xffff0000
3000b378:	30007fa9 	.word	0x30007fa9

3000b37c <ddr_init>:
3000b37c:	b500      	push	{lr}
3000b37e:	b083      	sub	sp, #12
3000b380:	f7fe fdf4 	bl	30009f6c <ddr_init_index>
3000b384:	4b06      	ldr	r3, [pc, #24]	; (3000b3a0 <ddr_init+0x24>)
3000b386:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b38a:	9001      	str	r0, [sp, #4]
3000b38c:	f7fe fe18 	bl	30009fc0 <rxi316_perf_tune>
3000b390:	9801      	ldr	r0, [sp, #4]
3000b392:	f7ff fa77 	bl	3000a884 <rxi316_dram_init>
3000b396:	b003      	add	sp, #12
3000b398:	f85d eb04 	ldr.w	lr, [sp], #4
3000b39c:	f7ff bf7e 	b.w	3000b29c <rxi316_ftend_init>
3000b3a0:	3000f598 	.word	0x3000f598

3000b3a4 <SWR_Calib_DCore>:
3000b3a4:	4b0f      	ldr	r3, [pc, #60]	; (3000b3e4 <SWR_Calib_DCore+0x40>)
3000b3a6:	f240 3231 	movw	r2, #817	; 0x331
3000b3aa:	490f      	ldr	r1, [pc, #60]	; (3000b3e8 <SWR_Calib_DCore+0x44>)
3000b3ac:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b3ae:	4001      	ands	r1, r0
3000b3b0:	430a      	orrs	r2, r1
3000b3b2:	651a      	str	r2, [r3, #80]	; 0x50
3000b3b4:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b3b6:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b3ba:	641a      	str	r2, [r3, #64]	; 0x40
3000b3bc:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b3c0:	f042 0201 	orr.w	r2, r2, #1
3000b3c4:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b3c8:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b3cc:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b3d0:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b3d4:	685a      	ldr	r2, [r3, #4]
3000b3d6:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b3da:	f042 0204 	orr.w	r2, r2, #4
3000b3de:	605a      	str	r2, [r3, #4]
3000b3e0:	4770      	bx	lr
3000b3e2:	bf00      	nop
3000b3e4:	42008100 	.word	0x42008100
3000b3e8:	fffffccc 	.word	0xfffffccc

3000b3ec <SWR_Calib_MEM>:
3000b3ec:	4b51      	ldr	r3, [pc, #324]	; (3000b534 <SWR_Calib_MEM+0x148>)
3000b3ee:	f240 3231 	movw	r2, #817	; 0x331
3000b3f2:	4951      	ldr	r1, [pc, #324]	; (3000b538 <SWR_Calib_MEM+0x14c>)
3000b3f4:	f04f 0c00 	mov.w	ip, #0
3000b3f8:	b510      	push	{r4, lr}
3000b3fa:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b3fe:	b082      	sub	sp, #8
3000b400:	4001      	ands	r1, r0
3000b402:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b406:	430a      	orrs	r2, r1
3000b408:	f10d 0107 	add.w	r1, sp, #7
3000b40c:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b410:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b414:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b418:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b41c:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b420:	f042 0201 	orr.w	r2, r2, #1
3000b424:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b428:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b42c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b430:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b434:	f88d c007 	strb.w	ip, [sp, #7]
3000b438:	f7fd fb4a 	bl	30008ad0 <OTP_Read8>
3000b43c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b440:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b444:	2af0      	cmp	r2, #240	; 0xf0
3000b446:	d156      	bne.n	3000b4f6 <SWR_Calib_MEM+0x10a>
3000b448:	f003 030f 	and.w	r3, r3, #15
3000b44c:	2b0f      	cmp	r3, #15
3000b44e:	d04e      	beq.n	3000b4ee <SWR_Calib_MEM+0x102>
3000b450:	2302      	movs	r3, #2
3000b452:	4c3a      	ldr	r4, [pc, #232]	; (3000b53c <SWR_Calib_MEM+0x150>)
3000b454:	f10d 0106 	add.w	r1, sp, #6
3000b458:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b45c:	7023      	strb	r3, [r4, #0]
3000b45e:	f7fd fb37 	bl	30008ad0 <OTP_Read8>
3000b462:	7822      	ldrb	r2, [r4, #0]
3000b464:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b468:	2a03      	cmp	r2, #3
3000b46a:	d05a      	beq.n	3000b522 <SWR_Calib_MEM+0x136>
3000b46c:	2a02      	cmp	r2, #2
3000b46e:	bf0c      	ite	eq
3000b470:	f003 030f 	andeq.w	r3, r3, #15
3000b474:	2300      	movne	r3, #0
3000b476:	492f      	ldr	r1, [pc, #188]	; (3000b534 <SWR_Calib_MEM+0x148>)
3000b478:	4831      	ldr	r0, [pc, #196]	; (3000b540 <SWR_Calib_MEM+0x154>)
3000b47a:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b47e:	f022 020f 	bic.w	r2, r2, #15
3000b482:	4313      	orrs	r3, r2
3000b484:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b488:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b48a:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b48e:	2b07      	cmp	r3, #7
3000b490:	d03a      	beq.n	3000b508 <SWR_Calib_MEM+0x11c>
3000b492:	2300      	movs	r3, #0
3000b494:	f10d 0107 	add.w	r1, sp, #7
3000b498:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b49c:	f88d 3007 	strb.w	r3, [sp, #7]
3000b4a0:	f7fd fb16 	bl	30008ad0 <OTP_Read8>
3000b4a4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b4a8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b4ac:	2af0      	cmp	r2, #240	; 0xf0
3000b4ae:	d124      	bne.n	3000b4fa <SWR_Calib_MEM+0x10e>
3000b4b0:	f003 030f 	and.w	r3, r3, #15
3000b4b4:	2b0f      	cmp	r3, #15
3000b4b6:	d022      	beq.n	3000b4fe <SWR_Calib_MEM+0x112>
3000b4b8:	2302      	movs	r3, #2
3000b4ba:	4c22      	ldr	r4, [pc, #136]	; (3000b544 <SWR_Calib_MEM+0x158>)
3000b4bc:	f10d 0107 	add.w	r1, sp, #7
3000b4c0:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b4c4:	7023      	strb	r3, [r4, #0]
3000b4c6:	f7fd fb03 	bl	30008ad0 <OTP_Read8>
3000b4ca:	7822      	ldrb	r2, [r4, #0]
3000b4cc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b4d0:	2a03      	cmp	r2, #3
3000b4d2:	d028      	beq.n	3000b526 <SWR_Calib_MEM+0x13a>
3000b4d4:	2a02      	cmp	r2, #2
3000b4d6:	d029      	beq.n	3000b52c <SWR_Calib_MEM+0x140>
3000b4d8:	2200      	movs	r2, #0
3000b4da:	4916      	ldr	r1, [pc, #88]	; (3000b534 <SWR_Calib_MEM+0x148>)
3000b4dc:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b4e0:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b4e4:	4313      	orrs	r3, r2
3000b4e6:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b4ea:	b002      	add	sp, #8
3000b4ec:	bd10      	pop	{r4, pc}
3000b4ee:	4b13      	ldr	r3, [pc, #76]	; (3000b53c <SWR_Calib_MEM+0x150>)
3000b4f0:	2201      	movs	r2, #1
3000b4f2:	701a      	strb	r2, [r3, #0]
3000b4f4:	e7cd      	b.n	3000b492 <SWR_Calib_MEM+0xa6>
3000b4f6:	2303      	movs	r3, #3
3000b4f8:	e7ab      	b.n	3000b452 <SWR_Calib_MEM+0x66>
3000b4fa:	2303      	movs	r3, #3
3000b4fc:	e7dd      	b.n	3000b4ba <SWR_Calib_MEM+0xce>
3000b4fe:	4b11      	ldr	r3, [pc, #68]	; (3000b544 <SWR_Calib_MEM+0x158>)
3000b500:	2201      	movs	r2, #1
3000b502:	701a      	strb	r2, [r3, #0]
3000b504:	b002      	add	sp, #8
3000b506:	bd10      	pop	{r4, pc}
3000b508:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b50a:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b50e:	d1c0      	bne.n	3000b492 <SWR_Calib_MEM+0xa6>
3000b510:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b514:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b518:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b51c:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b520:	e7b7      	b.n	3000b492 <SWR_Calib_MEM+0xa6>
3000b522:	091b      	lsrs	r3, r3, #4
3000b524:	e7a7      	b.n	3000b476 <SWR_Calib_MEM+0x8a>
3000b526:	091b      	lsrs	r3, r3, #4
3000b528:	059a      	lsls	r2, r3, #22
3000b52a:	e7d6      	b.n	3000b4da <SWR_Calib_MEM+0xee>
3000b52c:	f003 030f 	and.w	r3, r3, #15
3000b530:	059a      	lsls	r2, r3, #22
3000b532:	e7d2      	b.n	3000b4da <SWR_Calib_MEM+0xee>
3000b534:	42008800 	.word	0x42008800
3000b538:	fffffccc 	.word	0xfffffccc
3000b53c:	3000f733 	.word	0x3000f733
3000b540:	42008100 	.word	0x42008100
3000b544:	3000f732 	.word	0x3000f732

3000b548 <SWR_Calib_AUD>:
3000b548:	4b48      	ldr	r3, [pc, #288]	; (3000b66c <SWR_Calib_AUD+0x124>)
3000b54a:	f240 3231 	movw	r2, #817	; 0x331
3000b54e:	4948      	ldr	r1, [pc, #288]	; (3000b670 <SWR_Calib_AUD+0x128>)
3000b550:	f04f 0c00 	mov.w	ip, #0
3000b554:	b510      	push	{r4, lr}
3000b556:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000b55a:	b082      	sub	sp, #8
3000b55c:	4001      	ands	r1, r0
3000b55e:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b562:	430a      	orrs	r2, r1
3000b564:	f10d 0107 	add.w	r1, sp, #7
3000b568:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b56c:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b570:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b574:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b578:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000b57c:	f042 0201 	orr.w	r2, r2, #1
3000b580:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000b584:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000b588:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b58c:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000b590:	f88d c007 	strb.w	ip, [sp, #7]
3000b594:	f7fd fa9c 	bl	30008ad0 <OTP_Read8>
3000b598:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b59c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b5a0:	2af0      	cmp	r2, #240	; 0xf0
3000b5a2:	d150      	bne.n	3000b646 <SWR_Calib_AUD+0xfe>
3000b5a4:	f003 030f 	and.w	r3, r3, #15
3000b5a8:	2b0f      	cmp	r3, #15
3000b5aa:	d048      	beq.n	3000b63e <SWR_Calib_AUD+0xf6>
3000b5ac:	2302      	movs	r3, #2
3000b5ae:	4c31      	ldr	r4, [pc, #196]	; (3000b674 <SWR_Calib_AUD+0x12c>)
3000b5b0:	f10d 0106 	add.w	r1, sp, #6
3000b5b4:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b5b8:	7023      	strb	r3, [r4, #0]
3000b5ba:	f7fd fa89 	bl	30008ad0 <OTP_Read8>
3000b5be:	7822      	ldrb	r2, [r4, #0]
3000b5c0:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b5c4:	2a03      	cmp	r2, #3
3000b5c6:	d047      	beq.n	3000b658 <SWR_Calib_AUD+0x110>
3000b5c8:	2a02      	cmp	r2, #2
3000b5ca:	bf0c      	ite	eq
3000b5cc:	f003 030f 	andeq.w	r3, r3, #15
3000b5d0:	2300      	movne	r3, #0
3000b5d2:	4926      	ldr	r1, [pc, #152]	; (3000b66c <SWR_Calib_AUD+0x124>)
3000b5d4:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000b5d8:	f022 020f 	bic.w	r2, r2, #15
3000b5dc:	4313      	orrs	r3, r2
3000b5de:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b5e2:	2300      	movs	r3, #0
3000b5e4:	f10d 0107 	add.w	r1, sp, #7
3000b5e8:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b5ec:	f88d 3007 	strb.w	r3, [sp, #7]
3000b5f0:	f7fd fa6e 	bl	30008ad0 <OTP_Read8>
3000b5f4:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b5f8:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b5fc:	2af0      	cmp	r2, #240	; 0xf0
3000b5fe:	d124      	bne.n	3000b64a <SWR_Calib_AUD+0x102>
3000b600:	f003 030f 	and.w	r3, r3, #15
3000b604:	2b0f      	cmp	r3, #15
3000b606:	d022      	beq.n	3000b64e <SWR_Calib_AUD+0x106>
3000b608:	2302      	movs	r3, #2
3000b60a:	4c1b      	ldr	r4, [pc, #108]	; (3000b678 <SWR_Calib_AUD+0x130>)
3000b60c:	f10d 0107 	add.w	r1, sp, #7
3000b610:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b614:	7023      	strb	r3, [r4, #0]
3000b616:	f7fd fa5b 	bl	30008ad0 <OTP_Read8>
3000b61a:	7822      	ldrb	r2, [r4, #0]
3000b61c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b620:	2a03      	cmp	r2, #3
3000b622:	d01b      	beq.n	3000b65c <SWR_Calib_AUD+0x114>
3000b624:	2a02      	cmp	r2, #2
3000b626:	d01c      	beq.n	3000b662 <SWR_Calib_AUD+0x11a>
3000b628:	2200      	movs	r2, #0
3000b62a:	4910      	ldr	r1, [pc, #64]	; (3000b66c <SWR_Calib_AUD+0x124>)
3000b62c:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000b630:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b634:	4313      	orrs	r3, r2
3000b636:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b63a:	b002      	add	sp, #8
3000b63c:	bd10      	pop	{r4, pc}
3000b63e:	4b0d      	ldr	r3, [pc, #52]	; (3000b674 <SWR_Calib_AUD+0x12c>)
3000b640:	2201      	movs	r2, #1
3000b642:	701a      	strb	r2, [r3, #0]
3000b644:	e7cd      	b.n	3000b5e2 <SWR_Calib_AUD+0x9a>
3000b646:	2303      	movs	r3, #3
3000b648:	e7b1      	b.n	3000b5ae <SWR_Calib_AUD+0x66>
3000b64a:	2303      	movs	r3, #3
3000b64c:	e7dd      	b.n	3000b60a <SWR_Calib_AUD+0xc2>
3000b64e:	4b0a      	ldr	r3, [pc, #40]	; (3000b678 <SWR_Calib_AUD+0x130>)
3000b650:	2201      	movs	r2, #1
3000b652:	701a      	strb	r2, [r3, #0]
3000b654:	b002      	add	sp, #8
3000b656:	bd10      	pop	{r4, pc}
3000b658:	091b      	lsrs	r3, r3, #4
3000b65a:	e7ba      	b.n	3000b5d2 <SWR_Calib_AUD+0x8a>
3000b65c:	091b      	lsrs	r3, r3, #4
3000b65e:	059a      	lsls	r2, r3, #22
3000b660:	e7e3      	b.n	3000b62a <SWR_Calib_AUD+0xe2>
3000b662:	f003 030f 	and.w	r3, r3, #15
3000b666:	059a      	lsls	r2, r3, #22
3000b668:	e7df      	b.n	3000b62a <SWR_Calib_AUD+0xe2>
3000b66a:	bf00      	nop
3000b66c:	42008800 	.word	0x42008800
3000b670:	fffffccc 	.word	0xfffffccc
3000b674:	3000f731 	.word	0x3000f731
3000b678:	3000f730 	.word	0x3000f730

3000b67c <SWR_MEM>:
3000b67c:	2801      	cmp	r0, #1
3000b67e:	d00e      	beq.n	3000b69e <SWR_MEM+0x22>
3000b680:	4b31      	ldr	r3, [pc, #196]	; (3000b748 <SWR_MEM+0xcc>)
3000b682:	2000      	movs	r0, #0
3000b684:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b688:	f022 0201 	bic.w	r2, r2, #1
3000b68c:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b690:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b694:	f022 0202 	bic.w	r2, r2, #2
3000b698:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b69c:	4770      	bx	lr
3000b69e:	b510      	push	{r4, lr}
3000b6a0:	4c29      	ldr	r4, [pc, #164]	; (3000b748 <SWR_MEM+0xcc>)
3000b6a2:	4a2a      	ldr	r2, [pc, #168]	; (3000b74c <SWR_MEM+0xd0>)
3000b6a4:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b6a8:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b6ac:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b6b0:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b6b4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6b8:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b6bc:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6c0:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b6c4:	f023 0303 	bic.w	r3, r3, #3
3000b6c8:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b6cc:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6d0:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b6d4:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6d8:	4790      	blx	r2
3000b6da:	2803      	cmp	r0, #3
3000b6dc:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b6e0:	d026      	beq.n	3000b730 <SWR_MEM+0xb4>
3000b6e2:	f043 0303 	orr.w	r3, r3, #3
3000b6e6:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b6ea:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6ee:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b6f2:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6f6:	4c14      	ldr	r4, [pc, #80]	; (3000b748 <SWR_MEM+0xcc>)
3000b6f8:	4b15      	ldr	r3, [pc, #84]	; (3000b750 <SWR_MEM+0xd4>)
3000b6fa:	4798      	blx	r3
3000b6fc:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b700:	4a14      	ldr	r2, [pc, #80]	; (3000b754 <SWR_MEM+0xd8>)
3000b702:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b706:	f043 0302 	orr.w	r3, r3, #2
3000b70a:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b70e:	4790      	blx	r2
3000b710:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b714:	4b10      	ldr	r3, [pc, #64]	; (3000b758 <SWR_MEM+0xdc>)
3000b716:	f042 0201 	orr.w	r2, r2, #1
3000b71a:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b71e:	e001      	b.n	3000b724 <SWR_MEM+0xa8>
3000b720:	3b01      	subs	r3, #1
3000b722:	d00e      	beq.n	3000b742 <SWR_MEM+0xc6>
3000b724:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b728:	03d2      	lsls	r2, r2, #15
3000b72a:	d5f9      	bpl.n	3000b720 <SWR_MEM+0xa4>
3000b72c:	2000      	movs	r0, #0
3000b72e:	bd10      	pop	{r4, pc}
3000b730:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b734:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b738:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b73c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b740:	e7d9      	b.n	3000b6f6 <SWR_MEM+0x7a>
3000b742:	2001      	movs	r0, #1
3000b744:	bd10      	pop	{r4, pc}
3000b746:	bf00      	nop
3000b748:	42008800 	.word	0x42008800
3000b74c:	30007dd9 	.word	0x30007dd9
3000b750:	3000b3ed 	.word	0x3000b3ed
3000b754:	00009b2d 	.word	0x00009b2d
3000b758:	000186a0 	.word	0x000186a0

3000b75c <SWR_MEM_Manual>:
3000b75c:	4a06      	ldr	r2, [pc, #24]	; (3000b778 <SWR_MEM_Manual+0x1c>)
3000b75e:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b762:	b120      	cbz	r0, 3000b76e <SWR_MEM_Manual+0x12>
3000b764:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b768:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b76c:	4770      	bx	lr
3000b76e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b772:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b776:	4770      	bx	lr
3000b778:	42008800 	.word	0x42008800

3000b77c <SWR_MEM_Mode_Set>:
3000b77c:	2801      	cmp	r0, #1
3000b77e:	b538      	push	{r3, r4, r5, lr}
3000b780:	d807      	bhi.n	3000b792 <SWR_MEM_Mode_Set+0x16>
3000b782:	d10a      	bne.n	3000b79a <SWR_MEM_Mode_Set+0x1e>
3000b784:	4c22      	ldr	r4, [pc, #136]	; (3000b810 <SWR_MEM_Mode_Set+0x94>)
3000b786:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b78a:	035b      	lsls	r3, r3, #13
3000b78c:	d51b      	bpl.n	3000b7c6 <SWR_MEM_Mode_Set+0x4a>
3000b78e:	2000      	movs	r0, #0
3000b790:	bd38      	pop	{r3, r4, r5, pc}
3000b792:	217e      	movs	r1, #126	; 0x7e
3000b794:	481f      	ldr	r0, [pc, #124]	; (3000b814 <SWR_MEM_Mode_Set+0x98>)
3000b796:	f000 fecf 	bl	3000c538 <__io_assert_failed_veneer>
3000b79a:	491d      	ldr	r1, [pc, #116]	; (3000b810 <SWR_MEM_Mode_Set+0x94>)
3000b79c:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b7a0:	0358      	lsls	r0, r3, #13
3000b7a2:	d5f4      	bpl.n	3000b78e <SWR_MEM_Mode_Set+0x12>
3000b7a4:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b7a8:	f242 7310 	movw	r3, #10000	; 0x2710
3000b7ac:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b7b0:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b7b4:	e001      	b.n	3000b7ba <SWR_MEM_Mode_Set+0x3e>
3000b7b6:	3b01      	subs	r3, #1
3000b7b8:	d027      	beq.n	3000b80a <SWR_MEM_Mode_Set+0x8e>
3000b7ba:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b7be:	0352      	lsls	r2, r2, #13
3000b7c0:	d4f9      	bmi.n	3000b7b6 <SWR_MEM_Mode_Set+0x3a>
3000b7c2:	2000      	movs	r0, #0
3000b7c4:	e7e4      	b.n	3000b790 <SWR_MEM_Mode_Set+0x14>
3000b7c6:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b7ca:	2064      	movs	r0, #100	; 0x64
3000b7cc:	4d12      	ldr	r5, [pc, #72]	; (3000b818 <SWR_MEM_Mode_Set+0x9c>)
3000b7ce:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b7d2:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b7d6:	47a8      	blx	r5
3000b7d8:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b7dc:	2064      	movs	r0, #100	; 0x64
3000b7de:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b7e2:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b7e6:	47a8      	blx	r5
3000b7e8:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b7ec:	f242 7310 	movw	r3, #10000	; 0x2710
3000b7f0:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b7f4:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b7f8:	e001      	b.n	3000b7fe <SWR_MEM_Mode_Set+0x82>
3000b7fa:	3b01      	subs	r3, #1
3000b7fc:	d005      	beq.n	3000b80a <SWR_MEM_Mode_Set+0x8e>
3000b7fe:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b802:	0355      	lsls	r5, r2, #13
3000b804:	d5f9      	bpl.n	3000b7fa <SWR_MEM_Mode_Set+0x7e>
3000b806:	2000      	movs	r0, #0
3000b808:	e7c2      	b.n	3000b790 <SWR_MEM_Mode_Set+0x14>
3000b80a:	2001      	movs	r0, #1
3000b80c:	bd38      	pop	{r3, r4, r5, pc}
3000b80e:	bf00      	nop
3000b810:	42008800 	.word	0x42008800
3000b814:	3000d9a8 	.word	0x3000d9a8
3000b818:	00009b2d 	.word	0x00009b2d

3000b81c <SWR_AUDIO>:
3000b81c:	2801      	cmp	r0, #1
3000b81e:	d00e      	beq.n	3000b83e <SWR_AUDIO+0x22>
3000b820:	4b1e      	ldr	r3, [pc, #120]	; (3000b89c <SWR_AUDIO+0x80>)
3000b822:	2000      	movs	r0, #0
3000b824:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b828:	f022 0201 	bic.w	r2, r2, #1
3000b82c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b830:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b834:	f022 0202 	bic.w	r2, r2, #2
3000b838:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b83c:	4770      	bx	lr
3000b83e:	b510      	push	{r4, lr}
3000b840:	4c16      	ldr	r4, [pc, #88]	; (3000b89c <SWR_AUDIO+0x80>)
3000b842:	4b17      	ldr	r3, [pc, #92]	; (3000b8a0 <SWR_AUDIO+0x84>)
3000b844:	4798      	blx	r3
3000b846:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b84a:	4a16      	ldr	r2, [pc, #88]	; (3000b8a4 <SWR_AUDIO+0x88>)
3000b84c:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b850:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b854:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b858:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b85c:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b860:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b864:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b868:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b86c:	f043 0302 	orr.w	r3, r3, #2
3000b870:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b874:	4790      	blx	r2
3000b876:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b87a:	4b0b      	ldr	r3, [pc, #44]	; (3000b8a8 <SWR_AUDIO+0x8c>)
3000b87c:	f042 0201 	orr.w	r2, r2, #1
3000b880:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b884:	e001      	b.n	3000b88a <SWR_AUDIO+0x6e>
3000b886:	3b01      	subs	r3, #1
3000b888:	d005      	beq.n	3000b896 <SWR_AUDIO+0x7a>
3000b88a:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b88e:	03d2      	lsls	r2, r2, #15
3000b890:	d5f9      	bpl.n	3000b886 <SWR_AUDIO+0x6a>
3000b892:	2000      	movs	r0, #0
3000b894:	bd10      	pop	{r4, pc}
3000b896:	2001      	movs	r0, #1
3000b898:	bd10      	pop	{r4, pc}
3000b89a:	bf00      	nop
3000b89c:	42008800 	.word	0x42008800
3000b8a0:	3000b549 	.word	0x3000b549
3000b8a4:	00009b2d 	.word	0x00009b2d
3000b8a8:	000186a0 	.word	0x000186a0

3000b8ac <SWR_AUDIO_Manual>:
3000b8ac:	4a06      	ldr	r2, [pc, #24]	; (3000b8c8 <SWR_AUDIO_Manual+0x1c>)
3000b8ae:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b8b2:	b120      	cbz	r0, 3000b8be <SWR_AUDIO_Manual+0x12>
3000b8b4:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b8b8:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b8bc:	4770      	bx	lr
3000b8be:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b8c2:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b8c6:	4770      	bx	lr
3000b8c8:	42008800 	.word	0x42008800

3000b8cc <SWR_BST_MODE_Set>:
3000b8cc:	4a06      	ldr	r2, [pc, #24]	; (3000b8e8 <SWR_BST_MODE_Set+0x1c>)
3000b8ce:	6813      	ldr	r3, [r2, #0]
3000b8d0:	b118      	cbz	r0, 3000b8da <SWR_BST_MODE_Set+0xe>
3000b8d2:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b8d6:	6013      	str	r3, [r2, #0]
3000b8d8:	4770      	bx	lr
3000b8da:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b8de:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b8e2:	6013      	str	r3, [r2, #0]
3000b8e4:	4770      	bx	lr
3000b8e6:	bf00      	nop
3000b8e8:	42008100 	.word	0x42008100

3000b8ec <crash_dump_memory>:
3000b8ec:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b8f0:	460e      	mov	r6, r1
3000b8f2:	4680      	mov	r8, r0
3000b8f4:	4b21      	ldr	r3, [pc, #132]	; (3000b97c <crash_dump_memory+0x90>)
3000b8f6:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000b8fa:	4a21      	ldr	r2, [pc, #132]	; (3000b980 <crash_dump_memory+0x94>)
3000b8fc:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000b900:	4644      	mov	r4, r8
3000b902:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000b906:	4b1f      	ldr	r3, [pc, #124]	; (3000b984 <crash_dump_memory+0x98>)
3000b908:	481f      	ldr	r0, [pc, #124]	; (3000b988 <crash_dump_memory+0x9c>)
3000b90a:	bf88      	it	hi
3000b90c:	2680      	movhi	r6, #128	; 0x80
3000b90e:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000b912:	4f1e      	ldr	r7, [pc, #120]	; (3000b98c <crash_dump_memory+0xa0>)
3000b914:	bf14      	ite	ne
3000b916:	4611      	movne	r1, r2
3000b918:	4619      	moveq	r1, r3
3000b91a:	f000 fd8d 	bl	3000c438 <__DiagPrintf_veneer>
3000b91e:	4632      	mov	r2, r6
3000b920:	4641      	mov	r1, r8
3000b922:	481b      	ldr	r0, [pc, #108]	; (3000b990 <crash_dump_memory+0xa4>)
3000b924:	f000 fd88 	bl	3000c438 <__DiagPrintf_veneer>
3000b928:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b998 <crash_dump_memory+0xac>
3000b92c:	2300      	movs	r3, #0
3000b92e:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000b99c <crash_dump_memory+0xb0>
3000b932:	e007      	b.n	3000b944 <crash_dump_memory+0x58>
3000b934:	6821      	ldr	r1, [r4, #0]
3000b936:	4638      	mov	r0, r7
3000b938:	f000 fd7e 	bl	3000c438 <__DiagPrintf_veneer>
3000b93c:	42ae      	cmp	r6, r5
3000b93e:	d00f      	beq.n	3000b960 <crash_dump_memory+0x74>
3000b940:	3404      	adds	r4, #4
3000b942:	462b      	mov	r3, r5
3000b944:	1c5d      	adds	r5, r3, #1
3000b946:	b183      	cbz	r3, 3000b96a <crash_dump_memory+0x7e>
3000b948:	079b      	lsls	r3, r3, #30
3000b94a:	d1f3      	bne.n	3000b934 <crash_dump_memory+0x48>
3000b94c:	4621      	mov	r1, r4
3000b94e:	4648      	mov	r0, r9
3000b950:	f000 fd72 	bl	3000c438 <__DiagPrintf_veneer>
3000b954:	6821      	ldr	r1, [r4, #0]
3000b956:	4638      	mov	r0, r7
3000b958:	f000 fd6e 	bl	3000c438 <__DiagPrintf_veneer>
3000b95c:	42ae      	cmp	r6, r5
3000b95e:	d1ef      	bne.n	3000b940 <crash_dump_memory+0x54>
3000b960:	480c      	ldr	r0, [pc, #48]	; (3000b994 <crash_dump_memory+0xa8>)
3000b962:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b966:	f000 bd67 	b.w	3000c438 <__DiagPrintf_veneer>
3000b96a:	4641      	mov	r1, r8
3000b96c:	4650      	mov	r0, sl
3000b96e:	f000 fd63 	bl	3000c438 <__DiagPrintf_veneer>
3000b972:	6821      	ldr	r1, [r4, #0]
3000b974:	4638      	mov	r0, r7
3000b976:	f000 fd5f 	bl	3000c438 <__DiagPrintf_veneer>
3000b97a:	e7e1      	b.n	3000b940 <crash_dump_memory+0x54>
3000b97c:	42008000 	.word	0x42008000
3000b980:	3000d9bc 	.word	0x3000d9bc
3000b984:	3000d9c0 	.word	0x3000d9c0
3000b988:	3000d9c4 	.word	0x3000d9c4
3000b98c:	3000da2c 	.word	0x3000da2c
3000b990:	3000d9e8 	.word	0x3000d9e8
3000b994:	3000ce70 	.word	0x3000ce70
3000b998:	3000da34 	.word	0x3000da34
3000b99c:	3000da24 	.word	0x3000da24

3000b9a0 <crash_dump>:
3000b9a0:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b9a4:	4c4f      	ldr	r4, [pc, #316]	; (3000bae4 <crash_dump+0x144>)
3000b9a6:	4615      	mov	r5, r2
3000b9a8:	4688      	mov	r8, r1
3000b9aa:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000b9ae:	484e      	ldr	r0, [pc, #312]	; (3000bae8 <crash_dump+0x148>)
3000b9b0:	462f      	mov	r7, r5
3000b9b2:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000bb14 <crash_dump+0x174>
3000b9b6:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000b9ba:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000bb20 <crash_dump+0x180>
3000b9be:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000b9c2:	e9c4 2100 	strd	r2, r1, [r4]
3000b9c6:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000b9c8:	60a2      	str	r2, [r4, #8]
3000b9ca:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000b9cc:	60e2      	str	r2, [r4, #12]
3000b9ce:	686a      	ldr	r2, [r5, #4]
3000b9d0:	6122      	str	r2, [r4, #16]
3000b9d2:	68aa      	ldr	r2, [r5, #8]
3000b9d4:	6162      	str	r2, [r4, #20]
3000b9d6:	68ea      	ldr	r2, [r5, #12]
3000b9d8:	61a2      	str	r2, [r4, #24]
3000b9da:	692a      	ldr	r2, [r5, #16]
3000b9dc:	61e2      	str	r2, [r4, #28]
3000b9de:	696a      	ldr	r2, [r5, #20]
3000b9e0:	6222      	str	r2, [r4, #32]
3000b9e2:	69aa      	ldr	r2, [r5, #24]
3000b9e4:	6262      	str	r2, [r4, #36]	; 0x24
3000b9e6:	69ea      	ldr	r2, [r5, #28]
3000b9e8:	62a2      	str	r2, [r4, #40]	; 0x28
3000b9ea:	6a2a      	ldr	r2, [r5, #32]
3000b9ec:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b9ee:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b9f0:	6322      	str	r2, [r4, #48]	; 0x30
3000b9f2:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000b9f4:	6362      	str	r2, [r4, #52]	; 0x34
3000b9f6:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000b9f8:	63a2      	str	r2, [r4, #56]	; 0x38
3000b9fa:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000b9fc:	63e2      	str	r2, [r4, #60]	; 0x3c
3000b9fe:	682a      	ldr	r2, [r5, #0]
3000ba00:	6422      	str	r2, [r4, #64]	; 0x40
3000ba02:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000ba06:	6462      	str	r2, [r4, #68]	; 0x44
3000ba08:	4a38      	ldr	r2, [pc, #224]	; (3000baec <crash_dump+0x14c>)
3000ba0a:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000ba0e:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000ba12:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000ba16:	64e1      	str	r1, [r4, #76]	; 0x4c
3000ba18:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000ba1c:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000ba20:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000ba24:	6561      	str	r1, [r4, #84]	; 0x54
3000ba26:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000ba28:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000ba2c:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000ba30:	65e1      	str	r1, [r4, #92]	; 0x5c
3000ba32:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000ba34:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000ba36:	b289      	uxth	r1, r1
3000ba38:	b292      	uxth	r2, r2
3000ba3a:	6621      	str	r1, [r4, #96]	; 0x60
3000ba3c:	6662      	str	r2, [r4, #100]	; 0x64
3000ba3e:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000ba42:	66a2      	str	r2, [r4, #104]	; 0x68
3000ba44:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000ba48:	66e3      	str	r3, [r4, #108]	; 0x6c
3000ba4a:	f000 fcf5 	bl	3000c438 <__DiagPrintf_veneer>
3000ba4e:	4620      	mov	r0, r4
3000ba50:	462c      	mov	r4, r5
3000ba52:	f000 f867 	bl	3000bb24 <fault_diagnosis>
3000ba56:	4826      	ldr	r0, [pc, #152]	; (3000baf0 <crash_dump+0x150>)
3000ba58:	f000 fcee 	bl	3000c438 <__DiagPrintf_veneer>
3000ba5c:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000ba5e:	4825      	ldr	r0, [pc, #148]	; (3000baf4 <crash_dump+0x154>)
3000ba60:	f000 fcea 	bl	3000c438 <__DiagPrintf_veneer>
3000ba64:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000ba66:	4824      	ldr	r0, [pc, #144]	; (3000baf8 <crash_dump+0x158>)
3000ba68:	f000 fce6 	bl	3000c438 <__DiagPrintf_veneer>
3000ba6c:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000ba6e:	4823      	ldr	r0, [pc, #140]	; (3000bafc <crash_dump+0x15c>)
3000ba70:	f000 fce2 	bl	3000c438 <__DiagPrintf_veneer>
3000ba74:	f857 1b20 	ldr.w	r1, [r7], #32
3000ba78:	4821      	ldr	r0, [pc, #132]	; (3000bb00 <crash_dump+0x160>)
3000ba7a:	463e      	mov	r6, r7
3000ba7c:	f000 fcdc 	bl	3000c438 <__DiagPrintf_veneer>
3000ba80:	4920      	ldr	r1, [pc, #128]	; (3000bb04 <crash_dump+0x164>)
3000ba82:	4658      	mov	r0, fp
3000ba84:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ba88:	f000 fcd6 	bl	3000c438 <__DiagPrintf_veneer>
3000ba8c:	454e      	cmp	r6, r9
3000ba8e:	d008      	beq.n	3000baa2 <crash_dump+0x102>
3000ba90:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ba94:	4658      	mov	r0, fp
3000ba96:	f85a 1b04 	ldr.w	r1, [sl], #4
3000ba9a:	f000 fccd 	bl	3000c438 <__DiagPrintf_veneer>
3000ba9e:	454e      	cmp	r6, r9
3000baa0:	d1f6      	bne.n	3000ba90 <crash_dump+0xf0>
3000baa2:	4e19      	ldr	r6, [pc, #100]	; (3000bb08 <crash_dump+0x168>)
3000baa4:	4919      	ldr	r1, [pc, #100]	; (3000bb0c <crash_dump+0x16c>)
3000baa6:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bb14 <crash_dump+0x174>
3000baaa:	e001      	b.n	3000bab0 <crash_dump+0x110>
3000baac:	f856 1b04 	ldr.w	r1, [r6], #4
3000bab0:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000bab4:	4648      	mov	r0, r9
3000bab6:	f000 fcbf 	bl	3000c438 <__DiagPrintf_veneer>
3000baba:	42bc      	cmp	r4, r7
3000babc:	d1f6      	bne.n	3000baac <crash_dump+0x10c>
3000babe:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000bac0:	4913      	ldr	r1, [pc, #76]	; (3000bb10 <crash_dump+0x170>)
3000bac2:	4814      	ldr	r0, [pc, #80]	; (3000bb14 <crash_dump+0x174>)
3000bac4:	f000 fcb8 	bl	3000c438 <__DiagPrintf_veneer>
3000bac8:	2180      	movs	r1, #128	; 0x80
3000baca:	4640      	mov	r0, r8
3000bacc:	f7ff ff0e 	bl	3000b8ec <crash_dump_memory>
3000bad0:	4811      	ldr	r0, [pc, #68]	; (3000bb18 <crash_dump+0x178>)
3000bad2:	f000 fcb1 	bl	3000c438 <__DiagPrintf_veneer>
3000bad6:	4811      	ldr	r0, [pc, #68]	; (3000bb1c <crash_dump+0x17c>)
3000bad8:	f000 fcae 	bl	3000c438 <__DiagPrintf_veneer>
3000badc:	2000      	movs	r0, #0
3000bade:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000bae2:	bf00      	nop
3000bae4:	3000fd28 	.word	0x3000fd28
3000bae8:	3000da48 	.word	0x3000da48
3000baec:	e000ed00 	.word	0xe000ed00
3000baf0:	3000da6c 	.word	0x3000da6c
3000baf4:	3000da94 	.word	0x3000da94
3000baf8:	3000daa4 	.word	0x3000daa4
3000bafc:	3000dab4 	.word	0x3000dab4
3000bb00:	3000dac4 	.word	0x3000dac4
3000bb04:	3000da40 	.word	0x3000da40
3000bb08:	3000db24 	.word	0x3000db24
3000bb0c:	3000da44 	.word	0x3000da44
3000bb10:	3000dae4 	.word	0x3000dae4
3000bb14:	3000dad4 	.word	0x3000dad4
3000bb18:	3000dae8 	.word	0x3000dae8
3000bb1c:	3000db14 	.word	0x3000db14
3000bb20:	3000db44 	.word	0x3000db44

3000bb24 <fault_diagnosis>:
3000bb24:	b510      	push	{r4, lr}
3000bb26:	4604      	mov	r4, r0
3000bb28:	4888      	ldr	r0, [pc, #544]	; (3000bd4c <fault_diagnosis+0x228>)
3000bb2a:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000bb2c:	f000 fc84 	bl	3000c438 <__DiagPrintf_veneer>
3000bb30:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000bb34:	079b      	lsls	r3, r3, #30
3000bb36:	f100 808a 	bmi.w	3000bc4e <fault_diagnosis+0x12a>
3000bb3a:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bb3c:	b1e1      	cbz	r1, 3000bb78 <fault_diagnosis+0x54>
3000bb3e:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bb42:	07d8      	lsls	r0, r3, #31
3000bb44:	f100 8094 	bmi.w	3000bc70 <fault_diagnosis+0x14c>
3000bb48:	079a      	lsls	r2, r3, #30
3000bb4a:	f100 809a 	bmi.w	3000bc82 <fault_diagnosis+0x15e>
3000bb4e:	0758      	lsls	r0, r3, #29
3000bb50:	f100 80a1 	bmi.w	3000bc96 <fault_diagnosis+0x172>
3000bb54:	0719      	lsls	r1, r3, #28
3000bb56:	f100 80a8 	bmi.w	3000bcaa <fault_diagnosis+0x186>
3000bb5a:	06da      	lsls	r2, r3, #27
3000bb5c:	f100 80af 	bmi.w	3000bcbe <fault_diagnosis+0x19a>
3000bb60:	0698      	lsls	r0, r3, #26
3000bb62:	f100 80b6 	bmi.w	3000bcd2 <fault_diagnosis+0x1ae>
3000bb66:	0619      	lsls	r1, r3, #24
3000bb68:	f100 80bd 	bmi.w	3000bce6 <fault_diagnosis+0x1c2>
3000bb6c:	065a      	lsls	r2, r3, #25
3000bb6e:	d503      	bpl.n	3000bb78 <fault_diagnosis+0x54>
3000bb70:	f013 0f28 	tst.w	r3, #40	; 0x28
3000bb74:	f040 8186 	bne.w	3000be84 <fault_diagnosis+0x360>
3000bb78:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bb7c:	b199      	cbz	r1, 3000bba6 <fault_diagnosis+0x82>
3000bb7e:	07cb      	lsls	r3, r1, #31
3000bb80:	f100 80b9 	bmi.w	3000bcf6 <fault_diagnosis+0x1d2>
3000bb84:	0788      	lsls	r0, r1, #30
3000bb86:	f100 80bf 	bmi.w	3000bd08 <fault_diagnosis+0x1e4>
3000bb8a:	070a      	lsls	r2, r1, #28
3000bb8c:	f100 80c5 	bmi.w	3000bd1a <fault_diagnosis+0x1f6>
3000bb90:	06cb      	lsls	r3, r1, #27
3000bb92:	f100 80cb 	bmi.w	3000bd2c <fault_diagnosis+0x208>
3000bb96:	0688      	lsls	r0, r1, #26
3000bb98:	f100 80d1 	bmi.w	3000bd3e <fault_diagnosis+0x21a>
3000bb9c:	060a      	lsls	r2, r1, #24
3000bb9e:	d502      	bpl.n	3000bba6 <fault_diagnosis+0x82>
3000bba0:	078b      	lsls	r3, r1, #30
3000bba2:	f040 8194 	bne.w	3000bece <fault_diagnosis+0x3aa>
3000bba6:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bbaa:	b1b1      	cbz	r1, 3000bbda <fault_diagnosis+0xb6>
3000bbac:	07c8      	lsls	r0, r1, #31
3000bbae:	f100 80f7 	bmi.w	3000bda0 <fault_diagnosis+0x27c>
3000bbb2:	078a      	lsls	r2, r1, #30
3000bbb4:	f100 80fd 	bmi.w	3000bdb2 <fault_diagnosis+0x28e>
3000bbb8:	074b      	lsls	r3, r1, #29
3000bbba:	f100 8103 	bmi.w	3000bdc4 <fault_diagnosis+0x2a0>
3000bbbe:	0708      	lsls	r0, r1, #28
3000bbc0:	f100 8109 	bmi.w	3000bdd6 <fault_diagnosis+0x2b2>
3000bbc4:	06ca      	lsls	r2, r1, #27
3000bbc6:	f100 810f 	bmi.w	3000bde8 <fault_diagnosis+0x2c4>
3000bbca:	068b      	lsls	r3, r1, #26
3000bbcc:	f100 8115 	bmi.w	3000bdfa <fault_diagnosis+0x2d6>
3000bbd0:	0608      	lsls	r0, r1, #24
3000bbd2:	d502      	bpl.n	3000bbda <fault_diagnosis+0xb6>
3000bbd4:	0789      	lsls	r1, r1, #30
3000bbd6:	f100 8180 	bmi.w	3000beda <fault_diagnosis+0x3b6>
3000bbda:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bbde:	b1b9      	cbz	r1, 3000bc10 <fault_diagnosis+0xec>
3000bbe0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bbe4:	07da      	lsls	r2, r3, #31
3000bbe6:	f100 810f 	bmi.w	3000be08 <fault_diagnosis+0x2e4>
3000bbea:	0798      	lsls	r0, r3, #30
3000bbec:	f100 8115 	bmi.w	3000be1a <fault_diagnosis+0x2f6>
3000bbf0:	0759      	lsls	r1, r3, #29
3000bbf2:	f100 811d 	bmi.w	3000be30 <fault_diagnosis+0x30c>
3000bbf6:	071a      	lsls	r2, r3, #28
3000bbf8:	f100 8125 	bmi.w	3000be46 <fault_diagnosis+0x322>
3000bbfc:	06db      	lsls	r3, r3, #27
3000bbfe:	f100 812d 	bmi.w	3000be5c <fault_diagnosis+0x338>
3000bc02:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bc06:	07d8      	lsls	r0, r3, #31
3000bc08:	f100 8133 	bmi.w	3000be72 <fault_diagnosis+0x34e>
3000bc0c:	0799      	lsls	r1, r3, #30
3000bc0e:	d424      	bmi.n	3000bc5a <fault_diagnosis+0x136>
3000bc10:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bc14:	2b00      	cmp	r3, #0
3000bc16:	db00      	blt.n	3000bc1a <fault_diagnosis+0xf6>
3000bc18:	bd10      	pop	{r4, pc}
3000bc1a:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bc1c:	2900      	cmp	r1, #0
3000bc1e:	d0fb      	beq.n	3000bc18 <fault_diagnosis+0xf4>
3000bc20:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bc24:	07da      	lsls	r2, r3, #31
3000bc26:	f100 8143 	bmi.w	3000beb0 <fault_diagnosis+0x38c>
3000bc2a:	0798      	lsls	r0, r3, #30
3000bc2c:	f100 8138 	bmi.w	3000bea0 <fault_diagnosis+0x37c>
3000bc30:	0759      	lsls	r1, r3, #29
3000bc32:	f100 812d 	bmi.w	3000be90 <fault_diagnosis+0x36c>
3000bc36:	071a      	lsls	r2, r3, #28
3000bc38:	f100 8141 	bmi.w	3000bebe <fault_diagnosis+0x39a>
3000bc3c:	06db      	lsls	r3, r3, #27
3000bc3e:	d5eb      	bpl.n	3000bc18 <fault_diagnosis+0xf4>
3000bc40:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bc42:	4a43      	ldr	r2, [pc, #268]	; (3000bd50 <fault_diagnosis+0x22c>)
3000bc44:	4843      	ldr	r0, [pc, #268]	; (3000bd54 <fault_diagnosis+0x230>)
3000bc46:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000bc4a:	f000 bbf5 	b.w	3000c438 <__DiagPrintf_veneer>
3000bc4e:	4a42      	ldr	r2, [pc, #264]	; (3000bd58 <fault_diagnosis+0x234>)
3000bc50:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000bc52:	4842      	ldr	r0, [pc, #264]	; (3000bd5c <fault_diagnosis+0x238>)
3000bc54:	f000 fbf0 	bl	3000c438 <__DiagPrintf_veneer>
3000bc58:	e76f      	b.n	3000bb3a <fault_diagnosis+0x16>
3000bc5a:	4a41      	ldr	r2, [pc, #260]	; (3000bd60 <fault_diagnosis+0x23c>)
3000bc5c:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bc60:	4840      	ldr	r0, [pc, #256]	; (3000bd64 <fault_diagnosis+0x240>)
3000bc62:	f000 fbe9 	bl	3000c438 <__DiagPrintf_veneer>
3000bc66:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bc6a:	2b00      	cmp	r3, #0
3000bc6c:	dad4      	bge.n	3000bc18 <fault_diagnosis+0xf4>
3000bc6e:	e7d4      	b.n	3000bc1a <fault_diagnosis+0xf6>
3000bc70:	4a3d      	ldr	r2, [pc, #244]	; (3000bd68 <fault_diagnosis+0x244>)
3000bc72:	483e      	ldr	r0, [pc, #248]	; (3000bd6c <fault_diagnosis+0x248>)
3000bc74:	f000 fbe0 	bl	3000c438 <__DiagPrintf_veneer>
3000bc78:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc7c:	079a      	lsls	r2, r3, #30
3000bc7e:	f57f af66 	bpl.w	3000bb4e <fault_diagnosis+0x2a>
3000bc82:	4a3b      	ldr	r2, [pc, #236]	; (3000bd70 <fault_diagnosis+0x24c>)
3000bc84:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc86:	4839      	ldr	r0, [pc, #228]	; (3000bd6c <fault_diagnosis+0x248>)
3000bc88:	f000 fbd6 	bl	3000c438 <__DiagPrintf_veneer>
3000bc8c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc90:	0758      	lsls	r0, r3, #29
3000bc92:	f57f af5f 	bpl.w	3000bb54 <fault_diagnosis+0x30>
3000bc96:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc98:	4a36      	ldr	r2, [pc, #216]	; (3000bd74 <fault_diagnosis+0x250>)
3000bc9a:	4834      	ldr	r0, [pc, #208]	; (3000bd6c <fault_diagnosis+0x248>)
3000bc9c:	f000 fbcc 	bl	3000c438 <__DiagPrintf_veneer>
3000bca0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bca4:	0719      	lsls	r1, r3, #28
3000bca6:	f57f af58 	bpl.w	3000bb5a <fault_diagnosis+0x36>
3000bcaa:	4a33      	ldr	r2, [pc, #204]	; (3000bd78 <fault_diagnosis+0x254>)
3000bcac:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bcae:	482f      	ldr	r0, [pc, #188]	; (3000bd6c <fault_diagnosis+0x248>)
3000bcb0:	f000 fbc2 	bl	3000c438 <__DiagPrintf_veneer>
3000bcb4:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bcb8:	06da      	lsls	r2, r3, #27
3000bcba:	f57f af51 	bpl.w	3000bb60 <fault_diagnosis+0x3c>
3000bcbe:	4a2f      	ldr	r2, [pc, #188]	; (3000bd7c <fault_diagnosis+0x258>)
3000bcc0:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bcc2:	482a      	ldr	r0, [pc, #168]	; (3000bd6c <fault_diagnosis+0x248>)
3000bcc4:	f000 fbb8 	bl	3000c438 <__DiagPrintf_veneer>
3000bcc8:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bccc:	0698      	lsls	r0, r3, #26
3000bcce:	f57f af4a 	bpl.w	3000bb66 <fault_diagnosis+0x42>
3000bcd2:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bcd4:	4a2a      	ldr	r2, [pc, #168]	; (3000bd80 <fault_diagnosis+0x25c>)
3000bcd6:	4825      	ldr	r0, [pc, #148]	; (3000bd6c <fault_diagnosis+0x248>)
3000bcd8:	f000 fbae 	bl	3000c438 <__DiagPrintf_veneer>
3000bcdc:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bce0:	0619      	lsls	r1, r3, #24
3000bce2:	f57f af43 	bpl.w	3000bb6c <fault_diagnosis+0x48>
3000bce6:	4a27      	ldr	r2, [pc, #156]	; (3000bd84 <fault_diagnosis+0x260>)
3000bce8:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bcea:	4820      	ldr	r0, [pc, #128]	; (3000bd6c <fault_diagnosis+0x248>)
3000bcec:	f000 fba4 	bl	3000c438 <__DiagPrintf_veneer>
3000bcf0:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bcf4:	e73a      	b.n	3000bb6c <fault_diagnosis+0x48>
3000bcf6:	4a24      	ldr	r2, [pc, #144]	; (3000bd88 <fault_diagnosis+0x264>)
3000bcf8:	4824      	ldr	r0, [pc, #144]	; (3000bd8c <fault_diagnosis+0x268>)
3000bcfa:	f000 fb9d 	bl	3000c438 <__DiagPrintf_veneer>
3000bcfe:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd02:	0788      	lsls	r0, r1, #30
3000bd04:	f57f af41 	bpl.w	3000bb8a <fault_diagnosis+0x66>
3000bd08:	4a21      	ldr	r2, [pc, #132]	; (3000bd90 <fault_diagnosis+0x26c>)
3000bd0a:	4820      	ldr	r0, [pc, #128]	; (3000bd8c <fault_diagnosis+0x268>)
3000bd0c:	f000 fb94 	bl	3000c438 <__DiagPrintf_veneer>
3000bd10:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd14:	070a      	lsls	r2, r1, #28
3000bd16:	f57f af3b 	bpl.w	3000bb90 <fault_diagnosis+0x6c>
3000bd1a:	4a1e      	ldr	r2, [pc, #120]	; (3000bd94 <fault_diagnosis+0x270>)
3000bd1c:	481b      	ldr	r0, [pc, #108]	; (3000bd8c <fault_diagnosis+0x268>)
3000bd1e:	f000 fb8b 	bl	3000c438 <__DiagPrintf_veneer>
3000bd22:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd26:	06cb      	lsls	r3, r1, #27
3000bd28:	f57f af35 	bpl.w	3000bb96 <fault_diagnosis+0x72>
3000bd2c:	4a1a      	ldr	r2, [pc, #104]	; (3000bd98 <fault_diagnosis+0x274>)
3000bd2e:	4817      	ldr	r0, [pc, #92]	; (3000bd8c <fault_diagnosis+0x268>)
3000bd30:	f000 fb82 	bl	3000c438 <__DiagPrintf_veneer>
3000bd34:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd38:	0688      	lsls	r0, r1, #26
3000bd3a:	f57f af2f 	bpl.w	3000bb9c <fault_diagnosis+0x78>
3000bd3e:	4a17      	ldr	r2, [pc, #92]	; (3000bd9c <fault_diagnosis+0x278>)
3000bd40:	4812      	ldr	r0, [pc, #72]	; (3000bd8c <fault_diagnosis+0x268>)
3000bd42:	f000 fb79 	bl	3000c438 <__DiagPrintf_veneer>
3000bd46:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bd4a:	e727      	b.n	3000bb9c <fault_diagnosis+0x78>
3000bd4c:	3000db54 	.word	0x3000db54
3000bd50:	3000e3e4 	.word	0x3000e3e4
3000bd54:	3000e33c 	.word	0x3000e33c
3000bd58:	3000db6c 	.word	0x3000db6c
3000bd5c:	3000db98 	.word	0x3000db98
3000bd60:	3000e2a0 	.word	0x3000e2a0
3000bd64:	3000e0e4 	.word	0x3000e0e4
3000bd68:	3000dbb0 	.word	0x3000dbb0
3000bd6c:	3000dbe0 	.word	0x3000dbe0
3000bd70:	3000dbf8 	.word	0x3000dbf8
3000bd74:	3000dc30 	.word	0x3000dc30
3000bd78:	3000dc68 	.word	0x3000dc68
3000bd7c:	3000dca0 	.word	0x3000dca0
3000bd80:	3000dcd0 	.word	0x3000dcd0
3000bd84:	3000dd0c 	.word	0x3000dd0c
3000bd88:	3000dd7c 	.word	0x3000dd7c
3000bd8c:	3000ddc0 	.word	0x3000ddc0
3000bd90:	3000ddd8 	.word	0x3000ddd8
3000bd94:	3000de14 	.word	0x3000de14
3000bd98:	3000de4c 	.word	0x3000de4c
3000bd9c:	3000de80 	.word	0x3000de80
3000bda0:	4a51      	ldr	r2, [pc, #324]	; (3000bee8 <fault_diagnosis+0x3c4>)
3000bda2:	4852      	ldr	r0, [pc, #328]	; (3000beec <fault_diagnosis+0x3c8>)
3000bda4:	f000 fb48 	bl	3000c438 <__DiagPrintf_veneer>
3000bda8:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bdac:	078a      	lsls	r2, r1, #30
3000bdae:	f57f af03 	bpl.w	3000bbb8 <fault_diagnosis+0x94>
3000bdb2:	4a4f      	ldr	r2, [pc, #316]	; (3000bef0 <fault_diagnosis+0x3cc>)
3000bdb4:	484d      	ldr	r0, [pc, #308]	; (3000beec <fault_diagnosis+0x3c8>)
3000bdb6:	f000 fb3f 	bl	3000c438 <__DiagPrintf_veneer>
3000bdba:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bdbe:	074b      	lsls	r3, r1, #29
3000bdc0:	f57f aefd 	bpl.w	3000bbbe <fault_diagnosis+0x9a>
3000bdc4:	4a4b      	ldr	r2, [pc, #300]	; (3000bef4 <fault_diagnosis+0x3d0>)
3000bdc6:	4849      	ldr	r0, [pc, #292]	; (3000beec <fault_diagnosis+0x3c8>)
3000bdc8:	f000 fb36 	bl	3000c438 <__DiagPrintf_veneer>
3000bdcc:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bdd0:	0708      	lsls	r0, r1, #28
3000bdd2:	f57f aef7 	bpl.w	3000bbc4 <fault_diagnosis+0xa0>
3000bdd6:	4a48      	ldr	r2, [pc, #288]	; (3000bef8 <fault_diagnosis+0x3d4>)
3000bdd8:	4844      	ldr	r0, [pc, #272]	; (3000beec <fault_diagnosis+0x3c8>)
3000bdda:	f000 fb2d 	bl	3000c438 <__DiagPrintf_veneer>
3000bdde:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bde2:	06ca      	lsls	r2, r1, #27
3000bde4:	f57f aef1 	bpl.w	3000bbca <fault_diagnosis+0xa6>
3000bde8:	4a44      	ldr	r2, [pc, #272]	; (3000befc <fault_diagnosis+0x3d8>)
3000bdea:	4840      	ldr	r0, [pc, #256]	; (3000beec <fault_diagnosis+0x3c8>)
3000bdec:	f000 fb24 	bl	3000c438 <__DiagPrintf_veneer>
3000bdf0:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bdf4:	068b      	lsls	r3, r1, #26
3000bdf6:	f57f aeeb 	bpl.w	3000bbd0 <fault_diagnosis+0xac>
3000bdfa:	4a41      	ldr	r2, [pc, #260]	; (3000bf00 <fault_diagnosis+0x3dc>)
3000bdfc:	483b      	ldr	r0, [pc, #236]	; (3000beec <fault_diagnosis+0x3c8>)
3000bdfe:	f000 fb1b 	bl	3000c438 <__DiagPrintf_veneer>
3000be02:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000be06:	e6e3      	b.n	3000bbd0 <fault_diagnosis+0xac>
3000be08:	4a3e      	ldr	r2, [pc, #248]	; (3000bf04 <fault_diagnosis+0x3e0>)
3000be0a:	483f      	ldr	r0, [pc, #252]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be0c:	f000 fb14 	bl	3000c438 <__DiagPrintf_veneer>
3000be10:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000be14:	0798      	lsls	r0, r3, #30
3000be16:	f57f aeeb 	bpl.w	3000bbf0 <fault_diagnosis+0xcc>
3000be1a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be1e:	4a3b      	ldr	r2, [pc, #236]	; (3000bf0c <fault_diagnosis+0x3e8>)
3000be20:	4839      	ldr	r0, [pc, #228]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be22:	f000 fb09 	bl	3000c438 <__DiagPrintf_veneer>
3000be26:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000be2a:	0759      	lsls	r1, r3, #29
3000be2c:	f57f aee3 	bpl.w	3000bbf6 <fault_diagnosis+0xd2>
3000be30:	4a37      	ldr	r2, [pc, #220]	; (3000bf10 <fault_diagnosis+0x3ec>)
3000be32:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be36:	4834      	ldr	r0, [pc, #208]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be38:	f000 fafe 	bl	3000c438 <__DiagPrintf_veneer>
3000be3c:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000be40:	071a      	lsls	r2, r3, #28
3000be42:	f57f aedb 	bpl.w	3000bbfc <fault_diagnosis+0xd8>
3000be46:	4a33      	ldr	r2, [pc, #204]	; (3000bf14 <fault_diagnosis+0x3f0>)
3000be48:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be4c:	482e      	ldr	r0, [pc, #184]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be4e:	f000 faf3 	bl	3000c438 <__DiagPrintf_veneer>
3000be52:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000be56:	06db      	lsls	r3, r3, #27
3000be58:	f57f aed3 	bpl.w	3000bc02 <fault_diagnosis+0xde>
3000be5c:	4a2e      	ldr	r2, [pc, #184]	; (3000bf18 <fault_diagnosis+0x3f4>)
3000be5e:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be62:	4829      	ldr	r0, [pc, #164]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be64:	f000 fae8 	bl	3000c438 <__DiagPrintf_veneer>
3000be68:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000be6c:	07d8      	lsls	r0, r3, #31
3000be6e:	f57f aecd 	bpl.w	3000bc0c <fault_diagnosis+0xe8>
3000be72:	4a2a      	ldr	r2, [pc, #168]	; (3000bf1c <fault_diagnosis+0x3f8>)
3000be74:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be78:	4823      	ldr	r0, [pc, #140]	; (3000bf08 <fault_diagnosis+0x3e4>)
3000be7a:	f000 fadd 	bl	3000c438 <__DiagPrintf_veneer>
3000be7e:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000be82:	e6c3      	b.n	3000bc0c <fault_diagnosis+0xe8>
3000be84:	4a26      	ldr	r2, [pc, #152]	; (3000bf20 <fault_diagnosis+0x3fc>)
3000be86:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000be88:	4826      	ldr	r0, [pc, #152]	; (3000bf24 <fault_diagnosis+0x400>)
3000be8a:	f000 fad5 	bl	3000c438 <__DiagPrintf_veneer>
3000be8e:	e673      	b.n	3000bb78 <fault_diagnosis+0x54>
3000be90:	4a25      	ldr	r2, [pc, #148]	; (3000bf28 <fault_diagnosis+0x404>)
3000be92:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be94:	4825      	ldr	r0, [pc, #148]	; (3000bf2c <fault_diagnosis+0x408>)
3000be96:	f000 facf 	bl	3000c438 <__DiagPrintf_veneer>
3000be9a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be9e:	e6ca      	b.n	3000bc36 <fault_diagnosis+0x112>
3000bea0:	4a23      	ldr	r2, [pc, #140]	; (3000bf30 <fault_diagnosis+0x40c>)
3000bea2:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bea4:	4821      	ldr	r0, [pc, #132]	; (3000bf2c <fault_diagnosis+0x408>)
3000bea6:	f000 fac7 	bl	3000c438 <__DiagPrintf_veneer>
3000beaa:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000beae:	e6bf      	b.n	3000bc30 <fault_diagnosis+0x10c>
3000beb0:	4a20      	ldr	r2, [pc, #128]	; (3000bf34 <fault_diagnosis+0x410>)
3000beb2:	481e      	ldr	r0, [pc, #120]	; (3000bf2c <fault_diagnosis+0x408>)
3000beb4:	f000 fac0 	bl	3000c438 <__DiagPrintf_veneer>
3000beb8:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bebc:	e6b5      	b.n	3000bc2a <fault_diagnosis+0x106>
3000bebe:	4a1e      	ldr	r2, [pc, #120]	; (3000bf38 <fault_diagnosis+0x414>)
3000bec0:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bec2:	481a      	ldr	r0, [pc, #104]	; (3000bf2c <fault_diagnosis+0x408>)
3000bec4:	f000 fab8 	bl	3000c438 <__DiagPrintf_veneer>
3000bec8:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000becc:	e6b6      	b.n	3000bc3c <fault_diagnosis+0x118>
3000bece:	4a1b      	ldr	r2, [pc, #108]	; (3000bf3c <fault_diagnosis+0x418>)
3000bed0:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000bed2:	481b      	ldr	r0, [pc, #108]	; (3000bf40 <fault_diagnosis+0x41c>)
3000bed4:	f000 fab0 	bl	3000c438 <__DiagPrintf_veneer>
3000bed8:	e665      	b.n	3000bba6 <fault_diagnosis+0x82>
3000beda:	4a1a      	ldr	r2, [pc, #104]	; (3000bf44 <fault_diagnosis+0x420>)
3000bedc:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000bede:	481a      	ldr	r0, [pc, #104]	; (3000bf48 <fault_diagnosis+0x424>)
3000bee0:	f000 faaa 	bl	3000c438 <__DiagPrintf_veneer>
3000bee4:	e679      	b.n	3000bbda <fault_diagnosis+0xb6>
3000bee6:	bf00      	nop
3000bee8:	3000df14 	.word	0x3000df14
3000beec:	3000df48 	.word	0x3000df48
3000bef0:	3000df60 	.word	0x3000df60
3000bef4:	3000df98 	.word	0x3000df98
3000bef8:	3000dfd0 	.word	0x3000dfd0
3000befc:	3000dff8 	.word	0x3000dff8
3000bf00:	3000e020 	.word	0x3000e020
3000bf04:	3000e09c 	.word	0x3000e09c
3000bf08:	3000e0e4 	.word	0x3000e0e4
3000bf0c:	3000e0fc 	.word	0x3000e0fc
3000bf10:	3000e148 	.word	0x3000e148
3000bf14:	3000e1a8 	.word	0x3000e1a8
3000bf18:	3000e1f0 	.word	0x3000e1f0
3000bf1c:	3000e24c 	.word	0x3000e24c
3000bf20:	3000dd3c 	.word	0x3000dd3c
3000bf24:	3000dd64 	.word	0x3000dd64
3000bf28:	3000e388 	.word	0x3000e388
3000bf2c:	3000e33c 	.word	0x3000e33c
3000bf30:	3000e354 	.word	0x3000e354
3000bf34:	3000e30c 	.word	0x3000e30c
3000bf38:	3000e3b4 	.word	0x3000e3b4
3000bf3c:	3000decc 	.word	0x3000decc
3000bf40:	3000defc 	.word	0x3000defc
3000bf44:	3000e060 	.word	0x3000e060
3000bf48:	3000e084 	.word	0x3000e084

3000bf4c <rtk_log_write>:
3000bf4c:	b40c      	push	{r2, r3}
3000bf4e:	4b18      	ldr	r3, [pc, #96]	; (3000bfb0 <rtk_log_write+0x64>)
3000bf50:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000bf54:	681a      	ldr	r2, [r3, #0]
3000bf56:	b083      	sub	sp, #12
3000bf58:	4680      	mov	r8, r0
3000bf5a:	460e      	mov	r6, r1
3000bf5c:	2a02      	cmp	r2, #2
3000bf5e:	d825      	bhi.n	3000bfac <rtk_log_write+0x60>
3000bf60:	681f      	ldr	r7, [r3, #0]
3000bf62:	b1a7      	cbz	r7, 3000bf8e <rtk_log_write+0x42>
3000bf64:	4d13      	ldr	r5, [pc, #76]	; (3000bfb4 <rtk_log_write+0x68>)
3000bf66:	2400      	movs	r4, #0
3000bf68:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000bfc0 <rtk_log_write+0x74>
3000bf6c:	e002      	b.n	3000bf74 <rtk_log_write+0x28>
3000bf6e:	3401      	adds	r4, #1
3000bf70:	42a7      	cmp	r7, r4
3000bf72:	d90c      	bls.n	3000bf8e <rtk_log_write+0x42>
3000bf74:	4628      	mov	r0, r5
3000bf76:	4631      	mov	r1, r6
3000bf78:	350b      	adds	r5, #11
3000bf7a:	47c8      	blx	r9
3000bf7c:	2800      	cmp	r0, #0
3000bf7e:	d1f6      	bne.n	3000bf6e <rtk_log_write+0x22>
3000bf80:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000bf84:	4b0c      	ldr	r3, [pc, #48]	; (3000bfb8 <rtk_log_write+0x6c>)
3000bf86:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000bf8a:	5d1b      	ldrb	r3, [r3, r4]
3000bf8c:	e001      	b.n	3000bf92 <rtk_log_write+0x46>
3000bf8e:	4b0b      	ldr	r3, [pc, #44]	; (3000bfbc <rtk_log_write+0x70>)
3000bf90:	781b      	ldrb	r3, [r3, #0]
3000bf92:	4598      	cmp	r8, r3
3000bf94:	d805      	bhi.n	3000bfa2 <rtk_log_write+0x56>
3000bf96:	aa0b      	add	r2, sp, #44	; 0x2c
3000bf98:	990a      	ldr	r1, [sp, #40]	; 0x28
3000bf9a:	2000      	movs	r0, #0
3000bf9c:	9201      	str	r2, [sp, #4]
3000bf9e:	f000 fa6b 	bl	3000c478 <__DiagVSprintf_veneer>
3000bfa2:	b003      	add	sp, #12
3000bfa4:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000bfa8:	b002      	add	sp, #8
3000bfaa:	4770      	bx	lr
3000bfac:	2703      	movs	r7, #3
3000bfae:	e7d9      	b.n	3000bf64 <rtk_log_write+0x18>
3000bfb0:	30010168 	.word	0x30010168
3000bfb4:	3001016d 	.word	0x3001016d
3000bfb8:	3001016c 	.word	0x3001016c
3000bfbc:	3000f744 	.word	0x3000f744
3000bfc0:	00012c89 	.word	0x00012c89

3000bfc4 <BOOT_VerCheck>:
3000bfc4:	b570      	push	{r4, r5, r6, lr}
3000bfc6:	4b1d      	ldr	r3, [pc, #116]	; (3000c03c <BOOT_VerCheck+0x78>)
3000bfc8:	4798      	blx	r3
3000bfca:	2800      	cmp	r0, #0
3000bfcc:	d135      	bne.n	3000c03a <BOOT_VerCheck+0x76>
3000bfce:	4b1c      	ldr	r3, [pc, #112]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000bfd0:	2403      	movs	r4, #3
3000bfd2:	4d1c      	ldr	r5, [pc, #112]	; (3000c044 <BOOT_VerCheck+0x80>)
3000bfd4:	2003      	movs	r0, #3
3000bfd6:	4a1c      	ldr	r2, [pc, #112]	; (3000c048 <BOOT_VerCheck+0x84>)
3000bfd8:	4619      	mov	r1, r3
3000bfda:	f7ff ffb7 	bl	3000bf4c <rtk_log_write>
3000bfde:	4b18      	ldr	r3, [pc, #96]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000bfe0:	462a      	mov	r2, r5
3000bfe2:	2003      	movs	r0, #3
3000bfe4:	4619      	mov	r1, r3
3000bfe6:	f7ff ffb1 	bl	3000bf4c <rtk_log_write>
3000bfea:	4b15      	ldr	r3, [pc, #84]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000bfec:	4a17      	ldr	r2, [pc, #92]	; (3000c04c <BOOT_VerCheck+0x88>)
3000bfee:	2003      	movs	r0, #3
3000bff0:	4619      	mov	r1, r3
3000bff2:	f7ff ffab 	bl	3000bf4c <rtk_log_write>
3000bff6:	4b12      	ldr	r3, [pc, #72]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000bff8:	4a15      	ldr	r2, [pc, #84]	; (3000c050 <BOOT_VerCheck+0x8c>)
3000bffa:	2003      	movs	r0, #3
3000bffc:	4619      	mov	r1, r3
3000bffe:	f7ff ffa5 	bl	3000bf4c <rtk_log_write>
3000c002:	4b0f      	ldr	r3, [pc, #60]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000c004:	4a13      	ldr	r2, [pc, #76]	; (3000c054 <BOOT_VerCheck+0x90>)
3000c006:	2003      	movs	r0, #3
3000c008:	4619      	mov	r1, r3
3000c00a:	f7ff ff9f 	bl	3000bf4c <rtk_log_write>
3000c00e:	4b0c      	ldr	r3, [pc, #48]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000c010:	462a      	mov	r2, r5
3000c012:	2003      	movs	r0, #3
3000c014:	4619      	mov	r1, r3
3000c016:	f7ff ff99 	bl	3000bf4c <rtk_log_write>
3000c01a:	4b09      	ldr	r3, [pc, #36]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000c01c:	4a0e      	ldr	r2, [pc, #56]	; (3000c058 <BOOT_VerCheck+0x94>)
3000c01e:	2003      	movs	r0, #3
3000c020:	4619      	mov	r1, r3
3000c022:	f7ff ff93 	bl	3000bf4c <rtk_log_write>
3000c026:	3c01      	subs	r4, #1
3000c028:	d1d9      	bne.n	3000bfde <BOOT_VerCheck+0x1a>
3000c02a:	4b05      	ldr	r3, [pc, #20]	; (3000c040 <BOOT_VerCheck+0x7c>)
3000c02c:	2003      	movs	r0, #3
3000c02e:	4a0b      	ldr	r2, [pc, #44]	; (3000c05c <BOOT_VerCheck+0x98>)
3000c030:	4619      	mov	r1, r3
3000c032:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000c036:	f7ff bf89 	b.w	3000bf4c <rtk_log_write>
3000c03a:	bd70      	pop	{r4, r5, r6, pc}
3000c03c:	30009c5d 	.word	0x30009c5d
3000c040:	3000e4b6 	.word	0x3000e4b6
3000c044:	3000e4fe 	.word	0x3000e4fe
3000c048:	3000e4bb 	.word	0x3000e4bb
3000c04c:	3000e541 	.word	0x3000e541
3000c050:	3000e584 	.word	0x3000e584
3000c054:	3000e5c7 	.word	0x3000e5c7
3000c058:	3000e60a 	.word	0x3000e60a
3000c05c:	3000e64d 	.word	0x3000e64d

3000c060 <__aeabi_uldivmod>:
3000c060:	b953      	cbnz	r3, 3000c078 <__aeabi_uldivmod+0x18>
3000c062:	b94a      	cbnz	r2, 3000c078 <__aeabi_uldivmod+0x18>
3000c064:	2900      	cmp	r1, #0
3000c066:	bf08      	it	eq
3000c068:	2800      	cmpeq	r0, #0
3000c06a:	bf1c      	itt	ne
3000c06c:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000c070:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000c074:	f000 b960 	b.w	3000c338 <__aeabi_idiv0>
3000c078:	f1ad 0c08 	sub.w	ip, sp, #8
3000c07c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000c080:	f000 f806 	bl	3000c090 <__udivmoddi4>
3000c084:	f8dd e004 	ldr.w	lr, [sp, #4]
3000c088:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000c08c:	b004      	add	sp, #16
3000c08e:	4770      	bx	lr

3000c090 <__udivmoddi4>:
3000c090:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000c094:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000c096:	4688      	mov	r8, r1
3000c098:	468e      	mov	lr, r1
3000c09a:	4615      	mov	r5, r2
3000c09c:	4604      	mov	r4, r0
3000c09e:	4619      	mov	r1, r3
3000c0a0:	2b00      	cmp	r3, #0
3000c0a2:	f040 80c6 	bne.w	3000c232 <__udivmoddi4+0x1a2>
3000c0a6:	4542      	cmp	r2, r8
3000c0a8:	fab2 f782 	clz	r7, r2
3000c0ac:	d946      	bls.n	3000c13c <__udivmoddi4+0xac>
3000c0ae:	b14f      	cbz	r7, 3000c0c4 <__udivmoddi4+0x34>
3000c0b0:	f1c7 0c20 	rsb	ip, r7, #32
3000c0b4:	fa08 f307 	lsl.w	r3, r8, r7
3000c0b8:	40bd      	lsls	r5, r7
3000c0ba:	40bc      	lsls	r4, r7
3000c0bc:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c0c0:	ea4c 0e03 	orr.w	lr, ip, r3
3000c0c4:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000c0c8:	fa1f fc85 	uxth.w	ip, r5
3000c0cc:	0c22      	lsrs	r2, r4, #16
3000c0ce:	fbbe f9f8 	udiv	r9, lr, r8
3000c0d2:	fb08 e319 	mls	r3, r8, r9, lr
3000c0d6:	fb09 fa0c 	mul.w	sl, r9, ip
3000c0da:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000c0de:	459a      	cmp	sl, r3
3000c0e0:	d928      	bls.n	3000c134 <__udivmoddi4+0xa4>
3000c0e2:	18eb      	adds	r3, r5, r3
3000c0e4:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000c0e8:	d204      	bcs.n	3000c0f4 <__udivmoddi4+0x64>
3000c0ea:	459a      	cmp	sl, r3
3000c0ec:	d902      	bls.n	3000c0f4 <__udivmoddi4+0x64>
3000c0ee:	f1a9 0002 	sub.w	r0, r9, #2
3000c0f2:	442b      	add	r3, r5
3000c0f4:	eba3 030a 	sub.w	r3, r3, sl
3000c0f8:	b2a4      	uxth	r4, r4
3000c0fa:	fbb3 f2f8 	udiv	r2, r3, r8
3000c0fe:	fb08 3312 	mls	r3, r8, r2, r3
3000c102:	fb02 fc0c 	mul.w	ip, r2, ip
3000c106:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c10a:	45a4      	cmp	ip, r4
3000c10c:	d914      	bls.n	3000c138 <__udivmoddi4+0xa8>
3000c10e:	192c      	adds	r4, r5, r4
3000c110:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000c114:	d203      	bcs.n	3000c11e <__udivmoddi4+0x8e>
3000c116:	45a4      	cmp	ip, r4
3000c118:	d901      	bls.n	3000c11e <__udivmoddi4+0x8e>
3000c11a:	1e93      	subs	r3, r2, #2
3000c11c:	442c      	add	r4, r5
3000c11e:	eba4 040c 	sub.w	r4, r4, ip
3000c122:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c126:	b11e      	cbz	r6, 3000c130 <__udivmoddi4+0xa0>
3000c128:	40fc      	lsrs	r4, r7
3000c12a:	2300      	movs	r3, #0
3000c12c:	6034      	str	r4, [r6, #0]
3000c12e:	6073      	str	r3, [r6, #4]
3000c130:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c134:	4648      	mov	r0, r9
3000c136:	e7dd      	b.n	3000c0f4 <__udivmoddi4+0x64>
3000c138:	4613      	mov	r3, r2
3000c13a:	e7f0      	b.n	3000c11e <__udivmoddi4+0x8e>
3000c13c:	b902      	cbnz	r2, 3000c140 <__udivmoddi4+0xb0>
3000c13e:	deff      	udf	#255	; 0xff
3000c140:	bb8f      	cbnz	r7, 3000c1a6 <__udivmoddi4+0x116>
3000c142:	eba8 0302 	sub.w	r3, r8, r2
3000c146:	2101      	movs	r1, #1
3000c148:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c14c:	b2aa      	uxth	r2, r5
3000c14e:	0c20      	lsrs	r0, r4, #16
3000c150:	fbb3 fcfe 	udiv	ip, r3, lr
3000c154:	fb0e 331c 	mls	r3, lr, ip, r3
3000c158:	fb0c f802 	mul.w	r8, ip, r2
3000c15c:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c160:	4598      	cmp	r8, r3
3000c162:	d962      	bls.n	3000c22a <__udivmoddi4+0x19a>
3000c164:	18eb      	adds	r3, r5, r3
3000c166:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000c16a:	d204      	bcs.n	3000c176 <__udivmoddi4+0xe6>
3000c16c:	4598      	cmp	r8, r3
3000c16e:	d902      	bls.n	3000c176 <__udivmoddi4+0xe6>
3000c170:	f1ac 0002 	sub.w	r0, ip, #2
3000c174:	442b      	add	r3, r5
3000c176:	eba3 0308 	sub.w	r3, r3, r8
3000c17a:	b2a4      	uxth	r4, r4
3000c17c:	fbb3 fcfe 	udiv	ip, r3, lr
3000c180:	fb0e 331c 	mls	r3, lr, ip, r3
3000c184:	fb0c f202 	mul.w	r2, ip, r2
3000c188:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c18c:	42a2      	cmp	r2, r4
3000c18e:	d94e      	bls.n	3000c22e <__udivmoddi4+0x19e>
3000c190:	192c      	adds	r4, r5, r4
3000c192:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000c196:	d204      	bcs.n	3000c1a2 <__udivmoddi4+0x112>
3000c198:	42a2      	cmp	r2, r4
3000c19a:	d902      	bls.n	3000c1a2 <__udivmoddi4+0x112>
3000c19c:	f1ac 0302 	sub.w	r3, ip, #2
3000c1a0:	442c      	add	r4, r5
3000c1a2:	1aa4      	subs	r4, r4, r2
3000c1a4:	e7bd      	b.n	3000c122 <__udivmoddi4+0x92>
3000c1a6:	f1c7 0c20 	rsb	ip, r7, #32
3000c1aa:	40bd      	lsls	r5, r7
3000c1ac:	fa08 f307 	lsl.w	r3, r8, r7
3000c1b0:	40bc      	lsls	r4, r7
3000c1b2:	fa28 f90c 	lsr.w	r9, r8, ip
3000c1b6:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c1ba:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c1be:	ea4c 0203 	orr.w	r2, ip, r3
3000c1c2:	b2ab      	uxth	r3, r5
3000c1c4:	fbb9 fcfe 	udiv	ip, r9, lr
3000c1c8:	0c11      	lsrs	r1, r2, #16
3000c1ca:	fb0e 901c 	mls	r0, lr, ip, r9
3000c1ce:	fb0c f803 	mul.w	r8, ip, r3
3000c1d2:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c1d6:	4580      	cmp	r8, r0
3000c1d8:	d923      	bls.n	3000c222 <__udivmoddi4+0x192>
3000c1da:	1828      	adds	r0, r5, r0
3000c1dc:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000c1e0:	d204      	bcs.n	3000c1ec <__udivmoddi4+0x15c>
3000c1e2:	4580      	cmp	r8, r0
3000c1e4:	d902      	bls.n	3000c1ec <__udivmoddi4+0x15c>
3000c1e6:	f1ac 0102 	sub.w	r1, ip, #2
3000c1ea:	4428      	add	r0, r5
3000c1ec:	eba0 0008 	sub.w	r0, r0, r8
3000c1f0:	b292      	uxth	r2, r2
3000c1f2:	fbb0 fcfe 	udiv	ip, r0, lr
3000c1f6:	fb0e 001c 	mls	r0, lr, ip, r0
3000c1fa:	fb0c f803 	mul.w	r8, ip, r3
3000c1fe:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c202:	4598      	cmp	r8, r3
3000c204:	d90f      	bls.n	3000c226 <__udivmoddi4+0x196>
3000c206:	18eb      	adds	r3, r5, r3
3000c208:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000c20c:	d204      	bcs.n	3000c218 <__udivmoddi4+0x188>
3000c20e:	4598      	cmp	r8, r3
3000c210:	d902      	bls.n	3000c218 <__udivmoddi4+0x188>
3000c212:	f1ac 0202 	sub.w	r2, ip, #2
3000c216:	442b      	add	r3, r5
3000c218:	eba3 0308 	sub.w	r3, r3, r8
3000c21c:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c220:	e792      	b.n	3000c148 <__udivmoddi4+0xb8>
3000c222:	4661      	mov	r1, ip
3000c224:	e7e2      	b.n	3000c1ec <__udivmoddi4+0x15c>
3000c226:	4662      	mov	r2, ip
3000c228:	e7f6      	b.n	3000c218 <__udivmoddi4+0x188>
3000c22a:	4660      	mov	r0, ip
3000c22c:	e7a3      	b.n	3000c176 <__udivmoddi4+0xe6>
3000c22e:	4663      	mov	r3, ip
3000c230:	e7b7      	b.n	3000c1a2 <__udivmoddi4+0x112>
3000c232:	4543      	cmp	r3, r8
3000c234:	d905      	bls.n	3000c242 <__udivmoddi4+0x1b2>
3000c236:	b10e      	cbz	r6, 3000c23c <__udivmoddi4+0x1ac>
3000c238:	e9c6 0800 	strd	r0, r8, [r6]
3000c23c:	2100      	movs	r1, #0
3000c23e:	4608      	mov	r0, r1
3000c240:	e776      	b.n	3000c130 <__udivmoddi4+0xa0>
3000c242:	fab3 fc83 	clz	ip, r3
3000c246:	f1bc 0f00 	cmp.w	ip, #0
3000c24a:	d110      	bne.n	3000c26e <__udivmoddi4+0x1de>
3000c24c:	4543      	cmp	r3, r8
3000c24e:	d301      	bcc.n	3000c254 <__udivmoddi4+0x1c4>
3000c250:	4282      	cmp	r2, r0
3000c252:	d80a      	bhi.n	3000c26a <__udivmoddi4+0x1da>
3000c254:	1a84      	subs	r4, r0, r2
3000c256:	eb68 0303 	sbc.w	r3, r8, r3
3000c25a:	2001      	movs	r0, #1
3000c25c:	469e      	mov	lr, r3
3000c25e:	2e00      	cmp	r6, #0
3000c260:	d068      	beq.n	3000c334 <__udivmoddi4+0x2a4>
3000c262:	e9c6 4e00 	strd	r4, lr, [r6]
3000c266:	2100      	movs	r1, #0
3000c268:	e762      	b.n	3000c130 <__udivmoddi4+0xa0>
3000c26a:	4660      	mov	r0, ip
3000c26c:	e7f7      	b.n	3000c25e <__udivmoddi4+0x1ce>
3000c26e:	f1cc 0e20 	rsb	lr, ip, #32
3000c272:	fa03 f30c 	lsl.w	r3, r3, ip
3000c276:	fa02 f50c 	lsl.w	r5, r2, ip
3000c27a:	fa00 f70c 	lsl.w	r7, r0, ip
3000c27e:	fa22 f40e 	lsr.w	r4, r2, lr
3000c282:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c286:	fa20 f10e 	lsr.w	r1, r0, lr
3000c28a:	431c      	orrs	r4, r3
3000c28c:	fa08 f30c 	lsl.w	r3, r8, ip
3000c290:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c294:	4319      	orrs	r1, r3
3000c296:	b2a3      	uxth	r3, r4
3000c298:	0c0a      	lsrs	r2, r1, #16
3000c29a:	fbbb f8f9 	udiv	r8, fp, r9
3000c29e:	fb09 bb18 	mls	fp, r9, r8, fp
3000c2a2:	fb08 fa03 	mul.w	sl, r8, r3
3000c2a6:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c2aa:	4592      	cmp	sl, r2
3000c2ac:	d93e      	bls.n	3000c32c <__udivmoddi4+0x29c>
3000c2ae:	18a2      	adds	r2, r4, r2
3000c2b0:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000c2b4:	d204      	bcs.n	3000c2c0 <__udivmoddi4+0x230>
3000c2b6:	4592      	cmp	sl, r2
3000c2b8:	d902      	bls.n	3000c2c0 <__udivmoddi4+0x230>
3000c2ba:	f1a8 0002 	sub.w	r0, r8, #2
3000c2be:	4422      	add	r2, r4
3000c2c0:	eba2 020a 	sub.w	r2, r2, sl
3000c2c4:	b289      	uxth	r1, r1
3000c2c6:	fbb2 f8f9 	udiv	r8, r2, r9
3000c2ca:	fb09 2218 	mls	r2, r9, r8, r2
3000c2ce:	fb08 f303 	mul.w	r3, r8, r3
3000c2d2:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c2d6:	4293      	cmp	r3, r2
3000c2d8:	d92a      	bls.n	3000c330 <__udivmoddi4+0x2a0>
3000c2da:	18a2      	adds	r2, r4, r2
3000c2dc:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000c2e0:	d204      	bcs.n	3000c2ec <__udivmoddi4+0x25c>
3000c2e2:	4293      	cmp	r3, r2
3000c2e4:	d902      	bls.n	3000c2ec <__udivmoddi4+0x25c>
3000c2e6:	f1a8 0102 	sub.w	r1, r8, #2
3000c2ea:	4422      	add	r2, r4
3000c2ec:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c2f0:	1ad2      	subs	r2, r2, r3
3000c2f2:	fba0 9805 	umull	r9, r8, r0, r5
3000c2f6:	4542      	cmp	r2, r8
3000c2f8:	464b      	mov	r3, r9
3000c2fa:	4641      	mov	r1, r8
3000c2fc:	d302      	bcc.n	3000c304 <__udivmoddi4+0x274>
3000c2fe:	d106      	bne.n	3000c30e <__udivmoddi4+0x27e>
3000c300:	454f      	cmp	r7, r9
3000c302:	d204      	bcs.n	3000c30e <__udivmoddi4+0x27e>
3000c304:	3801      	subs	r0, #1
3000c306:	ebb9 0305 	subs.w	r3, r9, r5
3000c30a:	eb68 0104 	sbc.w	r1, r8, r4
3000c30e:	b18e      	cbz	r6, 3000c334 <__udivmoddi4+0x2a4>
3000c310:	1afc      	subs	r4, r7, r3
3000c312:	eb62 0701 	sbc.w	r7, r2, r1
3000c316:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c31a:	fa24 f30c 	lsr.w	r3, r4, ip
3000c31e:	fa27 f70c 	lsr.w	r7, r7, ip
3000c322:	ea4e 0303 	orr.w	r3, lr, r3
3000c326:	e9c6 3700 	strd	r3, r7, [r6]
3000c32a:	e79c      	b.n	3000c266 <__udivmoddi4+0x1d6>
3000c32c:	4640      	mov	r0, r8
3000c32e:	e7c7      	b.n	3000c2c0 <__udivmoddi4+0x230>
3000c330:	4641      	mov	r1, r8
3000c332:	e7db      	b.n	3000c2ec <__udivmoddi4+0x25c>
3000c334:	4631      	mov	r1, r6
3000c336:	e6fb      	b.n	3000c130 <__udivmoddi4+0xa0>

3000c338 <__aeabi_idiv0>:
3000c338:	4770      	bx	lr
3000c33a:	bf00      	nop

3000c33c <_mbtowc_r>:
3000c33c:	b430      	push	{r4, r5}
3000c33e:	4c06      	ldr	r4, [pc, #24]	; (3000c358 <_mbtowc_r+0x1c>)
3000c340:	4d06      	ldr	r5, [pc, #24]	; (3000c35c <_mbtowc_r+0x20>)
3000c342:	6824      	ldr	r4, [r4, #0]
3000c344:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c346:	2c00      	cmp	r4, #0
3000c348:	bf08      	it	eq
3000c34a:	462c      	moveq	r4, r5
3000c34c:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c350:	46a4      	mov	ip, r4
3000c352:	bc30      	pop	{r4, r5}
3000c354:	4760      	bx	ip
3000c356:	bf00      	nop
3000c358:	3000f8b8 	.word	0x3000f8b8
3000c35c:	3000f748 	.word	0x3000f748

3000c360 <__ascii_mbtowc>:
3000c360:	b082      	sub	sp, #8
3000c362:	b901      	cbnz	r1, 3000c366 <__ascii_mbtowc+0x6>
3000c364:	a901      	add	r1, sp, #4
3000c366:	b142      	cbz	r2, 3000c37a <__ascii_mbtowc+0x1a>
3000c368:	b14b      	cbz	r3, 3000c37e <__ascii_mbtowc+0x1e>
3000c36a:	7813      	ldrb	r3, [r2, #0]
3000c36c:	600b      	str	r3, [r1, #0]
3000c36e:	7812      	ldrb	r2, [r2, #0]
3000c370:	1e10      	subs	r0, r2, #0
3000c372:	bf18      	it	ne
3000c374:	2001      	movne	r0, #1
3000c376:	b002      	add	sp, #8
3000c378:	4770      	bx	lr
3000c37a:	4610      	mov	r0, r2
3000c37c:	e7fb      	b.n	3000c376 <__ascii_mbtowc+0x16>
3000c37e:	f06f 0001 	mvn.w	r0, #1
3000c382:	e7f8      	b.n	3000c376 <__ascii_mbtowc+0x16>

3000c384 <_wctomb_r>:
3000c384:	b430      	push	{r4, r5}
3000c386:	4c06      	ldr	r4, [pc, #24]	; (3000c3a0 <_wctomb_r+0x1c>)
3000c388:	4d06      	ldr	r5, [pc, #24]	; (3000c3a4 <_wctomb_r+0x20>)
3000c38a:	6824      	ldr	r4, [r4, #0]
3000c38c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c38e:	2c00      	cmp	r4, #0
3000c390:	bf08      	it	eq
3000c392:	462c      	moveq	r4, r5
3000c394:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c398:	46a4      	mov	ip, r4
3000c39a:	bc30      	pop	{r4, r5}
3000c39c:	4760      	bx	ip
3000c39e:	bf00      	nop
3000c3a0:	3000f8b8 	.word	0x3000f8b8
3000c3a4:	3000f748 	.word	0x3000f748

3000c3a8 <__ascii_wctomb>:
3000c3a8:	b149      	cbz	r1, 3000c3be <__ascii_wctomb+0x16>
3000c3aa:	2aff      	cmp	r2, #255	; 0xff
3000c3ac:	bf8d      	iteet	hi
3000c3ae:	238a      	movhi	r3, #138	; 0x8a
3000c3b0:	2001      	movls	r0, #1
3000c3b2:	700a      	strbls	r2, [r1, #0]
3000c3b4:	6003      	strhi	r3, [r0, #0]
3000c3b6:	bf88      	it	hi
3000c3b8:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000c3bc:	4770      	bx	lr
3000c3be:	4608      	mov	r0, r1
3000c3c0:	4770      	bx	lr
3000c3c2:	bf00      	nop
3000c3c4:	0000      	movs	r0, r0
	...

3000c3c8 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000c3c8:	b401      	push	{r0}
3000c3ca:	4802      	ldr	r0, [pc, #8]	; (3000c3d4 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000c3cc:	4684      	mov	ip, r0
3000c3ce:	bc01      	pop	{r0}
3000c3d0:	4760      	bx	ip
3000c3d2:	bf00      	nop
3000c3d4:	000024dd 	.word	0x000024dd

3000c3d8 <____wrap_memset_veneer>:
3000c3d8:	b401      	push	{r0}
3000c3da:	4802      	ldr	r0, [pc, #8]	; (3000c3e4 <____wrap_memset_veneer+0xc>)
3000c3dc:	4684      	mov	ip, r0
3000c3de:	bc01      	pop	{r0}
3000c3e0:	4760      	bx	ip
3000c3e2:	bf00      	nop
3000c3e4:	00012efd 	.word	0x00012efd

3000c3e8 <__INT_HardFault_C_veneer>:
3000c3e8:	b401      	push	{r0}
3000c3ea:	4802      	ldr	r0, [pc, #8]	; (3000c3f4 <__INT_HardFault_C_veneer+0xc>)
3000c3ec:	4684      	mov	ip, r0
3000c3ee:	bc01      	pop	{r0}
3000c3f0:	4760      	bx	ip
3000c3f2:	bf00      	nop
3000c3f4:	0000d8ed 	.word	0x0000d8ed

3000c3f8 <____wrap_memcpy_veneer>:
3000c3f8:	b401      	push	{r0}
3000c3fa:	4802      	ldr	r0, [pc, #8]	; (3000c404 <____wrap_memcpy_veneer+0xc>)
3000c3fc:	4684      	mov	ip, r0
3000c3fe:	bc01      	pop	{r0}
3000c400:	4760      	bx	ip
3000c402:	bf00      	nop
3000c404:	00012f05 	.word	0x00012f05

3000c408 <__CRYPTO_SHA_Init_B_veneer>:
3000c408:	b401      	push	{r0}
3000c40a:	4802      	ldr	r0, [pc, #8]	; (3000c414 <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000c40c:	4684      	mov	ip, r0
3000c40e:	bc01      	pop	{r0}
3000c410:	4760      	bx	ip
3000c412:	bf00      	nop
3000c414:	00002451 	.word	0x00002451

3000c418 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000c418:	b401      	push	{r0}
3000c41a:	4802      	ldr	r0, [pc, #8]	; (3000c424 <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000c41c:	4684      	mov	ip, r0
3000c41e:	bc01      	pop	{r0}
3000c420:	4760      	bx	ip
3000c422:	bf00      	nop
3000c424:	00002369 	.word	0x00002369

3000c428 <__rtl_crypto_sha2_final_B_veneer>:
3000c428:	b401      	push	{r0}
3000c42a:	4802      	ldr	r0, [pc, #8]	; (3000c434 <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000c42c:	4684      	mov	ip, r0
3000c42e:	bc01      	pop	{r0}
3000c430:	4760      	bx	ip
3000c432:	bf00      	nop
3000c434:	000053f1 	.word	0x000053f1

3000c438 <__DiagPrintf_veneer>:
3000c438:	b401      	push	{r0}
3000c43a:	4802      	ldr	r0, [pc, #8]	; (3000c444 <__DiagPrintf_veneer+0xc>)
3000c43c:	4684      	mov	ip, r0
3000c43e:	bc01      	pop	{r0}
3000c440:	4760      	bx	ip
3000c442:	bf00      	nop
3000c444:	0000e5f9 	.word	0x0000e5f9

3000c448 <__SBOOT_Validate_Algorithm_A_veneer>:
3000c448:	b401      	push	{r0}
3000c44a:	4802      	ldr	r0, [pc, #8]	; (3000c454 <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000c44c:	4684      	mov	ip, r0
3000c44e:	bc01      	pop	{r0}
3000c450:	4760      	bx	ip
3000c452:	bf00      	nop
3000c454:	00003279 	.word	0x00003279

3000c458 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000c458:	b401      	push	{r0}
3000c45a:	4802      	ldr	r0, [pc, #8]	; (3000c464 <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000c45c:	4684      	mov	ip, r0
3000c45e:	bc01      	pop	{r0}
3000c460:	4760      	bx	ip
3000c462:	bf00      	nop
3000c464:	00005b4d 	.word	0x00005b4d

3000c468 <__SBOOT_Validate_Algorithm_B_veneer>:
3000c468:	b401      	push	{r0}
3000c46a:	4802      	ldr	r0, [pc, #8]	; (3000c474 <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000c46c:	4684      	mov	ip, r0
3000c46e:	bc01      	pop	{r0}
3000c470:	4760      	bx	ip
3000c472:	bf00      	nop
3000c474:	000033ed 	.word	0x000033ed

3000c478 <__DiagVSprintf_veneer>:
3000c478:	b401      	push	{r0}
3000c47a:	4802      	ldr	r0, [pc, #8]	; (3000c484 <__DiagVSprintf_veneer+0xc>)
3000c47c:	4684      	mov	ip, r0
3000c47e:	bc01      	pop	{r0}
3000c480:	4760      	bx	ip
3000c482:	bf00      	nop
3000c484:	0000e331 	.word	0x0000e331

3000c488 <__SBOOT_Validate_PubKey_B_veneer>:
3000c488:	b401      	push	{r0}
3000c48a:	4802      	ldr	r0, [pc, #8]	; (3000c494 <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000c48c:	4684      	mov	ip, r0
3000c48e:	bc01      	pop	{r0}
3000c490:	4760      	bx	ip
3000c492:	bf00      	nop
3000c494:	00003495 	.word	0x00003495

3000c498 <__SBOOT_Validate_Signature_B_veneer>:
3000c498:	b401      	push	{r0}
3000c49a:	4802      	ldr	r0, [pc, #8]	; (3000c4a4 <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000c49c:	4684      	mov	ip, r0
3000c49e:	bc01      	pop	{r0}
3000c4a0:	4760      	bx	ip
3000c4a2:	bf00      	nop
3000c4a4:	00003431 	.word	0x00003431

3000c4a8 <__SBOOT_Validate_PubKey_A_veneer>:
3000c4a8:	b401      	push	{r0}
3000c4aa:	4802      	ldr	r0, [pc, #8]	; (3000c4b4 <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000c4ac:	4684      	mov	ip, r0
3000c4ae:	bc01      	pop	{r0}
3000c4b0:	4760      	bx	ip
3000c4b2:	bf00      	nop
3000c4b4:	00003321 	.word	0x00003321

3000c4b8 <__SBOOT_Validate_ImgHash_B_veneer>:
3000c4b8:	b401      	push	{r0}
3000c4ba:	4802      	ldr	r0, [pc, #8]	; (3000c4c4 <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000c4bc:	4684      	mov	ip, r0
3000c4be:	bc01      	pop	{r0}
3000c4c0:	4760      	bx	ip
3000c4c2:	bf00      	nop
3000c4c4:	00003541 	.word	0x00003541

3000c4c8 <____wrap_memcmp_veneer>:
3000c4c8:	b401      	push	{r0}
3000c4ca:	4802      	ldr	r0, [pc, #8]	; (3000c4d4 <____wrap_memcmp_veneer+0xc>)
3000c4cc:	4684      	mov	ip, r0
3000c4ce:	bc01      	pop	{r0}
3000c4d0:	4760      	bx	ip
3000c4d2:	bf00      	nop
3000c4d4:	00012f0d 	.word	0x00012f0d

3000c4d8 <____wrap_strlen_veneer>:
3000c4d8:	b401      	push	{r0}
3000c4da:	4802      	ldr	r0, [pc, #8]	; (3000c4e4 <____wrap_strlen_veneer+0xc>)
3000c4dc:	4684      	mov	ip, r0
3000c4de:	bc01      	pop	{r0}
3000c4e0:	4760      	bx	ip
3000c4e2:	bf00      	nop
3000c4e4:	00012f25 	.word	0x00012f25

3000c4e8 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000c4e8:	b401      	push	{r0}
3000c4ea:	4802      	ldr	r0, [pc, #8]	; (3000c4f4 <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000c4ec:	4684      	mov	ip, r0
3000c4ee:	bc01      	pop	{r0}
3000c4f0:	4760      	bx	ip
3000c4f2:	bf00      	nop
3000c4f4:	000059d9 	.word	0x000059d9

3000c4f8 <__SBOOT_Validate_Signature_A_veneer>:
3000c4f8:	b401      	push	{r0}
3000c4fa:	4802      	ldr	r0, [pc, #8]	; (3000c504 <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000c4fc:	4684      	mov	ip, r0
3000c4fe:	bc01      	pop	{r0}
3000c500:	4760      	bx	ip
3000c502:	bf00      	nop
3000c504:	000032bd 	.word	0x000032bd

3000c508 <__rtl_crypto_sha2_init_A_veneer>:
3000c508:	b401      	push	{r0}
3000c50a:	4802      	ldr	r0, [pc, #8]	; (3000c514 <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000c50c:	4684      	mov	ip, r0
3000c50e:	bc01      	pop	{r0}
3000c510:	4760      	bx	ip
3000c512:	bf00      	nop
3000c514:	00004e89 	.word	0x00004e89

3000c518 <__SBOOT_GetMdType_B_veneer>:
3000c518:	b401      	push	{r0}
3000c51a:	4802      	ldr	r0, [pc, #8]	; (3000c524 <__SBOOT_GetMdType_B_veneer+0xc>)
3000c51c:	4684      	mov	ip, r0
3000c51e:	bc01      	pop	{r0}
3000c520:	4760      	bx	ip
3000c522:	bf00      	nop
3000c524:	0000319d 	.word	0x0000319d

3000c528 <__rtl_crypto_sha2_final_A_veneer>:
3000c528:	b401      	push	{r0}
3000c52a:	4802      	ldr	r0, [pc, #8]	; (3000c534 <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000c52c:	4684      	mov	ip, r0
3000c52e:	bc01      	pop	{r0}
3000c530:	4760      	bx	ip
3000c532:	bf00      	nop
3000c534:	0000527d 	.word	0x0000527d

3000c538 <__io_assert_failed_veneer>:
3000c538:	b401      	push	{r0}
3000c53a:	4802      	ldr	r0, [pc, #8]	; (3000c544 <__io_assert_failed_veneer+0xc>)
3000c53c:	4684      	mov	ip, r0
3000c53e:	bc01      	pop	{r0}
3000c540:	4760      	bx	ip
3000c542:	bf00      	nop
3000c544:	0000992d 	.word	0x0000992d

3000c548 <__rtl_crypto_sha2_init_B_veneer>:
3000c548:	b401      	push	{r0}
3000c54a:	4802      	ldr	r0, [pc, #8]	; (3000c554 <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000c54c:	4684      	mov	ip, r0
3000c54e:	bc01      	pop	{r0}
3000c550:	4760      	bx	ip
3000c552:	bf00      	nop
3000c554:	00004ffd 	.word	0x00004ffd

3000c558 <__SBOOT_Validate_ImgHash_A_veneer>:
3000c558:	b401      	push	{r0}
3000c55a:	4802      	ldr	r0, [pc, #8]	; (3000c564 <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000c55c:	4684      	mov	ip, r0
3000c55e:	bc01      	pop	{r0}
3000c560:	4760      	bx	ip
3000c562:	bf00      	nop
3000c564:	000033cd 	.word	0x000033cd

3000c568 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000c568:	b401      	push	{r0}
3000c56a:	4802      	ldr	r0, [pc, #8]	; (3000c574 <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000c56c:	4684      	mov	ip, r0
3000c56e:	bc01      	pop	{r0}
3000c570:	4760      	bx	ip
3000c572:	bf00      	nop
3000c574:	00005a45 	.word	0x00005a45

3000c578 <__CRYPTO_SHA_Init_A_veneer>:
3000c578:	b401      	push	{r0}
3000c57a:	4802      	ldr	r0, [pc, #8]	; (3000c584 <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000c57c:	4684      	mov	ip, r0
3000c57e:	bc01      	pop	{r0}
3000c580:	4760      	bx	ip
3000c582:	bf00      	nop
3000c584:	000022dd 	.word	0x000022dd

3000c588 <__SBOOT_GetMdType_A_veneer>:
3000c588:	b401      	push	{r0}
3000c58a:	4802      	ldr	r0, [pc, #8]	; (3000c594 <__SBOOT_GetMdType_A_veneer+0xc>)
3000c58c:	4684      	mov	ip, r0
3000c58e:	bc01      	pop	{r0}
3000c590:	4760      	bx	ip
3000c592:	bf00      	nop
3000c594:	00003029 	.word	0x00003029

3000c598 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000c598:	b401      	push	{r0}
3000c59a:	4802      	ldr	r0, [pc, #8]	; (3000c5a4 <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000c59c:	4684      	mov	ip, r0
3000c59e:	bc01      	pop	{r0}
3000c5a0:	4760      	bx	ip
3000c5a2:	bf00      	nop
3000c5a4:	00005bb9 	.word	0x00005bb9

3000c5a8 <__rtl_crypto_sha2_update_B_veneer>:
3000c5a8:	b401      	push	{r0}
3000c5aa:	4802      	ldr	r0, [pc, #8]	; (3000c5b4 <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000c5ac:	4684      	mov	ip, r0
3000c5ae:	bc01      	pop	{r0}
3000c5b0:	4760      	bx	ip
3000c5b2:	bf00      	nop
3000c5b4:	00005385 	.word	0x00005385

3000c5b8 <__rtl_crypto_sha2_update_A_veneer>:
3000c5b8:	b401      	push	{r0}
3000c5ba:	4802      	ldr	r0, [pc, #8]	; (3000c5c4 <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000c5bc:	4684      	mov	ip, r0
3000c5be:	bc01      	pop	{r0}
3000c5c0:	4760      	bx	ip
3000c5c2:	bf00      	nop
3000c5c4:	00005211 	.word	0x00005211
3000c5c8:	004f4931 	.word	0x004f4931
3000c5cc:	00004f32 	.word	0x00004f32
3000c5d0:	004f4932 	.word	0x004f4932
3000c5d4:	00004f34 	.word	0x00004f34
3000c5d8:	004f4934 	.word	0x004f4934
3000c5dc:	53414c46 	.word	0x53414c46
3000c5e0:	4b4c4348 	.word	0x4b4c4348
3000c5e4:	00000000 	.word	0x00000000
3000c5e8:	2d73255b 	.word	0x2d73255b
3000c5ec:	63205d49 	.word	0x63205d49
3000c5f0:	62696c61 	.word	0x62696c61
3000c5f4:	69746172 	.word	0x69746172
3000c5f8:	6f5f6e6f 	.word	0x6f5f6e6f
3000c5fc:	255b3a6b 	.word	0x255b3a6b
3000c600:	64253a64 	.word	0x64253a64
3000c604:	5d64253a 	.word	0x5d64253a
3000c608:	00000a20 	.word	0x00000a20
3000c60c:	2d73255b 	.word	0x2d73255b
3000c610:	46205d49 	.word	0x46205d49
3000c614:	4853414c 	.word	0x4853414c
3000c618:	4c414320 	.word	0x4c414320
3000c61c:	305b4249 	.word	0x305b4249
3000c620:	20782578 	.word	0x20782578
3000c624:	0a5d4b4f 	.word	0x0a5d4b4f
3000c628:	00000000 	.word	0x00000000
3000c62c:	2d73255b 	.word	0x2d73255b
3000c630:	46205d45 	.word	0x46205d45
3000c634:	4853414c 	.word	0x4853414c
3000c638:	4c414320 	.word	0x4c414320
3000c63c:	305b4249 	.word	0x305b4249
3000c640:	20782578 	.word	0x20782578
3000c644:	4c494146 	.word	0x4c494146
3000c648:	00000a5d 	.word	0x00000a5d
3000c64c:	2d73255b 	.word	0x2d73255b
3000c650:	46205d49 	.word	0x46205d49
3000c654:	6873616c 	.word	0x6873616c
3000c658:	61655220 	.word	0x61655220
3000c65c:	73252064 	.word	0x73252064
3000c660:	0000000a 	.word	0x0000000a
3000c664:	2d73255b 	.word	0x2d73255b
3000c668:	46205d45 	.word	0x46205d45
3000c66c:	6873616c 	.word	0x6873616c
3000c670:	69775320 	.word	0x69775320
3000c674:	20686374 	.word	0x20686374
3000c678:	64616552 	.word	0x64616552
3000c67c:	646f4d20 	.word	0x646f4d20
3000c680:	41462065 	.word	0x41462065
3000c684:	000a4c49 	.word	0x000a4c49
3000c688:	2d73255b 	.word	0x2d73255b
3000c68c:	46205d49 	.word	0x46205d49
3000c690:	6873616c 	.word	0x6873616c
3000c694:	3a444920 	.word	0x3a444920
3000c698:	2d782520 	.word	0x2d782520
3000c69c:	252d7825 	.word	0x252d7825
3000c6a0:	00000a78 	.word	0x00000a78
3000c6a4:	2d73255b 	.word	0x2d73255b
3000c6a8:	54205d57 	.word	0x54205d57
3000c6ac:	20736968 	.word	0x20736968
3000c6b0:	73616c66 	.word	0x73616c66
3000c6b4:	79742068 	.word	0x79742068
3000c6b8:	69206570 	.word	0x69206570
3000c6bc:	6f6e2073 	.word	0x6f6e2073
3000c6c0:	75732074 	.word	0x75732074
3000c6c4:	726f7070 	.word	0x726f7070
3000c6c8:	21646574 	.word	0x21646574
3000c6cc:	0000000a 	.word	0x0000000a
3000c6d0:	2d73255b 	.word	0x2d73255b
3000c6d4:	4e205d49 	.word	0x4e205d49
3000c6d8:	20444e41 	.word	0x20444e41
3000c6dc:	203a4449 	.word	0x203a4449
3000c6e0:	252d7825 	.word	0x252d7825
3000c6e4:	00000a78 	.word	0x00000a78
3000c6e8:	00003552 	.word	0x00003552
3000c6ec:	00003652 	.word	0x00003652
3000c6f0:	00003752 	.word	0x00003752
3000c6f4:	00003852 	.word	0x00003852
3000c6f8:	00003952 	.word	0x00003952
3000c6fc:	00303152 	.word	0x00303152
3000c700:	00313152 	.word	0x00313152
3000c704:	00003152 	.word	0x00003152
3000c708:	00003252 	.word	0x00003252
3000c70c:	00003352 	.word	0x00003352
3000c710:	544f4f42 	.word	0x544f4f42
3000c714:	00000000 	.word	0x00000000
3000c718:	2d73255b 	.word	0x2d73255b
3000c71c:	48205d41 	.word	0x48205d41
3000c720:	20647261 	.word	0x20647261
3000c724:	6c756146 	.word	0x6c756146
3000c728:	61502074 	.word	0x61502074
3000c72c:	20686374 	.word	0x20686374
3000c730:	63655328 	.word	0x63655328
3000c734:	29657275 	.word	0x29657275
3000c738:	00000a0d 	.word	0x00000a0d
3000c73c:	2d73255b 	.word	0x2d73255b
3000c740:	45205d41 	.word	0x45205d41
3000c744:	70656378 	.word	0x70656378
3000c748:	6e6f6974 	.word	0x6e6f6974
3000c74c:	6f726620 	.word	0x6f726620
3000c750:	6573206d 	.word	0x6573206d
3000c754:	65727563 	.word	0x65727563
3000c758:	726f7720 	.word	0x726f7720
3000c75c:	0d21646c 	.word	0x0d21646c
3000c760:	0000000a 	.word	0x0000000a
3000c764:	2d73255b 	.word	0x2d73255b
3000c768:	45205d41 	.word	0x45205d41
3000c76c:	70656378 	.word	0x70656378
3000c770:	6e6f6974 	.word	0x6e6f6974
3000c774:	6f726620 	.word	0x6f726620
3000c778:	6f6e206d 	.word	0x6f6e206d
3000c77c:	65732d6e 	.word	0x65732d6e
3000c780:	65727563 	.word	0x65727563
3000c784:	726f7720 	.word	0x726f7720
3000c788:	0d21646c 	.word	0x0d21646c
3000c78c:	0000000a 	.word	0x0000000a
3000c790:	2d73255b 	.word	0x2d73255b
3000c794:	53205d41 	.word	0x53205d41
3000c798:	20525346 	.word	0x20525346
3000c79c:	30203d20 	.word	0x30203d20
3000c7a0:	38302578 	.word	0x38302578
3000c7a4:	000a0d78 	.word	0x000a0d78
3000c7a8:	2d73255b 	.word	0x2d73255b
3000c7ac:	53205d41 	.word	0x53205d41
3000c7b0:	20524146 	.word	0x20524146
3000c7b4:	30203d20 	.word	0x30203d20
3000c7b8:	38302578 	.word	0x38302578
3000c7bc:	000a0d78 	.word	0x000a0d78
3000c7c0:	2d73255b 	.word	0x2d73255b
3000c7c4:	53205d41 	.word	0x53205d41
3000c7c8:	52534348 	.word	0x52534348
3000c7cc:	30203d20 	.word	0x30203d20
3000c7d0:	38302578 	.word	0x38302578
3000c7d4:	000a0d78 	.word	0x000a0d78
3000c7d8:	2d73255b 	.word	0x2d73255b
3000c7dc:	42205d41 	.word	0x42205d41
3000c7e0:	5f50554b 	.word	0x5f50554b
3000c7e4:	32474552 	.word	0x32474552
3000c7e8:	25783020 	.word	0x25783020
3000c7ec:	0a0d2078 	.word	0x0a0d2078
3000c7f0:	00000000 	.word	0x00000000
3000c7f4:	3141544f 	.word	0x3141544f
3000c7f8:	45535520 	.word	0x45535520
3000c7fc:	6576202c 	.word	0x6576202c
3000c800:	6f697372 	.word	0x6f697372
3000c804:	25203a6e 	.word	0x25203a6e
3000c808:	00000a64 	.word	0x00000a64
3000c80c:	3241544f 	.word	0x3241544f
3000c810:	45535520 	.word	0x45535520
3000c814:	6576202c 	.word	0x6576202c
3000c818:	6f697372 	.word	0x6f697372
3000c81c:	25203a6e 	.word	0x25203a6e
3000c820:	00000a64 	.word	0x00000a64
3000c824:	68746f42 	.word	0x68746f42
3000c828:	41544f20 	.word	0x41544f20
3000c82c:	616d6920 	.word	0x616d6920
3000c830:	76206567 	.word	0x76206567
3000c834:	66697265 	.word	0x66697265
3000c838:	61662079 	.word	0x61662079
3000c83c:	64656c69 	.word	0x64656c69
3000c840:	0000000a 	.word	0x0000000a
3000c844:	2d73255b 	.word	0x2d73255b
3000c848:	49205d49 	.word	0x49205d49
3000c84c:	2074696e 	.word	0x2074696e
3000c850:	0a0d4257 	.word	0x0a0d4257
3000c854:	00000000 	.word	0x00000000
3000c858:	73616c66 	.word	0x73616c66
3000c85c:	69735f68 	.word	0x69735f68
3000c860:	203a657a 	.word	0x203a657a
3000c864:	000a4d38 	.word	0x000a4d38
3000c868:	73616c66 	.word	0x73616c66
3000c86c:	69735f68 	.word	0x69735f68
3000c870:	203a657a 	.word	0x203a657a
3000c874:	0a4d3631 	.word	0x0a4d3631
3000c878:	00000000 	.word	0x00000000
3000c87c:	73616c66 	.word	0x73616c66
3000c880:	69735f68 	.word	0x69735f68
3000c884:	203a657a 	.word	0x203a657a
3000c888:	0a4d3233 	.word	0x0a4d3233
3000c88c:	00000000 	.word	0x00000000
3000c890:	20315042 	.word	0x20315042
3000c894:	73726576 	.word	0x73726576
3000c898:	206e6f69 	.word	0x206e6f69
3000c89c:	61766e69 	.word	0x61766e69
3000c8a0:	0a64696c 	.word	0x0a64696c
3000c8a4:	00000000 	.word	0x00000000
3000c8a8:	20315042 	.word	0x20315042
3000c8ac:	6d726f66 	.word	0x6d726f66
3000c8b0:	69207461 	.word	0x69207461
3000c8b4:	6c61766e 	.word	0x6c61766e
3000c8b8:	000a6469 	.word	0x000a6469
3000c8bc:	20315042 	.word	0x20315042
3000c8c0:	69746361 	.word	0x69746361
3000c8c4:	69206576 	.word	0x69206576
3000c8c8:	7865646e 	.word	0x7865646e
3000c8cc:	766e6920 	.word	0x766e6920
3000c8d0:	64696c61 	.word	0x64696c61
3000c8d4:	0000000a 	.word	0x0000000a
3000c8d8:	20315042 	.word	0x20315042
3000c8dc:	61746164 	.word	0x61746164
3000c8e0:	6c617620 	.word	0x6c617620
3000c8e4:	202c6469 	.word	0x202c6469
3000c8e8:	73726576 	.word	0x73726576
3000c8ec:	3a6e6f69 	.word	0x3a6e6f69
3000c8f0:	000a6425 	.word	0x000a6425
3000c8f4:	20325042 	.word	0x20325042
3000c8f8:	6d726f66 	.word	0x6d726f66
3000c8fc:	69207461 	.word	0x69207461
3000c900:	6c61766e 	.word	0x6c61766e
3000c904:	000a6469 	.word	0x000a6469
3000c908:	20315042 	.word	0x20315042
3000c90c:	33435243 	.word	0x33435243
3000c910:	616d2032 	.word	0x616d2032
3000c914:	2c686374 	.word	0x2c686374
3000c918:	74746120 	.word	0x74746120
3000c91c:	65686361 	.word	0x65686361
3000c920:	52432064 	.word	0x52432064
3000c924:	30203a43 	.word	0x30203a43
3000c928:	2c782578 	.word	0x2c782578
3000c92c:	6c616320 	.word	0x6c616320
3000c930:	616c7563 	.word	0x616c7563
3000c934:	20646574 	.word	0x20646574
3000c938:	3a435243 	.word	0x3a435243
3000c93c:	25783020 	.word	0x25783020
3000c940:	00000a78 	.word	0x00000a78
3000c944:	20315042 	.word	0x20315042
3000c948:	33435243 	.word	0x33435243
3000c94c:	6f6e2032 	.word	0x6f6e2032
3000c950:	616d2074 	.word	0x616d2074
3000c954:	2c686374 	.word	0x2c686374
3000c958:	74746120 	.word	0x74746120
3000c95c:	65686361 	.word	0x65686361
3000c960:	52432064 	.word	0x52432064
3000c964:	30203a43 	.word	0x30203a43
3000c968:	2c782578 	.word	0x2c782578
3000c96c:	6c616320 	.word	0x6c616320
3000c970:	616c7563 	.word	0x616c7563
3000c974:	20646574 	.word	0x20646574
3000c978:	3a435243 	.word	0x3a435243
3000c97c:	25783020 	.word	0x25783020
3000c980:	00000a78 	.word	0x00000a78
3000c984:	20325042 	.word	0x20325042
3000c988:	33435243 	.word	0x33435243
3000c98c:	616d2032 	.word	0x616d2032
3000c990:	2c686374 	.word	0x2c686374
3000c994:	74746120 	.word	0x74746120
3000c998:	65686361 	.word	0x65686361
3000c99c:	52432064 	.word	0x52432064
3000c9a0:	30203a43 	.word	0x30203a43
3000c9a4:	2c782578 	.word	0x2c782578
3000c9a8:	6c616320 	.word	0x6c616320
3000c9ac:	616c7563 	.word	0x616c7563
3000c9b0:	20646574 	.word	0x20646574
3000c9b4:	3a435243 	.word	0x3a435243
3000c9b8:	25783020 	.word	0x25783020
3000c9bc:	00000a78 	.word	0x00000a78
3000c9c0:	20325042 	.word	0x20325042
3000c9c4:	33435243 	.word	0x33435243
3000c9c8:	6f6e2032 	.word	0x6f6e2032
3000c9cc:	616d2074 	.word	0x616d2074
3000c9d0:	2c686374 	.word	0x2c686374
3000c9d4:	74746120 	.word	0x74746120
3000c9d8:	65686361 	.word	0x65686361
3000c9dc:	52432064 	.word	0x52432064
3000c9e0:	30203a43 	.word	0x30203a43
3000c9e4:	2c782578 	.word	0x2c782578
3000c9e8:	6c616320 	.word	0x6c616320
3000c9ec:	616c7563 	.word	0x616c7563
3000c9f0:	20646574 	.word	0x20646574
3000c9f4:	3a435243 	.word	0x3a435243
3000c9f8:	25783020 	.word	0x25783020
3000c9fc:	00000a78 	.word	0x00000a78
3000ca00:	20315042 	.word	0x20315042
3000ca04:	3161746f 	.word	0x3161746f
3000ca08:	64646120 	.word	0x64646120
3000ca0c:	73736572 	.word	0x73736572
3000ca10:	766e6920 	.word	0x766e6920
3000ca14:	64696c61 	.word	0x64696c61
3000ca18:	0000000a 	.word	0x0000000a
3000ca1c:	20315042 	.word	0x20315042
3000ca20:	3261746f 	.word	0x3261746f
3000ca24:	64646120 	.word	0x64646120
3000ca28:	73736572 	.word	0x73736572
3000ca2c:	766e6920 	.word	0x766e6920
3000ca30:	64696c61 	.word	0x64696c61
3000ca34:	0000000a 	.word	0x0000000a
3000ca38:	68746f42 	.word	0x68746f42
3000ca3c:	20504220 	.word	0x20504220
3000ca40:	20435243 	.word	0x20435243
3000ca44:	696c6176 	.word	0x696c6176
3000ca48:	00000a64 	.word	0x00000a64
3000ca4c:	20315042 	.word	0x20315042
3000ca50:	656c6573 	.word	0x656c6573
3000ca54:	64657463 	.word	0x64657463
3000ca58:	0000000a 	.word	0x0000000a
3000ca5c:	20325042 	.word	0x20325042
3000ca60:	656c6573 	.word	0x656c6573
3000ca64:	64657463 	.word	0x64657463
3000ca68:	0000000a 	.word	0x0000000a
3000ca6c:	68746f42 	.word	0x68746f42
3000ca70:	20504220 	.word	0x20504220
3000ca74:	61766e69 	.word	0x61766e69
3000ca78:	0a64696c 	.word	0x0a64696c
3000ca7c:	00000000 	.word	0x00000000
3000ca80:	6f636552 	.word	0x6f636552
3000ca84:	79726576 	.word	0x79726576
3000ca88:	61747320 	.word	0x61747320
3000ca8c:	000a7472 	.word	0x000a7472
3000ca90:	69746341 	.word	0x69746341
3000ca94:	49206576 	.word	0x49206576
3000ca98:	7865646e 	.word	0x7865646e
3000ca9c:	6425203a 	.word	0x6425203a
3000caa0:	0000000a 	.word	0x0000000a
3000caa4:	3141544f 	.word	0x3141544f
3000caa8:	766e6920 	.word	0x766e6920
3000caac:	64696c61 	.word	0x64696c61
3000cab0:	0000000a 	.word	0x0000000a
3000cab4:	3241544f 	.word	0x3241544f
3000cab8:	766e6920 	.word	0x766e6920
3000cabc:	64696c61 	.word	0x64696c61
3000cac0:	0000000a 	.word	0x0000000a
3000cac4:	3141544f 	.word	0x3141544f
3000cac8:	43524320 	.word	0x43524320
3000cacc:	6d203233 	.word	0x6d203233
3000cad0:	68637461 	.word	0x68637461
3000cad4:	7461202c 	.word	0x7461202c
3000cad8:	68636174 	.word	0x68636174
3000cadc:	43206465 	.word	0x43206465
3000cae0:	203a4352 	.word	0x203a4352
3000cae4:	78257830 	.word	0x78257830
3000cae8:	6163202c 	.word	0x6163202c
3000caec:	6c75636c 	.word	0x6c75636c
3000caf0:	64657461 	.word	0x64657461
3000caf4:	43524320 	.word	0x43524320
3000caf8:	7830203a 	.word	0x7830203a
3000cafc:	000a7825 	.word	0x000a7825
3000cb00:	3141544f 	.word	0x3141544f
3000cb04:	43524320 	.word	0x43524320
3000cb08:	6e203233 	.word	0x6e203233
3000cb0c:	6d20746f 	.word	0x6d20746f
3000cb10:	68637461 	.word	0x68637461
3000cb14:	7461202c 	.word	0x7461202c
3000cb18:	68636174 	.word	0x68636174
3000cb1c:	43206465 	.word	0x43206465
3000cb20:	203a4352 	.word	0x203a4352
3000cb24:	78257830 	.word	0x78257830
3000cb28:	6163202c 	.word	0x6163202c
3000cb2c:	6c75636c 	.word	0x6c75636c
3000cb30:	64657461 	.word	0x64657461
3000cb34:	43524320 	.word	0x43524320
3000cb38:	7830203a 	.word	0x7830203a
3000cb3c:	202c7825 	.word	0x202c7825
3000cb40:	73617265 	.word	0x73617265
3000cb44:	544f2065 	.word	0x544f2065
3000cb48:	000a3141 	.word	0x000a3141
3000cb4c:	3241544f 	.word	0x3241544f
3000cb50:	43524320 	.word	0x43524320
3000cb54:	6d203233 	.word	0x6d203233
3000cb58:	68637461 	.word	0x68637461
3000cb5c:	7461202c 	.word	0x7461202c
3000cb60:	68636174 	.word	0x68636174
3000cb64:	43206465 	.word	0x43206465
3000cb68:	203a4352 	.word	0x203a4352
3000cb6c:	78257830 	.word	0x78257830
3000cb70:	6163202c 	.word	0x6163202c
3000cb74:	6c75636c 	.word	0x6c75636c
3000cb78:	64657461 	.word	0x64657461
3000cb7c:	43524320 	.word	0x43524320
3000cb80:	7830203a 	.word	0x7830203a
3000cb84:	000a7825 	.word	0x000a7825
3000cb88:	3241544f 	.word	0x3241544f
3000cb8c:	43524320 	.word	0x43524320
3000cb90:	6e203233 	.word	0x6e203233
3000cb94:	6d20746f 	.word	0x6d20746f
3000cb98:	68637461 	.word	0x68637461
3000cb9c:	7461202c 	.word	0x7461202c
3000cba0:	68636174 	.word	0x68636174
3000cba4:	43206465 	.word	0x43206465
3000cba8:	203a4352 	.word	0x203a4352
3000cbac:	78257830 	.word	0x78257830
3000cbb0:	6163202c 	.word	0x6163202c
3000cbb4:	6c75636c 	.word	0x6c75636c
3000cbb8:	64657461 	.word	0x64657461
3000cbbc:	43524320 	.word	0x43524320
3000cbc0:	7830203a 	.word	0x7830203a
3000cbc4:	202c7825 	.word	0x202c7825
3000cbc8:	73617265 	.word	0x73617265
3000cbcc:	544f2065 	.word	0x544f2065
3000cbd0:	000a3241 	.word	0x000a3241
3000cbd4:	69746341 	.word	0x69746341
3000cbd8:	69206576 	.word	0x69206576
3000cbdc:	7865646e 	.word	0x7865646e
3000cbe0:	41544f20 	.word	0x41544f20
3000cbe4:	766e6920 	.word	0x766e6920
3000cbe8:	64696c61 	.word	0x64696c61
3000cbec:	6572202c 	.word	0x6572202c
3000cbf0:	74697277 	.word	0x74697277
3000cbf4:	63612065 	.word	0x63612065
3000cbf8:	65766974 	.word	0x65766974
3000cbfc:	646e6920 	.word	0x646e6920
3000cc00:	74207865 	.word	0x74207865
3000cc04:	6425206f 	.word	0x6425206f
3000cc08:	0000000a 	.word	0x0000000a
3000cc0c:	206c6c41 	.word	0x206c6c41
3000cc10:	2041544f 	.word	0x2041544f
3000cc14:	72646461 	.word	0x72646461
3000cc18:	65737365 	.word	0x65737365
3000cc1c:	6f6e2073 	.word	0x6f6e2073
3000cc20:	61762074 	.word	0x61762074
3000cc24:	2c64696c 	.word	0x2c64696c
3000cc28:	63657220 	.word	0x63657220
3000cc2c:	7265766f 	.word	0x7265766f
3000cc30:	61662079 	.word	0x61662079
3000cc34:	000a6c69 	.word	0x000a6c69
3000cc38:	20325042 	.word	0x20325042
3000cc3c:	3261746f 	.word	0x3261746f
3000cc40:	64646120 	.word	0x64646120
3000cc44:	73736572 	.word	0x73736572
3000cc48:	766e6920 	.word	0x766e6920
3000cc4c:	64696c61 	.word	0x64696c61
3000cc50:	0000000a 	.word	0x0000000a
3000cc54:	20325042 	.word	0x20325042
3000cc58:	73726576 	.word	0x73726576
3000cc5c:	206e6f69 	.word	0x206e6f69
3000cc60:	61766e69 	.word	0x61766e69
3000cc64:	0a64696c 	.word	0x0a64696c
3000cc68:	00000000 	.word	0x00000000
3000cc6c:	20325042 	.word	0x20325042
3000cc70:	69746361 	.word	0x69746361
3000cc74:	69206576 	.word	0x69206576
3000cc78:	7865646e 	.word	0x7865646e
3000cc7c:	766e6920 	.word	0x766e6920
3000cc80:	64696c61 	.word	0x64696c61
3000cc84:	0000000a 	.word	0x0000000a
3000cc88:	20325042 	.word	0x20325042
3000cc8c:	3161746f 	.word	0x3161746f
3000cc90:	64646120 	.word	0x64646120
3000cc94:	73736572 	.word	0x73736572
3000cc98:	766e6920 	.word	0x766e6920
3000cc9c:	64696c61 	.word	0x64696c61
3000cca0:	0000000a 	.word	0x0000000a
3000cca4:	20325042 	.word	0x20325042
3000cca8:	61746164 	.word	0x61746164
3000ccac:	6c617620 	.word	0x6c617620
3000ccb0:	202c6469 	.word	0x202c6469
3000ccb4:	73726576 	.word	0x73726576
3000ccb8:	3a6e6f69 	.word	0x3a6e6f69
3000ccbc:	000a6425 	.word	0x000a6425
3000ccc0:	2d73255b 	.word	0x2d73255b
3000ccc4:	44205d49 	.word	0x44205d49
3000ccc8:	62617369 	.word	0x62617369
3000cccc:	4b20656c 	.word	0x4b20656c
3000ccd0:	4220304d 	.word	0x4220304d
3000ccd4:	20746f6f 	.word	0x20746f6f
3000ccd8:	206e6920 	.word	0x206e6920
3000ccdc:	74736554 	.word	0x74736554
3000cce0:	646f6d20 	.word	0x646f6d20
3000cce4:	000a0d65 	.word	0x000a0d65
3000cce8:	2d73255b 	.word	0x2d73255b
3000ccec:	42205d49 	.word	0x42205d49
3000ccf0:	73617079 	.word	0x73617079
3000ccf4:	544f2073 	.word	0x544f2073
3000ccf8:	75612050 	.word	0x75612050
3000ccfc:	6f6c6f74 	.word	0x6f6c6f74
3000cd00:	0a0d6461 	.word	0x0a0d6461
3000cd04:	00000000 	.word	0x00000000
3000cd08:	2d73255b 	.word	0x2d73255b
3000cd0c:	4e205d49 	.word	0x4e205d49
3000cd10:	72462050 	.word	0x72462050
3000cd14:	25207165 	.word	0x25207165
3000cd18:	484d2064 	.word	0x484d2064
3000cd1c:	00000a7a 	.word	0x00000a7a
3000cd20:	2d73255b 	.word	0x2d73255b
3000cd24:	41205d49 	.word	0x41205d49
3000cd28:	72462050 	.word	0x72462050
3000cd2c:	25207165 	.word	0x25207165
3000cd30:	484d2064 	.word	0x484d2064
3000cd34:	00000a7a 	.word	0x00000a7a
3000cd38:	7361700a 	.word	0x7361700a
3000cd3c:	726f7773 	.word	0x726f7773
3000cd40:	65762064 	.word	0x65762064
3000cd44:	79666972 	.word	0x79666972
3000cd48:	69616620 	.word	0x69616620
3000cd4c:	00000a6c 	.word	0x00000a6c
3000cd50:	7361700a 	.word	0x7361700a
3000cd54:	726f7773 	.word	0x726f7773
3000cd58:	65762064 	.word	0x65762064
3000cd5c:	79666972 	.word	0x79666972
3000cd60:	63757320 	.word	0x63757320
3000cd64:	73736563 	.word	0x73736563
3000cd68:	0000000a 	.word	0x0000000a
3000cd6c:	31474d49 	.word	0x31474d49
3000cd70:	4f425320 	.word	0x4f425320
3000cd74:	4f20544f 	.word	0x4f20544f
3000cd78:	000a4646 	.word	0x000a4646
3000cd7c:	31474d49 	.word	0x31474d49
3000cd80:	4f425320 	.word	0x4f425320
3000cd84:	5320544f 	.word	0x5320544f
3000cd88:	45434355 	.word	0x45434355
3000cd8c:	000a5353 	.word	0x000a5353
3000cd90:	746f6f62 	.word	0x746f6f62
3000cd94:	64616f6c 	.word	0x64616f6c
3000cd98:	765f7265 	.word	0x765f7265
3000cd9c:	69737265 	.word	0x69737265
3000cda0:	25206e6f 	.word	0x25206e6f
3000cda4:	00000a73 	.word	0x00000a73
3000cda8:	444f4d5b 	.word	0x444f4d5b
3000cdac:	5f454c55 	.word	0x5f454c55
3000cdb0:	544f4f42 	.word	0x544f4f42
3000cdb4:	56454c2d 	.word	0x56454c2d
3000cdb8:	495f4c45 	.word	0x495f4c45
3000cdbc:	5d4f464e 	.word	0x5d4f464e
3000cdc0:	474d493a 	.word	0x474d493a
3000cdc4:	4e452031 	.word	0x4e452031
3000cdc8:	20524554 	.word	0x20524554
3000cdcc:	3a50534d 	.word	0x3a50534d
3000cdd0:	3830255b 	.word	0x3830255b
3000cdd4:	000a5d78 	.word	0x000a5d78
3000cdd8:	444f4d5b 	.word	0x444f4d5b
3000cddc:	5f454c55 	.word	0x5f454c55
3000cde0:	544f4f42 	.word	0x544f4f42
3000cde4:	56454c2d 	.word	0x56454c2d
3000cde8:	495f4c45 	.word	0x495f4c45
3000cdec:	5d4f464e 	.word	0x5d4f464e
3000cdf0:	474d493a 	.word	0x474d493a
3000cdf4:	45532031 	.word	0x45532031
3000cdf8:	45525543 	.word	0x45525543
3000cdfc:	41545320 	.word	0x41545320
3000ce00:	203a4554 	.word	0x203a4554
3000ce04:	000a6425 	.word	0x000a6425
3000ce08:	2d73255b 	.word	0x2d73255b
3000ce0c:	49205d49 	.word	0x49205d49
3000ce10:	2074696e 	.word	0x2074696e
3000ce14:	41525350 	.word	0x41525350
3000ce18:	000a0d4d 	.word	0x000a0d4d
3000ce1c:	2d73255b 	.word	0x2d73255b
3000ce20:	73205d57 	.word	0x73205d57
3000ce24:	70207465 	.word	0x70207465
3000ce28:	66206d66 	.word	0x66206d66
3000ce2c:	0d6c6961 	.word	0x0d6c6961
3000ce30:	0000000a 	.word	0x0000000a
3000ce34:	2d73255b 	.word	0x2d73255b
3000ce38:	45205d45 	.word	0x45205d45
3000ce3c:	524f5252 	.word	0x524f5252
3000ce40:	53202121 	.word	0x53202121
3000ce44:	6c756f68 	.word	0x6c756f68
3000ce48:	6f4e2064 	.word	0x6f4e2064
3000ce4c:	6e652074 	.word	0x6e652074
3000ce50:	656c6261 	.word	0x656c6261
3000ce54:	6d654d20 	.word	0x6d654d20
3000ce58:	5f727753 	.word	0x5f727753
3000ce5c:	796c6e4f 	.word	0x796c6e4f
3000ce60:	206e6920 	.word	0x206e6920
3000ce64:	20524444 	.word	0x20524444
3000ce68:	70696843 	.word	0x70696843
3000ce6c:	0d212121 	.word	0x0d212121
3000ce70:	0000000a 	.word	0x0000000a
3000ce74:	2d73255b 	.word	0x2d73255b
3000ce78:	49205d49 	.word	0x49205d49
3000ce7c:	2074696e 	.word	0x2074696e
3000ce80:	0d524444 	.word	0x0d524444
3000ce84:	0000000a 	.word	0x0000000a
3000ce88:	2d73255b 	.word	0x2d73255b
3000ce8c:	42205d49 	.word	0x42205d49
3000ce90:	5f50554b 	.word	0x5f50554b
3000ce94:	32474552 	.word	0x32474552
3000ce98:	25783020 	.word	0x25783020
3000ce9c:	0a0d2078 	.word	0x0a0d2078
3000cea0:	00000000 	.word	0x00000000
3000cea4:	55676f4c 	.word	0x55676f4c
3000cea8:	20747261 	.word	0x20747261
3000ceac:	64756142 	.word	0x64756142
3000ceb0:	65746172 	.word	0x65746172
3000ceb4:	6425203a 	.word	0x6425203a
3000ceb8:	0000000a 	.word	0x0000000a
3000cebc:	4848530a 	.word	0x4848530a
3000cec0:	4946544f 	.word	0x4946544f
3000cec4:	6f6e2058 	.word	0x6f6e2058
3000cec8:	65722074 	.word	0x65722074
3000cecc:	76696563 	.word	0x76696563
3000ced0:	000a6465 	.word	0x000a6465
3000ced4:	726f4e0a 	.word	0x726f4e0a
3000ced8:	206c616d 	.word	0x206c616d
3000cedc:	746f6f62 	.word	0x746f6f62
3000cee0:	0000000a 	.word	0x0000000a
3000cee4:	5f6e6962 	.word	0x5f6e6962
3000cee8:	313e6f6e 	.word	0x313e6f6e
3000ceec:	3d202c30 	.word	0x3d202c30
3000cef0:	0a642520 	.word	0x0a642520
3000cef4:	00000000 	.word	0x00000000
3000cef8:	6e206164 	.word	0x6e206164
3000cefc:	7220746f 	.word	0x7220746f
3000cf00:	69656365 	.word	0x69656365
3000cf04:	0a646576 	.word	0x0a646576
3000cf08:	00000000 	.word	0x00000000
3000cf0c:	746f680a 	.word	0x746f680a
3000cf10:	20786966 	.word	0x20786966
3000cf14:	656e6f64 	.word	0x656e6f64
3000cf18:	202e2e2e 	.word	0x202e2e2e
3000cf1c:	6f626552 	.word	0x6f626552
3000cf20:	6e69746f 	.word	0x6e69746f
3000cf24:	0a2e2e67 	.word	0x0a2e2e67
3000cf28:	00000000 	.word	0x00000000
3000cf2c:	20304d4b 	.word	0x20304d4b
3000cf30:	20504958 	.word	0x20504958
3000cf34:	00474d49 	.word	0x00474d49
3000cf38:	20304d4b 	.word	0x20304d4b
3000cf3c:	4d415253 	.word	0x4d415253
3000cf40:	00000000 	.word	0x00000000
3000cf44:	20304d4b 	.word	0x20304d4b
3000cf48:	4d415244 	.word	0x4d415244
3000cf4c:	00000000 	.word	0x00000000
3000cf50:	20344d4b 	.word	0x20344d4b
3000cf54:	20504958 	.word	0x20504958
3000cf58:	00474d49 	.word	0x00474d49
3000cf5c:	20344d4b 	.word	0x20344d4b
3000cf60:	4d415253 	.word	0x4d415253
3000cf64:	00000000 	.word	0x00000000
3000cf68:	20344d4b 	.word	0x20344d4b
3000cf6c:	4d415244 	.word	0x4d415244
3000cf70:	00000000 	.word	0x00000000
3000cf74:	58205041 	.word	0x58205041
3000cf78:	49205049 	.word	0x49205049
3000cf7c:	0000474d 	.word	0x0000474d
3000cf80:	42205041 	.word	0x42205041
3000cf84:	5320314c 	.word	0x5320314c
3000cf88:	004d4152 	.word	0x004d4152
3000cf8c:	42205041 	.word	0x42205041
3000cf90:	4420314c 	.word	0x4420314c
3000cf94:	004d4152 	.word	0x004d4152
3000cf98:	46205041 	.word	0x46205041
3000cf9c:	00005049 	.word	0x00005049
3000cfa0:	2d73255b 	.word	0x2d73255b
3000cfa4:	25205d49 	.word	0x25205d49
3000cfa8:	6e492073 	.word	0x6e492073
3000cfac:	696c6176 	.word	0x696c6176
3000cfb0:	00000a64 	.word	0x00000a64
3000cfb4:	2d73255b 	.word	0x2d73255b
3000cfb8:	25205d49 	.word	0x25205d49
3000cfbc:	30255b73 	.word	0x30255b73
3000cfc0:	253a7838 	.word	0x253a7838
3000cfc4:	000a5d78 	.word	0x000a5d78
3000cfc8:	2d73255b 	.word	0x2d73255b
3000cfcc:	52205d49 	.word	0x52205d49
3000cfd0:	45205044 	.word	0x45205044
3000cfd4:	00000a4e 	.word	0x00000a4e
3000cfd8:	2d73255b 	.word	0x2d73255b
3000cfdc:	46205d49 	.word	0x46205d49
3000cfe0:	206c6961 	.word	0x206c6961
3000cfe4:	6c206f74 	.word	0x6c206f74
3000cfe8:	2064616f 	.word	0x2064616f
3000cfec:	20504452 	.word	0x20504452
3000cff0:	67616d69 	.word	0x67616d69
3000cff4:	000a2165 	.word	0x000a2165
3000cff8:	2d73255b 	.word	0x2d73255b
3000cffc:	52205d49 	.word	0x52205d49
3000d000:	4f205044 	.word	0x4f205044
3000d004:	000a4646 	.word	0x000a4646
3000d008:	2d73255b 	.word	0x2d73255b
3000d00c:	49205d49 	.word	0x49205d49
3000d010:	2032474d 	.word	0x2032474d
3000d014:	2046544f 	.word	0x2046544f
3000d018:	000a4e45 	.word	0x000a4e45
3000d01c:	2d73255b 	.word	0x2d73255b
3000d020:	4f205d45 	.word	0x4f205d45
3000d024:	4d204654 	.word	0x4d204654
3000d028:	2065646f 	.word	0x2065646f
3000d02c:	6f727265 	.word	0x6f727265
3000d030:	000a0d72 	.word	0x000a0d72
3000d034:	494c4156 	.word	0x494c4156
3000d038:	00000044 	.word	0x00000044
3000d03c:	41564e49 	.word	0x41564e49
3000d040:	0044494c 	.word	0x0044494c
3000d044:	74726563 	.word	0x74726563
3000d048:	63696669 	.word	0x63696669
3000d04c:	20657461 	.word	0x20657461
3000d050:	20434345 	.word	0x20434345
3000d054:	69726576 	.word	0x69726576
3000d058:	66207966 	.word	0x66207966
3000d05c:	2c6c6961 	.word	0x2c6c6961
3000d060:	6c6f7220 	.word	0x6c6f7220
3000d064:	6361626c 	.word	0x6361626c
3000d068:	000a0d6b 	.word	0x000a0d6b
3000d06c:	444f4d5b 	.word	0x444f4d5b
3000d070:	5f454c55 	.word	0x5f454c55
3000d074:	544f4f42 	.word	0x544f4f42
3000d078:	56454c2d 	.word	0x56454c2d
3000d07c:	495f4c45 	.word	0x495f4c45
3000d080:	5d4f464e 	.word	0x5d4f464e
3000d084:	474d493a 	.word	0x474d493a
3000d088:	544f2832 	.word	0x544f2832
3000d08c:	29642541 	.word	0x29642541
3000d090:	2c732520 	.word	0x2c732520
3000d094:	74657220 	.word	0x74657220
3000d098:	6425203a 	.word	0x6425203a
3000d09c:	0000000a 	.word	0x0000000a
3000d0a0:	32474d49 	.word	0x32474d49
3000d0a4:	43434520 	.word	0x43434520
3000d0a8:	72657620 	.word	0x72657620
3000d0ac:	20796669 	.word	0x20796669
3000d0b0:	6c696166 	.word	0x6c696166
3000d0b4:	6f72202c 	.word	0x6f72202c
3000d0b8:	61626c6c 	.word	0x61626c6c
3000d0bc:	0a0d6b63 	.word	0x0a0d6b63
3000d0c0:	00000000 	.word	0x00000000
3000d0c4:	5f41544f 	.word	0x5f41544f
3000d0c8:	45205041 	.word	0x45205041
3000d0cc:	76204343 	.word	0x76204343
3000d0d0:	66697265 	.word	0x66697265
3000d0d4:	61662079 	.word	0x61662079
3000d0d8:	202c6c69 	.word	0x202c6c69
3000d0dc:	6c6c6f72 	.word	0x6c6c6f72
3000d0e0:	6b636162 	.word	0x6b636162
3000d0e4:	00000a0d 	.word	0x00000a0d
3000d0e8:	64616f4c 	.word	0x64616f4c
3000d0ec:	41544f20 	.word	0x41544f20
3000d0f0:	2050415f 	.word	0x2050415f
3000d0f4:	67616d69 	.word	0x67616d69
3000d0f8:	61662065 	.word	0x61662065
3000d0fc:	202c6c69 	.word	0x202c6c69
3000d100:	6c6c6f72 	.word	0x6c6c6f72
3000d104:	6b636162 	.word	0x6b636162
3000d108:	00000a0d 	.word	0x00000a0d
3000d10c:	444f4d5b 	.word	0x444f4d5b
3000d110:	5f454c55 	.word	0x5f454c55
3000d114:	544f4f42 	.word	0x544f4f42
3000d118:	56454c2d 	.word	0x56454c2d
3000d11c:	455f4c45 	.word	0x455f4c45
3000d120:	524f5252 	.word	0x524f5252
3000d124:	61463a5d 	.word	0x61463a5d
3000d128:	74206c69 	.word	0x74206c69
3000d12c:	6f6c206f 	.word	0x6f6c206f
3000d130:	41206461 	.word	0x41206461
3000d134:	6d692050 	.word	0x6d692050
3000d138:	21656761 	.word	0x21656761
3000d13c:	0000000a 	.word	0x0000000a
3000d140:	20344d4b 	.word	0x20344d4b
3000d144:	33474d49 	.word	0x33474d49
3000d148:	00000000 	.word	0x00000000
3000d14c:	20344d4b 	.word	0x20344d4b
3000d150:	0043534e 	.word	0x0043534e
3000d154:	31474d49 	.word	0x31474d49
3000d158:	00000000 	.word	0x00000000
3000d15c:	32474d49 	.word	0x32474d49
3000d160:	00000000 	.word	0x00000000
3000d164:	42205041 	.word	0x42205041
3000d168:	4626314c 	.word	0x4626314c
3000d16c:	00005049 	.word	0x00005049
3000d170:	444f4d5b 	.word	0x444f4d5b
3000d174:	5f454c55 	.word	0x5f454c55
3000d178:	544f4f42 	.word	0x544f4f42
3000d17c:	56454c2d 	.word	0x56454c2d
3000d180:	495f4c45 	.word	0x495f4c45
3000d184:	5d4f464e 	.word	0x5d4f464e
3000d188:	5b73253a 	.word	0x5b73253a
3000d18c:	78383025 	.word	0x78383025
3000d190:	5d78253a 	.word	0x5d78253a
3000d194:	0000000a 	.word	0x0000000a
3000d198:	20504452 	.word	0x20504452
3000d19c:	4c494146 	.word	0x4c494146
3000d1a0:	0000000a 	.word	0x0000000a
3000d1a4:	20504452 	.word	0x20504452
3000d1a8:	43435553 	.word	0x43435553
3000d1ac:	0a535345 	.word	0x0a535345
3000d1b0:	00000000 	.word	0x00000000
3000d1b4:	059e4eb3 	.word	0x059e4eb3
3000d1b8:	4b11ea39 	.word	0x4b11ea39
3000d1bc:	2a1b920a 	.word	0x2a1b920a
3000d1c0:	29939cee 	.word	0x29939cee
3000d1c4:	00000000 	.word	0x00000000
3000d1c8:	42205041 	.word	0x42205041
3000d1cc:	0032334c 	.word	0x0032334c
3000d1d0:	2d73255b 	.word	0x2d73255b
3000d1d4:	25205d49 	.word	0x25205d49
3000d1d8:	45562073 	.word	0x45562073
3000d1dc:	59464952 	.word	0x59464952
3000d1e0:	53415020 	.word	0x53415020
3000d1e4:	00000a53 	.word	0x00000a53
3000d1e8:	2d73255b 	.word	0x2d73255b
3000d1ec:	25205d45 	.word	0x25205d45
3000d1f0:	45562073 	.word	0x45562073
3000d1f4:	59464952 	.word	0x59464952
3000d1f8:	49414620 	.word	0x49414620
3000d1fc:	72202c4c 	.word	0x72202c4c
3000d200:	3d207465 	.word	0x3d207465
3000d204:	0a642520 	.word	0x0a642520
3000d208:	00000000 	.word	0x00000000
3000d20c:	2d73255b 	.word	0x2d73255b
3000d210:	43205d45 	.word	0x43205d45
3000d214:	69747265 	.word	0x69747265
3000d218:	61636966 	.word	0x61636966
3000d21c:	56206574 	.word	0x56206574
3000d220:	46495245 	.word	0x46495245
3000d224:	41462059 	.word	0x41462059
3000d228:	202c4c49 	.word	0x202c4c49
3000d22c:	20746572 	.word	0x20746572
3000d230:	6425203d 	.word	0x6425203d
3000d234:	0000000a 	.word	0x0000000a
3000d238:	6c630a0d 	.word	0x6c630a0d
3000d23c:	2065736f 	.word	0x2065736f
3000d240:	646f4d79 	.word	0x646f4d79
3000d244:	54206d65 	.word	0x54206d65
3000d248:	736e6172 	.word	0x736e6172
3000d24c:	20726566 	.word	0x20726566
3000d250:	0d2e2e2e 	.word	0x0d2e2e2e
3000d254:	0000000a 	.word	0x0000000a
3000d258:	50494843 	.word	0x50494843
3000d25c:	4f464e49 	.word	0x4f464e49
3000d260:	00000000 	.word	0x00000000
3000d264:	2d73255b 	.word	0x2d73255b
3000d268:	49205d45 	.word	0x49205d45
3000d26c:	6c61766e 	.word	0x6c61766e
3000d270:	43206469 	.word	0x43206469
3000d274:	696e6968 	.word	0x696e6968
3000d278:	216f666e 	.word	0x216f666e
3000d27c:	65684320 	.word	0x65684320
3000d280:	4f206b63 	.word	0x4f206b63
3000d284:	000a5054 	.word	0x000a5054
3000d288:	2d73255b 	.word	0x2d73255b
3000d28c:	50205d57 	.word	0x50205d57
3000d290:	7361656c 	.word	0x7361656c
3000d294:	72702065 	.word	0x72702065
3000d298:	6172676f 	.word	0x6172676f
3000d29c:	6863206d 	.word	0x6863206d
3000d2a0:	6e697069 	.word	0x6e697069
3000d2a4:	69206f66 	.word	0x69206f66
3000d2a8:	544f206e 	.word	0x544f206e
3000d2ac:	0a212050 	.word	0x0a212050
3000d2b0:	00000000 	.word	0x00000000

3000d2b4 <Memory_Info>:
3000d2b4:	03e00007 00060004 010202c0 02010100     ................
3000d2c4:	02000102 00040302 02040400 01010102     ................
3000d2d4:	01020221 02240401 03000102 00030103     !.....$.........
3000d2e4:	01050500 06000002 01020206 01070700     ................
3000d2f4:	08000002 01020208 02280801 09000102     ..........(.....
3000d304:	00040309 03290901 0a000004 0003010a     ......).........
3000d314:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d324:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d334:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d344:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d354:	62696c61 6f696172 000a0d6e              alibraion...

3000d360 <ddrphy_tx_scan>:
3000d360:	00000000 00080000 00080808 00000000     ................
3000d370:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d380:	07060012 000f0707 080f0f0f 00000008     ................
3000d390:	20000001 22222222 000c0012 34000000     ... """".......4
3000d3a0:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d3b0:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d3c0:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d3d0:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d3e0:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d3f0:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d400:	0e0e001a 00170e0e 12171717 00000012     ................
3000d410:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d420:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d430:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d440:	0f0f001b 00180f0f 13181818 00000013     ................
3000d450:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d460:	1110001b 00191111 13191919 00000013     ................
3000d470:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d480:	1313001c 001c1313 121c1c1c 00000012     ................
3000d490:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d4a0 <ddrphy_zq_rx_scan>:
	...
3000d4a8:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d4b8:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d4c8:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d4d8:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d4e8:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d4f8:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d508:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d518:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d528:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d538:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d548:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d558:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d568 <Flash_AVL>:
3000d568:	000000ef 000000ff 00000000 000043fc     .............C..
3000d578:	00000000 000000a1 000000ff 00000000     ................
3000d588:	0000fffc 00000000 0000000b 000000ff     ................
3000d598:	00000000 000043fc 00000000 0000000e     .....C..........
3000d5a8:	000000ff 00000000 000043fc 00000000     .........C......
3000d5b8:	000000c8 000000ff 00000001 000043fc     .............C..
3000d5c8:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d5d8:	000200fc 00000000 000000c2 000000ff     ................
3000d5e8:	00000002 000000fc 00000000 00000068     ............h...
3000d5f8:	000000ff 00000002 000000fc 00000000     ................
3000d608:	00000051 000000ff 00000002 000000fc     Q...............
3000d618:	00000000 0000001c 000000ff 00000003     ................
3000d628:	000000fc 00000000 00000020 000000ff     ........ .......
3000d638:	00000004 000000fc 00000000 00000000     ................
3000d648:	000000ff 000000fe ffffffff 3000864d     ............M..0
3000d658:	000000ff ffffffff 000000ff ffffffff     ................
3000d668:	00000000                                ....

3000d66c <Flash_ReadMode>:
3000d66c:	                                         ..

3000d66e <Flash_Speed>:
3000d66e:	                                         ..

3000d670 <NAND_AVL>:
3000d670:	000000ef 000000ff 00000000 ffffffff     ................
3000d680:	00000000 000000c8 000000ff 00000001     ................
3000d690:	ffffffff 00000000 000000e5 000000ff     ................
3000d6a0:	00000001 ffffffff 00000000 000000c2     ................
3000d6b0:	000000ff 00000002 ffffffff 00000000     ................
3000d6c0:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d6d8:	000000ff 000000fe ffffffff 300086ad     ...............0
3000d6e8:	000000ff ffffffff 000000ff ffffffff     ................
3000d6f8:	00000000 3000c5c8 3000c5cc 3000c5d0     .......0...0...0
3000d708:	3000c5d4 3000c5d8                       ...0...0

3000d710 <__FUNCTION__.0>:
3000d710:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d720 <__FUNCTION__.1>:
3000d720:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d730:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000d740:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000d750:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000d760:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000d770:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000d780:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000d790:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000d7a0:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d7b0:	00000a20                                 ...

3000d7b4 <__FUNCTION__.1>:
3000d7b4:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d7c4:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000d7d4:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000d7e4:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000d7f4:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d804:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d814:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d824:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d834:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d844:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d854:	726f6620 41485320 00000a32 2d73255b      for SHA2...[%s-
3000d864:	69205d57 255b7172 70205d64 726f6972     W] irq[%d] prior
3000d874:	20797469 73206425 6c6c6168 203d3c20     ity %d shall <= 
3000d884:	000a6425                                %d..

3000d888 <__FUNCTION__.0>:
3000d888:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d898:	00000000                                ....

3000d89c <__FUNCTION__.1>:
3000d89c:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d8ac <__FUNCTION__.3>:
3000d8ac:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d8bc:	32616873 696e695f 00000074              sha2_init...

3000d8c8 <__FUNCTION__.4>:
3000d8c8:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d8d8:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000d8e8:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000d8f8:	6e207369 6320746f 69666e6f 65727567     is not configure
3000d908:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000d918:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000d928:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000d938:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000d948:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000d958:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000d968:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000d978:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000d988:	69625f6b 21217374 00000a21              k_bits!!!...

3000d994 <__FUNCTION__.0>:
3000d994:	33697872 445f3631 72536e79 6e695f65     rxi316_DynSre_in
3000d9a4:	00007469                                it..

3000d9a8 <__FUNCTION__.1>:
3000d9a8:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d9b8:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000d9c8:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000d9d8:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000d9e8:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000d9f8:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000da08:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000da18:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000da28:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000da38:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000da48:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000da58:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000da68:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000da78:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000da88:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000da98:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000daa8:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000dab8:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000dac8:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000dad8:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000dae8:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000daf8:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000db08:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000db18:	00000000                                ....

3000db1c <register_string>:
3000db1c:	00000000 3000da44 3000c6e8 3000c6ec     ....D..0...0...0
3000db2c:	3000c6f0 3000c6f4 3000c6f8 3000c6fc     ...0...0...0...0
3000db3c:	3000c700 3000da40 3000c704 3000c708     ...0@..0...0...0
3000db4c:	3000c70c 3000dae4 65637845 6f697470     ...0...0Exceptio
3000db5c:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000db6c:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000db7c:	64657375 20796220 6c696166 76206465     used by failed v
3000db8c:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000db9c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000dbac:	00000000 75636553 79746972 75616620     ....Security fau
3000dbbc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dbcc:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000dbdc:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000dbec:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000dbfc:	79746972 75616620 6920746c 61632073     rity fault is ca
3000dc0c:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000dc1c:	65746e69 74697267 69732079 74616e67     integrity signat
3000dc2c:	00657275 75636553 79746972 75616620     ure.Security fau
3000dc3c:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dc4c:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000dc5c:	6572206e 6e727574 00000000 75636553     n return....Secu
3000dc6c:	79746972 75616620 6920746c 61632073     rity fault is ca
3000dc7c:	64657375 20796220 72747441 74756269     used by Attribut
3000dc8c:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000dc9c:	00006e6f 75636553 79746972 75616620     on..Security fau
3000dcac:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dcbc:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000dccc:	00006e6f 75636553 79746972 75616620     on..Security fau
3000dcdc:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dcec:	797a614c 61747320 70206574 65736572     Lazy state prese
3000dcfc:	74617672 206e6f69 6f727265 00000072     rvation error...
3000dd0c:	75636553 79746972 75616620 6920746c     Security fault i
3000dd1c:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000dd2c:	61747320 65206574 726f7272 00000000      state error....
3000dd3c:	20656854 75636573 79746972 75616620     The security fau
3000dd4c:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000dd5c:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000dd6c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000dd7c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000dd8c:	61662074 20746c75 63207369 65737561     t fault is cause
3000dd9c:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000ddac:	63636120 20737365 6c6f6976 6f697461      access violatio
3000ddbc:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000ddcc:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000dddc:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000ddec:	20746c75 63207369 65737561 79622064     ult is caused by
3000ddfc:	74616420 63612061 73736563 6f697620      data access vio
3000de0c:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000de1c:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000de2c:	63207369 65737561 79622064 736e7520     is caused by uns
3000de3c:	6b636174 20676e69 6f727265 00000072     tacking error...
3000de4c:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000de5c:	61662074 20746c75 63207369 65737561     t fault is cause
3000de6c:	79622064 61747320 6e696b63 72652067     d by stacking er
3000de7c:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000de8c:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000de9c:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000deac:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000debc:	70206574 65736572 74617672 006e6f69     te preservation.
3000decc:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000dedc:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000deec:	64657272 64646120 73736572 00736920     rred address is.
3000defc:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000df0c:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000df1c:	73692074 75616320 20646573 69207962     t is caused by i
3000df2c:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000df3c:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000df4c:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000df5c:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000df6c:	75616320 20646573 70207962 69636572      caused by preci
3000df7c:	64206573 20617461 65636361 76207373     se data access v
3000df8c:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000df9c:	6c756166 73692074 75616320 20646573     fault is caused 
3000dfac:	69207962 6572706d 65736963 74616420     by imprecise dat
3000dfbc:	63612061 73736563 6f697620 6974616c     a access violati
3000dfcc:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000dfdc:	75616320 20646573 75207962 6174736e      caused by unsta
3000dfec:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000dffc:	6c756166 73692074 75616320 20646573     fault is caused 
3000e00c:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000e01c:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000e02c:	75616320 20646573 66207962 74616f6c      caused by float
3000e03c:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000e04c:	65746174 65727020 76726573 6f697461     tate preservatio
3000e05c:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000e06c:	636f2074 72727563 61206465 65726464     t occurred addre
3000e07c:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000e08c:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e09c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e0ac:	65737561 79622064 74746120 74706d65     aused by attempt
3000e0bc:	6f742073 65786520 65747563 206e6120     s to execute an 
3000e0cc:	65646e75 656e6966 6e692064 75727473     undefined instru
3000e0dc:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000e0ec:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e0fc:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e10c:	65737561 79622064 74746120 74706d65     aused by attempt
3000e11c:	6f742073 69777320 20686374 61206f74     s to switch to a
3000e12c:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000e13c:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e14c:	61662065 20746c75 63207369 65737561     e fault is cause
3000e15c:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e16c:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e17c:	74697720 20612068 20646162 756c6176      with a bad valu
3000e18c:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e19c:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e1ac:	61662065 20746c75 63207369 65737561     e fault is cause
3000e1bc:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e1cc:	65786520 65747563 63206120 6f72706f      execute a copro
3000e1dc:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e1ec:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e1fc:	63207369 65737561 79622064 646e6920     is caused by ind
3000e20c:	74616369 74207365 20746168 74732061     icates that a st
3000e21c:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e22c:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e23c:	61742073 206e656b 63616c70 00000065     s taken place...
3000e24c:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e25c:	65737561 79622064 646e6920 74616369     aused by indicat
3000e26c:	74207365 20746168 75206e61 696c616e     es that an unali
3000e27c:	64656e67 63636120 20737365 6c756166     gned access faul
3000e28c:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e29c:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e2ac:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e2bc:	74616369 61207365 76696420 20656469     icates a divide 
3000e2cc:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e2dc:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e2ec:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e2fc:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e30c:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e31c:	65737561 79622064 6c616820 65722074     aused by halt re
3000e32c:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e33c:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e34c:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e35c:	20746c75 63207369 65737561 79622064     ult is caused by
3000e36c:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e37c:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e38c:	61662067 20746c75 63207369 65737561     g fault is cause
3000e39c:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e3ac:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e3bc:	20746c75 63207369 65737561 79622064     ult is caused by
3000e3cc:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e3dc:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e3ec:	20746c75 63207369 65737561 79622064     ult is caused by
3000e3fc:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e40c:	72657373 00646574 00000208 0000020c     sserted.........
3000e41c:	00000210 00000008 00000214 00000218     ................
3000e42c:	0000021c 0000000c                       ........

3000e434 <__FUNCTION__.0>:
3000e434:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000e444:	3000cf2c 3000cf38 3000cf44 3000cf50     ,..08..0D..0P..0
3000e454:	3000cf5c 3000cf68 3000cf74 3000cf80     \..0h..0t..0...0
3000e464:	3000cf8c 3000cf98 3000d140 3000d14c     ...0...0@..0L..0
3000e474:	3000d154 3000d15c 3000d144 3000d164     T..0\..0D..0d..0

3000e484 <km4_bootloader_rev>:
3000e484:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e494:	615f7265 65306131 65643765 30325f62     er_a1a0ee7deb_20
3000e4a4:	312f3432 31332f32 3a35312d 333a3434     24/12/31-15:44:3
3000e4b4:	4f420033 5b00544f 572d7325 2a2a205d     3.BOOT.[%s-W] **
3000e4c4:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e4d4:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000e4e4:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000e4f4:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000e504:	20202a20 20202020 20202020 20202020      *              
3000e514:	20202020 20202020 20202020 20202020                     
3000e524:	20202020 20202020 20202020 20202020                     
3000e534:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000e544:	205d572d 2020202a 20202020 20202020     -W] *           
3000e554:	75432020 6e657272 68632074 76207069       Current chip v
3000e564:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000e574:	20202021 20202020 20202020 000a2a20     !            *..
3000e584:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000e594:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000e5a4:	2074726f 72727563 20746e65 73726576     ort current vers
3000e5b4:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000e5c4:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000e5d4:	656c5020 20657361 746e6f63 20746361      Please contact 
3000e5e4:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000e5f4:	646f7270 20746375 6f666e69 20202021     product info!   
3000e604:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000e614:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e624:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e634:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e644:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000e654:	0043000a 49534f50 002e0058              ..C.POSIX...

3000e660 <_ctype_>:
3000e660:	20202000 20202020 28282020 20282828     .         ((((( 
3000e670:	20202020 20202020 20202020 20202020                     
3000e680:	10108820 10101010 10101010 10101010      ...............
3000e690:	04040410 04040404 10040404 10101010     ................
3000e6a0:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000e6b0:	01010101 01010101 01010101 10101010     ................
3000e6c0:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000e6d0:	02020202 02020202 02020202 10101010     ................
3000e6e0:	00000020 00000000 00000000 00000000      ...............
	...
