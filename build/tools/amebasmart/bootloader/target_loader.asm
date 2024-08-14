
/home/ziyik/sambashare/tizenrt_8730e/sdk/project/realtek_amebaD2_va0_example/GCC-RELEASE/project_hp/asdk/image/target_loader.axf:     file format elf32-littlearm


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
300032c4:	30009bb5 	.word	0x30009bb5
300032c8:	2001c01c 	.word	0x2001c01c
300032cc:	00009c99 	.word	0x00009c99
300032d0:	e000ed00 	.word	0xe000ed00
300032d4:	000129bd 	.word	0x000129bd
300032d8:	0000b7e9 	.word	0x0000b7e9
300032dc:	ffff0000 	.word	0xffff0000
300032e0:	300099bd 	.word	0x300099bd
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
300033fc:	30009bb5 	.word	0x30009bb5
30003400:	2001c01c 	.word	0x2001c01c
30003404:	00009c99 	.word	0x00009c99
30003408:	300099bd 	.word	0x300099bd
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
3000349c:	f008 fd26 	bl	3000beec <rtk_log_write>
300034a0:	2001      	movs	r0, #1
300034a2:	f886 b061 	strb.w	fp, [r6, #97]	; 0x61
300034a6:	f886 a01c 	strb.w	sl, [r6, #28]
300034aa:	f886 a01d 	strb.w	sl, [r6, #29]
300034ae:	b007      	add	sp, #28
300034b0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300034b4:	2001c01c 	.word	0x2001c01c
300034b8:	3000c57c 	.word	0x3000c57c
300034bc:	3000c588 	.word	0x3000c588

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
300034f0:	f008 fcfc 	bl	3000beec <rtk_log_write>
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
30003524:	f008 fce2 	bl	3000beec <rtk_log_write>
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
30003540:	3000c57c 	.word	0x3000c57c
30003544:	3000c5cc 	.word	0x3000c5cc
30003548:	30009bb5 	.word	0x30009bb5
3000354c:	0000c045 	.word	0x0000c045
30003550:	3000c5ac 	.word	0x3000c5ac
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
3000365a:	f008 fc47 	bl	3000beec <rtk_log_write>
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
3000367a:	f008 fc37 	bl	3000beec <rtk_log_write>
3000367e:	2001      	movs	r0, #1
30003680:	b00b      	add	sp, #44	; 0x2c
30003682:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30003686:	bf00      	nop
30003688:	3000d664 	.word	0x3000d664
3000368c:	2001c01c 	.word	0x2001c01c
30003690:	000129bd 	.word	0x000129bd
30003694:	e000ed00 	.word	0xe000ed00
30003698:	00054060 	.word	0x00054060
3000369c:	00009db5 	.word	0x00009db5
300036a0:	0000a149 	.word	0x0000a149
300036a4:	3000c57c 	.word	0x3000c57c
300036a8:	3000c604 	.word	0x3000c604
300036ac:	3000c5ec 	.word	0x3000c5ec
300036b0:	30009bb5 	.word	0x30009bb5
300036b4:	300099bd 	.word	0x300099bd
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
3000373e:	f008 fbd5 	bl	3000beec <rtk_log_write>
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
30003790:	f008 fbac 	bl	3000beec <rtk_log_write>
30003794:	4879      	ldr	r0, [pc, #484]	; (3000397c <flash_highspeed_setup+0x2bc>)
30003796:	f240 2127 	movw	r1, #551	; 0x227
3000379a:	f008 fe9d 	bl	3000c4d8 <__io_assert_failed_veneer>
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
300037ca:	f008 fb8f 	bl	3000beec <rtk_log_write>
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
30003816:	f008 fb69 	bl	3000beec <rtk_log_write>
3000381a:	485b      	ldr	r0, [pc, #364]	; (30003988 <flash_highspeed_setup+0x2c8>)
3000381c:	f240 215e 	movw	r1, #606	; 0x25e
30003820:	f008 fe5a 	bl	3000c4d8 <__io_assert_failed_veneer>
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
30003958:	3000d5d4 	.word	0x3000d5d4
3000395c:	3000d5d6 	.word	0x3000d5d6
30003960:	2001c01c 	.word	0x2001c01c
30003964:	00009db5 	.word	0x00009db5
30003968:	3000c57c 	.word	0x3000c57c
3000396c:	3000c628 	.word	0x3000c628
30003970:	3000d4d0 	.word	0x3000d4d0
30003974:	3000d5d8 	.word	0x3000d5d8
30003978:	3000c644 	.word	0x3000c644
3000397c:	3000d688 	.word	0x3000d688
30003980:	0000b8c1 	.word	0x0000b8c1
30003984:	3000c670 	.word	0x3000c670
30003988:	3000d678 	.word	0x3000d678
3000398c:	0000c1d9 	.word	0x0000c1d9
30003990:	3000fc51 	.word	0x3000fc51
30003994:	000129bd 	.word	0x000129bd
30003998:	0000a149 	.word	0x0000a149
3000399c:	0000c0f9 	.word	0x0000c0f9
300039a0:	0000bd0d 	.word	0x0000bd0d
300039a4:	0000bfcd 	.word	0x0000bfcd
300039a8:	0000be71 	.word	0x0000be71
300039ac:	0000bee9 	.word	0x0000bee9
300039b0:	0000bf59 	.word	0x0000bf59
300039b4:	30009bb5 	.word	0x30009bb5
300039b8:	3000fc54 	.word	0x3000fc54
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
30003a36:	f008 fd4f 	bl	3000c4d8 <__io_assert_failed_veneer>
30003a3a:	f8d8 3000 	ldr.w	r3, [r8]
30003a3e:	691b      	ldr	r3, [r3, #16]
30003a40:	e767      	b.n	30003912 <flash_highspeed_setup+0x252>
30003a42:	f44f 7111 	mov.w	r1, #580	; 0x244
30003a46:	480c      	ldr	r0, [pc, #48]	; (30003a78 <flash_highspeed_setup+0x3b8>)
30003a48:	f008 fd46 	bl	3000c4d8 <__io_assert_failed_veneer>
30003a4c:	f8d8 3000 	ldr.w	r3, [r8]
30003a50:	691b      	ldr	r3, [r3, #16]
30003a52:	e705      	b.n	30003860 <flash_highspeed_setup+0x1a0>
30003a54:	4b09      	ldr	r3, [pc, #36]	; (30003a7c <flash_highspeed_setup+0x3bc>)
30003a56:	4798      	blx	r3
30003a58:	e703      	b.n	30003862 <flash_highspeed_setup+0x1a2>
30003a5a:	bf00      	nop
30003a5c:	0000a3c5 	.word	0x0000a3c5
30003a60:	3000fc51 	.word	0x3000fc51
30003a64:	0000a345 	.word	0x0000a345
30003a68:	0000a441 	.word	0x0000a441
30003a6c:	0000b991 	.word	0x0000b991
30003a70:	0000bd0d 	.word	0x0000bd0d
30003a74:	3000d678 	.word	0x3000d678
30003a78:	3000d688 	.word	0x3000d688
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
30003afc:	f004 f828 	bl	30007b50 <ymodem_uart_getdata>
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
30003bc4:	4b3c      	ldr	r3, [pc, #240]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003bc6:	b095      	sub	sp, #84	; 0x54
30003bc8:	4616      	mov	r6, r2
30003bca:	4604      	mov	r4, r0
30003bcc:	4a3b      	ldr	r2, [pc, #236]	; (30003cbc <BOOT_FaultHandler+0xfc>)
30003bce:	2001      	movs	r0, #1
30003bd0:	460d      	mov	r5, r1
30003bd2:	4619      	mov	r1, r3
30003bd4:	f008 f98a 	bl	3000beec <rtk_log_write>
30003bd8:	2244      	movs	r2, #68	; 0x44
30003bda:	2100      	movs	r1, #0
30003bdc:	a803      	add	r0, sp, #12
30003bde:	f008 fbcb 	bl	3000c378 <____wrap_memset_veneer>
30003be2:	0672      	lsls	r2, r6, #25
30003be4:	4b34      	ldr	r3, [pc, #208]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003be6:	d555      	bpl.n	30003c94 <BOOT_FaultHandler+0xd4>
30003be8:	4619      	mov	r1, r3
30003bea:	4a35      	ldr	r2, [pc, #212]	; (30003cc0 <BOOT_FaultHandler+0x100>)
30003bec:	2001      	movs	r0, #1
30003bee:	f008 f97d 	bl	3000beec <rtk_log_write>
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
30003c34:	4c23      	ldr	r4, [pc, #140]	; (30003cc4 <BOOT_FaultHandler+0x104>)
30003c36:	aa03      	add	r2, sp, #12
30003c38:	4629      	mov	r1, r5
30003c3a:	6be8      	ldr	r0, [r5, #60]	; 0x3c
30003c3c:	f007 fe80 	bl	3000b940 <crash_dump>
30003c40:	f8d4 10e4 	ldr.w	r1, [r4, #228]	; 0xe4
30003c44:	4b1c      	ldr	r3, [pc, #112]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c46:	2001      	movs	r0, #1
30003c48:	9100      	str	r1, [sp, #0]
30003c4a:	4a1f      	ldr	r2, [pc, #124]	; (30003cc8 <BOOT_FaultHandler+0x108>)
30003c4c:	4619      	mov	r1, r3
30003c4e:	f008 f94d 	bl	3000beec <rtk_log_write>
30003c52:	f8d4 20e8 	ldr.w	r2, [r4, #232]	; 0xe8
30003c56:	4b18      	ldr	r3, [pc, #96]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c58:	2001      	movs	r0, #1
30003c5a:	9200      	str	r2, [sp, #0]
30003c5c:	4619      	mov	r1, r3
30003c5e:	4a1b      	ldr	r2, [pc, #108]	; (30003ccc <BOOT_FaultHandler+0x10c>)
30003c60:	f008 f944 	bl	3000beec <rtk_log_write>
30003c64:	6a61      	ldr	r1, [r4, #36]	; 0x24
30003c66:	4b14      	ldr	r3, [pc, #80]	; (30003cb8 <BOOT_FaultHandler+0xf8>)
30003c68:	2001      	movs	r0, #1
30003c6a:	9100      	str	r1, [sp, #0]
30003c6c:	4a18      	ldr	r2, [pc, #96]	; (30003cd0 <BOOT_FaultHandler+0x110>)
30003c6e:	4619      	mov	r1, r3
30003c70:	f008 f93c 	bl	3000beec <rtk_log_write>
30003c74:	f3ef 8488 	mrs	r4, MSP_NS
30003c78:	f3ef 8589 	mrs	r5, PSP_NS
30003c7c:	2101      	movs	r1, #1
30003c7e:	2002      	movs	r0, #2
30003c80:	4b14      	ldr	r3, [pc, #80]	; (30003cd4 <BOOT_FaultHandler+0x114>)
30003c82:	4798      	blx	r3
30003c84:	463b      	mov	r3, r7
30003c86:	4632      	mov	r2, r6
30003c88:	4629      	mov	r1, r5
30003c8a:	4620      	mov	r0, r4
30003c8c:	f008 fb7c 	bl	3000c388 <__INT_HardFault_C_veneer>
30003c90:	b015      	add	sp, #84	; 0x54
30003c92:	bdf0      	pop	{r4, r5, r6, r7, pc}
30003c94:	4a10      	ldr	r2, [pc, #64]	; (30003cd8 <BOOT_FaultHandler+0x118>)
30003c96:	2001      	movs	r0, #1
30003c98:	4619      	mov	r1, r3
30003c9a:	f008 f927 	bl	3000beec <rtk_log_write>
30003c9e:	f3ef 8394 	mrs	r3, CONTROL_NS
30003ca2:	f3ef 8e88 	mrs	lr, MSP_NS
30003ca6:	f3ef 8589 	mrs	r5, PSP_NS
30003caa:	f016 0108 	ands.w	r1, r6, #8
30003cae:	bf18      	it	ne
30003cb0:	f3c3 0140 	ubfxne	r1, r3, #1, #1
30003cb4:	e7a5      	b.n	30003c02 <BOOT_FaultHandler+0x42>
30003cb6:	bf00      	nop
30003cb8:	3000c6b0 	.word	0x3000c6b0
30003cbc:	3000c6b8 	.word	0x3000c6b8
30003cc0:	3000c6dc 	.word	0x3000c6dc
30003cc4:	e000ed00 	.word	0xe000ed00
30003cc8:	3000c730 	.word	0x3000c730
30003ccc:	3000c748 	.word	0x3000c748
30003cd0:	3000c760 	.word	0x3000c760
30003cd4:	0000994d 	.word	0x0000994d
30003cd8:	3000c704 	.word	0x3000c704

30003cdc <FLASH_Erase_With_Lock>:
30003cdc:	4b11      	ldr	r3, [pc, #68]	; (30003d24 <FLASH_Erase_With_Lock+0x48>)
30003cde:	b510      	push	{r4, lr}
30003ce0:	460c      	mov	r4, r1
30003ce2:	f101 4178 	add.w	r1, r1, #4160749568	; 0xf8000000
30003ce6:	4798      	blx	r3
30003ce8:	4b0f      	ldr	r3, [pc, #60]	; (30003d28 <FLASH_Erase_With_Lock+0x4c>)
30003cea:	695b      	ldr	r3, [r3, #20]
30003cec:	03db      	lsls	r3, r3, #15
30003cee:	d512      	bpl.n	30003d16 <FLASH_Erase_With_Lock+0x3a>
30003cf0:	f014 021f 	ands.w	r2, r4, #31
30003cf4:	d110      	bne.n	30003d18 <FLASH_Erase_With_Lock+0x3c>
30003cf6:	f44f 5280 	mov.w	r2, #4096	; 0x1000
30003cfa:	f3bf 8f4f 	dsb	sy
30003cfe:	480a      	ldr	r0, [pc, #40]	; (30003d28 <FLASH_Erase_With_Lock+0x4c>)
30003d00:	4422      	add	r2, r4
30003d02:	f8c0 425c 	str.w	r4, [r0, #604]	; 0x25c
30003d06:	3420      	adds	r4, #32
30003d08:	1b13      	subs	r3, r2, r4
30003d0a:	2b00      	cmp	r3, #0
30003d0c:	dcf9      	bgt.n	30003d02 <FLASH_Erase_With_Lock+0x26>
30003d0e:	f3bf 8f4f 	dsb	sy
30003d12:	f3bf 8f6f 	isb	sy
30003d16:	bd10      	pop	{r4, pc}
30003d18:	f024 041f 	bic.w	r4, r4, #31
30003d1c:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30003d20:	e7eb      	b.n	30003cfa <FLASH_Erase_With_Lock+0x1e>
30003d22:	bf00      	nop
30003d24:	0000a16d 	.word	0x0000a16d
30003d28:	e000ed00 	.word	0xe000ed00

30003d2c <FLASH_TxData_With_Lock>:
30003d2c:	b538      	push	{r3, r4, r5, lr}
30003d2e:	4b13      	ldr	r3, [pc, #76]	; (30003d7c <FLASH_TxData_With_Lock+0x50>)
30003d30:	4604      	mov	r4, r0
30003d32:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
30003d36:	460d      	mov	r5, r1
30003d38:	4798      	blx	r3
30003d3a:	4b11      	ldr	r3, [pc, #68]	; (30003d80 <FLASH_TxData_With_Lock+0x54>)
30003d3c:	695b      	ldr	r3, [r3, #20]
30003d3e:	03db      	lsls	r3, r3, #15
30003d40:	d517      	bpl.n	30003d72 <FLASH_TxData_With_Lock+0x46>
30003d42:	f014 031f 	ands.w	r3, r4, #31
30003d46:	d115      	bne.n	30003d74 <FLASH_TxData_With_Lock+0x48>
30003d48:	f3bf 8f4f 	dsb	sy
30003d4c:	b16d      	cbz	r5, 30003d6a <FLASH_TxData_With_Lock+0x3e>
30003d4e:	1e6a      	subs	r2, r5, #1
30003d50:	f104 0320 	add.w	r3, r4, #32
30003d54:	490a      	ldr	r1, [pc, #40]	; (30003d80 <FLASH_TxData_With_Lock+0x54>)
30003d56:	f022 021f 	bic.w	r2, r2, #31
30003d5a:	441a      	add	r2, r3
30003d5c:	4293      	cmp	r3, r2
30003d5e:	f8c1 425c 	str.w	r4, [r1, #604]	; 0x25c
30003d62:	461c      	mov	r4, r3
30003d64:	f103 0320 	add.w	r3, r3, #32
30003d68:	d1f8      	bne.n	30003d5c <FLASH_TxData_With_Lock+0x30>
30003d6a:	f3bf 8f4f 	dsb	sy
30003d6e:	f3bf 8f6f 	isb	sy
30003d72:	bd38      	pop	{r3, r4, r5, pc}
30003d74:	f024 041f 	bic.w	r4, r4, #31
30003d78:	441d      	add	r5, r3
30003d7a:	e7e5      	b.n	30003d48 <FLASH_TxData_With_Lock+0x1c>
30003d7c:	0000a04d 	.word	0x0000a04d
30003d80:	e000ed00 	.word	0xe000ed00

30003d84 <flash_read_id>:
30003d84:	b538      	push	{r3, r4, r5, lr}
30003d86:	4d04      	ldr	r5, [pc, #16]	; (30003d98 <flash_read_id+0x14>)
30003d88:	460c      	mov	r4, r1
30003d8a:	4602      	mov	r2, r0
30003d8c:	4b03      	ldr	r3, [pc, #12]	; (30003d9c <flash_read_id+0x18>)
30003d8e:	f895 0055 	ldrb.w	r0, [r5, #85]	; 0x55
30003d92:	4798      	blx	r3
30003d94:	4620      	mov	r0, r4
30003d96:	bd38      	pop	{r3, r4, r5, pc}
30003d98:	2001c01c 	.word	0x2001c01c
30003d9c:	00009db5 	.word	0x00009db5

30003da0 <mcc_ReverseBit>:
30003da0:	0843      	lsrs	r3, r0, #1
30003da2:	08c1      	lsrs	r1, r0, #3
30003da4:	f003 0308 	and.w	r3, r3, #8
30003da8:	0942      	lsrs	r2, r0, #5
30003daa:	f001 0104 	and.w	r1, r1, #4
30003dae:	ea4f 1c40 	mov.w	ip, r0, lsl #5
30003db2:	ea43 13c0 	orr.w	r3, r3, r0, lsl #7
30003db6:	f002 0202 	and.w	r2, r2, #2
30003dba:	f00c 0c40 	and.w	ip, ip, #64	; 0x40
30003dbe:	ea43 13d0 	orr.w	r3, r3, r0, lsr #7
30003dc2:	430b      	orrs	r3, r1
30003dc4:	00c1      	lsls	r1, r0, #3
30003dc6:	4313      	orrs	r3, r2
30003dc8:	f001 0120 	and.w	r1, r1, #32
30003dcc:	0042      	lsls	r2, r0, #1
30003dce:	ea43 000c 	orr.w	r0, r3, ip
30003dd2:	f002 0310 	and.w	r3, r2, #16
30003dd6:	4308      	orrs	r0, r1
30003dd8:	4318      	orrs	r0, r3
30003dda:	b2c0      	uxtb	r0, r0
30003ddc:	4770      	bx	lr
30003dde:	bf00      	nop

30003de0 <mcc_init_crc32>:
30003de0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30003de2:	2600      	movs	r6, #0
30003de4:	4f16      	ldr	r7, [pc, #88]	; (30003e40 <mcc_init_crc32+0x60>)
30003de6:	4605      	mov	r5, r0
30003de8:	b2f0      	uxtb	r0, r6
30003dea:	f7ff ffd9 	bl	30003da0 <mcc_ReverseBit>
30003dee:	2208      	movs	r2, #8
30003df0:	0604      	lsls	r4, r0, #24
30003df2:	2c00      	cmp	r4, #0
30003df4:	ea4f 0444 	mov.w	r4, r4, lsl #1
30003df8:	bfb8      	it	lt
30003dfa:	407c      	eorlt	r4, r7
30003dfc:	3a01      	subs	r2, #1
30003dfe:	d1f8      	bne.n	30003df2 <mcc_init_crc32+0x12>
30003e00:	0e20      	lsrs	r0, r4, #24
30003e02:	3504      	adds	r5, #4
30003e04:	f7ff ffcc 	bl	30003da0 <mcc_ReverseBit>
30003e08:	4603      	mov	r3, r0
30003e0a:	f3c4 4007 	ubfx	r0, r4, #16, #8
30003e0e:	3601      	adds	r6, #1
30003e10:	f805 3c04 	strb.w	r3, [r5, #-4]
30003e14:	f7ff ffc4 	bl	30003da0 <mcc_ReverseBit>
30003e18:	4603      	mov	r3, r0
30003e1a:	f3c4 2007 	ubfx	r0, r4, #8, #8
30003e1e:	f805 3c03 	strb.w	r3, [r5, #-3]
30003e22:	f7ff ffbd 	bl	30003da0 <mcc_ReverseBit>
30003e26:	4603      	mov	r3, r0
30003e28:	b2e0      	uxtb	r0, r4
30003e2a:	f805 3c02 	strb.w	r3, [r5, #-2]
30003e2e:	f7ff ffb7 	bl	30003da0 <mcc_ReverseBit>
30003e32:	f5b6 7f80 	cmp.w	r6, #256	; 0x100
30003e36:	f805 0c01 	strb.w	r0, [r5, #-1]
30003e3a:	d1d5      	bne.n	30003de8 <mcc_init_crc32+0x8>
30003e3c:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30003e3e:	bf00      	nop
30003e40:	04c11db7 	.word	0x04c11db7

30003e44 <mcc_crc32>:
30003e44:	b179      	cbz	r1, 30003e66 <mcc_crc32+0x22>
30003e46:	eb00 0c01 	add.w	ip, r0, r1
30003e4a:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003e4e:	f810 3b01 	ldrb.w	r3, [r0], #1
30003e52:	404b      	eors	r3, r1
30003e54:	4560      	cmp	r0, ip
30003e56:	b2db      	uxtb	r3, r3
30003e58:	f852 3023 	ldr.w	r3, [r2, r3, lsl #2]
30003e5c:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003e60:	d1f5      	bne.n	30003e4e <mcc_crc32+0xa>
30003e62:	43c8      	mvns	r0, r1
30003e64:	4770      	bx	lr
30003e66:	4608      	mov	r0, r1
30003e68:	4770      	bx	lr
30003e6a:	bf00      	nop

30003e6c <recovery_check>:
30003e6c:	f101 6300 	add.w	r3, r1, #134217728	; 0x8000000
30003e70:	4283      	cmp	r3, r0
30003e72:	d322      	bcc.n	30003eba <recovery_check+0x4e>
30003e74:	f1b0 6f00 	cmp.w	r0, #134217728	; 0x8000000
30003e78:	d91f      	bls.n	30003eba <recovery_check+0x4e>
30003e7a:	b510      	push	{r4, lr}
30003e7c:	f8d0 400a 	ldr.w	r4, [r0, #10]
30003e80:	f104 030c 	add.w	r3, r4, #12
30003e84:	4299      	cmp	r1, r3
30003e86:	d316      	bcc.n	30003eb6 <recovery_check+0x4a>
30003e88:	f100 0c04 	add.w	ip, r0, #4
30003e8c:	b183      	cbz	r3, 30003eb0 <recovery_check+0x44>
30003e8e:	f100 0e10 	add.w	lr, r0, #16
30003e92:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30003e96:	44a6      	add	lr, r4
30003e98:	4c0b      	ldr	r4, [pc, #44]	; (30003ec8 <recovery_check+0x5c>)
30003e9a:	f81c 3b01 	ldrb.w	r3, [ip], #1
30003e9e:	404b      	eors	r3, r1
30003ea0:	45f4      	cmp	ip, lr
30003ea2:	b2db      	uxtb	r3, r3
30003ea4:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003ea8:	ea83 2111 	eor.w	r1, r3, r1, lsr #8
30003eac:	d1f5      	bne.n	30003e9a <recovery_check+0x2e>
30003eae:	43cb      	mvns	r3, r1
30003eb0:	6801      	ldr	r1, [r0, #0]
30003eb2:	4299      	cmp	r1, r3
30003eb4:	d003      	beq.n	30003ebe <recovery_check+0x52>
30003eb6:	2000      	movs	r0, #0
30003eb8:	bd10      	pop	{r4, pc}
30003eba:	2000      	movs	r0, #0
30003ebc:	4770      	bx	lr
30003ebe:	f8d0 3006 	ldr.w	r3, [r0, #6]
30003ec2:	2001      	movs	r0, #1
30003ec4:	6013      	str	r3, [r2, #0]
30003ec6:	bd10      	pop	{r4, pc}
30003ec8:	3000e89c 	.word	0x3000e89c

30003ecc <rewrite_bp>:
30003ecc:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30003ed0:	b0a3      	sub	sp, #140	; 0x8c
30003ed2:	4616      	mov	r6, r2
30003ed4:	4607      	mov	r7, r0
30003ed6:	460d      	mov	r5, r1
30003ed8:	f89d 40ac 	ldrb.w	r4, [sp, #172]	; 0xac
30003edc:	4699      	mov	r9, r3
30003ede:	f10d 0808 	add.w	r8, sp, #8
30003ee2:	2280      	movs	r2, #128	; 0x80
30003ee4:	2c00      	cmp	r4, #0
30003ee6:	f000 80b2 	beq.w	3000404e <rewrite_bp+0x182>
30003eea:	4619      	mov	r1, r3
30003eec:	4640      	mov	r0, r8
30003eee:	f008 fa53 	bl	3000c398 <____wrap_memcpy_veneer>
30003ef2:	2301      	movs	r3, #1
30003ef4:	f109 0404 	add.w	r4, r9, #4
30003ef8:	4649      	mov	r1, r9
30003efa:	2002      	movs	r0, #2
30003efc:	f88d 7014 	strb.w	r7, [sp, #20]
30003f00:	e9cd 3303 	strd	r3, r3, [sp, #12]
30003f04:	f7ff feea 	bl	30003cdc <FLASH_Erase_With_Lock>
30003f08:	4620      	mov	r0, r4
30003f0a:	217c      	movs	r1, #124	; 0x7c
30003f0c:	aa03      	add	r2, sp, #12
30003f0e:	f7ff ff0d 	bl	30003d2c <FLASH_TxData_With_Lock>
30003f12:	4620      	mov	r0, r4
30003f14:	f509 5180 	add.w	r1, r9, #4096	; 0x1000
30003f18:	f04f 33ff 	mov.w	r3, #4294967295	; 0xffffffff
30003f1c:	4c54      	ldr	r4, [pc, #336]	; (30004070 <rewrite_bp+0x1a4>)
30003f1e:	f810 cb01 	ldrb.w	ip, [r0], #1
30003f22:	ea8c 0c03 	eor.w	ip, ip, r3
30003f26:	4288      	cmp	r0, r1
30003f28:	fa5f fc8c 	uxtb.w	ip, ip
30003f2c:	f854 202c 	ldr.w	r2, [r4, ip, lsl #2]
30003f30:	ea82 2313 	eor.w	r3, r2, r3, lsr #8
30003f34:	d1f3      	bne.n	30003f1e <rewrite_bp+0x52>
30003f36:	43db      	mvns	r3, r3
30003f38:	4649      	mov	r1, r9
30003f3a:	2002      	movs	r0, #2
30003f3c:	9302      	str	r3, [sp, #8]
30003f3e:	f7ff fecd 	bl	30003cdc <FLASH_Erase_With_Lock>
30003f42:	4642      	mov	r2, r8
30003f44:	4648      	mov	r0, r9
30003f46:	2180      	movs	r1, #128	; 0x80
30003f48:	f7ff fef0 	bl	30003d2c <FLASH_TxData_With_Lock>
30003f4c:	4b49      	ldr	r3, [pc, #292]	; (30004074 <rewrite_bp+0x1a8>)
30003f4e:	aa01      	add	r2, sp, #4
30003f50:	2104      	movs	r1, #4
30003f52:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30003f56:	4b48      	ldr	r3, [pc, #288]	; (30004078 <rewrite_bp+0x1ac>)
30003f58:	4798      	blx	r3
30003f5a:	f89d 3006 	ldrb.w	r3, [sp, #6]
30003f5e:	f04f 0c01 	mov.w	ip, #1
30003f62:	f8d5 e00a 	ldr.w	lr, [r5, #10]
30003f66:	682a      	ldr	r2, [r5, #0]
30003f68:	fa0c fc03 	lsl.w	ip, ip, r3
30003f6c:	f10e 0e0c 	add.w	lr, lr, #12
30003f70:	f8d6 300a 	ldr.w	r3, [r6, #10]
30003f74:	45e6      	cmp	lr, ip
30003f76:	6831      	ldr	r1, [r6, #0]
30003f78:	f103 030c 	add.w	r3, r3, #12
30003f7c:	bf28      	it	cs
30003f7e:	46e6      	movcs	lr, ip
30003f80:	459c      	cmp	ip, r3
30003f82:	bf28      	it	cs
30003f84:	469c      	movcs	ip, r3
30003f86:	bb6f      	cbnz	r7, 30003fe4 <rewrite_bp+0x118>
30003f88:	1d28      	adds	r0, r5, #4
30003f8a:	f1be 0f00 	cmp.w	lr, #0
30003f8e:	d00e      	beq.n	30003fae <rewrite_bp+0xe2>
30003f90:	4486      	add	lr, r0
30003f92:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003f96:	f810 3b01 	ldrb.w	r3, [r0], #1
30003f9a:	407b      	eors	r3, r7
30003f9c:	4570      	cmp	r0, lr
30003f9e:	b2db      	uxtb	r3, r3
30003fa0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003fa4:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003fa8:	d1f5      	bne.n	30003f96 <rewrite_bp+0xca>
30003faa:	ea6f 0e07 	mvn.w	lr, r7
30003fae:	4572      	cmp	r2, lr
30003fb0:	d042      	beq.n	30004038 <rewrite_bp+0x16c>
30003fb2:	1d32      	adds	r2, r6, #4
30003fb4:	f1bc 0f00 	cmp.w	ip, #0
30003fb8:	d00e      	beq.n	30003fd8 <rewrite_bp+0x10c>
30003fba:	4494      	add	ip, r2
30003fbc:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003fc0:	f812 3b01 	ldrb.w	r3, [r2], #1
30003fc4:	407b      	eors	r3, r7
30003fc6:	4562      	cmp	r2, ip
30003fc8:	b2db      	uxtb	r3, r3
30003fca:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30003fce:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30003fd2:	d1f5      	bne.n	30003fc0 <rewrite_bp+0xf4>
30003fd4:	ea6f 0c07 	mvn.w	ip, r7
30003fd8:	4561      	cmp	r1, ip
30003fda:	d03d      	beq.n	30004058 <rewrite_bp+0x18c>
30003fdc:	4827      	ldr	r0, [pc, #156]	; (3000407c <rewrite_bp+0x1b0>)
30003fde:	f008 f9fb 	bl	3000c3d8 <__DiagPrintf_veneer>
30003fe2:	e7fe      	b.n	30003fe2 <rewrite_bp+0x116>
30003fe4:	1d30      	adds	r0, r6, #4
30003fe6:	f1bc 0f00 	cmp.w	ip, #0
30003fea:	d00e      	beq.n	3000400a <rewrite_bp+0x13e>
30003fec:	4484      	add	ip, r0
30003fee:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
30003ff2:	f810 3b01 	ldrb.w	r3, [r0], #1
30003ff6:	407b      	eors	r3, r7
30003ff8:	4560      	cmp	r0, ip
30003ffa:	b2db      	uxtb	r3, r3
30003ffc:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004000:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
30004004:	d1f5      	bne.n	30003ff2 <rewrite_bp+0x126>
30004006:	ea6f 0c07 	mvn.w	ip, r7
3000400a:	4561      	cmp	r1, ip
3000400c:	d024      	beq.n	30004058 <rewrite_bp+0x18c>
3000400e:	1d29      	adds	r1, r5, #4
30004010:	f1be 0f00 	cmp.w	lr, #0
30004014:	d00e      	beq.n	30004034 <rewrite_bp+0x168>
30004016:	448e      	add	lr, r1
30004018:	f04f 37ff 	mov.w	r7, #4294967295	; 0xffffffff
3000401c:	f811 3b01 	ldrb.w	r3, [r1], #1
30004020:	407b      	eors	r3, r7
30004022:	4571      	cmp	r1, lr
30004024:	b2db      	uxtb	r3, r3
30004026:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000402a:	ea83 2717 	eor.w	r7, r3, r7, lsr #8
3000402e:	d1f5      	bne.n	3000401c <rewrite_bp+0x150>
30004030:	ea6f 0e07 	mvn.w	lr, r7
30004034:	4572      	cmp	r2, lr
30004036:	d1d1      	bne.n	30003fdc <rewrite_bp+0x110>
30004038:	f8c4 5400 	str.w	r5, [r4, #1024]	; 0x400
3000403c:	4810      	ldr	r0, [pc, #64]	; (30004080 <rewrite_bp+0x1b4>)
3000403e:	f8d5 1006 	ldr.w	r1, [r5, #6]
30004042:	f008 f9c9 	bl	3000c3d8 <__DiagPrintf_veneer>
30004046:	2001      	movs	r0, #1
30004048:	b023      	add	sp, #140	; 0x8c
3000404a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000404e:	992a      	ldr	r1, [sp, #168]	; 0xa8
30004050:	4640      	mov	r0, r8
30004052:	f008 f9a1 	bl	3000c398 <____wrap_memcpy_veneer>
30004056:	e74c      	b.n	30003ef2 <rewrite_bp+0x26>
30004058:	f8c4 6404 	str.w	r6, [r4, #1028]	; 0x404
3000405c:	4809      	ldr	r0, [pc, #36]	; (30004084 <rewrite_bp+0x1b8>)
3000405e:	f8d6 1006 	ldr.w	r1, [r6, #6]
30004062:	f008 f9b9 	bl	3000c3d8 <__DiagPrintf_veneer>
30004066:	2002      	movs	r0, #2
30004068:	b023      	add	sp, #140	; 0x8c
3000406a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000406e:	bf00      	nop
30004070:	3000e89c 	.word	0x3000e89c
30004074:	2001c01c 	.word	0x2001c01c
30004078:	00009db5 	.word	0x00009db5
3000407c:	3000c7a8 	.word	0x3000c7a8
30004080:	3000c778 	.word	0x3000c778
30004084:	3000c790 	.word	0x3000c790

30004088 <BOOT_SectionInit>:
30004088:	4800      	ldr	r0, [pc, #0]	; (3000408c <BOOT_SectionInit+0x4>)
3000408a:	4770      	bx	lr
3000408c:	60000020 	.word	0x60000020

30004090 <BOOT_NsStart>:
30004090:	b570      	push	{r4, r5, r6, lr}
30004092:	4c23      	ldr	r4, [pc, #140]	; (30004120 <BOOT_NsStart+0x90>)
30004094:	4605      	mov	r5, r0
30004096:	f384 8808 	msr	MSP, r4
3000409a:	f44f 5280 	mov.w	r2, #4096	; 0x1000
3000409e:	2100      	movs	r1, #0
300040a0:	4620      	mov	r0, r4
300040a2:	4e20      	ldr	r6, [pc, #128]	; (30004124 <BOOT_NsStart+0x94>)
300040a4:	47b0      	blx	r6
300040a6:	4a20      	ldr	r2, [pc, #128]	; (30004128 <BOOT_NsStart+0x98>)
300040a8:	6953      	ldr	r3, [r2, #20]
300040aa:	03d9      	lsls	r1, r3, #15
300040ac:	d50b      	bpl.n	300040c6 <BOOT_NsStart+0x36>
300040ae:	f3bf 8f4f 	dsb	sy
300040b2:	f8c2 4270 	str.w	r4, [r2, #624]	; 0x270
300040b6:	3420      	adds	r4, #32
300040b8:	f1b4 2f30 	cmp.w	r4, #805318656	; 0x30003000
300040bc:	d1f9      	bne.n	300040b2 <BOOT_NsStart+0x22>
300040be:	f3bf 8f4f 	dsb	sy
300040c2:	f3bf 8f6f 	isb	sy
300040c6:	f04f 2330 	mov.w	r3, #805318656	; 0x30003000
300040ca:	f383 8808 	msr	MSP, r3
300040ce:	f44f 5280 	mov.w	r2, #4096	; 0x1000
300040d2:	2100      	movs	r1, #0
300040d4:	4815      	ldr	r0, [pc, #84]	; (3000412c <BOOT_NsStart+0x9c>)
300040d6:	47b0      	blx	r6
300040d8:	4a13      	ldr	r2, [pc, #76]	; (30004128 <BOOT_NsStart+0x98>)
300040da:	6953      	ldr	r3, [r2, #20]
300040dc:	03db      	lsls	r3, r3, #15
300040de:	d50c      	bpl.n	300040fa <BOOT_NsStart+0x6a>
300040e0:	f3bf 8f4f 	dsb	sy
300040e4:	4b11      	ldr	r3, [pc, #68]	; (3000412c <BOOT_NsStart+0x9c>)
300040e6:	490e      	ldr	r1, [pc, #56]	; (30004120 <BOOT_NsStart+0x90>)
300040e8:	f8c2 3270 	str.w	r3, [r2, #624]	; 0x270
300040ec:	3320      	adds	r3, #32
300040ee:	428b      	cmp	r3, r1
300040f0:	d1fa      	bne.n	300040e8 <BOOT_NsStart+0x58>
300040f2:	f3bf 8f4f 	dsb	sy
300040f6:	f3bf 8f6f 	isb	sy
300040fa:	f025 0501 	bic.w	r5, r5, #1
300040fe:	086d      	lsrs	r5, r5, #1
30004100:	006d      	lsls	r5, r5, #1
30004102:	e92d 0ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
30004106:	ed2d 8b10 	vpush	{d8-d15}
3000410a:	ec9f 0a20 	vscclrm	{s0-s31, VPR}
3000410e:	e89f 9fdf 	clrm	{r0, r1, r2, r3, r4, r6, r7, r8, r9, sl, fp, ip, APSR}
30004112:	47ac      	blxns	r5
30004114:	ecbd 8b10 	vpop	{d8-d15}
30004118:	e8bd 0ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp}
3000411c:	bd70      	pop	{r4, r5, r6, pc}
3000411e:	bf00      	nop
30004120:	30002000 	.word	0x30002000
30004124:	00012be5 	.word	0x00012be5
30004128:	e000ed00 	.word	0xe000ed00
3000412c:	30001000 	.word	0x30001000

30004130 <BOOT_RccConfig>:
30004130:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30004134:	4c2f      	ldr	r4, [pc, #188]	; (300041f4 <BOOT_RccConfig+0xc4>)
30004136:	b090      	sub	sp, #64	; 0x40
30004138:	4f2f      	ldr	r7, [pc, #188]	; (300041f8 <BOOT_RccConfig+0xc8>)
3000413a:	466d      	mov	r5, sp
3000413c:	f10d 0c20 	add.w	ip, sp, #32
30004140:	683e      	ldr	r6, [r7, #0]
30004142:	f10d 0e30 	add.w	lr, sp, #48	; 0x30
30004146:	e894 000f 	ldmia.w	r4, {r0, r1, r2, r3}
3000414a:	e885 000f 	stmia.w	r5, {r0, r1, r2, r3}
3000414e:	f104 0310 	add.w	r3, r4, #16
30004152:	2400      	movs	r4, #0
30004154:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30004156:	e88c 000f 	stmia.w	ip, {r0, r1, r2, r3}
3000415a:	1c72      	adds	r2, r6, #1
3000415c:	e9cd 4404 	strd	r4, r4, [sp, #16]
30004160:	e9cd 4406 	strd	r4, r4, [sp, #24]
30004164:	e9cd 440c 	strd	r4, r4, [sp, #48]	; 0x30
30004168:	e9cd 440e 	strd	r4, r4, [sp, #56]	; 0x38
3000416c:	d03f      	beq.n	300041ee <BOOT_RccConfig+0xbe>
3000416e:	463a      	mov	r2, r7
30004170:	6891      	ldr	r1, [r2, #8]
30004172:	0fb3      	lsrs	r3, r6, #30
30004174:	a810      	add	r0, sp, #64	; 0x40
30004176:	2901      	cmp	r1, #1
30004178:	eb00 0383 	add.w	r3, r0, r3, lsl #2
3000417c:	d10e      	bne.n	3000419c <BOOT_RccConfig+0x6c>
3000417e:	f853 0c30 	ldr.w	r0, [r3, #-48]
30004182:	6854      	ldr	r4, [r2, #4]
30004184:	4330      	orrs	r0, r6
30004186:	ae10      	add	r6, sp, #64	; 0x40
30004188:	0fa1      	lsrs	r1, r4, #30
3000418a:	f843 0c30 	str.w	r0, [r3, #-48]
3000418e:	eb06 0181 	add.w	r1, r6, r1, lsl #2
30004192:	f851 3c10 	ldr.w	r3, [r1, #-16]
30004196:	4323      	orrs	r3, r4
30004198:	f841 3c10 	str.w	r3, [r1, #-16]
3000419c:	f852 6f0c 	ldr.w	r6, [r2, #12]!
300041a0:	1c73      	adds	r3, r6, #1
300041a2:	d1e5      	bne.n	30004170 <BOOT_RccConfig+0x40>
300041a4:	9b0c      	ldr	r3, [sp, #48]	; 0x30
300041a6:	9c04      	ldr	r4, [sp, #16]
300041a8:	af0d      	add	r7, sp, #52	; 0x34
300041aa:	ae05      	add	r6, sp, #20
300041ac:	4a13      	ldr	r2, [pc, #76]	; (300041fc <BOOT_RccConfig+0xcc>)
300041ae:	f85c 0b04 	ldr.w	r0, [ip], #4
300041b2:	f023 4340 	bic.w	r3, r3, #3221225472	; 0xc0000000
300041b6:	f855 1b04 	ldr.w	r1, [r5], #4
300041ba:	f024 4440 	bic.w	r4, r4, #3221225472	; 0xc0000000
300041be:	f850 8002 	ldr.w	r8, [r0, r2]
300041c2:	45e6      	cmp	lr, ip
300041c4:	ea43 0308 	orr.w	r3, r3, r8
300041c8:	5083      	str	r3, [r0, r2]
300041ca:	588b      	ldr	r3, [r1, r2]
300041cc:	ea44 0403 	orr.w	r4, r4, r3
300041d0:	508c      	str	r4, [r1, r2]
300041d2:	d004      	beq.n	300041de <BOOT_RccConfig+0xae>
300041d4:	f857 3b04 	ldr.w	r3, [r7], #4
300041d8:	f856 4b04 	ldr.w	r4, [r6], #4
300041dc:	e7e7      	b.n	300041ae <BOOT_RccConfig+0x7e>
300041de:	200a      	movs	r0, #10
300041e0:	4b07      	ldr	r3, [pc, #28]	; (30004200 <BOOT_RccConfig+0xd0>)
300041e2:	4798      	blx	r3
300041e4:	4b07      	ldr	r3, [pc, #28]	; (30004204 <BOOT_RccConfig+0xd4>)
300041e6:	b010      	add	sp, #64	; 0x40
300041e8:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300041ec:	4718      	bx	r3
300041ee:	4623      	mov	r3, r4
300041f0:	e7da      	b.n	300041a8 <BOOT_RccConfig+0x78>
300041f2:	bf00      	nop
300041f4:	3000e37c 	.word	0x3000e37c
300041f8:	3000e6e0 	.word	0x3000e6e0
300041fc:	42008000 	.word	0x42008000
30004200:	00009b2d 	.word	0x00009b2d
30004204:	30007e55 	.word	0x30007e55

30004208 <BOOT_CACHEWRR_Set>:
30004208:	4b14      	ldr	r3, [pc, #80]	; (3000425c <BOOT_CACHEWRR_Set+0x54>)
3000420a:	b430      	push	{r4, r5}
3000420c:	681a      	ldr	r2, [r3, #0]
3000420e:	2a01      	cmp	r2, #1
30004210:	d10f      	bne.n	30004232 <BOOT_CACHEWRR_Set+0x2a>
30004212:	f04f 20e0 	mov.w	r0, #3758153728	; 0xe000e000
30004216:	685d      	ldr	r5, [r3, #4]
30004218:	689c      	ldr	r4, [r3, #8]
3000421a:	6882      	ldr	r2, [r0, #8]
3000421c:	4910      	ldr	r1, [pc, #64]	; (30004260 <BOOT_CACHEWRR_Set+0x58>)
3000421e:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
30004222:	ea42 3205 	orr.w	r2, r2, r5, lsl #12
30004226:	6082      	str	r2, [r0, #8]
30004228:	f8c1 40a0 	str.w	r4, [r1, #160]	; 0xa0
3000422c:	68da      	ldr	r2, [r3, #12]
3000422e:	f8c1 20a4 	str.w	r2, [r1, #164]	; 0xa4
30004232:	691a      	ldr	r2, [r3, #16]
30004234:	2a01      	cmp	r2, #1
30004236:	d10f      	bne.n	30004258 <BOOT_CACHEWRR_Set+0x50>
30004238:	f04f 21e0 	mov.w	r1, #3758153728	; 0xe000e000
3000423c:	695d      	ldr	r5, [r3, #20]
3000423e:	4a08      	ldr	r2, [pc, #32]	; (30004260 <BOOT_CACHEWRR_Set+0x58>)
30004240:	e9d3 4006 	ldrd	r4, r0, [r3, #24]
30004244:	688b      	ldr	r3, [r1, #8]
30004246:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000424a:	ea43 5305 	orr.w	r3, r3, r5, lsl #20
3000424e:	608b      	str	r3, [r1, #8]
30004250:	f8c2 40a8 	str.w	r4, [r2, #168]	; 0xa8
30004254:	f8c2 00ac 	str.w	r0, [r2, #172]	; 0xac
30004258:	bc30      	pop	{r4, r5}
3000425a:	4770      	bx	lr
3000425c:	3000ef04 	.word	0x3000ef04
30004260:	e0042000 	.word	0xe0042000

30004264 <BOOT_TCMSet>:
30004264:	2300      	movs	r3, #0
30004266:	b5f0      	push	{r4, r5, r6, r7, lr}
30004268:	4db3      	ldr	r5, [pc, #716]	; (30004538 <BOOT_TCMSet+0x2d4>)
3000426a:	461c      	mov	r4, r3
3000426c:	f855 2023 	ldr.w	r2, [r5, r3, lsl #2]
30004270:	3307      	adds	r3, #7
30004272:	4282      	cmp	r2, r0
30004274:	d03c      	beq.n	300042f0 <BOOT_TCMSet+0x8c>
30004276:	3401      	adds	r4, #1
30004278:	2c07      	cmp	r4, #7
3000427a:	d1f7      	bne.n	3000426c <BOOT_TCMSet+0x8>
3000427c:	f3bf 8f4f 	dsb	sy
30004280:	f3bf 8f6f 	isb	sy
30004284:	48ad      	ldr	r0, [pc, #692]	; (3000453c <BOOT_TCMSet+0x2d8>)
30004286:	2200      	movs	r2, #0
30004288:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
3000428c:	f3bf 8f4f 	dsb	sy
30004290:	f3bf 8f6f 	isb	sy
30004294:	6943      	ldr	r3, [r0, #20]
30004296:	f443 3300 	orr.w	r3, r3, #131072	; 0x20000
3000429a:	6143      	str	r3, [r0, #20]
3000429c:	f3bf 8f4f 	dsb	sy
300042a0:	f3bf 8f6f 	isb	sy
300042a4:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
300042a8:	f3bf 8f4f 	dsb	sy
300042ac:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300042b0:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300042b4:	f3c5 344e 	ubfx	r4, r5, #13, #15
300042b8:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300042bc:	0164      	lsls	r4, r4, #5
300042be:	ea04 0106 	and.w	r1, r4, r6
300042c2:	462b      	mov	r3, r5
300042c4:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300042c8:	3b01      	subs	r3, #1
300042ca:	1c5f      	adds	r7, r3, #1
300042cc:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300042d0:	d1f8      	bne.n	300042c4 <BOOT_TCMSet+0x60>
300042d2:	3c20      	subs	r4, #32
300042d4:	f114 0f20 	cmn.w	r4, #32
300042d8:	d1f1      	bne.n	300042be <BOOT_TCMSet+0x5a>
300042da:	f3bf 8f4f 	dsb	sy
300042de:	6943      	ldr	r3, [r0, #20]
300042e0:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
300042e4:	6143      	str	r3, [r0, #20]
300042e6:	f3bf 8f4f 	dsb	sy
300042ea:	f3bf 8f6f 	isb	sy
300042ee:	bdf0      	pop	{r4, r5, r6, r7, pc}
300042f0:	2c00      	cmp	r4, #0
300042f2:	d0c3      	beq.n	3000427c <BOOT_TCMSet+0x18>
300042f4:	f3bf 8f4f 	dsb	sy
300042f8:	f3bf 8f6f 	isb	sy
300042fc:	4890      	ldr	r0, [pc, #576]	; (30004540 <BOOT_TCMSet+0x2dc>)
300042fe:	2300      	movs	r3, #0
30004300:	f8c0 3250 	str.w	r3, [r0, #592]	; 0x250
30004304:	f3bf 8f4f 	dsb	sy
30004308:	f3bf 8f6f 	isb	sy
3000430c:	6942      	ldr	r2, [r0, #20]
3000430e:	03d2      	lsls	r2, r2, #15
30004310:	d520      	bpl.n	30004354 <BOOT_TCMSet+0xf0>
30004312:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004316:	f3bf 8f4f 	dsb	sy
3000431a:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000431e:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004322:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
30004326:	f3c3 334e 	ubfx	r3, r3, #13, #15
3000432a:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000432e:	ea0c 0106 	and.w	r1, ip, r6
30004332:	4673      	mov	r3, lr
30004334:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004338:	3b01      	subs	r3, #1
3000433a:	1c5f      	adds	r7, r3, #1
3000433c:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30004340:	d1f8      	bne.n	30004334 <BOOT_TCMSet+0xd0>
30004342:	f1ac 0c20 	sub.w	ip, ip, #32
30004346:	f11c 0f20 	cmn.w	ip, #32
3000434a:	d1f0      	bne.n	3000432e <BOOT_TCMSet+0xca>
3000434c:	f3bf 8f4f 	dsb	sy
30004350:	f3bf 8f6f 	isb	sy
30004354:	f3bf 8f4f 	dsb	sy
30004358:	f3bf 8f6f 	isb	sy
3000435c:	4878      	ldr	r0, [pc, #480]	; (30004540 <BOOT_TCMSet+0x2dc>)
3000435e:	2200      	movs	r2, #0
30004360:	6943      	ldr	r3, [r0, #20]
30004362:	f423 3300 	bic.w	r3, r3, #131072	; 0x20000
30004366:	6143      	str	r3, [r0, #20]
30004368:	f8c0 2250 	str.w	r2, [r0, #592]	; 0x250
3000436c:	f3bf 8f4f 	dsb	sy
30004370:	f3bf 8f6f 	isb	sy
30004374:	f8c0 2084 	str.w	r2, [r0, #132]	; 0x84
30004378:	f3bf 8f4f 	dsb	sy
3000437c:	6943      	ldr	r3, [r0, #20]
3000437e:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
30004382:	6143      	str	r3, [r0, #20]
30004384:	f3bf 8f4f 	dsb	sy
30004388:	f8d0 3080 	ldr.w	r3, [r0, #128]	; 0x80
3000438c:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30004390:	f3c3 06c9 	ubfx	r6, r3, #3, #10
30004394:	f3c3 334e 	ubfx	r3, r3, #13, #15
30004398:	ea4f 1c43 	mov.w	ip, r3, lsl #5
3000439c:	ea0c 010e 	and.w	r1, ip, lr
300043a0:	4633      	mov	r3, r6
300043a2:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300043a6:	3b01      	subs	r3, #1
300043a8:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300043ac:	1c5a      	adds	r2, r3, #1
300043ae:	d1f8      	bne.n	300043a2 <BOOT_TCMSet+0x13e>
300043b0:	f1ac 0c20 	sub.w	ip, ip, #32
300043b4:	f11c 0f20 	cmn.w	ip, #32
300043b8:	d1f0      	bne.n	3000439c <BOOT_TCMSet+0x138>
300043ba:	f3bf 8f4f 	dsb	sy
300043be:	f3bf 8f6f 	isb	sy
300043c2:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300043c6:	ebc4 02c4 	rsb	r2, r4, r4, lsl #3
300043ca:	00e3      	lsls	r3, r4, #3
300043cc:	6b41      	ldr	r1, [r0, #52]	; 0x34
300043ce:	eb05 0282 	add.w	r2, r5, r2, lsl #2
300043d2:	f441 5180 	orr.w	r1, r1, #4096	; 0x1000
300043d6:	6341      	str	r1, [r0, #52]	; 0x34
300043d8:	6911      	ldr	r1, [r2, #16]
300043da:	2903      	cmp	r1, #3
300043dc:	d00f      	beq.n	300043fe <BOOT_TCMSet+0x19a>
300043de:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
300043e2:	4858      	ldr	r0, [pc, #352]	; (30004544 <BOOT_TCMSet+0x2e0>)
300043e4:	f04f 0c00 	mov.w	ip, #0
300043e8:	4f57      	ldr	r7, [pc, #348]	; (30004548 <BOOT_TCMSet+0x2e4>)
300043ea:	68b2      	ldr	r2, [r6, #8]
300043ec:	f422 1240 	bic.w	r2, r2, #3145728	; 0x300000
300043f0:	ea42 5201 	orr.w	r2, r2, r1, lsl #20
300043f4:	60b2      	str	r2, [r6, #8]
300043f6:	f8c0 c0a8 	str.w	ip, [r0, #168]	; 0xa8
300043fa:	f8c0 70ac 	str.w	r7, [r0, #172]	; 0xac
300043fe:	1b1a      	subs	r2, r3, r4
30004400:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004404:	68d1      	ldr	r1, [r2, #12]
30004406:	2903      	cmp	r1, #3
30004408:	d00f      	beq.n	3000442a <BOOT_TCMSet+0x1c6>
3000440a:	f04f 26e0 	mov.w	r6, #3758153728	; 0xe000e000
3000440e:	484d      	ldr	r0, [pc, #308]	; (30004544 <BOOT_TCMSet+0x2e0>)
30004410:	f04f 0c00 	mov.w	ip, #0
30004414:	4f4c      	ldr	r7, [pc, #304]	; (30004548 <BOOT_TCMSet+0x2e4>)
30004416:	68b2      	ldr	r2, [r6, #8]
30004418:	f422 5240 	bic.w	r2, r2, #12288	; 0x3000
3000441c:	ea42 3201 	orr.w	r2, r2, r1, lsl #12
30004420:	60b2      	str	r2, [r6, #8]
30004422:	f8c0 c0a0 	str.w	ip, [r0, #160]	; 0xa0
30004426:	f8c0 70a4 	str.w	r7, [r0, #164]	; 0xa4
3000442a:	1b1a      	subs	r2, r3, r4
3000442c:	4945      	ldr	r1, [pc, #276]	; (30004544 <BOOT_TCMSet+0x2e0>)
3000442e:	eb05 0282 	add.w	r2, r5, r2, lsl #2
30004432:	6950      	ldr	r0, [r2, #20]
30004434:	6608      	str	r0, [r1, #96]	; 0x60
30004436:	6990      	ldr	r0, [r2, #24]
30004438:	6648      	str	r0, [r1, #100]	; 0x64
3000443a:	6852      	ldr	r2, [r2, #4]
3000443c:	2a01      	cmp	r2, #1
3000443e:	d04b      	beq.n	300044d8 <BOOT_TCMSet+0x274>
30004440:	1b1b      	subs	r3, r3, r4
30004442:	eb05 0583 	add.w	r5, r5, r3, lsl #2
30004446:	68ab      	ldr	r3, [r5, #8]
30004448:	2b01      	cmp	r3, #1
3000444a:	f47f af50 	bne.w	300042ee <BOOT_TCMSet+0x8a>
3000444e:	483b      	ldr	r0, [pc, #236]	; (3000453c <BOOT_TCMSet+0x2d8>)
30004450:	2300      	movs	r3, #0
30004452:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30004456:	f3bf 8f4f 	dsb	sy
3000445a:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000445e:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30004462:	f3c5 344e 	ubfx	r4, r5, #13, #15
30004466:	f3c5 05c9 	ubfx	r5, r5, #3, #10
3000446a:	0164      	lsls	r4, r4, #5
3000446c:	ea04 0106 	and.w	r1, r4, r6
30004470:	462b      	mov	r3, r5
30004472:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30004476:	3b01      	subs	r3, #1
30004478:	1c5f      	adds	r7, r3, #1
3000447a:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
3000447e:	d1f8      	bne.n	30004472 <BOOT_TCMSet+0x20e>
30004480:	3c20      	subs	r4, #32
30004482:	f114 0f20 	cmn.w	r4, #32
30004486:	d1f1      	bne.n	3000446c <BOOT_TCMSet+0x208>
30004488:	f3bf 8f4f 	dsb	sy
3000448c:	6943      	ldr	r3, [r0, #20]
3000448e:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
30004492:	6143      	str	r3, [r0, #20]
30004494:	f3bf 8f4f 	dsb	sy
30004498:	f3bf 8f6f 	isb	sy
3000449c:	4828      	ldr	r0, [pc, #160]	; (30004540 <BOOT_TCMSet+0x2dc>)
3000449e:	2300      	movs	r3, #0
300044a0:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300044a4:	f3bf 8f4f 	dsb	sy
300044a8:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300044ac:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300044b0:	f3c5 344e 	ubfx	r4, r5, #13, #15
300044b4:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300044b8:	0164      	lsls	r4, r4, #5
300044ba:	ea04 0106 	and.w	r1, r4, r6
300044be:	462b      	mov	r3, r5
300044c0:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300044c4:	3b01      	subs	r3, #1
300044c6:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
300044ca:	1c5a      	adds	r2, r3, #1
300044cc:	d1f8      	bne.n	300044c0 <BOOT_TCMSet+0x25c>
300044ce:	3c20      	subs	r4, #32
300044d0:	f114 0f20 	cmn.w	r4, #32
300044d4:	d1f1      	bne.n	300044ba <BOOT_TCMSet+0x256>
300044d6:	e700      	b.n	300042da <BOOT_TCMSet+0x76>
300044d8:	f3bf 8f4f 	dsb	sy
300044dc:	f3bf 8f6f 	isb	sy
300044e0:	4a16      	ldr	r2, [pc, #88]	; (3000453c <BOOT_TCMSet+0x2d8>)
300044e2:	2000      	movs	r0, #0
300044e4:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
300044e8:	f3bf 8f4f 	dsb	sy
300044ec:	f3bf 8f6f 	isb	sy
300044f0:	6951      	ldr	r1, [r2, #20]
300044f2:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
300044f6:	6151      	str	r1, [r2, #20]
300044f8:	f3bf 8f4f 	dsb	sy
300044fc:	f3bf 8f6f 	isb	sy
30004500:	f3bf 8f4f 	dsb	sy
30004504:	f3bf 8f6f 	isb	sy
30004508:	f5a2 3200 	sub.w	r2, r2, #131072	; 0x20000
3000450c:	f8c2 0250 	str.w	r0, [r2, #592]	; 0x250
30004510:	f3bf 8f4f 	dsb	sy
30004514:	f3bf 8f6f 	isb	sy
30004518:	6951      	ldr	r1, [r2, #20]
3000451a:	f441 3100 	orr.w	r1, r1, #131072	; 0x20000
3000451e:	6151      	str	r1, [r2, #20]
30004520:	f3bf 8f4f 	dsb	sy
30004524:	f3bf 8f6f 	isb	sy
30004528:	1b1b      	subs	r3, r3, r4
3000452a:	eb05 0583 	add.w	r5, r5, r3, lsl #2
3000452e:	68ab      	ldr	r3, [r5, #8]
30004530:	2b01      	cmp	r3, #1
30004532:	f47f aedc 	bne.w	300042ee <BOOT_TCMSet+0x8a>
30004536:	e78a      	b.n	3000444e <BOOT_TCMSet+0x1ea>
30004538:	3000ee40 	.word	0x3000ee40
3000453c:	e002ed00 	.word	0xe002ed00
30004540:	e000ed00 	.word	0xe000ed00
30004544:	e0042000 	.word	0xe0042000
30004548:	ffff0000 	.word	0xffff0000

3000454c <BOOT_GRstConfig>:
3000454c:	b538      	push	{r3, r4, r5, lr}
3000454e:	4c09      	ldr	r4, [pc, #36]	; (30004574 <BOOT_GRstConfig+0x28>)
30004550:	f241 050a 	movw	r5, #4106	; 0x100a
30004554:	2028      	movs	r0, #40	; 0x28
30004556:	4a08      	ldr	r2, [pc, #32]	; (30004578 <BOOT_GRstConfig+0x2c>)
30004558:	8823      	ldrh	r3, [r4, #0]
3000455a:	b29b      	uxth	r3, r3
3000455c:	431d      	orrs	r5, r3
3000455e:	8025      	strh	r5, [r4, #0]
30004560:	4790      	blx	r2
30004562:	88a3      	ldrh	r3, [r4, #4]
30004564:	b29b      	uxth	r3, r3
30004566:	ea23 0305 	bic.w	r3, r3, r5
3000456a:	80a3      	strh	r3, [r4, #4]
3000456c:	8863      	ldrh	r3, [r4, #2]
3000456e:	8065      	strh	r5, [r4, #2]
30004570:	bd38      	pop	{r3, r4, r5, pc}
30004572:	bf00      	nop
30004574:	42008200 	.word	0x42008200
30004578:	00009b2d 	.word	0x00009b2d

3000457c <BOOT_DDR_LCDC_HPR>:
3000457c:	4a04      	ldr	r2, [pc, #16]	; (30004590 <BOOT_DDR_LCDC_HPR+0x14>)
3000457e:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
30004582:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30004586:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
3000458a:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
3000458e:	4770      	bx	lr
30004590:	42008000 	.word	0x42008000

30004594 <BOOT_DDR_Init>:
30004594:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30004598:	b530      	push	{r4, r5, lr}
3000459a:	4d15      	ldr	r5, [pc, #84]	; (300045f0 <BOOT_DDR_Init+0x5c>)
3000459c:	b099      	sub	sp, #100	; 0x64
3000459e:	6b1c      	ldr	r4, [r3, #48]	; 0x30
300045a0:	47a8      	blx	r5
300045a2:	2803      	cmp	r0, #3
300045a4:	d11e      	bne.n	300045e4 <BOOT_DDR_Init+0x50>
300045a6:	f044 74c0 	orr.w	r4, r4, #25165824	; 0x1800000
300045aa:	4a12      	ldr	r2, [pc, #72]	; (300045f4 <BOOT_DDR_Init+0x60>)
300045ac:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300045b0:	4911      	ldr	r1, [pc, #68]	; (300045f8 <BOOT_DDR_Init+0x64>)
300045b2:	a801      	add	r0, sp, #4
300045b4:	631c      	str	r4, [r3, #48]	; 0x30
300045b6:	f8d2 3204 	ldr.w	r3, [r2, #516]	; 0x204
300045ba:	f423 5380 	bic.w	r3, r3, #4096	; 0x1000
300045be:	f8c2 3204 	str.w	r3, [r2, #516]	; 0x204
300045c2:	4788      	blx	r1
300045c4:	a801      	add	r0, sp, #4
300045c6:	4b0d      	ldr	r3, [pc, #52]	; (300045fc <BOOT_DDR_Init+0x68>)
300045c8:	4798      	blx	r3
300045ca:	4b0d      	ldr	r3, [pc, #52]	; (30004600 <BOOT_DDR_Init+0x6c>)
300045cc:	4798      	blx	r3
300045ce:	4b0d      	ldr	r3, [pc, #52]	; (30004604 <BOOT_DDR_Init+0x70>)
300045d0:	4798      	blx	r3
300045d2:	4b0d      	ldr	r3, [pc, #52]	; (30004608 <BOOT_DDR_Init+0x74>)
300045d4:	4798      	blx	r3
300045d6:	2101      	movs	r1, #1
300045d8:	f44f 60e0 	mov.w	r0, #1792	; 0x700
300045dc:	4b0b      	ldr	r3, [pc, #44]	; (3000460c <BOOT_DDR_Init+0x78>)
300045de:	4798      	blx	r3
300045e0:	b019      	add	sp, #100	; 0x64
300045e2:	bd30      	pop	{r4, r5, pc}
300045e4:	47a8      	blx	r5
300045e6:	2802      	cmp	r0, #2
300045e8:	d0dd      	beq.n	300045a6 <BOOT_DDR_Init+0x12>
300045ea:	f044 7480 	orr.w	r4, r4, #16777216	; 0x1000000
300045ee:	e7dc      	b.n	300045aa <BOOT_DDR_Init+0x16>
300045f0:	0000c0f9 	.word	0x0000c0f9
300045f4:	42008000 	.word	0x42008000
300045f8:	30007f71 	.word	0x30007f71
300045fc:	300084e9 	.word	0x300084e9
30004600:	3000b31d 	.word	0x3000b31d
30004604:	300085b9 	.word	0x300085b9
30004608:	300085bd 	.word	0x300085bd
3000460c:	3000b27d 	.word	0x3000b27d

30004610 <BOOT_PSRAM_Init>:
30004610:	b500      	push	{lr}
30004612:	b089      	sub	sp, #36	; 0x24
30004614:	4b1b      	ldr	r3, [pc, #108]	; (30004684 <BOOT_PSRAM_Init+0x74>)
30004616:	a801      	add	r0, sp, #4
30004618:	4798      	blx	r3
3000461a:	a801      	add	r0, sp, #4
3000461c:	4b1a      	ldr	r3, [pc, #104]	; (30004688 <BOOT_PSRAM_Init+0x78>)
3000461e:	4798      	blx	r3
30004620:	4b1a      	ldr	r3, [pc, #104]	; (3000468c <BOOT_PSRAM_Init+0x7c>)
30004622:	4798      	blx	r3
30004624:	4b1a      	ldr	r3, [pc, #104]	; (30004690 <BOOT_PSRAM_Init+0x80>)
30004626:	4798      	blx	r3
30004628:	2801      	cmp	r0, #1
3000462a:	d01d      	beq.n	30004668 <BOOT_PSRAM_Init+0x58>
3000462c:	4b19      	ldr	r3, [pc, #100]	; (30004694 <BOOT_PSRAM_Init+0x84>)
3000462e:	2004      	movs	r0, #4
30004630:	4a19      	ldr	r2, [pc, #100]	; (30004698 <BOOT_PSRAM_Init+0x88>)
30004632:	4619      	mov	r1, r3
30004634:	f007 fc5a 	bl	3000beec <rtk_log_write>
30004638:	4b18      	ldr	r3, [pc, #96]	; (3000469c <BOOT_PSRAM_Init+0x8c>)
3000463a:	4798      	blx	r3
3000463c:	4b18      	ldr	r3, [pc, #96]	; (300046a0 <BOOT_PSRAM_Init+0x90>)
3000463e:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004642:	009b      	lsls	r3, r3, #2
30004644:	d517      	bpl.n	30004676 <BOOT_PSRAM_Init+0x66>
30004646:	4a17      	ldr	r2, [pc, #92]	; (300046a4 <BOOT_PSRAM_Init+0x94>)
30004648:	4b17      	ldr	r3, [pc, #92]	; (300046a8 <BOOT_PSRAM_Init+0x98>)
3000464a:	6812      	ldr	r2, [r2, #0]
3000464c:	605a      	str	r2, [r3, #4]
3000464e:	4a17      	ldr	r2, [pc, #92]	; (300046ac <BOOT_PSRAM_Init+0x9c>)
30004650:	f04f 0c20 	mov.w	ip, #32
30004654:	2101      	movs	r1, #1
30004656:	4b16      	ldr	r3, [pc, #88]	; (300046b0 <BOOT_PSRAM_Init+0xa0>)
30004658:	6952      	ldr	r2, [r2, #20]
3000465a:	4608      	mov	r0, r1
3000465c:	fbbc f2f2 	udiv	r2, ip, r2
30004660:	4798      	blx	r3
30004662:	b009      	add	sp, #36	; 0x24
30004664:	f85d fb04 	ldr.w	pc, [sp], #4
30004668:	4b12      	ldr	r3, [pc, #72]	; (300046b4 <BOOT_PSRAM_Init+0xa4>)
3000466a:	4798      	blx	r3
3000466c:	4b0c      	ldr	r3, [pc, #48]	; (300046a0 <BOOT_PSRAM_Init+0x90>)
3000466e:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
30004672:	009b      	lsls	r3, r3, #2
30004674:	d4e7      	bmi.n	30004646 <BOOT_PSRAM_Init+0x36>
30004676:	4b10      	ldr	r3, [pc, #64]	; (300046b8 <BOOT_PSRAM_Init+0xa8>)
30004678:	4798      	blx	r3
3000467a:	4a0b      	ldr	r2, [pc, #44]	; (300046a8 <BOOT_PSRAM_Init+0x98>)
3000467c:	4b09      	ldr	r3, [pc, #36]	; (300046a4 <BOOT_PSRAM_Init+0x94>)
3000467e:	6852      	ldr	r2, [r2, #4]
30004680:	601a      	str	r2, [r3, #0]
30004682:	e7e4      	b.n	3000464e <BOOT_PSRAM_Init+0x3e>
30004684:	30008d09 	.word	0x30008d09
30004688:	30008d29 	.word	0x30008d29
3000468c:	30008d91 	.word	0x30008d91
30004690:	30007e09 	.word	0x30007e09
30004694:	3000c6b0 	.word	0x3000c6b0
30004698:	3000c7c8 	.word	0x3000c7c8
3000469c:	30009035 	.word	0x30009035
300046a0:	42008000 	.word	0x42008000
300046a4:	3000fd68 	.word	0x3000fd68
300046a8:	41002000 	.word	0x41002000
300046ac:	3000fc60 	.word	0x3000fc60
300046b0:	30009315 	.word	0x30009315
300046b4:	30008f5d 	.word	0x30008f5d
300046b8:	30009071 	.word	0x30009071

300046bc <BOOT_WakeFromPG>:
300046bc:	b538      	push	{r3, r4, r5, lr}
300046be:	4c46      	ldr	r4, [pc, #280]	; (300047d8 <BOOT_WakeFromPG+0x11c>)
300046c0:	f002 ff54 	bl	3000756c <BOOT_RAM_TZCfg>
300046c4:	69a3      	ldr	r3, [r4, #24]
300046c6:	b10b      	cbz	r3, 300046cc <BOOT_WakeFromPG+0x10>
300046c8:	4a44      	ldr	r2, [pc, #272]	; (300047dc <BOOT_WakeFromPG+0x120>)
300046ca:	6093      	str	r3, [r2, #8]
300046cc:	4b44      	ldr	r3, [pc, #272]	; (300047e0 <BOOT_WakeFromPG+0x124>)
300046ce:	f06f 0c1a 	mvn.w	ip, #26
300046d2:	4844      	ldr	r0, [pc, #272]	; (300047e4 <BOOT_WakeFromPG+0x128>)
300046d4:	f103 0e50 	add.w	lr, r3, #80	; 0x50
300046d8:	ebac 0c04 	sub.w	ip, ip, r4
300046dc:	eb0c 0203 	add.w	r2, ip, r3
300046e0:	f813 1f01 	ldrb.w	r1, [r3, #1]!
300046e4:	4402      	add	r2, r0
300046e6:	4573      	cmp	r3, lr
300046e8:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
300046ec:	d1f6      	bne.n	300046dc <BOOT_WakeFromPG+0x20>
300046ee:	6822      	ldr	r2, [r4, #0]
300046f0:	4b3a      	ldr	r3, [pc, #232]	; (300047dc <BOOT_WakeFromPG+0x120>)
300046f2:	f422 7280 	bic.w	r2, r2, #256	; 0x100
300046f6:	e9d4 5101 	ldrd	r5, r1, [r4, #4]
300046fa:	6002      	str	r2, [r0, #0]
300046fc:	6045      	str	r5, [r0, #4]
300046fe:	6081      	str	r1, [r0, #8]
30004700:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004702:	4a39      	ldr	r2, [pc, #228]	; (300047e8 <BOOT_WakeFromPG+0x12c>)
30004704:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
30004708:	4d38      	ldr	r5, [pc, #224]	; (300047ec <BOOT_WakeFromPG+0x130>)
3000470a:	6259      	str	r1, [r3, #36]	; 0x24
3000470c:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000470e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004712:	6251      	str	r1, [r2, #36]	; 0x24
30004714:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30004718:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000471c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004720:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004724:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004728:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000472c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004730:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004734:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30004738:	f7ff fd66 	bl	30004208 <BOOT_CACHEWRR_Set>
3000473c:	4b2c      	ldr	r3, [pc, #176]	; (300047f0 <BOOT_WakeFromPG+0x134>)
3000473e:	6818      	ldr	r0, [r3, #0]
30004740:	f7ff fd90 	bl	30004264 <BOOT_TCMSet>
30004744:	2201      	movs	r2, #1
30004746:	492b      	ldr	r1, [pc, #172]	; (300047f4 <BOOT_WakeFromPG+0x138>)
30004748:	f04f 2040 	mov.w	r0, #1073758208	; 0x40004000
3000474c:	47a8      	blx	r5
3000474e:	4b2a      	ldr	r3, [pc, #168]	; (300047f8 <BOOT_WakeFromPG+0x13c>)
30004750:	4798      	blx	r3
30004752:	4b2a      	ldr	r3, [pc, #168]	; (300047fc <BOOT_WakeFromPG+0x140>)
30004754:	4798      	blx	r3
30004756:	2801      	cmp	r0, #1
30004758:	4604      	mov	r4, r0
3000475a:	d026      	beq.n	300047aa <BOOT_WakeFromPG+0xee>
3000475c:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30004760:	2201      	movs	r2, #1
30004762:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30004766:	47a8      	blx	r5
30004768:	4a25      	ldr	r2, [pc, #148]	; (30004800 <BOOT_WakeFromPG+0x144>)
3000476a:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
3000476e:	f043 0302 	orr.w	r3, r3, #2
30004772:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30004776:	f7ff ff4b 	bl	30004610 <BOOT_PSRAM_Init>
3000477a:	2096      	movs	r0, #150	; 0x96
3000477c:	4b21      	ldr	r3, [pc, #132]	; (30004804 <BOOT_WakeFromPG+0x148>)
3000477e:	4798      	blx	r3
30004780:	4a21      	ldr	r2, [pc, #132]	; (30004808 <BOOT_WakeFromPG+0x14c>)
30004782:	4c19      	ldr	r4, [pc, #100]	; (300047e8 <BOOT_WakeFromPG+0x12c>)
30004784:	6893      	ldr	r3, [r2, #8]
30004786:	4921      	ldr	r1, [pc, #132]	; (3000480c <BOOT_WakeFromPG+0x150>)
30004788:	6852      	ldr	r2, [r2, #4]
3000478a:	4821      	ldr	r0, [pc, #132]	; (30004810 <BOOT_WakeFromPG+0x154>)
3000478c:	605a      	str	r2, [r3, #4]
3000478e:	60a3      	str	r3, [r4, #8]
30004790:	4a20      	ldr	r2, [pc, #128]	; (30004814 <BOOT_WakeFromPG+0x158>)
30004792:	61c8      	str	r0, [r1, #28]
30004794:	f382 8888 	msr	MSP_NS, r2
30004798:	f5a2 6200 	sub.w	r2, r2, #2048	; 0x800
3000479c:	f382 8809 	msr	PSP, r2
300047a0:	6858      	ldr	r0, [r3, #4]
300047a2:	e8bd 4038 	ldmia.w	sp!, {r3, r4, r5, lr}
300047a6:	f7ff bc73 	b.w	30004090 <BOOT_NsStart>
300047aa:	4602      	mov	r2, r0
300047ac:	491a      	ldr	r1, [pc, #104]	; (30004818 <BOOT_WakeFromPG+0x15c>)
300047ae:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
300047b2:	47a8      	blx	r5
300047b4:	4622      	mov	r2, r4
300047b6:	4919      	ldr	r1, [pc, #100]	; (3000481c <BOOT_WakeFromPG+0x160>)
300047b8:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
300047bc:	47a8      	blx	r5
300047be:	f7ff fee9 	bl	30004594 <BOOT_DDR_Init>
300047c2:	4a0f      	ldr	r2, [pc, #60]	; (30004800 <BOOT_WakeFromPG+0x144>)
300047c4:	f8d2 3290 	ldr.w	r3, [r2, #656]	; 0x290
300047c8:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
300047cc:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
300047d0:	f8c2 3290 	str.w	r3, [r2, #656]	; 0x290
300047d4:	e7d4      	b.n	30004780 <BOOT_WakeFromPG+0xc4>
300047d6:	bf00      	nop
300047d8:	3000fcfc 	.word	0x3000fcfc
300047dc:	e000ed00 	.word	0xe000ed00
300047e0:	3000fd17 	.word	0x3000fd17
300047e4:	e000e100 	.word	0xe000e100
300047e8:	e002ed00 	.word	0xe002ed00
300047ec:	0000b479 	.word	0x0000b479
300047f0:	3000e6d8 	.word	0x3000e6d8
300047f4:	40001000 	.word	0x40001000
300047f8:	0000d835 	.word	0x0000d835
300047fc:	30007d31 	.word	0x30007d31
30004800:	42008000 	.word	0x42008000
30004804:	00009b2d 	.word	0x00009b2d
30004808:	60000020 	.word	0x60000020
3000480c:	30000000 	.word	0x30000000
30004810:	30003a81 	.word	0x30003a81
30004814:	2001bffc 	.word	0x2001bffc
30004818:	40080000 	.word	0x40080000
3000481c:	40040000 	.word	0x40040000

30004820 <BOOT_SCBConfig_HP>:
30004820:	4b0e      	ldr	r3, [pc, #56]	; (3000485c <BOOT_SCBConfig_HP+0x3c>)
30004822:	4a0f      	ldr	r2, [pc, #60]	; (30004860 <BOOT_SCBConfig_HP+0x40>)
30004824:	6a59      	ldr	r1, [r3, #36]	; 0x24
30004826:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000482a:	6259      	str	r1, [r3, #36]	; 0x24
3000482c:	6a51      	ldr	r1, [r2, #36]	; 0x24
3000482e:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30004832:	6251      	str	r1, [r2, #36]	; 0x24
30004834:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
30004838:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000483c:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30004840:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30004844:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30004848:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
3000484c:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
30004850:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
30004854:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
30004858:	4770      	bx	lr
3000485a:	bf00      	nop
3000485c:	e000ed00 	.word	0xe000ed00
30004860:	e002ed00 	.word	0xe002ed00

30004864 <BOOT_SCBVTORBackup_HP>:
30004864:	4a02      	ldr	r2, [pc, #8]	; (30004870 <BOOT_SCBVTORBackup_HP+0xc>)
30004866:	4b03      	ldr	r3, [pc, #12]	; (30004874 <BOOT_SCBVTORBackup_HP+0x10>)
30004868:	6892      	ldr	r2, [r2, #8]
3000486a:	619a      	str	r2, [r3, #24]
3000486c:	4770      	bx	lr
3000486e:	bf00      	nop
30004870:	e000ed00 	.word	0xe000ed00
30004874:	3000fcfc 	.word	0x3000fcfc

30004878 <BOOT_SCBVTORReFill_HP>:
30004878:	4a02      	ldr	r2, [pc, #8]	; (30004884 <BOOT_SCBVTORReFill_HP+0xc>)
3000487a:	4b03      	ldr	r3, [pc, #12]	; (30004888 <BOOT_SCBVTORReFill_HP+0x10>)
3000487c:	6992      	ldr	r2, [r2, #24]
3000487e:	609a      	str	r2, [r3, #8]
30004880:	4770      	bx	lr
30004882:	bf00      	nop
30004884:	3000fcfc 	.word	0x3000fcfc
30004888:	e000ed00 	.word	0xe000ed00

3000488c <BOOT_NVICBackup_HP>:
3000488c:	491a      	ldr	r1, [pc, #104]	; (300048f8 <BOOT_NVICBackup_HP+0x6c>)
3000488e:	f06f 001a 	mvn.w	r0, #26
30004892:	b410      	push	{r4}
30004894:	4c19      	ldr	r4, [pc, #100]	; (300048fc <BOOT_NVICBackup_HP+0x70>)
30004896:	680a      	ldr	r2, [r1, #0]
30004898:	f104 031b 	add.w	r3, r4, #27
3000489c:	1b00      	subs	r0, r0, r4
3000489e:	6022      	str	r2, [r4, #0]
300048a0:	f104 0c6b 	add.w	ip, r4, #107	; 0x6b
300048a4:	684a      	ldr	r2, [r1, #4]
300048a6:	6062      	str	r2, [r4, #4]
300048a8:	688a      	ldr	r2, [r1, #8]
300048aa:	60a2      	str	r2, [r4, #8]
300048ac:	18c2      	adds	r2, r0, r3
300048ae:	440a      	add	r2, r1
300048b0:	f892 2300 	ldrb.w	r2, [r2, #768]	; 0x300
300048b4:	f803 2f01 	strb.w	r2, [r3, #1]!
300048b8:	4563      	cmp	r3, ip
300048ba:	d1f7      	bne.n	300048ac <BOOT_NVICBackup_HP+0x20>
300048bc:	4b10      	ldr	r3, [pc, #64]	; (30004900 <BOOT_NVICBackup_HP+0x74>)
300048be:	695b      	ldr	r3, [r3, #20]
300048c0:	03db      	lsls	r3, r3, #15
300048c2:	d512      	bpl.n	300048ea <BOOT_NVICBackup_HP+0x5e>
300048c4:	f014 011f 	ands.w	r1, r4, #31
300048c8:	4b0c      	ldr	r3, [pc, #48]	; (300048fc <BOOT_NVICBackup_HP+0x70>)
300048ca:	d111      	bne.n	300048f0 <BOOT_NVICBackup_HP+0x64>
300048cc:	216c      	movs	r1, #108	; 0x6c
300048ce:	f3bf 8f4f 	dsb	sy
300048d2:	480b      	ldr	r0, [pc, #44]	; (30004900 <BOOT_NVICBackup_HP+0x74>)
300048d4:	4419      	add	r1, r3
300048d6:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
300048da:	3320      	adds	r3, #32
300048dc:	1aca      	subs	r2, r1, r3
300048de:	2a00      	cmp	r2, #0
300048e0:	dcf9      	bgt.n	300048d6 <BOOT_NVICBackup_HP+0x4a>
300048e2:	f3bf 8f4f 	dsb	sy
300048e6:	f3bf 8f6f 	isb	sy
300048ea:	f85d 4b04 	ldr.w	r4, [sp], #4
300048ee:	4770      	bx	lr
300048f0:	f024 031f 	bic.w	r3, r4, #31
300048f4:	316c      	adds	r1, #108	; 0x6c
300048f6:	e7ea      	b.n	300048ce <BOOT_NVICBackup_HP+0x42>
300048f8:	e000e100 	.word	0xe000e100
300048fc:	3000fcfc 	.word	0x3000fcfc
30004900:	e000ed00 	.word	0xe000ed00

30004904 <BOOT_NVICReFill_HP>:
30004904:	b510      	push	{r4, lr}
30004906:	f06f 0c1a 	mvn.w	ip, #26
3000490a:	4c0d      	ldr	r4, [pc, #52]	; (30004940 <BOOT_NVICReFill_HP+0x3c>)
3000490c:	480d      	ldr	r0, [pc, #52]	; (30004944 <BOOT_NVICReFill_HP+0x40>)
3000490e:	f104 031b 	add.w	r3, r4, #27
30004912:	f104 0e6b 	add.w	lr, r4, #107	; 0x6b
30004916:	ebac 0c04 	sub.w	ip, ip, r4
3000491a:	eb0c 0203 	add.w	r2, ip, r3
3000491e:	f813 1f01 	ldrb.w	r1, [r3, #1]!
30004922:	4402      	add	r2, r0
30004924:	4573      	cmp	r3, lr
30004926:	f882 1300 	strb.w	r1, [r2, #768]	; 0x300
3000492a:	d1f6      	bne.n	3000491a <BOOT_NVICReFill_HP+0x16>
3000492c:	e9d4 3100 	ldrd	r3, r1, [r4]
30004930:	68a2      	ldr	r2, [r4, #8]
30004932:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30004936:	6003      	str	r3, [r0, #0]
30004938:	6041      	str	r1, [r0, #4]
3000493a:	6082      	str	r2, [r0, #8]
3000493c:	bd10      	pop	{r4, pc}
3000493e:	bf00      	nop
30004940:	3000fcfc 	.word	0x3000fcfc
30004944:	e000e100 	.word	0xe000e100

30004948 <BOOT_ImgCopy>:
30004948:	b510      	push	{r4, lr}
3000494a:	b082      	sub	sp, #8
3000494c:	4b09      	ldr	r3, [pc, #36]	; (30004974 <BOOT_ImgCopy+0x2c>)
3000494e:	4604      	mov	r4, r0
30004950:	e9cd 2100 	strd	r2, r1, [sp]
30004954:	4798      	blx	r3
30004956:	e9dd 2100 	ldrd	r2, r1, [sp]
3000495a:	b128      	cbz	r0, 30004968 <BOOT_ImgCopy+0x20>
3000495c:	4620      	mov	r0, r4
3000495e:	4b06      	ldr	r3, [pc, #24]	; (30004978 <BOOT_ImgCopy+0x30>)
30004960:	b002      	add	sp, #8
30004962:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004966:	4718      	bx	r3
30004968:	4620      	mov	r0, r4
3000496a:	b002      	add	sp, #8
3000496c:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30004970:	f003 bf8a 	b.w	30008888 <NandImgCopy>
30004974:	30009bb5 	.word	0x30009bb5
30004978:	00012a1d 	.word	0x00012a1d

3000497c <BOOT_LoadImages>:
3000497c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30004980:	4b59      	ldr	r3, [pc, #356]	; (30004ae8 <BOOT_LoadImages+0x16c>)
30004982:	2700      	movs	r7, #0
30004984:	2104      	movs	r1, #4
30004986:	2601      	movs	r6, #1
30004988:	ed2d 8b02 	vpush	{d8}
3000498c:	b0b7      	sub	sp, #220	; 0xdc
3000498e:	f893 0055 	ldrb.w	r0, [r3, #85]	; 0x55
30004992:	4b56      	ldr	r3, [pc, #344]	; (30004aec <BOOT_LoadImages+0x170>)
30004994:	aa11      	add	r2, sp, #68	; 0x44
30004996:	9703      	str	r7, [sp, #12]
30004998:	9705      	str	r7, [sp, #20]
3000499a:	4798      	blx	r3
3000499c:	f89d 3046 	ldrb.w	r3, [sp, #70]	; 0x46
300049a0:	409e      	lsls	r6, r3
300049a2:	f5b6 0f00 	cmp.w	r6, #8388608	; 0x800000
300049a6:	f000 823e 	beq.w	30004e26 <BOOT_LoadImages+0x4aa>
300049aa:	f1b6 7f80 	cmp.w	r6, #16777216	; 0x1000000
300049ae:	f000 8379 	beq.w	300050a4 <BOOT_LoadImages+0x728>
300049b2:	f1b6 7f00 	cmp.w	r6, #33554432	; 0x2000000
300049b6:	f000 8383 	beq.w	300050c0 <BOOT_LoadImages+0x744>
300049ba:	4c4d      	ldr	r4, [pc, #308]	; (30004af0 <BOOT_LoadImages+0x174>)
300049bc:	f04f 0800 	mov.w	r8, #0
300049c0:	4d4c      	ldr	r5, [pc, #304]	; (30004af4 <BOOT_LoadImages+0x178>)
300049c2:	eb06 0904 	add.w	r9, r6, r4
300049c6:	484c      	ldr	r0, [pc, #304]	; (30004af8 <BOOT_LoadImages+0x17c>)
300049c8:	4435      	add	r5, r6
300049ca:	e9cd 8812 	strd	r8, r8, [sp, #72]	; 0x48
300049ce:	e9cd 8814 	strd	r8, r8, [sp, #80]	; 0x50
300049d2:	f7ff fa05 	bl	30003de0 <mcc_init_crc32>
300049d6:	f8d9 3004 	ldr.w	r3, [r9, #4]
300049da:	f8d5 a008 	ldr.w	sl, [r5, #8]
300049de:	f899 200c 	ldrb.w	r2, [r9, #12]
300049e2:	9308      	str	r3, [sp, #32]
300049e4:	3b01      	subs	r3, #1
300049e6:	9206      	str	r2, [sp, #24]
300049e8:	3303      	adds	r3, #3
300049ea:	686a      	ldr	r2, [r5, #4]
300049ec:	f895 b00c 	ldrb.w	fp, [r5, #12]
300049f0:	9204      	str	r2, [sp, #16]
300049f2:	f200 81bc 	bhi.w	30004d6e <BOOT_LoadImages+0x3f2>
300049f6:	f8d9 2008 	ldr.w	r2, [r9, #8]
300049fa:	9b04      	ldr	r3, [sp, #16]
300049fc:	3a01      	subs	r2, #1
300049fe:	3b01      	subs	r3, #1
30004a00:	3203      	adds	r2, #3
30004a02:	d867      	bhi.n	30004ad4 <BOOT_LoadImages+0x158>
30004a04:	9a06      	ldr	r2, [sp, #24]
30004a06:	9307      	str	r3, [sp, #28]
30004a08:	2a01      	cmp	r2, #1
30004a0a:	f240 8305 	bls.w	30005018 <BOOT_LoadImages+0x69c>
30004a0e:	483b      	ldr	r0, [pc, #236]	; (30004afc <BOOT_LoadImages+0x180>)
30004a10:	f007 fce2 	bl	3000c3d8 <__DiagPrintf_veneer>
30004a14:	9b07      	ldr	r3, [sp, #28]
30004a16:	3303      	adds	r3, #3
30004a18:	d863      	bhi.n	30004ae2 <BOOT_LoadImages+0x166>
30004a1a:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30004a1e:	f11a 0f03 	cmn.w	sl, #3
30004a22:	f200 81aa 	bhi.w	30004d7a <BOOT_LoadImages+0x3fe>
30004a26:	f1bb 0f01 	cmp.w	fp, #1
30004a2a:	f200 8347 	bhi.w	300050bc <BOOT_LoadImages+0x740>
30004a2e:	9904      	ldr	r1, [sp, #16]
30004a30:	4833      	ldr	r0, [pc, #204]	; (30004b00 <BOOT_LoadImages+0x184>)
30004a32:	f007 fcd1 	bl	3000c3d8 <__DiagPrintf_veneer>
30004a36:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004a3a:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004a3e:	4c2e      	ldr	r4, [pc, #184]	; (30004af8 <BOOT_LoadImages+0x17c>)
30004a40:	9307      	str	r3, [sp, #28]
30004a42:	2300      	movs	r3, #0
30004a44:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004a48:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004a4c:	9309      	str	r3, [sp, #36]	; 0x24
30004a4e:	f8df c0c8 	ldr.w	ip, [pc, #200]	; 30004b18 <BOOT_LoadImages+0x19c>
30004a52:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
30004a56:	6829      	ldr	r1, [r5, #0]
30004a58:	f04f 30ff 	mov.w	r0, #4294967295	; 0xffffffff
30004a5c:	44b4      	add	ip, r6
30004a5e:	f81c 2b01 	ldrb.w	r2, [ip], #1
30004a62:	4042      	eors	r2, r0
30004a64:	459c      	cmp	ip, r3
30004a66:	b2d2      	uxtb	r2, r2
30004a68:	f854 2022 	ldr.w	r2, [r4, r2, lsl #2]
30004a6c:	ea82 2010 	eor.w	r0, r2, r0, lsr #8
30004a70:	d1f5      	bne.n	30004a5e <BOOT_LoadImages+0xe2>
30004a72:	43c2      	mvns	r2, r0
30004a74:	930f      	str	r3, [sp, #60]	; 0x3c
30004a76:	4291      	cmp	r1, r2
30004a78:	f000 8301 	beq.w	3000507e <BOOT_LoadImages+0x702>
30004a7c:	4821      	ldr	r0, [pc, #132]	; (30004b04 <BOOT_LoadImages+0x188>)
30004a7e:	f007 fcab 	bl	3000c3d8 <__DiagPrintf_veneer>
30004a82:	2200      	movs	r2, #0
30004a84:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004a86:	9909      	ldr	r1, [sp, #36]	; 0x24
30004a88:	2900      	cmp	r1, #0
30004a8a:	d04e      	beq.n	30004b2a <BOOT_LoadImages+0x1ae>
30004a8c:	f899 100c 	ldrb.w	r1, [r9, #12]
30004a90:	455b      	cmp	r3, fp
30004a92:	9106      	str	r1, [sp, #24]
30004a94:	d342      	bcc.n	30004b1c <BOOT_LoadImages+0x1a0>
30004a96:	f06f 4c78 	mvn.w	ip, #4160749568	; 0xf8000000
30004a9a:	45e3      	cmp	fp, ip
30004a9c:	d93e      	bls.n	30004b1c <BOOT_LoadImages+0x1a0>
30004a9e:	9807      	ldr	r0, [sp, #28]
30004aa0:	4560      	cmp	r0, ip
30004aa2:	f240 8203 	bls.w	30004eac <BOOT_LoadImages+0x530>
30004aa6:	4283      	cmp	r3, r0
30004aa8:	f0c0 8200 	bcc.w	30004eac <BOOT_LoadImages+0x530>
30004aac:	2a00      	cmp	r2, #0
30004aae:	f000 820d 	beq.w	30004ecc <BOOT_LoadImages+0x550>
30004ab2:	4553      	cmp	r3, sl
30004ab4:	f080 8410 	bcs.w	300052d8 <BOOT_LoadImages+0x95c>
30004ab8:	4813      	ldr	r0, [pc, #76]	; (30004b08 <BOOT_LoadImages+0x18c>)
30004aba:	f007 fc8d 	bl	3000c3d8 <__DiagPrintf_veneer>
30004abe:	9f06      	ldr	r7, [sp, #24]
30004ac0:	4812      	ldr	r0, [pc, #72]	; (30004b0c <BOOT_LoadImages+0x190>)
30004ac2:	464d      	mov	r5, r9
30004ac4:	f007 fc88 	bl	3000c3d8 <__DiagPrintf_veneer>
30004ac8:	9b07      	ldr	r3, [sp, #28]
30004aca:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004ace:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004ad2:	e04a      	b.n	30004b6a <BOOT_LoadImages+0x1ee>
30004ad4:	480e      	ldr	r0, [pc, #56]	; (30004b10 <BOOT_LoadImages+0x194>)
30004ad6:	9307      	str	r3, [sp, #28]
30004ad8:	f007 fc7e 	bl	3000c3d8 <__DiagPrintf_veneer>
30004adc:	9b07      	ldr	r3, [sp, #28]
30004ade:	3303      	adds	r3, #3
30004ae0:	d99b      	bls.n	30004a1a <BOOT_LoadImages+0x9e>
30004ae2:	480c      	ldr	r0, [pc, #48]	; (30004b14 <BOOT_LoadImages+0x198>)
30004ae4:	e14a      	b.n	30004d7c <BOOT_LoadImages+0x400>
30004ae6:	bf00      	nop
30004ae8:	2001c01c 	.word	0x2001c01c
30004aec:	00009db5 	.word	0x00009db5
30004af0:	07ffe000 	.word	0x07ffe000
30004af4:	07fff000 	.word	0x07fff000
30004af8:	3000e89c 	.word	0x3000e89c
30004afc:	3000c840 	.word	0x3000c840
30004b00:	3000cc28 	.word	0x3000cc28
30004b04:	3000c944 	.word	0x3000c944
30004b08:	3000cc0c 	.word	0x3000cc0c
30004b0c:	3000c9d0 	.word	0x3000c9d0
30004b10:	3000c82c 	.word	0x3000c82c
30004b14:	3000cbd8 	.word	0x3000cbd8
30004b18:	07fff004 	.word	0x07fff004
30004b1c:	48c4      	ldr	r0, [pc, #784]	; (30004e30 <BOOT_LoadImages+0x4b4>)
30004b1e:	930f      	str	r3, [sp, #60]	; 0x3c
30004b20:	9209      	str	r2, [sp, #36]	; 0x24
30004b22:	f007 fc59 	bl	3000c3d8 <__DiagPrintf_veneer>
30004b26:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004b28:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004b2a:	2a00      	cmp	r2, #0
30004b2c:	f000 8131 	beq.w	30004d92 <BOOT_LoadImages+0x416>
30004b30:	459a      	cmp	sl, r3
30004b32:	f200 82c9 	bhi.w	300050c8 <BOOT_LoadImages+0x74c>
30004b36:	f1ba 6f00 	cmp.w	sl, #134217728	; 0x8000000
30004b3a:	bf2c      	ite	cs
30004b3c:	2200      	movcs	r2, #0
30004b3e:	2201      	movcc	r2, #1
30004b40:	2a00      	cmp	r2, #0
30004b42:	f040 82c1 	bne.w	300050c8 <BOOT_LoadImages+0x74c>
30004b46:	4598      	cmp	r8, r3
30004b48:	f200 81b8 	bhi.w	30004ebc <BOOT_LoadImages+0x540>
30004b4c:	f1b8 6f00 	cmp.w	r8, #134217728	; 0x8000000
30004b50:	f0c0 81b4 	bcc.w	30004ebc <BOOT_LoadImages+0x540>
30004b54:	7b2f      	ldrb	r7, [r5, #12]
30004b56:	2a00      	cmp	r2, #0
30004b58:	f040 81de 	bne.w	30004f18 <BOOT_LoadImages+0x59c>
30004b5c:	48b5      	ldr	r0, [pc, #724]	; (30004e34 <BOOT_LoadImages+0x4b8>)
30004b5e:	f007 fc3b 	bl	3000c3d8 <__DiagPrintf_veneer>
30004b62:	f8c4 a400 	str.w	sl, [r4, #1024]	; 0x400
30004b66:	f8c4 8404 	str.w	r8, [r4, #1028]	; 0x404
30004b6a:	4639      	mov	r1, r7
30004b6c:	48b2      	ldr	r0, [pc, #712]	; (30004e38 <BOOT_LoadImages+0x4bc>)
30004b6e:	f007 fc33 	bl	3000c3d8 <__DiagPrintf_veneer>
30004b72:	4bb2      	ldr	r3, [pc, #712]	; (30004e3c <BOOT_LoadImages+0x4c0>)
30004b74:	4798      	blx	r3
30004b76:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004b7a:	f8d4 2404 	ldr.w	r2, [r4, #1028]	; 0x404
30004b7e:	f8d1 300a 	ldr.w	r3, [r1, #10]
30004b82:	6809      	ldr	r1, [r1, #0]
30004b84:	330c      	adds	r3, #12
30004b86:	910b      	str	r1, [sp, #44]	; 0x2c
30004b88:	42b3      	cmp	r3, r6
30004b8a:	bf28      	it	cs
30004b8c:	4633      	movcs	r3, r6
30004b8e:	930d      	str	r3, [sp, #52]	; 0x34
30004b90:	f8d2 300a 	ldr.w	r3, [r2, #10]
30004b94:	6812      	ldr	r2, [r2, #0]
30004b96:	330c      	adds	r3, #12
30004b98:	920a      	str	r2, [sp, #40]	; 0x28
30004b9a:	429e      	cmp	r6, r3
30004b9c:	bf28      	it	cs
30004b9e:	461e      	movcs	r6, r3
30004ba0:	960c      	str	r6, [sp, #48]	; 0x30
30004ba2:	2800      	cmp	r0, #0
30004ba4:	f040 8194 	bne.w	30004ed0 <BOOT_LoadImages+0x554>
30004ba8:	4ba5      	ldr	r3, [pc, #660]	; (30004e40 <BOOT_LoadImages+0x4c4>)
30004baa:	4606      	mov	r6, r0
30004bac:	f8df 92bc 	ldr.w	r9, [pc, #700]	; 30004e6c <BOOT_LoadImages+0x4f0>
30004bb0:	4680      	mov	r8, r0
30004bb2:	f8df b2bc 	ldr.w	fp, [pc, #700]	; 30004e70 <BOOT_LoadImages+0x4f4>
30004bb6:	4682      	mov	sl, r0
30004bb8:	e9cd 0303 	strd	r0, r3, [sp, #12]
30004bbc:	2f00      	cmp	r7, #0
30004bbe:	f040 81c9 	bne.w	30004f54 <BOOT_LoadImages+0x5d8>
30004bc2:	f8d4 c400 	ldr.w	ip, [r4, #1024]	; 0x400
30004bc6:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30004bc8:	f10c 0104 	add.w	r1, ip, #4
30004bcc:	2b00      	cmp	r3, #0
30004bce:	f000 83df 	beq.w	30005390 <BOOT_LoadImages+0xa14>
30004bd2:	1d18      	adds	r0, r3, #4
30004bd4:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004bd8:	4460      	add	r0, ip
30004bda:	f811 3b01 	ldrb.w	r3, [r1], #1
30004bde:	4053      	eors	r3, r2
30004be0:	4281      	cmp	r1, r0
30004be2:	b2db      	uxtb	r3, r3
30004be4:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004be8:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004bec:	d1f5      	bne.n	30004bda <BOOT_LoadImages+0x25e>
30004bee:	43d2      	mvns	r2, r2
30004bf0:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
30004bf2:	4293      	cmp	r3, r2
30004bf4:	f000 83a7 	beq.w	30005346 <BOOT_LoadImages+0x9ca>
30004bf8:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004bfc:	f088 0301 	eor.w	r3, r8, #1
30004c00:	3101      	adds	r1, #1
30004c02:	f003 0301 	and.w	r3, r3, #1
30004c06:	ea46 0603 	orr.w	r6, r6, r3
30004c0a:	f000 8346 	beq.w	3000529a <BOOT_LoadImages+0x91e>
30004c0e:	990b      	ldr	r1, [sp, #44]	; 0x2c
30004c10:	488c      	ldr	r0, [pc, #560]	; (30004e44 <BOOT_LoadImages+0x4c8>)
30004c12:	f007 fbe1 	bl	3000c3d8 <__DiagPrintf_veneer>
30004c16:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
30004c1a:	2002      	movs	r0, #2
30004c1c:	f7ff f85e 	bl	30003cdc <FLASH_Erase_With_Lock>
30004c20:	2e00      	cmp	r6, #0
30004c22:	f040 8324 	bne.w	3000526e <BOOT_LoadImages+0x8f2>
30004c26:	9605      	str	r6, [sp, #20]
30004c28:	f08a 0601 	eor.w	r6, sl, #1
30004c2c:	f04f 0801 	mov.w	r8, #1
30004c30:	f006 0601 	and.w	r6, r6, #1
30004c34:	9b03      	ldr	r3, [sp, #12]
30004c36:	2b01      	cmp	r3, #1
30004c38:	f000 8238 	beq.w	300050ac <BOOT_LoadImages+0x730>
30004c3c:	9b03      	ldr	r3, [sp, #12]
30004c3e:	2b02      	cmp	r3, #2
30004c40:	f000 83b2 	beq.w	300053a8 <BOOT_LoadImages+0xa2c>
30004c44:	9b0e      	ldr	r3, [sp, #56]	; 0x38
30004c46:	4650      	mov	r0, sl
30004c48:	f8c9 3000 	str.w	r3, [r9]
30004c4c:	47d8      	blx	fp
30004c4e:	1e03      	subs	r3, r0, #0
30004c50:	bf18      	it	ne
30004c52:	2301      	movne	r3, #1
30004c54:	2800      	cmp	r0, #0
30004c56:	f000 8156 	beq.w	30004f06 <BOOT_LoadImages+0x58a>
30004c5a:	2e00      	cmp	r6, #0
30004c5c:	f000 8153 	beq.w	30004f06 <BOOT_LoadImages+0x58a>
30004c60:	fab7 f787 	clz	r7, r7
30004c64:	9b05      	ldr	r3, [sp, #20]
30004c66:	097f      	lsrs	r7, r7, #5
30004c68:	2b00      	cmp	r3, #0
30004c6a:	f000 83ee 	beq.w	3000544a <BOOT_LoadImages+0xace>
30004c6e:	469a      	mov	sl, r3
30004c70:	4c75      	ldr	r4, [pc, #468]	; (30004e48 <BOOT_LoadImages+0x4cc>)
30004c72:	2f00      	cmp	r7, #0
30004c74:	f040 8138 	bne.w	30004ee8 <BOOT_LoadImages+0x56c>
30004c78:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30004c7c:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004c80:	3201      	adds	r2, #1
30004c82:	f000 82ad 	beq.w	300051e0 <BOOT_LoadImages+0x864>
30004c86:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004c8a:	4870      	ldr	r0, [pc, #448]	; (30004e4c <BOOT_LoadImages+0x4d0>)
30004c8c:	f007 fba4 	bl	3000c3d8 <__DiagPrintf_veneer>
30004c90:	2301      	movs	r3, #1
30004c92:	9303      	str	r3, [sp, #12]
30004c94:	f04f 0801 	mov.w	r8, #1
30004c98:	4646      	mov	r6, r8
30004c9a:	f8cd 8014 	str.w	r8, [sp, #20]
30004c9e:	f1ba 0f00 	cmp.w	sl, #0
30004ca2:	d0c7      	beq.n	30004c34 <BOOT_LoadImages+0x2b8>
30004ca4:	ab16      	add	r3, sp, #88	; 0x58
30004ca6:	2280      	movs	r2, #128	; 0x80
30004ca8:	21ff      	movs	r1, #255	; 0xff
30004caa:	46d0      	mov	r8, sl
30004cac:	461e      	mov	r6, r3
30004cae:	4618      	mov	r0, r3
30004cb0:	ee08 3a10 	vmov	s16, r3
30004cb4:	f007 fb60 	bl	3000c378 <____wrap_memset_veneer>
30004cb8:	2280      	movs	r2, #128	; 0x80
30004cba:	4629      	mov	r1, r5
30004cbc:	4630      	mov	r0, r6
30004cbe:	f007 fb6b 	bl	3000c398 <____wrap_memcpy_veneer>
30004cc2:	2f01      	cmp	r7, #1
30004cc4:	f000 8327 	beq.w	30005316 <BOOT_LoadImages+0x99a>
30004cc8:	2100      	movs	r1, #0
30004cca:	f8cd a014 	str.w	sl, [sp, #20]
30004cce:	f04f 0a01 	mov.w	sl, #1
30004cd2:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
30004cd6:	1d2e      	adds	r6, r5, #4
30004cd8:	485d      	ldr	r0, [pc, #372]	; (30004e50 <BOOT_LoadImages+0x4d4>)
30004cda:	f007 fb7d 	bl	3000c3d8 <__DiagPrintf_veneer>
30004cde:	4629      	mov	r1, r5
30004ce0:	2002      	movs	r0, #2
30004ce2:	f7fe fffb 	bl	30003cdc <FLASH_Erase_With_Lock>
30004ce6:	aa17      	add	r2, sp, #92	; 0x5c
30004ce8:	217c      	movs	r1, #124	; 0x7c
30004cea:	4630      	mov	r0, r6
30004cec:	f7ff f81e 	bl	30003d2c <FLASH_TxData_With_Lock>
30004cf0:	4630      	mov	r0, r6
30004cf2:	f505 5180 	add.w	r1, r5, #4096	; 0x1000
30004cf6:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004cfa:	f810 3b01 	ldrb.w	r3, [r0], #1
30004cfe:	4053      	eors	r3, r2
30004d00:	4288      	cmp	r0, r1
30004d02:	b2db      	uxtb	r3, r3
30004d04:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004d08:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004d0c:	d1f5      	bne.n	30004cfa <BOOT_LoadImages+0x37e>
30004d0e:	43d2      	mvns	r2, r2
30004d10:	4629      	mov	r1, r5
30004d12:	2002      	movs	r0, #2
30004d14:	9216      	str	r2, [sp, #88]	; 0x58
30004d16:	f7fe ffe1 	bl	30003cdc <FLASH_Erase_With_Lock>
30004d1a:	ee18 2a10 	vmov	r2, s16
30004d1e:	2180      	movs	r1, #128	; 0x80
30004d20:	4628      	mov	r0, r5
30004d22:	f7ff f803 	bl	30003d2c <FLASH_TxData_With_Lock>
30004d26:	9b05      	ldr	r3, [sp, #20]
30004d28:	2b00      	cmp	r3, #0
30004d2a:	f000 8143 	beq.w	30004fb4 <BOOT_LoadImages+0x638>
30004d2e:	f1b8 0f00 	cmp.w	r8, #0
30004d32:	f040 837f 	bne.w	30005434 <BOOT_LoadImages+0xab8>
30004d36:	2f01      	cmp	r7, #1
30004d38:	f000 8268 	beq.w	3000520c <BOOT_LoadImages+0x890>
30004d3c:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d40:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004d44:	3201      	adds	r2, #1
30004d46:	f000 81d4 	beq.w	300050f2 <BOOT_LoadImages+0x776>
30004d4a:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004d4e:	4841      	ldr	r0, [pc, #260]	; (30004e54 <BOOT_LoadImages+0x4d8>)
30004d50:	f007 fb42 	bl	3000c3d8 <__DiagPrintf_veneer>
30004d54:	f08a 0601 	eor.w	r6, sl, #1
30004d58:	f04f 0801 	mov.w	r8, #1
30004d5c:	f006 0601 	and.w	r6, r6, #1
30004d60:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004d64:	3310      	adds	r3, #16
30004d66:	930e      	str	r3, [sp, #56]	; 0x38
30004d68:	2302      	movs	r3, #2
30004d6a:	9303      	str	r3, [sp, #12]
30004d6c:	e76a      	b.n	30004c44 <BOOT_LoadImages+0x2c8>
30004d6e:	483a      	ldr	r0, [pc, #232]	; (30004e58 <BOOT_LoadImages+0x4dc>)
30004d70:	f007 fb32 	bl	3000c3d8 <__DiagPrintf_veneer>
30004d74:	9b04      	ldr	r3, [sp, #16]
30004d76:	3b01      	subs	r3, #1
30004d78:	e64d      	b.n	30004a16 <BOOT_LoadImages+0x9a>
30004d7a:	4838      	ldr	r0, [pc, #224]	; (30004e5c <BOOT_LoadImages+0x4e0>)
30004d7c:	f007 fb2c 	bl	3000c3d8 <__DiagPrintf_veneer>
30004d80:	f8d9 3011 	ldr.w	r3, [r9, #17]
30004d84:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30004d88:	9307      	str	r3, [sp, #28]
30004d8a:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30004d8e:	f8d5 8011 	ldr.w	r8, [r5, #17]
30004d92:	4833      	ldr	r0, [pc, #204]	; (30004e60 <BOOT_LoadImages+0x4e4>)
30004d94:	f007 fb20 	bl	3000c3d8 <__DiagPrintf_veneer>
30004d98:	4832      	ldr	r0, [pc, #200]	; (30004e64 <BOOT_LoadImages+0x4e8>)
30004d9a:	f007 fb1d 	bl	3000c3d8 <__DiagPrintf_veneer>
30004d9e:	4631      	mov	r1, r6
30004da0:	aa12      	add	r2, sp, #72	; 0x48
30004da2:	4658      	mov	r0, fp
30004da4:	f7ff f862 	bl	30003e6c <recovery_check>
30004da8:	4631      	mov	r1, r6
30004daa:	4604      	mov	r4, r0
30004dac:	aa13      	add	r2, sp, #76	; 0x4c
30004dae:	9807      	ldr	r0, [sp, #28]
30004db0:	f7ff f85c 	bl	30003e6c <recovery_check>
30004db4:	4631      	mov	r1, r6
30004db6:	aa14      	add	r2, sp, #80	; 0x50
30004db8:	9004      	str	r0, [sp, #16]
30004dba:	4650      	mov	r0, sl
30004dbc:	f7ff f856 	bl	30003e6c <recovery_check>
30004dc0:	4631      	mov	r1, r6
30004dc2:	aa15      	add	r2, sp, #84	; 0x54
30004dc4:	4606      	mov	r6, r0
30004dc6:	4640      	mov	r0, r8
30004dc8:	f7ff f850 	bl	30003e6c <recovery_check>
30004dcc:	9b04      	ldr	r3, [sp, #16]
30004dce:	4306      	orrs	r6, r0
30004dd0:	431c      	orrs	r4, r3
30004dd2:	d04f      	beq.n	30004e74 <BOOT_LoadImages+0x4f8>
30004dd4:	e9dd 3212 	ldrd	r3, r2, [sp, #72]	; 0x48
30004dd8:	2e00      	cmp	r6, #0
30004dda:	f000 818e 	beq.w	300050fa <BOOT_LoadImages+0x77e>
30004dde:	4293      	cmp	r3, r2
30004de0:	e9dd 1014 	ldrd	r1, r0, [sp, #80]	; 0x50
30004de4:	f0c0 827c 	bcc.w	300052e0 <BOOT_LoadImages+0x964>
30004de8:	428b      	cmp	r3, r1
30004dea:	d302      	bcc.n	30004df2 <BOOT_LoadImages+0x476>
30004dec:	4283      	cmp	r3, r0
30004dee:	f080 82fd 	bcs.w	300053ec <BOOT_LoadImages+0xa70>
30004df2:	4293      	cmp	r3, r2
30004df4:	f000 8274 	beq.w	300052e0 <BOOT_LoadImages+0x964>
30004df8:	4293      	cmp	r3, r2
30004dfa:	bf38      	it	cc
30004dfc:	4613      	movcc	r3, r2
30004dfe:	428b      	cmp	r3, r1
30004e00:	f240 82e5 	bls.w	300053ce <BOOT_LoadImages+0xa52>
30004e04:	428b      	cmp	r3, r1
30004e06:	bf38      	it	cc
30004e08:	460b      	movcc	r3, r1
30004e0a:	4283      	cmp	r3, r0
30004e0c:	f240 82f6 	bls.w	300053fc <BOOT_LoadImages+0xa80>
30004e10:	f04f 0800 	mov.w	r8, #0
30004e14:	4b0a      	ldr	r3, [pc, #40]	; (30004e40 <BOOT_LoadImages+0x4c4>)
30004e16:	2601      	movs	r6, #1
30004e18:	f8df 9050 	ldr.w	r9, [pc, #80]	; 30004e6c <BOOT_LoadImages+0x4f0>
30004e1c:	f8df b050 	ldr.w	fp, [pc, #80]	; 30004e70 <BOOT_LoadImages+0x4f4>
30004e20:	46c2      	mov	sl, r8
30004e22:	9304      	str	r3, [sp, #16]
30004e24:	e70e      	b.n	30004c44 <BOOT_LoadImages+0x2c8>
30004e26:	4810      	ldr	r0, [pc, #64]	; (30004e68 <BOOT_LoadImages+0x4ec>)
30004e28:	f007 fad6 	bl	3000c3d8 <__DiagPrintf_veneer>
30004e2c:	e5c5      	b.n	300049ba <BOOT_LoadImages+0x3e>
30004e2e:	bf00      	nop
30004e30:	3000c984 	.word	0x3000c984
30004e34:	3000c9e0 	.word	0x3000c9e0
30004e38:	3000ca14 	.word	0x3000ca14
30004e3c:	0000c149 	.word	0x0000c149
30004e40:	3000e6d7 	.word	0x3000e6d7
30004e44:	3000ca84 	.word	0x3000ca84
30004e48:	3000e89c 	.word	0x3000e89c
30004e4c:	3000c778 	.word	0x3000c778
30004e50:	3000cb58 	.word	0x3000cb58
30004e54:	3000c790 	.word	0x3000c790
30004e58:	3000c814 	.word	0x3000c814
30004e5c:	3000c878 	.word	0x3000c878
30004e60:	3000c9f0 	.word	0x3000c9f0
30004e64:	3000ca04 	.word	0x3000ca04
30004e68:	3000c7dc 	.word	0x3000c7dc
30004e6c:	3000fcf8 	.word	0x3000fcf8
30004e70:	30006c7d 	.word	0x30006c7d
30004e74:	2e00      	cmp	r6, #0
30004e76:	f000 8285 	beq.w	30005384 <BOOT_LoadImages+0xa08>
30004e7a:	4651      	mov	r1, sl
30004e7c:	e9dd 2314 	ldrd	r2, r3, [sp, #80]	; 0x50
30004e80:	e9cd 5400 	strd	r5, r4, [sp]
30004e84:	429a      	cmp	r2, r3
30004e86:	464b      	mov	r3, r9
30004e88:	4642      	mov	r2, r8
30004e8a:	f080 8223 	bcs.w	300052d4 <BOOT_LoadImages+0x958>
30004e8e:	2001      	movs	r0, #1
30004e90:	f7ff f81c 	bl	30003ecc <rewrite_bp>
30004e94:	46a0      	mov	r8, r4
30004e96:	b2c3      	uxtb	r3, r0
30004e98:	2601      	movs	r6, #1
30004e9a:	46a2      	mov	sl, r4
30004e9c:	f8df 92e8 	ldr.w	r9, [pc, #744]	; 30005188 <BOOT_LoadImages+0x80c>
30004ea0:	9303      	str	r3, [sp, #12]
30004ea2:	4ba3      	ldr	r3, [pc, #652]	; (30005130 <BOOT_LoadImages+0x7b4>)
30004ea4:	f8df b2e4 	ldr.w	fp, [pc, #740]	; 3000518c <BOOT_LoadImages+0x810>
30004ea8:	9304      	str	r3, [sp, #16]
30004eaa:	e6c3      	b.n	30004c34 <BOOT_LoadImages+0x2b8>
30004eac:	48a1      	ldr	r0, [pc, #644]	; (30005134 <BOOT_LoadImages+0x7b8>)
30004eae:	930f      	str	r3, [sp, #60]	; 0x3c
30004eb0:	9209      	str	r2, [sp, #36]	; 0x24
30004eb2:	f007 fa91 	bl	3000c3d8 <__DiagPrintf_veneer>
30004eb6:	9a09      	ldr	r2, [sp, #36]	; 0x24
30004eb8:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
30004eba:	e636      	b.n	30004b2a <BOOT_LoadImages+0x1ae>
30004ebc:	489e      	ldr	r0, [pc, #632]	; (30005138 <BOOT_LoadImages+0x7bc>)
30004ebe:	9204      	str	r2, [sp, #16]
30004ec0:	f007 fa8a 	bl	3000c3d8 <__DiagPrintf_veneer>
30004ec4:	9a04      	ldr	r2, [sp, #16]
30004ec6:	2a00      	cmp	r2, #0
30004ec8:	f43f af63 	beq.w	30004d92 <BOOT_LoadImages+0x416>
30004ecc:	9f06      	ldr	r7, [sp, #24]
30004ece:	e5f7      	b.n	30004ac0 <BOOT_LoadImages+0x144>
30004ed0:	2300      	movs	r3, #0
30004ed2:	4a97      	ldr	r2, [pc, #604]	; (30005130 <BOOT_LoadImages+0x7b4>)
30004ed4:	f8df 92b0 	ldr.w	r9, [pc, #688]	; 30005188 <BOOT_LoadImages+0x80c>
30004ed8:	f8df b2b0 	ldr.w	fp, [pc, #688]	; 3000518c <BOOT_LoadImages+0x810>
30004edc:	469a      	mov	sl, r3
30004ede:	9303      	str	r3, [sp, #12]
30004ee0:	9204      	str	r2, [sp, #16]
30004ee2:	2f00      	cmp	r7, #0
30004ee4:	f43f aec8 	beq.w	30004c78 <BOOT_LoadImages+0x2fc>
30004ee8:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30004eec:	f8d3 200a 	ldr.w	r2, [r3, #10]
30004ef0:	3201      	adds	r2, #1
30004ef2:	f000 819f 	beq.w	30005234 <BOOT_LoadImages+0x8b8>
30004ef6:	f8d3 1006 	ldr.w	r1, [r3, #6]
30004efa:	4890      	ldr	r0, [pc, #576]	; (3000513c <BOOT_LoadImages+0x7c0>)
30004efc:	f007 fa6c 	bl	3000c3d8 <__DiagPrintf_veneer>
30004f00:	2302      	movs	r3, #2
30004f02:	9303      	str	r3, [sp, #12]
30004f04:	e6c6      	b.n	30004c94 <BOOT_LoadImages+0x318>
30004f06:	f00a 0401 	and.w	r4, sl, #1
30004f0a:	b1c3      	cbz	r3, 30004f3e <BOOT_LoadImages+0x5c2>
30004f0c:	b1bc      	cbz	r4, 30004f3e <BOOT_LoadImages+0x5c2>
30004f0e:	4c8c      	ldr	r4, [pc, #560]	; (30005140 <BOOT_LoadImages+0x7c4>)
30004f10:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30004f14:	47a0      	blx	r4
30004f16:	e7fb      	b.n	30004f10 <BOOT_LoadImages+0x594>
30004f18:	488a      	ldr	r0, [pc, #552]	; (30005144 <BOOT_LoadImages+0x7c8>)
30004f1a:	f007 fa5d 	bl	3000c3d8 <__DiagPrintf_veneer>
30004f1e:	9b08      	ldr	r3, [sp, #32]
30004f20:	9a04      	ldr	r2, [sp, #16]
30004f22:	4293      	cmp	r3, r2
30004f24:	f4ff ae1a 	bcc.w	30004b5c <BOOT_LoadImages+0x1e0>
30004f28:	4887      	ldr	r0, [pc, #540]	; (30005148 <BOOT_LoadImages+0x7cc>)
30004f2a:	464d      	mov	r5, r9
30004f2c:	f007 fa54 	bl	3000c3d8 <__DiagPrintf_veneer>
30004f30:	9b07      	ldr	r3, [sp, #28]
30004f32:	9f06      	ldr	r7, [sp, #24]
30004f34:	f8c4 b400 	str.w	fp, [r4, #1024]	; 0x400
30004f38:	f8c4 3404 	str.w	r3, [r4, #1028]	; 0x404
30004f3c:	e615      	b.n	30004b6a <BOOT_LoadImages+0x1ee>
30004f3e:	9b04      	ldr	r3, [sp, #16]
30004f40:	781b      	ldrb	r3, [r3, #0]
30004f42:	2b00      	cmp	r3, #0
30004f44:	f040 80c4 	bne.w	300050d0 <BOOT_LoadImages+0x754>
30004f48:	2001      	movs	r0, #1
30004f4a:	b037      	add	sp, #220	; 0xdc
30004f4c:	ecbd 8b02 	vpop	{d8}
30004f50:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30004f54:	f8d4 c404 	ldr.w	ip, [r4, #1028]	; 0x404
30004f58:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004f5a:	f10c 0104 	add.w	r1, ip, #4
30004f5e:	2b00      	cmp	r3, #0
30004f60:	f000 8214 	beq.w	3000538c <BOOT_LoadImages+0xa10>
30004f64:	1d18      	adds	r0, r3, #4
30004f66:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004f6a:	4460      	add	r0, ip
30004f6c:	f811 3b01 	ldrb.w	r3, [r1], #1
30004f70:	4053      	eors	r3, r2
30004f72:	4281      	cmp	r1, r0
30004f74:	b2db      	uxtb	r3, r3
30004f76:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004f7a:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004f7e:	d1f5      	bne.n	30004f6c <BOOT_LoadImages+0x5f0>
30004f80:	43d2      	mvns	r2, r2
30004f82:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004f84:	4293      	cmp	r3, r2
30004f86:	f000 81cd 	beq.w	30005324 <BOOT_LoadImages+0x9a8>
30004f8a:	f8dc 100a 	ldr.w	r1, [ip, #10]
30004f8e:	f088 0301 	eor.w	r3, r8, #1
30004f92:	3101      	adds	r1, #1
30004f94:	f003 0301 	and.w	r3, r3, #1
30004f98:	ea46 0603 	orr.w	r6, r6, r3
30004f9c:	f000 8181 	beq.w	300052a2 <BOOT_LoadImages+0x926>
30004fa0:	990a      	ldr	r1, [sp, #40]	; 0x28
30004fa2:	486a      	ldr	r0, [pc, #424]	; (3000514c <BOOT_LoadImages+0x7d0>)
30004fa4:	f007 fa18 	bl	3000c3d8 <__DiagPrintf_veneer>
30004fa8:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
30004fac:	2002      	movs	r0, #2
30004fae:	f7fe fe95 	bl	30003cdc <FLASH_Erase_With_Lock>
30004fb2:	e635      	b.n	30004c20 <BOOT_LoadImages+0x2a4>
30004fb4:	f1b8 0f00 	cmp.w	r8, #0
30004fb8:	f040 8227 	bne.w	3000540a <BOOT_LoadImages+0xa8e>
30004fbc:	2f01      	cmp	r7, #1
30004fbe:	f000 80e7 	beq.w	30005190 <BOOT_LoadImages+0x814>
30004fc2:	f8d4 6404 	ldr.w	r6, [r4, #1028]	; 0x404
30004fc6:	9b0c      	ldr	r3, [sp, #48]	; 0x30
30004fc8:	1d31      	adds	r1, r6, #4
30004fca:	2b00      	cmp	r3, #0
30004fcc:	f000 81ee 	beq.w	300053ac <BOOT_LoadImages+0xa30>
30004fd0:	1d18      	adds	r0, r3, #4
30004fd2:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
30004fd6:	4430      	add	r0, r6
30004fd8:	f811 3b01 	ldrb.w	r3, [r1], #1
30004fdc:	4053      	eors	r3, r2
30004fde:	4281      	cmp	r1, r0
30004fe0:	b2db      	uxtb	r3, r3
30004fe2:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
30004fe6:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30004fea:	d1f5      	bne.n	30004fd8 <BOOT_LoadImages+0x65c>
30004fec:	43d2      	mvns	r2, r2
30004fee:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30004ff0:	4293      	cmp	r3, r2
30004ff2:	f000 81b8 	beq.w	30005366 <BOOT_LoadImages+0x9ea>
30004ff6:	f8d6 300a 	ldr.w	r3, [r6, #10]
30004ffa:	3301      	adds	r3, #1
30004ffc:	d079      	beq.n	300050f2 <BOOT_LoadImages+0x776>
30004ffe:	990a      	ldr	r1, [sp, #40]	; 0x28
30005000:	4852      	ldr	r0, [pc, #328]	; (3000514c <BOOT_LoadImages+0x7d0>)
30005002:	f007 f9e9 	bl	3000c3d8 <__DiagPrintf_veneer>
30005006:	f8d4 1404 	ldr.w	r1, [r4, #1028]	; 0x404
3000500a:	2002      	movs	r0, #2
3000500c:	f7fe fe66 	bl	30003cdc <FLASH_Erase_With_Lock>
30005010:	484f      	ldr	r0, [pc, #316]	; (30005150 <BOOT_LoadImages+0x7d4>)
30005012:	f007 f9e1 	bl	3000c3d8 <__DiagPrintf_veneer>
30005016:	e7fe      	b.n	30005016 <BOOT_LoadImages+0x69a>
30005018:	9908      	ldr	r1, [sp, #32]
3000501a:	484e      	ldr	r0, [pc, #312]	; (30005154 <BOOT_LoadImages+0x7d8>)
3000501c:	f007 f9dc 	bl	3000c3d8 <__DiagPrintf_veneer>
30005020:	9b07      	ldr	r3, [sp, #28]
30005022:	3303      	adds	r3, #3
30005024:	f240 81b6 	bls.w	30005394 <BOOT_LoadImages+0xa18>
30005028:	484b      	ldr	r0, [pc, #300]	; (30005158 <BOOT_LoadImages+0x7dc>)
3000502a:	f007 f9d5 	bl	3000c3d8 <__DiagPrintf_veneer>
3000502e:	f8cd 8024 	str.w	r8, [sp, #36]	; 0x24
30005032:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005036:	f8d9 b00d 	ldr.w	fp, [r9, #13]
3000503a:	9307      	str	r3, [sp, #28]
3000503c:	f8d5 a00d 	ldr.w	sl, [r5, #13]
30005040:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005044:	4845      	ldr	r0, [pc, #276]	; (3000515c <BOOT_LoadImages+0x7e0>)
30005046:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
3000504a:	5931      	ldr	r1, [r6, r4]
3000504c:	4430      	add	r0, r6
3000504e:	4c44      	ldr	r4, [pc, #272]	; (30005160 <BOOT_LoadImages+0x7e4>)
30005050:	f810 3b01 	ldrb.w	r3, [r0], #1
30005054:	4053      	eors	r3, r2
30005056:	4285      	cmp	r5, r0
30005058:	b2db      	uxtb	r3, r3
3000505a:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
3000505e:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
30005062:	d1f5      	bne.n	30005050 <BOOT_LoadImages+0x6d4>
30005064:	43d2      	mvns	r2, r2
30005066:	4291      	cmp	r1, r2
30005068:	d010      	beq.n	3000508c <BOOT_LoadImages+0x710>
3000506a:	483e      	ldr	r0, [pc, #248]	; (30005164 <BOOT_LoadImages+0x7e8>)
3000506c:	f007 f9b4 	bl	3000c3d8 <__DiagPrintf_veneer>
30005070:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005072:	2b00      	cmp	r3, #0
30005074:	f43f ae8d 	beq.w	30004d92 <BOOT_LoadImages+0x416>
30005078:	2300      	movs	r3, #0
3000507a:	9309      	str	r3, [sp, #36]	; 0x24
3000507c:	e4e7      	b.n	30004a4e <BOOT_LoadImages+0xd2>
3000507e:	460a      	mov	r2, r1
30005080:	4839      	ldr	r0, [pc, #228]	; (30005168 <BOOT_LoadImages+0x7ec>)
30005082:	f007 f9a9 	bl	3000c3d8 <__DiagPrintf_veneer>
30005086:	2201      	movs	r2, #1
30005088:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000508a:	e4fc      	b.n	30004a86 <BOOT_LoadImages+0x10a>
3000508c:	460a      	mov	r2, r1
3000508e:	4837      	ldr	r0, [pc, #220]	; (3000516c <BOOT_LoadImages+0x7f0>)
30005090:	f007 f9a2 	bl	3000c3d8 <__DiagPrintf_veneer>
30005094:	9b09      	ldr	r3, [sp, #36]	; 0x24
30005096:	2b00      	cmp	r3, #0
30005098:	f47f acd9 	bne.w	30004a4e <BOOT_LoadImages+0xd2>
3000509c:	461a      	mov	r2, r3
3000509e:	f106 6300 	add.w	r3, r6, #134217728	; 0x8000000
300050a2:	e4f3      	b.n	30004a8c <BOOT_LoadImages+0x110>
300050a4:	4832      	ldr	r0, [pc, #200]	; (30005170 <BOOT_LoadImages+0x7f4>)
300050a6:	f007 f997 	bl	3000c3d8 <__DiagPrintf_veneer>
300050aa:	e486      	b.n	300049ba <BOOT_LoadImages+0x3e>
300050ac:	4c2c      	ldr	r4, [pc, #176]	; (30005160 <BOOT_LoadImages+0x7e4>)
300050ae:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300050b2:	3310      	adds	r3, #16
300050b4:	930e      	str	r3, [sp, #56]	; 0x38
300050b6:	2301      	movs	r3, #1
300050b8:	9303      	str	r3, [sp, #12]
300050ba:	e5c3      	b.n	30004c44 <BOOT_LoadImages+0x2c8>
300050bc:	482d      	ldr	r0, [pc, #180]	; (30005174 <BOOT_LoadImages+0x7f8>)
300050be:	e65d      	b.n	30004d7c <BOOT_LoadImages+0x400>
300050c0:	482d      	ldr	r0, [pc, #180]	; (30005178 <BOOT_LoadImages+0x7fc>)
300050c2:	f007 f989 	bl	3000c3d8 <__DiagPrintf_veneer>
300050c6:	e478      	b.n	300049ba <BOOT_LoadImages+0x3e>
300050c8:	482c      	ldr	r0, [pc, #176]	; (3000517c <BOOT_LoadImages+0x800>)
300050ca:	f007 f985 	bl	3000c3d8 <__DiagPrintf_veneer>
300050ce:	e660      	b.n	30004d92 <BOOT_LoadImages+0x416>
300050d0:	4b2b      	ldr	r3, [pc, #172]	; (30005180 <BOOT_LoadImages+0x804>)
300050d2:	4650      	mov	r0, sl
300050d4:	4798      	blx	r3
300050d6:	1e03      	subs	r3, r0, #0
300050d8:	bf18      	it	ne
300050da:	2301      	movne	r3, #1
300050dc:	b110      	cbz	r0, 300050e4 <BOOT_LoadImages+0x768>
300050de:	2e00      	cmp	r6, #0
300050e0:	f47f adbe 	bne.w	30004c60 <BOOT_LoadImages+0x2e4>
300050e4:	2c00      	cmp	r4, #0
300050e6:	f43f af2f 	beq.w	30004f48 <BOOT_LoadImages+0x5cc>
300050ea:	2b00      	cmp	r3, #0
300050ec:	f47f af0f 	bne.w	30004f0e <BOOT_LoadImages+0x592>
300050f0:	e72a      	b.n	30004f48 <BOOT_LoadImages+0x5cc>
300050f2:	4824      	ldr	r0, [pc, #144]	; (30005184 <BOOT_LoadImages+0x808>)
300050f4:	f007 f970 	bl	3000c3d8 <__DiagPrintf_veneer>
300050f8:	e78a      	b.n	30005010 <BOOT_LoadImages+0x694>
300050fa:	4293      	cmp	r3, r2
300050fc:	f04f 0401 	mov.w	r4, #1
30005100:	464b      	mov	r3, r9
30005102:	4659      	mov	r1, fp
30005104:	bf28      	it	cs
30005106:	4630      	movcs	r0, r6
30005108:	9a07      	ldr	r2, [sp, #28]
3000510a:	bf38      	it	cc
3000510c:	4620      	movcc	r0, r4
3000510e:	9500      	str	r5, [sp, #0]
30005110:	9401      	str	r4, [sp, #4]
30005112:	46b0      	mov	r8, r6
30005114:	f7fe feda 	bl	30003ecc <rewrite_bp>
30005118:	4626      	mov	r6, r4
3000511a:	b2c3      	uxtb	r3, r0
3000511c:	f8df 9068 	ldr.w	r9, [pc, #104]	; 30005188 <BOOT_LoadImages+0x80c>
30005120:	46c2      	mov	sl, r8
30005122:	f8df b068 	ldr.w	fp, [pc, #104]	; 3000518c <BOOT_LoadImages+0x810>
30005126:	9303      	str	r3, [sp, #12]
30005128:	4b01      	ldr	r3, [pc, #4]	; (30005130 <BOOT_LoadImages+0x7b4>)
3000512a:	9304      	str	r3, [sp, #16]
3000512c:	e582      	b.n	30004c34 <BOOT_LoadImages+0x2b8>
3000512e:	bf00      	nop
30005130:	3000e6d7 	.word	0x3000e6d7
30005134:	3000c9a0 	.word	0x3000c9a0
30005138:	3000cbbc 	.word	0x3000cbbc
3000513c:	3000c790 	.word	0x3000c790
30005140:	00009be5 	.word	0x00009be5
30005144:	3000c9bc 	.word	0x3000c9bc
30005148:	3000c9d0 	.word	0x3000c9d0
3000514c:	3000cb0c 	.word	0x3000cb0c
30005150:	3000c7a8 	.word	0x3000c7a8
30005154:	3000c85c 	.word	0x3000c85c
30005158:	3000cbd8 	.word	0x3000cbd8
3000515c:	07ffe004 	.word	0x07ffe004
30005160:	3000e89c 	.word	0x3000e89c
30005164:	3000c8c8 	.word	0x3000c8c8
30005168:	3000c908 	.word	0x3000c908
3000516c:	3000c88c 	.word	0x3000c88c
30005170:	3000c7ec 	.word	0x3000c7ec
30005174:	3000cbf0 	.word	0x3000cbf0
30005178:	3000c800 	.word	0x3000c800
3000517c:	3000cc0c 	.word	0x3000cc0c
30005180:	30006c81 	.word	0x30006c81
30005184:	3000ca38 	.word	0x3000ca38
30005188:	3000fcf8 	.word	0x3000fcf8
3000518c:	30006c7d 	.word	0x30006c7d
30005190:	f8d4 6400 	ldr.w	r6, [r4, #1024]	; 0x400
30005194:	9b0d      	ldr	r3, [sp, #52]	; 0x34
30005196:	1d31      	adds	r1, r6, #4
30005198:	2b00      	cmp	r3, #0
3000519a:	f000 8123 	beq.w	300053e4 <BOOT_LoadImages+0xa68>
3000519e:	1d18      	adds	r0, r3, #4
300051a0:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300051a4:	4430      	add	r0, r6
300051a6:	f811 3b01 	ldrb.w	r3, [r1], #1
300051aa:	4053      	eors	r3, r2
300051ac:	4281      	cmp	r1, r0
300051ae:	b2db      	uxtb	r3, r3
300051b0:	f854 3023 	ldr.w	r3, [r4, r3, lsl #2]
300051b4:	ea83 2212 	eor.w	r2, r3, r2, lsr #8
300051b8:	d1f5      	bne.n	300051a6 <BOOT_LoadImages+0x82a>
300051ba:	43d2      	mvns	r2, r2
300051bc:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300051be:	4293      	cmp	r3, r2
300051c0:	f000 80f6 	beq.w	300053b0 <BOOT_LoadImages+0xa34>
300051c4:	f8d6 300a 	ldr.w	r3, [r6, #10]
300051c8:	3301      	adds	r3, #1
300051ca:	d07f      	beq.n	300052cc <BOOT_LoadImages+0x950>
300051cc:	990b      	ldr	r1, [sp, #44]	; 0x2c
300051ce:	48a1      	ldr	r0, [pc, #644]	; (30005454 <BOOT_LoadImages+0xad8>)
300051d0:	f007 f902 	bl	3000c3d8 <__DiagPrintf_veneer>
300051d4:	f8d4 1400 	ldr.w	r1, [r4, #1024]	; 0x400
300051d8:	2002      	movs	r0, #2
300051da:	f7fe fd7f 	bl	30003cdc <FLASH_Erase_With_Lock>
300051de:	e717      	b.n	30005010 <BOOT_LoadImages+0x694>
300051e0:	ab16      	add	r3, sp, #88	; 0x58
300051e2:	489d      	ldr	r0, [pc, #628]	; (30005458 <BOOT_LoadImages+0xadc>)
300051e4:	461e      	mov	r6, r3
300051e6:	ee08 3a10 	vmov	s16, r3
300051ea:	f007 f8f5 	bl	3000c3d8 <__DiagPrintf_veneer>
300051ee:	2280      	movs	r2, #128	; 0x80
300051f0:	21ff      	movs	r1, #255	; 0xff
300051f2:	4630      	mov	r0, r6
300051f4:	f007 f8c0 	bl	3000c378 <____wrap_memset_veneer>
300051f8:	2280      	movs	r2, #128	; 0x80
300051fa:	4629      	mov	r1, r5
300051fc:	4630      	mov	r0, r6
300051fe:	f007 f8cb 	bl	3000c398 <____wrap_memcpy_veneer>
30005202:	f1ba 0f00 	cmp.w	sl, #0
30005206:	d050      	beq.n	300052aa <BOOT_LoadImages+0x92e>
30005208:	46b8      	mov	r8, r7
3000520a:	e55d      	b.n	30004cc8 <BOOT_LoadImages+0x34c>
3000520c:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005210:	f8d3 200a 	ldr.w	r2, [r3, #10]
30005214:	3201      	adds	r2, #1
30005216:	d059      	beq.n	300052cc <BOOT_LoadImages+0x950>
30005218:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000521c:	488f      	ldr	r0, [pc, #572]	; (3000545c <BOOT_LoadImages+0xae0>)
3000521e:	f007 f8db 	bl	3000c3d8 <__DiagPrintf_veneer>
30005222:	9705      	str	r7, [sp, #20]
30005224:	f08a 0601 	eor.w	r6, sl, #1
30005228:	f04f 0801 	mov.w	r8, #1
3000522c:	f006 0601 	and.w	r6, r6, #1
30005230:	4647      	mov	r7, r8
30005232:	e73c      	b.n	300050ae <BOOT_LoadImages+0x732>
30005234:	ab16      	add	r3, sp, #88	; 0x58
30005236:	488a      	ldr	r0, [pc, #552]	; (30005460 <BOOT_LoadImages+0xae4>)
30005238:	461e      	mov	r6, r3
3000523a:	ee08 3a10 	vmov	s16, r3
3000523e:	f007 f8cb 	bl	3000c3d8 <__DiagPrintf_veneer>
30005242:	2280      	movs	r2, #128	; 0x80
30005244:	21ff      	movs	r1, #255	; 0xff
30005246:	4630      	mov	r0, r6
30005248:	f007 f896 	bl	3000c378 <____wrap_memset_veneer>
3000524c:	2280      	movs	r2, #128	; 0x80
3000524e:	4629      	mov	r1, r5
30005250:	4630      	mov	r0, r6
30005252:	f007 f8a1 	bl	3000c398 <____wrap_memcpy_veneer>
30005256:	f1ba 0f00 	cmp.w	sl, #0
3000525a:	d130      	bne.n	300052be <BOOT_LoadImages+0x942>
3000525c:	2301      	movs	r3, #1
3000525e:	9305      	str	r3, [sp, #20]
30005260:	46d0      	mov	r8, sl
30005262:	f04f 0a00 	mov.w	sl, #0
30005266:	4651      	mov	r1, sl
30005268:	f88d a064 	strb.w	sl, [sp, #100]	; 0x64
3000526c:	e533      	b.n	30004cd6 <BOOT_LoadImages+0x35a>
3000526e:	ab16      	add	r3, sp, #88	; 0x58
30005270:	2280      	movs	r2, #128	; 0x80
30005272:	21ff      	movs	r1, #255	; 0xff
30005274:	461e      	mov	r6, r3
30005276:	4618      	mov	r0, r3
30005278:	ee08 3a10 	vmov	s16, r3
3000527c:	f007 f87c 	bl	3000c378 <____wrap_memset_veneer>
30005280:	2280      	movs	r2, #128	; 0x80
30005282:	4629      	mov	r1, r5
30005284:	4630      	mov	r0, r6
30005286:	f007 f887 	bl	3000c398 <____wrap_memcpy_veneer>
3000528a:	f1ba 0f00 	cmp.w	sl, #0
3000528e:	f040 80d9 	bne.w	30005444 <BOOT_LoadImages+0xac8>
30005292:	b9bf      	cbnz	r7, 300052c4 <BOOT_LoadImages+0x948>
30005294:	46c2      	mov	sl, r8
30005296:	9705      	str	r7, [sp, #20]
30005298:	e009      	b.n	300052ae <BOOT_LoadImages+0x932>
3000529a:	486f      	ldr	r0, [pc, #444]	; (30005458 <BOOT_LoadImages+0xadc>)
3000529c:	f007 f89c 	bl	3000c3d8 <__DiagPrintf_veneer>
300052a0:	e4be      	b.n	30004c20 <BOOT_LoadImages+0x2a4>
300052a2:	486f      	ldr	r0, [pc, #444]	; (30005460 <BOOT_LoadImages+0xae4>)
300052a4:	f007 f898 	bl	3000c3d8 <__DiagPrintf_veneer>
300052a8:	e4ba      	b.n	30004c20 <BOOT_LoadImages+0x2a4>
300052aa:	2301      	movs	r3, #1
300052ac:	9305      	str	r3, [sp, #20]
300052ae:	46d0      	mov	r8, sl
300052b0:	2101      	movs	r1, #1
300052b2:	f04f 0a00 	mov.w	sl, #0
300052b6:	f88d 1064 	strb.w	r1, [sp, #100]	; 0x64
300052ba:	4657      	mov	r7, sl
300052bc:	e50b      	b.n	30004cd6 <BOOT_LoadImages+0x35a>
300052be:	f04f 0800 	mov.w	r8, #0
300052c2:	e4fe      	b.n	30004cc2 <BOOT_LoadImages+0x346>
300052c4:	f8cd a014 	str.w	sl, [sp, #20]
300052c8:	46c2      	mov	sl, r8
300052ca:	e7c9      	b.n	30005260 <BOOT_LoadImages+0x8e4>
300052cc:	4862      	ldr	r0, [pc, #392]	; (30005458 <BOOT_LoadImages+0xadc>)
300052ce:	f007 f883 	bl	3000c3d8 <__DiagPrintf_veneer>
300052d2:	e69d      	b.n	30005010 <BOOT_LoadImages+0x694>
300052d4:	4620      	mov	r0, r4
300052d6:	e5db      	b.n	30004e90 <BOOT_LoadImages+0x514>
300052d8:	45e2      	cmp	sl, ip
300052da:	f67f abed 	bls.w	30004ab8 <BOOT_LoadImages+0x13c>
300052de:	e432      	b.n	30004b46 <BOOT_LoadImages+0x1ca>
300052e0:	428a      	cmp	r2, r1
300052e2:	f4ff ad89 	bcc.w	30004df8 <BOOT_LoadImages+0x47c>
300052e6:	4282      	cmp	r2, r0
300052e8:	f4ff ad86 	bcc.w	30004df8 <BOOT_LoadImages+0x47c>
300052ec:	2601      	movs	r6, #1
300052ee:	464b      	mov	r3, r9
300052f0:	9a07      	ldr	r2, [sp, #28]
300052f2:	4659      	mov	r1, fp
300052f4:	4630      	mov	r0, r6
300052f6:	9500      	str	r5, [sp, #0]
300052f8:	9601      	str	r6, [sp, #4]
300052fa:	f7fe fde7 	bl	30003ecc <rewrite_bp>
300052fe:	f04f 0800 	mov.w	r8, #0
30005302:	b2c3      	uxtb	r3, r0
30005304:	f8df 9180 	ldr.w	r9, [pc, #384]	; 30005488 <BOOT_LoadImages+0xb0c>
30005308:	f8df b180 	ldr.w	fp, [pc, #384]	; 3000548c <BOOT_LoadImages+0xb10>
3000530c:	46c2      	mov	sl, r8
3000530e:	9303      	str	r3, [sp, #12]
30005310:	4b54      	ldr	r3, [pc, #336]	; (30005464 <BOOT_LoadImages+0xae8>)
30005312:	9304      	str	r3, [sp, #16]
30005314:	e48e      	b.n	30004c34 <BOOT_LoadImages+0x2b8>
30005316:	f8cd a014 	str.w	sl, [sp, #20]
3000531a:	4639      	mov	r1, r7
3000531c:	46ba      	mov	sl, r7
3000531e:	f88d 7064 	strb.w	r7, [sp, #100]	; 0x64
30005322:	e4d8      	b.n	30004cd6 <BOOT_LoadImages+0x35a>
30005324:	461a      	mov	r2, r3
30005326:	4619      	mov	r1, r3
30005328:	484f      	ldr	r0, [pc, #316]	; (30005468 <BOOT_LoadImages+0xaec>)
3000532a:	f04f 0801 	mov.w	r8, #1
3000532e:	f007 f853 	bl	3000c3d8 <__DiagPrintf_veneer>
30005332:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005336:	484d      	ldr	r0, [pc, #308]	; (3000546c <BOOT_LoadImages+0xaf0>)
30005338:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000533c:	f007 f84c 	bl	3000c3d8 <__DiagPrintf_veneer>
30005340:	2302      	movs	r3, #2
30005342:	9303      	str	r3, [sp, #12]
30005344:	e46c      	b.n	30004c20 <BOOT_LoadImages+0x2a4>
30005346:	461a      	mov	r2, r3
30005348:	4619      	mov	r1, r3
3000534a:	4849      	ldr	r0, [pc, #292]	; (30005470 <BOOT_LoadImages+0xaf4>)
3000534c:	f007 f844 	bl	3000c3d8 <__DiagPrintf_veneer>
30005350:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
30005354:	4841      	ldr	r0, [pc, #260]	; (3000545c <BOOT_LoadImages+0xae0>)
30005356:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000535a:	f007 f83d 	bl	3000c3d8 <__DiagPrintf_veneer>
3000535e:	2301      	movs	r3, #1
30005360:	4698      	mov	r8, r3
30005362:	9303      	str	r3, [sp, #12]
30005364:	e45c      	b.n	30004c20 <BOOT_LoadImages+0x2a4>
30005366:	461a      	mov	r2, r3
30005368:	4619      	mov	r1, r3
3000536a:	483f      	ldr	r0, [pc, #252]	; (30005468 <BOOT_LoadImages+0xaec>)
3000536c:	f007 f834 	bl	3000c3d8 <__DiagPrintf_veneer>
30005370:	f8d4 3404 	ldr.w	r3, [r4, #1028]	; 0x404
30005374:	483d      	ldr	r0, [pc, #244]	; (3000546c <BOOT_LoadImages+0xaf0>)
30005376:	f8d3 1006 	ldr.w	r1, [r3, #6]
3000537a:	f007 f82d 	bl	3000c3d8 <__DiagPrintf_veneer>
3000537e:	2300      	movs	r3, #0
30005380:	9305      	str	r3, [sp, #20]
30005382:	e4e7      	b.n	30004d54 <BOOT_LoadImages+0x3d8>
30005384:	483b      	ldr	r0, [pc, #236]	; (30005474 <BOOT_LoadImages+0xaf8>)
30005386:	f007 f827 	bl	3000c3d8 <__DiagPrintf_veneer>
3000538a:	e7fe      	b.n	3000538a <BOOT_LoadImages+0xa0e>
3000538c:	9a0c      	ldr	r2, [sp, #48]	; 0x30
3000538e:	e5f8      	b.n	30004f82 <BOOT_LoadImages+0x606>
30005390:	9a0d      	ldr	r2, [sp, #52]	; 0x34
30005392:	e42d      	b.n	30004bf0 <BOOT_LoadImages+0x274>
30005394:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
30005398:	f11a 0f03 	cmn.w	sl, #3
3000539c:	d824      	bhi.n	300053e8 <BOOT_LoadImages+0xa6c>
3000539e:	f1bb 0f01 	cmp.w	fp, #1
300053a2:	d937      	bls.n	30005414 <BOOT_LoadImages+0xa98>
300053a4:	4834      	ldr	r0, [pc, #208]	; (30005478 <BOOT_LoadImages+0xafc>)
300053a6:	e640      	b.n	3000502a <BOOT_LoadImages+0x6ae>
300053a8:	4c34      	ldr	r4, [pc, #208]	; (3000547c <BOOT_LoadImages+0xb00>)
300053aa:	e4d9      	b.n	30004d60 <BOOT_LoadImages+0x3e4>
300053ac:	9a0c      	ldr	r2, [sp, #48]	; 0x30
300053ae:	e61e      	b.n	30004fee <BOOT_LoadImages+0x672>
300053b0:	461a      	mov	r2, r3
300053b2:	4619      	mov	r1, r3
300053b4:	482e      	ldr	r0, [pc, #184]	; (30005470 <BOOT_LoadImages+0xaf4>)
300053b6:	f007 f80f 	bl	3000c3d8 <__DiagPrintf_veneer>
300053ba:	f8d4 3400 	ldr.w	r3, [r4, #1024]	; 0x400
300053be:	4827      	ldr	r0, [pc, #156]	; (3000545c <BOOT_LoadImages+0xae0>)
300053c0:	f8d3 1006 	ldr.w	r1, [r3, #6]
300053c4:	f007 f808 	bl	3000c3d8 <__DiagPrintf_veneer>
300053c8:	2300      	movs	r3, #0
300053ca:	9305      	str	r3, [sp, #20]
300053cc:	e72a      	b.n	30005224 <BOOT_LoadImages+0x8a8>
300053ce:	4281      	cmp	r1, r0
300053d0:	f4ff ad18 	bcc.w	30004e04 <BOOT_LoadImages+0x488>
300053d4:	2400      	movs	r4, #0
300053d6:	464b      	mov	r3, r9
300053d8:	4642      	mov	r2, r8
300053da:	4651      	mov	r1, sl
300053dc:	4620      	mov	r0, r4
300053de:	9500      	str	r5, [sp, #0]
300053e0:	9401      	str	r4, [sp, #4]
300053e2:	e555      	b.n	30004e90 <BOOT_LoadImages+0x514>
300053e4:	9a0d      	ldr	r2, [sp, #52]	; 0x34
300053e6:	e6e9      	b.n	300051bc <BOOT_LoadImages+0x840>
300053e8:	4825      	ldr	r0, [pc, #148]	; (30005480 <BOOT_LoadImages+0xb04>)
300053ea:	e61e      	b.n	3000502a <BOOT_LoadImages+0x6ae>
300053ec:	2601      	movs	r6, #1
300053ee:	464b      	mov	r3, r9
300053f0:	9a07      	ldr	r2, [sp, #28]
300053f2:	4659      	mov	r1, fp
300053f4:	2000      	movs	r0, #0
300053f6:	9500      	str	r5, [sp, #0]
300053f8:	9601      	str	r6, [sp, #4]
300053fa:	e77e      	b.n	300052fa <BOOT_LoadImages+0x97e>
300053fc:	2400      	movs	r4, #0
300053fe:	464b      	mov	r3, r9
30005400:	4642      	mov	r2, r8
30005402:	4651      	mov	r1, sl
30005404:	9500      	str	r5, [sp, #0]
30005406:	9401      	str	r4, [sp, #4]
30005408:	e541      	b.n	30004e8e <BOOT_LoadImages+0x512>
3000540a:	f08a 0601 	eor.w	r6, sl, #1
3000540e:	f006 0601 	and.w	r6, r6, #1
30005412:	e40f      	b.n	30004c34 <BOOT_LoadImages+0x2b8>
30005414:	9904      	ldr	r1, [sp, #16]
30005416:	481b      	ldr	r0, [pc, #108]	; (30005484 <BOOT_LoadImages+0xb08>)
30005418:	f006 ffde 	bl	3000c3d8 <__DiagPrintf_veneer>
3000541c:	f8d9 3011 	ldr.w	r3, [r9, #17]
30005420:	f8d9 b00d 	ldr.w	fp, [r9, #13]
30005424:	9307      	str	r3, [sp, #28]
30005426:	2301      	movs	r3, #1
30005428:	f8d5 a00d 	ldr.w	sl, [r5, #13]
3000542c:	f8d5 8011 	ldr.w	r8, [r5, #17]
30005430:	9309      	str	r3, [sp, #36]	; 0x24
30005432:	e607      	b.n	30005044 <BOOT_LoadImages+0x6c8>
30005434:	f08a 0601 	eor.w	r6, sl, #1
30005438:	f8cd 8014 	str.w	r8, [sp, #20]
3000543c:	f006 0601 	and.w	r6, r6, #1
30005440:	f7ff bbf8 	b.w	30004c34 <BOOT_LoadImages+0x2b8>
30005444:	f04f 0a00 	mov.w	sl, #0
30005448:	e43b      	b.n	30004cc2 <BOOT_LoadImages+0x346>
3000544a:	2601      	movs	r6, #1
3000544c:	4c0b      	ldr	r4, [pc, #44]	; (3000547c <BOOT_LoadImages+0xb00>)
3000544e:	46b2      	mov	sl, r6
30005450:	f7ff bbb4 	b.w	30004bbc <BOOT_LoadImages+0x240>
30005454:	3000ca84 	.word	0x3000ca84
30005458:	3000ca28 	.word	0x3000ca28
3000545c:	3000c778 	.word	0x3000c778
30005460:	3000ca38 	.word	0x3000ca38
30005464:	3000e6d7 	.word	0x3000e6d7
30005468:	3000cad0 	.word	0x3000cad0
3000546c:	3000c790 	.word	0x3000c790
30005470:	3000ca48 	.word	0x3000ca48
30005474:	3000cb90 	.word	0x3000cb90
30005478:	3000cbf0 	.word	0x3000cbf0
3000547c:	3000e89c 	.word	0x3000e89c
30005480:	3000c878 	.word	0x3000c878
30005484:	3000cc28 	.word	0x3000cc28
30005488:	3000fcf8 	.word	0x3000fcf8
3000548c:	30006c7d 	.word	0x30006c7d

30005490 <BOOT_ReasonSet>:
30005490:	4a03      	ldr	r2, [pc, #12]	; (300054a0 <BOOT_ReasonSet+0x10>)
30005492:	4904      	ldr	r1, [pc, #16]	; (300054a4 <BOOT_ReasonSet+0x14>)
30005494:	8c13      	ldrh	r3, [r2, #32]
30005496:	b29b      	uxth	r3, r3
30005498:	8413      	strh	r3, [r2, #32]
3000549a:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
3000549e:	4770      	bx	lr
300054a0:	42008000 	.word	0x42008000
300054a4:	42008200 	.word	0x42008200

300054a8 <BOOT_Enable_KM0>:
300054a8:	b510      	push	{r4, lr}
300054aa:	4c12      	ldr	r4, [pc, #72]	; (300054f4 <BOOT_Enable_KM0+0x4c>)
300054ac:	47a0      	blx	r4
300054ae:	b110      	cbz	r0, 300054b6 <BOOT_Enable_KM0+0xe>
300054b0:	4b11      	ldr	r3, [pc, #68]	; (300054f8 <BOOT_Enable_KM0+0x50>)
300054b2:	4798      	blx	r3
300054b4:	b1a8      	cbz	r0, 300054e2 <BOOT_Enable_KM0+0x3a>
300054b6:	47a0      	blx	r4
300054b8:	b940      	cbnz	r0, 300054cc <BOOT_Enable_KM0+0x24>
300054ba:	4a10      	ldr	r2, [pc, #64]	; (300054fc <BOOT_Enable_KM0+0x54>)
300054bc:	f8d2 3268 	ldr.w	r3, [r2, #616]	; 0x268
300054c0:	f043 7300 	orr.w	r3, r3, #33554432	; 0x2000000
300054c4:	f8c2 3268 	str.w	r3, [r2, #616]	; 0x268
300054c8:	bf40      	sev
300054ca:	bd10      	pop	{r4, pc}
300054cc:	4b0a      	ldr	r3, [pc, #40]	; (300054f8 <BOOT_Enable_KM0+0x50>)
300054ce:	4798      	blx	r3
300054d0:	2800      	cmp	r0, #0
300054d2:	d0f2      	beq.n	300054ba <BOOT_Enable_KM0+0x12>
300054d4:	4b0a      	ldr	r3, [pc, #40]	; (30005500 <BOOT_Enable_KM0+0x58>)
300054d6:	2004      	movs	r0, #4
300054d8:	4a0a      	ldr	r2, [pc, #40]	; (30005504 <BOOT_Enable_KM0+0x5c>)
300054da:	4619      	mov	r1, r3
300054dc:	f006 fd06 	bl	3000beec <rtk_log_write>
300054e0:	e7eb      	b.n	300054ba <BOOT_Enable_KM0+0x12>
300054e2:	4b07      	ldr	r3, [pc, #28]	; (30005500 <BOOT_Enable_KM0+0x58>)
300054e4:	2004      	movs	r0, #4
300054e6:	4a08      	ldr	r2, [pc, #32]	; (30005508 <BOOT_Enable_KM0+0x60>)
300054e8:	4619      	mov	r1, r3
300054ea:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
300054ee:	f006 bcfd 	b.w	3000beec <rtk_log_write>
300054f2:	bf00      	nop
300054f4:	0000c09d 	.word	0x0000c09d
300054f8:	0000c0ad 	.word	0x0000c0ad
300054fc:	42008000 	.word	0x42008000
30005500:	3000c6b0 	.word	0x3000c6b0
30005504:	3000cc6c 	.word	0x3000cc6c
30005508:	3000cc44 	.word	0x3000cc44

3000550c <BOOT_AP_Clk_Get>:
3000550c:	4a06      	ldr	r2, [pc, #24]	; (30005528 <BOOT_AP_Clk_Get+0x1c>)
3000550e:	6913      	ldr	r3, [r2, #16]
30005510:	2b00      	cmp	r3, #0
30005512:	db03      	blt.n	3000551c <BOOT_AP_Clk_Get+0x10>
30005514:	6810      	ldr	r0, [r2, #0]
30005516:	fbb0 f0f3 	udiv	r0, r0, r3
3000551a:	4770      	bx	lr
3000551c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30005520:	6850      	ldr	r0, [r2, #4]
30005522:	fbb0 f0f3 	udiv	r0, r0, r3
30005526:	4770      	bx	lr
30005528:	3000ef24 	.word	0x3000ef24

3000552c <BOOT_SOC_ClkSet>:
3000552c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005530:	4c5d      	ldr	r4, [pc, #372]	; (300056a8 <BOOT_SOC_ClkSet+0x17c>)
30005532:	b083      	sub	sp, #12
30005534:	6926      	ldr	r6, [r4, #16]
30005536:	6825      	ldr	r5, [r4, #0]
30005538:	2e00      	cmp	r6, #0
3000553a:	db7a      	blt.n	30005632 <BOOT_SOC_ClkSet+0x106>
3000553c:	fbb5 f6f6 	udiv	r6, r5, r6
30005540:	4b5a      	ldr	r3, [pc, #360]	; (300056ac <BOOT_SOC_ClkSet+0x180>)
30005542:	68a7      	ldr	r7, [r4, #8]
30005544:	fba3 1305 	umull	r1, r3, r3, r5
30005548:	68e2      	ldr	r2, [r4, #12]
3000554a:	2f01      	cmp	r7, #1
3000554c:	ea4f 1ad3 	mov.w	sl, r3, lsr #7
30005550:	ea4f 1b53 	mov.w	fp, r3, lsr #5
30005554:	ea4f 1393 	mov.w	r3, r3, lsr #6
30005558:	f10a 3aff 	add.w	sl, sl, #4294967295	; 0xffffffff
3000555c:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005560:	f103 39ff 	add.w	r9, r3, #4294967295	; 0xffffffff
30005564:	fbb5 f5f2 	udiv	r5, r5, r2
30005568:	d06f      	beq.n	3000564a <BOOT_SOC_ClkSet+0x11e>
3000556a:	4b51      	ldr	r3, [pc, #324]	; (300056b0 <BOOT_SOC_ClkSet+0x184>)
3000556c:	2200      	movs	r2, #0
3000556e:	2dfa      	cmp	r5, #250	; 0xfa
30005570:	f883 210c 	strb.w	r2, [r3, #268]	; 0x10c
30005574:	d863      	bhi.n	3000563e <BOOT_SOC_ClkSet+0x112>
30005576:	4f4f      	ldr	r7, [pc, #316]	; (300056b4 <BOOT_SOC_ClkSet+0x188>)
30005578:	783b      	ldrb	r3, [r7, #0]
3000557a:	2b01      	cmp	r3, #1
3000557c:	d103      	bne.n	30005586 <BOOT_SOC_ClkSet+0x5a>
3000557e:	f5b6 7f66 	cmp.w	r6, #920	; 0x398
30005582:	f200 8085 	bhi.w	30005690 <BOOT_SOC_ClkSet+0x164>
30005586:	2001      	movs	r0, #1
30005588:	f8df 8154 	ldr.w	r8, [pc, #340]	; 300056e0 <BOOT_SOC_ClkSet+0x1b4>
3000558c:	f002 fc6c 	bl	30007e68 <CLK_SWITCH_XTAL>
30005590:	6820      	ldr	r0, [r4, #0]
30005592:	4b49      	ldr	r3, [pc, #292]	; (300056b8 <BOOT_SOC_ClkSet+0x18c>)
30005594:	ea4f 2a0a 	mov.w	sl, sl, lsl #8
30005598:	fb08 f000 	mul.w	r0, r8, r0
3000559c:	ea4f 3b0b 	mov.w	fp, fp, lsl #12
300055a0:	4798      	blx	r3
300055a2:	4a46      	ldr	r2, [pc, #280]	; (300056bc <BOOT_SOC_ClkSet+0x190>)
300055a4:	68e1      	ldr	r1, [r4, #12]
300055a6:	f40a 6ae0 	and.w	sl, sl, #1792	; 0x700
300055aa:	f8d2 0228 	ldr.w	r0, [r2, #552]	; 0x228
300055ae:	ea4f 5909 	mov.w	r9, r9, lsl #20
300055b2:	1e4b      	subs	r3, r1, #1
300055b4:	4942      	ldr	r1, [pc, #264]	; (300056c0 <BOOT_SOC_ClkSet+0x194>)
300055b6:	fa1f fb8b 	uxth.w	fp, fp
300055ba:	f409 09e0 	and.w	r9, r9, #7340032	; 0x700000
300055be:	4001      	ands	r1, r0
300055c0:	f003 0307 	and.w	r3, r3, #7
300055c4:	2000      	movs	r0, #0
300055c6:	430b      	orrs	r3, r1
300055c8:	ea4a 0303 	orr.w	r3, sl, r3
300055cc:	ea4b 0303 	orr.w	r3, fp, r3
300055d0:	ea49 0303 	orr.w	r3, r9, r3
300055d4:	f8c2 3228 	str.w	r3, [r2, #552]	; 0x228
300055d8:	f002 fc46 	bl	30007e68 <CLK_SWITCH_XTAL>
300055dc:	4b39      	ldr	r3, [pc, #228]	; (300056c4 <BOOT_SOC_ClkSet+0x198>)
300055de:	4a3a      	ldr	r2, [pc, #232]	; (300056c8 <BOOT_SOC_ClkSet+0x19c>)
300055e0:	2004      	movs	r0, #4
300055e2:	4619      	mov	r1, r3
300055e4:	9500      	str	r5, [sp, #0]
300055e6:	f006 fc81 	bl	3000beec <rtk_log_write>
300055ea:	783b      	ldrb	r3, [r7, #0]
300055ec:	b1f3      	cbz	r3, 3000562c <BOOT_SOC_ClkSet+0x100>
300055ee:	6923      	ldr	r3, [r4, #16]
300055f0:	2b00      	cmp	r3, #0
300055f2:	db40      	blt.n	30005676 <BOOT_SOC_ClkSet+0x14a>
300055f4:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300055f8:	492d      	ldr	r1, [pc, #180]	; (300056b0 <BOOT_SOC_ClkSet+0x184>)
300055fa:	2000      	movs	r0, #0
300055fc:	6913      	ldr	r3, [r2, #16]
300055fe:	f881 010d 	strb.w	r0, [r1, #269]	; 0x10d
30005602:	f043 030c 	orr.w	r3, r3, #12
30005606:	6113      	str	r3, [r2, #16]
30005608:	f04f 4582 	mov.w	r5, #1090519040	; 0x41000000
3000560c:	6921      	ldr	r1, [r4, #16]
3000560e:	4b2d      	ldr	r3, [pc, #180]	; (300056c4 <BOOT_SOC_ClkSet+0x198>)
30005610:	6928      	ldr	r0, [r5, #16]
30005612:	3901      	subs	r1, #1
30005614:	4a2d      	ldr	r2, [pc, #180]	; (300056cc <BOOT_SOC_ClkSet+0x1a0>)
30005616:	f020 0003 	bic.w	r0, r0, #3
3000561a:	f001 0103 	and.w	r1, r1, #3
3000561e:	4301      	orrs	r1, r0
30005620:	2004      	movs	r0, #4
30005622:	6129      	str	r1, [r5, #16]
30005624:	4619      	mov	r1, r3
30005626:	9600      	str	r6, [sp, #0]
30005628:	f006 fc60 	bl	3000beec <rtk_log_write>
3000562c:	b003      	add	sp, #12
3000562e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005632:	f026 4600 	bic.w	r6, r6, #2147483648	; 0x80000000
30005636:	6863      	ldr	r3, [r4, #4]
30005638:	fbb3 f6f6 	udiv	r6, r3, r6
3000563c:	e780      	b.n	30005540 <BOOT_SOC_ClkSet+0x14>
3000563e:	f240 61bd 	movw	r1, #1725	; 0x6bd
30005642:	4823      	ldr	r0, [pc, #140]	; (300056d0 <BOOT_SOC_ClkSet+0x1a4>)
30005644:	f006 ff48 	bl	3000c4d8 <__io_assert_failed_veneer>
30005648:	e795      	b.n	30005576 <BOOT_SOC_ClkSet+0x4a>
3000564a:	4b22      	ldr	r3, [pc, #136]	; (300056d4 <BOOT_SOC_ClkSet+0x1a8>)
3000564c:	4638      	mov	r0, r7
3000564e:	4798      	blx	r3
30005650:	4b17      	ldr	r3, [pc, #92]	; (300056b0 <BOOT_SOC_ClkSet+0x184>)
30005652:	f5b5 7fa7 	cmp.w	r5, #334	; 0x14e
30005656:	f883 710c 	strb.w	r7, [r3, #268]	; 0x10c
3000565a:	d21f      	bcs.n	3000569c <BOOT_SOC_ClkSet+0x170>
3000565c:	4f15      	ldr	r7, [pc, #84]	; (300056b4 <BOOT_SOC_ClkSet+0x188>)
3000565e:	783b      	ldrb	r3, [r7, #0]
30005660:	2b01      	cmp	r3, #1
30005662:	d190      	bne.n	30005586 <BOOT_SOC_ClkSet+0x5a>
30005664:	f5b6 6f96 	cmp.w	r6, #1200	; 0x4b0
30005668:	d98d      	bls.n	30005586 <BOOT_SOC_ClkSet+0x5a>
3000566a:	f240 61b9 	movw	r1, #1721	; 0x6b9
3000566e:	4818      	ldr	r0, [pc, #96]	; (300056d0 <BOOT_SOC_ClkSet+0x1a4>)
30005670:	f006 ff32 	bl	3000c4d8 <__io_assert_failed_veneer>
30005674:	e787      	b.n	30005586 <BOOT_SOC_ClkSet+0x5a>
30005676:	6860      	ldr	r0, [r4, #4]
30005678:	4b17      	ldr	r3, [pc, #92]	; (300056d8 <BOOT_SOC_ClkSet+0x1ac>)
3000567a:	fb08 f000 	mul.w	r0, r8, r0
3000567e:	4798      	blx	r3
30005680:	4b16      	ldr	r3, [pc, #88]	; (300056dc <BOOT_SOC_ClkSet+0x1b0>)
30005682:	2001      	movs	r0, #1
30005684:	4798      	blx	r3
30005686:	4b0a      	ldr	r3, [pc, #40]	; (300056b0 <BOOT_SOC_ClkSet+0x184>)
30005688:	2201      	movs	r2, #1
3000568a:	f883 210d 	strb.w	r2, [r3, #269]	; 0x10d
3000568e:	e7bb      	b.n	30005608 <BOOT_SOC_ClkSet+0xdc>
30005690:	f240 61bf 	movw	r1, #1727	; 0x6bf
30005694:	480e      	ldr	r0, [pc, #56]	; (300056d0 <BOOT_SOC_ClkSet+0x1a4>)
30005696:	f006 ff1f 	bl	3000c4d8 <__io_assert_failed_veneer>
3000569a:	e774      	b.n	30005586 <BOOT_SOC_ClkSet+0x5a>
3000569c:	f240 61b7 	movw	r1, #1719	; 0x6b7
300056a0:	480b      	ldr	r0, [pc, #44]	; (300056d0 <BOOT_SOC_ClkSet+0x1a4>)
300056a2:	f006 ff19 	bl	3000c4d8 <__io_assert_failed_veneer>
300056a6:	e7d9      	b.n	3000565c <BOOT_SOC_ClkSet+0x130>
300056a8:	3000ef24 	.word	0x3000ef24
300056ac:	51eb851f 	.word	0x51eb851f
300056b0:	23020000 	.word	0x23020000
300056b4:	3000e6d7 	.word	0x3000e6d7
300056b8:	30008a91 	.word	0x30008a91
300056bc:	42008000 	.word	0x42008000
300056c0:	ff8f08f8 	.word	0xff8f08f8
300056c4:	3000c6b0 	.word	0x3000c6b0
300056c8:	3000cc8c 	.word	0x3000cc8c
300056cc:	3000cca4 	.word	0x3000cca4
300056d0:	3000e39c 	.word	0x3000e39c
300056d4:	3000b86d 	.word	0x3000b86d
300056d8:	30008b11 	.word	0x30008b11
300056dc:	30008b51 	.word	0x30008b51
300056e0:	000f4240 	.word	0x000f4240

300056e4 <BOOT_Disable_AP>:
300056e4:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
300056e8:	4906      	ldr	r1, [pc, #24]	; (30005704 <BOOT_Disable_AP+0x20>)
300056ea:	685a      	ldr	r2, [r3, #4]
300056ec:	f042 0202 	orr.w	r2, r2, #2
300056f0:	605a      	str	r2, [r3, #4]
300056f2:	685a      	ldr	r2, [r3, #4]
300056f4:	f042 0230 	orr.w	r2, r2, #48	; 0x30
300056f8:	605a      	str	r2, [r3, #4]
300056fa:	6f8b      	ldr	r3, [r1, #120]	; 0x78
300056fc:	f043 0303 	orr.w	r3, r3, #3
30005700:	678b      	str	r3, [r1, #120]	; 0x78
30005702:	4770      	bx	lr
30005704:	41000200 	.word	0x41000200

30005708 <BOOT_Enable_AP>:
30005708:	b538      	push	{r3, r4, r5, lr}
3000570a:	f04f 4482 	mov.w	r4, #1090519040	; 0x41000000
3000570e:	2201      	movs	r2, #1
30005710:	2104      	movs	r1, #4
30005712:	4b2b      	ldr	r3, [pc, #172]	; (300057c0 <BOOT_Enable_AP+0xb8>)
30005714:	482b      	ldr	r0, [pc, #172]	; (300057c4 <BOOT_Enable_AP+0xbc>)
30005716:	4798      	blx	r3
30005718:	6863      	ldr	r3, [r4, #4]
3000571a:	4d2b      	ldr	r5, [pc, #172]	; (300057c8 <BOOT_Enable_AP+0xc0>)
3000571c:	2032      	movs	r0, #50	; 0x32
3000571e:	f043 0302 	orr.w	r3, r3, #2
30005722:	6063      	str	r3, [r4, #4]
30005724:	6863      	ldr	r3, [r4, #4]
30005726:	f043 0330 	orr.w	r3, r3, #48	; 0x30
3000572a:	6063      	str	r3, [r4, #4]
3000572c:	6823      	ldr	r3, [r4, #0]
3000572e:	f043 0302 	orr.w	r3, r3, #2
30005732:	6023      	str	r3, [r4, #0]
30005734:	47a8      	blx	r5
30005736:	6823      	ldr	r3, [r4, #0]
30005738:	2032      	movs	r0, #50	; 0x32
3000573a:	f043 0304 	orr.w	r3, r3, #4
3000573e:	6023      	str	r3, [r4, #0]
30005740:	6823      	ldr	r3, [r4, #0]
30005742:	f043 0301 	orr.w	r3, r3, #1
30005746:	6023      	str	r3, [r4, #0]
30005748:	6823      	ldr	r3, [r4, #0]
3000574a:	f043 0310 	orr.w	r3, r3, #16
3000574e:	6023      	str	r3, [r4, #0]
30005750:	47a8      	blx	r5
30005752:	6823      	ldr	r3, [r4, #0]
30005754:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
30005758:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000575c:	6023      	str	r3, [r4, #0]
3000575e:	47a8      	blx	r5
30005760:	6863      	ldr	r3, [r4, #4]
30005762:	4a1a      	ldr	r2, [pc, #104]	; (300057cc <BOOT_Enable_AP+0xc4>)
30005764:	f023 0310 	bic.w	r3, r3, #16
30005768:	6063      	str	r3, [r4, #4]
3000576a:	6863      	ldr	r3, [r4, #4]
3000576c:	f023 0302 	bic.w	r3, r3, #2
30005770:	6063      	str	r3, [r4, #4]
30005772:	4790      	blx	r2
30005774:	b178      	cbz	r0, 30005796 <BOOT_Enable_AP+0x8e>
30005776:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000577a:	4815      	ldr	r0, [pc, #84]	; (300057d0 <BOOT_Enable_AP+0xc8>)
3000577c:	4a15      	ldr	r2, [pc, #84]	; (300057d4 <BOOT_Enable_AP+0xcc>)
3000577e:	68d9      	ldr	r1, [r3, #12]
30005780:	f041 0101 	orr.w	r1, r1, #1
30005784:	60d9      	str	r1, [r3, #12]
30005786:	6899      	ldr	r1, [r3, #8]
30005788:	f041 0101 	orr.w	r1, r1, #1
3000578c:	6099      	str	r1, [r3, #8]
3000578e:	6843      	ldr	r3, [r0, #4]
30005790:	431a      	orrs	r2, r3
30005792:	6042      	str	r2, [r0, #4]
30005794:	bd38      	pop	{r3, r4, r5, pc}
30005796:	4604      	mov	r4, r0
30005798:	4d0f      	ldr	r5, [pc, #60]	; (300057d8 <BOOT_Enable_AP+0xd0>)
3000579a:	4601      	mov	r1, r0
3000579c:	2041      	movs	r0, #65	; 0x41
3000579e:	47a8      	blx	r5
300057a0:	4621      	mov	r1, r4
300057a2:	2042      	movs	r0, #66	; 0x42
300057a4:	47a8      	blx	r5
300057a6:	4621      	mov	r1, r4
300057a8:	2043      	movs	r0, #67	; 0x43
300057aa:	47a8      	blx	r5
300057ac:	4621      	mov	r1, r4
300057ae:	2044      	movs	r0, #68	; 0x44
300057b0:	47a8      	blx	r5
300057b2:	4621      	mov	r1, r4
300057b4:	2045      	movs	r0, #69	; 0x45
300057b6:	47a8      	blx	r5
300057b8:	4621      	mov	r1, r4
300057ba:	2046      	movs	r0, #70	; 0x46
300057bc:	47a8      	blx	r5
300057be:	e7da      	b.n	30005776 <BOOT_Enable_AP+0x6e>
300057c0:	0000aab9 	.word	0x0000aab9
300057c4:	4200c000 	.word	0x4200c000
300057c8:	00009b2d 	.word	0x00009b2d
300057cc:	30009bfd 	.word	0x30009bfd
300057d0:	41000200 	.word	0x41000200
300057d4:	01001111 	.word	0x01001111
300057d8:	0000b20d 	.word	0x0000b20d

300057dc <BOOT_RAM_KeyDeriveFunc>:
300057dc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300057e0:	ed2d 8b04 	vpush	{d8-d9}
300057e4:	b0cb      	sub	sp, #300	; 0x12c
300057e6:	4c6a      	ldr	r4, [pc, #424]	; (30005990 <BOOT_RAM_KeyDeriveFunc+0x1b4>)
300057e8:	4680      	mov	r8, r0
300057ea:	4689      	mov	r9, r1
300057ec:	4869      	ldr	r0, [pc, #420]	; (30005994 <BOOT_RAM_KeyDeriveFunc+0x1b8>)
300057ee:	ee08 2a90 	vmov	s17, r2
300057f2:	4969      	ldr	r1, [pc, #420]	; (30005998 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
300057f4:	2201      	movs	r2, #1
300057f6:	ee09 3a10 	vmov	s18, r3
300057fa:	f8dd a160 	ldr.w	sl, [sp, #352]	; 0x160
300057fe:	47a0      	blx	r4
30005800:	2201      	movs	r2, #1
30005802:	4966      	ldr	r1, [pc, #408]	; (3000599c <BOOT_RAM_KeyDeriveFunc+0x1c0>)
30005804:	ae02      	add	r6, sp, #8
30005806:	4864      	ldr	r0, [pc, #400]	; (30005998 <BOOT_RAM_KeyDeriveFunc+0x1bc>)
30005808:	47a0      	blx	r4
3000580a:	4b65      	ldr	r3, [pc, #404]	; (300059a0 <BOOT_RAM_KeyDeriveFunc+0x1c4>)
3000580c:	2000      	movs	r0, #0
3000580e:	4798      	blx	r3
30005810:	22e0      	movs	r2, #224	; 0xe0
30005812:	2100      	movs	r1, #0
30005814:	a812      	add	r0, sp, #72	; 0x48
30005816:	f006 fdaf 	bl	3000c378 <____wrap_memset_veneer>
3000581a:	2300      	movs	r3, #0
3000581c:	f8ad 3004 	strh.w	r3, [sp, #4]
30005820:	f88d 3006 	strb.w	r3, [sp, #6]
30005824:	2301      	movs	r3, #1
30005826:	f88d 3007 	strb.w	r3, [sp, #7]
3000582a:	9b59      	ldr	r3, [sp, #356]	; 0x164
3000582c:	2b00      	cmp	r3, #0
3000582e:	f000 80ab 	beq.w	30005988 <BOOT_RAM_KeyDeriveFunc+0x1ac>
30005832:	ab0a      	add	r3, sp, #40	; 0x28
30005834:	f8df b17c 	ldr.w	fp, [pc, #380]	; 300059b4 <BOOT_RAM_KeyDeriveFunc+0x1d8>
30005838:	4f5a      	ldr	r7, [pc, #360]	; (300059a4 <BOOT_RAM_KeyDeriveFunc+0x1c8>)
3000583a:	ee08 3a10 	vmov	s16, r3
3000583e:	ab12      	add	r3, sp, #72	; 0x48
30005840:	464a      	mov	r2, r9
30005842:	4641      	mov	r1, r8
30005844:	2020      	movs	r0, #32
30005846:	47d8      	blx	fp
30005848:	aa12      	add	r2, sp, #72	; 0x48
3000584a:	ee19 1a10 	vmov	r1, s18
3000584e:	ee18 0a90 	vmov	r0, s17
30005852:	47b8      	blx	r7
30005854:	ee18 5a10 	vmov	r5, s16
30005858:	aa12      	add	r2, sp, #72	; 0x48
3000585a:	2104      	movs	r1, #4
3000585c:	a801      	add	r0, sp, #4
3000585e:	4634      	mov	r4, r6
30005860:	47b8      	blx	r7
30005862:	a912      	add	r1, sp, #72	; 0x48
30005864:	4b50      	ldr	r3, [pc, #320]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
30005866:	ee18 0a10 	vmov	r0, s16
3000586a:	4798      	blx	r3
3000586c:	f1ba 0f01 	cmp.w	sl, #1
30005870:	cd0f      	ldmia	r5!, {r0, r1, r2, r3}
30005872:	c40f      	stmia	r4!, {r0, r1, r2, r3}
30005874:	e895 000f 	ldmia.w	r5, {r0, r1, r2, r3}
30005878:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
3000587c:	d91f      	bls.n	300058be <BOOT_RAM_KeyDeriveFunc+0xe2>
3000587e:	2501      	movs	r5, #1
30005880:	f10d 0447 	add.w	r4, sp, #71	; 0x47
30005884:	ab12      	add	r3, sp, #72	; 0x48
30005886:	4641      	mov	r1, r8
30005888:	464a      	mov	r2, r9
3000588a:	2020      	movs	r0, #32
3000588c:	47d8      	blx	fp
3000588e:	aa12      	add	r2, sp, #72	; 0x48
30005890:	2120      	movs	r1, #32
30005892:	4630      	mov	r0, r6
30005894:	47b8      	blx	r7
30005896:	a912      	add	r1, sp, #72	; 0x48
30005898:	4b43      	ldr	r3, [pc, #268]	; (300059a8 <BOOT_RAM_KeyDeriveFunc+0x1cc>)
3000589a:	4630      	mov	r0, r6
3000589c:	4798      	blx	r3
3000589e:	f10d 0327 	add.w	r3, sp, #39	; 0x27
300058a2:	f10d 0107 	add.w	r1, sp, #7
300058a6:	f813 2f01 	ldrb.w	r2, [r3, #1]!
300058aa:	f811 0f01 	ldrb.w	r0, [r1, #1]!
300058ae:	42a3      	cmp	r3, r4
300058b0:	ea82 0200 	eor.w	r2, r2, r0
300058b4:	701a      	strb	r2, [r3, #0]
300058b6:	d1f6      	bne.n	300058a6 <BOOT_RAM_KeyDeriveFunc+0xca>
300058b8:	3501      	adds	r5, #1
300058ba:	45aa      	cmp	sl, r5
300058bc:	d1e2      	bne.n	30005884 <BOOT_RAM_KeyDeriveFunc+0xa8>
300058be:	9a59      	ldr	r2, [sp, #356]	; 0x164
300058c0:	ee18 1a10 	vmov	r1, s16
300058c4:	9b59      	ldr	r3, [sp, #356]	; 0x164
300058c6:	2a20      	cmp	r2, #32
300058c8:	985a      	ldr	r0, [sp, #360]	; 0x168
300058ca:	bf28      	it	cs
300058cc:	2220      	movcs	r2, #32
300058ce:	1a9b      	subs	r3, r3, r2
300058d0:	9359      	str	r3, [sp, #356]	; 0x164
300058d2:	4603      	mov	r3, r0
300058d4:	4413      	add	r3, r2
300058d6:	935a      	str	r3, [sp, #360]	; 0x168
300058d8:	f006 fd5e 	bl	3000c398 <____wrap_memcpy_veneer>
300058dc:	f10d 0207 	add.w	r2, sp, #7
300058e0:	2104      	movs	r1, #4
300058e2:	7813      	ldrb	r3, [r2, #0]
300058e4:	3901      	subs	r1, #1
300058e6:	3301      	adds	r3, #1
300058e8:	b2db      	uxtb	r3, r3
300058ea:	f802 3901 	strb.w	r3, [r2], #-1
300058ee:	b90b      	cbnz	r3, 300058f4 <BOOT_RAM_KeyDeriveFunc+0x118>
300058f0:	2900      	cmp	r1, #0
300058f2:	d1f6      	bne.n	300058e2 <BOOT_RAM_KeyDeriveFunc+0x106>
300058f4:	9b59      	ldr	r3, [sp, #356]	; 0x164
300058f6:	2b00      	cmp	r3, #0
300058f8:	d1a1      	bne.n	3000583e <BOOT_RAM_KeyDeriveFunc+0x62>
300058fa:	2220      	movs	r2, #32
300058fc:	2100      	movs	r1, #0
300058fe:	4c2b      	ldr	r4, [pc, #172]	; (300059ac <BOOT_RAM_KeyDeriveFunc+0x1d0>)
30005900:	4630      	mov	r0, r6
30005902:	47a0      	blx	r4
30005904:	2220      	movs	r2, #32
30005906:	2100      	movs	r1, #0
30005908:	ee18 0a10 	vmov	r0, s16
3000590c:	47a0      	blx	r4
3000590e:	4b28      	ldr	r3, [pc, #160]	; (300059b0 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005910:	695b      	ldr	r3, [r3, #20]
30005912:	03da      	lsls	r2, r3, #15
30005914:	d512      	bpl.n	3000593c <BOOT_RAM_KeyDeriveFunc+0x160>
30005916:	f016 021f 	ands.w	r2, r6, #31
3000591a:	4633      	mov	r3, r6
3000591c:	d130      	bne.n	30005980 <BOOT_RAM_KeyDeriveFunc+0x1a4>
3000591e:	2220      	movs	r2, #32
30005920:	f3bf 8f4f 	dsb	sy
30005924:	4822      	ldr	r0, [pc, #136]	; (300059b0 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005926:	441a      	add	r2, r3
30005928:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
3000592c:	3320      	adds	r3, #32
3000592e:	1ad1      	subs	r1, r2, r3
30005930:	2900      	cmp	r1, #0
30005932:	dcf9      	bgt.n	30005928 <BOOT_RAM_KeyDeriveFunc+0x14c>
30005934:	f3bf 8f4f 	dsb	sy
30005938:	f3bf 8f6f 	isb	sy
3000593c:	4b1c      	ldr	r3, [pc, #112]	; (300059b0 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
3000593e:	695b      	ldr	r3, [r3, #20]
30005940:	03db      	lsls	r3, r3, #15
30005942:	d513      	bpl.n	3000596c <BOOT_RAM_KeyDeriveFunc+0x190>
30005944:	ee18 3a10 	vmov	r3, s16
30005948:	f013 021f 	ands.w	r2, r3, #31
3000594c:	d114      	bne.n	30005978 <BOOT_RAM_KeyDeriveFunc+0x19c>
3000594e:	2220      	movs	r2, #32
30005950:	f3bf 8f4f 	dsb	sy
30005954:	4816      	ldr	r0, [pc, #88]	; (300059b0 <BOOT_RAM_KeyDeriveFunc+0x1d4>)
30005956:	441a      	add	r2, r3
30005958:	f8c0 3270 	str.w	r3, [r0, #624]	; 0x270
3000595c:	3320      	adds	r3, #32
3000595e:	1ad1      	subs	r1, r2, r3
30005960:	2900      	cmp	r1, #0
30005962:	dcf9      	bgt.n	30005958 <BOOT_RAM_KeyDeriveFunc+0x17c>
30005964:	f3bf 8f4f 	dsb	sy
30005968:	f3bf 8f6f 	isb	sy
3000596c:	2000      	movs	r0, #0
3000596e:	b04b      	add	sp, #300	; 0x12c
30005970:	ecbd 8b04 	vpop	{d8-d9}
30005974:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005978:	f023 031f 	bic.w	r3, r3, #31
3000597c:	3220      	adds	r2, #32
3000597e:	e7e7      	b.n	30005950 <BOOT_RAM_KeyDeriveFunc+0x174>
30005980:	f026 031f 	bic.w	r3, r6, #31
30005984:	3220      	adds	r2, #32
30005986:	e7cb      	b.n	30005920 <BOOT_RAM_KeyDeriveFunc+0x144>
30005988:	ab0a      	add	r3, sp, #40	; 0x28
3000598a:	ee08 3a10 	vmov	s16, r3
3000598e:	e7b4      	b.n	300058fa <BOOT_RAM_KeyDeriveFunc+0x11e>
30005990:	0000b479 	.word	0x0000b479
30005994:	40000008 	.word	0x40000008
30005998:	40000004 	.word	0x40000004
3000599c:	40000002 	.word	0x40000002
300059a0:	30009e99 	.word	0x30009e99
300059a4:	30009c9d 	.word	0x30009c9d
300059a8:	30009cd9 	.word	0x30009cd9
300059ac:	00012be5 	.word	0x00012be5
300059b0:	e000ed00 	.word	0xe000ed00
300059b4:	3000951d 	.word	0x3000951d

300059b8 <password_hash_check>:
300059b8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300059bc:	b08f      	sub	sp, #60	; 0x3c
300059be:	4690      	mov	r8, r2
300059c0:	461d      	mov	r5, r3
300059c2:	4606      	mov	r6, r0
300059c4:	af04      	add	r7, sp, #16
300059c6:	3201      	adds	r2, #1
300059c8:	6d3b      	ldr	r3, [r7, #80]	; 0x50
300059ca:	6079      	str	r1, [r7, #4]
300059cc:	2100      	movs	r1, #0
300059ce:	f103 0008 	add.w	r0, r3, #8
300059d2:	f108 0308 	add.w	r3, r8, #8
300059d6:	f8d7 b050 	ldr.w	fp, [r7, #80]	; 0x50
300059da:	f023 0307 	bic.w	r3, r3, #7
300059de:	f020 0007 	bic.w	r0, r0, #7
300059e2:	ebad 0d03 	sub.w	sp, sp, r3
300059e6:	6d3b      	ldr	r3, [r7, #80]	; 0x50
300059e8:	f10d 0910 	add.w	r9, sp, #16
300059ec:	3301      	adds	r3, #1
300059ee:	ebad 0d00 	sub.w	sp, sp, r0
300059f2:	603b      	str	r3, [r7, #0]
300059f4:	4648      	mov	r0, r9
300059f6:	4b3f      	ldr	r3, [pc, #252]	; (30005af4 <password_hash_check+0x13c>)
300059f8:	f10d 0a10 	add.w	sl, sp, #16
300059fc:	4798      	blx	r3
300059fe:	683b      	ldr	r3, [r7, #0]
30005a00:	2100      	movs	r1, #0
30005a02:	4650      	mov	r0, sl
30005a04:	461a      	mov	r2, r3
30005a06:	4b3b      	ldr	r3, [pc, #236]	; (30005af4 <password_hash_check+0x13c>)
30005a08:	4798      	blx	r3
30005a0a:	f1b8 0001 	subs.w	r0, r8, #1
30005a0e:	d01d      	beq.n	30005a4c <password_hash_check+0x94>
30005a10:	2200      	movs	r2, #0
30005a12:	f109 33ff 	add.w	r3, r9, #4294967295	; 0xffffffff
30005a16:	4611      	mov	r1, r2
30005a18:	e00b      	b.n	30005a32 <password_hash_check+0x7a>
30005a1a:	f108 38ff 	add.w	r8, r8, #4294967295	; 0xffffffff
30005a1e:	2100      	movs	r1, #0
30005a20:	1c9c      	adds	r4, r3, #2
30005a22:	4282      	cmp	r2, r0
30005a24:	7059      	strb	r1, [r3, #1]
30005a26:	f103 0301 	add.w	r3, r3, #1
30005a2a:	4611      	mov	r1, r2
30005a2c:	eba4 0409 	sub.w	r4, r4, r9
30005a30:	d20b      	bcs.n	30005a4a <password_hash_check+0x92>
30005a32:	1874      	adds	r4, r6, r1
30005a34:	f102 0c01 	add.w	ip, r2, #1
30005a38:	5c71      	ldrb	r1, [r6, r1]
30005a3a:	3202      	adds	r2, #2
30005a3c:	7864      	ldrb	r4, [r4, #1]
30005a3e:	2c30      	cmp	r4, #48	; 0x30
30005a40:	d101      	bne.n	30005a46 <password_hash_check+0x8e>
30005a42:	295c      	cmp	r1, #92	; 0x5c
30005a44:	d0e9      	beq.n	30005a1a <password_hash_check+0x62>
30005a46:	4662      	mov	r2, ip
30005a48:	e7ea      	b.n	30005a20 <password_hash_check+0x68>
30005a4a:	4406      	add	r6, r0
30005a4c:	7833      	ldrb	r3, [r6, #0]
30005a4e:	f809 3004 	strb.w	r3, [r9, r4]
30005a52:	6d3b      	ldr	r3, [r7, #80]	; 0x50
30005a54:	1e5c      	subs	r4, r3, #1
30005a56:	d04a      	beq.n	30005aee <password_hash_check+0x136>
30005a58:	2200      	movs	r2, #0
30005a5a:	f10a 33ff 	add.w	r3, sl, #4294967295	; 0xffffffff
30005a5e:	4611      	mov	r1, r2
30005a60:	e00b      	b.n	30005a7a <password_hash_check+0xc2>
30005a62:	f10b 3bff 	add.w	fp, fp, #4294967295	; 0xffffffff
30005a66:	2100      	movs	r1, #0
30005a68:	1c98      	adds	r0, r3, #2
30005a6a:	42a2      	cmp	r2, r4
30005a6c:	7059      	strb	r1, [r3, #1]
30005a6e:	f103 0301 	add.w	r3, r3, #1
30005a72:	4611      	mov	r1, r2
30005a74:	eba0 000a 	sub.w	r0, r0, sl
30005a78:	d20a      	bcs.n	30005a90 <password_hash_check+0xd8>
30005a7a:	1868      	adds	r0, r5, r1
30005a7c:	1c56      	adds	r6, r2, #1
30005a7e:	5c69      	ldrb	r1, [r5, r1]
30005a80:	3202      	adds	r2, #2
30005a82:	7840      	ldrb	r0, [r0, #1]
30005a84:	2830      	cmp	r0, #48	; 0x30
30005a86:	d101      	bne.n	30005a8c <password_hash_check+0xd4>
30005a88:	295c      	cmp	r1, #92	; 0x5c
30005a8a:	d0ea      	beq.n	30005a62 <password_hash_check+0xaa>
30005a8c:	4632      	mov	r2, r6
30005a8e:	e7eb      	b.n	30005a68 <password_hash_check+0xb0>
30005a90:	4425      	add	r5, r4
30005a92:	782b      	ldrb	r3, [r5, #0]
30005a94:	f107 0408 	add.w	r4, r7, #8
30005a98:	2520      	movs	r5, #32
30005a9a:	4641      	mov	r1, r8
30005a9c:	f80a 3000 	strb.w	r3, [sl, r0]
30005aa0:	4652      	mov	r2, sl
30005aa2:	687b      	ldr	r3, [r7, #4]
30005aa4:	4648      	mov	r0, r9
30005aa6:	9402      	str	r4, [sp, #8]
30005aa8:	9300      	str	r3, [sp, #0]
30005aaa:	465b      	mov	r3, fp
30005aac:	9501      	str	r5, [sp, #4]
30005aae:	f7ff fe95 	bl	300057dc <BOOT_RAM_KeyDeriveFunc>
30005ab2:	4620      	mov	r0, r4
30005ab4:	462a      	mov	r2, r5
30005ab6:	4910      	ldr	r1, [pc, #64]	; (30005af8 <password_hash_check+0x140>)
30005ab8:	f006 fcd6 	bl	3000c468 <____wrap_memcmp_veneer>
30005abc:	4604      	mov	r4, r0
30005abe:	b968      	cbnz	r0, 30005adc <password_hash_check+0x124>
30005ac0:	480e      	ldr	r0, [pc, #56]	; (30005afc <password_hash_check+0x144>)
30005ac2:	f006 fc89 	bl	3000c3d8 <__DiagPrintf_veneer>
30005ac6:	462a      	mov	r2, r5
30005ac8:	4621      	mov	r1, r4
30005aca:	f107 0008 	add.w	r0, r7, #8
30005ace:	4b09      	ldr	r3, [pc, #36]	; (30005af4 <password_hash_check+0x13c>)
30005ad0:	4798      	blx	r3
30005ad2:	4620      	mov	r0, r4
30005ad4:	372c      	adds	r7, #44	; 0x2c
30005ad6:	46bd      	mov	sp, r7
30005ad8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005adc:	2401      	movs	r4, #1
30005ade:	4808      	ldr	r0, [pc, #32]	; (30005b00 <password_hash_check+0x148>)
30005ae0:	f006 fc7a 	bl	3000c3d8 <__DiagPrintf_veneer>
30005ae4:	372c      	adds	r7, #44	; 0x2c
30005ae6:	4620      	mov	r0, r4
30005ae8:	46bd      	mov	sp, r7
30005aea:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30005aee:	4620      	mov	r0, r4
30005af0:	e7cf      	b.n	30005a92 <password_hash_check+0xda>
30005af2:	bf00      	nop
30005af4:	00012be5 	.word	0x00012be5
30005af8:	3000eca4 	.word	0x3000eca4
30005afc:	3000ccd4 	.word	0x3000ccd4
30005b00:	3000ccbc 	.word	0x3000ccbc

30005b04 <BOOT_Share_Memory_Patch>:
30005b04:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
30005b08:	4817      	ldr	r0, [pc, #92]	; (30005b68 <BOOT_Share_Memory_Patch+0x64>)
30005b0a:	4918      	ldr	r1, [pc, #96]	; (30005b6c <BOOT_Share_Memory_Patch+0x68>)
30005b0c:	b430      	push	{r4, r5}
30005b0e:	6b13      	ldr	r3, [r2, #48]	; 0x30
30005b10:	f04f 5408 	mov.w	r4, #570425344	; 0x22000000
30005b14:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
30005b18:	6313      	str	r3, [r2, #48]	; 0x30
30005b1a:	6823      	ldr	r3, [r4, #0]
30005b1c:	6804      	ldr	r4, [r0, #0]
30005b1e:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b22:	6809      	ldr	r1, [r1, #0]
30005b24:	4423      	add	r3, r4
30005b26:	6805      	ldr	r5, [r0, #0]
30005b28:	f04f 5400 	mov.w	r4, #536870912	; 0x20000000
30005b2c:	440b      	add	r3, r1
30005b2e:	4910      	ldr	r1, [pc, #64]	; (30005b70 <BOOT_Share_Memory_Patch+0x6c>)
30005b30:	6808      	ldr	r0, [r1, #0]
30005b32:	442b      	add	r3, r5
30005b34:	6b11      	ldr	r1, [r2, #48]	; 0x30
30005b36:	4403      	add	r3, r0
30005b38:	480e      	ldr	r0, [pc, #56]	; (30005b74 <BOOT_Share_Memory_Patch+0x70>)
30005b3a:	f421 21c0 	bic.w	r1, r1, #393216	; 0x60000
30005b3e:	6311      	str	r1, [r2, #48]	; 0x30
30005b40:	6821      	ldr	r1, [r4, #0]
30005b42:	4a0d      	ldr	r2, [pc, #52]	; (30005b78 <BOOT_Share_Memory_Patch+0x74>)
30005b44:	6804      	ldr	r4, [r0, #0]
30005b46:	440b      	add	r3, r1
30005b48:	f500 3000 	add.w	r0, r0, #131072	; 0x20000
30005b4c:	6811      	ldr	r1, [r2, #0]
30005b4e:	4423      	add	r3, r4
30005b50:	f04f 520c 	mov.w	r2, #587202560	; 0x23000000
30005b54:	6800      	ldr	r0, [r0, #0]
30005b56:	440b      	add	r3, r1
30005b58:	6812      	ldr	r2, [r2, #0]
30005b5a:	4908      	ldr	r1, [pc, #32]	; (30005b7c <BOOT_Share_Memory_Patch+0x78>)
30005b5c:	4403      	add	r3, r0
30005b5e:	6808      	ldr	r0, [r1, #0]
30005b60:	4413      	add	r3, r2
30005b62:	bc30      	pop	{r4, r5}
30005b64:	4418      	add	r0, r3
30005b66:	4770      	bx	lr
30005b68:	2200c000 	.word	0x2200c000
30005b6c:	22018000 	.word	0x22018000
30005b70:	22100000 	.word	0x22100000
30005b74:	20010000 	.word	0x20010000
30005b78:	20020000 	.word	0x20020000
30005b7c:	23010000 	.word	0x23010000

30005b80 <BOOT_Image1>:
30005b80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30005b84:	4baa      	ldr	r3, [pc, #680]	; (30005e30 <BOOT_Image1+0x2b0>)
30005b86:	b087      	sub	sp, #28
30005b88:	4798      	blx	r3
30005b8a:	2800      	cmp	r0, #0
30005b8c:	f040 80c1 	bne.w	30005d12 <BOOT_Image1+0x192>
30005b90:	48a8      	ldr	r0, [pc, #672]	; (30005e34 <BOOT_Image1+0x2b4>)
30005b92:	f006 fc21 	bl	3000c3d8 <__DiagPrintf_veneer>
30005b96:	49a8      	ldr	r1, [pc, #672]	; (30005e38 <BOOT_Image1+0x2b8>)
30005b98:	48a8      	ldr	r0, [pc, #672]	; (30005e3c <BOOT_Image1+0x2bc>)
30005b9a:	f006 fc1d 	bl	3000c3d8 <__DiagPrintf_veneer>
30005b9e:	48a8      	ldr	r0, [pc, #672]	; (30005e40 <BOOT_Image1+0x2c0>)
30005ba0:	4aa8      	ldr	r2, [pc, #672]	; (30005e44 <BOOT_Image1+0x2c4>)
30005ba2:	2100      	movs	r1, #0
30005ba4:	4ba8      	ldr	r3, [pc, #672]	; (30005e48 <BOOT_Image1+0x2c8>)
30005ba6:	1a12      	subs	r2, r2, r0
30005ba8:	4798      	blx	r3
30005baa:	4aa8      	ldr	r2, [pc, #672]	; (30005e4c <BOOT_Image1+0x2cc>)
30005bac:	49a8      	ldr	r1, [pc, #672]	; (30005e50 <BOOT_Image1+0x2d0>)
30005bae:	8c13      	ldrh	r3, [r2, #32]
30005bb0:	b29b      	uxth	r3, r3
30005bb2:	8413      	strh	r3, [r2, #32]
30005bb4:	f8a1 3064 	strh.w	r3, [r1, #100]	; 0x64
30005bb8:	f003 fbd2 	bl	30009360 <BOOT_Reason>
30005bbc:	2800      	cmp	r0, #0
30005bbe:	f000 80e9 	beq.w	30005d94 <BOOT_Image1+0x214>
30005bc2:	4ea4      	ldr	r6, [pc, #656]	; (30005e54 <BOOT_Image1+0x2d4>)
30005bc4:	4ba4      	ldr	r3, [pc, #656]	; (30005e58 <BOOT_Image1+0x2d8>)
30005bc6:	4798      	blx	r3
30005bc8:	7834      	ldrb	r4, [r6, #0]
30005bca:	2c00      	cmp	r4, #0
30005bcc:	f000 8097 	beq.w	30005cfe <BOOT_Image1+0x17e>
30005bd0:	2000      	movs	r0, #0
30005bd2:	4ba2      	ldr	r3, [pc, #648]	; (30005e5c <BOOT_Image1+0x2dc>)
30005bd4:	4798      	blx	r3
30005bd6:	4ba2      	ldr	r3, [pc, #648]	; (30005e60 <BOOT_Image1+0x2e0>)
30005bd8:	2001      	movs	r0, #1
30005bda:	4798      	blx	r3
30005bdc:	4c9c      	ldr	r4, [pc, #624]	; (30005e50 <BOOT_Image1+0x2d0>)
30005bde:	f7ff fca5 	bl	3000552c <BOOT_SOC_ClkSet>
30005be2:	f241 070a 	movw	r7, #4106	; 0x100a
30005be6:	2028      	movs	r0, #40	; 0x28
30005be8:	8823      	ldrh	r3, [r4, #0]
30005bea:	4d9e      	ldr	r5, [pc, #632]	; (30005e64 <BOOT_Image1+0x2e4>)
30005bec:	b29b      	uxth	r3, r3
30005bee:	431f      	orrs	r7, r3
30005bf0:	4b9d      	ldr	r3, [pc, #628]	; (30005e68 <BOOT_Image1+0x2e8>)
30005bf2:	8027      	strh	r7, [r4, #0]
30005bf4:	4798      	blx	r3
30005bf6:	88a3      	ldrh	r3, [r4, #4]
30005bf8:	b29b      	uxth	r3, r3
30005bfa:	ea23 0307 	bic.w	r3, r3, r7
30005bfe:	80a3      	strh	r3, [r4, #4]
30005c00:	8863      	ldrh	r3, [r4, #2]
30005c02:	8067      	strh	r7, [r4, #2]
30005c04:	47a8      	blx	r5
30005c06:	2802      	cmp	r0, #2
30005c08:	f000 808b 	beq.w	30005d22 <BOOT_Image1+0x1a2>
30005c0c:	4b97      	ldr	r3, [pc, #604]	; (30005e6c <BOOT_Image1+0x2ec>)
30005c0e:	2401      	movs	r4, #1
30005c10:	4997      	ldr	r1, [pc, #604]	; (30005e70 <BOOT_Image1+0x2f0>)
30005c12:	f04f 4082 	mov.w	r0, #1090519040	; 0x41000000
30005c16:	f883 410e 	strb.w	r4, [r3, #270]	; 0x10e
30005c1a:	4622      	mov	r2, r4
30005c1c:	4f95      	ldr	r7, [pc, #596]	; (30005e74 <BOOT_Image1+0x2f4>)
30005c1e:	47b8      	blx	r7
30005c20:	4622      	mov	r2, r4
30005c22:	4995      	ldr	r1, [pc, #596]	; (30005e78 <BOOT_Image1+0x2f8>)
30005c24:	f04f 4081 	mov.w	r0, #1082130432	; 0x40800000
30005c28:	47b8      	blx	r7
30005c2a:	4a88      	ldr	r2, [pc, #544]	; (30005e4c <BOOT_Image1+0x2cc>)
30005c2c:	f8d2 32fc 	ldr.w	r3, [r2, #764]	; 0x2fc
30005c30:	f043 0303 	orr.w	r3, r3, #3
30005c34:	f8c2 32fc 	str.w	r3, [r2, #764]	; 0x2fc
30005c38:	4b90      	ldr	r3, [pc, #576]	; (30005e7c <BOOT_Image1+0x2fc>)
30005c3a:	4798      	blx	r3
30005c3c:	4a83      	ldr	r2, [pc, #524]	; (30005e4c <BOOT_Image1+0x2cc>)
30005c3e:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30005c42:	f023 0303 	bic.w	r3, r3, #3
30005c46:	b108      	cbz	r0, 30005c4c <BOOT_Image1+0xcc>
30005c48:	f043 0302 	orr.w	r3, r3, #2
30005c4c:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
30005c50:	4b8b      	ldr	r3, [pc, #556]	; (30005e80 <BOOT_Image1+0x300>)
30005c52:	781b      	ldrb	r3, [r3, #0]
30005c54:	2b00      	cmp	r3, #0
30005c56:	d17f      	bne.n	30005d58 <BOOT_Image1+0x1d8>
30005c58:	4c8a      	ldr	r4, [pc, #552]	; (30005e84 <BOOT_Image1+0x304>)
30005c5a:	2102      	movs	r1, #2
30005c5c:	2201      	movs	r2, #1
30005c5e:	488a      	ldr	r0, [pc, #552]	; (30005e88 <BOOT_Image1+0x308>)
30005c60:	47a0      	blx	r4
30005c62:	4c8a      	ldr	r4, [pc, #552]	; (30005e8c <BOOT_Image1+0x30c>)
30005c64:	68a3      	ldr	r3, [r4, #8]
30005c66:	0799      	lsls	r1, r3, #30
30005c68:	d470      	bmi.n	30005d4c <BOOT_Image1+0x1cc>
30005c6a:	4989      	ldr	r1, [pc, #548]	; (30005e90 <BOOT_Image1+0x310>)
30005c6c:	e841 f100 	tt	r1, r1
30005c70:	68a3      	ldr	r3, [r4, #8]
30005c72:	f3c1 5180 	ubfx	r1, r1, #22, #1
30005c76:	079a      	lsls	r2, r3, #30
30005c78:	f100 8088 	bmi.w	30005d8c <BOOT_Image1+0x20c>
30005c7c:	4b85      	ldr	r3, [pc, #532]	; (30005e94 <BOOT_Image1+0x314>)
30005c7e:	4f86      	ldr	r7, [pc, #536]	; (30005e98 <BOOT_Image1+0x318>)
30005c80:	681b      	ldr	r3, [r3, #0]
30005c82:	4798      	blx	r3
30005c84:	f7fe fa54 	bl	30004130 <BOOT_RccConfig>
30005c88:	f7fe fabe 	bl	30004208 <BOOT_CACHEWRR_Set>
30005c8c:	4b83      	ldr	r3, [pc, #524]	; (30005e9c <BOOT_Image1+0x31c>)
30005c8e:	6818      	ldr	r0, [r3, #0]
30005c90:	f7fe fae8 	bl	30004264 <BOOT_TCMSet>
30005c94:	4b82      	ldr	r3, [pc, #520]	; (30005ea0 <BOOT_Image1+0x320>)
30005c96:	695b      	ldr	r3, [r3, #20]
30005c98:	03db      	lsls	r3, r3, #15
30005c9a:	d512      	bpl.n	30005cc2 <BOOT_Image1+0x142>
30005c9c:	f017 011f 	ands.w	r1, r7, #31
30005ca0:	d13b      	bne.n	30005d1a <BOOT_Image1+0x19a>
30005ca2:	463b      	mov	r3, r7
30005ca4:	2174      	movs	r1, #116	; 0x74
30005ca6:	f3bf 8f4f 	dsb	sy
30005caa:	487d      	ldr	r0, [pc, #500]	; (30005ea0 <BOOT_Image1+0x320>)
30005cac:	4419      	add	r1, r3
30005cae:	f8c0 3268 	str.w	r3, [r0, #616]	; 0x268
30005cb2:	3320      	adds	r3, #32
30005cb4:	1aca      	subs	r2, r1, r3
30005cb6:	2a00      	cmp	r2, #0
30005cb8:	dcf9      	bgt.n	30005cae <BOOT_Image1+0x12e>
30005cba:	f3bf 8f4f 	dsb	sy
30005cbe:	f3bf 8f6f 	isb	sy
30005cc2:	4b6a      	ldr	r3, [pc, #424]	; (30005e6c <BOOT_Image1+0x2ec>)
30005cc4:	2200      	movs	r2, #0
30005cc6:	f8df 8184 	ldr.w	r8, [pc, #388]	; 30005e4c <BOOT_Image1+0x2cc>
30005cca:	f8c8 7270 	str.w	r7, [r8, #624]	; 0x270
30005cce:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
30005cd2:	f7fd fcf5 	bl	300036c0 <flash_highspeed_setup>
30005cd6:	47a8      	blx	r5
30005cd8:	2802      	cmp	r0, #2
30005cda:	f000 827d 	beq.w	300061d8 <BOOT_Image1+0x658>
30005cde:	7833      	ldrb	r3, [r6, #0]
30005ce0:	2b00      	cmp	r3, #0
30005ce2:	d05e      	beq.n	30005da2 <BOOT_Image1+0x222>
30005ce4:	4c6f      	ldr	r4, [pc, #444]	; (30005ea4 <BOOT_Image1+0x324>)
30005ce6:	4e70      	ldr	r6, [pc, #448]	; (30005ea8 <BOOT_Image1+0x328>)
30005ce8:	4d70      	ldr	r5, [pc, #448]	; (30005eac <BOOT_Image1+0x32c>)
30005cea:	4633      	mov	r3, r6
30005cec:	462a      	mov	r2, r5
30005cee:	496e      	ldr	r1, [pc, #440]	; (30005ea8 <BOOT_Image1+0x328>)
30005cf0:	2002      	movs	r0, #2
30005cf2:	f006 f8fb 	bl	3000beec <rtk_log_write>
30005cf6:	f241 3088 	movw	r0, #5000	; 0x1388
30005cfa:	47a0      	blx	r4
30005cfc:	e7f5      	b.n	30005cea <BOOT_Image1+0x16a>
30005cfe:	4b58      	ldr	r3, [pc, #352]	; (30005e60 <BOOT_Image1+0x2e0>)
30005d00:	2001      	movs	r0, #1
30005d02:	4798      	blx	r3
30005d04:	4620      	mov	r0, r4
30005d06:	4b6a      	ldr	r3, [pc, #424]	; (30005eb0 <BOOT_Image1+0x330>)
30005d08:	4798      	blx	r3
30005d0a:	4b6a      	ldr	r3, [pc, #424]	; (30005eb4 <BOOT_Image1+0x334>)
30005d0c:	2001      	movs	r0, #1
30005d0e:	4798      	blx	r3
30005d10:	e764      	b.n	30005bdc <BOOT_Image1+0x5c>
30005d12:	4869      	ldr	r0, [pc, #420]	; (30005eb8 <BOOT_Image1+0x338>)
30005d14:	f006 fb60 	bl	3000c3d8 <__DiagPrintf_veneer>
30005d18:	e73d      	b.n	30005b96 <BOOT_Image1+0x16>
30005d1a:	3174      	adds	r1, #116	; 0x74
30005d1c:	f027 031f 	bic.w	r3, r7, #31
30005d20:	e7c1      	b.n	30005ca6 <BOOT_Image1+0x126>
30005d22:	6de3      	ldr	r3, [r4, #92]	; 0x5c
30005d24:	2201      	movs	r2, #1
30005d26:	f04f 4188 	mov.w	r1, #1140850688	; 0x44000000
30005d2a:	f423 7398 	bic.w	r3, r3, #304	; 0x130
30005d2e:	65e3      	str	r3, [r4, #92]	; 0x5c
30005d30:	4b4e      	ldr	r3, [pc, #312]	; (30005e6c <BOOT_Image1+0x2ec>)
30005d32:	f883 010e 	strb.w	r0, [r3, #270]	; 0x10e
30005d36:	f04f 4090 	mov.w	r0, #1207959552	; 0x48000000
30005d3a:	4b4e      	ldr	r3, [pc, #312]	; (30005e74 <BOOT_Image1+0x2f4>)
30005d3c:	4798      	blx	r3
30005d3e:	f8d4 30fc 	ldr.w	r3, [r4, #252]	; 0xfc
30005d42:	f043 0302 	orr.w	r3, r3, #2
30005d46:	f8c4 30fc 	str.w	r3, [r4, #252]	; 0xfc
30005d4a:	e775      	b.n	30005c38 <BOOT_Image1+0xb8>
30005d4c:	f3ef 8108 	mrs	r1, MSP
30005d50:	485a      	ldr	r0, [pc, #360]	; (30005ebc <BOOT_Image1+0x33c>)
30005d52:	f006 fb41 	bl	3000c3d8 <__DiagPrintf_veneer>
30005d56:	e788      	b.n	30005c6a <BOOT_Image1+0xea>
30005d58:	4f59      	ldr	r7, [pc, #356]	; (30005ec0 <BOOT_Image1+0x340>)
30005d5a:	47b8      	blx	r7
30005d5c:	4604      	mov	r4, r0
30005d5e:	47b8      	blx	r7
30005d60:	2c02      	cmp	r4, #2
30005d62:	f43f af79 	beq.w	30005c58 <BOOT_Image1+0xd8>
30005d66:	2803      	cmp	r0, #3
30005d68:	f43f af76 	beq.w	30005c58 <BOOT_Image1+0xd8>
30005d6c:	4b55      	ldr	r3, [pc, #340]	; (30005ec4 <BOOT_Image1+0x344>)
30005d6e:	4798      	blx	r3
30005d70:	4c44      	ldr	r4, [pc, #272]	; (30005e84 <BOOT_Image1+0x304>)
30005d72:	2200      	movs	r2, #0
30005d74:	2101      	movs	r1, #1
30005d76:	4844      	ldr	r0, [pc, #272]	; (30005e88 <BOOT_Image1+0x308>)
30005d78:	47a0      	blx	r4
30005d7a:	2101      	movs	r1, #1
30005d7c:	4842      	ldr	r0, [pc, #264]	; (30005e88 <BOOT_Image1+0x308>)
30005d7e:	4b52      	ldr	r3, [pc, #328]	; (30005ec8 <BOOT_Image1+0x348>)
30005d80:	4798      	blx	r3
30005d82:	2201      	movs	r2, #1
30005d84:	4840      	ldr	r0, [pc, #256]	; (30005e88 <BOOT_Image1+0x308>)
30005d86:	4611      	mov	r1, r2
30005d88:	47a0      	blx	r4
30005d8a:	e766      	b.n	30005c5a <BOOT_Image1+0xda>
30005d8c:	484f      	ldr	r0, [pc, #316]	; (30005ecc <BOOT_Image1+0x34c>)
30005d8e:	f006 fb23 	bl	3000c3d8 <__DiagPrintf_veneer>
30005d92:	e773      	b.n	30005c7c <BOOT_Image1+0xfc>
30005d94:	4601      	mov	r1, r0
30005d96:	f44f 7201 	mov.w	r2, #516	; 0x204
30005d9a:	4834      	ldr	r0, [pc, #208]	; (30005e6c <BOOT_Image1+0x2ec>)
30005d9c:	f006 faec 	bl	3000c378 <____wrap_memset_veneer>
30005da0:	e70f      	b.n	30005bc2 <BOOT_Image1+0x42>
30005da2:	4b41      	ldr	r3, [pc, #260]	; (30005ea8 <BOOT_Image1+0x328>)
30005da4:	2004      	movs	r0, #4
30005da6:	4a4a      	ldr	r2, [pc, #296]	; (30005ed0 <BOOT_Image1+0x350>)
30005da8:	4619      	mov	r1, r3
30005daa:	f006 f89f 	bl	3000beec <rtk_log_write>
30005dae:	f7fe fbf1 	bl	30004594 <BOOT_DDR_Init>
30005db2:	f8d8 3290 	ldr.w	r3, [r8, #656]	; 0x290
30005db6:	f423 6340 	bic.w	r3, r3, #3072	; 0xc00
30005dba:	f443 6300 	orr.w	r3, r3, #2048	; 0x800
30005dbe:	f8c8 3290 	str.w	r3, [r8, #656]	; 0x290
30005dc2:	4b22      	ldr	r3, [pc, #136]	; (30005e4c <BOOT_Image1+0x2cc>)
30005dc4:	2200      	movs	r2, #0
30005dc6:	f8c3 22d0 	str.w	r2, [r3, #720]	; 0x2d0
30005dca:	f8c3 22d4 	str.w	r2, [r3, #724]	; 0x2d4
30005dce:	f8c3 22d8 	str.w	r2, [r3, #728]	; 0x2d8
30005dd2:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
30005dd6:	f7ff fe95 	bl	30005b04 <BOOT_Share_Memory_Patch>
30005dda:	4b3e      	ldr	r3, [pc, #248]	; (30005ed4 <BOOT_Image1+0x354>)
30005ddc:	781b      	ldrb	r3, [r3, #0]
30005dde:	2b00      	cmp	r3, #0
30005de0:	f040 82a4 	bne.w	3000632c <BOOT_Image1+0x7ac>
30005de4:	6823      	ldr	r3, [r4, #0]
30005de6:	f043 0302 	orr.w	r3, r3, #2
30005dea:	6023      	str	r3, [r4, #0]
30005dec:	4b3a      	ldr	r3, [pc, #232]	; (30005ed8 <BOOT_Image1+0x358>)
30005dee:	4798      	blx	r3
30005df0:	2801      	cmp	r0, #1
30005df2:	f04f 0100 	mov.w	r1, #0
30005df6:	f04f 0002 	mov.w	r0, #2
30005dfa:	bf0c      	ite	eq
30005dfc:	f44f 32e1 	moveq.w	r2, #115200	; 0x1c200
30005e00:	4a36      	ldrne	r2, [pc, #216]	; (30005edc <BOOT_Image1+0x35c>)
30005e02:	f001 fe3d 	bl	30007a80 <ymodem_uart_port_init>
30005e06:	4b34      	ldr	r3, [pc, #208]	; (30005ed8 <BOOT_Image1+0x358>)
30005e08:	4798      	blx	r3
30005e0a:	2800      	cmp	r0, #0
30005e0c:	f000 81df 	beq.w	300061ce <BOOT_Image1+0x64e>
30005e10:	f44f 31e1 	mov.w	r1, #115200	; 0x1c200
30005e14:	4832      	ldr	r0, [pc, #200]	; (30005ee0 <BOOT_Image1+0x360>)
30005e16:	f006 fadf 	bl	3000c3d8 <__DiagPrintf_veneer>
30005e1a:	4832      	ldr	r0, [pc, #200]	; (30005ee4 <BOOT_Image1+0x364>)
30005e1c:	2564      	movs	r5, #100	; 0x64
30005e1e:	f006 fb2b 	bl	3000c478 <____wrap_strlen_veneer>
30005e22:	4601      	mov	r1, r0
30005e24:	482f      	ldr	r0, [pc, #188]	; (30005ee4 <BOOT_Image1+0x364>)
30005e26:	4c1f      	ldr	r4, [pc, #124]	; (30005ea4 <BOOT_Image1+0x324>)
30005e28:	f001 fe84 	bl	30007b34 <ymodem_uart_putdata>
30005e2c:	e05f      	b.n	30005eee <BOOT_Image1+0x36e>
30005e2e:	bf00      	nop
30005e30:	0000c149 	.word	0x0000c149
30005e34:	3000ccf0 	.word	0x3000ccf0
30005e38:	3000e3ec 	.word	0x3000e3ec
30005e3c:	3000cd14 	.word	0x3000cd14
30005e40:	3000fc50 	.word	0x3000fc50
30005e44:	300100f4 	.word	0x300100f4
30005e48:	00012be5 	.word	0x00012be5
30005e4c:	42008000 	.word	0x42008000
30005e50:	42008200 	.word	0x42008200
30005e54:	3000e6d6 	.word	0x3000e6d6
30005e58:	3000b345 	.word	0x3000b345
30005e5c:	3000b6fd 	.word	0x3000b6fd
30005e60:	3000b61d 	.word	0x3000b61d
30005e64:	30007d31 	.word	0x30007d31
30005e68:	00009b2d 	.word	0x00009b2d
30005e6c:	23020000 	.word	0x23020000
30005e70:	40080000 	.word	0x40080000
30005e74:	0000b479 	.word	0x0000b479
30005e78:	40040000 	.word	0x40040000
30005e7c:	30009bfd 	.word	0x30009bfd
30005e80:	3000e6d5 	.word	0x3000e6d5
30005e84:	0000aab9 	.word	0x0000aab9
30005e88:	4200c000 	.word	0x4200c000
30005e8c:	2001c00c 	.word	0x2001c00c
30005e90:	30005b81 	.word	0x30005b81
30005e94:	00033000 	.word	0x00033000
30005e98:	2001c01c 	.word	0x2001c01c
30005e9c:	3000e6d8 	.word	0x3000e6d8
30005ea0:	e000ed00 	.word	0xe000ed00
30005ea4:	00009be5 	.word	0x00009be5
30005ea8:	3000c6b0 	.word	0x3000c6b0
30005eac:	3000cdb8 	.word	0x3000cdb8
30005eb0:	3000b84d 	.word	0x3000b84d
30005eb4:	3000b7bd 	.word	0x3000b7bd
30005eb8:	3000cd00 	.word	0x3000cd00
30005ebc:	3000cd2c 	.word	0x3000cd2c
30005ec0:	0000c0f9 	.word	0x0000c0f9
30005ec4:	0000aea1 	.word	0x0000aea1
30005ec8:	0000aaa5 	.word	0x0000aaa5
30005ecc:	3000cd5c 	.word	0x3000cd5c
30005ed0:	3000cdf8 	.word	0x3000cdf8
30005ed4:	3000e6d4 	.word	0x3000e6d4
30005ed8:	0000c169 	.word	0x0000c169
30005edc:	0016e360 	.word	0x0016e360
30005ee0:	3000ce0c 	.word	0x3000ce0c
30005ee4:	3000ecc4 	.word	0x3000ecc4
30005ee8:	3d01      	subs	r5, #1
30005eea:	f000 8239 	beq.w	30006360 <BOOT_Image1+0x7e0>
30005eee:	2001      	movs	r0, #1
30005ef0:	47a0      	blx	r4
30005ef2:	f001 fe05 	bl	30007b00 <ymodem_uart_readable>
30005ef6:	2800      	cmp	r0, #0
30005ef8:	d0f6      	beq.n	30005ee8 <BOOT_Image1+0x368>
30005efa:	f001 fe01 	bl	30007b00 <ymodem_uart_readable>
30005efe:	2800      	cmp	r0, #0
30005f00:	f000 8234 	beq.w	3000636c <BOOT_Image1+0x7ec>
30005f04:	2301      	movs	r3, #1
30005f06:	9303      	str	r3, [sp, #12]
30005f08:	2001      	movs	r0, #1
30005f0a:	47a0      	blx	r4
30005f0c:	2108      	movs	r1, #8
30005f0e:	48c3      	ldr	r0, [pc, #780]	; (3000621c <BOOT_Image1+0x69c>)
30005f10:	f001 fe1e 	bl	30007b50 <ymodem_uart_getdata>
30005f14:	49c1      	ldr	r1, [pc, #772]	; (3000621c <BOOT_Image1+0x69c>)
30005f16:	2208      	movs	r2, #8
30005f18:	4bc1      	ldr	r3, [pc, #772]	; (30006220 <BOOT_Image1+0x6a0>)
30005f1a:	f101 0080 	add.w	r0, r1, #128	; 0x80
30005f1e:	4798      	blx	r3
30005f20:	4605      	mov	r5, r0
30005f22:	2800      	cmp	r0, #0
30005f24:	f040 8282 	bne.w	3000642c <BOOT_Image1+0x8ac>
30005f28:	f001 fdea 	bl	30007b00 <ymodem_uart_readable>
30005f2c:	2800      	cmp	r0, #0
30005f2e:	d0fb      	beq.n	30005f28 <BOOT_Image1+0x3a8>
30005f30:	2101      	movs	r1, #1
30005f32:	48bc      	ldr	r0, [pc, #752]	; (30006224 <BOOT_Image1+0x6a4>)
30005f34:	f001 fe0c 	bl	30007b50 <ymodem_uart_getdata>
30005f38:	f001 fde2 	bl	30007b00 <ymodem_uart_readable>
30005f3c:	2800      	cmp	r0, #0
30005f3e:	d0fb      	beq.n	30005f38 <BOOT_Image1+0x3b8>
30005f40:	4fb9      	ldr	r7, [pc, #740]	; (30006228 <BOOT_Image1+0x6a8>)
30005f42:	2101      	movs	r1, #1
30005f44:	f207 4051 	addw	r0, r7, #1105	; 0x451
30005f48:	f001 fe02 	bl	30007b50 <ymodem_uart_getdata>
30005f4c:	f897 2451 	ldrb.w	r2, [r7, #1105]	; 0x451
30005f50:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f54:	429a      	cmp	r2, r3
30005f56:	f000 828e 	beq.w	30006476 <BOOT_Image1+0x8f6>
30005f5a:	f207 4952 	addw	r9, r7, #1106	; 0x452
30005f5e:	2600      	movs	r6, #0
30005f60:	46b0      	mov	r8, r6
30005f62:	3601      	adds	r6, #1
30005f64:	f001 fdcc 	bl	30007b00 <ymodem_uart_readable>
30005f68:	2800      	cmp	r0, #0
30005f6a:	d0fb      	beq.n	30005f64 <BOOT_Image1+0x3e4>
30005f6c:	4648      	mov	r0, r9
30005f6e:	2101      	movs	r1, #1
30005f70:	f001 fdee 	bl	30007b50 <ymodem_uart_getdata>
30005f74:	f899 2000 	ldrb.w	r2, [r9]
30005f78:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005f7c:	f108 080b 	add.w	r8, r8, #11
30005f80:	f109 0901 	add.w	r9, r9, #1
30005f84:	46b3      	mov	fp, r6
30005f86:	429a      	cmp	r2, r3
30005f88:	d1ea      	bne.n	30005f60 <BOOT_Image1+0x3e0>
30005f8a:	2e00      	cmp	r6, #0
30005f8c:	dd13      	ble.n	30005fb6 <BOOT_Image1+0x436>
30005f8e:	f507 618a 	add.w	r1, r7, #1104	; 0x450
30005f92:	2300      	movs	r3, #0
30005f94:	1988      	adds	r0, r1, r6
30005f96:	e001      	b.n	30005f9c <BOOT_Image1+0x41c>
30005f98:	4288      	cmp	r0, r1
30005f9a:	d00d      	beq.n	30005fb8 <BOOT_Image1+0x438>
30005f9c:	f811 2f01 	ldrb.w	r2, [r1, #1]!
30005fa0:	eb03 0383 	add.w	r3, r3, r3, lsl #2
30005fa4:	f1a2 0c30 	sub.w	ip, r2, #48	; 0x30
30005fa8:	eb02 0343 	add.w	r3, r2, r3, lsl #1
30005fac:	f1bc 0f09 	cmp.w	ip, #9
30005fb0:	f1a3 0330 	sub.w	r3, r3, #48	; 0x30
30005fb4:	d9f0      	bls.n	30005f98 <BOOT_Image1+0x418>
30005fb6:	2300      	movs	r3, #0
30005fb8:	f8c7 34d4 	str.w	r3, [r7, #1236]	; 0x4d4
30005fbc:	f001 fda0 	bl	30007b00 <ymodem_uart_readable>
30005fc0:	2800      	cmp	r0, #0
30005fc2:	d0fb      	beq.n	30005fbc <BOOT_Image1+0x43c>
30005fc4:	4995      	ldr	r1, [pc, #596]	; (3000621c <BOOT_Image1+0x69c>)
30005fc6:	460b      	mov	r3, r1
30005fc8:	2101      	movs	r1, #1
30005fca:	eb03 0008 	add.w	r0, r3, r8
30005fce:	9302      	str	r3, [sp, #8]
30005fd0:	f001 fdbe 	bl	30007b50 <ymodem_uart_getdata>
30005fd4:	9902      	ldr	r1, [sp, #8]
30005fd6:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
30005fda:	f811 2008 	ldrb.w	r2, [r1, r8]
30005fde:	429a      	cmp	r2, r3
30005fe0:	f000 8245 	beq.w	3000646e <BOOT_Image1+0x8ee>
30005fe4:	f10b 080b 	add.w	r8, fp, #11
30005fe8:	f04f 0900 	mov.w	r9, #0
30005fec:	4488      	add	r8, r1
30005fee:	f109 0901 	add.w	r9, r9, #1
30005ff2:	f001 fd85 	bl	30007b00 <ymodem_uart_readable>
30005ff6:	2800      	cmp	r0, #0
30005ff8:	d0fb      	beq.n	30005ff2 <BOOT_Image1+0x472>
30005ffa:	4640      	mov	r0, r8
30005ffc:	2101      	movs	r1, #1
30005ffe:	f001 fda7 	bl	30007b50 <ymodem_uart_getdata>
30006002:	f898 2000 	ldrb.w	r2, [r8]
30006006:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
3000600a:	f108 0801 	add.w	r8, r8, #1
3000600e:	eb09 0a0b 	add.w	sl, r9, fp
30006012:	429a      	cmp	r2, r3
30006014:	d1eb      	bne.n	30005fee <BOOT_Image1+0x46e>
30006016:	f001 fd73 	bl	30007b00 <ymodem_uart_readable>
3000601a:	2800      	cmp	r0, #0
3000601c:	d0fb      	beq.n	30006016 <BOOT_Image1+0x496>
3000601e:	9b02      	ldr	r3, [sp, #8]
30006020:	f10a 080b 	add.w	r8, sl, #11
30006024:	2101      	movs	r1, #1
30006026:	eb03 0008 	add.w	r0, r3, r8
3000602a:	f001 fd91 	bl	30007b50 <ymodem_uart_getdata>
3000602e:	9902      	ldr	r1, [sp, #8]
30006030:	f897 24d1 	ldrb.w	r2, [r7, #1233]	; 0x4d1
30006034:	f811 3008 	ldrb.w	r3, [r1, r8]
30006038:	429a      	cmp	r2, r3
3000603a:	f000 8216 	beq.w	3000646a <BOOT_Image1+0x8ea>
3000603e:	f10a 080c 	add.w	r8, sl, #12
30006042:	4488      	add	r8, r1
30006044:	3501      	adds	r5, #1
30006046:	f001 fd5b 	bl	30007b00 <ymodem_uart_readable>
3000604a:	2800      	cmp	r0, #0
3000604c:	d0fb      	beq.n	30006046 <BOOT_Image1+0x4c6>
3000604e:	4640      	mov	r0, r8
30006050:	2101      	movs	r1, #1
30006052:	f001 fd7d 	bl	30007b50 <ymodem_uart_getdata>
30006056:	f898 2000 	ldrb.w	r2, [r8]
3000605a:	f897 34d1 	ldrb.w	r3, [r7, #1233]	; 0x4d1
3000605e:	f108 0801 	add.w	r8, r8, #1
30006062:	eb05 0b0a 	add.w	fp, r5, sl
30006066:	429a      	cmp	r2, r3
30006068:	d1ec      	bne.n	30006044 <BOOT_Image1+0x4c4>
3000606a:	9b02      	ldr	r3, [sp, #8]
3000606c:	f10b 080c 	add.w	r8, fp, #12
30006070:	2101      	movs	r1, #1
30006072:	eb03 0008 	add.w	r0, r3, r8
30006076:	f001 fd6b 	bl	30007b50 <ymodem_uart_getdata>
3000607a:	9b02      	ldr	r3, [sp, #8]
3000607c:	f813 1008 	ldrb.w	r1, [r3, r8]
30006080:	290a      	cmp	r1, #10
30006082:	f8c7 14d8 	str.w	r1, [r7, #1240]	; 0x4d8
30006086:	f200 81d8 	bhi.w	3000643a <BOOT_Image1+0x8ba>
3000608a:	9b02      	ldr	r3, [sp, #8]
3000608c:	f10b 000d 	add.w	r0, fp, #13
30006090:	2101      	movs	r1, #1
30006092:	f10b 080e 	add.w	r8, fp, #14
30006096:	4418      	add	r0, r3
30006098:	f001 fd5a 	bl	30007b50 <ymodem_uart_getdata>
3000609c:	9b02      	ldr	r3, [sp, #8]
3000609e:	2102      	movs	r1, #2
300060a0:	4498      	add	r8, r3
300060a2:	4640      	mov	r0, r8
300060a4:	f001 fd54 	bl	30007b50 <ymodem_uart_getdata>
300060a8:	4641      	mov	r1, r8
300060aa:	2202      	movs	r2, #2
300060ac:	485f      	ldr	r0, [pc, #380]	; (3000622c <BOOT_Image1+0x6ac>)
300060ae:	4b5c      	ldr	r3, [pc, #368]	; (30006220 <BOOT_Image1+0x6a0>)
300060b0:	4798      	blx	r3
300060b2:	2800      	cmp	r0, #0
300060b4:	f040 81d5 	bne.w	30006462 <BOOT_Image1+0x8e2>
300060b8:	9b02      	ldr	r3, [sp, #8]
300060ba:	f10b 0010 	add.w	r0, fp, #16
300060be:	2101      	movs	r1, #1
300060c0:	f10b 0811 	add.w	r8, fp, #17
300060c4:	4418      	add	r0, r3
300060c6:	f001 fd43 	bl	30007b50 <ymodem_uart_getdata>
300060ca:	4b59      	ldr	r3, [pc, #356]	; (30006230 <BOOT_Image1+0x6b0>)
300060cc:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
300060d0:	f103 0228 	add.w	r2, r3, #40	; 0x28
300060d4:	f103 0130 	add.w	r1, r3, #48	; 0x30
300060d8:	f7fd fcf2 	bl	30003ac0 <hotfix_get_addr_size>
300060dc:	9b02      	ldr	r3, [sp, #8]
300060de:	2102      	movs	r1, #2
300060e0:	4498      	add	r8, r3
300060e2:	4640      	mov	r0, r8
300060e4:	f001 fd34 	bl	30007b50 <ymodem_uart_getdata>
300060e8:	4641      	mov	r1, r8
300060ea:	2202      	movs	r2, #2
300060ec:	4851      	ldr	r0, [pc, #324]	; (30006234 <BOOT_Image1+0x6b4>)
300060ee:	4b4c      	ldr	r3, [pc, #304]	; (30006220 <BOOT_Image1+0x6a0>)
300060f0:	4798      	blx	r3
300060f2:	2800      	cmp	r0, #0
300060f4:	f000 80c8 	beq.w	30006288 <BOOT_Image1+0x708>
300060f8:	9b03      	ldr	r3, [sp, #12]
300060fa:	2b00      	cmp	r3, #0
300060fc:	f000 810f 	beq.w	3000631e <BOOT_Image1+0x79e>
30006100:	f005 ff30 	bl	3000bf64 <BOOT_VerCheck>
30006104:	f7fe fc3a 	bl	3000497c <BOOT_LoadImages>
30006108:	2800      	cmp	r0, #0
3000610a:	d07f      	beq.n	3000620c <BOOT_Image1+0x68c>
3000610c:	f7ff f9cc 	bl	300054a8 <BOOT_Enable_KM0>
30006110:	2200      	movs	r2, #0
30006112:	2101      	movs	r1, #1
30006114:	4b48      	ldr	r3, [pc, #288]	; (30006238 <BOOT_Image1+0x6b8>)
30006116:	4849      	ldr	r0, [pc, #292]	; (3000623c <BOOT_Image1+0x6bc>)
30006118:	4798      	blx	r3
3000611a:	4b49      	ldr	r3, [pc, #292]	; (30006240 <BOOT_Image1+0x6c0>)
3000611c:	2008      	movs	r0, #8
3000611e:	4798      	blx	r3
30006120:	f001 fa24 	bl	3000756c <BOOT_RAM_TZCfg>
30006124:	4b47      	ldr	r3, [pc, #284]	; (30006244 <BOOT_Image1+0x6c4>)
30006126:	781b      	ldrb	r3, [r3, #0]
30006128:	2b01      	cmp	r3, #1
3000612a:	f000 8192 	beq.w	30006452 <BOOT_Image1+0x8d2>
3000612e:	4b46      	ldr	r3, [pc, #280]	; (30006248 <BOOT_Image1+0x6c8>)
30006130:	781b      	ldrb	r3, [r3, #0]
30006132:	2b00      	cmp	r3, #0
30006134:	f000 8104 	beq.w	30006340 <BOOT_Image1+0x7c0>
30006138:	f7ff fae6 	bl	30005708 <BOOT_Enable_AP>
3000613c:	4a43      	ldr	r2, [pc, #268]	; (3000624c <BOOT_Image1+0x6cc>)
3000613e:	f892 3066 	ldrb.w	r3, [r2, #102]	; 0x66
30006142:	f043 0306 	orr.w	r3, r3, #6
30006146:	f882 3066 	strb.w	r3, [r2, #102]	; 0x66
3000614a:	4b3e      	ldr	r3, [pc, #248]	; (30006244 <BOOT_Image1+0x6c4>)
3000614c:	781b      	ldrb	r3, [r3, #0]
3000614e:	2b01      	cmp	r3, #1
30006150:	f000 8177 	beq.w	30006442 <BOOT_Image1+0x8c2>
30006154:	483d      	ldr	r0, [pc, #244]	; (3000624c <BOOT_Image1+0x6cc>)
30006156:	493e      	ldr	r1, [pc, #248]	; (30006250 <BOOT_Image1+0x6d0>)
30006158:	f890 3067 	ldrb.w	r3, [r0, #103]	; 0x67
3000615c:	4a3d      	ldr	r2, [pc, #244]	; (30006254 <BOOT_Image1+0x6d4>)
3000615e:	f043 0301 	orr.w	r3, r3, #1
30006162:	4d3d      	ldr	r5, [pc, #244]	; (30006258 <BOOT_Image1+0x6d8>)
30006164:	f880 3067 	strb.w	r3, [r0, #103]	; 0x67
30006168:	688c      	ldr	r4, [r1, #8]
3000616a:	680b      	ldr	r3, [r1, #0]
3000616c:	493b      	ldr	r1, [pc, #236]	; (3000625c <BOOT_Image1+0x6dc>)
3000616e:	6063      	str	r3, [r4, #4]
30006170:	4b3b      	ldr	r3, [pc, #236]	; (30006260 <BOOT_Image1+0x6e0>)
30006172:	6094      	str	r4, [r2, #8]
30006174:	61e9      	str	r1, [r5, #28]
30006176:	6a59      	ldr	r1, [r3, #36]	; 0x24
30006178:	483a      	ldr	r0, [pc, #232]	; (30006264 <BOOT_Image1+0x6e4>)
3000617a:	f441 2170 	orr.w	r1, r1, #983040	; 0xf0000
3000617e:	6259      	str	r1, [r3, #36]	; 0x24
30006180:	6a51      	ldr	r1, [r2, #36]	; 0x24
30006182:	f441 21e0 	orr.w	r1, r1, #458752	; 0x70000
30006186:	6251      	str	r1, [r2, #36]	; 0x24
30006188:	f8d3 1088 	ldr.w	r1, [r3, #136]	; 0x88
3000618c:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
30006190:	f8c3 1088 	str.w	r1, [r3, #136]	; 0x88
30006194:	f8d2 1088 	ldr.w	r1, [r2, #136]	; 0x88
30006198:	f441 0170 	orr.w	r1, r1, #15728640	; 0xf00000
3000619c:	f8c2 1088 	str.w	r1, [r2, #136]	; 0x88
300061a0:	f8d3 208c 	ldr.w	r2, [r3, #140]	; 0x8c
300061a4:	f442 6240 	orr.w	r2, r2, #3072	; 0xc00
300061a8:	f8c3 208c 	str.w	r2, [r3, #140]	; 0x8c
300061ac:	689b      	ldr	r3, [r3, #8]
300061ae:	6183      	str	r3, [r0, #24]
300061b0:	f7fe fb6c 	bl	3000488c <BOOT_NVICBackup_HP>
300061b4:	4b2c      	ldr	r3, [pc, #176]	; (30006268 <BOOT_Image1+0x6e8>)
300061b6:	f383 8888 	msr	MSP_NS, r3
300061ba:	f5a3 6300 	sub.w	r3, r3, #2048	; 0x800
300061be:	f383 8809 	msr	PSP, r3
300061c2:	6860      	ldr	r0, [r4, #4]
300061c4:	b007      	add	sp, #28
300061c6:	e8bd 4ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300061ca:	f7fd bf61 	b.w	30004090 <BOOT_NsStart>
300061ce:	4927      	ldr	r1, [pc, #156]	; (3000626c <BOOT_Image1+0x6ec>)
300061d0:	4827      	ldr	r0, [pc, #156]	; (30006270 <BOOT_Image1+0x6f0>)
300061d2:	f006 f901 	bl	3000c3d8 <__DiagPrintf_veneer>
300061d6:	e620      	b.n	30005e1a <BOOT_Image1+0x29a>
300061d8:	4b26      	ldr	r3, [pc, #152]	; (30006274 <BOOT_Image1+0x6f4>)
300061da:	2004      	movs	r0, #4
300061dc:	4a26      	ldr	r2, [pc, #152]	; (30006278 <BOOT_Image1+0x6f8>)
300061de:	4619      	mov	r1, r3
300061e0:	f005 fe84 	bl	3000beec <rtk_log_write>
300061e4:	4b25      	ldr	r3, [pc, #148]	; (3000627c <BOOT_Image1+0x6fc>)
300061e6:	4798      	blx	r3
300061e8:	f7fe fa12 	bl	30004610 <BOOT_PSRAM_Init>
300061ec:	7830      	ldrb	r0, [r6, #0]
300061ee:	2800      	cmp	r0, #0
300061f0:	f47f ade7 	bne.w	30005dc2 <BOOT_Image1+0x242>
300061f4:	4b22      	ldr	r3, [pc, #136]	; (30006280 <BOOT_Image1+0x700>)
300061f6:	4798      	blx	r3
300061f8:	2800      	cmp	r0, #0
300061fa:	f43f ade2 	beq.w	30005dc2 <BOOT_Image1+0x242>
300061fe:	4b1d      	ldr	r3, [pc, #116]	; (30006274 <BOOT_Image1+0x6f4>)
30006200:	2003      	movs	r0, #3
30006202:	4a20      	ldr	r2, [pc, #128]	; (30006284 <BOOT_Image1+0x704>)
30006204:	4619      	mov	r1, r3
30006206:	f005 fe71 	bl	3000beec <rtk_log_write>
3000620a:	e5da      	b.n	30005dc2 <BOOT_Image1+0x242>
3000620c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006210:	47a0      	blx	r4
30006212:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30006216:	47a0      	blx	r4
30006218:	e7f8      	b.n	3000620c <BOOT_Image1+0x68c>
3000621a:	bf00      	nop
3000621c:	3000ece4 	.word	0x3000ece4
30006220:	000129bd 	.word	0x000129bd
30006224:	3000ecec 	.word	0x3000ecec
30006228:	3000e89c 	.word	0x3000e89c
3000622c:	3000ed78 	.word	0x3000ed78
30006230:	3000ed7c 	.word	0x3000ed7c
30006234:	3000edb8 	.word	0x3000edb8
30006238:	0000ac6d 	.word	0x0000ac6d
3000623c:	4200c000 	.word	0x4200c000
30006240:	0000e1d9 	.word	0x0000e1d9
30006244:	3000e6d4 	.word	0x3000e6d4
30006248:	3000e6d7 	.word	0x3000e6d7
3000624c:	42008200 	.word	0x42008200
30006250:	60000020 	.word	0x60000020
30006254:	e002ed00 	.word	0xe002ed00
30006258:	30000000 	.word	0x30000000
3000625c:	30003a81 	.word	0x30003a81
30006260:	e000ed00 	.word	0xe000ed00
30006264:	3000fcfc 	.word	0x3000fcfc
30006268:	2001bffc 	.word	0x2001bffc
3000626c:	0016e360 	.word	0x0016e360
30006270:	3000ce0c 	.word	0x3000ce0c
30006274:	3000c6b0 	.word	0x3000c6b0
30006278:	3000cd8c 	.word	0x3000cd8c
3000627c:	30008bb1 	.word	0x30008bb1
30006280:	3000b71d 	.word	0x3000b71d
30006284:	3000cda0 	.word	0x3000cda0
30006288:	9b02      	ldr	r3, [sp, #8]
3000628a:	f10b 0013 	add.w	r0, fp, #19
3000628e:	2101      	movs	r1, #1
30006290:	f10b 0814 	add.w	r8, fp, #20
30006294:	4418      	add	r0, r3
30006296:	f001 fc5b 	bl	30007b50 <ymodem_uart_getdata>
3000629a:	4b7c      	ldr	r3, [pc, #496]	; (3000648c <BOOT_Image1+0x90c>)
3000629c:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
300062a0:	f1a3 0218 	sub.w	r2, r3, #24
300062a4:	f1a3 0110 	sub.w	r1, r3, #16
300062a8:	f7fd fc0a 	bl	30003ac0 <hotfix_get_addr_size>
300062ac:	9b02      	ldr	r3, [sp, #8]
300062ae:	2102      	movs	r1, #2
300062b0:	4498      	add	r8, r3
300062b2:	4640      	mov	r0, r8
300062b4:	f001 fc4c 	bl	30007b50 <ymodem_uart_getdata>
300062b8:	4641      	mov	r1, r8
300062ba:	2202      	movs	r2, #2
300062bc:	4874      	ldr	r0, [pc, #464]	; (30006490 <BOOT_Image1+0x910>)
300062be:	4b75      	ldr	r3, [pc, #468]	; (30006494 <BOOT_Image1+0x914>)
300062c0:	4798      	blx	r3
300062c2:	2800      	cmp	r0, #0
300062c4:	f47f af18 	bne.w	300060f8 <BOOT_Image1+0x578>
300062c8:	9b02      	ldr	r3, [sp, #8]
300062ca:	f10b 0016 	add.w	r0, fp, #22
300062ce:	2101      	movs	r1, #1
300062d0:	f10b 0817 	add.w	r8, fp, #23
300062d4:	4418      	add	r0, r3
300062d6:	f001 fc3b 	bl	30007b50 <ymodem_uart_getdata>
300062da:	4b6f      	ldr	r3, [pc, #444]	; (30006498 <BOOT_Image1+0x918>)
300062dc:	f8d7 04d8 	ldr.w	r0, [r7, #1240]	; 0x4d8
300062e0:	f1a3 0244 	sub.w	r2, r3, #68	; 0x44
300062e4:	f1a3 013c 	sub.w	r1, r3, #60	; 0x3c
300062e8:	f7fd fbea 	bl	30003ac0 <hotfix_get_addr_size>
300062ec:	9a02      	ldr	r2, [sp, #8]
300062ee:	2102      	movs	r1, #2
300062f0:	4490      	add	r8, r2
300062f2:	4640      	mov	r0, r8
300062f4:	f001 fc2c 	bl	30007b50 <ymodem_uart_getdata>
300062f8:	4641      	mov	r1, r8
300062fa:	2202      	movs	r2, #2
300062fc:	4867      	ldr	r0, [pc, #412]	; (3000649c <BOOT_Image1+0x91c>)
300062fe:	4b65      	ldr	r3, [pc, #404]	; (30006494 <BOOT_Image1+0x914>)
30006300:	4798      	blx	r3
30006302:	2800      	cmp	r0, #0
30006304:	f47f aef8 	bne.w	300060f8 <BOOT_Image1+0x578>
30006308:	9b02      	ldr	r3, [sp, #8]
3000630a:	f10b 0019 	add.w	r0, fp, #25
3000630e:	2101      	movs	r1, #1
30006310:	4418      	add	r0, r3
30006312:	f001 fc1d 	bl	30007b50 <ymodem_uart_getdata>
30006316:	9b03      	ldr	r3, [sp, #12]
30006318:	2b00      	cmp	r3, #0
3000631a:	f040 80b2 	bne.w	30006482 <BOOT_Image1+0x902>
3000631e:	2002      	movs	r0, #2
30006320:	f001 fbc6 	bl	30007ab0 <ymodem_uart_port_deinit>
30006324:	485e      	ldr	r0, [pc, #376]	; (300064a0 <BOOT_Image1+0x920>)
30006326:	f006 f857 	bl	3000c3d8 <__DiagPrintf_veneer>
3000632a:	e6e9      	b.n	30006100 <BOOT_Image1+0x580>
3000632c:	4b5d      	ldr	r3, [pc, #372]	; (300064a4 <BOOT_Image1+0x924>)
3000632e:	4798      	blx	r3
30006330:	2800      	cmp	r0, #0
30006332:	f47f ad57 	bne.w	30005de4 <BOOT_Image1+0x264>
30006336:	68a3      	ldr	r3, [r4, #8]
30006338:	f043 0302 	orr.w	r3, r3, #2
3000633c:	60a3      	str	r3, [r4, #8]
3000633e:	e551      	b.n	30005de4 <BOOT_Image1+0x264>
30006340:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
30006344:	685a      	ldr	r2, [r3, #4]
30006346:	f042 0202 	orr.w	r2, r2, #2
3000634a:	605a      	str	r2, [r3, #4]
3000634c:	685a      	ldr	r2, [r3, #4]
3000634e:	f042 0230 	orr.w	r2, r2, #48	; 0x30
30006352:	605a      	str	r2, [r3, #4]
30006354:	4a54      	ldr	r2, [pc, #336]	; (300064a8 <BOOT_Image1+0x928>)
30006356:	6f93      	ldr	r3, [r2, #120]	; 0x78
30006358:	f043 0303 	orr.w	r3, r3, #3
3000635c:	6793      	str	r3, [r2, #120]	; 0x78
3000635e:	e6f4      	b.n	3000614a <BOOT_Image1+0x5ca>
30006360:	f001 fbce 	bl	30007b00 <ymodem_uart_readable>
30006364:	2800      	cmp	r0, #0
30006366:	d0da      	beq.n	3000631e <BOOT_Image1+0x79e>
30006368:	9503      	str	r5, [sp, #12]
3000636a:	e5cd      	b.n	30005f08 <BOOT_Image1+0x388>
3000636c:	4f4f      	ldr	r7, [pc, #316]	; (300064ac <BOOT_Image1+0x92c>)
3000636e:	4605      	mov	r5, r0
30006370:	f04f 080b 	mov.w	r8, #11
30006374:	260a      	movs	r6, #10
30006376:	f507 6389 	add.w	r3, r7, #1096	; 0x448
3000637a:	4681      	mov	r9, r0
3000637c:	9302      	str	r3, [sp, #8]
3000637e:	2004      	movs	r0, #4
30006380:	f001 fb96 	bl	30007ab0 <ymodem_uart_port_deinit>
30006384:	9a02      	ldr	r2, [sp, #8]
30006386:	9500      	str	r5, [sp, #0]
30006388:	4613      	mov	r3, r2
3000638a:	1990      	adds	r0, r2, r6
3000638c:	f8d7 14d4 	ldr.w	r1, [r7, #1236]	; 0x4d4
30006390:	464a      	mov	r2, r9
30006392:	4443      	add	r3, r8
30006394:	f7ff fb10 	bl	300059b8 <password_hash_check>
30006398:	4680      	mov	r8, r0
3000639a:	2800      	cmp	r0, #0
3000639c:	d1bf      	bne.n	3000631e <BOOT_Image1+0x79e>
3000639e:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300063a2:	47a0      	blx	r4
300063a4:	4842      	ldr	r0, [pc, #264]	; (300064b0 <BOOT_Image1+0x930>)
300063a6:	f006 f867 	bl	3000c478 <____wrap_strlen_veneer>
300063aa:	4601      	mov	r1, r0
300063ac:	4840      	ldr	r0, [pc, #256]	; (300064b0 <BOOT_Image1+0x930>)
300063ae:	f001 fbc1 	bl	30007b34 <ymodem_uart_putdata>
300063b2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300063b6:	47a0      	blx	r4
300063b8:	f8d7 24d8 	ldr.w	r2, [r7, #1240]	; 0x4d8
300063bc:	493d      	ldr	r1, [pc, #244]	; (300064b4 <BOOT_Image1+0x934>)
300063be:	2002      	movs	r0, #2
300063c0:	f001 fc58 	bl	30007c74 <ymodem_img_download>
300063c4:	2032      	movs	r0, #50	; 0x32
300063c6:	47a0      	blx	r4
300063c8:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
300063cc:	b1e3      	cbz	r3, 30006408 <BOOT_Image1+0x888>
300063ce:	f8df 90c8 	ldr.w	r9, [pc, #200]	; 30006498 <BOOT_Image1+0x918>
300063d2:	f1a9 0a2c 	sub.w	sl, r9, #44	; 0x2c
300063d6:	f859 6b04 	ldr.w	r6, [r9], #4
300063da:	0b36      	lsrs	r6, r6, #12
300063dc:	d00c      	beq.n	300063f8 <BOOT_Image1+0x878>
300063de:	2500      	movs	r5, #0
300063e0:	f8da 1000 	ldr.w	r1, [sl]
300063e4:	2002      	movs	r0, #2
300063e6:	eb01 3105 	add.w	r1, r1, r5, lsl #12
300063ea:	3501      	adds	r5, #1
300063ec:	f7fd fc76 	bl	30003cdc <FLASH_Erase_With_Lock>
300063f0:	2032      	movs	r0, #50	; 0x32
300063f2:	47a0      	blx	r4
300063f4:	42ae      	cmp	r6, r5
300063f6:	d1f3      	bne.n	300063e0 <BOOT_Image1+0x860>
300063f8:	f108 0801 	add.w	r8, r8, #1
300063fc:	f8d7 34d8 	ldr.w	r3, [r7, #1240]	; 0x4d8
30006400:	f10a 0a04 	add.w	sl, sl, #4
30006404:	4543      	cmp	r3, r8
30006406:	d8e6      	bhi.n	300063d6 <BOOT_Image1+0x856>
30006408:	482b      	ldr	r0, [pc, #172]	; (300064b8 <BOOT_Image1+0x938>)
3000640a:	f005 ffe5 	bl	3000c3d8 <__DiagPrintf_veneer>
3000640e:	4b2b      	ldr	r3, [pc, #172]	; (300064bc <BOOT_Image1+0x93c>)
30006410:	a804      	add	r0, sp, #16
30006412:	4798      	blx	r3
30006414:	f44f 72fa 	mov.w	r2, #500	; 0x1f4
30006418:	4829      	ldr	r0, [pc, #164]	; (300064c0 <BOOT_Image1+0x940>)
3000641a:	a904      	add	r1, sp, #16
3000641c:	4b29      	ldr	r3, [pc, #164]	; (300064c4 <BOOT_Image1+0x944>)
3000641e:	f8ad 2012 	strh.w	r2, [sp, #18]
30006422:	4798      	blx	r3
30006424:	4826      	ldr	r0, [pc, #152]	; (300064c0 <BOOT_Image1+0x940>)
30006426:	4b28      	ldr	r3, [pc, #160]	; (300064c8 <BOOT_Image1+0x948>)
30006428:	4798      	blx	r3
3000642a:	e7fe      	b.n	3000642a <BOOT_Image1+0x8aa>
3000642c:	4827      	ldr	r0, [pc, #156]	; (300064cc <BOOT_Image1+0x94c>)
3000642e:	f005 ffd3 	bl	3000c3d8 <__DiagPrintf_veneer>
30006432:	481b      	ldr	r0, [pc, #108]	; (300064a0 <BOOT_Image1+0x920>)
30006434:	f005 ffd0 	bl	3000c3d8 <__DiagPrintf_veneer>
30006438:	e65e      	b.n	300060f8 <BOOT_Image1+0x578>
3000643a:	4825      	ldr	r0, [pc, #148]	; (300064d0 <BOOT_Image1+0x950>)
3000643c:	f005 ffcc 	bl	3000c3d8 <__DiagPrintf_veneer>
30006440:	e65a      	b.n	300060f8 <BOOT_Image1+0x578>
30006442:	4b24      	ldr	r3, [pc, #144]	; (300064d4 <BOOT_Image1+0x954>)
30006444:	4798      	blx	r3
30006446:	2801      	cmp	r0, #1
30006448:	bf0c      	ite	eq
3000644a:	20e6      	moveq	r0, #230	; 0xe6
3000644c:	2019      	movne	r0, #25
3000644e:	47a0      	blx	r4
30006450:	e680      	b.n	30006154 <BOOT_Image1+0x5d4>
30006452:	4b20      	ldr	r3, [pc, #128]	; (300064d4 <BOOT_Image1+0x954>)
30006454:	4798      	blx	r3
30006456:	2801      	cmp	r0, #1
30006458:	bf0c      	ite	eq
3000645a:	2019      	moveq	r0, #25
3000645c:	2004      	movne	r0, #4
3000645e:	47a0      	blx	r4
30006460:	e665      	b.n	3000612e <BOOT_Image1+0x5ae>
30006462:	481d      	ldr	r0, [pc, #116]	; (300064d8 <BOOT_Image1+0x958>)
30006464:	f005 ffb8 	bl	3000c3d8 <__DiagPrintf_veneer>
30006468:	e646      	b.n	300060f8 <BOOT_Image1+0x578>
3000646a:	46d3      	mov	fp, sl
3000646c:	e5fd      	b.n	3000606a <BOOT_Image1+0x4ea>
3000646e:	46da      	mov	sl, fp
30006470:	f04f 0900 	mov.w	r9, #0
30006474:	e5cf      	b.n	30006016 <BOOT_Image1+0x496>
30006476:	2600      	movs	r6, #0
30006478:	f04f 080a 	mov.w	r8, #10
3000647c:	4633      	mov	r3, r6
3000647e:	46b3      	mov	fp, r6
30006480:	e59a      	b.n	30005fb8 <BOOT_Image1+0x438>
30006482:	f106 080b 	add.w	r8, r6, #11
30006486:	360a      	adds	r6, #10
30006488:	44c8      	add	r8, r9
3000648a:	e778      	b.n	3000637e <BOOT_Image1+0x7fe>
3000648c:	3000edbc 	.word	0x3000edbc
30006490:	3000ede4 	.word	0x3000ede4
30006494:	000129bd 	.word	0x000129bd
30006498:	3000ede8 	.word	0x3000ede8
3000649c:	3000ee10 	.word	0x3000ee10
300064a0:	3000ce3c 	.word	0x3000ce3c
300064a4:	0000c179 	.word	0x0000c179
300064a8:	41000200 	.word	0x41000200
300064ac:	3000e89c 	.word	0x3000e89c
300064b0:	3000f69c 	.word	0x3000f69c
300064b4:	3000ed7c 	.word	0x3000ed7c
300064b8:	3000ce74 	.word	0x3000ce74
300064bc:	0000d3f9 	.word	0x0000d3f9
300064c0:	41000440 	.word	0x41000440
300064c4:	0000d409 	.word	0x0000d409
300064c8:	0000d489 	.word	0x0000d489
300064cc:	3000ce24 	.word	0x3000ce24
300064d0:	3000ce4c 	.word	0x3000ce4c
300064d4:	0000c169 	.word	0x0000c169
300064d8:	3000ce60 	.word	0x3000ce60

300064dc <BOOT_LoadSubImage>:
300064dc:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
300064e0:	b08f      	sub	sp, #60	; 0x3c
300064e2:	469a      	mov	sl, r3
300064e4:	460f      	mov	r7, r1
300064e6:	f89d 3060 	ldrb.w	r3, [sp, #96]	; 0x60
300064ea:	9305      	str	r3, [sp, #20]
300064ec:	b39a      	cbz	r2, 30006556 <BOOT_LoadSubImage+0x7a>
300064ee:	4691      	mov	r9, r2
300064f0:	2600      	movs	r6, #0
300064f2:	4605      	mov	r5, r0
300064f4:	2220      	movs	r2, #32
300064f6:	4639      	mov	r1, r7
300064f8:	a806      	add	r0, sp, #24
300064fa:	f7fe fa25 	bl	30004948 <BOOT_ImgCopy>
300064fe:	9b06      	ldr	r3, [sp, #24]
30006500:	4a49      	ldr	r2, [pc, #292]	; (30006628 <BOOT_LoadSubImage+0x14c>)
30006502:	4293      	cmp	r3, r2
30006504:	d157      	bne.n	300065b6 <BOOT_LoadSubImage+0xda>
30006506:	f102 427b 	add.w	r2, r2, #4211081216	; 0xfb000000
3000650a:	9b07      	ldr	r3, [sp, #28]
3000650c:	f502 0278 	add.w	r2, r2, #16252928	; 0xf80000
30006510:	f502 62c0 	add.w	r2, r2, #1536	; 0x600
30006514:	4293      	cmp	r3, r2
30006516:	d14e      	bne.n	300065b6 <BOOT_LoadSubImage+0xda>
30006518:	4b44      	ldr	r3, [pc, #272]	; (3000662c <BOOT_LoadSubImage+0x150>)
3000651a:	e9dd 4008 	ldrd	r4, r0, [sp, #32]
3000651e:	18c3      	adds	r3, r0, r3
30006520:	f1a0 0820 	sub.w	r8, r0, #32
30006524:	3420      	adds	r4, #32
30006526:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000652a:	d21a      	bcs.n	30006562 <BOOT_LoadSubImage+0x86>
3000652c:	b17d      	cbz	r5, 3000654e <BOOT_LoadSubImage+0x72>
3000652e:	1d2b      	adds	r3, r5, #4
30006530:	f845 8036 	str.w	r8, [r5, r6, lsl #3]
30006534:	4a3e      	ldr	r2, [pc, #248]	; (30006630 <BOOT_LoadSubImage+0x154>)
30006536:	2004      	movs	r0, #4
30006538:	e9cd 8401 	strd	r8, r4, [sp, #4]
3000653c:	f843 4036 	str.w	r4, [r3, r6, lsl #3]
30006540:	f85a 3026 	ldr.w	r3, [sl, r6, lsl #2]
30006544:	9300      	str	r3, [sp, #0]
30006546:	4b3b      	ldr	r3, [pc, #236]	; (30006634 <BOOT_LoadSubImage+0x158>)
30006548:	4619      	mov	r1, r3
3000654a:	f005 fccf 	bl	3000beec <rtk_log_write>
3000654e:	3601      	adds	r6, #1
30006550:	4427      	add	r7, r4
30006552:	454e      	cmp	r6, r9
30006554:	d1ce      	bne.n	300064f4 <BOOT_LoadSubImage+0x18>
30006556:	4b38      	ldr	r3, [pc, #224]	; (30006638 <BOOT_LoadSubImage+0x15c>)
30006558:	2001      	movs	r0, #1
3000655a:	601f      	str	r7, [r3, #0]
3000655c:	b00f      	add	sp, #60	; 0x3c
3000655e:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006562:	4639      	mov	r1, r7
30006564:	4622      	mov	r2, r4
30006566:	4640      	mov	r0, r8
30006568:	f7fe f9ee 	bl	30004948 <BOOT_ImgCopy>
3000656c:	4933      	ldr	r1, [pc, #204]	; (3000663c <BOOT_LoadSubImage+0x160>)
3000656e:	694b      	ldr	r3, [r1, #20]
30006570:	03db      	lsls	r3, r3, #15
30006572:	d5db      	bpl.n	3000652c <BOOT_LoadSubImage+0x50>
30006574:	ea08 0304 	and.w	r3, r8, r4
30006578:	3301      	adds	r3, #1
3000657a:	d023      	beq.n	300065c4 <BOOT_LoadSubImage+0xe8>
3000657c:	f018 031f 	ands.w	r3, r8, #31
30006580:	d042      	beq.n	30006608 <BOOT_LoadSubImage+0x12c>
30006582:	4423      	add	r3, r4
30006584:	f028 021f 	bic.w	r2, r8, #31
30006588:	f3bf 8f4f 	dsb	sy
3000658c:	2b00      	cmp	r3, #0
3000658e:	dd0d      	ble.n	300065ac <BOOT_LoadSubImage+0xd0>
30006590:	1e59      	subs	r1, r3, #1
30006592:	f102 0320 	add.w	r3, r2, #32
30006596:	4829      	ldr	r0, [pc, #164]	; (3000663c <BOOT_LoadSubImage+0x160>)
30006598:	f021 011f 	bic.w	r1, r1, #31
3000659c:	4419      	add	r1, r3
3000659e:	428b      	cmp	r3, r1
300065a0:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
300065a4:	461a      	mov	r2, r3
300065a6:	f103 0320 	add.w	r3, r3, #32
300065aa:	d1f8      	bne.n	3000659e <BOOT_LoadSubImage+0xc2>
300065ac:	f3bf 8f4f 	dsb	sy
300065b0:	f3bf 8f6f 	isb	sy
300065b4:	e7ba      	b.n	3000652c <BOOT_LoadSubImage+0x50>
300065b6:	9b05      	ldr	r3, [sp, #20]
300065b8:	2b01      	cmp	r3, #1
300065ba:	d028      	beq.n	3000660e <BOOT_LoadSubImage+0x132>
300065bc:	2000      	movs	r0, #0
300065be:	b00f      	add	sp, #60	; 0x3c
300065c0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300065c4:	2300      	movs	r3, #0
300065c6:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300065ca:	f3bf 8f4f 	dsb	sy
300065ce:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
300065d2:	f643 7be0 	movw	fp, #16352	; 0x3fe0
300065d6:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
300065da:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300065de:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300065e2:	ea0c 000b 	and.w	r0, ip, fp
300065e6:	4673      	mov	r3, lr
300065e8:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300065ec:	3b01      	subs	r3, #1
300065ee:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300065f2:	d2f9      	bcs.n	300065e8 <BOOT_LoadSubImage+0x10c>
300065f4:	f1ac 0c20 	sub.w	ip, ip, #32
300065f8:	f11c 0f20 	cmn.w	ip, #32
300065fc:	d1f1      	bne.n	300065e2 <BOOT_LoadSubImage+0x106>
300065fe:	f3bf 8f4f 	dsb	sy
30006602:	f3bf 8f6f 	isb	sy
30006606:	e791      	b.n	3000652c <BOOT_LoadSubImage+0x50>
30006608:	4623      	mov	r3, r4
3000660a:	4642      	mov	r2, r8
3000660c:	e7bc      	b.n	30006588 <BOOT_LoadSubImage+0xac>
3000660e:	f85a 4026 	ldr.w	r4, [sl, r6, lsl #2]
30006612:	2004      	movs	r0, #4
30006614:	4b07      	ldr	r3, [pc, #28]	; (30006634 <BOOT_LoadSubImage+0x158>)
30006616:	4a0a      	ldr	r2, [pc, #40]	; (30006640 <BOOT_LoadSubImage+0x164>)
30006618:	4619      	mov	r1, r3
3000661a:	9400      	str	r4, [sp, #0]
3000661c:	f005 fc66 	bl	3000beec <rtk_log_write>
30006620:	2000      	movs	r0, #0
30006622:	b00f      	add	sp, #60	; 0x3c
30006624:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006628:	35393138 	.word	0x35393138
3000662c:	f7ffffe0 	.word	0xf7ffffe0
30006630:	3000cf1c 	.word	0x3000cf1c
30006634:	3000c6b0 	.word	0x3000c6b0
30006638:	3000ee14 	.word	0x3000ee14
3000663c:	e000ed00 	.word	0xe000ed00
30006640:	3000cf08 	.word	0x3000cf08

30006644 <BOOT_LoadAPSubImage>:
30006644:	4bb5      	ldr	r3, [pc, #724]	; (3000691c <BOOT_LoadAPSubImage+0x2d8>)
30006646:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000664a:	460c      	mov	r4, r1
3000664c:	4606      	mov	r6, r0
3000664e:	b08f      	sub	sp, #60	; 0x3c
30006650:	4615      	mov	r5, r2
30006652:	2220      	movs	r2, #32
30006654:	e893 0003 	ldmia.w	r3, {r0, r1}
30006658:	0c0b      	lsrs	r3, r1, #16
3000665a:	9004      	str	r0, [sp, #16]
3000665c:	f8ad 1014 	strh.w	r1, [sp, #20]
30006660:	a806      	add	r0, sp, #24
30006662:	4621      	mov	r1, r4
30006664:	f88d 3016 	strb.w	r3, [sp, #22]
30006668:	f7fe f96e 	bl	30004948 <BOOT_ImgCopy>
3000666c:	4bac      	ldr	r3, [pc, #688]	; (30006920 <BOOT_LoadAPSubImage+0x2dc>)
3000666e:	9a06      	ldr	r2, [sp, #24]
30006670:	429a      	cmp	r2, r3
30006672:	d108      	bne.n	30006686 <BOOT_LoadAPSubImage+0x42>
30006674:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006678:	9a07      	ldr	r2, [sp, #28]
3000667a:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
3000667e:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006682:	429a      	cmp	r2, r3
30006684:	d006      	beq.n	30006694 <BOOT_LoadAPSubImage+0x50>
30006686:	2d01      	cmp	r5, #1
30006688:	f000 80b3 	beq.w	300067f2 <BOOT_LoadAPSubImage+0x1ae>
3000668c:	2000      	movs	r0, #0
3000668e:	b00f      	add	sp, #60	; 0x3c
30006690:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006694:	9a09      	ldr	r2, [sp, #36]	; 0x24
30006696:	4ba3      	ldr	r3, [pc, #652]	; (30006924 <BOOT_LoadAPSubImage+0x2e0>)
30006698:	6ca7      	ldr	r7, [r4, #72]	; 0x48
3000669a:	f1a2 0820 	sub.w	r8, r2, #32
3000669e:	4413      	add	r3, r2
300066a0:	37d0      	adds	r7, #208	; 0xd0
300066a2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300066a6:	d247      	bcs.n	30006738 <BOOT_LoadAPSubImage+0xf4>
300066a8:	4b9f      	ldr	r3, [pc, #636]	; (30006928 <BOOT_LoadAPSubImage+0x2e4>)
300066aa:	eb07 0508 	add.w	r5, r7, r8
300066ae:	f8d4 8070 	ldr.w	r8, [r4, #112]	; 0x70
300066b2:	4798      	blx	r3
300066b4:	2801      	cmp	r0, #1
300066b6:	4427      	add	r7, r4
300066b8:	4b9c      	ldr	r3, [pc, #624]	; (3000692c <BOOT_LoadAPSubImage+0x2e8>)
300066ba:	d029      	beq.n	30006710 <BOOT_LoadAPSubImage+0xcc>
300066bc:	4619      	mov	r1, r3
300066be:	4a9c      	ldr	r2, [pc, #624]	; (30006930 <BOOT_LoadAPSubImage+0x2ec>)
300066c0:	2004      	movs	r0, #4
300066c2:	f005 fc13 	bl	3000beec <rtk_log_write>
300066c6:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
300066ca:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300066ce:	f080 809c 	bcs.w	3000680a <BOOT_LoadAPSubImage+0x1c6>
300066d2:	4445      	add	r5, r8
300066d4:	eb08 0107 	add.w	r1, r8, r7
300066d8:	f105 4378 	add.w	r3, r5, #4160749568	; 0xf8000000
300066dc:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300066e0:	d256      	bcs.n	30006790 <BOOT_LoadAPSubImage+0x14c>
300066e2:	2e00      	cmp	r6, #0
300066e4:	f000 8081 	beq.w	300067ea <BOOT_LoadAPSubImage+0x1a6>
300066e8:	4b90      	ldr	r3, [pc, #576]	; (3000692c <BOOT_LoadAPSubImage+0x2e8>)
300066ea:	aa04      	add	r2, sp, #16
300066ec:	2004      	movs	r0, #4
300066ee:	9200      	str	r2, [sp, #0]
300066f0:	4a90      	ldr	r2, [pc, #576]	; (30006934 <BOOT_LoadAPSubImage+0x2f0>)
300066f2:	e9dd 1408 	ldrd	r1, r4, [sp, #32]
300066f6:	3c20      	subs	r4, #32
300066f8:	3120      	adds	r1, #32
300066fa:	e9c6 4100 	strd	r4, r1, [r6]
300066fe:	9102      	str	r1, [sp, #8]
30006700:	4619      	mov	r1, r3
30006702:	9401      	str	r4, [sp, #4]
30006704:	f005 fbf2 	bl	3000beec <rtk_log_write>
30006708:	2001      	movs	r0, #1
3000670a:	b00f      	add	sp, #60	; 0x3c
3000670c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30006710:	4a89      	ldr	r2, [pc, #548]	; (30006938 <BOOT_LoadAPSubImage+0x2f4>)
30006712:	2004      	movs	r0, #4
30006714:	4619      	mov	r1, r3
30006716:	f005 fbe9 	bl	3000beec <rtk_log_write>
3000671a:	4642      	mov	r2, r8
3000671c:	4629      	mov	r1, r5
3000671e:	4638      	mov	r0, r7
30006720:	f000 fbe4 	bl	30006eec <BOOT_LoadBL32RDPImg>
30006724:	2800      	cmp	r0, #0
30006726:	d1d4      	bne.n	300066d2 <BOOT_LoadAPSubImage+0x8e>
30006728:	4b80      	ldr	r3, [pc, #512]	; (3000692c <BOOT_LoadAPSubImage+0x2e8>)
3000672a:	2004      	movs	r0, #4
3000672c:	4a83      	ldr	r2, [pc, #524]	; (3000693c <BOOT_LoadAPSubImage+0x2f8>)
3000672e:	4619      	mov	r1, r3
30006730:	f005 fbdc 	bl	3000beec <rtk_log_write>
30006734:	20ff      	movs	r0, #255	; 0xff
30006736:	e7aa      	b.n	3000668e <BOOT_LoadAPSubImage+0x4a>
30006738:	463a      	mov	r2, r7
3000673a:	4621      	mov	r1, r4
3000673c:	4640      	mov	r0, r8
3000673e:	f7fe f903 	bl	30004948 <BOOT_ImgCopy>
30006742:	497f      	ldr	r1, [pc, #508]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
30006744:	694b      	ldr	r3, [r1, #20]
30006746:	03da      	lsls	r2, r3, #15
30006748:	d5ae      	bpl.n	300066a8 <BOOT_LoadAPSubImage+0x64>
3000674a:	ea07 0308 	and.w	r3, r7, r8
3000674e:	3301      	adds	r3, #1
30006750:	f000 8086 	beq.w	30006860 <BOOT_LoadAPSubImage+0x21c>
30006754:	f018 031f 	ands.w	r3, r8, #31
30006758:	f000 80bb 	beq.w	300068d2 <BOOT_LoadAPSubImage+0x28e>
3000675c:	443b      	add	r3, r7
3000675e:	f028 021f 	bic.w	r2, r8, #31
30006762:	f3bf 8f4f 	dsb	sy
30006766:	2b00      	cmp	r3, #0
30006768:	dd0d      	ble.n	30006786 <BOOT_LoadAPSubImage+0x142>
3000676a:	1e59      	subs	r1, r3, #1
3000676c:	f102 0320 	add.w	r3, r2, #32
30006770:	4873      	ldr	r0, [pc, #460]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
30006772:	f021 011f 	bic.w	r1, r1, #31
30006776:	4419      	add	r1, r3
30006778:	428b      	cmp	r3, r1
3000677a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000677e:	461a      	mov	r2, r3
30006780:	f103 0320 	add.w	r3, r3, #32
30006784:	d1f8      	bne.n	30006778 <BOOT_LoadAPSubImage+0x134>
30006786:	f3bf 8f4f 	dsb	sy
3000678a:	f3bf 8f6f 	isb	sy
3000678e:	e78b      	b.n	300066a8 <BOOT_LoadAPSubImage+0x64>
30006790:	f8d4 4098 	ldr.w	r4, [r4, #152]	; 0x98
30006794:	4628      	mov	r0, r5
30006796:	4622      	mov	r2, r4
30006798:	f7fe f8d6 	bl	30004948 <BOOT_ImgCopy>
3000679c:	4968      	ldr	r1, [pc, #416]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
3000679e:	694b      	ldr	r3, [r1, #20]
300067a0:	03d8      	lsls	r0, r3, #15
300067a2:	d59e      	bpl.n	300066e2 <BOOT_LoadAPSubImage+0x9e>
300067a4:	ea05 0304 	and.w	r3, r5, r4
300067a8:	3301      	adds	r3, #1
300067aa:	d075      	beq.n	30006898 <BOOT_LoadAPSubImage+0x254>
300067ac:	f015 031f 	ands.w	r3, r5, #31
300067b0:	d002      	beq.n	300067b8 <BOOT_LoadAPSubImage+0x174>
300067b2:	f025 051f 	bic.w	r5, r5, #31
300067b6:	441c      	add	r4, r3
300067b8:	f3bf 8f4f 	dsb	sy
300067bc:	2c00      	cmp	r4, #0
300067be:	dd0d      	ble.n	300067dc <BOOT_LoadAPSubImage+0x198>
300067c0:	1e62      	subs	r2, r4, #1
300067c2:	f105 0320 	add.w	r3, r5, #32
300067c6:	495e      	ldr	r1, [pc, #376]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
300067c8:	f022 021f 	bic.w	r2, r2, #31
300067cc:	441a      	add	r2, r3
300067ce:	4293      	cmp	r3, r2
300067d0:	f8c1 5270 	str.w	r5, [r1, #624]	; 0x270
300067d4:	461d      	mov	r5, r3
300067d6:	f103 0320 	add.w	r3, r3, #32
300067da:	d1f8      	bne.n	300067ce <BOOT_LoadAPSubImage+0x18a>
300067dc:	f3bf 8f4f 	dsb	sy
300067e0:	f3bf 8f6f 	isb	sy
300067e4:	2e00      	cmp	r6, #0
300067e6:	f47f af7f 	bne.w	300066e8 <BOOT_LoadAPSubImage+0xa4>
300067ea:	2001      	movs	r0, #1
300067ec:	b00f      	add	sp, #60	; 0x3c
300067ee:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300067f2:	4b4e      	ldr	r3, [pc, #312]	; (3000692c <BOOT_LoadAPSubImage+0x2e8>)
300067f4:	ac04      	add	r4, sp, #16
300067f6:	4a53      	ldr	r2, [pc, #332]	; (30006944 <BOOT_LoadAPSubImage+0x300>)
300067f8:	2004      	movs	r0, #4
300067fa:	4619      	mov	r1, r3
300067fc:	9400      	str	r4, [sp, #0]
300067fe:	f005 fb75 	bl	3000beec <rtk_log_write>
30006802:	2000      	movs	r0, #0
30006804:	b00f      	add	sp, #60	; 0x3c
30006806:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000680a:	4639      	mov	r1, r7
3000680c:	4642      	mov	r2, r8
3000680e:	4628      	mov	r0, r5
30006810:	f7fe f89a 	bl	30004948 <BOOT_ImgCopy>
30006814:	494a      	ldr	r1, [pc, #296]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
30006816:	694b      	ldr	r3, [r1, #20]
30006818:	03db      	lsls	r3, r3, #15
3000681a:	f57f af5a 	bpl.w	300066d2 <BOOT_LoadAPSubImage+0x8e>
3000681e:	ea08 0305 	and.w	r3, r8, r5
30006822:	3301      	adds	r3, #1
30006824:	d058      	beq.n	300068d8 <BOOT_LoadAPSubImage+0x294>
30006826:	f015 031f 	ands.w	r3, r5, #31
3000682a:	d073      	beq.n	30006914 <BOOT_LoadAPSubImage+0x2d0>
3000682c:	4443      	add	r3, r8
3000682e:	f025 021f 	bic.w	r2, r5, #31
30006832:	f3bf 8f4f 	dsb	sy
30006836:	2b00      	cmp	r3, #0
30006838:	dd0d      	ble.n	30006856 <BOOT_LoadAPSubImage+0x212>
3000683a:	1e59      	subs	r1, r3, #1
3000683c:	f102 0320 	add.w	r3, r2, #32
30006840:	483f      	ldr	r0, [pc, #252]	; (30006940 <BOOT_LoadAPSubImage+0x2fc>)
30006842:	f021 011f 	bic.w	r1, r1, #31
30006846:	4419      	add	r1, r3
30006848:	428b      	cmp	r3, r1
3000684a:	f8c0 2270 	str.w	r2, [r0, #624]	; 0x270
3000684e:	461a      	mov	r2, r3
30006850:	f103 0320 	add.w	r3, r3, #32
30006854:	d1f8      	bne.n	30006848 <BOOT_LoadAPSubImage+0x204>
30006856:	f3bf 8f4f 	dsb	sy
3000685a:	f3bf 8f6f 	isb	sy
3000685e:	e738      	b.n	300066d2 <BOOT_LoadAPSubImage+0x8e>
30006860:	2300      	movs	r3, #0
30006862:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
30006866:	f3bf 8f4f 	dsb	sy
3000686a:	f8d1 0080 	ldr.w	r0, [r1, #128]	; 0x80
3000686e:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
30006872:	f3c0 324e 	ubfx	r2, r0, #13, #15
30006876:	f3c0 00c9 	ubfx	r0, r0, #3, #10
3000687a:	0152      	lsls	r2, r2, #5
3000687c:	ea02 0c0e 	and.w	ip, r2, lr
30006880:	4603      	mov	r3, r0
30006882:	ea4c 7583 	orr.w	r5, ip, r3, lsl #30
30006886:	3b01      	subs	r3, #1
30006888:	f8c1 5274 	str.w	r5, [r1, #628]	; 0x274
3000688c:	d2f9      	bcs.n	30006882 <BOOT_LoadAPSubImage+0x23e>
3000688e:	3a20      	subs	r2, #32
30006890:	f112 0f20 	cmn.w	r2, #32
30006894:	d1f2      	bne.n	3000687c <BOOT_LoadAPSubImage+0x238>
30006896:	e776      	b.n	30006786 <BOOT_LoadAPSubImage+0x142>
30006898:	2300      	movs	r3, #0
3000689a:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
3000689e:	f3bf 8f4f 	dsb	sy
300068a2:	f8d1 5080 	ldr.w	r5, [r1, #128]	; 0x80
300068a6:	f643 77e0 	movw	r7, #16352	; 0x3fe0
300068aa:	f3c5 344e 	ubfx	r4, r5, #13, #15
300068ae:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300068b2:	0164      	lsls	r4, r4, #5
300068b4:	ea04 0007 	and.w	r0, r4, r7
300068b8:	462b      	mov	r3, r5
300068ba:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
300068be:	3b01      	subs	r3, #1
300068c0:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
300068c4:	1c5a      	adds	r2, r3, #1
300068c6:	d1f8      	bne.n	300068ba <BOOT_LoadAPSubImage+0x276>
300068c8:	3c20      	subs	r4, #32
300068ca:	f114 0f20 	cmn.w	r4, #32
300068ce:	d1f1      	bne.n	300068b4 <BOOT_LoadAPSubImage+0x270>
300068d0:	e784      	b.n	300067dc <BOOT_LoadAPSubImage+0x198>
300068d2:	463b      	mov	r3, r7
300068d4:	4642      	mov	r2, r8
300068d6:	e744      	b.n	30006762 <BOOT_LoadAPSubImage+0x11e>
300068d8:	2300      	movs	r3, #0
300068da:	f8c1 3084 	str.w	r3, [r1, #132]	; 0x84
300068de:	f3bf 8f4f 	dsb	sy
300068e2:	f8d1 3080 	ldr.w	r3, [r1, #128]	; 0x80
300068e6:	f643 79e0 	movw	r9, #16352	; 0x3fe0
300068ea:	f3c3 3c4e 	ubfx	ip, r3, #13, #15
300068ee:	f3c3 0ec9 	ubfx	lr, r3, #3, #10
300068f2:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
300068f6:	ea0c 0009 	and.w	r0, ip, r9
300068fa:	4673      	mov	r3, lr
300068fc:	ea40 7283 	orr.w	r2, r0, r3, lsl #30
30006900:	3b01      	subs	r3, #1
30006902:	f8c1 2274 	str.w	r2, [r1, #628]	; 0x274
30006906:	d2f9      	bcs.n	300068fc <BOOT_LoadAPSubImage+0x2b8>
30006908:	f1ac 0c20 	sub.w	ip, ip, #32
3000690c:	f11c 0f20 	cmn.w	ip, #32
30006910:	d1f1      	bne.n	300068f6 <BOOT_LoadAPSubImage+0x2b2>
30006912:	e7a0      	b.n	30006856 <BOOT_LoadAPSubImage+0x212>
30006914:	4643      	mov	r3, r8
30006916:	462a      	mov	r2, r5
30006918:	e78b      	b.n	30006832 <BOOT_LoadAPSubImage+0x1ee>
3000691a:	bf00      	nop
3000691c:	3000cf00 	.word	0x3000cf00
30006920:	35393138 	.word	0x35393138
30006924:	f7ffffe0 	.word	0xf7ffffe0
30006928:	0000c109 	.word	0x0000c109
3000692c:	3000c6b0 	.word	0x3000c6b0
30006930:	3000cf60 	.word	0x3000cf60
30006934:	3000cf1c 	.word	0x3000cf1c
30006938:	3000cf30 	.word	0x3000cf30
3000693c:	3000cf40 	.word	0x3000cf40
30006940:	e000ed00 	.word	0xe000ed00
30006944:	3000cf08 	.word	0x3000cf08

30006948 <BOOT_RSIPIvSet>:
30006948:	b530      	push	{r4, r5, lr}
3000694a:	2300      	movs	r3, #0
3000694c:	b083      	sub	sp, #12
3000694e:	4a09      	ldr	r2, [pc, #36]	; (30006974 <BOOT_RSIPIvSet+0x2c>)
30006950:	4605      	mov	r5, r0
30006952:	460c      	mov	r4, r1
30006954:	9300      	str	r3, [sp, #0]
30006956:	9301      	str	r3, [sp, #4]
30006958:	4790      	blx	r2
3000695a:	b148      	cbz	r0, 30006970 <BOOT_RSIPIvSet+0x28>
3000695c:	f105 0120 	add.w	r1, r5, #32
30006960:	4b05      	ldr	r3, [pc, #20]	; (30006978 <BOOT_RSIPIvSet+0x30>)
30006962:	2208      	movs	r2, #8
30006964:	4668      	mov	r0, sp
30006966:	4798      	blx	r3
30006968:	4669      	mov	r1, sp
3000696a:	b2e0      	uxtb	r0, r4
3000696c:	4b03      	ldr	r3, [pc, #12]	; (3000697c <BOOT_RSIPIvSet+0x34>)
3000696e:	4798      	blx	r3
30006970:	b003      	add	sp, #12
30006972:	bd30      	pop	{r4, r5, pc}
30006974:	0000c129 	.word	0x0000c129
30006978:	00012a1d 	.word	0x00012a1d
3000697c:	300098a5 	.word	0x300098a5

30006980 <BOOT_OTFCheck>:
30006980:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30006984:	4d1c      	ldr	r5, [pc, #112]	; (300069f8 <BOOT_OTFCheck+0x78>)
30006986:	b082      	sub	sp, #8
30006988:	4604      	mov	r4, r0
3000698a:	4688      	mov	r8, r1
3000698c:	4617      	mov	r7, r2
3000698e:	461e      	mov	r6, r3
30006990:	47a8      	blx	r5
30006992:	b148      	cbz	r0, 300069a8 <BOOT_OTFCheck+0x28>
30006994:	f104 4378 	add.w	r3, r4, #4160749568	; 0xf8000000
30006998:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
3000699c:	d204      	bcs.n	300069a8 <BOOT_OTFCheck+0x28>
3000699e:	f108 4378 	add.w	r3, r8, #4160749568	; 0xf8000000
300069a2:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
300069a6:	d302      	bcc.n	300069ae <BOOT_OTFCheck+0x2e>
300069a8:	b002      	add	sp, #8
300069aa:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
300069ae:	4b13      	ldr	r3, [pc, #76]	; (300069fc <BOOT_OTFCheck+0x7c>)
300069b0:	2004      	movs	r0, #4
300069b2:	4a13      	ldr	r2, [pc, #76]	; (30006a00 <BOOT_OTFCheck+0x80>)
300069b4:	4619      	mov	r1, r3
300069b6:	f005 fa99 	bl	3000beec <rtk_log_write>
300069ba:	4b12      	ldr	r3, [pc, #72]	; (30006a04 <BOOT_OTFCheck+0x84>)
300069bc:	4798      	blx	r3
300069be:	4603      	mov	r3, r0
300069c0:	b150      	cbz	r0, 300069d8 <BOOT_OTFCheck+0x58>
300069c2:	2801      	cmp	r0, #1
300069c4:	d009      	beq.n	300069da <BOOT_OTFCheck+0x5a>
300069c6:	4b0d      	ldr	r3, [pc, #52]	; (300069fc <BOOT_OTFCheck+0x7c>)
300069c8:	2002      	movs	r0, #2
300069ca:	4a0f      	ldr	r2, [pc, #60]	; (30006a08 <BOOT_OTFCheck+0x88>)
300069cc:	4619      	mov	r1, r3
300069ce:	b002      	add	sp, #8
300069d0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300069d4:	f005 ba8a 	b.w	3000beec <rtk_log_write>
300069d8:	2302      	movs	r3, #2
300069da:	4630      	mov	r0, r6
300069dc:	4621      	mov	r1, r4
300069de:	9301      	str	r3, [sp, #4]
300069e0:	4642      	mov	r2, r8
300069e2:	2301      	movs	r3, #1
300069e4:	4c09      	ldr	r4, [pc, #36]	; (30006a0c <BOOT_OTFCheck+0x8c>)
300069e6:	9700      	str	r7, [sp, #0]
300069e8:	47a0      	blx	r4
300069ea:	2001      	movs	r0, #1
300069ec:	4b08      	ldr	r3, [pc, #32]	; (30006a10 <BOOT_OTFCheck+0x90>)
300069ee:	b002      	add	sp, #8
300069f0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
300069f4:	4718      	bx	r3
300069f6:	bf00      	nop
300069f8:	0000c129 	.word	0x0000c129
300069fc:	3000c6b0 	.word	0x3000c6b0
30006a00:	3000cf70 	.word	0x3000cf70
30006a04:	0000c245 	.word	0x0000c245
30006a08:	3000cf84 	.word	0x3000cf84
30006a0c:	300098d9 	.word	0x300098d9
30006a10:	300098c1 	.word	0x300098c1

30006a14 <BOOT_OTA_LoadIMG2>:
30006a14:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006a18:	4b6e      	ldr	r3, [pc, #440]	; (30006bd4 <BOOT_OTA_LoadIMG2+0x1c0>)
30006a1a:	b08d      	sub	sp, #52	; 0x34
30006a1c:	4a6e      	ldr	r2, [pc, #440]	; (30006bd8 <BOOT_OTA_LoadIMG2+0x1c4>)
30006a1e:	f103 040c 	add.w	r4, r3, #12
30006a22:	9003      	str	r0, [sp, #12]
30006a24:	f8d2 8000 	ldr.w	r8, [r2]
30006a28:	ae06      	add	r6, sp, #24
30006a2a:	e893 0007 	ldmia.w	r3, {r0, r1, r2}
30006a2e:	ab09      	add	r3, sp, #36	; 0x24
30006a30:	e886 0007 	stmia.w	r6, {r0, r1, r2}
30006a34:	e894 0007 	ldmia.w	r4, {r0, r1, r2}
30006a38:	e883 0007 	stmia.w	r3, {r0, r1, r2}
30006a3c:	4641      	mov	r1, r8
30006a3e:	4867      	ldr	r0, [pc, #412]	; (30006bdc <BOOT_OTA_LoadIMG2+0x1c8>)
30006a40:	f44f 72b2 	mov.w	r2, #356	; 0x164
30006a44:	f7fd ff80 	bl	30004948 <BOOT_ImgCopy>
30006a48:	4641      	mov	r1, r8
30006a4a:	4864      	ldr	r0, [pc, #400]	; (30006bdc <BOOT_OTA_LoadIMG2+0x1c8>)
30006a4c:	f000 fd2a 	bl	300074a4 <BOOT_CertificateCheck>
30006a50:	2800      	cmp	r0, #0
30006a52:	f040 80b7 	bne.w	30006bc4 <BOOT_OTA_LoadIMG2+0x1b0>
30006a56:	4604      	mov	r4, r0
30006a58:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006a5c:	f508 617f 	add.w	r1, r8, #4080	; 0xff0
30006a60:	485f      	ldr	r0, [pc, #380]	; (30006be0 <BOOT_OTA_LoadIMG2+0x1cc>)
30006a62:	f7fd ff71 	bl	30004948 <BOOT_ImgCopy>
30006a66:	4b5f      	ldr	r3, [pc, #380]	; (30006be4 <BOOT_OTA_LoadIMG2+0x1d0>)
30006a68:	e9cd 4404 	strd	r4, r4, [sp, #16]
30006a6c:	4798      	blx	r3
30006a6e:	2800      	cmp	r0, #0
30006a70:	f040 809e 	bne.w	30006bb0 <BOOT_OTA_LoadIMG2+0x19c>
30006a74:	f641 75f0 	movw	r5, #8176	; 0x1ff0
30006a78:	f8df b1ac 	ldr.w	fp, [pc, #428]	; 30006c28 <BOOT_OTA_LoadIMG2+0x214>
30006a7c:	4a5a      	ldr	r2, [pc, #360]	; (30006be8 <BOOT_OTA_LoadIMG2+0x1d4>)
30006a7e:	2000      	movs	r0, #0
30006a80:	4445      	add	r5, r8
30006a82:	4659      	mov	r1, fp
30006a84:	4f59      	ldr	r7, [pc, #356]	; (30006bec <BOOT_OTA_LoadIMG2+0x1d8>)
30006a86:	f04f 0801 	mov.w	r8, #1
30006a8a:	462b      	mov	r3, r5
30006a8c:	f8df 919c 	ldr.w	r9, [pc, #412]	; 30006c2c <BOOT_OTA_LoadIMG2+0x218>
30006a90:	47b8      	blx	r7
30006a92:	2101      	movs	r1, #1
30006a94:	2000      	movs	r0, #0
30006a96:	4b56      	ldr	r3, [pc, #344]	; (30006bf0 <BOOT_OTA_LoadIMG2+0x1dc>)
30006a98:	4798      	blx	r3
30006a9a:	2301      	movs	r3, #1
30006a9c:	4952      	ldr	r1, [pc, #328]	; (30006be8 <BOOT_OTA_LoadIMG2+0x1d4>)
30006a9e:	4658      	mov	r0, fp
30006aa0:	461a      	mov	r2, r3
30006aa2:	f7ff ff6d 	bl	30006980 <BOOT_OTFCheck>
30006aa6:	47c8      	blx	r9
30006aa8:	2800      	cmp	r0, #0
30006aaa:	bf0c      	ite	eq
30006aac:	46aa      	moveq	sl, r5
30006aae:	46da      	movne	sl, fp
30006ab0:	4633      	mov	r3, r6
30006ab2:	2203      	movs	r2, #3
30006ab4:	4651      	mov	r1, sl
30006ab6:	484f      	ldr	r0, [pc, #316]	; (30006bf4 <BOOT_OTA_LoadIMG2+0x1e0>)
30006ab8:	f8cd 8000 	str.w	r8, [sp]
30006abc:	f7ff fd0e 	bl	300064dc <BOOT_LoadSubImage>
30006ac0:	2800      	cmp	r0, #0
30006ac2:	d070      	beq.n	30006ba6 <BOOT_OTA_LoadIMG2+0x192>
30006ac4:	4e4b      	ldr	r6, [pc, #300]	; (30006bf4 <BOOT_OTA_LoadIMG2+0x1e0>)
30006ac6:	47c8      	blx	r9
30006ac8:	2800      	cmp	r0, #0
30006aca:	bf08      	it	eq
30006acc:	46ab      	moveq	fp, r5
30006ace:	4f47      	ldr	r7, [pc, #284]	; (30006bec <BOOT_OTA_LoadIMG2+0x1d8>)
30006ad0:	68f2      	ldr	r2, [r6, #12]
30006ad2:	4640      	mov	r0, r8
30006ad4:	6873      	ldr	r3, [r6, #4]
30006ad6:	f8c6 b000 	str.w	fp, [r6]
30006ada:	4413      	add	r3, r2
30006adc:	6972      	ldr	r2, [r6, #20]
30006ade:	f8df b150 	ldr.w	fp, [pc, #336]	; 30006c30 <BOOT_OTA_LoadIMG2+0x21c>
30006ae2:	441a      	add	r2, r3
30006ae4:	4b44      	ldr	r3, [pc, #272]	; (30006bf8 <BOOT_OTA_LoadIMG2+0x1e4>)
30006ae6:	eb0a 0102 	add.w	r1, sl, r2
30006aea:	61b1      	str	r1, [r6, #24]
30006aec:	1e51      	subs	r1, r2, #1
30006aee:	400b      	ands	r3, r1
30006af0:	4659      	mov	r1, fp
30006af2:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006af6:	441d      	add	r5, r3
30006af8:	1a9b      	subs	r3, r3, r2
30006afa:	4a40      	ldr	r2, [pc, #256]	; (30006bfc <BOOT_OTA_LoadIMG2+0x1e8>)
30006afc:	61f3      	str	r3, [r6, #28]
30006afe:	462b      	mov	r3, r5
30006b00:	47b8      	blx	r7
30006b02:	4641      	mov	r1, r8
30006b04:	4640      	mov	r0, r8
30006b06:	4b3a      	ldr	r3, [pc, #232]	; (30006bf0 <BOOT_OTA_LoadIMG2+0x1dc>)
30006b08:	4798      	blx	r3
30006b0a:	4642      	mov	r2, r8
30006b0c:	2302      	movs	r3, #2
30006b0e:	493b      	ldr	r1, [pc, #236]	; (30006bfc <BOOT_OTA_LoadIMG2+0x1e8>)
30006b10:	4658      	mov	r0, fp
30006b12:	f7ff ff35 	bl	30006980 <BOOT_OTFCheck>
30006b16:	47c8      	blx	r9
30006b18:	2800      	cmp	r0, #0
30006b1a:	bf0c      	ite	eq
30006b1c:	462f      	moveq	r7, r5
30006b1e:	465f      	movne	r7, fp
30006b20:	ab09      	add	r3, sp, #36	; 0x24
30006b22:	2203      	movs	r2, #3
30006b24:	f106 0020 	add.w	r0, r6, #32
30006b28:	4639      	mov	r1, r7
30006b2a:	f8cd 8000 	str.w	r8, [sp]
30006b2e:	f7ff fcd5 	bl	300064dc <BOOT_LoadSubImage>
30006b32:	2800      	cmp	r0, #0
30006b34:	d037      	beq.n	30006ba6 <BOOT_OTA_LoadIMG2+0x192>
30006b36:	47c8      	blx	r9
30006b38:	6af2      	ldr	r2, [r6, #44]	; 0x2c
30006b3a:	6a73      	ldr	r3, [r6, #36]	; 0x24
30006b3c:	2800      	cmp	r0, #0
30006b3e:	bf08      	it	eq
30006b40:	46ab      	moveq	fp, r5
30006b42:	4827      	ldr	r0, [pc, #156]	; (30006be0 <BOOT_OTA_LoadIMG2+0x1cc>)
30006b44:	4413      	add	r3, r2
30006b46:	6b72      	ldr	r2, [r6, #52]	; 0x34
30006b48:	f8c6 b020 	str.w	fp, [r6, #32]
30006b4c:	4413      	add	r3, r2
30006b4e:	4a2a      	ldr	r2, [pc, #168]	; (30006bf8 <BOOT_OTA_LoadIMG2+0x1e4>)
30006b50:	1e59      	subs	r1, r3, #1
30006b52:	441f      	add	r7, r3
30006b54:	400a      	ands	r2, r1
30006b56:	2100      	movs	r1, #0
30006b58:	63b7      	str	r7, [r6, #56]	; 0x38
30006b5a:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006b5e:	9100      	str	r1, [sp, #0]
30006b60:	1ad1      	subs	r1, r2, r3
30006b62:	2900      	cmp	r1, #0
30006b64:	63f1      	str	r1, [r6, #60]	; 0x3c
30006b66:	4631      	mov	r1, r6
30006b68:	bf18      	it	ne
30006b6a:	4613      	movne	r3, r2
30006b6c:	4a24      	ldr	r2, [pc, #144]	; (30006c00 <BOOT_OTA_LoadIMG2+0x1ec>)
30006b6e:	441d      	add	r5, r3
30006b70:	4b1a      	ldr	r3, [pc, #104]	; (30006bdc <BOOT_OTA_LoadIMG2+0x1c8>)
30006b72:	6015      	str	r5, [r2, #0]
30006b74:	bf0c      	ite	eq
30006b76:	2207      	moveq	r2, #7
30006b78:	2208      	movne	r2, #8
30006b7a:	f000 fb89 	bl	30007290 <BOOT_SignatureCheck>
30006b7e:	4b21      	ldr	r3, [pc, #132]	; (30006c04 <BOOT_OTA_LoadIMG2+0x1f0>)
30006b80:	4605      	mov	r5, r0
30006b82:	689b      	ldr	r3, [r3, #8]
30006b84:	079b      	lsls	r3, r3, #30
30006b86:	d50a      	bpl.n	30006b9e <BOOT_OTA_LoadIMG2+0x18a>
30006b88:	4b1f      	ldr	r3, [pc, #124]	; (30006c08 <BOOT_OTA_LoadIMG2+0x1f4>)
30006b8a:	4a20      	ldr	r2, [pc, #128]	; (30006c0c <BOOT_OTA_LoadIMG2+0x1f8>)
30006b8c:	9903      	ldr	r1, [sp, #12]
30006b8e:	2800      	cmp	r0, #0
30006b90:	bf18      	it	ne
30006b92:	461a      	movne	r2, r3
30006b94:	481e      	ldr	r0, [pc, #120]	; (30006c10 <BOOT_OTA_LoadIMG2+0x1fc>)
30006b96:	3101      	adds	r1, #1
30006b98:	462b      	mov	r3, r5
30006b9a:	f005 fc1d 	bl	3000c3d8 <__DiagPrintf_veneer>
30006b9e:	b11d      	cbz	r5, 30006ba8 <BOOT_OTA_LoadIMG2+0x194>
30006ba0:	481c      	ldr	r0, [pc, #112]	; (30006c14 <BOOT_OTA_LoadIMG2+0x200>)
30006ba2:	f005 fc19 	bl	3000c3d8 <__DiagPrintf_veneer>
30006ba6:	24ff      	movs	r4, #255	; 0xff
30006ba8:	4620      	mov	r0, r4
30006baa:	b00d      	add	sp, #52	; 0x34
30006bac:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006bb0:	2208      	movs	r2, #8
30006bb2:	4919      	ldr	r1, [pc, #100]	; (30006c18 <BOOT_OTA_LoadIMG2+0x204>)
30006bb4:	4b19      	ldr	r3, [pc, #100]	; (30006c1c <BOOT_OTA_LoadIMG2+0x208>)
30006bb6:	a804      	add	r0, sp, #16
30006bb8:	4798      	blx	r3
30006bba:	a904      	add	r1, sp, #16
30006bbc:	2001      	movs	r0, #1
30006bbe:	4b18      	ldr	r3, [pc, #96]	; (30006c20 <BOOT_OTA_LoadIMG2+0x20c>)
30006bc0:	4798      	blx	r3
30006bc2:	e757      	b.n	30006a74 <BOOT_OTA_LoadIMG2+0x60>
30006bc4:	24ff      	movs	r4, #255	; 0xff
30006bc6:	4817      	ldr	r0, [pc, #92]	; (30006c24 <BOOT_OTA_LoadIMG2+0x210>)
30006bc8:	f005 fc06 	bl	3000c3d8 <__DiagPrintf_veneer>
30006bcc:	4620      	mov	r0, r4
30006bce:	b00d      	add	sp, #52	; 0x34
30006bd0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006bd4:	3000e3ac 	.word	0x3000e3ac
30006bd8:	3000fcf8 	.word	0x3000fcf8
30006bdc:	3000fd6c 	.word	0x3000fd6c
30006be0:	3000fed0 	.word	0x3000fed0
30006be4:	0000c129 	.word	0x0000c129
30006be8:	0cffffe0 	.word	0x0cffffe0
30006bec:	30009955 	.word	0x30009955
30006bf0:	30009995 	.word	0x30009995
30006bf4:	30010078 	.word	0x30010078
30006bf8:	fffff000 	.word	0xfffff000
30006bfc:	0dffffe0 	.word	0x0dffffe0
30006c00:	3000ee14 	.word	0x3000ee14
30006c04:	2001c00c 	.word	0x2001c00c
30006c08:	3000cfa4 	.word	0x3000cfa4
30006c0c:	3000cf9c 	.word	0x3000cf9c
30006c10:	3000cfd4 	.word	0x3000cfd4
30006c14:	3000d008 	.word	0x3000d008
30006c18:	3000fef0 	.word	0x3000fef0
30006c1c:	00012a1d 	.word	0x00012a1d
30006c20:	300098a5 	.word	0x300098a5
30006c24:	3000cfac 	.word	0x3000cfac
30006c28:	0c000000 	.word	0x0c000000
30006c2c:	30009bb5 	.word	0x30009bb5
30006c30:	0d000000 	.word	0x0d000000

30006c34 <BOOT_OTA_Region_Init>:
30006c34:	b510      	push	{r4, lr}
30006c36:	4c10      	ldr	r4, [pc, #64]	; (30006c78 <BOOT_OTA_Region_Init+0x44>)
30006c38:	2200      	movs	r2, #0
30006c3a:	2002      	movs	r0, #2
30006c3c:	4621      	mov	r1, r4
30006c3e:	f001 fd45 	bl	300086cc <flash_get_layout_info>
30006c42:	2200      	movs	r2, #0
30006c44:	1d21      	adds	r1, r4, #4
30006c46:	2003      	movs	r0, #3
30006c48:	f001 fd40 	bl	300086cc <flash_get_layout_info>
30006c4c:	2200      	movs	r2, #0
30006c4e:	f104 0110 	add.w	r1, r4, #16
30006c52:	2004      	movs	r0, #4
30006c54:	f001 fd3a 	bl	300086cc <flash_get_layout_info>
30006c58:	2200      	movs	r2, #0
30006c5a:	f104 0114 	add.w	r1, r4, #20
30006c5e:	2005      	movs	r0, #5
30006c60:	f001 fd34 	bl	300086cc <flash_get_layout_info>
30006c64:	e9d4 2300 	ldrd	r2, r3, [r4]
30006c68:	f502 5280 	add.w	r2, r2, #4096	; 0x1000
30006c6c:	f503 5380 	add.w	r3, r3, #4096	; 0x1000
30006c70:	e9c4 2302 	strd	r2, r3, [r4, #8]
30006c74:	bd10      	pop	{r4, pc}
30006c76:	bf00      	nop
30006c78:	30010060 	.word	0x30010060

30006c7c <BOOT_OTA_IMG2>:
30006c7c:	f7ff beca 	b.w	30006a14 <BOOT_OTA_LoadIMG2>

30006c80 <BOOT_OTA_AP>:
30006c80:	b5f0      	push	{r4, r5, r6, r7, lr}
30006c82:	4a32      	ldr	r2, [pc, #200]	; (30006d4c <BOOT_OTA_AP+0xcc>)
30006c84:	b0eb      	sub	sp, #428	; 0x1ac
30006c86:	4b32      	ldr	r3, [pc, #200]	; (30006d50 <BOOT_OTA_AP+0xd0>)
30006c88:	6815      	ldr	r5, [r2, #0]
30006c8a:	ac02      	add	r4, sp, #8
30006c8c:	4e31      	ldr	r6, [pc, #196]	; (30006d54 <BOOT_OTA_AP+0xd4>)
30006c8e:	f505 5780 	add.w	r7, r5, #4096	; 0x1000
30006c92:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006c94:	e884 000f 	stmia.w	r4, {r0, r1, r2, r3}
30006c98:	4629      	mov	r1, r5
30006c9a:	4d2f      	ldr	r5, [pc, #188]	; (30006d58 <BOOT_OTA_AP+0xd8>)
30006c9c:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006ca0:	a806      	add	r0, sp, #24
30006ca2:	f7fd fe51 	bl	30004948 <BOOT_ImgCopy>
30006ca6:	4a2d      	ldr	r2, [pc, #180]	; (30006d5c <BOOT_OTA_AP+0xdc>)
30006ca8:	463b      	mov	r3, r7
30006caa:	4629      	mov	r1, r5
30006cac:	2003      	movs	r0, #3
30006cae:	47b0      	blx	r6
30006cb0:	2101      	movs	r1, #1
30006cb2:	4b2b      	ldr	r3, [pc, #172]	; (30006d60 <BOOT_OTA_AP+0xe0>)
30006cb4:	2003      	movs	r0, #3
30006cb6:	4798      	blx	r3
30006cb8:	2201      	movs	r2, #1
30006cba:	4928      	ldr	r1, [pc, #160]	; (30006d5c <BOOT_OTA_AP+0xdc>)
30006cbc:	2303      	movs	r3, #3
30006cbe:	4628      	mov	r0, r5
30006cc0:	f7ff fe5e 	bl	30006980 <BOOT_OTFCheck>
30006cc4:	4b27      	ldr	r3, [pc, #156]	; (30006d64 <BOOT_OTA_AP+0xe4>)
30006cc6:	4798      	blx	r3
30006cc8:	4623      	mov	r3, r4
30006cca:	2201      	movs	r2, #1
30006ccc:	4c26      	ldr	r4, [pc, #152]	; (30006d68 <BOOT_OTA_AP+0xe8>)
30006cce:	2800      	cmp	r0, #0
30006cd0:	bf14      	ite	ne
30006cd2:	4629      	movne	r1, r5
30006cd4:	4639      	moveq	r1, r7
30006cd6:	9200      	str	r2, [sp, #0]
30006cd8:	4620      	mov	r0, r4
30006cda:	2204      	movs	r2, #4
30006cdc:	f7ff fbfe 	bl	300064dc <BOOT_LoadSubImage>
30006ce0:	2801      	cmp	r0, #1
30006ce2:	d122      	bne.n	30006d2a <BOOT_OTA_AP+0xaa>
30006ce4:	2302      	movs	r3, #2
30006ce6:	4605      	mov	r5, r0
30006ce8:	2204      	movs	r2, #4
30006cea:	a806      	add	r0, sp, #24
30006cec:	9300      	str	r3, [sp, #0]
30006cee:	4621      	mov	r1, r4
30006cf0:	4b1e      	ldr	r3, [pc, #120]	; (30006d6c <BOOT_OTA_AP+0xec>)
30006cf2:	f000 facd 	bl	30007290 <BOOT_SignatureCheck>
30006cf6:	4606      	mov	r6, r0
30006cf8:	bb18      	cbnz	r0, 30006d42 <BOOT_OTA_AP+0xc2>
30006cfa:	68e3      	ldr	r3, [r4, #12]
30006cfc:	462a      	mov	r2, r5
30006cfe:	6861      	ldr	r1, [r4, #4]
30006d00:	f104 0018 	add.w	r0, r4, #24
30006d04:	4419      	add	r1, r3
30006d06:	6963      	ldr	r3, [r4, #20]
30006d08:	4419      	add	r1, r3
30006d0a:	4439      	add	r1, r7
30006d0c:	f7ff fc9a 	bl	30006644 <BOOT_LoadAPSubImage>
30006d10:	2801      	cmp	r0, #1
30006d12:	d102      	bne.n	30006d1a <BOOT_OTA_AP+0x9a>
30006d14:	4630      	mov	r0, r6
30006d16:	b06b      	add	sp, #428	; 0x1ac
30006d18:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d1a:	4b15      	ldr	r3, [pc, #84]	; (30006d70 <BOOT_OTA_AP+0xf0>)
30006d1c:	681b      	ldr	r3, [r3, #0]
30006d1e:	079b      	lsls	r3, r3, #30
30006d20:	d40a      	bmi.n	30006d38 <BOOT_OTA_AP+0xb8>
30006d22:	26ff      	movs	r6, #255	; 0xff
30006d24:	4630      	mov	r0, r6
30006d26:	b06b      	add	sp, #428	; 0x1ac
30006d28:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d2a:	26ff      	movs	r6, #255	; 0xff
30006d2c:	4811      	ldr	r0, [pc, #68]	; (30006d74 <BOOT_OTA_AP+0xf4>)
30006d2e:	f005 fb53 	bl	3000c3d8 <__DiagPrintf_veneer>
30006d32:	4630      	mov	r0, r6
30006d34:	b06b      	add	sp, #428	; 0x1ac
30006d36:	bdf0      	pop	{r4, r5, r6, r7, pc}
30006d38:	480f      	ldr	r0, [pc, #60]	; (30006d78 <BOOT_OTA_AP+0xf8>)
30006d3a:	26ff      	movs	r6, #255	; 0xff
30006d3c:	f005 fb4c 	bl	3000c3d8 <__DiagPrintf_veneer>
30006d40:	e7e8      	b.n	30006d14 <BOOT_OTA_AP+0x94>
30006d42:	480e      	ldr	r0, [pc, #56]	; (30006d7c <BOOT_OTA_AP+0xfc>)
30006d44:	26ff      	movs	r6, #255	; 0xff
30006d46:	f005 fb47 	bl	3000c3d8 <__DiagPrintf_veneer>
30006d4a:	e7e3      	b.n	30006d14 <BOOT_OTA_AP+0x94>
30006d4c:	3000ee14 	.word	0x3000ee14
30006d50:	3000e3c4 	.word	0x3000e3c4
30006d54:	30009955 	.word	0x30009955
30006d58:	0e000000 	.word	0x0e000000
30006d5c:	0fffffe0 	.word	0x0fffffe0
30006d60:	30009995 	.word	0x30009995
30006d64:	30009bb5 	.word	0x30009bb5
30006d68:	30010078 	.word	0x30010078
30006d6c:	3000fd6c 	.word	0x3000fd6c
30006d70:	2001c00c 	.word	0x2001c00c
30006d74:	3000d050 	.word	0x3000d050
30006d78:	3000d074 	.word	0x3000d074
30006d7c:	3000d02c 	.word	0x3000d02c

30006d80 <BOOT_LoadRDPImg>:
30006d80:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006d84:	4d4e      	ldr	r5, [pc, #312]	; (30006ec0 <BOOT_LoadRDPImg+0x140>)
30006d86:	b0d9      	sub	sp, #356	; 0x164
30006d88:	4613      	mov	r3, r2
30006d8a:	468b      	mov	fp, r1
30006d8c:	4604      	mov	r4, r0
30006d8e:	aa06      	add	r2, sp, #24
30006d90:	f503 5880 	add.w	r8, r3, #4096	; 0x1000
30006d94:	e895 0003 	ldmia.w	r5, {r0, r1}
30006d98:	2500      	movs	r5, #0
30006d9a:	e882 0003 	stmia.w	r2, {r0, r1}
30006d9e:	4619      	mov	r1, r3
30006da0:	f44f 72c8 	mov.w	r2, #400	; 0x190
30006da4:	4620      	mov	r0, r4
30006da6:	f7fd fdcf 	bl	30004948 <BOOT_ImgCopy>
30006daa:	ab06      	add	r3, sp, #24
30006dac:	2202      	movs	r2, #2
30006dae:	4641      	mov	r1, r8
30006db0:	4658      	mov	r0, fp
30006db2:	9500      	str	r5, [sp, #0]
30006db4:	f7ff fb92 	bl	300064dc <BOOT_LoadSubImage>
30006db8:	2801      	cmp	r0, #1
30006dba:	d06f      	beq.n	30006e9c <BOOT_LoadRDPImg+0x11c>
30006dbc:	4b41      	ldr	r3, [pc, #260]	; (30006ec4 <BOOT_LoadRDPImg+0x144>)
30006dbe:	4628      	mov	r0, r5
30006dc0:	4798      	blx	r3
30006dc2:	2101      	movs	r1, #1
30006dc4:	4b40      	ldr	r3, [pc, #256]	; (30006ec8 <BOOT_LoadRDPImg+0x148>)
30006dc6:	4628      	mov	r0, r5
30006dc8:	4798      	blx	r3
30006dca:	4b40      	ldr	r3, [pc, #256]	; (30006ecc <BOOT_LoadRDPImg+0x14c>)
30006dcc:	2120      	movs	r1, #32
30006dce:	a810      	add	r0, sp, #64	; 0x40
30006dd0:	4798      	blx	r3
30006dd2:	f8df a110 	ldr.w	sl, [pc, #272]	; 30006ee4 <BOOT_LoadRDPImg+0x164>
30006dd6:	f104 0120 	add.w	r1, r4, #32
30006dda:	2210      	movs	r2, #16
30006ddc:	a80c      	add	r0, sp, #48	; 0x30
30006dde:	af08      	add	r7, sp, #32
30006de0:	47d0      	blx	sl
30006de2:	f8df 9104 	ldr.w	r9, [pc, #260]	; 30006ee8 <BOOT_LoadRDPImg+0x168>
30006de6:	ab06      	add	r3, sp, #24
30006de8:	9504      	str	r5, [sp, #16]
30006dea:	9305      	str	r3, [sp, #20]
30006dec:	4641      	mov	r1, r8
30006dee:	a818      	add	r0, sp, #96	; 0x60
30006df0:	2210      	movs	r2, #16
30006df2:	f7fd fda9 	bl	30004948 <BOOT_ImgCopy>
30006df6:	2310      	movs	r3, #16
30006df8:	aa0c      	add	r2, sp, #48	; 0x30
30006dfa:	a818      	add	r0, sp, #96	; 0x60
30006dfc:	4619      	mov	r1, r3
30006dfe:	9700      	str	r7, [sp, #0]
30006e00:	47c8      	blx	r9
30006e02:	683b      	ldr	r3, [r7, #0]
30006e04:	4a32      	ldr	r2, [pc, #200]	; (30006ed0 <BOOT_LoadRDPImg+0x150>)
30006e06:	4293      	cmp	r3, r2
30006e08:	d14c      	bne.n	30006ea4 <BOOT_LoadRDPImg+0x124>
30006e0a:	9a05      	ldr	r2, [sp, #20]
30006e0c:	687b      	ldr	r3, [r7, #4]
30006e0e:	f852 1b04 	ldr.w	r1, [r2], #4
30006e12:	9205      	str	r2, [sp, #20]
30006e14:	4a2f      	ldr	r2, [pc, #188]	; (30006ed4 <BOOT_LoadRDPImg+0x154>)
30006e16:	4293      	cmp	r3, r2
30006e18:	d148      	bne.n	30006eac <BOOT_LoadRDPImg+0x12c>
30006e1a:	9100      	str	r1, [sp, #0]
30006e1c:	2004      	movs	r0, #4
30006e1e:	68fe      	ldr	r6, [r7, #12]
30006e20:	4b2d      	ldr	r3, [pc, #180]	; (30006ed8 <BOOT_LoadRDPImg+0x158>)
30006e22:	3e20      	subs	r6, #32
30006e24:	4a2d      	ldr	r2, [pc, #180]	; (30006edc <BOOT_LoadRDPImg+0x15c>)
30006e26:	4619      	mov	r1, r3
30006e28:	9601      	str	r6, [sp, #4]
30006e2a:	68bd      	ldr	r5, [r7, #8]
30006e2c:	3520      	adds	r5, #32
30006e2e:	9502      	str	r5, [sp, #8]
30006e30:	f005 f85c 	bl	3000beec <rtk_log_write>
30006e34:	f1bb 0f00 	cmp.w	fp, #0
30006e38:	d006      	beq.n	30006e48 <BOOT_LoadRDPImg+0xc8>
30006e3a:	9a04      	ldr	r2, [sp, #16]
30006e3c:	f10b 0304 	add.w	r3, fp, #4
30006e40:	f84b 6032 	str.w	r6, [fp, r2, lsl #3]
30006e44:	f843 5032 	str.w	r5, [r3, r2, lsl #3]
30006e48:	b1dd      	cbz	r5, 30006e82 <BOOT_LoadRDPImg+0x102>
30006e4a:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006e4e:	462c      	mov	r4, r5
30006e50:	4641      	mov	r1, r8
30006e52:	a818      	add	r0, sp, #96	; 0x60
30006e54:	bf28      	it	cs
30006e56:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006e5a:	4622      	mov	r2, r4
30006e5c:	44a0      	add	r8, r4
30006e5e:	f7fd fd73 	bl	30004948 <BOOT_ImgCopy>
30006e62:	aa0c      	add	r2, sp, #48	; 0x30
30006e64:	4621      	mov	r1, r4
30006e66:	2310      	movs	r3, #16
30006e68:	9600      	str	r6, [sp, #0]
30006e6a:	a818      	add	r0, sp, #96	; 0x60
30006e6c:	47c8      	blx	r9
30006e6e:	ab18      	add	r3, sp, #96	; 0x60
30006e70:	f1a4 0110 	sub.w	r1, r4, #16
30006e74:	2210      	movs	r2, #16
30006e76:	a80c      	add	r0, sp, #48	; 0x30
30006e78:	4426      	add	r6, r4
30006e7a:	4419      	add	r1, r3
30006e7c:	47d0      	blx	sl
30006e7e:	1b2d      	subs	r5, r5, r4
30006e80:	d1e3      	bne.n	30006e4a <BOOT_LoadRDPImg+0xca>
30006e82:	9b04      	ldr	r3, [sp, #16]
30006e84:	b13b      	cbz	r3, 30006e96 <BOOT_LoadRDPImg+0x116>
30006e86:	2100      	movs	r1, #0
30006e88:	4b0f      	ldr	r3, [pc, #60]	; (30006ec8 <BOOT_LoadRDPImg+0x148>)
30006e8a:	4608      	mov	r0, r1
30006e8c:	4798      	blx	r3
30006e8e:	2002      	movs	r0, #2
30006e90:	b059      	add	sp, #356	; 0x164
30006e92:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006e96:	2301      	movs	r3, #1
30006e98:	9304      	str	r3, [sp, #16]
30006e9a:	e7a7      	b.n	30006dec <BOOT_LoadRDPImg+0x6c>
30006e9c:	2002      	movs	r0, #2
30006e9e:	b059      	add	sp, #356	; 0x164
30006ea0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006ea4:	9b04      	ldr	r3, [sp, #16]
30006ea6:	aa06      	add	r2, sp, #24
30006ea8:	f852 1023 	ldr.w	r1, [r2, r3, lsl #2]
30006eac:	4b0a      	ldr	r3, [pc, #40]	; (30006ed8 <BOOT_LoadRDPImg+0x158>)
30006eae:	2004      	movs	r0, #4
30006eb0:	9100      	str	r1, [sp, #0]
30006eb2:	4a0b      	ldr	r2, [pc, #44]	; (30006ee0 <BOOT_LoadRDPImg+0x160>)
30006eb4:	4619      	mov	r1, r3
30006eb6:	f005 f819 	bl	3000beec <rtk_log_write>
30006eba:	2000      	movs	r0, #0
30006ebc:	e7e8      	b.n	30006e90 <BOOT_LoadRDPImg+0x110>
30006ebe:	bf00      	nop
30006ec0:	3000e3d4 	.word	0x3000e3d4
30006ec4:	00002429 	.word	0x00002429
30006ec8:	00002479 	.word	0x00002479
30006ecc:	00004641 	.word	0x00004641
30006ed0:	35393138 	.word	0x35393138
30006ed4:	31313738 	.word	0x31313738
30006ed8:	3000c6b0 	.word	0x3000c6b0
30006edc:	3000cf1c 	.word	0x3000cf1c
30006ee0:	3000cf08 	.word	0x3000cf08
30006ee4:	00012a1d 	.word	0x00012a1d
30006ee8:	000046f5 	.word	0x000046f5

30006eec <BOOT_LoadBL32RDPImg>:
30006eec:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ef0:	4b35      	ldr	r3, [pc, #212]	; (30006fc8 <BOOT_LoadBL32RDPImg+0xdc>)
30006ef2:	b0d1      	sub	sp, #324	; 0x144
30006ef4:	f8df c0ec 	ldr.w	ip, [pc, #236]	; 30006fe4 <BOOT_LoadBL32RDPImg+0xf8>
30006ef8:	4607      	mov	r7, r0
30006efa:	f10d 0810 	add.w	r8, sp, #16
30006efe:	460e      	mov	r6, r1
30006f00:	4615      	mov	r5, r2
30006f02:	ac02      	add	r4, sp, #8
30006f04:	f8df 90e0 	ldr.w	r9, [pc, #224]	; 30006fe8 <BOOT_LoadBL32RDPImg+0xfc>
30006f08:	f8df a0e0 	ldr.w	sl, [pc, #224]	; 30006fec <BOOT_LoadBL32RDPImg+0x100>
30006f0c:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
30006f0e:	e888 000f 	stmia.w	r8, {r0, r1, r2, r3}
30006f12:	e89c 0003 	ldmia.w	ip, {r0, r1}
30006f16:	e884 0003 	stmia.w	r4, {r0, r1}
30006f1a:	2000      	movs	r0, #0
30006f1c:	47c8      	blx	r9
30006f1e:	2101      	movs	r1, #1
30006f20:	2000      	movs	r0, #0
30006f22:	47d0      	blx	sl
30006f24:	2120      	movs	r1, #32
30006f26:	4b29      	ldr	r3, [pc, #164]	; (30006fcc <BOOT_LoadBL32RDPImg+0xe0>)
30006f28:	eb0d 0001 	add.w	r0, sp, r1
30006f2c:	4798      	blx	r3
30006f2e:	4b28      	ldr	r3, [pc, #160]	; (30006fd0 <BOOT_LoadBL32RDPImg+0xe4>)
30006f30:	689b      	ldr	r3, [r3, #8]
30006f32:	079b      	lsls	r3, r3, #30
30006f34:	d436      	bmi.n	30006fa4 <BOOT_LoadBL32RDPImg+0xb8>
30006f36:	b1fd      	cbz	r5, 30006f78 <BOOT_LoadBL32RDPImg+0x8c>
30006f38:	f8df b0b4 	ldr.w	fp, [pc, #180]	; 30006ff0 <BOOT_LoadBL32RDPImg+0x104>
30006f3c:	f8df 90b4 	ldr.w	r9, [pc, #180]	; 30006ff4 <BOOT_LoadBL32RDPImg+0x108>
30006f40:	f5b5 7f80 	cmp.w	r5, #256	; 0x100
30006f44:	462c      	mov	r4, r5
30006f46:	4639      	mov	r1, r7
30006f48:	a810      	add	r0, sp, #64	; 0x40
30006f4a:	bf28      	it	cs
30006f4c:	f44f 7480 	movcs.w	r4, #256	; 0x100
30006f50:	4622      	mov	r2, r4
30006f52:	4427      	add	r7, r4
30006f54:	f7fd fcf8 	bl	30004948 <BOOT_ImgCopy>
30006f58:	4642      	mov	r2, r8
30006f5a:	4621      	mov	r1, r4
30006f5c:	2310      	movs	r3, #16
30006f5e:	9600      	str	r6, [sp, #0]
30006f60:	a810      	add	r0, sp, #64	; 0x40
30006f62:	47d8      	blx	fp
30006f64:	ab10      	add	r3, sp, #64	; 0x40
30006f66:	f1a4 0110 	sub.w	r1, r4, #16
30006f6a:	2210      	movs	r2, #16
30006f6c:	4640      	mov	r0, r8
30006f6e:	4426      	add	r6, r4
30006f70:	4419      	add	r1, r3
30006f72:	47c8      	blx	r9
30006f74:	1b2d      	subs	r5, r5, r4
30006f76:	d1e3      	bne.n	30006f40 <BOOT_LoadBL32RDPImg+0x54>
30006f78:	f856 2c20 	ldr.w	r2, [r6, #-32]
30006f7c:	3e20      	subs	r6, #32
30006f7e:	4b15      	ldr	r3, [pc, #84]	; (30006fd4 <BOOT_LoadBL32RDPImg+0xe8>)
30006f80:	429a      	cmp	r2, r3
30006f82:	d108      	bne.n	30006f96 <BOOT_LoadBL32RDPImg+0xaa>
30006f84:	f103 437b 	add.w	r3, r3, #4211081216	; 0xfb000000
30006f88:	6872      	ldr	r2, [r6, #4]
30006f8a:	f503 0378 	add.w	r3, r3, #16252928	; 0xf80000
30006f8e:	f503 63c0 	add.w	r3, r3, #1536	; 0x600
30006f92:	429a      	cmp	r2, r3
30006f94:	d00d      	beq.n	30006fb2 <BOOT_LoadBL32RDPImg+0xc6>
30006f96:	4810      	ldr	r0, [pc, #64]	; (30006fd8 <BOOT_LoadBL32RDPImg+0xec>)
30006f98:	f005 fa1e 	bl	3000c3d8 <__DiagPrintf_veneer>
30006f9c:	2000      	movs	r0, #0
30006f9e:	b051      	add	sp, #324	; 0x144
30006fa0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006fa4:	4621      	mov	r1, r4
30006fa6:	462b      	mov	r3, r5
30006fa8:	4632      	mov	r2, r6
30006faa:	480c      	ldr	r0, [pc, #48]	; (30006fdc <BOOT_LoadBL32RDPImg+0xf0>)
30006fac:	f005 fa14 	bl	3000c3d8 <__DiagPrintf_veneer>
30006fb0:	e7c1      	b.n	30006f36 <BOOT_LoadBL32RDPImg+0x4a>
30006fb2:	480b      	ldr	r0, [pc, #44]	; (30006fe0 <BOOT_LoadBL32RDPImg+0xf4>)
30006fb4:	f005 fa10 	bl	3000c3d8 <__DiagPrintf_veneer>
30006fb8:	2100      	movs	r1, #0
30006fba:	4608      	mov	r0, r1
30006fbc:	47d0      	blx	sl
30006fbe:	2001      	movs	r0, #1
30006fc0:	b051      	add	sp, #324	; 0x144
30006fc2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30006fc6:	bf00      	nop
30006fc8:	3000d11c 	.word	0x3000d11c
30006fcc:	00004641 	.word	0x00004641
30006fd0:	2001c00c 	.word	0x2001c00c
30006fd4:	35393138 	.word	0x35393138
30006fd8:	3000d100 	.word	0x3000d100
30006fdc:	3000d0d8 	.word	0x3000d0d8
30006fe0:	3000d10c 	.word	0x3000d10c
30006fe4:	3000d130 	.word	0x3000d130
30006fe8:	00002429 	.word	0x00002429
30006fec:	00002479 	.word	0x00002479
30006ff0:	000046f5 	.word	0x000046f5
30006ff4:	00012a1d 	.word	0x00012a1d

30006ff8 <_BOOT_Validate_ImgHash>:
30006ff8:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30006ffc:	f5ad 7d51 	sub.w	sp, sp, #836	; 0x344
30007000:	4605      	mov	r5, r0
30007002:	4690      	mov	r8, r2
30007004:	22e0      	movs	r2, #224	; 0xe0
30007006:	9103      	str	r1, [sp, #12]
30007008:	2100      	movs	r1, #0
3000700a:	a818      	add	r0, sp, #96	; 0x60
3000700c:	461c      	mov	r4, r3
3000700e:	f88d 101f 	strb.w	r1, [sp, #31]
30007012:	f005 f9b1 	bl	3000c378 <____wrap_memset_veneer>
30007016:	4628      	mov	r0, r5
30007018:	f10d 021f 	add.w	r2, sp, #31
3000701c:	f10d 011e 	add.w	r1, sp, #30
30007020:	f002 fdfe 	bl	30009c20 <SBOOT_GetMdType>
30007024:	9001      	str	r0, [sp, #4]
30007026:	2800      	cmp	r0, #0
30007028:	d166      	bne.n	300070f8 <_BOOT_Validate_ImgHash+0x100>
3000702a:	f89d 301f 	ldrb.w	r3, [sp, #31]
3000702e:	2b00      	cmp	r3, #0
30007030:	d067      	beq.n	30007102 <_BOOT_Validate_ImgHash+0x10a>
30007032:	2101      	movs	r1, #1
30007034:	4605      	mov	r5, r0
30007036:	4b80      	ldr	r3, [pc, #512]	; (30007238 <_BOOT_Validate_ImgHash+0x240>)
30007038:	4608      	mov	r0, r1
3000703a:	4798      	blx	r3
3000703c:	4629      	mov	r1, r5
3000703e:	ab18      	add	r3, sp, #96	; 0x60
30007040:	2220      	movs	r2, #32
30007042:	f89d 001e 	ldrb.w	r0, [sp, #30]
30007046:	4d7d      	ldr	r5, [pc, #500]	; (3000723c <_BOOT_Validate_ImgHash+0x244>)
30007048:	47a8      	blx	r5
3000704a:	2800      	cmp	r0, #0
3000704c:	f040 808b 	bne.w	30007166 <_BOOT_Validate_ImgHash+0x16e>
30007050:	b32c      	cbz	r4, 3000709e <_BOOT_Validate_ImgHash+0xa6>
30007052:	aad0      	add	r2, sp, #832	; 0x340
30007054:	1e63      	subs	r3, r4, #1
30007056:	ae50      	add	r6, sp, #320	; 0x140
30007058:	f8df a1f8 	ldr.w	sl, [pc, #504]	; 30007254 <_BOOT_Validate_ImgHash+0x25c>
3000705c:	f012 0218 	ands.w	r2, r2, #24
30007060:	b2db      	uxtb	r3, r3
30007062:	f026 091f 	bic.w	r9, r6, #31
30007066:	4d76      	ldr	r5, [pc, #472]	; (30007240 <_BOOT_Validate_ImgHash+0x248>)
30007068:	9204      	str	r2, [sp, #16]
3000706a:	f108 0208 	add.w	r2, r8, #8
3000706e:	bf08      	it	eq
30007070:	46b1      	moveq	r9, r6
30007072:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30007076:	9302      	str	r3, [sp, #8]
30007078:	e9d8 b700 	ldrd	fp, r7, [r8]
3000707c:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
30007080:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30007084:	f0c0 80a7 	bcc.w	300071d6 <_BOOT_Validate_ImgHash+0x1de>
30007088:	4639      	mov	r1, r7
3000708a:	4658      	mov	r0, fp
3000708c:	aa18      	add	r2, sp, #96	; 0x60
3000708e:	47d0      	blx	sl
30007090:	2800      	cmp	r0, #0
30007092:	d168      	bne.n	30007166 <_BOOT_Validate_ImgHash+0x16e>
30007094:	f108 0808 	add.w	r8, r8, #8
30007098:	9b02      	ldr	r3, [sp, #8]
3000709a:	4543      	cmp	r3, r8
3000709c:	d1ec      	bne.n	30007078 <_BOOT_Validate_ImgHash+0x80>
3000709e:	ac08      	add	r4, sp, #32
300070a0:	a918      	add	r1, sp, #96	; 0x60
300070a2:	4b68      	ldr	r3, [pc, #416]	; (30007244 <_BOOT_Validate_ImgHash+0x24c>)
300070a4:	4620      	mov	r0, r4
300070a6:	4798      	blx	r3
300070a8:	4601      	mov	r1, r0
300070aa:	2800      	cmp	r0, #0
300070ac:	d15b      	bne.n	30007166 <_BOOT_Validate_ImgHash+0x16e>
300070ae:	2001      	movs	r0, #1
300070b0:	4b61      	ldr	r3, [pc, #388]	; (30007238 <_BOOT_Validate_ImgHash+0x240>)
300070b2:	4798      	blx	r3
300070b4:	f89d 501e 	ldrb.w	r5, [sp, #30]
300070b8:	4b63      	ldr	r3, [pc, #396]	; (30007248 <_BOOT_Validate_ImgHash+0x250>)
300070ba:	4798      	blx	r3
300070bc:	fbb0 f3f5 	udiv	r3, r0, r5
300070c0:	fb03 0315 	mls	r3, r3, r5, r0
300070c4:	b2db      	uxtb	r3, r3
300070c6:	b1bd      	cbz	r5, 300070f8 <_BOOT_Validate_ImgHash+0x100>
300070c8:	2000      	movs	r0, #0
300070ca:	9f03      	ldr	r7, [sp, #12]
300070cc:	4606      	mov	r6, r0
300070ce:	1c5a      	adds	r2, r3, #1
300070d0:	5cf9      	ldrb	r1, [r7, r3]
300070d2:	5ce3      	ldrb	r3, [r4, r3]
300070d4:	3001      	adds	r0, #1
300070d6:	b2d2      	uxtb	r2, r2
300070d8:	404b      	eors	r3, r1
300070da:	431e      	orrs	r6, r3
300070dc:	fbb2 f3f5 	udiv	r3, r2, r5
300070e0:	fb05 2213 	mls	r2, r5, r3, r2
300070e4:	b2d3      	uxtb	r3, r2
300070e6:	b2c2      	uxtb	r2, r0
300070e8:	4295      	cmp	r5, r2
300070ea:	d8f0      	bhi.n	300070ce <_BOOT_Validate_ImgHash+0xd6>
300070ec:	2e00      	cmp	r6, #0
300070ee:	9b01      	ldr	r3, [sp, #4]
300070f0:	bf18      	it	ne
300070f2:	f06f 0306 	mvnne.w	r3, #6
300070f6:	9301      	str	r3, [sp, #4]
300070f8:	9801      	ldr	r0, [sp, #4]
300070fa:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
300070fe:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007102:	a918      	add	r1, sp, #96	; 0x60
30007104:	f89d 001e 	ldrb.w	r0, [sp, #30]
30007108:	4b50      	ldr	r3, [pc, #320]	; (3000724c <_BOOT_Validate_ImgHash+0x254>)
3000710a:	4798      	blx	r3
3000710c:	bb58      	cbnz	r0, 30007166 <_BOOT_Validate_ImgHash+0x16e>
3000710e:	b31c      	cbz	r4, 30007158 <_BOOT_Validate_ImgHash+0x160>
30007110:	aad0      	add	r2, sp, #832	; 0x340
30007112:	1e63      	subs	r3, r4, #1
30007114:	ae50      	add	r6, sp, #320	; 0x140
30007116:	f8df a140 	ldr.w	sl, [pc, #320]	; 30007258 <_BOOT_Validate_ImgHash+0x260>
3000711a:	f012 0218 	ands.w	r2, r2, #24
3000711e:	b2db      	uxtb	r3, r3
30007120:	f026 091f 	bic.w	r9, r6, #31
30007124:	4d46      	ldr	r5, [pc, #280]	; (30007240 <_BOOT_Validate_ImgHash+0x248>)
30007126:	9204      	str	r2, [sp, #16]
30007128:	f108 0208 	add.w	r2, r8, #8
3000712c:	bf08      	it	eq
3000712e:	46b1      	moveq	r9, r6
30007130:	eb02 03c3 	add.w	r3, r2, r3, lsl #3
30007134:	9302      	str	r3, [sp, #8]
30007136:	e9d8 b700 	ldrd	fp, r7, [r8]
3000713a:	f10b 4278 	add.w	r2, fp, #4160749568	; 0xf8000000
3000713e:	f1b2 6f00 	cmp.w	r2, #134217728	; 0x8000000
30007142:	d318      	bcc.n	30007176 <_BOOT_Validate_ImgHash+0x17e>
30007144:	4639      	mov	r1, r7
30007146:	4658      	mov	r0, fp
30007148:	aa18      	add	r2, sp, #96	; 0x60
3000714a:	47d0      	blx	sl
3000714c:	b958      	cbnz	r0, 30007166 <_BOOT_Validate_ImgHash+0x16e>
3000714e:	f108 0808 	add.w	r8, r8, #8
30007152:	9b02      	ldr	r3, [sp, #8]
30007154:	4543      	cmp	r3, r8
30007156:	d1ee      	bne.n	30007136 <_BOOT_Validate_ImgHash+0x13e>
30007158:	ac08      	add	r4, sp, #32
3000715a:	a918      	add	r1, sp, #96	; 0x60
3000715c:	4b3c      	ldr	r3, [pc, #240]	; (30007250 <_BOOT_Validate_ImgHash+0x258>)
3000715e:	4620      	mov	r0, r4
30007160:	4798      	blx	r3
30007162:	2800      	cmp	r0, #0
30007164:	d0a6      	beq.n	300070b4 <_BOOT_Validate_ImgHash+0xbc>
30007166:	f06f 030a 	mvn.w	r3, #10
3000716a:	9301      	str	r3, [sp, #4]
3000716c:	9801      	ldr	r0, [sp, #4]
3000716e:	f50d 7d51 	add.w	sp, sp, #836	; 0x344
30007172:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
30007176:	2f00      	cmp	r7, #0
30007178:	d0e9      	beq.n	3000714e <_BOOT_Validate_ImgHash+0x156>
3000717a:	9c04      	ldr	r4, [sp, #16]
3000717c:	f8cd 8014 	str.w	r8, [sp, #20]
30007180:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
30007184:	46b8      	mov	r8, r7
30007186:	4659      	mov	r1, fp
30007188:	4630      	mov	r0, r6
3000718a:	bf28      	it	cs
3000718c:	f44f 7800 	movcs.w	r8, #512	; 0x200
30007190:	4642      	mov	r2, r8
30007192:	f7fd fbd9 	bl	30004948 <BOOT_ImgCopy>
30007196:	696a      	ldr	r2, [r5, #20]
30007198:	03d3      	lsls	r3, r2, #15
3000719a:	d50f      	bpl.n	300071bc <_BOOT_Validate_ImgHash+0x1c4>
3000719c:	eb08 0004 	add.w	r0, r8, r4
300071a0:	f3bf 8f4f 	dsb	sy
300071a4:	4448      	add	r0, r9
300071a6:	464a      	mov	r2, r9
300071a8:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
300071ac:	3220      	adds	r2, #32
300071ae:	1a83      	subs	r3, r0, r2
300071b0:	2b00      	cmp	r3, #0
300071b2:	dcf9      	bgt.n	300071a8 <_BOOT_Validate_ImgHash+0x1b0>
300071b4:	f3bf 8f4f 	dsb	sy
300071b8:	f3bf 8f6f 	isb	sy
300071bc:	aa18      	add	r2, sp, #96	; 0x60
300071be:	4641      	mov	r1, r8
300071c0:	4630      	mov	r0, r6
300071c2:	47d0      	blx	sl
300071c4:	2800      	cmp	r0, #0
300071c6:	d1ce      	bne.n	30007166 <_BOOT_Validate_ImgHash+0x16e>
300071c8:	ebb7 0708 	subs.w	r7, r7, r8
300071cc:	44c3      	add	fp, r8
300071ce:	d1d7      	bne.n	30007180 <_BOOT_Validate_ImgHash+0x188>
300071d0:	f8dd 8014 	ldr.w	r8, [sp, #20]
300071d4:	e7bb      	b.n	3000714e <_BOOT_Validate_ImgHash+0x156>
300071d6:	2f00      	cmp	r7, #0
300071d8:	f43f af5c 	beq.w	30007094 <_BOOT_Validate_ImgHash+0x9c>
300071dc:	9c04      	ldr	r4, [sp, #16]
300071de:	f8cd 8014 	str.w	r8, [sp, #20]
300071e2:	f5b7 7f00 	cmp.w	r7, #512	; 0x200
300071e6:	46b8      	mov	r8, r7
300071e8:	4659      	mov	r1, fp
300071ea:	4630      	mov	r0, r6
300071ec:	bf28      	it	cs
300071ee:	f44f 7800 	movcs.w	r8, #512	; 0x200
300071f2:	4642      	mov	r2, r8
300071f4:	f7fd fba8 	bl	30004948 <BOOT_ImgCopy>
300071f8:	696a      	ldr	r2, [r5, #20]
300071fa:	03d2      	lsls	r2, r2, #15
300071fc:	d50f      	bpl.n	3000721e <_BOOT_Validate_ImgHash+0x226>
300071fe:	eb08 0004 	add.w	r0, r8, r4
30007202:	f3bf 8f4f 	dsb	sy
30007206:	4448      	add	r0, r9
30007208:	464a      	mov	r2, r9
3000720a:	f8c5 2270 	str.w	r2, [r5, #624]	; 0x270
3000720e:	3220      	adds	r2, #32
30007210:	1a83      	subs	r3, r0, r2
30007212:	2b00      	cmp	r3, #0
30007214:	dcf9      	bgt.n	3000720a <_BOOT_Validate_ImgHash+0x212>
30007216:	f3bf 8f4f 	dsb	sy
3000721a:	f3bf 8f6f 	isb	sy
3000721e:	aa18      	add	r2, sp, #96	; 0x60
30007220:	4641      	mov	r1, r8
30007222:	4630      	mov	r0, r6
30007224:	47d0      	blx	sl
30007226:	2800      	cmp	r0, #0
30007228:	d19d      	bne.n	30007166 <_BOOT_Validate_ImgHash+0x16e>
3000722a:	ebb7 0708 	subs.w	r7, r7, r8
3000722e:	44c3      	add	fp, r8
30007230:	d1d7      	bne.n	300071e2 <_BOOT_Validate_ImgHash+0x1ea>
30007232:	f8dd 8014 	ldr.w	r8, [sp, #20]
30007236:	e72d      	b.n	30007094 <_BOOT_Validate_ImgHash+0x9c>
30007238:	30009c5d 	.word	0x30009c5d
3000723c:	3000951d 	.word	0x3000951d
30007240:	e000ed00 	.word	0xe000ed00
30007244:	30009cd9 	.word	0x30009cd9
30007248:	0000ea4d 	.word	0x0000ea4d
3000724c:	30009d19 	.word	0x30009d19
30007250:	30009d95 	.word	0x30009d95
30007254:	30009c9d 	.word	0x30009c9d
30007258:	30009d59 	.word	0x30009d59

3000725c <BOOT_Validate_ImgHash>:
3000725c:	b530      	push	{r4, r5, lr}
3000725e:	b085      	sub	sp, #20
30007260:	4d0a      	ldr	r5, [pc, #40]	; (3000728c <BOOT_Validate_ImgHash+0x30>)
30007262:	4604      	mov	r4, r0
30007264:	9301      	str	r3, [sp, #4]
30007266:	e9cd 2102 	strd	r2, r1, [sp, #8]
3000726a:	47a8      	blx	r5
3000726c:	9903      	ldr	r1, [sp, #12]
3000726e:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30007272:	b128      	cbz	r0, 30007280 <BOOT_Validate_ImgHash+0x24>
30007274:	4620      	mov	r0, r4
30007276:	b005      	add	sp, #20
30007278:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
3000727c:	f002 bdee 	b.w	30009e5c <SBOOT_Validate_ImgHash>
30007280:	4620      	mov	r0, r4
30007282:	b005      	add	sp, #20
30007284:	e8bd 4030 	ldmia.w	sp!, {r4, r5, lr}
30007288:	f7ff beb6 	b.w	30006ff8 <_BOOT_Validate_ImgHash>
3000728c:	30009bb5 	.word	0x30009bb5

30007290 <BOOT_SignatureCheck>:
30007290:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007294:	461e      	mov	r6, r3
30007296:	b089      	sub	sp, #36	; 0x24
30007298:	4b79      	ldr	r3, [pc, #484]	; (30007480 <BOOT_SignatureCheck+0x1f0>)
3000729a:	4680      	mov	r8, r0
3000729c:	4689      	mov	r9, r1
3000729e:	4617      	mov	r7, r2
300072a0:	f10d 0c20 	add.w	ip, sp, #32
300072a4:	4c77      	ldr	r4, [pc, #476]	; (30007484 <BOOT_SignatureCheck+0x1f4>)
300072a6:	9d10      	ldr	r5, [sp, #64]	; 0x40
300072a8:	cb0f      	ldmia	r3, {r0, r1, r2, r3}
300072aa:	e90c 000f 	stmdb	ip, {r0, r1, r2, r3}
300072ae:	47a0      	blx	r4
300072b0:	b910      	cbnz	r0, 300072b8 <BOOT_SignatureCheck+0x28>
300072b2:	b009      	add	sp, #36	; 0x24
300072b4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300072b8:	4b73      	ldr	r3, [pc, #460]	; (30007488 <BOOT_SignatureCheck+0x1f8>)
300072ba:	2000      	movs	r0, #0
300072bc:	4798      	blx	r3
300072be:	f898 3013 	ldrb.w	r3, [r8, #19]
300072c2:	f898 2012 	ldrb.w	r2, [r8, #18]
300072c6:	f10d 010f 	add.w	r1, sp, #15
300072ca:	f10d 000e 	add.w	r0, sp, #14
300072ce:	f002 fdff 	bl	30009ed0 <SBOOT_Validate_Algorithm>
300072d2:	4604      	mov	r4, r0
300072d4:	b9e8      	cbnz	r0, 30007312 <BOOT_SignatureCheck+0x82>
300072d6:	69f1      	ldr	r1, [r6, #28]
300072d8:	b919      	cbnz	r1, 300072e2 <BOOT_SignatureCheck+0x52>
300072da:	e0cf      	b.n	3000747c <BOOT_SignatureCheck+0x1ec>
300072dc:	b2d0      	uxtb	r0, r2
300072de:	4288      	cmp	r0, r1
300072e0:	d208      	bcs.n	300072f4 <BOOT_SignatureCheck+0x64>
300072e2:	eb00 04c0 	add.w	r4, r0, r0, lsl #3
300072e6:	1c42      	adds	r2, r0, #1
300072e8:	eb06 0484 	add.w	r4, r6, r4, lsl #2
300072ec:	f8d4 30b0 	ldr.w	r3, [r4, #176]	; 0xb0
300072f0:	42ab      	cmp	r3, r5
300072f2:	d1f3      	bne.n	300072dc <BOOT_SignatureCheck+0x4c>
300072f4:	2224      	movs	r2, #36	; 0x24
300072f6:	23b4      	movs	r3, #180	; 0xb4
300072f8:	fb12 3200 	smlabb	r2, r2, r0, r3
300072fc:	f108 0570 	add.w	r5, r8, #112	; 0x70
30007300:	4432      	add	r2, r6
30007302:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007306:	4629      	mov	r1, r5
30007308:	f002 fd64 	bl	30009dd4 <SBOOT_Validate_PubKey>
3000730c:	4604      	mov	r4, r0
3000730e:	2800      	cmp	r0, #0
30007310:	d07a      	beq.n	30007408 <BOOT_SignatureCheck+0x178>
30007312:	f898 3011 	ldrb.w	r3, [r8, #17]
30007316:	aa08      	add	r2, sp, #32
30007318:	eb02 0383 	add.w	r3, r2, r3, lsl #2
3000731c:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007320:	4b5a      	ldr	r3, [pc, #360]	; (3000748c <BOOT_SignatureCheck+0x1fc>)
30007322:	2002      	movs	r0, #2
30007324:	4a5a      	ldr	r2, [pc, #360]	; (30007490 <BOOT_SignatureCheck+0x200>)
30007326:	e9cd 1400 	strd	r1, r4, [sp]
3000732a:	4619      	mov	r1, r3
3000732c:	f004 fdde 	bl	3000beec <rtk_log_write>
30007330:	2f00      	cmp	r7, #0
30007332:	d045      	beq.n	300073c0 <BOOT_SignatureCheck+0x130>
30007334:	464d      	mov	r5, r9
30007336:	f107 39ff 	add.w	r9, r7, #4294967295	; 0xffffffff
3000733a:	f04f 0800 	mov.w	r8, #0
3000733e:	4f55      	ldr	r7, [pc, #340]	; (30007494 <BOOT_SignatureCheck+0x204>)
30007340:	fa5f f389 	uxtb.w	r3, r9
30007344:	f105 0908 	add.w	r9, r5, #8
30007348:	1d2e      	adds	r6, r5, #4
3000734a:	4c53      	ldr	r4, [pc, #332]	; (30007498 <BOOT_SignatureCheck+0x208>)
3000734c:	eb09 09c3 	add.w	r9, r9, r3, lsl #3
30007350:	e003      	b.n	3000735a <BOOT_SignatureCheck+0xca>
30007352:	3508      	adds	r5, #8
30007354:	3608      	adds	r6, #8
30007356:	45a9      	cmp	r9, r5
30007358:	d032      	beq.n	300073c0 <BOOT_SignatureCheck+0x130>
3000735a:	f856 0c04 	ldr.w	r0, [r6, #-4]
3000735e:	f100 4378 	add.w	r3, r0, #4160749568	; 0xf8000000
30007362:	f1b3 6f00 	cmp.w	r3, #134217728	; 0x8000000
30007366:	d3f4      	bcc.n	30007352 <BOOT_SignatureCheck+0xc2>
30007368:	2100      	movs	r1, #0
3000736a:	686a      	ldr	r2, [r5, #4]
3000736c:	47b8      	blx	r7
3000736e:	6963      	ldr	r3, [r4, #20]
30007370:	03d9      	lsls	r1, r3, #15
30007372:	d5ee      	bpl.n	30007352 <BOOT_SignatureCheck+0xc2>
30007374:	f856 2c04 	ldr.w	r2, [r6, #-4]
30007378:	686b      	ldr	r3, [r5, #4]
3000737a:	ea02 0103 	and.w	r1, r2, r3
3000737e:	3101      	adds	r1, #1
30007380:	d022      	beq.n	300073c8 <BOOT_SignatureCheck+0x138>
30007382:	f012 011f 	ands.w	r1, r2, #31
30007386:	d002      	beq.n	3000738e <BOOT_SignatureCheck+0xfe>
30007388:	440b      	add	r3, r1
3000738a:	f022 021f 	bic.w	r2, r2, #31
3000738e:	f3bf 8f4f 	dsb	sy
30007392:	2b00      	cmp	r3, #0
30007394:	dd0c      	ble.n	300073b0 <BOOT_SignatureCheck+0x120>
30007396:	1e59      	subs	r1, r3, #1
30007398:	f102 0320 	add.w	r3, r2, #32
3000739c:	f021 011f 	bic.w	r1, r1, #31
300073a0:	4419      	add	r1, r3
300073a2:	428b      	cmp	r3, r1
300073a4:	f8c4 2270 	str.w	r2, [r4, #624]	; 0x270
300073a8:	461a      	mov	r2, r3
300073aa:	f103 0320 	add.w	r3, r3, #32
300073ae:	d1f8      	bne.n	300073a2 <BOOT_SignatureCheck+0x112>
300073b0:	f3bf 8f4f 	dsb	sy
300073b4:	f3bf 8f6f 	isb	sy
300073b8:	3508      	adds	r5, #8
300073ba:	3608      	adds	r6, #8
300073bc:	45a9      	cmp	r9, r5
300073be:	d1cc      	bne.n	3000735a <BOOT_SignatureCheck+0xca>
300073c0:	20ff      	movs	r0, #255	; 0xff
300073c2:	b009      	add	sp, #36	; 0x24
300073c4:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300073c8:	f8c4 8084 	str.w	r8, [r4, #132]	; 0x84
300073cc:	f3bf 8f4f 	dsb	sy
300073d0:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
300073d4:	f643 7ee0 	movw	lr, #16352	; 0x3fe0
300073d8:	f3c3 304e 	ubfx	r0, r3, #13, #15
300073dc:	f3c3 0cc9 	ubfx	ip, r3, #3, #10
300073e0:	0140      	lsls	r0, r0, #5
300073e2:	ea00 010e 	and.w	r1, r0, lr
300073e6:	4663      	mov	r3, ip
300073e8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300073ec:	3b01      	subs	r3, #1
300073ee:	f8c4 2274 	str.w	r2, [r4, #628]	; 0x274
300073f2:	1c5a      	adds	r2, r3, #1
300073f4:	d1f8      	bne.n	300073e8 <BOOT_SignatureCheck+0x158>
300073f6:	3820      	subs	r0, #32
300073f8:	f110 0f20 	cmn.w	r0, #32
300073fc:	d1f1      	bne.n	300073e2 <BOOT_SignatureCheck+0x152>
300073fe:	f3bf 8f4f 	dsb	sy
30007402:	f3bf 8f6f 	isb	sy
30007406:	e7d7      	b.n	300073b8 <BOOT_SignatureCheck+0x128>
30007408:	f508 7380 	add.w	r3, r8, #256	; 0x100
3000740c:	462a      	mov	r2, r5
3000740e:	f89d 100f 	ldrb.w	r1, [sp, #15]
30007412:	9301      	str	r3, [sp, #4]
30007414:	f44f 7380 	mov.w	r3, #256	; 0x100
30007418:	f89d 000e 	ldrb.w	r0, [sp, #14]
3000741c:	9300      	str	r3, [sp, #0]
3000741e:	4643      	mov	r3, r8
30007420:	f002 fcf6 	bl	30009e10 <SBOOT_Validate_Signature>
30007424:	4604      	mov	r4, r0
30007426:	2800      	cmp	r0, #0
30007428:	f47f af73 	bne.w	30007312 <BOOT_SignatureCheck+0x82>
3000742c:	f108 0530 	add.w	r5, r8, #48	; 0x30
30007430:	4b1a      	ldr	r3, [pc, #104]	; (3000749c <BOOT_SignatureCheck+0x20c>)
30007432:	f89d 400f 	ldrb.w	r4, [sp, #15]
30007436:	4798      	blx	r3
30007438:	4629      	mov	r1, r5
3000743a:	b9c0      	cbnz	r0, 3000746e <BOOT_SignatureCheck+0x1de>
3000743c:	4620      	mov	r0, r4
3000743e:	463b      	mov	r3, r7
30007440:	464a      	mov	r2, r9
30007442:	f7ff fdd9 	bl	30006ff8 <_BOOT_Validate_ImgHash>
30007446:	4604      	mov	r4, r0
30007448:	f898 3011 	ldrb.w	r3, [r8, #17]
3000744c:	aa08      	add	r2, sp, #32
3000744e:	eb02 0383 	add.w	r3, r2, r3, lsl #2
30007452:	f853 1c10 	ldr.w	r1, [r3, #-16]
30007456:	2c00      	cmp	r4, #0
30007458:	f47f af62 	bne.w	30007320 <BOOT_SignatureCheck+0x90>
3000745c:	4b0b      	ldr	r3, [pc, #44]	; (3000748c <BOOT_SignatureCheck+0x1fc>)
3000745e:	2004      	movs	r0, #4
30007460:	9100      	str	r1, [sp, #0]
30007462:	4a0f      	ldr	r2, [pc, #60]	; (300074a0 <BOOT_SignatureCheck+0x210>)
30007464:	4619      	mov	r1, r3
30007466:	f004 fd41 	bl	3000beec <rtk_log_write>
3000746a:	4620      	mov	r0, r4
3000746c:	e721      	b.n	300072b2 <BOOT_SignatureCheck+0x22>
3000746e:	4620      	mov	r0, r4
30007470:	463b      	mov	r3, r7
30007472:	464a      	mov	r2, r9
30007474:	f002 fcf2 	bl	30009e5c <SBOOT_Validate_ImgHash>
30007478:	4604      	mov	r4, r0
3000747a:	e7e5      	b.n	30007448 <BOOT_SignatureCheck+0x1b8>
3000747c:	22b4      	movs	r2, #180	; 0xb4
3000747e:	e73d      	b.n	300072fc <BOOT_SignatureCheck+0x6c>
30007480:	3000e3dc 	.word	0x3000e3dc
30007484:	0000c149 	.word	0x0000c149
30007488:	30009e99 	.word	0x30009e99
3000748c:	3000c6b0 	.word	0x3000c6b0
30007490:	3000d150 	.word	0x3000d150
30007494:	00012be5 	.word	0x00012be5
30007498:	e000ed00 	.word	0xe000ed00
3000749c:	30009bb5 	.word	0x30009bb5
300074a0:	3000d138 	.word	0x3000d138

300074a4 <BOOT_CertificateCheck>:
300074a4:	b570      	push	{r4, r5, r6, lr}
300074a6:	4b2b      	ldr	r3, [pc, #172]	; (30007554 <BOOT_CertificateCheck+0xb0>)
300074a8:	b0b0      	sub	sp, #192	; 0xc0
300074aa:	4604      	mov	r4, r0
300074ac:	460d      	mov	r5, r1
300074ae:	4798      	blx	r3
300074b0:	b910      	cbnz	r0, 300074b8 <BOOT_CertificateCheck+0x14>
300074b2:	2000      	movs	r0, #0
300074b4:	b030      	add	sp, #192	; 0xc0
300074b6:	bd70      	pop	{r4, r5, r6, pc}
300074b8:	69a1      	ldr	r1, [r4, #24]
300074ba:	2290      	movs	r2, #144	; 0x90
300074bc:	a80c      	add	r0, sp, #48	; 0x30
300074be:	4429      	add	r1, r5
300074c0:	f7fd fa42 	bl	30004948 <BOOT_ImgCopy>
300074c4:	4b24      	ldr	r3, [pc, #144]	; (30007558 <BOOT_CertificateCheck+0xb4>)
300074c6:	4798      	blx	r3
300074c8:	4924      	ldr	r1, [pc, #144]	; (3000755c <BOOT_CertificateCheck+0xb8>)
300074ca:	f000 031f 	and.w	r3, r0, #31
300074ce:	2220      	movs	r2, #32
300074d0:	6808      	ldr	r0, [r1, #0]
300074d2:	f100 4084 	add.w	r0, r0, #1107296256	; 0x42000000
300074d6:	a930      	add	r1, sp, #192	; 0xc0
300074d8:	3a01      	subs	r2, #1
300074da:	f813 c000 	ldrb.w	ip, [r3, r0]
300074de:	4419      	add	r1, r3
300074e0:	f012 02ff 	ands.w	r2, r2, #255	; 0xff
300074e4:	f103 0301 	add.w	r3, r3, #1
300074e8:	f801 ccb0 	strb.w	ip, [r1, #-176]
300074ec:	f003 031f 	and.w	r3, r3, #31
300074f0:	d1f1      	bne.n	300074d6 <BOOT_CertificateCheck+0x32>
300074f2:	4610      	mov	r0, r2
300074f4:	4b1a      	ldr	r3, [pc, #104]	; (30007560 <BOOT_CertificateCheck+0xbc>)
300074f6:	4798      	blx	r3
300074f8:	7ce3      	ldrb	r3, [r4, #19]
300074fa:	7ca2      	ldrb	r2, [r4, #18]
300074fc:	f10d 010f 	add.w	r1, sp, #15
30007500:	f10d 000e 	add.w	r0, sp, #14
30007504:	f002 fce4 	bl	30009ed0 <SBOOT_Validate_Algorithm>
30007508:	4605      	mov	r5, r0
3000750a:	b9c0      	cbnz	r0, 3000753e <BOOT_CertificateCheck+0x9a>
3000750c:	f104 0620 	add.w	r6, r4, #32
30007510:	aa04      	add	r2, sp, #16
30007512:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007516:	4631      	mov	r1, r6
30007518:	f002 fc5c 	bl	30009dd4 <SBOOT_Validate_PubKey>
3000751c:	4605      	mov	r5, r0
3000751e:	b970      	cbnz	r0, 3000753e <BOOT_CertificateCheck+0x9a>
30007520:	69a0      	ldr	r0, [r4, #24]
30007522:	4623      	mov	r3, r4
30007524:	ac0c      	add	r4, sp, #48	; 0x30
30007526:	4632      	mov	r2, r6
30007528:	f89d 100f 	ldrb.w	r1, [sp, #15]
3000752c:	e9cd 0400 	strd	r0, r4, [sp]
30007530:	f89d 000e 	ldrb.w	r0, [sp, #14]
30007534:	f002 fc6c 	bl	30009e10 <SBOOT_Validate_Signature>
30007538:	4605      	mov	r5, r0
3000753a:	2800      	cmp	r0, #0
3000753c:	d0b9      	beq.n	300074b2 <BOOT_CertificateCheck+0xe>
3000753e:	4b09      	ldr	r3, [pc, #36]	; (30007564 <BOOT_CertificateCheck+0xc0>)
30007540:	2002      	movs	r0, #2
30007542:	4a09      	ldr	r2, [pc, #36]	; (30007568 <BOOT_CertificateCheck+0xc4>)
30007544:	4619      	mov	r1, r3
30007546:	9500      	str	r5, [sp, #0]
30007548:	f004 fcd0 	bl	3000beec <rtk_log_write>
3000754c:	20ff      	movs	r0, #255	; 0xff
3000754e:	b030      	add	sp, #192	; 0xc0
30007550:	bd70      	pop	{r4, r5, r6, pc}
30007552:	bf00      	nop
30007554:	0000c149 	.word	0x0000c149
30007558:	0000ea4d 	.word	0x0000ea4d
3000755c:	3000e6dc 	.word	0x3000e6dc
30007560:	30009e99 	.word	0x30009e99
30007564:	3000c6b0 	.word	0x3000c6b0
30007568:	3000d174 	.word	0x3000d174

3000756c <BOOT_RAM_TZCfg>:
3000756c:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30007570:	b085      	sub	sp, #20
30007572:	4e58      	ldr	r6, [pc, #352]	; (300076d4 <BOOT_RAM_TZCfg+0x168>)
30007574:	4958      	ldr	r1, [pc, #352]	; (300076d8 <BOOT_RAM_TZCfg+0x16c>)
30007576:	2501      	movs	r5, #1
30007578:	f10d 0e08 	add.w	lr, sp, #8
3000757c:	4a57      	ldr	r2, [pc, #348]	; (300076dc <BOOT_RAM_TZCfg+0x170>)
3000757e:	4b58      	ldr	r3, [pc, #352]	; (300076e0 <BOOT_RAM_TZCfg+0x174>)
30007580:	46ec      	mov	ip, sp
30007582:	4677      	mov	r7, lr
30007584:	f8df 8178 	ldr.w	r8, [pc, #376]	; 30007700 <BOOT_RAM_TZCfg+0x194>
30007588:	f44f 1ac0 	mov.w	sl, #1572864	; 0x180000
3000758c:	f8df 9174 	ldr.w	r9, [pc, #372]	; 30007704 <BOOT_RAM_TZCfg+0x198>
30007590:	9202      	str	r2, [sp, #8]
30007592:	9303      	str	r3, [sp, #12]
30007594:	e9cd 6100 	strd	r6, r1, [sp]
30007598:	f85c 2b04 	ldr.w	r2, [ip], #4
3000759c:	2300      	movs	r3, #0
3000759e:	f85e 1b04 	ldr.w	r1, [lr], #4
300075a2:	680c      	ldr	r4, [r1, #0]
300075a4:	eb02 0bc3 	add.w	fp, r2, r3, lsl #3
300075a8:	fa05 f003 	lsl.w	r0, r5, r3
300075ac:	f1b4 3fff 	cmp.w	r4, #4294967295	; 0xffffffff
300075b0:	d00c      	beq.n	300075cc <BOOT_RAM_TZCfg+0x60>
300075b2:	f842 4033 	str.w	r4, [r2, r3, lsl #3]
300075b6:	3301      	adds	r3, #1
300075b8:	684c      	ldr	r4, [r1, #4]
300075ba:	310c      	adds	r1, #12
300075bc:	2b08      	cmp	r3, #8
300075be:	f8cb 4004 	str.w	r4, [fp, #4]
300075c2:	6c14      	ldr	r4, [r2, #64]	; 0x40
300075c4:	ea40 0004 	orr.w	r0, r0, r4
300075c8:	6410      	str	r0, [r2, #64]	; 0x40
300075ca:	d1ea      	bne.n	300075a2 <BOOT_RAM_TZCfg+0x36>
300075cc:	42b2      	cmp	r2, r6
300075ce:	d072      	beq.n	300076b6 <BOOT_RAM_TZCfg+0x14a>
300075d0:	6455      	str	r5, [r2, #68]	; 0x44
300075d2:	45bc      	cmp	ip, r7
300075d4:	d1e0      	bne.n	30007598 <BOOT_RAM_TZCfg+0x2c>
300075d6:	4b43      	ldr	r3, [pc, #268]	; (300076e4 <BOOT_RAM_TZCfg+0x178>)
300075d8:	f46f 3100 	mvn.w	r1, #131072	; 0x20000
300075dc:	f04f 32ff 	mov.w	r2, #4294967295	; 0xffffffff
300075e0:	4841      	ldr	r0, [pc, #260]	; (300076e8 <BOOT_RAM_TZCfg+0x17c>)
300075e2:	6019      	str	r1, [r3, #0]
300075e4:	2501      	movs	r5, #1
300075e6:	605a      	str	r2, [r3, #4]
300075e8:	2100      	movs	r1, #0
300075ea:	4a40      	ldr	r2, [pc, #256]	; (300076ec <BOOT_RAM_TZCfg+0x180>)
300075ec:	4c40      	ldr	r4, [pc, #256]	; (300076f0 <BOOT_RAM_TZCfg+0x184>)
300075ee:	f8c0 5398 	str.w	r5, [r0, #920]	; 0x398
300075f2:	6813      	ldr	r3, [r2, #0]
300075f4:	3301      	adds	r3, #1
300075f6:	d019      	beq.n	3000762c <BOOT_RAM_TZCfg+0xc0>
300075f8:	f8c4 10d8 	str.w	r1, [r4, #216]	; 0xd8
300075fc:	320c      	adds	r2, #12
300075fe:	f852 3c0c 	ldr.w	r3, [r2, #-12]
30007602:	3101      	adds	r1, #1
30007604:	f023 031f 	bic.w	r3, r3, #31
30007608:	2908      	cmp	r1, #8
3000760a:	f8c4 30dc 	str.w	r3, [r4, #220]	; 0xdc
3000760e:	e952 0302 	ldrd	r0, r3, [r2, #-8]
30007612:	ea4f 0343 	mov.w	r3, r3, lsl #1
30007616:	f020 001f 	bic.w	r0, r0, #31
3000761a:	f003 0302 	and.w	r3, r3, #2
3000761e:	ea43 0300 	orr.w	r3, r3, r0
30007622:	f043 0301 	orr.w	r3, r3, #1
30007626:	f8c4 30e0 	str.w	r3, [r4, #224]	; 0xe0
3000762a:	d1e2      	bne.n	300075f2 <BOOT_RAM_TZCfg+0x86>
3000762c:	4830      	ldr	r0, [pc, #192]	; (300076f0 <BOOT_RAM_TZCfg+0x184>)
3000762e:	2201      	movs	r2, #1
30007630:	4b30      	ldr	r3, [pc, #192]	; (300076f4 <BOOT_RAM_TZCfg+0x188>)
30007632:	f8c0 20d0 	str.w	r2, [r0, #208]	; 0xd0
30007636:	f8d3 3268 	ldr.w	r3, [r3, #616]	; 0x268
3000763a:	f013 5300 	ands.w	r3, r3, #536870912	; 0x20000000
3000763e:	d11e      	bne.n	3000767e <BOOT_RAM_TZCfg+0x112>
30007640:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30007644:	f3bf 8f4f 	dsb	sy
30007648:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
3000764c:	f643 76e0 	movw	r6, #16352	; 0x3fe0
30007650:	f3c5 344e 	ubfx	r4, r5, #13, #15
30007654:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30007658:	0164      	lsls	r4, r4, #5
3000765a:	ea04 0106 	and.w	r1, r4, r6
3000765e:	462b      	mov	r3, r5
30007660:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30007664:	3b01      	subs	r3, #1
30007666:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
3000766a:	1c5a      	adds	r2, r3, #1
3000766c:	d1f8      	bne.n	30007660 <BOOT_RAM_TZCfg+0xf4>
3000766e:	3c20      	subs	r4, #32
30007670:	f114 0f20 	cmn.w	r4, #32
30007674:	d1f1      	bne.n	3000765a <BOOT_RAM_TZCfg+0xee>
30007676:	f3bf 8f4f 	dsb	sy
3000767a:	f3bf 8f6f 	isb	sy
3000767e:	4b1c      	ldr	r3, [pc, #112]	; (300076f0 <BOOT_RAM_TZCfg+0x184>)
30007680:	f649 75f7 	movw	r5, #40951	; 0x9ff7
30007684:	491c      	ldr	r1, [pc, #112]	; (300076f8 <BOOT_RAM_TZCfg+0x18c>)
30007686:	691c      	ldr	r4, [r3, #16]
30007688:	4a1c      	ldr	r2, [pc, #112]	; (300076fc <BOOT_RAM_TZCfg+0x190>)
3000768a:	f044 0408 	orr.w	r4, r4, #8
3000768e:	611c      	str	r4, [r3, #16]
30007690:	f04f 34ff 	mov.w	r4, #4294967295	; 0xffffffff
30007694:	68d8      	ldr	r0, [r3, #12]
30007696:	4028      	ands	r0, r5
30007698:	4301      	orrs	r1, r0
3000769a:	60d9      	str	r1, [r3, #12]
3000769c:	f46f 0180 	mvn.w	r1, #4194304	; 0x400000
300076a0:	f06f 03c0 	mvn.w	r3, #192	; 0xc0
300076a4:	f8c2 4280 	str.w	r4, [r2, #640]	; 0x280
300076a8:	f8c2 1284 	str.w	r1, [r2, #644]	; 0x284
300076ac:	f8c2 3288 	str.w	r3, [r2, #648]	; 0x288
300076b0:	b005      	add	sp, #20
300076b2:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300076b6:	f898 3000 	ldrb.w	r3, [r8]
300076ba:	2b00      	cmp	r3, #0
300076bc:	d189      	bne.n	300075d2 <BOOT_RAM_TZCfg+0x66>
300076be:	f8c6 a038 	str.w	sl, [r6, #56]	; 0x38
300076c2:	f8c6 903c 	str.w	r9, [r6, #60]	; 0x3c
300076c6:	6c33      	ldr	r3, [r6, #64]	; 0x40
300076c8:	f043 0380 	orr.w	r3, r3, #128	; 0x80
300076cc:	6433      	str	r3, [r6, #64]	; 0x40
300076ce:	6475      	str	r5, [r6, #68]	; 0x44
300076d0:	e77f      	b.n	300075d2 <BOOT_RAM_TZCfg+0x66>
300076d2:	bf00      	nop
300076d4:	51001a00 	.word	0x51001a00
300076d8:	51001200 	.word	0x51001200
300076dc:	3000e7dc 	.word	0x3000e7dc
300076e0:	3000e77c 	.word	0x3000e77c
300076e4:	51001b00 	.word	0x51001b00
300076e8:	500e0000 	.word	0x500e0000
300076ec:	3000e83c 	.word	0x3000e83c
300076f0:	e000ed00 	.word	0xe000ed00
300076f4:	42008000 	.word	0x42008000
300076f8:	05fa6000 	.word	0x05fa6000
300076fc:	e000e100 	.word	0xe000e100
30007700:	3000e6d7 	.word	0x3000e6d7
30007704:	005fffff 	.word	0x005fffff

30007708 <ymodem_inquiry>:
30007708:	2806      	cmp	r0, #6
3000770a:	b510      	push	{r4, lr}
3000770c:	4604      	mov	r4, r0
3000770e:	d003      	beq.n	30007718 <ymodem_inquiry+0x10>
30007710:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007714:	4b03      	ldr	r3, [pc, #12]	; (30007724 <ymodem_inquiry+0x1c>)
30007716:	4798      	blx	r3
30007718:	4620      	mov	r0, r4
3000771a:	f000 fa07 	bl	30007b2c <ymodem_uart_putc>
3000771e:	2001      	movs	r0, #1
30007720:	bd10      	pop	{r4, pc}
30007722:	bf00      	nop
30007724:	00009b2d 	.word	0x00009b2d

30007728 <ymodem_get_first>:
30007728:	b530      	push	{r4, r5, lr}
3000772a:	b083      	sub	sp, #12
3000772c:	2300      	movs	r3, #0
3000772e:	4604      	mov	r4, r0
30007730:	4936      	ldr	r1, [pc, #216]	; (3000780c <ymodem_get_first+0xe4>)
30007732:	f10d 0006 	add.w	r0, sp, #6
30007736:	f88d 3006 	strb.w	r3, [sp, #6]
3000773a:	f000 fa19 	bl	30007b70 <ymodem_uart_getc_to>
3000773e:	b9d0      	cbnz	r0, 30007776 <ymodem_get_first+0x4e>
30007740:	f89d 3006 	ldrb.w	r3, [sp, #6]
30007744:	3b01      	subs	r3, #1
30007746:	2b17      	cmp	r3, #23
30007748:	d80d      	bhi.n	30007766 <ymodem_get_first+0x3e>
3000774a:	e8df f003 	tbb	[pc, r3]
3000774e:	231e      	.short	0x231e
30007750:	0c0c290c 	.word	0x0c0c290c
30007754:	0c0c0c0c 	.word	0x0c0c0c0c
30007758:	0c0c0c0c 	.word	0x0c0c0c0c
3000775c:	0c0c0c0c 	.word	0x0c0c0c0c
30007760:	0c0c0c0c 	.word	0x0c0c0c0c
30007764:	190c      	.short	0x190c
30007766:	4d29      	ldr	r5, [pc, #164]	; (3000780c <ymodem_get_first+0xe4>)
30007768:	4629      	mov	r1, r5
3000776a:	f10d 0007 	add.w	r0, sp, #7
3000776e:	f000 f9ff 	bl	30007b70 <ymodem_uart_getc_to>
30007772:	2800      	cmp	r0, #0
30007774:	d0f8      	beq.n	30007768 <ymodem_get_first+0x40>
30007776:	2301      	movs	r3, #1
30007778:	2008      	movs	r0, #8
3000777a:	60a3      	str	r3, [r4, #8]
3000777c:	b003      	add	sp, #12
3000777e:	bd30      	pop	{r4, r5, pc}
30007780:	2301      	movs	r3, #1
30007782:	2002      	movs	r0, #2
30007784:	60a3      	str	r3, [r4, #8]
30007786:	b003      	add	sp, #12
30007788:	bd30      	pop	{r4, r5, pc}
3000778a:	2380      	movs	r3, #128	; 0x80
3000778c:	2001      	movs	r0, #1
3000778e:	80a3      	strh	r3, [r4, #4]
30007790:	b003      	add	sp, #12
30007792:	bd30      	pop	{r4, r5, pc}
30007794:	f44f 6380 	mov.w	r3, #1024	; 0x400
30007798:	2001      	movs	r0, #1
3000779a:	80a3      	strh	r3, [r4, #4]
3000779c:	b003      	add	sp, #12
3000779e:	bd30      	pop	{r4, r5, pc}
300077a0:	2006      	movs	r0, #6
300077a2:	4d1b      	ldr	r5, [pc, #108]	; (30007810 <ymodem_get_first+0xe8>)
300077a4:	f000 f9c2 	bl	30007b2c <ymodem_uart_putc>
300077a8:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077ac:	47a8      	blx	r5
300077ae:	2043      	movs	r0, #67	; 0x43
300077b0:	f000 f9bc 	bl	30007b2c <ymodem_uart_putc>
300077b4:	2006      	movs	r0, #6
300077b6:	f000 f9b9 	bl	30007b2c <ymodem_uart_putc>
300077ba:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077be:	47a8      	blx	r5
300077c0:	2043      	movs	r0, #67	; 0x43
300077c2:	f000 f9b3 	bl	30007b2c <ymodem_uart_putc>
300077c6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077ca:	47a8      	blx	r5
300077cc:	2018      	movs	r0, #24
300077ce:	f000 f9ad 	bl	30007b2c <ymodem_uart_putc>
300077d2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077d6:	47a8      	blx	r5
300077d8:	2018      	movs	r0, #24
300077da:	f000 f9a7 	bl	30007b2c <ymodem_uart_putc>
300077de:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077e2:	47a8      	blx	r5
300077e4:	2018      	movs	r0, #24
300077e6:	f000 f9a1 	bl	30007b2c <ymodem_uart_putc>
300077ea:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077ee:	47a8      	blx	r5
300077f0:	2018      	movs	r0, #24
300077f2:	f000 f99b 	bl	30007b2c <ymodem_uart_putc>
300077f6:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300077fa:	47a8      	blx	r5
300077fc:	2018      	movs	r0, #24
300077fe:	f000 f995 	bl	30007b2c <ymodem_uart_putc>
30007802:	2301      	movs	r3, #1
30007804:	2005      	movs	r0, #5
30007806:	60a3      	str	r3, [r4, #8]
30007808:	b003      	add	sp, #12
3000780a:	bd30      	pop	{r4, r5, pc}
3000780c:	000caa30 	.word	0x000caa30
30007810:	00009b2d 	.word	0x00009b2d

30007814 <ymodem_get_others>:
30007814:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30007818:	b083      	sub	sp, #12
3000781a:	2300      	movs	r3, #0
3000781c:	4605      	mov	r5, r0
3000781e:	4952      	ldr	r1, [pc, #328]	; (30007968 <ymodem_get_others+0x154>)
30007820:	a801      	add	r0, sp, #4
30007822:	f88d 3006 	strb.w	r3, [sp, #6]
30007826:	f88d 3007 	strb.w	r3, [sp, #7]
3000782a:	f000 f9a1 	bl	30007b70 <ymodem_uart_getc_to>
3000782e:	bb38      	cbnz	r0, 30007880 <ymodem_get_others+0x6c>
30007830:	494d      	ldr	r1, [pc, #308]	; (30007968 <ymodem_get_others+0x154>)
30007832:	f10d 0005 	add.w	r0, sp, #5
30007836:	f000 f99b 	bl	30007b70 <ymodem_uart_getc_to>
3000783a:	4607      	mov	r7, r0
3000783c:	bb00      	cbnz	r0, 30007880 <ymodem_get_others+0x6c>
3000783e:	f89d 2004 	ldrb.w	r2, [sp, #4]
30007842:	f89d 6005 	ldrb.w	r6, [sp, #5]
30007846:	43d3      	mvns	r3, r2
30007848:	b2db      	uxtb	r3, r3
3000784a:	429e      	cmp	r6, r3
3000784c:	bf0c      	ite	eq
3000784e:	2601      	moveq	r6, #1
30007850:	2604      	movne	r6, #4
30007852:	682b      	ldr	r3, [r5, #0]
30007854:	429a      	cmp	r2, r3
30007856:	d21a      	bcs.n	3000788e <ymodem_get_others+0x7a>
30007858:	2701      	movs	r7, #1
3000785a:	88ab      	ldrh	r3, [r5, #4]
3000785c:	b1d3      	cbz	r3, 30007894 <ymodem_get_others+0x80>
3000785e:	2400      	movs	r4, #0
30007860:	f8df 9108 	ldr.w	r9, [pc, #264]	; 3000796c <ymodem_get_others+0x158>
30007864:	f8df 8100 	ldr.w	r8, [pc, #256]	; 30007968 <ymodem_get_others+0x154>
30007868:	e002      	b.n	30007870 <ymodem_get_others+0x5c>
3000786a:	88ab      	ldrh	r3, [r5, #4]
3000786c:	42a3      	cmp	r3, r4
3000786e:	d911      	bls.n	30007894 <ymodem_get_others+0x80>
30007870:	eb04 0009 	add.w	r0, r4, r9
30007874:	4641      	mov	r1, r8
30007876:	3401      	adds	r4, #1
30007878:	f000 f97a 	bl	30007b70 <ymodem_uart_getc_to>
3000787c:	2800      	cmp	r0, #0
3000787e:	d0f4      	beq.n	3000786a <ymodem_get_others+0x56>
30007880:	2301      	movs	r3, #1
30007882:	2608      	movs	r6, #8
30007884:	60ab      	str	r3, [r5, #8]
30007886:	4630      	mov	r0, r6
30007888:	b003      	add	sp, #12
3000788a:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
3000788e:	d0e4      	beq.n	3000785a <ymodem_get_others+0x46>
30007890:	2604      	movs	r6, #4
30007892:	e7e2      	b.n	3000785a <ymodem_get_others+0x46>
30007894:	4934      	ldr	r1, [pc, #208]	; (30007968 <ymodem_get_others+0x154>)
30007896:	f10d 0006 	add.w	r0, sp, #6
3000789a:	f000 f969 	bl	30007b70 <ymodem_uart_getc_to>
3000789e:	2800      	cmp	r0, #0
300078a0:	d1ee      	bne.n	30007880 <ymodem_get_others+0x6c>
300078a2:	4931      	ldr	r1, [pc, #196]	; (30007968 <ymodem_get_others+0x154>)
300078a4:	f10d 0007 	add.w	r0, sp, #7
300078a8:	f000 f962 	bl	30007b70 <ymodem_uart_getc_to>
300078ac:	2800      	cmp	r0, #0
300078ae:	d1e7      	bne.n	30007880 <ymodem_get_others+0x6c>
300078b0:	f89d 3006 	ldrb.w	r3, [sp, #6]
300078b4:	f89d e007 	ldrb.w	lr, [sp, #7]
300078b8:	88aa      	ldrh	r2, [r5, #4]
300078ba:	ea4e 2e03 	orr.w	lr, lr, r3, lsl #8
300078be:	b36a      	cbz	r2, 3000791c <ymodem_get_others+0x108>
300078c0:	4603      	mov	r3, r0
300078c2:	482a      	ldr	r0, [pc, #168]	; (3000796c <ymodem_get_others+0x158>)
300078c4:	f241 0c21 	movw	ip, #4129	; 0x1021
300078c8:	eb02 0800 	add.w	r8, r2, r0
300078cc:	7801      	ldrb	r1, [r0, #0]
300078ce:	3001      	adds	r0, #1
300078d0:	ea83 2301 	eor.w	r3, r3, r1, lsl #8
300078d4:	2108      	movs	r1, #8
300078d6:	f403 4400 	and.w	r4, r3, #32768	; 0x8000
300078da:	005b      	lsls	r3, r3, #1
300078dc:	b10c      	cbz	r4, 300078e2 <ymodem_get_others+0xce>
300078de:	ea83 030c 	eor.w	r3, r3, ip
300078e2:	3901      	subs	r1, #1
300078e4:	d1f7      	bne.n	300078d6 <ymodem_get_others+0xc2>
300078e6:	4580      	cmp	r8, r0
300078e8:	d1f0      	bne.n	300078cc <ymodem_get_others+0xb8>
300078ea:	b29b      	uxth	r3, r3
300078ec:	459e      	cmp	lr, r3
300078ee:	d00a      	beq.n	30007906 <ymodem_get_others+0xf2>
300078f0:	4b1f      	ldr	r3, [pc, #124]	; (30007970 <ymodem_get_others+0x15c>)
300078f2:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
300078f6:	4798      	blx	r3
300078f8:	2015      	movs	r0, #21
300078fa:	f000 f917 	bl	30007b2c <ymodem_uart_putc>
300078fe:	2301      	movs	r3, #1
30007900:	2604      	movs	r6, #4
30007902:	60ab      	str	r3, [r5, #8]
30007904:	e7bf      	b.n	30007886 <ymodem_get_others+0x72>
30007906:	b917      	cbnz	r7, 3000790e <ymodem_get_others+0xfa>
30007908:	4c1a      	ldr	r4, [pc, #104]	; (30007974 <ymodem_get_others+0x160>)
3000790a:	6823      	ldr	r3, [r4, #0]
3000790c:	b9b3      	cbnz	r3, 3000793c <ymodem_get_others+0x128>
3000790e:	f000 f947 	bl	30007ba0 <ymodem_uart_clean_rx>
30007912:	2e01      	cmp	r6, #1
30007914:	d004      	beq.n	30007920 <ymodem_get_others+0x10c>
30007916:	2e04      	cmp	r6, #4
30007918:	d1b5      	bne.n	30007886 <ymodem_get_others+0x72>
3000791a:	e7e9      	b.n	300078f0 <ymodem_get_others+0xdc>
3000791c:	4613      	mov	r3, r2
3000791e:	e7e5      	b.n	300078ec <ymodem_get_others+0xd8>
30007920:	4c14      	ldr	r4, [pc, #80]	; (30007974 <ymodem_get_others+0x160>)
30007922:	2006      	movs	r0, #6
30007924:	60ae      	str	r6, [r5, #8]
30007926:	f000 f901 	bl	30007b2c <ymodem_uart_putc>
3000792a:	6823      	ldr	r3, [r4, #0]
3000792c:	b193      	cbz	r3, 30007954 <ymodem_get_others+0x140>
3000792e:	2f00      	cmp	r7, #0
30007930:	f103 0301 	add.w	r3, r3, #1
30007934:	bf18      	it	ne
30007936:	2604      	movne	r6, #4
30007938:	6023      	str	r3, [r4, #0]
3000793a:	e7a4      	b.n	30007886 <ymodem_get_others+0x72>
3000793c:	68eb      	ldr	r3, [r5, #12]
3000793e:	6861      	ldr	r1, [r4, #4]
30007940:	3301      	adds	r3, #1
30007942:	480a      	ldr	r0, [pc, #40]	; (3000796c <ymodem_get_others+0x158>)
30007944:	60eb      	str	r3, [r5, #12]
30007946:	f000 f94f 	bl	30007be8 <UARTIMG_Write>
3000794a:	6862      	ldr	r2, [r4, #4]
3000794c:	88ab      	ldrh	r3, [r5, #4]
3000794e:	4413      	add	r3, r2
30007950:	6063      	str	r3, [r4, #4]
30007952:	e7dc      	b.n	3000790e <ymodem_get_others+0xfa>
30007954:	4b06      	ldr	r3, [pc, #24]	; (30007970 <ymodem_get_others+0x15c>)
30007956:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
3000795a:	4798      	blx	r3
3000795c:	2043      	movs	r0, #67	; 0x43
3000795e:	f000 f8e5 	bl	30007b2c <ymodem_uart_putc>
30007962:	6823      	ldr	r3, [r4, #0]
30007964:	e7e3      	b.n	3000792e <ymodem_get_others+0x11a>
30007966:	bf00      	nop
30007968:	000caa30 	.word	0x000caa30
3000796c:	20016ffc 	.word	0x20016ffc
30007970:	00009b2d 	.word	0x00009b2d
30007974:	3000ee20 	.word	0x3000ee20

30007978 <ymodem_rx_frame>:
30007978:	2300      	movs	r3, #0
3000797a:	2201      	movs	r2, #1
3000797c:	b510      	push	{r4, lr}
3000797e:	4604      	mov	r4, r0
30007980:	6083      	str	r3, [r0, #8]
30007982:	8082      	strh	r2, [r0, #4]
30007984:	f7ff fed0 	bl	30007728 <ymodem_get_first>
30007988:	68a3      	ldr	r3, [r4, #8]
3000798a:	b92b      	cbnz	r3, 30007998 <ymodem_rx_frame+0x20>
3000798c:	4620      	mov	r0, r4
3000798e:	f7ff ff41 	bl	30007814 <ymodem_get_others>
30007992:	68a3      	ldr	r3, [r4, #8]
30007994:	2b00      	cmp	r3, #0
30007996:	d0f9      	beq.n	3000798c <ymodem_rx_frame+0x14>
30007998:	2801      	cmp	r0, #1
3000799a:	d000      	beq.n	3000799e <ymodem_rx_frame+0x26>
3000799c:	bd10      	pop	{r4, pc}
3000799e:	6823      	ldr	r3, [r4, #0]
300079a0:	3301      	adds	r3, #1
300079a2:	b2db      	uxtb	r3, r3
300079a4:	6023      	str	r3, [r4, #0]
300079a6:	bd10      	pop	{r4, pc}

300079a8 <ymodem_img_rxbuffer>:
300079a8:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300079ac:	2300      	movs	r3, #0
300079ae:	4c28      	ldr	r4, [pc, #160]	; (30007a50 <ymodem_img_rxbuffer+0xa8>)
300079b0:	4e28      	ldr	r6, [pc, #160]	; (30007a54 <ymodem_img_rxbuffer+0xac>)
300079b2:	460d      	mov	r5, r1
300079b4:	60a3      	str	r3, [r4, #8]
300079b6:	6163      	str	r3, [r4, #20]
300079b8:	f000 f8f2 	bl	30007ba0 <ymodem_uart_clean_rx>
300079bc:	2043      	movs	r0, #67	; 0x43
300079be:	f000 f8b5 	bl	30007b2c <ymodem_uart_putc>
300079c2:	f24c 3050 	movw	r0, #50000	; 0xc350
300079c6:	47b0      	blx	r6
300079c8:	f000 f89a 	bl	30007b00 <ymodem_uart_readable>
300079cc:	2800      	cmp	r0, #0
300079ce:	d0f5      	beq.n	300079bc <ymodem_img_rxbuffer+0x14>
300079d0:	2700      	movs	r7, #0
300079d2:	f04f 0801 	mov.w	r8, #1
300079d6:	e9c4 7500 	strd	r7, r5, [r4]
300079da:	4d1f      	ldr	r5, [pc, #124]	; (30007a58 <ymodem_img_rxbuffer+0xb0>)
300079dc:	4628      	mov	r0, r5
300079de:	f8a4 800c 	strh.w	r8, [r4, #12]
300079e2:	6127      	str	r7, [r4, #16]
300079e4:	f7ff fea0 	bl	30007728 <ymodem_get_first>
300079e8:	6923      	ldr	r3, [r4, #16]
300079ea:	b92b      	cbnz	r3, 300079f8 <ymodem_img_rxbuffer+0x50>
300079ec:	4628      	mov	r0, r5
300079ee:	f7ff ff11 	bl	30007814 <ymodem_get_others>
300079f2:	6923      	ldr	r3, [r4, #16]
300079f4:	2b00      	cmp	r3, #0
300079f6:	d0f9      	beq.n	300079ec <ymodem_img_rxbuffer+0x44>
300079f8:	2801      	cmp	r0, #1
300079fa:	d024      	beq.n	30007a46 <ymodem_img_rxbuffer+0x9e>
300079fc:	3804      	subs	r0, #4
300079fe:	2804      	cmp	r0, #4
30007a00:	d80c      	bhi.n	30007a1c <ymodem_img_rxbuffer+0x74>
30007a02:	a301      	add	r3, pc, #4	; (adr r3, 30007a08 <ymodem_img_rxbuffer+0x60>)
30007a04:	f853 f020 	ldr.w	pc, [r3, r0, lsl #2]
30007a08:	300079dd 	.word	0x300079dd
30007a0c:	30007a29 	.word	0x30007a29
30007a10:	30007a1d 	.word	0x30007a1d
30007a14:	30007a1d 	.word	0x30007a1d
30007a18:	30007a35 	.word	0x30007a35
30007a1c:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a20:	47b0      	blx	r6
30007a22:	2018      	movs	r0, #24
30007a24:	f000 f882 	bl	30007b2c <ymodem_uart_putc>
30007a28:	200a      	movs	r0, #10
30007a2a:	f000 f8c9 	bl	30007bc0 <ymodem_uart_waitbusy>
30007a2e:	2001      	movs	r0, #1
30007a30:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30007a34:	f000 f8b4 	bl	30007ba0 <ymodem_uart_clean_rx>
30007a38:	f44f 707a 	mov.w	r0, #1000	; 0x3e8
30007a3c:	47b0      	blx	r6
30007a3e:	2015      	movs	r0, #21
30007a40:	f000 f874 	bl	30007b2c <ymodem_uart_putc>
30007a44:	e7ca      	b.n	300079dc <ymodem_img_rxbuffer+0x34>
30007a46:	68a3      	ldr	r3, [r4, #8]
30007a48:	3301      	adds	r3, #1
30007a4a:	b2db      	uxtb	r3, r3
30007a4c:	60a3      	str	r3, [r4, #8]
30007a4e:	e7c5      	b.n	300079dc <ymodem_img_rxbuffer+0x34>
30007a50:	3000ee20 	.word	0x3000ee20
30007a54:	00009b2d 	.word	0x00009b2d
30007a58:	3000ee28 	.word	0x3000ee28

30007a5c <ymodem_log_open>:
30007a5c:	b510      	push	{r4, lr}
30007a5e:	2303      	movs	r3, #3
30007a60:	4c04      	ldr	r4, [pc, #16]	; (30007a74 <ymodem_log_open+0x18>)
30007a62:	2200      	movs	r2, #0
30007a64:	2108      	movs	r1, #8
30007a66:	4804      	ldr	r0, [pc, #16]	; (30007a78 <ymodem_log_open+0x1c>)
30007a68:	47a0      	blx	r4
30007a6a:	2008      	movs	r0, #8
30007a6c:	4b03      	ldr	r3, [pc, #12]	; (30007a7c <ymodem_log_open+0x20>)
30007a6e:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007a72:	4718      	bx	r3
30007a74:	30009b35 	.word	0x30009b35
30007a78:	0000ed39 	.word	0x0000ed39
30007a7c:	0000e1bd 	.word	0x0000e1bd

30007a80 <ymodem_uart_port_init>:
30007a80:	b500      	push	{lr}
30007a82:	b083      	sub	sp, #12
30007a84:	2008      	movs	r0, #8
30007a86:	4b06      	ldr	r3, [pc, #24]	; (30007aa0 <ymodem_uart_port_init+0x20>)
30007a88:	9201      	str	r2, [sp, #4]
30007a8a:	4798      	blx	r3
30007a8c:	4b05      	ldr	r3, [pc, #20]	; (30007aa4 <ymodem_uart_port_init+0x24>)
30007a8e:	2008      	movs	r0, #8
30007a90:	4798      	blx	r3
30007a92:	9901      	ldr	r1, [sp, #4]
30007a94:	4804      	ldr	r0, [pc, #16]	; (30007aa8 <ymodem_uart_port_init+0x28>)
30007a96:	4b05      	ldr	r3, [pc, #20]	; (30007aac <ymodem_uart_port_init+0x2c>)
30007a98:	b003      	add	sp, #12
30007a9a:	f85d eb04 	ldr.w	lr, [sp], #4
30007a9e:	4718      	bx	r3
30007aa0:	0000e1d9 	.word	0x0000e1d9
30007aa4:	0000e2fd 	.word	0x0000e2fd
30007aa8:	4200c000 	.word	0x4200c000
30007aac:	0000a9e5 	.word	0x0000a9e5

30007ab0 <ymodem_uart_port_deinit>:
30007ab0:	2804      	cmp	r0, #4
30007ab2:	d004      	beq.n	30007abe <ymodem_uart_port_deinit+0xe>
30007ab4:	4b0d      	ldr	r3, [pc, #52]	; (30007aec <ymodem_uart_port_deinit+0x3c>)
30007ab6:	681b      	ldr	r3, [r3, #0]
30007ab8:	039b      	lsls	r3, r3, #14
30007aba:	d40f      	bmi.n	30007adc <ymodem_uart_port_deinit+0x2c>
30007abc:	4770      	bx	lr
30007abe:	2200      	movs	r2, #0
30007ac0:	2303      	movs	r3, #3
30007ac2:	2108      	movs	r1, #8
30007ac4:	480a      	ldr	r0, [pc, #40]	; (30007af0 <ymodem_uart_port_deinit+0x40>)
30007ac6:	b510      	push	{r4, lr}
30007ac8:	4c0a      	ldr	r4, [pc, #40]	; (30007af4 <ymodem_uart_port_deinit+0x44>)
30007aca:	47a0      	blx	r4
30007acc:	4b0a      	ldr	r3, [pc, #40]	; (30007af8 <ymodem_uart_port_deinit+0x48>)
30007ace:	2008      	movs	r0, #8
30007ad0:	4798      	blx	r3
30007ad2:	4b06      	ldr	r3, [pc, #24]	; (30007aec <ymodem_uart_port_deinit+0x3c>)
30007ad4:	681b      	ldr	r3, [r3, #0]
30007ad6:	039a      	lsls	r2, r3, #14
30007ad8:	d403      	bmi.n	30007ae2 <ymodem_uart_port_deinit+0x32>
30007ada:	bd10      	pop	{r4, pc}
30007adc:	4807      	ldr	r0, [pc, #28]	; (30007afc <ymodem_uart_port_deinit+0x4c>)
30007ade:	f004 bc7b 	b.w	3000c3d8 <__DiagPrintf_veneer>
30007ae2:	4806      	ldr	r0, [pc, #24]	; (30007afc <ymodem_uart_port_deinit+0x4c>)
30007ae4:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
30007ae8:	f004 bc76 	b.w	3000c3d8 <__DiagPrintf_veneer>
30007aec:	2001c00c 	.word	0x2001c00c
30007af0:	0000ed39 	.word	0x0000ed39
30007af4:	30009b35 	.word	0x30009b35
30007af8:	0000e1bd 	.word	0x0000e1bd
30007afc:	3000d1a0 	.word	0x3000d1a0

30007b00 <ymodem_uart_readable>:
30007b00:	4b00      	ldr	r3, [pc, #0]	; (30007b04 <ymodem_uart_readable+0x4>)
30007b02:	4718      	bx	r3
30007b04:	0000ac39 	.word	0x0000ac39

30007b08 <ymodem_uart_writable>:
30007b08:	4a03      	ldr	r2, [pc, #12]	; (30007b18 <ymodem_uart_writable+0x10>)
30007b0a:	b508      	push	{r3, lr}
30007b0c:	4b03      	ldr	r3, [pc, #12]	; (30007b1c <ymodem_uart_writable+0x14>)
30007b0e:	6810      	ldr	r0, [r2, #0]
30007b10:	4798      	blx	r3
30007b12:	b2c0      	uxtb	r0, r0
30007b14:	bd08      	pop	{r3, pc}
30007b16:	bf00      	nop
30007b18:	3000ee38 	.word	0x3000ee38
30007b1c:	0000d1a9 	.word	0x0000d1a9

30007b20 <ymodem_uart_getc>:
30007b20:	2000      	movs	r0, #0
30007b22:	4b01      	ldr	r3, [pc, #4]	; (30007b28 <ymodem_uart_getc+0x8>)
30007b24:	4718      	bx	r3
30007b26:	bf00      	nop
30007b28:	0000ac1d 	.word	0x0000ac1d

30007b2c <ymodem_uart_putc>:
30007b2c:	4b00      	ldr	r3, [pc, #0]	; (30007b30 <ymodem_uart_putc+0x4>)
30007b2e:	4718      	bx	r3
30007b30:	0000abb5 	.word	0x0000abb5

30007b34 <ymodem_uart_putdata>:
30007b34:	b149      	cbz	r1, 30007b4a <ymodem_uart_putdata+0x16>
30007b36:	b570      	push	{r4, r5, r6, lr}
30007b38:	4604      	mov	r4, r0
30007b3a:	1845      	adds	r5, r0, r1
30007b3c:	4e03      	ldr	r6, [pc, #12]	; (30007b4c <ymodem_uart_putdata+0x18>)
30007b3e:	f814 0b01 	ldrb.w	r0, [r4], #1
30007b42:	47b0      	blx	r6
30007b44:	42ac      	cmp	r4, r5
30007b46:	d1fa      	bne.n	30007b3e <ymodem_uart_putdata+0xa>
30007b48:	bd70      	pop	{r4, r5, r6, pc}
30007b4a:	4770      	bx	lr
30007b4c:	0000abb5 	.word	0x0000abb5

30007b50 <ymodem_uart_getdata>:
30007b50:	b151      	cbz	r1, 30007b68 <ymodem_uart_getdata+0x18>
30007b52:	b570      	push	{r4, r5, r6, lr}
30007b54:	4604      	mov	r4, r0
30007b56:	1845      	adds	r5, r0, r1
30007b58:	4e04      	ldr	r6, [pc, #16]	; (30007b6c <ymodem_uart_getdata+0x1c>)
30007b5a:	2001      	movs	r0, #1
30007b5c:	47b0      	blx	r6
30007b5e:	f804 0b01 	strb.w	r0, [r4], #1
30007b62:	42ac      	cmp	r4, r5
30007b64:	d1f9      	bne.n	30007b5a <ymodem_uart_getdata+0xa>
30007b66:	bd70      	pop	{r4, r5, r6, pc}
30007b68:	4770      	bx	lr
30007b6a:	bf00      	nop
30007b6c:	0000ac1d 	.word	0x0000ac1d

30007b70 <ymodem_uart_getc_to>:
30007b70:	b570      	push	{r4, r5, r6, lr}
30007b72:	4d09      	ldr	r5, [pc, #36]	; (30007b98 <ymodem_uart_getc_to+0x28>)
30007b74:	4606      	mov	r6, r0
30007b76:	460c      	mov	r4, r1
30007b78:	e001      	b.n	30007b7e <ymodem_uart_getc_to+0xe>
30007b7a:	3c01      	subs	r4, #1
30007b7c:	d009      	beq.n	30007b92 <ymodem_uart_getc_to+0x22>
30007b7e:	47a8      	blx	r5
30007b80:	2800      	cmp	r0, #0
30007b82:	d0fa      	beq.n	30007b7a <ymodem_uart_getc_to+0xa>
30007b84:	4b05      	ldr	r3, [pc, #20]	; (30007b9c <ymodem_uart_getc_to+0x2c>)
30007b86:	2000      	movs	r0, #0
30007b88:	4798      	blx	r3
30007b8a:	4603      	mov	r3, r0
30007b8c:	2000      	movs	r0, #0
30007b8e:	7033      	strb	r3, [r6, #0]
30007b90:	bd70      	pop	{r4, r5, r6, pc}
30007b92:	2001      	movs	r0, #1
30007b94:	bd70      	pop	{r4, r5, r6, pc}
30007b96:	bf00      	nop
30007b98:	0000ac39 	.word	0x0000ac39
30007b9c:	0000ac1d 	.word	0x0000ac1d

30007ba0 <ymodem_uart_clean_rx>:
30007ba0:	b538      	push	{r3, r4, r5, lr}
30007ba2:	4c05      	ldr	r4, [pc, #20]	; (30007bb8 <ymodem_uart_clean_rx+0x18>)
30007ba4:	4d05      	ldr	r5, [pc, #20]	; (30007bbc <ymodem_uart_clean_rx+0x1c>)
30007ba6:	e000      	b.n	30007baa <ymodem_uart_clean_rx+0xa>
30007ba8:	47a8      	blx	r5
30007baa:	47a0      	blx	r4
30007bac:	4603      	mov	r3, r0
30007bae:	2000      	movs	r0, #0
30007bb0:	2b00      	cmp	r3, #0
30007bb2:	d1f9      	bne.n	30007ba8 <ymodem_uart_clean_rx+0x8>
30007bb4:	bd38      	pop	{r3, r4, r5, pc}
30007bb6:	bf00      	nop
30007bb8:	0000ac39 	.word	0x0000ac39
30007bbc:	0000ac1d 	.word	0x0000ac1d

30007bc0 <ymodem_uart_waitbusy>:
30007bc0:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007bc2:	2400      	movs	r4, #0
30007bc4:	4e06      	ldr	r6, [pc, #24]	; (30007be0 <ymodem_uart_waitbusy+0x20>)
30007bc6:	4f07      	ldr	r7, [pc, #28]	; (30007be4 <ymodem_uart_waitbusy+0x24>)
30007bc8:	4605      	mov	r5, r0
30007bca:	e002      	b.n	30007bd2 <ymodem_uart_waitbusy+0x12>
30007bcc:	47b8      	blx	r7
30007bce:	42ac      	cmp	r4, r5
30007bd0:	d804      	bhi.n	30007bdc <ymodem_uart_waitbusy+0x1c>
30007bd2:	6973      	ldr	r3, [r6, #20]
30007bd4:	3401      	adds	r4, #1
30007bd6:	2064      	movs	r0, #100	; 0x64
30007bd8:	03db      	lsls	r3, r3, #15
30007bda:	d5f7      	bpl.n	30007bcc <ymodem_uart_waitbusy+0xc>
30007bdc:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
30007bde:	bf00      	nop
30007be0:	4200c000 	.word	0x4200c000
30007be4:	00009b2d 	.word	0x00009b2d

30007be8 <UARTIMG_Write>:
30007be8:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
30007bec:	f101 4478 	add.w	r4, r1, #4160749568	; 0xf8000000
30007bf0:	f1b4 6f00 	cmp.w	r4, #134217728	; 0x8000000
30007bf4:	d21d      	bcs.n	30007c32 <UARTIMG_Write+0x4a>
30007bf6:	3a01      	subs	r2, #1
30007bf8:	4f19      	ldr	r7, [pc, #100]	; (30007c60 <UARTIMG_Write+0x78>)
30007bfa:	f8df 9074 	ldr.w	r9, [pc, #116]	; 30007c70 <UARTIMG_Write+0x88>
30007bfe:	4688      	mov	r8, r1
30007c00:	1916      	adds	r6, r2, r4
30007c02:	4605      	mov	r5, r0
30007c04:	f8d9 3000 	ldr.w	r3, [r9]
30007c08:	403e      	ands	r6, r7
30007c0a:	4027      	ands	r7, r4
30007c0c:	42bb      	cmp	r3, r7
30007c0e:	d91b      	bls.n	30007c48 <UARTIMG_Write+0x60>
30007c10:	42b3      	cmp	r3, r6
30007c12:	d910      	bls.n	30007c36 <UARTIMG_Write+0x4e>
30007c14:	4e13      	ldr	r6, [pc, #76]	; (30007c64 <UARTIMG_Write+0x7c>)
30007c16:	f105 6300 	add.w	r3, r5, #134217728	; 0x8000000
30007c1a:	4f13      	ldr	r7, [pc, #76]	; (30007c68 <UARTIMG_Write+0x80>)
30007c1c:	4446      	add	r6, r8
30007c1e:	eba3 0808 	sub.w	r8, r3, r8
30007c22:	eb08 0204 	add.w	r2, r8, r4
30007c26:	4620      	mov	r0, r4
30007c28:	3408      	adds	r4, #8
30007c2a:	2108      	movs	r1, #8
30007c2c:	47b8      	blx	r7
30007c2e:	42b4      	cmp	r4, r6
30007c30:	d1f7      	bne.n	30007c22 <UARTIMG_Write+0x3a>
30007c32:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30007c36:	4631      	mov	r1, r6
30007c38:	f506 5680 	add.w	r6, r6, #4096	; 0x1000
30007c3c:	2002      	movs	r0, #2
30007c3e:	4b0b      	ldr	r3, [pc, #44]	; (30007c6c <UARTIMG_Write+0x84>)
30007c40:	4798      	blx	r3
30007c42:	f8c9 6000 	str.w	r6, [r9]
30007c46:	e7e5      	b.n	30007c14 <UARTIMG_Write+0x2c>
30007c48:	4b08      	ldr	r3, [pc, #32]	; (30007c6c <UARTIMG_Write+0x84>)
30007c4a:	4639      	mov	r1, r7
30007c4c:	2002      	movs	r0, #2
30007c4e:	4798      	blx	r3
30007c50:	f507 5380 	add.w	r3, r7, #4096	; 0x1000
30007c54:	42b3      	cmp	r3, r6
30007c56:	f8c9 3000 	str.w	r3, [r9]
30007c5a:	d8db      	bhi.n	30007c14 <UARTIMG_Write+0x2c>
30007c5c:	e7eb      	b.n	30007c36 <UARTIMG_Write+0x4e>
30007c5e:	bf00      	nop
30007c60:	fffff000 	.word	0xfffff000
30007c64:	f8000400 	.word	0xf8000400
30007c68:	0000a04d 	.word	0x0000a04d
30007c6c:	0000a16d 	.word	0x0000a16d
30007c70:	3000ee3c 	.word	0x3000ee3c

30007c74 <ymodem_img_download>:
30007c74:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30007c78:	4616      	mov	r6, r2
30007c7a:	460d      	mov	r5, r1
30007c7c:	4a0a      	ldr	r2, [pc, #40]	; (30007ca8 <ymodem_img_download+0x34>)
30007c7e:	2100      	movs	r1, #0
30007c80:	4680      	mov	r8, r0
30007c82:	f7ff fefd 	bl	30007a80 <ymodem_uart_port_init>
30007c86:	b156      	cbz	r6, 30007c9e <ymodem_img_download+0x2a>
30007c88:	3d04      	subs	r5, #4
30007c8a:	2400      	movs	r4, #0
30007c8c:	4f06      	ldr	r7, [pc, #24]	; (30007ca8 <ymodem_img_download+0x34>)
30007c8e:	3401      	adds	r4, #1
30007c90:	f855 1f04 	ldr.w	r1, [r5, #4]!
30007c94:	4638      	mov	r0, r7
30007c96:	f7ff fe87 	bl	300079a8 <ymodem_img_rxbuffer>
30007c9a:	42b4      	cmp	r4, r6
30007c9c:	d1f7      	bne.n	30007c8e <ymodem_img_download+0x1a>
30007c9e:	4640      	mov	r0, r8
30007ca0:	e8bd 41f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, lr}
30007ca4:	f7ff bf04 	b.w	30007ab0 <ymodem_uart_port_deinit>
30007ca8:	0016e360 	.word	0x0016e360

30007cac <ChipInfo_Invalid>:
30007cac:	4e06      	ldr	r6, [pc, #24]	; (30007cc8 <ChipInfo_Invalid+0x1c>)
30007cae:	4d07      	ldr	r5, [pc, #28]	; (30007ccc <ChipInfo_Invalid+0x20>)
30007cb0:	4c07      	ldr	r4, [pc, #28]	; (30007cd0 <ChipInfo_Invalid+0x24>)
30007cb2:	b508      	push	{r3, lr}
30007cb4:	462b      	mov	r3, r5
30007cb6:	4622      	mov	r2, r4
30007cb8:	4904      	ldr	r1, [pc, #16]	; (30007ccc <ChipInfo_Invalid+0x20>)
30007cba:	2002      	movs	r0, #2
30007cbc:	f004 f916 	bl	3000beec <rtk_log_write>
30007cc0:	f242 7010 	movw	r0, #10000	; 0x2710
30007cc4:	47b0      	blx	r6
30007cc6:	e7f5      	b.n	30007cb4 <ChipInfo_Invalid+0x8>
30007cc8:	00009be5 	.word	0x00009be5
30007ccc:	3000d1c0 	.word	0x3000d1c0
30007cd0:	3000d1cc 	.word	0x3000d1cc

30007cd4 <ChipInfo_Get>:
30007cd4:	b570      	push	{r4, r5, r6, lr}
30007cd6:	4c11      	ldr	r4, [pc, #68]	; (30007d1c <ChipInfo_Get+0x48>)
30007cd8:	7820      	ldrb	r0, [r4, #0]
30007cda:	28ff      	cmp	r0, #255	; 0xff
30007cdc:	d104      	bne.n	30007ce8 <ChipInfo_Get+0x14>
30007cde:	4d10      	ldr	r5, [pc, #64]	; (30007d20 <ChipInfo_Get+0x4c>)
30007ce0:	f895 010f 	ldrb.w	r0, [r5, #271]	; 0x10f
30007ce4:	b108      	cbz	r0, 30007cea <ChipInfo_Get+0x16>
30007ce6:	7020      	strb	r0, [r4, #0]
30007ce8:	bd70      	pop	{r4, r5, r6, pc}
30007cea:	4621      	mov	r1, r4
30007cec:	f240 70ff 	movw	r0, #2047	; 0x7ff
30007cf0:	f000 fec4 	bl	30008a7c <OTP_Read8>
30007cf4:	7820      	ldrb	r0, [r4, #0]
30007cf6:	28ff      	cmp	r0, #255	; 0xff
30007cf8:	d002      	beq.n	30007d00 <ChipInfo_Get+0x2c>
30007cfa:	f885 010f 	strb.w	r0, [r5, #271]	; 0x10f
30007cfe:	bd70      	pop	{r4, r5, r6, pc}
30007d00:	4e08      	ldr	r6, [pc, #32]	; (30007d24 <ChipInfo_Get+0x50>)
30007d02:	4d09      	ldr	r5, [pc, #36]	; (30007d28 <ChipInfo_Get+0x54>)
30007d04:	4c09      	ldr	r4, [pc, #36]	; (30007d2c <ChipInfo_Get+0x58>)
30007d06:	462b      	mov	r3, r5
30007d08:	4622      	mov	r2, r4
30007d0a:	4907      	ldr	r1, [pc, #28]	; (30007d28 <ChipInfo_Get+0x54>)
30007d0c:	2003      	movs	r0, #3
30007d0e:	f004 f8ed 	bl	3000beec <rtk_log_write>
30007d12:	f242 7010 	movw	r0, #10000	; 0x2710
30007d16:	47b0      	blx	r6
30007d18:	e7f5      	b.n	30007d06 <ChipInfo_Get+0x32>
30007d1a:	bf00      	nop
30007d1c:	3000ef38 	.word	0x3000ef38
30007d20:	23020000 	.word	0x23020000
30007d24:	00009be5 	.word	0x00009be5
30007d28:	3000d1c0 	.word	0x3000d1c0
30007d2c:	3000d1f0 	.word	0x3000d1f0

30007d30 <ChipInfo_MemoryType>:
30007d30:	b510      	push	{r4, lr}
30007d32:	4c11      	ldr	r4, [pc, #68]	; (30007d78 <ChipInfo_MemoryType+0x48>)
30007d34:	47a0      	blx	r4
30007d36:	2802      	cmp	r0, #2
30007d38:	d019      	beq.n	30007d6e <ChipInfo_MemoryType+0x3e>
30007d3a:	47a0      	blx	r4
30007d3c:	2803      	cmp	r0, #3
30007d3e:	d016      	beq.n	30007d6e <ChipInfo_MemoryType+0x3e>
30007d40:	4c0e      	ldr	r4, [pc, #56]	; (30007d7c <ChipInfo_MemoryType+0x4c>)
30007d42:	4b0f      	ldr	r3, [pc, #60]	; (30007d80 <ChipInfo_MemoryType+0x50>)
30007d44:	4798      	blx	r3
30007d46:	2100      	movs	r1, #0
30007d48:	23e0      	movs	r3, #224	; 0xe0
30007d4a:	4622      	mov	r2, r4
30007d4c:	e004      	b.n	30007d58 <ChipInfo_MemoryType+0x28>
30007d4e:	7a13      	ldrb	r3, [r2, #8]
30007d50:	3101      	adds	r1, #1
30007d52:	3206      	adds	r2, #6
30007d54:	2bff      	cmp	r3, #255	; 0xff
30007d56:	d00c      	beq.n	30007d72 <ChipInfo_MemoryType+0x42>
30007d58:	4298      	cmp	r0, r3
30007d5a:	d1f8      	bne.n	30007d4e <ChipInfo_MemoryType+0x1e>
30007d5c:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007d60:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007d64:	78cb      	ldrb	r3, [r1, #3]
30007d66:	2b02      	cmp	r3, #2
30007d68:	d001      	beq.n	30007d6e <ChipInfo_MemoryType+0x3e>
30007d6a:	2001      	movs	r0, #1
30007d6c:	bd10      	pop	{r4, pc}
30007d6e:	2002      	movs	r0, #2
30007d70:	bd10      	pop	{r4, pc}
30007d72:	f7ff ff9b 	bl	30007cac <ChipInfo_Invalid>
30007d76:	bf00      	nop
30007d78:	0000c0f9 	.word	0x0000c0f9
30007d7c:	3000d21c 	.word	0x3000d21c
30007d80:	30007cd5 	.word	0x30007cd5

30007d84 <ChipInfo_DDRType>:
30007d84:	b510      	push	{r4, lr}
30007d86:	4b0f      	ldr	r3, [pc, #60]	; (30007dc4 <ChipInfo_DDRType+0x40>)
30007d88:	4798      	blx	r3
30007d8a:	4b0f      	ldr	r3, [pc, #60]	; (30007dc8 <ChipInfo_DDRType+0x44>)
30007d8c:	4604      	mov	r4, r0
30007d8e:	4798      	blx	r3
30007d90:	2801      	cmp	r0, #1
30007d92:	d112      	bne.n	30007dba <ChipInfo_DDRType+0x36>
30007d94:	480d      	ldr	r0, [pc, #52]	; (30007dcc <ChipInfo_DDRType+0x48>)
30007d96:	2100      	movs	r1, #0
30007d98:	22e0      	movs	r2, #224	; 0xe0
30007d9a:	4603      	mov	r3, r0
30007d9c:	e003      	b.n	30007da6 <ChipInfo_DDRType+0x22>
30007d9e:	789a      	ldrb	r2, [r3, #2]
30007da0:	3101      	adds	r1, #1
30007da2:	2aff      	cmp	r2, #255	; 0xff
30007da4:	d00b      	beq.n	30007dbe <ChipInfo_DDRType+0x3a>
30007da6:	4294      	cmp	r4, r2
30007da8:	f103 0306 	add.w	r3, r3, #6
30007dac:	d1f7      	bne.n	30007d9e <ChipInfo_DDRType+0x1a>
30007dae:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007db2:	eb00 0141 	add.w	r1, r0, r1, lsl #1
30007db6:	78c8      	ldrb	r0, [r1, #3]
30007db8:	bd10      	pop	{r4, pc}
30007dba:	20ff      	movs	r0, #255	; 0xff
30007dbc:	bd10      	pop	{r4, pc}
30007dbe:	2000      	movs	r0, #0
30007dc0:	bd10      	pop	{r4, pc}
30007dc2:	bf00      	nop
30007dc4:	30007cd5 	.word	0x30007cd5
30007dc8:	30007d31 	.word	0x30007d31
30007dcc:	3000d21c 	.word	0x3000d21c

30007dd0 <ChipInfo_MemorySize>:
30007dd0:	b510      	push	{r4, lr}
30007dd2:	4c0b      	ldr	r4, [pc, #44]	; (30007e00 <ChipInfo_MemorySize+0x30>)
30007dd4:	4b0b      	ldr	r3, [pc, #44]	; (30007e04 <ChipInfo_MemorySize+0x34>)
30007dd6:	4798      	blx	r3
30007dd8:	2100      	movs	r1, #0
30007dda:	23e0      	movs	r3, #224	; 0xe0
30007ddc:	4622      	mov	r2, r4
30007dde:	e004      	b.n	30007dea <ChipInfo_MemorySize+0x1a>
30007de0:	7a13      	ldrb	r3, [r2, #8]
30007de2:	3101      	adds	r1, #1
30007de4:	3206      	adds	r2, #6
30007de6:	2bff      	cmp	r3, #255	; 0xff
30007de8:	d007      	beq.n	30007dfa <ChipInfo_MemorySize+0x2a>
30007dea:	4298      	cmp	r0, r3
30007dec:	d1f8      	bne.n	30007de0 <ChipInfo_MemorySize+0x10>
30007dee:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007df2:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007df6:	7908      	ldrb	r0, [r1, #4]
30007df8:	bd10      	pop	{r4, pc}
30007dfa:	f7ff ff57 	bl	30007cac <ChipInfo_Invalid>
30007dfe:	bf00      	nop
30007e00:	3000d21c 	.word	0x3000d21c
30007e04:	30007cd5 	.word	0x30007cd5

30007e08 <ChipInfo_MemoryVendor>:
30007e08:	b510      	push	{r4, lr}
30007e0a:	4c0f      	ldr	r4, [pc, #60]	; (30007e48 <ChipInfo_MemoryVendor+0x40>)
30007e0c:	47a0      	blx	r4
30007e0e:	2803      	cmp	r0, #3
30007e10:	d016      	beq.n	30007e40 <ChipInfo_MemoryVendor+0x38>
30007e12:	47a0      	blx	r4
30007e14:	2802      	cmp	r0, #2
30007e16:	d012      	beq.n	30007e3e <ChipInfo_MemoryVendor+0x36>
30007e18:	4c0c      	ldr	r4, [pc, #48]	; (30007e4c <ChipInfo_MemoryVendor+0x44>)
30007e1a:	4b0d      	ldr	r3, [pc, #52]	; (30007e50 <ChipInfo_MemoryVendor+0x48>)
30007e1c:	4798      	blx	r3
30007e1e:	2100      	movs	r1, #0
30007e20:	23e0      	movs	r3, #224	; 0xe0
30007e22:	4622      	mov	r2, r4
30007e24:	e004      	b.n	30007e30 <ChipInfo_MemoryVendor+0x28>
30007e26:	7a13      	ldrb	r3, [r2, #8]
30007e28:	3101      	adds	r1, #1
30007e2a:	3206      	adds	r2, #6
30007e2c:	2bff      	cmp	r3, #255	; 0xff
30007e2e:	d009      	beq.n	30007e44 <ChipInfo_MemoryVendor+0x3c>
30007e30:	4298      	cmp	r0, r3
30007e32:	d1f8      	bne.n	30007e26 <ChipInfo_MemoryVendor+0x1e>
30007e34:	eb01 0141 	add.w	r1, r1, r1, lsl #1
30007e38:	eb04 0141 	add.w	r1, r4, r1, lsl #1
30007e3c:	7948      	ldrb	r0, [r1, #5]
30007e3e:	bd10      	pop	{r4, pc}
30007e40:	2001      	movs	r0, #1
30007e42:	bd10      	pop	{r4, pc}
30007e44:	f7ff ff32 	bl	30007cac <ChipInfo_Invalid>
30007e48:	0000c0f9 	.word	0x0000c0f9
30007e4c:	3000d21c 	.word	0x3000d21c
30007e50:	30007cd5 	.word	0x30007cd5

30007e54 <SDM32K_Enable>:
30007e54:	4a02      	ldr	r2, [pc, #8]	; (30007e60 <SDM32K_Enable+0xc>)
30007e56:	4b03      	ldr	r3, [pc, #12]	; (30007e64 <SDM32K_Enable+0x10>)
30007e58:	6811      	ldr	r1, [r2, #0]
30007e5a:	430b      	orrs	r3, r1
30007e5c:	6013      	str	r3, [r2, #0]
30007e5e:	4770      	bx	lr
30007e60:	42008e00 	.word	0x42008e00
30007e64:	c0060000 	.word	0xc0060000

30007e68 <CLK_SWITCH_XTAL>:
30007e68:	b508      	push	{r3, lr}
30007e6a:	2801      	cmp	r0, #1
30007e6c:	4b0a      	ldr	r3, [pc, #40]	; (30007e98 <CLK_SWITCH_XTAL+0x30>)
30007e6e:	f8d3 2224 	ldr.w	r2, [r3, #548]	; 0x224
30007e72:	d00d      	beq.n	30007e90 <CLK_SWITCH_XTAL+0x28>
30007e74:	4b09      	ldr	r3, [pc, #36]	; (30007e9c <CLK_SWITCH_XTAL+0x34>)
30007e76:	4313      	orrs	r3, r2
30007e78:	4907      	ldr	r1, [pc, #28]	; (30007e98 <CLK_SWITCH_XTAL+0x30>)
30007e7a:	4a09      	ldr	r2, [pc, #36]	; (30007ea0 <CLK_SWITCH_XTAL+0x38>)
30007e7c:	f8c1 3224 	str.w	r3, [r1, #548]	; 0x224
30007e80:	4790      	blx	r2
30007e82:	4a08      	ldr	r2, [pc, #32]	; (30007ea4 <CLK_SWITCH_XTAL+0x3c>)
30007e84:	4b08      	ldr	r3, [pc, #32]	; (30007ea8 <CLK_SWITCH_XTAL+0x40>)
30007e86:	fba2 2000 	umull	r2, r0, r2, r0
30007e8a:	0c80      	lsrs	r0, r0, #18
30007e8c:	6018      	str	r0, [r3, #0]
30007e8e:	bd08      	pop	{r3, pc}
30007e90:	4b06      	ldr	r3, [pc, #24]	; (30007eac <CLK_SWITCH_XTAL+0x44>)
30007e92:	4013      	ands	r3, r2
30007e94:	e7f0      	b.n	30007e78 <CLK_SWITCH_XTAL+0x10>
30007e96:	bf00      	nop
30007e98:	42008000 	.word	0x42008000
30007e9c:	80000500 	.word	0x80000500
30007ea0:	30009805 	.word	0x30009805
30007ea4:	431bde83 	.word	0x431bde83
30007ea8:	2001c700 	.word	0x2001c700
30007eac:	7ffffaff 	.word	0x7ffffaff

30007eb0 <DDR_PHY_ChipInfo.part.0>:
30007eb0:	b508      	push	{r3, lr}
30007eb2:	4b24      	ldr	r3, [pc, #144]	; (30007f44 <DDR_PHY_ChipInfo.part.0+0x94>)
30007eb4:	4798      	blx	r3
30007eb6:	280e      	cmp	r0, #14
30007eb8:	d80d      	bhi.n	30007ed6 <DDR_PHY_ChipInfo.part.0+0x26>
30007eba:	2801      	cmp	r0, #1
30007ebc:	d912      	bls.n	30007ee4 <DDR_PHY_ChipInfo.part.0+0x34>
30007ebe:	1e83      	subs	r3, r0, #2
30007ec0:	2b0c      	cmp	r3, #12
30007ec2:	d80f      	bhi.n	30007ee4 <DDR_PHY_ChipInfo.part.0+0x34>
30007ec4:	e8df f003 	tbb	[pc, r3]
30007ec8:	210e1d19 	.word	0x210e1d19
30007ecc:	2a0e260e 	.word	0x2a0e260e
30007ed0:	340e0e2f 	.word	0x340e0e2f
30007ed4:	39          	.byte	0x39
30007ed5:	00          	.byte	0x00
30007ed6:	28e0      	cmp	r0, #224	; 0xe0
30007ed8:	d104      	bne.n	30007ee4 <DDR_PHY_ChipInfo.part.0+0x34>
30007eda:	2301      	movs	r3, #1
30007edc:	4a1a      	ldr	r2, [pc, #104]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007ede:	4618      	mov	r0, r3
30007ee0:	7013      	strb	r3, [r2, #0]
30007ee2:	bd08      	pop	{r3, pc}
30007ee4:	4b19      	ldr	r3, [pc, #100]	; (30007f4c <DDR_PHY_ChipInfo.part.0+0x9c>)
30007ee6:	2002      	movs	r0, #2
30007ee8:	4a19      	ldr	r2, [pc, #100]	; (30007f50 <DDR_PHY_ChipInfo.part.0+0xa0>)
30007eea:	4619      	mov	r1, r3
30007eec:	f003 fffe 	bl	3000beec <rtk_log_write>
30007ef0:	2300      	movs	r3, #0
30007ef2:	4a15      	ldr	r2, [pc, #84]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007ef4:	4618      	mov	r0, r3
30007ef6:	7013      	strb	r3, [r2, #0]
30007ef8:	bd08      	pop	{r3, pc}
30007efa:	4b13      	ldr	r3, [pc, #76]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007efc:	2202      	movs	r2, #2
30007efe:	701a      	strb	r2, [r3, #0]
30007f00:	bd08      	pop	{r3, pc}
30007f02:	4b11      	ldr	r3, [pc, #68]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f04:	2203      	movs	r2, #3
30007f06:	701a      	strb	r2, [r3, #0]
30007f08:	bd08      	pop	{r3, pc}
30007f0a:	2304      	movs	r3, #4
30007f0c:	4a0e      	ldr	r2, [pc, #56]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f0e:	4618      	mov	r0, r3
30007f10:	7013      	strb	r3, [r2, #0]
30007f12:	bd08      	pop	{r3, pc}
30007f14:	4b0c      	ldr	r3, [pc, #48]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f16:	2207      	movs	r2, #7
30007f18:	701a      	strb	r2, [r3, #0]
30007f1a:	bd08      	pop	{r3, pc}
30007f1c:	2305      	movs	r3, #5
30007f1e:	4a0a      	ldr	r2, [pc, #40]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f20:	4618      	mov	r0, r3
30007f22:	7013      	strb	r3, [r2, #0]
30007f24:	bd08      	pop	{r3, pc}
30007f26:	2306      	movs	r3, #6
30007f28:	4a07      	ldr	r2, [pc, #28]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f2a:	4618      	mov	r0, r3
30007f2c:	7013      	strb	r3, [r2, #0]
30007f2e:	bd08      	pop	{r3, pc}
30007f30:	2308      	movs	r3, #8
30007f32:	4a05      	ldr	r2, [pc, #20]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f34:	4618      	mov	r0, r3
30007f36:	7013      	strb	r3, [r2, #0]
30007f38:	bd08      	pop	{r3, pc}
30007f3a:	2309      	movs	r3, #9
30007f3c:	4a02      	ldr	r2, [pc, #8]	; (30007f48 <DDR_PHY_ChipInfo.part.0+0x98>)
30007f3e:	4618      	mov	r0, r3
30007f40:	7013      	strb	r3, [r2, #0]
30007f42:	bd08      	pop	{r3, pc}
30007f44:	30007cd5 	.word	0x30007cd5
30007f48:	3000ef39 	.word	0x3000ef39
30007f4c:	3000d2a0 	.word	0x3000d2a0
30007f50:	3000d2a8 	.word	0x3000d2a8

30007f54 <DDR_PHY_ChipInfo_ddrtype>:
30007f54:	b510      	push	{r4, lr}
30007f56:	4c04      	ldr	r4, [pc, #16]	; (30007f68 <DDR_PHY_ChipInfo_ddrtype+0x14>)
30007f58:	7820      	ldrb	r0, [r4, #0]
30007f5a:	b100      	cbz	r0, 30007f5e <DDR_PHY_ChipInfo_ddrtype+0xa>
30007f5c:	bd10      	pop	{r4, pc}
30007f5e:	4b03      	ldr	r3, [pc, #12]	; (30007f6c <DDR_PHY_ChipInfo_ddrtype+0x18>)
30007f60:	4798      	blx	r3
30007f62:	7020      	strb	r0, [r4, #0]
30007f64:	bd10      	pop	{r4, pc}
30007f66:	bf00      	nop
30007f68:	3000fc50 	.word	0x3000fc50
30007f6c:	30007d85 	.word	0x30007d85

30007f70 <DDR_PHY_StructInit>:
30007f70:	f240 2315 	movw	r3, #533	; 0x215
30007f74:	6583      	str	r3, [r0, #88]	; 0x58
30007f76:	4770      	bx	lr

30007f78 <DDR_PHY_DLL_CLK_DIV>:
30007f78:	b5f8      	push	{r3, r4, r5, r6, r7, lr}
30007f7a:	4d70      	ldr	r5, [pc, #448]	; (3000813c <DDR_PHY_DLL_CLK_DIV+0x1c4>)
30007f7c:	4c70      	ldr	r4, [pc, #448]	; (30008140 <DDR_PHY_DLL_CLK_DIV+0x1c8>)
30007f7e:	fba5 3200 	umull	r3, r2, r5, r0
30007f82:	2365      	movs	r3, #101	; 0x65
30007f84:	eb00 0e04 	add.w	lr, r0, r4
30007f88:	0952      	lsrs	r2, r2, #5
30007f8a:	fb00 f303 	mul.w	r3, r0, r3
30007f8e:	486d      	ldr	r0, [pc, #436]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30007f90:	f1a2 0c03 	sub.w	ip, r2, #3
30007f94:	6a47      	ldr	r7, [r0, #36]	; 0x24
30007f96:	eb0c 018c 	add.w	r1, ip, ip, lsl #2
30007f9a:	ea4f 2c0c 	mov.w	ip, ip, lsl #8
30007f9e:	ebae 0ec1 	sub.w	lr, lr, r1, lsl #3
30007fa2:	fa1f fc8c 	uxth.w	ip, ip
30007fa6:	ea4f 2ece 	mov.w	lr, lr, lsl #11
30007faa:	fba5 1e0e 	umull	r1, lr, r5, lr
30007fae:	4966      	ldr	r1, [pc, #408]	; (30008148 <DDR_PHY_DLL_CLK_DIV+0x1d0>)
30007fb0:	fba1 6103 	umull	r6, r1, r1, r3
30007fb4:	4e65      	ldr	r6, [pc, #404]	; (3000814c <DDR_PHY_DLL_CLK_DIV+0x1d4>)
30007fb6:	0a09      	lsrs	r1, r1, #8
30007fb8:	1a52      	subs	r2, r2, r1
30007fba:	3903      	subs	r1, #3
30007fbc:	02d2      	lsls	r2, r2, #11
30007fbe:	eb02 125e 	add.w	r2, r2, lr, lsr #5
30007fc2:	ea06 2ece 	and.w	lr, r6, lr, lsl #11
30007fc6:	4e62      	ldr	r6, [pc, #392]	; (30008150 <DDR_PHY_DLL_CLK_DIV+0x1d8>)
30007fc8:	403e      	ands	r6, r7
30007fca:	ea4e 0606 	orr.w	r6, lr, r6
30007fce:	6246      	str	r6, [r0, #36]	; 0x24
30007fd0:	4e60      	ldr	r6, [pc, #384]	; (30008154 <DDR_PHY_DLL_CLK_DIV+0x1dc>)
30007fd2:	fba6 6303 	umull	r6, r3, r6, r3
30007fd6:	eb04 1353 	add.w	r3, r4, r3, lsr #5
30007fda:	6a84      	ldr	r4, [r0, #40]	; 0x28
30007fdc:	f424 447f 	bic.w	r4, r4, #65280	; 0xff00
30007fe0:	ea4c 0404 	orr.w	r4, ip, r4
30007fe4:	6284      	str	r4, [r0, #40]	; 0x28
30007fe6:	eb01 0481 	add.w	r4, r1, r1, lsl #2
30007fea:	b2c9      	uxtb	r1, r1
30007fec:	eba3 03c4 	sub.w	r3, r3, r4, lsl #3
30007ff0:	02db      	lsls	r3, r3, #11
30007ff2:	fba5 4303 	umull	r4, r3, r5, r3
30007ff6:	6a45      	ldr	r5, [r0, #36]	; 0x24
30007ff8:	4c57      	ldr	r4, [pc, #348]	; (30008158 <DDR_PHY_DLL_CLK_DIV+0x1e0>)
30007ffa:	eba2 1253 	sub.w	r2, r2, r3, lsr #5
30007ffe:	f3c3 134a 	ubfx	r3, r3, #5, #11
30008002:	402c      	ands	r4, r5
30008004:	4d55      	ldr	r5, [pc, #340]	; (3000815c <DDR_PHY_DLL_CLK_DIV+0x1e4>)
30008006:	eb02 1242 	add.w	r2, r2, r2, lsl #5
3000800a:	4323      	orrs	r3, r4
3000800c:	02d2      	lsls	r2, r2, #11
3000800e:	6243      	str	r3, [r0, #36]	; 0x24
30008010:	6a84      	ldr	r4, [r0, #40]	; 0x28
30008012:	fba5 3202 	umull	r3, r2, r5, r2
30008016:	4b52      	ldr	r3, [pc, #328]	; (30008160 <DDR_PHY_DLL_CLK_DIV+0x1e8>)
30008018:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
3000801c:	4321      	orrs	r1, r4
3000801e:	2401      	movs	r4, #1
30008020:	6281      	str	r1, [r0, #40]	; 0x28
30008022:	6a01      	ldr	r1, [r0, #32]
30008024:	400b      	ands	r3, r1
30008026:	494f      	ldr	r1, [pc, #316]	; (30008164 <DDR_PHY_DLL_CLK_DIV+0x1ec>)
30008028:	ea43 3292 	orr.w	r2, r3, r2, lsr #14
3000802c:	6202      	str	r2, [r0, #32]
3000802e:	6784      	str	r4, [r0, #120]	; 0x78
30008030:	7808      	ldrb	r0, [r1, #0]
30008032:	28ff      	cmp	r0, #255	; 0xff
30008034:	d075      	beq.n	30008122 <DDR_PHY_DLL_CLK_DIV+0x1aa>
30008036:	4c4c      	ldr	r4, [pc, #304]	; (30008168 <DDR_PHY_DLL_CLK_DIV+0x1f0>)
30008038:	0145      	lsls	r5, r0, #5
3000803a:	4b42      	ldr	r3, [pc, #264]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000803c:	eb04 1040 	add.w	r0, r4, r0, lsl #5
30008040:	5962      	ldr	r2, [r4, r5]
30008042:	4e4a      	ldr	r6, [pc, #296]	; (3000816c <DDR_PHY_DLL_CLK_DIV+0x1f4>)
30008044:	611a      	str	r2, [r3, #16]
30008046:	6842      	ldr	r2, [r0, #4]
30008048:	615a      	str	r2, [r3, #20]
3000804a:	6882      	ldr	r2, [r0, #8]
3000804c:	f8c3 2098 	str.w	r2, [r3, #152]	; 0x98
30008050:	e9d0 1203 	ldrd	r1, r2, [r0, #12]
30008054:	f8c3 10a0 	str.w	r1, [r3, #160]	; 0xa0
30008058:	609a      	str	r2, [r3, #8]
3000805a:	f8d3 209c 	ldr.w	r2, [r3, #156]	; 0x9c
3000805e:	f022 52f8 	bic.w	r2, r2, #520093696	; 0x1f000000
30008062:	f8c3 209c 	str.w	r2, [r3, #156]	; 0x9c
30008066:	7830      	ldrb	r0, [r6, #0]
30008068:	2800      	cmp	r0, #0
3000806a:	d056      	beq.n	3000811a <DDR_PHY_DLL_CLK_DIV+0x1a2>
3000806c:	2801      	cmp	r0, #1
3000806e:	442c      	add	r4, r5
30008070:	4b34      	ldr	r3, [pc, #208]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30008072:	d037      	beq.n	300080e4 <DDR_PHY_DLL_CLK_DIV+0x16c>
30008074:	2222      	movs	r2, #34	; 0x22
30008076:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
3000807a:	6962      	ldr	r2, [r4, #20]
3000807c:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
30008080:	e9d4 1206 	ldrd	r1, r2, [r4, #24]
30008084:	f8c3 1084 	str.w	r1, [r3, #132]	; 0x84
30008088:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000808c:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008090:	4937      	ldr	r1, [pc, #220]	; (30008170 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
30008092:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
30008096:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000809a:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000809e:	009b      	lsls	r3, r3, #2
300080a0:	d516      	bpl.n	300080d0 <DDR_PHY_DLL_CLK_DIV+0x158>
300080a2:	2800      	cmp	r0, #0
300080a4:	d044      	beq.n	30008130 <DDR_PHY_DLL_CLK_DIV+0x1b8>
300080a6:	4b27      	ldr	r3, [pc, #156]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300080a8:	4a32      	ldr	r2, [pc, #200]	; (30008174 <DDR_PHY_DLL_CLK_DIV+0x1fc>)
300080aa:	601a      	str	r2, [r3, #0]
300080ac:	4b25      	ldr	r3, [pc, #148]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300080ae:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
300080b2:	f422 2270 	bic.w	r2, r2, #983040	; 0xf0000
300080b6:	f442 2240 	orr.w	r2, r2, #786432	; 0xc0000
300080ba:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
300080be:	699a      	ldr	r2, [r3, #24]
300080c0:	f422 1260 	bic.w	r2, r2, #3670016	; 0x380000
300080c4:	619a      	str	r2, [r3, #24]
300080c6:	699a      	ldr	r2, [r3, #24]
300080c8:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
300080cc:	619a      	str	r2, [r3, #24]
300080ce:	bdf8      	pop	{r3, r4, r5, r6, r7, pc}
300080d0:	b920      	cbnz	r0, 300080dc <DDR_PHY_DLL_CLK_DIV+0x164>
300080d2:	4b29      	ldr	r3, [pc, #164]	; (30008178 <DDR_PHY_DLL_CLK_DIV+0x200>)
300080d4:	4798      	blx	r3
300080d6:	2801      	cmp	r0, #1
300080d8:	7030      	strb	r0, [r6, #0]
300080da:	d025      	beq.n	30008128 <DDR_PHY_DLL_CLK_DIV+0x1b0>
300080dc:	4b19      	ldr	r3, [pc, #100]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
300080de:	4a27      	ldr	r2, [pc, #156]	; (3000817c <DDR_PHY_DLL_CLK_DIV+0x204>)
300080e0:	601a      	str	r2, [r3, #0]
300080e2:	e7e3      	b.n	300080ac <DDR_PHY_DLL_CLK_DIV+0x134>
300080e4:	2233      	movs	r2, #51	; 0x33
300080e6:	6965      	ldr	r5, [r4, #20]
300080e8:	4921      	ldr	r1, [pc, #132]	; (30008170 <DDR_PHY_DLL_CLK_DIV+0x1f8>)
300080ea:	f8c3 2088 	str.w	r2, [r3, #136]	; 0x88
300080ee:	f8c3 5080 	str.w	r5, [r3, #128]	; 0x80
300080f2:	e9d4 0206 	ldrd	r0, r2, [r4, #24]
300080f6:	f8c3 0084 	str.w	r0, [r3, #132]	; 0x84
300080fa:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
300080fe:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
30008102:	f422 427f 	bic.w	r2, r2, #65280	; 0xff00
30008106:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000810a:	f8d1 3268 	ldr.w	r3, [r1, #616]	; 0x268
3000810e:	009a      	lsls	r2, r3, #2
30008110:	d50a      	bpl.n	30008128 <DDR_PHY_DLL_CLK_DIV+0x1b0>
30008112:	4b0c      	ldr	r3, [pc, #48]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
30008114:	4a1a      	ldr	r2, [pc, #104]	; (30008180 <DDR_PHY_DLL_CLK_DIV+0x208>)
30008116:	601a      	str	r2, [r3, #0]
30008118:	e7c8      	b.n	300080ac <DDR_PHY_DLL_CLK_DIV+0x134>
3000811a:	4b17      	ldr	r3, [pc, #92]	; (30008178 <DDR_PHY_DLL_CLK_DIV+0x200>)
3000811c:	4798      	blx	r3
3000811e:	7030      	strb	r0, [r6, #0]
30008120:	e7a4      	b.n	3000806c <DDR_PHY_DLL_CLK_DIV+0xf4>
30008122:	f7ff fec5 	bl	30007eb0 <DDR_PHY_ChipInfo.part.0>
30008126:	e786      	b.n	30008036 <DDR_PHY_DLL_CLK_DIV+0xbe>
30008128:	4b06      	ldr	r3, [pc, #24]	; (30008144 <DDR_PHY_DLL_CLK_DIV+0x1cc>)
3000812a:	4a16      	ldr	r2, [pc, #88]	; (30008184 <DDR_PHY_DLL_CLK_DIV+0x20c>)
3000812c:	601a      	str	r2, [r3, #0]
3000812e:	e7bd      	b.n	300080ac <DDR_PHY_DLL_CLK_DIV+0x134>
30008130:	4b11      	ldr	r3, [pc, #68]	; (30008178 <DDR_PHY_DLL_CLK_DIV+0x200>)
30008132:	4798      	blx	r3
30008134:	2801      	cmp	r0, #1
30008136:	7030      	strb	r0, [r6, #0]
30008138:	d1b5      	bne.n	300080a6 <DDR_PHY_DLL_CLK_DIV+0x12e>
3000813a:	e7ea      	b.n	30008112 <DDR_PHY_DLL_CLK_DIV+0x19a>
3000813c:	cccccccd 	.word	0xcccccccd
30008140:	001fff88 	.word	0x001fff88
30008144:	41011000 	.word	0x41011000
30008148:	10624dd3 	.word	0x10624dd3
3000814c:	07ff0000 	.word	0x07ff0000
30008150:	f800ffff 	.word	0xf800ffff
30008154:	51eb851f 	.word	0x51eb851f
30008158:	fffff800 	.word	0xfffff800
3000815c:	d1b71759 	.word	0xd1b71759
30008160:	fff80000 	.word	0xfff80000
30008164:	3000ef39 	.word	0x3000ef39
30008168:	3000d2c8 	.word	0x3000d2c8
3000816c:	3000fc50 	.word	0x3000fc50
30008170:	42008000 	.word	0x42008000
30008174:	4c000821 	.word	0x4c000821
30008178:	30007d85 	.word	0x30007d85
3000817c:	40000021 	.word	0x40000021
30008180:	4c000903 	.word	0x4c000903
30008184:	40000103 	.word	0x40000103

30008188 <DDR_PHY_CRT_Init>:
30008188:	4a32      	ldr	r2, [pc, #200]	; (30008254 <DDR_PHY_CRT_Init+0xcc>)
3000818a:	2005      	movs	r0, #5
3000818c:	b538      	push	{r3, r4, r5, lr}
3000818e:	f8d2 325c 	ldr.w	r3, [r2, #604]	; 0x25c
30008192:	4c31      	ldr	r4, [pc, #196]	; (30008258 <DDR_PHY_CRT_Init+0xd0>)
30008194:	f443 7380 	orr.w	r3, r3, #256	; 0x100
30008198:	4d30      	ldr	r5, [pc, #192]	; (3000825c <DDR_PHY_CRT_Init+0xd4>)
3000819a:	f8c2 325c 	str.w	r3, [r2, #604]	; 0x25c
3000819e:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081a2:	f043 0301 	orr.w	r3, r3, #1
300081a6:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081aa:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081ae:	f043 0330 	orr.w	r3, r3, #48	; 0x30
300081b2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081b6:	68e3      	ldr	r3, [r4, #12]
300081b8:	f023 03ff 	bic.w	r3, r3, #255	; 0xff
300081bc:	60e3      	str	r3, [r4, #12]
300081be:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081c2:	f043 0302 	orr.w	r3, r3, #2
300081c6:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081ca:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081ce:	f023 0308 	bic.w	r3, r3, #8
300081d2:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081d6:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081da:	f043 0304 	orr.w	r3, r3, #4
300081de:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081e2:	47a8      	blx	r5
300081e4:	f8d4 308c 	ldr.w	r3, [r4, #140]	; 0x8c
300081e8:	f44f 70fa 	mov.w	r0, #500	; 0x1f4
300081ec:	f043 0308 	orr.w	r3, r3, #8
300081f0:	f8c4 308c 	str.w	r3, [r4, #140]	; 0x8c
300081f4:	6aa3      	ldr	r3, [r4, #40]	; 0x28
300081f6:	f043 6380 	orr.w	r3, r3, #67108864	; 0x4000000
300081fa:	62a3      	str	r3, [r4, #40]	; 0x28
300081fc:	47a8      	blx	r5
300081fe:	6aa2      	ldr	r2, [r4, #40]	; 0x28
30008200:	f641 73ff 	movw	r3, #8191	; 0x1fff
30008204:	4916      	ldr	r1, [pc, #88]	; (30008260 <DDR_PHY_CRT_Init+0xd8>)
30008206:	f022 6280 	bic.w	r2, r2, #67108864	; 0x4000000
3000820a:	62a2      	str	r2, [r4, #40]	; 0x28
3000820c:	69e2      	ldr	r2, [r4, #28]
3000820e:	f042 0202 	orr.w	r2, r2, #2
30008212:	61e2      	str	r2, [r4, #28]
30008214:	69e2      	ldr	r2, [r4, #28]
30008216:	f042 0208 	orr.w	r2, r2, #8
3000821a:	61e2      	str	r2, [r4, #28]
3000821c:	68a2      	ldr	r2, [r4, #8]
3000821e:	f042 4200 	orr.w	r2, r2, #2147483648	; 0x80000000
30008222:	60a2      	str	r2, [r4, #8]
30008224:	6862      	ldr	r2, [r4, #4]
30008226:	4313      	orrs	r3, r2
30008228:	6063      	str	r3, [r4, #4]
3000822a:	68a3      	ldr	r3, [r4, #8]
3000822c:	f443 23e0 	orr.w	r3, r3, #458752	; 0x70000
30008230:	60a3      	str	r3, [r4, #8]
30008232:	f8c4 1240 	str.w	r1, [r4, #576]	; 0x240
30008236:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000823a:	f023 0303 	bic.w	r3, r3, #3
3000823e:	f043 0302 	orr.w	r3, r3, #2
30008242:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008246:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000824a:	f443 23c0 	orr.w	r3, r3, #393216	; 0x60000
3000824e:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008252:	bd38      	pop	{r3, r4, r5, pc}
30008254:	42008000 	.word	0x42008000
30008258:	41011000 	.word	0x41011000
3000825c:	00009b2d 	.word	0x00009b2d
30008260:	90161f00 	.word	0x90161f00

30008264 <DDR_PHY_R240_ZQ_CAL>:
30008264:	b538      	push	{r3, r4, r5, lr}
30008266:	4b1e      	ldr	r3, [pc, #120]	; (300082e0 <DDR_PHY_R240_ZQ_CAL+0x7c>)
30008268:	7818      	ldrb	r0, [r3, #0]
3000826a:	28ff      	cmp	r0, #255	; 0xff
3000826c:	d034      	beq.n	300082d8 <DDR_PHY_R240_ZQ_CAL+0x74>
3000826e:	4a1d      	ldr	r2, [pc, #116]	; (300082e4 <DDR_PHY_R240_ZQ_CAL+0x80>)
30008270:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008274:	4b1c      	ldr	r3, [pc, #112]	; (300082e8 <DDR_PHY_R240_ZQ_CAL+0x84>)
30008276:	2400      	movs	r4, #0
30008278:	eb02 0180 	add.w	r1, r2, r0, lsl #2
3000827c:	f852 5020 	ldr.w	r5, [r2, r0, lsl #2]
30008280:	f8d3 014c 	ldr.w	r0, [r3, #332]	; 0x14c
30008284:	7c0a      	ldrb	r2, [r1, #16]
30008286:	f020 003e 	bic.w	r0, r0, #62	; 0x3e
3000828a:	0052      	lsls	r2, r2, #1
3000828c:	f002 023e 	and.w	r2, r2, #62	; 0x3e
30008290:	4302      	orrs	r2, r0
30008292:	6848      	ldr	r0, [r1, #4]
30008294:	f8c3 214c 	str.w	r2, [r3, #332]	; 0x14c
30008298:	f8c3 5180 	str.w	r5, [r3, #384]	; 0x180
3000829c:	4a13      	ldr	r2, [pc, #76]	; (300082ec <DDR_PHY_R240_ZQ_CAL+0x88>)
3000829e:	f8c3 4184 	str.w	r4, [r3, #388]	; 0x184
300082a2:	f8c3 0188 	str.w	r0, [r3, #392]	; 0x188
300082a6:	f8c3 418c 	str.w	r4, [r3, #396]	; 0x18c
300082aa:	e9d1 0102 	ldrd	r0, r1, [r1, #8]
300082ae:	f8c3 0190 	str.w	r0, [r3, #400]	; 0x190
300082b2:	f8c3 2194 	str.w	r2, [r3, #404]	; 0x194
300082b6:	f8c3 1198 	str.w	r1, [r3, #408]	; 0x198
300082ba:	f8c3 219c 	str.w	r2, [r3, #412]	; 0x19c
300082be:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300082c2:	f042 6200 	orr.w	r2, r2, #134217728	; 0x8000000
300082c6:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300082ca:	f8d3 2130 	ldr.w	r2, [r3, #304]	; 0x130
300082ce:	f022 6200 	bic.w	r2, r2, #134217728	; 0x8000000
300082d2:	f8c3 2130 	str.w	r2, [r3, #304]	; 0x130
300082d6:	bd38      	pop	{r3, r4, r5, pc}
300082d8:	f7ff fdea 	bl	30007eb0 <DDR_PHY_ChipInfo.part.0>
300082dc:	e7c7      	b.n	3000826e <DDR_PHY_R240_ZQ_CAL+0xa>
300082de:	bf00      	nop
300082e0:	3000ef39 	.word	0x3000ef39
300082e4:	3000d408 	.word	0x3000d408
300082e8:	41011000 	.word	0x41011000
300082ec:	00262626 	.word	0x00262626

300082f0 <DDR_PHY_ZQ_SET_SEL>:
300082f0:	b410      	push	{r4}
300082f2:	4b25      	ldr	r3, [pc, #148]	; (30008388 <DDR_PHY_ZQ_SET_SEL+0x98>)
300082f4:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
300082f8:	4a24      	ldr	r2, [pc, #144]	; (3000838c <DDR_PHY_ZQ_SET_SEL+0x9c>)
300082fa:	4c25      	ldr	r4, [pc, #148]	; (30008390 <DDR_PHY_ZQ_SET_SEL+0xa0>)
300082fc:	4925      	ldr	r1, [pc, #148]	; (30008394 <DDR_PHY_ZQ_SET_SEL+0xa4>)
300082fe:	f8c3 21c0 	str.w	r2, [r3, #448]	; 0x1c0
30008302:	f8c3 21c4 	str.w	r2, [r3, #452]	; 0x1c4
30008306:	f8c3 21c8 	str.w	r2, [r3, #456]	; 0x1c8
3000830a:	f8c3 21cc 	str.w	r2, [r3, #460]	; 0x1cc
3000830e:	f8c3 41d0 	str.w	r4, [r3, #464]	; 0x1d0
30008312:	f8c3 41d4 	str.w	r4, [r3, #468]	; 0x1d4
30008316:	f64f 74ff 	movw	r4, #65535	; 0xffff
3000831a:	f8c3 21d8 	str.w	r2, [r3, #472]	; 0x1d8
3000831e:	f8c3 21dc 	str.w	r2, [r3, #476]	; 0x1dc
30008322:	2200      	movs	r2, #0
30008324:	f8c3 01e0 	str.w	r0, [r3, #480]	; 0x1e0
30008328:	f8c3 01e4 	str.w	r0, [r3, #484]	; 0x1e4
3000832c:	f8c3 11e8 	str.w	r1, [r3, #488]	; 0x1e8
30008330:	f8c3 11ec 	str.w	r1, [r3, #492]	; 0x1ec
30008334:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
30008338:	f8c3 21f0 	str.w	r2, [r3, #496]	; 0x1f0
3000833c:	f8c3 2204 	str.w	r2, [r3, #516]	; 0x204
30008340:	f8c3 2208 	str.w	r2, [r3, #520]	; 0x208
30008344:	f8c3 220c 	str.w	r2, [r3, #524]	; 0x20c
30008348:	f8c3 2210 	str.w	r2, [r3, #528]	; 0x210
3000834c:	f8c3 2214 	str.w	r2, [r3, #532]	; 0x214
30008350:	f8c3 2218 	str.w	r2, [r3, #536]	; 0x218
30008354:	f8c3 221c 	str.w	r2, [r3, #540]	; 0x21c
30008358:	f8c3 2220 	str.w	r2, [r3, #544]	; 0x220
3000835c:	f8c3 4224 	str.w	r4, [r3, #548]	; 0x224
30008360:	f8c3 1228 	str.w	r1, [r3, #552]	; 0x228
30008364:	f8c3 122c 	str.w	r1, [r3, #556]	; 0x22c
30008368:	f8c3 1230 	str.w	r1, [r3, #560]	; 0x230
3000836c:	f8c3 1234 	str.w	r1, [r3, #564]	; 0x234
30008370:	f8c3 1328 	str.w	r1, [r3, #808]	; 0x328
30008374:	f8c3 132c 	str.w	r1, [r3, #812]	; 0x32c
30008378:	f8c3 1330 	str.w	r1, [r3, #816]	; 0x330
3000837c:	f8c3 1334 	str.w	r1, [r3, #820]	; 0x334
30008380:	f85d 4b04 	ldr.w	r4, [sp], #4
30008384:	4770      	bx	lr
30008386:	bf00      	nop
30008388:	41011000 	.word	0x41011000
3000838c:	ffff1111 	.word	0xffff1111
30008390:	ffff3333 	.word	0xffff3333
30008394:	ffff0000 	.word	0xffff0000

30008398 <DDR_PHY_DELAY_TAP_SET>:
30008398:	b510      	push	{r4, lr}
3000839a:	4b15      	ldr	r3, [pc, #84]	; (300083f0 <DDR_PHY_DELAY_TAP_SET+0x58>)
3000839c:	f04f 3288 	mov.w	r2, #2290649224	; 0x88888888
300083a0:	4914      	ldr	r1, [pc, #80]	; (300083f4 <DDR_PHY_DELAY_TAP_SET+0x5c>)
300083a2:	4c15      	ldr	r4, [pc, #84]	; (300083f8 <DDR_PHY_DELAY_TAP_SET+0x60>)
300083a4:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
300083a8:	f8c3 2104 	str.w	r2, [r3, #260]	; 0x104
300083ac:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300083b0:	f8c3 110c 	str.w	r1, [r3, #268]	; 0x10c
300083b4:	f8c3 2110 	str.w	r2, [r3, #272]	; 0x110
300083b8:	7820      	ldrb	r0, [r4, #0]
300083ba:	b1a0      	cbz	r0, 300083e6 <DDR_PHY_DELAY_TAP_SET+0x4e>
300083bc:	2801      	cmp	r0, #1
300083be:	4b0c      	ldr	r3, [pc, #48]	; (300083f0 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083c0:	f04f 3188 	mov.w	r1, #2290649224	; 0x88888888
300083c4:	bf0c      	ite	eq
300083c6:	4a0d      	ldreq	r2, [pc, #52]	; (300083fc <DDR_PHY_DELAY_TAP_SET+0x64>)
300083c8:	4a0d      	ldrne	r2, [pc, #52]	; (30008400 <DDR_PHY_DELAY_TAP_SET+0x68>)
300083ca:	f8c3 2154 	str.w	r2, [r3, #340]	; 0x154
300083ce:	f640 0288 	movw	r2, #2184	; 0x888
300083d2:	4b07      	ldr	r3, [pc, #28]	; (300083f0 <DDR_PHY_DELAY_TAP_SET+0x58>)
300083d4:	f8c3 1480 	str.w	r1, [r3, #1152]	; 0x480
300083d8:	f8c3 1484 	str.w	r1, [r3, #1156]	; 0x484
300083dc:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
300083e0:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
300083e4:	bd10      	pop	{r4, pc}
300083e6:	4b07      	ldr	r3, [pc, #28]	; (30008404 <DDR_PHY_DELAY_TAP_SET+0x6c>)
300083e8:	4798      	blx	r3
300083ea:	7020      	strb	r0, [r4, #0]
300083ec:	e7e6      	b.n	300083bc <DDR_PHY_DELAY_TAP_SET+0x24>
300083ee:	bf00      	nop
300083f0:	41011000 	.word	0x41011000
300083f4:	20888888 	.word	0x20888888
300083f8:	3000fc50 	.word	0x3000fc50
300083fc:	7c033333 	.word	0x7c033333
30008400:	7c011111 	.word	0x7c011111
30008404:	30007d85 	.word	0x30007d85

30008408 <DDR_PHY_READ_CTRL>:
30008408:	4b10      	ldr	r3, [pc, #64]	; (3000844c <DDR_PHY_READ_CTRL+0x44>)
3000840a:	b510      	push	{r4, lr}
3000840c:	7818      	ldrb	r0, [r3, #0]
3000840e:	28ff      	cmp	r0, #255	; 0xff
30008410:	d019      	beq.n	30008446 <DDR_PHY_READ_CTRL+0x3e>
30008412:	4a0f      	ldr	r2, [pc, #60]	; (30008450 <DDR_PHY_READ_CTRL+0x48>)
30008414:	eb00 0080 	add.w	r0, r0, r0, lsl #2
30008418:	4b0e      	ldr	r3, [pc, #56]	; (30008454 <DDR_PHY_READ_CTRL+0x4c>)
3000841a:	2108      	movs	r1, #8
3000841c:	eb02 0080 	add.w	r0, r2, r0, lsl #2
30008420:	22fe      	movs	r2, #254	; 0xfe
30008422:	7c44      	ldrb	r4, [r0, #17]
30008424:	f8c3 45f8 	str.w	r4, [r3, #1528]	; 0x5f8
30008428:	7c84      	ldrb	r4, [r0, #18]
3000842a:	7cc0      	ldrb	r0, [r0, #19]
3000842c:	f8c3 45fc 	str.w	r4, [r3, #1532]	; 0x5fc
30008430:	f8c3 0170 	str.w	r0, [r3, #368]	; 0x170
30008434:	f8c3 1408 	str.w	r1, [r3, #1032]	; 0x408
30008438:	f8c3 140c 	str.w	r1, [r3, #1036]	; 0x40c
3000843c:	f8c3 2418 	str.w	r2, [r3, #1048]	; 0x418
30008440:	f8c3 241c 	str.w	r2, [r3, #1052]	; 0x41c
30008444:	bd10      	pop	{r4, pc}
30008446:	f7ff fd33 	bl	30007eb0 <DDR_PHY_ChipInfo.part.0>
3000844a:	e7e2      	b.n	30008412 <DDR_PHY_READ_CTRL+0xa>
3000844c:	3000ef39 	.word	0x3000ef39
30008450:	3000d408 	.word	0x3000d408
30008454:	41011000 	.word	0x41011000

30008458 <DDR_PHY_READ_LEVELING>:
30008458:	b508      	push	{r3, lr}
3000845a:	4b21      	ldr	r3, [pc, #132]	; (300084e0 <DDR_PHY_READ_LEVELING+0x88>)
3000845c:	7818      	ldrb	r0, [r3, #0]
3000845e:	28ff      	cmp	r0, #255	; 0xff
30008460:	d03b      	beq.n	300084da <DDR_PHY_READ_LEVELING+0x82>
30008462:	4b20      	ldr	r3, [pc, #128]	; (300084e4 <DDR_PHY_READ_LEVELING+0x8c>)
30008464:	b1e8      	cbz	r0, 300084a2 <DDR_PHY_READ_LEVELING+0x4a>
30008466:	f04f 3112 	mov.w	r1, #303174162	; 0x12121212
3000846a:	f04f 3011 	mov.w	r0, #286331153	; 0x11111111
3000846e:	220f      	movs	r2, #15
30008470:	f8c3 1440 	str.w	r1, [r3, #1088]	; 0x440
30008474:	f8c3 1448 	str.w	r1, [r3, #1096]	; 0x448
30008478:	f8c3 0444 	str.w	r0, [r3, #1092]	; 0x444
3000847c:	f8c3 044c 	str.w	r0, [r3, #1100]	; 0x44c
30008480:	f8c3 0458 	str.w	r0, [r3, #1112]	; 0x458
30008484:	f8c3 0460 	str.w	r0, [r3, #1120]	; 0x460
30008488:	f8c3 145c 	str.w	r1, [r3, #1116]	; 0x45c
3000848c:	f8c3 1464 	str.w	r1, [r3, #1124]	; 0x464
30008490:	f8c3 2450 	str.w	r2, [r3, #1104]	; 0x450
30008494:	f8c3 2454 	str.w	r2, [r3, #1108]	; 0x454
30008498:	f8c3 2468 	str.w	r2, [r3, #1128]	; 0x468
3000849c:	f8c3 246c 	str.w	r2, [r3, #1132]	; 0x46c
300084a0:	bd08      	pop	{r3, pc}
300084a2:	f04f 3210 	mov.w	r2, #269488144	; 0x10101010
300084a6:	2110      	movs	r1, #16
300084a8:	f8c3 2440 	str.w	r2, [r3, #1088]	; 0x440
300084ac:	f8c3 2448 	str.w	r2, [r3, #1096]	; 0x448
300084b0:	f8c3 2458 	str.w	r2, [r3, #1112]	; 0x458
300084b4:	f8c3 2460 	str.w	r2, [r3, #1120]	; 0x460
300084b8:	f8c3 2444 	str.w	r2, [r3, #1092]	; 0x444
300084bc:	f8c3 244c 	str.w	r2, [r3, #1100]	; 0x44c
300084c0:	f8c3 245c 	str.w	r2, [r3, #1116]	; 0x45c
300084c4:	f8c3 2464 	str.w	r2, [r3, #1124]	; 0x464
300084c8:	f8c3 1450 	str.w	r1, [r3, #1104]	; 0x450
300084cc:	f8c3 1454 	str.w	r1, [r3, #1108]	; 0x454
300084d0:	f8c3 1468 	str.w	r1, [r3, #1128]	; 0x468
300084d4:	f8c3 146c 	str.w	r1, [r3, #1132]	; 0x46c
300084d8:	bd08      	pop	{r3, pc}
300084da:	f7ff fce9 	bl	30007eb0 <DDR_PHY_ChipInfo.part.0>
300084de:	e7c0      	b.n	30008462 <DDR_PHY_READ_LEVELING+0xa>
300084e0:	3000ef39 	.word	0x3000ef39
300084e4:	41011000 	.word	0x41011000

300084e8 <DDR_PHY_Init>:
300084e8:	b510      	push	{r4, lr}
300084ea:	6d80      	ldr	r0, [r0, #88]	; 0x58
300084ec:	4c2f      	ldr	r4, [pc, #188]	; (300085ac <DDR_PHY_Init+0xc4>)
300084ee:	f7ff fd43 	bl	30007f78 <DDR_PHY_DLL_CLK_DIV>
300084f2:	f7ff fe49 	bl	30008188 <DDR_PHY_CRT_Init>
300084f6:	f7ff feb5 	bl	30008264 <DDR_PHY_R240_ZQ_CAL>
300084fa:	f7ff fef9 	bl	300082f0 <DDR_PHY_ZQ_SET_SEL>
300084fe:	f04f 1211 	mov.w	r2, #1114129	; 0x110011
30008502:	2300      	movs	r3, #0
30008504:	f44f 5180 	mov.w	r1, #4096	; 0x1000
30008508:	4829      	ldr	r0, [pc, #164]	; (300085b0 <DDR_PHY_Init+0xc8>)
3000850a:	f8c4 24f0 	str.w	r2, [r4, #1264]	; 0x4f0
3000850e:	f8c4 24f4 	str.w	r2, [r4, #1268]	; 0x4f4
30008512:	f8c4 34e8 	str.w	r3, [r4, #1256]	; 0x4e8
30008516:	f8c4 34ec 	str.w	r3, [r4, #1260]	; 0x4ec
3000851a:	f8c4 017c 	str.w	r0, [r4, #380]	; 0x17c
3000851e:	f8c4 12a4 	str.w	r1, [r4, #676]	; 0x2a4
30008522:	f7ff ff39 	bl	30008398 <DDR_PHY_DELAY_TAP_SET>
30008526:	f7ff ff6f 	bl	30008408 <DDR_PHY_READ_CTRL>
3000852a:	f04f 3322 	mov.w	r3, #572662306	; 0x22222222
3000852e:	4a21      	ldr	r2, [pc, #132]	; (300085b4 <DDR_PHY_Init+0xcc>)
30008530:	f8c4 2178 	str.w	r2, [r4, #376]	; 0x178
30008534:	f8c4 3420 	str.w	r3, [r4, #1056]	; 0x420
30008538:	f8c4 3424 	str.w	r3, [r4, #1060]	; 0x424
3000853c:	f8c4 3430 	str.w	r3, [r4, #1072]	; 0x430
30008540:	f8c4 3434 	str.w	r3, [r4, #1076]	; 0x434
30008544:	f7ff ff88 	bl	30008458 <DDR_PHY_READ_LEVELING>
30008548:	f8d4 31f8 	ldr.w	r3, [r4, #504]	; 0x1f8
3000854c:	f04f 31d8 	mov.w	r1, #3638089944	; 0xd8d8d8d8
30008550:	f04f 328c 	mov.w	r2, #2358021260	; 0x8c8c8c8c
30008554:	f423 437e 	bic.w	r3, r3, #65024	; 0xfe00
30008558:	f443 4364 	orr.w	r3, r3, #58368	; 0xe400
3000855c:	f8c4 31f8 	str.w	r3, [r4, #504]	; 0x1f8
30008560:	f8c4 11fc 	str.w	r1, [r4, #508]	; 0x1fc
30008564:	f8d4 32d0 	ldr.w	r3, [r4, #720]	; 0x2d0
30008568:	f043 032f 	orr.w	r3, r3, #47	; 0x2f
3000856c:	f8c4 32d0 	str.w	r3, [r4, #720]	; 0x2d0
30008570:	f8c4 21fc 	str.w	r2, [r4, #508]	; 0x1fc
30008574:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008578:	f043 0303 	orr.w	r3, r3, #3
3000857c:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
30008580:	f8d4 323c 	ldr.w	r3, [r4, #572]	; 0x23c
30008584:	f043 030c 	orr.w	r3, r3, #12
30008588:	f8c4 323c 	str.w	r3, [r4, #572]	; 0x23c
3000858c:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
30008590:	f023 0303 	bic.w	r3, r3, #3
30008594:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
30008598:	f8d4 3238 	ldr.w	r3, [r4, #568]	; 0x238
3000859c:	f423 537c 	bic.w	r3, r3, #16128	; 0x3f00
300085a0:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
300085a4:	f8c4 3238 	str.w	r3, [r4, #568]	; 0x238
300085a8:	bd10      	pop	{r4, pc}
300085aa:	bf00      	nop
300085ac:	41011000 	.word	0x41011000
300085b0:	00045500 	.word	0x00045500
300085b4:	11117777 	.word	0x11117777

300085b8 <DDR_PHY_CAL>:
300085b8:	4770      	bx	lr
300085ba:	bf00      	nop

300085bc <DDR_PHY_AutoGating>:
300085bc:	4a0c      	ldr	r2, [pc, #48]	; (300085f0 <DDR_PHY_AutoGating+0x34>)
300085be:	2032      	movs	r0, #50	; 0x32
300085c0:	490c      	ldr	r1, [pc, #48]	; (300085f4 <DDR_PHY_AutoGating+0x38>)
300085c2:	b508      	push	{r3, lr}
300085c4:	6893      	ldr	r3, [r2, #8]
300085c6:	f023 5300 	bic.w	r3, r3, #536870912	; 0x20000000
300085ca:	6093      	str	r3, [r2, #8]
300085cc:	f8d2 30ac 	ldr.w	r3, [r2, #172]	; 0xac
300085d0:	f023 033c 	bic.w	r3, r3, #60	; 0x3c
300085d4:	f043 0314 	orr.w	r3, r3, #20
300085d8:	f8c2 30ac 	str.w	r3, [r2, #172]	; 0xac
300085dc:	4788      	blx	r1
300085de:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
300085e2:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
300085e6:	f043 0303 	orr.w	r3, r3, #3
300085ea:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
300085ee:	bd08      	pop	{r3, pc}
300085f0:	41011000 	.word	0x41011000
300085f4:	00009b2d 	.word	0x00009b2d

300085f8 <flash_init_userdef>:
300085f8:	4b14      	ldr	r3, [pc, #80]	; (3000864c <flash_init_userdef+0x54>)
300085fa:	2201      	movs	r2, #1
300085fc:	2140      	movs	r1, #64	; 0x40
300085fe:	f44f 6000 	mov.w	r0, #2048	; 0x800
30008602:	b430      	push	{r4, r5}
30008604:	e9c3 1203 	strd	r1, r2, [r3, #12]
30008608:	2502      	movs	r5, #2
3000860a:	2400      	movs	r4, #0
3000860c:	f640 0104 	movw	r1, #2052	; 0x804
30008610:	601a      	str	r2, [r3, #0]
30008612:	22bb      	movs	r2, #187	; 0xbb
30008614:	83d8      	strh	r0, [r3, #30]
30008616:	8419      	strh	r1, [r3, #32]
30008618:	203b      	movs	r0, #59	; 0x3b
3000861a:	21eb      	movs	r1, #235	; 0xeb
3000861c:	629a      	str	r2, [r3, #40]	; 0x28
3000861e:	4a0c      	ldr	r2, [pc, #48]	; (30008650 <flash_init_userdef+0x58>)
30008620:	6258      	str	r0, [r3, #36]	; 0x24
30008622:	6319      	str	r1, [r3, #48]	; 0x30
30008624:	f64a 3120 	movw	r1, #43808	; 0xab20
30008628:	655a      	str	r2, [r3, #84]	; 0x54
3000862a:	22b9      	movs	r2, #185	; 0xb9
3000862c:	4809      	ldr	r0, [pc, #36]	; (30008654 <flash_init_userdef+0x5c>)
3000862e:	f8a3 105c 	strh.w	r1, [r3, #92]	; 0x5c
30008632:	6598      	str	r0, [r3, #88]	; 0x58
30008634:	f883 205e 	strb.w	r2, [r3, #94]	; 0x5e
30008638:	e9c3 5405 	strd	r5, r4, [r3, #20]
3000863c:	2406      	movs	r4, #6
3000863e:	f883 4022 	strb.w	r4, [r3, #34]	; 0x22
30008642:	246b      	movs	r4, #107	; 0x6b
30008644:	62dc      	str	r4, [r3, #44]	; 0x2c
30008646:	bc30      	pop	{r4, r5}
30008648:	4770      	bx	lr
3000864a:	bf00      	nop
3000864c:	2001c01c 	.word	0x2001c01c
30008650:	00059f06 	.word	0x00059f06
30008654:	d8600001 	.word	0xd8600001

30008658 <nand_init_userdef>:
30008658:	4b18      	ldr	r3, [pc, #96]	; (300086bc <nand_init_userdef+0x64>)
3000865a:	2001      	movs	r0, #1
3000865c:	2200      	movs	r2, #0
3000865e:	2102      	movs	r1, #2
30008660:	b410      	push	{r4}
30008662:	6018      	str	r0, [r3, #0]
30008664:	f640 0408 	movw	r4, #2056	; 0x808
30008668:	6159      	str	r1, [r3, #20]
3000866a:	83dc      	strh	r4, [r3, #30]
3000866c:	f44f 6400 	mov.w	r4, #2048	; 0x800
30008670:	f8a3 1062 	strh.w	r1, [r3, #98]	; 0x62
30008674:	841c      	strh	r4, [r3, #32]
30008676:	246b      	movs	r4, #107	; 0x6b
30008678:	619a      	str	r2, [r3, #24]
3000867a:	62dc      	str	r4, [r3, #44]	; 0x2c
3000867c:	4c10      	ldr	r4, [pc, #64]	; (300086c0 <nand_init_userdef+0x68>)
3000867e:	775a      	strb	r2, [r3, #29]
30008680:	f883 2022 	strb.w	r2, [r3, #34]	; 0x22
30008684:	629a      	str	r2, [r3, #40]	; 0x28
30008686:	631a      	str	r2, [r3, #48]	; 0x30
30008688:	f883 2061 	strb.w	r2, [r3, #97]	; 0x61
3000868c:	65da      	str	r2, [r3, #92]	; 0x5c
3000868e:	e9c3 0003 	strd	r0, r0, [r3, #12]
30008692:	203b      	movs	r0, #59	; 0x3b
30008694:	6258      	str	r0, [r3, #36]	; 0x24
30008696:	200b      	movs	r0, #11
30008698:	e9c3 1211 	strd	r1, r2, [r3, #68]	; 0x44
3000869c:	6698      	str	r0, [r3, #104]	; 0x68
3000869e:	2008      	movs	r0, #8
300086a0:	4908      	ldr	r1, [pc, #32]	; (300086c4 <nand_init_userdef+0x6c>)
300086a2:	64d8      	str	r0, [r3, #76]	; 0x4c
300086a4:	f241 0013 	movw	r0, #4115	; 0x1013
300086a8:	6599      	str	r1, [r3, #88]	; 0x58
300086aa:	4907      	ldr	r1, [pc, #28]	; (300086c8 <nand_init_userdef+0x70>)
300086ac:	f8a3 006e 	strh.w	r0, [r3, #110]	; 0x6e
300086b0:	6719      	str	r1, [r3, #112]	; 0x70
300086b2:	e9c3 2414 	strd	r2, r4, [r3, #80]	; 0x50
300086b6:	f85d 4b04 	ldr.w	r4, [sp], #4
300086ba:	4770      	bx	lr
300086bc:	2001c01c 	.word	0x2001c01c
300086c0:	000f9f06 	.word	0x000f9f06
300086c4:	d800001f 	.word	0xd800001f
300086c8:	10300804 	.word	0x10300804

300086cc <flash_get_layout_info>:
300086cc:	b570      	push	{r4, r5, r6, lr}
300086ce:	4b0f      	ldr	r3, [pc, #60]	; (3000870c <flash_get_layout_info+0x40>)
300086d0:	4615      	mov	r5, r2
300086d2:	4604      	mov	r4, r0
300086d4:	460e      	mov	r6, r1
300086d6:	4798      	blx	r3
300086d8:	4b0d      	ldr	r3, [pc, #52]	; (30008710 <flash_get_layout_info+0x44>)
300086da:	4a0e      	ldr	r2, [pc, #56]	; (30008714 <flash_get_layout_info+0x48>)
300086dc:	2800      	cmp	r0, #0
300086de:	bf14      	ite	ne
300086e0:	469c      	movne	ip, r3
300086e2:	4694      	moveq	ip, r2
300086e4:	f8dc 3000 	ldr.w	r3, [ip]
300086e8:	2bff      	cmp	r3, #255	; 0xff
300086ea:	d104      	bne.n	300086f6 <flash_get_layout_info+0x2a>
300086ec:	e00d      	b.n	3000870a <flash_get_layout_info+0x3e>
300086ee:	f85c 3f0c 	ldr.w	r3, [ip, #12]!
300086f2:	2bff      	cmp	r3, #255	; 0xff
300086f4:	d009      	beq.n	3000870a <flash_get_layout_info+0x3e>
300086f6:	429c      	cmp	r4, r3
300086f8:	d1f9      	bne.n	300086ee <flash_get_layout_info+0x22>
300086fa:	b116      	cbz	r6, 30008702 <flash_get_layout_info+0x36>
300086fc:	f8dc 3004 	ldr.w	r3, [ip, #4]
30008700:	6033      	str	r3, [r6, #0]
30008702:	b115      	cbz	r5, 3000870a <flash_get_layout_info+0x3e>
30008704:	f8dc 3008 	ldr.w	r3, [ip, #8]
30008708:	602b      	str	r3, [r5, #0]
3000870a:	bd70      	pop	{r4, r5, r6, pc}
3000870c:	30009bb5 	.word	0x30009bb5
30008710:	3000f014 	.word	0x3000f014
30008714:	3000ef3c 	.word	0x3000ef3c

30008718 <NAND_CHECK_IS_BAD_BLOCK>:
30008718:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000871c:	4e24      	ldr	r6, [pc, #144]	; (300087b0 <NAND_CHECK_IS_BAD_BLOCK+0x98>)
3000871e:	0907      	lsrs	r7, r0, #4
30008720:	0045      	lsls	r5, r0, #1
30008722:	b085      	sub	sp, #20
30008724:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
30008728:	f005 051e 	and.w	r5, r5, #30
3000872c:	40eb      	lsrs	r3, r5
3000872e:	f003 0303 	and.w	r3, r3, #3
30008732:	2b02      	cmp	r3, #2
30008734:	d02c      	beq.n	30008790 <NAND_CHECK_IS_BAD_BLOCK+0x78>
30008736:	2b01      	cmp	r3, #1
30008738:	d026      	beq.n	30008788 <NAND_CHECK_IS_BAD_BLOCK+0x70>
3000873a:	4b1e      	ldr	r3, [pc, #120]	; (300087b4 <NAND_CHECK_IS_BAD_BLOCK+0x9c>)
3000873c:	f04f 0801 	mov.w	r8, #1
30008740:	2204      	movs	r2, #4
30008742:	4604      	mov	r4, r0
30008744:	f893 106c 	ldrb.w	r1, [r3, #108]	; 0x6c
30008748:	0180      	lsls	r0, r0, #6
3000874a:	ab03      	add	r3, sp, #12
3000874c:	f8df 9070 	ldr.w	r9, [pc, #112]	; 300087c0 <NAND_CHECK_IS_BAD_BLOCK+0xa8>
30008750:	fa08 f101 	lsl.w	r1, r8, r1
30008754:	47c8      	blx	r9
30008756:	f89d 300c 	ldrb.w	r3, [sp, #12]
3000875a:	f89d 200d 	ldrb.w	r2, [sp, #13]
3000875e:	4013      	ands	r3, r2
30008760:	2bff      	cmp	r3, #255	; 0xff
30008762:	d019      	beq.n	30008798 <NAND_CHECK_IS_BAD_BLOCK+0x80>
30008764:	2002      	movs	r0, #2
30008766:	f856 2027 	ldr.w	r2, [r6, r7, lsl #2]
3000876a:	4b13      	ldr	r3, [pc, #76]	; (300087b8 <NAND_CHECK_IS_BAD_BLOCK+0xa0>)
3000876c:	fa00 f505 	lsl.w	r5, r0, r5
30008770:	9400      	str	r4, [sp, #0]
30008772:	4619      	mov	r1, r3
30008774:	432a      	orrs	r2, r5
30008776:	f846 2027 	str.w	r2, [r6, r7, lsl #2]
3000877a:	4a10      	ldr	r2, [pc, #64]	; (300087bc <NAND_CHECK_IS_BAD_BLOCK+0xa4>)
3000877c:	f003 fbb6 	bl	3000beec <rtk_log_write>
30008780:	4640      	mov	r0, r8
30008782:	b005      	add	sp, #20
30008784:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008788:	2000      	movs	r0, #0
3000878a:	b005      	add	sp, #20
3000878c:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008790:	2001      	movs	r0, #1
30008792:	b005      	add	sp, #20
30008794:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30008798:	f856 3027 	ldr.w	r3, [r6, r7, lsl #2]
3000879c:	fa08 f005 	lsl.w	r0, r8, r5
300087a0:	4303      	orrs	r3, r0
300087a2:	2000      	movs	r0, #0
300087a4:	f846 3027 	str.w	r3, [r6, r7, lsl #2]
300087a8:	b005      	add	sp, #20
300087aa:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300087ae:	bf00      	nop
300087b0:	3000fc58 	.word	0x3000fc58
300087b4:	2001c01c 	.word	0x2001c01c
300087b8:	3000d69c 	.word	0x3000d69c
300087bc:	3000d6a8 	.word	0x3000d6a8
300087c0:	30009b05 	.word	0x30009b05

300087c4 <Nand_Get_NandAddr>:
300087c4:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
300087c8:	4f10      	ldr	r7, [pc, #64]	; (3000880c <Nand_Get_NandAddr+0x48>)
300087ca:	f101 4678 	add.w	r6, r1, #4160749568	; 0xf8000000
300087ce:	f100 4078 	add.w	r0, r0, #4160749568	; 0xf8000000
300087d2:	f897 506c 	ldrb.w	r5, [r7, #108]	; 0x6c
300087d6:	40e8      	lsrs	r0, r5
300087d8:	fa26 f505 	lsr.w	r5, r6, r5
300087dc:	09ad      	lsrs	r5, r5, #6
300087de:	ebb5 1f90 	cmp.w	r5, r0, lsr #6
300087e2:	d90f      	bls.n	30008804 <Nand_Get_NandAddr+0x40>
300087e4:	0984      	lsrs	r4, r0, #6
300087e6:	f04f 0840 	mov.w	r8, #64	; 0x40
300087ea:	4620      	mov	r0, r4
300087ec:	3401      	adds	r4, #1
300087ee:	f7ff ff93 	bl	30008718 <NAND_CHECK_IS_BAD_BLOCK>
300087f2:	b128      	cbz	r0, 30008800 <Nand_Get_NandAddr+0x3c>
300087f4:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
300087f8:	3501      	adds	r5, #1
300087fa:	fa08 f303 	lsl.w	r3, r8, r3
300087fe:	441e      	add	r6, r3
30008800:	42a5      	cmp	r5, r4
30008802:	d8f2      	bhi.n	300087ea <Nand_Get_NandAddr+0x26>
30008804:	4630      	mov	r0, r6
30008806:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000880a:	bf00      	nop
3000880c:	2001c01c 	.word	0x2001c01c

30008810 <Nand_L2P_Table>:
30008810:	e92d 41f0 	stmdb	sp!, {r4, r5, r6, r7, r8, lr}
30008814:	b082      	sub	sp, #8
30008816:	4604      	mov	r4, r0
30008818:	2002      	movs	r0, #2
3000881a:	aa01      	add	r2, sp, #4
3000881c:	4669      	mov	r1, sp
3000881e:	f7ff ff55 	bl	300086cc <flash_get_layout_info>
30008822:	9800      	ldr	r0, [sp, #0]
30008824:	42a0      	cmp	r0, r4
30008826:	d810      	bhi.n	3000884a <Nand_L2P_Table+0x3a>
30008828:	9b01      	ldr	r3, [sp, #4]
3000882a:	42a3      	cmp	r3, r4
3000882c:	d805      	bhi.n	3000883a <Nand_L2P_Table+0x2a>
3000882e:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008832:	4620      	mov	r0, r4
30008834:	b002      	add	sp, #8
30008836:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000883a:	4621      	mov	r1, r4
3000883c:	f7ff ffc2 	bl	300087c4 <Nand_Get_NandAddr>
30008840:	4604      	mov	r4, r0
30008842:	4620      	mov	r0, r4
30008844:	b002      	add	sp, #8
30008846:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
3000884a:	4f0e      	ldr	r7, [pc, #56]	; (30008884 <Nand_L2P_Table+0x74>)
3000884c:	f104 4478 	add.w	r4, r4, #4160749568	; 0xf8000000
30008850:	f897 606c 	ldrb.w	r6, [r7, #108]	; 0x6c
30008854:	fa24 f606 	lsr.w	r6, r4, r6
30008858:	09b6      	lsrs	r6, r6, #6
3000885a:	d0ea      	beq.n	30008832 <Nand_L2P_Table+0x22>
3000885c:	2500      	movs	r5, #0
3000885e:	f04f 0840 	mov.w	r8, #64	; 0x40
30008862:	4628      	mov	r0, r5
30008864:	3501      	adds	r5, #1
30008866:	f7ff ff57 	bl	30008718 <NAND_CHECK_IS_BAD_BLOCK>
3000886a:	b128      	cbz	r0, 30008878 <Nand_L2P_Table+0x68>
3000886c:	f897 306c 	ldrb.w	r3, [r7, #108]	; 0x6c
30008870:	3601      	adds	r6, #1
30008872:	fa08 f303 	lsl.w	r3, r8, r3
30008876:	441c      	add	r4, r3
30008878:	42b5      	cmp	r5, r6
3000887a:	d3f2      	bcc.n	30008862 <Nand_L2P_Table+0x52>
3000887c:	4620      	mov	r0, r4
3000887e:	b002      	add	sp, #8
30008880:	e8bd 81f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, pc}
30008884:	2001c01c 	.word	0x2001c01c

30008888 <NandImgCopy>:
30008888:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000888c:	4617      	mov	r7, r2
3000888e:	4603      	mov	r3, r0
30008890:	b089      	sub	sp, #36	; 0x24
30008892:	f8df 9100 	ldr.w	r9, [pc, #256]	; 30008994 <NandImgCopy+0x10c>
30008896:	f107 38ff 	add.w	r8, r7, #4294967295	; 0xffffffff
3000889a:	4608      	mov	r0, r1
3000889c:	9307      	str	r3, [sp, #28]
3000889e:	f7ff ffb7 	bl	30008810 <Nand_L2P_Table>
300088a2:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
300088a6:	4480      	add	r8, r0
300088a8:	fa20 f406 	lsr.w	r4, r0, r6
300088ac:	fa28 f806 	lsr.w	r8, r8, r6
300088b0:	eba8 0804 	sub.w	r8, r8, r4
300088b4:	f1b8 3fff 	cmp.w	r8, #4294967295	; 0xffffffff
300088b8:	d054      	beq.n	30008964 <NandImgCopy+0xdc>
300088ba:	2300      	movs	r3, #0
300088bc:	4605      	mov	r5, r0
300088be:	f04f 0b40 	mov.w	fp, #64	; 0x40
300088c2:	469a      	mov	sl, r3
300088c4:	9306      	str	r3, [sp, #24]
300088c6:	463b      	mov	r3, r7
300088c8:	4647      	mov	r7, r8
300088ca:	4698      	mov	r8, r3
300088cc:	e020      	b.n	30008910 <NandImgCopy+0x88>
300088ce:	f1c2 0320 	rsb	r3, r2, #32
300088d2:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
300088d6:	40d9      	lsrs	r1, r3
300088d8:	9b07      	ldr	r3, [sp, #28]
300088da:	4029      	ands	r1, r5
300088dc:	4453      	add	r3, sl
300088de:	b377      	cbz	r7, 3000893e <NandImgCopy+0xb6>
300088e0:	2601      	movs	r6, #1
300088e2:	4620      	mov	r0, r4
300088e4:	9105      	str	r1, [sp, #20]
300088e6:	fa06 f202 	lsl.w	r2, r6, r2
300088ea:	4e27      	ldr	r6, [pc, #156]	; (30008988 <NandImgCopy+0x100>)
300088ec:	9204      	str	r2, [sp, #16]
300088ee:	1a52      	subs	r2, r2, r1
300088f0:	4492      	add	sl, r2
300088f2:	4415      	add	r5, r2
300088f4:	47b0      	blx	r6
300088f6:	f010 0f70 	tst.w	r0, #112	; 0x70
300088fa:	9b04      	ldr	r3, [sp, #16]
300088fc:	9905      	ldr	r1, [sp, #20]
300088fe:	d134      	bne.n	3000896a <NandImgCopy+0xe2>
30008900:	eba8 0803 	sub.w	r8, r8, r3
30008904:	4488      	add	r8, r1
30008906:	f899 606c 	ldrb.w	r6, [r9, #108]	; 0x6c
3000890a:	3f01      	subs	r7, #1
3000890c:	fa25 f406 	lsr.w	r4, r5, r6
30008910:	9b06      	ldr	r3, [sp, #24]
30008912:	4632      	mov	r2, r6
30008914:	09a6      	lsrs	r6, r4, #6
30008916:	ebb3 1f94 	cmp.w	r3, r4, lsr #6
3000891a:	d8d8      	bhi.n	300088ce <NandImgCopy+0x46>
3000891c:	e005      	b.n	3000892a <NandImgCopy+0xa2>
3000891e:	f899 306c 	ldrb.w	r3, [r9, #108]	; 0x6c
30008922:	3440      	adds	r4, #64	; 0x40
30008924:	fa0b f303 	lsl.w	r3, fp, r3
30008928:	441d      	add	r5, r3
3000892a:	4630      	mov	r0, r6
3000892c:	3601      	adds	r6, #1
3000892e:	f7ff fef3 	bl	30008718 <NAND_CHECK_IS_BAD_BLOCK>
30008932:	2800      	cmp	r0, #0
30008934:	d1f3      	bne.n	3000891e <NandImgCopy+0x96>
30008936:	f899 206c 	ldrb.w	r2, [r9, #108]	; 0x6c
3000893a:	9606      	str	r6, [sp, #24]
3000893c:	e7c7      	b.n	300088ce <NandImgCopy+0x46>
3000893e:	4642      	mov	r2, r8
30008940:	4620      	mov	r0, r4
30008942:	4d11      	ldr	r5, [pc, #68]	; (30008988 <NandImgCopy+0x100>)
30008944:	47a8      	blx	r5
30008946:	f010 0f70 	tst.w	r0, #112	; 0x70
3000894a:	d00b      	beq.n	30008964 <NandImgCopy+0xdc>
3000894c:	09a1      	lsrs	r1, r4, #6
3000894e:	4b0f      	ldr	r3, [pc, #60]	; (3000898c <NandImgCopy+0x104>)
30008950:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008954:	4a0e      	ldr	r2, [pc, #56]	; (30008990 <NandImgCopy+0x108>)
30008956:	e9cd 1001 	strd	r1, r0, [sp, #4]
3000895a:	9400      	str	r4, [sp, #0]
3000895c:	4619      	mov	r1, r3
3000895e:	2004      	movs	r0, #4
30008960:	f003 fac4 	bl	3000beec <rtk_log_write>
30008964:	b009      	add	sp, #36	; 0x24
30008966:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000896a:	1ac9      	subs	r1, r1, r3
3000896c:	09a3      	lsrs	r3, r4, #6
3000896e:	f004 043f 	and.w	r4, r4, #63	; 0x3f
30008972:	9002      	str	r0, [sp, #8]
30008974:	9301      	str	r3, [sp, #4]
30008976:	4488      	add	r8, r1
30008978:	4b04      	ldr	r3, [pc, #16]	; (3000898c <NandImgCopy+0x104>)
3000897a:	2004      	movs	r0, #4
3000897c:	4a04      	ldr	r2, [pc, #16]	; (30008990 <NandImgCopy+0x108>)
3000897e:	4619      	mov	r1, r3
30008980:	9400      	str	r4, [sp, #0]
30008982:	f003 fab3 	bl	3000beec <rtk_log_write>
30008986:	e7be      	b.n	30008906 <NandImgCopy+0x7e>
30008988:	30009b05 	.word	0x30009b05
3000898c:	3000d69c 	.word	0x3000d69c
30008990:	3000d6c0 	.word	0x3000d6c0
30008994:	2001c01c 	.word	0x2001c01c

30008998 <OTP_Read8.part.0>:
30008998:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000899c:	4b30      	ldr	r3, [pc, #192]	; (30008a60 <OTP_Read8.part.0+0xc8>)
3000899e:	4680      	mov	r8, r0
300089a0:	e843 f300 	tt	r3, r3
300089a4:	f3c3 5380 	ubfx	r3, r3, #22, #1
300089a8:	4a2e      	ldr	r2, [pc, #184]	; (30008a64 <OTP_Read8.part.0+0xcc>)
300089aa:	4e2f      	ldr	r6, [pc, #188]	; (30008a68 <OTP_Read8.part.0+0xd0>)
300089ac:	b082      	sub	sp, #8
300089ae:	2b00      	cmp	r3, #0
300089b0:	4689      	mov	r9, r1
300089b2:	bf0e      	itee	eq
300089b4:	f04f 4784 	moveq.w	r7, #1107296256	; 0x42000000
300089b8:	f04f 47a4 	movne.w	r7, #1375731712	; 0x52000000
300089bc:	4616      	movne	r6, r2
300089be:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300089c0:	05d8      	lsls	r0, r3, #23
300089c2:	d505      	bpl.n	300089d0 <OTP_Read8.part.0+0x38>
300089c4:	4d29      	ldr	r5, [pc, #164]	; (30008a6c <OTP_Read8.part.0+0xd4>)
300089c6:	2064      	movs	r0, #100	; 0x64
300089c8:	47a8      	blx	r5
300089ca:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300089cc:	05d9      	lsls	r1, r3, #23
300089ce:	d4fa      	bmi.n	300089c6 <OTP_Read8.part.0+0x2e>
300089d0:	6c3b      	ldr	r3, [r7, #64]	; 0x40
300089d2:	f443 7380 	orr.w	r3, r3, #256	; 0x100
300089d6:	643b      	str	r3, [r7, #64]	; 0x40
300089d8:	4b25      	ldr	r3, [pc, #148]	; (30008a70 <OTP_Read8.part.0+0xd8>)
300089da:	681a      	ldr	r2, [r3, #0]
300089dc:	07d2      	lsls	r2, r2, #31
300089de:	d403      	bmi.n	300089e8 <OTP_Read8.part.0+0x50>
300089e0:	681a      	ldr	r2, [r3, #0]
300089e2:	f042 0201 	orr.w	r2, r2, #1
300089e6:	601a      	str	r2, [r3, #0]
300089e8:	f04f 4184 	mov.w	r1, #1107296256	; 0x42000000
300089ec:	ea4f 2308 	mov.w	r3, r8, lsl #8
300089f0:	694a      	ldr	r2, [r1, #20]
300089f2:	f022 427f 	bic.w	r2, r2, #4278190080	; 0xff000000
300089f6:	614a      	str	r2, [r1, #20]
300089f8:	6033      	str	r3, [r6, #0]
300089fa:	6833      	ldr	r3, [r6, #0]
300089fc:	2b00      	cmp	r3, #0
300089fe:	db10      	blt.n	30008a22 <OTP_Read8.part.0+0x8a>
30008a00:	2400      	movs	r4, #0
30008a02:	4d1a      	ldr	r5, [pc, #104]	; (30008a6c <OTP_Read8.part.0+0xd4>)
30008a04:	f644 6a20 	movw	sl, #20000	; 0x4e20
30008a08:	e001      	b.n	30008a0e <OTP_Read8.part.0+0x76>
30008a0a:	4554      	cmp	r4, sl
30008a0c:	d01a      	beq.n	30008a44 <OTP_Read8.part.0+0xac>
30008a0e:	2005      	movs	r0, #5
30008a10:	3401      	adds	r4, #1
30008a12:	47a8      	blx	r5
30008a14:	6833      	ldr	r3, [r6, #0]
30008a16:	2b00      	cmp	r3, #0
30008a18:	daf7      	bge.n	30008a0a <OTP_Read8.part.0+0x72>
30008a1a:	f644 6320 	movw	r3, #20000	; 0x4e20
30008a1e:	429c      	cmp	r4, r3
30008a20:	d010      	beq.n	30008a44 <OTP_Read8.part.0+0xac>
30008a22:	6833      	ldr	r3, [r6, #0]
30008a24:	2001      	movs	r0, #1
30008a26:	f889 3000 	strb.w	r3, [r9]
30008a2a:	f04f 4284 	mov.w	r2, #1107296256	; 0x42000000
30008a2e:	6953      	ldr	r3, [r2, #20]
30008a30:	f023 437f 	bic.w	r3, r3, #4278190080	; 0xff000000
30008a34:	6153      	str	r3, [r2, #20]
30008a36:	6c3b      	ldr	r3, [r7, #64]	; 0x40
30008a38:	f423 7380 	bic.w	r3, r3, #256	; 0x100
30008a3c:	643b      	str	r3, [r7, #64]	; 0x40
30008a3e:	b002      	add	sp, #8
30008a40:	e8bd 87f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, pc}
30008a44:	22ff      	movs	r2, #255	; 0xff
30008a46:	4b0b      	ldr	r3, [pc, #44]	; (30008a74 <OTP_Read8.part.0+0xdc>)
30008a48:	2002      	movs	r0, #2
30008a4a:	f8cd 8000 	str.w	r8, [sp]
30008a4e:	4619      	mov	r1, r3
30008a50:	f889 2000 	strb.w	r2, [r9]
30008a54:	4a08      	ldr	r2, [pc, #32]	; (30008a78 <OTP_Read8.part.0+0xe0>)
30008a56:	f003 fa49 	bl	3000beec <rtk_log_write>
30008a5a:	2000      	movs	r0, #0
30008a5c:	e7e5      	b.n	30008a2a <OTP_Read8.part.0+0x92>
30008a5e:	bf00      	nop
30008a60:	0000e5f9 	.word	0x0000e5f9
30008a64:	5200000c 	.word	0x5200000c
30008a68:	42000008 	.word	0x42000008
30008a6c:	00009b2d 	.word	0x00009b2d
30008a70:	42008000 	.word	0x42008000
30008a74:	3000d6fc 	.word	0x3000d6fc
30008a78:	3000d700 	.word	0x3000d700

30008a7c <OTP_Read8>:
30008a7c:	f5b0 6f00 	cmp.w	r0, #2048	; 0x800
30008a80:	d201      	bcs.n	30008a86 <OTP_Read8+0xa>
30008a82:	f7ff bf89 	b.w	30008998 <OTP_Read8.part.0>
30008a86:	22ff      	movs	r2, #255	; 0xff
30008a88:	2000      	movs	r0, #0
30008a8a:	700a      	strb	r2, [r1, #0]
30008a8c:	4770      	bx	lr
30008a8e:	bf00      	nop

30008a90 <PLL_NP_ClkSet>:
30008a90:	b570      	push	{r4, r5, r6, lr}
30008a92:	4604      	mov	r4, r0
30008a94:	4b1b      	ldr	r3, [pc, #108]	; (30008b04 <PLL_NP_ClkSet+0x74>)
30008a96:	4798      	blx	r3
30008a98:	4d1b      	ldr	r5, [pc, #108]	; (30008b08 <PLL_NP_ClkSet+0x78>)
30008a9a:	08c2      	lsrs	r2, r0, #3
30008a9c:	fbb4 f3f0 	udiv	r3, r4, r0
30008aa0:	fb00 4c13 	mls	ip, r0, r3, r4
30008aa4:	1e9c      	subs	r4, r3, #2
30008aa6:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008aa8:	0164      	lsls	r4, r4, #5
30008aaa:	f423 63fc 	bic.w	r3, r3, #2016	; 0x7e0
30008aae:	f404 64fc 	and.w	r4, r4, #2016	; 0x7e0
30008ab2:	431c      	orrs	r4, r3
30008ab4:	fbbc f3f2 	udiv	r3, ip, r2
30008ab8:	646c      	str	r4, [r5, #68]	; 0x44
30008aba:	041c      	lsls	r4, r3, #16
30008abc:	fb02 c013 	mls	r0, r2, r3, ip
30008ac0:	2300      	movs	r3, #0
30008ac2:	6cee      	ldr	r6, [r5, #76]	; 0x4c
30008ac4:	f404 24e0 	and.w	r4, r4, #458752	; 0x70000
30008ac8:	4619      	mov	r1, r3
30008aca:	fa1f fc86 	uxth.w	ip, r6
30008ace:	4e0f      	ldr	r6, [pc, #60]	; (30008b0c <PLL_NP_ClkSet+0x7c>)
30008ad0:	ea50 314f 	orrs.w	r1, r0, pc, lsl #13
30008ad4:	ea44 040c 	orr.w	r4, r4, ip
30008ad8:	f003 fa92 	bl	3000c000 <__aeabi_uldivmod>
30008adc:	4603      	mov	r3, r0
30008ade:	2001      	movs	r0, #1
30008ae0:	ea44 44c3 	orr.w	r4, r4, r3, lsl #19
30008ae4:	64ec      	str	r4, [r5, #76]	; 0x4c
30008ae6:	47b0      	blx	r6
30008ae8:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008aea:	203c      	movs	r0, #60	; 0x3c
30008aec:	f443 4300 	orr.w	r3, r3, #32768	; 0x8000
30008af0:	646b      	str	r3, [r5, #68]	; 0x44
30008af2:	6c6b      	ldr	r3, [r5, #68]	; 0x44
30008af4:	f423 4300 	bic.w	r3, r3, #32768	; 0x8000
30008af8:	646b      	str	r3, [r5, #68]	; 0x44
30008afa:	4633      	mov	r3, r6
30008afc:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30008b00:	4718      	bx	r3
30008b02:	bf00      	nop
30008b04:	000099f5 	.word	0x000099f5
30008b08:	42008800 	.word	0x42008800
30008b0c:	00009b2d 	.word	0x00009b2d

30008b10 <PLL_AP_ClkSet>:
30008b10:	b510      	push	{r4, lr}
30008b12:	4604      	mov	r4, r0
30008b14:	4b0b      	ldr	r3, [pc, #44]	; (30008b44 <PLL_AP_ClkSet+0x34>)
30008b16:	4798      	blx	r3
30008b18:	fbb4 f0f0 	udiv	r0, r4, r0
30008b1c:	f1a0 0319 	sub.w	r3, r0, #25
30008b20:	1e84      	subs	r4, r0, #2
30008b22:	2b0f      	cmp	r3, #15
30008b24:	d807      	bhi.n	30008b36 <PLL_AP_ClkSet+0x26>
30008b26:	4b08      	ldr	r3, [pc, #32]	; (30008b48 <PLL_AP_ClkSet+0x38>)
30008b28:	b2e0      	uxtb	r0, r4
30008b2a:	695c      	ldr	r4, [r3, #20]
30008b2c:	f024 04ff 	bic.w	r4, r4, #255	; 0xff
30008b30:	4320      	orrs	r0, r4
30008b32:	6158      	str	r0, [r3, #20]
30008b34:	bd10      	pop	{r4, pc}
30008b36:	f240 1181 	movw	r1, #385	; 0x181
30008b3a:	4804      	ldr	r0, [pc, #16]	; (30008b4c <PLL_AP_ClkSet+0x3c>)
30008b3c:	f003 fccc 	bl	3000c4d8 <__io_assert_failed_veneer>
30008b40:	e7f1      	b.n	30008b26 <PLL_AP_ClkSet+0x16>
30008b42:	bf00      	nop
30008b44:	000099f5 	.word	0x000099f5
30008b48:	41000300 	.word	0x41000300
30008b4c:	3000d71c 	.word	0x3000d71c

30008b50 <PLL_AP>:
30008b50:	2801      	cmp	r0, #1
30008b52:	b538      	push	{r3, r4, r5, lr}
30008b54:	d009      	beq.n	30008b6a <PLL_AP+0x1a>
30008b56:	4b13      	ldr	r3, [pc, #76]	; (30008ba4 <PLL_AP+0x54>)
30008b58:	681a      	ldr	r2, [r3, #0]
30008b5a:	f022 0209 	bic.w	r2, r2, #9
30008b5e:	601a      	str	r2, [r3, #0]
30008b60:	681a      	ldr	r2, [r3, #0]
30008b62:	f022 0206 	bic.w	r2, r2, #6
30008b66:	601a      	str	r2, [r3, #0]
30008b68:	bd38      	pop	{r3, r4, r5, pc}
30008b6a:	4b0f      	ldr	r3, [pc, #60]	; (30008ba8 <PLL_AP+0x58>)
30008b6c:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008b6e:	f012 0f05 	tst.w	r2, #5
30008b72:	d00f      	beq.n	30008b94 <PLL_AP+0x44>
30008b74:	4d0d      	ldr	r5, [pc, #52]	; (30008bac <PLL_AP+0x5c>)
30008b76:	4c0b      	ldr	r4, [pc, #44]	; (30008ba4 <PLL_AP+0x54>)
30008b78:	2001      	movs	r0, #1
30008b7a:	6823      	ldr	r3, [r4, #0]
30008b7c:	f043 0306 	orr.w	r3, r3, #6
30008b80:	6023      	str	r3, [r4, #0]
30008b82:	47a8      	blx	r5
30008b84:	6823      	ldr	r3, [r4, #0]
30008b86:	f043 0309 	orr.w	r3, r3, #9
30008b8a:	6023      	str	r3, [r4, #0]
30008b8c:	6863      	ldr	r3, [r4, #4]
30008b8e:	2b00      	cmp	r3, #0
30008b90:	dafc      	bge.n	30008b8c <PLL_AP+0x3c>
30008b92:	bd38      	pop	{r3, r4, r5, pc}
30008b94:	6f9a      	ldr	r2, [r3, #120]	; 0x78
30008b96:	20a0      	movs	r0, #160	; 0xa0
30008b98:	4d04      	ldr	r5, [pc, #16]	; (30008bac <PLL_AP+0x5c>)
30008b9a:	f042 0207 	orr.w	r2, r2, #7
30008b9e:	679a      	str	r2, [r3, #120]	; 0x78
30008ba0:	47a8      	blx	r5
30008ba2:	e7e8      	b.n	30008b76 <PLL_AP+0x26>
30008ba4:	41000300 	.word	0x41000300
30008ba8:	42008800 	.word	0x42008800
30008bac:	00009b2d 	.word	0x00009b2d

30008bb0 <PSRAM_INFO_Update>:
30008bb0:	4b46      	ldr	r3, [pc, #280]	; (30008ccc <PSRAM_INFO_Update+0x11c>)
30008bb2:	4947      	ldr	r1, [pc, #284]	; (30008cd0 <PSRAM_INFO_Update+0x120>)
30008bb4:	4a47      	ldr	r2, [pc, #284]	; (30008cd4 <PSRAM_INFO_Update+0x124>)
30008bb6:	b530      	push	{r4, r5, lr}
30008bb8:	f8d3 3228 	ldr.w	r3, [r3, #552]	; 0x228
30008bbc:	b083      	sub	sp, #12
30008bbe:	680d      	ldr	r5, [r1, #0]
30008bc0:	f3c3 2302 	ubfx	r3, r3, #8, #3
30008bc4:	4c44      	ldr	r4, [pc, #272]	; (30008cd8 <PSRAM_INFO_Update+0x128>)
30008bc6:	fb02 f505 	mul.w	r5, r2, r5
30008bca:	3301      	adds	r3, #1
30008bcc:	fbb5 f5f3 	udiv	r5, r5, r3
30008bd0:	086a      	lsrs	r2, r5, #1
30008bd2:	4b42      	ldr	r3, [pc, #264]	; (30008cdc <PSRAM_INFO_Update+0x12c>)
30008bd4:	60e2      	str	r2, [r4, #12]
30008bd6:	4798      	blx	r3
30008bd8:	4b41      	ldr	r3, [pc, #260]	; (30008ce0 <PSRAM_INFO_Update+0x130>)
30008bda:	6020      	str	r0, [r4, #0]
30008bdc:	4798      	blx	r3
30008bde:	4b41      	ldr	r3, [pc, #260]	; (30008ce4 <PSRAM_INFO_Update+0x134>)
30008be0:	220a      	movs	r2, #10
30008be2:	60a0      	str	r0, [r4, #8]
30008be4:	6122      	str	r2, [r4, #16]
30008be6:	2220      	movs	r2, #32
30008be8:	fbb3 f3f5 	udiv	r3, r3, r5
30008bec:	6163      	str	r3, [r4, #20]
30008bee:	fbb2 f3f3 	udiv	r3, r2, r3
30008bf2:	6822      	ldr	r2, [r4, #0]
30008bf4:	6223      	str	r3, [r4, #32]
30008bf6:	2a01      	cmp	r2, #1
30008bf8:	68e3      	ldr	r3, [r4, #12]
30008bfa:	4a3b      	ldr	r2, [pc, #236]	; (30008ce8 <PSRAM_INFO_Update+0x138>)
30008bfc:	d01d      	beq.n	30008c3a <PSRAM_INFO_Update+0x8a>
30008bfe:	4293      	cmp	r3, r2
30008c00:	d810      	bhi.n	30008c24 <PSRAM_INFO_Update+0x74>
30008c02:	2304      	movs	r3, #4
30008c04:	2108      	movs	r1, #8
30008c06:	461a      	mov	r2, r3
30008c08:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008c0c:	4b37      	ldr	r3, [pc, #220]	; (30008cec <PSRAM_INFO_Update+0x13c>)
30008c0e:	f883 2110 	strb.w	r2, [r3, #272]	; 0x110
30008c12:	4b37      	ldr	r3, [pc, #220]	; (30008cf0 <PSRAM_INFO_Update+0x140>)
30008c14:	2004      	movs	r0, #4
30008c16:	4a37      	ldr	r2, [pc, #220]	; (30008cf4 <PSRAM_INFO_Update+0x144>)
30008c18:	4619      	mov	r1, r3
30008c1a:	9500      	str	r5, [sp, #0]
30008c1c:	f003 f966 	bl	3000beec <rtk_log_write>
30008c20:	b003      	add	sp, #12
30008c22:	bd30      	pop	{r4, r5, pc}
30008c24:	4a34      	ldr	r2, [pc, #208]	; (30008cf8 <PSRAM_INFO_Update+0x148>)
30008c26:	4293      	cmp	r3, r2
30008c28:	d916      	bls.n	30008c58 <PSRAM_INFO_Update+0xa8>
30008c2a:	4a34      	ldr	r2, [pc, #208]	; (30008cfc <PSRAM_INFO_Update+0x14c>)
30008c2c:	4293      	cmp	r3, r2
30008c2e:	d824      	bhi.n	30008c7a <PSRAM_INFO_Update+0xca>
30008c30:	2306      	movs	r3, #6
30008c32:	461a      	mov	r2, r3
30008c34:	e9c4 3306 	strd	r3, r3, [r4, #24]
30008c38:	e7e8      	b.n	30008c0c <PSRAM_INFO_Update+0x5c>
30008c3a:	4293      	cmp	r3, r2
30008c3c:	d907      	bls.n	30008c4e <PSRAM_INFO_Update+0x9e>
30008c3e:	4a2e      	ldr	r2, [pc, #184]	; (30008cf8 <PSRAM_INFO_Update+0x148>)
30008c40:	4293      	cmp	r3, r2
30008c42:	d80f      	bhi.n	30008c64 <PSRAM_INFO_Update+0xb4>
30008c44:	2205      	movs	r2, #5
30008c46:	230f      	movs	r3, #15
30008c48:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c4c:	e7e1      	b.n	30008c12 <PSRAM_INFO_Update+0x62>
30008c4e:	2204      	movs	r2, #4
30008c50:	230f      	movs	r3, #15
30008c52:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c56:	e7dc      	b.n	30008c12 <PSRAM_INFO_Update+0x62>
30008c58:	2305      	movs	r3, #5
30008c5a:	2108      	movs	r1, #8
30008c5c:	461a      	mov	r2, r3
30008c5e:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008c62:	e7d3      	b.n	30008c0c <PSRAM_INFO_Update+0x5c>
30008c64:	4a25      	ldr	r2, [pc, #148]	; (30008cfc <PSRAM_INFO_Update+0x14c>)
30008c66:	4293      	cmp	r3, r2
30008c68:	d917      	bls.n	30008c9a <PSRAM_INFO_Update+0xea>
30008c6a:	4a25      	ldr	r2, [pc, #148]	; (30008d00 <PSRAM_INFO_Update+0x150>)
30008c6c:	4293      	cmp	r3, r2
30008c6e:	d81f      	bhi.n	30008cb0 <PSRAM_INFO_Update+0x100>
30008c70:	2207      	movs	r2, #7
30008c72:	2314      	movs	r3, #20
30008c74:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008c78:	e7cb      	b.n	30008c12 <PSRAM_INFO_Update+0x62>
30008c7a:	4a21      	ldr	r2, [pc, #132]	; (30008d00 <PSRAM_INFO_Update+0x150>)
30008c7c:	4293      	cmp	r3, r2
30008c7e:	d911      	bls.n	30008ca4 <PSRAM_INFO_Update+0xf4>
30008c80:	4a20      	ldr	r2, [pc, #128]	; (30008d04 <PSRAM_INFO_Update+0x154>)
30008c82:	4293      	cmp	r3, r2
30008c84:	bf8c      	ite	hi
30008c86:	2300      	movhi	r3, #0
30008c88:	2301      	movls	r3, #1
30008c8a:	2804      	cmp	r0, #4
30008c8c:	d100      	bne.n	30008c90 <PSRAM_INFO_Update+0xe0>
30008c8e:	b94b      	cbnz	r3, 30008ca4 <PSRAM_INFO_Update+0xf4>
30008c90:	2803      	cmp	r0, #3
30008c92:	d100      	bne.n	30008c96 <PSRAM_INFO_Update+0xe6>
30008c94:	b9a3      	cbnz	r3, 30008cc0 <PSRAM_INFO_Update+0x110>
30008c96:	7e22      	ldrb	r2, [r4, #24]
30008c98:	e7b8      	b.n	30008c0c <PSRAM_INFO_Update+0x5c>
30008c9a:	2206      	movs	r2, #6
30008c9c:	2312      	movs	r3, #18
30008c9e:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008ca2:	e7b6      	b.n	30008c12 <PSRAM_INFO_Update+0x62>
30008ca4:	2307      	movs	r3, #7
30008ca6:	2106      	movs	r1, #6
30008ca8:	461a      	mov	r2, r3
30008caa:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008cae:	e7ad      	b.n	30008c0c <PSRAM_INFO_Update+0x5c>
30008cb0:	4a14      	ldr	r2, [pc, #80]	; (30008d04 <PSRAM_INFO_Update+0x154>)
30008cb2:	4293      	cmp	r3, r2
30008cb4:	d8ad      	bhi.n	30008c12 <PSRAM_INFO_Update+0x62>
30008cb6:	2208      	movs	r2, #8
30008cb8:	231c      	movs	r3, #28
30008cba:	e9c4 2306 	strd	r2, r3, [r4, #24]
30008cbe:	e7a8      	b.n	30008c12 <PSRAM_INFO_Update+0x62>
30008cc0:	230a      	movs	r3, #10
30008cc2:	2106      	movs	r1, #6
30008cc4:	461a      	mov	r2, r3
30008cc6:	e9c4 3106 	strd	r3, r1, [r4, #24]
30008cca:	e79f      	b.n	30008c0c <PSRAM_INFO_Update+0x5c>
30008ccc:	42008000 	.word	0x42008000
30008cd0:	3000ef24 	.word	0x3000ef24
30008cd4:	000f4240 	.word	0x000f4240
30008cd8:	3000fc60 	.word	0x3000fc60
30008cdc:	30007e09 	.word	0x30007e09
30008ce0:	30007dd1 	.word	0x30007dd1
30008ce4:	3b9aca00 	.word	0x3b9aca00
30008ce8:	0632ea00 	.word	0x0632ea00
30008cec:	23020000 	.word	0x23020000
30008cf0:	3000d72c 	.word	0x3000d72c
30008cf4:	3000d734 	.word	0x3000d734
30008cf8:	07ed6b40 	.word	0x07ed6b40
30008cfc:	09e4f580 	.word	0x09e4f580
30008d00:	0bebc200 	.word	0x0bebc200
30008d04:	0ee6b280 	.word	0x0ee6b280

30008d08 <PSRAM_PHY_StructInit>:
30008d08:	2203      	movs	r2, #3
30008d0a:	2301      	movs	r3, #1
30008d0c:	210a      	movs	r1, #10
30008d0e:	b410      	push	{r4}
30008d10:	e9c0 2202 	strd	r2, r2, [r0, #8]
30008d14:	2402      	movs	r4, #2
30008d16:	2207      	movs	r2, #7
30008d18:	e9c0 3400 	strd	r3, r4, [r0]
30008d1c:	e9c0 1304 	strd	r1, r3, [r0, #16]
30008d20:	f85d 4b04 	ldr.w	r4, [sp], #4
30008d24:	6182      	str	r2, [r0, #24]
30008d26:	4770      	bx	lr

30008d28 <PSRAM_PHY_Init>:
30008d28:	b538      	push	{r3, r4, r5, lr}
30008d2a:	4604      	mov	r4, r0
30008d2c:	6903      	ldr	r3, [r0, #16]
30008d2e:	4d14      	ldr	r5, [pc, #80]	; (30008d80 <PSRAM_PHY_Init+0x58>)
30008d30:	f003 031f 	and.w	r3, r3, #31
30008d34:	4913      	ldr	r1, [pc, #76]	; (30008d84 <PSRAM_PHY_Init+0x5c>)
30008d36:	e9d0 0200 	ldrd	r0, r2, [r0]
30008d3a:	0612      	lsls	r2, r2, #24
30008d3c:	ea43 7300 	orr.w	r3, r3, r0, lsl #28
30008d40:	f002 62e0 	and.w	r2, r2, #117440512	; 0x7000000
30008d44:	4313      	orrs	r3, r2
30008d46:	68a2      	ldr	r2, [r4, #8]
30008d48:	0412      	lsls	r2, r2, #16
30008d4a:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30008d4e:	4313      	orrs	r3, r2
30008d50:	68e2      	ldr	r2, [r4, #12]
30008d52:	0212      	lsls	r2, r2, #8
30008d54:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
30008d58:	4313      	orrs	r3, r2
30008d5a:	606b      	str	r3, [r5, #4]
30008d5c:	4788      	blx	r1
30008d5e:	2801      	cmp	r0, #1
30008d60:	e9d4 3105 	ldrd	r3, r1, [r4, #20]
30008d64:	bf08      	it	eq
30008d66:	4a08      	ldreq	r2, [pc, #32]	; (30008d88 <PSRAM_PHY_Init+0x60>)
30008d68:	ea4f 1303 	mov.w	r3, r3, lsl #4
30008d6c:	f001 010f 	and.w	r1, r1, #15
30008d70:	bf18      	it	ne
30008d72:	4a06      	ldrne	r2, [pc, #24]	; (30008d8c <PSRAM_PHY_Init+0x64>)
30008d74:	b2db      	uxtb	r3, r3
30008d76:	430b      	orrs	r3, r1
30008d78:	431a      	orrs	r2, r3
30008d7a:	61aa      	str	r2, [r5, #24]
30008d7c:	bd38      	pop	{r3, r4, r5, pc}
30008d7e:	bf00      	nop
30008d80:	41002000 	.word	0x41002000
30008d84:	30007e09 	.word	0x30007e09
30008d88:	80000e00 	.word	0x80000e00
30008d8c:	80000f00 	.word	0x80000f00

30008d90 <PSRAM_CTRL_Init>:
30008d90:	b470      	push	{r4, r5, r6}
30008d92:	4b4c      	ldr	r3, [pc, #304]	; (30008ec4 <PSRAM_CTRL_Init+0x134>)
30008d94:	2400      	movs	r4, #0
30008d96:	4a4c      	ldr	r2, [pc, #304]	; (30008ec8 <PSRAM_CTRL_Init+0x138>)
30008d98:	f24d 0101 	movw	r1, #53249	; 0xd001
30008d9c:	609c      	str	r4, [r3, #8]
30008d9e:	2001      	movs	r0, #1
30008da0:	601a      	str	r2, [r3, #0]
30008da2:	4a4a      	ldr	r2, [pc, #296]	; (30008ecc <PSRAM_CTRL_Init+0x13c>)
30008da4:	6158      	str	r0, [r3, #20]
30008da6:	f8c3 1120 	str.w	r1, [r3, #288]	; 0x120
30008daa:	6811      	ldr	r1, [r2, #0]
30008dac:	2902      	cmp	r1, #2
30008dae:	d04a      	beq.n	30008e46 <PSRAM_CTRL_Init+0xb6>
30008db0:	6954      	ldr	r4, [r2, #20]
30008db2:	f44f 717a 	mov.w	r1, #1000	; 0x3e8
30008db6:	69d0      	ldr	r0, [r2, #28]
30008db8:	6915      	ldr	r5, [r2, #16]
30008dba:	fbb0 f0f4 	udiv	r0, r0, r4
30008dbe:	fbb1 f1f4 	udiv	r1, r1, r4
30008dc2:	6994      	ldr	r4, [r2, #24]
30008dc4:	f005 020f 	and.w	r2, r5, #15
30008dc8:	f241 5551 	movw	r5, #5457	; 0x1551
30008dcc:	f442 628e 	orr.w	r2, r2, #1136	; 0x470
30008dd0:	f8c3 5110 	str.w	r5, [r3, #272]	; 0x110
30008dd4:	f8c3 2134 	str.w	r2, [r3, #308]	; 0x134
30008dd8:	094a      	lsrs	r2, r1, #5
30008dda:	0181      	lsls	r1, r0, #6
30008ddc:	f000 003f 	and.w	r0, r0, #63	; 0x3f
30008de0:	4d3b      	ldr	r5, [pc, #236]	; (30008ed0 <PSRAM_CTRL_Init+0x140>)
30008de2:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008de6:	ea41 6102 	orr.w	r1, r1, r2, lsl #24
30008dea:	00a2      	lsls	r2, r4, #2
30008dec:	4301      	orrs	r1, r0
30008dee:	f24a 00a0 	movw	r0, #41120	; 0xa0a0
30008df2:	3a01      	subs	r2, #1
30008df4:	430d      	orrs	r5, r1
30008df6:	f242 0120 	movw	r1, #8224	; 0x2020
30008dfa:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008dfe:	f8c3 5138 	str.w	r5, [r3, #312]	; 0x138
30008e02:	f8c3 10e0 	str.w	r1, [r3, #224]	; 0xe0
30008e06:	f44f 2180 	mov.w	r1, #262144	; 0x40000
30008e0a:	f8c3 00f4 	str.w	r0, [r3, #244]	; 0xf4
30008e0e:	f06f 4000 	mvn.w	r0, #2147483648	; 0x80000000
30008e12:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008e16:	f442 2184 	orr.w	r1, r2, #270336	; 0x42000
30008e1a:	4420      	add	r0, r4
30008e1c:	f8c3 1118 	str.w	r1, [r3, #280]	; 0x118
30008e20:	f640 71fe 	movw	r1, #4094	; 0xffe
30008e24:	f8d3 511c 	ldr.w	r5, [r3, #284]	; 0x11c
30008e28:	ea01 0140 	and.w	r1, r1, r0, lsl #1
30008e2c:	432a      	orrs	r2, r5
30008e2e:	f442 4200 	orr.w	r2, r2, #32768	; 0x8000
30008e32:	f8c3 211c 	str.w	r2, [r3, #284]	; 0x11c
30008e36:	f8c3 113c 	str.w	r1, [r3, #316]	; 0x13c
30008e3a:	4a22      	ldr	r2, [pc, #136]	; (30008ec4 <PSRAM_CTRL_Init+0x134>)
30008e3c:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008e3e:	061b      	lsls	r3, r3, #24
30008e40:	d5fc      	bpl.n	30008e3c <PSRAM_CTRL_Init+0xac>
30008e42:	bc70      	pop	{r4, r5, r6}
30008e44:	4770      	bx	lr
30008e46:	6955      	ldr	r5, [r2, #20]
30008e48:	207d      	movs	r0, #125	; 0x7d
30008e4a:	69d1      	ldr	r1, [r2, #28]
30008e4c:	26a0      	movs	r6, #160	; 0xa0
30008e4e:	6914      	ldr	r4, [r2, #16]
30008e50:	6992      	ldr	r2, [r2, #24]
30008e52:	f004 040f 	and.w	r4, r4, #15
30008e56:	0092      	lsls	r2, r2, #2
30008e58:	fbb1 f1f5 	udiv	r1, r1, r5
30008e5c:	fbb0 f5f5 	udiv	r5, r0, r5
30008e60:	f245 5051 	movw	r0, #21841	; 0x5551
30008e64:	f8c3 0110 	str.w	r0, [r3, #272]	; 0x110
30008e68:	f001 003f 	and.w	r0, r1, #63	; 0x3f
30008e6c:	0189      	lsls	r1, r1, #6
30008e6e:	ea40 6005 	orr.w	r0, r0, r5, lsl #24
30008e72:	f641 4570 	movw	r5, #7280	; 0x1c70
30008e76:	f401 617c 	and.w	r1, r1, #4032	; 0xfc0
30008e7a:	4325      	orrs	r5, r4
30008e7c:	2420      	movs	r4, #32
30008e7e:	4308      	orrs	r0, r1
30008e80:	1e51      	subs	r1, r2, #1
30008e82:	f8c3 5134 	str.w	r5, [r3, #308]	; 0x134
30008e86:	3a02      	subs	r2, #2
30008e88:	f440 1080 	orr.w	r0, r0, #1048576	; 0x100000
30008e8c:	f3c1 010b 	ubfx	r1, r1, #0, #12
30008e90:	f3c2 020b 	ubfx	r2, r2, #0, #12
30008e94:	f8c3 0138 	str.w	r0, [r3, #312]	; 0x138
30008e98:	f44f 20a0 	mov.w	r0, #327680	; 0x50000
30008e9c:	f8c3 60e0 	str.w	r6, [r3, #224]	; 0xe0
30008ea0:	f8c3 40f4 	str.w	r4, [r3, #244]	; 0xf4
30008ea4:	f8c3 011c 	str.w	r0, [r3, #284]	; 0x11c
30008ea8:	f441 20a2 	orr.w	r0, r1, #331776	; 0x51000
30008eac:	f8c3 0118 	str.w	r0, [r3, #280]	; 0x118
30008eb0:	f8d3 011c 	ldr.w	r0, [r3, #284]	; 0x11c
30008eb4:	4301      	orrs	r1, r0
30008eb6:	f441 4100 	orr.w	r1, r1, #32768	; 0x8000
30008eba:	f8c3 111c 	str.w	r1, [r3, #284]	; 0x11c
30008ebe:	f8c3 213c 	str.w	r2, [r3, #316]	; 0x13c
30008ec2:	e7ba      	b.n	30008e3a <PSRAM_CTRL_Init+0xaa>
30008ec4:	41012000 	.word	0x41012000
30008ec8:	00bfe000 	.word	0x00bfe000
30008ecc:	3000fc60 	.word	0x3000fc60
30008ed0:	00107000 	.word	0x00107000

30008ed4 <PSRAM_REG_Write>:
30008ed4:	b430      	push	{r4, r5}
30008ed6:	4c20      	ldr	r4, [pc, #128]	; (30008f58 <PSRAM_REG_Write+0x84>)
30008ed8:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008eda:	f013 0301 	ands.w	r3, r3, #1
30008ede:	d1fb      	bne.n	30008ed8 <PSRAM_REG_Write+0x4>
30008ee0:	6825      	ldr	r5, [r4, #0]
30008ee2:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008ee6:	6025      	str	r5, [r4, #0]
30008ee8:	60a3      	str	r3, [r4, #8]
30008eea:	6825      	ldr	r5, [r4, #0]
30008eec:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008ef0:	6025      	str	r5, [r4, #0]
30008ef2:	6063      	str	r3, [r4, #4]
30008ef4:	f021 437f 	bic.w	r3, r1, #4278190080	; 0xff000000
30008ef8:	f8c4 3130 	str.w	r3, [r4, #304]	; 0x130
30008efc:	23c0      	movs	r3, #192	; 0xc0
30008efe:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f02:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f06:	0e03      	lsrs	r3, r0, #24
30008f08:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f0c:	f3c0 4307 	ubfx	r3, r0, #16, #8
30008f10:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f14:	f3c0 2307 	ubfx	r3, r0, #8, #8
30008f18:	b2c0      	uxtb	r0, r0
30008f1a:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008f1e:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008f22:	b141      	cbz	r1, 30008f36 <PSRAM_REG_Write+0x62>
30008f24:	3a01      	subs	r2, #1
30008f26:	480c      	ldr	r0, [pc, #48]	; (30008f58 <PSRAM_REG_Write+0x84>)
30008f28:	4411      	add	r1, r2
30008f2a:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30008f2e:	428a      	cmp	r2, r1
30008f30:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30008f34:	d1f9      	bne.n	30008f2a <PSRAM_REG_Write+0x56>
30008f36:	4a08      	ldr	r2, [pc, #32]	; (30008f58 <PSRAM_REG_Write+0x84>)
30008f38:	2301      	movs	r3, #1
30008f3a:	6093      	str	r3, [r2, #8]
30008f3c:	6893      	ldr	r3, [r2, #8]
30008f3e:	07d9      	lsls	r1, r3, #31
30008f40:	d4fc      	bmi.n	30008f3c <PSRAM_REG_Write+0x68>
30008f42:	4a05      	ldr	r2, [pc, #20]	; (30008f58 <PSRAM_REG_Write+0x84>)
30008f44:	6a93      	ldr	r3, [r2, #40]	; 0x28
30008f46:	07db      	lsls	r3, r3, #31
30008f48:	d4fc      	bmi.n	30008f44 <PSRAM_REG_Write+0x70>
30008f4a:	6813      	ldr	r3, [r2, #0]
30008f4c:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30008f50:	6013      	str	r3, [r2, #0]
30008f52:	bc30      	pop	{r4, r5}
30008f54:	4770      	bx	lr
30008f56:	bf00      	nop
30008f58:	41012000 	.word	0x41012000

30008f5c <PSRAM_APM_DEVIC_Init>:
30008f5c:	4b12      	ldr	r3, [pc, #72]	; (30008fa8 <PSRAM_APM_DEVIC_Init+0x4c>)
30008f5e:	2102      	movs	r1, #2
30008f60:	2000      	movs	r0, #0
30008f62:	b530      	push	{r4, r5, lr}
30008f64:	4c11      	ldr	r4, [pc, #68]	; (30008fac <PSRAM_APM_DEVIC_Init+0x50>)
30008f66:	b083      	sub	sp, #12
30008f68:	699d      	ldr	r5, [r3, #24]
30008f6a:	466a      	mov	r2, sp
30008f6c:	eb04 0c05 	add.w	ip, r4, r5
30008f70:	1eeb      	subs	r3, r5, #3
30008f72:	4c0f      	ldr	r4, [pc, #60]	; (30008fb0 <PSRAM_APM_DEVIC_Init+0x54>)
30008f74:	408b      	lsls	r3, r1
30008f76:	f81c cc03 	ldrb.w	ip, [ip, #-3]
30008f7a:	f003 031c 	and.w	r3, r3, #28
30008f7e:	ea4f 1c4c 	mov.w	ip, ip, lsl #5
30008f82:	f043 0320 	orr.w	r3, r3, #32
30008f86:	fa5f fc8c 	uxtb.w	ip, ip
30008f8a:	f88d 3000 	strb.w	r3, [sp]
30008f8e:	f88d c004 	strb.w	ip, [sp, #4]
30008f92:	f88d 3001 	strb.w	r3, [sp, #1]
30008f96:	f88d c005 	strb.w	ip, [sp, #5]
30008f9a:	47a0      	blx	r4
30008f9c:	aa01      	add	r2, sp, #4
30008f9e:	2102      	movs	r1, #2
30008fa0:	2004      	movs	r0, #4
30008fa2:	47a0      	blx	r4
30008fa4:	b003      	add	sp, #12
30008fa6:	bd30      	pop	{r4, r5, pc}
30008fa8:	3000fc60 	.word	0x3000fc60
30008fac:	3000f0ec 	.word	0x3000f0ec
30008fb0:	30008ed5 	.word	0x30008ed5

30008fb4 <PSRAM_WB_REG_Write>:
30008fb4:	b430      	push	{r4, r5}
30008fb6:	4c1e      	ldr	r4, [pc, #120]	; (30009030 <PSRAM_WB_REG_Write+0x7c>)
30008fb8:	6aa3      	ldr	r3, [r4, #40]	; 0x28
30008fba:	f013 0301 	ands.w	r3, r3, #1
30008fbe:	d1fb      	bne.n	30008fb8 <PSRAM_WB_REG_Write+0x4>
30008fc0:	6825      	ldr	r5, [r4, #0]
30008fc2:	b2c0      	uxtb	r0, r0
30008fc4:	f045 4500 	orr.w	r5, r5, #2147483648	; 0x80000000
30008fc8:	6025      	str	r5, [r4, #0]
30008fca:	60a3      	str	r3, [r4, #8]
30008fcc:	6825      	ldr	r5, [r4, #0]
30008fce:	f425 7540 	bic.w	r5, r5, #768	; 0x300
30008fd2:	6025      	str	r5, [r4, #0]
30008fd4:	f021 457f 	bic.w	r5, r1, #4278190080	; 0xff000000
30008fd8:	6063      	str	r3, [r4, #4]
30008fda:	f8c4 5130 	str.w	r5, [r4, #304]	; 0x130
30008fde:	2560      	movs	r5, #96	; 0x60
30008fe0:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30008fe4:	2501      	movs	r5, #1
30008fe6:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008fea:	f884 5060 	strb.w	r5, [r4, #96]	; 0x60
30008fee:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008ff2:	f884 3060 	strb.w	r3, [r4, #96]	; 0x60
30008ff6:	f884 0060 	strb.w	r0, [r4, #96]	; 0x60
30008ffa:	b141      	cbz	r1, 3000900e <PSRAM_WB_REG_Write+0x5a>
30008ffc:	3a01      	subs	r2, #1
30008ffe:	480c      	ldr	r0, [pc, #48]	; (30009030 <PSRAM_WB_REG_Write+0x7c>)
30009000:	4411      	add	r1, r2
30009002:	f812 3f01 	ldrb.w	r3, [r2, #1]!
30009006:	428a      	cmp	r2, r1
30009008:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
3000900c:	d1f9      	bne.n	30009002 <PSRAM_WB_REG_Write+0x4e>
3000900e:	4a08      	ldr	r2, [pc, #32]	; (30009030 <PSRAM_WB_REG_Write+0x7c>)
30009010:	2301      	movs	r3, #1
30009012:	6093      	str	r3, [r2, #8]
30009014:	6893      	ldr	r3, [r2, #8]
30009016:	07d9      	lsls	r1, r3, #31
30009018:	d4fc      	bmi.n	30009014 <PSRAM_WB_REG_Write+0x60>
3000901a:	4a05      	ldr	r2, [pc, #20]	; (30009030 <PSRAM_WB_REG_Write+0x7c>)
3000901c:	6a93      	ldr	r3, [r2, #40]	; 0x28
3000901e:	07db      	lsls	r3, r3, #31
30009020:	d4fc      	bmi.n	3000901c <PSRAM_WB_REG_Write+0x68>
30009022:	6813      	ldr	r3, [r2, #0]
30009024:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009028:	6013      	str	r3, [r2, #0]
3000902a:	bc30      	pop	{r4, r5}
3000902c:	4770      	bx	lr
3000902e:	bf00      	nop
30009030:	41012000 	.word	0x41012000

30009034 <PSRAM_WB_DEVIC_Init>:
30009034:	490b      	ldr	r1, [pc, #44]	; (30009064 <PSRAM_WB_DEVIC_Init+0x30>)
30009036:	4b0c      	ldr	r3, [pc, #48]	; (30009068 <PSRAM_WB_DEVIC_Init+0x34>)
30009038:	b510      	push	{r4, lr}
3000903a:	6988      	ldr	r0, [r1, #24]
3000903c:	b082      	sub	sp, #8
3000903e:	248f      	movs	r4, #143	; 0x8f
30009040:	2102      	movs	r1, #2
30009042:	4403      	add	r3, r0
30009044:	aa01      	add	r2, sp, #4
30009046:	f88d 4004 	strb.w	r4, [sp, #4]
3000904a:	2000      	movs	r0, #0
3000904c:	f813 3c03 	ldrb.w	r3, [r3, #-3]
30009050:	4c06      	ldr	r4, [pc, #24]	; (3000906c <PSRAM_WB_DEVIC_Init+0x38>)
30009052:	011b      	lsls	r3, r3, #4
30009054:	f043 030c 	orr.w	r3, r3, #12
30009058:	f88d 3005 	strb.w	r3, [sp, #5]
3000905c:	47a0      	blx	r4
3000905e:	b002      	add	sp, #8
30009060:	bd10      	pop	{r4, pc}
30009062:	bf00      	nop
30009064:	3000fc60 	.word	0x3000fc60
30009068:	3000f10c 	.word	0x3000f10c
3000906c:	30008fb5 	.word	0x30008fb5

30009070 <PSRAM_calibration>:
30009070:	4a9c      	ldr	r2, [pc, #624]	; (300092e4 <PSRAM_calibration+0x274>)
30009072:	2300      	movs	r3, #0
30009074:	499c      	ldr	r1, [pc, #624]	; (300092e8 <PSRAM_calibration+0x278>)
30009076:	489d      	ldr	r0, [pc, #628]	; (300092ec <PSRAM_calibration+0x27c>)
30009078:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000907c:	6852      	ldr	r2, [r2, #4]
3000907e:	b093      	sub	sp, #76	; 0x4c
30009080:	e9cd 230b 	strd	r2, r3, [sp, #44]	; 0x2c
30009084:	400a      	ands	r2, r1
30009086:	9311      	str	r3, [sp, #68]	; 0x44
30009088:	e9cd 330d 	strd	r3, r3, [sp, #52]	; 0x34
3000908c:	e9cd 330f 	strd	r3, r3, [sp, #60]	; 0x3c
30009090:	920a      	str	r2, [sp, #40]	; 0x28
30009092:	6942      	ldr	r2, [r0, #20]
30009094:	03d1      	lsls	r1, r2, #15
30009096:	d51e      	bpl.n	300090d6 <PSRAM_calibration+0x66>
30009098:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
3000909c:	f3bf 8f4f 	dsb	sy
300090a0:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300090a4:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300090a8:	f3c5 344e 	ubfx	r4, r5, #13, #15
300090ac:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300090b0:	0164      	lsls	r4, r4, #5
300090b2:	ea04 0106 	and.w	r1, r4, r6
300090b6:	462b      	mov	r3, r5
300090b8:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
300090bc:	3b01      	subs	r3, #1
300090be:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
300090c2:	1c5a      	adds	r2, r3, #1
300090c4:	d1f8      	bne.n	300090b8 <PSRAM_calibration+0x48>
300090c6:	3c20      	subs	r4, #32
300090c8:	f114 0f20 	cmn.w	r4, #32
300090cc:	d1f1      	bne.n	300090b2 <PSRAM_calibration+0x42>
300090ce:	f3bf 8f4f 	dsb	sy
300090d2:	f3bf 8f6f 	isb	sy
300090d6:	4885      	ldr	r0, [pc, #532]	; (300092ec <PSRAM_calibration+0x27c>)
300090d8:	2300      	movs	r3, #0
300090da:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300090de:	f3bf 8f4f 	dsb	sy
300090e2:	6943      	ldr	r3, [r0, #20]
300090e4:	f423 3380 	bic.w	r3, r3, #65536	; 0x10000
300090e8:	6143      	str	r3, [r0, #20]
300090ea:	f3bf 8f4f 	dsb	sy
300090ee:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
300090f2:	f643 76e0 	movw	r6, #16352	; 0x3fe0
300090f6:	f3c5 344e 	ubfx	r4, r5, #13, #15
300090fa:	f3c5 05c9 	ubfx	r5, r5, #3, #10
300090fe:	0164      	lsls	r4, r4, #5
30009100:	ea04 0106 	and.w	r1, r4, r6
30009104:	462b      	mov	r3, r5
30009106:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
3000910a:	3b01      	subs	r3, #1
3000910c:	1c5f      	adds	r7, r3, #1
3000910e:	f8c0 2274 	str.w	r2, [r0, #628]	; 0x274
30009112:	d1f8      	bne.n	30009106 <PSRAM_calibration+0x96>
30009114:	3c20      	subs	r4, #32
30009116:	f114 0f20 	cmn.w	r4, #32
3000911a:	d1f1      	bne.n	30009100 <PSRAM_calibration+0x90>
3000911c:	f3bf 8f4f 	dsb	sy
30009120:	f3bf 8f6f 	isb	sy
30009124:	2204      	movs	r2, #4
30009126:	461e      	mov	r6, r3
30009128:	9306      	str	r3, [sp, #24]
3000912a:	2700      	movs	r7, #0
3000912c:	9209      	str	r2, [sp, #36]	; 0x24
3000912e:	46b3      	mov	fp, r6
30009130:	4a6c      	ldr	r2, [pc, #432]	; (300092e4 <PSRAM_calibration+0x274>)
30009132:	4c6f      	ldr	r4, [pc, #444]	; (300092f0 <PSRAM_calibration+0x280>)
30009134:	6813      	ldr	r3, [r2, #0]
30009136:	f8df 91d4 	ldr.w	r9, [pc, #468]	; 3000930c <PSRAM_calibration+0x29c>
3000913a:	f023 0301 	bic.w	r3, r3, #1
3000913e:	f8df a1d0 	ldr.w	sl, [pc, #464]	; 30009310 <PSRAM_calibration+0x2a0>
30009142:	6013      	str	r3, [r2, #0]
30009144:	2301      	movs	r3, #1
30009146:	9305      	str	r3, [sp, #20]
30009148:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000914a:	9a05      	ldr	r2, [sp, #20]
3000914c:	9704      	str	r7, [sp, #16]
3000914e:	2700      	movs	r7, #0
30009150:	ea43 7802 	orr.w	r8, r3, r2, lsl #28
30009154:	463d      	mov	r5, r7
30009156:	e9cd b607 	strd	fp, r6, [sp, #28]
3000915a:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
3000915e:	465e      	mov	r6, fp
30009160:	e010      	b.n	30009184 <PSRAM_calibration+0x114>
30009162:	f1b6 3fff 	cmp.w	r6, #4294967295	; 0xffffffff
30009166:	bf08      	it	eq
30009168:	462e      	moveq	r6, r5
3000916a:	2d1f      	cmp	r5, #31
3000916c:	eb06 0b07 	add.w	fp, r6, r7
30009170:	f107 0701 	add.w	r7, r7, #1
30009174:	d102      	bne.n	3000917c <PSRAM_calibration+0x10c>
30009176:	9b04      	ldr	r3, [sp, #16]
30009178:	429f      	cmp	r7, r3
3000917a:	dc46      	bgt.n	3000920a <PSRAM_calibration+0x19a>
3000917c:	3501      	adds	r5, #1
3000917e:	2d20      	cmp	r5, #32
30009180:	f000 80ac 	beq.w	300092dc <PSRAM_calibration+0x26c>
30009184:	ea48 0205 	orr.w	r2, r8, r5
30009188:	4b56      	ldr	r3, [pc, #344]	; (300092e4 <PSRAM_calibration+0x274>)
3000918a:	f04f 4ec0 	mov.w	lr, #1610612736	; 0x60000000
3000918e:	4959      	ldr	r1, [pc, #356]	; (300092f4 <PSRAM_calibration+0x284>)
30009190:	605a      	str	r2, [r3, #4]
30009192:	6822      	ldr	r2, [r4, #0]
30009194:	4b58      	ldr	r3, [pc, #352]	; (300092f8 <PSRAM_calibration+0x288>)
30009196:	f8ce 2000 	str.w	r2, [lr]
3000919a:	6860      	ldr	r0, [r4, #4]
3000919c:	4a57      	ldr	r2, [pc, #348]	; (300092fc <PSRAM_calibration+0x28c>)
3000919e:	f8c9 0000 	str.w	r0, [r9]
300091a2:	68a0      	ldr	r0, [r4, #8]
300091a4:	f8ca 0000 	str.w	r0, [sl]
300091a8:	68e0      	ldr	r0, [r4, #12]
300091aa:	6018      	str	r0, [r3, #0]
300091ac:	6920      	ldr	r0, [r4, #16]
300091ae:	6008      	str	r0, [r1, #0]
300091b0:	6960      	ldr	r0, [r4, #20]
300091b2:	6010      	str	r0, [r2, #0]
300091b4:	f8de 0000 	ldr.w	r0, [lr]
300091b8:	900c      	str	r0, [sp, #48]	; 0x30
300091ba:	f8d9 0000 	ldr.w	r0, [r9]
300091be:	900d      	str	r0, [sp, #52]	; 0x34
300091c0:	f8da 0000 	ldr.w	r0, [sl]
300091c4:	900e      	str	r0, [sp, #56]	; 0x38
300091c6:	6818      	ldr	r0, [r3, #0]
300091c8:	4b4d      	ldr	r3, [pc, #308]	; (30009300 <PSRAM_calibration+0x290>)
300091ca:	900f      	str	r0, [sp, #60]	; 0x3c
300091cc:	a80c      	add	r0, sp, #48	; 0x30
300091ce:	6809      	ldr	r1, [r1, #0]
300091d0:	9110      	str	r1, [sp, #64]	; 0x40
300091d2:	6812      	ldr	r2, [r2, #0]
300091d4:	4946      	ldr	r1, [pc, #280]	; (300092f0 <PSRAM_calibration+0x280>)
300091d6:	9211      	str	r2, [sp, #68]	; 0x44
300091d8:	2218      	movs	r2, #24
300091da:	4798      	blx	r3
300091dc:	2800      	cmp	r0, #0
300091de:	d0c0      	beq.n	30009162 <PSRAM_calibration+0xf2>
300091e0:	1c70      	adds	r0, r6, #1
300091e2:	d0cb      	beq.n	3000917c <PSRAM_calibration+0x10c>
300091e4:	9b04      	ldr	r3, [sp, #16]
300091e6:	42bb      	cmp	r3, r7
300091e8:	da0a      	bge.n	30009200 <PSRAM_calibration+0x190>
300091ea:	9b05      	ldr	r3, [sp, #20]
300091ec:	f8cd b01c 	str.w	fp, [sp, #28]
300091f0:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
300091f4:	9704      	str	r7, [sp, #16]
300091f6:	2700      	movs	r7, #0
300091f8:	9608      	str	r6, [sp, #32]
300091fa:	465e      	mov	r6, fp
300091fc:	9306      	str	r3, [sp, #24]
300091fe:	e7bd      	b.n	3000917c <PSRAM_calibration+0x10c>
30009200:	f04f 3bff 	mov.w	fp, #4294967295	; 0xffffffff
30009204:	2700      	movs	r7, #0
30009206:	465e      	mov	r6, fp
30009208:	e7b8      	b.n	3000917c <PSRAM_calibration+0x10c>
3000920a:	9b05      	ldr	r3, [sp, #20]
3000920c:	9306      	str	r3, [sp, #24]
3000920e:	9b05      	ldr	r3, [sp, #20]
30009210:	005b      	lsls	r3, r3, #1
30009212:	9305      	str	r3, [sp, #20]
30009214:	9b09      	ldr	r3, [sp, #36]	; 0x24
30009216:	3b01      	subs	r3, #1
30009218:	9309      	str	r3, [sp, #36]	; 0x24
3000921a:	d195      	bne.n	30009148 <PSRAM_calibration+0xd8>
3000921c:	9b06      	ldr	r3, [sp, #24]
3000921e:	2004      	movs	r0, #4
30009220:	4a38      	ldr	r2, [pc, #224]	; (30009304 <PSRAM_calibration+0x294>)
30009222:	e9cd 7302 	strd	r7, r3, [sp, #8]
30009226:	4b38      	ldr	r3, [pc, #224]	; (30009308 <PSRAM_calibration+0x298>)
30009228:	e9cd 6b00 	strd	r6, fp, [sp]
3000922c:	4619      	mov	r1, r3
3000922e:	f002 fe5d 	bl	3000beec <rtk_log_write>
30009232:	482e      	ldr	r0, [pc, #184]	; (300092ec <PSRAM_calibration+0x27c>)
30009234:	6943      	ldr	r3, [r0, #20]
30009236:	03d9      	lsls	r1, r3, #15
30009238:	d425      	bmi.n	30009286 <PSRAM_calibration+0x216>
3000923a:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000923c:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
30009240:	f3bf 8f4f 	dsb	sy
30009244:	f8d0 5080 	ldr.w	r5, [r0, #128]	; 0x80
30009248:	f643 7ce0 	movw	ip, #16352	; 0x3fe0
3000924c:	f3c5 344e 	ubfx	r4, r5, #13, #15
30009250:	f3c5 05c9 	ubfx	r5, r5, #3, #10
30009254:	0164      	lsls	r4, r4, #5
30009256:	ea04 010c 	and.w	r1, r4, ip
3000925a:	462b      	mov	r3, r5
3000925c:	ea41 7283 	orr.w	r2, r1, r3, lsl #30
30009260:	3b01      	subs	r3, #1
30009262:	f8c0 2260 	str.w	r2, [r0, #608]	; 0x260
30009266:	1c5a      	adds	r2, r3, #1
30009268:	d1f8      	bne.n	3000925c <PSRAM_calibration+0x1ec>
3000926a:	3c20      	subs	r4, #32
3000926c:	f114 0f20 	cmn.w	r4, #32
30009270:	d1f1      	bne.n	30009256 <PSRAM_calibration+0x1e6>
30009272:	f3bf 8f4f 	dsb	sy
30009276:	6943      	ldr	r3, [r0, #20]
30009278:	f443 3380 	orr.w	r3, r3, #65536	; 0x10000
3000927c:	6143      	str	r3, [r0, #20]
3000927e:	f3bf 8f4f 	dsb	sy
30009282:	f3bf 8f6f 	isb	sy
30009286:	2f08      	cmp	r7, #8
30009288:	dd24      	ble.n	300092d4 <PSRAM_calibration+0x264>
3000928a:	ebbb 0306 	subs.w	r3, fp, r6
3000928e:	9806      	ldr	r0, [sp, #24]
30009290:	bf48      	it	mi
30009292:	3301      	addmi	r3, #1
30009294:	eb16 060b 	adds.w	r6, r6, fp
30009298:	ea4f 0363 	mov.w	r3, r3, asr #1
3000929c:	bf48      	it	mi
3000929e:	3601      	addmi	r6, #1
300092a0:	1e9a      	subs	r2, r3, #2
300092a2:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
300092a4:	f003 617f 	and.w	r1, r3, #267386880	; 0xff00000
300092a8:	0414      	lsls	r4, r2, #16
300092aa:	f3c6 0344 	ubfx	r3, r6, #1, #5
300092ae:	0212      	lsls	r2, r2, #8
300092b0:	ea41 7100 	orr.w	r1, r1, r0, lsl #28
300092b4:	f404 2470 	and.w	r4, r4, #983040	; 0xf0000
300092b8:	f402 6270 	and.w	r2, r2, #3840	; 0xf00
300092bc:	2001      	movs	r0, #1
300092be:	430b      	orrs	r3, r1
300092c0:	4908      	ldr	r1, [pc, #32]	; (300092e4 <PSRAM_calibration+0x274>)
300092c2:	4323      	orrs	r3, r4
300092c4:	4313      	orrs	r3, r2
300092c6:	604b      	str	r3, [r1, #4]
300092c8:	680b      	ldr	r3, [r1, #0]
300092ca:	4303      	orrs	r3, r0
300092cc:	600b      	str	r3, [r1, #0]
300092ce:	b013      	add	sp, #76	; 0x4c
300092d0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300092d4:	2000      	movs	r0, #0
300092d6:	b013      	add	sp, #76	; 0x4c
300092d8:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
300092dc:	9f04      	ldr	r7, [sp, #16]
300092de:	e9dd b607 	ldrd	fp, r6, [sp, #28]
300092e2:	e794      	b.n	3000920e <PSRAM_calibration+0x19e>
300092e4:	41002000 	.word	0x41002000
300092e8:	0fffffe0 	.word	0x0fffffe0
300092ec:	e000ed00 	.word	0xe000ed00
300092f0:	3000f0f4 	.word	0x3000f0f4
300092f4:	60200000 	.word	0x60200000
300092f8:	60150000 	.word	0x60150000
300092fc:	60250000 	.word	0x60250000
30009300:	000129bd 	.word	0x000129bd
30009304:	3000d754 	.word	0x3000d754
30009308:	3000d72c 	.word	0x3000d72c
3000930c:	60050000 	.word	0x60050000
30009310:	60100000 	.word	0x60100000

30009314 <PSRAM_AutoGating>:
30009314:	b1c0      	cbz	r0, 30009348 <PSRAM_AutoGating+0x34>
30009316:	f04f 4382 	mov.w	r3, #1090519040	; 0x41000000
3000931a:	4810      	ldr	r0, [pc, #64]	; (3000935c <PSRAM_AutoGating+0x48>)
3000931c:	f3c1 0108 	ubfx	r1, r1, #0, #9
30009320:	0252      	lsls	r2, r2, #9
30009322:	b410      	push	{r4}
30009324:	f8d3 41e4 	ldr.w	r4, [r3, #484]	; 0x1e4
30009328:	f402 5278 	and.w	r2, r2, #15872	; 0x3e00
3000932c:	4020      	ands	r0, r4
3000932e:	4301      	orrs	r1, r0
30009330:	430a      	orrs	r2, r1
30009332:	f8c3 21e4 	str.w	r2, [r3, #484]	; 0x1e4
30009336:	f8d3 21e0 	ldr.w	r2, [r3, #480]	; 0x1e0
3000933a:	f042 0201 	orr.w	r2, r2, #1
3000933e:	f8c3 21e0 	str.w	r2, [r3, #480]	; 0x1e0
30009342:	f85d 4b04 	ldr.w	r4, [sp], #4
30009346:	4770      	bx	lr
30009348:	f04f 4282 	mov.w	r2, #1090519040	; 0x41000000
3000934c:	f8d2 31e0 	ldr.w	r3, [r2, #480]	; 0x1e0
30009350:	f023 0301 	bic.w	r3, r3, #1
30009354:	f8c2 31e0 	str.w	r3, [r2, #480]	; 0x1e0
30009358:	4770      	bx	lr
3000935a:	bf00      	nop
3000935c:	ffffc000 	.word	0xffffc000

30009360 <BOOT_Reason>:
30009360:	4b02      	ldr	r3, [pc, #8]	; (3000936c <BOOT_Reason+0xc>)
30009362:	f8b3 0064 	ldrh.w	r0, [r3, #100]	; 0x64
30009366:	b280      	uxth	r0, r0
30009368:	4770      	bx	lr
3000936a:	bf00      	nop
3000936c:	42008200 	.word	0x42008200

30009370 <CRYPTO_SetSecurityModeAD>:
30009370:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009374:	b083      	sub	sp, #12
30009376:	460d      	mov	r5, r1
30009378:	4616      	mov	r6, r2
3000937a:	4699      	mov	r9, r3
3000937c:	4604      	mov	r4, r0
3000937e:	e9dd 870b 	ldrd	r8, r7, [sp, #44]	; 0x2c
30009382:	2800      	cmp	r0, #0
30009384:	f000 80a9 	beq.w	300094da <CRYPTO_SetSecurityModeAD+0x16a>
30009388:	7823      	ldrb	r3, [r4, #0]
3000938a:	2b01      	cmp	r3, #1
3000938c:	d003      	beq.n	30009396 <CRYPTO_SetSecurityModeAD+0x26>
3000938e:	2125      	movs	r1, #37	; 0x25
30009390:	485d      	ldr	r0, [pc, #372]	; (30009508 <CRYPTO_SetSecurityModeAD+0x198>)
30009392:	f003 f8a1 	bl	3000c4d8 <__io_assert_failed_veneer>
30009396:	2300      	movs	r3, #0
30009398:	1c6a      	adds	r2, r5, #1
3000939a:	6065      	str	r5, [r4, #4]
3000939c:	8123      	strh	r3, [r4, #8]
3000939e:	d008      	beq.n	300093b2 <CRYPTO_SetSecurityModeAD+0x42>
300093a0:	f005 0330 	and.w	r3, r5, #48	; 0x30
300093a4:	2b20      	cmp	r3, #32
300093a6:	d07b      	beq.n	300094a0 <CRYPTO_SetSecurityModeAD+0x130>
300093a8:	f085 0580 	eor.w	r5, r5, #128	; 0x80
300093ac:	f3c5 15c0 	ubfx	r5, r5, #7, #1
300093b0:	7265      	strb	r5, [r4, #9]
300093b2:	1c73      	adds	r3, r6, #1
300093b4:	60e6      	str	r6, [r4, #12]
300093b6:	f000 8082 	beq.w	300094be <CRYPTO_SetSecurityModeAD+0x14e>
300093ba:	f006 0303 	and.w	r3, r6, #3
300093be:	f3c6 0280 	ubfx	r2, r6, #2, #1
300093c2:	7422      	strb	r2, [r4, #16]
300093c4:	f1a3 0202 	sub.w	r2, r3, #2
300093c8:	2b01      	cmp	r3, #1
300093ca:	fab2 f282 	clz	r2, r2
300093ce:	ea4f 1252 	mov.w	r2, r2, lsr #5
300093d2:	7462      	strb	r2, [r4, #17]
300093d4:	fab3 f283 	clz	r2, r3
300093d8:	ea4f 1252 	mov.w	r2, r2, lsr #5
300093dc:	74a2      	strb	r2, [r4, #18]
300093de:	bf0c      	ite	eq
300093e0:	2201      	moveq	r2, #1
300093e2:	2200      	movne	r2, #0
300093e4:	74e2      	strb	r2, [r4, #19]
300093e6:	d13f      	bne.n	30009468 <CRYPTO_SetSecurityModeAD+0xf8>
300093e8:	f006 03f0 	and.w	r3, r6, #240	; 0xf0
300093ec:	2b40      	cmp	r3, #64	; 0x40
300093ee:	f000 8083 	beq.w	300094f8 <CRYPTO_SetSecurityModeAD+0x188>
300093f2:	d86a      	bhi.n	300094ca <CRYPTO_SetSecurityModeAD+0x15a>
300093f4:	2b10      	cmp	r3, #16
300093f6:	d06d      	beq.n	300094d4 <CRYPTO_SetSecurityModeAD+0x164>
300093f8:	2b20      	cmp	r3, #32
300093fa:	d173      	bne.n	300094e4 <CRYPTO_SetSecurityModeAD+0x174>
300093fc:	7523      	strb	r3, [r4, #20]
300093fe:	62a3      	str	r3, [r4, #40]	; 0x28
30009400:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009402:	6227      	str	r7, [r4, #32]
30009404:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
30009408:	2f00      	cmp	r7, #0
3000940a:	d038      	beq.n	3000947e <CRYPTO_SetSecurityModeAD+0x10e>
3000940c:	7d23      	ldrb	r3, [r4, #20]
3000940e:	f104 0064 	add.w	r0, r4, #100	; 0x64
30009412:	2b30      	cmp	r3, #48	; 0x30
30009414:	65e0      	str	r0, [r4, #92]	; 0x5c
30009416:	d046      	beq.n	300094a6 <CRYPTO_SetSecurityModeAD+0x136>
30009418:	2b40      	cmp	r3, #64	; 0x40
3000941a:	d044      	beq.n	300094a6 <CRYPTO_SetSecurityModeAD+0x136>
3000941c:	f104 03a4 	add.w	r3, r4, #164	; 0xa4
30009420:	2240      	movs	r2, #64	; 0x40
30009422:	2136      	movs	r1, #54	; 0x36
30009424:	4d39      	ldr	r5, [pc, #228]	; (3000950c <CRYPTO_SetSecurityModeAD+0x19c>)
30009426:	6623      	str	r3, [r4, #96]	; 0x60
30009428:	47a8      	blx	r5
3000942a:	2240      	movs	r2, #64	; 0x40
3000942c:	215c      	movs	r1, #92	; 0x5c
3000942e:	6e20      	ldr	r0, [r4, #96]	; 0x60
30009430:	47a8      	blx	r5
30009432:	4b37      	ldr	r3, [pc, #220]	; (30009510 <CRYPTO_SetSecurityModeAD+0x1a0>)
30009434:	681d      	ldr	r5, [r3, #0]
30009436:	bb25      	cbnz	r5, 30009482 <CRYPTO_SetSecurityModeAD+0x112>
30009438:	f1b8 0f00 	cmp.w	r8, #0
3000943c:	d05f      	beq.n	300094fe <CRYPTO_SetSecurityModeAD+0x18e>
3000943e:	f8c4 8024 	str.w	r8, [r4, #36]	; 0x24
30009442:	e001      	b.n	30009448 <CRYPTO_SetSecurityModeAD+0xd8>
30009444:	f8d4 8024 	ldr.w	r8, [r4, #36]	; 0x24
30009448:	6de2      	ldr	r2, [r4, #92]	; 0x5c
3000944a:	f818 3005 	ldrb.w	r3, [r8, r5]
3000944e:	5d51      	ldrb	r1, [r2, r5]
30009450:	404b      	eors	r3, r1
30009452:	5553      	strb	r3, [r2, r5]
30009454:	6e22      	ldr	r2, [r4, #96]	; 0x60
30009456:	6a63      	ldr	r3, [r4, #36]	; 0x24
30009458:	5d51      	ldrb	r1, [r2, r5]
3000945a:	5d5b      	ldrb	r3, [r3, r5]
3000945c:	404b      	eors	r3, r1
3000945e:	5553      	strb	r3, [r2, r5]
30009460:	3501      	adds	r5, #1
30009462:	42af      	cmp	r7, r5
30009464:	d1ee      	bne.n	30009444 <CRYPTO_SetSecurityModeAD+0xd4>
30009466:	e00c      	b.n	30009482 <CRYPTO_SetSecurityModeAD+0x112>
30009468:	2200      	movs	r2, #0
3000946a:	7522      	strb	r2, [r4, #20]
3000946c:	b99b      	cbnz	r3, 30009496 <CRYPTO_SetSecurityModeAD+0x126>
3000946e:	2314      	movs	r3, #20
30009470:	6227      	str	r7, [r4, #32]
30009472:	62a3      	str	r3, [r4, #40]	; 0x28
30009474:	9b0a      	ldr	r3, [sp, #40]	; 0x28
30009476:	e9c4 3911 	strd	r3, r9, [r4, #68]	; 0x44
3000947a:	2f00      	cmp	r7, #0
3000947c:	d1c6      	bne.n	3000940c <CRYPTO_SetSecurityModeAD+0x9c>
3000947e:	e9c4 7717 	strd	r7, r7, [r4, #92]	; 0x5c
30009482:	3601      	adds	r6, #1
30009484:	d003      	beq.n	3000948e <CRYPTO_SetSecurityModeAD+0x11e>
30009486:	2201      	movs	r2, #1
30009488:	2300      	movs	r3, #0
3000948a:	85a2      	strh	r2, [r4, #44]	; 0x2c
3000948c:	6323      	str	r3, [r4, #48]	; 0x30
3000948e:	2000      	movs	r0, #0
30009490:	b003      	add	sp, #12
30009492:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009496:	2b02      	cmp	r3, #2
30009498:	d114      	bne.n	300094c4 <CRYPTO_SetSecurityModeAD+0x154>
3000949a:	2310      	movs	r3, #16
3000949c:	62a3      	str	r3, [r4, #40]	; 0x28
3000949e:	e7af      	b.n	30009400 <CRYPTO_SetSecurityModeAD+0x90>
300094a0:	2301      	movs	r3, #1
300094a2:	7223      	strb	r3, [r4, #8]
300094a4:	e780      	b.n	300093a8 <CRYPTO_SetSecurityModeAD+0x38>
300094a6:	f104 03e4 	add.w	r3, r4, #228	; 0xe4
300094aa:	2280      	movs	r2, #128	; 0x80
300094ac:	2136      	movs	r1, #54	; 0x36
300094ae:	4d17      	ldr	r5, [pc, #92]	; (3000950c <CRYPTO_SetSecurityModeAD+0x19c>)
300094b0:	6623      	str	r3, [r4, #96]	; 0x60
300094b2:	47a8      	blx	r5
300094b4:	2280      	movs	r2, #128	; 0x80
300094b6:	215c      	movs	r1, #92	; 0x5c
300094b8:	6e20      	ldr	r0, [r4, #96]	; 0x60
300094ba:	47a8      	blx	r5
300094bc:	e7b9      	b.n	30009432 <CRYPTO_SetSecurityModeAD+0xc2>
300094be:	2300      	movs	r3, #0
300094c0:	6123      	str	r3, [r4, #16]
300094c2:	7523      	strb	r3, [r4, #20]
300094c4:	2300      	movs	r3, #0
300094c6:	62a3      	str	r3, [r4, #40]	; 0x28
300094c8:	e79a      	b.n	30009400 <CRYPTO_SetSecurityModeAD+0x90>
300094ca:	2b80      	cmp	r3, #128	; 0x80
300094cc:	d10a      	bne.n	300094e4 <CRYPTO_SetSecurityModeAD+0x174>
300094ce:	2340      	movs	r3, #64	; 0x40
300094d0:	7523      	strb	r3, [r4, #20]
300094d2:	e794      	b.n	300093fe <CRYPTO_SetSecurityModeAD+0x8e>
300094d4:	231c      	movs	r3, #28
300094d6:	7523      	strb	r3, [r4, #20]
300094d8:	e7f5      	b.n	300094c6 <CRYPTO_SetSecurityModeAD+0x156>
300094da:	2124      	movs	r1, #36	; 0x24
300094dc:	480a      	ldr	r0, [pc, #40]	; (30009508 <CRYPTO_SetSecurityModeAD+0x198>)
300094de:	f002 fffb 	bl	3000c4d8 <__io_assert_failed_veneer>
300094e2:	e751      	b.n	30009388 <CRYPTO_SetSecurityModeAD+0x18>
300094e4:	4b0b      	ldr	r3, [pc, #44]	; (30009514 <CRYPTO_SetSecurityModeAD+0x1a4>)
300094e6:	2002      	movs	r0, #2
300094e8:	4a0b      	ldr	r2, [pc, #44]	; (30009518 <CRYPTO_SetSecurityModeAD+0x1a8>)
300094ea:	4619      	mov	r1, r3
300094ec:	9600      	str	r6, [sp, #0]
300094ee:	f002 fcfd 	bl	3000beec <rtk_log_write>
300094f2:	2300      	movs	r3, #0
300094f4:	7523      	strb	r3, [r4, #20]
300094f6:	e7e6      	b.n	300094c6 <CRYPTO_SetSecurityModeAD+0x156>
300094f8:	2330      	movs	r3, #48	; 0x30
300094fa:	7523      	strb	r3, [r4, #20]
300094fc:	e7e3      	b.n	300094c6 <CRYPTO_SetSecurityModeAD+0x156>
300094fe:	2180      	movs	r1, #128	; 0x80
30009500:	4801      	ldr	r0, [pc, #4]	; (30009508 <CRYPTO_SetSecurityModeAD+0x198>)
30009502:	f002 ffe9 	bl	3000c4d8 <__io_assert_failed_veneer>
30009506:	e79a      	b.n	3000943e <CRYPTO_SetSecurityModeAD+0xce>
30009508:	3000d830 	.word	0x3000d830
3000950c:	00012be5 	.word	0x00012be5
30009510:	2001c200 	.word	0x2001c200
30009514:	3000d794 	.word	0x3000d794
30009518:	3000d7a0 	.word	0x3000d7a0

3000951c <rtl_crypto_hmac_sha2_init>:
3000951c:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
30009520:	4c97      	ldr	r4, [pc, #604]	; (30009780 <rtl_crypto_hmac_sha2_init+0x264>)
30009522:	4617      	mov	r7, r2
30009524:	e844 f400 	tt	r4, r4
30009528:	4a96      	ldr	r2, [pc, #600]	; (30009784 <rtl_crypto_hmac_sha2_init+0x268>)
3000952a:	f414 0f80 	tst.w	r4, #4194304	; 0x400000
3000952e:	461c      	mov	r4, r3
30009530:	4b95      	ldr	r3, [pc, #596]	; (30009788 <rtl_crypto_hmac_sha2_init+0x26c>)
30009532:	bf18      	it	ne
30009534:	4613      	movne	r3, r2
30009536:	b085      	sub	sp, #20
30009538:	4605      	mov	r5, r0
3000953a:	460e      	mov	r6, r1
3000953c:	6b5b      	ldr	r3, [r3, #52]	; 0x34
3000953e:	2b00      	cmp	r3, #0
30009540:	db05      	blt.n	3000954e <rtl_crypto_hmac_sha2_init+0x32>
30009542:	2900      	cmp	r1, #0
30009544:	f000 8118 	beq.w	30009778 <rtl_crypto_hmac_sha2_init+0x25c>
30009548:	078b      	lsls	r3, r1, #30
3000954a:	f040 8112 	bne.w	30009772 <rtl_crypto_hmac_sha2_init+0x256>
3000954e:	2d20      	cmp	r5, #32
30009550:	bf18      	it	ne
30009552:	2d1c      	cmpne	r5, #28
30009554:	bf0c      	ite	eq
30009556:	f04f 0901 	moveq.w	r9, #1
3000955a:	f04f 0900 	movne.w	r9, #0
3000955e:	d04d      	beq.n	300095fc <rtl_crypto_hmac_sha2_init+0xe0>
30009560:	2d40      	cmp	r5, #64	; 0x40
30009562:	bf18      	it	ne
30009564:	2d30      	cmpne	r5, #48	; 0x30
30009566:	d102      	bne.n	3000956e <rtl_crypto_hmac_sha2_init+0x52>
30009568:	2f80      	cmp	r7, #128	; 0x80
3000956a:	f200 80f9 	bhi.w	30009760 <rtl_crypto_hmac_sha2_init+0x244>
3000956e:	f8df 8238 	ldr.w	r8, [pc, #568]	; 300097a8 <rtl_crypto_hmac_sha2_init+0x28c>
30009572:	f898 3000 	ldrb.w	r3, [r8]
30009576:	2b01      	cmp	r3, #1
30009578:	d003      	beq.n	30009582 <rtl_crypto_hmac_sha2_init+0x66>
3000957a:	21b4      	movs	r1, #180	; 0xb4
3000957c:	4883      	ldr	r0, [pc, #524]	; (3000978c <rtl_crypto_hmac_sha2_init+0x270>)
3000957e:	f002 ffab 	bl	3000c4d8 <__io_assert_failed_veneer>
30009582:	2d30      	cmp	r5, #48	; 0x30
30009584:	f000 809b 	beq.w	300096be <rtl_crypto_hmac_sha2_init+0x1a2>
30009588:	d83f      	bhi.n	3000960a <rtl_crypto_hmac_sha2_init+0xee>
3000958a:	2d1c      	cmp	r5, #28
3000958c:	f000 80cb 	beq.w	30009726 <rtl_crypto_hmac_sha2_init+0x20a>
30009590:	f1b9 0f00 	cmp.w	r9, #0
30009594:	d078      	beq.n	30009688 <rtl_crypto_hmac_sha2_init+0x16c>
30009596:	2c00      	cmp	r4, #0
30009598:	f000 808f 	beq.w	300096ba <rtl_crypto_hmac_sha2_init+0x19e>
3000959c:	4b7c      	ldr	r3, [pc, #496]	; (30009790 <rtl_crypto_hmac_sha2_init+0x274>)
3000959e:	f8df e210 	ldr.w	lr, [pc, #528]	; 300097b0 <rtl_crypto_hmac_sha2_init+0x294>
300095a2:	f8df 921c 	ldr.w	r9, [pc, #540]	; 300097c0 <rtl_crypto_hmac_sha2_init+0x2a4>
300095a6:	4a7b      	ldr	r2, [pc, #492]	; (30009794 <rtl_crypto_hmac_sha2_init+0x278>)
300095a8:	e9c4 e302 	strd	lr, r3, [r4, #8]
300095ac:	4b7a      	ldr	r3, [pc, #488]	; (30009798 <rtl_crypto_hmac_sha2_init+0x27c>)
300095ae:	e9c4 9304 	strd	r9, r3, [r4, #16]
300095b2:	4b7a      	ldr	r3, [pc, #488]	; (3000979c <rtl_crypto_hmac_sha2_init+0x280>)
300095b4:	e9c4 2306 	strd	r2, r3, [r4, #24]
300095b8:	4b79      	ldr	r3, [pc, #484]	; (300097a0 <rtl_crypto_hmac_sha2_init+0x284>)
300095ba:	2225      	movs	r2, #37	; 0x25
300095bc:	6223      	str	r3, [r4, #32]
300095be:	f103 5374 	add.w	r3, r3, #1023410176	; 0x3d000000
300095c2:	f5a3 0323 	sub.w	r3, r3, #10682368	; 0xa30000
300095c6:	f6a3 4392 	subw	r3, r3, #3218	; 0xc92
300095ca:	6263      	str	r3, [r4, #36]	; 0x24
300095cc:	2300      	movs	r3, #0
300095ce:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
300095d2:	4d74      	ldr	r5, [pc, #464]	; (300097a4 <rtl_crypto_hmac_sha2_init+0x288>)
300095d4:	9300      	str	r3, [sp, #0]
300095d6:	9702      	str	r7, [sp, #8]
300095d8:	9601      	str	r6, [sp, #4]
300095da:	4873      	ldr	r0, [pc, #460]	; (300097a8 <rtl_crypto_hmac_sha2_init+0x28c>)
300095dc:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
300095e0:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
300095e4:	47a8      	blx	r5
300095e6:	4605      	mov	r5, r0
300095e8:	4b70      	ldr	r3, [pc, #448]	; (300097ac <rtl_crypto_hmac_sha2_init+0x290>)
300095ea:	4620      	mov	r0, r4
300095ec:	4798      	blx	r3
300095ee:	2301      	movs	r3, #1
300095f0:	4628      	mov	r0, r5
300095f2:	f884 304b 	strb.w	r3, [r4, #75]	; 0x4b
300095f6:	b005      	add	sp, #20
300095f8:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300095fc:	2f40      	cmp	r7, #64	; 0x40
300095fe:	d9b6      	bls.n	3000956e <rtl_crypto_hmac_sha2_init+0x52>
30009600:	21ae      	movs	r1, #174	; 0xae
30009602:	4862      	ldr	r0, [pc, #392]	; (3000978c <rtl_crypto_hmac_sha2_init+0x270>)
30009604:	f002 ff68 	bl	3000c4d8 <__io_assert_failed_veneer>
30009608:	e7b1      	b.n	3000956e <rtl_crypto_hmac_sha2_init+0x52>
3000960a:	2d40      	cmp	r5, #64	; 0x40
3000960c:	d13c      	bne.n	30009688 <rtl_crypto_hmac_sha2_init+0x16c>
3000960e:	2c00      	cmp	r4, #0
30009610:	d040      	beq.n	30009694 <rtl_crypto_hmac_sha2_init+0x178>
30009612:	4b67      	ldr	r3, [pc, #412]	; (300097b0 <rtl_crypto_hmac_sha2_init+0x294>)
30009614:	4a67      	ldr	r2, [pc, #412]	; (300097b4 <rtl_crypto_hmac_sha2_init+0x298>)
30009616:	4968      	ldr	r1, [pc, #416]	; (300097b8 <rtl_crypto_hmac_sha2_init+0x29c>)
30009618:	4d68      	ldr	r5, [pc, #416]	; (300097bc <rtl_crypto_hmac_sha2_init+0x2a0>)
3000961a:	f8df c1e4 	ldr.w	ip, [pc, #484]	; 30009800 <rtl_crypto_hmac_sha2_init+0x2e4>
3000961e:	e9c4 3202 	strd	r3, r2, [r4, #8]
30009622:	f103 53d6 	add.w	r3, r3, #448790528	; 0x1ac00000
30009626:	2285      	movs	r2, #133	; 0x85
30009628:	f503 4340 	add.w	r3, r3, #49152	; 0xc000
3000962c:	33d4      	adds	r3, #212	; 0xd4
3000962e:	6163      	str	r3, [r4, #20]
30009630:	f103 535a 	add.w	r3, r3, #914358272	; 0x36800000
30009634:	f503 13e8 	add.w	r3, r3, #1900544	; 0x1d0000
30009638:	f203 734a 	addw	r3, r3, #1866	; 0x74a
3000963c:	6123      	str	r3, [r4, #16]
3000963e:	4b60      	ldr	r3, [pc, #384]	; (300097c0 <rtl_crypto_hmac_sha2_init+0x2a4>)
30009640:	e9c4 3106 	strd	r3, r1, [r4, #24]
30009644:	f103 530a 	add.w	r3, r3, #578813952	; 0x22800000
30009648:	f503 1339 	add.w	r3, r3, #3031040	; 0x2e4000
3000964c:	f203 337f 	addw	r3, r3, #895	; 0x37f
30009650:	6263      	str	r3, [r4, #36]	; 0x24
30009652:	4b51      	ldr	r3, [pc, #324]	; (30009798 <rtl_crypto_hmac_sha2_init+0x27c>)
30009654:	6223      	str	r3, [r4, #32]
30009656:	f103 6309 	add.w	r3, r3, #143654912	; 0x8900000
3000965a:	f503 23d1 	add.w	r3, r3, #428032	; 0x68800
3000965e:	f203 5397 	addw	r3, r3, #1431	; 0x597
30009662:	62e3      	str	r3, [r4, #44]	; 0x2c
30009664:	4b4b      	ldr	r3, [pc, #300]	; (30009794 <rtl_crypto_hmac_sha2_init+0x278>)
30009666:	62a3      	str	r3, [r4, #40]	; 0x28
30009668:	f103 435a 	add.w	r3, r3, #3657433088	; 0xda000000
3000966c:	f503 1340 	add.w	r3, r3, #3145728	; 0x300000
30009670:	f503 53cd 	add.w	r3, r3, #6560	; 0x19a0
30009674:	6363      	str	r3, [r4, #52]	; 0x34
30009676:	4b49      	ldr	r3, [pc, #292]	; (3000979c <rtl_crypto_hmac_sha2_init+0x280>)
30009678:	6323      	str	r3, [r4, #48]	; 0x30
3000967a:	4b49      	ldr	r3, [pc, #292]	; (300097a0 <rtl_crypto_hmac_sha2_init+0x284>)
3000967c:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
30009680:	4b50      	ldr	r3, [pc, #320]	; (300097c4 <rtl_crypto_hmac_sha2_init+0x2a8>)
30009682:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009686:	e7a1      	b.n	300095cc <rtl_crypto_hmac_sha2_init+0xb0>
30009688:	f04f 35ff 	mov.w	r5, #4294967295	; 0xffffffff
3000968c:	4628      	mov	r0, r5
3000968e:	b005      	add	sp, #20
30009690:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
30009694:	2285      	movs	r2, #133	; 0x85
30009696:	2300      	movs	r3, #0
30009698:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000969c:	4c41      	ldr	r4, [pc, #260]	; (300097a4 <rtl_crypto_hmac_sha2_init+0x288>)
3000969e:	9702      	str	r7, [sp, #8]
300096a0:	4841      	ldr	r0, [pc, #260]	; (300097a8 <rtl_crypto_hmac_sha2_init+0x28c>)
300096a2:	f888 3034 	strb.w	r3, [r8, #52]	; 0x34
300096a6:	f888 3040 	strb.w	r3, [r8, #64]	; 0x40
300096aa:	e9cd 3600 	strd	r3, r6, [sp]
300096ae:	47a0      	blx	r4
300096b0:	4605      	mov	r5, r0
300096b2:	4628      	mov	r0, r5
300096b4:	b005      	add	sp, #20
300096b6:	e8bd 83f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, pc}
300096ba:	2225      	movs	r2, #37	; 0x25
300096bc:	e7eb      	b.n	30009696 <rtl_crypto_hmac_sha2_init+0x17a>
300096be:	2c00      	cmp	r4, #0
300096c0:	d053      	beq.n	3000976a <rtl_crypto_hmac_sha2_init+0x24e>
300096c2:	4b41      	ldr	r3, [pc, #260]	; (300097c8 <rtl_crypto_hmac_sha2_init+0x2ac>)
300096c4:	f8df e130 	ldr.w	lr, [pc, #304]	; 300097f8 <rtl_crypto_hmac_sha2_init+0x2dc>
300096c8:	4a40      	ldr	r2, [pc, #256]	; (300097cc <rtl_crypto_hmac_sha2_init+0x2b0>)
300096ca:	4941      	ldr	r1, [pc, #260]	; (300097d0 <rtl_crypto_hmac_sha2_init+0x2b4>)
300096cc:	4d41      	ldr	r5, [pc, #260]	; (300097d4 <rtl_crypto_hmac_sha2_init+0x2b8>)
300096ce:	f8df c12c 	ldr.w	ip, [pc, #300]	; 300097fc <rtl_crypto_hmac_sha2_init+0x2e0>
300096d2:	e9c4 3e02 	strd	r3, lr, [r4, #8]
300096d6:	f103 43d6 	add.w	r3, r3, #1795162112	; 0x6b000000
300096da:	f5a3 137b 	sub.w	r3, r3, #4112384	; 0x3ec000
300096de:	f6a3 0356 	subw	r3, r3, #2134	; 0x856
300096e2:	6163      	str	r3, [r4, #20]
300096e4:	4b3c      	ldr	r3, [pc, #240]	; (300097d8 <rtl_crypto_hmac_sha2_init+0x2bc>)
300096e6:	6123      	str	r3, [r4, #16]
300096e8:	f103 434e 	add.w	r3, r3, #3456106496	; 0xce000000
300096ec:	f5a3 1325 	sub.w	r3, r3, #2703360	; 0x294000
300096f0:	f6a3 4313 	subw	r3, r3, #3091	; 0xc13
300096f4:	61e3      	str	r3, [r4, #28]
300096f6:	4b39      	ldr	r3, [pc, #228]	; (300097dc <rtl_crypto_hmac_sha2_init+0x2c0>)
300096f8:	61a3      	str	r3, [r4, #24]
300096fa:	4b39      	ldr	r3, [pc, #228]	; (300097e0 <rtl_crypto_hmac_sha2_init+0x2c4>)
300096fc:	e9c4 3208 	strd	r3, r2, [r4, #32]
30009700:	4b38      	ldr	r3, [pc, #224]	; (300097e4 <rtl_crypto_hmac_sha2_init+0x2c8>)
30009702:	2245      	movs	r2, #69	; 0x45
30009704:	e9c4 310a 	strd	r3, r1, [r4, #40]	; 0x28
30009708:	f103 7392 	add.w	r3, r3, #19136512	; 0x1240000
3000970c:	f503 436e 	add.w	r3, r3, #60928	; 0xee00
30009710:	33aa      	adds	r3, #170	; 0xaa
30009712:	6363      	str	r3, [r4, #52]	; 0x34
30009714:	4b34      	ldr	r3, [pc, #208]	; (300097e8 <rtl_crypto_hmac_sha2_init+0x2cc>)
30009716:	6323      	str	r3, [r4, #48]	; 0x30
30009718:	4b34      	ldr	r3, [pc, #208]	; (300097ec <rtl_crypto_hmac_sha2_init+0x2d0>)
3000971a:	e9c4 350e 	strd	r3, r5, [r4, #56]	; 0x38
3000971e:	4b34      	ldr	r3, [pc, #208]	; (300097f0 <rtl_crypto_hmac_sha2_init+0x2d4>)
30009720:	e9c4 3c10 	strd	r3, ip, [r4, #64]	; 0x40
30009724:	e752      	b.n	300095cc <rtl_crypto_hmac_sha2_init+0xb0>
30009726:	b314      	cbz	r4, 3000976e <rtl_crypto_hmac_sha2_init+0x252>
30009728:	4b32      	ldr	r3, [pc, #200]	; (300097f4 <rtl_crypto_hmac_sha2_init+0x2d8>)
3000972a:	2215      	movs	r2, #21
3000972c:	4932      	ldr	r1, [pc, #200]	; (300097f8 <rtl_crypto_hmac_sha2_init+0x2dc>)
3000972e:	4827      	ldr	r0, [pc, #156]	; (300097cc <rtl_crypto_hmac_sha2_init+0x2b0>)
30009730:	4d28      	ldr	r5, [pc, #160]	; (300097d4 <rtl_crypto_hmac_sha2_init+0x2b8>)
30009732:	e9c4 1302 	strd	r1, r3, [r4, #8]
30009736:	f103 4379 	add.w	r3, r3, #4177526784	; 0xf9000000
3000973a:	f503 0374 	add.w	r3, r3, #15990784	; 0xf40000
3000973e:	f503 6301 	add.w	r3, r3, #2064	; 0x810
30009742:	6123      	str	r3, [r4, #16]
30009744:	4b22      	ldr	r3, [pc, #136]	; (300097d0 <rtl_crypto_hmac_sha2_init+0x2b4>)
30009746:	e9c4 0305 	strd	r0, r3, [r4, #20]
3000974a:	f103 43d1 	add.w	r3, r3, #1753219072	; 0x68800000
3000974e:	f503 13c0 	add.w	r3, r3, #1572864	; 0x180000
30009752:	f503 631e 	add.w	r3, r3, #2528	; 0x9e0
30009756:	61e3      	str	r3, [r4, #28]
30009758:	4b28      	ldr	r3, [pc, #160]	; (300097fc <rtl_crypto_hmac_sha2_init+0x2e0>)
3000975a:	e9c4 5308 	strd	r5, r3, [r4, #32]
3000975e:	e735      	b.n	300095cc <rtl_crypto_hmac_sha2_init+0xb0>
30009760:	21b1      	movs	r1, #177	; 0xb1
30009762:	480a      	ldr	r0, [pc, #40]	; (3000978c <rtl_crypto_hmac_sha2_init+0x270>)
30009764:	f002 feb8 	bl	3000c4d8 <__io_assert_failed_veneer>
30009768:	e701      	b.n	3000956e <rtl_crypto_hmac_sha2_init+0x52>
3000976a:	2245      	movs	r2, #69	; 0x45
3000976c:	e793      	b.n	30009696 <rtl_crypto_hmac_sha2_init+0x17a>
3000976e:	2215      	movs	r2, #21
30009770:	e791      	b.n	30009696 <rtl_crypto_hmac_sha2_init+0x17a>
30009772:	f06f 0505 	mvn.w	r5, #5
30009776:	e789      	b.n	3000968c <rtl_crypto_hmac_sha2_init+0x170>
30009778:	f06f 0503 	mvn.w	r5, #3
3000977c:	e786      	b.n	3000968c <rtl_crypto_hmac_sha2_init+0x170>
3000977e:	bf00      	nop
30009780:	0000e5f9 	.word	0x0000e5f9
30009784:	500c8000 	.word	0x500c8000
30009788:	400c8000 	.word	0x400c8000
3000978c:	3000d814 	.word	0x3000d814
30009790:	bb67ae85 	.word	0xbb67ae85
30009794:	510e527f 	.word	0x510e527f
30009798:	a54ff53a 	.word	0xa54ff53a
3000979c:	9b05688c 	.word	0x9b05688c
300097a0:	1f83d9ab 	.word	0x1f83d9ab
300097a4:	30009371 	.word	0x30009371
300097a8:	2001c460 	.word	0x2001c460
300097ac:	00004c99 	.word	0x00004c99
300097b0:	6a09e667 	.word	0x6a09e667
300097b4:	f3bcc908 	.word	0xf3bcc908
300097b8:	fe94f82b 	.word	0xfe94f82b
300097bc:	fb41bd6b 	.word	0xfb41bd6b
300097c0:	3c6ef372 	.word	0x3c6ef372
300097c4:	5be0cd19 	.word	0x5be0cd19
300097c8:	cbbb9d5d 	.word	0xcbbb9d5d
300097cc:	f70e5939 	.word	0xf70e5939
300097d0:	ffc00b31 	.word	0xffc00b31
300097d4:	64f98fa7 	.word	0x64f98fa7
300097d8:	629a292a 	.word	0x629a292a
300097dc:	9159015a 	.word	0x9159015a
300097e0:	152fecd8 	.word	0x152fecd8
300097e4:	67332667 	.word	0x67332667
300097e8:	8eb44a87 	.word	0x8eb44a87
300097ec:	db0c2e0d 	.word	0xdb0c2e0d
300097f0:	47b5481d 	.word	0x47b5481d
300097f4:	367cd507 	.word	0x367cd507
300097f8:	c1059ed8 	.word	0xc1059ed8
300097fc:	befa4fa4 	.word	0xbefa4fa4
30009800:	137e2179 	.word	0x137e2179

30009804 <CPU_ClkGet>:
30009804:	b570      	push	{r4, r5, r6, lr}
30009806:	4d1e      	ldr	r5, [pc, #120]	; (30009880 <CPU_ClkGet+0x7c>)
30009808:	4b1e      	ldr	r3, [pc, #120]	; (30009884 <CPU_ClkGet+0x80>)
3000980a:	f8d5 6224 	ldr.w	r6, [r5, #548]	; 0x224
3000980e:	f8d5 427c 	ldr.w	r4, [r5, #636]	; 0x27c
30009812:	f3c4 1481 	ubfx	r4, r4, #6, #2
30009816:	4798      	blx	r3
30009818:	b92c      	cbnz	r4, 30009826 <CPU_ClkGet+0x22>
3000981a:	2801      	cmp	r0, #1
3000981c:	d007      	beq.n	3000982e <CPU_ClkGet+0x2a>
3000981e:	07b2      	lsls	r2, r6, #30
30009820:	d407      	bmi.n	30009832 <CPU_ClkGet+0x2e>
30009822:	4819      	ldr	r0, [pc, #100]	; (30009888 <CPU_ClkGet+0x84>)
30009824:	bd70      	pop	{r4, r5, r6, pc}
30009826:	2c01      	cmp	r4, #1
30009828:	d007      	beq.n	3000983a <CPU_ClkGet+0x36>
3000982a:	2000      	movs	r0, #0
3000982c:	bd70      	pop	{r4, r5, r6, pc}
3000982e:	4817      	ldr	r0, [pc, #92]	; (3000988c <CPU_ClkGet+0x88>)
30009830:	bd70      	pop	{r4, r5, r6, pc}
30009832:	4b17      	ldr	r3, [pc, #92]	; (30009890 <CPU_ClkGet+0x8c>)
30009834:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
30009838:	4718      	bx	r3
3000983a:	2801      	cmp	r0, #1
3000983c:	d011      	beq.n	30009862 <CPU_ClkGet+0x5e>
3000983e:	05f3      	lsls	r3, r6, #23
30009840:	d51c      	bpl.n	3000987c <CPU_ClkGet+0x78>
30009842:	4914      	ldr	r1, [pc, #80]	; (30009894 <CPU_ClkGet+0x90>)
30009844:	f8d5 3228 	ldr.w	r3, [r5, #552]	; 0x228
30009848:	6c48      	ldr	r0, [r1, #68]	; 0x44
3000984a:	4a13      	ldr	r2, [pc, #76]	; (30009898 <CPU_ClkGet+0x94>)
3000984c:	f003 0307 	and.w	r3, r3, #7
30009850:	f3c0 1045 	ubfx	r0, r0, #5, #6
30009854:	3301      	adds	r3, #1
30009856:	3002      	adds	r0, #2
30009858:	fb02 f000 	mul.w	r0, r2, r0
3000985c:	fbb0 f0f3 	udiv	r0, r0, r3
30009860:	bd70      	pop	{r4, r5, r6, pc}
30009862:	f8d5 326c 	ldr.w	r3, [r5, #620]	; 0x26c
30009866:	f413 0f7f 	tst.w	r3, #16711680	; 0xff0000
3000986a:	f3c3 4007 	ubfx	r0, r3, #16, #8
3000986e:	d003      	beq.n	30009878 <CPU_ClkGet+0x74>
30009870:	4b0a      	ldr	r3, [pc, #40]	; (3000989c <CPU_ClkGet+0x98>)
30009872:	fb03 f000 	mul.w	r0, r3, r0
30009876:	bd70      	pop	{r4, r5, r6, pc}
30009878:	4809      	ldr	r0, [pc, #36]	; (300098a0 <CPU_ClkGet+0x9c>)
3000987a:	bd70      	pop	{r4, r5, r6, pc}
3000987c:	4806      	ldr	r0, [pc, #24]	; (30009898 <CPU_ClkGet+0x94>)
3000987e:	bd70      	pop	{r4, r5, r6, pc}
30009880:	42008000 	.word	0x42008000
30009884:	0000c0f9 	.word	0x0000c0f9
30009888:	003d0900 	.word	0x003d0900
3000988c:	01312d00 	.word	0x01312d00
30009890:	000099f5 	.word	0x000099f5
30009894:	42008800 	.word	0x42008800
30009898:	02625a00 	.word	0x02625a00
3000989c:	000f4240 	.word	0x000f4240
300098a0:	02faf080 	.word	0x02faf080

300098a4 <RSIP_IV_Set>:
300098a4:	4b05      	ldr	r3, [pc, #20]	; (300098bc <RSIP_IV_Set+0x18>)
300098a6:	eb03 00c0 	add.w	r0, r3, r0, lsl #3
300098aa:	680b      	ldr	r3, [r1, #0]
300098ac:	ba1b      	rev	r3, r3
300098ae:	f8c0 3084 	str.w	r3, [r0, #132]	; 0x84
300098b2:	684b      	ldr	r3, [r1, #4]
300098b4:	ba1b      	rev	r3, r3
300098b6:	f8c0 3080 	str.w	r3, [r0, #128]	; 0x80
300098ba:	4770      	bx	lr
300098bc:	42008c00 	.word	0x42008c00

300098c0 <RSIP_OTF_Cmd>:
300098c0:	4a04      	ldr	r2, [pc, #16]	; (300098d4 <RSIP_OTF_Cmd+0x14>)
300098c2:	2801      	cmp	r0, #1
300098c4:	6953      	ldr	r3, [r2, #20]
300098c6:	bf0c      	ite	eq
300098c8:	f443 7380 	orreq.w	r3, r3, #256	; 0x100
300098cc:	f423 7380 	bicne.w	r3, r3, #256	; 0x100
300098d0:	6153      	str	r3, [r2, #20]
300098d2:	4770      	bx	lr
300098d4:	42008c00 	.word	0x42008c00

300098d8 <RSIP_OTF_Enable>:
300098d8:	b570      	push	{r4, r5, r6, lr}
300098da:	4604      	mov	r4, r0
300098dc:	b1f3      	cbz	r3, 3000991c <RSIP_OTF_Enable+0x44>
300098de:	9b04      	ldr	r3, [sp, #16]
300098e0:	460d      	mov	r5, r1
300098e2:	4616      	mov	r6, r2
300098e4:	2b07      	cmp	r3, #7
300098e6:	d827      	bhi.n	30009938 <RSIP_OTF_Enable+0x60>
300098e8:	9b05      	ldr	r3, [sp, #20]
300098ea:	3b01      	subs	r3, #1
300098ec:	2b01      	cmp	r3, #1
300098ee:	d81d      	bhi.n	3000992c <RSIP_OTF_Enable+0x54>
300098f0:	9a04      	ldr	r2, [sp, #16]
300098f2:	9b05      	ldr	r3, [sp, #20]
300098f4:	f002 0007 	and.w	r0, r2, #7
300098f8:	005b      	lsls	r3, r3, #1
300098fa:	4305      	orrs	r5, r0
300098fc:	4813      	ldr	r0, [pc, #76]	; (3000994c <RSIP_OTF_Enable+0x74>)
300098fe:	f003 0306 	and.w	r3, r3, #6
30009902:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009906:	ea43 0206 	orr.w	r2, r3, r6
3000990a:	6c63      	ldr	r3, [r4, #68]	; 0x44
3000990c:	f042 0201 	orr.w	r2, r2, #1
30009910:	f023 0301 	bic.w	r3, r3, #1
30009914:	6463      	str	r3, [r4, #68]	; 0x44
30009916:	6425      	str	r5, [r4, #64]	; 0x40
30009918:	6462      	str	r2, [r4, #68]	; 0x44
3000991a:	bd70      	pop	{r4, r5, r6, pc}
3000991c:	480b      	ldr	r0, [pc, #44]	; (3000994c <RSIP_OTF_Enable+0x74>)
3000991e:	eb00 04c4 	add.w	r4, r0, r4, lsl #3
30009922:	6c63      	ldr	r3, [r4, #68]	; 0x44
30009924:	f023 0301 	bic.w	r3, r3, #1
30009928:	6463      	str	r3, [r4, #68]	; 0x44
3000992a:	bd70      	pop	{r4, r5, r6, pc}
3000992c:	f240 2131 	movw	r1, #561	; 0x231
30009930:	4807      	ldr	r0, [pc, #28]	; (30009950 <RSIP_OTF_Enable+0x78>)
30009932:	f002 fdd1 	bl	3000c4d8 <__io_assert_failed_veneer>
30009936:	e7db      	b.n	300098f0 <RSIP_OTF_Enable+0x18>
30009938:	f44f 710c 	mov.w	r1, #560	; 0x230
3000993c:	4804      	ldr	r0, [pc, #16]	; (30009950 <RSIP_OTF_Enable+0x78>)
3000993e:	f002 fdcb 	bl	3000c4d8 <__io_assert_failed_veneer>
30009942:	9b05      	ldr	r3, [sp, #20]
30009944:	3b01      	subs	r3, #1
30009946:	2b01      	cmp	r3, #1
30009948:	d9d2      	bls.n	300098f0 <RSIP_OTF_Enable+0x18>
3000994a:	e7ef      	b.n	3000992c <RSIP_OTF_Enable+0x54>
3000994c:	42008c00 	.word	0x42008c00
30009950:	3000d804 	.word	0x3000d804

30009954 <RSIP_MMU_Config>:
30009954:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009958:	311f      	adds	r1, #31
3000995a:	321f      	adds	r2, #31
3000995c:	331f      	adds	r3, #31
3000995e:	0080      	lsls	r0, r0, #2
30009960:	0949      	lsrs	r1, r1, #5
30009962:	0952      	lsrs	r2, r2, #5
30009964:	095b      	lsrs	r3, r3, #5
30009966:	0249      	lsls	r1, r1, #9
30009968:	0252      	lsls	r2, r2, #9
3000996a:	025b      	lsls	r3, r3, #9
3000996c:	b410      	push	{r4}
3000996e:	4c08      	ldr	r4, [pc, #32]	; (30009990 <RSIP_MMU_Config+0x3c>)
30009970:	4404      	add	r4, r0
30009972:	f8d4 0108 	ldr.w	r0, [r4, #264]	; 0x108
30009976:	f020 0001 	bic.w	r0, r0, #1
3000997a:	f8c4 0108 	str.w	r0, [r4, #264]	; 0x108
3000997e:	f8c4 1100 	str.w	r1, [r4, #256]	; 0x100
30009982:	f8c4 2104 	str.w	r2, [r4, #260]	; 0x104
30009986:	f8c4 3108 	str.w	r3, [r4, #264]	; 0x108
3000998a:	f85d 4b04 	ldr.w	r4, [sp], #4
3000998e:	4770      	bx	lr
30009990:	42008c00 	.word	0x42008c00

30009994 <RSIP_MMU_Cmd>:
30009994:	eb00 0040 	add.w	r0, r0, r0, lsl #1
30009998:	4b07      	ldr	r3, [pc, #28]	; (300099b8 <RSIP_MMU_Cmd+0x24>)
3000999a:	0080      	lsls	r0, r0, #2
3000999c:	4403      	add	r3, r0
3000999e:	f8d3 2108 	ldr.w	r2, [r3, #264]	; 0x108
300099a2:	b121      	cbz	r1, 300099ae <RSIP_MMU_Cmd+0x1a>
300099a4:	f042 0201 	orr.w	r2, r2, #1
300099a8:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300099ac:	4770      	bx	lr
300099ae:	f022 0201 	bic.w	r2, r2, #1
300099b2:	f8c3 2108 	str.w	r2, [r3, #264]	; 0x108
300099b6:	4770      	bx	lr
300099b8:	42008c00 	.word	0x42008c00

300099bc <RSIP_MMU_Cache_Clean>:
300099bc:	4a02      	ldr	r2, [pc, #8]	; (300099c8 <RSIP_MMU_Cache_Clean+0xc>)
300099be:	6993      	ldr	r3, [r2, #24]
300099c0:	f043 0304 	orr.w	r3, r3, #4
300099c4:	6193      	str	r3, [r2, #24]
300099c6:	4770      	bx	lr
300099c8:	42008c00 	.word	0x42008c00

300099cc <NAND_RxData>:
300099cc:	e92d 43f8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, lr}
300099d0:	460c      	mov	r4, r1
300099d2:	4616      	mov	r6, r2
300099d4:	461f      	mov	r7, r3
300099d6:	2800      	cmp	r0, #0
300099d8:	f000 808a 	beq.w	30009af0 <NAND_RxData+0x124>
300099dc:	4605      	mov	r5, r0
300099de:	f8df 911c 	ldr.w	r9, [pc, #284]	; 30009afc <NAND_RxData+0x130>
300099e2:	2000      	movs	r0, #0
300099e4:	f8df 8118 	ldr.w	r8, [pc, #280]	; 30009b00 <NAND_RxData+0x134>
300099e8:	47c0      	blx	r8
300099ea:	f04f 4388 	mov.w	r3, #1140850688	; 0x44000000
300099ee:	2000      	movs	r0, #0
300099f0:	f026 427f 	bic.w	r2, r6, #4278190080	; 0xff000000
300099f4:	6819      	ldr	r1, [r3, #0]
300099f6:	f041 4100 	orr.w	r1, r1, #2147483648	; 0x80000000
300099fa:	6019      	str	r1, [r3, #0]
300099fc:	6819      	ldr	r1, [r3, #0]
300099fe:	f441 7140 	orr.w	r1, r1, #768	; 0x300
30009a02:	6019      	str	r1, [r3, #0]
30009a04:	605a      	str	r2, [r3, #4]
30009a06:	f8c3 0130 	str.w	r0, [r3, #304]	; 0x130
30009a0a:	f899 c062 	ldrb.w	ip, [r9, #98]	; 0x62
30009a0e:	f8d3 1118 	ldr.w	r1, [r3, #280]	; 0x118
30009a12:	ea4f 420c 	mov.w	r2, ip, lsl #16
30009a16:	f1bc 0f03 	cmp.w	ip, #3
30009a1a:	f421 2170 	bic.w	r1, r1, #983040	; 0xf0000
30009a1e:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
30009a22:	ea42 0201 	orr.w	r2, r2, r1
30009a26:	f8c3 2118 	str.w	r2, [r3, #280]	; 0x118
30009a2a:	f04f 0201 	mov.w	r2, #1
30009a2e:	611a      	str	r2, [r3, #16]
30009a30:	f883 5060 	strb.w	r5, [r3, #96]	; 0x60
30009a34:	bf08      	it	eq
30009a36:	f883 0060 	strbeq.w	r0, [r3, #96]	; 0x60
30009a3a:	f3c4 2307 	ubfx	r3, r4, #8, #8
30009a3e:	f04f 4088 	mov.w	r0, #1140850688	; 0x44000000
30009a42:	b2e4      	uxtb	r4, r4
30009a44:	f880 3060 	strb.w	r3, [r0, #96]	; 0x60
30009a48:	2301      	movs	r3, #1
30009a4a:	f880 4060 	strb.w	r4, [r0, #96]	; 0x60
30009a4e:	f017 0403 	ands.w	r4, r7, #3
30009a52:	6083      	str	r3, [r0, #8]
30009a54:	d137      	bne.n	30009ac6 <NAND_RxData+0xfa>
30009a56:	463b      	mov	r3, r7
30009a58:	f04f 4e88 	mov.w	lr, #1140850688	; 0x44000000
30009a5c:	42b4      	cmp	r4, r6
30009a5e:	d216      	bcs.n	30009a8e <NAND_RxData+0xc2>
30009a60:	f8de 2024 	ldr.w	r2, [lr, #36]	; 0x24
30009a64:	f012 0f3c 	tst.w	r2, #60	; 0x3c
30009a68:	f3c2 0583 	ubfx	r5, r2, #2, #4
30009a6c:	d00c      	beq.n	30009a88 <NAND_RxData+0xbc>
30009a6e:	00ad      	lsls	r5, r5, #2
30009a70:	1f19      	subs	r1, r3, #4
30009a72:	f1a5 0c04 	sub.w	ip, r5, #4
30009a76:	449c      	add	ip, r3
30009a78:	f8de 0060 	ldr.w	r0, [lr, #96]	; 0x60
30009a7c:	f841 0f04 	str.w	r0, [r1, #4]!
30009a80:	458c      	cmp	ip, r1
30009a82:	d1f9      	bne.n	30009a78 <NAND_RxData+0xac>
30009a84:	442b      	add	r3, r5
30009a86:	442c      	add	r4, r5
30009a88:	1b32      	subs	r2, r6, r4
30009a8a:	2a03      	cmp	r2, #3
30009a8c:	d8e6      	bhi.n	30009a5c <NAND_RxData+0x90>
30009a8e:	1e63      	subs	r3, r4, #1
30009a90:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009a94:	441f      	add	r7, r3
30009a96:	e002      	b.n	30009a9e <NAND_RxData+0xd2>
30009a98:	6a93      	ldr	r3, [r2, #40]	; 0x28
30009a9a:	071b      	lsls	r3, r3, #28
30009a9c:	d40d      	bmi.n	30009aba <NAND_RxData+0xee>
30009a9e:	42b4      	cmp	r4, r6
30009aa0:	d3fa      	bcc.n	30009a98 <NAND_RxData+0xcc>
30009aa2:	2004      	movs	r0, #4
30009aa4:	47c0      	blx	r8
30009aa6:	2000      	movs	r0, #0
30009aa8:	47c0      	blx	r8
30009aaa:	f04f 4288 	mov.w	r2, #1140850688	; 0x44000000
30009aae:	6813      	ldr	r3, [r2, #0]
30009ab0:	f023 4300 	bic.w	r3, r3, #2147483648	; 0x80000000
30009ab4:	6013      	str	r3, [r2, #0]
30009ab6:	e8bd 83f8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, pc}
30009aba:	f892 3060 	ldrb.w	r3, [r2, #96]	; 0x60
30009abe:	3401      	adds	r4, #1
30009ac0:	f807 3f01 	strb.w	r3, [r7, #1]!
30009ac4:	e7eb      	b.n	30009a9e <NAND_RxData+0xd2>
30009ac6:	f1c4 0404 	rsb	r4, r4, #4
30009aca:	2200      	movs	r2, #0
30009acc:	4639      	mov	r1, r7
30009ace:	e00a      	b.n	30009ae6 <NAND_RxData+0x11a>
30009ad0:	6a85      	ldr	r5, [r0, #40]	; 0x28
30009ad2:	072d      	lsls	r5, r5, #28
30009ad4:	d505      	bpl.n	30009ae2 <NAND_RxData+0x116>
30009ad6:	f890 3060 	ldrb.w	r3, [r0, #96]	; 0x60
30009ada:	3201      	adds	r2, #1
30009adc:	700b      	strb	r3, [r1, #0]
30009ade:	18b9      	adds	r1, r7, r2
30009ae0:	460b      	mov	r3, r1
30009ae2:	4294      	cmp	r4, r2
30009ae4:	d0b8      	beq.n	30009a58 <NAND_RxData+0x8c>
30009ae6:	42b2      	cmp	r2, r6
30009ae8:	460b      	mov	r3, r1
30009aea:	d3f1      	bcc.n	30009ad0 <NAND_RxData+0x104>
30009aec:	4614      	mov	r4, r2
30009aee:	e7ce      	b.n	30009a8e <NAND_RxData+0xc2>
30009af0:	f8df 9008 	ldr.w	r9, [pc, #8]	; 30009afc <NAND_RxData+0x130>
30009af4:	f899 5008 	ldrb.w	r5, [r9, #8]
30009af8:	e773      	b.n	300099e2 <NAND_RxData+0x16>
30009afa:	bf00      	nop
30009afc:	2001c01c 	.word	0x2001c01c
30009b00:	0000b9c1 	.word	0x0000b9c1

30009b04 <NAND_Page_Read>:
30009b04:	b530      	push	{r4, r5, lr}
30009b06:	b085      	sub	sp, #20
30009b08:	4c07      	ldr	r4, [pc, #28]	; (30009b28 <NAND_Page_Read+0x24>)
30009b0a:	4d08      	ldr	r5, [pc, #32]	; (30009b2c <NAND_Page_Read+0x28>)
30009b0c:	9301      	str	r3, [sp, #4]
30009b0e:	e9cd 2102 	strd	r2, r1, [sp, #8]
30009b12:	47a0      	blx	r4
30009b14:	4604      	mov	r4, r0
30009b16:	9903      	ldr	r1, [sp, #12]
30009b18:	7a28      	ldrb	r0, [r5, #8]
30009b1a:	4d05      	ldr	r5, [pc, #20]	; (30009b30 <NAND_Page_Read+0x2c>)
30009b1c:	e9dd 3201 	ldrd	r3, r2, [sp, #4]
30009b20:	47a8      	blx	r5
30009b22:	4620      	mov	r0, r4
30009b24:	b005      	add	sp, #20
30009b26:	bd30      	pop	{r4, r5, pc}
30009b28:	0000bd85 	.word	0x0000bd85
30009b2c:	2001c01c 	.word	0x2001c01c
30009b30:	300099cd 	.word	0x300099cd

30009b34 <irq_register>:
30009b34:	b530      	push	{r4, r5, lr}
30009b36:	461c      	mov	r4, r3
30009b38:	460d      	mov	r5, r1
30009b3a:	f040 0001 	orr.w	r0, r0, #1
30009b3e:	4916      	ldr	r1, [pc, #88]	; (30009b98 <irq_register+0x64>)
30009b40:	4b16      	ldr	r3, [pc, #88]	; (30009b9c <irq_register+0x68>)
30009b42:	2c07      	cmp	r4, #7
30009b44:	f841 0025 	str.w	r0, [r1, r5, lsl #2]
30009b48:	b085      	sub	sp, #20
30009b4a:	f843 2025 	str.w	r2, [r3, r5, lsl #2]
30009b4e:	d813      	bhi.n	30009b78 <irq_register+0x44>
30009b50:	2c07      	cmp	r4, #7
30009b52:	bf28      	it	cs
30009b54:	2407      	movcs	r4, #7
30009b56:	2d00      	cmp	r5, #0
30009b58:	ea4f 1444 	mov.w	r4, r4, lsl #5
30009b5c:	b2e4      	uxtb	r4, r4
30009b5e:	db04      	blt.n	30009b6a <irq_register+0x36>
30009b60:	4b0f      	ldr	r3, [pc, #60]	; (30009ba0 <irq_register+0x6c>)
30009b62:	2001      	movs	r0, #1
30009b64:	555c      	strb	r4, [r3, r5]
30009b66:	b005      	add	sp, #20
30009b68:	bd30      	pop	{r4, r5, pc}
30009b6a:	f005 050f 	and.w	r5, r5, #15
30009b6e:	4b0d      	ldr	r3, [pc, #52]	; (30009ba4 <irq_register+0x70>)
30009b70:	2001      	movs	r0, #1
30009b72:	555c      	strb	r4, [r3, r5]
30009b74:	b005      	add	sp, #20
30009b76:	bd30      	pop	{r4, r5, pc}
30009b78:	4b0b      	ldr	r3, [pc, #44]	; (30009ba8 <irq_register+0x74>)
30009b7a:	2107      	movs	r1, #7
30009b7c:	4a0b      	ldr	r2, [pc, #44]	; (30009bac <irq_register+0x78>)
30009b7e:	2003      	movs	r0, #3
30009b80:	9500      	str	r5, [sp, #0]
30009b82:	e9cd 4101 	strd	r4, r1, [sp, #4]
30009b86:	4619      	mov	r1, r3
30009b88:	f002 f9b0 	bl	3000beec <rtk_log_write>
30009b8c:	f44f 7168 	mov.w	r1, #928	; 0x3a0
30009b90:	4807      	ldr	r0, [pc, #28]	; (30009bb0 <irq_register+0x7c>)
30009b92:	f002 fca1 	bl	3000c4d8 <__io_assert_failed_veneer>
30009b96:	e7db      	b.n	30009b50 <irq_register+0x1c>
30009b98:	30000184 	.word	0x30000184
30009b9c:	300002c4 	.word	0x300002c4
30009ba0:	e000e400 	.word	0xe000e400
30009ba4:	e000ed14 	.word	0xe000ed14
30009ba8:	3000d794 	.word	0x3000d794
30009bac:	3000d7c8 	.word	0x3000d7c8
30009bb0:	3000d7f0 	.word	0x3000d7f0

30009bb4 <SYSCFG_BootFromNor>:
30009bb4:	4b0e      	ldr	r3, [pc, #56]	; (30009bf0 <SYSCFG_BootFromNor+0x3c>)
30009bb6:	b510      	push	{r4, lr}
30009bb8:	f8d3 227c 	ldr.w	r2, [r3, #636]	; 0x27c
30009bbc:	f8d3 3280 	ldr.w	r3, [r3, #640]	; 0x280
30009bc0:	0392      	lsls	r2, r2, #14
30009bc2:	d50a      	bpl.n	30009bda <SYSCFG_BootFromNor+0x26>
30009bc4:	f003 4370 	and.w	r3, r3, #4026531840	; 0xf0000000
30009bc8:	f1b3 4f60 	cmp.w	r3, #3758096384	; 0xe0000000
30009bcc:	d105      	bne.n	30009bda <SYSCFG_BootFromNor+0x26>
30009bce:	4b09      	ldr	r3, [pc, #36]	; (30009bf4 <SYSCFG_BootFromNor+0x40>)
30009bd0:	4798      	blx	r3
30009bd2:	3800      	subs	r0, #0
30009bd4:	bf18      	it	ne
30009bd6:	2001      	movne	r0, #1
30009bd8:	bd10      	pop	{r4, pc}
30009bda:	4c07      	ldr	r4, [pc, #28]	; (30009bf8 <SYSCFG_BootFromNor+0x44>)
30009bdc:	47a0      	blx	r4
30009bde:	2802      	cmp	r0, #2
30009be0:	d004      	beq.n	30009bec <SYSCFG_BootFromNor+0x38>
30009be2:	47a0      	blx	r4
30009be4:	2801      	cmp	r0, #1
30009be6:	d1f2      	bne.n	30009bce <SYSCFG_BootFromNor+0x1a>
30009be8:	2000      	movs	r0, #0
30009bea:	bd10      	pop	{r4, pc}
30009bec:	2001      	movs	r0, #1
30009bee:	bd10      	pop	{r4, pc}
30009bf0:	42008000 	.word	0x42008000
30009bf4:	0000c0bd 	.word	0x0000c0bd
30009bf8:	0000c1a9 	.word	0x0000c1a9

30009bfc <SYSCFG_RLVersion>:
30009bfc:	4b07      	ldr	r3, [pc, #28]	; (30009c1c <SYSCFG_RLVersion+0x20>)
30009bfe:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009c02:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009c06:	f042 4120 	orr.w	r1, r2, #2684354560	; 0xa0000000
30009c0a:	f8c3 1274 	str.w	r1, [r3, #628]	; 0x274
30009c0e:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009c12:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009c16:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009c1a:	4770      	bx	lr
30009c1c:	42008000 	.word	0x42008000

30009c20 <SBOOT_GetMdType>:
30009c20:	b470      	push	{r4, r5, r6}
30009c22:	4c0c      	ldr	r4, [pc, #48]	; (30009c54 <SBOOT_GetMdType+0x34>)
30009c24:	4e0c      	ldr	r6, [pc, #48]	; (30009c58 <SBOOT_GetMdType+0x38>)
30009c26:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009c2a:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009c2e:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009c32:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009c36:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009c3a:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009c3e:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009c42:	6033      	str	r3, [r6, #0]
30009c44:	b913      	cbnz	r3, 30009c4c <SBOOT_GetMdType+0x2c>
30009c46:	bc70      	pop	{r4, r5, r6}
30009c48:	f002 bc6e 	b.w	3000c528 <__SBOOT_GetMdType_A_veneer>
30009c4c:	bc70      	pop	{r4, r5, r6}
30009c4e:	f002 bc33 	b.w	3000c4b8 <__SBOOT_GetMdType_B_veneer>
30009c52:	bf00      	nop
30009c54:	42008000 	.word	0x42008000
30009c58:	3000fc84 	.word	0x3000fc84

30009c5c <CRYPTO_OTPKey_SHA_Init>:
30009c5c:	4a0d      	ldr	r2, [pc, #52]	; (30009c94 <CRYPTO_OTPKey_SHA_Init+0x38>)
30009c5e:	b410      	push	{r4}
30009c60:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009c64:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009c68:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009c6c:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009c70:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009c74:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009c78:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009c7c:	4a06      	ldr	r2, [pc, #24]	; (30009c98 <CRYPTO_OTPKey_SHA_Init+0x3c>)
30009c7e:	6013      	str	r3, [r2, #0]
30009c80:	b91b      	cbnz	r3, 30009c8a <CRYPTO_OTPKey_SHA_Init+0x2e>
30009c82:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c86:	f002 bb97 	b.w	3000c3b8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>
30009c8a:	f85d 4b04 	ldr.w	r4, [sp], #4
30009c8e:	f002 bb6b 	b.w	3000c368 <__CRYPTO_OTPKey_SHA_Init_B_veneer>
30009c92:	bf00      	nop
30009c94:	42008000 	.word	0x42008000
30009c98:	3000fc84 	.word	0x3000fc84

30009c9c <rtl_crypto_hmac_sha2_update>:
30009c9c:	b470      	push	{r4, r5, r6}
30009c9e:	4c0c      	ldr	r4, [pc, #48]	; (30009cd0 <rtl_crypto_hmac_sha2_update+0x34>)
30009ca0:	4e0c      	ldr	r6, [pc, #48]	; (30009cd4 <rtl_crypto_hmac_sha2_update+0x38>)
30009ca2:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009ca6:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009caa:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009cae:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009cb2:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009cb6:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009cba:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009cbe:	6033      	str	r3, [r6, #0]
30009cc0:	b913      	cbnz	r3, 30009cc8 <rtl_crypto_hmac_sha2_update+0x2c>
30009cc2:	bc70      	pop	{r4, r5, r6}
30009cc4:	f002 bbe0 	b.w	3000c488 <__rtl_crypto_hmac_sha2_update_A_veneer>
30009cc8:	bc70      	pop	{r4, r5, r6}
30009cca:	f002 bb95 	b.w	3000c3f8 <__rtl_crypto_hmac_sha2_update_B_veneer>
30009cce:	bf00      	nop
30009cd0:	42008000 	.word	0x42008000
30009cd4:	3000fc84 	.word	0x3000fc84

30009cd8 <rtl_crypto_hmac_sha2_final>:
30009cd8:	4a0d      	ldr	r2, [pc, #52]	; (30009d10 <rtl_crypto_hmac_sha2_final+0x38>)
30009cda:	b410      	push	{r4}
30009cdc:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009ce0:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009ce4:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009ce8:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009cec:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009cf0:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009cf4:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009cf8:	4a06      	ldr	r2, [pc, #24]	; (30009d14 <rtl_crypto_hmac_sha2_final+0x3c>)
30009cfa:	6013      	str	r3, [r2, #0]
30009cfc:	b91b      	cbnz	r3, 30009d06 <rtl_crypto_hmac_sha2_final+0x2e>
30009cfe:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d02:	f002 bc01 	b.w	3000c508 <__rtl_crypto_hmac_sha2_final_A_veneer>
30009d06:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d0a:	f002 bc15 	b.w	3000c538 <__rtl_crypto_hmac_sha2_final_B_veneer>
30009d0e:	bf00      	nop
30009d10:	42008000 	.word	0x42008000
30009d14:	3000fc84 	.word	0x3000fc84

30009d18 <rtl_crypto_sha2_init>:
30009d18:	4a0d      	ldr	r2, [pc, #52]	; (30009d50 <rtl_crypto_sha2_init+0x38>)
30009d1a:	b410      	push	{r4}
30009d1c:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009d20:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009d24:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009d28:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009d2c:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009d30:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009d34:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d38:	4a06      	ldr	r2, [pc, #24]	; (30009d54 <rtl_crypto_sha2_init+0x3c>)
30009d3a:	6013      	str	r3, [r2, #0]
30009d3c:	b91b      	cbnz	r3, 30009d46 <rtl_crypto_sha2_init+0x2e>
30009d3e:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d42:	f002 bbb1 	b.w	3000c4a8 <__rtl_crypto_sha2_init_A_veneer>
30009d46:	f85d 4b04 	ldr.w	r4, [sp], #4
30009d4a:	f002 bbcd 	b.w	3000c4e8 <__rtl_crypto_sha2_init_B_veneer>
30009d4e:	bf00      	nop
30009d50:	42008000 	.word	0x42008000
30009d54:	3000fc84 	.word	0x3000fc84

30009d58 <rtl_crypto_sha2_update>:
30009d58:	b470      	push	{r4, r5, r6}
30009d5a:	4c0c      	ldr	r4, [pc, #48]	; (30009d8c <rtl_crypto_sha2_update+0x34>)
30009d5c:	4e0c      	ldr	r6, [pc, #48]	; (30009d90 <rtl_crypto_sha2_update+0x38>)
30009d5e:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009d62:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009d66:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009d6a:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009d6e:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009d72:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009d76:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009d7a:	6033      	str	r3, [r6, #0]
30009d7c:	b913      	cbnz	r3, 30009d84 <rtl_crypto_sha2_update+0x2c>
30009d7e:	bc70      	pop	{r4, r5, r6}
30009d80:	f002 bbea 	b.w	3000c558 <__rtl_crypto_sha2_update_A_veneer>
30009d84:	bc70      	pop	{r4, r5, r6}
30009d86:	f002 bbdf 	b.w	3000c548 <__rtl_crypto_sha2_update_B_veneer>
30009d8a:	bf00      	nop
30009d8c:	42008000 	.word	0x42008000
30009d90:	3000fc84 	.word	0x3000fc84

30009d94 <rtl_crypto_sha2_final>:
30009d94:	4a0d      	ldr	r2, [pc, #52]	; (30009dcc <rtl_crypto_sha2_final+0x38>)
30009d96:	b410      	push	{r4}
30009d98:	f8d2 4274 	ldr.w	r4, [r2, #628]	; 0x274
30009d9c:	f024 4470 	bic.w	r4, r4, #4026531840	; 0xf0000000
30009da0:	f044 4320 	orr.w	r3, r4, #2684354560	; 0xa0000000
30009da4:	f8c2 3274 	str.w	r3, [r2, #628]	; 0x274
30009da8:	f8d2 3274 	ldr.w	r3, [r2, #628]	; 0x274
30009dac:	f8c2 4274 	str.w	r4, [r2, #628]	; 0x274
30009db0:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009db4:	4a06      	ldr	r2, [pc, #24]	; (30009dd0 <rtl_crypto_sha2_final+0x3c>)
30009db6:	6013      	str	r3, [r2, #0]
30009db8:	b91b      	cbnz	r3, 30009dc2 <rtl_crypto_sha2_final+0x2e>
30009dba:	f85d 4b04 	ldr.w	r4, [sp], #4
30009dbe:	f002 bb83 	b.w	3000c4c8 <__rtl_crypto_sha2_final_A_veneer>
30009dc2:	f85d 4b04 	ldr.w	r4, [sp], #4
30009dc6:	f002 baff 	b.w	3000c3c8 <__rtl_crypto_sha2_final_B_veneer>
30009dca:	bf00      	nop
30009dcc:	42008000 	.word	0x42008000
30009dd0:	3000fc84 	.word	0x3000fc84

30009dd4 <SBOOT_Validate_PubKey>:
30009dd4:	b470      	push	{r4, r5, r6}
30009dd6:	4c0c      	ldr	r4, [pc, #48]	; (30009e08 <SBOOT_Validate_PubKey+0x34>)
30009dd8:	4e0c      	ldr	r6, [pc, #48]	; (30009e0c <SBOOT_Validate_PubKey+0x38>)
30009dda:	f8d4 5274 	ldr.w	r5, [r4, #628]	; 0x274
30009dde:	f025 4570 	bic.w	r5, r5, #4026531840	; 0xf0000000
30009de2:	f045 4320 	orr.w	r3, r5, #2684354560	; 0xa0000000
30009de6:	f8c4 3274 	str.w	r3, [r4, #628]	; 0x274
30009dea:	f8d4 3274 	ldr.w	r3, [r4, #628]	; 0x274
30009dee:	f8c4 5274 	str.w	r5, [r4, #628]	; 0x274
30009df2:	f3c3 4303 	ubfx	r3, r3, #16, #4
30009df6:	6033      	str	r3, [r6, #0]
30009df8:	b913      	cbnz	r3, 30009e00 <SBOOT_Validate_PubKey+0x2c>
30009dfa:	bc70      	pop	{r4, r5, r6}
30009dfc:	f002 bb24 	b.w	3000c448 <__SBOOT_Validate_PubKey_A_veneer>
30009e00:	bc70      	pop	{r4, r5, r6}
30009e02:	f002 bb11 	b.w	3000c428 <__SBOOT_Validate_PubKey_B_veneer>
30009e06:	bf00      	nop
30009e08:	42008000 	.word	0x42008000
30009e0c:	3000fc84 	.word	0x3000fc84

30009e10 <SBOOT_Validate_Signature>:
30009e10:	b5f0      	push	{r4, r5, r6, r7, lr}
30009e12:	4d10      	ldr	r5, [pc, #64]	; (30009e54 <SBOOT_Validate_Signature+0x44>)
30009e14:	9f05      	ldr	r7, [sp, #20]
30009e16:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009e1a:	f8dd c018 	ldr.w	ip, [sp, #24]
30009e1e:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009e22:	f8df e034 	ldr.w	lr, [pc, #52]	; 30009e58 <SBOOT_Validate_Signature+0x48>
30009e26:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009e2a:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009e2e:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009e32:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009e36:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009e3a:	e9cd 7c05 	strd	r7, ip, [sp, #20]
30009e3e:	f8ce 4000 	str.w	r4, [lr]
30009e42:	b91c      	cbnz	r4, 30009e4c <SBOOT_Validate_Signature+0x3c>
30009e44:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009e48:	f002 bb26 	b.w	3000c498 <__SBOOT_Validate_Signature_A_veneer>
30009e4c:	e8bd 40f0 	ldmia.w	sp!, {r4, r5, r6, r7, lr}
30009e50:	f002 baf2 	b.w	3000c438 <__SBOOT_Validate_Signature_B_veneer>
30009e54:	42008000 	.word	0x42008000
30009e58:	3000fc84 	.word	0x3000fc84

30009e5c <SBOOT_Validate_ImgHash>:
30009e5c:	b4f0      	push	{r4, r5, r6, r7}
30009e5e:	4d0c      	ldr	r5, [pc, #48]	; (30009e90 <SBOOT_Validate_ImgHash+0x34>)
30009e60:	4f0c      	ldr	r7, [pc, #48]	; (30009e94 <SBOOT_Validate_ImgHash+0x38>)
30009e62:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009e66:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009e6a:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009e6e:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009e72:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009e76:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009e7a:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009e7e:	603c      	str	r4, [r7, #0]
30009e80:	b914      	cbnz	r4, 30009e88 <SBOOT_Validate_ImgHash+0x2c>
30009e82:	bcf0      	pop	{r4, r5, r6, r7}
30009e84:	f002 bb38 	b.w	3000c4f8 <__SBOOT_Validate_ImgHash_A_veneer>
30009e88:	bcf0      	pop	{r4, r5, r6, r7}
30009e8a:	f002 bae5 	b.w	3000c458 <__SBOOT_Validate_ImgHash_B_veneer>
30009e8e:	bf00      	nop
30009e90:	42008000 	.word	0x42008000
30009e94:	3000fc84 	.word	0x3000fc84

30009e98 <CRYPTO_SHA_Init>:
30009e98:	4b0b      	ldr	r3, [pc, #44]	; (30009ec8 <CRYPTO_SHA_Init+0x30>)
30009e9a:	490c      	ldr	r1, [pc, #48]	; (30009ecc <CRYPTO_SHA_Init+0x34>)
30009e9c:	f8d3 2274 	ldr.w	r2, [r3, #628]	; 0x274
30009ea0:	f022 4270 	bic.w	r2, r2, #4026531840	; 0xf0000000
30009ea4:	f042 4020 	orr.w	r0, r2, #2684354560	; 0xa0000000
30009ea8:	f8c3 0274 	str.w	r0, [r3, #628]	; 0x274
30009eac:	f8d3 0274 	ldr.w	r0, [r3, #628]	; 0x274
30009eb0:	f8c3 2274 	str.w	r2, [r3, #628]	; 0x274
30009eb4:	f3c0 4003 	ubfx	r0, r0, #16, #4
30009eb8:	6008      	str	r0, [r1, #0]
30009eba:	b908      	cbnz	r0, 30009ec0 <CRYPTO_SHA_Init+0x28>
30009ebc:	f002 bb2c 	b.w	3000c518 <__CRYPTO_SHA_Init_A_veneer>
30009ec0:	2000      	movs	r0, #0
30009ec2:	f002 ba71 	b.w	3000c3a8 <__CRYPTO_SHA_Init_B_veneer>
30009ec6:	bf00      	nop
30009ec8:	42008000 	.word	0x42008000
30009ecc:	3000fc84 	.word	0x3000fc84

30009ed0 <SBOOT_Validate_Algorithm>:
30009ed0:	b4f0      	push	{r4, r5, r6, r7}
30009ed2:	4d0c      	ldr	r5, [pc, #48]	; (30009f04 <SBOOT_Validate_Algorithm+0x34>)
30009ed4:	4f0c      	ldr	r7, [pc, #48]	; (30009f08 <SBOOT_Validate_Algorithm+0x38>)
30009ed6:	f8d5 6274 	ldr.w	r6, [r5, #628]	; 0x274
30009eda:	f026 4670 	bic.w	r6, r6, #4026531840	; 0xf0000000
30009ede:	f046 4420 	orr.w	r4, r6, #2684354560	; 0xa0000000
30009ee2:	f8c5 4274 	str.w	r4, [r5, #628]	; 0x274
30009ee6:	f8d5 4274 	ldr.w	r4, [r5, #628]	; 0x274
30009eea:	f8c5 6274 	str.w	r6, [r5, #628]	; 0x274
30009eee:	f3c4 4403 	ubfx	r4, r4, #16, #4
30009ef2:	603c      	str	r4, [r7, #0]
30009ef4:	b914      	cbnz	r4, 30009efc <SBOOT_Validate_Algorithm+0x2c>
30009ef6:	bcf0      	pop	{r4, r5, r6, r7}
30009ef8:	f002 ba76 	b.w	3000c3e8 <__SBOOT_Validate_Algorithm_A_veneer>
30009efc:	bcf0      	pop	{r4, r5, r6, r7}
30009efe:	f002 ba83 	b.w	3000c408 <__SBOOT_Validate_Algorithm_B_veneer>
30009f02:	bf00      	nop
30009f04:	42008000 	.word	0x42008000
30009f08:	3000fc84 	.word	0x3000fc84

30009f0c <ddr_init_index>:
30009f0c:	b570      	push	{r4, r5, r6, lr}
30009f0e:	4b0f      	ldr	r3, [pc, #60]	; (30009f4c <ddr_init_index+0x40>)
30009f10:	4798      	blx	r3
30009f12:	4604      	mov	r4, r0
30009f14:	4b0e      	ldr	r3, [pc, #56]	; (30009f50 <ddr_init_index+0x44>)
30009f16:	4798      	blx	r3
30009f18:	2c03      	cmp	r4, #3
30009f1a:	d014      	beq.n	30009f46 <ddr_init_index+0x3a>
30009f1c:	2c01      	cmp	r4, #1
30009f1e:	d00c      	beq.n	30009f3a <ddr_init_index+0x2e>
30009f20:	4e0c      	ldr	r6, [pc, #48]	; (30009f54 <ddr_init_index+0x48>)
30009f22:	4d0d      	ldr	r5, [pc, #52]	; (30009f58 <ddr_init_index+0x4c>)
30009f24:	4c0d      	ldr	r4, [pc, #52]	; (30009f5c <ddr_init_index+0x50>)
30009f26:	462b      	mov	r3, r5
30009f28:	4622      	mov	r2, r4
30009f2a:	490b      	ldr	r1, [pc, #44]	; (30009f58 <ddr_init_index+0x4c>)
30009f2c:	2002      	movs	r0, #2
30009f2e:	f001 ffdd 	bl	3000beec <rtk_log_write>
30009f32:	f242 7010 	movw	r0, #10000	; 0x2710
30009f36:	47b0      	blx	r6
30009f38:	e7f5      	b.n	30009f26 <ddr_init_index+0x1a>
30009f3a:	2802      	cmp	r0, #2
30009f3c:	d003      	beq.n	30009f46 <ddr_init_index+0x3a>
30009f3e:	2803      	cmp	r0, #3
30009f40:	d1ee      	bne.n	30009f20 <ddr_init_index+0x14>
30009f42:	2000      	movs	r0, #0
30009f44:	bd70      	pop	{r4, r5, r6, pc}
30009f46:	4620      	mov	r0, r4
30009f48:	bd70      	pop	{r4, r5, r6, pc}
30009f4a:	bf00      	nop
30009f4c:	30007f55 	.word	0x30007f55
30009f50:	30007dd1 	.word	0x30007dd1
30009f54:	00009be5 	.word	0x00009be5
30009f58:	3000d84c 	.word	0x3000d84c
30009f5c:	3000d850 	.word	0x3000d850

30009f60 <rxi316_perf_tune>:
30009f60:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
30009f64:	4606      	mov	r6, r0
30009f66:	6800      	ldr	r0, [r0, #0]
30009f68:	b0c3      	sub	sp, #268	; 0x10c
30009f6a:	7985      	ldrb	r5, [r0, #6]
30009f6c:	2d00      	cmp	r5, #0
30009f6e:	f000 8334 	beq.w	3000a5da <rxi316_perf_tune+0x67a>
30009f72:	2d01      	cmp	r5, #1
30009f74:	d003      	beq.n	30009f7e <rxi316_perf_tune+0x1e>
30009f76:	2d03      	cmp	r5, #3
30009f78:	bf14      	ite	ne
30009f7a:	2502      	movne	r5, #2
30009f7c:	2500      	moveq	r5, #0
30009f7e:	68f3      	ldr	r3, [r6, #12]
30009f80:	2420      	movs	r4, #32
30009f82:	4684      	mov	ip, r0
30009f84:	6859      	ldr	r1, [r3, #4]
30009f86:	46a3      	mov	fp, r4
30009f88:	46a2      	mov	sl, r4
30009f8a:	46a0      	mov	r8, r4
30009f8c:	784b      	ldrb	r3, [r1, #1]
30009f8e:	46a1      	mov	r9, r4
30009f90:	7bcf      	ldrb	r7, [r1, #15]
30009f92:	9323      	str	r3, [sp, #140]	; 0x8c
30009f94:	788b      	ldrb	r3, [r1, #2]
30009f96:	780a      	ldrb	r2, [r1, #0]
30009f98:	9324      	str	r3, [sp, #144]	; 0x90
30009f9a:	78cb      	ldrb	r3, [r1, #3]
30009f9c:	9325      	str	r3, [sp, #148]	; 0x94
30009f9e:	790b      	ldrb	r3, [r1, #4]
30009fa0:	9326      	str	r3, [sp, #152]	; 0x98
30009fa2:	794b      	ldrb	r3, [r1, #5]
30009fa4:	9327      	str	r3, [sp, #156]	; 0x9c
30009fa6:	798b      	ldrb	r3, [r1, #6]
30009fa8:	9328      	str	r3, [sp, #160]	; 0xa0
30009faa:	79cb      	ldrb	r3, [r1, #7]
30009fac:	9329      	str	r3, [sp, #164]	; 0xa4
30009fae:	7a0b      	ldrb	r3, [r1, #8]
30009fb0:	932a      	str	r3, [sp, #168]	; 0xa8
30009fb2:	7a4b      	ldrb	r3, [r1, #9]
30009fb4:	932b      	str	r3, [sp, #172]	; 0xac
30009fb6:	7a8b      	ldrb	r3, [r1, #10]
30009fb8:	932c      	str	r3, [sp, #176]	; 0xb0
30009fba:	7acb      	ldrb	r3, [r1, #11]
30009fbc:	932d      	str	r3, [sp, #180]	; 0xb4
30009fbe:	7b0b      	ldrb	r3, [r1, #12]
30009fc0:	932e      	str	r3, [sp, #184]	; 0xb8
30009fc2:	7b4b      	ldrb	r3, [r1, #13]
30009fc4:	932f      	str	r3, [sp, #188]	; 0xbc
30009fc6:	7b8b      	ldrb	r3, [r1, #14]
30009fc8:	9330      	str	r3, [sp, #192]	; 0xc0
30009fca:	2300      	movs	r3, #0
30009fcc:	9731      	str	r7, [sp, #196]	; 0xc4
30009fce:	7c0f      	ldrb	r7, [r1, #16]
30009fd0:	941d      	str	r4, [sp, #116]	; 0x74
30009fd2:	9732      	str	r7, [sp, #200]	; 0xc8
30009fd4:	7c4f      	ldrb	r7, [r1, #17]
30009fd6:	9733      	str	r7, [sp, #204]	; 0xcc
30009fd8:	7c8f      	ldrb	r7, [r1, #18]
30009fda:	9734      	str	r7, [sp, #208]	; 0xd0
30009fdc:	7ccf      	ldrb	r7, [r1, #19]
30009fde:	9735      	str	r7, [sp, #212]	; 0xd4
30009fe0:	7d0f      	ldrb	r7, [r1, #20]
30009fe2:	9736      	str	r7, [sp, #216]	; 0xd8
30009fe4:	7d4f      	ldrb	r7, [r1, #21]
30009fe6:	9737      	str	r7, [sp, #220]	; 0xdc
30009fe8:	7d8f      	ldrb	r7, [r1, #22]
30009fea:	9738      	str	r7, [sp, #224]	; 0xe0
30009fec:	7dcf      	ldrb	r7, [r1, #23]
30009fee:	9739      	str	r7, [sp, #228]	; 0xe4
30009ff0:	7e0f      	ldrb	r7, [r1, #24]
30009ff2:	973a      	str	r7, [sp, #232]	; 0xe8
30009ff4:	7e4f      	ldrb	r7, [r1, #25]
30009ff6:	973b      	str	r7, [sp, #236]	; 0xec
30009ff8:	7e8f      	ldrb	r7, [r1, #26]
30009ffa:	973c      	str	r7, [sp, #240]	; 0xf0
30009ffc:	7ecf      	ldrb	r7, [r1, #27]
30009ffe:	973d      	str	r7, [sp, #244]	; 0xf4
3000a000:	7f0f      	ldrb	r7, [r1, #28]
3000a002:	973e      	str	r7, [sp, #248]	; 0xf8
3000a004:	7f4f      	ldrb	r7, [r1, #29]
3000a006:	973f      	str	r7, [sp, #252]	; 0xfc
3000a008:	7f8f      	ldrb	r7, [r1, #30]
3000a00a:	7fc9      	ldrb	r1, [r1, #31]
3000a00c:	9740      	str	r7, [sp, #256]	; 0x100
3000a00e:	4627      	mov	r7, r4
3000a010:	9141      	str	r1, [sp, #260]	; 0x104
3000a012:	4619      	mov	r1, r3
3000a014:	9409      	str	r4, [sp, #36]	; 0x24
3000a016:	941c      	str	r4, [sp, #112]	; 0x70
3000a018:	9404      	str	r4, [sp, #16]
3000a01a:	941b      	str	r4, [sp, #108]	; 0x6c
3000a01c:	940a      	str	r4, [sp, #40]	; 0x28
3000a01e:	9303      	str	r3, [sp, #12]
3000a020:	e9cd 4405 	strd	r4, r4, [sp, #20]
3000a024:	e9cd 4401 	strd	r4, r4, [sp, #4]
3000a028:	e9cd 4407 	strd	r4, r4, [sp, #28]
3000a02c:	e9cd 4419 	strd	r4, r4, [sp, #100]	; 0x64
3000a030:	e9cd 4417 	strd	r4, r4, [sp, #92]	; 0x5c
3000a034:	e9cd 4415 	strd	r4, r4, [sp, #84]	; 0x54
3000a038:	e9cd 4413 	strd	r4, r4, [sp, #76]	; 0x4c
3000a03c:	e9cd 4411 	strd	r4, r4, [sp, #68]	; 0x44
3000a040:	e9cd 440f 	strd	r4, r4, [sp, #60]	; 0x3c
3000a044:	e9cd 440d 	strd	r4, r4, [sp, #52]	; 0x34
3000a048:	e9cd 440b 	strd	r4, r4, [sp, #44]	; 0x2c
3000a04c:	b94a      	cbnz	r2, 3000a062 <rxi316_perf_tune+0x102>
3000a04e:	3101      	adds	r1, #1
3000a050:	4699      	mov	r9, r3
3000a052:	3301      	adds	r3, #1
3000a054:	2b20      	cmp	r3, #32
3000a056:	d069      	beq.n	3000a12c <rxi316_perf_tune+0x1cc>
3000a058:	aa22      	add	r2, sp, #136	; 0x88
3000a05a:	f852 2023 	ldr.w	r2, [r2, r3, lsl #2]
3000a05e:	2a00      	cmp	r2, #0
3000a060:	d0f5      	beq.n	3000a04e <rxi316_perf_tune+0xee>
3000a062:	2a01      	cmp	r2, #1
3000a064:	d05d      	beq.n	3000a122 <rxi316_perf_tune+0x1c2>
3000a066:	2a02      	cmp	r2, #2
3000a068:	d06e      	beq.n	3000a148 <rxi316_perf_tune+0x1e8>
3000a06a:	2a03      	cmp	r2, #3
3000a06c:	d06f      	beq.n	3000a14e <rxi316_perf_tune+0x1ee>
3000a06e:	2a04      	cmp	r2, #4
3000a070:	d070      	beq.n	3000a154 <rxi316_perf_tune+0x1f4>
3000a072:	2a05      	cmp	r2, #5
3000a074:	d071      	beq.n	3000a15a <rxi316_perf_tune+0x1fa>
3000a076:	2a06      	cmp	r2, #6
3000a078:	d072      	beq.n	3000a160 <rxi316_perf_tune+0x200>
3000a07a:	2a07      	cmp	r2, #7
3000a07c:	f000 82af 	beq.w	3000a5de <rxi316_perf_tune+0x67e>
3000a080:	2a08      	cmp	r2, #8
3000a082:	f000 8381 	beq.w	3000a788 <rxi316_perf_tune+0x828>
3000a086:	2a09      	cmp	r2, #9
3000a088:	f000 8386 	beq.w	3000a798 <rxi316_perf_tune+0x838>
3000a08c:	2a0a      	cmp	r2, #10
3000a08e:	f000 838e 	beq.w	3000a7ae <rxi316_perf_tune+0x84e>
3000a092:	2a0b      	cmp	r2, #11
3000a094:	f000 8386 	beq.w	3000a7a4 <rxi316_perf_tune+0x844>
3000a098:	2a0c      	cmp	r2, #12
3000a09a:	f000 838d 	beq.w	3000a7b8 <rxi316_perf_tune+0x858>
3000a09e:	2a0d      	cmp	r2, #13
3000a0a0:	f000 8394 	beq.w	3000a7cc <rxi316_perf_tune+0x86c>
3000a0a4:	2a0e      	cmp	r2, #14
3000a0a6:	f000 838c 	beq.w	3000a7c2 <rxi316_perf_tune+0x862>
3000a0aa:	2a0f      	cmp	r2, #15
3000a0ac:	f000 8393 	beq.w	3000a7d6 <rxi316_perf_tune+0x876>
3000a0b0:	2a10      	cmp	r2, #16
3000a0b2:	f000 8395 	beq.w	3000a7e0 <rxi316_perf_tune+0x880>
3000a0b6:	2a11      	cmp	r2, #17
3000a0b8:	f000 8394 	beq.w	3000a7e4 <rxi316_perf_tune+0x884>
3000a0bc:	2a12      	cmp	r2, #18
3000a0be:	f000 8393 	beq.w	3000a7e8 <rxi316_perf_tune+0x888>
3000a0c2:	2a13      	cmp	r2, #19
3000a0c4:	f000 8394 	beq.w	3000a7f0 <rxi316_perf_tune+0x890>
3000a0c8:	2a14      	cmp	r2, #20
3000a0ca:	f000 838f 	beq.w	3000a7ec <rxi316_perf_tune+0x88c>
3000a0ce:	2a15      	cmp	r2, #21
3000a0d0:	f000 8396 	beq.w	3000a800 <rxi316_perf_tune+0x8a0>
3000a0d4:	2a16      	cmp	r2, #22
3000a0d6:	f000 8391 	beq.w	3000a7fc <rxi316_perf_tune+0x89c>
3000a0da:	2a17      	cmp	r2, #23
3000a0dc:	f000 83a0 	beq.w	3000a820 <rxi316_perf_tune+0x8c0>
3000a0e0:	2a18      	cmp	r2, #24
3000a0e2:	f000 839b 	beq.w	3000a81c <rxi316_perf_tune+0x8bc>
3000a0e6:	2a19      	cmp	r2, #25
3000a0e8:	f000 8396 	beq.w	3000a818 <rxi316_perf_tune+0x8b8>
3000a0ec:	2a1a      	cmp	r2, #26
3000a0ee:	f000 8391 	beq.w	3000a814 <rxi316_perf_tune+0x8b4>
3000a0f2:	2a1b      	cmp	r2, #27
3000a0f4:	f000 838c 	beq.w	3000a810 <rxi316_perf_tune+0x8b0>
3000a0f8:	2a1c      	cmp	r2, #28
3000a0fa:	f000 8387 	beq.w	3000a80c <rxi316_perf_tune+0x8ac>
3000a0fe:	2a1d      	cmp	r2, #29
3000a100:	f000 8382 	beq.w	3000a808 <rxi316_perf_tune+0x8a8>
3000a104:	2a1e      	cmp	r2, #30
3000a106:	f000 837d 	beq.w	3000a804 <rxi316_perf_tune+0x8a4>
3000a10a:	2a1f      	cmp	r2, #31
3000a10c:	f000 8374 	beq.w	3000a7f8 <rxi316_perf_tune+0x898>
3000a110:	2a20      	cmp	r2, #32
3000a112:	f000 836f 	beq.w	3000a7f4 <rxi316_perf_tune+0x894>
3000a116:	9809      	ldr	r0, [sp, #36]	; 0x24
3000a118:	2a21      	cmp	r2, #33	; 0x21
3000a11a:	bf08      	it	eq
3000a11c:	4618      	moveq	r0, r3
3000a11e:	9009      	str	r0, [sp, #36]	; 0x24
3000a120:	e797      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a122:	4698      	mov	r8, r3
3000a124:	3301      	adds	r3, #1
3000a126:	3101      	adds	r1, #1
3000a128:	2b20      	cmp	r3, #32
3000a12a:	d195      	bne.n	3000a058 <rxi316_perf_tune+0xf8>
3000a12c:	f89c 2002 	ldrb.w	r2, [ip, #2]
3000a130:	f8dc 3008 	ldr.w	r3, [ip, #8]
3000a134:	b1ba      	cbz	r2, 3000a166 <rxi316_perf_tune+0x206>
3000a136:	2a01      	cmp	r2, #1
3000a138:	f000 8331 	beq.w	3000a79e <rxi316_perf_tune+0x83e>
3000a13c:	2a03      	cmp	r2, #3
3000a13e:	bf0c      	ite	eq
3000a140:	2204      	moveq	r2, #4
3000a142:	2203      	movne	r2, #3
3000a144:	921e      	str	r2, [sp, #120]	; 0x78
3000a146:	e010      	b.n	3000a16a <rxi316_perf_tune+0x20a>
3000a148:	3101      	adds	r1, #1
3000a14a:	461f      	mov	r7, r3
3000a14c:	e781      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a14e:	3101      	adds	r1, #1
3000a150:	469a      	mov	sl, r3
3000a152:	e77e      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a154:	3101      	adds	r1, #1
3000a156:	469b      	mov	fp, r3
3000a158:	e77b      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a15a:	3101      	adds	r1, #1
3000a15c:	9301      	str	r3, [sp, #4]
3000a15e:	e778      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a160:	3101      	adds	r1, #1
3000a162:	9302      	str	r3, [sp, #8]
3000a164:	e775      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a166:	2201      	movs	r2, #1
3000a168:	921e      	str	r2, [sp, #120]	; 0x78
3000a16a:	3b02      	subs	r3, #2
3000a16c:	428b      	cmp	r3, r1
3000a16e:	d005      	beq.n	3000a17c <rxi316_perf_tune+0x21c>
3000a170:	4bac      	ldr	r3, [pc, #688]	; (3000a424 <rxi316_perf_tune+0x4c4>)
3000a172:	2002      	movs	r0, #2
3000a174:	4aac      	ldr	r2, [pc, #688]	; (3000a428 <rxi316_perf_tune+0x4c8>)
3000a176:	4619      	mov	r1, r3
3000a178:	f001 feb8 	bl	3000beec <rtk_log_write>
3000a17c:	9b1e      	ldr	r3, [sp, #120]	; 0x78
3000a17e:	9a03      	ldr	r2, [sp, #12]
3000a180:	4293      	cmp	r3, r2
3000a182:	d005      	beq.n	3000a190 <rxi316_perf_tune+0x230>
3000a184:	4ba7      	ldr	r3, [pc, #668]	; (3000a424 <rxi316_perf_tune+0x4c4>)
3000a186:	2002      	movs	r0, #2
3000a188:	4aa8      	ldr	r2, [pc, #672]	; (3000a42c <rxi316_perf_tune+0x4cc>)
3000a18a:	4619      	mov	r1, r3
3000a18c:	f001 feae 	bl	3000beec <rtk_log_write>
3000a190:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a192:	2b20      	cmp	r3, #32
3000a194:	f000 82f1 	beq.w	3000a77a <rxi316_perf_tune+0x81a>
3000a198:	1b5a      	subs	r2, r3, r5
3000a19a:	3a05      	subs	r2, #5
3000a19c:	f002 030f 	and.w	r3, r2, #15
3000a1a0:	9303      	str	r3, [sp, #12]
3000a1a2:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a1a4:	2b20      	cmp	r3, #32
3000a1a6:	f000 82e6 	beq.w	3000a776 <rxi316_perf_tune+0x816>
3000a1aa:	1b5a      	subs	r2, r3, r5
3000a1ac:	3a06      	subs	r2, #6
3000a1ae:	0112      	lsls	r2, r2, #4
3000a1b0:	b2d2      	uxtb	r2, r2
3000a1b2:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a1b4:	2b20      	cmp	r3, #32
3000a1b6:	f000 82da 	beq.w	3000a76e <rxi316_perf_tune+0x80e>
3000a1ba:	1b5b      	subs	r3, r3, r5
3000a1bc:	3b07      	subs	r3, #7
3000a1be:	021b      	lsls	r3, r3, #8
3000a1c0:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a1c4:	930c      	str	r3, [sp, #48]	; 0x30
3000a1c6:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a1c8:	2b20      	cmp	r3, #32
3000a1ca:	f000 820b 	beq.w	3000a5e4 <rxi316_perf_tune+0x684>
3000a1ce:	1b5b      	subs	r3, r3, r5
3000a1d0:	3b08      	subs	r3, #8
3000a1d2:	031b      	lsls	r3, r3, #12
3000a1d4:	b29b      	uxth	r3, r3
3000a1d6:	930b      	str	r3, [sp, #44]	; 0x2c
3000a1d8:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a1da:	2b20      	cmp	r3, #32
3000a1dc:	f000 8209 	beq.w	3000a5f2 <rxi316_perf_tune+0x692>
3000a1e0:	1b5b      	subs	r3, r3, r5
3000a1e2:	3b09      	subs	r3, #9
3000a1e4:	041b      	lsls	r3, r3, #16
3000a1e6:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a1ea:	930a      	str	r3, [sp, #40]	; 0x28
3000a1ec:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a1ee:	2b20      	cmp	r3, #32
3000a1f0:	f000 8206 	beq.w	3000a600 <rxi316_perf_tune+0x6a0>
3000a1f4:	1b5b      	subs	r3, r3, r5
3000a1f6:	3b0a      	subs	r3, #10
3000a1f8:	051b      	lsls	r3, r3, #20
3000a1fa:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a1fe:	930d      	str	r3, [sp, #52]	; 0x34
3000a200:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a202:	2b20      	cmp	r3, #32
3000a204:	f000 8203 	beq.w	3000a60e <rxi316_perf_tune+0x6ae>
3000a208:	1b5b      	subs	r3, r3, r5
3000a20a:	3b0b      	subs	r3, #11
3000a20c:	061b      	lsls	r3, r3, #24
3000a20e:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a212:	930e      	str	r3, [sp, #56]	; 0x38
3000a214:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a216:	2b20      	cmp	r3, #32
3000a218:	f000 8200 	beq.w	3000a61c <rxi316_perf_tune+0x6bc>
3000a21c:	1b5b      	subs	r3, r3, r5
3000a21e:	3b0c      	subs	r3, #12
3000a220:	071b      	lsls	r3, r3, #28
3000a222:	930f      	str	r3, [sp, #60]	; 0x3c
3000a224:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a226:	2b20      	cmp	r3, #32
3000a228:	f000 81ff 	beq.w	3000a62a <rxi316_perf_tune+0x6ca>
3000a22c:	1b5b      	subs	r3, r3, r5
3000a22e:	3b0d      	subs	r3, #13
3000a230:	f003 030f 	and.w	r3, r3, #15
3000a234:	9310      	str	r3, [sp, #64]	; 0x40
3000a236:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a238:	2b20      	cmp	r3, #32
3000a23a:	f000 81fc 	beq.w	3000a636 <rxi316_perf_tune+0x6d6>
3000a23e:	1b5b      	subs	r3, r3, r5
3000a240:	3b0e      	subs	r3, #14
3000a242:	011b      	lsls	r3, r3, #4
3000a244:	b2db      	uxtb	r3, r3
3000a246:	9311      	str	r3, [sp, #68]	; 0x44
3000a248:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a24a:	2b20      	cmp	r3, #32
3000a24c:	f000 81f9 	beq.w	3000a642 <rxi316_perf_tune+0x6e2>
3000a250:	1b5b      	subs	r3, r3, r5
3000a252:	3b0f      	subs	r3, #15
3000a254:	021b      	lsls	r3, r3, #8
3000a256:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a25a:	9312      	str	r3, [sp, #72]	; 0x48
3000a25c:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a25e:	2b20      	cmp	r3, #32
3000a260:	f000 81f6 	beq.w	3000a650 <rxi316_perf_tune+0x6f0>
3000a264:	1b5b      	subs	r3, r3, r5
3000a266:	031b      	lsls	r3, r3, #12
3000a268:	f403 4370 	and.w	r3, r3, #61440	; 0xf000
3000a26c:	9321      	str	r3, [sp, #132]	; 0x84
3000a26e:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a270:	2b20      	cmp	r3, #32
3000a272:	f000 81f4 	beq.w	3000a65e <rxi316_perf_tune+0x6fe>
3000a276:	1b5b      	subs	r3, r3, r5
3000a278:	3b11      	subs	r3, #17
3000a27a:	041b      	lsls	r3, r3, #16
3000a27c:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a280:	9320      	str	r3, [sp, #128]	; 0x80
3000a282:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a284:	2b20      	cmp	r3, #32
3000a286:	f000 81f1 	beq.w	3000a66c <rxi316_perf_tune+0x70c>
3000a28a:	1b5b      	subs	r3, r3, r5
3000a28c:	3b12      	subs	r3, #18
3000a28e:	051b      	lsls	r3, r3, #20
3000a290:	f403 0370 	and.w	r3, r3, #15728640	; 0xf00000
3000a294:	931f      	str	r3, [sp, #124]	; 0x7c
3000a296:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a298:	2b20      	cmp	r3, #32
3000a29a:	f000 81ee 	beq.w	3000a67a <rxi316_perf_tune+0x71a>
3000a29e:	1b5b      	subs	r3, r3, r5
3000a2a0:	3b13      	subs	r3, #19
3000a2a2:	061b      	lsls	r3, r3, #24
3000a2a4:	f003 6370 	and.w	r3, r3, #251658240	; 0xf000000
3000a2a8:	931e      	str	r3, [sp, #120]	; 0x78
3000a2aa:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a2ac:	2b20      	cmp	r3, #32
3000a2ae:	f000 81eb 	beq.w	3000a688 <rxi316_perf_tune+0x728>
3000a2b2:	1b5b      	subs	r3, r3, r5
3000a2b4:	3b14      	subs	r3, #20
3000a2b6:	071b      	lsls	r3, r3, #28
3000a2b8:	9318      	str	r3, [sp, #96]	; 0x60
3000a2ba:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a2bc:	2b20      	cmp	r3, #32
3000a2be:	f000 81ea 	beq.w	3000a696 <rxi316_perf_tune+0x736>
3000a2c2:	1b5b      	subs	r3, r3, r5
3000a2c4:	3b15      	subs	r3, #21
3000a2c6:	f003 030f 	and.w	r3, r3, #15
3000a2ca:	9317      	str	r3, [sp, #92]	; 0x5c
3000a2cc:	9b07      	ldr	r3, [sp, #28]
3000a2ce:	2b20      	cmp	r3, #32
3000a2d0:	f000 81e7 	beq.w	3000a6a2 <rxi316_perf_tune+0x742>
3000a2d4:	1b5b      	subs	r3, r3, r5
3000a2d6:	3b02      	subs	r3, #2
3000a2d8:	f003 031f 	and.w	r3, r3, #31
3000a2dc:	9316      	str	r3, [sp, #88]	; 0x58
3000a2de:	9b08      	ldr	r3, [sp, #32]
3000a2e0:	2b20      	cmp	r3, #32
3000a2e2:	f000 81e4 	beq.w	3000a6ae <rxi316_perf_tune+0x74e>
3000a2e6:	1b5b      	subs	r3, r3, r5
3000a2e8:	3b02      	subs	r3, #2
3000a2ea:	015b      	lsls	r3, r3, #5
3000a2ec:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a2f0:	9315      	str	r3, [sp, #84]	; 0x54
3000a2f2:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a2f4:	2b20      	cmp	r3, #32
3000a2f6:	f000 81e1 	beq.w	3000a6bc <rxi316_perf_tune+0x75c>
3000a2fa:	1b5b      	subs	r3, r3, r5
3000a2fc:	f1b9 0f20 	cmp.w	r9, #32
3000a300:	f1a3 0302 	sub.w	r3, r3, #2
3000a304:	ea4f 2383 	mov.w	r3, r3, lsl #10
3000a308:	f403 43f8 	and.w	r3, r3, #31744	; 0x7c00
3000a30c:	9314      	str	r3, [sp, #80]	; 0x50
3000a30e:	f000 81dc 	beq.w	3000a6ca <rxi316_perf_tune+0x76a>
3000a312:	eba9 0105 	sub.w	r1, r9, r5
3000a316:	f1b8 0f20 	cmp.w	r8, #32
3000a31a:	f1a1 0102 	sub.w	r1, r1, #2
3000a31e:	f001 030f 	and.w	r3, r1, #15
3000a322:	9313      	str	r3, [sp, #76]	; 0x4c
3000a324:	f000 81d7 	beq.w	3000a6d6 <rxi316_perf_tune+0x776>
3000a328:	eba8 0105 	sub.w	r1, r8, r5
3000a32c:	2f20      	cmp	r7, #32
3000a32e:	f1a1 0103 	sub.w	r1, r1, #3
3000a332:	ea4f 1101 	mov.w	r1, r1, lsl #4
3000a336:	b2c9      	uxtb	r1, r1
3000a338:	f000 81d2 	beq.w	3000a6e0 <rxi316_perf_tune+0x780>
3000a33c:	1b7b      	subs	r3, r7, r5
3000a33e:	f1ba 0f20 	cmp.w	sl, #32
3000a342:	f1a3 0304 	sub.w	r3, r3, #4
3000a346:	ea4f 2303 	mov.w	r3, r3, lsl #8
3000a34a:	f403 6370 	and.w	r3, r3, #3840	; 0xf00
3000a34e:	9308      	str	r3, [sp, #32]
3000a350:	f000 81cd 	beq.w	3000a6ee <rxi316_perf_tune+0x78e>
3000a354:	ebaa 0305 	sub.w	r3, sl, r5
3000a358:	f1bb 0f20 	cmp.w	fp, #32
3000a35c:	f1a3 0305 	sub.w	r3, r3, #5
3000a360:	ea4f 3303 	mov.w	r3, r3, lsl #12
3000a364:	b29b      	uxth	r3, r3
3000a366:	9307      	str	r3, [sp, #28]
3000a368:	f000 81c8 	beq.w	3000a6fc <rxi316_perf_tune+0x79c>
3000a36c:	ebab 0c05 	sub.w	ip, fp, r5
3000a370:	9b01      	ldr	r3, [sp, #4]
3000a372:	f1ac 0c06 	sub.w	ip, ip, #6
3000a376:	2b20      	cmp	r3, #32
3000a378:	ea4f 4c0c 	mov.w	ip, ip, lsl #16
3000a37c:	f40c 2c70 	and.w	ip, ip, #983040	; 0xf0000
3000a380:	f000 81c2 	beq.w	3000a708 <rxi316_perf_tune+0x7a8>
3000a384:	eba3 0e05 	sub.w	lr, r3, r5
3000a388:	9b02      	ldr	r3, [sp, #8]
3000a38a:	f1ae 0e07 	sub.w	lr, lr, #7
3000a38e:	2b20      	cmp	r3, #32
3000a390:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a394:	f40e 0e70 	and.w	lr, lr, #15728640	; 0xf00000
3000a398:	f000 81bc 	beq.w	3000a714 <rxi316_perf_tune+0x7b4>
3000a39c:	eba3 0805 	sub.w	r8, r3, r5
3000a3a0:	9b04      	ldr	r3, [sp, #16]
3000a3a2:	f1a8 0808 	sub.w	r8, r8, #8
3000a3a6:	2b20      	cmp	r3, #32
3000a3a8:	ea4f 6808 	mov.w	r8, r8, lsl #24
3000a3ac:	f008 6870 	and.w	r8, r8, #251658240	; 0xf000000
3000a3b0:	f000 81b6 	beq.w	3000a720 <rxi316_perf_tune+0x7c0>
3000a3b4:	1b5b      	subs	r3, r3, r5
3000a3b6:	3b09      	subs	r3, #9
3000a3b8:	071b      	lsls	r3, r3, #28
3000a3ba:	9302      	str	r3, [sp, #8]
3000a3bc:	9b05      	ldr	r3, [sp, #20]
3000a3be:	2b20      	cmp	r3, #32
3000a3c0:	f000 81b5 	beq.w	3000a72e <rxi316_perf_tune+0x7ce>
3000a3c4:	eba3 0905 	sub.w	r9, r3, r5
3000a3c8:	9b06      	ldr	r3, [sp, #24]
3000a3ca:	f1a9 090a 	sub.w	r9, r9, #10
3000a3ce:	2b20      	cmp	r3, #32
3000a3d0:	f009 090f 	and.w	r9, r9, #15
3000a3d4:	f000 81b1 	beq.w	3000a73a <rxi316_perf_tune+0x7da>
3000a3d8:	1b58      	subs	r0, r3, r5
3000a3da:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a3dc:	380b      	subs	r0, #11
3000a3de:	2b20      	cmp	r3, #32
3000a3e0:	ea4f 1000 	mov.w	r0, r0, lsl #4
3000a3e4:	b2c0      	uxtb	r0, r0
3000a3e6:	f000 81ad 	beq.w	3000a744 <rxi316_perf_tune+0x7e4>
3000a3ea:	eba3 0b05 	sub.w	fp, r3, r5
3000a3ee:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a3f0:	f1ab 0b02 	sub.w	fp, fp, #2
3000a3f4:	2b20      	cmp	r3, #32
3000a3f6:	f00b 0b1f 	and.w	fp, fp, #31
3000a3fa:	f000 81a9 	beq.w	3000a750 <rxi316_perf_tune+0x7f0>
3000a3fe:	eba3 0a05 	sub.w	sl, r3, r5
3000a402:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a404:	f1aa 0a02 	sub.w	sl, sl, #2
3000a408:	2b20      	cmp	r3, #32
3000a40a:	f00a 0a1f 	and.w	sl, sl, #31
3000a40e:	f000 81a5 	beq.w	3000a75c <rxi316_perf_tune+0x7fc>
3000a412:	1b5f      	subs	r7, r3, r5
3000a414:	2c20      	cmp	r4, #32
3000a416:	f1a7 0702 	sub.w	r7, r7, #2
3000a41a:	f007 071f 	and.w	r7, r7, #31
3000a41e:	f000 81a2 	beq.w	3000a766 <rxi316_perf_tune+0x806>
3000a422:	e005      	b.n	3000a430 <rxi316_perf_tune+0x4d0>
3000a424:	3000d84c 	.word	0x3000d84c
3000a428:	3000d874 	.word	0x3000d874
3000a42c:	3000d8b8 	.word	0x3000d8b8
3000a430:	1b63      	subs	r3, r4, r5
3000a432:	3b02      	subs	r3, #2
3000a434:	015b      	lsls	r3, r3, #5
3000a436:	f403 7378 	and.w	r3, r3, #992	; 0x3e0
3000a43a:	9301      	str	r3, [sp, #4]
3000a43c:	9c03      	ldr	r4, [sp, #12]
3000a43e:	ea40 0009 	orr.w	r0, r0, r9
3000a442:	4314      	orrs	r4, r2
3000a444:	4622      	mov	r2, r4
3000a446:	e9dd 4510 	ldrd	r4, r5, [sp, #64]	; 0x40
3000a44a:	432c      	orrs	r4, r5
3000a44c:	9d13      	ldr	r5, [sp, #76]	; 0x4c
3000a44e:	4329      	orrs	r1, r5
3000a450:	e9dd 5315 	ldrd	r5, r3, [sp, #84]	; 0x54
3000a454:	431d      	orrs	r5, r3
3000a456:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000a458:	431a      	orrs	r2, r3
3000a45a:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a45c:	431c      	orrs	r4, r3
3000a45e:	9b08      	ldr	r3, [sp, #32]
3000a460:	4319      	orrs	r1, r3
3000a462:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a464:	431d      	orrs	r5, r3
3000a466:	9b0b      	ldr	r3, [sp, #44]	; 0x2c
3000a468:	431a      	orrs	r2, r3
3000a46a:	9b21      	ldr	r3, [sp, #132]	; 0x84
3000a46c:	431c      	orrs	r4, r3
3000a46e:	9b07      	ldr	r3, [sp, #28]
3000a470:	4319      	orrs	r1, r3
3000a472:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a474:	431a      	orrs	r2, r3
3000a476:	9b20      	ldr	r3, [sp, #128]	; 0x80
3000a478:	ea41 010c 	orr.w	r1, r1, ip
3000a47c:	431c      	orrs	r4, r3
3000a47e:	9b01      	ldr	r3, [sp, #4]
3000a480:	ea41 010e 	orr.w	r1, r1, lr
3000a484:	431f      	orrs	r7, r3
3000a486:	9b0d      	ldr	r3, [sp, #52]	; 0x34
3000a488:	ea41 0108 	orr.w	r1, r1, r8
3000a48c:	431a      	orrs	r2, r3
3000a48e:	9b1f      	ldr	r3, [sp, #124]	; 0x7c
3000a490:	431c      	orrs	r4, r3
3000a492:	9b0e      	ldr	r3, [sp, #56]	; 0x38
3000a494:	431a      	orrs	r2, r3
3000a496:	9b1e      	ldr	r3, [sp, #120]	; 0x78
3000a498:	4323      	orrs	r3, r4
3000a49a:	9c0f      	ldr	r4, [sp, #60]	; 0x3c
3000a49c:	4322      	orrs	r2, r4
3000a49e:	9c18      	ldr	r4, [sp, #96]	; 0x60
3000a4a0:	4323      	orrs	r3, r4
3000a4a2:	9c02      	ldr	r4, [sp, #8]
3000a4a4:	430c      	orrs	r4, r1
3000a4a6:	49ba      	ldr	r1, [pc, #744]	; (3000a790 <rxi316_perf_tune+0x830>)
3000a4a8:	f8c1 2400 	str.w	r2, [r1, #1024]	; 0x400
3000a4ac:	f8c1 3404 	str.w	r3, [r1, #1028]	; 0x404
3000a4b0:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a4b2:	f8c1 3408 	str.w	r3, [r1, #1032]	; 0x408
3000a4b6:	f8c1 540c 	str.w	r5, [r1, #1036]	; 0x40c
3000a4ba:	f8c1 4410 	str.w	r4, [r1, #1040]	; 0x410
3000a4be:	f8c1 0414 	str.w	r0, [r1, #1044]	; 0x414
3000a4c2:	f8c1 b418 	str.w	fp, [r1, #1048]	; 0x418
3000a4c6:	f8c1 a41c 	str.w	sl, [r1, #1052]	; 0x41c
3000a4ca:	f8c1 7420 	str.w	r7, [r1, #1056]	; 0x420
3000a4ce:	68f3      	ldr	r3, [r6, #12]
3000a4d0:	689a      	ldr	r2, [r3, #8]
3000a4d2:	e9d2 3200 	ldrd	r3, r2, [r2]
3000a4d6:	f003 0301 	and.w	r3, r3, #1
3000a4da:	ea43 4302 	orr.w	r3, r3, r2, lsl #16
3000a4de:	f8c1 3460 	str.w	r3, [r1, #1120]	; 0x460
3000a4e2:	68f3      	ldr	r3, [r6, #12]
3000a4e4:	689a      	ldr	r2, [r3, #8]
3000a4e6:	7a13      	ldrb	r3, [r2, #8]
3000a4e8:	6950      	ldr	r0, [r2, #20]
3000a4ea:	ea43 6300 	orr.w	r3, r3, r0, lsl #24
3000a4ee:	e9d2 0203 	ldrd	r0, r2, [r2, #12]
3000a4f2:	0200      	lsls	r0, r0, #8
3000a4f4:	0412      	lsls	r2, r2, #16
3000a4f6:	b280      	uxth	r0, r0
3000a4f8:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a4fc:	4303      	orrs	r3, r0
3000a4fe:	4313      	orrs	r3, r2
3000a500:	f8c1 3464 	str.w	r3, [r1, #1124]	; 0x464
3000a504:	68f3      	ldr	r3, [r6, #12]
3000a506:	6898      	ldr	r0, [r3, #8]
3000a508:	e9d0 3207 	ldrd	r3, r2, [r0, #28]
3000a50c:	7e00      	ldrb	r0, [r0, #24]
3000a50e:	021b      	lsls	r3, r3, #8
3000a510:	0412      	lsls	r2, r2, #16
3000a512:	b29b      	uxth	r3, r3
3000a514:	f402 027f 	and.w	r2, r2, #16711680	; 0xff0000
3000a518:	4313      	orrs	r3, r2
3000a51a:	4303      	orrs	r3, r0
3000a51c:	f8c1 3468 	str.w	r3, [r1, #1128]	; 0x468
3000a520:	68f3      	ldr	r3, [r6, #12]
3000a522:	689a      	ldr	r2, [r3, #8]
3000a524:	e9d2 2309 	ldrd	r2, r3, [r2, #36]	; 0x24
3000a528:	041b      	lsls	r3, r3, #16
3000a52a:	f002 023f 	and.w	r2, r2, #63	; 0x3f
3000a52e:	f403 2370 	and.w	r3, r3, #983040	; 0xf0000
3000a532:	4313      	orrs	r3, r2
3000a534:	f8c1 3480 	str.w	r3, [r1, #1152]	; 0x480
3000a538:	6833      	ldr	r3, [r6, #0]
3000a53a:	781b      	ldrb	r3, [r3, #0]
3000a53c:	2b03      	cmp	r3, #3
3000a53e:	f000 811f 	beq.w	3000a780 <rxi316_perf_tune+0x820>
3000a542:	68f3      	ldr	r3, [r6, #12]
3000a544:	6899      	ldr	r1, [r3, #8]
3000a546:	6b0b      	ldr	r3, [r1, #48]	; 0x30
3000a548:	005b      	lsls	r3, r3, #1
3000a54a:	f003 0302 	and.w	r3, r3, #2
3000a54e:	6b8a      	ldr	r2, [r1, #56]	; 0x38
3000a550:	6ac8      	ldr	r0, [r1, #44]	; 0x2c
3000a552:	0412      	lsls	r2, r2, #16
3000a554:	6b49      	ldr	r1, [r1, #52]	; 0x34
3000a556:	f000 0001 	and.w	r0, r0, #1
3000a55a:	f402 2270 	and.w	r2, r2, #983040	; 0xf0000
3000a55e:	0209      	lsls	r1, r1, #8
3000a560:	431a      	orrs	r2, r3
3000a562:	f401 6170 	and.w	r1, r1, #3840	; 0xf00
3000a566:	4b8a      	ldr	r3, [pc, #552]	; (3000a790 <rxi316_perf_tune+0x830>)
3000a568:	4302      	orrs	r2, r0
3000a56a:	488a      	ldr	r0, [pc, #552]	; (3000a794 <rxi316_perf_tune+0x834>)
3000a56c:	430a      	orrs	r2, r1
3000a56e:	4310      	orrs	r0, r2
3000a570:	f8c3 0484 	str.w	r0, [r3, #1156]	; 0x484
3000a574:	68f2      	ldr	r2, [r6, #12]
3000a576:	6892      	ldr	r2, [r2, #8]
3000a578:	6bd2      	ldr	r2, [r2, #60]	; 0x3c
3000a57a:	f8c3 2488 	str.w	r2, [r3, #1160]	; 0x488
3000a57e:	68f2      	ldr	r2, [r6, #12]
3000a580:	6892      	ldr	r2, [r2, #8]
3000a582:	6c12      	ldr	r2, [r2, #64]	; 0x40
3000a584:	f8c3 248c 	str.w	r2, [r3, #1164]	; 0x48c
3000a588:	68f2      	ldr	r2, [r6, #12]
3000a58a:	6892      	ldr	r2, [r2, #8]
3000a58c:	6c52      	ldr	r2, [r2, #68]	; 0x44
3000a58e:	f8c3 2490 	str.w	r2, [r3, #1168]	; 0x490
3000a592:	68f2      	ldr	r2, [r6, #12]
3000a594:	6892      	ldr	r2, [r2, #8]
3000a596:	6c92      	ldr	r2, [r2, #72]	; 0x48
3000a598:	f8c3 2494 	str.w	r2, [r3, #1172]	; 0x494
3000a59c:	68f2      	ldr	r2, [r6, #12]
3000a59e:	6892      	ldr	r2, [r2, #8]
3000a5a0:	6cd2      	ldr	r2, [r2, #76]	; 0x4c
3000a5a2:	f8c3 2498 	str.w	r2, [r3, #1176]	; 0x498
3000a5a6:	68f2      	ldr	r2, [r6, #12]
3000a5a8:	6892      	ldr	r2, [r2, #8]
3000a5aa:	6d12      	ldr	r2, [r2, #80]	; 0x50
3000a5ac:	f002 020f 	and.w	r2, r2, #15
3000a5b0:	f8c3 24a0 	str.w	r2, [r3, #1184]	; 0x4a0
3000a5b4:	f8d3 2470 	ldr.w	r2, [r3, #1136]	; 0x470
3000a5b8:	f022 023f 	bic.w	r2, r2, #63	; 0x3f
3000a5bc:	f042 0204 	orr.w	r2, r2, #4
3000a5c0:	f8c3 2470 	str.w	r2, [r3, #1136]	; 0x470
3000a5c4:	f8d3 2474 	ldr.w	r2, [r3, #1140]	; 0x474
3000a5c8:	f422 4270 	bic.w	r2, r2, #61440	; 0xf000
3000a5cc:	f442 4280 	orr.w	r2, r2, #16384	; 0x4000
3000a5d0:	f8c3 2474 	str.w	r2, [r3, #1140]	; 0x474
3000a5d4:	b043      	add	sp, #268	; 0x10c
3000a5d6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000a5da:	2502      	movs	r5, #2
3000a5dc:	e4cf      	b.n	30009f7e <rxi316_perf_tune+0x1e>
3000a5de:	3101      	adds	r1, #1
3000a5e0:	9304      	str	r3, [sp, #16]
3000a5e2:	e536      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a5e4:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a5e8:	930b      	str	r3, [sp, #44]	; 0x2c
3000a5ea:	9b0a      	ldr	r3, [sp, #40]	; 0x28
3000a5ec:	2b20      	cmp	r3, #32
3000a5ee:	f47f adf7 	bne.w	3000a1e0 <rxi316_perf_tune+0x280>
3000a5f2:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a5f6:	930a      	str	r3, [sp, #40]	; 0x28
3000a5f8:	9b0f      	ldr	r3, [sp, #60]	; 0x3c
3000a5fa:	2b20      	cmp	r3, #32
3000a5fc:	f47f adfa 	bne.w	3000a1f4 <rxi316_perf_tune+0x294>
3000a600:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a604:	930d      	str	r3, [sp, #52]	; 0x34
3000a606:	9b10      	ldr	r3, [sp, #64]	; 0x40
3000a608:	2b20      	cmp	r3, #32
3000a60a:	f47f adfd 	bne.w	3000a208 <rxi316_perf_tune+0x2a8>
3000a60e:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a612:	930e      	str	r3, [sp, #56]	; 0x38
3000a614:	9b11      	ldr	r3, [sp, #68]	; 0x44
3000a616:	2b20      	cmp	r3, #32
3000a618:	f47f ae00 	bne.w	3000a21c <rxi316_perf_tune+0x2bc>
3000a61c:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a620:	930f      	str	r3, [sp, #60]	; 0x3c
3000a622:	9b12      	ldr	r3, [sp, #72]	; 0x48
3000a624:	2b20      	cmp	r3, #32
3000a626:	f47f ae01 	bne.w	3000a22c <rxi316_perf_tune+0x2cc>
3000a62a:	230f      	movs	r3, #15
3000a62c:	9310      	str	r3, [sp, #64]	; 0x40
3000a62e:	9b13      	ldr	r3, [sp, #76]	; 0x4c
3000a630:	2b20      	cmp	r3, #32
3000a632:	f47f ae04 	bne.w	3000a23e <rxi316_perf_tune+0x2de>
3000a636:	23f0      	movs	r3, #240	; 0xf0
3000a638:	9311      	str	r3, [sp, #68]	; 0x44
3000a63a:	9b14      	ldr	r3, [sp, #80]	; 0x50
3000a63c:	2b20      	cmp	r3, #32
3000a63e:	f47f ae07 	bne.w	3000a250 <rxi316_perf_tune+0x2f0>
3000a642:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a646:	9312      	str	r3, [sp, #72]	; 0x48
3000a648:	9b15      	ldr	r3, [sp, #84]	; 0x54
3000a64a:	2b20      	cmp	r3, #32
3000a64c:	f47f ae0a 	bne.w	3000a264 <rxi316_perf_tune+0x304>
3000a650:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a654:	9321      	str	r3, [sp, #132]	; 0x84
3000a656:	9b16      	ldr	r3, [sp, #88]	; 0x58
3000a658:	2b20      	cmp	r3, #32
3000a65a:	f47f ae0c 	bne.w	3000a276 <rxi316_perf_tune+0x316>
3000a65e:	f44f 2370 	mov.w	r3, #983040	; 0xf0000
3000a662:	9320      	str	r3, [sp, #128]	; 0x80
3000a664:	9b17      	ldr	r3, [sp, #92]	; 0x5c
3000a666:	2b20      	cmp	r3, #32
3000a668:	f47f ae0f 	bne.w	3000a28a <rxi316_perf_tune+0x32a>
3000a66c:	f44f 0370 	mov.w	r3, #15728640	; 0xf00000
3000a670:	931f      	str	r3, [sp, #124]	; 0x7c
3000a672:	9b18      	ldr	r3, [sp, #96]	; 0x60
3000a674:	2b20      	cmp	r3, #32
3000a676:	f47f ae12 	bne.w	3000a29e <rxi316_perf_tune+0x33e>
3000a67a:	f04f 6370 	mov.w	r3, #251658240	; 0xf000000
3000a67e:	931e      	str	r3, [sp, #120]	; 0x78
3000a680:	9b19      	ldr	r3, [sp, #100]	; 0x64
3000a682:	2b20      	cmp	r3, #32
3000a684:	f47f ae15 	bne.w	3000a2b2 <rxi316_perf_tune+0x352>
3000a688:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a68c:	9318      	str	r3, [sp, #96]	; 0x60
3000a68e:	9b1a      	ldr	r3, [sp, #104]	; 0x68
3000a690:	2b20      	cmp	r3, #32
3000a692:	f47f ae16 	bne.w	3000a2c2 <rxi316_perf_tune+0x362>
3000a696:	230f      	movs	r3, #15
3000a698:	9317      	str	r3, [sp, #92]	; 0x5c
3000a69a:	9b07      	ldr	r3, [sp, #28]
3000a69c:	2b20      	cmp	r3, #32
3000a69e:	f47f ae19 	bne.w	3000a2d4 <rxi316_perf_tune+0x374>
3000a6a2:	231f      	movs	r3, #31
3000a6a4:	9316      	str	r3, [sp, #88]	; 0x58
3000a6a6:	9b08      	ldr	r3, [sp, #32]
3000a6a8:	2b20      	cmp	r3, #32
3000a6aa:	f47f ae1c 	bne.w	3000a2e6 <rxi316_perf_tune+0x386>
3000a6ae:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a6b2:	9315      	str	r3, [sp, #84]	; 0x54
3000a6b4:	9b1b      	ldr	r3, [sp, #108]	; 0x6c
3000a6b6:	2b20      	cmp	r3, #32
3000a6b8:	f47f ae1f 	bne.w	3000a2fa <rxi316_perf_tune+0x39a>
3000a6bc:	f44f 43f8 	mov.w	r3, #31744	; 0x7c00
3000a6c0:	f1b9 0f20 	cmp.w	r9, #32
3000a6c4:	9314      	str	r3, [sp, #80]	; 0x50
3000a6c6:	f47f ae24 	bne.w	3000a312 <rxi316_perf_tune+0x3b2>
3000a6ca:	230f      	movs	r3, #15
3000a6cc:	f1b8 0f20 	cmp.w	r8, #32
3000a6d0:	9313      	str	r3, [sp, #76]	; 0x4c
3000a6d2:	f47f ae29 	bne.w	3000a328 <rxi316_perf_tune+0x3c8>
3000a6d6:	2f20      	cmp	r7, #32
3000a6d8:	f04f 01f0 	mov.w	r1, #240	; 0xf0
3000a6dc:	f47f ae2e 	bne.w	3000a33c <rxi316_perf_tune+0x3dc>
3000a6e0:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a6e4:	f1ba 0f20 	cmp.w	sl, #32
3000a6e8:	9308      	str	r3, [sp, #32]
3000a6ea:	f47f ae33 	bne.w	3000a354 <rxi316_perf_tune+0x3f4>
3000a6ee:	f44f 4370 	mov.w	r3, #61440	; 0xf000
3000a6f2:	f1bb 0f20 	cmp.w	fp, #32
3000a6f6:	9307      	str	r3, [sp, #28]
3000a6f8:	f47f ae38 	bne.w	3000a36c <rxi316_perf_tune+0x40c>
3000a6fc:	9b01      	ldr	r3, [sp, #4]
3000a6fe:	f44f 2c70 	mov.w	ip, #983040	; 0xf0000
3000a702:	2b20      	cmp	r3, #32
3000a704:	f47f ae3e 	bne.w	3000a384 <rxi316_perf_tune+0x424>
3000a708:	9b02      	ldr	r3, [sp, #8]
3000a70a:	f44f 0e70 	mov.w	lr, #15728640	; 0xf00000
3000a70e:	2b20      	cmp	r3, #32
3000a710:	f47f ae44 	bne.w	3000a39c <rxi316_perf_tune+0x43c>
3000a714:	9b04      	ldr	r3, [sp, #16]
3000a716:	f04f 6870 	mov.w	r8, #251658240	; 0xf000000
3000a71a:	2b20      	cmp	r3, #32
3000a71c:	f47f ae4a 	bne.w	3000a3b4 <rxi316_perf_tune+0x454>
3000a720:	f04f 4370 	mov.w	r3, #4026531840	; 0xf0000000
3000a724:	9302      	str	r3, [sp, #8]
3000a726:	9b05      	ldr	r3, [sp, #20]
3000a728:	2b20      	cmp	r3, #32
3000a72a:	f47f ae4b 	bne.w	3000a3c4 <rxi316_perf_tune+0x464>
3000a72e:	9b06      	ldr	r3, [sp, #24]
3000a730:	f04f 090f 	mov.w	r9, #15
3000a734:	2b20      	cmp	r3, #32
3000a736:	f47f ae4f 	bne.w	3000a3d8 <rxi316_perf_tune+0x478>
3000a73a:	9b1c      	ldr	r3, [sp, #112]	; 0x70
3000a73c:	20f0      	movs	r0, #240	; 0xf0
3000a73e:	2b20      	cmp	r3, #32
3000a740:	f47f ae53 	bne.w	3000a3ea <rxi316_perf_tune+0x48a>
3000a744:	9b09      	ldr	r3, [sp, #36]	; 0x24
3000a746:	f04f 0b1f 	mov.w	fp, #31
3000a74a:	2b20      	cmp	r3, #32
3000a74c:	f47f ae57 	bne.w	3000a3fe <rxi316_perf_tune+0x49e>
3000a750:	9b1d      	ldr	r3, [sp, #116]	; 0x74
3000a752:	f04f 0a1f 	mov.w	sl, #31
3000a756:	2b20      	cmp	r3, #32
3000a758:	f47f ae5b 	bne.w	3000a412 <rxi316_perf_tune+0x4b2>
3000a75c:	2c20      	cmp	r4, #32
3000a75e:	f04f 071f 	mov.w	r7, #31
3000a762:	f47f ae65 	bne.w	3000a430 <rxi316_perf_tune+0x4d0>
3000a766:	f44f 7378 	mov.w	r3, #992	; 0x3e0
3000a76a:	9301      	str	r3, [sp, #4]
3000a76c:	e666      	b.n	3000a43c <rxi316_perf_tune+0x4dc>
3000a76e:	f44f 6370 	mov.w	r3, #3840	; 0xf00
3000a772:	930c      	str	r3, [sp, #48]	; 0x30
3000a774:	e527      	b.n	3000a1c6 <rxi316_perf_tune+0x266>
3000a776:	22f0      	movs	r2, #240	; 0xf0
3000a778:	e51b      	b.n	3000a1b2 <rxi316_perf_tune+0x252>
3000a77a:	230f      	movs	r3, #15
3000a77c:	9303      	str	r3, [sp, #12]
3000a77e:	e510      	b.n	3000a1a2 <rxi316_perf_tune+0x242>
3000a780:	68f2      	ldr	r2, [r6, #12]
3000a782:	2302      	movs	r3, #2
3000a784:	6891      	ldr	r1, [r2, #8]
3000a786:	e6e2      	b.n	3000a54e <rxi316_perf_tune+0x5ee>
3000a788:	3101      	adds	r1, #1
3000a78a:	9305      	str	r3, [sp, #20]
3000a78c:	e461      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a78e:	bf00      	nop
3000a790:	41100000 	.word	0x41100000
3000a794:	40000008 	.word	0x40000008
3000a798:	3101      	adds	r1, #1
3000a79a:	9306      	str	r3, [sp, #24]
3000a79c:	e459      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a79e:	2202      	movs	r2, #2
3000a7a0:	921e      	str	r2, [sp, #120]	; 0x78
3000a7a2:	e4e2      	b.n	3000a16a <rxi316_perf_tune+0x20a>
3000a7a4:	9a03      	ldr	r2, [sp, #12]
3000a7a6:	9308      	str	r3, [sp, #32]
3000a7a8:	3201      	adds	r2, #1
3000a7aa:	9203      	str	r2, [sp, #12]
3000a7ac:	e451      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7ae:	9a03      	ldr	r2, [sp, #12]
3000a7b0:	9307      	str	r3, [sp, #28]
3000a7b2:	3201      	adds	r2, #1
3000a7b4:	9203      	str	r2, [sp, #12]
3000a7b6:	e44c      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7b8:	9a03      	ldr	r2, [sp, #12]
3000a7ba:	931b      	str	r3, [sp, #108]	; 0x6c
3000a7bc:	3201      	adds	r2, #1
3000a7be:	9203      	str	r2, [sp, #12]
3000a7c0:	e447      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7c2:	9a03      	ldr	r2, [sp, #12]
3000a7c4:	461c      	mov	r4, r3
3000a7c6:	3201      	adds	r2, #1
3000a7c8:	9203      	str	r2, [sp, #12]
3000a7ca:	e442      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7cc:	9a03      	ldr	r2, [sp, #12]
3000a7ce:	931d      	str	r3, [sp, #116]	; 0x74
3000a7d0:	3201      	adds	r2, #1
3000a7d2:	9203      	str	r2, [sp, #12]
3000a7d4:	e43d      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7d6:	9a03      	ldr	r2, [sp, #12]
3000a7d8:	931c      	str	r3, [sp, #112]	; 0x70
3000a7da:	3201      	adds	r2, #1
3000a7dc:	9203      	str	r2, [sp, #12]
3000a7de:	e438      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7e0:	930e      	str	r3, [sp, #56]	; 0x38
3000a7e2:	e436      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7e4:	930d      	str	r3, [sp, #52]	; 0x34
3000a7e6:	e434      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7e8:	930c      	str	r3, [sp, #48]	; 0x30
3000a7ea:	e432      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7ec:	930a      	str	r3, [sp, #40]	; 0x28
3000a7ee:	e430      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7f0:	930b      	str	r3, [sp, #44]	; 0x2c
3000a7f2:	e42e      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7f4:	931a      	str	r3, [sp, #104]	; 0x68
3000a7f6:	e42c      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7f8:	9319      	str	r3, [sp, #100]	; 0x64
3000a7fa:	e42a      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a7fc:	9310      	str	r3, [sp, #64]	; 0x40
3000a7fe:	e428      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a800:	930f      	str	r3, [sp, #60]	; 0x3c
3000a802:	e426      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a804:	9318      	str	r3, [sp, #96]	; 0x60
3000a806:	e424      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a808:	9317      	str	r3, [sp, #92]	; 0x5c
3000a80a:	e422      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a80c:	9316      	str	r3, [sp, #88]	; 0x58
3000a80e:	e420      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a810:	9315      	str	r3, [sp, #84]	; 0x54
3000a812:	e41e      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a814:	9314      	str	r3, [sp, #80]	; 0x50
3000a816:	e41c      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a818:	9313      	str	r3, [sp, #76]	; 0x4c
3000a81a:	e41a      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a81c:	9312      	str	r3, [sp, #72]	; 0x48
3000a81e:	e418      	b.n	3000a052 <rxi316_perf_tune+0xf2>
3000a820:	9311      	str	r3, [sp, #68]	; 0x44
3000a822:	e416      	b.n	3000a052 <rxi316_perf_tune+0xf2>

3000a824 <rxi316_dram_init>:
3000a824:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000a828:	4604      	mov	r4, r0
3000a82a:	b08f      	sub	sp, #60	; 0x3c
3000a82c:	4b10      	ldr	r3, [pc, #64]	; (3000a870 <rxi316_dram_init+0x4c>)
3000a82e:	4798      	blx	r3
3000a830:	f894 b014 	ldrb.w	fp, [r4, #20]
3000a834:	2101      	movs	r1, #1
3000a836:	900c      	str	r0, [sp, #48]	; 0x30
3000a838:	fa01 f30b 	lsl.w	r3, r1, fp
3000a83c:	9302      	str	r3, [sp, #8]
3000a83e:	6823      	ldr	r3, [r4, #0]
3000a840:	78da      	ldrb	r2, [r3, #3]
3000a842:	6923      	ldr	r3, [r4, #16]
3000a844:	fa03 f30b 	lsl.w	r3, r3, fp
3000a848:	b1a2      	cbz	r2, 3000a874 <rxi316_dram_init+0x50>
3000a84a:	428a      	cmp	r2, r1
3000a84c:	f000 8469 	beq.w	3000b122 <rxi316_dram_init+0x8fe>
3000a850:	2a02      	cmp	r2, #2
3000a852:	bf0b      	itete	eq
3000a854:	2206      	moveq	r2, #6
3000a856:	220a      	movne	r2, #10
3000a858:	2540      	moveq	r5, #64	; 0x40
3000a85a:	2580      	movne	r5, #128	; 0x80
3000a85c:	9207      	str	r2, [sp, #28]
3000a85e:	bf0c      	ite	eq
3000a860:	2204      	moveq	r2, #4
3000a862:	2208      	movne	r2, #8
3000a864:	9208      	str	r2, [sp, #32]
3000a866:	bf0c      	ite	eq
3000a868:	2208      	moveq	r2, #8
3000a86a:	2210      	movne	r2, #16
3000a86c:	9201      	str	r2, [sp, #4]
3000a86e:	e007      	b.n	3000a880 <rxi316_dram_init+0x5c>
3000a870:	30007f55 	.word	0x30007f55
3000a874:	2203      	movs	r2, #3
3000a876:	25c0      	movs	r5, #192	; 0xc0
3000a878:	9108      	str	r1, [sp, #32]
3000a87a:	9207      	str	r2, [sp, #28]
3000a87c:	2202      	movs	r2, #2
3000a87e:	9201      	str	r2, [sp, #4]
3000a880:	6860      	ldr	r0, [r4, #4]
3000a882:	f04f 31ff 	mov.w	r1, #4294967295	; 0xffffffff
3000a886:	4abf      	ldr	r2, [pc, #764]	; (3000ab84 <rxi316_dram_init+0x360>)
3000a888:	6806      	ldr	r6, [r0, #0]
3000a88a:	960d      	str	r6, [sp, #52]	; 0x34
3000a88c:	6846      	ldr	r6, [r0, #4]
3000a88e:	6880      	ldr	r0, [r0, #8]
3000a890:	9609      	str	r6, [sp, #36]	; 0x24
3000a892:	9003      	str	r0, [sp, #12]
3000a894:	f8c2 1230 	str.w	r1, [r2, #560]	; 0x230
3000a898:	6820      	ldr	r0, [r4, #0]
3000a89a:	7981      	ldrb	r1, [r0, #6]
3000a89c:	6882      	ldr	r2, [r0, #8]
3000a89e:	b129      	cbz	r1, 3000a8ac <rxi316_dram_init+0x88>
3000a8a0:	2901      	cmp	r1, #1
3000a8a2:	f000 8436 	beq.w	3000b112 <rxi316_dram_init+0x8ee>
3000a8a6:	2903      	cmp	r1, #3
3000a8a8:	f000 8443 	beq.w	3000b132 <rxi316_dram_init+0x90e>
3000a8ac:	3a06      	subs	r2, #6
3000a8ae:	7881      	ldrb	r1, [r0, #2]
3000a8b0:	f002 020f 	and.w	r2, r2, #15
3000a8b4:	7940      	ldrb	r0, [r0, #5]
3000a8b6:	0109      	lsls	r1, r1, #4
3000a8b8:	0280      	lsls	r0, r0, #10
3000a8ba:	f001 0130 	and.w	r1, r1, #48	; 0x30
3000a8be:	f400 6040 	and.w	r0, r0, #3072	; 0xc00
3000a8c2:	4301      	orrs	r1, r0
3000a8c4:	430d      	orrs	r5, r1
3000a8c6:	432a      	orrs	r2, r5
3000a8c8:	4dae      	ldr	r5, [pc, #696]	; (3000ab84 <rxi316_dram_init+0x360>)
3000a8ca:	f8c5 2224 	str.w	r2, [r5, #548]	; 0x224
3000a8ce:	68e2      	ldr	r2, [r4, #12]
3000a8d0:	6812      	ldr	r2, [r2, #0]
3000a8d2:	f892 206c 	ldrb.w	r2, [r2, #108]	; 0x6c
3000a8d6:	f002 0207 	and.w	r2, r2, #7
3000a8da:	f8c5 2228 	str.w	r2, [r5, #552]	; 0x228
3000a8de:	68e2      	ldr	r2, [r4, #12]
3000a8e0:	6811      	ldr	r1, [r2, #0]
3000a8e2:	6822      	ldr	r2, [r4, #0]
3000a8e4:	6848      	ldr	r0, [r1, #4]
3000a8e6:	f892 9000 	ldrb.w	r9, [r2]
3000a8ea:	688a      	ldr	r2, [r1, #8]
3000a8ec:	9004      	str	r0, [sp, #16]
3000a8ee:	0300      	lsls	r0, r0, #12
3000a8f0:	0412      	lsls	r2, r2, #16
3000a8f2:	f009 070f 	and.w	r7, r9, #15
3000a8f6:	f400 5080 	and.w	r0, r0, #4096	; 0x1000
3000a8fa:	f402 3280 	and.w	r2, r2, #65536	; 0x10000
3000a8fe:	4302      	orrs	r2, r0
3000a900:	68c8      	ldr	r0, [r1, #12]
3000a902:	0440      	lsls	r0, r0, #17
3000a904:	433a      	orrs	r2, r7
3000a906:	f400 3600 	and.w	r6, r0, #131072	; 0x20000
3000a90a:	6908      	ldr	r0, [r1, #16]
3000a90c:	0480      	lsls	r0, r0, #18
3000a90e:	4332      	orrs	r2, r6
3000a910:	f400 2080 	and.w	r0, r0, #262144	; 0x40000
3000a914:	4310      	orrs	r0, r2
3000a916:	694a      	ldr	r2, [r1, #20]
3000a918:	04d2      	lsls	r2, r2, #19
3000a91a:	f402 2200 	and.w	r2, r2, #524288	; 0x80000
3000a91e:	4310      	orrs	r0, r2
3000a920:	698a      	ldr	r2, [r1, #24]
3000a922:	0552      	lsls	r2, r2, #21
3000a924:	f402 1200 	and.w	r2, r2, #2097152	; 0x200000
3000a928:	4310      	orrs	r0, r2
3000a92a:	69ca      	ldr	r2, [r1, #28]
3000a92c:	0592      	lsls	r2, r2, #22
3000a92e:	f402 0280 	and.w	r2, r2, #4194304	; 0x400000
3000a932:	4310      	orrs	r0, r2
3000a934:	6a8a      	ldr	r2, [r1, #40]	; 0x28
3000a936:	0612      	lsls	r2, r2, #24
3000a938:	f002 7240 	and.w	r2, r2, #50331648	; 0x3000000
3000a93c:	4302      	orrs	r2, r0
3000a93e:	e9d1 1008 	ldrd	r1, r0, [r1, #32]
3000a942:	06c0      	lsls	r0, r0, #27
3000a944:	0789      	lsls	r1, r1, #30
3000a946:	f000 6000 	and.w	r0, r0, #134217728	; 0x8000000
3000a94a:	f001 4180 	and.w	r1, r1, #1073741824	; 0x40000000
3000a94e:	4310      	orrs	r0, r2
3000a950:	9a02      	ldr	r2, [sp, #8]
3000a952:	0212      	lsls	r2, r2, #8
3000a954:	4308      	orrs	r0, r1
3000a956:	f402 62e0 	and.w	r2, r2, #1792	; 0x700
3000a95a:	4310      	orrs	r0, r2
3000a95c:	6068      	str	r0, [r5, #4]
3000a95e:	6822      	ldr	r2, [r4, #0]
3000a960:	7992      	ldrb	r2, [r2, #6]
3000a962:	2a00      	cmp	r2, #0
3000a964:	f040 8321 	bne.w	3000afaa <rxi316_dram_init+0x786>
3000a968:	686a      	ldr	r2, [r5, #4]
3000a96a:	f042 0210 	orr.w	r2, r2, #16
3000a96e:	606a      	str	r2, [r5, #4]
3000a970:	68e2      	ldr	r2, [r4, #12]
3000a972:	9802      	ldr	r0, [sp, #8]
3000a974:	6812      	ldr	r2, [r2, #0]
3000a976:	f100 3aff 	add.w	sl, r0, #4294967295	; 0xffffffff
3000a97a:	6861      	ldr	r1, [r4, #4]
3000a97c:	6ad0      	ldr	r0, [r2, #44]	; 0x2c
3000a97e:	f8d1 e010 	ldr.w	lr, [r1, #16]
3000a982:	9005      	str	r0, [sp, #20]
3000a984:	6b10      	ldr	r0, [r2, #48]	; 0x30
3000a986:	fa2e f80b 	lsr.w	r8, lr, fp
3000a98a:	6949      	ldr	r1, [r1, #20]
3000a98c:	ea1e 0f0a 	tst.w	lr, sl
3000a990:	9006      	str	r0, [sp, #24]
3000a992:	6c55      	ldr	r5, [r2, #68]	; 0x44
3000a994:	fa21 fe0b 	lsr.w	lr, r1, fp
3000a998:	bf18      	it	ne
3000a99a:	f108 0801 	addne.w	r8, r8, #1
3000a99e:	ea11 0f0a 	tst.w	r1, sl
3000a9a2:	9906      	ldr	r1, [sp, #24]
3000a9a4:	950a      	str	r5, [sp, #40]	; 0x28
3000a9a6:	bf18      	it	ne
3000a9a8:	f10e 0e01 	addne.w	lr, lr, #1
3000a9ac:	6c95      	ldr	r5, [r2, #72]	; 0x48
3000a9ae:	0049      	lsls	r1, r1, #1
3000a9b0:	ea4f 3808 	mov.w	r8, r8, lsl #12
3000a9b4:	950b      	str	r5, [sp, #44]	; 0x2c
3000a9b6:	f001 0102 	and.w	r1, r1, #2
3000a9ba:	ea4f 5e0e 	mov.w	lr, lr, lsl #20
3000a9be:	f408 38f8 	and.w	r8, r8, #126976	; 0x1f000
3000a9c2:	e9d2 0c0d 	ldrd	r0, ip, [r2, #52]	; 0x34
3000a9c6:	e9d2 760f 	ldrd	r7, r6, [r2, #60]	; 0x3c
3000a9ca:	0080      	lsls	r0, r0, #2
3000a9cc:	f00e 7ef8 	and.w	lr, lr, #32505856	; 0x1f00000
3000a9d0:	ea4f 0ccc 	mov.w	ip, ip, lsl #3
3000a9d4:	9006      	str	r0, [sp, #24]
3000a9d6:	013f      	lsls	r7, r7, #4
3000a9d8:	9805      	ldr	r0, [sp, #20]
3000a9da:	f00c 0c08 	and.w	ip, ip, #8
3000a9de:	0176      	lsls	r6, r6, #5
3000a9e0:	f007 0710 	and.w	r7, r7, #16
3000a9e4:	f000 0001 	and.w	r0, r0, #1
3000a9e8:	f006 0620 	and.w	r6, r6, #32
3000a9ec:	4605      	mov	r5, r0
3000a9ee:	9806      	ldr	r0, [sp, #24]
3000a9f0:	f000 0004 	and.w	r0, r0, #4
3000a9f4:	4301      	orrs	r1, r0
3000a9f6:	4329      	orrs	r1, r5
3000a9f8:	ea41 0c0c 	orr.w	ip, r1, ip
3000a9fc:	990a      	ldr	r1, [sp, #40]	; 0x28
3000a9fe:	ea4c 0707 	orr.w	r7, ip, r7
3000aa02:	018d      	lsls	r5, r1, #6
3000aa04:	990b      	ldr	r1, [sp, #44]	; 0x2c
3000aa06:	4337      	orrs	r7, r6
3000aa08:	f005 0540 	and.w	r5, r5, #64	; 0x40
3000aa0c:	0208      	lsls	r0, r1, #8
3000aa0e:	6cd1      	ldr	r1, [r2, #76]	; 0x4c
3000aa10:	432f      	orrs	r7, r5
3000aa12:	f400 6070 	and.w	r0, r0, #3840	; 0xf00
3000aa16:	06c9      	lsls	r1, r1, #27
3000aa18:	e9d2 6214 	ldrd	r6, r2, [r2, #80]	; 0x50
3000aa1c:	4307      	orrs	r7, r0
3000aa1e:	f001 6100 	and.w	r1, r1, #134217728	; 0x8000000
3000aa22:	0736      	lsls	r6, r6, #28
3000aa24:	430f      	orrs	r7, r1
3000aa26:	0752      	lsls	r2, r2, #29
3000aa28:	f006 5680 	and.w	r6, r6, #268435456	; 0x10000000
3000aa2c:	f002 5200 	and.w	r2, r2, #536870912	; 0x20000000
3000aa30:	433e      	orrs	r6, r7
3000aa32:	4316      	orrs	r6, r2
3000aa34:	4a53      	ldr	r2, [pc, #332]	; (3000ab84 <rxi316_dram_init+0x360>)
3000aa36:	ea46 0608 	orr.w	r6, r6, r8
3000aa3a:	ea46 060e 	orr.w	r6, r6, lr
3000aa3e:	6096      	str	r6, [r2, #8]
3000aa40:	6862      	ldr	r2, [r4, #4]
3000aa42:	6990      	ldr	r0, [r2, #24]
3000aa44:	9a01      	ldr	r2, [sp, #4]
3000aa46:	2a10      	cmp	r2, #16
3000aa48:	6822      	ldr	r2, [r4, #0]
3000aa4a:	7811      	ldrb	r1, [r2, #0]
3000aa4c:	7916      	ldrb	r6, [r2, #4]
3000aa4e:	f000 8321 	beq.w	3000b094 <rxi316_dram_init+0x870>
3000aa52:	f1a1 0502 	sub.w	r5, r1, #2
3000aa56:	f1a1 0e09 	sub.w	lr, r1, #9
3000aa5a:	9f01      	ldr	r7, [sp, #4]
3000aa5c:	fab5 f585 	clz	r5, r5
3000aa60:	fabe fe8e 	clz	lr, lr
3000aa64:	2f08      	cmp	r7, #8
3000aa66:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000aa6a:	ea4f 1e5e 	mov.w	lr, lr, lsr #5
3000aa6e:	ea45 020e 	orr.w	r2, r5, lr
3000aa72:	f000 82a8 	beq.w	3000afc6 <rxi316_dram_init+0x7a2>
3000aa76:	9f01      	ldr	r7, [sp, #4]
3000aa78:	2f04      	cmp	r7, #4
3000aa7a:	f000 837c 	beq.w	3000b176 <rxi316_dram_init+0x952>
3000aa7e:	9f01      	ldr	r7, [sp, #4]
3000aa80:	2f02      	cmp	r7, #2
3000aa82:	bf08      	it	eq
3000aa84:	4310      	orreq	r0, r2
3000aa86:	2a00      	cmp	r2, #0
3000aa88:	f040 82a2 	bne.w	3000afd0 <rxi316_dram_init+0x7ac>
3000aa8c:	2903      	cmp	r1, #3
3000aa8e:	f000 8355 	beq.w	3000b13c <rxi316_dram_init+0x918>
3000aa92:	2500      	movs	r5, #0
3000aa94:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000aa98:	462f      	mov	r7, r5
3000aa9a:	433d      	orrs	r5, r7
3000aa9c:	4f39      	ldr	r7, [pc, #228]	; (3000ab84 <rxi316_dram_init+0x360>)
3000aa9e:	430d      	orrs	r5, r1
3000aaa0:	4305      	orrs	r5, r0
3000aaa2:	ea45 06c6 	orr.w	r6, r5, r6, lsl #3
3000aaa6:	637e      	str	r6, [r7, #52]	; 0x34
3000aaa8:	6862      	ldr	r2, [r4, #4]
3000aaaa:	9903      	ldr	r1, [sp, #12]
3000aaac:	69d2      	ldr	r2, [r2, #28]
3000aaae:	ea42 02c1 	orr.w	r2, r2, r1, lsl #3
3000aab2:	63ba      	str	r2, [r7, #56]	; 0x38
3000aab4:	6822      	ldr	r2, [r4, #0]
3000aab6:	7810      	ldrb	r0, [r2, #0]
3000aab8:	6862      	ldr	r2, [r4, #4]
3000aaba:	2803      	cmp	r0, #3
3000aabc:	6a11      	ldr	r1, [r2, #32]
3000aabe:	f000 82dc 	beq.w	3000b07a <rxi316_dram_init+0x856>
3000aac2:	2809      	cmp	r0, #9
3000aac4:	f000 82fa 	beq.w	3000b0bc <rxi316_dram_init+0x898>
3000aac8:	482e      	ldr	r0, [pc, #184]	; (3000ab84 <rxi316_dram_init+0x360>)
3000aaca:	63c1      	str	r1, [r0, #60]	; 0x3c
3000aacc:	6862      	ldr	r2, [r4, #4]
3000aace:	6a52      	ldr	r2, [r2, #36]	; 0x24
3000aad0:	6402      	str	r2, [r0, #64]	; 0x40
3000aad2:	6860      	ldr	r0, [r4, #4]
3000aad4:	68c5      	ldr	r5, [r0, #12]
3000aad6:	6802      	ldr	r2, [r0, #0]
3000aad8:	6841      	ldr	r1, [r0, #4]
3000aada:	442a      	add	r2, r5
3000aadc:	6880      	ldr	r0, [r0, #8]
3000aade:	4429      	add	r1, r5
3000aae0:	ea1a 0f02 	tst.w	sl, r2
3000aae4:	fa22 f20b 	lsr.w	r2, r2, fp
3000aae8:	fa21 f60b 	lsr.w	r6, r1, fp
3000aaec:	fa20 f50b 	lsr.w	r5, r0, fp
3000aaf0:	bf18      	it	ne
3000aaf2:	3201      	addne	r2, #1
3000aaf4:	ea1a 0f01 	tst.w	sl, r1
3000aaf8:	6821      	ldr	r1, [r4, #0]
3000aafa:	bf18      	it	ne
3000aafc:	3601      	addne	r6, #1
3000aafe:	ea1a 0f00 	tst.w	sl, r0
3000ab02:	7808      	ldrb	r0, [r1, #0]
3000ab04:	bf18      	it	ne
3000ab06:	3501      	addne	r5, #1
3000ab08:	68a1      	ldr	r1, [r4, #8]
3000ab0a:	3802      	subs	r0, #2
3000ab0c:	6dcf      	ldr	r7, [r1, #92]	; 0x5c
3000ab0e:	2801      	cmp	r0, #1
3000ab10:	f240 82ac 	bls.w	3000b06c <rxi316_dram_init+0x848>
3000ab14:	698f      	ldr	r7, [r1, #24]
3000ab16:	fbb7 f0f3 	udiv	r0, r7, r3
3000ab1a:	fb03 7710 	mls	r7, r3, r0, r7
3000ab1e:	b107      	cbz	r7, 3000ab22 <rxi316_dram_init+0x2fe>
3000ab20:	3001      	adds	r0, #1
3000ab22:	6a4f      	ldr	r7, [r1, #36]	; 0x24
3000ab24:	fbb7 f1f3 	udiv	r1, r7, r3
3000ab28:	fb03 7711 	mls	r7, r3, r1, r7
3000ab2c:	b107      	cbz	r7, 3000ab30 <rxi316_dram_init+0x30c>
3000ab2e:	3101      	adds	r1, #1
3000ab30:	0176      	lsls	r6, r6, #5
3000ab32:	f002 021f 	and.w	r2, r2, #31
3000ab36:	02ad      	lsls	r5, r5, #10
3000ab38:	f406 7678 	and.w	r6, r6, #992	; 0x3e0
3000ab3c:	0500      	lsls	r0, r0, #20
3000ab3e:	f405 45f8 	and.w	r5, r5, #31744	; 0x7c00
3000ab42:	4316      	orrs	r6, r2
3000ab44:	f000 70f8 	and.w	r0, r0, #32505856	; 0x1f00000
3000ab48:	4a0e      	ldr	r2, [pc, #56]	; (3000ab84 <rxi316_dram_init+0x360>)
3000ab4a:	432e      	orrs	r6, r5
3000ab4c:	4306      	orrs	r6, r0
3000ab4e:	0648      	lsls	r0, r1, #25
3000ab50:	f000 5078 	and.w	r0, r0, #1040187392	; 0x3e000000
3000ab54:	4330      	orrs	r0, r6
3000ab56:	6310      	str	r0, [r2, #48]	; 0x30
3000ab58:	68a0      	ldr	r0, [r4, #8]
3000ab5a:	6802      	ldr	r2, [r0, #0]
3000ab5c:	fbb2 f1f3 	udiv	r1, r2, r3
3000ab60:	fb03 2211 	mls	r2, r3, r1, r2
3000ab64:	2a00      	cmp	r2, #0
3000ab66:	f000 827f 	beq.w	3000b068 <rxi316_dram_init+0x844>
3000ab6a:	1c4a      	adds	r2, r1, #1
3000ab6c:	9205      	str	r2, [sp, #20]
3000ab6e:	6842      	ldr	r2, [r0, #4]
3000ab70:	fbb2 f0f3 	udiv	r0, r2, r3
3000ab74:	fb03 2210 	mls	r2, r3, r0, r2
3000ab78:	2a00      	cmp	r2, #0
3000ab7a:	f000 8273 	beq.w	3000b064 <rxi316_dram_init+0x840>
3000ab7e:	38c7      	subs	r0, #199	; 0xc7
3000ab80:	e002      	b.n	3000ab88 <rxi316_dram_init+0x364>
3000ab82:	bf00      	nop
3000ab84:	41100000 	.word	0x41100000
3000ab88:	68e2      	ldr	r2, [r4, #12]
3000ab8a:	6811      	ldr	r1, [r2, #0]
3000ab8c:	4ac3      	ldr	r2, [pc, #780]	; (3000ae9c <rxi316_dram_init+0x678>)
3000ab8e:	ea02 2000 	and.w	r0, r2, r0, lsl #8
3000ab92:	e9d1 2117 	ldrd	r2, r1, [r1, #92]	; 0x5c
3000ab96:	900a      	str	r0, [sp, #40]	; 0x28
3000ab98:	0712      	lsls	r2, r2, #28
3000ab9a:	ea40 7141 	orr.w	r1, r0, r1, lsl #29
3000ab9e:	f002 5280 	and.w	r2, r2, #268435456	; 0x10000000
3000aba2:	430a      	orrs	r2, r1
3000aba4:	f89d 1014 	ldrb.w	r1, [sp, #20]
3000aba8:	430a      	orrs	r2, r1
3000abaa:	49bd      	ldr	r1, [pc, #756]	; (3000aea0 <rxi316_dram_init+0x67c>)
3000abac:	f042 7280 	orr.w	r2, r2, #16777216	; 0x1000000
3000abb0:	610a      	str	r2, [r1, #16]
3000abb2:	68a5      	ldr	r5, [r4, #8]
3000abb4:	692a      	ldr	r2, [r5, #16]
3000abb6:	fbb2 fcf3 	udiv	ip, r2, r3
3000abba:	fb03 221c 	mls	r2, r3, ip, r2
3000abbe:	2a00      	cmp	r2, #0
3000abc0:	f000 824d 	beq.w	3000b05e <rxi316_dram_init+0x83a>
3000abc4:	f10c 0201 	add.w	r2, ip, #1
3000abc8:	9206      	str	r2, [sp, #24]
3000abca:	69aa      	ldr	r2, [r5, #24]
3000abcc:	fbb2 f0f3 	udiv	r0, r2, r3
3000abd0:	fb03 2210 	mls	r2, r3, r0, r2
3000abd4:	b102      	cbz	r2, 3000abd8 <rxi316_dram_init+0x3b4>
3000abd6:	3001      	adds	r0, #1
3000abd8:	68ea      	ldr	r2, [r5, #12]
3000abda:	fbb2 f1f3 	udiv	r1, r2, r3
3000abde:	fb03 2211 	mls	r2, r3, r1, r2
3000abe2:	9103      	str	r1, [sp, #12]
3000abe4:	b10a      	cbz	r2, 3000abea <rxi316_dram_init+0x3c6>
3000abe6:	3101      	adds	r1, #1
3000abe8:	9103      	str	r1, [sp, #12]
3000abea:	6a6a      	ldr	r2, [r5, #36]	; 0x24
3000abec:	fbb2 f1f3 	udiv	r1, r2, r3
3000abf0:	fb03 2211 	mls	r2, r3, r1, r2
3000abf4:	b102      	cbz	r2, 3000abf8 <rxi316_dram_init+0x3d4>
3000abf6:	3101      	adds	r1, #1
3000abf8:	6822      	ldr	r2, [r4, #0]
3000abfa:	7812      	ldrb	r2, [r2, #0]
3000abfc:	2a09      	cmp	r2, #9
3000abfe:	f000 8259 	beq.w	3000b0b4 <rxi316_dram_init+0x890>
3000ac02:	2a02      	cmp	r2, #2
3000ac04:	d104      	bne.n	3000ac10 <rxi316_dram_init+0x3ec>
3000ac06:	9a08      	ldr	r2, [sp, #32]
3000ac08:	3a02      	subs	r2, #2
3000ac0a:	fa22 f20b 	lsr.w	r2, r2, fp
3000ac0e:	4411      	add	r1, r2
3000ac10:	68e2      	ldr	r2, [r4, #12]
3000ac12:	0349      	lsls	r1, r1, #13
3000ac14:	6d2d      	ldr	r5, [r5, #80]	; 0x50
3000ac16:	6812      	ldr	r2, [r2, #0]
3000ac18:	f401 31f0 	and.w	r1, r1, #122880	; 0x1e000
3000ac1c:	fa25 f60b 	lsr.w	r6, r5, fp
3000ac20:	ea15 0f0a 	tst.w	r5, sl
3000ac24:	6e52      	ldr	r2, [r2, #100]	; 0x64
3000ac26:	bf18      	it	ne
3000ac28:	3601      	addne	r6, #1
3000ac2a:	05d2      	lsls	r2, r2, #23
3000ac2c:	f402 0200 	and.w	r2, r2, #8388608	; 0x800000
3000ac30:	ea42 6606 	orr.w	r6, r2, r6, lsl #24
3000ac34:	9a06      	ldr	r2, [sp, #24]
3000ac36:	0112      	lsls	r2, r2, #4
3000ac38:	f402 72f8 	and.w	r2, r2, #496	; 0x1f0
3000ac3c:	4332      	orrs	r2, r6
3000ac3e:	0246      	lsls	r6, r0, #9
3000ac40:	f406 56f0 	and.w	r6, r6, #7680	; 0x1e00
3000ac44:	4316      	orrs	r6, r2
3000ac46:	9a03      	ldr	r2, [sp, #12]
3000ac48:	f002 020f 	and.w	r2, r2, #15
3000ac4c:	4316      	orrs	r6, r2
3000ac4e:	4a94      	ldr	r2, [pc, #592]	; (3000aea0 <rxi316_dram_init+0x67c>)
3000ac50:	4331      	orrs	r1, r6
3000ac52:	f441 11a0 	orr.w	r1, r1, #1310720	; 0x140000
3000ac56:	6151      	str	r1, [r2, #20]
3000ac58:	68a7      	ldr	r7, [r4, #8]
3000ac5a:	697a      	ldr	r2, [r7, #20]
3000ac5c:	fbb2 fef3 	udiv	lr, r2, r3
3000ac60:	fb03 221e 	mls	r2, r3, lr, r2
3000ac64:	b10a      	cbz	r2, 3000ac6a <rxi316_dram_init+0x446>
3000ac66:	f10e 0e01 	add.w	lr, lr, #1
3000ac6a:	6939      	ldr	r1, [r7, #16]
3000ac6c:	fbb1 f2f3 	udiv	r2, r1, r3
3000ac70:	fb03 1112 	mls	r1, r3, r2, r1
3000ac74:	b101      	cbz	r1, 3000ac78 <rxi316_dram_init+0x454>
3000ac76:	3201      	adds	r2, #1
3000ac78:	68fd      	ldr	r5, [r7, #12]
3000ac7a:	fbb5 f1f3 	udiv	r1, r5, r3
3000ac7e:	fb03 5511 	mls	r5, r3, r1, r5
3000ac82:	b105      	cbz	r5, 3000ac86 <rxi316_dram_init+0x462>
3000ac84:	3101      	adds	r1, #1
3000ac86:	6b3d      	ldr	r5, [r7, #48]	; 0x30
3000ac88:	fbb5 f6f3 	udiv	r6, r5, r3
3000ac8c:	fb03 5516 	mls	r5, r3, r6, r5
3000ac90:	b105      	cbz	r5, 3000ac94 <rxi316_dram_init+0x470>
3000ac92:	3601      	adds	r6, #1
3000ac94:	68bd      	ldr	r5, [r7, #8]
3000ac96:	4411      	add	r1, r2
3000ac98:	fbb5 f2f3 	udiv	r2, r5, r3
3000ac9c:	42b1      	cmp	r1, r6
3000ac9e:	fb03 5512 	mls	r5, r3, r2, r5
3000aca2:	bf38      	it	cc
3000aca4:	4631      	movcc	r1, r6
3000aca6:	b905      	cbnz	r5, 3000acaa <rxi316_dram_init+0x486>
3000aca8:	3a01      	subs	r2, #1
3000acaa:	6afd      	ldr	r5, [r7, #44]	; 0x2c
3000acac:	ea15 0f0a 	tst.w	r5, sl
3000acb0:	fa25 fc0b 	lsr.w	ip, r5, fp
3000acb4:	69fd      	ldr	r5, [r7, #28]
3000acb6:	bf18      	it	ne
3000acb8:	f10c 0c01 	addne.w	ip, ip, #1
3000acbc:	fa25 f60b 	lsr.w	r6, r5, fp
3000acc0:	ea15 0f0a 	tst.w	r5, sl
3000acc4:	bf18      	it	ne
3000acc6:	3601      	addne	r6, #1
3000acc8:	f1b9 0f09 	cmp.w	r9, #9
3000accc:	f000 81ed 	beq.w	3000b0aa <rxi316_dram_init+0x886>
3000acd0:	f1b9 0f02 	cmp.w	r9, #2
3000acd4:	f000 81f4 	beq.w	3000b0c0 <rxi316_dram_init+0x89c>
3000acd8:	f1b9 0f03 	cmp.w	r9, #3
3000acdc:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000acde:	f000 827d 	beq.w	3000b1dc <rxi316_dram_init+0x9b8>
3000ace2:	46a8      	mov	r8, r5
3000ace4:	9d01      	ldr	r5, [sp, #4]
3000ace6:	44a8      	add	r8, r5
3000ace8:	fa28 f50b 	lsr.w	r5, r8, fp
3000acec:	ea18 0f0a 	tst.w	r8, sl
3000acf0:	f8d7 8040 	ldr.w	r8, [r7, #64]	; 0x40
3000acf4:	bf18      	it	ne
3000acf6:	3501      	addne	r5, #1
3000acf8:	fbb8 f7f3 	udiv	r7, r8, r3
3000acfc:	fb03 8817 	mls	r8, r3, r7, r8
3000ad00:	f1b8 0f00 	cmp.w	r8, #0
3000ad04:	d000      	beq.n	3000ad08 <rxi316_dram_init+0x4e4>
3000ad06:	3701      	adds	r7, #1
3000ad08:	0109      	lsls	r1, r1, #4
3000ad0a:	f00e 0e0f 	and.w	lr, lr, #15
3000ad0e:	ea4f 3c8c 	mov.w	ip, ip, lsl #14
3000ad12:	f401 717c 	and.w	r1, r1, #1008	; 0x3f0
3000ad16:	063f      	lsls	r7, r7, #24
3000ad18:	f40c 3ce0 	and.w	ip, ip, #114688	; 0x1c000
3000ad1c:	ea41 0e0e 	orr.w	lr, r1, lr
3000ad20:	0291      	lsls	r1, r2, #10
3000ad22:	f007 57f8 	and.w	r7, r7, #520093696	; 0x1f000000
3000ad26:	f401 5170 	and.w	r1, r1, #15360	; 0x3c00
3000ad2a:	ea4e 0e01 	orr.w	lr, lr, r1
3000ad2e:	0471      	lsls	r1, r6, #17
3000ad30:	ea4e 0c0c 	orr.w	ip, lr, ip
3000ad34:	f401 2160 	and.w	r1, r1, #917504	; 0xe0000
3000ad38:	ea4c 0c01 	orr.w	ip, ip, r1
3000ad3c:	0529      	lsls	r1, r5, #20
3000ad3e:	f401 0170 	and.w	r1, r1, #15728640	; 0xf00000
3000ad42:	ea4c 0c01 	orr.w	ip, ip, r1
3000ad46:	4956      	ldr	r1, [pc, #344]	; (3000aea0 <rxi316_dram_init+0x67c>)
3000ad48:	ea4c 0707 	orr.w	r7, ip, r7
3000ad4c:	618f      	str	r7, [r1, #24]
3000ad4e:	6821      	ldr	r1, [r4, #0]
3000ad50:	780f      	ldrb	r7, [r1, #0]
3000ad52:	2f02      	cmp	r7, #2
3000ad54:	f000 8153 	beq.w	3000affe <rxi316_dram_init+0x7da>
3000ad58:	2f09      	cmp	r7, #9
3000ad5a:	f000 8150 	beq.w	3000affe <rxi316_dram_init+0x7da>
3000ad5e:	68e1      	ldr	r1, [r4, #12]
3000ad60:	2f03      	cmp	r7, #3
3000ad62:	6809      	ldr	r1, [r1, #0]
3000ad64:	6e89      	ldr	r1, [r1, #104]	; 0x68
3000ad66:	f001 080f 	and.w	r8, r1, #15
3000ad6a:	68a1      	ldr	r1, [r4, #8]
3000ad6c:	6a09      	ldr	r1, [r1, #32]
3000ad6e:	d005      	beq.n	3000ad7c <rxi316_dram_init+0x558>
3000ad70:	ea0a 0701 	and.w	r7, sl, r1
3000ad74:	fa21 f10b 	lsr.w	r1, r1, fp
3000ad78:	b107      	cbz	r7, 3000ad7c <rxi316_dram_init+0x558>
3000ad7a:	3101      	adds	r1, #1
3000ad7c:	f1b9 0f02 	cmp.w	r9, #2
3000ad80:	f000 8155 	beq.w	3000b02e <rxi316_dram_init+0x80a>
3000ad84:	f1b9 0f03 	cmp.w	r9, #3
3000ad88:	f000 819d 	beq.w	3000b0c6 <rxi316_dram_init+0x8a2>
3000ad8c:	f1b9 0f09 	cmp.w	r9, #9
3000ad90:	f000 8214 	beq.w	3000b1bc <rxi316_dram_init+0x998>
3000ad94:	f04f 0e00 	mov.w	lr, #0
3000ad98:	46f1      	mov	r9, lr
3000ad9a:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000ad9e:	0109      	lsls	r1, r1, #4
3000ada0:	9f06      	ldr	r7, [sp, #24]
3000ada2:	b2c9      	uxtb	r1, r1
3000ada4:	ea4f 1c57 	mov.w	ip, r7, lsr #5
3000ada8:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000adaa:	ea41 0108 	orr.w	r1, r1, r8
3000adae:	4339      	orrs	r1, r7
3000adb0:	4f3b      	ldr	r7, [pc, #236]	; (3000aea0 <rxi316_dram_init+0x67c>)
3000adb2:	ea41 0109 	orr.w	r1, r1, r9
3000adb6:	ea41 010e 	orr.w	r1, r1, lr
3000adba:	61f9      	str	r1, [r7, #28]
3000adbc:	68a7      	ldr	r7, [r4, #8]
3000adbe:	6c3c      	ldr	r4, [r7, #64]	; 0x40
3000adc0:	fbb4 f1f3 	udiv	r1, r4, r3
3000adc4:	fb03 4311 	mls	r3, r3, r1, r4
3000adc8:	b103      	cbz	r3, 3000adcc <rxi316_dram_init+0x5a8>
3000adca:	3101      	adds	r1, #1
3000adcc:	6b7c      	ldr	r4, [r7, #52]	; 0x34
3000adce:	094b      	lsrs	r3, r1, #5
3000add0:	f8d7 903c 	ldr.w	r9, [r7, #60]	; 0x3c
3000add4:	0912      	lsrs	r2, r2, #4
3000add6:	ea14 0f0a 	tst.w	r4, sl
3000adda:	fa24 f80b 	lsr.w	r8, r4, fp
3000adde:	6bbc      	ldr	r4, [r7, #56]	; 0x38
3000ade0:	ea4f 06d6 	mov.w	r6, r6, lsr #3
3000ade4:	bf18      	it	ne
3000ade6:	f108 0801 	addne.w	r8, r8, #1
3000adea:	9306      	str	r3, [sp, #24]
3000adec:	ea14 0f0a 	tst.w	r4, sl
3000adf0:	fa24 fe0b 	lsr.w	lr, r4, fp
3000adf4:	9b01      	ldr	r3, [sp, #4]
3000adf6:	fa29 f40b 	lsr.w	r4, r9, fp
3000adfa:	bf18      	it	ne
3000adfc:	f10e 0e01 	addne.w	lr, lr, #1
3000ae00:	6aff      	ldr	r7, [r7, #44]	; 0x2c
3000ae02:	ea19 0f0a 	tst.w	r9, sl
3000ae06:	fa23 f90b 	lsr.w	r9, r3, fp
3000ae0a:	9905      	ldr	r1, [sp, #20]
3000ae0c:	ea4f 1515 	mov.w	r5, r5, lsr #4
3000ae10:	bf18      	it	ne
3000ae12:	3401      	addne	r4, #1
3000ae14:	45b9      	cmp	r9, r7
3000ae16:	ea4f 2111 	mov.w	r1, r1, lsr #8
3000ae1a:	ea4f 1010 	mov.w	r0, r0, lsr #4
3000ae1e:	f240 80ec 	bls.w	3000affa <rxi316_dram_init+0x7d6>
3000ae22:	eba9 0707 	sub.w	r7, r9, r7
3000ae26:	fa27 f40b 	lsr.w	r4, r7, fp
3000ae2a:	ea17 070a 	ands.w	r7, r7, sl
3000ae2e:	d001      	beq.n	3000ae34 <rxi316_dram_init+0x610>
3000ae30:	3401      	adds	r4, #1
3000ae32:	2700      	movs	r7, #0
3000ae34:	9b04      	ldr	r3, [sp, #16]
3000ae36:	b10b      	cbz	r3, 3000ae3c <rxi316_dram_init+0x618>
3000ae38:	073b      	lsls	r3, r7, #28
3000ae3a:	9304      	str	r3, [sp, #16]
3000ae3c:	ea4f 0c8c 	mov.w	ip, ip, lsl #2
3000ae40:	9b06      	ldr	r3, [sp, #24]
3000ae42:	0289      	lsls	r1, r1, #10
3000ae44:	02c0      	lsls	r0, r0, #11
3000ae46:	f00c 0c04 	and.w	ip, ip, #4
3000ae4a:	f401 6180 	and.w	r1, r1, #1024	; 0x400
3000ae4e:	01d2      	lsls	r2, r2, #7
3000ae50:	f400 6000 	and.w	r0, r0, #2048	; 0x800
3000ae54:	0236      	lsls	r6, r6, #8
3000ae56:	ea4c 0c01 	orr.w	ip, ip, r1
3000ae5a:	b2d2      	uxtb	r2, r2
3000ae5c:	026d      	lsls	r5, r5, #9
3000ae5e:	f406 7680 	and.w	r6, r6, #256	; 0x100
3000ae62:	ea4c 0c00 	orr.w	ip, ip, r0
3000ae66:	00df      	lsls	r7, r3, #3
3000ae68:	f405 7500 	and.w	r5, r5, #512	; 0x200
3000ae6c:	ea4f 3888 	mov.w	r8, r8, lsl #14
3000ae70:	ea4c 0202 	orr.w	r2, ip, r2
3000ae74:	f007 0708 	and.w	r7, r7, #8
3000ae78:	ea4f 4e4e 	mov.w	lr, lr, lsl #17
3000ae7c:	f408 38e0 	and.w	r8, r8, #114688	; 0x1c000
3000ae80:	4332      	orrs	r2, r6
3000ae82:	0524      	lsls	r4, r4, #20
3000ae84:	f40e 2e60 	and.w	lr, lr, #917504	; 0xe0000
3000ae88:	4905      	ldr	r1, [pc, #20]	; (3000aea0 <rxi316_dram_init+0x67c>)
3000ae8a:	ea42 0305 	orr.w	r3, r2, r5
3000ae8e:	f404 0470 	and.w	r4, r4, #15728640	; 0xf00000
3000ae92:	9a04      	ldr	r2, [sp, #16]
3000ae94:	433b      	orrs	r3, r7
3000ae96:	ea43 0308 	orr.w	r3, r3, r8
3000ae9a:	e003      	b.n	3000aea4 <rxi316_dram_init+0x680>
3000ae9c:	00ffff00 	.word	0x00ffff00
3000aea0:	41100000 	.word	0x41100000
3000aea4:	ea43 030e 	orr.w	r3, r3, lr
3000aea8:	4323      	orrs	r3, r4
3000aeaa:	4313      	orrs	r3, r2
3000aeac:	9a03      	ldr	r2, [sp, #12]
3000aeae:	f3c2 1200 	ubfx	r2, r2, #4, #1
3000aeb2:	620b      	str	r3, [r1, #32]
3000aeb4:	f8c1 2090 	str.w	r2, [r1, #144]	; 0x90
3000aeb8:	9b02      	ldr	r3, [sp, #8]
3000aeba:	2b01      	cmp	r3, #1
3000aebc:	d002      	beq.n	3000aec4 <rxi316_dram_init+0x6a0>
3000aebe:	9b08      	ldr	r3, [sp, #32]
3000aec0:	3303      	adds	r3, #3
3000aec2:	9307      	str	r3, [sp, #28]
3000aec4:	4bb2      	ldr	r3, [pc, #712]	; (3000b190 <rxi316_dram_init+0x96c>)
3000aec6:	f64f 71ff 	movw	r1, #65535	; 0xffff
3000aeca:	9a07      	ldr	r2, [sp, #28]
3000aecc:	f8c3 2094 	str.w	r2, [r3, #148]	; 0x94
3000aed0:	f44f 62e0 	mov.w	r2, #1792	; 0x700
3000aed4:	f8c3 10cc 	str.w	r1, [r3, #204]	; 0xcc
3000aed8:	60da      	str	r2, [r3, #12]
3000aeda:	f8d3 2100 	ldr.w	r2, [r3, #256]	; 0x100
3000aede:	430a      	orrs	r2, r1
3000aee0:	49ac      	ldr	r1, [pc, #688]	; (3000b194 <rxi316_dram_init+0x970>)
3000aee2:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000aee6:	f8d1 2268 	ldr.w	r2, [r1, #616]	; 0x268
3000aeea:	0096      	lsls	r6, r2, #2
3000aeec:	d57f      	bpl.n	3000afee <rxi316_dram_init+0x7ca>
3000aeee:	f04f 4200 	mov.w	r2, #2147483648	; 0x80000000
3000aef2:	4ca7      	ldr	r4, [pc, #668]	; (3000b190 <rxi316_dram_init+0x96c>)
3000aef4:	601a      	str	r2, [r3, #0]
3000aef6:	691a      	ldr	r2, [r3, #16]
3000aef8:	f042 5280 	orr.w	r2, r2, #268435456	; 0x10000000
3000aefc:	611a      	str	r2, [r3, #16]
3000aefe:	f8d1 32fc 	ldr.w	r3, [r1, #764]	; 0x2fc
3000af02:	f043 0303 	orr.w	r3, r3, #3
3000af06:	f8c1 32fc 	str.w	r3, [r1, #764]	; 0x2fc
3000af0a:	f44f 0160 	mov.w	r1, #14680064	; 0xe00000
3000af0e:	9b0c      	ldr	r3, [sp, #48]	; 0x30
3000af10:	2b01      	cmp	r3, #1
3000af12:	4ba1      	ldr	r3, [pc, #644]	; (3000b198 <rxi316_dram_init+0x974>)
3000af14:	bf0c      	ite	eq
3000af16:	4aa1      	ldreq	r2, [pc, #644]	; (3000b19c <rxi316_dram_init+0x978>)
3000af18:	4aa1      	ldrne	r2, [pc, #644]	; (3000b1a0 <rxi316_dram_init+0x97c>)
3000af1a:	601a      	str	r2, [r3, #0]
3000af1c:	2203      	movs	r2, #3
3000af1e:	68e0      	ldr	r0, [r4, #12]
3000af20:	4ba0      	ldr	r3, [pc, #640]	; (3000b1a4 <rxi316_dram_init+0x980>)
3000af22:	4003      	ands	r3, r0
3000af24:	60e3      	str	r3, [r4, #12]
3000af26:	2308      	movs	r3, #8
3000af28:	62a1      	str	r1, [r4, #40]	; 0x28
3000af2a:	62e2      	str	r2, [r4, #44]	; 0x2c
3000af2c:	6023      	str	r3, [r4, #0]
3000af2e:	6823      	ldr	r3, [r4, #0]
3000af30:	071d      	lsls	r5, r3, #28
3000af32:	d5fc      	bpl.n	3000af2e <rxi316_dram_init+0x70a>
3000af34:	4b9c      	ldr	r3, [pc, #624]	; (3000b1a8 <rxi316_dram_init+0x984>)
3000af36:	2001      	movs	r0, #1
3000af38:	4798      	blx	r3
3000af3a:	68e2      	ldr	r2, [r4, #12]
3000af3c:	4b99      	ldr	r3, [pc, #612]	; (3000b1a4 <rxi316_dram_init+0x980>)
3000af3e:	f44f 1100 	mov.w	r1, #2097152	; 0x200000
3000af42:	4013      	ands	r3, r2
3000af44:	2203      	movs	r2, #3
3000af46:	60e3      	str	r3, [r4, #12]
3000af48:	2308      	movs	r3, #8
3000af4a:	62a1      	str	r1, [r4, #40]	; 0x28
3000af4c:	62e2      	str	r2, [r4, #44]	; 0x2c
3000af4e:	4a90      	ldr	r2, [pc, #576]	; (3000b190 <rxi316_dram_init+0x96c>)
3000af50:	6023      	str	r3, [r4, #0]
3000af52:	6813      	ldr	r3, [r2, #0]
3000af54:	071c      	lsls	r4, r3, #28
3000af56:	d5fc      	bpl.n	3000af52 <rxi316_dram_init+0x72e>
3000af58:	68d0      	ldr	r0, [r2, #12]
3000af5a:	f04f 7190 	mov.w	r1, #18874368	; 0x1200000
3000af5e:	4b91      	ldr	r3, [pc, #580]	; (3000b1a4 <rxi316_dram_init+0x980>)
3000af60:	4003      	ands	r3, r0
3000af62:	2008      	movs	r0, #8
3000af64:	60d3      	str	r3, [r2, #12]
3000af66:	6291      	str	r1, [r2, #40]	; 0x28
3000af68:	6ad3      	ldr	r3, [r2, #44]	; 0x2c
3000af6a:	4989      	ldr	r1, [pc, #548]	; (3000b190 <rxi316_dram_init+0x96c>)
3000af6c:	f023 0302 	bic.w	r3, r3, #2
3000af70:	62d3      	str	r3, [r2, #44]	; 0x2c
3000af72:	6010      	str	r0, [r2, #0]
3000af74:	680b      	ldr	r3, [r1, #0]
3000af76:	0718      	lsls	r0, r3, #28
3000af78:	d5fc      	bpl.n	3000af74 <rxi316_dram_init+0x750>
3000af7a:	4a85      	ldr	r2, [pc, #532]	; (3000b190 <rxi316_dram_init+0x96c>)
3000af7c:	f44f 63c0 	mov.w	r3, #1536	; 0x600
3000af80:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000af84:	60d3      	str	r3, [r2, #12]
3000af86:	980a      	ldr	r0, [sp, #40]	; 0x28
3000af88:	6913      	ldr	r3, [r2, #16]
3000af8a:	4318      	orrs	r0, r3
3000af8c:	6110      	str	r0, [r2, #16]
3000af8e:	6913      	ldr	r3, [r2, #16]
3000af90:	f023 5380 	bic.w	r3, r3, #268435456	; 0x10000000
3000af94:	6113      	str	r3, [r2, #16]
3000af96:	6011      	str	r1, [r2, #0]
3000af98:	68d3      	ldr	r3, [r2, #12]
3000af9a:	f3c3 030a 	ubfx	r3, r3, #0, #11
3000af9e:	f5b3 6fc0 	cmp.w	r3, #1536	; 0x600
3000afa2:	d1f9      	bne.n	3000af98 <rxi316_dram_init+0x774>
3000afa4:	b00f      	add	sp, #60	; 0x3c
3000afa6:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000afaa:	2a01      	cmp	r2, #1
3000afac:	f000 80b4 	beq.w	3000b118 <rxi316_dram_init+0x8f4>
3000afb0:	2a02      	cmp	r2, #2
3000afb2:	f000 811d 	beq.w	3000b1f0 <rxi316_dram_init+0x9cc>
3000afb6:	2a03      	cmp	r2, #3
3000afb8:	f47f acda 	bne.w	3000a970 <rxi316_dram_init+0x14c>
3000afbc:	686a      	ldr	r2, [r5, #4]
3000afbe:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000afc2:	606a      	str	r2, [r5, #4]
3000afc4:	e4d4      	b.n	3000a970 <rxi316_dram_init+0x14c>
3000afc6:	2a00      	cmp	r2, #0
3000afc8:	f43f ad60 	beq.w	3000aa8c <rxi316_dram_init+0x268>
3000afcc:	2203      	movs	r2, #3
3000afce:	4310      	orrs	r0, r2
3000afd0:	68a2      	ldr	r2, [r4, #8]
3000afd2:	9f09      	ldr	r7, [sp, #36]	; 0x24
3000afd4:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000afd6:	013f      	lsls	r7, r7, #4
3000afd8:	2d00      	cmp	r5, #0
3000afda:	f000 80ba 	beq.w	3000b152 <rxi316_dram_init+0x92e>
3000afde:	1e91      	subs	r1, r2, #2
3000afe0:	2906      	cmp	r1, #6
3000afe2:	f200 8093 	bhi.w	3000b10c <rxi316_dram_init+0x8e8>
3000afe6:	1e55      	subs	r5, r2, #1
3000afe8:	2100      	movs	r1, #0
3000afea:	026d      	lsls	r5, r5, #9
3000afec:	e555      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000afee:	2201      	movs	r2, #1
3000aff0:	601a      	str	r2, [r3, #0]
3000aff2:	681a      	ldr	r2, [r3, #0]
3000aff4:	07d2      	lsls	r2, r2, #31
3000aff6:	d5fc      	bpl.n	3000aff2 <rxi316_dram_init+0x7ce>
3000aff8:	e7bf      	b.n	3000af7a <rxi316_dram_init+0x756>
3000affa:	2701      	movs	r7, #1
3000affc:	e71a      	b.n	3000ae34 <rxi316_dram_init+0x610>
3000affe:	68a1      	ldr	r1, [r4, #8]
3000b000:	2f03      	cmp	r7, #3
3000b002:	6a09      	ldr	r1, [r1, #32]
3000b004:	f000 80a2 	beq.w	3000b14c <rxi316_dram_init+0x928>
3000b008:	2f02      	cmp	r7, #2
3000b00a:	f040 80fc 	bne.w	3000b206 <rxi316_dram_init+0x9e2>
3000b00e:	fbb1 fcf3 	udiv	ip, r1, r3
3000b012:	fb03 1e1c 	mls	lr, r3, ip, r1
3000b016:	4661      	mov	r1, ip
3000b018:	f1be 0f00 	cmp.w	lr, #0
3000b01c:	f000 808c 	beq.w	3000b138 <rxi316_dram_init+0x914>
3000b020:	f1b9 0f02 	cmp.w	r9, #2
3000b024:	f101 0101 	add.w	r1, r1, #1
3000b028:	46b8      	mov	r8, r7
3000b02a:	f47f aeab 	bne.w	3000ad84 <rxi316_dram_init+0x560>
3000b02e:	4f5f      	ldr	r7, [pc, #380]	; (3000b1ac <rxi316_dram_init+0x988>)
3000b030:	f04f 0e00 	mov.w	lr, #0
3000b034:	f8df 9178 	ldr.w	r9, [pc, #376]	; 3000b1b0 <rxi316_dram_init+0x98c>
3000b038:	fbb7 f7f3 	udiv	r7, r7, r3
3000b03c:	fbb9 f9f3 	udiv	r9, r9, r3
3000b040:	09ff      	lsrs	r7, r7, #7
3000b042:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b046:	3701      	adds	r7, #1
3000b048:	f109 0c01 	add.w	ip, r9, #1
3000b04c:	f8df 9164 	ldr.w	r9, [pc, #356]	; 3000b1b4 <rxi316_dram_init+0x990>
3000b050:	023f      	lsls	r7, r7, #8
3000b052:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b056:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b05a:	9709      	str	r7, [sp, #36]	; 0x24
3000b05c:	e69f      	b.n	3000ad9e <rxi316_dram_init+0x57a>
3000b05e:	f8cd c018 	str.w	ip, [sp, #24]
3000b062:	e5b2      	b.n	3000abca <rxi316_dram_init+0x3a6>
3000b064:	38c8      	subs	r0, #200	; 0xc8
3000b066:	e58f      	b.n	3000ab88 <rxi316_dram_init+0x364>
3000b068:	9105      	str	r1, [sp, #20]
3000b06a:	e580      	b.n	3000ab6e <rxi316_dram_init+0x34a>
3000b06c:	ea1a 0f07 	tst.w	sl, r7
3000b070:	fa27 f00b 	lsr.w	r0, r7, fp
3000b074:	f43f ad55 	beq.w	3000ab22 <rxi316_dram_init+0x2fe>
3000b078:	e552      	b.n	3000ab20 <rxi316_dram_init+0x2fc>
3000b07a:	9a0d      	ldr	r2, [sp, #52]	; 0x34
3000b07c:	1f50      	subs	r0, r2, #5
3000b07e:	00c0      	lsls	r0, r0, #3
3000b080:	68e2      	ldr	r2, [r4, #12]
3000b082:	6812      	ldr	r2, [r2, #0]
3000b084:	6b92      	ldr	r2, [r2, #56]	; 0x38
3000b086:	fab2 f282 	clz	r2, r2
3000b08a:	0952      	lsrs	r2, r2, #5
3000b08c:	ea41 2142 	orr.w	r1, r1, r2, lsl #9
3000b090:	4301      	orrs	r1, r0
3000b092:	e519      	b.n	3000aac8 <rxi316_dram_init+0x2a4>
3000b094:	f1a1 0502 	sub.w	r5, r1, #2
3000b098:	2909      	cmp	r1, #9
3000b09a:	fab5 f585 	clz	r5, r5
3000b09e:	ea4f 1555 	mov.w	r5, r5, lsr #5
3000b0a2:	f000 80a3 	beq.w	3000b1ec <rxi316_dram_init+0x9c8>
3000b0a6:	462a      	mov	r2, r5
3000b0a8:	e4ed      	b.n	3000aa86 <rxi316_dram_init+0x262>
3000b0aa:	9d09      	ldr	r5, [sp, #36]	; 0x24
3000b0ac:	46a8      	mov	r8, r5
3000b0ae:	9d08      	ldr	r5, [sp, #32]
3000b0b0:	44a8      	add	r8, r5
3000b0b2:	e619      	b.n	3000ace8 <rxi316_dram_init+0x4c4>
3000b0b4:	9a08      	ldr	r2, [sp, #32]
3000b0b6:	fa22 f10b 	lsr.w	r1, r2, fp
3000b0ba:	e5a9      	b.n	3000ac10 <rxi316_dram_init+0x3ec>
3000b0bc:	2000      	movs	r0, #0
3000b0be:	e7df      	b.n	3000b080 <rxi316_dram_init+0x85c>
3000b0c0:	f8dd 801c 	ldr.w	r8, [sp, #28]
3000b0c4:	e610      	b.n	3000ace8 <rxi316_dram_init+0x4c4>
3000b0c6:	f242 7710 	movw	r7, #10000	; 0x2710
3000b0ca:	f8df 90ec 	ldr.w	r9, [pc, #236]	; 3000b1b8 <rxi316_dram_init+0x994>
3000b0ce:	f8df e0e0 	ldr.w	lr, [pc, #224]	; 3000b1b0 <rxi316_dram_init+0x98c>
3000b0d2:	fbb7 fcf3 	udiv	ip, r7, r3
3000b0d6:	9f05      	ldr	r7, [sp, #20]
3000b0d8:	4467      	add	r7, ip
3000b0da:	fbb9 f9f3 	udiv	r9, r9, r3
3000b0de:	fbbe fef3 	udiv	lr, lr, r3
3000b0e2:	09ff      	lsrs	r7, r7, #7
3000b0e4:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b0e8:	3701      	adds	r7, #1
3000b0ea:	ea4f 2e9e 	mov.w	lr, lr, lsr #10
3000b0ee:	f109 0c01 	add.w	ip, r9, #1
3000b0f2:	f8df 90c0 	ldr.w	r9, [pc, #192]	; 3000b1b4 <rxi316_dram_init+0x990>
3000b0f6:	023f      	lsls	r7, r7, #8
3000b0f8:	f10e 0e01 	add.w	lr, lr, #1
3000b0fc:	ea09 390c 	and.w	r9, r9, ip, lsl #12
3000b100:	f407 6770 	and.w	r7, r7, #3840	; 0xf00
3000b104:	ea4f 5e8e 	mov.w	lr, lr, lsl #22
3000b108:	9709      	str	r7, [sp, #36]	; 0x24
3000b10a:	e648      	b.n	3000ad9e <rxi316_dram_init+0x57a>
3000b10c:	2100      	movs	r1, #0
3000b10e:	460d      	mov	r5, r1
3000b110:	e4c3      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b112:	3a07      	subs	r2, #7
3000b114:	f7ff bbcb 	b.w	3000a8ae <rxi316_dram_init+0x8a>
3000b118:	686a      	ldr	r2, [r5, #4]
3000b11a:	f042 0220 	orr.w	r2, r2, #32
3000b11e:	606a      	str	r2, [r5, #4]
3000b120:	e426      	b.n	3000a970 <rxi316_dram_init+0x14c>
3000b122:	2204      	movs	r2, #4
3000b124:	2102      	movs	r1, #2
3000b126:	2500      	movs	r5, #0
3000b128:	9201      	str	r2, [sp, #4]
3000b12a:	e9cd 2107 	strd	r2, r1, [sp, #28]
3000b12e:	f7ff bba7 	b.w	3000a880 <rxi316_dram_init+0x5c>
3000b132:	3a08      	subs	r2, #8
3000b134:	f7ff bbbb 	b.w	3000a8ae <rxi316_dram_init+0x8a>
3000b138:	46b8      	mov	r8, r7
3000b13a:	e61f      	b.n	3000ad7c <rxi316_dram_init+0x558>
3000b13c:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b13e:	2a04      	cmp	r2, #4
3000b140:	d90c      	bls.n	3000b15c <rxi316_dram_init+0x938>
3000b142:	2a0b      	cmp	r2, #11
3000b144:	d86b      	bhi.n	3000b21e <rxi316_dram_init+0x9fa>
3000b146:	1f17      	subs	r7, r2, #4
3000b148:	013f      	lsls	r7, r7, #4
3000b14a:	e008      	b.n	3000b15e <rxi316_dram_init+0x93a>
3000b14c:	f04f 0802 	mov.w	r8, #2
3000b150:	e614      	b.n	3000ad7c <rxi316_dram_init+0x558>
3000b152:	2903      	cmp	r1, #3
3000b154:	d005      	beq.n	3000b162 <rxi316_dram_init+0x93e>
3000b156:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b15a:	e49e      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b15c:	2710      	movs	r7, #16
3000b15e:	68a2      	ldr	r2, [r4, #8]
3000b160:	6dd2      	ldr	r2, [r2, #92]	; 0x5c
3000b162:	2a04      	cmp	r2, #4
3000b164:	d94a      	bls.n	3000b1fc <rxi316_dram_init+0x9d8>
3000b166:	1f51      	subs	r1, r2, #5
3000b168:	2903      	cmp	r1, #3
3000b16a:	d84f      	bhi.n	3000b20c <rxi316_dram_init+0x9e8>
3000b16c:	1f15      	subs	r5, r2, #4
3000b16e:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b172:	026d      	lsls	r5, r5, #9
3000b174:	e491      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b176:	f1a1 0c02 	sub.w	ip, r1, #2
3000b17a:	f1bc 0f01 	cmp.w	ip, #1
3000b17e:	d903      	bls.n	3000b188 <rxi316_dram_init+0x964>
3000b180:	f1be 0f00 	cmp.w	lr, #0
3000b184:	f43f ac7f 	beq.w	3000aa86 <rxi316_dram_init+0x262>
3000b188:	f040 0002 	orr.w	r0, r0, #2
3000b18c:	e47b      	b.n	3000aa86 <rxi316_dram_init+0x262>
3000b18e:	bf00      	nop
3000b190:	41100000 	.word	0x41100000
3000b194:	42008000 	.word	0x42008000
3000b198:	41011000 	.word	0x41011000
3000b19c:	44000103 	.word	0x44000103
3000b1a0:	44000021 	.word	0x44000021
3000b1a4:	fff8fffc 	.word	0xfff8fffc
3000b1a8:	00009b2d 	.word	0x00009b2d
3000b1ac:	00061a80 	.word	0x00061a80
3000b1b0:	0bebc200 	.word	0x0bebc200
3000b1b4:	003ff000 	.word	0x003ff000
3000b1b8:	1dcd6500 	.word	0x1dcd6500
3000b1bc:	4f1d      	ldr	r7, [pc, #116]	; (3000b234 <rxi316_dram_init+0xa10>)
3000b1be:	f04f 0e00 	mov.w	lr, #0
3000b1c2:	fbb7 f9f3 	udiv	r9, r7, r3
3000b1c6:	ea4f 2999 	mov.w	r9, r9, lsr #10
3000b1ca:	f8cd e024 	str.w	lr, [sp, #36]	; 0x24
3000b1ce:	f109 0701 	add.w	r7, r9, #1
3000b1d2:	f8df 9064 	ldr.w	r9, [pc, #100]	; 3000b238 <rxi316_dram_init+0xa14>
3000b1d6:	ea09 3907 	and.w	r9, r9, r7, lsl #12
3000b1da:	e5e0      	b.n	3000ad9e <rxi316_dram_init+0x57a>
3000b1dc:	f105 0802 	add.w	r8, r5, #2
3000b1e0:	9d0d      	ldr	r5, [sp, #52]	; 0x34
3000b1e2:	eba8 0805 	sub.w	r8, r8, r5
3000b1e6:	9d08      	ldr	r5, [sp, #32]
3000b1e8:	44a8      	add	r8, r5
3000b1ea:	e57d      	b.n	3000ace8 <rxi316_dram_init+0x4c4>
3000b1ec:	2204      	movs	r2, #4
3000b1ee:	e6ee      	b.n	3000afce <rxi316_dram_init+0x7aa>
3000b1f0:	686a      	ldr	r2, [r5, #4]
3000b1f2:	f042 0240 	orr.w	r2, r2, #64	; 0x40
3000b1f6:	606a      	str	r2, [r5, #4]
3000b1f8:	f7ff bbba 	b.w	3000a970 <rxi316_dram_init+0x14c>
3000b1fc:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b200:	f44f 7500 	mov.w	r5, #512	; 0x200
3000b204:	e449      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b206:	f04f 0802 	mov.w	r8, #2
3000b20a:	e5b1      	b.n	3000ad70 <rxi316_dram_init+0x54c>
3000b20c:	f1a2 0109 	sub.w	r1, r2, #9
3000b210:	2905      	cmp	r1, #5
3000b212:	d80b      	bhi.n	3000b22c <rxi316_dram_init+0xa08>
3000b214:	0855      	lsrs	r5, r2, #1
3000b216:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b21a:	026d      	lsls	r5, r5, #9
3000b21c:	e43d      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b21e:	9a09      	ldr	r2, [sp, #36]	; 0x24
3000b220:	f040 0004 	orr.w	r0, r0, #4
3000b224:	f1a2 070c 	sub.w	r7, r2, #12
3000b228:	013f      	lsls	r7, r7, #4
3000b22a:	e798      	b.n	3000b15e <rxi316_dram_init+0x93a>
3000b22c:	f44f 5180 	mov.w	r1, #4096	; 0x1000
3000b230:	2500      	movs	r5, #0
3000b232:	e432      	b.n	3000aa9a <rxi316_dram_init+0x276>
3000b234:	0bebc200 	.word	0x0bebc200
3000b238:	003ff000 	.word	0x003ff000

3000b23c <rxi316_ftend_init>:
3000b23c:	2300      	movs	r3, #0
3000b23e:	b510      	push	{r4, lr}
3000b240:	4c0c      	ldr	r4, [pc, #48]	; (3000b274 <rxi316_ftend_init+0x38>)
3000b242:	f8c4 3818 	str.w	r3, [r4, #2072]	; 0x818
3000b246:	f8c4 3858 	str.w	r3, [r4, #2136]	; 0x858
3000b24a:	f8c4 3898 	str.w	r3, [r4, #2200]	; 0x898
3000b24e:	f7fe fe5d 	bl	30009f0c <ddr_init_index>
3000b252:	4a09      	ldr	r2, [pc, #36]	; (3000b278 <rxi316_ftend_init+0x3c>)
3000b254:	f8d4 3104 	ldr.w	r3, [r4, #260]	; 0x104
3000b258:	f852 1020 	ldr.w	r1, [r2, r0, lsl #2]
3000b25c:	f023 02ff 	bic.w	r2, r3, #255	; 0xff
3000b260:	68cb      	ldr	r3, [r1, #12]
3000b262:	689b      	ldr	r3, [r3, #8]
3000b264:	6a5b      	ldr	r3, [r3, #36]	; 0x24
3000b266:	3301      	adds	r3, #1
3000b268:	b2db      	uxtb	r3, r3
3000b26a:	4313      	orrs	r3, r2
3000b26c:	f8c4 3104 	str.w	r3, [r4, #260]	; 0x104
3000b270:	bd10      	pop	{r4, pc}
3000b272:	bf00      	nop
3000b274:	41101000 	.word	0x41101000
3000b278:	3000f500 	.word	0x3000f500

3000b27c <rxi316_DynSre_init>:
3000b27c:	b538      	push	{r3, r4, r5, lr}
3000b27e:	4b22      	ldr	r3, [pc, #136]	; (3000b308 <rxi316_DynSre_init+0x8c>)
3000b280:	4604      	mov	r4, r0
3000b282:	460d      	mov	r5, r1
3000b284:	4798      	blx	r3
3000b286:	2802      	cmp	r0, #2
3000b288:	d937      	bls.n	3000b2fa <rxi316_DynSre_init+0x7e>
3000b28a:	4b20      	ldr	r3, [pc, #128]	; (3000b30c <rxi316_DynSre_init+0x90>)
3000b28c:	691b      	ldr	r3, [r3, #16]
3000b28e:	f3c3 230f 	ubfx	r3, r3, #8, #16
3000b292:	3b46      	subs	r3, #70	; 0x46
3000b294:	42a3      	cmp	r3, r4
3000b296:	d915      	bls.n	3000b2c4 <rxi316_DynSre_init+0x48>
3000b298:	2d01      	cmp	r5, #1
3000b29a:	d01a      	beq.n	3000b2d2 <rxi316_DynSre_init+0x56>
3000b29c:	4a1b      	ldr	r2, [pc, #108]	; (3000b30c <rxi316_DynSre_init+0x90>)
3000b29e:	6893      	ldr	r3, [r2, #8]
3000b2a0:	f023 0380 	bic.w	r3, r3, #128	; 0x80
3000b2a4:	6093      	str	r3, [r2, #8]
3000b2a6:	4b19      	ldr	r3, [pc, #100]	; (3000b30c <rxi316_DynSre_init+0x90>)
3000b2a8:	f44f 1200 	mov.w	r2, #2097152	; 0x200000
3000b2ac:	f04f 4100 	mov.w	r1, #2147483648	; 0x80000000
3000b2b0:	625a      	str	r2, [r3, #36]	; 0x24
3000b2b2:	629a      	str	r2, [r3, #40]	; 0x28
3000b2b4:	69da      	ldr	r2, [r3, #28]
3000b2b6:	f422 6270 	bic.w	r2, r2, #3840	; 0xf00
3000b2ba:	f442 7200 	orr.w	r2, r2, #512	; 0x200
3000b2be:	61da      	str	r2, [r3, #28]
3000b2c0:	6019      	str	r1, [r3, #0]
3000b2c2:	bd38      	pop	{r3, r4, r5, pc}
3000b2c4:	f240 411e 	movw	r1, #1054	; 0x41e
3000b2c8:	4811      	ldr	r0, [pc, #68]	; (3000b310 <rxi316_DynSre_init+0x94>)
3000b2ca:	f001 f905 	bl	3000c4d8 <__io_assert_failed_veneer>
3000b2ce:	2d01      	cmp	r5, #1
3000b2d0:	d1e4      	bne.n	3000b29c <rxi316_DynSre_init+0x20>
3000b2d2:	4b0e      	ldr	r3, [pc, #56]	; (3000b30c <rxi316_DynSre_init+0x90>)
3000b2d4:	f64f 70fe 	movw	r0, #65534	; 0xfffe
3000b2d8:	4a0e      	ldr	r2, [pc, #56]	; (3000b314 <rxi316_DynSre_init+0x98>)
3000b2da:	f8d3 1100 	ldr.w	r1, [r3, #256]	; 0x100
3000b2de:	4004      	ands	r4, r0
3000b2e0:	400a      	ands	r2, r1
3000b2e2:	f8c3 2100 	str.w	r2, [r3, #256]	; 0x100
3000b2e6:	f8d3 0100 	ldr.w	r0, [r3, #256]	; 0x100
3000b2ea:	4304      	orrs	r4, r0
3000b2ec:	f8c3 4100 	str.w	r4, [r3, #256]	; 0x100
3000b2f0:	689a      	ldr	r2, [r3, #8]
3000b2f2:	f042 0280 	orr.w	r2, r2, #128	; 0x80
3000b2f6:	609a      	str	r2, [r3, #8]
3000b2f8:	e7d5      	b.n	3000b2a6 <rxi316_DynSre_init+0x2a>
3000b2fa:	4b07      	ldr	r3, [pc, #28]	; (3000b318 <rxi316_DynSre_init+0x9c>)
3000b2fc:	4798      	blx	r3
3000b2fe:	2801      	cmp	r0, #1
3000b300:	bf08      	it	eq
3000b302:	2400      	moveq	r4, #0
3000b304:	e7c8      	b.n	3000b298 <rxi316_DynSre_init+0x1c>
3000b306:	bf00      	nop
3000b308:	30009bfd 	.word	0x30009bfd
3000b30c:	41100000 	.word	0x41100000
3000b310:	3000d8fc 	.word	0x3000d8fc
3000b314:	ffff0000 	.word	0xffff0000
3000b318:	30007f55 	.word	0x30007f55

3000b31c <ddr_init>:
3000b31c:	b500      	push	{lr}
3000b31e:	b083      	sub	sp, #12
3000b320:	f7fe fdf4 	bl	30009f0c <ddr_init_index>
3000b324:	4b06      	ldr	r3, [pc, #24]	; (3000b340 <ddr_init+0x24>)
3000b326:	f853 0020 	ldr.w	r0, [r3, r0, lsl #2]
3000b32a:	9001      	str	r0, [sp, #4]
3000b32c:	f7fe fe18 	bl	30009f60 <rxi316_perf_tune>
3000b330:	9801      	ldr	r0, [sp, #4]
3000b332:	f7ff fa77 	bl	3000a824 <rxi316_dram_init>
3000b336:	b003      	add	sp, #12
3000b338:	f85d eb04 	ldr.w	lr, [sp], #4
3000b33c:	f7ff bf7e 	b.w	3000b23c <rxi316_ftend_init>
3000b340:	3000f500 	.word	0x3000f500

3000b344 <SWR_Calib_DCore>:
3000b344:	4b0f      	ldr	r3, [pc, #60]	; (3000b384 <SWR_Calib_DCore+0x40>)
3000b346:	f240 3231 	movw	r2, #817	; 0x331
3000b34a:	490f      	ldr	r1, [pc, #60]	; (3000b388 <SWR_Calib_DCore+0x44>)
3000b34c:	6d18      	ldr	r0, [r3, #80]	; 0x50
3000b34e:	4001      	ands	r1, r0
3000b350:	430a      	orrs	r2, r1
3000b352:	651a      	str	r2, [r3, #80]	; 0x50
3000b354:	6c1a      	ldr	r2, [r3, #64]	; 0x40
3000b356:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b35a:	641a      	str	r2, [r3, #64]	; 0x40
3000b35c:	f8d3 2084 	ldr.w	r2, [r3, #132]	; 0x84
3000b360:	f042 0201 	orr.w	r2, r2, #1
3000b364:	f8c3 2084 	str.w	r2, [r3, #132]	; 0x84
3000b368:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b36c:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b370:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b374:	685a      	ldr	r2, [r3, #4]
3000b376:	f022 02ff 	bic.w	r2, r2, #255	; 0xff
3000b37a:	f042 0204 	orr.w	r2, r2, #4
3000b37e:	605a      	str	r2, [r3, #4]
3000b380:	4770      	bx	lr
3000b382:	bf00      	nop
3000b384:	42008100 	.word	0x42008100
3000b388:	fffffccc 	.word	0xfffffccc

3000b38c <SWR_Calib_MEM>:
3000b38c:	4b51      	ldr	r3, [pc, #324]	; (3000b4d4 <SWR_Calib_MEM+0x148>)
3000b38e:	f240 3231 	movw	r2, #817	; 0x331
3000b392:	4951      	ldr	r1, [pc, #324]	; (3000b4d8 <SWR_Calib_MEM+0x14c>)
3000b394:	f04f 0c00 	mov.w	ip, #0
3000b398:	b510      	push	{r4, lr}
3000b39a:	f8d3 00e0 	ldr.w	r0, [r3, #224]	; 0xe0
3000b39e:	b082      	sub	sp, #8
3000b3a0:	4001      	ands	r1, r0
3000b3a2:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b3a6:	430a      	orrs	r2, r1
3000b3a8:	f10d 0107 	add.w	r1, sp, #7
3000b3ac:	f8c3 20e0 	str.w	r2, [r3, #224]	; 0xe0
3000b3b0:	f8d3 20d0 	ldr.w	r2, [r3, #208]	; 0xd0
3000b3b4:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b3b8:	f8c3 20d0 	str.w	r2, [r3, #208]	; 0xd0
3000b3bc:	f8d3 20f8 	ldr.w	r2, [r3, #248]	; 0xf8
3000b3c0:	f042 0201 	orr.w	r2, r2, #1
3000b3c4:	f8c3 20f8 	str.w	r2, [r3, #248]	; 0xf8
3000b3c8:	f8d3 20fc 	ldr.w	r2, [r3, #252]	; 0xfc
3000b3cc:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b3d0:	f8c3 20fc 	str.w	r2, [r3, #252]	; 0xfc
3000b3d4:	f88d c007 	strb.w	ip, [sp, #7]
3000b3d8:	f7fd fb50 	bl	30008a7c <OTP_Read8>
3000b3dc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b3e0:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b3e4:	2af0      	cmp	r2, #240	; 0xf0
3000b3e6:	d156      	bne.n	3000b496 <SWR_Calib_MEM+0x10a>
3000b3e8:	f003 030f 	and.w	r3, r3, #15
3000b3ec:	2b0f      	cmp	r3, #15
3000b3ee:	d04e      	beq.n	3000b48e <SWR_Calib_MEM+0x102>
3000b3f0:	2302      	movs	r3, #2
3000b3f2:	4c3a      	ldr	r4, [pc, #232]	; (3000b4dc <SWR_Calib_MEM+0x150>)
3000b3f4:	f10d 0106 	add.w	r1, sp, #6
3000b3f8:	f240 70e4 	movw	r0, #2020	; 0x7e4
3000b3fc:	7023      	strb	r3, [r4, #0]
3000b3fe:	f7fd fb3d 	bl	30008a7c <OTP_Read8>
3000b402:	7822      	ldrb	r2, [r4, #0]
3000b404:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b408:	2a03      	cmp	r2, #3
3000b40a:	d05a      	beq.n	3000b4c2 <SWR_Calib_MEM+0x136>
3000b40c:	2a02      	cmp	r2, #2
3000b40e:	bf0c      	ite	eq
3000b410:	f003 030f 	andeq.w	r3, r3, #15
3000b414:	2300      	movne	r3, #0
3000b416:	492f      	ldr	r1, [pc, #188]	; (3000b4d4 <SWR_Calib_MEM+0x148>)
3000b418:	4831      	ldr	r0, [pc, #196]	; (3000b4e0 <SWR_Calib_MEM+0x154>)
3000b41a:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b41e:	f022 020f 	bic.w	r2, r2, #15
3000b422:	4313      	orrs	r3, r2
3000b424:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b428:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b42a:	f3c3 1382 	ubfx	r3, r3, #6, #3
3000b42e:	2b07      	cmp	r3, #7
3000b430:	d03a      	beq.n	3000b4a8 <SWR_Calib_MEM+0x11c>
3000b432:	2300      	movs	r3, #0
3000b434:	f10d 0107 	add.w	r1, sp, #7
3000b438:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b43c:	f88d 3007 	strb.w	r3, [sp, #7]
3000b440:	f7fd fb1c 	bl	30008a7c <OTP_Read8>
3000b444:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b448:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b44c:	2af0      	cmp	r2, #240	; 0xf0
3000b44e:	d124      	bne.n	3000b49a <SWR_Calib_MEM+0x10e>
3000b450:	f003 030f 	and.w	r3, r3, #15
3000b454:	2b0f      	cmp	r3, #15
3000b456:	d022      	beq.n	3000b49e <SWR_Calib_MEM+0x112>
3000b458:	2302      	movs	r3, #2
3000b45a:	4c22      	ldr	r4, [pc, #136]	; (3000b4e4 <SWR_Calib_MEM+0x158>)
3000b45c:	f10d 0107 	add.w	r1, sp, #7
3000b460:	f240 70e5 	movw	r0, #2021	; 0x7e5
3000b464:	7023      	strb	r3, [r4, #0]
3000b466:	f7fd fb09 	bl	30008a7c <OTP_Read8>
3000b46a:	7822      	ldrb	r2, [r4, #0]
3000b46c:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b470:	2a03      	cmp	r2, #3
3000b472:	d028      	beq.n	3000b4c6 <SWR_Calib_MEM+0x13a>
3000b474:	2a02      	cmp	r2, #2
3000b476:	d029      	beq.n	3000b4cc <SWR_Calib_MEM+0x140>
3000b478:	2200      	movs	r2, #0
3000b47a:	4916      	ldr	r1, [pc, #88]	; (3000b4d4 <SWR_Calib_MEM+0x148>)
3000b47c:	f8d1 30d0 	ldr.w	r3, [r1, #208]	; 0xd0
3000b480:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b484:	4313      	orrs	r3, r2
3000b486:	f8c1 30d0 	str.w	r3, [r1, #208]	; 0xd0
3000b48a:	b002      	add	sp, #8
3000b48c:	bd10      	pop	{r4, pc}
3000b48e:	4b13      	ldr	r3, [pc, #76]	; (3000b4dc <SWR_Calib_MEM+0x150>)
3000b490:	2201      	movs	r2, #1
3000b492:	701a      	strb	r2, [r3, #0]
3000b494:	e7cd      	b.n	3000b432 <SWR_Calib_MEM+0xa6>
3000b496:	2303      	movs	r3, #3
3000b498:	e7ab      	b.n	3000b3f2 <SWR_Calib_MEM+0x66>
3000b49a:	2303      	movs	r3, #3
3000b49c:	e7dd      	b.n	3000b45a <SWR_Calib_MEM+0xce>
3000b49e:	4b11      	ldr	r3, [pc, #68]	; (3000b4e4 <SWR_Calib_MEM+0x158>)
3000b4a0:	2201      	movs	r2, #1
3000b4a2:	701a      	strb	r2, [r3, #0]
3000b4a4:	b002      	add	sp, #8
3000b4a6:	bd10      	pop	{r4, pc}
3000b4a8:	6c83      	ldr	r3, [r0, #72]	; 0x48
3000b4aa:	f413 6f60 	tst.w	r3, #3584	; 0xe00
3000b4ae:	d1c0      	bne.n	3000b432 <SWR_Calib_MEM+0xa6>
3000b4b0:	f8d1 30e4 	ldr.w	r3, [r1, #228]	; 0xe4
3000b4b4:	f423 73e0 	bic.w	r3, r3, #448	; 0x1c0
3000b4b8:	f443 6360 	orr.w	r3, r3, #3584	; 0xe00
3000b4bc:	f8c1 30e4 	str.w	r3, [r1, #228]	; 0xe4
3000b4c0:	e7b7      	b.n	3000b432 <SWR_Calib_MEM+0xa6>
3000b4c2:	091b      	lsrs	r3, r3, #4
3000b4c4:	e7a7      	b.n	3000b416 <SWR_Calib_MEM+0x8a>
3000b4c6:	091b      	lsrs	r3, r3, #4
3000b4c8:	059a      	lsls	r2, r3, #22
3000b4ca:	e7d6      	b.n	3000b47a <SWR_Calib_MEM+0xee>
3000b4cc:	f003 030f 	and.w	r3, r3, #15
3000b4d0:	059a      	lsls	r2, r3, #22
3000b4d2:	e7d2      	b.n	3000b47a <SWR_Calib_MEM+0xee>
3000b4d4:	42008800 	.word	0x42008800
3000b4d8:	fffffccc 	.word	0xfffffccc
3000b4dc:	3000f69b 	.word	0x3000f69b
3000b4e0:	42008100 	.word	0x42008100
3000b4e4:	3000f69a 	.word	0x3000f69a

3000b4e8 <SWR_Calib_AUD>:
3000b4e8:	4b48      	ldr	r3, [pc, #288]	; (3000b60c <SWR_Calib_AUD+0x124>)
3000b4ea:	f240 3231 	movw	r2, #817	; 0x331
3000b4ee:	4948      	ldr	r1, [pc, #288]	; (3000b610 <SWR_Calib_AUD+0x128>)
3000b4f0:	f04f 0c00 	mov.w	ip, #0
3000b4f4:	b510      	push	{r4, lr}
3000b4f6:	f8d3 00a0 	ldr.w	r0, [r3, #160]	; 0xa0
3000b4fa:	b082      	sub	sp, #8
3000b4fc:	4001      	ands	r1, r0
3000b4fe:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b502:	430a      	orrs	r2, r1
3000b504:	f10d 0107 	add.w	r1, sp, #7
3000b508:	f8c3 20a0 	str.w	r2, [r3, #160]	; 0xa0
3000b50c:	f8d3 2090 	ldr.w	r2, [r3, #144]	; 0x90
3000b510:	f442 22c0 	orr.w	r2, r2, #393216	; 0x60000
3000b514:	f8c3 2090 	str.w	r2, [r3, #144]	; 0x90
3000b518:	f8d3 20b8 	ldr.w	r2, [r3, #184]	; 0xb8
3000b51c:	f042 0201 	orr.w	r2, r2, #1
3000b520:	f8c3 20b8 	str.w	r2, [r3, #184]	; 0xb8
3000b524:	f8d3 20bc 	ldr.w	r2, [r3, #188]	; 0xbc
3000b528:	f442 5280 	orr.w	r2, r2, #4096	; 0x1000
3000b52c:	f8c3 20bc 	str.w	r2, [r3, #188]	; 0xbc
3000b530:	f88d c007 	strb.w	ip, [sp, #7]
3000b534:	f7fd faa2 	bl	30008a7c <OTP_Read8>
3000b538:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b53c:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b540:	2af0      	cmp	r2, #240	; 0xf0
3000b542:	d150      	bne.n	3000b5e6 <SWR_Calib_AUD+0xfe>
3000b544:	f003 030f 	and.w	r3, r3, #15
3000b548:	2b0f      	cmp	r3, #15
3000b54a:	d048      	beq.n	3000b5de <SWR_Calib_AUD+0xf6>
3000b54c:	2302      	movs	r3, #2
3000b54e:	4c31      	ldr	r4, [pc, #196]	; (3000b614 <SWR_Calib_AUD+0x12c>)
3000b550:	f10d 0106 	add.w	r1, sp, #6
3000b554:	f240 70e6 	movw	r0, #2022	; 0x7e6
3000b558:	7023      	strb	r3, [r4, #0]
3000b55a:	f7fd fa8f 	bl	30008a7c <OTP_Read8>
3000b55e:	7822      	ldrb	r2, [r4, #0]
3000b560:	f89d 3006 	ldrb.w	r3, [sp, #6]
3000b564:	2a03      	cmp	r2, #3
3000b566:	d047      	beq.n	3000b5f8 <SWR_Calib_AUD+0x110>
3000b568:	2a02      	cmp	r2, #2
3000b56a:	bf0c      	ite	eq
3000b56c:	f003 030f 	andeq.w	r3, r3, #15
3000b570:	2300      	movne	r3, #0
3000b572:	4926      	ldr	r1, [pc, #152]	; (3000b60c <SWR_Calib_AUD+0x124>)
3000b574:	f8d1 2090 	ldr.w	r2, [r1, #144]	; 0x90
3000b578:	f022 020f 	bic.w	r2, r2, #15
3000b57c:	4313      	orrs	r3, r2
3000b57e:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b582:	2300      	movs	r3, #0
3000b584:	f10d 0107 	add.w	r1, sp, #7
3000b588:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b58c:	f88d 3007 	strb.w	r3, [sp, #7]
3000b590:	f7fd fa74 	bl	30008a7c <OTP_Read8>
3000b594:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b598:	f003 02f0 	and.w	r2, r3, #240	; 0xf0
3000b59c:	2af0      	cmp	r2, #240	; 0xf0
3000b59e:	d124      	bne.n	3000b5ea <SWR_Calib_AUD+0x102>
3000b5a0:	f003 030f 	and.w	r3, r3, #15
3000b5a4:	2b0f      	cmp	r3, #15
3000b5a6:	d022      	beq.n	3000b5ee <SWR_Calib_AUD+0x106>
3000b5a8:	2302      	movs	r3, #2
3000b5aa:	4c1b      	ldr	r4, [pc, #108]	; (3000b618 <SWR_Calib_AUD+0x130>)
3000b5ac:	f10d 0107 	add.w	r1, sp, #7
3000b5b0:	f240 70e7 	movw	r0, #2023	; 0x7e7
3000b5b4:	7023      	strb	r3, [r4, #0]
3000b5b6:	f7fd fa61 	bl	30008a7c <OTP_Read8>
3000b5ba:	7822      	ldrb	r2, [r4, #0]
3000b5bc:	f89d 3007 	ldrb.w	r3, [sp, #7]
3000b5c0:	2a03      	cmp	r2, #3
3000b5c2:	d01b      	beq.n	3000b5fc <SWR_Calib_AUD+0x114>
3000b5c4:	2a02      	cmp	r2, #2
3000b5c6:	d01c      	beq.n	3000b602 <SWR_Calib_AUD+0x11a>
3000b5c8:	2200      	movs	r2, #0
3000b5ca:	4910      	ldr	r1, [pc, #64]	; (3000b60c <SWR_Calib_AUD+0x124>)
3000b5cc:	f8d1 3090 	ldr.w	r3, [r1, #144]	; 0x90
3000b5d0:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b5d4:	4313      	orrs	r3, r2
3000b5d6:	f8c1 3090 	str.w	r3, [r1, #144]	; 0x90
3000b5da:	b002      	add	sp, #8
3000b5dc:	bd10      	pop	{r4, pc}
3000b5de:	4b0d      	ldr	r3, [pc, #52]	; (3000b614 <SWR_Calib_AUD+0x12c>)
3000b5e0:	2201      	movs	r2, #1
3000b5e2:	701a      	strb	r2, [r3, #0]
3000b5e4:	e7cd      	b.n	3000b582 <SWR_Calib_AUD+0x9a>
3000b5e6:	2303      	movs	r3, #3
3000b5e8:	e7b1      	b.n	3000b54e <SWR_Calib_AUD+0x66>
3000b5ea:	2303      	movs	r3, #3
3000b5ec:	e7dd      	b.n	3000b5aa <SWR_Calib_AUD+0xc2>
3000b5ee:	4b0a      	ldr	r3, [pc, #40]	; (3000b618 <SWR_Calib_AUD+0x130>)
3000b5f0:	2201      	movs	r2, #1
3000b5f2:	701a      	strb	r2, [r3, #0]
3000b5f4:	b002      	add	sp, #8
3000b5f6:	bd10      	pop	{r4, pc}
3000b5f8:	091b      	lsrs	r3, r3, #4
3000b5fa:	e7ba      	b.n	3000b572 <SWR_Calib_AUD+0x8a>
3000b5fc:	091b      	lsrs	r3, r3, #4
3000b5fe:	059a      	lsls	r2, r3, #22
3000b600:	e7e3      	b.n	3000b5ca <SWR_Calib_AUD+0xe2>
3000b602:	f003 030f 	and.w	r3, r3, #15
3000b606:	059a      	lsls	r2, r3, #22
3000b608:	e7df      	b.n	3000b5ca <SWR_Calib_AUD+0xe2>
3000b60a:	bf00      	nop
3000b60c:	42008800 	.word	0x42008800
3000b610:	fffffccc 	.word	0xfffffccc
3000b614:	3000f699 	.word	0x3000f699
3000b618:	3000f698 	.word	0x3000f698

3000b61c <SWR_MEM>:
3000b61c:	2801      	cmp	r0, #1
3000b61e:	d00e      	beq.n	3000b63e <SWR_MEM+0x22>
3000b620:	4b31      	ldr	r3, [pc, #196]	; (3000b6e8 <SWR_MEM+0xcc>)
3000b622:	2000      	movs	r0, #0
3000b624:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b628:	f022 0201 	bic.w	r2, r2, #1
3000b62c:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b630:	f8d3 20c0 	ldr.w	r2, [r3, #192]	; 0xc0
3000b634:	f022 0202 	bic.w	r2, r2, #2
3000b638:	f8c3 20c0 	str.w	r2, [r3, #192]	; 0xc0
3000b63c:	4770      	bx	lr
3000b63e:	b510      	push	{r4, lr}
3000b640:	4c29      	ldr	r4, [pc, #164]	; (3000b6e8 <SWR_MEM+0xcc>)
3000b642:	4a2a      	ldr	r2, [pc, #168]	; (3000b6ec <SWR_MEM+0xd0>)
3000b644:	f8d4 30e4 	ldr.w	r3, [r4, #228]	; 0xe4
3000b648:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b64c:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b650:	f8c4 30e4 	str.w	r3, [r4, #228]	; 0xe4
3000b654:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b658:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b65c:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b660:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b664:	f023 0303 	bic.w	r3, r3, #3
3000b668:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b66c:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b670:	f023 7370 	bic.w	r3, r3, #62914560	; 0x3c00000
3000b674:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b678:	4790      	blx	r2
3000b67a:	2803      	cmp	r0, #3
3000b67c:	f8d4 30d8 	ldr.w	r3, [r4, #216]	; 0xd8
3000b680:	d026      	beq.n	3000b6d0 <SWR_MEM+0xb4>
3000b682:	f043 0303 	orr.w	r3, r3, #3
3000b686:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b68a:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b68e:	f043 7340 	orr.w	r3, r3, #50331648	; 0x3000000
3000b692:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b696:	4c14      	ldr	r4, [pc, #80]	; (3000b6e8 <SWR_MEM+0xcc>)
3000b698:	4b15      	ldr	r3, [pc, #84]	; (3000b6f0 <SWR_MEM+0xd4>)
3000b69a:	4798      	blx	r3
3000b69c:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b6a0:	4a14      	ldr	r2, [pc, #80]	; (3000b6f4 <SWR_MEM+0xd8>)
3000b6a2:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b6a6:	f043 0302 	orr.w	r3, r3, #2
3000b6aa:	f8c4 30c0 	str.w	r3, [r4, #192]	; 0xc0
3000b6ae:	4790      	blx	r2
3000b6b0:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b6b4:	4b10      	ldr	r3, [pc, #64]	; (3000b6f8 <SWR_MEM+0xdc>)
3000b6b6:	f042 0201 	orr.w	r2, r2, #1
3000b6ba:	f8c4 20c0 	str.w	r2, [r4, #192]	; 0xc0
3000b6be:	e001      	b.n	3000b6c4 <SWR_MEM+0xa8>
3000b6c0:	3b01      	subs	r3, #1
3000b6c2:	d00e      	beq.n	3000b6e2 <SWR_MEM+0xc6>
3000b6c4:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b6c8:	03d2      	lsls	r2, r2, #15
3000b6ca:	d5f9      	bpl.n	3000b6c0 <SWR_MEM+0xa4>
3000b6cc:	2000      	movs	r0, #0
3000b6ce:	bd10      	pop	{r4, pc}
3000b6d0:	f8c4 30d8 	str.w	r3, [r4, #216]	; 0xd8
3000b6d4:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b6d8:	f443 0340 	orr.w	r3, r3, #12582912	; 0xc00000
3000b6dc:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b6e0:	e7d9      	b.n	3000b696 <SWR_MEM+0x7a>
3000b6e2:	2001      	movs	r0, #1
3000b6e4:	bd10      	pop	{r4, pc}
3000b6e6:	bf00      	nop
3000b6e8:	42008800 	.word	0x42008800
3000b6ec:	30007d85 	.word	0x30007d85
3000b6f0:	3000b38d 	.word	0x3000b38d
3000b6f4:	00009b2d 	.word	0x00009b2d
3000b6f8:	000186a0 	.word	0x000186a0

3000b6fc <SWR_MEM_Manual>:
3000b6fc:	4a06      	ldr	r2, [pc, #24]	; (3000b718 <SWR_MEM_Manual+0x1c>)
3000b6fe:	f8d2 30c0 	ldr.w	r3, [r2, #192]	; 0xc0
3000b702:	b120      	cbz	r0, 3000b70e <SWR_MEM_Manual+0x12>
3000b704:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b708:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b70c:	4770      	bx	lr
3000b70e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b712:	f8c2 30c0 	str.w	r3, [r2, #192]	; 0xc0
3000b716:	4770      	bx	lr
3000b718:	42008800 	.word	0x42008800

3000b71c <SWR_MEM_Mode_Set>:
3000b71c:	2801      	cmp	r0, #1
3000b71e:	b538      	push	{r3, r4, r5, lr}
3000b720:	d807      	bhi.n	3000b732 <SWR_MEM_Mode_Set+0x16>
3000b722:	d10a      	bne.n	3000b73a <SWR_MEM_Mode_Set+0x1e>
3000b724:	4c22      	ldr	r4, [pc, #136]	; (3000b7b0 <SWR_MEM_Mode_Set+0x94>)
3000b726:	f8d4 30c0 	ldr.w	r3, [r4, #192]	; 0xc0
3000b72a:	035b      	lsls	r3, r3, #13
3000b72c:	d51b      	bpl.n	3000b766 <SWR_MEM_Mode_Set+0x4a>
3000b72e:	2000      	movs	r0, #0
3000b730:	bd38      	pop	{r3, r4, r5, pc}
3000b732:	217e      	movs	r1, #126	; 0x7e
3000b734:	481f      	ldr	r0, [pc, #124]	; (3000b7b4 <SWR_MEM_Mode_Set+0x98>)
3000b736:	f000 fecf 	bl	3000c4d8 <__io_assert_failed_veneer>
3000b73a:	491d      	ldr	r1, [pc, #116]	; (3000b7b0 <SWR_MEM_Mode_Set+0x94>)
3000b73c:	f8d1 30c0 	ldr.w	r3, [r1, #192]	; 0xc0
3000b740:	0358      	lsls	r0, r3, #13
3000b742:	d5f4      	bpl.n	3000b72e <SWR_MEM_Mode_Set+0x12>
3000b744:	f8d1 20d0 	ldr.w	r2, [r1, #208]	; 0xd0
3000b748:	f242 7310 	movw	r3, #10000	; 0x2710
3000b74c:	f022 0240 	bic.w	r2, r2, #64	; 0x40
3000b750:	f8c1 20d0 	str.w	r2, [r1, #208]	; 0xd0
3000b754:	e001      	b.n	3000b75a <SWR_MEM_Mode_Set+0x3e>
3000b756:	3b01      	subs	r3, #1
3000b758:	d027      	beq.n	3000b7aa <SWR_MEM_Mode_Set+0x8e>
3000b75a:	f8d1 20c0 	ldr.w	r2, [r1, #192]	; 0xc0
3000b75e:	0352      	lsls	r2, r2, #13
3000b760:	d4f9      	bmi.n	3000b756 <SWR_MEM_Mode_Set+0x3a>
3000b762:	2000      	movs	r0, #0
3000b764:	e7e4      	b.n	3000b730 <SWR_MEM_Mode_Set+0x14>
3000b766:	f8d4 30d4 	ldr.w	r3, [r4, #212]	; 0xd4
3000b76a:	2064      	movs	r0, #100	; 0x64
3000b76c:	4d12      	ldr	r5, [pc, #72]	; (3000b7b8 <SWR_MEM_Mode_Set+0x9c>)
3000b76e:	f443 7380 	orr.w	r3, r3, #256	; 0x100
3000b772:	f8c4 30d4 	str.w	r3, [r4, #212]	; 0xd4
3000b776:	47a8      	blx	r5
3000b778:	f8d4 30d0 	ldr.w	r3, [r4, #208]	; 0xd0
3000b77c:	2064      	movs	r0, #100	; 0x64
3000b77e:	f043 0340 	orr.w	r3, r3, #64	; 0x40
3000b782:	f8c4 30d0 	str.w	r3, [r4, #208]	; 0xd0
3000b786:	47a8      	blx	r5
3000b788:	f8d4 20d4 	ldr.w	r2, [r4, #212]	; 0xd4
3000b78c:	f242 7310 	movw	r3, #10000	; 0x2710
3000b790:	f422 7280 	bic.w	r2, r2, #256	; 0x100
3000b794:	f8c4 20d4 	str.w	r2, [r4, #212]	; 0xd4
3000b798:	e001      	b.n	3000b79e <SWR_MEM_Mode_Set+0x82>
3000b79a:	3b01      	subs	r3, #1
3000b79c:	d005      	beq.n	3000b7aa <SWR_MEM_Mode_Set+0x8e>
3000b79e:	f8d4 20c0 	ldr.w	r2, [r4, #192]	; 0xc0
3000b7a2:	0355      	lsls	r5, r2, #13
3000b7a4:	d5f9      	bpl.n	3000b79a <SWR_MEM_Mode_Set+0x7e>
3000b7a6:	2000      	movs	r0, #0
3000b7a8:	e7c2      	b.n	3000b730 <SWR_MEM_Mode_Set+0x14>
3000b7aa:	2001      	movs	r0, #1
3000b7ac:	bd38      	pop	{r3, r4, r5, pc}
3000b7ae:	bf00      	nop
3000b7b0:	42008800 	.word	0x42008800
3000b7b4:	3000d910 	.word	0x3000d910
3000b7b8:	00009b2d 	.word	0x00009b2d

3000b7bc <SWR_AUDIO>:
3000b7bc:	2801      	cmp	r0, #1
3000b7be:	d00e      	beq.n	3000b7de <SWR_AUDIO+0x22>
3000b7c0:	4b1e      	ldr	r3, [pc, #120]	; (3000b83c <SWR_AUDIO+0x80>)
3000b7c2:	2000      	movs	r0, #0
3000b7c4:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b7c8:	f022 0201 	bic.w	r2, r2, #1
3000b7cc:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b7d0:	f8d3 2080 	ldr.w	r2, [r3, #128]	; 0x80
3000b7d4:	f022 0202 	bic.w	r2, r2, #2
3000b7d8:	f8c3 2080 	str.w	r2, [r3, #128]	; 0x80
3000b7dc:	4770      	bx	lr
3000b7de:	b510      	push	{r4, lr}
3000b7e0:	4c16      	ldr	r4, [pc, #88]	; (3000b83c <SWR_AUDIO+0x80>)
3000b7e2:	4b17      	ldr	r3, [pc, #92]	; (3000b840 <SWR_AUDIO+0x84>)
3000b7e4:	4798      	blx	r3
3000b7e6:	f8d4 30a4 	ldr.w	r3, [r4, #164]	; 0xa4
3000b7ea:	4a16      	ldr	r2, [pc, #88]	; (3000b844 <SWR_AUDIO+0x88>)
3000b7ec:	f44f 7096 	mov.w	r0, #300	; 0x12c
3000b7f0:	f423 5340 	bic.w	r3, r3, #12288	; 0x3000
3000b7f4:	f443 5380 	orr.w	r3, r3, #4096	; 0x1000
3000b7f8:	f8c4 30a4 	str.w	r3, [r4, #164]	; 0xa4
3000b7fc:	f8d4 3090 	ldr.w	r3, [r4, #144]	; 0x90
3000b800:	f023 0330 	bic.w	r3, r3, #48	; 0x30
3000b804:	f8c4 3090 	str.w	r3, [r4, #144]	; 0x90
3000b808:	f8d4 3080 	ldr.w	r3, [r4, #128]	; 0x80
3000b80c:	f043 0302 	orr.w	r3, r3, #2
3000b810:	f8c4 3080 	str.w	r3, [r4, #128]	; 0x80
3000b814:	4790      	blx	r2
3000b816:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b81a:	4b0b      	ldr	r3, [pc, #44]	; (3000b848 <SWR_AUDIO+0x8c>)
3000b81c:	f042 0201 	orr.w	r2, r2, #1
3000b820:	f8c4 2080 	str.w	r2, [r4, #128]	; 0x80
3000b824:	e001      	b.n	3000b82a <SWR_AUDIO+0x6e>
3000b826:	3b01      	subs	r3, #1
3000b828:	d005      	beq.n	3000b836 <SWR_AUDIO+0x7a>
3000b82a:	f8d4 2080 	ldr.w	r2, [r4, #128]	; 0x80
3000b82e:	03d2      	lsls	r2, r2, #15
3000b830:	d5f9      	bpl.n	3000b826 <SWR_AUDIO+0x6a>
3000b832:	2000      	movs	r0, #0
3000b834:	bd10      	pop	{r4, pc}
3000b836:	2001      	movs	r0, #1
3000b838:	bd10      	pop	{r4, pc}
3000b83a:	bf00      	nop
3000b83c:	42008800 	.word	0x42008800
3000b840:	3000b4e9 	.word	0x3000b4e9
3000b844:	00009b2d 	.word	0x00009b2d
3000b848:	000186a0 	.word	0x000186a0

3000b84c <SWR_AUDIO_Manual>:
3000b84c:	4a06      	ldr	r2, [pc, #24]	; (3000b868 <SWR_AUDIO_Manual+0x1c>)
3000b84e:	f8d2 3080 	ldr.w	r3, [r2, #128]	; 0x80
3000b852:	b120      	cbz	r0, 3000b85e <SWR_AUDIO_Manual+0x12>
3000b854:	f443 2300 	orr.w	r3, r3, #524288	; 0x80000
3000b858:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b85c:	4770      	bx	lr
3000b85e:	f423 2300 	bic.w	r3, r3, #524288	; 0x80000
3000b862:	f8c2 3080 	str.w	r3, [r2, #128]	; 0x80
3000b866:	4770      	bx	lr
3000b868:	42008800 	.word	0x42008800

3000b86c <SWR_BST_MODE_Set>:
3000b86c:	4a06      	ldr	r2, [pc, #24]	; (3000b888 <SWR_BST_MODE_Set+0x1c>)
3000b86e:	6813      	ldr	r3, [r2, #0]
3000b870:	b118      	cbz	r0, 3000b87a <SWR_BST_MODE_Set+0xe>
3000b872:	f443 1340 	orr.w	r3, r3, #3145728	; 0x300000
3000b876:	6013      	str	r3, [r2, #0]
3000b878:	4770      	bx	lr
3000b87a:	f423 1340 	bic.w	r3, r3, #3145728	; 0x300000
3000b87e:	f443 1380 	orr.w	r3, r3, #1048576	; 0x100000
3000b882:	6013      	str	r3, [r2, #0]
3000b884:	4770      	bx	lr
3000b886:	bf00      	nop
3000b888:	42008100 	.word	0x42008100

3000b88c <crash_dump_memory>:
3000b88c:	e92d 47f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b890:	460e      	mov	r6, r1
3000b892:	4680      	mov	r8, r0
3000b894:	4b21      	ldr	r3, [pc, #132]	; (3000b91c <crash_dump_memory+0x90>)
3000b896:	f1a6 0080 	sub.w	r0, r6, #128	; 0x80
3000b89a:	4a21      	ldr	r2, [pc, #132]	; (3000b920 <crash_dump_memory+0x94>)
3000b89c:	f8d3 127c 	ldr.w	r1, [r3, #636]	; 0x27c
3000b8a0:	4644      	mov	r4, r8
3000b8a2:	f5b0 5f7e 	cmp.w	r0, #16256	; 0x3f80
3000b8a6:	4b1f      	ldr	r3, [pc, #124]	; (3000b924 <crash_dump_memory+0x98>)
3000b8a8:	481f      	ldr	r0, [pc, #124]	; (3000b928 <crash_dump_memory+0x9c>)
3000b8aa:	bf88      	it	hi
3000b8ac:	2680      	movhi	r6, #128	; 0x80
3000b8ae:	f011 0fc0 	tst.w	r1, #192	; 0xc0
3000b8b2:	4f1e      	ldr	r7, [pc, #120]	; (3000b92c <crash_dump_memory+0xa0>)
3000b8b4:	bf14      	ite	ne
3000b8b6:	4611      	movne	r1, r2
3000b8b8:	4619      	moveq	r1, r3
3000b8ba:	f000 fd8d 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b8be:	4632      	mov	r2, r6
3000b8c0:	4641      	mov	r1, r8
3000b8c2:	481b      	ldr	r0, [pc, #108]	; (3000b930 <crash_dump_memory+0xa4>)
3000b8c4:	f000 fd88 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b8c8:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000b938 <crash_dump_memory+0xac>
3000b8cc:	2300      	movs	r3, #0
3000b8ce:	f8df a06c 	ldr.w	sl, [pc, #108]	; 3000b93c <crash_dump_memory+0xb0>
3000b8d2:	e007      	b.n	3000b8e4 <crash_dump_memory+0x58>
3000b8d4:	6821      	ldr	r1, [r4, #0]
3000b8d6:	4638      	mov	r0, r7
3000b8d8:	f000 fd7e 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b8dc:	42ae      	cmp	r6, r5
3000b8de:	d00f      	beq.n	3000b900 <crash_dump_memory+0x74>
3000b8e0:	3404      	adds	r4, #4
3000b8e2:	462b      	mov	r3, r5
3000b8e4:	1c5d      	adds	r5, r3, #1
3000b8e6:	b183      	cbz	r3, 3000b90a <crash_dump_memory+0x7e>
3000b8e8:	079b      	lsls	r3, r3, #30
3000b8ea:	d1f3      	bne.n	3000b8d4 <crash_dump_memory+0x48>
3000b8ec:	4621      	mov	r1, r4
3000b8ee:	4648      	mov	r0, r9
3000b8f0:	f000 fd72 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b8f4:	6821      	ldr	r1, [r4, #0]
3000b8f6:	4638      	mov	r0, r7
3000b8f8:	f000 fd6e 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b8fc:	42ae      	cmp	r6, r5
3000b8fe:	d1ef      	bne.n	3000b8e0 <crash_dump_memory+0x54>
3000b900:	480c      	ldr	r0, [pc, #48]	; (3000b934 <crash_dump_memory+0xa8>)
3000b902:	e8bd 47f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, lr}
3000b906:	f000 bd67 	b.w	3000c3d8 <__DiagPrintf_veneer>
3000b90a:	4641      	mov	r1, r8
3000b90c:	4650      	mov	r0, sl
3000b90e:	f000 fd63 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b912:	6821      	ldr	r1, [r4, #0]
3000b914:	4638      	mov	r0, r7
3000b916:	f000 fd5f 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b91a:	e7e1      	b.n	3000b8e0 <crash_dump_memory+0x54>
3000b91c:	42008000 	.word	0x42008000
3000b920:	3000d924 	.word	0x3000d924
3000b924:	3000d928 	.word	0x3000d928
3000b928:	3000d92c 	.word	0x3000d92c
3000b92c:	3000d994 	.word	0x3000d994
3000b930:	3000d950 	.word	0x3000d950
3000b934:	3000cdf4 	.word	0x3000cdf4
3000b938:	3000d99c 	.word	0x3000d99c
3000b93c:	3000d98c 	.word	0x3000d98c

3000b940 <crash_dump>:
3000b940:	e92d 4ff8 	stmdb	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000b944:	4c4f      	ldr	r4, [pc, #316]	; (3000ba84 <crash_dump+0x144>)
3000b946:	4615      	mov	r5, r2
3000b948:	4688      	mov	r8, r1
3000b94a:	f04f 23e0 	mov.w	r3, #3758153728	; 0xe000e000
3000b94e:	484e      	ldr	r0, [pc, #312]	; (3000ba88 <crash_dump+0x148>)
3000b950:	462f      	mov	r7, r5
3000b952:	f8df b160 	ldr.w	fp, [pc, #352]	; 3000bab4 <crash_dump+0x174>
3000b956:	f105 0930 	add.w	r9, r5, #48	; 0x30
3000b95a:	f8df a164 	ldr.w	sl, [pc, #356]	; 3000bac0 <crash_dump+0x180>
3000b95e:	e9d2 2109 	ldrd	r2, r1, [r2, #36]	; 0x24
3000b962:	e9c4 2100 	strd	r2, r1, [r4]
3000b966:	6aea      	ldr	r2, [r5, #44]	; 0x2c
3000b968:	60a2      	str	r2, [r4, #8]
3000b96a:	6b2a      	ldr	r2, [r5, #48]	; 0x30
3000b96c:	60e2      	str	r2, [r4, #12]
3000b96e:	686a      	ldr	r2, [r5, #4]
3000b970:	6122      	str	r2, [r4, #16]
3000b972:	68aa      	ldr	r2, [r5, #8]
3000b974:	6162      	str	r2, [r4, #20]
3000b976:	68ea      	ldr	r2, [r5, #12]
3000b978:	61a2      	str	r2, [r4, #24]
3000b97a:	692a      	ldr	r2, [r5, #16]
3000b97c:	61e2      	str	r2, [r4, #28]
3000b97e:	696a      	ldr	r2, [r5, #20]
3000b980:	6222      	str	r2, [r4, #32]
3000b982:	69aa      	ldr	r2, [r5, #24]
3000b984:	6262      	str	r2, [r4, #36]	; 0x24
3000b986:	69ea      	ldr	r2, [r5, #28]
3000b988:	62a2      	str	r2, [r4, #40]	; 0x28
3000b98a:	6a2a      	ldr	r2, [r5, #32]
3000b98c:	62e2      	str	r2, [r4, #44]	; 0x2c
3000b98e:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000b990:	6322      	str	r2, [r4, #48]	; 0x30
3000b992:	6baa      	ldr	r2, [r5, #56]	; 0x38
3000b994:	6362      	str	r2, [r4, #52]	; 0x34
3000b996:	6bea      	ldr	r2, [r5, #60]	; 0x3c
3000b998:	63a2      	str	r2, [r4, #56]	; 0x38
3000b99a:	6c2a      	ldr	r2, [r5, #64]	; 0x40
3000b99c:	63e2      	str	r2, [r4, #60]	; 0x3c
3000b99e:	682a      	ldr	r2, [r5, #0]
3000b9a0:	6422      	str	r2, [r4, #64]	; 0x40
3000b9a2:	f8d3 2d24 	ldr.w	r2, [r3, #3364]	; 0xd24
3000b9a6:	6462      	str	r2, [r4, #68]	; 0x44
3000b9a8:	4a38      	ldr	r2, [pc, #224]	; (3000ba8c <crash_dump+0x14c>)
3000b9aa:	f892 1028 	ldrb.w	r1, [r2, #40]	; 0x28
3000b9ae:	f884 1048 	strb.w	r1, [r4, #72]	; 0x48
3000b9b2:	f8d3 1d34 	ldr.w	r1, [r3, #3380]	; 0xd34
3000b9b6:	64e1      	str	r1, [r4, #76]	; 0x4c
3000b9b8:	f892 1029 	ldrb.w	r1, [r2, #41]	; 0x29
3000b9bc:	f884 1050 	strb.w	r1, [r4, #80]	; 0x50
3000b9c0:	f8d3 1d38 	ldr.w	r1, [r3, #3384]	; 0xd38
3000b9c4:	6561      	str	r1, [r4, #84]	; 0x54
3000b9c6:	8d51      	ldrh	r1, [r2, #42]	; 0x2a
3000b9c8:	f8a4 1058 	strh.w	r1, [r4, #88]	; 0x58
3000b9cc:	f8d3 1d2c 	ldr.w	r1, [r3, #3372]	; 0xd2c
3000b9d0:	65e1      	str	r1, [r4, #92]	; 0x5c
3000b9d2:	8e11      	ldrh	r1, [r2, #48]	; 0x30
3000b9d4:	8f92      	ldrh	r2, [r2, #60]	; 0x3c
3000b9d6:	b289      	uxth	r1, r1
3000b9d8:	b292      	uxth	r2, r2
3000b9da:	6621      	str	r1, [r4, #96]	; 0x60
3000b9dc:	6662      	str	r2, [r4, #100]	; 0x64
3000b9de:	f8d3 2de4 	ldr.w	r2, [r3, #3556]	; 0xde4
3000b9e2:	66a2      	str	r2, [r4, #104]	; 0x68
3000b9e4:	f8d3 3de8 	ldr.w	r3, [r3, #3560]	; 0xde8
3000b9e8:	66e3      	str	r3, [r4, #108]	; 0x6c
3000b9ea:	f000 fcf5 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b9ee:	4620      	mov	r0, r4
3000b9f0:	462c      	mov	r4, r5
3000b9f2:	f000 f867 	bl	3000bac4 <fault_diagnosis>
3000b9f6:	4826      	ldr	r0, [pc, #152]	; (3000ba90 <crash_dump+0x150>)
3000b9f8:	f000 fcee 	bl	3000c3d8 <__DiagPrintf_veneer>
3000b9fc:	6ba9      	ldr	r1, [r5, #56]	; 0x38
3000b9fe:	4825      	ldr	r0, [pc, #148]	; (3000ba94 <crash_dump+0x154>)
3000ba00:	f000 fcea 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba04:	6be9      	ldr	r1, [r5, #60]	; 0x3c
3000ba06:	4824      	ldr	r0, [pc, #144]	; (3000ba98 <crash_dump+0x158>)
3000ba08:	f000 fce6 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba0c:	6c29      	ldr	r1, [r5, #64]	; 0x40
3000ba0e:	4823      	ldr	r0, [pc, #140]	; (3000ba9c <crash_dump+0x15c>)
3000ba10:	f000 fce2 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba14:	f857 1b20 	ldr.w	r1, [r7], #32
3000ba18:	4821      	ldr	r0, [pc, #132]	; (3000baa0 <crash_dump+0x160>)
3000ba1a:	463e      	mov	r6, r7
3000ba1c:	f000 fcdc 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba20:	4920      	ldr	r1, [pc, #128]	; (3000baa4 <crash_dump+0x164>)
3000ba22:	4658      	mov	r0, fp
3000ba24:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ba28:	f000 fcd6 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba2c:	454e      	cmp	r6, r9
3000ba2e:	d008      	beq.n	3000ba42 <crash_dump+0x102>
3000ba30:	f856 2f04 	ldr.w	r2, [r6, #4]!
3000ba34:	4658      	mov	r0, fp
3000ba36:	f85a 1b04 	ldr.w	r1, [sl], #4
3000ba3a:	f000 fccd 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba3e:	454e      	cmp	r6, r9
3000ba40:	d1f6      	bne.n	3000ba30 <crash_dump+0xf0>
3000ba42:	4e19      	ldr	r6, [pc, #100]	; (3000baa8 <crash_dump+0x168>)
3000ba44:	4919      	ldr	r1, [pc, #100]	; (3000baac <crash_dump+0x16c>)
3000ba46:	f8df 906c 	ldr.w	r9, [pc, #108]	; 3000bab4 <crash_dump+0x174>
3000ba4a:	e001      	b.n	3000ba50 <crash_dump+0x110>
3000ba4c:	f856 1b04 	ldr.w	r1, [r6], #4
3000ba50:	f854 2f04 	ldr.w	r2, [r4, #4]!
3000ba54:	4648      	mov	r0, r9
3000ba56:	f000 fcbf 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba5a:	42bc      	cmp	r4, r7
3000ba5c:	d1f6      	bne.n	3000ba4c <crash_dump+0x10c>
3000ba5e:	6b6a      	ldr	r2, [r5, #52]	; 0x34
3000ba60:	4913      	ldr	r1, [pc, #76]	; (3000bab0 <crash_dump+0x170>)
3000ba62:	4814      	ldr	r0, [pc, #80]	; (3000bab4 <crash_dump+0x174>)
3000ba64:	f000 fcb8 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba68:	2180      	movs	r1, #128	; 0x80
3000ba6a:	4640      	mov	r0, r8
3000ba6c:	f7ff ff0e 	bl	3000b88c <crash_dump_memory>
3000ba70:	4811      	ldr	r0, [pc, #68]	; (3000bab8 <crash_dump+0x178>)
3000ba72:	f000 fcb1 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba76:	4811      	ldr	r0, [pc, #68]	; (3000babc <crash_dump+0x17c>)
3000ba78:	f000 fcae 	bl	3000c3d8 <__DiagPrintf_veneer>
3000ba7c:	2000      	movs	r0, #0
3000ba7e:	e8bd 8ff8 	ldmia.w	sp!, {r3, r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000ba82:	bf00      	nop
3000ba84:	3000fc88 	.word	0x3000fc88
3000ba88:	3000d9b0 	.word	0x3000d9b0
3000ba8c:	e000ed00 	.word	0xe000ed00
3000ba90:	3000d9d4 	.word	0x3000d9d4
3000ba94:	3000d9fc 	.word	0x3000d9fc
3000ba98:	3000da0c 	.word	0x3000da0c
3000ba9c:	3000da1c 	.word	0x3000da1c
3000baa0:	3000da2c 	.word	0x3000da2c
3000baa4:	3000d9a8 	.word	0x3000d9a8
3000baa8:	3000da8c 	.word	0x3000da8c
3000baac:	3000d9ac 	.word	0x3000d9ac
3000bab0:	3000da4c 	.word	0x3000da4c
3000bab4:	3000da3c 	.word	0x3000da3c
3000bab8:	3000da50 	.word	0x3000da50
3000babc:	3000da7c 	.word	0x3000da7c
3000bac0:	3000daac 	.word	0x3000daac

3000bac4 <fault_diagnosis>:
3000bac4:	b510      	push	{r4, lr}
3000bac6:	4604      	mov	r4, r0
3000bac8:	4888      	ldr	r0, [pc, #544]	; (3000bcec <fault_diagnosis+0x228>)
3000baca:	6ba1      	ldr	r1, [r4, #56]	; 0x38
3000bacc:	f000 fc84 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bad0:	f894 305c 	ldrb.w	r3, [r4, #92]	; 0x5c
3000bad4:	079b      	lsls	r3, r3, #30
3000bad6:	f100 808a 	bmi.w	3000bbee <fault_diagnosis+0x12a>
3000bada:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000badc:	b1e1      	cbz	r1, 3000bb18 <fault_diagnosis+0x54>
3000bade:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bae2:	07d8      	lsls	r0, r3, #31
3000bae4:	f100 8094 	bmi.w	3000bc10 <fault_diagnosis+0x14c>
3000bae8:	079a      	lsls	r2, r3, #30
3000baea:	f100 809a 	bmi.w	3000bc22 <fault_diagnosis+0x15e>
3000baee:	0758      	lsls	r0, r3, #29
3000baf0:	f100 80a1 	bmi.w	3000bc36 <fault_diagnosis+0x172>
3000baf4:	0719      	lsls	r1, r3, #28
3000baf6:	f100 80a8 	bmi.w	3000bc4a <fault_diagnosis+0x186>
3000bafa:	06da      	lsls	r2, r3, #27
3000bafc:	f100 80af 	bmi.w	3000bc5e <fault_diagnosis+0x19a>
3000bb00:	0698      	lsls	r0, r3, #26
3000bb02:	f100 80b6 	bmi.w	3000bc72 <fault_diagnosis+0x1ae>
3000bb06:	0619      	lsls	r1, r3, #24
3000bb08:	f100 80bd 	bmi.w	3000bc86 <fault_diagnosis+0x1c2>
3000bb0c:	065a      	lsls	r2, r3, #25
3000bb0e:	d503      	bpl.n	3000bb18 <fault_diagnosis+0x54>
3000bb10:	f013 0f28 	tst.w	r3, #40	; 0x28
3000bb14:	f040 8186 	bne.w	3000be24 <fault_diagnosis+0x360>
3000bb18:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bb1c:	b199      	cbz	r1, 3000bb46 <fault_diagnosis+0x82>
3000bb1e:	07cb      	lsls	r3, r1, #31
3000bb20:	f100 80b9 	bmi.w	3000bc96 <fault_diagnosis+0x1d2>
3000bb24:	0788      	lsls	r0, r1, #30
3000bb26:	f100 80bf 	bmi.w	3000bca8 <fault_diagnosis+0x1e4>
3000bb2a:	070a      	lsls	r2, r1, #28
3000bb2c:	f100 80c5 	bmi.w	3000bcba <fault_diagnosis+0x1f6>
3000bb30:	06cb      	lsls	r3, r1, #27
3000bb32:	f100 80cb 	bmi.w	3000bccc <fault_diagnosis+0x208>
3000bb36:	0688      	lsls	r0, r1, #26
3000bb38:	f100 80d1 	bmi.w	3000bcde <fault_diagnosis+0x21a>
3000bb3c:	060a      	lsls	r2, r1, #24
3000bb3e:	d502      	bpl.n	3000bb46 <fault_diagnosis+0x82>
3000bb40:	078b      	lsls	r3, r1, #30
3000bb42:	f040 8194 	bne.w	3000be6e <fault_diagnosis+0x3aa>
3000bb46:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bb4a:	b1b1      	cbz	r1, 3000bb7a <fault_diagnosis+0xb6>
3000bb4c:	07c8      	lsls	r0, r1, #31
3000bb4e:	f100 80f7 	bmi.w	3000bd40 <fault_diagnosis+0x27c>
3000bb52:	078a      	lsls	r2, r1, #30
3000bb54:	f100 80fd 	bmi.w	3000bd52 <fault_diagnosis+0x28e>
3000bb58:	074b      	lsls	r3, r1, #29
3000bb5a:	f100 8103 	bmi.w	3000bd64 <fault_diagnosis+0x2a0>
3000bb5e:	0708      	lsls	r0, r1, #28
3000bb60:	f100 8109 	bmi.w	3000bd76 <fault_diagnosis+0x2b2>
3000bb64:	06ca      	lsls	r2, r1, #27
3000bb66:	f100 810f 	bmi.w	3000bd88 <fault_diagnosis+0x2c4>
3000bb6a:	068b      	lsls	r3, r1, #26
3000bb6c:	f100 8115 	bmi.w	3000bd9a <fault_diagnosis+0x2d6>
3000bb70:	0608      	lsls	r0, r1, #24
3000bb72:	d502      	bpl.n	3000bb7a <fault_diagnosis+0xb6>
3000bb74:	0789      	lsls	r1, r1, #30
3000bb76:	f100 8180 	bmi.w	3000be7a <fault_diagnosis+0x3b6>
3000bb7a:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bb7e:	b1b9      	cbz	r1, 3000bbb0 <fault_diagnosis+0xec>
3000bb80:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bb84:	07da      	lsls	r2, r3, #31
3000bb86:	f100 810f 	bmi.w	3000bda8 <fault_diagnosis+0x2e4>
3000bb8a:	0798      	lsls	r0, r3, #30
3000bb8c:	f100 8115 	bmi.w	3000bdba <fault_diagnosis+0x2f6>
3000bb90:	0759      	lsls	r1, r3, #29
3000bb92:	f100 811d 	bmi.w	3000bdd0 <fault_diagnosis+0x30c>
3000bb96:	071a      	lsls	r2, r3, #28
3000bb98:	f100 8125 	bmi.w	3000bde6 <fault_diagnosis+0x322>
3000bb9c:	06db      	lsls	r3, r3, #27
3000bb9e:	f100 812d 	bmi.w	3000bdfc <fault_diagnosis+0x338>
3000bba2:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000bba6:	07d8      	lsls	r0, r3, #31
3000bba8:	f100 8133 	bmi.w	3000be12 <fault_diagnosis+0x34e>
3000bbac:	0799      	lsls	r1, r3, #30
3000bbae:	d424      	bmi.n	3000bbfa <fault_diagnosis+0x136>
3000bbb0:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bbb4:	2b00      	cmp	r3, #0
3000bbb6:	db00      	blt.n	3000bbba <fault_diagnosis+0xf6>
3000bbb8:	bd10      	pop	{r4, pc}
3000bbba:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bbbc:	2900      	cmp	r1, #0
3000bbbe:	d0fb      	beq.n	3000bbb8 <fault_diagnosis+0xf4>
3000bbc0:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000bbc4:	07da      	lsls	r2, r3, #31
3000bbc6:	f100 8143 	bmi.w	3000be50 <fault_diagnosis+0x38c>
3000bbca:	0798      	lsls	r0, r3, #30
3000bbcc:	f100 8138 	bmi.w	3000be40 <fault_diagnosis+0x37c>
3000bbd0:	0759      	lsls	r1, r3, #29
3000bbd2:	f100 812d 	bmi.w	3000be30 <fault_diagnosis+0x36c>
3000bbd6:	071a      	lsls	r2, r3, #28
3000bbd8:	f100 8141 	bmi.w	3000be5e <fault_diagnosis+0x39a>
3000bbdc:	06db      	lsls	r3, r3, #27
3000bbde:	d5eb      	bpl.n	3000bbb8 <fault_diagnosis+0xf4>
3000bbe0:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000bbe2:	4a43      	ldr	r2, [pc, #268]	; (3000bcf0 <fault_diagnosis+0x22c>)
3000bbe4:	4843      	ldr	r0, [pc, #268]	; (3000bcf4 <fault_diagnosis+0x230>)
3000bbe6:	e8bd 4010 	ldmia.w	sp!, {r4, lr}
3000bbea:	f000 bbf5 	b.w	3000c3d8 <__DiagPrintf_veneer>
3000bbee:	4a42      	ldr	r2, [pc, #264]	; (3000bcf8 <fault_diagnosis+0x234>)
3000bbf0:	6de1      	ldr	r1, [r4, #92]	; 0x5c
3000bbf2:	4842      	ldr	r0, [pc, #264]	; (3000bcfc <fault_diagnosis+0x238>)
3000bbf4:	f000 fbf0 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bbf8:	e76f      	b.n	3000bada <fault_diagnosis+0x16>
3000bbfa:	4a41      	ldr	r2, [pc, #260]	; (3000bd00 <fault_diagnosis+0x23c>)
3000bbfc:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bc00:	4840      	ldr	r0, [pc, #256]	; (3000bd04 <fault_diagnosis+0x240>)
3000bc02:	f000 fbe9 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc06:	f994 305f 	ldrsb.w	r3, [r4, #95]	; 0x5f
3000bc0a:	2b00      	cmp	r3, #0
3000bc0c:	dad4      	bge.n	3000bbb8 <fault_diagnosis+0xf4>
3000bc0e:	e7d4      	b.n	3000bbba <fault_diagnosis+0xf6>
3000bc10:	4a3d      	ldr	r2, [pc, #244]	; (3000bd08 <fault_diagnosis+0x244>)
3000bc12:	483e      	ldr	r0, [pc, #248]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc14:	f000 fbe0 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc18:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc1c:	079a      	lsls	r2, r3, #30
3000bc1e:	f57f af66 	bpl.w	3000baee <fault_diagnosis+0x2a>
3000bc22:	4a3b      	ldr	r2, [pc, #236]	; (3000bd10 <fault_diagnosis+0x24c>)
3000bc24:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc26:	4839      	ldr	r0, [pc, #228]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc28:	f000 fbd6 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc2c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc30:	0758      	lsls	r0, r3, #29
3000bc32:	f57f af5f 	bpl.w	3000baf4 <fault_diagnosis+0x30>
3000bc36:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc38:	4a36      	ldr	r2, [pc, #216]	; (3000bd14 <fault_diagnosis+0x250>)
3000bc3a:	4834      	ldr	r0, [pc, #208]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc3c:	f000 fbcc 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc40:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc44:	0719      	lsls	r1, r3, #28
3000bc46:	f57f af58 	bpl.w	3000bafa <fault_diagnosis+0x36>
3000bc4a:	4a33      	ldr	r2, [pc, #204]	; (3000bd18 <fault_diagnosis+0x254>)
3000bc4c:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc4e:	482f      	ldr	r0, [pc, #188]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc50:	f000 fbc2 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc54:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc58:	06da      	lsls	r2, r3, #27
3000bc5a:	f57f af51 	bpl.w	3000bb00 <fault_diagnosis+0x3c>
3000bc5e:	4a2f      	ldr	r2, [pc, #188]	; (3000bd1c <fault_diagnosis+0x258>)
3000bc60:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc62:	482a      	ldr	r0, [pc, #168]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc64:	f000 fbb8 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc68:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc6c:	0698      	lsls	r0, r3, #26
3000bc6e:	f57f af4a 	bpl.w	3000bb06 <fault_diagnosis+0x42>
3000bc72:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc74:	4a2a      	ldr	r2, [pc, #168]	; (3000bd20 <fault_diagnosis+0x25c>)
3000bc76:	4825      	ldr	r0, [pc, #148]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc78:	f000 fbae 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc7c:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc80:	0619      	lsls	r1, r3, #24
3000bc82:	f57f af43 	bpl.w	3000bb0c <fault_diagnosis+0x48>
3000bc86:	4a27      	ldr	r2, [pc, #156]	; (3000bd24 <fault_diagnosis+0x260>)
3000bc88:	6ea1      	ldr	r1, [r4, #104]	; 0x68
3000bc8a:	4820      	ldr	r0, [pc, #128]	; (3000bd0c <fault_diagnosis+0x248>)
3000bc8c:	f000 fba4 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc90:	f894 3068 	ldrb.w	r3, [r4, #104]	; 0x68
3000bc94:	e73a      	b.n	3000bb0c <fault_diagnosis+0x48>
3000bc96:	4a24      	ldr	r2, [pc, #144]	; (3000bd28 <fault_diagnosis+0x264>)
3000bc98:	4824      	ldr	r0, [pc, #144]	; (3000bd2c <fault_diagnosis+0x268>)
3000bc9a:	f000 fb9d 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bc9e:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bca2:	0788      	lsls	r0, r1, #30
3000bca4:	f57f af41 	bpl.w	3000bb2a <fault_diagnosis+0x66>
3000bca8:	4a21      	ldr	r2, [pc, #132]	; (3000bd30 <fault_diagnosis+0x26c>)
3000bcaa:	4820      	ldr	r0, [pc, #128]	; (3000bd2c <fault_diagnosis+0x268>)
3000bcac:	f000 fb94 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bcb0:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bcb4:	070a      	lsls	r2, r1, #28
3000bcb6:	f57f af3b 	bpl.w	3000bb30 <fault_diagnosis+0x6c>
3000bcba:	4a1e      	ldr	r2, [pc, #120]	; (3000bd34 <fault_diagnosis+0x270>)
3000bcbc:	481b      	ldr	r0, [pc, #108]	; (3000bd2c <fault_diagnosis+0x268>)
3000bcbe:	f000 fb8b 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bcc2:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bcc6:	06cb      	lsls	r3, r1, #27
3000bcc8:	f57f af35 	bpl.w	3000bb36 <fault_diagnosis+0x72>
3000bccc:	4a1a      	ldr	r2, [pc, #104]	; (3000bd38 <fault_diagnosis+0x274>)
3000bcce:	4817      	ldr	r0, [pc, #92]	; (3000bd2c <fault_diagnosis+0x268>)
3000bcd0:	f000 fb82 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bcd4:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bcd8:	0688      	lsls	r0, r1, #26
3000bcda:	f57f af2f 	bpl.w	3000bb3c <fault_diagnosis+0x78>
3000bcde:	4a17      	ldr	r2, [pc, #92]	; (3000bd3c <fault_diagnosis+0x278>)
3000bce0:	4812      	ldr	r0, [pc, #72]	; (3000bd2c <fault_diagnosis+0x268>)
3000bce2:	f000 fb79 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bce6:	f894 1048 	ldrb.w	r1, [r4, #72]	; 0x48
3000bcea:	e727      	b.n	3000bb3c <fault_diagnosis+0x78>
3000bcec:	3000dabc 	.word	0x3000dabc
3000bcf0:	3000e34c 	.word	0x3000e34c
3000bcf4:	3000e2a4 	.word	0x3000e2a4
3000bcf8:	3000dad4 	.word	0x3000dad4
3000bcfc:	3000db00 	.word	0x3000db00
3000bd00:	3000e208 	.word	0x3000e208
3000bd04:	3000e04c 	.word	0x3000e04c
3000bd08:	3000db18 	.word	0x3000db18
3000bd0c:	3000db48 	.word	0x3000db48
3000bd10:	3000db60 	.word	0x3000db60
3000bd14:	3000db98 	.word	0x3000db98
3000bd18:	3000dbd0 	.word	0x3000dbd0
3000bd1c:	3000dc08 	.word	0x3000dc08
3000bd20:	3000dc38 	.word	0x3000dc38
3000bd24:	3000dc74 	.word	0x3000dc74
3000bd28:	3000dce4 	.word	0x3000dce4
3000bd2c:	3000dd28 	.word	0x3000dd28
3000bd30:	3000dd40 	.word	0x3000dd40
3000bd34:	3000dd7c 	.word	0x3000dd7c
3000bd38:	3000ddb4 	.word	0x3000ddb4
3000bd3c:	3000dde8 	.word	0x3000dde8
3000bd40:	4a51      	ldr	r2, [pc, #324]	; (3000be88 <fault_diagnosis+0x3c4>)
3000bd42:	4852      	ldr	r0, [pc, #328]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd44:	f000 fb48 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bd48:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd4c:	078a      	lsls	r2, r1, #30
3000bd4e:	f57f af03 	bpl.w	3000bb58 <fault_diagnosis+0x94>
3000bd52:	4a4f      	ldr	r2, [pc, #316]	; (3000be90 <fault_diagnosis+0x3cc>)
3000bd54:	484d      	ldr	r0, [pc, #308]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd56:	f000 fb3f 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bd5a:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd5e:	074b      	lsls	r3, r1, #29
3000bd60:	f57f aefd 	bpl.w	3000bb5e <fault_diagnosis+0x9a>
3000bd64:	4a4b      	ldr	r2, [pc, #300]	; (3000be94 <fault_diagnosis+0x3d0>)
3000bd66:	4849      	ldr	r0, [pc, #292]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd68:	f000 fb36 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bd6c:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd70:	0708      	lsls	r0, r1, #28
3000bd72:	f57f aef7 	bpl.w	3000bb64 <fault_diagnosis+0xa0>
3000bd76:	4a48      	ldr	r2, [pc, #288]	; (3000be98 <fault_diagnosis+0x3d4>)
3000bd78:	4844      	ldr	r0, [pc, #272]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd7a:	f000 fb2d 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bd7e:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd82:	06ca      	lsls	r2, r1, #27
3000bd84:	f57f aef1 	bpl.w	3000bb6a <fault_diagnosis+0xa6>
3000bd88:	4a44      	ldr	r2, [pc, #272]	; (3000be9c <fault_diagnosis+0x3d8>)
3000bd8a:	4840      	ldr	r0, [pc, #256]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd8c:	f000 fb24 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bd90:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bd94:	068b      	lsls	r3, r1, #26
3000bd96:	f57f aeeb 	bpl.w	3000bb70 <fault_diagnosis+0xac>
3000bd9a:	4a41      	ldr	r2, [pc, #260]	; (3000bea0 <fault_diagnosis+0x3dc>)
3000bd9c:	483b      	ldr	r0, [pc, #236]	; (3000be8c <fault_diagnosis+0x3c8>)
3000bd9e:	f000 fb1b 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bda2:	f894 1050 	ldrb.w	r1, [r4, #80]	; 0x50
3000bda6:	e6e3      	b.n	3000bb70 <fault_diagnosis+0xac>
3000bda8:	4a3e      	ldr	r2, [pc, #248]	; (3000bea4 <fault_diagnosis+0x3e0>)
3000bdaa:	483f      	ldr	r0, [pc, #252]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000bdac:	f000 fb14 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bdb0:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bdb4:	0798      	lsls	r0, r3, #30
3000bdb6:	f57f aeeb 	bpl.w	3000bb90 <fault_diagnosis+0xcc>
3000bdba:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bdbe:	4a3b      	ldr	r2, [pc, #236]	; (3000beac <fault_diagnosis+0x3e8>)
3000bdc0:	4839      	ldr	r0, [pc, #228]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000bdc2:	f000 fb09 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bdc6:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bdca:	0759      	lsls	r1, r3, #29
3000bdcc:	f57f aee3 	bpl.w	3000bb96 <fault_diagnosis+0xd2>
3000bdd0:	4a37      	ldr	r2, [pc, #220]	; (3000beb0 <fault_diagnosis+0x3ec>)
3000bdd2:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bdd6:	4834      	ldr	r0, [pc, #208]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000bdd8:	f000 fafe 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bddc:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bde0:	071a      	lsls	r2, r3, #28
3000bde2:	f57f aedb 	bpl.w	3000bb9c <fault_diagnosis+0xd8>
3000bde6:	4a33      	ldr	r2, [pc, #204]	; (3000beb4 <fault_diagnosis+0x3f0>)
3000bde8:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000bdec:	482e      	ldr	r0, [pc, #184]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000bdee:	f000 faf3 	bl	3000c3d8 <__DiagPrintf_veneer>
3000bdf2:	f894 3058 	ldrb.w	r3, [r4, #88]	; 0x58
3000bdf6:	06db      	lsls	r3, r3, #27
3000bdf8:	f57f aed3 	bpl.w	3000bba2 <fault_diagnosis+0xde>
3000bdfc:	4a2e      	ldr	r2, [pc, #184]	; (3000beb8 <fault_diagnosis+0x3f4>)
3000bdfe:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be02:	4829      	ldr	r0, [pc, #164]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000be04:	f000 fae8 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be08:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000be0c:	07d8      	lsls	r0, r3, #31
3000be0e:	f57f aecd 	bpl.w	3000bbac <fault_diagnosis+0xe8>
3000be12:	4a2a      	ldr	r2, [pc, #168]	; (3000bebc <fault_diagnosis+0x3f8>)
3000be14:	f8b4 1058 	ldrh.w	r1, [r4, #88]	; 0x58
3000be18:	4823      	ldr	r0, [pc, #140]	; (3000bea8 <fault_diagnosis+0x3e4>)
3000be1a:	f000 fadd 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be1e:	f894 3059 	ldrb.w	r3, [r4, #89]	; 0x59
3000be22:	e6c3      	b.n	3000bbac <fault_diagnosis+0xe8>
3000be24:	4a26      	ldr	r2, [pc, #152]	; (3000bec0 <fault_diagnosis+0x3fc>)
3000be26:	6ee1      	ldr	r1, [r4, #108]	; 0x6c
3000be28:	4826      	ldr	r0, [pc, #152]	; (3000bec4 <fault_diagnosis+0x400>)
3000be2a:	f000 fad5 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be2e:	e673      	b.n	3000bb18 <fault_diagnosis+0x54>
3000be30:	4a25      	ldr	r2, [pc, #148]	; (3000bec8 <fault_diagnosis+0x404>)
3000be32:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be34:	4825      	ldr	r0, [pc, #148]	; (3000becc <fault_diagnosis+0x408>)
3000be36:	f000 facf 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be3a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be3e:	e6ca      	b.n	3000bbd6 <fault_diagnosis+0x112>
3000be40:	4a23      	ldr	r2, [pc, #140]	; (3000bed0 <fault_diagnosis+0x40c>)
3000be42:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be44:	4821      	ldr	r0, [pc, #132]	; (3000becc <fault_diagnosis+0x408>)
3000be46:	f000 fac7 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be4a:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be4e:	e6bf      	b.n	3000bbd0 <fault_diagnosis+0x10c>
3000be50:	4a20      	ldr	r2, [pc, #128]	; (3000bed4 <fault_diagnosis+0x410>)
3000be52:	481e      	ldr	r0, [pc, #120]	; (3000becc <fault_diagnosis+0x408>)
3000be54:	f000 fac0 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be58:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be5c:	e6b5      	b.n	3000bbca <fault_diagnosis+0x106>
3000be5e:	4a1e      	ldr	r2, [pc, #120]	; (3000bed8 <fault_diagnosis+0x414>)
3000be60:	6e21      	ldr	r1, [r4, #96]	; 0x60
3000be62:	481a      	ldr	r0, [pc, #104]	; (3000becc <fault_diagnosis+0x408>)
3000be64:	f000 fab8 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be68:	f894 3060 	ldrb.w	r3, [r4, #96]	; 0x60
3000be6c:	e6b6      	b.n	3000bbdc <fault_diagnosis+0x118>
3000be6e:	4a1b      	ldr	r2, [pc, #108]	; (3000bedc <fault_diagnosis+0x418>)
3000be70:	6ce1      	ldr	r1, [r4, #76]	; 0x4c
3000be72:	481b      	ldr	r0, [pc, #108]	; (3000bee0 <fault_diagnosis+0x41c>)
3000be74:	f000 fab0 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be78:	e665      	b.n	3000bb46 <fault_diagnosis+0x82>
3000be7a:	4a1a      	ldr	r2, [pc, #104]	; (3000bee4 <fault_diagnosis+0x420>)
3000be7c:	6d61      	ldr	r1, [r4, #84]	; 0x54
3000be7e:	481a      	ldr	r0, [pc, #104]	; (3000bee8 <fault_diagnosis+0x424>)
3000be80:	f000 faaa 	bl	3000c3d8 <__DiagPrintf_veneer>
3000be84:	e679      	b.n	3000bb7a <fault_diagnosis+0xb6>
3000be86:	bf00      	nop
3000be88:	3000de7c 	.word	0x3000de7c
3000be8c:	3000deb0 	.word	0x3000deb0
3000be90:	3000dec8 	.word	0x3000dec8
3000be94:	3000df00 	.word	0x3000df00
3000be98:	3000df38 	.word	0x3000df38
3000be9c:	3000df60 	.word	0x3000df60
3000bea0:	3000df88 	.word	0x3000df88
3000bea4:	3000e004 	.word	0x3000e004
3000bea8:	3000e04c 	.word	0x3000e04c
3000beac:	3000e064 	.word	0x3000e064
3000beb0:	3000e0b0 	.word	0x3000e0b0
3000beb4:	3000e110 	.word	0x3000e110
3000beb8:	3000e158 	.word	0x3000e158
3000bebc:	3000e1b4 	.word	0x3000e1b4
3000bec0:	3000dca4 	.word	0x3000dca4
3000bec4:	3000dccc 	.word	0x3000dccc
3000bec8:	3000e2f0 	.word	0x3000e2f0
3000becc:	3000e2a4 	.word	0x3000e2a4
3000bed0:	3000e2bc 	.word	0x3000e2bc
3000bed4:	3000e274 	.word	0x3000e274
3000bed8:	3000e31c 	.word	0x3000e31c
3000bedc:	3000de34 	.word	0x3000de34
3000bee0:	3000de64 	.word	0x3000de64
3000bee4:	3000dfc8 	.word	0x3000dfc8
3000bee8:	3000dfec 	.word	0x3000dfec

3000beec <rtk_log_write>:
3000beec:	b40c      	push	{r2, r3}
3000beee:	4b18      	ldr	r3, [pc, #96]	; (3000bf50 <rtk_log_write+0x64>)
3000bef0:	e92d 43f0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000bef4:	681a      	ldr	r2, [r3, #0]
3000bef6:	b083      	sub	sp, #12
3000bef8:	4680      	mov	r8, r0
3000befa:	460e      	mov	r6, r1
3000befc:	2a02      	cmp	r2, #2
3000befe:	d825      	bhi.n	3000bf4c <rtk_log_write+0x60>
3000bf00:	681f      	ldr	r7, [r3, #0]
3000bf02:	b1a7      	cbz	r7, 3000bf2e <rtk_log_write+0x42>
3000bf04:	4d13      	ldr	r5, [pc, #76]	; (3000bf54 <rtk_log_write+0x68>)
3000bf06:	2400      	movs	r4, #0
3000bf08:	f8df 9054 	ldr.w	r9, [pc, #84]	; 3000bf60 <rtk_log_write+0x74>
3000bf0c:	e002      	b.n	3000bf14 <rtk_log_write+0x28>
3000bf0e:	3401      	adds	r4, #1
3000bf10:	42a7      	cmp	r7, r4
3000bf12:	d90c      	bls.n	3000bf2e <rtk_log_write+0x42>
3000bf14:	4628      	mov	r0, r5
3000bf16:	4631      	mov	r1, r6
3000bf18:	350b      	adds	r5, #11
3000bf1a:	47c8      	blx	r9
3000bf1c:	2800      	cmp	r0, #0
3000bf1e:	d1f6      	bne.n	3000bf0e <rtk_log_write+0x22>
3000bf20:	eb04 0284 	add.w	r2, r4, r4, lsl #2
3000bf24:	4b0c      	ldr	r3, [pc, #48]	; (3000bf58 <rtk_log_write+0x6c>)
3000bf26:	eb04 0442 	add.w	r4, r4, r2, lsl #1
3000bf2a:	5d1b      	ldrb	r3, [r3, r4]
3000bf2c:	e001      	b.n	3000bf32 <rtk_log_write+0x46>
3000bf2e:	4b0b      	ldr	r3, [pc, #44]	; (3000bf5c <rtk_log_write+0x70>)
3000bf30:	781b      	ldrb	r3, [r3, #0]
3000bf32:	4598      	cmp	r8, r3
3000bf34:	d805      	bhi.n	3000bf42 <rtk_log_write+0x56>
3000bf36:	aa0b      	add	r2, sp, #44	; 0x2c
3000bf38:	990a      	ldr	r1, [sp, #40]	; 0x28
3000bf3a:	2000      	movs	r0, #0
3000bf3c:	9201      	str	r2, [sp, #4]
3000bf3e:	f000 fa6b 	bl	3000c418 <__DiagVSprintf_veneer>
3000bf42:	b003      	add	sp, #12
3000bf44:	e8bd 43f0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, lr}
3000bf48:	b002      	add	sp, #8
3000bf4a:	4770      	bx	lr
3000bf4c:	2703      	movs	r7, #3
3000bf4e:	e7d9      	b.n	3000bf04 <rtk_log_write+0x18>
3000bf50:	300100c8 	.word	0x300100c8
3000bf54:	300100cd 	.word	0x300100cd
3000bf58:	300100cc 	.word	0x300100cc
3000bf5c:	3000f6ac 	.word	0x3000f6ac
3000bf60:	00012c89 	.word	0x00012c89

3000bf64 <BOOT_VerCheck>:
3000bf64:	b570      	push	{r4, r5, r6, lr}
3000bf66:	4b1d      	ldr	r3, [pc, #116]	; (3000bfdc <BOOT_VerCheck+0x78>)
3000bf68:	4798      	blx	r3
3000bf6a:	2800      	cmp	r0, #0
3000bf6c:	d135      	bne.n	3000bfda <BOOT_VerCheck+0x76>
3000bf6e:	4b1c      	ldr	r3, [pc, #112]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bf70:	2403      	movs	r4, #3
3000bf72:	4d1c      	ldr	r5, [pc, #112]	; (3000bfe4 <BOOT_VerCheck+0x80>)
3000bf74:	2003      	movs	r0, #3
3000bf76:	4a1c      	ldr	r2, [pc, #112]	; (3000bfe8 <BOOT_VerCheck+0x84>)
3000bf78:	4619      	mov	r1, r3
3000bf7a:	f7ff ffb7 	bl	3000beec <rtk_log_write>
3000bf7e:	4b18      	ldr	r3, [pc, #96]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bf80:	462a      	mov	r2, r5
3000bf82:	2003      	movs	r0, #3
3000bf84:	4619      	mov	r1, r3
3000bf86:	f7ff ffb1 	bl	3000beec <rtk_log_write>
3000bf8a:	4b15      	ldr	r3, [pc, #84]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bf8c:	4a17      	ldr	r2, [pc, #92]	; (3000bfec <BOOT_VerCheck+0x88>)
3000bf8e:	2003      	movs	r0, #3
3000bf90:	4619      	mov	r1, r3
3000bf92:	f7ff ffab 	bl	3000beec <rtk_log_write>
3000bf96:	4b12      	ldr	r3, [pc, #72]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bf98:	4a15      	ldr	r2, [pc, #84]	; (3000bff0 <BOOT_VerCheck+0x8c>)
3000bf9a:	2003      	movs	r0, #3
3000bf9c:	4619      	mov	r1, r3
3000bf9e:	f7ff ffa5 	bl	3000beec <rtk_log_write>
3000bfa2:	4b0f      	ldr	r3, [pc, #60]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bfa4:	4a13      	ldr	r2, [pc, #76]	; (3000bff4 <BOOT_VerCheck+0x90>)
3000bfa6:	2003      	movs	r0, #3
3000bfa8:	4619      	mov	r1, r3
3000bfaa:	f7ff ff9f 	bl	3000beec <rtk_log_write>
3000bfae:	4b0c      	ldr	r3, [pc, #48]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bfb0:	462a      	mov	r2, r5
3000bfb2:	2003      	movs	r0, #3
3000bfb4:	4619      	mov	r1, r3
3000bfb6:	f7ff ff99 	bl	3000beec <rtk_log_write>
3000bfba:	4b09      	ldr	r3, [pc, #36]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bfbc:	4a0e      	ldr	r2, [pc, #56]	; (3000bff8 <BOOT_VerCheck+0x94>)
3000bfbe:	2003      	movs	r0, #3
3000bfc0:	4619      	mov	r1, r3
3000bfc2:	f7ff ff93 	bl	3000beec <rtk_log_write>
3000bfc6:	3c01      	subs	r4, #1
3000bfc8:	d1d9      	bne.n	3000bf7e <BOOT_VerCheck+0x1a>
3000bfca:	4b05      	ldr	r3, [pc, #20]	; (3000bfe0 <BOOT_VerCheck+0x7c>)
3000bfcc:	2003      	movs	r0, #3
3000bfce:	4a0b      	ldr	r2, [pc, #44]	; (3000bffc <BOOT_VerCheck+0x98>)
3000bfd0:	4619      	mov	r1, r3
3000bfd2:	e8bd 4070 	ldmia.w	sp!, {r4, r5, r6, lr}
3000bfd6:	f7ff bf89 	b.w	3000beec <rtk_log_write>
3000bfda:	bd70      	pop	{r4, r5, r6, pc}
3000bfdc:	30009bfd 	.word	0x30009bfd
3000bfe0:	3000e41e 	.word	0x3000e41e
3000bfe4:	3000e466 	.word	0x3000e466
3000bfe8:	3000e423 	.word	0x3000e423
3000bfec:	3000e4a9 	.word	0x3000e4a9
3000bff0:	3000e4ec 	.word	0x3000e4ec
3000bff4:	3000e52f 	.word	0x3000e52f
3000bff8:	3000e572 	.word	0x3000e572
3000bffc:	3000e5b5 	.word	0x3000e5b5

3000c000 <__aeabi_uldivmod>:
3000c000:	b953      	cbnz	r3, 3000c018 <__aeabi_uldivmod+0x18>
3000c002:	b94a      	cbnz	r2, 3000c018 <__aeabi_uldivmod+0x18>
3000c004:	2900      	cmp	r1, #0
3000c006:	bf08      	it	eq
3000c008:	2800      	cmpeq	r0, #0
3000c00a:	bf1c      	itt	ne
3000c00c:	f04f 31ff 	movne.w	r1, #4294967295	; 0xffffffff
3000c010:	f04f 30ff 	movne.w	r0, #4294967295	; 0xffffffff
3000c014:	f000 b960 	b.w	3000c2d8 <__aeabi_idiv0>
3000c018:	f1ad 0c08 	sub.w	ip, sp, #8
3000c01c:	e96d ce04 	strd	ip, lr, [sp, #-16]!
3000c020:	f000 f806 	bl	3000c030 <__udivmoddi4>
3000c024:	f8dd e004 	ldr.w	lr, [sp, #4]
3000c028:	e9dd 2302 	ldrd	r2, r3, [sp, #8]
3000c02c:	b004      	add	sp, #16
3000c02e:	4770      	bx	lr

3000c030 <__udivmoddi4>:
3000c030:	e92d 4ff0 	stmdb	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, lr}
3000c034:	9e09      	ldr	r6, [sp, #36]	; 0x24
3000c036:	4688      	mov	r8, r1
3000c038:	468e      	mov	lr, r1
3000c03a:	4615      	mov	r5, r2
3000c03c:	4604      	mov	r4, r0
3000c03e:	4619      	mov	r1, r3
3000c040:	2b00      	cmp	r3, #0
3000c042:	f040 80c6 	bne.w	3000c1d2 <__udivmoddi4+0x1a2>
3000c046:	4542      	cmp	r2, r8
3000c048:	fab2 f782 	clz	r7, r2
3000c04c:	d946      	bls.n	3000c0dc <__udivmoddi4+0xac>
3000c04e:	b14f      	cbz	r7, 3000c064 <__udivmoddi4+0x34>
3000c050:	f1c7 0c20 	rsb	ip, r7, #32
3000c054:	fa08 f307 	lsl.w	r3, r8, r7
3000c058:	40bd      	lsls	r5, r7
3000c05a:	40bc      	lsls	r4, r7
3000c05c:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c060:	ea4c 0e03 	orr.w	lr, ip, r3
3000c064:	ea4f 4815 	mov.w	r8, r5, lsr #16
3000c068:	fa1f fc85 	uxth.w	ip, r5
3000c06c:	0c22      	lsrs	r2, r4, #16
3000c06e:	fbbe f9f8 	udiv	r9, lr, r8
3000c072:	fb08 e319 	mls	r3, r8, r9, lr
3000c076:	fb09 fa0c 	mul.w	sl, r9, ip
3000c07a:	ea42 4303 	orr.w	r3, r2, r3, lsl #16
3000c07e:	459a      	cmp	sl, r3
3000c080:	d928      	bls.n	3000c0d4 <__udivmoddi4+0xa4>
3000c082:	18eb      	adds	r3, r5, r3
3000c084:	f109 30ff 	add.w	r0, r9, #4294967295	; 0xffffffff
3000c088:	d204      	bcs.n	3000c094 <__udivmoddi4+0x64>
3000c08a:	459a      	cmp	sl, r3
3000c08c:	d902      	bls.n	3000c094 <__udivmoddi4+0x64>
3000c08e:	f1a9 0002 	sub.w	r0, r9, #2
3000c092:	442b      	add	r3, r5
3000c094:	eba3 030a 	sub.w	r3, r3, sl
3000c098:	b2a4      	uxth	r4, r4
3000c09a:	fbb3 f2f8 	udiv	r2, r3, r8
3000c09e:	fb08 3312 	mls	r3, r8, r2, r3
3000c0a2:	fb02 fc0c 	mul.w	ip, r2, ip
3000c0a6:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c0aa:	45a4      	cmp	ip, r4
3000c0ac:	d914      	bls.n	3000c0d8 <__udivmoddi4+0xa8>
3000c0ae:	192c      	adds	r4, r5, r4
3000c0b0:	f102 33ff 	add.w	r3, r2, #4294967295	; 0xffffffff
3000c0b4:	d203      	bcs.n	3000c0be <__udivmoddi4+0x8e>
3000c0b6:	45a4      	cmp	ip, r4
3000c0b8:	d901      	bls.n	3000c0be <__udivmoddi4+0x8e>
3000c0ba:	1e93      	subs	r3, r2, #2
3000c0bc:	442c      	add	r4, r5
3000c0be:	eba4 040c 	sub.w	r4, r4, ip
3000c0c2:	ea43 4000 	orr.w	r0, r3, r0, lsl #16
3000c0c6:	b11e      	cbz	r6, 3000c0d0 <__udivmoddi4+0xa0>
3000c0c8:	40fc      	lsrs	r4, r7
3000c0ca:	2300      	movs	r3, #0
3000c0cc:	6034      	str	r4, [r6, #0]
3000c0ce:	6073      	str	r3, [r6, #4]
3000c0d0:	e8bd 8ff0 	ldmia.w	sp!, {r4, r5, r6, r7, r8, r9, sl, fp, pc}
3000c0d4:	4648      	mov	r0, r9
3000c0d6:	e7dd      	b.n	3000c094 <__udivmoddi4+0x64>
3000c0d8:	4613      	mov	r3, r2
3000c0da:	e7f0      	b.n	3000c0be <__udivmoddi4+0x8e>
3000c0dc:	b902      	cbnz	r2, 3000c0e0 <__udivmoddi4+0xb0>
3000c0de:	deff      	udf	#255	; 0xff
3000c0e0:	bb8f      	cbnz	r7, 3000c146 <__udivmoddi4+0x116>
3000c0e2:	eba8 0302 	sub.w	r3, r8, r2
3000c0e6:	2101      	movs	r1, #1
3000c0e8:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c0ec:	b2aa      	uxth	r2, r5
3000c0ee:	0c20      	lsrs	r0, r4, #16
3000c0f0:	fbb3 fcfe 	udiv	ip, r3, lr
3000c0f4:	fb0e 331c 	mls	r3, lr, ip, r3
3000c0f8:	fb0c f802 	mul.w	r8, ip, r2
3000c0fc:	ea40 4303 	orr.w	r3, r0, r3, lsl #16
3000c100:	4598      	cmp	r8, r3
3000c102:	d962      	bls.n	3000c1ca <__udivmoddi4+0x19a>
3000c104:	18eb      	adds	r3, r5, r3
3000c106:	f10c 30ff 	add.w	r0, ip, #4294967295	; 0xffffffff
3000c10a:	d204      	bcs.n	3000c116 <__udivmoddi4+0xe6>
3000c10c:	4598      	cmp	r8, r3
3000c10e:	d902      	bls.n	3000c116 <__udivmoddi4+0xe6>
3000c110:	f1ac 0002 	sub.w	r0, ip, #2
3000c114:	442b      	add	r3, r5
3000c116:	eba3 0308 	sub.w	r3, r3, r8
3000c11a:	b2a4      	uxth	r4, r4
3000c11c:	fbb3 fcfe 	udiv	ip, r3, lr
3000c120:	fb0e 331c 	mls	r3, lr, ip, r3
3000c124:	fb0c f202 	mul.w	r2, ip, r2
3000c128:	ea44 4403 	orr.w	r4, r4, r3, lsl #16
3000c12c:	42a2      	cmp	r2, r4
3000c12e:	d94e      	bls.n	3000c1ce <__udivmoddi4+0x19e>
3000c130:	192c      	adds	r4, r5, r4
3000c132:	f10c 33ff 	add.w	r3, ip, #4294967295	; 0xffffffff
3000c136:	d204      	bcs.n	3000c142 <__udivmoddi4+0x112>
3000c138:	42a2      	cmp	r2, r4
3000c13a:	d902      	bls.n	3000c142 <__udivmoddi4+0x112>
3000c13c:	f1ac 0302 	sub.w	r3, ip, #2
3000c140:	442c      	add	r4, r5
3000c142:	1aa4      	subs	r4, r4, r2
3000c144:	e7bd      	b.n	3000c0c2 <__udivmoddi4+0x92>
3000c146:	f1c7 0c20 	rsb	ip, r7, #32
3000c14a:	40bd      	lsls	r5, r7
3000c14c:	fa08 f307 	lsl.w	r3, r8, r7
3000c150:	40bc      	lsls	r4, r7
3000c152:	fa28 f90c 	lsr.w	r9, r8, ip
3000c156:	ea4f 4e15 	mov.w	lr, r5, lsr #16
3000c15a:	fa20 fc0c 	lsr.w	ip, r0, ip
3000c15e:	ea4c 0203 	orr.w	r2, ip, r3
3000c162:	b2ab      	uxth	r3, r5
3000c164:	fbb9 fcfe 	udiv	ip, r9, lr
3000c168:	0c11      	lsrs	r1, r2, #16
3000c16a:	fb0e 901c 	mls	r0, lr, ip, r9
3000c16e:	fb0c f803 	mul.w	r8, ip, r3
3000c172:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c176:	4580      	cmp	r8, r0
3000c178:	d923      	bls.n	3000c1c2 <__udivmoddi4+0x192>
3000c17a:	1828      	adds	r0, r5, r0
3000c17c:	f10c 31ff 	add.w	r1, ip, #4294967295	; 0xffffffff
3000c180:	d204      	bcs.n	3000c18c <__udivmoddi4+0x15c>
3000c182:	4580      	cmp	r8, r0
3000c184:	d902      	bls.n	3000c18c <__udivmoddi4+0x15c>
3000c186:	f1ac 0102 	sub.w	r1, ip, #2
3000c18a:	4428      	add	r0, r5
3000c18c:	eba0 0008 	sub.w	r0, r0, r8
3000c190:	b292      	uxth	r2, r2
3000c192:	fbb0 fcfe 	udiv	ip, r0, lr
3000c196:	fb0e 001c 	mls	r0, lr, ip, r0
3000c19a:	fb0c f803 	mul.w	r8, ip, r3
3000c19e:	ea42 4300 	orr.w	r3, r2, r0, lsl #16
3000c1a2:	4598      	cmp	r8, r3
3000c1a4:	d90f      	bls.n	3000c1c6 <__udivmoddi4+0x196>
3000c1a6:	18eb      	adds	r3, r5, r3
3000c1a8:	f10c 32ff 	add.w	r2, ip, #4294967295	; 0xffffffff
3000c1ac:	d204      	bcs.n	3000c1b8 <__udivmoddi4+0x188>
3000c1ae:	4598      	cmp	r8, r3
3000c1b0:	d902      	bls.n	3000c1b8 <__udivmoddi4+0x188>
3000c1b2:	f1ac 0202 	sub.w	r2, ip, #2
3000c1b6:	442b      	add	r3, r5
3000c1b8:	eba3 0308 	sub.w	r3, r3, r8
3000c1bc:	ea42 4101 	orr.w	r1, r2, r1, lsl #16
3000c1c0:	e792      	b.n	3000c0e8 <__udivmoddi4+0xb8>
3000c1c2:	4661      	mov	r1, ip
3000c1c4:	e7e2      	b.n	3000c18c <__udivmoddi4+0x15c>
3000c1c6:	4662      	mov	r2, ip
3000c1c8:	e7f6      	b.n	3000c1b8 <__udivmoddi4+0x188>
3000c1ca:	4660      	mov	r0, ip
3000c1cc:	e7a3      	b.n	3000c116 <__udivmoddi4+0xe6>
3000c1ce:	4663      	mov	r3, ip
3000c1d0:	e7b7      	b.n	3000c142 <__udivmoddi4+0x112>
3000c1d2:	4543      	cmp	r3, r8
3000c1d4:	d905      	bls.n	3000c1e2 <__udivmoddi4+0x1b2>
3000c1d6:	b10e      	cbz	r6, 3000c1dc <__udivmoddi4+0x1ac>
3000c1d8:	e9c6 0800 	strd	r0, r8, [r6]
3000c1dc:	2100      	movs	r1, #0
3000c1de:	4608      	mov	r0, r1
3000c1e0:	e776      	b.n	3000c0d0 <__udivmoddi4+0xa0>
3000c1e2:	fab3 fc83 	clz	ip, r3
3000c1e6:	f1bc 0f00 	cmp.w	ip, #0
3000c1ea:	d110      	bne.n	3000c20e <__udivmoddi4+0x1de>
3000c1ec:	4543      	cmp	r3, r8
3000c1ee:	d301      	bcc.n	3000c1f4 <__udivmoddi4+0x1c4>
3000c1f0:	4282      	cmp	r2, r0
3000c1f2:	d80a      	bhi.n	3000c20a <__udivmoddi4+0x1da>
3000c1f4:	1a84      	subs	r4, r0, r2
3000c1f6:	eb68 0303 	sbc.w	r3, r8, r3
3000c1fa:	2001      	movs	r0, #1
3000c1fc:	469e      	mov	lr, r3
3000c1fe:	2e00      	cmp	r6, #0
3000c200:	d068      	beq.n	3000c2d4 <__udivmoddi4+0x2a4>
3000c202:	e9c6 4e00 	strd	r4, lr, [r6]
3000c206:	2100      	movs	r1, #0
3000c208:	e762      	b.n	3000c0d0 <__udivmoddi4+0xa0>
3000c20a:	4660      	mov	r0, ip
3000c20c:	e7f7      	b.n	3000c1fe <__udivmoddi4+0x1ce>
3000c20e:	f1cc 0e20 	rsb	lr, ip, #32
3000c212:	fa03 f30c 	lsl.w	r3, r3, ip
3000c216:	fa02 f50c 	lsl.w	r5, r2, ip
3000c21a:	fa00 f70c 	lsl.w	r7, r0, ip
3000c21e:	fa22 f40e 	lsr.w	r4, r2, lr
3000c222:	fa28 fb0e 	lsr.w	fp, r8, lr
3000c226:	fa20 f10e 	lsr.w	r1, r0, lr
3000c22a:	431c      	orrs	r4, r3
3000c22c:	fa08 f30c 	lsl.w	r3, r8, ip
3000c230:	ea4f 4914 	mov.w	r9, r4, lsr #16
3000c234:	4319      	orrs	r1, r3
3000c236:	b2a3      	uxth	r3, r4
3000c238:	0c0a      	lsrs	r2, r1, #16
3000c23a:	fbbb f8f9 	udiv	r8, fp, r9
3000c23e:	fb09 bb18 	mls	fp, r9, r8, fp
3000c242:	fb08 fa03 	mul.w	sl, r8, r3
3000c246:	ea42 420b 	orr.w	r2, r2, fp, lsl #16
3000c24a:	4592      	cmp	sl, r2
3000c24c:	d93e      	bls.n	3000c2cc <__udivmoddi4+0x29c>
3000c24e:	18a2      	adds	r2, r4, r2
3000c250:	f108 30ff 	add.w	r0, r8, #4294967295	; 0xffffffff
3000c254:	d204      	bcs.n	3000c260 <__udivmoddi4+0x230>
3000c256:	4592      	cmp	sl, r2
3000c258:	d902      	bls.n	3000c260 <__udivmoddi4+0x230>
3000c25a:	f1a8 0002 	sub.w	r0, r8, #2
3000c25e:	4422      	add	r2, r4
3000c260:	eba2 020a 	sub.w	r2, r2, sl
3000c264:	b289      	uxth	r1, r1
3000c266:	fbb2 f8f9 	udiv	r8, r2, r9
3000c26a:	fb09 2218 	mls	r2, r9, r8, r2
3000c26e:	fb08 f303 	mul.w	r3, r8, r3
3000c272:	ea41 4202 	orr.w	r2, r1, r2, lsl #16
3000c276:	4293      	cmp	r3, r2
3000c278:	d92a      	bls.n	3000c2d0 <__udivmoddi4+0x2a0>
3000c27a:	18a2      	adds	r2, r4, r2
3000c27c:	f108 31ff 	add.w	r1, r8, #4294967295	; 0xffffffff
3000c280:	d204      	bcs.n	3000c28c <__udivmoddi4+0x25c>
3000c282:	4293      	cmp	r3, r2
3000c284:	d902      	bls.n	3000c28c <__udivmoddi4+0x25c>
3000c286:	f1a8 0102 	sub.w	r1, r8, #2
3000c28a:	4422      	add	r2, r4
3000c28c:	ea41 4000 	orr.w	r0, r1, r0, lsl #16
3000c290:	1ad2      	subs	r2, r2, r3
3000c292:	fba0 9805 	umull	r9, r8, r0, r5
3000c296:	4542      	cmp	r2, r8
3000c298:	464b      	mov	r3, r9
3000c29a:	4641      	mov	r1, r8
3000c29c:	d302      	bcc.n	3000c2a4 <__udivmoddi4+0x274>
3000c29e:	d106      	bne.n	3000c2ae <__udivmoddi4+0x27e>
3000c2a0:	454f      	cmp	r7, r9
3000c2a2:	d204      	bcs.n	3000c2ae <__udivmoddi4+0x27e>
3000c2a4:	3801      	subs	r0, #1
3000c2a6:	ebb9 0305 	subs.w	r3, r9, r5
3000c2aa:	eb68 0104 	sbc.w	r1, r8, r4
3000c2ae:	b18e      	cbz	r6, 3000c2d4 <__udivmoddi4+0x2a4>
3000c2b0:	1afc      	subs	r4, r7, r3
3000c2b2:	eb62 0701 	sbc.w	r7, r2, r1
3000c2b6:	fa07 fe0e 	lsl.w	lr, r7, lr
3000c2ba:	fa24 f30c 	lsr.w	r3, r4, ip
3000c2be:	fa27 f70c 	lsr.w	r7, r7, ip
3000c2c2:	ea4e 0303 	orr.w	r3, lr, r3
3000c2c6:	e9c6 3700 	strd	r3, r7, [r6]
3000c2ca:	e79c      	b.n	3000c206 <__udivmoddi4+0x1d6>
3000c2cc:	4640      	mov	r0, r8
3000c2ce:	e7c7      	b.n	3000c260 <__udivmoddi4+0x230>
3000c2d0:	4641      	mov	r1, r8
3000c2d2:	e7db      	b.n	3000c28c <__udivmoddi4+0x25c>
3000c2d4:	4631      	mov	r1, r6
3000c2d6:	e6fb      	b.n	3000c0d0 <__udivmoddi4+0xa0>

3000c2d8 <__aeabi_idiv0>:
3000c2d8:	4770      	bx	lr
3000c2da:	bf00      	nop

3000c2dc <_mbtowc_r>:
3000c2dc:	b430      	push	{r4, r5}
3000c2de:	4c06      	ldr	r4, [pc, #24]	; (3000c2f8 <_mbtowc_r+0x1c>)
3000c2e0:	4d06      	ldr	r5, [pc, #24]	; (3000c2fc <_mbtowc_r+0x20>)
3000c2e2:	6824      	ldr	r4, [r4, #0]
3000c2e4:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c2e6:	2c00      	cmp	r4, #0
3000c2e8:	bf08      	it	eq
3000c2ea:	462c      	moveq	r4, r5
3000c2ec:	f8d4 40e4 	ldr.w	r4, [r4, #228]	; 0xe4
3000c2f0:	46a4      	mov	ip, r4
3000c2f2:	bc30      	pop	{r4, r5}
3000c2f4:	4760      	bx	ip
3000c2f6:	bf00      	nop
3000c2f8:	3000f820 	.word	0x3000f820
3000c2fc:	3000f6b0 	.word	0x3000f6b0

3000c300 <__ascii_mbtowc>:
3000c300:	b082      	sub	sp, #8
3000c302:	b901      	cbnz	r1, 3000c306 <__ascii_mbtowc+0x6>
3000c304:	a901      	add	r1, sp, #4
3000c306:	b142      	cbz	r2, 3000c31a <__ascii_mbtowc+0x1a>
3000c308:	b14b      	cbz	r3, 3000c31e <__ascii_mbtowc+0x1e>
3000c30a:	7813      	ldrb	r3, [r2, #0]
3000c30c:	600b      	str	r3, [r1, #0]
3000c30e:	7812      	ldrb	r2, [r2, #0]
3000c310:	1e10      	subs	r0, r2, #0
3000c312:	bf18      	it	ne
3000c314:	2001      	movne	r0, #1
3000c316:	b002      	add	sp, #8
3000c318:	4770      	bx	lr
3000c31a:	4610      	mov	r0, r2
3000c31c:	e7fb      	b.n	3000c316 <__ascii_mbtowc+0x16>
3000c31e:	f06f 0001 	mvn.w	r0, #1
3000c322:	e7f8      	b.n	3000c316 <__ascii_mbtowc+0x16>

3000c324 <_wctomb_r>:
3000c324:	b430      	push	{r4, r5}
3000c326:	4c06      	ldr	r4, [pc, #24]	; (3000c340 <_wctomb_r+0x1c>)
3000c328:	4d06      	ldr	r5, [pc, #24]	; (3000c344 <_wctomb_r+0x20>)
3000c32a:	6824      	ldr	r4, [r4, #0]
3000c32c:	6b64      	ldr	r4, [r4, #52]	; 0x34
3000c32e:	2c00      	cmp	r4, #0
3000c330:	bf08      	it	eq
3000c332:	462c      	moveq	r4, r5
3000c334:	f8d4 40e0 	ldr.w	r4, [r4, #224]	; 0xe0
3000c338:	46a4      	mov	ip, r4
3000c33a:	bc30      	pop	{r4, r5}
3000c33c:	4760      	bx	ip
3000c33e:	bf00      	nop
3000c340:	3000f820 	.word	0x3000f820
3000c344:	3000f6b0 	.word	0x3000f6b0

3000c348 <__ascii_wctomb>:
3000c348:	b149      	cbz	r1, 3000c35e <__ascii_wctomb+0x16>
3000c34a:	2aff      	cmp	r2, #255	; 0xff
3000c34c:	bf8d      	iteet	hi
3000c34e:	238a      	movhi	r3, #138	; 0x8a
3000c350:	2001      	movls	r0, #1
3000c352:	700a      	strbls	r2, [r1, #0]
3000c354:	6003      	strhi	r3, [r0, #0]
3000c356:	bf88      	it	hi
3000c358:	f04f 30ff 	movhi.w	r0, #4294967295	; 0xffffffff
3000c35c:	4770      	bx	lr
3000c35e:	4608      	mov	r0, r1
3000c360:	4770      	bx	lr
3000c362:	bf00      	nop
3000c364:	0000      	movs	r0, r0
	...

3000c368 <__CRYPTO_OTPKey_SHA_Init_B_veneer>:
3000c368:	b401      	push	{r0}
3000c36a:	4802      	ldr	r0, [pc, #8]	; (3000c374 <__CRYPTO_OTPKey_SHA_Init_B_veneer+0xc>)
3000c36c:	4684      	mov	ip, r0
3000c36e:	bc01      	pop	{r0}
3000c370:	4760      	bx	ip
3000c372:	bf00      	nop
3000c374:	000024dd 	.word	0x000024dd

3000c378 <____wrap_memset_veneer>:
3000c378:	b401      	push	{r0}
3000c37a:	4802      	ldr	r0, [pc, #8]	; (3000c384 <____wrap_memset_veneer+0xc>)
3000c37c:	4684      	mov	ip, r0
3000c37e:	bc01      	pop	{r0}
3000c380:	4760      	bx	ip
3000c382:	bf00      	nop
3000c384:	00012efd 	.word	0x00012efd

3000c388 <__INT_HardFault_C_veneer>:
3000c388:	b401      	push	{r0}
3000c38a:	4802      	ldr	r0, [pc, #8]	; (3000c394 <__INT_HardFault_C_veneer+0xc>)
3000c38c:	4684      	mov	ip, r0
3000c38e:	bc01      	pop	{r0}
3000c390:	4760      	bx	ip
3000c392:	bf00      	nop
3000c394:	0000d8ed 	.word	0x0000d8ed

3000c398 <____wrap_memcpy_veneer>:
3000c398:	b401      	push	{r0}
3000c39a:	4802      	ldr	r0, [pc, #8]	; (3000c3a4 <____wrap_memcpy_veneer+0xc>)
3000c39c:	4684      	mov	ip, r0
3000c39e:	bc01      	pop	{r0}
3000c3a0:	4760      	bx	ip
3000c3a2:	bf00      	nop
3000c3a4:	00012f05 	.word	0x00012f05

3000c3a8 <__CRYPTO_SHA_Init_B_veneer>:
3000c3a8:	b401      	push	{r0}
3000c3aa:	4802      	ldr	r0, [pc, #8]	; (3000c3b4 <__CRYPTO_SHA_Init_B_veneer+0xc>)
3000c3ac:	4684      	mov	ip, r0
3000c3ae:	bc01      	pop	{r0}
3000c3b0:	4760      	bx	ip
3000c3b2:	bf00      	nop
3000c3b4:	00002451 	.word	0x00002451

3000c3b8 <__CRYPTO_OTPKey_SHA_Init_A_veneer>:
3000c3b8:	b401      	push	{r0}
3000c3ba:	4802      	ldr	r0, [pc, #8]	; (3000c3c4 <__CRYPTO_OTPKey_SHA_Init_A_veneer+0xc>)
3000c3bc:	4684      	mov	ip, r0
3000c3be:	bc01      	pop	{r0}
3000c3c0:	4760      	bx	ip
3000c3c2:	bf00      	nop
3000c3c4:	00002369 	.word	0x00002369

3000c3c8 <__rtl_crypto_sha2_final_B_veneer>:
3000c3c8:	b401      	push	{r0}
3000c3ca:	4802      	ldr	r0, [pc, #8]	; (3000c3d4 <__rtl_crypto_sha2_final_B_veneer+0xc>)
3000c3cc:	4684      	mov	ip, r0
3000c3ce:	bc01      	pop	{r0}
3000c3d0:	4760      	bx	ip
3000c3d2:	bf00      	nop
3000c3d4:	000053f1 	.word	0x000053f1

3000c3d8 <__DiagPrintf_veneer>:
3000c3d8:	b401      	push	{r0}
3000c3da:	4802      	ldr	r0, [pc, #8]	; (3000c3e4 <__DiagPrintf_veneer+0xc>)
3000c3dc:	4684      	mov	ip, r0
3000c3de:	bc01      	pop	{r0}
3000c3e0:	4760      	bx	ip
3000c3e2:	bf00      	nop
3000c3e4:	0000e5f9 	.word	0x0000e5f9

3000c3e8 <__SBOOT_Validate_Algorithm_A_veneer>:
3000c3e8:	b401      	push	{r0}
3000c3ea:	4802      	ldr	r0, [pc, #8]	; (3000c3f4 <__SBOOT_Validate_Algorithm_A_veneer+0xc>)
3000c3ec:	4684      	mov	ip, r0
3000c3ee:	bc01      	pop	{r0}
3000c3f0:	4760      	bx	ip
3000c3f2:	bf00      	nop
3000c3f4:	00003279 	.word	0x00003279

3000c3f8 <__rtl_crypto_hmac_sha2_update_B_veneer>:
3000c3f8:	b401      	push	{r0}
3000c3fa:	4802      	ldr	r0, [pc, #8]	; (3000c404 <__rtl_crypto_hmac_sha2_update_B_veneer+0xc>)
3000c3fc:	4684      	mov	ip, r0
3000c3fe:	bc01      	pop	{r0}
3000c400:	4760      	bx	ip
3000c402:	bf00      	nop
3000c404:	00005b4d 	.word	0x00005b4d

3000c408 <__SBOOT_Validate_Algorithm_B_veneer>:
3000c408:	b401      	push	{r0}
3000c40a:	4802      	ldr	r0, [pc, #8]	; (3000c414 <__SBOOT_Validate_Algorithm_B_veneer+0xc>)
3000c40c:	4684      	mov	ip, r0
3000c40e:	bc01      	pop	{r0}
3000c410:	4760      	bx	ip
3000c412:	bf00      	nop
3000c414:	000033ed 	.word	0x000033ed

3000c418 <__DiagVSprintf_veneer>:
3000c418:	b401      	push	{r0}
3000c41a:	4802      	ldr	r0, [pc, #8]	; (3000c424 <__DiagVSprintf_veneer+0xc>)
3000c41c:	4684      	mov	ip, r0
3000c41e:	bc01      	pop	{r0}
3000c420:	4760      	bx	ip
3000c422:	bf00      	nop
3000c424:	0000e331 	.word	0x0000e331

3000c428 <__SBOOT_Validate_PubKey_B_veneer>:
3000c428:	b401      	push	{r0}
3000c42a:	4802      	ldr	r0, [pc, #8]	; (3000c434 <__SBOOT_Validate_PubKey_B_veneer+0xc>)
3000c42c:	4684      	mov	ip, r0
3000c42e:	bc01      	pop	{r0}
3000c430:	4760      	bx	ip
3000c432:	bf00      	nop
3000c434:	00003495 	.word	0x00003495

3000c438 <__SBOOT_Validate_Signature_B_veneer>:
3000c438:	b401      	push	{r0}
3000c43a:	4802      	ldr	r0, [pc, #8]	; (3000c444 <__SBOOT_Validate_Signature_B_veneer+0xc>)
3000c43c:	4684      	mov	ip, r0
3000c43e:	bc01      	pop	{r0}
3000c440:	4760      	bx	ip
3000c442:	bf00      	nop
3000c444:	00003431 	.word	0x00003431

3000c448 <__SBOOT_Validate_PubKey_A_veneer>:
3000c448:	b401      	push	{r0}
3000c44a:	4802      	ldr	r0, [pc, #8]	; (3000c454 <__SBOOT_Validate_PubKey_A_veneer+0xc>)
3000c44c:	4684      	mov	ip, r0
3000c44e:	bc01      	pop	{r0}
3000c450:	4760      	bx	ip
3000c452:	bf00      	nop
3000c454:	00003321 	.word	0x00003321

3000c458 <__SBOOT_Validate_ImgHash_B_veneer>:
3000c458:	b401      	push	{r0}
3000c45a:	4802      	ldr	r0, [pc, #8]	; (3000c464 <__SBOOT_Validate_ImgHash_B_veneer+0xc>)
3000c45c:	4684      	mov	ip, r0
3000c45e:	bc01      	pop	{r0}
3000c460:	4760      	bx	ip
3000c462:	bf00      	nop
3000c464:	00003541 	.word	0x00003541

3000c468 <____wrap_memcmp_veneer>:
3000c468:	b401      	push	{r0}
3000c46a:	4802      	ldr	r0, [pc, #8]	; (3000c474 <____wrap_memcmp_veneer+0xc>)
3000c46c:	4684      	mov	ip, r0
3000c46e:	bc01      	pop	{r0}
3000c470:	4760      	bx	ip
3000c472:	bf00      	nop
3000c474:	00012f0d 	.word	0x00012f0d

3000c478 <____wrap_strlen_veneer>:
3000c478:	b401      	push	{r0}
3000c47a:	4802      	ldr	r0, [pc, #8]	; (3000c484 <____wrap_strlen_veneer+0xc>)
3000c47c:	4684      	mov	ip, r0
3000c47e:	bc01      	pop	{r0}
3000c480:	4760      	bx	ip
3000c482:	bf00      	nop
3000c484:	00012f25 	.word	0x00012f25

3000c488 <__rtl_crypto_hmac_sha2_update_A_veneer>:
3000c488:	b401      	push	{r0}
3000c48a:	4802      	ldr	r0, [pc, #8]	; (3000c494 <__rtl_crypto_hmac_sha2_update_A_veneer+0xc>)
3000c48c:	4684      	mov	ip, r0
3000c48e:	bc01      	pop	{r0}
3000c490:	4760      	bx	ip
3000c492:	bf00      	nop
3000c494:	000059d9 	.word	0x000059d9

3000c498 <__SBOOT_Validate_Signature_A_veneer>:
3000c498:	b401      	push	{r0}
3000c49a:	4802      	ldr	r0, [pc, #8]	; (3000c4a4 <__SBOOT_Validate_Signature_A_veneer+0xc>)
3000c49c:	4684      	mov	ip, r0
3000c49e:	bc01      	pop	{r0}
3000c4a0:	4760      	bx	ip
3000c4a2:	bf00      	nop
3000c4a4:	000032bd 	.word	0x000032bd

3000c4a8 <__rtl_crypto_sha2_init_A_veneer>:
3000c4a8:	b401      	push	{r0}
3000c4aa:	4802      	ldr	r0, [pc, #8]	; (3000c4b4 <__rtl_crypto_sha2_init_A_veneer+0xc>)
3000c4ac:	4684      	mov	ip, r0
3000c4ae:	bc01      	pop	{r0}
3000c4b0:	4760      	bx	ip
3000c4b2:	bf00      	nop
3000c4b4:	00004e89 	.word	0x00004e89

3000c4b8 <__SBOOT_GetMdType_B_veneer>:
3000c4b8:	b401      	push	{r0}
3000c4ba:	4802      	ldr	r0, [pc, #8]	; (3000c4c4 <__SBOOT_GetMdType_B_veneer+0xc>)
3000c4bc:	4684      	mov	ip, r0
3000c4be:	bc01      	pop	{r0}
3000c4c0:	4760      	bx	ip
3000c4c2:	bf00      	nop
3000c4c4:	0000319d 	.word	0x0000319d

3000c4c8 <__rtl_crypto_sha2_final_A_veneer>:
3000c4c8:	b401      	push	{r0}
3000c4ca:	4802      	ldr	r0, [pc, #8]	; (3000c4d4 <__rtl_crypto_sha2_final_A_veneer+0xc>)
3000c4cc:	4684      	mov	ip, r0
3000c4ce:	bc01      	pop	{r0}
3000c4d0:	4760      	bx	ip
3000c4d2:	bf00      	nop
3000c4d4:	0000527d 	.word	0x0000527d

3000c4d8 <__io_assert_failed_veneer>:
3000c4d8:	b401      	push	{r0}
3000c4da:	4802      	ldr	r0, [pc, #8]	; (3000c4e4 <__io_assert_failed_veneer+0xc>)
3000c4dc:	4684      	mov	ip, r0
3000c4de:	bc01      	pop	{r0}
3000c4e0:	4760      	bx	ip
3000c4e2:	bf00      	nop
3000c4e4:	0000992d 	.word	0x0000992d

3000c4e8 <__rtl_crypto_sha2_init_B_veneer>:
3000c4e8:	b401      	push	{r0}
3000c4ea:	4802      	ldr	r0, [pc, #8]	; (3000c4f4 <__rtl_crypto_sha2_init_B_veneer+0xc>)
3000c4ec:	4684      	mov	ip, r0
3000c4ee:	bc01      	pop	{r0}
3000c4f0:	4760      	bx	ip
3000c4f2:	bf00      	nop
3000c4f4:	00004ffd 	.word	0x00004ffd

3000c4f8 <__SBOOT_Validate_ImgHash_A_veneer>:
3000c4f8:	b401      	push	{r0}
3000c4fa:	4802      	ldr	r0, [pc, #8]	; (3000c504 <__SBOOT_Validate_ImgHash_A_veneer+0xc>)
3000c4fc:	4684      	mov	ip, r0
3000c4fe:	bc01      	pop	{r0}
3000c500:	4760      	bx	ip
3000c502:	bf00      	nop
3000c504:	000033cd 	.word	0x000033cd

3000c508 <__rtl_crypto_hmac_sha2_final_A_veneer>:
3000c508:	b401      	push	{r0}
3000c50a:	4802      	ldr	r0, [pc, #8]	; (3000c514 <__rtl_crypto_hmac_sha2_final_A_veneer+0xc>)
3000c50c:	4684      	mov	ip, r0
3000c50e:	bc01      	pop	{r0}
3000c510:	4760      	bx	ip
3000c512:	bf00      	nop
3000c514:	00005a45 	.word	0x00005a45

3000c518 <__CRYPTO_SHA_Init_A_veneer>:
3000c518:	b401      	push	{r0}
3000c51a:	4802      	ldr	r0, [pc, #8]	; (3000c524 <__CRYPTO_SHA_Init_A_veneer+0xc>)
3000c51c:	4684      	mov	ip, r0
3000c51e:	bc01      	pop	{r0}
3000c520:	4760      	bx	ip
3000c522:	bf00      	nop
3000c524:	000022dd 	.word	0x000022dd

3000c528 <__SBOOT_GetMdType_A_veneer>:
3000c528:	b401      	push	{r0}
3000c52a:	4802      	ldr	r0, [pc, #8]	; (3000c534 <__SBOOT_GetMdType_A_veneer+0xc>)
3000c52c:	4684      	mov	ip, r0
3000c52e:	bc01      	pop	{r0}
3000c530:	4760      	bx	ip
3000c532:	bf00      	nop
3000c534:	00003029 	.word	0x00003029

3000c538 <__rtl_crypto_hmac_sha2_final_B_veneer>:
3000c538:	b401      	push	{r0}
3000c53a:	4802      	ldr	r0, [pc, #8]	; (3000c544 <__rtl_crypto_hmac_sha2_final_B_veneer+0xc>)
3000c53c:	4684      	mov	ip, r0
3000c53e:	bc01      	pop	{r0}
3000c540:	4760      	bx	ip
3000c542:	bf00      	nop
3000c544:	00005bb9 	.word	0x00005bb9

3000c548 <__rtl_crypto_sha2_update_B_veneer>:
3000c548:	b401      	push	{r0}
3000c54a:	4802      	ldr	r0, [pc, #8]	; (3000c554 <__rtl_crypto_sha2_update_B_veneer+0xc>)
3000c54c:	4684      	mov	ip, r0
3000c54e:	bc01      	pop	{r0}
3000c550:	4760      	bx	ip
3000c552:	bf00      	nop
3000c554:	00005385 	.word	0x00005385

3000c558 <__rtl_crypto_sha2_update_A_veneer>:
3000c558:	b401      	push	{r0}
3000c55a:	4802      	ldr	r0, [pc, #8]	; (3000c564 <__rtl_crypto_sha2_update_A_veneer+0xc>)
3000c55c:	4684      	mov	ip, r0
3000c55e:	bc01      	pop	{r0}
3000c560:	4760      	bx	ip
3000c562:	bf00      	nop
3000c564:	00005211 	.word	0x00005211
3000c568:	004f4931 	.word	0x004f4931
3000c56c:	00004f32 	.word	0x00004f32
3000c570:	004f4932 	.word	0x004f4932
3000c574:	00004f34 	.word	0x00004f34
3000c578:	004f4934 	.word	0x004f4934
3000c57c:	53414c46 	.word	0x53414c46
3000c580:	4b4c4348 	.word	0x4b4c4348
3000c584:	00000000 	.word	0x00000000
3000c588:	2d73255b 	.word	0x2d73255b
3000c58c:	63205d49 	.word	0x63205d49
3000c590:	62696c61 	.word	0x62696c61
3000c594:	69746172 	.word	0x69746172
3000c598:	6f5f6e6f 	.word	0x6f5f6e6f
3000c59c:	255b3a6b 	.word	0x255b3a6b
3000c5a0:	64253a64 	.word	0x64253a64
3000c5a4:	5d64253a 	.word	0x5d64253a
3000c5a8:	00000a20 	.word	0x00000a20
3000c5ac:	2d73255b 	.word	0x2d73255b
3000c5b0:	46205d49 	.word	0x46205d49
3000c5b4:	4853414c 	.word	0x4853414c
3000c5b8:	4c414320 	.word	0x4c414320
3000c5bc:	305b4249 	.word	0x305b4249
3000c5c0:	20782578 	.word	0x20782578
3000c5c4:	0a5d4b4f 	.word	0x0a5d4b4f
3000c5c8:	00000000 	.word	0x00000000
3000c5cc:	2d73255b 	.word	0x2d73255b
3000c5d0:	46205d45 	.word	0x46205d45
3000c5d4:	4853414c 	.word	0x4853414c
3000c5d8:	4c414320 	.word	0x4c414320
3000c5dc:	305b4249 	.word	0x305b4249
3000c5e0:	20782578 	.word	0x20782578
3000c5e4:	4c494146 	.word	0x4c494146
3000c5e8:	00000a5d 	.word	0x00000a5d
3000c5ec:	2d73255b 	.word	0x2d73255b
3000c5f0:	46205d49 	.word	0x46205d49
3000c5f4:	6873616c 	.word	0x6873616c
3000c5f8:	61655220 	.word	0x61655220
3000c5fc:	73252064 	.word	0x73252064
3000c600:	0000000a 	.word	0x0000000a
3000c604:	2d73255b 	.word	0x2d73255b
3000c608:	46205d45 	.word	0x46205d45
3000c60c:	6873616c 	.word	0x6873616c
3000c610:	69775320 	.word	0x69775320
3000c614:	20686374 	.word	0x20686374
3000c618:	64616552 	.word	0x64616552
3000c61c:	646f4d20 	.word	0x646f4d20
3000c620:	41462065 	.word	0x41462065
3000c624:	000a4c49 	.word	0x000a4c49
3000c628:	2d73255b 	.word	0x2d73255b
3000c62c:	46205d49 	.word	0x46205d49
3000c630:	6873616c 	.word	0x6873616c
3000c634:	3a444920 	.word	0x3a444920
3000c638:	2d782520 	.word	0x2d782520
3000c63c:	252d7825 	.word	0x252d7825
3000c640:	00000a78 	.word	0x00000a78
3000c644:	2d73255b 	.word	0x2d73255b
3000c648:	54205d57 	.word	0x54205d57
3000c64c:	20736968 	.word	0x20736968
3000c650:	73616c66 	.word	0x73616c66
3000c654:	79742068 	.word	0x79742068
3000c658:	69206570 	.word	0x69206570
3000c65c:	6f6e2073 	.word	0x6f6e2073
3000c660:	75732074 	.word	0x75732074
3000c664:	726f7070 	.word	0x726f7070
3000c668:	21646574 	.word	0x21646574
3000c66c:	0000000a 	.word	0x0000000a
3000c670:	2d73255b 	.word	0x2d73255b
3000c674:	4e205d49 	.word	0x4e205d49
3000c678:	20444e41 	.word	0x20444e41
3000c67c:	203a4449 	.word	0x203a4449
3000c680:	252d7825 	.word	0x252d7825
3000c684:	00000a78 	.word	0x00000a78
3000c688:	00003552 	.word	0x00003552
3000c68c:	00003652 	.word	0x00003652
3000c690:	00003752 	.word	0x00003752
3000c694:	00003852 	.word	0x00003852
3000c698:	00003952 	.word	0x00003952
3000c69c:	00303152 	.word	0x00303152
3000c6a0:	00313152 	.word	0x00313152
3000c6a4:	00003152 	.word	0x00003152
3000c6a8:	00003252 	.word	0x00003252
3000c6ac:	00003352 	.word	0x00003352
3000c6b0:	544f4f42 	.word	0x544f4f42
3000c6b4:	00000000 	.word	0x00000000
3000c6b8:	2d73255b 	.word	0x2d73255b
3000c6bc:	48205d41 	.word	0x48205d41
3000c6c0:	20647261 	.word	0x20647261
3000c6c4:	6c756146 	.word	0x6c756146
3000c6c8:	61502074 	.word	0x61502074
3000c6cc:	20686374 	.word	0x20686374
3000c6d0:	63655328 	.word	0x63655328
3000c6d4:	29657275 	.word	0x29657275
3000c6d8:	00000a0d 	.word	0x00000a0d
3000c6dc:	2d73255b 	.word	0x2d73255b
3000c6e0:	45205d41 	.word	0x45205d41
3000c6e4:	70656378 	.word	0x70656378
3000c6e8:	6e6f6974 	.word	0x6e6f6974
3000c6ec:	6f726620 	.word	0x6f726620
3000c6f0:	6573206d 	.word	0x6573206d
3000c6f4:	65727563 	.word	0x65727563
3000c6f8:	726f7720 	.word	0x726f7720
3000c6fc:	0d21646c 	.word	0x0d21646c
3000c700:	0000000a 	.word	0x0000000a
3000c704:	2d73255b 	.word	0x2d73255b
3000c708:	45205d41 	.word	0x45205d41
3000c70c:	70656378 	.word	0x70656378
3000c710:	6e6f6974 	.word	0x6e6f6974
3000c714:	6f726620 	.word	0x6f726620
3000c718:	6f6e206d 	.word	0x6f6e206d
3000c71c:	65732d6e 	.word	0x65732d6e
3000c720:	65727563 	.word	0x65727563
3000c724:	726f7720 	.word	0x726f7720
3000c728:	0d21646c 	.word	0x0d21646c
3000c72c:	0000000a 	.word	0x0000000a
3000c730:	2d73255b 	.word	0x2d73255b
3000c734:	53205d41 	.word	0x53205d41
3000c738:	20525346 	.word	0x20525346
3000c73c:	30203d20 	.word	0x30203d20
3000c740:	38302578 	.word	0x38302578
3000c744:	000a0d78 	.word	0x000a0d78
3000c748:	2d73255b 	.word	0x2d73255b
3000c74c:	53205d41 	.word	0x53205d41
3000c750:	20524146 	.word	0x20524146
3000c754:	30203d20 	.word	0x30203d20
3000c758:	38302578 	.word	0x38302578
3000c75c:	000a0d78 	.word	0x000a0d78
3000c760:	2d73255b 	.word	0x2d73255b
3000c764:	53205d41 	.word	0x53205d41
3000c768:	52534348 	.word	0x52534348
3000c76c:	30203d20 	.word	0x30203d20
3000c770:	38302578 	.word	0x38302578
3000c774:	000a0d78 	.word	0x000a0d78
3000c778:	3141544f 	.word	0x3141544f
3000c77c:	45535520 	.word	0x45535520
3000c780:	6576202c 	.word	0x6576202c
3000c784:	6f697372 	.word	0x6f697372
3000c788:	25203a6e 	.word	0x25203a6e
3000c78c:	00000a64 	.word	0x00000a64
3000c790:	3241544f 	.word	0x3241544f
3000c794:	45535520 	.word	0x45535520
3000c798:	6576202c 	.word	0x6576202c
3000c79c:	6f697372 	.word	0x6f697372
3000c7a0:	25203a6e 	.word	0x25203a6e
3000c7a4:	00000a64 	.word	0x00000a64
3000c7a8:	68746f42 	.word	0x68746f42
3000c7ac:	41544f20 	.word	0x41544f20
3000c7b0:	616d6920 	.word	0x616d6920
3000c7b4:	76206567 	.word	0x76206567
3000c7b8:	66697265 	.word	0x66697265
3000c7bc:	61662079 	.word	0x61662079
3000c7c0:	64656c69 	.word	0x64656c69
3000c7c4:	0000000a 	.word	0x0000000a
3000c7c8:	2d73255b 	.word	0x2d73255b
3000c7cc:	49205d49 	.word	0x49205d49
3000c7d0:	2074696e 	.word	0x2074696e
3000c7d4:	0a0d4257 	.word	0x0a0d4257
3000c7d8:	00000000 	.word	0x00000000
3000c7dc:	73616c66 	.word	0x73616c66
3000c7e0:	69735f68 	.word	0x69735f68
3000c7e4:	203a657a 	.word	0x203a657a
3000c7e8:	000a4d38 	.word	0x000a4d38
3000c7ec:	73616c66 	.word	0x73616c66
3000c7f0:	69735f68 	.word	0x69735f68
3000c7f4:	203a657a 	.word	0x203a657a
3000c7f8:	0a4d3631 	.word	0x0a4d3631
3000c7fc:	00000000 	.word	0x00000000
3000c800:	73616c66 	.word	0x73616c66
3000c804:	69735f68 	.word	0x69735f68
3000c808:	203a657a 	.word	0x203a657a
3000c80c:	0a4d3233 	.word	0x0a4d3233
3000c810:	00000000 	.word	0x00000000
3000c814:	20315042 	.word	0x20315042
3000c818:	73726576 	.word	0x73726576
3000c81c:	206e6f69 	.word	0x206e6f69
3000c820:	61766e69 	.word	0x61766e69
3000c824:	0a64696c 	.word	0x0a64696c
3000c828:	00000000 	.word	0x00000000
3000c82c:	20315042 	.word	0x20315042
3000c830:	6d726f66 	.word	0x6d726f66
3000c834:	69207461 	.word	0x69207461
3000c838:	6c61766e 	.word	0x6c61766e
3000c83c:	000a6469 	.word	0x000a6469
3000c840:	20315042 	.word	0x20315042
3000c844:	69746361 	.word	0x69746361
3000c848:	69206576 	.word	0x69206576
3000c84c:	7865646e 	.word	0x7865646e
3000c850:	766e6920 	.word	0x766e6920
3000c854:	64696c61 	.word	0x64696c61
3000c858:	0000000a 	.word	0x0000000a
3000c85c:	20315042 	.word	0x20315042
3000c860:	61746164 	.word	0x61746164
3000c864:	6c617620 	.word	0x6c617620
3000c868:	202c6469 	.word	0x202c6469
3000c86c:	73726576 	.word	0x73726576
3000c870:	3a6e6f69 	.word	0x3a6e6f69
3000c874:	000a6425 	.word	0x000a6425
3000c878:	20325042 	.word	0x20325042
3000c87c:	6d726f66 	.word	0x6d726f66
3000c880:	69207461 	.word	0x69207461
3000c884:	6c61766e 	.word	0x6c61766e
3000c888:	000a6469 	.word	0x000a6469
3000c88c:	20315042 	.word	0x20315042
3000c890:	33435243 	.word	0x33435243
3000c894:	616d2032 	.word	0x616d2032
3000c898:	2c686374 	.word	0x2c686374
3000c89c:	74746120 	.word	0x74746120
3000c8a0:	65686361 	.word	0x65686361
3000c8a4:	52432064 	.word	0x52432064
3000c8a8:	30203a43 	.word	0x30203a43
3000c8ac:	2c782578 	.word	0x2c782578
3000c8b0:	6c616320 	.word	0x6c616320
3000c8b4:	616c7563 	.word	0x616c7563
3000c8b8:	20646574 	.word	0x20646574
3000c8bc:	3a435243 	.word	0x3a435243
3000c8c0:	25783020 	.word	0x25783020
3000c8c4:	00000a78 	.word	0x00000a78
3000c8c8:	20315042 	.word	0x20315042
3000c8cc:	33435243 	.word	0x33435243
3000c8d0:	6f6e2032 	.word	0x6f6e2032
3000c8d4:	616d2074 	.word	0x616d2074
3000c8d8:	2c686374 	.word	0x2c686374
3000c8dc:	74746120 	.word	0x74746120
3000c8e0:	65686361 	.word	0x65686361
3000c8e4:	52432064 	.word	0x52432064
3000c8e8:	30203a43 	.word	0x30203a43
3000c8ec:	2c782578 	.word	0x2c782578
3000c8f0:	6c616320 	.word	0x6c616320
3000c8f4:	616c7563 	.word	0x616c7563
3000c8f8:	20646574 	.word	0x20646574
3000c8fc:	3a435243 	.word	0x3a435243
3000c900:	25783020 	.word	0x25783020
3000c904:	00000a78 	.word	0x00000a78
3000c908:	20325042 	.word	0x20325042
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
3000c944:	20325042 	.word	0x20325042
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
3000c984:	20315042 	.word	0x20315042
3000c988:	3161746f 	.word	0x3161746f
3000c98c:	64646120 	.word	0x64646120
3000c990:	73736572 	.word	0x73736572
3000c994:	766e6920 	.word	0x766e6920
3000c998:	64696c61 	.word	0x64696c61
3000c99c:	0000000a 	.word	0x0000000a
3000c9a0:	20315042 	.word	0x20315042
3000c9a4:	3261746f 	.word	0x3261746f
3000c9a8:	64646120 	.word	0x64646120
3000c9ac:	73736572 	.word	0x73736572
3000c9b0:	766e6920 	.word	0x766e6920
3000c9b4:	64696c61 	.word	0x64696c61
3000c9b8:	0000000a 	.word	0x0000000a
3000c9bc:	68746f42 	.word	0x68746f42
3000c9c0:	20504220 	.word	0x20504220
3000c9c4:	20435243 	.word	0x20435243
3000c9c8:	696c6176 	.word	0x696c6176
3000c9cc:	00000a64 	.word	0x00000a64
3000c9d0:	20315042 	.word	0x20315042
3000c9d4:	656c6573 	.word	0x656c6573
3000c9d8:	64657463 	.word	0x64657463
3000c9dc:	0000000a 	.word	0x0000000a
3000c9e0:	20325042 	.word	0x20325042
3000c9e4:	656c6573 	.word	0x656c6573
3000c9e8:	64657463 	.word	0x64657463
3000c9ec:	0000000a 	.word	0x0000000a
3000c9f0:	68746f42 	.word	0x68746f42
3000c9f4:	20504220 	.word	0x20504220
3000c9f8:	61766e69 	.word	0x61766e69
3000c9fc:	0a64696c 	.word	0x0a64696c
3000ca00:	00000000 	.word	0x00000000
3000ca04:	6f636552 	.word	0x6f636552
3000ca08:	79726576 	.word	0x79726576
3000ca0c:	61747320 	.word	0x61747320
3000ca10:	000a7472 	.word	0x000a7472
3000ca14:	69746341 	.word	0x69746341
3000ca18:	49206576 	.word	0x49206576
3000ca1c:	7865646e 	.word	0x7865646e
3000ca20:	6425203a 	.word	0x6425203a
3000ca24:	0000000a 	.word	0x0000000a
3000ca28:	3141544f 	.word	0x3141544f
3000ca2c:	766e6920 	.word	0x766e6920
3000ca30:	64696c61 	.word	0x64696c61
3000ca34:	0000000a 	.word	0x0000000a
3000ca38:	3241544f 	.word	0x3241544f
3000ca3c:	766e6920 	.word	0x766e6920
3000ca40:	64696c61 	.word	0x64696c61
3000ca44:	0000000a 	.word	0x0000000a
3000ca48:	3141544f 	.word	0x3141544f
3000ca4c:	43524320 	.word	0x43524320
3000ca50:	6d203233 	.word	0x6d203233
3000ca54:	68637461 	.word	0x68637461
3000ca58:	7461202c 	.word	0x7461202c
3000ca5c:	68636174 	.word	0x68636174
3000ca60:	43206465 	.word	0x43206465
3000ca64:	203a4352 	.word	0x203a4352
3000ca68:	78257830 	.word	0x78257830
3000ca6c:	6163202c 	.word	0x6163202c
3000ca70:	6c75636c 	.word	0x6c75636c
3000ca74:	64657461 	.word	0x64657461
3000ca78:	43524320 	.word	0x43524320
3000ca7c:	7830203a 	.word	0x7830203a
3000ca80:	000a7825 	.word	0x000a7825
3000ca84:	3141544f 	.word	0x3141544f
3000ca88:	43524320 	.word	0x43524320
3000ca8c:	6e203233 	.word	0x6e203233
3000ca90:	6d20746f 	.word	0x6d20746f
3000ca94:	68637461 	.word	0x68637461
3000ca98:	7461202c 	.word	0x7461202c
3000ca9c:	68636174 	.word	0x68636174
3000caa0:	43206465 	.word	0x43206465
3000caa4:	203a4352 	.word	0x203a4352
3000caa8:	78257830 	.word	0x78257830
3000caac:	6163202c 	.word	0x6163202c
3000cab0:	6c75636c 	.word	0x6c75636c
3000cab4:	64657461 	.word	0x64657461
3000cab8:	43524320 	.word	0x43524320
3000cabc:	7830203a 	.word	0x7830203a
3000cac0:	202c7825 	.word	0x202c7825
3000cac4:	73617265 	.word	0x73617265
3000cac8:	544f2065 	.word	0x544f2065
3000cacc:	000a3141 	.word	0x000a3141
3000cad0:	3241544f 	.word	0x3241544f
3000cad4:	43524320 	.word	0x43524320
3000cad8:	6d203233 	.word	0x6d203233
3000cadc:	68637461 	.word	0x68637461
3000cae0:	7461202c 	.word	0x7461202c
3000cae4:	68636174 	.word	0x68636174
3000cae8:	43206465 	.word	0x43206465
3000caec:	203a4352 	.word	0x203a4352
3000caf0:	78257830 	.word	0x78257830
3000caf4:	6163202c 	.word	0x6163202c
3000caf8:	6c75636c 	.word	0x6c75636c
3000cafc:	64657461 	.word	0x64657461
3000cb00:	43524320 	.word	0x43524320
3000cb04:	7830203a 	.word	0x7830203a
3000cb08:	000a7825 	.word	0x000a7825
3000cb0c:	3241544f 	.word	0x3241544f
3000cb10:	43524320 	.word	0x43524320
3000cb14:	6e203233 	.word	0x6e203233
3000cb18:	6d20746f 	.word	0x6d20746f
3000cb1c:	68637461 	.word	0x68637461
3000cb20:	7461202c 	.word	0x7461202c
3000cb24:	68636174 	.word	0x68636174
3000cb28:	43206465 	.word	0x43206465
3000cb2c:	203a4352 	.word	0x203a4352
3000cb30:	78257830 	.word	0x78257830
3000cb34:	6163202c 	.word	0x6163202c
3000cb38:	6c75636c 	.word	0x6c75636c
3000cb3c:	64657461 	.word	0x64657461
3000cb40:	43524320 	.word	0x43524320
3000cb44:	7830203a 	.word	0x7830203a
3000cb48:	202c7825 	.word	0x202c7825
3000cb4c:	73617265 	.word	0x73617265
3000cb50:	544f2065 	.word	0x544f2065
3000cb54:	000a3241 	.word	0x000a3241
3000cb58:	69746341 	.word	0x69746341
3000cb5c:	69206576 	.word	0x69206576
3000cb60:	7865646e 	.word	0x7865646e
3000cb64:	41544f20 	.word	0x41544f20
3000cb68:	766e6920 	.word	0x766e6920
3000cb6c:	64696c61 	.word	0x64696c61
3000cb70:	6572202c 	.word	0x6572202c
3000cb74:	74697277 	.word	0x74697277
3000cb78:	63612065 	.word	0x63612065
3000cb7c:	65766974 	.word	0x65766974
3000cb80:	646e6920 	.word	0x646e6920
3000cb84:	74207865 	.word	0x74207865
3000cb88:	6425206f 	.word	0x6425206f
3000cb8c:	0000000a 	.word	0x0000000a
3000cb90:	206c6c41 	.word	0x206c6c41
3000cb94:	2041544f 	.word	0x2041544f
3000cb98:	72646461 	.word	0x72646461
3000cb9c:	65737365 	.word	0x65737365
3000cba0:	6f6e2073 	.word	0x6f6e2073
3000cba4:	61762074 	.word	0x61762074
3000cba8:	2c64696c 	.word	0x2c64696c
3000cbac:	63657220 	.word	0x63657220
3000cbb0:	7265766f 	.word	0x7265766f
3000cbb4:	61662079 	.word	0x61662079
3000cbb8:	000a6c69 	.word	0x000a6c69
3000cbbc:	20325042 	.word	0x20325042
3000cbc0:	3261746f 	.word	0x3261746f
3000cbc4:	64646120 	.word	0x64646120
3000cbc8:	73736572 	.word	0x73736572
3000cbcc:	766e6920 	.word	0x766e6920
3000cbd0:	64696c61 	.word	0x64696c61
3000cbd4:	0000000a 	.word	0x0000000a
3000cbd8:	20325042 	.word	0x20325042
3000cbdc:	73726576 	.word	0x73726576
3000cbe0:	206e6f69 	.word	0x206e6f69
3000cbe4:	61766e69 	.word	0x61766e69
3000cbe8:	0a64696c 	.word	0x0a64696c
3000cbec:	00000000 	.word	0x00000000
3000cbf0:	20325042 	.word	0x20325042
3000cbf4:	69746361 	.word	0x69746361
3000cbf8:	69206576 	.word	0x69206576
3000cbfc:	7865646e 	.word	0x7865646e
3000cc00:	766e6920 	.word	0x766e6920
3000cc04:	64696c61 	.word	0x64696c61
3000cc08:	0000000a 	.word	0x0000000a
3000cc0c:	20325042 	.word	0x20325042
3000cc10:	3161746f 	.word	0x3161746f
3000cc14:	64646120 	.word	0x64646120
3000cc18:	73736572 	.word	0x73736572
3000cc1c:	766e6920 	.word	0x766e6920
3000cc20:	64696c61 	.word	0x64696c61
3000cc24:	0000000a 	.word	0x0000000a
3000cc28:	20325042 	.word	0x20325042
3000cc2c:	61746164 	.word	0x61746164
3000cc30:	6c617620 	.word	0x6c617620
3000cc34:	202c6469 	.word	0x202c6469
3000cc38:	73726576 	.word	0x73726576
3000cc3c:	3a6e6f69 	.word	0x3a6e6f69
3000cc40:	000a6425 	.word	0x000a6425
3000cc44:	2d73255b 	.word	0x2d73255b
3000cc48:	44205d49 	.word	0x44205d49
3000cc4c:	62617369 	.word	0x62617369
3000cc50:	4b20656c 	.word	0x4b20656c
3000cc54:	4220304d 	.word	0x4220304d
3000cc58:	20746f6f 	.word	0x20746f6f
3000cc5c:	206e6920 	.word	0x206e6920
3000cc60:	74736554 	.word	0x74736554
3000cc64:	646f6d20 	.word	0x646f6d20
3000cc68:	000a0d65 	.word	0x000a0d65
3000cc6c:	2d73255b 	.word	0x2d73255b
3000cc70:	42205d49 	.word	0x42205d49
3000cc74:	73617079 	.word	0x73617079
3000cc78:	544f2073 	.word	0x544f2073
3000cc7c:	75612050 	.word	0x75612050
3000cc80:	6f6c6f74 	.word	0x6f6c6f74
3000cc84:	0a0d6461 	.word	0x0a0d6461
3000cc88:	00000000 	.word	0x00000000
3000cc8c:	2d73255b 	.word	0x2d73255b
3000cc90:	4e205d49 	.word	0x4e205d49
3000cc94:	72462050 	.word	0x72462050
3000cc98:	25207165 	.word	0x25207165
3000cc9c:	484d2064 	.word	0x484d2064
3000cca0:	00000a7a 	.word	0x00000a7a
3000cca4:	2d73255b 	.word	0x2d73255b
3000cca8:	41205d49 	.word	0x41205d49
3000ccac:	72462050 	.word	0x72462050
3000ccb0:	25207165 	.word	0x25207165
3000ccb4:	484d2064 	.word	0x484d2064
3000ccb8:	00000a7a 	.word	0x00000a7a
3000ccbc:	7361700a 	.word	0x7361700a
3000ccc0:	726f7773 	.word	0x726f7773
3000ccc4:	65762064 	.word	0x65762064
3000ccc8:	79666972 	.word	0x79666972
3000cccc:	69616620 	.word	0x69616620
3000ccd0:	00000a6c 	.word	0x00000a6c
3000ccd4:	7361700a 	.word	0x7361700a
3000ccd8:	726f7773 	.word	0x726f7773
3000ccdc:	65762064 	.word	0x65762064
3000cce0:	79666972 	.word	0x79666972
3000cce4:	63757320 	.word	0x63757320
3000cce8:	73736563 	.word	0x73736563
3000ccec:	0000000a 	.word	0x0000000a
3000ccf0:	31474d49 	.word	0x31474d49
3000ccf4:	4f425320 	.word	0x4f425320
3000ccf8:	4f20544f 	.word	0x4f20544f
3000ccfc:	000a4646 	.word	0x000a4646
3000cd00:	31474d49 	.word	0x31474d49
3000cd04:	4f425320 	.word	0x4f425320
3000cd08:	5320544f 	.word	0x5320544f
3000cd0c:	45434355 	.word	0x45434355
3000cd10:	000a5353 	.word	0x000a5353
3000cd14:	746f6f62 	.word	0x746f6f62
3000cd18:	64616f6c 	.word	0x64616f6c
3000cd1c:	765f7265 	.word	0x765f7265
3000cd20:	69737265 	.word	0x69737265
3000cd24:	25206e6f 	.word	0x25206e6f
3000cd28:	00000a73 	.word	0x00000a73
3000cd2c:	444f4d5b 	.word	0x444f4d5b
3000cd30:	5f454c55 	.word	0x5f454c55
3000cd34:	544f4f42 	.word	0x544f4f42
3000cd38:	56454c2d 	.word	0x56454c2d
3000cd3c:	495f4c45 	.word	0x495f4c45
3000cd40:	5d4f464e 	.word	0x5d4f464e
3000cd44:	474d493a 	.word	0x474d493a
3000cd48:	4e452031 	.word	0x4e452031
3000cd4c:	20524554 	.word	0x20524554
3000cd50:	3a50534d 	.word	0x3a50534d
3000cd54:	3830255b 	.word	0x3830255b
3000cd58:	000a5d78 	.word	0x000a5d78
3000cd5c:	444f4d5b 	.word	0x444f4d5b
3000cd60:	5f454c55 	.word	0x5f454c55
3000cd64:	544f4f42 	.word	0x544f4f42
3000cd68:	56454c2d 	.word	0x56454c2d
3000cd6c:	495f4c45 	.word	0x495f4c45
3000cd70:	5d4f464e 	.word	0x5d4f464e
3000cd74:	474d493a 	.word	0x474d493a
3000cd78:	45532031 	.word	0x45532031
3000cd7c:	45525543 	.word	0x45525543
3000cd80:	41545320 	.word	0x41545320
3000cd84:	203a4554 	.word	0x203a4554
3000cd88:	000a6425 	.word	0x000a6425
3000cd8c:	2d73255b 	.word	0x2d73255b
3000cd90:	49205d49 	.word	0x49205d49
3000cd94:	2074696e 	.word	0x2074696e
3000cd98:	41525350 	.word	0x41525350
3000cd9c:	000a0d4d 	.word	0x000a0d4d
3000cda0:	2d73255b 	.word	0x2d73255b
3000cda4:	73205d57 	.word	0x73205d57
3000cda8:	70207465 	.word	0x70207465
3000cdac:	66206d66 	.word	0x66206d66
3000cdb0:	0d6c6961 	.word	0x0d6c6961
3000cdb4:	0000000a 	.word	0x0000000a
3000cdb8:	2d73255b 	.word	0x2d73255b
3000cdbc:	45205d45 	.word	0x45205d45
3000cdc0:	524f5252 	.word	0x524f5252
3000cdc4:	53202121 	.word	0x53202121
3000cdc8:	6c756f68 	.word	0x6c756f68
3000cdcc:	6f4e2064 	.word	0x6f4e2064
3000cdd0:	6e652074 	.word	0x6e652074
3000cdd4:	656c6261 	.word	0x656c6261
3000cdd8:	6d654d20 	.word	0x6d654d20
3000cddc:	5f727753 	.word	0x5f727753
3000cde0:	796c6e4f 	.word	0x796c6e4f
3000cde4:	206e6920 	.word	0x206e6920
3000cde8:	20524444 	.word	0x20524444
3000cdec:	70696843 	.word	0x70696843
3000cdf0:	0d212121 	.word	0x0d212121
3000cdf4:	0000000a 	.word	0x0000000a
3000cdf8:	2d73255b 	.word	0x2d73255b
3000cdfc:	49205d49 	.word	0x49205d49
3000ce00:	2074696e 	.word	0x2074696e
3000ce04:	0d524444 	.word	0x0d524444
3000ce08:	0000000a 	.word	0x0000000a
3000ce0c:	55676f4c 	.word	0x55676f4c
3000ce10:	20747261 	.word	0x20747261
3000ce14:	64756142 	.word	0x64756142
3000ce18:	65746172 	.word	0x65746172
3000ce1c:	6425203a 	.word	0x6425203a
3000ce20:	0000000a 	.word	0x0000000a
3000ce24:	4848530a 	.word	0x4848530a
3000ce28:	4946544f 	.word	0x4946544f
3000ce2c:	6f6e2058 	.word	0x6f6e2058
3000ce30:	65722074 	.word	0x65722074
3000ce34:	76696563 	.word	0x76696563
3000ce38:	000a6465 	.word	0x000a6465
3000ce3c:	726f4e0a 	.word	0x726f4e0a
3000ce40:	206c616d 	.word	0x206c616d
3000ce44:	746f6f62 	.word	0x746f6f62
3000ce48:	0000000a 	.word	0x0000000a
3000ce4c:	5f6e6962 	.word	0x5f6e6962
3000ce50:	313e6f6e 	.word	0x313e6f6e
3000ce54:	3d202c30 	.word	0x3d202c30
3000ce58:	0a642520 	.word	0x0a642520
3000ce5c:	00000000 	.word	0x00000000
3000ce60:	6e206164 	.word	0x6e206164
3000ce64:	7220746f 	.word	0x7220746f
3000ce68:	69656365 	.word	0x69656365
3000ce6c:	0a646576 	.word	0x0a646576
3000ce70:	00000000 	.word	0x00000000
3000ce74:	746f680a 	.word	0x746f680a
3000ce78:	20786966 	.word	0x20786966
3000ce7c:	656e6f64 	.word	0x656e6f64
3000ce80:	202e2e2e 	.word	0x202e2e2e
3000ce84:	6f626552 	.word	0x6f626552
3000ce88:	6e69746f 	.word	0x6e69746f
3000ce8c:	0a2e2e67 	.word	0x0a2e2e67
3000ce90:	00000000 	.word	0x00000000
3000ce94:	20304d4b 	.word	0x20304d4b
3000ce98:	20504958 	.word	0x20504958
3000ce9c:	00474d49 	.word	0x00474d49
3000cea0:	20304d4b 	.word	0x20304d4b
3000cea4:	4d415253 	.word	0x4d415253
3000cea8:	00000000 	.word	0x00000000
3000ceac:	20304d4b 	.word	0x20304d4b
3000ceb0:	4d415244 	.word	0x4d415244
3000ceb4:	00000000 	.word	0x00000000
3000ceb8:	20344d4b 	.word	0x20344d4b
3000cebc:	20504958 	.word	0x20504958
3000cec0:	00474d49 	.word	0x00474d49
3000cec4:	20344d4b 	.word	0x20344d4b
3000cec8:	4d415253 	.word	0x4d415253
3000cecc:	00000000 	.word	0x00000000
3000ced0:	20344d4b 	.word	0x20344d4b
3000ced4:	4d415244 	.word	0x4d415244
3000ced8:	00000000 	.word	0x00000000
3000cedc:	58205041 	.word	0x58205041
3000cee0:	49205049 	.word	0x49205049
3000cee4:	0000474d 	.word	0x0000474d
3000cee8:	42205041 	.word	0x42205041
3000ceec:	5320314c 	.word	0x5320314c
3000cef0:	004d4152 	.word	0x004d4152
3000cef4:	42205041 	.word	0x42205041
3000cef8:	4420314c 	.word	0x4420314c
3000cefc:	004d4152 	.word	0x004d4152
3000cf00:	46205041 	.word	0x46205041
3000cf04:	00005049 	.word	0x00005049
3000cf08:	2d73255b 	.word	0x2d73255b
3000cf0c:	25205d49 	.word	0x25205d49
3000cf10:	6e492073 	.word	0x6e492073
3000cf14:	696c6176 	.word	0x696c6176
3000cf18:	00000a64 	.word	0x00000a64
3000cf1c:	2d73255b 	.word	0x2d73255b
3000cf20:	25205d49 	.word	0x25205d49
3000cf24:	30255b73 	.word	0x30255b73
3000cf28:	253a7838 	.word	0x253a7838
3000cf2c:	000a5d78 	.word	0x000a5d78
3000cf30:	2d73255b 	.word	0x2d73255b
3000cf34:	52205d49 	.word	0x52205d49
3000cf38:	45205044 	.word	0x45205044
3000cf3c:	00000a4e 	.word	0x00000a4e
3000cf40:	2d73255b 	.word	0x2d73255b
3000cf44:	46205d49 	.word	0x46205d49
3000cf48:	206c6961 	.word	0x206c6961
3000cf4c:	6c206f74 	.word	0x6c206f74
3000cf50:	2064616f 	.word	0x2064616f
3000cf54:	20504452 	.word	0x20504452
3000cf58:	67616d69 	.word	0x67616d69
3000cf5c:	000a2165 	.word	0x000a2165
3000cf60:	2d73255b 	.word	0x2d73255b
3000cf64:	52205d49 	.word	0x52205d49
3000cf68:	4f205044 	.word	0x4f205044
3000cf6c:	000a4646 	.word	0x000a4646
3000cf70:	2d73255b 	.word	0x2d73255b
3000cf74:	49205d49 	.word	0x49205d49
3000cf78:	2032474d 	.word	0x2032474d
3000cf7c:	2046544f 	.word	0x2046544f
3000cf80:	000a4e45 	.word	0x000a4e45
3000cf84:	2d73255b 	.word	0x2d73255b
3000cf88:	4f205d45 	.word	0x4f205d45
3000cf8c:	4d204654 	.word	0x4d204654
3000cf90:	2065646f 	.word	0x2065646f
3000cf94:	6f727265 	.word	0x6f727265
3000cf98:	000a0d72 	.word	0x000a0d72
3000cf9c:	494c4156 	.word	0x494c4156
3000cfa0:	00000044 	.word	0x00000044
3000cfa4:	41564e49 	.word	0x41564e49
3000cfa8:	0044494c 	.word	0x0044494c
3000cfac:	74726563 	.word	0x74726563
3000cfb0:	63696669 	.word	0x63696669
3000cfb4:	20657461 	.word	0x20657461
3000cfb8:	20434345 	.word	0x20434345
3000cfbc:	69726576 	.word	0x69726576
3000cfc0:	66207966 	.word	0x66207966
3000cfc4:	2c6c6961 	.word	0x2c6c6961
3000cfc8:	6c6f7220 	.word	0x6c6f7220
3000cfcc:	6361626c 	.word	0x6361626c
3000cfd0:	000a0d6b 	.word	0x000a0d6b
3000cfd4:	444f4d5b 	.word	0x444f4d5b
3000cfd8:	5f454c55 	.word	0x5f454c55
3000cfdc:	544f4f42 	.word	0x544f4f42
3000cfe0:	56454c2d 	.word	0x56454c2d
3000cfe4:	495f4c45 	.word	0x495f4c45
3000cfe8:	5d4f464e 	.word	0x5d4f464e
3000cfec:	474d493a 	.word	0x474d493a
3000cff0:	544f2832 	.word	0x544f2832
3000cff4:	29642541 	.word	0x29642541
3000cff8:	2c732520 	.word	0x2c732520
3000cffc:	74657220 	.word	0x74657220
3000d000:	6425203a 	.word	0x6425203a
3000d004:	0000000a 	.word	0x0000000a
3000d008:	32474d49 	.word	0x32474d49
3000d00c:	43434520 	.word	0x43434520
3000d010:	72657620 	.word	0x72657620
3000d014:	20796669 	.word	0x20796669
3000d018:	6c696166 	.word	0x6c696166
3000d01c:	6f72202c 	.word	0x6f72202c
3000d020:	61626c6c 	.word	0x61626c6c
3000d024:	0a0d6b63 	.word	0x0a0d6b63
3000d028:	00000000 	.word	0x00000000
3000d02c:	5f41544f 	.word	0x5f41544f
3000d030:	45205041 	.word	0x45205041
3000d034:	76204343 	.word	0x76204343
3000d038:	66697265 	.word	0x66697265
3000d03c:	61662079 	.word	0x61662079
3000d040:	202c6c69 	.word	0x202c6c69
3000d044:	6c6c6f72 	.word	0x6c6c6f72
3000d048:	6b636162 	.word	0x6b636162
3000d04c:	00000a0d 	.word	0x00000a0d
3000d050:	64616f4c 	.word	0x64616f4c
3000d054:	41544f20 	.word	0x41544f20
3000d058:	2050415f 	.word	0x2050415f
3000d05c:	67616d69 	.word	0x67616d69
3000d060:	61662065 	.word	0x61662065
3000d064:	202c6c69 	.word	0x202c6c69
3000d068:	6c6c6f72 	.word	0x6c6c6f72
3000d06c:	6b636162 	.word	0x6b636162
3000d070:	00000a0d 	.word	0x00000a0d
3000d074:	444f4d5b 	.word	0x444f4d5b
3000d078:	5f454c55 	.word	0x5f454c55
3000d07c:	544f4f42 	.word	0x544f4f42
3000d080:	56454c2d 	.word	0x56454c2d
3000d084:	455f4c45 	.word	0x455f4c45
3000d088:	524f5252 	.word	0x524f5252
3000d08c:	61463a5d 	.word	0x61463a5d
3000d090:	74206c69 	.word	0x74206c69
3000d094:	6f6c206f 	.word	0x6f6c206f
3000d098:	41206461 	.word	0x41206461
3000d09c:	6d692050 	.word	0x6d692050
3000d0a0:	21656761 	.word	0x21656761
3000d0a4:	0000000a 	.word	0x0000000a
3000d0a8:	20344d4b 	.word	0x20344d4b
3000d0ac:	33474d49 	.word	0x33474d49
3000d0b0:	00000000 	.word	0x00000000
3000d0b4:	20344d4b 	.word	0x20344d4b
3000d0b8:	0043534e 	.word	0x0043534e
3000d0bc:	31474d49 	.word	0x31474d49
3000d0c0:	00000000 	.word	0x00000000
3000d0c4:	32474d49 	.word	0x32474d49
3000d0c8:	00000000 	.word	0x00000000
3000d0cc:	42205041 	.word	0x42205041
3000d0d0:	4626314c 	.word	0x4626314c
3000d0d4:	00005049 	.word	0x00005049
3000d0d8:	444f4d5b 	.word	0x444f4d5b
3000d0dc:	5f454c55 	.word	0x5f454c55
3000d0e0:	544f4f42 	.word	0x544f4f42
3000d0e4:	56454c2d 	.word	0x56454c2d
3000d0e8:	495f4c45 	.word	0x495f4c45
3000d0ec:	5d4f464e 	.word	0x5d4f464e
3000d0f0:	5b73253a 	.word	0x5b73253a
3000d0f4:	78383025 	.word	0x78383025
3000d0f8:	5d78253a 	.word	0x5d78253a
3000d0fc:	0000000a 	.word	0x0000000a
3000d100:	20504452 	.word	0x20504452
3000d104:	4c494146 	.word	0x4c494146
3000d108:	0000000a 	.word	0x0000000a
3000d10c:	20504452 	.word	0x20504452
3000d110:	43435553 	.word	0x43435553
3000d114:	0a535345 	.word	0x0a535345
3000d118:	00000000 	.word	0x00000000
3000d11c:	059e4eb3 	.word	0x059e4eb3
3000d120:	4b11ea39 	.word	0x4b11ea39
3000d124:	2a1b920a 	.word	0x2a1b920a
3000d128:	29939cee 	.word	0x29939cee
3000d12c:	00000000 	.word	0x00000000
3000d130:	42205041 	.word	0x42205041
3000d134:	0032334c 	.word	0x0032334c
3000d138:	2d73255b 	.word	0x2d73255b
3000d13c:	25205d49 	.word	0x25205d49
3000d140:	45562073 	.word	0x45562073
3000d144:	59464952 	.word	0x59464952
3000d148:	53415020 	.word	0x53415020
3000d14c:	00000a53 	.word	0x00000a53
3000d150:	2d73255b 	.word	0x2d73255b
3000d154:	25205d45 	.word	0x25205d45
3000d158:	45562073 	.word	0x45562073
3000d15c:	59464952 	.word	0x59464952
3000d160:	49414620 	.word	0x49414620
3000d164:	72202c4c 	.word	0x72202c4c
3000d168:	3d207465 	.word	0x3d207465
3000d16c:	0a642520 	.word	0x0a642520
3000d170:	00000000 	.word	0x00000000
3000d174:	2d73255b 	.word	0x2d73255b
3000d178:	43205d45 	.word	0x43205d45
3000d17c:	69747265 	.word	0x69747265
3000d180:	61636966 	.word	0x61636966
3000d184:	56206574 	.word	0x56206574
3000d188:	46495245 	.word	0x46495245
3000d18c:	41462059 	.word	0x41462059
3000d190:	202c4c49 	.word	0x202c4c49
3000d194:	20746572 	.word	0x20746572
3000d198:	6425203d 	.word	0x6425203d
3000d19c:	0000000a 	.word	0x0000000a
3000d1a0:	6c630a0d 	.word	0x6c630a0d
3000d1a4:	2065736f 	.word	0x2065736f
3000d1a8:	646f4d79 	.word	0x646f4d79
3000d1ac:	54206d65 	.word	0x54206d65
3000d1b0:	736e6172 	.word	0x736e6172
3000d1b4:	20726566 	.word	0x20726566
3000d1b8:	0d2e2e2e 	.word	0x0d2e2e2e
3000d1bc:	0000000a 	.word	0x0000000a
3000d1c0:	50494843 	.word	0x50494843
3000d1c4:	4f464e49 	.word	0x4f464e49
3000d1c8:	00000000 	.word	0x00000000
3000d1cc:	2d73255b 	.word	0x2d73255b
3000d1d0:	49205d45 	.word	0x49205d45
3000d1d4:	6c61766e 	.word	0x6c61766e
3000d1d8:	43206469 	.word	0x43206469
3000d1dc:	696e6968 	.word	0x696e6968
3000d1e0:	216f666e 	.word	0x216f666e
3000d1e4:	65684320 	.word	0x65684320
3000d1e8:	4f206b63 	.word	0x4f206b63
3000d1ec:	000a5054 	.word	0x000a5054
3000d1f0:	2d73255b 	.word	0x2d73255b
3000d1f4:	50205d57 	.word	0x50205d57
3000d1f8:	7361656c 	.word	0x7361656c
3000d1fc:	72702065 	.word	0x72702065
3000d200:	6172676f 	.word	0x6172676f
3000d204:	6863206d 	.word	0x6863206d
3000d208:	6e697069 	.word	0x6e697069
3000d20c:	69206f66 	.word	0x69206f66
3000d210:	544f206e 	.word	0x544f206e
3000d214:	0a212050 	.word	0x0a212050
3000d218:	00000000 	.word	0x00000000

3000d21c <Memory_Info>:
3000d21c:	03e00007 00060004 010202c0 02010100     ................
3000d22c:	02000102 00040302 02040400 01010102     ................
3000d23c:	01020221 02240401 03000102 00030103     !.....$.........
3000d24c:	01050500 06000002 01020206 01070700     ................
3000d25c:	08000002 01020208 02280801 09000102     ..........(.....
3000d26c:	00040309 03290901 0a000004 0003010a     ......).........
3000d27c:	012a0a01 0b000003 0102020b 020c0c00     ..*.............
3000d28c:	0d000102 0004030d 010e0e00 ffff0003     ................
3000d29c:	ffffffff 50524444 00005948 2d73255b     ....DDRPHY..[%s-
3000d2ac:	44205d45 204d4152 6e207369 4320746f     E] DRAM is not C
3000d2bc:	62696c61 6f696172 000a0d6e              alibraion...

3000d2c8 <ddrphy_tx_scan>:
3000d2c8:	00000000 00080000 00080808 00000000     ................
3000d2d8:	20000000 33333333 000c0022 00000000     ... 3333".......
3000d2e8:	07060012 000f0707 080f0f0f 00000008     ................
3000d2f8:	20000001 22222222 000c0012 34000000     ... """".......4
3000d308:	0b0b0016 00130b0b 0e131313 0000000e     ................
3000d318:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d328:	0e0e0018 00170e0e 0e171717 0000000e     ................
3000d338:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d348:	0a0a0018 00130a0a 0e131313 0000000e     ................
3000d358:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d368:	0e0e001a 00170e0e 12171717 00000012     ................
3000d378:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d388:	0f0f0016 00180f0f 0e181818 0000000e     ................
3000d398:	200003c1 22222222 000c0012 34000000     ... """".......4
3000d3a8:	0f0f001b 00180f0f 13181818 00000013     ................
3000d3b8:	20000fc1 22222222 000c0012 34000000     ... """".......4
3000d3c8:	1110001b 00191111 13191919 00000013     ................
3000d3d8:	20000ffd 22222222 000c0012 34000000     ... """".......4
3000d3e8:	1313001c 001c1313 121c1c1c 00000012     ................
3000d3f8:	20000ffd 22222222 000c0012 34000000     ... """".......4

3000d408 <ddrphy_zq_rx_scan>:
	...
3000d410:	26262626 26262626 0303030f 0e141414     &&&&&&&&........
3000d420:	140e0e0e 261f362e 261c3029 0302020f     .....6.&)0.&....
3000d430:	0e141414 140e0e0e 261f362e 261c3029     .........6.&)0.&
3000d440:	0302020f 0a0c0b0b 0b0b0a0a 26172721     ............!'.&
3000d450:	2615231e 04020206 0a0e0c0c 0c0b0a0a     .#.&............
3000d460:	261e362c 26162620 04020203 0e141414     ,6.& &.&........
3000d470:	140e0e0e 26243c34 261c3029 0303030f     ....4<$&)0.&....
3000d480:	090b0a0a 0a0a0909 2615231e 2613201b     .........#.&. .&
3000d490:	04020204 0b0f0c0c 0c0b0b0b 26233a30     ............0:#&
3000d4a0:	26182722 04020205 0e141414 140e0e0e     "'.&............
3000d4b0:	26243c34 261b2f28 0303030f 0a0c0c0c     4<$&(/.&........
3000d4c0:	0c0a0a0a 2616251f 2614221d 04020205     .....%.&.".&....

3000d4d0 <Flash_AVL>:
3000d4d0:	000000ef 000000ff 00000000 000043fc     .............C..
3000d4e0:	00000000 000000a1 000000ff 00000000     ................
3000d4f0:	0000fffc 00000000 0000000b 000000ff     ................
3000d500:	00000000 000043fc 00000000 0000000e     .....C..........
3000d510:	000000ff 00000000 000043fc 00000000     .........C......
3000d520:	000000c8 000000ff 00000001 000043fc     .............C..
3000d530:	00000000 000028c2 0000ffff 00000005     .....(..........
3000d540:	000200fc 00000000 000000c2 000000ff     ................
3000d550:	00000002 000000fc 00000000 00000068     ............h...
3000d560:	000000ff 00000002 000000fc 00000000     ................
3000d570:	00000051 000000ff 00000002 000000fc     Q...............
3000d580:	00000000 0000001c 000000ff 00000003     ................
3000d590:	000000fc 00000000 00000020 000000ff     ........ .......
3000d5a0:	00000004 000000fc 00000000 00000000     ................
3000d5b0:	000000ff 000000fe ffffffff 300085f9     ...............0
3000d5c0:	000000ff ffffffff 000000ff ffffffff     ................
3000d5d0:	00000000                                ....

3000d5d4 <Flash_ReadMode>:
3000d5d4:	                                         ..

3000d5d6 <Flash_Speed>:
3000d5d6:	                                         ..

3000d5d8 <NAND_AVL>:
3000d5d8:	000000ef 000000ff 00000000 ffffffff     ................
3000d5e8:	00000000 000000c8 000000ff 00000001     ................
3000d5f8:	ffffffff 00000000 000000e5 000000ff     ................
3000d608:	00000001 ffffffff 00000000 000000c2     ................
3000d618:	000000ff 00000002 ffffffff 00000000     ................
3000d628:	0000002c 000000ff 00000004 ffffffff     ,...............
	...
3000d640:	000000ff 000000fe ffffffff 30008659     ............Y..0
3000d650:	000000ff ffffffff 000000ff ffffffff     ................
3000d660:	00000000 3000c568 3000c56c 3000c570     ....h..0l..0p..0
3000d670:	3000c574 3000c578                       t..0x..0

3000d678 <__FUNCTION__.0>:
3000d678:	646e616e 7465675f 6e65765f 00726f64     nand_get_vendor.

3000d688 <__FUNCTION__.1>:
3000d688:	73616c66 65675f68 65765f74 726f646e     flash_get_vendor
3000d698:	00000000 444e414e 53414c46 00000048     ....NANDFLASH...
3000d6a8:	2d73255b 42205d45 30206b6c 20782578     [%s-E] Blk 0x%x 
3000d6b8:	42207369 000a2142 2d73255b 52205d49     is BB!..[%s-I] R
3000d6c8:	20646165 65676150 25783020 666f2078     ead Page 0x%x of
3000d6d8:	6f6c4220 30206b63 20782578 2079614d      Block 0x%x May 
3000d6e8:	6c696146 7473202c 73757461 25783020     Fail, status 0x%
3000d6f8:	000a2178 0050544f 2d73255b 4f205d45     x!..OTP.[%s-E] O
3000d708:	525f5054 38646165 69614620 7825206c     TP_Read8 Fail %x
3000d718:	00000a20                                 ...

3000d71c <__FUNCTION__.1>:
3000d71c:	5f4c4c50 435f5041 65536b6c 00000074     PLL_AP_ClkSet...
3000d72c:	41525350 0000004d 2d73255b 50205d49     PSRAM...[%s-I] P
3000d73c:	4d415253 72744320 4c43206c 25203a4b     SRAM Ctrl CLK: %
3000d74c:	7a482064 00000a20 2d73255b 43205d49     d Hz ...[%s-I] C
3000d75c:	6d4e6c61 3d206e69 20782520 4e6c6143     alNmin = %x CalN
3000d76c:	2078616d 7825203d 6e695720 53776f64     max = %x WindowS
3000d77c:	20657a69 7825203d 61687020 203a6573     ize = %x phase: 
3000d78c:	0a207825 00000000 504d4f52 48435441     %x .....ROMPATCH
3000d79c:	00000000 2d73255b 4e205d45 6874206f     ....[%s-E] No th
3000d7ac:	61207369 5f687475 65707974 29642528     is auth_type(%d)
3000d7bc:	726f6620 41485320 00000a32 2d73255b      for SHA2...[%s-
3000d7cc:	69205d57 255b7172 70205d64 726f6972     W] irq[%d] prior
3000d7dc:	20797469 73206425 6c6c6168 203d3c20     ity %d shall <= 
3000d7ec:	000a6425                                %d..

3000d7f0 <__FUNCTION__.0>:
3000d7f0:	5f717269 5f746573 6f697270 79746972     irq_set_priority
3000d800:	00000000                                ....

3000d804 <__FUNCTION__.1>:
3000d804:	50495352 46544f5f 616e455f 00656c62     RSIP_OTF_Enable.

3000d814 <__FUNCTION__.3>:
3000d814:	5f6c7472 70797263 685f6f74 5f63616d     rtl_crypto_hmac_
3000d824:	32616873 696e695f 00000074              sha2_init...

3000d830 <__FUNCTION__.4>:
3000d830:	50595243 535f4f54 65537465 69727563     CRYPTO_SetSecuri
3000d840:	6f4d7974 44416564 00000000 00524444     tyModeAD....DDR.
3000d850:	2d73255b 44205d45 53205244 20657a69     [%s-E] DDR Size 
3000d860:	6e207369 6320746f 69666e6f 65727567     is not configure
3000d870:	00000a64 2d73255b 3e205d45 44415b20     d...[%s-E] > [AD
3000d880:	52205244 50414d45 52524520 3a5d524f     DR REMAP ERROR]:
3000d890:	6c6f6320 65725f75 5f70616d 73746962      colu_remap_bits
3000d8a0:	203d2120 6d617264 6c6f635f 69625f75      != dram_colu_bi
3000d8b0:	21217374 00000a21 2d73255b 3e205d45     ts!!!...[%s-E] >
3000d8c0:	44415b20 52205244 50414d45 52524520      [ADDR REMAP ERR
3000d8d0:	3a5d524f 6e616220 65725f6b 5f70616d     OR]: bank_remap_
3000d8e0:	73746962 203d2120 6d617264 6e61625f     bits != dram_ban
3000d8f0:	69625f6b 21217374 00000a21              k_bits!!!...

3000d8fc <__FUNCTION__.0>:
3000d8fc:	33697872 445f3631 72536e79 6e695f65     rxi316_DynSre_in
3000d90c:	00007469                                it..

3000d910 <__FUNCTION__.1>:
3000d910:	5f525753 5f4d454d 65646f4d 7465535f     SWR_MEM_Mode_Set
3000d920:	00000000 00344d4b 00304d4b 3d3d3d3d     ....KM4.KM0.====
3000d930:	3d3d3d3d 73253d3d 61745320 44206b63     ======%s Stack D
3000d940:	20706d75 3d3d3d3d 3d3d3d3d 000a3d3d     ump ==========..
3000d950:	72727543 20746e65 63617453 6f50206b     Current Stack Po
3000d960:	65746e69 203d2072 70257830 6e61202c     inter = 0x%p, an
3000d970:	75642064 7320706d 6b636174 70656420     d dump stack dep
3000d980:	3d206874 0d642520 0000000a 3830255b     th = %d.....[%08
3000d990:	00205d78 78383025 00000020 255b0a0d     x] .%08x .....[%
3000d9a0:	5d783830 00000020 00003052 00003452     08x] ...R0..R4..
3000d9b0:	3d3d3d3d 3d3d3d3d 43203d3d 68736172     ========== Crash
3000d9c0:	6d754420 3d3d2070 3d3d3d3d 3d3d3d3d      Dump ==========
3000d9d0:	0000000a 3d3d3d3d 3d3d3d3d 52203d3d     ....========== R
3000d9e0:	73696765 20726574 706d7544 3d3d3d20     egister Dump ===
3000d9f0:	3d3d3d3d 0a3d3d3d 00000000 4c20205b     =======.....[  L
3000da00:	30205d52 78382578 0000000a 5020205b     R] 0x%8x....[  P
3000da10:	30205d43 78382578 0000000a 5350785b     C] 0x%8x....[xPS
3000da20:	30205d52 78382578 0000000a 4358455b     R] 0x%8x....[EXC
3000da30:	30205d52 78382578 0000000a 7325205b     R] 0x%8x....[ %s
3000da40:	7830205d 0a783825 00000000 00323152     ] 0x%8x.....R12.
3000da50:	3d3d3d3d 3d3d3d3d 45203d3d 6f20646e     ========== End o
3000da60:	72432066 20687361 706d7544 3d3d3d20     f Crash Dump ===
3000da70:	3d3d3d3d 0a3d3d3d 00000000 0a0d0a0d     =======.........
3000da80:	00000000                                ....

3000da84 <register_string>:
3000da84:	00000000 3000d9ac 3000c688 3000c68c     .......0...0...0
3000da94:	3000c690 3000c694 3000c698 3000c69c     ...0...0...0...0
3000daa4:	3000c6a0 3000d9a8 3000c6a4 3000c6a8     ...0...0...0...0
3000dab4:	3000c6ac 3000da4c 65637845 6f697470     ...0L..0Exceptio
3000dac4:	6163206e 74686775 206e6f20 000a7025     n caught on %p..
3000dad4:	64726148 75616620 6920746c 61632073     Hard fault is ca
3000dae4:	64657375 20796220 6c696166 76206465     used by failed v
3000daf4:	6f746365 65662072 00686374 52534648     ector fetch.HFSR
3000db04:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000db14:	00000000 75636553 79746972 75616620     ....Security fau
3000db24:	6920746c 61632073 64657375 20796220     lt is caused by 
3000db34:	61766e49 2064696c 72746e65 6f702079     Invalid entry po
3000db44:	00746e69 52534653 305b203a 78382578     int.SFSR: [0x%8x
3000db54:	3e2d205d 0a732520 00000000 75636553     ] -> %s.....Secu
3000db64:	79746972 75616620 6920746c 61632073     rity fault is ca
3000db74:	64657375 20796220 61766e49 2064696c     used by Invalid 
3000db84:	65746e69 74697267 69732079 74616e67     integrity signat
3000db94:	00657275 75636553 79746972 75616620     ure.Security fau
3000dba4:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dbb4:	61766e49 2064696c 65637865 6f697470     Invalid exceptio
3000dbc4:	6572206e 6e727574 00000000 75636553     n return....Secu
3000dbd4:	79746972 75616620 6920746c 61632073     rity fault is ca
3000dbe4:	64657375 20796220 72747441 74756269     used by Attribut
3000dbf4:	206e6f69 74696e75 6f697620 6974616c     ion unit violati
3000dc04:	00006e6f 75636553 79746972 75616620     on..Security fau
3000dc14:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dc24:	61766e49 2064696c 6e617274 69746973     Invalid transiti
3000dc34:	00006e6f 75636553 79746972 75616620     on..Security fau
3000dc44:	6920746c 61632073 64657375 20796220     lt is caused by 
3000dc54:	797a614c 61747320 70206574 65736572     Lazy state prese
3000dc64:	74617672 206e6f69 6f727265 00000072     rvation error...
3000dc74:	75636553 79746972 75616620 6920746c     Security fault i
3000dc84:	61632073 64657375 20796220 797a614c     s caused by Lazy
3000dc94:	61747320 65206574 726f7272 00000000      state error....
3000dca4:	20656854 75636573 79746972 75616620     The security fau
3000dcb4:	6f20746c 72756363 20646572 72646461     lt occurred addr
3000dcc4:	20737365 00007369 52414653 305b203a     ess is..SFAR: [0
3000dcd4:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000dce4:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000dcf4:	61662074 20746c75 63207369 65737561     t fault is cause
3000dd04:	79622064 736e6920 63757274 6e6f6974     d by instruction
3000dd14:	63636120 20737365 6c6f6976 6f697461      access violatio
3000dd24:	0000006e 53464d4d 5b203a52 38257830     n...MMFSR: [0x%8
3000dd34:	2d205d78 7325203e 0000000a 6f6d654d     x] -> %s....Memo
3000dd44:	6d207972 67616e61 6e656d65 61662074     ry management fa
3000dd54:	20746c75 63207369 65737561 79622064     ult is caused by
3000dd64:	74616420 63612061 73736563 6f697620      data access vio
3000dd74:	6974616c 00006e6f 6f6d654d 6d207972     lation..Memory m
3000dd84:	67616e61 6e656d65 61662074 20746c75     anagement fault 
3000dd94:	63207369 65737561 79622064 736e7520     is caused by uns
3000dda4:	6b636174 20676e69 6f727265 00000072     tacking error...
3000ddb4:	6f6d654d 6d207972 67616e61 6e656d65     Memory managemen
3000ddc4:	61662074 20746c75 63207369 65737561     t fault is cause
3000ddd4:	79622064 61747320 6e696b63 72652067     d by stacking er
3000dde4:	00726f72 6f6d654d 6d207972 67616e61     ror.Memory manag
3000ddf4:	6e656d65 61662074 20746c75 63207369     ement fault is c
3000de04:	65737561 79622064 6f6c6620 6e697461     aused by floatin
3000de14:	6f702d67 20746e69 797a616c 61747320     g-point lazy sta
3000de24:	70206574 65736572 74617672 006e6f69     te preservation.
3000de34:	20656854 6f6d656d 6d207972 67616e61     The memory manag
3000de44:	6e656d65 61662074 20746c75 7563636f     ement fault occu
3000de54:	64657272 64646120 73736572 00736920     rred address is.
3000de64:	52414d4d 305b203a 78382578 3e2d205d     MMAR: [0x%8x] ->
3000de74:	0a732520 00000000 20737542 6c756166      %s.....Bus faul
3000de84:	73692074 75616320 20646573 69207962     t is caused by i
3000de94:	7274736e 69746375 61206e6f 73656363     nstruction acces
3000dea4:	69762073 74616c6f 006e6f69 52534642     s violation.BFSR
3000deb4:	305b203a 78382578 3e2d205d 0a732520     : [0x%8x] -> %s.
3000dec4:	00000000 20737542 6c756166 73692074     ....Bus fault is
3000ded4:	75616320 20646573 70207962 69636572      caused by preci
3000dee4:	64206573 20617461 65636361 76207373     se data access v
3000def4:	616c6f69 6e6f6974 00000000 20737542     iolation....Bus 
3000df04:	6c756166 73692074 75616320 20646573     fault is caused 
3000df14:	69207962 6572706d 65736963 74616420     by imprecise dat
3000df24:	63612061 73736563 6f697620 6974616c     a access violati
3000df34:	00006e6f 20737542 6c756166 73692074     on..Bus fault is
3000df44:	75616320 20646573 75207962 6174736e      caused by unsta
3000df54:	6e696b63 72652067 00726f72 20737542     cking error.Bus 
3000df64:	6c756166 73692074 75616320 20646573     fault is caused 
3000df74:	73207962 6b636174 20676e69 6f727265     by stacking erro
3000df84:	00000072 20737542 6c756166 73692074     r...Bus fault is
3000df94:	75616320 20646573 66207962 74616f6c      caused by float
3000dfa4:	2d676e69 6e696f70 616c2074 7320797a     ing-point lazy s
3000dfb4:	65746174 65727020 76726573 6f697461     tate preservatio
3000dfc4:	0000006e 20656854 20737562 6c756166     n...The bus faul
3000dfd4:	636f2074 72727563 61206465 65726464     t occurred addre
3000dfe4:	69207373 00000073 52414642 305b203a     ss is...BFAR: [0
3000dff4:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e004:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e014:	65737561 79622064 74746120 74706d65     aused by attempt
3000e024:	6f742073 65786520 65747563 206e6120     s to execute an 
3000e034:	65646e75 656e6966 6e692064 75727473     undefined instru
3000e044:	6f697463 0000006e 52534655 305b203a     ction...UFSR: [0
3000e054:	78382578 3e2d205d 0a732520 00000000     x%8x] -> %s.....
3000e064:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e074:	65737561 79622064 74746120 74706d65     aused by attempt
3000e084:	6f742073 69777320 20686374 61206f74     s to switch to a
3000e094:	6e69206e 696c6176 74732064 20657461     n invalid state 
3000e0a4:	672e6528 41202c2e 00294d52 67617355     (e.g., ARM).Usag
3000e0b4:	61662065 20746c75 63207369 65737561     e fault is cause
3000e0c4:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e0d4:	206f6420 65206e61 70656378 6e6f6974      do an exception
3000e0e4:	74697720 20612068 20646162 756c6176      with a bad valu
3000e0f4:	6e692065 65687420 43584520 5445525f     e in the EXC_RET
3000e104:	204e5255 626d756e 00007265 67617355     URN number..Usag
3000e114:	61662065 20746c75 63207369 65737561     e fault is cause
3000e124:	79622064 74746120 74706d65 6f742073     d by attempts to
3000e134:	65786520 65747563 63206120 6f72706f      execute a copro
3000e144:	73736563 6920726f 7274736e 69746375     cessor instructi
3000e154:	00006e6f 67617355 61662065 20746c75     on..Usage fault 
3000e164:	63207369 65737561 79622064 646e6920     is caused by ind
3000e174:	74616369 74207365 20746168 74732061     icates that a st
3000e184:	206b6361 7265766f 776f6c66 61682820     ack overflow (ha
3000e194:	61776472 63206572 6b636568 61682029     rdware check) ha
3000e1a4:	61742073 206e656b 63616c70 00000065     s taken place...
3000e1b4:	67617355 61662065 20746c75 63207369     Usage fault is c
3000e1c4:	65737561 79622064 646e6920 74616369     aused by indicat
3000e1d4:	74207365 20746168 75206e61 696c616e     es that an unali
3000e1e4:	64656e67 63636120 20737365 6c756166     gned access faul
3000e1f4:	61682074 61742073 206e656b 63616c70     t has taken plac
3000e204:	00000065 67617355 61662065 20746c75     e...Usage fault 
3000e214:	63207369 65737561 79622064 646e4920     is caused by Ind
3000e224:	74616369 61207365 76696420 20656469     icates a divide 
3000e234:	7a207962 206f7265 20736168 656b6174     by zero has take
3000e244:	6c70206e 20656361 6e616328 20656220     n place (can be 
3000e254:	20746573 796c6e6f 20666920 5f564944     set only if DIV_
3000e264:	52545f30 73692050 74657320 00000029     0_TRP is set)...
3000e274:	75626544 61662067 20746c75 63207369     Debug fault is c
3000e284:	65737561 79622064 6c616820 65722074     aused by halt re
3000e294:	73657571 20646574 4e206e69 00434956     quested in NVIC.
3000e2a4:	52534644 305b203a 78382578 3e2d205d     DFSR: [0x%8x] ->
3000e2b4:	0a732520 00000000 75626544 61662067      %s.....Debug fa
3000e2c4:	20746c75 63207369 65737561 79622064     ult is caused by
3000e2d4:	504b4220 6e692054 75727473 6f697463      BKPT instructio
3000e2e4:	7865206e 74756365 00006465 75626544     n executed..Debu
3000e2f4:	61662067 20746c75 63207369 65737561     g fault is cause
3000e304:	79622064 54574420 74616d20 6f206863     d by DWT match o
3000e314:	72756363 00646572 75626544 61662067     ccurred.Debug fa
3000e324:	20746c75 63207369 65737561 79622064     ult is caused by
3000e334:	63655620 20726f74 63746566 636f2068      Vector fetch oc
3000e344:	72727563 00006465 75626544 61662067     curred..Debug fa
3000e354:	20746c75 63207369 65737561 79622064     ult is caused by
3000e364:	42444520 20515247 6e676973 61206c61      EDBGRQ signal a
3000e374:	72657373 00646574 00000208 0000020c     sserted.........
3000e384:	00000210 00000008 00000214 00000218     ................
3000e394:	0000021c 0000000c                       ........

3000e39c <__FUNCTION__.0>:
3000e39c:	544f4f42 434f535f 6b6c435f 00746553     BOOT_SOC_ClkSet.
3000e3ac:	3000ce94 3000cea0 3000ceac 3000ceb8     ...0...0...0...0
3000e3bc:	3000cec4 3000ced0 3000cedc 3000cee8     ...0...0...0...0
3000e3cc:	3000cef4 3000cf00 3000d0a8 3000d0b4     ...0...0...0...0
3000e3dc:	3000d0bc 3000d0c4 3000d0ac 3000d0cc     ...0...0...0...0

3000e3ec <km4_bootloader_rev>:
3000e3ec:	5f346d6b 746f6f62 64616f6c 765f7265     km4_bootloader_v
3000e3fc:	615f7265 32323235 30636132 30325f31     er_a52222ac01_20
3000e40c:	302f3432 38302f38 3a30312d 323a3733     24/08/08-10:37:2
3000e41c:	4f420030 5b00544f 572d7325 2a2a205d     0.BOOT.[%s-W] **
3000e42c:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e43c:	2a2a2a2a 2a2a2a2a 52415720 474e494e     ******** WARNING
3000e44c:	2a2a2a20 2a2a2a2a 2a2a2a2a 2a2a2a2a      ***************
3000e45c:	2a2a2a2a 2a2a2a2a 255b000a 5d572d73     ********..[%s-W]
3000e46c:	20202a20 20202020 20202020 20202020      *              
3000e47c:	20202020 20202020 20202020 20202020                     
3000e48c:	20202020 20202020 20202020 20202020                     
3000e49c:	20202020 20202020 0a2a2020 73255b00               *..[%s
3000e4ac:	205d572d 2020202a 20202020 20202020     -W] *           
3000e4bc:	75432020 6e657272 68632074 76207069       Current chip v
3000e4cc:	69737265 69206e6f 43412073 21217475     ersion is ACut!!
3000e4dc:	20202021 20202020 20202020 000a2a20     !            *..
3000e4ec:	2d73255b 2a205d57 52202020 53204b54     [%s-W] *   RTK S
3000e4fc:	64204b44 2073656f 20746f6e 70707573     DK does not supp
3000e50c:	2074726f 72727563 20746e65 73726576     ort current vers
3000e51c:	206e6f69 20796e61 65726f6d 2020202e     ion any more.   
3000e52c:	5b000a2a 572d7325 202a205d 20202020     *..[%s-W] *     
3000e53c:	656c5020 20657361 746e6f63 20746361      Please contact 
3000e54c:	204b5452 66204d50 6d20726f 2065726f     RTK PM for more 
3000e55c:	646f7270 20746375 6f666e69 20202021     product info!   
3000e56c:	2a202020 255b000a 5d572d73 2a2a2a20        *..[%s-W] ***
3000e57c:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e58c:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e59c:	2a2a2a2a 2a2a2a2a 2a2a2a2a 2a2a2a2a     ****************
3000e5ac:	2a2a2a2a 0a2a2a2a 73255b00 205d572d     *******..[%s-W] 
3000e5bc:	0043000a 49534f50 002e0058              ..C.POSIX...

3000e5c8 <_ctype_>:
3000e5c8:	20202000 20202020 28282020 20282828     .         ((((( 
3000e5d8:	20202020 20202020 20202020 20202020                     
3000e5e8:	10108820 10101010 10101010 10101010      ...............
3000e5f8:	04040410 04040404 10040404 10101010     ................
3000e608:	41411010 41414141 01010101 01010101     ..AAAAAA........
3000e618:	01010101 01010101 01010101 10101010     ................
3000e628:	42421010 42424242 02020202 02020202     ..BBBBBB........
3000e638:	02020202 02020202 02020202 10101010     ................
3000e648:	00000020 00000000 00000000 00000000      ...............
	...
